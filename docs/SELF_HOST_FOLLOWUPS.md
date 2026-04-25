# Self-Hosting Fast Followups

Language improvements to make right after the compiler can compile itself.
Prioritized by impact on real code (the compiler itself is the benchmark).

## High Priority

### 1. `for..in` loops — DONE
Added `for x in items` (array iteration) and `for i = 0, n` (range iteration).
Both desugar to `while` at parse time. Iterator increment happens before the user body so `break`/`continue` work correctly. All compiler files rewritten to use `for..in`. Fixed-point verified.

### 2. `elif` — DONE
Added `elif` as parser-level desugaring into nested `if/else`. No new AST nodes or codegen changes needed — the parser rewrites `if/elif/elif/else/end` into `if/else{if/else{if/else/end}/end}/end`. All compiler files rewritten to use `elif`, eliminating deeply nested tag-dispatch pyramids. Codegen.gem dropped ~160 lines. Fixed-point verified. Test runner updated to use self-hosted compiler.

### 3. Multi-line function call arguments — DONE
Added `skip_nl()` calls in `parse_call` after the opening paren, after each comma, and before the closing paren. This allows newlines anywhere inside `(...)` argument lists. No lexer, AST, or codegen changes needed. Trailing `do` blocks still work since `skip_nl()` only consumes NEWLINE tokens. Fixed-point verified.

### 4. `push()` builtin — DONE
Added `push(arr, val)` as a runtime builtin (`gem_push_fn`) that appends a value at the next integer index. Returns the pushed value. Registered in the codegen as a builtin with direct call optimization. All compiler files (codegen.gem, parser.gem, lexer.gem) rewritten to use `push()`, eliminating ~15 counter variables and ~30 `count += 1` lines. Fixed-point verified.

## Medium Priority

### 5. String interpolation — DONE
Added `{expr}` interpolation syntax in string literals. Expressions are auto-coerced to strings via `to_string`. Literal braces escaped with `\{` and `\}`. Lexer emits INTERP_START/STRING/expr/INTERP_END token sequences; parser collects into `interp` AST nodes; codegen chains parts with `gem_add` and `gem_to_string_fn`. Supports nested expressions including function calls, table literals (with brace depth tracking), and nested strings. Two-phase bootstrap required: first bootstrap `\{` escape support, then escape all literal braces in source strings. Fixed-point verified.

### 6. Implicit `to_string` in concatenation — DONE (via #5)
String interpolation (`{expr}`) auto-coerces all types to strings, eliminating the need for explicit `to_string()` calls. The remaining manual `to_string()` calls in `+` chains will be cleaned up as the compiler is rewritten to use interpolation.

### 7. `has_key` / `in` operator for tables
Currently checking key existence with `table[key] != nil`. A `has_key(table, key)` builtin or `key in table` syntax would be clearer.

### ~~10. `keys()` as a builtin~~ DONE
`keys()` is now a first-class builtin like `len` and `type`. `gem_keys_fn` added to the runtime; registered in the builtins table, var reference block, and direct call optimization in codegen. The `extern fn keys` declaration is removed from `compiler/codegen.gem`.

### 11. `match` on strings
The codegen has many tag-dispatch patterns (`if tag == "int" ... if tag == "float" ...`) that would naturally be `match` statements, but `match` uses `==` comparison which works on strings. The problem is `match` doesn't short-circuit on return — when a `when` branch returns, execution falls through checking subsequent branches. This is correct but wasteful. More importantly, using `match` here would require all the handler code to be in the `when` body, which doesn't help when each handler is 5+ lines. This is really the same issue as `elif` (item 2) — both solve tag dispatch. `elif` is strictly more useful since it handles arbitrary conditions, not just equality.

### 12. `str_replace` builtin
The codegen needs string replacement for re-indenting setup code in `and`/`or`/`while` short-circuit compilation. Had to write a 20-line `str_replace_all` function in Gem. A builtin `str_replace(s, old, new)` would be cleaner.

### 13. Reserved word `match` as variable name
`match` is a keyword, which means `let match = true` is a parse error. Hit this when porting `str_replace_all` — had to rename the variable to `found`. Other keywords that might conflict with common variable names: `end`, `do`, `when`. Not a high priority but worth noting.

## Debugging & Error Handling

### Source-mapped error messages — DONE
The compiler now emits C `#line` directives in generated output so that runtime errors report Gem source file and line numbers. Added `.line` field to all statement-level AST constructors (`let`, `assign`, `dot_assign`, `index_assign`, `fn_def`, `if`, `while`, `match`, `return`). The parser captures `peek().line` at the start of each statement and passes it through. The codegen emits `#line N "file.gem"` before each statement's C code. Synthetic nodes from `for..in` and `elif` desugaring carry the original keyword's line number. Multi-file `#line` (tracking per-node filenames across `load` boundaries) deferred for later. Fixed-point verified.

### Stack traces on error — DONE
Added a global call stack to the C runtime (`GemFrame gem_call_stack[]`). The codegen emits `gem_push_frame()` at function/closure entry and `gem_pop_frame()` before every return. All error paths (`gem_error()`, `gem_error_fn()`, `gem_error_at_fn()`) call `gem_print_stack_trace()` which walks the stack and prints `at <name> (<file>:<line>)` for each frame. Works for named functions, closures, runtime type errors, and the top-level main. Fixed-point verified.

### Warn on shadowed/unreachable named `fn` inside functions
A named `fn` definition inside another function is silently ignored — the function compiles but never binds. This caused a subtle bug where `fn check(...)` couldn't access top-level `let` variables, producing silently wrong results. The compiler should emit a warning (or error) when a `fn_def` appears in a non-top-level scope.

### `_compile_stmt_return` completeness
The bootstrap's `_compile_stmt_return` didn't handle `dot_assign`, `index_assign`, `break`, `continue`, or `fn_def` — these fell through to `compile_expr` which produced `/* unknown node */` comments in the C output. This was a silent correctness bug: `set_add(defined_fns, name)` inside a function body was silently compiled to a no-op, causing the codegen to treat all user-defined functions as generic closure calls. Fixed in the bootstrap by adding an explicit case for these statement types (compile as statement + `return GEM_NIL`). This class of bug — a missing case that silently produces wrong output instead of erroring — is worth guarding against systematically.

### Error recovery — DONE
Added `pcall(f)` (Lua-style protected call). Calls `f()` with zero arguments; on success returns `{ok: true, value: <result>}`, on error returns `{ok: false, error: <message>, stack: <frames>}`. Uses `setjmp`/`longjmp` in the runtime. All error paths (`gem_error`, `gem_error_fn`, `gem_error_at_fn`) check for active pcall before exiting. Nested pcall works. Call stack is snapshotted into the result before longjmp restores it. Fixed-point verified.

### `extern fn` should support `Table` / `GemVal` return types
The `extern fn` type map only handled `Int`, `Float`, `String`, `Bool`, `Ptr`, and `Nil`. Unknown return types (including `Table`) silently discarded the return value and returned `GEM_NIL`. This caused `keys()` to always return nil. Fixed by adding `Table` as a passthrough type (return the `GemVal` directly). The silent failure mode was particularly nasty — no error, just nil propagating until something downstream crashes.

## Low Priority

### 8. Allow keywords as table literal keys
`"else"` is a keyword so `{else: body}` is a parse error. Currently requires `node["else"] = body` after construction. Could allow keywords in table literal key position since it's unambiguous.

### 9. Destructuring / multi-return
Minor ergonomic win for parser helpers and similar patterns. Not blocking anything.
