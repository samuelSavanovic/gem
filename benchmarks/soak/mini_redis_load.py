#!/usr/bin/env python3
"""Steady mixed load on examples/mini_redis for a soak run.

Threads, all paced (the point is a long steady load, not peak throughput):

  data clients   each owns a slice of the keyspace and keeps a model of it:
                 strings (GET/SET/DEL), counters (INCR), keys with a 1-5 s
                 TTL (SET EX), a list used as a FIFO (LPUSH/RPOP), a hash
                 and a set. Every reply is checked against the model, so a
                 wrong value is an error, not just a slow one. One request in
                 ten is a pipelined batch of 16.
  churn          opens a connection, SET/GET/DEL on a key of its own, closes
                 it (QUIT or an abrupt close): process and fd reuse.
  publisher      PUBLISHes a sequence number and a timestamp on 4 channels.
  subscribers    each subscribes to one channel, checks that sequence
                 numbers arrive in order with no gaps, leaves after a random
                 time (UNSUBSCRIBE or an abrupt close) and joins again.

Writes one CSV row per --sample-s interval (see soaklib.run).
"""

import argparse
import random
import socket
import threading
import time
from collections import deque

import soaklib

CHANNELS = 4


# ─── RESP client ───────────────────────────────────────────────

class RespError(Exception):
    pass


class Conn:
    def __init__(self, host, port, timeout=10.0):
        self.s = socket.create_connection((host, port), timeout=timeout)
        self.s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self.buf = b""

    def send(self, *cmds):
        out = []
        for cmd in cmds:
            out.append(b"*%d\r\n" % len(cmd))
            for a in cmd:
                a = a if isinstance(a, bytes) else str(a).encode()
                out.append(b"$%d\r\n%s\r\n" % (len(a), a))
        self.s.sendall(b"".join(out))

    def call(self, *cmds):
        self.send(*cmds)
        return [self.reply() for _ in cmds]

    def reply(self):
        """The next reply. The buffer is consumed only once a whole reply is
        in it, so a socket timeout (subscribers read with one) never leaves
        half a reply behind."""
        while True:
            r = parse(self.buf, 0)
            if r is not None:
                value, pos = r
                self.buf = self.buf[pos:]
                return value
            chunk = self.s.recv(1 << 16)
            if not chunk:
                raise ConnectionError("server closed the connection")
            self.buf += chunk

    def close(self):
        try:
            self.s.close()
        except OSError:
            pass


def parse(buf, pos):
    """(value, end) for the reply at buf[pos:], or None if it is incomplete."""
    i = buf.find(b"\r\n", pos)
    if i < 0:
        return None
    kind, rest = buf[pos:pos + 1], buf[pos + 1:i]
    pos = i + 2
    if kind == b"+":
        return rest.decode(), pos
    if kind == b"-":
        return RespError(rest.decode()), pos
    if kind == b":":
        return int(rest), pos
    if kind == b"$":
        n = int(rest)
        if n < 0:
            return None, pos
        if len(buf) < pos + n + 2:
            return None
        return buf[pos:pos + n], pos + n + 2
    if kind == b"*":
        n = int(rest)
        if n < 0:
            return None, pos
        items = []
        for _ in range(n):
            r = parse(buf, pos)
            if r is None:
                return None
            item, pos = r
            items.append(item)
        return items, pos
    raise ConnectionError(f"bad reply line {buf[pos:i][:40]!r}")


# ─── Data clients ──────────────────────────────────────────────

UNKNOWN = object()


class DataClient(threading.Thread):
    """One connection doing a weighted mix of commands on its own keys, each
    reply checked against `self.model`. After a connection error the state
    of the in-flight commands is unknown: the client reconnects, deletes its
    structured keys and forgets what it knew about its strings."""

    OPS = [("get", 30), ("set", 20), ("del", 3), ("incr", 8), ("ttl_set", 10),
           ("ttl_get", 10), ("list", 8), ("hash", 6), ("set_type", 5)]

    def __init__(self, idx, args, stats, err, stop, rate):
        super().__init__(name=f"data{idx}")
        self.idx, self.args, self.stats, self.err, self.stop = idx, args, stats, err, stop
        self.rng = random.Random(args.seed * 1000 + idx)
        self.pacer = soaklib.Pacer(rate)
        self.ops = [o for o, w in self.OPS for _ in range(w)]
        self.version = 0
        self.reset_model(known=True)

    def reset_model(self, known):
        # strings: key -> value, or None (known absent); a missing key is
        # known absent only while `known_absent` holds.
        self.strings = {}
        self.known_absent = known
        self.counters = {}
        self.ttl = {}  # key -> (value, set_at, ttl_s)
        self.fifo = deque()
        self.hash = {}
        self.members = set()

    def key(self, kind, k=None):
        return f"{kind}:{self.idx}" if k is None else f"{kind}:{self.idx}:{k}"

    def value(self, key):
        self.version += 1
        pad = self.rng.randrange(16, 512)
        return f"{key}#{self.version}#".encode() + b"x" * pad

    def make_op(self):
        """Returns (command, check) where check(reply) raises on a wrong reply."""
        op = self.rng.choice(self.ops)
        a = self.args
        if op in ("get", "set", "del"):
            key = self.key("s", self.rng.randrange(a.keys))
            if op == "set":
                v = self.value(key)
                self.strings[key] = v
                return ["SET", key, v], expect("OK")
            if op == "del":
                had = self.strings.get(key, None if self.known_absent else UNKNOWN)
                self.strings[key] = None
                if had is UNKNOWN:
                    return ["DEL", key], lambda r: check_type(r, int)
                return ["DEL", key], expect(0 if had is None else 1)
            want = self.strings.get(key, None if self.known_absent else UNKNOWN)
            if want is UNKNOWN:
                return ["GET", key], lambda r: None
            return ["GET", key], expect(want)
        if op == "incr":
            key = self.key("c", self.rng.randrange(100))
            n = self.counters.get(key)
            self.counters[key] = (n or 0) + 1
            if n is None and not self.known_absent:
                return ["INCR", key], lambda r: check_type(r, int)
            return ["INCR", key], expect((n or 0) + 1)
        if op == "ttl_set":
            key = self.key("t", self.rng.randrange(a.keys))
            v = self.value(key)
            ttl = self.rng.randrange(1, 6)
            entry = TtlEntry(v, time.monotonic(), ttl)
            self.ttl[key] = entry
            return ["SET", key, v, "EX", ttl], entry.check_set
        if op == "ttl_get":
            key = self.key("t", self.rng.randrange(a.keys))
            entry = self.ttl.get(key)
            sent = time.monotonic()
            if entry is None:
                return ["GET", key], lambda r: None
            return ["GET", key], lambda r: entry.check_get(r, sent)
        if op == "list":
            key = self.key("l")
            if len(self.fifo) < 200 and (len(self.fifo) < 20 or self.rng.random() < 0.5):
                v = self.value(key)
                self.fifo.appendleft(v)
                return ["LPUSH", key, v], expect(len(self.fifo))
            want = self.fifo.pop() if self.fifo else None
            return ["RPOP", key], expect(want)
        if op == "hash":
            key = self.key("h")
            field = f"f{self.rng.randrange(500)}"
            r = self.rng.random()
            if r < 0.5:
                v = self.value(field)
                new = field not in self.hash
                self.hash[field] = v
                return ["HSET", key, field, v], expect(1 if new else 0)
            if r < 0.8:
                return ["HGET", key, field], expect(self.hash.get(field))
            had = self.hash.pop(field, None)
            return ["HDEL", key, field], expect(0 if had is None else 1)
        key = self.key("z")
        m = f"m{self.rng.randrange(500)}"
        r = self.rng.random()
        if r < 0.5:
            new = m not in self.members
            self.members.add(m)
            return ["SADD", key, m], expect(1 if new else 0)
        if r < 0.8:
            return ["SISMEMBER", key, m], expect(1 if m in self.members else 0)
        had = m in self.members
        self.members.discard(m)
        return ["SREM", key, m], expect(1 if had else 0)

    def connect(self):
        while not self.stop.is_set():
            try:
                c = Conn(self.args.host, self.args.port)
                if not self.known_absent:
                    # Start the structured keys over; strings stay unknown
                    # until they are written again.
                    c.call(["DEL", self.key("l"), self.key("h"), self.key("z")]
                           + [self.key("c", k) for k in range(100)])
                    self.fifo.clear()
                    self.hash.clear()
                    self.members.clear()
                    self.counters = {self.key("c", k): 0 for k in range(100)}
                return c
            except (OSError, ConnectionError) as e:
                self.err("connect", f"{self.name}: {e}")
                self.stop.wait(1.0)
        return None

    def run(self):
        c = self.connect()
        while c is not None and self.pacer.wait(self.stop):
            n = 16 if self.rng.random() < 0.1 else 1
            batch = [self.make_op() for _ in range(n)]
            t0 = time.monotonic()
            try:
                replies = c.call(*[cmd for cmd, _ in batch])
            except (OSError, ConnectionError, ValueError) as e:
                self.err("io", f"{self.name}: {type(e).__name__}: {e}")
                c.close()
                self.reset_model(known=False)
                c = self.connect()
                continue
            self.stats.latency(time.monotonic() - t0)
            self.stats.add("ops", n)
            for (cmd, check), r in zip(batch, replies):
                try:
                    if isinstance(r, RespError):
                        raise AssertionError(f"error reply {r}")
                    check(r)
                except AssertionError as e:
                    self.err("verify", f"{self.name}: {cmd[0]} {cmd[1]}: {e}")
        if c is not None:
            c.close()


def expect(want):
    def check(r):
        if r != want:
            raise AssertionError(f"got {short(r)}, want {short(want)}")
    return check


def check_type(r, t):
    if not isinstance(r, t):
        raise AssertionError(f"got {short(r)}, want a {t.__name__}")


# Clock reads on the two sides can differ by a little (ms rounding, two
# clocks); a TTL check only fails beyond this.
SLACK_S = 0.1


class TtlEntry:
    """A SET EX this client made. The server set the deadline somewhere
    between `sent_at` and `acked_at` (+ ttl); a check only fails when the
    reply is wrong for every moment in that window, so a slow reply or a
    server pause is never a false error."""

    def __init__(self, value, sent_at, ttl):
        self.value, self.sent_at, self.ttl = value, sent_at, ttl
        self.acked_at = None

    def check_set(self, r):
        self.acked_at = time.monotonic()
        expect("OK")(r)

    def check_get(self, r, get_sent):
        now = time.monotonic()  # the GET ran before now
        if r is None:
            if now < self.sent_at + self.ttl - SLACK_S:
                raise AssertionError(f"gone {now - self.sent_at:.2f} s into a {self.ttl} s TTL")
            return
        if r != self.value:
            raise AssertionError(f"got {short(r)}, want {short(self.value)}")
        if self.acked_at is not None and get_sent > self.acked_at + self.ttl + SLACK_S:
            raise AssertionError(f"still there {get_sent - self.acked_at:.2f} s into a {self.ttl} s TTL")


def short(v):
    s = repr(v)
    return s if len(s) <= 60 else s[:57] + "..."


# ─── Connection churn ──────────────────────────────────────────

class Churn(threading.Thread):
    def __init__(self, args, stats, err, stop):
        super().__init__(name="churn")
        self.args, self.stats, self.err, self.stop = args, stats, err, stop
        self.pacer = soaklib.Pacer(args.churn_rate)
        self.rng = random.Random(args.seed * 1000 + 999)

    def run(self):
        n = 0
        while self.pacer.wait(self.stop):
            n += 1
            key = f"churn:{n % 1000}"
            try:
                c = Conn(self.args.host, self.args.port)
                r = c.call(["SET", key, str(n)], ["GET", key], ["DEL", key])
                if r != ["OK", str(n).encode(), 1]:
                    self.err("verify", f"churn: got {short(r)}")
                if self.rng.random() < 0.5:
                    c.call(["QUIT"])
                c.close()
                self.stats.add("conns")
            except (OSError, ConnectionError, ValueError) as e:
                self.err("churn", f"{type(e).__name__}: {e}")


# ─── Pub/sub ───────────────────────────────────────────────────

class Publisher(threading.Thread):
    def __init__(self, args, stats, err, stop):
        super().__init__(name="publisher")
        self.args, self.stats, self.err, self.stop = args, stats, err, stop
        self.pacer = soaklib.Pacer(args.pub_rate)

    def run(self):
        seq = [0] * CHANNELS
        c = None
        i = 0
        while self.pacer.wait(self.stop):
            ch = i % CHANNELS
            i += 1
            try:
                if c is None:
                    c = Conn(self.args.host, self.args.port)
                seq[ch] += 1
                payload = f"{seq[ch]}:{time.time():.6f}"
                c.call(["PUBLISH", f"soak:{ch}", payload])
                self.stats.add("published")
            except (OSError, ConnectionError, ValueError) as e:
                self.err("publish", f"{type(e).__name__}: {e}")
                if c is not None:
                    c.close()
                c = None
                self.stop.wait(1.0)


class Subscriber(threading.Thread):
    def __init__(self, idx, args, stats, err, stop):
        super().__init__(name=f"sub{idx}")
        self.idx, self.args, self.stats, self.err, self.stop = idx, args, stats, err, stop
        self.rng = random.Random(args.seed * 1000 + 500 + idx)

    def run(self):
        while not self.stop.is_set():
            ch = self.rng.randrange(CHANNELS)
            stay = self.rng.expovariate(1.0 / self.args.sub_stay_s)
            try:
                self.session(ch, time.monotonic() + stay)
                self.stats.add("sub_sessions")
            except (OSError, ConnectionError, ValueError) as e:
                self.err("subscriber", f"{self.name}: {type(e).__name__}: {e}")
                self.stop.wait(1.0)

    def session(self, ch, leave_at):
        c = Conn(self.args.host, self.args.port)
        try:
            c.s.settimeout(0.5)
            c.send(["SUBSCRIBE", f"soak:{ch}"])
            last = None
            while not self.stop.is_set() and time.monotonic() < leave_at:
                try:
                    msg = c.reply()
                except socket.timeout:
                    continue
                if not isinstance(msg, list) or len(msg) != 3:
                    raise ConnectionError(f"unexpected push {short(msg)}")
                if msg[0] == b"subscribe":
                    continue
                seq_s, sent_s = msg[2].decode().split(":")
                seq = int(seq_s)
                self.stats.add("delivered")
                self.stats.latency(max(0.0, time.time() - float(sent_s)), "deliver")
                if last is not None and seq != last + 1:
                    self.err("gap", f"{self.name} soak:{ch}: {last} then {seq}")
                last = seq
            if self.rng.random() < 0.5:
                c.s.settimeout(10.0)
                c.send(["UNSUBSCRIBE", f"soak:{ch}"])
        finally:
            c.close()


# ─── Main ──────────────────────────────────────────────────────

COLUMNS = ["t", "elapsed_s", "interval_s", "ops", "lat_n", "lat_p50_ms", "lat_p99_ms",
           "lat_max_ms", "conns", "published", "delivered", "deliver_p50_ms",
           "deliver_p99_ms", "deliver_max_ms", "sub_sessions", "errors", "err_verify",
           "err_gap", "err_io", "err_connect", "err_churn", "err_publish",
           "err_subscriber", "probe_ms", "dbsize", "probe_error"]


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    soaklib.common_args(p, 6395)
    p.add_argument("--clients", type=int, default=8)
    p.add_argument("--rate", type=float, default=4000, help="data requests/s, all clients")
    p.add_argument("--keys", type=int, default=10000, help="string and TTL keys per client")
    p.add_argument("--churn-rate", type=float, default=20, help="connections/s")
    p.add_argument("--pub-rate", type=float, default=200, help="PUBLISHes/s")
    p.add_argument("--subs", type=int, default=20)
    p.add_argument("--sub-stay-s", type=float, default=30, help="mean subscription time")
    args = p.parse_args()

    stats = soaklib.Stats(("lat", "deliver"))
    err = soaklib.ErrorLog(args.errors, stats)
    stop = threading.Event()
    threads = [DataClient(i, args, stats, err, stop, args.rate / args.clients)
               for i in range(args.clients)]
    threads += [Churn(args, stats, err, stop), Publisher(args, stats, err, stop)]
    threads += [Subscriber(i, args, stats, err, stop) for i in range(args.subs)]

    probe = {"c": None}

    def sample_extra():
        # A fresh request on a connection of its own: the key count, and
        # how long an idle client waits for an answer.
        if probe["c"] is None:
            probe["c"] = Conn(args.host, args.port, timeout=10.0)
        t0 = time.monotonic()
        try:
            n = probe["c"].call(["DBSIZE"])[0]
        except (OSError, ConnectionError, ValueError):
            probe["c"].close()
            probe["c"] = None
            raise
        return {"dbsize": n, "probe_ms": (time.monotonic() - t0) * 1000}

    soaklib.run(args, stats, COLUMNS, sample_extra, threads, stop)


if __name__ == "__main__":
    main()
