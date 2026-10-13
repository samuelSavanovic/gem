# Telnet honeypot: plan

A telnet honeypot written in Gem, run for weeks on a cheap VPS with a public IPv4 address. It has two purposes:

1. Capture what botnets try (credentials, commands, malware URLs).
2. **A real-world stress test of Gem.** Bots send garbage, wrong protocols, slow drips and connection floods. The goal is to find runtime and std bugs, memory or fd leaks, and perf cliffs under hostile, long-lived load. Crashes are signal, not noise.

Telnet only; SSH is out of scope. Port 80 on the same host is `examples/httpd`, a std/http site run as its own service, not part of the honeypot.

## Non-negotiables

- **Pure simulation.** Never execute anything, and never make an outbound connection because of attacker input. `wget`/`curl`/`tftp` only log the URL and print fake output.
- **Hard caps on everything** (see Limits). A cap that is hit is a logged event.
- **Don't hide crashes.** No `pcall` that swallows an error in session logic (the one around a whole session only records the trace and raises the error again). Session processes may crash, and each crash is recorded with its reason, stack trace and the session's raw input bytes, which become the replay corpus.
- Follows `docs/BEST_PRACTICES.md`.

## Gem prerequisites

| Gap | Today | Needed |
|---|---|---|
| Peer address | done: `tcp_peer(sock)` returns `{ip, port}`, or `nil` once the peer has reset | per-IP caps and the `ip` column; the acceptor treats `nil` as a connection already gone |
| Socket ownership | done: sockets and sqlite handles are owned resources (SPEC "Owned Resources", `docs/design/process_owned_resources.md`); a session takes its socket over with `claim(sock)`, and it closes when the session exits for any reason | `process_info(acceptor).resources` and `GEM_DIAG=2` show a session that forgot its claim; `tests/check_socket_leak.sh` runs this layout |
| Crash data | done in the honeypot: a `DOWN` message carries only the error message, so the session sends its trace to the recorder under one `pcall` and raises the error again; it sends its raw input as it reads it, which the recorder stores within a second | crash rows with the reason, the trace and the input that led to them |
| Runtime stats | done: `runtime_stats()` returns the reset totals, process counts, arena memory and open resources; `process_info(pid).memory` is one process's arena | the sampler records them alongside RSS and fds |

Socket ownership is in place, so the honeypot closes nothing on `DOWN`: each session claims its socket.

## Process layout

- **acceptor**: a loop on `tcp_accept` over the listener main opens on the configured port (2323; in production port 23 is redirected to it), sending `{sock, peer}` to the registry. `tcp_accept` takes no timeout, so this process does nothing else.
- **registry**: owns the global and per-IP connection counts, applies the caps (closes and logs when over), spawns and monitors one session per connection, and records abnormal exits. It claims each socket it takes from its mailbox; it and the acceptor restart together, so a registry crash closes the sockets not yet handed to a session, those still in its mailbox included. Sessions are temporary and never restarted, so no `dynamic_supervisor`.
- **session** (one per connection): claims its socket first (`claim(sock)`), so any exit closes it. Runs telnet negotiation, the fake login and the fake shell. Streams its raw input to the recorder. Exits on close, idle timeout, max session length or a byte cap.
- **recorder**: the only process that touches sqlite. Batches inserts in a transaction (every 200 events or 1 s). Stores every session's raw input (the first 64 KB) as it arrives, so it survives a crash of the session and, but for the last second, of the whole program. Past a backlog of 10,000 messages it drops all but the events that open and end a session's row and the metrics, and logs how many it dropped.
- **metrics sampler**: every minute records RSS (`/proc/self/status`, Linux only), open fds (`list_dir("/dev/fd")`), `runtime_stats()` (live processes, arena memory, open and ownerless resources, reset time, resets over 10 and 100 ms and refused spawns since the last sample), live sessions and their IPs, the recorder's backlog and dropped events, the uptime and the Gem commit. Crashes and cap hits are counted from their own tables.
- **dashboard** (later, maybe never): `sqlite_query` and `sqlite_exec` run on the scheduler thread, so heavy aggregate queries in-process would stall every session. Either the recorder keeps rollup tables, or the dashboard is a separate OS process reading the WAL database. Until then the `sqlite3` CLI over SSH is the dashboard.
- A top-level supervisor over the recorder, the sampler and a `one_for_all` front supervisor of the registry and the acceptor (README.md, "Design").

## Telnet

- Strip IAC sequences incrementally (a sequence can be split across reads): WILL/WONT/DO/DONT + option, SB … IAC SE with a capped length, IAC IAC as a literal 255.
- On connect send WILL ECHO and WILL SUPPRESS-GO-AHEAD. Many bots ignore negotiation entirely.
- CR LF, CR NUL, a lone LF and a lone CR each end a line. Cap the line length (about 4 KB) and log when it's hit.
- Input is arbitrary bytes: NULs and invalid UTF-8 are expected. Store raw input as bytes.
- Every `tcp_write` takes a timeout, for bots that never read.

## Fake device

A cheap BusyBox IoT box: `login:`, `Password:`, then `# `. Login is accepted on the Nth attempt (configurable) or for a fixed share of username/password pairs, and every username/password pair is recorded.

Canned responses first for:

- `enable`, `system`, `shell`, `sh`, `linuxshell`
- `cat /proc/cpuinfo`, `cat /proc/mounts`, `uname -a`, `ps`, `ls`, `cd`
- `echo -e '\x..'`: decode the escapes and log the bytes
- `wget`/`curl`/`tftp`: log the URL, print fake progress
- `chmod`, `./binary`: fake failure or silence
- `/bin/busybox NAME` prints `NAME: applet not found`
- anything else: `sh: X: not found`

Commands chained with `;`, `&&`, `||` and `|` are split roughly. The first days of real traffic decide what to fake next.

## Storage (sqlite)

- `sessions`: id, pid, ip, port, start/end time, end reason (closed, idle, exit, write_failed, max_time, max_bytes, max_logins, max_commands, crash, lost, week_end), bytes in/out, login succeeded
- `input`: session_id, time, raw bytes (BLOB), one row per chunk read
- `logins`: session_id, username, password, ok, time
- `commands`: session_id, raw line, time
- `urls`: session_id, url, via, time
- `echoes`: session_id, the bytes an `echo -e` decoded, time
- `crashes`: session_id, reason, trace (none for a kill), time
- `cap_events`: session_id or ip, cap, detail, time
- `metrics`: time, uptime, rss, fds, procs, arena memory, resources open and ownerless, live sessions and IPs, reset time and resets over 10 and 100 ms, refused spawns, recorder backlog and dropped events, gem commit

One database file per week, deleting files past the retention limit. IPs are stored in full for analysis (and printed in full by `--log`); anything published shows them only truncated or aggregated. Anything shown in a browser is escaped and URLs are never clickable: every stored string is attacker-controlled.

## Limits

All configurable, all logged when hit: max concurrent sessions (global and per IP), idle timeout (about 60 s), max session length (about 10 min), max line length, max bytes in per session, max subnegotiation length, max login attempts, max commands per session, recorder backlog. README.md ("Limits") has the defaults.

## Testing

- `test.gem` in this directory (std/test) covers the pure parts (the IAC parser including sequences split across reads, the line splitter, the command responses) and drives the server over TCP: caps, crashes, a recorder that restarts, the metrics.
- A replayer (not written yet) sends stored crash input back over TCP. Crashes that reproduce become numbered examples or `docs/KNOWN_BUGS.md` entries.
- A hostile load generator under `benchmarks/soak/` runs the local fuzz hour (`TARGETS=honeypot benchmarks/soak/run.sh`): random bytes, IAC floods, a 10 MB line with no newline, 1-byte-per-second drips, 5,000 idle connections, connect and reset, half-close, clients that never read, Mirai-style scripts and a checked canary. RSS and fds must be flat once the connections close. It passes on macOS and on Linux (`benchmarks/README.md`, "Recorded runs"); after the load RSS keeps the peak's process table slots, about 22 KB per process at the peak (OPTIMIZATIONS.md, "Per-process footprint").
- Success in production is defined before deploy: see Success criteria.

## Deployment

`deploy/` has the systemd unit, the nftables ruleset, the flags and the health query, and `deploy/README.md` the steps.

- A dedicated VPS running nothing but this and `examples/httpd`, each as its own sandboxed `DynamicUser` service. Gem's runtime is C, so a memory-safety bug could mean remote code execution on the box.
- Port 23 redirected to 2323 with nftables; the program runs unprivileged.
- `--max-sessions` sized to the VM's memory: an idle session holds about 76 KB of arena, but one that keeps sending holds up to about 1.1 MB, since its loop resets only once it has allocated 1 MB (OPTIMIZATIONS.md, "A loop keeps its high-water memory"), so the default 1,000 slow clients could take about 1 GB. The deploy's 4 GB VM keeps the default 1,000 with `MemoryMax=2G`; on a 1 GB VM, start near 300, with `MemoryMax` below the RAM.
- Egress default-deny in nftables (DNS, package updates, NTP and SSH replies only) plus the provider firewall: the "no outbound connections" rule is enforced by the host, not only by the code.
- systemd with restart on failure (a restart is a finding too), `DynamicUser`, `NoNewPrivileges`, `ProtectSystem=strict`, `MemoryMax`, `LimitNOFILE`, `TasksMax`.

### Hosting requirements

- A dedicated public IPv4 address, not IPv6-only or behind NAT, with no proxy or always-on DDoS layer in front (it would hide source IPs or drop the floods worth recording).
- Raw inbound TCP on any port, port 23 included.
- A KVM VM with root (not OpenVZ/LXC): own nftables rules, systemd, raised fd limits.
- At least 1 vCPU, 1 GB RAM (Gem is compiled on the box), 10 GB disk.
- Monthly billing with no long commitment; terms that don't forbid honeypots; an EU location.

The host: a Hetzner CX23 in Helsinki (2 x86 vCPUs, 4 GB RAM, 40 GB disk), bought 2026-10-11, running Ubuntu 26.04. The other candidate was BuyVM Slice 1024 in Luxembourg (out of stock). Rejected: OVH VPS-1 (anti-DDoS can't be turned off) and netcup (cheap only on a 24-month term).

## Success criteria

**Before port 23 opens**: an 8-hour soak on the VPS itself, with the deploy's session cap and memory limit (`deploy/README.md`, step 2), passes every check of its report.

**The first 7 days of real traffic**, by `deploy/health.sql` over the metrics rows and by the host:

| Criterion | Measured as | Must hold |
|---|---|---|
| No restarts | `restarts` (`uptime_ms` drops), `systemctl show honeypot -p NRestarts` | 0, but for those of our own upgrades |
| No fd or resource leak | `fds_extra` (the day's median of the fds beyond one per live session), `res_extra` (resources beyond one per live session), `ownerless` | `fds_extra` at most 2 above the first day's, `res_extra` at most 2, `ownerless` at most 1 (the listener), on every day |
| Memory flat | `rss_max_mb` by day | under 1 GB (half the unit's `MemoryMax`) on every day, and day 7's at most 25% above day 2's |
| No stalls | `resets_100ms` | 0 |
| Nothing lost | `lost`, `dropped`, `spawn_refused` | 0, unless a flood explains it (`cap_events`), which is then a finding |
| No outbound connection | `nft list counter inet honeypot honeypot_egress` | 0 packets |

Crashes don't fail the week: they are what the honeypot is for. Each one is triaged before the week is called: its stored input replayed (by hand with `nc` until the replayer exists), and the bug fixed or added to `docs/KNOWN_BUGS.md`. A criterion that fails is a finding the same way: what caused it goes to KNOWN_BUGS.md or OPTIMIZATIONS.md, then the week starts again.

## Phases

1. `tcp_peer` builtin (done).
2. Process-owned sockets (done: `docs/design/process_owned_resources.md`).
3. IAC parser, line splitter, fake login and shell on :2323, with `test.gem` (done: see README.md).
4. Crash-data path, recorder and schema (done: see README.md).
5. Limits, metrics sampler, local fuzz hour (done: see README.md).
6. Deploy on :23 (`deploy/`: the unit, the firewall and the health query; the soak on the VPS, `benchmarks/soak/results/2026-10-11_cx23_honeypot`, stopped at 6 h 36 m of 8 h for go-live with every steady-state check passing; live since 2026-10-11: done), watch the first day's traffic, extend the fake commands.
7. Optional: rollups or a dashboard. (Port 80 is `examples/httpd`, a separate program and service on the same host.)
