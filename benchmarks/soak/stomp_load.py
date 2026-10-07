#!/usr/bin/env python3
"""Steady mixed load on examples/stomp_broker for a soak run.

Threads, all paced:

  topic publisher    SENDs a sequence number and a timestamp to 4 topics.
  topic subscribers  each subscribes to one topic, checks that sequence
                     numbers arrive in order with no gaps, leaves after a
                     random time (UNSUBSCRIBE + DISCONNECT, or an abrupt
                     close) and joins again.
  queue producer     SENDs numbered jobs to one queue.
  queue workers      stay subscribed for the whole run; every job must reach
                     exactly one of them (a duplicate is an error, and the
                     backlog, sent minus received, must stay bounded).
  churn              connects, SENDs one message, DISCONNECTs with a
                     receipt and waits for it.

No slow consumers: the broker queues a slow subscriber's messages for up
to 10 s and then disconnects it (examples/stomp_broker/README.md, "Known
limits"), so memory would follow the consumers rather than the broker, and
each disconnect would count as an error. Writes one CSV row per --sample-s
interval (see soaklib.run).
"""

import argparse
import random
import socket
import threading
import time

import soaklib

TOPICS = 4
NUL = b"\x00"


# ─── STOMP client ──────────────────────────────────────────────

class Conn:
    def __init__(self, host, port, timeout=10.0):
        self.s = socket.create_connection((host, port), timeout=timeout)
        self.s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self.buf = b""
        self.send("CONNECT", {"accept-version": "1.2", "host": host})
        f = self.frame()
        if f[0] != "CONNECTED":
            raise ConnectionError(f"handshake got {f[0]}")

    def send(self, command, headers, body=b""):
        out = [command.encode(), b"\n"]
        for k, v in headers.items():
            out.append(f"{k}:{v}\n".encode())
        out += [b"\n", body, NUL]
        self.s.sendall(b"".join(out))

    def frame(self):
        """(command, headers, body) of the next frame. The buffer is consumed
        only once a whole frame is in it, so a socket timeout loses nothing."""
        while True:
            self.buf = self.buf.lstrip(b"\r\n")  # heart-beats
            i = self.buf.find(NUL)
            if i >= 0:
                raw, self.buf = self.buf[:i], self.buf[i + 1:]
                head, _, body = raw.partition(b"\n\n")
                lines = head.decode().split("\n")
                headers = {}
                for line in lines[1:]:
                    k, _, v = line.partition(":")
                    headers.setdefault(k, v)
                if lines[0] == "ERROR":
                    raise ConnectionError(f"ERROR frame: {headers.get('message', '')}")
                return lines[0], headers, body
            chunk = self.s.recv(1 << 16)
            if not chunk:
                raise ConnectionError("broker closed the connection")
            self.buf += chunk

    def wait_receipt(self, rid):
        while True:
            cmd, h, _ = self.frame()
            if cmd == "RECEIPT" and h.get("receipt-id") == rid:
                return

    def close(self):
        try:
            self.s.close()
        except OSError:
            pass


IO_ERRORS = (OSError, ConnectionError, ValueError, UnicodeDecodeError)


def body(seq, pad):
    return f"{seq} {time.time():.6f} ".encode() + pad


def parse_body(b):
    seq, sent, _ = b.split(b" ", 2)
    return int(seq), float(sent)


# ─── Topics ────────────────────────────────────────────────────

class Publisher(threading.Thread):
    """SENDs to `dests` in turn at `rate`, each with its own sequence."""

    def __init__(self, name, dests, rate, args, stats, err, stop, counter, on_send=None,
                 ready=None):
        super().__init__(name=name)
        self.ready = ready
        self.dests, self.args, self.stats, self.err, self.stop = dests, args, stats, err, stop
        self.rate = rate
        self.counter = counter
        self.on_send = on_send
        self.pad = b"x" * args.body_size

    def run(self):
        if self.ready is not None:
            while not self.ready() and not self.stop.wait(0.1):
                pass
        pacer = soaklib.Pacer(self.rate)
        seq = [0] * len(self.dests)
        c = None
        i = 0
        while pacer.wait(self.stop):
            d = i % len(self.dests)
            i += 1
            try:
                if c is None:
                    c = Conn(self.args.host, self.args.port)
                seq[d] += 1
                if self.on_send:
                    self.on_send(seq[d])
                c.send("SEND", {"destination": self.dests[d]}, body(seq[d], self.pad))
                self.stats.add(self.counter)
            except IO_ERRORS as e:
                self.err("publish", f"{self.name}: {type(e).__name__}: {e}")
                if c is not None:
                    c.close()
                c = None
                self.stop.wait(1.0)


class TopicSubscriber(threading.Thread):
    def __init__(self, idx, args, stats, err, stop):
        super().__init__(name=f"sub{idx}")
        self.idx, self.args, self.stats, self.err, self.stop = idx, args, stats, err, stop
        self.rng = random.Random(args.seed * 1000 + idx)

    def run(self):
        while not self.stop.is_set():
            t = self.rng.randrange(TOPICS)
            stay = self.rng.expovariate(1.0 / self.args.sub_stay_s)
            try:
                self.session(t, time.monotonic() + stay)
                self.stats.add("sub_sessions")
            except IO_ERRORS as e:
                self.err("subscriber", f"{self.name}: {type(e).__name__}: {e}")
                self.stop.wait(1.0)

    def session(self, t, leave_at):
        c = Conn(self.args.host, self.args.port)
        try:
            dest = f"/topic/soak.{t}"
            c.send("SUBSCRIBE", {"destination": dest, "id": "0", "receipt": "sub"})
            c.wait_receipt("sub")
            c.s.settimeout(0.5)
            last = None
            while not self.stop.is_set() and time.monotonic() < leave_at:
                try:
                    cmd, h, b = c.frame()
                except socket.timeout:
                    continue
                if cmd != "MESSAGE":
                    continue
                seq, sent = parse_body(b)
                self.stats.add("delivered")
                self.stats.latency(max(0.0, time.time() - sent), "deliver")
                if last is not None and seq != last + 1:
                    self.err("gap", f"{self.name} {dest}: {last} then {seq}")
                last = seq
            if self.rng.random() < 0.5:
                c.s.settimeout(10.0)
                c.send("UNSUBSCRIBE", {"id": "0"})
                c.send("DISCONNECT", {"receipt": "bye"})
                c.wait_receipt("bye")
        finally:
            c.close()


# ─── Queue ─────────────────────────────────────────────────────

class QueueBook:
    """Which jobs were sent and which arrived, to find duplicates and to
    report the backlog."""

    def __init__(self, stats, err):
        self.lock = threading.Lock()
        self.subscribed = 0
        self.sent = 0
        self.received = set()
        self.stats, self.err = stats, err

    def worker_subscribed(self):
        with self.lock:
            self.subscribed += 1

    def on_send(self, seq):
        with self.lock:
            self.sent = seq

    def on_receive(self, seq):
        with self.lock:
            if seq in self.received:
                dup = True
            else:
                dup = False
                self.received.add(seq)
        if dup:
            self.err("duplicate", f"job {seq} delivered twice")

    def backlog(self):
        with self.lock:
            return self.sent - len(self.received)


class Worker(threading.Thread):
    def __init__(self, idx, args, stats, err, stop, book):
        super().__init__(name=f"worker{idx}")
        self.args, self.stats, self.err, self.stop, self.book = args, stats, err, stop, book

    def run(self):
        while not self.stop.is_set():
            c = None
            try:
                c = Conn(self.args.host, self.args.port)
                c.send("SUBSCRIBE", {"destination": "/queue/soak.jobs", "id": "0",
                                     "receipt": "sub"})
                c.wait_receipt("sub")
                self.book.worker_subscribed()
                c.s.settimeout(0.5)
                while not self.stop.is_set():
                    try:
                        cmd, h, b = c.frame()
                    except socket.timeout:
                        continue
                    if cmd == "MESSAGE":
                        seq, sent = parse_body(b)
                        self.book.on_receive(seq)
                        self.stats.add("jobs_done")
                        self.stats.latency(max(0.0, time.time() - sent), "job")
            except IO_ERRORS as e:
                self.err("worker", f"{self.name}: {type(e).__name__}: {e}")
                self.stop.wait(1.0)
            finally:
                if c is not None:
                    c.close()


# ─── Connection churn ──────────────────────────────────────────

class Churn(threading.Thread):
    def __init__(self, args, stats, err, stop):
        super().__init__(name="churn")
        self.args, self.stats, self.err, self.stop = args, stats, err, stop
        self.pacer = soaklib.Pacer(args.churn_rate)

    def run(self):
        n = 0
        while self.pacer.wait(self.stop):
            n += 1
            try:
                c = Conn(self.args.host, self.args.port)
                c.send("SEND", {"destination": "/topic/soak.churn"}, f"{n}".encode())
                c.send("DISCONNECT", {"receipt": "bye"})
                c.wait_receipt("bye")
                c.close()
                self.stats.add("conns")
            except IO_ERRORS as e:
                self.err("churn", f"{type(e).__name__}: {e}")


# ─── Main ──────────────────────────────────────────────────────

COLUMNS = ["t", "elapsed_s", "interval_s", "published", "delivered", "deliver_n",
           "deliver_p50_ms", "deliver_p99_ms", "deliver_max_ms", "sub_sessions",
           "jobs_sent", "jobs_done", "job_p50_ms", "job_p99_ms", "job_max_ms",
           "queue_backlog", "conns", "errors", "err_gap", "err_duplicate", "err_publish",
           "err_subscriber", "err_worker", "err_churn", "probe_ms", "probe_error"]


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    soaklib.common_args(p, 61613)
    p.add_argument("--pub-rate", type=float, default=200, help="topic SENDs/s")
    p.add_argument("--subs", type=int, default=30)
    p.add_argument("--sub-stay-s", type=float, default=30, help="mean subscription time")
    p.add_argument("--job-rate", type=float, default=200, help="queue SENDs/s")
    p.add_argument("--workers", type=int, default=4)
    p.add_argument("--churn-rate", type=float, default=10, help="connections/s")
    p.add_argument("--body-size", type=int, default=256)
    args = p.parse_args()

    stats = soaklib.Stats(("deliver", "job"))
    err = soaklib.ErrorLog(args.errors, stats)
    stop = threading.Event()
    book = QueueBook(stats, err)
    topics = [f"/topic/soak.{t}" for t in range(TOPICS)]
    threads = [Publisher("publisher", topics, args.pub_rate, args, stats, err, stop, "published"),
               Publisher("producer", ["/queue/soak.jobs"], args.job_rate, args, stats, err,
                         stop, "jobs_sent", book.on_send,
                         ready=lambda: book.subscribed >= args.workers),
               Churn(args, stats, err, stop)]
    threads += [TopicSubscriber(i, args, stats, err, stop) for i in range(args.subs)]
    threads += [Worker(i, args, stats, err, stop, book) for i in range(args.workers)]

    def sample_extra():
        # A connection of its own: how long a new client waits for CONNECTED.
        t0 = time.monotonic()
        c = Conn(args.host, args.port)
        c.close()
        return {"probe_ms": (time.monotonic() - t0) * 1000, "queue_backlog": book.backlog()}

    soaklib.run(args, stats, COLUMNS, sample_extra, threads, stop)


if __name__ == "__main__":
    main()
