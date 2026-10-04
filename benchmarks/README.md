# Benchmarks

This directory holds the benchmark **harness**, not the application being benchmarked. The Gem target lives in [`examples/bookmark_app`](../examples/bookmark_app) — a small SQLite-backed bookmark CRUD app written in Gem and served via `std/http`.

## Layout

- `run.sh` — wrk-driven benchmark of `examples/bookmark_app`. Starts the app on port 8080, runs warmup → read bursts → 5-minute soak → write burst, samples RSS throughout, writes results to `benchmarks/logs/<timestamp>/`.
- `wrk_post_bookmark.lua` — wrk script for the POST burst phase.
- `node_baseline/` — reference Node.js implementation of the routes the benchmark hits (`GET /`, `GET /bookmarks`, `POST /bookmarks`; port 8081). Lets us compare like-for-like under identical wrk parameters. Has its own `run_bench.sh`.
- `logs/` — output of `run.sh` runs (gitignored). One subdirectory per run: phase outputs, `rss.csv`, `meta.txt` with system info and the gem commit SHA.

The app's routes live in `examples/bookmark_app/bookmarks.gem` (`app.gem` is the entry point); `examples/bookmark_app/test.gem` checks them, as part of `make test`. Keep the response bodies the same as the Node baseline's when changing it, or the two stop being comparable.

`logstat/run.sh [lines]` is a separate benchmark: it times `examples/logstat` (an access-log analyzer) against the same program in Python (`logstat/logstat.py`) on a generated log and diffs their reports. It needs only `python3`.

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
