# Gem Best Practices

How to write Gem that reads well, runs fast, and doesn't fall into the
runtime's traps. [`SPEC.md`](SPEC.md) says what the language *does*;
[`CHEATSHEET.md`](CHEATSHEET.md) is the syntax on one page; this file says
what to *reach for*.

Every rule and sample here was checked by running it with the compiler.
Timings were measured on Linux x86_64 and are only meant as orders of
magnitude. Two markers:

- **(trap)**: fails silently, crashes, or falls off a performance cliff.
  Treat it as a hard rule.
- **(bug)**: the rule exists only because of a compiler, runtime or std bug
  listed in [`KNOWN_BUGS.md`](KNOWN_BUGS.md). When the bug is fixed, the
  rule goes.

Contents: [Before you start](#before-you-start) ·
[A first program](#a-first-program) ·
[Variables and scope](#variables-and-scope) · [Loops](#loops) ·
[Control flow](#control-flow) · [Functions](#functions) ·
[Tables and arrays](#tables-and-arrays) · [Numbers](#numbers) ·
[Strings](#strings) · [Errors](#errors) ·
[State and memory](#state-and-memory) · [Processes](#processes) ·
[I/O and blocking](#io-and-blocking) · [Modules](#modules) ·
[C interop](#c-interop) · [Tests](#tests) · [Style](#style) ·
[Trap index](#trap-index)

---

## Before you start

**Running.** `gem prog.gem` compiles and runs a program; `gem prog.gem -o
prog` only builds the binary; `gem --check prog.gem` only checks it (and
prints the compiler's `note:` and `warning:` lines). `argv()` returns the
command-line arguments, with the program name at `argv()[0]`, and `getenv(name)` reads the environment.

**Builtins and std modules.** Builtins such as `print`, `len`, `push`,
`keys`, `sort`, `spawn`, `send` and `str_replace` are always there (the
full list is in [`CHEATSHEET.md`](CHEATSHEET.md)). Everything else lives
in std modules: `load "std/json"` makes the module's exports available as
`json.parse`, `json.encode`, and so on. The modules are `string`, `table`,
`math`, `time`, `log`, `json`, `url`, `mime`, `http`, `request`, `sqlite`,
`task`, `gen_server`, `supervisor`, `dynamic_supervisor` and `test`.

**Blocks.** A call can take a closure as its last argument, written after
the call as `do |params| ... end` (or `do ... end` with no parameters):

```gem
let evens = table.filter(nums) do |n|
  n % 2 == 0
end
```

is the same as `table.filter(nums, fn(n) n % 2 == 0 end)`. `spawn do ...
end`, `pcall do ... end` and `build_string do |add| ... end` are the same
form.

**Words this doc uses.**

- *Array*: a table with integer keys `0 .. n-1` (`[1, 2, 3]`). *Record*: a
  table with string keys (`{name: "a"}`).
- *Process*: a lightweight thread with its own memory and mailbox, started
  with `spawn`. Processes share nothing and talk with `send`/`receive`.
  *pid*: a process id. *ref*: a unique value from `make_ref()`, used to tag
  a request so its reply can be recognized.
- *Pin*: `^name` in a pattern, which compares with the variable's value
  instead of binding a new variable.
- *Arena*: a process's memory. The runtime frees garbage at each loop's
  *back-edge*, the point where an iteration ends and the next begins.

**Statements, not expressions.** `if`, `match` and `receive` don't produce
values inside expressions (`let x = if ...` doesn't parse). An expression
can't continue on the next line, even inside parentheses; only call
arguments and table literals can span lines. Both come up below.

---

## A first program

A small JSON API: notes kept in a `gen_server` (a process that owns state
and answers requests), served by `std/http`, with word counts computed in
parallel by `task`.

```gem
load "std/gen_server"
load "std/http"
load "std/json"
load "std/task"
load "std/string"

# The gen_server callbacks. handle_call answers gen_server.call; every
# callback returns the new state, so each match needs an else.
let NOTES = {
  init: fn() [] end,
  handle_call: fn(msg, from, notes)
    match msg
    when {tag: "add", text: text}
      push(notes, text)
      {reply: len(notes) - 1, state: notes}
    when {tag: "all"}
      {reply: notes, state: notes}
    else
      {reply: nil, state: notes}
    end
  end,
  handle_cast: fn(msg, notes) {state: notes} end,
  handle_info: fn(msg, notes) {state: notes} end
}

fn word_count(text)
  len(string.split(text, " "))
end

fn routes(store)
  let app = http.router()
  app.post("/notes") do |req|
    let r = pcall json.parse(req.body)
    if not r.ok or type(r.value) != "table" or type(r.value.text) != "string"
      return http.bad_request('expected {"text": "..."}')
    end
    let id = gen_server.call(store, {tag: "add", text: r.value.text})
    http.json_response({id: id})
  end
  app.get("/stats") do |req|
    let notes = gen_server.call(store, {tag: "all"})
    let tasks = []
    for text in notes
      push(tasks, task.async(fn() word_count(text) end))
    end
    http.json_response({notes: len(notes), words: task.await_all(tasks, 2000)})
  end
  app
end

fn main()
  let store = gen_server.start(NOTES).pid
  http.serve(routes(store), {port: 8080})
end
```

`POST /notes` with `{"text": "hello big world"}` answers `{"id":0}`, and
`GET /stats` then answers `{"notes":1,"words":[3]}`. What it shows:

- The state lives in a process. Each HTTP connection runs in its own
  process, so a module-level `let notes = []` would give every connection
  its own copy ([State and memory](#state-and-memory)).
- `gen_server.start(...)` returns `{pid: ...}`; pass `.pid` to
  `gen_server.call` ([Working around std today](#working-around-std-today)).
- A handler returns a response table (`http.json_response`,
  `http.bad_request`, ...). `return` inside a `do` block leaves the block,
  which here is the handler.
- User input goes through `pcall` and a type check before it is trusted.
- `task.async` takes a closure; `task.await_all` waits for all of them,
  with a timeout.

---

## Variables and scope

### `let` declares, `=` assigns **(trap)**

`let x = ...` always makes a *new* variable, even when an `x` already
exists; the new one hides the old one until the end of the block. To change
a variable, assign it without `let`:

```gem
fn sum(xs)
  let total = 0
  for x in xs
    total = total + x          # updates the outer total
  end
  total
end
```

With `let total = total + x` inside the loop, each iteration makes a new
`total` that disappears at the end of the iteration, and `sum` returns `0`
with no warning. The compiler warns only in one case: a `let` in a `while`
body that hides a variable the loop's condition reads.

Shadowing on purpose is fine. The new variable's initializer still sees
the old one, so `let n = n - 1` and `let line = string.trim(line)` work,
and inside a function a closure created before the second `let` keeps the
old variable. (At module level a second `let` rebinds instead; see
below.)

### Declare before the block, assign inside

A `let` is visible from its declaration to the end of the block it is in
(the body of a function, `if` branch, loop or `match` arm). Using it after
the block ends is a compile error, `undeclared identifier`, even when every
branch of an `if` declares it. Declare the variable before the block and
assign it inside:

```gem
let label = "small"
if n > 100
  label = "big"
end
print(label)
```

### Module-level variables

A `let` at the top level of a file (outside any function) is a
*module-level* variable. Every function in the file can use it, wherever
the function is defined, but its value exists only once the `let` has run:
read before that (by top-level code above it, or by a function called from
there), it is `nil`, with no error. Put module-level `let`s at the top of
the file. A second `let` of the same name at module level rebinds that
variable instead of making a new one. A `fn` or `extern fn` can't share a name
with a module-level `let` or another `fn` of the file: that is a compile
error.

Each process has its own copy of module-level variables (see
[State and memory](#state-and-memory)), so they are for constants and
configuration, not for state that processes share.

### Closures

A closure (`fn(...) ... end`, or a `do` block) can use the variables
declared before it is created. A closure that refers to a `let` written
after it is a compile error, and that includes a local closure calling
itself through the `let` that defines it. Declare the name first:

```gem
fn count_down(n)
  let step = nil
  step = fn(i)
    if i == 0 then return "done" end
    step(i - 1)
  end
  step(n)
end
```

Named `fn` definitions don't have this limit: they can call each other in
any order.

A closure captures *variables*, not values, so it sees later assignments
to them. A `for` loop declares its variable inside the body, so each
iteration has a fresh one and each closure keeps its own; a variable
declared *before* a `while` loop is shared, and every closure sees its
final value:

```gem
let fs = []
for i = 0, 3
  push(fs, fn() i end)         # 0, 1, 2
end
let j = 0
while j < 3
  push(fs, fn() j end)         # 3, 3, 3
  j += 1
end
```

A closure given to `spawn` is the exception: the new process gets a copy
of what the closure captures, taken at the `spawn`, and later changes on
either side are not shared.

---

## Loops

### Iterate with `for`, not an index counter

```gem
# Prefer
for item in items
  handle(item)
end
for name, value in headers
  add("{name}: {value}\r\n")
end
for i = 0, len(rows)          # when you need the index; runs 0 .. len-1
  print("{i}: {rows[i]}")
end

# Over
let ks = keys(headers)
let i = 0
while i < len(ks)
  let name = ks[i]
  ...
  i += 1
end
```

`for` walks the table without building a `keys()` array, so it is never
slower than the index loop, can't forget the increment, and `continue`
works in it. The range form `for i = a, b` counts up from `a` to `b - 1`,
with no step; count down with a `while`.

`for k, v in tbl` visits keys in `keys(tbl)` order (insertion order for
string keys, until a `delete` moves the last key into the hole); on an
array, `k` is the index. The single-variable form `for x in t` is for
arrays only: on a record it yields `nil` for every entry. Use `for k, v in
t` or `values(t)`.

### Don't add or remove entries of what you're iterating **(trap)**

`for k, v in tbl` reads the length once, and `delete` moves the last entry
into the hole, so deleting during the loop skips entries and then visits
`nil`. `for x in arr` re-reads the length each time round, so `remove_at`
during the loop skips the element after each one removed, and `push`
makes the loop visit the new elements too. Iterate over a snapshot
instead:

```gem
for k in keys(sessions)
  if expired(sessions[k])
    delete(sessions, k)
  end
end
```

### Use `while` for scanners, but prefer builtins to byte loops

A `while` with a cursor is right when the step varies or the loop looks
ahead: tokenizers, frame parsers. Read bytes with `ord(s, i)`, which
doesn't allocate.

Before writing a byte loop, check whether a builtin does the job:
`str_replace` and `substr` run in C. HTML-escaping 800 KB took about 150 ms
with a per-byte loop and 25 to 60 ms with five chained `str_replace` calls.
`string.split` and `string.index_of` are Gem byte loops themselves and are
not fast: `string.split` on 800 KB took about 200 ms, against 70 ms for a
hand-written `ord`/`substr` loop.

### Use `for`, not `table.each`, when you need `return` or `break`

A `do` block is a closure. `return` inside it returns from the *block*, so
`table.each` carries on with the next element, and `break` or `continue`
inside it is a compile error. Use `table.each`/`map`/`filter` for
straight-through transforms; use `for` when you need to stop early.

```gem
fn find_user(users, id)
  for u in users
    if u.id == id
      return u
    end
  end
  nil
end
```

---

## Control flow

### `if`, `match` and `receive` are statements

`let x = if ...`, `let x = match ...`, `let x = receive ...` and
`return match ...` don't parse. A `match` or `if` yields a value only as the
last statement of a function, closure or block (including inside any branch
of a final `if`). When you need a value, put the `match` in a small
function, or declare the variable first and assign it in each branch:

```gem
fn status_class(code)
  match code
  when 200 then "ok"
  when 404 then "missing"
  else "error"
  end
end
```

An arm fits on one line with `then` (`when 200 then "ok"`, `after 100 then
nil`), like a one-line `if`. Without `then`, the body must start on the
next line. `pcall` is different: `let r = pcall f(x)` and `let r = pcall do
... end` are expressions.

### `match` on shape instead of `if` chains on fields

```gem
# Prefer
match pcall parse(input)
when {ok: true, value: ast}
  compile(ast)
when {ok: false, error: msg}
  eprint("parse failed: {msg}")
end

# Over
let r = pcall parse(input)
if r.ok
  compile(r.value)
else
  eprint("parse failed: " + r.error)
end
```

Use `if` for plain boolean conditions. Use `match` when you branch on a
tag, a literal, or the shape of a table. A table pattern matches when the
listed keys match; extra keys are ignored.

### A bare name in a pattern binds; pin it to compare **(trap)**

`when NAME` (alone or inside a table pattern) always matches and binds a
new variable, even when a variable or constant of that name exists. Use
`^NAME` to compare against its value:

```gem
let STATUS_OK = 200
fn classify(code)
  match code
  when ^STATUS_OK then "ok"     # `when STATUS_OK` would match every code
  else "error"
  end
end
```

Literals (`when 200`, `when "get"`) compare by value without a pin.
There are no guards or alternatives: `when v > 5` and `when "a" or "b"`
compile, but compare the target with the *value* of `v > 5` or `"a" or
"b"`. Use an `if` chain, or one arm per value. An array pattern `[x, y]`
also matches a record with two keys **(bug)**, so put array arms after
record arms when both can arrive.

### Give `match` an `else` when no arm should be skipped **(trap)**

A `match` whose arms all fail yields `nil` and carries on. When falling
through would be a bug, say so:

```gem
match msg
when {tag: "get"}
  ...
when {tag: "put", value: v}
  ...
else
  error("store: unknown message {msg}")
end
```

`receive` has no `else`; its catch-all is an arm that binds anything,
`when other` (see [Keep mailboxes clean](#keep-mailboxes-clean)).

### Compare with `nil` on purpose

Only `nil` and `false` are falsy. `0`, `""` and `[]` are truthy. Write
`if x == nil` when you mean "missing" and `if not x` when you mean "falsy".

---

## Functions

### Optional arguments: defaults, not `...rest`

```gem
# Prefer
fn call(target, msg, timeout_ms = 5000)

# Over
fn call(target, msg, ...rest)
  let timeout_ms = 5000
  if len(rest) > 0
    timeout_ms = rest[0]
  end
```

Keep `...rest` for real variadics, such as a function that joins all its
arguments. A default applies only when the argument is left out: passing
`nil` explicitly gives the parameter `nil`.

### Option bags: destructured parameters

```gem
# Prefer
fn serve(router, {port = 8080, host = "0.0.0.0"} = {})

# Over
fn serve(router, opts)
  let port = 8080
  if opts != nil and opts.port != nil
    port = opts.port
  end
```

Inside the bag the rule is looser than for plain defaults: a field default
applies when the field is missing *or* `nil`. The trailing `= {}` makes the
bag itself optional, and an explicit `nil` for the bag also becomes `{}`;
without `= {}`, passing `nil` is an error. Destructured names are ordinary
locals that closures and `spawn` bodies can capture.

### Arity is not checked

A call with too many arguments drops the extras, and missing arguments are
`nil` (or their default). Neither is an error, so a wrong call shows up
later as a `nil` somewhere else.

### `+=` works only on variables

`t.count += 1` and `t[k] += 1` are compile errors. Write
`t.count = t.count + 1`.

### Don't reuse module names for variables **(trap)**

Any variable named `string`, `table`, `json`, `time` (or like any module
the file loads) hides the module, and `json.encode(x)` then fails only at
runtime (`field access on non-table`, or `attempt to call nil value` if
the variable holds a table). A module-level `let` of that name
hides it for the whole file, functions defined above the `let` included.
Pick another name.

Naming a function or variable like a builtin (`fn error`, `let len = 3`)
is allowed: it hides the builtin in that file only, and modules the file
loads keep the builtin. A module-level one hides it in the whole file,
functions above it included, and `let keys = keys(t)` at module level
fails because its own initializer no longer reaches the builtin **(bug)**.
Do it only when the name is the module's API (`log.error`); elsewhere pick
another name.

### `pcall` takes a call, not a function **(trap)**

`pcall` is a keyword. `pcall f(x)` runs `f(x)` and catches any error in
it; the result is `{ok: true, value: v}` or `{ok: false, error: message,
stack: trace}`. For several statements, use a block:

```gem
let r = pcall do
  let data = read_file(path)
  json.parse(data)
end
```

`pcall fn() ... end` does *not* run the closure: it evaluates the closure
expression and returns `{ok: true, value: <fn>}`. And `pcall(f, x)` calls
`f()` with no arguments, dropping `x`. Stick to `pcall f(x)` and
`pcall do ... end`.

### `fn main` runs automatically

If the entry file defines `fn main`, the compiler calls it, with no
arguments, after the top-level code. Don't also call it yourself, or it
runs twice.

---

## Tables and arrays

### Append with `push` **(trap)**

```gem
push(items, x)                 # Prefer
items[len(items)] = x          # Over
items[count] = x; count += 1   # Over (and drop the separate counter)
```

Appending by index is quadratic: 20,000 appends took 1.6 s with
`a[len(a)] = x` and 4 ms with `push`.

### Remove from arrays with `remove_at`, never `delete` **(trap)**

`delete` is for string-keyed tables. On an array it moves the last element
into the hole: `["a", "b", "c", "d"]` after `delete(arr, 0)` is
`{3: "d", 1: "b", 2: "c"}`. After that, `arr[0]` is `nil` and `for x in arr`
visits `nil, "b", "c"` and never reaches `"d"`.

```gem
remove_at(arr, i)              # arrays: shifts the rest left, returns the element
delete(tbl, "key")             # string-keyed tables only
```

### Test keys with `has_key`

`tbl[k] != nil` can't tell "missing" from "present and nil"; `has_key(tbl,
k)` can. `x in tbl` means `has_key` only on a table with string keys. On a
table without them (an array, or a set like `seen[id] = true`) it scans
the *values*, so test such a set with `has_key(seen, id)`.

Assigning `nil` doesn't remove a key: after `t.x = nil`, `x` is still in
`keys(t)`, `len(t)` and `json.encode(t)`. Use `delete(t, "x")`.

### Don't mix string keys into arrays

A table is either an array or a record. Mixing them makes `len`, `in`,
`for` and `json.encode` unpredictable.

### Tables compare by identity

`{a: 1} == {a: 1}` and `[] == []` are `false`. Compare fields, or compare a
primitive key such as an id. The same applies to pinned patterns (pin
strings, numbers and refs, never tables) and to `test.assert_eq`.

### Tables and JSON

An empty array and an empty record can't be told apart, so `json.encode({})`
gives `[]`, and so does a record emptied with `delete`. When an empty
object matters on the wire, write that part of the JSON yourself (`'{}'`).

`json.encode` writes keys in insertion order, so two equal records built in
different orders encode differently. It decides "array or object" from
the first key alone, so a table with int keys that aren't exactly
`0 .. n-1` (`by_id[row.id] = row`) loses entries **(bug)**: key `42` alone
encodes as `[null]`. Key such tables by string (`by_id["{row.id}"]`)
before encoding.

`json.parse` raises on malformed input; wrap it in `pcall` when the input
comes from outside.

### Out-of-range reads

Reading an array past its end gives `nil`, but a negative index past the
start raises, and any out-of-range string index raises (`"abc"[10]`).
Negative integers are always positions from the end (`arr[-1]` is the last
element), never keys: `t[-10] = x` raises on a table with fewer than 10
entries. Use string keys for data keyed by negative numbers.

### `sort` comparators return a number **(trap)**

`sort(arr, cmp)` sorts in place and expects `cmp(a, b)` to return a
negative number, zero or a positive number. A boolean comparator
(`fn(a, b) a < b end`) leaves the array unsorted, with no error:

```gem
sort(people, fn(a, b) a.age - b.age end)
```

---

## Numbers

### Don't compare ints and floats with `==` **(trap)**

`2.0 == 2` is `false`, `when 2` doesn't match `2.0`, and `t[1]` and
`t[1.0]` are different keys. JSON numbers with a decimal point or an
exponent (`1e2`) parse as floats. Convert first (`to_int`, `floor`) when
values may come from either. `<` and the other orderings do compare ints
with floats numerically; only equality doesn't. A float always prints with a
decimal point or an exponent (`2.0`, `1e+16`), an int never does.

### Integer arithmetic follows C

`7 / 2` is `3` and `-7 % 3` is `-1`. Use `to_float` for real division.
`%` takes integers only, and dividing by zero raises `division by zero`,
for floats too.

### `to_int` and `to_float` raise on bad input

`to_int("12abc")` is an error, and so is `to_int("12\n")`: `string.trim` lines
read from files first. Wrap conversions of user input (route params,
query strings, form fields) in `pcall` and answer a 400, or a bad id
becomes a 500.

---

## Strings

### Interpolate instead of concatenating

```gem
"user {id} not found"                         # Prefer
"user " + to_string(id) + " not found"        # Over
```

Interpolation calls `to_string` for you, and any expression works inside
the braces, string literals included (`"{"a" in t}"`). Use single quotes
for text with literal braces, such as `'{"key": 1}'`.

### Building strings

Inside a loop, `s = s + piece` (or `s += piece`) is compiled into an
in-place append, so it is fast, **as long as the loop doesn't read `s`
until it is done**. A loop that tests `len(s)` or compares `s` each time
round, a string threaded through recursion as an argument, or a prepend
(`s = piece + s`) copies the whole string every iteration and is
quadratic (200 KB: about 1.2 s, against 2 ms for the plain loop). For
anything non-trivial, use `build_string`, which has no such conditions:

```gem
let out = build_string do |add|
  for row in rows
    add(row.name, ",", row.value, "\n")
  end
end
```

`add` takes any number of values and converts each with `to_string`,
except a buffer, which it drops **(bug)**: pass `to_string(buf)`. Use a
buffer (`buf_new()`, `buf_push(buf, s)`, `to_string(buf)`) when the text
has to be built across several functions or loop iterations that a single
block can't hold, such as a read loop that collects chunks. `print(buf)`
and `"{buf}"` show `<buffer:N>`, not the contents.

### Strings are bytes

`len` is the byte count (`len("é")` is `2`), `s[i]` is a 1-byte string,
and `ord(s, i)` is the byte value. Use `substr(s, start, count)` to slice;
unlike `s[-1]`, a negative `start` counts as `0`. Double-quoted strings
have no `\x` or `\u` escapes (an unknown escape is kept as written); use
`chr(n)` for other bytes. Strings may contain `\0`, but `print` stops at
the first one; use `write_stdout` for binary output.

---

## Errors

### `error` takes a string **(trap)**

`error({code: 404})` and `error(42)` lose the value: `pcall` reports the
message as the string `"error"`. When callers need structure, return a
result table instead of raising.

### Prefix messages with where they came from

```gem
error("task.await: only the process that started a task can await it")
```

### Return results for expected failures, raise for bugs

Lookups that can miss return `nil`. Operations whose failure the caller
should handle return `{ok: true, value: v}` / `{ok: false, error: msg}`
(the same shape `pcall` uses, so it works with `match`). Use `error` for
broken invariants and misuse.

### `pcall` at boundaries, not everywhere

An uncaught error in a spawned process kills only that process: the
runtime prints a crash report with a stack trace on stderr, and the
processes monitoring or linked to it find out. That is the recovery
mechanism. Put `pcall` where a failure must not escape: an HTTP handler
that has to answer 400 or 500, a test runner, a parser fed user input.
Don't wrap every call. In a long-running loop, wrap one iteration's work
inside the loop rather than the whole loop, so one bad request doesn't end
the loop.

Re-raising a caught error with `error(r.error)` reports the stack of the
re-raise, not the original, so log `r.stack` first if you need it.

---

## State and memory

### Module-level variables are per process **(trap)**

Module-level `let`s (in the entry file and in loaded modules) are
per-process, as in Erlang. A spawned process starts with a copy of its
parent's module-level variables as they were at the `spawn`. From then on
its writes stay in that process, and other processes never see them.
Inside one process, functions and closures all read the live value.

So a module-level `let` can't hold state that processes share. A counter
bumped from a handler process counts only that process's calls. Shared,
changing state belongs in a process: a `gen_server` (as in
[A first program](#a-first-program)) or a `receive` loop:

```gem
# Prefer: state lives in a process
fn counter_loop(n)
  receive
  when {tag: "inc"}
    counter_loop(n + 1)
  when {tag: "get", from: from, ref: ref}
    send(from, {tag: "count", ref: ref, value: n})
    counter_loop(n)
  when other
    counter_loop(n)
  end
end

# Over: each process increments its own copy
let count = 0
fn inc() count = count + 1 end
```

The compiler prints a `note:` for each write to module-level state in code
reachable from a `spawn do ... end` or `spawn(fn() ... end)` body. Read it
as a question: is this state meant to be private to the process? A
function passed to `spawn` by name or in a variable gets no `note:`, and
neither do `std/http` handlers, which run in a process per connection that
std spawns: there a write to a module-level variable changes that
connection's copy only.

Configuration that main sets before spawning is copied into every process
spawned after it, so set it first; that includes `log.set_level`, which
must run before `http.start`. Name constants in `UPPER_SNAKE` (`READ_SIZE`,
`STATUS_TEXT`) so it's obvious they never change.

The copy is lazy: a child copies a module-level variable the first time it
reads it, and pays nothing for the ones it never reads. Large module state
still costs time: a child reading a module-level array of 200,000 small
records spent a few hundred milliseconds copying it. Keep large data in a
process that answers queries, or pass a child only what it needs.

### A process loop is `while` or a tail call **(trap)**

Each process's memory is freed when it exits, and every loop also frees
its garbage at its back-edge: `while` and `for` loops, and functions that
call themselves as their last action (a *tail call*, which the compiler
turns into a loop). A long-running process stays in constant memory with
no special structure, as long as it is a loop.

A loop written as recursion must make the self call the last thing the
function does. `loop(state)` followed by `return nil` (or any other
statement) is an ordinary call: every message handled adds a stack frame
and keeps its memory, until `stack overflow` kills the process after a few
thousand iterations.

```gem
fn serve_loop(state)
  receive
  when {tag: "stop"}
    nil
  when msg
    serve_loop(handle(state, msg))   # last expression of the arm: a loop
  end
end
```

`return serve_loop(x)` is a tail call too. A `while true` loop works the
same way:

```gem
fn serve_forever(state)
  while true
    receive
    when msg
      state = handle(state, msg)
    end
  end
end
```

If the compiler prints `warning: ... cannot reset per-process arena at this
loop's back-edge`, that loop's memory grows without bound. It is printed
only for `while true` loops, and says why; restructure the loop, or file an
issue.

### Mutate state in place

Tables are never shared between processes (messages are copies), so a
process can change its own tables freely. In a process loop,
`state.mode = "connected"` followed by `loop(state)` is fine and cheaper
than rebuilding the table. Within one process, remember that two variables
can point to the same table: copy it first if one of them must keep the
old contents.

### Deep recursion raises an error

Every process has an 8 MB stack: from a few thousand non-tail frames
(functions with a large `match` or `receive`) to about 30,000 (small
ones). Past that, the call raises `stack overflow in <fn>`, which `pcall`
catches like any error. `json.parse` refuses nesting deeper than about 128
levels; `json.encode` has no cap and overflows at about 5,000. For
recursive walkers over untrusted input, cap the depth or use an explicit
stack, so a hostile input gets a clear error instead of a stack overflow.

---

## Processes

### Use the std behaviours before writing a raw loop

| Need | Reach for |
|---|---|
| Run work concurrently, collect results | `task.async(f)` / `task.await(t, ms)` / `task.await_all(ts, ms)` |
| A server with state and request/reply | `gen_server` (module of callbacks, `start`, `call`, `cast`) |
| Restart processes that crash | `supervisor` (fixed children), `dynamic_supervisor` (children started on demand) |
| Something else | a `receive` loop |

[A first program](#a-first-program) shows `gen_server` and `task`;
[`SPEC.md`](SPEC.md) documents every callback and option.

### Working around std today

Some std APIs don't follow this doc yet. Until they are fixed:

- `gen_server.start`, `supervisor.start` and `dynamic_supervisor.start`
  return `{pid: pid}`, but `gen_server.call`, `gen_server.cast` and
  `supervisor.which_children` take a bare pid or a registered name: pass
  `handle.pid`. `http.start` returns a bare pid.
- A gen_server callback that returns `nil` or a non-table (such as the
  `nil` of a `match` with no `else`) crashes the server. `handle_call`
  returns `{reply: v, state: s}` (or `{noreply: s}`), the others
  `{state: s}`; a `handle_call` result with neither `reply` nor `noreply`
  sets the state to `nil` and leaves the caller waiting until its
  timeout.
- `gen_server.call` waits its full timeout (5 s by default) when the
  server is dead; `supervisor.which_children` and the `dynamic_supervisor`
  calls wait with no timeout at all.
- A supervisor child's `start` function must return a bare pid:
  `start: fn() gen_server.start(m).pid end` **(bug)**.
- `supervisor.start` with `name:` registers the name only after the
  children start, so use the returned pid right after `start` **(bug)**.
- A dynamic supervisor crashes when it removes any child but the last one
  `which_children` lists: through `terminate_child`, or when a temporary
  or transient child exits. It also overflows its stack after about 2,000
  temporary or transient child exits **(bug)**. Don't use it for pools of
  short-lived workers yet.
- `std/http` answers a handler error (or a handler that doesn't return a
  response table) with an empty 500 and logs nothing **(bug)**: catch and
  log errors in the handler while debugging. Request header names keep
  the client's case (`req.headers["Content-Type"]` misses
  `content-type`) **(bug)**. `http.serve` returns early when the calling
  process receives any message **(bug)**, so call it last, from `main`.
- `std/request` reads with no timeout **(bug)**.
- Don't send messages tagged `"call"`, `"cast"`, `"gs_reply"`,
  `"start_child"` or `"which_children"` to std processes: std uses those
  tags internally.

### Spawning

```gem
let parent = self()
spawn do                       # or spawn(fn() ... end)
  send(parent, {tag: "done", value: work(5)})
end
```

- Inside the `spawn` body, `self()` is the child. Take the parent's pid
  before spawning, as above.
- Pass arguments through the closure: `spawn(worker, 5)` calls `worker()`
  with no arguments.
- Spawn a literal function (`spawn do ... end`) rather than one held in a
  variable: the compiler follows a literal body to find writes to
  module-level state and print a `note:`.

### Messages are deep copies with a `tag`

```gem
send(pid, {tag: "deliver", frame: f})
```

`send` (and `spawn`, for what its closure captures) copies the value into
the receiving process, so changing a received table doesn't affect the
sender. The copy costs time proportional to the whole message, strings
included: 2,000 round trips took 13 ms with a 10 B request body and 1.3 s
with a 1 MB one. Send what the receiver needs, not a large shared
structure, and never fan a big message out to many processes.

Match on `tag` in `receive`. Prefix tags that are private to a module with
`_` (`"_task_result"`) so they can't collide with user messages.

### Monitor, link, trap

- `monitor(pid)`: "tell me when it dies". When `pid` exits, the monitoring
  process gets `{tag: "DOWN", pid: pid, reason: reason}` (reason `"normal"`
  or the error message). Use it in clients, callers and observers.
- `spawn_link(f)`: "we live and die together" (a reader/writer pair). If
  either process dies with an error, the other dies too. Prefer it to
  `spawn` followed by `link`: linking to a process that has already
  exited, even normally, kills the caller with reason `noproc`, and
  `pcall` can't catch that.
- `spawn_monitor(f)`: spawn and monitor in one step. It returns
  `{pid: pid}`, not a bare pid.
- `process_flag("trap_exit", true)`: turns the death of a linked process
  into a message, `{tag: "EXIT", pid: pid, reason: reason}`, instead of
  killing this process. Only for processes whose job is to handle deaths
  (supervisors). Such a process gets an `EXIT` for every linked process
  that ends, normal exits included, so its loop needs an arm or a
  catch-all for them.

`pcall` doesn't catch a `kill` or a link's exit: those end the process at
once.

### Request/reply: a ref, a pin, a timeout, and a monitor

When you write the request side yourself (instead of using
`gen_server.call`):

```gem
fn store_get(server, k)
  let ref = make_ref()
  send(server, {tag: "get", from: self(), ref: ref, key: k})
  receive
  when {tag: "reply", ref: ^ref, value: v}
    v
  after 5000
    error("store.get: timeout")
  end
end
```

- Without `^ref`, a late reply from an earlier, timed-out request can be
  taken for this one.
- Every blocking request in a library needs an `after`.
- When the target may die, monitor it and add a `DOWN` arm, so a dead
  server fails at once instead of after the full timeout. `std/task` is the
  model: it monitors, matches `{tag: "DOWN", pid: ^pid}`, and removes the
  `DOWN` when it's done. There is no `demonitor`, and a monitor from a
  process that has exited stays on the target's list until the target
  dies, so don't monitor a long-lived server from many short-lived
  processes, such as per-connection handlers.
- A `receive` needs at least one `when` arm: an `after`-only `receive`
  doesn't parse **(bug)**. To wait, use `sleep(ms)`.
- `after` restarts each time a `receive` is entered, and a message that
  matches another arm ends the wait, so `after` in a server loop that keeps
  getting messages may never fire. For periodic work, send yourself a
  message with `send_after(self(), {tag: "tick"}, ms)` and handle `tick`
  like any other message.
- After a timeout, the reply may still arrive later. The pin makes it
  harmless to *this* wait, but it sits in the mailbox until something
  removes it; the process's main-loop catch-all (below) is what drops it.

### Keep mailboxes clean

Every `receive` scans the whole mailbox for the first message that matches
one of its arms, so messages nobody matches make every later `receive`
slower: 2,000 round trips took 11 ms with an empty mailbox and 1.1 s with
1,000 stale messages in it.

- A process's main loop ends its `receive` with a catch-all arm
  (`when other`) that drops or logs unknown messages.
- A *request/reply wait* never uses a catch-all, and never `receive()`
  (which takes the next message, whatever it is): it would take messages
  that belong to someone else, such as a task result or another call's
  reply.

### Registered names

`register("db", pid)` lets other processes `send("db", msg)` and
`whereis("db")`. Register a process from its parent, after `spawn`: a
child that registers itself may not have run yet when the parent sends.
`send` to a dead pid silently drops the message, but `send("db", msg)`
raises `send: no process registered with that name` once the process has
died (say, while a supervisor restarts it). Where that can happen, check
`whereis` for `nil`, or `pcall` the send.

### There are at most 1024 processes

`spawn` raises `spawn: process table full` past the limit; main uses one
slot, so 1,023 spawned processes can be alive at once. A loop that spawns
quick tasks can still hit it: `spawn` doesn't let the children run, so
`for i = 0, 5000` spawning a one-line task fails at the 1,024th. Batch the
work, or cap how many are in flight (a `sleep(0)` in the loop lets
finished children exit). `task.async` over a long list has the same limit.
A reader/writer pair per connection uses two. An acceptor that spawns per
connection should catch that error and close the connection, or cap the
number of connections; otherwise one burst kills the acceptor.

### When the program ends

The program doesn't exit when main's code ends: it keeps running while any
process can still do something, such as a server waiting on a socket, a
pending `sleep`, `after` or timer. Once main has finished and every
remaining process waits in a `receive` that nothing can satisfy, the
program exits with status 0 and those processes are dropped. If main
itself is stuck that way, the runtime reports `deadlock: main process is
waiting in receive ...` and exits with status 1. Call `exit()` to end the
program from anywhere.

A spawned process that crashes prints its error on stderr but doesn't
change the program's exit status: if main finishes normally, the program
exits 0. When a failure must fail the program (a test, a batch job),
monitor the process (or use `task.await`) and react to the `DOWN`.

---

## I/O and blocking

### Know what blocks every process

Gem code is preempted at loop back-edges and self tail calls, so a busy
loop doesn't starve other processes. Non-tail recursion has no such point:
a naive recursive `fib(30)` holds every other process up until it
returns. A builtin that blocks the OS thread also stops **all**
processes.

| Yields to other processes | Blocks everything |
|---|---|
| `tcp_*` (except name lookup, see right), `sleep`, `receive` | `tcp_connect` to a host *name* (DNS lookup runs inline) |
| `read_file`, `write_file`, `append_file`, `exec`, `sqlite_open`, `sqlite_close`, `extern blocking fn` (4-thread pool) | `sqlite_query`, `sqlite_exec` |
| | `file_exists`, `is_dir`, `list_dir`, `mkdir`, `remove_file`, `normalize_path` |
| | plain `extern fn`, `input`, `read_stdin`; `print`, `eprint`, `write_stdout` to a slow pipe |

Keep sqlite queries short and indexed: a one-second query stalls every
process for that second. A sqlite handle is a raw pointer: a wrong or
already-closed one crashes the program, which `pcall` can't catch
**(bug)**. Use `extern blocking fn` for any C call that can take more than
about a millisecond. The pool has 4 workers, so four long `exec` calls
delay every file read behind them.

### The process that opens a handle closes it, on every path

Sockets and sqlite handles are not closed when the process that owns them
dies. A crash between open and close leaks the descriptor (and a listening
socket keeps its port). For a short exchange, close in the same function
that opened it, with the risky part in `pcall`:

```gem
let fd = tcp_connect(host, port)
let r = pcall request_once(fd)
tcp_close(fd)
if not r.ok
  error(r.error)
end
r.value
```

To keep a connection open after a bad request, `pcall` each iteration
inside the connection loop instead. Since `pcall` doesn't catch `kill` or
a link's exit, keep long-lived handles in a process nobody kills.

### `tcp_listen` takes an IP address

`tcp_listen("localhost", port)` raises `invalid address`; pass
`"127.0.0.1"` or `"0.0.0.0"`. `tcp_connect` accepts host names.

### Pass timeouts to reads, check writes

- `tcp_read(fd, n, timeout_ms)` returns `""` at end of stream and `nil` on
  timeout. Library code should always pass a timeout; without one, a silent
  peer blocks the caller forever. A timeout of `0` means *no* timeout.
- `tcp_write` returns the number of bytes written and does not raise when
  the peer has gone. Treat `tcp_write(fd, s) < len(s)` as "connection
  lost", but expect the first write after a disconnect to still report
  success; only later writes show it. `tcp_write` has no timeout, so a peer
  that stops reading blocks the writer.

---

## Modules

### Layout

```gem
# thing — one line on what it is for.
#
#   let x = thing.make(...)      # a short usage example
#   thing.use(x)

load "std/string"

let DEFAULT_LIMIT = 100

fn helper(...)               # private: not in the export list
end

fn make(...)
end

export make, use
```

- `load "./thing"` loads `thing.gem` from the loading file's directory and
  binds its exports as `thing.make`, `thing.use`. `load "std/x"` loads a
  std module. `load "std/string" (split, trim)` binds only `split` and
  `trim`, with no `string.` namespace; prefer the plain `load` and the
  namespace (`string.split`) unless a name is used often enough to be
  noise.
- The `export` list decides what is public. Private functions don't need a
  `_` prefix.
- Name module files in `snake_case`: a file name that isn't a C
  identifier (`my-utils.gem`) fails in the C compiler **(bug)**. Don't name
  a module like any std module (`log.gem`, `json.gem`): the namespace is
  the file's base name, and a module of the same name loaded anywhere in
  the program, by you or by std (`std/http` loads `string`, `url`,
  `mime`, `json` and `time`), replaces it or breaks the C compile
  **(bug)**.
- Modules can't load each other in a cycle: the compiler reports
  `load cycle: a.gem → b.gem → a.gem`. Move shared code into a third
  module.

### Accept a pid or a `{pid}` handle in one place

When your own API takes a process, accept both a bare pid and the
`{pid: pid}` handle that std's `start` functions return, and normalize it
once in a helper; don't repeat `if type(t) == "table"` in every function.

---

## C interop

- Put the C code in a header of `static` functions and write
  `extern include "<path>"` before the `extern fn` declarations. Give
  your own header as an absolute path **(bug)**: a relative path is not
  looked up next to the `.gem` file. The header comes before `gem.h`, so
  include `"gem.h"` in it if it uses `GemVal` or `GemBytes`. The program
  links only libc, libm and pthreads.
- For a libc function, `extern include` its header (`"stdio.h"` for
  `puts`). Without any `extern include`, the compiler writes its own
  prototype from the extern types, which clashes with libc's.
- Types are checked, not converted: a `Float` parameter rejects `2` (pass
  `2.0` or `to_float(n)`). Too few arguments raise; extra ones are ignored
  **(bug)**. A `Ptr` is an int in Gem, and `NULL` comes back as `0`, not
  `nil`. `extern blocking fn` can't take or return a `Table`.
- `String` parameters arrive as `const char *` and stop at the first `\0`.
  Use `Bytes` for binary data.
- A plain `extern fn` runs on the scheduler thread and blocks every
  process. Declare slow calls `extern blocking fn`.
- Returned strings: a plain `extern fn` must return static memory (the
  runtime copies it and does not free it); an `extern blocking fn` must
  return `malloc`'d memory (the runtime frees it). A `NULL` return is
  `nil` from a plain `extern fn` but `""` from an `extern blocking fn`.
- Don't keep pointers to Gem strings or tables on the C side after the call
  returns: the next arena reset or the process's exit frees that memory,
  and an `extern blocking fn` gets copies that are freed when it returns.
- C code that recurses without limit kills the process, and `pcall` can't
  catch it.

---

## Tests

- Use `std/test` (`test.case`, `test.assert`, `test.assert_eq`,
  `test.assert_throws`, `test.run()`) for checks that verify themselves.
  `assert_eq` compares with `==`, so tables compare by identity and `1`
  doesn't equal `1.0` (the failure reads `expected 1, got 1`): compare
  primitives or individual fields.
- A test that spawns a process should `spawn_monitor` it (or use `task`)
  and check the result or `DOWN`. With `spawn_link`, a crashing child kills
  the test runner, and the remaining cases never run.
- Cover the edges as well as the happy path: empty input, a single element,
  `nil` where a table is expected, deep nesting, timeouts, and a process
  dying mid-request.
- In this repository, every behavior change also gets a numbered example
  under `examples/`, with its stdout appended to `expected_output.txt`;
  `make test` runs them all.

---

## Style

- Two-space indent, `snake_case` for functions and variables, `UPPER_SNAKE`
  for module constants.
- Keep `if ... then ... end` and `when ... then ...` on one line only when
  they are short.
- An expression can't continue on the next line, even inside parentheses
  (only call arguments and table literals can), and a line starting with
  `- 2` is silently a separate statement **(trap)**. Split a long
  condition into named `let`s.
- Comments say *why*, not what. Write a header comment for every module and
  a one-line comment for any function whose contract isn't obvious from its
  name.
- Keep functions short. Prefer a well-named helper to a long arm inside a
  `match`.

---

## Trap index

| Trap | What happens | Do instead |
|---|---|---|
| `let x = ...` inside a block, meant to update an outer `x` | new variable; the outer one never changes | `x = ...` without `let` |
| Module-level variable read before its `let` runs | `nil` | module-level `let`s at the top |
| Module-level `let` used as shared state | each process changes only its own copy | keep shared state in a process |
| Module-level state written from an `http` handler | per-connection copy, no `note:` | keep state in a process |
| A variable named `json`, `string`, `table`... | module hidden; runtime error | another name |
| Expression continued on the next line | parse error, or a silent separate statement | named `let`s |
| `delete(arr, i)` | hole in the array; `for` misses the last element | `remove_at(arr, i)` |
| `delete`/`remove_at`/`push` on what a `for` iterates | skips or adds entries, visits `nil` | iterate a snapshot (`keys(tbl)`) |
| `for x in record` | `x` is `nil` for every entry | `for k, v in record` |
| `a[len(a)] = x` in a loop | quadratic | `push(a, x)` |
| `t.x = nil` to remove a key | key stays | `delete(t, "x")` |
| `id in seen` on an int-keyed set | scans values | `has_key(seen, id)` |
| Boolean `sort` comparator | array left unsorted | return `a - b` |
| `json.encode({})` | `[]` | write `'{}'` yourself |
| Int-keyed table (not `0 .. n-1`) to `json.encode` **(bug)** | entries lost | string keys |
| `match` with no arm matching | yields `nil` silently | add an `else` |
| `when NAME` meant to compare with a variable | always matches, binds a new `NAME` | `when ^NAME` |
| `when x > 5`, `when "a" or "b"` | compares with a bool / one value | `if` chain |
| `nil` passed for a defaulted parameter | parameter is `nil` | leave the argument out |
| `pcall fn() ... end`, `pcall(f, x)` | runs nothing / drops `x` | `pcall f(x)`, `pcall do ... end` |
| Calling `main()` when `fn main` exists | runs twice | let the compiler call it |
| `2.0 == 2` | `false` | convert first |
| `to_int` on user input or a file line | raises | `trim`, then `pcall` |
| String accumulator read inside its loop | quadratic | `build_string` |
| `error(non_string)` | message becomes `"error"` | string message or result table |
| Re-raising with `error(r.error)` | original stack lost | log `r.stack` first |
| `loop(state)` followed by more statements | stack and memory grow until overflow | self call as the last expression |
| `warning: cannot reset ... back-edge` on a `while true` | memory grows without bound | restructure the loop |
| `{pid}` handle passed to `gen_server.call` | raises | `handle.pid` |
| gen_server callback returning `nil` | server crashes | `else` arm returning a result table |
| `self()` inside `spawn do ... end` to mean the parent | it's the child | `let parent = self()` before |
| `spawn(f, x)` | `f` called with no arguments | `spawn do f(x) end` |
| `link` to a process that may have exited | caller dies with `noproc` | `spawn_link` |
| Reply pattern without `^ref` | takes a stale reply | `ref: ^ref` |
| `receive()` or catch-all in a reply wait | steals other replies | selective `receive ... when` |
| Stale messages nobody matches | every `receive` slows down | catch-all in main loops |
| `after` in a busy server loop | never fires | `send_after` ticks |
| Monitoring a long-lived server from many short-lived processes | monitor list grows | monitor only when needed |
| `send` to a registered name whose process died | raises | `whereis` + check, or `pcall` |
| `spawn` past 1,023 live processes | raises; unguarded acceptor dies | catch it or cap connections |
| Spawning thousands of quick tasks in a loop | `process table full` | batch, or cap in-flight tasks |
| Blocking call (`sqlite_query`, DNS, plain `extern fn`) | all processes stall | keep short; `extern blocking fn` |
| Bad or closed sqlite handle **(bug)** | segfault, not catchable | close once, in the owning process |
| Handle opened, process crashes | fd leak | close on every path |
| `tcp_listen("localhost", ...)` | raises | `"127.0.0.1"` |
| `tcp_read` with no timeout, or `0` | blocks forever on a silent peer | pass a timeout |
