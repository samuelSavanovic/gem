# Future Capabilities

Forward-looking features that are not yet implemented. Distinct from `OPTIMIZATIONS.md` (perf work on existing capabilities) and `LSP_ROADMAP.md` (tooling). Items here change what the language can *do*, not how fast it does it.

Priority scale: **P0** = next likely feature work, **P1** = clear value, larger scope, **P2** = speculative, **P3** = small, or waiting for a program that needs it.

## Distribution / horizontal scaling (P1)

Run Gem across multiple OS processes (same box or across the network), with `send` working transparently to remote pids. The actor model is a natural fit — Erlang's distribution is the canonical example — and Gem's existing semantics line up well:

- Values already deep-copy on `send` across local processes; serializing to bytes for a remote send is the same operation with a different transport.
- No shared memory between Gem processes, so there's no cache coherence or locking story across nodes.
- `monitor`/`link` already assume the peer can vanish at any time — a dropped TCP connection is just another `{DOWN, ...}` message.

**What needs to be built:**

- **Wire format for `GemVal`.** Primitives, strings, arrays, tables are straightforward. Closures need function id + captured env, with both nodes running the same compiled code (BEAM's "external term format" is the reference). Independent value: snapshots, debugging-over-the-wire, eventual hot reload.
- **Pid identity extended with a node id** — `{node_id, local_pid}`. `send` checks the node tag and either enqueues locally or writes to the peer connection.
- **Per-peer TCP connection** with a small framing protocol; multiplex all cross-node sends over it.
- **Handshake / naming layer** — `Node.connect("host:port")`, `Node.spawn(node, fn)`, optional global name registry (or leave it to user-level via `gen_server`).

**Why this over a multithreaded scheduler (also P2 in `OPTIMIZATIONS.md`):**

- Easier to build correctly — no lock-free data structures, no memory-model reasoning. A bug at worst drops a connection.
- Scales further — past one box, which multithreading can't.
- Single-box throughput is already decent (~90k rps on the bookmark app's static page, M1 Pro, `benchmarks/baselines/2026-10-07_m1pro`); the next 10x is more likely from N nodes behind a load balancer than from squeezing the single-threaded scheduler.
- The two are complementary long-term (BEAM has both), but distribution is the less invasive starting point.

**Trade-off:** cross-node sends pay serialization cost vs. a memcpy. Negligible for shared-nothing request/response workloads; matters for chatty cross-node protocols.

## TLS for sockets / HTTPS (options under consideration)

`std/http`, `std/request`, and the `tcp_*` builtins are plain TCP only.

**Inbound (servers): the recommended deployment stays a reverse proxy.** Terminate TLS at Caddy, nginx or a cloud load balancer in front of `std/http.serve`. The edge gets ACME certificate rotation, HTTP/2, and a far larger audit surface for its crypto than anything Gem would embed. Server-side TLS in `std/http` is out of scope for every option below.

**Outbound (`std/request` against `https://`) and native TLS: options being weighed, none chosen.**

- **libcurl via `extern blocking fn`.** Expose the easy API as one blocking C function (URL, method, headers, body in; status, headers, body bytes out) on the existing 4-worker thread pool (`gem_threadpool.c`), with `std/request` as a thin wrapper. Outbound only, about a day of work, no scheduler changes. Costs: a system dependency (needs "Linker and compiler flags for C interop", below), and each in-flight request occupies one of the 4 workers, so concurrency is capped at 4.
- **Native `tls_*` builtins** (`tls_connect` / `tls_read` / `tls_write` / `tls_close`, mirroring `tcp_*`). The handshake, reads and writes park the process on `poll()` the way `tcp_read` does. The twist: any TLS call can ask to wait until the socket is readable *or* writable, and a write may have to wait for a read, so the parking path takes the direction from the TLS library rather than from the call. The TLS context is a malloc'd C object kept in a table keyed by fd. `std/request` gains an `https://` branch. Library choices:
  - **System OpenSSL or libretls, linked dynamically.** libretls is the libtls API on top of OpenSSL, so it still needs OpenSSL. The distro ships the security updates. macOS is awkward: no libtls by default, OpenSSL from Homebrew. Also needs the linker-flags work below.
  - **Vendored mbedTLS, 3.6 LTS line** (4.0 moved its crypto into TF-PSA-Crypto and changed APIs). A self-contained binary; maintained, TLS 1.3. Its callback-based I/O fits parked processes, and a custom config header trims it to client-only, no DTLS, no legacy ciphers. The cost: **security updates become our job**. mbedTLS publishes advisories regularly, and someone has to watch them and bump the vendored copy.
  - Ruled out: wolfSSL (GPL or commercial licence), BearSSL (no release since 2018, no TLS 1.3), vendoring LibreSSL or OpenSSL (too large).
  - Shared concerns, whatever the library: loading the CA bundle (its path differs per distro, and macOS keeps roots in the Keychain); hostname verification on by default; a few milliseconds of handshake crypto on the scheduler thread, during which no other process runs; a TLS stream should be an owned resource like a socket (see "Process-owned resources closed on exit"). Testing in `make test` needs a local `openssl s_server` and a self-signed CA.
  - A possible later step: both backends behind the same `tls_*` API, chosen per build.

## Hot code reload (P2)

Replace a module's compiled code in a running node without dropping state. Erlang ships this; it pairs naturally with distribution (the wire format for closures already needs to identify functions by id, not pointer).

**Mechanism (Erlang model):** functions addressed by `{module, fn, version}` through a per-module dispatch table the runtime swaps atomically; two versions of a module live concurrently (current + old) so in-flight calls finish on the version they started in; processes hop to new code on the next fully-qualified call (`mymod.fn(...)`), which is the convention for `gen_server`-style loops.

**What Gem would need:**

- Stable function identity — closures carry `{module_id, fn_id}` and resolve through a dispatch table rather than a raw C pointer. (Same change distribution needs for serializing closures.)
- Per-module compilation unit — today the whole program compiles to one C translation unit; hot reload needs each module to be a separately-loadable artifact (likely `dlopen`'d shared library).
- State migration hook (analogue of Erlang's `code_change/3`) for `gen_server` callback upgrades.
- Purge policy — track which processes execute which version; refuse or force-kill on third load.

**Cost:** the dispatch-table mechanics are straightforward; the real cost is that the current compilation model (whole-program `cc`, leaf-fn elision, cross-fn inlining, TCO into `while(1)` loops) actively fights per-module separability. Probably means maintaining two compilation modes (whole-program for standalone, separate-module for hot-reload-enabled deployments). Out of scope until distribution lands.

## `extern struct` for less painful C struct wrapping (P2)

Today, wrapping a C library that uses small structs by value (raylib's `Vector2`/`Color`/`Rectangle`, SDL events, most graphics APIs) requires hand-written C shims that flatten every struct into primitive params: `DrawCircleV(Vector2, float)` becomes a wrapper `gem_draw_circle(float x, float y, float r)`. Tedious for any non-trivial binding surface.

**Cheap version (the only one worth doing):** add `extern struct Name { field: Type, ... }` syntax. The compiler treats it as a Gem-side fixed-shape table that gets flattened to a flat parameter list at extern call sites — a `{x: 10, y: 20}` argument to a `Vector2` param expands to two floats on the C side. The C function still takes primitives; no struct ever crosses the FFI boundary. Returns work the same way: an `extern fn` returning a struct fills out-params or returns a small `GemStruct`-style helper, then the runtime constructs the Gem table.

**Why not "real" struct ABI support:** passing structs by value directly to unmodified C functions means reimplementing libffi's classification logic per platform (arm64 AAPCS, x86-64 SysV, Windows x64 all differ in subtle ways for small POD structs). Owning that forever is a bad trade for a feature that exists to make graphics bindings less verbose. The shim layer the cheap version requires (one C function per extern, taking primitives) is something binding authors would write anyway.

**Pairs well with a bindings generator** — small tool that reads a manifest (or libclang-parsed header) and emits both the Gem `extern struct`/`extern fn` decls and the matching C shim. Without the generator, `extern struct` is still a real ergonomic win; with it, writing a raylib binding becomes a manifest edit.

**Why P2:** no current user — Gem isn't aimed at game/graphics bindings, and the existing hand-shim path works for the small surfaces that have come up. Worth keeping on the list because it's a clean addition (extern is already the typed island in Gem; struct shape just extends what's expressible there) and because "can you wrap raylib and write a 2D game" is the kind of question that surfaces a language's FFI ergonomics.

## Linker and compiler flags for C interop (options under consideration)

A module can declare `extern fn`s and `extern include` a header, but not the C libraries those need: the compiler builds every program with a fixed `cc` command (`-pthread … -lm`, in compiler/main.gem), so an extern into a library outside the runtime and the libraries that command links fails to link. The libcurl option and native TLS on a non-vendored library (see "TLS for sockets / HTTPS") both depend on this. Options, probably both layers together:

- **Declared in the source, next to the externs**, e.g. `extern link "mbedtls"` / `extern pkg "openssl"`. Prior art: Nim's `{.passL.}`, cgo's `#cgo LDFLAGS` / `#cgo pkg-config`, Rust's `#[link(name = …)]`. A std module such as a future `std/tls` would declare its own libraries, and its users never see them (CLAUDE.md, "Design Philosophy": don't leak concepts onto the user). The compiler gathers the declarations from every loaded module, removes duplicates and keeps load order (link order matters for static libraries).
- **`gem.toml` for flags that depend on the machine:** `-L` / `-I` paths, Homebrew prefixes, static versus dynamic linking. Today `gem.toml` is an empty marker for the project root (SPEC, "Project root marker").

Design include paths and compiler flags alongside the linker flags rather than as a later bolt-on, and consider `pkg-config` support (it answers the Homebrew-prefix question on macOS for libraries that ship a `.pc` file). Trade-offs: source-declared flags make a module's build depend on what is installed on the machine, which surfaces as a `cc` link error unless the compiler checks first; machine flags in `gem.toml` don't travel with a module once there is a package manager.

## Package manager / external dependencies (P2)

`load` today resolves stdlib (`std/...`) and project-local paths. There is no story for depending on third-party Gem code — no manifest, no fetch, no version pinning, no lockfile. Becomes pressing the moment a second real Gem app wants to share code with the first. Likely shape: dependencies declared in `gem.toml` (today an empty marker for the project root; SPEC "Project root marker"), a `gem_modules/` (or `.gem/deps/`) cache, git-URL or registry-based resolution, lockfile for reproducibility. The design waits for real users' needs.

## std API gaps (P2)

Std API that programs need and the std modules lack:

- **`gen_server.stop`**: there is no way to stop a server except `kill(h.pid, reason)` or a supervisor. Add
  `stop(target, reason = "normal", timeout_ms = 5000)` that waits for the exit, like `supervisor.stop`,
  and maybe a `{stop: reason, reply?, state}` callback result (Erlang's `{stop, ...}`).
- **Dropping late replies**: after a `gen_server.call`, `supervisor.which_children` or a
  `dynamic_supervisor` call times out, the late reply still lands in the caller's mailbox and nothing
  removes it. Erlang solves this with process aliases (a reply to a deactivated alias is dropped). Needs a
  runtime alias or a per-ref "drop" set checked at delivery.
- **`http.json_response` with a status**: a JSON 201/404 needs `http.response(status, {"Content-Type":
  ...}, json.encode(x))`. Add an optional `status` param.
- **The port bound by `http.start({port: 0})`**: an ephemeral port works but nothing reports it, so tests
  can't use one. Return `{pid, port}` (needs the bound port from `tcp_listen`, e.g. a `tcp_local_port`
  builtin).
- **Multi-value query keys in `url.build_query`**: `{tags: ["a", "b"]}` should give `tags=a&tags=b`; today
  the table's `to_string` text is encoded. `parse_query` would need a matching opt-in (last value wins now).
- **Parsing dates in `std/time`**: nothing turns an ISO 8601 or HTTP date back into epoch ms, and `format`
  has no millisecond directive. A log analyzer had to hand-write days-from-civil.
- **`json.encode` pretty-printing**: an `indent` option.
- **Stopping what a supervised child spawned**: `supervisor.stop` stops direct children only; a task a
  worker is awaiting keeps running (tasks aren't linked to their owner). A linked `task.async` variant, or
  tasks dying with their owner, would make a supervised shutdown complete.
- **More from the http request**: the client address, the raw query string and multi-value query/form
  parsing. On the client side, cookie help in `std/request` (today: split `set-cookie` by hand).
- **Naming a gen_server at start**: `gen_server.start(module, {name})` that registers before `init`, so a
  supervised restart can't leave a window where the name is unregistered.
- **More `std/mime` types**: `text/javascript` for `ext`, `.wav`, `.ogg`, `.md`, `.yaml`, `.map`.
- **One rule for what a child's `start` returns**: `supervisor` accepts a pid, a `{pid}` handle or a
  registered name; `dynamic_supervisor` rejects a name. Pick one for both.

## `gem doc` and checked doc examples (P3)

`##` doc comments (BEST_PRACTICES.md, "Document the public API with `##`") document the public API of std and user modules, but nothing reads them yet. Needs: a `gem doc <file>` subcommand that prints (or writes HTML for) a module's header and its exported functions' docs, with a comment collection an LSP hover could reuse; and a doctest pass that runs each `call    # result` example line and compares the printed value, wired into `make test` for std, so docs can't drift from behavior (as Rust's doctests do). Trade-off: examples have to stay self-contained one-liners for the checker; a multi-line example would need an explicit marker.

## Debugger / breakpoints (P2)

Stack traces on `error()` are good; there's no interactive step-through, breakpoint, or variable-inspection story. Pairs with `LSP_ROADMAP.md` but is a separate capability — typically a DAP (Debug Adapter Protocol) server that the runtime cooperates with (instrumented `gem_set_line` callbacks, ability to pause a coroutine, mailbox/process inspection).

## Per-monitor refs (P2)

A process monitors a target at most once (`gem_monitor_fn` in `runtime/gem_scheduler.c`), so `demonitor(pid)` also drops a monitor the caller set up elsewhere; std code that monitors for the length of a request removes its monitor only when its `monitor` returned `true`. Entries of watchers that have exited are dropped lazily, by the next `monitor` of that target.

What needs building: Erlang-style refs (`monitor` returns a ref, `demonitor(ref)`), with a per-process list of the targets it monitors so an exiting process removes its entries eagerly. Trade-off: one more list per process, maintained on every `monitor`, and refs in the API.

## Line-at-a-time input from files and stdin (P2)

A program can read a file only whole (`read_file`), so a log analyzer
holds the whole log in memory (twice at the peak, see OPTIMIZATIONS.md)
and a file larger than memory, or than the 2 GiB string limit, can't be
processed at all. stdin has `input()` (one line), `read_stdin(n)`
(n bytes) and `read_file("/dev/stdin")` (all of it, held whole).
`examples/logstat` reads files and stdin.

What needs building: a line reader over a file or stdin, e.g.
`read_lines(path) do |line| ... end` with `"-"` for stdin, or an open
handle with `read_line(h)` and `close(h)`, plus `read_stdin()` with no count
reading to EOF. The callback form keeps the handle out of the user's hands
(no leak when the process dies), runs inside one loop the compiler can
reset at each line, and needs no new concept. Reads should go through the
I/O thread pool in large blocks, like `read_file`, so they don't block
every process the way `input()` does.

## Process-owned resources closed on exit (P2)

Sockets and SQLite handles are owned resources (SPEC "Owned Resources", design in `docs/design/process_owned_resources.md`): values of their own types in one runtime table, closed when a process that claimed them (`claim(r)`) exits or when their opener crashes or is killed. A command started by `exec` is not one yet: it keeps running after its process is killed, because `system()` does not expose the child's pid. Erlang ties every port to an owning process; supervisors make this pressing, since they kill a child with the untrappable `"kill"` once its `shutdown` budget runs out.

What remains:

- `exec`'s child process as a third resource kind: `posix_spawn` + `waitpid` so the child can be signalled when its owner dies. It registers a close callback in `runtime/gem_resource.c` and gets `claim` for free.

## Shared read-mostly data between processes (P2)

Module-level bindings are per-process (SPEC "Module-level bindings are per-process"): every process has its own copy, writes stay local, and `spawn` copies the parent's module state. Sharing mutable state means a process plus messages, which is the right default but makes large read-mostly data (a config tree, a routing table, a lookup cache) cost a copy per spawn or a message round-trip per read.

What needs building: an ETS-like store owned by a process, whose entries live outside any arena (immortal or refcounted copies) and can be read from any process without a round-trip, written only through its owner. Trade-offs: a new concept to learn (keep it a std module, not syntax), copy-on-read vs. handing out immutable shared values (needs an immutability flag on tables, which namespace tables already have), and freeing entries that are overwritten while another process still reads them.

## Deep non-tail recursion ceiling (P3)

Every process runs on an 8 MB stack (`GEM_CORO_STACK_SIZE` in `runtime/gem.h`, `GEM_MAIN_STACK_SIZE` in `runtime/gem_scheduler.c`), roughly 30,000 frames of a small recursive function; a Gem call past it raises `"stack overflow in <fn>"` (SPEC "Stack depth"). Tail calls don't use stack.

Not covered:

- **Growable stacks.** The depth bound itself remains: a recursion that needs more than 8 MB fails, cleanly. Removing the bound means growing stacks on demand (copying stacks, or segmented stacks with a fault-driven grow path). High cost: minicoro has no support, pointers into the stack would have to be fixed up, and the signal-handler path gets harder. The motivation is programs that want unbounded recursion to *succeed*, e.g. recursive descent over adversarially deep input without a depth cap. An explicit depth limit (as `std/json` has) covers that case.
- **Pcall for guard-page overflows.** An overflow caught by the guard page (inside C code) always ends the process, because the C code it interrupts may hold half-updated state. Only `extern fn` code can reach it (`gem_deep_copy` and `gem_deep_free` are iterative); making it catchable would need a contract for what an interrupted extern may leave behind.

## Exponent syntax in float literals (P3)

`1e6` doesn't lex as a number (it reads as `1` followed by the name `e6`), while `to_string` writes floats below `1e-4` or from `1e16` up in exponent form (`1e-05`, `1e+16`). So the text a float prints as isn't always a valid Gem literal; `to_float("1e-7")` is the way to write one today. Needs: the lexer accepting `<digits>[.<digits>](e|E)[+-]<digits>` as a float (an exponent makes it a float even without a dot, as in C), the editor grammars, SPEC "Numbers". Trade-off: none beyond the work; `1e6` currently fails to compile, so nothing changes meaning.
