# Known bugs

Bugs found and not yet fixed. Each entry has a minimal repro (checked
against `build/gem`), what goes wrong, and where the code is. A fix deletes
its entry in the same change, along with any **(bug)** rule in
`docs/BEST_PRACTICES.md` that exists because of it. Performance problems go in
`docs/OPTIMIZATIONS.md`, missing features in `docs/ROADMAP.md`.

## Compiler

## Runtime

## Standard library

## Language server

## Editor grammars

### tree-sitter grammar rejects `pcall` of an assignment

```gem
let a = 1
print((pcall a = 9).value)   # the compiler accepts it: nil
```

`tree-sitter parse` reports an ERROR node at `= 9`, so
`examples/194_compiler_known_bugs.gem` (line 84) fails the "every `.gem`
file parses" check in `editors/CLAUDE.md`. The `pcall <expr>` form in
`editors/tree-sitter-gem/grammar.js` takes an expression, and an
assignment is a statement there, while compiler/parser.gem accepts an
assignment after `pcall`.

## Example programs
