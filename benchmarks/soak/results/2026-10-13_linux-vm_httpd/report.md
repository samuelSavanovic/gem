# Soak run

```
date: 2026-10-13T16:35:01Z
gem commit: 1ace345 (dirty)
system: Linux 6.18.44-fc-v114 x86_64
cpu: Intel(R) Xeon(R) Processor @ 2.10GHz, 4 cores
memory: 15 GB
python: Python 3.13.16
targets: httpd
duration per target: 3600 s, sample every 30 s, guard RSS 4096 MB
```

| target | verdict |
|---|---|
| httpd | PASS |

## httpd

- **PASS** ran to the end: load: deadline, 1h00m of 1h00m; server at the end: alive
- **PASS** no errors: 0 errors
- **PASS** memory steady: RSS 63 MB early, 63 MB late, peak 68 MB; second half -0.3 MB (-0.6 MB/h), limit 10.0 MB
- **PASS** fds steady: 694 open early, 696 late, max 1008, limit +10
- **PASS** throughput steady: canaries 2/s early, 2/s late, lowest interval 2/s, limit ×0.9
- **PASS** CPU steady: 13.4% early, 13.1% late, limit ×1.5 or +10 points
- **PASS** canary_p99_ms steady: 45.94 ms early, 45.82 ms late (×1.00), worst interval 224.2 ms
- **PASS** probe_ms steady: 1.26 ms early, 1.08 ms late (×0.86), worst interval 44.8 ms
- **PASS** no crashes in std/http: 0 crashes
- **PASS** no lost connections: 0 connections ended as lost (their close never reached the recorder)
- **PASS** recorder kept up: 0 events dropped, largest backlog 408 messages
- **PASS** settled after the load: last sample: 0 connections, 2 resources open (limit 2), 9 fds (limit 11: 9 at the start, +2), 5 processes, arena 9.1 MB; at the peak 999 connections, 1004 processes, 1008 fds, arena 81 MB

Totals over 1h00m: canaries 7,167, scans 71,648, malformed 17,883, garbage 17,898, bigheads 360, bodies 3,550, pipelines 3,529, nonreaders 714, slowloris_done 17,910, idle_opened 215,058, crowd_opened 244,707, refused_crowd 233,206, burst_conns 34,800, burst_refused 25,779, refused_during_burst 714, resets 72,000, halfcloses 7,059.

```
RSS MB           ▁▁▅▆▇▆▆█▇▇▆█▅▇█▅▇▇▅▇▆▇█▆▇▇▆▆▅▇▇▆▇▇▆▇▅▇▆▆▆▇▅█▆▇█▆
fds              ▁▁▁▁▁▇█▁██▁█▁██▁██▁█▁██▁██▁█▁██▁██▁█▁██▁██▁█▁██▁
CPU %            ▂▂▆▄▄▇██▅▄▇▃▄▃▄▅▅▅▅▆▃▄▃▄▆▄▅▆▆▄▃▅█▄▂▄▁▆▄▅▃▃▄▄▃▃▃▄
canary_p99_ms    ▁▆▁▆▁█▆▁▅▅▁▇▁▆▆▁▆▇▁▆▁██▁▅▅▁█▁▅▅▁█▆▁▅▁▆▇▂▆▇▁▆▁▆▆▁
probe_ms         ▁▁▁▁▁▁▁▁▁▁▁▁▁▄▁▁▁▂▁▁▁▁▁▁▁▁▁▄▁▁▁▁▁▁▁▂▁▁▄▁▁▂▁▁▁▁▁█
```

httpd database: connections by end reason: idle 224,355, done 80,271, closed 62,167, rejected 24,674, timeout 17,800, write_failed 704; requests by status: 200 3,179,870, 404 67,925, 400 18,954, 413 2,333, 505 2,015, 501 1,012, 431 360; caps hit: max_per_ip 232,128, max_connections 29,692; 24,674 bad_input rows; resets over 10 ms 1, over 100 ms 0; spawns refused 0.

