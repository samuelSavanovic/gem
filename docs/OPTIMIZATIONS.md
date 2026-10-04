# Optimization TODOs

Future performance improvements. None are blocking — collect ideas here as they come up. Shipped optimizations live in [`OPTIMIZATIONS_LOG.md`](OPTIMIZATIONS_LOG.md) with their work logs and benchmark anchors.

Priorities are informed by benchmark results from the bookmark CRUD app.

Priority scale: **P0** = measurable impact on benchmark right now, **P1** = significant but requires groundwork, **P2** = nice to have or niche.

## Current performance reference

Single-threaded scheduler ceiling on M1 Pro was ~26–29k req/s on the bookmark app's `/` (static HTML, April 2026, before the std/http hardening) regardless of c=4/100/500 — higher concurrency just queues. The hardened std/http is back at its pre-hardening throughput in its own load test on Linux x86_64 (`OPTIMIZATIONS_LOG.md`, "std/http"); the bookmark-app figure has not been re-measured since. Full benchmark history is in `OPTIMIZATIONS_LOG.md`.

Text processing in one process is the weak spot: `examples/logstat` summarizes 1M access-log lines (123 MB) in 1.9–2.4 s, from stdin or from a file, against 2.0 s for the same program in Python (`benchmarks/logstat/run.sh`, macOS arm64, October 2026; `--by hour` is the fast end). Part of it is resets re-copying the aggregate tables ("Survivors of a reset are copied again"); in a `sample` profile (`--by ip`, from a file) the rest is spread over string-key hashing in table get/set ("Hash string table keys faster"), `substr` copies, std/string's argument checks and the per-call overhead of Gem code. Memory stays flat on stdin (the arena resets keep up); a file is held whole, twice at the peak.

The CPU-bound core runs at about CPython's speed: `examples/lox`, a tree-walking interpreter, takes 0.66–1.15 times the time of the same interpreter in Python on its six bench programs (`benchmarks/lox/run.sh`, Linux x86_64 VM, October 2026). In callgrind profiles, 40–45% of its instructions are string-key table lookups, nearly all of them inline-cache misses ("Inline caches key on the table, not its shape"), and 20–30% of its time is system time, faulting in fresh arena pages ("Resets unmap the blocks they free"). Recursion that allocates holds all of it until a loop moves on: Lox `fib(28)` peaks at 1.9 GB ("A recursion frees nothing until a loop moves on").

Key bottlenecks under the current arena + region-reset mechanism:
- Per-process arena allocation eliminates GC pauses; every loop resets the region it allocated once it passes max(1 MB, 2 × the last reset's cost), so memory is bounded at roughly 3× a loop's live data plus whatever was allocated before the loop started.
- String concatenation patterns that escape `build_string` still allocate per-concat.
- Every loop entry takes a region mark and every back-edge checks the trigger (a load and a compare); the reset itself is amortized O(1) per allocated byte (see `OPTIMIZATIONS_LOG.md` "Region resets").

## Arena / Memory

### Survivors of a reset are copied again by every later reset (P1)
A reset copies what is live in its loop's region into fresh blocks, and those blocks belong to the region too, so the next reset copies the same survivors again. Data that a long loop keeps, such as the table it aggregates into, is copied once per reset for the rest of the loop: its cost is `live size × resets`, bounded only by the hysteresis (the next reset waits for 2 × the last one's work). In `examples/logstat` on 1M lines from stdin (`benchmarks/logstat/run.sh`, GEM_DIAG=1, macOS arm64), grouping by IP (990 groups) copies 1.3 GB in 3,511 resets, 0.87 s of a 4.7 s run, and grouping by path (5,010 groups) 1.7 GB, 1.06 s of 5.0 s; grouping by hour (24 groups) copies 9 MB, 0.11 s. Most of it is the remembered-log walk (`walk=` in the diag line): the group table is older than the loop's mark, so every reset finds it written and copies the group records it holds again. Python's whole run takes 2.0 s.

`examples/mini_redis` shows it in a long-lived server: the store is a gen_server whose loop started with the program, so the whole keyspace sits in the loop's region and every reset copies it again. With a 1M-element list in the store, 20,000 `LRANGE mylist 0 599` from 50 clients spend 5.1 s in 1,746 resets (1.67 GB copied, 4.0 s of it the walk) of a 9.4 s run; with a 1,000-element list the same load spends 0.58 s (Linux x86_64 VM, GEM_DIAG=1). The same server under `benchmarks/mini_redis/run.sh`'s pipelined phase spent 24.7 s in resets, copying 7 GB, as its lists grew.

`examples/lox`'s `binary_trees.lox 12` shows it in a CPU-bound program: the benchmark keeps one tree of 8,191 instances (two tables each) alive while it builds and drops 670,000 short-lived ones, and 5,125 resets copy 1.6 GB, 1.9 s (1.06 s of it the walk) of a 7.6–7.9 s run (GEM_DIAG=1, Linux x86_64 VM). Python's run takes 11.5 s.

Fix: promote survivors. After a reset, treat the blocks it copied into as older than the mark (move the mark past them), so later resets of the same loop leave them alone, as a generational collector's old space. Soundness already holds for older objects: tables and buffers are tracked by the write barrier and the buffer walk, pinned boxes by their pin `seq`. Garbage among promoted objects (a group removed later) is then kept until an enclosing loop resets or the process exits, so a loop whose live set churns would grow; promoting only after an object survived two resets, or capping promotion at a fraction of the region, bounds that.

### A loop keeps its high-water memory after its live data shrinks (P2)
The next reset of a loop waits until it has allocated 2 × the last reset's work, so a loop that held a lot and then dropped it keeps that memory until it allocates as much again, and an idle one keeps it for good. `examples/mini_redis` with 1M keys set to expire in 2 s: 387 MB RSS after the fill, 384 MB ten seconds later with every key gone and the server idle (Redis: 17 MB). Under GET load on the now empty keyspace, RSS swung between 200 and 475 MB over the next three 200,000-request runs. Options: let the budget decay with time, or have the scheduler run a pending reset when a process with a large region blocks in `receive` (the cost is then off the request path).

### A recursion frees nothing until a loop moves on (P1)
Memory is reclaimed only at loop back-edges, so what a recursive computation allocates stays until a loop that was running when it started finishes its iteration: memory grows with the number of calls, not the depth. A tree walker hits this at once. `examples/lox` interpreting a naive `fib(28)` (1.03M Lox calls, each a new environment, an argument array and a return record) peaks at 1.9 GB, and `fib(30)` (2.7M calls) at 4.9 GB, where Python stays at 11 MB (GEM_DIAG=1: 2 resets in the whole run; Linux x86_64 VM). The `fib(30)` run took 10–43 s depending on how the VM coped with the memory, against 7.4 s for Python. A plain Gem recursion making one small table per call reaches 530 MB at a million calls; with the two recursive calls made from a `for` loop it stays at 10 MB (BEST_PRACTICES.md, "Recursion keeps what it allocates").

Fix: a region boundary at function return. Take a mark at entry (cheap: the arena's current position and clock), and at return, when the frame's region has passed the reset threshold, run the same reset with the return value as the only root; the write barrier, pinned boxes and buffers already cover everything older that can point into the region. Only functions that can recurse need it (the call graph knows), and the hysteresis keeps it amortized as for loops. The interpreter's `call_function` would then return with its callees' garbage freed.

### Resets unmap the blocks they free, and the next allocations fault fresh pages (P1)
A region reset `munmap`s the region's blocks (`gem_arena_free_blocks`), and the loop's next iterations `mmap` new ones, so every byte a loop allocates is a fresh zero page again: one page fault per 4 KB. In `examples/lox` the Gem runs spend 20–30% of their time in the kernel: `closures.lox 100000` frees 2 GB in 1,388 resets and takes 525,000 minor faults, 1.0 s of system time in a 4.5 s run (Python: 99,000 faults, 0.4 s); `binary_trees.lox 12` frees 7.4 GB, 1.1M faults, 1.6 s of 7.3 s; `fib.lox 28` 0.9 s of 3.1 s. Fix: keep the blocks a reset frees on a per-arena free list, up to about the loop's reset budget, and hand them out before mapping new ones; unmap the rest (and the list when the process exits or the loop ends), so memory still comes back when a loop's live data shrinks.

### Garbage allocated before a loop starts is kept by that loop (P2)
A region reset frees only what the loop allocated since its mark. Garbage from straight-line code before the loop (e.g. main's startup work before a top-level `while true` server loop) stays until an enclosing loop resets or the process exits. It is a constant, not growth. For loops at depth 0 of the main program the mark could sit at the start of the arena (main has no caller frames and its module slots are roots), reclaiming startup garbage too; spawned bodies would need their env rooted.

### Remembered log instead of a per-table walk for buffers (P2)
Region resets find old tables written since the mark through a write barrier and a remembered log, but still walk every buffer older than the mark (cost charged to the hysteresis budget). A barrier in `buf_push`/`gem_string_append_to` growth would make the walk proportional to the buffers actually grown.

### Mailbox messages are copied by each reset that finds them in the region (P2)
Messages are deep-copied into the receiver's arena by the sender, so a backlog that arrives during a loop is part of that loop's region and is copied at each reset until it is consumed. Hysteresis keeps the total linear (300 × 100 KB messages drain in linear time), but a large backlog doubles its memory while a reset runs. Allocating message bodies in a separate per-process message arena that a reset never scans (freed when the message is received and dropped) would avoid both; the cost is a second allocator and a copy (or ownership transfer) when the message is received.

### Investigate post-idle high-water at high concurrency (P2)
Originally reported at 2.39 GB stuck post-c=500 with the 16 MB threshold. After lowering the default threshold to 1 MB, post-idle RSS at c=500 dropped to 174 MB — flat across +0/+30/+90s probes, so it's still not draining, but the absolute waste is now an order of magnitude smaller and unlikely to matter for typical workloads. The underlying mechanism (per-process arenas of completed connection handlers not fully releasing — likely `madvise(DONTNEED)` happens but `munmap` doesn't, or proc-table objects linger until late cleanup) is unchanged. Keep tracking but don't prioritize until a workload demonstrates the residual is a real problem. If revisited, trace `gem_proc_exit` against actual mmap accounting under load.

## Value Representation

### NaN boxing (P1)
GemVal is currently 16 bytes (4-byte type enum + 8-byte union + padding). NaN boxing packs type + value into a single 8-byte double by exploiting the NaN payload space. Halves memory per value, improves cache locality, eliminates the type field branch in hot paths. The payoff isn't just memory — every function call, table lookup, and arithmetic op passes GemVals, so halving their size compounds across the entire runtime. Requires rewriting every GemVal constructor and accessor. Major undertaking but large payoff. Would also directly reduce arena allocation pressure since every value, table entry, and function argument shrinks.

### Avoid hashing integers in tables (P2)
An int key k is found at once when entry k holds it; otherwise a table that is not an array (`is_array` in runtime/gem.h: entry i has key i) is searched linearly. On an array, `t[len(t)] = v`, `push` and a lookup past the end are O(1). A dedicated int hash map (parallel to `str_index`) would give O(1) lookup for sparse integer keys. Sparse int sets are common (ids from a database): 20,000 of them (`seen[id] = true`, then `has_key`) take 3.1 s against 30 ms as string keys, and `table.group_by` with 20,000 int groups about 4.9 s; BEST_PRACTICES tells users to key such tables by string.

## Strings

### String views / slices (P1)
`substr` allocates a copy. A view (pointer + offset + length) into the original string would make substring extraction O(1). Strings already carry their length (`slen`), but every consumer that relies on the trailing NUL (C interop, `printf %s`) would need to copy or check views first. Defer until profiling shows substring allocation as a real bottleneck — the C interop boundary assumes null-terminated strings throughout, and `substr`/`ord(s, i)` already cover the hot cases without changing the representation.

### `find_any` and a byte-mapping builtin, so the remaining std string loops run in C (P2)
A byte loop written in Gem (`ord(s, i)` per byte, plus a reduction check and a reset check at every back-edge) costs about 50 ns a byte, against about 1 ns in C. `find` covers searching for a fixed needle (OPTIMIZATIONS_LOG.md, "`find` builtin"); two kinds of scan still run in Gem:

- `find_any(s, chars, start)` — index of the first byte at or after `start` that is in the set `chars`, or -1. `html_escape`, `url.encode` (1 MB with a reserved byte every 10: 215 ms, Linux x86_64), `url.parse_query` and tokenizers like the `std/json` scanner scan to the next special byte, then copy the whole run before it. `trim` needs the inverse (skip bytes that *are* in the set, like `strspn`), so give it a negate flag or a sibling `skip_any`. std/http and std/request reach the span case today through the runtime extern `gem_bytes_span`; a builtin would retire it.

`upper`/`lower` copy unchanged runs with `substr` but still allocate a string per changed byte (`add(chr(c))`): `string.upper` of 1 MB of mostly lowercase text takes 160 ms, `lower` of the same text 80 ms. They need a byte-mapping builtin rather than either of these.

### String interning for short strings (P1)
Small strings (< 16 bytes) could be interned in a global table, turning equality checks into pointer comparison. Most table keys are short identifier strings — this would speed up every `gem_table_get`/`gem_table_set` with string keys. Trade-off: interned strings must live in a shared arena or be reference-counted so they outlive individual process arenas. Would also reduce per-process allocation rate for keys built at runtime (literal keys like `"tag"` are already static, `GEM_STR_LIT`).

### `gem_string()` copies unconditionally (P2)
`gem_string(const char *s)` always allocates + memcpy. Callers that already have an arena-allocated string pay for a redundant copy. A `gem_string_own(char *s)` variant that takes ownership would eliminate this.

## Codegen Output

### Don't pin plain local reads for left-to-right evaluation (P2)
Left-to-right evaluation (`left_to_right` / `operands` / `pin_operand` in codegen.gem) copies every operand that isn't a literal or a temp into a C temp when a later operand runs code, plain C locals (`gem_v_s`, `gem_v__for_i_N`) included. On a 50M-iteration `s = s + t[i % 10] * 2 + len(t) - (i % 7)` loop that is +9–10% over the old (wrong-order) code on macOS arm64 (1.78 s vs 1.62 s); removing just the local pins gets back about a third of it. A plain unboxed local can only change mid-expression through a later operand that assigns it, which after "assignment is a statement" means a non-escaping closure assigning it through its stack env (`g(x, pcall x = 2)` must still read the old `x`). So: treat `gem_v_<name>` as inert unless a later operand of the same expression assigns `name` (directly, or in a non-escaping closure's `stack_env_writes`). Boxed locals and module slots stay pinned.

### Large C frames: about 1 KB for a small function, 2 KB with a `match` (P2)
`gcc -O2 -fstack-usage` on `examples/lox` (October 2026): a three-line `check_numbers` takes 576 B of stack, `ancestor` (a `for` loop over `.enclosing`) 736 B, `evaluate` and `execute` (a 12- and a 9-arm `match` on a string) 1,792 and 2,000 B. Each call's arguments go in their own addressable `GemVal _tN[] = {...}` array (24 B per value; 16 of them in `execute`, besides 50 scalar temporaries), and a reset loop adds a `GemArenaMark`. A Lox call recurses through about ten such frames, so the 8 MB process stack holds 600–900 interpreted calls, and the interpreter caps them at 256. Options: one scratch argument array per function, sized for its largest call and reused (arguments are evaluated into temporaries first anyway); direct calls with C parameters for named fns of known arity, which would also skip the `argc` checks.

### `match` on string literals compares bytes arm by arm (P2)
`match e.kind when "literal" ... when "super"` compiles to a chain of `gem_val_eq` calls, each a length check and a `memcmp`, so the twelfth arm costs twelve comparisons. In a callgrind profile of `examples/lox` (`fib.lox 20`), `gem_val_eq` runs 46 times per interpreted call and takes 5% of the instructions, `memcmp` another 2%. Most of these strings are literals on both sides (`GEM_STR_LIT` values point at C string constants, which the C compiler merges), so a pointer-equality check before the `memcmp` would settle most of them; a `match` with only string-literal arms could also switch on the length and first byte, or hash the target once.

### Dead code elimination (P2)
Unreachable code after `return`, `break`, `error()` could be stripped. Currently emitted as-is.

## Runtime Hot Paths

### Inline caches key on the table, not its shape (P1)
Each `t.field` site has a one-slot cache (`GemICacheSlot`, `gem_table_get_cached` in gem.h) that hits only when the *same table* comes back with the same `shape_id`, and every table gets its own `shape_id` when it is created. Code that reads the same field of many tables, such as a tree walk over records (`node.kind`, `node.left`) or objects of one class, misses every time and hashes the key (`gem_table_get_ic_miss`, which also `strlen`s the literal key on each miss). In `examples/lox`, 810,881 of 821,119 cached field reads missed (99%) in `fib.lox 20`, and string-key lookups (`gem_str_index_get`/`find`/`put`, the miss path, `strlen`, `memcmp`) are 40–45% of the instructions of every bench program (callgrind, October 2026). The log entry that introduced the cache (OPTIMIZATIONS_LOG.md) expected one shape per site in AST walking, but shapes are per table. Fix: hidden classes. Give tables built with the same sequence of string keys the same shape (a transition tree from the empty shape, as V8 and LuaJIT's templates do: `{kind:, op:, left:, right:, line:}` literals from one site always share one), cache `(shape, index)` per site, and change the shape only when a key is added out of order or removed. The miss path should take the key length from codegen instead of `strlen`.

### Every string-keyed table `calloc`s its index, and resets `free` them one by one (P1)
The string-key index of a table (`gem_str_index_alloc` in runtime/gem_core.c) is `calloc`'d, outside the arena, as soon as the table gets a string key, and a region reset walks every table the region allocated to `free` the indexes of the dead ones (`post_tables` in `gem_region_reset_impl`). So a short-lived record costs a `malloc`/`free` pair, and a reset's cost grows with the garbage it drops, not only with what survives. In a callgrind profile of `examples/lox` (`closures.lox 5000`, where every call makes an environment record), `calloc`, `free` and `gem_str_index_free` take 9% of the instructions. Fix: no index for small tables (a record with up to 8 string keys is faster to scan, comparing lengths and then bytes), and arena-allocated indexes for larger ones, so dead indexes go with their blocks.

### Hash string table keys faster (P1)

The string-key index (`gem_str_index_*` in runtime/gem_core.c) hashes the
key's `slen` bytes with byte-wise FNV-1a on every lookup. Measured on
macOS arm64 with 200k keys: 1M lookups take 95 ms against 84 ms with the
old stb_ds index (about +13%), while insert/delete churn got faster.
Cache the hash on the string (strings are immutable), or use a
word-at-a-time hash. A literal key (`GEM_STR_LIT`) could carry a hash
computed by the compiler. `gem_str_index_get` and `gem_str_index_put`
are the two largest runtime functions in a `sample` profile of logstat
(`--by ip`, October 2026), and the two largest functions of any kind in
callgrind profiles of `examples/lox` (23–28% of the instructions, most
of them behind inline-cache misses; see the entry above).

### `buf_push` specialization for non-strings (P2)
`buf_push` auto-coerces non-string values via `to_string`, allocating a temporary string. Specialized variants (`buf_push_int`, `buf_push_float`) that write directly into the buffer would skip the allocation. Small win per call but high frequency in formatting-heavy code.

### Integer-key append in `gem_table_set` scans every key (P1)
`gem_table_set(t, int k, v)` with `k == len(t)` (append by index) falls through to the linear "find existing key" scan before appending, so building an array by index — and the `keys` and `values` builtins (`gem_keys`/`gem_values` in runtime/gem_builtins_collection.c), which build their result that way, and the rows of a `sqlite_query` result (10,000 rows 0.5 s, 40,000 rows 6.4 s, all inline on the scheduler thread) — is O(n²): `keys` of a 10,000-entry table takes 0.4 s, of 40,000 entries 6.4 s (`values` the same; `for k, v in` over the same table: 4 ms). std/test's deep equality stopped calling `keys` because of it. Fix: an append fast path when every key so far is array-shaped (track a flag on the table, cleared by any non-array key), or have `keys`/`values` push directly.

### Table grow strategy (P2)
`gem_table_grow` doubles capacity. Could use a growth factor of 1.5 to reduce memory waste, or start with capacity 0 (no allocation) for tables that might stay empty.

## Runtime I/O

### `read_file` holds the file twice at its peak (P2)
The I/O worker reads the file into a malloc'd buffer, and `gem_read_file_fn` (runtime/gem_builtins_io.c) then copies it into the arena: logstat on a 123 MB log peaks at 261–274 MB RSS (macOS arm64), against 20–33 MB reading the same lines from stdin. Allocating the arena block first and having the worker read into it, or adopting the malloc'd buffer as a large arena block, would halve it; a line reader (ROADMAP "Line-at-a-time input") would keep only the current line.

### Selective receive save-queue optimization (P2)
`receive ... when` scans the mailbox from oldest to newest on every wake. If a process accumulates many messages and the match is near the end, that's O(n) pattern matches per wake. Erlang's optimization: remember which messages were already tested against the current receive and skip them on re-scan, only testing newly arrived messages. Non-trivial but maps onto the existing mailbox structure — a "scan cursor" per process that advances as messages are rejected and resets when the receive shape changes or a new message arrives.

### kqueue/epoll for sockets (P2)
The scheduler currently uses `poll()` for socket readiness. Replacing with **kqueue** (macOS/BSD) or **epoll** (Linux) would improve scalability at high connection counts (thousands of fds). `poll()` scans the entire fd set on each call — O(n) per wake. kqueue/epoll return only ready fds — O(ready). For the current HTTP server benchmark (~100 concurrent connections), `poll()` is not the bottleneck; this optimization matters when scaling to thousands of simultaneous connections: the scheduler rebuilds and polls the whole fd-waiter set on every idle wake-up, and while processes run it spaces the polls by twice their cost (at most 1 ms apart), so with a few hundred waiters a busy process can lose up to a third of its time to polling, and socket wake-ups wait up to twice a poll's cost (300 idle sockets beside a CPU-bound process on macOS arm64: 2,000 socket round trips take 1.05 s and the process's work 0.74 s, against 0.64 s and 0.97 s when polling after every pass). kqueue/epoll would make the non-blocking check after a pass O(ready) and remove that trade-off.

## Scheduler / Concurrency

### Fan-out writes one message per syscall (P1)
`examples/mini_redis`'s pub/sub sends each published message to every subscriber's connection process, which writes it to its socket. Redis delivers messages about 25× faster to 100 and to 1,000 subscribers (3.6M against 146k, and 1.8M against 69k deliveries/s; `benchmarks/mini_redis/run.sh`, phase `pubsub`, Linux x86_64 VM); with one subscriber Gem is at 0.8×. The server is CPU-bound and `strace -c` shows why: 50,105 `write` calls for 50,000 deliveries (100 subscribers × 500 messages), 95% of the syscall time, about 28 µs each in the Linux VM measured. Redis appends to each client's output buffer and writes it once per event-loop pass, so a burst of 100 PUBLISHes costs each subscriber one write. Here every subscriber runs as soon as the send wakes it, before the next message exists, so the connection's "take every queued message, then write once" loop never finds a second one; and the publishing connection makes one `gen_server.call` per PUBLISH, so subscribers run between any two publishes. Options: in the app, send a pipelined run of PUBLISHes to the pubsub server in one call (as the store's batches do); in the runtime, have `tcp_write` of a small buffer append to a per-socket output buffer that the scheduler flushes once per pass, as Redis does (the write's result then reports what was buffered, and a failure shows on a later write, as it already can).

### Multi-threaded work-stealing scheduler (P2)
The scheduler is single-threaded — one scheduler loop round-robining coroutines on one OS thread. N scheduler threads with per-thread run queues and work-stealing (Chase-Lev deque) would scale throughput ~linearly with cores. The per-process arena model already eliminates shared-heap contention. Hard parts: mailboxes need lock-free MPSC queues for cross-thread sends, shared globals (`gem_proc_table`, `gem_name_registry`, free list) need synchronization, each thread needs its own kqueue/epoll set, and process migration (stealing a coroutine between scheduler ticks) needs care. Erlang/BEAM does exactly this architecture. Nothing in the current design blocks it — isolated processes, message passing, and per-process memory are the right foundation.

### Per-process footprint: four memory mappings and ~21 KB (P1)
Every live process costs about 21 KB of memory on Linux x86_64 (about 70 KB on macOS arm64, whose pages are 16 KB) and four memory mappings: the stack block is three (minicoro's header, the `PROT_NONE` guard, the stack), the arena's first block a fourth. On Linux the per-program mapping limit `vm.max_map_count` (default 65,530) therefore stops `spawn` at about 14,000 live processes (it keeps 1/8 of the limit as headroom for the running processes; "spawn: too many processes for the system's memory-mapping limit"), well below the process table's 262,144. Measured on Linux x86_64: 1,000 idle processes in 23 MB, 10,000 in 210 MB, 50,000 in 1.0 GB, 100,000 in 2.1 GB (with the limit raised); on macOS arm64, 10,000 in 0.68 GB, 50,000 in 3.5 GB. Ways down, roughly in order of payoff: carve the first arena block out of the stack mapping (above the stack top, so a process is three mappings); move `GemProcess.pcall_stack` (64 frames with a `jmp_buf` each, 15 KB of the 22 KB slot) and `call_stack` (256 frames, 6 KB) out of the slot into lazily allocated blocks, so an idle process touches one page of its slot; and allocate stacks from larger mappings carved into guarded slabs (one `mprotect` per guard still splits the mapping, so this needs guard-less stacks with a red-zone check only, or `MAP_GROWSDOWN`-style tricks — weigh against the stack-overflow containment in gem_scheduler.c "Process stacks").

### A module-level table that grows between spawns is copied for every child (P1)
`spawn` puts each module-level binding whose value changed since the last spawn into a new snapshot unit (gem_copy.c, "Module globals"), a malloc'd copy the child keeps until it exits, whether or not it reads the binding. Top-level code that does `push(pids, spawn(...))` in a loop therefore copies `pids` once per spawn, O(n²) in time and memory: 3,000 spawns 290 ms, 6,000 spawns 1.0 s, 10,000 spawns 11 s and 2.6 GB, against 64 ms, 116 ms and 0.2 s with `pids` a local of a function (Linux x86_64). A program that keeps tens of thousands of processes hits it, so it is a trap (BEST_PRACTICES.md, "Don't grow a module-level table while spawning"). Fix options: build the unit lazily, at the child's first read, from a copy-on-write freeze of the parent's table (the parent's next mutation copies instead); or share one unit between consecutive spawns and record only the appended tail per child.

### `monitor` of one target by many processes is O(n²) (P2)
`gem_monitor_fn` walks the target's whole monitor list to deduplicate (and to drop nodes of exited watchers), so n processes monitoring one target cost O(n²): 5,000 in 0.3 s, 20,000 in 6.6 s, 40,000 in 38 s on Linux x86_64; 0.14 s, 1.5 s and 6.4 s on macOS arm64 (spawning the watchers included: 0.05 s, 0.19 s and 0.38 s of it). A per-target hash set of watcher pids (or a per-watcher set of targets checked from the caller's side) makes it O(1); the exited-watcher cleanup can happen when the target dies.

### `cancel_timer` scans the timer heap (P2)
`gem_cancel_timer_builtin` finds the timer by a linear scan of `gem_timers`, so cancelling many of a large number of pending timers is O(timers) each. An index from timer ref to heap position (kept up to date by the sift functions, as the scheduler's deadline heap does with `dl_idx`) makes it O(log n).

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
