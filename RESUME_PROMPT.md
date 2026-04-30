# Resume: rescue-codegen fix surfaced by item 4, then continue recursion-elimination roadmap

Branch: `tco-watermark-revert-explore`. Item 4 (mutation-aware boxing) just shipped — see commit log and `docs/OPTIMIZATIONS.md` "Next steps for `while true` adoption" item 4. This session has two parts:

1. **Fix the rescue-codegen bug item 4 surfaced** (small, P1). One real bug, blocking `examples/http_server/server.gem` from compiling.
2. **Continue the recursion-elimination roadmap.** Item 4 was the highest-frequency refusal class. The two remaining items in the roadmap are independent: (5) box relocation on arena reset, and (7) lift the main-arena reset invariant. Pick based on what you find when you measure real-code impact.

## State at session start

- Last item 4 commit: see `git log -5`. Branch pushed.
- Liveness gate: **10/0 negatives, 2/0 positives**. `make test` green (all 49 + new `examples/59_read_only_capture_loop.gem`). `make bootstrap` clean. Stress 55–58 green.
- `make test-json` clean; `test-json-suite` 282/283 (one pre-existing failure, not a regression).
- `examples/http_server/server.gem` **does not currently compile** — see Part 1.

## Part 1 — Rescue codegen references arm-let names not at fn scope

### What's broken

```
$ ./build/gem examples/http_server/server.gem -o /tmp/server
warning: examples/http_server/server.gem:137: ... live var `sup_name` is captured by a closure ...   # see Part 2
examples/http_server/server.gem:201:86: error: use of undeclared identifier 'gem_v_fd'; did you mean 'gem_v_sfd'?
  201 |  GemVal *_loop_roots[] = {&gem_v_total, &gem_v_active, &gem_v_server_fd, &gem_v_fd, &gem_v_r, &gem_v_acc, &gem_v_router, &gem_v_me};
```

The rescue at the back-edge of the outer `while true` (line 198 in `acceptor`) emits `&gem_v_fd`, but `fd` is declared inside the `else` arm at line 215 (`let fd = client_fd`). It's a C-block-scoped local — the `&` from outside that block won't resolve.

### Why it's surfacing now (and not before item 4)

Pre-item-4, `fd` was always boxed because the inner `spawn(fn() pcall handle_connection(fd, r) ... end)` closure captured it. `filter_live_for_rescue` (`compiler/codegen.gem` ~line 2984) refused with *"live var `fd` is captured by a closure"* before ever building the broken `_loop_roots` array. Item 4 reclassifies `fd` as read-only (the inner closures only read it), so it's no longer in `boxed_vars`, the refusal lifts, and the latent codegen bug surfaces.

Verify with the pre-item-4 compiler:

```
$ build/gem_old_pre_4b examples/http_server/server.gem -o /tmp/srv_old 2>&1 | head -3
warning: examples/http_server/server.gem:198: ... live var `fd` is captured by a closure ...
warning: examples/http_server/server.gem:200: ... live var `fd` is captured by a closure ...
```

### Pointers

- `compiler/codegen.gem`:
  - `filter_live_for_rescue` (~line 2984). The function that decides which live vars become rescue roots.
  - The `set_contains(local_names, name)` branch (line 2988) accepts the name but doesn't verify it has an addressable fn-scope C local.
  - `compile_fn` (~line 3640) — `scope_vars` is the set of params + `collect_top_let_names(body, ...)` results. That's the set of names that get a `GemVal gem_v_<name>` declaration at fn scope. `local_names` is broader (`= shadowed_locals = scope_vars + collect_shadow_lets_in_fn`), which is why arm-lets slip through.
  - `compile_closure_fn` (~line 3779) has a parallel structure for closure bodies.
- The arm-let-hoist pre-pass: search for `to_hoist` in `compiler/codegen.gem` (~lines 380–402). It already hoists arm-lets to fn scope **when conditions allow** (no shadowing/duplication). It does not currently consider liveness across an enclosing PT-loop's back-edge.

### Two fixes — pick one

**(a) Refuse-instead-of-broken-codegen** *(fast, default)*. Tighten `filter_live_for_rescue` so it knows the difference between "in fn scope" and "in some enclosing block". Easiest: thread `scope_vars` (or a renamed `fn_scope_locals`) through to `compile_while`, and in `filter_live_for_rescue` require both `set_contains(local_names, name)` AND `set_contains(fn_scope_locals, name)` for the "is local" branch. If only the former, fall through to the existing `"live var `{name}` has no C local at this loop's back-edge"` refusal. Cost: one warning re-emerges on `server.gem:198` (`fd`); user can restructure by moving `let fd = client_fd` outside the else-arm.

**(b) Extend the arm-let hoist** *(principled, larger)*. When a let inside an arm is live across an enclosing PT-loop's back-edge, lift it to fn scope (declare `GemVal gem_v_<name> = nil;` at fn top, rewrite the arm-let to an assign). The `to_hoist` machinery already does this for if/match arms when names are shared across arms; extend its trigger to "live across the enclosing PT-loop". Cost: a real codegen pre-pass — but no DX warning, no user restructure.

Default to **(a)** unless (b) is small. Add a regression `examples/60_arm_let_in_pt_loop.gem` (smaller version of the `acceptor` shape) so the choice is pinned.

### Validate Part 1

- `examples/http_server/server.gem` compiles (no error; (a) emits a warning, (b) doesn't).
- `make test` still green; new regression `examples/60_*.gem` runs.
- Liveness gate still 10/0 / 2/0.
- `make bootstrap` clean.

## Part 2 — Recursion-elimination roadmap, what's left

Read `docs/OPTIMIZATIONS.md` "Next steps for `while true` adoption" — items 5 and 7 are the two remaining independent items.

### Item 7 — Lift the main-arena reset invariant (P1, runtime)

**Why first.** It's the only item that closes a *silent* perf cliff (top-level long-running loops grow memory unbounded with no warning — `CLAUDE.md` forbids this). Items 4, 3, 2 closed compile-time refusals; item 7 closes a runtime invariant. The DX impact is high and the roadmap explicitly calls this out as P1.

**The invariant.** `gem_deep_copy_internal` in `runtime/gem_core.c` has a `gem_in_main_arena` fast-path that *shares* strings allocated in main's arena across processes (cheaper than copying — main never exits). If main's arena reset, those shared pointers would dangle. So `tag_process_tail_while_loops` calls `walk_for_tagging_stmts(top_stmts, false)` to scope top-level out of PT tagging.

**Two paths.**
- **(a) Drop the fast-path; measure deep-copy cost.** Conjectured small — most cross-process strings are short-lived. One commit, then a perf bench vs. `2026-04-30_post_loop_liveness/`.
- **(b) Re-anchor shared strings into a long-lived shared pool before reset.** Real implementation work in the runtime. Defer unless (a) regresses noticeably.

Once landed: drop the `false` in `tag_process_tail_while_loops`, add a regression that exercises a top-level PT loop (variant of `examples/55_*` without the spawn wrapper).

### Item 5 — Box relocation on arena reset (P2, mid)

**Why second.** Item 4 compresses the residual mutated-capture refusal class to "mutate-via-block in a process-tail loop" — `let total = 0; arr.each(fn(x) total = total + x end)` and the supervisor `sup_name` case (see *Known follow-ups* in OPTIMIZATIONS.md). Item 5 lifts this last common Ruby/Lua-flavored DX cliff: walk the boxed-var roots in `gem_arena_reset_with_roots`, copy each box's value out, reset, re-allocate, copy back, fix up `GemVal *` in env structs and locals.

The resume prompt for item 4 said: *"Defer until (4) lands; only do this if real-code measurement shows the mutate-via-block pattern is common in idiomatic Gem."* That measurement is the gating question. Before picking up item 5, scan `std/`, `examples/`, and `examples/http_server/` for `*** is captured by a closure (box lives in arena memory)` warnings under HEAD. If supervisor + a couple more, do item 5; otherwise sit on it.

### Sequencing recommendation

1. Land Part 1 (rescue codegen fix) — it's blocking real workload compile.
2. Item 7 (lift main-arena invariant) — silent perf cliff, and the only remaining P1.
3. Measure remaining real-code refusal counts → decide on item 5 vs. demote to P3.

## Background reading

- `81a75d6` — widened PT tagging (item 3).
- `d65565d` — post-loop liveness substrate (item 3 prereq).
- Item 4 commit (this session) — mutation-aware boxing. Adds `walk_writes` / `walk_writes_node` / `find_capture_writes_in_scope` / `find_writes_in_closure_body` in `compiler/codegen.gem`. Read these — they're the analysis pass everything in Part 1 will reuse if it touches the captured-var classifier.
- `docs/OPTIMIZATIONS.md` "Next steps for `while true` adoption" — the canonical roadmap; keep updates in sync there.

## What NOT to do

- **Don't** skip the `make bootstrap` round-trip. The compiler is full of closures; if any analysis you touch is wrong, bootstrap diverges.
- **Don't** widen the rescue codegen to "best-effort" — refuse cleanly when a name isn't addressable. Silent under-rescue corrupts memory.
- **Don't** chase the `std/supervisor.gem:137` `sup_name` refusal as a bug. It's a legitimate residual for item 5 (mutated capture in a closure used in a PT loop). Same shape as the prior session's misleading note about the supervisor — ignore unless you're working on item 5.
- **Don't** skip the validation gate. 10/0 negatives + 2/0 positives is the bar; a regression here masks under-rescue corruption.
- **Don't** touch the runtime's deep-copy fast-path for item 5 unless you're explicitly tackling item 7. They're orthogonal — main-arena fast-path is about *cross-process sharing*, box relocation is about *single-process arena reset*.
