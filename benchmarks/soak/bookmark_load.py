#!/usr/bin/env python3
"""Steady mixed load on examples/bookmark_app for a soak run.

Threads, all paced:

  writer     the only one that changes the table: POST, PUT and DELETE,
             keeping it near --rows bookmarks (so the list the app renders
             on every change stays the same size, see benchmarks/README.md,
             "POST phase is O(N²)"). It keeps a model of the table and
             checks that the list each change answers with matches it.
  readers    keep-alive connections doing GET /, GET /bookmarks and
             GET /bookmarks/<id>/edit for a bookmark the writer made.
  churn      one request per connection (Connection: close).

Writes one CSV row per --sample-s interval (see soaklib.run).
"""

import argparse
import http.client
import random
import re
import threading
import time
import urllib.parse

import soaklib

ROW = re.compile(r"id='title-(\d+)'>([^<]*)<")
IO_ERRORS = (OSError, http.client.HTTPException)


class Model:
    """The table as the writer made it: id -> title. Readers pick ids from it."""

    def __init__(self):
        self.lock = threading.Lock()
        self.rows = {}

    def ids(self):
        with self.lock:
            return list(self.rows)


def request(conn, method, path, body=None):
    headers = {}
    if body is not None:
        body = urllib.parse.urlencode(body)
        headers["Content-Type"] = "application/x-www-form-urlencoded"
    conn.request(method, path, body=body, headers=headers)
    r = conn.getresponse()
    return r.status, r.read().decode("utf-8", "replace")


class Writer(threading.Thread):
    def __init__(self, args, stats, err, stop, model):
        super().__init__(name="writer")
        self.args, self.stats, self.err, self.stop, self.model = args, stats, err, stop, model
        self.pacer = soaklib.Pacer(args.write_rate)
        self.rng = random.Random(args.seed * 1000 + 1)
        self.n = 0

    def connect(self):
        return http.client.HTTPConnection(self.args.host, self.args.port, timeout=30)

    def run(self):
        c = self.connect()
        while self.pacer.wait(self.stop):
            rows = self.model.rows
            self.n += 1
            title = f"soak-{self.n}"
            if len(rows) < self.args.rows and (not rows or self.rng.random() < 0.7):
                method, path = "POST", "/bookmarks"
                form = {"url": f"https://example.com/soak/{self.n}", "title": title,
                        "tags": "soak"}
            elif self.rng.random() < 0.5:
                victim = self.rng.choice(list(rows))
                method, path = "PUT", f"/bookmarks/{victim}"
                form = {"url": f"https://example.com/soak/{victim}", "title": title,
                        "tags": "soak,edited"}
            else:
                victim = min(rows)
                method, path, form = "DELETE", f"/bookmarks/{victim}", None
            t0 = time.monotonic()
            try:
                status, page = request(c, method, path, form)
            except IO_ERRORS as e:
                self.err("io", f"writer {method} {path}: {type(e).__name__}: {e}")
                c.close()
                c = self.connect()
                self.resync(c)
                continue
            self.stats.latency(time.monotonic() - t0, "write")
            self.stats.add("writes")
            if status != 200:
                self.err("status", f"writer {method} {path}: HTTP {status}")
                self.resync(c)
                continue
            got = dict((int(i), t) for i, t in ROW.findall(page))
            with self.model.lock:
                if method == "POST":
                    new = [i for i, t in got.items() if t == title]
                    if len(new) != 1:
                        self.err("verify", f"writer POST: {title} appears {len(new)} times")
                    else:
                        rows[new[0]] = title
                elif method == "PUT":
                    rows[victim] = title
                else:
                    rows.pop(victim, None)
                if got != rows:
                    missing = set(rows) - set(got)
                    extra = set(got) - set(rows)
                    self.err("verify", f"writer {method} {path}: list differs, "
                             f"{len(missing)} missing, {len(extra)} extra")
                    rows.clear()
                    rows.update(got)

    def resync(self, c):
        """After a failed request the table is unknown: take the app's list."""
        try:
            status, page = request(c, "GET", "/bookmarks")
            if status == 200:
                with self.model.lock:
                    self.model.rows.clear()
                    self.model.rows.update((int(i), t) for i, t in ROW.findall(page))
        except IO_ERRORS:
            pass


class Reader(threading.Thread):
    def __init__(self, idx, args, stats, err, stop, model, rate, close_each):
        super().__init__(name=("churn" if close_each else f"reader{idx}"))
        self.args, self.stats, self.err, self.stop, self.model = args, stats, err, stop, model
        self.pacer = soaklib.Pacer(rate)
        self.rng = random.Random(args.seed * 1000 + 100 + idx)
        self.close_each = close_each

    def run(self):
        c = None
        while self.pacer.wait(self.stop):
            r = self.rng.random()
            ids = self.model.ids()
            if r < 0.2:
                path, want = "/", "<html"
            elif r < 0.7 or not ids:
                path, want = "/bookmarks", "<table>"
            else:
                i = self.rng.choice(ids)
                path, want = f"/bookmarks/{i}/edit", f"Editing bookmark #{i}"
            t0 = time.monotonic()
            try:
                if c is None:
                    c = http.client.HTTPConnection(self.args.host, self.args.port, timeout=30)
                if self.close_each:
                    c.request("GET", path, headers={"Connection": "close"})
                    resp = c.getresponse()
                    status, page = resp.status, resp.read().decode("utf-8", "replace")
                else:
                    status, page = request(c, "GET", path)
            except IO_ERRORS as e:
                self.err("io", f"{self.name} GET {path}: {type(e).__name__}: {e}")
                if c is not None:
                    c.close()
                c = None
                continue
            self.stats.latency(time.monotonic() - t0, "read")
            self.stats.add("reads")
            if self.close_each:
                c.close()
                c = None
                self.stats.add("conns")
            # An edit page may race with the writer deleting that bookmark.
            if status == 404 and path.endswith("/edit"):
                continue
            if status != 200:
                self.err("status", f"{self.name} GET {path}: HTTP {status}")
            elif want not in page:
                self.err("verify", f"{self.name} GET {path}: no {want!r} in the page")


COLUMNS = ["t", "elapsed_s", "interval_s", "reads", "read_n", "read_p50_ms", "read_p99_ms",
           "read_max_ms", "writes", "write_p50_ms", "write_p99_ms", "write_max_ms", "conns",
           "rows", "errors", "err_verify", "err_status", "err_io", "probe_ms", "probe_error"]


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    soaklib.common_args(p, 8080)
    p.add_argument("--readers", type=int, default=4)
    p.add_argument("--read-rate", type=float, default=200, help="GETs/s, all readers")
    p.add_argument("--write-rate", type=float, default=20, help="POST/PUT/DELETEs per s")
    p.add_argument("--churn-rate", type=float, default=10, help="one-request connections/s")
    p.add_argument("--rows", type=int, default=100, help="table size the writer keeps")
    args = p.parse_args()

    stats = soaklib.Stats(("read", "write"))
    err = soaklib.ErrorLog(args.errors, stats)
    stop = threading.Event()
    model = Model()
    threads = [Writer(args, stats, err, stop, model)]
    threads += [Reader(i, args, stats, err, stop, model, args.read_rate / args.readers, False)
                for i in range(args.readers)]
    threads.append(Reader(args.readers, args, stats, err, stop, model, args.churn_rate, True))

    def sample_extra():
        t0 = time.monotonic()
        c = http.client.HTTPConnection(args.host, args.port, timeout=30)
        try:
            c.request("GET", "/")
            c.getresponse().read()
        finally:
            c.close()
        return {"probe_ms": (time.monotonic() - t0) * 1000, "rows": len(model.rows)}

    soaklib.run(args, stats, COLUMNS, sample_extra, threads, stop)


if __name__ == "__main__":
    main()
