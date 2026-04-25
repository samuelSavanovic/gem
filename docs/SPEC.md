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

Six types: `Int`, `Float`, `String`, `Bool`, `Nil`, `Table`, `Fn`. All dynamically typed. Every value is a tagged C union. Yes this means primitives are boxed and slow — doesn't matter for v0. Future optimization: NaN-boxing to pack ints, bools, and nil into a double's NaN space, eliminating heap allocation for primitives.

**Variables**

```
let x = 10
let name = "hello"
x = 20
```

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

This one rule lets stdlib define `each`, `map`, `unless`, `until`, `with_file`, `loop`, etc. without compiler changes.

**Tables**

```
let point = { x: 10, y: 20 }
point.x
point["x"]

let list = [1, 2, 3]
list[0]
```

`{ }` with keys is a table. `[ ]` is sugar for an integer-keyed table. Dot access is sugar for string key lookup.

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

`elif` desugars to nested `if/else` at parse time — no new AST nodes. One `end` closes the entire chain.

`break` exits the innermost loop (`while` or `for`). `continue` skips to the next iteration.

**For Loops**

Two forms, both desugared to `while` at parse time:

```
# Array iteration
for item in items
  print(item)
end

# Range iteration (0 to n-1)
for i = 0, n
  print(i)
end
```

`break` and `continue` work inside `for` loops. The iterator increment happens before the user body, so `continue` correctly advances to the next element.

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

**C Interop**

```
extern fn puts(s: String) -> Int
extern fn sqrt(x: Float) -> Float
extern fn fopen(path: String, mode: String) -> Ptr

puts("hello from C")
```

`extern fn` declares a C function. The compiler emits the call directly since we compile to C. Type annotations on extern declarations only — the rest of the language stays dynamically typed. `Ptr` is an opaque type for C pointers.

For C headers:

```
extern include "math.h"
extern include "stdio.h"
```

**Operators**

`+` for both arithmetic and string concatenation. If types don't match (e.g. string + int), runtime error. No `..` operator — keep it simple.

`+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `and`, `or`, `not`

**Assignment Operators**

`=`, `+=`, `-=`, `*=`, `/=`

**Strings**

```
let s = "hello"
s.len()
s[0]
s + " world"
```

**String Interpolation**

Expressions inside `{...}` in string literals are evaluated and auto-coerced to strings via `to_string`:

```
let name = "world"
print("hello {name}")          # hello world
print("{1 + 2} items")         # 3 items
print("flag: {true}")          # flag: true
```

Literal braces are escaped with `\{` and `\}`:

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

`keys(tbl)` — returns a new table containing the keys of `tbl` as an integer-indexed array. Currently requires `extern fn` declaration; will become a builtin.

All builtins are first-class values — they can be stored in variables and passed to functions.

**Module System (v0.1 — minimal)**

```
load "compiler/parser"
load "stdlib/table"
```

File inclusion with a re-inclusion guard — the runtime keeps a table of already-loaded file paths. `load` checks before including. Prevents double-loading in multi-file projects like the compiler itself.

No namespacing in v0. Everything is global.

**What the Compiler Needs to Emit**

Every value is a tagged C union. The compiler emits C code that uses the runtime glue (~200–350 lines) which wires together Boehm GC, minicoro, and stb_ds.h. The generated C code calls `GC_malloc` for allocations (with coroutine stacks registered as GC roots via custom allocator), uses minicoro for coroutines with a built-in round-robin scheduler and mailbox channels, and stb_ds for table operations. The compiler emits C `#line` directives so that runtime errors report Gem source file and line numbers instead of C line numbers.

**What's NOT in v0**

No classes. No inheritance. No exceptions (use `error()` to halt, `pcall()` for recovery). No variadic functions. No destructuring. No type system. No operator overloading. No namespaces.

All of those can be added later _in the language itself_ via blocks, or in a future compiler version compiled by v0.
