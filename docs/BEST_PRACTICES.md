# Gem Best Practices

How to write Gem that reads well, runs fast, and doesn't fall into the
runtime's traps. [`SPEC.md`](SPEC.md) says what the language *does*;
[`CHEATSHEET.md`](CHEATSHEET.md) is the syntax on one page; this file says
what to *reach for*.

Every rule and sample here was checked by running it with the compiler.
Timings are only meant as orders of magnitude. One that names no platform
was measured on Linux x86_64; the others name theirs. Two markers:

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

**Larger programs.** `examples/bookmark_app/` (an HTMX web app on
`std/http` and `std/sqlite`), `examples/stomp_broker/` (a message broker
built from `supervisor`, `dynamic_supervisor` and `gen_server`),
`examples/logstat/` (a command-line log analyzer in a `gem.toml` project),
`examples/mini_redis/` (a Redis-protocol server: one process owning
a large keyspace, a process per connection, pub/sub), `examples/lox/`
(an interpreter for the Lox language: a lexer, a recursive-descent parser
and a tree walker over tables), `examples/gemgrep/` (a recursive grep
on libc's regex through `extern fn`: a C object behind a `Ptr`, file
contents as `Bytes`), `examples/jobqueue/` (a job queue whose workers
crash, hang and get killed under a `dynamic_supervisor`: retries,
deadlines, restart intensity) and `examples/honeypot/` (a telnet
honeypot: a byte-level protocol parser fed across reads, a process per
connection that claims its socket, timeouts on every read and write, one
process that owns a sqlite database and batches its writes)
follow this doc and test themselves with
`std/test`; read them for how the pieces fit together.

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
  *back-edge*, the point where an iteration ends and the next begins, and
  when a recursive function returns.

**Block values.** `if`, `match` and `receive` give a value, and go
anywhere an expression does (`let x = if ...`, `print(if ...)`) except a
parameter default and a `when` arm's value. An expression can't continue
on the next line, even inside parentheses; only call arguments, table
literals and the lines of a block itself can span lines. Both come up
below.

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
  init: fn() {state: []} end,
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
  let store = gen_server.start(NOTES)
  http.serve(routes(store), {port: 8080})
end
```

`POST /notes` with `{"text": "hello big world"}` answers `{"id":0}`, and
`GET /stats` then answers `{"notes":1,"words":[3]}`. What it shows:

- The state lives in a process. Each HTTP connection runs in its own
  process, so a module-level `let notes = []` would give every connection
  its own copy ([State and memory](#state-and-memory)).
- `gen_server.start(...)` returns a `{pid}` handle, which `gen_server.call`
  and `gen_server.cast` take as it is (a bare pid or a registered name
  works too).
- A handler returns a response table (`http.json_response`,
  `http.bad_request`, ...). `return` inside a `do` block leaves the block,
  which here is the handler.
- User input goes through `pcall` and a type check before it is trusted.
- A handler that raises, or returns something other than a valid
  response (an int status from 200 to 999, no CR or LF in a header),
  answers 500; std/http prints the error and its stack on stderr.
  Request header names arrive lowercased: `req.headers["content-type"]`.
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
`total` that disappears at the end of the iteration, and `sum` returns `0`.
The compiler warns about a loop's `let` that reads the name it declares
when the code after the loop reads the outer one, and about a `let` in a
`while` body that hides a variable the loop's condition reads. A `let` in
an `if` branch meant to update an outer variable gets no warning.

Shadowing on purpose is fine. The new variable's initializer still sees
the old one, so `let n = n - 1` and `let line = string.trim(line)` work,
and inside a function a closure created before the second `let` keeps the
old variable. (At module level a second `let` rebinds instead; see
below.)

### Declare before the block, assign inside

A `let` is visible from its declaration to the end of the block it is in
(the body of a function, `if` branch, loop or `match` arm). Using it after
the block ends is a compile error, `undeclared identifier`, even when every
branch of an `if` declares it. Give the `if` the value instead:

```gem
let label = if n > 100 then "big" else "small" end
print(label)
```

When the block sets more than one variable, declare them before it and
assign them inside.

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
arrays only: on a table with any other key it raises (`for: one loop
variable needs an array or a string, ...`). Use `for k, v in t` or
`values(t)`.

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
For searching, call `find(s, needle, start)` (or `string.index_of`,
`split`, `contains`, which use it) rather than an `ord` loop: the search
runs in C, so a needle 1 MB in is found in well under 1 ms, against
about 25 ms for the simplest `ord` loop (5 ms on macOS arm64), and `split` cuts 100,000
ten-byte fields in about 6 ms. On short strings the call dominates: on a
115-byte line, a million `find` calls took 14 ms, a million
`string.index_of` calls 53 ms (macOS arm64).

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

### `if`, `match` and `receive` give values

The value is the last expression of the branch taken:

```gem
let kind = match code
when 200 then "ok"
when 404 then "missing"
else "error"
end
```

A block goes anywhere an expression does: a `let` (destructuring too), an
assignment (`x = `, `x += `, `t.k = `, `t[k] = `), a `return`, a call
argument, an operand, a table entry, an interpolation, and the last
statement of a function or block. A branch that ends in a statement, and
an `if` or `match` that takes no branch, gives `nil`. In `return match
...`, a call at the end of an arm is a tail call, so a process loop can be
written that way.

```gem
print("{n} item{if n == 1 then "" else "s" end}")
let total = base + match kind
when "a" then 1
else 2
end
```

Operands still run left to right (in `f(g(), if c then h() end)`, `g()`
runs first), and a block in the right operand of `and` / `or` runs only
when that operand is needed. A statement that starts with `if` is the
block alone: write `(if c then a else b end).name`, not `if ... end.name`
(a brace block's body is an expression and goes on).
A parameter default and a `when` arm's value can't be a block (a compile
error); compute the default in the body, and bind the `when` value first
and pin it (`when ^v`).

An arm fits on one line with `then` (`when 200 then "ok"`, `after 100 then
nil`), like a one-line `if`. Without `then`, the body must start on the
next line. `pcall` is an expression and goes anywhere: `f(pcall g(x))`,
`let r = pcall do ... end`.

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

Literals (`when 200`, `when "get"`) compare by value without a pin. A
name bound twice in one pattern (`when [x, x]`) is a compile error, not an
equality test: bind two names and compare them in the arm.
There are no guards or alternatives: `when v > 5` and `when "a" or "b"`
are compile errors. Use an `if` chain, or one arm per value.

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

A field default applies only when the field is missing, as a plain
default applies only when the argument is left out: `{port: nil}` gives
`port = nil`. So an option where `nil` means something (`timeout_ms: nil`
for no limit) needs no `has_key` check, and a caller that forwards an
option it may not have (`{port: opts.port}`) passes `nil`, not the
default: leave the key out instead. The trailing `= {}` makes the bag
itself optional, and an explicit `nil` for the bag also becomes `{}`;
without `= {}`, passing `nil` is an error. Destructured names are ordinary
locals that closures and `spawn` bodies can capture.

### Arity is checked only for direct calls

A call to a named fn, an `extern fn` or a module-level `let f = fn(...)`
that nothing reassigns, by name or as a module export (`json.encode(x)`),
with fewer arguments than its parameters without defaults, or more than
all of them (and no rest parameter), is a compile error. Make a parameter
optional with a default (`msg = nil`), not by leaving it out at the call.

A call through a value (a parameter, variable or table field holding a
fn, as in a callback) is not checked: extra arguments are dropped and
missing ones are `nil`, so a wrong call shows up later as a `nil`
somewhere else. An `extern fn` called through a value still raises
unless the count matches exactly.

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
loads keep the builtin. A module-level `fn` hides it in the whole file.
A module-level `let` hides it in every function and closure of the file,
those above it included, and in top-level code from the `let` on: its
own initializer still reaches the builtin (`let keys = keys(t)` works).
Do it only when the name is the module's API (`log.error`); elsewhere pick
another name. Such a module can still keep the builtin under another
name: bind it in top-level code above a `let` that defines the export
(`let raise = error`, then `let error = fn(msg) ... end`, as std/log
does). With `fn error`, nothing in the file reaches the builtin.

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

`pcall(f(x))`, with parentheses, is the function form: `f(x)` runs first,
as its argument, outside the protection, so its error is not caught.
Stick to `pcall f(x)` and `pcall do ... end`. (`pcall fn() ... end`,
which wouldn't run the closure, `pcall f` and `pcall(f, x)` are compile
errors.)

### `fn main` runs automatically

If the entry file defines `fn main`, the compiler calls it, with no
arguments, after the top-level code. Don't also call it yourself, or it
runs twice.

---

## Tables and arrays

### Append with `push`

```gem
push(items, x)                 # Prefer
items[len(items)] = x          # Over: the same thing, longer
items[count] = x; count += 1   # Over (and drop the separate counter)
```

On an array all three take the same time (1M appends: 40–60 ms).

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

In a table of more than 8 entries, string, int, float, bool and ref keys
are hashed: 20,000 sparse int ids
(`seen[id] = true`, then `has_key`) take 3 ms, the same ids as string
keys 9 ms (macOS arm64, M1 Pro). A table used as a key is found by a
linear scan, so a large set of tables is quadratic **(trap)**: adding
20,000 tables as keys took 0.27 s. Key such a set by an id the tables
carry.

Assigning `nil` doesn't remove a key: after `t.x = nil`, `x` is still in
`keys(t)`, `len(t)` and `json.encode(t)`. Use `delete(t, "x")`.

### Don't mix string keys into arrays

A table is either an array or a record. Mixing them makes `len`, `in`
and `for` unpredictable, and `json.encode` writes the whole table as an
object (`{"0":"z","name":"n"}`).

### Tables compare by identity

`{a: 1} == {a: 1}` and `[] == []` are `false`. Compare fields, or compare a
primitive key such as an id. The same applies to pinned patterns (pin
strings, numbers and refs, never tables). `test.assert_eq` is the
exception: it compares tables by structure.

### Tables and JSON

An empty table encodes as `{}` when it was made with braces (`{}`, a
record emptied with `delete`, an object from `json.parse`) and as `[]`
otherwise (`[]`, `keys(t)`), so start a record with `{}` and a list with
`[]`.

`json.encode` writes keys in insertion order, so two equal records built in
different orders encode differently. A table is a JSON array only when its
keys are exactly `0 .. n-1`; anything else is an object, with int keys
written as strings: `by_id[42] = "x"` encodes as `{"42":"x"}`, and parses
back with the string key `"42"`. Other key types (floats, bools) raise.

`json.parse` reads an integer too big for 64 bits as a float, which loses
digits past about 16 (`12345678901234567890` becomes
`1.2345678901234567e+19`). Send ids that big as strings. A number past
the float range (`1e309`) raises; one below it (`1e-400`) reads as `0.0`.

`json.parse` raises on malformed input; wrap it in `pcall` when the input
comes from outside.

### Out-of-range reads

Reading an array past its end gives `nil`, but a negative index past the
start raises, and any out-of-range string index raises (`"abc"[10]`).
Negative integers are always positions from the end (`arr[-1]` is the last
element), never keys: `t[-10] = x` raises on a table with fewer than 10
entries. Use string keys for data keyed by negative numbers.

### `sort` comparators return a number

`sort(arr, cmp)` sorts in place and expects `cmp(a, b)` to return a
negative number, zero or a positive number; a boolean comparator
(`fn(a, b) a < b end`) raises `sort: the comparator must return a number
...`:

```gem
sort(people, fn(a, b) a.age - b.age end)
```

`sort` and `table.sort` take only arrays; to order a record, sort
`keys(t)` or `values(t)`.

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
read from files first. A float outside the int range, an infinity or NaN
raises too, from `to_int`, `floor`, `ceil` and `round` alike. Wrap conversions of user input (route params,
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
round, a string threaded through recursion as an argument, a prepend
(`s = piece + s`), or a module-level `s` appended to inside a fn (or in a
top-level loop that calls a fn of yours) copies the whole string every
iteration and is quadratic (200 KB: about 1.2 s, against 2 ms for the
plain loop). For
anything non-trivial, use `build_string`, which has no such conditions:

```gem
let out = build_string do |add|
  for row in rows
    add(row.name, ",", row.value, "\n")
  end
end
```

`add` takes any number of values and appends each as `to_string` would
(a buffer appends its contents, as `buf_push` does). Use a
buffer (`buf_new()`, `buf_push(buf, s)`, `to_string(buf)`) when the text
has to be built across several functions or loop iterations that a single
block can't hold, such as a read loop that collects chunks. `print(buf)`
and `"{buf}"` show `<buffer:N>`, not the contents.

### Strings are bytes

`len` is the byte count (`len("é")` is `2`), `s[i]` is a 1-byte string,
and `ord(s, i)` is the byte value. `for ch in s` walks the bytes as
1-byte strings, and `for i, ch in s` adds the 0-based byte index. Use `substr(s, start, count)` to slice;
unlike `s[-1]`, a negative `start` counts as `0`. Double-quoted strings
have no `\x` or `\u` escapes (an unknown escape is kept as written); use
`chr(n)` for other bytes. Strings may contain `\0`, and `print` writes
them whole.

---

## Errors

### `error` takes a string

`error({code: 404})` raises the string `"{code: 404}"`: `pcall`'s `error`
is always a string, so `r.error.code` doesn't work. When callers need
structure, return a result table instead of raising.

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

### Don't grow a module-level table while spawning **(trap)**

Each `spawn` hands the child a copy of every module-level variable that
changed since the previous spawn, whether the child reads it or not (made
at the spawn, kept until the child exits). Top-level code that collects pids in a module-level
array therefore copies the whole array for every child: 3,000 spawns take
290 ms, 10,000 take 11 s and 2.6 GB on Linux x86_64 (0.41 s, and 1.6 s
and 3.3 GB, on macOS arm64). Keep the table in a local of a function
(`fn main` runs automatically), and the same 10,000 spawns take 0.2 s
(0.39 s on macOS arm64).

```gem
fn main()
  let pids = []                      # Prefer: a local
  for i = 0, 10000
    push(pids, spawn(fn() receive() end))
  end
  for p in pids
    kill(p, "kill")
  end
end

# Over, at the top level:
#   let pids = []
#   for i = 0, 10000
#     push(pids, spawn(fn() receive() end))   # copies pids into each child
#   end
```

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

### A process holding a lot of data pauses every process **(trap)**

A long-running loop copies what it keeps alive at its arena resets, and
processes share one thread, so nothing else runs meanwhile. Most resets
copy only what the loop made since the last one: what a reset keeps is
promoted, and later resets leave it alone. But now and then a full reset
copies all of it again, to drop what the loop no longer holds. It comes
once the kept data has doubled since the last one (grown fourfold, when
that one found little garbage). A process whose state grows inside its
loop (a gen_server's state, made after `init`) so pauses every process
for as long as one copy of that state takes. A process that keeps one
small record per message it gets pauses for up to 0.04 s at 100,000
records and 0.25 s at 300,000 (`GEM_DIAG=1`, `max=`; Linux x86_64 VM).
`examples/jobqueue`'s queue, which keeps a record per job, pauses up to
0.09 s at 20,000 jobs and 0.25–0.5 s at 100,000 (up to 0.02 s and
0.08–0.09 s on macOS arm64, `benchmarks/baselines/2026-10-07_m1pro`). A timer set to 100 ms can
then fire before a 20 ms job has had a chance to report. Bound what a
long-lived process keeps (expire finished records, keep a count instead
of a history), or split it across processes, and leave deadlines room for
the pauses. A long-running program can watch for them: `runtime_stats()`
counts the resets that took 1, 10 and 100 ms or more
(`resets_over_10ms`, ...), and `process_info(pid).memory` shows which
process holds the data.

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
catches like any error. `json.parse` and `json.encode` refuse nesting
deeper than 1,000 levels (and `json.encode` so stops on a cyclic table). For
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

### Spawning

```gem
let parent = self()
spawn do                       # or spawn(fn() ... end)
  send(parent, {tag: "done", value: work(5)})
end
```

- Inside the `spawn` body, `self()` is the child. Take the parent's pid
  before spawning, as above.
- Pass arguments through the closure: `spawn do worker(5) end`
  (`spawn(worker, 5)` is a compile error).
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
  catch-all for them. Never in a task: a `task.await` timeout can't kill a
  task that traps exits, so it runs on and its result and `DOWN` are left
  in the owner's mailbox.

`pcall` doesn't catch a `kill` or a link's exit: those end the process at
once.

To stop another process, `kill` it with a reason other than `"normal"`
(`"shutdown"` is the convention). As in Erlang, a `"normal"` exit signal
from another process is ignored unless the target traps exits;
`kill(self(), "normal")` does end the caller.

A process that traps exits (a supervisor, an http server) handles a
`kill` when it next reads its mailbox, so it is still alive, and its name
still registered, when `kill` returns (except for `kill(pid, "kill")`,
below, which also skips its cleanup). To stop one and know it is gone,
monitor it and wait for its `DOWN`, which is what `supervisor.stop`,
`dynamic_supervisor.stop` and `http.stop` do:

```gem
let pid = sup.pid
monitor(pid)
kill(pid, "shutdown")
receive
when {tag: "DOWN", pid: ^pid} then nil
after 5000 then error("supervisor did not stop")
end
```

### A process that traps exits: `"shutdown"` first, `"kill"` last

`kill(pid, "kill")` can't be trapped: the target dies at once with reason
`"killed"`, which its monitors and links see (a link passes `"killed"` on
as an ordinary reason, so a linked process that traps exits gets an
`EXIT`). Give a process the chance to clean up first: send `"shutdown"`,
wait for its `DOWN` with a deadline, then `"kill"`. Supervisors do that
for you, with each child's `shutdown` budget (5000 ms unless the child
spec says otherwise; a nested supervisor waits with no limit, since its
handle carries `shutdown: nil`).

A supervised child that traps exits should return on the `EXIT` from its
supervisor with reason `"shutdown"`; otherwise it is killed when its
budget runs out. The child spec's `start` runs in the supervisor, so
`self()` there is the supervisor's pid:

```gem
fn start_worker()
  let sup = self()                 # `start` runs in the supervisor
  spawn do
    process_flag("trap_exit", true)
    let running = true
    while running
      receive
      when {tag: "EXIT", pid: ^sup, reason: reason}
        running = reason == "normal"   # flush, close files, then return
      when other
        handle(other)
      end
    end
  end
end

supervisor.start({children: [{id: "w", start: start_worker, shutdown: 2000}]})
```

Set `shutdown:` to how long the child's cleanup can take. Only give
`nil` to a child that is sure to exit: one that never does keeps its
supervisor waiting for good, and with it `supervisor.stop`, which waits
with no limit by default.

### A process traps exits only once it has run **(trap)**

`process_flag("trap_exit", true)` in a `spawn` body takes effect when the
new process gets to run that line. An exit signal sent before then, by a
`kill` right after the `spawn` or a starter that stops at once, kills it
as if it didn't trap, and its cleanup never runs. When the cleanup
matters (a buffer to write out), have the starter wait for the child to
say it traps:

```gem
fn start_worker()
  let parent = self()
  let ref = make_ref()
  let pid = spawn do
    process_flag("trap_exit", true)
    send(parent, {tag: "ready", ref: ref})
    serve()
  end
  receive
  when {tag: "ready", ref: ^ref} then pid
  end
end
```

A `gen_server` that sets the flag in `init` needs nothing more:
`gen_server.start` returns after `init`.

### Don't pass an option's `shutdown` through as `nil` **(trap)**

In a child spec, a missing `shutdown` key means the default budget, but
`shutdown: nil` means no limit, like `timeout_ms: nil` elsewhere in std.
`{id: id, start: s, shutdown: opts.shutdown}` sets the key to `nil` when
`opts` has no `shutdown`, so a stubborn child keeps its supervisor waiting
for good. Copy the key only when it is there, which leaves the
supervisor's own default in place (5000 ms, or the budget the child's
handle asks for):

```gem
fn worker_spec(id, opts)
  let spec = {id: id, start: start_worker}
  if has_key(opts, "shutdown")      # not `shutdown: opts.shutdown`
    spec.shutdown = opts.shutdown
  end
  spec
end
```

Or give the option a default when you read it: a destructuring default
fires only on a missing key, so an explicit `nil` still passes through
(`fn worker_spec(id, {shutdown = 5000} = {})`), at the cost of fixing the
default yourself.

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
  `DOWN` when it's done. Monitoring a long-lived server from many
  short-lived processes, such as per-connection handlers, is fine: a
  monitor ends with the process that set it up (after 20,000 callers in
  turn, `process_info(server).monitors` is empty and the runtime's list
  holds at most one stale entry).
- A monitor taken for one request should end with it, or the caller gets
  the server's `DOWN` whenever it dies, long after the request. A process
  monitors a live target at most once, so `monitor` returns `false` when
  the caller already monitors it; remove the monitor afterwards only when
  `monitor` returned `true`, and drop the `DOWN` it may have delivered.
  `gen_server.call` works this way:

  ```gem
  let added = monitor(pid)
  # ... send, then receive the reply or the DOWN ...
  if added and not demonitor(pid)
    receive
    when {tag: "DOWN", pid: ^pid} then nil
    after 0 then nil
    end
  end
  ```

  `demonitor` returns `false` once the target has died, because its
  `DOWN` is already in the mailbox. If the request got the `DOWN` and
  `added` is `false`, send it back to `self()` for the caller's own
  monitor. A dead target sends a `DOWN` (`"noproc"`) on every `monitor`.
- A `receive` with only an `after` clause waits that long and takes no
  message: anything that arrives meanwhile stays queued. It is the same as
  `sleep(ms)`; use whichever reads better.
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

### A process that makes calls doesn't also collect a stream **(trap)**

A request/reply wait (`gen_server.call`, a `receive` on `^ref`) scans the
whole mailbox for its reply, messages that will be read later included.
A process that makes calls while another process streams messages to it
(notifications, results, a server telling it each job is done) therefore
scans the growing stream on every call: quadratic. 10,000 calls to a
server that also sends the caller a note per call took 14 s; the same
calls made from a process of their own took 68 ms (Linux x86_64 VM;
`examples/jobqueue`'s driver: 21 s against 0.7 s for 10,000 jobs).
Split the two roles:

```gem
let me = self()
spawn do                              # Prefer: the calls in their own process
  for i = 0, n
    gen_server.call(server, {n: i, notify: me})
  end
end
for i = 0, n                          # this process only collects
  receive
  when {tag: "done"} then nil
  end
end

# Over: one process calls and collects
#   for i = 0, n
#     gen_server.call(server, {n: i, notify: self()})
#   end
#   ... then receive the n notes
```

### Registered names

`register("db", pid)` lets other processes `send("db", msg)` and
`whereis("db")`. Register a process from its parent, after `spawn`: a
child that registers itself may not have run yet when the parent sends.
A gen_server's `init` can register it: `gen_server.start` returns after
`init`.
`send` to a dead pid silently drops the message, but `send("db", msg)`
raises `send: no process registered with that name` once the process has
died (say, while a supervisor restarts it). Where that can happen, check
`whereis` for `nil`, or `pcall` the send.

### A child's `start` and a server's `init` must not wait for their starter **(trap)**

`supervisor.start` returns once the supervisor has called every child's
`start`, and `gen_server.start` once the server's `init` has returned, so
a name registered there works right after, and a failure there makes the
call raise (`supervisor.start: child "db" failed to start: ...`,
`gen_server.start: init failed: ...`). While it waits, the caller
handles no messages: a `start` or `init` that sends it a request and
waits for the answer gets none. When no other process can run, the
program stops with `deadlock: main process is waiting in receive ...`;
otherwise both wait for good, or until the request times out. Under a
supervisor, the supervisor is the one starting a gen_server, so its
`init` must not ask the supervisor anything (`which_children`) either.
Pass what a child needs in its spec (`start: fn() start_worker(config)
end`), or do the work after `start` returns: send the server a message
from `init` and handle it in `handle_info`.

```gem
init: fn()
  send(self(), "connect")          # handled after start returns
  {state: {db: nil}}
end,
handle_info: fn(msg, s)
  if msg == "connect"
    s.db = connect()
  end
  {state: s}
end,
```

A slow `init` holds up its starter the same way: a supervisor starts its
other children after it, and a dynamic supervisor answers no other
request meanwhile; a `dynamic_supervisor.start_child` whose `init` takes
longer than its `timeout_ms` (5000 by default) raises `timeout`,
although the child does start.

### Only the pending call learns why a server died **(trap)**

A `gen_server.call` waiting when the server dies raises
`gen_server.call: server exited: <reason>`. A later call by pid raises
`gen_server.call: server exited: noproc`, and one by registered name
`gen_server.call: no process registered as "<name>"`. An error that
kills it is printed on stderr when it happens; to act on the reason in
code, monitor the server, whose `DOWN` carries it.

### `spawn` can fail: catch it where load decides **(trap)**

Processes are cheap (an idle one takes about 21 KB on Linux x86_64, 70 KB
on macOS arm64), and a program can keep thousands alive (tens of
thousands where memory mappings allow), but not an unbounded number. `spawn` raises when it can't start one:

- `spawn: process table full` past the process limit: 262,144 by default,
  lower when the environment sets `GEM_MAX_PROCS`;
- `spawn: too many processes for the system's memory-mapping limit (...)`
  on Linux, which comes first: at about 14,000 live processes with the
  default `vm.max_map_count` (65,530), leaving the rest of the limit to
  the processes already running. Raise that sysctl to go further;
- `spawn: cannot map a stack for a new process (N processes alive)` when
  the system refuses the memory (a `ulimit -v`).

Spawning 5,000 one-line tasks in a loop is fine. What needs care is
spawning in proportion to outside load: an acceptor that spawns per
connection should catch the error and close the connection (std/http
answers 503 and keeps accepting), or one burst kills the acceptor. Fan-out
over a list of unknown length (`task.async` per item) should cap how many
are in flight.

```gem
let r = pcall spawn(fn() handle(fd) end)
if not r.ok
  tcp_close(fd)        # refuse this one, keep accepting
end
```

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
| `read_file`, `write_file`, `append_file`, `exec`, `sqlite_open`, `sqlite_close`, `extern blocking fn` (4-thread pool) | `sqlite_query`, `sqlite_exec`, and the close of a database at its owner's exit |
| | `file_exists`, `is_dir`, `list_dir`, `mkdir`, `remove_file`, `normalize_path` |
| | plain `extern fn`, `input`, `read_stdin`; `print`, `eprint`, `write_stdout` to a slow pipe |

Keep sqlite queries short and indexed: a one-second query stalls every
process for that second. Use `extern blocking fn` for any C call that can take more than
about a millisecond. The pool has 4 workers, so four long `exec` calls
delay every file read behind them.

### Claim a resource in the process responsible for it

Sockets and sqlite handles belong to the process that opened them. One
it never claimed is closed when that process crashes or is killed, and
left open (with no owner) when it returns. `claim(r)` moves a resource
to the calling process and ties it to that process's life: it is closed
when the claimer exits for any reason, an early `return` or an orderly
`"shutdown"` included. So the process that serves a connection claims
it as its first line, wherever the socket came from:

```gem
fn acceptor(listener, registry)
  while true
    let sock = tcp_accept(listener)
    send(registry, {sock: sock, peer: tcp_peer(sock)})
  end
end

fn session(sock)
  claim(sock)               # any exit of this process now closes sock
  let line = tcp_read(sock, 4096, 30000)
  ...
end
```

Helpers that only use a socket (a reader process, a `task.async` that
writes to it, a watchdog that closes it on idle) don't claim it, and
their crash, kill or return leaves it to its owner. Any process may
`tcp_close` it. A session that forgets its `claim` leaves its socket
with the acceptor: it stays open after the session crashes, for as long
as the acceptor lives. Watch
`process_info(acceptor).resources`, which counts what a process owns, or
run with `GEM_DIAG=2`, which prints a `gem_resources:` line for every
process that exits leaving resources open.

For a short exchange in one process, open and close in the same
function: a crash or kill closes it anyway, and a normal return would
otherwise leave it open until the program ends.

```gem
let sock = tcp_connect(host, port)
let r = pcall request_once(sock)
tcp_close(sock)
if not r.ok
  error(r.error)
end
r.value
```

Stop an `http.start` server with `http.stop(server)`: it closes the
listening socket, so the port is free again (killing it with `"kill"`
does too).

### A restartable child opens what it claims **(trap)**

A supervised child that claims a resource it got from its child spec
closes it with its first crash, and every restart then fails on the
closed resource (`claim: socket is closed`, `tcp_accept: socket is
closed`) until the supervisor reaches its restart intensity. Either the child opens the resource itself (a restarted child
opens a fresh listener, as std/http's server process does), or it only
uses the parent's resource without claiming it, so a crash leaves it
open for the restart:

```gem
# The listener stays with main, which opened it; each restarted acceptor
# uses it and claims only the sockets it accepts.
let listener = tcp_listen("0.0.0.0", 2323)
supervisor.start({
  strategy: "one_for_one",
  children: [{id: "acceptor", start: fn() spawn_link(fn() acceptor(listener) end) end}]
})
```

### Reading input line by line

`input()` returns the next line of stdin, of any length and NUL bytes
included, without its `\n` or `\r\n`, and `nil` at the end. There is no
way to read a *file* a line at a time: `read_file` and split it yourself
(see `examples/logstat/lib/source.gem`), which holds the whole file in
memory, twice while it is read.

### Write large output in one piece

`print` flushes after every line, so each line is a `write` system call:
a million lines to a file took 0.6 s with `print` against 0.3 s with one
`write_stdout` of a string built with `build_string`, and 0.73 s with
`print` into a pipe (Linux x86_64 VM). On macOS arm64 the gap is wider: 2.7 s with `print`
to a file and 0.8 s into a pipe, against 0.16 s with one `write_stdout`.
For bulk output, build a block of lines and write it once:

```gem
write_stdout(build_string do |add|
  for line in lines
    add(line, "\n")
  end
end)
```

### Pass timeouts to reads, check writes

- `tcp_read(fd, n, timeout_ms)` returns `""` at end of stream and `nil` on
  timeout. Library code should always pass a timeout; without one, a silent
  peer blocks the caller forever. As with `after`, a timeout of `0` or
  less doesn't wait: it returns what's already there, or `nil`.
- `tcp_write(fd, s, timeout_ms)` returns the number of bytes written and
  does not raise when the peer has gone or the timeout passes. Treat
  `tcp_write(fd, s, ms) < len(s)` as "connection lost or too slow", but
  expect the first write after a disconnect to still report success; only
  later writes show it. Without a timeout, a peer that stops reading
  blocks the writer for as long as it keeps the connection open.

### `tcp_peer` is `nil` once the peer has reset **(trap)**

A client that connects and resets at once (scanners and bots do it all
the time) leaves a socket with no peer address by the time the acceptor
asks, so `tcp_peer(fd).ip` raises `field access on non-table: got nil`
and kills the acceptor. Later calls can return `nil` too: a client that
closed cleanly loses its address as soon as a write to it draws a reset.
Read the address once, right after `tcp_accept`, check for `nil`, and
pass the value on:

```gem
let peer = tcp_peer(fd)
if peer == nil
  tcp_close(fd)        # gone before we looked
else
  handle(fd, peer.ip)
end
```

---

## Modules

### Layout

```gem
## thing — one line on what it is for.
##
##   let x = thing.make(...)      # a short usage example
##   thing.use(x)

load "std/string"

let DEFAULT_LIMIT = 100

fn helper(...)               # private: not in the export list
end

## Makes a thing from ...
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
- Name module files in `snake_case`: the namespace is the file's base
  name, so `load "./my-utils"` is a compile error (``module file name
  `my-utils` is not an identifier``); `load "./my-utils" as my_utils`
  works. A module named like a std module (`json.gem`) is fine, even when
  std loads its own (`std/http` loads `std/json`): they are different
  modules. Only one file loading both is an error (``module name `json`
  is already used by ...``); load one of them with `as`.
- Modules can't load each other in a cycle: the compiler reports
  `load cycle: a.gem → b.gem → a.gem`. Move shared code into a third
  module.

### Document the public API with `##`

A comment that starts with `##` is a doc comment. It is for *callers*:
the module header and the block directly above each exported function.
A plain `#` comment is for whoever *maintains* the code: why it's shaped this way,
invariants, performance internals. To decide which one a sentence
belongs in, ask: **would a caller write different code if they knew
this?** "Returns -1 when there is none" goes in `##`; "searches growing
chunks in C, then bisects" goes in `#`.

A function's doc, in this order, leaving out what doesn't apply:

1. **One summary sentence** that starts with a verb ("Splits...",
   "Returns..."). Don't restate the signature: the editor shows it.
2. **Results and edge cases**: what comes back for empty input, not
   found, `nil`, a negative count. Name arguments in backticks.
3. **Examples**, indented two spaces past the text, as
   `call    # result`.
4. **Raises**: when it calls `error()`, for conditions the module
   header doesn't already cover.
5. **Process effects**: whether it blocks, spawns, links or monitors,
   what it can leave in the caller's mailbox, and what happens if the
   caller or the target dies.
6. **Cost**, only when the caller can see it and act on it ("O(n) per
   call; build a table for repeated lookups").

```gem
## Returns the byte offset of the first `needle` in `s` at or after
## `start`, or -1 when there is none.
##
## A negative `start` counts as 0. An empty `needle` is found at `start`
## (-1 when `start` is past the end).
##
##   string.index_of("hello", "l")       # 2
##   string.index_of("hello", "l", 3)    # 3
##
## Raises if `start` is not an int.
fn index_of(s, needle, start = 0)
```

The module header is the module's front page: the `##` line
`name — what it is for.`, a usage example, and the conventions that hold
for every function (argument types, units, what errors look like), so
each function's doc doesn't repeat them. A small module needs about a
dozen lines; a large one (std/http, the OTP modules) can need two or three
times that, and that's fine as long as every line is a rule that holds
across functions. A rule about one function goes on that function. A
design overview that only maintainers need goes in a `#` block after the
`load`s.

Private functions don't get `##`. Give one a `#` comment when its
contract isn't obvious from its name.

### Accept a pid or a `{pid}` handle in one place

When your own API takes a process, accept both a bare pid and the
`{pid: pid}` handle that std's `start` functions return, and normalize it
once in a helper; don't repeat `if type(t) == "table"` in every function.

---

## C interop

The full rules are in [`SPEC.md`](SPEC.md) ("C Interop"); these are the
ones that bite.

- Put the C code in a header of `static` functions and write
  `extern include "<path>"` before the `extern fn` declarations, with
  the path relative to the `.gem` file (`"support/helpers.h"`; in a
  loaded module, relative to the module). The header comes before
  `gem.h`, so include `"gem.h"` in it if it uses `GemVal` or `GemBytes`.
  The program links only libc, libm and pthreads.
- For a libc function, `extern include` its header (`"stdio.h"` for
  `puts`). Without any `extern include`, the compiler writes its own
  prototype from the extern types, which clashes with libc's.
- Types are checked, not converted: a `Float` parameter rejects `2` (pass
  `2.0` or `to_float(n)`). The argument count must match exactly: too few
  or too many raise. `extern blocking fn` can't take or return a `Table`.
- A `Ptr` is an int in Gem, and `NULL` comes back as `0`, not `nil`.
  `0` is truthy, so `if not p` never catches a `NULL` **(trap)**: compare
  with `p == 0`.
- `String` parameters arrive as `const char *` and stop at the first `\0`.
  Use `Bytes` for binary data. A plain `extern fn` gets a pointer into
  the string itself, not a copy, so passing a large string with offsets
  (`data: Bytes, start: Int, stop: Int`) costs the same as passing a
  short one; slicing it with `substr` first copies the slice.
- Returned strings: a plain `extern fn` must return static memory (the
  runtime copies it and does not free it); an `extern blocking fn` must
  return `malloc`'d memory (the runtime frees it). A `NULL` return is
  `nil` from a plain `extern fn` but `""` from an `extern blocking fn`.
- Don't keep pointers to Gem strings or tables on the C side after the call
  returns: the next arena reset or the process's exit frees that memory,
  and an `extern blocking fn` gets copies that are freed when it returns.
- Pass a socket to C as a `Socket` parameter, not `tcp_fd(sock)` as an
  `Int`: the wrapper checks it is open, and an `extern blocking fn` keeps
  its fd open until the call returns even if another process closes the
  socket meanwhile. With an `Int`, C code can write to whatever took the
  number after the close.
- C code that recurses without limit kills the process, and `pcall` can't
  catch it.

### Plain `extern fn` for quick calls, `extern blocking fn` for waits **(trap)**

A plain `extern fn` runs on the scheduler thread: no process runs until
it returns. An `extern blocking fn` runs on the 4-thread pool while the
caller waits and other processes run, but every call pays a hand-off to
a worker and back, and copies every `String` and `Bytes` argument. Called
once per line, that is ruinous: `regexec` on each of 19,000 lines of a
1 MB file took 5 ms as a plain `extern fn`, 0.64 s as an `extern blocking
fn` given each line, and 1.7 s given the whole file each time (Linux
x86_64; `examples/gemgrep`).

```gem
extern include "unistd.h"
extern fn write(fd: Int, data: Bytes) -> Int      # quick: a plain call, inline
extern blocking fn fsync(fd: Int) -> Int           # can wait for the disk: the pool
```

Use `extern blocking fn` for calls that wait (I/O, locks, the network) or
run for more than about a millisecond, and a plain `extern fn` for short
calls in loops. A plain call from a loop holds the other processes only
for one call, since the loop's back-edge lets them run; one call that
runs long holds them for all of it (glibc's `regexec` with a
backreference can take seconds on one line).

### A `Ptr` is a number, not an owner **(trap)**

A C object behind a `Ptr` (a `FILE *`, a compiled regex, a handle a
library gave you) is not freed when the process holding it dies: unlike a
socket or a sqlite handle, it is not an owned resource. And `spawn` and `send` copy the number, not the
object: two processes then share one object, and when one frees it the
other holds a dangling pointer, which crashes or corrupts memory rather
than raising. Make, use and free a C object in one process, free it on
every path, and give other processes what they need to make their own:

```gem
fn file_magic(path)
  let f = fopen(path, "rb")
  if f == 0
    return nil
  end
  let r = pcall magic(f)          # Gem code that may raise
  fclose(f)                       # on every path
  if not r.ok
    error(r.error)
  end
  r.value
end

for path in paths
  push(tasks, task.async(fn() file_magic(path) end))   # each task opens its own
end
```

When the free function tolerates it, zero the handle after freeing
(`h.ptr = 0`) and check for `0` before each use, so a use after the free
raises in Gem instead of reaching C (`examples/gemgrep/regex.gem`).

---

## Tests

- Use `std/test` (`test.case`, `test.assert`, `test.assert_eq`,
  `test.assert_neq`, `test.assert_throws`, `test.run()`) for checks that
  verify themselves. `assert_eq` compares tables by structure (same keys,
  equal values, any insertion order; cycles, shared subtables and deep
  nesting are fine) and
  everything else with `==`, so `1` doesn't equal `1.0`. A failure shows
  both values, the path to the first difference, and the types when they
  differ: `expected {a: [1, 2]}, got {a: [1, 2.0]}: at .a[1], expected 2
  (int), got 2.0 (float)`. `assert_eq` and `assert_neq` take an optional
  message, shown after the prefix (`test.assert_eq(n, 3, "count")`).
  `assert_throws` returns the error message, so check it with `assert_eq`
  when it matters.
- Register cases with `test.case` at the top level (or from `main`), in
  the process that calls `test.run()`; `test.case` raises when called in a
  spawned process, whose copy of the case list `test.run()` would never
  see.
  `test.run()` calls `exit(1)` when a case fails, which ends the whole
  program with status 1, so nothing after it runs.
- A test that spawns a process should `spawn_monitor` it (or use `task`)
  and check the result or `DOWN`. With `spawn_link`, a crashing child kills
  the test runner, and the remaining cases never run.
- Cover the edges as well as the happy path: empty input, a single element,
  `nil` where a table is expected, deep nesting, timeouts, and a process
  dying mid-request.

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
- Comments say *why*, not what. Document every module and every exported
  function with `##` (see
  [Document the public API with `##`](#document-the-public-api-with-)),
  and give a private function a `#` comment when its contract isn't
  obvious from its name.
- How much to comment depends on who reads the code. A library (std, or
  a module other code loads) has callers who don't read its source, so
  its `##` docs are complete. Application code and examples are read as
  code: a one-line `##` per module and exported function is enough, and a
  `#` comment earns its place only by saying what the code can't, such
  as a design choice that's easy to undo by mistake or a Gem behaviour
  the reader wouldn't expect (a lookup that blocks every process, a
  process that traps exits so it can close its socket). Don't narrate
  what the next line does.
- Keep functions short. Prefer a well-named helper to a long arm inside a
  `match`.

---

## Trap index

| Trap | What happens | Do instead |
|---|---|---|
| `let x = ...` inside a block, meant to update an outer `x` | new variable; the outer one never changes (a warning only in loops) | `x = ...` without `let` |
| Module-level variable read before its `let` runs | `nil` | module-level `let`s at the top |
| Module-level `let` used as shared state | each process changes only its own copy | keep shared state in a process |
| Module-level table pushed to between spawns | copied into every child: O(n²) time and memory | keep it in a local of `fn main` |
| Module-level state written from an `http` handler | per-connection copy, no `note:` | keep state in a process |
| A variable named `json`, `string`, `table`... | module hidden; runtime error | another name |
| Expression continued on the next line | parse error, or a silent separate statement | named `let`s |
| `delete(arr, i)` | hole in the array; `for` misses the last element | `remove_at(arr, i)` |
| `delete`/`remove_at`/`push` on what a `for` iterates | skips or adds entries, visits `nil` | iterate a snapshot (`keys(tbl)`) |
| `t.x = nil` to remove a key | key stays | `delete(t, "x")` |
| `id in seen` on an int-keyed set | scans values | `has_key(seen, id)` |
| Large set or index with tables as keys | quadratic | key by an id the tables carry |
| `match` with no arm matching | yields `nil` silently | add an `else` |
| `when NAME` meant to compare with a variable | always matches, binds a new `NAME` | `when ^NAME` |
| `nil` passed for a defaulted parameter or option field | parameter (field) is `nil` | leave the argument (key) out |
| `pcall(f(x))` | `f(x)` runs outside the protection: not caught | `pcall f(x)`, `pcall do ... end` |
| Calling `main()` when `fn main` exists | runs twice | let the compiler call it |
| `2.0 == 2` | `false` | convert first |
| `to_int` on user input or a file line | raises | `trim`, then `pcall` |
| String accumulator read inside its loop | quadratic | `build_string` |
| Re-raising with `error(r.error)` | original stack lost | log `r.stack` first |
| `loop(state)` followed by more statements | stack and memory grow until overflow | self call as the last expression |
| `warning: cannot reset ... back-edge` on a `while true` | memory grows without bound | restructure the loop |
| gen_server callback returning `nil` | server dies, the `call` raises | `else` arm returning a result table |
| `self()` inside `spawn do ... end` to mean the parent | it's the child | `let parent = self()` before |
| `link` to a process that may have exited | caller dies with `noproc` | `spawn_link` |
| Reply pattern without `^ref` | takes a stale reply | `ref: ^ref` |
| `receive()` or catch-all in a reply wait | steals other replies | selective `receive ... when` |
| Stale messages nobody matches | every `receive` slows down | catch-all in main loops |
| Calls from a process that also collects a stream of messages | every reply wait scans the stream: quadratic | make the calls from a separate process |
| A long-lived process holding 100,000s of records | full resets copy them all and stall every process (0.04–0.5 s) | bound or shard the state |
| `after` in a busy server loop | never fires | `send_after` ticks |
| `process_flag("trap_exit", true)` in a fresh `spawn` body, killed at once | dies before the line runs; no cleanup | the starter waits for a "ready" message |
| `shutdown: opts.shutdown` in a child spec | a missing option becomes `nil`: no limit, the supervisor can wait for good | copy the key only when `has_key` |
| Monitoring a server for one request and not removing it | its `DOWN` arrives whenever the server dies | `demonitor` when `monitor` returned `true` |
| `send` to a registered name whose process died | raises | `whereis` + check, or `pcall` |
| A child's `start` or a gen_server's `init` waiting for an answer from its starter | deadlock error, or a wait for good or until a timeout | pass it in the spec, or `send(self(), ...)` in `init` |
| `gen_server.call` to a server that has already died | `server exited: noproc` (by name: `no process registered`), not why it died | monitor it: the `DOWN` has the reason |
| `spawn` per connection or per item, unguarded | raises at the process or memory limit (~14,000 on stock Linux); the acceptor dies | catch it, or cap in-flight work |
| Blocking call (`sqlite_query`, DNS, plain `extern fn`) | all processes stall | keep short; `extern blocking fn` |
| `extern blocking fn` called once per line or item | a thread hand-off and argument copies per call: 100x slower | plain `extern fn` for quick calls |
| `if not p` on a `Ptr` | `NULL` is `0`, which is truthy | `p == 0` |
| A `Ptr` sent, captured by `spawn`, or left when its process dies | shared or leaked C object; use after free | one process makes, uses and frees it, on every path |
| Socket handed to a session that doesn't `claim` it | stays open after the session crashes (with the acceptor, or with no owner) | `claim(sock)` first in the session |
| Supervised child claims a resource from its spec | its first crash closes it; every restart raises on it | the child opens its own, or doesn't claim |
| `tcp_read` with no timeout | blocks forever on a silent peer | pass a timeout |
| `tcp_peer(fd).ip` | `nil` once the peer has reset, even after a clean close; field access raises | read it once after `tcp_accept`, check for `nil`, keep the value |
