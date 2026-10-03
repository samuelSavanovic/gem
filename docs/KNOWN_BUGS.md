# Known bugs

Bugs found and not yet fixed. Each entry has a minimal repro (checked
against `build/gem`), what goes wrong, and where the code is. A fix deletes
its entry in the same change, along with any **(bug)** rule in
`docs/BEST_PRACTICES.md` that exists because of it. Performance problems go in
`docs/OPTIMIZATIONS.md`, missing features in `docs/ROADMAP.md`.

## Compiler

## Runtime

### A top-level `let` named like a builtin can't read the builtin

```gem
let t = {a: 1}
let keys = keys(t)          # attempt to call nil value
```

The entry file's top-level binding named like a builtin replaces the
builtin in the whole file (`shadow_entry_builtins` in compiler/main.gem),
so its own initializer calls the not-yet-set slot. A `let` inside a
function (`let keys = keys(t)`) works: its initializer sees the builtin.

## C interop

### `extern include` with a relative path is not found next to the source

```gem
extern include "helpers.h"           # helpers.h sits next to this file
extern fn twice(x: Int) -> Int
print(twice(21))
```

fails in the C compiler with `fatal error: helpers.h: No such file or
directory`. The line is copied as `#include "helpers.h"` into
`/tmp/gem_<name>.c`, which `cc` compiles with only the runtime directory
on its include path (the cc command in compiler/main.gem), so a relative
path resolves against `/tmp` and `runtime/`, not the `.gem` file. The
examples work around it with `"../examples/support/..."`, which resolves
from `runtime/`. Expected: resolve a relative path against the directory
of the file that contains the `extern include` (or add that directory with
`-I`), and report a missing header as a Gem error at the line.
`docs/BEST_PRACTICES.md` (C interop) says to use an absolute path until
this is fixed.

## Standard library

### `dynamic_supervisor` crashes after removing a child that isn't the last

```gem
load "std/dynamic_supervisor"
fn w(a) spawn do while true receive when other then nil end end end end
let ds = dynamic_supervisor.start({child: {start: w}})
let a = dynamic_supervisor.start_child(ds, "a")
dynamic_supervisor.start_child(ds, "b")
dynamic_supervisor.terminate_child(ds, a)
print(dynamic_supervisor.which_children(ds))
```

The supervisor dies with `field access on non-table: got nil`
(dynamic_supervisor.gem `find_child_index`), and `which_children`, which
has no `after`, deadlocks main. `dsup_loop` removes children with
`delete(state.children, idx)` on an array (the hole trap); use
`remove_at`. The DOWN path for temporary and transient children has the
same `delete`, so the exit of such a child that isn't last in the list
crashes the supervisor too (three `restart: "temporary"` children; stop
the first, then the second).

### The supervisors' loops are not tail calls

Some paths of `sup_loop` (std/supervisor) and `dsup_loop`
(std/dynamic_supervisor) recurse with `sup_loop(state)` /
`dsup_loop(state)` followed by `return nil`, a non-tail call that adds a
stack frame: the exit of a temporary or transient child, `terminate_child`
and `which_children` in the dynamic supervisor. (Restarts are tail calls.)
A dynamic supervisor with `restart: "temporary"` children that exit at
once dies with `stack overflow in <fn>` after 1,750 to 2,000 child exits.
Make the self call the last expression.

### `http.serve` returns when the caller gets any message

```gem
spawn do sleep(100); send(me, {tag: "hello"}) end   # let me = self() before
http.serve(app, {port: 8080})                       # returns {tag: "hello"}
```

`serve` monitors the acceptor and then calls `receive()`, which takes
whatever arrives first. Match `{tag: "DOWN", pid: ^pid}` instead.

### `std/http` hides handler errors and sends empty default bodies

A handler that raises, or returns something other than a response table,
gets a 500 with an empty body and nothing on stderr. The server's own
404/500 responses also have empty bodies, although SPEC says the
defaults are `"Not Found"` and `"Internal Server Error"`: std/http calls
`not_found(nil)` / `server_error(nil)` with an explicit `nil`, which
doesn't apply the default. Log the caught error, and call the builders
with no argument.

### `json.encode` drops entries of tables with non-sequential int keys

```gem
let ids = {}
ids[42] = "x"
print(json.encode(ids))         # [null]
let m = ["z"]
m.name = "n"
print(json.encode(m))           # ["z",null]
```

`is_array` in std/json looks only at the first key's type. A table whose
keys aren't exactly `0 .. n-1` should encode as an object (with string
keys), or raise.

### `json.parse` can't parse integers beyond 64 bits

`json.parse("12345678901234567890")` raises `to_int: cannot convert ...`
(std/json `parse_number`). Parse them as floats.

### `supervisor.start` with `name:` registers the name after starting the children

`supervisor.which_children("sup")` right after `supervisor.start({name:
"sup", ...})` can raise `send: no process registered with that name`:
the supervisor process registers itself only once all children are
started, and `start` has already returned. Register before starting
children (or from `start`).

### Supervisor children must return a bare pid

A child spec `start: fn() gen_server.start(mod) end` returns `{pid}`, and
the supervisor dies with `monitor: expected pid (int) argument` while
`supervisor.start` still returns a handle to it. Accept both forms.

### `std/request` has no timeout and doesn't decode chunked bodies

`request` reads with `tcp_read(fd, READ_SIZE)` and no timeout, so a silent
server blocks the caller forever; a parse error leaks the socket; a
chunked response comes back with the chunk framing in `body`; a status
line with no reason phrase (`HTTP/1.1 204`) or an `https://` URL raises
`to_int: cannot convert "" to int`.

### A bad sqlite handle crashes the program

`print(pcall sqlite_query(12345, "select 1", []))` dies with a segmentation
fault (exit 139); `pcall` can't catch it. A handle is a raw `sqlite3 *`
stored as an int: using one after `sqlite_close` is a use-after-free, and
closing twice returns `nil`. Keep a table of open handles in
runtime/gem_builtins_sqlite.c and raise on an unknown one.

### `std/http` request headers keep the client's case

`req.headers["content-type"]` is `nil` when the client sent
`Content-Type`, and the other way round; `Cookie` is looked up in that
exact case only, so `cookie: a=1` gives empty `req.cookies`. A header
written without a space after the colon (`Host:x`, valid HTTP) is
dropped. `parse_headers` in std/http.gem splits on `": "`. Lowercase the
names, trim optional whitespace, and document the `req.headers` keys.

### `sqlite_query` doesn't check its parameters

`sqlite_query(db, "SELECT ?, ?", [1])` binds `NULL` for the missing
parameter, extra parameters are ignored, and a table parameter binds
`NULL`. All three should raise. (runtime/gem_builtins_sqlite.c)

## Editor grammars

### tree-sitter grammar gaps

- No `;` statement separator.
- About 21 repo files parse with ERROR nodes (e.g. examples/06_blocks.gem,
  examples 115–126, std/http.gem, `load ... (names)` in compiler/*.gem).
- `"""` nested in a `"""` interpolation is an ERROR node, and probably ends
  the outer string early in the VS Code grammar too.

Not re-checked after #28 (no tree-sitter CLI in that environment).
