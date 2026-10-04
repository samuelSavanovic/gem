# Optimization TODOs

Future performance improvements. None are blocking — collect ideas here as they come up. Shipped optimizations live in [`OPTIMIZATIONS_LOG.md`](OPTIMIZATIONS_LOG.md) with their work logs and benchmark anchors.

Priorities are informed by benchmark results from the bookmark CRUD app.

Priority scale: **P0** = measurable impact on benchmark right now, **P1** = significant but requires groundwork, **P2** = nice to have or niche.

## Current performance reference

Single-threaded scheduler ceiling on M1 Pro was ~26–29k req/s on the bookmark app's `/` (static HTML, April 2026, before the std/http hardening) regardless of c=4/100/500 — higher concurrency just queues. The hardened std/http is back at its pre-hardening throughput in its own load test on Linux x86_64 (`OPTIMIZATIONS_LOG.md`, "std/http"); the bookmark-app figure has not been re-measured since. Full benchmark history is in `OPTIMIZATIONS_LOG.md`.

Text processing in one process is the weak spot: `examples/logstat` summarizes 1M access-log lines (123 MB) in 26 s from stdin and 35 s from a file, against 4 s for the same program in Python (`benchmarks/logstat/run.sh`, Linux x86_64, October 2026). Most of it is std/string searching in Gem ("Search and scan builtins"); 4–7 s is resets re-copying the aggregate tables ("Survivors of a reset are copied again"). Memory stays flat on stdin (the arena resets keep up); a file is held whole, twice at the peak.

Key bottlenecks under the current arena + region-reset mechanism:
- Per-process arena allocation eliminates GC pauses; every loop resets the region it allocated once it passes max(1 MB, 2 × the last reset's cost), so memory is bounded at roughly 3× a loop's live data plus whatever was allocated before the loop started.
- String concatenation patterns that escape `build_string` still allocate per-concat.
- Every loop entry takes a region mark and every back-edge checks the trigger (a load and a compare); the reset itself is amortized O(1) per allocated byte (see `OPTIMIZATIONS_LOG.md` "Region resets").

## Arena / Memory

### Survivors of a reset are copied again by every later reset (P1)
A reset copies what is live in its loop's region into fresh blocks, and those blocks belong to the region too, so the next reset copies the same survivors again. Data that a long loop keeps, such as the table it aggregates into, is copied once per reset for the rest of the loop: its cost is `live size × resets`, bounded only by the hysteresis (the next reset waits for 2 × the last one's work). In `examples/logstat` on 1M lines (`benchmarks/logstat/run.sh`, GEM_DIAG=1), grouping by IP (990 groups) copies 1.3 GB in 3,471 resets, 4.4 s of a 25.5 s run, and grouping by path (5,010 groups) 1.7 GB, 6.7 s; grouping by hour (24 groups) copies 9 MB, 0.7 s. Python's whole run takes 4 s.

Fix: promote survivors. After a reset, treat the blocks it copied into as older than the mark (move the mark past them), so later resets of the same loop leave them alone, as a generational collector's old space. Soundness already holds for older objects: tables and buffers are tracked by the write barrier and the buffer walk, pinned boxes by their pin `seq`. Garbage among promoted objects (a group removed later) is then kept until an enclosing loop resets or the process exits, so a loop whose live set churns would grow; promoting only after an object survived two resets, or capping promotion at a fraction of the region, bounds that.

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
Integer keys currently fall through to a linear scan if they don't match the array-style fast path (sequential 0..n). A dedicated int hash map (parallel to `str_index`) would give O(1) lookup for sparse integer keys. Sparse int sets are common (ids from a database): 20,000 of them (`seen[id] = true`, then `has_key`) take 3.1 s against 30 ms as string keys, and `table.group_by` with 20,000 int groups about 4.9 s; BEST_PRACTICES tells users to key such tables by string.

## Strings

### String views / slices (P1)
`substr` allocates a copy. A view (pointer + offset + length) into the original string would make substring extraction O(1). Strings already carry their length (`slen`), but every consumer that relies on the trailing NUL (C interop, `printf %s`) would need to copy or check views first. Defer until profiling shows substring allocation as a real bottleneck — the C interop boundary assumes null-terminated strings throughout, and `substr`/`ord(s, i)` already cover the hot cases without changing the representation.

### Search and scan builtins so std string loops run in C (P1)
A byte loop written in Gem (`ord(s, i)` per byte, plus a reduction check and a reset check at every back-edge) costs about 50 ns a byte, against about 1 ns for `memchr`/`memmem`, and std leans on such loops. `std/string` now works around the missing search builtin: its private `find` scans the first 32 positions in Gem, then tests growing chunks (64 bytes doubling to 64 KB) for a match in C with `len(str_replace(substr(s, i, n), needle, "")) != n` (a miss returns the input without copying), and bisects the matching chunk the same way down to a 32-byte Gem scan. `split` scans in Gem while delimiters are close together and hands longer gaps to `find`. Measured on Linux x86_64 (1 MB strings; before → after the workaround):

| Call | Gem byte loop | chunked `str_replace` |
|---|---|---|
| `string.index_of(s, "needle")`, match at byte 1,000,000 | 185 ms | 4 ms |
| `string.contains(s, "zz")`, no match | 150 ms | 4 ms |
| `string.split(s, ";")`, no match | 210 ms | 4 ms |
| `string.split(s, ",")`, 100,000 ten-byte fields | 245 ms | 140 ms |
| `url.decode(s)`, no `%` | 175 ms | 7 ms |

The dense `split` stays slow: the Gem scan alone is about 55 ms and the 100,000 `substr` + `push` about 45 ms; a C `find` would leave only the latter. The workaround also copies each chunk it tests, and does about 2× the C work of one `memmem` pass. Two general builtins would retire it:

- `find(s, needle, start)` — `memmem`-backed; index of the first match at or after `start`, or -1. `string.index_of`/`contains` become one call, `split` becomes `find` plus one `substr` per piece. std/http already uses a runtime extern helper of this shape, `gem_bytes_find` (and `gem_bytes_span` for spans over a byte set), reached through `extern fn` because there is no builtin; std/string's `find` could switch to it today, and a public builtin would retire both helpers.
- `find_any(s, chars, start)` — index of the first byte at or after `start` that is in the set `chars`, or -1. `html_escape`, `url.encode` (1 MB with a reserved byte every 10: 215 ms), `url.parse_query` and tokenizers like the `std/json` scanner scan to the next special byte, then copy the whole run before it. `trim` needs the inverse (skip bytes that *are* in the set, like `strspn`), so give it a negate flag or a sibling `skip_any`.

Short strings are the other half of the problem: every search first scans up to 32 positions in Gem, and a call costs about 2 µs even when the match is a few bytes away. `examples/logstat` (a log analyzer, `benchmarks/logstat/run.sh`) parses each 120-byte access-log line with eight `index_of` calls and one three-field `split`. On 1M lines from stdin (Linux x86_64): reading the lines with `input()` takes 0.26 s, the eight `index_of` calls 10.9 s, the `split` 3.5 s, the whole parse 21.5 s, and the full run 25.5 s, against 4.1 s for the same program in Python. A `memchr`-backed `find` would take the per-call cost to roughly that of a builtin call.

`upper`/`lower` copy unchanged runs with `substr` but still allocate a string per changed byte (`add(chr(c))`): `string.upper` of 1 MB of mostly lowercase text takes 160 ms, `lower` of the same text 80 ms. They need a byte-mapping builtin rather than either of these.

### String interning for short strings (P1)
Small strings (< 16 bytes) could be interned in a global table, turning equality checks into pointer comparison. Most table keys are short identifier strings — this would speed up every `gem_table_get`/`gem_table_set` with string keys. Trade-off: interned strings must live in a shared arena or be reference-counted so they outlive individual process arenas. Would also reduce per-process allocation rate — repeated key lookups like `"tag"`, `"pid"`, `"url"` currently allocate a fresh string each time via `gem_string()`.

### `gem_string()` copies unconditionally (P2)
`gem_string(const char *s)` always allocates + memcpy. Callers that already have an arena-allocated string (e.g. `buf_str`) pay for a redundant copy. A `gem_string_own(char *s)` variant that takes ownership would eliminate this.

## Codegen Output

### Don't pin plain local reads for left-to-right evaluation (P2)
Left-to-right evaluation (`left_to_right` / `operands` / `pin_operand` in codegen.gem) copies every operand that isn't a literal or a temp into a C temp when a later operand runs code, plain C locals (`gem_v_s`, `gem_v__for_i_N`) included. On a 50M-iteration `s = s + t[i % 10] * 2 + len(t) - (i % 7)` loop that is +9–10% over the old (wrong-order) code on macOS arm64 (1.78 s vs 1.62 s); removing just the local pins gets back about a third of it. A plain unboxed local can only change mid-expression through a later operand that assigns it, which after "assignment is a statement" means a non-escaping closure assigning it through its stack env (`g(x, pcall x = 2)` must still read the old `x`). So: treat `gem_v_<name>` as inert unless a later operand of the same expression assigns `name` (directly, or in a non-escaping closure's `stack_env_writes`). Boxed locals and module slots stay pinned.

### Dead code elimination (P2)
Unreachable code after `return`, `break`, `error()` could be stripped. Currently emitted as-is.

## Runtime Hot Paths

### Hash string table keys faster (P2)

The string-key index (`gem_str_index_*` in runtime/gem_core.c) hashes the
key's `slen` bytes with byte-wise FNV-1a on every lookup. Measured on
macOS arm64 with 200k keys: 1M lookups take 95 ms against 84 ms with the
old stb_ds index (about +13%), while insert/delete churn got faster.
Cache the hash on the string (strings are immutable), or use a
word-at-a-time hash.

### `gem_eq` for strings (P2)
Currently `strcmp`. If string interning lands, short strings become pointer equality. Even without interning, caching string length would let us short-circuit on length mismatch before comparing bytes.

### `type(v) == "string"` allocates two strings (P2)
`gem_type_fn` returns a fresh `gem_string("string")` and the literal on the right is allocated again, so the comparison costs about 130 ns (1M iterations: 131 ms, against 40 ms for 1M calls of an empty fn). std's argument checks (`string.<fn>`, `url.<fn>`, `mime.<fn>` raising on a non-string) pay it once per argument. Returning static strings from `type`, or having codegen turn `type(x) == "<literal>"` into a tag compare, would make such checks nearly free.

### `gem_add` for strings (P1)
Every string `+` does `strlen` on both operands. If strings carried their length, this becomes a field read. Depends on string views/length-aware representation. Directly impacts the HTML response building hot path.

### `buf_push` specialization for non-strings (P2)
`buf_push` auto-coerces non-string values via `to_string`, allocating a temporary string. Specialized variants (`buf_push_int`, `buf_push_float`) that write directly into the buffer would skip the allocation. Small win per call but high frequency in formatting-heavy code.

### Constructor return-by-value (P1)
`gem_int()`, `gem_float()`, `gem_bool()`, `gem_string()` all return `GemVal` by value (16 bytes). With NaN boxing these become trivial bit operations returning 8 bytes. Without NaN boxing, the compiler could use static inline or macros for the trivial constructors. Blocked on NaN boxing for the full win.

### Integer-key append in `gem_table_set` scans every key (P1)
`gem_table_set(t, int k, v)` with `k == len(t)` (append by index) falls through to the linear "find existing key" scan before appending, so building an array by index — and the `keys` and `values` builtins (`gem_keys`/`gem_values` in runtime/gem_builtins_collection.c), which build their result that way, and the rows of a `sqlite_query` result (10,000 rows 0.5 s, 40,000 rows 6.4 s, all inline on the scheduler thread) — is O(n²): `keys` of a 10,000-entry table takes 0.4 s, of 40,000 entries 6.4 s (`values` the same; `for k, v in` over the same table: 4 ms). std/test's deep equality stopped calling `keys` because of it. Fix: an append fast path when every key so far is array-shaped (track a flag on the table, cleared by any non-array key), or have `keys`/`values` push directly.

### Table grow strategy (P2)
`gem_table_grow` doubles capacity. Could use a growth factor of 1.5 to reduce memory waste, or start with capacity 0 (no allocation) for tables that might stay empty.

## Runtime I/O

### `read_file` holds the file twice at its peak (P2)
The I/O worker reads the file into a malloc'd buffer, and `gem_read_file_fn` (runtime/gem_builtins_io.c) then copies it into the arena: a 123 MB log peaks at 247 MB RSS. Allocating the arena block first and having the worker read into it, or adopting the malloc'd buffer as a large arena block, would halve it. Iterating the result line by line is also slower than `input()`: logstat takes 35 s on a 1M-line file against 26 s for the same lines on stdin, the difference being one `string.index_of(data, "\n", start)` per line on the 123 MB string (see "Search and scan builtins"). A line reader (ROADMAP "Line-at-a-time input") would remove both costs for this use.

### Selective receive save-queue optimization (P2)
`receive ... when` scans the mailbox from oldest to newest on every wake. If a process accumulates many messages and the match is near the end, that's O(n) pattern matches per wake. Erlang's optimization: remember which messages were already tested against the current receive and skip them on re-scan, only testing newly arrived messages. Non-trivial but maps onto the existing mailbox structure — a "scan cursor" per process that advances as messages are rejected and resets when the receive shape changes or a new message arrives.

### kqueue/epoll for sockets (P2)
The scheduler currently uses `poll()` for socket readiness. Replacing with **kqueue** (macOS/BSD) or **epoll** (Linux) would improve scalability at high connection counts (thousands of fds). `poll()` scans the entire fd set on each call — O(n) per wake. kqueue/epoll return only ready fds — O(ready). For the current HTTP server benchmark (~100 concurrent connections), `poll()` is not the bottleneck; this optimization matters when scaling to thousands of simultaneous connections.

## Scheduler / Concurrency

### Multi-threaded work-stealing scheduler (P2)
The scheduler is single-threaded — one scheduler loop round-robining coroutines on one OS thread. N scheduler threads with per-thread run queues and work-stealing (Chase-Lev deque) would scale throughput ~linearly with cores. The per-process arena model already eliminates shared-heap contention. Hard parts: mailboxes need lock-free MPSC queues for cross-thread sends, shared globals (`gem_proc_table`, `gem_name_registry`, free list) need synchronization, each thread needs its own kqueue/epoll set, and process migration (stealing a coroutine between scheduler ticks) needs care. Erlang/BEAM does exactly this architecture. Nothing in the current design blocks it — isolated processes, message passing, and per-process memory are the right foundation.

### Supervisor restart bookkeeping is O(restarts in window) per restart (P2)
`std/supervisor` (`note_restart`) and `std/dynamic_supervisor` (`check_intensity`) rebuild `state.restart_times` on every restart, keeping the times inside `max_seconds` (at most `max_restarts + 1` of them, since one more crashes the supervisor). A restart therefore costs O(restarts in the window): 8,000 restarts of a permanent child took 394 ms with a 1 ms window and 7.2 s with a 100 s window and a huge `max_restarts`. The times are pushed in order, so a queue that drops expired entries from the front (a head index into the array, compacted now and then) makes each restart amortized O(1). Only matters for supervisors configured to tolerate thousands of restarts per window.

## C Interop Hardening

### String-return ownership convention is path-dependent (P2)
`extern blocking fn` String returns are documented (SPEC §C Interop) to be `malloc`/`strdup`'d — the runtime copies into the arena and `free`s the original. `extern fn` (non-blocking) String returns are *not* freed: the runtime `gem_string`s the pointer (which copies) but the original is leaked if it was malloc'd, or fine if it was a static literal. Two reasonable behaviors with opposite ownership rules, documented in SPEC ("String-return ownership"). Options for removing the asymmetry: (a) unify on the blocking convention (always free), which is the most consistent but breaks the obvious `getenv`/`strerror`-style use case; (b) introduce a `StringStatic` / `StringOwned` distinction.

## std/json

### Scanner as plain table instead of closure (P2)
The closure-based scanner (`{peek, advance, skip_ws}`) pays for hashmap lookup + closure call + captured variable access on every character. A flat table `{input, pos, length}` with module-level functions `peek(s)`, `advance(s)`, `skip_ws(s)` avoids closure overhead. More idiomatic for a language without methods.

## Known DX warts of the rescue+reset mechanism (P2)

The arena reset mechanism is invisible to user code by design — `while true` Just Works and resets at the back-edge once the threshold trips. This list captures the DX warts the mechanism has. None forces users to write code differently; all are observable by users in some form (jitter, throughput, mystery RSS) but not explainable from the source alone.

1. **Latency cliff at threshold crossings.** With a 1 MB threshold, most iterations of a tight loop pay only the gate check; every Nth iteration pays a full sweep+rescue (proportional to live-set size). Visible as p99 jitter on hot HTTP loops — see `OPTIMIZATIONS_LOG.md` for headline numbers. Post-rescue p99 is ~6–20× better than the unbounded-RSS baseline, but the floor isn't flat. Mitigation idea: adaptive threshold based on observed allocation rate, or a hint mechanism per loop. Both edge into "language tax" territory (CLAUDE.md), so probably never worth shipping unless a real workload demands it. Document, don't fix.

2. **Rescue set is invisible.** A user who keeps a 10 MB value live across the back-edge pays for copying it at resets (amortized by the hysteresis, but visible as memory and jitter), with no way to see which loop it is. `GEM_DIAG=1` prints whole-program reset totals (count, bytes copied/scanned/freed, time) at exit; there is still no per-loop accounting. Next step: a `GEM_DEBUG_RESETS=1` that logs `[reset pid=N at line X: rescued K bytes, took T µs]`.

3. **Stdlib comments must not leak the mechanism.** A stdlib reader (or a user reading stdlib for examples) shouldn't have to know about `GEM_ARENA_RESET_THRESHOLD`, "back-edge", "PT tagging", or "rescue+reset". Comments should describe what a function does at the API level. Caught and removed two such comments in `std/http.gem` (commit `91bb6be`): `accept_loop`'s stale "depth 2 from process entry" fence, and `handle_connection_loop`'s `GEM_ARENA_RESET_THRESHOLD` reference. Future stdlib additions (and CLAUDE.md guidance) should keep this discipline. Not really an optimization — call it a documentation invariant.
