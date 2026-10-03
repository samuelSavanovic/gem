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
