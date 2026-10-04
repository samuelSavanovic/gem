"""gemgrep in Python: the same options, output, walk order, exit statuses
and messages as examples/gemgrep, on Python's `re` (bytes patterns).

    python3 gemgrep.py [-EFHchilnoqrsvwx] [-e PATTERN] PATTERN [FILE...]

Python's `re` is not POSIX: the pattern is translated from an ERE first
(bracket classes like [[:alpha:]], \\< and \\>), and it matches
leftmost-first where POSIX matches leftmost-longest, so `-o` can print
a shorter match for an alternation (`a|ab` on "ab" prints "a", not
"ab"). Which lines match is the same for the ERE subset the benchmark
uses. A bad pattern reports Python's message, not libc's.
"""

import os
import re
import sys

FLAGS = {
    "i": "icase", "v": "invert", "c": "count", "l": "list", "n": "number", "w": "word",
    "x": "line", "F": "fixed", "r": "recursive", "o": "only", "q": "quiet", "s": "no_messages",
}
USAGE = "Usage: gemgrep [OPTION]... PATTERN [FILE]..."
CLASSES = {
    "alnum": "A-Za-z0-9", "alpha": "A-Za-z", "digit": "0-9", "lower": "a-z", "upper": "A-Z",
    "space": " \\t\\n\\r\\f\\v", "blank": " \\t", "punct": "!-/:-@\\[-`{-~", "xdigit": "0-9A-Fa-f",
    "cntrl": "\\x00-\\x1f\\x7f", "print": " -~", "graph": "!-~",
}
WORD = "A-Za-z0-9_"


class Usage(Exception):
    pass


def parse(argv):
    opts = {name: False for name in FLAGS.values()}
    opts.update(pattern=None, with_filename=None, help=False)
    operands = []
    i = 0
    done = False
    while i < len(argv):
        arg = argv[i]
        i += 1
        if done or arg == "-" or len(arg) < 2 or arg[0] != "-":
            operands.append(arg)
        elif arg == "--":
            done = True
        elif arg == "--help":
            opts["help"] = True
        elif arg[1] == "-":
            raise Usage("unrecognized option '%s'" % arg)
        else:
            j = 1
            while j < len(arg):
                ch = arg[j]
                j += 1
                if ch == "e":
                    if j < len(arg):
                        opts["pattern"] = arg[j:]
                    elif i < len(argv):
                        opts["pattern"] = argv[i]
                        i += 1
                    else:
                        raise Usage("option requires an argument -- 'e'")
                    break
                elif ch == "H":
                    opts["with_filename"] = True
                elif ch == "h":
                    opts["with_filename"] = False
                elif ch == "E":
                    pass
                elif ch in FLAGS:
                    opts[FLAGS[ch]] = True
                else:
                    raise Usage("invalid option -- '%s'" % ch)
    if opts["help"]:
        return opts
    if opts["pattern"] is None:
        if not operands:
            raise Usage(None)
        opts["pattern"] = operands.pop(0)
    opts["files"] = operands
    return opts


def posix_class(m):
    if m.group(1) not in CLASSES:
        raise re.error("Invalid character class name")
    return CLASSES[m.group(1)]


def translate(ere):
    """An ERE as a Python pattern: POSIX bracket classes, \\< and \\>."""
    out = []
    i = 0
    while i < len(ere):
        ch = ere[i]
        if ch == "\\" and i + 1 < len(ere):
            nxt = ere[i + 1]
            out.append("\\b" if nxt in "<>" else ere[i:i + 2])
            i += 2
        elif ch == "[":
            j = i + 1
            if j < len(ere) and ere[j] == "^":
                j += 1
            if j < len(ere) and ere[j] == "]":
                j += 1
            while j < len(ere) and ere[j] != "]":
                if ere.startswith("[:", j):
                    j = ere.index(":]", j) + 2
                else:
                    j += 1
            body = ere[i + 1:j]
            body = re.sub(r"\[:(\w*):\]", posix_class, body.replace("\\", "\\\\"))
            out.append("[" + body + "]")
            i = j + 1
        else:
            out.append(ch)
            i += 1
    return "".join(out)


def compile_pattern(opts):
    pattern = re.escape(opts["pattern"]) if opts["fixed"] else translate(opts["pattern"])
    if opts["line"]:
        pattern = "^(%s)$" % pattern
    elif opts["word"]:
        pattern = "(?:^|[^%s])(%s)(?:[^%s]|$)" % (WORD, pattern, WORD)
    else:
        pattern = "(%s)" % pattern
    flags = re.IGNORECASE if opts["icase"] else 0
    return re.compile(pattern.encode("latin-1"), flags)


def walk_dir(d, out):
    try:
        names = sorted(os.listdir(d or "."), key=os.fsencode)
    except OSError as e:
        out.append({"path": d or ".", "error": e.strerror})
        return
    for name in names:
        path = name if d == "" else (d + name if d.endswith("/") else d + "/" + name)
        if os.path.islink(path):
            continue
        if os.path.isdir(path):
            walk_dir(path, out)
        else:
            out.append({"path": path, "in_dir": True})


def files(operands, recursive):
    out = []
    if not operands:
        if recursive:
            walk_dir("", out)
        else:
            out.append({"path": "-", "in_dir": False})
        return out
    for path in operands:
        if path != "-" and os.path.isdir(path):
            if recursive:
                while len(path) > 1 and path.endswith("/"):
                    path = path[:-1]
                walk_dir(path, out)
            else:
                out.append({"path": path, "error": "Is a directory"})
        else:
            out.append({"path": path, "in_dir": False})
    return out


def search(data, rx, opts, prefix):
    binary = b"\0" in data
    quiet = opts["quiet"] or opts["list"] or (binary and not opts["count"])
    printing = not quiet and not opts["count"] and not (opts["only"] and opts["invert"])
    invert = opts["invert"]
    out = []
    count = 0
    lines = data.split(b"\n")
    if lines[-1] == b"":
        lines.pop()
    for lineno, line in enumerate(lines, 1):
        if (rx.search(line) is not None) != invert:
            count += 1
            if quiet:
                break
            if printing:
                p = prefix
                if opts["number"]:
                    p += b"%d:" % lineno
                if opts["only"]:
                    for m in rx.finditer(line):
                        if m.end(1) > m.start(1):
                            out.append(p + m.group(1) + b"\n")
                else:
                    out.append(p + line + b"\n")
    return b"".join(out), count, binary and count > 0


def main():
    try:
        opts = parse(sys.argv[1:])
    except Usage as e:
        if e.args[0] is not None:
            print("gemgrep: %s" % e.args[0], file=sys.stderr)
        print(USAGE, file=sys.stderr)
        print("Try 'gemgrep --help' for more information.", file=sys.stderr)
        sys.exit(2)
    if opts["help"]:
        print("see examples/gemgrep")
        sys.exit(0)
    try:
        rx = compile_pattern(opts)
    except re.error as e:
        print("gemgrep: %s" % e, file=sys.stderr)
        sys.exit(2)
    out = sys.stdout.buffer
    with_filename = opts["with_filename"]
    if with_filename is None and len(opts["files"]) > 1:
        with_filename = True
    selected = failed = False
    for entry in files(opts["files"], opts["recursive"]):
        path = entry["path"]
        if "error" in entry:
            err = entry["error"]
        else:
            name = "(standard input)" if path == "-" else path
            try:
                if path == "-":
                    data = sys.stdin.buffer.read()
                else:
                    with open(path, "rb") as f:
                        data = f.read()
                err = None
            except OSError as e:
                err = e.strerror
        if err is not None:
            failed = True
            if not opts["no_messages"]:
                out.flush()
                print("gemgrep: %s: %s" % (path, err), file=sys.stderr)
            continue
        show = with_filename or (with_filename is None and entry["in_dir"])
        prefix = (name + ":").encode() if show else b""
        text, count, binary = search(data, rx, opts, prefix)
        if count > 0:
            selected = True
            if opts["quiet"]:
                sys.exit(0)
        if opts["quiet"]:
            continue
        if opts["list"]:
            if count > 0:
                out.write(name.encode() + b"\n")
        elif opts["count"]:
            out.write(prefix + b"%d\n" % count)
        else:
            out.write(text)
        if binary and not opts["count"] and not opts["list"]:
            out.flush()
            print("gemgrep: %s: binary file matches" % name, file=sys.stderr)
    out.flush()
    sys.exit(2 if failed else 0 if selected else 1)


main()
