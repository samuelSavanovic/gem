#!/usr/bin/env python3
"""Samples a server process for a soak run: RSS, on macOS its physical
footprint, CPU time and open file descriptors every --interval seconds, one
CSV row each (flushed and fsynced, see soaklib.Csv).

Exits when the process is gone. Then, or when its RSS passes --guard-rss-mb
(the process is killed first), it writes the reason to --status and sends
SIGTERM to --notify, the load generator, so that it stops early instead of
loading a server that isn't there.
"""

import argparse
import ctypes
import os
import platform
import signal
import subprocess
import time

import soaklib


def ps(pid):
    """(rss_kb, cpu_s) of `pid`, or None when it is gone."""
    r = subprocess.run(["ps", "-o", "rss=,time=", "-p", str(pid)],
                       capture_output=True, text=True, timeout=10)
    parts = r.stdout.split()
    if r.returncode != 0 or len(parts) < 2:
        return None
    return int(parts[0]), proc_cpu_seconds(pid) or cpu_seconds(parts[1])


def footprint_kb(pid):
    """The physical footprint of `pid` in KB (macOS), or None.

    It leaves out pages the process has released with MADV_FREE_REUSABLE,
    which RSS counts until the kernel takes them back."""
    if platform.system() != "Darwin":
        return None
    try:
        libc = ctypes.CDLL("/usr/lib/libSystem.B.dylib")
    except OSError:
        return None
    # struct rusage_info_v2 (flavor 2) is a 16-byte uuid and then uint64
    # fields; ri_phys_footprint is the eighth of them.
    info = (ctypes.c_uint64 * 20)()
    if libc.proc_pid_rusage(pid, 2, ctypes.byref(info)) != 0:
        return None
    return info[9] // 1024


def proc_cpu_seconds(pid):
    """utime + stime from /proc (Linux), whose ps only shows whole seconds."""
    try:
        with open(f"/proc/{pid}/stat") as f:
            fields = f.read().rsplit(")", 1)[1].split()
        return (int(fields[11]) + int(fields[12])) / os.sysconf("SC_CLK_TCK")
    except (OSError, IndexError, ValueError):
        return None


def cpu_seconds(s):
    # [[dd-]hh:]mm:ss[.cc]
    days = 0
    if "-" in s:
        d, s = s.split("-", 1)
        days = int(d)
    total = 0.0
    for part in s.split(":"):
        total = total * 60 + float(part)
    return days * 86400 + total


def open_fds(pid):
    if platform.system() == "Linux":
        try:
            return len(os.listdir(f"/proc/{pid}/fd"))
        except OSError:
            return None
    r = subprocess.run(["lsof", "-n", "-P", "-p", str(pid)],
                       capture_output=True, text=True, timeout=30)
    # Header line, then one line per open file (cwd, txt and mapped files
    # included: compare the series with itself, not with Linux's count).
    lines = r.stdout.count("\n")
    return lines - 1 if lines > 0 else None


def alive(pid):
    try:
        os.kill(pid, 0)
        return True
    except ProcessLookupError:
        return False
    except PermissionError:
        return True


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--pid", type=int, required=True)
    p.add_argument("--interval", type=float, required=True)
    p.add_argument("--out", required=True)
    p.add_argument("--status", required=True)
    p.add_argument("--notify", type=int, default=0, help="pid to SIGTERM when the server is gone")
    p.add_argument("--guard-rss-mb", type=float, default=0)
    args = p.parse_args()

    stopping = []
    signal.signal(signal.SIGTERM, lambda s, f: stopping.append(s))
    out = soaklib.Csv(args.out, ["t", "elapsed_s", "rss_kb", "footprint_kb", "cpu_s", "cpu_pct",
                                 "fds"])
    start = time.time()
    prev = None
    reason = None
    parent = os.getppid()
    while not stopping and os.getppid() == parent:  # run.sh killed hard: stop
        sample = ps(args.pid)
        if sample is None or not alive(args.pid):
            reason = "server exited"
            break
        rss, cpu = sample
        now = time.time()
        cpu_pct = None
        if prev is not None and now > prev[0]:
            cpu_pct = 100.0 * (cpu - prev[1]) / (now - prev[0])
        prev = (now, cpu)
        out.row({"t": round(now, 3), "elapsed_s": round(now - start, 1), "rss_kb": rss,
                 "footprint_kb": footprint_kb(args.pid), "cpu_s": cpu, "cpu_pct": cpu_pct, "fds": open_fds(args.pid)})
        if args.guard_rss_mb and rss / 1024 > args.guard_rss_mb:
            reason = f"guard: RSS {rss / 1024:.1f} MB > {args.guard_rss_mb:g} MB, server killed"
            try:
                os.kill(args.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
            break
        deadline = time.monotonic() + args.interval
        while not stopping:
            left = deadline - time.monotonic()
            if left <= 0:
                break
            time.sleep(min(0.2, left))
    out.close()
    if reason is not None:
        with open(args.status, "a") as f:
            f.write(f"sampler={reason} at {time.time() - start:.0f} s\n")
        if args.notify:
            try:
                os.kill(args.notify, signal.SIGTERM)
            except ProcessLookupError:
                pass


if __name__ == "__main__":
    main()
