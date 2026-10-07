# gemgrep

A recursive grep on libc's POSIX regular expressions (`regcomp`,
`regexec`, `regfree` from `<regex.h>`), called through Gem's C interop.
It is the yardstick for that interop, which no other example program
exercises: a C object behind a `Ptr` handle with an explicit lifetime,
file contents passed as `Bytes`, match offsets coming back, and plain
`extern fn` calls on the hot path. `benchmarks/gemgrep/run.sh` times it
against GNU grep and the same program in Python.

```sh
cd examples/gemgrep
../../build/gem main.gem -o gemgrep
./gemgrep -n 'regcomp|regfree' rx.h
./gemgrep -c extern regex.gem walk.gem
./gemgrep -rlw Ptr .
../../build/gem test.gem              # the tests (part of make test)
```

```
27:/* macOS's regcomp reads `\<`, `\>`, `\b`, `\w`, `\s` and backreferences
36:/* 0, or the regcomp error code for rx_error. REG_NEWLINE: a range that
42:    int rc = regcomp(&r->re, pattern, flags);
47:/* The message for regcomp's error `code`: glibc's text, which is GNU
78:        regfree(&r->re);
regex.gem:8
walk.gem:3
./README.md
./gemgrep
./regex.gem
./rx.h
```

## What it supports

`gemgrep [-EFHchilnoqrsvwx] [-e PATTERN] PATTERN [FILE...]`, with the
output of `grep -E` (GNU grep 3.11 in the C locale) for these options:

| Option | Meaning |
|---|---|
| `-E` | the pattern is an extended regular expression (the default, and the only syntax) |
| `-F` | the pattern is a fixed string |
| `-e PATTERN` | the pattern, for one that starts with `-` |
| `-i`, `-w`, `-x` | ignore case; match whole words; match whole lines |
| `-v` | select the lines that don't match |
| `-c`, `-l`, `-o`, `-q` | print a count per file; the names of files with a selected line; each match; nothing |
| `-n`, `-H`, `-h` | line numbers; the file name always; never |
| `-r` | search directories recursively |
| `-s` | don't report files that can't be read |

Short options cluster (`-rin`) and may come after the pattern, `--` ends
them, and `-` is standard input. As in grep:

- File names prefix each line when there are several operands or `-r`
  found the file in a directory; `-r` with no operand searches the working
  directory and names its files without `./`. `-r` follows no symbolic
  link it finds, only those named on the command line.
- The exit status is 0 when a line is selected, 1 when none is, 2 after
  any error (even with matches), except that `-q` exits 0 at the first
  selected line.
- Errors go to stderr as `gemgrep: nope: No such file or directory`,
  `gemgrep: src: Is a directory` (a directory without `-r`),
  `gemgrep: Unmatched ( or \(` (grep's message for a bad pattern), and a
  bad command line prints grep-style usage lines.
- A file that holds a NUL byte is binary: its lines aren't printed;
  `gemgrep: FILE: binary file matches` goes to stderr instead when one is
  selected. `-c` and `-l` treat it like any file.

Differences from GNU grep:

- **The regex is libc's**, so its dialect is: no BRE mode (`-G`);
  `\<`, `\>`, `\b`, `\w`, `\s` and backreferences work as extensions
  (with `-w` and `-x` too, up to `\9` counting the groups they add); and a few edge
  patterns that GNU grep accepts are errors (`a{1` is `Unmatched \{`).
  The messages are glibc's, which are grep's, whatever the libc:
  `rx_error` holds glibc's text for the POSIX error codes. glibc's `.`
  never matches a NUL byte (`RE_DOT_NOT_NULL`).
- **On macOS**, libc's regex has the extensions above only under
  `REG_ENHANCED`, which `rx.h` sets there. It differs from glibc's in
  more places than these: `.` matches a NUL byte; an empty alternative
  (`a|`, `(|b)`) and a repeated repetition (`a**`) are errors (`empty
  (sub)expression`, `Invalid preceding regular expression`); `\d` is a
  digit and `\t`, `\n` and `\r` are control characters (glibc: the
  letters); `a{,2}` is literal text (glibc: `a{0,2}`); `+?`, `*?` and `??`
  are lazy (glibc: greedy); and the backtracking pattern under "Inline,
  not blocking" below runs for more than 30 s. The empty pattern itself
  works: `regex.compile` passes it as `()`.
- **Binary files**: grep decides from the first buffer it reads and,
  in a binary file, also ends lines at NUL bytes, so `-v` can select the
  bytes before a NUL; here a NUL anywhere makes the file binary and lines
  end only at `\n`. Devices and FIFOs under `-r` are read, not skipped.
- **Directory order**: grep walks in readdir order; gemgrep sorts each
  directory's entries by name, in byte order.
- No context lines (`-A`, `-B`, `-C`), `-L`, `-m`, `--include`, colors,
  or long options other than `--help`; patterns hold no newline.

## Layout

| File | What it holds |
|---|---|
| `main.gem` | the command: arguments, `grep.run`, the exit status |
| `args.gem` | the command line, as grep reads it |
| `walk.gem`, `fs.h` | the operands expanded to files; `lstat` from C |
| `regex.gem`, `rx.h` | the regex binding: a `regex_t` behind a `Ptr`, matching on byte ranges |
| `search.gem` | one file's contents through the regex: the lines to print |
| `grep.gem` | the run: batches of files searched in tasks, output in order |
| `test.gem` | unit tests, and whole runs on a tree the test writes under `/tmp` |

## The C side

`rx.h` and `fs.h` are headers of `static` functions that the `.gem`
files include with `extern include`; they call nothing beyond libc. What each part shows:

- **A handle with a lifetime.** `regex.compile` allocates an `rx_handle`
  holding a `regex_t` (`rx_new`) and returns it in a table, `{handle, group}`; `regex.free`
  calls `regfree` and `free` and zeroes the handle, so a second free does
  nothing and a use after it raises instead of reading freed memory. A
  bad pattern takes the error path in the right order: the message
  first, then the free, then `{ok: false, error}`. A NULL from `rx_new`
  comes back as `0`, which Gem treats as true, so it is compared with `0`.
- **Freed on every path.** A `Ptr` is an int to Gem: nothing frees the
  C object when the process holding it dies, the way nothing closes a
  socket. Each batch task compiles its own regex and runs its files
  under `pcall`, frees the regex, then re-raises.
- **One owner per handle.** `spawn` or `send` would copy the number, not
  the object, so a process freeing it would leave the other with a
  dangling pointer. The tasks get the pattern and compile
  their own (once per 32 files; it costs nothing measurable).
- **`Bytes`, not `String`, for data.** The file's contents go to `rx_exec`
  as `Bytes`, a pointer and a length, and `REG_STARTEND` makes `regexec`
  read the line by its offsets rather than up to a NUL, so a line is
  never copied and a NUL in it is a byte like any other. The pattern is a
  `String` (`const char *`): one with a NUL in it is refused up front
  rather than silently cut short.
- **Offsets back.** `regex.search` returns `{start, stop}` byte offsets
  into the string, which `-o` slices with `substr`; with `-w` the pattern
  is wrapped as `(^|[^[:alnum:]_])(PATTERN)([^[:alnum:]_]|$)` and the
  offsets are group 2's; the pattern's own backreferences are renumbered
  past the wrapper's groups (`\1` becomes `\3`), as with `-x`, which
  wraps it as `^(PATTERN)$`.
- **Static strings back.** `rx_error` returns a string literal, or
  `regerror`'s text in a `static` buffer: a plain `extern fn` copies a
  returned string and never frees it.
- **Inline, not blocking.** Matching is a plain `extern fn`: it runs on
  the scheduler thread, between two loop back-edges, so the scheduler can
  preempt a task between two lines (every `GEM_REDUCTION_LIMIT`
  back-edges), never inside a call. As an
  `extern blocking fn`, each call would go through the thread pool and
  copy its `Bytes` argument: 19,000 lines of a 1 MB file took 0.64 s
  passing each line, 1.7 s passing the whole file, against 5 ms inline.
  The cost of inline is a pathological pattern: glibc's backreferences
  backtrack, and `(a|b|ab)*\1c` on one 400-byte line of `abab...` takes
  5.4 s in a single call, during which no process runs (GNU grep: 5 ms).
  File reads use `read_file`, which runs on the pool, so a task waiting
  for its file lets the others match.

## Design

- **One `regexec` per line.** `search.text` walks the lines with `find`
  and calls `regex.test` on each line's range of the file's string. A
  helper that hands `regexec` the rest of the buffer (with `REG_NEWLINE`,
  a match stays inside a line) and returns the start of the next matching
  line is 1.4 to 2.4 times faster when matches are rare (on the corpus
  at `MB=64`: `-r handler` 0.25 s against 0.52 s, `-rn deadbeef` 0.18 against
  0.44, `-rl deadbeef` 0.09 against 0.19) and the same when most lines
  match (`-rn e`, 1.25 s), because the per-line loop then runs anyway.
  The program doesn't use one: it would need a second helper to count
  lines for `-n`, `-v` would still need the line walk, a file with no match holds the scheduler for
  the whole file in one call, and the per-line loop is the program a Gem
  user writes first.
- **Tasks for batches, output in order.** `grep.run` cuts the file list
  into batches of 32 and keeps four `task.async`s in flight, awaiting them
  in order, so the output is the same as one process would print. Since
  matching runs on the scheduler thread, the tasks don't match in
  parallel; what they gain is overlap with the file reads on the thread
  pool: 10 to 18% faster than the same batches in one process.
- **A file's output is one string.** `search.text` builds it with
  `build_string`, and `main` writes it with one `write_stdout`. `print`
  per line flushes per line (1M lines to a file: 0.6 s against 0.3 s).
- **The whole file is read at once** (`read_file`; Gem has no buffered
  reader), so memory follows the largest file: 21 MB for the 9.5 MB
  `runtime/sqlite3.c`, against 10 MB for a small file.

## Performance

`benchmarks/gemgrep/run.sh` runs eleven searches over a generated
128 MB tree of 1,092 text files and over a generated 12 MB tree shaped
like the repository's sources, through gemgrep, GNU grep and the Python
twin, and checks that the three print the same lines. On a 4-core Linux
x86_64 VM (October 2026, two runs, the last two searches over the
repository's own sources), gemgrep takes 3.2 to 13 times GNU grep's time and 0.40 to 2.3
times Python's.

| Search | Gem/grep | Gem/Python |
|---|---|---|
| `-r handler` (162,000 lines out) | 4.9–5.2 | 1.37–1.43 |
| `-ri timeout` | 6.2–6.4 | 0.82–0.88 |
| `-r 'connect(ed\|ion)\|socket'` | 3.5–3.8 | 0.76–0.84 |
| `-rw id` | 5.9–6.1 | 0.45–0.46 |
| `-rc error` | 6.3–7.5 | 1.51–1.62 |
| `-rl deadbeef` | 9.5–11 | 1.02–1.09 |
| `-rv e` | 5.2–5.9 | 0.88–1.19 |
| `-rn deadbeef` (1,046 lines out) | 7.4–7.8 | 1.29–1.41 |
| `-rn e` (2.4M lines, 191 MB out) | 4.5–4.7 | 1.35–1.63 |
| `-rn gem_table_set` on the sources | 13 | 2.0–2.3 |
| `-rin 'todo\|fixme'` on the sources | 3.2–6.1 | 0.40–0.58 |

Where it goes:

- **GNU grep doesn't run a regex per line.** It searches the buffer for
  the pattern's literal parts (Boyer-Moore) and runs a DFA only around
  candidates. Per line, gemgrep spends about 1,100 instructions in
  `regexec` and about as many on its own side: the `find`, the
  `regex.test` call and its range checks, the loop (callgrind, `-c
  deadbeef` on 163,000 lines). The extern call itself is cheap, about 60
  instructions.
- **Against Python**, gemgrep wins where the wrapped or case-folded
  pattern defeats `re`'s literal-prefix search (`-w`, `-i`, alternation)
  and loses where `re` can jump to a literal (`handler`, `deadbeef`,
  `gem_table_set`): there Python's per-line cost is lower than Gem's
  loop overhead alone.
- **Big outputs are copied by resets.** The batch's result array and
  the output being built survive each reset of the loop that grows them.
  A reset promotes what it keeps, so later resets of that loop don't copy
  it again, but the short inner loops start fresh marks whose first reset
  is full: in `-rn e`, 655 of 1,693 resets are full, and the resets copy
  1.6 GB (OPTIMIZATIONS.md, "A loop's first reset is full").

Keep this program idiomatic: it is the yardstick for C-interop and
text-processing fixes, not a place to work around them.
