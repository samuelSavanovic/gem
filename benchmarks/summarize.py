#!/usr/bin/env python3
"""Summarizes a baseline directory written by benchmarks/measure_all.sh.

    summarize.py DIR                  # writes DIR/summary.csv and DIR/summary.md
    summarize.py --compare OLD NEW    # prints how NEW's numbers differ from OLD's

summary.csv has one row per (bench, case, impl, metric). The batch rows
come from batch.csv (medians over the timed runs, the highest peak RSS);
the server rows from the harnesses' raw output. --compare reads both
directories' summary.csv and prints a table of the headline metrics they
share (COMPARED: wall time, instructions, peak memory, throughput,
latency), with the change in percent, marked ✓ (better) or ✗ (worse)
beyond 5%.
"""

import csv
import json
import os
import re
import statistics
import sys
from collections import OrderedDict

# The metrics in COMPARED where a higher value is better; for the rest of
# COMPARED lower is.
HIGHER_IS_BETTER = {"req_per_s", "rps", "throughput_jobs_per_s", "publish_per_s",
                    "delivered_per_s", "publish_msgs_per_s", "delivered_msgs_per_s"}


class Summary:
    def __init__(self):
        self.rows = []

    def add(self, bench, case, impl, metric, value):
        if value is None or value == "":
            return
        self.rows.append((bench, case, impl, metric, value))


def fmt(v, digits=2):
    if isinstance(v, float) and v.is_integer() and abs(v) >= 1000:
        return f"{int(v):,}"
    if isinstance(v, float):
        return f"{v:,.{digits}f}"
    if isinstance(v, int):
        return f"{v:,}"
    return str(v)


def md_table(headers, rows):
    out = ["| " + " | ".join(headers) + " |",
           "|" + "|".join("---" if i == 0 else "---:" for i in range(len(headers))) + "|"]
    for r in rows:
        out.append("| " + " | ".join(fmt(c) for c in r) + " |")
    return "\n".join(out)


def read_text(path):
    try:
        with open(path, errors="replace") as f:
            return f.read()
    except OSError:
        return None


# ---------------------------------------------------------------- batch

GEM_IMPLS = ("gem", "gem_file", "gem_stdin")


def batch(base, s, md):
    path = os.path.join(base, "batch.csv")
    if not os.path.exists(path):
        return
    groups = OrderedDict()
    with open(path) as f:
        for r in csv.DictReader(f):
            groups.setdefault((r["bench"], r["case"], r["impl"]), []).append(r)

    stats = OrderedDict()
    for key, runs in groups.items():
        def col(name, conv=float):
            vals = [conv(r[name]) for r in runs if r[name] not in ("", "None")]
            return vals
        walls = col("wall_s")
        instr = col("instructions", int)
        rss = col("maxrss_kb", int)
        foot = col("footprint_kb", int)
        st = {
            "runs": len(runs),
            "wall_s": statistics.median(walls),
            "wall_min_s": min(walls),
            "wall_max_s": max(walls),
            "wall_spread_pct": 100 * (max(walls) - min(walls)) / statistics.median(walls) if statistics.median(walls) else 0.0,
            "user_s": statistics.median(col("user_s")) if col("user_s") else None,
            "sys_s": statistics.median(col("sys_s")) if col("sys_s") else None,
            "instructions": int(statistics.median(instr)) if instr else None,
            "instr_spread_pct": 100 * (max(instr) - min(instr)) / statistics.median(instr) if instr else None,
            "cycles": int(statistics.median(col("cycles", int))) if col("cycles", int) else None,
            "maxrss_kb": max(rss) if rss else None,
            "footprint_kb": max(foot) if foot else None,
        }
        stats[key] = st
        for m, v in st.items():
            s.add(*key, m, round(v, 4) if isinstance(v, float) else v)

    benches = OrderedDict()
    for (bench, case, impl) in stats:
        benches.setdefault(bench, OrderedDict()).setdefault(case, []).append(impl)
    for bench, cases in benches.items():
        md.append(f"\n### {bench}\n")
        rows = []
        for case, impls in cases.items():
            gem = next((i for i in impls if i in GEM_IMPLS), None)
            for impl in impls:
                st = stats[(bench, case, impl)]
                ratio = ""
                if gem and impl not in GEM_IMPLS:
                    g = stats[(bench, case, gem)]
                    ratio = f"{g['wall_s'] / st['wall_s']:.2f}" if st["wall_s"] else ""
                    if g["instructions"] and st["instructions"]:
                        ratio += f" / {g['instructions'] / st['instructions']:.2f}"
                rows.append([case, impl, st["wall_s"],
                             f"{st['wall_spread_pct']:.0f}%",
                             st["instructions"] / 1e9 if st["instructions"] else "",
                             st["maxrss_kb"] / 1024 if st["maxrss_kb"] else "",
                             ratio])
        md.append(md_table(["case", "impl", "wall s (median)", "spread", "G instr", "peak RSS MB",
                            "gem/impl (wall / instr)"], rows))


# ---------------------------------------------------------------- jobqueue

JQ_ROW = re.compile(
    r"^(\w+)\s+([\d.]+) s\s+(\S+) jobs/s\s+p50\s+(\S+)\s+p99\s+(\S+) ms\s+lag\s+(\S+) ms\s+(\d+) MB\s+"
    r"procs\s+(\S+)\s+retries\s+(\S+)\s+sup\s+(\S+)\s+(\S+)")


def jobqueue(base, s, md):
    text = read_text(os.path.join(base, "jobqueue.txt"))
    if text is None:
        return
    runs = OrderedDict()
    scenario = None
    for line in text.splitlines():
        m = re.match(r"^── (\w+):", line)
        if m:
            scenario = m.group(1)
            continue
        m = JQ_ROW.match(line)
        if m and scenario:
            runs.setdefault((scenario, m.group(1)), []).append(m.groups())
    rows = []
    for (scenario, impl), rs in runs.items():
        def med(i, conv=float):
            vals = [conv(r[i]) for r in rs if r[i] not in ("?",)]
            return statistics.median(vals) if vals else None
        vals = {
            "throughput_jobs_per_s": med(2),
            "latency_p50_ms": med(3),
            "latency_p99_ms": med(4),
            "tick_lag_ms": med(5),
            "peak_rss_mb": max(int(r[6]) for r in rs),
            "retries": med(8),
            "invariants_ok": sum(1 for r in rs if r[10] == "ok"),
            "runs": len(rs),
        }
        for k, v in vals.items():
            s.add("jobqueue", scenario, impl, k, v)
        rows.append([scenario, impl, vals["throughput_jobs_per_s"], vals["latency_p50_ms"],
                     vals["latency_p99_ms"], vals["tick_lag_ms"], vals["peak_rss_mb"],
                     f"{vals['invariants_ok']}/{vals['runs']}"])
    if rows:
        md.append("\n### jobqueue (program-measured; medians over runs)\n")
        md.append(md_table(["scenario", "impl", "jobs/s", "p50 ms", "p99 ms", "tick lag ms",
                            "peak RSS MB", "invariants ok"], rows))


# ---------------------------------------------------------------- wrk servers

def to_ms(v):
    m = re.match(r"([\d.]+)(us|ms|s)$", v)
    if not m:
        return None
    x = float(m.group(1))
    return {"us": x / 1000, "ms": x, "s": x * 1000}[m.group(2)]


def parse_wrk(text):
    r = {}
    m = re.search(r"Requests/sec:\s+([\d.]+)", text)
    if m:
        r["req_per_s"] = float(m.group(1))
    for pct in ("50", "75", "90", "99"):
        m = re.search(rf"^\s+{pct}%\s+(\S+)", text, re.M)
        if m:
            r[f"p{pct}_ms"] = to_ms(m.group(1))
    m = re.search(r"(\d+) requests in", text)
    if m:
        r["requests"] = int(m.group(1))
    m = re.search(r"Non-2xx or 3xx responses:\s+(\d+)", text)
    r["non_2xx"] = int(m.group(1)) if m else 0
    m = re.search(r"Socket errors: connect (\d+), read (\d+), write (\d+), timeout (\d+)", text)
    r["socket_errors"] = sum(int(x) for x in m.groups()) if m else 0
    return r


def parse_rss_summary(text):
    r = {}
    # The bookmark harnesses print the lowest sample as Start and End minus
    # it as Growth; stomp's start_kb and growth_kb use the first sample.
    for key, name in (("Start", "rss_min_kb"), ("End", "rss_end_kb"), ("Peak", "rss_peak_kb"),
                      ("Mean", "rss_mean_kb"), ("Growth", "rss_growth_from_min_kb"),
                      ("start_kb", "rss_start_kb"), ("end_kb", "rss_end_kb"), ("peak_kb", "rss_peak_kb"),
                      ("mean_kb", "rss_mean_kb"), ("growth_kb", "rss_growth_kb")):
        m = re.search(rf"^\s*{key}:\s+(-?\d+)", text, re.M)
        if m:
            r[name] = int(m.group(1))
    return r


def bookmark(base, s, md):
    phases = ["burst_get_bookmarks", "burst_get_index", "soak_get_bookmarks", "burst_post_bookmarks"]
    impls = [("bookmark", "gem"), ("bookmark_node", "node")]
    found = False
    rows = []
    for phase in phases:
        for d, impl in impls:
            text = read_text(os.path.join(base, d, phase + ".txt"))
            if text is None:
                continue
            found = True
            w = parse_wrk(text)
            for k, v in w.items():
                s.add("bookmark", phase, impl, k, v)
            rows.append([phase, impl, w.get("req_per_s", ""), w.get("p50_ms", ""), w.get("p99_ms", ""),
                         w.get("non_2xx", ""), w.get("socket_errors", "")])
    if not found:
        return
    md.append("\n### bookmark_app vs Node (wrk)\n")
    md.append(md_table(["phase", "impl", "req/s", "p50 ms", "p99 ms", "non-2xx", "socket errors"], rows))
    rows = []
    for d, impl in impls:
        text = read_text(os.path.join(base, d, "rss_summary.txt"))
        if text is None:
            continue
        r = parse_rss_summary(text)
        for k, v in r.items():
            s.add("bookmark", "rss_whole_run", impl, k, v)
        rows.append([impl] + [r.get(k, "") / 1024 if r.get(k) is not None else ""
                              for k in ("rss_min_kb", "rss_peak_kb", "rss_mean_kb", "rss_end_kb", "rss_growth_from_min_kb")])
        tail = soak_tail_growth(os.path.join(base, d, "rss.csv"))
        if tail is not None:
            s.add("bookmark", "rss_whole_run", impl, "rss_second_half_growth_kb", tail)
            rows[-1].append(tail / 1024)
    md.append("\nRSS over the whole run (MB):\n")
    md.append(md_table(["impl", "min", "peak", "mean", "end", "end − min", "growth over 2nd half"], rows))


def soak_tail_growth(path):
    """RSS growth between the middle and the end of an rss.csv, in KB."""
    try:
        with open(path) as f:
            pts = [(int(r["elapsed_s"]), int(r["rss_kb"])) for r in csv.DictReader(f)]
    except (OSError, ValueError, KeyError):
        return None
    if len(pts) < 4:
        return None
    return pts[-1][1] - pts[len(pts) // 2][1]


# ---------------------------------------------------------------- mini_redis

def mini_redis(base, s, md):
    d = os.path.join(base, "mini_redis")
    if not os.path.isdir(d):
        return
    md.append("\n### mini_redis vs redis-server\n")
    for phase in ("basic", "pipeline", "clients", "keyspace_fill", "keyspace_get"):
        tables = {}
        for impl in ("redis", "gem"):
            p = os.path.join(d, f"{phase}.{impl}.csv")
            if not os.path.exists(p):
                continue
            with open(p) as f:
                tables[impl] = OrderedDict((r["test"], r) for r in csv.DictReader(f))
        if len(tables) < 2:
            continue
        rows = []
        for test, rr in tables["redis"].items():
            rg = tables["gem"].get(test)
            if rg is None:
                continue
            for impl, r in (("redis", rr), ("gem", rg)):
                s.add("mini_redis", f"{phase}/{test}", impl, "rps", float(r["rps"]))
                s.add("mini_redis", f"{phase}/{test}", impl, "p50_ms", float(r["p50_latency_ms"]))
                s.add("mini_redis", f"{phase}/{test}", impl, "p99_ms", float(r["p99_latency_ms"]))
            rows.append([test, float(rr["rps"]), float(rg["rps"]), float(rg["rps"]) / float(rr["rps"]),
                         float(rr["p99_latency_ms"]), float(rg["p99_latency_ms"])])
        md.append(f"\n{phase}:\n")
        md.append(md_table(["test", "redis rps", "gem rps", "gem/redis", "redis p99 ms", "gem p99 ms"], rows))
    text = read_text(os.path.join(d, "summary.txt")) or ""
    mem = re.findall(r"^(.+?)\s+redis\s+(\d+) KB \((\d+) keys\)\s+gem\s+(\d+) KB \((\d+) keys\)", text, re.M)
    if mem:
        rows = []
        for label, rkb, rkeys, gkb, gkeys in mem:
            label = label.strip()
            s.add("mini_redis", f"memory/{label}", "redis", "rss_kb", int(rkb))
            s.add("mini_redis", f"memory/{label}", "redis", "keys", int(rkeys))
            s.add("mini_redis", f"memory/{label}", "gem", "rss_kb", int(gkb))
            s.add("mini_redis", f"memory/{label}", "gem", "keys", int(gkeys))
            rows.append([label, int(rkb) / 1024, int(rkeys), int(gkb) / 1024, int(gkeys)])
        md.append("\nMemory (RSS MB and DBSIZE after each phase):\n")
        md.append(md_table(["point", "redis MB", "redis keys", "gem MB", "gem keys"], rows))
    pub = re.findall(r"^\s+(redis|gem)\s+(\d+),(\d+),(\d+),([\d.]+),([\d.]+),([\d.]+)", text, re.M)
    if pub:
        rows = []
        for impl, subs, msgs, payload, pps, dps, secs in pub:
            case = f"pubsub/{subs}_subs"
            s.add("mini_redis", case, impl, "publish_per_s", float(pps))
            s.add("mini_redis", case, impl, "delivered_per_s", float(dps))
            s.add("mini_redis", case, impl, "seconds", float(secs))
            rows.append([f"{subs} subs × {msgs} msgs", impl, float(pps), float(dps), float(secs)])
        md.append("\nPub/sub fan-out:\n")
        md.append(md_table(["case", "impl", "publish/s", "delivered/s", "seconds"], rows))


# ---------------------------------------------------------------- stomp

STOMP_METRICS = {
    "fanout": ["publish_msgs_per_s", "delivered_msgs_per_s", "fanout_elapsed_s",
               "median_first_msg_latency_s", "p100_first_msg_latency_s", "complete_subs", "n_errors"],
    "slow": ["msgs_sent", "publish_msgs_per_s", "fast_subs_min", "slow_subs_max", "n_errors"],
    "queue": ["publish_msgs_per_s", "total_elapsed_s", "delivered_total", "fairness_ratio",
              "n_publisher_errors"],
    "soak": ["msgs_sent", "delivered_total", "expected_total", "min_received", "n_errors"],
}


def stomp(base, s, md):
    d = os.path.join(base, "stomp")
    if not os.path.isdir(d):
        return
    log = read_text(os.path.join(base, "stomp.txt")) or ""
    rows = []
    for phase, metrics in STOMP_METRICS.items():
        text = read_text(os.path.join(d, f"{phase}.json"))
        if text is None:
            continue
        try:
            j = json.loads(text)
        except ValueError:
            rows.append([phase, "harness output is not JSON", "", ""])
            continue
        shown = []
        for m in metrics:
            if m in j:
                s.add("stomp", phase, "gem", m, j[m])
                shown.append(f"{m}={fmt(j[m])}")
        rss = parse_rss_summary(read_text(os.path.join(d, f"{phase}.rss.summary.txt")) or "")
        for k, v in rss.items():
            s.add("stomp", phase, "gem", k, v)
        status = re.search(rf"=== phase: {phase} ===.*?broker status: (.+?)$", log, re.S | re.M)
        rows.append([phase, ", ".join(shown),
                     f"{rss['rss_peak_kb'] / 1024:.0f} / {rss['rss_end_kb'] / 1024:.0f}" if "rss_peak_kb" in rss else "",
                     status.group(1).strip() if status else ""])
    if rows:
        md.append("\n### stomp_broker (one fresh broker per phase)\n")
        md.append(md_table(["phase", "harness", "RSS MB peak / end", "broker"], rows))


# ---------------------------------------------------------------- main

def summarize(base):
    s = Summary()
    md = ["# Benchmark baseline", ""]
    meta = read_text(os.path.join(base, "meta.txt"))
    if meta:
        md += ["```", meta.rstrip(), "```"]
    sections = read_text(os.path.join(base, "sections.txt"))
    if sections:
        md += ["", "Sections (status, wall time):", "", "```", sections.rstrip(), "```"]
    md.append("\n## Batch programs\n")
    md.append("Medians over the timed runs; spread is (max − min) / median of wall time; "
              "peak RSS is the highest of the runs. gem/impl > 1 means Gem is slower.")
    batch(base, s, md)
    jobqueue(base, s, md)
    md.append("\n## Servers\n")
    md.append("One run each; the load generator runs on the same machine.")
    bookmark(base, s, md)
    mini_redis(base, s, md)
    stomp(base, s, md)

    with open(os.path.join(base, "summary.csv"), "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["bench", "case", "impl", "metric", "value"])
        w.writerows(s.rows)
    with open(os.path.join(base, "summary.md"), "w") as f:
        f.write("\n".join(md) + "\n")
    print(f"wrote {base}/summary.csv ({len(s.rows)} rows) and {base}/summary.md")


def load_summary(base):
    with open(os.path.join(base, "summary.csv")) as f:
        return OrderedDict(((r["bench"], r["case"], r["impl"], r["metric"]), r["value"])
                           for r in csv.DictReader(f))


COMPARED = {"wall_s", "instructions", "maxrss_kb", "footprint_kb", "req_per_s", "p50_ms", "p99_ms",
            "rps", "throughput_jobs_per_s", "latency_p99_ms", "tick_lag_ms", "peak_rss_mb",
            "rss_peak_kb", "rss_end_kb", "rss_kb", "publish_per_s", "delivered_per_s",
            "publish_msgs_per_s", "delivered_msgs_per_s", "total_elapsed_s", "fanout_elapsed_s"}


def compare(old, new):
    a, b = load_summary(old), load_summary(new)
    print(f"| bench | case | impl | metric | {os.path.basename(old.rstrip('/'))} | "
          f"{os.path.basename(new.rstrip('/'))} | change |")
    print("|---|---|---|---|---:|---:|---:|")
    for key, nv in b.items():
        if key[3] not in COMPARED or key not in a:
            continue
        try:
            x, y = float(a[key]), float(nv)
        except ValueError:
            continue
        change = f"{100 * (y - x) / x:+.1f}%" if x else ""
        better = (y > x) == (key[3] in HIGHER_IS_BETTER)
        mark = "" if not x or abs(y - x) / x < 0.05 else (" ✓" if better else " ✗")
        print(f"| {' | '.join(key)} | {fmt(x)} | {fmt(y)} | {change}{mark} |")


def main():
    args = sys.argv[1:]
    if len(args) == 3 and args[0] == "--compare":
        compare(args[1], args[2])
    elif len(args) == 1:
        summarize(args[0])
    else:
        print(__doc__, file=sys.stderr)
        sys.exit(2)


if __name__ == "__main__":
    main()
