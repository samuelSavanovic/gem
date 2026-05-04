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

### ~~Arena compaction for long-running processes~~ ✓ Done via a different mechanism (2026-04-30 → 2026-05-01)
Original sketch: a runtime "periodic compaction" pass that walks roots from the coroutine stack and closure envs, copies live data to a fresh arena, frees old blocks. **That approach is unsound for Gem** — there is no precise GC root map, no stackmap on the C compiler's output, no way to safely walk a coroutine's spilled register state and identify which slots hold `GemVal` vs raw bytes. Adding any of that would be a major undertaking (full GC infrastructure on a language that explicitly chose arenas to avoid GC).

What actually ships gives the same end-state — old blocks freed, live data preserved — driven by **compile-time liveness** rather than runtime scanning:

- At every process-tail loop back-edge, codegen emits `gem_arena_reset_with_roots_pinned(roots, n, pinned, m)` guarded by a 1 MB threshold. The roots are the liveness pass's output filtered by `filter_live_for_rescue`; the runtime deep-copies live values into a fresh arena and `munmap`s the old blocks.
- Mutated-captured fn-local boxes are pinned outside the arena (item 5b) and migrated by the same walk; the pin-set's mark-and-sweep frees boxes orphaned by short-lived calls.
- Top-level boxed lets sit in BSS (item 7), so they survive resets directly.

Effective compaction frequency: every ~1 MB of arena-allocated bytes. Bookmark soak benchmark validates: peak RSS stays bounded across 5-minute runs (vs. unbounded growth pre-mechanism). No runtime root scan, no stackmap, no GC pause.

### ~~Growable structure waste~~ ✓ Done via the same mechanism
Original concern: `gem_table_grow` doubles capacity in place, leaving the old keys/vals backing as dead space until process exit. The back-edge reset above reclaims this — when the arena resets, the table is deep-copied with `cap = current_cap` (no historical doubling overhead), and the old arena blocks (containing every prior backing array) are `munmap`'d. So a long-running gen_server's route table that grew 1→2→4→…→1024 entries pays the dead-backing cost only until the next reset, not for the lifetime of the process.

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

### ~~Redundant `gem_push_frame` / `gem_pop_frame` for leaf functions~~ ✓ Done (2026-05-04)
A leaf function — one whose body contains no `call` AST node and no `receive_match` — emits its body without `gem_push_frame`/`gem_pop_frame` and without `gem_set_line`. Detection (`body_is_leaf` in `compiler/codegen.gem`) is conservative: every builtin invocation parses as a `call` (since `print`, `len`, `error`, … all go through `compile_call`'s direct-call path), so any use of one disqualifies the fn. Predicates / accessors / pure arithmetic / control-flow / table-and-array literal builders qualify; bookkeeping helpers do not.

State plumbing: a global `in_leaf_fn` is set in `compile_fn` and `compile_closure_fn` and saved/restored alongside the existing `boxed_vars`/`in_top_level`/`local_names`/`fn_scope_locals` quartet. Two helpers (`pop_str` and `setline_str`) replace every `gem_pop_frame()` / `gem_set_line(...)` literal in the codegen templates; both return empty strings when `in_leaf_fn` is set. The `gem_push_frame` calls at fn entry are gated directly. Top-level `main` is never marked leaf so the outermost frame is always present.

Stack-trace impact (acceptable): when a builtin errors inside a leaf, the trace shows the *caller's* frame at the call-site line (since the caller's `gem_set_line` populated the call-site line before evaluating the call expression). The leaf's name and per-stmt line are absent. Verified: `divide(10, 0)` from a non-leaf `caller` reports `at caller (file:6)` then `at main (file:10)` — leaf is invisible but the location is correct.

Coverage on the bootstrapped compiler: 50/121 named fns and 12/112 anonymous closures are leaf-elided. Microbench (5M iter tight loop calling `add(a, b)` and a 4-arm `classify(x)`): 140ms pre → 127ms post (−9.3%, three-run best). Bookmark HTTP `/` burst is within run-to-run noise (~1% delta either direction). The big bench savings would require many leaf calls per request; HTTP handlers mostly aren't leaf.

Regression: `examples/86_leaf_fn_elision.gem` covers a few leaf shapes (predicate, multi-arm if, table accessor) plus a non-leaf caller invoking leaf helpers in a loop.

### ~~Escape analysis for non-escaping closures (P1, partial)~~ ✓ Done (2026-04-30)
A pre-codegen pass marks `anon_fn` nodes that appear as the immediate argument of an allowlisted callee (`pcall`, `sort`) as `non_escaping`, provided the closure body contains no nested `anon_fn`s. Marked closures (a) skip contributing their captures to the enclosing function's `captured` set, so the enclosing params/locals stay unboxed, and (b) get a stack-allocated env in `compile_anon_fn` instead of `GC_MALLOC` from the arena. `spawn`/`spawn_link`/`spawn_monitor`/`send_after` deliberately stay off the allowlist — their closures run asynchronously in another coroutine and must keep the boxing-via-arena path. The conservative "no nested anon_fns" precondition sidesteps the case where an escaping inner closure could smuggle outer captures past the synchronous call boundary.

Concrete payoff: `safe_handle_request` deleted from `std/http.gem` and inlined into `handle_connection_loop`, which now has unboxed params and a stack-allocated pcall env. The `accept_loop` `let fd = client_fd` aliasing is **kept** — it dodges the *spawn* closure capture, which is still escaping.

Scope deferred: block-syntax `each`/`map`/`filter`/`reduce` from std/table aren't allowlisted yet (their callee parses as `dot`, not `var`, and they're regular Gem fns rather than builtins where we can audit storage behavior). Same goes for `build_string`. Add to allowlist when a workload demands it.

### ~~Static process-tail analysis~~ ✓ Done
Replaced the runtime depth-2 fence with a compile-time `mark_process_tail` pass (`compiler/codegen.gem`). A pre-codegen pass builds a call graph of `(caller_id, callee, is_tail_position)` records, seeds it with top-level statements and `anon_fn` bodies passed to `spawn`/`spawn_link`/`spawn_monitor`, then runs a greatest-fixed-point demotion: start with all candidates, demote any candidate whose incoming edge from a non-candidate non-seed caller isn't tail-position, repeat until stable. Final pass: BFS from seeds through candidates via tail edges to compute the actual `process_tail` set. Codegen at the TCO back-edge (line 827) drops the `gem_call_depth - entry_call_depth <= 2` check for `process_tail` functions; non-process-tail TCO functions get a compile-time warning and keep the depth check as a safety net.

**Why GFP demotion instead of BFS**: naive BFS-from-seeds (the original sketch below) over-approximates — it would mark functions reachable via tail calls even if they're also called non-tail elsewhere, which is unsound. GFP demotion correctly handles self-recursion (sup_loop, gen_server `loop`) and mutual tail-call cycles. A "trivial return" rule (`f(); return nil` or `f(); return <const>`) treats early-exit patterns as tail-effective so supervisor / gen_server idioms get marked.

**Result**: zero TCO warnings on all 49 examples + bookmark_app + std modules; bootstrap roundtrip clean; bench parity on `/bookmarks` c=50 t=4 30s (median 3157 vs 3104 req/s, RSS equal). The depth-2 fence is no longer the mechanism; it's a belt-and-suspenders safety for the rare functions the analysis can't prove.

**Follow-up (small)**: better warning cause-attribution. Today the warning fires on the demoted TCO function with a generic "reachable from non-tail context" message. Naming the offending non-tail call-site (file:line of the caller) would make the diagnostic actionable. Low priority — no demotions exist in the current codebase.

### ~~Eliminate user-visible recursion-as-iteration~~ ✓ Done (2026-04-30)
A `while true` loop in a process-tail context now resets the per-process arena at its back-edge using the same machinery as TCO. Recursion-as-iteration is no longer required for long-running processes.

**Mechanism shipped**:
1. **Liveness pass** (`compiler/liveness.gem`, ~770 LOC). Backward dataflow over `while true` bodies; returns `{ok: true, live}` or `{ok: false, reason}`. Closure captures over-approximated (every anon_fn's free vars unconditionally live across the back-edge). Refuses on `break` at this loop's level. Validation gate (`compiler/test_liveness.gem`) walks every TCO function in `std/` + `examples/`, synthesizes `while true { fn.body }`, and asserts `live ⊇ params` so no existing TCO loop would under-rescue if rewritten as `while true`.
2. **Process-tail tagging** (`tag_process_tail_while_loops` in `compiler/codegen.gem`). After `mark_process_tail` finishes, a tree walk tags every `while true` node inside a process-tail context (a fn in `process_tail_fns`, the top-level program, or a spawn-arg anon_fn body) with `process_tail = true`. The walker stops at anon_fn / fn_def boundaries except when descending into spawn-arg anon bodies (treated as fresh process roots).
3. **Codegen** (`compile_while`). When `node.process_tail == true` and `node.cond` is `bool true`, the back-edge emits `gem_arena_reset_with_roots(_loop_roots, N);` guarded by `gem_current_pid >= 0 && gem_current_arena()->bytes_allocated > GEM_ARENA_RESET_THRESHOLD`. The roots are the live set filtered against `local_names` ∖ `boxed_vars` ∖ globals/fn-names/builtins.
4. **Narrowed warning**. The blanket *"use tail recursion instead"* warning is replaced by a refusal-only warning naming the specific blocker (closure-captured live var, declaration in nested if/match arm, `break` in body). Process-tail `while true` loops with a clean liveness result emit no warning.

**Refusal cases**: empty as of 2026-05-01. The four classes that originally refused (closure-captured live var, declaration in nested if/match arm, `break` in body, `pcall(fn() spawn(...) end)`-style escape) are all resolved (items 2, 3, 4(b), 5b in the work log below). Codebase scan finds zero `cannot reset` warnings. The one residual advisory is a TCO-reachability hint — `compiler/main.gem:109 rename_node` is a self-recursive helper called from a non-tail context, so its TCO loop won't reset at the back-edge. Different mechanism, different fix; not a refusal of the rescue path.

**Closure-capture over-approximation (tightened 2026-04-30)**: liveness recognizes closures appearing as arg[0] of `spawn` / `spawn_link` / `spawn_monitor` and excludes their captures from the back-edge live set. Sound because the runtime deep-copies a spawned closure's captures into the child process at the call site (see `runtime/gem_scheduler.c`) — the parent's copies are dead after the call returns. Captures are still live AT the call site (via `_uses_expr` on the anon_fn), so any prior `let captured = ...` in the same iteration is correctly killed before the back-edge.
  - Still over-approximated: closures escaping via assignment to outer-scope vars, returns, or non-spawn calls. These keep the closure value alive in caller memory or another process's mailbox, so captures must remain live across the back-edge. Item 5b made this benign for the common case (mutated captures are now pin-rescued regardless of how the closure escapes), so the over-approximation no longer translates to a refusal.

**Result**: `examples/55_while_true_process_loop.gem` regression test runs 10000 iters × ~200 bytes/iter (≫1MB threshold) and completes with stable RSS. All 49 other examples + json_parser + bookmarks pass; bootstrap roundtrip clean; liveness gate (4 fns from `std/`) PASS.

### `while true` adoption — work log

All items shipped. The mechanism is now sound for every refusal class observed in real code; users no longer need to think about the rescue path. Each entry below is kept for historical context (commit refs, design rationale, regression tests). Listed in shipping order — each unblocks more user code without requiring users to think about the rescue mechanism (per `CLAUDE.md`: language must not leak runtime invariants onto users).

1. **Dogfood: rewrite `std/http` accept/handle loops as `while true`** ✅ done (2026-04-30). Both `accept_loop` and `handle_connection_loop` now use `while true`. Compiles with no warnings; bench at parity with `2026-04-30_head_quiet/` (see `2026-04-30_std_http_while_true/` log entry above). Confirms the back-edge codegen is shape-equivalent on a real workload.

2. **Hoist lets declared in nested `if`/`match` arms** ✅ done (`89058b3`, 2026-04-30). AST lift pre-pass in `compiler/codegen.gem` (option (b) — option (a) wouldn't have fixed the underlying C-scoping issue). For each `if`/`match`/`receive_match`, intersects top-level `let` names across arms, hoists `let n = nil` before the construct, rewrites the per-arm lets to assigns. Skips names already bound in the enclosing block to avoid shadow conflicts. Surfaced and fixed a latent capture-emission bug in `compile_closure_fn` (top-level boxed var captured into a closure created in non-top-level scope: env field stores the existing pointer, not its address). Negative test `arm-merged-lift-shape` and regression example `examples/56_arm_merged_let_process_loop.gem` added; gate at 7/0 negatives.

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

7. **Lift the main-arena reset invariant** ✅ done (2026-05-01). Top-level `while` loops are now PT-tagged (`walk_for_tagging_stmts(top_stmts, true)` in `compiler/codegen.gem`); `tag_process_tail_while_loops` no longer scopes top-level out. The runtime invariants that previously blocked this:

    - **Cross-process string sharing fast-path** (`gem_in_main_arena`): deleted from `gem_arena.c` / `gem.h`. Replaced by a more general external-pointer mechanism in `gem_deep_copy_internal` — when `preserve_external` is set on the copy map, any pointer outside `[old_arena_lo, old_arena_hi)` is left in place. Used both for cross-process value sharing (BSS-backed boxes, global arena strings) and for the top-level rescue.

    - **Top-level boxed lets**: previously allocated their boxes via `GC_MALLOC` in main's arena, so the box pointer became dangling on reset. Now declared as `GemVal gem_box_<name>; GemVal *gem_v_<name> = &gem_box_<name>;` (BSS-backed). Closure envs that captured those boxes survive resets unchanged.

    - **Frozen module tables** (e.g. `string` from `load "std/string"`): `gem_table_freeze` marks them immutable, and `gem_deep_copy_internal` previously short-circuited immutable tables with `if (!map->use_malloc && val.table->immutable) return val;`. Safe for spawn/send (source arena keeps living), unsafe for arena reset (source arena is about to be munmapped). Fix: only apply the immutable shortcut when `preserve_external` is *not* set.

    Top-level rescue rescues *all* top-level vars (the rescue list is populated from `top_level_vars` regardless of liveness, since identifying which top-level vars are live across a top-level back-edge is harder than just rescuing them all). `filter_live_for_rescue` permits top-level boxed names because they're BSS-backed.

    Regression: `examples/61_top_level_pt_loop.gem` (5000 iters of `string.split` at top level — exercises the frozen-module-table path and triggers ~4 arena resets at the 1 MB threshold). Bench vs `2026-04-30_post_loop_liveness/`: throughput at parity (+1–2% across read paths and POST burst), RSS peak 57.8 MB → 50.4 MB (−13%) and growth 29.2 → 18.9 MB (−35%). Logs: `benchmarks/logs/2026-05-01_main_arena_lifted/`.

    The compile-time refusal class for top-level boxed lets is gone, and the silent perf cliff for top-level `while` loops is closed. The non-top-level mutate-via-block residual was subsequently closed by item 5b (pinned boxes outside the arena, see above).

    **Follow-up landed (2026-05-01)**: top-level for-loop iter vars (`i`, `_for_i_N`, `_for_len_N`) were spuriously refused as Class B "no C local at back-edge" because `local_names` at top level was populated only from `top_level_vars` (flat) — `collect_top_let_names` does not descend into while-bodies, so iter vars introduced by for-desugaring were missing. Top-level main now mirrors `compile_fn`: populates `local_names` via `collect_shadow_lets_in_fn(top_stmts, local_names)` (over-approximation) and tracks `fn_scope_locals` dynamically (`set_add` in compile_stmt's let arm, save/restore at C-`{}` introducers — `compile_while` body, `compile_if`/`match`/`receive_match` arms, `compile_stmt_return` block). 8 false positives gone (7 for-loop iter vars at the top level, plus `compiler/main.gem:492` and `examples/59_read_only_capture_loop.gem:22`). The remaining acceptor arm-let pattern (`examples/http_server/server.gem`) was subsequently closed by item 4(b) (`cfb1541`) — fn-top placeholders for arm-lets live across an enclosing PT-loop's back-edge.

8. **Indirect-spawn PT tagging** (Stage A done 2026-05-01; Stage B open). When the spawned fn is the *return value* of a call rather than a literal `anon_fn` — `spawn(make_worker_closure(arg))` — `walk_for_tagging_node`'s spawn-arg case doesn't fire, because it pattern-matches `is_node(node.args[0], "anon_fn")`. The closure's body never gets PT-tagged, so its top-level `while true` runs without arena rescue and the receiver's RSS grows unbounded until process exit. This is a **silent perf cliff** of the kind the design philosophy explicitly disallows (CLAUDE.md: *"Silent perf cliffs are not"*).

    **Stage A — compile-time warning** ✅ done (2026-05-01). `walk_for_tagging_node` now emits a warning at every `spawn`/`spawn_link`/`spawn_monitor` call site whose `args[0]` is not a literal `anon_fn`. Threaded `source_name` through the walker (`tag_process_tail_while_loops` → `walk_for_tagging_stmts` → `walk_for_tagging_node`). Same DX bar as the *cannot reset* warning: surfaces the hidden constraint without silent regression. Audit at landing time: zero existing call sites in `std/`, `examples/`, `compiler/` triggered the warning — every existing `spawn(...)` passes a literal `fn(...)` or a trailing `do` block (which the parser desugars to `make_anon_fn`). Regression: `examples/73_indirect_spawn_warning.gem`.

    **Stage B — closure-escape analysis** (open). Tag every `while true` that's the top-level statement of any `anon_fn` whose only escape paths are spawn-like calls. Soundness: a closure invoked from a non-spawn context retains the caller's frame state, so its inner `while true` is **not** process-tail. Escape analysis must conservatively refuse to tag if any non-spawn escape exists. Punted unless Stage A's warning becomes noisy in practice — at landing time the bootstrap'd compiler and `std/` emit zero such warnings.

    Workaround for users (also recommended in the warning text): write `spawn(fn() inner_closure(args) end)` — the wrapper is a literal `anon_fn` at spawn-arg[0]. PT-tagging only fires for `while true` *literally inside* the spawn-arg `anon_fn`, so this workaround applies when the long-running loop can live in the wrapper.

The roadmap above assumes the existing soundness bar (under-rescue = silent memory corruption). Every change must keep the param-superset gate green and the negative-test gate green (currently 10/0), plus pin a deterministic stress test against the new pattern (cf. `examples/55_while_true_process_loop.gem` … `examples/58_break_process_loop.gem`).

#### Known follow-ups from item 4 — resolved

1. **Rescue codegen references arm-let names not at fn scope** ✅ done via fix (a) (2026-05-01); fix (b) (the principled DX-friendly version — extend the arm-let hoist pre-pass to cover lets live across an enclosing PT-loop's back-edge) shipped as item 4(b) (`cfb1541`). Threaded a new `fn_scope_locals` set through `compile_fn` / `compile_closure_fn` / top-level alongside `local_names`. `filter_live_for_rescue` refuses names that are in `local_names` but not in `fn_scope_locals` with the existing *"no C local at this loop's back-edge"* reason. Regression `examples/60_arm_let_in_pt_loop.gem`.

2. **`std/supervisor.gem:137` `sup_name` refusal** ✅ resolved by item 5b (`62816fd`). `sup_name` is mutated → boxed → captured by a spawn closure → live across the back-edge of a PT loop in the spawn body. With pin-set rescue, the box now survives reset and the refusal is gone.

### Benchmark baseline: process-tail `while true` rescue+reset (2026-04-30)

Canonical comparison point for the `while true` rescue+reset codegen vs. the pre-mechanism baseline. Use these as the reference going forward when judging codegen-level changes to the back-edge or liveness pass.

- **Baseline**: `6b6999a` (pre-`while true` rescue path; depth-2 fence still in play). Logs: `benchmarks/logs/2026-04-30_baseline_quiet/`.
- **HEAD at capture**: `16f49a0` (rescue+reset shipped, shadow-fix applied). Logs: `benchmarks/logs/2026-04-30_head_quiet/`.
- **std/http dogfood**: `std/http` accept/handle loops rewritten as `while true`. Logs: `benchmarks/logs/2026-04-30_std_http_while_true/`. Read throughput within −1.0% / −2.4% / −0.4% of `head_quiet` (burst /bookmarks, burst /, soak); p50/p99 within ±2%; POST parity. RSS peak 50 MB vs 56 MB and growth 28 MB vs 37 MB — slightly improved, well within run-to-run noise. Confirms `while true` rescue+reset and self-recursive TCO emit equivalent back-edge code on a real workload.
- **Conditions**: same M1 Pro, quiet machine, back-to-back runs. `bash benchmarks/run.sh` defaults (5s warmup, 30s read bursts c=10, 5min soak c=4, 10s POST burst c=4).

Headline numbers:

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

**Known regression — read-path p50 throughput, −4 to −6.5%**. Outside the ±2% noise band on `/bookmarks`. `/` (cheap static handler) and POST (SQLite-bound) are unaffected. Plausible cause: per-iteration arena `rescue+reset` work emitted at the `while true`/TCO back-edge of the accept and per-connection handler loops — it runs on every iteration regardless of whether the threshold gates the actual reset. The check itself (`gem_current_pid >= 0 && bytes_allocated > GEM_ARENA_RESET_THRESHOLD`) plus root-set construction is non-zero work in a tight HTTP request loop. Trade is intentional and favorable: tail latency collapses ~6–20× and memory growth is no longer unbounded over a soak. Investigating only worth it if a workload demonstrates the p50 hit matters more than the tail/memory wins (P2). When investigating, look at: (a) lifting the threshold check to a precomputed pointer-hot-path, (b) hoisting root-set construction out of the per-iteration path when the live set is loop-invariant, (c) skipping the rescue entirely when the live set is empty.

### Known DX warts of the rescue+reset mechanism (P2)

The arena rescue+reset mechanism is invisible to user code by design — `while true` Just Works and resets at the back-edge once the threshold trips. This list captures the residual DX warts that remain even with everything currently shipped. None forces users to write code differently; all are observable by users in some form (jitter, throughput, mystery RSS) but not explainable from the source alone. Kept here so we don't pretend the trade-offs aren't there.

1. **Latency cliff at threshold crossings.** With a 1 MB threshold, most iterations of a tight loop pay only the gate check; every Nth iteration pays a full sweep+rescue (proportional to live-set size). Visible as p99 jitter on hot HTTP loops — see the headline numbers above. Post-rescue p99 is ~6–20× better than the unbounded-RSS baseline, but the floor isn't flat. Mitigation idea: adaptive threshold based on observed allocation rate, or a hint mechanism per loop. Both edge into "language tax" territory (CLAUDE.md), so probably never worth shipping unless a real workload demands it. Document, don't fix.

2. **Rescue set is invisible.** Item 5b made pinning more implicit (mutated captured boxes pin automatically). A user who accidentally captures a 10 MB value across the back-edge gets a 10 MB copy on every reset, with no way to see it. There is no introspection — no `--explain-rescue`, no runtime log, no per-loop accounting. Cheap mitigation: a `GEM_DEBUG_RESETS=1` env var that logs `[reset pid=N at line X: rescued K values, M bytes, took T µs]`. Surfaces the cost on demand without leaking it into everyone's mental model.

3. **Stdlib comments must not leak the mechanism.** A stdlib reader (or a user reading stdlib for examples) shouldn't have to know about `GEM_ARENA_RESET_THRESHOLD`, "back-edge", "PT tagging", or "rescue+reset". Comments should describe what a function does at the API level. Caught and removed two such comments in `std/http.gem` (commit `91bb6be`): `accept_loop`'s stale "depth 2 from process entry" fence, and `handle_connection_loop`'s `GEM_ARENA_RESET_THRESHOLD` reference. Future stdlib additions (and CLAUDE.md guidance) should keep this discipline. Not really an optimization — call it a documentation invariant.

### ~~TCO reset with pinned roots~~ ✓ Done (2026-05-04)

`emit_tco_continue` (`compiler/codegen.gem`) now passes mutated-captured params (the `tco_boxed ∩ tco_params` slice) as `_tco_pinned_roots[]` to `gem_arena_reset_with_roots_pinned`, so the post-reset sweep keeps the boxes alive across the tail call (the assignment `*gem_v_<p> = arg_temps[i]` immediately after the reset would otherwise touch freed memory). The conservative `any_boxed`-skip guard is gone — TCO functions with mutated captures now reset per-iteration like any other.

Body-let boxed locals don't need pinning: top-level body lets re-allocate their box each iteration via `let x = gem_box_alloc()` before any use, and inner-scope lets aren't visible at the tail-call point. If such a box is referenced via a closure passed in args, env-walk during deep-copy marks it and the sweep keeps it alive automatically.

Regression: `examples/84_tco_mutated_param.gem` (10000-iter TCO with a closure mutating a captured param). Stress at 200k iters holds RSS at 52 MB (would otherwise grow linearly with allocations).

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

### ~~POST burst p99 regression after 2026-05-01 cleanup pass~~ ✓ Closed as noise (2026-05-01)
The reported "regression" (80→263→525 ms across three runs) was below the noise floor of the bench. Re-ran three back-to-back 30s POST bursts at HEAD with the same app and got p99 = 75 ms / 264 ms / 562 ms — the original three numbers fall inside the same envelope. p50 stayed flat at 35–39 ms in all runs. SQLite fsync tail on macOS is bumpy enough at this sample size (~1500 samples → p99 = worst ~15) that ~6× swings on the deepest tail are baseline variance, not a code change.

Lesson for future POST bench reads: don't trust a p99 swing on a 10s burst. Either lengthen `WRITE_BURST_DURATION` to 60s+ for smaller p99 sample noise, or run the burst 3+ times back-to-back and look at the variance band, not single numbers.

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

### Timer min-heap (DONE)
Replaced the 256-slot fixed timer array with a dynamic min-heap keyed by `deadline_ms` in `runtime/gem_scheduler.c`. Insert is O(log n), `gem_fire_timers` pops expired entries from the root in O(log n) per fired timer, and `gem_earliest_timer_deadline` is O(1) (peek root). The 256-slot cap and "timer table full" failure mode are gone. Cancel-by-ref is still an O(n) linear scan to locate the ref before a heap remove (sift-down + sift-up); a side index ref→heap-pos would make it O(log n) if cancel ever becomes hot. Verified by `examples/85_timer_heap_capacity.gem` (1000 concurrent timers, in-order fire, 200 cancels).

### kqueue/epoll for sockets (P2)
The scheduler currently uses `poll()` for socket readiness. Replacing with **kqueue** (macOS/BSD) or **epoll** (Linux) would improve scalability at high connection counts (thousands of fds). `poll()` scans the entire fd set on each call — O(n) per wake. kqueue/epoll return only ready fds — O(ready). For the current HTTP server benchmark (~100 concurrent connections), `poll()` is not the bottleneck; this optimization matters when scaling to thousands of simultaneous connections.

## Scheduler / Concurrency

### Multi-threaded work-stealing scheduler (P2)
The scheduler is single-threaded — one `while(1)` loop round-robining coroutines on one OS thread. N scheduler threads with per-thread run queues and work-stealing (Chase-Lev deque) would scale throughput ~linearly with cores. The per-process arena model already eliminates shared-heap contention. Hard parts: mailboxes need lock-free MPSC queues for cross-thread sends, shared globals (`gem_proc_table`, `gem_name_registry`, free list) need synchronization, each thread needs its own kqueue/epoll set, and process migration (stealing a coroutine between scheduler ticks) needs care. Erlang/BEAM does exactly this architecture. Nothing in the current design blocks it — isolated processes, message passing, and per-process memory are the right foundation.

## C Interop Hardening

Findings from the 2026-05-01 `extern fn` / `extern blocking fn` soundness audit. Two unambiguous bugs (Ptr-return missing magic, NULL-string return crash) were fixed in-tree; the items below are tracked follow-ups.

### Arity / type validation at extern boundary (P2)
Generated `extern fn` wrappers read `args[i].sval` / `args[i].ival` directly without checking `argc` or `args[i].type`. A Gem-side mistake (wrong arity, wrong type) doesn't error — it reads garbage out of the union and passes it to C, where the result depends on the C function's tolerance. Built-ins (`read_file`, `tcp_*`, etc.) already do these checks and `gem_error` with a typed message. Bringing extern wrappers in line is a small codegen change in `compile_extern_fn` / `compile_blocking_extern_fn`. Trade-off: a few extra branches per call vs catching bugs at the boundary instead of in C.

### String-return ownership convention is path-dependent (P2)
`extern blocking fn` String returns are documented (SPEC §C Interop) to be `malloc`/`strdup`'d — the runtime copies into the arena and `free`s the original. `extern fn` (non-blocking) String returns are *not* freed: the runtime `gem_string`s the pointer (which copies) but the original is leaked if it was malloc'd, or fine if it was a static literal. Two reasonable behaviors with opposite ownership rules. Options: (a) document the asymmetry in SPEC, (b) unify on the blocking convention (always free), (c) introduce a `StringStatic` / `StringOwned` distinction. (a) is the cheapest; (b) is the most consistent but breaks the obvious `getenv`/`strerror`-style use case.

### Pointer lifetime across arena reset (documentation only)
A C function that stashes an arena-backed `String` or `Table` `GemVal` past the call (e.g. in a `static` cache) will dangle on the next arena reset (TCO loop, rescue+reset, or process exit). The marshaling layer can't enforce this — extern is unsafe by definition — but SPEC should call it out alongside the existing "extern is C, you own correctness" framing.

### `extern blocking fn` with no callers in tree
There are zero `extern blocking fn` declarations in `std/`, `examples/`, or `compiler/`. The codegen path is exercised only by ad-hoc test programs, not the test suite. If the convention churns (per the String-return point above), add at least one example that round-trips a malloc'd string through the thread pool to lock the contract in.

## std/json

### Null byte handling in strings (P2)
C strings are null-terminated, so embedded `\x00` bytes are invisible to the parser. This causes one JSONTestSuite failure: `n_multidigit_number_then_00` (a number followed by a null byte parses as valid because the null truncates the input). Fixing this requires length-aware strings throughout the runtime — depends on the string views/slices work above. When strings carry their length, the scanner can detect unexpected null bytes and reject them.

### Fast path for escape-free strings in parse (P2)
`parse_string` always allocates a buffer and pushes byte-by-byte. Most JSON strings contain no escapes. A fast path that scans for the closing `"` first (checking for `\` along the way) and uses `substr` when no escapes are found would avoid the buffer allocation entirely. 2-3x speedup on string-heavy JSON.

### Scanner as plain table instead of closure (P2)
The closure-based scanner (`{peek, advance, skip_ws}`) pays for hashmap lookup + closure call + captured variable access on every character. A flat table `{input, pos, length}` with module-level functions `peek(s)`, `advance(s)`, `skip_ws(s)` avoids closure overhead. More idiomatic for a language without methods.
