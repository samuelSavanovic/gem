# Soak run

```
date: 2026-10-10T22:41:01Z
gem commit: ddb8cbb
system: Linux 6.18.44-fc-v114 x86_64
cpu: Intel(R) Xeon(R) Processor @ 2.10GHz, 4 cores
memory: 15 GB
python: Python 3.13.16
targets: honeypot
duration per target: 14400 s, sample every 30 s, guard RSS 4096 MB
```

| target | verdict |
|---|---|
| honeypot | FAIL |

## honeypot

- **FAIL** ran to the end: load: (load still running or killed hard), 2h05m of 4h00m; server at the end: (not recorded)
- **PASS** no errors: 0 errors
- **PASS** memory steady: RSS 303 MB early, 316 MB late, peak 345 MB; second half -1.0 MB (-0.9 MB/h), limit 15.8 MB
- **PASS** fds steady: 5072 open early, 5070 late, max 5077, limit +10
- **PASS** throughput steady: canaries 2/s early, 2/s late, lowest interval 2/s, limit ×0.9
- **PASS** CPU steady: 33.5% early, 30.4% late, limit ×1.5 or +10 points
- **PASS** session_p99_ms steady: 55.47 ms early, 64.92 ms late (×1.17), worst interval 1298.9 ms
- **PASS** probe_ms steady: 6.41 ms early, 7.17 ms late (×1.12), worst interval 128.0 ms
- **PASS** no session crashes: 0 crashes
- **PASS** no lost sessions: 0 sessions ended as lost (their end never reached the recorder)
- **PASS** recorder kept up: 0 events dropped, largest backlog 57 messages
- **FAIL** settled after the load: last sample: 5063 sessions, 5065 resources open (limit 2), 5072 fds (limit 11: 9 at the start, +2), 5069 processes, arena 396.6 MB; at the peak 5070 sessions, 5068 processes, 5078 fds, arena 359 MB

Totals over 2h05m: canaries 15,060, bots 37,640, garbage 37,649, iac 9,975, long_lines 251, drips_done 600, idle_opened 628,547, resets 150,600, halfcloses 15,060, nonreaders 1,500.

```
RSS MB           ▁▄▆▄▃▆▇▄▄▇█▅▅▇█▅▅█▄▅▆█▅▄▆█▅▄▇█▅▅██▅▅█▄▅▆█▅▃▇█▅▄▇
fds              ▁█▂▂▂▁▁▁▁▁▂▁▁▁▁▁▁▁█▂▁▁▂▁▁▂▁▂▁▁▁▁▁▁▁▁▂▁▁▁▁▁▁▁▁▁▁▄
CPU %            █▇▆▅▄▄▃▃▃▂▂▂▂▂▂▂▂▂▂▂▂▁▁▂▂▂▂▂▂▁▂▂▂▁▂▁▁▁▁▁▁▁▁▁▁▁▁▁
session_p99_ms   ▁▁▁▁▁▁▁▁█▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▂▁▁▁▁▁▁▁▁▁▁▁▁▁
probe_ms         ▁▁▁▁▁▂▁▁▂▂▁▃█▁▁▁▁▁▂▁▂▁▁▁▁▁▂▁▁▂▂▂▁▁▁▁▂▂▁▂▂▂▂▂▂▄▂▂
```

Honeypot database: sessions by end reason: idle 625,000, closed 101,698, exit 15,101, max_commands 5,095, (open) 5,064, write_failed 3,315, max_time 600, max_bytes 252; caps hit: subneg_too_long 5,962, line_too_long 5,314, max_commands 5,095, input_truncated 4,682, max_time 600, max_bytes 252; 754,062 input rows; resets over 10 ms 290, over 100 ms 0; spawns refused 0.

