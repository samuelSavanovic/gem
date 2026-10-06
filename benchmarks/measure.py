#!/usr/bin/env python3
"""Runs a command and prints one row: median wall time, peak RSS and
instructions retired. Shared by the batch harnesses (logstat, lox,
gemgrep, jobqueue).

    measure.py [options] LABEL -- CMD...

Options:
    --stdin FILE      the command's stdin (default /dev/null)
    --out FILE        its stdout (of the last run)
    --err FILE        its stderr (of the last run)
    --ok-max N        exit statuses up to N count as success (default 0)
    --wall-file FILE  write the median wall time (seconds) there
    --width N         width of the label column (default 28)
    --bench, --case, --impl   the CSV row's keys (--case defaults to the
                      label, the others to empty)

Environment:
    BENCH_REPS    timed runs (default 1); the row shows their median wall
                  time, the highest peak RSS and the median instruction count
    BENCH_WARMUP  untimed runs before them (default 0)
    BENCH_CSV     append one row per timed run to this CSV file

On macOS each run goes through `/usr/bin/time -l`, which reports user and
system time, peak RSS, instructions retired, cycles and peak memory
footprint; elsewhere they come from wait4, without the last three. On a
failing run (an exit status above --ok-max, or death by a signal) it
prints the command's stderr and exits non-zero.
"""

import argparse
import csv
import os
import re
import statistics
import subprocess
import sys
import tempfile
import time

FIELDS = ["bench", "case", "impl", "rep", "wall_s", "user_s", "sys_s",
          "maxrss_kb", "footprint_kb", "instructions", "cycles"]

TIME_KEYS = {
    "maximum resident set size": "maxrss",
    "instructions retired": "instructions",
    "cycles elapsed": "cycles",
    "peak memory footprint": "footprint",
}


def parse_time_l(text):
    """Parses the report of macOS `/usr/bin/time -l`."""
    stats = {}
    m = re.search(r"([\d.]+) real\s+([\d.]+) user\s+([\d.]+) sys", text)
    if m:
        stats["user_s"] = float(m.group(2))
        stats["sys_s"] = float(m.group(3))
    for line in text.splitlines():
        m = re.match(r"\s*(\d+)\s+(.+?)\s*$", line)
        if m and m.group(2) in TIME_KEYS:
            stats[TIME_KEYS[m.group(2)]] = int(m.group(1))
    return {
        "user_s": stats.get("user_s"),
        "sys_s": stats.get("sys_s"),
        "maxrss_kb": stats["maxrss"] // 1024 if "maxrss" in stats else None,
        "footprint_kb": stats["footprint"] // 1024 if "footprint" in stats else None,
        "instructions": stats.get("instructions"),
        "cycles": stats.get("cycles"),
    }


def run_once(cmd, stdin, out, err):
    """Runs cmd once; returns (exit status, wall seconds, stats)."""
    darwin = sys.platform == "darwin"
    if darwin:
        fd, report = tempfile.mkstemp(prefix="gem_measure_")
        os.close(fd)
        cmd = ["/usr/bin/time", "-l", "-o", report] + cmd
    with open(stdin, "rb") as i, open(out, "wb") as o, open(err, "wb") as e:
        start = time.monotonic()
        proc = subprocess.Popen(cmd, stdin=i, stdout=o, stderr=e)
        _, status, ru = os.wait4(proc.pid, 0)
        wall = time.monotonic() - start
    proc.returncode = os.waitstatus_to_exitcode(status)
    if darwin:
        with open(report) as f:
            stats = parse_time_l(f.read())
        os.unlink(report)
        # time exits 1 when the command dies by a signal, and says so on
        # its stderr, which is the command's.
        with open(err, errors="replace") as f:
            if "time: command terminated abnormally" in f.read().splitlines():
                proc.returncode = -1
    else:
        stats = {"user_s": ru.ru_utime, "sys_s": ru.ru_stime, "maxrss_kb": ru.ru_maxrss,
                 "footprint_kb": None, "instructions": None, "cycles": None}
    return proc.returncode, wall, stats


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--stdin", default="/dev/null")
    ap.add_argument("--out", default="/dev/null")
    ap.add_argument("--err", default=None)
    ap.add_argument("--ok-max", type=int, default=0)
    ap.add_argument("--wall-file")
    ap.add_argument("--width", type=int, default=28)
    ap.add_argument("--bench", default="")
    ap.add_argument("--case", default="")
    ap.add_argument("--impl", default="")
    ap.add_argument("label")
    ap.add_argument("cmd", nargs=argparse.REMAINDER)
    a = ap.parse_args()
    cmd = a.cmd[1:] if a.cmd[:1] == ["--"] else a.cmd
    if not cmd:
        ap.error("no command")
    err = a.err
    if err is None:
        fd, err = tempfile.mkstemp(prefix="gem_measure_err_")
        os.close(fd)

    reps = max(1, int(os.environ.get("BENCH_REPS", "1")))
    warmup = max(0, int(os.environ.get("BENCH_WARMUP", "0")))
    rows = []
    for i in range(warmup + reps):
        rc, wall, stats = run_once(cmd, a.stdin, a.out, err)
        if rc < 0 or rc > a.ok_max:
            sys.stdout.write(open(err, errors="replace").read())
            print(f"{a.label}: {'killed by a signal' if rc < 0 else f'exit status {rc}'}", file=sys.stderr)
            sys.exit(rc if rc > 0 else 1)
        if i >= warmup:
            rows.append(dict(stats, wall_s=wall, rep=i - warmup + 1))

    walls = [r["wall_s"] for r in rows]
    med = statistics.median(walls)
    peaks = [r["maxrss_kb"] for r in rows if r["maxrss_kb"] is not None]
    instrs = [r["instructions"] for r in rows if r["instructions"] is not None]
    line = f"{a.label:<{a.width}} {med:8.2f} s {max(peaks) if peaks else 0:9d} KB"
    if instrs:
        line += f" {statistics.median(instrs) / 1e9:8.2f} G instr"
    if reps > 1:
        line += f"   (n={reps}, min {min(walls):.2f}, max {max(walls):.2f})"
    print(line, flush=True)

    if a.wall_file:
        with open(a.wall_file, "w") as f:
            f.write(f"{med}\n")
    path = os.environ.get("BENCH_CSV")
    if path:
        new = not os.path.exists(path) or os.path.getsize(path) == 0
        with open(path, "a", newline="") as f:
            w = csv.DictWriter(f, fieldnames=FIELDS)
            if new:
                w.writeheader()
            for r in rows:
                w.writerow({
                    "bench": a.bench, "case": a.case or a.label, "impl": a.impl,
                    "rep": r["rep"], "wall_s": f"{r['wall_s']:.4f}",
                    "user_s": r["user_s"], "sys_s": r["sys_s"],
                    "maxrss_kb": r["maxrss_kb"], "footprint_kb": r["footprint_kb"],
                    "instructions": r["instructions"], "cycles": r["cycles"],
                })
    if a.err is None:
        os.unlink(err)


if __name__ == "__main__":
    main()
