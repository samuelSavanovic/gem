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

### A float key in a table literal becomes a string key

```gem
let t = {1.5: "e"}
print(has_key(t, "1.5"), has_key(t, 1.5))   # true false
```

The parser keeps a NUMBER key's text, and `compile_table` emits it as a
string key; a pattern `{1.5: x}` does the same (compiler/parser.gem
`parse_int_key` handles only ints). It should be a float key or a
compile error.

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

### A non-integer `after` timeout is taken as 0

```gem
receive
after nil then print("no wait")
end
```

runs at once instead of raising: the timeout's `.ival` is read with no
type check (`compile_receive_match` in compiler/codegen.gem). A
non-integer timeout should raise.

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

### sqlite: named parameters bind by position; TEXT values stop at a NUL

```gem
let db = sqlite_open(":memory:")
sqlite_exec(db, "CREATE TABLE t(a, b)")
sqlite_query(db, "INSERT INTO t VALUES (:b, :a)", {a: 10, b: 20})
print(sqlite_query(db, "SELECT a, b FROM t", []))  # [{a: 10, b: 20}]: :b got 10
let r = sqlite_query(db, "SELECT ? AS s", ["x\0y"])
print(len(r[0].s))                                 # 1, not 3
```

`sqlite_query` binds the params table's values in insertion order and
ignores its keys, so `:name` placeholders get the wrong values; a record
of params should bind by name (`sqlite3_bind_parameter_index`). TEXT
columns are read back with `gem_string` (strlen), so a string with an
embedded NUL is cut short; use `sqlite3_column_bytes`.
runtime/gem_builtins_sqlite.c.

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
