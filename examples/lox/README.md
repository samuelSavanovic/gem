# lox

A tree-walking interpreter for Lox, the language of Robert Nystrom's [Crafting Interpreters](https://craftinginterpreters.com/), built the way the book's jlox is: a scanner, a recursive-descent parser, a resolver and an interpreter that walks the syntax tree. It exercises the CPU-bound side of Gem that the other examples barely touch: function calls, recursion, closures, `match` dispatch, field access on tables used as objects, and lots of short-lived tables.

```sh
cd examples/lox
../../build/gem main.gem -o lox
./lox bench/fib.lox 25
./lox bench/mandelbrot.lox
../../build/gem test.gem              # the tests (part of make test)
```

```
fib(0) = 0
...
fib(25) = 75025
```

The exit status follows jlox: 0, 65 for a syntax error (nothing runs), 70 for a runtime error, 64 for a bad command line, 66 for an unreadable file. Errors go to stderr in jlox's format (`[line 3] Error at ';': Expect expression.`, or `Operands must be numbers.` followed by `[line 3]`).

## What it supports

All of Lox: numbers, strings, booleans and `nil`; `var`, blocks and lexical scope; `if`, `while`, `for`, `and`, `or`; functions, closures, recursion and `return`; classes with fields, methods, `init`, `this`, inheritance and `super`. The resolver reports the same static errors jlox does (`return` at the top level, `this` outside a class, a local read in its own initializer, and so on).

The natives are `clock()` (seconds) and three that Lox lacks and the bench programs need: `str(v)` (`v` as `print` shows it), `len(s)` (the byte length of a string) and `arg(i)` (the i-th argument after the script path, a number when it reads as one, `nil` past the end).

Differences from jlox:

- Numbers print the way Gem and Python print floats (the shortest text that reads back exactly: `0.1`, `1e+16`), and integral values below 1e16 without `.0`, as jlox does. jlox uses Java's format (`1.0E16`).
- Division by zero is a runtime error (`Division by zero.`). jlox gives `Infinity`.
- Calls nest at most 256 deep, then fail with `Stack overflow.`. A function whose body nests blocks deeply can run out of Gem stack before that, and reports the same error without a line.
- `NaN == NaN` is false (IEEE). jlox says true.

## Layout

| File | What it holds |
|---|---|
| `main.gem` | the command: reads the script, runs it, prints the errors |
| `lox.gem` | scan, parse, resolve, run; returns the exit status and errors |
| `lexer.gem` | source to tokens |
| `parser.gem` | tokens to syntax tree (tables with a `kind`) |
| `resolver.gem` | the scope depth of each variable use, and the static errors |
| `interp.gem` | the tree walker, the runtime values and the natives |
| `bench/*.lox` | the benchmark programs; each `*.expected` is the output at the size `test.gem` runs it at |
| `test.gem` | unit tests, whole programs and their errors, the bench programs at small sizes |

## Design

It follows jlox closely, and `benchmarks/lox/lox.py` is the same program in Python, module for module.

Each syntax tree node is a table with a `kind` (`{kind: "binary", op: "+", left, right, line}`), and `evaluate` and `execute` dispatch on it with `match`. Lox values are Gem values where possible (`nil`, booleans, floats, strings), so Lox's truthiness and `==` are Gem's. Functions, classes and instances are tables with a `kind`, and an instance keeps its fields in a table.

An environment is `{values, enclosing}`, a new one per block and per call. The resolver stores on each variable node how many environments up its declaration is (nil for a global), so a lookup follows that many links and does one table lookup.

Statements return nil, or a `{value}` table when a `return` ran, and every statement passes it up until the call takes it. Runtime errors end the program: they are `error()`s caught once in `interp.run`. Syntax errors unwind the parser to the next statement the same way. The interpreter's state (globals, call depth, output) is module-level, since one program runs at a time in one process.

## Performance

`benchmarks/lox/run.sh` runs each bench program through both interpreters and checks that the outputs match. On an M1 Pro:

| Program | What it exercises | Gem/Python time | peak RSS, Gem / Python |
|---|---|---|---|
| `fib.lox 28` | calls and recursion | 0.49 | 7 / 21 MB |
| `binary_trees.lox 12` | allocating instances, a long-lived tree | 0.65 | 27 / 24 MB |
| `closures.lox 100000` | closures, captured variables | 0.49 | 6 / 21 MB |
| `strings.lox 20000` | string concatenation and comparison | 0.43 | 4 / 20 MB |
| `mandelbrot.lox 60` | float arithmetic in loops | 0.43 | 5 / 21 MB |
| `methods.lox 3000` | method calls, fields, `super` | 0.50 | 6 / 21 MB |
