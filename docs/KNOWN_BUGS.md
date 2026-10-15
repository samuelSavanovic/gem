# Known bugs

Bugs found and not yet fixed. Each entry has a minimal repro (checked
against `build/gem`), what goes wrong, and where the code is. A fix deletes
its entry in the same change, along with any **(bug)** rule in
`docs/BEST_PRACTICES.md` that exists because of it. Performance problems go in
`docs/OPTIMIZATIONS.md`, missing features in `docs/ROADMAP.md`.

## Compiler

### Pattern bindings are emitted at line 0

A variable a `match` or `receive` pattern binds is emitted with
`#line 0` and `gem_set_line(0)` (`gem_leaf_line = 0` in a leaf fn).
Run from a file in the repository's root:

```gem
fn f(m)
  match m
  when {a: a}
    print(a)
  end
end
f({a: 1})
```

`build/gem f.gem --emit-c` shows `#line 0 "f.gem"` before
`GemVal gem_v_a = gem_table_get(...)`. `cc -Wpedantic` warns about each
one (clang: `-Wgnu-zero-line-directive`), and a debugger or profiler
puts the binding on line 0. Runtime traces are unaffected: a binding is
a field read that cannot raise, and the next statement sets its own
line. `pattern_to_cond` in compiler/lower.gem builds the bindings with
`make_let(name, target, 0)` (the `pat_table` shorthand and `pat_name`
cases); the pattern's line would do. Part of "A pass over the generated
C" in docs/OPTIMIZATIONS.md.

## Runtime

### stb_ds's 8-byte key hash shifts into an int's sign bit

`stbds_hash_bytes` (runtime/stb_ds.h, the `len == 8` case) builds the
hash from `d[i] << 24` with `d` an `unsigned char *`, so a byte of 128
or more shifts into the sign bit of an `int`: undefined behaviour, which
the `asan` runtime's UBSan stops at. Repro:

```bash
make asan
GEM_RUNTIME=asan build/gem examples/203_many_processes.gem -o /tmp/p203 && /tmp/p203
```

prints `runtime/stb_ds.h:1180:61: runtime error: left shift of 219 by 24
places cannot be represented in type 'int'` and aborts. Release builds
print the right output. Casting each byte to `size_t` before the shift
would do.

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
