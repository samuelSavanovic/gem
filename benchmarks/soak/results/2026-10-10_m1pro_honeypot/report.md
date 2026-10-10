# Soak run

```
date: 2026-10-10T16:35:02Z
gem commit: e5d8ae8 (dirty)
system: Darwin 27.0.0 arm64
cpu: Apple M1 Pro, 10 cores
memory: 16 GB
python: Python 3.14.5
targets: honeypot
duration per target: 3600 s, sample every 30 s, guard RSS 4096 MB
```

| target | verdict |
|---|---|
| honeypot | PASS |

## honeypot

- **PASS** ran to the end: load: deadline, 1h00m of 1h00m; server at the end: alive
- **PASS** no errors: 0 errors
- **PASS** memory steady: footprint 457 MB early, 467 MB late, peak 489 MB; second half +5.4 MB (+10.7 MB/h), limit 23.4 MB
- **PASS** fds steady: 5066 open early, 5066 late, max 5081, limit +10
- **PASS** throughput steady: canaries 2/s early, 2/s late, lowest interval 2/s, limit ×0.9
- **PASS** CPU steady: 37.2% early, 35.6% late, limit ×1.5 or +10 points
- **PASS** session_p99_ms steady: 77.40 ms early, 76.98 ms late (×0.99), worst interval 97.6 ms
- **PASS** probe_ms steady: 23.06 ms early, 20.74 ms late (×0.90), worst interval 46.8 ms
- **PASS** no session crashes: 0 crashes
- **PASS** no lost sessions: 0 sessions ended as lost (their end never reached the recorder)
- **PASS** recorder kept up: 0 events dropped, largest backlog 98 messages
- **PASS** settled after the load: last sample: 0 sessions, 2 resources open (limit 2), 9 fds (limit 11: 9 at the start, +2), 6 processes, arena 12.4 MB; at the peak 5071 sessions, 5069 processes, 5078 fds, arena 464 MB

Totals over 1h00m: canaries 7,200, bots 17,991, garbage 18,000, iac 4,921, long_lines 121, drips_done 250, idle_opened 300,000, resets 72,001, halfcloses 7,201, nonreaders 717.

```
footprint MB     ▁▂▄▅▇█▂▄▁▃▄▆▇█▃▄▁▂▃▆▇█▃▅▁▃▄▆▇█▂▅▁▂▄▆▇█▂▅▁▃▄▆▇█▃▅
fds              ▅▂▃▅▁▁▂▂▂▂▂▆▁▂▅▂▁▂▁▃▃▁▁▂▂▁▁▂▁▂▂▂█▂▂▁▂▂▂▂▆▁▂▂▂▁▂▂
CPU %            ▅█▇▄▃▃▃▂▂▂▂▂▂▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
session_p99_ms   ▃▃▁▃▁▆▃▅▂▃▂▆▆▄▂▃▇▁▂▃▂▆▄█▅▁▁▅▂▅▅▄▇▆▃▄▂▄▃▆▃▅▁▁▃▂▆▅
probe_ms         ▂▃▁▃▃▅▂▆▄▄▅▃▃▄▃▄▇▆▄▃▄▅▂▂▅▄▄▅▃▄▃▂▄▆█▄▂▄▄▅▃█▃▂▄▄▄▄
```

Honeypot database: sessions by end reason: idle 295,000, closed 53,411, exit 7,201, max_commands 2,360, write_failed 2,003, max_time 250, max_bytes 121; caps hit: subneg_too_long 2,782, line_too_long 2,561, max_commands 2,360, input_truncated 1,473, max_time 250, max_bytes 121; 350,958 input rows; resets over 10 ms 0, over 100 ms 0; spawns refused 0.

