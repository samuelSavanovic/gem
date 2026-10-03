# Known bugs

Bugs found and not yet fixed. Each entry has a minimal repro (checked
against `build/gem`), what goes wrong, and where the code is. A fix deletes
its entry in the same change, along with any **(bug)** rule in
`docs/BEST_PRACTICES.md` that exists because of it. Performance problems go in
`docs/OPTIMIZATIONS.md`, missing features in `docs/ROADMAP.md`.

## Compiler

### An assignment as a call argument crashes the compiler

```gem
fn f(x) x end
let a = nil
print(f(a = 1))
```

reports `[Compiler Bug]: unknown expression node: assign`. The parser
accepts an assignment where an expression is expected; it should be a
compile error at the `=`. (compiler/parser.gem, codegen's
`compile_expr`)

### Files with CRLF line endings don't lex

`printf 'let b = "q"\r\nprint(b)\r\n' > x.gem; gem x.gem` reports
`unexpected character` at the `\r`; a `"""` followed by `\r\n` reports
`triple-quoted string must be followed by a newline`. The lexer
(compiler/lexer.gem) should treat `\r\n` as a newline everywhere.

### `0..n` lexes as a float and fails at runtime

```gem
for i in 0..len("ab")
  print(i)
end
```

fails at runtime with `field access on non-table: got float`: `0.` lexes
as a float and `.len(...)` as a field access. A number followed by `..`
should be a compile error (Gem's range loop is `for i = 0, n`).

### A destructuring `let` accepts a non-name field

`let {"a"} = {a: 1}` (or `let {1} = ...`) compiles and runs with no
error and binds nothing. The field loops of `let {...}` and `{...}`
param patterns in compiler/parser.gem take any token as a field name;
they should report `expected a field name`.

### Only a module's last `export` statement counts

A module with `fn a() 1 end`, `export a`, `fn b() 2 end`, `export b`
exports only `b`: `m.a()` reports `module `two` has no export `a``.
`find_export_node` in compiler/main.gem takes the last one. Merge them,
or report a second `export` as an error.

### Error columns for nested named fns and private module fields

`fn inner() ... end` inside a fn body or closure reports "named fn inside
function body is not supported" at column 1 instead of the `fn` token.
`print(ok.priv())`, where module `ok` doesn't export `priv`, reports "has
no export" at column 1 with the span of `print`, not at `priv`.

### A fn named `<f>_body` clashes with a mutual-recursion helper

```gem
fn f(n)
  if n == 0 then return 0 end
  g(n - 1)
end
fn g(n)
  f(n)
end
fn f_body() 1 end
print(f(3), f_body())
```

fails in cc with `redefinition of 'gem_fn_f_body'`: each fn of a mutual
tail-call cycle gets a C helper `gem_fn_<name>_body` (`scc_wrapper_for` in
compiler/codegen.gem), which a user fn named `<name>_body` also gets as
its symbol. Modules hit it too: `a` with `f`↔`g` next to a module `a_f`
with `fn body` (`module_mangle` in compiler/main.gem doesn't account for
the `_body` suffix). Give the helper a name no user fn can have.

### Closures in a destructuring `let` of a builtin's name see the new binding

```gem
let [len, size] = [fn(x) 0 end, fn(x) len(x) end]
print(size("abc"))        # 0, not 3
```

SPEC says the initializer of a module-level `let` that shadows a builtin,
closures in it included, still reaches the builtin, and it does for a
plain `let len = ...` and for a direct call in the destructuring
initializer (`let [len, n] = [f, len("abc")]` binds `n = 3`). A closure in
a destructuring initializer calls the new `len` instead, in the entry file
and in a loaded module alike. The initializer scope for a shadowing
module-level `let` is set up in `rename_stmt_in_scope` /
`pending_builtin_lets` (compiler/main.gem); the destructuring form doesn't
get it for closures.

### A field access in a call's arguments runs before the arguments left of it

```gem
let log = []
fn f(x)
  push(log, x)
  {v: x}
end
print(f(1).v, f(2), f(3).v)
print(log)                       # [1, 3, 2]: f(3) ran before f(2)
```

Arguments are evaluated left to right, except that the object of a field
access (`f(3)` in `f(3).v`, `process_info(p)` in
`process_info(p).monitors`) is hoisted into a temporary before the
argument array, so it runs before the plain arguments left of it
(`print(monitor(p), process_info(p).monitors)` shows the monitors from
before the `monitor`). Codegen builds the args as a C initializer list
(`GemVal _t[] = {...}`) after hoisting the field object
(compiler/codegen.gem, the `gem_table_get_cached` path); hoist every
argument in order, or none.

### A float key in a table literal becomes a string key

```gem
let t = {1.5: "e"}
print(has_key(t, "1.5"), has_key(t, 1.5))   # true false
```

The parser keeps a NUMBER key's text, and `compile_table` emits it as a
string key; a pattern `{1.5: x}` does the same (compiler/parser.gem
`parse_int_key` handles only ints). It should be a float key or a
compile error.

### A default parameter in a loaded module can't use the module's `let`s

```gem
# lib.gem                           # main.gem
let D = 5                           load "./lib"
fn f(x = D)                         print(lib.f())
  x
end
export f
```

fails with ``undeclared identifier `D` `` at `lib.gem:2`; the same code in
the entry file prints `5`. `rename_node` (compiler/main.gem) prefixes the
module's top-level names in fn bodies but never walks the param defaults
(`node.defaults`), so the default still names `D` while the slot is
`_mod_lib_D`. std/http writes `ok`'s default content type out as a literal
because of it.

### A newline inside a `"..."` or `'...'` string isn't counted in line numbers

```gem
let s = "a
  b"
print(1)
error("x")        # reported at line 3, shows the line `print(1)`
```

Compile errors (`unexpected character`, `undeclared identifier`) and runtime
error locations after such a string are one line early per newline inside
it. Triple-quoted strings count correctly. The single- and double-quoted
string branches of the lexer (compiler/lexer.gem, from `if ch == "\""`) step
over a raw `\n` without incrementing `line` or resetting `line_start`.

### An unterminated `{` in a string is reported past the end of the line

```gem
let r = f(u, "{not json", {headers: {"Content-Type": "application/json"}, timeout_ms: 1000})
```

`unterminated string interpolation` points at a column past the end of the
line (131 on a 105-character line; longer files give columns in the
hundreds or thousands), not at the `{` that opened it. The lexer
(compiler/lexer.gem, the `unterminated string interpolation` report near
line 825) uses the scan position after searching on for the `}`, not the
position of the `{`. The note "this '{' is never closed" form at line 858
already has the right location.

## Runtime

### `INT64_MIN / -1` kills the program on x86-64

```gem
let m = -9223372036854775807 - 1
let d = -1
print(m / d)      # x86-64: Floating point exception, exit 136
```

`%` does the same. The constant folder hits it too: on x86-64, compiling
`print(-9223372036854775808 / -1)` crashes the compiler (exit 136). On
arm64 nothing traps (AArch64 `sdiv` doesn't): `/` gives `INT64_MIN` and
`%` gives `0`. Either way it is undefined behaviour in C. `gem_div`/
`gem_mod` in runtime/gem_ops.c and `try_fold_binop` in compiler/fold.gem
should raise (or wrap) instead. Relatedly, int `+`, `-` and `*` overflow
is signed-overflow undefined behaviour in C (no `-fwrapv`); it wraps in
practice.

### A non-integer `after` timeout is read as raw bits; a huge one expires at once

```gem
receive
after nil then print("no wait")
end
```

runs at once instead of raising: the timeout's `.ival` is read with no
type check (`compile_receive_match` in compiler/codegen.gem), so the
value's raw payload is taken as milliseconds. `nil` waits 0 ms and `true`
1 ms; a float waits its bit pattern read as an int (`1.5` is about
4.6e18 ms, i.e. forever; `0.0` is 0); a string or a table waits its
pointer value, which in practice is forever (`after "abc"` still waits
when another process exits). A non-integer timeout should raise.

An int timeout close to `INT64_MAX` times out at once instead of waiting
(practically) forever:

```gem
let t0 = time_ms()
receive
when "never" then nil
after 9223372036854775807 then print("timed out after", time_ms() - t0, "ms")
end
```

prints `timed out after 0 ms`: the deadline is computed as
`gem_now_ms() + (int64_t)ms` (`compile_receive_match` in
compiler/codegen.gem), which overflows to a negative time already past.
The runtime does the same for `send_after(pid, msg, ms)` (delivered at
once; `gem_send_after_builtin` in runtime/gem_scheduler.c) and `sleep(ms)`
(the deadline goes negative, which reads as "no deadline", so a lone main
process reports a deadlock; `gem_sleep_builtin`). The std timeouts built
on them (`gen_server.call`, `task.await`, `task.await_all`, the
`supervisor` and `dynamic_supervisor` requests and `stop`, a gen_server
callback's `timeout`) inherit it: a timeout this large raises `...:
timeout` at once. Use `nil` (no timeout) to wait forever. The deadline
should saturate at `INT64_MAX`.

### `s = s + x` in a loop skips the `+` type check

```gem
fn f()
  let s = ""
  let i = 0
  while i < 3
    s = s + i
    i += 1
  end
  s
end
print(f())        # 012, but "" + 1 raises a type error elsewhere
```

Codegen turns `s = s + x` inside a loop into `gem_string_append`
(`decompose_concat`/`find_append_vars` in compiler/codegen.gem,
runtime/gem_ops.c), which appends any value's `to_string` form instead
of raising like `+`.

### Printing a table cuts strings at an embedded NUL

`let t = ["a\0b"]` / `print(len(t[0]), t)` prints `3 ["a"]`. `fmt_value`
in runtime/gem_builtins_core.c (used by `print`, `to_string` and
interpolation of tables) writes strings with `strlen`, not `slen`.

### sqlite: empty SQL, placeholders in `sqlite_exec`, several statements

```gem
let db = sqlite_open(":memory:")
pcall(fn() sqlite_query(db, "", []) end)         # error "sqlite_query: not an error"
sqlite_exec(db, "CREATE TABLE t(x)")
sqlite_exec(db, "INSERT INTO t VALUES (?)")      # inserts NULL, no error
sqlite_query(db, "INSERT INTO t VALUES (1); INSERT INTO t VALUES (2)", [])
print(sqlite_query(db, "SELECT count(*) AS n FROM t", []))   # [{n: 2}]: the 2nd INSERT never ran
```

Empty or comment-only SQL should return `[]`; `sqlite_exec` should raise
on a statement with parameters; `sqlite_query` should run (or reject)
the text after the first statement. runtime/gem_builtins_sqlite.c.

### sqlite: TEXT values stop at a NUL

```gem
let db = sqlite_open(":memory:")
let r = sqlite_query(db, "SELECT ? AS s", ["x\0y"])
print(len(r[0].s))                                 # 1, not 3
```

TEXT columns are read back with `gem_string` (strlen), so a string with an
embedded NUL is cut short; use `sqlite3_column_bytes`.
runtime/gem_builtins_sqlite.c.

### `sort` keeps the comparator in a global shared by all processes

```gem
let groups = [[3, 1], [9, 8], [5, 4]]
sort(groups, fn(a, b)
  sort(a, fn(x, y) x - y end)
  a[0] - b[0]
end)
print(groups)
```

fails with `type error in -: got table and table` at the inner
comparator: the `sort` builtin stores the comparator in `static GemVal
gem_sort_cmp_fn_global` (runtime/gem_builtins_collection.c) and calls
`qsort`, so a nested sort replaces it and the outer `qsort` goes on
calling the inner comparator. Two processes sorting at once do the same
when a comparator has a loop (the scheduler can switch processes at its
back-edge): the array comes back unsorted, with no error. Keep the
comparator per call (`qsort_r`, or a sort of our own that passes it
along), saved and restored across a yield.

### String table keys stop at the first NUL

```gem
let t = {}
t["a\0b"] = 1
t["a\0c"] = 2
print(len(t), t["a\0zzz"], t["a"])   # 1 2 2
```

The string-key index of a table (`shput`/`shgeti` in runtime/gem_core.c)
is stb_ds's C-string hash map, which hashes and compares with
`strlen`/`strcmp`, while `==` compares `slen` bytes. Keys that differ only
after a NUL are the same key, so `table.unique`, `table.group_by`,
`url.parse_query`, `mime.lookup` and `json.parse` (object keys with
`\u0000`) merge them, and `test.assert_eq` calls two tables equal whose
keys differ only after a NUL (`mime.lookup("x.html\0")`
is `text/html`). Hash and compare string keys by `slen`.

## Standard library

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

### `http.start` doesn't range-check the port

```gem
load "std/http"
let h = http.start(http.router(), {port: 70000, host: "127.0.0.1"})  # starts, listens on 4464
let h2 = http.start(http.router(), {port: -1, host: "127.0.0.1"})    # starts too
```

`start` passes `port` to `tcp_listen` (std/http.gem, `pcall tcp_listen`)
without checking it, and the runtime truncates it to 16 bits. A non-int port
raises, but as `http.start: tcp_listen: expected (string host, int port)`.
`start` should raise `http.start: port must be an int from 0 to 65535`.

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

### `log.set_level` returns the internal level number

`print(log.set_level("info"))` prints `1`. `set_level` (std/log.gem) ends
with the assignment to `min_level`, so it leaks that value; it should
return nil.

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

`gem --check` prints a note starting `note: std/log.gem:36: this changes
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

### A `one_for_all` restart hangs on a child that traps exits

```gem
load "std/supervisor"
fn trapper()
  spawn do
    process_flag("trap_exit", true)
    while true
      receive
      when other then nil
      end
    end
  end
end
fn crasher() spawn do receive when {tag: "crash"} then error("boom") end end end
let h = supervisor.start({strategy: "one_for_all",
  children: [{id: "t", start: trapper}, {id: "c", start: crasher}]})
send(supervisor.which_children(h)[1].pid, {tag: "crash"})
sleep(50)
print(pcall supervisor.which_children(h, 300))   # timeout
```

To restart all children, the supervisor sends each running child
`kill(pid, "shutdown")` and waits for its `DOWN` (std/supervisor
`restart_all`). A child that traps exits gets an `EXIT` message instead of
dying, so the supervisor waits forever. Erlang waits a shutdown timeout
and then sends the untrappable `kill`; Gem has no untrappable exit signal,
so that needs one in the runtime (`kill` in runtime/gem_scheduler.c).
The same goes for a supervisor's own exit (`stop`, an exit signal,
restart intensity reached): it kills each child with its exit reason and
waits for the child's `DOWN`, but a child that traps exits gets an `EXIT`
message, so the supervisor waits out its whole shutdown wait (4000 ms,
`SHUTDOWN_MS` in std/supervisor and std/dynamic_supervisor), then exits
and leaves the child running. The window is shared by all children, so the
siblings killed after such a child get no wait: a sibling that is itself
a supervisor may still be shutting down, and holding its names, when the
parent's `DOWN` arrives.

`dynamic_supervisor.terminate_child` has the same limit: it sends the
child `kill(pid, "shutdown")` and waits for its `DOWN`, so a child that
traps exits and doesn't exit on the `EXIT` message makes it raise
`dynamic_supervisor.terminate_child: timeout`, and the child keeps running
(left out of `which_children`, not restarted; another `terminate_child`
sends the signal again):

```gem
load "std/dynamic_supervisor"
fn trapper(a)
  spawn do
    process_flag("trap_exit", true)
    while true
      receive
      when other then nil
      end
    end
  end
end
let d = dynamic_supervisor.start({child: {start: trapper}})
let p = dynamic_supervisor.start_child(d, nil)
print(pcall dynamic_supervisor.terminate_child(d, p, 100))   # timeout
print(process_info(p) != nil)                                 # true
```
