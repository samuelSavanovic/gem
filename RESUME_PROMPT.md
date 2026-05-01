# Resume: stress-test the recursion-elimination work

Branch: `main`. Item 5b shipped (`62816fd`), branch `tco-watermark-revert-explore` merged + cleaned up. OPTIMIZATIONS.md cleaned (`c45e8f3`, `b58ec78`). All refusal classes for `while true` PT-loop rescue are empty across the codebase.

The implementation hasn't been adversarially exercised yet. This session: write targeted stress tests, run them, and update runtime/docs for any soundness issues or now-overstated limitations we surface.

**Order**: #3 and #6 first (both probe closure-escape semantics and are the cases I'm least confident about). Then the rest.

## Background — what's in place

- `gem_box_alloc` (runtime/gem_core.c) malloc's a box and registers it in the current process's `pinned_boxes` (stb_ds map, value = mark bit).
- `gem_arena_reset_with_roots_pinned(roots, n, pinned_roots, m)` runtime entry. Pinned-root pre-pass walks each pinned box's contents into the fresh arena AND marks the pin-set entry; env-walk in `gem_deep_copy_fn` calls `gem_pin_mark_walked` to dedup. After all walks: `gem_pin_sweep` frees unmarked entries; survivors reset to untouched.
- `gem_pin_free_all` runs in `gem_free_proc_slot` on process death.
- `gem_copy_is_external` is a block-walk (replaces lo/hi bounding-box compare).
- Codegen: `filter_live_for_rescue` returns `{roots, pinned_roots}`; `compile_while` emits a separate `_pinned_loop_roots[]` array when needed.

Existing regressions: ex64 (10k-iter PT-while with table-valued mutated box), ex65 (50k short-lived calls — RSS bounded ~19 MB at 50k *and* 500k iters, validates sweep).

## Stress tests to write

Each is a numbered example (`examples/66_*.gem` and up). Add expected output to `examples/expected_output.txt` once verified. Run via `make test`.

### #3 — Closure escapes via spawn while box still locally live

**Pattern**: a fn-local mutated-captured box is read locally AFTER the capturing closure has been sent away to another process. This is the case the prompt's original "skip pinned from rescue roots" design would have broken — I added the separate `pinned_roots[]` array specifically to handle it. Need to actually exercise it.

```gem
fn worker(reply_to)
  let counter = 0
  let inc = fn() counter = counter + 1 end
  spawn(fn() inc(); inc(); inc() end)   # inc escapes; not live below
  let me = self()
  while true
    # `counter` is locally live but no capturing closure is alive in this fn
    let msg = receive()
    if msg == "stop"
      send(reply_to, {tag: "result", counter: counter})
      return
    end
    # padding to drive arena resets
    let junk = {x: "pad-" + to_string(counter), y: [1,2,3,4,5,6,7,8]}
  end
end
```

The spawned process bumps counter 3× on its end (Erlang semantics: those mutations stay in the receiver's copy, don't propagate back). The parent then sits in a PT loop. The parent's `counter` box must:
- survive arena resets even though no capturing closure is alive in the parent
- still be reachable via `gem_v_counter` (the parent's local `GemVal *`)

**Verify**: parent reads back `counter == 0` (no mutations propagate from spawn). 5000-iter PT loop. No crash.

If this fails: the `pinned_roots[]` pre-pass isn't picking up locally-rooted-but-env-orphaned boxes. Fix in `filter_live_for_rescue` — make sure `boxed_vars` ∩ `live_set` consistently goes to `pinned_roots` even when no live closure references the name.

### #6 — Cross-process closure transfer with mutated captures

**Claim being tested**: the runtime risk-register entry from item 5b said "cross-process closure transfer with mutating captures: out of scope, receiver's env-field boxes dangle on its arena reset". On second look I think this is overstated — the receiver's env-field box is arena-allocated via `gem_copy_alloc`, and the receiver's own arena reset migrates it via env-walk. There's no path where the receiver encounters the *sender's* malloc'd pin-set entry (deep_copy at spawn replaces the pointer with a fresh receiver-arena box).

**Pattern**:

```gem
let main_pid = self()

fn make_worker_closure()
  let count = 0
  fn()
    while true
      let msg = receive()
      if msg == "report"
        send(main_pid, {tag: "count", count: count})
      elif msg == "stop"
        return
      else
        count = count + 1
      end
      let junk = {pad: "x" + to_string(count), arr: [1,2,3,4,5,6,7]}  # drive resets
    end
  end
end

let w = spawn(make_worker_closure())   # receiver runs the closure with its own count
for i = 0, 5000
  send(w, "tick")
end
send(w, "report")
let r = receive()
send(w, "stop")
print("count: " + to_string(r.count))   # expect 5000
```

**Verify**: receiver's count survives many arena resets, reaches 5000. No crash.

If this works (it should): update OPTIMIZATIONS.md item 5b's "soundness bar" paragraph and the comment in `gem_deep_copy_fn`'s external branch — the cross-process limitation is overstated; the receiver's env-field boxes ARE arena-walked correctly. The actual semantic (mutations don't propagate to sender) is correct Erlang behavior, not a bug.

If it fails: trace which step breaks. Likely candidate is that the receiver's closure value isn't a rescue root in its PT loop, so env-walk doesn't visit the env, so the box isn't migrated. Fix would be in `filter_live_for_rescue` or `compute_live_at_backedge`.

### Then — the broader sweep

After #3 and #6 settle, work through:

#### #1 — Pin-set under heavy churn
Spawn 100+ processes simultaneously, each with its own pinned box pattern. Verify per-process pin-sets stay isolated (no cross-process leakage), no process leaks boxes on death (gem_pin_free_all in gem_free_proc_slot runs).

#### #2 — Nested capture chains
A closure that captures a closure that captures a closure, all with mutated boxes. Three levels deep.
```gem
let a = 0; let b = 0; let c = 0
let f = fn() a = a + 1; let g = fn() b = b + 1; let h = fn() c = c + 1 end; h end; g() end
# Run f() inside a PT loop, verify all three counters increment correctly.
```
Verify: env-walk correctly recurses through all three levels, each box's contents migrate. No double-walk (mark dedup works across nested envs).

#### #4 — Pin-set growing huge between resets
Allocate 100k+ pinned boxes within a single arena reset window (i.e., before bytes_allocated trips the threshold). Verify the stb_ds map handles the size, sweep still works, no quadratic slowdown.

#### #5 — Box content with embedded box references
A pinned box whose value is a table containing another pinned box (transitively). Verify recursive content migration walks find both, mark dedup works.

#### #7 — pcall(fn() spawn(fn() ... end) end) with mutated captures
The exact pattern OPTIMIZATIONS.md item 6 (now marked subsumed) describes. Validates the claim. Should now compile + run cleanly without warnings.

## Test scaffolding tips

- Drive arena resets by allocating ~200-byte tables/strings each iteration; the 1 MB threshold trips every ~5000 iters.
- For RSS bounding: `/usr/bin/time -l ./binary` reports peak memory footprint. Compare 5k iters vs 50k vs 500k — expect bounded.
- For correctness: verify computed values exactly. Mismatches indicate dangling pointers / double-walks / missing migrations.
- Check `/tmp/<bin>` from `--emit-c` output to see what codegen actually emits. Look for `gem_box_alloc`, `_pinned_loop_roots[]`, `gem_arena_reset_with_roots_pinned`.

## What we expect to find

Probably nothing wrong with #3 (it's the case I designed for). #6 will likely reveal that the cross-process "limitation" was overstated and we should update docs. #1–#5 and #7 are confidence-building rather than expected-bug-finders.

If something does break, the right move is to fix it at the source (per global CLAUDE.md: never work around a bug). Likely places:
- `filter_live_for_rescue` (`compiler/codegen.gem:3345`): which names go to roots vs pinned_roots vs neither.
- `gem_deep_copy_fn` external branch (`runtime/gem_core.c`): mark+recurse vs just preserve.
- `gem_arena_reset_with_roots_pinned` (`runtime/gem_core.c`): pre-pass ordering, sweep timing.

## Validation gates after each change

- `make test` — all examples (currently 54 passing).
- `make test-json`.
- `./build/gem compiler/test_liveness.gem --run` — should be 10/0 + 2/0.
- `make bootstrap` — fixed-point roundtrip. Strongest test; runs the compiler against itself.
- Codebase scan: zero `cannot reset` warnings:
  ```fish
  for f in (find . -name "*.gem"); ./build/gem $f --emit-c 2>&1 1>/dev/null | grep "cannot reset" && echo $f; end
  ```

## Files to read first (cold start)

1. `docs/OPTIMIZATIONS.md` items 5 (5b) and 6 — current claims about pin-set + cross-process.
2. `runtime/gem_core.c` — `gem_box_alloc`, `gem_pin_mark_walked`, `gem_pin_sweep`, `gem_arena_reset_with_roots_pinned`, `gem_deep_copy_fn`'s external branch.
3. `compiler/codegen.gem:3345–3500` — `filter_live_for_rescue` and `compile_while`'s reset emission.
4. `examples/64_pinned_box_pt_loop.gem`, `examples/65_pinned_box_many_calls.gem` — existing regressions; pattern for new tests.
5. `git show 62816fd` — item 5b commit message has the design rationale in compressed form.
