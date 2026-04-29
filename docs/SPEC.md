**Gem Language Spec v0.1**

_(working name, obviously)_

---

**Implementation Strategy**

The compiler is self-hosting — written in Gem, compiling itself. A checked-in C file (`bootstrap/stage0.c`) serves as the bootstrap artifact so any C compiler can rebuild from scratch. No Python, no external parser generators.

Compilation target is C source code. `gcc`/`clang` handles optimization and linking. This gives us free C interop since we're already in native land.

C runtime is minimal glue code (~100–200 lines) wiring together three libraries:

- **Boehm GC** — conservative garbage collector, use `GC_malloc` instead of `malloc`, never free anything
- **minicoro** — single-header stackful coroutines (libdill is abandoned, crashes on arm64 macOS — see `prototype/`). Runtime builds scheduler + channels on top (~150 lines)
- **stb_ds.h** — single-header hash maps and dynamic arrays for table implementation

**Boehm + coroutine integration: prototyped and working.** See `prototype/gc_coro_test.c`. The trick: provide minicoro a custom stack allocator that calls `malloc()` + `GC_add_roots()`, and on dealloc calls `GC_remove_roots()` + `free()`. This lets Boehm scan coroutine stacks for GC roots. Tested with 50 concurrent coroutines under heavy GC pressure.

**Bootstrap Path** (all complete)

1. ~~Prototype Boehm + coroutine integration~~ — done, see `prototype/`
2. ~~Python bootstrap compiler~~ — done, deleted
3. ~~C runtime glue~~ — done, `runtime/gem.c`
4. ~~Write the real Gem compiler in Gem~~ — done, `compiler/`
5. ~~New compiler compiles itself~~ — done, fixed-point verified
6. ~~Delete the Python code forever~~ — done

---

**Compiler CLI**

```
gem <file.gem>              # compile to ./<basename> (e.g. foo.gem → ./foo)
gem <file.gem> -o <name>    # compile to <name>
gem <file.gem> --emit-c     # print generated C to stdout (used for bootstrapping)
gem <file.gem> --run        # compile to a temp binary and run it immediately
gem <file.gem> --run [args] # compile and run, passing args through to the program
```

The default behavior (`gem foo.gem`) writes generated C to `/tmp/gem_<basename>.c`, invokes `cc` to compile it to an executable, and produces `./<basename>`. GC flags are sourced from `pkg-config --cflags --libs bdw-gc` if available, falling back to `-lgc`.

`--emit-c` is the current-behavior flag — it prints C to stdout and exits. This is what the bootstrapping Makefile targets use.

`--run` compiles to a temp file and immediately executes it, forwarding any additional arguments. The temp executable is left on disk after the run.

---

**Values and Types**

Nine types: `Int`, `Float`, `String`, `Bool`, `Nil`, `Table`, `Fn`, `Buffer`, `Ref`. All dynamically typed. Every value is a tagged C union. Yes this means primitives are boxed and slow — doesn't matter for v0. Future optimization: NaN-boxing to pack ints, bools, and nil into a double's NaN space, eliminating heap allocation for primitives.

**Variables**

```
let x = 10
let name = "hello"
x = 20
```

Destructuring extracts fields from tables or elements from arrays:

```
# Table destructuring — extract named fields
let {method, path} = parse_request(raw)

# Array destructuring — extract by position
let [first, second] = string.split(line, ",")
```

Table destructuring extracts by name (`let {a, b} = expr` is `let a = expr.a; let b = expr.b`). Array destructuring extracts by index (`let [a, b] = expr` is `let a = expr[0]; let b = expr[1]`). The RHS is evaluated exactly once. Missing keys/indices produce `nil`. No renaming, nesting, rest/splat, or default values.

**Functions**

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

**Variadic Functions**

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

**Default Parameters**

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

Default parameters work in named functions, anonymous functions, and block parameters. Passing `nil` explicitly does *not* trigger the default — only omitting the argument does. Functions with default parameters are not eligible for tail call optimization.

**Blocks**

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

This one rule lets std define `each`, `map`, `unless`, `until`, `with_file`, `loop`, etc. without compiler changes.

**Tables**

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

**Control Flow**

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
```

**Destructuring Patterns in Match**

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
```

Pattern rules:
- `{key: pattern, ...}` — checks target is a table, each key exists, and recursively matches each value against its sub-pattern. Extra keys in the target are ignored (partial match).
- `[p1, p2, ...]` — checks target is a table with `len(target) == N`, then recursively matches each element.
- A literal (int, float, string, bool) in pattern position matches by equality. `nil` is also a literal — `when nil` matches only `nil`, it does not bind a variable.
- A name in pattern position is a variable binding — always matches and binds the matched value.
- A bare name after `when` (e.g., `when x`) is a catch-all that binds the entire match target.
- Patterns compose recursively: `{users: [{name: n}]}` works.
- Regular expression whens (e.g., `when some_var + 1`) still work alongside destructuring patterns.

Destructuring patterns desugar in the parser to condition checks + variable bindings. The codegen sees normal if/else chains with let statements — no new AST node types are needed.

`elif` desugars to nested `if/else` at parse time — no new AST nodes. One `end` closes the entire chain.

`break` exits the innermost loop (`while` or `for`). `continue` skips to the next iteration.

**`break`, `continue`, and `return` inside blocks:** A `do`/`end` block is a closure (anonymous function). `return` inside a block returns from the block, not from the enclosing function — the iteration function receives the return value as the result of calling the block. `break` and `continue` inside a block only affect loops *within* the block itself; they cannot break or continue a loop in the caller.

**For Loops**

Three forms, all desugared to `while` at parse time:

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

The table form evaluates the RHS expression exactly once. It desugars to: store the table in a temp, call `keys()` on it, iterate by index, and look up each value by key inside the loop.

**Closures**

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

**Tail Call Optimization**

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

TCO applies to named functions (`fn name(...)`) without rest, default, or block parameters. Anonymous functions and mutual recursion are not optimized. Non-tail calls (where the result is used in a further expression, e.g. `n * f(n-1)`) remain normal recursive calls.

**Important:** only direct self-recursion is optimized. If `fn A` calls `fn B` which calls `fn A`, neither call is a TCO candidate — both grow the stack. Write long-running loops as direct self-recursion or use `while`. Splitting a loop body into helper functions that recurse back to the main loop will leak stack frames under sustained load.

**Green Threads and Message Passing**

```
let pid = spawn do
  loop do
    let msg = receive()
    print("got: " + msg)
  end
end

send(pid, "hello")
send(pid, "world")
```

`spawn`, `send`, `receive` are runtime functions, not keywords. Under the hood they use minicoro coroutines with a round-robin scheduler. Each spawned coroutine gets a mailbox (a simple queue). `receive` yields the coroutine if the mailbox is empty; the scheduler resumes it when a message arrives via `send`.

The main process (top-level code) is itself a schedulable coroutine (PID 0). All concurrency primitives — `self()`, `send`, `receive`, `sleep`, `monitor`, `link` — work at the top level. The program exits when all processes have terminated; if spawned processes outlive main, the program continues running until they complete.

**Preemptive Scheduling (Reduction-Based)**

Processes are cooperatively scheduled but the compiler inserts automatic yield points so tight loops cannot starve the scheduler. Each process has a reduction counter that increments at loop back-edges (the top of every `while` body, including `for` loops which desugar to `while`). When the counter exceeds the threshold (currently 4000), the process yields and is immediately re-queued as READY. The counter resets to 0 each time the scheduler resumes a process.

Yield checks are only inserted in user-written loops. Compiler-generated loops (e.g. the mailbox scan in selective receive) do not get yield checks, since yielding mid-scan would break the selective receive contract.

The yield check is a no-op when running outside a spawned process (top-level code before the scheduler starts).

**Process Monitoring**

```
let child = spawn do
  error("crash")
end
monitor(child)
let msg = receive()
# msg == {tag: "DOWN", pid: <child_pid>, reason: "crash"}
```

`monitor(pid)` registers the caller as a monitor of the target. When the target exits (normally or via error), a `DOWN` message is delivered to each monitor's mailbox:

- `{tag: "DOWN", pid: <target_pid>, reason: "normal"}` — clean exit
- `{tag: "DOWN", pid: <target_pid>, reason: "<error message>"}` — crash

Monitoring a dead/invalid pid delivers the DOWN message immediately. Duplicate monitors are deduplicated. Returns `true`.

`spawn_monitor(fn)` atomically spawns and monitors a process. Returns `{pid: <pid>}`.

Error isolation: a crashing spawned process does not kill other processes. The error is captured, DOWN messages are delivered, and the scheduler continues. `pcall` inside a monitored process still works — it catches errors locally before the process-level handler.

**Links**

Links are bidirectional process monitors. When two processes are linked, an abnormal exit in one propagates to the other.

```
let pid = spawn_link do
  error("crash")
end

# caller dies too, unless trap_exit is set
```

`link(pid)` creates a bidirectional link between the calling process and the target. `unlink(pid)` removes it. `spawn_link(fn)` atomically spawns and links (no race window between spawn and link).

When a linked process dies with a reason other than `"normal"`, the exit signal propagates to all linked processes:

- If the linked process has `trap_exit` set to `true` (via `process_flag("trap_exit", true)`), the exit signal is converted to a message: `{tag: "EXIT", pid: <pid>, reason: <reason>}`.
- If the linked process does not trap exits, it dies with the same reason, propagating the signal further.
- Normal exits (reason `"normal"`) do not propagate to non-trapping linked processes.

Propagation is transitive: if A is linked to B and B is linked to C, and C crashes, B dies (unless trapping), which causes A to die (unless trapping). The implementation is cycle-safe.

```
# Trapping exits turns death signals into messages
process_flag("trap_exit", true)
let pid = spawn_link do
  error("boom")
end

receive
when {tag: "EXIT", pid: p, reason: r}
  print("child " + to_string(p) + " died: " + r)
end
```

`process_flag("trap_exit", bool)` sets the trap_exit flag on the current process and returns the previous value.

**Named Processes**

```
let pid = spawn do
  register("worker", self())
  let msg = receive()
  print(msg)
end

send("worker", "hello")    # send by name
let p = whereis("worker")  # returns pid or nil
```

`register(name, pid)` associates a string name with a pid. Errors if the name is already taken. `whereis(name)` returns the pid for a name, or `nil` if not registered. `send` accepts either a pid (int) or a registered name (string). When a process dies, its name is automatically unregistered.

`send` edge cases: sending to a dead pid silently drops the message. Sending to a registered name that does not exist raises an error.

**Selective Receive**

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

The `after <ms>` clause is optional. If present and the timeout elapses with no matching message, the `after` body executes. `after 0` means "check once, don't block." Omitting `after` means block forever (like `receive()`).

The `receive` block can produce a value when used as the last statement of a function (implicit return), just like `match`.

The existing `receive()` function call continues to work unchanged — it always pops the head of the mailbox unconditionally.

**Process Control**

`kill(pid, reason)` sends an exit signal to a process. If the target has `trap_exit` enabled, an `{tag: "EXIT", pid: sender_pid, reason: reason}` message is delivered to its mailbox instead of terminating it. Otherwise, the process is terminated immediately: marked dead, DOWN messages delivered to monitors, registered name removed, and exit propagated to linked processes. Returns `true` if the process was alive, `nil` otherwise.

`sleep(ms)` suspends the current process for `ms` milliseconds. The scheduler resumes the process after the deadline expires.

`time_ms()` returns the current monotonic time in milliseconds (int). Useful for timeouts, benchmarks, and restart intensity tracking. Not suitable for wall-clock formatting — use `epoch_ms()` for that.

`epoch_ms()` returns the current wall-clock time as milliseconds since the Unix epoch (int). Use this for timestamps that need to be formatted into dates or times. Uses `gettimeofday` internally.

`format_time(epoch_ms, format_str)` formats a wall-clock epoch millisecond timestamp into a UTC string using `strftime` specifiers. Returns a string. Supported specifiers include `%Y` (4-digit year), `%m` (month 01-12), `%d` (day 01-31), `%H` (hour 00-23), `%M` (minute 00-59), `%S` (second 00-59), `%a` (abbreviated weekday), `%b` (abbreviated month), `%T` (`%H:%M:%S`), `%F` (`%Y-%m-%d`), and all other platform-supported `strftime` specifiers.

`format_time_local(epoch_ms, format_str)` — same as `format_time` but formats in the local timezone instead of UTC.

**Timers**

```
let ref = send_after(pid, "tick", 1000)
cancel_timer(ref)
```

`send_after(pid, msg, delay_ms)` schedules `msg` to be delivered to `pid` after `delay_ms` milliseconds. Returns a unique ref identifying the timer. The timer fires from the scheduler loop, not from the calling process, so it works even if the caller is blocked. If the target process is dead when the timer fires, the message is silently dropped.

`cancel_timer(ref)` cancels a pending timer by ref. Returns `true` if cancelled, `false` if already fired or not found.

Timers are stored in a global fixed-size array (256 slots). An error is raised if the array is full.

**Process Introspection**

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

**C Interop**

```
extern fn puts(s: String) -> Int
extern fn sqrt(x: Float) -> Float
extern fn fopen(path: String, mode: String) -> Ptr

puts("hello from C")
```

`extern fn` declares a C function. The compiler emits the call directly since we compile to C. Type annotations on extern declarations only — the rest of the language stays dynamically typed. `Ptr` is an opaque type for C pointers.

The compiler auto-generates C forward declarations from `extern fn` type signatures, so no separate `.h` file is needed for function declarations. The type mapping: `Int` → `int64_t`, `Float` → `double`, `String` → `const char*` (params) / `char*` (returns), `Bool` → `int`, `Ptr` → `void*`, `Nil` → `void`, `Table` → `GemVal`. Auto-generation is skipped when `extern include` is present (the user manages declarations via headers).

`extern blocking fn` declares a C function that may block (e.g. socket I/O, database calls, DNS lookups). When called from a coroutine (`spawn`), the call is submitted to a thread pool so the scheduler can continue running other coroutines. When called outside a coroutine, it calls directly (synchronous). The C function runs on a worker thread and must not call `GC_MALLOC` — for `String` returns, use `malloc`/`strdup` (the runtime copies to GC memory and frees the original).

```
extern blocking fn net_accept(server_fd: Int) -> Int
extern blocking fn net_read(fd: Int, max_bytes: Int) -> String
```

For C headers (structs, typedefs, system headers the compiler can't infer):

```
extern include "math.h"
extern include "stdio.h"
```

**Operators**

`+` for both arithmetic and string concatenation. If types don't match (e.g. string + int), runtime error. No `..` operator — keep it simple.

`+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `and`, `or`, `not`, `in`

`x in tbl` — membership test. For arrays (integer-indexed tables with no string keys): returns `true` if `x` equals any value in the array (linear scan). For string-keyed tables: returns `true` if `x` is a key in the table (same as `has_key(tbl, x)`). Precedence is at the comparison level (same as `==`, `<`, etc.).

**Equality semantics:** `==` compares by value for primitives (int, float, string, bool, nil) and by identity (reference) for tables, functions, and refs. Two distinct tables with identical contents are not equal: `{a: 1} == {a: 1}` is `false`. The same table reference compared to itself is `true`.

**Assignment Operators**

`=`, `+=`, `-=`, `*=`, `/=`

**Strings**

Two quote styles: double-quoted strings support interpolation, single-quoted strings do not.

```
let s = "hello"
let t = 'hello'
s.len()
s[0]
s + " world"
```

Both styles support escape sequences: `\n`, `\r`, `\t`, `\0`, `\\`, and the matching quote (`\"` or `\'`). `\0` produces a null byte (0x00). Double-quoted strings also support `\{` and `\}` to escape interpolation braces.

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

**Multi-line Strings**

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
- **Dedent**: the indentation of the closing delimiter (number of leading spaces) is the base indentation. That many leading spaces are stripped from every content line. Extra indentation beyond the base is preserved.
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

**String Interpolation**

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
```

**Nil and Truthiness**

`nil` and `false` are falsy, everything else truthy.

**Comments**

```
# single line comment
```

**Error Handling**

`error(msg)` prints the message with file and line info to stderr, followed by a call stack trace showing each Gem function frame, and halts (`exit(1)`). Runtime type errors (e.g. `1 + "a"`) also print a stack trace with the actual types involved (e.g. `type error in +: got string and int`). The compiler reports the first error and stops.

**Inside spawned processes**, `error()` does not terminate the program. Each spawned process has an implicit error boundary — if an unhandled error occurs, the process dies but other processes continue. The error is captured, DOWN messages are delivered to monitors, EXIT signals propagate to linked processes, and the scheduler continues. `pcall` inside a spawned process still works — it catches errors locally before the process-level boundary. See Process Monitoring for details.

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
- On error: returns `{ok: false, error: <error message string>, stack: <array of {name, file, line} tables>}`
- Errors caught by `pcall` do not print to stderr and do not call `exit(1)`
- If no `pcall` is active, errors behave as before (print + stack trace + exit)
- `pcall` catches both user `error()` calls and runtime type errors (e.g. `1 + "hello"`)
- Nested `pcall` works — each level catches errors independently
- To pass arguments to the called function, use a closure: `pcall(fn() f(x, y) end)`

**Built-in Functions**

`print(args...)` — prints values separated by spaces, followed by a newline.

`error(msg)` — prints the message with file and line info to stderr, prints a call stack trace, and halts (`exit(1)`).

`len(v)` — returns the length of a string or the total number of entries in a table (both integer-keyed and string-keyed). `len({a: 1, b: 2})` returns 2. `len([10, 20, 30])` returns 3.

`type(v)` — returns the type name as a string: `"int"`, `"float"`, `"string"`, `"bool"`, `"nil"`, `"table"`, `"fn"`, `"ref"`.

`to_string(v)` — converts any value to its string representation.

`to_int(v)` — converts a value to an integer. Strings are parsed as decimal integers. Floats are truncated. Bools become 0/1. Errors on nil, tables, functions, or unparseable strings.

`to_float(v)` — converts a value to a float. Strings are parsed as decimal floats. Ints are widened. Bools become 0.0/1.0. Errors on nil, tables, functions, or unparseable strings.

`push(arr, val)` — appends `val` to the table at the next integer index. Mutates the table in place. Returns the pushed value.

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

`substr(s, start[, len])` — returns a substring of `s` starting at `start`. If `len` is provided, returns at most `len` characters; otherwise returns to the end of the string.

`chr(n)` — converts an integer (0–255) to a single-character string with that byte value.

`ord(s)` — returns the byte value of the first character of string `s` as an integer.

`ord(s, i)` — returns the byte value at index `i` in string `s` without allocating a temporary string. Equivalent to `ord(s[i])` but avoids the 1-char string allocation.

`buf_new()` — creates a new mutable string buffer. Returns a buffer value (type `"buffer"`).

`buf_push(buf, val)` — appends `val` to the buffer. Non-string values are auto-coerced to strings. Returns the buffer for chaining. Uses a doubling growth strategy internally — O(n) total for n appends vs O(n²) for repeated `+` concatenation.

`buf_str(buf)` — finalizes the buffer into an immutable string. The buffer can still be used after this call.

`make_ref()` — returns a unique opaque reference value. Type is `"ref"`. Refs are equal only to themselves (identity equality). Usable as table keys. Format: `#Ref<N>` where N is a monotonically increasing integer.

`link(pid)` — creates a bidirectional link between the calling process and the target process. If the target is already dead with a non-normal reason and the caller does not trap exits, the caller dies. Returns `true`.

`unlink(pid)` — removes the bidirectional link between the calling process and the target. Returns `true`.

`spawn_link(fn)` — atomically spawns a new process and links it to the caller. No race window between spawn and link. Returns the new pid.

`process_flag("trap_exit", bool)` — sets the `trap_exit` flag on the current process. Returns the previous value (bool). When `trap_exit` is `true`, exit signals from linked processes are converted to `{tag: "EXIT", pid: <pid>, reason: <reason>}` messages in the process's mailbox instead of killing the process.

`sleep(ms)` — suspends the current process for `ms` milliseconds. Must be called inside a spawned process.

`send_after(pid, msg, delay_ms)` — schedules `msg` to be delivered to `pid` after `delay_ms` milliseconds. Returns a unique ref identifying the timer. The timer fires from the scheduler, not the calling process. Messages to dead processes are silently dropped.

`cancel_timer(ref)` — cancels a pending timer by ref. Returns `true` if cancelled, `false` if already fired or not found.

`processes()` — returns a list of all live process pids (excludes free and dead processes).

`process_info(pid)` — returns a table with process metadata: `state`, `mailbox_len`, `links`, `monitors`, `trap_exit`, `exit_reason`. Returns `nil` for invalid/free pids.

`read_file(path)` — reads the entire file at `path` and returns its contents as a string. Opens in binary mode (no newline translation). Raises an error if the file cannot be opened.

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

`exec(command)` — runs `command` via the system shell (`sh -c`). Blocks until the command exits. Returns the exit code as an integer (0 on success). The shell expands glob patterns and environment variables in `command`. Output goes to the process's stdout/stderr unless redirected inside `command`.

**TCP Sockets**

`tcp_listen(host, port)` — creates a TCP server socket bound to `host` (string) on `port` (int). Calls `socket`, `bind`, and `listen` with a backlog of 128. Sets `SO_REUSEADDR`. Returns the socket file descriptor as an integer. Raises an error on failure. Always synchronous (fast).

`tcp_connect(host, port)` — opens a TCP connection to `host:port`. Returns the connected socket file descriptor as an integer. Raises an error if the connection fails or the host cannot be resolved. Supports both IP addresses and hostnames. When called from a spawned process, uses a non-blocking connect and yields to the scheduler until the connection completes.

`tcp_accept(socket)` — accepts an incoming connection on a listening socket. Returns the new connection's file descriptor as an integer. When called from a spawned process, yields to the scheduler on EAGAIN and resumes when a connection is ready. Raises an error on failure.

`tcp_read(socket[, max_bytes[, timeout_ms]])` — reads up to `max_bytes` bytes from a connected socket (default 4096). Returns the data as a string. Returns an empty string `""` when the remote end has closed the connection (EOF) or when the optional `timeout_ms` expires with no data available. When called from a spawned process, yields to the scheduler on EAGAIN and resumes when data is available or the timeout deadline is reached. The timeout is only supported from within spawned processes (the scheduler manages the deadline via the process's `deadline_ms` field).

`tcp_write(socket, data)` — writes the string `data` to a connected socket. Writes all bytes (loops internally on partial writes). Returns the number of bytes written as an integer. When called from a spawned process, yields to the scheduler on EAGAIN and resumes when the socket is writable.

`tcp_close(socket)` — closes a socket file descriptor. Always synchronous. Returns `nil`.

All TCP builtins use non-blocking sockets with scheduler poll integration. The scheduler's `poll()` loop handles readiness notification with zero thread pool overhead.

**SQLite**

`sqlite_open(path)` — opens (or creates) a SQLite database at `path`. Enables WAL mode and foreign keys by default. Returns an opaque database handle (stored as an int). Use `":memory:"` for an in-memory database. When called from a spawned process, runs on the thread pool (blocking fn). Raises on error.

`sqlite_close(db)` — closes the database handle. When called from a spawned process, runs on the thread pool (blocking fn). Returns `nil`.

`sqlite_exec(db, sql)` — executes SQL that returns no rows (DDL, INSERT without RETURNING, etc.). Inline execution (no thread pool). Raises on error.

`sqlite_query(db, sql, params)` — executes a parameterized query. `params` is an array of bind values matching `?` placeholders in `sql`. Returns an array of row tables, where each row is a string-keyed table (e.g., `{id: 1, name: "Alice"}`). Column type mapping: INTEGER → Int, REAL → Float, TEXT → String, NULL → Nil, BLOB → String (raw bytes). Inline execution (no thread pool). Raises on error.

`sqlite_last_insert_id(db)` — returns `sqlite3_last_insert_rowid` as an int.

`sqlite_changes(db)` — returns the number of rows affected by the last INSERT/UPDATE/DELETE as an int.

SQLite is vendored as an amalgamation (`runtime/sqlite3.c` + `runtime/sqlite3.h`), compiled into the runtime static library. No system dependency needed.

**Negative array indexing** — Integer indices to arrays and strings may be negative. A negative index `i` on a collection of length `n` resolves to `n + i`. So `arr[-1]` is the last element, `arr[-2]` is second-to-last, etc. Indices that remain out of bounds after resolution raise a runtime error.

All builtins are first-class values — they can be stored in variables and passed to functions.

**Module System**

**Load statement** — `load` brings another file's exported definitions into scope. Every loaded file must have an `export` statement declaring its public API. The compiler keeps a table of already-loaded file paths; re-importing a module skips re-parsing but still creates the requested import bindings.

```
load "compiler/parser"
load "std/table"
```

Two-step path resolution:

1. **Relative to the importing file** — `load "parser"` from `compiler/main.gem` finds `compiler/parser.gem`.
2. **Relative to the install root** — if step 1 fails, try `<binary_dir>/../<path>.gem`. Since `build/gem` lives at `<project>/build/gem`, the install root is `<project>/`. So `load "std/string"` resolves to `<project>/std/string.gem` from any source file location.

The first path that exists wins. This lets `load "std/string"` work without fragile relative paths like `../../std/string`.

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

**Circular imports** are not detected — the compiler tracks loaded paths and reuses cached modules. If module A loads B and B loads A, the second load of A returns the cached (possibly incomplete) module. In practice this means circular dependencies may see missing exports. Avoid circular imports; restructure shared code into a third module.

**Standard Library (std/)**

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

The std versions are implemented in pure Gem using `ord()`, `chr()`, `buf_new()`/`buf_push()`/`buf_str()`, and `substr()`. `split` and `index_of` are only available through `std/string` (not as bare builtins).

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
- `time.http_date(ms?)` — RFC 7231 format: `"Mon, 28 Apr 2026 14:30:00 GMT"`. Uses current time if `ms` is omitted.
- `time.iso8601(ms?)` — ISO 8601 format: `"2026-04-28T14:30:00Z"`. Uses current time if `ms` is omitted.
- `time.date(ms?)` — calendar date: `"2026-04-28"`. Uses current time if `ms` is omitted.

`std/log` — exports `log` table. Structured logging to stderr. Depends on `std/time`.

- `log.debug(msg)`, `log.info(msg)`, `log.warn(msg)`, `log.error(msg)` — log at the given level. Output format: `2026-04-28T14:30:00Z [INFO] message`. One line per call, written to stderr via `eprint`.
- `log.set_level(level)` — set minimum log level. One of `"debug"`, `"info"`, `"warn"`, `"error"`. Default: `"info"`. Messages below the level are silently dropped.

`std/sqlite` — exports `sqlite` table. Thin wrapper over the SQLite C builtins.

- `sqlite.open(path)` — opens (or creates) a SQLite database. Returns an opaque db handle. Use `":memory:"` for in-memory.
- `sqlite.close(db)` — closes the database handle.
- `sqlite.exec(db, sql)` — executes SQL that returns no rows (DDL, mutations without RETURNING).
- `sqlite.query(db, sql, params?)` — executes a parameterized query. `params` defaults to `[]`. Returns an array of row tables. Use `?` placeholders for bind parameters.
- `sqlite.last_id(db)` — returns the last inserted row ID.
- `sqlite.changes(db)` — returns the number of rows affected by the last mutation.

`std/http` — exports `http` table. HTTP/1.1 server with routing and keep-alive. Depends on `std/string`, `std/url`, `std/mime`, `std/json`, `std/time`.

**Response builders:**

- `http.response(status, headers, body)` — generic response constructor. Returns `{status, headers, body}`.
- `http.ok(body[, content_type])` — 200 response. Default content type: `"text/plain; charset=utf-8"`.
- `http.html(body)` — 200 with `text/html; charset=utf-8`.
- `http.json_response(data)` — 200 with `application/json`. Auto-encodes `data` via `json.encode`.
- `http.redirect(url[, status])` — redirect response. Default status: 302.
- `http.not_found([body])` — 404. Default body: `"Not Found"`.
- `http.bad_request([body])` — 400. Default body: `"Bad Request"`.
- `http.server_error([body])` — 500. Default body: `"Internal Server Error"`.

**Router:**

- `http.router()` — returns a router table with methods:
  - `router.get(pattern, handler)`, `router.post(...)`, `router.put(...)`, `router.patch(...)`, `router.delete(...)` — register a route. Handler signature: `fn(req) → response table`.
  - `router.static(url_prefix, dir_path)` — serve static files. Uses `std/mime` for content types. Prevents path traversal by normalizing paths and checking that they don't escape `dir_path`.
- Route patterns: segments starting with `:` are parameters. `/users/:id` matches `/users/123` and sets `req.params.id = "123"`. First registered route wins.
- Handler receives a request table: `{method, path, params, query, headers, body, cookies}`. `params` are extracted from route pattern, `query` is parsed from query string via `url.parse_query`, `cookies` are parsed from the `Cookie` header.

**Cookies:**

- `http.set_cookie(resp, name, value[, opts])` — returns new response with `Set-Cookie` header. `opts` table supports: `path` (default `"/"`), `http_only` (default `true`), `secure` (default `false`), `same_site` (default `"Lax"`), `max_age` (seconds), `expires` (epoch ms).
- `http.delete_cookie(resp, name[, opts])` — sets cookie with `Max-Age=0`.

**Form parsing:**

- `http.parse_form(body)` — parses a URL-encoded form body (`key=value&key=value`) into a table. Convenience wrapper around `url.parse_query(body)` — form-encoded bodies use the same percent-encoded format as query strings.

**Security:**

- `http.html_escape(str)` — escapes `&`, `<`, `>`, `"`, `'` to HTML entities. Use when interpolating user data into HTML.

**Server:**

- `http.serve(router[, opts])` — starts the HTTP server and blocks the caller. `opts` table: `port` (default 8080), `host` (default `"0.0.0.0"`). Blocks until the acceptor process dies.
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

`std/supervisor` — exports `supervisor` table:

- `supervisor.start(spec)` — start a supervisor process. Returns `{pid: <pid>}`. The spec table supports:
  - `strategy` — `"one_for_one"` (default): restart only the crashed child. `"one_for_all"`: stop all children and restart all in order.
  - `children` — array of child specs: `{id: <string>, start: <fn>, restart: <string>}`. The `start` function must spawn and return a pid. `restart` is `"permanent"` (always restart, default), `"temporary"` (never restart), or `"transient"` (restart only on abnormal exit).
  - `max_restarts` — max restarts within the time window before the supervisor itself crashes (default 3).
  - `max_seconds` — time window in milliseconds for restart intensity (default 5000).
  - `name` — optional string name to register the supervisor process.
- `supervisor.which_children(pid_or_name)` — query a supervisor for its children. Returns an array of `{id, pid, restart}` tables. Must be called from a spawned process (uses `receive`).

`std/gen_server` — exports `gen_server` table. OTP-style synchronous server abstraction. Load with `load "std/gen_server"`.

The caller provides a module table with callback functions: `init`, `handle_call`, `handle_cast`, and `handle_info`.

- `gen_server.start(module)` — starts a gen_server process running the given module. Returns `{pid: <pid>}`. The module's `init()` is called to produce the initial state. `init` may return a bare value (used as initial state) or `{state: <s>, timeout: <ms>}` to set an initial idle timeout.
- `gen_server.call(target, msg)` / `gen_server.call(target, msg, timeout_ms)` — sends a synchronous request and waits for a reply. Default timeout is 5000ms. Internally sends `{tag: "call", from: {pid, ref}, msg: <msg>}` to the server, then blocks waiting for a matching `{tag: "gs_reply", ref: <ref>, value: <value>}`. Returns the reply value. Errors on timeout.
- `gen_server.cast(target, msg)` — sends an asynchronous (fire-and-forget) message. Sends `{tag: "cast", msg: <msg>}` to the server. Returns `nil`.
- `gen_server.reply(from, value)` — sends a reply to a pending `call`. Used for deferred replies when `handle_call` returns `{noreply: state}` instead of replying immediately. Returns `nil`.

Module callback return values:

- `handle_call(msg, from, state)` — must return `{reply: <value>, state: <new_state>}` to reply immediately, or `{noreply: <new_state>}` to defer the reply (use `gen_server.reply(from, value)` later). May include `timeout: <ms>` to schedule an idle timeout.
- `handle_cast(msg, state)` — must return `{state: <new_state>}`. May include `timeout: <ms>`.
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
    end
  end,
  handle_info: fn(msg, state)
    {state: state}
  end
}

let {pid: server} = gen_server.start(counter_mod)
gen_server.call(server, "inc")    # 1
gen_server.call(server, "inc")    # 2
gen_server.call(server, "get")    # 2
gen_server.cast(server, "reset")
```

Top-level `let` bindings (including std namespaces like `string`, `table`) compile to C globals, so they are accessible from named `fn` declarations, closures, and top-level code alike.

**What the Compiler Needs to Emit**

Every value is a tagged C union. The compiler emits C code that uses the runtime glue (~200–350 lines) which wires together Boehm GC, minicoro, and stb_ds.h. The generated C code calls `GC_malloc` for allocations (with coroutine stacks registered as GC roots via custom allocator), uses minicoro for coroutines with a built-in round-robin scheduler and mailbox channels, and stb_ds for table operations. The compiler emits C `#line` directives so that runtime errors report Gem source file and line numbers instead of C line numbers.

**What's NOT in v0**

No classes. No inheritance. No exceptions (use `error()` to halt, `pcall()` for recovery). No type system. No operator overloading.

All of those can be added later _in the language itself_ via blocks, or in a future compiler version compiled by v0.
