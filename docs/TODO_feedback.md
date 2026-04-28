# Feedback-driven TODO

Code and design changes identified from external review of the spec and examples.

## ~~1. Remove `...rest, last` variadic form~~ ✓ Done

## ~~2. Fix supervisor.gem mutual recursion (stack leak)~~ ✓ Done

## 3. Fix HTTP server example comment

**What:** The accept loop in `examples/http_server/server.gem` drains "done" messages with `after 0`, then blocks on `tcp_accept`. While blocked in accept, the `active` counter doesn't decrement for finishing connections until the next connection arrives.

**Why:** The `active` count is a lagging indicator and the connection cap is soft, not hard. This isn't a bug per se — under load there's always another connection — but readers shouldn't copy this pattern thinking it's exact.

**Fix:** Add a comment in the accept loop acknowledging that the active count is approximate / a soft cap.

**Where:** `examples/http_server/server.gem` around lines 195-210
