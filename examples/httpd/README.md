# httpd

A small public website on `std/http`: one page about Gem, the server's
own numbers as JSON, and a few static files. It records everything its
clients do in sqlite. It is meant to run for weeks on port 80 of the
honeypot's host, as a stress test of Gem's HTTP stack under whatever the
internet sends; `PLAN.md` has the whole plan and which phases are done.
It is not an HTTP honeypot: there are no fake endpoints, and anything
without a route is a plain 404.

## Run it

```sh
cd examples/httpd
build/gem main.gem                                  # 127.0.0.1:8080, data in ./httpd_data, files from ./static
build/gem main.gem --port 8080 --bind 0.0.0.0 --data DIR --static DIR --keep-weeks 8
build/gem main.gem --log                            # also print each event on stdout
build/gem main.gem --commit "$(git rev-parse --short HEAD)" --max-per-ip 8
build/gem test.gem                                  # the tests (part of make test)
```

(`build/gem` here is the repository's compiler, `../../build/gem` from
this directory.)

These limits have a flag that takes a positive int: `--max-connections`
(default 1,000), `--max-per-ip` (32), `--max-body` (65,536 bytes),
`--idle-ms`, `--request-ms` and `--write-ms` (10,000 each; see
`http.start` in `docs/SPEC.md`), `--max-backlog` (the recorder's,
10,000); and `--port`, `--metrics-ms` (how often the metrics are sampled)
and `--keep-weeks`. `--commit ID` names the build in each metrics row.

| Path | What it serves |
|---|---|
| `/` | a page about Gem with a link to the repository |
| `/stats` | the server's counts as JSON (below); 503 when the recorder doesn't answer within a second |
| `/static/...` | the files in `--static` (`style.css`, `gem.svg`) |
| anything else | std/http's 404 |

`/stats` shows only numbers the server made itself, never a path,
address or header a client sent:

```json
{"counting_since":"2026-10-13T16:10:45Z","uptime_s":3,
 "requests":{"200":3,"404":1,"400":1},
 "connections":{"open":1,"ended":{"closed":4,"rejected":1}},
 "refused":{},"crashes":0,"events_dropped":0,
 "rss_kb":5544,"processes":5,"gem_commit":"da37aa2"}
```

The counts are the recorder's since it started (`counting_since`): every
event it received, also those it didn't store. `uptime_s`, `rss_kb`,
`processes` and `gem_commit` are from the last metrics sample.

## Data

Events go to a sqlite database, one file per week in the data directory
(`httpd-2026-10-12.db`, after the week's Monday, UTC), made if missing
(its parent must exist); files past the newest `--keep-weeks` are deleted
when the recorder starts and at its first write in a new week. Read them
with the `sqlite3` shell:

```sh
sqlite3 httpd_data/httpd-2026-10-12.db \
  "SELECT path, count(*) FROM requests GROUP BY 1 ORDER BY 2 DESC LIMIT 20"
```

Every stored string and blob but the end reasons and caps is
attacker-controlled: escape it before showing it in a terminal or a
browser.

The events are std/http's (`on_event`, SPEC "Server"), cut down to what
is stored by `server.event_of`, which runs in the connection's process:

| Event | Fields | Stored in |
|---|---|---|
| `open` | `ip`, `port` | a `connections` row |
| `request` | `status`, `ms`, `bytes_out`; `method` (first 64 bytes), `path` (first 1 KB) and `agent` (the User-Agent, first 256 bytes) once the head was parsed; `error` when the handler failed; `raw` (the first 4 KB of the head) when the server rejected the request | `requests`, and `bad_input` (BLOB) with `raw` |
| `close` | `reason` (`closed`, `idle`, `timeout`, `done`, `rejected`, `write_failed`, `error`), `requests`, `bytes_in`, `ms` | ends the `connections` row, with the bytes its responses sent |
| `crash` | `error`, `trace`, `raw`: an error in std/http's own code | `crashes` |
| `refused` | `ip`, `port`, `reason` (`max_connections`, `max_per_ip`, `spawn_failed` with `error`) | `cap_events` |
| `cap` | `recorder_backlog`, with how many events of each kind the recorder dropped | `cap_events` |
| `metrics` | see below: when the program starts, then every minute | `metrics` |

The other limits show where they hit: a body over `--max-body` is a
`413` request, a head over 64 KB a `431`, and the timeouts end
connections as `idle`, `timeout` or `write_failed`.

A connection whose close never reached the recorder (the HTTP server
restarted and killed it, or the recorder restarted meanwhile) is ended
within two minutes of its death as `lost`. A connection open when a new
week starts has its row in the old file ended as `week_end` and gets a
new row in the new file. When the recorder starts, it ends the rows a
previous run of the program left open as `lost`. The schema is `SCHEMA`
in `recorder.gem`; times are epoch milliseconds.

A `metrics` row holds the program's health at that time, as the
honeypot's (`examples/honeypot/README.md`), with `connections` and
`connection_ips` (the open connections the recorder knows of, and the
IPs they come from) in place of its sessions.

## Design

```
httpd_sup (supervisor, one_for_one)
├── recorder            ── stores the events in sqlite, keeps the counts
├── http                ── http.start: acceptor, a process per connection
└── sampler             ── records the metrics
```

| File | What it holds |
|---|---|
| `main.gem` | entry point: flags, start |
| `server.gem` | the supervision tree, and the observer that turns std/http's events into the recorder's |
| `site.gem` | the routes and the page |
| `recorder.gem` | the database: schema, batched writes, weekly files, the backlog cap, the counts for `/stats`; the log lines |
| `sampler.gem` | the metrics: memory, fds, processes, resets |
| `static/` | the stylesheet and the image |
| `test.gem` | unit tests and end-to-end tests over TCP |

Choices worth knowing:

- **The recorder is copied from the honeypot's, not shared,** so the two
  programs change independently.
- **std/http does the work.** The caps, the timeouts, the parsing and the
  rejections are all `http.start`'s; this program adds only the
  routes and the recording, so what the traffic finds is std/http's.
- **`/stats` asks the recorder,** a message round trip per request and
  no sqlite query, so a flood of `/stats` costs no more than one of `/`.
- **The observer cuts the events down** in the connection's process, so
  the recorder's mailbox holds small messages even when a client sends
  a 64 KB head.
