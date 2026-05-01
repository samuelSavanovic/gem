# Resume: extern fn / extern blocking fn soundness audit

Branch: `main`. Last session cleared two of three queued items; this session is dedicated to item 3, which is a substantial read-the-code-and-write-up task in the same shape as the TCO reset analysis from two sessions ago.

## Last session in one screen

Commits on `main`:
- `b084374` — Log POST burst p99 regression in `docs/OPTIMIZATIONS.md` under Runtime Hot Paths (P2). Three cleanup-era bench runs show POST p99 at 263-525 ms vs 80 ms baseline, but throughput, p50/p75/p90, soak GET, RSS curve all unchanged. Only the deep tail (~15 worst POSTs out of ~1550) is affected. Ruled out: arena blowup, TCO guard (doesn't apply to http hot path — handlers are `while true`, not TCO), DB carry-over (run.sh wipes), app-log anomalies. Cause not identified; deferred. If revisited, bisect the four cleanup commits (`3919775`, `442d8ed`, `d658a45`, `9642aa8`) or run with `dtruss`/`sample` against the app PID during the POST burst.
- `cef5645` — Remove `runtime/test_concurrency.c` and the `make test-concurrency` target. The hand-written C tests were written against the pre-arena runtime model; since `533f27b` (per-process arena migration), `gem_spawn_fn` deep-copies its env using the Gem closure layout `[intptr_t count; GemVal* slots[count]]`, and the tests' raw C struct envs are misread as that layout. Only the two tests that pass `env=NULL` actually worked. Recent GC_INIT() removal exposed it via segfault instead of silent UB. Examples/ already covers spawn/send/receive/selective-receive/monitoring/links/registry through real Gem programs.

Validation gates after item 2: all four green (test, test-json, liveness 10/0+2/0, bootstrap roundtrip).

## This session — item 3 only

### C interop soundness audit — `extern fn` and `extern blocking fn` (P1)

Gem treats C interop as unsafe by default, but the user is concerned about places where pointers/values can leak across the safety boundary in unintended ways. This is a **soundness audit**, similar in shape to the TCO reset analysis from two sessions ago: read the code, identify lifetime/aliasing bugs, document or fix.

Specifically check:

- **Lifetime**: arena-allocated values passed to C functions that hold pointers past the call. Next arena reset frees them. What happens if a C function stashes a `GemVal *` somewhere?
- **Cross-process / threadpool**: `extern blocking fn` runs on a 4-thread worker pool (`runtime/gem_threadpool.c`). What can a worker thread see/touch in the calling process's arena? Race conditions on shape_id, pin-set, mailbox?
- **Return values**: C strings returned via marshaling — who owns the storage, who frees, when does the arena it's copied into get reset?
- **The marshaling boundary**: `extern fn puts(s: String) -> Int` — generated wrapper in `compiler/codegen.gem`. Audit each path (String, Int, Float, etc.) for assumptions that break under arena reset.
- **`extern include`**: arbitrary C headers can declare types and functions. What's the trust model? Anything we're implicitly relying on (alignment, layout)?

Files to read:
- `runtime/gem_threadpool.c` — the blocking pool
- `runtime/gem_scheduler.c` — how blocking fn calls suspend the coroutine
- `compiler/codegen.gem` — search for `extern_fn`, `extern fn`, `blocking` to find marshaling code
- Existing extern fns in `std/*` — patterns we already ship that should serve as reference

**Deliverable shape**: a written-up list of what's safe vs what's a soundness risk, plus fixes (or `docs/OPTIMIZATIONS.md` entries for tracked follow-ups) for the risks. Same level of rigor as the TCO analysis last session.

## Validation gates (run after each commit)

```bash
make test            # ALL EXAMPLES PASSED
make bootstrap       # roundtrip clean
make test-json       # === all tests passed ===
./build/gem compiler/test_liveness.gem --run   # 10/0 + 2/0
```

`make test-concurrency` no longer exists.

## Out of scope

- Re-running the bench (cleanup-era POST p99 already logged as P2; not blocking).
- TCO reset option B (pinned roots in `emit_tco_continue`) — tracked in OPTIMIZATIONS.md, not urgent.
- Refactoring `gem.h` — defer unless reading it actively confuses you during item 3.
- New language features.
