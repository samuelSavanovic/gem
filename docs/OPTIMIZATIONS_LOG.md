# Optimization Log

Shipped optimizations with their write-ups, work logs, and benchmark anchors. Forward-looking TODOs are in [`OPTIMIZATIONS.md`](OPTIMIZATIONS.md).

Kept for historical context — commit refs, design rationale, regression tests, and the work logs that explain *why* the chosen mechanism beat the alternatives. Grouped by the same categories as the active doc.

## Benchmark anchors

### TCO arena reset (2026-04-30, post commit 7c376ed, default 16 MB threshold)

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

### Arena allocator (2026-04-29, post Boehm GC removal)

- GET /bookmarks (20 rows): p50=758us, p99=3.26ms, 4.0k req/s
- GET / (static HTML): p50=292us, p99=1.55ms, 30.3k req/s
- Soak (60s, 4c): p50=758us, p99=3.26ms, 4.0k req/s, RSS peak 1.6 GB
- POST /bookmarks: p50=2.14ms, p99=4.69ms, 4.1k req/s

### Boehm GC baseline (2026-04-29, before arena migration)

- GET /bookmarks (20 rows): p50=1.60ms, p99=9.8ms, 5.3k req/s
- GET / (static HTML): p50=313us, p99=3.4ms, 26.5k req/s
- Soak (5min, 4c): p50=556us, p99=7.8ms, 5.2k req/s, RSS peak 790 MB
- POST /bookmarks: p50=25ms, p99=70ms, 159 req/s

Arena wins: p99 latency 2-3x lower (no GC pauses), POST throughput 26x higher.
Arena costs: RSS ~2x higher (arenas don't compact), GET throughput ~similar.

### Process-tail `while true` rescue+reset (2026-04-30)

Canonical comparison point for the `while true` rescue+reset codegen vs. the pre-mechanism baseline. Use as the reference when judging codegen-level changes to the back-edge or liveness pass.

- **Baseline**: `6b6999a` (pre-`while true` rescue path; depth-2 fence still in play). Logs: `benchmarks/logs/2026-04-30_baseline_quiet/`.
- **HEAD at capture**: `16f49a0` (rescue+reset shipped, shadow-fix applied). Logs: `benchmarks/logs/2026-04-30_head_quiet/`.
- **std/http dogfood**: `std/http` accept/handle loops rewritten as `while true`. Logs: `benchmarks/logs/2026-04-30_std_http_while_true/`. Read throughput within −1.0% / −2.4% / −0.4% of `head_quiet` (burst /bookmarks, burst /, soak); p50/p99 within ±2%; POST parity. RSS peak 50 MB vs 56 MB and growth 28 MB vs 37 MB — slightly improved, well within run-to-run noise. Confirms `while true` rescue+reset and self-recursive TCO emit equivalent back-edge code on a real workload.
- **Conditions**: same M1 Pro, quiet machine, back-to-back runs. `bash benchmarks/run.sh` defaults (5s warmup, 30s read bursts c=10, 5min soak c=4, 10s POST burst c=4).

| Phase | metric | baseline `6b6999a` | HEAD `16f49a0` | Δ |
|---|---|---|---|---|
| burst GET /bookmarks | req/s | 5558 | 5197 | **−6.5%** |
| burst GET /bookmarks | p50 / p99 | 1.55 ms / 28.89 ms | 2.07 ms / **4.80 ms** | p99 −6× |
| burst GET / | req/s | 30 629 | 31 090 | +1.5% |
| burst GET / | p50 / p99 | 279 µs / 5.63 ms | 287 µs / **662 µs** | p99 −8.5× |
| soak GET /bookmarks (5min) | req/s | 5427 | 5209 | **−4.0%** |
| soak GET /bookmarks (5min) | p50 / p99 | 530 µs / 33.95 ms | 756 µs / **1.73 ms** | p99 −20× |
| burst POST /bookmarks | req/s, p99 | 159 / 97.4 ms | 157 / 81.3 ms | parity |
| RSS over 5min soak | peak / growth | **3655 MB / 3636 MB** | **55 MB / 37 MB** | peak ÷66, growth ÷98 |

**Known regression — read-path p50 throughput, −4 to −6.5%**. Outside the ±2% noise band on `/bookmarks`. `/` (cheap static handler) and POST (SQLite-bound) are unaffected. Plausible cause: per-iteration arena `rescue+reset` work emitted at the `while true`/TCO back-edge of the accept and per-connection handler loops — it runs on every iteration regardless of whether the threshold gates the actual reset. The check itself (`gem_current_pid >= 0 && bytes_allocated > GEM_ARENA_RESET_THRESHOLD`) plus root-set construction is non-zero work in a tight HTTP request loop. Trade is intentional and favorable: tail latency collapses ~6–20× and memory growth is no longer unbounded over a soak. Investigating only worth it if a workload demonstrates the p50 hit matters more than the tail/memory wins.

## Arena / Memory

### Promote what a reset keeps ✓ Done (2026-10-04)

A reset used to copy what was live in its loop's region into blocks that still belonged to the region, so every later reset copied the same survivors again: a loop's kept data cost `live size × resets`. A `GemArenaMark` now holds two `GemArenaPoint`s, `base` (where the loop started) and `young` (the end of what its earlier resets kept). A reset normally frees only what is newer than `young` and then moves `young` past its copies, so kept data counts as older and later resets leave it alone; the barrier, the buffer walk and the pin `seq` already fix up older objects, so kept ones need nothing new. The copies go into the free tail of the block where the previous reset's copies ended, and new allocations start in a fresh block. A full reset from `base` drops the garbage among kept data: a mark's first reset, and any reset once the bytes kept since the last full one pass 2 × what it kept + `GEM_ARENA_RESET_THRESHOLD` (4 × when it kept at least half its region). The remembered log drops entries for tables the loop keeps; a young reset compacts only the entries since the young point, and the whole window since `base`'s epoch is compacted by full resets and once it has doubled. Each log entry and mailbox node a reset visits is charged to the budget (64 and 256 bytes), so a loop that writes many tables made before it, or holds a promoted mailbox backlog, spaces its resets out instead of walking them all at every threshold. A written table whose string keys moved gets its index re-pointed instead of rebuilt. `GEM_DIAG=1` also prints full resets (`full=`) and the longest reset (`max=`). Kept memory now stays below about twice (fourfold, when it is mostly live) what the last full reset kept, instead of 3 × the live data.

Before → after (Linux x86_64 VM, 4 cores, the same binaries rebuilt on each runtime; GEM_DIAG=1):
- `logstat --by ip` on 1M lines from a file: resets 0.87–0.99 s copying 526 MB → 0.32–0.43 s copying 0.7 MB (wall 5.4–5.9 s → 5.1–5.4 s).
- `examples/mini_redis` under `benchmarks/mini_redis/run.sh` (`PHASES="basic pipeline" N=50000`): resets 10.7 s → 3.6 s in `basic`, 12.5 s → 1.5 s in `pipeline`; copied 4.0 / 3.5 GB → 0.4 / 0.4 GB. LRANGE_100 p99 in `basic` 51 → 8.5 ms; LRANGE_600 in `pipeline` 2,074 → 3,903 requests/s (p99 509 → 278 ms). Other commands within the noise.
- `jobqueue` `backlog` (100,000 jobs): longest tick lag 445–555 ms → 300–355 ms (a run before this session's measured 1.4–1.7 s); peak RSS 780–960 MB → 472–475 MB; reset time unchanged at about 2 s, half of it now the walk of the written job table ("A kept table written once is walked whole" in OPTIMIZATIONS.md) and the longest reset a full one (0.32 s).
- `jobqueue --jobs 20000 --slow 0.1` (healthy 20 ms attempts killed by their 100 ms deadline during a pause): 8–16 per run → 0–5; peak RSS 180–194 → 101–104 MB. At 40,000 jobs 28 → 14.
- `gemgrep -rn e` on the 128 MB corpus: resets 1.33 s copying 2.1 GB → 0.94–1.0 s copying 1.6 GB (655 of 1,693 resets are a fresh mark's full first one).
- `lox binary_trees.lox 12`: copied 1.59 → 1.36 GB; the rest is fresh marks' first resets inside the recursion ("A loop's first reset is full" in OPTIMIZATIONS.md). `closures.lox`, `fib.lox` unchanged.
- Churn (a loop keeping 20,000 records in an older table and replacing or deleting one per iteration, 4M iterations): peak RSS 42 → 44 MB, wall 7.3 → 7.4 s; it copies 1.5 × as much (1.7 against 1.1 GB), since nearly every record lives long enough to be promoted.
- A process keeping one record per message it gets: at 100,000 records, resets 0.29 → 0.20 s, peak RSS 191 → 129 MB; at 300,000, resets 0.61 → 0.78 s and copied 214 → 316 MB (each record is copied at promotion and again by each full reset), peak RSS 381 → 288 MB, longest reset 0.25 s.
- A loop writing once into each of 500,000 tables made before it (20M iterations): 7.9 → 4.5 s, resets 3.5 → 0.54 s. A first version that compacted the log's whole window at every reset and charged nothing for it took 94 s.
- A receiver draining a 400,000-message backlog that arrived during its loop: resets 1.2 → 1.0 s (8.1 s with the walk uncharged); an 800,000-message backlog older than the loop: resets 33.5 → 1.0 s, wall 43.7 → 11.2 s.
- A 50,000-record list rebuilt at every iteration of a loop: peak RSS 152 → 264 MB (each replaced list stays kept until a full reset), time unchanged (13.3 → 14.0 s).

### Zero only the arena memory a reset hands out again ✓ Done (2026-10-04)
`gem_arena_alloc` cleared every allocation with `memset`, although a block comes from `mmap` already zeroed: in a `sample` profile of `examples/logstat` the `memset` was about 9% of the main thread. A block now records the highest `used` a region reset rewound it from (`dirty` in `GemArenaBlock`), and an allocation clears only what lies below it. logstat, 1M lines on stdin, `--by ip` (macOS arm64): 5.41 s → 4.93 s, same report.

### Region resets: sound in any call context, with hysteresis ✓ Done (2026-10-02)

Replaced the whole-arena reset (sound only for loops reachable from a process root through tail calls, guarded elsewhere by a runtime "depth-2 fence") with region resets. Each loop (`while`, TCO fn, mutual-TCO trampoline) takes a `GemArenaMark` at entry; its back-edge reset copies what is reachable from the memory allocated since the mark and unmaps the rest. Older memory is never moved, so callers' frames stay valid; older tables written since the mark are found through a write barrier + remembered log and fixed up in place, old buffers / pinned boxes / module slots / mailbox likewise. The process-tail analysis, the depth fence, the pcall skip and the "TCO function not reachable from a process root" warnings are gone; zero-arg tail calls reset too. Next reset waits for max(1 MB, 2 × copied + scanned) bytes.

Supersedes "Tighten TCO function not reachable from process root warning (structural decrease)" and "Indirect-spawn PT tagging Stage B" (nothing left to warn about or tag).

Before → after (Linux x86-64, same machine):
- top-level loop pushing 5000 rows once live data > 1 MB: 18.3 s → 12 ms; 1.5 MB `buf_push` at top level: 67.7 s → 14 ms; top-level 5000 rows of 3 fields (review top1): 12.2 s → 9 ms.
- gen_server-style store with 5000-entry state, 1000 calls: 6.35 s → 14 ms.
- `while true` server loop in a spawned process called from a non-tail position (and, before, *any* caller once a non-tail call site existed): ~1 GB/s growth (4.9 GB after 5 s) → 7.9 MB flat; tail-called variant unchanged at 7.9 MB.
- zero-arg receive loop, 400k messages: 160 MB → 7 MB peak.
- 50/100/200 queued 1 MB messages draining through a reset loop: quadratic (0.75/3.2/14.4 s) → linear (200 msgs: 0.13 s).
- self-hosting compile of `compiler/main.gem`: 3.1 s / 1.36 GB peak → 3.8 s / 117 MB peak (1,300 resets, 0.84 s of reset work).
- spawn of 100k short processes with `std/http`, `std/json`, `std/log` loaded: 18 µs → 22 µs per spawn (module state copy; fixed by "Lazy per-slot module copy at spawn" below).

### Lazy per-slot module copy at spawn ✓ Done (2026-10-02)
Module-level bindings are per-process, and `spawn` used to deep-copy every slot of the parent into the child, used or not. A 10k- or 100k-entry top-level table cost every spawn its full size (and `std/http` spawns per connection). Now a spawn copies only light slot values (numbers, short strings, fns without env); every other slot value lives in a **snapshot unit**: an immutable, refcounted malloc copy of the parent's slots that share structure (one aliasing component, found by walking the values). The child's slot holds `VAL_LAZY` plus a reference to the unit; its first read (`gem_global_get`, an inline tag test) copies all of the unit's still-lazy slots into the child's arena with one copy map, so aliasing between bindings survives, and shares the unit's strings. The parent reuses a unit for later spawns until it changes it: a slot write (`gem_global_set`/`gem_global_ref`) or a change to a table copied into it (tables carry the unit's `snap_gen` stamp; every table mutator calls `gem_table_check_mutable`, which already refused frozen namespaces) drops just that unit, and the next spawn rebuilds just that unit. Units holding buffers or pinned capture boxes (which change without a table write) are used by one spawn only. A child that never touched a slot hands the same unit on to its own children. A spawned closure's env is copied with the same map as the units it reaches, so a captured local referring to a module table is still that table in the child. Runtime in `runtime/gem_copy.c` ("Module globals"), codegen emits `gem_global_get`/`gem_global_set`/`gem_global_ref` and `gem_globals_init_lazy`.

Before → after (Linux x86-64, same machine, median of 3):
- 200 live children, parent holding a 10k-entry top-level array of strings, children never reading it: 317 ms / 199 MB peak → 7 ms / 12.7 MB.
- same with 100k entries: 4.1 s / 1.57 GB → 26 ms / 36 MB.
- same with 100k entries, every child reading the array once (`len(big)`): 4.1 s / 1.57 GB → 0.83 s / 41 MB (strings shared with the unit, one unit for all children).
- spawn + reply of 100k short processes with `std/http`, `std/json`, `std/log` loaded: 29 µs → 24 µs per spawn, the same as with no modules loaded (24–25 µs).
- tight loop reading a module binding (50M iterations): 1.94 s → 1.92 s; `fib(32)`: 270 ms → 275 ms (identical code, noise).
- self-hosting compile of `compiler/main.gem` (`--emit-c`): 4.13 s → 4.29 s mean of 8 (noise-level; 0.26% more instructions under callgrind for the same compiler source, 0.4% including the namespace pass); peak RSS 118 MB → 119 MB.

### Lower default `GEM_ARENA_RESET_THRESHOLD` ✓ Done (2026-04-30)
Default lowered from 16 MB to 1 MB. Threshold sweep at c100 on `/` showed 1 MB strictly dominates: same throughput (28.8k vs 28.9k req/s), p99 −3.6× (26.5ms → 7.3ms), peak RSS −14× (2.04 GB → 139 MB), idle RSS −10× (495 MB → 50 MB). `/bookmarks` validation at c50 (heavier per-request allocation) confirmed no regression: throughput unchanged (4041 vs 4007 req/s), p99 −15% (26.9ms → 23.0ms), peak RSS −14× (728 MB → 52 MB). The hypothesis "smaller threshold = more reset overhead" did not show up in numbers — live set after a request is tiny so reset cost is negligible.

### Arena compaction for long-running processes ✓ Done via a different mechanism (2026-04-30 → 2026-05-01)
Original sketch: a runtime "periodic compaction" pass that walks roots from the coroutine stack and closure envs, copies live data to a fresh arena, frees old blocks. **That approach is unsound for Gem** — there is no precise GC root map, no stackmap on the C compiler's output, no way to safely walk a coroutine's spilled register state and identify which slots hold `GemVal` vs raw bytes. Adding any of that would be a major undertaking (full GC infrastructure on a language that explicitly chose arenas to avoid GC).

What actually ships gives the same end-state — old blocks freed, live data preserved — driven by **compile-time liveness** rather than runtime scanning:

- At every process-tail loop back-edge, codegen emits `gem_arena_reset_with_roots_pinned(roots, n, pinned, m)` guarded by a 1 MB threshold. The roots are the liveness pass's output filtered by `filter_live_for_rescue`; the runtime deep-copies live values into a fresh arena and `munmap`s the old blocks.
- Mutated-captured fn-local boxes are pinned outside the arena (item 5b in the work log) and migrated by the same walk; the pin-set's mark-and-sweep frees boxes orphaned by short-lived calls.
- Top-level boxed lets sit in BSS (item 7), so they survive resets directly.

Effective compaction frequency: every ~1 MB of arena-allocated bytes. Bookmark soak benchmark validates: peak RSS stays bounded across 5-minute runs (vs. unbounded growth pre-mechanism). No runtime root scan, no stackmap, no GC pause.

### Growable structure waste ✓ Done via the same mechanism
Original concern: `gem_table_grow` doubles capacity in place, leaving the old keys/vals backing as dead space until process exit. The back-edge reset above reclaims this — when the arena resets, the table is deep-copied with `cap = current_cap` (no historical doubling overhead), and the old arena blocks (containing every prior backing array) are `munmap`'d. So a long-running gen_server's route table that grew 1→2→4→…→1024 entries pays the dead-backing cost only until the next reset, not for the lifetime of the process.

### Deep copy optimization for shared immutable data ✓ Done
Module tables are marked immutable (`gem_table_freeze`) by the compiler after construction. `gem_deep_copy_internal` skips copying immutable tables, sharing them read-only across all processes. Strings from the main process arena are also shared (all strings are immutable; the main arena is never freed). User-created mutable tables are still deep-copied correctly. Bookmark app soak (60s, 4c, GET /bookmarks): p99 1.90ms → 1.45ms (−24%), throughput 4,816 → 4,986 req/s (+3.5%), RSS 1,511 → 1,236 MB (−18%).

### Reuse tcp_read buffer per process ✓ Done
`GemProcess` now has a `read_buf`/`read_buf_cap` pair. `tcp_read` allocates the read buffer once per process and reuses it across calls, copying only the actual bytes read into an exact-size string for the return value.

### String builder for response construction ✓ Done
`build_string` builtin added — creates a buffer, passes an `add` closure to the block, returns the finalized string. `push` and `to_string` now work on buffers. `std/http.gem` response serialization and `examples/bookmark_app/app.gem` HTML templates rewritten to use `build_string` blocks instead of `+` concatenation.

### Eliminate GC pauses ✓ Done
Replaced Boehm GC with per-process arena allocation. No global stop-the-world pauses. Each process allocates from its own bump allocator; memory is freed in bulk when the process exits.

## Codegen Output

### O(n²) string building in codegen ✓ Done
All heavy string accumulators in `compiler/codegen.gem` rewritten to use `buf_new`/`buf_push`/`buf_str`. Eliminated ~188 O(n²) concatenations.

### `x = x + y` auto-optimization ✓ Done
The compiler detects the self-append pattern `assign(name, binary("+", var(name), expr))` inside `while`/`for` loop bodies and emits `gem_string_append_to`/`gem_string_finish` instead of `gem_add`. Eligible variables must only appear in append patterns within the loop body (no non-append reads), and a module-level variable is eligible only in top-level code whose loop calls no user fn and none of `pcall`, `sort`, `build_string` or the `spawn` family (anything else could read its slot mid-loop). A per-loop C flag records that the variable holds the buffer the loop built; only then are appends in place and the variable turned back into a string after the loop, so a buffer the program passed in gets plain `+`. Handles chained concatenation (`x = x + a + b + c`), conditional appends inside `if`/`match`, and nested loops. Runtime fallback for non-string types (integers, floats) ensures correctness without static type information.

### Redundant `gem_push_frame` / `gem_pop_frame` for leaf functions ✓ Done (2026-05-04)
A leaf function — one whose body contains no `call` AST node and no `receive_match` — emits its body without `gem_push_frame`/`gem_pop_frame` and without `gem_set_line`. Detection (`body_is_leaf` in `compiler/codegen.gem`) is conservative: every builtin invocation parses as a `call` (since `print`, `len`, `error`, … all go through `compile_call`'s direct-call path), so any use of one disqualifies the fn. Predicates / accessors / pure arithmetic / control-flow / table-and-array literal builders qualify; bookkeeping helpers do not.

State plumbing: a global `in_leaf_fn` is set in `compile_fn` and `compile_closure_fn` and saved/restored alongside the existing `boxed_vars`/`in_top_level`/`local_names`/`fn_scope_locals` quartet. Two helpers (`pop_str` and `setline_str`) replace every `gem_pop_frame()` / `gem_set_line(...)` literal in the codegen templates; both return empty strings when `in_leaf_fn` is set. The `gem_push_frame` calls at fn entry are gated directly. Top-level `main` is never marked leaf so the outermost frame is always present.

Stack-trace impact: originally the leaf was invisible in traces (errors were attributed to the caller's call-site line, and a spawned leaf closure printed no location at all). Since 2026-10 a leaf records itself in a per-process *leaf slot* instead of a frame (gem.h `GemLeafSite`): `gem_leaf_site = &static_site` on entry, `gem_leaf_line = N` per statement, `gem_leaf_site = NULL` on return; reports and pcall's `stack` show it as the innermost frame, and the scheduler saves/restores the slot on every resume (a leaf's loop can yield). Cost, measured by instruction count (cachegrind) on a 1M-iteration loop calling `fn inc(x) x + 1 end`: 247 → 250 instructions per iteration (+3, +1.2%); a full frame push/pop with per-statement `gem_set_line` would have cost +24 (+10%). Wall time differences were within noise.

Coverage on the bootstrapped compiler: 50/121 named fns and 12/112 anonymous closures are leaf-elided. Microbench (5M iter tight loop calling `add(a, b)` and a 4-arm `classify(x)`): 140ms pre → 127ms post (−9.3%, three-run best). Bookmark HTTP `/` burst is within run-to-run noise (~1% delta either direction). The big bench savings would require many leaf calls per request; HTTP handlers mostly aren't leaf.

Regression: `examples/86_leaf_fn_elision.gem` covers a few leaf shapes (predicate, multi-arm if, table accessor) plus a non-leaf caller invoking leaf helpers in a loop.

### Escape analysis for non-escaping closures ✓ Done (2026-04-30)
A pre-codegen pass marks `anon_fn` nodes that appear as the immediate argument of an allowlisted callee (`pcall`, `sort`) as `non_escaping`, provided the closure body contains no nested `anon_fn`s. Marked closures (a) skip contributing their captures to the enclosing function's `captured` set, so the enclosing params/locals stay unboxed, and (b) get a stack-allocated env in `compile_anon_fn` instead of `GC_MALLOC` from the arena. `spawn`/`spawn_link`/`spawn_monitor`/`send_after` deliberately stay off the allowlist — their closures run asynchronously in another coroutine and must keep the boxing-via-arena path. The conservative "no nested anon_fns" precondition sidesteps the case where an escaping inner closure could smuggle outer captures past the synchronous call boundary.

Concrete payoff: `safe_handle_request` deleted from `std/http.gem` and inlined into `handle_connection_loop`, which now has unboxed params and a stack-allocated pcall env. The `accept_loop` `let fd = client_fd` aliasing is **kept** — it dodges the *spawn* closure capture, which is still escaping.

Scope deferred: block-syntax `each`/`map`/`filter`/`reduce` from std/table aren't allowlisted yet (their callee parses as `dot`, not `var`, and they're regular Gem fns rather than builtins where we can audit storage behavior). Same goes for `build_string`. Add to allowlist when a workload demands it.

### Static process-tail analysis ✓ Done
Replaced the runtime depth-2 fence with a compile-time `mark_process_tail` pass (`compiler/codegen.gem`). A pre-codegen pass builds a call graph of `(caller_id, callee, is_tail_position)` records, seeds it with top-level statements and `anon_fn` bodies passed to `spawn`/`spawn_link`/`spawn_monitor`, then runs a greatest-fixed-point demotion: start with all candidates, demote any candidate whose incoming edge from a non-candidate non-seed caller isn't tail-position, repeat until stable. Final pass: BFS from seeds through candidates via tail edges to compute the actual `process_tail` set. Codegen at the TCO back-edge (line 827) drops the `gem_call_depth - entry_call_depth <= 2` check for `process_tail` functions; non-process-tail TCO functions get a compile-time warning and keep the depth check as a safety net.

**Why GFP demotion instead of BFS**: naive BFS-from-seeds (the original sketch) over-approximates — it would mark functions reachable via tail calls even if they're also called non-tail elsewhere, which is unsound. GFP demotion correctly handles self-recursion (sup_loop, gen_server `loop`) and mutual tail-call cycles. A "trivial return" rule (`f(); return nil` or `f(); return <const>`) treats early-exit patterns as tail-effective so supervisor / gen_server idioms get marked.

**Result**: zero TCO warnings on all 49 examples + bookmark_app + std modules; bootstrap roundtrip clean; bench parity on `/bookmarks` c=50 t=4 30s (median 3157 vs 3104 req/s, RSS equal). The depth-2 fence is no longer the mechanism; it's a belt-and-suspenders safety for the rare functions the analysis can't prove.

**Follow-up**: better warning cause-attribution. Today the warning fires on the demoted TCO function with a generic "reachable from non-tail context" message. Naming the offending non-tail call-site (file:line of the caller) would make the diagnostic actionable. Low priority — no demotions exist in the current codebase.

### Mutual TCO via tail-edge SCC trampoline ✓ Done (2026-05-10)
Direct self-recursive tail calls were already collapsed into `while(1) { ... continue; }` by `is_self_tail_call`. Mutual cycles — `a → tail b → tail a`, possibly through 3+ functions — were not, so the broker's `writer_loop ↔ handle_frame ↔ handle_<command>` 7-fn cycle hit a deterministic 175-frame ceiling per connection (see `examples/stomp_broker/NOTES.md` "Milestone 6: lived experience"). Compiler now finds these cycles statically and emits a tiny runtime trampoline.

**Mechanism**:
1. **SCC detection** (`find_tail_call_sccs` in `compiler/tco.gem`). Reuses `mark_process_tail`'s already-built tail-edge graph (`caller_records[fn:NAME].tail_callees`), restricts to fn_def→fn_def edges, runs Tarjan's SCC. Components of size ≥ 2 are candidates; size-1 stays under the existing direct-self path.
2. **Viability filter**. Per SCC, every member must have: no `rest_param`, no defaults, no boxed (mutated-captured) params, no name-shadow with its own body, and ≤ `GEM_MAX_TAIL_ARGS` (16) parameters. Conservative — bails the whole SCC silently if any member disqualifies. The broker case clears all conditions.
3. **Body + wrapper emission** (`compile_fn`, `scc_wrapper_for`). For each viable SCC member, the body emits as `gem_fn_<name>_body` with the original direct-self TCO `while(1)` intact. The public `gem_fn_<name>` is replaced with a thin trampoline loop: `while (1) { _gem_tail_fn = NULL; _r = _next(env, args, argc); if (!_gem_tail_fn) break; _next = _gem_tail_fn; ... arena reset ... gem_yield_check(); }`.
4. **Trampoline marker** (`emit_scc_tail_call`). At an intra-SCC tail call site, instead of a real C call, the body writes `gem_tail_fn`, `gem_tail_env`, `gem_tail_args[]`, `gem_tail_argc` (single global TLB in `runtime/gem_error.c`), pops its own frame, and returns `GEM_NIL`. The wrapper sees the non-NULL marker and dispatches the next body. Direct self-tail and out-of-SCC tail calls take the existing paths.
5. **Single-global TLB safety**. The scheduler is cooperative; yields only happen inside body code. The TLB is set at the very last statement before return — there is no yield between TLB-set and return, and no yield between wrapper-read and dispatch. So a single global is sound; no per-process or per-coroutine TLB needed.
6. **Per-iteration arena reset**. The wrapper's between-iteration reset mirrors `emit_tco_continue`'s gating: unconditional for SCCs whose members are all `process_tail` (every member of an all-tail cycle reachable from a seed is auto-marked PT by `mark_process_tail`); depth-2 fenced otherwise. Roots are `&gem_tail_args[0..argc]`.

**Stack-trace fidelity**: the body — not the wrapper — pushes/pops its own frame. Across an intra-SCC trampoline transition, the previous body's frame pops before the next body's frame pushes, so the active top-of-stack frame always names the currently-executing member (unlike the merged-function-with-goto alternative I considered first, which would have shown the entry function for the entire cycle).

**Result**: broker writer cycle iterates at constant stack depth. Sweep cells that previously died at delivered ≈ N×175 (`100×200 → 17,499 dead`, `50×500 → 8,749 dead`, `200×100 → dead`) now complete at the expected `100% delivered`. Cells that hit a *different* cliff (memory or scheduler-shape: `500×100 → still 26,999/50,000 dead`, `10×1000 → 5,439/10,000 dead`) survive 3× longer but still fall over — those are the next-tier bottleneck (mailbox unboundedness or process-table pressure), confirmed not the recursion ceiling. Full numbers in `examples/stomp_broker/NOTES.md` "Milestone 6, second pass". Bootstrap roundtrip clean on first pass; all 124 examples + LSP smokes green.

**Bail-out telemetry**: silent today (the SCC simply isn't merged). Adding a "note: SCC `[a, b, c]` not merged because <reason>" diagnostic would help future users refactor toward mergeable shape. Deferred until a real SCC fails viability — the broker doesn't, and synthetic cases haven't surfaced.

### Eliminate user-visible recursion-as-iteration ✓ Done (2026-04-30)
A `while true` loop in a process-tail context now resets the per-process arena at its back-edge using the same machinery as TCO. Recursion-as-iteration is no longer required for long-running processes.

**Mechanism shipped**:
1. **Liveness pass** (`compiler/liveness.gem`, ~770 LOC). Backward dataflow over `while true` bodies; returns `{ok: true, live}` or `{ok: false, reason}`. Closure captures over-approximated (every anon_fn's free vars unconditionally live across the back-edge). Refuses on `break` at this loop's level. Validation gate (`compiler/test_liveness.gem`) walks every TCO function in `std/` + `examples/`, synthesizes `while true { fn.body }`, and asserts `live ⊇ params` so no existing TCO loop would under-rescue if rewritten as `while true`.
2. **Process-tail tagging** (`tag_process_tail_while_loops` in `compiler/loops.gem`). After `mark_process_tail` finishes, a tree walk tags every `while true` node inside a process-tail context (a fn in `process_tail_fns`, the top-level program, or a spawn-arg anon_fn body) with `process_tail = true`. The walker stops at anon_fn / fn_def boundaries except when descending into spawn-arg anon bodies (treated as fresh process roots).
3. **Codegen** (`compile_while`). When `node.process_tail == true` and `node.cond` is `bool true`, the back-edge emits `gem_arena_reset_with_roots(_loop_roots, N);` guarded by `gem_current_pid >= 0 && gem_current_arena()->bytes_allocated > GEM_ARENA_RESET_THRESHOLD`. The roots are the live set filtered against `local_names` ∖ `boxed_vars` ∖ globals/fn-names/builtins.
4. **Narrowed warning**. The blanket *"use tail recursion instead"* warning is replaced by a refusal-only warning naming the specific blocker (closure-captured live var, declaration in nested if/match arm, `break` in body). Process-tail `while true` loops with a clean liveness result emit no warning.

**Refusal cases**: empty as of 2026-05-01. The four classes that originally refused (closure-captured live var, declaration in nested if/match arm, `break` in body, `pcall(fn() spawn(...) end)`-style escape) are all resolved (items 2, 3, 4(b), 5b in the work log below). Codebase scan finds zero `cannot reset` warnings. The one residual advisory is a TCO-reachability hint — `compiler/main.gem:109 rename_node` is a self-recursive helper called from a non-tail context, so its TCO loop won't reset at the back-edge. Different mechanism, different fix; not a refusal of the rescue path.

**Closure-capture over-approximation (tightened 2026-04-30)**: liveness recognizes closures appearing as arg[0] of `spawn` / `spawn_link` / `spawn_monitor` and excludes their captures from the back-edge live set. Sound because the runtime deep-copies a spawned closure's captures into the child process at the call site (see `runtime/gem_scheduler.c`) — the parent's copies are dead after the call returns. Captures are still live AT the call site (via `_uses_expr` on the anon_fn), so any prior `let captured = ...` in the same iteration is correctly killed before the back-edge.
  - Still over-approximated: closures escaping via assignment to outer-scope vars, returns, or non-spawn calls. These keep the closure value alive in caller memory or another process's mailbox, so captures must remain live across the back-edge. Item 5b made this benign for the common case (mutated captures are now pin-rescued regardless of how the closure escapes), so the over-approximation no longer translates to a refusal.

**Result**: `examples/55_while_true_process_loop.gem` regression test runs 10000 iters × ~200 bytes/iter (≫1MB threshold) and completes with stable RSS. All 49 other examples + json_parser + bookmarks pass; bootstrap roundtrip clean; liveness gate (4 fns from `std/`) PASS.

#### `while true` adoption — work log

All items shipped. The mechanism is now sound for every refusal class observed in real code; users no longer need to think about the rescue path. Each entry below is kept for historical context (commit refs, design rationale, regression tests). Listed in shipping order — each unblocks more user code without requiring users to think about the rescue mechanism (per `CLAUDE.md`: language must not leak runtime invariants onto users).

1. **Dogfood: rewrite `std/http` accept/handle loops as `while true`** ✅ done (2026-04-30). Both `accept_loop` and `handle_connection_loop` now use `while true`. Compiles with no warnings; bench at parity with `2026-04-30_head_quiet/` (see `2026-04-30_std_http_while_true/` log entry above). Confirms the back-edge codegen is shape-equivalent on a real workload.

2. **Hoist lets declared in nested `if`/`match` arms** ✅ done (`89058b3`, 2026-04-30). AST lift pre-pass in `compiler/codegen.gem` (option (b) — option (a) wouldn't have fixed the underlying C-scoping issue). For each `if`/`match`/`receive_match`, intersects top-level `let` names across arms, hoists `let n = nil` before the construct, rewrites the per-arm lets to assigns. Skips names already bound in the enclosing block to avoid shadow conflicts. Surfaced and fixed a latent capture-emission bug in `compile_closure_fn` (top-level boxed var captured into a closure created in non-top-level scope: env field stores the existing pointer, not its address). Negative test `arm-merged-lift-shape` and regression example `examples/56_arm_merged_let_process_loop.gem` added; gate at 7/0 negatives. *Later removed (2026-10): with strict block scoping a `let` in every arm is not visible after the construct, so the pass had nothing left to do; example 56 now declares `next` before the `if`.*

3. **Conditional-exit process-tail loops** ✅ done (`d65565d` substrate + `81a75d6` gates, 2026-04-30). Post-loop liveness substrate landed first (`compute_loop_live_at_backedge(while_node, live_after_loop)` parameterised worker; `compute_live_for_pt_loops_in_stmts` walks fn bodies backward and attaches `liveness_result` to every PT-tagged while). Then dropped the `cond_is_true` gates in `walk_for_tagging_node` and `compile_while`, so any `while cond` (graceful shutdown via assignment) and `while true` with `break` get the same back-edge rescue as a literal `while true`. Refusal warning copy no longer suggests "tail-recursive helper". Regression examples 57 (`while running` with `running = false`) and 58 (`while true` with `break`) at 5000 iters each. Bench vs `2026-04-30_head_quiet/`: throughput within ~5% on read paths, RSS growth improved (37.6MB → 29.1MB); see `2026-04-30_post_loop_liveness/`. **Deviation from the original plan**: top-level statements are *not* PT-tagged — see item 7 below for the runtime invariant that forced this.

4. **Mutation-aware boxing** ✅ done (2026-04-30). New `walk_writes` analysis pass classifies captures by whether they are reassigned anywhere in their lexical scope; only mutated captures are heap-boxed at the outer level. Read-only captures stay as plain `GemVal` locals at the outer fn (so the rescue's "live var captured by a closure" refusal no longer fires for them). Closure env layout stays uniform `GemVal *` (the runtime's `gem_deep_copy_fn` walks fields as pointers); for escaping closures whose outer is unboxed, `compile_anon_fn` allocates a fresh single-cell box per closure-creation site to preserve that uniformity. Counted writes correctly include outer-scope rebinds (e.g. `let fib = nil; fib = fn(...)`) so Lua-style rebind-after-capture semantics are preserved — closure observes the post-assign value via the shared box. Regression `examples/59_read_only_capture_loop.gem`; all 49 examples + stress 55–58 green; liveness gate 10/0 negatives, 2/0 positives; bootstrap round-trips. **Surfaced** a pre-existing rescue-codegen bug previously masked by refusals — see *Known follow-ups from item 4* below.

5. **Pin mutated-captured fn-local boxes outside the arena** ✅ done (2026-05-01). Was: *mutate-via-block in a process-tail loop* — `let total = 0; arr.each(fn(x) total = total + x end)` — refused because `total`'s box lives in the arena and dangles on reset. Approach is *not* the originally-sketched relocation walker (which would have needed to fix up every dangling pointer on reset); instead, pin the boxes outside the arena and let the existing deep-copy machinery migrate their *contents* on reset:

    - `gem_box_alloc()` (replaces `GC_MALLOC(sizeof(GemVal))` for fn-local boxed lets/params/rest at five codegen sites): `malloc`s a `GemVal` and registers it in the current process's `pinned_boxes` set (stb_ds hash map, value = mark bit).

    - `gem_arena_reset_with_roots_pinned(roots, n_roots, pinned_roots, n_pinned)` (new variant of the runtime reset entry point): codegen passes fn-local pinned boxes that are live at the back-edge in a *separate* `pinned_roots[]` array (not the regular roots, which would double-walk the contents). The runtime walks each pinned root once via `gem_deep_copy_internal`, marking the pin-set entry "walked".

    - `gem_deep_copy_fn`'s external branch (which already short-circuited BSS-backed top-level boxes via `preserve_external`) now *also* checks the pin-set: if the env field points at a pinned box that has not yet been walked this cycle, recurse into its contents; otherwise just preserve the pointer. This handles boxes reached only via live capturing closures, with no double-walk regardless of how many envs reach the same box.

    - `gem_pin_sweep` runs at the end of every reset: any pin-set entry not marked is unreachable — `free` it. Surviving entries reset to "untouched" for the next cycle. `gem_pin_free_all` runs on process exit.

    - `gem_copy_is_external` switched from a bounding-box compare on `[arena.lo, arena.hi)` to a block-list walk of `arena.head`. The old check was empirically OK for BSS but unsound for malloc'd pointers, which can land between arena blocks (the lo/hi span is *not* a contiguous range — blocks are individual mmap regions).

    Soundness bar: every pinned box is walked at most once per reset (regardless of how many roots / env paths reach it), and the **live-local-without-live-closure** case is handled — the prompt's original sketch ("just rely on env-walk") missed this and would have left dangling `gem_v_<name>` pointers when a fn-local box was live via the function's local but no capturing closure was alive at the back-edge.

    Regressions: `examples/64_pinned_box_pt_loop.gem` (10k-iter spawned PT-while with mutate-via-block accumulator holding a small table — exercises content migration across many resets) and `examples/65_pinned_box_many_calls.gem` (50k calls to a fn that allocates a pinned box and lets the closure go out of scope — RSS bounded at ~19 MB across 50k *and* 500k iterations, validating the sweep).

    Stress tests added (2026-05-01): `examples/66_pinned_box_escaped_closure.gem` (the closure capturing `counter` is sent into another process via spawn; the parent then runs a 5000-iter PT loop where `counter` is locally live but no in-process capturing closure references it — exercises the `_pinned_loop_roots[]` pre-pass, since the box would otherwise be unreachable via env-walk alone), `examples/67_pinned_box_cross_process.gem` (top-level `count` mutated by `bump` is captured into a `spawn(fn() … end)` body. Deep-copy at spawn produces a fresh receiver-side arena box; both env fields — `count` direct and `count` inside `bump`'s env — alias to the same box via the copy_map. Confirms cross-process closure transfer with mutated captures is sound: the receiver never observes the sender's pin-set, since deep_copy at spawn always lands the box in the receiver's arena), `examples/68_pinned_box_nested_captures.gem` (three levels of nested closures, each mutating its own pinned box; validates env-walk recursion + mark-dedup across `outer→middle→inner` chains), `examples/69_pinned_box_transitive_refs.gem` (one pinned box held in a regular root whose value transitively references another pinned box via a fn env — validates dedup between pinned-roots pre-pass and env-walk reaching the same box transitively), `examples/70_pcall_spawn_mutating.gem` (`pcall(fn() spawn(fn() … end) end)` with mutated captures — old item 6 pattern, compiles cleanly with no refusal warning and runs to 5000), `examples/71_pinned_box_many_procs.gem` (200 simultaneous processes, each with its own pinned-box pattern; validates pin-set isolation and `gem_pin_free_all` on death — sum across processes matches expected 19900), and `examples/72_pinned_box_huge_pinset.gem` (~30k pin-set entries between sweeps × ~12 resets, total ~400k pinned-box allocs in <100 ms — validates stb_ds map scaling and sweep cost is not quadratic). All 61 examples + bootstrap roundtrip + json suite green.

    Silent perf cliff surfaced while writing #67: indirect-spawn (`spawn(make_closure())` where the spawned fn is the *return value* of a call rather than a literal `anon_fn`) doesn't trigger `walk_for_tagging_node`'s spawn-arg case — so the closure's top-level `while true` is never PT-tagged and the receiver's arena grows without reset. Tracked as item 8 below.

6. **Tighten pcall-wrapped spawn closures** ✅ subsumed by item 5b (2026-05-01). Was: `pcall(fn() spawn(fn() ... end) end)` — the outer pcall closure escapes captures synchronously; the liveness over-approximation kept them live across the back-edge → refusal. With item 5b's pin-set rescue, mutated captures survive reset regardless of how the closure escapes; read-only captures are already unboxed by item 4. The originally-planned codegen tightening (recognising `pcall` as a non-escape callee in liveness) is no longer needed to lift the refusal — it remains a possible future micro-optimisation if profiling ever shows the unnecessary live-across-backedge tracking is costly.

7. **Lift the main-arena reset invariant** ✅ done (2026-05-01). Top-level `while` loops are now PT-tagged (`walk_for_tagging_stmts(top_stmts, true)` in `compiler/loops.gem`); `tag_process_tail_while_loops` no longer scopes top-level out. The runtime invariants that previously blocked this:

    - **Cross-process string sharing fast-path** (`gem_in_main_arena`): deleted from `gem_arena.c` / `gem.h`. Replaced by a more general external-pointer mechanism in `gem_deep_copy_internal` — when `preserve_external` is set on the copy map, any pointer outside `[old_arena_lo, old_arena_hi)` is left in place. Used both for cross-process value sharing (BSS-backed boxes, global arena strings) and for the top-level rescue.

    - **Top-level boxed lets**: previously allocated their boxes via `GC_MALLOC` in main's arena, so the box pointer became dangling on reset. Now declared as `GemVal gem_box_<name>; GemVal *gem_v_<name> = &gem_box_<name>;` (BSS-backed). Closure envs that captured those boxes survive resets unchanged.

    - **Frozen module tables** (e.g. `string` from `load "std/string"`): `gem_table_freeze` marks them immutable, and `gem_deep_copy_internal` previously short-circuited immutable tables with `if (!map->use_malloc && val.table->immutable) return val;`. Safe for spawn/send (source arena keeps living), unsafe for arena reset (source arena is about to be munmapped). Fix: only apply the immutable shortcut when `preserve_external` is *not* set.

    Top-level rescue rescues *all* top-level vars (the rescue list is populated from `top_level_vars` regardless of liveness, since identifying which top-level vars are live across a top-level back-edge is harder than just rescuing them all). `filter_live_for_rescue` permits top-level boxed names because they're BSS-backed.

    Regression: `examples/61_top_level_pt_loop.gem` (5000 iters of `string.split` at top level — exercises the frozen-module-table path and triggers ~4 arena resets at the 1 MB threshold). Bench vs `2026-04-30_post_loop_liveness/`: throughput at parity (+1–2% across read paths and POST burst), RSS peak 57.8 MB → 50.4 MB (−13%) and growth 29.2 → 18.9 MB (−35%). Logs: `benchmarks/logs/2026-05-01_main_arena_lifted/`.

    The compile-time refusal class for top-level boxed lets is gone, and the silent perf cliff for top-level `while` loops is closed. The non-top-level mutate-via-block residual was subsequently closed by item 5b (pinned boxes outside the arena, see above).

    **Follow-up landed (2026-05-01)**: top-level for-loop iter vars (`i`, `_for_i_N`, `_for_len_N`) were spuriously refused as Class B "no C local at back-edge" because `local_names` at top level was populated only from `top_level_vars` (flat) — `collect_top_let_names` does not descend into while-bodies, so iter vars introduced by for-desugaring were missing. Top-level main now mirrors `compile_fn`: populates `local_names` via `collect_shadow_lets_in_fn(top_stmts, local_names)` (over-approximation) and tracks `fn_scope_locals` dynamically (`set_add` in compile_stmt's let arm, save/restore at C-`{}` introducers — `compile_while` body, `compile_if`/`match`/`receive_match` arms, `compile_stmt_return` block). 8 false positives gone (7 for-loop iter vars at the top level, plus `compiler/main.gem:492` and `examples/59_read_only_capture_loop.gem:22`). The remaining acceptor arm-let pattern (`examples/http_server/server.gem`) was subsequently closed by item 4(b) (`cfb1541`) — fn-top placeholders for arm-lets live across an enclosing PT-loop's back-edge.

8. **Indirect-spawn PT tagging** (Stage A done 2026-05-01; Stage B still open — see `OPTIMIZATIONS.md`). When the spawned fn is the *return value* of a call rather than a literal `anon_fn` — `spawn(make_worker_closure(arg))` — `walk_for_tagging_node`'s spawn-arg case doesn't fire, because it pattern-matches `is_node(node.args[0], "anon_fn")`. The closure's body never gets PT-tagged, so its top-level `while true` runs without arena rescue and the receiver's RSS grows unbounded until process exit. This is a **silent perf cliff** of the kind the design philosophy explicitly disallows (CLAUDE.md: *"Silent perf cliffs are not"*).

    **Stage A — compile-time warning** ✅ done (2026-05-01). `walk_for_tagging_node` now emits a warning at every `spawn`/`spawn_link`/`spawn_monitor` call site whose `args[0]` is not a literal `anon_fn`. Threaded `source_name` through the walker (`tag_process_tail_while_loops` → `walk_for_tagging_stmts` → `walk_for_tagging_node`). Same DX bar as the *cannot reset* warning: surfaces the hidden constraint without silent regression. Audit at landing time: zero existing call sites in `std/`, `examples/`, `compiler/` triggered the warning — every existing `spawn(...)` passes a literal `fn(...)` or a trailing `do` block (which the parser desugars to `make_anon_fn`). Regression: `examples/73_indirect_spawn_warning.gem`.

    Stage B (closure-escape analysis) is still TODO — see `OPTIMIZATIONS.md`.

The roadmap above assumes the existing soundness bar (under-rescue = silent memory corruption). Every change must keep the param-superset gate green and the negative-test gate green (currently 10/0), plus pin a deterministic stress test against the new pattern (cf. `examples/55_while_true_process_loop.gem` … `examples/58_break_process_loop.gem`).

##### Known follow-ups from item 4 — resolved

1. **Rescue codegen references arm-let names not at fn scope** ✅ done via fix (a) (2026-05-01); fix (b) (the principled DX-friendly version — extend the arm-let hoist pre-pass to cover lets live across an enclosing PT-loop's back-edge) shipped as item 4(b) (`cfb1541`). Threaded a new `fn_scope_locals` set through `compile_fn` / `compile_closure_fn` / top-level alongside `local_names`. `filter_live_for_rescue` refuses names that are in `local_names` but not in `fn_scope_locals` with the existing *"no C local at this loop's back-edge"* reason. Regression `examples/60_arm_let_in_pt_loop.gem`.

2. **`std/supervisor.gem:137` `sup_name` refusal** ✅ resolved by item 5b (`62816fd`). `sup_name` is mutated → boxed → captured by a spawn closure → live across the back-edge of a PT loop in the spawn body. With pin-set rescue, the box now survives reset and the refusal is gone.

### TCO reset with pinned roots ✓ Done (2026-05-04)

`emit_tco_continue` (`compiler/codegen.gem`) now passes mutated-captured params (the `tco_boxed ∩ tco_params` slice) as `_tco_pinned_roots[]` to `gem_arena_reset_with_roots_pinned`, so the post-reset sweep keeps the boxes alive across the tail call (the assignment `*gem_v_<p> = arg_temps[i]` immediately after the reset would otherwise touch freed memory). The conservative `any_boxed`-skip guard is gone — TCO functions with mutated captures now reset per-iteration like any other.

Body-let boxed locals don't need pinning: top-level body lets re-allocate their box each iteration via `let x = gem_box_alloc()` before any use, and inner-scope lets aren't visible at the tail-call point. If such a box is referenced via a closure passed in args, env-walk during deep-copy marks it and the sweep keeps it alive automatically.

Regression: `examples/84_tco_mutated_param.gem` (10000-iter TCO with a closure mutating a captured param). Stress at 200k iters holds RSS at 52 MB (would otherwise grow linearly with allocations).

### Constant folding ✓ Done (2026-05-04)
`compiler/fold.gem` runs as a pre-codegen pass on the resolved AST (after load resolution, before `make_codegen`). Folds binop / unop where every operand is a literal: int/float/mixed arithmetic, string concat, all six comparisons, and `not`. `and`/`or` short-circuit on a literal-truthy / literal-falsy left even if the right is non-literal — matches runtime branch semantics.

Soundness corners: skip int `/` and `%` when rhs is `0` so `pcall (1/x)` still errors at runtime when x is statically zero via a non-folded path; unary `-` on a float literal uses `e.value * -1.0` rather than `0.0 - e.value` to preserve IEEE signed zero (`-0.0` test in `examples/25_runtime_edge_cases.gem`); `==`/`!=` across incompatible literal types fold to `false`/`true` (matches `gem_eq`'s type-check), but ordering ops (`< > <= >=`) only fold same-type pairs so the runtime type-error is preserved. Float overflow / int wrap on multiplication: Gem's int64 arithmetic wraps the same as the C runtime, so folded values match.

Pass shape: `fold(node)` recursively folds children first, then `try_fold_binop`/`try_fold_unop` on the parent. Walks into all expression-bearing nodes (call args, table values, array elements, interp parts, fn defaults, control-flow conds and bodies). Regression: `examples/87_constant_folding.gem`. Bootstrap roundtrip clean.

### Self-recursive tail call optimization ✓ Done
Codegen detects self-recursive calls in tail position (last expression in function body, propagating through if/else, match, receive, and block branches) and emits a `while(1)` loop with parameter reassignment + `continue` instead of a recursive call. `gem_push_frame` runs once on entry; `gem_yield_check` runs each iteration for cooperative scheduling. Multi-param reassignment uses temps to avoid ordering issues. Boxed (closure-captured) params reassign through the pointer. Functions with rest/block params or shadowed names skip TCO. Covers ~95% of OTP patterns (gen_server loops, supervisor restarts, recursive receive handlers).

### Tighten "TCO function not reachable from process root" warning — option E stopgap ✓ Done (2026-05-09)

The diagnostic now splits in two based on whether an outer reset boundary exists above the demoted fn:

- `note: ...will not arena-reset at its own back-edge..., but an outer reset boundary above (process exit, PT loop, or PT TCO back-edge) caps allocation per outer iteration. Likely benign for bounded recursion...` — emitted when at least one transitive caller chain reaches a process root (top-level / spawn-anon → process exit) or a `process_tail_fns` member (its TCO/while back-edge resets per outer iteration).
- `warning: ...not reachable from any process root or process-tail context — per-process arena will not reset at its back-edge, and no outer reset boundary caps allocation...` — the original wording, now reserved for the genuinely dangerous case.

Mechanism: a backward BFS through the call graph (built from `caller_records` in `mark_process_tail`). The substrate had three blind spots — fixing them was the bulk of the diff:

1. Selective imports (`load "./mod" (fn_a)`) desugar to `let fn_a = _mod_mod_fn_a`. Call sites then look up the alias, not the prefixed fn_def, so the static call graph dropped those edges. Fix: scan `top_stmts` for the `let X = var(Y)` pattern and resolve callees through it.
2. Whole-module loads (`load "./mod"`) desugar to `let mod = {fn_a: _mod_mod_fn_a, …}` (frozen table). `mod.fn_a(...)` is a dot-call — also dropped. Fix: scan `top_stmts` for table literals over fn_def vars and resolve `mod.field` callees through the resulting two-level map.
3. Closure-mediated calls (`fn make_dispatcher() let dispatch = fn(msg) … completion.handle(...) … end end`) record edges into `anon:N` callers, which lose the connection to the lexical fn that built the closure. Fix: track `anon_parent[anon:N]` at AST-walk time and have the BFS hop through it before deciding seedness.

Soundness: the `note:` is still a heuristic, not a proof — bounded inner recursion under the outer reset is assumed but not verified. That's why `note:` not `warning:` — the user is being told "this is probably fine, but if it isn't here's what to look at" rather than "you have a leak." Same three in-tree fns (`rename_node`, `walk_node`, `walk_for_fields`) all downgrade cleanly; bootstrap roundtrip and `make test` are clean.

Structural-decrease termination check (option B) is still TODO — see `OPTIMIZATIONS.md` for the principled fix.

**Rejected approaches** (kept here so the rationale survives the next time someone considers them):

- **Option A** (suppress when reachable caller is PT, no termination check): explicitly rejected. The unsoundness gap — unbounded inner recursion under an outer reset would silently leak within one outer iteration — is real even though no in-tree case currently hits it, and "silently silences a real leak class" is the wrong direction. E gives the same false-positive relief without making the warning unsound; B does the work A skipped.
- **Options C & D** (worklist rewrite in user code; `@no_arena_reset` annotation): inconsistent with the design philosophy. Optimizations that introduce DX warts are "tolerable only as transient hacks with a structural fix on the roadmap" (`CLAUDE.md`). The structural fix lives in the compiler, not in user code or annotations.

## Table Access

### O(1) appends and misses on arrays ✓ Done (2026-10-04)
`t[len(t)] = v` searched every key before appending, so building an array by index, and the runtime's own `keys`, `values`, `list_dir`, `argv` and sqlite result rows (all built that way), were quadratic. A table now carries `is_array` (entry i has key i): on such a table an int key at or past the end is appended, and a lookup or `has_key` past the end is a miss, without a search. `push` uses the flag too; on a table that is not an array it is `t[len(t)] = v`, where it used to add a second entry with an existing key. `keys` + `values` of a 40,000-entry table: 2,110 ms → 2 ms; of 1M entries: 30 ms. Sparse int keys are still searched (OPTIMIZATIONS.md, "Avoid hashing integers in tables").

### Inline caching for `.field` access ✓ Done
Codegen emits a `static GemICacheSlot` per `.field` access site. On cache hit (same table + same shape_id), returns `t->vals[cached_index]` directly — no hash, no `gem_string()` allocation. Cache miss falls back to full `shgeti` lookup and populates the cache. `shape_id` on `GemTable` is bumped by structural mutations (delete, pop, sort, insert, remove_at) but not by set/push (which don't move existing key→index mappings). Monomorphic (1 slot per site) — sufficient for AST walking where each access site typically sees one table shape.

### `for k, v in tbl` allocates a keys array ✓ Done
Desugaring now uses `__table_key_at` / `__table_val_at` to index directly into the table's storage arrays. No keys array allocation, no per-key re-lookup.

## Runtime Hot Paths

### Inline value constructors and operators ✓ Done (2026-10-04)
`gem_int`, `gem_float`, `gem_bool`, `gem_truthy`, `gem_val_eq`, `gem_eq`/`gem_neq`/`gem_not` and the comparisons are `static inline` in gem.h, and `gem_add`/`gem_sub`/`gem_mul`/`gem_lt` inline their int (for `+` also float) case and call `gem_<op>_slow` in gem_ops.c for every other operand type. Codegen is unchanged: it already emitted these calls for every operator, `if` and literal. In a `sample` profile of `examples/logstat` (`--by ip`, from a file) the out-of-line calls were about 15% of the main thread. logstat, 1M lines (macOS arm64): `--by ip` from a file 3.19 → 2.42 s, from stdin 3.02 → 2.25 s; `--by path` 3.25 → 2.39 s and 3.16 → 2.36 s; `--by hour` 2.72 → 1.85 s and 2.64 → 1.85 s (Python: 2.0 s).

### POST burst p99 regression after 2026-05-01 cleanup pass ✓ Closed as noise (2026-05-01)
The reported "regression" (80→263→525 ms across three runs) was below the noise floor of the bench. Re-ran three back-to-back 30s POST bursts at HEAD with the same app and got p99 = 75 ms / 264 ms / 562 ms — the original three numbers fall inside the same envelope. p50 stayed flat at 35–39 ms in all runs. SQLite fsync tail on macOS is bumpy enough at this sample size (~1500 samples → p99 = worst ~15) that ~6× swings on the deepest tail are baseline variance, not a code change.

Lesson for future POST bench reads: don't trust a p99 swing on a 10s burst. Either lengthen `WRITE_BURST_DURATION` to 60s+ for smaller p99 sample noise, or run the burst 3+ times back-to-back and look at the variance band, not single numbers.

## Runtime I/O

### Thread pool for async I/O (Phase 1) ✓ Done
4 OS worker threads (`runtime/gem_threadpool.c`) handle `read_file`, `write_file`, `append_file`, `exec`, and `extern blocking fn` when called from a spawned process. The coroutine yields on submission; a wake-pipe notifies the scheduler on completion. Top-level (non-coroutine) I/O remains synchronous. The Gem-facing API is unchanged.

### Non-blocking sockets for TCP builtins ✓ Done
TCP builtins (`tcp_accept`, `tcp_read`, `tcp_write`, `tcp_connect`) use non-blocking sockets + `gem_io_yield(fd, direction)` to yield to the scheduler's `poll()` set. No thread pool involvement — the scheduler resumes the coroutine directly when the fd is ready.

**Why not the thread pool?** Benchmarked both approaches with `wrk -t4 -c100 -d10s` against an HTTP server:

| Approach | Req/sec | Notes |
|----------|---------|-------|
| Non-blocking + poll (net.c extern fn) | ~24,000 | Original baseline |
| Thread pool (tcp builtins v1) | ~1,550 | 15x slower — each request needs 3 thread pool round trips (accept/read/write), 4 workers caps throughput |
| Non-blocking + poll (tcp builtins v2) | ~23,800 | Matches baseline |

The thread pool adds per-operation overhead: mutex lock → enqueue → cond signal → worker thread pickup → execute → wake-pipe write → scheduler drain → process scan → resume. For socket ops on localhost where the actual I/O is microseconds, this coordination overhead dominates. With 4 workers and 3 ops/request, max throughput is ~4/3 ≈ 1.3k req/s — matches the 1,550 observed.

**Lesson:** Thread pool is correct for file I/O and `exec` (where the kernel provides no readiness notification). For sockets, always use non-blocking + readiness notification (poll, kqueue, epoll).

### Timer min-heap ✓ Done
Replaced the 256-slot fixed timer array with a dynamic min-heap keyed by `deadline_ms` in `runtime/gem_scheduler.c`. Insert is O(log n), `gem_fire_timers` pops expired entries from the root in O(log n) per fired timer, and `gem_earliest_timer_deadline` is O(1) (peek root). The 256-slot cap and "timer table full" failure mode are gone. Cancel-by-ref is still an O(n) linear scan to locate the ref before a heap remove (sift-down + sift-up); a side index ref→heap-pos would make it O(log n) if cancel ever becomes hot. Verified by `examples/85_timer_heap_capacity.gem` (1000 concurrent timers, in-order fire, 200 cancels).

### Lazy-paged coroutine stacks via mmap ✓ Done (2026-10-02)
Shipped together with stack-overflow containment (SPEC §"Stack depth"; `runtime/gem_scheduler.c` "Process stacks"). Every process stack, main included, is now an mmap'd block with a 64 KB `PROT_NONE` guard between minicoro's header and the stack, and `GEM_CORO_STACK_SIZE` went from 256 KB to 8 MB, the same as main. The old entry assumed malloc'd stacks were committed up front. On Linux glibc they were not: 256 KB is above the mmap threshold, so they were lazily paged too. Measured on Linux x86_64 with `VmRSS`, 1000 idle processes take 22.6 MB both before and after (≈16.5 KB per process, mostly arena and the stack's top pages). What changed is reserved address space: 8 GB with all 1024 slots in use, against 256 MB before. That is free on 64-bit Linux (`MAP_NORESERVE`, default overcommit) and macOS. Under `vm.overcommit_memory=2` it is charged in full, and spawn fails with a catchable "coroutine creation failed" once the commit limit is hit. Build with a smaller `-DGEM_CORO_STACK_SIZE` there.

Spawn cost: mapping, guarding and unmapping a stack per spawn made spawn+exit about 2.3x slower (200k spawn/exit: 1.5 s before, 3.5 s after). Released stacks go to a LIFO cache (`gem_stack_cache`) sized to the process table, `GEM_MAX_PROCS` (since the growable process table, a fixed 1,024: see "Growable process table" below). The cache can't hold more stacks than were alive at once, and each is trimmed on release, so the size costs address space, not memory. An earlier cap of 128 made every exit an 8 MB `munmap` and every spawn an `mmap` + `mprotect` once more than 128 processes churned: 200k spawn/exit with 1000 alive took 3.2 s on macOS and 3.7 s on Linux, against 0.8 s and 1.8 s on main.

On release, everything below the top 16 KB of the *stack* (not of the mapping, which has a trailing page beyond the stack; on 16 KB-page macOS that page alone filled the kept region, so the real top page was discarded and re-faulted on every spawn) is handed back with `madvise` (`MADV_DONTNEED`, or `MADV_FREE_REUSABLE` on macOS, the only one of the three that lowers `phys_footprint` there). `madvise` over 8 MB costs several microseconds on macOS even when nothing in the range is resident, so a single `mincore` over the 64 KB below the kept region decides first: stacks are touched from the top down, so if none of those pages is resident, nothing deeper is either and the `madvise` is skipped. A C frame larger than 64 KB that skipped the probed pages could leave deeper pages resident in the cached stack until it is reused; that costs memory, not correctness.

Measured after these fixes, 200k spawn/exit: macOS arm64 1.80 / 0.78 / 0.99 s with 1 / 100 / 1000 alive (main: 1.70 / 0.66 / 0.82 s); Linux x86_64 3.7 / 1.3 / 1.5 s (main: 3.7 / 1.6 / 1.8 s). Memory stays bounded: on Linux, four rounds of 128 processes recursing 20,000 deep, then 2000 small spawns, end at 14 MB RSS; on macOS, four rounds of 128 processes recursing about 6 MB deep, then 2000 small spawns, end at 27 MB `phys_footprint`.

On macOS, `ps` and `top` report RSS well above `phys_footprint` after deep processes exit (381 MB against 21 MB in the test above). That is how `MADV_FREE_REUSABLE` works: the pages are reclaimable but stay counted until the system needs them. Use `phys_footprint` (`footprint` or Activity Monitor's Memory column), not RSS, when looking for a stack leak on macOS. Call overhead of the soft limit check in `gem_push_frame` (one load and one compare) is within noise: fib(35) 1.07 s before vs 1.11 s after, averaged over 5 runs with ±10% run-to-run spread; self-compile of `compiler/main.gem` was 3.0–3.3 s in both.

Not done: ASan builds. ASan's own SIGSEGV reporting is replaced by the overflow handler, which hands non-guard faults to the default action, and minicoro's ASan fiber hooks were not exercised with the mmap'd stacks.

## Strings

### Static string literals and `type()` results ✓ Done (2026-10-04)
Codegen emitted `gem_string_with_len("...", N)` for every string literal and `gem_string("k")` (`gem_string_with_len` when the key held a NUL) for every record-literal and field-assignment key, so each evaluation allocated and copied the bytes; `type()` returned a fresh string too, so `type(v) == "string"` allocated two strings per check (std's argument checks run one per argument). Literals and keys now compile to `GEM_STR_LIT("...", N)` (gem.h), a `GemVal` whose `sval` points at the C string constant, and `type()` and the constant cases of `to_string` return such values. Sound because no runtime path writes a string's bytes in place or frees a string it did not copy, region resets leave strings outside the region alone, and copies to another process duplicate the bytes. 1M calls (macOS arm64): `string.index_of` on a 115–120-byte line 174 → 72 ms (`find`: 17 ms); `type(s) == "string"` 131 → 18 ms (with "Inline value constructors and operators": 53 ms and 8 ms). `examples/logstat`, 1M lines (macOS arm64), with `gem_strlen_check` and `gem_current_arena` made `static inline` in the same change (2–3% of it): `--by ip` from a file 5.18 → 3.19 s, from stdin 4.74 → 3.02 s; `--by hour` 3.96 → 2.72 s and 3.77 → 2.64 s (Python: 2.0 s).

### `find` builtin ✓ Done (2026-10-04)
`find(s, needle, start)` searches with `memchr` (+ `memcmp` for longer needles) in C. It replaced std/string's private `find`, which scanned 32 positions in Gem and then tested growing chunks with `str_replace`, and std/http's runtime extern `gem_bytes_find`. `string.index_of`, `contains` and `split` are now one `find` per match. `examples/logstat` (eight `index_of` and one `split` per line), 1M lines, macOS arm64: from a file 11.3–13.1 s → 4.5–6.0 s, from stdin 7.9–9.4 s → 4.3–5.7 s (Python: 2.0 s).

## std/json

### Fast path for escape-free strings in parse ✓ Done (2026-10-03)
`read_string` (std/json.gem, scanner) first scans for the closing `"`, checking for `\` and control bytes on the way, and returns one `substr` when the string has no escapes; only a string with an escape gets a buffer, which then copies whole runs between escapes instead of pushing byte by byte. Parsing a 2.5 MB string-heavy document (20,000 records of four short strings and a three-string array, best of 5): 514 ms with the fast path disabled, 371 ms with it (1.4x); 940 ms with the parser as it was before the rewrite in commit a6d6942, which pushed every byte into a buffer.

## std/http

### Hardened server back to origin/main's throughput ✓ Done (2026-10-03)
The round-2 hardening of std/http (request validation, deadlines, connection tracking by the server process) cost a quarter to a third of the throughput: `parse_head` alone was about 58k instructions for a 2-line head, spent in std/string's Gem byte loops (`index_of`, `split`, `lower`, `trim`) and in per-request checks. What brought it back:
- A runtime extern helper `gem_bytes_find(s, needle, from)` (memchr + memcmp, `runtime/gem_builtins_string.c`), next to `gem_bytes_span`, for every search in std/http (head terminator, line ends, request line, chunk lines). `gem_bytes_span` keeps the lookup tables of its last 16 byte sets (a memcmp of the set instead of rebuilding a 256-byte table per call).
- `parse_head` makes one pass over the lines: per line a find, a span to the colon (which also rejects whitespace in the name), a span check for control bytes (which replaces the whole-head `str_replace` check), a lowercase only when the name has an uppercase letter, and a trim by span. Repeated headers are collected per name and joined once at the end (was `"{prev}, {value}"` per repeat, O(n²)).
- A route pattern without `:` parameters is matched by string equality, and the path is split at most once per match. A target without `?` or `#` skips `url.parse`'s byte loop. `status_text(200)` doesn't read `STATUS_TEXT` (which copies the table into each new connection process on first read). A Connection value that is one lowercase token skips the split.
- Request bodies: `read_exact` pushes each `tcp_read` chunk straight into the result buffer; `fill` used to copy every 8 KB chunk twice (`substr`, then `"{rd.data}{chunk}"`).
- Server bookkeeping: one catch-all receive with a dispatch on the tag (the patterns built a string literal per comparison: 41 per connection), int pid keys instead of `"{pid}"`, and the server takes every queued message before it monitors the connections registered meanwhile: a short connection's `_http_closing` is usually queued right behind its `_http_conn`, so it needs no monitor and sends no DOWN.

Measured on Linux x86_64 (4 cores, shared with other jobs, so ±10%), a minimal app answering `GET /x` with `http.ok("hello")`; `bench.c` load generator with 16 connections, 4 s per run, mean of 3 alternating runs; callgrind on a fixed 2,000 requests:

| | origin/main (pre-hardening) | hardened, before | after |
|---|---|---|---|
| keep-alive, req/s | 14.0k | 10.6k | 14.3k |
| connection per request, req/s | 8.7k | 5.7k | 8.4k |
| keep-alive, instructions/request | 115k | 160k | 79k |
| connection per request, instructions/request | 221k | 339k | 187k |
| 8 MB POST body (Content-Length) | 75 ms | 120 ms | 57 ms |
| 12,000 repeated header lines (60 KB head) | 55 ms | 530 ms | 15 ms |

What is left per connection is mostly runtime work: the spawn (copying the closure env with the router, and the module slots: about 23k instructions), the server process's wakeup, and the arena teardown at exit.

## Scheduler / Concurrency

### Growable process table, scheduler without table scans ✓ Done
The process table was a static array of 1,024 slots, and every scheduler pass scanned all slots up to the high-water mark: picking READY processes, checking deadlines, building the poll set, checking thread-pool completions. Both changed together (runtime/gem_scheduler.c, "Process table" and "Run state"):

- **Table.** `gem_proc_table` is a `PROT_NONE` reservation for `GEM_MAX_PROCS` (262,144) slots, made accessible 64 slots at a time as the high-water mark grows; it never moves, so `GemProcess *` pointers stay valid across spawns. `GEM_MAX_PROCS=<n>` in the environment lowers the limit. New slots come from the high-water mark up to 1,024, then from freed slots (FIFO), then from the mark again, so programs with fewer than 1,024 live processes get the same slots as before (and the same pids until a slot is reused: the pid modulus is now 262,144, so the 1,101st process of a program that spawns them one at a time is pid 262,221, not 1,101), and the table only grows to the larger of 1,024 and the peak live count. The old FIFO over a pre-filled free list would have cycled through every slot of a large table (with a 65,536-slot build of the old scheduler, a spawn/exit churn test timed out after 180 s).
- **Run state.** A three-level bitmap of READY slots lets a pass run them in slot order (the old scan's order, so every recorded interleaving is unchanged) at a cost proportional to the READY ones. Deadlines (`receive ... after`, `sleep`, tcp timeouts) sit in an indexed min-heap; fd waiters and thread-pool waiters in their own lists; WAITING processes are a count. Every state change goes through `gem_proc_set_state`. Expired deadlines wake after a pass, all at once, and only for waits that began in an earlier pass, which reproduces when the old scan noticed them (an adversarial review caught a first version that woke one deadline value per pass: 100 sleepers 1 ms apart beside a CPU-bound process woke up to 1 s late). Pool waiters are checked only when the wake pipe had a byte.

Measured on Linux x86_64 (4 cores), best of 3, binaries only:

| Workload | Before | After |
|---|---|---|
| 200k ping-pong round trips, no idle processes | 0.30 s | 0.29 s |
| same, 1,000 idle processes in `receive` | 0.84 s | 0.35 s |
| same, 10,000 idle (impossible before) | — | 0.52 s incl. spawning them |
| 200k spawn + exit | 4.6 s | 2.1 s |
| 200k spawn + exit, 1,000 alive | 4.8 s | 1.9 s |
| ring of 50,000 × 20 rounds (before: a 65,536-slot build of the old scheduler) | 11.9 s | 6.6 s |

Live processes now stop at memory mappings, not the table: four per process, and `spawn` refuses once the runtime's mappings (`gem_runtime_maps`) would pass 7/8 of Linux's `vm.max_map_count`, about 14,000 processes by default, so the running processes keep room to grow; 100,000 idle processes ran in 2.1 GB with the limit raised (OPTIMIZATIONS.md, "Per-process footprint"). The stack cache stays capped at 1,024 stacks instead of growing with the table, so a burst of tens of thousands of processes doesn't leave their stacks mapped.

An adversarial review of the first version found what more processes newly reach, fixed in the same change: the thread pool's request queue was a fixed 1,024 entries (1,500 concurrent `exec` calls: 453 raised "I/O queue full"; it now grows); a spawn that hit the mapping limit left no mappings for anything else (main's next arena block killed the program; hence the headroom); under `ulimit -v` the 5.5 GB table reservation crowded out the program (it now takes at most 1/16 of `RLIMIT_AS`); and the non-blocking poll after every pass costs O(fd waiters), which with 7,000 idle connections made 20,000 message round trips take 5.9 s on Linux x86_64. It now runs after every pass only up to 64 fd waiters; above that, the next poll waits twice as long as the last one took, at most 1 ms (20,000 round trips beside 5,000 idle connections on macOS arm64: 57–66 ms; spacing polls a fixed 1 ms apart took 36.9 s there, since one poll of 5,000 fds takes over a millisecond).

### `std/supervisor` keeps every restart time ✓ Done (std modernization)
`restart` used to push the time of each restart onto `state.restart_times` and never drop old entries, so restarts were O(n²) in the supervisor's lifetime restart count and its memory grew without bound (8,000 restarts of a permanent child: 2.9 s). Restart times outside `max_seconds` are now dropped at each restart (`note_restart` in std/supervisor, `check_intensity` in std/dynamic_supervisor); what remains (O(restarts in the window) per restart) is tracked in OPTIMIZATIONS.md.

## C Interop Hardening

### Arity / type validation at extern boundary ✓ Done (2026-05-05)
`emit_extern_validation` (`compiler/codegen.gem`) now emits an `argc`/type-tag prelude at the top of every `extern fn` and `extern blocking fn` wrapper. One `if (argc < N) gem_error(...)` plus one `if (args[i].type != VAL_X) gem_error(...)` per typed param, mirroring the diagnostic style of runtime builtins. Tag mapping: Int→VAL_INT, Float→VAL_FLOAT, String/Bytes→VAL_STRING, Bool→VAL_BOOL, Ptr→VAL_INT, Table→VAL_TABLE. Errors mention the declared Gem-level type name (e.g. "expected Bytes, got int") rather than the underlying VAL_ tag, so the message lines up with the `extern fn` signature. Regression: `examples/91_extern_validation.gem` covers missing-arg and wrong-type cases via `pcall`.
