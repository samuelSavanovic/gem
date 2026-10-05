# Benchmark baseline

```
date: 2026-10-05T16:27:25Z
gem commit: 21ce7ce
system: Darwin 27.0.0 arm64
os: macOS 27.0.1 (26A434)
cpu: Apple M1 Pro, 10 cores (8 performance)
memory: 16 GB
power: AC Power
low power mode: 0
load average at start: 1.05 1.25 1.35
cc: Apple clang version 21.0.0 (clang-2100.3.34.2)
python3: Python 3.14.5
python3.14: Python 3.14.5
node: v25.9.0
elixir: Elixir 1.20.1 (compiled with Erlang/OTP 29)
redis: Redis server v=8.10.2 sha=00000000:1 malloc=libc bits=64 build=764b3e45065a66fb
wrk: wrk 4.2.0 [kqueue] Copyright (C) 2012 Will Glozer
grep: ggrep (GNU grep) 3.12
REPS=5 WARMUP=1
GUARD_RSS_MB=8192 GUARD_SWAP_MB=2048 GUARD_TIMEOUT_S=2400
SECTIONS=logstat lox gemgrep jobqueue bookmark bookmark_node mini_redis stomp
```

Sections (status, wall time):

```
logstat        ok        93 s  peak    269 MB
lox            ok       130 s  peak   1845 MB
gemgrep        ok       209 s  peak    371 MB
jobqueue       ok       576 s  peak    583 MB
bookmark       ok       377 s  peak     30 MB
bookmark_node  ok       377 s  peak    317 MB
mini_redis     ok        92 s  peak    825 MB
stomp          ok        77 s  peak    742 MB
```

## Batch programs

Medians over the timed runs; spread is (max − min) / median of wall time; peak RSS is the highest of the runs. gem/impl > 1 means Gem is slower.

### logstat

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| --by ip | gem_file | 1.64 | 1% | 26.89 | 240.23 |  |
| --by ip | gem_stdin | 1.61 | 2% | 26.42 | 4.88 |  |
| --by ip | python | 1.68 | 3% | 26.72 | 21.67 | 0.98 / 1.01 |
| --by path | gem_file | 1.63 | 1% | 26.92 | 244.88 |  |
| --by path | gem_stdin | 1.61 | 1% | 26.46 | 9.55 |  |
| --by path | python | 1.69 | 2% | 26.73 | 23.39 | 0.97 / 1.01 |
| --by hour | gem_file | 1.56 | 0% | 26.24 | 239.36 |  |
| --by hour | gem_stdin | 1.54 | 1% | 25.77 | 4.00 |  |
| --by hour | python | 1.64 | 1% | 26.68 | 21.27 | 0.95 / 0.98 |

### lox

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| fib 28 | gem | 1.10 | 2% | 17.44 | 1,990.44 |  |
| fib 28 | python | 1.62 | 1% | 28.11 | 20.28 | 0.68 / 0.62 |
| binary_trees 12 | gem | 2.59 | 0% | 40.78 | 76.28 |  |
| binary_trees 12 | python | 3.17 | 1% | 53.61 | 23.45 | 0.82 / 0.76 |
| closures 100000 | gem | 1.65 | 1% | 26.79 | 7.55 |  |
| closures 100000 | python | 2.49 | 1% | 40.22 | 20.50 | 0.66 / 0.67 |
| strings 20000 | gem | 1.07 | 2% | 17.79 | 5.00 |  |
| strings 20000 | python | 1.90 | 1% | 31.89 | 20.30 | 0.57 / 0.56 |
| mandelbrot 60 | gem | 1.01 | 1% | 17.25 | 6.27 |  |
| mandelbrot 60 | python | 2.02 | 2% | 33.68 | 20.28 | 0.50 / 0.51 |
| methods 3000 | gem | 1.09 | 1% | 17.06 | 9.27 |  |
| methods 3000 | python | 1.54 | 1% | 25.58 | 20.34 | 0.71 / 0.67 |

### gemgrep

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| literal | gem | 1.30 | 0% | 18.64 | 14.89 |  |
| literal | grep | 0.16 | 1% | 0.87 | 2.47 | 8.29 / 21.33 |
| literal | python | 0.48 | 1% | 6.69 | 22.73 | 2.70 / 2.79 |
| icase | gem | 3.03 | 1% | 41.92 | 14.09 |  |
| icase | grep | 0.16 | 1% | 0.91 | 2.47 | 19.15 / 46.08 |
| icase | python | 1.09 | 1% | 19.26 | 21.88 | 2.78 / 2.18 |
| alternation | gem | 3.22 | 0% | 45.82 | 20.61 |  |
| alternation | grep | 0.34 | 1% | 2.40 | 2.55 | 9.35 / 19.08 |
| alternation | python | 1.16 | 0% | 18.56 | 24.25 | 2.77 / 2.47 |
| word | gem | 2.79 | 2% | 42.50 | 16.38 |  |
| word | grep | 0.27 | 0% | 1.62 | 2.53 | 10.18 / 26.24 |
| word | python | 2.78 | 0% | 44.24 | 23.19 | 1.00 / 0.96 |
| count | gem | 1.40 | 1% | 20.07 | 13.34 |  |
| count | grep | 0.16 | 1% | 0.67 | 2.47 | 8.98 / 29.94 |
| count | python | 0.53 | 0% | 6.62 | 22.27 | 2.64 / 3.03 |
| list | gem | 0.37 | 1% | 5.78 | 12.38 |  |
| list | grep | 0.05 | 2% | 0.31 | 2.38 | 8.22 / 18.34 |
| list | python | 0.28 | 1% | 3.69 | 21.44 | 1.34 / 1.56 |
| invert | gem | 0.61 | 0% | 9.47 | 14.03 |  |
| invert | grep | 0.18 | 1% | 1.48 | 2.47 | 3.47 / 6.38 |
| invert | python | 0.52 | 2% | 8.20 | 23.16 | 1.17 / 1.15 |
| few | gem | 0.93 | 0% | 14.53 | 12.89 |  |
| few | grep | 0.12 | 1% | 0.62 | 2.47 | 7.87 / 23.44 |
| few | python | 0.43 | 3% | 6.18 | 21.48 | 2.16 / 2.35 |
| many | gem | 6.10 | 2% | 86.45 | 49.67 |  |
| many | grep | 0.63 | 6% | 8.07 | 2.47 | 9.71 / 10.71 |
| many | python | 1.06 | 2% | 16.45 | 31.52 | 5.74 / 5.25 |
| src_literal | gem | 0.08 | 2% | 1.21 | 22.86 |  |
| src_literal | grep | 0.03 | 3% | 0.13 | 2.45 | 2.92 / 8.97 |
| src_literal | python | 0.08 | 2% | 0.98 | 49.77 | 1.03 / 1.23 |
| src_icase | gem | 0.50 | 0% | 6.80 | 22.88 |  |
| src_icase | grep | 0.04 | 3% | 0.24 | 2.45 | 11.11 / 28.07 |
| src_icase | python | 0.25 | 1% | 4.59 | 49.78 | 1.98 / 1.48 |

### jobqueue

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| none | gem | 0.65 | 49% | 5.62 | 123.44 |  |
| none | python | 1.14 | 3% | 9.46 | 54.30 | 0.57 / 0.59 |
| none | elixir | 1.27 | 3% | 13.31 | 147.34 | 0.51 / 0.42 |
| none | elixir_s1 | 1.27 | 1% | 10.22 | 121.38 | 0.51 / 0.55 |
| crash5 | gem | 0.76 | 5% | 6.45 | 145.06 |  |
| crash5 | python | 1.35 | 1% | 10.96 | 55.30 | 0.56 / 0.59 |
| crash5 | elixir | 1.32 | 2% | 15.44 | 147.25 | 0.57 / 0.42 |
| crash5 | elixir_s1 | 1.34 | 2% | 11.00 | 119.61 | 0.57 / 0.59 |
| hang5 | gem | 3.69 | 1% | 2.94 | 77.94 |  |
| hang5 | python | 3.71 | 0% | 4.93 | 42.05 | 1.00 / 0.60 |
| hang5 | elixir | 4.24 | 1% | 12.21 | 121.22 | 0.87 / 0.24 |
| hang5 | elixir_s1 | 4.26 | 1% | 10.10 | 107.64 | 0.87 / 0.29 |
| mixed | gem | 4.71 | 1% | 3.34 | 86.81 |  |
| mixed | python | 4.76 | 1% | 5.78 | 42.36 | 0.99 / 0.58 |
| mixed | elixir | 5.29 | 0% | 14.69 | 138.12 | 0.89 / 0.23 |
| mixed | elixir_s1 | 5.30 | 0% | 11.35 | 107.53 | 0.89 / 0.29 |
| storm | gem | 1.68 | 2% | 2.85 | 72.06 |  |
| storm | python | 1.66 | 3% | 4.81 | 42.05 | 1.01 / 0.59 |
| storm | elixir | 2.28 | 2% | 11.34 | 134.58 | 0.74 / 0.25 |
| storm | elixir_s1 | 2.33 | 1% | 9.21 | 107.59 | 0.72 / 0.31 |
| backlog | gem | 13.97 | 1% | 27.18 | 557.25 |  |
| backlog | python | 12.71 | 0% | 42.42 | 175.31 | 1.10 / 0.64 |
| backlog | elixir | 16.30 | 1% | 116.52 | 246.16 | 0.86 / 0.23 |
| backlog | elixir_s1 | 16.59 | 2% | 87.39 | 224.06 | 0.84 / 0.31 |

### jobqueue (program-measured; medians over runs)

| scenario | impl | jobs/s | p50 ms | p99 ms | tick lag ms | peak RSS MB | invariants ok |
|---|---:|---:|---:|---:|---:|---:|---:|
| none | gem | 61,728 | 0.00 | 7.00 | 2.00 | 123 | 5/5 |
| none | python | 26,737 | 0.00 | 3.00 | 0.00 | 54 | 5/5 |
| none | elixir | 50,890 | 98.00 | 112.00 | 1.00 | 147 | 5/5 |
| none | elixir_s1 | 49,504 | 68.00 | 80.00 | 1.00 | 121 | 5/5 |
| crash5 | gem | 46,620 | 0.00 | 13.00 | 3.00 | 145 | 5/5 |
| crash5 | python | 21,186 | 0.00 | 11.00 | 1.00 | 55 | 5/5 |
| crash5 | elixir | 43,859 | 88.00 | 195.00 | 1.00 | 147 | 5/5 |
| crash5 | elixir_s1 | 42,194 | 32.00 | 86.00 | 1.00 | 119 | 5/5 |
| hang5 | gem | 2,949 | 1,588 | 3,171 | 11.00 | 77 | 5/5 |
| hang5 | python | 2,985 | 1,513 | 3,050 | 3.00 | 42 | 5/5 |
| hang5 | elixir | 2,948 | 1,597 | 3,146 | 6.00 | 121 | 5/5 |
| hang5 | elixir_s1 | 2,945 | 1,595 | 3,141 | 1.00 | 107 | 5/5 |
| mixed | gem | 2,263 | 1,896 | 4,142 | 10.00 | 86 | 5/5 |
| mixed | python | 2,271 | 1,793 | 3,983 | 3.00 | 42 | 5/5 |
| mixed | elixir | 2,255 | 1,902 | 4,126 | 5.00 | 138 | 5/5 |
| mixed | elixir_s1 | 2,250 | 1,892 | 4,114 | 1.00 | 107 | 5/5 |
| storm | gem | 7,153 | 636.00 | 1,347 | 6.00 | 72 | 5/5 |
| storm | python | 7,598 | 506.00 | 1,175 | 2.00 | 42 | 5/5 |
| storm | elixir | 6,963 | 642.00 | 1,381 | 2.00 | 134 | 5/5 |
| storm | elixir_s1 | 6,811 | 651.00 | 1,398 | 1.00 | 107 | 5/5 |
| backlog | gem | 7,479 | 6,358 | 12,647 | 53.00 | 557 | 5/5 |
| backlog | python | 8,382 | 4,909 | 9,921 | 3.00 | 175 | 5/5 |
| backlog | elixir | 6,522 | 6,194 | 8,793 | 4.00 | 246 | 5/5 |
| backlog | elixir_s1 | 6,416 | 6,170 | 8,374 | 2.00 | 224 | 5/5 |

## Servers

One run each; the load generator runs on the same machine.

### bookmark_app vs Node (wrk)

| phase | impl | req/s | p50 ms | p99 ms | non-2xx | socket errors |
|---|---:|---:|---:|---:|---:|---:|
| burst_get_bookmarks | gem | 10,230.05 | 0.88 | 1.93 | 0 | 0 |
| burst_get_bookmarks | node | 17,055.34 | 0.53 | 1.12 | 0 | 0 |
| burst_get_index | gem | 82,810.45 | 0.11 | 0.30 | 0 | 0 |
| burst_get_index | node | 64,482.21 | 0.14 | 0.30 | 0 | 0 |
| soak_get_bookmarks | gem | 10,570.56 | 0.29 | 0.64 | 0 | 0 |
| soak_get_bookmarks | node | 17,054.53 | 0.19 | 0.41 | 0 | 0 |
| burst_post_bookmarks | gem | 226.18 | 17.33 | 51.24 | 0 | 0 |
| burst_post_bookmarks | node | 306.31 | 12.96 | 37.38 | 0 | 0 |

RSS over the whole run (MB):

| impl | min | peak | mean | end | end − min | growth over 2nd half |
|---|---:|---:|---:|---:|---:|---:|
| gem | 3.95 | 21.94 | 16.01 | 15.77 | 11.81 | -1.38 |
| node | 63.22 | 304.75 | 100.68 | 303.06 | 239.84 | 213.53 |

### mini_redis vs redis-server


basic:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| PING_INLINE | 174,216.03 | 173,611.12 | 1.00 | 0.34 | 0.69 |
| PING_MBULK | 183,486.23 | 178,890.88 | 0.97 | 0.30 | 0.55 |
| SET | 177,619.89 | 112,612.61 | 0.63 | 0.42 | 1.21 |
| GET | 185,528.77 | 120,772.95 | 0.65 | 0.30 | 0.80 |
| INCR | 187,265.92 | 106,269.93 | 0.57 | 0.33 | 2.13 |
| LPUSH | 182,149.36 | 118,623.96 | 0.65 | 0.31 | 1.30 |
| RPUSH | 180,505.41 | 111,482.72 | 0.62 | 0.39 | 1.36 |
| LPOP | 186,219.73 | 126,262.62 | 0.68 | 0.29 | 0.78 |
| RPOP | 185,185.17 | 123,001.23 | 0.66 | 0.35 | 0.80 |
| SADD | 184,162.06 | 109,769.48 | 0.60 | 0.31 | 1.59 |
| HSET | 181,159.42 | 107,642.62 | 0.59 | 0.38 | 1.21 |
| SPOP | 181,488.20 | 114,942.53 | 0.63 | 0.39 | 1.30 |
| LPUSH (needed to benchmark LRANGE) | 177,619.89 | 112,485.94 | 0.63 | 0.37 | 1.38 |
| LRANGE_100 (first 100 elements) | 108,225.10 | 44,642.86 | 0.41 | 0.41 | 1.74 |
| LRANGE_600 (first 600 elements) | 33,749.58 | 9,968.10 | 0.30 | 0.97 | 5.97 |
| MSET (10 keys) | 144,300.14 | 62,266.50 | 0.43 | 0.57 | 4.72 |

pipeline:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| PING_INLINE | 1,492,537.25 | 877,193 | 0.59 | 1.92 | 5.87 |
| PING_MBULK | 2,777,778 | 819,672.12 | 0.30 | 0.29 | 7.39 |
| SET | 1,351,351.38 | 564,971.75 | 0.42 | 0.85 | 5.93 |
| GET | 2,040,816.38 | 628,930.81 | 0.31 | 0.49 | 9.30 |
| INCR | 1,612,903.25 | 502,512.56 | 0.31 | 0.71 | 7.50 |
| LPUSH | 1,538,461.62 | 546,448.06 | 0.36 | 0.75 | 7.50 |
| RPUSH | 1,960,784.38 | 588,235.31 | 0.30 | 0.50 | 7.70 |
| LPOP | 1,694,915.25 | 675,675.69 | 0.40 | 0.56 | 2.88 |
| RPOP | 1,851,851.75 | 704,225.31 | 0.38 | 0.52 | 2.59 |
| SADD | 1,694,915.25 | 558,659.19 | 0.33 | 0.56 | 6.11 |
| HSET | 1,515,151.50 | 429,184.56 | 0.28 | 0.65 | 6.97 |
| SPOP | 1,351,351.38 | 617,283.94 | 0.46 | 0.89 | 3.45 |
| LPUSH (needed to benchmark LRANGE) | 1,515,151.50 | 529,100.56 | 0.35 | 0.77 | 5.74 |
| LRANGE_100 (first 100 elements) | 242,130.77 | 59,523.81 | 0.25 | 3.61 | 19.68 |
| LRANGE_600 (first 600 elements) | 28,336.64 | 10,946.91 | 0.39 | 19.65 | 75.97 |
| MSET (10 keys) | 276,243.09 | 119,617.22 | 0.43 | 4.29 | 14.78 |

clients:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| SET | 159,744.41 | 73,800.73 | 0.46 | 13.82 | 22.89 |
| GET | 163,132.14 | 74,404.77 | 0.46 | 17.10 | 24.85 |

keyspace_fill:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| SET | 1,187,648.50 | 464,037.12 | 0.39 | 1.05 | 11.88 |

keyspace_get:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| GET | 172,711.58 | 111,731.84 | 0.65 | 0.36 | 0.86 |

Memory (RSS MB and DBSIZE after each phase):

| point | redis MB | redis keys | gem MB | gem keys |
|---|---:|---:|---:|---:|
| after basic | 27.61 | 163,110 | 181.48 | 163,295 |
| after pipeline | 31.81 | 163,138 | 164.25 | 163,297 |
| after 1000 clients | 28.61 | 63,143 | 442.92 | 63,155 |
| 1M keys of 100 B | 124.23 | 632,280 | 351.77 | 631,977 |
| 1M keys with a 2 s TTL | 61.17 | 0 | 759.77 | 631,873 |
| 3 s later | 61.19 | 0 | 760.02 | 517,661 |
| 6 s later | 61.19 | 0 | 760.34 | 326,321 |
| 9 s later | 61.19 | 0 | 760.66 | 127,001 |

Pub/sub fan-out:

| case | impl | publish/s | delivered/s | seconds |
|---|---:|---:|---:|---:|
| 1 subs × 50000 msgs | redis | 69,719 | 69,595 | 0.72 |
| 1 subs × 50000 msgs | gem | 28,818 | 28,799 | 1.74 |
| 100 subs × 2000 msgs | redis | 78,352 | 7,692,222 | 0.03 |
| 100 subs × 2000 msgs | gem | 2,639 | 263,506 | 0.76 |
| 1000 subs × 200 msgs | redis | 10,446 | 9,244,868 | 0.02 |
| 1000 subs × 200 msgs | gem | 182.00 | 182,115 | 1.10 |

### stomp_broker (one fresh broker per phase)

| phase | harness | RSS MB peak / end | broker |
|---|---:|---:|---:|
| fanout | publish_msgs_per_s=25,488.50, delivered_msgs_per_s=11,167.30, fanout_elapsed_s=0.45, median_first_msg_latency_s=0.02, p100_first_msg_latency_s=0.34, complete_subs=1,000, n_errors=0 | 199 / 188 | alive_at_phase_end=yes  killed-by-driver |
| slow | msgs_sent=304,821, publish_msgs_per_s=20,315.30, fast_subs_min=880, slow_subs_max=97, n_errors=0 | 711 / 708 | alive_at_phase_end=yes  killed-by-driver |
| queue | publish_msgs_per_s=43,326, total_elapsed_s=0.44, delivered_total=10,000, fairness_ratio=1.00, n_publisher_errors=0 | 21 / 21 | alive_at_phase_end=yes  killed-by-driver |
| soak | msgs_sent=1,500, delivered_total=300,000, expected_total=300,000, min_received=1,500, n_errors=0 | 303 / 206 | alive_at_phase_end=yes  killed-by-driver |
