# Gem Best Practices

How to write Gem that reads well, runs fast, and doesn't fall into the
runtime's traps. [`SPEC.md`](SPEC.md) says what the language *does*;
[`CHEATSHEET.md`](CHEATSHEET.md) is the syntax on one page; this file says
what to *reach for*. New code in `std/`, `examples/`, `compiler/` and `lsp/`
should follow it, and old code should move toward it when touched.

Every rule here was checked against the compiler. Rules marked **(trap)**
describe behavior that fails silently or crashes, so treat them as hard
rules, not style.

Contents: [Loops](#loops) · [Control flow](#control-flow) ·
[Functions](#functions) · [Tables and arrays](#tables-and-arrays) ·
[Strings](#strings) · [Errors](#errors) · [State](#state) ·
[Processes](#processes) · [Modules](#modules) · [Tests](#tests) ·
[Style](#style) · [Trap index](#trap-index)

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
for i = 0, len(rows)          # when you need the index
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

`for` lowers to the same `while` loop, so it costs nothing. It also can't
forget the increment, and `continue` works correctly in it (the increment
runs before the body). `for k, v in tbl` visits keys in `keys(tbl)` order.

### Use `while` for scanners

A `while` with a cursor is the right tool when the step varies or the loop
looks ahead: tokenizers, percent-decoders, frame parsers. Read bytes with
`ord(s, i)`, which doesn't allocate:

```gem
let i = 0
while i < len(s)
  let c = ord(s, i)
  if c == 37 and i + 2 < len(s)     # '%'
    add(chr(hex_byte(s, i + 1)))
    i += 3
  else
    add(chr(c))
    i += 1
  end
end
```

### Use `for`, not `table.each`, when you need `return` or `break`

A `do` block is a closure. `return` inside it returns from the *block*, and
`break` can't leave the caller's loop. Use `table.each`/`map`/`filter` for
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

### Write server loops plainly

`while true` and self-tail-recursion are both fine for a process that runs
forever. The compiler resets the process arena at the back-edge so memory
stays flat. You don't need to restructure the loop for it. **If the
compiler prints a `cannot reset per-process arena` warning, fix it**; that
warning means memory grows without bound.

---

## Control flow

### `if` and `match` are statements

`let x = if ...` and `let x = match ...` don't parse. A `match` or `if`
yields a value only as the last statement of a function. When you need a
value, put the `match` in a small function:

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

Only `nil` and `false` are falsy. `0` and `""` are truthy. Write
`if x == nil` when you mean "missing" and `if not x` when you mean "falsy",
and don't treat them as the same thing.

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

Keep `...rest` for real variadics like `log(level, ...parts)`. Note that
passing `nil` to a defaulted parameter does *not* apply the default; only
leaving the argument out does.

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
`{}`. A field default applies when the field is missing or `nil`.
Destructured names are ordinary locals: closures and `spawn` bodies can
capture them directly, so don't copy them into other locals first.

### No defaults on self-recursive loop functions **(trap)**

Functions with default or destructured parameters are **not** tail-call
optimized. A tail-recursive loop with a default parameter grows the C
stack and crashes with a segfault after enough iterations. The compiler
gives no warning.

```gem
# Crashes at depth (no TCO)
fn count(n, acc = 0)
  if n == 0 then return acc end
  count(n - 1, acc + 1)
end

# Prefer: required params on the loop, defaults on a wrapper
fn count(n)
  count_loop(n, 0)
end
fn count_loop(n, acc)
  if n == 0 then return acc end
  count_loop(n - 1, acc + 1)
end
```

The same goes for `...rest` parameters.

### `+=` works only on variables

`t.count += 1` is a compile error. Write `t.count = t.count + 1`.

---

## Tables and arrays

### Append with `push`

```gem
push(items, x)                 # Prefer
items[len(items)] = x          # Over
items[count] = x; count += 1   # Over (and drop the separate counter)
```

### Remove from arrays with `remove_at`, never `delete` **(trap)**

`delete` is for string-keyed tables. On an array it removes the key and
leaves a gap: `[a, b, c, d]` after `delete(arr, 0)` is `{1: b, 2: c, 3: d}`.
After that, `arr[0]` is `nil` and a `for x in arr` loop visits `nil` and
skips `d`.

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
`json.encode` hard to predict.

### Tables compare by identity

`{a: 1} == {a: 1}` is `false`. Compare fields, or compare a primitive key
such as an id. The same applies to `^pin` patterns: pin primitives and refs,
never tables.

### Empty tables have no shape

`{}` and `[]` are the same value. `json.encode({})` gives `[]`. When an
empty object matters on the wire, handle it explicitly.

---

## Strings

### Interpolate instead of concatenating

```gem
"user {id} not found"                         # Prefer
"user " + to_string(id) + " not found"        # Over
```

Interpolation calls `to_string` for you. Use single quotes for text with
literal braces, such as `'{"key": 1}'`.

### Build in loops with `build_string` **(trap)**

`s = s + piece` copies the whole string every time, so a loop of it is
quadratic. Building 2 MB this way takes **~118 s**; with `build_string` it
takes **13 ms**.

```gem
let out = build_string do |add|
  for row in rows
    add(row.name, ",", row.value, "\n")
  end
end
```

Use `buf_new`/`buf_push`/`to_string(buf)` when the buffer has to outlive
a single block, such as a read loop that collects chunks.

### Strings are bytes

`len` is the byte count, `s[i]` is a 1-byte string, and `ord(s, i)` is the
byte value. Strings may contain `\0`. Use `substr` to slice. Use `Bytes`,
not `String`, for `extern` parameters that can contain NULs.

---

## Errors

### `error` takes a string **(trap)**

`error({code: 404})` loses the table. `pcall` reports the message as the
string `"error"`. When callers need structure, return a result table
instead of raising.

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
call.

---

## State

### Module-level `let` holds constants only **(trap)**

Top-level `let` bindings are C globals shared by every process. A process
can only safely *read* them:

- If a spawned process puts a table into a global, through `push`, a field
  write or assignment, the table's memory belongs to that process. When the
  process exits, the global points at freed memory, and the next access
  crashes (a segfault in tests).
- A top-level `spawn do ... end` block reads its own snapshot of top-level
  names, taken when the process starts. It doesn't see later updates, and
  its own writes don't reach the global.

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

Global *configuration* that main sets once before spawning anything (a log
level, for example) is fine.

Name constants in `UPPER_SNAKE` (`READ_SIZE`, `STATUS_TEXT`) so it's
obvious they never change.

### Mutate state in place when it's safe

Gem tables are mutable. In a process loop, `state.mode = "connected"`
followed by `loop(state)` is fine and cheaper than rebuilding the table.
Rebuild only when another holder of the table must not see the change.

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

The compiler can only find a `while true` loop to reset in a literal
`fn ... end` or `do` block passed straight to `spawn`. With a variable, the
spawned process's memory grows forever.

Don't copy variables before capturing them. `let fd = client_fd` before
`spawn(fn() handle(fd) end)` is a leftover; capture `client_fd` directly.
Closures made in a loop capture that iteration's values.

### Messages are tables with a `tag`

```gem
send(pid, {tag: "deliver", frame: f})
```

Match on `tag` in `receive`. Prefix tags that are private to a module with
`_` (`"_task_result"`, `"_gs_timeout"`) so they can't collide with user
messages.

### Request/reply: a ref, a pin and a timeout

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

Without `^ref`, an old reply from an earlier, timed-out request can be
mistaken for this one. Every blocking request in a library should have
an `after`.

### Use selective `receive`, not `receive()`, in processes that use std behaviours

`receive()` takes whatever message is first, including a task result or a
`gen_server` reply that someone else is waiting for. Use
`receive ... when` with patterns, and let a catch-all arm exist only where
the process owns every message it can get (such as `handle_info`).

### Monitor, link, trap

- `monitor`: "tell me when it dies" (clients, callers, observers).
- `spawn_link`: "we live and die together" (a reader/writer pair).
- `trap_exit`: only in processes whose job is to handle deaths
  (supervisors).

### Messages are deep copies

`send` and `spawn` copy their values into the receiving process. Changing
a received table doesn't affect the sender. The copy costs about the
same for every table, however large its strings are, so send flat, small
tables. Don't send a large shared structure to many processes.

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
- A module must not load itself in a cycle. Move shared code into a third
  module.

### Accept a pid or a `{pid}` handle in one place

`start` functions return `{pid: pid}`. When an API takes either form,
normalize it once in a helper; don't repeat `if type(t) == "table"` in
every function.

---

## Tests

- Every behavior change gets a numbered example under `examples/`, with
  its stdout appended to `expected_output.txt`. `make test` runs them all.
- For library code, use `std/test` for small checks that verify themselves:
  `test.case`, `test.assert_eq`, `test.run()`.
- Cover the edges as well as the happy path: empty input, a single element,
  `nil` where a table is expected, timeouts, and a process dying mid-request.

---

## Style

- Two-space indent, `snake_case` for functions and variables,
  `UPPER_SNAKE` for module constants.
- Keep `if ... then ... end` on one line only when it is short.
- Comments say *why*, not what. Write a header comment for every module
  and a one-line comment for any function whose contract isn't obvious
  from its name.
- Keep functions short. Prefer a well-named helper to a long arm inside a
  `match`; mutual tail calls between helpers and a loop are optimized.

---

## Trap index

| Trap | What happens | Do instead |
|---|---|---|
| Spawned process changes a global table | segfault later in another process | keep state in a process |
| Default or `...rest` param on a tail-recursive loop | no TCO; stack overflow, segfault | required params on the loop |
| `delete(arr, i)` | gap in the array; `for` visits `nil` and drops the last element | `remove_at(arr, i)` |
| `s = s + x` in a loop | quadratic time (2 MB: ~118 s) | `build_string` / `buf_push` |
| `match` with no arm matching | yields `nil` silently | add an `else` |
| `error(table)` | message becomes `"error"` | string message or result table |
| `spawn(variable)` | arena never resets; memory grows | `spawn do ... end` |
| `receive()` alongside task or gen_server calls | steals their replies | selective `receive ... when` |
| Reply pattern without `^ref` | takes a stale reply | `ref: ^ref` |
| `t.f += 1` | compile error | `t.f = t.f + 1` |
