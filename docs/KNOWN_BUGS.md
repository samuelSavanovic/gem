# Known bugs

Bugs found and not yet fixed. Each entry has a minimal repro (checked
against `build/gem`), what goes wrong, and where the code is. A fix deletes
its entry in the same change, together with any **(bug)** rule in
`docs/BEST_PRACTICES.md` that exists because of it. Performance problems go
in `docs/OPTIMIZATIONS.md`, missing features in `docs/ROADMAP.md`.

## Compiler diagnostics

### A missing source file prints a stack trace into the driver

`gem missing.gem` prints `[Runtime Error]: read_file: cannot open
'missing.gem'` with a caret line and stack trace in compiler/main.gem
(`let source = read_file(src_path)`), exit 1. It should be a one-line
`gem: cannot open 'missing.gem'` like the other usage errors
(`usage_error` in compiler/main.gem).

### Loading a missing file crashes the compiler

```gem
load "mods/nope"
```

prints `[Runtime Error]: read_file: cannot open 'mods/nope.gem'` with a
stack trace of the compiler itself (`at resolve_loads (compiler/main.gem:…)`)
instead of a compile error at the `load`. `resolve_loads` in
compiler/main.gem reads the file without checking it exists.

### A named `fn` inside a top-level block is dropped

```gem
if true
  fn helper() print("nested") end
  helper()
end
```

reports undeclared identifier `helper` at the call. Inside a fn body
the same code gets a clear "named fn inside function body is not
supported" error; at the top level the definition silently disappears.

### A module-level `let` and `fn` with the same name are both accepted

```gem
let helper = 1
fn helper() print("fn") end
helper()             # attempt to call int value
```

No error at the second definition; the `let` silently wins.

## Runtime

### Runtime traces lose the source line when run from another directory

Trace paths are project-relative (or relative to where the program was
compiled from), and the runtime opens them relative to the current
directory to print the `-->` source line. A binary run from any other
directory prints the trace without the source line, silently.
`gem_print_source_context` in runtime/gem_error.c.

### `kill(pid, "normal")` kills a process that doesn't trap exits

```gem
let m = self()
spawn do
  kill(m, "normal")
end
sleep(50)
print("not reached")    # main is gone; the program exits 0 silently
```

In Erlang, an exit signal with reason `normal` is ignored by a process
that doesn't trap exits; here it ends the target (main included, with no
report). `gem_exit_builtin` in runtime/gem_scheduler.c. Decide which
semantics Gem wants and document it in SPEC.md (`kill`).

## Editor grammars

### tree-sitter grammar gaps

- No `;` statement separator.
- About 21 repo files parse with ERROR nodes (e.g. examples/06_blocks.gem,
  examples 115–126, std/http.gem, `load ... (names)` in compiler/*.gem).
- `"""` nested in a `"""` interpolation is an ERROR node, and probably ends
  the outer string early in the VS Code grammar too.

Not re-checked after #28 (no tree-sitter CLI in that environment).
