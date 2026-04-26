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

### ~~7. `has_key` / `in` operator for tables~~ DONE
`has_key(tbl, key)` builtin is implemented. The `in` operator (`key in tbl`) is parsed as a binary operator at comparison precedence and compiles to `gem_has_key_fn` with swapped arguments. Works with `not`, `and`, `or`, and `for..in` coexists without ambiguity since `for` consumes `in` as part of its own syntax before expression parsing.

### ~~10. `keys()` as a builtin~~ DONE
`keys()` is now a first-class builtin like `len` and `type`. `gem_keys_fn` added to the runtime; registered in the builtins table, var reference block, and direct call optimization in codegen. The `extern fn keys` declaration is removed from `compiler/codegen.gem`.

### ~~11. `match` on strings~~ DONE
`match` works on strings — the codegen already emits `else if` chains, so branches short-circuit correctly. Both `compile_match` and `compile_match_return` use `} else if` between when-clauses. No code changes needed; the original concern about fall-through was outdated.

### 12. `str_replace` builtin — DONE
The codegen needs string replacement for re-indenting setup code in `and`/`or`/`while` short-circuit compilation. Had a 20-line `str_replace_all` function in Gem. Replaced with a C runtime builtin `str_replace(s, old, new)`.

### ~~13. Reserved words as table keys~~ DONE
Keywords (`else`, `end`, `when`, `match`, `fn`, `for`, `in`, etc.) are now allowed in table literal key position and after `.` for dot access/assignment. The parser accepts any token as a key when followed by `:` in table literals, and any token as a field name after `.`. This eliminates the need for `node["else"]` — you can write `{else: body}` and `node.else` directly. Keywords are still reserved as variable names (`let match = ...` remains an error) since that would create real ambiguity.

## Debugging & Error Handling

### Source-mapped error messages — DONE
The compiler now emits C `#line` directives in generated output so that runtime errors report Gem source file and line numbers. Added `.line` field to all statement-level AST constructors (`let`, `assign`, `dot_assign`, `index_assign`, `fn_def`, `if`, `while`, `match`, `return`). The parser captures `peek().line` at the start of each statement and passes it through. The codegen emits `#line N "file.gem"` before each statement's C code. Synthetic nodes from `for..in` and `elif` desugaring carry the original keyword's line number. Multi-file `#line` (tracking per-node filenames across `load` boundaries) deferred for later. Fixed-point verified.

### Stack traces on error — DONE
Added a global call stack to the C runtime (`GemFrame gem_call_stack[]`). The codegen emits `gem_push_frame()` at function/closure entry and `gem_pop_frame()` before every return. All error paths (`gem_error()`, `gem_error_fn()`, `gem_error_at_fn()`) call `gem_print_stack_trace()` which walks the stack and prints `at <name> (<file>:<line>)` for each frame. Works for named functions, closures, runtime type errors, and the top-level main. Fixed-point verified.

### Top-level `let` variables compile to C globals — DONE
Top-level `let` declarations now emit C global variables instead of locals inside `main()`. Named `fn` definitions (which compile to separate C functions) can now access top-level variables directly. This unblocks std adoption: `load "std/string"` creates a `string` namespace table as a global, and named functions like `parse_request()` can call `string.split(...)` without workarounds. Variables captured by closures at top level remain heap-boxed (`GemVal *`) globals. Variables inside nested scopes (for-loops, if/while bodies) stay as C locals to preserve per-iteration semantics for closures.

### Warn on shadowed/unreachable named `fn` inside functions — DONE
A named `fn` definition inside another function is silently ignored — the function compiles but never binds. This caused a subtle bug where `fn check(...)` couldn't access top-level `let` variables, producing silently wrong results. Now a hard parse-time error: the parser tracks `fn_depth` (incremented on every `parse_fn_body()` entry, decremented on exit) and calls `error()` when a named `fn NAME` is encountered with `fn_depth > 0`. Anonymous `fn()` inside functions (closures) is unaffected.

### `_compile_stmt_return` completeness
The bootstrap's `_compile_stmt_return` didn't handle `dot_assign`, `index_assign`, `break`, `continue`, or `fn_def` — these fell through to `compile_expr` which produced `/* unknown node */` comments in the C output. This was a silent correctness bug: `set_add(defined_fns, name)` inside a function body was silently compiled to a no-op, causing the codegen to treat all user-defined functions as generic closure calls. Fixed in the bootstrap by adding an explicit case for these statement types (compile as statement + `return GEM_NIL`). This class of bug — a missing case that silently produces wrong output instead of erroring — is worth guarding against systematically.

### Rust-style compile error diagnostics — DONE
The compiler now produces formatted error messages with source context, caret highlighting, and optional hints. Added column tracking to the lexer (every token now carries `col` alongside `line`). Created `compiler/errors.gem` module with `compile_error(source, file, line, col, span_len, msg, hint)` that prints the diagnostic to stderr and exits. Updated all error calls in the lexer (15 sites) and parser (12 sites) to use the new formatter. Multi-character tokens get proportionally wide underlines. The gutter width adapts to line number digits. Also fixed a pre-existing crash when `peek_at(1)` goes out of bounds at EOF inside table literal parsing.

### Error recovery — DONE
Added `pcall(f)` (Lua-style protected call). Calls `f()` with zero arguments; on success returns `{ok: true, value: <result>}`, on error returns `{ok: false, error: <message>, stack: <frames>}`. Uses `setjmp`/`longjmp` in the runtime. All error paths (`gem_error`, `gem_error_fn`, `gem_error_at_fn`) check for active pcall before exiting. Nested pcall works. Call stack is snapshotted into the result before longjmp restores it. Fixed-point verified.

### `extern fn` should support `Table` / `GemVal` return types
The `extern fn` type map only handled `Int`, `Float`, `String`, `Bool`, `Ptr`, and `Nil`. Unknown return types (including `Table`) silently discarded the return value and returned `GEM_NIL`. This caused `keys()` to always return nil. Fixed by adding `Table` as a passthrough type (return the `GemVal` directly). The silent failure mode was particularly nasty — no error, just nil propagating until something downstream crashes.

## Low Priority

### 8. Allow keywords as table literal keys
`"else"` is a keyword so `{else: body}` is a parse error. Currently requires `node["else"] = body` after construction. Could allow keywords in table literal key position since it's unambiguous.

### 9. Destructuring / multi-return — DONE
Implemented `let {a, b} = expr` (table) and `let [a, b] = expr` (array) destructuring. Desugars at parse time to a temp variable + individual lets. No renaming, nesting, rest/splat, or defaults (v0 scope).

## Stdlib Migration — Move Builtins to Gem

**Goal:** Keep the C runtime minimal. `chr`/`ord` must stay in C (no way to construct arbitrary bytes in pure Gem).

### Migrated to `std/` — DONE
- `split(s, delim)` — now `string.split` in `std/string.gem`, implemented in pure Gem using `ord(s, i)` + `buf_*`
- `index_of(s, needle)` — now `string.index_of` in `std/string.gem`, implemented in pure Gem
- Removed as bare builtins from the compiler. `std/string` is the canonical source.

### Builtins that could move with string performance work
- `substr(s, start, len)` — trivial in Gem but currently copies; needs string views or slices to be competitive

### String performance prerequisites
These runtime/compiler improvements would make pure-Gem string code competitive with C:

1. **String views / slices** — `substr` returns a pointer+offset+length into the original string instead of allocating a copy. Makes substring extraction O(1). Not yet implemented — requires changing the string representation in GemVal (add len field, stop assuming null-termination). Every string consumer (strlen, strcmp, printf %s) would need updating. Deferred until needed.
2. **String builder (`buf_new`/`buf_push`/`buf_str`)** — DONE. Explicit mutable buffer builtins with doubling growth. `buf_push` appends without allocating a new string; `buf_str` finalizes into an immutable GemVal string. Benchmarks: ~58x faster than `s = s + x` at 100K iterations.
3. **Byte-level access without allocation (`ord(s, i)`)** — DONE. `ord` now accepts an optional second argument for direct byte indexing into the string. `ord(s, i)` returns the byte value at index `i` without allocating a temporary 1-char string.
