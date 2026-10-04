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

