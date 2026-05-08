# LSP Roadmap

A minimal language server for Gem, focused on the features that provide the most value for the least complexity. Dynamic typing puts a ceiling on static analysis, so the strategy is: handle the common patterns well, don't try to be omniscient.

## Status & Plan (locked 2026-05-07)

**Decisions:**

- **Implementation language: Gem.** The roadmap originally leaned TypeScript (fastest scaffold via `vscode-languageserver`), but the AST-reuse argument flipped the call: every feature past diagnostics needs the parsed AST, and `load "compiler/parser"` from a Gem LSP is materially cheaper than reimplementing the parser in TS (drift forever) or shelling out `gem --dump-ast --json` per request (sluggish completion). The actor model also fits LSP semantics — document state as a registered process per file, request workers as spawned coroutines, cancellation as `exit(worker, kill)`. Cost: ~300–400 lines of JSON-RPC framing + LSP type table-shapes that we'd get for free in the npm ecosystem. Worth it.
- **Binary shape: `gem lsp` subcommand.** Single artifact, single bootstrap surface, parser reuse is literally the same compiled code. Dispatched from `compiler/main.gem`. Avoids "two binaries that must agree on parser semantics" forever.
- **Repo layout:** `lsp/` at repo root, sibling to `compiler/`, `runtime/`, `std/`. Modules: `lsp/main.gem` (entrypoint), `lsp/{rpc,server,doc,symbols,handlers}.gem`. The compiler imports nothing from `lsp/`; the LSP imports from `compiler/`.
- **Format-on-save: in scope** for v1, with a minimal AST-driven pretty-printer (no source-preservation; comments reattached heuristically, some may move). Lifted out of "Not in Scope" below. Source-preserving formatter (CST or token-level) deferred to v2.
- **Phase 0 first.** Multi-error recovery is the single biggest UX cliff between "useful LSP" and "annoying LSP" — without it every save shows one error at a time. Land before any LSP work.

**PR plan:**

| PR | Branch | Scope | Status | Lines |
|---|---|---|---|---|
| #6 | `lsp/plan` | Lock LSP implementation plan + Phase 0b design | ✓ Done (2026-05-07, commit `21b9623`) | — |
| #7 | `lsp/phase-0a-codegen-caret` | Phase 0a — route codegen errors through `compile_error()` for caret context | ✓ Done (2026-05-07) | ~50 |
| #8 | `lsp/phase-0b-multi-error` | Phase 0b — multi-error recovery in lexer/parser/codegen | ✓ Done (2026-05-07) | ~430 |
| #9 | `lsp/phase-1a-scaffold` | Phase 1a — `gem lsp` subcommand, JSON-RPC framing, lifecycle, didOpen/didChange | ✓ Done (2026-05-07) | ~400 |
| #10 | `lsp/phase-1b-symbols` | Phase 1b — symbol table pass, per-doc AST cache | ✓ Done (2026-05-07) | ~250 |
| #11 | `lsp/phase-2a-features` | Phase 2a — goto-def + completion (table fields, identifiers, builtins) | Pending | ~300 |
| #12 | `lsp/phase-2b-diagnostics` | Phase 2b — diagnostics integration (consumes Phase 0b error list) | Pending | ~100 |
| #13 | `lsp/phase-3-format` | Phase 3 — minimal AST formatter + format-on-save | Pending | ~600 |

Each PR ships independently. After merge, mark `✓ Done (date, commit-sha)` here and append any deviations from the plan in the relevant phase section below. Per-PR planning docs (e.g. `LSP_PHASE_0B_PLAN.md`) live alongside this file during implementation, get deleted once the PR lands (the work is in the code; the plan doc becomes noise).

**Out of scope for v1** (deferred, not rejected): hover, document symbols, find references, rename, signature help, semantic tokens, source-preserving formatter. Land once core is real and a real workload demands them.

## Phase 0: Pre-LSP groundwork

Land these before any LSP work — they're useful standalone and the LSP needs them anyway.

### Diagnostic location coverage

`compiler/main.gem`'s `--check` flag (already shipped) plus `efm-langserver` is enough to wire up red squiggles in any LSP-only editor (Helix, Neovim) without writing a real LSP. But two gaps make placement worse than it could be:

1. ~~**Cross-file errors point at the wrong file.**~~ ✓ Done (2026-05-05). `tag_source_file` (`compiler/main.gem`) walks the parsed AST after `parse_source` and stamps `node.file = <full_path>` on every AST node — both the entry file and each loaded module. `make_codegen` exposes `file_of(node)` / `loc_of(node)` helpers (defined up front so they're capturable by every nested closure that reports a diagnostic) which prefer `node.file` over the entry-file `source_name` fallback. All `[Compile Error]`, `[Compiler Bug]`, and `cannot reset` / TCO / spawn-target warnings now point at the originating file, so a typo in `std/foo.gem` surfaces as `std/foo.gem:42` instead of `main.gem:1`. Verified: a synthetic `load`-then-undeclared-identifier program now reports `--> /path/to/loaded.gem:N`. Runtime stack-trace plumbing (`#line` directives, `gem_push_frame` calls, `gem_error_at_fn` / `gem_check_callable` file args) also routes through `file_of(node)`, so an `error()` raised inside a loaded module shows the module's path in the trace rather than the entry file's. The synthetic top-level `gem_user_main` frame keeps the entry-file `source_name` since it has no AST node of its own.

2. ~~**Codegen errors lack caret context.**~~ ✓ Done (2026-05-07). `make_codegen` now takes a `sources_by_file` map (`{path → source text}`) populated in `compiler/main.gem` for the entry file and by `resolve_loads` for every loaded module. The four user-facing `[Compile Error]` sites in `compiler/codegen.gem` (cannot-assign-to-function, undeclared-identifier in assign target, undeclared-identifier in var ref, module-has-no-export) now route through `compile_error()` in `compiler/errors.gem`, producing Rust-style output with source line + `^^^` caret. The `[Compiler Bug]` sentinels and unknown-op paths stay terminal — they're unreachable on a valid parse and exit with the bug-template message intentionally. Carets render correctly for errors in loaded files because `source_of(node)` looks up by `file_of(node)`.

These two together give every diagnostic the same shape and accuracy regardless of which compiler stage produced it.

### Multi-error recovery

✓ Done (2026-05-07, PR #8). The compiler now accumulates every diagnostic in one pass via a closure-captured error sink (`make_error_sink` in `compiler/errors.gem`) threaded through lexer, parser, codegen, and `resolve_loads`. The lexer recovers from unexpected characters and missing newlines after `"""`/`'''`; unterminated-string variants emit one error and stop scanning. The parser's `expect()` reports without consuming and returns a synthetic token, with a per-loop `force_progress` watchdog at every `while peek().type != X` site (table/array/interp/destructure) and a top-level guard in `parse()`. Garbage in expression or pattern position returns `error_expr` / a never-matching pattern stub; codegen's four user-facing sites (assign-to-fn, undeclared identifier in assign, undeclared identifier in var ref, missing module export) report-and-continue with `GEM_NIL` placeholders. The driver gates each phase on `sink.has_any()` so codegen output is byte-identical for valid programs (bootstrap roundtrip verified). Tooling: `tests/broken/*.gem` corpus (8 files) plus `tests/check_broken.sh` (min/max error-count bounds, catches both regression and cascade) and `tests/check_corpus.sh` (`--check` over compiler+std+examples), both wired into `make test`. Sink dedupes by `file:line:col:msg` and caps at 20 errors with a "too many errors; further diagnostics suppressed" trailer.

Deferred from the plan to a follow-up PR (per §3.3 Option B): no codegen-side AST tolerance for `error_expr`/`error_stmt` — they never reach codegen because main.gem skips it once the parser has reported. Adding tolerance would let codegen surface semantic errors on the same pass as parse errors; not worth the audit cost yet.

## Phase 1: Foundations (~2 days)

### LSP Scaffold

Set up the `gem lsp` subcommand with JSON-RPC over stdio. Handle lifecycle (`initialize`, `shutdown`, `exit`) and document sync (`textDocument/didOpen`, `textDocument/didChange`, `textDocument/didClose`).

**Implementation language: Gem** (locked — see Status & Plan above). Modules:

- `lsp/rpc.gem` — Content-Length framing, JSON encode/decode (uses `std/json`), request/response/notification dispatch.
- `lsp/server.gem` — main loop, request routing, lifecycle handlers.
- `lsp/doc.gem` — per-document state (text + cached AST), incremental edit application.
- `lsp/handlers.gem` — request handlers (`textDocument/definition`, `completion`, `publishDiagnostics`, …).

**Process model.** Main loop reads stdin frames, dispatches each request as a spawned worker process. Per-document state lives in registered processes (one per open file) — workers query them via `send`/`receive`. Cancellation = `exit(worker, kill)`. Mailboxes serialize edits naturally.

**Document store:** keep full text of open files in memory. Re-parse on every change (debounced ~200ms). Gem files are small — full reparse is fine for v1.

**LSP type plumbing:** define table-shape constructors (`lsp_position(line, char)`, `lsp_range(start, end)`, `lsp_diagnostic(...)`, etc.) in `lsp/rpc.gem` rather than freeform tables, to keep handler call sites self-documenting. UTF-16-vs-UTF-8 column conversion happens at the rpc boundary, not in handlers.

**Effort:** ~400 lines (PR #9).

**Deviations from the locked plan (filled in 2026-05-07):**

- **Synchronous main loop instead of spawn-per-request.** `rpc.read_message` calls `input()` and `read_stdin(N)`, both of which block the OS thread (they don't yield via the I/O thread pool). Spawning a worker per request wouldn't actually run concurrently with the next stdin read in v1, so `lsp/server.gem` dispatches each frame synchronously. Per-document `register`ed processes still get scheduled cooperatively between frames (any handler that does `send`/`receive` to a doc process yields). Cancellation-grade concurrency (cancel mid-completion via `kill(worker, …)`) is the upgrade trigger — when a request handler grows large enough that we want it cancellable, route stdin reads through a yielding builtin and switch to spawn-per-request. Tracked as the first item to revisit in PR #11/#12.
- **New runtime builtins** for binary-safe stdio: `read_stdin(n) -> String` (exactly N bytes via `fread`) and `write_stdout(s)` (raw bytes, no trailing `\n`, `fflush` after each call so frames hit the wire immediately). Documented in SPEC §C / Builtins. Both are blocking; promoting them to thread-pool yielding is the same upgrade as the bullet above.
- **`load "../lsp/main"` from `compiler/main.gem`** is the dispatch wire-up. The LSP code becomes part of the compiled `gem` binary (and `bootstrap/stage0.c`), gated by `if argv()[1] == "lsp"` before `parse_args`. Compiling a normal user program reads zero LSP code — LSP modules aren't reachable from any user `load`.
- **Smoke test** at `tests/lsp/smoke.sh` (wired into `make test` via the `test-lsp` target). Pipes a canned `initialize → didOpen → didChange → didClose → shutdown → exit` session and asserts the framed responses (capabilities echo, two `publishDiagnostics` notifications, `id:2 result:null` for shutdown). `publishDiagnostics` is stubbed to `[]`; PR #12 wires it to the Phase 0b error sink.

### Symbol Table

On each parse, walk the AST and build a symbol table:
- **Functions** — name, location (file + line + column), parameters.
- **Variables** — name, location, scope (which function or top-level).
- **Imports** — `load "std/foo"` and `load "path"` → resolve to file path.

This is the core data structure everything else queries.

**Scope rules:** Gem has function scope + top-level scope. No block scope (if/for don't create scopes). Closures capture outer variables. Track scope nesting to resolve which definition a name refers to.

**Effort:** ~200 lines.

**Deviations from the locked plan (filled in 2026-05-07, PR #10):**

- **Resolver extracted to `compiler/loader.gem`.** `resolve_load_path`, `find_project_root`, and `compute_stdlib_root` moved out of `compiler/main.gem` into a shared module. Both the compiler driver and `lsp/symbols.gem` import from it, so the LSP doesn't fork the resolver.
- **Per-param positions not yet plumbed.** The parser stores params as bare strings, so every param entry inherits the enclosing fn's `line` (col=0). Acceptable for v1; PR #11 plumbs real per-param spans if goto-def precision needs them.
- **`anon_fn` has no `line` stamp.** The parser doesn't set a line on anon_fn nodes themselves. The let-binding (`let foo = fn() ... end`) carries the line; raw inline anons get `line: nil`. Same fix path as the param spans above.
- **URI→path is `substr(uri, 7, …)` (no percent-decoding).** First workspace path with spaces or unicode will trip; revisit when it actually shows up.
- **Synchronous `doc.open` handshake.** The LSP main loop blocks on stdin and never yields, so a request following `didOpen` on the same input frame would race the spawned doc process. `open()` now sends a `doc_ready` reply from inside the spawn and the caller waits for it (5s timeout). This vanishes once `read_stdin` routes through the I/O thread pool and the dispatcher spawns workers per request (PR #11/#12 trigger).
- **`gem/debug/symbols` debug method.** Internal-only LSP method gated on `$GEM_LSP_DEBUG`; emits `{functions, vars, imports}` for the smoke test. PR #11 surfaces the same data through real `textDocument/documentSymbol` (lifted from "Not in Scope" to v2 once a real workload demands it).
- **Desugar gensyms filtered.** The parser introduces `_pdestr<N>`, `_d<N>`, `_for_<role>_<N>` for destructuring + `for` desugar. `is_gensym(name)` in `lsp/symbols.gem` keeps these out of the symbol table so completion/outline never surfaces them. Real user names starting with `_` (e.g. `_data`) survive — only the digit-suffixed gensym pattern is filtered.
- **Debounce stays deferred.** Parse + symbol-build on the largest std module (`std/http.gem`, 574 lines, 13.4KB) measured at 9ms avg locally — well under the 50ms threshold. Mailbox serialization at the doc process makes back-to-back `didChange`s natural already. Reopen the debounce question when this number grows.
- **`walk_node` TCO warning.** Mirrors `compiler/main.gem`'s pre-existing `rename_node` warning — bounded recursion over an AST, so arena pressure is bounded inside one call and the doc-loop back-edge resets between edits. No action needed for correctness; tracked under `docs/OPTIMIZATIONS.md` "Tighten 'TCO function not reachable from process root' warning" as a P2 — the warning is a false positive on bounded structural recursion and would be worth tightening if the count of bounded walkers grows.
- **CI surfaced two latent bugs not caught on macOS:**
  1. **Top-level mutable `let` written from a spawned process.** `lsp/doc.gem` originally lazily filled `_install_root_cache` from inside `analyze()` (which runs in the spawned doc process). The string `dirname(dirname(argv()[0]))` allocated in the spawn's arena, but the BSS-backed top-level box outlives that arena — once the doc-loop back-edge reset fired, the box dangled. macOS malloc tolerated the UAF; Linux glibc tripped `realloc(): invalid next size`. Fixed by computing the path at module load time so it lives in the main arena (never freed). General lesson: **top-level boxed lets must only be written from main**, since their box is shared globally but the value's arena follows whoever wrote last.
  2. **`GEM_CORO_STACK_SIZE = 16 KB` was too small to run the lexer + parser inside a spawned coroutine.** Every spawn before PR #10 was an HTTP handler / supervisor / gen_server doing small per-message work; the LSP is the first place a tree-walking parser ran inside a spawn. The 16 KB stack overflowed in `lexer.tokenize`, with the same Linux-vs-macOS asymmetry: macOS happened to map adjacent memory writable, Linux glibc detected the corruption. Bumped to 256 KB; bookmark soak peaks gain ~24 MB at c=100 / ~120 MB at c=500, well within the existing memory envelope. mmap'd lazy-paged stacks tracked as a P2 in `OPTIMIZATIONS.md`. General lesson: **assume any new spawn target may want compiler-grade stack**, and revisit the constant if a workload pushes harder.

## Phase 2: Core Features (~3 days)

### Go to Definition (`textDocument/definition`)

Given a cursor position, find the symbol under the cursor, look it up in the symbol table, return its definition location.

**What works:**
- Functions defined in the same file.
- Variables defined in the same file.
- Cross-file: follow `load` statements, parse the loaded file, resolve the symbol there.
- Builtins — jump to the entry in `compiler/codegen.gem` or `runtime/gem_builtins_*.c` (stretch goal).

**What doesn't (v1):**
- Dynamic dispatch (`t.method()` where `t` could be anything).
- Values returned from functions.

**Effort:** ~100 lines.

### Completion (`textDocument/completion`)

Three kinds of completion, in priority order:

**1. Table field completion (`t.` triggers).**
When the user types `t.`, find the definition of `t` and infer its fields:
- **Table literal:** `let t = {foo: 1, bar: fn() ... end}` → offer `foo`, `bar`.
- **Direct assignment:** `t.baz = 123` anywhere in the same scope → add `baz`.
- **Module tables:** `load "std/sqlite"` → parse `std/sqlite.gem`, find the returned table's fields. Most std modules end with a table literal export — easy to detect.

This covers the tables-as-objects pattern which is the primary Gem idiom.

**2. Identifiers in scope.**
Complete variable and function names visible at the cursor position. Includes: local variables, function parameters, top-level definitions, imported names.

**3. Builtin functions.**
Complete from the known builtin list (same list as `compiler/codegen.gem`'s `builtin_fns` table). Static data, no analysis needed.

**Effort:** ~200 lines.

### Diagnostics (`textDocument/publishDiagnostics`)

Run the Gem compiler in a check mode (or parse-only mode) on save, report errors as diagnostics. This gives red squiggles for syntax errors.

**Options:**
- Add a `--check` flag to the compiler that parses + analyzes but doesn't emit C. Cleanest.
- Shell out to `build/gem` and parse stderr. Quick and dirty but works.

**Effort:** ~50 lines (shelling out) or ~100 lines (check mode in compiler).

## Phase 3: Quality of Life (~2 days)

### Hover (`textDocument/hover`)

Show type/signature info on hover:
- **Functions:** show parameter names and count.
- **Builtins:** show a short doc string (maintain a static table of builtin docs).
- **Variables:** show inferred "type" if known (e.g., `table`, `function`, `string`).

**Effort:** ~100 lines.

### Document Symbols (`textDocument/documentSymbol`)

List all top-level functions and variable definitions for the outline view / breadcrumb bar.

**Effort:** ~50 lines.

### Find References (`textDocument/references`)

Given a symbol, find all locations where it's used. Inverse of go-to-definition — scan the symbol table for all occurrences of the name in the same scope chain.

**Effort:** ~80 lines.

## Phase 3: Format-on-save (PR #13)

Minimal AST-driven pretty-printer + LSP `textDocument/formatting` integration. Scope:

- **Formatter as a library** (`lsp/format.gem` or `compiler/format.gem`) consuming the resolved AST and emitting canonical source. Drives off node shapes (no token stream), so comments are not source-preserving — they get reattached heuristically to the nearest following node, and some edge-case placements may shift. Acceptable trade for v1; revisit if it bites.
- **Style:** 2-space indent, trailing comma on multi-line collections, `if` / `match` / `receive` arms one per line, function bodies on their own lines. Pin a small set of canonical examples in `examples/format/` so behavior changes are diffable.
- **CLI hook:** `gem fmt <file>` reads source, parses, formats, writes back. Same artifact as `gem lsp`. Lets users format outside the LSP and gives the LSP an obvious place to call into.
- **LSP integration:** handler calls the formatter on `textDocument/formatting`, returns a single `TextEdit` replacing the whole document range. Cheap, correct, and editor-agnostic.

Source-preserving formatter (CST or token-level rewriting, ~1000–1500 lines) deferred to v2 — only worth building if comment placement turns out to bite real users.

## Not in Scope (v2+)

- **Rename symbol** — needs reliable reference finding across files first.
- **Type inference** — beyond simple table field tracking. Would need flow analysis.
- **Signature help** — showing parameter hints as you type function arguments.
- **Source-preserving formatter** — see Phase 3 above; v1 is AST-driven.
- **Code actions** — auto-import, extract function, etc.
- **Semantic tokens** — LSP-driven syntax highlighting (tree-sitter handles this already).

## Summary

| Feature | LSP Method | Effort | PR |
|---|---|---|---|
| Codegen errors → caret context | (internal) | ~50 lines | #7 |
| Multi-error recovery | (internal) | ~300–400 lines | #8 |
| LSP scaffold + document sync | lifecycle, didOpen/didChange | ~400 lines | #9 |
| Symbol table | (internal) | ~250 lines | #10 |
| Go to definition + Completion | textDocument/definition, /completion | ~300 lines | #11 |
| Diagnostics | textDocument/publishDiagnostics | ~100 lines | #12 |
| Format-on-save | textDocument/formatting + `gem fmt` | ~600 lines | #13 |
| **Total** | | **~2000 lines** | |

Hover, document symbols, and find references deferred to v2 (see *Not in Scope*).
