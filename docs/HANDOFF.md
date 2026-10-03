# Handoff: best-practices doc, then std (compiler fixes done)

Notes for the next session. Delete this file once its work is done.

## Plan agreed with the maintainer

1. **Done.** Fix the compiler bugs listed below, merge the fixes into one
   branch, and open one PR to `main`: PR #28, squash-merged into `main` as
   8b482c2 and merged into `std-modernize`. "Ground rules", "Build and test
   recipe" and "The fixes" below are kept as a record.
2. **Done.** Known-bugs fixes: PR #29, squash-merged into `main` as ccb251d. Most of
   the list below moved to `docs/KNOWN_BUGS.md` and was fixed there.
3. **Done once its PR merges: rewrite `docs/BEST_PRACTICES.md`.** The
   review rounds found ~25 new bugs, now in `docs/KNOWN_BUGS.md`.
   Steps, kept as a record:
   1. Rebase `std-modernize` onto `origin/main` (the maintainer asked for a
      rebase here, not a merge; the branch is only theirs and the doc
      sessions', so push with `--force-with-lease`). The branch's own
      commits are the BEST_PRACTICES doc, the CLAUDE.md attribution rule and
      links, and this file; the merge commits of `main` drop out. Expect
      conflicts in CLAUDE.md. Then put the BEST_PRACTICES clause back into
      CLAUDE.md "Known Bugs Tracking" and the intro of `docs/KNOWN_BUGS.md`
      (see "Known bugs" below). `make build && make test` before pushing.
   2. Rewrite the doc against the current compiler: verify every rule and
      every code sample by running it with `build/gem`; delete the **(bug)**
      rules #28 and #29 fixed; check the remaining ones against
      `docs/KNOWN_BUGS.md` (see "For the doc session" below).
   3. **Fix the other docs as you go.** The maintainer isn't sure how
      accurate SPEC.md, CHEATSHEET.md and the rest are. Whenever writing or
      checking the doc turns up a claim in `docs/SPEC.md`,
      `docs/CHEATSHEET.md`, `CLAUDE.md`, `README.md` or a doc comment that
      disagrees with what `build/gem` does, decide which side is wrong:
      if the doc is wrong, fix it in the same PR; if the code is wrong (it
      contradicts the design or is clearly a bug), add a `docs/KNOWN_BUGS.md`
      entry with a repro and describe the current behavior in the doc only
      if users can rely on it. Keep a list of what changed for the PR body.
      Give the adversarial and blind-reader agents the same brief: report a
      wrong SPEC/CHEATSHEET claim like a wrong BEST_PRACTICES rule.
   4. **Adversarial review passes** until one comes back clean: fresh
      subagents (no inherited context) that try to break each rule: a
      program that follows the rule and still fails, a sample that doesn't
      compile or prints something else, a rule that contradicts SPEC or
      another rule, a missing trap. Fix, then run a new round.
   5. **Then a blind-reader pass:** a fresh subagent playing a competent
      developer who has never seen Gem reads the doc cold (only the doc, not
      SPEC or the code) and reports what is unclear, assumed, out of order
      or missing for writing their first real program. Fix what it finds.
      If the fixes are large, run one more adversarial round.
   6. Open a PR to `main` (no AI attribution, CLAUDE.md "Commits and Pull
      Requests").
4. **Next: compiler and runtime fix pass** (agreed after the doc review
   found many bugs). Fix every `docs/KNOWN_BUGS.md` entry outside
   "Standard library": the sections "Compiler", "Runtime",
   "C interop" and "Editor grammars" (the param-named-like-a-fn capture,
   float literal precision and `1e-06.0`, float `%g` formatting,
   integer-literal overflow, renaming destructuring, bad module names and
   same-basename modules, project root and symlinked-binary lookup,
   mangled export/import errors, `pcall` line 0, `extern include` paths,
   extern extra args, plus the older entries). `sqlite_query`'s parameter
   checks are runtime C, so they belong here too even though the entry
   sits under "Standard library". Same setup as #28/#29 ("Ground rules",
   "Build and test recipe"): one subagent per fix in its own worktree off
   `origin/main`, an integration branch, one PR. Each fix deletes its
   KNOWN_BUGS entry **and** its **(bug)** rule or trap-index row in
   `docs/BEST_PRACTICES.md`, and fixes SPEC where it describes the bug
   (e.g. `to_string` floats, `http.serve`, extern arity).
5. **Then: modernize `std/`** against the merged doc, fixing the
   "Standard library" entries of `docs/KNOWN_BUGS.md` as part of it
   (dynamic_supervisor `delete`, non-tail supervisor loops, `http.serve`
   and silent handler errors, `json.encode` int keys and big ints,
   supervisor `name:` race and `{pid}` children, `std/request` timeout and
   chunked bodies), along with the list in "For std modernization" below.

## Ground rules from the maintainer

- **Don't trust docs, including SPEC.md.** Verify behavior by running code
  against `build/gem`.
- **No AI attribution** in commits, PR descriptions or comments (CLAUDE.md
  "Commits and Pull Requests"). Check the PR body before creating it.
- **Fix bugs in subagents** without inherited context, each in its own git
  worktree off `origin/main`, one branch per fix (`fix/<name>`). Give each
  agent the bug, a complete repro, the agreed semantics, and the build
  steps below. They commit locally and don't push.
- **Then integrate:** merge every fix branch into `integrate/compiler-fixes`
  (off `origin/main`), resolve conflicts, regenerate `stage0.c` once, run
  everything, push, and open the PR.
- **Testing discipline** (CLAUDE.md): happy path, edge cases, adversarial
  input, `make test`. Each fix adds a numbered example (next free slot is
  127; give agents distinct numbers up front so merges don't collide).
- **macOS:** CI covers Linux x86_64 and arm64 only. The maintainer runs
  `make test` on macOS arm64 before merging; say in the PR what needs a
  look there.
- **Don't widen scope.** Bugs found along the way go into the PR's
  follow-up list (or ROADMAP.md / OPTIMIZATIONS.md), not into the fix.

## Build and test recipe (tested; give it to every agent)

- `make build` builds `build/gem` from `bootstrap/stage0.c`.
- After changing `compiler/*.gem`: `build/gem compiler/main.gem -o
  build/gem1`, then `build/gem1 compiler/main.gem -o build/gem2`. Keep
  them in `build/`, because the binary finds the runtime relative to
  itself. Check the fixed point: `--emit-c` output of `compiler/main.gem`
  must be identical from gem1 and gem2.
- To run `make test` with a new compiler: `cp build/gem2 build/gem && touch
  build/gem`. `make` rebuilds `build/gem` from stage0 whenever
  `runtime/gem.h` is newer, silently discarding the copy, so touch it again
  after editing gem.h.
- Fix branches must **not** commit `bootstrap/stage0.c`. The integration
  branch regenerates it once (`make bootstrap` with the merged gem2 as
  `build/gem`), then rebuilds from it (`rm -rf build && make build`) and
  checks `build/gem compiler/main.gem --emit-c | cmp - bootstrap/stage0.c`.
- `examples/run_all.sh` writes binaries to the shared `/tmp/gem_<name>`, so
  don't run `make test` in two worktrees at once.
- `expected_output.txt` must stay in numeric example order. When merging
  branches that each appended output, put the hunks in example order.

## The fixes

Repros were run on `main` after #27. Re-verify each before fixing.

### 1. `let` redeclaration → shadowing (agreed)

A second `let` of a name already in scope fails:

```gem
fn f(n)
  let n = n - 1        # C: redefinition of 'gem_v_n'
  n
end
fn g(n)
  if n == 0 then return 0 end
  let n = n - 1        # tail-recursive: right-hand n reads nil
  g(n)
end
fn h()
  let a = 1
  let a = a + 1        # C: redefinition
  a
end
```

Also: a parameter shadowed inside an `if` block reads `nil`; an outer
local shadowed in a nested block reads an uninitialized value (`type error
in +: got unknown and int`); a module global shadowed inside a function
reads `nil`.

**Semantics:** the second `let` makes a new variable. Its initializer sees
the old one, and closures made before it keep the old one. Cover TCO and
mutual-TCO functions, closures, boxed (captured and mutated) variables,
loops, and `match`/`receive` bindings.

### 2. One-line `when` arms → `when <pat> then <body>` (recommended; confirm in the PR)

`when x nil end` in `match`/`receive` reports "undeclared identifier `x`"
(sometimes a C error), and `when x then nil end` is rejected. Add `when
<pat> then <body>` for one-line arms, mirroring one-line `if ... then
... end`, and make `when x nil` without `then` a clear Gem error. The
maintainer leaned towards `then` but didn't confirm, so call it out at the
top of the PR body. New syntax means updating SPEC, CHEATSHEET and both
editor grammars (CLAUDE.md "Editor Extension Maintenance"). You can't run
Helix here: run `tree-sitter generate` and `tree-sitter parse` if
tree-sitter is installable, otherwise note it in the PR.

### 3. Reading a block `let` outside its block → Gem error

`if c then let x = 2 end; print(x)` is a C "`gem_v_x` undeclared" error,
at top level and in functions. Declared in one branch of an `if` inside a
loop and read in a later iteration, it compiles and silently reads `nil`.
Both should be a Gem "undeclared identifier" error at the read. A `let`
declared in **every** branch and read after the `if` currently works
inside functions; decide whether to keep that, and document the choice in
SPEC.

### 4. Undeclared names inside closures → Gem error

A typo inside `fn() ... end`, a `do` block, a `spawn do` body or `pcall
f(x)` (which wraps a closure) gives a C `'gem_v_<name>' undeclared` error
at an approximate line. In a named function the same typo gets a proper
Gem error. Assigning an undeclared name inside a closure fails the same
way.

### 5. Interpolation starting with a string literal

`"{"a" in t}"`, `"{"a" + "b"}"` and `"{"a" == "a"}"` fail with
`unexpected token`. `"{"a"}"`, `"{x + "b"}"` and `"{("a" in t)}"` work.
Fix the lexer/parser.

### 6. `break` / `continue` inside a `do` block → Gem error

These are caught only by the C compiler, at an approximate line. Report a
Gem error at the statement: "`break` inside a `do` block; use a `for`
loop".

### 7. Defining a builtin's name

`fn error(...)` in a loaded module silently replaces the builtin for that
module; in the entry file it is silently ignored. Make this one behavior.
Recommendation: a Gem compile error, "`error` is a builtin; pick another
name", which is the least surprising. If you find it's used intentionally
somewhere in the repo, make it consistent shadowing instead and say so in
the PR.

### Lower priority: do them only if the above are done and green

- The `note:` about module writes in spawned code prints mangled names
  (`_mod_counter_count`) and absolute paths; print `counter.count` and a
  project-relative path.
- A spawned process that dies with an uncaught runtime error prints
  nothing.
- Main blocked in `receive` with no possible sender exits 0 silently.
- A module-load cycle reports its error at the entry file.

Leave these for later and list them in the PR: `keys` is O(n²) on
string-keyed tables; `read_file` on procfs returns `""`; `build_string`'s
`add` closure can't be sent or captured in a spawn; exit reasons leak on
the kill/link paths; stack traces show the wrong line for an implicit
return.

## State of branches

| Branch | What |
|---|---|
| `main` | includes #26, #27, #28 (compiler fixes; 8b482c2) and #29 (known-bugs fixes, `docs/KNOWN_BUGS.md`; ccb251d) |
| `std-modernize` | rebased onto `main` (ccb251d); the rewritten `docs/BEST_PRACTICES.md`, the SPEC/CHEATSHEET/README fixes and new `docs/KNOWN_BUGS.md` entries from the doc review, the CLAUDE.md attribution rule and BEST_PRACTICES links, this file. Goes to `main` as the step 3 PR. |

## For the doc session (step 2), so it isn't lost

What #28 changed, each checked by running `build/gem` on small programs
after the merge:

- **Delete the rules #28 removed.** These **(bug)** rules in
  BEST_PRACTICES.md are fixed: "Declare before the `if`, assign inside"
  (keep the advice, drop the bug framing, see below); "Don't redeclare a
  name that is already in scope" (now shadowing); "Typos inside closures
  reach the C compiler"; the `break`/`continue` in a `do` block sentence
  under "Use `for`, not `table.each`"; the interpolation that starts with a
  string literal (`"{"a" in t}"` works); the load-cycle sentence in the
  module section (now a compile error); the "neither is reported" half of
  "Don't reuse builtin or module names". Update the matching rows of the
  trap index.
- **Shadowing.** A second `let` makes a new variable. Its initializer and
  any destructuring default see the shadowed binding (`let n = n - 1` gives
  4 for `n = 5`; `let {x = x + 1} = {}` sees the outer `x`). Closures made
  before it keep the old variable. A second `let` directly at module scope
  still rebinds the module slot (a fn reading it sees the new value).
- **Strict block scope.** A `let` is visible to the end of its block. A use
  after the block (including when every branch of an `if` declares it),
  before the `let`, or from a closure created before the `let` is a compile
  error `undeclared identifier`, with a hint. To use a value after a block:
  `let x = nil` before it, assign inside. Self-recursive local closures:
  `let f = nil`, then `f = fn(...) ... f(...) end`. "Declare before the
  `if`, assign inside" is now the rule, not a workaround.
- **One-line arms:** `when <pat> then <body>` (decided). A same-line arm
  body without `then` is a compile error ("expected `then` or a newline
  after the `when` pattern"). `after <ms> then <body>` works too.
- **Builtin names:** a binding named like a builtin shadows it in its own
  file, the same in the entry file and in loaded modules (std does this on
  purpose, e.g. `log.error`, `std/sqlite` `exec`). It no longer leaks into
  modules the file loads. The "module name hidden by a local" half of the
  trap (`let json = ...`) is unchanged.
- **Compile errors that used to be C errors:** typos (reads and
  assignments) inside closures, `do` blocks, `spawn do` bodies and
  `pcall f(x)`; `break`/`continue` outside a loop of their own fn (e.g.
  "`break` inside a `do` block; use a `for` loop"); `"{}"`, `"{1 +}"` and
  an unclosed `{` in interpolation.
- **Runtime:** a spawned process dying from an uncaught error prints a crash
  report with stack trace on stderr (`[Runtime Error in process <pid>]`;
  pcall-caught errors, `kill` and link deaths stay silent). Main blocked in
  `receive` with nothing able to send prints `deadlock: main process is
  waiting in receive ...` and exits 1. Load cycles are compile errors at
  the closing `load` (`load cycle: a.gem → b.gem → a.gem`).
- **What #29 changed** (check each against the doc):
  - a `while` body `let` that shadows a name the condition reads warns at
    compile time, unless the loop assigns it or (for a module-level name)
    a named fn of the file does;
  - main killed by another process (link or `kill`) prints a report and
    exits 1; `link()` to a dead process exits the caller with `noproc`
    (an `EXIT` message if it traps exits), not a catchable error;
  - traces and the pcall `stack` use `anonymous fn` and `module.fn`, the
    right line for leaf fns and last expressions, project-relative paths;
  - `build_string`'s `add` called from another process is a catchable
    error; `read_file` works on procfs/pipes and errors on a directory;
    `exec` output goes to the program's stdout in every process;
  - `gem --help`; options after the source path go to the program;
  - `extern fn` named like a builtin shadows it.
- "Module-level `let`": large module state no longer costs every spawn,
  because copies are lazy per slot. A child pays only for the slots it
  reads, on first read. Re-measure and rewrite.
- Round-1 review findings were applied. Run fresh adversarial review
  rounds until clean.

## Known bugs

The list that was here moved to `docs/KNOWN_BUGS.md` (on `main`), tracked
under the CLAUDE.md rule "Known Bugs Tracking". #29 fixed most of it; what
is left, plus bugs found while fixing, is in that file.

When rebasing onto `main`, put the BEST_PRACTICES clause back (dropped in
#29 review because the file wasn't on `main` yet): in CLAUDE.md "Known Bugs
Tracking" and the intro of `docs/KNOWN_BUGS.md`, a fix also deletes any
**(bug)** rule in `docs/BEST_PRACTICES.md` that exists because of it.

## For std modernization (step 5)

- `dynamic_supervisor`: `delete(state.children, idx)` leaves a hole, so
  `terminate_child` of any child but the last crashes the supervisor and
  hangs the caller. Use `remove_at`.
- `json`: `max_depth` is 128, and arrays one deeper parse (off by one).
  With 8 MB stacks, parse depth around 2,000 is safe. `json.encode` has no
  cap and overflows at a few thousand levels.
- `test.assert_eq` compares tables by identity; add deep equality.
- `request`: reads with no timeout; an fd leaks if `tcp_write` raises.
- `http`: `start` returns a bare pid while other `start`s return `{pid}`;
  the acceptor doesn't catch "process table full"; drop the `let fd =
  client_fd` / `let rr = router_ref` copies before spawn.
- `gen_server.call` and `supervisor.which_children` reject the `{pid}`
  handle their own `start` returns. `gen_server.call` to a dead server
  waits the full timeout: monitor the target as `std/task` does. `call`
  takes `...rest` for its timeout; use a default param.
- `supervisor.which_children` and the `dynamic_supervisor` calls wait with
  no `after`.
- Private message tags without a `_` prefix (`gs_reply`, `start_child`,
  ...) land in user mailboxes.
- `std/test` keeps cases in module globals with index-append and `_`
  prefixes.
- Index loops over `keys()` throughout `http`, `url`, `mime`, `request`,
  `string`, `json`; `+` chains instead of interpolation.
- `string.split` and `string.index_of` are Gem byte loops; consider C
  builtins (log in OPTIMIZATIONS.md first).
