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

### A non-integer `after` timeout is read as raw bits

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

### After main ends, the process that reuses its slot counts as main

```gem
spawn do
  sleep(20)                 # main has ended and freed slot 0
  let p = 1
  while p % 1024 != 0       # spawn until a process lands in slot 0
    p = spawn(fn()
      sleep(1)
      error("child failed")
    end)
  end
end
print("main ends")
```

prints 1,022 ordinary crash reports, then `[Runtime Error]: child failed`
for the process in slot 0 and exits with status 1. `gem_main_pid` stays 0
after main's slot is freed (`gem_propagate_exit` → `gem_free_proc_slot`
in runtime/gem_scheduler.c), so every `== gem_main_pid` check fires for
whichever process gets slot 0 next (about every 1,024th spawn): its
uncaught error or `exit` ends the program, a `kill` or link death of it is
reported as "main process killed", the deadlock check reports a false
deadlock, and its arena and globals are never freed (about 1 MB per
reuse). It hits the "start a supervisor tree and let main end" pattern
(examples/http_server/server.gem): a restarting child ends the program
after about 1,022 restarts. Clear `gem_main_pid` (or mark main finished)
when main's slot is freed.

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
`url.parse_query` and `mime.lookup` merge them (`mime.lookup("x.html\0")`
is `text/html`). Hash and compare string keys by `slen`.

## Standard library

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
The same goes for a supervisor's own death: its children are linked to it
and exit with it, except a child that traps exits, which gets an `EXIT`
message and keeps running.

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

### `std/http` route params decode `+` as a space

```gem
load "std/http"
load "std/request"
let r = http.router()
r.get("/tags/:name", fn(req) http.ok("[{req.params.name}]") end)
http.start(r, {port: 19882})
sleep(100)
print(request.get("http://127.0.0.1:19882/tags/c++").body)
exit(0)
```

prints `[c  ]` instead of `[c++]`. `match_route` in std/http.gem decodes
path segments with `url.decode`, which follows form encoding (`+` is a
space); in a path `+` is a literal plus. Decode path segments with
`url.decode(str_replace(seg, "+", "%2B"))` or a path-specific decoder.
