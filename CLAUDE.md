# Gem Language

A dynamically typed language that compiles to C with Erlang-style concurrency. See `docs/SPEC.md` for the full spec.

## Design Philosophy

**Gem the language must be simple to write.** The OTP model (supervisors, gen_servers, "let it crash", spawn/send/receive) is already the learnability tax — that's the cognitive ceiling. The language layer must not pile more on top.

When evaluating a runtime or codegen mechanism, ask: *does this leak a concept the user has to track to write correct or performant code?* If yes, push the burden into the compiler (static analysis) or the runtime (uniform mechanism), not the user.

For example:
- Long-running process loops are written naturally with `while true` (or self-recursion). The compiler does liveness analysis and emits per-iteration arena resets at the back-edge so memory does not grow. No `@process_loop` annotation, no rewriting code as recursion.
- No "you must structure your code this way for X to work" runtime constraints. Either auto-detect via static analysis or make the mechanism work uniformly.
- Compiler warnings are OK when they surface a hidden constraint ("this loop won't reset because it's reachable from a non-tail context"). Silent perf cliffs are not.
- Optimizations that introduce DX warts (depth fences, hand-inlined wrappers, manual aliasing tricks) are tolerable only as transient hacks with a structural fix on the roadmap.

## Project Structure

```
compiler/             # self-hosting compiler (lexer, parser, AST, errors, liveness, lower, fold, codegen, loader, main)
lsp/                  # language server: main (entry), rpc (framing+types), server (loop), handlers (lifecycle+textDocument), doc (per-uri process). Dispatched from compiler/main.gem when argv[1] == "lsp"
std/                  # standard library (string, table, math, time, log, http, request, json, url, mime, sqlite, supervisor, dynamic_supervisor, gen_server, test)
runtime/              # C runtime — split by category:
  gem.h               #   public API, tagged values, process table, scheduler decls
  gem_core.c          #   value constructors, table internals, equality
  gem_copy.c          #   deep copy, pin-set, arena reset (rescue+reset path)
  gem_arena.c         #   per-process arena allocator
  gem_ops.c           #   arithmetic and comparison operators
  gem_error.c         #   error handling, stack trace printing
  gem_scheduler.c     #   coroutine scheduler, mailbox, process lifecycle, I/O polling
  gem_threadpool.c    #   worker thread pool for blocking I/O (4 workers)
  gem_builtins_core.c #   print, error, len, type, conversions, pcall, argv, etc.
  gem_builtins_collection.c  # push, pop, keys, values, sort, insert, delete
  gem_builtins_string.c      # str_replace, substr, chr/ord, buf_* API
  gem_builtins_math.c        # math ops, random, bitwise operations
  gem_builtins_io.c          # file I/O, filesystem ops, exec
  gem_builtins_tcp.c         # TCP socket operations (non-blocking)
  gem_builtins_sqlite.c      # SQLite operations (open, close, exec, query, etc.)
  gem_builtins_time.c        # time/clock builtins
  minicoro.h          #   single-header coroutine library (vendored)
  stb_ds.h            #   single-header hash maps/arrays (vendored)
  sqlite3.c/sqlite3.h #   SQLite amalgamation (vendored)
bootstrap/stage0.c    # checked-in C output — bootstrap artifact for clean builds
build/gem             # compiled compiler binary (gitignored, built from stage0.c)
examples/             # numbered tests (01-83+) + run_all.sh; plus json_parser, http_server, tcp_echo, bookmark_app
docs/SPEC.md          # language spec (source of truth for all language decisions)
docs/OPTIMIZATIONS.md     # tracked future performance improvements
docs/OPTIMIZATIONS_LOG.md # shipped optimizations + work logs + benchmark anchors
docs/ROADMAP.md           # future capabilities (features, not perf)
docs/LSP_ROADMAP.md       # Gem LSP plan + deferred v2 features
editors/vscode/       # VS Code extension (TextMate grammar)
editors/tree-sitter-gem/  # tree-sitter grammar for Helix (+ queries)
benchmarks/           # wrk harness for examples/bookmark_app (run.sh) + node_baseline reference impl
                      #   stomp/                 — Python STOMP load harness for examples/stomp_broker (M6 lived-experience numbers)
```

## Build, Test, Bootstrap

```bash
make build             # compiles bootstrap/stage0.c → build/gem (first time or after clean)
make bootstrap         # regenerate stage0.c from current compiler sources (verified roundtrip)
make test              # run all numbered examples in examples/ against expected_output.txt
make test-json         # run examples/json_parser.gem
make test-json-suite   # run JSONTestSuite parser conformance (clones to /tmp on first run)
make test-lsp          # smoke-test the `gem lsp` subcommand (canned initialize/didOpen/shutdown session)
make clean             # remove build/ and /tmp/gem_*
```

After changing compiler sources, run `make bootstrap` to update `stage0.c`. The bootstrap target verifies the new stage0 can compile itself (fixed-point check) before replacing it. If codegen output changes, the built-in roundtrip will fail on the first pass — do a manual 3-stage bootstrap (see `RESUME_PROMPT.md` for the exact commands).

## Testing Discipline

After any compiler change, run edge-case and adversarial tests before considering the work done:

1. **Happy path** — basic programs that exercise the new feature.
2. **Edge cases** — empty bodies, zero args, boundary values, nested constructs.
3. **Adversarial inputs** — malformed source, missing tokens, type mismatches at runtime.
4. **Regression** — `make test` to make sure nothing broke.

Add a numbered example under `examples/` (next free slot) and append its stdout to `expected_output.txt`. For programs that exit non-zero (e.g. uncaught `error()` to test stack-trace output), `run_all.sh` tolerates non-zero exits and diffs by output. For expected compile-time errors, just verify by hand — those don't fit the diff harness.

## Adding a New Builtin

1. Implement the C function in the appropriate `runtime/gem_builtins_*.c` file.
2. Add the declaration to `runtime/gem.h`.
3. Add the name → C function mapping in `compiler/codegen.gem` (`builtin_fns` table around line 1043).
4. Update `docs/SPEC.md`.
5. Update both editor extensions (see below).
6. Add a numbered example to `examples/`, append expected output, run `make test`, then `make bootstrap`.

## Adding a Std Module

1. Create `std/<name>.gem`. Use `load` for any std deps. End the file with `export <fn>, <fn>, ...`.
2. Update `docs/SPEC.md` (Standard Library section) and `docs/CHEATSHEET.md`.
3. Add tests as a numbered example (e.g. `78_time_stdlib.gem`) so they run in `make test`.
4. Update editor extensions if the module exposes new identifiers worth highlighting.

## Optimization Tracking

`docs/OPTIMIZATIONS.md` tracks future performance improvements. When you spot an obvious optimization (e.g. a new builtin that copies when it could use views, a hot path that could be specialized), add it there rather than implementing it immediately. Keep the file organized by category. When an optimization ships, move its write-up to `docs/OPTIMIZATIONS_LOG.md` (under the same category heading) — keep the active doc focused on what's still TODO.

## Roadmap Tracking

`docs/ROADMAP.md` tracks future *capabilities* — features the language doesn't have yet (distribution, hot code reload, etc.). When a discussion surfaces a capability worth pursuing later, add it there rather than letting it evaporate. Distinct from OPTIMIZATIONS.md (perf on existing features) and LSP_ROADMAP.md (tooling). Keep entries brief: motivation, what needs building, trade-offs.

## Editor Extension Maintenance

After any language change, update **both** editor extensions.

### VS Code (`editors/vscode/syntaxes/gem.tmLanguage.json`)

- **New keyword** — add it to `keyword.control.gem`, `keyword.other.gem`, or `keyword.declaration.function.gem`.
- **New builtin function** — add it to the alternation in the `builtin` rule.
- **New syntax construct** — add a repository rule and include it in the top-level `patterns` array at the right priority.
- **New type annotation** (extern context) — add it to the alternation in `extern-type-annotation`.

The symlink `~/.vscode/extensions/gem-language` → `editors/vscode` makes changes take effect on VS Code reload.

### Helix (`editors/tree-sitter-gem/`)

- **New keyword** — tree-sitter picks it up automatically if it's a string literal in `grammar.js`; add a highlight query entry in `queries/highlights.scm`.
- **New builtin function** — add it to the `#match?` regex in highlights.scm (both `call_expression` and `call_with_block`).
- **New syntax construct** — add a rule in `grammar.js`, regenerate, add highlight/indent/textobject queries.
- **New type annotation** — add it to the `type` choice in `grammar.js`.

After grammar.js changes: `tree-sitter generate`, `hx --grammar build`, then copy `queries/highlights.scm` to `~/.config/helix/runtime/queries/gem/`. Test with `tree-sitter parse` on all `.gem` files to verify zero errors.

## Key Decisions

- Compilation target is C source code. `cc` handles optimization and linking.
- The compiler is self-hosting: `build/gem` compiles `compiler/main.gem` → C → new binary. `bootstrap/stage0.c` is the escape hatch — checked into git so any C compiler can rebuild from scratch.
- Concurrency uses minicoro (stackful coroutines) with a round-robin scheduler. Each process gets its own arena (bump allocator); values are deep-copied across process boundaries (`spawn`, `send`). Arenas are freed in bulk when a process exits.
- **Per-iteration arena reset**: long-running `while true` (or self-recursive) process loops emit a `gem_arena_reset_with_roots(...)` at the back-edge, after copying live vars into a pin set. Liveness analysis in `compiler/liveness.gem` decides eligibility; if the loop can't be proven safe (closure captures, vars not addressable at the back-edge, `break` in body), the compiler emits a warning and skips the reset. This is what lets server loops run forever without unbounded memory growth.
- Preemptive scheduling via reduction counter at loop back-edges (`GEM_REDUCTION_LIMIT = 4000` in `runtime/gem_scheduler.c`).
- TCP builtins use non-blocking sockets + `poll()` in the scheduler loop. Blocking operations (file I/O, `exec`, `extern blocking fn`) use a 4-worker thread pool.
- Selective receive (`receive ... when ... end`) scans the mailbox and removes the first matching message, leaving others queued.
- Process monitoring, linking, and `trap_exit` follow Erlang semantics. Named processes via `register`/`whereis` with auto-cleanup on death.
- Tail call optimization for self-recursive functions emits `while(1)` loops with parameter reassignment. Mutual tail-call cycles (≥2 fns in an SCC of the tail-edge graph) emit a `gem_fn_<name>_body` + thin trampoline wrapper; intra-SCC tail calls write a global TLB (`gem_tail_fn`, `gem_tail_args`, …) instead of a real C call, so a 7-fn cycle iterates at constant stack depth. Bail-out conditions for the SCC: any member with rest params, defaults, boxed params, name-shadow, or > 16 params. See `compiler/codegen.gem` `find_tail_call_sccs` / `scc_wrapper_for` / `emit_scc_tail_call`.
- `extern fn` / `extern blocking fn` provide C interop with type marshaling. **Note**: `String` parameters marshal as `const char *` and are truncated at the first NUL on the C side — Gem strings are binary-safe internally (slen-tracked) but the C ABI is not. For binary FFI, use the `Bytes` extern type (see SPEC §C Interop): a `Bytes` param expands to `(const uint8_t *data, int64_t len)`, a `Bytes` return uses `GemBytes { data, len }` from `gem.h`. Any Gem string (file contents, `tcp_read`, `build_string`) is a valid `Bytes` argument — no conversion needed.
- **Binary-safe strings**: `VAL_STRING` carries an explicit `slen` (gem.h:80) — `len(s)` returns slen, not strlen, so embedded NULs survive. The trailing `\0` terminator is maintained as an invariant for cheap C interop. When adding a new builtin that returns or accepts a string, set/use `.slen` (not `strlen`). I/O sinks (`tcp_write`, `write_file`, `append_file`) read `slen` to send the full byte range. Codegen emits `gem_string_with_len("...", N)` for string literals; embedded NUL escapes use `\000` (3-digit octal) in the C output to avoid digit absorption.
- No classes, static types, exceptions, or namespaces in v0 — by design. Tables with closures serve as objects, `error()`/`pcall()` for error handling.
- libdill is dead (crashes on arm64 macOS). Don't reach for it.
- Emitted `#line` directives and `gem_push_frame` paths are project-root-relative (codegen.gem `rel_path`, fed `project_root` from `compiler/main.gem`). Keeps `bootstrap/stage0.c` reproducible across machines and avoids leaking developer paths into compiled binaries. Out-of-tree files compiled by absolute path keep their absolute path.
- **Lowering pass** (`compiler/lower.gem`): runs between `resolve_loads` and `fold_constants`. The parser emits structural nodes for `for_in` / `for_range` / `match` / `receive_match` (with `match_arm` + `pat_*` children) verbatim; `lower()` rewrites them into the desugared shapes codegen consumes (block-of-while; `when_clause` with concrete `value`/`bindings`). Gensyms for `_for_*_N` / `_match_N` / `_recv_N` are produced here, not in the parser. Phase ordering matters: any tree-walking pass that runs **before** lower (`tag_source_file`, `rename_node` in compiler/main.gem) needs to know about the structural tags; everything **after** lower (fold, codegen, liveness) only ever sees the desugared shapes. The LSP modules (`lsp/doc.gem`, `lsp/workspace.gem`) call `lower()` before walking so their consumers (symbols / definition / completion) see the post-lower form. Future LSP features that want pattern-aware behavior should expose the pre-lower AST as a separate accessor — don't add a second copy of the post-lower walk.

## Spec Maintenance

`docs/SPEC.md` is the source of truth for the language. After any change to syntax, semantics, or builtins, update it. If the spec disagrees with the code, fix the spec.

## Documentation Maintenance

Treat this file as living documentation: when a claim here turns out stale, a constant has drifted, or a workflow is missing a step, fix it in the same change — a wrong claim is worse than a missing one. SPEC.md describes the language; CLAUDE.md captures how to work in this repo (where things live, what's load-bearing, the workflows for adding/changing things). If a fact is purely about language semantics, it belongs in SPEC.md.

## Language Quick Reference

See [`docs/CHEATSHEET.md`](docs/CHEATSHEET.md) for a one-page summary of syntax, builtins, and std modules.

