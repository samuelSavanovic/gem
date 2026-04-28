# Web Stack Roadmap

Standard library modules and runtime extensions needed to build HTMX-backed CRUD applications in Gem. Backend serves HTML fragments + static files, sits behind a reverse proxy (nginx) for TLS.

## Dependency Graph

```
std/mime          std/url           C: time builtins
    \                |                  |
     \               |              std/time
      \              |              /      \
       ------→  std/http  ←--------      std/log
                    |
               std/json (exists)

C: sqlite builtins → std/sqlite
C: crypto builtins → std/crypto

std/url + TCP builtins → std/request
```

Build order follows the DAG — leaves first, dependents after.

## Phase 1: Pure Gem Foundations -- DONE

### std/mime -- DONE

MIME type lookup by file extension. Pure data table, no dependencies.

**Exports:**

- `mime.lookup(path_or_ext)` — `".html"` or `"index.html"` → `"text/html; charset=utf-8"`. Returns `"application/octet-stream"` for unknown extensions.
- `mime.ext(content_type)` — reverse lookup: `"text/html"` → `".html"`. Returns `nil` for unknown types.

**Coverage (minimum):** html, htm, css, js, mjs, json, xml, txt, csv, png, jpg, jpeg, gif, svg, ico, webp, avif, woff, woff2, ttf, otf, pdf, zip, gz, mp3, mp4, webm, wasm.

**Charset:** text types (`text/*`, `application/json`, `application/javascript`, `application/xml`) include `; charset=utf-8` automatically.

**Effort:** ~80 lines.

### std/url -- DONE

URL percent-encoding, query string parsing, and path handling. No dependencies.

**Exports:**

- `url.encode(str)` — percent-encode per RFC 3986 (unreserved chars: `A-Za-z0-9-_.~` pass through, everything else becomes `%XX`).
- `url.decode(str)` — percent-decode (`%XX` → byte, `+` → space).
- `url.parse_query(str)` — `"a=1&b=hello+world&c=%2F"` → `{a: "1", b: "hello world", c: "/"}`. Decodes both keys and values. Duplicate keys: last value wins (document this; multi-value is a v2 concern).
- `url.build_query(table)` — `{a: "1", b: "hello world"}` → `"a=1&b=hello+world"`. Encodes both keys and values.
- `url.parse(path_with_query)` — `"/users/123?q=foo&page=2"` → `{path: "/users/123", query_string: "q=foo&page=2", query: {q: "foo", page: "2"}}`. Splits on first `?`, parses query portion.

**Implementation note:** percent-decode needs `ord()` and `chr()` for hex conversion. Pure Gem, no C builtins needed — will be slower than a C version but fine until profiling says otherwise. Add a note in OPTIMIZATIONS.md if the pure-Gem version becomes a bottleneck.

**Effort:** ~120 lines.

## Phase 2: C Runtime Extensions

### Time builtins -- DONE

New file: `runtime/gem_builtins_time.c`. Three new builtins registered in `gem.h` and `codegen.gem`.

**C builtins:**

- `epoch_ms()` → int. Returns wall-clock time as milliseconds since Unix epoch via `gettimeofday`. Distinct from `time_ms()` which is monotonic.
- `format_time(epoch_ms, format_str)` → string. Converts epoch milliseconds to UTC broken-down time via `gmtime_r`, formats with `strftime`. Returns GC-allocated string.
- `format_time_local(epoch_ms, format_str)` → string. Same but uses `localtime_r` for local timezone.

**Format strings:** standard `strftime` specifiers (`%Y`, `%m`, `%d`, `%H`, `%M`, `%S`, etc.). Document the supported specifiers in the spec — don't invent new ones.

**Why two builtins instead of a UTC flag:** clearer call sites, no boolean argument ambiguity, matches C's `gmtime`/`localtime` split.

**Effort:** ~60 lines C + declarations in gem.h + codegen.gem mapping.

### SQLite builtins -- DONE

New file: `runtime/gem_builtins_sqlite.c`. SQLite is vendored as an amalgamation (`runtime/sqlite3.c` + `runtime/sqlite3.h`) — no system dependency.

**Blocking strategy — `extern fn` vs `extern blocking fn`:**

Most SQLite operations on a local file with WAL mode complete in microseconds (page cache hits, no real disk I/O). The thread pool has measurable overhead (enqueue, wake worker, mutex, signal completion) — we found with TCP primitives that thread pool dispatch is often more expensive than the operation itself for fast calls. For CRUD workloads, inline execution that briefly blocks the coroutine is cheaper than paying thread pool tax on every query.

- `sqlite_open` → **`extern blocking fn`** (thread pool). First open may hit cold disk, create WAL file, run PRAGMA setup. One-time cost, worth offloading.
- `sqlite_close` → **`extern blocking fn`** (thread pool). Fsyncs WAL to main DB. Rare (shutdown), but can take milliseconds.
- `sqlite_exec`, `sqlite_query`, `sqlite_last_insert_id`, `sqlite_changes` → **`extern fn`** (inline). Sub-millisecond for typical CRUD. Blocks the coroutine briefly, but avoids thread pool overhead on every request.

**Trade-off to revisit at implementation time:** if a user runs a heavy query (bulk import, complex JOIN on large tables), it blocks the scheduler for the duration. This is acceptable — they can isolate heavy work in a dedicated spawned process. The common case (simple CRUD) shouldn't pay for the uncommon case. Benchmark both approaches during implementation to validate this assumption.

**C builtins:**

- `sqlite_open(path)` → Ptr. `extern blocking fn`. Opens (or creates) a SQLite database. Enables WAL mode and foreign keys by default. Returns opaque handle.
- `sqlite_close(db)` → Nil. `extern blocking fn`. Closes the database handle.
- `sqlite_exec(db, sql)` → Nil. `extern fn`. Executes SQL that returns no rows (CREATE TABLE, INSERT without RETURNING, etc.). Raises on error.
- `sqlite_query(db, sql, params)` → Table (array of row tables). `extern fn`. Executes a parameterized query. `params` is a Gem array of bind values (positional `?` placeholders). Returns an array where each row is a string-keyed table (`{id: 1, name: "Alice"}`). Column types map: INTEGER → Int, REAL → Float, TEXT → String, NULL → Nil, BLOB → String (raw bytes).
- `sqlite_last_insert_id(db)` → Int. `extern fn`. Returns `sqlite3_last_insert_rowid`.
- `sqlite_changes(db)` → Int. `extern fn`. Returns `sqlite3_changes` (rows affected by last INSERT/UPDATE/DELETE).

**Parameterized queries are mandatory** for anything with user input. The API takes `params` as a separate array, not string interpolation into SQL. This is the SQL injection prevention boundary.

**Why no prepare/step/finalize:** the query-and-collect model is simpler and sufficient for CRUD apps. A streaming/cursor API can come later if needed (add to OPTIMIZATIONS.md).

**Connection pooling:** not in scope for the builtins. A `std/sqlite` wrapper can implement a pool using gen_server if needed. For a single-writer WAL-mode SQLite, one connection per request is fine.

**Effort:** ~300 lines C.

### Crypto builtins (libsodium)

New file: `runtime/gem_builtins_crypto.c`. New build dependency: `libsodium` (system package, like bdw-gc and libsqlite3).

Call `sodium_init()` once during runtime initialization (in `gem_init` or similar).

**Gem strings are null-terminated**, so raw binary output (which may contain `\0`) can't be stored as-is. All functions that produce binary output return **hex-encoded strings** using libsodium's `sodium_bin2hex`. This is safe, simple, and round-trips cleanly. Base64 variants are also provided for compactness where needed.

**Blocking strategy:** password hashing is deliberately CPU-intensive (argon2id, ~100ms+ by design). It **must** run on the thread pool to avoid starving the scheduler. Everything else completes in microseconds.

**C builtins:**

- `crypto_random_bytes(n)` → String. `extern fn`. Generates `n` random bytes, returns as hex string (2×n chars). Wraps `randombytes_buf` + `sodium_bin2hex`.
- `crypto_random_int(upper_bound)` → Int. `extern fn`. Uniform random integer in `[0, upper_bound)`. Wraps `randombytes_uniform`.
- `crypto_hash_password(password)` → String. `extern blocking fn`. Hashes password with argon2id using libsodium's `crypto_pwhash_str` with `MODERATE` ops/mem limits. Returns the encoded hash string (~128 chars, includes algorithm, salt, and hash — safe to store directly in the database).
- `crypto_verify_password(hash, password)` → Bool. `extern blocking fn`. Verifies a password against a hash from `crypto_hash_password`. Wraps `crypto_pwhash_str_verify`. Timing-safe.
- `crypto_hmac(key, message)` → String. `extern fn`. HMAC-SHA-512-256. Returns hex-encoded tag. Wraps `crypto_auth`. Key must be 64 hex chars (32 bytes).
- `crypto_hmac_verify(key, message, tag)` → Bool. `extern fn`. Verifies HMAC tag. Timing-safe. Wraps `crypto_auth_verify`.
- `crypto_generic_hash(message, key?)` → String. `extern fn`. BLAKE2b hash, returns hex. Optional key for keyed hashing. Wraps `crypto_generichash`.
- `crypto_constant_time_eq(a, b)` → Bool. `extern fn`. Timing-safe string comparison. Wraps `sodium_memcmp`. Both strings must be equal length (returns false otherwise).
- `crypto_base64_encode(hex_str)` → String. `extern fn`. Converts hex-encoded binary to base64. Wraps `sodium_hex2bin` + `sodium_bin2base64`.
- `crypto_base64_decode(b64_str)` → String. `extern fn`. Converts base64 to hex-encoded binary. Wraps `sodium_base642bin` + `sodium_bin2hex`.

**Why hex as the interchange format:** it's simple, unambiguous, no padding issues, trivially debuggable, and libsodium provides `sodium_bin2hex`/`sodium_hex2bin` for free. The base64 functions are there for cases where compactness matters (cookies, headers).

**Effort:** ~180 lines C + ~20 lines declarations.

## Phase 3: Standard Library Modules

### std/time -- DONE

Higher-level time formatting built on the Phase 2 C builtins. Depends on: `format_time`, `format_time_local`, `time_ms` (exists).

**Exports:**

- `time.now()` → Int. Alias for `epoch_ms()`. Wall-clock milliseconds since Unix epoch.
- `time.format(ms, fmt)` → String. UTC formatting, wraps `format_time`.
- `time.format_local(ms, fmt)` → String. Local time, wraps `format_time_local`.
- `time.http_date(ms?)` → String. RFC 7231 format: `"Mon, 28 Apr 2026 14:30:00 GMT"`. Uses `ms` if given, otherwise `time_ms()`. Needed for HTTP `Date` header and cookie `Expires`.
- `time.iso8601(ms?)` → String. `"2026-04-28T14:30:00Z"`. Default format for logging and APIs.
- `time.date(ms?)` → String. `"2026-04-28"`. Calendar date only.

**Design decision:** `time.now()` returns wall-clock epoch ms (via `epoch_ms()`), not monotonic ms. `time_ms()` remains monotonic for timeouts and benchmarks. `format_time`/`format_time_local` require epoch ms, so `time.now()` gives the right value for formatting.

**Effort:** ~40 lines.

### std/log -- DONE

Structured logging to stderr. Depends on: `std/time`.

**Exports:**

- `log.debug(msg)`, `log.info(msg)`, `log.warn(msg)`, `log.error(msg)` — log at the given level.
- `log.set_level(level)` — set minimum level. One of `"debug"`, `"info"`, `"warn"`, `"error"`. Default: `"info"`. Messages below the level are silently dropped.

**Output format:**

```
2026-04-28T14:30:00Z [INFO] Server listening on port 8080
2026-04-28T14:30:01Z [ERROR] sqlite_query failed: no such table: users
```

ISO 8601 UTC timestamp, bracketed uppercase level, message. One line per entry. To stderr via `print_error` (or a new `eprint` builtin if `print_error` doesn't exist — check).

**No structured fields in v1.** `log.info("request method={method} path={path} status={status}")` using string interpolation is good enough. Structured key-value logging (JSON lines, logfmt) is a v2 concern.

**Process-safe:** logging from concurrent processes interleaves at line boundaries. Each `log.*` call writes a single line atomically (one `write(2)` call). This may need a C builtin (`eprint` or similar) that does a single `write(STDERR_FILENO, ...)` to avoid interleaving from buffered I/O.

**Effort:** ~50 lines Gem + possibly a small C builtin for atomic stderr write.

### std/http -- DONE

The main module. Depends on: `std/url`, `std/mime`, `std/json` (exists), `std/time`, `std/string` (exists).

This absorbs and generalizes the existing `examples/http_server/server.gem` code. The example's parse_request, response builders, router, and accept loop become library code.

**Exports — Response builders:**

- `http.response(status, headers, body)` → response table. Generic response constructor. `headers` is a table of header name → value.
- `http.ok(body, content_type?)` → 200 response. Default content type: `"text/plain"`.
- `http.html(body)` → 200 with `text/html; charset=utf-8`.
- `http.json(data)` → 200 with `application/json`. Auto-encodes `data` via `json.encode`.
- `http.redirect(url, status?)` → 301/302 redirect. Default status: 302.
- `http.not_found(body?)` → 404. Default body: `"Not Found"`.
- `http.bad_request(body?)` → 400.
- `http.error(body?)` → 500. Default body: `"Internal Server Error"`.

**Response table shape:** `{status: 200, headers: {"Content-Type": "text/html"}, body: "<h1>Hello</h1>"}`. The server serializes this to HTTP/1.1 wire format. Users can also build this table manually for full control.

**Exports — Router:**

- `http.router()` → router table with methods:
  - `router.get(pattern, handler)`
  - `router.post(pattern, handler)`
  - `router.put(pattern, handler)`
  - `router.patch(pattern, handler)`
  - `router.delete(pattern, handler)`
  - `router.static(url_prefix, dir_path)` — serve static files. `router.static("/static", "./public")` serves `./public/style.css` at `/static/style.css`. Uses `std/mime` for content types. Returns 404 for missing files. **Path traversal prevention:** reject any resolved path that escapes `dir_path` (normalize and check prefix).

**Route patterns:** segments starting with `:` are parameters. `/users/:id` matches `/users/123` and sets `req.params.id = "123"`. `/users/:id/posts/:post_id` extracts both. Literal segments must match exactly. No wildcards/globs in v1.

**Match order:** first registered route wins (not most-specific). This matches Express/Sinatra behavior and is simpler to reason about. Document that users should register specific routes before general ones.

**Handler signature:** `fn(req) → response`. The request table:

```
{
  method: "GET",
  path: "/users/123",
  params: {id: "123"},
  query: {page: "2", sort: "name"},
  headers: {"Content-Type": "application/json", "Cookie": "session=abc123"},
  body: "...",
  cookies: {session: "abc123"}
}
```

- `params` — extracted from route pattern (`:id` etc.)
- `query` — parsed from query string via `std/url`
- `cookies` — parsed from `Cookie` header
- `headers` — raw headers table (keys are original case from request)

**Exports — Cookies:**

- `http.set_cookie(resp, name, value, opts?)` → new response with `Set-Cookie` header appended. `opts` table supports: `path` (string, default `"/"`), `http_only` (bool, default `true`), `secure` (bool, default `false` — reverse proxy handles TLS), `same_site` (string: `"Strict"`, `"Lax"`, `"None"`, default `"Lax"`), `max_age` (int, seconds), `expires` (int, epoch ms — formatted via `std/time`).
- `http.delete_cookie(resp, name, opts?)` → sets cookie with `Max-Age=0`.

Cookie parsing (request side) is internal — the server parses the `Cookie` header and populates `req.cookies` automatically.

**Exports — Security:**

- `http.html_escape(str)` → string. Escapes `&`, `<`, `>`, `"`, `'` → HTML entities. **Must be used** when interpolating user data into HTML responses. This is the XSS prevention boundary.

**Exports — Server:**

- `http.serve(router, opts?)` → pid. Starts the HTTP server. Spawns an acceptor process that listens for connections and dispatches to the router. Returns the acceptor pid (supervisable). `opts` table: `port` (int, default 8080), `host` (string, default `"0.0.0.0"`), `max_connections` (int, default 1000).

**Request lifecycle:**

1. Acceptor calls `tcp_accept`, spawns a handler process per connection.
2. Handler enters a **connection loop** (keep-alive):
   a. Read bytes into a connection buffer via `tcp_read`.
   b. Scan buffer for `\r\n\r\n` (end of headers). If not found, read more and repeat.
   c. Parse headers. If `Content-Length` is present, read exactly that many body bytes from the buffer (reading more from the socket if needed).
   d. Build request table: split path from query string, parse query via `url.parse_query`, parse cookies from headers.
   e. Router matches path against registered patterns, extracts params.
   f. Matched handler function is called with the request table.
   g. Handler returns a response table (or the router returns 404/405).
   h. Response is serialized to HTTP/1.1 wire format and written via `tcp_write`. Response includes `Content-Length` (required for keep-alive framing).
   i. If request had `Connection: close` header, break the loop.
   j. Otherwise, loop back to (a) for the next request on this connection.
3. On idle timeout or `Connection: close`, `tcp_close` and process exits.
4. Errors at any step are caught by `pcall`, a 500 response is written, and the connection is closed (don't try to recover a broken stream).

**Buffered reader:** the connection loop maintains a byte buffer across iterations. Leftover bytes after consuming one request (common with pipelined clients or eager senders) become the start of the next request. This is the core complexity of keep-alive — ~60-80 lines of careful buffer management. Edge cases to handle: partial header reads, Content-Length larger than initial buffer, zero-length bodies (GET/DELETE/HEAD).

**Keep-alive timeout:** idle connections (no data arriving) must be closed to avoid fd exhaustion. Options to evaluate at implementation time:
- `send_after(self(), "timeout", idle_ms)` — watchdog timer, cancelled on each new request. Simple but requires the connection loop to select between tcp data and mailbox messages, which doesn't fit the current `tcp_read` blocking model.
- Scheduler-level tcp_read timeout — add an optional timeout parameter to `tcp_read(fd, max, timeout_ms?)` that returns `""` on timeout. Cleaner, but requires a runtime change.
- Rely on the reverse proxy (nginx) to close idle upstream connections. Pragmatic for the demo, but fragile as a default.

Recommend evaluating the `tcp_read` timeout approach — it's useful beyond keep-alive (any protocol with idle detection) and is a small scheduler change (add a deadline to the poll fd entry).

**What about middleware?** No explicit middleware chain in v1. Users compose behavior with wrapper functions:

```
fn with_logging(handler)
  fn(req)
    log.info("→ {req.method} {req.path}")
    let resp = handler(req)
    log.info("← {resp.status}")
    resp
  end
end

router.get("/users", with_logging(fn(req) ... end))
```

This is explicit, composable, and doesn't require framework machinery. A `http.use(middleware)` API can come later if patterns emerge.

**Effort:** ~400 lines (up from ~300 — buffered reader + keep-alive loop + timeout handling).

### std/request

HTTP client for making outbound requests. Pure Gem, no new C builtins. Depends on: TCP builtins, `std/url`, `std/json` (exists).

**Exports:**

- `request.get(url, opts?)` → response table. `opts` table supports: `headers` (table of header name → value).
- `request.post(url, opts?)` → response table. `opts` table supports: `headers`, `body` (string).
- `request.put(url, opts?)` → response table. Same as post.
- `request.patch(url, opts?)` → response table. Same as post.
- `request.delete(url, opts?)` → response table. Same as get.
- `request.request(method, url, opts?)` → response table. Generic method for full control.

**Response table shape:** `{status: 200, headers: {"Content-Type": "application/json", ...}, body: "..."}`.

**URL parsing:** split `http://host:port/path?query` into components. Default port 80. Only HTTP (no TLS — reverse proxy or external services on HTTP are fine for v1).

**Request lifecycle:**

1. Parse URL into host, port, path+query.
2. `tcp_connect(host, port)`.
3. Build HTTP/1.1 request: method, path, `Host` header, `Connection: close` (no keep-alive in v1), `Content-Length` if body present, user headers.
4. `tcp_write` the request.
5. Read response: buffer reads until `\r\n\r\n` (end of headers). Parse status line and headers. Read body based on `Content-Length` or read until connection close.
6. `tcp_close`, return response table.

**Chunked transfer encoding:** v1 sends `Connection: close` to avoid chunked responses from most servers. If a server sends chunked anyway, fall back to reading until EOF. Proper chunked parsing is a v2 concern (add to OPTIMIZATIONS.md).

**No HTTPS:** TLS requires a crypto library (OpenSSL/LibreSSL). Out of scope. For APIs that require HTTPS, users can curl via `exec` or go through a local proxy. Document this limitation clearly.

**Usage example:**

```
load "std/request"
load "std/json"

# Simple GET
let resp = request.get("http://api.example.com/data")
let data = json.decode(resp.body)

# POST with JSON body
let resp = request.post("http://api.example.com/items", {
  headers: {"Content-Type": "application/json"},
  body: json.encode({name: "widget", price: 9.99})
})

# Fetch weather, store in SQLite
load "std/sqlite"
let db = sqlite.open("weather.db")
let weather = json.decode(request.get("http://api.weather.example/current").body)
sqlite.query(db, "INSERT INTO readings (temp, humidity) VALUES (?, ?)",
  [weather.temp, weather.humidity])
```

**Effort:** ~100 lines.

### std/sqlite -- DONE

Thin Gem wrapper over the Phase 2 C builtins. Provides a slightly friendlier API.

**Exports:**

- `sqlite.open(path)` → db handle.
- `sqlite.close(db)` → nil.
- `sqlite.exec(db, sql)` → nil. For DDL and statements without results.
- `sqlite.query(db, sql, params?)` → array of row tables. `params` defaults to `[]`. Each row is a table: `{id: 1, name: "Alice", email: "alice@example.com"}`.
- `sqlite.last_id(db)` → int. Last inserted row ID.
- `sqlite.changes(db)` → int. Rows affected by last mutation.

**Usage example:**

```
load "std/sqlite"

let db = sqlite.open("app.db")
sqlite.exec(db, "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT, email TEXT)")

sqlite.query(db, "INSERT INTO users (name, email) VALUES (?, ?)", ["Alice", "alice@example.com"])
let id = sqlite.last_id(db)

let users = sqlite.query(db, "SELECT * FROM users WHERE id = ?", [id])
# → [{id: 1, name: "Alice", email: "alice@example.com"}]

sqlite.close(db)
```

**Effort:** ~40 lines.

### std/crypto

Friendly Gem wrapper over the Phase 2 crypto C builtins. Depends on: crypto builtins.

**Exports:**

- `crypto.token(n?)` → String. Generate a random hex token. Default `n` = 32 bytes (64 hex chars). Good for session IDs, CSRF tokens.
- `crypto.random_int(upper_bound)` → Int. Uniform random integer in `[0, upper_bound)`.
- `crypto.hash_password(password)` → String. Argon2id hash suitable for database storage.
- `crypto.verify_password(hash, password)` → Bool. Verify password against stored hash.
- `crypto.hmac(key, message)` → String. HMAC-SHA-512-256, hex-encoded.
- `crypto.hmac_verify(key, message, tag)` → Bool. Timing-safe HMAC verification.
- `crypto.hash(message, key?)` → String. BLAKE2b, hex-encoded. Optional key for keyed hashing.
- `crypto.constant_time_eq(a, b)` → Bool. Timing-safe string comparison.
- `crypto.base64_encode(hex_str)` → String. Hex to base64.
- `crypto.base64_decode(b64_str)` → String. Base64 to hex.

**Usage example:**

```
load "std/crypto"

# Session token
let session_id = crypto.token()  # "a3f2b9c1...64 hex chars..."

# Password storage
let hash = crypto.hash_password("hunter2")
# store hash in DB...

# Password verification on login
if crypto.verify_password(stored_hash, submitted_password)
  # authenticated
end

# HMAC-signed cookie value
let secret = crypto.token(32)  # generate once, store securely
let sig = crypto.hmac(secret, session_id)
# Set-Cookie: session=<session_id>.<sig>
```

**Effort:** ~60 lines.

## Phase 4: Demo Application

### HTMX CRUD App

A todo-list (or contact book) app demonstrating the full stack.

**Structure:**

```
examples/crud_app/
  app.gem           # routes, handlers, server start
  db.gem            # database setup, query helpers
  static/
    index.html      # main page, loads HTMX
    style.css       # minimal styling
```

**Features:**

- List all items (GET `/`)
- Create item (POST `/items` via HTMX form)
- Edit item inline (GET `/items/:id/edit` returns form fragment, PUT `/items/:id` saves)
- Delete item (DELETE `/items/:id` via HTMX button)
- SQLite persistence
- HTML escaping on all user-provided content
- Error handling (invalid input, missing items)
- Request logging

**app.gem sketch** (not real code, just to show the shape):

```
load "std/http"
load "std/log"
load "std/sqlite"

let db = sqlite.open("todos.db")
# ... create table ...

let router = http.router()
router.static("/static", "./static")

router.get("/", fn(req)
  let todos = sqlite.query(db, "SELECT * FROM todos ORDER BY id DESC")
  http.html(render_todo_list(todos))
end)

router.post("/todos", fn(req)
  let title = url.decode(req.body)  # form-encoded
  sqlite.query(db, "INSERT INTO todos (title) VALUES (?)", [title])
  let todos = sqlite.query(db, "SELECT * FROM todos ORDER BY id DESC")
  http.html(render_todo_list(todos))
end)

router.delete("/todos/:id", fn(req)
  sqlite.query(db, "DELETE FROM todos WHERE id = ?", [req.params.id])
  http.ok("")
end)

http.serve(router, {port: 8080})
```

**index.html** loads HTMX from CDN, contains the initial page shell. All dynamic content is served as HTML fragments from Gem.

**Effort:** ~200 lines Gem + ~50 lines HTML/CSS.

## Summary

| Module | Type | Depends on | Lines (est.) | Phase |
|---|---|---|---|---|
| std/mime | Pure Gem | — | ~80 | 1 |
| std/url | Pure Gem | — | ~120 | 1 |
| time builtins | C runtime | — | ~60 | 2 |
| sqlite builtins | C runtime | libsqlite3 | ~300 | 2 |
| crypto builtins | C runtime | libsodium | ~200 | 2 |
| std/time | Pure Gem | time builtins | ~40 | 3 |
| std/log | Pure Gem | std/time | ~50 | 3 |
| std/http | Pure Gem | std/url, std/mime, std/json, std/time, std/string | ~400 | 3 |
| std/sqlite | Pure Gem | sqlite builtins | ~40 | 3 |
| std/request | Pure Gem | TCP builtins, std/url | ~100 | 3 |
| std/crypto | Pure Gem | crypto builtins | ~60 | 3 |
| CRUD demo | Gem + HTML | everything above | ~250 | 4 |
| **Total** | | | **~1720** | |

## Not in Scope (v2+)

- **HTTP/2, WebSockets** — reverse proxy handles these.
- **Multipart form parsing** — file uploads are a separate effort.
- **Session management** — can be built with cookies + sqlite, but no std/session module yet.
- **Rate limiting / CSRF** — application-level concerns, not std library. (CSRF tokens can be built with `std/crypto.token()` + cookie comparison at the app level.)
- **Streaming responses** — chunked transfer encoding, SSE. Future.
- **Connection pooling** — SQLite WAL mode with one connection works for demos. Pool via gen_server later.
- **std/regex** — wrap POSIX `<regex.h>` (already on macOS/Linux, zero new dependencies). C wrapper ~80 lines (compile, match, extract captures, free), Gem module ~60 lines. API sketch: `regex.match(pattern, str)` → match table or nil, `regex.find_all(pattern, str)` → array of matches, `regex.replace(pattern, str, replacement)` → string. Not blocking for the CRUD demo (path params handle routing, string functions handle validation), but useful for input validation, log parsing, and more flexible routing later.
