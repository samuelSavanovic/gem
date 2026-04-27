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

### ~~Self-recursive tail call optimization~~ ✓ Done
Codegen detects self-recursive calls in tail position (last expression in function body, propagating through if/else, match, receive, and block branches) and emits a `while(1)` loop with parameter reassignment + `continue` instead of a recursive call. `gem_push_frame` runs once on entry; `gem_yield_check` runs each iteration for cooperative scheduling. Multi-param reassignment uses temps to avoid ordering issues. Boxed (closure-captured) params reassign through the pointer. Functions with rest/block params or shadowed names skip TCO. Covers ~95% of OTP patterns (gen_server loops, supervisor restarts, recursive receive handlers).

### Mutual tail call optimization
Mutual recursion (A calls B in tail position, B calls A) could use trampolines or computed goto. Lower priority — rare in OTP patterns. Self-recursive TCO already covers the common case.

## Runtime I/O

### Thread pool for async I/O — HIGH PRIORITY
The scheduler currently has no I/O integration — any file or network operation blocks the entire coroutine scheduler. All spawned coroutines effectively run sequentially if they do I/O. This breaks the core concurrency promise.

**Phase 1: Thread pool.** Spawn N OS worker threads at startup. When a coroutine calls `read_file`/`write_file`/`net_*`, the scheduler hands the operation to a worker thread, suspends the coroutine, and resumes it when the thread completes. A wake-pipe (the worker writes a byte when done) lets the scheduler poll for completions without busy-waiting. This fixes all I/O — files, sockets, everything — and the Gem-facing API doesn't change. Existing code just starts working concurrently.

**Phase 2: kqueue/epoll for sockets.** After the thread pool works, socket operations (accept, read, write) can be pulled out of the pool and into a platform event loop: **kqueue** (macOS/BSD), **epoll** (Linux), **IOCP** (Windows). Lower latency and no thread overhead per socket op. File I/O stays in the thread pool since kqueue/epoll report regular files as always ready. For true async disk I/O, **io_uring** (Linux 5.1+) is an option; macOS/Windows stay with the thread pool.

The scheduler loop becomes:
1. Run ready coroutines until they yield
2. Check wake-pipe for thread pool completions → mark coroutines ready
3. Check kqueue/epoll for socket events → mark coroutines ready (phase 2)
4. If nothing ready, block on kevent/epoll_wait with timeout
