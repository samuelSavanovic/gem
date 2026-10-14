# Gem Best Practices

How to write Gem that reads well, runs fast, and doesn't fall into the
runtime's traps. [`SPEC.md`](SPEC.md) says what the language *does*;
[`CHEATSHEET.md`](CHEATSHEET.md) is the syntax on one page; this file says
what to *reach for*.

Every rule and sample here was checked by running it with the compiler.
Timings are orders of magnitude, measured on Linux x86_64 unless they say
otherwise. Two markers:

- **(trap)**: fails silently, crashes, or falls off a performance cliff.
  Treat it as a hard rule.
- **(bug)**: the rule exists only because of a compiler, runtime or std bug
  listed in [`KNOWN_BUGS.md`](KNOWN_BUGS.md). When the bug is fixed, the
  rule goes.

Mistakes the compiler reports by name (a `let` in a nested block hiding a
variable of the same function, an unpinned name in a pattern, a statement
that does nothing) aren't repeated here: the message says what to do.

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

**Running.** `gem prog.gem` builds at `-O0`, caches and runs it; `-o prog`
builds an optimized binary (1.5–1.75x faster on Linux x86_64, 2.3–2.8x on
macOS arm64: measure with it); `--check` prints the `note:`/`warning:` lines.
`gem new`/`test`/`build` work on a `gem.toml` project. `argv()[0]` is the
program (after `--`, options go to it; `std/args` parses them); `getenv(name)`
reads the environment.

**Builtins and std modules.** Builtins (`print`, `len`, `push`, `keys`,
`spawn`, `send`, ...) are always there; the list is in
[`CHEATSHEET.md`](CHEATSHEET.md). The rest lives in std modules: `load
"std/json"` binds `json.parse`, `json.encode`, and so on. The modules are
`string`, `table`, `math`, `time`, `log`, `json`, `url`, `mime`, `http`,
`request`, `sqlite`, `io`, `args`, `regex`, `base64`, `hex`, `crypto`,
`utf8`, `csv`, `task`, `gen_server`, `supervisor`,
`dynamic_supervisor` and `test`.

**Blocks.** A call can take a closure as its last argument, written after
it as `do |params| ... end`: `table.filter(nums) do |n| n % 2 == 0 end` is
`table.filter(nums, fn(n) n % 2 == 0 end)`. `spawn do ... end`, `pcall do
... end` and `build_string do |add| ... end` are the same form.

**Larger programs.** The apps under `examples/` (`bookmark_app`,
`stomp_broker`, `logstat`, `mini_redis`, `lox`, `gemgrep`, `jobqueue`,
`honeypot`, `httpd`) test themselves with `std/test`; read them for how
the pieces fit together.

**Words this doc uses.** *Array*: a table with keys `0 .. n-1`. *Record*:
a table with string keys. *Process*: a lightweight thread with its own
memory and mailbox, started with `spawn`; processes share nothing and talk
with `send`/`receive`. *ref*: a unique value from `make_ref()` that tags a
request so its reply can be recognized. *Pin*: `^name` in a pattern,
which compares with the variable's value. *Back-edge*: where one loop
iteration ends and the next begins; the runtime frees garbage there.

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
- `gen_server.start` returns a `{pid}` handle that `call` takes as it is.
- A handler returns a response table; `return` inside its `do` block
  leaves the handler.
- User input goes through `pcall` and a type check before it is trusted;
  a handler that raises answers 500 and logs the error.
- `task.async` takes a closure; `task.await_all` waits with a timeout.

---

## Variables and scope

### `let` declares, `=` assigns

`let x = ...` always makes a *new* variable; to change one, assign it
(`total = total + x`). A `let` is visible to the end of its block, so a
variable set in every branch of an `if` is gone after it: give the `if`
the value (`let label = if n > 100 then "big" else "small" end`), or
declare the variables before the block and assign them inside.

### Module-level variables

A `let` outside any function is visible to every function of the file,
but holds its value only once the `let` has run: read earlier (by
top-level code above it, or a function called from there), it is `nil`
**(trap)**. Put module-level `let`s at the top. Each process has its own
copy ([State and memory](#state-and-memory)), so they are for constants
and configuration. In a function, `let total = total + x` makes a local
from the module-level `total`, which keeps its value, and the compiler
says nothing **(trap)**: to change it, assign (`total = total + x`).

### Closures

A closure can use the variables declared before it; one that calls itself
through its own `let` declares the name first (`let step = nil`, then
`step = fn(i) ... step(i - 1) ... end`). Named `fn`s can call each other
in any order.

A closure captures *variables*, not values. A `for` loop's variable is
fresh each iteration, so closures made in it see 0, 1, 2; a counter
declared before a `while` is one variable, so closures made in that loop
all see its final value. A `spawn` closure is the exception: it gets a
copy of what it captures, and later changes on either side aren't shared.

---

## Loops

### Iterate with `for`, not an index counter

`for item in items`, `for name, value in headers`, and `for i = 0,
len(rows)` (runs `0 .. len-1`) build no `keys()` array, can't forget the
increment, and take `continue`. The range form has no step; count down
with a `while`. `for k, v in tbl` visits keys in `keys(tbl)` order. The
one-variable form takes arrays and strings only; use `for k, v in t` or
`values(t)` for records. A `do` block is a closure, so `return` inside
`table.each` returns from the block and the loop goes on: use a `for` to
stop early.

### Don't add or remove entries of what you're iterating **(trap)**

`delete` during `for k, v in tbl` skips entries and then visits `nil`;
`remove_at` during `for x in arr` skips the element after each one
removed, and `push` makes the loop visit the new ones. Iterate over a
snapshot:

```gem
for k in keys(sessions)
  if expired(sessions[k])
    delete(sessions, k)
  end
end
```

### Prefer builtins to byte loops

A `while` with a cursor suits tokenizers and frame parsers; read bytes
with `ord(s, i)`, which doesn't allocate. But `str_replace`, `substr` and
`find` (and `string.index_of`, `split`, `contains`) run in C: a needle 1
MB into a string is found in well under 1 ms with `find`, against about
25 ms with an `ord` loop. For a pattern, `std/regex` tests a short line
in about 0.5 µs (patterns are compiled once and cached); in a Gem string
its backslashes are doubled (`"\\d+"`).

---

## Control flow

### `if`, `match` and `receive` give values

The value is the last expression of the branch taken; a block goes
anywhere an expression does except a parameter default and a `when` arm's
value:

```gem
let kind = match code
when 200 then "ok"
when 404 then "missing"
else "error"
end
print("{n} item{if n == 1 then "" else "s" end}")
```

A branch ending in a statement, or an `if` that takes no branch, gives
`nil`. In `return match ...`, a call at the end of an arm is a tail call.
A statement starting with `if` is the block alone: write `(if c then a
else b end).name`. Details: SPEC "Control Flow".

### `match` on shape instead of `if` chains on fields

Use `if` for boolean conditions and `match` to branch on a tag, a literal
or a table's shape (`match pcall parse(input)` with arms `when {ok: true,
value: ast}` and `when {ok: false, error: msg}`). A table pattern matches
when the listed keys match; extra keys are ignored. A bare name binds; to compare with a variable or
constant, pin it (`when ^STATUS_OK`). There are no guards or
alternatives: use an `if` chain or one arm per value.

A `match` with no `else` raises `no match for <value>` when no arm
matches: write `else nil` when that's fine, or `else error("store:
unknown message {msg}")`. `receive` has no `else`; its catch-all is `when
other` ([Keep mailboxes clean](#keep-mailboxes-clean)).

### Compare with `nil` on purpose

Only `nil` and `false` are falsy; `0`, `""` and `[]` are truthy. Write
`if x == nil` for "missing" and `if not x` for "falsy".

---

## Functions

### Optional arguments: defaults and option bags

```gem
fn call(target, msg, timeout_ms = 5000)
fn serve(router, {port = 8080, host = "0.0.0.0"} = {})
```

A key the bag's pattern doesn't name raises (`serve: unknown option
"prot" (did you mean "port"?)`); end the pattern with `...rest` to take
the other keys instead, for options passed on. std's option tables and
child specs are checked the same way. Keep `...rest` parameters for real
variadics. A default applies only when the
argument (or key) is left out: passing `nil` gives `nil` **(trap)**, so a
caller that forwards an option it may not have (`{port: opts.port}`)
passes `nil`, not the default. Leave the key out instead. The trailing
`= {}` makes the bag optional; a `nil` bag also becomes `{}`.

### Arity is checked only for direct calls

A direct call to a named fn or module export with the wrong argument
count is a compile error. A call through a value (a callback) isn't
checked: extra arguments are dropped and missing ones are `nil`.

### Don't reuse module names for local variables **(trap)**

A local or parameter named like a loaded module (`string`, `json`, ...)
hides the module in its scope, and `json.encode(x)` there fails only at
runtime (`field access on non-table`). Naming something like a builtin
(`let len = 3`) hides the builtin in that file only; do it only when the
name is the module's API (`log.error`).

### Small rules

- `pcall f(x)` or `pcall do ... end` catches errors, giving `{ok: true,
  value: v}` or `{ok: false, error: message, stack: trace}`.
  `pcall(f(x))` runs `f(x)` unprotected and calls the fn it returns
  **(trap)**: an error in `f(x)` itself is not caught. The compiler warns
  about it; it's right only when `f(x)` returns the fn to call.
- If the entry file defines `fn main`, the compiler calls it after the
  top-level code; calling it yourself runs it twice **(trap)**.

---

## Tables and arrays

### Append with `push`, remove with `remove_at`

```gem
push(items, x)                 # not items[len(items)] = x or a counter
remove_at(arr, i)              # arrays: shifts the rest left
delete(tbl, "key")             # keys; delete(arr, i) before the end leaves a hole
```

Assigning `nil` doesn't remove a key **(trap)**: after `t.x = nil`, `x` is
still in `keys(t)`, `len(t)` and `json.encode(t)`. Use `delete`.

### Test keys with `has_key`, values with `in`

`tbl[k] != nil` can't tell "missing" from "present and nil"; `has_key(tbl,
k)` can. `x in t` looks for a value in a non-empty array (keys exactly
`0 .. n-1`) and for a key in any other table: `20 in [10, 20]` and
`"name" in record` are both `true`.

An int-keyed set is an array when its keys happen to be exactly `0 .. n-1`,
and then `in` tests its values **(trap)**: with `seen[0] = true` and
`seen[1] = true`, `1 in seen` is `false` and `true in seen` is `true`.
Test an int-keyed set with `has_key(seen, id)`.

A table of more than 8 entries finds a scalar key through a hash index (a
smaller one scans), but a table used as a key is always found by a linear
scan, so a large set of tables is quadratic **(trap)**: 20,000 tables as
keys took 0.27 s. Key such a set by an id the tables carry.

### Arrays, records, identity

- A table is an array or a record; a string key added to an array makes
  it a record: `len` counts the string key too, `in` tests keys instead
  of values, and a one-variable `for` raises.
- Tables compare by identity: `{a: 1} == {a: 1}` is `false`, and so is a
  pinned table in a pattern. Compare fields or an id (`test.assert_eq`
  compares by structure).
- Reading an array past its end gives `nil`; a negative index past the
  start, or any out-of-range string index, raises. Negative ints are
  positions from the end (`arr[-1]`), never keys.
- `sort(arr, cmp)` sorts an array in place; `cmp(a, b)` returns a number
  (`fn(a, b) a.age - b.age end`), not a boolean. To order a record, sort
  `keys(t)`.

### Tables and JSON

An empty table encodes as `{}` when made with braces and `[]` otherwise,
so start a record with `{}`. Keys encode in insertion order; a table is a
JSON array only when its keys are exactly `0 .. n-1`, and other int keys
become strings (`by_id[42]` parses back as key `"42"`). `json.parse`
reads integers past 64 bits as floats, losing digits: send big ids as
strings. It raises on malformed input, so `pcall` it on outside input.

---

## Numbers

### Don't compare ints and floats with `==` **(trap)**

`2.0 == 2` is `false`, `when 2` doesn't match `2.0`, and `t[1]` and
`t[1.0]` are different keys. JSON numbers with a `.` or an exponent parse
as floats. Convert first (`to_int`, `floor`) when values may come from
either; `<` and the other orderings compare them numerically.

### Arithmetic and conversions

`7 / 2` is `3` and `-7 % 3` is `-1` (as in C); use `to_float` for real
division. Dividing by zero raises. `to_int("12\n")` raises: `string.trim`
lines read from files, and wrap conversions of user input in `pcall` and
answer 400, or a bad id becomes a 500.

---

## Strings

### Interpolate instead of concatenating

`"user {id} not found"`, not `"user " + to_string(id) + ...`. Any
expression works inside the braces; use single quotes for literal braces
(`'{"key": 1}'`).

### Building strings

In a loop, `s = s + piece` (or `s += piece`) is an in-place append, fast
**only while the loop doesn't read `s`** **(trap)**. A loop that tests
`len(s)`, a string threaded through recursion, a prepend, or a
module-level `s` appended to inside a fn copies the whole string every
time: quadratic (200 KB: 1.2 s against 2 ms). `build_string` has no such
conditions:

```gem
let out = build_string do |add|
  for row in rows
    add(row.name, ",", row.value, "\n")
  end
end
```

`add` takes any number of values and appends each as `to_string` would.
For text built across functions, use a buffer: `buf_new()`, `buf_push(buf,
s)`, `to_string(buf)` (`print(buf)` shows `<buffer:N>`, not the text).

### Strings are bytes

`len` is the byte count (`len("é")` is `2`), `s[i]` is a 1-byte string,
`ord(s, i)` the byte value, and `for ch in s` walks bytes. Slice with
`substr(s, start, count)`. There are no `\x` or `\u` escapes; use
`chr(n)` (a byte) or `utf8.char(n)` (a code point). Strings may contain
`\0`. For text by characters, `std/utf8` counts, slices and changes case
(`utf8.len("é")` is `1`).

---

## Errors

- `error` takes a string: `error({code: 404})` raises `"{code: 404}"`, so
  `r.error.code` doesn't work. When callers need structure, return a
  result table.
- Prefix messages with where they came from: `error("task.await: only the
  process that started a task can await it")`.
- Lookups that can miss return `nil`. Failures the caller should handle
  return `{ok: true, value: v}` / `{ok: false, error: msg}` (the `pcall`
  shape, so it works with `match`). Raise for broken invariants and misuse.
- Put `pcall` at boundaries: an HTTP handler that answers 400 or 500, a
  test runner, a parser fed user input. An uncaught error in a spawned
  process kills only that process and tells its monitors and links: that
  is the recovery mechanism. In a long-running loop, wrap one iteration's
  work, not the whole loop.
- Re-raising with `error(r.error)` reports the stack of the re-raise; log
  `r.stack` first if you need the original.

---

## State and memory

### Module-level variables are per process **(trap)**

A spawned process starts with a copy of its parent's module-level
variables as they were at the `spawn`; after that each process's writes
stay its own, so a counter bumped from a handler counts only that
process's calls. Shared, changing state belongs in a process: a
`gen_server` ([A first program](#a-first-program)) or a `receive` loop
([below](#a-process-loop-is-while-or-a-tail-call-trap)).

The compiler prints a `note:` for a write to module-level state in code
reachable from a `spawn do ... end` body; read it as a question. A
function spawned through a variable gets no `note:`, and neither do
`std/http` handlers, which run in a process per connection.

Set configuration before spawning (`log.set_level` before `http.start`),
and name constants in `UPPER_SNAKE`. A child copies a module-level
variable when it first reads it, and that costs time for large data (a
200,000-record array: a few hundred ms): keep large data in a process
that answers queries.

### Don't grow a module-level table while spawning **(trap)**

Each `spawn` snapshots every module-level variable changed since the
previous spawn. Top-level code that pushes pids into a module-level array
copies the array into every child: 10,000 spawns take 11 s and 2.6 GB.
Kept in a local of `fn main` (`let pids = []`), they take 0.2 s.

### A process loop is `while` or a tail call **(trap)**

Every loop frees its garbage at its back-edge: `while`, `for`, and a
function that calls itself as its last action (a *tail call*). So a
long-running process stays in constant memory as long as it is a loop.
`loop(state)` followed by any other statement is an ordinary call: every
message adds a stack frame and keeps its memory, until `stack overflow`.
The compiler warns about that in a function with a `receive`, but not in a
cycle through several functions (`loop` → `handle` → `loop`): keep each
call the last thing its function does.

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

`warning: ... cannot reset per-process arena at this loop's back-edge` on
a `while true` means its memory grows without bound; restructure the loop
as the warning says.

### A process holding a lot of data pauses every process **(trap)**

Now and then a long-running loop copies everything it keeps alive, and
processes share one thread, so nothing else runs meanwhile: about 0.04 s
at 100,000 small records, 0.25 s at 300,000. Bound what a long-lived
process keeps (expire finished records, keep counts, not histories) or
split it across processes, and leave deadlines room. `runtime_stats()`
counts slow resets (`resets_over_10ms`, ...) and
`process_info(pid).memory` shows who holds the data.

### Mutate state in place; mind the stack

Messages are copies, so a process can change its own tables freely:
`state.mode = "connected"` then `loop(state)` beats rebuilding the table.
Each process has an 8 MB stack (a few thousand to 30,000 non-tail frames);
past it a call raises `stack overflow in <fn>`, which `pcall` catches.
`json` refuses nesting deeper than 1,000; give your own recursive walkers
over untrusted input a depth cap or an explicit stack.

---

## Processes

### Use the std behaviours before writing a raw loop

| Need | Reach for |
|---|---|
| Run work concurrently, collect results | `task.async(f)` / `task.await(t, ms)` / `task.await_all(ts, ms)` |
| A server with state and request/reply | `gen_server` (`start`, `call`, `cast`, `stop`) |
| Restart processes that crash | `supervisor` (fixed children), `dynamic_supervisor` (children on demand) |
| Something else | a `receive` loop |

[`SPEC.md`](SPEC.md) documents every callback and option. A gen_server
callback returns a result table (`{state: s}`, `{reply: r, state: s}`);
anything else kills the server.

### Spawning and messages

- Inside a `spawn do ... end` body `self()` is the child **(trap)**: take
  the parent's pid before (`let parent = self()`). Pass arguments through
  the closure (`spawn do worker(5) end`).
- `send` copies the whole value, strings included: 2,000 round trips took
  13 ms with a 10 B body and 1.3 s with a 1 MB one. Send what the
  receiver needs; never fan a big message out to many processes.
- Match on `tag`; prefix tags private to a module with `_`.

### Monitor, link, trap

- `monitor(pid)` gets you `{tag: "DOWN", pid: pid, reason: reason}` when
  it dies; `spawn_monitor(f)` does both and returns `{pid: pid}`.
- `spawn_link(f)`: "we live and die together". `link` to a process that
  has already exited kills the caller with `noproc`, which `pcall` can't
  catch **(trap)**: use `spawn_link`.
- `process_flag("trap_exit", true)` turns a linked process's exit (normal
  ones too) into an `{tag: "EXIT", ...}` message. Only for processes
  whose job is handling deaths; never in a task, which `task.await` then
  can't kill on timeout.

`pcall` doesn't catch a `kill` or a link's exit. Stop a process with
`kill(pid, "shutdown")`; one that traps exits is still alive when `kill`
returns, so to know it is gone, `monitor` it first and wait for its `DOWN`
with an `after`, as the std `stop` functions do. `kill(pid, "kill")`
can't be trapped and skips cleanup: use it only after `"shutdown"` and a
deadline, as supervisors do with each child's `shutdown` budget (5000 ms
by default). A child spec's `start` runs in the supervisor, so `self()`
there is the supervisor.

### A process traps exits only once it has run **(trap)**

`process_flag("trap_exit", true)` takes effect when the new process runs
that line; a `kill` sent before then kills it untrapped, with no cleanup.
When cleanup matters, the starter waits for the child to say it traps (a
gen_server that sets the flag in `init` needs nothing more):

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

### Don't pass an option's `shutdown` through as `nil` **(trap)**

In a child spec, a missing `shutdown` means the default budget but
`shutdown: nil` means no limit, so `{id: id, start: s, shutdown:
opts.shutdown}` lets a stubborn child hold its supervisor for good when
`opts` has no `shutdown`. Set the key only `if has_key(opts,
"shutdown")`.

### Request/reply: a ref, a pin, a timeout, and a monitor

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

- Without `^ref`, a late reply to an earlier, timed-out request is taken
  for this one **(trap)**. Every blocking request in a library needs an
  `after`.
- When the target may die, monitor it and add a `DOWN` arm, so a dead
  server fails at once. A monitor taken for one request ends with it
  **(trap)**, or the caller gets the server's `DOWN` whenever it dies:
  `monitor` returns `false` when the caller already monitors the target,
  so `demonitor` only when it returned `true`, and drop a `DOWN` it
  delivered (`gen_server.call` and `std/task` show how).
- `after` restarts each time a `receive` is entered, so in a server loop
  that keeps getting messages it may never fire **(trap)**. For periodic
  work, `send_after(self(), {tag: "tick"}, ms)` and handle the tick.

### Keep mailboxes clean

Every `receive` scans the mailbox for a match, so messages nobody matches
make every later `receive` slower **(trap)**: 2,000 round trips took 11 ms
with an empty mailbox and 1.1 s with 1,000 stale messages.

- A main loop ends its `receive` with a catch-all that logs what it didn't
  expect (`when other then log.warn("counter: unexpected {other}")`), so
  a misspelled tag shows up. Messages you mean to ignore (a late reply, a
  stale `DOWN`) get their own arm.
- A request/reply wait never uses a catch-all or `receive()` **(trap)**:
  it would take messages that belong to someone else, such as a task
  result or another call's reply.

### A process that makes calls doesn't also collect a stream **(trap)**

A reply wait scans the whole mailbox, messages to be read later included.
A process that makes calls while another streams messages to it rescans
the stream on every call: 10,000 calls to a server that sent a note per
call took 14 s, against 68 ms from a process of their own. Make the calls
from a separate process (`spawn do ... gen_server.call(...) ... end`) and
let this one only collect.

### Registered names

`register("db", pid)` lets others `send("db", msg)` and `whereis("db")`;
register from the parent after `spawn`, or in a gen_server's `init`.
`send` to a dead pid drops the message, but `send("db", msg)` raises once
the process has died **(trap)**, say while a supervisor restarts it:
check `whereis` for `nil`, or `pcall` the send.

### A child's `start` and a server's `init` must not wait for their starter **(trap)**

`supervisor.start` returns once every child's `start` has run, and
`gen_server.start` once `init` has returned; meanwhile the caller handles
no messages. A `start` or `init` that sends its starter a request and
waits gets a `deadlock` error or waits for good (under a supervisor,
`init` must not call `which_children` either). Pass what a child needs in
its spec, or do the work after `start` returns: `send(self(),
"connect")` in `init`, and connect in `handle_info`. A slow `init` holds
up its starter too.

### Only the pending call learns why a server died **(trap)**

A `gen_server.call` waiting when the server dies raises `gen_server.call:
server exited: <reason>`; later calls raise `...: server exited: noproc`
(by handle or pid) or `...: no process registered as "<name>"`. To act
on the reason, monitor the server.

### `spawn` can fail: catch it where load decides **(trap)**

An idle process takes about 21 KB, but `spawn` raises when it can't start
one: at the process limit (262,144, or `GEM_MAX_PROCS`), at about 14,000
processes under Linux's default `vm.max_map_count`, or when the system
refuses memory. With a large `vm.max_map_count` and little memory, the
system kills the program first: cap the count yourself or set
`GEM_MAX_MAPS`.

An acceptor that spawns per connection catches the error (`pcall
spawn(...)`) and closes the connection, or one burst kills it; a public
server caps connections (`http.start`'s `max_connections`), and fan-out
over a list of unknown length caps the tasks in flight.

### When the program ends

The program runs while any process can still do something (a server on a
socket, a pending timer). Once main has finished and every other process
waits in a `receive` nothing can satisfy, it exits 0; if main itself is
stuck that way, the runtime reports `deadlock` and exits 1. A crashed
spawned process doesn't change the exit status: when a failure must fail
the program, monitor the process (or `task.await` it).

---

## I/O and blocking

### Know what blocks every process

Gem code is preempted at loop back-edges and tail calls; non-tail
recursion (a naive `fib(30)`) and blocking builtins hold up **all**
processes until they return.

| Yields to other processes | Blocks everything |
|---|---|
| `tcp_*`, `tls_connect`, `sleep`, `receive` | `sqlite_query`, `sqlite_exec`; a TLS handshake's crypto (about 1 ms) |
| `read_file`, `write_file`, `append_file`, `exec`, `exec_capture`, `sqlite_open`, `sqlite_close`, `extern blocking fn` (a pool of 4 threads) | `file_exists`, `is_dir`, `stat`, `list_dir`, `mkdir`, `rename`, `remove_file`; plain `extern fn`, `input`, `read_stdin`; `print` to a slow pipe |

Keep sqlite queries short and indexed: a one-second query stalls every
process for a second.

The pool has 4 workers **(trap)**: four long calls (`exec`, an `extern
blocking fn`) take all of them, and every file read waits behind them
(16 readers fell from 77,000 reads/s to 240 beside four 100 ms calls).
Run long calls from fewer processes than workers, or set `GEM_POOL_SIZE`
just above the number that run at once; more makes fast calls slower.

### Claim a resource in the process responsible for it

Sockets and sqlite handles belong to the process that opened them: closed
when it crashes or is killed, left open with no owner when it returns.
`claim(r)` moves a resource to the calling process, which closes it when
it exits for any reason. A process that serves a connection claims it
first **(trap)**; otherwise the socket stays open with the acceptor after
the session crashes:

```gem
fn session(sock)
  claim(sock)               # any exit of this process now closes sock
  let line = tcp_read(sock, 4096, 30000)
  handle(line)
end
```

Helpers that only use a socket (a reader, a watchdog) don't claim it.
`process_info(pid).resources` counts what a process owns, and
`GEM_DIAG=2` reports processes that exit leaving resources open. For a
short exchange, open and close in one function, closing on every path.
`http.stop(server)` frees the server's port; a test server takes port `0`
and reads `server.port` (`tcp_local_port(l)` for a raw listener).

### A restartable child opens what it claims **(trap)**

A supervised child that claims a resource from its spec closes it with its
first crash, and every restart then fails on the closed resource. Either
the child opens the resource itself (std/http's server opens a fresh
listener), or it uses the parent's without claiming it (a listener stays
with main, and each restarted acceptor claims only the sockets it
accepts).

### Reading and writing in bulk

Read a file or stdin a line at a time with `std/io`: `io.lines(path) do
|line| ... end` (`"-"` is stdin) reads 64 KB blocks, so memory stays flat:
a 200 MB log took 0.9 s at a 4 MB peak, against 1.2 s and the whole file
in memory for `read_file` and a `find` loop. `io.reader(path)` gives the
lines one `next()` at a time, for stopping early. `print` flushes every line: a
million lines took 0.6 s with `print` and 0.3 s as one `write_stdout` of
a `build_string` (2.7 s against 0.16 s on macOS arm64), so write bulk
output in one piece.

### Run commands with `exec_capture`

`exec_capture(cmd)` returns `{status, stdout, stderr}`; pass the
command's input as a second argument instead of a temporary file. `exec`
is for commands whose output goes to the terminal. Both run `sh -c`, so
quote what you splice into the command, and check `status`: a command
that fails doesn't raise.

### Pass timeouts to reads, check writes

- `tcp_connect(host, port, timeout_ms)` and `tls_connect(host, port,
  {timeout_ms: ms})` bound the connect; without one, a host that drops
  packets holds the caller a minute or more.
- `tcp_read(fd, n, timeout_ms)` returns `""` at end of stream and `nil` on
  timeout. Without a timeout a silent peer blocks the caller forever
  **(trap)**: library code always passes one.
- `tcp_write(fd, s, timeout_ms)` returns the bytes written and doesn't
  raise. Treat `< len(s)` as "connection lost or too slow", but expect the
  first write after a disconnect to still succeed.

### `tcp_peer` is `nil` once the peer has reset **(trap)**

A client that connects and resets at once (scanners do it all the time)
leaves no peer address, so `tcp_peer(fd).ip` raises and kills the
acceptor; later calls can return `nil` too. Read it once, right after
`tcp_accept`, check for `nil` (and close the socket), and pass the value
on.

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

- `load "./thing"` loads `thing.gem` beside the loading file and binds
  `thing.make`, `thing.use`. Prefer the namespace (`string.split`) to
  `load "std/string" (split, trim)` unless a name is used often.
- The `export` list decides what is public; no `_` prefix needed.
- Name module files in `snake_case` (or `load "./my-utils" as my_utils`).
  Break a load cycle by moving shared code into a third module.

### Document the public API with `##`

A `##` comment is a doc comment, for *callers*: the module header and the
block directly above each exported function. A plain `#` comment is for
whoever *maintains* the code: why it's shaped this way, invariants,
performance internals. To choose, ask: **would a caller write different
code if they knew this?** "Returns -1 when there is none" goes in `##`;
"searches growing chunks in C, then bisects" goes in `#`.

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
times that, as long as every line is a rule that holds across functions.
A rule about one function goes on that function. A design overview that
only maintainers need goes in a `#` block after the `load`s.

Private functions don't get `##`. Give one a `#` comment when its
contract isn't obvious from its name.

### Accept a pid or a `{pid}` handle in one place

When your API takes a process, accept both a bare pid and the `{pid:
pid}` handle std's `start` functions return, and normalize it once in a
helper.

---

## C interop

The full rules are in [`SPEC.md`](SPEC.md) ("C Interop"); these are the
ones that bite.

- Put the C code in a header of `static` functions and `extern include
  "<path>"` it (relative to the `.gem` file) before the `extern fn`s; for
  libc, `extern include` its header (`"stdio.h"`). Link other libraries
  with `gem -L <dir> -lfoo app.gem`.
- Types are checked, not converted: a `Float` parameter rejects `2`.
- A `Ptr` is an int, and `NULL` comes back as `0`, which is truthy: `if
  not p` never catches it **(trap)**. Compare with `p == 0`.
- `String` parameters stop at the first `\0`; use `Bytes` for binary
  data. A plain `extern fn` gets a pointer into the string, not a copy,
  so pass a large string with offsets rather than a `substr` slice.
- A plain `extern fn` returns static memory; an `extern blocking fn`
  returns `malloc`'d memory, which the runtime frees. Don't keep pointers
  to Gem strings or tables in C after the call returns.
- Pass a socket as a `Socket` parameter, not `tcp_fd(sock)`: the wrapper
  checks it is open and keeps its fd valid during the call.

### Plain `extern fn` for quick calls, `extern blocking fn` for waits **(trap)**

A plain `extern fn` runs on the scheduler thread: no process runs until it
returns. An `extern blocking fn` runs on the pool while others run, but
every call pays a hand-off and copies its `String` and `Bytes` arguments:
`regexec` on each of 19,000 lines took 5 ms plain and 0.64 s blocking
(`examples/gemgrep`). Use `extern blocking fn` for calls that wait (I/O,
locks, the network) or run past about a millisecond, and a plain
`extern fn` for short calls in loops (`extern fn write` beside `extern
blocking fn fsync`).

### A `Ptr` is a number, not an owner **(trap)**

A C object behind a `Ptr` isn't freed when its process dies, and `spawn`
and `send` copy the number, not the object: when one process frees it,
another holds a dangling pointer, which crashes rather than raising. Make,
use and free a C object in one process, freeing it on every path (`let r
= pcall use(f)`, then free, then re-raise), and let other processes make
their own. Zeroing a handle after the free (`h.ptr = 0`) and checking for
`0` before each use turns a use after free into a Gem error
(`examples/gemgrep/regex.gem`).

---

## Tests

- Use `std/test` (`test.case`, `test.assert`, `test.assert_eq`,
  `test.assert_neq`, `test.assert_throws`, `test.run()`). `assert_eq`
  compares tables by structure and everything else with `==` (so `1`
  isn't `1.0`), and a failure shows the path to the first difference.
  `assert_throws` returns the message; check it when it matters.
- Register cases at the top level (or from `main`), in the process that
  calls `test.run()`, which exits with status 1 when a case fails.
- A test that spawns a process `spawn_monitor`s it (or uses `task`); with
  `spawn_link`, a crashing child kills the runner.
- Cover the edges: empty input, a single element, `nil` where a table is
  expected, deep nesting, timeouts, a process dying mid-request.

---

## Style

- Two-space indent, `snake_case` for functions and variables, `UPPER_SNAKE`
  for module constants.
- Keep `if ... then ... end` and `when ... then ...` on one line only when
  they are short.
- An expression can't continue on the next line, even inside parentheses
  (only call arguments and table literals can). Split a long condition
  into named `let`s.
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
  the reader wouldn't expect (a `sqlite_query` that blocks every process, a
  process that traps exits so it can close its socket). Don't narrate
  what the next line does.
- Keep functions short. Prefer a well-named helper to a long arm inside a
  `match`.

---

## Trap index

| Trap | What happens | Do instead |
|---|---|---|
| Module-level variable read before its `let` runs | `nil` | module-level `let`s at the top |
| `let total = total + x` in a fn, `total` module-level | a local; the module-level one keeps its value (no warning) | `total = total + x` |
| `delete`/`remove_at`/`push` on what a `for` iterates | skips or adds entries, visits `nil` | iterate a snapshot (`keys(tbl)`) |
| `nil` passed for a defaulted parameter or option field | it is `nil` | leave the argument (key) out |
| A local named `json`, `string`, `table`... | module hidden; runtime error | another name |
| Calling `main()` when `fn main` exists | runs twice | let the compiler call it |
| `t.x = nil` to remove a key | key stays | `delete(t, "x")` |
| `id in seen` on an int-keyed set with keys `0 .. n-1` | tests values: it's an array | `has_key(seen, id)` |
| `pcall(f(x))` | an error in `f(x)` isn't caught (compiler warns) | `pcall f(x)` |
| Large set or index with tables as keys | quadratic | key by an id the tables carry |
| `2.0 == 2` | `false` | convert first |
| String accumulator read inside its loop | quadratic | `build_string` |
| Module-level `let` used as shared state | each process changes its own copy | keep shared state in a process |
| Module-level table pushed to between spawns | copied into every child: O(n²) | a local of `fn main` |
| `loop(state)` then more statements, across fns | stack and memory grow until overflow | self call as the last expression |
| A long-lived process holding 100,000s of records | stalls every process (0.04 s and up) | bound or shard the state |
| `self()` inside `spawn do ... end` for the parent | it's the child | `let parent = self()` before |
| `link` to a process that may have exited | caller dies with `noproc` | `spawn_link` |
| `trap_exit` in a fresh `spawn` body, killed at once | dies untrapped; no cleanup | the starter waits for "ready" |
| `shutdown: opts.shutdown` in a child spec | a missing option becomes no limit | set the key only when `has_key` |
| Reply pattern without `^ref` | takes a stale reply | `ref: ^ref` |
| Monitor for one request not removed | its `DOWN` arrives later | `demonitor` when `monitor` returned `true` |
| `after` in a busy server loop | never fires | `send_after` ticks |
| Stale messages nobody matches | every `receive` slows; typos unseen | catch-all in main loops that logs |
| `receive()` or catch-all in a reply wait | steals other replies | selective `receive ... when` |
| Calls from a process that also collects a stream | quadratic reply waits | calls from a separate process |
| `send` to a registered name whose process died | raises | `whereis` + check, or `pcall` |
| `start` or `init` waiting on its starter | deadlock, or a wait for good | pass it in the spec, or `send(self(), ...)` |
| `gen_server.call` to a dead server | `server exited: noproc` or `no process registered`, not why it died | monitor it |
| `spawn` per connection or item, unguarded | raises at the limit; the acceptor dies | catch it, or cap in-flight work |
| Long `exec` / `extern blocking fn` calls filling the pool | every file read waits | fewer at once, or `GEM_POOL_SIZE` |
| Session that doesn't `claim` its socket | stays open after a crash | `claim(sock)` first |
| Supervised child claims a resource from its spec | every restart fails on it | open its own, or don't claim |
| `tcp_read` with no timeout | blocks forever on a silent peer | pass a timeout |
| `tcp_peer(fd).ip` | `nil` once the peer has reset; raises | read once after `tcp_accept`, check |
| `if not p` on a `Ptr` | `NULL` is `0`, truthy | `p == 0` |
| Slow plain `extern fn`, or `extern blocking fn` per line | all processes stall, or 100x slower | blocking for waits, plain for quick calls |
| A `Ptr` sent, captured by `spawn`, or left at exit | shared or leaked C object | one process makes, uses and frees it |
