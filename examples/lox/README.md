# lox

A tree-walking interpreter for Lox, the language of Robert Nystrom's
[Crafting Interpreters](https://craftinginterpreters.com/), built the way
the book's jlox is: a scanner, a recursive-descent parser, a resolver and
an interpreter that walks the syntax tree. It is the yardstick for the
CPU-bound core of Gem, the part the other example programs barely touch:
function calls, recursion, closures, `match` dispatch, field access on
tables used as objects, and many short-lived tables.
`benchmarks/lox/run.sh` times it against the same interpreter in Python.

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

Exit status, as jlox's: 0, 65 for a syntax error (nothing runs), 70 for
a runtime error, 64 for a bad command line, 66 for an unreadable file.
Errors go to stderr in jlox's format (`[line 3] Error at ';': Expect
expression.`, `Operands must be numbers.` then `[line 3]`).

## What it supports

All of Lox: numbers, strings, booleans and `nil`; `var`, blocks and
lexical scope; `if`, `while`, `for`, `and`, `or`; functions, closures,
recursion and `return`; classes with fields, methods, `init`, `this`,
inheritance and `super`. The resolver reports the static errors jlox
does (`return` at the top level, `this` outside a class, a local read in
its own initializer, ...).

Natives: `clock()` (seconds), and three that Lox lacks and the bench
programs need: `str(v)` (`v` as `print` shows it), `len(s)` (the byte
length of a string) and `arg(i)` (the i-th argument after the script
path, a number when it reads as one, `nil` past the end).

Differences from jlox:

- **Numbers print** as Gem and Python print floats (the shortest text
  that reads back exactly: `0.1`, `1e+16`), and integral values below
  1e16 without `.0`, as jlox does. jlox uses Java's format (`1.0E16`).
- **Division by zero** is a runtime error (`Division by zero.`); jlox
  gives `Infinity`. Gem raises on a float division by zero, so the
  interpreter checks first.
- **Calls nest at most 256 deep**, then `Stack overflow.` (jlox: as deep
  as the JVM's stack allows). A function whose body nests blocks deeply
  can run out of Gem stack before that; it reports the same error,
  without a line.
- `NaN == NaN` is false (IEEE); jlox says true.

## Layout

| File | What it holds |
|---|---|
| `main.gem` | the command: reads the script, runs it, prints the errors |
| `lox.gem` | scan, parse, resolve, run; returns the exit status and errors |
| `lexer.gem` | source → tokens |
| `parser.gem` | tokens → syntax tree (tables with a `kind`) |
| `resolver.gem` | the scope depth of each variable use, and the static errors |
| `interp.gem` | the tree walker, the runtime values and the natives |
| `bench/*.lox` | the benchmark programs; `*.expected` is each one's output at the size `test.gem` runs it at |
| `test.gem` | unit tests, whole programs and their errors, the bench programs at small sizes |

## Design

It follows jlox closely, so the work it does per Lox operation is what a
textbook interpreter does, and the Python twin
(`benchmarks/lox/lox.py`) is the same program module for module.

- **The syntax tree is tables.** Each node is a record with a `kind`
  (`{kind: "binary", op: "+", left, right, line}`), and `evaluate` and
  `execute` dispatch with a `match` on it.
- **Lox values are Gem values** where they can be: `nil`, booleans,
  floats, strings. Lox's truthiness and `==` are Gem's, so `if`, `not`
  and `==` work on them directly. Functions, classes and instances are
  records with a `kind`; an instance keeps its fields in a table.
- **An environment is `{values, enclosing}`**, a new one per block and
  per call. The resolver stores on each variable node how many
  environments up its declaration is (nil for a global), so a lookup
  walks that many links and does one table lookup.
- **`return` is a value, not an error.** Statements return nil, or a
  `{value}` record when a `return` ran, and every statement passes it up
  until the call takes it. Runtime errors, which end the program, are
  `error()`s caught once in `interp.run`; syntax errors unwind the parser
  to the next statement the same way.
- **Interpreter state** (the globals, the call depth, the output sink)
  is module-level: one program runs at a time, in one process.

## Performance

`benchmarks/lox/run.sh` runs each bench program through both
interpreters and checks that the outputs match. On a 4-core Linux x86_64
VM (October 2026, two runs), Gem takes 0.66 to 1.15 times Python's
time: a compiled Gem program runs a tree walker at about CPython's speed.

| Program | What it exercises | Gem/Python time |
|---|---|---|
| `fib.lox 28` | calls and recursion | 1.09–1.15 (peak RSS 1.9 GB against 11 MB) |
| `binary_trees.lox 12` | allocating instances, a long-lived tree | 0.66–0.69 |
| `closures.lox 100000` | closures, captured variables | 0.90–0.97 |
| `strings.lox 20000` | string concatenation and comparison | 0.76–0.79 |
| `mandelbrot.lox 60` | float arithmetic in loops | 0.66–0.69 |
| `methods.lox 3000` | method calls, fields, `super` | 0.92–0.93 |

Where it goes:

- **Field access hashes the name every time.** The inline cache at a
  `t.field` site remembers one table, and the walker sees a different
  node or environment table at nearly every access: 99% of them miss
  and hash the key. Looking up string keys is 40 to 45% of the
  instructions in every program.
- **Every interpreted call pays for a return point.** The interpreter's
  functions call each other recursively, so each records where its call
  began in the arena, and its return frees what the call allocated once
  that passes 1 MB: `fib(28)`'s million calls stay at 14 MB. The point
  costs about 20 instructions a Gem call, 2 to 3% of the instructions.
- **Freed memory comes back as fresh pages.** A reset unmaps the blocks
  it frees and the next allocations map new ones, so the Gem runs spend
  20 to 30% of their time in the kernel taking page faults.
- **Trees are copied at every level that builds them.** `binary_trees`
  builds each subtree in one iteration of the interpreter's statement
  loop, and each level's loop starts a fresh mark whose first reset
  copies the subtree it finds, and the returns that hand a subtree up
  copy it too: 1.7 GB in 5,900 resets.
- **Gem frames are large.** A Lox call takes about ten Gem frames of 0.5
  to 2 KB, which is why the call depth is capped at 256.

These are tracked in `docs/OPTIMIZATIONS.md` ("Inline caches key on the
table, not its shape", "A return point costs about 20 instructions a
call", "Resets unmap the blocks they free", "A loop's first reset is
full", "Return resets copy a large live return value whole", "Large C
frames"). Keep
this program idiomatic: it is the yardstick for those fixes, not a place
to work around them.
