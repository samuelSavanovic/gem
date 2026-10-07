# First benchmark runs on a Linux x86_64 VM

The first recorded run of four of the yardstick benchmarks in `benchmarks/` (mini_redis, lox, gemgrep, jobqueue), on a Linux x86_64 VM with 4 cores, in October 2026, at the commits named in each section. They describe the runtime at those commits, not the current one; the recorded runs of the current code are the baselines in `benchmarks/baselines/`, and `benchmarks/README.md` describes each harness.

## mini_redis

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
