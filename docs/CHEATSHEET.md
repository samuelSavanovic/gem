# Gem Language Cheatsheet

A one-page summary of Gem syntax, builtins, and standard library modules. For full semantics see [`SPEC.md`](SPEC.md); for idioms and traps see [`BEST_PRACTICES.md`](BEST_PRACTICES.md).

```gem
# Variables
let x = 10
let {a, b} = tbl                     # table destructuring
let [first, second] = arr            # array destructuring
let {port = 8080, host = "0.0.0.0"} = opts   # field defaults (fire on a missing key, not nil)
let x = x + 1                        # in a fn/block: new variable shadowing the old x (to block end)
                                     # (warned in a loop when nothing there assigns x and the loop's condition
                                     #  or the code after the loop reads it)
                                     # a let is visible only to the end of its block (if/loop body/arm):
let s = if c then 1 else 2 end       #   give the if a value to use it after the block

# Functions — fn/end, last expression is implicit return; named fns only at top level
fn add(a, b)
  a + b
end
fn greet(name, greeting = "Hello")   # default params; a direct call with the wrong count is a compile error
  print("{greeting}, {name}!")
end
fn log(level, ...msgs)               # variadic (rest param)
end
fn server({port = 8080, host = "0.0.0.0"} = {})   # destructured params
end                                                # `= {}` makes the bag optional / nil-tolerant

# Closures / anonymous functions
let f = fn(x) x * 2 end              # inside a fn or block: let helper = fn() ... end

# Blocks — trailing do/end or { } passed as the last arg
table.each(items) do |item|
  print(item)
end
table.map(items) { |x| x * 2 }
spawn do ... end                     # block is the only arg: () optional
f { |x| x + 1 }                      # brace block without (): needs |params| (|| for none)
# if/while/for headers take no do and no trailing blocks — `while x do` is an error

# Control flow — end-terminated, elif (not else if)
if cond then expr else expr end      # single-line
if cond
  body
elif cond2
  body
else
  body
end

while cond
  body
end

for item in arr ... end              # array iteration (raises on a non-array table)
for k, v in tbl ... end              # key-value iteration
for i, ch in str ... end             # bytes: 0-based index, 1-byte string
for i = 0, n ... end                 # range [0, n)

match val
when "a"
  handle_a()
when {ok: true, value: v}            # destructuring pattern
  use(v)
when {id: ^wanted}                   # ^pin: equals existing var `wanted`, no binding
  mine()
when 0 then zero()                   # one-line arm: `then` (as in `if ... then`)
                                     # no guards: `when x > 5` is a compile error
else
  fallback()
end

# if / match / receive give a value to a let, an assignment or a return (nil when no branch is taken)
let size = if n > 10 then "big" else "small" end
total += match op
when "inc" then 1
else 0
end
                                     # not inside an expression: print(if ...) is an error; bind it first

# Modules — load (NOT import), export at end of file
load "std/string"                    # => string.split(...)  (namespace = file base name)
load "std/string" as str             # => str.split(...)
load "std/string" (split, trim)      # => split(...) directly

export my_fn, my_other_fn            # at end of module file

# Strings — double-quoted: interpolation, single-quoted: literal
"hello {name}"                       # interpolation with { }
'no {interpolation} here'            # literal braces
"""
multi-line with {interpolation}
"""                                  # opening quotes end their line
'''
multi-line literal
'''

# Numbers — 42 is an int, 2.0 a float (no exponent literals; to_float("1e-7")); 2 == 2.0 is false
# Floats print as the shortest text that reads back exactly: 0.1, 2.0, -0.0, 1e-05, 1e+16, inf, nan

# Operators — and/or/not (NOT &&/||/!), x in tbl, x in arr
# Tables — { key: val } or [1, 2, 3], dot access, bracket access (negative indexing supported; arr[i] past the end is nil, a string index out of range raises)
# Logical — nil and false are falsy, everything else truthy

# Concurrency
let pid = spawn do ... end
let pid = spawn_link do ... end       # spawn + link atomically
let {pid} = spawn_monitor do ... end
send(pid, msg)
let msg = receive()                  # pop head
receive                              # selective receive
when {tag: "DOWN", pid: p}
  handle(p)
when {tag: "ping"} then pong()       # one-line arm
after 5000
  timeout()                          # or: after 5000 then timeout()
end
receive                              # arms optional: just wait, mailbox untouched
after 100 then nil
end
monitor(pid)                         # → true (false if already monitoring); DOWN message on exit
demonitor(pid)                       # → true if a monitor was removed; a delivered DOWN stays
link(pid); unlink(pid)
process_flag("trap_exit", true)
kill(pid, "shutdown")                # → true if alive; EXIT msg if pid traps exits
                                     #   reason "normal" is ignored unless pid == self()
kill(pid, "kill")                    # untrappable: pid dies with reason "killed"
register("name", self())
whereis("name")                      # → pid or nil
let ref = make_ref()
let timer = send_after(pid, msg, 1000); cancel_timer(timer)
processes()                          # → array of live pids
process_info(pid)                    # → table or nil

# Run work concurrently and collect results (std/task)
let t = task.async do
  fetch(url)
end
let body = task.await(t)               # value, or re-raises the task's error
let [x, y] = task.await_all([t1, t2], 5000)

# Error handling
error("msg")                         # halt with stack trace; uncaught in main prints source line + stack trace
                                     # uncaught in a spawned process: that process dies, same report on stderr ("[Runtime Error in process <pid>]")
let r = pcall some_fn()              # {ok: bool, value/error: ..., stack: [{name, file, line}]} — names as in traces (`anonymous fn`, `mod.fn`)
# Stack: 8 MB per process; overflow raises "stack overflow in <fn>" (pcall-catchable, kills only that process)

# Common builtins
# I/O & process: print, eprint, error, pcall, len, type, to_string, to_int, to_float, exit, kill, argv, getenv, input, read_stdin, write_stdout, sleep
# Collections:   push, pop, keys, values, sort, insert, delete, remove_at, has_key
# Strings:       find, str_replace, substr, chr, ord, buf_new, buf_push, build_string
# Filesystem:    read_file, write_file, append_file, file_exists, remove_file, mkdir, list_dir, is_dir, dirname, path_join, normalize_path, exec
# TCP:           tcp_listen, tcp_accept, tcp_connect, tcp_read, tcp_write, tcp_close
# Time:          time_ms, epoch_ms, format_time, format_time_local
# SQLite:        sqlite_open, sqlite_close, sqlite_exec, sqlite_query, sqlite_last_insert_id, sqlite_changes
# Math:          floor, ceil, round, abs, pow, sqrt, random
# Builtin names aren't reserved: a fn/extern fn/let/param of the same name shadows the builtin in its scope (a top-level one: that file only)
# Bitwise:       band, bor, bxor, bnot, bshl, bshr

# String building
let s = build_string do |add|
  add("hello", " ", "world")           # multi-arg, no intermediate allocs
end

# Std library modules
# std/string     split, join, trim, upper, lower, repeat, index_of(s, x, start = 0), contains,
#                starts_with, ends_with
# std/table      each, map, filter, reduce, find, any, all, count, reverse, unique, contains, index_of,
#                slice, concat, copy, flatten, flat_map, zip, group_by, sort
# std/math       min, max, clamp, assert
# std/time       now, format, format_local, iso8601, http_date, date
# std/log        set_level, debug, info, warn, error
# std/json       parse, encode   (array = keys 0..n-1, else object with int keys as "42";
#                nesting > 1000 raises; ints past 64 bits parse as floats)
# std/http       response, ok, html, json_response, redirect, not_found, bad_request, server_error,
#                set_cookie, delete_cookie, parse_form, html_escape, router, start, serve, stop
#                (start opts {port, host, max_body = 8 MB, idle_timeout_ms, request_timeout_ms, write_timeout_ms = 30000, nil: none};
#                start returns {pid}; stop(server) closes every socket; HEAD uses the GET route;
#                set_cookie raises on ; , whitespace or control bytes: url.encode values)
# std/request    get, post, put, patch, delete, request  (http:// only; opts {body, headers, timeout_ms = 30000, nil: none};
#                returns {status, headers (lowercase names), body (chunked decoded)})
# std/url        encode, decode, parse, parse_query, build_query
# std/mime       lookup, ext
# std/sqlite     open, close, exec, query, last_id, changes  (wraps sqlite_* builtins)
# std/supervisor start, which_children, stop
#   child spec {id, start, restart, shutdown}: on stop, "shutdown", then "kill"
#   after `shutdown` ms (default 5000; nil = wait; a supervisor child: nil)
#   start returns once every child's start has; one that raises makes it raise
# std/dynamic_supervisor  start, start_child, terminate_child, which_children, stop
#   child template {start, restart, shutdown}, as a supervisor child spec
# std/task       async, await, await_all
# std/gen_server start, call, cast, reply
#   start returns once init has; an init that raises makes it raise
# std/test       case, assert, assert_eq, assert_neq, assert_throws, run  (assert_eq is deep, takes an optional msg; run exits 1 on failure)

# C interop
extern include "stdio.h"                 # a libc fn needs its header; your own: path relative to the .gem file
extern fn puts(s: String) -> Int
extern blocking fn net_read(fd: Int) -> String
```
