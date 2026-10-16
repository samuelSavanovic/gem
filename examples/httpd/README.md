# httpd

A small website on `std/http`: one page about Gem, the server's own numbers as JSON, and a few static files. It records everything its clients do in SQLite, which makes it a test of the HTTP stack under whatever traffic you point at it. It isn't an HTTP honeypot: there are no fake endpoints, and anything without a route gets a plain 404.

## Run it

```sh
cd examples/httpd
../../build/gem main.gem                     # 127.0.0.1:8080, data in ./httpd_data, files from ./static
../../build/gem main.gem --port 8080 --bind 0.0.0.0 --data DIR --static DIR --keep-weeks 8
../../build/gem main.gem --log               # also print each event on stdout
../../build/gem test.gem                     # the tests (part of make test)
```

These flags take a positive int: `--max-connections` (default 1,000), `--max-per-ip` (32), `--max-body` (65,536 bytes), `--idle-ms`, `--request-ms` and `--write-ms` (10,000 each), `--max-backlog` (the recorder's, 10,000), `--port`, `--metrics-ms` (how often metrics are sampled) and `--keep-weeks`. `--commit ID` is stored in each metrics row to name the build.

| Path | What it serves |
|---|---|
| `/` | a page about Gem with a link to the repository |
| `/stats` | the server's counts as JSON (below); 503 when the recorder doesn't answer within a second |
| `/static/...` | the files in `--static` (`style.css`, `gem.svg`) |
| anything else | std/http's 404 |

`/stats` shows only numbers the server made itself, never a path, address or header a client sent:

```json
{"counting_since":"2026-11-24T12:20:44Z","uptime_s":0,
 "requests":{"200":2,"404":1,"400":1},
 "connections":{"open":1,"ended":{"closed":3,"rejected":1}},
 "refused":{},"crashes":0,"events_dropped":0,
 "rss_kb":null,"processes":6,"gem_commit":null}
```

The counts are the recorder's since it started (`counting_since`), including events it received but didn't store. `uptime_s`, `rss_kb`, `processes` and `gem_commit` come from the last metrics sample. `rss_kb` is read from `/proc/self/status`, so it is `null` on macOS.

## Data

Events go to a SQLite database, one file per week in the data directory (`httpd-YYYY-MM-DD.db`, named after the week's Monday, UTC), with the same file rotation as the honeypot (see `examples/honeypot/README.md`). Read them with the `sqlite3` shell:

```sh
sqlite3 httpd_data/httpd-*.db \
  "SELECT path, count(*) FROM requests GROUP BY 1 ORDER BY 2 DESC LIMIT 20"
```

Apart from end reasons and caps, every stored string and blob comes from a client, so escape it before showing it in a terminal or a browser.

The events are std/http's (the `on_event` option of `http.start`), cut down to what gets stored by `server.event_of`, which runs in the connection's process so the recorder's mailbox only holds small messages:

| Event | Fields | Stored in |
|---|---|---|
| `open` | `ip`, `port` | a `connections` row |
| `request` | `status`, `ms`, `bytes_out`; once the head is parsed, `method` (first 64 bytes), `path` (first 1 KB) and `agent` (User-Agent, first 256 bytes); `error` when the handler failed; `raw` (the first 4 KB of the head) when the server rejected the request | `requests`, plus `bad_input` (BLOB) for `raw` |
| `close` | `reason` (`closed`, `idle`, `timeout`, `done`, `rejected`, `write_failed`, `error`), `requests`, `bytes_in`, `ms` | ends the `connections` row |
| `crash` | `error`, `trace`, `raw`: an error in std/http's own code | `crashes` |
| `refused` | `ip`, `port`, `reason` (`max_connections`, `max_per_ip`, or `spawn_failed` with `error`) | `cap_events` |
| `cap` | `recorder_backlog`, with how many events of each kind the recorder dropped | `cap_events` |
| `metrics` | at startup, then every minute | `metrics` |

The other limits show up where they're hit: a body over `--max-body` is a `413`, a head over 64 KB a `431`, and the timeouts end connections as `idle`, `timeout` or `write_failed`.

If a connection's close never reaches the recorder (because the HTTP server or the recorder restarted), its row is ended as `lost` within two minutes. A connection open when a new week starts has its row in the old file ended as `week_end` and gets a new row in the new file, and on startup the recorder ends rows a previous run left open as `lost`. The schema is `SCHEMA` in `recorder.gem`, and times are epoch milliseconds. The `metrics` rows have the same columns as the honeypot's, with `connections` and `connection_ips` in place of its sessions.

## Design

```
httpd_sup (supervisor, one_for_one)
├── recorder            stores the events in SQLite, keeps the counts
├── http                http.start: acceptor, a process per connection
└── sampler             records the metrics
```

| File | What it holds |
|---|---|
| `main.gem` | entry point: flags, start |
| `server.gem` | the supervision tree, and the observer that turns std/http's events into the recorder's |
| `site.gem` | the routes and the page |
| `recorder.gem` | the database (schema, batched writes, weekly files, the backlog cap), the counts for `/stats`, and the log lines |
| `sampler.gem` | the metrics |
| `static/` | the stylesheet and the image |
| `test.gem` | unit tests and end-to-end tests over TCP |

The recorder is a copy of the honeypot's, not shared code, so the two programs can change independently. The caps, timeouts, parsing and rejections all come from `http.start`; this program only adds the routes and the recording. `/stats` asks the recorder for its counts (a message round trip, no SQLite query), so a flood of `/stats` costs no more than a flood of `/`.
