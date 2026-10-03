# Handoff: untrappable kill next, then runtime known bugs

Notes for the next session. Delete this file once its work is done.

## Where things stand

Merged into `main`, one squash commit each:

| PR | What | Commit |
|---|---|---|
| #28 | compiler fixes (shadowing, strict block scope, `when ... then`, ...) | 8b482c2 |
| #29 | known-bugs fixes, `docs/KNOWN_BUGS.md` | ccb251d |
| #30 | `docs/BEST_PRACTICES.md` rewritten against the compiler | e76042b |
| #31 | compiler and runtime known-bugs fixes | 5747c2b |
| #32 | std modernization + the maintainer's review round | 656340f |

The PR bodies of #28–#32 hold the detail. #32's review threads record the
decisions: `{pid}` handles everywhere, `demonitor` and request-scoped
monitors, `init` returns `{state}`, children of a stopping supervisor get
`"shutdown"`, and `nil` means "no timeout" in all of std and in `tcp_read`
(an int `<= 0` doesn't wait, like `after 0`).

## Plan agreed with the maintainer

1. **Untrappable kill + per-child shutdown timeouts** (next; its own PR
   and its own macOS run). The design is below.
2. **Runtime known-bugs batch**, if step 1 goes quickly. It can go in the
   same session as a second PR, or in the same PR if it stays small; ask
   the maintainer. The list is below.
3. Proposals, to confirm with the maintainer before starting:
   - the compiler known bugs (`docs/KNOWN_BUGS.md` "Compiler");
   - a language decision on defaults (see "Open language decision");
   - the ROADMAP items std leans on: a write timeout for `tcp_write`,
     per-monitor refs, named sqlite parameters.

## Step 1: untrappable kill and per-child shutdown

### The problem

`docs/KNOWN_BUGS.md` "A `one_for_all` restart hangs on a child that traps
exits" has the repros:

- **`one_for_all` restarts:** `restart_all` in std/supervisor.gem sends
  `kill(pid, "shutdown")` and waits for the `DOWN` with no deadline. A
  child that traps exits gets an `EXIT` message and never dies, so the
  supervisor hangs for good.
- **A supervisor's own shutdown:** `shutdown_children` in both
  supervisor.gem and dynamic_supervisor.gem kills the children in reverse
  order and waits for each `DOWN`, with one budget for the whole tree
  (`SHUTDOWN_MS = 4000`). A trapping child uses up the budget; the
  siblings after it get no wait. A nested supervisor can still be shutting
  down when its parent's budget runs out. The maintainer measured `stop`
  returning after about 4008 ms with a trapping grandchild still alive,
  and the program never exited.
- **`dynamic_supervisor.terminate_child`:** times out on such a child and
  leaves it running.

### Agreed design

Agreed in the #32 review; the maintainer's points are verbatim in
substance.

- **Untrappable kill.** `kill(pid, "kill")` can't be trapped. The target
  dies with reason `"killed"`, which is what its monitors' `DOWN` and its
  links see.
- **`"killed"` propagates as an ordinary reason.** Links propagate
  `"killed"` like any other reason, as in Erlang: a linked process that
  traps exits gets an `EXIT` with `"killed"`, and one that doesn't dies
  with `"killed"`. Only the direct `kill(pid, "kill")` is untrappable.
- **Per-child shutdown.** Child specs (supervisor children and the
  dynamic_supervisor `child` template) get `shutdown: ms`:
  - default 5000;
  - `nil` waits forever, consistent with the std timeout convention;
  - any other value raises in `start`.

  The supervisor sends `"shutdown"`, waits up to that long for the
  child's `DOWN`, then sends `"kill"` and waits for that `DOWN` too. This
  applies to:
  - a supervisor's own shutdown (each child gets its own budget, in
    reverse order; `SHUTDOWN_MS` goes away);
  - `one_for_all` restarts (`restart_all`);
  - `dynamic_supervisor.terminate_child`.
- **Supervisor children default to `shutdown: nil`**, as in OTP, where
  supervisor children default to `infinity`. Otherwise the parent's 5000
  ms default kills a nested tree in the middle of its own ordered
  shutdown. Users shouldn't have to know to set it. The maintainer's
  suggestion: `supervisor.start` / `dynamic_supervisor.start` return a
  handle that marks itself, e.g. `{pid, supervisor: true}`, and the
  parent picks the default from the handle a child spec's `start`
  returned. An explicit `shutdown:` in the spec wins.

### Points to settle while building it

Put these in the PR body under "Decisions to confirm".

- **`http.start`'s handle.** The http server also shuts down in order:
  connections first, then the acceptor. Should its handle mark itself the
  same way, so a supervised http server isn't killed mid-shutdown?
  Proposal: yes, with a generic marker that both kinds of handle use.
- **The marker key.** `supervisor: true` names one kind. A generic
  `shutdown: nil` key on the handle itself would serve the http server
  too. Pick one and document it in SPEC.
- **Timeout defaults.** `supervisor.stop` / `dynamic_supervisor.stop`
  default to 5000 ms. With per-child budgets, and nested supervisors on
  `nil`, a tree can take longer than that to stop. Should `stop` default
  to `nil`, or stay at 5000 and say so?
- **Edge cases to test:**
  - `kill(self(), "kill")`;
  - `kill(main, "kill")`: main dies and the program reports it, as for
    other reasons;
  - a process killed with `"kill"` while it waits on the thread pool;
  - a transient child that dies with `"killed"`: abnormal, so it's
    restarted;
  - `"killed"` passing through a chain of links, where trapping and
    non-trapping processes mix;
  - `kill(pid, "killed")`, which is an ordinary reason and stays
    trappable.

### Where the code is

- **runtime/gem_scheduler.c:**
  - `gem_exit_builtin` (the `kill` builtin): the trap_exit check decides
    `EXIT` message vs. termination;
  - `gem_propagate_exit` (link propagation; `lproc->trap_exit`);
  - `gem_exit_self` (for `kill(self(), ...)`);
  - the "main process killed" paths (`gem_report_main_killed`).
- **std/supervisor.gem:** `restart_all`, `wait_down`,
  `shutdown_children`, `SHUTDOWN_MS`, child spec validation (search
  `supervisor.start: `).
- **std/dynamic_supervisor.gem:** `shutdown_children`, `wait_down`, the
  `_dsup_terminate_child` handler, the `child` template validation.
- **std/http.gem:** `shut_down` and `start`'s returned handle (if the
  marker applies).

### Docs to update in the same PR

- **SPEC:**
  - `kill` (around "`kill(pid, reason)` sends an exit signal");
  - Process Linking (the trap_exit bullets);
  - the supervisor and dynamic_supervisor sections. Search "4000",
    "traps exits" and "There is no untrappable exit signal".
- **KNOWN_BUGS:** delete the whole "A `one_for_all` restart hangs on a
  child that traps exits" entry (the Standard library section becomes
  empty).
- **BEST_PRACTICES:**
  - delete the **(bug)** bullet under "Working around std today" (the
    section may go entirely) and its trap-index row, "Supervised child
    that traps exits **(bug)**";
  - add the new rules: `"kill"` is untrappable, and a child that traps
    exits gets `"shutdown"`, then is killed after its `shutdown` budget.

  Note: that bullet's wording is already stale. It says a child gets the
  supervisor's error message on restart intensity, but since #32 it gets
  `"shutdown"`.
- CHEATSHEET (`kill`, the supervisor child spec), README if it shows a
  child spec, ROADMAP if anything moves, and the std file header comments.

### Tests

- A new numbered example (next free slot: **192**) covering:
  - the KNOWN_BUGS repros, which must now finish;
  - `"killed"` through links and monitors;
  - per-child budgets, `nil` and the supervisor-child default;
  - the edge cases above.
- Expect output changes in the examples that cover trapping children and
  shutdown (174, 184, 187, 188, 189 use `trap_exit`). Check every changed
  line is intended.

## Step 2: runtime known-bugs batch

Re-run each repro in `docs/KNOWN_BUGS.md` "Runtime" before fixing it.
Suggested order, worst first (silent wrong results before crashes):

1. **`sort` keeps the comparator in a global shared by all processes**
   (`gem_sort_cmp_fn_global` in runtime/gem_builtins_collection.c). A
   nested sort, or a comparator with a loop while another process sorts,
   gives wrong or unsorted results with no error. Keep the comparator per
   call, saved across a yield.
2. **String table keys stop at the first NUL** (stb_ds `shput`/`shgeti`
   in runtime/gem_core.c hash with strlen). It affects json, url,
   `table.unique`, `mime.lookup` and `test.assert_eq`. Hash and compare by
   `slen`. Mind the remembered log, snapshot units and copies, which
   touch `str_index` (CLAUDE.md "Per-iteration arena reset" and "Module
   globals").
3. **`INT64_MIN / -1` kills the program on x86-64** (SIGFPE). Check the
   `%` path too.
4. **A non-integer `after` timeout is read as raw bits; a huge one
   expires at once.**
5. **sqlite:**
   - empty SQL;
   - placeholders in `sqlite_exec`;
   - several statements;
   - TEXT values stop at a NUL (use `sqlite3_column_bytes`).
6. **`s = s + x` in a loop skips the `+` type check**, and **printing a
   table cuts strings at an embedded NUL.**

Each fix deletes its KNOWN_BUGS entry and any **(bug)** rule plus trap-index
row in BEST_PRACTICES (the `sort` one is "`sort` inside a comparator, or a
comparator with a loop **(bug)**").

## Open language decision (step 3, ask first)

A destructured field's default fires on an explicit `nil`
(`let {a = 5} = {a: nil}` gives 5). A positional default fires only
when the argument is left out (`f(nil)` passes `nil`); SPEC §Default
parameters vs. table destructuring. Since std treats `nil` as "no
timeout", std/http and std/request need a `has_key` check (`given_nil` in
request.gem, `timeout_opt` in http.gem) to tell `{timeout_ms: nil}` from
a left-out option. Anyone writing an options-table API runs into the
same thing. The options:

- make destructuring defaults fire only on a missing key, matching
  positional defaults;
- keep the difference and add a BEST_PRACTICES rule.

The maintainer wants a decision, not a silent choice.

## Ground rules from the maintainer

- **No AI attribution** anywhere: no `Co-Authored-By` or `Claude-Session`
  trailers, no "Generated with Claude Code" lines (CLAUDE.md "Commits and
  Pull Requests"). Commit as `Samuel <samuel.savanovic@gmail.com>`:
  `git -c user.name=Samuel -c user.email=samuel.savanovic@gmail.com
  commit`.
- **GitHub footers.** The GitHub connector appends a footer to PR
  comments and bodies. After posting one, read it back and strip the
  footer: `update_issue_comment` for comments, `update_pull_request` for
  the body. Review-thread replies can't be edited, so prefer one PR
  comment plus resolving threads over per-thread replies. Post only when
  the reviewer needs an answer.
- **Branches.** One branch per change, regular pushes (no force on a
  shared branch). The maintainer squash-merges.
- **The PR body:**
  - lists "Decisions to confirm" first;
  - lists what needs a check on macOS (CI is Linux x86_64 + arm64 only);
  - lists the new known bugs found.
- **macOS review.** The maintainer reviews on macOS arm64 and runs the
  full suite, the macOS items and benchmarks. They answer open decisions
  in the PR conversation. Don't change a listed decision on your own:
  propose and ask.
- **Don't widen scope.** A bug found along the way goes into
  `docs/KNOWN_BUGS.md` with a repro checked against `build/gem` (plus a
  **(bug)** rule in BEST_PRACTICES if it's a trap users hit). A
  performance problem goes into OPTIMIZATIONS.md, a missing feature into
  ROADMAP.md.
- **Don't trust docs, including SPEC.md.** Verify by running code. Every
  rule or sample you write in BEST_PRACTICES must be run first.
- **Testing discipline** (CLAUDE.md): happy path, edge cases, adversarial
  input, `make test`.
- **Adversarial review: bounded, not "until clean".** A reviewer can
  always find something, so the stopping rule is fixed up front:
  - **At most 3 rounds** of fresh subagents with no inherited context.
    Give each round the diff, the agreed design and the build recipe, and
    split it by area (runtime, std, docs) with one reviewer per area.
  - **Each finding gets triaged:**
    - **Blocking:** a bug in what this PR changes (wrong behavior, crash,
      hang, leak, a test that doesn't test what it claims), or a doc,
      SPEC or BEST_PRACTICES claim the PR makes that is false. Fix it, and
      re-verify by running code.
    - **Not blocking:** a pre-existing bug, a problem outside the PR's
      scope, a performance idea or a style nit. Log it (KNOWN_BUGS with a
      repro, OPTIMIZATIONS, ROADMAP) or drop it. Never widen the PR for
      one.
  - **Stop** after the first round with no blocking findings, or after
    round 3, whichever comes first. A later round reviews only what the
    previous round's fixes touched, not the whole PR again.
  - **The PR body** says how many rounds ran, what they fixed, and any
    blocking finding still open after round 3, with why. The maintainer
    decides whether that blocks the merge.

## Build and test recipe (tested)

- **Building and testing.** `make build` builds `build/gem` from
  `bootstrap/stage0.c`. Then run `make test`, `make test-json` and
  `make test-lsp`; `make test-json-suite` runs too when json changes.
- **Compiler changes.** After changing `compiler/*.gem` (including a
  `BUILTIN_FNS` entry in compiler/builtins.gem for a new builtin):
  1. `build/gem compiler/main.gem -o build/gem1`
  2. `build/gem1 compiler/main.gem -o build/gem2`
  3. The `--emit-c` outputs of gem1 and gem2 must be identical.
  4. `cp build/gem2 build/gem && touch build/gem`. `make` rebuilds
     `build/gem` from stage0 whenever runtime/gem.h is newer, silently
     discarding the copy, so touch it again after editing gem.h.
- **When to bootstrap.** Only for compiler sources, or `std/string` and
  `std/json` (the compiler and LSP embed them): run `make bootstrap`, then
  `rm -rf build && make build` and check
  `build/gem compiler/main.gem --emit-c | cmp - bootstrap/stage0.c`.
  Runtime C and other std changes need no bootstrap.
- **`examples/expected_output.txt`** must stay in numeric example order
  and byte-exact. It contains CR bytes, so never rewrite it through a tool
  that normalizes newlines. The safe way to regenerate it is to replay
  `run_all.sh`'s own capture into a file, then diff and review every
  changed line before copying it over:

  ```bash
  cd /home/user/gem
  examples=$(printf '%s\n' examples/[0-9]*.gem | LC_ALL=C sort -t/ -k2,2n)
  actual=$( for f in $examples; do
      bin="/tmp/gem_$(basename "$f" .gem)"
      build/gem "$f" -o "$bin" 2>/dev/null
      "$bin" 2>&1 || true
  done )
  echo "$actual" > "$SCRATCH/actual.txt"   # then diff against expected
  ```

- **Crash reports** of spawned processes go to stderr, which run_all.sh
  captures. They include std file line numbers, so editing a std file
  shifts expected output in other examples (`at gen_server.loop
  (std/gen_server.gem:106)`).
- **TCP ports.** Examples that use TCP need unique ports. Grep `examples/`
  for the number before picking one (18190–18192, 19191–19196 and
  19790+ are among those taken).
- **Parallel worktrees.** The compiler writes `/tmp/gem_<base>.c`, so two
  worktrees can't build at once with a shared `/tmp`. Give each one a
  private `/tmp` and network namespace:
  `unshare -m -n bash -c 'python3 lo_up.py && mount --bind
  /var/tmp/iso_<name> /tmp && make test'`. Here `lo_up.py` brings up `lo`
  with an `SIOCSIFFLAGS` ioctl setting `IFF_UP`.

## Traps found in #32

Not all are in BEST_PRACTICES; check them before relying on them.

- **Destructuring defaults fire on `nil`.** A destructured option's
  default fires on an explicit `nil` (see "Open language decision").
- **Field access in call arguments runs early (bug).** In
  `print(monitor(p), process_info(p).monitors)`, `process_info` runs
  first. It's logged in KNOWN_BUGS, with a **(bug)** rule in BEST_PRACTICES.
  Give side-effecting arguments their own statements.
- **Supervisor children start after `start` returns.** `supervisor.start`
  returns before its children run, so a test that `whereis`es a child's
  name must wait for it.
- **Braces in string literals interpolate.** `"{state}"` in a string
  literal is interpolation; write `"\{state\}"`.
- **A dead target's `DOWN` reason.** Monitoring a dead target gives
  `"noproc"` once the runtime has dropped the exit reason. Repeated calls
  to a dead server report `noproc`, not the original reason.
