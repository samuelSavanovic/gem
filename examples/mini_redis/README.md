# mini_redis

An in-memory key-value server that speaks the Redis protocol (RESP2) and enough of Redis's commands that `redis-cli` and `redis-benchmark` work against it unchanged. One process owns the whole keyspace, each connection gets its own process, and pub/sub fans out to subscriber processes. `benchmarks/mini_redis/` compares it with a real `redis-server`.

## Run it

```sh
build/gem examples/mini_redis/main.gem                  # 127.0.0.1:6379
build/gem examples/mini_redis/main.gem --port 6380 --bind 0.0.0.0
build/gem examples/mini_redis/test.gem                  # the tests (part of make test)
```

```sh
redis-cli -p 6380 set greeting hello
redis-cli -p 6380 get greeting
redis-benchmark -p 6380 -q -t set,get
```

## What it supports

Replies, error messages included, match Redis 7.0 byte for byte for the commands below. `benchmarks/mini_redis/compat.sh` checks 138 commands against a real server.

| Group | Commands |
|---|---|
| Connection | `PING`, `ECHO`, `SELECT 0`, `QUIT`, `SHUTDOWN`, `CONFIG GET save\|appendonly`, `COMMAND` (empty) |
| Keys | `DEL`, `EXISTS`, `TYPE`, `KEYS` (glob `*`, `?`, `[a-z]`, `[^x]`, `\`), `DBSIZE`, `FLUSHDB`, `FLUSHALL` |
| Expiry | `EXPIRE`, `PEXPIRE`, `TTL`, `PTTL`, `PERSIST`, `SET ... EX\|PX\|KEEPTTL` |
| Strings | `GET`, `SET` (`NX`, `XX`, `EX`, `PX`, `KEEPTTL`), `MGET`, `MSET`, `INCR`, `DECR`, `INCRBY`, `DECRBY`, `APPEND`, `STRLEN` |
| Lists | `LPUSH`, `RPUSH`, `LPOP`, `RPOP` (with a count), `LLEN`, `LRANGE`, `LINDEX` |
| Hashes | `HSET`, `HGET`, `HDEL`, `HGETALL`, `HLEN`, `HEXISTS` |
| Sets | `SADD`, `SREM`, `SISMEMBER`, `SMEMBERS`, `SCARD`, `SPOP` (no count) |
| Pub/sub | `SUBSCRIBE`, `UNSUBSCRIBE`, `PUBLISH` |

Commands can be multibulk or inline (`GET k\r\n`, as `nc` sends them), pipelined or not. That covers every `redis-benchmark` test except the sorted-set ones and `XADD`.

There are no sorted sets, streams, transactions, Lua, blocking commands, pattern subscriptions, RESP3, AUTH, databases other than 0, persistence, replication or `SCAN`, no command options added after Redis 6 (`EXPIRE ... NX`, `SET ... GET`), and no quotes in inline commands.

## Design

```
mini_redis_sup (supervisor, one_for_one)
├── mini_redis_store (gen_server)    the keyspace; runs batches of commands
├── mini_redis_pubsub (gen_server)   channel -> subscribers; fans PUBLISH out
└── acceptor                         tcp_accept loop
                                     └── one process per connection
                                         (plus a reader once it subscribes)
```

| File | What it holds |
|---|---|
| `main.gem` | entry point: flags, listen, start |
| `server.gem` | the supervision tree and the accept loop |
| `connection.gem` | one client: parse, batch, reply; pub/sub delivery |
| `store.gem` | the gen_server that owns the keyspace |
| `keyspace.gem` | the data commands and expiry, as functions over a db table |
| `pubsub.gem` | the gen_server that keeps subscriptions |
| `resp.gem` | the RESP parser and encoder |
| `deque.gem`, `rset.gem` | the list and set structures (`rset` is also the expiry index) |
| `test.gem` | unit tests and end-to-end tests with a Gem client |

As with Redis's single thread, one process owns the keyspace, so commands are atomic without locks. A connection sends every command it read in one go as a single `gen_server.call` (a pipeline of 16 is one message each way) and gets back one string holding all the encoded replies, since a string is the cheapest thing to copy between processes. Commands that don't touch the keyspace (`PING`, `ECHO`, `SELECT`, pub/sub) are answered by the connection itself, after it has sent the batch so far, so replies stay in order.

Expiry works the way Redis does it. A key past its deadline is removed when a command reads it, and ten times a second the store samples 20 keys that have a deadline, removes the expired ones, and samples again while more than a quarter were expired, for at most 25 ms. Sampling needs a random key in O(1), which is what `rset` (an array plus an index) is for. Lists are deques (two arrays, the front one reversed), so `LPUSH`, `RPOP` and `LINDEX` are O(1).

A connection normally blocks in `tcp_read`. Once it subscribes it also has to wait for messages, so a linked reader process takes over the socket reads and mails the chunks to the connection, which waits for both in one `receive`. The connection claims the socket, so it gets closed however the connection ends, and a crash of either process kills the other through their link. The pubsub server monitors its subscribers and drops a connection's channels when it gets the `DOWN`.

## Known limits

- A client that doesn't take a reply within 10 s is disconnected. Redis has output-buffer limits instead.
- Messages to a slow subscriber queue in its mailbox until that timeout.
- Memory isn't bounded: no `maxmemory`, no eviction.
- Connections aren't supervised. If the store dies, every connection waiting on it dies too, and the restarted store is empty.
