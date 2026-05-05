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

## TLS for sockets / HTTPS (P1)

`std/http`, `std/request`, and the `tcp_*` builtins are plain TCP only. For a language aimed at servers this is the most obvious real-world gap — most production deployments need to terminate TLS somewhere, and outbound HTTPS is needed even for trivial integrations.

**Likely shape:**

- Link libtls (LibreSSL) or BoringSSL into the runtime; vendor as a single amalgamation if feasible (matches the SQLite precedent).
- Add `tls_listen` / `tls_accept` / `tls_connect` / `tls_read` / `tls_write` / `tls_close` builtins paralleling the `tcp_*` shape, integrating with the same non-blocking + `poll()` readiness path so the scheduler model is unchanged.
- Extend `std/http.serve` with a TLS variant (cert/key paths) and `std/request` to accept `https://` URLs.
- Certificate verification on by default; an explicit insecure flag for dev.

Could initially be prototyped via `extern fn` against libtls before being promoted to first-class builtins.

## Hot code reload (P2)

Replace a module's compiled code in a running node without dropping state. Erlang ships this; it pairs naturally with distribution (the wire format for closures already needs to identify functions by id, not pointer).

**Mechanism (Erlang model):** functions addressed by `{module, fn, version}` through a per-module dispatch table the runtime swaps atomically; two versions of a module live concurrently (current + old) so in-flight calls finish on the version they started in; processes hop to new code on the next fully-qualified call (`mymod.fn(...)`), which is the convention for `gen_server`-style loops.

**What Gem would need:**

- Stable function identity — closures carry `{module_id, fn_id}` and resolve through a dispatch table rather than a raw C pointer. (Same change distribution needs for serializing closures.)
- Per-module compilation unit — today the whole program compiles to one C translation unit; hot reload needs each module to be a separately-loadable artifact (likely `dlopen`'d shared library).
- State migration hook (analogue of Erlang's `code_change/3`) for `gen_server` callback upgrades.
- Purge policy — track which processes execute which version; refuse or force-kill on third load.

**Cost:** the dispatch-table mechanics are straightforward; the real cost is that the current compilation model (whole-program `cc`, leaf-fn elision, cross-fn inlining, TCO into `while(1)` loops) actively fights per-module separability. Probably means maintaining two compilation modes (whole-program for standalone, separate-module for hot-reload-enabled deployments). Out of scope until distribution lands.

## Package manager / external dependencies (P2)

`load` today resolves stdlib (`std/...`) and project-local paths. There is no story for depending on third-party Gem code — no manifest, no fetch, no version pinning, no lockfile. Becomes pressing the moment a second real Gem app wants to share code with the first. Likely shape: a `gem.toml` manifest, a `gem_modules/` (or `.gem/deps/`) cache, git-URL or registry-based resolution, lockfile for reproducibility. Design intentionally deferred until pull from real users.

## `Bytes` extern type (P1)

Gem strings are binary-safe internally (slen-tracked, embedded NULs preserved), but `extern fn foo(s: String)` marshals as `const char *` — the C side calls `strlen`/walks until NUL, so a Gem string with embedded NULs is silently truncated at the FFI boundary. Today the workaround is to pass a `Buffer`, but `Buffer` doesn't currently round-trip cleanly through `extern fn` either.

**What needs building:**

- A `Bytes` annotation in `extern fn` signatures that marshals as two C parameters: `const char *data, int len`. Mirrors how most C libraries take binary data.
- Same on the return side — `-> Bytes` marshals as out-params or a small struct, decision TBD.
- Codegen: extend `extern-type-annotation` in both editor grammars; update SPEC's C interop section.
- Keep `String` extern marshaling as-is (NUL-terminated `const char *`) since most C string APIs want that.

Small change, big quality-of-life win for protocol/binary work that bridges to C libs (compression, crypto, parsing). Pairs naturally with the binary-safe-string work already done.

## Debugger / breakpoints (P2)

Stack traces on `error()` are good; there's no interactive step-through, breakpoint, or variable-inspection story. Pairs with `LSP_ROADMAP.md` but is a separate capability — typically a DAP (Debug Adapter Protocol) server that the runtime cooperates with (instrumented `gem_set_line` callbacks, ability to pause a coroutine, mailbox/process inspection).

## ~~Compile-time errors point at user source, not compiler internals~~ ✓ Done (2026-05-05)

The three `error("unknown ... node/operator: ...")` sites in `compiler/codegen.gem` (unary op, binary op, expression dispatch) now route through a `codegen_error(node, msg)` helper inside `make_codegen` that captures `source_name`, reads `node.line`, and prints `[Compiler Bug]: <msg>\n  --> <file>:<line>` plus a "this is a compiler bug, not your code" note before exiting. Frames these as compiler bugs (not user errors) because the parser shouldn't produce shapes codegen can't handle — when one of these fires, the right fix is a parser/semantic guard upstream (cf. the `if x = y` guard from `79fb217`), and the diagnostic now gives a usable `.gem:line` to start from instead of the compiler's own stack trace.

Smaller scope than the original audit anticipated: there are exactly 3 such sites in codegen, not ~dozen. Other `[Compile Error]` paths in codegen (undeclared identifier, assign-to-fn) already had source-location formatting.
