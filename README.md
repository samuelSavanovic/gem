# Gem

A small, dynamically typed language with Erlang-style concurrency that compiles to C.

```gem
fn greet(name)
  "hello {name}"
end

let pid = spawn do
  receive
  when {from: p, name: n}
    send(p, greet(n))
  end
end

send(pid, {from: self(), name: "world"})
print(receive())   # → hello world
```

Processes are cheap (stackful coroutines), each gets its own arena, messages are deep-copied across process boundaries. No shared mutable state, no global GC pauses, "let it crash" with supervisors and links. Tables with closures stand in for objects; `error()`/`pcall()` for error handling. No classes, no static types, no exceptions.

The compiler is self-hosting: `build/gem` compiles `compiler/main.gem` to C and links a fresh binary. `bootstrap/stage0.c` is the escape hatch checked into git so any C compiler can rebuild from scratch.

## Build

Requires a C compiler (`cc`) and `make`.

```sh
make build      # build/gem from bootstrap/stage0.c
make test       # run all numbered examples in examples/
make bootstrap  # regenerate stage0.c from current compiler sources
```

## Run a program

```sh
build/gem examples/01_basics.gem            # compile and run
build/gem examples/01_basics.gem -o hello   # compile to ./hello, don't run
build/gem examples/01_basics.gem --emit-c   # print generated C
```

## Platforms

- **macOS** (arm64, x86_64) — supported.
- **Linux** (arm64, x86_64) — supported.
- **Windows** — use WSL2. A native port would need to replace the POSIX I/O layer (poll, pthreads, BSD sockets, fork/exec) with Win32 equivalents; not done.

## Where things live

- `docs/SPEC.md` — language spec and CLI reference (source of truth).
- `compiler/` — self-hosting compiler (lexer, parser, liveness, codegen).
- `runtime/` — C runtime (scheduler, arenas, builtins, vendored minicoro/stb_ds/sqlite).
- `std/` — standard library written in Gem (string, table, json, http, sqlite, OTP behaviors, …).
- `examples/` — numbered tests + larger programs (json parser, http server, bookmark app).
- `editors/` — VS Code grammar and tree-sitter grammar for Helix.

## Status

Pre-1.0, working name. Spec is at v0.1. Things will move.
