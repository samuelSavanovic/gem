#!/usr/bin/env python3
"""Differential fuzzing of generated Gem programs (make fuzz-diff).

    tests/fuzz/diff.py [--seeds A:B] [--jobs N] [--keep DIR] [--debug]

For each seed, tests/fuzz/gen_program.py writes a program; it is built
at the default -O2 and with CFLAGS=-O0 (and, with --debug, a third time
on the debug runtime), and run as
  - the -O2 build,
  - the -O2 build under GEM_RESET_STRESS=1 (every loop resets at almost
    every back-edge),
  - the -O0 build,
  - with --debug, a GEM_RUNTIME=debug build under GEM_RESET_STRESS=1
    (needs `make debug`): invariant asserts and heap checks after resets
    (budgeted: gem_debug_check_reset in runtime/gem_copy.c).
All runs must print the same and exit with the same status. A seed that
doesn't, or whose program fails to compile, is reported and its program
kept in --keep (default build/fuzz_failures/) for a repro. Exit status 1
when any seed failed.
"""

import argparse
import concurrent.futures
import os
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GEM = os.path.join(ROOT, "build", "gem")
GEN = os.path.join(ROOT, "tests", "fuzz", "gen_program.py")


def run(cmd, env=None, timeout=120):
    e = dict(os.environ)
    e.pop("GEM_RESET_STRESS", None)
    e.pop("GEM_RUNTIME", None)
    e.pop("CFLAGS", None)
    if env:
        e.update(env)
    try:
        p = subprocess.run(cmd, env=e, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=timeout)
        return p.returncode, p.stdout.decode("utf-8", "replace"), p.stderr.decode("utf-8", "replace")
    except subprocess.TimeoutExpired:
        return "timeout", "", ""


def check_seed(seed, keep, debug):
    work = tempfile.mkdtemp(prefix="gem_fuzz_")
    try:
        src = os.path.join(work, "p{}.gem".format(seed))
        with open(src, "w") as f:
            f.write(subprocess.check_output([sys.executable, GEN, str(seed)]).decode())
        builds = [("O2", {}), ("O0", {"CFLAGS": "-O0"})]
        if debug:
            builds.append(("debug", {"GEM_RUNTIME": "debug"}))
        bins = {}
        for name, env in builds:
            out = os.path.join(work, name)
            rc, _, err = run([GEM, src, "-o", out], env)
            if rc != 0:
                return fail(seed, src, keep, "doesn't compile ({}):\n{}".format(name, err[-2000:]))
            bins[name] = out
        runs = [("O2", bins["O2"], {}),
                ("O2 stress", bins["O2"], {"GEM_RESET_STRESS": "1"}),
                ("O0", bins["O0"], {})]
        if debug:
            runs.append(("debug stress", bins["debug"], {"GEM_RESET_STRESS": "1"}))
        results = []
        for name, b, env in runs:
            rc, out, err = run([b], env)
            results.append((name, rc, out, err))
        base = results[0]
        for r in results[1:]:
            if (r[1], r[2]) != (base[1], base[2]):
                return fail(seed, src, keep, "{} differs from {}: exit {} vs {}\n{}".format(
                    r[0], base[0], r[1], base[1], first_diff(base[2], r[2]) + r[3][-1500:]))
        if base[1] != 0:
            return fail(seed, src, keep, "exits with {}:\n{}".format(base[1], base[3][-1500:]))
        return None
    finally:
        shutil.rmtree(work, ignore_errors=True)


def first_diff(a, b):
    al, bl = a.splitlines(), b.splitlines()
    for i in range(max(len(al), len(bl))):
        x = al[i] if i < len(al) else "<end>"
        y = bl[i] if i < len(bl) else "<end>"
        if x != y:
            return "first difference at line {}:\n  - {}\n  + {}\n".format(i + 1, x, y)
    return ""


def fail(seed, src, keep, msg):
    os.makedirs(keep, exist_ok=True)
    shutil.copy(src, keep)
    return "seed {}: {}\n  program: {}".format(seed, msg, os.path.join(keep, os.path.basename(src)))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--seeds", default="1:50", help="A:B, the seeds A..B-1 (default 1:50)")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 2)
    ap.add_argument("--keep", default=os.path.join(ROOT, "build", "fuzz_failures"))
    ap.add_argument("--debug", action="store_true", help="also run a GEM_RUNTIME=debug build")
    a = ap.parse_args()
    lo, hi = (int(x) for x in a.seeds.split(":"))
    failures = 0
    with concurrent.futures.ThreadPoolExecutor(max_workers=a.jobs) as ex:
        for msg in ex.map(lambda s: check_seed(s, a.keep, a.debug), range(lo, hi)):
            if msg:
                failures += 1
                print("FAIL:", msg, flush=True)
    print("fuzz-diff: {} of {} seeds failed".format(failures, hi - lo))
    sys.exit(1 if failures else 0)


if __name__ == "__main__":
    main()
