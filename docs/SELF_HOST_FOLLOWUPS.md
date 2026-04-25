# Self-Hosting Fast Followups

Language improvements to make right after the compiler can compile itself.
Prioritized by impact on real code (the compiler itself is the benchmark).

## High Priority

### 1. `for..in` loops — DONE
Added `for x in items` (array iteration) and `for i = 0, n` (range iteration).
Both desugar to `while` at parse time. Iterator increment happens before the user body so `break`/`continue` work correctly. All compiler files rewritten to use `for..in`. Fixed-point verified.

### 2. `elif` — DONE
Added `elif` as parser-level desugaring into nested `if/else`. No new AST nodes or codegen changes needed — the parser rewrites `if/elif/elif/else/end` into `if/else{if/else{if/else/end}/end}/end`. All compiler files rewritten to use `elif`, eliminating deeply nested tag-dispatch pyramids. Codegen.gem dropped ~160 lines. Fixed-point verified. Test runner updated to use self-hosted compiler.

### 3. Multi-line function call arguments
The grammar currently disallows newlines between `(` and `)`. This forces long single-line calls:
```gem
check("compound assign desugars", "x += 1", "(assign x (binop + (var x) (int 1)))")
```
Allow newlines inside argument lists.

### 4. `push()` builtin — DONE
Added `push(arr, val)` as a runtime builtin (`gem_push_fn`) that appends a value at the next integer index. Returns the pushed value. Registered in the codegen as a builtin with direct call optimization. All compiler files (codegen.gem, parser.gem, lexer.gem) rewritten to use `push()`, eliminating ~15 counter variables and ~30 `count += 1` lines. Fixed-point verified.

## Medium Priority

### 5. String interpolation
Error messages and debug output require painful concat chains:
```gem
error("expected '" + tp + "' but got '" + t.type + "' at line " + to_string(t.line))
```
Something like `error("expected '{tp}' but got '{t.type}' at line {t.line}")` with auto `to_string`.

**Codegen note:** The codegen is the worst offender in the entire codebase for string concatenation. Almost every line of the codegen builds C source code via `+` chains. Example from `compile_extern_fn`:
```gem
lines = lines + "    " + c_type + " _p" + to_string(i) + " = args[" + to_string(i) + "]." + field + ";\n"
```
These are extremely hard to read and easy to get wrong (missing a quote, wrong spacing). String interpolation would be transformative here.

### 6. Implicit `to_string` in concatenation
`"count: " + to_string(n)` should just be `"count: " + n`. If string interpolation lands first, this becomes less urgent.

**Codegen note:** The codegen calls `to_string()` constantly when embedding integers into C source: `to_string(i)`, `to_string(argc)`, `to_string(line)`, `to_string(node.value)`. This is pervasive — at least 30 occurrences in codegen.gem. Implicit coercion would help even without string interpolation.

### 7. `has_key` / `in` operator for tables
Currently checking key existence with `table[key] != nil`. A `has_key(table, key)` builtin or `key in table` syntax would be clearer.

### 10. `keys()` as a builtin
The codegen needs `keys()` to iterate table fields during AST walking (free variable analysis, capture analysis). This required adding `gem_keys()` to the C runtime and declaring it via `extern fn`. It should be a builtin like `len()` and `type()` — accessible without extern declarations.

### 11. `match` on strings
The codegen has many tag-dispatch patterns (`if tag == "int" ... if tag == "float" ...`) that would naturally be `match` statements, but `match` uses `==` comparison which works on strings. The problem is `match` doesn't short-circuit on return — when a `when` branch returns, execution falls through checking subsequent branches. This is correct but wasteful. More importantly, using `match` here would require all the handler code to be in the `when` body, which doesn't help when each handler is 5+ lines. This is really the same issue as `elif` (item 2) — both solve tag dispatch. `elif` is strictly more useful since it handles arbitrary conditions, not just equality.

### 12. `str_replace` builtin
The codegen needs string replacement for re-indenting setup code in `and`/`or`/`while` short-circuit compilation. Had to write a 20-line `str_replace_all` function in Gem. A builtin `str_replace(s, old, new)` would be cleaner.

### 13. Reserved word `match` as variable name
`match` is a keyword, which means `let match = true` is a parse error. Hit this when porting `str_replace_all` — had to rename the variable to `found`. Other keywords that might conflict with common variable names: `end`, `do`, `when`. Not a high priority but worth noting.

## Debugging & Error Handling

### Source-mapped error messages
Runtime errors currently report C line numbers (`gem.c:847`), not Gem source locations. The compiler should emit `#line` directives or track source positions so that crashes point back to the `.gem` file and line. This was the single biggest time sink during development.

**Codegen note:** Confirmed — this was again the biggest debugging pain. When the codegen crashes (e.g., `len: expected string or table`), the error gives no indication of WHERE in the Gem source the problem is. Debugging required binary-search with print statements to narrow down which function, which loop iteration, which expression caused the crash. `#line` directives would have saved hours.

### Stack traces on error
`error()` kills the program with one message and no call stack. When a crash happens deep in the parser (called from a test harness, called from main), there's no way to see the call chain without manually adding print statements. At minimum, print a call stack on `error()`.

### Warn on shadowed/unreachable named `fn` inside functions
A named `fn` definition inside another function is silently ignored — the function compiles but never binds. This caused a subtle bug where `fn check(...)` couldn't access top-level `let` variables, producing silently wrong results. The compiler should emit a warning (or error) when a `fn_def` appears in a non-top-level scope.

### `_compile_stmt_return` completeness
The bootstrap's `_compile_stmt_return` didn't handle `dot_assign`, `index_assign`, `break`, `continue`, or `fn_def` — these fell through to `compile_expr` which produced `/* unknown node */` comments in the C output. This was a silent correctness bug: `set_add(defined_fns, name)` inside a function body was silently compiled to a no-op, causing the codegen to treat all user-defined functions as generic closure calls. Fixed in the bootstrap by adding an explicit case for these statement types (compile as statement + `return GEM_NIL`). This class of bug — a missing case that silently produces wrong output instead of erroring — is worth guarding against systematically.

### Error recovery (paradigm TBD)
Currently `error()` is fatal — no way to catch failures. This makes it impossible to write tests that verify the parser rejects bad input. Some form of recoverable errors is needed, but the right paradigm is an open question:
- **try/catch** (exceptions) — familiar, but adds complexity to codegen and runtime
- **pcall-style** (Lua) — `let ok, result = pcall(fn() ... end)` — simpler, no stack unwinding
- **Result types** — functional style, but needs sum types or conventions
- **Multiple return + error flag** — minimal, but verbose at call sites

Decision deferred until we have more experience with what the language needs.

### `extern fn` should support `Table` / `GemVal` return types
The `extern fn` type map only handled `Int`, `Float`, `String`, `Bool`, `Ptr`, and `Nil`. Unknown return types (including `Table`) silently discarded the return value and returned `GEM_NIL`. This caused `keys()` to always return nil. Fixed by adding `Table` as a passthrough type (return the `GemVal` directly). The silent failure mode was particularly nasty — no error, just nil propagating until something downstream crashes.

## Low Priority

### 8. Allow keywords as table literal keys
`"else"` is a keyword so `{else: body}` is a parse error. Currently requires `node["else"] = body` after construction. Could allow keywords in table literal key position since it's unambiguous.

### 9. Destructuring / multi-return
Minor ergonomic win for parser helpers and similar patterns. Not blocking anything.
