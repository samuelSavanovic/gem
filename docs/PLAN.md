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
  barrier, reset roots, I/O request lifecycle, …) with no assertion, debug
  build, sanitizer or fuzzer behind them. Resets fire only after 1 MB, so
  most tests never reset. With the threshold at 1 KB all 223 examples gave
  the right output, but 117_arena_reset_pressure ran out of memory
  mappings after 108,509 resets.
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
- The work lands in four stacked PRs, each branched from the one before
  and pushed once `make test` passes for each item in it; each runs on
  macOS before it merges, in order:
  1. `plan/a-safety-net`: this plan, A1 reset stress mode, A5 rule lint,
     A6 debug runtime and sanitizers, A7 fuzzing.
  2. `plan/a-semantics`: A2 silent-semantics errors, A3 `match` raises,
     A4 data traps, A8 shorter BEST_PRACTICES.
  3. `plan/b-dev-loop`: B1 fast edit-run loop, B2 install and project
     commands, B8 error messages.
  4. `plan/b-std`: B3 script basics, B4 regex, B5 encodings, B6
     option-bag typos, B7 std gaps.

## Track A: trustworthy

The order is the order of the work.

1. **Reset stress mode.** ✓ Done (PR 1). `GEM_RESET_STRESS` (env var, read at startup)
   lowers the reset threshold so every loop resets almost every
   iteration; a `make test-stress` target runs the examples and the
   example apps under it. Root-cause 117's mapping growth first.
   *Landed:* `GEM_RESET_STRESS=<bytes>`; `make test-stress` (examples,
   example apps, and the compiler compiling itself, which must emit the
   same C); `tests/check_reset_stress.sh` in `make test`. 117's growth
   came from resets that mapped a fresh 64 KB block for their copies and
   another for the next allocations even when they copied nothing, in a
   loop that ends right after (sort's comparator, called from top-level
   code with no enclosing loop to free them). A reset that copies nothing
   maps nothing. Left: a reset that copies something leaves its copy
   block's tail (OPTIMIZATIONS, P2).
2. **Silent-semantics pass in the compiler.** Turn the silent traps into
   compile errors or warnings, each a local check on the AST:
   - a `let` that hides a local of an enclosing block of the same fn;
   - a bare name in a pattern that names a visible binding (`when C`
     with a `C` in scope: `^C` compares, a new name binds);
   - `pcall(f(x))`, the parenthesized form;
   - an effect-free expression statement (the `- 2` on its own line);
   - a fn with a `receive` that calls itself in non-tail position;
   - a module-level `let` named like a loaded module.
3. **`match` with no matching arm raises** (`no match for <value>`), as
   Erlang's `case_clause`; `else nil` gives nil.
4. **Data traps fixed at the source.** `delete(arr, i)` on an array
   index raises (use `remove_at`); `in` means has-key on every table.
5. **Rule lint in `make test`.** ✓ Done (PR 1, `tests/check_rules.sh`). A script that flags direct `->state =`,
   `gem_cur_globals[` outside gem.h, `close(` in socket code outside an
   allowlist, `mco_create`, and a builtin-list check (`BUILTIN_FNS`
   against SPEC, CHEATSHEET and both grammars).
6. **Debug runtime.** ✓ Done (PR 1: `make debug`, `make asan`,
   `GEM_RUNTIME=debug|asan`, `make test-debug`). `make debug` builds a runtime with invariant
   asserts, freed regions poisoned instead of zeroed (a dangling read
   crashes instead of reading nil), and a heap check after each reset;
   an ASan/UBSan build next to it.
7. **Fuzzing.** ✓ Done (PR 1: `make fuzz-diff`, `make fuzz-frontend`,
   `make fuzz-parser`, a quick run in `make test`). A generator of random Gem programs run differentially
   (default threshold against stress, `-O0` against `-O2`), and a
   libFuzzer harness on the lexer and parser.
8. **Shrink BEST_PRACTICES.** Every trap A removes goes, its trap-index
   row too; restated SPEC material and runtime internals move out.
   Target: under 1,000 lines.

## Track B: a daily driver

1. **Fast edit-run loop.** `gem file.gem` compiles at `-O0` and caches
   the binary by a hash of its sources, the runtime and the flags; `-o`
   keeps `-O2`.
2. **Install and project commands.** `make install PREFIX=…`; `gem new`,
   `gem test` (runs `test.gem`), `gem build` from `gem.toml`.
3. **Script basics.** `exec` that captures stdout/stderr and the status;
   a line reader for files and stdin (ROADMAP "Line-at-a-time input");
   `std/args`; `rename`, `stat`, `setenv`.
4. **`std/regex`** on POSIX `regcomp`/`regexec` in the runtime (gemgrep
   proves it).
5. **Encodings.** base64, hex, sha256/HMAC (OpenSSL is already found for
   TLS), CSV, UTF-8 helpers (code points, case mapping).
6. **Option-bag typos** (ROADMAP P1, steps 1–2): unknown keys raise, with
   "did you mean".
7. **Small std gaps** from ROADMAP "std API gaps": json `indent`, mime
   types, `time.parse`, gen_server names.
8. **Error messages:** "did you mean" for undefined names; a missing
   `end` points at the construct it closes.

## Track C: deeper on servers (after A and B)

The OPTIMIZATIONS P1s that real programs hit (fan-out syscalls, inline
caches on shapes, the selective-receive marker, resets walking kept
tables), keep-alive in std/request, IPv6, a TLS server. Let the honeypot's
and httpd's data pick the order.

## Not now

Distribution, hot code reload, `extern struct`, growable stacks, a
package manager (until there is a second user).

## Housekeeping

- ROADMAP: re-score the priorities against this plan; "Process-owned
  resources closed on exit" is down to `exec` children, and `exec`
  already uses `posix_spawn`.
- A fresh benchmark baseline (the last is 2026-10-07, before TLS, the
  resolver lane and expressions as values).
- Commit the live honeypot's and httpd's health data now and then.
