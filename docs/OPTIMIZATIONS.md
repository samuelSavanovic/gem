# Optimization TODOs

Future performance improvements. None are blocking — collect ideas here as they come up.

Priorities are informed by benchmark results from the bookmark CRUD app.

TCO arena reset (2026-04-30, post commit 7c376ed, default 16 MB threshold):
- GET / baseline (c4 t2 30s): p50=141us, p99=2.96ms, 25.6k req/s, RSS 50–88 MB / 37 MB idle
- GET / c100 (t4 30s): p50=3.17ms, p99=26.5ms, 28.9k req/s, RSS peak 2.04 GB / 495 MB idle
- GET / c500 (t8 30s): p50=16.7ms, p99=236ms, 26.4k req/s, RSS peak 5.83 GB / 2.39 GB idle
- GET /bookmarks c50 (t4 30s, 30 rows): p50=11.3ms, p99=28.8ms, 4.0k req/s, RSS peak 3.10 GB
- Soak GET / (c4 t2 5min, 7.5M req): p50=142us, p99=10.4ms, 25.0k req/s, RSS 54–82 MB throughout, returns to 37 MB idle — no drift
- Mixed reads (c20) + writes (10/s POST, 60s): 27.4k req/s reads, p99=4.41ms, RSS peak 307 MB, 528 bookmarks created — SQLite write path bounded

Threshold sweep at c100 t4 30s on GET /:
| Threshold | RPS    | p50    | p99    | RSS peak | Post-idle |
|-----------|--------|--------|--------|----------|-----------|
| 1 MB      | 28.8k  | 3.23ms | 7.32ms | 139 MB   | 50 MB     |
| 16 MB     | 28.9k  | 3.17ms | 26.5ms | 2.04 GB  | 495 MB    |
| 64 MB     | 27.9k  | 3.24ms | 48.5ms | 5.90 GB  | 1.67 GB   |

`/bookmarks` validation at c50 t4 30s, 30 rows (heavier per-request allocation: SQLite query + HTML render):
| Threshold | RPS    | p50     | p99     | RSS peak | Post-idle |
|-----------|--------|---------|---------|----------|-----------|
| 1 MB      | 4041   | 11.65ms | 22.98ms | 52 MB    | 22 MB     |
| 16 MB     | 4007   | 11.40ms | 26.91ms | 728 MB   | 58 MB     |

Same conclusion as `/`: no throughput regression, p99 −15%, RSS peak 14× lower. Default lowered to 1 MB.

c=500 stress retest on GET / after the flip (t8 30s):
| Threshold | RPS   | p50     | p99     | RSS peak | Post-idle |
|-----------|-------|---------|---------|----------|-----------|
| 16 MB     | 26.4k | 16.7ms  | 236ms   | 5.83 GB  | 2.39 GB   |
| 1 MB      | 28.0k | 16.3ms  | 38.8ms  | 612 MB   | 174 MB    |

Throughput +6%, p99 −6×, RSS peak −9.5×, post-idle high-water −14×.

Throughput plateaus at ~26–29k req/s on `/` regardless of c=4/100/500 — single-threaded scheduler is the ceiling. Higher concurrency just queues. Per-connection process arenas dominate memory under load.

Arena allocator (2026-04-29, post Boehm GC removal):
- GET /bookmarks (20 rows): p50=758us, p99=3.26ms, 4.0k req/s
- GET / (static HTML): p50=292us, p99=1.55ms, 30.3k req/s
- Soak (60s, 4c): p50=758us, p99=3.26ms, 4.0k req/s, RSS peak 1.6 GB
- POST /bookmarks: p50=2.14ms, p99=4.69ms, 4.1k req/s

Boehm GC baseline (2026-04-29, before arena migration):
- GET /bookmarks (20 rows): p50=1.60ms, p99=9.8ms, 5.3k req/s
- GET / (static HTML): p50=313us, p99=3.4ms, 26.5k req/s
- Soak (5min, 4c): p50=556us, p99=7.8ms, 5.2k req/s, RSS peak 790 MB
- POST /bookmarks: p50=25ms, p99=70ms, 159 req/s

Arena wins: p99 latency 2-3x lower (no GC pauses), POST throughput 26x higher.
Arena costs: RSS ~2x higher (arenas don't compact), GET throughput ~similar.

Key bottlenecks: per-process arena allocation eliminates GC pauses but arenas only grow during a process's lifetime; string concatenation patterns that escape `build_string` still allocate per-concat.

Priority scale: **P0** = measurable impact on benchmark right now, **P1** = significant but
requires groundwork, **P2** = nice to have or niche.

## Arena / Memory (P0)

### ~~Lower default `GEM_ARENA_RESET_THRESHOLD`~~ ✓ Done (2026-04-30)
Default lowered from 16 MB to 1 MB. Threshold sweep at c100 on `/` showed 1 MB strictly dominates: same throughput (28.8k vs 28.9k req/s), p99 −3.6× (26.5ms → 7.3ms), peak RSS −14× (2.04 GB → 139 MB), idle RSS −10× (495 MB → 50 MB). `/bookmarks` validation at c50 (heavier per-request allocation) confirmed no regression: throughput unchanged (4041 vs 4007 req/s), p99 −15% (26.9ms → 23.0ms), peak RSS −14× (728 MB → 52 MB). The hypothesis "smaller threshold = more reset overhead" did not show up in numbers — live set after a request is tiny so reset cost is negligible.

### Investigate post-idle high-water at high concurrency (P2 — downgraded 2026-04-30)
Originally reported at 2.39 GB stuck post-c=500 with the 16 MB threshold. After lowering the default threshold to 1 MB, post-idle RSS at c=500 dropped to 174 MB — flat across +0/+30/+90s probes, so it's still not draining, but the absolute waste is now an order of magnitude smaller and unlikely to matter for typical workloads. The underlying mechanism (per-process arenas of completed connection handlers not fully releasing — likely `madvise(DONTNEED)` happens but `munmap` doesn't, or proc-table objects linger until late cleanup) is unchanged. Downgrade P1 → P2: keep tracking but don't prioritize until a workload demonstrates the residual is a real problem. If revisited, trace `gem_proc_exit` against actual mmap accounting under load.

### Arena compaction for long-running processes (P1)
Arenas only grow — dead objects (replaced table entries, overwritten variables) waste space until the process exits. For long-running processes (gen_servers, accept loops), memory usage creeps up. Periodic compaction (copy live data into a fresh arena, swap, free old blocks) would reclaim this waste. Requires a root-scanning mechanism to find all live references from the coroutine stack and closure envs.

### Growable structure waste (P1)
Tables and arrays allocated in arenas can't individually shrink or free their old backing arrays when they grow. Each `gem_table_grow` doubles capacity, leaving the old keys/vals arrays as dead space in the arena. For tables that grow incrementally (e.g. route tables, accumulators), this wastes up to 2x the live data. Copy-on-grow (allocate new backing in the arena, accept old waste) is the current approach. Compaction (above) would reclaim this.

### ~~Deep copy optimization for shared immutable data~~ ✓ Done
Module tables are marked immutable (`gem_table_freeze`) by the compiler after construction. `gem_deep_copy_internal` skips copying immutable tables, sharing them read-only across all processes. Strings from the main process arena are also shared (all strings are immutable; the main arena is never freed). User-created mutable tables are still deep-copied correctly. Bookmark app soak (60s, 4c, GET /bookmarks): p99 1.90ms → 1.45ms (−24%), throughput 4,816 → 4,986 req/s (+3.5%), RSS 1,511 → 1,236 MB (−18%).

### ~~Reuse tcp_read buffer per process~~ ✓ Done
`GemProcess` now has a `read_buf`/`read_buf_cap` pair. `tcp_read` allocates the read buffer once per process and reuses it across calls, copying only the actual bytes read into an exact-size string for the return value.

### ~~String builder for response construction~~ ✓ Done
`build_string` builtin added — creates a buffer, passes an `add` closure to the block, returns the finalized string. `push` and `to_string` now work on buffers. `std/http.gem` response serialization and `examples/bookmark_app/app.gem` HTML templates rewritten to use `build_string` blocks instead of `+` concatenation.

### ~~Eliminate GC pauses~~ ✓ Done
Replaced Boehm GC with per-process arena allocation. No global stop-the-world pauses. Each process allocates from its own bump allocator; memory is freed in bulk when the process exits.

## Value Representation (P1)

### NaN boxing
GemVal is currently 16 bytes (4-byte type enum + 8-byte union + padding). NaN boxing packs type + value into a single 8-byte double by exploiting the NaN payload space. Halves memory per value, improves cache locality, eliminates the type field branch in hot paths. The payoff isn't just memory — every function call, table lookup, and arithmetic op passes GemVals, so halving their size compounds across the entire runtime. Requires rewriting every GemVal constructor and accessor. Major undertaking but large payoff. Would also directly reduce arena allocation pressure since every value, table entry, and function argument shrinks.

### Avoid hashing integers in tables (P2)
Integer keys currently fall through to a linear scan if they don't match the array-style fast path (sequential 0..n). A dedicated int hash map (parallel to `str_index`) would give O(1) lookup for sparse integer keys. Low priority — most integer keys follow the array pattern.

## Strings

### String views / slices (P1)
`substr` allocates a copy. A view (pointer + offset + length) into the original string would make substring extraction O(1). Requires changing the string representation in GemVal (add length field, stop assuming null-termination). Every string consumer (`strlen`, `strcmp`, `printf %s`) needs updating. Defer until profiling shows substring allocation as a real bottleneck — the C interop boundary assumes null-terminated strings throughout, and `substr`/`ord(s, i)` already cover the hot cases without changing the representation.

### String interning for short strings (P1)
Small strings (< 16 bytes) could be interned in a global table, turning equality checks into pointer comparison. Most table keys are short identifier strings — this would speed up every `gem_table_get`/`gem_table_set` with string keys. Trade-off: interned strings must live in a shared arena or be reference-counted so they outlive individual process arenas. Would also reduce per-process allocation rate — repeated key lookups like `"tag"`, `"pid"`, `"url"` currently allocate a fresh string each time via `gem_string()`.

### `gem_string()` copies unconditionally (P2)
`gem_string(const char *s)` always allocates + memcpy. Callers that already have an arena-allocated string (e.g. `buf_str`) pay for a redundant copy. A `gem_string_own(char *s)` variant that takes ownership would eliminate this.

## Codegen Output

### ~~O(n²) string building in codegen~~ ✓ Done
All heavy string accumulators in `compiler/codegen.gem` rewritten to use `buf_new`/`buf_push`/`buf_str`. Eliminated ~188 O(n²) concatenations.

### ~~`x = x + y` auto-optimization~~ ✓ Done
The compiler detects the self-append pattern `assign(name, binary("+", var(name), expr))` inside `while`/`for` loop bodies and emits `gem_string_append`/`gem_string_finish` instead of `gem_add`. Eligible variables must only appear in append patterns within the loop body (no non-append reads). Handles chained concatenation (`x = x + a + b + c`), conditional appends inside `if`/`match`, and nested loops. Runtime fallback for non-string types (integers, floats) ensures correctness without static type information.

### Redundant `gem_push_frame` / `gem_pop_frame` (P1)
Every function call emits frame push/pop for stack traces. Leaf functions (no calls inside) could skip this. Requires analysis in the codegen to detect leaf status. Benchmark shows overhead in the hot path — eliminating frame tracking for leaf functions called per-request would help throughput.

### ~~Escape analysis for non-escaping closures (P1, partial)~~ ✓ Done (2026-04-30)
A pre-codegen pass marks `anon_fn` nodes that appear as the immediate argument of an allowlisted callee (`pcall`, `sort`) as `non_escaping`, provided the closure body contains no nested `anon_fn`s. Marked closures (a) skip contributing their captures to the enclosing function's `captured` set, so the enclosing params/locals stay unboxed, and (b) get a stack-allocated env in `compile_anon_fn` instead of `GC_MALLOC` from the arena. `spawn`/`spawn_link`/`spawn_monitor`/`send_after` deliberately stay off the allowlist — their closures run asynchronously in another coroutine and must keep the boxing-via-arena path. The conservative "no nested anon_fns" precondition sidesteps the case where an escaping inner closure could smuggle outer captures past the synchronous call boundary.

Concrete payoff: `safe_handle_request` deleted from `std/http.gem` and inlined into `handle_connection_loop`, which now has unboxed params and a stack-allocated pcall env. The `accept_loop` `let fd = client_fd` aliasing is **kept** — it dodges the *spawn* closure capture, which is still escaping.

Scope deferred: block-syntax `each`/`map`/`filter`/`reduce` from std/table aren't allowlisted yet (their callee parses as `dot`, not `var`, and they're regular Gem fns rather than builtins where we can audit storage behavior). Same goes for `build_string`. Add to allowlist when a workload demands it.

### TCO arena reset boundary — current state (2026-04-30)
Process-loop arena resets fire at TCO back-edges when `gem_call_depth - entry_call_depth <= 2` (depth-2 fence). The fence prevents the reset from running deep inside a request handler where the arena holds live values from the caller — without it, the reset corrupts pointers held above the TCO frame.

This is the shipping mechanism on `tco-watermark-revert-explore` (commits `fb8186c`, `40c29fd`, `354f4fa`). Two earlier attempts to remove the fence were reverted:
- **Per-frame watermark** (`ef77df5`, reverted `40c29fd`) — moved the boundary inside each TCO frame via `gem_arena_mark()` on entry and `reset_to_mark` at the back-edge. Eliminated depth coupling but regressed `/bookmarks` 4041 → 3380 req/s because every TCO function paid for a mark + truncation pass on every iteration, and the rescue was a 2× deep_copy of mutable roots (notably `router_ref`).
- **Single-copy splice** (`2eeda74`, reverted `fb8186c`) — eliminated the rescue's 2× by splicing the new arena block ahead of the surviving block instead of copying back. Recovered `/bookmarks` to ~3318 req/s and recovered the gen_server soak fully (217k → 235k ops/sec). Stranded `mark.block->cap - mark.used` bytes per reset (RSS bounded, VSZ grows).

Alternating-pair re-bench on the revert branch (3 pairs c=50 t=4 30s on `/bookmarks`):

| | revert (depth-2 + escape) | main (watermark + splice + escape) |
|---|---|---|
| Median req/s | 3120 | 3122 |
| RSS median | 42–44 MB | 45–48 MB |

Throughput identical within noise; revert wins ~3 MB on RSS (no stranded splice blocks). The watermark+splice machinery bought neither throughput nor memory on this workload.

The 4041 baseline was on a different machine state — not a clean comparison point on the current machine.

### Static process-tail analysis (P0, next)
Replace the runtime depth-2 fence with a compile-time mark. A pre-codegen pass walks every TCO function and asks: is this function reachable only via tail calls from a process-entry boundary (`spawn`/`spawn_link`/`spawn_monitor` closure, or `main`)? If yes, mark it `process_tail` — the codegen emits an unconditional reset at the back-edge (drop the `gem_call_depth - entry_call_depth <= 2` runtime check). If no, emit a compile-time warning ("this loop won't reset because it's reachable from a non-tail context") and skip the reset.

**Why this matters**: the depth-2 fence is a hidden runtime constraint. Today it works empirically (every long-running process loop *is* at depth 2 by construction), but any future user who structures a process as `spawn(fn() helper(); loop() end)` would hit a silent perf cliff with no diagnostic. Static analysis turns the silent constraint into either a guarantee or a compile-time warning.

**Implementation sketch** (~80–120 LOC, modeled on the `mark_non_escaping` pass at `compiler/codegen.gem:674-718`):
1. Build a call graph keyed by function name. For each call site, record (caller, callee, is_tail_position).
2. Seed the worklist with: top-level statements in `main`, and every `anon_fn` body passed to `spawn`/`spawn_link`/`spawn_monitor`.
3. BFS: from each seed, follow only tail-position edges. Mark every visited function `process_tail`.
4. In `compile_tail_call`, if the enclosing function is `process_tail`, drop the depth check from the emitted reset.
5. After analysis, walk the AST one more time and warn for any TCO function reachable from non-tail call sites only.

Expected throughput impact: small (the depth check is one cmp + branch, predictable). The win is DX — the user-facing constraint disappears. After this lands, the depth fence is no longer the *mechanism* (the static mark is); the runtime check exists only as a belt-and-suspenders safety for cases the analysis can't prove (and emits a warning).

### Eliminate user-visible recursion-as-iteration (P1, follow-on to static process-tail)
**Goal**: a process loop is `while true ... end`, not a tail-recursive helper. Recursion stays in the language for state-machine patterns; it stops being *required* for long-running processes. This is the structural fix that closes the gap the depth-2 fence and the watermark were both trying to paper over.

**Why it matters**: recursion-as-iteration is the steepest part of the OTP cognitive ramp. New users hit `while true` first (intuitive), then bounce off the warning telling them to rewrite as a tail-recursive helper. Even experienced Erlang users find it awkward — it's a runtime/codegen requirement leaking into syntax. Removing it means a Gem process loop reads like every other loop in every other language.

**Mechanism**: the same arena-rescue-and-reset machinery that TCO uses today, lifted from "tail-call back-edge" to "loop back-edge". Requires:
1. **Liveness analysis on `while` bodies** (~150 LOC, standard textbook pass) — at each back-edge, compute the live-out set: variables `read` on a later iteration. That's exactly what needs to survive arena reset.
2. **Reuse the static process-tail mark** to identify which `while true` loops are at process scope (reachable only via tail calls from a `spawn` closure or `main`). Loops in non-process contexts don't get reset; they keep allocating into the parent arena (correct behavior).
3. **Codegen at the back-edge** — emit `rescue(live_set); arena_reset; continue;` — identical to the TCO back-edge today, just driven by the syntactic loop boundary instead of a self-recursive call.
4. **Refusal cases** — analysis must detect when a value escapes the loop body via mechanisms it can't track (e.g. `register("name", state)` where state is mutable + arena-allocated). For these, emit a compile-time warning identifying the escape and skip the reset for that loop. Same warning model as the static process-tail pass.

**What stays the same**: TCO machinery, `spawn` closures, gen_server / supervisor patterns. Tail-recursive functions still work exactly as today — this just makes the simpler construct (`while true`) work too.

**What changes for users**:
- The current warning *"`while true` indefinite loop — long-running processes should use tail recursion instead so the runtime can reset the per-process arena"* gets replaced with a much narrower one that fires only when the loop has an unrescuable escape.
- A user can now write a gen_server's main loop as a plain `while true` block. Recursion remains available for those who want it.
- The spec's "long-running processes use tail recursion" rule becomes "long-running processes use `while true`; recursion is for state-machine transitions when convenient".

**Bar to land**: liveness bugs = silent memory corruption (same risk class as the watermark had). The pass must be conservative-by-default — when in doubt about whether a value is live, treat it as live (over-rescue is wasteful, under-rescue is unsound). Test exhaustively against existing TCO process loops (their tail params are by construction the live set; the analysis must agree on every existing example before being trusted on new code).

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

## Scheduler / Concurrency

### Multi-threaded work-stealing scheduler (P2)
The scheduler is single-threaded — one `while(1)` loop round-robining coroutines on one OS thread. N scheduler threads with per-thread run queues and work-stealing (Chase-Lev deque) would scale throughput ~linearly with cores. The per-process arena model already eliminates shared-heap contention. Hard parts: mailboxes need lock-free MPSC queues for cross-thread sends, shared globals (`gem_proc_table`, `gem_name_registry`, free list) need synchronization, each thread needs its own kqueue/epoll set, and process migration (stealing a coroutine between scheduler ticks) needs care. Erlang/BEAM does exactly this architecture. Nothing in the current design blocks it — isolated processes, message passing, and per-process memory are the right foundation.

## std/json

### Null byte handling in strings (P2)
C strings are null-terminated, so embedded `\x00` bytes are invisible to the parser. This causes one JSONTestSuite failure: `n_multidigit_number_then_00` (a number followed by a null byte parses as valid because the null truncates the input). Fixing this requires length-aware strings throughout the runtime — depends on the string views/slices work above. When strings carry their length, the scanner can detect unexpected null bytes and reject them.

### Fast path for escape-free strings in parse (P2)
`parse_string` always allocates a buffer and pushes byte-by-byte. Most JSON strings contain no escapes. A fast path that scans for the closing `"` first (checking for `\` along the way) and uses `substr` when no escapes are found would avoid the buffer allocation entirely. 2-3x speedup on string-heavy JSON.

### Scanner as plain table instead of closure (P2)
The closure-based scanner (`{peek, advance, skip_ws}`) pays for hashmap lookup + closure call + captured variable access on every character. A flat table `{input, pos, length}` with module-level functions `peek(s)`, `advance(s)`, `skip_ws(s)` avoids closure overhead. More idiomatic for a language without methods.
