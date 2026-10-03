# Handoff: best-practices doc, then std (compiler fixes done)

Notes for the next session. Delete this file once its work is done.

## Plan agreed with the maintainer

1. **Done.** Fix the compiler bugs listed below, merge the fixes into one
   branch, and open one PR to `main`: PR #28, squash-merged into `main` as
   8b482c2 and merged into `std-modernize`. "Ground rules", "Build and test
   recipe" and "The fixes" below are kept as a record.
2. **Next session:** update `docs/BEST_PRACTICES.md` against the fixed
   compiler, review it until clean, and open a PR.
3. **After that:** modernize `std/` against the merged doc.

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
| `main` | includes #26 (contained stack overflow), #27 (region resets, per-process module globals copied lazily, iterative deep copy, TCO for every param kind; examples 112–126) and #28 (the compiler fixes above; examples 127–131 and 133–136, `tests/broken/*`, `tests/check_notes.sh`) |
| `std-modernize` | `docs/BEST_PRACTICES.md` (one review round applied after #27; still needs the spawn-cost rewrite, the #28 updates below and a second round), the CLAUDE.md attribution rule, the BEST_PRACTICES links in CLAUDE.md and CHEATSHEET, this file. Up to date with `main` (8b482c2 merged). |

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
- "Module-level `let`": large module state no longer costs every spawn,
  because copies are lazy per slot. A child pays only for the slots it
  reads, on first read. Re-measure and rewrite.
- Round-1 review findings were applied. Run fresh adversarial review
  rounds until clean.

## Known bugs

The list that was here moved to `docs/KNOWN_BUGS.md`, tracked under a new
CLAUDE.md rule ("Known Bugs Tracking"). Most entries were fixed on branch
`integrate/known-bugs` (off `main`; not yet merged): main killed through a
link now reports and exits 1, `link()` to a dead pid sends `noproc`, leaf
fns and last expressions report the right line, frame names are user
names (`anonymous fn`, `module.fn`) in traces and the pcall `stack`,
paths shown to the user are project-relative, a warning for a `while`
counter shadowed by a body `let`, `gem --help`, nested `"""` dedent,
`build_string`'s `add` across processes, `read_file` on procfs. Once that
branch is merged into `main`, merge `main` here, and update the doc
against it (BEST_PRACTICES rules that mention these become stale). What is
left, plus bugs found while fixing, is in `docs/KNOWN_BUGS.md`.

When merging `main` here, put the BEST_PRACTICES clause back (dropped in
#29 review because the file wasn't on `main` yet): in CLAUDE.md "Known Bugs
Tracking" and the intro of `docs/KNOWN_BUGS.md`, a fix also deletes any
**(bug)** rule in `docs/BEST_PRACTICES.md` that exists because of it.

## For std modernization (step 3)

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
