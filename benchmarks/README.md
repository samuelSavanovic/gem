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

`stomp/` loads `examples/stomp_broker` (a STOMP message broker) with topic fan-out, slow subscribers and work queues; there is no control implementation. See [below](#stomp).

## Baselines

`measure_all.sh` runs every harness in this directory (stomp's is in `stomp/`) and writes a baseline directory, `baselines/<date>_<machine>/` (e.g. `2026-10-05_m1pro`), to commit:

```bash
benchmarks/measure_all.sh                          # everything, about 32 minutes on an M1 Pro
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
benchmarks/mini_redis/run.sh       # all phases, about 90 s on an M1 Pro
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

Recorded runs are in the `mini_redis` files of each baseline in [`baselines/`](baselines/); runs on a Linux x86_64 VM at older commits are in [`docs/archive/benchmark_first_runs_linux_vm.md`](../docs/archive/benchmark_first_runs_linux_vm.md#mini_redis).

## lox

`examples/lox` interprets Lox programs by walking their syntax tree, so it measures the CPU-bound core of Gem: calls, recursion, closures, `match` dispatch, field access on tables, and short-lived tables. The reference is `lox/lox.py`, the same interpreter in Python, module for module (dict nodes with a `kind`, the same resolver, `return` passed up as a record), so the ratio compares the two runtimes on the same algorithm. It needs only `python3`.

```bash
benchmarks/lox/run.sh                    # the six programs, about a minute
benchmarks/lox/run.sh fib methods        # some of them
GEM_DIAG=1 benchmarks/lox/run.sh         # plus the arena reset statistics of each Gem run
```

For each program in `examples/lox/bench/`, at a size where Python takes 1.5–3.2 s on an M1 Pro, it prints the wall time and peak RSS of both runs and the Gem/Python time ratio, and stops with a diff if the outputs differ.

| Program | Size | What it loads |
|---|---|---|
| `fib.lox` | 28 | a naive recursive Fibonacci: 1M calls, no loop |
| `binary_trees.lox` | 12 | the benchmarks game's binary-trees: 670,000 short-lived instances, one long-lived tree |
| `closures.lox` | 100000 | closures made and called in a loop, a list made of closures |
| `strings.lox` | 20000 | numbers spelled digit by digit and word-wrapped: concatenation, `len`, `==` |
| `mandelbrot.lox` | 60 | the Mandelbrot set in ASCII: float arithmetic in nested loops |
| `methods.lox` | 3000 | a particle simulation with classes: method calls, fields, inheritance, `super` |

Recorded runs are in the `lox` files of each baseline in [`baselines/`](baselines/); runs on a Linux x86_64 VM at older commits are in [`docs/archive/benchmark_first_runs_linux_vm.md`](../docs/archive/benchmark_first_runs_linux_vm.md#lox).

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

Recorded runs are in the `gemgrep` files of each baseline in [`baselines/`](baselines/); runs on a Linux x86_64 VM at older commits are in [`docs/archive/benchmark_first_runs_linux_vm.md`](../docs/archive/benchmark_first_runs_linux_vm.md#gemgrep).

## jobqueue

`examples/jobqueue` runs a seeded load of jobs through a queue gen_server and a pool of workers under a `dynamic_supervisor`, while a fault schedule makes attempts crash, hang past their deadline, run slow or kill their worker, and storms kill workers in bursts until the worker supervisor gives up. It measures the OTP machinery under failure: monitors and `DOWN`s, restarts, `send_after` timers, kills, and a long-lived server holding a record per job. The control is `jobqueue/jobqueue.exs`, the same design in Elixir/OTP (a `Supervisor` over a `DynamicSupervisor` of transient GenServer workers and the queue GenServer). `jobqueue/jobqueue.py` is the same design again in Python asyncio: tasks instead of processes, done callbacks as monitors, a supervisor class with the same restart intensity. All three take the same options, compute the same workload and fault schedule from the seed (the same integer hash), print the same summary, and check the same invariants at the end: every job completed once or dead-lettered, every attempt failed as scheduled (storm and shutdown losses counted apart), the counters agree, and the system back at its baseline after the drain. It needs `python3` (3.11 or later) and `elixir` (`apt install elixir`; `IMPLS="gem python"` skips it).

```bash
benchmarks/jobqueue/run.sh                  # the six scenarios, about 2 minutes on an M1 Pro
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

Recorded runs are in the `jobqueue` files of each baseline in [`baselines/`](baselines/); runs on a Linux x86_64 VM at older commits are in [`docs/archive/benchmark_first_runs_linux_vm.md`](../docs/archive/benchmark_first_runs_linux_vm.md#jobqueue).

## stomp

`examples/stomp_broker` is a STOMP message broker built on gen_servers, so this load measures fan-out of one message to many connection processes, and memory under a steady publish rate. There is no control implementation: the numbers are the broker's own. `stomp/harness.py` is the load generator (`python3.14`, one thread per client); it prints one JSON line of summary numbers per workload.

```bash
benchmarks/stomp/run.sh                            # fanout, slow and queue
PHASES="fanout soak" benchmarks/stomp/run.sh
```

`run.sh` builds the broker once, then starts a fresh one for each phase (with `GEM_DIAG=1`), samples its RSS every half second, and runs the phase's workload against it. The phases (`PHASES`, default `fanout slow queue`):

| Phase | What it loads |
|---|---|
| `fanout` | 1 publisher, 1,000 topic subscribers, 5 messages of 256 B (`FANOUT_SUBS`, `FANOUT_MSGS`, `FANOUT_BODY`) |
| `fanout_small` | the same with 100 subscribers |
| `slow` | 1 publisher for 15 s to 4 fast subscribers and 1 that takes 200 ms per message (`SLOW_*`) |
| `queue` | 10 publishers, 10,000 messages into one queue, 4 workers (`QUEUE_*`) |
| `soak` | 200 subscribers at 50 messages a second for 30 s (`SOAK_*`): whether memory stays flat |

Results land in `benchmarks/stomp/logs/<timestamp>/` (gitignored; `OUT` picks another directory): per phase the harness's JSON, the broker's log with its `GEM_DIAG` line, the RSS samples and their summary, and `meta.txt`. The script exits 1 if a workload failed or the broker died. `measure_all.sh` runs it with `soak` added.
