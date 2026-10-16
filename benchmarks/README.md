# Benchmarks

Harnesses that run the example programs under load, most of them next to a reference implementation that prints the same output. Every harness here needs a built `build/gem`; the other prerequisites are listed per harness.

| Harness | Example program | Compared with |
|---|---|---|
| `run.sh` | `examples/bookmark_app` | the same routes in Node (`node_baseline/`) |
| `logstat/` | `examples/logstat` | the same program in Python |
| `lox/` | `examples/lox` | the same interpreter in Python |
| `gemgrep/` | `examples/gemgrep` | GNU grep, and the same program in Python |
| `mini_redis/` | `examples/mini_redis` | `redis-server` |
| `jobqueue/` | `examples/jobqueue` | the same design in Python asyncio and in Elixir/OTP |
| `stomp/` | `examples/stomp_broker` | nothing; the numbers are the broker's own |
| `threadpool/` | `threadpool/bench.gem` | itself, at different `GEM_POOL_SIZE` values |
| `soak/` | the long-running servers | nothing; it checks they stay steady for an hour |

Some ratios on an M1 Pro (wall time, lower is better for Gem):

| | Gem / control |
|---|---|
| lox, six programs | 0.43 to 0.65 × Python |
| logstat, 1M lines | 0.8 × Python |
| gemgrep, eleven searches | 0.55 to 2.6 × Python, 1.4 to 17 × GNU grep |

## Running everything

`measure_all.sh` runs every harness except `threadpool/` and `soak/` and writes the results to `baselines/<date>_<machine>/` (gitignored). `summarize.py` writes a summary of one and compares two:

```bash
benchmarks/measure_all.sh                          # about half an hour
SECTIONS="logstat lox" REPS=3 benchmarks/measure_all.sh
python3 benchmarks/summarize.py --compare benchmarks/baselines/OLD benchmarks/baselines/NEW
```

It needs everything the harnesses below need. Each section runs under a watchdog that kills it when it uses too much memory or swap or runs past 40 minutes; the variables at the top of `measure_all.sh` set the limits. Compare baselines from the same machine. Between machines, only the ratios against the reference programs carry over.

The batch harnesses (logstat, lox, gemgrep, jobqueue) time each run through `measure.py`: `BENCH_REPS` timed runs (median wall time), `BENCH_WARMUP` untimed ones first, and `BENCH_CSV` to append the runs to a CSV. On macOS it also reports instructions retired, which vary far less between runs than wall time. `GEM_DIAG=1` adds the arena reset statistics of each Gem run.

## bookmark_app

Needs `wrk`. Build the app once, then run:

```bash
cd examples/bookmark_app && ../../build/gem app.gem -o app && cd ../..
benchmarks/run.sh
```

It starts the app on port 8080 and runs a warmup, read bursts, a 5-minute read soak and a write burst, sampling RSS throughout. Results go to `benchmarks/logs/<timestamp>/`. The phase durations are variables at the top of `run.sh` (`BURST_DURATION`, `SOAK_DURATION` and so on); with the defaults a run takes about 6.5 minutes.

`node_baseline/` is the same three routes (`GET /`, `GET /bookmarks`, `POST /bookmarks`) in Node, on port 8081, with the same response bodies:

```bash
cd benchmarks/node_baseline && npm install && ./run_bench.sh
```

`POST /bookmarks` answers with the whole bookmark list, so each request costs more as the table grows. A 10-second burst keeps the table small. Don't read a long POST run as a write-throughput number: its latency and memory follow the size of the response.

## logstat, lox, gemgrep

```bash
benchmarks/logstat/run.sh [lines]            # default 1,000,000 lines
benchmarks/lox/run.sh [program...]           # about a minute
benchmarks/gemgrep/run.sh [case...]          # about a minute
MB=512 benchmarks/gemgrep/run.sh             # a bigger corpus (default 128 MB)
```

logstat and lox need only `python3`; gemgrep also needs GNU grep (`ggrep` on macOS, `brew install grep`). Each script generates or ships its input, runs the Gem program and the reference on it, prints wall time and peak RSS, and stops if their outputs differ. The programs' READMEs have the full tables.

## mini_redis

Needs `redis-server`, `redis-benchmark`, `redis-cli` and `python3`.

```bash
benchmarks/mini_redis/compat.sh    # same replies as redis-server?
benchmarks/mini_redis/run.sh       # all phases
PHASES="basic pubsub" N=20000 benchmarks/mini_redis/run.sh
```

`run.sh` starts both servers fresh for each phase (Redis without persistence), runs the same load against each, and prints requests per second, the Gem/Redis ratio, p50 and p99 latency, and RSS where a phase is about memory.

| Phase | Load |
|---|---|
| `basic` | redis-benchmark's tests, 50 clients, no pipelining |
| `pipeline` | the same with `-P 16` |
| `clients` | SET/GET with 1000 clients |
| `keyspace` | 1M SETs of 100-byte values, then GETs |
| `expire` | 1M keys with a 2 s TTL, then the key count and RSS every 3 s |
| `pubsub` | 1, 100 and 1000 subscribers on one channel |

Results go to `benchmarks/mini_redis/logs/<timestamp>/`.

## jobqueue

Needs `python3` 3.11 or later and `elixir` (`IMPLS="gem python"` leaves Elixir out).

```bash
benchmarks/jobqueue/run.sh                  # the six scenarios, about 2 minutes
benchmarks/jobqueue/run.sh crash5 storm     # some of them
```

All three implementations take the same options, derive the same workload and fault schedule from the seed, and check the same invariants at the end. For each run the script prints wall time, throughput, latency percentiles, the longest tick lag (how long no process could run), peak RSS and process count, and whether the invariants held, then the Gem/Python and Gem/Elixir throughput ratios. Elixir runs twice, once with a scheduler per core and once with `+S 1`, like Gem and asyncio. The script exits 1 if an invariant fails.

| Scenario | Load |
|---|---|
| `none` | 20,000 jobs, no faults |
| `crash5` | 20,000 jobs, 5% of attempts crash |
| `hang5` | 10,000 jobs, 5% hang until their 100 ms deadline |
| `mixed` | 10,000 jobs with crash, hang, slow and kill faults |
| `storm` | 10,000 jobs while bursts kill 16 workers at a time |
| `backlog` | 100,000 jobs on 4 workers |

## stomp

Needs `python3.14` (or set `PYTHON`).

```bash
benchmarks/stomp/run.sh                            # fanout, slow and queue
PHASES="fanout soak" benchmarks/stomp/run.sh
```

Each phase gets a fresh broker with its RSS sampled every half second. Results go to `benchmarks/stomp/logs/<timestamp>/`, and the script exits 1 if a workload failed or the broker died.

| Phase | Load |
|---|---|
| `fanout` | 1 publisher, 1,000 topic subscribers, 5 messages of 256 bytes |
| `fanout_small` | the same with 100 subscribers |
| `slow` | 1 publisher for 15 s to 4 fast subscribers and 1 that takes 200 ms per message |
| `queue` | 10 publishers, 10,000 messages into one queue, 4 workers |
| `soak` | 200 subscribers at 50 messages a second for 30 s |

## threadpool

```bash
benchmarks/threadpool/run.sh                  # sizes 1 2 4 8 16 32 64
SIZES="4 16" REPS=5 benchmarks/threadpool/run.sh
```

Sweeps the I/O thread pool's size (`GEM_POOL_SIZE`) over three loads in `bench.gem`: many processes reading a small file, readers next to processes that hold a worker for 100 ms at a time, and a SQLite/append/read mix. It prints operations per second and latency percentiles for each, and the start time of an empty program at each size.

## soak

`soak/run.sh` runs each long-lived server for an hour (by default) under a steady, paced load whose answers it checks, and reports whether memory, open files, CPU, throughput and latency stayed flat. It needs `python3`, `git` and, on macOS, `lsof`.

```bash
DURATION=2m benchmarks/soak/run.sh               # a quick check that the harness works
benchmarks/soak/run.sh                           # all five targets, an hour each
TARGETS=mini_redis DURATION=4h benchmarks/soak/run.sh
python3 benchmarks/soak/report.py benchmarks/soak/logs/<run>   # rebuild the report
```

The targets are `mini_redis`, `stomp`, `bookmark`, `honeypot` and `httpd`. The last two get hostile clients: thousands of idle connections, one-byte-a-second senders, floods, malformed and oversized input, clients that never read. For those two the report also checks the server's own database for crashes, lost sessions and dropped events. Each `*_load.py --help` lists its options, and `report.py --help` lists the thresholds.

Results go to `soak/logs/<timestamp>/`, one directory per target, with `report.md` at the top. Every row is written as it is taken, so a run stopped early with Ctrl-C still gets a report. The steadiness checks only count in runs of at least 10 minutes; a shorter run is reported as SHORT RUN.
