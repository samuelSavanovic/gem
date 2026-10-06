# Benchmark baseline

```
date: 2026-10-06T15:45:51Z
gem commit: 05b979c
system: Darwin 27.0.0 arm64
os: macOS 27.0.1 (26A434)
cpu: Apple M1 Pro, 10 cores (8 performance)
memory: 16 GB
power: AC Power
low power mode: 0
load average at start: 1.47 1.66 2.95
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
logstat        ok        89 s  peak    268 MB
lox            ok       120 s  peak    105 MB
gemgrep        ok       211 s  peak    371 MB
jobqueue       ok       576 s  peak    364 MB
bookmark       ok       377 s  peak     26 MB
bookmark_node  ok       377 s  peak    325 MB
mini_redis     ok        91 s  peak    932 MB
stomp          ok        78 s  peak    784 MB
```

## Batch programs

Medians over the timed runs; spread is (max − min) / median of wall time; peak RSS is the highest of the runs. gem/impl > 1 means Gem is slower.

### logstat

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| --by ip | gem_file | 1.48 | 0% | 24.22 | 239.33 |  |
| --by ip | gem_stdin | 1.45 | 1% | 23.71 | 3.98 |  |
| --by ip | python | 1.69 | 3% | 26.65 | 21.78 | 0.88 / 0.91 |
| --by path | gem_file | 1.48 | 1% | 24.29 | 242.67 |  |
| --by path | gem_stdin | 1.46 | 0% | 23.79 | 7.34 |  |
| --by path | python | 1.69 | 2% | 26.78 | 23.44 | 0.88 / 0.91 |
| --by hour | gem_file | 1.43 | 0% | 23.82 | 238.77 |  |
| --by hour | gem_stdin | 1.41 | 0% | 23.30 | 3.41 |  |
| --by hour | python | 1.64 | 2% | 26.72 | 21.36 | 0.87 / 0.89 |

### lox

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| fib 28 | gem | 0.86 | 1% | 14.36 | 9.84 |  |
| fib 28 | python | 1.63 | 2% | 28.17 | 20.38 | 0.53 / 0.51 |
| binary_trees 12 | gem | 2.13 | 2% | 33.71 | 27.09 |  |
| binary_trees 12 | python | 3.17 | 1% | 53.57 | 23.45 | 0.67 / 0.63 |
| closures 100000 | gem | 1.32 | 1% | 22.09 | 5.69 |  |
| closures 100000 | python | 2.47 | 1% | 40.00 | 20.53 | 0.53 / 0.55 |
| strings 20000 | gem | 0.89 | 1% | 15.81 | 3.97 |  |
| strings 20000 | python | 1.90 | 1% | 31.96 | 20.38 | 0.47 / 0.49 |
| mandelbrot 60 | gem | 0.91 | 1% | 16.08 | 4.42 |  |
| mandelbrot 60 | python | 2.00 | 2% | 33.73 | 20.39 | 0.46 / 0.48 |
| methods 3000 | gem | 0.84 | 1% | 14.07 | 5.97 |  |
| methods 3000 | python | 1.53 | 1% | 25.54 | 20.39 | 0.55 / 0.55 |

### gemgrep

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| literal | gem | 1.31 | 1% | 18.79 | 14.45 |  |
| literal | grep | 0.16 | 1% | 0.87 | 2.47 | 8.27 / 21.51 |
| literal | python | 0.48 | 1% | 6.69 | 21.91 | 2.70 / 2.81 |
| icase | gem | 3.04 | 0% | 42.00 | 14.00 |  |
| icase | grep | 0.16 | 1% | 0.91 | 2.47 | 19.03 / 46.08 |
| icase | python | 1.08 | 1% | 19.25 | 22.67 | 2.81 / 2.18 |
| alternation | gem | 3.24 | 0% | 46.05 | 20.16 |  |
| alternation | grep | 0.35 | 1% | 2.40 | 2.53 | 9.38 / 19.16 |
| alternation | python | 1.16 | 1% | 18.57 | 24.05 | 2.78 / 2.48 |
| word | gem | 2.80 | 0% | 42.63 | 14.91 |  |
| word | grep | 0.28 | 0% | 1.62 | 2.53 | 10.19 / 26.30 |
| word | python | 2.79 | 1% | 44.25 | 23.09 | 1.01 / 0.96 |
| count | gem | 1.39 | 1% | 20.07 | 12.42 |  |
| count | grep | 0.16 | 1% | 0.67 | 2.47 | 8.90 / 29.93 |
| count | python | 0.53 | 0% | 6.63 | 21.59 | 2.63 / 3.03 |
| list | gem | 0.37 | 0% | 5.77 | 11.77 |  |
| list | grep | 0.05 | 1% | 0.31 | 2.38 | 8.11 / 18.33 |
| list | python | 0.28 | 2% | 3.69 | 22.41 | 1.34 / 1.56 |
| invert | gem | 0.62 | 0% | 9.47 | 13.53 |  |
| invert | grep | 0.18 | 2% | 1.48 | 2.47 | 3.51 / 6.39 |
| invert | python | 0.52 | 1% | 8.34 | 21.67 | 1.19 / 1.14 |
| few | gem | 0.93 | 0% | 14.52 | 12.09 |  |
| few | grep | 0.12 | 1% | 0.62 | 2.47 | 7.75 / 23.43 |
| few | python | 0.44 | 2% | 6.19 | 21.62 | 2.12 / 2.35 |
| many | gem | 6.23 | 1% | 87.69 | 48.61 |  |
| many | grep | 0.63 | 1% | 8.09 | 2.47 | 9.94 / 10.84 |
| many | python | 1.06 | 1% | 16.57 | 30.73 | 5.85 / 5.29 |
| src_literal | gem | 0.09 | 1% | 1.34 | 22.98 |  |
| src_literal | grep | 0.05 | 3% | 0.23 | 2.45 | 1.91 / 5.75 |
| src_literal | python | 0.08 | 2% | 1.04 | 49.67 | 1.08 / 1.29 |
| src_icase | gem | 0.62 | 1% | 8.55 | 23.03 |  |
| src_icase | grep | 0.07 | 2% | 0.45 | 2.45 | 9.14 / 18.98 |
| src_icase | python | 0.31 | 1% | 5.78 | 49.67 | 2.02 / 1.48 |

### jobqueue

| case | impl | wall s (median) | spread | G instr | peak RSS MB | gem/impl (wall / instr) |
|---|---:|---:|---:|---:|---:|---:|
| none | gem | 0.61 | 40% | 4.88 | 102.03 |  |
| none | python | 1.14 | 2% | 9.47 | 54.27 | 0.54 / 0.52 |
| none | elixir | 1.27 | 5% | 13.42 | 147.86 | 0.48 / 0.36 |
| none | elixir_s1 | 1.28 | 1% | 10.26 | 120.91 | 0.48 / 0.48 |
| crash5 | gem | 0.71 | 4% | 5.78 | 110.38 |  |
| crash5 | python | 1.35 | 2% | 10.97 | 55.31 | 0.53 / 0.53 |
| crash5 | elixir | 1.33 | 1% | 15.39 | 149.06 | 0.53 / 0.38 |
| crash5 | elixir_s1 | 1.35 | 2% | 11.00 | 120.92 | 0.53 / 0.53 |
| hang5 | gem | 3.67 | 1% | 2.60 | 60.44 |  |
| hang5 | python | 3.71 | 1% | 4.93 | 42.11 | 0.99 / 0.53 |
| hang5 | elixir | 4.25 | 1% | 12.19 | 126.31 | 0.86 / 0.21 |
| hang5 | elixir_s1 | 4.26 | 1% | 10.11 | 107.48 | 0.86 / 0.26 |
| mixed | gem | 4.69 | 1% | 2.99 | 68.70 |  |
| mixed | python | 4.76 | 0% | 5.78 | 42.52 | 0.99 / 0.52 |
| mixed | elixir | 5.30 | 1% | 15.07 | 127.64 | 0.88 / 0.20 |
| mixed | elixir_s1 | 5.32 | 1% | 11.37 | 107.22 | 0.88 / 0.26 |
| storm | gem | 1.69 | 2% | 2.51 | 53.27 |  |
| storm | python | 1.68 | 3% | 4.81 | 41.84 | 1.01 / 0.52 |
| storm | elixir | 2.28 | 3% | 11.31 | 124.81 | 0.74 / 0.22 |
| storm | elixir_s1 | 2.33 | 1% | 9.22 | 107.23 | 0.72 / 0.27 |
| backlog | gem | 13.79 | 1% | 23.92 | 393.55 |  |
| backlog | python | 12.72 | 1% | 42.44 | 173.53 | 1.08 / 0.56 |
| backlog | elixir | 16.36 | 0% | 118.29 | 242.20 | 0.84 / 0.20 |
| backlog | elixir_s1 | 16.67 | 2% | 87.52 | 226.28 | 0.83 / 0.27 |

### jobqueue (program-measured; medians over runs)

| scenario | impl | jobs/s | p50 ms | p99 ms | tick lag ms | peak RSS MB | invariants ok |
|---|---:|---:|---:|---:|---:|---:|---:|
| none | gem | 69,686 | 5.00 | 22.00 | 0.00 | 102 | 5/5 |
| none | python | 26,917 | 0.00 | 3.00 | 0.00 | 54 | 5/5 |
| none | elixir | 50,890 | 97.00 | 114.00 | 1.00 | 147 | 5/5 |
| none | elixir_s1 | 49,504 | 68.00 | 82.00 | 1.00 | 120 | 5/5 |
| crash5 | gem | 50,505 | 0.00 | 14.00 | 1.00 | 110 | 5/5 |
| crash5 | python | 21,231 | 0.00 | 11.00 | 1.00 | 55 | 5/5 |
| crash5 | elixir | 43,763 | 87.00 | 195.00 | 1.00 | 149 | 5/5 |
| crash5 | elixir_s1 | 42,372 | 29.00 | 79.00 | 1.00 | 120 | 5/5 |
| hang5 | gem | 2,949 | 1,590 | 3,164 | 10.00 | 60 | 5/5 |
| hang5 | python | 2,983 | 1,515 | 3,053 | 3.00 | 42 | 5/5 |
| hang5 | elixir | 2,948 | 1,596 | 3,144 | 6.00 | 126 | 5/5 |
| hang5 | elixir_s1 | 2,945 | 1,596 | 3,141 | 1.00 | 107 | 5/5 |
| mixed | gem | 2,265 | 1,898 | 4,142 | 10.00 | 68 | 5/5 |
| mixed | python | 2,272 | 1,791 | 3,981 | 3.00 | 42 | 5/5 |
| mixed | elixir | 2,252 | 1,905 | 4,130 | 5.00 | 127 | 5/5 |
| mixed | elixir_s1 | 2,246 | 1,905 | 4,120 | 10.00 | 107 | 5/5 |
| storm | gem | 7,097 | 639.00 | 1,359 | 6.00 | 53 | 5/5 |
| storm | python | 7,598 | 505.00 | 1,170 | 3.00 | 41 | 5/5 |
| storm | elixir | 7,017 | 641.00 | 1,373 | 2.00 | 124 | 5/5 |
| storm | elixir_s1 | 6,835 | 651.00 | 1,398 | 1.00 | 107 | 5/5 |
| backlog | gem | 7,550 | 6,365 | 12,625 | 57.00 | 393 | 5/5 |
| backlog | python | 8,378 | 4,926 | 9,946 | 13.00 | 173 | 5/5 |
| backlog | elixir | 6,521 | 6,212 | 8,788 | 4.00 | 242 | 5/5 |
| backlog | elixir_s1 | 6,406 | 6,179 | 8,381 | 2.00 | 226 | 5/5 |

## Servers

One run each; the load generator runs on the same machine.

### bookmark_app vs Node (wrk)

| phase | impl | req/s | p50 ms | p99 ms | non-2xx | socket errors |
|---|---:|---:|---:|---:|---:|---:|
| burst_get_bookmarks | gem | 10,545.14 | 0.85 | 1.84 | 0 | 0 |
| burst_get_bookmarks | node | 16,795.26 | 0.53 | 1.15 | 0 | 0 |
| burst_get_index | gem | 87,982.80 | 0.10 | 0.26 | 0 | 0 |
| burst_get_index | node | 64,376.12 | 0.14 | 0.30 | 0 | 0 |
| soak_get_bookmarks | gem | 10,782 | 0.28 | 0.62 | 0 | 0 |
| soak_get_bookmarks | node | 16,807.20 | 0.19 | 0.42 | 0 | 0 |
| burst_post_bookmarks | gem | 225.41 | 17.82 | 51.37 | 0 | 0 |
| burst_post_bookmarks | node | 303.72 | 13.15 | 37.94 | 0 | 0 |

RSS over the whole run (MB):

| impl | min | peak | mean | end | end − min | growth over 2nd half |
|---|---:|---:|---:|---:|---:|---:|
| gem | 3.95 | 17.34 | 12.44 | 12.86 | 8.91 | -1.12 |
| node | 63.31 | 309.36 | 103.39 | 305.75 | 242.44 | 213.42 |

### mini_redis vs redis-server


basic:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| PING_INLINE | 177,304.97 | 180,505.41 | 1.02 | 0.85 | 0.48 |
| PING_MBULK | 186,915.88 | 185,185.17 | 0.99 | 0.29 | 0.49 |
| SET | 181,159.42 | 120,481.93 | 0.67 | 0.37 | 1.05 |
| GET | 183,823.52 | 129,701.68 | 0.71 | 0.29 | 0.73 |
| INCR | 184,501.84 | 115,473.45 | 0.63 | 0.34 | 1.81 |
| LPUSH | 186,915.88 | 120,772.95 | 0.65 | 0.33 | 1.18 |
| RPUSH | 184,842.88 | 122,850.12 | 0.66 | 0.34 | 1.19 |
| LPOP | 177,935.95 | 133,333.33 | 0.75 | 0.38 | 0.71 |
| RPOP | 182,149.36 | 132,100.39 | 0.73 | 0.36 | 0.73 |
| SADD | 184,162.06 | 118,906.06 | 0.65 | 0.32 | 1.38 |
| HSET | 180,180.17 | 119,474.31 | 0.66 | 0.40 | 1.11 |
| SPOP | 187,265.92 | 124,533.01 | 0.67 | 0.38 | 1.24 |
| LPUSH (needed to benchmark LRANGE) | 180,180.17 | 127,388.53 | 0.71 | 0.34 | 1.11 |
| LRANGE_100 (first 100 elements) | 111,358.58 | 44,483.99 | 0.40 | 0.39 | 1.74 |
| LRANGE_600 (first 600 elements) | 34,317.09 | 9,866.80 | 0.29 | 0.96 | 5.89 |
| MSET (10 keys) | 148,148.14 | 65,963.06 | 0.45 | 0.54 | 4.45 |

pipeline:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| PING_INLINE | 1,428,571.38 | 990,099 | 0.69 | 1.48 | 4.74 |
| PING_MBULK | 2,500,000 | 884,955.75 | 0.35 | 0.37 | 6.61 |
| SET | 1,515,151.50 | 537,634.38 | 0.35 | 0.62 | 5.82 |
| GET | 1,960,784.38 | 641,025.62 | 0.33 | 0.50 | 6.68 |
| INCR | 1,666,666.75 | 518,134.72 | 0.31 | 0.58 | 7.64 |
| LPUSH | 1,754,386 | 520,833.34 | 0.30 | 0.54 | 7.12 |
| RPUSH | 1,923,076.88 | 609,756.06 | 0.32 | 0.51 | 6.73 |
| LPOP | 1,666,666.75 | 704,225.31 | 0.42 | 0.60 | 2.39 |
| RPOP | 1,818,181.88 | 699,300.69 | 0.38 | 0.53 | 2.37 |
| SADD | 1,538,461.62 | 543,478.25 | 0.35 | 0.72 | 6.93 |
| HSET | 1,369,863 | 512,820.53 | 0.37 | 0.75 | 5.01 |
| SPOP | 1,315,789.50 | 621,118 | 0.47 | 0.90 | 2.95 |
| LPUSH (needed to benchmark LRANGE) | 1,754,386 | 510,204.09 | 0.29 | 0.54 | 6.76 |
| LRANGE_100 (first 100 elements) | 241,545.89 | 58,582.31 | 0.24 | 3.10 | 17.98 |
| LRANGE_600 (first 600 elements) | 28,376.85 | 10,576.42 | 0.37 | 19.74 | 78.53 |
| MSET (10 keys) | 272,479.56 | 116,009.28 | 0.43 | 4.62 | 15.18 |

clients:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| SET | 165,289.25 | 77,881.62 | 0.47 | 12.36 | 21.76 |
| GET | 161,290.33 | 78,431.38 | 0.49 | 15.66 | 23.95 |

keyspace_fill:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| SET | 1,197,604.88 | 447,227.19 | 0.37 | 1.08 | 15.85 |

keyspace_get:

| test | redis rps | gem rps | gem/redis | redis p99 ms | gem p99 ms |
|---|---:|---:|---:|---:|---:|
| GET | 176,366.86 | 125,156.45 | 0.71 | 0.50 | 0.72 |

Memory (RSS MB and DBSIZE after each phase):

| point | redis MB | redis keys | gem MB | gem keys |
|---|---:|---:|---:|---:|
| after basic | 27.36 | 163,255 | 133.84 | 163,250 |
| after pipeline | 31.27 | 163,333 | 182.84 | 163,234 |
| after 1000 clients | 28.77 | 63,125 | 329.58 | 63,252 |
| 1M keys of 100 B | 124.39 | 632,301 | 341.70 | 632,397 |
| 1M keys with a 2 s TTL | 63.00 | 0 | 865.20 | 631,825 |
| 3 s later | 63.02 | 0 | 865.45 | 523,347 |
| 6 s later | 63.02 | 0 | 865.77 | 343,667 |
| 9 s later | 63.02 | 0 | 866.09 | 138,787 |

Pub/sub fan-out:

| case | impl | publish/s | delivered/s | seconds |
|---|---:|---:|---:|---:|
| 1 subs × 50000 msgs | redis | 69,236 | 69,135 | 0.72 |
| 1 subs × 50000 msgs | gem | 29,174 | 29,153 | 1.72 |
| 100 subs × 2000 msgs | redis | 77,455 | 7,604,213 | 0.03 |
| 100 subs × 2000 msgs | gem | 2,765 | 276,006 | 0.72 |
| 1000 subs × 200 msgs | redis | 10,395 | 9,187,412 | 0.02 |
| 1000 subs × 200 msgs | gem | 192.00 | 192,058 | 1.04 |

### stomp_broker (one fresh broker per phase)

| phase | harness | RSS MB peak / end | broker |
|---|---:|---:|---:|
| fanout | publish_msgs_per_s=30,737.80, delivered_msgs_per_s=12,043.50, fanout_elapsed_s=0.42, median_first_msg_latency_s=0.02, p100_first_msg_latency_s=0.35, complete_subs=1,000, n_errors=0 | 183 / 177 | alive_at_phase_end=yes  killed-by-driver |
| slow | msgs_sent=313,704, publish_msgs_per_s=20,911.50, fast_subs_min=984, slow_subs_max=97, n_errors=0 | 754 / 750 | alive_at_phase_end=yes  killed-by-driver |
| queue | publish_msgs_per_s=41,992.50, total_elapsed_s=0.44, delivered_total=10,000, fairness_ratio=1.00, n_publisher_errors=0 | 8 / 8 | alive_at_phase_end=yes  killed-by-driver |
| soak | msgs_sent=1,500, delivered_total=300,000, expected_total=300,000, min_received=1,500, n_errors=0 | 251 / 175 | alive_at_phase_end=yes  killed-by-driver |
