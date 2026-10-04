# Benchmarks

This directory holds the benchmark **harness**, not the application being benchmarked. The Gem target lives in [`examples/bookmark_app`](../examples/bookmark_app) — a small SQLite-backed bookmark CRUD app written in Gem and served via `std/http`.

## Layout

- `run.sh` — wrk-driven benchmark of `examples/bookmark_app`. Starts the app on port 8080, runs warmup → read bursts → 5-minute soak → write burst, samples RSS throughout, writes results to `benchmarks/logs/<timestamp>/`.
- `wrk_post_bookmark.lua` — wrk script for the POST burst phase.
- `node_baseline/` — reference Node.js implementation of the routes the benchmark hits (`GET /`, `GET /bookmarks`, `POST /bookmarks`; port 8081). Lets us compare like-for-like under identical wrk parameters. Has its own `run_bench.sh`.
- `logs/` — output of `run.sh` runs (gitignored). One subdirectory per run: phase outputs, `rss.csv`, `meta.txt` with system info and the gem commit SHA.

The app's routes live in `examples/bookmark_app/bookmarks.gem` (`app.gem` is the entry point); `examples/bookmark_app/test.gem` checks them, as part of `make test`. Keep the response bodies the same as the Node baseline's when changing it, or the two stop being comparable.

`logstat/run.sh [lines]` is a separate benchmark: it times `examples/logstat` (an access-log analyzer) against the same program in Python (`logstat/logstat.py`) on a generated log and diffs their reports. It needs only `python3`.

`mini_redis/` benchmarks `examples/mini_redis` (a Redis-protocol server) against a real `redis-server`; see [below](#mini_redis).

`lox/` benchmarks `examples/lox` (a tree-walking interpreter for the Lox language) against the same interpreter in Python; see [below](#lox).

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

The Gem server spent 32.8 s in arena resets in the `basic` phase and 25.8 s in `pipeline` (`gem.log`): its keyspace is re-copied by every reset ("Survivors of a reset are copied again" in OPTIMIZATIONS.md).

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
