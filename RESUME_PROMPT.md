# Resume: runtime cleanup / readability pass

Branch: `main`. Big feature shipped (arena rescue+reset for `while true` PT loops; users can write Ruby-style imperative code without recursion). Bench at parity, two cleanup commits landed (`58b43c1` refactor, `f58c302` stale-comment sweep). Runtime itself was only lightly touched and still carries vestiges from the Boehm-GC era.

This session: make the runtime read like it actually works. The compiler is now clear; the C side is not.

## Goal

Anyone reading `runtime/*.c` and `runtime/gem.h` cold should understand the memory model from the names and comments alone. Today they will see `GC_MALLOC` everywhere, conclude there's a garbage collector, and be wrong.

## Concrete targets (ranked by impact)

### 1. Kill `GC_MALLOC` / `GC_MALLOC_ATOMIC` / `GC_INIT` (P0, mechanical)

**Status:** 62 occurrences across 11 runtime files. All three macros are defined in `runtime/gem.h:55-58`:

```c
/* Compatibility shim — old stage0.c emits GC_INIT() in main() */
#define GC_INIT() ((void)0)
#define GC_MALLOC(size)        gem_arena_alloc(gem_current_arena(), (size))
#define GC_MALLOC_ATOMIC(size) gem_arena_alloc(gem_current_arena(), (size))
```

The comment is stale — the compatibility shim is no longer needed. `bootstrap/stage0.c` was regenerated multiple times since Boehm came out; check whether it still emits `GC_INIT()` at codegen time. If yes, fix codegen to stop emitting it. If no, the macros are pure vestigial naming.

**Plan:**
- Audit codegen: search `compiler/codegen.gem` for `"GC_MALLOC"`, `"GC_INIT"`. Any emission sites? If yes, replace with `gem_arena_alloc(gem_current_arena(), ...)` directly (or a shorter helper, see below).
- Sweep runtime: replace `GC_MALLOC_ATOMIC(n)` and `GC_MALLOC(n)` with `gem_arena_alloc(gem_current_arena(), n)`. No semantic difference — they're the same macro now (the `_ATOMIC` distinction was a Boehm signal that the alloc contains no pointers, irrelevant for a bump arena).
- Consider introducing a shorter helper `gem_alloc(n)` that wraps `gem_arena_alloc(gem_current_arena(), n)` — 62 call sites, all use `gem_current_arena()`. Saves visual clutter.
- Delete the three macros from `gem.h` once call sites are clean.
- Run `make test` + `make bootstrap` — bootstrap roundtrip is the strongest signal here since stage0.c regeneration will surface any codegen emission of the old macros.

**Delegate to sonnet** — pure mechanical sed-equivalent work after the codegen audit is done. Give it the call-site list and ask for the rename + shim removal + verification. ~15 min of agent time, low risk.

### 2. Stale comments referencing GC, depth-2 fence, gem_in_main_arena (P0, mechanical)

`gem_in_main_arena` was deleted in item 7 of the rescue+reset work. Check for lingering doc references:

```fish
rg -n 'gem_in_main_arena|main arena fast.path|Boehm|garbage colle' runtime/ compiler/
```

The depth-2 fence is still live (TCO non-PT path) but several comments described it as "the runtime depth-2 fence" — that fence is in codegen now (`emit_tco_continue`), not runtime. Tier 1 cleanup last session caught some; runtime side may have more.

**Delegate to sonnet** — give it the rg output and a cleanup spec. Mechanical.

### 3. `runtime/gem_core.c` — 754 LOC, probably mixed concerns (P1, judgment)

Per `CLAUDE.md` the runtime is split by category (`gem_ops.c`, `gem_error.c`, etc.). `gem_core.c` was supposed to be "value constructors, table internals, equality" but accreted other things during the rescue+reset work — particularly `gem_arena_reset_with_roots` and `_pinned` variants live there now (lines ~675-757) but really belong in `gem_arena.c`.

**Plan:**
- Read `gem_core.c` end-to-end (it's 754 LOC, ~30 min).
- Identify functions that don't fit the category (arena reset entry points, deep_copy, pin-set helpers).
- Move misplaced functions to the right file (`gem_arena.c` for arena/reset, possibly a new `gem_copy.c` for deep_copy machinery if it's >100 LOC).
- Update `gem.h` decls accordingly.

**Do this myself, not sonnet** — requires reading code and judging fit. Sonnet can handle the file-move once boundaries are decided.

### 4. Two reset entry points — investigate or document (P1)

Last session deferred collapsing `gem_arena_reset_with_roots` (called from TCO emission, codegen.gem:2452) into `gem_arena_reset_with_roots_pinned` because the TCO caller passes no pinned roots. Two questions:

- Can a TCO function have mutated-captured pinned boxes that are live across the tail call? If yes, the TCO emitter is currently unsound and we have a bug. If no, the unpinned variant is correct but the asymmetry deserves a comment in `gem.h` explaining "TCO doesn't need pinned roots because [reason]".
- If the analysis goes either way, capture it in `gem_arena_reset_with_roots`'s doc comment and either fix the bug or remove the dual-entry-point clutter.

**Do this myself** — soundness analysis, not mechanical.

### 5. `runtime/gem.h` organization (P2, judgment)

562 LOC. Likely contains:
- Tagged value layout
- Process table / scheduler decls
- Public API
- Internal helpers leaking out
- Macros that should be inline functions

Skim with an eye for: which decls are truly public (called by generated C from codegen) vs. internal (only used between runtime .c files). Internal-only decls could move to a new `gem_internal.h`. Lower priority — only worth doing if reading the file is genuinely confusing.

**Defer unless time allows.**

### 6. Pin-set / deep_copy duplication (P2)

`gem_deep_copy_internal` in `gem_core.c` and the pin-set logic in arena reset both walk closure envs. Last session's review flagged potential duplication but didn't dig in. If #3 separates these into their own file, the duplication will become obvious or vanish. Revisit after #3.

## Process recommendation

Order: **#2 → #1 → #4 → #3 → #5/6**.

- #2 first because comment-only changes can't break anything; clears the stage.
- #1 next because it's a global rename — best done before structural changes invalidate line numbers.
- #4 before #3 because it might change what lives in `gem_arena.c`.
- #3 last among the structural items because it's the biggest move.

**Delegate to sonnet:** #1 (after codegen audit), #2, file-moves in #3 once boundaries decided.
**Do myself:** codegen audit in #1, soundness analysis in #4, boundary decisions in #3, judgment in #5.

After each step:
- `make test` (`ALL EXAMPLES PASSED`)
- `make bootstrap` (roundtrip clean — strong signal since stage0.c regen will catch any compiler-emission regressions)
- `bash benchmarks/run.sh` ONLY at the end — runtime renames shouldn't move bench numbers, but verify before committing.

## Commit hygiene

Two-commit pattern from last session worked well:

1. **Mechanical** (rename, comment fixes, file moves) — one commit.
2. **Structural** (helper extraction, function moves with logic changes) — separate commit.

Don't mix them; bisect-friendly history matters here because bootstrap regeneration touches `bootstrap/stage0.c` (~30k lines diff per commit) and isolating which compiler/runtime change actually caused a behavior shift requires clean separation.

## Files to read cold

1. `runtime/gem.h` — full 562 LOC. Map the sections.
2. `runtime/gem_arena.c` — 120 LOC, smallest runtime file, defines the actual arena.
3. `runtime/gem_core.c` — 754 LOC. The cleanup target.
4. `compiler/codegen.gem` — search for `GC_MALLOC` and `GC_INIT` to scope the codegen audit for #1.
5. `docs/OPTIMIZATIONS.md` — section "Arena / Memory" and the "while true adoption — work log" for context on what mechanism is in place.

## Validation gates

- `make test` after every change.
- `make bootstrap` after every batch of changes that touches codegen or value layout.
- `make test-json`.
- `./build/gem compiler/test_liveness.gem --run` — liveness gate (10/0 + 2/0).
- Final bench run, compare to `benchmarks/logs/2026-05-01_14-47-39/`.

## Out of scope

- TCO + pinned-boxes soundness fix beyond #4's analysis (if a real bug exists, scope it as a separate session).
- Item 8 Stage B (escape analysis for indirect-spawn) — still open but Stage A's warning hasn't surfaced any real call sites.
- NaN boxing / string interning / new builtins — these are feature work, not cleanup.

## What "done" looks like

- Zero `GC_*` identifiers in the runtime.
- Reading `gem.h` + `gem_arena.c` + `gem_core.c` cold conveys the per-process arena model accurately.
- File boundaries match `CLAUDE.md`'s description.
- Bench at parity (within ±2%).
- Two clean commits, push, done.
