# Gem Language

A dynamically typed language that compiles to C with Erlang-style concurrency. See `docs/SPEC.md` for the full spec.

## Project Structure

```
compiler/             # self-hosting compiler (lexer, parser, AST, codegen, main)
std/                  # standard library (string, table, math, supervisor, gen_server)
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
  minicoro.h          #   single-header coroutine library (vendored)
  stb_ds.h            #   single-header hash maps/arrays (vendored)
bootstrap/stage0.c    # checked-in C output — bootstrap artifact for clean builds
build/gem             # compiled compiler binary (gitignored, built from stage0.c)
examples/             # 45 numbered tests + run_all.sh, plus json_parser, http_server, tcp_echo
docs/SPEC.md          # language spec (source of truth for all language decisions)
docs/OPTIMIZATIONS.md # tracked future performance improvements
prototype/            # C prototypes (Boehm GC + minicoro integration, scheduler + mailbox)
editors/vscode/       # VS Code extension (TextMate grammar)
editors/tree-sitter-gem/  # tree-sitter grammar for Helix (+ queries)
```

## Setup

```bash
brew install bdw-gc    # Boehm GC
make build             # compiles bootstrap/stage0.c → build/gem
```

No Python, no external parser generators. The compiler is fully self-hosted.

## C Dependencies

- **Boehm GC** (`bdw-gc`) — garbage collector
- **minicoro** (`runtime/minicoro.h`) — single-header coroutine library, vendored
- **stb_ds.h** (`runtime/stb_ds.h`) — single-header hash maps/arrays, vendored

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
- Concurrency uses minicoro (stackful coroutines) with a round-robin scheduler. Each process gets a 16KB coroutine stack registered as a GC root via `GC_add_roots()`.
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

## Git Conventions

- Never add `Co-Authored-By` trailers to commit messages.
