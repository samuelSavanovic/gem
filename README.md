# Gem

A small dynamic language with Erlang-style concurrency that compiles to C. Built to be pleasant to write small networked services in - request handlers, message brokers, supervised processes - without a VM or a runtime that's bigger than the program.

```gem
let pid = spawn() do
  receive
  when {from: p, msg: m}
    send(p, "echo: {m}")
  end
end

send(pid, {from: self(), msg: "hi"})
print(receive())   # -> echo: hi
```

Ruby-ish syntax (blocks-as-trailing-arg, `do/end`), Lua-ish data model (tables for everything - objects, dicts, arrays, modules), Erlang-ish concurrency (processes, mailboxes, monitors, links, supervisors). Compiles to C; vendors `minicoro` for stackful coroutines and `stb_ds` for hash tables. Bootstrap is a checked-in `stage0.c` so any C compiler can rebuild from scratch.

A *process* is a lightweight green thread with its own heap and mailbox, cheap enough to start one per connection or task: about 21 KB each when idle on Linux x86_64 (70 KB on macOS arm64, with its 16 KB pages), with about 14,000 alive at once on a stock Linux and more with a raised `vm.max_map_count`. Processes share nothing and talk only by message.

## Memory model

The interesting design choice. In short:

- **Inside a process, tables are references.** `let b = a` and passing `a` to a function share the same table, as in Lua or JavaScript. Strings and numbers are values.
- **Between processes, everything is copied.** `send` deep-copies the message into the receiver's heap, and `spawn` copies the variables the new process captures and the module-level state. No shared memory, no locks, no data races.
- **No tracing GC, no global pause.** Each process allocates from its own arena, freed in one go when the process exits. For long-running processes, the compiler inserts a reset at the back-edge of every loop and self tail call: a liveness analysis decides what the loop still uses, the runtime copies that and frees the rest. `while true ... end` in an accept loop stays at constant memory without any annotation.
- **The costs are visible.** A message costs a copy proportional to its size. A loop reset costs time proportional to what the loop keeps. Non-tail recursion keeps its memory until it returns. If the compiler can't reset a `while true` loop, it warns instead of leaking silently.

The full rules are in [docs/SPEC.md, "Memory Model"](docs/SPEC.md#memory-model); the practical advice is in [docs/BEST_PRACTICES.md, "State and memory"](docs/BEST_PRACTICES.md#state-and-memory).

## OTP in plain Gem

OTP-style abstractions are written in pure Gem on top of the actor primitives. `gen_server` and `supervisor` are a few hundred lines each. No special compiler support - they fall out of `spawn` + `receive ... when` + selective receive + tail-recursive loops.

```gem
load "std/gen_server"

let counter = {
  init: fn() {state: 0} end,
  handle_call: fn(msg, from, state)
    match msg
    when "get"
      {reply: state, state: state}
    when "inc"
      {reply: state + 1, state: state + 1}
    end
  end,
  handle_cast: fn(msg, state) {state: 0} end,
  handle_info: fn(msg, state) {state: state} end
}

let {pid} = gen_server.start(counter)
gen_server.call(pid, "inc")    # 1
gen_server.call(pid, "inc")    # 2
```

The standard library is written in Gem and includes `string`, `table`, `math`, `json` (passes 283/283 of JSONTestSuite), `url`, `mime`, `time`, `log`, `http` (server with routing and keep-alive), `request` (HTTP client), `sqlite`, `task`, `gen_server`, `supervisor`, `dynamic_supervisor`, and `test`.

## What Gem is not

Not a general-purpose language. Good at protocol parsing, request handling, and supervised long-running processes; not aimed at numeric computing, systems programming, or anything that needs a polished editor experience.

Not production software for anyone but me. The HTTP server has held ~25k req/s on a laptop (GET `/`, c=4, p50=141µs, p99=2.96ms) and survived a 5-minute / 7.5M-request soak with stable RSS, but there's no security audit, no commitment to backwards compatibility, and TLS is deliberately not in the runtime (terminate at a reverse proxy).

Not a community project. Personal weekend work. Issues and PRs welcome but response time is "when I have a weekend free."

## Build & run

Requires a C compiler and `make`.

```sh
make build      # build/gem from bootstrap/stage0.c
make test       # run all numbered examples
make bootstrap  # regenerate stage0.c from current compiler sources

build/gem examples/01_basics.gem            # compile and run
build/gem examples/01_basics.gem -o hello   # compile, don't run
build/gem examples/01_basics.gem --emit-c   # print generated C
build/gem examples/01_basics.gem --check    # parse + analyze only
```

To put `gem` on your `PATH`, symlink it (`ln -s "$PWD/build/gem" ~/.local/bin/gem`) or add `build/` to `PATH`. The binary finds `std/` and `runtime/` two levels above its real path (symlinks resolved), so keep it inside the checkout.

macOS (arm64, x86_64) and Linux (arm64, x86_64). Windows via WSL2; no native port.

## Layout

- `docs/SPEC.md` - language spec, source of truth.
- `docs/BEST_PRACTICES.md` - how to write Gem: idioms and traps.
- `compiler/` - self-hosting compiler (lexer, parser, liveness, codegen).
- `runtime/` - C runtime (scheduler, arenas, builtins, vendored deps).
- `std/` - standard library, in Gem.
- `examples/` - numbered tests and larger programs: a JSON parser, a TCP echo server, a bookmark web app on SQLite (`bookmark_app/`), a STOMP message broker (`stomp_broker/`) and a command-line log analyzer (`logstat/`).
- `editors/` - VS Code grammar and tree-sitter grammar for Helix.

## Status

Pre-1.0. Memory model, concurrency primitives, and surface syntax are stable. Stdlib APIs may grow but existing ones are unlikely to break.
