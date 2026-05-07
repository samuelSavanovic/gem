# LSP Phase 0b — Multi-error recovery in the compiler

**Status:** plan only, pre-PR #8. No code yet.
**Predecessor:** PR #7 (Phase 0a, codegen caret context — threads `source` into `make_codegen`, routes the four `eprint("\n[Compile Error]:")` sites through `compile_error()`).
**Goal:** collect *all* compile-time errors and report them in one pass before exiting non-zero. Applies uniformly to `gem foo.gem` and `gem --check foo.gem` — same error model, same output format. `--check` only differs in skipping codegen + cc when there are zero errors.
**Non-goal:** changing AST shape for valid programs, or producing different C for valid programs. Bootstrap roundtrip must still pass.

**Decision delta from initial plan (2026-05-07 review):** earlier draft preserved fast-fail in non-check mode. Reverted — unifying error behavior across modes is simpler (no `mode` flag in the sink), strictly better UX on broken inputs in both invocation paths, and bootstrap is unaffected because valid inputs hit zero errors regardless. See §5.

The core observation: the parser is hand-rolled recursive descent; the lexer is largely linear; codegen's "errors" are mostly *semantic* checks that fire after the parse already succeeded. So the work splits cleanly:

- **Lexer:** convert the small set of fatal `compile_error()` calls into "emit error, recover by skipping the malformed token, continue". Most lexer failures are *unterminated*-style — those are unrecoverable mid-token, but easily turned into "emit one error and synthesize EOF".
- **Parser:** the real work. Need (a) a sync-point set, (b) error AST stubs, (c) a non-fatal `report()` that records and returns a stub.
- **Codegen + later passes (fold, liveness, mark_process_tail):** all dispatch on `node.tag` via `match` or `is_node`. Need to either (i) skip error stubs cleanly or (ii) abort codegen entirely if the parser produced any errors.

The plan recommends (ii) for codegen — the simplest, safest contract is **"if the parser reported errors, do not run codegen at all"**. Codegen's job changes from "must succeed" to "skipped iff parser had errors, otherwise runs and reports all of its own errors". Same in both `--check` and non-check invocations.

---

## 1. Audit — every error site

`compile_error()` (defined in `compiler/errors.gem:31`) is the universal terminal "print caret + exit(1)". The audit table below covers every site that can fire on user input.

Format: `file:line` · stage · message (truncated) · classification · proposed sync.

### 1.1 Lexer — `compiler/lexer.gem`

| # | line | message | class | sync strategy |
|---|------|---------|-------|---------------|
| L1 | 188 | triple-quoted string must be followed by a newline | **recoverable** | emit error, treat opening `"""` as if newline was present, continue |
| L2 | 217 | unterminated triple-quoted string | **hard** | emit error, push synthetic STRING(`""`) + EOF, return tokens |
| L3 | 257 | unterminated string escape (triple dq) | **hard-ish** | emit error, append `\` literal, continue |
| L4 | 297 | unterminated string escape (regular dq) | **hard-ish** | same as L3 |
| L5 | 324 | unterminated string | **hard** | emit error, push synthetic STRING(partial) + EOF |
| L6 | 344 | triple-quoted string (sq) must be followed by a newline | **recoverable** | as L1 |
| L7 | 373 | unterminated triple-quoted string (sq) | **hard** | as L2 |
| L8 | 411 | unterminated string escape (triple sq) | **hard-ish** | as L3 |
| L9 | 437 | unterminated string escape (regular sq) | **hard-ish** | as L3 |
| L10 | 453 | unterminated string (sq) | **hard** | as L5 |
| L11 | 595 | unterminated string escape (resume after interp, triple dq) | **hard-ish** | as L3 |
| L12 | 623 | unterminated triple-quoted string interpolation | **hard** | as L5 |
| L13 | 638 | unterminated string escape (resume after interp, dq) | **hard-ish** | as L3 |
| L14 | 665 | unterminated string interpolation | **hard** | as L5 |
| L15 | 690 | unexpected character: '{ch}' | **recoverable** | emit error, skip the offending byte, continue |

**Lexer rule of thumb.** "Unterminated *X*" errors all have the same recovery: emit one error, push an `EOF` token, and **stop the scan**. This is acceptable because at that point the rest of the file is inside a string the user forgot to close — any tokens we synthesize past that will be garbage, and the parser's first line of defense (`expect("EOF")` from above) will report a useless cascade. A single unterminated-string error per file is fine; lexer recovery for these is not worth the engineering.

So the **practical lexer strategy** is:
- L15 ("unexpected character"): real recovery — skip byte, continue. Yields multi-error at the lexer level for the cheap case.
- L1, L6 (missing newline after `"""`): real recovery — pretend the newline is there.
- L2, L5, L7, L10, L12, L14 (unterminated): emit one error, push EOF, stop.
- L3, L4, L8, L9, L11, L13 (unterminated escape): treat as unterminated (push EOF, stop) — they all happen at end-of-file, same condition as the unterminated-string family.

**Net lexer recovery payoff:** small. Most real "user typos" reach the lexer cleanly and surface in the parser. The lexer changes are mostly mechanical (replace `compile_error(...); exit(1)` with `report_lex_error(...); break/continue`), but they exist mainly so the lexer participates in the same error-sink discipline as the parser.

### 1.2 Parser — `compiler/parser.gem`

| # | line | message | class | sync point |
|---|------|---------|-------|-----------|
| P0 | 35 | not a digit (digit_val) | **bug** | unreachable on valid lex; classify as compiler bug, not a user error. Leave as `error()` (terminal). |
| P1 | 94 | expected '{tp}' but got '{t.type}' (the `expect()` helper) | **recoverable** (most common error) | per-caller — see §2 |
| P2 | 185 | required parameter after optional parameter (destructured) | **recoverable** | continue param list, sync at `)` |
| P3 | 227 | required parameter after optional parameter (regular) | **recoverable** | as P2 |
| P4 | 366 | expected '}' or table key | **recoverable** | sync at `}` or NEWLINE |
| P5 | 419 | unexpected token (parse_atom fallthrough) | **recoverable** | return `error_expr`; sync at NEWLINE/end-of-stmt |
| P6 | 623 | invalid assignment target | **recoverable** | error_stmt, sync at NEWLINE |
| P7 | 632 | compound assignment requires variable target | **recoverable** | as P6 |
| P8 | 672 | expected key name in table pattern | **recoverable** | sync at `}` |
| P9 | 768 | expected pattern (parse_pattern fallthrough) | **recoverable** | return error_pattern; sync at NEWLINE |
| P10 | 880 | named fn inside function body is not supported | **recoverable** | continue parsing the named fn anyway (it's syntactically valid, just a Gem rule); record and proceed |
| P11 | 898 | assignment in if condition | **recoverable** | record, continue with the parsed (incorrect) cond |
| P12 | 912 | use 'elif' instead of 'else if' | **recoverable** | record, treat the 'if' as an elif and continue |
| P13 | 929 | assignment in while condition | **recoverable** | as P11 |
| P14 | 1025 | expected 'in' or '=' after for variable | **recoverable** | error_stmt; sync at `end` |
| P15 | 1164 | expected 'fn' after 'extern blocking' | **recoverable** | error_stmt; sync at NEWLINE |

**The dominant error site is P1 (`expect()` mismatch).** Every `expect("end")`, `expect(")")`, `expect("=")`, etc. funnels through it. The audit needs to be read with that in mind — P1 covers ~60+ static call sites across the parser, each of which could fire on user input. The recovery strategy must specialize per *caller* of `expect`, not just per `expect` line.

**Caller-specific recovery for P1** (the important ones):

| `expect()` caller | line(s) | what we want on mismatch |
|---|---|---|
| `expect(":")` in table literal/pattern key | 368, 678, etc. | report, skip to next `,` or `}` |
| `expect("end")` after if/while/for/match/receive/fn body | 918, 932, 953, 980, 1004, 1066, 1208, 888 | **the cascade case** — report, but also try to insert an implicit `end` if the next token is a sync keyword (`fn`, `let`, top-level start). See §7. |
| `expect(")")` after call args / params | 344 (params), parse_call args | report, sync at NEWLINE or `end` |
| `expect("=")` in let | 869 | report, treat RHS as error_expr until NEWLINE |
| `expect("NAME")` (load/export/extern/let-destructure/etc.) | many | report, skip token, continue |
| `expect("(")` after `fn`/`extern fn` | 342, 882, 1137 | report, sync at `)` then continue |

**Conclusion:** rather than patch each `expect()` call site, the cleanest move is to make `expect()` non-fatal: report the mismatch, **do not consume**, return a synthetic token of the expected type. Most parser code is structured so that "we got the thing we expected and continue" is the happy path; on a `?` token-of-the-expected-type, the next iteration of the loop / next call typically tries the right thing. This is the YACC/Bison "`%expect`-like recovery" pattern adapted for hand-written parsers. Cases where this lies — e.g. when `expect("end")` is followed by another `expect("end")` in a nested elif chain — get manual sync (§2).

### 1.3 Codegen — `compiler/codegen.gem`

| # | line | message | class | mode |
|---|------|---------|-------|------|
| C1 | 2923 | "[Compiler Bug]: ..." (unknown unary/binary op, unknown expr/stmt tag) | bug — unreachable on valid parse | leave terminal. Codegen is skipped if parser reported errors (so error stubs never reach here); if a non-error stub with unknown tag does fire C1, it's still a real bug. |
| C2 | 2933 | cannot assign to function `{name}` | **recoverable** semantic | report, continue codegen with a no-op statement |
| C3 | 2935 | undeclared identifier `{name}` (in assign target) | **recoverable** | as C2 |
| C4 | 2990 | undeclared identifier `{name}` (in var ref) | **recoverable** | report, emit `GEM_NIL` placeholder for the read so codegen continues — still abort check before run |
| C5 | 3044 | module `{name}` has no export `{field}` | **recoverable** | as C4 |
| C6 | 3017 | unknown unary operator | **bug** (parser only emits known ops) | leave terminal |
| C7 | 3061 | unknown expression node | **bug** | leave terminal |
| C8 | 3351 | unknown binary operator | **bug** | leave terminal |

**Codegen rule of thumb.** C2/C3/C4/C5 are the four real "compile error" sites. C1/C6/C7/C8 are bug sentinels. The bug sentinels stay terminal — they're reachable only when the AST is malformed, which never happens because codegen is skipped if any parser/lexer errors were reported (§4.4). If one does fire, it's a genuine compiler bug and `[Compiler Bug]` exit is correct behavior.

The four real sites already each `eprint` + `exit(1)`; they need to become `record_compile_error(node, msg)` + continue. C4 has a nice shape — emitting `GEM_NIL` for an undeclared var is a good "recovery" choice because the rest of codegen produces valid C even if it's wrong C. Same for C5 (emit `GEM_NIL`).

### 1.4 main.gem

| # | line | message | class | notes |
|---|------|---------|-------|-------|
| M1 | 491, 540, 546 | usage / `-o` requires arg | terminal | not user-source, leave as-is |
| M2 | 617 | gem: compilation failed (cc) | terminal | leave |
| M3 | (load resolution) | various — "module X not found", redeclared, etc. | recoverable in principle | deferred — see §7.6. Today's hard-exit retained for v1. |

### 1.5 Other passes

- `compiler/fold.gem` — no `compile_error` calls. Pure rewrite, returns input on unknown shape (`fold.gem:156` short-circuits when `node.tag == nil`). Error stubs flow through it as no-ops if we tag them with `tag: "error_expr"` etc. **No changes needed.**
- `compiler/liveness.gem` — `match node` falls through silently for unknown tags. Error stubs are inert. **No changes needed.**
- `mark_process_tail` walk in `codegen.gem:2008–2043` — same `match`-and-fallthrough pattern. Inert on unknown tags. **No changes needed.**

This is the key insight: **the existing AST visitors already silently ignore unknown tags**, because Gem's `match` is a fall-through-on-no-match construct, not a sealed exhaustive match. We can introduce error_node tags and they'll be no-ops in fold/liveness/process-tail. This dramatically reduces blast radius.

---

## 2. Sync-point strategy

### 2.1 The sync set

For statement-level recovery (parser errors that fire from `parse_stmt_fn` and below), sync points are the tokens that unambiguously begin a new statement. Cite call sites from §1.2:

```
STMT_FIRST = { fn, let, if, while, for, match, return, break, continue,
               load, export, extern, NAME-followed-by-receive, NEWLINE }
STMT_END   = { end, else, elif, when, after, then, EOF }
```

`then` is in STMT_END because it terminates if-conditions; `when` because it terminates match arms; `after` for receive blocks. These are the same set that `parse_body` uses as its termination check (`parser.gem:262`).

For expression-level recovery (within an expression on a single line), sync points are:
```
EXPR_TERMINATORS = STMT_END ∪ { NEWLINE, ',', ')', ']', '}' }
```

### 2.2 Two recovery primitives

Introduce two helpers in the parser:

```
let sync_to_stmt = fn()
  # consume tokens until peek().type in STMT_FIRST or STMT_END.
  # used after a parse_stmt failure.
end

let sync_to_expr_end = fn()
  # consume tokens until peek().type in EXPR_TERMINATORS.
  # used after a parse_expr failure (e.g. parse_atom fallthrough P5).
end
```

Both are **consume-until** strategies, not lookahead-only. Lookahead-only is safer (no risk of skipping a recoverable token), but it leaves the offending token in the stream and the next call typically re-fails, generating duplicate errors. Consume-until is the right default; lookahead-only is reserved for `expect()` fallthrough (see §1.2 — `expect` reports without consuming, which gives the parser a chance to "see" the missing token and synthesize behavior).

### 2.3 The `expect()` redesign

**Today** (`parser.gem:91–98`):
```
let expect = fn(tp)
  let t = tokens[pos]
  if t.type != tp
    compile_error(...)  # exits
  end
  pos += 1
  t
end
```

**Proposed:**
```
let expect = fn(tp)
  let t = tokens[pos]
  if t.type != tp
    report(t.line, t.col, len(t.value), "expected '{tp}' but got '{t.type}'", nil)
    # Do not consume. Return a synthetic token of the expected type at
    # the current position, so callers that immediately read the result
    # still get a sensible value.
    return {type: tp, value: tp, line: t.line, col: t.col, synthetic: true}
  end
  pos += 1
  t
end
```

Two consequences:

1. The parser keeps making forward progress structurally (synthetic tokens fill in for missing ones), but the *raw token stream* doesn't advance. The next loop iteration / next call sees the same wrong token. For loops with a termination condition based on token type — e.g. `while peek().type != "}"` — this is a problem: we'd loop forever. Fix: every `while peek().type != X` loop in the parser must add a "or progress not made this iteration → bail" guard, OR use `expect_or_sync(tp, sync_set)` that consumes-until in the failure case. The cleanest answer is a third primitive:

```
let expect_loop = fn(tp, sync_set)
  # Like expect, but on mismatch reports and consumes-until any token in sync_set.
  # Used inside loops where infinite-loop-on-non-progress is the failure mode.
end
```

   Used at: closing `}` of table literals (`parser.gem:377`), `]` of array literals (`:394`), `)` of param lists / call args, etc. The wrapping loop body stays the same — `expect_loop` just guarantees forward progress.

2. P10/P11/P12/P13 (the "stylistic" errors — `else if`, assignment-in-cond, named-fn-in-body) become trivial: report and continue. No sync needed; the AST after them is well-formed.

### 2.4 Where sync_to_stmt is used

- Top-level `parse` loop (`parser.gem:1222`): wrap `parse_stmt()` in a try-or-sync. If `parse_stmt` returned an `error_stmt` (signaling unrecoverable mid-stmt), call `sync_to_stmt()`.
- `parse_body` and `parse_fn_body`: same wrapping. Critical because if a body is malformed, we want to find the next `end`/`when`/`else` and resume parsing the enclosing construct. **This is the sync point that prevents the missing-`end` cascade described in §7.**

### 2.5 Tradeoffs with real cases

- P5 (`parse_atom` fallthrough on garbage like `let x = + 3`): sync_to_expr_end is the right answer. The parsed `let x =` is fine, the RHS becomes `error_expr`, parser resumes at the next NEWLINE.
- P1 with `expect("end")` after a `while` body (line 932): if the body itself contained an error and got stuck, the body parser may have consumed the `end`. Recovery here is the `expect_loop` pattern — accept synthetic `end`, report once, move on.
- P2/P3 (parameter list errors): sync to `)`. Today the parser keeps trying to parse params; with the new `expect_loop` over `,`/`)` it terminates cleanly.
- P14 (for variable not followed by `in` or `=`): sync to `end`. The `for` is unparseable; we emit error_stmt and let the surrounding body re-find its bearings.

---

## 3. Error-node AST shapes

### 3.1 The shapes

Three new node tags, all of which carry `{file, line, col}` for diagnostics:

```
{tag: "error_expr", line: ..., col: ..., file: ...}      # placeholder where an expr was expected
{tag: "error_stmt", line: ..., col: ..., file: ...}      # placeholder where a stmt was expected
{tag: "error_pattern", line: ..., col: ..., file: ...}   # placeholder where a match pattern was expected
```

That's it. No "kind tag" union; per-construct stubs because the three sites have different downstream visitors and conflating them adds branches everywhere.

Add constructors to `compiler/ast.gem`:

```
fn make_error_expr(line) {tag: "error_expr", line: line} end
fn make_error_stmt(line) {tag: "error_stmt", line: line} end
fn make_error_pattern(target_expr) {condition: make_bool(false), bindings: []} end
```

`error_pattern` is special — it doesn't need its own tag because the existing match-pattern shape is `{condition, bindings}`. Returning a falsy condition is the cleanest "this arm never matches" stub. **No new tag for patterns.**

### 3.2 How visitors react

| Pass | Treatment of error_expr / error_stmt | Code change required? |
|---|---|---|
| `fold.gem` | `node.tag` not in match → return node unchanged (line 156 check) | **none** |
| `liveness.gem` (`_collect_free_node`, `_is_stc`, etc.) | `match` falls through on unknown tag → no-op | **none** |
| `mark_process_tail` walk in `codegen.gem:2008+` | `match` falls through → no-op | **none** |
| `codegen.gem` `compile_expr` | falls through → calls `codegen_error("unknown expression node")` → `[Compiler Bug]` exit | **must change** — see §3.3 |
| `codegen.gem` `compile_stmt` | falls through → falls through to `let r = compile_expr(node)` which then hits the same path | **must change** — see §3.3 |
| `tag_source_file` (in main.gem) | walks AST stamping `node.file` — should already handle unknown tags as it walks structurally over known fields. Need to verify. | likely none; verify. |
| `resolve_loads` (in main.gem) | rewrites `load` nodes; ignores everything else | **none** |

### 3.3 Codegen contract under the new error model

Two options, choose **one**:

**Option A (recommended): refuse codegen if any errors before it.**
After `parse_source` + `tag_source_file` + `resolve_loads`, if `sink.has_any()` returns true, **skip codegen entirely** — print all errors, exit 1. Same in `--check` and non-check invocations (§5).

This is the safest contract: codegen never sees an error_expr/error_stmt. Its existing fallthrough-to-`codegen_error` stays as a bug sentinel and remains correct.

**Option B: make codegen tolerant of error stubs.**
Add `when {tag: "error_expr"}` to `compile_expr` returning `{expr: "GEM_NIL", setup: ""}`, and `when {tag: "error_stmt"}` to `compile_stmt` returning `""`. This lets codegen run partially even with error stubs, which surfaces *additional* semantic errors (C2–C5) on the same pass.

**Recommendation: Option A for the first PR, Option B as a follow-up.**

Rationale:
- Option A is ~5 lines (one early-exit gate); Option B requires verifying that no codepath through codegen relies on error stubs producing meaningful expressions or types. Decompose-concat (`codegen.gem:3425`), TCO detection, ic_cache numbering, free-var analysis — all of these run over the AST and could mis-behave on `error_expr`. Auditing them is an extra ~half-day.
- User-visible difference: with A, you see all parse errors per save but you don't see undeclared-identifier errors until parse is clean. With B, you see both. B is strictly better UX, but A is a fine first step that already moves the needle from 1 error/save to N errors/save.

The plan below assumes Option A.

---

## 4. Threading mechanism

Three options listed in the prompt:

(a) `errors[]` parameter threaded through every error-reporting function
(b) global mutable error sink set by `compile_error_at(node, msg)`
(c) closure-captured sink at parser/codegen entry

### 4.1 Pick: (c), with a thin (b) shim for compatibility

The codebase already uses (c) heavily: `make_codegen` (`codegen.gem:1`-ish) is a closure-of-state pattern; `make_parser` (`parser.gem:66`) is the same. The natural extension is to thread the error sink into both closures.

**Sketch:**

```
# compiler/errors.gem additions

fn make_error_sink()
  let errors = []
  let report = fn(source, file, line, col, span_len, msg, hint)
    push(errors, {source: source, file: file, line: line, col: col,
                  span: span_len, msg: msg, hint: hint})
  end
  let has_any = fn() len(errors) > 0 end
  let drain = fn() errors end
  let count = fn() len(errors) end
  {report: report, has_any: has_any, drain: drain, count: count}
end

fn print_all_errors(sink)
  for e in sink.drain()
    # Same caret rendering as compile_error, but doesn't exit.
    print_one_error(e.source, e.file, e.line, e.col, e.span, e.msg, e.hint)
  end
end
```

No mode flag — the sink always accumulates. Mode-specific behavior (skip codegen when errors, skip cc when `--check`) lives in `main.gem`, not the sink.

Then:

```
# parser entry
fn parse_source(source, file, sink)
  let tokens = tokenize(source, file, sink)
  let p = make_parser(tokens, source, file, sink)
  p.parse()
end

# codegen entry
fn make_codegen(source_name, module_bindings, project_root, source_text, sink)
  ...
  let report_compile_error = fn(node, msg, hint)
    sink.report(source_text, file_of(node), node.line, node.col, span_of(node), msg, hint)
  end
  ...
end
```

### 4.2 Why not (a)?

Option (a) — threading `errors[]` through every function — touches every helper in the parser. The parser has ~30 nested closures (`parse_atom`, `parse_call`, `parse_postfix`, …); every one would need a parameter. The closure-captured sink in the parent scope avoids this entirely.

### 4.3 Why not pure (b)?

Pure global state has two problems:
- The compiler is invoked once per process today, but the LSP will eventually want to invoke parse-and-check in-process for editor buffers (see Phase 1 in the roadmap). A global sink means a stale error from a previous parse can bleed in if reset is ever forgotten. A closure-scoped sink is safe-by-construction.
- `make_codegen`'s closure pattern already establishes the convention. (c) is consistent.

The "thin (b) shim" caveat: a single function `report_at(node, msg, hint)` that pulls the current sink from a module-local closure could simplify the audit pass — every `compile_error(...)` call site becomes a one-line replacement. The sink itself is captured by the parser/codegen closures (option c); the call-site shim just wraps `sink.report(...)` for ergonomics.

### 4.4 Driver flow in main.gem

```
let sink = make_error_sink()

let ast = parse_source(source, src_path, sink)
tag_source_file(ast, src_path)
resolve_loads(ast, sink)

if sink.has_any()
  print_all_errors(sink)
  exit(1)
end

if cli.check
  exit(0)   # --check exits before codegen on success
end

let c_output = make_codegen(...).emit(ast, sink)

if sink.has_any()
  print_all_errors(sink)
  exit(1)
end

# write c_output, run cc as today
```

Same flow for both invocations. `--check` only changes one branch (the early exit before codegen on success). Errors are always accumulated, always printed in one pass, exit code is 1 if any errors regardless of mode.

---

## 5. Behavior across `--check` and non-check

Both invocation modes share the same error model: accumulate, print all on exit. The only difference is what runs after a successful (zero-error) parse + analysis:

| Phase | non-check (`gem foo.gem`) | `gem --check foo.gem` |
|---|---|---|
| parse + load resolution | runs, errors accumulate | runs, errors accumulate |
| codegen | skipped if errors; runs otherwise | always skipped |
| cc | skipped if errors; runs otherwise | always skipped |
| exit code | 0 if zero errors and cc succeeded; 1 otherwise | 0 if zero errors; 1 otherwise |

**Behavioral invariants preserved:**

1. **Valid programs.** The AST and emitted C for a zero-error program is byte-identical to today. Recovery code only fires on errors; `tag_source_file`/`fold`/`liveness`/codegen are unchanged for valid programs. Bootstrap roundtrip (§6.1) is the gate.
2. **Exit code on broken inputs.** Same as today (1) — only the error count printed before exit changes.

**Behavioral changes on broken inputs (intentional):**

- `gem foo.gem` on a file with multiple errors now prints all of them before exiting, instead of one. This is a user-visible improvement; nothing in the bootstrap or test corpus depends on the old "exactly 1 error printed" behavior.
- Stylistic errors (P10/P11/P12/P13 — `else if`, assignment-in-cond, named-fn-in-body) that today exit will now be reported and the parser will continue. The accumulated AST may proceed further than today's parser would on broken inputs — but it never reaches codegen because we gate codegen on `sink.has_any()` (§4.4). **No behavioral divergence for valid programs.**

**Why this beats the original "preserve fast-fail in non-check" plan:** the only argument for fast-fail was "minimal blast radius." But the unified path is *simpler* (no `mode` field in the sink, single error-print path), strictly more useful (more errors per save in both modes), and bootstrap-safe by construction. Cascade noise on broken inputs is bounded by the cap + heuristics in §7.1.

---

## 6. AST stability gate

### 6.1 Positive test: bootstrap roundtrip + golden corpus

```
make bootstrap                  # stage0.c → stage0 → stage1.c, byte-equal
build/gem --check examples/*.gem      # all 99 examples succeed
build/gem --check std/*.gem           # std lib succeeds
build/gem --check compiler/*.gem      # compiler self-check succeeds
```

The `make bootstrap` target already exists per the user's note. The rest can be a small shell script `tests/check_corpus.sh` that loops:

```bash
fails=0
for f in examples/*.gem std/*.gem compiler/*.gem; do
  if ! build/gem --check "$f" 2>/dev/null; then
    echo "FAIL: $f"
    fails=$((fails + 1))
  fi
done
[ $fails -eq 0 ]
```

Hook into `make test` or `make check`. **Required to pass for PR #8 to merge.**

### 6.2 Negative test: broken-program corpus

Create `tests/broken/` with intentionally malformed programs. Each must produce **N≥2** errors with sane locations. Suggested initial set:

```
tests/broken/missing_end.gem            # fn foo() ... [missing end] fn bar() ... end
tests/broken/unterminated_string.gem    # let x = "hello\n let y = 3
tests/broken/double_typo.gem            # let x = 1 + ; let y = ; (two atom-failures on same line)
tests/broken/bad_pattern.gem            # match x when @ ... when % ... end
tests/broken/cascade_braces.gem         # let t = {a: 1, b: , c: 3} (table key error in middle)
tests/broken/multi_undeclared.gem       # let x = unknown_a + unknown_b + unknown_c
tests/broken/missing_then_branches.gem  # if a == 1 b = 2 elif a == 2 c = 3 end (missing then in either branch)
```

Gate script `tests/check_broken.sh`:

```bash
expected_min=( missing_end:2 unterminated_string:2 double_typo:2 bad_pattern:2 cascade_braces:2 multi_undeclared:3 missing_then_branches:1 )
for entry in "${expected_min[@]}"; do
  name=${entry%:*}; min=${entry#*:}
  out=$(build/gem --check "tests/broken/$name.gem" 2>&1 | grep -c "^\[Compile Error\]")
  if [ "$out" -lt "$min" ]; then
    echo "FAIL: $name produced $out errors, expected >=$min"
    exit 1
  fi
done
```

**Note on multi_undeclared:** this one only works under §3.3 Option B (codegen tolerant of error stubs). Under Option A, codegen is skipped if parse failed — but the source has valid syntax, so codegen runs normally and emits 3 separate undeclared-identifier errors (sites C2–C5). This case actually exercises the codegen-side multi-error story; keep it.

**Note on cascade discipline:** the gate should also verify *upper bounds* in some cases. E.g. `tests/broken/missing_end.gem` should produce **≥2 and ≤8** errors — not 50 cascading errors from one missing keyword. Hard cap is the most important property to test for cascade behavior. See §7.

### 6.3 Optional: AST-equivalence gate

A stronger but more expensive gate: run `--emit-c` over the corpus before and after the change, diff the C. Already implicitly covered by the bootstrap roundtrip (which compiles `compiler/*.gem` to C and diffs against the committed stage0.c). For external corpora (examples, std), the byte-equal check is straightforward:

```
for f in examples/*.gem; do
  diff <(build/gem-old --emit-c "$f") <(build/gem-new --emit-c "$f") || exit 1
done
```

Worth running once locally during the PR; doesn't need to be a CI gate.

---

## 7. Risk corners

### 7.1 The cascade problem (most important)

A single missing `end` triggers parser-state confusion: `parse_body` keeps consuming statements past where a body should have ended, eventually choking on something at the top level. Without care, this produces 20+ spurious errors all rooted in one missing keyword.

**Mitigation:**
- Stop reporting after some cap (e.g. `errors.count() > 20 → bail with "too many errors, fix the first ones"`). This is a UX safety valve, not a correctness fix.
- Heuristic at `expect("end")` mismatch: if the next token is `fn`/top-level, **silently accept the missing end** (don't consume), report once, and let the outer parser see the `fn` start. This is the "indent-aware end inference" trick; it works because Gem's grammar is statement-keyword-led.
- Track `last_error_pos` — if the parser hasn't advanced since the last report, force-advance one token. Prevents tight loops at a single bad token from spamming the sink.

**Reality check:** doing this *well* in a hand-rolled recursive-descent parser is the genuinely ugly part of this PR. yacc/ANTLR have FIRST-set machinery that does this for you; we don't. Expect the cascade-suppression heuristics to need iteration after first contact with real broken programs in `tests/broken/`. Budget time accordingly. **Honest assessment: this is the part of the work most likely to be revisited after PR #8 ships.**

### 7.2 Error stubs in binary expressions

`f() + g()` where both are `error_expr`. Under Option A this never reaches codegen, so no concern. Under Option B (follow-up), `compile_binop` would call `compile_expr` on each side, which returns `{expr: "GEM_NIL", setup: ""}`, and the binop would emit `gem_add(GEM_NIL, GEM_NIL)` — valid C, runtime error, never executed because `--check` exits before producing a binary. Not a real risk.

### 7.3 Liveness on bodies with `error_stmt`

`liveness.gem` `_is_stc` and `_collect_free_stmts` recurse over body arrays. An `error_stmt` element is a no-op (match falls through). The pass completes correctly; it just sees the body as one statement shorter than the user wrote. **No risk.** (Confirmed by code reading at `liveness.gem:101` — explicit `if type(node) != "table" or node.tag == nil then return` defensive guard, and the match falls through.)

### 7.4 Codegen `error("unknown ... node")` sites

Already addressed in §1.3 — these are bug sentinels, kept terminal, and unreachable from `--check` under Option A. **No risk.**

### 7.5 Half-consumed token stream after parser failure

The `expect()` redesign (§2.3) explicitly does NOT consume on failure. Combined with `expect_loop`'s "consume until sync set", the parser never half-consumes a structural token. Risk: forgetting to use `expect_loop` in a loop — then `expect` reports and returns synthetic, the loop condition is unchanged, and we infinite-loop. **Mitigation:** every `while peek().type != X` in the parser must be audited and converted to use `expect_loop` for its terminator. Audit list:

- `parser.gem:144` (destructuring param `}`)
- `parser.gem:161` (destructuring param close)
- `parser.gem:308` (interp `INTERP_END`)
- `parser.gem:358` (table literal `}`)
- `parser.gem:386` (array literal `]`)
- `parser.gem:664` (table pattern `}`)
- `parser.gem:708` (array pattern `]`)
- `parser.gem:787` (let-destruct `}`)
- `parser.gem:832` (let-destruct array `]`)
- `parse_body` and `parse_fn_body` (already have multi-token termination + at_end check; safer)
- `parse_call` arg list, `parse_params` — verify

Plus a global watchdog: at the top of the main parse loop, track `last_pos`. If `parse_stmt` returned without advancing `pos`, force-advance one token. This is the ultimate safety net and is cheap.

### 7.6 Module loading

`resolve_loads` in `main.gem` happens between parse and codegen. It can fail (file not found, etc.). Should eventually report-and-continue (treat the missing module's exports as empty so downstream `module.field` references error out cleanly via the existing C5 code path), but errors are at file granularity rather than source position so the caret-rendering shape differs. Out of scope for PR #8; defer. Today it exits hard, which remains acceptable for v1 — load resolution failures are typically immediate-fix bugs (missing file, typo in path) rather than the kind of error you want to see alongside others.

### 7.7 Lexer error stubs in token stream

If the lexer emits N+1 tokens but two of them are wrong, the parser will try to parse them and produce parser errors. Expected behavior: lexer errors are sufficient context, parser errors that derive from them are noise.

**Decision (2026-05-07 review): skip the parser entirely if the lexer reported any errors.** Matches the user mental model — "you have a syntax error in your string, fix that first." Costs some multi-error count on files that have *both* lexer and parser errors, but those are rare in practice (a user editing typically introduces one kind of error at a time), and the cascade-suppression infrastructure for lexer errors would be its own rabbit hole.

Implementation: after `tokenize(source, file, sink)`, check `sink.has_any()` before constructing the parser. If true, jump straight to `print_all_errors` + exit. Five lines.

### 7.8 `print_all_errors` ordering

Parser errors emerge in source order naturally (the parser walks the file linearly). Codegen errors emerge in AST traversal order, which is *also* roughly source order. But interleaving the two is not source-ordered. **Recommendation:** at print time, sort by `(file, line, col)`. Trivial and matches user expectations.

### 7.9 Performance

`--check` is invoked on every save in editor flows. Today it's fast. The error-sink overhead is negligible (one closure call per error site, only fires on errors). **No performance risk.**

---

## 8. PR scope and decomposition

### 8.1 Estimated LOC for full PR

| Component | New / changed LOC |
|---|---|
| `compiler/errors.gem` — `make_error_sink`, `print_all_errors`, `print_one_error` (refactor of `compile_error`) | ~80 |
| `compiler/lexer.gem` — thread sink, replace `compile_error()` with `sink.report()`, recovery for L1/L6/L15, abort-on-unterminated for the rest | ~50 |
| `compiler/parser.gem` — thread sink, `expect()` rewrite, `expect_loop` helper, `sync_to_stmt`, `sync_to_expr_end`, ~17 call-site wraps, infinite-loop audit | ~150 |
| `compiler/ast.gem` — `make_error_expr`, `make_error_stmt` constructors | ~6 |
| `compiler/codegen.gem` — thread sink, replace 4 `eprint+exit(1)` sites with `sink.report()` (C2–C5), error_stub guard at codegen entry (Option A) | ~30 |
| `compiler/main.gem` — instantiate sink, gate codegen on parser errors in `--check` mode, post-pass print_all_errors | ~20 |
| `tests/broken/*.gem` (7 files) | ~50 (small files) |
| `tests/check_corpus.sh`, `tests/check_broken.sh` | ~40 |
| Makefile hooks | ~10 |

**Total: ~430 lines.** This is consistent with the roadmap's 200-400 estimate (the high end, because the audit revealed P1 needs `expect_loop` infrastructure which the roadmap didn't anticipate).

### 8.2 Possible decomposition into smaller PRs

If scope creeps, here are the natural cut points:

**PR #8a (preliminary): error sink scaffolding + lexer.** ~120 LOC.
- `make_error_sink`, `print_all_errors`, refactored `compile_error`.
- Thread sink through lexer; replace `compile_error()` sites with `sink.report()`.
- Add lexer recovery for L15 (unexpected character) and L1/L6 (missing newline).
- Driver in `main.gem`: after lex, if `sink.has_any()`, print and exit 1 (no parser yet — temporary gate, lifted in 8b).
- Bootstrap roundtrip + corpus check still passes (no AST changes for valid programs).
- **User-visible change**: a file with multiple lexer errors now prints all of them; previously it printed one and exited. Single-error files: identical to today. Sets up the infrastructure for the real PR.

**PR #8b (main): parser recovery.** ~250 LOC.
- `expect()` redesign, `expect_loop`, sync helpers.
- Thread sink through parser, `error_expr` / `error_stmt` constructors.
- Codegen gate (Option A): if parser errors, skip codegen.
- `tests/broken/*` and gate script.
- **Ships the user-visible win**: multi-error output in `--check` mode.

**PR #8c (follow-up, optional): codegen recovery (Option B).** ~80 LOC.
- Tolerant `compile_expr` / `compile_stmt` for error stubs.
- Thread the four C2–C5 sites through `sink.report()`.
- `tests/broken/multi_undeclared.gem` upgraded to verify codegen multi-error.

**Recommendation:** ship as 8a + 8b together (roughly the original "PR #8"), with 8c deferred until needed. 8a alone is too inert to be worth a separate review pass, and it lacks tests. 8b without 8a is a single mega-PR; awkward but doable.

### 8.3 Open questions for the implementer

- Does `compile_error` need to keep its `exit(1)` for the case where `source` or `line` is nil (errors.gem:35)? Probably yes — that branch is for compiler-bug paths where caret rendering is impossible; preserve it.
- Span length on `error_expr` / `error_stmt` — should we walk the AST to compute the "real" extent of the error region for caret rendering? Probably overkill for v1; use the token width at the report site.
- Should we deduplicate errors? E.g. if the same `(file, line, col, msg)` is reported twice (which can happen on cascade), suppress duplicates. **Recommendation: yes, in `print_all_errors`. ~5 lines.**
- Lexer interaction: if the lexer emits a synthesized `EOF` after an unterminated string, the parser will see a truncated token stream and may produce a few "expected X but got EOF" errors that are not useful. Suppress those if any lexer errors were reported. ~5 lines in `print_all_errors`.

### 8.4 Sequencing relative to PR #7

PR #7 (Phase 0a) threads `source` text into `make_codegen` and routes the codegen `[Compile Error]` sites through `compile_error()`. **This PR's codegen changes (§1.3 sites C2–C5) are direct continuations of that work** — once `compile_error()` is the codegen error path, replacing `compile_error()` with `sink.report()` is a one-token change per site.

So PR #8 is unblocked the moment PR #7 lands, and the diffs don't conflict structurally.

---

## Summary of decisions

| Decision | Choice | Rationale |
|---|---|---|
| Recovery primitive | report-without-consume `expect()` + `expect_loop` for terminator loops + `sync_to_stmt`/`sync_to_expr_end` | survives the FIRST-set-less hand-rolled recursive descent |
| Error AST | per-construct stubs (`error_expr`, `error_stmt`); patterns reuse `{condition: false, bindings: []}` | minimum new shapes; existing visitors are inert via match-fallthrough |
| Threading | closure-captured sink at `make_parser` / `make_codegen` entry (option c) | matches existing codegen pattern; safer than global |
| Codegen on errors | Option A (skip codegen if parser errors) for v1; Option B (tolerant codegen) deferred | smaller PR; meaningful UX gain already; B follows up cleanly |
| Mode gating | unified — sink always accumulates; `--check` only differs in skipping codegen + cc on success | simpler than fast/collect dichotomy; better UX on broken inputs in both modes; bootstrap safe |
| Lexer→parser handoff | skip parser entirely if lexer reported any errors | matches user mental model ("fix syntax-in-string first"); avoids cascade duplication |
| Stability gate | bootstrap + corpus `--check` + tests/broken with N≥2 + cascade upper-bound | catches regressions at AST and behavior levels |
| Cascade defense | `expect_loop` for terminators + force-advance watchdog at top-level + error count cap (~20) + heuristic missing-`end` insertion at top-level keywords | hand-rolled equivalent of FIRST-set sync; expect iteration |

---

### Critical Files for Implementation

- `/Users/samuel/Documents/projects/gem/compiler/parser.gem` — the bulk of the work; `expect()` rewrite, sync helpers, ~17 error site rewrites, infinite-loop audit at the loop sites listed in §7.5
- `/Users/samuel/Documents/projects/gem/compiler/errors.gem` — sink scaffolding (`make_error_sink`, `print_all_errors`); refactor `compile_error` so its rendering can be reused without exit
- `/Users/samuel/Documents/projects/gem/compiler/codegen.gem` — thread sink, route C2–C5 (lines 2933, 2935, 2990, 3044) through `sink.report()`
- `/Users/samuel/Documents/projects/gem/compiler/main.gem` — instantiate sink based on `cli.check`, gate codegen-skipping behind parser errors, drive `print_all_errors` at exit
- `/Users/samuel/Documents/projects/gem/compiler/lexer.gem` — thread sink, replace 15 `compile_error()` sites with `sink.report()`, add recovery for L1/L6/L15
