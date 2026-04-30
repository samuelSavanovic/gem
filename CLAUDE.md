# Gem Language

A dynamically typed language that compiles to C with Erlang-style concurrency. See `docs/SPEC.md` for the full spec.

## Design Philosophy

**Gem the language must be simple to write.** The OTP model (supervisors, gen_servers, "let it crash", spawn/send/receive, recursion-as-iteration via TCO) is already the learnability tax — that's the cognitive ceiling. The language layer must not pile more on top.

When evaluating a runtime or codegen mechanism, ask: *does this leak a concept the user has to track to write correct or performant code?* If yes, push the burden into the compiler (static analysis) or the runtime (uniform mechanism), not the user.

Concretely:
- No annotations like `@process_loop` to mark TCO-eligible loops. Auto-detect.
- No "you must structure your code this way for X to work" runtime constraints (e.g. depth fences). Either auto-detect via static analysis or make the mechanism work uniformly.
- Compiler warnings are OK when they surface a hidden constraint ("this loop won't reset because it's reachable from a non-tail context"). Silent perf cliffs are not.
- Optimizations that introduce DX warts (depth fences, hand-inlined wrappers, manual aliasing tricks) are tolerable only as transient hacks with a structural fix on the roadmap.

## Project Structure

```
compiler/             # self-hosting compiler (lexer, parser, AST, codegen, main)
std/                  # standard library (string, table, math, supervisor, gen_server, sqlite, ...)
runtime/              # C runtime — split by category:
  gem.h               #   public API, tagged values, process table, scheduler decls
  gem_core.c          #   value constructors, table internals, equality
  gem_ops.c           #   arithmetic and comparison operators
  gem_error.c         #   error handling, stack trace printing
  gem_scheduler.c     #   coroutine scheduler, mailbox, process lifecycle, I/O polling
  gem_threadpool.c    #   worker thread pool for blocking I/O
  gem_builtins_core.c #   print, error, len, type, conversions, pcall, argv, etc.
  gem_builtins_collection.c  # push, pop, keys, values, sort, insert, delete
  gem_builtins_string.c      # str_replace, substr, chr/ord, buf_* API
  gem_builtins_math.c        # math ops, random, bitwise operations
  gem_builtins_io.c          # file I/O, filesystem ops, exec
  gem_builtins_tcp.c         # TCP socket operations (non-blocking)
  gem_builtins_sqlite.c      # SQLite operations (open, close, exec, query, etc.)
  minicoro.h          #   single-header coroutine library (vendored)
  stb_ds.h            #   single-header hash maps/arrays (vendored)
  sqlite3.c/sqlite3.h #  SQLite amalgamation (vendored)
bootstrap/stage0.c    # checked-in C output — bootstrap artifact for clean builds
build/gem             # compiled compiler binary (gitignored, built from stage0.c)
examples/             # 49 numbered tests + run_all.sh, plus json_parser, http_server, tcp_echo
docs/SPEC.md          # language spec (source of truth for all language decisions)
docs/OPTIMIZATIONS.md # tracked future performance improvements
prototype/            # C prototypes (Boehm GC + minicoro integration, scheduler + mailbox)
editors/vscode/       # VS Code extension (TextMate grammar)
editors/tree-sitter-gem/  # tree-sitter grammar for Helix (+ queries)
```

## Setup

```bash
make build             # compiles bootstrap/stage0.c → build/gem
```

No Python, no external parser generators. The compiler is fully self-hosted.

## C Dependencies

- **minicoro** (`runtime/minicoro.h`) — single-header coroutine library, vendored
- **stb_ds.h** (`runtime/stb_ds.h`) — single-header hash maps/arrays, vendored
- **SQLite** (`runtime/sqlite3.c`, `runtime/sqlite3.h`) — amalgamation, vendored

## Building

```bash
make build             # stage0.c → build/gem (first time or after clean)
make bootstrap         # regenerate stage0.c from current compiler sources (verified roundtrip)
make clean             # remove build/ and /tmp/gem_*
```

After changing compiler sources, run `make bootstrap` to update `stage0.c`. The bootstrap target verifies the new stage0 can compile itself (fixed-point check) before replacing it.

## Adding a New Builtin

1. Implement the C function in the appropriate `runtime/gem_builtins_*.c` file
2. Add the declaration to `runtime/gem.h`
3. Add the name → C function mapping in `compiler/codegen.gem` (`builtin_fns` table)
4. Update `docs/SPEC.md`
5. Update both editor extensions (see below)
6. Write tests, run `make test`, run `make bootstrap`

## Optimization Tracking

`docs/OPTIMIZATIONS.md` tracks future performance improvements. When implementing something that has an obvious optimization opportunity (e.g. a new builtin that copies when it could use views, a hot path that could be specialized), add it there rather than implementing it immediately. Keep the file organized by category.

## Spec Maintenance

After any language change (new syntax, new builtin, changed semantics), update `docs/SPEC.md` to reflect the change. The spec is the source of truth — if it disagrees with the code, fix the spec.

## Editor Extension Maintenance

After any language change, update **both** editor extensions:

### VS Code (`editors/vscode/syntaxes/gem.tmLanguage.json`)

- **New keyword** — add it to the appropriate pattern in the `keyword` repository rule (`keyword.control.gem`, `keyword.other.gem`, or `keyword.declaration.function.gem`)
- **New builtin function** — add it to the alternation in the `builtin` rule
- **New syntax construct** — add a new repository rule and include it in the top-level `patterns` array at the right priority (before `#keyword` and `#identifier` if it needs to take precedence)
- **New type annotation** (extern context) — add it to the alternation in `extern-type-annotation`

The symlink at `~/.vscode/extensions/gem-language` → `editors/vscode` means changes take effect on VS Code reload with no reinstall needed.

### Helix (`editors/tree-sitter-gem/`)

- **New keyword** — tree-sitter picks it up automatically if it's a string literal in grammar.js; add a highlight query entry in `queries/highlights.scm`
- **New builtin function** — add it to the `#match?` regex in highlights.scm (both `call_expression` and `call_with_block` patterns)
- **New syntax construct** — add a rule in `grammar.js`, regenerate, and add highlight/indent/textobject queries as needed
- **New type annotation** — add it to the `type` choice in grammar.js

After grammar.js changes: `tree-sitter generate` (requires node via mise), `hx --grammar build`, then copy `queries/highlights.scm` to `~/.config/helix/runtime/queries/gem/`. Test with `tree-sitter parse` on all `.gem` files to verify zero errors.

## Key Decisions

- Compilation target is C source code. `cc` handles optimization and linking.
- The compiler is self-hosting: `build/gem` compiles `compiler/main.gem` → C → new binary.
- `bootstrap/stage0.c` is the escape hatch — checked into git so any C compiler can rebuild from scratch.
- Concurrency uses minicoro (stackful coroutines) with a round-robin scheduler. Each process gets its own arena (bump allocator); values are deep-copied across process boundaries (spawn, send). Arenas are freed in bulk when a process exits.
- Preemptive scheduling via reduction counter at loop back-edges (threshold: 4000).
- TCP builtins use non-blocking sockets + `poll()` in the scheduler loop. Blocking operations (file I/O, exec, `extern blocking fn`) use a 4-thread worker pool.
- Selective receive (`receive ... when ... end`) scans the mailbox and removes the first matching message, leaving others queued.
- Process monitoring, linking, and `trap_exit` follow Erlang semantics.
- Named processes via `register`/`whereis` with auto-cleanup on death.
- Tail call optimization for self-recursive functions emits `while(1)` loops with parameter reassignment.
- `extern fn` / `extern blocking fn` provide C interop with type marshaling.
- No classes, static types, exceptions, or namespaces in v0 — by design. Tables with closures serve as objects, `error()`/`pcall()` for error handling.
- libdill is dead (crashes on arm64 macOS). Don't use it.

## Testing

After any compiler change, run edge-case and adversarial tests before considering the work done. This means:

1. **Happy path** — basic programs that exercise the new feature
2. **Edge cases** — empty bodies, zero args, boundary values, nested constructs
3. **Adversarial inputs** — malformed source, missing tokens, type mismatches at runtime
4. **Regression** — re-run previous features to make sure nothing broke

Write a `.gem` test file, compile and run it, verify output. For expected errors (parse errors, runtime errors), confirm the compiler or program fails with a sensible message rather than crashing or silently producing wrong output.

## Running Tests

```bash
make test          # run all numbered examples against expected output
make test-json     # run JSON parser stress test
```

## Running Prototypes

```bash
cd prototype && make run
```

## Language Quick Reference

Keep this section up to date when adding new syntax, keywords, builtins, or std modules.

```gem
# Variables
let x = 10
let {a, b} = tbl          # table destructuring
let [first, second] = arr  # array destructuring

# Functions — fn/end, last expression is implicit return
fn add(a, b)
  a + b
end
fn greet(name, greeting = "Hello")   # default params
  print("{greeting}, {name}!")
end
fn log(level, ...msgs)               # variadic (rest param)
end

# Closures / anonymous functions
let f = fn(x) x * 2 end

# Blocks — trailing do/end or { } passed as last arg
items.each do |item|
  print(item)
end
items.each { |item| print(item) }

# Control flow — end-terminated, elif (not else if)
if cond then expr else expr end      # single-line
if cond
  body
elif cond2
  body
else
  body
end

while cond
  body
end

for item in arr ... end              # array iteration
for k, v in tbl ... end              # key-value iteration
for i = 0, n ... end                 # range [0, n)

match val
when "a"
  handle_a()
when {ok: true, value: v}            # destructuring pattern
  use(v)
else
  fallback()
end

# Modules — load (NOT import), export at end of file
load "std/string"                    # => string.split(...)
load "std/string" as str             # => str.split(...)
load "std/string" (split, trim)      # => split(...) directly

export my_fn, my_other_fn            # at end of module file

# Strings — double-quoted: interpolation, single-quoted: literal
"hello {name}"                       # interpolation with { }
'no {interpolation} here'            # literal braces
"""multi-line with {interpolation}"""
'''multi-line literal'''

# Operators — and/or/not (NOT &&/||/!), x in tbl
# Tables — { key: val } or [1, 2, 3], dot access, bracket access
# Logical — nil and false are falsy, everything else truthy

# Concurrency
let pid = spawn do ... end
send(pid, msg)
let msg = receive()                  # pop head
receive                              # selective receive
when {tag: "DOWN", pid: p}
  handle(p)
after 5000
  timeout()
end
monitor(pid)
link(pid)
process_flag("trap_exit", true)
register("name", self())

# Error handling
error("msg")                         # halt with stack trace
let r = pcall some_fn()              # {ok: bool, value/error: ...}

# Common builtins
# print, len, type, to_string, to_int, to_float
# push, pop, keys, values, sort, insert, delete, remove_at
# str_replace, substr, chr, ord, has_key
# buf_new, buf_push, buf_str, build_string
# push/to_string work on buffers too

# String building — build_string needs () before do block
let s = build_string() do |add|
  add("hello", " ", "world")           # multi-arg, no intermediate allocs
end

# read_file, write_file, exec, sleep, self, spawn
# tcp_listen, tcp_accept, tcp_read, tcp_write, tcp_close

# Std library modules
# std/string (split, join, trim, index_of, contains, ...)
# std/table (each, map, filter, reduce, find, ...)
# std/json (parse, encode)
# std/http (router, serve, ok, html, json_response, ...)
# std/sqlite (open, close, exec, query, ...)
# std/math (min, max, clamp, assert)

# C interop
extern fn puts(s: String) -> Int
extern blocking fn net_read(fd: Int) -> String
extern include "header.h"
```

## Git Conventions

- Never add `Co-Authored-By` trailers to commit messages.
