# Self-Hosting Fast Followups

Language improvements to make right after the compiler can compile itself.
Prioritized by impact on real code (the compiler itself is the benchmark).

## High Priority

### 1. `for..in` loops
Every loop is `while` with a manual counter. The lexer, parser, codegen, and tests are full of:
```gem
let i = 0
while i < len(items)
  # use items[i]
  i += 1
end
```
Add `for x in items` (arrays) and `for i = 0, n` (ranges).

### 2. `elif`
The parser and codegen are long chains of `if ... end` repeated. With `elif`:
```gem
# before                      # after
if tag == "int"               if tag == "int"
  ...                           ...
end                           elif tag == "float"
if tag == "float"               ...
  ...                         elif tag == "string"
end                             ...
if tag == "string"            end
  ...
end
```

### 3. Multi-line function call arguments
The grammar currently disallows newlines between `(` and `)`. This forces long single-line calls:
```gem
check("compound assign desugars", "x += 1", "(assign x (binop + (var x) (int 1)))")
```
Allow newlines inside argument lists.

### 4. `push()` builtin
Every array build is `arr[count] = val; count += 1`. A builtin `push(arr, val)` would eliminate the manual count variable pattern.

## Medium Priority

### 5. String interpolation
Error messages and debug output require painful concat chains:
```gem
error("expected '" + tp + "' but got '" + t.type + "' at line " + to_string(t.line))
```
Something like `error("expected '{tp}' but got '{t.type}' at line {t.line}")` with auto `to_string`.

### 6. Implicit `to_string` in concatenation
`"count: " + to_string(n)` should just be `"count: " + n`. If string interpolation lands first, this becomes less urgent.

### 7. `has_key` / `in` operator for tables
Currently checking key existence with `table[key] != nil`. A `has_key(table, key)` builtin or `key in table` syntax would be clearer.

## Debugging & Error Handling

### Source-mapped error messages
Runtime errors currently report C line numbers (`gem.c:847`), not Gem source locations. The compiler should emit `#line` directives or track source positions so that crashes point back to the `.gem` file and line. This was the single biggest time sink during development.

### Stack traces on error
`error()` kills the program with one message and no call stack. When a crash happens deep in the parser (called from a test harness, called from main), there's no way to see the call chain without manually adding print statements. At minimum, print a call stack on `error()`.

### Warn on shadowed/unreachable named `fn` inside functions
A named `fn` definition inside another function is silently ignored — the function compiles but never binds. This caused a subtle bug where `fn check(...)` couldn't access top-level `let` variables, producing silently wrong results. The compiler should emit a warning (or error) when a `fn_def` appears in a non-top-level scope.

### Error recovery (paradigm TBD)
Currently `error()` is fatal — no way to catch failures. This makes it impossible to write tests that verify the parser rejects bad input. Some form of recoverable errors is needed, but the right paradigm is an open question:
- **try/catch** (exceptions) — familiar, but adds complexity to codegen and runtime
- **pcall-style** (Lua) — `let ok, result = pcall(fn() ... end)` — simpler, no stack unwinding
- **Result types** — functional style, but needs sum types or conventions
- **Multiple return + error flag** — minimal, but verbose at call sites

Decision deferred until we have more experience with what the language needs.

## Low Priority

### 8. Allow keywords as table literal keys
`"else"` is a keyword so `{else: body}` is a parse error. Currently requires `node["else"] = body` after construction. Could allow keywords in table literal key position since it's unambiguous.

### 9. Destructuring / multi-return
Minor ergonomic win for parser helpers and similar patterns. Not blocking anything.
