# Optimization TODOs

Future performance improvements. None are blocking — collect ideas here as they come up.

Priorities are informed by benchmark results from the bookmark CRUD app (2026-04-29):
- GET /bookmarks (20 rows): p50=1.60ms, p99=9.8ms, 5.3k req/s
- GET / (static HTML): p50=313us, p99=3.4ms, 26.5k req/s
- Soak (5min, 4c): p50=556us, p99=7.8ms, 5.2k req/s, RSS peak 790 MB
- POST /bookmarks: p50=25ms, p99=70ms, 159 req/s

Key bottlenecks: per-request GC allocation rate drives RSS; string concatenation patterns
that escape `build_string` still allocate per-concat. GC pauses are now sub-10ms.

Priority scale: **P0** = measurable impact on benchmark right now, **P1** = significant but
requires groundwork, **P2** = nice to have or niche.

## GC / Memory (P0)

### ~~Reuse tcp_read buffer per process~~ ✓ Done
`GemProcess` now has a `read_buf`/`read_buf_cap` pair. `tcp_read` allocates the read buffer once per process and reuses it across calls, copying only the actual bytes read into an exact-size GC string for the return value.

### ~~String builder for response construction~~ ✓ Done
`build_string` builtin added — creates a buffer, passes an `add` closure to the block, returns the finalized string. `push` and `to_string` now work on buffers. `std/http.gem` response serialization and `examples/bookmark_app/app.gem` HTML templates rewritten to use `build_string` blocks instead of `+` concatenation.

### ~~Reduce GC pause duration~~ ✓ Done
Tuned Boehm GC to collect more frequently with shorter pauses: `GC_set_free_space_divisor(6)` (was 3) triggers collection earlier, and the scheduler GC threshold was reduced to 10 completions / 1000 resumes (was 50/5000). Result: p99 dropped from 34ms to 7.8ms, RSS peak from 3.6 GB to 790 MB, at a 9% throughput cost. Incremental mode (`GC_enable_incremental`) was tested but its mprotect-based write barriers conflict with coroutine stack switching, tanking throughput by 40%.

## Value Representation (P1)

### NaN boxing
GemVal is currently 16 bytes (4-byte type enum + 8-byte union + padding). NaN boxing packs type + value into a single 8-byte double by exploiting the NaN payload space. Halves memory per value, improves cache locality, eliminates the type field branch in hot paths. The payoff isn't just memory — every function call, table lookup, and arithmetic op passes GemVals, so halving their size compounds across the entire runtime. Requires rewriting every GemVal constructor and accessor. Major undertaking but large payoff. Would also directly reduce GC pressure since every value, table entry, and function argument shrinks.

### Avoid hashing integers in tables (P2)
Integer keys currently fall through to a linear scan if they don't match the array-style fast path (sequential 0..n). A dedicated int hash map (parallel to `str_index`) would give O(1) lookup for sparse integer keys. Low priority — most integer keys follow the array pattern.

## Strings

### String views / slices (P1)
`substr` allocates a copy. A view (pointer + offset + length) into the original string would make substring extraction O(1). Requires changing the string representation in GemVal (add length field, stop assuming null-termination). Every string consumer (`strlen`, `strcmp`, `printf %s`) needs updating. Defer until profiling shows substring allocation as a real bottleneck — the C interop boundary assumes null-terminated strings throughout, and `substr`/`ord(s, i)` already cover the hot cases without changing the representation.

### String interning for short strings (P1)
Small strings (< 16 bytes) could be interned in a global table, turning equality checks into pointer comparison. Most table keys are short identifier strings — this would speed up every `gem_table_get`/`gem_table_set` with string keys. Trade-off: GC interaction (interned strings must be roots or use weak refs). Would also reduce GC allocation rate — repeated key lookups like `"tag"`, `"pid"`, `"url"` currently allocate a fresh string each time via `gem_string()`.

### `gem_string()` copies unconditionally (P2)
`gem_string(const char *s)` always allocates + memcpy. Callers that already have a GC-allocated string (e.g. `buf_str`) pay for a redundant copy. A `gem_string_own(char *s)` variant that takes ownership would eliminate this.

## Codegen Output

### ~~O(n²) string building in codegen~~ ✓ Done
All heavy string accumulators in `compiler/codegen.gem` rewritten to use `buf_new`/`buf_push`/`buf_str`. Eliminated ~188 O(n²) concatenations.

### `x = x + y` auto-optimization (P0)
The compiler could detect the self-append pattern `assign(name, binary("+", var(name), expr))` and emit buffer operations instead of `gem_add`. Would fix O(n²) string building globally without requiring source changes. Non-trivial compiler change — needs tracking of which variables are in "append mode". This is the highest-leverage compiler optimization for HTTP workloads where response bodies are built by repeated concatenation.

### Redundant `gem_push_frame` / `gem_pop_frame` (P1)
Every function call emits frame push/pop for stack traces. Leaf functions (no calls inside) could skip this. Requires analysis in the codegen to detect leaf status. Benchmark shows overhead in the hot path — eliminating frame tracking for leaf functions called per-request would help throughput.

### Escape analysis for non-escaping closures (P1)
Every `fn() ... end` heap-allocates a closure with its captured environment. Most closures never escape: `pcall(fn() ... end)`, `do ... end` blocks passed to iterators, `sort(arr, fn(a,b) ... end)`. If the compiler detects that a closure is only called within the local scope and never stored, it could stack-allocate or skip the closure allocation entirely. The pattern is syntactically detectable for the common cases (immediate call-site argument). Bigger lift but high frequency — closures are the primary abstraction mechanism. Directly reduces GC allocation rate.

### Constant folding (P2)
Expressions like `1 + 2` or `"hello" + " world"` could be evaluated at compile time. The compiler already has all the information. Low value at current stage — the cases where humans write foldable constants are rare, and it doesn't affect compilation time.

### Dead code elimination (P2)
Unreachable code after `return`, `break`, `error()` could be stripped. Currently emitted as-is.

### ~~Self-recursive tail call optimization~~ ✓ Done
Codegen detects self-recursive calls in tail position (last expression in function body, propagating through if/else, match, receive, and block branches) and emits a `while(1)` loop with parameter reassignment + `continue` instead of a recursive call. `gem_push_frame` runs once on entry; `gem_yield_check` runs each iteration for cooperative scheduling. Multi-param reassignment uses temps to avoid ordering issues. Boxed (closure-captured) params reassign through the pointer. Functions with rest/block params or shadowed names skip TCO. Covers ~95% of OTP patterns (gen_server loops, supervisor restarts, recursive receive handlers).

### Mutual tail call optimization (P2)
Mutual recursion (A calls B in tail position, B calls A) could use trampolines or computed goto. Lower priority — rare in OTP patterns. Self-recursive TCO already covers the common case.

## Table Access

### ~~Inline caching for `.field` access~~ ✓ Done
Codegen emits a `static GemICacheSlot` per `.field` access site. On cache hit (same table + same shape_id), returns `t->vals[cached_index]` directly — no hash, no `gem_string()` allocation. Cache miss falls back to full `shgeti` lookup and populates the cache. `shape_id` on `GemTable` is bumped by structural mutations (delete, pop, sort, insert, remove_at) but not by set/push (which don't move existing key→index mappings). Monomorphic (1 slot per site) — sufficient for AST walking where each access site typically sees one table shape.

### ~~`for k, v in tbl` allocates a keys array~~ ✓ Done
Desugaring now uses `__table_key_at` / `__table_val_at` to index directly into the table's storage arrays. No keys array allocation, no per-key re-lookup.

## Runtime Hot Paths

### `gem_eq` for strings (P2)
Currently `strcmp`. If string interning lands, short strings become pointer equality. Even without interning, caching string length would let us short-circuit on length mismatch before comparing bytes.

### `gem_add` for strings (P1)
Every string `+` does `strlen` on both operands. If strings carried their length, this becomes a field read. Depends on string views/length-aware representation. Directly impacts the HTML response building hot path.

### `buf_push` specialization for non-strings (P2)
`buf_push` auto-coerces non-string values via `to_string`, allocating a temporary string. Specialized variants (`buf_push_int`, `buf_push_float`) that write directly into the buffer would skip the allocation. Small win per call but high frequency in formatting-heavy code.

### Constructor return-by-value (P1)
`gem_int()`, `gem_float()`, `gem_bool()`, `gem_string()` all return `GemVal` by value (16 bytes). With NaN boxing these become trivial bit operations returning 8 bytes. Without NaN boxing, the compiler could use static inline or macros for the trivial constructors. Blocked on NaN boxing for the full win.

### Table grow strategy (P2)
`gem_table_grow` doubles capacity. Could use a growth factor of 1.5 to reduce memory waste, or start with capacity 0 (no allocation) for tables that might stay empty.

## Runtime I/O

### ~~Thread pool for async I/O (Phase 1)~~ ✓ Done
4 OS worker threads (`runtime/gem_threadpool.c`) handle `read_file`, `write_file`, `append_file`, `exec`, and `extern blocking fn` when called from a spawned process. The coroutine yields on submission; a wake-pipe notifies the scheduler on completion. Top-level (non-coroutine) I/O remains synchronous. The Gem-facing API is unchanged.

### ~~Non-blocking sockets for TCP builtins~~ ✓ Done
TCP builtins (`tcp_accept`, `tcp_read`, `tcp_write`, `tcp_connect`) use non-blocking sockets + `gem_io_yield(fd, direction)` to yield to the scheduler's `poll()` set. No thread pool involvement — the scheduler resumes the coroutine directly when the fd is ready.

**Why not the thread pool?** Benchmarked both approaches with `wrk -t4 -c100 -d10s` against an HTTP server:

| Approach | Req/sec | Notes |
|----------|---------|-------|
| Non-blocking + poll (net.c extern fn) | ~24,000 | Original baseline |
| Thread pool (tcp builtins v1) | ~1,550 | 15x slower — each request needs 3 thread pool round trips (accept/read/write), 4 workers caps throughput |
| Non-blocking + poll (tcp builtins v2) | ~23,800 | Matches baseline |

The thread pool adds per-operation overhead: mutex lock → enqueue → cond signal → worker thread pickup → execute → wake-pipe write → scheduler drain → process scan → resume. For socket ops on localhost where the actual I/O is microseconds, this coordination overhead dominates. With 4 workers and 3 ops/request, max throughput is ~4/3 ≈ 1.3k req/s — matches the 1,550 observed.

**Lesson:** Thread pool is correct for file I/O and `exec` (where the kernel provides no readiness notification). For sockets, always use non-blocking + readiness notification (poll, kqueue, epoll).

### Selective receive save-queue optimization (P2)
`receive ... when` scans the mailbox from oldest to newest on every wake. If a process accumulates many messages and the match is near the end, that's O(n) pattern matches per wake. Erlang's optimization: remember which messages were already tested against the current receive and skip them on re-scan, only testing newly arrived messages. Non-trivial but maps onto the existing mailbox structure — a "scan cursor" per process that advances as messages are rejected and resets when the receive shape changes or a new message arrives.

### Timer min-heap (P2)
Timers are stored in a fixed-size array (256 slots) with linear scan for insert, fire, cancel, and earliest-deadline lookup. Replace with a dynamic min-heap keyed by deadline. Insert and cancel become O(log n), `gem_fire_timers` pops expired entries from the front in O(log n) per fired timer, and `gem_earliest_timer_deadline` becomes O(1) (peek the root). Removes the 256-slot cap — unlimited concurrent timers without hitting "timer table full". Matters for applications with many gen_servers or heavy `send_after` usage.

### kqueue/epoll for sockets (P2)
The scheduler currently uses `poll()` for socket readiness. Replacing with **kqueue** (macOS/BSD) or **epoll** (Linux) would improve scalability at high connection counts (thousands of fds). `poll()` scans the entire fd set on each call — O(n) per wake. kqueue/epoll return only ready fds — O(ready). For the current HTTP server benchmark (~100 concurrent connections), `poll()` is not the bottleneck; this optimization matters when scaling to thousands of simultaneous connections.

## std/json

### Null byte handling in strings (P2)
C strings are null-terminated, so embedded `\x00` bytes are invisible to the parser. This causes one JSONTestSuite failure: `n_multidigit_number_then_00` (a number followed by a null byte parses as valid because the null truncates the input). Fixing this requires length-aware strings throughout the runtime — depends on the string views/slices work above. When strings carry their length, the scanner can detect unexpected null bytes and reject them.

### Fast path for escape-free strings in parse (P2)
`parse_string` always allocates a buffer and pushes byte-by-byte. Most JSON strings contain no escapes. A fast path that scans for the closing `"` first (checking for `\` along the way) and uses `substr` when no escapes are found would avoid the buffer allocation entirely. 2-3x speedup on string-heavy JSON.

### Scanner as plain table instead of closure (P2)
The closure-based scanner (`{peek, advance, skip_ws}`) pays for hashmap lookup + closure call + captured variable access on every character. A flat table `{input, pos, length}` with module-level functions `peek(s)`, `advance(s)`, `skip_ws(s)` avoids closure overhead. More idiomatic for a language without methods.
