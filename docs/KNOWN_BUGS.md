# Known bugs

Bugs found and not yet fixed. Each entry has a minimal repro (checked
against `build/gem`), what goes wrong, and where the code is. A fix deletes
its entry in the same change, along with any **(bug)** rule in
`docs/BEST_PRACTICES.md` that exists because of it. Performance problems go in
`docs/OPTIMIZATIONS.md`, missing features in `docs/ROADMAP.md`.

## Compiler

### A parameter named like a top-level `fn` reads the fn inside closures

```gem
fn item() "FN" end
fn c(item)
  let f = fn() item end
  f()
end
print(c(5))          # <fn>, expected 5
```

A closure (or `do` block) that uses a parameter (plain, defaulted or
rest) of its enclosing function, or of itself, gets the module-level `fn`
of the same name instead. A `let`, a `for` variable or a destructured
parameter of that name is fine, and so is a parameter named like a builtin or a module
`let`. The capture resolution in compiler/codegen.gem prefers the named
fn to the parameter. `docs/BEST_PRACTICES.md` has a **(bug)** rule for it.

### Float literals keep only six significant digits; some don't compile

```gem
let a = 3.14159265358979
print(a == 3.14159)                   # true
print(0.000001)                       # C error: invalid suffix ".0"
```

`format_float` in compiler/codegen.gem emits a float literal through
`to_string`, which uses `%g` (six significant digits), so the literal is
rounded at compile time. When the `%g` form has an exponent and no dot
(`1e-06`, `1e+06`), codegen appends `.0` and the C compiler rejects
`1e-06.0`; constant folding hits it too (`1000000 * 1.0`). Emit literals
with `%.17g` (and add `.0` only to a form with no `.` or `e`).

### Floats print, interpolate and JSON-encode with six significant digits

```gem
load "std/json"
let x = to_float("1234567.5")
print(x, "{x}", json.encode(x))      # 1.23457e+06 three times
print(to_float(to_string(x)) == x)   # false
```

`to_string` (and `print`, interpolation, `buf_push`, `build_string`'s
`add`) formats floats with `%g` (runtime/gem_builtins_core.c,
runtime/gem_builtins_string.c), and `std/json` encodes floats with
`to_string`, so a float loses precision on every round trip through text.
Integral floats also print without a decimal point (`2.0` prints `2`).
Use the shortest representation that reads back to the same double.

### A renaming destructuring pattern reaches the C compiler

```gem
let {pid: s} = {pid: 5}
```

SPEC says destructuring has no renaming, but the parser accepts
`{key: name}` in a `let` pattern and codegen emits invalid C
(`#define gem_gi_: 0`, then `gem: compilation failed`). It should be a
parse error at the pattern.

### Exporting or importing a name that doesn't exist shows a mangled name

```gem
# mods/e1.gem: fn a() 1 end / export a, b
load "./mods/e1"               # undeclared identifier `_mod_e1_b`
load "std/string" (nosuch)     # undeclared identifier `_mod_string_nosuch`
```

Both errors point at the entry file with no line, and show the mangled
slot name. Expected: `module e1 exports b, which it doesn't define` at the
`export` line, and `module string has no export nosuch` at the `load`.

### A module file name that isn't a C identifier reaches the C compiler

`load "./mods/my-utils"` fails in cc (`gem_fn__mod_my-utils_f`). Either
mangle the name or report a Gem error at the `load`.

### Two loaded modules with the same base name replace each other

```gem
load "std/string"
load "./mods/string"           # exports only `upper2`
string.upper("a")              # module `string` has no export `upper`
```

The namespace is named after the file's base name, and the later `load`
silently wins (if both modules export the same name, cc fails with
`redefinition of 'gem_fn__mod_string_upper'` instead). A module loaded indirectly counts too: with a user
`./json.gem` exporting `parse`, `load "std/http"` (which loads std/json)
plus `load "./json"` fails in the C compiler (`redefinition of
'gem_fn__mod_json_parse'`). It should be a compile error at the second
`load`, or modules should be named by path.

### The project root is not found when the entry path has no directory

With `p/gem.toml`, `p/lib/util.gem` and `p/app/main.gem` containing
`load "lib/util"`, `cd p/app && gem main.gem` fails with
`read_file: cannot open './lib/util.gem'` and a compiler stack trace;
`cd p && gem app/main.gem` and an absolute path work. `find_project_root`
in compiler/loader.gem starts at `dirname("main.gem")`, which is `"."`,
and stops because `dirname(".")` is `"."`. Make the start directory
absolute first.

### A symlinked `gem` binary finds neither `std/` nor `runtime/`

With a symlink to `build/gem` on `PATH`, `gem prog.gem` fails with
`gem: stdlib module not found: std/string (looked in .)`: the install root
is computed from `argv()[0]` as typed. `GEM_STDLIB` finds `std/` but not
`runtime/`, so the C compile still fails (`gem.h: No such file`). Resolve
the executable's real path (`/proc/self/exe`, `realpath`) first.

### Integer literals out of range wrap silently

`print(99999999999999999999)` prints `7766279631452241919`. The lexer
should report an integer literal that doesn't fit in 64 bits.

### `pcall <expr>` at top level records line 0

```gem
let t = nil
print((pcall t[0]).stack)    # [{name: "anonymous fn", file: ..., line: 0}]
```

The same for `pcall 1 / 0` and `pcall 1 < "a"` at top level; inside a
function the line is right. The closure the expression form desugars to
has no line for an operator or index expression.

### A `receive` with only an `after` clause doesn't parse

```gem
receive
after 10 then nil
end
```

reports `unexpected token 'after'`. SPEC says the `after` clause is
optional but never that a `when` arm is required, and Erlang's
`receive after N -> ok end` is a common way to wait. Accept a `receive`
with no arms (it waits `after` ms, leaving the mailbox alone).

### An array pattern matches a record of the same size

```gem
match {a: 1, b: 2}
when [x, y] then print("pair", x, y)    # pair nil nil
end
```

The `[p1, p2]` check is `len(target) == 2`, which a two-key record
passes. It should also require the keys `0 .. n-1`.

### A tab before a closing `"""` is not accepted

A triple-quoted string whose closing `"""` is indented with tabs reports
`unterminated triple-quoted string`; with spaces it works. SPEC says
"only leading whitespace".

### `for i, ch in "abc"` reports an internal name

`for ch in "abc"` iterates the bytes, but the two-variable form fails at
runtime with `__table_key_at: expected table` (the lowered loop's helper).
Either support strings there or report `for k, v` over a string at the
`for`.

## Runtime

### Runtime traces lose the source line when run from another directory

Trace paths are project-relative (or relative to where the program was
compiled from), and the runtime opens them relative to the current
directory to print the `-->` source line. A binary run from any other
directory prints the trace without the source line, silently.
`gem_print_source_context` in runtime/gem_error.c.

### `kill(pid, "normal")` kills a process that doesn't trap exits

```gem
let m = self()
spawn do
  kill(m, "normal")
end
sleep(50)
print("not reached")    # main is gone; the program exits 0 silently
```

In Erlang, an exit signal with reason `normal` is ignored by a process
that doesn't trap exits; here it ends the target (main included, with no
report). `gem_exit_builtin` in runtime/gem_scheduler.c. Decide which
semantics Gem wants and document it in SPEC.md (`kill`).

### `buf_push` and `build_string`'s `add` drop buffers, functions and refs

```gem
let b = buf_new()
buf_push(b, "hi")
let c = buf_new()
buf_push(c, b)
print(len(to_string(c)))                         # 0
print(build_string do |add| add("[", b, "]") end) # []
```

`gem_buf_push_fn` in runtime/gem_builtins_string.c turns any value it has
no case for into `""`. A buffer should append its contents, and other
values their `to_string` form.

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

### An `extern fn` ignores extra arguments

```gem
extern include "string.h"
extern fn strlen(s: String) -> Int
print(strlen("a", "b"))      # 1, no error
```

The generated wrapper checks `argc` only against too few arguments.
Raise `expected 1 argument(s), got 2` for too many too.

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
