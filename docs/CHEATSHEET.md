# Gem Language Cheatsheet

A one-page summary of Gem syntax, builtins, and standard library modules. For full semantics see [`SPEC.md`](SPEC.md).

Keep this file up to date when adding new syntax, keywords, builtins, or std modules.

```gem
# Variables
let x = 10
let {a, b} = tbl                     # table destructuring
let [first, second] = arr            # array destructuring
let {port = 8080, host = "0.0.0.0"} = opts   # field defaults (fire on missing or nil)

# Functions — fn/end, last expression is implicit return
fn add(a, b)
  a + b
end
fn greet(name, greeting = "Hello")   # default params
  print("{greeting}, {name}!")
end
fn log(level, ...msgs)               # variadic (rest param)
end
fn server({port = 8080, host = "0.0.0.0"} = {})   # destructured params
end                                                # `= {}` makes the bag optional / nil-tolerant

# Closures / anonymous functions
let f = fn(x) x * 2 end

# Blocks — trailing do/end or { } passed as last arg
items.each do |item|
  print(item)
end
items.each { |item| print(item) }

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

for item in arr ... end              # array iteration
for k, v in tbl ... end              # key-value iteration
for i = 0, n ... end                 # range [0, n)

match val
when "a"
  handle_a()
when {ok: true, value: v}            # destructuring pattern
  use(v)
else
  fallback()
end

# Modules — load (NOT import), export at end of file
load "std/string"                    # => string.split(...)
load "std/string" as str             # => str.split(...)
load "std/string" (split, trim)      # => split(...) directly

export my_fn, my_other_fn            # at end of module file

# Strings — double-quoted: interpolation, single-quoted: literal
"hello {name}"                       # interpolation with { }
'no {interpolation} here'            # literal braces
"""multi-line with {interpolation}"""
'''multi-line literal'''

# Operators — and/or/not (NOT &&/||/!), x in tbl, x in arr
# Tables — { key: val } or [1, 2, 3], dot access, bracket access (negative indexing supported)
# Logical — nil and false are falsy, everything else truthy

# Concurrency
let pid = spawn do ... end
let pid = spawn_link do ... end       # spawn + link atomically
let {pid, ref} = spawn_monitor do ... end
send(pid, msg)
let msg = receive()                  # pop head
receive                              # selective receive
when {tag: "DOWN", pid: p}
  handle(p)
after 5000
  timeout()
end
monitor(pid)                         # → ref
link(pid); unlink(pid)
process_flag("trap_exit", true)
register("name", self())
whereis("name")                      # → pid or nil
let ref = make_ref()
let timer = send_after(pid, msg, 1000); cancel_timer(timer)
processes()                          # → array of live pids
process_info(pid)                    # → table or nil

# Error handling
error("msg")                         # halt with stack trace; uncaught in main prints source context + caret
let r = pcall some_fn()              # {ok: bool, value/error: ...}

# Common builtins
# I/O & process: print, eprint, error, pcall, len, type, to_string, to_int, to_float, exit, kill, argv, getenv, input, sleep
# Collections:   push, pop, keys, values, sort, insert, delete, remove_at, has_key
# Strings:       str_replace, substr, chr, ord, buf_new, buf_push, build_string
# Filesystem:    read_file, write_file, append_file, file_exists, remove_file, mkdir, list_dir, is_dir, dirname, path_join, normalize_path, exec
# TCP:           tcp_listen, tcp_accept, tcp_connect, tcp_read, tcp_write, tcp_close
# Time:          time_ms, epoch_ms, format_time, format_time_local
# SQLite:        sqlite_open, sqlite_close, sqlite_exec, sqlite_query, sqlite_last_insert_id, sqlite_changes
# Math:          floor, ceil, round, abs, pow, sqrt, random
# Bitwise:       band, bor, bxor, bnot, bshl, bshr

# String building — build_string needs () before do block
let s = build_string() do |add|
  add("hello", " ", "world")           # multi-arg, no intermediate allocs
end

# Std library modules
# std/string     split, join, trim, upper, lower, repeat, index_of, contains, starts_with, ends_with
# std/table      each, map, filter, reduce, find, any, all, count, reverse, unique, contains, index_of,
#                slice, concat, copy, flatten, flat_map, zip, group_by
# std/math       min, max, clamp, assert
# std/time       now, format, format_local, iso8601, http_date, date
# std/log        set_level, debug, info, warn, error
# std/json       parse, encode
# std/http       response, ok, html, json_response, redirect, not_found, bad_request, server_error,
#                set_cookie, delete_cookie, parse_form, html_escape, router, start, serve
# std/request    get, post, put, patch, delete, request
# std/url        encode, decode, parse, parse_query, build_query
# std/mime       lookup, ext
# std/sqlite     open, close, exec, query, last_id, changes  (wraps sqlite_* builtins)
# std/supervisor start, which_children
# std/gen_server start, call, cast, reply
# std/test       case, assert, assert_eq, assert_neq, assert_throws, run

# C interop
extern fn puts(s: String) -> Int
extern blocking fn net_read(fd: Int) -> String
extern include "header.h"
```
