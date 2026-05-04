# LSP Roadmap

A minimal language server for Gem, focused on the features that provide the most value for the least complexity. Dynamic typing puts a ceiling on static analysis, so the strategy is: handle the common patterns well, don't try to be omniscient.

## Phase 0: Pre-LSP groundwork

Land these before any LSP work — they're useful standalone and the LSP needs them anyway.

### Diagnostic location coverage

`compiler/main.gem`'s `--check` flag (already shipped) plus `efm-langserver` is enough to wire up red squiggles in any LSP-only editor (Helix, Neovim) without writing a real LSP. But two gaps make placement worse than it could be:

1. **Cross-file errors point at the wrong file.** When a typo is in a loaded module (e.g. `std/foo.gem`), the AST node carries the line within that file but the codegen-emitted diagnostic uses the top-level entry file's name. Fix: tag each AST node with its source file in `resolve_loads` (`compiler/main.gem:290`), and use it instead of `source_name` in error formatters. ~30 lines.

2. **Codegen errors lack caret context.** Parser errors render Rust-style with source line + `^^^` underline (via `compile_error()` in `compiler/errors.gem`). Codegen errors (undeclared identifier, etc.) print only `path:line` because `make_codegen()` doesn't carry source text. Fix: thread source text into the codegen closure, route the two `eprint("\n[Compile Error]:")` sites in `compiler/codegen.gem` (`:2768` and `:3410`) through `compile_error()`. ~50 lines including the source-text plumbing.

These two together give every diagnostic the same shape and accuracy regardless of which compiler stage produced it.

### Multi-error recovery

Today the compiler bails at the first `error()` — 32 sites across lexer/parser/codegen, each terminal. Single-error is tolerable for `--check` in CI/save-hooks, but in an LSP loop it's the difference between "fix the file" and "fix one thing, save, fix the next thing, save, …".

The work: parser sync points (statement boundaries, `end` keyword), AST stub nodes for partial parses, and threading an error list through lexer/parser/codegen instead of calling `error()` directly. Realistically 200–400 lines and a careful pass — not a quick fix. Defer until the single-error UX bites.

## Phase 1: Foundations (~2 days)

### LSP Scaffold

Set up the LSP binary with JSON-RPC over stdio. Handle lifecycle (`initialize`, `shutdown`, `exit`) and document sync (`textDocument/didOpen`, `textDocument/didChange`, `textDocument/didClose`).

**Implementation language:** TBD. Options:
- **Gem** — dogfooding, but no LSP library support, would need to implement JSON-RPC from scratch.
- **TypeScript** — `vscode-languageserver` library handles all protocol plumbing. Fastest to ship.
- **Rust** — `tower-lsp` crate. More work than TS, but single binary, no runtime dependency.
- **C** — closest to the existing codebase, but most boilerplate.

Recommend TypeScript for v1 (ship fast, iterate), consider Rust rewrite if performance matters later.

**Document store:** keep full text of open files in memory. Re-parse on every change (debounced). Gem files are small — full reparse is fine for v1.

**Effort:** ~150 lines (with vscode-languageserver).

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

## Not in Scope (v2+)

- **Rename symbol** — needs reliable reference finding across files first.
- **Type inference** — beyond simple table field tracking. Would need flow analysis.
- **Signature help** — showing parameter hints as you type function arguments.
- **Formatting** — auto-formatter for Gem source. Separate tool, not LSP-specific.
- **Code actions** — auto-import, extract function, etc.
- **Semantic tokens** — LSP-driven syntax highlighting (tree-sitter handles this already).

## Summary

| Feature | LSP Method | Effort | Phase |
|---|---|---|---|
| LSP scaffold + document sync | lifecycle, didOpen/didChange | ~150 lines | 1 |
| Symbol table | (internal) | ~200 lines | 1 |
| Go to definition | textDocument/definition | ~100 lines | 2 |
| Completion (table fields, identifiers, builtins) | textDocument/completion | ~200 lines | 2 |
| Diagnostics | textDocument/publishDiagnostics | ~50-100 lines | 2 |
| Hover | textDocument/hover | ~100 lines | 3 |
| Document symbols | textDocument/documentSymbol | ~50 lines | 3 |
| Find references | textDocument/references | ~80 lines | 3 |
| **Total** | | **~950-1000 lines** | |

Phases 1-2 (the useful core) are ~700 lines and ~5 days. Phase 3 is polish.
