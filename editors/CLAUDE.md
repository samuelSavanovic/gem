# Editor Extensions

## Tree-sitter Grammar (`tree-sitter-gem/`)

### Toolchain

- `tree-sitter-cli` is installed via npm (`npm install -g tree-sitter-cli`). Node is managed via **mise** — activated in all shells (fish, zsh).
- After editing `grammar.js`: run `tree-sitter generate`, then `hx --grammar build`.
- Helix reads queries from `~/.config/helix/runtime/queries/gem/` — copy updated query files there after changes.

### Grammar Design Decisions

- **`pattern` is a visible rule** (not `_pattern`) so it can appear in `conflicts: $ => [...]`. The parser uses GLR (`prec.dynamic`) to disambiguate `pattern` vs `array`/`table_pair` in `when` clauses.
- **`brace_block` is NOT attached to `call_expression`**. Brace blocks after calls (e.g. `times(5) { |i| ... }`) are intentionally unsupported in the grammar because `{` after a call is ambiguous with table literals. Only `do ... end` blocks work on calls. This is a pragmatic trade-off — very few files use brace blocks on calls vs. many use table literals after calls.
- **`call_with_block`** handles `identifier do ... end` (no parens) as a separate rule from `call_expression`.
- **`break_statement` and `continue_statement`** are single-keyword named nodes. In highlight queries, use `(break_statement) @keyword` not `"break" @keyword` — there are no anonymous children to match.
- **Triple-quoted strings** are opaque `token(seq(...))` nodes — no interpolation or escape highlighting inside them.
- **`_string`** is the choice node for all 4 string types — use it anywhere a string is needed (load, table keys, etc.).

### Testing Changes

After any grammar.js change, verify with:

```bash
tree-sitter generate
for f in $(find examples compiler std -name '*.gem'); do tree-sitter parse "$f" 2>&1 | grep -q ERROR && echo "FAIL: $f"; done
```

All 66+ `.gem` files should parse with zero errors. If a change introduces parse errors, fix them before committing.

### Helix Config

Helix config lives at `~/.config/helix/`:
- `languages.toml` — gem language definition + grammar source path
- `runtime/queries/gem/highlights.scm` — highlight queries
- `runtime/queries/gem/indents.scm` — indent/outdent rules
- `runtime/queries/gem/textobjects.scm` — function/class/parameter/comment textobjects

## VS Code Extension (`vscode/`)

TextMate grammar at `syntaxes/gem.tmLanguage.json`. Symlinked into `~/.vscode/extensions/gem-language`.
