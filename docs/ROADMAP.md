# Future Capabilities

Forward-looking features that are not yet implemented. Distinct from `OPTIMIZATIONS.md` (perf work on existing capabilities) and `LSP_ROADMAP.md` (tooling). Items here change what the language can *do*, not how fast it does it.

Priority scale: **P0** = next likely feature work, **P1** = clear value, larger scope, **P2** = speculative.

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
- Single-box throughput is already decent (~26–29k rps on the bookmark app's static page, M1 Pro, April 2026); the next 10x is more likely from N nodes behind a load balancer than from squeezing the single-threaded scheduler.
- The two are complementary long-term (BEAM has both), but distribution is the less invasive starting point.

**Trade-off:** cross-node sends pay serialization cost vs. a memcpy. Negligible for shared-nothing request/response workloads; matters for chatty cross-node protocols.

## TLS for sockets / HTTPS — deferred indefinitely

`std/http`, `std/request`, and the `tcp_*` builtins are plain TCP only. Rather than pull a TLS stack into the runtime, the deployment story is:

- **Inbound (servers):** terminate TLS at a reverse proxy (Caddy, nginx, or a cloud LB) in front of `std/http.serve`. This is what most Go and Node deployments do anyway — the edge gets free ACME cert rotation, HTTP/2, and a much larger crypto-bug audit surface than a vendored libtls would. Document this as the recommended deployment recipe.
- **Outbound (`std/request` against `https://`):** link libcurl and expose its easy API as an `extern blocking fn`, routed through the existing 4-worker thread pool (`gem_threadpool.c`). libcurl is on every mainstream system (macOS and the major Linux distros ship it), battle-tested for TLS/cert handling, and the easy API is blocking — which is exactly what `extern blocking fn` is designed for, so no scheduler changes are needed. `std/request` becomes a thin wrapper that hands URL/method/headers/body to a single C function returning status + headers + body bytes. No process-spawn cost, real error codes, binary-safe responses via the `Bytes` extern type.

Reasons to revisit (i.e. actually vendor a TLS stack): someone wants a single self-contained Gem binary that serves HTTPS directly with no reverse proxy, or a workload where libcurl's blocking-call-per-request ergonomics become limiting (e.g. needing thousands of concurrent outbound requests, where the multi API + scheduler `poll()` integration would start to pay off). Until then, the integration cost (vendoring LibreSSL, threading TLS_WANT_POLLIN/OUT through the non-blocking path, handshake-as-coroutine-yield) buys very little over the recipe above.

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

## Package manager / external dependencies (P2)

`load` today resolves stdlib (`std/...`) and project-local paths. There is no story for depending on third-party Gem code — no manifest, no fetch, no version pinning, no lockfile. Becomes pressing the moment a second real Gem app wants to share code with the first. Likely shape: a `gem.toml` manifest, a `gem_modules/` (or `.gem/deps/`) cache, git-URL or registry-based resolution, lockfile for reproducibility. Design intentionally deferred until pull from real users.

## `gem doc` and checked doc examples (P3)

`##` doc comments (BEST_PRACTICES.md, "Document the public API with `##`") document the public API of std and user modules, but nothing reads them yet outside the editor. Needs: a `gem doc <file>` subcommand that prints (or writes HTML for) a module's header and its exported functions' docs, using the same comment collection as LSP hover; and a doctest pass that runs each `call    # result` example line and compares the printed value, wired into `make test` for std, so docs can't drift from behavior (as Rust's doctests do). Trade-off: examples have to stay self-contained one-liners for the checker; a multi-line example would need an explicit marker.

## Debugger / breakpoints (P2)

Stack traces on `error()` are good; there's no interactive step-through, breakpoint, or variable-inspection story. Pairs with `LSP_ROADMAP.md` but is a separate capability — typically a DAP (Debug Adapter Protocol) server that the runtime cooperates with (instrumented `gem_set_line` callbacks, ability to pause a coroutine, mailbox/process inspection).

## Per-monitor refs (P2)

A process monitors a target at most once (`gem_monitor_fn` in `runtime/gem_scheduler.c`), so `demonitor(pid)` also drops a monitor the caller set up elsewhere; std code that monitors for the length of a request removes its monitor only when its `monitor` returned `true`. Entries of watchers that have exited are dropped lazily, by the next `monitor` of that target.

What needs building: Erlang-style refs (`monitor` returns a ref, `demonitor(ref)`), with a per-process list of the targets it monitors so an exiting process removes its entries eagerly. Trade-off: one more list per process, maintained on every `monitor`, and refs in the API.

## Named sqlite parameters (P3)

`sqlite_query` takes an array of params; a `:name` placeholder binds by its position. A record (`{a: 1, b: 2}`) raises. What needs building: bind a string-keyed params table by name (`sqlite3_bind_parameter_index`, trying the `:`, `@` and `$` prefixes). Trade-off: none beyond the code; arrays keep working.

## Process-owned resources closed on exit (P2)

TCP sockets and SQLite handles are plain ints. A process that crashes or is killed without closing them leaks the file descriptor or connection; Erlang ties a port to an owning process and closes it when the owner exits. Likewise a command started by `exec` keeps running after its process is killed, because `system()` does not expose the child's pid.

What needs building: a per-process resource list filled by `tcp_listen`/`tcp_accept`/`tcp_connect`/`sqlite_open` and closed in `gem_free_proc_slot`; for `exec`, `posix_spawn` + `waitpid` so the child can be signalled. Trade-off: a handle passed to another process (an acceptor handing a socket to a handler) needs ownership to move with it. Making the user transfer ownership explicitly would add a concept to the language, so the transfer should happen implicitly, e.g. on `send` or `spawn` capture.

## Write timeout for `tcp_write` (P2)

`tcp_write` loops until every byte is written and has no timeout (`gem_tcp_write_fn` in `runtime/gem_builtins_tcp.c`), so a peer that stops reading blocks the writer for as long as it keeps the connection open. In `std/http` such a client holds its connection process (and a process-table slot) forever: the server's idle and request timeouts only cover reads. `std/request` likewise can't bound the write of a large request body.

What needs building: an optional `timeout_ms` argument, `tcp_write(fd, data, timeout_ms)`, using the same per-process deadline as `tcp_read`, that returns the number of bytes written before the deadline (so the caller can tell a partial write); then `std/http` passes a write deadline for each response and `std/request` counts the write against its `timeout_ms`. Trade-off: callers must check the count, which they already should (see BEST_PRACTICES "Pass timeouts to reads, check writes").

## Shared read-mostly data between processes (P2)

Module-level bindings are per-process (SPEC "Module-level bindings are per-process"): every process has its own copy, writes stay local, and `spawn` copies the parent's module state. Sharing mutable state means a process plus messages, which is the right default but makes large read-mostly data (a config tree, a routing table, a lookup cache) cost a copy per spawn or a message round-trip per read.

What needs building: an ETS-like store owned by a process, whose entries live outside any arena (immortal or refcounted copies) and can be read from any process without a round-trip, written only through its owner. Trade-offs: a new concept to learn (keep it a std module, not syntax), copy-on-read vs. handing out immutable shared values (needs an immutability flag on tables, which namespace tables already have), and freeing entries that are overwritten while another process still reads them.

## Deep non-tail recursion ceiling (P3)

Every process, main included, runs on an 8 MB stack (`GEM_CORO_STACK_SIZE` in `runtime/gem.h`, `GEM_MAIN_STACK_SIZE` in `runtime/gem_scheduler.c`). The stacks are mmap'd, so only the pages a process touches cost memory. That is roughly 30,000 frames of a small recursive function, and about 2,000 nesting levels for the `std/json` parser (3,000 for the encoder; both refuse more than 1,000, see SPEC). Recursing past it no longer crashes the program. A Gem call that would run into the bottom 256 KB raises `"stack overflow in <fn>"`, which `pcall` catches. Native code that overflows on its own (a recursive C function behind an `extern fn`) hits a guard page, and only the offending process dies; the runtime's value copies and frees are iterative, so deep data cannot get there. SPEC §"Stack depth" has the user-facing rules. Tail calls, self or mutual within one tail-call cycle, do not consume stack at all (see `OPTIMIZATIONS_LOG.md` §"Mutual TCO via tail-edge SCC trampoline").

What's left:

- **Growable stacks.** The depth bound itself remains: a recursion that needs more than 8 MB fails, cleanly. Removing the bound means growing stacks on demand (copying stacks, or segmented stacks with a fault-driven grow path). High cost: minicoro has no support, pointers into the stack would have to be fixed up, and the signal-handler path gets harder. The motivation is programs that want unbounded recursion to *succeed*, e.g. recursive descent over adversarially deep input without a depth cap. So far an explicit depth limit (as `std/json` has) has been the better answer.
- **Pcall for guard-page overflows.** An overflow caught by the guard page (inside C code) always ends the process, because the C code it interrupts may hold half-updated state. Only `extern fn` code can get there now (`gem_deep_copy` and `gem_deep_free` are iterative); making it catchable would need a contract for what an interrupted extern may leave behind.

## Exponent syntax in float literals (P3)

`1e6` doesn't lex as a number (it reads as `1` followed by the name `e6`), while `to_string` writes floats below `1e-4` or from `1e16` up in exponent form (`1e-05`, `1e+16`). So the text a float prints as isn't always a valid Gem literal; `to_float("1e-7")` is the way to write one today. Needs: the lexer accepting `<digits>[.<digits>](e|E)[+-]<digits>` as a float (an exponent makes it a float even without a dot, as in C), the editor grammars, SPEC "Numbers". Trade-off: none beyond the work; `1e6` currently fails to compile, so nothing changes meaning.
