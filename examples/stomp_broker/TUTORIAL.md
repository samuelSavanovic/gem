# Building a STOMP Broker in Gem — Tutorial

A milestone-by-milestone scope for building a minimal message broker in Gem. Each
milestone is independently runnable so you can demo progress and feel the language
push back at the edges.

The goal is not just a broker — it's a real-world workload that exercises OTP-style
concurrency end-to-end. The most valuable artifact at the end is the writeup of
what bent.

## What you're building

A broker that speaks a subset of [STOMP 1.2](https://stomp.github.io/stomp-specification-1.2.html) —
text frames, easy to test with `nc` or a Python client. Final shape: clients connect
over TCP, `SUBSCRIBE` to destinations, `SEND` messages, and the broker fans out to
subscribers. No persistence, no transactions, no acks beyond auto.

**Frame format** (this is the whole wire protocol):

```
COMMAND\n
header1:value1\n
header2:value2\n
\n
body\0
```

Commands you need: `CONNECT`/`CONNECTED`, `SUBSCRIBE`, `UNSUBSCRIBE`, `SEND`,
`MESSAGE`, `DISCONNECT`, `ERROR`.

---

## Milestone 1 — Echo server with frame parsing

**Goal**: accept a TCP connection, parse one STOMP frame, echo it back as text.

**Concepts**: `tcp_listen` / `tcp_accept` / `tcp_read` / `tcp_write`, one process per
connection (`spawn` inside the accept loop), and a frame parser.

**Things to figure out**:

- `tcp_read` returns whatever's available — frames can arrive split across reads, or
  multiple frames in one read. You'll need a per-connection read buffer that you keep
  appending to until you see `\0`.
- Parse into a table like `{command: "SEND", headers: {destination: "/queue/foo"}, body: "hi"}`.
  Headers are `key:value` lines; body is everything after the blank line up to `\0`.
- Don't skip header unescaping yet (`\n`, `\c`, `\\`, `\r`) — note it as a TODO and
  revisit when you do CONNECT properly.

**Gotcha worth watching**: your accept loop is a perfect candidate for the per-iteration
arena reset (see CLAUDE.md "Key Decisions"). If the compiler emits a warning about it,
that's a real signal — chase it down before you're 5 milestones deep with a leaky loop.

**Test with**:

```sh
printf 'CONNECT\naccept-version:1.2\n\n\0' | nc localhost 61613
```

### Pseudocode

The accept loop and per-connection process:

```gem
# main.gem
load "std/string"

fn main()
  let listener = tcp_listen("0.0.0.0", 61613)
  print("listening on 61613")
  while true
    let client = tcp_accept(listener)
    spawn do
      handle_connection(client)
    end
  end
end

fn handle_connection(fd)
  let buf = ""                          # accumulator
  while true
    let chunk = tcp_read(fd, 4096)
    if chunk == nil or len(chunk) == 0
      tcp_close(fd)
      return nil
    end
    buf = buf .. chunk                  # or buf_push if you switch to buf_*

    # drain as many complete frames as we have
    while true
      let nul = string.index_of(buf, "\0")
      if nul == nil then break end       # need more bytes

      let raw = substr(buf, 0, nul)      # frame WITHOUT trailing \0
      buf = substr(buf, nul + 1, len(buf))

      let frame = parse_frame(raw)
      if frame == nil
        # malformed — write ERROR and bail (milestone 2 does this properly)
        tcp_write(fd, "ERROR\nmessage:bad-frame\n\n\0")
        tcp_close(fd)
        return nil
      end

      # echo back as text — milestone 1 only
      tcp_write(fd, render_frame(frame))
    end
  end
end

main()
```

The parser — split on the blank line, then split the head into command + headers:

```gem
# frame.gem
load "std/string"

fn parse_frame(raw)
  # raw = "COMMAND\nh1:v1\nh2:v2\n\nbody"   (no trailing \0)
  let sep = string.index_of(raw, "\n\n")
  if sep == nil then return nil end

  let head = substr(raw, 0, sep)
  let body = substr(raw, sep + 2, len(raw))

  let lines = string.split(head, "\n")
  if len(lines) == 0 then return nil end

  let command = lines[0]
  let headers = {}
  let i = 1
  while i < len(lines)
    let line = lines[i]
    let colon = string.index_of(line, ":")
    if colon == nil then return nil end
    let key = substr(line, 0, colon)
    let val = substr(line, colon + 1, len(line))
    # TODO: STOMP header unescaping (\n, \c, \\, \r) — defer to milestone 2
    headers[key] = val
    i = i + 1
  end

  {command: command, headers: headers, body: body}
end

fn render_frame(frame)
  # build_string wants () before do — see CLAUDE.md quick reference
  build_string() do |add|
    add(frame.command, "\n")
    for k, v in frame.headers
      add(k, ":", v, "\n")
    end
    add("\n", frame.body, "\0")
  end
end

export parse_frame, render_frame
```

**What to verify before moving on**:

1. `printf 'CONNECT\naccept-version:1.2\n\n\0' | nc localhost 61613` → echoes back.
2. Split frame across two writes (use a script with a sleep mid-frame) → still parses.
3. Two frames in one write → both parse, in order.
4. Garbage bytes → server stays up, that connection dies, others unaffected.

---

## Milestone 2 — CONNECT handshake + connection process

**Goal**: respond to `CONNECT` with `CONNECTED`, reject anything else until connected,
handle `DISCONNECT` cleanly.

**Concepts**: per-connection state machine (just two states: `awaiting_connect` and
`connected`), structured frame writer (the inverse of your parser).

**Things to figure out**:

- Spawn a connection process per accepted socket. Its loop: read → parse → handle →
  maybe write → repeat.
- `link` the connection process to the socket cleanup. When the process dies (crash
  or DISCONNECT), close the fd.
- Give every connection a unique id (counter or pid string). You'll need it in
  milestone 3.
- Now is also the time to do header unescaping properly.

**Adversarial tests to write now** (before you have subscribers to lose):

- Client sends garbage bytes
- Client disconnects mid-frame
- Two frames arrive in one `tcp_read`
- Client sends `SUBSCRIBE` before `CONNECT` → server returns `ERROR` and closes

---

## Milestone 3 — Destinations as gen_servers

**Goal**: `SUBSCRIBE` registers interest in a destination; `SEND` to that destination
delivers `MESSAGE` frames to all current subscribers.

**Concepts**: this is where OTP earns its keep. A destination is a long-lived process
that owns `{subscribers: [{conn_pid, sub_id}]}`. Use `std/gen_server`.

**Architecture decision** (worth thinking through before coding):

- Lazy creation: destination process is spawned the first time someone subscribes or
  sends to it. Use `register` / `whereis` keyed on the destination string
  (`"/queue/foo"`).
- Connection process talks to destination with `send`; destination `send`s `MESSAGE`
  frames back to connection processes, which serialize and write to the socket.
- *Why route writes through the connection process*: the socket fd is owned by one
  process, and TCP writes from multiple processes would interleave. The destination
  just hands the frame off; the connection owns the wire.

**Gotcha**: `monitor` the connection process from the destination so you can prune
dead subscribers. Otherwise a crashed client leaks subscriber entries forever.

**Demo it**: two `nc` sessions subscribed to `/queue/test`, third session sends → both
receive.

---

## Milestone 4 — Supervision

**Goal**: a connection crash takes down only that connection; a destination crash
recovers without losing other destinations.

**Concepts**: `std/supervisor` with appropriate restart strategies. A top-level
supervisor for the broker; one supervisor for connections (transient — don't restart,
the client will reconnect), one for destinations (permanent — restart and re-register).

**Things to figure out**:

- What's the right thing to do if a destination crashes mid-fan-out? Subscribers are
  lost. Is that acceptable for v0? (Yes — STOMP without persistence promises nothing.)
- Inject a crash on purpose (`error("boom")` on a specific message body) and confirm
  the rest of the system stays up.

---

## Milestone 5 — Topics vs queues

**Goal**: `/topic/*` fan-outs to all subscribers (current behavior); `/queue/*`
round-robins to one subscriber.

**Concepts**: minor extension to the destination process — track an index for queues,
advance on each delivery.

This milestone is small but it's where you'll first feel the **deep-copy fan-out cost**
if you load-test topics with many subscribers. Note it; don't fix it.

---

## Milestone 6 — Load test and write up the boundaries

**Goal**: not more features. Stress what you have and document what bent.

**Suggested workloads**:

- 1 publisher, 1000 subscribers on a topic, small messages — measures fan-out /
  deep-copy cost.
- 100 publishers, 100 queues, 1 consumer each — measures scheduler under many active
  processes.
- 1 slow consumer (introduce `sleep` in the connection write loop) on a topic with a
  fast publisher — does the destination's mailbox grow without bound? This is the
  backpressure question.

**Write up** (a markdown file in the example dir): what you measured, what surprised
you, what you'd want from the language/runtime to do better. *This is the actual
output of the project* — it's what makes the broker valuable beyond "another example."

---

## Suggested layout

```
examples/stomp_broker/
  main.gem              # entry: starts top supervisor, listens on 61613
  frame.gem             # parse_frame, render_frame
  connection.gem        # per-connection process loop
  destination.gem       # gen_server for a destination
  broker.gem            # supervisor tree, destination registry
  README.md             # how to run + what worked / didn't
```

---

## A few ground rules to hold yourself to

1. **Get to milestone 3 end-to-end before polishing milestone 1.** Vertical slices
   reveal model-level pain that horizontal completeness hides.
2. **When you hit something awkward, write it down before working around it.** A list
   of "I had to do X because Y wasn't possible" is the most valuable artifact this
   project produces.
3. **Don't add a builtin to make broker code prettier.** If the broker wants something
   the language doesn't have, that's *the signal*. Note it; build the broker on what
   exists.
