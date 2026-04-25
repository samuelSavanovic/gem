# Gem Language

A dynamically typed language that compiles to C. See `docs/SPEC.md` for the full spec.

## Project Structure

```
compiler/             # self-hosting compiler (lexer, parser, AST, codegen, main)
runtime/              # standalone C runtime (gem.h + gem.c + stb_ds.h)
bootstrap/stage0.c    # checked-in C output — bootstrap artifact for clean builds
build/gem             # compiled compiler binary (gitignored, built from stage0.c)
examples/             # test programs + run_all.sh
docs/SPEC.md          # language spec (source of truth for all language decisions)
prototype/            # C prototypes (Boehm GC + minicoro integration, scheduler + mailbox)
```

## Setup

```bash
brew install bdw-gc    # Boehm GC
make build             # compiles bootstrap/stage0.c → build/gem
```

No Python, no external parser generators. The compiler is fully self-hosted.

## C Dependencies

- **Boehm GC** (`bdw-gc`) — garbage collector
- **minicoro** (`prototype/minicoro.h`) — single-header coroutine library, vendored
- **stb_ds.h** (`runtime/stb_ds.h`) — single-header hash maps/arrays, vendored

## Building

```bash
make build             # stage0.c → build/gem (first time or after clean)
make bootstrap         # regenerate stage0.c from current compiler sources (verified roundtrip)
make clean             # remove build/ and /tmp/gem_*
```

After changing compiler sources, run `make bootstrap` to update `stage0.c`. The bootstrap target verifies the new stage0 can compile itself (fixed-point check) before replacing it.

## Spec Maintenance

After any language change (new syntax, new builtin, changed semantics), update `docs/SPEC.md` to reflect the change. The spec is the source of truth — if it disagrees with the code, fix the spec. Also update `docs/SELF_HOST_FOLLOWUPS.md` if the change completes a listed item.

## Key Decisions

- Compilation target is C source code. `cc` handles optimization and linking.
- The compiler is self-hosting: `build/gem` compiles `compiler/main.gem` → C → new binary.
- `bootstrap/stage0.c` is the escape hatch — checked into git so any C compiler can rebuild from scratch.
- Coroutine stacks must be registered as GC roots via `GC_add_roots()` — see `prototype/gc_coro_test.c`.
- Scheduler and mailbox (spawn/send/receive) are built on minicoro, not a library — see `prototype/scheduler_test.c`.
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
