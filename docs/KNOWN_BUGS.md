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

## Runtime

### Int `+`, `-` and `*` overflow is undefined behaviour in C

```gem
let m = 9223372036854775807
print(m + 1)      # -9223372036854775808 in practice
```

`gem_add`/`gem_sub`/`gem_mul` in runtime/gem_ops.c (and the constant
folder, compiler/fold.gem, which runs them) overflow signed 64-bit ints
with no `-fwrapv`, so the wrap SPEC promises is what the C compilers do in
practice, not what C guarantees; an optimizer may assume it never
happens. Compute in `uint64_t` and convert back, as `gem_div` does for
`INT64_MIN / -1`.

### `insert` and `remove_at` on a table with string keys keep a stale key index

```gem
let h = {a: 3, b: 1}
insert(h, 0, 9)
print(h.a, h.b, h)        # 9 3 [9, 3, 1]: the keys are 0..2 now
let r = {a: 3, b: 1, c: 2}
remove_at(r, 0)
print(r.a, r.b, r)        # 1 2 [1, 2]
```

Both renumber every key to an int (`gem_insert_fn`, `gem_remove_at_fn` in
runtime/gem_builtins_collection.c) but leave the table's string-key index
in place, so the old string keys still find (wrong) values. The index
points at key strings that nothing roots any more: after a region reset
frees them, a lookup reads freed memory and can crash. `sort` had the same
problem and now drops the index (`gem_str_index_free`); these two should
too.

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

### `sort` doesn't renumber the keys of a table with one entry

```gem
print(sort({a: 5}), sort({a: 5, b: 1}))   # {a: 5} [1, 5]
```

SPEC says `sort` renumbers keys to `0..n-1`, but `gem_sort_fn`
(runtime/gem_builtins_collection.c) returns early when the table has at
most one entry, so `{a: 5}` keeps its string key.

## Standard library


None at the moment.
