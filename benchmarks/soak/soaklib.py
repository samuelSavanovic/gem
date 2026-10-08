"""Shared pieces of the soak load generators: interval statistics, a CSV
writer that survives an interrupted run, and the run loop that ends on
the deadline or on SIGINT/SIGTERM/SIGHUP.

Every row is flushed and fsynced as it is written, so a run that is
killed, or a machine that goes down, keeps every interval sampled before
it. Nothing is buffered for the end of the run.
"""

import csv
import os
import signal
import sys
import threading
import time


class Csv:
    """Appends rows to a CSV file, flushed and fsynced per row."""

    def __init__(self, path, columns):
        self.columns = columns
        new = not os.path.exists(path) or os.path.getsize(path) == 0
        self.f = open(path, "a", newline="")
        self.w = csv.writer(self.f)
        if new:
            self.w.writerow(columns)
            self.sync()

    def row(self, values):
        self.w.writerow([fmt(values.get(c)) for c in self.columns])
        self.sync()

    def sync(self):
        self.f.flush()
        os.fsync(self.f.fileno())

    def close(self):
        self.f.close()


def fmt(v):
    if v is None:
        return ""
    if isinstance(v, float):
        return f"{v:.3f}"
    return str(v)


def percentile(sorted_values, q):
    if not sorted_values:
        return None
    i = min(len(sorted_values) - 1, int(q * len(sorted_values)))
    return sorted_values[i]


class Stats:
    """Counters and latency samples for the current interval, shared by the
    load threads. `take` returns them and starts a new interval; `totals`
    keeps the sums over the whole run."""

    def __init__(self, latency_names=("lat",)):
        self.lock = threading.Lock()
        self.latency_names = latency_names
        self.counts = {}
        self.totals = {}
        self.lat = {n: [] for n in latency_names}

    def add(self, name, n=1):
        with self.lock:
            self.counts[name] = self.counts.get(name, 0) + n
            self.totals[name] = self.totals.get(name, 0) + n

    def latency(self, seconds, name="lat"):
        with self.lock:
            self.lat[name].append(seconds)

    def take(self):
        with self.lock:
            counts, self.counts = self.counts, {}
            lat, self.lat = self.lat, {n: [] for n in self.latency_names}
        row = dict(counts)
        for name, values in lat.items():
            values.sort()
            row[f"{name}_n"] = len(values)
            for label, q in (("p50", 0.50), ("p99", 0.99)):
                v = percentile(values, q)
                row[f"{name}_{label}_ms"] = None if v is None else v * 1000
            row[f"{name}_max_ms"] = values[-1] * 1000 if values else None
        return row


class ErrorLog:
    """Writes the first `limit` errors in full to a file (flushed per line)
    and counts every error in `stats` under `errors`."""

    def __init__(self, path, stats, limit=200):
        self.f = open(path, "a")
        self.stats = stats
        self.limit = limit
        self.seen = 0
        self.lock = threading.Lock()

    def __call__(self, kind, message):
        self.stats.add("errors")
        self.stats.add(f"err_{kind}")
        with self.lock:
            self.seen += 1
            if self.seen <= self.limit:
                self.f.write(f"{time.time():.3f} {kind} {message}\n")
                self.f.flush()
            elif self.seen == self.limit + 1:
                self.f.write("(further errors are only counted)\n")
                self.f.flush()


class Pacer:
    """Spaces calls `1 / rate` seconds apart. After a stall it catches up at
    most one second's worth, rather than bursting the whole backlog."""

    def __init__(self, rate):
        self.period = 1.0 / rate if rate > 0 else 0.0
        self.next = time.monotonic()

    def wait(self, stop):
        if self.period == 0.0:
            return not stop.is_set()
        now = time.monotonic()
        if self.next < now - 1.0:
            self.next = now - 1.0
        if self.next > now:
            if stop.wait(self.next - now):
                return False
        self.next += self.period
        return not stop.is_set()


def run(args, stats, columns, sample_extra, threads, stop):
    """Starts `threads`, writes a row of `columns` to `args.out` every
    `args.sample_s` seconds until `args.duration_s` passes or a SIGINT,
    SIGTERM or SIGHUP arrives, then stops the threads and writes `args.out` + ".done"
    with the totals. `sample_extra()` adds probe values (key counts and the
    like) to each row; it runs on this thread.

    Exits 0 when the run reached its deadline, 3 when it was stopped early:
    by a signal (Ctrl-C, or the sampler's SIGTERM when the server is gone)
    or because run.sh, its parent, exited.
    """
    out = Csv(args.out, columns)
    interrupted = []

    # The handler only records the signal: it runs on the main thread
    # between bytecodes, possibly inside stop.wait(), and an Event's lock is
    # not reentrant, so calling stop.set() here can deadlock.
    def on_signal(signum, frame):
        interrupted.append(signal.Signals(signum).name)

    signal.signal(signal.SIGINT, on_signal)
    signal.signal(signal.SIGTERM, on_signal)
    signal.signal(signal.SIGHUP, on_signal)

    start = time.time()
    parent = os.getppid()
    deadline = time.monotonic() + args.duration_s
    for t in threads:
        t.daemon = True
        t.start()
    next_sample = time.monotonic() + args.sample_s
    last = time.monotonic()
    while True:
        wait = min(next_sample, deadline) - time.monotonic()
        if wait > 0:
            time.sleep(min(wait, 0.2))
        if os.getppid() != parent:
            interrupted.append("parent exited")  # run.sh was killed hard
        if interrupted or stop.is_set():
            break
        now = time.monotonic()
        if now < next_sample and now < deadline:
            continue
        if now >= next_sample:
            write_row(out, stats, sample_extra, start, now - last)
            last = now
            next_sample += args.sample_s
            if next_sample < time.monotonic():
                # A probe stalled for a whole interval (a paused server):
                # carry on from now rather than writing catch-up rows.
                next_sample = time.monotonic() + args.sample_s
        if now >= deadline:
            break
    stop.set()
    for t in threads:
        t.join(timeout=5.0)
    # The partial interval since the last row, so the totals add up.
    if time.monotonic() - last >= 1.0:
        write_row(out, stats, sample_extra, start, time.monotonic() - last)
    out.close()

    reached = not interrupted and time.monotonic() >= deadline - 1.0
    reason = "deadline" if reached else (interrupted[0] if interrupted else "stopped")
    with open(args.out + ".done", "w") as f:
        f.write(f"end_reason={reason}\n")
        f.write(f"elapsed_s={time.time() - start:.1f}\n")
        for k in sorted(stats.totals):
            f.write(f"total_{k}={stats.totals[k]}\n")
    print(f"load: {reason} after {time.time() - start:.0f} s", file=sys.stderr)
    sys.exit(0 if reached else 3)


def write_row(out, stats, sample_extra, start, interval_s):
    row = stats.take()
    row["t"] = round(time.time(), 3)
    row["elapsed_s"] = round(time.time() - start, 1)
    row["interval_s"] = round(interval_s, 2)
    try:
        row.update(sample_extra())
    except Exception as e:  # a probe failing is data, not a reason to stop
        row["probe_error"] = f"{type(e).__name__}"
    out.row(row)


def common_args(p, port):
    p.add_argument("--host", default="127.0.0.1")
    p.add_argument("--port", type=int, default=port)
    p.add_argument("--duration-s", type=float, required=True)
    p.add_argument("--sample-s", type=float, default=30.0)
    p.add_argument("--out", required=True, help="interval CSV to append to")
    p.add_argument("--errors", required=True, help="error log")
    p.add_argument("--seed", type=int, default=1)
