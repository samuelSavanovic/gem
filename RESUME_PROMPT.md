# Resume: item 5 (box relocation on arena reset) or pivot

Branch: `tco-watermark-revert-explore`. Item 7 (lift main-arena reset invariant) shipped in `a80a5d0`. The for-loop bloom follow-up shipped in this session — top-level main now mirrors `compile_fn` (shadow-walk for `local_names`, dynamic `fn_scope_locals` tracking with snapshot/restore at C-`{}` introducers). 8 spurious refusals cleared. `make test` green (50 examples); `make test-json` clean; `make bootstrap` clean. Liveness gate: 10/0 negatives, 2/0 positives.

## State at session start

- HEAD: tip of `tco-watermark-revert-explore` after the for-loop bloom commit.
- `local_names` at top level now populated via `collect_shadow_lets_in_fn(top_stmts, local_names)` (over-approximation including arm-lets and nested-scope lets).
- `fn_scope_locals` tracked dynamically: `set_add` in `compile_stmt`/`compile_stmt_return` let arms; snapshot/restore in `compile_while` body, `compile_if` arms, `compile_match` whens + else, `compile_receive_match` arms + after_body, `compile_stmt_return` block.

## Refusal landscape (post-fix scan)

**Class A — closure-captured boxed (item 5 territory): 10 sites** (unchanged from pre-fix count — bloom was Class B noise):
- `std/supervisor.gem:137` — `sup_name`
- `examples/30_supervisor.gem:137,237` — `sup_name`, `supervisor`
- `examples/50_arena_compact.gem:8,35,56,89` — `main_pid` ×4
- `examples/18_concurrency.gem:59` — `pong`
- `examples/51_http_server_test.gem:241` — `string` (module ref — anomaly: marked boxed even though module imports are read-only; resume prompt hypothesised this would clear post-item-7 via `preserve_external`. It didn't — the refusal hits the `boxed_vars`/`top_level_boxed` branch in `filter_live_for_rescue`. Worth a brief look before item 5 — may be a separate, smaller fix.)
- `examples/http_server/server.gem:137` — `sup_name`

**Class B — "no C local at back-edge" (item 4 fix (b) territory): 3 sites**:
- `examples/60_arm_let_in_pt_loop.gem:13` — `acc` (regression test, expected refusal)
- `examples/http_server/server.gem:198,200` — `fd` ×2 (acceptor arm-let pattern)

## What to do this session

Class A is dominated by supervisor-pattern refusals: closures capture boxed values whose box lives in arena memory. **Item 5** (box relocation on arena reset) is the principled fix — when the arena resets, walk the rescue roots and relocate any heap boxes pointed to into a stable region (BSS-backed or pid-0 arena). The recursion-elimination roadmap is otherwise functionally done; this is the last lever for the supervisor pattern.

Choices for this session:

1. **Investigate the `string` anomaly first** (~30min). Why is a frozen module ref classified as boxed in a non-top-level fn? Either it's incorrectly detected as written, or `boxed_vars` is computed from a captured-set that doesn't distinguish read-only module captures. A small fix here may clear 1 Class A site and inform the item 5 design.

2. **Start item 5 design**. Mechanism: at arena reset, the rescue path needs to relocate boxes. Currently `gem_arena_reset_with_roots` walks roots and copies values out — but a boxed root is `GemVal *` pointing into the arena; the `*` itself needs a new home. Options: (a) allocate a side-arena for boxes that PT loops would otherwise refuse, (b) promote refused boxes to BSS at compile time (per-fn analysis), (c) walk the env-chain and rewrite box pointers post-reset. Pick after sketching on paper.

3. **Pivot to a P1 elsewhere** — `gem_add` for strings (HTTP hot path) or NaN-boxing prep. Do this if the Class A refusals don't bite any benchmark workload right now (the supervisor patterns mostly run with bounded iter counts — if no benchmark shows unbounded growth, item 5 can sit at P3).

## What NOT to do

- **Don't** widen `filter_live_for_rescue` to silently permit closure-captured boxed roots without a relocation mechanism. That's silent corruption.
- **Don't** skip `make bootstrap` after any compiler change. Item-7 follow-up was a compiler change and roundtrip is the only safety net.
- **Don't** skip the liveness gate (10/0 negatives, 2/0 positives) — under-rescue is silent memory corruption.

## Background reading

- This session's commit (for-loop bloom fix) — added `collect_shadow_lets_in_fn(top_stmts, local_names)` at top-level main + dynamic `fn_scope_locals` tracking. See `docs/OPTIMIZATIONS.md` item 7 "Follow-up landed" paragraph.
- `a80a5d0` (item 7) — `preserve_external` copy-map flag, BSS-backed top-level boxes, `walk_for_tagging_stmts(top_stmts, true)`.
- `62aa227` (item 4 fix (a)) — `fn_scope_locals` threading for arm-let refusal.
- `9486736` (mutation-aware boxing) — read-only captures live as plain locals.
- `docs/OPTIMIZATIONS.md` "Next steps for `while true` adoption" — items 5, 7.
