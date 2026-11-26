# Gem for VS Code

Syntax highlighting for Gem.

## Features

- Keywords, strings with escapes, and interpolation (`"hello {name}"`) with the expression inside highlighted
- Single-quoted strings (`'no interpolation'`)
- Named and anonymous functions, and block parameters (`do |x, y|`, `{ |x| ... }`)
- `extern fn` declarations with their type annotations (`Int`, `Float`, `String`, ...)
- Builtin functions
- `#` comments
- Bracket matching, auto-closing pairs, and indentation after `fn`, `if`, `while` and the like, dedenting on `end`, `else`, `elif` and `when`

Files ending in `.gem` open as Gem.

## Installation

The extension isn't on the Marketplace. To install it from the repository:

```bash
cd editors/vscode
npm install -g @vscode/vsce   # if you don't have it
vsce package                  # makes gem-language-0.1.0.vsix
code --install-extension gem-language-0.1.0.vsix
```

You can also open `editors/vscode` in VS Code and press F5 to start an Extension Development Host.
