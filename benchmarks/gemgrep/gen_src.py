"""Writes the source-tree corpus for the `src_*` cases: a tree shaped like
this repository's compiler/, runtime/, std/, lsp/ and examples/ (flat
directories, except for app directories under examples/; about 370 files,
most of them a few KB, one C amalgamation of 9.5 MB), with C-like,
Gem-like and text lines, the same bytes on every run.

    python3 gen_src.py DIR
"""

import os
import random
import sys

# Files per top directory, the large ones below included.
TOP = [("compiler", 18), ("runtime", 20), ("std", 16), ("lsp", 11), ("examples", 304)]
EXAMPLE_DIRS = [""] * 14 + ["app", "app/static", "broker", "logs", "logs/lib", "redis",
                            "interp", "interp/bench", "grep", "jobs", "modules"]
WORDS = (
    "table value string index key len cap arena region reset copy mark block frame stack "
    "process mailbox message reply send receive spawn monitor link timer socket buffer "
    "parse lexer token scope node expr stmt emit codegen lower fold loop call return "
    "error trace line file path count first last next prev size data entry slot hash"
).split()
FNS = ["gem_table_get", "gem_arena_alloc", "gem_string", "gem_push_frame", "gem_copy_shallow",
       "gem_val_eq", "gem_table_index", "gem_proc_set_state", "gem_error", "gem_int"]
MARKS = ["TODO", "FIXME", "todo", "Todo", "fixme"]


def words(rng, lo, hi):
    return " ".join(rng.choice(WORDS) for _ in range(rng.randint(lo, hi)))


def c_line(rng):
    k = rng.random()
    ind = "    " * rng.randint(0, 3)
    if k < 0.30:
        return "%s%s(%s, %s);" % (ind, rng.choice(FNS), rng.choice(WORDS), rng.choice(WORDS))
    if k < 0.50:
        return "%sif (%s->%s == %s) return %d;" % (ind, rng.choice(WORDS), rng.choice(WORDS),
                                                 rng.choice(WORDS), rng.randrange(64))
    if k < 0.70:
        return "%s/* %s */" % (ind, words(rng, 3, 12))
    if k < 0.90:
        return "%sint %s_%s = %s;" % (ind, rng.choice(WORDS), rng.choice(WORDS), rng.randrange(4096))
    return ""


def gem_line(rng):
    k = rng.random()
    ind = "  " * rng.randint(0, 3)
    if k < 0.30:
        return "%slet %s = %s(%s)" % (ind, rng.choice(WORDS), rng.choice(WORDS), rng.choice(WORDS))
    if k < 0.45:
        return "%sif %s.%s == nil" % (ind, rng.choice(WORDS), rng.choice(WORDS))
    if k < 0.55:
        return "%send" % ind
    if k < 0.75:
        return "%s# %s" % (ind, words(rng, 3, 12))
    if k < 0.90:
        return '%sprint("%s {%s}")' % (ind, words(rng, 1, 5), rng.choice(WORDS))
    return ""


def text_line(rng):
    return words(rng, 0, 14)


def write(path, size, gen, rng):
    lines = []
    used = 0
    while used < size:
        text = gen(rng)
        r = rng.random()
        if r < 0.00016:
            text += " gem_table_set(t, k, v);"
        elif r < 0.00039:
            text += " # %s: %s" % (rng.choice(MARKS), words(rng, 2, 6))
        lines.append(text)
        used += len(text) + 1
    data = ("\n".join(lines) + "\n").encode()
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as f:
        f.write(data)
    return len(data)


def main():
    root = sys.argv[1]
    rng = random.Random(7)
    files = [("runtime/amalgamation.c", 9_500_000, c_line),
             ("runtime/amalgamation.h", 690_000, c_line),
             ("examples/expected_output.txt", 112_000, text_line),
             ("runtime/big0.c", rng.randint(40_000, 120_000), c_line),
             ("runtime/big1.c", rng.randint(40_000, 120_000), c_line),
             ("compiler/big2.gem", rng.randint(40_000, 120_000), gem_line),
             ("compiler/big3.gem", rng.randint(40_000, 120_000), gem_line)]
    i = 0
    for top, count in TOP:
        count -= sum(1 for rel, _, _ in files if rel.startswith(top + "/"))
        for _ in range(count):
            if top == "runtime":
                ext = rng.choice(["c", "h"])
            elif top == "examples":
                ext = rng.choice(["gem"] * 17 + ["md", "txt", "html"])
            else:
                ext = "gem"
            sub = rng.choice(EXAMPLE_DIRS) if top == "examples" else ""
            size = int(min(rng.lognormvariate(7.7, 1.1), 40_000))
            files.append((os.path.join(top, sub, "f%03d.%s" % (i, ext)), size, None))
            i += 1
    n = total = 0
    for rel, size, gen in files:
        if gen is None:
            ext = rel.rsplit(".", 1)[1]
            gen = c_line if ext in ("c", "h") else gem_line if ext == "gem" else text_line
        total += write(os.path.join(root, rel), size, gen, rng)
        n += 1
    print("%d files, %d bytes" % (n, total))


main()
