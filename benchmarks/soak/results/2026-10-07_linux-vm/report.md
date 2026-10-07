# Soak run

```
date: 2026-10-07T18:55:36Z
gem commit: 531563d
system: Linux 6.18.44-fc-v77 x86_64
cpu: Intel(R) Xeon(R) Processor @ 2.10GHz, 4 cores
memory: 15 GB
python: Python 3.13.16
targets: mini_redis stomp bookmark
duration per target: 3600 s, sample every 30 s, guard RSS 4096 MB
```

| target | verdict |
|---|---|
| mini_redis | PASS |

## mini_redis

- **PASS** ran to the end: load: deadline, 1h00m of 1h00m; server at the end: alive
- **PASS** no errors: 0 errors
- **PASS** memory steady: RSS 108 MB early, 108 MB late, peak 142 MB; second half -0.9 MB (-1.8 MB/h), limit 10 MB
- **PASS** fds steady: 36 open early, 36 late, max 37, limit +10
- **PASS** lat_p99_ms steady: 1.79 ms early, 1.79 ms late (×1.00), worst interval 3.0 ms
- **PASS** deliver_p99_ms steady: 3.28 ms early, 3.35 ms late (×1.02), worst interval 4.6 ms
- **PASS** probe_ms steady: 0.86 ms early, 1.23 ms late (×1.43), worst interval 11.2 ms

Totals over 1h00m: ops 36,012,961, conns 72,001, published 720,001, delivered 3,600,832, sub_sessions 2,436.

```
RSS MB           ▁█▆▄▄▅▆▆▄▅▅▆▆▆▄▄▅▆▆▇▃▅▅▅▆▇▄▄▅▆▆▄▅▆▆▆▆▃▄▅▆▇▄▅▅▆▆▆
fds              ▁▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅█▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅▅
CPU %            ▁▁▆▁▅▆▅▄▃▅▅▅▂▆▂▃▂▂▄█▄▆▇▇▅▃▁▃▂▃▄▆▄▃▁▁▂▃▆▁▄▆▅▆▁▁▁▅
lat_p99_ms       ▂▂▄▁▂▂▂▁▁▁▂▁▁▁▁▁▁▂▁█▂▂▂▂▆▁▁▁▂▁▁▂▁▁▁▁▁▃▇▂▁▃▁▂▂▁▁▂
deliver_p99_ms   ▁▆▅▄▄▅▄▄▃▄▃▄▅▄▄▄▂▇▃█▅▅▆▅▇▄▅▄▃▄▄▄▅▄▄▅▂▅▇▆▃▅▅▆▄▄▃▆
probe_ms         ▂▁▁▂▃▂▁▁▁▁▁▁▂▁▂▁▃▁▂▂▁▂▁▁▁▁▁▂▁▁▁▁▁▁▁▁▁▁▂▁▂█▂▂▁▂▁▁
```

GEM_DIAG at exit:

    gem_diag: arena_resets=88782 copied=8420856656 scanned=120471638520 freed=152347274096 time=93.222s walk=46.606s full=2440 ret_resets=98 ret_copied=12624496 max=0.087s
    gem_diag: spawn_overflow=0 proc_hwm=1024 max_procs=262144

