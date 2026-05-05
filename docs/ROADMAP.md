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
- Single-box throughput is already decent (~26–29k rps on the HTTP bench); the next 10x is more likely from N nodes behind a load balancer than from squeezing the single-threaded scheduler.
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

## ~~`Bytes` extern type~~ ✓ Done (2026-05-05)

`extern fn foo(b: Bytes) -> Bytes` now marshals binary-safe data across the FFI boundary. A `Bytes` parameter expands to two C parameters `(const uint8_t *data, int64_t len)`, sourced from `args[i].sval` and `args[i].slen` so embedded NULs survive. A `Bytes` return uses the small `GemBytes { data, len }` struct from `gem.h` (with a `gem_bytes(ptr, len)` helper); the runtime copies into the calling process's arena via `gem_string_with_len`.

Ownership of returned data mirrors `String`: non-blocking `extern fn` does not free the original (use static storage or accept the leak); `extern blocking fn` frees with `free()` after copying. The blocking path also `memcpy`s `Bytes` arguments into a malloc'd thread-safe buffer for the duration of the worker call.

There is no separate Bytes type at the Gem level — a `Bytes` parameter accepts any Gem string (file contents, `tcp_read` output, `build_string` output), since strings are already binary-safe. The annotation only changes how the value crosses the C boundary. SPEC §C-Interop has the type mapping table, ownership rules, and construction recipes; `examples/90_bytes_extern.gem` is the regression. Editor grammars (VS Code, Helix tree-sitter) updated to recognise `Bytes` in extern type annotations.

## Debugger / breakpoints (P2)

Stack traces on `error()` are good; there's no interactive step-through, breakpoint, or variable-inspection story. Pairs with `LSP_ROADMAP.md` but is a separate capability — typically a DAP (Debug Adapter Protocol) server that the runtime cooperates with (instrumented `gem_set_line` callbacks, ability to pause a coroutine, mailbox/process inspection).

## ~~Compile-time errors point at user source, not compiler internals~~ ✓ Done (2026-05-05)

The three `error("unknown ... node/operator: ...")` sites in `compiler/codegen.gem` (unary op, binary op, expression dispatch) now route through a `codegen_error(node, msg)` helper inside `make_codegen` that captures `source_name`, reads `node.line`, and prints `[Compiler Bug]: <msg>\n  --> <file>:<line>` plus a "this is a compiler bug, not your code" note before exiting. Frames these as compiler bugs (not user errors) because the parser shouldn't produce shapes codegen can't handle — when one of these fires, the right fix is a parser/semantic guard upstream (cf. the `if x = y` guard from `79fb217`), and the diagnostic now gives a usable `.gem:line` to start from instead of the compiler's own stack trace.

Smaller scope than the original audit anticipated: there are exactly 3 such sites in codegen, not ~dozen. Other `[Compile Error]` paths in codegen (undeclared identifier, assign-to-fn) already had source-location formatting.
