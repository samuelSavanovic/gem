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

## Names and paths in reports

### `pcall` result `stack` keeps raw `_anon_N` names

```gem
let f = fn() error("x") end
let r = pcall f()
print(r.stack)       # [{name: "_anon_1", ...}, ...]
```

Printed traces say `anonymous fn`; the `stack` table should match.

### Loaded-module functions show mangled names and absolute paths

A runtime error in a function of a loaded module `k8m` prints
`at _mod_k8m_inner (/abs/path/mods/k8m.gem:1)`, and a compile error in a
loaded module prints an absolute path in the `-->` line. Expected:
`k8m.inner` and a project-relative path, as the spawn `note:` already does
(#28).

### Name-keyed codegen analyses treat locals as builtins

`spawn_callees` and `mutating_builtins` (compiler/codegen.gem) match call
names, so a param or local named `spawn`, `push`, ... is treated as the
builtin. `extern fn` declarations named like a builtin (`extern fn
len(s: String) -> Int`) are accepted without any check.

## Runtime

### Main killed through a link exits 0 with no report

```gem
spawn_link do
  error("child dies")
end
sleep(100)
print("not reached")
```

The child's crash report prints, then main dies silently with exit
status 0. Main should report why it died and exit non-zero.

### `link()` to a dead process raises a catchable error

`pcall link(dead_pid)` returns `{ok: false, error: "noproc"}`; uncaught, it
is reported as an ordinary crash. Erlang semantics would exit the caller
with reason `noproc` (`gem_exit_self(reason)`), which `trap_exit` can
observe. `gem_link_fn` in runtime/gem_scheduler.c.

### Crashes in leaf functions lose their location

```gem
spawn(fn() 1 + "a" end)
sleep(20)
```

prints only `[Runtime Error in process 1]: type error in +: got int and
string`: no source line, no stack trace. A crash in a leaf function called
from elsewhere is reported at the caller's line, with no frame for the
leaf. Leaf fns skip `gem_push_frame` for speed, so the fix must stay cheap.

### Wrong line for an error in a fn's last expression

```gem
fn g() 1 end
fn f(x)
  let y = g()
  print("hi")
  x + y              # error is here
end
f("a")
```

The report points at line 4 (`print("hi")`), and the trace has no
`at main` frame.

## Editor grammars

### tree-sitter grammar gaps

- No `;` statement separator.
- About 21 repo files parse with ERROR nodes (e.g. examples/06_blocks.gem,
  examples 115–126, std/http.gem, `load ... (names)` in compiler/*.gem).
- `"""` nested in a `"""` interpolation is an ERROR node, and probably ends
  the outer string early in the VS Code grammar too.

Not re-checked after #28 (no tree-sitter CLI in that environment).
