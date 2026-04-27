# Tree-sitter Grammar for Gem (Helix)

## After Grammar Changes

When `grammar.js` or query files are updated (new builtins, keywords, syntax):

```bash
cd editors/tree-sitter-gem

# 1. Regenerate the parser (requires node via mise)
tree-sitter generate

# 2. Rebuild the Helix grammar
hx --grammar build

# 3. Copy query files to Helix runtime
cp queries/highlights.scm ~/.config/helix/runtime/queries/gem/
```

Restart Helix to pick up the changes.

Indent and textobject queries live in `~/.config/helix/runtime/queries/gem/` (not in this repo). If those need updating, edit them directly:

- `~/.config/helix/runtime/queries/gem/indents.scm`
- `~/.config/helix/runtime/queries/gem/textobjects.scm`

## Verify

```bash
# Parse all .gem files — should be zero errors
cd /path/to/gem
for f in $(find examples compiler std -name '*.gem'); do
  tree-sitter parse "$f" 2>&1 | grep -q ERROR && echo "FAIL: $f"
done

# Check Helix sees everything
hx --health gem
```

All four checks (parser, highlights, textobjects, indents) should show green.
