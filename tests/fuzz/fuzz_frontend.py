#!/usr/bin/env python3
"""Mutation fuzzing of the whole compiler front end (make fuzz-frontend).

    tests/fuzz/fuzz_frontend.py [--iters N] [--seed S] [--jobs N] [--keep DIR]

Takes the numbered examples and tests/broken/*.gem as a corpus, mutates
them (cut, duplicate or swap lines and spans, insert keywords and
punctuation, truncate) and runs `gem --check` on each mutant. Bad source
must give compile errors (exit 1) or pass (exit 0); a compiler that raises
a runtime error of its own, dies on a signal, exits otherwise or runs past
the time limit has a bug, and the mutant is kept in --keep (default
build/fuzz_failures/) as the repro. Exit status 1 when any mutant failed.

Portable (any OS with python3); make fuzz-parser runs libFuzzer on the
lexer and parser in process, much faster, where clang has libFuzzer.
"""

import argparse
import concurrent.futures
import glob
import os
import random
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GEM = os.path.join(ROOT, "build", "gem")

TOKENS = ["fn", "end", "let", "if", "elif", "else", "then", "while", "for", "in", "do",
          "match", "when", "receive", "after", "return", "break", "continue", "not",
          "and", "or", "nil", "true", "false", "load", "export", "extern", "spawn",
          "(", ")", "[", "]", "{", "}", ",", ".", ":", "=", "==", "+=", "^", "|", "...",
          "\"", "'", "\"{", "}\"", "#", "##", "\"\"\"", "0x", "1e", "-", "\\", "\n", "\t",
          "__x", "x", "1", "0.5", "\"s\"", "fn(x) x end", "do |a, b|", "when {a: b}"]


def mutate(r, src):
    lines = src.split("\n")
    for _ in range(r.randrange(1, 4)):
        op = r.randrange(8)
        if not lines:
            lines = [""]
        i = r.randrange(len(lines))
        if op == 0:
            del lines[i]
        elif op == 1:
            lines.insert(r.randrange(len(lines) + 1), lines[i])
        elif op == 2:
            j = r.randrange(len(lines))
            lines[i], lines[j] = lines[j], lines[i]
        elif op == 3 and lines[i]:
            a = r.randrange(len(lines[i]))
            b = r.randrange(a, len(lines[i]) + 1)
            lines[i] = lines[i][:a] + lines[i][b:]
        elif op == 4:
            s = lines[i]
            p = r.randrange(len(s) + 1)
            lines[i] = s[:p] + " " + r.choice(TOKENS) + " " + s[p:]
        elif op == 5:
            lines = lines[:i]
        elif op == 6 and lines[i]:
            p = r.randrange(len(lines[i]))
            lines[i] = lines[i][:p] + chr(r.randrange(1, 128)) + lines[i][p + 1:]
        else:
            # a span of lines from elsewhere (another construct's middle)
            j = r.randrange(len(lines))
            lines[i:i] = lines[j:j + r.randrange(1, 6)]
    return "\n".join(lines)


def check(path):
    try:
        p = subprocess.run([GEM, "--check", path], stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=30)
    except subprocess.TimeoutExpired:
        return "timed out"
    err = p.stderr.decode("utf-8", "replace")
    if p.returncode < 0:
        return "killed by signal {}:\n{}".format(-p.returncode, err[-1500:])
    if p.returncode not in (0, 1):
        return "exit status {}:\n{}".format(p.returncode, err[-1500:])
    if "[Runtime Error]" in err or "Stack trace:" in err:
        return "the compiler raised:\n{}".format(err[-1500:])
    return None


def one(args):
    i, seed, corpus, keep, work = args
    r = random.Random(seed * 1000003 + i)
    base = r.choice(corpus)
    with open(base, encoding="utf-8", errors="replace") as f:
        src = mutate(r, f.read())
    path = os.path.join(work, "m{}_{}.gem".format(seed, i))
    with open(path, "w") as f:
        f.write(src)
    msg = check(path)
    if msg:
        os.makedirs(keep, exist_ok=True)
        shutil.copy(path, keep)
        msg = "{} (from {}): {}\n  mutant: {}".format(i, os.path.relpath(base, ROOT), msg,
                                                      os.path.join(keep, os.path.basename(path)))
    os.remove(path)
    return msg


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--iters", type=int, default=2000)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 2)
    ap.add_argument("--keep", default=os.path.join(ROOT, "build", "fuzz_failures"))
    a = ap.parse_args()
    corpus = sorted(glob.glob(os.path.join(ROOT, "examples", "[0-9]*.gem")) +
                    glob.glob(os.path.join(ROOT, "tests", "broken", "*.gem")))
    # the mutants are written to a work directory holding copies of
    # examples/modules and examples/support, the modules and C headers the
    # numbered examples name by relative path
    work = tempfile.mkdtemp(prefix="gem_fuzzfe_")
    shutil.copytree(os.path.join(ROOT, "examples", "modules"), os.path.join(work, "modules"))
    shutil.copytree(os.path.join(ROOT, "examples", "support"), os.path.join(work, "support"))
    failures = 0
    try:
        with concurrent.futures.ThreadPoolExecutor(max_workers=a.jobs) as ex:
            jobs = ((i, a.seed, corpus, a.keep, work) for i in range(a.iters))
            for msg in ex.map(one, jobs):
                if msg:
                    failures += 1
                    print("FAIL:", msg, flush=True)
    finally:
        shutil.rmtree(work, ignore_errors=True)
    print("fuzz-frontend: {} of {} mutants failed".format(failures, a.iters))
    sys.exit(1 if failures else 0)


if __name__ == "__main__":
    main()
