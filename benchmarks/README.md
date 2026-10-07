# Benchmarks

This directory holds the benchmark **harness**, not the application being benchmarked. The Gem target lives in [`examples/bookmark_app`](../examples/bookmark_app) — a small SQLite-backed bookmark CRUD app written in Gem and served via `std/http`.

## Layout

- `run.sh` — wrk-driven benchmark of `examples/bookmark_app`. Starts the app on port 8080, runs warmup → read bursts → 5-minute soak → write burst, samples RSS throughout, writes results to `benchmarks/logs/<timestamp>/`.
- `wrk_post_bookmark.lua` — wrk script for the POST burst phase.
- `node_baseline/` — reference Node.js implementation of the routes the benchmark hits (`GET /`, `GET /bookmarks`, `POST /bookmarks`; port 8081). Lets us compare like-for-like under identical wrk parameters. Has its own `run_bench.sh`.
- `logs/` — output of `run.sh` runs (gitignored; `OUT` picks another directory). One subdirectory per run: phase outputs, `rss.csv`, `meta.txt` with system info and the gem commit SHA.

The app's routes live in `examples/bookmark_app/bookmarks.gem` (`app.gem` is the entry point); `examples/bookmark_app/test.gem` checks them, as part of `make test`. Keep the response bodies the same as the Node baseline's when changing it, or the two stop being comparable.

`logstat/run.sh [lines]` is a separate benchmark: it times `examples/logstat` (an access-log analyzer) against the same program in Python (`logstat/logstat.py`) on a generated log and diffs their reports. It needs only `python3`.

`mini_redis/` benchmarks `examples/mini_redis` (a Redis-protocol server) against a real `redis-server`; see [below](#mini_redis).

`lox/` benchmarks `examples/lox` (a tree-walking interpreter for the Lox language) against the same interpreter in Python; see [below](#lox).

`gemgrep/` benchmarks `examples/gemgrep` (a recursive grep on libc's regex) against GNU grep and the same program in Python; see [below](#gemgrep).

`jobqueue/` benchmarks `examples/jobqueue` (a job queue with a supervised worker pool under fault injection) against the same design in Python asyncio and in Elixir/OTP; see [below](#jobqueue).

`soak/` is not a benchmark: it runs the long-lived servers (mini_redis, stomp_broker, bookmark_app) under a steady mixed load for an hour each and checks that they stay up, answer correctly, and keep memory, file descriptors and latency flat; see [below](#soak).

## Baselines

`measure_all.sh` runs every harness in this directory (stomp's is in `stomp/`) and writes a baseline directory, `baselines/<date>_<machine>/` (e.g. `2026-10-05_m1pro`), to commit:

```bash
benchmarks/measure_all.sh                          # everything, about an hour
SECTIONS="logstat lox" REPS=3 benchmarks/measure_all.sh
python3 benchmarks/summarize.py --compare benchmarks/baselines/OLD benchmarks/baselines/NEW
```

A baseline holds `meta.txt` (commit, machine, tool versions, and on macOS the power source), each harness's output (`<section>.txt`, and a directory per server), `batch.csv` (one row per timed run of a batch program), `sections.txt` (status, wall time and peak RSS of each section), and what `summarize.py` makes of them: `summary.csv` (one row per bench, case, implementation and metric) and `summary.md`. `--compare` prints the headline metrics two baselines share (wall time, instructions, peak memory, throughput, latency) and the change in percent. Compare baselines from the same machine; between machines only the ratios against the controls carry over.

Each section runs under a watchdog that kills it, servers and load generators included, when its processes pass half the RAM (`GUARD_RSS_MB`), swap grows by 2 GB (`GUARD_SWAP_MB`), macOS reports critical memory pressure, or it runs past 40 minutes (`GUARD_TIMEOUT_S`); `sections.txt` records each section's peak RSS (the highest of its samples) and the reason for any kill.

The batch harnesses (logstat, lox, gemgrep, jobqueue) time each run through `measure.py`: `BENCH_REPS` timed runs (median wall time), `BENCH_WARMUP` untimed ones before them, and `BENCH_CSV` to append the runs to a CSV. On macOS it reads `/usr/bin/time -l`, which adds instructions retired, cycles and peak memory footprint; instruction counts move far less between runs than wall time. The servers run once each, with their load generators on the same machine.

## Running

Prereqs: `wrk` on PATH (`brew install wrk`), the gem app built once.

```bash
# Build the gem app
cd examples/bookmark_app && ../../build/gem app.gem -o app && cd ../..

# Run the benchmark
./benchmarks/run.sh
```

Phase durations are tunable via env vars — see the top of `run.sh` (`BURST_DURATION`, `SOAK_DURATION`, etc.) for the full list. With the defaults a run takes about 6.5 minutes.

## Caveat: POST phase is O(N²), not a steady-state write benchmark

`POST /bookmarks` returns the full bookmark list as HTMX (`list_page` in `examples/bookmark_app/bookmarks.gem`), so per-request work scales with the table size. A 10s burst keeps N small and the numbers look reasonable; a 5-minute POST soak grows N to several thousand and latency/RSS climb accordingly (response size hits ~2 MB/req, p99 walks into the hundreds of ms). The cost comes from the response shape, not from the runtime. If you want a real write-throughput number, either change the POST handler to return only the new row, truncate the table between phases, or hit a write-only endpoint that doesn't render the list.

## Node baseline

```bash
cd benchmarks/node_baseline
npm install
./run_bench.sh
```

Results land in `benchmarks/node_baseline/results/`. Use the same wrk parameters as the gem run for a fair comparison.

## mini_redis

`examples/mini_redis` speaks enough of the Redis protocol for `redis-benchmark` and `redis-cli`, so the reference implementation is Redis itself, with the same client and the same parameters. Prereqs: `redis-server`, `redis-benchmark`, `redis-cli` (`brew install redis`) and `python3`.

```bash
benchmarks/mini_redis/compat.sh    # same replies as redis-server for compat_commands.txt?
benchmarks/mini_redis/run.sh       # all phases, about 10 minutes
PHASES="basic pubsub" N=20000 benchmarks/mini_redis/run.sh
```

`run.sh` starts both servers fresh for each phase (Redis without persistence), runs the same load against each, and prints them side by side: requests per second, the Gem/Redis ratio, p50 and p99 latency, and RSS and key counts where a phase is about memory. The phases:

| Phase | What it loads | What it shows |
|---|---|---|
| `basic` | redis-benchmark's tests, 50 clients, no pipelining | per-command overhead: parse, a `gen_server.call` to the store, reply |
| `pipeline` | the same with `-P 16` | the cost per command once the round trips are amortized |
| `clients` | SET/GET with 1000 clients | a process per connection, `poll()` over 1000 sockets |
| `keyspace` | 1M SETs of 100-byte values over 1M keys, then GETs | memory for a large keyspace; resets of a loop that holds it |
| `expire` | 1M keys with a 2 s TTL, then key count and RSS every 3 s | active expiry, and whether memory comes back |
| `pubsub` | `pubsub_bench.py`: 1, 100 and 1000 subscribers on one channel | fan-out: a message copy and a socket write per subscriber |

Results land in `benchmarks/mini_redis/logs/<timestamp>/` (gitignored): `summary.txt`, the raw `--csv` output of each run, `meta.txt` (commit, CPU, Redis version), and `gem.log`, with the server's `GEM_DIAG=1` arena statistics for each phase (the harness stops the Gem server with `SHUTDOWN` so it prints them).

Compare ratios, not absolute numbers, between machines: Redis is the control. Both servers run on the machine that runs the load, and `redis-benchmark` uses CPU of its own.

First run (October 2026, commit 97426ba + mini_redis, Linux x86_64 VM, 4 cores, Redis 7.0.15; ratios are Gem/Redis requests per second):

| Phase | Gem/Redis | Notes |
|---|---|---|
| basic: PING | 0.78–0.83 | protocol and connection process only |
| basic: SET, GET, INCR, list/set/hash ops | 0.25–0.41 | p99 4–12 ms against ~1 ms |
| basic: LRANGE_100 / LRANGE_600 | 0.15 / 0.09 | p99 124 / 157 ms |
| pipeline (-P 16) | 0.10–0.25 | Gem tops out at 130–220k ops/s |
| 1000 clients: SET, GET | 0.25 | p50 60 ms against 8 ms; RSS 442 MB against 21 MB |
| 1M keys of 100 B | 0.25 (fill) | RSS 330 MB against 143 MB |
| 1M keys, 2 s TTL | | Gem's active expiry takes ~9 s to clear them, and RSS stays at 442 MB |
| pub/sub, 1 / 100 / 1000 subscribers | 0.81 / 0.04 / 0.04 | one write per delivered message (OPTIMIZATIONS.md) |

The Gem server spent 32.8 s in arena resets in the `basic` phase and 25.8 s in `pipeline` (`gem.log`): its keyspace was re-copied by every reset. Since resets promote what they keep (OPTIMIZATIONS_LOG.md, "Promote what a reset keeps"), with `PHASES="basic pipeline" N=50000` on the same VM the resets take 3.6 s in `basic` and 1.5 s in `pipeline` (10.7 s and 12.5 s just before), copying 0.4 GB in each (4.0 and 3.5 GB); LRANGE_100's p99 in `basic` fell from 51 to 8.5 ms, and LRANGE_600's throughput in `pipeline` went from 2,074 to 3,903 requests per second. The other commands moved within the run-to-run noise.

## lox

`examples/lox` interprets Lox programs by walking their syntax tree, so it measures the CPU-bound core of Gem: calls, recursion, closures, `match` dispatch, field access on tables, and short-lived tables. The reference is `lox/lox.py`, the same interpreter in Python, module for module (dict nodes with a `kind`, the same resolver, `return` passed up as a record), so the ratio compares the two runtimes on the same algorithm. It needs only `python3`.

```bash
benchmarks/lox/run.sh                    # the six programs, about a minute
benchmarks/lox/run.sh fib methods        # some of them
GEM_DIAG=1 benchmarks/lox/run.sh         # plus the arena reset statistics of each Gem run
```

For each program in `examples/lox/bench/`, at a size where Python takes 2–12 s, it prints the wall time and peak RSS of both runs and the Gem/Python time ratio, and stops with a diff if the outputs differ.

| Program | Size | What it loads |
|---|---|---|
| `fib.lox` | 28 | a naive recursive Fibonacci: 1M calls, no loop |
| `binary_trees.lox` | 12 | the benchmarks game's binary-trees: 670,000 short-lived instances, one long-lived tree |
| `closures.lox` | 100000 | closures made and called in a loop, a list made of closures |
| `strings.lox` | 20000 | numbers spelled digit by digit and word-wrapped: concatenation, `len`, `==` |
| `mandelbrot.lox` | 60 | the Mandelbrot set in ASCII: float arithmetic in nested loops |
| `methods.lox` | 3000 | a particle simulation with classes: method calls, fields, inheritance, `super` |

First run (October 2026, commit 2487e74 + lox, Linux x86_64 VM, 4 cores, Python 3.11; two runs, ratios are Gem/Python wall time):

| Program | Gem/Python | Notes |
|---|---|---|
| `fib.lox 28` | 1.09–1.15 | peak RSS 1.9 GB against 11 MB: nothing is freed during the recursion |
| `binary_trees.lox 12` | 0.66–0.69 | resets copy the long-lived tree again: 1.6 GB, a quarter of the run |
| `closures.lox 100000` | 0.90–0.97 | |
| `strings.lox 20000` | 0.76–0.79 | |
| `mandelbrot.lox 60` | 0.66–0.69 | |
| `methods.lox 3000` | 0.92–0.93 | |

The other programs stay at 10 MB, like Python. In callgrind profiles 40–45% of the Gem instructions are string-key table lookups, because the field-access inline cache misses on 99% of the reads, and the Gem runs spend 20–30% of their time in page faults on the arena blocks resets map anew. `docs/OPTIMIZATIONS.md` tracks each of these.

## gemgrep

`examples/gemgrep` is a recursive grep on libc's POSIX regex (`regcomp`/`regexec` through `extern fn`), so it measures Gem's C interop on a hot path, plus whole-file reads, line walking with `find`, and output building. The control is GNU grep (`grep -E`, in the C locale), as redis-server is for mini_redis; `gemgrep/gemgrep.py` is the same program in Python on `re` (same options, walk order, output and messages; its docstring lists where `re` and POSIX EREs differ). It needs `python3` and GNU grep.

```bash
benchmarks/gemgrep/run.sh                  # the eleven searches, about a minute
benchmarks/gemgrep/run.sh few many         # some of them
MB=512 benchmarks/gemgrep/run.sh           # a bigger corpus (default 128 MB)
GEM_DIAG=1 benchmarks/gemgrep/run.sh       # plus the arena reset statistics of each Gem run
```

`gen_corpus.py` writes the corpus into a temporary directory: 1,092 files (at 128 MB) of log-like and code-like lines in 80 directories, the same bytes on every run, with the rare token `deadbeef` on about one line in 2,500. `gen_src.py` writes the tree the `src_*` searches run over, shaped like this repository's sources: `compiler`, `runtime`, `std`, `lsp` and `examples`, 369 files and 12 MB of C-like, Gem-like and text lines, most files a few KB and one C file of 9.5 MB, the same bytes on every run. For each search it prints the wall time and peak RSS of the three programs and the Gem/grep and Gem/Python ratios, and stops with a diff if gemgrep's output differs from the twin's, or from grep's once sorted (grep walks directories in readdir order, the other two in sorted order).

| Search | What it loads |
|---|---|
| `literal`: `-r handler` | a common word: 162,000 lines out |
| `icase`: `-ri timeout` | case folding |
| `alternation`: `-r 'connect(ed\|ion)\|socket'` | an alternation with a group: 453,000 lines out |
| `word`: `-rw id` | `-w`, which wraps the pattern in boundary groups |
| `count`: `-rc error` | counting, no output |
| `list`: `-rl deadbeef` | `-l`: each file until its first match |
| `invert`: `-rv e` | `-v` |
| `few`: `-rn deadbeef` | a pattern with few matches: nearly all regex work |
| `many`: `-rn e` | most lines match: 2.4M lines, 191 MB out |
| `src_literal`, `src_icase` | a source-like tree: many small files, a few large ones |

First run (October 2026, commit 5304ef5 + gemgrep, Linux x86_64 VM, 4 cores, GNU grep 3.11, Python 3.11; two runs, ratios of wall time):

| Search | Gem/grep | Gem/Python |
|---|---|---|
| `literal` | 4.9–5.2 | 1.37–1.43 |
| `icase` | 6.2–6.4 | 0.82–0.88 |
| `alternation` | 3.5–3.8 | 0.76–0.84 |
| `word` | 5.9–6.1 | 0.45–0.46 |
| `count` | 6.3–7.5 | 1.51–1.62 |
| `list` | 9.5–11 | 1.02–1.09 |
| `invert` | 5.2–5.9 | 0.88–1.19 |
| `few` | 7.4–7.8 | 1.29–1.41 |
| `many` | 4.5–4.7 | 1.35–1.63 |
| `src_literal` | 13 | 2.0–2.3 |
| `src_icase` | 3.2–6.1 | 0.40–0.58 |

In this run the `src_*` searches ran over the repository's own sources at that commit, not the generated tree. The `src_*` runs take 0.1–0.4 s, so their ratios are the noisiest. Peak RSS: 13–20 MB for gemgrep on the corpus (49 MB for `many`), 10 MB for grep, 13–18 MB for Python; 22 MB against Python's 40 MB on the sources. Per line, gemgrep spends about as many instructions on its own side (the line walk, the binding's checks, the call) as in `regexec`, and GNU grep runs no regex per line at all; `examples/gemgrep/README.md` has the breakdown ("Performance") and the numbers for a whole-buffer helper the program doesn't use ("Design").

## jobqueue

`examples/jobqueue` runs a seeded load of jobs through a queue gen_server and a pool of workers under a `dynamic_supervisor`, while a fault schedule makes attempts crash, hang past their deadline, run slow or kill their worker, and storms kill workers in bursts until the worker supervisor gives up. It measures the OTP machinery under failure: monitors and `DOWN`s, restarts, `send_after` timers, kills, and a long-lived server holding a record per job. The control is `jobqueue/jobqueue.exs`, the same design in Elixir/OTP (a `Supervisor` over a `DynamicSupervisor` of transient GenServer workers and the queue GenServer). `jobqueue/jobqueue.py` is the same design again in Python asyncio: tasks instead of processes, done callbacks as monitors, a supervisor class with the same restart intensity. All three take the same options, compute the same workload and fault schedule from the seed (the same integer hash), print the same summary, and check the same invariants at the end: every job completed once or dead-lettered, every attempt failed as scheduled (storm and shutdown losses counted apart), the counters agree, and the system back at its baseline after the drain. It needs `python3` (3.11 or later) and `elixir` (`apt install elixir`; `IMPLS="gem python"` skips it).

```bash
benchmarks/jobqueue/run.sh                  # the six scenarios, about 3.5 minutes
benchmarks/jobqueue/run.sh crash5 storm     # some of them
IMPLS="gem python" benchmarks/jobqueue/run.sh
```

For each scenario it prints, per implementation, the wall time of the whole program, the throughput and latency percentiles the program measured, the longest tick lag (a 20 ms ticker in the driver: how long no process could run), peak RSS, the process count at the baseline and its peak above it, the retries and worker-supervisor restarts, and whether the invariants held; then the Gem/Python and Gem/Elixir throughput ratios. Elixir runs twice, as it comes (a scheduler per core) and with `+S 1` (one scheduler, like Gem and asyncio). The Gem rows are followed by their `GEM_DIAG=1` arena statistics. The script exits 1 if the invariants fail in any run.

| Scenario | Arguments | What it loads |
|---|---|---|
| `none` | 20,000 jobs, 250 ms deadline | the machinery alone: a call per submit, a message per attempt and per result, a deadline timer set and cancelled |
| `crash5` | 20,000 jobs, 5% crash, 250 ms deadline | a worker crash, `DOWN` and supervisor restart per failed attempt; backoff timers |
| `hang5` | 10,000 jobs, 5% hang, 100 ms deadline | deadline kills: bound by the deadlines, so all three match |
| `mixed` | 10,000 jobs, crash, hang, slow and kill faults | every failure path at once; dead letters |
| `storm` | 10,000 jobs, 10 bursts killing 16 workers, 20 restarts/s allowed | restart intensity: the worker supervisor gives up 5 times and is restarted |
| `backlog` | 100,000 jobs, 4 workers, 5% slow | a long backlog in the queue: big state in one process |

First run (October 2026, commit d88fca9 + jobqueue, Linux x86_64 VM, 4 cores, Python 3.11, Elixir 1.14 on OTP 24; two to four runs, ratios of throughput, higher is better for Gem):

| Scenario | Gem/Python | Gem/Elixir | Gem/Elixir `+S 1` | Notes |
|---|---|---|---|---|
| `none` | 0.88–1.14 | 0.30–0.34 | 0.30–0.44 | |
| `crash5` | 0.97–1.12 | 0.32–0.36 | 0.50–0.55 | Elixir crashes 1,700 workers/s, over the 1,000/s limit: its worker supervisor restarts once |
| `hang5` | 0.99–1.00 | 0.99–1.00 | 1.00 | |
| `mixed` | 1.00 | 1.01 | 1.01 | |
| `storm` | 0.91–0.93 | 0.95–0.97 | 0.94–0.97 | |
| `backlog` | 0.86–0.92 | 1.08–1.16 | 1.08–1.15 | Gem: 810–896 MB peak RSS, 1.5–1.7 s longest stall |

The invariants held in every run of all three, with the same retry counts wherever the run is deterministic (everything but the storm and Elixir's extra restart). The cost is in memory and pauses, not in failure handling. Peak RSS for Gem is 85–174 MB in the 10,000- and 20,000-job scenarios against 34–48 MB for Python and 77–116 MB for Elixir (whose VM starts at about 80 MB), and 810–896 MB against 166 and 200–218 MB for the backlog. These Gem figures predate tables of up to 8 entries without a string-key index (OPTIMIZATIONS_LOG.md, "Small tables scan their keys instead of indexing them"), which on macOS arm64 took the backlog's peak from 559 to 396 MB. The longest tick lag is 40–100 ms for Gem with 10,000–20,000 jobs and 1.5–1.7 s with 100,000, against at most 75 ms for Python and 36 ms for Elixir: each reset of the queue's loop copied every job record it holds, and with the default 100 ms deadline such a pause now and then kills a healthy attempt (the scenarios use 250 ms where they don't test deadlines). Since resets promote what they keep, only full resets copy all the records: in `backlog` the longest tick lag is 0.25–0.48 s and peak RSS 472–475 MB (0.45–0.56 s and 780–960 MB just before, on the same VM). `docs/OPTIMIZATIONS.md` has the numbers ("Full resets still copy all a loop keeps").

## soak

The other harnesses run each server for seconds to a few minutes. `soak/run.sh` runs each one for an hour (by default) under a steady, paced load, so what only shows over time can show: memory a loop's resets never give back, a remembered log, pin set or mailbox that grows slowly, latency that creeps up as kept data grows, process slots, pids, sockets and timers that leak a little per connection. It needs only `python3`; it is not part of `measure_all.sh`.

```bash
DURATION=2m benchmarks/soak/run.sh               # a short run: does the harness work here?
benchmarks/soak/run.sh                           # the three targets, an hour each
TARGETS=mini_redis DURATION=4h benchmarks/soak/run.sh
MINI_REDIS_ARGS="--rate 8000 --subs 50" TARGETS=mini_redis benchmarks/soak/run.sh
python3 benchmarks/soak/report.py benchmarks/soak/logs/<run>   # the report again
```

Each target gets a fresh server, built once at the start (a build error stops the run before it starts), run with `GEM_DIAG=1`, and a load generator that checks every answer it gets:

| Target | Load (defaults; `--help` on each `*_load.py` lists the options) | Checked |
|---|---|---|
| `mini_redis` | 8 clients at 4,000 requests/s in all (one in ten a pipelined batch of 16): GET/SET/DEL, INCR, SET EX with 1–5 s TTLs, a list used as a FIFO, a hash and a set, each client on its own keys; 20 new connections/s; 200 PUBLISHes/s on 4 channels to 20 subscribers that leave and rejoin every 30 s on average | every reply against the client's model of its keys (a TTL check fails only when the reply is wrong for every moment the server could have run the command), messages in order with no gaps |
| `stomp` | 200 SENDs/s to 4 topics with 30 subscribers that leave and rejoin (UNSUBSCRIBE + DISCONNECT, or an abrupt close); 200 jobs/s to a queue with 4 workers; 10 connections/s that SEND and DISCONNECT with a receipt | messages in order with no gaps; every job delivered at most once, and the backlog (sent − received) bounded |
| `bookmark` | 4 readers at 200 GETs/s in all (`/`, `/bookmarks`, edit forms); one writer at 20 POST/PUT/DELETEs per second keeping the table near 100 rows; 10 one-request connections/s | after each change, the list the app answers with against the writer's model of the table; the pages readers get |

There are no slow consumers and no unbounded tables: stomp_broker queues messages for a slow subscriber without bound by design, and bookmark_app's list grows with the table ("POST phase is O(N²)" above), so either would read as a leak. jobqueue isn't a target yet: a run has a fixed number of jobs and keeps a record of each to check its invariants, so its memory grows with the run by design; it needs a mode that runs for a duration and drops finished records first.

Alongside the load, `sample.py` samples the server's RSS, CPU time and open file descriptors (`DURATION`/120 seconds apart, 2 to 30). Results land in `soak/logs/<timestamp>/` (gitignored), one directory per target:

| File | What it holds |
|---|---|
| `load.csv` | a row per interval: throughput, latency p50/p99/max of each kind of request, errors by kind, a probe (a new connection's first answer) |
| `server.csv` | a row per sample: RSS, CPU time and percent, open fds (on macOS `lsof`'s count, mapped files included: compare a run with itself) |
| `errors.log` | the first 200 errors in full; the rest are counted |
| `server.log` | the server's output, with its `GEM_DIAG` statistics at the end when it was shut down cleanly (mini_redis with `SHUTDOWN`; the other two have no clean shutdown, so SIGTERM ends them without the statistics) |
| `status.txt` | how the target ended: the load's exit, whether the server was alive, what the sampler saw |
| `load.csv.done` | the load's totals and why it stopped |

**Nothing is lost when a run stops early.** Every row is flushed and fsynced when it is written, never kept for the end. Ctrl-C (or SIGTERM to `run.sh`) stops the current target cleanly (the server ignores the SIGINT, so it is still shut down normally), skips the rest and writes the report, whose verdict for that target is STOPPED. If the server dies or its RSS passes `GUARD_RSS_MB` (default 4096; the sampler kills it), the load stops at once and the report says why; the run goes on with the next target. If `run.sh` itself is killed hard (`kill -9`, a closed terminal), the load generator and sampler notice and exit, the data on disk is complete up to then, and `report.py` reports on it; the server keeps running and needs stopping by hand. A load generator still running 5 minutes after its deadline is killed and the target fails. On macOS the run holds off system sleep with `caffeinate`.

`report.md` gives each target a verdict and the checks behind it, with a sparkline of each series. The run is split into warm-up (the first 15%, at most 10 minutes: mini_redis's keyspace fills in about 4 minutes), early (the next 10%) and late (the last 10%):

| Check | Passes when |
|---|---|
| ran to the end | the load reached its deadline and the server was alive at the end |
| no errors | no wrong answer, gap, duplicate or I/O error (`errors.log` lists them) |
| memory steady | RSS grew at most 10 MB, or 5% of the late RSS if more, over the second half (least-squares slope) |
| fds steady | the late median of open fds is at most 10 above the early one |
| `<latency>` steady | each latency's median interval p99 late is at most 1.5× the early one, or at most 1 ms above it |
| queue backlog bounded | (stomp) at most 1,000 jobs in the late window |

The steadiness checks (all but the first two) only count in a run of at least 10 minutes; in a shorter one they are shown and the verdict is SHORT RUN, which says the harness works, not that the server is steady. `report.py --help` lists the thresholds. `run.sh` exits 0 when no target failed and the run was not stopped.

