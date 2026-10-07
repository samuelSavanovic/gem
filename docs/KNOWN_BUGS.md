# Known bugs

Bugs found and not yet fixed. Each entry has a minimal repro (checked
against `build/gem`), what goes wrong, and where the code is. A fix deletes
its entry in the same change, along with any **(bug)** rule in
`docs/BEST_PRACTICES.md` that exists because of it. Performance problems go in
`docs/OPTIMIZATIONS.md`, missing features in `docs/ROADMAP.md`.

## Compiler

### A param default that names its own or a later param

```gem
fn h(x = x) x end
print(h())             # prints whatever the C local held (nil here)
```

```gem
fn k(a = b, b = 1) a end
print(k())             # cc fails: 'gem_v_b' undeclared
```

```gem
let D = 5
fn f(x = D, D = 1) x end
print(f())             # cc fails: 'gem_v_D' undeclared (a later param
                       # shadows the module binding the default names)
```

A default runs in the fn's scope with every param visible, so `x = x`
reads the param before it is set (an uninitialized C local, `gem_v_x =
gem_v_x;`) and a later param reaches C undeclared. SPEC says a default
may refer to *earlier* params; the others should be a compile error
(`undeclared identifier`), or `x = x` should read the binding the param
shadows, as a `let` initializer does. Param defaults are emitted in
compiler/codegen.gem (the `if (argc > i) ... else` prelude); the scope
check belongs in `scope_shadowing_lets`.

### An error in a param default is reported without the fn

```gem
fn f(s, n = len(s))
  print(n)
end
f(5)
```

```
[Runtime Error]: len: expected string, table, or buffer, got int
  --> kb2.gem:4
   |
 4 | f(5)
   |
Stack trace:
  at main (kb2.gem:4)
```

A fn's param defaults run before it records itself on the call stack, so
an error raised in a default (or in a fn a default calls) shows the
caller on top, at the call site, and `f` is missing from the trace and
from pcall's `stack`. The default runs because `f` was called: `f` should
be on top, at the line of its `fn` header. When a self tail call reruns
the defaults (`_tco_rebind`), the frame is there, but its line is the
tail call's, not the default's. Leaf fns behave the same as framed ones
(`examples/207_leaf_builtins.gem`, section 6, which prints the current
traces). The param prelude (`if (argc > i) ... else`) is emitted before
`gem_push_frame` / `leaf_entry_str` in `compile_fn` and
`compile_closure_fn` (compiler/codegen.gem); moving the frame push above
it, with a line set before each default, would put the defaults inside
the fn. The frame push's stack-overflow check would then run before the
defaults too.

### An extern fn returning a `const char *` makes cc warn

```gem
extern include "netdb.h"
extern fn hstrerror(err: Int) -> String
print(hstrerror(1))     # Unknown host, after a cc warning on every build:
                        # initialization discards 'const' qualifier ...
```

The wrapper of a plain `extern fn ... -> String` stores the result in a
`char *` (`char* _ret = ...` in the extern wrapper emitted by
compiler/codegen.gem), so any C function that returns `const char *` (libc's
`hstrerror`, `gai_strerror`, a helper returning a string literal) gets a
`-Wdiscarded-qualifiers` warning printed at every compile. The runtime only
copies the string, so the plain wrapper can use `const char *`; the
`extern blocking fn` wrapper frees it and should keep `char *`.

### A file ending in `x.` crashes the parser

```sh
printf 's.' > a.gem      # no trailing newline
build/gem --check a.gem  # [Runtime Error]: field access on non-table: got nil
                         #   --> compiler/parser.gem:884
```

The "expected field name after '.'" path in the postfix loop
(compiler/parser.gem, `report(..., "expected field name after '.', ...")`)
calls `advance()` even when the token after the dot is EOF, so the next
`peek()` returns nil. The LSP parses every buffer as it is typed, so typing
`x.` at the end of a file crashes the document's process and requests wait
about 5 s for it. It should report the error and not consume EOF.

### A malformed triple-quoted string crashes the lexer

```sh
printf 'let s = """{"\n """}\n ' > a.gem   # trailing space, no newline
build/gem --check a.gem   # [Runtime Error]: string index out of bounds
                          #   --> compiler/lexer.gem:776
```

After `tq_dedent` skips the indentation of the last line, the triple-quote
loop in compiler/lexer.gem reads `source[pos]` (`let tqrc = source[pos]`)
without checking `pos < length`. It should report `unterminated
triple-quoted string`.

### Deep nesting overflows the compiler's own stack

```sh
python3 -c "print('print(' + '('*1500 + '1' + ')'*1500 + ')')" > a.gem
build/gem --check a.gem   # [Runtime Error]: stack overflow in anonymous fn
                          #   --> compiler/parser.gem:744
python3 -c "print('let s = ' + ' + '.join(['\"a\"']*8000))" > b.gem
build/gem --check b.gem   # [Runtime Error]: stack overflow in walk_ast
                          #   --> compiler/main.gem:737
```

The parser and the AST walks recurse once per nesting level (a binary
operator chain nests once per operand), so about 1,000 nested parens or
8,000 terms of one `+` chain overflow the compiler, which prints its own
stack trace. It should report a compile error at the source position, e.g.
a nesting limit in the parser checked well below what the walks after it can
handle.

### Duplicate parameter names reach cc

```gem
fn f(a, a) a end      # error: redefinition of 'gem_v_a' (from cc)
print(f(1, 2))
```

The same for `fn(x, x)`, `fn f(a, ...a)` and a block `do |a, a|`. `--check`
accepts them. The parser (or `scope_shadowing_lets`) should report
`duplicate parameter 'a'`.

### A name repeated in a pattern binds the last value

```gem
match [1, 2]
when [x, x] then print("same", x)    # prints same 2
else print("differ")
end
let [a, a] = [1, 2]                  # a is 2
fn g(x, {x}) x end                   # g(1, {x: 2}) is 2
```

A pattern binding a name twice reads as an equality test (as in Erlang) but
silently binds the last match, in `match`, `receive`, destructuring `let`,
`for k, k in` and destructured params. SPEC doesn't say what it means; it
should be a compile error (`'x' is bound twice in this pattern`), which
leaves equality to a `^x` pin or a guard. Patterns are lowered in
compiler/lower.gem.

### `export` of an undefined name is ignored in the entry file

```gem
fn f() 1 end
export f, nope        # compiles; in a loaded module the same line is an error
print(f())
```

A loaded module reports `module 'm' exports 'nope', which it doesn't
define`; the entry file's `export` is never checked. It should get the same
error.

### A string literal with invalid UTF-8 bytes makes cc warn

```sh
printf 'let s = "\xff\xfe"\nprint(len(s))\n' > a.gem
build/gem a.gem -o a      # warning: illegal character encoding in string
                          # literal [-Winvalid-source-encoding]
```

Codegen copies the literal's bytes into `GEM_STR_LIT("...", n)` as they are,
and clang warns on every build. Bytes from 0x80 up should be written as
octal escapes, as NUL already is.

### An `extern fn` named like a C keyword or a libc function fails in cc

```gem
extern fn int(x: Int) -> Int    # error: cannot combine with previous
                                # 'type-name' declaration specifier
```

```gem
extern fn abs(x: Int) -> Int    # error: conflicting types for 'abs'
print(abs(-3))
```

The extern's declared name is the C symbol (`c_name`), and codegen always
emits its own prototype `int64_t abs(int64_t);`. A C keyword can never name
a C function, so it should be a compile error at the `extern fn` line. A
prototype that conflicts with a system header's is a real mismatch (libc's
`abs` takes an `int`), but the user sees only cc's message about the
generated file; the build should name the extern and its line.

## Runtime

### `in` answers differently on a copy of a table whose string keys were deleted

```gem
let t = {}
t.a = 1
delete(t, "a")
push(t, "a")
let me = self()
spawn do
  send(me, t)
end
receive
when c then print("a" in t, "a" in c)   # false true
end
```

`gem_in_fn` (runtime/gem_builtins_collection.c) treats a table as an
array, and looks for the value, when its string-key index is `NULL`, and
as a map, looking for the key, otherwise. Deleting the last string key
leaves an empty index; a copy rebuilds the index lazily and gets `NULL`.
Decide on the table's keys, not on the index.

### sqlite: SQL after an embedded NUL is ignored

```gem
let db = sqlite_open(":memory:")
sqlite_exec(db, "CREATE TABLE a(x)")
sqlite_exec(db, "INSERT INTO a VALUES (1);\0INSERT INTO a VALUES (2)")
print(sqlite_query(db, "SELECT count(*) AS n FROM a", []))   # [{n: 1}]
```

sqlite's parser stops at a NUL even when given the full length, so
`sqlite_exec` runs only what comes before it, and `sqlite_query` doesn't
see a second statement after one. Both should raise on SQL containing a
NUL (runtime/gem_builtins_sqlite.c).

### `read_file` and `list_dir` don't say why a path can't be opened

```gem
print((pcall read_file("/no/such/file")).error)   # read_file: cannot open '/no/such/file'
print((pcall list_dir("/etc/passwd")).error)      # list_dir: cannot open directory '/etc/passwd'
```

The messages drop `errno`, so a program can't tell a missing file from a
permission problem or report it the way other tools do (`No such file or
directory`, `Not a directory`, `Permission denied`); `examples/gemgrep`
opens the file again from C (`fs_open_error` in `fs.h`) to get
`strerror`'s text. The messages are made in `gem_read_whole_file` in
runtime/gem_threadpool.c (on the worker thread, where `errno` is still
set) and in `gem_list_dir_fn` in runtime/gem_builtins_io.c; `write_file`
and `append_file` do the same. Append `strerror(errno)` where the call
fails.

### `print` and `eprint` stop at the first NUL

```gem
print("a\0b")          # writes "a\n"
let s = "a\0b"
print("x{s}y")         # writes "xa\n"
eprint("e\0f")         # writes "e\n" to stderr
```

Strings are binary-safe and `write_stdout` writes them whole, but `gem_print`
and `gem_eprint_fn` (runtime/gem_builtins_core.c) write strings with
`printf("%s", ...)` / `fputs`, which stop at a NUL. They should write
`slen` bytes with `fwrite`.

### Integer `pow` is computed in `double`; `to_int` of an out-of-range float is undefined

```gem
print(pow(3, 39))                   # 4052555153018976256 (exact: 4052555153018976267)
print(pow(2, 64))                   # 9223372036854775807; int arithmetic wraps
print(to_int(to_float("1e300")))    # 9223372036854775807 on arm64
print(to_int(to_float("nan")))      # 0 on arm64
```

`gem_pow_fn` (runtime/gem_builtins_math.c) computes int ** int with C's
`pow` and casts back, which loses precision above 2^53 and clamps instead of
wrapping like `*` (SPEC "Integers"). It should use integer
exponentiation by squaring when both operands are ints and the exponent is
non-negative. `gem_to_int_fn` (runtime/gem_builtins_core.c) casts any
double to `int64_t`, which is undefined behaviour in C for NaN, infinities
and values out of range (arm64 clamps, x86 gives INT64_MIN); it should
raise.

### NaN and nil are accepted as table keys

```gem
let t = {}
let nan = to_float("nan")
for i = 0, 5
  t[nan] = i
end
print(len(t), has_key(t, nan))     # 5 false: five keys no lookup can reach
t[nil] = 1
print(len(t))                      # 6
```

NaN never equals itself, so each `t[nan] = v` adds an entry that `t[nan]`,
`has_key` and `delete` can't find, and a long-lived table grows without
bound. A nil key is stored too, although `t[nil]` reads like a missing key.
Both should raise (`table key is nil` / `table key is NaN`), as Lua does, in
`gem_table_set` (runtime/gem_core.c).

### `substr` with a huge count aborts the process

```gem
print(pcall substr("abc", 1, 9223372036854775807))
# gem_arena: mmap failed (size=9223372036854792192), process exits
```

`start + count > slen` in `gem_substr_fn` (runtime/gem_builtins_string.c)
overflows, so the clamp is skipped and the allocation asks for 2^63 bytes,
which pcall can't catch. Compare `count > slen - start` instead.

### `chr` of a value outside 0–255 wraps

```gem
print(ord(chr(300)), len(chr(-1)))     # 44 1
print(chr(256) == "\0")                # true
```

`gem_chr_fn` (runtime/gem_builtins_string.c) keeps the low byte. A code
point above 255 or a negative value should raise (`chr: expected 0..255,
got 300`).

### `sort` ignores a comparator that isn't a fn

```gem
let a = [3, 1, 2]
sort(a, 5)
print(a)       # [1, 2, 3]: sorted by the default order, no error
```

`gem_sort_fn` (runtime/gem_builtins_collection.c) uses the comparator only
when it is a fn. Any other non-nil second argument should raise.

### `in` blames the wrong operand

```gem
print("x" in "xyz")    # in: expected table as first argument, got string
```

`x in t` calls `gem_in_fn` with the table first, so its message (runtime/
gem_builtins_collection.c) calls the right-hand operand the first argument.
It should say `in: right operand must be a table, got string`.

### `tcp_listen` and `tcp_connect` truncate the port to 16 bits

```gem
let l = tcp_listen("127.0.0.1", 70000)   # listens on 4464
let c = pcall tcp_connect("127.0.0.1", 70000)   # connects to 4464
```

```gem
load "std/http"
let h = http.start(http.router(), {port: 70000, host: "127.0.0.1"})  # starts, listens on 4464
let h2 = http.start(http.router(), {port: -1, host: "127.0.0.1"})    # starts too
```

`runtime/gem_builtins_tcp.c` casts the port with `htons((uint16_t)port)`.
Both builtins should raise for a port outside 0–65535, and `http.start`
should check it first and raise `http.start: port must be an int from 0 to
65535`.

### Sockets are inherited by `exec`'d commands

```gem
let l = tcp_listen("127.0.0.1", 47123)
exec("lsof -nP -iTCP:47123 -sTCP:LISTEN")   # lists the shell and lsof too
```

`socket()` and `accept()` in runtime/gem_builtins_tcp.c don't set
close-on-exec, so every command `exec` starts holds the program's sockets.
A child that outlives the program keeps its listening port bound. Set
`FD_CLOEXEC` on every socket the runtime creates.

### Field access and method call errors don't name the field

```gem
let t = {a: 1}
print((pcall t.a.b).error)    # field access on non-table: got int
print((pcall t.zz()).error)   # attempt to call nil value
```

The message says what went wrong but not where in the expression: with
`req.body.user.name` a reader can't tell which link was the int, and
`t.zz()` (a typo in a method name, the most common cause) doesn't say `zz`.
Codegen knows the field name at each `.` and each call through a field; the
messages should be `field access .b on int` and `attempt to call nil (field
zz)`. The runtime messages come from `gem.h` (`field access on
non-table`) and `gem_builtins_core.c` (`attempt to call`).

## Standard library

### `request` loses a status the server sent before the body was written

```gem
load "std/request"
load "std/string"
let l = tcp_listen("127.0.0.1", 18298)
spawn do
  let c = tcp_accept(l)
  tcp_read(c, 4096, 1000)
  tcp_write(c, "HTTP/1.1 413 Payload Too Large\r\nContent-Length: 0\r\nConnection: close\r\n\r\n")
  tcp_close(c)
end
let body = string.repeat("x", 32 * 1024 * 1024)
print((pcall request.post("http://127.0.0.1:18298/up", {body: body})).error)
# request.post: connection lost while sending the request to ...
```

A server that answers early (413, 401) and closes while the body is still
being sent leaves a short `tcp_write`, which `exchange` (std/request.gem)
turns into "connection lost while sending"; the status the server already
sent is never read. On a short write it should still try to read a
response before raising.

### `json.encode` writes infinite and NaN floats as bare `inf`/`nan`

```gem
load "std/json"
let inf = to_float("inf")
print(json.encode([inf - inf, inf]))         # [nan,inf]: not JSON
print(pcall json.parse(json.encode(inf)))    # error: unexpected character 'i' at byte 0
```

`encode` (std/json.gem, the `int`/`float`/`bool` branch) writes floats with
`to_string`, so non-finite values produce output no JSON parser accepts,
`json.parse` included. It should raise (as JSON.stringify-style encoders that
reject them do) or write `null`.

### `http` static files: the request path is not percent-decoded

```gem
load "std/http"
load "std/request"
# public/my file.txt exists
let app = http.router()
app.static("/", "public")
let s = http.start(app, {port: 18931, host: "127.0.0.1"})
print(request.get("http://127.0.0.1:18931/my%20file.txt").status)   # 404
```

`r.static` (std/http.gem) joins the raw request path to `dir`, so a file
whose name needs percent-encoding can't be served, while route `:params` are
decoded. It should decode the path first and apply the `..` check to the
decoded path.

### `sort` and `table.sort` on a record turn it into an array

```gem
let rec = {b: 2, a: 1}
sort(rec)
print(rec)            # [1, 2]: the keys are gone
```

The `sort` builtin (runtime/gem_builtins_collection.c) accepts any table
and rewrites its values as entries 0 .. n-1; `table.sort` (std/table.gem)
only checks that `arr` is a table, so it does the same. With a comparator,
`table.sort` first checks `cmp(arr[0], arr[1])`, which on a record is
`cmp(nil, nil)`, so a typical `a - b` comparator raises `type error in -: got
nil and nil` from the caller's code instead. Both should raise for a table
that isn't an array.

### Some std errors don't name the function called

```gem
load "std/http"
load "std/time"
load "std/sqlite"
load "std/request"
pcall http.html_escape(5)       # "str_replace: all arguments must be strings, got int, string, string"
pcall http.parse_form(nil)      # "url.parse_query: s must be a string, got nil"
pcall time.date(nil)            # "format_time: expected (int, string), got (nil, string)"
pcall sqlite.last_id(999)       # "sqlite_last_insert_id: not an open database handle"
pcall request.post("http://127.0.0.1:1/", "x", "opts")   # "field access on non-table: got string"
pcall math.min([4, 2])          # "type error in <: got table and nil"
pcall http.start(http.router(), nil)                   # "field access on non-table: got nil"
```

The convention (BEST_PRACTICES, "Prefix messages with where they came
from") is `<module>.<fn>: ...`. These functions pass arguments straight to a
builtin or another module without checking them first, so the message names
the callee. std/sqlite uses the builtins' names for every error, which for
`last_id` differs from the std name.

### The spawned-module-state note fires for std's per-process settings

```gem
load "std/log"
let p = spawn do
  log.set_level("debug")      # what std/log's docs recommend
  log.debug("hi")
end
```

`gem --check` prints a note starting `note: std/log.gem:38: this changes
module-level` and naming `log.min_level`, "in code that runs in a spawned
process". The same
happens for `test.case` in a spawned process (`std/test.gem`). The write is
the intended per-process behaviour and documented, yet the note fires, and it
points at a std line and names a private variable, not the user's call.
`note_spawn_global_writes` (compiler/codegen.gem) should attribute the note to
the call site in the user's file, or skip writes inside std whose
per-process meaning the module documents.

### `http` sends a nil or table header value as the text `nil`

```gem
load "std/http"
let app = http.router()
app.get("/n") do |req|
  let h = {}
  h["X-Nil"] = nil
  h["X-T"] = {a: 1}
  http.response(200, h, "x")   # sent as "X-Nil: nil" and "X-T: nil"
end
app.get("/c") do |req|
  http.set_cookie(http.ok("x"), "a", nil)                  # sent as "Set-Cookie: a=nil; ..."
end
```

The response check in std/http.gem validates header names and CR/LF in
values but interpolates any value, so nil becomes `nil` and a record is
taken for an empty-ish array. std/request raises `header X has a nil value`
for the same input. The server should treat these as an invalid response
(500), and `set_cookie` should raise on a non-string value.

### `test.assert_throws` passes when its body isn't a fn

```gem
load "std/test"
print(test.assert_throws(42))     # "attempt to call int value": the assert passes
```

`assert_throws` (std/test.gem) calls `body` under pcall, so a non-fn body
raises inside the protection and counts as the expected error; a typo makes
a test pass. It should raise `test.assert_throws: body must be a fn`.

### `string.join` rejects an array whose keys were added out of order

```gem
load "std/string"
load "std/json"
let t = {}
t[1] = "b"
t[0] = "a"
print(json.encode(t))                    # ["a","b"]
print(pcall string.join(t, ","))         # error: arr must be an array, got a table with key 1
```

`join` (std/string.gem) requires the iteration order to be 0, 1, 2, ...,
while `table.*`, `json.encode` and array patterns accept any table whose keys
are exactly 0 .. n-1. std should share one definition of an array; the error
also names a valid key.

## Language server

### A malformed message ends the server

```sh
printf 'Content-Length: 5\r\n\r\nnull}' | build/gem lsp
# [Runtime Error]: json.parse: unexpected data after JSON value at byte 4
```

The main loop in lsp/server.gem runs `rpc.read_message()` and
`d.dispatch(msg)` unprotected, so any error in reading or handling one
message ends the server and the editor loses the session. Besides bad
JSON, a body that isn't an object (`42`, `null`), a missing or non-numeric
`Content-Length`, `params: null`, a `position` without `character`, a
`didChange` without `contentChanges`, `didClose` without a `textDocument`,
a non-string `uri`, and a message with an `id` but no `method` all do it.
Each message should be handled under `pcall`: a request that fails gets a
JSON-RPC error response (`-32700` parse error, `-32600` invalid request,
`-32602` invalid params, `-32603` internal error), a notification is
dropped, and the loop goes on. A framing error the reader can't recover
from (no `Content-Length`) can still end the server, with a message on
stderr.

## Example programs

### gemgrep: a backreference with `-w` or `-x` names the wrong group

```sh
cd examples/gemgrep && ../../build/gem main.gem -o /tmp/gemgrep
printf 'aa\na\n' | /tmp/gemgrep -w '(a)\1'    # prints a (GNU grep: aa)
printf 'aa\na\n' | /tmp/gemgrep -x '(a)\1'    # gemgrep: Invalid back reference (GNU grep: aa)
```

`regex.compile` (examples/gemgrep/regex.gem) wraps the pattern in groups
of its own for `-w` (`(^|[^[:alnum:]_])(PATTERN)([^[:alnum:]_]|$)`) and
`-x` (`^(PATTERN)$`), which come first, so the user's `\1` names the
wrapper's group. The wrapper should renumber the pattern's backreferences
by the groups it adds before it, and refuse one that would pass `\9`.
