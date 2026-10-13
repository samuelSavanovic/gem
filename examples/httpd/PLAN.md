# Public HTTP server: plan

A small website served by `std/http` on port 80 of the honeypot's VPS (`examples/honeypot/PLAN.md`, "Hosting requirements"), run for weeks. Its purpose is the honeypot's second one, for the HTTP stack: **a real-world stress test of Gem.** Anything on a public port 80 gets exploit scanners, malformed requests, slowloris, header floods and clients that never read. The goal is to find bugs, leaks and perf cliffs in `std/http`, `std/url`, the tcp builtins and the scheduler under that load. It is not an HTTP honeypot: no fake vulnerable endpoints, and anything without a route gets a plain 404.

Worth it: with the honeypot about 40 hours in, port 80 on the same host, opened with only a firewall counter behind it, drew about as many connection attempts as the honeypot's port 23.

## Non-negotiables

- **Never act on request content** beyond routing it: no outbound connections, no file access outside the static dir, no shelling out.
- **Hard caps on everything** (see Limits). A cap that is hit is a logged event.
- **Don't hide crashes.** An error in `std/http` itself (not a handler's error, which is a 500 by design) is recorded with its reason, trace and the start of the request.
- **Publish nothing attacker-controlled.** Public pages show counts only; no paths, IPs, headers or bodies.
- Follows `docs/BEST_PRACTICES.md`.

## Gem prerequisites (std/http)

Done: `http.start` has what an internet-facing server needs (SPEC, "Server"; `examples/227_http_events.gem`).

| Gap | Now |
|---|---|
| Client address | `req.peer`, `{ip, port}`, read once per connection right after the accept |
| Connection caps | `max_connections` and `max_per_ip`: over a cap, a 503 and an immediate close, and a `"refused"` event |
| Observing what never reaches a handler | `on_event`: `"open"` when a connection's process starts (so slowloris sockets that never send a byte are counted), `"request"` after every response (the server's own 400/404/413/431/500 included, with the first 4 KB of a rejected head as `raw`), `"close"` with the reason (closed, idle, timeout, done, rejected, write_failed, error), `"refused"` |
| Crash data | `"crash"` for an error in std/http's own code: its message, stack and the first 4 KB of the request; the client gets a 500 and the server keeps running |

`on_event` runs in the connection process (`"refused"` in the acceptor), so this program's observer only sends each event to the recorder.

## Process layout

- **http server**: `http.start` with the routes, the caps and `on_event`, under the top-level supervisor.
- **recorder**: the only process that touches sqlite. Batches inserts (every 200 events or 1 s), sheds all but connection opens and closes, crashes and metrics past a backlog of 10,000. One database file per week. Keeps in-memory counters for `/stats`, and the open connections (from the `"open"` and `"close"` events), which it adds to each metrics row.
- **sampler**: every minute records RSS, open fds, `runtime_stats()`, and (filled in by the recorder) open connections and their IPs, the recorder's backlog and dropped events, uptime and the Gem commit.
- A top-level supervisor over the server, recorder and sampler.

The honeypot's recorder and sampler are the model, copied, not shared, so the two programs change independently.

## Content

- `/`: one HTML page about Gem with a link to the repository.
- `/stats`: JSON of the server's own numbers (uptime, requests and connections by status and end reason, caps hit, RSS, live processes), from the recorder's counters, so a request does no sqlite query.
- `/static/`: a few files (CSS, an image) through `app.static`.
- Everything else: 404.

There's no domain and nothing links to the IP, so almost all traffic will be scanners whatever the content is. The content is there to put a handler, the recorder round-trip and static files under that traffic.

## Storage (sqlite)

- `connections`: id, ip, port, start/end time, end reason, requests, bytes in/out
- `requests`: connection_id, time, method (first 64 bytes), path (first 1 KB), status, bytes out, ms, user agent (first 256 bytes), the handler's error (std/http reports no per-request bytes in; the connection's `bytes_in` covers them)
- `bad_input`: connection_id, time, status, the first 4 KB read (BLOB)
- `crashes`: connection_id, time, reason, trace, the bytes read (BLOB)
- `cap_events`: ip, time, cap, detail (refused connections, and the recorder's backlog)
- `metrics`: as the honeypot's, plus open connections

## Limits

All configurable, all logged when hit: max connections (global 1,000, per IP 32), `max_body` 64 KB, idle and request timeouts 10 s, write timeout 10 s, recorder backlog. The unit's `MemoryMax` is set from the local soak's peak at 1,000 connections, with 2x headroom.

## Testing

- `test.gem` here (std/test): the routes, `/stats`, the recorder's batching against a temp database, run by `tests/check_example_apps.sh`.
- A hostile load target under `benchmarks/soak/` (`TARGETS=httpd`, `httpd_load.py`), run with the deploy's caps: scanner-style requests replayed from a list, malformed request lines and headers, a 1 MB header, slowloris (one byte per second), bodies past `max_body`, pipelined floods, clients that never read, connect-and-reset, 600 idle connections from 60 IPs, 100 from one (over the per-IP cap), a burst of 1,200 every 2 minutes (over the global cap), and a checked canary. The held connections stay under the global cap but for the bursts: 2,000 held for good would keep the canary refused. RSS and fds must be flat once the connections close.

## Deployment

`deploy/`: systemd unit, firewall fragment, health query, README with the steps.

- **Same VPS as the honeypot, own service.** Both are throwaway Gem programs on a box that holds nothing else of value; each runs as its own `DynamicUser` with its state dir mode `0750` and `UMask=0077` (as `examples/honeypot/deploy/honeypot.service`), so neither can read the other's state. The unit is the honeypot's sandbox plus `CPUQuota=100%` (one of the two vCPUs) and `Nice=5`, so a flood on :80 can't starve the honeypot.
- **Firewall:** the host ruleset (`examples/honeypot/deploy/honeypot.nft`) ends with `include "/etc/nftables.d/*.nft"`, and this directory's fragment adds port 80 → 8080, a counter of new connections, and the matching input rule. The egress counter already covers every `DynamicUser` UID, so it covers both services and must stay at 0. The provider's firewall (the Hetzner Cloud Firewall, set in Hetzner's console, not in this repository) gets TCP 80 in.
- **Port 443 is out of scope:** `std/http` has no TLS, and `docs/ROADMAP.md` puts server TLS behind a reverse proxy, which would hide the bytes this is meant to test.

## Success criteria

**Before port 80 opens**: the hostile load hour passes on macOS and Linux, then an 8-hour soak on the VPS with the deploy's caps and memory limit, with the load generator niced, passes every check of its report. The honeypot's metrics during that soak show no restart and no change in its `resets_100ms`.

**The first 7 days of real traffic**, as the honeypot's (`examples/honeypot/PLAN.md`, "Success criteria"): no restarts but our own, `fds_extra` at most 2 above day 1's, `res_extra` at most 2, `ownerless` at most 1, `rss_max_mb` under half of `MemoryMax` and day 7's at most 25% above day 2's, `resets_100ms` 0, no lost or dropped events not explained by a flood, egress counter 0. Each crash is triaged: replayed with its stored bytes, then fixed or added to `docs/KNOWN_BUGS.md`.

## Phases

1. `std/http`: `req.peer`, `max_connections` / `max_per_ip`, `on_event`, crash reporting (done), and the `"open"` event (done).
2. This directory: routes, recorder, sampler, `test.gem`, README (done).
3. The hostile load target (done) and the local soak hour: Linux PASS (`benchmarks/soak/results/2026-10-13_linux-vm_httpd`), macOS to do.
4. Deploy: unit, firewall fragment, health query (done); the 8-hour soak on the VPS, then open :80.
