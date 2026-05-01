# Resume: close item 8 (indirect-spawn PT tagging)

Branch: `main`. Stress-test sweep done (`examples/66`–`72` shipped); item 5b is well-exercised. Surfaced one open issue, tracked as **OPTIMIZATIONS.md item 8**: `spawn(make_closure())` (where the spawned fn is the *return value* of a call, not a literal `anon_fn`) silently doesn't PT-tag the closure's `while true`. The receiver's arena grows unbounded until process exit. This is a silent perf cliff and contradicts the project's design philosophy (CLAUDE.md: *"Silent perf cliffs are not"*).

This session: close item 8. Pick **conservative first** (warning), then evaluate whether the principled fix is worth shipping.

## Background — what's in place

- `walk_for_tagging_node` (`compiler/codegen.gem:893–943`): tree walker that stamps `process_tail = true` on `while` nodes inside a process-tail context. The spawn-arg special case at line 925:
  ```
  if node.tag == "call" and is_node(node.func, "var") and is_spawn_callee_name(node.func.name)
    if node.args != nil and len(node.args) > 0 and is_node(node.args[0], "anon_fn")
      let anon = node.args[0]
      walk_for_tagging_stmts(anon.body, true)
      ...
    end
  end
  ```
  Pattern-matches *literal* `anon_fn` at `args[0]`. Returning a closure from a fn (`spawn(make_worker(...))`) doesn't match.
- `is_spawn_callee_name` (line 877): `spawn`, `spawn_link`, `spawn_monitor`.
- The receiver, when running an indirect-spawned closure with a top-level `while true`, gets no `gem_arena_reset_with_roots_pinned` calls. RSS grows linearly with iteration count.

## The fix — staged

### Stage A (conservative): warning at indirect-spawn call sites

In `compile_call` (or nearest equivalent in codegen) — when the callee resolves to `spawn`/`spawn_link`/`spawn_monitor` and `args[0]` is **not** an `anon_fn`, emit a compile-time warning:

```
warning: <file>:<line>: spawn target is not a literal `fn() … end`; any `while true` inside the spawned closure will not be process-tail-tagged and the receiver's arena will not reset (silent perf cliff). Pass an anonymous fn directly: `spawn(fn() <body> end)`.
```

Existing warning helper: search `compiler/codegen.gem` for `"warning:"` to find the helper used by other compile-time refusals.

This is the same DX bar as the *cannot reset* warning class — surfaces the hidden constraint instead of letting it silently regress runtime behavior.

### Stage B (principled, evaluate after Stage A): closure-escape analysis

Tag every `while true` that's the top-level statement of an `anon_fn` whose only escape paths are spawn-like calls. Requires escape analysis on closure values (track each `anon_fn` to call sites where it escapes; if all escapes are `spawn(<this anon>)` or returns from a fn whose only call sites are `spawn(<call>)`, tag is safe).

Soundness check: a closure invoked from a non-spawn context retains the caller's frame state, so its inner `while true` is **not** process-tail. Escape analysis must conservatively refuse to tag if any non-spawn escape exists.

This is non-trivial — punt to a follow-up unless Stage A's warning turns out to be wildly noisy. Most real code passes literal anon fns to spawn (the std/http loops do; the bootstrapped compiler does).

## Stress test

Add `examples/73_indirect_spawn_warning.gem`:
```gem
# Should emit a Stage A warning at compile time.
fn make_loop(reply_to)
  fn()
    while true
      let msg = receive()
      if msg == "stop"
        send(reply_to, "done")
        return
      end
    end
  end
end
let me = self()
let w = spawn(make_loop(me))
send(w, "stop")
print(receive())
```

Verify:
- Compile emits the new warning at the `spawn(make_loop(me))` line.
- Program still runs to completion (warning is non-fatal).
- After Stage B (if shipped): no warning, codegen emits `gem_arena_reset_with_roots_pinned` for the inner loop; rerun with 5000 ticks + RSS check via `/usr/bin/time -l`.

## Files to read first (cold start)

1. `docs/OPTIMIZATIONS.md` item 8 (lines 206–211) — the spec for what we're fixing.
2. `compiler/codegen.gem:877–943` — `is_spawn_callee_name` and `walk_for_tagging_node`'s spawn-arg case.
3. `compiler/codegen.gem` — search for `"warning:"` to find the existing warning emission helper (used by `cannot reset` and other refusal classes).
4. `examples/55_while_true_process_loop.gem` … `examples/58_break_process_loop.gem` — pattern for adding a numbered example with deterministic output.
5. `examples/66_pinned_box_escaped_closure.gem` and `examples/67_pinned_box_cross_process.gem` — recent stress tests for context on what's already covered.

## Validation gates after change

- `make test` (currently 61 passing — adding 73 makes 62).
- `make test-json`.
- `./build/gem compiler/test_liveness.gem --run` — should still be 10/0 + 2/0.
- `make bootstrap` — fixed-point roundtrip.
- Codebase scan: every existing `spawn(<call>)` or `spawn(<var>)` should now warn. List them with:
  ```fish
  rg -n 'spawn\s*\([^f]' --type-add 'gem:*.gem' -tgem
  ```
  Audit each: is it a real silent-cliff pattern, or is it `spawn(some_var_holding_fn)` where the var came from an immediate `fn(...)` literal that just got bound? The latter is also a silent cliff but worth noting separately.
- Bootstrap may surface warnings at `compiler/main.gem` / `compiler/codegen.gem` — investigate before silencing. If the compiler itself triggers Stage A warnings, that's a real DX issue to fix.

## What we expect to find

- Stage A warning is mechanical — a few lines in `walk_for_tagging_node`'s call branch (or wherever spawn calls are inspected) plus the warning emission. Should land in <100 LOC.
- Stage A may surface 0–N existing call sites in `std/`, `examples/`, or `compiler/` that hit this pattern. Each is a real bug (silent RSS growth in long-running loops). Audit and either fix the call sites (use literal anon-fn) or, if pervasive, escalate to Stage B.
- Stage B is real work (~200–400 LOC of escape analysis). Only ship if Stage A's signal:noise is bad.

## Closure: open questions if Stage A surfaces real bugs

- Does `std/supervisor.gem`'s spawn pattern hit this? It uses `spawn_link(fn() supervisor_loop(state) end)` (literal anon-fn wrapper) — should be fine.
- Does `std/http`'s accept loop hit this? Recent dogfood (`2026-04-30_std_http_while_true/`) rewrote it as `while true`; check whether the spawn site is a literal anon-fn.
- The bootstrap'd compiler self-compiles — if it had this bug at scale, `make bootstrap` would already be slow. Empirically it's not, suggesting the pattern is rare in our own code.
