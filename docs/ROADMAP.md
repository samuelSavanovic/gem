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

## TLS: what the client lacks, and servers (P3)

Outbound TLS exists: `tls_connect` on system OpenSSL 3, and `https://` in `std/request` (SPEC "TLS"). Not built:

- **Server-side TLS stays out of scope.** Terminate TLS at Caddy, nginx or a cloud load balancer in front of `std/http.serve`: the edge gets ACME certificate rotation, HTTP/2, and a far larger audit surface for its crypto than anything Gem would embed.
- **Client certificates (mutual TLS)**: `cert_file` / `key_file` options, for APIs that authenticate clients that way.
- **Upgrading an open socket** (`tls_upgrade(sock, opts)`, for STARTTLS in SMTP, IMAP, Postgres).
- **Session resumption and ALPN**: each request makes a full handshake; HTTP/2 would need ALPN and much more in `std/request`.
- **Handshake crypto off the scheduler thread**: about 1 ms per handshake on the scheduler thread, during which no other process runs. Fine for a client making a few requests at a time, not for hundreds of handshakes a second.
- **A vendored backend** (mbedTLS) for a build with no system OpenSSL, behind the same `tls_connect`, if a deployment without OpenSSL ever needs one. The cost is owning its security updates.

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

## Linker and compiler flags declared by modules (options under consideration)

The command line can add libraries and paths (`gem -I<dir> -L<dir> -l<lib> app.gem`, and `CC`, `CFLAGS`, `LDFLAGS`; SPEC "C Interop"), and the runtime's own OpenSSL flags come from `make build` (`runtime/tls_config.sh`). What is missing is a module saying what it needs, so that a program loading a module that wraps a C library doesn't have to repeat its flags. Options, probably both layers together:

- **Declared in the source, next to the externs**, e.g. `extern link "mbedtls"` / `extern pkg "openssl"`. Prior art: Nim's `{.passL.}`, cgo's `#cgo LDFLAGS` / `#cgo pkg-config`, Rust's `#[link(name = …)]`. A module wrapping a library would declare it, and its users never see it (CLAUDE.md, "Design Philosophy": don't leak concepts onto the user). The compiler gathers the declarations from every loaded module, removes duplicates and keeps load order (link order matters for static libraries).
- **`gem.toml` for flags that depend on the machine:** `-L` / `-I` paths, Homebrew prefixes, static versus dynamic linking. Today `gem.toml` is an empty marker for the project root (SPEC, "Project root marker").

Design include paths and compiler flags alongside the linker flags rather than as a later bolt-on, and consider `pkg-config` support (it answers the Homebrew-prefix question on macOS for libraries that ship a `.pc` file). Trade-offs: source-declared flags make a module's build depend on what is installed on the machine, which surfaces as a `cc` link error unless the compiler checks first; machine flags in `gem.toml` don't travel with a module once there is a package manager.

## Package manager / external dependencies (P2)

`load` today resolves stdlib (`std/...`) and project-local paths. There is no story for depending on third-party Gem code — no manifest, no fetch, no version pinning, no lockfile. Becomes pressing the moment a second real Gem app wants to share code with the first. Likely shape: dependencies declared in `gem.toml` (today an empty marker for the project root; SPEC "Project root marker"), a `gem_modules/` (or `.gem/deps/`) cache, git-URL or registry-based resolution, lockfile for reproducibility. The design waits for real users' needs.

## std API gaps (P2)

Std API that programs need and the std modules lack:

- **A gen_server stopping itself**: a callback ends its server by raising (reported as a crash) or with
  `kill(self(), reason)`, which a server that traps exits only gets as an `EXIT` message. Maybe a
  `{stop: reason, reply?, state}` callback result (Erlang's `{stop, ...}`).
- **Dropping late replies**: after a `gen_server.call`, `supervisor.which_children` or a
  `dynamic_supervisor` call times out, the late reply still lands in the caller's mailbox and nothing
  removes it. Erlang solves this with process aliases (a reply to a deactivated alias is dropped). Needs a
  runtime alias or a per-ref "drop" set checked at delivery.
- **Multi-value query keys in `url.build_query`**: `{tags: ["a", "b"]}` should give `tags=a&tags=b`; today
  the table's `to_string` text is encoded. `parse_query` would need a matching opt-in (last value wins now).
- **Stopping what a supervised child spawned**: `supervisor.stop` stops direct children only; a task a
  worker is awaiting keeps running (tasks aren't linked to their owner). A linked `task.async` variant, or
  tasks dying with their owner, would make a supervised shutdown complete.
- **More from the http request**: the raw query string and multi-value query/form parsing (the
  client address is `req.peer`). On the client side, cookie help in `std/request` (today: split `set-cookie` by hand).
- **One rule for what a child's `start` returns**: `supervisor` accepts a pid, a `{pid}` handle or a
  registered name; `dynamic_supervisor` rejects a name. Pick one for both.

## Compound assignment to fields and indexes (P2)

`n += 1` works on a variable, but `state.count += 1` and `counts[k] += 1` are compile errors (`compound assignment requires variable target`), so a gen_server callback writes `st.n = st.n + 1`. Lowering `t.f op= e` to `t.f = t.f op e`, with `t` (and an index expression) evaluated once into temporaries, would close it; the in-place string append path (compiler/append.gem) would need to learn the field form to keep `buf.text += piece` fast.

## Keep-alive connections in `std/request` (P2)

Every `request.<fn>` call opens a connection, sends `Connection: close` and closes it, so each request pays a TCP connect (a round trip to a remote host) and leaves a socket in TIME_WAIT. Measured on loopback against a Go server (4-core Linux VM, 2026-10-13): a Gem client with 100 processes doing GETs reached about 11,800 requests/s; a Go client opening a connection per request about 17,700, and reusing connections about 47,100. Gem's gap at a new connection per request is its one scheduler thread; reusing connections is the larger one, and it grows with the distance to the server.

What needs building: a pool of idle connections per host and port, reused by later calls in the same process.

- **Where the pool lives.** Module-level state is per process, which fits (each process's pool, its sockets claimed by it), but a `spawn` copies module state into the child: the pool must skip sockets another process owns, or key itself by `self()`. An explicit handle (`let c = request.client()`, then `c.get(url)`) avoids that and makes the lifetime plain, at the cost of a concept; the implicit pool keeps `request.get` as it is.
- **Stale connections.** A server closes an idle connection whenever it likes, so a reused socket can fail on its first write or read; the call retries once on a fresh connection for idempotent methods (GET, HEAD, PUT, DELETE), and never for POST and PATCH.
- **Bounds.** At most a few idle connections per host, an idle timeout shorter than common servers' (std/http's is 30 s by default), and responses read to their end before the socket goes back (a body cut short or `Connection: close` from the server drops it).
- **Name lookups.** A call to a host name looks it up unless a lookup of that name is already running (`getaddrinfo` on the resolver threads, runtime/gem_threadpool.c: a thread hand-off, and a round trip when the system doesn't cache the answer). A pool keyed by host name keeps the address it connected to for its connections' lifetime; `getaddrinfo` returns no TTL, so a cache of answers beyond that would hold one for a fixed short time.

## `gem doc` and checked doc examples (P3)

`##` doc comments (BEST_PRACTICES.md, "Document the public API with `##`") document the public API of std and user modules, but nothing reads them yet. Needs: a `gem doc <file>` subcommand that prints (or writes HTML for) a module's header and its exported functions' docs, with a comment collection an LSP hover could reuse; and a doctest pass that runs each `call    # result` example line and compares the printed value, wired into `make test` for std, so docs can't drift from behavior (as Rust's doctests do). Trade-off: examples have to stay self-contained one-liners for the checker; a multi-line example would need an explicit marker.

## Catching typos in option bags and message tags (P1; steps 1–2 done)

std configures most things with a table of string-named options (`supervisor.start({strategy: "one_for_all"})`, child specs, `http.start`), and a typo there is easy. A misspelled *value* is mostly caught already: `strategy: "one_for_al"`, `restart: "permanant"` and `log.set_level("warnign")` raise as the call runs. A misspelled *key* is not: a destructured option bag (`fn start({strategy = "one_for_one"} = {})`) ignores keys it doesn't name, so `{stratgy: "one_for_all"}` silently runs with the default, and `http.start` documents "other keys are ignored".

What needs building, in order (1–3 for option bags, 4 for messages):

Steps 1 and 2 are done (PLAN.md B6): a destructured parameter with an outer default (`{port = 80} = {}`) raises for an unknown key, `...rest` opts out, std checks its option tables and child specs, and the value checks name the closest value. Step 3 and 4 remain.

1. **Unknown keys raise, for every destructured option bag.** The compiler already emits the prelude that unpacks the bag; it also checks that the table has no other keys and raises `<fn>: unknown option "stratgy" (did you mean "strategy"?)`. Every std function and every user fn in that style gets it with nothing new to learn. A bag that means to take more keys says so with a rest entry in the pattern (`{port = 8080, ...rest}`), the opt-out. Trade-off: it changes behaviour; code that passes a larger table through on purpose starts raising, so the change finds those call sites first (std's "other keys are ignored" docs go).
2. **"Did you mean" on the value checks** std already makes (strategy, restart, log level), from a small edit-distance helper, and `process_flag: unknown flag` names the flag.
3. **The same check at compile time** where the compiler can see both sides: a direct call passing a table literal to a destructured parameter, as most `supervisor.start(...)` and `http.start(...)` calls do, gets the error at compile time, like a wrong argument count does today.

4. **Message tags no `send` produces.** A `receive` pattern with a misspelled tag never matches, and a `send` with one is dropped by the receiver's catch-all; Erlang has the same hole (a mailbox has no type), and typed actor systems (Gleam, Akka Typed) close it with a message type per process. Gem can close most of it without types: the compiler sees the whole program, std included, and every message comes from a `send` or `send_after` in it or from the runtime (`DOWN`, `EXIT`). It collects the literal tags the sends build and warns on a `receive` pattern whose literal tag none of them produces, naming the nearest tag that is produced (`warning: no send produces tag "_hp_flsh" (did you mean "_hp_flush"?)`). A send whose tag is not a literal can produce anything, so it keeps the check quiet for the patterns it could match; the warning never becomes an error. Possibly the reverse too (a literal tag no pattern names), though catch-alls and pass-through code make that one noisier. Until then, BEST_PRACTICES ("Keep mailboxes clean") has a main loop's catch-all log what it didn't expect, so a sender's typo shows. Trade-off: once distribution exists, a remote node's sends are invisible, and the check must become per-program or weaker.

Not planned: atoms/symbols or enum types. They add a concept, and they don't catch the typo (in Erlang `one_for_al` is a valid atom); checking the value is what catches it. Out of reach of all four: typos in a program's own string comparisons (`ev.kind == "requst"`), which just never match; warning on those needs the compiler to know which strings can reach a comparison.

## Debugger / breakpoints (P2)

Stack traces on `error()` are good; there's no interactive step-through, breakpoint, or variable-inspection story. Pairs with `LSP_ROADMAP.md` but is a separate capability — typically a DAP (Debug Adapter Protocol) server that the runtime cooperates with (instrumented `gem_set_line` callbacks, ability to pause a coroutine, mailbox/process inspection).

## Per-monitor refs (P2)

A process monitors a target at most once (`gem_monitor_fn` in `runtime/gem_scheduler.c`), so `demonitor(pid)` also drops a monitor the caller set up elsewhere; std code that monitors for the length of a request removes its monitor only when its `monitor` returned `true`. Entries of watchers that have exited are dropped lazily, by the next `monitor` of that target.

What needs building: Erlang-style refs (`monitor` returns a ref, `demonitor(ref)`), with a per-process list of the targets it monitors so an exiting process removes its entries eagerly. Trade-off: one more list per process, maintained on every `monitor`, and refs in the API.

## `exec` children as owned resources (P2)

Sockets and SQLite handles are owned resources (SPEC "Owned Resources", design in `docs/design/process_owned_resources.md`): values of their own types in one runtime table, closed when a process that claimed them (`claim(r)`) exits or when their opener crashes or is killed. A command started by `exec` is not one yet: it keeps running after its process is killed. Erlang ties every port to an owning process; supervisors make this pressing, since they kill a child with the untrappable `"kill"` once its `shutdown` budget runs out.

`exec` already starts its child with `posix_spawn` and waits with `waitpid` (`gem_run_shell`, runtime/gem_builtins_io.c). What remains: register the child as a third resource kind (a `GEM_RES_*` kind with a close that signals it, in `runtime/gem_resource.c`), which gives it `claim` and the exit close for free.

## IPv6 sockets (P3)

`tcp_listen` and `tcp_connect` take IPv4 addresses and names only (`gem_tcp_resolve4`, `socket(AF_INET, …)` in `runtime/gem_builtins_tcp.c`); `tcp_peer` and `tcp_from_fd` already handle IPv6. A server can't listen on `::`, so the honeypot answers on its host's IPv4 address only. Needs `getaddrinfo` with `AF_UNSPEC` (or `AF_INET6` with `IPV6_V6ONLY` off for a dual-stack listener) and `sockaddr_storage` throughout (`gem_resolve4`, runtime/gem_threadpool.c, asks for IPv4 only).

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
