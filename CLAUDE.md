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
                      #   codegen.gem emits C; its analyses live beside it: scope (shadowing lets, module namespaces),
                      #   captures (free vars, boxing, non-escaping closures), tco (self/mutual tail calls), callgraph,
                      #   loops (arm-let hoist, reset tagging), append (in-place string append), cgutil (shared helpers)
lsp/                  # language server: main (entry), rpc (framing+types), server (loop), handlers (lifecycle+textDocument), doc (per-uri process), workspace (cross-file AST cache), diagnostics, definition, completion, symbols, position (cursor helpers). Dispatched from compiler/main.gem when argv[1] == "lsp"
std/                  # standard library (string, table, math, time, log, http, request, json, url, mime, sqlite, task, supervisor, dynamic_supervisor, gen_server, test)
runtime/              # C runtime — split by category:
  gem.h               #   public API, tagged values, process table, scheduler decls
  gem_core.c          #   value constructors, table internals, equality
  gem_copy.c          #   deep copy, pin-set, region arena reset, module slots + lazy snapshot units at spawn
  gem_arena.c         #   per-process arena allocator
  gem_ops.c           #   arithmetic and comparison operators
  gem_error.c         #   error handling, stack trace printing
  gem_scheduler.c     #   process table + run state, coroutine scheduler, mailbox, process lifecycle, I/O polling, process stacks + overflow handling
  gem_threadpool.c    #   worker thread pool for blocking I/O (4 workers)
  gem_resource.c      #   resource table: sockets and sqlite handles as owned resources, claim, close at a process's exit
  gem_builtins_core.c #   print, error, len, type, conversions, pcall, argv, etc.
  gem_builtins_collection.c  # push, pop, keys, values, sort, insert, delete
  gem_builtins_string.c      # find, str_replace, substr, chr/ord, buf_* API; gem_bytes_span (std/http's and std/request's extern helper)
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
examples/             # numbered tests (01–224) + run_all.sh; plus larger programs that test themselves (tests/check_example_apps.sh):
                      #   json_parser.gem, tcp_echo.gem, bookmark_app/ (HTMX + sqlite web app), stomp_broker/ (OTP-style STOMP broker),
                      #   logstat/ (CLI access-log analyzer, its own gem.toml project; the text-processing perf yardstick),
                      #   mini_redis/ (Redis-protocol server; the yardstick for long-lived state, connections and fan-out),
                      #   lox/ (tree-walking Lox interpreter + bench/*.lox; the yardstick for calls, closures, recursion, objects),
                      #   gemgrep/ (recursive grep on libc's regcomp/regexec via extern fn + a header of static helpers; the yardstick for C interop),
                      #   jobqueue/ (job queue on a dynamic_supervisor worker pool under seeded fault injection; the yardstick for failure under load)
docs/SPEC.md          # language spec (source of truth for all language decisions)
docs/BEST_PRACTICES.md    # how to write Gem: idioms, traps, std conventions (new code follows it)
docs/OPTIMIZATIONS.md     # tracked future performance improvements
docs/OPTIMIZATIONS_LOG.md # shipped optimizations + work logs + benchmark anchors
docs/ROADMAP.md           # future capabilities (features, not perf)
docs/KNOWN_BUGS.md        # bugs found and not yet fixed, each with a repro
docs/LSP_ROADMAP.md       # Gem LSP: current state, limits, plan
docs/archive/             # measurements and design notes of earlier designs, kept for reference (not current state)
editors/vscode/       # VS Code extension (TextMate grammar)
editors/tree-sitter-gem/  # tree-sitter grammar for Helix (+ queries)
benchmarks/           # wrk harness for examples/bookmark_app (run.sh) + node_baseline reference impl
                      #   measure_all.sh         — runs every harness below into baselines/<date>_<machine>/ (to commit); summarize.py writes
                      #                            its summary.csv/.md and compares two baselines (--compare); measure.py times the batch runs
                      #   stomp/                 — Python STOMP load harness for examples/stomp_broker
                      #   logstat/               — examples/logstat vs the same program in Python, with an output diff
                      #   mini_redis/            — examples/mini_redis vs redis-server (redis-benchmark, pub/sub fan-out, a reply diff)
                      #   lox/                   — examples/lox vs the same interpreter in Python on bench/*.lox, with an output diff
                      #   gemgrep/               — examples/gemgrep vs GNU grep -E and a Python twin on a generated tree, with output diffs
                      #   jobqueue/              — examples/jobqueue vs Python asyncio and Elixir/OTP twins on six fault scenarios; checks their invariants
                      #   soak/                  — not in measure_all.sh: the example servers under an hour of checked load: flat memory, fds, throughput, CPU, latency? (milestone reports in results/)
```

## Build, Test, Bootstrap

```bash
make build             # compiles bootstrap/stage0.c → build/gem (first time or after clean)
make bootstrap         # regenerate stage0.c from current compiler sources (verified roundtrip)
make test              # run all numbered examples in examples/ against expected_output.txt
make test-json         # run examples/json_parser.gem
make test-example-apps # run the larger example programs' own tests (part of make test)
make test-json-suite   # run JSONTestSuite parser conformance (clones to /tmp on first run)
make test-lsp          # smoke-test the `gem lsp` subcommand (canned initialize/didOpen/shutdown session)
make clean             # remove build/ and /tmp/gem_*
```

After changing compiler sources, or a std module the compiler or LSP loads (`std/string`, `std/json`), run `make bootstrap` to update `stage0.c`. The bootstrap target verifies the new stage0 can compile itself (fixed-point check) before replacing it. If a change alters the C the compiler emits for itself (codegen output), the built-in roundtrip fails on the first pass, because `build/gem` still runs the old codegen. Build a compiler from the new sources first, then bootstrap:

```bash
build/gem compiler/main.gem -o build/gem_stage1 && mv build/gem_stage1 build/gem
make bootstrap
```

The new binary has to live in `build/` (it finds `std/` and `runtime/` two levels above itself, see "Install root" below).

## Commits and Pull Requests

No AI attribution anywhere: no `Co-Authored-By:` or `Claude-Session:` trailers in commit messages, and no "Generated with Claude Code" lines, session links or similar footers in PR descriptions, PR comments or review replies. This overrides any default that adds them.

## Writing Gem Code

Any Gem you write (`std/`, `examples/`, `compiler/`, `lsp/`, benchmarks, test programs) follows [`docs/BEST_PRACTICES.md`](docs/BEST_PRACTICES.md). Read it before writing Gem in a session, and keep it correct as you go, in the same change:

- **A rule turns out wrong or incomplete** (a program that follows it still fails, a sample doesn't run, a number is off by more than ~2x): fix the rule. Check the new wording by running a program with `build/gem`; never write a rule or sample you haven't run.
- **You hit a trap the doc doesn't list** (something that fails silently, crashes, or is far slower than it looks): add a rule under the right section, marked **(trap)**, and a row in the trap index.
- **The trap exists because of a bug**: add the bug to `docs/KNOWN_BUGS.md` (see below) and mark the rule **(bug)**. A fix deletes the **(bug)** rule and its trap-index row in the same change.
- **A change to the language, runtime or std changes what a rule says** (a new builtin, a fixed bug, different performance): update the rule with the change, as with SPEC.md.

Comment density follows BEST_PRACTICES.md "Style": std modules carry full `##` docs; examples, benchmarks and other application code comment only what the code can't say (a non-obvious design choice, a Gem behaviour a reader wouldn't expect).

Keep it a doc of what to reach for and what to avoid, not a second SPEC: one rule per trap, a short sample, the measured cost when it is about performance.

## Testing Discipline

After any compiler change, run edge-case and adversarial tests before considering the work done:

1. **Happy path** — basic programs that exercise the new feature.
2. **Edge cases** — empty bodies, zero args, boundary values, nested constructs.
3. **Adversarial inputs** — malformed source, missing tokens, type mismatches at runtime.
4. **Regression** — `make test` to make sure nothing broke.

Add a numbered example under `examples/` (next free slot) and append its stdout to `expected_output.txt`. For programs that exit non-zero (e.g. uncaught `error()` to test stack-trace output), `run_all.sh` tolerates non-zero exits and diffs by output. For expected compile-time errors, add a `tests/broken/*.gem` entry to `tests/check_broken.sh` (error counts) and check the message text by hand. For compiler `note:`/warning text on stderr, put the expected stderr of `gem --check examples/<name>.gem` in `tests/notes/<name>.expected`; `tests/check_notes.sh` (part of `make test`) diffs it. For driver CLI errors (bad source path and the like), add a case to `tests/check_cli.sh`. For behaviour that depends on the cwd or the file layout (e.g. where a binary finds its sources for trace context), add a script under `tests/` and wire it into the Makefile `test` target, like `tests/check_project_root.sh` or `tests/check_trace_source.sh`. The larger programs in `examples/` (`json_parser.gem`, `tcp_echo.gem`, `bookmark_app/`, `stomp_broker/`, `logstat/`, `mini_redis/`, `lox/`, `gemgrep/`, `jobqueue/`) check themselves instead (`std/test` suites in each directory's `test.gem`; logstat's built program is also diffed against `logstat/expected_report.txt`, lox's tests run its bench programs against `lox/bench/*.expected`, gemgrep's built program runs on stdin and a small tree for its output, messages and exit statuses, and jobqueue's on a small seeded run whose invariants must hold); `tests/check_example_apps.sh` runs them, so a change to one of them updates its tests too.

## Adding a New Builtin

1. Implement the C function in the appropriate `runtime/gem_builtins_*.c` file.
2. Add the declaration to `runtime/gem.h`.
3. Add the name → C function mapping to the `BUILTIN_FNS` table in `compiler/builtins.gem`, and the name to `LEAF_BUILTINS` there if the builtin runs no Gem code (no callback, no closure argument it calls) and never yields to the scheduler: a function whose body's and param defaults' only calls are to such builtins, with no `receive` block, pushes no frame.
4. Update `docs/SPEC.md`.
5. Update both editor extensions (see below).
6. Add a numbered example to `examples/`, append expected output, run `make test`, then `make bootstrap`.

## Adding a Std Module

1. Create `std/<name>.gem`. Use `load` for any std deps. End the file with `export <fn>, <fn>, ...`. Give the module a `##` header and every exported fn a `##` doc comment (BEST_PRACTICES.md, "Document the public API with `##`").
2. Update `docs/SPEC.md` (Standard Library section) and `docs/CHEATSHEET.md`.
3. Add tests as a numbered example (e.g. `78_time_stdlib.gem`) so they run in `make test`.
4. Update editor extensions if the module exposes new identifiers worth highlighting.

## Optimization Tracking

`docs/OPTIMIZATIONS.md` tracks future performance improvements. When you spot an obvious optimization (e.g. a new builtin that copies when it could use views, a hot path that could be specialized), add it there rather than implementing it immediately. Keep the file organized by category. When an optimization ships, move its write-up to `docs/OPTIMIZATIONS_LOG.md` (under the same category heading) — keep the active doc focused on what's still TODO.

## Roadmap Tracking

`docs/ROADMAP.md` tracks future *capabilities* — features the language doesn't have yet (distribution, hot code reload, etc.). When a discussion surfaces a capability worth pursuing later, add it there rather than letting it evaporate. Distinct from OPTIMIZATIONS.md (perf on existing features) and LSP_ROADMAP.md (tooling). Keep entries brief: motivation, what needs building, trade-offs.

## Known Bugs Tracking

`docs/KNOWN_BUGS.md` tracks bugs that were found and not fixed. When you find a bug outside the scope of the current change, add it there instead of fixing it on the side or leaving it in a PR description: a minimal repro checked against `build/gem`, what goes wrong, and where the code is. A fix deletes its entry in the same change, along with any **(bug)** rule in `docs/BEST_PRACTICES.md` that exists because of it. Before working on an entry, re-run its repro; if it no longer reproduces, delete the entry. Performance problems go in OPTIMIZATIONS.md and missing features in ROADMAP.md, not here.

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
- **New syntax construct** — add a rule in `grammar.js`, regenerate, add highlight queries (plus indent/textobject queries in the local Helix config, `editors/CLAUDE.md` "Helix Config").
- **New type annotation** — add it to the `type` choice in `grammar.js`.

After a `grammar.js` change, regenerate, rebuild and check that every `.gem` file parses as `editors/CLAUDE.md` describes ("Toolchain", "Testing Changes").

## Key Decisions

- Compilation target is C source code. `cc` handles optimization and linking.
- The compiler is self-hosting: `build/gem` compiles `compiler/main.gem` → C → new binary. `bootstrap/stage0.c` is the escape hatch — checked into git so any C compiler can rebuild from scratch.
- Concurrency uses minicoro (stackful coroutines) with a round-robin scheduler. Each process gets its own arena (bump allocator); values are deep-copied across process boundaries (`spawn`, `send`, and the module slots at `spawn`, lazily). A spawned process's arena is freed in bulk when it exits; the main process's arena lives until the program ends.
- **Process stacks and stack overflow** (overview: `runtime/gem_scheduler.c`, "Process stacks"): every process, main included, runs on an 8 MB mmap'd stack with a 64 KB `PROT_NONE` guard below it. `gem_push_frame` (gem.h) raises a pcall-catchable `"stack overflow in <fn>"` once a frame is within 256 KB of the stack floor; C code that recurses on its own into the guard kills the process through a SIGSEGV/SIGBUS handler (not catchable). Rules: don't go back to malloc'd stacks or call `mco_create` directly (`gem_coro_create` relies on minicoro's block layout and checks it at every create). Leaf fns skip `gem_push_frame`, so the soft check holds only because every Gem-level recursion cycle passes through a non-leaf fn; keep it that way.
- **Per-iteration arena reset (region resets)** (overview: `runtime/gem_copy.c`, "Region reset"; `GemArenaMark`, `GemArenaPoint` and "Return points" in gem.h): every `while` loop, TCO fn and mutual-TCO trampoline takes a mark where it starts and resets at its back-edge (`compile_while`, `emit_tco_continue`, `scc_wrapper_for` in codegen.gem): it copies what is reachable from the memory allocated since the mark and frees the rest, and memory older than the mark is never moved or freed. A fn that can recurse (`mark_recursive_fns`, compiler/callgraph.gem) also resets at its returns, with the return value the only root. Liveness (compiler/liveness.gem) supplies the roots; a loop whose live set it can't root runs without resetting, and only `while true` loops warn. Reset statistics are always counted (`gem_reset_stats_get`, read by the `runtime_stats()` builtin in gem_scheduler.c); `GEM_DIAG=1` prints them at exit; `GEM_DIAG=2` also prints a line per reset of 1 ms or more, with the process and the function it ran in. Rules that keep it sound:
  - **Any new runtime path that stores a value into an existing table (or grows its arrays) must call `gem_table_written`**, the write barrier that lets a reset find older tables pointing into its region; any new kind of mutable arena object must be fixed up in `gem_region_reset_impl`.
  - Arena closure envs and capture boxes are write-once: a closure that assigns a capture shares a pinned box with its creator (boxing, compiler/captures.gem), and a copy to another process makes a pinned box a pinned box of the receiver (`gem_copy_new_box`).
  - Every return of a fn body with a return point goes through `return_str` / `return_nil_str` in codegen.gem.
  - Every copy, free and reset walk over values is iterative (`gem_copy_shallow` + worklist in gem_copy.c), so the depth of user data never reaches the C stack; keep new walks iterative (printing recurses, capped at `GEM_FMT_MAX_DEPTH`).
  - Every copy and free walks a closure env as `[intptr_t n][GemVal *box × n]`, so a closure the runtime builds itself (e.g. `build_string`'s `add`) must use that layout too.
- **Module globals are per-process** (overview: gem.h and `runtime/gem_copy.c`, "Module globals"): top-level bindings compile to per-process slots (`gem_g_<name>` reads through `gem_global_get(i)`, writes are `gem_global_set(i, v)`, read-modify-write lvalues `(*gem_global_ref(i))`; `is_global_ref` in codegen.gem decides slot vs local). `spawn` copies them lazily, per slot, through refcounted snapshot units the parent reuses until it changes them. Rules:
  - Never emit a raw `gem_cur_globals[i]` access: the read must fault in lazy slots and the write must drop the slot's snapshot unit.
  - **Every runtime path that changes a table's entries must call `gem_table_check_mutable`** (in addition to `gem_table_written`); a new kind of mutable value reachable from module state must be stamped or make its unit single-use (`saw_mutable`, as buffers and pinned boxes do).
  - Closures never capture module bindings. Module namespace tables (`let m = {…}` from `resolve_loads`) are frozen, and `m.x` compiles straight to the module's slot (`collect_module_namespaces`, compiler/scope.gem).
- **Analyses are keyed by name**: codegen and liveness key their analyses by name, so `scope_shadowing_lets` (compiler/scope.gem, "Shadowing lets") first renames every `let` that shadows a visible name to a fresh `__sh<N>_<name>` (a module-scope `let` rebinds its slot instead) and enforces strict block scope; add any new name-keyed analysis after it. Module loading (`transform_module`, compiler/main.gem) prefixes a module's top-level names with `__mod_<mangle>_` (`module_mangle`) and renames the entry file's bindings named like a builtin to `__main_<name>` (`shadow_entry_builtins`); messages and frame names map them back (`build_global_display`, codegen's `display_name`) and never show the mangle. Every name the compiler makes (these renames, the parser's and lower's temporaries, `__anon_<N>` fn literals) starts with `__`, a prefix `check_reserved_names` (compiler/lexer.gem, run by `parse_source`) rejects in source, so it can't meet a user's name; a new generated name must start with `__` too.
- Preemptive scheduling via reduction counter at loop back-edges (`GEM_REDUCTION_LIMIT = 4000` in `runtime/gem_scheduler.c`).
- **Process table and run state** (overview: `runtime/gem_scheduler.c`, "Process table" and "Run state"): `gem_proc_table` is a `PROT_NONE` reservation for `GEM_MAX_PROCS` (262,144) slots that never moves; pid = slot + gen × `GEM_MAX_PROCS`, and the `GEM_MAX_PROCS` env var lowers the limit (tests/check_proc_limit.sh). A scheduler pass runs READY processes in slot order, which the recorded interleavings in `expected_output.txt` depend on. Rules:
  - Check a slot index against `gem_proc_hwm` (not `GEM_MAX_PROCS`) before indexing.
  - **Every change of `GemProcess.state` goes through `gem_proc_set_state`**; a direct `proc->state = …` desynchronizes the scheduler.
  - `spawn` refuses before Linux's `vm.max_map_count` runs out, counting the runtime's own mappings in `gem_runtime_maps`: a new kind of long-lived runtime `mmap` must count itself there too.
- **Owned resources** (overview: gem.h "Owned resources", `runtime/gem_resource.c`; SPEC "Owned Resources"): sockets and sqlite handles are `VAL_RESOURCE` values naming an entry of one resource table by a never-reused serial, so a closed resource stays closed in every copy. Each entry has an owner pid; `gem_res_proc_exit` (called from `gem_free_proc_slot`) closes what a process claimed, and on an abnormal exit (`crashed` flag or an `exit_reason` other than `"normal"`) what it opened, and disowns the rest. Rules:
  - Every runtime path that closes a resource goes through the table (`gem_res_close`, or `gem_res_take` for a kind that closes itself, like `sqlite_close` through the pool); never `close()` a socket's fd directly.
  - A tcp builtin resolves its socket with `gem_res_get` and again with `gem_res_lookup` after every `gem_io_yield`, and never touches the fd once the entry is gone: the number may already name a new file.
  - A socket an `extern blocking fn` got as a `Socket` param is busy until the call returns (`gem_res_busy_begin`/`_end`, called by the codegen'd wrapper, or by the scheduler for a killed caller's parked request): closing it then only retires it (`dead`), and its fd closes when the last busy call ends.
  - A new kind (an `exec` child, a TLS stream) adds a `GEM_RES_*` kind with its names in `gem_res_kinds` and its close in `gem_res_close_n`, and gets `claim`, the exit close and the diagnostics for free.
- TCP builtins use non-blocking sockets + `poll()` in the scheduler loop. `read_file`, `write_file`, `append_file`, `exec`, `sqlite_open`, `sqlite_close` and `extern blocking fn` calls run on a 4-worker thread pool; the other filesystem and sqlite builtins run inline.
- A `GemIORequest` (runtime/gem.h) is shared by the worker and the requesting process, and each side calls `gem_io_release` exactly once; the last release frees it. A process can be killed mid-request, so a builtin that submits to the pool copies its result out, then releases, and never frees the request or its args by hand. File and `exec` requests (`gem_io_submit`) keep their inputs and outputs in request fields that the last release frees. Other work goes through `gem_io_submit_extern(fn, args, free_args)`, where `free_args` frees anything in `args` the requester hasn't taken (see the sqlite builtins, and `_blk_<name>_free` in codegen for `extern blocking fn`).
- Selective receive (`receive ... when ... end`) scans the mailbox and removes the first matching message, leaving others queued.
- Process monitoring, linking, and `trap_exit` follow Erlang semantics. Named processes via `register`/`whereis` with auto-cleanup on death.
- Tail call optimization for self-recursive functions emits `while(1)` loops with parameter reassignment. Functions with default or rest params (destructured params lower to a defaulted param + body prelude) instead store the tail-call args in `_tco_argv` and `goto _tco_rebind`, a block at the loop bottom that reruns the entry param bindings, so omitted args evaluate their defaults and the rest table is fresh, exactly as for a real call. Captured-and-mutated params get a fresh box per iteration. Mutual tail-call cycles (≥2 fns in an SCC of the tail-edge graph) emit a `gem_sccbody_<name>` + thin trampoline wrapper; intra-SCC tail calls write a global TLB (`gem_tail_fn`, `gem_tail_args`, …) instead of a real C call (the TLB is shared by every process: the wrapper must copy the args into its own frame and clear `gem_tail_fn` right after the body returns, before its back-edge reset and yield check; a call site passing > 16 args stays a plain call), so a process loop written as a `loop ↔ handle_frame ↔ handle_<command>` cycle iterates at constant stack depth (examples/96_mutual_tco.gem). Bail-out conditions for the SCC: any member with rest params, defaults, boxed params, name-shadow, or > 16 params. See `find_tail_call_sccs` in `compiler/tco.gem` (detection) and `scc_wrapper_for` / `emit_scc_tail_call` in `compiler/codegen.gem` (emission).
- `extern fn` / `extern blocking fn` provide C interop with type marshaling. **Note**: `String` parameters marshal as `const char *` and are truncated at the first NUL on the C side — Gem strings are binary-safe internally (slen-tracked) but the C ABI is not. For binary FFI, use the `Bytes` extern type (see SPEC §C Interop): a `Bytes` param expands to `(const uint8_t *data, int64_t len)`, a `Bytes` return uses `GemBytes { data, len }` from `gem.h`. Any Gem string (file contents, `tcp_read`, `build_string`) is a valid `Bytes` argument — no conversion needed. An extern's Gem binding is renamed like a top-level fn's (`__mod_<mod>_<name>` in a loaded module, `__main_<name>` when it shadows a builtin in the entry file); `rename_node` (compiler/main.gem) keeps the declared name in `c_name`, which is the C symbol codegen calls and declares.
- **Binary-safe strings**: `VAL_STRING` carries an explicit `slen` (the `GemVal` string member in gem.h) — `len(s)` returns slen, not strlen, so embedded NULs survive. The trailing `\0` terminator is maintained as an invariant for cheap C interop. When adding a new builtin that returns or accepts a string, set/use `.slen` (not `strlen`). I/O sinks (`tcp_write`, `write_file`, `append_file`) read `slen` to send the full byte range. Codegen emits `GEM_STR_LIT("...", N)` (gem.h) for string literals and literal keys: a value whose `sval` points at the C string constant, built without an allocation or copy (copies to another process duplicate it as usual), which is sound only while no runtime path writes a string's bytes in place or frees a string it did not copy. Embedded NUL escapes use `\000` (3-digit octal) in the C output to avoid digit absorption.
- No classes, static types, exceptions, or namespaces in v0 — by design. Tables with closures serve as objects, `error()`/`pcall()` for error handling.
- libdill is dead (crashes on arm64 macOS). Don't reach for it.
- **Install root**: the compiler and the LSP find `std/`, `runtime/` and `build/libgem_runtime.a` two levels above the binary's real path (`find_install_root` in compiler/loader.gem). The real path comes from `gem_exe_path()` in runtime/gem_builtins_io.c (`/proc/self/exe` on Linux, `_NSGetExecutablePath` on macOS, else `argv[0]` via `PATH`, then `realpath`), which loader.gem calls through `extern fn gem_exe_path() -> String`, not a builtin, so a compiler built by an older stage0 can compile it. `tests/check_exe_path.sh` (part of `make test`) runs the compiler through symlinks, `PATH` and relative paths.
- Every path shown to a user (emitted `#line` directives, `gem_push_frame` records, compile errors, notes, load cycles) goes through one function, `make_show_path` in compiler/loader.gem (codegen's `rel_path`, `print_all_errors(sink, show_path)`): project-root-relative with a gem.toml, else under the entry file's directory as typed. Keeps `bootstrap/stage0.c` reproducible across machines and avoids leaking developer paths into compiled binaries. Files outside the root keep their path. Error records in the sink keep the full path (the LSP maps them to URIs). Frame names likewise come from codegen's `frame_name` (`anonymous fn`, `<mod>.<name>`, unprefixed `__main_`/`__sh<N>_` renames); the runtime prints and snapshots them verbatim.
- **Lowering pass** (`compiler/lower.gem`): runs between `resolve_loads` and `fold_constants`. The parser emits structural nodes for `for_in` / `for_range` / `match` / `receive_match` (with `match_arm` + `pat_*` children) verbatim; `lower()` rewrites them into the desugared shapes codegen consumes (block-of-while; `when_clause` with concrete `value`/`bindings`), and moves an `if` / `match` / `receive` used as a value out of its expression into statements that run first (before the statement holding it; inside the `if` on the left operand for the right operand of `and` / `or`; at the top of the loop body for a `while` condition), with each branch's value stored in a `__val_N` temporary at the branch's end (`return <block>` returns from each branch instead); operands evaluated before it go into `__val_N` temporaries too, unless `stays_in_place` shows the moved code can't change them (or the operand is a module namespace callee `m.f` whose binding nothing assigns), so evaluation stays left to right. Gensyms for `__for_*_N` / `__match_N` / `__recv_N` / `__val_N` are produced here, not in the parser. The builtin calls lower synthesizes use the internal aliases `__type` / `__has_key` / `__table_key_at` / `__for_len` / `__for_items` (builtins.gem; `__for_items` is the one-variable `for`'s array check), so a user binding named `len` can't capture them. Phase ordering matters: any tree-walking pass that runs **before** lower (`tag_source_file`, `rename_node` in compiler/main.gem) needs to know about the structural tags; everything **after** lower (fold, codegen, liveness) only ever sees the desugared shapes. The LSP modules (`lsp/doc.gem`, `lsp/workspace.gem`) call `lower()` before walking so their consumers (symbols / definition / completion) see the post-lower form. Future LSP features that want pattern-aware behavior should expose the pre-lower AST as a separate accessor — don't add a second copy of the post-lower walk.

## Spec Maintenance

`docs/SPEC.md` is the source of truth for the language. After any change to syntax, semantics, or builtins, update it. If the spec disagrees with the code, fix the spec.

## Documentation Maintenance

Treat this file as living documentation: when a claim here turns out stale, a constant has drifted, or a workflow is missing a step, fix it in the same change — a wrong claim is worse than a missing one. SPEC.md describes the language; CLAUDE.md captures how to work in this repo (where things live, what's load-bearing, the workflows for adding/changing things). If a fact is purely about language semantics, it belongs in SPEC.md.

## Language Quick Reference

See [`docs/CHEATSHEET.md`](docs/CHEATSHEET.md) for a one-page summary of syntax, builtins, and std modules, and [`docs/BEST_PRACTICES.md`](docs/BEST_PRACTICES.md) for how to write Gem (idioms and traps). See "Writing Gem Code" above for how to keep it current.

