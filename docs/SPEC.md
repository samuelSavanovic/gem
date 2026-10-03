# Gem Language Spec v0.1

_(working name, obviously)_

## Implementation Strategy

The compiler is self-hosting — written in Gem, compiling itself. A checked-in C file (`bootstrap/stage0.c`) serves as the bootstrap artifact so any C compiler can rebuild from scratch. No Python, no external parser generators.

Compilation target is C source code. `gcc`/`clang` handles optimization and linking. This gives us free C interop since we're already in native land.

C runtime is minimal glue code wiring together two libraries plus a per-process arena allocator:

- **Per-process arena allocation** — each process (coroutine) gets its own arena (bump allocator). When a process spawns another, values are deep-copied into the new process's arena. When a process dies, its entire arena is freed at once. Messages (`send`) are deep-copied into the target process's arena, and so is the parent's module state at `spawn` (each process has its own copy of the module-level bindings, see Variables). No global GC pauses — memory is reclaimed per-process on exit, and loops reclaim their own garbage as they run (see Long-Running Processes).
- **minicoro** — single-header stackful coroutines (libdill is abandoned, crashes on arm64 macOS). Runtime builds scheduler + channels on top (~150 lines)
- **stb_ds.h** — single-header hash maps and dynamic arrays for table implementation

## Compiler CLI

```
gem <file.gem>              # compile to a temp binary and run it
gem <file.gem> [args...]    # run, passing extra args through to the program
gem <file.gem> -c           # compile to ./<basename>, don't run
gem <file.gem> -o <name>    # compile to <name>, don't run (implies -c)
gem <file.gem> --emit-c     # print generated C to stdout (used for bootstrapping)
gem <file.gem> --check      # parse + analyze, exit 0 on success, 1 on error
gem <file.gem> --run        # accepted as a no-op; run is the default
gem --help                  # print usage (also -h), exit 0
gem lsp                     # start the language server on stdin/stdout
```

Options can come before or after the source path. Before it, an argument starting with `-` that is not one of the options above is a usage error: `gem` prints a short message and the usage line on stderr and exits 2 (as for a missing source path or `-o` without a name). After the source path, an unknown argument is passed to the program, so `gem prog.gem --help` gives `--help` to `prog.gem`.

A source path that cannot be read prints one line on stderr and exits 1: `gem: cannot open 'missing.gem'`, or `gem: 'src' is a directory, not a source file`.

The default behavior (`gem foo.gem`) writes generated C to `/tmp/gem_<basename>.c`, compiles it to `/tmp/gem_<basename>_bin`, and runs it. Extra positional arguments after the source path are forwarded to the program via `argv()`.

`-c` (or `--compile-only`) keeps the artifact: compiles to `./<basename>` and exits without running. `-o <name>` does the same but lets you pick the path.

`--emit-c` prints C to stdout and exits. This is what the bootstrapping Makefile targets use.

`--check` runs the parser, load resolution, and codegen analysis but skips emitting C and invoking `cc`. Exits 0 silently on success; on failure, prints the same caret-context diagnostic that the normal compile path produces and exits 1. Intended for editor save hooks, pre-commit, and CI lint.

## Values and Types

Nine types: `Int`, `Float`, `String`, `Bool`, `Nil`, `Table`, `Fn`, `Buffer`, `Ref`. All dynamically typed. Every value is a tagged C union. Yes this means primitives are boxed and slow — doesn't matter for v0. Future optimization: NaN-boxing to pack ints, bools, and nil into a double's NaN space, eliminating heap allocation for primitives.

Strings are binary-safe: they carry an explicit byte length, so `"\0"` is a 1-byte string, `len(s)` reports byte length (not strlen), and embedded NULs survive concatenation, indexing, equality, `build_string`, `tcp_write`, and `read_file`/`write_file` round-trips. Strings remain NUL-terminated for C interop convenience; the byte after the last content byte is always `\0`. **Caveat — `extern fn`:** when a Gem `String` is passed to an `extern fn ... s: String` parameter, it marshals as `const char *` and the C side will see only the bytes up to the first NUL. Use the `Bytes` extern type (see C Interop) when the C function needs binary data — it marshals the byte length alongside the pointer.

## Variables

```
let x = 10
let name = "hello"
x = 20
```

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
- `for` loop variables and `match`/`receive` pattern bindings are lets of their body and shadow the same way.
- **Warning:** a `let` in a `while` body (at any depth, not inside a closure) that shadows a variable the loop's condition reads, when nothing in the loop body assigns that name (`x = …` or `x += …`, closures included), gets a compile-time warning on stderr. The condition reads the outer variable, which then can't change through it: `while i < n` with `let i = i + 1` in the body never ends. To advance the counter, assign it: `i = i + 1`. Any assignment to the name in the body turns the warning off, as does renaming the new variable. A module-level variable that a named function of the same file assigns is not warned about, since a call in the loop can change it.

```
fn f(n)
  let before = fn() n end
  let n = n - 1        # new n, initialized from the parameter
  if n > 0
    let n = n * 10     # shadows until `end`
    print(n)           # 40
  end
  print(n, before())   # 4 5
end
f(5)
```

The exception is a `let` directly in a file's top-level code: that declares the module-level binding of the name (see below), of which there is one per name. A second top-level `let` of the same name rebinds that module-level binding, as an assignment would; named functions and closures, which always read module-level bindings live, see the new value.

### Destructuring

Destructuring extracts fields from tables or elements from arrays:

```
# Table destructuring — extract named fields
let {method, path} = parse_request(raw)

# Array destructuring — extract by position
let [first, second] = string.split(line, ",")

# Per-field defaults — apply when the field is missing OR nil
let {strategy = "one_for_one", max_restarts = 3} = spec
let [head, tail = []] = parts
```

Table destructuring extracts by name (`let {a, b} = expr` is `let a = expr.a; let b = expr.b`). Array destructuring extracts by index (`let [a, b] = expr` is `let a = expr[0]; let b = expr[1]`). The RHS is evaluated exactly once. Missing keys/indices produce `nil`. Per-field defaults (`name = expr`) substitute when the extracted value is `nil` (i.e. the key was absent or its value was nil); the default is only evaluated in that case and may reference earlier names in the same destructure. Like any `let` initializer, a default that names the field it binds sees the binding it shadows: in `fn f(port) let {port = port + 1} = {} ... end`, the default reads the parameter. Patterns are flat — no renaming, nesting, or rest/splat. Match/receive patterns are stricter and do **not** accept defaults; defaults are a binding-context feature only (let, fn params).

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

To share state between processes, put it in a process and talk to it with messages (`gen_server`, `register`). The compiler prints a `note:` at each write to module state in code reachable from a `spawn` body, as a reminder that the write is process-local. The note names the binding as you write it in source (`count` in the entry file, `counter.count` for the top-level `count` of module `counter.gem`, even when it is loaded with an alias), with project-relative paths.

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

Function call argument lists allow newlines inside the parentheses — after `(`, after each `,`, and before `)`:

```
let result = add(
  long_expression_a,
  long_expression_b
)
```

Table and array literals may span lines the same way. Other expressions may not: a line break after a binary operator or inside parentheses ends the statement (`let t = (1 +` followed by `2)` on the next line is a parse error), and a line that starts with `-` is a new statement (`let x = 1` followed by `- 2` on the next line leaves `x` at `1`). Parameter lists in a `fn` definition must fit on one line. Split a long condition into named `let`s.

A call with more arguments than the function declares drops the extra ones, and missing arguments are `nil` (or the parameter's default); neither is an error. Calling a non-function value is a runtime error.

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

Default parameters work in named functions, anonymous functions, and block parameters. Passing `nil` explicitly does *not* trigger the default — only omitting the argument does. A default is evaluated at call time, each time the argument is omitted, and may refer to earlier parameters.

## Destructuring Parameters

A function parameter position can be a flat table-destructure pattern. The named fields are bound as locals at the start of the body, and per-field defaults work the same way as in `let`-destructuring (the default fires when the field is missing or nil):

```
fn start({port = 8080, host = "0.0.0.0", name = nil})
  ...
end

start({port: 9000})              # host defaults, name defaults to nil
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

Extra fields in the caller's table are ignored (partial match). Patterns are flat (no nested destructuring) and table-only — array-destructured parameters are not supported. Param destructuring composes with regular and rest parameters, and with tail call optimization.

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

Single-expression blocks use braces:

```
times(5) { |i| print(i) }
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

A trailing block cannot follow a call anywhere in an `if`, `elif`, `while` or `for` header, and a header takes no `do`: `while running do` is reported as an error. To pass a block to a call there, assign the call's result to a variable first.

This one rule lets std define `each`, `map`, `filter` and friends without compiler changes.

## Tables

```
let point = { x: 10, y: 20 }
point.x
point["x"]

let list = [1, 2, 3]
list[0]
```

`{ }` with keys is a table. `[ ]` is sugar for an integer-keyed table. Dot access is sugar for string key lookup. Keywords are allowed as table keys and dot fields: `{else: body}`, `node.else`.

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

# Array pattern — match on length and bind positional elements
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
- `[p1, p2, ...]` — checks target is a table with `len(target) == N`, then recursively matches each element. A record with N keys passes the length check too (see `docs/KNOWN_BUGS.md`), so put array arms after record arms when both can occur.
- There are no guards or alternatives: `when v > 5` and `when "a" or "b"` are expression arms that compare the target with the value of `v > 5` or `"a" or "b"`. Use an `if` chain, or one arm per value.
- A literal (int, float, string, bool) in pattern position matches by equality. `nil` is also a literal — `when nil` matches only `nil`, it does not bind a variable.
- A name in pattern position is a variable binding — always matches and binds the matched value.
- `^name` (a pin) matches by equality with the current value of the variable `name`; it binds nothing. The pinned name must already be in scope, and only a plain variable name can be pinned — bind an expression like `t.ref` to a local first. `when ^x` works at the top of a `match` or `receive` arm as well as inside table and array patterns. The comparison is `==`, so a pinned table matches only that same table — never a copy received in a message. Pin primitives and refs.
- A bare name after `when` (e.g., `when x`) is a catch-all that binds the entire match target.
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

# Range iteration (0 to n-1)
for i = 0, n
  print(i)
end
```

`break` and `continue` work inside `for` loops. The iterator increment happens before the user body, so `continue` correctly advances to the next element.

The range form evaluates its bound once, before the loop, and assigning the loop variable in the body doesn't change the next iteration. The table form evaluates the RHS expression exactly once. It walks the table's entries in `keys()` order without building a `keys()` array, reading the entry count once before the loop: an entry added during the loop is not visited, and a `delete` during the loop (which moves the last entry into the hole) makes it skip entries and then visit `nil`. The single-variable form is for arrays: it re-reads `len` every iteration, so `push` during the loop extends it and `remove_at` makes it skip elements; on a string-keyed table it yields `nil` for each entry (use `for k, v` or `values(t)`).

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

Mutual tail recursion between named functions (`fn A` tail-calls `fn B`, which tail-calls `fn A`, through any number of functions) is also run at constant stack depth, as long as no function in the cycle has rest or default parameters, a parameter that a nested closure assigns to, or more than 16 parameters. A cycle that does not qualify grows the stack on every call; write such a loop as direct self-recursion or use `while`.

## Long-Running Processes — Per-Iteration Arena Reset

Each process has its own arena (bump allocator), freed in bulk when the process exits. For short-lived processes (request handlers, one-shot tasks) that is all there is — no per-allocation free, no GC pauses.

Loops reclaim their own garbage as they run, so long-lived processes (accept loops, keep-alive HTTP handlers, supervisors, gen_servers) and long computations stay at bounded memory. Every loop gets the treatment:

1. **`while` loops**, including `for` loops and `while true`.
2. **Self tail calls** (TCO, see above), with any number of arguments — including none — and any kind of parameter.
3. **Mutual tail calls** that the compiler turns into a trampoline.

When a loop starts, the runtime remembers how much of the arena is in use. At the loop's back-edge, once the loop has allocated enough (at least 1 MB), the runtime copies the values that are still reachable from what was allocated since the loop started — the loop's live variables (computed by the compiler's liveness analysis), the process's module state and mailbox, and anything stored into older tables or buffers — into fresh memory, and returns the rest to the OS. Memory allocated before the loop started is never moved or freed by the loop, which is what makes this safe wherever the loop runs: called from any position, at any depth, inside `pcall`, in a spawned closure or in the main program. From the program's perspective nothing happens.

The cost of a reset is proportional to the data it keeps, and the next reset waits until the loop has allocated at least twice that much again, so the total reset work stays proportional to the memory the loop allocates — a loop that builds a large table, or a server holding a large state, does not slow down as its live data grows.

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

Garbage made outside any loop (straight-line code, or before a loop starts) is freed when an enclosing loop resets or when the process exits.

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

`spawn`, `send`, `receive` are runtime functions, not keywords. Under the hood they use minicoro coroutines with a round-robin scheduler. Each spawned coroutine gets a mailbox (a simple queue). `receive` yields the coroutine if the mailbox is empty; the scheduler resumes it when a message arrives via `send`.

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

Monitoring a pid whose process no longer exists delivers the DOWN message immediately, with reason `"noproc"`. Duplicate monitors are deduplicated. Returns `true`.

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

The `after <ms>` clause is optional. If present and the timeout elapses with no matching message, the `after` body executes. `after 0` means "check once, don't block." Omitting `after` means block forever (like `receive()`).

The `receive` block can produce a value when used as the last statement of a function (implicit return), just like `match`.

The `receive()` function call always pops the head of the mailbox unconditionally, whatever the message is.

## Process Control

`kill(pid, reason)` sends an exit signal to a process. If the target has `trap_exit` enabled, an `{tag: "EXIT", pid: sender_pid, reason: reason}` message is delivered to its mailbox instead of terminating it. Otherwise, the process is terminated immediately: marked dead, DOWN messages delivered to monitors, registered name removed, and exit propagated to linked processes. Returns `true` if the process was alive, `nil` otherwise. A process can kill itself: `kill(self(), reason)` ends it at once with that reason (unless it traps exits), and `pcall` does not catch it. Likewise, when a `kill` brings down the caller through a link, the caller dies at once; `pcall` does not catch that either. If the process ending this way is the main process and the reason is not `"normal"`, the reason is printed as a runtime error and the program exits with status 1; when another process kills main, or main dies through a link, the message names the sender: `main process killed by process <pid>: <reason>` or `main process killed by linked process <pid>: <reason>`, followed by main's stack trace (where main was when the signal arrived). A process killed while it waits on the thread pool (`read_file`, `write_file`, `append_file`, `exec`, `sqlite_open`, `sqlite_close`, an `extern blocking fn`) dies at once, but the operation itself still runs to completion on its worker thread: a write still lands, a command started by `exec` keeps running, and once the worker is done the runtime frees the result (closing the connection an abandoned `sqlite_open` opened), except a `Ptr` returned by an `extern blocking fn`.

`sleep(ms)` suspends the current process for `ms` milliseconds. The scheduler resumes the process after the deadline expires; messages that arrive meanwhile wait in the mailbox and do not end the sleep early. `sleep(0)` yields to other ready processes and returns.

`time_ms()` returns the current monotonic time in milliseconds (int). Useful for timeouts, benchmarks, and restart intensity tracking. Not suitable for wall-clock formatting — use `epoch_ms()` for that.

`epoch_ms()` returns the current wall-clock time as milliseconds since the Unix epoch (int). Use this for timestamps that need to be formatted into dates or times. Uses `gettimeofday` internally.

`format_time(epoch_ms, format_str)` formats a wall-clock epoch millisecond timestamp into a UTC string using `strftime` specifiers. Returns a string. Supported specifiers include `%Y` (4-digit year), `%m` (month 01-12), `%d` (day 01-31), `%H` (hour 00-23), `%M` (minute 00-59), `%S` (second 00-59), `%a` (abbreviated weekday), `%b` (abbreviated month), `%T` (`%H:%M:%S`), `%F` (`%Y-%m-%d`), and all other platform-supported `strftime` specifiers.

`format_time_local(epoch_ms, format_str)` — same as `format_time` but formats in the local timezone instead of UTC.

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
# info == {state: "ready", mailbox_len: 0, links: [], monitors: [],
#          trap_exit: false, exit_reason: nil}
```

`process_info(pid)` returns a table with metadata about the process:

- `state` — `"ready"`, `"waiting"`, or `"dead"`
- `mailbox_len` — number of messages in mailbox
- `links` — array of linked pids
- `monitors` — array of monitoring pids
- `trap_exit` — bool
- `exit_reason` — string or nil

Returns `nil` if the pid is invalid or the slot is free.

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

An `extern fn` is a binding like a top-level `fn`: one named like a builtin (`extern fn sqrt(x: Float) -> Float`) shadows the builtin in its own file, and one in a loaded module can be exported and called as `module.name`. The C function it calls is always the declared name.

The generated wrapper validates `argc` and each argument's runtime type tag before reading the `GemVal` union, so a Gem-side mistake (wrong arity, wrong type) raises a Gem-level error at the boundary instead of passing garbage to C. Errors mention the declared Gem-level type name (e.g. `foo: arg 0 expected String, got int`). Types are not converted: a `Float` parameter rejects an int (`sqrt(2)` raises; pass `2.0` or `to_float(n)`). Too few arguments raise; extra arguments are currently ignored (see `docs/KNOWN_BUGS.md`). A `Ptr` is an int on the Gem side, and `NULL` comes back as `0`, not `nil`.

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

Auto-generation is skipped when `extern include` is present (the user manages declarations via headers).

`extern blocking fn` does not accept `Table` parameters or returns (compile error `extern blocking fn cannot take a Table`): the call runs on another thread, away from the process's arena.

### `Bytes` — binary-safe FFI

`Bytes` is for binary data that may contain embedded NULs (compressed payloads, crypto blobs, length-framed protocols). Unlike `String`, which marshals as a NUL-terminated `const char *` and is implicitly truncated at the first `\0` on the C side, a `Bytes` parameter expands to **two** C parameters — pointer + length — so the C function knows the exact byte count.

```
extern fn write(fd: Int, buf: Bytes) -> Int          # POSIX write(int, const void*, size_t)
extern fn sha256(data: Bytes, out: Bytes) -> Nil     # in-place hash
```

The first declaration generates the C call `write(fd, buf, buf_len)` — three C arguments from two Gem arguments. This matches POSIX `write` exactly.

**Constructing a `Bytes` argument from Gem.** There is no separate `Bytes` type at the Gem level — a `Bytes` parameter accepts any Gem `String`. Strings are already binary-safe (`len(s)` is the byte length, embedded NULs are preserved), so any operation that produces a string produces a valid `Bytes` payload:

```
let s1 = "header\0\x01\x02data"          # literal with embedded NUL
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
extern blocking fn net_accept(server_fd: Int) -> Int
extern blocking fn net_read(fd: Int, max_bytes: Int) -> String
```

For C headers (structs, typedefs, system headers the compiler can't infer):

```
extern include "math.h"
extern include "stdio.h"
```

The line is emitted as `#include "<path>"` into the generated C file, which is compiled in a temporary directory with the runtime directory on the include path; so system headers work by name, and a header of your own needs an absolute path (a relative one is not looked up next to the `.gem` file; see `docs/KNOWN_BUGS.md`). Put your own C functions in the header as `static` functions. The header is included before `gem.h`, so one that uses `GemVal`, `GemBytes` or `gem_bytes` must `#include "gem.h"` itself. The program is linked against libc, libm and pthreads only.

**String-return ownership** differs by call kind:

- `extern fn` (non-blocking) — the runtime copies the returned `char*` into the calling process's arena via `gem_string` and **does not free the original**. Use this for static literals (`getenv`, `strerror`, etc.). A `malloc`'d return will leak.
- `extern blocking fn` — the runtime copies into the arena and **frees the original** with `free`. The C function must return a `malloc`/`strdup`'d pointer; returning a static literal will crash on the free. NULL is allowed and yields an empty string from an `extern blocking fn`, or `nil` from an `extern fn`.

**Pointer lifetime.** `String`, `Bytes`, and `Table` arguments passed to an `extern fn` point into the calling process's arena. They are stable for the duration of the call but **not** across the next arena reset (which can happen at the back-edge of any loop or self tail call). An `extern blocking fn` receives malloc'd copies of its `String` and `Bytes` arguments instead, which the runtime frees once the call is over. In both cases C code must not stash these pointers — copy out with `strdup`, `memcpy`, or by value before retaining.

The wrapper checks the Gem-level type of each argument and that enough arguments were passed (see above), but not the declaration itself: a signature that doesn't match the real C function passes wrong values or crashes, and nothing checks what the C code does with its pointers.

## Operators

`+` for both arithmetic and string concatenation. If types don't match (e.g. string + int), runtime error. No `..` operator — keep it simple.

`+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `and`, `or`, `not`, `in`

`%` takes integers only (a float operand raises). Integer division and `%` by zero raise `division by zero`, and so does float division by zero (no `inf`). `<`, `<=`, `>`, `>=` compare an int with a float numerically, but `==` never equates them (`2 == 2.0` is `false`).

`x in tbl` — membership test. For tables with no string keys (arrays, and int-keyed tables such as `seen[5] = true`): returns `true` if `x` equals any value (linear scan), so use `has_key` to test an int key. For string-keyed tables: returns `true` if `x` is a key in the table (same as `has_key(tbl, x)`). Precedence is at the comparison level (same as `==`, `<`, etc.).

**Equality semantics:** `==` compares by value for primitives (int, float, string, bool, nil) and by identity (reference) for tables, functions, and refs. Two distinct tables with identical contents are not equal: `{a: 1} == {a: 1}` is `false`. The same table reference compared to itself is `true`.

## Assignment Operators

`=`, `+=`, `-=`, `*=`, `/=`

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
- The closing `"""` or `'''` must appear on its own line with only leading whitespace before it.
- **Dedent**: the indentation of the closing delimiter (number of leading spaces) is the base indentation. That many leading spaces are stripped from every content line. Extra indentation beyond the base is preserved. A `"""` string inside an interpolation has its own closing line and its own dedent; it does not change the outer string's.
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

## Comments

```
# single line comment
```

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

**Expression form** — `pcall <expr>` wraps a single expression. The parser desugars it to `pcall(fn() <expr> end)`:

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

**Function form** — `pcall(f)` calls `f()` with zero arguments. Useful when the body contains multiple statements:

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
- To pass arguments to the called function, use a closure: `pcall(fn() f(x, y) end)`

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

`print(args...)` — prints values separated by spaces, followed by a newline. Not binary-safe: a string is printed up to its first `\0` (use `write_stdout` for raw bytes). Stdout is line-buffered, so each `print` flushes — long-running processes don't lose output on `SIGKILL` even when stdout is redirected to a pipe or file.

`error(msg)` — prints the message with file and line info to stderr, prints a call stack trace, and halts (`exit(1)`).

`len(v)` — returns the length of a string, the byte length of a buffer, or the total number of entries in a table (both integer-keyed and string-keyed). `len({a: 1, b: 2})` returns 2. `len([10, 20, 30])` returns 3.

`type(v)` — returns the type name as a string: `"int"`, `"float"`, `"string"`, `"bool"`, `"nil"`, `"table"`, `"fn"`, `"ref"`, `"buffer"`.

`to_string(v)` — converts any value to its string representation. For buffers, returns the buffer contents as a string. For tables and arrays, recursively renders a `{key: val, ...}` / `[v1, v2, ...]` form (cycles render as `<cycle>`; deep/wide structures truncate with `...`). Same repr is used by `print`, `eprint`, and `"{x}"` interpolation, except for buffers, which those show as `<buffer:N>` (N is the length); call `to_string(buf)` for the contents. Floats are formatted with C's `%g`: six significant digits, and no decimal point for integral values (`to_string(2.0)` is `"2"`, `to_string(1234567.89)` is `"1.23457e+06"`); see `docs/KNOWN_BUGS.md`.

`to_int(v)` — converts a value to an integer. Strings are parsed as decimal integers; leading spaces are skipped, but trailing whitespace (a `\n` from a file line included) is an error, so `trim` first. Floats are truncated. Bools become 0/1. Errors on nil, tables, functions, or unparseable strings.

`to_float(v)` — converts a value to a float. Strings are parsed as decimal floats. Ints are widened. Bools become 0.0/1.0. Errors on nil, tables, functions, or unparseable strings.

`push(target, val)` — if `target` is a table, appends `val` at the next integer index (mutates in place, returns `val`). If `target` is a buffer, appends `val` coerced to a string (same as `buf_push`; returns the buffer).

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

`substr(s, start[, len])` — returns a substring of `s` starting at `start` (a negative `start` counts as `0`, unlike `s[-1]`). If `len` is provided, returns at most `len` characters; otherwise returns to the end of the string. Accepts buffers as well as strings (the result is always a fresh string).

`chr(n)` — converts an integer (0–255) to a single-character string with that byte value.

`ord(s)` — returns the byte value of the first character of string (or buffer) `s` as an integer.

`ord(s, i)` — returns the byte value at index `i` in string or buffer `s` without allocating a temporary string. Equivalent to `ord(s[i])` but avoids the 1-char string allocation.

`buf_new()` — creates a new mutable string buffer. Returns a buffer value (type `"buffer"`).

`buf_push(buf, val)` — appends `val` to the buffer. Non-string values are auto-coerced to strings. Returns the buffer for chaining. Uses a doubling growth strategy internally — O(n) total for n appends vs O(n²) for repeated `+` concatenation.

To finalize a buffer into an immutable string, use `to_string(buf)` — the generic `to_string` builtin handles buffers. The buffer remains usable afterward.

`build_string(block)` — creates a buffer, calls `block` with an `add` function that appends its arguments to the buffer, then returns the finalized string. Equivalent to `buf_new`/`buf_push`/`to_string` but more concise:

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

`process_flag("trap_exit", bool)` — sets the `trap_exit` flag on the current process. Returns the previous value (bool). When `trap_exit` is `true`, exit signals from linked processes are converted to `{tag: "EXIT", pid: <pid>, reason: <reason>}` messages in the process's mailbox instead of killing the process. A process that doesn't trap exits dies when a linked process exits with a reason other than `"normal"`; when that process is main, the program reports `main process killed by linked process <pid>: <reason>` with main's stack trace on stderr and exits with status 1.

`sleep(ms)` — suspends the current process for `ms` milliseconds. Works in any process, including the main program.

`send_after(pid, msg, delay_ms)` — schedules `msg` to be delivered to `pid` after `delay_ms` milliseconds. Returns a unique ref identifying the timer. The timer fires from the scheduler, not the calling process. Timers for a process are dropped when it exits, and none is scheduled for a pid that has already exited.

`cancel_timer(ref)` — cancels a pending timer by ref. Returns `true` if cancelled, `false` if it already fired, was dropped because its target exited, or is not found.

`processes()` — returns a list of all live process pids (excludes free and dead processes).

`process_info(pid)` — returns a table with process metadata: `state`, `mailbox_len`, `links`, `monitors`, `trap_exit`, `exit_reason`. Returns `nil` for invalid/free pids.

`read_file(path)` — reads the entire file at `path` and returns its contents as a string. Opens in binary mode (no newline translation). Files whose size isn't known up front (`/proc` and `/sys` files, pipes, devices such as `/dev/stdin`) are read until end of file. Raises an error if the file cannot be opened, is a directory, or a read fails.

`write_file(path, content)` — writes the string `content` to `path`, overwriting any existing file. Opens in binary mode. Raises an error if the file cannot be opened or if the write fails.

`append_file(path, content)` — appends the string `content` to `path`. Creates the file if it doesn't exist. Opens in binary mode. Raises an error on failure.

`delete(tbl, key)` — removes the entry for `key` from the table. Returns the removed value, or `nil` if the key was not found. Uses swap-remove internally (O(1) but does not preserve insertion order). For ordered removal from arrays, use `remove_at`.

`pop(arr)` — removes and returns the last element of an array table. Errors on empty table.

`values(tbl)` — returns a new integer-indexed array containing all values from `tbl`, in storage order.

`eprint(args...)` — like `print`, but writes to stderr instead of stdout.

`exit(code)` — terminates the program with the given exit code (default 0).

`argv()` — returns the command-line arguments as an integer-indexed array. `argv()[0]` is the program name.

`sort(arr)` — sorts an array table in place using the default ordering (numbers < strings, within type: numeric/lexicographic). Returns the table. Renumbers keys to 0..n-1.

`sort(arr, cmp)` — sorts with a custom comparator function. `cmp(a, b)` must return a negative number if a < b, 0 if equal, positive if a > b.

`floor(x)` — returns the largest integer ≤ x. Returns the value unchanged if already an integer.

`ceil(x)` — returns the smallest integer ≥ x. Returns the value unchanged if already an integer.

`round(x)` — rounds to the nearest integer (half away from zero). Returns the value unchanged if already an integer.

`abs(x)` — returns the absolute value. Preserves type (int→int, float→float).

`pow(x, y)` — returns x raised to the power y. Returns int if both arguments are int and y ≥ 0, float otherwise.

`sqrt(x)` — returns the square root of x as a float. Errors on negative argument.

`random()` — returns a random float in [0, 1). Uses xorshift64* internally.

`random(n)` — returns a random integer in [0, n). Errors if n ≤ 0.

`getenv(name)` — returns the value of environment variable `name` as a string, or `nil` if not set.

`input()` — reads a line from stdin, stripping the trailing newline. Returns `nil` on EOF.

`input(prompt)` — prints `prompt` to stdout (no newline), then reads a line from stdin.

`read_stdin(n)` — reads exactly `n` bytes from stdin and returns them as a binary-safe string. Returns `""` for `n == 0`. On EOF the returned string may be shorter than `n` (or empty if EOF arrives first). Blocks the OS thread on `fread` — does not yield to the scheduler. Intended for length-framed protocols (e.g. LSP `Content-Length` bodies); for line-oriented input use `input()`.

`write_stdout(s)` — writes `s` to stdout as raw bytes (no trailing newline) and `fflush`es. Binary-safe (uses the string's stored length, not `strlen`). Pair with `read_stdin` when implementing framed stdio protocols where the receiver expects the frame on the wire as soon as the writer is done.

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

## TCP Sockets

`tcp_listen(host, port)` — creates a TCP server socket bound to `host` (string) on `port` (int). Calls `socket`, `bind`, and `listen` with a backlog of 1024. Sets `SO_REUSEADDR`. Returns the socket file descriptor as an integer. Raises an error on failure. Always synchronous (fast).

`tcp_connect(host, port)` — opens a TCP connection to `host:port`. Returns the connected socket file descriptor as an integer. Raises an error if the connection fails or the host cannot be resolved. Supports both IP addresses and hostnames. The connect itself is non-blocking: the calling process (main included) yields to the scheduler until it completes. Resolving a host name is not: `gethostbyname` runs inline and blocks every process.

`tcp_accept(socket)` — accepts an incoming connection on a listening socket. Returns the new connection's file descriptor as an integer. The calling process (main included) yields to the scheduler until a connection is ready. Raises an error on failure.

`tcp_read(socket[, max_bytes[, timeout_ms]])` — reads up to `max_bytes` bytes from a connected socket (default 4096). Returns the data as a string on success, `""` when the remote end has closed the connection (EOF or `ECONNRESET`), or `nil` when the optional `timeout_ms` expires with no data available. Callers without a timeout never see `nil`. While no data is available the calling process (main included) yields to the scheduler, and resumes when data arrives or the timeout deadline is reached. A `timeout_ms` of `0` or less means no timeout.

`tcp_write(socket, data)` — writes the string `data` to a connected socket. Writes all bytes (loops internally on partial writes). Returns the number of bytes written as an integer. The calling process yields to the scheduler while the socket is not writable; there is no timeout. Writing to a peer that has closed the connection does not raise: the first write usually still reports success and later ones return `0`.

`tcp_close(socket)` — closes a socket file descriptor. Always synchronous. Returns `nil`.

All TCP builtins use non-blocking sockets with scheduler poll integration. The scheduler's `poll()` loop handles readiness notification with zero thread pool overhead.

## SQLite

`sqlite_open(path)` — opens (or creates) a SQLite database at `path`. Enables WAL mode and foreign keys by default. Returns an opaque database handle (stored as an int). Use `":memory:"` for an in-memory database. Runs on the thread pool in every process, main included, so other processes keep running. Raises on error.

`sqlite_close(db)` — closes the database handle. Runs on the thread pool, like `sqlite_open`. Returns `nil`.

`sqlite_exec(db, sql)` — executes SQL that returns no rows (DDL, INSERT without RETURNING, etc.). Inline execution (no thread pool). Raises on error.

`sqlite_query(db, sql, params)` — executes a parameterized query. `params` is an array of bind values matching `?` placeholders in `sql`. Returns an array of row tables, where each row is a string-keyed table (e.g., `{id: 1, name: "Alice"}`). Column type mapping: INTEGER → Int, REAL → Float, TEXT → String, NULL → Nil, BLOB → String (raw bytes). Inline execution (no thread pool). Raises on error.

`sqlite_last_insert_id(db)` — returns `sqlite3_last_insert_rowid` as an int.

`sqlite_changes(db)` — returns the number of rows affected by the last INSERT/UPDATE/DELETE as an int.

SQLite is vendored as an amalgamation (`runtime/sqlite3.c` + `runtime/sqlite3.h`), compiled into the runtime static library. No system dependency needed.

**Negative array indexing** — Integer indices to arrays and strings may be negative. A negative index `i` on a collection of length `n` resolves to `n + i`. So `arr[-1]` is the last element, `arr[-2]` is second-to-last, etc. Negative integers are always positions, never keys: `t[-10] = x` on a table with fewer than 10 entries raises, so key data by negative numbers with strings. Reading an array at a non-negative index past the end gives `nil`; a negative index that is still out of bounds after resolution raises `array index out of bounds`. A string index out of range either way raises `string index out of bounds`.

All builtins are first-class values — they can be stored in variables and passed to functions.

**Builtin names are not reserved.** Builtins live in the outermost scope, so any binding with a builtin's name shadows the builtin wherever that binding is in scope, and every call or reference there reaches the binding:

- A top-level `fn`, `extern fn` or `let` (a destructuring `let` or a selective import such as `load "std/log" (error)` included) shadows the builtin for the whole file it is in, whether that is the program's entry file or a loaded module. It does not leak into modules that file loads, or into files that load it: `std/log` defines and exports `error`, and `log.error(msg)` logs while a bare `error(msg)` elsewhere still raises.
- A parameter, `let` local, loop variable or pattern binding shadows it for its scope (`fn f(len) len + 1 end` is fine).

`for` loops and `match` patterns keep working inside such a scope: they never call a user binding named `len`, `type` or `has_key`.

## Module System

**Load statement** — `load` brings another file's exported definitions into scope. Every loaded file must have an `export` statement declaring its public API. The compiler keeps a table of already-loaded file paths; re-importing a module skips re-parsing but still creates the requested import bindings.

```
load "compiler/parser"
load "std/table"
```

Path resolution depends on the form of the load path:

1. **`load "std/X"`** — reserved for the standard library. Resolves against the stdlib root (see below). Hard error if the file does not exist; never falls back to a local file.
2. **`load "./X"` / `load "../X"`** — relative to the importing file's directory. Use this for sibling/cousin imports inside a project (e.g. `compiler/main.gem` doing `load "./parser"`).
3. **`load "X"` or `load "X/Y"`** (bare path, no prefix) — relative to the **project root**, defined as the nearest ancestor directory of the entry source file containing a `gem.toml` marker. If no `gem.toml` is found, falls back to the importing file's directory.

**Stdlib root** is resolved in this order:

1. `$GEM_STDLIB` if set: the directory that *contains* `std/` (not `std/` itself).
2. The project root, if it contains a `std/` subdirectory (lets a project vendor or override the stdlib). It replaces the whole stdlib: a `load "std/x"` that isn't in the project's `std/` fails, with no fallback to the installed one.
3. The install root, computed as `dirname(dirname(argv()[0]))` — so a binary at `<project>/build/gem` finds `<project>/std/`. `argv()[0]` is taken as typed, so a symlink to the binary on `PATH` finds neither `std/` nor `runtime/` (see `docs/KNOWN_BUGS.md`); call the binary by its real path (`GEM_STDLIB` finds `std/` but not `runtime/`, so the C compile still fails).

**Project root marker** — drop a `gem.toml` file at the root of your project to mark it. The file may be empty; its presence is what matters. Without it, bare-path loads behave like relative-to-importing-file (which is the safe default for single-file scripts).

**Export declaration** — `export name1, name2, ...` declares which names a file exports. Placed at the end of the file.

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
- The exported names are collected into a table named after the file's basename.

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

All three `load` forms use the same two-step path resolution. When a module has already been loaded by the program, re-importing it skips re-parsing but still creates the requested bindings (table, alias, or selective).

**Circular imports** are a compile error. If module A loads B and B loads A (or a longer chain closes back on a file still being loaded, the entry file included, or a file loads itself), the compiler reports one error at the `load` that closes the cycle, naming the chain with project-relative paths: `load cycle: a.gem → b.gem → a.gem`. Move the code the modules share into a module that loads none of them. A diamond (A loads B and C, both load D) is not a cycle: D is loaded once and reused.

## Standard Library (std/)

Each std file uses `export` to expose a single namespace table. When loaded, the module table is bound to the basename (e.g., `string`, `table`, `math`).

```
load "std/string"
load "std/table"

let parts = string.split("a,b,c", ",")
let trimmed = string.trim("  hello  ")
table.each(parts) { |item| print(item) }
```

`std/string` — exports `string` table:

- `string.split(s, delim)` — split string by delimiter, return array
- `string.index_of(s, needle)` — find first occurrence, return index or -1
- `string.join(arr, delim)` — join array elements with delimiter
- `string.trim(s)` — strip leading/trailing ASCII whitespace
- `string.starts_with(s, prefix)` / `string.ends_with(s, suffix)` — boolean prefix/suffix check
- `string.upper(s)` / `string.lower(s)` — ASCII case conversion
- `string.contains(s, needle)` — return true if needle is found in s
- `string.repeat(s, n)` — repeat string n times

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
- `table.sort(arr[, cmp])` — sort array in-place. Without `cmp`, uses default ascending order. With `cmp`, uses `cmp(a, b)` returning negative/zero/positive. Wrapper around the `sort` builtin.
- `table.slice(arr, start, len)` — return new array with `len` elements starting at `start`. Negative `start` counts from end.
- `table.index_of(arr, val)` — return index of first occurrence of `val`, or -1 if not found
- `table.concat(a, b)` — return new array with elements of `a` followed by elements of `b`
- `table.copy(tbl)` — shallow copy of an array or string-keyed table. Mutations to the copy do not affect the original.
- `table.flat_map(arr, fn)` — map each element with `fn`, then flatten one level. If `fn` returns an array, its elements are inlined; scalars are kept as-is.
- `table.zip(a, b)` — return array of `[a[i], b[i]]` pairs, truncated to the shorter array
- `table.unique(arr)` — return new array with duplicate values removed (first occurrence kept). Uses `to_string` for identity comparison.
- `table.count(arr, fn)` — count elements where `fn(item)` is truthy
- `table.flatten(arr)` — flatten one level of nesting. Nested arrays are inlined; non-array elements are kept as-is. Empty arrays are dropped.
- `table.group_by(arr, fn)` — group elements by the string key returned by `fn(item)`. Returns a table mapping keys to arrays of matching elements.

`std/math` — exports `math` table:

- `math.min(a, b)` — return the smaller of two comparable values
- `math.max(a, b)` — return the larger of two comparable values
- `math.clamp(val, low, high)` — constrain `val` to the range `[low, high]`
- `math.assert(cond[, msg])` — raise an error if `cond` is falsy. Optional `msg` is included in the error message.

`std/test` — exports `test` table. Minimal harness for self-validating example programs. Cases register on import; nothing runs until `test.run()` is called.

- `test.case(name, fn)` — register a test case. `fn` takes no arguments.
- `test.assert(cond[, msg])` — raise an error if `cond` is falsy.
- `test.assert_eq(actual, expected)` / `test.assert_neq(actual, expected)` — equality / inequality assertion. On failure the error message includes both formatted values.
- `test.assert_throws(fn)` — assert that calling `fn()` raises an error.
- `test.run()` — run every registered case under `pcall`, print one-line `PASS <name>` or `FAIL <name>: <reason>` per case, then a `<n> passed / <m> failed` summary, and `exit(1)` if any failed.

The std versions are implemented in pure Gem using `ord()`, `chr()`, `buf_new()`/`buf_push()`/`to_string()`, and `substr()`. `split` and `index_of` are only available through `std/string` (not as bare builtins).

`std/json` — exports `json` table:

- `json.parse(s)` — parse a JSON string into Gem values. Objects become string-keyed tables, arrays become integer-keyed tables, strings/numbers/booleans map to native types, `null` maps to `nil`. Supports the full JSON spec: nested structures, all string escape sequences (`\"`, `\\`, `\/`, `\b`, `\f`, `\n`, `\r`, `\t`, `\uXXXX`), negative numbers, floats with decimal points and/or exponents (`1.5e-3`). Raises an error on malformed input — wrap with `pcall` for recovery.
- `json.encode(val)` — serialize a Gem value to a compact JSON string (no extra whitespace). Integer-keyed tables encode as JSON arrays, string-keyed tables as JSON objects, strings are escaped, numbers/booleans/nil map to their JSON equivalents. Raises an error for unencodable types (functions, buffers, refs). Note: empty tables (`{}` / `[]`) are indistinguishable in Gem and always encode as `[]`.

`std/mime` — exports `mime` table:

- `mime.lookup(path_or_ext)` — returns the MIME type for a file path or extension. Accepts `"index.html"`, `".html"`, or `"html"`. Returns `"application/octet-stream"` for unknown extensions. Text types (`text/*`, `application/json`, `application/javascript`, `application/xml`) automatically include `; charset=utf-8`. Case-insensitive.
- `mime.ext(content_type)` — reverse lookup: `"text/html"` → `".html"`. Strips `; charset=...` before matching. Returns `nil` for unknown types.

Coverage: html, htm, css, js, mjs, json, xml, txt, csv, png, jpg, jpeg, gif, svg, ico, webp, avif, woff, woff2, ttf, otf, pdf, zip, gz, mp3, mp4, webm, wasm.

`std/url` — exports `url` table:

- `url.encode(str)` — percent-encode per RFC 3986. Unreserved chars (`A-Za-z0-9-_.~`) pass through, everything else becomes `%XX` (uppercase hex).
- `url.decode(str)` — percent-decode `%XX` sequences. `+` is decoded as space.
- `url.parse_query(str)` — parse a query string like `"a=1&b=hello+world&c=%2F"` into a table `{a: "1", b: "hello world", c: "/"}`. Decodes both keys and values. Duplicate keys: last value wins.
- `url.build_query(table)` — build a query string from a table. Encodes both keys and values.
- `url.parse(path_with_query)` — split a path on the first `?`. Returns `{path: "/users/123", query_string: "q=foo&page=2", query: {q: "foo", page: "2"}}`. If no `?`, `query_string` is `""` and `query` is `{}`.

`std/time` — exports `time` table:

- `time.now()` — returns the current wall-clock time as milliseconds since the Unix epoch (int). Alias for `epoch_ms()`.
- `time.format(ms, fmt)` — format epoch milliseconds as a UTC string using `strftime` specifiers. Wraps `format_time`.
- `time.format_local(ms, fmt)` — format epoch milliseconds as a local timezone string. Wraps `format_time_local`.
- `time.http_date(ms?)` — RFC 7231 format: `"Tue, 28 Apr 2026 14:30:00 GMT"`. Uses current time if `ms` is omitted.
- `time.iso8601(ms?)` — ISO 8601 format: `"2026-04-28T14:30:00Z"`. Uses current time if `ms` is omitted.
- `time.date(ms?)` — calendar date: `"2026-04-28"`. Uses current time if `ms` is omitted.

`std/log` — exports `log` table. Structured logging to stderr. Depends on `std/time`.

- `log.debug(msg)`, `log.info(msg)`, `log.warn(msg)`, `log.error(msg)` — log at the given level. Output format: `2026-04-28T14:30:00Z [INFO] message`. One line per call, written to stderr via `eprint`.
- `log.set_level(level)` — set minimum log level. One of `"debug"`, `"info"`, `"warn"`, `"error"`. Default: `"info"`. Messages below the level are silently dropped. The level is module state, so it is per process: set it before spawning (or before `http.start`), or call it in the process that logs.

`std/sqlite` — exports `sqlite` table. Thin wrapper over the SQLite C builtins.

- `sqlite.open(path)` — opens (or creates) a SQLite database. Returns an opaque db handle. Use `":memory:"` for in-memory.
- `sqlite.close(db)` — closes the database handle.
- `sqlite.exec(db, sql)` — executes SQL that returns no rows (DDL, mutations without RETURNING).
- `sqlite.query(db, sql, params?)` — executes a parameterized query. `params` defaults to `[]`. Returns an array of row tables. Use `?` placeholders for bind parameters.
- `sqlite.last_id(db)` — returns the last inserted row ID.
- `sqlite.changes(db)` — returns the number of rows affected by the last mutation.

`std/http` — exports `http` table. HTTP/1.1 server with routing and keep-alive. Depends on `std/string`, `std/url`, `std/mime`, `std/json`, `std/time`.

### Response builders

- `http.response(status, headers, body)` — generic response constructor. Returns `{status, headers, body}`.
- `http.ok(body[, content_type])` — 200 response. Default content type: `"text/plain; charset=utf-8"`.
- `http.html(body)` — 200 with `text/html; charset=utf-8`.
- `http.json_response(data)` — 200 with `application/json`. Auto-encodes `data` via `json.encode`.
- `http.redirect(url[, status])` — redirect response. Default status: 302.
- `http.not_found([body])` — 404. Default body: `"Not Found"`.
- `http.bad_request([body])` — 400. Default body: `"Bad Request"`.
- `http.server_error([body])` — 500. Default body: `"Internal Server Error"`.

### Router

- `http.router()` — returns a router table with methods:
  - `router.get(pattern, handler)`, `router.post(...)`, `router.put(...)`, `router.patch(...)`, `router.delete(...)` — register a route. Handler signature: `fn(req) → response table`.
  - `router.static(url_prefix, dir_path)` — serve static files. Uses `std/mime` for content types. Prevents path traversal by normalizing paths and checking that they don't escape `dir_path`.
- Route patterns: segments starting with `:` are parameters. `/users/:id` matches `/users/123` and sets `req.params.id = "123"`. First registered route wins.
- Handler receives a request table: `{method, path, params, query, headers, body, cookies}`. `params` are extracted from route pattern, `query` is parsed from query string via `url.parse_query`, `cookies` are parsed from the `Cookie` header.

### Cookies

- `http.set_cookie(resp, name, value[, opts])` — returns new response with `Set-Cookie` header. `opts` table supports: `path` (default `"/"`), `http_only` (default `true`), `secure` (default `false`), `same_site` (default `"Lax"`), `max_age` (seconds), `expires` (epoch ms).
- `http.delete_cookie(resp, name[, opts])` — sets cookie with `Max-Age=0`.

### Form parsing

- `http.parse_form(body)` — parses a URL-encoded form body (`key=value&key=value`) into a table. Convenience wrapper around `url.parse_query(body)` — form-encoded bodies use the same percent-encoded format as query strings.

### Security

- `http.html_escape(str)` — escapes `&`, `<`, `>`, `"`, `'` to HTML entities. Use when interpolating user data into HTML.

### Server

- `http.serve(router[, opts])` — starts the HTTP server and blocks the caller. `opts` table: `port` (default 8080), `host` (default `"0.0.0.0"`). Meant to block until the acceptor process dies; currently it also returns when the caller receives any other message (see `docs/KNOWN_BUGS.md`).
- `http.start(router[, opts])` — starts the HTTP server without blocking. Returns the acceptor pid. Same options as `serve`.
- Spawns one process per connection. Supports HTTP/1.1 keep-alive with a 30-second idle timeout (via `tcp_read` timeout). Handles `Connection: close`. Errors in handlers are caught by `pcall` and return 500.

`std/request` — exports `request` table. HTTP/1.1 client for outbound requests. Depends on `std/string`, TCP builtins.

- `request.get(url[, opts])` — GET request. Returns `{status, headers, body}`.
- `request.post(url[, opts])` — POST request. `opts` supports `body` (string) and `headers` (table).
- `request.put(url[, opts])` — PUT request. Same opts as post.
- `request.patch(url[, opts])` — PATCH request. Same opts as post.
- `request.delete(url[, opts])` — DELETE request. Same opts as get.
- `request.request(method, url[, opts])` — generic method for full control.
- URL format: `http://host[:port]/path[?query]`. Default port 80. HTTP only (no TLS).
- Sends `Connection: close` — no keep-alive. Reads response until EOF.

`std/task` — exports `task` table. Runs a function in its own process and waits for its result; the simplest way to do several things at once. Load with `load "std/task"`.

- `task.async(f)` — spawns a process that calls `f()` and returns a task handle `{pid, ref, owner, done}`.
- `task.await(t)` / `task.await(t, timeout_ms)` — waits for the task and returns `f`'s value. If `f` raised an error, `await` raises the same message in the caller. On timeout, the task is killed and `await` raises `"task.await timeout"`. If the task process is killed before it produces a result, `await` raises `"task exited: <reason>"`; for reason `"normal"` the message instead says the task exited before its result was received and that a catch-all receive may have taken the result. With no timeout, `await` waits until the task finishes.
- `task.await_all(tasks)` / `task.await_all(tasks, timeout_ms)` — awaits every task and returns their values in the order of `tasks`. The timeout covers the whole call. If any task fails or the timeout expires, the tasks not yet awaited are killed and the error is raised.

Rules:
- Only the process that called `async` can await the task, and each task can be awaited once. Breaking either rule — including passing the same task to `await_all` twice — raises an error.
- A task whose function sets `process_flag("trap_exit", true)` is not killed by a timeout or an `await_all` failure: it runs on, and its result and `DOWN` later arrive in the owner's mailbox.
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

- `supervisor.start(spec)` — start a supervisor process. Returns `{pid: <pid>}`. The spec table supports:
  - `strategy` — `"one_for_one"` (default): restart only the crashed child. `"one_for_all"`: stop all children and restart all in order.
  - `children` — array of child specs: `{id: <string>, start: <fn>, restart: <string>}`. The `start` function must spawn and return a pid. `restart` is `"permanent"` (always restart, default), `"temporary"` (never restart), or `"transient"` (restart only on abnormal exit).
  - `max_restarts` — max restarts within the time window before the supervisor itself crashes (default 3).
  - `max_seconds` — time window in milliseconds for restart intensity (default 5000).
  - `name` — optional string name to register the supervisor process.
- `supervisor.which_children(pid_or_name)` — query a supervisor for its children. Returns an array of `{id, pid, restart}` tables. Takes a pid or a registered name, not the `{pid}` table `start` returns (pass `handle.pid`). Waits with no timeout.

`std/dynamic_supervisor` — exports `dynamic_supervisor` table. For workloads where children are spawned on demand from a single template (worker pools, per-connection processes, lazily-created topic actors), instead of being declared up front. Restart policies and intensity (`max_restarts` / `max_seconds`) match `std/supervisor`. Strategy is fixed at one-for-one; failed children restart with their original `args`.

- `dynamic_supervisor.start({child, max_restarts, max_seconds, name})` — start a dynamic supervisor. Returns `{pid: <pid>}`. The `child` template is `{start: <fn>, restart: <string>}` where `start` takes one argument (the per-child args, packed into a table or any value the caller chose) and must spawn-and-return a pid; `restart` is `"permanent"` (default), `"transient"`, or `"temporary"`.
- `dynamic_supervisor.start_child(target, args)` — ask the supervisor to spawn a child by calling `template.start(args)`. The supervisor monitors the child and stores `args` so it can replay the start on crash. Returns the new child pid.
- `dynamic_supervisor.terminate_child(target, pid)` — stop the named child cleanly (`kill(pid, "shutdown")`) and remove it from the supervisor's child set. Returns `true` if the child was known, `false` otherwise.
- `dynamic_supervisor.which_children(target)` — returns an array of `{pid, args}` tables, one per live child.

`std/gen_server` — exports `gen_server` table. OTP-style synchronous server abstraction. Load with `load "std/gen_server"`.

The caller provides a module table with callback functions: `init`, `handle_call`, `handle_cast`, and `handle_info`.

- `gen_server.start(module)` — starts a gen_server process running the given module. Returns `{pid: <pid>}`. The module's `init()` is called to produce the initial state. `init` may return a bare value (used as initial state) or `{state: <s>, timeout: <ms>}` to set an initial idle timeout.
- `gen_server.call(target, msg)` / `gen_server.call(target, msg, timeout_ms)` — sends a synchronous request and waits for a reply. Default timeout is 5000ms. Internally sends `{tag: "call", from: {pid, ref}, msg: <msg>}` to the server, then blocks waiting for a matching `{tag: "gs_reply", ref: <ref>, value: <value>}`. Returns the reply value. Errors on timeout.
- `gen_server.cast(target, msg)` — sends an asynchronous (fire-and-forget) message. Sends `{tag: "cast", msg: <msg>}` to the server. Returns `nil`.
- `gen_server.reply(from, value)` — sends a reply to a pending `call`. Used for deferred replies when `handle_call` returns `{noreply: state}` instead of replying immediately. Returns `nil`.

Module callback return values:

- `handle_call(msg, from, state)` — must return `{reply: <value>, state: <new_state>}` to reply immediately, or `{noreply: <new_state>}` to defer the reply (use `gen_server.reply(from, value)` later). May include `timeout: <ms>` to schedule an idle timeout.
- `handle_cast(msg, state)` — must return `{state: <new_state>}`. May include `timeout: <ms>`. A callback that returns anything else (a `match` with no matching arm returns `nil`) crashes the server, so give its `match` an `else`.
- `handle_info(msg, state)` — handles any message not from `call`/`cast` (e.g. DOWN messages, EXIT messages, `"timeout"`). Must return `{state: <new_state>}`. May include `timeout: <ms>`.

When any callback returns a `timeout` field, a timer is scheduled: after `timeout` milliseconds with no other message, `handle_info` is called with the string `"timeout"` as the message. Each new timeout cancels the previous pending one. Omitting `timeout` (or setting it to `nil`) cancels any pending timeout without scheduling a new one.

```
load "std/gen_server"

let counter_mod = {
  init: fn()
    0
  end,
  handle_call: fn(msg, from, state)
    match msg
    when "get"
      {reply: state, state: state}
    when "inc"
      {reply: state + 1, state: state + 1}
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

let server = gen_server.start(counter_mod).pid
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
