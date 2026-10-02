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

### Don't `delete` from a table while iterating it **(trap)**

`for k, v in tbl` reads the length once, and `delete` moves the last entry
into the hole, so deleting during the loop skips entries and visits `nil`.
Iterate over a snapshot instead:

```gem
for k in keys(sessions)
  if expired(sessions[k])
    delete(sessions, k)
  end
end
```

### Use `while` for scanners, but prefer builtins to byte loops

A `while` with a cursor is right when the step varies or the loop looks
ahead: tokenizers, frame parsers. Read bytes with `ord(s, i)`, which doesn't
allocate.

Before writing a byte loop, check whether a builtin does the job:
`str_replace` and `substr` run in C. Escaping 800 KB took about 100 ms with
a per-byte loop and about 10 ms with chained `str_replace`. `string.split`
and `string.index_of` are Gem byte loops themselves, so they cost about the
same as writing the loop by hand.

### Use `for`, not `table.each`, when you need `return` or `break`

A `do` block is a closure. `return` inside it returns from the *block*, so
`table.each` carries on with the next element. `break` or `continue` inside
a `do` block is reported by the C compiler, not by Gem, at an approximate
line. Use
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

### Don't redeclare a parameter with `let` **(bug)**

`let n = n - 1` where `n` is a parameter fails: in most functions the C
compiler reports `redefinition of 'gem_v_n'`, and in a tail-recursive
function the right-hand `n` reads `nil` at runtime. Assign instead
(`n = n - 1`), or pick a new name.

### `+=` works only on variables

`t.count += 1` and `t[k] += 1` are compile errors. Write
`t.count = t.count + 1`.

### Don't reuse builtin or module names **(trap)**

Defining `fn error` (or `print`, `len`...) does something different
depending on the file, and neither is reported. In a loaded module it
replaces the builtin everywhere in that module, so `error("bad")` stops
raising. In the program's entry file your definition is ignored. A local
or parameter named `string`, `table`, `json` or `time` hides the module
and fails only at runtime (`field access on non-table`). Pick another name.

### `fn main` runs automatically

If the entry file defines `fn main`, the compiler calls it after the
top-level code. Don't also call it yourself, or it runs twice.

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

`json.encode` writes keys in insertion order, so two equal records built in
different orders encode differently.

---

## Numbers

### Don't compare ints and floats with `==` **(trap)**

`2.0 == 2` is `false`, `when 2` doesn't match `2.0`, and `t[1]` and
`t[1.0]` are different keys. JSON numbers with a decimal point or an
exponent (`1e2`) parse as floats. Convert first (`to_int`, `floor`) when values may come from either.

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
whole string every iteration and is quadratic (200 KB: about 1 s, against
2 ms for the plain loop). For anything non-trivial, use `build_string`,
which has no such conditions:

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
call. In a long-running loop, wrap one iteration's work inside the loop
rather than the whole loop, so one bad request doesn't end the loop
(`std/http`'s `handle_connection_loop` does this).

Re-raising a caught error with `error(r.error)` reports the stack of the
re-raise, not the original. Log `r.stack` first if you need it.

---

## State and memory

### Module-level `let`: each process has its own copy **(trap)**

Top-level `let` bindings (in the entry file and in loaded modules) are
per-process, as in Erlang. A spawned process starts with a deep copy of its
parent's module state, taken at the `spawn`. From then on its writes stay in
that process, and other processes never see them. Inside one process,
functions and closures all read the live value.

So a module-level `let` can't hold state that processes share. A counter
bumped from a handler process counts only that process's calls. Shared,
changing state belongs in a process (`gen_server`, or a `receive` loop):

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

# Over: each process increments its own copy
let count = 0
fn inc() count = count + 1 end
```

The compiler prints a `note:` for each write to module state in code
reachable from a `spawn`. Read it as a question: is this state meant to be
private to the process?

Configuration that main sets before spawning is copied into every process
spawned after it, so set it first. Name constants in `UPPER_SNAKE`
(`READ_SIZE`, `STATUS_TEXT`) so it's obvious they never change. Large module
state makes every `spawn` slower, since each one copies it all.

### How memory is reclaimed

Each process allocates from its own arena, freed in one go when the process
exits. Every loop (`while`, `for`) and tail-recursive function also resets
at its back-edge: it copies what is still reachable from the memory
allocated since the loop started, and frees the rest. This works wherever
the loop is, in a non-tail call, inside `pcall`, or in top-level code, so a
server loop runs in constant memory with no special structure. The next
reset waits until about twice the copied size has been allocated, so loops
that keep large data alive don't copy it every iteration.

One case is not handled: if the compiler prints `warning: cannot reset
per-process arena at this loop's back-edge`, that loop's memory grows
without bound. It is printed only for `while true`, and
says why; restructure the loop, or file an issue.

### Mutate state in place when it's safe

Gem tables are mutable. In a process loop, `state.mode = "connected"`
followed by `loop(state)` is fine and cheaper than rebuilding the table.

### Deep recursion raises an error

Every process, main included, has an 8 MB stack: about 28,000 simple
non-tail frames. Past that, the call raises `stack overflow in <fn>`, which
`pcall` catches like any error. `json.parse` refuses nesting deeper than
128; `json.encode` has no cap and raises `stack overflow` at a few thousand
levels. For recursive walkers over untrusted input, cap the depth or use an
explicit stack, so a hostile input gets a clear error instead of a stack
overflow.

C code that recurses without limit (in an `extern fn`) is different: it
kills the process, and `pcall` can't catch it.

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
spawn(body)                    # Over
```

The compiler follows a literal body to find writes to module state (see
[Module-level `let`](#module-level-let-each-process-has-its-own-copy-trap)); a
function passed in a variable gets no `note:`.

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
- After a timeout, the reply may still arrive later. The pin makes it
  harmless to *this* wait, but it sits in the mailbox until something
  removes it; the process's main-loop catch-all (below) is what drops it.

### Keep mailboxes clean

Every selective `receive` scans the whole mailbox, so messages nobody
matches make every later `receive` slower: 2,000 round trips take 10 ms
with an empty mailbox and 0.8 s with 1,000 stale messages in it.

- A process's main loop ends its `receive` with a catch-all arm
  (`when other`) that drops or logs unknown messages.
- A *request/reply wait* never uses a catch-all, and never `receive()`: it
  would take messages that belong to someone else, such as a task result or
  another call's reply.

### Monitor, link, trap

- `monitor`: "tell me when it dies" (clients, callers, observers).
- `spawn_link`: "we live and die together" (a reader/writer pair).
- `process_flag("trap_exit", true)`: only in processes whose job is to
  handle deaths (supervisors).

### Messages are deep copies

`send` and `spawn` copy their values into the receiving process. Changing a
received table doesn't affect the sender. The copy costs time proportional
to the whole message, strings included: 2,000 round trips take 8 ms with a
10 B body and 0.9 s with a 1 MB body. A closure passed to `spawn` copies
everything it captures. Send what the receiver needs, not a large shared
structure, and never fan a big message out to many processes.

### There are at most 1024 processes

`spawn` raises `spawn: process table full` past the limit (`GEM_MAX_PROCS`);
main uses one slot, so 1,023 spawned processes can be alive at once. A
reader/writer pair per connection uses two. An acceptor that spawns per
connection should catch that error and close the connection, or cap the
number of connections; otherwise one burst kills the acceptor.

---

## I/O and blocking

### Know what blocks every process

The scheduler is cooperative: a builtin that blocks the OS thread stops
**all** processes.

| Yields to other processes | Blocks everything |
|---|---|
| `tcp_*` (except name lookup, see right), `sleep`, `receive` | `tcp_connect` to a host *name* (DNS lookup runs inline) |
| `read_file`, `write_file`, `append_file`, `exec`, `sqlite_open`, `sqlite_close`, `extern blocking fn` (4-thread pool) | `sqlite_query`, `sqlite_exec` |
| | `file_exists`, `is_dir`, `list_dir`, `mkdir`, `remove_file` |
| | plain `extern fn`, `input`, `read_stdin` |

Keep sqlite queries short and indexed. Use `extern blocking fn` for any C
call that can take more than about a millisecond. The pool has 4 workers,
so four long `exec` calls delay every file read behind them.

### The process that opens a handle closes it, on every path

Sockets and sqlite handles are not closed when the process that owns them
dies. A crash between open and close leaks the descriptor. For a short
exchange, close in the same function that opened it, with the risky part in
`pcall`:

```gem
let fd = tcp_connect(host, port)
let r = pcall request_once(fd)
tcp_close(fd)
if not r.ok
  error(r.error)
end
r.value
```

The same shape works around a long-running connection loop. To keep the
connection open after a bad request, `pcall` each iteration inside the loop
instead, as `std/http` does.

### Pass timeouts to reads, check writes

- `tcp_read(fd, n, timeout_ms)` returns `""` at end of stream and `nil` on
  timeout. Library code should always pass a timeout; without one, a silent
  peer blocks the caller forever. A timeout of `0` means *no* timeout.
- `tcp_write` returns the number of bytes written and does not raise when
  the peer has gone. Treat `tcp_write(fd, s) < len(s)` as "connection lost",
  but expect the first write after a disconnect to still report success;
  only later writes show it. `tcp_write` has no timeout, so a peer that
  stops reading blocks the writer.

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
- Avoid modules that load each other in a cycle. Functions calling across
  the cycle work, but a top-level `let` that uses the other module while it
  loads finds its namespace `nil`, and the error points at the entry file.
  Move shared code into a third module.

### Accept a pid or a `{pid}` handle in one place

`gen_server.start`, `supervisor.start` and `dynamic_supervisor.start`
return `{pid: pid}`. When an API takes either form, normalize it once in a
helper; don't repeat `if type(t) == "table"` in every function.

### Where std doesn't follow this yet

Some std APIs predate these rules: `http.start` returns a bare pid;
`gen_server.call` and `supervisor.which_children` reject the `{pid}` handle
their own `start` returns; several private message tags (`"gs_reply"`,
`"start_child"`) have no `_` prefix; `supervisor.which_children` and the
`dynamic_supervisor` calls wait with no `after`; `http`'s acceptor doesn't
catch a full process table; `std/test` keeps its cases in module globals
and uses index-append and `_` prefixes. These are being fixed.

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
  tables by identity, so compare primitives or individual fields.
  (Comparing `json.encode` output works only when both sides were built in
  the same key order.)
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
| Module-level `let` used as shared state | each process changes only its own copy | keep shared state in a process |
| `warning: cannot reset ... back-edge` on a `while true` | memory grows without bound | restructure the loop |
| `let p = p ...` on a parameter `p` | C compile error, or `nil` in a tail-recursive fn | assign, or use a new name |
| `delete(arr, i)` | hole in the array; `for` misses the last element | `remove_at(arr, i)` |
| `delete` inside `for k, v in tbl` | skips entries, visits `nil` | iterate `keys(tbl)` |
| String accumulator read inside its loop | quadratic | `build_string` |
| `match` with no arm matching | yields `nil` silently | add an `else` |
| `error(non_string)` | message becomes `"error"` | string message or result table |
| `receive()` or catch-all in a reply wait | steals other replies | selective `receive ... when` |
| Reply pattern without `^ref` | takes a stale reply | `ref: ^ref` |
| `2.0 == 2` | `false` | convert first |
| Function named `error`, `print`... | replaces the builtin in a module, ignored in the entry file | another name |
| Calling `main()` when `fn main` exists | runs twice | let the compiler call it |
| Blocking call (`sqlite_query`, DNS, plain `extern fn`) | all processes stall | keep short; `extern blocking fn` |
| Handle opened, process crashes | fd leak | close on every path |
| `tcp_read` with no timeout, or `0` | blocks forever on a silent peer | pass a timeout |
| `spawn` past 1,023 live processes | raises; unguarded acceptor dies | catch it or cap connections |
| `t.f += 1` | compile error | `t.f = t.f + 1` |
