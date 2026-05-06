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

The interesting design choice is the memory model. Each process has its own arena. Messages are deep-copied across process boundaries. There's no GC. Long-running processes work because the compiler emits an arena reset at loop back-edges in process-tail position, with a compile-time liveness pass deciding what to rescue. User code never thinks about lifetimes - `while true ... end` in an accept loop just works.

OTP-style abstractions are written in pure Gem on top of the actor primitives. `gen_server` is 83 lines, `supervisor` is 169. No special compiler support - they fall out of `spawn` + `receive ... when` + selective receive + tail-recursive loops.

```gem
load "std/gen_server"

let counter = {
  init: fn() 0 end,
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

The standard library is written in Gem and includes `string`, `table`, `math`, `json` (passes 283/283 of JSONTestSuite), `url`, `mime`, `time`, `log`, `http` (server with routing and keep-alive), `request` (HTTP client), `sqlite`, `supervisor`, `gen_server`, and `test`.

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

macOS (arm64, x86_64) and Linux (arm64, x86_64). Windows via WSL2; no native port.

## Layout

- `docs/SPEC.md` - language spec, source of truth.
- `compiler/` - self-hosting compiler (lexer, parser, liveness, codegen).
- `runtime/` - C runtime (scheduler, arenas, builtins, vendored deps).
- `std/` - standard library, in Gem.
- `examples/` - numbered tests and larger programs (HTTP server, bookmark app on SQLite).
- `editors/` - VS Code grammar and tree-sitter grammar for Helix.

## Status

Pre-1.0. Memory model, concurrency primitives, and surface syntax are stable. Stdlib APIs may grow but existing ones are unlikely to break.
