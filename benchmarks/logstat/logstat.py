#!/usr/bin/env python3
"""Reference implementation of examples/logstat for benchmarks/logstat/run.sh.

Same parsing rules and the same text report, written the obvious way in
Python, so the two can be timed on the same input and their output diffed.
Supports the subset run.sh uses: --by, --sort, --top, files or stdin.
"""
import argparse
import sys

MONTHS = {m: f"{i + 1:02d}" for i, m in enumerate(
    "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec".split())}


def parse(line):
    ip_end = line.find(" ")
    user_start = line.find(" ", ip_end + 1) + 1
    time_start = line.find(" [", user_start)
    time_end = line.find('] "', time_start)
    if ip_end <= 0 or user_start <= 0 or time_start < 0 or time_end < 0:
        return None
    req_start = time_end + 3
    req_end = line.find('" ', req_start)
    if req_end < 0:
        return None
    status_end = line.find(" ", req_end + 2)
    if status_end < 0:
        return None
    bytes_end = line.find(" ", status_end + 1)
    if bytes_end < 0:
        bytes_end = len(line)
    status = line[req_end + 2:status_end]
    size = line[status_end + 1:bytes_end]
    if len(status) != 3 or not status.isdigit():
        return None
    if size == "-":
        size = "0"
    elif not size.isdigit():
        return None
    t = line[time_start + 2:time_end]
    if len(t) < 20 or t[3:6] not in MONTHS:
        return None
    hour = f"{t[7:11]}-{MONTHS[t[3:6]]}-{t[0:2]} {t[12:14]}:00"
    request = line[req_start:req_end]
    if request == "-":
        method, path = "-", "-"
    else:
        parts = request.split(" ")
        if len(parts) != 3:
            return None
        method, path = parts[0], parts[1].split("?", 1)[0]
    return {"ip": line[:ip_end], "user": line[user_start:time_start],
            "method": method, "path": path, "status": status,
            "bytes": int(size), "hour": hour}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--by", default="path")
    ap.add_argument("--sort", default="requests")
    ap.add_argument("--top", type=int, default=10)
    ap.add_argument("--quiet", action="store_true")
    ap.add_argument("files", nargs="*")
    opts = ap.parse_args()

    lines = malformed = requests = total_bytes = errors = 0
    groups = {}
    for name in opts.files or ["-"]:
        f = sys.stdin if name == "-" else open(name)
        for line in f:
            line = line.rstrip("\n").rstrip("\r")
            lines += 1
            req = parse(line)
            if req is None:
                malformed += 1
                continue
            requests += 1
            total_bytes += req["bytes"]
            is_error = int(req["status"]) >= 400
            g = groups.setdefault(req[opts.by], {"requests": 0, "bytes": 0, "errors": 0})
            g["requests"] += 1
            g["bytes"] += req["bytes"]
            if is_error:
                errors += 1
                g["errors"] += 1

    rows = sorted(groups.items(), key=lambda kv: (-kv[1][opts.sort], kv[0]))
    if opts.top > 0:
        rows = rows[:opts.top]
    tenths = (errors * 1000 + requests // 2) // requests if requests else 0
    out = [f"lines {lines}, malformed {malformed}, requests {requests}, "
           f"bytes {total_bytes}, errors {errors} ({tenths // 10}.{tenths % 10}%)", "",
           f"top {len(rows)} of {len(groups)} by {opts.by}, sorted by {opts.sort}"]
    cols = ["requests", "bytes", "errors"]
    widths = [max([len(c)] + [len(str(g[c])) for _, g in rows]) for c in cols]
    out.append("".join(c.rjust(w) + "  " for c, w in zip(cols, widths)) + opts.by)
    for key, g in rows:
        out.append("".join(str(g[c]).rjust(w) + "  " for c, w in zip(cols, widths)) + key)
    print("\n".join(out))


if __name__ == "__main__":
    main()
