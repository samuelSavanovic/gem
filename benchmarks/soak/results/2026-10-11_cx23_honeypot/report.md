# Soak run

```
date: 2026-10-11T13:47:42Z
gem commit: 395f5aa
system: Linux 7.0.0-38-generic x86_64
cpu: AMD EPYC-Rome Processor, 2 cores
memory: 3 GB
python: Python 3.14.4
targets: honeypot
duration per target: 28800 s, sample every 30 s, guard RSS 2048 MB
HONEYPOT_ARGS: --idle 800
HONEYPOT_SERVER_ARGS: --max-sessions 1000
```

| target | verdict |
|---|---|
| honeypot | STOPPED |

## honeypot

- **STOPPED** ran to the end: load: SIGTERM, 6h36m of 8h00m; server at the end: alive
- **PASS** no errors: 0 errors
- **PASS** memory steady: RSS 84 MB early, 85 MB late, peak 93 MB; second half +0.6 MB (+0.2 MB/h), limit 10.0 MB
- **PASS** fds steady: 872 open early, 872 late, max 879, limit +10
- **PASS** throughput steady: canaries 2/s early, 2/s late, lowest interval 2/s, limit ×0.9
- **PASS** CPU steady: 12.7% early, 12.5% late, limit ×1.5 or +10 points
- **PASS** session_p99_ms steady: 52.33 ms early, 55.05 ms late (×1.05), worst interval 106.3 ms
- **PASS** probe_ms steady: 2.63 ms early, 2.61 ms late (×0.99), worst interval 55.9 ms
- **PASS** no session crashes: 0 crashes
- **PASS** no lost sessions: 0 sessions ended as lost (their end never reached the recorder)
- **PASS** recorder kept up: 0 events dropped, largest backlog 45 messages
- **SKIPPED** settled after the load: not judged: the run was stopped before the settle period; last sample: 849 sessions, 851 resources open (limit 2), 858 fds (not judged: no sample before the load), 855 processes, arena 92.0 MB; at the peak 870 sessions, 868 processes, 878 fds, arena 93 MB

Totals over 6h36m: canaries 47,605, bots 119,003, garbage 119,012, iac 31,754, long_lines 794, drips_done 1,983, idle_opened 316,854, resets 476,049, halfcloses 47,605, nonreaders 4,755.

```
RSS MB           █▁▁▂▁▂▁▁▁▁▁▁▁▁▂▁▂▁▁▁▁▁▂▃▃▂▁▂▁▂▂▃▄▂▂▁▂▂▂▂▂▂▂▂▂▂▂▃
fds              █▁▂▃▁▂▁▂▃▃▃▁▁▄▁▂▁▁▁▁▁▁▁▃▁▁▅▂▁▂▁▁▁▁▁▁▁▂▁▄▁▁▄▁▂▃▂▁
CPU %            ▆▆██▆▆▇▆█▇▆▅▅▄▅▅▅▃▃▄▃▃▃▃▁▂▂▁▁▁▁▁▁▂▃▄▂▃▄▅▄▄▃▄▅▄▅▅
session_p99_ms   ▃▁▅▅▅▁▄▄▄▄▄▁▃▅▂▄▃▁▁▄▁▃▅▂█▅▂▂▁▃▃▃▁▆▂▃▂▃▃▇▃▃▃▃▃▅▄▂
probe_ms         ▁▂▁▂▆▁▁▅▁▁▅▁▁▁▄▂▄▃▃▃▁▁▁▁▂▁▁▁▁▁▁▁▁▁▁▅▁▁▁▁▁▁▄▁▁▁▄█
```

Honeypot database: sessions by end reason: closed 329,619, idle 316,061, write_failed 63,856, exit 47,605, max_commands 15,850, max_time 1,934, max_bytes 794; caps hit: subneg_too_long 18,768, line_too_long 16,654, max_commands 15,850, input_truncated 14,674, max_time 1,934, max_bytes 794; 2,377,733 input rows; resets over 10 ms 0, over 100 ms 0; spawns refused 0.

