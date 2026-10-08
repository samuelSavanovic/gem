# Soak run

```
date: 2026-10-08T08:25:04Z
gem commit: a6a1448 (dirty)
system: Darwin 27.0.0 arm64
cpu: Apple M1 Pro, 10 cores
memory: 16 GB
python: Python 3.14.5
targets: mini_redis
duration per target: 28800 s, sample every 30 s, guard RSS 4096 MB
```

| target | verdict |
|---|---|
| mini_redis | PASS |

## mini_redis

- **PASS** ran to the end: load: deadline, 8h00m of 8h00m; server at the end: alive
- **PASS** no errors: 0 errors
- **PASS** memory steady: RSS 129 MB early, 132 MB late, peak 156 MB; second half +4.9 MB (+1.2 MB/h), limit 10 MB
- **PASS** fds steady: 39 open early, 39 late, max 40, limit +10
- **PASS** throughput steady: ops 10,006/s early, 10,001/s late, lowest interval 3,813/s, limit ×0.9
- **PASS** CPU steady: 37.0% early, 44.3% late, limit ×1.5 or +10 points
- **PASS** lat_p99_ms steady: 4.06 ms early, 1.45 ms late (×0.36), worst interval 62.0 ms
- **PASS** deliver_p99_ms steady: 5.88 ms early, 5.09 ms late (×0.87), worst interval 50.2 ms
- **PASS** probe_ms steady: 0.58 ms early, 0.53 ms late (×0.90), worst interval 117.5 ms

Totals over 8h00m: ops 287,597,218, conns 576,000, published 5,759,812, delivered 28,805,873, sub_sessions 18,963.

```
RSS MB           ▅▆▅▁▄█▆▇█▆▄▅▇▆▃▇▅▆▁▂▄▆▆█▄▂▄▅▂▃▄▆▇▄▅▅▂▂▄▃▄▄▆▇▇▇█▇
fds              ▁▁▁▁▁▁▁▁▁▁▁█▁▁▁▁▁█▁█▁▁▁▁▁▁▁▁▁█▁█▁▁▁▁▁▁▁█▁▁▁▁▁▁▁▁
CPU %            ▄▄▄▅▆▆▇▇▇▅▇██████▄▅▇███▁▁▁▅▇▆▇▄███▇▆▆▇▇▁████████
lat_p99_ms       ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁█▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
deliver_p99_ms   ▁▁▁▂▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁█▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
probe_ms         ▁▁▁▁▁▁▁▁▁▁▂█▄▁▁▁▁▁▁▂▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
```

GEM_DIAG at exit:

    gem_diag: arena_resets=708707 copied=67574170560 scanned=966261689512 freed=1340045539760 time=1027.840s walk=481.273s full=18833 ret_resets=763 ret_copied=14688064 max=0.525s
    gem_diag: spawn_overflow=0 proc_hwm=1024 max_procs=262144

