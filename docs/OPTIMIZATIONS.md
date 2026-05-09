# Optimization TODOs

Future performance improvements. None are blocking — collect ideas here as they come up. Shipped optimizations live in [`OPTIMIZATIONS_LOG.md`](OPTIMIZATIONS_LOG.md) with their work logs and benchmark anchors.

Priorities are informed by benchmark results from the bookmark CRUD app.

Priority scale: **P0** = measurable impact on benchmark right now, **P1** = significant but requires groundwork, **P2** = nice to have or niche.

## Current performance reference

Single-threaded scheduler ceiling on M1 Pro is ~26–29k req/s on `/` (static HTML) regardless of c=4/100/500 — higher concurrency just queues. Full benchmark history is in `OPTIMIZATIONS_LOG.md`.

Key bottlenecks under the current arena+rescue mechanism:
- Per-process arena allocation eliminates GC pauses but arenas only grow during a process's lifetime; the back-edge rescue mechanism caps growth at 1 MB live-set per reset.
- String concatenation patterns that escape `build_string` still allocate per-concat.
- Per-iteration arena rescue work runs at every `while true` / TCO back-edge, contributing a measurable read-path p50 cost (see `OPTIMIZATIONS_LOG.md` "while true rescue+reset benchmark anchor").

## Arena / Memory

### Investigate post-idle high-water at high concurrency (P2)
Originally reported at 2.39 GB stuck post-c=500 with the 16 MB threshold. After lowering the default threshold to 1 MB, post-idle RSS at c=500 dropped to 174 MB — flat across +0/+30/+90s probes, so it's still not draining, but the absolute waste is now an order of magnitude smaller and unlikely to matter for typical workloads. The underlying mechanism (per-process arenas of completed connection handlers not fully releasing — likely `madvise(DONTNEED)` happens but `munmap` doesn't, or proc-table objects linger until late cleanup) is unchanged. Keep tracking but don't prioritize until a workload demonstrates the residual is a real problem. If revisited, trace `gem_proc_exit` against actual mmap accounting under load.

## Value Representation

### NaN boxing (P1)
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

### Tighten "TCO function not reachable from process root" warning — structural-decrease (P1)

A stopgap (option E) shipped 2026-05-09 that splits the diagnostic into `note:` (benign — outer reset boundary caps allocation) and `warning:` (genuinely dangerous). See `OPTIMIZATIONS_LOG.md` for the substrate. Still TODO is the principled fix:

**Structural-decrease termination check (option B from the original options table).** Verify each self-call passes a strict subterm of at least one parameter (`x.field`, `x[lit]`, etc.) so the recursion is provably bounded. Combined with the outer-reset detection from the stopgap, this would let the analysis emit zero diagnostics on the bounded-walker pattern and reserve `warning:` strictly for the unbounded case.

Why it's still worth doing despite E being landed:
- Termination analysis has uses beyond gating this one warning (compile-time infinite-recursion detection, termination guarantees for trusted code paths).
- The `note:` is still noise once the user has read it once. A walker the analysis can prove bounded should be silent.
- Estimated +150–200 LOC on top of the `mark_process_tail` substrate; the simplest version ("self-call arg is `x.field` / `x[lit]` of a param `x`") catches every tree walker we've written and is cheap to implement.

Soundness bar for B: the rescue mechanism's whole purpose was to make memory bounded under load — a regression here would re-pay that work. A self-call to `g(x)` (same param, no destructuring) must not pass the check.

### Indirect-spawn PT tagging — Stage B (P2)

Stage A (compile-time warning when `spawn(...)` arg[0] is not a literal `anon_fn`) shipped 2026-05-01. Stage B is closure-escape analysis: tag every `while true` that's the top-level statement of any `anon_fn` whose only escape paths are spawn-like calls. Soundness: a closure invoked from a non-spawn context retains the caller's frame state, so its inner `while true` is **not** process-tail. Escape analysis must conservatively refuse to tag if any non-spawn escape exists. Punted unless Stage A's warning becomes noisy in practice — at landing time the bootstrap'd compiler and `std/` emit zero such warnings.

Workaround for users (also recommended in the warning text): write `spawn(fn() inner_closure(args) end)` — the wrapper is a literal `anon_fn` at spawn-arg[0]. PT-tagging only fires for `while true` *literally inside* the spawn-arg `anon_fn`, so this workaround applies when the long-running loop can live in the wrapper.

### Dead code elimination (P2)
Unreachable code after `return`, `break`, `error()` could be stripped. Currently emitted as-is.

### Mutual tail call optimization (P2)
Mutual recursion (A calls B in tail position, B calls A) could use trampolines or computed goto. Lower priority — rare in OTP patterns. Self-recursive TCO already covers the common case.

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

### Selective receive save-queue optimization (P2)
`receive ... when` scans the mailbox from oldest to newest on every wake. If a process accumulates many messages and the match is near the end, that's O(n) pattern matches per wake. Erlang's optimization: remember which messages were already tested against the current receive and skip them on re-scan, only testing newly arrived messages. Non-trivial but maps onto the existing mailbox structure — a "scan cursor" per process that advances as messages are rejected and resets when the receive shape changes or a new message arrives.

### Lazy-paged coroutine stacks via mmap (P2)
`GEM_CORO_STACK_SIZE` is currently 256 KB, allocated via plain `malloc` in `gem_coro_stack_alloc` (`runtime/gem_scheduler.c:87`). malloc'd stacks pay the full physical commit up front, so every spawned process pays 256 KB regardless of how deep its call chain actually gets. Bookmark soak at c=500 spends ~120 MB on stack memory alone with the current setting; an HTTP handler that uses 8 KB of stack pays the same as the LSP doc process that uses 200 KB.

mmap-backed stacks (with `MAP_ANON | MAP_PRIVATE`) are zero-filled lazy-paged: virtual size is reserved up front but physical pages only commit when the stack actually touches them. Bumping the virtual size to 1 MB or 2 MB becomes free for stacks that don't use it, and a deep one-off pays only what it needs. Per-iteration cost is one extra `mmap` + `munmap` syscall per spawn/exit instead of `malloc`/`free`; both are page-aligned and fast.

Two implementation considerations: (a) ASan's allocator dislikes mmap'd stacks unless they're registered via `__asan_handle_no_return`; the malloc path is friendlier under instrumented builds, so an `#ifdef __SANITIZE_ADDRESS__` fallback is probably needed. (b) macOS and Linux differ slightly on guard pages — `mmap` + `mprotect(NONE)` for the bottom page catches stack overflows cleanly, whereas malloc'd stacks today silently corrupt adjacent heap. Adding a guard page is a small bonus alongside the lazy-paging change.

Trigger: re-bench bookmark app at c=500 and the LSP under load. If stack memory shows up as a meaningful fraction of RSS, ship this. If the existing 256 KB envelope is fine, defer.

### kqueue/epoll for sockets (P2)
The scheduler currently uses `poll()` for socket readiness. Replacing with **kqueue** (macOS/BSD) or **epoll** (Linux) would improve scalability at high connection counts (thousands of fds). `poll()` scans the entire fd set on each call — O(n) per wake. kqueue/epoll return only ready fds — O(ready). For the current HTTP server benchmark (~100 concurrent connections), `poll()` is not the bottleneck; this optimization matters when scaling to thousands of simultaneous connections.

## Scheduler / Concurrency

### Multi-threaded work-stealing scheduler (P2)
The scheduler is single-threaded — one `while(1)` loop round-robining coroutines on one OS thread. N scheduler threads with per-thread run queues and work-stealing (Chase-Lev deque) would scale throughput ~linearly with cores. The per-process arena model already eliminates shared-heap contention. Hard parts: mailboxes need lock-free MPSC queues for cross-thread sends, shared globals (`gem_proc_table`, `gem_name_registry`, free list) need synchronization, each thread needs its own kqueue/epoll set, and process migration (stealing a coroutine between scheduler ticks) needs care. Erlang/BEAM does exactly this architecture. Nothing in the current design blocks it — isolated processes, message passing, and per-process memory are the right foundation.

## C Interop Hardening

Findings from the 2026-05-01 `extern fn` / `extern blocking fn` soundness audit. Two unambiguous bugs (Ptr-return missing magic, NULL-string return crash) were fixed in-tree; the items below are tracked follow-ups.

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

## Known DX warts of the rescue+reset mechanism (P2)

The arena rescue+reset mechanism is invisible to user code by design — `while true` Just Works and resets at the back-edge once the threshold trips. This list captures the residual DX warts that remain even with everything currently shipped. None forces users to write code differently; all are observable by users in some form (jitter, throughput, mystery RSS) but not explainable from the source alone. Kept here so we don't pretend the trade-offs aren't there.

1. **Latency cliff at threshold crossings.** With a 1 MB threshold, most iterations of a tight loop pay only the gate check; every Nth iteration pays a full sweep+rescue (proportional to live-set size). Visible as p99 jitter on hot HTTP loops — see `OPTIMIZATIONS_LOG.md` for headline numbers. Post-rescue p99 is ~6–20× better than the unbounded-RSS baseline, but the floor isn't flat. Mitigation idea: adaptive threshold based on observed allocation rate, or a hint mechanism per loop. Both edge into "language tax" territory (CLAUDE.md), so probably never worth shipping unless a real workload demands it. Document, don't fix.

2. **Rescue set is invisible.** A user who accidentally captures a 10 MB value across the back-edge gets a 10 MB copy on every reset, with no way to see it. There is no introspection — no `--explain-rescue`, no runtime log, no per-loop accounting. Cheap mitigation: a `GEM_DEBUG_RESETS=1` env var that logs `[reset pid=N at line X: rescued K values, M bytes, took T µs]`. Surfaces the cost on demand without leaking it into everyone's mental model.

3. **Stdlib comments must not leak the mechanism.** A stdlib reader (or a user reading stdlib for examples) shouldn't have to know about `GEM_ARENA_RESET_THRESHOLD`, "back-edge", "PT tagging", or "rescue+reset". Comments should describe what a function does at the API level. Caught and removed two such comments in `std/http.gem` (commit `91bb6be`): `accept_loop`'s stale "depth 2 from process entry" fence, and `handle_connection_loop`'s `GEM_ARENA_RESET_THRESHOLD` reference. Future stdlib additions (and CLAUDE.md guidance) should keep this discipline. Not really an optimization — call it a documentation invariant.
