# Gem Language

A dynamically typed language that compiles to C. See `docs/SPEC.md` for the full spec.

## Project Structure

```
docs/SPEC.md          # language spec (source of truth for all language decisions)
prototype/            # C prototypes (Boehm GC + minicoro integration, scheduler + mailbox)
bootstrap/            # Python bootstrap compiler (WIP)
runtime/              # standalone C runtime (gem.h + gem.c + stb_ds.h)
examples/             # test programs + run_all.sh
```

## Setup

```bash
python3.14 -m venv .venv
source .venv/bin/activate
pip install lark
```

- **Lark** — EBNF parser generator. Gem grammar defined declaratively, Lark builds the parse tree. Use `propagate_positions=True` for line/column info in error messages. Use Earley parser (handles ambiguity, speed doesn't matter for bootstrap).

## C Dependencies (for compiling Gem output)

```bash
brew install bdw-gc
```

- **Boehm GC** (`bdw-gc`) — garbage collector
- **minicoro** (`prototype/minicoro.h`) — single-header coroutine library, vendored
- **stb_ds.h** (`runtime/stb_ds.h`) — single-header hash maps/arrays, vendored

## Key Decisions

- Bootstrap compiler targets ~1500 lines of Python. It will be deleted after self-hosting.
- Compilation target is C source code. `cc` handles optimization and linking.
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
