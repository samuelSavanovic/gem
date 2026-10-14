# Known bugs

Bugs found and not yet fixed. Each entry has a minimal repro (checked
against `build/gem`), what goes wrong, and where the code is. A fix deletes
its entry in the same change, along with any **(bug)** rule in
`docs/BEST_PRACTICES.md` that exists because of it. Performance problems go in
`docs/OPTIMIZATIONS.md`, missing features in `docs/ROADMAP.md`.

## Compiler

## Runtime

## Standard library

## Language server

## Editor grammars

## Example programs

### The honeypot's loops drop unexpected messages without a word

The main loops of the honeypot's recorder, registry and sampler end their
`receive` with `when other nil`, so a message with a misspelled tag (from
a future change, or a std message they don't expect) disappears with no
trace. BEST_PRACTICES ("Keep mailboxes clean") now has a main loop's
catch-all log what it didn't expect, as `examples/httpd` does. Run from a
file in the repository's root:

```gem
load "examples/honeypot/recorder"
load "std/sqlite"

fn main()
  let pid = recorder.start("hp_repro", {dir: "/tmp/hp_repro"})
  send(pid, {tag: "hp_evnt", event: {kind: "command", session: 1, line: "id", time: epoch_ms()}})
  let db = sqlite.open(recorder.sync(pid))
  print(sqlite.query(db, "SELECT count(*) AS n FROM commands"))   # [{n: 0}], nothing on stderr
  exit(0)
end
```

Where: `loop` in `examples/honeypot/recorder.gem` and
`examples/honeypot/sampler.gem`, the receive loop in
`examples/honeypot/registry.gem`. The fix is a `log.warn` of the clipped
message in each catch-all, shipped with the honeypot's next deploy.
