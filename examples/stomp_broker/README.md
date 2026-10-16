# STOMP broker

A message broker that speaks a subset of [STOMP 1.2](https://stomp.github.io/stomp-specification-1.2.html): topics, queues and receipts, with no persistence, transactions or acks. It is the largest OTP-style program in the repository, with a supervision tree, a gen_server per destination started on demand, and a writer/reader process pair per connection.

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

`benchmarks/stomp/` has a load harness (fan-out, slow consumers, queues).

## What it supports

- `CONNECT` or `STOMP`, answered with `CONNECTED` (version 1.2, heart-beats `0,0`). Any other frame before it gets an `ERROR`.
- `SUBSCRIBE` (`destination`, `id`), `UNSUBSCRIBE` (`id`), `SEND` (`destination`; `content-type` is passed on) and `DISCONNECT`.
- A `receipt` header on any of those gets a `RECEIPT` once the frame has been handled. A connection's frames are handled in order, so after the receipt for a `SUBSCRIBE`, anything sent reaches the new subscription.
- A destination whose name starts with `/queue/` gives each message to one subscriber, in turn. Any other name is a topic, and every subscriber gets every message.
- An `ERROR` (malformed frame, missing header, duplicate subscription id, unknown command) closes the connection, as the spec says.

`content-length` isn't supported, so a body can't contain a NUL. Neither are `ACK`/`NACK` (every subscription is `ack:auto`), `BEGIN`/`COMMIT`/`ABORT` or heart-beats.

## Design

```
stomp_broker_sup (supervisor, one_for_one)
├── stomp_destinations (dynamic_supervisor)  one gen_server per destination
├── stomp_registry (gen_server)              name -> destination pid
└── acceptor                                 tcp_accept loop
                                             └── per connection: writer + reader
```

| File | What it holds |
|---|---|
| `main.gem` | entry point: listens and starts the broker |
| `broker.gem` | the supervision tree and the accept loop |
| `registry.gem` | finds or starts the destination for a name |
| `destination.gem` | a destination's subscribers and delivery (topic or queue) |
| `connection.gem` | the writer and reader processes of one connection |
| `frame.gem` | parsing and rendering frames, header escapes |
| `test.gem` | frame unit tests and end-to-end tests with Gem clients |

Destinations are looked up in the registry's table rather than registered as process names, because their names come from clients and a client could otherwise subscribe to `stomp_registry` itself. The registry monitors each destination and forgets it on its `DOWN`. Destinations are temporary children of the dynamic supervisor, not of the registry, so a restarted registry rebuilds its table from `dynamic_supervisor.which_children`, and a dead destination is replaced by a fresh one on the next lookup of its name. Each connection monitors the destinations it subscribes to and, on a `DOWN`, subscribes the same ids to the new one. Messages published in between are lost.

Destinations monitor their subscribers too, so a client that disappears without `UNSUBSCRIBE` leaves nothing behind.

Each connection is two processes. The reader blocks in `tcp_read` and sends each parsed frame to the writer. The writer owns the protocol state and every write to the socket, and waits in one `receive` for both client frames and deliveries. The writer claims the socket, so it is closed however the writer ends, and a crash of either process kills the other through their link. Connections aren't supervised: a client whose connection dies reconnects, and there is nothing to restore. The reader has no timeout, since with heart-beats off a quiet subscriber is still a live client.

## Known limits

- A frame is at most 4 MB (`MAX_FRAME` in `connection.gem`). A bigger one gets an `ERROR` and the connection is closed.
- A client that doesn't take a frame within `write_timeout_ms` (10 s by default, an option of `broker.start`) is disconnected. Until then its messages queue in its writer's mailbox.
- Every `SEND` asks the registry for the destination with a `gen_server.call`, so all publishers go through one process.
- Fan-out copies the message once per subscriber, so a big body on a topic with many subscribers costs its size times the number of subscribers.
