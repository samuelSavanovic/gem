# tree-sitter-gem

A tree-sitter grammar for Gem, used with Helix. The generated parser is checked in under `src/`, and `queries/` has the highlight queries.

## After changing the grammar

When `grammar.js` or a query changes (a new builtin, keyword or construct):

```bash
cd editors/tree-sitter-gem
tree-sitter generate          # regenerate src/ (needs node)
hx --grammar build            # rebuild the grammar for Helix
cp queries/highlights.scm ~/.config/helix/runtime/queries/gem/
```

Then restart Helix. The repository has no indent or textobject queries; Helix reads those from the same `queries/gem/` directory if you write your own.

## Verify

Every `.gem` file in the repository should parse without errors:

```bash
# from the repository root
for f in $(find examples std compiler lsp tests benchmarks editors -name '*.gem' -not -path 'tests/broken/*'); do
  (cd editors/tree-sitter-gem && tree-sitter parse "../../$f" 2>&1) | grep -q 'ERROR\|MISSING' && echo "FAIL: $f"
done

hx --health gem
```
