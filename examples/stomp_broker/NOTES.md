# STOMP broker — builder notes

> These are observations from building the broker. They're written from the
> *coder's* perspective (what tripped me up while wiring it together), not
> from the lived-experience perspective the tutorial asks for in milestone 6.
> The latter — what makes you reach for a feature the language doesn't
> have, what the runtime cost of the actor model felt like under load — is
> still yours to add.

## What worked well

- **OTP primitives carried 80% of the structure for free.** A destination
  is a `gen_server`, so SUBSCRIBE/SEND become `cast` messages and the
  state machine just falls out. Subscriber pruning is one `monitor()`
  call plus a DOWN handler. This is the part of the design that doesn't
  need any cleverness — the standard library has the right shape.

- **Per-connection reader+writer split is the right pattern.** A single
  process can't both block on `tcp_read` and dispatch MESSAGE deliveries
  from destinations through `receive`. Splitting them — reader pipes
  parsed frames to writer via mailbox; writer owns the fd for writes —
  is clean. `spawn_link` from inside the writer pairs them atomically
  (no race window), and Erlang link semantics (normal exits don't
  propagate) means a clean DISCONNECT doesn't cascade.

- **Auto-unregister on death.** Destinations are registered under their
  STOMP path (`/topic/foo`). When a destination crashes, the runtime
  unregisters the name — next SUBSCRIBE on that path goes through the
  registry, which sees `whereis(name) == nil` and lazily re-creates it.
  Free recovery, no supervisor entry needed for the dynamic side.

- **Binary-safe strings + `string.index_of`** handled NUL framing
  cleanly. The broker buffer is just a string; finding `\0` and slicing
  with `substr` works exactly as you'd hope.

## What I had to work around

- **No `..` operator.** I instinctively wrote `buf .. chunk` for string
  concatenation. Gem uses `+` for both arithmetic and string concat (per
  SPEC), and `..` is parsed as field access — which fails at *runtime*
  (`field access on non-table: got string`), not compile time. A
  long-running process silently dies on its first concat. **Worth a
  compile-time hint** if the LHS is statically a string and the field
  access has the lexical shape `<expr>.<expr>` rather than
  `<expr>.<ident>`. Filed mentally as a future LSP/compiler diagnostic.

- **Stdout buffering hides debug `print()` calls.** When the broker is
  run with `> /tmp/stomp.log` and later SIGKILLed, *nothing* gets
  written — not even the startup banner. `print()` doesn't flush stdout
  (it just `printf`s and a newline). I had to swap in `write_stdout`
  (which does flush) for any debug output during a long-running
  process. Worked around it without runtime changes, but `print()`
  flushing on every call would have been the right default for a
  language that's actor-shaped — long-running processes that *never*
  exit cleanly are normal here.

- **Static supervisor doesn't match dynamic destinations.** `std/supervisor`
  takes a static `children` array — fine for the registry and the
  acceptor, but destinations are spawned on demand. Erlang has
  `simple_one_for_one` / `DynamicSupervisor` for this; Gem doesn't. The
  workaround is reasonable (registry serializes lookup-or-create,
  destination crashes are absorbed by auto-unregister + lazy
  recreation), but it means destination crashes lose subscribers
  instead of resurrecting state — fine for STOMP-without-persistence,
  not fine for everything. If Gem grows a dynamic supervisor, this is
  the kind of workload it would unlock.

- **`tcp_read` returns `""` on both EOF and timeout.** I didn't end up
  using the timeout form — the reader/writer split means the reader
  blocks indefinitely and only returns `""` on real EOF. But if you
  *did* try to single-process the connection with a `tcp_read` timeout
  to interleave wire reads with MESSAGE deliveries, you'd find that you
  can't tell EOF from "no data yet." A separate return code (or a
  return value like `{kind: "eof"}` vs `{kind: "timeout"}`) would have
  been a cleaner API. The two-process workaround sidesteps this, so
  it's not blocking, but it pushed a design choice.

- **Field assignment to `state.mode = "connected"` works** but in a
  long-recursive writer loop it relies on the table being mutated in
  place (rather than treated as immutable + rebuilt). Gem tables are
  mutable so this is fine, but if you're coming from Elixir/Erlang
  where state is rebuilt-and-passed every iteration, the muscle memory
  fights you a little. No real workaround needed; just a flavor note.

## What I didn't try (and why)

- **Heart-beats.** Strip-only handling: the reader skips leading `\n`/`\r`
  bytes between frames, which absorbs incoming heart-beats. Outgoing
  heart-beats (broker → client) aren't implemented — `CONNECTED` always
  advertises `0,0`. Adding outgoing heart-beats requires a per-connection
  timer that interleaves with `receive`; doable with `send_after`, but it's
  a bunch of state-machine plumbing for a feature most clients tolerate
  being absent.

- **`content-length` body framing.** Bodies can contain raw `\0` bytes
  if the client sends a `content-length` header — the parser is
  supposed to read exactly that many bytes and then expect a single
  trailing `\0`. I just terminate at the first `\0`, which is correct
  for the common case and wrong for binary payloads with embedded
  NULs. Documented as a v0 limit.

- **`receipt` headers on SEND/SUBSCRIBE.** Only `DISCONNECT` honors
  `receipt` (returns a `RECEIPT` frame). Other commands ignore it.
  Trivially extensible — the writer just needs to render a RECEIPT
  after each handled frame whose request had `receipt`.

- **Backpressure / slow-consumer handling.** A destination just `send`s
  MESSAGE frames to all subscribers. `send` is non-blocking; mailboxes
  grow without bound. A slow consumer with a fast publisher = OOM. This
  is the milestone-6 question the tutorial flags, and it's the right
  thing to leave open — the *answer* (selective receive with mailbox
  size limits, or push-back via `gen_server.call` instead of `cast`,
  or per-subscriber drop policy) is a real design decision, not a
  one-line fix. **This is the most interesting thing left to find out.**

- **Load test under the workloads in milestone 6.** I ran a 4-test smoke
  pass (fan-out, queue round-robin, garbage-frame survival, ghost-sub
  pruning), all green. I did not run the 1k-fanout or slow-consumer
  workloads — those are where the actor model's deep-copy fan-out cost
  and the mailbox-growth question would actually show up, and the
  tutorial is right that *those numbers* are the artifact worth keeping.

## File layout

```
main.gem         # entry: tcp_listen, broker.start, sleep loop
broker.gem       # supervision tree (registry + acceptor)
connection.gem   # reader + writer pair, state machine, frame dispatch
destination.gem  # gen_server per topic/queue
registry.gem    # gen_server: lookup-or-create destinations
frame.gem        # parse_frame, render_frame, header escape/unescape
test_frame.gem   # unit tests for the parser
smoke_test.sh    # end-to-end: fan-out, round-robin, adversarial
```

## How to run

```sh
# In one terminal:
build/gem examples/stomp_broker/main.gem

# In another:
bash examples/stomp_broker/smoke_test.sh
```

Or hand-test:

```sh
printf 'CONNECT\naccept-version:1.2\nhost:localhost\n\n\0' | nc localhost 61613
```
