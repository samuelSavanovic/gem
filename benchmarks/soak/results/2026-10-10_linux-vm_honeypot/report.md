# Soak run

```
date: 2026-10-10T19:15:11Z
gem commit: 5738a9f
system: Linux 6.18.44-fc-v114 x86_64
cpu: Intel(R) Xeon(R) Processor @ 2.10GHz, 4 cores
memory: 15 GB
python: Python 3.13.16
targets: honeypot
duration per target: 3600 s, sample every 30 s, guard RSS 4096 MB
```

| target | verdict |
|---|---|
| honeypot | PASS |

## honeypot

- **PASS** ran to the end: load: deadline, 1h00m of 1h00m; server at the end: alive
- **PASS** no errors: 0 errors
- **PASS** memory steady: RSS 303 MB early, 320 MB late, peak 342 MB; second half +6.6 MB (+13.2 MB/h), limit 16.0 MB
- **PASS** fds steady: 5072 open early, 5072 late, max 5080, limit +10
- **PASS** throughput steady: canaries 2/s early, 2/s late, lowest interval 2/s, limit ×0.9
- **PASS** CPU steady: 34.6% early, 32.0% late, limit ×1.5 or +10 points
- **PASS** session_p99_ms steady: 66.19 ms early, 60.27 ms late (×0.91), worst interval 137.1 ms
- **PASS** probe_ms steady: 5.63 ms early, 6.44 ms late (×1.14), worst interval 294.5 ms
- **PASS** no session crashes: 0 crashes
- **PASS** no lost sessions: 0 sessions ended as lost (their end never reached the recorder)
- **PASS** recorder kept up: 0 events dropped, largest backlog 49 messages
- **PASS** settled after the load: last sample: 0 sessions, 2 resources open (limit 2), 9 fds (limit 11: 9 at the start, +2), 6 processes, arena 8.8 MB; at the peak 5071 sessions, 5077 processes, 5074 fds, arena 362 MB

Totals over 1h00m: canaries 7,200, bots 17,990, garbage 17,999, iac 4,862, long_lines 120, drips_done 250, idle_opened 300,000, resets 72,000, halfcloses 7,200, nonreaders 714.

```
RSS MB           ▁▂▃▅▆▇▃▅▂▄▄▆▇█▄▅▃▄▅▆▇█▄▅▃▄▅▇▇█▄▆▃▄▅▇██▄▅▃▄▅▇▇█▄▆
fds              ▁█▄▃▂▂▂▂▁▂▁▁▁▁▁▁▁▁▁▁▂▂▁▁▂▁▁▁▁▁▁▂▁▁▁▁▁▁▂▂▁▁▁▁▁▁▁▁
CPU %            ▁█▇▆▅▅▄▃▃▃▃▂▂▂▂▁▂▁▂▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁
session_p99_ms   ▂▇▅▆▃▄▃▅▂▂▃▄▃▅▁▆▅▄▂▄▃▅▆▂▅▄▄▃▂▄▄▆▁▁▅▅▂▃▂▃▄▅▃█▆▂▁▃
probe_ms         ▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▂▁▁▁▁▁▁▂▁▁▁▁▂▁▁▁▁▁▁█
```

Honeypot database: sessions by end reason: idle 295,000, closed 53,628, exit 7,200, max_commands 2,337, write_failed 1,741, max_time 250, max_bytes 120; caps hit: subneg_too_long 2,747, line_too_long 2,585, max_commands 2,337, input_truncated 2,161, max_time 250, max_bytes 120; 358,668 input rows; resets over 10 ms 100, over 100 ms 0; spawns refused 0.

