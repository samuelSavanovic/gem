#!/usr/bin/env python3.14
"""STOMP load harness for the milestone-6 broker tests.

Subcommands:
  fanout       1 publisher → N topic subscribers
  slow         1 publisher → mix of fast + slow topic subscribers
  queue        M publishers → /queue/X with K workers
  microbench   open N subs and have publisher send 1 msg, time fanout latency

Output: a single JSON line to stdout with the run's summary numbers.
"""

import argparse
import json
import os
import random
import socket
import string
import sys
import threading
import time
from contextlib import contextmanager


HOST = "127.0.0.1"
PORT = 61613
NUL = b"\x00"


# ─── STOMP wire helpers ────────────────────────────────────────

def encode_frame(command: str, headers: dict, body: bytes = b"") -> bytes:
    out = command.encode() + b"\n"
    for k, v in headers.items():
        out += f"{k}:{v}".encode() + b"\n"
    out += b"\n" + body + NUL
    return out


def connect_stomp(timeout: float = 5.0) -> socket.socket:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(timeout)
    s.connect((HOST, PORT))
    s.sendall(encode_frame("CONNECT", {"accept-version": "1.2", "host": HOST}))
    # Read until we see the CONNECTED frame's terminating NUL.
    buf = b""
    while NUL not in buf:
        chunk = s.recv(4096)
        if not chunk:
            raise RuntimeError("broker closed during CONNECT")
        buf += chunk
    if not buf.startswith(b"CONNECTED"):
        raise RuntimeError(f"unexpected handshake reply: {buf!r}")
    return s


def subscribe(s: socket.socket, sub_id: str, dest: str) -> None:
    s.sendall(encode_frame("SUBSCRIBE", {"id": sub_id, "destination": dest}))


def send_message(s: socket.socket, dest: str, body: bytes, extra: dict | None = None) -> None:
    headers = {"destination": dest, "content-length": str(len(body))}
    if extra:
        headers.update(extra)
    s.sendall(encode_frame("SEND", headers, body))


def disconnect(s: socket.socket) -> None:
    try:
        s.sendall(encode_frame("DISCONNECT", {}))
    except OSError:
        pass
    try:
        s.close()
    except OSError:
        pass


def random_body(n: int) -> bytes:
    # Random ASCII so compression / dedup tricks (if any) don't hide cost.
    return ("".join(random.choices(string.ascii_letters + string.digits, k=n))).encode()


# ─── Subscriber thread: count MESSAGE frames, optionally slow ──

class Subscriber(threading.Thread):
    """Connects, subscribes, counts MESSAGE frames received.

    If `slow_delay_s` > 0, sleeps that long after each MESSAGE
    *before reading more from the socket*, simulating a slow consumer.
    Crucially: we never drain past the kernel buffer, so the broker's
    writer process backs up on tcp_write — that's the backpressure-free
    failure mode this test is meant to stress.
    """

    def __init__(self, idx: int, dest: str, slow_delay_s: float = 0.0,
                 stop_event: threading.Event = None,
                 ready_event: threading.Event = None,
                 record_first: bool = False,
                 record_each: bool = False,
                 connect_delay_s: float = 0.0):
        super().__init__(daemon=True)
        self.idx = idx
        self.dest = dest
        self.slow_delay_s = slow_delay_s
        self.stop_event = stop_event or threading.Event()
        self.ready_event = ready_event
        self.record_first = record_first
        self.record_each = record_each
        self.connect_delay_s = connect_delay_s
        self.first_msg_at: float | None = None
        self.recv_times: list[float] = []
        self.count = 0
        self.error: str | None = None
        self.bytes_in = 0

    def run(self) -> None:
        try:
            if self.connect_delay_s > 0:
                time.sleep(self.connect_delay_s)
            s = connect_stomp(timeout=30.0)
            s.settimeout(2.0)
            subscribe(s, f"s{self.idx}", self.dest)
            if self.ready_event:
                self.ready_event.set()
            buf = b""
            while not self.stop_event.is_set():
                try:
                    chunk = s.recv(65536)
                except socket.timeout:
                    continue
                if not chunk:
                    break
                self.bytes_in += len(chunk)
                buf += chunk
                # Drain whole frames terminated by NUL.
                while True:
                    idx = buf.find(NUL)
                    if idx < 0:
                        break
                    frame = buf[:idx]
                    buf = buf[idx + 1:]
                    # Skip heartbeat/leading newlines
                    if frame.startswith(b"MESSAGE"):
                        self.count += 1
                        now = time.monotonic()
                        if self.record_first and self.first_msg_at is None:
                            self.first_msg_at = now
                        if self.record_each:
                            self.recv_times.append(now)
                        if self.slow_delay_s > 0:
                            time.sleep(self.slow_delay_s)
            disconnect(s)
        except Exception as e:
            self.error = f"{type(e).__name__}: {e}"


# ─── RSS sampler (broker pid) ──────────────────────────────────

def sample_rss(pid: int, out_path: str, interval: float, stop: threading.Event) -> None:
    import subprocess
    start = time.monotonic()
    with open(out_path, "w") as f:
        f.write("elapsed_s,rss_kb\n")
        while not stop.is_set():
            try:
                rv = subprocess.run(
                    ["ps", "-o", "rss=", "-p", str(pid)],
                    capture_output=True, text=True, timeout=2.0,
                )
                rss = rv.stdout.strip()
                if not rss:
                    break
                f.write(f"{time.monotonic() - start:.2f},{rss}\n")
                f.flush()
            except Exception:
                break
            stop.wait(interval)


# ─── Workloads ─────────────────────────────────────────────────

def cmd_fanout(args: argparse.Namespace) -> dict:
    """1 publisher → args.n_subs topic subscribers; args.n_msgs body=args.body_size."""
    dest = f"/topic/load_fanout_{int(time.time())}"
    stop = threading.Event()
    ready_events = [threading.Event() for _ in range(args.n_subs)]
    # Stagger connects so the broker's accept loop isn't slammed with N
    # simultaneous SYNs (1000 at once on a single accept loop wedges).
    stagger_window_s = min(5.0, args.n_subs * 0.002)
    subs = [
        Subscriber(i, dest, stop_event=stop,
                   ready_event=ready_events[i],
                   record_first=True,
                   connect_delay_s=(i / max(args.n_subs, 1)) * stagger_window_s)
        for i in range(args.n_subs)
    ]
    for s in subs:
        s.start()
    # Wait for all SUBSCRIBE frames to land.
    deadline = time.monotonic() + 60.0
    for ev in ready_events:
        remaining = deadline - time.monotonic()
        if remaining <= 0 or not ev.wait(timeout=remaining):
            raise RuntimeError("subscribers never became ready")
    # Give the broker a moment to register all subs before publishing.
    time.sleep(1.0)

    pub = connect_stomp()
    body_template = random_body(args.body_size)
    publish_send_times: list[float] = []
    publish_start = time.monotonic()
    for n in range(args.n_msgs):
        send_message(pub, dest, body_template, extra={"seq": str(n)})
        publish_send_times.append(time.monotonic())
        if args.publish_pause_s > 0:
            time.sleep(args.publish_pause_s)
    publish_done = time.monotonic()
    disconnect(pub)

    # Wait for all subs to receive all messages, with a generous timeout.
    expected = args.n_msgs
    deadline = time.monotonic() + max(60.0, args.n_msgs * args.n_subs * 0.001 + 30.0)
    while time.monotonic() < deadline:
        if all(s.count >= expected for s in subs):
            break
        time.sleep(0.1)
    fanout_done = time.monotonic()
    stop.set()
    for s in subs:
        s.join(timeout=2.0)

    counts = [s.count for s in subs]
    received_total = sum(counts)
    expected_total = expected * args.n_subs
    n_complete = sum(1 for c in counts if c >= expected)
    publish_elapsed = publish_done - publish_start
    fanout_elapsed = fanout_done - publish_start

    # Per-message fan-out latency: time from publisher's send of msg-0 to last
    # subscriber's first MESSAGE receipt. Coarse, but enough to spot regressions.
    first_received = [s.first_msg_at for s in subs if s.first_msg_at is not None]
    last_first = max(first_received) - publish_send_times[0] if first_received else None
    median_first = (sorted(first_received)[len(first_received) // 2] - publish_send_times[0]
                    if first_received else None)

    errors = [s.error for s in subs if s.error]

    return {
        "workload": "fanout",
        "n_subs": args.n_subs,
        "n_msgs": args.n_msgs,
        "body_size": args.body_size,
        "publish_elapsed_s": round(publish_elapsed, 4),
        "publish_msgs_per_s": round(args.n_msgs / publish_elapsed, 1) if publish_elapsed > 0 else None,
        "fanout_elapsed_s": round(fanout_elapsed, 4),
        "delivered_msgs_total": received_total,
        "delivered_msgs_per_s": round(received_total / fanout_elapsed, 1) if fanout_elapsed > 0 else None,
        "expected_total": expected_total,
        "complete_subs": n_complete,
        "median_first_msg_latency_s": round(median_first, 4) if median_first is not None else None,
        "p100_first_msg_latency_s": round(last_first, 4) if last_first is not None else None,
        "errors": errors[:5],
        "n_errors": len(errors),
    }


def cmd_slow(args: argparse.Namespace) -> dict:
    """Fast publisher + mix of fast and slow subs, both on the same topic."""
    dest = f"/topic/load_slow_{int(time.time())}"
    stop = threading.Event()
    ready_events_fast = [threading.Event() for _ in range(args.n_fast)]
    ready_events_slow = [threading.Event() for _ in range(args.n_slow)]
    total = args.n_fast + args.n_slow
    stagger_window_s = min(2.0, total * 0.002)
    fast_subs = [
        Subscriber(i, dest, slow_delay_s=0.0, stop_event=stop,
                   ready_event=ready_events_fast[i],
                   connect_delay_s=(i / max(total, 1)) * stagger_window_s)
        for i in range(args.n_fast)
    ]
    slow_subs = [
        Subscriber(args.n_fast + i, dest,
                   slow_delay_s=args.slow_delay_ms / 1000.0,
                   stop_event=stop,
                   ready_event=ready_events_slow[i],
                   connect_delay_s=((args.n_fast + i) / max(total, 1)) * stagger_window_s)
        for i in range(args.n_slow)
    ]
    all_subs = fast_subs + slow_subs
    for s in all_subs:
        s.start()
    deadline = time.monotonic() + 60.0
    for ev in ready_events_fast + ready_events_slow:
        remaining = deadline - time.monotonic()
        if remaining <= 0 or not ev.wait(timeout=remaining):
            raise RuntimeError("subscribers never became ready")
    time.sleep(1.0)

    pub = connect_stomp(timeout=120.0)
    body_template = random_body(args.body_size)
    publish_start = time.monotonic()
    sent = 0
    publisher_blocked_at: float | None = None
    publish_deadline = publish_start + args.duration_s
    try:
        # Fire as fast as the OS lets us — the broker's writer to the slow sub's
        # connection process is the bottleneck we want to expose.
        while time.monotonic() < publish_deadline:
            if args.max_publishes and sent >= args.max_publishes:
                break
            send_message(pub, dest, body_template, extra={"seq": str(sent)})
            sent += 1
            if args.publish_pause_s > 0:
                time.sleep(args.publish_pause_s)
    except Exception as e:
        publisher_blocked_at = time.monotonic() - publish_start
        publisher_err = f"{type(e).__name__}: {e}"
    else:
        publisher_err = None
    publish_elapsed = time.monotonic() - publish_start
    disconnect(pub)

    # Let consumers drain for a short window post-publish.
    drain_until = time.monotonic() + min(args.drain_s, 30.0)
    while time.monotonic() < drain_until:
        time.sleep(0.5)
    stop.set()
    for s in all_subs:
        s.join(timeout=2.0)

    fast_counts = [s.count for s in fast_subs]
    slow_counts = [s.count for s in slow_subs]
    return {
        "workload": "slow_consumer",
        "n_fast": args.n_fast,
        "n_slow": args.n_slow,
        "slow_delay_ms": args.slow_delay_ms,
        "body_size": args.body_size,
        "duration_s": args.duration_s,
        "publish_elapsed_s": round(publish_elapsed, 3),
        "msgs_sent": sent,
        "publish_msgs_per_s": round(sent / publish_elapsed, 1) if publish_elapsed > 0 else None,
        "publisher_error": publisher_err,
        "publisher_blocked_at_s": round(publisher_blocked_at, 3) if publisher_blocked_at else None,
        "fast_subs_msgs": fast_counts,
        "fast_subs_min": min(fast_counts) if fast_counts else None,
        "fast_subs_max": max(fast_counts) if fast_counts else None,
        "slow_subs_msgs": slow_counts,
        "slow_subs_min": min(slow_counts) if slow_counts else None,
        "slow_subs_max": max(slow_counts) if slow_counts else None,
        "n_errors": sum(1 for s in all_subs if s.error),
    }


def cmd_soak(args: argparse.Namespace) -> dict:
    """Sustained low-rate fanout to characterize steady-state behavior.

    Publishes at args.rate msg/s for args.duration_s while N subs receive.
    Use with a separately-sampled RSS series to see whether per-iteration
    arena reset keeps memory flat.
    """
    dest = f"/topic/load_soak_{int(time.time())}"
    stop = threading.Event()
    ready_events = [threading.Event() for _ in range(args.n_subs)]
    stagger_window_s = min(5.0, args.n_subs * 0.002)
    subs = [
        Subscriber(i, dest, stop_event=stop,
                   ready_event=ready_events[i],
                   connect_delay_s=(i / max(args.n_subs, 1)) * stagger_window_s)
        for i in range(args.n_subs)
    ]
    for s in subs:
        s.start()
    deadline = time.monotonic() + 60.0
    for ev in ready_events:
        remaining = deadline - time.monotonic()
        if remaining <= 0 or not ev.wait(timeout=remaining):
            raise RuntimeError("subscribers never became ready")
    time.sleep(1.0)

    pub = connect_stomp()
    body_template = random_body(args.body_size)
    sent = 0
    publish_start = time.monotonic()
    publish_deadline = publish_start + args.duration_s
    period = 1.0 / max(args.rate, 0.001)
    publisher_err = None
    try:
        while time.monotonic() < publish_deadline:
            send_message(pub, dest, body_template, extra={"seq": str(sent)})
            sent += 1
            next_send = publish_start + sent * period
            now = time.monotonic()
            if next_send > now:
                time.sleep(next_send - now)
    except Exception as e:
        publisher_err = f"{type(e).__name__}: {e}"
    publish_elapsed = time.monotonic() - publish_start
    disconnect(pub)

    drain_until = time.monotonic() + min(args.drain_s, 30.0)
    while time.monotonic() < drain_until:
        time.sleep(0.5)
    stop.set()
    for s in subs:
        s.join(timeout=2.0)

    counts = [s.count for s in subs]
    return {
        "workload": "soak",
        "n_subs": args.n_subs,
        "rate_msgs_per_s": args.rate,
        "body_size": args.body_size,
        "duration_s": args.duration_s,
        "msgs_sent": sent,
        "publish_elapsed_s": round(publish_elapsed, 3),
        "delivered_total": sum(counts),
        "expected_total": sent * args.n_subs,
        "min_received": min(counts) if counts else None,
        "max_received": max(counts) if counts else None,
        "publisher_error": publisher_err,
        "n_errors": sum(1 for s in subs if s.error),
    }


def cmd_queue(args: argparse.Namespace) -> dict:
    """M publishers → /queue/X with K workers."""
    dest = f"/queue/load_q_{int(time.time())}"
    stop = threading.Event()
    ready_events = [threading.Event() for _ in range(args.n_workers)]
    workers = [
        Subscriber(i, dest, stop_event=stop, ready_event=ready_events[i],
                   connect_delay_s=i * 0.05)
        for i in range(args.n_workers)
    ]
    for w in workers:
        w.start()
    deadline = time.monotonic() + 30.0
    for ev in ready_events:
        remaining = deadline - time.monotonic()
        if remaining <= 0 or not ev.wait(timeout=remaining):
            raise RuntimeError("workers never became ready")
    time.sleep(1.0)

    publisher_threads: list[threading.Thread] = []
    publisher_errors: list[str] = []
    publisher_lock = threading.Lock()
    msgs_per_publisher = args.n_msgs // args.n_publishers
    body_size = args.body_size

    def publisher(pid: int) -> None:
        try:
            pub = connect_stomp()
            body = random_body(body_size)
            for n in range(msgs_per_publisher):
                send_message(pub, dest, body,
                             extra={"pub": str(pid), "seq": str(n)})
            disconnect(pub)
        except Exception as e:
            with publisher_lock:
                publisher_errors.append(f"{type(e).__name__}: {e}")

    publish_start = time.monotonic()
    for p in range(args.n_publishers):
        t = threading.Thread(target=publisher, args=(p,), daemon=True)
        t.start()
        publisher_threads.append(t)
    for t in publisher_threads:
        t.join(timeout=120.0)
    publish_elapsed = time.monotonic() - publish_start

    expected_total = msgs_per_publisher * args.n_publishers
    deadline = time.monotonic() + 60.0
    while time.monotonic() < deadline:
        if sum(w.count for w in workers) >= expected_total:
            break
        time.sleep(0.2)
    elapsed_total = time.monotonic() - publish_start
    stop.set()
    for w in workers:
        w.join(timeout=2.0)

    counts = [w.count for w in workers]
    return {
        "workload": "queue",
        "n_publishers": args.n_publishers,
        "n_workers": args.n_workers,
        "msgs_per_publisher": msgs_per_publisher,
        "body_size": args.body_size,
        "publish_elapsed_s": round(publish_elapsed, 3),
        "publish_msgs_per_s": round(expected_total / publish_elapsed, 1) if publish_elapsed > 0 else None,
        "total_elapsed_s": round(elapsed_total, 3),
        "delivered_total": sum(counts),
        "expected_total": expected_total,
        "worker_counts": counts,
        "worker_min": min(counts),
        "worker_max": max(counts),
        "fairness_ratio": round(min(counts) / max(counts), 3) if max(counts) > 0 else None,
        "publisher_errors": publisher_errors[:5],
        "n_publisher_errors": len(publisher_errors),
    }


# ─── argparse ──────────────────────────────────────────────────

def main() -> None:
    global HOST, PORT
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--host", default=HOST)
    p.add_argument("--port", type=int, default=PORT)
    sub = p.add_subparsers(dest="cmd", required=True)

    f = sub.add_parser("fanout")
    f.add_argument("--n-subs", type=int, default=1000)
    f.add_argument("--n-msgs", type=int, default=10)
    f.add_argument("--body-size", type=int, default=128)
    f.add_argument("--publish-pause-s", type=float, default=0.0,
                   help="sleep between publishes; lets the destination drain")
    f.set_defaults(fn=cmd_fanout)

    soak = sub.add_parser("soak")
    soak.add_argument("--n-subs", type=int, default=100)
    soak.add_argument("--rate", type=float, default=10.0)
    soak.add_argument("--body-size", type=int, default=256)
    soak.add_argument("--duration-s", type=float, default=30.0)
    soak.add_argument("--drain-s", type=float, default=3.0)
    soak.set_defaults(fn=cmd_soak)

    s = sub.add_parser("slow")
    s.add_argument("--n-fast", type=int, default=4)
    s.add_argument("--n-slow", type=int, default=1)
    s.add_argument("--slow-delay-ms", type=int, default=200)
    s.add_argument("--body-size", type=int, default=1024)
    s.add_argument("--duration-s", type=float, default=10.0)
    s.add_argument("--publish-pause-s", type=float, default=0.0)
    s.add_argument("--drain-s", type=float, default=5.0)
    s.add_argument("--max-publishes", type=int, default=0,
                   help="cap on publishes; 0 = unbounded (run for duration_s)")
    s.set_defaults(fn=cmd_slow)

    q = sub.add_parser("queue")
    q.add_argument("--n-publishers", type=int, default=10)
    q.add_argument("--n-workers", type=int, default=4)
    q.add_argument("--n-msgs", type=int, default=10000)
    q.add_argument("--body-size", type=int, default=256)
    q.set_defaults(fn=cmd_queue)

    args = p.parse_args()
    HOST = args.host
    PORT = args.port

    result = args.fn(args)
    json.dump(result, sys.stdout, indent=2)
    sys.stdout.write("\n")


if __name__ == "__main__":
    main()
