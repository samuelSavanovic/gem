# gemgrep

A recursive grep on libc's POSIX regular expressions (`regcomp`, `regexec`, `regfree` from `<regex.h>`), called through Gem's C interop. It shows the parts of that interop no other example uses: a C object behind a `Ptr` handle with an explicit lifetime, file contents passed as `Bytes`, match offsets coming back, and plain `extern fn` calls on the hot path.

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
77:        regfree(&r->re);
regex.gem:8
walk.gem:2
./README.md
./gemgrep
./regex.gem
./rx.h
```

## What it supports

`gemgrep [-EFHchilnoqrsvwx] [-e PATTERN] PATTERN [FILE...]`, with the output of `grep -E` (GNU grep in the C locale) for these options:

| Option | Meaning |
|---|---|
| `-E` | extended regular expression (the default and the only syntax) |
| `-F` | fixed string |
| `-e PATTERN` | the pattern, for one that starts with `-` |
| `-i`, `-w`, `-x` | ignore case; match whole words; match whole lines |
| `-v` | select the lines that don't match |
| `-c`, `-l`, `-o`, `-q` | a count per file; the names of matching files; each match; nothing |
| `-n`, `-H`, `-h` | line numbers; always the file name; never the file name |
| `-r` | search directories recursively |
| `-s` | don't report unreadable files |

Short options cluster (`-rin`) and may come after the pattern, `--` ends them, and `-` is standard input. As in grep:

- File names prefix each line when there are several operands or `-r` found the file in a directory. `-r` with no operand searches the working directory and names its files without `./`. `-r` follows symbolic links only when they're named on the command line.
- The exit status is 0 when a line is selected, 1 when none is, and 2 after any error (even with matches). `-q` exits 0 at the first selected line.
- Errors go to stderr in grep's words (`gemgrep: nope: No such file or directory`, `gemgrep: Unmatched ( or \(`), and a bad command line prints grep-style usage.
- A file containing a NUL byte is binary. Its lines aren't printed; `gemgrep: FILE: binary file matches` goes to stderr instead. `-c` and `-l` treat it like any other file.

Differences from GNU grep:

- The regex dialect is libc's. There is no BRE mode (`-G`). `\<`, `\>`, `\b`, `\w`, `\s` and backreferences work as extensions, and a few edge patterns that GNU grep accepts are errors (`a{1`). Error messages are glibc's text on every platform (`rx_error` in `rx.h`).
- On macOS the extensions need `REG_ENHANCED`, which `rx.h` sets, and the regex differs from glibc's in more places: `.` matches a NUL byte, an empty alternative (`a|`) or a repeated repetition (`a**`) is an error, `\d` is a digit, `\t`, `\n` and `\r` are control characters, `a{,2}` is literal text, and `+?`, `*?` and `??` are lazy.
- Any NUL makes a file binary, and lines end only at `\n`. grep decides from its first buffer and also splits binary files at NULs. Devices and FIFOs under `-r` are read, not skipped.
- grep walks directories in readdir order. gemgrep sorts each directory's entries by name.
- There are no context lines (`-A`, `-B`, `-C`), `-L`, `-m`, `--include`, colors, or long options other than `--help`, and patterns can't contain a newline.

## Layout

| File | What it holds |
|---|---|
| `main.gem` | the command: arguments, `grep.run`, the exit status |
| `args.gem` | the command line, as grep reads it |
| `walk.gem`, `fs.h` | expands the operands into files; `lstat` from C |
| `regex.gem`, `rx.h` | the regex binding: a `regex_t` behind a `Ptr`, matching on byte ranges |
| `search.gem` | one file's contents through the regex: the lines to print |
| `grep.gem` | the run: batches of files searched in tasks, output in order |
| `test.gem` | unit tests, and whole runs on a tree the test writes under `/tmp` |

## The C side

`rx.h` and `fs.h` are headers of `static` functions that the `.gem` files pull in with `extern include`. They only call libc.

`regex.compile` allocates an `rx_handle` holding a `regex_t` and returns it in a table, `{handle, group}`. `regex.free` calls `regfree` and `free` and zeroes the handle, so a second free does nothing and a use after it raises instead of reading freed memory. A NULL from `rx_new` comes back as `0`, which Gem treats as true, so the code compares it with `0`.

A `Ptr` is just an int to Gem. Nothing frees the C object when the process holding it dies, and `spawn` or `send` would copy the number, not the object. So each batch task compiles its own regex from the pattern, runs its files under `pcall`, frees the regex, and then re-raises any error.

File contents go to `rx_exec` as `Bytes` (a pointer and a length), and `REG_STARTEND` makes `regexec` read a line by its offsets instead of up to a NUL, so a line is never copied and a NUL in it is an ordinary byte. The pattern is a `String` (`const char *`), so one containing a NUL is refused up front. `regex.search` returns `{start, stop}` byte offsets, which `-o` slices with `substr`. `-w` wraps the pattern as `(^|[^[:alnum:]_])(PATTERN)([^[:alnum:]_]|$)` and takes group 2's offsets, and `-x` wraps it as `^(PATTERN)$`; either way the pattern's own backreferences are renumbered past the added groups.

`rx_error` returns a string literal or `regerror`'s text in a `static` buffer, because a plain `extern fn` copies a returned string and never frees it.

Matching is a plain `extern fn`, not an `extern blocking fn`, so it runs on the scheduler thread and the scheduler can only preempt a task between two lines. Sending each call through the thread pool would copy the `Bytes` argument every time and is far slower. The cost is that a pathological pattern blocks every process while it runs: glibc's backreferences backtrack, and `(a|b|ab)*\1c` on a 400-byte line of `abab...` takes seconds in a single call. File reads use `read_file`, which runs on the pool, so a task waiting for its file lets the others match.

## Design

`search.text` walks the lines with `find` and calls `regex.test` on each line's range of the file's string, one `regexec` per line. That is the loop a Gem user would write first, and `-n` and `-v` need the line walk anyway.

`grep.run` cuts the file list into batches of 32 and keeps four `task.async`s in flight, awaiting them in order, so the output matches what one process would print. Matching runs on the scheduler thread, so the tasks don't match in parallel; what they overlap is the file reads on the thread pool.

A file's output is built with `build_string` and written with one `write_stdout`, since `print` flushes on every line. Each file is read whole with `read_file`, so memory follows the largest file.

## Performance

`benchmarks/gemgrep/run.sh` runs eleven searches over a generated 128 MB tree of 1,092 text files and a generated 12 MB tree shaped like this repository's sources, through gemgrep, GNU grep and a Python twin on `re`, and checks that all three print the same lines. Wall-time ratios on an M1 Pro:

| Search | Gem/grep | Gem/Python |
|---|---|---|
| `-r handler` | 7.0 | 2.3 |
| `-ri timeout` | 17 | 2.4 |
| `-r 'connect(ed\|ion)\|socket'` | 6.6 | 2.0 |
| `-rw id` | 7.9 | 0.83 |
| `-rc error` | 8.7 | 2.6 |
| `-rl deadbeef` | 7.9 | 1.3 |
| `-rv e` | 1.6 | 0.55 |
| `-rn deadbeef` | 7.7 | 2.1 |
| `-rn e` | 1.4 | 0.78 |
| `-rn gem_table_set` on the source tree | 4.2 | 1.2 |
| `-rin 'todo\|fixme'` on the source tree | 12 | 1.9 |

On searches with few matches, such as `-rc error`, most of gemgrep's time is in libc's `regexec` (`benchmarks/gemgrep/regexec_floor.c` makes the same calls from C), and on macOS that is slower than Python's `re`. GNU grep doesn't run a regex per line at all: it searches the buffer for the pattern's literal parts and runs a DFA only around candidates.
