# LSP Roadmap

A minimal language server for Gem, focused on the features that provide the most value for the least complexity. Dynamic typing puts a ceiling on static analysis, so the strategy is: handle the common patterns well, don't try to be omniscient. Why it is written in Gem and ships as a subcommand: `docs/archive/lsp_v1_decisions.md`.

## Current state

`gem lsp` (dispatched from `compiler/main.gem` when `argv[1] == "lsp"`) speaks JSON-RPC over stdio. The code is in `lsp/`: `main` (entry), `rpc` (framing and LSP shapes, UTF-16 columns), `server` (main loop), `handlers` (dispatch), `doc` (one registered process per open file, reparsed on every change), `workspace` (a cache of files the editor hasn't opened, for cross-file lookups), `symbols`, `definition`, `completion`, `diagnostics` and `position`.

It handles:

- **Lifecycle and sync:** `initialize`, `initialized`, `shutdown`, `exit`, `textDocument/didOpen`, `didChange`, `didClose`.
- **Diagnostics** (`textDocument/publishDiagnostics` after every open and change): the lexer and parser errors of the file, and a `load` of a missing module or a directory.
- **Go to definition** (`textDocument/definition`): functions and variables in the file, and across `load`s.
- **Completion** (`textDocument/completion`): table fields after `t.`, identifiers in scope, builtins (`BUILTIN_FNS` in `compiler/builtins.gem`).
- **`gem/debug/symbols`**, only with `GEM_LSP_DEBUG` set: the symbol table, for the smoke tests.

`make test-lsp` runs the smoke tests in `tests/lsp/`.

## Current limits

- **Diagnostics are lexer, parser and `load` errors only.** The LSP runs the lexer, parser and `lower`, not codegen, so codegen errors, warnings and `note:`s don't reach the editor, and every diagnostic has severity 1 (error). It does not open the modules a file loads.
- **A named function's parameter has its function's line** (column 0), and an anonymous function's has no position: the parser keeps parameters as bare names.
- **URIs are turned into paths by dropping `file://`**, with no percent-decoding, so a path with spaces or non-ASCII characters doesn't resolve.
- **The workspace cache is never refreshed.** A module parsed for a cross-file lookup stays as first parsed until the server restarts; a file the editor has open uses its doc process instead.
- **Definition and completion stop at dynamic values:** `t.method()` where `t` could be anything, and values returned from functions.

### Synchronous main loop instead of spawn-per-request

`rpc.read_message` reads stdin with `input()` and `read_stdin(n)`, which block the OS thread, so `lsp/server.gem` handles each message before reading the next, and a request can't be cancelled while it runs. Doc processes still run between messages (a handler's `send`/`receive` to one yields), and `doc.open` waits for the new doc process's `doc_ready` (up to 5 s) so a request right after `didOpen` finds it registered. Cancellable requests need stdin reads that yield to the scheduler (through the I/O thread pool, as `read_file` does) and a worker process per request that `$/cancelRequest` can kill.

## Formatting (not started)

Format-on-save waits for comment preservation: the lexer drops `#` comments, and string literals lose their quote style, so an AST-driven formatter would delete comments. The parser already keeps `for` and `match` structural (`lower` desugars them afterwards), so a formatter can render them as written. The work:

- **Comment attachment in the lexer/parser** (~200–400 LOC). Lex `#` comments as their own token type; the parser attaches preceding-line comments as `leading_comments` on the next node and trailing comments as `trailing_comment` on the current node. The lowering pass propagates them through structural rewrites.
- **Quote-style preservation** (~50 LOC). Tag string tokens with their quote style (`"dq"` / `"sq"` / `"tdq"` / `"tsq"`) and thread it through to `make_string`. Interpolated strings already keep their `parts`.
- **Formatter library** (`compiler/format.gem`, ~600 LOC). AST-driven, consuming structural nodes (post-parser, pre-lower). Style: 2-space indent, trailing comma on multi-line, `if` / `match` / `receive` arms one per line, fn bodies on their own lines, ~80 columns before a collection goes multi-line.
- **CLI hook** (`gem fmt <file>` and `--check`, ~50 LOC), dispatched like `gem lsp`.
- **LSP integration** (~50 LOC): `textDocument/formatting` returns one full-document `TextEdit`.
- **Test corpus and idempotence harness** (`examples/format/` + `tests/check_format.sh`, ~50 LOC): every fixture must satisfy `format(format(x)) == format(x)`.

A source-preserving formatter (CST or token-level rewriting, ~1000–1500 LOC on top of the above) is worth building only if AST-driven comment attachment misplaces comments in ways users report.

## Later (sketched, not rejected)

Land once a real workload demands them.

- **Hover** (`textDocument/hover`, ~100 lines). Functions: parameter names, plus the `##` doc comment directly above the definition (and a module's `##` header on its namespace name); strip the `## ` prefix and render lines indented past the text as code. Builtins: short doc string from a static table. Variables: inferred "type" if known (`table`, `function`, `string`).
- **Document symbols** (`textDocument/documentSymbol`, ~50 lines). Top-level functions and variables for the outline; `gem/debug/symbols` already builds the same data.
- **Find references** (`textDocument/references`, ~80 lines). The inverse of goto-def over the symbol table.
- **Rename symbol**, once references work across files.
- **Signature help**: parameter hints while typing arguments.
- **Codegen diagnostics**: a `--check`-like pass that runs codegen without writing C, so its errors, warnings and notes reach the editor.
- **Type inference** beyond table field tracking (needs flow analysis).
- **Code actions**: auto-import, extract function.
- **Semantic tokens** (tree-sitter already highlights).
