# Gem Language

A dynamically typed language that compiles to C with Erlang-style concurrency. See `docs/SPEC.md` for the full spec.

## Design Philosophy

**Gem the language must be simple to write.** The OTP model (supervisors, gen_servers, "let it crash", spawn/send/receive) is already the learnability tax — that's the cognitive ceiling. The language layer must not pile more on top.

When evaluating a runtime or codegen mechanism, ask: *does this leak a concept the user has to track to write correct or performant code?* If yes, push the burden into the compiler (static analysis) or the runtime (uniform mechanism), not the user.

For example:
- Long-running process loops are written naturally with `while true` (or self-recursion). The compiler does liveness analysis and emits per-iteration arena resets at the back-edge so memory does not grow. No `@process_loop` annotation, no rewriting code as recursion.
- No "you must structure your code this way for X to work" runtime constraints. Either auto-detect via static analysis or make the mechanism work uniformly.
- Compiler warnings are OK when they surface a hidden constraint ("this loop won't reset because a live value has no addressable home at its back-edge"). Silent perf cliffs are not.
- Optimizations that introduce DX warts (depth fences, hand-inlined wrappers, manual aliasing tricks) are tolerable only as transient hacks with a structural fix on the roadmap.

## Project Structure

```
compiler/             # self-hosting compiler (lexer, parser, AST, errors, liveness, lower, fold, codegen, loader, main)
lsp/                  # language server: main (entry), rpc (framing+types), server (loop), handlers (lifecycle+textDocument), doc (per-uri process). Dispatched from compiler/main.gem when argv[1] == "lsp"
std/                  # standard library (string, table, math, time, log, http, request, json, url, mime, sqlite, task, supervisor, dynamic_supervisor, gen_server, test)
runtime/              # C runtime — split by category:
  gem.h               #   public API, tagged values, process table, scheduler decls
  gem_core.c          #   value constructors, table internals, equality
  gem_copy.c          #   deep copy, pin-set, region arena reset, module-slot copy at spawn
  gem_arena.c         #   per-process arena allocator
  gem_ops.c           #   arithmetic and comparison operators
  gem_error.c         #   error handling, stack trace printing
  gem_scheduler.c     #   coroutine scheduler, mailbox, process lifecycle, I/O polling, process stacks + overflow handling
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
3. Add the name → C function mapping to the `BUILTIN_FNS` table in `compiler/builtins.gem`.
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
- **New syntax construct** — add a rule in `grammar.js`, regenerate, add highlight queries (plus indent/textobject queries in the local Helix config, below).
- **New type annotation** — add it to the `type` choice in `grammar.js`.

After grammar.js changes: `tree-sitter generate`, `hx --grammar build`, then copy `queries/highlights.scm` to `~/.config/helix/runtime/queries/gem/`. The last two steps assume a local Helix setup that is not in this repository: a `gem` entry in `~/.config/helix/languages.toml`, and `indents.scm`/`textobjects.scm` under `~/.config/helix/runtime/queries/gem/`, which exist only in the maintainer's config. `editors/CLAUDE.md` ("Helix Config") lists what each file holds. Test with `tree-sitter parse` on all `.gem` files to verify zero errors.

## Key Decisions

- Compilation target is C source code. `cc` handles optimization and linking.
- The compiler is self-hosting: `build/gem` compiles `compiler/main.gem` → C → new binary. `bootstrap/stage0.c` is the escape hatch — checked into git so any C compiler can rebuild from scratch.
- Concurrency uses minicoro (stackful coroutines) with a round-robin scheduler. Each process gets its own arena (bump allocator); values are deep-copied across process boundaries (`spawn`, `send`, and the module slots at `spawn`). A spawned process's arena is freed in bulk when it exits; the main process's arena lives until the program ends.
- **Process stacks and stack overflow**: every process, main included, runs on an 8 MB mmap'd stack (lazily paged) with a 64 KB `PROT_NONE` guard below it. The guard sits inside minicoro's padded storage area, so `gem_coro_create` in `runtime/gem_scheduler.c` relies on minicoro's `[mco_coro | context | storage | stack]` layout and checks it at every create. Overflow is caught at two levels. (1) `gem_push_frame` (gem.h) compares the frame address against `gem_stack_limit`, which the scheduler sets on each resume to 256 KB above the stack floor, and raises a pcall-catchable `"stack overflow in <fn>"`. (2) C code that recurses on its own runs into the guard. A SIGSEGV/SIGBUS handler on a sigaltstack redirects the faulting context to a rescue function on a static stack, and that function longjmps to the process's `proc_jmp`, so the process dies (not catchable). Any other fault keeps its default action. Don't go back to malloc'd stacks or call `mco_create` directly. A Gem-level recursion cycle always passes through a non-leaf function (leaf fns skip `gem_push_frame`), which is what makes the soft check sufficient.
- **Per-iteration arena reset (region resets)**: every `while` loop, TCO function and mutual-TCO trampoline takes a `GemArenaMark` when it starts (`gem_arena_mark`) and calls `gem_arena_reset_region(&mark, roots, …)` at its back-edge (`compile_while`, `emit_tco_continue`, `scc_wrapper_for` in codegen.gem; runtime in `runtime/gem_copy.c` "Region reset"). A reset copies what is reachable from the region allocated since the mark into fresh blocks and unmaps the rest; memory older than the mark is never moved or freed. **Soundness invariant:** after a reset no reachable pointer refers into the freed region. Callers' C frames were suspended before the mark, so they only hold older values; the resetting frame's live values are the roots (liveness in `compiler/liveness.gem`; module slots are never roots, the runtime roots them); and every older object that can be made to point into the region is fixed up by the runtime: tables via a write barrier (`gem_table_written` in every runtime path that stores into a table or grows its arrays, feeding a per-arena remembered log sorted by mark clock), buffers (arena `buffer_list`), pinned boxes older than the mark (pin `seq`), module slots, the mailbox. Arena closure envs and capture boxes are write-once — a closure that assigns a capture must share a pinned box with its creator (boxing classifies writes over every local, nested-block lets included). Copies to another process keep that: `spawn`, `send` and `send_after` copy a pinned box (one in the source process's pin-set, or one marked in `gem_malloc_pinned` inside a malloc'd timer copy) as a pinned box of the receiver (`gem_copy_new_box`), so the receiver's resets keep what the copied closures store; a plain arena box there would be freed by the receiver's next reset. **Any new runtime path that stores a value into an existing table must call `gem_table_written`**, and any new kind of mutable arena object must be fixed up in `gem_region_reset_impl`. Hysteresis: the next reset for a mark waits for `max(GEM_ARENA_RESET_THRESHOLD, 2 × copied + scanned)` bytes. If liveness can't root a loop's live set (a live var with no C local at the back-edge, or a non-converging fixpoint), the loop runs without resetting; only `while true` loops warn. `GEM_DIAG=1` prints reset statistics at exit. All value copies (`gem_deep_copy`, `gem_deep_copy_malloc`, `gem_spawn_copy`, the reset's copy) and `gem_deep_free` are iterative (`gem_copy_shallow` + worklist in gem_copy.c, inline fill only up to `GEM_COPY_INLINE_DEPTH`), so the depth of user data never reaches the C stack; keep any new walk over values iterative too.
- **Module globals are per-process**: top-level bindings (`top_level_vars`) compile to slots `gem_g_<name>` = `gem_cur_globals[i]` (`#define`d in the emitted C); `is_global_ref` in codegen.gem decides slot vs local (flow-sensitive via `fn_scope_locals`). Each process owns a malloc'd slot array (`GemProcess.globals`); the scheduler points `gem_cur_globals` at it on resume; `gem_spawn_fn` deep-copies the parent's slots together with the closure env (`gem_spawn_copy`, one copy map). Closures never capture module bindings. Frozen module namespace tables are immortal malloc copies (`gem_table_freeze_static`), shared by every process. `note_spawn_global_writes` prints a `note:` for writes to module state in code reachable from a spawn body.
- Preemptive scheduling via reduction counter at loop back-edges (`GEM_REDUCTION_LIMIT = 4000` in `runtime/gem_scheduler.c`).
- TCP builtins use non-blocking sockets + `poll()` in the scheduler loop. `read_file`, `write_file`, `append_file`, `exec`, `sqlite_open`, `sqlite_close` and `extern blocking fn` calls run on a 4-worker thread pool; the other filesystem and sqlite builtins run inline.
- A `GemIORequest` (runtime/gem.h) is shared by the worker and the requesting process, and each side calls `gem_io_release` exactly once; the last release frees it. A process can be killed mid-request, so a builtin that submits to the pool copies its result out, then releases, and never frees the request or its args by hand. File and `exec` requests (`gem_io_submit`) keep their inputs and outputs in request fields that the last release frees. Other work goes through `gem_io_submit_extern(fn, args, free_args)`, where `free_args` frees anything in `args` the requester hasn't taken (see the sqlite builtins, and `_blk_<name>_free` in codegen for `extern blocking fn`).
- Selective receive (`receive ... when ... end`) scans the mailbox and removes the first matching message, leaving others queued.
- Process monitoring, linking, and `trap_exit` follow Erlang semantics. Named processes via `register`/`whereis` with auto-cleanup on death.
- Tail call optimization for self-recursive functions emits `while(1)` loops with parameter reassignment. Functions with default or rest params (destructured params lower to a defaulted param + body prelude) instead store the tail-call args in `_tco_argv` and `goto _tco_rebind`, a block at the loop bottom that reruns the entry param bindings, so omitted args evaluate their defaults and the rest table is fresh, exactly as for a real call. Captured-and-mutated params get a fresh box per iteration. Mutual tail-call cycles (≥2 fns in an SCC of the tail-edge graph) emit a `gem_fn_<name>_body` + thin trampoline wrapper; intra-SCC tail calls write a global TLB (`gem_tail_fn`, `gem_tail_args`, …) instead of a real C call, so the stomp broker's 6-fn `writer_loop ↔ handle_frame ↔ handle_<command>` cycle iterates at constant stack depth. Bail-out conditions for the SCC: any member with rest params, defaults, boxed params, name-shadow, or > 16 params. See `compiler/codegen.gem` `find_tail_call_sccs` / `scc_wrapper_for` / `emit_scc_tail_call`.
- `extern fn` / `extern blocking fn` provide C interop with type marshaling. **Note**: `String` parameters marshal as `const char *` and are truncated at the first NUL on the C side — Gem strings are binary-safe internally (slen-tracked) but the C ABI is not. For binary FFI, use the `Bytes` extern type (see SPEC §C Interop): a `Bytes` param expands to `(const uint8_t *data, int64_t len)`, a `Bytes` return uses `GemBytes { data, len }` from `gem.h`. Any Gem string (file contents, `tcp_read`, `build_string`) is a valid `Bytes` argument — no conversion needed.
- **Binary-safe strings**: `VAL_STRING` carries an explicit `slen` (the `GemVal` string member in gem.h) — `len(s)` returns slen, not strlen, so embedded NULs survive. The trailing `\0` terminator is maintained as an invariant for cheap C interop. When adding a new builtin that returns or accepts a string, set/use `.slen` (not `strlen`). I/O sinks (`tcp_write`, `write_file`, `append_file`) read `slen` to send the full byte range. Codegen emits `gem_string_with_len("...", N)` for string literals; embedded NUL escapes use `\000` (3-digit octal) in the C output to avoid digit absorption.
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

