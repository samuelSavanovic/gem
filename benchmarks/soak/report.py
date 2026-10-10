#!/usr/bin/env python3
"""Summarizes a soak run directory: for each target, whether the server
stayed up, whether its memory, file descriptors and latency held steady,
and whether the load saw any wrong answers. Writes <run dir>/report.md and
prints it.

    python3 benchmarks/soak/report.py benchmarks/soak/logs/<run>

Works on a run that was cut short or is still going: it does without
what a target writes only at its end (load.csv.done, the end of
status.txt), so it can be rerun at any time.

A target's samples are split into warm-up (the first 15%, at most 10
minutes), early (the next 10% of the run) and late (the last 10%). The
steadiness checks (memory, fds, throughput, CPU, latency, backlog) use
those windows (memory: the second half's slope) and only count once a target ran for --min-judge-s (default 10 minutes); in a shorter run they are
shown as "short" ("too short" without enough samples) and the verdict
is SHORT RUN, which says the harness works,
not that the server is steady. In a run that long, a check whose early or
late window has fewer than three samples fails.

The honeypot's report also reads the database it wrote (work/data): its
crashes, sessions it lost, events its recorder dropped, and its metrics
after the load has gone (run.sh lets it settle for SETTLE_S).
"""

import argparse
import csv
import glob
import os
import sqlite3
import statistics
import sys

# Latency columns each target's load writes: interval p99s, and the probe's
# single timing per interval.
LATENCIES = {
    "mini_redis": ["lat_p99_ms", "deliver_p99_ms", "probe_ms"],
    "stomp": ["deliver_p99_ms", "job_p99_ms", "probe_ms"],
    "bookmark": ["read_p99_ms", "write_p99_ms", "probe_ms"],
    "honeypot": ["session_p99_ms", "probe_ms"],
}
# The counter whose rate per interval stands for the target's throughput.
THROUGHPUT = {"mini_redis": "ops", "stomp": "delivered", "bookmark": "reads",
              "honeypot": "canaries"}
# Counters summed over the run, shown in the totals line.
COUNTERS = {
    "mini_redis": ["ops", "conns", "published", "delivered", "sub_sessions"],
    "stomp": ["published", "delivered", "jobs_sent", "jobs_done", "conns", "sub_sessions"],
    "bookmark": ["reads", "writes", "conns"],
    "honeypot": ["canaries", "bots", "garbage", "iac", "long_lines", "drips_done", "idle_opened",
                 "resets", "halfcloses", "nonreaders"],
}
TARGETS = ("mini_redis", "stomp", "bookmark", "honeypot")
# The resources the honeypot holds with no session live: the listener and
# the database.
HONEYPOT_IDLE_RESOURCES = 2
# Open fds the settled honeypot may hold beyond its baseline.
HONEYPOT_FD_SLACK = 2
SPARK = "▁▂▃▄▅▆▇█"
# The checks that use the early and late windows (memory: the second
# half); the others (ran to the end, no errors, the honeypot's database
# checks) count however long the run was.
STEADINESS = {"memory steady", "fds steady", "queue backlog bounded", "throughput steady",
              "CPU steady"} | {
    f"{c} steady" for cols in LATENCIES.values() for c in cols}


def read_csv(path):
    if not os.path.exists(path):
        return []
    with open(path, newline="") as f:
        return [r for r in csv.DictReader(f)]


def read_kv(path):
    out = {}
    if os.path.exists(path):
        for line in open(path):
            k, sep, v = line.strip().partition("=")
            if sep:
                out[k] = v
    return out


def num(r, col):
    v = r.get(col)
    try:
        return float(v) if v not in (None, "") else None
    except ValueError:
        return None


def series(rows, col):
    return [(float(r["elapsed_s"]), num(r, col)) for r in rows if num(r, col) is not None]


def windows(points, span, judged):
    """(early, late) value lists of `points` [(t, v)], or None when too few.
    A run too short to judge (`judged` false) falls back to the first and
    last three points after warm-up; a judged run never does, so a series
    that stops early is missing data, not a comparison of other windows."""
    warm = min(600.0, 0.15 * span)
    tenth = 0.1 * span
    early = [v for t, v in points if warm < t <= warm + tenth]
    late = [v for t, v in points if t > span - tenth]
    if len(early) < 3 or len(late) < 3:
        if judged:
            return None
        after = [v for t, v in points if t > warm]
        if len(after) < 6:
            return None
        early, late = after[:3], after[-3:]
    return early, late


def slope_per_hour(points):
    """Least-squares slope of `points` in units per hour."""
    if len(points) < 3:
        return None
    xs = [t for t, _ in points]
    ys = [v for _, v in points]
    mx, my = statistics.fmean(xs), statistics.fmean(ys)
    den = sum((x - mx) ** 2 for x in xs)
    if den == 0:
        return None
    return sum((x - mx) * (y - my) for x, y in zip(xs, ys)) / den * 3600


def spark(values, width=48):
    values = [v for v in values if v is not None]
    if not values:
        return ""
    if len(values) > width:
        step = len(values) / width
        values = [max(values[int(i * step):int((i + 1) * step)] or [values[-1]])
                  for i in range(width)]
    lo, hi = min(values), max(values)
    if hi == lo:
        return SPARK[0] * len(values)
    return "".join(SPARK[min(7, int((v - lo) / (hi - lo) * 8))] for v in values)


def fmt_dur(s):
    s = int(s)
    return f"{s // 3600}h{s % 3600 // 60:02d}m" if s >= 3600 else f"{s // 60}m{s % 60:02d}s"


def target_report(name, d, args):
    lines = [f"## {name}", ""]
    checks = []  # (name, verdict, detail)
    status = read_kv(os.path.join(d, "status.txt"))
    done = read_kv(os.path.join(d, "load.csv.done"))
    load = read_csv(os.path.join(d, "load.csv"))
    server = read_csv(os.path.join(d, "server.csv"))
    if status.get("settled_s") and load:
        # The samples after the load ended (the honeypot's settling) would
        # make the late window an idle server's.
        end = float(load[-1]["elapsed_s"])
        server = [r for r in server if float(r["elapsed_s"]) <= end]
    planned = float(status.get("duration_s", "0") or 0)
    span = max([float(r["elapsed_s"]) for r in load] + [float(r["elapsed_s"]) for r in server]
               + [0.0])
    judged = span >= args.min_judge_s
    # A check without enough samples: in a judged run the data is missing.
    no_data = "FAIL" if judged else "too short"

    def few(n, what):
        return f"{n} {what}" + (", too few in the early or late window" if judged else "")

    # ── ran to the end ──
    end_reason = done.get("end_reason", "(load still running or killed hard)")
    server_end = status.get("server_end", "(not recorded)")
    sampler = status.get("sampler")
    detail = f"load: {end_reason}, {fmt_dur(span)} of {fmt_dur(planned)}; server at the end: {server_end}"
    if sampler:
        detail += f"; {sampler}"
    if end_reason == "deadline" and server_end == "alive" and not sampler:
        verdict = "PASS"
    elif end_reason in ("SIGINT", "SIGTERM", "SIGHUP") and server_end == "alive" and not sampler:
        verdict = "STOPPED"  # by hand (Ctrl-C, a closed terminal), not by a failure
    else:
        verdict = "FAIL"
    if status.get("load_hung"):
        verdict = "FAIL"
        detail += "; the load generator hung and was killed"
    checks.append(("ran to the end", verdict, detail))

    # ── errors ──
    totals = {k[len("total_"):]: int(v) for k, v in done.items() if k.startswith("total_")}
    if not totals and load:
        for r in load:
            for k, v in r.items():
                if v and (k == "errors" or k.startswith("err_")):
                    totals[k] = totals.get(k, 0) + int(float(v))
    probe_fails = sum(1 for r in load if r.get("probe_error"))
    if probe_fails:
        totals["err_probe"] = probe_fails
    errors = totals.get("errors", 0) + probe_fails
    kinds = ", ".join(f"{k[4:]}={v}" for k, v in sorted(totals.items())
                      if k.startswith("err_") and v)
    checks.append(("no errors", "PASS" if errors == 0 else "FAIL",
                   f"{errors} errors" + (f" ({kinds}), see errors.log" if kinds else "")))

    # ── memory ──
    # macOS's physical footprint where the sampler recorded it: its RSS also
    # counts pages the server has released, until the kernel takes them.
    mem_name, mem = "footprint", [(t, v / 1024) for t, v in series(server, "footprint_kb")]
    if not mem:
        mem_name, mem = "RSS", [(t, v / 1024) for t, v in series(server, "rss_kb")]
    w = windows(mem, span, judged)
    if w is None:
        checks.append(("memory steady", no_data, few(len(mem), f"{mem_name} samples")))
    else:
        early, late = statistics.median(w[0]), statistics.median(w[1])
        half = [(t, v) for t, v in mem if t >= span / 2]
        slope = slope_per_hour(half)
        growth = (slope or 0) * (span / 2) / 3600
        limit = max(args.mem_growth_mb, args.mem_growth_pct / 100 * late)
        verdict = "PASS" if growth <= limit else "FAIL"
        checks.append(("memory steady", verdict,
                       f"{mem_name} {early:.0f} MB early, {late:.0f} MB late, peak "
                       f"{max(v for _, v in mem):.0f} MB; second half {growth:+.1f} MB "
                       f"({slope or 0:+.1f} MB/h), limit {limit:.1f} MB"))

    # ── file descriptors ──
    fds = series(server, "fds")
    w = windows(fds, span, judged)
    if w is None:
        checks.append(("fds steady", no_data, few(len(fds), "fd samples")))
    else:
        early, late = statistics.median(w[0]), statistics.median(w[1])
        verdict = "PASS" if late - early <= args.fd_growth else "FAIL"
        checks.append(("fds steady", verdict,
                       f"{early:.0f} open early, {late:.0f} late, max "
                       f"{max(v for _, v in fds):.0f}, limit +{args.fd_growth}"))

    # ── throughput: the paced load keeps its rate ──
    col = THROUGHPUT.get(name)
    rate = [(float(r["elapsed_s"]), num(r, col) / num(r, "interval_s")) for r in load
            if col and num(r, col) is not None and num(r, "interval_s")]
    w = windows(rate, span, judged)
    if col and w is None:
        checks.append(("throughput steady", no_data, few(len(rate), "intervals")))
    elif col:
        early, late = statistics.median(w[0]), statistics.median(w[1])
        ok = late >= early * args.throughput_ratio
        checks.append(("throughput steady", "PASS" if ok else "FAIL",
                       f"{col} {early:,.0f}/s early, {late:,.0f}/s late, lowest interval "
                       f"{min(v for _, v in rate):,.0f}/s, limit ×{args.throughput_ratio:g}"))

    # ── CPU: the same paced work should cost the same ──
    cpu = series(server, "cpu_pct")
    w = windows(cpu, span, judged)
    if w is None:
        checks.append(("CPU steady", no_data, few(len(cpu), "CPU samples")))
    else:
        early, late = statistics.median(w[0]), statistics.median(w[1])
        ok = late <= early * args.cpu_ratio or late - early <= args.cpu_floor_pct
        checks.append(("CPU steady", "PASS" if ok else "FAIL",
                       f"{early:.1f}% early, {late:.1f}% late, limit ×{args.cpu_ratio:g} "
                       f"or +{args.cpu_floor_pct:g} points"))

    # ── latency ──
    lat_rows = []
    for col in LATENCIES.get(name, []):
        pts = series(load, col)
        w = windows(pts, span, judged)
        if w is None:
            checks.append((f"{col} steady", no_data, few(len(pts), "samples")))
            continue
        early, late = statistics.median(w[0]), statistics.median(w[1])
        worst = max(v for _, v in pts)
        ok = late <= early * args.p99_ratio or late - early <= args.p99_floor_ms
        checks.append((f"{col} steady", "PASS" if ok else "FAIL",
                       f"{early:.2f} ms early, {late:.2f} ms late "
                       f"(×{late / early if early else float('inf'):.2f}), worst interval "
                       f"{worst:.1f} ms"))
        lat_rows.append((col, [v for _, v in pts]))

    # ── stomp: the queue backlog stays bounded ──
    backlog = series(load, "queue_backlog")
    if backlog:
        w = windows(backlog, span, judged)
        if w is None:
            checks.append(("queue backlog bounded", no_data, few(len(backlog), "samples")))
        else:
            late = max(w[1])
            ok = late <= args.backlog
            checks.append(("queue backlog bounded", "PASS" if ok else "FAIL",
                           f"max {max(v for _, v in backlog):.0f} jobs, {late:.0f} in the "
                           f"late window, limit {args.backlog}"))

    if name == "honeypot":
        hp_checks, hp_lines = honeypot_report(d)
        checks += hp_checks
    else:
        hp_lines = []

    if not judged:
        checks = [(c, "short" if c in STEADINESS and v in ("PASS", "FAIL") else v, dt)
                  for c, v, dt in checks]
        lines.append(f"Ran {fmt_dur(span)}, under {fmt_dur(args.min_judge_s)}: the steadiness "
                     "checks are shown but not judged.")
        lines.append("")
    for check, verdict, detail in checks:
        lines.append(f"- **{verdict}** {check}: {detail}")
    lines.append("")
    counters = COUNTERS.get(name, [])
    parts = []
    for c in counters:
        v = totals.get(c)
        if v is None:
            v = sum(int(float(r[c])) for r in load if r.get(c))
        parts.append(f"{c} {v:,}")
    if span > 0 and parts:
        lines.append(f"Totals over {fmt_dur(span)}: " + ", ".join(parts) + ".")
        lines.append("")
    lines.append("```")
    lines.append(f"{mem_name + ' MB':<16} {spark([v for _, v in mem])}")
    if fds:
        lines.append(f"{'fds':<16} {spark([v for _, v in fds])}")
    if cpu:
        lines.append(f"{'CPU %':<16} {spark([v for _, v in cpu])}")
    for col, vals in lat_rows:
        lines.append(f"{col:<16} {spark(vals)}")
    lines.append("```")
    lines.append("")
    lines += hp_lines
    diag = [l.strip() for l in open(os.path.join(d, "server.log"), errors="replace")
            if l.startswith("gem_diag:")] if os.path.exists(os.path.join(d, "server.log")) else []
    if diag:
        lines.append("GEM_DIAG at exit:")
        lines.append("")
        lines += [f"    {l}" for l in diag]
        lines.append("")
    failed = any(v == "FAIL" for _, v, _ in checks)
    stopped = any(v == "STOPPED" for _, v, _ in checks)
    short = any(v in ("too short", "short") for _, v, _ in checks)
    return lines, ("FAIL" if failed else "STOPPED" if stopped else "SHORT RUN" if short
                   else "PASS")


def honeypot_report(d):
    """Checks and summary lines from the honeypot's weekly databases."""
    files = sorted(glob.glob(os.path.join(d, "work", "data", "honeypot-*.db")))
    if not files:
        return [("database written", "FAIL", "no honeypot-*.db in work/data")], []
    sessions, caps, crashes, metrics = {}, {}, {}, []
    inputs = 0
    for f in files:
        db = sqlite3.connect(f"file:{f}?mode=ro", uri=True)
        db.row_factory = sqlite3.Row
        for r in db.execute("SELECT coalesce(end_reason, '(open)') AS k, count(*) AS n "
                            "FROM sessions GROUP BY 1"):
            sessions[r["k"]] = sessions.get(r["k"], 0) + r["n"]
        for r in db.execute("SELECT cap AS k, count(*) AS n FROM cap_events GROUP BY 1"):
            caps[r["k"]] = caps.get(r["k"], 0) + r["n"]
        for r in db.execute("SELECT reason AS k, count(*) AS n FROM crashes GROUP BY 1"):
            crashes[r["k"]] = crashes.get(r["k"], 0) + r["n"]
        inputs += db.execute("SELECT count(*) FROM input").fetchone()[0]
        metrics += [dict(r) for r in db.execute("SELECT * FROM metrics")]
        db.close()
    metrics.sort(key=lambda m: m["time_ms"])
    checks = []
    n_crash = sum(crashes.values())
    top = sorted(crashes.items(), key=lambda kv: -kv[1])[:5]
    checks.append(("no session crashes", "PASS" if n_crash == 0 else "FAIL",
                   f"{n_crash} crashes" + "".join(f"; {n}× {k[:120]!r}" for k, n in top)))
    lost = sessions.get("lost", 0)
    checks.append(("no lost sessions", "PASS" if lost == 0 else "FAIL",
                   f"{lost} sessions ended as lost (their end never reached the recorder)"))
    if not metrics:
        checks.append(("metrics recorded", "FAIL", "no rows in the metrics table"))
        return checks, []
    dropped = sum(m["events_dropped"] or 0 for m in metrics)
    backlog = max(m["backlog"] or 0 for m in metrics)
    checks.append(("recorder kept up", "PASS" if dropped == 0 else "FAIL",
                   f"{dropped} events dropped, largest backlog {backlog} messages"))
    last = metrics[-1]
    # The sampler's first row is taken as the server starts: with no
    # session yet, its fds are the baseline.
    first = metrics[0]
    baseline = first["sessions"] == 0 and len(metrics) > 1
    fd_limit = first["fds"] + HONEYPOT_FD_SLACK if baseline else None
    settled = (last["sessions"] == 0 and last["resources_open"] <= HONEYPOT_IDLE_RESOURCES
               and (fd_limit is None or last["fds"] <= fd_limit))
    fd_note = (f"limit {fd_limit}: {first['fds']} at the start, +{HONEYPOT_FD_SLACK}" if baseline
               else "not judged: no sample before the load")
    peak = max(metrics, key=lambda m: m["sessions"] or 0)
    checks.append(("settled after the load", "PASS" if settled else "FAIL",
                   f"last sample: {last['sessions']} sessions, {last['resources_open']} resources "
                   f"open (limit {HONEYPOT_IDLE_RESOURCES}), {last['fds']} fds ({fd_note}), "
                   f"{last['procs']} processes, arena {last['memory'] / 1048576:.1f} MB; at the peak "
                   f"{peak['sessions']} sessions, {peak['procs']} processes, {peak['fds']} fds, "
                   f"arena {peak['memory'] / 1048576:.0f} MB"))
    lines = [
        "Honeypot database: sessions by end reason: "
        + ", ".join(f"{k} {v:,}" for k, v in sorted(sessions.items(), key=lambda kv: -kv[1]))
        + "; caps hit: " + (", ".join(f"{k} {v:,}" for k, v in sorted(caps.items(),
                                                                    key=lambda kv: -kv[1]))
                           or "none")
        + f"; {inputs:,} input rows; resets over 10 ms "
        + f"{sum(m['resets_over_10ms'] or 0 for m in metrics)}, over 100 ms "
        + f"{sum(m['resets_over_100ms'] or 0 for m in metrics)}; spawns refused "
        + f"{sum(m['spawn_refused'] or 0 for m in metrics)}.",
        "",
    ]
    return checks, lines


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("run_dir")
    p.add_argument("--mem-growth-mb", type=float, default=10,
                   help="allowed memory growth over the second half, MB (default 10): "
                        "the physical footprint on macOS, RSS elsewhere")
    p.add_argument("--mem-growth-pct", type=float, default=5,
                   help="... or this percent of the late memory, if larger (default 5)")
    p.add_argument("--fd-growth", type=int, default=10,
                   help="allowed rise of the median open fds, late over early (default 10)")
    p.add_argument("--p99-ratio", type=float, default=1.5,
                   help="allowed late/early ratio of an interval p99 (default 1.5)")
    p.add_argument("--p99-floor-ms", type=float, default=1.0,
                   help="... or this absolute rise in ms, if larger (default 1)")
    p.add_argument("--throughput-ratio", type=float, default=0.9,
                   help="lowest allowed late/early ratio of the throughput (default 0.9)")
    p.add_argument("--cpu-ratio", type=float, default=1.5,
                   help="allowed late/early ratio of the server's CPU %% (default 1.5)")
    p.add_argument("--cpu-floor-pct", type=float, default=10,
                   help="... or this rise in percentage points, if larger (default 10)")
    p.add_argument("--backlog", type=int, default=1000,
                   help="queue backlog limit in the late window, jobs (default 1000)")
    p.add_argument("--min-judge-s", type=float, default=600,
                   help="shortest run whose steadiness checks count (default 600)")
    args = p.parse_args()

    d = args.run_dir
    if not os.path.isdir(d):
        sys.exit(f"{d}: not a directory")
    out = ["# Soak run", ""]
    if os.path.exists(os.path.join(d, "meta.txt")):
        out += ["```"] + open(os.path.join(d, "meta.txt")).read().rstrip().split("\n") + ["```", ""]
    verdicts = []
    targets = [t for t in TARGETS if os.path.isdir(os.path.join(d, t))]
    if not targets:
        sys.exit(f"{d}: no target directories ({', '.join(TARGETS)})")
    body = []
    for t in targets:
        lines, verdict = target_report(t, os.path.join(d, t), args)
        verdicts.append((t, verdict))
        body += lines
    out.append("| target | verdict |")
    out.append("|---|---|")
    out += [f"| {t} | {v} |" for t, v in verdicts]
    out.append("")
    out += body
    text = "\n".join(out)
    with open(os.path.join(d, "report.md"), "w") as f:
        f.write(text + "\n")
    print(text)


if __name__ == "__main__":
    main()
