# Gem Language

A dynamically typed language that compiles to C. See `docs/SPEC.md` for the full spec.

## Project Structure

```
docs/SPEC.md          # language spec (source of truth for all language decisions)
prototype/            # C prototypes (Boehm GC + minicoro integration, scheduler + mailbox)
bootstrap/            # Python bootstrap compiler (WIP)
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
- **stb_ds.h** — single-header hash maps/arrays (not yet vendored)

## Key Decisions

- Bootstrap compiler targets ~1500 lines of Python. It will be deleted after self-hosting.
- Compilation target is C source code. `cc` handles optimization and linking.
- Coroutine stacks must be registered as GC roots via `GC_add_roots()` — see `prototype/gc_coro_test.c`.
- Scheduler and mailbox (spawn/send/receive) are built on minicoro, not a library — see `prototype/scheduler_test.c`.
- libdill is dead (crashes on arm64 macOS). Don't use it.

## Running Prototypes

```bash
cd prototype && make run
```
