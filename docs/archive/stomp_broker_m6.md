# STOMP broker: first-version load tests (archive)

Measurements and design notes from the first version of
`examples/stomp_broker` (May 2026, Apple Silicon macOS, broker and load
generator on the same machine). They describe the runtime of that time,
not the current one; the broker's current design and limits are in
`examples/stomp_broker/README.md`, and today's load numbers come from
`benchmarks/stomp/run.sh` (recorded runs under `benchmarks/baselines/`).

## Deep-copy cost of a fan-out `send`

`benchmarks/stomp/microbench_fanout_one.gem <k> <body_size> <n_msgs>`
spawns `k` drain processes and has the parent send `n_msgs` MESSAGE
frames (`{command, headers, body}` with a 3-entry headers table, wrapped
in a `{tag, frame}` message) to each of them, with no TCP involved; the
table below used `n_msgs` = 200. Every `send` deep-copies the message
into the receiver's arena, and a fan-out message to `k` subscribers costs
`k` sends.

| k   | body=64 µs/send | body=256 | body=1024 | body=4096 | µs per k-fanout msg @ body=1024 |
|----:|----------------:|---------:|----------:|----------:|--------------------------------:|
|  10 |               3 |        2 |         2 |         3 |                              20 |
|  50 |               2 |        2 |         2 |         2 |                             115 |
| 100 |               2 |        2 |         2 |         2 |                             225 |
| 200 |               2 |        2 |         2 |         2 |                             465 |
| 500 |               2 |        2 |         2 |         2 |                            1230 |
| 800 |               2 |        2 |         2 |         3 |                            2000 |

A send cost about 2 µs whatever the body size from 64 B to 1 KB: the
cost was the table structure, not the body bytes. A 4 KB body added
about 1 µs at k=800. At 1,000 subscribers one message cost about 2.5 ms
of the destination process's time, a ceiling of about 400 publishes a
second.

## Backpressure options for slow consumers

A slow subscriber's messages queue in its connection writer's mailbox.
The current broker disconnects a client that doesn't take a frame within
`write_timeout_ms` (README, "Known limits"). The policies considered for
dropping or throttling instead:

1. **Selective drop on the writer**: the writer discards queued
   deliveries past a length or byte budget. Cheapest; loses messages
   silently.
2. **`gen_server.call` instead of `cast` for the fan-out**: the
   destination waits for each writer to take the delivery. Real
   backpressure, but the slowest subscriber throttles the whole topic,
   the wrong default for STOMP.
3. **A per-subscriber policy chosen at SUBSCRIBE**: the client declares
   whether it tolerates drops (lossy) or wants the publisher held back
   (blocking). STOMP 1.2 has no such header, so this is an extension.
4. **The destination reads `process_info(pid).mailbox_len`** of each
   writer before fanning out and skips writers past a soft cap.

The preferred combination was 3 with 4 as its mechanism: the client
knows whether dropping is acceptable, and the mailbox length check
enforces it without a synchronous round trip per message.

## Scheduler poll fairness

Under sustained input a broker reader process was always runnable, and
the scheduler of that time called `poll` only when no process was
runnable, so a writer waiting for its socket to drain (`tcp_write` got
`EAGAIN`) was never woken: the slow subscriber of the slow-consumer test
stopped at about 131 messages (its kernel send buffer's worth of 4 KB
frames) however long the test ran, while its mailbox grew linearly.
Slowing the publisher to 72 msg/s let it drain at its own 5 msg/s.

The fix was a non-blocking `poll` over the fd waiters on every scheduler
pass while any process waited on an fd. It cost about 12% of the
publisher's rate in the slow-consumer test (2,535 → 2,239 msg/s) and
nothing measurable on a 500-subscriber fan-out cell (62k → 66k msg/s,
within noise). An epoll/kqueue readiness set, which makes the check
O(ready), was left for later (`docs/OPTIMIZATIONS.md`, "kqueue/epoll for
sockets").
