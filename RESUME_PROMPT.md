# Resume: caret span for runtime errors

Branch: `main`. Last session landed two commits on runtime error message quality:

- `d08bd44` — codegen emits `gem_set_line(N)` per statement; runtime errors now point at the offending line, not the function header.
- `228e40a` — uncaught errors in the main process now print `[Runtime Error]` + `--> file:line` + source line + stack trace, mirroring the compile-error format. Previously they were silently swallowed by the scheduler (user main runs as a process; no monitor → exit_reason dropped on the floor).

What's missing: the **caret**. Compile errors render `^^^^^^^^^` under the offending span; runtime errors don't, because the AST (and therefore `GemFrame`) only carries `line`, not `col` or `span_len`.

## This session — column + span tracking

### Concrete starting point

1. **AST.** `compiler/ast.gem:46-121`. Every node constructor takes `line` only. Add `col` (and probably `span_len` or `end_col`) to constructors that error paths need most: `call`, `binop` (or whatever `+`/`-` use), `index`, `dot`, `let`, `assign`. Don't bother with nodes that can't trip a runtime error.

2. **Parser.** `compiler/parser.gem` produces these nodes. The lexer must already track column for the existing compile-error caret — find how `compile_error` gets its `col`/`span_len` (`compiler/errors.gem:31-65` is the printer; trace where the call sites compute span length). Reuse that.

3. **GemFrame.** `runtime/gem.h:101-105`. Add `col` (and `span_len` if you went that way). Update `gem_push_frame`, `gem_set_line` → `gem_set_loc(line, col, span_len)`.

4. **Codegen.** `compiler/codegen.gem:3127, 3233`. Two `line_dir` emission sites. Currently emit `gem_set_line({node.line})`. Change to `gem_set_loc({node.line}, {node.col}, {node.span_len})`.

5. **Printer.** `runtime/gem_error.c:gem_print_source_context`. Currently prints the source line with no caret. Add the caret row using `col` and `span_len`. Match `compiler/errors.gem` formatting exactly — same gutter width arithmetic, same caret character.

### What good looks like

```
[Runtime Error]: type error in +: got int and string
  --> file.gem:4:13
   |
 4 |   let c = 5 + "oops"
   |             ^
   |
Stack trace:
  at doit (file.gem:4)
  at main (file.gem:8)
```

Caret position points at the operator (or the offending expression's start), span underlines the full expression. The exact "where to point" is a judgment call — pointing at the binary op's `+` is probably more useful than the whole expression. Decide as you go.

### Bootstrap reminder

Codegen output changes → `make bootstrap`'s built-in fixed-point check fails on first pass. Manual 3-stage:

```
make build
./build/gem compiler/main.gem --emit-c > /tmp/s1.c
cc -o /tmp/s1 /tmp/s1.c -I runtime -I compiler -std=c11 -O2 -lm -pthread build/libgem_runtime.a
/tmp/s1 compiler/main.gem --emit-c > /tmp/s2.c
cc -o /tmp/s2 /tmp/s2.c -I runtime -I compiler -std=c11 -O2 -lm -pthread build/libgem_runtime.a
/tmp/s2 compiler/main.gem --emit-c > /tmp/s3.c
diff -q /tmp/s2.c /tmp/s3.c    # must be identical
cp /tmp/s2.c bootstrap/stage0.c
make build
make bootstrap                  # now roundtrips clean
```

### Out of scope

- Rewriting individual `gem_error` message strings (separate quality pass).
- Builtin arity validation (tracked under "C Interop Hardening" in `docs/OPTIMIZATIONS.md`).
- Spawned-process crash visibility — current Erlang-style silent death is correct; only the main process prints.

## Validation gates

```bash
make test            # ALL EXAMPLES PASSED
make bootstrap       # roundtrip clean (after manual 3-stage seed)
make test-json       # === all tests passed ===
./build/gem compiler/test_liveness.gem --run   # 10/0 + 2/0
```

Existing expected outputs only reference `gem_error` strings via pcall (`expected_output.txt` lines 211, 389, 391, 393) — those are message text only and won't be affected by caret rendering, which only fires on the uncaught path.

## Deliverable shape

One commit threading `col` (and `span_len`) through AST → frame → printer. End-of-session: a user who writes `5 + "oops"` sees a caret pointing at the offending operator, matching compile-error UX exactly.
