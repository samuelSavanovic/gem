# Plan: a practical Gem

The direction chosen on 2026-10-14.
The yardstick is **practical**: its owner (and later, maybe, others) can
build and run real programs without surprises and without knowing how the
runtime works. Two tracks, A before B where they compete; C comes after.

Where the project stood on 2026-10-14, in short:

- **Strong:** concurrency and staying up (every soak 0 errors; the
  honeypot live since 2026-10-11 with no runtime bug; jobqueue at or above
  Elixir), CPU speed (0.41–0.64x Python's time on lox), memory (1/18 of
  Node's on bookmark_app), error messages, docs that match the code.
- **Silent wrong results:** docs/BEST_PRACTICES.md holds about 58 things a
  user must keep in mind (48 trap-index rows, ~10 unmarked); about 60% are
  bugs, could move into the compiler or runtime, or are silent perf cliffs.
  They are written up as rules, so the doc grows (1,932 lines, longer than
  SPEC) instead of shrinking.
- **Soundness rests on memory:** ~14 hand-kept runtime invariants (write
  barrier, reset roots, I/O request lifecycle, …). Resets fire only after
  1 MB, so most tests never reset. A1 and A5–A7 below are the checks
  behind them.
- **Not yet a daily driver for scripts:** every run compiles at `-O2`
  with no cache (3.3 s for a 70-line program, 12 s for mini_redis; 0.6 s
  and 1.8 s at `-O0`); no install, `gem test` or `gem new`; no regex,
  captured `exec` output, file line reader, argument parser, base64/sha,
  CSV or UTF-8 helpers.

## Decisions (2026-10-14)

- The silent-semantics checks (A2) are **compile errors**, not warnings.
  Code in the repo that trips them (compiler, std, lsp, examples) is
  fixed in the same change.
- A `match` with no matching arm **raises** (A3); `else nil` gives nil, as
  without it today.
- `gem file.gem` builds at **`-O0` with a cache**; `gem -o` keeps `-O2`
  (B1).
- The items land in four groups, in this order:
  1. A1 reset stress mode, A5 rule lint, A6 debug runtime and
     sanitizers, A7 fuzzing.
  2. A2 silent-semantics errors, A3 `match` raises, A4 data traps, A8
     shorter BEST_PRACTICES.
  3. B1 fast edit-run loop, B2 install and project commands, B8 error
     messages.
  4. B3 script basics, B4 regex, B5 encodings, B6 option-bag typos, B7
     std gaps.

## Track A: trustworthy

The order is the order of the work.

1. **Reset stress mode.** ✓ Done. `GEM_RESET_STRESS=<bytes>` (env var,
   read at startup) lowers the reset threshold so every loop resets
   almost every iteration. `make test-stress` runs the examples, the
   example apps and the compiler compiling itself (which must emit the
   same C) under it; `tests/check_reset_stress.sh` runs in `make test`.
   A reset that copies nothing maps no block, so a loop that ends right
   after one (a sort comparator's, called from top-level code with no
   enclosing loop) leaves no mapping behind: at a 1 KB threshold,
   117_arena_reset_pressure's arena ends at 45 MB, and at 7.1 GB when
   such resets map blocks (macOS). Left: a reset that copies
   something leaves blocks behind when its loop ends right after it,
   for good when no enclosing loop resets (OPTIMIZATIONS, P2).
2. **Silent-semantics pass in the compiler.** ✓ Done (compiler/checks.gem
   and the parser; `pcall(f(x))` and the `receive` self call are
   warnings, the rest compile errors). Turn the silent traps into
   compile errors or warnings, each a local check on the AST:
   - a `let` that hides a local of an enclosing block of the same fn;
   - a bare name in a pattern that names a visible binding (`when C`
     with a `C` in scope: `^C` compares, a new name binds);
   - `pcall(f(x))`, the parenthesized form;
   - an effect-free expression statement (the `- 2` on its own line);
   - a fn with a `receive` that calls itself in non-tail position;
   - a module-level `let` named like a loaded module.
3. **`match` with no matching arm raises** ✓ Done (`no match for <value>`), as
   Erlang's `case_clause`; with `else nil`, a value no arm matches gives `nil`.
4. **Data traps fixed at the source.** ✓ Done. `x in t` tests the
   values of a non-empty array (keys exactly `0 .. n-1`, whatever the
   order they were added in) and is `has_key` on every other table, so an
   int-keyed set with other keys finds its ids. `delete` is a key delete
   on every table; `remove_at` removes an element from a list.
5. **Rule lint in `make test`.** ✓ Done (`tests/check_rules.sh`). A script that flags direct `->state =`,
   `gem_cur_globals[` outside gem.h, `close(` in socket code outside an
   allowlist, `mco_create`, and a builtin-list check (`BUILTIN_FNS`
   against SPEC, CHEATSHEET and both grammars).
6. **Debug runtime.** ✓ Done (`make debug`, `make asan`,
   `GEM_RUNTIME=debug|asan`, `make test-debug`). `make debug` builds a
   runtime with invariant asserts, the block tail a reset gives back
   poisoned (a release build leaves the old bytes there until the tail is
   reused) and freed blocks kept `PROT_NONE` for a while, so a dangling
   access reads a value of no known type or faults, and heap checks after
   resets, as many as a budget of what the resets cost allows; an
   ASan/UBSan build next to it.
7. **Fuzzing.** ✓ Done (`make fuzz-diff`, `make fuzz-frontend`,
   `make fuzz-parser`, a quick run in `make test`). A generator of random Gem programs run differentially
   (default threshold against stress, `-O0` against `-O2`), and a
   libFuzzer harness on the lexer, parser, lower and fold.
8. **Shrink BEST_PRACTICES.** ✓ Done (999 lines). Every trap A removes goes, its trap-index
   row too; restated SPEC material and runtime internals move out.
   Target: under 1,000 lines.

## Track B: a daily driver

1. **Fast edit-run loop.** ✓ Done. `gem file.gem` compiles at `-O0` and caches
   the binary by a hash of its sources, the runtime and the flags; `-o`
   keeps `-O2`.
   The key hashes the C (every loaded file's code), the cc
   command, the contents of the runtime library, the runtime headers and
   `extern include` headers;
   `GEM_CACHE=off`, `GEM_CACHE_DIR`; at most 256 entries, then emptied.
   `--` after the file passes options to the program. Left: `-l`
   libraries are not in the key.
2. **Install and project commands.** ✓ Done. `make install PREFIX=…`; `gem new`,
   `gem test` (runs `test.gem`), `gem build` from `gem.toml`.
3. **Script basics.** ✓ Done (PR 4). `exec` that captures stdout/stderr and the status;
   a line reader for files and stdin (ROADMAP "Line-at-a-time input");
   `std/args`; `rename`, `stat`, `setenv`.
   *Landed:* `exec_capture(cmd, input?)`; `read_file(path, offset,
   count)` and `read_stdin()`; `std/io` (`lines`, `reader`,
   `read_lines`); `std/args`; `string.closest`. stdin lines still come
   from `input()`, which blocks every process.
4. **`std/regex`** ✓ Done (PR 4). On POSIX `regcomp`/`regexec` in the runtime (gemgrep
   proves it).
   *Landed:* `test`, `find`, `find_all`, `replace(_first)`, `split`,
   `escape`, `check_pattern`; `\d \w \s` rewritten for POSIX so a pattern
   means the same on glibc and macOS; a 64-entry compile cache. Left:
   word boundaries and lazy quantifiers (POSIX has neither).
5. **Encodings.** ✓ Done (PR 4). base64, hex, sha256/HMAC (OpenSSL is already found for
   TLS), CSV, UTF-8 helpers (code points, case mapping).
   *Landed:* `std/base64`, `std/hex`, `std/crypto` (SHA-256 written in
   the runtime instead of OpenSSL, which is optional and linked only for
   TLS), `std/utf8`, `std/csv`. Left: full Unicode case mapping (`ß` to
   `SS`, scripts past Armenian).
6. **Option-bag typos** (ROADMAP P1, steps 1–2): unknown keys raise, with
   "did you mean".
7. **Small std gaps** from ROADMAP "std API gaps": json `indent`, mime
   types, `time.parse`, gen_server names.
8. **Error messages:** ✓ Done. "did you mean" for undefined names; a missing
   `end` points at the construct it closes.
   The block named is the first whose `end` was indented unlike its
   opening line, else the innermost open one; reported once, when a
   named `fn` at column 1 lands inside a body or the file ends.

## Track C: deeper on servers (after A and B)

The OPTIMIZATIONS P1s that real programs hit (fan-out syscalls, inline
caches on shapes, the selective-receive marker, resets walking kept
tables), keep-alive in std/request, IPv6, a TLS server. Let the honeypot's
and httpd's data pick the order.

## Not now

Distribution, hot code reload, `extern struct`, growable stacks, a
package manager (until there is a second user).

## Housekeeping

- ROADMAP: re-score the priorities against this plan.
- A fresh benchmark baseline (the last is 2026-10-07, before TLS, the
  resolver lane and expressions as values).
- Commit the live honeypot's and httpd's health data now and then.
