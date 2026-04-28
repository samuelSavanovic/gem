# Feedback-driven TODO

Code and design changes identified from external review of the spec and examples.

## 1. Remove `...rest, last` variadic form

**What:** Cut the rule where a single parameter after `...name` captures the last argument. Keep only simple `...name` as the final parameter.

**Why:** The trailing-block convention already passes blocks as the last argument implicitly. The `...rest, last` split adds a second unpacking mode that no other language uses, creating confusion at call sites. The stdlib doesn't use it — `gen_server.call` already shows the cleaner `...rest` + manual extraction pattern.

**Where:**
- `compiler/parser.gem` — remove the logic that handles a parameter after `...name`
- `docs/SPEC.md` — remove the `each_with(context, ...items, body)` example and the paragraph about "when a single parameter follows `...name`"
- Verify no existing code uses this form (grep for patterns like `...name, param`)

## 2. Fix supervisor.gem mutual recursion (stack leak)

**What:** `sup_loop` calls `handle_down` which calls `sup_loop` back. This is mutual recursion that doesn't get TCO, so every child restart event grows the stack. Under sustained child crashes this will eventually overflow.

**Why:** The compiler only optimizes direct self-recursion. The `sup_loop -> handle_down -> sup_loop` chain means neither call is optimized.

**Fix:** Inline `handle_down`'s logic directly into `sup_loop`'s receive arms so the loop is direct self-recursion. The function is only ~80 lines when inlined — manageable for a single receive block.

**Where:** `std/supervisor.gem` lines 9-101

## 3. Fix HTTP server example comment

**What:** The accept loop in `examples/http_server/server.gem` drains "done" messages with `after 0`, then blocks on `tcp_accept`. While blocked in accept, the `active` counter doesn't decrement for finishing connections until the next connection arrives.

**Why:** The `active` count is a lagging indicator and the connection cap is soft, not hard. This isn't a bug per se — under load there's always another connection — but readers shouldn't copy this pattern thinking it's exact.

**Fix:** Add a comment in the accept loop acknowledging that the active count is approximate / a soft cap.

**Where:** `examples/http_server/server.gem` around lines 195-210
