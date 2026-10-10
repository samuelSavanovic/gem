# Telnet honeypot: plan

A telnet honeypot written in Gem, run for weeks on a cheap VPS with a public IPv4 address. It has two purposes:

1. Capture what botnets try (credentials, commands, malware URLs).
2. **A real-world stress test of Gem.** Bots send garbage, wrong protocols, slow drips and connection floods. The goal is to find runtime and std bugs, memory or fd leaks, and perf cliffs under hostile, long-lived load. Crashes are signal, not noise.

Telnet only; SSH is out of scope. An HTTP honeypot on :80 via std/http is an optional later phase.

## Non-negotiables

- **Pure simulation.** Never execute anything, and never make an outbound connection because of attacker input. `wget`/`curl`/`tftp` only log the URL and print fake output.
- **Hard caps on everything** (see Limits). A cap that is hit is a logged event.
- **Don't hide crashes.** No blanket `pcall` around session logic. Session processes may crash, and each crash is recorded with its reason, stack trace and the session's raw input bytes, which become the replay corpus.
- Follows `docs/BEST_PRACTICES.md`.

## Gem prerequisites

| Gap | Today | Needed |
|---|---|---|
| Peer address | done: `tcp_peer(sock)` returns `{ip, port}`, or `nil` once the peer has reset | per-IP caps and the `ip` column; the acceptor treats `nil` as a connection already gone |
| Socket ownership | done: sockets and sqlite handles are owned resources (SPEC "Owned Resources", `docs/design/process_owned_resources.md`); a session takes its socket over with `claim(sock)`, and it closes when the session exits for any reason | `process_info(acceptor).resources` and `GEM_DIAG=2` show a session that forgot its claim; `tests/check_socket_leak.sh` runs this layout |
| Crash data | a `DOWN` message carries only the error message; the trace goes to stderr and the raw input dies with the session | the session streams capped raw-input chunks to the recorder as it reads; traces come from the service's stderr log, matched by pid |
| Runtime stats | done: `runtime_stats()` returns the reset totals, process counts, arena memory and open resources; `process_info(pid).memory` is one process's arena | the sampler records them alongside RSS and fds |

Socket ownership is in place, so the honeypot closes nothing on `DOWN`: each session claims its socket.

## Process layout

- **acceptor**: a loop on `tcp_accept` over the listener main opens on the configured port (2323; in production port 23 is redirected to it), sending `{sock, peer}` to the registry. `tcp_accept` takes no timeout, so this process does nothing else.
- **registry**: owns the global and per-IP connection counts, applies the caps (refuses and logs when over), spawns and monitors one session per connection, and records abnormal exits. It claims each socket it takes from its mailbox; it and the acceptor restart together, so a registry crash closes the sockets not yet handed to a session, those still in its mailbox included. Sessions are temporary and never restarted, so no `dynamic_supervisor`.
- **session** (one per connection): claims its socket first (`claim(sock)`), so any exit closes it. Runs telnet negotiation, the fake login and the fake shell. Streams its raw input to the recorder. Exits on close, idle timeout, max session length or a byte cap.
- **recorder**: the only process that touches sqlite. Batches inserts in a transaction (every N events or T ms). Keeps a capped raw-input ring per live session, written out on a crash and dropped on a normal exit. Counts the events it drops when its backlog is over the cap.
- **metrics sampler**: every 1–5 minutes records RSS (`/proc/self/statm`), open fds (`list_dir("/proc/self/fd")`), `runtime_stats()` (live processes, arena memory, open and ownerless resources, reset time, and resets over 10 and 100 ms since the last sample), sessions per state, crashes, cap hits and the Gem commit. The `/proc` reads are Linux only.
- **dashboard** (later, maybe never): `sqlite_query` and `sqlite_exec` run on the scheduler thread, so heavy aggregate queries in-process would stall every session. Either the recorder keeps rollup tables, or the dashboard is a separate OS process reading the WAL database. Until then the `sqlite3` CLI over SSH is the dashboard.
- A top-level supervisor over acceptor, registry, recorder and sampler.

## Telnet

- Strip IAC sequences incrementally (a sequence can be split across reads): WILL/WONT/DO/DONT + option, SB … IAC SE with a capped length, IAC IAC as a literal 255.
- On connect send WILL ECHO and WILL SUPPRESS-GO-AHEAD. Many bots ignore negotiation entirely.
- CR LF, CR NUL, a lone LF and a lone CR each end a line. Cap the line length (about 4 KB) and log when it's hit.
- Input is arbitrary bytes: NULs and invalid UTF-8 are expected. Store raw input as bytes.
- Every `tcp_write` takes a timeout, for bots that never read.

## Fake device

A cheap BusyBox IoT box: `login:`, `Password:`, then `# `. Login is accepted after N attempts or at random (configurable), and every username/password pair is recorded.

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

- `sessions`: id, ip, port, start/end time, end reason (closed, idle, cap, crash), bytes in/out, login succeeded
- `logins`: session_id, username, password, time
- `commands`: session_id, raw line, time
- `urls`: session_id, url, time
- `crashes`: session_id, reason, trace, raw input bytes
- `cap_events`: type, ip, time
- `metrics`: time, rss, fds, procs, arena_memory, resources_open, resources_ownerless, reset_ms, resets_over_10ms, resets_over_100ms, sessions_active, crashes_total, gem commit

One database file per week, deleting files past the retention limit, so the disk can't fill. IPs are stored for analysis and shown only truncated or aggregated. Anything shown in a browser is escaped and URLs are never clickable: every stored string is attacker-controlled.

## Limits

All configurable, all logged when hit: max concurrent sessions (global and per IP), idle timeout (about 60 s), max session length (about 10 min), max line length, max bytes in per session, max subnegotiation length, max login attempts, max commands per session, recorder backlog.

## Testing

- `test.gem` in this directory (std/test) covers the pure parts: the IAC parser including sequences split across reads, the line splitter, the command responses.
- A replayer sends stored crash input back over TCP. Crashes that reproduce become numbered examples or `docs/KNOWN_BUGS.md` entries.
- A hostile load generator under `benchmarks/soak/` runs the local fuzz hour: random bytes, IAC floods, a 10 MB line with no newline, 1-byte-per-second drips, 5,000 idle connections, connect and reset, half-close. RSS and fds must be flat once the connections close.
- Success in production is defined before deploy, e.g. RSS and fd count flat over 7 days.

## Deployment

- A dedicated VPS running nothing else. Gem's runtime is C, so a memory-safety bug could mean remote code execution on the box.
- Port 23 redirected to 2323 with nftables; the program runs unprivileged.
- Egress default-deny in nftables (DNS, package updates, NTP and SSH replies only) plus the provider firewall: the "no outbound connections" rule is enforced by the host, not only by the code.
- systemd with restart on failure (a restart is a finding too), `DynamicUser`, `NoNewPrivileges`, `ProtectSystem=strict`, `MemoryMax`, `LimitNOFILE`, `TasksMax`.

### Hosting requirements

- A dedicated public IPv4 address, not IPv6-only or behind NAT, with no proxy or always-on DDoS layer in front (it would hide source IPs or drop the floods worth recording).
- Raw inbound TCP on any port, port 23 included.
- A KVM VM with root (not OpenVZ/LXC): own nftables rules, systemd, raised fd limits.
- At least 1 vCPU, 1 GB RAM (Gem is compiled on the box), 10 GB disk.
- Monthly billing with no long commitment; terms that don't forbid honeypots; an EU location.

Candidates as of 2026-10: BuyVM Slice 1024 in Luxembourg (often out of stock) or Hetzner CX23 (was out of stock). Rejected: OVH VPS-1 (anti-DDoS can't be turned off) and netcup (cheap only on a 24-month term). Re-check stock and prices at deploy time; buy only then.

## Phases

1. `tcp_peer` builtin (done).
2. Process-owned sockets (ROADMAP), with a plan agreed before implementation: the plan is `docs/design/process_owned_resources.md`, agreed.
3. IAC parser, line splitter, fake login and shell on :2323, with `test.gem` (done: see README.md; events go to stdout as log lines until phase 4).
4. Crash-data path, recorder and schema.
5. Limits, metrics sampler, local fuzz hour.
6. Deploy on :23, watch the first day's traffic, extend the fake commands.
7. Optional: rollups or a dashboard; an HTTP honeypot on :80 under the same supervisor.
