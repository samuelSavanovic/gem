# Known bugs

Bugs found and not yet fixed. Each entry has a minimal repro (checked
against `build/gem`), what goes wrong, and where the code is. A fix deletes
its entry in the same change, together with any **(bug)** rule in
`docs/BEST_PRACTICES.md` that exists because of it. Performance problems go
in `docs/OPTIMIZATIONS.md`, missing features in `docs/ROADMAP.md`.

## Compiler diagnostics

### Shadowed loop counter loops forever, silently

```gem
let n = 3
let i = 0
while i < n
  let i = i + 1      # new variable; the outer `i` never changes
end
```

The loop never ends and nothing is reported. A second `let` shadows by
design, so the code is "correct", but this is a silent cliff. A warning
when a `while` body `let` shadows a name the condition reads, and the outer
variable is never assigned in the loop, would surface it.

### Spurious "`break` outside a loop" after a parse error

```gem
fn times(n, f) f(1) end
for x in [1, 2]
  times(1) { |i| if i then 1 + end }
  break
end
```

Besides the real parse errors on line 3, the compiler reports "`break`
outside a loop" for line 4, which is inside the `for`. Parser error
recovery loses the loop context (the check lives in the parser, see #28
fix 6).

### `"""` nested in a `"""` interpolation takes the wrong dedent width

```gem
let s = """
    outer {"""
      inner
      """}
    tail
    """
```

The outer string's dedent width comes from the inner closing `"""` line:
the lexer's pre-scan (compiler/lexer.gem) finds the inner one first.

### `gem --help` is read as a source path

`gem --help` prints `[Runtime Error]: read_file: cannot open '--help'` with
a stack trace into compiler/main.gem. Unknown `--` flags should be a usage
error, and `--help` should print usage.

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

### `build_string`'s `add` captured in a spawn aborts the program

```gem
let s = build_string do |add|
  spawn do
    add("x")
  end
  sleep(20)
end
```

The whole program aborts with `gem_arena: mmap failed (size=...)`. `add`
can't meaningfully run in another process (the buffer lives in the
builder's arena), but it must fail as a Gem error, not corrupt memory.

### `read_file` on procfs returns `""`

`read_file("/proc/self/status")` returns an empty string: the file size is
taken from `fseek`/`ftell`, which give 0 for procfs files
(runtime/gem_builtins_io.c, runtime/gem_threadpool.c). Read until EOF instead.

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

### Exit reasons leak on the kill/link paths

Reported in the original compiler-fix handoff; no simple repro yet. Find
one before fixing.

### `exec` in a spawned process passes stdout through

`spawn do exec("echo hi") end` prints `hi` on the program's stdout. May be
intended; decide, and document the behavior in SPEC.md either way.

## Editor grammars

### tree-sitter grammar gaps

- No `;` statement separator.
- About 21 repo files parse with ERROR nodes (e.g. examples/06_blocks.gem,
  examples 115–126, std/http.gem, `load ... (names)` in compiler/*.gem).
- `"""` nested in a `"""` interpolation is an ERROR node, and probably ends
  the outer string early in the VS Code grammar too.

Not re-checked after #28 (no tree-sitter CLI in that environment).
