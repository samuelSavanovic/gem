# STOMP broker

A message broker that speaks a subset of [STOMP 1.2](https://stomp.github.io/stomp-specification-1.2.html):
topics, queues and receipts, with no persistence, transactions or acks.
It is the largest OTP-style program in the repository: a supervision tree,
a gen_server per destination started on demand, and a writer/reader
process pair per connection.

## Run it

```sh
build/gem examples/stomp_broker/main.gem      # listens on port 61613
build/gem examples/stomp_broker/test.gem      # the tests (part of make test)
```

By hand, with `nc` (`\0` ends a frame):

```sh
printf 'CONNECT\naccept-version:1.2\n\n\0SUBSCRIBE\nid:0\ndestination:/topic/news\n\n\0' | nc localhost 61613
# in another terminal:
printf 'CONNECT\naccept-version:1.2\n\n\0SEND\ndestination:/topic/news\n\nhello\0' | nc localhost 61613
```

`benchmarks/stomp/` has the load harness (fan-out, slow consumers, queues).

## What it supports

- `CONNECT`/`STOMP` → `CONNECTED` (version 1.2, heart-beats `0,0`).
  Anything else before it is an `ERROR`.
- `SUBSCRIBE` (`destination`, `id`), `UNSUBSCRIBE` (`id`), `SEND`
  (`destination`; `content-type` is passed on), `DISCONNECT`.
- A `receipt` header on any of those gets a `RECEIPT` once the frame has
  been handled. Since the broker handles a connection's frames in order, a
  receipt for `SUBSCRIBE` means messages sent after it reach the new
  subscription.
- Destinations whose names start with `/queue/` send each message to one
  subscriber, in turn; any other name is a topic, and every subscriber gets
  every message.
- An `ERROR` (malformed frame, missing header, duplicate subscription id,
  unknown command) closes the connection, as the spec says.
- Not supported: `content-length` (so a body can't contain NUL), `ACK`/
  `NACK` (every subscription is `ack:auto`), `BEGIN`/`COMMIT`/`ABORT`,
  heart-beats.

## Design

```
stomp_broker_sup (supervisor, one_for_one)
├── stomp_destinations (dynamic_supervisor) ── one gen_server per destination
├── stomp_registry (gen_server)              ── name → destination pid
└── acceptor                                 ── tcp_accept loop
                                                └── per connection: writer ⇄ reader
```

| File | What it holds |
|---|---|
| `main.gem` | entry point: listens and starts the broker |
| `broker.gem` | the supervision tree and the accept loop |
| `registry.gem` | finds or starts the destination for a name |
| `destination.gem` | a destination's subscribers and delivery (topic or queue) |
| `connection.gem` | the writer and reader processes of one connection |
| `frame.gem` | parse and render frames, header escapes |
| `test.gem` | frame unit tests and end-to-end tests with Gem clients |

Choices worth knowing:

- **Destinations are found through the registry's table, not registered
  names.** Destination names come from clients; registering them would
  let a client subscribe to `stomp_registry` itself. The registry
  monitors each destination and forgets it on its `DOWN`. Destinations
  belong to the dynamic supervisor, not the registry, so a restarted
  registry rebuilds its table from `dynamic_supervisor.which_children`.
- **Destinations are temporary children.** The registry forgets a
  destination on its `DOWN` and starts a fresh one on the next lookup of
  its name; a restart by the supervisor would leave a second process the
  registry doesn't know. Each connection monitors the destinations it
  subscribes to and, on a `DOWN`, subscribes the same ids to the fresh
  one; messages published in between are lost. The dynamic supervisor
  stops the destinations with the rest of the broker (`broker.stop`);
  connections left open then get an `ERROR` and are closed.
- **Destinations monitor their subscribers** and drop a connection's
  subscriptions on its `DOWN`, so a client that disappears without
  `UNSUBSCRIBE` leaves nothing behind.
- **One connection is two processes.** The reader blocks in `tcp_read`
  and sends each parsed frame to the writer. The writer owns the
  protocol state and every write to the socket, and waits in one
  `receive` for both client frames and deliveries from destinations.
  The writer traps exits, so a crashing reader becomes a message, and it
  closes the socket on every path out, a crash included.
- **Connections are not supervised.** A client whose connection dies
  reconnects; there is nothing to restore for it.
- **The reader has no timeout.** With heart-beats off, a subscriber that
  stays quiet for hours is still a live client.

## Known limits

- **A frame is at most 4 MB** (`MAX_FRAME` in `connection.gem`); a
  bigger one gets an `ERROR` and the connection is closed, so a client
  that never ends a frame can't grow the broker's memory without bound.
- **Slow consumers are disconnected.** A client that doesn't take a
  frame within `write_timeout_ms` (10 s by default, a `broker.start`
  option) is dropped, so its messages can't pile up without bound. Until
  then they queue in its writer's mailbox. Dropping messages instead of
  the client would be the other policy (NOTES.md discusses ways to drop
  or throttle).
- **Every `SEND` asks the registry for the destination** (one
  `gen_server.call`), so all publishers go through one process. A
  connection could keep the pids it has looked up, at the cost of
  noticing when a destination dies.
- **Fan-out copies the message once per subscriber** (messages are deep
  copies), so a big body on a topic with many subscribers costs
  `size × subscribers`.

NOTES.md is the build log of the first version and its load tests.
