# Gem Best Practices

How to write Gem that reads well, runs fast, and doesn't fall into the
runtime's traps. [`SPEC.md`](SPEC.md) says what the language *does*;
[`CHEATSHEET.md`](CHEATSHEET.md) is the syntax on one page; this file says
what to *reach for*. New code in `std/`, `examples/`, `compiler/` and `lsp/`
should follow it, and old code should move toward it when touched.

Every rule here was checked by running code against the compiler, not
taken from other docs. Two markers:

- **(trap)**: fails silently, crashes, or falls off a performance cliff.
  Treat it as a hard rule.
- **(bug)**: the rule exists only because of a compiler or runtime bug.
  When the bug is fixed, delete the rule.

Contents: [Loops](#loops) · [Control flow](#control-flow) ·
[Functions](#functions) · [Tables and arrays](#tables-and-arrays) ·
[Numbers](#numbers) · [Strings](#strings) · [Errors](#errors) ·
[State and memory](#state-and-memory) · [Processes](#processes) ·
[I/O and blocking](#io-and-blocking) · [Modules](#modules) ·
[C interop](#c-interop) · [Tests](#tests) · [Style](#style) ·
[Trap index](#trap-index)

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

`for` lowers to a `while` loop, and `for k, v` walks the table without
building a `keys()` array, so it is never slower. It can't forget the
increment, and `continue` works in it. `for k, v in tbl` visits keys in
`keys(tbl)` order.

### Use `while` for scanners, but prefer builtins to byte loops

A `while` with a cursor is right when the step varies or the loop looks
ahead: tokenizers, frame parsers. Read bytes with `ord(s, i)`, which doesn't
allocate.

Before writing a byte loop, check whether a builtin does the job:
`str_replace`, `substr` and `string.index_of` run in C. Escaping 800 KB took
82 ms with a per-byte `add(chr(c))` loop and 4 ms with chained
`str_replace`. When you must loop, copy unchanged *runs* with `substr`
instead of single bytes.

### Use `for`, not `table.each`, when you need `return` or `break`

A `do` block is a closure. `return` inside it returns from the *block*, so
`table.each` carries on with the next element. `break` inside a `do` block
fails in the C compiler, with an error pointing at the wrong file. Use
`table.each`/`map`/`filter` for straight-through transforms; use `for` when
you need to stop early.

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

### `if` and `match` are statements

`let x = if ...`, `let x = match ...` and `return match ...` don't parse. A
`match` or `if` yields a value only as the last statement of a function,
closure or block (including inside the last branch of an `if`). When you
need a value, put the `match` in a small function:

```gem
# Prefer
fn status_class(code)
  match code
  when 200
    "ok"
  when 404
    "missing"
  else
    "error"
  end
end

# Or, for two cases
let label = "small"
if n > 100
  label = "big"
end
```

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

Use `if` for plain boolean conditions. Use `match` when you branch on a tag,
a literal, or the shape of a table.

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

Keep `...rest` for real variadics like `log(level, ...parts)`. Passing `nil`
to a defaulted parameter does *not* apply the default; only leaving the
argument out does.

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

The trailing `= {}` makes the bag optional and turns an explicit `nil` into
`{}`; without it, passing `nil` is an error. A field default applies when
the field is missing or `nil`. Destructured names are ordinary locals that
closures and `spawn` bodies can capture.

### Loop functions take plain parameters only **(trap) (bug)**

A self-tail-recursive function, or any function in a cycle of mutual tail
calls, is **not** tail-call optimized if it has a default, destructured or
`...rest` parameter. It grows the C stack and segfaults (in a spawned
process after about a thousand iterations, in main after tens of
thousands). The compiler gives no warning.

```gem
# Segfaults at depth
fn count(n, acc = 0)
  if n == 0 then return acc end
  count(n - 1, acc + 1)
end

# Prefer: a plain loop function, with the friendly signature on a wrapper
fn count(n)
  count_loop(n, 0)
end
fn count_loop(n, acc)
  if n == 0 then return acc end
  count_loop(n - 1, acc + 1)
end
```

### Don't capture a parameter of a tail-recursive function **(bug)**

If a closure inside a self-tail-recursive function captures one of its
parameters, the generated C doesn't compile (`invalid type argument of
unary '*'`). Until this is fixed, copy the parameter into a local first:

```gem
fn accept_loop(server_fd, router)
  let client = tcp_accept(server_fd)
  let r = router                       # needed: router is a parameter
  spawn do
    handle(client, r)
  end
  accept_loop(server_fd, router)
end
```

`while` loops and non-recursive functions don't have this problem.

### `+=` works only on variables

`t.count += 1` and `t[k] += 1` are compile errors. Write
`t.count = t.count + 1`.

### Don't reuse builtin or module names **(trap)**

A module that defines `fn error` (or `print`, `len`...) silently replaces
the builtin everywhere in that module: `error("bad")` stops raising. A
local or parameter named `string`, `table`, `json` or `time` hides the
module and fails only at runtime (`field access on non-table`). Pick
another name.

---

## Tables and arrays

### Append with `push`

```gem
push(items, x)                 # Prefer
items[len(items)] = x          # Over
items[count] = x; count += 1   # Over (and drop the separate counter)
```

### Remove from arrays with `remove_at`, never `delete` **(trap)**

`delete` is for string-keyed tables. On an array it moves the last element
into the hole: `["a", "b", "c", "d"]` after `delete(arr, 0)` is
`{3: "d", 1: "b", 2: "c"}`. After that, `arr[0]` is `nil` and `for x in arr`
visits `nil, "b", "c"` and never reaches `"d"`.

```gem
remove_at(arr, i)              # arrays: shifts the rest left
delete(tbl, "key")             # string-keyed tables only
```

### Test keys with `has_key` when `nil` is a valid value

`tbl[k] != nil` can't tell "missing" from "present and nil". `has_key`
can. `x in tbl` is `has_key` for string-keyed tables and a value scan for
arrays.

### Don't mix string keys into arrays

A table is either an array (`[...]`, integer keys from 0) or a record
(`{...}`, string keys). Mixing them makes `len`, `in`, `for` and
`json.encode` unpredictable.

### Tables compare by identity

`{a: 1} == {a: 1}` and `[] == []` are `false`. Compare fields, or compare a
primitive key such as an id. The same applies to `^pin` patterns (pin
primitives and refs, never tables) and to `test.assert_eq`.

### Empty tables have no shape

An empty array and an empty record can't be told apart, so `json.encode({})`
gives `[]`, and so does a record emptied with `delete`. When an empty object
matters on the wire, handle it explicitly.

---

## Numbers

### Don't compare ints and floats with `==` **(trap)**

`2.0 == 2` is `false`, `when 2` doesn't match `2.0`, and `t[1]` and
`t[1.0]` are different keys. JSON numbers with a decimal point parse as
floats. Convert first (`to_int`, `floor`) when values may come from either.

### Integer arithmetic follows C

`7 / 2` is `3` and `-7 % 3` is `-1`. Use `to_float` for real division.

### `to_int` and `to_float` raise on bad input

`to_int("12abc")` is an error. Wrap conversions of user input (route
params, query strings, form fields) in `pcall`, or a bad id becomes a 500.

---

## Strings

### Interpolate instead of concatenating

```gem
"user {id} not found"                         # Prefer
"user " + to_string(id) + " not found"        # Over
```

Interpolation calls `to_string` for you. Use single quotes for text with
literal braces, such as `'{"key": 1}'`. `in` doesn't parse inside `{...}`;
compute it into a variable first.

### Building strings

Inside a loop, `s = s + piece` (or `s += piece`) is compiled into an
in-place append, so it is fast, **as long as the loop doesn't read `s`
until it is done**. A loop that tests `len(s)` or compares `s` each time
round, or a string threaded through recursion as an argument, copies the
whole string every iteration and is quadratic (200 KB: 1.8 s). For anything
non-trivial, use `build_string`, which has no such conditions:

```gem
let out = build_string do |add|
  for row in rows
    add(row.name, ",", row.value, "\n")
  end
end
```

Use `buf_new`/`buf_push`/`to_string(buf)` when the buffer has to outlive a
single block, such as a read loop that collects chunks.

### Strings are bytes

`len` is the byte count, `s[i]` is a 1-byte string, and `ord(s, i)` is the
byte value. Strings may contain `\0`. Use `substr` to slice.

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

Inside a spawned process, an uncaught error kills only that process, and
its monitors and links find out. That is the recovery mechanism. Put
`pcall` where a failure must not escape, such as an HTTP handler that has
to answer 500, a test runner, or a parser fed user input. Don't wrap every
call, and never wrap a long-running loop in it (see
[Reach long-running loops through tail calls](#reach-long-running-loops-through-tail-calls-only-trap-bug)).

---

## State and memory

### Module-level `let` holds constants only **(trap) (bug)**

Top-level `let` bindings are C globals shared by every process, but memory
belongs to the process that allocated it. A spawned process must not write
to a global, or to anything reachable from one, if the write allocates:
assigning a string, pushing onto a global array (even integers, once the
array grows), or storing a string in a field of a global table. When the
process exits, the global points into freed memory and the program
segfaults later. Writing an integer into an existing slot happens to work;
don't rely on it.

Top-level closures are inconsistent too: a top-level `spawn do ... end`
block, or a top-level closure, sees the values top-level names had when it
was created, not their current values.

```gem
# Prefer: state lives in a process
fn counter_loop(n)
  receive
  when {tag: "inc"}
    counter_loop(n + 1)
  when {tag: "get", from: from, ref: ref}
    send(from, {tag: "count", ref: ref, value: n})
    counter_loop(n)
  end
end

# Over
let count = 0
fn inc() count = count + 1 end
```

Configuration that main sets once, before spawning anything, is fine. Name
constants in `UPPER_SNAKE` (`READ_SIZE`, `STATUS_TEXT`) so it's obvious they
never change.

### How memory is reclaimed

Each process allocates from its own arena, freed in one go when the process
exits. A process that loops forever relies on the compiler's **back-edge
reset**: at the end of each iteration of a `while true` loop, or at each
self-tail call, once the arena is over 1 MB the runtime copies the values
still in use into a fresh arena and frees the rest. The three rules below
keep that working.

### Reach long-running loops through tail calls only **(trap) (bug)**

The reset happens only in a loop that is reached, all the way from a
`spawn` literal or from top-level code, by tail calls. If the loop's
function is called anywhere in a non-tail position, the reset is switched
off for **every** call site, silently, and memory grows at up to gigabytes
per second:

```gem
spawn do
  serve(conn)              # Fine: tail position
end

spawn do
  serve(conn)
  tcp_close(conn)          # Not fine: serve is no longer in tail position
end
let r = pcall serve(conn)  # Not fine either, anywhere in the program
```

Put cleanup and `pcall` *inside* the loop (`std/http`'s
`handle_connection_loop` wraps each request in `pcall`), and have the loop
function close its own resources before it returns.

The compiler signals some of these cases. Treat all three messages as
errors for any loop that runs unbounded:

- `warning: cannot reset per-process arena at this loop's back-edge`
- `warning: spawn target is not a literal fn() … end`
- `warning/note: TCO function ... not reachable from any process root` /
  `will not arena-reset at its own back-edge` (the note calls it "likely
  benign"; it isn't for an unbounded loop)

### Keep a looping process's live data under 1 MB **(trap) (bug)**

Once the data a loop keeps alive across iterations passes 1 MB, the reset
copies all of it on *every* iteration. A `gen_server` whose state holds
5,000 small records takes 6 s for 1,000 calls instead of milliseconds. Keep
large data in sqlite (`":memory:"` works) or split it across processes.

Top-level code counts as a loop too, and every top-level variable is kept
alive by it. A top-level `for` loop that builds 5,000 rows takes 17 s; the
same loop inside a function takes 3 ms. Put scripts in a function:

```gem
fn main()
  ...
end

main()
```

### Mutate state in place when it's safe

Gem tables are mutable. In a process loop, `state.mode = "connected"`
followed by `loop(state)` is fine and cheaper than rebuilding the table.

### Recursion depth is small in spawned processes **(trap) (bug)**

A spawned process has a 256 KB stack: about 900 simple non-tail frames,
or about 60 levels of nesting in `json.parse`. Overflowing it is a segfault
that kills the **whole program**; `pcall` can't catch it. A web handler
that parses JSON from the request body can be killed by a 130-byte request.
Write recursive walkers over untrusted data (JSON, user trees) with an
explicit stack, or cap the depth well below these limits.

---

## Processes

### Use the std behaviours before writing a raw loop

| Need | Reach for |
|---|---|
| Run work concurrently, collect results | `task.async` / `task.await` / `task.await_all` |
| A server with state and request/reply | `gen_server` |
| Restart processes that crash | `supervisor` (fixed children), `dynamic_supervisor` (children started on demand) |
| Something else | a `receive` loop |

### Spawn a literal function

```gem
spawn do                       # Prefer (or spawn(fn() ... end))
  serve(conn)
end

let body = fn() serve(conn) end
spawn(body)                    # Over: the compiler warns, and the arena never resets
```

### Closures capture variables, not values

A closure sees later changes to the variables it captures. Inside a loop,
`for` variables and `let`s declared in the loop body are fresh each
iteration, so each closure keeps its own. A variable declared *before* a
`while` loop is shared: every closure sees its final value.

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

### Messages are tables with a `tag`

```gem
send(pid, {tag: "deliver", frame: f})
```

Match on `tag` in `receive`. Prefix tags that are private to a module with
`_` (`"_task_result"`, `"_gs_timeout"`) so they can't collide with user
messages.

### Request/reply: a ref, a pin, a timeout, and a monitor

```gem
let ref = make_ref()
send(server, {tag: "get", from: self(), ref: ref, key: k})
receive
when {tag: "reply", ref: ^ref, value: v}
  v
after 5000
  error("store.get: timeout")
end
```

- Without `^ref`, a late reply from an earlier, timed-out request can be
  taken for this one.
- Every blocking request in a library needs an `after`.
- When the target may die, monitor it and add a `DOWN` arm, so a dead
  server fails at once instead of after the full timeout. `std/task` is the
  model: it monitors, matches `{tag: "DOWN", pid: ^pid}`, and flushes the
  `DOWN` when it's done. There is no `demonitor`, so monitor short-lived
  targets, not long-lived servers called thousands of times.
- After a timeout, a reply may still arrive and sit in the mailbox. Drop
  late replies the way `task` does (`receive when {ref: ^ref} ... after 0`).

### Keep mailboxes clean

Every selective `receive` scans the whole mailbox, so messages nobody
matches make every later `receive` slower: 2,000 round trips take 8 ms
with an empty mailbox and 1 s with 1,000 stale messages in it.

- A process's main loop ends its `receive` with a catch-all arm
  (`when other`) that drops or logs unknown messages.
- A *request/reply wait* never uses a catch-all, and never `receive()`: it
  would take messages that belong to someone else, such as a task result or
  another call's reply.

### Monitor, link, trap

- `monitor`: "tell me when it dies" (clients, callers, observers).
- `spawn_link`: "we live and die together" (a reader/writer pair).
- `trap_exit`: only in processes whose job is to handle deaths
  (supervisors).

### Messages are deep copies

`send` and `spawn` copy their values into the receiving process. Changing a
received table doesn't affect the sender. The copy costs time proportional
to the whole message, strings included: 2,000 sends cost 6 ms with a 10 B
body and 1 s with a 1 MB body. A closure passed to `spawn` copies
everything it captures. Send what the receiver needs, not a large shared
structure, and never fan a big message out to many processes.

### There are at most 1024 processes

`spawn` raises `spawn: process table full` past the limit (`GEM_MAX_PROCS`).
A reader/writer pair per connection uses two. An acceptor that spawns per
connection should catch that error and close the connection, or cap the
number of connections; otherwise one burst kills the acceptor.

---

## I/O and blocking

### Know what blocks every process

The scheduler is cooperative: a builtin that blocks the OS thread stops
**all** processes.

| Yields to other processes | Blocks everything |
|---|---|
| `tcp_*`, `sleep`, `receive` | `sqlite_query`, `sqlite_exec` |
| `read_file`, `write_file`, `append_file`, `exec`, `sqlite_open`, `sqlite_close`, `extern blocking fn` (4-thread pool) | plain `extern fn`, `input`, `read_stdin` |

Keep sqlite queries short and indexed. Use `extern blocking fn` for any C
call that can take more than about a millisecond. The pool has 4 workers,
so four long `exec` calls delay every file read behind them.

### The process that opens a handle closes it, on every path

Sockets and sqlite handles are not closed when the process that owns them
dies. A crash between open and close leaks the descriptor. Close in the
same function that opened it, with the risky part in `pcall`:

```gem
let fd = tcp_connect(host, port)
let r = pcall talk(fd)
tcp_close(fd)
if not r.ok
  error(r.error)
end
r.value
```

### Pass timeouts to reads, check writes

- `tcp_read(fd, n, timeout_ms)` returns `""` at end of stream and `nil` on
  timeout. Library code should always pass a timeout; without one, a silent
  peer blocks the caller forever.
- `tcp_write` returns the number of bytes written and does not raise when
  the peer has gone. Treat `tcp_write(fd, s) < len(s)` as "connection lost".

---

## Modules

### Layout

```gem
# std/thing — one line on what it is for.
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

- The `export` list decides what is public. Private functions don't need a
  `_` prefix.
- `load "./sibling"` inside a project, `load "std/x"` for the standard
  library. Prefer the module namespace (`string.split`) over a selective
  import, unless a name is used often enough to be noise.
- Avoid modules that load each other in a cycle: one of them may see the
  other's exports missing. Move shared code into a third module.

### Accept a pid or a `{pid}` handle in one place

`gen_server.start`, `supervisor.start` and `dynamic_supervisor.start`
return `{pid: pid}`. When an API takes either form, normalize it once in a
helper; don't repeat `if type(t) == "table"` in every function.

### Where std doesn't follow this yet

Some std APIs predate these rules: `http.start` returns a bare pid;
`gen_server.call` and `supervisor.which_children` reject the `{pid}` handle
their own `start` returns; several private message tags (`"gs_reply"`,
`"start_child"`) have no `_` prefix; `supervisor.which_children` and the
`dynamic_supervisor` calls wait with no `after`. These are being fixed.

---

## C interop

- `String` parameters arrive as `const char *` and stop at the first `\0`.
  Use `Bytes` for binary data.
- A plain `extern fn` runs on the scheduler thread and blocks every
  process. Declare slow calls `extern blocking fn`.
- Returned strings: a plain `extern fn` must return static memory (the
  runtime copies it and does not free it); an `extern blocking fn` must
  return `malloc`'d memory (the runtime frees it).
- Don't keep pointers to Gem strings or tables on the C side after the call
  returns; the next arena reset moves them.

---

## Tests

- Every behavior change gets a numbered example under `examples/`, with its
  stdout appended to `expected_output.txt`. `make test` runs them all.
- For library code, use `std/test` (`test.case`, `test.assert_eq`,
  `test.run()`) for checks that verify themselves. `assert_eq` compares
  tables by identity, so compare primitives, or compare `json.encode` of
  both sides.
- Cover the edges as well as the happy path: empty input, a single element,
  `nil` where a table is expected, deep nesting, timeouts, and a process
  dying mid-request.

---

## Style

- Two-space indent, `snake_case` for functions and variables, `UPPER_SNAKE`
  for module constants.
- Keep `if ... then ... end` on one line only when it is short.
- Comments say *why*, not what. Write a header comment for every module and
  a one-line comment for any function whose contract isn't obvious from its
  name.
- Keep functions short. Prefer a well-named helper to a long arm inside a
  `match`.

---

## Trap index

| Trap | What happens | Do instead |
|---|---|---|
| Spawned process allocates into a global | segfault later | keep state in a process |
| Loop fn also called in non-tail position anywhere | arena reset off everywhere; GB of memory | reach loops by tail calls; `pcall` inside |
| Live loop data over 1 MB, or a top-level loop | every iteration copies everything (17 s vs 3 ms) | keep state small; put scripts in `main()` |
| Deep recursion in a spawned process | whole program segfaults (~900 frames, ~60 JSON levels) | explicit stack or depth cap |
| Default / destructured / `...rest` param on a tail-recursive fn | no TCO; segfault | plain params on loop fns |
| Closure captures a param of a tail-recursive fn | C compile error | copy to a local first |
| `delete(arr, i)` | hole in the array; `for` misses the last element | `remove_at(arr, i)` |
| String accumulator read inside its loop | quadratic | `build_string` |
| `match` with no arm matching | yields `nil` silently | add an `else` |
| `error(non_string)` | message becomes `"error"` | string message or result table |
| `spawn(variable)` | arena never resets | `spawn do ... end` |
| `receive()` or catch-all in a reply wait | steals other replies | selective `receive ... when` |
| Reply pattern without `^ref` | takes a stale reply | `ref: ^ref` |
| `2.0 == 2` | `false` | convert first |
| Function named `error`, `print`... | builtin silently replaced | another name |
| Blocking call (`sqlite_query`, plain `extern fn`) | all processes stall | keep short; `extern blocking fn` |
| Handle opened, process crashes | fd leak | close on every path |
| `spawn` past 1024 processes | raises; unguarded acceptor dies | catch it or cap connections |
| `t.f += 1` | compile error | `t.f = t.f + 1` |
