# Benchmark baseline

```
date: 2026-10-07T15:56:00Z
gem commit: 5bae04c
system: Darwin 27.0.0 arm64
os: macOS 27.0.1 (26A434)
cpu: Apple M1 Pro, 10 cores (8 performance)
memory: 16 GB
power: AC Power
low power mode: 0
load average at start: 1.46 1.45 2.59
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
logstat        ok        83 s  peak    267 MB
lox            ok       118 s  peak    105 MB
gemgrep        ok       162 s  peak    371 MB
jobqueue       ok       575 s  peak    396 MB
bookmark       ok       376 s  peak     26 MB
bookmark_node  ok       377 s  peak    319 MB
mini_redis     ok        89 s  peak    930 MB
stomp          ok        77 s  peak    759 MB
```

## Batch programs

Medians over the timed runs; spread is (max − min) / median of wall time; peak RSS is the highest of the runs. gem/impl > 1 means Gem is slower.

### logstat

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| --by ip | gem_file | 1.38 | 1% | 20.75 | 239.30 |  |
| --by ip | gem_stdin | 1.35 | 1% | 20.46 | 3.94 |  |
| --by ip | python | 1.71 | 3% | 26.74 | 22.03 | 0.81 / 0.78 |
| --by path | gem_file | 1.36 | 1% | 20.82 | 242.69 |  |
| --by path | gem_stdin | 1.34 | 1% | 20.54 | 7.39 |  |
| --by path | python | 1.68 | 2% | 26.75 | 23.55 | 0.81 / 0.78 |
| --by hour | gem_file | 1.32 | 1% | 20.37 | 238.73 |  |
| --by hour | gem_stdin | 1.29 | 1% | 20.06 | 3.39 |  |
| --by hour | python | 1.67 | 4% | 26.81 | 21.36 | 0.79 / 0.76 |

### lox

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| fib 28 | gem | 0.79 | 1% | 13.39 | 9.84 |  |
| fib 28 | python | 1.62 | 3% | 28.17 | 20.38 | 0.49 / 0.48 |
| binary_trees 12 | gem | 2.02 | 1% | 31.76 | 26.88 |  |
| binary_trees 12 | python | 3.18 | 2% | 53.60 | 23.55 | 0.64 / 0.59 |
| closures 100000 | gem | 1.21 | 1% | 20.47 | 5.70 |  |
| closures 100000 | python | 2.47 | 1% | 39.94 | 20.58 | 0.49 / 0.51 |
| strings 20000 | gem | 0.82 | 1% | 14.70 | 3.98 |  |
| strings 20000 | python | 1.90 | 2% | 31.88 | 20.36 | 0.43 / 0.46 |
| mandelbrot 60 | gem | 0.83 | 1% | 15.03 | 4.44 |  |
| mandelbrot 60 | python | 2.04 | 4% | 33.80 | 20.41 | 0.41 / 0.44 |
| methods 3000 | gem | 0.76 | 0% | 13.01 | 5.98 |  |
| methods 3000 | python | 1.52 | 1% | 25.44 | 20.41 | 0.50 / 0.51 |

### gemgrep

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| literal | gem | 0.97 | 1% | 13.99 | 14.33 |  |
| literal | grep | 0.16 | 1% | 0.87 | 2.47 | 6.16 / 16.05 |
| literal | python | 0.48 | 2% | 6.68 | 21.83 | 2.01 / 2.09 |
| icase | gem | 2.70 | 0% | 37.23 | 14.28 |  |
| icase | grep | 0.16 | 1% | 0.91 | 2.47 | 16.88 / 41.00 |
| icase | python | 1.07 | 1% | 19.08 | 21.84 | 2.51 / 1.95 |
| alternation | gem | 2.33 | 1% | 33.30 | 19.73 |  |
| alternation | grep | 0.34 | 1% | 2.40 | 2.55 | 6.76 / 13.90 |
| alternation | python | 1.16 | 1% | 18.42 | 23.30 | 2.00 / 1.81 |
| word | gem | 2.25 | 1% | 34.96 | 14.92 |  |
| word | grep | 0.28 | 1% | 1.62 | 2.55 | 8.17 / 21.62 |
| word | python | 2.79 | 1% | 44.09 | 22.70 | 0.81 / 0.79 |
| count | gem | 1.38 | 0% | 19.77 | 12.38 |  |
| count | grep | 0.16 | 1% | 0.67 | 2.47 | 8.85 / 29.50 |
| count | python | 0.53 | 0% | 6.63 | 21.55 | 2.61 / 2.98 |
| list | gem | 0.37 | 0% | 5.66 | 11.42 |  |
| list | grep | 0.05 | 2% | 0.31 | 2.38 | 8.06 / 17.99 |
| list | python | 0.28 | 1% | 3.70 | 22.44 | 1.32 / 1.53 |
| invert | gem | 0.29 | 2% | 4.89 | 13.50 |  |
| invert | grep | 0.18 | 1% | 1.48 | 2.47 | 1.66 / 3.30 |
| invert | python | 0.52 | 2% | 8.33 | 21.62 | 0.56 / 0.59 |
| few | gem | 0.92 | 1% | 14.21 | 12.69 |  |
| few | grep | 0.12 | 1% | 0.62 | 2.47 | 7.69 / 22.94 |
| few | python | 0.43 | 2% | 6.18 | 21.62 | 2.12 / 2.30 |
| many | gem | 0.87 | 1% | 12.00 | 48.97 |  |
| many | grep | 0.62 | 3% | 8.07 | 2.47 | 1.39 / 1.49 |
| many | python | 1.06 | 2% | 16.42 | 28.84 | 0.82 / 0.73 |
| src_literal | gem | 0.09 | 1% | 1.31 | 22.53 |  |
| src_literal | grep | 0.02 | 1% | 0.12 | 2.44 | 4.07 / 10.81 |
| src_literal | python | 0.08 | 1% | 0.94 | 49.25 | 1.16 / 1.40 |
| src_icase | gem | 0.42 | 0% | 5.80 | 22.39 |  |
| src_icase | grep | 0.03 | 4% | 0.20 | 2.45 | 12.42 / 29.13 |
| src_icase | python | 0.22 | 2% | 4.00 | 49.23 | 1.92 / 1.45 |

### jobqueue

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| none | gem | 0.62 | 53% | 4.84 | 102.55 |  |
| none | python | 1.15 | 3% | 9.47 | 54.41 | 0.54 / 0.51 |
| none | elixir | 1.26 | 4% | 13.34 | 145.53 | 0.49 / 0.36 |
| none | elixir_s1 | 1.28 | 1% | 10.24 | 121.45 | 0.48 / 0.47 |
| crash5 | gem | 0.70 | 4% | 5.63 | 110.55 |  |
| crash5 | python | 1.35 | 2% | 10.97 | 55.08 | 0.52 / 0.51 |
| crash5 | elixir | 1.33 | 2% | 15.45 | 153.33 | 0.53 / 0.36 |
| crash5 | elixir_s1 | 1.35 | 1% | 10.99 | 120.12 | 0.52 / 0.51 |
| hang5 | gem | 3.67 | 2% | 2.55 | 59.94 |  |
| hang5 | python | 3.71 | 1% | 4.93 | 42.14 | 0.99 / 0.52 |
| hang5 | elixir | 4.25 | 1% | 12.39 | 123.25 | 0.86 / 0.21 |
| hang5 | elixir_s1 | 4.26 | 0% | 10.12 | 107.97 | 0.86 / 0.25 |
| mixed | gem | 4.68 | 0% | 2.92 | 68.41 |  |
| mixed | python | 4.76 | 0% | 5.78 | 42.44 | 0.98 / 0.50 |
| mixed | elixir | 5.29 | 1% | 14.91 | 124.00 | 0.88 / 0.20 |
| mixed | elixir_s1 | 5.32 | 0% | 11.38 | 107.61 | 0.88 / 0.26 |
| storm | gem | 1.65 | 1% | 2.46 | 53.05 |  |
| storm | python | 1.67 | 1% | 4.81 | 41.72 | 0.99 / 0.51 |
| storm | elixir | 2.31 | 3% | 11.34 | 127.77 | 0.72 / 0.22 |
| storm | elixir_s1 | 2.33 | 1% | 9.23 | 107.55 | 0.71 / 0.27 |
| backlog | gem | 13.80 | 0% | 23.40 | 390.75 |  |
| backlog | python | 12.72 | 1% | 42.46 | 173.42 | 1.09 / 0.55 |
| backlog | elixir | 16.35 | 0% | 118.34 | 237.09 | 0.84 / 0.20 |
| backlog | elixir_s1 | 16.65 | 2% | 87.49 | 219.19 | 0.83 / 0.27 |

### jobqueue (program-measured; medians over runs)

| scenario | impl | jobs/s | p50 ms | p99 ms | tick lag ms | peak RSS MB | invariants ok |
|---|---:|---:|---:|---:|---:|---:|---:|
| none | gem | 66,225 | 5.00 | 19.00 | 0.00 | 102 | 5/5 |
| none | python | 26,809 | 0.00 | 3.00 | 0.00 | 54 | 5/5 |
| none | elixir | 50,761 | 99.00 | 113.00 | 1.00 | 145 | 5/5 |
| none | elixir_s1 | 49,751 | 67.00 | 80.00 | 1.00 | 121 | 5/5 |
| crash5 | gem | 51,150 | 0.00 | 14.00 | 3.00 | 110 | 5/5 |
| crash5 | python | 21,186 | 0.00 | 11.00 | 0.00 | 55 | 5/5 |
| crash5 | elixir | 43,956 | 87.00 | 193.00 | 1.00 | 153 | 5/5 |
| crash5 | elixir_s1 | 42,283 | 29.00 | 85.00 | 1.00 | 120 | 5/5 |
| hang5 | gem | 2,952 | 1,589 | 3,155 | 10.00 | 59 | 5/5 |
| hang5 | python | 2,985 | 1,514 | 3,051 | 3.00 | 42 | 5/5 |
| hang5 | elixir | 2,948 | 1,597 | 3,147 | 6.00 | 123 | 5/5 |
| hang5 | elixir_s1 | 2,945 | 1,594 | 3,140 | 1.00 | 107 | 5/5 |
| mixed | gem | 2,266 | 1,898 | 4,140 | 10.00 | 68 | 5/5 |
| mixed | python | 2,273 | 1,789 | 3,978 | 3.00 | 42 | 5/5 |
| mixed | elixir | 2,254 | 1,903 | 4,126 | 4.00 | 124 | 5/5 |
| mixed | elixir_s1 | 2,247 | 1,895 | 4,116 | 2.00 | 107 | 5/5 |
| storm | gem | 7,147 | 635.00 | 1,348 | 6.00 | 53 | 5/5 |
| storm | python | 7,627 | 507.00 | 1,170 | 2.00 | 41 | 5/5 |
| storm | elixir | 6,973 | 643.00 | 1,378 | 2.00 | 127 | 5/5 |
| storm | elixir_s1 | 6,811 | 650.00 | 1,403 | 1.00 | 107 | 5/5 |
| backlog | gem | 7,551 | 6,375 | 12,634 | 79.00 | 390 | 5/5 |
| backlog | python | 8,374 | 4,907 | 9,944 | 3.00 | 173 | 5/5 |
| backlog | elixir | 6,524 | 6,234 | 8,894 | 4.00 | 237 | 5/5 |
| backlog | elixir_s1 | 6,411 | 6,163 | 8,389 | 3.00 | 219 | 5/5 |

## Servers

One run each; the load generator runs on the same machine.

### bookmark_app vs Node (wrk)

| phase | impl | req/s | p50 ms | p99 ms | non-2xx | socket errors |
|---|---:|---:|---:|---:|---:|---:|
| burst_get_bookmarks | gem | 10,552.05 | 0.85 | 1.84 | 0 | 0 |
| burst_get_bookmarks | node | 16,931.15 | 0.53 | 1.14 | 0 | 0 |
| burst_get_index | gem | 90,396.74 | 0.10 | 0.26 | 0 | 0 |
| burst_get_index | node | 65,068.81 | 0.14 | 0.30 | 0 | 0 |
| soak_get_bookmarks | gem | 10,778.32 | 0.28 | 0.61 | 0 | 0 |
| soak_get_bookmarks | node | 16,978.81 | 0.19 | 0.41 | 0 | 0 |
| burst_post_bookmarks | gem | 221.80 | 17.41 | 53.21 | 0 | 0 |
| burst_post_bookmarks | node | 305.85 | 13.05 | 37.30 | 0 | 0 |

RSS over the whole run (MB):

| impl | min | peak | mean | end | end − min | growth over 2nd half |
|---|---:|---:|---:|---:|---:|---:|
| gem | 3.89 | 17.38 | 12.37 | 12.39 | 8.50 | -0.36 |
| node | 63.27 | 307.23 | 101.05 | 305.05 | 241.78 | 214.81 |

### mini_redis vs redis-server


basic:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| PING_INLINE | 173,310.22 | 186,219.73 | 1.07 | 0.45 | 0.52 |
| PING_MBULK | 186,567.16 | 179,856.11 | 0.96 | 0.30 | 0.48 |
| SET | 179,211.45 | 125,944.58 | 0.70 | 0.39 | 1.00 |
| GET | 184,842.88 | 134,952.77 | 0.73 | 0.30 | 0.67 |
| INCR | 180,831.83 | 116,279.07 | 0.64 | 0.34 | 1.83 |
| LPUSH | 185,185.17 | 125,470.52 | 0.68 | 0.31 | 1.13 |
| RPUSH | 188,679.25 | 124,533.01 | 0.66 | 0.31 | 1.12 |
| LPOP | 186,915.88 | 136,054.42 | 0.73 | 0.38 | 0.69 |
| RPOP | 186,915.88 | 139,082.06 | 0.74 | 0.29 | 0.66 |
| SADD | 181,159.42 | 123,609.39 | 0.68 | 0.38 | 1.40 |
| HSET | 183,150.19 | 119,189.52 | 0.65 | 0.39 | 1.10 |
| SPOP | 184,842.88 | 128,700.12 | 0.70 | 0.40 | 1.23 |
| LPUSH (needed to benchmark LRANGE) | 179,856.11 | 125,944.58 | 0.70 | 0.42 | 1.11 |
| LRANGE_100 (first 100 elements) | 108,342.37 | 46,317.74 | 0.43 | 0.40 | 1.66 |
| LRANGE_600 (first 600 elements) | 34,305.32 | 10,334.85 | 0.30 | 0.95 | 5.51 |
| MSET (10 keys) | 142,247.52 | 67,934.78 | 0.48 | 0.57 | 4.33 |

pipeline:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| PING_INLINE | 1,492,537.25 | 970,873.81 | 0.65 | 1.90 | 4.70 |
| PING_MBULK | 2,777,778 | 892,857.12 | 0.32 | 0.29 | 6.65 |
| SET | 1,408,450.62 | 578,034.69 | 0.41 | 0.78 | 5.00 |
| GET | 1,562,499.88 | 675,675.69 | 0.43 | 0.72 | 6.61 |
| INCR | 1,639,344.25 | 520,833.34 | 0.32 | 0.61 | 8.12 |
| LPUSH | 1,724,138 | 552,486.19 | 0.32 | 0.54 | 6.89 |
| RPUSH | 1,960,784.38 | 617,283.94 | 0.31 | 0.49 | 7.08 |
| LPOP | 1,562,499.88 | 724,637.69 | 0.46 | 0.76 | 2.29 |
| RPOP | 1,818,181.88 | 636,942.62 | 0.35 | 0.52 | 4.99 |
| SADD | 1,666,666.75 | 552,486.19 | 0.33 | 0.58 | 5.98 |
| HSET | 1,449,275.38 | 531,914.94 | 0.37 | 0.66 | 5.82 |
| SPOP | 1,298,701.25 | 632,911.38 | 0.49 | 0.97 | 3.38 |
| LPUSH (needed to benchmark LRANGE) | 1,587,301.50 | 621,118 | 0.39 | 0.77 | 5.62 |
| LRANGE_100 (first 100 elements) | 240,963.86 | 60,606.06 | 0.25 | 3.84 | 17.95 |
| LRANGE_600 (first 600 elements) | 39,588.28 | 10,960.11 | 0.28 | 16.69 | 75.26 |
| MSET (10 keys) | 271,739.12 | 124,223.60 | 0.46 | 3.81 | 14.27 |

clients:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| SET | 161,550.89 | 79,554.50 | 0.49 | 13.63 | 23.33 |
| GET | 159,489.64 | 79,428.12 | 0.50 | 16.88 | 27.23 |

keyspace_fill:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| SET | 1,196,172.25 | 468,164.81 | 0.39 | 1.00 | 10.34 |

keyspace_get:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| GET | 177,619.89 | 128,534.70 | 0.72 | 0.67 | 0.69 |

Memory (RSS MB and DBSIZE after each phase):

| point | redis MB | redis keys | gem MB | gem keys |
|---|---:|---:|---:|---:|
| after basic | 27.61 | 163,278 | 134.52 | 163,235 |
| after pipeline | 30.69 | 163,179 | 181.56 | 163,143 |
| after 1000 clients | 28.95 | 63,194 | 323.88 | 63,314 |
| 1M keys of 100 B | 124.36 | 631,724 | 340.61 | 632,077 |
| 1M keys with a 2 s TTL | 61.17 | 0 | 865.33 | 632,202 |
| 3 s later | 61.19 | 0 | 865.55 | 534,761 |
| 6 s later | 61.19 | 0 | 865.86 | 347,261 |
| 9 s later | 61.19 | 0 | 866.17 | 163,121 |

Pub/sub fan-out:

| case | impl | publish/s | delivered/s | seconds |
|---|---:|---:|---:|---:|
| 1 subs × 50000 msgs | redis | 69,685 | 69,562 | 0.72 |
| 1 subs × 50000 msgs | gem | 29,509 | 29,508 | 1.69 |
| 100 subs × 2000 msgs | redis | 76,079 | 7,473,795 | 0.03 |
| 100 subs × 2000 msgs | gem | 2,818 | 281,711 | 0.71 |
| 1000 subs × 200 msgs | redis | 10,358 | 9,179,172 | 0.02 |
| 1000 subs × 200 msgs | gem | 194.00 | 194,285 | 1.03 |

### stomp_broker (one fresh broker per phase)

| phase | harness | RSS MB peak / end | broker |
|---|---:|---:|---:|
| fanout | publish_msgs_per_s=22,857.10, delivered_msgs_per_s=10,024.10, fanout_elapsed_s=0.50, median_first_msg_latency_s=0.28, p100_first_msg_latency_s=0.40, complete_subs=1,000, n_errors=0 | 184 / 176 | alive_at_phase_end=yes  killed-by-driver |
| slow | msgs_sent=299,749, publish_msgs_per_s=19,947.70, fast_subs_min=879, slow_subs_max=98, n_errors=0 | 729 / 726 | alive_at_phase_end=yes  killed-by-driver |
| queue | publish_msgs_per_s=42,616.40, total_elapsed_s=0.45, delivered_total=10,000, fairness_ratio=1.00, n_publisher_errors=0 | 8 / 8 | alive_at_phase_end=yes  killed-by-driver |
| soak | msgs_sent=1,500, delivered_total=300,000, expected_total=300,000, min_received=1,500, n_errors=0 | 253 / 196 | alive_at_phase_end=yes  killed-by-driver |
