# LSP: implementation decisions (archive)

The reasoning behind the shape of `gem lsp`, settled before the first
version was written (May 2026). The current state and plan are in
`docs/LSP_ROADMAP.md`.

- **Written in Gem, not TypeScript.** TypeScript and
  `vscode-languageserver` would have been the fastest scaffold, but every
  feature past diagnostics needs the parsed AST. Loading the compiler's
  own lexer and parser from a Gem server is cheaper than reimplementing
  the parser in TypeScript (two parsers that drift apart) or shelling
  out to the compiler for an AST dump on every request (too slow for
  completion). The cost was writing the JSON-RPC framing and the LSP
  table shapes by hand (`lsp/rpc.gem`).
- **A subcommand of the compiler binary, not a second binary.**
  `compiler/main.gem` loads `lsp/main.gem` and dispatches on
  `argv[1] == "lsp"`, so the server runs the same compiled parser as the
  compiler, there is one artifact to build and bootstrap, and no two
  binaries have to agree on parser semantics. Apart from that one
  `load` in `compiler/main.gem`, the compiler loads nothing from `lsp/`;
  the LSP loads from `compiler/`.
- **Multi-error recovery before any LSP work.** Without it the editor
  would show one error per save. The compiler collects diagnostics in an
  error sink (`make_error_sink` in `compiler/errors.gem`) that the lexer,
  parser and codegen report into and keep going.
- **Format-on-save waits for comment preservation**, since an AST-driven
  formatter that drops comments is a feature people turn on once and
  then off. The parser was made structural for `for` and `match` (with
  `compiler/lower.gem` desugaring afterwards) so a formatter can render
  them as written.
