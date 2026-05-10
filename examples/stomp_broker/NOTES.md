# STOMP broker — builder notes

> Builder notes from the M1–M5 work, then a "Milestone 6: lived experience"
> section at the bottom with the load-test numbers and the design
> questions they surface.

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
  acceptor, but destinations are spawned on demand. Gem now has
  `std/dynamic_supervisor` (added in PR #20, mirrors Erlang
  `simple_one_for_one`), but it's the wrong shape here: it restarts
  by replaying `template.start(args)` with the original args. For
  destinations we want **named, on-demand, crash-loses-state** —
  the registry-as-lazy-factory pattern (whereis / lookup-or-create)
  matches that exactly. A dynamic supervisor would buy us automatic
  re-creation, but at the cost of either (a) needing a per-name
  child template lookup, or (b) restarting under a fresh anonymous
  pid that the existing subscribers wouldn't know to re-register
  with. Neither is an improvement on the current registry. Filed as
  a "considered and rejected" design note rather than a missing
  feature.

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

---

## Milestone 6: lived experience

The harness is in `benchmarks/stomp/`:

- `harness.py` — Python 3.14, four subcommands: `fanout`, `slow`, `queue`,
  `soak`, plus `microbench_fanout_one.gem` (deep-copy isolation, no TCP).
- `run.sh` — driver that boots a fresh broker per phase, samples the broker's
  RSS, runs the harness from a separate process, and writes the JSON output
  + RSS CSV under `benchmarks/stomp/logs/<run-id>/`.
- `sweep.sh` — fanout cell sweep, used to find the cliff.

> Hardware reference: Apple Silicon (M-series) macOS, ARM64. Numbers below
> are from a single host running broker and harness on the same machine,
> so absolute throughput is loopback-bound. The interesting signal is the
> *shape* of the curves — where the cliffs are, not the peak Mbit/s.

### TL;DR

What I expected the load tests to expose was the **deep-copy fan-out
cost** (each subscriber gets its own copy on `send`) and the **mailbox
unboundedness** (slow consumer + fast publisher → OOM). What I actually
ran into first, before either of those could land, was a **silent
broker death after exactly 175 frames per connection writer**. That's
not a runtime/memory issue — it's the connection's `writer_loop ↔
handle_frame` mutual tail-recursion (the writer's own state machine,
not `gen_server.loop`) consuming ~1.4 KB of C stack per dispatched
frame, hitting minicoro's 256 KB ceiling, and crashing the coroutine
in a way that takes the broker down with it.

So the artifact is two-tier: numbers from the regime where the broker
survives (5k–25k delivered messages per topic), and a clear
description of the cliff that prevents going further.

### Findings

**Fanout works inside the cliff.** Sustained 25k MESSAGE-frame deliveries
at 50–55k msg/s, body sizes 256B–1KB, with median first-message
fanout latency of 10–20 ms and p100 of 35–45 ms. Cells that worked:

| n_subs | n_msgs | body | delivered | publish msg/s | fanout msg/s | p100 first-msg |
|-------:|-------:|-----:|----------:|--------------:|-------------:|---------------:|
|    100 |     5  |  256 |       500 |          34k  |        4.9k  |          5 ms  |
|    500 |    10  |  256 |     5,000 |          54k  |          47k |         42 ms  |
|    500 |    20  |  256 |    10,000 |          76k  |          44k |         20 ms  |
|    500 |    50  |  256 |    25,000 |          89k  |          54k |         45 ms  |
|    500 |    50  | 1024 |    25,000 |         106k  |          54k |         37 ms  |
|    100 |   100  |  256 |    10,000 |         141k  |          46k |          4 ms  |
|    200 |   100  |  256 |    20,000 |          64k  |          59k |         27 ms  |

(Publisher rates are misleading — Python is just stuffing frames into the
kernel TCP buffer in a millisecond. Treat them as "publisher is much faster
than the broker" rather than as a steady-state rate.)

**The cliff: 175 frames per writer.** Crossing the threshold causes minicoro
to print `coroutine stack overflow, try increasing the stack size` to stderr
once per affected coroutine; the broker process then dies (exit 139,
SIGSEGV) because the stack overflow corrupts whatever was adjacent.
The exact iteration is *deterministic*:

| workload                 | publishes before death | deliveries before death |
|--------------------------|-----------------------:|------------------------:|
| 1 pub × 200 to /queue (4 wkr)  |                  175 |                     175 |
| 2 pubs × 1000 to /queue (4 wkr)|                  175 |                     175 |
| 100 subs × 200 publishes       |                  175 |                  17,499 |
| 50 subs × 500 publishes        |                  175 |                   8,749 |
| 10 subs × 1000 publishes       |                  175 |                   1,749 |
| 500 subs × 100 publishes       |                   54 |                  26,999 |

The 500-sub cell is the odd one out — it dies sooner because the *destination's*
fan-out for-loop pushes its own connection-writer past the cliff faster (each
publish queues 500 deliveries; per-writer cliff is still ~175 deliveries, so
~54 publishes × 500 = ~27k get out before the first writer crashes).

**Root cause, isolated.** A bare `gen_server` (no broker, no TCP) handles
500 casts cleanly with no overflow — see
`benchmarks/stomp/microbench_gen_server.gem`. So `gen_server.loop`'s self-tail
recursion *is* being TCO'd. The cliff is in `connection.gem` itself: the
writer's loop is

```gem
fn writer_loop(state)
  receive
  when {tag: "frame", frame: f}
    handle_frame(state, f)        # last expression of the arm
  when {tag: "deliver", frame: f}
    write_frame(state.fd, f)
    writer_loop(state)            # self-tail: TCO'd → continue
  end
end

fn handle_frame(state, f)
  if state.mode == "awaiting_connect" ...
  else
    if f.command == "SUBSCRIBE"
      handle_subscribe(state, f)  # which ends in writer_loop(state)
    elif f.command == "SEND"
      handle_send(state, f)       # which ends in writer_loop(state)
    ...
  end
end
```

`writer_loop → handle_frame → handle_send → writer_loop` is mutual
tail-recursion through three functions. Per `docs/ROADMAP.md` ("Deep
non-tail recursion ceiling, P2") the compiler only TCO's *direct*
self-recursive tail calls — mutual tail-call cycles are not collapsed. So
each round-trip eats ~1.4 KB of stack, the 256 KB minicoro stack fills
after ~175 frames, and the coroutine smashes through into adjacent
memory.

The `{tag: "deliver"}` branch is fine — it's a direct self-tail call
on `writer_loop`, so it TCO's. That's why the destination's fan-out
loop produces deliveries that work for a while: deliveries themselves
don't grow the writer's stack. It's the *publisher's* writer (or any
connection that issues many SEND/SUBSCRIBE/UNSUBSCRIBE frames) that
overflows first. That writer dies, the publisher's `tcp_write` sees a
broken pipe, and because the overflow corrupts adjacent memory, the
broker's other coroutines start stepping on each other and the
process exits soon after.

**This is the most important milestone-6 finding.** It dominates
everything else. Until it's fixed, no STOMP-shaped service in Gem can
handle more than ~175 commands per connection without dying. The
feature the language is missing is **mutual-tail-call elimination**
(or growable coroutine stacks, which would make the issue irrelevant
without needing a new analysis). The `gen_server` and `supervisor`
patterns happened to be written as direct self-recursion, so they
shipped without exposing this; broker-shaped code (where one process
is a state machine that dispatches via helpers and loops back) hits
it immediately.

**Backpressure: confirmed absent, observable, but bounded by the cliff.**
Slow-consumer test: 4 fast subs + 1 slow sub (Python doing
`sleep(0.5)` after each MESSAGE), 4 KB body, 150 publishes total
(below the cliff so the broker survives the test). Result: fast subs
each got 150/150, slow sub got 63/150, 87 messages stuck in the slow
sub's writer mailbox by end-of-run. The publisher fired all 150
SENDs in 3 ms — kernel buffer absorbed them — and there was no
backpressure signal of any kind. RSS stayed flat at 32 MB throughout
because 87 × ~5 KB = ~430 KB is below the per-process arena's first
block (64 KB initial, doubles up to 1 MB) overhead floor. Push the
test harder and the broker hits the *frame-count* cliff before the
*memory* cliff, so the OOM scenario the milestone asks about is
masked by the recursion ceiling.

**Queue fairness is solid.** With 4 workers and small enough volume
to stay below the cliff: 4 publishers × 100 messages → 4 × 100
deliveries, every worker receives exactly 100. With 2 publishers ×
1000 messages (above the cliff): broker dies at 175 deliveries, but
the distribution at the moment of death is 44/44/44/43 — fairness
ratio 0.977. Round-robin holds under load right up to the death
point.

**Deep-copy fan-out cost (microbench, no TCP).**
`microbench_fanout_one.gem` spawns K simple drain processes and has
the parent fan out 200 frames to all of them. Each `send()` does a
deep-copy into the target's arena. Numbers (per-send cost is the
slope; per-msg cost = `K × per_send`):

| K   | body=64 µs/send | body=256 | body=1024 | body=4096 | µs per N-fanout msg @ body=1024 |
|----:|----------------:|---------:|----------:|----------:|--------------------------------:|
|  10 |               3 |        2 |         2 |         3 |                              20 |
|  50 |               2 |        2 |         2 |         2 |                             115 |
| 100 |               2 |        2 |         2 |         2 |                             225 |
| 200 |               2 |        2 |         2 |         2 |                             465 |
| 500 |               2 |        2 |         2 |         2 |                            1230 |
| 800 |               2 |        2 |         2 |         3 |                            2000 |

Two takeaways:

- Per-`send` cost is ~2 µs and **flat** across body sizes 64–1024 B.
  Most of the deep-copy cost is the *table structure* (the
  `{command, headers, body}` outer frame plus the 3-entry headers
  table) — the body string itself is ~1 KB of memcpy and that's
  cheap. So small-message workloads pay full per-frame overhead.
- The 4096 B body adds about 1 µs at K=800. So body bytes do start
  to show, but only in the high hundreds of subscribers regime.

Implication for the broker: at K=1000 a single MESSAGE costs
~2.5 ms of destination CPU to fan out (1000 × 2.5 µs). That sets
a hard upper bound of ~400 publishes/sec sustained at 1k subs *if*
the broker could survive that long. Actually doing 1k topic subs
also requires bumping `GEM_MAX_PROCS` from 1024 (each TCP connection
is reader+writer = 2 procs, so 500 conns ≈ ceiling).

### What this surfaces (design questions, not commitments)

The interesting observation isn't that backpressure is missing —
that was always going to be the case for v0 — but that the broker
hit a *language-shaped* cliff before the actor-shaped cliff (slow
consumer mailbox → OOM). The recursion ceiling is the language
saying "I can't run your event loop the way you wrote it." The
backpressure question only becomes interesting once that's fixed.

When it is fixed, the backpressure design space is roughly:

1. **Selective receive + per-writer mailbox cap.** Writer drops
   `{tag: "deliver", ...}` from its mailbox if the queue exceeds N,
   either by length or by a soft byte budget. Cheapest, but loses
   messages silently — not great for at-most-once → at-least-once
   semantics.
2. **`gen_server.call` instead of `cast` for SEND fan-out.** The
   destination synchronously waits for each writer to ack the
   delivery before moving to the next subscriber. Slowest sub
   throttles the whole fan-out, which is *real* backpressure but
   degrades a topic-fanout system to whatever the slowest subscriber
   can do — wrong default for STOMP.
3. **Per-subscriber drop policy with a lossy-or-blocking flag at
   subscribe time.** STOMP doesn't have this header in 1.2; would
   need to invent one. But it puts the choice on the client (which
   knows whether dropping is acceptable for its use case) rather
   than on the broker.
4. **Writer-side observability into mailbox depth.** A `process_info`
   query on the writer's pid that reports `mailbox_len` would let
   the destination skip slow subscribers proactively. The runtime
   already exposes `mailbox_len` via the `process_info(pid)` builtin
   (see `runtime/gem_scheduler.c:1148`), so this is a few lines of
   destination logic away.

My current preference, if forced to pick, would be **#3** (let the
client declare its tolerance) plus **#4** as the mechanism. But this
is the next-next session — first the recursion cliff has to go, then
backpressure becomes worth designing, because right now the broker
dies before it can demonstrate the OOM the design would prevent.

### Things I'd want from the language/runtime

- **Mutual-tail-call elimination.** The recursion cliff is the
  single most disruptive thing I hit. Per the roadmap this is
  classified P2 ("no user has hit this yet"); this writeup is the
  first user. The fix is either compiler-side (extend the
  GFP-based TCO analysis from self-recursive cycles to mutual
  cycles) or runtime-side (growable / mmap'd coroutine stacks
  with guard pages). Either would unblock everything else.
- **Surfaceable stack-overflow as a Gem error.** The current
  failure mode is `MCO_LOG` to stderr and undefined behavior — the
  process dies somewhere downstream of a corrupted coroutine. A
  guard-page-driven `error("stack overflow in process N")` would at
  least let `pcall`-wrapped code recover, and would point at the
  offending function rather than presenting as "broker silently
  vanished." Listed in `ROADMAP.md` as the middle option for the
  recursion ceiling — would be a real improvement.
- **`process_info(pid).mailbox_len`** is the introspection a
  destination needs for design option #4 above. It already exists
  (cheatsheet line 99, spec §process model). The broker code didn't
  reach for it because pruning ghost subscribers via `monitor` /
  DOWN was sufficient — but the moment backpressure becomes a
  real concern, this is the hook.

### How to reproduce

```sh
# All numbers in this section are reproducible from the driver:
bash benchmarks/stomp/run.sh                  # default: fanout, slow, queue
PHASES=fanout_small bash benchmarks/stomp/run.sh   # cheap smoke test
bash benchmarks/stomp/sweep.sh                # cell sweep that shows the cliff
build/gem benchmarks/stomp/microbench_fanout_one.gem 500 1024 200
build/gem benchmarks/stomp/microbench_gen_server.gem
```

Logs land under `benchmarks/stomp/logs/<run-id>/` (one
subdir per invocation; `meta.txt`, per-phase JSON, per-phase
broker stdout, per-phase RSS CSV).

---

## Milestone 6, second pass: with mutual TCO

The compiler now eliminates tail calls across functions in the same SCC,
not just direct self-recursion (see `compiler/codegen.gem` §"Mutual-tail-
call SCC detection"). The broker's `writer_loop ↔ handle_frame ↔
handle_<command>` 7-function cycle survived viability filtering and is
emitted as `gem_fn_<name>_body` + thin trampoline-loop wrappers. Re-ran
the sweep and the cells where the *previous* cliff was the limiting
factor jumped from "DEAD at delivered ≈ N×175" to "fully delivered."
Cells that hit a *different* failure mode (memory or scheduler-shape
saturation) still fall over, but later and with a different signature.

### What changed

| cell                  | old result                | new result            |
|-----------------------|---------------------------|-----------------------|
| 500 subs × 10 msgs    | 5,000 ✓                  | 5,000 ✓              |
| 500 subs × 20 msgs    | 10,000 ✓                 | 10,000 ✓             |
| 500 subs × 50 msgs    | 25,000 ✓                 | 25,000 ✓             |
| 500 subs × 50 msgs (1KB body) | 25,000 ✓         | 25,000 ✓             |
| **100 subs × 200 msgs** | DEAD at 17,499         | **20,000 ✓**          |
| **50 subs × 500 msgs** | DEAD at 8,749           | **25,000 ✓**          |
| **50 subs × 500 msgs (1KB)** | DEAD                | **25,000 ✓**          |
| **200 subs × 100 msgs** | DEAD                   | **20,000 ✓**          |
| 500 subs × 100 msgs   | DEAD at 26,999            | DEAD at 26,999        |
| 10 subs × 1000 msgs   | DEAD at 1,749             | DEAD at 5,439         |

The only cells that still die are ones where the death point was *not*
the writer-cycle ceiling. The 500×100 cell still bottoms out at
`delivered=26999/50000` — same number as before. The 10×1000 cell now
gets to 5,439 (3× further) but still bottoms out before completion.
Both fail silently: no `coroutine stack overflow` line in the broker
log, no Gem error, no stack trace. The broker process simply exits
during the run and `kill -0 $bpid` from the sweep harness reports
"DIED" after the harness gives up.

### What the *new* cliff is

Two suspects, both anticipated by the original M6 writeup:

1. **Mailbox unboundedness.** When publish rate >> consumer rate,
   each subscriber's writer process accumulates `{tag: "deliver", ...}`
   frames in its mailbox without bound. With 5 KB per frame (256 B body
   + table + headers) and 50,000 deliveries pending, that's ~250 MB
   of resident state across the broker — single-process budgets are
   fine, but each subscriber's writer arena grows past its 1 MB
   reset-threshold too fast for the per-iteration reset to keep up.
2. **Process-table pressure.** 500 subscribers + 1 publisher = 501
   connections × 2 procs each = 1002 procs, plus destinations,
   acceptor, registry, broker supervisor → ~1010 procs against
   `GEM_MAX_PROCS = 1024`. Headroom is thin; a transient spike of
   short-lived processes (a connection retry, an exit-trap closure)
   could push past the cap.

The test that would distinguish these — slow consumer + fast publisher
holding for 60s+ to actually OOM — wasn't run this pass. With the
cliff lifted, it's the right next experiment.

### Updated "things I'd want from the language/runtime"

- ~~**Mutual-tail-call elimination.**~~ Shipped. The original cliff is gone.
- **Surfaceable stack-overflow as a Gem error.** Still wanted. The new
  failure mode ("broker silently dies") is exactly the symptom this
  would diagnose. If we'd had it during this pass, we'd already know
  whether the second-pass deaths are stack-related or not.
- **Mailbox cap with a `gen_server.cast` overflow signal.** Now that
  mutual TCO doesn't mask it, the slow-consumer mailbox-growth scenario
  is the next-most-disruptive language gap. Per-subscriber drop policy
  (option #3 in the original writeup) plus mailbox introspection
  (option #4) is still my preferred shape.

### How to reproduce the second pass

```sh
bash benchmarks/stomp/sweep.sh   # same script; numbers above are the run output
```

The compiler change is in `compiler/codegen.gem`
`find_tail_call_sccs` (Tarjan over fn_def→fn_def tail edges) +
`scc_wrapper_for` (trampoline). The runtime side is the
`gem_tail_fn` / `gem_tail_args` TLB in `runtime/gem.h` and
`runtime/gem_error.c`. Direct self-tail recursion still uses the
existing `while(1) { ... continue; }` shape in the body — only
*cross*-fn intra-SCC tail calls go through the trampoline.
