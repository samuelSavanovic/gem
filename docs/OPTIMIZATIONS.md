# Optimization TODOs

Future performance improvements. None are blocking — collect ideas here as they come up.
Prioritize by real-world impact (the compiler is the benchmark).

## Value Representation

### NaN boxing
GemVal is currently 16 bytes (4-byte type enum + 8-byte union + padding). NaN boxing packs type + value into a single 8-byte double by exploiting the NaN payload space. Halves memory per value, improves cache locality, eliminates the type field branch in hot paths. Requires rewriting every GemVal constructor and accessor. Major undertaking but large payoff for value-heavy code.

### Avoid hashing integers in tables
Integer keys currently fall through to a linear scan if they don't match the array-style fast path (sequential 0..n). A dedicated int hash map (parallel to `str_index`) would give O(1) lookup for sparse integer keys. Low priority — most integer keys follow the array pattern.

## Strings

### String views / slices
`substr` allocates a copy. A view (pointer + offset + length) into the original string would make substring extraction O(1). Requires changing the string representation in GemVal (add length field, stop assuming null-termination). Every string consumer (`strlen`, `strcmp`, `printf %s`) needs updating. See `docs/SELF_HOST_FOLLOWUPS.md` for details.

### String interning for short strings
Small strings (< 16 bytes) could be interned in a global table, turning equality checks into pointer comparison. Most table keys are short identifier strings — this would speed up every `gem_table_get`/`gem_table_set` with string keys. Trade-off: GC interaction (interned strings must be roots or use weak refs).

### `gem_string()` copies unconditionally
`gem_string(const char *s)` always allocates + memcpy. Callers that already have a GC-allocated string (e.g. `buf_str`) pay for a redundant copy. A `gem_string_own(char *s)` variant that takes ownership would eliminate this.

## Codegen Output

### ~~O(n²) string building in codegen~~ ✓ Done
All heavy string accumulators in `compiler/codegen.gem` rewritten to use `buf_new`/`buf_push`/`buf_str`. Eliminated ~188 O(n²) concatenations.

### `x = x + y` auto-optimization
The compiler could detect the self-append pattern `assign(name, binary("+", var(name), expr))` and emit buffer operations instead of `gem_add`. Would fix O(n²) string building globally without requiring source changes. Non-trivial compiler change — needs tracking of which variables are in "append mode".

### Redundant `gem_push_frame` / `gem_pop_frame`
Every function call emits frame push/pop for stack traces. Leaf functions (no calls inside) could skip this. Requires analysis in the codegen to detect leaf status.

## Runtime Hot Paths

### `gem_eq` for strings
Currently `strcmp`. If string interning lands, short strings become pointer equality. Even without interning, caching string length would let us short-circuit on length mismatch before comparing bytes.

### `gem_add` for strings
Every string `+` does `strlen` on both operands. If strings carried their length, this becomes a field read. Depends on string views/length-aware representation.

### Constructor return-by-value
`gem_int()`, `gem_float()`, `gem_bool()`, `gem_string()` all return `GemVal` by value (16 bytes). With NaN boxing these become trivial bit operations returning 8 bytes. Without NaN boxing, the compiler could use static inline or macros for the trivial constructors.

### Table grow strategy
`gem_table_grow` doubles capacity. Could use a growth factor of 1.5 to reduce memory waste, or start with capacity 0 (no allocation) for tables that might stay empty.

## Compiler

### Constant folding
Expressions like `1 + 2` or `"hello" + " world"` could be evaluated at compile time. The compiler already has all the information.

### Dead code elimination
Unreachable code after `return`, `break`, `error()` could be stripped. Currently emitted as-is.

### Tail call optimization — HIGH PRIORITY
Critical for the OTP concurrency model. Gen_server loops, supervisor restarts, and recursive message handlers all rely on self-recursive tail calls that currently grow the stack unboundedly. Prioritize **self-recursive TCO** (function calls itself in tail position) — covers ~95% of OTP patterns. Implementation: codegen detects tail self-calls and emits a `while(1)` loop with parameter reassignment instead of a recursive call. Key challenges: tail position detection across if/else/match branches, `gem_push_frame`/`gem_pop_frame` handling (push once on entry, no pop on loop iteration), multi-param reassignment ordering (needs temps), and boxed param reassignment for closure-captured variables. Mutual recursion (trampolines/goto) is lower priority — rare in OTP patterns.

## Runtime I/O

### Non-blocking I/O for the scheduler
The scheduler currently has no I/O integration — any file or network operation blocks the entire coroutine scheduler. Platform-specific event loops would let coroutines yield on I/O and be resumed when data is ready, matching OTP's non-blocking model. Platform APIs: **kqueue** (macOS/BSD), **epoll** (Linux), **IOCP** (I/O Completion Ports, Windows). Note: these work for sockets, pipes, and network I/O — regular disk files always report as ready. For true async disk I/O, **io_uring** (Linux) is the solution; macOS/Windows require thread pools. Start with kqueue (current dev platform) for socket I/O, then abstract behind a platform layer.
