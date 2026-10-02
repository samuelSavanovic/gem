# Optimization TODOs

Future performance improvements. None are blocking — collect ideas here as they come up. Shipped optimizations live in [`OPTIMIZATIONS_LOG.md`](OPTIMIZATIONS_LOG.md) with their work logs and benchmark anchors.

Priorities are informed by benchmark results from the bookmark CRUD app.

Priority scale: **P0** = measurable impact on benchmark right now, **P1** = significant but requires groundwork, **P2** = nice to have or niche.

## Current performance reference

Single-threaded scheduler ceiling on M1 Pro is ~26–29k req/s on `/` (static HTML) regardless of c=4/100/500 — higher concurrency just queues. Full benchmark history is in `OPTIMIZATIONS_LOG.md`.

Key bottlenecks under the current arena + region-reset mechanism:
- Per-process arena allocation eliminates GC pauses; every loop resets the region it allocated once it passes max(1 MB, 2 × the last reset's cost), so memory is bounded at roughly 3× a loop's live data plus whatever was allocated before the loop started.
- String concatenation patterns that escape `build_string` still allocate per-concat.
- Every loop entry takes a region mark and every back-edge checks the trigger (a load and a compare); the reset itself is amortized O(1) per allocated byte (see `OPTIMIZATIONS_LOG.md` "Region resets").

## Arena / Memory

### Spawn copies the parent's module state (P2)
Module-level bindings are per-process, so `spawn` deep-copies every module slot (namespace tables are immortal and shared, so they cost nothing). A program loading `std/http`, `std/json` and `std/log` pays ~4 µs extra per spawn (22 µs vs 18 µs for 100k short processes), mostly copying `STATUS_TEXT`, mime tables and similar constant data. Options: share slots whose values are immutable after module init (needs a "never written after init" analysis, or freezing literal tables that nothing mutates), or copy lazily on first access per slot. Only worth it if spawn-heavy workloads show it.

### Garbage allocated before a loop starts is kept by that loop (P2)
A region reset frees only what the loop allocated since its mark. Garbage from straight-line code before the loop (e.g. main's startup work before a top-level `while true` server loop) stays until an enclosing loop resets or the process exits. It is a constant, not growth. For loops at depth 0 of the main program the mark could sit at the start of the arena (main has no caller frames and its module slots are roots), reclaiming startup garbage too; spawned bodies would need their env rooted.

### Remembered log instead of a per-table walk for buffers (P2)
Region resets find old tables written since the mark through a write barrier and a remembered log, but still walk every buffer older than the mark (cost charged to the hysteresis budget). A barrier in `buf_push`/`gem_string_append` growth would make the walk proportional to the buffers actually grown.

### Mailbox messages are copied by each reset that finds them in the region (P2)
Messages are deep-copied into the receiver's arena by the sender, so a backlog that arrives during a loop is part of that loop's region and is copied at each reset until it is consumed. Hysteresis keeps the total linear (300 × 100 KB messages drain in linear time), but a large backlog doubles its memory while a reset runs. Allocating message bodies in a separate per-process message arena that a reset never scans (freed when the message is received and dropped) would avoid both; the cost is a second allocator and a copy (or ownership transfer) when the message is received.

### Investigate post-idle high-water at high concurrency (P2)
Originally reported at 2.39 GB stuck post-c=500 with the 16 MB threshold. After lowering the default threshold to 1 MB, post-idle RSS at c=500 dropped to 174 MB — flat across +0/+30/+90s probes, so it's still not draining, but the absolute waste is now an order of magnitude smaller and unlikely to matter for typical workloads. The underlying mechanism (per-process arenas of completed connection handlers not fully releasing — likely `madvise(DONTNEED)` happens but `munmap` doesn't, or proc-table objects linger until late cleanup) is unchanged. Keep tracking but don't prioritize until a workload demonstrates the residual is a real problem. If revisited, trace `gem_proc_exit` against actual mmap accounting under load.

## Value Representation

### NaN boxing (P1)
GemVal is currently 16 bytes (4-byte type enum + 8-byte union + padding). NaN boxing packs type + value into a single 8-byte double by exploiting the NaN payload space. Halves memory per value, improves cache locality, eliminates the type field branch in hot paths. The payoff isn't just memory — every function call, table lookup, and arithmetic op passes GemVals, so halving their size compounds across the entire runtime. Requires rewriting every GemVal constructor and accessor. Major undertaking but large payoff. Would also directly reduce arena allocation pressure since every value, table entry, and function argument shrinks.

### Avoid hashing integers in tables (P2)
Integer keys currently fall through to a linear scan if they don't match the array-style fast path (sequential 0..n). A dedicated int hash map (parallel to `str_index`) would give O(1) lookup for sparse integer keys. Low priority — most integer keys follow the array pattern.

## Strings

### String views / slices (P1)
`substr` allocates a copy. A view (pointer + offset + length) into the original string would make substring extraction O(1). Strings already carry their length (`slen`), but every consumer that relies on the trailing NUL (C interop, `printf %s`) would need to copy or check views first. Defer until profiling shows substring allocation as a real bottleneck — the C interop boundary assumes null-terminated strings throughout, and `substr`/`ord(s, i)` already cover the hot cases without changing the representation.

### Search and scan builtins so std string loops run in C (P1)
Byte-at-a-time loops written in Gem (`ord(s, i)` per byte, plus a reduction check at every back-edge) are much slower than the same loop in C, and std leans on them: `std/string`'s `index_of`, `contains`, `split`, `starts_with` and `ends_with` all compare byte by byte through `str_eq_at`, and `std/http`'s `html_escape` dispatches on every byte. `split` also builds each piece with `buf_push(buf, chr(ord(s, i)))`, allocating a one-byte string per input byte. Two general builtins would move the inner loops into C:

- `find(s, needle, start)` — `memmem`-backed; index of the first match at or after `start`, or -1. `index_of` and `contains` become one call, `split` becomes `find` plus one `substr` per piece, and `std/http`'s search for the `\r\n\r\n` header terminator (one `ord` comparison per byte) becomes one call.
- `find_any(s, chars, start)` — index of the first byte at or after `start` that is in the set `chars`, or -1. `html_escape`, `std/url`'s percent-encoding and tokenizers like the `std/json` scanner scan to the next special byte, then copy the whole run before it. `trim` needs the inverse (skip bytes that *are* in the set, like `strspn`), so give it a negate flag or a sibling `skip_any`.

The range copy already exists: `substr(s, start, count)` is one `memcpy`. The std loops just don't use it; `starts_with`/`ends_with` need no new builtin at all, since `substr(s, pos, len(x)) == x` is one copy plus one compare. `upper`/`lower` also allocate a string per byte (`add(chr(c))`) but need a byte-mapping builtin rather than either of these.

### String interning for short strings (P1)
Small strings (< 16 bytes) could be interned in a global table, turning equality checks into pointer comparison. Most table keys are short identifier strings — this would speed up every `gem_table_get`/`gem_table_set` with string keys. Trade-off: interned strings must live in a shared arena or be reference-counted so they outlive individual process arenas. Would also reduce per-process allocation rate — repeated key lookups like `"tag"`, `"pid"`, `"url"` currently allocate a fresh string each time via `gem_string()`.

### `gem_string()` copies unconditionally (P2)
`gem_string(const char *s)` always allocates + memcpy. Callers that already have an arena-allocated string (e.g. `buf_str`) pay for a redundant copy. A `gem_string_own(char *s)` variant that takes ownership would eliminate this.

## Codegen Output

### Dead code elimination (P2)
Unreachable code after `return`, `break`, `error()` could be stripped. Currently emitted as-is.

## Runtime Hot Paths

### `gem_eq` for strings (P2)
Currently `strcmp`. If string interning lands, short strings become pointer equality. Even without interning, caching string length would let us short-circuit on length mismatch before comparing bytes.

### `gem_add` for strings (P1)
Every string `+` does `strlen` on both operands. If strings carried their length, this becomes a field read. Depends on string views/length-aware representation. Directly impacts the HTML response building hot path.

### `buf_push` specialization for non-strings (P2)
`buf_push` auto-coerces non-string values via `to_string`, allocating a temporary string. Specialized variants (`buf_push_int`, `buf_push_float`) that write directly into the buffer would skip the allocation. Small win per call but high frequency in formatting-heavy code.

### Constructor return-by-value (P1)
`gem_int()`, `gem_float()`, `gem_bool()`, `gem_string()` all return `GemVal` by value (16 bytes). With NaN boxing these become trivial bit operations returning 8 bytes. Without NaN boxing, the compiler could use static inline or macros for the trivial constructors. Blocked on NaN boxing for the full win.

### Integer-key append in `gem_table_set` scans every key (P1)
`gem_table_set(t, int k, v)` with `k == len(t)` (append by index) falls through to the linear "find existing key" scan before appending, so building an array by index — and the `keys` builtin, which builds its result that way — is O(n²): `keys` of a 20000-entry table takes ~1.8 s. Fix: an append fast path when every key so far is array-shaped (track a flag on the table, cleared by any non-array key), or have `keys`/`values` push directly.

### Table grow strategy (P2)
`gem_table_grow` doubles capacity. Could use a growth factor of 1.5 to reduce memory waste, or start with capacity 0 (no allocation) for tables that might stay empty.

## Runtime I/O

### Selective receive save-queue optimization (P2)
`receive ... when` scans the mailbox from oldest to newest on every wake. If a process accumulates many messages and the match is near the end, that's O(n) pattern matches per wake. Erlang's optimization: remember which messages were already tested against the current receive and skip them on re-scan, only testing newly arrived messages. Non-trivial but maps onto the existing mailbox structure — a "scan cursor" per process that advances as messages are rejected and resets when the receive shape changes or a new message arrives.

### kqueue/epoll for sockets (P2)
The scheduler currently uses `poll()` for socket readiness. Replacing with **kqueue** (macOS/BSD) or **epoll** (Linux) would improve scalability at high connection counts (thousands of fds). `poll()` scans the entire fd set on each call — O(n) per wake. kqueue/epoll return only ready fds — O(ready). For the current HTTP server benchmark (~100 concurrent connections), `poll()` is not the bottleneck; this optimization matters when scaling to thousands of simultaneous connections.

## Scheduler / Concurrency

### Multi-threaded work-stealing scheduler (P2)
The scheduler is single-threaded — one scheduler loop round-robining coroutines on one OS thread. N scheduler threads with per-thread run queues and work-stealing (Chase-Lev deque) would scale throughput ~linearly with cores. The per-process arena model already eliminates shared-heap contention. Hard parts: mailboxes need lock-free MPSC queues for cross-thread sends, shared globals (`gem_proc_table`, `gem_name_registry`, free list) need synchronization, each thread needs its own kqueue/epoll set, and process migration (stealing a coroutine between scheduler ticks) needs care. Erlang/BEAM does exactly this architecture. Nothing in the current design blocks it — isolated processes, message passing, and per-process memory are the right foundation.

## C Interop Hardening

### String-return ownership convention is path-dependent (P2)
`extern blocking fn` String returns are documented (SPEC §C Interop) to be `malloc`/`strdup`'d — the runtime copies into the arena and `free`s the original. `extern fn` (non-blocking) String returns are *not* freed: the runtime `gem_string`s the pointer (which copies) but the original is leaked if it was malloc'd, or fine if it was a static literal. Two reasonable behaviors with opposite ownership rules, documented in SPEC ("String-return ownership"). Options for removing the asymmetry: (a) unify on the blocking convention (always free), which is the most consistent but breaks the obvious `getenv`/`strerror`-style use case; (b) introduce a `StringStatic` / `StringOwned` distinction.

## std/json

### Fast path for escape-free strings in parse (P2)
`parse_string` always allocates a buffer and pushes byte-by-byte. Most JSON strings contain no escapes. A fast path that scans for the closing `"` first (checking for `\` along the way) and uses `substr` when no escapes are found would avoid the buffer allocation entirely. 2-3x speedup on string-heavy JSON.

### Scanner as plain table instead of closure (P2)
The closure-based scanner (`{peek, advance, skip_ws}`) pays for hashmap lookup + closure call + captured variable access on every character. A flat table `{input, pos, length}` with module-level functions `peek(s)`, `advance(s)`, `skip_ws(s)` avoids closure overhead. More idiomatic for a language without methods.

## Known DX warts of the rescue+reset mechanism (P2)

The arena reset mechanism is invisible to user code by design — `while true` Just Works and resets at the back-edge once the threshold trips. This list captures the DX warts the mechanism has. None forces users to write code differently; all are observable by users in some form (jitter, throughput, mystery RSS) but not explainable from the source alone.

1. **Latency cliff at threshold crossings.** With a 1 MB threshold, most iterations of a tight loop pay only the gate check; every Nth iteration pays a full sweep+rescue (proportional to live-set size). Visible as p99 jitter on hot HTTP loops — see `OPTIMIZATIONS_LOG.md` for headline numbers. Post-rescue p99 is ~6–20× better than the unbounded-RSS baseline, but the floor isn't flat. Mitigation idea: adaptive threshold based on observed allocation rate, or a hint mechanism per loop. Both edge into "language tax" territory (CLAUDE.md), so probably never worth shipping unless a real workload demands it. Document, don't fix.

2. **Rescue set is invisible.** A user who keeps a 10 MB value live across the back-edge pays for copying it at resets (amortized by the hysteresis, but visible as memory and jitter), with no way to see which loop it is. `GEM_DIAG=1` prints whole-program reset totals (count, bytes copied/scanned/freed, time) at exit; there is still no per-loop accounting. Next step: a `GEM_DEBUG_RESETS=1` that logs `[reset pid=N at line X: rescued K bytes, took T µs]`.

3. **Stdlib comments must not leak the mechanism.** A stdlib reader (or a user reading stdlib for examples) shouldn't have to know about `GEM_ARENA_RESET_THRESHOLD`, "back-edge", "PT tagging", or "rescue+reset". Comments should describe what a function does at the API level. Caught and removed two such comments in `std/http.gem` (commit `91bb6be`): `accept_loop`'s stale "depth 2 from process entry" fence, and `handle_connection_loop`'s `GEM_ARENA_RESET_THRESHOLD` reference. Future stdlib additions (and CLAUDE.md guidance) should keep this discipline. Not really an optimization — call it a documentation invariant.
