# Gem Language — VS Code Extension

Syntax highlighting for the [Gem programming language](../../docs/SPEC.md), a dynamically typed language that compiles to C.

## Features

- Keyword highlighting (control flow, declarations, literals)
- String highlighting with escape sequences
- String interpolation (`"hello {name}"`) with nested expression highlighting
- Single-quoted strings (`'no interpolation'`)
- Named and anonymous function declarations
- Block parameter highlighting (`do |x, y|`, `{ |x| ... }`)
- `extern fn` declarations with type annotations (`Int`, `Float`, `String`, etc.)
- Built-in function highlighting
- Line comments (`#`)
- Bracket matching and auto-closing pairs
- Indentation rules (increase after `fn`, `if`, `while`, etc.; decrease on `end`, `else`, `elif`)

## Installation

This extension is not yet published to the VS Code Marketplace. To install locally:

```bash
# From the repo root
cd editors/vscode
npm install -g @vscode/vsce   # if not already installed
vsce package                  # produces gem-language-0.1.0.vsix
code --install-extension gem-language-0.1.0.vsix
```

Or open the `editors/vscode` folder in VS Code and press **F5** to launch an Extension Development Host.

## File Association

Files with the `.gem` extension are automatically detected as Gem source files.
