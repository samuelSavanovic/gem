# Resume: Item 7 — lift the main-arena reset invariant

Branch: `tco-watermark-revert-explore`. Part 1 (rescue codegen fix for item-4-surfaced arm-let bug) just shipped — see `62aa227`. This session is **Part 2** of the recursion-elimination roadmap: item 7 is the only remaining P1, and the only item that closes a *silent* perf cliff (`CLAUDE.md` forbids those).

## State at session start

- HEAD: `62aa227` "Refuse arm-let rescue roots — fix item-4-surfaced codegen bug"
- `make test` green (50 examples incl. new `examples/60_arm_let_in_pt_loop.gem`); `make test-json` clean; `make bootstrap` clean.
- Liveness gate: 10/0 negatives, 2/0 positives.
- `examples/http_server/server.gem` compiles end-to-end (warns on `acceptor`'s `fd`/`acc` arm-lets — that's the expected residual from Part 1's fix (a); fix (b) — extending the arm-let hoist pre-pass to cover lets live across an enclosing PT-loop's back-edge — remains a P3 follow-up).
- Item 5 (box relocation on arena reset) is still gated on a real-code measurement; defer until after item 7 lands and you can re-scan refusal counts.

## The invariant to lift

`compiler/codegen.gem:691` `tag_process_tail_while_loops` calls `walk_for_tagging_stmts(top_stmts, false)` to exclude top-level `while` loops from PT tagging. The reason — quoted in the comment at lines 697–703 — is the runtime fast-path:

```c
// runtime/gem_core.c:443, inside gem_deep_copy_internal, case VAL_STRING
if (!map->use_malloc && gem_in_main_arena(val.sval)) return val;
```

`gem_in_main_arena` (`runtime/gem_arena.c:98`) checks whether a string's pointer lies inside `gem_proc_table[gem_main_pid].arena`. When true, cross-process deep-copies *share* the pointer instead of strdup-ing — main is assumed to live forever, so the pointer can't dangle. If we let main's PT-tagged top-level loops reset main's arena, those shared strings would dangle in every other process that received them.

**Consequence today:** any long-running loop at top level (e.g. a `while true` accept loop directly in the main script, no `spawn` wrapper) silently grows the main arena unbounded. No warning. That's the cliff.

## Two paths

### (a) Drop the fast-path; measure deep-copy cost (default)

One commit's worth of runtime change:

1. Delete the `gem_in_main_arena` short-circuit in `gem_deep_copy_internal` (`runtime/gem_core.c:443`). Cross-process string copies always go through `gem_copy_strdup` + `gem_copy_map` (already the path for non-main pointers).
2. Drop the `false` parameter at `compiler/codegen.gem:704` so top-level statements get the same PT tagging as fn bodies.
3. Update the comment at `tag_process_tail_while_loops` (lines 697–703) to reflect the lifted invariant.
4. `make bootstrap` round-trip.

Measurement: bench against `benchmarks/logs/2026-04-30_post_loop_liveness/` using the canonical `bash benchmarks/run.sh` (defaults: 5s warmup, 30s read bursts c=10, 5min soak c=4, 10s POST burst c=4). Expect modest deep-copy regression on cross-process string-heavy paths; conjecture is small because most cross-process strings are short-lived (request bytes, not config). Save logs under `benchmarks/logs/2026-05-01_main_arena_lifted/`.

### (b) Re-anchor shared strings into a long-lived shared pool before reset (defer)

The principled fix: maintain a process-arena-independent pool that strings get promoted into when first shared cross-process; deep-copy returns the pooled pointer; arena reset is safe because the pool outlives the per-process arena. Real implementation work in the runtime (allocator, hash map keyed on string identity, lifecycle around `spawn` and `send`). Only do this if (a) regresses noticeably.

## Validation

- `make test` still green; `make test-json` clean; `make bootstrap` clean. Liveness gate still 10/0 / 2/0.
- New regression: top-level PT loop variant of `examples/55_*` (rename `55_while_true_process_loop.gem`'s shape but place the loop directly at top level, no spawn wrapper). Confirm it now PT-tags and resets the arena across iterations (no unbounded RSS growth in `time -lp` over a high iteration count).
- `examples/http_server/server.gem` still compiles (the warnings from Part 1 are unchanged; lifting the invariant doesn't touch arm-let logic).
- Before/after RSS bench using `benchmarks/run.sh`. Compare throughput, p50/p99, and RSS growth vs. `2026-04-30_post_loop_liveness/`.

## After landing

1. Update `docs/OPTIMIZATIONS.md` "Next steps for `while true` adoption" item 7 — mark done, link the bench logs, note any regression vs. baseline.
2. Re-scan `std/`, `examples/`, `examples/http_server/` for `*** is captured by a closure (box lives in arena memory)` warnings under HEAD. Count them. If supervisor + a couple more, item 5 (box relocation on arena reset) becomes the next P2. If fewer, demote to P3 and the recursion-elimination roadmap is functionally done.

## Background reading

- `62aa227` (Part 1) — `fn_scope_locals` threading; `filter_live_for_rescue` arm-let refusal.
- `9486736` — mutation-aware boxing (item 4). Surfaced the Part 1 bug.
- `81a75d6` + `d65565d` — widened PT tagging + post-loop liveness substrate (items 2/3).
- `docs/OPTIMIZATIONS.md` "Next steps for `while true` adoption" — item 7 description; canonical roadmap.
- `benchmarks/logs/2026-04-30_post_loop_liveness/` — the comparison baseline for the bench.

## What NOT to do

- **Don't** chase `std/supervisor.gem:137` `sup_name` refusal. It's the residual for item 5 (mutated capture in a closure used in a PT loop). Same shape as before; ignore unless you're explicitly working on item 5.
- **Don't** skip the bench. Item 7 trades a hot-path optimization (string sharing) for a uniform invariant; the user needs numbers to judge if path (b) becomes necessary.
- **Don't** widen the rescue codegen or touch boxing. Part 1 fixed the bug item 4 surfaced; further classifier work belongs in item 5.
- **Don't** skip `make bootstrap`. The compiler is full of closures; if the runtime change perturbs anything, bootstrap divergence is the canary.
- **Don't** skip the liveness gate. 10/0 / 2/0 is the bar; a regression here masks under-rescue corruption.
