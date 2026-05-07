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
| #6 | `lsp/phase-0a-codegen-caret` | Phase 0a — route codegen errors through `compile_error()` for caret context | Pending | ~50 |
| #7 | `lsp/phase-0b-multi-error` | Phase 0b — multi-error recovery in lexer/parser/codegen | Pending (planning in `docs/LSP_PHASE_0B_PLAN.md`) | ~300–400 |
| #8 | `lsp/phase-1a-scaffold` | Phase 1a — `gem lsp` subcommand, JSON-RPC framing, lifecycle, didOpen/didChange | Pending | ~400 |
| #9 | `lsp/phase-1b-symbols` | Phase 1b — symbol table pass, per-doc AST cache | Pending | ~250 |
| #10 | `lsp/phase-2a-features` | Phase 2a — goto-def + completion (table fields, identifiers, builtins) | Pending | ~300 |
| #11 | `lsp/phase-2b-diagnostics` | Phase 2b — diagnostics integration (consumes Phase 0b error list) | Pending | ~100 |
| #12 | `lsp/phase-3-format` | Phase 3 — minimal AST formatter + format-on-save | Pending | ~600 |

Each PR ships independently. After merge, mark `✓ Done (date, commit-sha)` here and append any deviations from the plan in the relevant phase section below. Per-PR planning docs (e.g. `LSP_PHASE_0B_PLAN.md`) live alongside this file during implementation, get deleted once the PR lands (the work is in the code; the plan doc becomes noise).

**Out of scope for v1** (deferred, not rejected): hover, document symbols, find references, rename, signature help, semantic tokens, source-preserving formatter. Land once core is real and a real workload demands them.

## Phase 0: Pre-LSP groundwork

Land these before any LSP work — they're useful standalone and the LSP needs them anyway.

### Diagnostic location coverage

`compiler/main.gem`'s `--check` flag (already shipped) plus `efm-langserver` is enough to wire up red squiggles in any LSP-only editor (Helix, Neovim) without writing a real LSP. But two gaps make placement worse than it could be:

1. ~~**Cross-file errors point at the wrong file.**~~ ✓ Done (2026-05-05). `tag_source_file` (`compiler/main.gem`) walks the parsed AST after `parse_source` and stamps `node.file = <full_path>` on every AST node — both the entry file and each loaded module. `make_codegen` exposes `file_of(node)` / `loc_of(node)` helpers (defined up front so they're capturable by every nested closure that reports a diagnostic) which prefer `node.file` over the entry-file `source_name` fallback. All `[Compile Error]`, `[Compiler Bug]`, and `cannot reset` / TCO / spawn-target warnings now point at the originating file, so a typo in `std/foo.gem` surfaces as `std/foo.gem:42` instead of `main.gem:1`. Verified: a synthetic `load`-then-undeclared-identifier program now reports `--> /path/to/loaded.gem:N`. Runtime stack-trace plumbing (`#line` directives, `gem_push_frame` calls, `gem_error_at_fn` / `gem_check_callable` file args) also routes through `file_of(node)`, so an `error()` raised inside a loaded module shows the module's path in the trace rather than the entry file's. The synthetic top-level `gem_user_main` frame keeps the entry-file `source_name` since it has no AST node of its own.

2. **Codegen errors lack caret context.** Parser errors render Rust-style with source line + `^^^` underline (via `compile_error()` in `compiler/errors.gem`). Codegen errors (undeclared identifier, etc.) print only `path:line` because `make_codegen()` doesn't carry source text. Fix: thread source text into the codegen closure, route the two `eprint("\n[Compile Error]:")` sites in `compiler/codegen.gem` (`:2768` and `:3410`) through `compile_error()`. ~50 lines including the source-text plumbing.

These two together give every diagnostic the same shape and accuracy regardless of which compiler stage produced it.

### Multi-error recovery

Today the compiler bails at the first `error()` — 32 sites across lexer/parser/codegen, each terminal. Single-error is tolerable for `--check` in CI/save-hooks, but in an LSP loop it's the difference between "fix the file" and "fix one thing, save, fix the next thing, save, …".

The work: parser sync points (statement boundaries, `end` keyword), AST stub nodes for partial parses, and threading an error list through lexer/parser/codegen instead of calling `error()` directly. Realistically 300–400 lines and a careful pass — not a quick fix. Lands as PR #7 (per the locked plan above); detailed design in `docs/LSP_PHASE_0B_PLAN.md` while the work is in flight.

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

**Effort:** ~400 lines (PR #8).

### Symbol Table

On each parse, walk the AST and build a symbol table:
- **Functions** — name, location (file + line + column), parameters.
- **Variables** — name, location, scope (which function or top-level).
- **Imports** — `load "std/foo"` and `load "path"` → resolve to file path.

This is the core data structure everything else queries.

**Scope rules:** Gem has function scope + top-level scope. No block scope (if/for don't create scopes). Closures capture outer variables. Track scope nesting to resolve which definition a name refers to.

**Effort:** ~200 lines.

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

## Phase 3: Format-on-save (PR #12)

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
| Codegen errors → caret context | (internal) | ~50 lines | #6 |
| Multi-error recovery | (internal) | ~300–400 lines | #7 |
| LSP scaffold + document sync | lifecycle, didOpen/didChange | ~400 lines | #8 |
| Symbol table | (internal) | ~250 lines | #9 |
| Go to definition + Completion | textDocument/definition, /completion | ~300 lines | #10 |
| Diagnostics | textDocument/publishDiagnostics | ~100 lines | #11 |
| Format-on-save | textDocument/formatting + `gem fmt` | ~600 lines | #12 |
| **Total** | | **~2000 lines** | |

Hover, document symbols, and find references deferred to v2 (see *Not in Scope*).
