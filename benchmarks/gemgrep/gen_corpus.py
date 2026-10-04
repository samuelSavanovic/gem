"""Writes the benchmark corpus: a tree of text files that look like logs
and source, the same bytes on every run.

    python3 gen_corpus.py DIR [MEGABYTES]     # default 128
"""

import os
import random
import sys

WORDS = (
    "the of and to in is it that for on with as was at by be this from or an are have not "
    "but had his they you were which one all we her she there can been has if more when will "
    "would who so no out up into than them do my about some time could only other then its "
    "request response handler server client socket connect connected connection timeout retry "
    "queue worker process message reply buffer table string value index cache store record "
    "parse token lexer scope frame stack arena region reset copy pointer handle open close read "
    "write file path dir line count error warning info debug trace user session id key name"
).split()
IDENTS = ["get_id", "set_id", "user_id", "idle", "ids", "valid", "id_map", "rx_exec", "id"]
LEVELS = ["INFO", "INFO", "INFO", "DEBUG", "WARN", "ERROR"]


def line(rng):
    kind = rng.random()
    if kind < 0.45:
        n = rng.randint(4, 18)
        return " ".join(rng.choice(WORDS) for _ in range(n))
    if kind < 0.75:
        return "%02d:%02d:%02d %s %s: %s %d" % (
            rng.randrange(24), rng.randrange(60), rng.randrange(60), rng.choice(LEVELS),
            rng.choice(WORDS), " ".join(rng.choice(WORDS) for _ in range(rng.randint(2, 8))),
            rng.randrange(100000))
    if kind < 0.95:
        return "    %s = %s(%s, %d)  # %s" % (
            rng.choice(IDENTS), rng.choice(IDENTS), rng.choice(WORDS), rng.randrange(1000),
            " ".join(rng.choice(WORDS) for _ in range(rng.randint(1, 6))))
    return ""


def main():
    root = sys.argv[1]
    megabytes = int(sys.argv[2]) if len(sys.argv) > 2 else 128
    rng = random.Random(42)
    budget = megabytes * 1024 * 1024
    written = 0
    n = 0
    while written < budget:
        d = os.path.join(root, "d%02d" % (n % 16), "s%d" % (n % 5))
        os.makedirs(d, exist_ok=True)
        size = int(rng.choice([2, 8, 32, 64, 128, 512]) * 1024 * rng.uniform(0.5, 1.5))
        lines = []
        used = 0
        while used < size:
            text = line(rng)
            if rng.random() < 0.0004:
                text += " deadbeef"
            lines.append(text)
            used += len(text) + 1
        data = ("\n".join(lines) + "\n").encode()
        with open(os.path.join(d, "f%04d.%s" % (n, rng.choice(["log", "txt", "src"]))), "wb") as f:
            f.write(data)
        written += len(data)
        n += 1
    print("%d files, %d bytes" % (n, written))


main()
