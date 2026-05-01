# Resume: runtime error message quality pass

Branch: `main`. Last three sessions were audits (TCO reset, runtime cleanup, extern fn soundness). The runtime is in a healthy state. The next bottleneck for a real Gem user is **debuggability**: when something goes wrong, does the message tell them *what* and *where*?

This session is open-ended quality work, not a soundness audit. Scope it tight.

## Last session in one screen

`726b0f1` — extern fn / extern blocking fn soundness audit. Fixed two bugs:
- Ptr return constructed `GemVal` directly without setting `magic`. Routed both blocking and non-blocking paths through `gem_int()`.
- Non-blocking String return crashed on NULL via `gem_string -> strlen(NULL)`. Now returns `GEM_NIL` on NULL, matching the blocking path.

Added `examples/74_extern_blocking.gem` — first in-tree caller of `extern blocking fn`. Exercises the thread-pool yield and malloc'd-string-return ownership contract via `usleep` + `strdup`.

Documented the String-return ownership asymmetry (non-blocking copies, blocking copies-and-frees), pointer lifetime across arena reset, and that `extern` does no boundary validation. Tracked deferred follow-ups (arity/type validation, ownership unification) under "C Interop Hardening" in `docs/OPTIMIZATIONS.md`.

All four validation gates green.

## This session — runtime error message quality

Goal: when a Gem program errors at runtime, the user gets a message that tells them **what went wrong** and **where in their source** — not "type error in gem_add" with no line number.

### Concrete starting point

1. **Survey the offenders.** Grep `gem_error(` across `runtime/` and triage. Worst categories likely:
   - Type errors in built-ins (`gem_add`, `gem_eq`, table access) — often print "expected X, got Y" with no source line.
   - Builtin arity/type errors — many use `gem_error("foo: expected ...")` with no call-site info.
   - Table key errors — missing key access, wrong-type indexing.
   - Some errors are just `gem_error("X")` with no formatting at all.

2. **Pick the top 5–10 by frequency-of-tripping**, not by ease-of-fixing. The print/len/type/push/index path is on every program; the math.atan2 edge is on none.

3. **Pattern**: the runtime already has `gem_call_stack` / `gem_push_frame` / `gem_pop_frame`. The stack trace printer (`gem_error.c`) uses these. The question is whether the *innermost* frame at the error site has the user's source location, or whether it's a runtime helper with no Gem-source mapping.

4. **Codegen side**: every Gem call site emits `gem_push_frame(name, file, line)` before the call. So when a builtin errors, the top of the call stack has the call site. Verify this is being printed clearly when a builtin errors — and if not, fix it.

### What good looks like

A user who writes `let x = "hello" + 5` should see something like:

```
[Error] cannot add String and Int
  at examples/foo.gem:3:13
    let x = "hello" + 5
                    ^
```

Not:

```
[Error] gem_add: type mismatch
```

The source-line + caret is the gold standard (we already do this for compile errors — check `compile_error` in `compiler/parser.gem` for the format). Reusing that machinery at runtime would be a meaningful upgrade.

### Files to read

- `runtime/gem_error.c` — error printing, stack trace
- `runtime/gem_ops.c` — arithmetic/comparison error paths
- `runtime/gem_builtins_*.c` — builtin error paths (consistent? caret? line?)
- `compiler/codegen.gem` — `gem_push_frame` emission and what `file/line` it passes
- `compiler/parser.gem` — `compile_error` for the format we already use at compile time

### Out of scope

- New error types or language-level changes (no `try/catch`, no `Result`).
- Performance — push_frame/pop_frame overhead is its own item in OPTIMIZATIONS.md.
- Stylistic rewrites of error wording in code that already shows source context.

## Validation gates

```bash
make test            # ALL EXAMPLES PASSED
make bootstrap       # roundtrip clean
make test-json       # === all tests passed ===
./build/gem compiler/test_liveness.gem --run   # 10/0 + 2/0
```

Existing example expected outputs may include error messages — touching error format will break those tests. Update `examples/expected_output.txt` deliberately, not blindly.

## Deliverable shape

A handful of focused commits, each addressing one error-quality issue, with a before/after example in the commit body. End-of-session: the most-tripped runtime errors have source location and (where feasible) a source-line + caret, matching the compile-error format.
