# Gem Language Spec v0.1

_(working name, obviously)_

## Implementation Strategy

The compiler is self-hosting — written in Gem, compiling itself. A checked-in C file (`bootstrap/stage0.c`) serves as the bootstrap artifact so any C compiler can rebuild from scratch. No Python, no external parser generators.

Compilation target is C source code. `gcc`/`clang` handles optimization and linking. This gives us free C interop since we're already in native land.

C runtime is minimal glue code wiring together two libraries plus a per-process arena allocator:

- **Per-process arena allocation** — each process (coroutine) gets its own arena (bump allocator). When a process spawns another, values are deep-copied into the new process's arena. When a process dies, its entire arena is freed at once. Messages (`send`) are deep-copied into the target process's arena, and so is the parent's module state at `spawn` (each process has its own copy of the module-level bindings, see Variables). No global GC pauses — memory is reclaimed per-process on exit, and loops reclaim their own garbage as they run (see Memory Model).
- **minicoro** — single-header stackful coroutines (libdill is abandoned, crashes on arm64 macOS). Runtime builds scheduler + channels on top (~150 lines)
- **stb_ds.h** — single-header hash maps and dynamic arrays for table implementation

## Compiler CLI

```
gem <file.gem>              # compile (at -O0, cached) and run it
gem <file.gem> [args...]    # run, passing extra args through to the program
gem <file.gem> -- [args...] # the same; after --, options go to the program too
gem <file.gem> -c           # compile to ./<basename>, don't run
gem <file.gem> -o <name>    # compile to <name>, don't run (implies -c)
gem <file.gem> --emit-c     # print generated C to stdout (used for bootstrapping)
gem <file.gem> --check      # parse + analyze, exit 0 on success, 1 on error
gem <file.gem> --run        # accepted as a no-op; run is the default
gem --help                  # print usage (also -h), exit 0
gem lsp                     # start the language server on stdin/stdout
gem new <name>              # make a project directory <name>/
gem test [args...]          # run the project's test.gem
gem build [cc flags]        # build the project's main at -O2 into build/<name>
```

Options can come before or after the source path. Before it, an argument starting with `-` that is not one of the options above is a usage error: `gem` prints a short message and the usage line on stderr and exits 2 (as for a missing source path or `-o` without a name). After the source path, an unknown argument is passed to the program, so `gem prog.gem --help` gives `--help` to `prog.gem`.

A source path that cannot be read prints one line on stderr and exits 1: `gem: cannot open 'missing.gem'`, or `gem: 'src' is a directory, not a source file`.

Nesting is limited, so that neither the compiler nor the C compiler runs out of stack. Each statement, expression, unary operator and pattern opens a level while it is parsed (a top-level statement and its expression are two; each `elif` of a chain nests one more), and opening more than 200 is a compile error (`nested too deeply: more than 200 levels`). Counting also each link of an operator chain (`a + b + c`) or postfix chain (`t.a.b`, `t[0][1]`, `f()()`), more than 1,000 levels is a compile error too (`expression too deep: more than 1000 levels`). Split such code with `let` or a helper fn.

The default behavior (`gem foo.gem`) writes generated C to `<basename>.c` in a new directory under `$TMPDIR` (else `/tmp`), compiles it at `-O0` (fast to build; Gem code runs about 1.5–3x slower than at `-O2`, while the runtime library stays optimized: 1.5–1.75x on the lox interpreter's benches on Linux x86_64, 2.3–2.8x on macOS arm64), runs it, and removes the directory (with the binary cache on, the binary is built in the cache, below). Extra positional arguments after the source path are forwarded to the program via `argv()`, each as one argument; after `--`, every argument is the program's, options included (`gem tool.gem -- --check`). When `cc` fails, the directory is kept, and `gem` prints where the C file is. The program gets every open file descriptor of `gem` that isn't close-on-exec, not only stdin, stdout and stderr (so `gem prog.gem <(cmd)` works). A binary in the cache replaces the `gem` process, so the program's exit status and signals are its own. The binary runs as a child instead when `GEM_CACHE=off`, when there is no cache directory that can be written, when linking it into the cache fails with no binary there, and when the cached binary is gone twice in a row as the run starts it. Then `gem` exits with the program's status, or 128 plus the signal number when a signal ended it.

**Binary cache.** Run mode keeps the binary it builds and runs it again, without calling `cc`, while nothing it is built from has changed: the key is a hash of the generated C (which holds the code of every file the program loads, so a change to the code of the program, a module or the compiler changes it), the `cc` command and its flags (`CC`, `CFLAGS`, `LDFLAGS`, `-I`/`-L`/`-l`), the contents of the runtime library and of the runtime's headers (`runtime/*.h`; also of its `.c` files when there is no runtime library and the program is built from them), and those of the headers `extern include` names. Not part of the key: libraries named with `-l`, headers an `extern include` leaves to the C compiler's search (found through `-I` or the system include paths, see C Interop), and headers that an included header includes itself. After changing one of those, remove the cache directory; `GEM_CACHE=off` builds without the cache but leaves its entries as they are. The cache lives in `$GEM_CACHE_DIR`, else `$XDG_CACHE_HOME/gem`, else `~/.cache/gem`; it holds up to 256 binaries and is emptied when a new one would go past that (its binaries move to `<cache>/.tmp`, and the first run of a new program a minute or more later removes them). A binary takes about 1.3 MB (hello world) to 1.8 MB (examples/mini_redis) on macOS arm64, so a full cache takes about 350–450 MB. `GEM_CACHE=off` builds every time and keeps nothing. A run builds the binary in a directory of its own under `<cache>/.tmp` and hard-links it into the cache, so runs at the same time never see a half-written binary; the binary in the cache then replaces `gem`, and `argv()[0]` is its path. A run that finds the binary gone when it starts it (another run emptied the cache) builds it again. A program keeps running when the cache is emptied meanwhile, but its `argv()[0]` path is gone.

`-c` (or `--compile-only`) keeps the artifact: compiles at `-O2` to `./<basename>` and exits without running. `-o <name>` does the same but lets you pick the path.

**Projects.** A project is a directory with a `gem.toml` (see Module System, "Project root marker"). `gem.toml` holds `key = "value"` lines; `#` starts a comment (on a line of its own or after a value), and other lines (`[sections]`) are ignored. Two keys are read: `name` (default: the directory's name, without a trailing `.gem`) and `main` (the entry file, default `main.gem`).

- `gem new <name>` makes `<name>/` holding a `gem.toml` (`name` and `main`), a `main.gem` whose `fn main` prints `hello, world`, a `test.gem` with one `std/test` case, and a `.gitignore` for `build/`. It refuses a directory that exists, and a name whose last part is empty, holds `..`, or starts with `.` or `-` (exit 1); a missing name, or one starting with `-`, is a usage error (exit 2).
- `gem test [args...]` runs the project's `test.gem` in run mode, passing every argument to it, options included (as after `--`), and exits with its status.
- `gem build [cc flags]` compiles the project's `main` at `-O2` to `build/<name>` under the project root; `-I`/`-L`/`-l` flags are passed to `cc`. A `name` that holds `/` or `..`, or starts with `.` or `-`, is an error (exit 1).

Both find the project from the current directory upwards (the nearest directory with a `gem.toml`), so they work from any subdirectory; with none, they print ``gem test: no gem.toml in this directory or above it (`gem new <name>` makes a project)`` (or the same after `gem build:`) and exit 1.

**Install.** `make install` (in the repository, after `make build`) copies the compiler, the runtime library and headers and `std/` to `$(PREFIX)/lib/gem/` (`PREFIX` defaults to `/usr/local`; `DESTDIR` stages it) and links `$(PREFIX)/bin/gem` to the compiler, which finds the rest two levels above its real path (see Module System, "install root"). `make uninstall` removes both.

`--emit-c` prints C to stdout and exits. This is what the bootstrapping Makefile targets use.

`GEM_RUNTIME` in the compiler's environment picks the runtime a program is linked with: `release` (the default), `debug` (built by `make debug`: the runtime checks its invariants, poisons the memory a reset frees and checks the heap after resets; a broken invariant prints `gem: runtime invariant broken: ...` and aborts) or `asan` (built by `make asan`: the debug runtime with AddressSanitizer and UndefinedBehaviorSanitizer). The two debug ones compile the program at `-O1 -g`, and are for finding bugs in the compiler and runtime, not for running programs. Under `asan` every process stack is four times its release size, to make room for ASan's larger frames, though a deep recursion can still overflow sooner than in a release build; a program uses several times its release memory. On Linux, LeakSanitizer is off unless `ASAN_OPTIONS` sets `detect_leaks=1`, and then also reports memory that only the process table and arenas point to (macOS arm64 has no LeakSanitizer). Any other value is a usage error (exit 2), and a debug runtime that hasn't been built is an error naming the `make` target.

`--check` runs the parser, load resolution, and codegen analysis but skips emitting C and invoking `cc`. Exits 0 silently on success; on failure, prints the same caret-context diagnostic that the normal compile path produces and exits 1. Intended for editor save hooks, pre-commit, and CI lint.

## Values and Types

Eleven types: `Int`, `Float`, `String`, `Bool`, `Nil`, `Table`, `Fn`, `Buffer`, `Ref`, and the two owned resources `Socket` and `Sqlite` (see Owned Resources). All dynamically typed. Every value is a tagged C union. Yes this means primitives are boxed and slow — doesn't matter for v0. Future optimization: NaN-boxing to pack ints, bools, and nil into a double's NaN space, eliminating heap allocation for primitives.

**Numbers.** An `Int` is a signed 64-bit integer, from -9223372036854775808 to 9223372036854775807; a `Float` is an IEEE 754 double. Integer literals are decimal digits (`42`, `007` is `7`; no hex, binary, octal or `_` separators), and a literal outside that range is a compile error at the literal (`integer literal out of range`). The literal `9223372036854775808` is accepted only right after a unary `-`, so `-9223372036854775808` writes the smallest int; `x - 9223372036854775808` is still an error. A number literal with a decimal point is a float (`2.0`, `0.000001`, `3.14159265358979`). There is no exponent syntax in literals (`1e6` does not lex as a number); `to_float("1e6")` parses one. A float literal keeps every digit: it compiles to the nearest double, as in C.

A float turns into text (`to_string`, `print`, `eprint`, interpolation, `buf_push`, `build_string`'s `add`, and so `json.encode`) as the shortest decimal that reads back to the same double, so `to_float(to_string(x)) == x` for every finite `x`. An integral float keeps a decimal point (`2.0`, `-0.0`, `100.0`), so a float never prints like an int. Magnitudes below `1e-4` or from `1e16` up use exponent form with a signed, at least two-digit exponent (`1e-05`, `1.5e-07`, `1e+16`, `1.2345678901234567e+19`); this is also valid JSON. The non-finite values (from overflow, e.g. `to_float("1e308") * 10.0`) print as `inf`, `-inf` and `nan`; `to_float` reads those back, and `json.encode` raises for them (JSON has no such numbers).

Strings are binary-safe: they carry an explicit byte length, so `"\0"` is a 1-byte string, `len(s)` reports byte length (not strlen), and embedded NULs survive concatenation, indexing, equality, `build_string`, `tcp_write`, and `read_file`/`write_file` round-trips. Strings remain NUL-terminated for C interop convenience; the byte after the last content byte is always `\0`. A string or buffer holds at most 2,147,483,646 bytes (2 GiB − 2): anything that would make a longer one (`+`, interpolation, `buf_push`, `build_string`, `str_replace`, `read_file`, `read_stdin`, `input`, an extern `Bytes` return) raises `"<op>: a string of N bytes is over the limit of 2147483646 bytes"` (`read_file` raises `"read_file: '<path>' is N bytes, over the string limit of 2147483646 bytes"` for a regular file, before reading it, and `"read_file: '<path>' holds more than the string limit of 2147483646 bytes"` for a pipe or device; an extern fn's message names its C function). **Caveat — `extern fn`:** when a Gem `String` is passed to an `extern fn ... s: String` parameter, it marshals as `const char *` and the C side will see only the bytes up to the first NUL. Use the `Bytes` extern type (see C Interop) when the C function needs binary data — it marshals the byte length alongside the pointer.

## Memory Model

Two rules: inside a process, values are shared; between processes, nothing is.

**Inside a process, tables, buffers and closures are references.** Assigning a table to another variable, passing it to a function or storing it in another table does not copy it, and a change made through one name shows through every other. `==` on tables compares identity (see Operators). `table.copy` makes a shallow copy when one holder must keep the old contents. Strings, numbers, bools and `nil` are values: `s2 = s1` followed by `s2 += "x"` leaves `s1` unchanged. A closure shares the variables it captures with the scope that created it (see Closures).

```
let a = {x: 1}
let b = a
b.x = 2
print(a.x)        # 2 — a and b are the same table

fn bump(t) t.x = t.x + 1 end
bump(a)
print(a.x)        # 3
```

**Between processes, every value is copied.** Each process has its own heap (an arena). `send` deep-copies the message into the receiver's arena; `spawn` copies the variables its closure captures and the module-level bindings (see Module-level bindings are per-process). After the copy the two sides are independent: a change on either side is never seen by the other, and no lock is ever needed. A copy keeps the shape of what it copies: two references to one table stay one table on the receiving side, and a table that contains itself is copied as a cycle. Because the copy is a different table, a received table is never `==` to the one sent.

```
let shared = [1]
let pid = spawn do
  receive
  when {from: from, m: m}
    m.p[0] = 99                          # changes the receiver's copy only
    send(from, {same: m.p == m.q, val: m.q[0]})
  end
end
send(pid, {from: self(), m: {p: shared, q: shared}})
print(receive())  # {same: true, val: 99}
print(shared[0])  # 1
```

**Memory is reclaimed without a tracing GC or a global pause.** There is no per-object `free` and no collector that walks every process. Memory comes back in three ways:

- When a process exits, its whole arena is freed at once. A short-lived process (a request handler, a task) needs nothing else.
- At the back-edge of every loop — `while`, `for`, self tail calls, mutual tail calls — the runtime copies what the loop still uses, as computed by the compiler's liveness analysis, and frees everything else the loop allocated. This is what keeps a `while true` server loop or a recursive `loop(state)` at constant memory (see Long-Running Processes).
- When a function that can recurse returns, once its call has allocated enough (at least 1 MB) that no reset has freed yet, the runtime copies the return value (if any) and frees everything else the call allocated. A recursion so frees its calls' garbage as they return: its memory grows with its depth, not with the number of calls (see Return resets).

Other garbage (made outside any loop, or in a call of a function that cannot recurse) is freed when an enclosing loop or recursive call resets or when the process exits.

A reset works on one process's arena only: it never scans or moves another process's memory, and its cost depends on that process's data alone. Processes share one scheduler thread, so while a reset runs the others wait, as they do for any other work in that process.

**What it costs.**

- `send` and `spawn` cost time proportional to the size of what they copy, strings included. Send what the receiver needs, not a large state table; keep large shared data in a process that answers queries.
- A loop reset costs time proportional to what the loop made since its last reset and still uses: what a reset keeps is not copied again by the next ones. Now and then a full reset copies everything the loop keeps, to free what it no longer holds; it comes once the kept data has doubled since the last one, so a loop that holds a large state does not slow down as it grows, but it pauses the other processes for one copy of that state at a time.

**When memory grows.**

- A non-tail recursive call keeps its frame and its garbage until it returns: `loop(state)` followed by another statement is not a loop (see Tail Call Optimization).
- A `while true` loop whose live values the compiler cannot root runs without resetting, and the compiler warns: `cannot reset per-process arena at this loop's back-edge` (see When the back-edge reset cannot fire).
- The main process's arena lives until the program ends, so top-level straight-line code that allocates a lot keeps it.

**C code sees arena pointers only for the duration of a call.** A `String`, `Bytes` or `Table` passed to an `extern fn` points into the process's arena and can move at the next loop reset; C code must copy anything it keeps (see C Interop, Pointer lifetime).

## Variables

```
let x = 10
let name = "hello"
x = 20
```

A name is a letter or `_` followed by letters, digits and `_`. Names starting with `__` are reserved for the compiler, which names its own temporaries and renamed bindings that way: using one for a variable, function, parameter, `for` variable or pattern binding, or reading one, is a compile error (`` `__x`: names starting with `__` are reserved for the compiler ``). A field name (`t.__x`), a table key (`{__x: 1}`, `when {__x: v}`) and an `extern fn` parameter name (`extern fn f(__n: Int)`, which binds nothing in Gem) may start with `__`. A call of one of the compiler's internal builtins (`__is_array_n(t, n)`, which std calls) compiles too; they are not part of the language and may change.

### Scope

A variable declared with `let` is visible from its declaration to the end of the block that contains the `let`, and nowhere else. Blocks are function and closure bodies (including `do` blocks), `if`/`elif`/`else` branches, `while` and `for` bodies, and `match`/`receive` arms (including `after`). `for` loop variables are scoped to the loop body and `match`/`receive` pattern bindings to their arm.

Using a name where none of its declarations is visible — after the block that declared it ended, or before its `let` (for example in a `while` condition, reading a `let` of the loop body) — is a compile error (`undeclared identifier`). That holds even when every branch of an `if` declares the name, and in a loop, where a `let` from an earlier iteration is gone. To use a value after a block, declare the variable before it and assign it inside:

```
fn sign(n)
  let s = nil          # declared before the if: visible after it
  if n < 0 then s = "-" else s = "+" end
  s
end

fn broken(n)
  if n < 0 then let s = "-" else let s = "+" end
  s                    # error: undeclared identifier `s` (each `s` ended with its branch)
end
```

If an outer variable of the same name is visible, a use after the block refers to that outer variable (see Shadowing). A `let` directly in a file's top-level code is a module-level binding, visible to all of the file's code (see below); a `let` nested in a top-level block is block-scoped like any other.

### Shadowing

A `let` always declares a new variable. If a variable of the same name is already visible (a parameter, an earlier `let` in the same or an enclosing block, a captured local of an enclosing function, a module-level binding, a named function, a builtin), the new one **shadows** it:

- The initializer is evaluated before the new variable exists, so it reads the old one: `let n = n - 1` reads the previous `n`.
- Later code, including assignments (`n = …`, `n += 1`), refers to the new variable. The old one is not changed.
- Closures created before the shadowing `let` keep the old variable; closures created after it see the new one.
- A shadow made inside a nested block (`if`/`elif`/`else`, `while` and `for` bodies, `match`/`receive` arms, closure bodies) ends with that block: after it the outer variable is visible again, unchanged. In a loop body every iteration starts from the outer variable.
- **A `let` in a nested block that hides a variable of an enclosing block of the same function** (a parameter, or a `let` or `for` variable of an enclosing block; in top-level code, a module-level `let` or a `let` of an enclosing top-level block; a module-level `fn`, `extern fn` or `load` name doesn't count) **is a compile error**: `` `let total` hides the `total` of an enclosing block (line 6): it makes a new variable, and the outer one keeps its value ``. To change the outer variable, assign it; for a new one, pick another name. A `let` of a name the same block already declares (an earlier `let` there, the `for` variable of that loop, a pattern binding of that arm) rebinds it there and is allowed, as is a `let` in a closure or a named function, which is a function of its own: `let n = n - 1` at the top of a body, `for line in lines` with `let line = string.trim(line)` in the loop, `fn() let x = 1 ... end` next to an outer `x`.
- `for` loop variables and `match`/`receive` pattern bindings are lets of their body and shadow the same way.
- A parameter (plain, defaulted, rest or destructured) shadows an outer variable, a module-level binding, a named or `extern` function or a builtin of the same name in the whole function body, closures and `do` blocks in it included: in `fn show(item) fn() item end end` the closure reads the argument even when a top-level `fn item` exists.
- **Warning:** a `let` in a `while` body (at any depth, not inside a closure) that shadows a variable the loop's condition reads, when nothing in the loop body assigns that name (`x = …` or `x += …`, closures included), gets a compile-time warning on stderr. The condition reads the outer variable, which then can't change through it: `while i < n` with `let i = i + 1` in the body never ends. To advance the counter, assign it: `i = i + 1`. Any assignment to the name in the body turns the warning off, as does renaming the new variable. A name that a module-level variable has is not warned about when a function or closure of the same file assigns it, since a call in the loop can change the module-level variable.
- **Warning:** a `let` in the body of any loop (at any depth, not inside a closure) whose value reads the name it declares (`let total = total + x`), when that name is not declared inside the loop before it (a loop variable, an earlier `let`, a pattern binding), nothing in the loop assigns it, and a statement after the loop (in its block or an enclosing one, up to the function body) reads it, gets a compile-time warning on stderr: each iteration computes a new variable from the outer one, which keeps its value from before the loop. To accumulate, assign: `total = total + x`. The same exceptions apply as for the warning above.

```
fn f(n)
  let before = fn() n end
  let n = n - 1        # new n, initialized from the parameter
  if n > 0
    let n10 = n * 10   # `let n` here would be a compile error
    print(n10)         # 40
  end
  print(n, before())   # 4 5
end
f(5)
```

The exception is a `let` directly in a file's top-level code: that declares the module-level binding of the name (see below), of which there is one per name. A second top-level `let` of the same name rebinds that module-level binding, as an assignment would; named functions and closures, which always read module-level bindings live, see the new value. A module-level name has one kind of definition: a `fn`, an `extern fn`, or `let`s (the names a selective `load "x" (a, b)` binds count as `let`s; the module namespace a plain `load` or `load ... as` binds does not, see the next sentence). A module-level `let` named like a module the file loads (`load "std/json"` and `let json = ...`, or `load "x" as m` and `let m = ...`), in either order, is a compile error: it would hide the module in every function of the file (`` `json` names the module loaded at line 1; a module-level `let json` would hide it in every fn of this file ``). A `fn` or `extern fn` with the name of another module-level `let`, `load` binding, `fn` or `extern fn` in the same file, in either order, is a compile error at the second definition (`` `helper` is already defined at line 1 (a `let`)``).

### Destructuring

Destructuring extracts fields from tables or elements from arrays:

```
# Table destructuring — extract named fields
let {method, path} = parse_request(raw)

# Array destructuring — extract by position
let [first, second] = string.split(line, ",")

# Per-field defaults — apply when the field is missing
let {strategy = "one_for_one", max_restarts = 3} = spec
let [head, tail = []] = parts
```

Table destructuring extracts by name (`let {a, b} = expr` is `let a = expr.a; let b = expr.b`). Array destructuring extracts by index (`let [a, b] = expr` is `let a = expr[0]; let b = expr[1]`). The RHS is evaluated exactly once. Missing keys/indices produce `nil`. Per-field defaults (`name = expr`) substitute when the key is missing from the table (for an array pattern, the index), exactly like a default parameter fires only for an argument left out: an explicit `nil` stays `nil` (`let {a = 5} = {a: nil}` binds `a = nil`, `let [x, y = 2] = [1]` binds `y = 2`). So an option table can give `nil` a meaning of its own, as std does with `timeout_ms: nil` for no limit, and a caller that wants the default leaves the key out. The default is only evaluated when it fires and may reference earlier names in the same destructure. The value is read first, so destructuring a non-table raises the same error with or without defaults. Like any `let` initializer, a default that names the field it binds sees the binding it shadows: in `fn f(port) let {port = port + 1} = {} ... end`, the default reads the parameter. Patterns are flat — no renaming, nesting, or rest/splat; a `{key: name}` entry in a `let` or fn-param pattern is a compile error (only match/receive patterns take `key: pattern`). Match/receive patterns are stricter and do **not** accept defaults; defaults are a binding-context feature only (let, fn params).

### Module-level bindings are per-process

A `let` at the top level of a file (including loaded modules' private `let`s and the namespace tables `load` creates) is a module-level binding, visible from every function and closure in the file. Module state follows the Erlang model — there is no memory shared between processes:

- Every process has its own copy of the module-level bindings. `spawn` gives the child a deep copy of the parent's values at the time of the spawn (one copy for the whole set, so two bindings that refer to the same table still do in the child, and so does a captured local that refers to one). Later changes in the parent, in place or by reassignment, are not seen by the child. The copy is made lazily: a binding is copied the first time the child uses it, so a large module-level table costs nothing in a child that never reads it. Namespace tables (`string`, `log`, …) are per-process like every other binding, and frozen: changing their own entries (`m.x = …`, `push(m, …)`, `delete(m, …)`, `insert`, `pop`, `sort`, `remove_at`) raises `cannot modify a module table`. Copies of a namespace (spawn, send) stay frozen.
- A write to a module-level binding — `x = …`, `x.field = …`, `push(x, …)` — changes only the running process's copy. Other processes, including the parent, never see it; a child spawned later starts from the parent's values at that time.
- Named functions, closures and top-level code all read the current value of the running process's copy. Closures do not snapshot module-level bindings when they are created.
- Only a `let` directly in the file's top-level code (a destructuring `let` included) declares a module-level binding. A `let`, `for` loop variable or pattern binding inside a top-level `if`, `while`, `for`, `match` or `receive` is block-scoped, exactly as inside a function: it shadows a module-level binding of the same name until its block ends and never changes it, and a closure created in the block captures the block's binding (in a loop, a fresh one per iteration). Named functions always see the module-level binding.

```
let count = 0
fn bump() count += 1 end
bump()                       # main: count == 1
spawn do
  bump()                     # this process: count == 2
  print(count)               # 2
end
print(count)                 # 1 — the child's write stayed in the child
```

To share state between processes, put it in a process and talk to it with messages (`gen_server`, `register`). The compiler prints a `note:` at each write to module state in code reachable from a `spawn` body, as a reminder that the write is process-local; writes inside the std modules are left out (their module state, such as `log.set_level`'s level, is per process by design). The note names the binding as you write it in source (`count` in the entry file, `counter.count` for the top-level `count` of module `counter.gem`, even when it is loaded with an alias), with project-relative paths.

## Functions

```
fn add(a, b)
  a + b
end

fn greet(name)
  print("hello " + name)
end
```

Last expression is implicit return. Explicit `return` also works.

A named `fn` is a definition in a file's top-level code only. One inside a function or `do` body, or inside a top-level `if`, `while`, `for`, `match` or `receive` block, is a compile error; bind an anonymous function instead (`let helper = fn() ... end`), or move the definition to the top level.

Function call argument lists allow newlines inside the parentheses — after `(`, after each `,`, and before `)`:

```
let result = add(
  long_expression_a,
  long_expression_b
)
```

Table and array literals may span lines the same way. Other expressions may not: a line break after a binary operator or inside parentheses ends the statement (`let t = (1 +` followed by `2)` on the next line is a parse error), and a line that starts with `-` is a new statement (`let x = 1` followed by `- 2` on the next line is the statement `- 2`). An expression statement that does nothing (a fn literal, or an expression with no call and no assignment in it: a literal other than `nil`, a variable, an operator, a field or index read, an array or table literal) is a compile error, `` a negation on its own does nothing (in the middle of a block) ``, where its value is not used: anywhere but the last statement of a block, and also as the last statement of a loop body or of top-level code. The last statement of a function, closure or `if`/`match`/`receive` branch is the block's value and may be anything; a bare `nil` is always allowed, to write an empty branch. Parameter lists in a `fn` definition must fit on one line. Split a long condition into named `let`s.

A parameter list names each parameter once (destructured fields and the rest parameter included); a repeated name is a compile error, `duplicate parameter 'a'`. A call to a named function, an `extern fn`, or a module-level `let name = fn(...)` that is declared once and never assigned, by name, as a module export (`m.f(...)`) or through a name a selective `load` imports, is checked at compile time: it must pass at least the parameters without a default and at most all of them (any number more with a rest parameter), else it is a compile error, `` `h` takes 2 arguments, called with 1 ``, whose hint shows the declaration. A trailing `do` block counts as the last argument. A call through a value (a variable, parameter or table field holding a function) is not checked: there, extra arguments are dropped and missing ones are `nil` (or the parameter's default). To make a parameter optional, give it a default (`b = nil`). Calling a non-function value is a runtime error.

Evaluation runs left to right: the callee, then each argument in order (a field access or a call inside one included), then the call. The same holds for the two operands of a binary operator (`f() + g()` calls `f` first), the parts of an interpolated string, the entries of a table or array literal, a table index (`t[k()]` evaluates `t` before `k()`), and an assignment to a field or index (object, key, then value). `and` and `or` evaluate their left operand once, and the right one only when needed.

A named function is a module-level binding: two `fn`s of the same name in one file, or a `fn` and a top-level `let` or `extern fn` of the same name, are a compile error at the second one (see Shadowing). A `let` of that name inside a function or block shadows the function as usual.

If the entry file defines `fn main()`, the compiler calls it with no arguments after the file's top-level code has run. Don't also call it yourself, or it runs twice. Use `argv()` for command-line arguments.

## Variadic Functions

A `...name` rest parameter collects extra positional arguments into an array. It must be the last declared parameter.

```
fn sum(...nums)
  let total = 0
  for n in nums
    total = total + n
  end
  total
end

sum(1, 2, 3)   # 6
sum()          # 0 — nums is []
```

Required parameters before `...` work normally:

```
fn log(level, ...msgs)
  for msg in msgs
    print("{level}: {msg}")
  end
end

log("INFO", "started", "port 8080")
```

Rest param packing happens inside the function body at the C level, so variadic functions also work when stored in a variable or passed as a callback — the packing is unconditional regardless of call form.

## Default Parameters

Parameters can have default values with `= expr`. The default expression is evaluated at call time (not definition time) when the caller passes fewer arguments.

```
fn greet(name, greeting = "Hello")
  print("{greeting}, {name}!")
end

greet("Alice")           # Hello, Alice!
greet("Alice", "Hey")    # Hey, Alice!
```

Required parameters must come before optional ones — a required parameter after an optional one is a compile error. Default expressions can reference earlier parameters:

```
fn repeat(s, n = len(s))
  ...
end
```

Default parameters work in named functions, anonymous functions, and block parameters. Passing `nil` explicitly does *not* trigger the default — only omitting the argument does, as a destructured field's default fires only for a missing key (see Destructuring). A default is evaluated at call time, each time the argument is omitted, and may refer to earlier parameters. Naming its own parameter or a later one is a compile error (`undeclared identifier`), even when an outer binding has that name: those parameters aren't bound yet.

## Destructuring Parameters

A function parameter position can be a flat table-destructure pattern. The named fields are bound as locals at the start of the body, and per-field defaults work the same way as in `let`-destructuring (the default fires only when the field is missing):

```
fn start({port = 8080, host = "0.0.0.0", name = nil})
  ...
end

start({port: 9000})              # host defaults, name defaults to nil
start({port: nil})               # port is nil: an explicit nil is not "left out"
start({})                        # all defaults
```

A trailing `= {}` makes the whole argument optional and also coerces an explicit `nil` argument to an empty table — useful for "options bag" APIs:

```
fn set_cookie(resp, name, value, {path = "/", http_only = true, secure = false} = {})
  ...
end

set_cookie(resp, "sid", "abc")           # uses all defaults
set_cookie(resp, "sid", "abc", nil)      # same — nil is coerced to {}
set_cookie(resp, "sid", "abc", {secure: true})
```

**Option bags reject unknown keys.** A destructured parameter with a trailing `= <default>` (`{port = 80} = {}`) is an option bag: when the function is called, a key of the table that the pattern doesn't name raises `<fn>: unknown option "<key>"`, with the closest field when one is near (`start: unknown option "prot" (did you mean "port"?)`; `<fn>` is the function's frame name, `anonymous fn` for a `fn` literal). The check runs at the start of the body, before the fields are read; a non-table argument is left to the field reads. A pattern without the trailing default takes a record, not options, even when its fields have defaults (`fn add({x, y})`, `fn handle({type, data = nil})`): extra fields are ignored.

A last entry `...rest` takes the keys the pattern doesn't name, in a new table (empty when there are none), and turns the check off — for options passed on to another function:

```
fn log_line(msg, {level = "info", ...rest} = {})
  write(level, msg, rest)        # rest: every other option, as given
end
```

Patterns are flat (no nested destructuring) and table-only — array-destructured parameters are not supported. `...rest` is only for parameters (a `let` pattern has none). Param destructuring composes with regular and rest parameters, and with tail call optimization.

## Blocks

This is the core feature. Any function can accept a trailing block with `do`/`end`. The block is passed as the last argument as an `Fn` value.

```
fn times(n, body)
  let i = 0
  while i < n
    body(i)
    i = i + 1
  end
end

times(5) do |i|
  print(i)
end
```

Single-expression blocks use braces. The body is one expression, assignment, or whole `if`, `match` or `receive`:

```
times(5) { |i| print(i) }
each(items) { |x| if x > 0 then print(x) end }
```

When the block is the only argument, the parentheses can be dropped. A brace block then needs its `|params|` (use `||` for none), so it is not read as a table literal:

```
let pid = spawn do
  serve()
end
let r = pcall do
  risky()
end
apply { |x| x * 2 }
```

The `do` or `{` must be on the same line as the call.

A trailing block cannot follow a call in an `if`, `elif`, `while` or `for` header (except inside the bodies of an `if`, `match` or `receive` used as a value there), and a header takes no `do`: `while running do` is reported as an error. To pass a block to a call there, assign the call's result to a variable first.

This one rule lets std define `each`, `map`, `filter` and friends without compiler changes.

## Tables

```
let point = { x: 10, y: 20 }
point.x
point["x"]

let list = [1, 2, 3]
list[0]
```

`{ }` with keys is a table. `[ ]` is sugar for an integer-keyed table. Tables are mutable and passed by reference within a process, and copied when sent to another (see Memory Model). Dot access is sugar for string key lookup. Keywords are allowed as table keys and dot fields: `{else: body}`, `node.else`. A literal key is a name or keyword (a string key), a string (`{"x y": 1}`; `{"5": 1}` has the string key `"5"`), or a non-negative int literal (`{0: "a", 10: "b"}`), which reads like any int literal: `{010: x}` has the key `10`, and a key outside the int range is a compile error. A negative int key (`{-1: x}`) is a compile error, since negative ints index from the end (see Negative array indexing). A float literal key (`{1.5: x}`) is a compile error too; set a float key with an index (`t[1.5] = x`). `nil` and NaN can't be keys: storing one raises `table key is nil` / `table key is NaN` (no lookup could find the entry). Table patterns take the same keys: `when {1: x}` matches a table with the int key `1`, `when {"1": x}` one with the string key `"1"`.

Tables can have methods via closures:

```
fn range(start, stop)
  {
    start: start,
    stop: stop,
    each: fn(body)
      let i = start
      while i < stop
        body(i)
        i = i + 1
      end
    end
  }
end

range(1, 10).each do |i|
  print(i)
end
```

## Control Flow

```
if x > 10
  print("big")
elif x > 5
  print("medium")
else
  print("small")
end

# Optional 'then' keyword — allows single-line bodies
if x > 10 then "big" else "small" end

while running
  step()
end

match tag
when "add"
  compile_add(node)
when "call"
  compile_call(node)
else
  error("unknown: " + tag)
end

# One-line arms: 'then' after the pattern, as with 'if ... then'
match n
when 0 then "zero"
when 1 then "one"
else "many"
end
```

`if`, `match` and `receive` are statements that give a value: the last expression of the branch taken.
One can be used anywhere an expression can: the value of a `let` (destructuring included), of an
assignment (`=`, `+=` and the other compound forms, a field or an index) or of a `return`, a call
argument, an operand, a table or array entry, an interpolation, a condition. Its value is also used where
it is the last statement of a function, closure or block (`fn size(x) if x > 10 then "big" else "small"
end end`):

```
let label = if n > 100 then "big" else "small" end
print("{n} is {if n % 2 == 0 then "even" else "odd" end}")
let reply = match msg
when {tag: "get"} then state
else nil
end
count += if hit then 1 else 0 end
let total = base + match kind
when "a" then 1
else 2
end
return match n
when 0 then acc
else loop(n - 1, acc + n)
end
```

A branch that ends in a statement (a `let`, a loop, an assignment) gives `nil`, and so does an `if`
with no `else` whose conditions are all false. A `match` with no `else` whose arms all fail raises
`no match for <value>` (the target as `print` shows it inside a table, strings quoted, cut at 200
bytes), as Erlang's `case_clause` does; write `else nil` for a `match` that may match nothing. A
branch may end in `return`, `break` or `continue`, which leaves the function or loop before the rest
of the expression runs (`print(if x == nil then return 0 else x end)`); in a `while` condition,
`break` and `continue` are a compile error (inside another loop, `` `break` can't be used in a
`while` condition ``), except in a loop or fn literal of their own there. In `let n = if n == nil then
0 else n end` the block reads the outer `n`. In `return <block>`, a call at the end of a branch is a
tail call (see Tail Call Optimization).

Evaluation stays left to right (see Functions): in `f(g(), if c then h() end)`, `g()` runs before the
block. The right operand of `and` / `or` runs only when it is needed, so a block there does too, and
a block in a `while` condition runs before every iteration.

The block ends at its `end`, and the expression it is part of goes on from there: `let x = if c then
1 else 2 end + 1`. A statement that starts with `if`, `match` or `receive` is that block alone, so
`if ... end + 1` there is an error; write `(if ... end) + 1` instead. A brace block's body is an
expression, so there it goes on: `{ |x| if x then 1 else 2 end + 1 }`.

Two places can't hold one: a parameter default (`` an `if` can't be a parameter default ``; compute
the value in the function body) and the value of a `when` arm (`` a `match` can't be a `when` arm's
value ``; bind it with a `let` before the block and pin it, `when ^v`). A fn literal inside either
can use them as usual.

A `when` arm's body starts either on the line after the pattern or, after `then`, on the same line (`then` must be on the pattern's line, like `if <cond> then`; the body may continue onto further lines). Anything else on the pattern's line is a compile error: `when x nil` reports "expected `then` or a newline after the `when` pattern". This holds for `match` and `receive` arms alike. A `receive`'s `after <ms>` clause also accepts an optional `then` (`after 100 then retry()`).

## Destructuring Patterns in Match

When clauses support destructuring patterns that match on the shape of data and bind inner values to variables:

```
# Table pattern — match if target has these keys, recurse on values
match response
when {ok: true, value: v}
  print(v)
when {ok: false, error: msg}
  print("failed: " + msg)
end

# Array pattern — match an array of exactly N elements and bind them by position
match point
when [x, y, 0]
  print("2D point")
when [x, y, z]
  print("3D point")
end

# Bare name — catch-all with binding
match val
when x
  print("caught: " + to_string(x))
end

# Nested patterns
match event
when {type: "click", pos: [x, y]}
  handle_click(x, y)
end

# Pin — compare against an existing variable instead of binding
let expected = "admin"
match user
when {role: ^expected}
  grant()
end
```

Pattern rules:
- `{key: pattern, ...}` — checks target is a table, each key exists, and recursively matches each value against its sub-pattern. Extra keys in the target are ignored (partial match).
- `[p1, p2, ...]` — checks target is a table with exactly N entries whose keys are the ints `0 .. N-1`, then recursively matches each element. A record with N string keys, or an array with an extra string key, does not match. `[]` matches any empty table (`{}` and `[]` are the same value). There is no rest element: `[x, y]` never matches a 3-element array.
- There are no guards or alternatives: an expression arm whose expression is a comparison (`==`, `!=`, `<`, `>`, `<=`, `>=`), `in`, `and`, `or` or `not` (`when v > 5`, `when "a" or "b"`) is a compile error, `` a `when` arm can't test a condition ``, since it would compare the target with the expression's value (`true`/`false`, or an operand of `and`/`or`). Use an `if` chain, or one arm per value.
- A literal (int, float, string, bool) in pattern position matches by equality. `nil` is also a literal — `when nil` matches only `nil`, it does not bind a variable.
- A name in pattern position is a variable binding — always matches and binds the matched value. A name that is already visible there (a local, a parameter, a module-level binding, a function, a name a `load` binds) is a compile error, since the arm would match anything where a comparison was almost always meant: `` `STATUS_OK` in this pattern binds a new variable and matches anything; it doesn't compare with the `STATUS_OK` in scope ``. Pin it (`^STATUS_OK`) to compare, or pick another name; `_` never counts.
- `^name` (a pin) matches by equality with the current value of the variable `name`; it binds nothing. The pinned name must already be in scope, and only a plain variable name can be pinned — bind an expression like `t.ref` to a local first. `when ^x` works at the top of a `match` or `receive` arm as well as inside table and array patterns. The comparison is `==`, so a pinned table matches only that same table — never a copy received in a message. Pin primitives and refs.
- A bare name after `when` (e.g., `when x`) is a catch-all that binds the entire match target.
- A pattern binds each name once: `when [x, x]` is a compile error (`'x' is bound twice in this pattern`), not an equality test; pin an earlier binding (`^x`) or compare in the body. `_` may repeat. Destructuring `let`s follow the same rule, and `for k, v` needs two different names.
- Patterns compose recursively: `{users: [{name: n}]}` works.
- Any other expression after `when` (e.g., `when some_var + 1`, `when 42`) is an expression arm: it matches when the target equals the expression's value. Expression arms and pattern arms can be mixed in one `match`.

Each pattern compiles to a condition check plus variable bindings; the bindings are in scope only inside that arm's body.

`elif` desugars to nested `if/else` at parse time — no new AST nodes. One `end` closes the entire chain.

`break` exits the innermost loop (`while` or `for`). `continue` skips to the next iteration.

**`break`, `continue`, and `return` inside blocks:** A `do`/`end` block is a closure (anonymous function). `return` inside a block returns from the block, not from the enclosing function — the iteration function receives the return value as the result of calling the block. `break` and `continue` inside a block only affect loops *within* the block itself; they cannot break or continue a loop in the caller.

`break` or `continue` with no loop around it inside its own function body is a compile error. That covers top-level code and named functions (`` `break` outside a loop``), and also a `do` block, an anonymous `fn` closure or a `spawn do` body even when the closure itself sits inside a loop, because the closure is a separate function (`` `break` inside a `do` block; use a `for` loop``). To stop iterating early, write a `for` loop instead of a block-taking call; `return` leaves the block.

## For Loops

Three forms, all lowered to `while` loops (in `compiler/lower.gem`):

```
# Array iteration
for item in items
  print(item)
end

# Table key-value iteration
for key, value in table
  print("{key} = {value}")
end

# String iteration (bytes)
for ch in "abc"
  print(ch)            # "a", "b", "c"
end
for i, ch in "abc"
  print("{i} {ch}")    # 0 a, 1 b, 2 c
end

# Range iteration (0 to n-1)
for i = 0, n
  print(i)
end
```

`break` and `continue` work inside `for` loops. The iterator increment happens before the user body, so `continue` correctly advances to the next element.

The range form evaluates its bound once, before the loop, and assigning the loop variable in the body doesn't change the next iteration. The table form evaluates the RHS expression exactly once. It walks the table's entries in `keys()` order without building a `keys()` array, reading the entry count once before the loop: an entry added during the loop is not visited, and a `delete` during the loop (which moves the last entry into the hole) makes it skip entries and then visit `nil`. The single-variable form is for arrays: it checks once, before the loop, that a table's keys are the ints `0 .. n-1` (in any order; `{}` is an empty array) and otherwise raises `for: one loop variable needs an array or a string, got a table with the key <key>; ...` (use `for k, v` or `values(t)`). It re-reads `len` every iteration, so `push` during the loop extends it and `remove_at` makes it skip elements. Over a string, both forms walk the bytes: `ch` is the one-byte string `s[i]` (a multi-byte UTF-8 character comes out as several bytes), and the two-variable form also gives the 0-based byte index `i`. Iterating anything other than a table or a string (an int, `nil`, a buffer, …) raises `for: expected a table or string to iterate, got <type>`, in either form.

## Closures

Functions capture their environment:

```
fn counter(start)
  let n = start
  fn()
    n = n + 1
    n
  end
end

let c = counter(0)
c()  # 1
c()  # 2
```

A closure body (`fn() ... end`, a `do` block, a `spawn do` body, `pcall <expr>`) resolves names exactly like a named function body: reading or assigning a name that no enclosing scope declares (no `let`, parameter, function, builtin or `load`) is a compile error, `undeclared identifier`, at the use. A closure sees only the variables declared before it is created, so a closure that reads a `let` written after it, or calls itself through the `let` it initializes, is the same error; declare the variable first (`let fact = nil`) and assign the closure to it (`fact = fn(n) ... fact(n - 1) ... end`).

## Tail Call Optimization

Self-recursive functions in tail position are optimized into loops. The compiler detects when a named function calls itself as the last expression (including through `if`/`elif`/`else`, `match`, and `receive` branches) and replaces the recursive call with parameter reassignment and a loop restart. This means tail-recursive functions use constant stack space regardless of recursion depth.

```
fn loop(n, acc)
  if n == 0
    return acc
  end
  loop(n - 1, acc + n)   # TCO: becomes a loop, no stack growth
end

loop(10000000, 0)         # works — would overflow without TCO
```

TCO applies to every named function (`fn name(...)`), including ones with default, rest, or destructured parameters. A self tail call behaves exactly like a real call: an omitted argument evaluates its default (seeing the new values of earlier parameters), extra arguments go into a fresh rest table, and a closure created in one iteration keeps that iteration's parameter bindings. Anonymous functions are not optimized. Non-tail calls (where the result is used in a further expression, e.g. `n * f(n-1)`) remain normal recursive calls.

A named function that waits for messages (a `receive` in its body, outside closures) and calls itself other than in tail position (the last expression of the body or of a branch there, or `return f(...)`) gets a compile-time warning on stderr (`` `serve` waits for messages (`receive`) and calls itself here before it is done, ... ``): each such call keeps its stack frame and memory until it returns, so as a process loop the stack grows with every message handled. Bounded recursion (receiving a known number of messages) is fine. Calls in closures (`spawn do serve(s) end`) don't count.

Mutual tail recursion between named functions (`fn A` tail-calls `fn B`, which tail-calls `fn A`, through any number of functions) is also run at constant stack depth, as long as no function in the cycle has rest or default parameters, a parameter that a nested closure assigns to, or more than 16 parameters. A cycle that does not qualify grows the stack on every call; write such a loop as direct self-recursion or use `while`.

## Long-Running Processes — Per-Iteration Arena Reset

Each process has its own arena (bump allocator), freed in bulk when the process exits. For short-lived processes (request handlers, one-shot tasks) that is all there is — no per-allocation free, no GC pauses.

Loops reclaim their own garbage as they run, so long-lived processes (accept loops, keep-alive HTTP handlers, supervisors, gen_servers) and long computations stay at bounded memory. Every loop gets the treatment:

1. **`while` loops**, including `for` loops and `while true`.
2. **Self tail calls** (TCO, see above), with any number of arguments — including none — and any kind of parameter.
3. **Mutual tail calls** that the compiler turns into a trampoline.

When a loop starts, the runtime remembers how much of the arena is in use. At the loop's back-edge, once the loop has allocated enough (at least 1 MB), the runtime copies the values that are still reachable from what was allocated since the loop's last reset (since the loop started, for the first one) — the loop's live variables (computed by the compiler's liveness analysis), the process's module state and mailbox, and anything stored into older tables or buffers — into fresh memory, and returns the rest to the OS. What a reset copied counts as older from then on, so the next resets leave it in place; a full reset, which starts again from where the loop started, frees what the loop dropped among it once that memory has doubled. Memory allocated before the loop started is never moved or freed by the loop, which is what makes this safe wherever the loop runs: called from any position, at any depth, inside `pcall`, in a spawned closure or in the main program. From the program's perspective nothing happens.

The cost of a reset is proportional to the data it copies, and the next reset waits until the loop has allocated at least twice that much again; a full reset copies everything the loop keeps and comes only once the kept data has doubled since the last one. So the total reset work stays proportional to the memory the loop allocates — a loop that builds a large table, or a server holding a large state, does not slow down as its live data grows.

```
# tail recursive: memory stays bounded however long it runs
fn accept_loop(server_fd)
  let client = tcp_accept(server_fd)
  spawn(fn() handle(client) end)
  accept_loop(server_fd)
end

# `while true`: same
fn accept_loop(server_fd)
  while true
    let client = tcp_accept(server_fd)
    spawn(fn() handle(client) end)
  end
end

spawn(fn() accept_loop(fd) end)
```

Garbage made outside any loop (straight-line code, or before a loop starts) is freed when an enclosing loop resets, when an enclosing call of a function that can recurse returns, or when the process exits.

### Return resets

The return of a function that can recurse is a reset point too. When a call starts, the runtime remembers how much of the arena is in use; when it returns, if the call has allocated at least 1 MB that no reset during it has freed, the runtime copies the return value (if any) — and, as for a loop, the process's module state and mailbox and anything stored into older tables, buffers or captured variables — into fresh memory and frees the rest of what the call allocated. Memory allocated before the call is never moved or freed by it. After a return reset, the next one also waits until the returning call's unfreed memory and what the process has allocated since add up to twice what that reset copied (four times, when it kept most of what it found), plus half of what it scanned, so a recursion that hands a growing result up its levels copies it a number of times logarithmic in its size, not once per level.

A function can recurse when the compiler finds it on a cycle of calls. A call through a variable, a parameter or a table field, and a call to `pcall`, `sort` or `build_string` (which call the function they are given), count as calls of every closure and of every named function used as a value, so recursion through closures and callbacks is found too. Other functions have no return reset. A closure written directly as the argument of `pcall` or `sort` usually has none either, and the body of a process skips it (its arena is freed when it exits). A call that ends by raising an error frees nothing at that point; its garbage is freed by the next enclosing loop or return reset.

### When the back-edge reset cannot fire

The reset needs an addressable home for every value live at the back-edge. When the liveness analysis cannot provide one — a live variable declared in a nested `if`/`match` arm that the compiler could not hoist, or a liveness computation that does not converge — the loop runs without resetting. For a `while true` loop the compiler then prints a warning (`cannot reset per-process arena at this loop's back-edge`), since its memory would grow without bound; a loop that terminates is silent (its garbage is reclaimed by an enclosing loop or at process exit).

## Green Threads and Message Passing

```
let pid = spawn do
  while true
    let msg = receive()
    print("got: " + msg)
  end
end

send(pid, "hello")
send(pid, "world")
```

`spawn(fn)` starts a process running `fn()` and returns its pid; to pass arguments, close over them (`spawn do handle(client) end`): a direct call with other than one argument is a compile error, as for `pcall`. `spawn`, `send`, `receive` are runtime functions, not keywords. Under the hood they use minicoro coroutines with a round-robin scheduler. Each spawned coroutine gets a mailbox (a simple queue). `receive` yields the coroutine if the mailbox is empty; the scheduler resumes it when a message arrives via `send`.

At most 262,144 processes, main included, are alive at once; past that, `spawn` raises `spawn: process table full`. The environment variable `GEM_MAX_PROCS`, read when the program starts, lowers the limit: a number of at least 2 (digits only; a number above 262,144 means 262,144). Any other value, the empty string included, stops the program with `gem: GEM_MAX_PROCS must be a number of processes (at least 2), got '<value>'` and exit status 1, and stops the `gem` compiler the same way, since it runs on the same runtime. A spawned process's slot is freed when it exits.

`read_file`, `write_file`, `append_file`, `exec`, `sqlite_open`, `sqlite_close` and every `extern blocking fn` call run on a pool of worker threads, 4 by default, while the calling process waits and the others run; a call made while every worker is busy waits for one. Name lookups (`tcp_connect`, `tls_connect` and `tcp_listen` given a host name) run on threads of their own, so a lookup that waits for a nameserver never holds a worker file I/O needs: a thread starts when a lookup has none idle to take it, up to 16 (more lookups wait for one), and stops after 10 s idle. A process asking for a name whose lookup is still running waits for that lookup's answer, so any number of processes asking for one name hold one thread. The program's end doesn't wait for a lookup still running. The environment variable `GEM_POOL_SIZE`, read when the program starts, sets the number of workers: a number of at least 1 (digits only; a number above 256 means 256). Any other value, the empty string included, stops the program with `gem: GEM_POOL_SIZE must be a number of worker threads (at least 1), got '<value>'` and exit status 1, and stops the `gem` compiler the same way. When the system starts fewer threads than asked (a limit on processes or threads, such as `ulimit -u` or systemd's `TasksMax`), the program prints `gem: started <n> of <size> I/O pool threads (GEM_POOL_SIZE)` on stderr and runs with those.

Each process also needs memory mappings of its own (its stack and heap). On Linux the system allows a program `vm.max_map_count` of them (65,530 by default), and `spawn` stops at 7/8 of that, so that the processes already running can still grow: past it, `spawn` raises `spawn: too many processes for the system's memory-mapping limit (N alive, vm.max_map_count = M)`, at about 14,000 live processes by default. Raise the limit (`sysctl vm.max_map_count=262144`) for more. Some distributions set it to 1,048,576 (Ubuntu 26.04, for one), where `spawn` stops only at about 229,000 processes, about 4.8 GB of them: on a machine with less memory the system kills the program first, and no `pcall` catches that. The environment variable `GEM_MAX_MAPS`, read when the program starts, lowers the limit the runtime uses: a number of at least 1 (digits only; a number above the system's limit means the system's limit, and on a system without one it has no effect). Any other value, the empty string included, stops the program with `gem: GEM_MAX_MAPS must be a number of memory mappings (at least 1), got '<value>'` and exit status 1. When the system refuses the memory for a new process's stack anyway (an address-space limit set with `ulimit -v`), `spawn` raises `spawn: cannot map a stack for a new process (N processes alive)`. All three errors are ordinary errors that `pcall` catches. An idle process takes about 21 KB of memory with 4 KB pages (Linux x86_64) and about 70 KB with 16 KB pages (macOS arm64).

The scheduler's work per pass is proportional to the processes that are ready to run, not to the number alive: a process waiting in `receive` or `sleep` costs memory but no time. Processes waiting on a socket are polled together: after every pass while there are at most 64 of them; above that, while other processes keep running, the next poll waits twice as long as the last one took, and at most a millisecond.

The main process (top-level code) is itself a schedulable coroutine (PID 0). All concurrency primitives — `self()`, `send`, `receive`, `sleep`, `monitor`, `link` — work at the top level. The program exits when all processes have terminated; if spawned processes outlive main, the program continues running until they complete.

**Deadlock.** When every process that is still alive is waiting in a `receive` without `after` (or `receive()`), and nothing else can wake any of them — no `sleep` or `receive ... after` deadline, no pending `send_after` timer, no process waiting on a socket or on thread-pool work (`read_file`, `exec`, an `extern blocking fn`, …) — no message can ever arrive. What happens then depends on main:

- If main is one of the waiting processes, the runtime reports a deadlock like an uncaught error in main: `deadlock: main process is waiting in receive and no other process can send to it` (or `... and the other N processes are also waiting in receive`) with main's stack trace, pointing at the `receive`, goes to stderr, and the program exits with status 1. `pcall` does not catch it.
- If main has already finished, the program ends normally (status 0); the processes still waiting in `receive` are abandoned.

A process blocked on I/O counts as able to send, so a server whose main waits in `receive` while another process loops on `tcp_accept` keeps running.

A pid is an int that names one process for good: after that process exits, its pid never refers to another process, even though the runtime reuses process slots. Messages sent to it are dropped, `kill` returns `nil`, `process_info` returns `nil`, `monitor` delivers `DOWN` with reason `"noproc"`, and `link` sends the caller an exit signal with reason `"noproc"`.

## Preemptive Scheduling (Reduction-Based)

Processes are cooperatively scheduled but the compiler inserts automatic yield points so tight loops cannot starve the scheduler. Each process has a reduction counter that increments at loop back-edges (the top of every `while` body, including `for` loops which desugar to `while`). When the counter exceeds the threshold (currently 4000), the process yields and is immediately re-queued as READY. The counter resets to 0 each time the scheduler resumes a process.

Yield checks are only inserted in user-written loops. Compiler-generated loops (e.g. the mailbox scan in selective receive) do not get yield checks, since yielding mid-scan would break the selective receive contract.

The main program runs as a process too (PID 0), so its loops yield the same way. Self tail calls compile to loops and yield too; non-tail recursion has no yield point, so a long CPU-bound recursive computation (a naive `fib(30)`) holds up every other process until it returns.

## Process Monitoring

```
let child = spawn() do
  error("crash")
end
monitor(child)
let msg = receive()
# msg == {tag: "DOWN", pid: <child_pid>, reason: "crash"}
```

`monitor(pid)` registers the caller as a monitor of the target. When the target exits (normally or via error), a `DOWN` message is delivered to each monitor's mailbox:

- `{tag: "DOWN", pid: <target_pid>, reason: "normal"}` — clean exit
- `{tag: "DOWN", pid: <target_pid>, reason: "<error message>"}` — crash

Monitoring a pid whose process no longer exists delivers the DOWN message immediately, with reason `"noproc"` (or the process's exit reason while the runtime still has it), and does so on every such call: a second `monitor` of a dead target sends another `DOWN`. Duplicate monitors of a live target are deduplicated: a process monitors a target at most once, and a second `monitor` of the same target does nothing, so the target's death delivers one `DOWN`. A monitor ends when the monitoring process exits (`monitor` drops the entries of exited monitoring processes from the target's list as it walks it, and `process_info` leaves them out), so a long-lived process monitored by many short-lived ones keeps a list as long as its live monitors. Returns `true` when it adds a monitor or delivers a `DOWN` for a dead target, and `false` when the caller already monitors the live target.

`demonitor(pid)` removes the caller's monitor of `pid`: the target's death no longer sends the caller a `DOWN`. Returns `true` if there was such a monitor, `false` otherwise (no monitor, or the target has already exited). A `DOWN` already in the caller's mailbox stays there. Since a process monitors a target at most once, `demonitor` also removes a monitor the caller set up elsewhere; code that monitors only for a while (like `gen_server.call`) removes the monitor only when its own `monitor` returned `true`.

`spawn_monitor(fn)` atomically spawns and monitors a process. Returns `{pid: <pid>}`.

Error isolation: a crashing spawned process does not kill other processes. The error is captured, DOWN messages are delivered, and the scheduler continues. `pcall` inside a monitored process still works — it catches errors locally before the process-level handler.

## Links

Links are bidirectional process monitors. When two processes are linked, an abnormal exit in one propagates to the other.

```
let pid = spawn_link() do
  error("crash")
end

# caller dies too, unless trap_exit is set
```

`link(pid)` creates a bidirectional link between the calling process and the target. `unlink(pid)` removes it. `spawn_link(fn)` atomically spawns and links (no race window between spawn and link).

When a linked process dies with a reason other than `"normal"`, the exit signal propagates to all linked processes:

- If the linked process has `trap_exit` set to `true` (via `process_flag("trap_exit", true)`), the exit signal is converted to a message: `{tag: "EXIT", pid: <pid>, reason: <reason>}`.
- If the linked process does not trap exits, it dies with the same reason, propagating the signal further.
- A process killed with the untrappable `kill(pid, "kill")` dies with reason `"killed"`, and that is an ordinary reason for its links: a linked process that traps exits gets `{tag: "EXIT", pid: <pid>, reason: "killed"}`, and one that doesn't dies with `"killed"`. Only the direct `kill(pid, "kill")` is untrappable.
- Normal exits (reason `"normal"`) do not propagate to non-trapping linked processes. A process that traps exits does get `{tag: "EXIT", pid: <pid>, reason: "normal"}` for them, so its receive loop needs an arm (or a catch-all) for those too.

Propagation is transitive: if A is linked to B and B is linked to C, and C crashes, B dies (unless trapping), which causes A to die (unless trapping). The implementation is cycle-safe.

```
# Trapping exits turns death signals into messages
process_flag("trap_exit", true)
let pid = spawn_link() do
  error("boom")
end

receive
when {tag: "EXIT", pid: p, reason: r}
  print("child " + to_string(p) + " died: " + r)
end
```

`process_flag("trap_exit", bool)` sets the trap_exit flag on the current process and returns the previous value.

## Named Processes

```
let pid = spawn() do
  let msg = receive()
  print(msg)
end
register("worker", pid)

send("worker", "hello")    # send by name
let p = whereis("worker")  # returns pid or nil
```

`register(name, pid)` associates a string name with a pid. Errors if the name is already taken. `whereis(name)` returns the pid for a name, or `nil` if not registered. `send` accepts either a pid (int) or a registered name (string). When a process dies, its name is automatically unregistered. Sending to a dead pid silently drops the message, but sending to a name that is not registered (including the name of a process that has died) raises `send: no process registered with that name`. Register from the parent, as above: a child that calls `register(name, self())` itself may not have run yet when the parent sends.


## Selective Receive

```
receive
when {tag: "DOWN", pid: p, reason: r}
  print("process " + to_string(p) + " died: " + r)
when {tag: "request", body: b}
  handle(b)
after 5000
  print("timed out")
end
```

`receive ... when ... end` is a syntactic form (not a function call) for Erlang-style selective receive. The process scans its mailbox from oldest to newest, testing each message against the `when` arms in order using the same destructuring patterns as `match`. When a pattern matches, that message is removed from the mailbox (even if it's not the head), pattern variables are bound, and the arm body executes. Non-matching messages remain in the mailbox in their original order.

If no arm matches any message, the process yields. When new messages arrive, it re-scans from the oldest unmatched message.

Pin patterns (`^name`, see Destructuring Patterns in Match) select a message by a value only known at runtime. This is how a request/reply exchange picks out the reply to *its* request when several replies may be queued:

```
let ref = make_ref()
send(server, {tag: "get", from: self(), ref: ref})
receive
when {tag: "reply", ref: ^ref, value: v}
  v
end
```

Without the `^`, `ref: ref` would bind whatever ref the first reply carries.

The `after <ms>` clause is optional. If present and the timeout elapses with no matching message, the `after` body executes. `after 0` means "check once, don't block" (so does a negative timeout). The timeout must be an int; anything else (`nil` included) raises `receive: after expects an int (milliseconds), got <type>`. A timeout too large for the clock (up to `9223372036854775807`) waits forever, as do `sleep` and `send_after` with such a delay. Omitting `after` means block forever (like `receive()`).

A `receive` may have an `after` clause and no `when` arms: it takes no message and leaves the mailbox as it is, waits `<ms>` milliseconds (messages that arrive meanwhile stay queued for a later `receive`), then evaluates to the `after` body. `after 0` returns at once. A `receive` with neither `when` arms nor `after` (`receive` directly followed by `end`) is a compile error.

```
receive
after 100 then nil                   # wait 100 ms, mailbox untouched
end
```

The `receive` block can produce a value when used as the last statement of a function (implicit return), just like `match`.

The `receive()` function call always pops the head of the mailbox unconditionally, whatever the message is.

## Process Control

`kill(pid, reason)` sends an exit signal to a process. The reason `"kill"` can't be trapped (as in Erlang): the target is terminated, whether it traps exits or not, with the reason `"killed"`, which is what its monitors' `DOWN` and its links see; links pass `"killed"` on as an ordinary reason (see Process Linking), and `kill(pid, "killed")` is an ordinary, trappable signal. For any other reason, if the target has `trap_exit` enabled, an `{tag: "EXIT", pid: sender_pid, reason: reason}` message is delivered to its mailbox instead of terminating it. Otherwise, a signal with reason `"normal"` from another process is ignored (as in Erlang: `"normal"` means "nothing went wrong", so it never ends a process that doesn't trap exits); with any other reason, the process is terminated immediately: marked dead, DOWN messages delivered to monitors, registered name removed, and exit propagated to linked processes. Returns `true` if the process was alive, `nil` otherwise. A process can kill itself: `kill(self(), reason)` ends it at once with that reason, `"normal"` included (unless it traps exits; `kill(self(), "kill")` ends it with `"killed"` even then), and `pcall` does not catch it. Likewise, when a `kill` brings down the caller through a link, the caller dies at once; `pcall` does not catch that either. If the process ending this way is the main process and the reason is not `"normal"`, the reason is printed as a runtime error and the program exits with status 1; when another process kills main, or main dies through a link, the message names the sender: `main process killed by process <pid>: <reason>` or `main process killed by linked process <pid>: <reason>`, followed by main's stack trace (where main was when the signal arrived). A process killed while it waits on the thread pool (`read_file`, `write_file`, `append_file`, `exec`, `sqlite_open`, `sqlite_close`, an `extern blocking fn`) dies at once, but the operation itself still runs to completion on its worker thread: a write still lands, a command started by `exec` keeps running, and once the worker is done the runtime frees the result (closing the connection an abandoned `sqlite_open` opened), except a `Ptr` returned by an `extern blocking fn`.

`sleep(ms)` suspends the current process for `ms` milliseconds. The scheduler resumes the process after the deadline expires; messages that arrive meanwhile wait in the mailbox and do not end the sleep early. `sleep(0)` yields to other ready processes and returns.

`time_ms()` returns the current monotonic time in milliseconds (int). Useful for timeouts, benchmarks, and restart intensity tracking. Not suitable for wall-clock formatting — use `epoch_ms()` for that.

`epoch_ms()` returns the current wall-clock time as milliseconds since the Unix epoch (int). Use this for timestamps that need to be formatted into dates or times. Uses `gettimeofday` internally.

`format_time(epoch_ms, format_str)` formats a wall-clock epoch millisecond timestamp into a UTC string using `strftime` specifiers. Returns a string. The milliseconds are dropped by rounding down, so `-1` is `1969-12-31T23:59:59Z`. `epoch_ms` must be an int and `format_str` a string (otherwise it raises `format_time: expected (int, string), ...`); a time the platform can't represent raises `format_time: time ... ms is out of range`. An empty format gives `""`, and output over 256 KB (262,144 bytes) raises `format_time: output is over 256 KB`. The format string stops at its first NUL byte. Supported specifiers include `%Y` (the year, zero-padded to at least 4 digits: `0001`, on every platform), `%m` (month 01-12), `%d` (day 01-31), `%H` (hour 00-23), `%M` (minute 00-59), `%S` (second 00-59), `%a` (abbreviated weekday), `%b` (abbreviated month), `%T` (`%H:%M:%S`), `%F` (`%Y-%m-%d`), and all other platform-supported `strftime` specifiers. `%z`, `%Z` and `%s` are UTC's whatever the local zone: `+0000`, `UTC` and the epoch seconds (`strftime` itself takes them from the local zone on some platforms).

`format_time_local(epoch_ms, format_str)` — same as `format_time` but formats in the local timezone instead of UTC (`%z`, `%Z` and `%s` come from the platform's `strftime`).

## Timers

```
let ref = send_after(pid, "tick", 1000)
cancel_timer(ref)
```

`send_after(pid, msg, delay_ms)` schedules `msg` to be delivered to `pid` after `delay_ms` milliseconds. Returns a unique ref identifying the timer. The timer fires from the scheduler loop, not from the calling process, so it works even if the caller is blocked or has exited. When the target process exits, its pending timers are dropped; a timer for a pid that has already exited is never scheduled. A pending timer keeps the program running until it fires or is cancelled.

`cancel_timer(ref)` cancels a pending timer by ref. Returns `true` if cancelled, `false` if it already fired, was dropped because its target exited, or was already cancelled. A non-ref argument raises.

Pending timers are kept in a min-heap ordered by deadline that grows as needed; timers with the same deadline fire in the order they were created.

## Process Introspection

```
let info = process_info(pid)
# info == {state: "ready", mailbox_len: 0, links: {}, monitors: {},
#          trap_exit: false, exit_reason: nil, resources: 0, memory: 69632}
# (memory: 81920 on macOS arm64, whose pages are 16 KB)
```

`process_info(pid)` returns a table with metadata about the process:

- `state` — `"ready"`, `"waiting"`, or `"dead"`
- `mailbox_len` — number of messages in mailbox
- `links` — array of linked pids
- `monitors` — array of the pids of the live processes monitoring it
- `trap_exit` — bool
- `exit_reason` — string or nil
- `resources` — how many open resources (sockets, database handles) it owns (see Owned Resources)
- `memory` — bytes of memory its arena holds (the blocks the runtime has mapped for it: at first 64 KB and a header, rounded up to whole pages)

Returns `nil` if the pid is invalid or the slot is free.

```
let s = runtime_stats()
# s.procs, s.memory, s.resets, s.reset_max_ms, ...
```

`runtime_stats()` returns a table of figures for the whole program, for a process that samples them (a metrics loop) or a test:

- `resets` — arena resets so far, in every process (see `GEM_DIAG` below); `full_resets` — those among them that copied everything their loop or function keeps, return resets included; `return_resets` — those at the return of a recursive function
- `reset_copied`, `reset_freed` — bytes the resets copied, and freed (the size of the regions they released)
- `reset_ms` — milliseconds spent in resets (a float); `reset_max_ms` — the longest one so far; `resets_over_1ms`, `resets_over_10ms`, `resets_over_100ms` — the resets that took at least that long. Every other process waits while a reset runs.
- `procs` — live processes; `proc_hwm` — the process table's high-water mark (it grows to 1,024 slots before the runtime reuses freed ones, then only as far as the most processes alive at once); `max_procs` — the process limit (`GEM_MAX_PROCS`); `spawn_refused` — spawns that raised for lack of room: a full process table, the system's limit on memory mappings, or no memory for a new process's stack
- `memory` — bytes the live processes' arenas hold (the sum of their `process_info(pid).memory`)
- `resources_open` — open resources (sockets, database handles); `resources_ownerless` — those with no owner (see Owned Resources)

The reset figures and `spawn_refused` only grow: a sampler takes the difference between two calls (`resets_over_10ms` going up between two samples means a pause of 10 ms or more in between). The others are counts at the time of the call.

`GEM_RESET_STRESS=<bytes>` in the environment lowers the allocation a loop makes between two resets from 1 MB to that many bytes (and so the allocation that makes a recursive function's return reset), for testing: with `GEM_RESET_STRESS=1` a loop resets at almost every back-edge that allocated something (the spacing that keeps reset work proportional to allocation still applies). A program must print the same with and without it; `make test-stress` checks the examples, the example apps and the compiler. A value that isn't a positive integer is an error at startup (exit 2).

`GEM_DIAG=1` in the environment prints a summary on stderr when the program exits, as `gem_diag:` lines: the reset totals (once a reset has run; the line has no duration counts) and the process and resource counts; `GEM_DIAG=2` also prints a `gem_reset:` line for every reset that takes 1 ms or more, with the process and the function it ran in.

## C Interop

```
extern include "stdio.h"
extern include "math.h"
extern fn puts(s: String) -> Int
extern fn sqrt(x: Float) -> Float
extern fn fopen(path: String, mode: String) -> Ptr

puts("hello from C")
```

Include the C header of a library function (`extern include`, below). Without one, the compiler writes its own prototype from the extern types (`int64_t puts(const char*)`), which conflicts with the real declaration of any libc function the runtime header already pulls in, and the C compiler rejects it.

`extern fn` declares a C function. The compiler emits the call directly since we compile to C. Type annotations on extern declarations only — the rest of the language stays dynamically typed. `Ptr` is an opaque type for C pointers.

`extern fn`, `extern blocking fn` and `extern include` go directly in a file's top-level code; inside any block or function body they are a compile error. The extern's name is the C function's, so a C keyword or a type name the generated prototypes use (`int`, `char`, `static`, `int64_t`, ...) is a compile error (`` `int` is reserved in C and can't name an extern fn ``); a prototype that conflicts with a system header's declaration is reported by cc at the `extern fn` line. An `extern fn` is a binding like a top-level `fn`: one named like a builtin (`extern fn sqrt(x: Float) -> Float`) shadows the builtin in its own file, and one in a loaded module can be exported and called as `module.name`. The C function it calls is always the declared name.

The generated wrapper validates `argc` and each argument's runtime type tag before reading the `GemVal` union, so a Gem-side mistake (wrong arity, wrong type) raises a Gem-level error at the boundary instead of passing garbage to C. Errors mention the declared Gem-level type name (e.g. `foo: arg 0 expected String, got int`). Types are not converted: a `Float` parameter rejects an int (`sqrt(2)` raises; pass `2.0` or `to_float(n)`). The argument count must match the declaration exactly: a direct call with another count is a compile error (as for a named fn), and a call through a value with too few or too many raises `<name>: expected N argument(s), got M` (unlike a Gem fn, which ignores extra arguments). Arity counts Gem arguments, so a `Bytes` parameter counts once. A `Ptr` is an int on the Gem side, and `NULL` comes back as `0`, not `nil`.

The compiler auto-generates C forward declarations from `extern fn` type signatures, so no separate `.h` file is needed for function declarations. The type mapping:

| Gem extern type | C parameter        | C return        |
|-----------------|--------------------|-----------------|
| `Int`           | `int64_t`          | `int64_t`       |
| `Float`         | `double`           | `double`        |
| `String`        | `const char*`      | `char*`         |
| `Bool`          | `int`              | `int`           |
| `Ptr`           | `void*`            | `void*`         |
| `Nil`           | —                  | `void`          |
| `Table`         | `GemVal`           | `GemVal`        |
| `Bytes`         | `const uint8_t*, int64_t` (two C parameters) | `GemBytes` (struct, see below) |
| `Socket`        | `int` (the socket's fd) | — (compile error `an extern fn cannot return a Socket`) |

Auto-generation is skipped when `extern include` is present (the user manages declarations via headers).

`extern blocking fn` does not accept `Table` parameters or returns (compile error `extern blocking fn cannot take a Table`): the call runs on another thread, away from the process's arena.

**`Socket` parameters.** A `Socket` parameter takes a socket (see Owned Resources) and passes its fd as a C `int`; anything else raises `<fn>: arg <i> expected Socket, got <type>`, a closed socket `<fn>: arg <i>: socket is closed`, and a TLS socket (see TLS), whose fd carries the encrypted stream, `<fn>: arg <i>: a TLS socket can't be passed to C`. The C function may use the fd for the call only, and must not close it. For an `extern blocking fn`, the socket stays open for the C call until it returns: a `tcp_close` meanwhile, or its owner's exit, closes it for Gem code at once (every builtin raises `socket is closed`), but its fd only once the call has returned. A C function that gets the fd some other way (`tcp_fd` passed as an `Int`) has no such guarantee, except for the sockets of the calling process itself, which a kill mid-call leaves open until the call returns. To hand the runtime a socket C code made, return its fd as an `Int` and register it with `tcp_from_fd`.

### `Bytes` — binary-safe FFI

`Bytes` is for binary data that may contain embedded NULs (compressed payloads, crypto blobs, length-framed protocols). Unlike `String`, which marshals as a NUL-terminated `const char *` and is implicitly truncated at the first `\0` on the C side, a `Bytes` parameter expands to **two** C parameters — pointer + length — so the C function knows the exact byte count.

```
extern fn write(fd: Int, buf: Bytes) -> Int          # POSIX write(int, const void*, size_t)
extern fn sha256(data: Bytes, out: Bytes) -> Nil     # in-place hash
```

The first declaration generates the C call `write(fd, buf, buf_len)` — three C arguments from two Gem arguments. This matches POSIX `write` exactly.

**Constructing a `Bytes` argument from Gem.** There is no separate `Bytes` type at the Gem level — a `Bytes` parameter accepts any Gem `String`. Strings are already binary-safe (`len(s)` is the byte length, embedded NULs are preserved), so any operation that produces a string produces a valid `Bytes` payload:

```
let s1 = "header\0" + chr(1) + chr(2) + "data"   # embedded NUL and control bytes
let s2 = read_file("/path/to/blob")      # raw file bytes
let s3 = tcp_read(sock, 1024)            # raw socket bytes
let s4 = build_string() do |add|         # assembled binary
  add(magic_header)
  add(chr(0))
  add(payload)
end
write(fd, s4)                            # all four work as Bytes args
```

The `Bytes` annotation only changes how the value crosses the C boundary — it does not introduce a runtime type or require a conversion call.

**Returning `Bytes` from C.** A `-> Bytes` return type expects the C function to return a `GemBytes` struct (defined in `gem.h`):

```c
typedef struct { const uint8_t *data; int64_t len; } GemBytes;

GemBytes my_compress(const uint8_t *src, int64_t src_len) {
    int64_t out_len;
    uint8_t *out = compress_alloc(src, src_len, &out_len);
    return gem_bytes(out, out_len);   // helper from gem.h
}
```

The runtime copies `data[0..len)` into the calling process's arena via `gem_string_with_len`, producing a binary-safe Gem string. Ownership of the returned buffer follows the same rule as `String` returns:

- `extern fn` (non-blocking): runtime does **not** free `data`. Use static storage, or accept the leak. For malloc'd returns, prefer `extern blocking fn`.
- `extern blocking fn`: runtime **frees** `data` with `free()` after copying. The C function must return a malloc'd pointer (or `{NULL, 0}` for empty).

Returning `{NULL, 0}` from a non-blocking `extern fn -> Bytes` yields `nil` on the Gem side; from a blocking variant it yields an empty string.

**Why use `Bytes` instead of `String`?** `String` is the right choice when the C function only needs NUL-terminated text (`getenv`, `strerror`, `puts`). `Bytes` is the right choice when the function needs the exact byte count or may receive data containing `0x00`. Mixing them up is a common FFI bug — Gem's `String` is binary-safe internally but the C ABI for `const char *` is not.



`extern blocking fn` declares a C function that may block (e.g. socket I/O, database calls, DNS lookups). Every call, including one from the main program (which runs as a process too), is submitted to a thread pool so the scheduler can keep running other processes while the calling process waits. The C function runs on a worker thread and must not allocate from process arenas — for `String` returns, use `malloc`/`strdup` (the runtime copies into the process's arena and frees the original). If the calling process is killed while the call is in flight, the C function still runs to completion; the runtime then frees its copies of the arguments and a `String` or `Bytes` result, but not a `Ptr` result, which it has no way to release.

```
extern blocking fn net_accept(server: Socket) -> Int      # the new fd: register it with tcp_from_fd
extern blocking fn net_read(sock: Socket, max_bytes: Int) -> String
```

For C headers (structs, typedefs, system headers the compiler can't infer):

```
extern include "math.h"
extern include "stdio.h"
```

A relative path is resolved against the directory of the `.gem` file that contains the `extern include` (in a loaded module, the module's own directory): if that file exists, it is included by its absolute path, once however many files include it. Otherwise the line is emitted as `#include "<path>"` and left to the C compiler's search, which has the runtime directory on its include path; so system headers (`"string.h"`, `"sys/socket.h"`) work by name, and a missing one is reported by the C compiler at the `extern include` line. A path that can only name a file of your own, an absolute one or one starting with `./` or `../`, is a compile error when the file doesn't exist. The C file itself is compiled from a temporary directory, so the generated C (`--emit-c` too) holds the absolute path of your header. Put your own C functions in the header as `static` functions. The header is included before `gem.h`, so one that uses `GemVal`, `GemBytes` or `gem_bytes` must `#include "gem.h"` itself.

**Compiler and linker flags.** The program is linked against libc, libm and pthreads, plus OpenSSL when its code names `tls_connect` (see TLS) and the Gem was built with it. Anything else comes from the command line, before the source file: `-l<lib>` links a library, `-L<dir>` adds a library directory and `-I<dir>` a header directory (each also as two words, `-l ssl`). The `-I` directories come before the runtime's on the include path; `-L` and `-l` come after the runtime library, in the order given. After the source file, `-l`, `-L` and `-I` go to the program, like any argument gem doesn't know there. The environment adds the rest: `CC` names the C compiler (default `cc`), and `CFLAGS` and `LDFLAGS` are added to its command as the shell splits them (`CFLAGS` after the runtime's `-std=gnu11 -O2 -pthread`, so a later `-O0` wins; `LDFLAGS` before the `-l` flags).

```
gem -I /opt/foo/include -L /opt/foo/lib -lfoo app.gem -o app
CFLAGS="-I/opt/foo/include" LDFLAGS="-L/opt/foo/lib -lfoo" gem app.gem
```

**String-return ownership** differs by call kind:

- `extern fn` (non-blocking) — the runtime copies the returned `char*` into the calling process's arena via `gem_string` and **does not free the original**. Use this for static literals (`getenv`, `strerror`, etc.). A `malloc`'d return will leak.
- `extern blocking fn` — the runtime copies into the arena and **frees the original** with `free`. The C function must return a `malloc`/`strdup`'d pointer; returning a static literal will crash on the free. NULL is allowed and yields an empty string from an `extern blocking fn`, or `nil` from an `extern fn`.

**Pointer lifetime.** `String`, `Bytes`, and `Table` arguments passed to an `extern fn` point into the calling process's arena. They are stable for the duration of the call but **not** across the next arena reset (which can happen at the back-edge of any loop or self tail call, or when a Gem function that can recurse returns). An `extern blocking fn` receives malloc'd copies of its `String` and `Bytes` arguments instead, which the runtime frees once the call is over. In both cases C code must not stash these pointers — copy out with `strdup`, `memcpy`, or by value before retaining.

The wrapper checks the Gem-level type of each argument and that enough arguments were passed (see above), but not the declaration itself: a signature that doesn't match the real C function passes wrong values or crashes, and nothing checks what the C code does with its pointers.

## Operators

`+` for both arithmetic and string concatenation. If types don't match (e.g. string + int), runtime error. No `..` operator — keep it simple: `a .. b` and `0..n` are compile errors (a range loop is `for i = start, stop`).

`+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `and`, `or`, `not`, `in`

`%` takes integers only (a float operand raises). Integer division and `%` by zero raise `division by zero`, and so does float division by zero (no `inf`). Integer arithmetic wraps on overflow (two's complement), division included: `INT64_MIN / -1` is `INT64_MIN`, and `INT64_MIN % -1` is `0`. `<`, `<=`, `>`, `>=` compare an int with a float numerically, but `==` never equates them (`2 == 2.0` is `false`).

`x in tbl` — membership test. On a non-empty array (a table whose keys are exactly the ints `0 .. n-1`, in whatever order they were added) it tests the values: `true` if some element `== x` (a linear scan; tables compare by identity), so `20 in [10, 20]` is `true` and `0 in [10, 20]` is `false`. On any other table (string keys, sparse or mixed int keys, an empty table) it is `has_key(tbl, x)`: a record's keys, an int-keyed set's members (`seen[5] = true`, then `5 in seen`). An int-keyed set whose keys happen to be exactly `0 .. n-1` is an array, so `in` tests its values there; test an int-keyed set with `has_key`. A right operand that isn't a table raises `in: right operand must be a table, got <type>`. Precedence is at the comparison level (same as `==`, `<`, etc.).

**Equality semantics:** `==` compares by value for primitives (int, float, string, bool, nil) and by identity (reference) for tables, functions, and refs. Two distinct tables with identical contents are not equal: `{a: 1} == {a: 1}` is `false`. The same table reference compared to itself is `true`.

## Assignment Operators

`=`, `+=`, `-=`, `*=`, `/=`

`=` assigns to a variable, a field (`t.x = v`) or an index (`a[i] = v`). The compound forms take only a
variable: `t.n += 1` and `a[0] += 1` are compile errors (`compound assignment requires variable target`);
write `t.n = t.n + 1`.

An assignment is a statement, not an expression: it has no value (as the last statement of a function or block it yields `nil`, like a `let` or a loop), so one where a value is expected (`f(a = 1)`, `let b = a = 1`, `(a = 1)`, `return a = 1`, an `if`, `while` or `for` header) is a compile error at its `=`. A `do` / `{ |x| ... }` block body or a `pcall` body may be an assignment, since it is a statement.

## Strings

Two quote styles: double-quoted strings support interpolation, single-quoted strings do not.

```
let s = "hello"
let t = 'hello'
s.len()
s[0]
s + " world"
```

Double-quoted strings support the escape sequences `\n`, `\r`, `\t`, `\0`, `\\`, `\"`, `\{` and `\}` (the last two escape interpolation braces). `\0` produces a null byte (0x00). Any other backslash sequence is kept as written (`"\x41"` is the four characters `\x41`); there are no `\x` or `\u` escapes, so build other bytes with `chr(n)`.

A raw line break inside either kind of quoted string is part of the string (`"a` / `b"` is `"a\nb"`), and line numbers in errors count it.

Note: `\0` in single-quoted strings produces the literal characters `\0` (two chars), not a null byte — single-quoted strings only process `\n`, `\r`, `\t`, `\\`, and `\'`.

Single-quoted strings pass content through literally — `{` has no special meaning:

```
let json = '{"key": "value"}'     # no escaping needed
print('hello {name}')              # prints: hello {name}
```

Each quote style can contain the other without escaping:

```
print("it's fine")                 # it's fine
print('say "hello"')               # say "hello"
```

## Multi-line Strings

Triple-quoted strings span multiple lines. `"""` supports interpolation and all escape sequences like regular `"` strings. `'''` has no interpolation and minimal escapes like regular `'` strings.

```
let html = """
  <html>
    <body>{title}</body>
  </html>
  """

let json = '''
  {"key": "value",
   "count": 42}
  '''
```

Rules:
- The opening `"""` or `'''` must be immediately followed by a newline (optional trailing whitespace before the newline is allowed). Content starts on the next line.
- The closing `"""` or `'''` must appear on its own line with only leading whitespace (spaces, tabs, or any mix) before it.
- **Dedent**: the whitespace before the closing delimiter is the base indentation. It is stripped byte for byte from the start of every content line, so a tab matches only a tab and a space only a space. Extra indentation beyond the base is preserved. A content line that does not start with the whole base (a blank line, a shorter indent, or tabs where the closing line has spaces) loses only its longest leading part that matches the start of the base, and keeps the rest: with a base of two tabs, a line starting with one tab loses that tab, and a line starting with spaces keeps them. A `"""` string inside an interpolation has its own closing line and its own dedent; it does not change the outer string's.
- The final newline before the closing delimiter is stripped, so the resulting string does not end with a trailing `\n`.
- `"""` supports `{expr}` interpolation and escape sequences identical to regular `"` strings.
- `'''` has no interpolation; `{` is a literal character. Escape sequences are identical to regular `'` strings.

Example with dedent:

```
fn render()
  let s = """
    hello
      world
    """
  # s == "hello\n  world"  — 4 spaces stripped from each line (indent of closing """)
end
```

## String Interpolation

Expressions inside `{...}` in double-quoted string literals are evaluated and auto-coerced to strings via `to_string`:

```
let name = "world"
print("hello {name}")          # hello world
print("{1 + 2} items")         # 3 items
print("flag: {true}")          # flag: true
```

Literal braces in double-quoted strings are escaped with `\{` and `\}`:

```
print("literal \{brace\}")     # literal {brace}
```

Interpolation supports arbitrary expressions including function calls, table access, and nested strings:

```
print("len: {len(arr)}")
print("val: {obj.field}")
print("wrapped: {wrap("inner")}")
print("{"a" in t}")             # an expression may start with a string literal
print("{"{x}!"}")               # strings inside an interpolation may interpolate too
```

The interpolation ends at the `}` that balances its `{` (braces of table literals inside it are counted; braces inside nested strings are not). An empty interpolation `{}` and an unclosed one are compile errors.

## Nil and Truthiness

`nil` and `false` are falsy, everything else truthy.

## Source files

A CRLF line ending reads as LF everywhere in a source file, string literals included, so a file saved with Windows line endings compiles like the same file with LF endings. A lone CR is an ordinary character (an `unexpected character` outside a string).

## Comments

```
# single line comment
## doc comment
```

A comment runs from `#` to the end of the line. A comment starting with `##` is a *doc comment*: the compiler treats it like any other comment, but by convention it documents the public API for callers (the module header, and the block directly above each exported function), and tooling reads only `##` lines. Plain `#` comments are for whoever maintains the code. See BEST_PRACTICES.md, "Document the public API with `##`".

## Error Handling

`error(msg)` prints the message with file and line info to stderr, followed by a call stack trace showing each Gem function frame, and halts (`exit(1)`). Runtime type errors (e.g. `1 + "a"`) also print a stack trace with the actual types involved (e.g. `type error in +: got string and int`). The compiler reports every error it finds in a file (a parse error can hide later ones) and produces no binary.

**Inside spawned processes**, `error()` does not terminate the program. Each spawned process has an implicit error boundary — if an unhandled error occurs, the process dies but other processes continue. The error is captured, DOWN messages are delivered to monitors, EXIT signals propagate to linked processes, and the scheduler continues. `pcall` inside a spawned process still works — it catches errors locally before the process-level boundary. This boundary covers running out of stack too (see Stack depth below). See Process Monitoring for details.

A process that dies this way is reported on stderr, like Erlang's error logger: the same message, source line and stack trace an uncaught error in the main process prints, under a header naming the process by pid (and registered name, if any). Its monitors and links see the error message as the exit reason, as before. Nothing is printed when a process returns normally, is ended by `kill` (including `kill(self(), reason)`), or dies because a linked process died; only the process whose error went uncaught is reported. (The main process is the exception: main dying from an exit signal ends the program and is reported, see `kill`.) Processes under a supervisor are reported too.

```
[Runtime Error in process 4 "logger"]: boom
  --> app.gem:10
    |
 10 |   error("boom")
    |
Stack trace:
  at handle (app.gem:10)
  at anonymous fn (app.gem:21)
```

A stack frame names its function as you write it: `handle` for `fn handle`, `anonymous fn` for a `fn` literal, and `counter.bump` for the top-level `fn bump` of a loaded module `counter.gem` (whatever alias it is loaded under). File paths in traces, compile errors and notes are relative to the project root (the directory holding `gem.toml`); without a `gem.toml`, a file in or below the entry file's directory is shown under that directory as you typed it on the command line (`gem app.gem` shows `lib/util.gem`, `gem /src/app.gem` shows `/src/lib/util.gem`). Other files keep their full path.

The `-->` source line of a runtime error is read from the source file when the error is printed. Since trace paths are relative, a program run from another directory looks for the file, in order: under `$GEM_SOURCE_ROOT` if that environment variable is set, from the current directory, under the directory of the executable and each of its ancestors (so a binary built into `<project>/build/` or `<project>/bin/` finds `<project>/src/app.gem`), then under each ancestor of the current directory. The first file found that has the line is shown. A file too short to have the line is skipped, but nothing else is checked, so an unrelated longer file at the same relative path is shown. If no source is found (the program was deployed without it), the trace is printed without the source line. A full path is opened as is.

**Compile-time error format**: the compiler produces Rust-style diagnostics to stderr with source context, caret highlighting, and optional hints:

```
[Compile Error]: expected 'end' but got 'EOF'
  --> src/example.gem:5:1
   |
 5 | fn broken(
   | ^^
   |
```

Tokens carry line and column information from the lexer, so the caret points to the exact position of the offending token. Multi-character tokens get a proportionally wide underline.

**Recoverable errors** use `pcall` (protected call), which catches any error instead of halting. Two forms:

**Expression form** — `pcall <expr>` wraps a single expression. The parser desugars it to `pcall(fn() <expr> end)`; an error raised by the expression itself shows as an `anonymous fn` frame at the expression's line (the `pcall` line). An expression that can't raise because it only names a function is a compile error: `pcall fn() ... end` (`` `pcall fn() ... end` returns the fn without running it ``; use `pcall do ... end`) and `pcall f` (`` `pcall f` returns `f` without calling it ``):

```
let result = pcall error("boom")
print(result.ok)      # false
print(result.error)   # boom

let result2 = pcall parse(input)
print(result2.ok)     # true
print(result2.value)  # <parsed value>
```

This composes naturally with `match`:

```
match pcall parse(dangerous_input)
when {ok: true, value: ast}
  compile(ast)
when {ok: false, error: msg}
  eprint("parse failed: " + msg)
end
```

**Function form** — `pcall(f)` calls `f()` with zero arguments. Useful when the body contains multiple statements. Its argument is evaluated first, unprotected, so `pcall(f(x))` (a call as the argument) runs `f(x)` outside the protection and calls the fn it returns; it compiles with a warning on stderr (`` `pcall(f(x))` runs f(x) before `pcall` is called, so an error raised in f(x) is not caught; ... ``). It is right when `f(x)` returns the fn to call (`pcall(make_handler(3))`); to catch the call's own errors, write `pcall f(x)`:

```
let result = pcall(fn()
  let data = read_file(path)
  parse(data)
end)
```

- On success: returns `{ok: true, value: <return value>}`
- On error: returns `{ok: false, error: <error message string>, stack: <array of {name, file, line} tables>}`, innermost frame first, with the same names and paths a printed stack trace shows
- Errors caught by `pcall` do not print to stderr and do not call `exit(1)`
- If no `pcall` is active, errors behave as before (print + stack trace + exit)
- `pcall` catches both user `error()` calls and runtime type errors (e.g. `1 + "hello"`)
- Nested `pcall` works — each level catches errors independently
- To pass arguments to the called function, use a closure: `pcall f(x, y)` or `pcall(fn() f(x, y) end)`. A direct call of `pcall` with other than one argument (`pcall(f, x)`, `pcall()`) is a compile error, `` `pcall` takes one fn and calls it with no arguments ``; the same holds for `spawn`, `spawn_link` and `spawn_monitor` (`spawn do f(x) end`)

### Stack depth

Every process, the main process and each spawned one alike, has an 8 MB call stack. It is reserved address space: a process pays only for the stack it actually uses. Tail calls do not use stack (see Tail Call Optimization). Non-tail recursion can go roughly 30,000 calls deep for a small function, less for functions with many locals.

Running out of stack is an ordinary runtime error, not a crash:

- A call that would exhaust the stack raises `"stack overflow in <fn>"`, where `<fn>` is the function being called (`anonymous fn` for a `fn` literal). `pcall` catches it like any other error, so a request handler can turn runaway recursion into an error response and keep serving:

  ```
  let r = pcall walk(untrusted_tree)
  if not r.ok then reply(500, r.error) end   # "stack overflow in walk"
  ```

- Uncaught in a spawned process, it ends that process with that reason. Monitors receive `{tag: "DOWN", pid: p, reason: "stack overflow in walk"}`, links propagate it like any other exit reason, and every other process keeps running. The process is reported on stderr like any other uncaught error in a spawned process (see Error Handling).
- Uncaught in the main process, it is reported like any other uncaught runtime error: the message and a stack trace go to stderr, and the program exits with status 1. In the trace, a run of identical frames is shown once, followed by `... same frame repeated N more times`, and `... (deeper frames not recorded)` marks a trace cut short (only the outermost 256 frames are recorded).
- Native code that runs out of stack on its own — a recursive C function reached through `extern fn` — ends the process with reason `"stack overflow in native code called from <fn>"`, and `pcall` does **not** catch it, because the C code was interrupted midway. In the main process it is reported as an uncaught error (exit status 1). The runtime's own work on values never gets there: copying for `send`, `spawn` and arena resets is iterative, so a list nested millions of levels deep can be built, kept live in a loop, sent and received.

## Built-in Functions

`print(args...)` — prints values separated by spaces, followed by a newline. A string is written whole, NUL bytes included. Stdout is line-buffered, so each `print` flushes — long-running processes don't lose output on `SIGKILL` even when stdout is redirected to a pipe or file.

`error(msg)` — prints the message with file and line info to stderr, prints a call stack trace, and halts (`exit(1)`). A `msg` that isn't a string becomes its `to_string` text, here and in `pcall`'s `error` field (`error({code: 1})` raises `"{code: 1}"`); `error()` raises `"error"`.

`len(v)` — returns the length of a string, the byte length of a buffer, or the total number of entries in a table (both integer-keyed and string-keyed). `len({a: 1, b: 2})` returns 2. `len([10, 20, 30])` returns 3.

`type(v)` — returns the type name as a string: `"int"`, `"float"`, `"string"`, `"bool"`, `"nil"`, `"table"`, `"fn"`, `"ref"`, `"buffer"`, `"socket"`, `"sqlite"`.

`to_string(v)` — converts any value to its string representation. For buffers, returns the buffer contents as a string. For tables and arrays, recursively renders a `{key: val, ...}` / `[v1, v2, ...]` form (cycles render as `<cycle>`; deep/wide structures truncate with `...`). Strings inside render quoted, with `"`, `\\`, newline, tab, CR and NUL escaped (`["a\0b"]`), and every byte kept; a string key that is a plain identifier renders bare. Same repr is used by `print`, `eprint`, and `"{x}"` interpolation, except for buffers, which those show as `<buffer:N>` (N is the length); call `to_string(buf)` for the contents. Floats are formatted as described under "Numbers" in Values and Types (`to_string(2.0)` is `"2.0"`, `to_string(1234567.89)` is `"1234567.89"`); `buf_push` and `build_string`'s `add` format them the same way.

`to_int(v)` — converts a value to an integer. Strings are parsed as decimal integers; leading spaces are skipped, but trailing whitespace (a `\n` from a file line included) is an error, so `trim` first. Floats are truncated; one outside the int range, an infinity or NaN raises `to_int: <x> is out of the int range`. Bools become 0/1. Errors on nil, tables, functions, or unparseable strings.

`to_float(v)` — converts a value to a float. Strings are parsed as decimal floats, with an optional exponent (`"1.5e-7"`); `"inf"` and `"nan"` are accepted, and a value too large for a double, or so small it would read as zero, is an error (subnormals are kept). Ints are widened. Bools become 0.0/1.0. Errors on nil, tables, functions, or unparseable strings.

`push(target, val)` — if `target` is a table, sets `target[len(target)] = val`: on an array, appends `val` at the end (mutates in place, returns `val`). On a table that already has the key `len(target)`, that entry's value is replaced. If `target` is a buffer, appends `val` coerced to a string (same as `buf_push`; returns the buffer).

```
let items = []
push(items, "a")
push(items, "b")
print(len(items))  # 2
print(items[0])    # a
```

`pcall(f)` — calls `f()` with zero arguments in a protected context. Returns `{ok: true, value: <result>}` on success, `{ok: false, error: <message>, stack: <frames>}` on error. See Error Handling section.

`keys(tbl)` — returns a new table containing the keys of `tbl` as an integer-indexed array.

`str_replace(s, old, new)` — replaces all occurrences of `old` with `new` in string `s`. Returns a new string. If `old` is empty, returns `s` unchanged.

`has_key(tbl, key)` — returns `true` if `key` exists in the table, `false` otherwise. Unlike `tbl[key] != nil`, correctly detects keys whose value is `nil`.

`find(s, needle[, start])` — returns the byte offset of the first occurrence of `needle` in `s` at or after `start` (default `0`; a negative `start` counts as `0`), or `-1` when there is none. An empty `needle` is found at `start` (`-1` when `start` is past the end). Binary-safe; `s` and `needle` may be strings or buffers. The search runs in C (`memchr`).

`substr(s, start[, len])` — returns a substring of `s` starting at `start` (a negative `start` counts as `0`, unlike `s[-1]`). If `len` is provided, returns at most `len` characters; otherwise returns to the end of the string. Accepts buffers as well as strings (the result is always a fresh string).

`chr(n)` — converts an integer (0–255) to a single-character string with that byte value; any other int raises `chr: expected 0..255, got <n>`.

`ord(s)` — returns the byte value of the first character of string (or buffer) `s` as an integer.

`ord(s, i)` — returns the byte value at index `i` in string or buffer `s` without allocating a temporary string. Equivalent to `ord(s[i])` but avoids the 1-char string allocation.

`buf_new()` — creates a new mutable string buffer. Returns a buffer value (type `"buffer"`).

`buf_push(buf, val)` — appends `val` to the buffer. A string appends its bytes, a buffer its current contents (pushing a buffer into itself appends a copy of what it held), and any other value appends exactly what `to_string(val)` gives (`nil`, `true`, `42`, `<fn>`, `#Ref<3>`, `{a: 1}`). Returns the buffer for chaining. Uses a doubling growth strategy internally — O(n) total for n appends vs O(n²) for repeated `+` concatenation.

To finalize a buffer into an immutable string, use `to_string(buf)` — the generic `to_string` builtin handles buffers. The buffer remains usable afterward.

`build_string(block)` — creates a buffer, calls `block` with an `add` function that appends each of its arguments to the buffer as `buf_push` does, then returns the finalized string. Equivalent to `buf_new`/`buf_push`/`to_string` but more concise:

```
let html = build_string() do |add|
  add("<ul>")
  for item in items
    add("<li>{item}</li>")
  end
  add("</ul>")
end
```

`add` belongs to the process that called `build_string`. It can be captured, sent and stored like any function, but calling it from another process (a `spawn` body that captured it, a receiver of a message holding it) raises `` build_string: `add` can only be called by the process that created it ``. Calling it after `build_string` has returned does nothing visible: the string is already built.

`make_ref()` — returns a unique opaque reference value. Type is `"ref"`. Refs are equal only to themselves (identity equality). Usable as table keys. Format: `#Ref<N>` where N is a monotonically increasing integer.

`link(pid)` — creates a bidirectional link between the calling process and the target process. If the target no longer exists, the caller gets an exit signal with reason `"noproc"`, as if the target had just exited with that reason (Erlang semantics): a caller that traps exits receives `{tag: "EXIT", pid: <pid>, reason: "noproc"}`; any other caller dies at once with reason `"noproc"`, and `pcall` does not catch it (it is an exit, not an error). Linking to yourself does nothing. Returns `true`.

`unlink(pid)` — removes the bidirectional link between the calling process and the target. Returns `true`.

`spawn_link(fn)` — atomically spawns a new process and links it to the caller. No race window between spawn and link. Returns the new pid.

`process_flag("trap_exit", bool)` — sets the `trap_exit` flag on the current process. Returns the previous value (bool). When `trap_exit` is `true`, exit signals from linked processes are converted to `{tag: "EXIT", pid: <pid>, reason: <reason>}` messages in the process's mailbox instead of killing the process. A process that doesn't trap exits dies when a linked process exits with a reason other than `"normal"`; when that process is main, the program reports `main process killed by linked process <pid>: <reason>` with main's stack trace on stderr and exits with status 1. An unknown flag raises `process_flag: unknown flag "<name>" (the one flag is "trap_exit")`.

`sleep(ms)` — suspends the current process for `ms` milliseconds. Works in any process, including the main program.

`send_after(pid, msg, delay_ms)` — schedules `msg` to be delivered to `pid` after `delay_ms` milliseconds. Returns a unique ref identifying the timer. The timer fires from the scheduler, not the calling process. Timers for a process are dropped when it exits, and none is scheduled for a pid that has already exited.

`cancel_timer(ref)` — cancels a pending timer by ref. Returns `true` if cancelled, `false` if it already fired, was dropped because its target exited, or is not found.

`processes()` — returns a list of all live process pids (excludes free and dead processes).

`process_info(pid)` — returns a table with process metadata: `state`, `mailbox_len`, `links`, `monitors`, `trap_exit`, `exit_reason`, `resources`, `memory`. Returns `nil` for invalid/free pids.

`runtime_stats()` — returns a table of figures for the whole program: arena resets (`resets`, `full_resets`, `return_resets`, `reset_copied`, `reset_freed`, `reset_ms`, `reset_max_ms`, `resets_over_1ms`, `resets_over_10ms`, `resets_over_100ms`), processes (`procs`, `proc_hwm`, `max_procs`, `spawn_refused`), `memory`, and resources (`resources_open`, `resources_ownerless`). See Process Introspection.

`read_file(path)` — reads the entire file at `path` and returns its contents as a string. Opens in binary mode (no newline translation). Files whose size isn't known up front (`/proc` and `/sys` files, pipes, devices such as `/dev/stdin`) are read until end of file. Raises an error if the file cannot be opened (`read_file: cannot open '<path>': <reason>`, the system's reason such as `No such file or directory` or `Permission denied`; `write_file`, `append_file` and `list_dir` say why the same way), is a directory, is larger than the string limit (see Strings), or a read fails.

`read_file(path, offset, count)` — reads up to `count` bytes of the file at `path` starting at byte `offset`, and returns them; `""` at or past the end of the file, so a file can be walked in blocks (`std/io` does). The file is opened for each call, so nothing stays open between calls. Only a regular file (or a block device) keeps its bytes from one open to the next: a pipe, FIFO, terminal or other device hands each open new data, so for one of those an `offset` past 0 raises `read_file: '<path>' is not a regular file: it can be read from offset 0 only (read it whole with read_file(path))` (without opening it), and `offset` 0 reads up to `count` bytes of what arrives. Runs on the thread pool, like `read_file(path)`. A negative `offset` or `count` raises `read_file: negative offset` (`count`); errors opening or reading the file are those of `read_file(path)`. Any other number of arguments than 1 or 3 raises.

`write_file(path, content)` — writes the string `content` to `path`, overwriting any existing file. Opens in binary mode. Raises an error if the file cannot be opened or if the write fails.

`append_file(path, content)` — appends the string `content` to `path`. Creates the file if it doesn't exist. Opens in binary mode. Raises an error on failure.

`delete(tbl, key)` — removes the entry for `key` from the table. Returns the removed value, or `nil` if the key was not found. Uses swap-remove internally (O(1) but does not preserve insertion order). On an array it removes the key too: `delete(arr, 0)` on `["a", "b", "c"]` leaves the keys `1` and `2`, a table that is no longer an array. To remove an element from a list, use `remove_at`, which shifts the rest left.

`pop(arr)` — removes and returns the last element of an array table. Errors on empty table.

`values(tbl)` — returns a new integer-indexed array containing all values from `tbl`, in storage order.

`eprint(args...)` — like `print`, but writes to stderr instead of stdout.

`exit(code)` — terminates the program with the given exit code (default 0).

`argv()` — returns the command-line arguments as an integer-indexed array. `argv()[0]` is the program name.

`sort(arr)` — sorts an array (a table whose keys are exactly `0 .. n-1`, in any order) in place using the default ordering: numbers ascending, strings by byte order, and mixed types in the order nil, bools, numbers, strings, fns, tables (bools, fns and tables tie among themselves, in no set order). Returns the table. Any other table raises `sort: expected an array (keys 0 .. n-1), got a table with other keys`, and a second argument that is neither a fn nor `nil` raises `sort: comparator must be a fn, got <type>`.

`sort(arr, cmp)` — sorts with a custom comparator function. `cmp(a, b)` must return a negative number if a < b, 0 if equal, positive if a > b. The sort is stable (elements the comparator calls equal keep their order). A comparator may sort other arrays itself, and may run while other processes sort. The comparator sees the array as it was before the sort, and what it stores into the array's slots (`arr[i] = x`) is lost (changes inside an element table are kept): the sorted result is written back at the end. A comparator that returns anything but an int or a float (a bool, `nil`) raises `sort: the comparator must return a number (negative, zero or positive), got <type>`. If it raises, the error propagates and the array is left as it was; one that changes the length of the array being sorted raises `sort: the comparator changed the length of the table being sorted`.

`floor(x)` — returns the largest integer ≤ x. Returns the value unchanged if already an integer. A float outside the int range, an infinity or NaN raises `floor: <x> is out of the int range` (`ceil`, `round` and `to_int` likewise).

`ceil(x)` — returns the smallest integer ≥ x. Returns the value unchanged if already an integer.

`round(x)` — rounds to the nearest integer (half away from zero). Returns the value unchanged if already an integer.

`abs(x)` — returns the absolute value. Preserves type (int→int, float→float).

`pow(x, y)` — returns x raised to the power y. Returns int if both arguments are int and y ≥ 0, computed exactly and wrapping on overflow like `*`; float otherwise.

`sqrt(x)` — returns the square root of x as a float. Errors on negative argument.

`random()` — returns a random float in [0, 1). Uses xorshift64* internally.

`random(n)` — returns a random integer in [0, n). Errors if n ≤ 0.

`getenv(name)` — returns the value of environment variable `name` as a string, or `nil` if not set (or when `name` is empty or holds `=` or a NUL byte). It reads the program's environment as `setenv` leaves it.

`setenv(name, value)` — sets environment variable `name` to the string `value`; `value` nil removes it. Returns nil. Later `getenv` calls see the change, and so do the commands `exec` and `exec_capture` start afterwards. An empty `name`, one with `=` or a NUL byte, or a `value` with a NUL byte raises. The environment is the program's, shared by every process. The runtime keeps it apart from the C library's: C code reached through `extern fn` (and libc itself, e.g. for `TZ`) sees the environment the program started with, never what `setenv` changed.

`input()` — reads a line from stdin, of any length and binary-safe (NUL bytes are kept), stripping the trailing `\n` or `\r\n`. A last line without a newline is returned as is. Returns `nil` on EOF.

`input(prompt)` — prints `prompt` to stdout (no newline), then reads a line from stdin.

`read_stdin(n)` — reads exactly `n` bytes from stdin and returns them as a binary-safe string. Returns `""` for `n == 0`. On EOF the returned string may be shorter than `n` (or empty if EOF arrives first). Blocks the OS thread on `fread` — does not yield to the scheduler. Intended for length-framed protocols (e.g. LSP `Content-Length` bodies); for line-oriented input use `input()`.

`read_stdin()` — reads stdin to its end and returns everything as a binary-safe string (`""` when it is already at its end). Blocks the OS thread, like `read_stdin(n)`. Raises when the input passes the string limit.

`write_stdout(s)` — writes `s` to stdout as raw bytes (no trailing newline), after anything `print` left buffered. It returns once every byte is written (on a non-blocking stdout it waits until stdout takes them); a write error, such as a closed reader, drops the rest. Binary-safe (uses the string's stored length, not `strlen`). Pair with `read_stdin` when implementing framed stdio protocols where the receiver expects the frame on the wire as soon as the writer is done.

`insert(arr, i, val)` — inserts `val` at index `i` in an array table, shifting subsequent elements right. Returns the table. Errors if `i < 0` or `i > len(arr)`.

`remove_at(arr, i)` — removes and returns the element at index `i`, shifting subsequent elements left. Errors if `i` is out of bounds.

`band(a, b)` — bitwise AND of two integers. Both arguments must be integers.

`bor(a, b)` — bitwise OR of two integers. Both arguments must be integers.

`bxor(a, b)` — bitwise XOR of two integers. Both arguments must be integers.

`bnot(a)` — bitwise NOT (complement) of an integer. The argument must be an integer.

`bshl(a, n)` — left-shift `a` by `n` bits. Both arguments must be integers; `n` must be in the range 0..63.

`bshr(a, n)` — logical (unsigned) right-shift `a` by `n` bits. Both arguments must be integers; `n` must be in the range 0..63. The result is always non-negative.

`file_exists(path)` — returns `true` if `path` can be opened for reading, `false` otherwise. Argument must be a string.

`dirname(path)` — returns the parent directory component of `path` (POSIX `dirname`). Argument must be a string. `dirname("/foo/bar")` → `"/foo"`. `dirname("/")` → `"/"`.

`path_join(dir, file)` — joins two path components with `/`. If `file` is an absolute path it is returned as-is. Handles trailing slashes in `dir`. Both arguments must be strings.

`normalize_path(path)` — resolves symlinks and `.`/`..` components using `realpath`. Returns the absolute canonical path if the file exists, or `path` unchanged if it does not. Argument must be a string.

`remove_file(path)` — removes the file at `path` using `unlink`. Returns `true` on success, `false` on failure. Argument must be a string.

`mkdir(path)` — creates a directory at `path` with permissions `0755`. Returns `true` on success, `false` if the directory already exists or the operation fails. Argument must be a string.

`list_dir(path)` — returns an integer-indexed array of filenames in the directory at `path`, excluding `.` and `..`. Raises an error if the path does not exist or is not a directory. Argument must be a string.

`is_dir(path)` — returns `true` if `path` exists and is a directory, `false` otherwise. Argument must be a string.

`exec(command)` — runs `command` via the system shell (`sh -c`). Blocks until the command exits. Returns the exit code as an integer (0 on success). The shell expands glob patterns and environment variables in `command`. The command inherits the program's stdout and stderr, so its output goes straight to the terminal (or wherever the program's output goes), the same from `main` and from a spawned process; `exec` does not capture it. To capture output, redirect it inside `command` (`exec("ls > /tmp/out.txt")`) and `read_file` the result.

`exec_capture(command[, input])` — runs `command` via the system shell (`sh -c`) and returns `{status, stdout, stderr}`: the exit status (`-1` when the command could not start or was killed by a signal, as for `exec`) and everything it wrote to its stdout and stderr, as binary-safe strings. `input` (a string) is written to the command's stdin; without it (or with `nil`) its stdin is at its end at once, so a command that reads stdin never waits on the terminal. Like `exec`, it runs on the thread pool, so other processes run while the command does, and it holds a pool worker until the command exits. Output past the string limit raises `exec_capture: the command's stdout is over the string limit of 2147483646 bytes` (`stderr`). A command started by `exec` or `exec_capture` inherits the program's stdin, stdout and stderr (for `exec_capture`, its pipes and the stdin described above) but none of the runtime's sockets or pipes, so it never holds a connection or another command's output open; it gets the default handling of `SIGPIPE` (the runtime ignores it), so a pipeline such as `yes | head -n 2` ends.

`rename(from, to)` — renames (moves) the file or directory `from` to `to` with `rename(2)`, replacing a file at `to`; atomic within one file system. Returns `true` on success, `false` on failure (no such file, another file system, no permission). Both arguments must be strings. Runs inline.

`stat(path)` — returns `{size, mtime, mode, type}` for the file at `path`, or nil when there is none: `size` in bytes, `mtime` the last change in milliseconds since the epoch (as `epoch_ms()`), `mode` the permission and mode bits (`st_mode & 07777`: `rw-r--r--` is `420`, and setuid, setgid and sticky add `2048`, `1024` and `512`), and `type` `"file"`, `"dir"`, `"link"` (only for a link whose target is missing) or `"other"`. Follows symlinks. Runs inline.

## Owned Resources

Sockets and sqlite database handles are *owned resources*: values of their own types (`type` is `"socket"` or `"sqlite"`), not file descriptors or pointers. A resource value names an entry of one runtime table by an id that is never reused. It prints as `#Socket<N>` / `#Sqlite<N>`, `N` that id. It is equal only to itself (in any process, after any number of copies), works as a table key, and sorts like a ref; `<` on it raises (`type error in <: got socket and socket`), as do `to_int` and `json.encode`. `spawn`, `send` and every other copy copy the value, not the resource: every copy names the same socket.

The rules:

1. Every resource has an **owner**, at first the process that opened it (`tcp_listen`, `tcp_accept`, `tcp_connect`, `tls_connect`, `tcp_from_fd`, `sqlite_open`).
2. **`claim(r)`** makes the calling process the owner and marks the resource *claimed*. Nothing else changes ownership: `spawn`, `send` and copies leave it where it is. `claim` returns `r`, never waits, and the last claim wins. It raises `claim: socket is closed` (`claim: database is closed`) for a closed resource and `claim: expected a socket or a database handle, got <type>` for any other value.
3. **A claimed resource is closed when its owner exits, for any reason. An unclaimed one is closed when its opener crashes, is killed or dies from a link's exit signal**; when the opener returns normally it stays open, with no owner, until some process closes or claims it.
4. **Any process that has a resource can use it and close it.** `tcp_close` and `sqlite_close` of a closed resource return `nil`.
5. **A closed resource stays closed in every process.** Any other use raises (`tcp_read: socket is closed`, `sqlite_query: not an open database handle`), including a builtin that was waiting on the socket when it closed, even if a new socket has the same fd number by then.

| How the owner ends | Claimed resources | Unclaimed (opener's) resources |
|---|---|---|
| Returns normally (main included) | closed | stay open, with no owner |
| Uncaught error (`error("normal")` included) | closed | closed |
| `kill(pid, reason)`, any reason but `"normal"` (`"kill"` included) | closed | closed |
| Exit signal from a link, not trapped | closed | closed |
| Main crashes, or `exit(code)` | the program ends; the OS closes everything | same |

A process that traps exits receives `EXIT` and doesn't die; if it then returns, that is a normal exit. A process killed while its own `extern blocking fn` runs on the thread pool keeps its sockets open until the C call returns (the call may be using their fds through `tcp_fd`), then they close; a `claim` by another process meanwhile takes one over. Every other resource a process's exit closes is closed before any other process runs, so before its `DOWN` or `EXIT` is handled: a database closed that way can be opened again at once (the close runs on the scheduler thread, and every process waits for it, as for `sqlite_exec`).

So the process responsible for a connection claims it first: an acceptor accepts and hands each socket on, and the session that serves it calls `claim(sock)`, whose exit (a crash, a kill, an early return, an orderly shutdown) closes it. Helpers that only use a socket (a reader process, a `task.async` that writes to it, a watchdog that closes it) never close it by exiting. A session that forgets to claim leaves its socket with the acceptor: `process_info(pid).resources` counts the open resources a process owns, `GEM_DIAG=2` prints a `gem_resources:` line on stderr for each process that exits leaving resources open that it owned or used without owning (`gem_resources: process 412 (session) exited (error) after using 1 socket owned by process 3 (acceptor)`), and `GEM_DIAG=1` adds `resources_open=N ownerless=M` to the `gem_diag:` line at exit.

## TCP Sockets

`tcp_listen(host, port)` — creates a TCP server socket bound to `host` (string: an IPv4 address, `"0.0.0.0"` for every interface, or a host name such as `"localhost"`, resolved to its first IPv4 address; a name that doesn't resolve raises `tcp_listen: cannot resolve '<host>'`) on `port` (int; `0` binds a free port, which `tcp_local_port` reports). Calls `socket`, `bind`, and `listen` with a backlog of 1024. Sets `SO_REUSEADDR`. A port outside 0–65535 raises `tcp_listen: port must be from 0 to 65535, got <port>` (`tcp_connect` likewise). Every socket the runtime opens is marked close-on-exec, so a command `exec` runs doesn't inherit it (one that starts at the very moment a socket is opened still can). Returns the socket, owned by the calling process (see Owned Resources). Raises an error on failure. A host name is resolved as for `tcp_connect`, while the calling process waits and the others run; the rest of the call doesn't wait.

`tcp_connect(host, port[, timeout_ms])` — opens a TCP connection to `host:port`. Returns the connected socket. Raises an error if the connection fails (`tcp_connect: connect failed: <reason>`; `tcp_connect: connect failed: socket closed while connecting` when another process closes it meanwhile) or the host cannot be resolved (`tcp_connect: cannot resolve '<host>'`). Supports both IP addresses and hostnames. The connect itself is non-blocking: the calling process (main included) yields to the scheduler until it completes, and owns the socket from the start, so a process killed mid-connect closes it. So does resolving a host name: `getaddrinfo` runs on a thread kept for name lookups ("Green Threads and Message Passing"; an IP address needs no lookup), and a process killed during a lookup ends at once. `timeout_ms` works like `tcp_read`'s: `nil` or omitted waits for the connection (or for the system to give up on it), an int is a deadline, and one of `0` or less gives up unless the connection is made at once. The deadline covers the name lookup too. Past it `tcp_connect: connect to <host>:<port> timed out` is raised, after closing the socket if the connect has begun; a lookup still running then finishes on its thread. Any other `timeout_ms` raises `tcp_connect: timeout_ms must be an int (milliseconds) or nil, got <type>`.

`tcp_accept(socket)` — accepts an incoming connection on a listening socket. Returns the new connection's socket, owned by the calling process. The calling process (main included) yields to the scheduler until a connection is ready. Raises an error on failure.

`tcp_read(socket[, max_bytes[, timeout_ms]])` — reads up to `max_bytes` bytes from a connected socket (an int `> 0`, or `nil` for the default 4096; anything else raises `tcp_read: max_bytes must be an int > 0 or nil, got <n>`, or `got <type>` for a non-int). Returns the data as a string on success, `""` when the remote end has closed the connection (EOF or `ECONNRESET`), or `nil` when the optional `timeout_ms` expires with no data available. Callers without a timeout never see `nil`. While no data is available the calling process (main included) yields to the scheduler, and resumes when data arrives or the timeout deadline is reached. `timeout_ms` works like `receive ... after`: `nil` or omitted waits until data or EOF, and an int of `0` or less doesn't wait: it returns what is already there, or `nil` (so a remaining time computed as `deadline - time_ms()` that has run out never blocks). Any other `timeout_ms` raises `tcp_read: timeout_ms must be an int (milliseconds) or nil, got <type>`.

`tcp_write(socket, data[, timeout_ms])` — writes the string (or buffer) `data` to a connected socket. Writes all bytes (loops internally on partial writes). Returns the number of bytes written as an integer. The calling process yields to the scheduler while the socket is not writable. `timeout_ms` is one deadline for the whole write, like `tcp_read`'s: `nil` or omitted waits until every byte is written, an int of `0` or less writes only what the socket takes at once, and past the deadline `tcp_write` returns the count written so far (less than `len(data)`), without raising. Any other `timeout_ms` raises `tcp_write: timeout_ms must be an int (milliseconds) or nil, got <type>`. Writing to a peer that has closed the connection does not raise: the first write usually still reports success and later ones return `0`.

`tcp_close(socket)` — closes a socket, from any process. Always synchronous. Returns `nil`, also for a socket that is already closed. A process waiting in `tcp_accept`, `tcp_read`, `tcp_write` or `tcp_connect` on that socket raises `<builtin>: socket is closed` (`tcp_connect: connect failed: socket closed while connecting`), even when a new socket has taken the same fd number by the time it runs.

`tcp_peer(socket)` — returns the address of a connected socket's remote end as a table `{ip, port}`: `ip` is a string (`"203.0.113.7"`), `port` an int. Returns `nil` when the socket has no peer: a listening socket, or a connection the peer has reset. A peer that closed cleanly keeps its address only until a write to it draws a reset, so read the address once, right after `tcp_accept`, and keep it. Raises `tcp_peer: not a TCP socket` for a socket that isn't a TCP one (only `tcp_from_fd` could make one) and `tcp_peer: getpeername failed: <reason>` for any other failure. Never waits.

`tcp_local_port(socket)` — returns the port the socket is bound to on this machine, as an int: the one a listening socket took for port `0`, or a connection's local port. Raises `tcp_local_port: getsockname failed: <reason>` on failure. Never waits.

`tcp_fd(socket)` — returns the socket's file descriptor number as an int (a TLS socket raises `tcp_fd: a TLS socket has no fd to give to C`), for debugging and for C code that needs the number outside a call (a `Socket` extern parameter passes a socket to C more safely, see C Interop). The fd stays valid while the socket is open. C code must not close it (`tcp_close` does) or keep it after the socket closes: the number can then belong to a new file. If C code does close it, the runtime notices when it closes the socket itself (it compares the fd's inode, or on macOS the kernel's id of the socket, with the one it registered) and leaves whatever took the number alone, but a `tcp_read` or `tcp_write` on the stale socket meanwhile can reach that file.

`tcp_from_fd(fd)` — registers a TCP socket made outside the runtime (by a TLS library, or inherited from the parent process) and returns it as a socket opened by the caller. From then on the runtime owns the fd: it marks it non-blocking and close-on-exec and closes it with the socket. Raises `tcp_from_fd: fd 9 is not open`, `tcp_from_fd: fd 9 is not a TCP socket` (any fd but an IPv4 or IPv6 stream socket), `tcp_from_fd: fd 9 is already registered as a socket` (the fd of an open socket), and `tcp_from_fd: expected an int fd, got <type>`.

Every TCP builtin that takes a socket raises `<builtin>: expected a socket, got <type>` for any other value (an int fd included) and `<builtin>: socket is closed` for a closed one (`tcp_close` returns `nil` instead). All TCP builtins use non-blocking sockets with scheduler poll integration. The scheduler's `poll()` loop handles readiness notification with zero thread pool overhead.

## TLS

`tls_connect(host, port[, opts])` — opens a TCP connection to `host:port` as `tcp_connect` does, then makes it a TLS connection (TLS 1.2 or 1.3) as a client, and returns it as a socket (`type` `"socket"`): `tcp_read`, `tcp_write`, `tcp_close`, `tcp_peer`, `tcp_local_port` and `claim` work on it as on any socket, with the same arguments, results, timeouts and waits, reading and writing plaintext, except at the end of the stream and after a short write (both below). `tcp_fd` and a `Socket` extern parameter refuse it. A TLS socket is an owned resource like any other: the calling process owns it from before the connect, so a process killed mid-handshake closes it. `tcp_close` discards input that has arrived and nobody read, up to 1 MB, without waiting (a TLS 1.3 server's session tickets, for one: closing with input unread resets the connection, and the end of what was written can be lost), sends the TLS close alert when the socket can take it at once, then closes. A key of `opts` other than `timeout_ms`, `ca_file` and `server_name` raises `tls_connect: unknown option "<key>"`, naming the closest one when one is close enough.

The server's certificate is always verified: its chain must lead to a trusted certificate, and it must name `host` (a DNS name, matched with `*.` wildcards, or an IP address). `host` is also sent as the server name (SNI) unless it is an IP address. The trusted certificates are the system's, as OpenSSL finds them in its own directory (`openssl version -d` shows it): on Debian and Ubuntu the distribution's CA bundle under `/etc/ssl/certs`, on macOS Homebrew OpenSSL's `cert.pem`; the `SSL_CERT_FILE` and `SSL_CERT_DIR` environment variables override them. The first `tls_connect` that uses them reads them, on the scheduler thread, so every process waits for that once.

`opts` (a table, all keys optional):

- `timeout_ms` — an int `> 0`, or `nil` (the default) for no limit: one deadline for the connect and the handshake.
- `ca_file` — a PEM file of the certificates to trust, in place of the system's: a private CA, or a test server's own certificate. Read at every call.
- `server_name` — the name to verify the certificate against and send as SNI, in place of `host` (to reach a server by IP address under its name).

Errors, besides `tcp_connect`'s with the `tls_connect: ` prefix:

- `tls_connect: certificate verify failed for <host>:<port>: <reason>`, with OpenSSL's reason: `unable to get local issuer certificate` (an unknown CA), `self-signed certificate`, `certificate has expired`, `hostname mismatch`, `IP address mismatch`. With a `server_name`, `for <host>:<port> (server name <name>): ...`. When there is no trusted certificate where OpenSSL looks (nothing loaded from its file, and its directories empty or missing), `unable to get local issuer certificate` is followed by ` (no trusted certificates found in <file> or <dir>; set SSL_CERT_FILE or SSL_CERT_DIR)`, naming those places.
- `tls_connect: handshake with <host>:<port> failed: <reason>` for any other failure: `connection closed by peer`, `unsupported protocol` (a server without TLS 1.2), `wrong version number` or `packet length too long` (a server that doesn't speak TLS).
- `tls_connect: handshake with <host>:<port> timed out` and `tls_connect: connect to <host>:<port> timed out` past `timeout_ms`.
- `tls_connect: cannot load ca_file '<path>': <reason>`; `tls_connect: opts must be a table, got <type>`; `tls_connect: timeout_ms must be an int > 0 (milliseconds) or nil, got ...`; `tls_connect: ca_file must be a non-empty string without NUL bytes, got ...` (`server_name` likewise).

On a TLS socket, `tcp_read` returns `""` at the end of the stream only after the server's TLS close alert. A connection that ends without it raises `tcp_read: the TLS connection was closed before the server's close alert; the data may be cut short` (`was reset` for a reset), since what was read may be missing its end. A protocol that frames its data needn't read that far: `std/request` stops at the end of a `Content-Length` body and returns a chunked body once its last chunk has come (a stream that ends in the trailer after it is tolerated), and raises `request.<fn>: tcp_read: the TLS connection was closed ...` only for a body that runs to the end of the stream. A TLS protocol error raises `tcp_read: TLS error: <reason>` (`tcp_write: ...`).

A `tcp_write` on a TLS socket that times out short of `len(data)` can leave one TLS record in flight, holding bytes the returned count doesn't include. The next `tcp_write` on the socket must start with them, as writing the rest, `substr(data, written)`, does; one that doesn't (other data, or fewer bytes than the record holds) raises `tcp_write: an earlier tcp_write on this TLS socket timed out with <n> bytes in flight; the next write must start with the bytes it didn't count`. An empty write is allowed and writes nothing.

TLS needs OpenSSL 3 or later when Gem is built: `make build` looks for it (Homebrew's `openssl@3`, then `pkg-config`, then the C compiler's default paths; the `OPENSSL_CFLAGS` and `OPENSSL_LIBS` environment variables name it directly, and `GEM_TLS=0` builds without it) and prints which it found whenever that changes. A program whose code names `tls_connect`, which includes every program that loads `std/request`, is linked against OpenSSL's shared libraries, which must be on any machine it runs on. A Gem built without OpenSSL compiles such programs, but `tls_connect` raises `tls_connect: this Gem was built without OpenSSL 3; ...`.

## SQLite

`sqlite_open(path)` — opens (or creates) a SQLite database at `path`. Enables WAL mode and foreign keys by default. Returns a database handle (an owned resource of type `"sqlite"`, see Owned Resources), valid in every process until it is closed, and owned by the calling process. Use `":memory:"` for an in-memory database. Runs on the thread pool in every process, main included, so other processes keep running. Raises on error.

`sqlite_close(db)` — closes the database handle, from any process. Runs on the thread pool, like `sqlite_open`. Returns `nil`, also for a handle that is already closed. The handle is invalid from the moment the close starts, in every process. A handle whose owner exits closes as Owned Resources says, through the thread pool too.

Every other `sqlite_*` builtin that takes a handle raises a catchable error prefixed with its name when the value is not a handle (`sqlite_query: expected a database handle, got int`) or the handle is closed (`sqlite_query: not an open database handle`).

`sqlite_exec(db, sql)` — executes SQL that returns no rows (DDL, INSERT without RETURNING, etc.). SQL containing a NUL byte raises `sqlite_exec: the SQL contains a NUL byte` (`sqlite_query` likewise), since sqlite would ignore what follows it. `sql` may hold several statements, run in order (rows a statement returns are dropped); empty or comment-only SQL does nothing. A statement with placeholders raises `sqlite_exec: statement <n> has parameters; use sqlite_query to bind them`, since there is nothing to bind them to. Inline execution (no thread pool). Raises on error; the statements before the failing one have run.

`sqlite_query(db, sql, params)` — executes a parameterized query. `params` is an array of bind values matching the placeholders in `sql` (`?`, `?N`, `:name`, ...): element `i` binds parameter `i + 1` (a `:name` placeholder's number is its position among the statement's parameters), whatever order the elements were added in. Or it is a record of names, binding each named placeholder (`:name`, `@name` or `$name`) to the value at `name` (`sqlite_query(db, "SELECT * FROM t WHERE id = :id", {id: 5})`; a placeholder used twice reads the same value, and a key holding `nil` binds NULL). A record raises for a positional placeholder (`sqlite_query: parameter 2 is positional (?); bind it with a params array, or name it (:name)`), a name it lacks (`sqlite_query: no value for parameter :b (params has no key "b")`) and a key no placeholder names (`sqlite_query: params key "x" matches no parameter of the statement`). `params` may be omitted or `nil` when the statement has none. A table with both string and other keys raises `sqlite_query: params must be an array or a record of names, not both`; an array with any key other than `0..n-1` raises `sqlite_query: params must be an array (keys 0..1), got key 5` (a float or other non-int key: `sqlite_query: params must be an array, got a float key`). For an array, the number of values must equal the statement's parameter count (the highest placeholder index, so `?1` used twice counts once). Each value must be nil, a bool (bound as 1/0), an int, a float or a string; otherwise `sqlite_query` raises, e.g. `sqlite_query: statement has 2 parameter(s), got 1` or `sqlite_query: parameter 1 is a table; expected nil, bool, int, float or string` (for a record, the parameter is named: `parameter :a is a table; ...`). `sql` is one statement (a trailing `;`, whitespace and comments are fine); more raises `sqlite_query: expected one SQL statement, got several (sqlite_exec runs several, without parameters)` before anything runs. Empty or comment-only SQL returns `[]`. Returns an array of row tables, where each row is a string-keyed table (e.g., `{id: 1, name: "Alice"}`). Column type mapping: INTEGER → Int, REAL → Float, TEXT → String (embedded NULs kept), NULL → Nil, BLOB → String (raw bytes). Inline execution (no thread pool). Raises on error.

`sqlite_last_insert_id(db)` — returns `sqlite3_last_insert_rowid` as an int.

`sqlite_changes(db)` — returns the number of rows affected by the last INSERT/UPDATE/DELETE as an int.

SQLite is vendored as an amalgamation (`runtime/sqlite3.c` + `runtime/sqlite3.h`), compiled into the runtime static library. No system dependency needed.

**Negative array indexing** — Integer indices to arrays and strings may be negative. A negative index `i` on a collection of length `n` resolves to `n + i`. So `arr[-1]` is the last element, `arr[-2]` is second-to-last, etc. Negative integers are always positions, never keys: `t[-10] = x` on a table with fewer than 10 entries raises, so key data by negative numbers with strings. Reading an array at a non-negative index past the end gives `nil`; a negative index that is still out of bounds after resolution raises `array index out of bounds`. A string index out of range either way raises `string index out of bounds`.

All builtins are first-class values — they can be stored in variables and passed to functions.

**Builtin names are not reserved.** Builtins live in the outermost scope, so any binding with a builtin's name shadows the builtin wherever that binding is in scope, and every call or reference there reaches the binding:

- A top-level `fn`, `extern fn` or `let` (a destructuring `let` or a selective import such as `load "std/log" (error)` included) shadows the builtin in the file it is in, whether that is the program's entry file or a loaded module: a `fn` or `extern fn` for the whole file; a `let` in every function and closure of the file (as for any module-level binding) and in the top-level code from that `let` on. Like any shadowing `let`, its initializer, closures in it included, still sees the builtin, as does top-level code before it: `let keys = keys(t)` binds the keys of `t`, and `let print = fn(x) print("> " + x) end` wraps the builtin. It does not leak into modules that file loads, or into files that load it: `std/log` defines and exports `error`, and `log.error(msg)` logs while a bare `error(msg)` elsewhere still raises.
- A parameter, `let` local, loop variable or pattern binding shadows it for its scope (`fn f(len) len + 1 end` is fine).

`for` loops and `match` patterns keep working inside such a scope: they never call a user binding named `len`, `type` or `has_key`.

## Module System

**Load statement** — `load` brings another file's exported definitions into scope. Every loaded file must have an `export` statement declaring its public API. The compiler keeps a table of already-loaded file paths; re-importing a module skips re-parsing but still creates the requested import bindings. `load` and `export` are statements of a file's top-level code only; one inside a function body or a top-level block (`if`, `while`, `for`, `match`, `receive`, `do`) is a compile error.

```
load "compiler/parser"
load "std/table"
```

Path resolution depends on the form of the load path:

1. **`load "std/X"`** — reserved for the standard library. Resolves against the stdlib root (see below). A compile error if the file does not exist; never falls back to a local file.
2. **`load "./X"` / `load "../X"`** — relative to the importing file's directory. Use this for sibling/cousin imports inside a project (e.g. `compiler/main.gem` doing `load "./parser"`).
3. **`load "X"` or `load "X/Y"`** (bare path, no prefix) — relative to the **project root**, defined as the nearest ancestor directory of the entry source file containing a `gem.toml` marker (searched from the entry file's real location, so `gem main.gem` run inside a subdirectory of the project finds it too). If no `gem.toml` is found, falls back to the importing file's directory.

A `load` whose file does not exist is a compile error at that `load`, naming the path it looked for (shown like other paths in compile errors): `` cannot find module `mods/nope` (no file mods/nope.gem) ``. The same holds in a loaded module, where the error points at the module's own `load` line, and for a path that names a directory (`` cannot load `./d`: d.gem is a directory ``; loading `./mods` when `mods/` is a directory hints at loading a module inside it). `gem lsp` reports the same error for the open file's loads.

**Stdlib root** is resolved in this order:

1. `$GEM_STDLIB` if set: the directory that *contains* `std/` (not `std/` itself).
2. The project root, if it contains a `std/` subdirectory (lets a project vendor or override the stdlib). It replaces the whole stdlib: a `load "std/x"` that isn't in the project's `std/` fails, with no fallback to the installed one.
3. The install root: the directory two levels above the `gem` binary's real path, so a binary at `<root>/build/gem` finds `<root>/std/`. The real path is the operating system's answer (`/proc/self/exe` on Linux, `_NSGetExecutablePath` on macOS), else `argv()[0]` looked up on `PATH` when it has no `/`; symlinks are resolved either way. So a symlink to the binary on `PATH`, a chain of symlinks, a bare `gem` found on `PATH` and a relative path all find the checkout's `std/`.

The install root also holds what every compile needs besides the stdlib: `runtime/` (headers) and `build/libgem_runtime.a`. Those always come from the install root; `$GEM_STDLIB` moves only the stdlib, so a binary copied out of its checkout (not symlinked) cannot compile.

**Project root marker** — drop a `gem.toml` file at the root of your project to mark it. The file may be empty; its presence is what matters. Without it, bare-path loads behave like relative-to-importing-file (which is the safe default for single-file scripts).

**Export declaration** — `export name1, name2, ...` declares which names a file exports. By convention it is placed at the end of the file; a file may have several `export` statements, and they all count (as one list).

Each exported name must be bound at the module's top level: a `fn`, `extern fn` or `let` (a destructuring `let` and the bindings of the module's own `load`s included, so a module can re-export a name it imports or a namespace it loads). A name the module doesn't define, or one listed twice, is a compile error at that name in the `export` list, in the file being compiled as in a loaded one.

```
# std/string.gem
fn split(s, delim)
  # ...
end

fn _helper(s)
  # internal, not exported
end

export split, join, trim, index_of, starts_with, ends_with, upper, lower, contains, repeat
```

When a file has an `export` statement, `load` treats it as a module:

- The file's code is scoped — non-exported names are private and not accessible from outside.
- The exported names are collected into a table named after the file's basename (`load "std/string"` binds `string`), so that name must be an identifier and not a keyword: `load "./my-utils"` is a compile error at the `load` (rename the file to `my_utils.gem`, or use `as` or a selective import, which don't bind the basename).
- Two different files loaded by one file can't bind the same namespace name: `load "std/string"` then `load "./lib/string"` is a compile error at the second `load`; load one of them with `as`. Loading the same file again (under any path that resolves to it) is fine.
- Modules are distinct by file, not by name: a module and one with the same basename loaded elsewhere in the program (a user `json.gem` while `std/http` loads `std/json`) are separate modules with separate state, and each file sees the one it loaded. Stack traces and messages name a module's bindings `<basename>.<name>`.

```
load "std/string"
string.split("a,b,c", ",")    # OK — split is exported
string._helper("x")           # error — _helper is not exported
```

Loading a file without an `export` statement is a compile error.

`m.x` is the module's own binding `x` in the running process, not a copy: `m.add("a"); len(m.items)` sees the item `add` pushed onto the module's `items`, and after the module reassigns `items`, `m.items` is the new value. This also holds when the namespace is used as a value (`let ns = m; ns.items`). The values a namespace exports stay as mutable as they are inside the module (`push(m.items, x)` changes the module's `items` in this process); only the namespace table's own entries are frozen.

**Module aliasing** — `load "path" as name` binds the module table to a custom name instead of the basename:

```
load "std/string" as str
str.split("a,b,c", ",")
```

**Selective import** — `load "path" (name1, name2)` imports specific exported names directly into scope without a namespace prefix:

```
load "std/string" (split, trim)
split("a,b,c", ",")     # OK — imported directly
trim("  hello  ")        # OK — imported directly
join(parts, ",")         # error — join was not imported
```

Naming a name the module doesn't export (one it doesn't define, or a private one) is a compile error at that name in the `load`: `module string has no export nosuch`.

All three `load` forms use the same two-step path resolution. When a module has already been loaded by the program, re-importing it skips re-parsing but still creates the requested bindings (table, alias, or selective).

**Circular imports** are a compile error. If module A loads B and B loads A (or a longer chain closes back on a file still being loaded, the entry file included, or a file loads itself), the compiler reports one error at the `load` that closes the cycle, naming the chain with project-relative paths: `load cycle: a.gem → b.gem → a.gem`. Move the code the modules share into a module that loads none of them. A diamond (A loads B and C, both load D) is not a cycle: D is loaded once and reused.

## Standard Library (std/)

Each std file uses `export` to expose a single namespace table. When loaded, the module table is bound to the basename (e.g., `string`, `table`, `math`).

A std function that takes an option table, a spec or a child spec (`http.start`, `request.get`, `supervisor.start`, `dynamic_supervisor.start`, `regex.find`, ...) raises for a key it doesn't know, naming the closest known one when one is close enough (as `string.closest` finds it): `http.start: unknown option "prot" (did you mean "port"?)`, `supervisor.start: unknown key "restrat" in child "a" (did you mean "restart"?)`. A value it checks against a fixed set (a strategy, a restart type, a log level) names the closest value the same way, when one is close enough.

```
load "std/string"
load "std/table"

let parts = string.split("a,b,c", ",")
let trimmed = string.trim("  hello  ")
table.each(parts) { |item| print(item) }
```

`std/string` — exports `string` table. Strings are bytes: positions are byte offsets. Every argument documented as a string may also be a buffer (read as its contents; results are strings); any other type raises `string.<fn>: <arg> must be a string, got <type>`.

- `string.split(s, delim)` — split `s` at each occurrence of `delim`, scanning left to right, and return the array of pieces. Empty pieces are kept (`split("a,,b,", ",")` is `["a", "", "b", ""]`), `split("", ",")` is `[""]`, and an empty `delim` returns `[s]`.
- `string.index_of(s, needle, start = 0)` — byte offset of the first occurrence of `needle` at or after `start`, or -1. A negative `start` counts as 0; a non-int raises. An empty `needle` is found at `start` (or -1 when `start > len(s)`).
- `string.contains(s, needle)` — true if `needle` occurs in `s` (always true for `""`).
- `string.join(arr, delim)` — join the array's elements with `delim`, adding strings and buffers as their bytes and converting other values as interpolation does (`join([1, nil], "-")` is `"1-nil"`). `arr` must be an array; a record raises `string.join: arr must be an array, ...`.
- `string.trim(s)` — strip leading and trailing ASCII whitespace (space, `\t`, `\n`, `\v`, `\f`, `\r`).
- `string.starts_with(s, prefix)` / `string.ends_with(s, suffix)` — boolean prefix/suffix check; the empty string is a prefix and suffix of everything.
- `string.upper(s)` / `string.lower(s)` — ASCII case conversion; other bytes (UTF-8 included) are unchanged.
- `string.repeat(s, n)` — `s` repeated `n` times (`""` for `n <= 0`). `n` must be an int.
- `string.closest(word, candidates)` — the string of the array `candidates` closest to `word` by edit distance (one byte added, removed or changed, or two neighbouring bytes swapped, per edit) when it is within 1 edit (for a `word` of up to 4 bytes) or 2 (longer); else, for a `word` of at least 3 bytes, the first candidate that is `word` followed by `_` and more (`closest("timeout", ["timeout_ms"])` is `"timeout_ms"`); else nil. Non-string candidates and `word` itself are skipped. For "did you mean" messages: `closest("stratgy", ["strategy"])` is `"strategy"`.

`std/table` — exports `table` table:

- `table.each(arr, fn)` — iterate calling fn(item)
- `table.map(arr, fn)` — return new array with fn(item) applied
- `table.filter(arr, fn)` — return new array where fn(item) is truthy
- `table.reduce(arr, init, fn)` — fold array: fn(accumulator, item) for each element
- `table.find(arr, fn)` — return first element where fn(item) is truthy, or nil
- `table.any(arr, fn)` — return true if fn(item) is truthy for any element
- `table.all(arr, fn)` — return true if fn(item) is truthy for all elements
- `table.reverse(arr)` — return new array with elements in reverse order
- `table.contains(arr, value)` — linear scan for value equality
- `table.sort(arr[, cmp])` — sort array in place and return it. Without `cmp`, uses default ascending order. With `cmp`, uses `cmp(a, b)` returning negative/zero/positive. Wrapper around the `sort` builtin that raises `table.sort: arr must be an array, ...` for a non-array; a comparator returning a non-number raises the builtin's `sort: the comparator must return a number ...`.
- `table.slice(arr, start[, len])` — return new array with `len` elements starting at `start` (fewer at the end of the array, none for `len <= 0`). Negative `start` counts from end; one before the first element counts as `0`. Without `len`, everything from `start` on.
- `table.index_of(arr, val)` — return index of first occurrence of `val`, or -1 if not found
- `table.concat(a, b)` — return new array with elements of `a` followed by elements of `b`
- `table.copy(tbl)` — shallow copy of any table: every key and value, in the same order. Mutations to the copy do not affect the original. An empty table copies to an empty table.
- `table.flat_map(arr, fn)` — map each element with `fn`, then flatten one level. If `fn` returns an array, its elements are inlined (an empty table adds nothing, as in `flatten`); scalars and non-empty records are kept as-is.
- `table.zip(a, b)` — return array of `[a[i], b[i]]` pairs, truncated to the shorter array
- `table.unique(arr)` — return new array with duplicate values removed (first occurrence kept). Two values are duplicates when `==` says so: `1`, `1.0` and `"1"` are all kept, and tables compare by identity.
- `table.count(arr, fn)` — count elements where `fn(item)` is truthy
- `table.flatten(arr)` — flatten one level of nesting. Nested arrays are inlined; non-array elements (non-empty records included) are kept as-is. Empty tables are dropped.
- `table.group_by(arr, fn)` — group elements by the key returned by `fn(item)`. Returns a table mapping keys to arrays of matching elements, in first-seen order. Keys are kept as `fn` returns them (many distinct int keys are slow, as for any table with sparse int keys; prefer strings) (`json.encode` writes int keys that aren't `0 .. n-1` as object keys); a negative int key raises `table.group_by: key ... is a negative int ...`.

`std/math` — exports `math` table:

- `math.min(a, b)` — return the smaller of two values `<` can order (numbers, mixed ints and floats, or strings); on a tie, `b`; other values raise `math.min: expected numbers or strings, got <type> and <type>` (`max` and `clamp` likewise)
- `math.max(a, b)` — return the larger of two such values; on a tie, `b`
- `math.clamp(val, low, high)` — constrain `val` to the range `[low, high]`. Raises `math.clamp: low (...) is greater than high (...)` when `high < low`.
- `math.assert(cond[, msg])` — raise `"assertion failed"` (`"assertion failed: <msg>"` with `msg`) if `cond` is `nil` or `false`.

`std/test` — exports `test` table. Minimal harness for self-validating example programs. Cases register on import; nothing runs until `test.run()` is called.

- `test.case(name, fn)` — register a test case. `fn` takes no arguments. The case list is module state, so it is per process: `test.run()` runs only the cases registered by the process that calls it. Called from any process but the one that loaded `std/test` (the main process), it raises `test.case: called in a spawned process; ...`, since `test.run` in main would never see the case.
- `test.assert(cond[, msg])` — raise `test.assert: assertion failed[: <msg>]` if `cond` is falsy.
- `test.assert_eq(actual, expected[, msg])` — raise unless `actual` deeply equals `expected`. Two tables are equal when they have the same keys and deeply equal values, in any insertion order (so `[]` equals `{}`); the walk uses an explicit stack, so any nesting depth works, and each pair of tables (an expected and an actual table) is compared once: a pair met again, through a shared subtable or a cycle, counts as equal, so cyclic and shared structures compare without looping and in time proportional to the number of distinct pairs times their keys. Two cyclic structures are equal when unrolling them gives the same infinite tree (a ring of 99 tables equals a ring of 3 with the same contents). Other values compare with `==`: `1` and `1.0` differ, functions, buffers and refs compare by identity. The message shows both values and, for tables, the path to the first difference, with the types when they differ: `test.assert_eq: expected {a: [1, 2]}, got {a: [1, 2.0]}: at .a[1], expected 2 (int), got 2.0 (float)`, `test.assert_eq: expected 1 (int), got 1.0 (float)`; a missing or extra key reads `actual is missing key .b` / `actual has extra key .b`. A `msg` follows the prefix: `test.assert_eq: sum: expected 3, got 2`.
- `test.assert_neq(actual, expected[, msg])` — raise `test.assert_neq: both values are <v>` (`test.assert_neq: <msg>: both values are <v>` with a `msg`) if `actual` deeply equals `expected` (the same equality as `assert_eq`).
- `test.assert_throws(fn)` — assert that calling `fn()` raises an error, and return the error message; raises `test.assert_throws: expected an error, but the call returned normally` otherwise. A `fn` that isn't a fn raises `test.assert_throws: body must be a fn, got <type>`.
- `test.run()` — run every registered case under `pcall`, print one-line `PASS <name>` or `FAIL <name>: <reason>` per case, then a `<n> passed / <m> failed` summary, and `exit(1)` if any failed (ending the whole program with status 1, from any process); otherwise it returns `nil`.

The std modules are written in Gem on top of the builtins (`ord`, `chr`, `substr`, `str_replace`, `build_string`, ...). `split` and `index_of` are only available through `std/string` (not as bare builtins).

`std/json` — exports `json` table:

- `json.parse(s)` — parse a JSON string into Gem values. Objects become string-keyed tables, arrays become integer-keyed tables, strings/numbers/booleans map to native types, `null` maps to `nil`. Supports the full JSON spec: nested structures, all string escape sequences (`\"`, `\\`, `\/`, `\b`, `\f`, `\n`, `\r`, `\t`, `\uXXXX`, surrogate pairs decoded to UTF-8; a lone surrogate, high or low, raises `json.parse: lone surrogate in unicode escape at byte N`, N being the offset of its `\u`), negative numbers, floats with decimal points and/or exponents (`1.5e-3`). A number with a decimal point or an exponent is a float; an integer is an int, or a float when it doesn't fit in 64 bits (`12345678901234567890` → `1.2345678901234567e+19`, negative ones too). A number too small for a float reads as the nearest subnormal or `0.0` (`-0.0` when negative: `1e-400` → `0.0`), and one too large for a float (`1e309`, a 400-digit integer) raises `json.parse: number out of range at byte N`, N being where the number starts (JSON has no infinity, and `json.encode` raises for an infinite float). Raises an error starting with `json.parse: ` and ending with the byte offset (`json.parse: unexpected character ']' at byte 6`) on malformed input or a non-string argument — wrap with `pcall` for recovery. Nesting deeper than 1,000 arrays/objects raises `json.parse: nesting deeper than 1000 levels at byte N`; the limit keeps a hostile input well inside the 8 MB process stack, in `main` and in spawned processes alike: without it, the parser and the encoder (also capped at 1,000) would overflow it at two to six times that depth (measured: about 2,000 and 3,000 levels on Linux x86-64, 3,800 and 6,300 on macOS arm64).
- `json.encode(val[, {indent}])` — serialize a Gem value to a JSON string: compact (no extra whitespace), or, with `indent` (an int from 0 to 16, that many spaces, or a string such as `"\t"`), one member or element per line, each nesting level indented one more step, with `": "` after keys; empty tables stay `{}` and `[]`. Any other `indent` raises `json.encode: indent must be ...`. A table whose keys are exactly `0 .. n-1` (in any order) encodes as a JSON array, in index order; every other table encodes as a JSON object in key order (`keys`), with string keys as they are and int keys written as decimal strings (`t[42] = "x"` → `{"42":"x"}`; an array with a string key added → `{"0":"z","name":"n"}`). A table with a key of any other type (float, bool, ...) raises `json.encode: cannot encode a float table key (only string and int keys)`. Strings are escaped (`\"`, `\\`, `\n`, `\r`, `\t`, `\b`, `\f`, other control bytes as `\u00XX`; other bytes are copied as they are), numbers/booleans/nil map to their JSON equivalents; an infinite or NaN float raises `json.encode: cannot encode inf: JSON has no infinite or NaN numbers`. Raises `json.encode: cannot encode a <type>` for functions, buffers and refs, and `json.encode: nesting deeper than 1000 levels (or a cyclic table)` for tables nested deeper than 1,000 levels, which is also how a cyclic table fails. An empty table encodes as `{}` when it was made by a `{...}` literal (`json.parse` makes its objects that way), even if its keys were deleted since, and as `[]` otherwise (`[]`, `keys(t)`, `values(t)` and other tables the runtime makes), so `json.encode(json.parse('{}'))` is `{}`. Only `json.encode` sees the difference: `{}` and `[]` are otherwise the same value.

`std/mime` — exports `mime` table. Both functions take a string (or a buffer) and raise `mime.<fn>: ...` on anything else.

- `mime.lookup(path_or_ext)` — returns the MIME type for a file path or extension. Accepts `"index.html"`, `".html"`, or `"html"`; only the last path segment counts, so `"static/html"` and `"v1.2/notes"` have no extension. Returns `"application/octet-stream"` for an unknown or missing extension. Known: html htm css js mjs json xml txt md csv yaml yml toml map ics rss atom png jpg jpeg gif svg ico webp avif bmp tif tiff woff woff2 ttf otf pdf zip gz tar mp3 wav ogg oga flac m4a mp4 ogv mov webm wasm (JavaScript is `text/javascript`, RFC 9239; a source map is `application/json`). Text types (`text/*`, `application/json`, `application/xml`, `application/yaml`, `application/toml`) automatically include `; charset=utf-8`. Case-insensitive.
- `mime.ext(content_type)` — reverse lookup: `"text/html"` → `".html"`; the first extension listed for a type (`.jpg`, `.html`, `.yaml`), and older names of a type too (`application/javascript` → `.js`, `text/yaml` → `.yaml`, `audio/x-wav` → `.wav`). Ignores case, surrounding spaces and parameters (`; charset=...`). Returns `nil` for unknown types.

`std/url` — exports `url` table. Every string argument may also be a buffer; any other type raises `url.<fn>: ...`.

- `url.encode(str)` — percent-encode per RFC 3986. Unreserved bytes (`A-Za-z0-9-_.~`) pass through, every other byte becomes `%XX` (uppercase hex), so UTF-8 text is encoded byte by byte.
- `url.decode(str)` — percent-decode `%XX` sequences (either hex case); `+` is decoded as space. A `%` not followed by two hex digits is kept as written (`"100%"` stays `"100%"`).
- `url.parse_query(str)` — parse a query string like `"a=1&b=hello+world&c=%2F"` into a table `{a: "1", b: "hello world", c: "/"}`. Decodes both keys and values, splitting each pair at its first `=`. A key without `=` gets `""`, pairs with an empty key are dropped, and for duplicate keys the last value wins.
- `url.build_query(table)` — build a query string from a table, in key order. Keys and values are converted with `to_string` and encoded; keys whose value is `nil` are left out.
- `url.parse(target)` — split a request target on the first `?`. Returns `{path: "/users/123", query_string: "q=foo&page=2", query: {q: "foo", page: "2"}}`. If no `?`, `query_string` is `""` and `query` is `{}`. A `#fragment` is dropped from the end first. The path is returned as written (not decoded).

`std/io` — exports `io` table: a file, or stdin, a line at a time. A path `"-"` is stdin, read with `input()` (which blocks every process while it waits); a regular file is read in 64 KB blocks with `read_file(path, offset, count)` on the thread pool, so memory stays flat whatever the file's size and nothing stays open between blocks; a pipe, FIFO or device (`/dev/stdin`, a `<(cmd)` path) can't be reopened at an offset, so it is read whole with `read_file(path)` before its first line (so is a block device, which `stat` reports as `"other"`). Lines come without their `\n` or `\r\n`; a last line without a newline comes as it is; an empty file has no lines. A path that can't be read raises `read_file`'s error at the first block; a path that isn't a string raises `io.<fn>: path must be a string, got <type>`.

- `io.lines(path, f)` — call `f(line)` for each line, in order; returns nil.
- `io.reader(path)` — a reader `r`: `r.next()` returns the next line, or nil at the end (and after it). It holds no open file, so a reader dropped unfinished leaks nothing.
- `io.read_lines(path)` — every line, as an array (the whole file in memory).

`std/args` — exports `args` table: command-line options. A spec is `{name, about, usage, options, min_args, max_args}` (`name` defaults to the file name of `argv()[0]`; `usage` to `[options] [args...]`, `[options]` when `max_args` is 0); `options` maps each option's name to `{short, help, default, type, multi, choices, required, value}`. `type` is `"string"`, `"int"`, `"float"` or `"flag"`, by default the type of `default` (an int or a float; `"flag"` for `true` or `false`), else `"string"`; a flag's `default` must be `true` or `false`. The result holds each option under its name: the value given (for a `multi` option, which collects every occurrence, an array of them); else its `default` (for a `multi` option, a table `default` as it is and any other in a one-element array); else `false` for a flag, `[]` for a `multi` option, or nil and the positional arguments in `args` (so no option may be named `args`). On the command line, an option `dry_run` is `--dry-run` or `--dry_run`; a value comes as `--top 5`, `--top=5`, `-n 5` or `-n5`; flags combine (`-vq`) and a flag given a value (`--verbose=1`) is a mistake; `--` ends the options; `-` alone is an argument. `-h` and `--help` ask for the help unless the spec defines them. A spec with an unknown key, an unknown `type` or a `short` of more than one letter raises `args: ...` (with the closest known key for an unknown one, when one is close enough).

- `args.parse(spec[, list])` — parse `list` (default: `argv()` without its first element) and return the result. `--help` prints the help to stdout and exits 0. A mistake (an unknown option, named with the closest long option when one is close enough, and with no hint for an unknown short option; a missing or ill-typed value; a value not in `choices`; a missing `required` option; too few or too many arguments) prints `<name>: <message>`, the usage line and `(--help for more)` on stderr and exits 2.
- `args.try_parse(spec[, list])` — the same, but returns `{ok: true, value: result}` or `{ok: false, error: message}` and never prints or exits; `--help` sets `result.help` to the help text.
- `args.help(spec)` — the help text: the usage line, `about`, and one line per option with its `help`, `choices` and default.

`std/regex` — exports `regex` table: regular expressions. Patterns are POSIX extended regexes (`|`, `+`, `?`, `{m,n}`, `{m,}`, `{,n}`, groups, bracket expressions with `[:class:]`s) plus `\d`, `\w`, `\s`, their negations `\D`, `\W`, `\S` (outside brackets; `\d`, `\w`, `\s` also inside them) and `\t`, `\n`, `\r`. Outside brackets, a backslash before any other character that isn't a letter or digit stands for that character (`\.`, `\{`, `\'`), except the word boundaries `\<` and `\>` below, and a backslash before any other letter or digit raises (`unknown escape: ...`). Inside a bracket expression a backslash before punctuation stands for that character, wherever it is (`[\]\-^]`, `[a\-z]` is `a`, `-`, `z`); a backslash before another letter or digit there raises, and so does an escaped `]`, `[`, `^` or `-` at either end of a range, or a range first in the brackets starting with one of those (`[--/]`); unescaped, they can end a range (`[!-^]`). An empty alternative (`a|`, `(|b)`, an empty pattern) matches the empty string. The runtime rewrites each pattern before handing it to the C library, into forms glibc and macOS read alike; what they read differently raises instead: word boundaries (`\b`, `\B`, `\<`, `\>`), back-references (`\1` .. `\9`), a quantifier after another (`a**`; there are no lazy quantifiers such as `*?`) or with nothing before it, a `{` that doesn't start a bound (write `\{`), a bound over 255. There are no lookarounds. A pattern that doesn't compile raises `regex: bad pattern '<pattern>': <why>`. A pattern stops at a NUL byte; the string searched may hold them. In a double-quoted string literal a bound is interpolation (`"\\d{4}"` is the pattern `\d4`): write such a pattern in single quotes (`'\d{4}'`) or escape the braces (`"\\d\{4\}"`). Every function but `escape` takes an option bag `{icase, multiline}` (`multiline`: `^` and `$` also match at line breaks, and `.` and `[^...]` don't match `\n`). Positions are byte offsets. A match is `{text, start, stop, groups}` (`stop` is the offset after it; `groups[i]` is group `i + 1`'s text, nil when it took no part). Compiled patterns are kept in a cache of 64 (keyed by pattern and options; when it is full, the one compiled longest ago makes room). Matching runs on the scheduler thread, like a plain `extern fn`. A `pattern` or `s` that isn't a string raises `regex.<fn>: <arg> must be a string, got <type>`.

- `regex.test(pattern, s, opts)` — true when `pattern` matches somewhere in `s`.
- `regex.find(pattern, s, {icase, multiline, start = 0})` — the first match at or after byte `start`, or nil. `^` matches at `start` only when it is 0 (or follows a newline in `multiline` mode).
- `regex.find_all(pattern, s, opts)` — every match, left to right, not overlapping; after an empty match the search moves on one byte (`find_all("x*", "axxb")` gives 4 matches: "", "xx", "", "").
- `regex.replace(pattern, s, repl, opts)` — `s` with every match replaced: `repl` is a string, where `\0` is the match, `\1`..`\9` its groups (an unmatched one gives `""`) and `\\` a backslash, or a fn called with each match, whose result is added as `to_string` would. `regex.replace_first` replaces the first match only.
- `regex.split(pattern, s, opts)` — the pieces between matches; matches at either end or next to each other give empty pieces, and empty matches don't split (`split("x", "")` is `[""]`).
- `regex.escape(s)` — `s` with every metacharacter escaped, a pattern matching `s` literally.
- `regex.check_pattern(pattern, opts)` — nil when `pattern` compiles, else the message the others would raise.

`std/base64` — exports `base64` table (RFC 4648). A non-string argument raises `base64.<fn>: s must be a string, got <type>`.

- `base64.encode(s)` — `s` in standard base64 (`+`, `/`, `=` padding).
- `base64.encode_url(s)` — `s` in the URL-safe alphabet (`-`, `_`), unpadded.
- `base64.decode(s)` — the bytes `s` encodes, in either alphabet, padded or not, skipping spaces, tabs and line breaks; nil for anything that isn't base64 (another character, a lone final character, data after the padding, nonzero leftover bits).

`std/hex` — exports `hex` table.

- `hex.encode(s)` — each byte of `s` as two lowercase hex digits.
- `hex.decode(s)` — the bytes hex text `s` encodes (either case), or nil for an odd length or a non-hex character.

`std/crypto` — exports `crypto` table. SHA-256 (FIPS 180-4) and HMAC-SHA256 (RFC 2104), written in C in the runtime (no OpenSSL needed). The raw forms return 32 bytes, the `_hex` forms 64 lowercase hex digits. A non-string argument raises `crypto.<fn>: <arg> must be a string, got <type>`. Runs inline.

- `crypto.sha256(s)` / `crypto.sha256_hex(s)` — the digest of `s`.
- `crypto.hmac_sha256(key, msg)` / `crypto.hmac_sha256_hex(key, msg)` — the HMAC of `msg` under `key` (a key over 64 bytes is hashed first).
- `crypto.equal(a, b)` — true when `a` and `b` hold the same bytes, in a time that depends only on their lengths (for checking MACs and tokens).

`std/utf8` — exports `utf8` table: text by characters (code points) instead of bytes. A byte that isn't part of a valid UTF-8 sequence (a stray continuation byte, a truncated or overlong sequence, a surrogate, past U+10FFFF) counts as one character: `codepoints` gives it as 65533, `chars` as its one byte, `upper`/`lower` keep it. A non-string `s` raises `utf8.<fn>: s must be a string, got <type>`.

- `utf8.len(s)` — the number of characters.
- `utf8.valid(s)` — true when `s` is valid UTF-8.
- `utf8.chars(s)` / `utf8.codepoints(s)` — the characters as strings, or as ints.
- `utf8.char(cp)` — the UTF-8 string of a code point, or of an array of them; raises `utf8.char: <cp> is not a Unicode scalar value` for a negative one, a surrogate or one past 1114111.
- `utf8.sub(s, start[, count])` — `count` characters from character `start` (to the end without `count`); a negative `start` counts from the end; `""` past the end.
- `utf8.upper(s)` / `utf8.lower(s)` — simple one-to-one case mapping for ASCII, Latin-1, Latin Extended-A, Greek, Cyrillic, Armenian and fullwidth Latin letters; mappings that change the number of characters (`ß` to `SS`) and other scripts are left as they are.
- `utf8.reverse(s)` — `s` reversed by characters.

`std/csv` — exports `csv` table (RFC 4180). Options: `sep` (one byte, not a quote or line break; default `","`).

- `csv.parse(text, {sep, header = false})` — the rows of `text`, each an array of field strings (never converted). A field in double quotes may hold the separator, `""` for a quote, and line breaks. Rows end at `\n` or `\r\n`; a last row needs none; an empty line is a row of one empty field; `""` has no rows. With `header`, the first row names the fields and the result is one record per later row (empty lines skipped; a shorter row leaves the rest out, a longer one raises). Malformed input raises `csv.parse: line <n>: <what>`: a quote inside an unquoted field and text after a closing quote at the line they are on, a quoted field never closed at the line it starts on, and (with `header`) a row longer than the header at the line the row starts on.
- `csv.encode(rows, {sep, columns = nil})` — CSV text, one `\n`-ended line per row. Rows are arrays; with `columns`, records, written as those fields after a header line.
- `csv.encode_row(row, {sep})` — one line, no line break: nil as an empty field, other values as `to_string` gives them, quoted when they hold the separator, a quote, a line break, or a space at either end.

`std/time` — exports `time` table:

- `time.now()` — returns the current wall-clock time as milliseconds since the Unix epoch (int). Alias for `epoch_ms()`.
- `time.format(ms, fmt)` — format epoch milliseconds as a UTC string using `strftime` specifiers. Wraps `format_time` (see there for negative times and the output limits); an `ms` that isn't an int or a `fmt` that isn't a string raises `time.format: ms must be an int, got <type>` (`fmt must be a string`), and the other `time` functions check the same way under their own names.
- `time.format_local(ms, fmt)` — format epoch milliseconds as a local timezone string. Wraps `format_time_local`. Both `format`s also take `%L`, the milliseconds as three digits (`"%H:%M:%S.%L"` → `"14:30:00.123"`; `%%L` is a literal `%L`).
- `time.http_date(ms?)` — RFC 7231 format: `"Tue, 28 Apr 2026 14:30:00 GMT"`. Uses current time if `ms` is omitted.
- `time.iso8601(ms?)` — ISO 8601 format: `"2026-04-28T14:30:00Z"`. Uses current time if `ms` is omitted.
- `time.date(ms?)` — calendar date: `"2026-04-28"`. Uses current time if `ms` is omitted.
- `time.parse(s)` — epoch milliseconds of an ISO 8601 date or an HTTP date, or nil when `s` is neither or names a day or time that doesn't exist (`2026-02-30`, `24:00`). ISO 8601: `YYYY-MM-DD` (midnight UTC), optionally `T` (or a space) and `HH:MM`, `:SS`, a fraction (to the millisecond; further digits are dropped), then `Z`, an offset (`±HH:MM`, `±HHMM`, `±HH`), or nothing for UTC. HTTP (RFC 9110): `Sun, 06 Nov 1994 08:49:37 GMT`, `Sunday, 06-Nov-94 08:49:37 GMT` (a two-digit year is 1970–2069) and `Sun Nov  6 08:49:37 1994`. Surrounding spaces are ignored; a non-string raises `time.parse: s must be a string, got <type>`.

`std/log` — exports `log` table. Structured logging to stderr. Depends on `std/time`.

- `log.debug(msg)`, `log.info(msg)`, `log.warn(msg)`, `log.error(msg)` — log at the given level. Output format: `2026-04-28T14:30:00Z [INFO] message`. One line per call, written to stderr via `eprint`.
- `log.set_level(level)` — set minimum log level. One of `"debug"`, `"info"`, `"warn"`, `"error"`; anything else raises `log.set_level: unknown level ...`. Default: `"info"`. Messages below the level are silently dropped. The level is module state, so it is per process: set it before spawning (or before `http.start`), or call it in the process that logs.

`std/sqlite` — exports `sqlite` table. Thin wrapper over the SQLite C builtins. Its errors name the wrapper (`sqlite.query: UNIQUE constraint failed: t.name`), with the builtin's own name dropped.

- `sqlite.open(path)` — opens (or creates) a SQLite database. Returns an opaque db handle. Use `":memory:"` for in-memory.
- `sqlite.close(db)` — closes the database handle.
- `sqlite.exec(db, sql)` — executes SQL that takes no parameters and returns no rows (DDL, PRAGMA, mutations without RETURNING).
- `sqlite.query(db, sql, params?)` — executes a parameterized query. `params` defaults to `[]`. Returns an array of row tables. Use `?` placeholders for bind parameters.
- `sqlite.last_id(db)` — returns the last inserted row ID.
- `sqlite.changes(db)` — returns the number of rows affected by the last mutation.

`std/http` — exports `http` table. HTTP/1.1 server with routing and keep-alive. Depends on `std/string`, `std/url`, `std/mime`, `std/json`, `std/time`.

### Response builders

- `http.response(status, headers, body)` — generic response constructor. Returns `{status, headers, body}`.
- A response is valid when `status` is an int from 200 to 999 (1xx interim responses are not supported), `body` is `nil` or a string, and `headers` is `nil` or a table of header name → value. A value is a string, number or bool, sent as `"{value}"`, or an array of them, sent as one header line per element (`set_cookie` uses this for several cookies); a `nil` value or another table makes the response invalid. A header name must be an HTTP token (letters, digits and ``!#$%&'*+-.^_`|~``: no spaces, `:` or control bytes, not empty), and a value containing CR or LF makes the response invalid (it could inject headers). The server writes `Content-Length` and `Connection` itself and drops a handler's own `Content-Length`, `Transfer-Encoding` and `Connection` headers (in any case); a handler's `Connection` header that lists `close` still closes the connection after the response.
- `http.ok(body[, content_type])` — 200 response. Default content type: `"text/plain; charset=utf-8"`.
- `http.html(body)` — 200 with `text/html; charset=utf-8`.
- `http.json_response(data[, status])` — `application/json; charset=utf-8` response. Auto-encodes `data` via `json.encode`. Default status: 200.
- `http.redirect(url[, status])` — redirect response. Default status: 302.
- `http.not_found([body])` — 404. Default body: `"Not Found"`.
- `http.bad_request([body])` — 400. Default body: `"Bad Request"`.
- `http.server_error([body])` — 500. Default body: `"Internal Server Error"`.

### Router

- `http.router()` — returns a router table with methods:
  - `router.get(pattern, handler)`, `router.post(...)`, `router.put(...)`, `router.patch(...)`, `router.delete(...)` — register a route. Handler signature: `fn(req) → response table`.
  - `router.static(url_prefix, dir_path)` — serve static files. Uses `std/mime` for content types. Prevents path traversal by normalizing paths and checking that they don't escape `dir_path`. The prefix matches whole path segments: `router.static("/static", dir)` serves `/static` and `/static/...` (`/static` and `/static/` give `index.html`), not `/staticx` or `/static.txt`. The rest of the path is percent-decoded (`%XX`; `+` stays `+`) before the lookup, so `/static/my%20file.txt` serves `my file.txt`.
- Route patterns: segments starting with `:` are parameters. `/users/:id` matches `/users/123` and sets `req.params.id = "123"`. First registered route wins.
- Handler receives a request table: `{method, path, params, query, headers, body, cookies, peer}`. `peer` is the client's address, `{ip, port}`, as `tcp_peer` gives it for the connection (behind a reverse proxy, the proxy's). `params` are extracted from route pattern, `query` is parsed from query string via `url.parse_query`, `cookies` are parsed from the `Cookie` header (pairs separated by `;`, names and values trimmed).
- `req.headers` keys are the header names lowercased (`req.headers["content-type"]`, whatever case the client sent), and values have surrounding whitespace trimmed (`Host:x` and `Host:  x` both give `"x"`). A header sent more than once has its values joined with `", "` (`Cookie` with `"; "`).

### Cookies

- `http.set_cookie(resp, name, value[, opts])` — returns a new response with a `Set-Cookie` header (`resp.headers` may be `nil`). The first cookie makes the header value a string; each further one turns it into an array of strings, sent as one `Set-Cookie` line each. `opts` table supports: `path` (default `"/"`), `http_only` (default `true`), `secure` (default `false`), `same_site` (default `"Lax"`), `max_age` (seconds, an int), `expires` (epoch ms). An option given as `nil` leaves its attribute out (`{path: nil}` sends no `Path`). A `nil` or table `value` raises `http.set_cookie: cookie value must be a string or a number, got <type>`.
- The name, value, `path` and `same_site` must not contain `;`, `,`, whitespace or control bytes (any of them could add an attribute, such as `Domain=`, or inject a header), and the name must not be empty or contain `=`; otherwise `set_cookie` raises `http.set_cookie: invalid cookie value "..." (no ;, comma, whitespace or control bytes)` (`name`, `path`, `same_site` likewise; `http.set_cookie: max_age must be an int or nil, got ...`; `http.set_cookie: expires must be an int (epoch ms), got <type>`). In a handler the error gets the usual 500 and report. Encode arbitrary data first (`url.encode(v)`).
- `http.delete_cookie(resp, name[, opts])` — sets cookie with `Max-Age=0`; checks `name` and `path` the same way (errors start with `http.delete_cookie: `).

### Form parsing

- `http.parse_form(body)` — parses a URL-encoded form body (`key=value&key=value`) into a table. Convenience wrapper around `url.parse_query(body)` — form-encoded bodies use the same percent-encoded format as query strings.

### Security

- `http.html_escape(str)` — escapes `&`, `<`, `>`, `"`, `'` to HTML entities. Use when interpolating user data into HTML.

### Server

- `http.start(router[, opts])` — starts the HTTP server without blocking and returns `{pid, port}`: the pid of the server process and the port it listens on. `opts` table: `port` (default 8080; an int from 0 to 65535, `0` binding a free port that the returned `port` names; else `start` raises `http.start: port must be an int from 0 to 65535, got <value>`), `host` (default `"0.0.0.0"`, an IP address; a string, else `http.start: host must be a string (an IP address), got <type>`), `max_body` (bytes, default 8388608 = 8 MB; an int `>= 0`, else `start` raises `http.start: max_body must be an int >= 0, got ...`), `idle_timeout_ms` (default 30000), `request_timeout_ms` (default 30000), `write_timeout_ms` (default 30000); a timeout given as `nil` means no limit, and otherwise must be an int `> 0`, else `start` raises `http.start: <name> must be an int > 0 (milliseconds) or nil, got <value or type>`. `max_connections` and `max_per_ip` (default `nil`: no limit; else an int `>= 1`, or `http.start: <name> must be an int >= 1, got <value>`) cap the open connections in all and per client IP (see Connection caps). `on_event` (default `nil`; else a fn, or `http.start: on_event must be a fn or nil, got <type>`) is called with each event (see Events). An `opts` that isn't a table raises `http.start: opts must be a table, got <type>`. A first argument that isn't a router (a table with a `match` function, as `http.router()` returns) raises `http.start: expected a router from http.router(), got <type>`; a port that can't be bound or a bad host raises `http.start: ` followed by the `tcp_listen` error.
- `http.stop(server[, timeout_ms])` — stops a server: kills its process with `"shutdown"` and waits (default 5000 ms, then raises `http.stop: the server did not stop within <ms> ms`) until the acceptor and the connection processes are killed and the listening socket and every connection's socket are closed, so the port can be bound again at once. A `timeout_ms` of `nil` waits until the server is down; one that is neither an int nor `nil` raises `http.stop: timeout_ms must be an int (milliseconds) or nil, got <type>`. This includes a connection accepted just before the stop whose process hasn't run yet: the acceptor registers each connection with the server right after spawning it. In-flight requests are cut off, and so is the request of a handler that calls `http.stop` on its own server (its connection process is killed with the rest; `stop` doesn't return to it). A handler that traps exits isn't killed: it finishes its current request, answers it with `Connection: close` and closes its connection, which may be after `stop` has returned (unless the handler takes the `EXIT` message itself); a connection process turns `trap_exit` off again after each handler, so one that trapped in an earlier request is killed as usual. `server` is the `{pid, port}` handle from `start`, the server pid, or a name it was registered under (an unregistered name raises `http.stop: no process registered as "<name>"`). Returns `nil`; stopping a server that has already stopped returns `nil` too.
- `http.serve(router[, opts])` — `start`, then block the caller until the server process exits (its argument errors are `start`'s, starting with `http.serve: `). Other messages sent to the caller stay in its mailbox. Returns `nil` when the server was stopped (`http.stop`, or killed with reason `"shutdown"`), and raises `http.serve: server died: <reason>` for any other reason. A `"normal"` exit signal is ignored, as by every process that traps exits, so `kill(server, "normal")` doesn't stop it. The sockets are closed before it returns or raises. The server monitors the caller of `serve`: when the caller is killed, the server shuts down and frees the port.
- Processes: the server process opens the listening socket itself, spawns the acceptor (linked) and tracks one process per connection (the acceptor registers each one with it; a connection that finishes tells it, and one that dies otherwise is seen through a monitor). It traps exits, so killing it with any reason but `"normal"` and `"kill"` (as `stop` and a supervisor do) shuts the server down cleanly, then the process exits with that reason; a supervisor can therefore restart an http server on the same port (its child spec's `start` returns `http.start`'s handle). The untrappable `kill(server, "kill")` ends the server process at once, without that shutdown: the listening socket closes with it (the server claimed it), so the port can be bound again, but open connections keep serving until they end. A supervisor sends `"kill"` only after the child's `shutdown` budget (5000 ms by default; the server's own shutdown doesn't wait on anything, so it takes far less). Each connection process claims its socket, so the socket closes however the process ends, and the client sees the connection end instead of hanging; the server also closes the socket of a connection that dies, which matters for one killed during its own `extern blocking fn` (its sockets otherwise stay open until the C call returns, see Owned Resources).
- Connection caps: with `max_connections` or `max_per_ip`, the acceptor monitors each connection process and counts the live ones, in all and per `peer.ip`. A connection that would go over a cap gets a `503 Service Unavailable` and is closed at once: the acceptor writes what the socket takes without waiting, reads what the client has sent already without waiting, and closes, so a client that sent more than that may see a reset instead of the 503. Without either option the acceptor monitors nothing.
- Events: `on_event(ev)` is called with a table that has `kind` and `peer` (`{ip, port}`). What it holds of the request (`method`, `path`, `headers`, `raw`) comes from the client unchecked, of any length up to the server's own limits. Kinds:
  - `"open"`, when a connection process starts, before it reads anything. Every connection that gets one gets a `"close"` as it ends, unless its process is killed.
  - `"request"`, after each response is written (or cut), the server's own `400`, `404`, `413`, `431`, `500`, `501` and `505` included: `status`, `ms` (from the request's first byte to the response written), `bytes_out` (the response's size); `method`, `path` and `headers` (as in the request table) once the head was parsed; `error` (the message) when the handler raised or returned an invalid response; `raw`, the first 4096 bytes of the request head (for a head over 64 KB or ending in bare LFs, of what was read), when the server rejected the request itself (`400`, `413`, `431`, `501`, `505`, or the `500` after a `"crash"`).
  - `"close"`, when a connection process ends its connection: `reason`, `requests` (responses sent), `bytes_in` (bytes read, those drained after an error response included) and `ms` (how long the connection was open). `reason` is `"closed"` (the client closed or reset it), `"idle"` (no byte of a next request within `idle_timeout_ms`), `"timeout"` (a request's head or body not read within `idle_timeout_ms` per read or `request_timeout_ms` in all), `"done"` (a response closed it: the request's or the handler's `Connection: close`, HTTP/1.0 without keep-alive, or a server that is stopping), `"rejected"` (after the server rejected a request with `400`, `413`, `431`, `501` or `505`), `"write_failed"` (a response or `100 Continue` cut by `write_timeout_ms`, or a client that has gone) or `"error"` (after a `"crash"`). A connection process killed (by `stop`, or a handler's `kill(self(), ...)`) reports nothing.
  - `"refused"`, for a connection the acceptor closes: `reason` is `"max_connections"`, `"max_per_ip"`, or `"spawn_failed"` with `error` (`spawn` raised: the process table or the system's memory is full, see `spawn`; the client gets the same 503). A client that resets before its address can be read is closed without an event.
  - `"crash"`, for an error raised in std/http's own code serving a connection (a bug, a socket error other than a reset, such as `tcp_read: read failed: ...`, or a router whose `match` raises; a handler's error is a `"request"` with `error`): `error`, `stack` (as from `pcall`) and `raw` (the first 4096 bytes of the request). The client gets a `500`, and `"request"` and `"close"` (reason `"error"`) follow.

  `on_event` runs in the connection's process, and `"refused"` in the acceptor's, which accepts nothing meanwhile: keep it quick (a `send` to a process that records the events). An error it raises is printed as `http: on_event: <message>` on stderr and changes nothing else.
- When the process table is full, a new connection gets a `503 Service Unavailable` and is closed, a line goes to stderr, and the acceptor keeps accepting. When accepting fails (for example, the process is out of file descriptors), the acceptor prints one line on stderr and retries every 50 ms; meanwhile new connections wait in the listen backlog (no 503 can be sent without a descriptor).
- Keep-alive: an HTTP/1.1 connection stays open unless the request's `Connection` header lists `close`; an HTTP/1.0 connection stays open only when it lists `keep-alive` (and not `close`). `Connection` is a comma-separated, case-insensitive token list (`Connection: close, TE` closes). Pipelined requests are answered in order. Responses are always `HTTP/1.1`.
- Timeouts: the server waits at most `idle_timeout_ms` for each read, including for the first byte of the next request on a kept-alive connection, and the head and body of one request must arrive within `request_timeout_ms` of its first byte; past either, the connection is closed without a response. `nil` turns a limit off. `write_timeout_ms` bounds how long a client may go without taking any bytes of a response (or a `100 Continue`): a slow client that keeps reading gets the whole response, however long it takes, while one that takes no bytes for a whole `write_timeout_ms` (so between `write_timeout_ms` and twice that after the last byte it took) has its connection closed with the response cut short (and a line `http: closing a connection: the client took no bytes of a <n>-byte response for <ms> ms` on stderr), so a client that stops reading can't hold its connection process. A client that has gone is closed the same way, without the line. The `503` the acceptor sends when the process table is full doesn't wait at all: it writes what the socket takes at once.
- Request bodies: read by `Content-Length` (decimal digits only) or, for `Transfer-Encoding: chunked`, decoded (chunk extensions and trailer fields are discarded; a chunk size may have leading zeros, up to 64 characters with at most 15 significant hex digits). A body over `max_body` gets `413 Content Too Large`. A request with both `Transfer-Encoding` and `Content-Length`, an HTTP/1.0 request with `Transfer-Encoding`, a malformed `Content-Length`, malformed chunk framing (a bad size line, chunk data not followed by CRLF, a bare CR or LF or another control byte other than HTAB in a size or trailer line) gets `400 Bad Request`; any transfer coding other than plain `chunked` gets `501 Not Implemented`. Each of these closes the connection, so a body is never read as the next request. With `Expect: 100-continue` (HTTP/1.1), the server sends `HTTP/1.1 100 Continue` before it reads the body, unless it rejects the request first.
- `HEAD` requests are dispatched to the `GET` route (`req.method` stays `"HEAD"`) and answered with the headers only: the `Content-Length` of the body the handler returned, and no body. No response to `HEAD` carries a body, the default 404 included. 204 and 304 responses carry neither a body nor `Content-Length`.
- A handler that raises, or returns something other than a valid response (see Response builders), gets the default `server_error()` response, and the server prints `http: <METHOD> <path>: <message>` on stderr, followed by the handler's stack trace for an error (the server's own frames, and the std frames on top of the handler's, such as those of a builder that raised and of the std functions it called, are left out; the message says what is wrong with an invalid response, e.g. `invalid response: header Location has a CR or LF in its value`). An unmatched route gets the default `not_found()` response, and so does a static route whose path names a directory. A malformed request line (it must be `<method> <target> HTTP/1.0` or `HTTP/1.1`) or header line (no `:`, or whitespace in the name), and a request head with a bare CR, a bare LF or another control byte other than HTAB anywhere in it (RFC 9112 §2.2: another parser could split such a head into different header lines, which smuggles requests), get `bad_request()`; another `HTTP/<digit>.<digit>` version gets `505 HTTP Version Not Supported`; a request head over 64 KB gets `431 Request Header Fields Too Large`; each closes the connection. Empty lines (CRLFs) before a request line are ignored. An absolute-form target (`GET http://host/path?q HTTP/1.1`, as sent to a proxy; scheme `http` or `https`, any case) is routed by its path and query, the host being ignored. Before closing after an error response, the server reads and discards what the client is still sending (until it pauses for 100 ms, at most 1 s), so the client receives the response instead of a connection reset.

`std/request` — exports `request` table. HTTP/1.1 client for outbound requests, over TLS for `https://`. Depends on `std/string`, the TCP builtins and `tls_connect`.

- `request.get(url[, opts])`, `request.post(url[, opts])`, `request.put(url[, opts])`, `request.patch(url[, opts])`, `request.delete(url[, opts])` — send that method. Return `{status, headers, body}`.
- `request.request(method, url[, opts])` — any other method (`"HEAD"`, `"OPTIONS"`, ...). A `HEAD` response's body is `""`.
- `opts` (all optional; a key left out gets the default, and a key holding `nil` is `nil`: no limit for `timeout_ms`, an error for `body` and `headers`): `body` (string, default `""`; a non-empty body gets a `Content-Length` header, and so does an empty one for `POST`, `PUT` and `PATCH`: `Content-Length: 0`, which RFC 9110 asks for and some servers answer `411 Length Required` without), `headers` (table of request header name → value, sent as given; a name must be an HTTP token, else `request.<fn>: invalid header name "<name>" (must be an HTTP token: ...)`, and a `nil` value raises `request.<fn>: header <name> has a nil value`), `timeout_ms` (an int `> 0`, default `30000`, or `nil` for no deadline; anything else raises `request.<fn>: timeout_ms must be an int > 0 (milliseconds) or nil, got ...`), `ca_file` (for `https://`, a PEM file of the certificates to trust in place of the system's, see `tls_connect`; `nil` by default; anything but a string or `nil` raises `request.<fn>: ca_file must be a string or nil, got <type>`). `timeout_ms` is one deadline for the whole call, counted from before the connect until the response has been read: the connect and the TLS handshake wait only for what is left of it, and so does each read; when it runs out the call raises `"request.<fn>: timed out after <ms> ms waiting for <url>"`. Writing the request counts against it too (a request not sent in time raises the same timeout error). Resolving a host name counts against it too.
- `status` is the int status code; the reason phrase is optional (`HTTP/1.1 204` works) and not returned. `headers` maps **lowercase** header names to values with surrounding whitespace trimmed (`resp.headers["content-type"]`); a header sent more than once has its values joined with `", "`, except `set-cookie`, whose values are joined with `"\n"` (a cookie's `Expires` contains a comma). `body` is the decoded body: a `Transfer-Encoding` whose last coding is `chunked` is decoded (chunk extensions and trailer fields dropped, so a trailer never overrides a header; the `transfer-encoding` header stays), `Content-Length` is read exactly, and anything else is read until the server closes. Other transfer codings (gzip) are not decoded. Interim `1xx` responses (other than `101`) are skipped.
- URL format: `http://host[:port][/path][?query][#fragment]`, or the same with `https://`. Default port 80 (443 for `https://`); the fragment is not sent; a path that is missing becomes `/`. An `https://` request is sent over `tls_connect`, which verifies the server's certificate and that it names the URL's host; there is no option to skip that. Other schemes (`request.<fn>: unsupported URL scheme "<scheme>" (only http:// and https:// are supported)`), a URL without a scheme, a missing host, a bad port, credentials (`user:pass@`), IPv6 literals, and spaces or control bytes in the URL raise a `request.<fn>: ` error too. Control bytes (CR, LF) in a method, header name or header value also raise (`request.<fn>: invalid character (byte <n>) in ...`), so a value can't inject a header.
- Sends `Host` (with the port when it isn't the scheme's default), `Connection: close` and `Content-Length` unless `headers` sets that name (in any case). A `Transfer-Encoding` header in `headers` raises `request.<fn>: a Transfer-Encoding header is not supported (the body is sent with Content-Length)`: the body is always sent whole. No keep-alive, no redirects followed (a `302` comes back as is).
- Every error is a string prefixed with the function called: `request.get: ...`, `request.request: ...`. A connect failure is `"request.<fn>: cannot connect to <host>:<port>: <tcp_connect or tls_connect error>"` (a certificate that fails verification included); a malformed response (bad status line, header line, `Content-Length` or chunk framing) or a connection closed early raises one. The socket is closed whether the call returns or raises; a process killed in the middle of a call (a `task.await` timeout, a link) closes it as it dies, since the socket is the caller's (see Owned Resources); still, bound requests with `timeout_ms` rather than by killing the caller. A response line (status line, header line, chunk-size line) over 64 KB raises `request.<fn>: response line longer than 65536 bytes`.

`std/task` — exports `task` table. Runs a function in its own process and waits for its result; the simplest way to do several things at once. Load with `load "std/task"`.

- `task.async(f)` — spawns a process that calls `f()` and returns a task handle `{pid, ref, owner, done}`. Raises `task.async: expected a function, ...` when `f` is not a function. `await` and `await_all` raise `task.await: expected a task from task.async, ...` (or `task.await_all: ...`) for anything but such a handle.
- `task.await(t)` / `task.await(t, timeout_ms)` — waits for the task and returns `f`'s value. If `f` raised an error, `await` raises the same message in the caller. On timeout, the task is killed (`kill(pid, "timeout")`, so a monitor of it sees reason `"timeout"`; the tasks `await_all` drops when it fails or times out get `"cancelled"`; a task that traps exits is not, see the rules below) and `await` raises `"task.await: timeout"`. If the task process ends before it produces a result (killed, or `kill(self(), reason)` in `f`), `await` raises `"task.await: task exited: <reason>"`; for reason `"normal"` the message instead says the task exited before its result was received and that a catch-all receive may have taken the result. With no timeout (`nil`, the default), `await` waits until the task finishes; a timeout that is neither an int nor `nil` raises `task.await: timeout must be an int (milliseconds) or nil, got <type>` (`task.await_all: ...` from `await_all`) before anything is awaited.
- `task.await_all(tasks)` / `task.await_all(tasks, timeout_ms)` — awaits every task and returns their values in the order of `tasks`. The timeout covers the whole call. If any task fails or the timeout expires, the tasks not yet awaited are killed (except those that trap exits) and the error is raised; the messages that `await` starts with `task.await: ` start with `task.await_all: ` here.

Rules:
- Only the process that called `async` can await the task, and each task can be awaited once. Breaking either rule — including passing the same task to `await_all` twice — raises an error.
- A task whose function sets `process_flag("trap_exit", true)` is not killed by a timeout or an `await_all` failure (it gets an `EXIT` message instead): it runs on, and its result and `DOWN` later arrive in the owner's mailbox, where `await` no longer takes them. Don't trap exits in a task; if the owner keeps running, give its receive loop a catch-all, or the two messages stay queued.
- The result reaches the awaiting process as a message, and each task is monitored by its owner. `await` consumes both the result and the task's `DOWN`, so nothing is left in the mailbox. Selective `receive ... when` arms leave these messages alone unless a pattern matches `{tag: "_task_result"}` or the task's `DOWN`; a catch-all `receive()` can take them first, and `await` then raises an error that says so instead of waiting.

```
load "std/task"

let a = task.async do
  fetch(url_a)
end
let b = task.async do
  fetch(url_b)
end
let pages = task.await_all([a, b], 5000)
```

`std/supervisor` — exports `supervisor` table:

- `supervisor.start(spec)` — start a supervisor process. Returns `{pid: <pid>, shutdown: nil}` (the `shutdown` key tells a parent supervisor not to kill it midway; see `children`). A `spec` that is not a table raises `supervisor.start: expected a spec table {strategy, children, ...}, got <type>`. The spec table supports:
  - `strategy` — `"one_for_one"` (default): restart only the crashed child. `"one_for_all"`: stop all other running children, in reverse child-spec order, each as the supervisor's own shutdown stops it (`"shutdown"`, then `"kill"` after its `shutdown` budget; see below), so a child that is itself a supervisor has shut its own children down first, then start again, in order, the crashed child and the stopped children that are not `"temporary"`; a temporary child it stopped, and a child that was neither running nor owed a restart (see below), stay down (pid `-1`). Any other value raises `supervisor.start: unknown strategy ...`.
  - `children` — array of child specs: `{id: <string>, start: <fn>, restart: <string>, shutdown: <ms or nil>}`. The `start` function must spawn the child and return its pid, a `{pid}` handle or a registered name (so `start: fn() gen_server.start(m) end` works); the supervisor calls it from its own process, monitors the child and links to it. `restart` is `"permanent"` (always restart, default), `"temporary"` (never restart), or `"transient"` (restart only on an abnormal exit: a transient child that exits with reason `"normal"` or `"shutdown"` stays down). Any other `restart` raises `supervisor.start: unknown restart "<restart>" for child "<id>"`. `shutdown` is how long the child gets to exit after the exit signal `"shutdown"` when the supervisor stops it, in milliseconds (an int `<= 0` doesn't wait); after that the supervisor kills it with the untrappable `"kill"`. `nil` waits for it with no limit. When the spec has no `shutdown` key, the budget comes from the value `start` returned: a handle with a `shutdown` key (an int or `nil`) gives its own budget, as the handles of `supervisor.start` and `dynamic_supervisor.start` do with `shutdown: nil`, so a nested supervisor gets all the time its own ordered shutdown takes; anything else gets 5000 ms. A handle's `shutdown` counts only when it is an int or `nil`: the handle belongs to the child's own API, so any other value (a `shutdown` method on an object-style handle, say) is not a budget, and the child gets 5000 ms. `start` also raises a `supervisor.start: ...` error, and starts nothing, when `children` is not a table, a spec is not a table, a spec has no `id` or repeats another's, a spec's `start` is not a fn, or a spec's `shutdown` is neither an int nor `nil` (`supervisor.start: shutdown of child "<id>" must be an int (milliseconds) or nil, got <type>`).

    `start` should return while the child is still running. A child that has already exited when `start` returns (because `start` waited, or the child's work was that short) has an unknown exit reason: the supervisor's monitor reports `"noproc"`. A permanent child is restarted, a transient child is taken to have exited normally and stays down (pid `-1`), a temporary one stays down. The supervisor can't tell this apart from a child that really exits with reason `"noproc"` (one that links to a process that is already gone, without trapping exits, dies with it): a transient child that exits with `"noproc"` is not restarted either.
  - `max_restarts` — max restarts within the time window before the supervisor itself crashes with `supervisor: max restart intensity reached` (default 3). Must be an int, as must `max_seconds`.
  - `max_seconds` — time window in milliseconds for restart intensity (default 5000).
  - `name` — optional string name to register the supervisor process. `start` registers it before returning, so the name works at once; `start` raises `supervisor.start: the name "<name>" is already registered` (and starts nothing) when the name is taken, and `supervisor.start: name must be a string, got <type>` for a name that is not a string.

  `start` returns once the supervisor process has called every child's `start`, in child-spec order, so a name a child's `start` registers is taken when `supervisor.start` returns, and the children before a child have been started when its `start` runs. As in OTP, a nested supervisor's whole tree is up when the outer `start` returns. A child's `start` runs while the caller of `supervisor.start` waits, so one that waits for an answer from that caller never gets one. When a child's `start` raises (or returns something that is neither a pid, a `{pid}` handle nor a registered name), the supervisor stops the children started before it, as below, prints where the error was raised on stderr and exits with the error as its reason, and then `start` raises `supervisor.start: child "<id>" failed to start: <error>`; the supervisor's name is free by then. A supervisor that dies otherwise before its children have started (`kill(sup, "kill")` from another process, say) makes `start` raise `supervisor.start: supervisor exited: <reason>`. `start` leaves no monitor or message of its own behind.

  The supervisor traps exits and is linked to each child. It exits when it reaches its restart intensity (reason `supervisor: max restart intensity reached`), when it gets an exit signal (`kill(sup, "shutdown")` from `supervisor.stop`, a parent supervisor, one of its own children or anyone else, or the death of a process linked to it; a `"normal"` exit signal is ignored), or on any other error in the supervisor (it prints where that error was raised on stderr before its crash report). A restart whose `start` raises (a gen_server whose `init` fails, say) doesn't end it: the supervisor prints the error and where it was raised on stderr, marks the child as not running (pid `-1` in `which_children`) and tries again through a message to itself, so requests already queued are answered first. Each try counts as a restart, so the restart intensity ends a `start` that keeps failing. In `"one_for_all"`, the children before the failing one stay up, and it and the ones after it start, in order, when a try succeeds. Before it exits, it shuts its children down, as in OTP: one at a time, in reverse child-spec order (a restarted child keeps its place), it sends each running child the exit signal `"shutdown"`, whatever its own exit reason, waits up to the child's `shutdown` budget for its `DOWN`, then kills it with `"kill"` if it is still alive (its `DOWN` then has reason `"killed"`); then it exits with its own reason. A child that is itself a supervisor (or a `dynamic_supervisor`) does the same when it gets that signal, so the whole tree below is down, and every name registered in it free, by the time the supervisor's `DOWN` arrives. An `http` server sends `"shutdown"` to its acceptor and connections with no budget and no `"kill"`, so a connection whose handler traps exits can outlive the server's `DOWN` (see `http.stop`). That is what lets supervisors nest: a supervisor started as another's child is restarted with fresh children, so a child that registers a name can register it again. A child that traps exits gets an `EXIT` message with reason `"shutdown"` from its supervisor (the supervisor's pid is `self()` inside the child spec's `start`, which runs in the supervisor) and should clean up and exit; one that doesn't is killed when its budget runs out. A child whose budget is `nil` and that never exits keeps the supervisor waiting for good. The child's `DOWN` is what the supervisor restarts on; the `EXIT` a child's death also sends it is dropped.

  The supervisor traps exits from before `start` returns, so a `kill(sup, reason)` is handled when it next reads its mailbox: right after the `kill` returns, the supervisor and its children are still alive and its name is still registered. The exception is `kill(sup, "kill")`, which can't be trapped: the supervisor dies at once with `"killed"`, with no ordered shutdown. Its children get the exit signal `"killed"` through their links, so a child that doesn't trap exits dies with it, while one that traps exits only gets an `EXIT` message and keeps running unless it exits on it. A parent supervisor sends `"kill"` only to a child whose `shutdown` budget ran out, which is why supervisor handles ask for `shutdown: nil`. To stop a supervisor and know it is gone, use `supervisor.stop`, or monitor it and wait for its `DOWN`.
- `supervisor.which_children(target, timeout_ms = 5000)` — query a supervisor for its children. `target` is a pid, the `{pid}` handle `start` returns, or a registered name. Returns an array of `{id, pid, restart}` tables in child-spec order; `pid` is `-1` for a child that is not running (a temporary or transient child that exited and was not restarted, or a child whose restart failed and is being tried again). Raises `supervisor.which_children: supervisor exited: <reason>` at once when the supervisor is dead or dies before answering, and `supervisor.which_children: timeout` after `timeout_ms`. A `timeout_ms` of `nil` waits until the answer or the supervisor's death; any other non-int raises `supervisor.which_children: timeout must be an int (milliseconds) or nil, ...`. It monitors the supervisor for the length of the request, the way `gen_server.call` monitors its server, and leaves no monitor or `DOWN` of its own behind.
- `supervisor.stop(target, timeout_ms = nil)` — stops the supervisor with the exit signal `"shutdown"` and waits for its `DOWN`, which arrives once it has shut down its children and, through nested supervisors, the whole tree below it (see above), so their names are free when `stop` returns. Returns `nil`, also for a supervisor that has already exited; raises `supervisor.stop: the supervisor did not stop within <ms> ms` after `timeout_ms`. The default, `nil`, waits until the supervisor is down: each child has its own shutdown budget, so the time a tree takes to stop is bounded by the budgets, not by `stop`. `target` is accepted as in `which_children`. Like `http.stop`, it consumes the supervisor's `DOWN`, unless the caller monitors the supervisor itself: that monitor still gets its `DOWN`.

`std/dynamic_supervisor` — exports `dynamic_supervisor` table. For workloads where children are spawned on demand from a single template (worker pools, per-connection processes, lazily-created topic actors), instead of being declared up front. Restart policies and intensity (`max_restarts` / `max_seconds`) match `std/supervisor`. Strategy is fixed at one-for-one; failed children restart with their original `args`.

- `dynamic_supervisor.start({child, max_restarts, max_seconds, name})` — start a dynamic supervisor. Returns `{pid: <pid>, shutdown: nil}`, like `supervisor.start`. An argument that is not a table raises `dynamic_supervisor.start: expected an options table {child, ...}, got <type>`. The `child` template is `{start: <fn>, restart: <string>, shutdown: <ms or nil>}` where `start` takes one argument (the per-child args, packed into a table or any value the caller chose) and must spawn a process and return its pid or a `{pid}` handle (so `start: fn(a) gen_server.start(m) end` works); `restart` is `"permanent"` (default), `"transient"` (restart unless the child exits with `"normal"` or `"shutdown"`), or `"temporary"`; `shutdown` is each child's shutdown budget, as in a `supervisor` child spec (left out: from the handle `start` returns, else 5000 ms). A missing template, a `start` that is not a fn, any other `restart`, a `shutdown` that is neither an int nor `nil`, or a `max_restarts` / `max_seconds` that is not an int raises a `dynamic_supervisor.start: ...` error. `name` registers the supervisor before `start` returns; a name that is already taken raises `dynamic_supervisor.start: the name "<name>" is already registered`, and one that is not a string `dynamic_supervisor.start: name must be a string, got <type>`. Like `supervisor`, it traps exits and links each child, and when it exits (on reaching its restart intensity, `dynamic_supervisor: max restart intensity reached`, on an exit signal, from one of its children as well as from anyone else, or on an error) it first shuts its children down the same way: one at a time, last started first (a restarted child keeps its place), `"shutdown"`, then `"kill"` after the child's budget. A `kill` of it is handled when it next reads its mailbox, except `kill(sup, "kill")`, which ends it at once, as for `supervisor`. A child that is already gone when `start` returns has exit reason `"noproc"`, and is handled as by `supervisor`: a transient child is not restarted, and neither is one that really exits with `"noproc"`. A restart whose `start` raises is printed on stderr and tried again, each try counted as a restart, as for `supervisor`; until a try succeeds, `which_children` leaves the child out.
- `dynamic_supervisor.start_child(target, args, timeout_ms = 5000)` — ask the supervisor to spawn a child by calling `template.start(args)`. The supervisor monitors and links the child and stores `args` so it can replay the start on crash. Returns the new child's pid (a bare pid, also when `start` returned a handle). If `start` raises or returns something other than a pid or `{pid}` handle, the call raises `dynamic_supervisor.start_child: <error>` and the supervisor keeps running. The supervisor runs `start` in its own process and handles no other request meanwhile, so a slow `start` (a gen_server's `init`) delays its other callers, and one slower than `timeout_ms` makes `start_child` raise `dynamic_supervisor.start_child: timeout` although the child is started.
- `dynamic_supervisor.terminate_child(target, child, timeout_ms = nil)` — stop the child as the supervisor's shutdown does (`"shutdown"`, then `"kill"` after the child's budget), and remove it from the supervisor's child set. `child` is a pid, a `{pid}` handle or a registered name. Returns `true` once the child has exited (it is not restarted), `false` if the supervisor doesn't know it. The supervisor handles no other request while it waits for the child, so a child that traps exits and ignores the `EXIT` delays the supervisor's other callers by up to its budget. The default `timeout_ms`, `nil`, waits until the child is gone; an int raises `dynamic_supervisor.terminate_child: timeout` after that long (the child is still stopped and removed).
- `dynamic_supervisor.which_children(target, timeout_ms = 5000)` — returns an array of `{pid, args}` tables, one per live child, in the order they were started.
- `dynamic_supervisor.stop(target, timeout_ms = nil)` — as `supervisor.stop`: kills the supervisor with `"shutdown"` and waits for its `DOWN`, which arrives once its children are down; raises `dynamic_supervisor.stop: the supervisor did not stop within <ms> ms`.

`target` is the `{pid}` handle `start` returned, a bare pid, or the registered name. The `start_child`, `terminate_child` and `which_children` calls monitor the supervisor: they raise `dynamic_supervisor.<fn>: supervisor exited: <reason>` at once when it is dead or dies during the call, and `dynamic_supervisor.<fn>: timeout` after `timeout_ms` (`nil`: no timeout; another non-int raises `dynamic_supervisor.<fn>: timeout must be an int (milliseconds) or nil, ...`). A name nobody holds raises `dynamic_supervisor.<fn>: no process registered as "<name>"`. As with `gen_server.call`, the call leaves no monitor and no `DOWN` of its own behind.

`std/gen_server` — exports `gen_server` table. OTP-style synchronous server abstraction. Load with `load "std/gen_server"`.

The caller provides a module table with callback functions: `init`, `handle_call`, `handle_cast`, and `handle_info`. All four are required, even when a server never gets casts or other messages (`handle_cast: fn(msg, s) {state: s} end` ignores them).

- `gen_server.start(module[, {name}])` — starts a gen_server process running the given module. Returns `{pid: <pid>}`. With `name` (a string), the server is registered under it before `init` runs, so `call(name, ...)` works from the start, and a restarted server holds the name before its own `init` runs (between the old server's death and the restart, `whereis(name)` is `nil`); a `name` that isn't a string or is already registered raises, and nothing is started. A `module` that is not a table raises `gen_server.start: expected a module table {init, handle_call, handle_cast, handle_info}, got <type>`, and one whose callback is missing or not a fn raises `gen_server.start: the module's <callback> must be a fn, got <type>`; nothing is started. The module's `init()` is called in the server process to produce the initial state, and `start` returns once it has returned, so whatever `init` does (registering a name, binding a port) is done by then. The caller handles no messages while it waits, so an `init` that waits for an answer from the process calling `start` gets none; to do work after `start` returns, `init` can `send(self(), msg)` and do it in `handle_info`. An `init` that raises, or returns something other than `{state}` (see below), ends the server: it prints where the error was raised on stderr and exits with the error as its reason, and then `start` raises `gen_server.start: init failed: <error>`. A server killed during `init` by an exit signal it doesn't trap (`kill(self(), reason)`, say) makes `start` raise `gen_server.start: server exited: <reason>`. `start` leaves no monitor or message of its own behind. Like `handle_cast` and `handle_info`, it returns `{state: <s>}`, and may include `timeout: <ms>` to set an initial idle timeout.
- `gen_server.call(target, msg, timeout_ms = 5000)` — sends a synchronous request and waits for the reply, which it returns. `target` (here and in `cast`) is the `{pid}` handle `start` returns, a bare pid, or a registered name; anything else, or a name nobody registered, raises a `gen_server.call: ...` error. The caller monitors the server first, so a server that is already dead, or dies before replying, fails the call at once with `gen_server.call: server exited: <reason>` (reason `"noproc"` for a server that was gone already); otherwise the call raises `gen_server.call: timeout` after `timeout_ms`. A `timeout_ms` of `nil` waits until the reply or the server's death; any other non-int raises `gen_server.call: timeout must be an int (milliseconds) or nil, ...`. The monitor lasts only for the call: the call removes it (and drops the `DOWN` it delivered) when the call added it, so a process that has called a server gets no `DOWN` when the server later dies, and calling a dead server again and again leaves nothing in the mailbox. A monitor the caller set up itself is the same monitor (a process monitors a target at most once): the call leaves it in place, and when the server dies during the call, the call puts that monitor's `DOWN` back at the end of the mailbox. The request and reply are private messages (`{tag: "_gs_call", ...}`, `{tag: "_gs_reply", ...}`); the reply is matched by a fresh ref, so a late reply to a call that timed out is never returned by a later call.
- `gen_server.cast(target, msg)` — sends an asynchronous (fire-and-forget) message (`{tag: "_gs_cast", ...}`). Returns `nil`. A cast through a pid or a handle to a server that has exited is dropped; a cast through a registered name raises `gen_server.cast: no process registered as "<name>"` once the process holding it has died (the name is gone with it), like `send` to a name.
- `gen_server.reply(from, value)` — sends a reply to a pending `call`. Used for deferred replies when `handle_call` returns `{noreply: state}` instead of replying immediately. Returns `nil`. A `from` that is not a `{pid, ref}` table raises `gen_server.reply: expected the {pid, ref} from that handle_call got, got <what it got>`.
- `gen_server.stop(target, reason = "normal", timeout_ms = 5000)` — stops the server and returns `nil` once it has exited (also when it was gone already, given by pid or handle; a name nobody holds raises, as for `call`). The stop is a private message (`{tag: "_gs_stop", ...}`) handled after the messages queued before it: the server turns `trap_exit` off and ends itself with `kill(self(), reason)`, so its monitors and links see `reason` (`"kill"` as `"killed"`), and a linked process that doesn't trap exits dies with it unless the reason is `"normal"`. A call queued behind the stop fails with `gen_server.call: server exited: <reason>`. A `reason` that is not a string raises `gen_server.stop: reason must be a string, got <type>`, and a `timeout_ms` that is neither an int nor `nil` raises as for `call`. Past `timeout_ms` (`nil`: no limit) it raises `gen_server.stop: the server did not stop within <ms> ms`, and the server still stops when it reaches the message. The caller monitors the server for the stop and takes its `DOWN`, unless it already monitored the server: that monitor still gets its `DOWN`. A server under a supervisor is restarted (unless its restart policy says otherwise); stop the supervisor instead.

Module callback return values:

- `handle_call(msg, from, state)` — must return `{reply: <value>, state: <new_state>}` to reply immediately, or `{noreply: <new_state>}` to defer the reply (use `gen_server.reply(from, value)` later), not a table with both `reply` and `noreply`. May include `timeout: <ms>` to schedule an idle timeout.
- `init()` — must return `{state: <initial_state>}`. May include `timeout: <ms>`.
- `handle_cast(msg, state)` — must return `{state: <new_state>}`. May include `timeout: <ms>`.
- `handle_info(msg, state)` — handles any message not from `call`/`cast`/`stop` (e.g. DOWN messages, EXIT messages, `"timeout"`). Must return `{state: <new_state>}`. May include `timeout: <ms>`.

A callback that returns anything else (`nil`, as a `match` with no matching arm does, a non-table, or a table without those keys) is a bug in the module: the server dies with an error naming the callback and what it should return, such as `gen_server: handle_cast returned nil, expected {state: state}`, and a pending `call` fails at once with that reason. So does a `timeout` that is neither an int nor `nil` in a callback's result (`gen_server: handle_call returned a timeout that is a string, expected an int (milliseconds) or nil`); `handle_call`'s reply is not sent. Give each callback's `match` an `else`.

When any callback returns a `timeout` field, a timer is scheduled: after `timeout` milliseconds with no other message, `handle_info` is called with the string `"timeout"` as the message. Each new timeout cancels the previous pending one. Omitting `timeout` (or setting it to `nil`) cancels any pending timeout without scheduling a new one. A timeout that had already fired, but was still queued behind the message whose callback cancelled or replaced it, is ignored.

```
load "std/gen_server"

let counter_mod = {
  init: fn()
    {state: 0}
  end,
  handle_call: fn(msg, from, state)
    match msg
    when "get"
      {reply: state, state: state}
    when "inc"
      {reply: state + 1, state: state + 1}
    else
      {reply: nil, state: state}
    end
  end,
  handle_cast: fn(msg, state)
    match msg
    when "reset"
      {state: 0}
    else
      {state: state}
    end
  end,
  handle_info: fn(msg, state)
    {state: state}
  end
}

let server = gen_server.start(counter_mod)
gen_server.call(server, "inc")    # 1
gen_server.call(server, "inc")    # 2
gen_server.call(server, "get")    # 2
gen_server.cast(server, "reset")
```

Top-level `let` bindings (including std namespaces like `string`, `table`) are module-level bindings, accessible from named `fn` declarations, closures, and top-level code alike. Each process has its own copy (see Variables).

## What the Compiler Needs to Emit

Every value is a tagged C union. The compiler emits C code that uses the runtime glue which wires together per-process arenas, minicoro, and stb_ds.h. The generated C code allocates from the current process's arena (bump allocator), uses minicoro for coroutines with a built-in round-robin scheduler and mailbox channels, and stb_ds for table operations. The compiler emits C `#line` directives so that runtime errors report Gem source file and line numbers instead of C line numbers.

## What's NOT in v0

No classes. No inheritance. No exceptions (use `error()` to halt, `pcall()` for recovery). No type system. No operator overloading.

All of those can be added later _in the language itself_ via blocks, or in a future compiler version compiled by v0.
