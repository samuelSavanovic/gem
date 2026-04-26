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

**Values and Types**

Eight types: `Int`, `Float`, `String`, `Bool`, `Nil`, `Table`, `Fn`, `Buffer`. All dynamically typed. Every value is a tagged C union. Yes this means primitives are boxed and slow — doesn't matter for v0. Future optimization: NaN-boxing to pack ints, bools, and nil into a double's NaN space, eliminating heap allocation for primitives.

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

A `...name` rest parameter collects extra positional arguments into an array. It must be the last declared parameter, optionally followed by one more parameter that receives the last argument (typically a do block).

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

When a single parameter follows `...name`, it receives the last argument. This is useful for capturing a do block explicitly:

```
fn each_with(context, ...items, body)
  for item in items
    body(context, item)
  end
end

each_with("prefix", "a", "b", "c") do |ctx, item|
  print("{ctx}_{item}")
end
```

Rest param packing happens inside the function body at the C level, so variadic functions also work when stored in a variable or passed as a callback — the packing is unconditional regardless of call form.

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
- A literal (int, float, string, bool, nil) in pattern position matches by equality.
- A name in pattern position is a variable binding — always matches and binds the matched value.
- A bare name after `when` (e.g., `when x`) is a catch-all that binds the entire match target.
- Patterns compose recursively: `{users: [{name: n}]}` works.
- Regular expression whens (e.g., `when some_var + 1`) still work alongside destructuring patterns.

Destructuring patterns desugar in the parser to condition checks + variable bindings. The codegen sees normal if/else chains with let statements — no new AST node types are needed.

`elif` desugars to nested `if/else` at parse time — no new AST nodes. One `end` closes the entire chain.

`break` exits the innermost loop (`while` or `for`). `continue` skips to the next iteration.

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

**C Interop**

```
extern fn puts(s: String) -> Int
extern fn sqrt(x: Float) -> Float
extern fn fopen(path: String, mode: String) -> Ptr

puts("hello from C")
```

`extern fn` declares a C function. The compiler emits the call directly since we compile to C. Type annotations on extern declarations only — the rest of the language stays dynamically typed. `Ptr` is an opaque type for C pointers.

The compiler auto-generates C forward declarations from `extern fn` type signatures, so no separate `.h` file is needed for function declarations. The type mapping: `Int` → `int64_t`, `Float` → `double`, `String` → `const char*` (params) / `char*` (returns), `Bool` → `int`, `Ptr` → `void*`, `Nil` → `void`, `Table` → `GemVal`. Auto-generation is skipped when `extern include` is present (the user manages declarations via headers).

For C headers (structs, typedefs, system headers the compiler can't infer):

```
extern include "math.h"
extern include "stdio.h"
```

**Operators**

`+` for both arithmetic and string concatenation. If types don't match (e.g. string + int), runtime error. No `..` operator — keep it simple.

`+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `and`, `or`, `not`, `in`

`key in tbl` — returns `true` if `key` exists in the table. Same as `has_key(tbl, key)`. Precedence is at the comparison level (same as `==`, `<`, etc.).

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

`error(msg)` prints the message with file and line info to stderr, followed by a call stack trace showing each Gem function frame, and halts (`exit(1)`). Runtime type errors (e.g. `1 + "a"`) also print a stack trace. The compiler reports the first error and stops.

**Recoverable errors** use `pcall(f)` (protected call), which calls `f()` with zero arguments and catches any error instead of halting:

```
let result = pcall(fn()
  error("boom")
end)
print(result.ok)      # false
print(result.error)   # boom

let result2 = pcall(fn()
  42
end)
print(result2.ok)     # true
print(result2.value)  # 42
```

- On success: returns `{ok: true, value: <return value>}`
- On error: returns `{ok: false, error: <error message string>, stack: <array of {name, file, line} tables>}`
- Errors caught by `pcall` do not print to stderr and do not call `exit(1)`
- If no `pcall` is active, errors behave as before (print + stack trace + exit)
- `pcall` catches both user `error()` calls and runtime type errors (e.g. `1 + "hello"`)
- Nested `pcall` works — each level catches errors independently
- To pass arguments to the called function, use a closure: `pcall(fn() f(x, y) end)`
- `pcall` is a regular builtin function, not a keyword or syntax construct

**Built-in Functions**

`print(args...)` — prints values separated by spaces, followed by a newline.

`error(msg)` — prints the message with file and line info to stderr, prints a call stack trace, and halts (`exit(1)`).

`len(v)` — returns the length of a string or the number of entries in a table.

`type(v)` — returns the type name as a string: `"int"`, `"float"`, `"string"`, `"bool"`, `"nil"`, `"table"`, `"fn"`.

`to_string(v)` — converts any value to its string representation.

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

All builtins are first-class values — they can be stored in variables and passed to functions.

**Module System (v0.1 — minimal)**

```
load "compiler/parser"
load "std/table"
```

File inclusion with a re-inclusion guard — the compiler keeps a table of already-loaded file paths. `load` checks before including. Prevents double-loading in multi-file projects like the compiler itself.

Two-step path resolution:

1. **Relative to the importing file** — `load "parser"` from `compiler/main.gem` finds `compiler/parser.gem`.
2. **Relative to the install root** — if step 1 fails, try `<binary_dir>/../<path>.gem`. Since `build/gem` lives at `<project>/build/gem`, the install root is `<project>/`. So `load "std/string"` resolves to `<project>/std/string.gem` from any source file location.

The first path that exists wins. This lets `load "std/string"` work without fragile relative paths like `../../std/string`.

No namespacing in v0. Everything is global.

**Standard Library (std/)**

Convention: each std file exports a single table as a namespace. No language changes needed — `load` dumps everything into scope, but the exported table acts as a namespace.

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

`std/table` — exports `table` table:

- `table.each(arr, fn)` — iterate calling fn(item)
- `table.map(arr, fn)` — return new array with fn(item) applied
- `table.filter(arr, fn)` — return new array where fn(item) is truthy
- `table.contains(arr, value)` — linear scan for value equality

The std versions are implemented in pure Gem using `ord()`, `chr()`, `buf_new()`/`buf_push()`/`buf_str()`, and `substr()`. `split` and `index_of` are only available through `std/string` (not as bare builtins).

Top-level `let` bindings (including std namespaces like `string`, `table`) compile to C globals, so they are accessible from named `fn` declarations, closures, and top-level code alike.

**What the Compiler Needs to Emit**

Every value is a tagged C union. The compiler emits C code that uses the runtime glue (~200–350 lines) which wires together Boehm GC, minicoro, and stb_ds.h. The generated C code calls `GC_malloc` for allocations (with coroutine stacks registered as GC roots via custom allocator), uses minicoro for coroutines with a built-in round-robin scheduler and mailbox channels, and stb_ds for table operations. The compiler emits C `#line` directives so that runtime errors report Gem source file and line numbers instead of C line numbers.

**What's NOT in v0**

No classes. No inheritance. No exceptions (use `error()` to halt, `pcall()` for recovery). No type system. No operator overloading. No namespaces.

All of those can be added later _in the language itself_ via blocks, or in a future compiler version compiled by v0.
