# Handoff: correctness fixes, best-practices doc, std modernization

Notes for the next session. Delete this file once its work is done.

## Goal

Modernize `std/` against a verified best-practices guide. Order agreed
with the maintainer:

1. Land the compiler and runtime fixes on `main`.
2. Bring `docs/BEST_PRACTICES.md` up to date with the fixed behavior,
   review it until clean, and merge it to `main`.
3. Then (fresh session) modernize std module by module.

## Ground rules from the maintainer

- **Don't trust docs, including SPEC.md.** Several of its claims were
  stale. Verify behavior by running code against `build/gem`.
- **No AI attribution** in commits, PR descriptions or comments (see
  CLAUDE.md "Commits and Pull Requests"). GitHub may append its own footer
  to comments; that's accepted.
- **Fix bugs in subagents** without inherited context, in their own git
  worktree, one branch per fix, each going to `main` as its own PR.
- **Review docs adversarially:** after each revision, spawn a fresh
  subagent with no context to verify every claim by running code. Repeat
  until it finds nothing substantive.
- **macOS check:** the maintainer runs `make test` on macOS arm64 before
  merging runtime changes. CI covers only Linux x86_64 and arm64.

## State of branches

| Branch | What | State |
|---|---|---|
| `main` | includes #26: stack overflow contained to its process, 8 MB mmap'd stacks with guard pages | merged |
| `arena-reset-safety` | `compiler-fixes` (TCO for default/rest/destructured params, captured-param fixes) + per-process module globals + region-based arena resets, merged with `main`; examples renumbered 112–119 | PR open, see below; needs macOS check |
| `std-modernize` | `docs/BEST_PRACTICES.md` draft (3 review rounds), CLAUDE.md attribution rule, this file | needs the doc update below, then a PR |

`compiler-fixes` must not merge alone: on its own it routes default-param
functions through the old, unsafe reset and makes a working program
segfault. It is fully contained in `arena-reset-safety`.

## What `arena-reset-safety` changes (verify, don't trust)

- **Region resets.** Every loop (`while`, `for`), TCO function and
  mutual-TCO trampoline takes an arena mark when it starts. At its
  back-edge it copies only what is reachable from memory allocated since
  the mark. Soundness rests on a write barrier: any runtime path that
  stores into an existing table must call `gem_table_written` (recorded in
  CLAUDE.md). The process-tail analysis, the depth-2 fence and the pcall
  skip are gone.
- **Hysteresis.** The next reset waits for max(1 MB, 2 × copied +
  scanned) bytes, so large live state no longer gets copied on every
  iteration.
- **Per-process module globals (Erlang-style).** A spawned process gets a
  deep copy of its parent's module state; writes stay local; closures read
  live values. The compiler prints a `note:` for writes to module state
  in code reachable from a spawn.
- **Measured before → after:**
  - top-level 5,000-row build: 17 s → 6 ms;
  - gen_server holding 5,000 records: 6 s → 14 ms per 1,000 calls;
  - non-tail or pcall-wrapped server loop: GBs → about 8 MB;
  - self-compile: 3.1 s → 3.8 s, but peak memory 1.36 GB → 117 MB;
  - spawn: about +4 µs when std modules are loaded.

## Next steps for the new session

1. **Land the reset PR.** Once the maintainer has run it on macOS, check
   CI and merge it (squash, matching repo history).
2. **Update the doc** on `std-modernize`: merge `main` first, then
   rewrite against the merged compiler:
   - Delete or rewrite every rule marked **(bug)** whose bug is fixed:
     TCO with default/rest/destructured params, capturing a param of a
     tail-recursive fn, the whole "How memory is reclaimed" group
     (non-tail calls, pcall, 1 MB live data, building in top-level loops),
     stack depth in spawned processes (now 8 MB, overflow is a catchable
     error), module globals (now per-process).
   - Rewrite "Module-level `let`" for per-process semantics. Shared
     *mutable* state still belongs in a process (gen_server), since a
     write is now invisible to other processes.
   - Update the trap index to match.
   - Re-measure any number you keep.
   - Then run fresh adversarial reviews until clean, and open a PR to
     `main` for the doc + CLAUDE.md. Link the doc from CLAUDE.md and
     `docs/CHEATSHEET.md`.
3. **Modernize std** (fresh session) against the merged doc. Known std
   bugs and gaps to fix there:
   - `dynamic_supervisor`: `delete(state.children, idx)` leaves a hole in
     the array, so `terminate_child` of any child but the last crashes the
     supervisor and hangs the caller. Use `remove_at`.
   - `json`: `max_depth` was 128; now that stacks are 8 MB, check it is
     still a sensible cap (parse depth ~2,000 is safe now).
   - `test.assert_eq` compares tables by identity, so equal arrays fail.
     Add deep equality.
   - `request`: reads with no timeout; an fd leaks if `tcp_write` raises.
   - `http`: `start` returns a bare pid while other `start`s return
     `{pid}`; the acceptor doesn't catch "process table full"; the
     `let fd = client_fd` / `let rr = router_ref` copies before spawn
     should go.
   - `gen_server.call` and `supervisor.which_children` reject the `{pid}`
     handle their own `start` returns. `gen_server.call` to a dead server
     waits the full timeout; monitor the target like `std/task` does.
     `call` takes `...rest` for its timeout, which should be a default
     param.
   - `supervisor.which_children` and the `dynamic_supervisor` calls wait
     with no `after`.
   - Private message tags without a `_` prefix (`gs_reply`,
     `start_child`, ...) land in user mailboxes.
   - `std/test` keeps cases in module globals with index-append and `_`
     prefixes.
   - Index loops over `keys()` throughout `http`, `url`, `mime`,
     `request`, `string`, `json`; `+` chains instead of interpolation.
   - `string.split` and `string.index_of` are Gem byte loops; consider C
     builtins (log in OPTIMIZATIONS.md first).

## Known compiler and runtime issues (not fixed, not blocking)

Move these to ROADMAP.md / OPTIMIZATIONS.md or fix them as separate PRs:

- **Parameter shadowing:** `let n = n - 1` on a parameter is a C
  "redefinition" error in normal functions, and reads nil in TCO
  functions.
- **Closure-written global as last statement:** assigning such a global
  as a function's last statement gives a C type error. Check whether
  per-process globals fixed it.
- **Undefined names in closure bodies** surface as C "undeclared" errors,
  not Gem diagnostics.
- **`break`/`continue` inside a `do` block** are caught only by the C
  compiler.
- **Defining a builtin name** (`fn error`, `fn len`) silently replaces
  the builtin in a loaded module and is silently ignored in the entry
  file.
- **Stack traces** show the wrong line for a function's implicit-return
  last expression.
- **Exit reasons** are overwritten without being freed on the kill/link
  paths (small leak).
- **`keys` is O(n²)** on string-keyed tables (`gem_table_set` scans for
  an integer key equal to len).
- **`read_file` on procfs** returns `""`, because the file reports
  size 0.
- **`build_string`'s `add` closure** has a raw buffer env: sending it or
  capturing it in a spawn crashes the deep copy.
- **`fn main` runs automatically**; calling `main()` as well runs it
  twice. Consider a warning.
- **Mailbox backlogs** briefly double their memory during a reset; a
  separate message arena is written up in OPTIMIZATIONS.md.
- **Stomp broker connection ids:** `next_conn_id` lives in module state
  and would restart from its copied value if the acceptor were restarted.
- **SPEC.md** still had stale claims at last check (`tcp_read` timeouts
  "only from spawned processes", `tcp_write` "writes all bytes"); verify
  the rest of SPEC as you touch each area.
