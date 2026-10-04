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

## Runtime

### A `tcp_accept` waiting on a listening socket that another process closes spins forever

```gem
let l = tcp_listen("127.0.0.1", 18296)
spawn do
  let r = pcall tcp_accept(l)
  print("accept returned", r)    # never printed
end
sleep(100)
tcp_close(l)
print("closed")                  # the program then never exits, at 100% CPU
```

The waiting process is never told: `poll` reports the closed fd as
`POLLNVAL` at once on every pass, the scheduler keeps it waiting
(`gem_run_scheduler` / the poll loop in runtime/gem_scheduler.c), and
`accept` is not retried, so the scheduler busy-loops. A `POLLNVAL` (or
`POLLERR`/`POLLHUP`) should make the waiter ready so `accept` fails with
`EBADF` and raises. Likely the same for a `tcp_read` on a socket another
process closes.

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

### A buffer passed as `s` to `s = s + x` in a loop is changed in place

```gem
fn f(s, xs)
  let i = 0
  while i < len(xs)
    s = s + xs[i]
    i += 1
  end
  s
end
let b = buf_new()
buf_push(b, "pre")
print(type(f(b, ["a", "b"])), to_string(b))   # string preab
```

`b + "a"` raises (`type error in +: got buffer and string`), but inside the
loop codegen turns `s = s + x` into `gem_string_append`
(`find_append_vars` in compiler/codegen.gem, runtime/gem_ops.c), which
can't tell the caller's buffer from the buffer it builds a string in: it
appends to the caller's buffer and returns a string.

### `input()` splits lines over 4095 bytes and stops at a NUL

```sh
python3 -c 'print("a" * 5000); print("b\0c")' | gem lines.gem
# lines.gem: while true / let line = input() / if line == nil then break end / print(len(line)) / end
# prints 4095, 905, 1 instead of 5000, 3
```

`gem_input_fn` (runtime/gem_builtins_core.c) reads with one `fgets` into a
4096-byte stack buffer and takes the length with `strlen`. A longer line
comes back as several lines, with nothing to tell the caller, and a NUL
byte ends the line early. Read with `getline` (or grow a buffer until the
newline) and use the returned length.

### `read_file` of a file of 2 GiB or more gives a wrong length

```sh
truncate -s 2200M big.bin
gem read.gem big.bin    # read.gem: print(len(read_file(argv()[1])))
# prints -1988100096; a 4200M file prints 109051904
```

A string's length is a C `int` (`slen` in `runtime/gem.h`), and
`gem_read_file_fn` (runtime/gem_builtins_io.c) stores the file size into it
with an unchecked `(int)` cast. Strings can't hold 2 GiB, so `read_file`
should raise for such a file; the other places that size a string from a
`size_t` need the same check.

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
