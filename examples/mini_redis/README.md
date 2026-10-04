# mini_redis

An in-memory key-value server that speaks the Redis protocol (RESP2) and a
subset of Redis's commands, close enough that `redis-cli` and
`redis-benchmark` work against it unchanged. It is the yardstick for
long-lived processes holding large, changing state: one process owns the
whole keyspace, a process per connection, and pub/sub fan-out.
`benchmarks/mini_redis/` runs it against a real `redis-server`.

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

Replies, error messages included, match Redis 7.0 byte for byte for
everything below; `benchmarks/mini_redis/compat.sh` checks 138 commands
against a real server.

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

Commands are multibulk or inline (`GET k\r\n`, as `nc` sends them),
pipelined or not. That covers every `redis-benchmark` test except the
sorted-set ones (`ZADD`, `ZPOPMIN`).

Not supported: sorted sets, streams, transactions (`MULTI`), Lua,
blocking commands (`BLPOP`), pattern subscriptions, RESP3 (`HELLO`), AUTH,
databases other than 0, persistence, replication, `SCAN`, command options
added after Redis 6 (`EXPIRE ... NX`, `SET ... GET`), and quotes in inline
commands.

## Design

```
mini_redis_sup (supervisor, one_for_one)
├── mini_redis_store (gen_server)    ── the keyspace; runs batches of commands
├── mini_redis_pubsub (gen_server)   ── channel → subscribers; fans PUBLISH out
└── acceptor                         ── tcp_accept loop
                                        └── per connection: one process
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
| `deque.gem`, `rset.gem` | the list and set structures (also the expiry index) |
| `test.gem` | unit tests and end-to-end tests with a Gem client |

Choices worth knowing:

- **One process owns the keyspace**, as Redis's single thread does, so
  commands are atomic with no locks. A connection sends it every command
  of a read in one `gen_server.call` (a pipeline of 16 is one message each
  way) and gets back one string with all the encoded replies. The store
  encodes them because a string is the cheapest thing to copy between
  processes.
- **Commands that don't touch the keyspace** (`PING`, `ECHO`, `SELECT`,
  the pub/sub ones) are answered by the connection. The connection sends
  the batch so far first, so replies stay in order.
- **Expiry works as Redis's does**: a key past its deadline is removed
  when a command reads it, and 10 times a second the store samples 20
  keys with a deadline, removes the expired ones, and samples again while
  more than a quarter were expired, for at most 25 ms. The sampling needs
  a random key in O(1), hence `rset` (an array plus an index).
- **Lists are deques** (two arrays, the front one reversed), so `LPUSH`,
  `RPOP` and `LINDEX` are O(1). A Gem array with `insert(arr, 0, x)` would
  make `LPUSH` O(n).
- **A subscribed connection is two processes.** A connection normally
  blocks in `tcp_read`. Once it subscribes it must also wait for
  messages, so a linked reader process takes over the socket reads and
  mails the chunks to the connection, which waits in one `receive` for
  both. The connection traps exits and closes the socket on every path.
- **The pubsub server monitors its subscribers** and forgets a
  connection's channels on its `DOWN`.

## Known limits

- **A slow client is disconnected** when it doesn't take a reply within
  10 s (`write_timeout_ms`), where Redis has output-buffer limits.
- **Messages to a slow subscriber queue in its mailbox** until that
  timeout.
- **Memory is not bounded**: no `maxmemory`, no eviction.
- **Connections aren't supervised.** If the store dies, every connection
  waiting on it dies too, and the restarted store is empty.
