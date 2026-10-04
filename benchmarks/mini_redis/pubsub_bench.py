#!/usr/bin/env python3
"""Pub/sub fan-out load for any Redis-protocol server (redis-benchmark has
no subscriber mode).

    pubsub_bench.py PORT SUBSCRIBERS MESSAGES [PAYLOAD_BYTES]

SUBSCRIBERS connections subscribe to one channel; one publisher sends
MESSAGES PUBLISH commands, pipelined 100 at a time. Prints the publish
rate and the delivery rate (messages received by all subscribers per
second, measured until the last subscriber has every message), as one CSV
line: subscribers,messages,payload,publish_per_s,delivered_per_s,seconds.
Only the standard library: it counts bytes, since every message frame has
the same length.
"""
import selectors
import socket
import sys
import time

CHANNEL = b"bench"


def command(*args):
    out = [b"*%d\r\n" % len(args)]
    for a in args:
        out.append(b"$%d\r\n%s\r\n" % (len(a), a))
    return b"".join(out)


def read_exactly(sock, n):
    buf = b""
    while len(buf) < n:
        chunk = sock.recv(n - len(buf))
        if not chunk:
            raise RuntimeError("server closed the connection")
        buf += chunk
    return buf


def main():
    port, nsubs, nmsgs = (int(x) for x in sys.argv[1:4])
    size = int(sys.argv[4]) if len(sys.argv) > 4 else 32
    payload = b"x" * size
    frame = b"*3\r\n$7\r\nmessage\r\n$%d\r\n%s\r\n$%d\r\n%s\r\n" % (
        len(CHANNEL), CHANNEL, len(payload), payload)
    confirm = b"*3\r\n$9\r\nsubscribe\r\n$%d\r\n%s\r\n:1\r\n" % (len(CHANNEL), CHANNEL)

    subs = []
    for _ in range(nsubs):
        s = socket.create_connection(("127.0.0.1", port))
        s.sendall(command(b"SUBSCRIBE", CHANNEL))
        if read_exactly(s, len(confirm)) != confirm:
            raise RuntimeError("unexpected SUBSCRIBE reply")
        s.setblocking(False)
        subs.append(s)

    sel = selectors.DefaultSelector()
    remaining = {}
    for s in subs:
        sel.register(s, selectors.EVENT_READ)
        remaining[s] = nmsgs * len(frame)

    pub = socket.create_connection(("127.0.0.1", port))
    pub.setblocking(False)
    reply_len = len(b":%d\r\n" % nsubs)
    start = time.monotonic()
    published_at = None
    sent = 0
    pending_replies = 0
    while sent < nmsgs or pending_replies or remaining:
        if sent < nmsgs and pending_replies == 0:
            batch = min(100, nmsgs - sent)
            pub.setblocking(True)
            pub.sendall(command(b"PUBLISH", CHANNEL, payload) * batch)
            pub.setblocking(False)
            sent += batch
            pending_replies = batch * reply_len
        if pending_replies:
            try:
                data = pub.recv(65536)
                pending_replies -= len(data)
            except BlockingIOError:
                pass
            if sent == nmsgs and pending_replies == 0:
                published_at = time.monotonic()
        for key, _ in sel.select(timeout=0.001):
            s = key.fileobj
            data = s.recv(1 << 20)
            if not data:
                raise RuntimeError("a subscriber was disconnected")
            remaining[s] -= len(data)
            if remaining[s] <= 0:
                sel.unregister(s)
                del remaining[s]
    end = time.monotonic()
    if published_at is None:
        published_at = end
    print("%d,%d,%d,%.0f,%.0f,%.2f" % (
        nsubs, nmsgs, size, nmsgs / (published_at - start),
        nsubs * nmsgs / (end - start), end - start))
    for s in subs:
        s.close()
    pub.close()


if __name__ == "__main__":
    main()
