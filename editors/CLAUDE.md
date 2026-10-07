# Editor Extensions

## Tree-sitter Grammar (`tree-sitter-gem/`)

### Toolchain

- `tree-sitter-cli` is installed via npm (`npm install -g tree-sitter-cli`); the repo's `.mise.toml` pins Node.
- After editing `grammar.js`: run `tree-sitter generate`, then `hx --grammar build`.
- Helix (with a `gem` entry in your `languages.toml`, see "Helix Config" below) reads queries from `~/.config/helix/runtime/queries/gem/` — copy updated query files there after changes (`cp queries/highlights.scm ~/.config/helix/runtime/queries/gem/`).

### Grammar Design Decisions

- **`pattern` is a visible rule** (not `_pattern`) so it can appear in `conflicts: $ => [...]`. The parser uses GLR (`prec.dynamic`) to disambiguate `pattern` vs `array`/`table_pair` in `when` clauses.
- **External scanner (`src/scanner.c`) for line-sensitive tokens.** Gem's parser ends an expression at a newline or `;`, while the grammar skips whitespace. So a call's `(`, a subscript's `[`, a binary `-` and a block `{` after a call's `)` are external tokens the scanner produces only when nothing but spaces separates them from the expression they continue; otherwise the internal lexer reads a plain `(`/`[`/unary `-`/table `{` that starts the next statement (`x` then `[a, b]` on the next line is two statements, as in compiler/parser.gem). They are aliased to `"("`, `"["`, `"-"`, `"{"`, so queries match them like the plain tokens. A `load`'s import list `(names)` uses the call `(` and its `as` comes from the scanner (`_load_as`, aliased to `"as"`), so both must be on the `load` line, as in the compiler; `as` is not a keyword, so `as = 2` on the next line is an assignment. The scanner also lexes the `pcall` of `pcall <expr>` (`pcall` not followed by `(` or `do`) and the text of a `"""` string. `_error_sentinel` keeps it out of error recovery. Inside the scanner, skip whitespace only before the token's first character: a skip after it moves the token start.
- **Brace blocks on calls**: `f(x) { |a| ... }` and `f { |a| ... }` use `param_brace_block` (the `{|` token); `f(x) { expr }` uses `brace_block`, whose `{` the scanner gives only when it is not a table literal (`{}` or `{name:`), as compiler/parser.gem decides.
- **`;`** is an extra: it separates statements like a newline.
- **`call_with_block`** handles a block after a callee with no parens (`f do ... end`, `obj.f do ... end`, `f { |a| ... }`) as a separate rule from `call_expression`.
- **`break_statement` and `continue_statement`** are single-keyword named nodes. In highlight queries, use `(break_statement) @keyword` not `"break" @keyword` — there are no anonymous children to match.
- **`"""` strings** interpolate like `"` strings (`string_content` from the scanner, `escape_sequence`, `interpolation`), so an interpolation can hold a `"""` string of its own. `'''` strings are opaque tokens.
- **`_string`** is the choice node for all 4 string types — use it anywhere a string is needed (load, table keys, etc.).

### Testing Changes

After any grammar.js change, verify with:

```bash
tree-sitter generate
for f in $(find examples std compiler lsp tests benchmarks editors -name '*.gem' -not -path 'tests/broken/*'); do (cd editors/tree-sitter-gem && tree-sitter parse "../../$f" 2>&1) | grep -q 'ERROR\|MISSING' && echo "FAIL: $f"; done
```

Every `.gem` file outside `tests/broken/` (which are deliberately invalid) should parse with no ERROR or MISSING nodes. The generated `src/` files are checked in; commit them with `grammar.js`. If a change introduces parse errors, fix them before committing.

### Helix Config

Helix config lives at `~/.config/helix/`, a local setup that is not in this repository (the `gem` entry, `indents.scm` and `textobjects.scm` exist only in the maintainer's config):
- `languages.toml` — gem language definition + grammar source path
- `runtime/queries/gem/highlights.scm` — highlight queries
- `runtime/queries/gem/indents.scm` — indent/outdent rules
- `runtime/queries/gem/textobjects.scm` — function/class/parameter/comment textobjects

## VS Code Extension (`vscode/`)

TextMate grammar at `syntaxes/gem.tmLanguage.json`. VS Code loads it when the directory is linked or copied into `~/.vscode/extensions/gem-language`.
