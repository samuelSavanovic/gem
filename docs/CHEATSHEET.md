# Gem Language Cheatsheet

A one-page summary of Gem syntax, builtins, and standard library modules. For full semantics see [`SPEC.md`](SPEC.md); for idioms and traps see [`BEST_PRACTICES.md`](BEST_PRACTICES.md).

```gem
# Variables
let x = 10
let {a, b} = tbl                     # table destructuring
let [first, second] = arr            # array destructuring
let {port = 8080, host = "0.0.0.0"} = opts   # field defaults (fire on a missing key, not nil)
let x = x + 1                        # same block: new variable shadowing the old x (to block end)
                                     # a let hiding an enclosing block's x (same fn) is a compile error: x = ... assigns
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
end                                                # `= {}` makes it an option bag, optional and nil-tolerant:
                                                   # a key not in the pattern raises: unknown option "prot" (did you mean "port"?)
fn handle({type, data = nil}) end                  # no `= {}`: a record pattern, other keys ignored
fn wrap({level = 1, ...rest} = {}) end             # ...rest: the other keys, no check

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
                                     # a bare name already in scope is a compile error: pin it (^x)
else
  fallback()
end                                  # no else and no arm matching raises "no match for <value>"; `else nil` to allow it

# if / match / receive give a value, anywhere an expression goes (nil when an if takes no branch)
let size = if n > 10 then "big" else "small" end
print("{n} item{if n == 1 then "" else "s" end}")
total += match op
when "inc" then 1
else 0
end
                                     # not as a param default or a `when` value

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

# Operators — and/or/not (NOT &&/||/!), x in arr (a value of a non-empty array, keys 0..n-1), x in tbl (= has_key(tbl, x) on any other table; int-keyed sets: has_key)
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
runtime_stats()                      # → {procs, memory, resets, reset_max_ms, ...}

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
                                     # pcall(some_fn()) runs some_fn() unprotected and calls what it returns (compiler warns)
# Stack: 8 MB per process; overflow raises "stack overflow in <fn>" (pcall-catchable, kills only that process)

# Common builtins
# I/O & process: print, eprint, error, pcall, len, type, to_string, to_int, to_float, exit, kill, argv, getenv, setenv, input, read_stdin, write_stdout, sleep
# Collections:   push, pop, keys, values, sort, insert, delete (a key; lists: remove_at), remove_at, has_key
# Strings:       find, str_replace, substr, chr, ord, buf_new, buf_push, build_string
# Filesystem:    read_file, write_file, append_file, file_exists, remove_file, rename, stat, mkdir, list_dir, is_dir, dirname, path_join, normalize_path
#                read_file(path, offset, count): a block ("" past the end); stat(path) -> {size, mtime, mode, type} or nil
# Commands:      exec(cmd) -> status (output to the terminal); exec_capture(cmd, input?) -> {status, stdout, stderr}
# TCP:           tcp_listen, tcp_accept, tcp_connect, tcp_read, tcp_write, tcp_close, tcp_peer, tcp_local_port, tcp_fd, tcp_from_fd
# TLS:           tls_connect(host, port, {timeout_ms, ca_file, server_name}) -> a socket for tcp_read/tcp_write/tcp_close;
#                verifies the certificate and host name against the system's CAs (or ca_file), always;
#                tcp_read raises when the server closes without its TLS close alert
# Resources:     a socket (type "socket") or db handle (type "sqlite") belongs to its opener; claim(r) makes the
#                caller its owner. Closed when a claimer exits, or when the opener crashes or is killed; anyone may close
# Time:          time_ms, epoch_ms, format_time, format_time_local
# SQLite:        sqlite_open, sqlite_close, sqlite_exec, sqlite_query, sqlite_last_insert_id, sqlite_changes
# Math:          floor, ceil, round, abs, pow, sqrt, random
# Builtin names aren't reserved: a fn/extern fn/let/param of the same name shadows the builtin in its scope (a top-level one: that file only)
# Names starting with `__` are reserved for the compiler (compile error); `t.__x` and `{__x: 1}` are fine
# Bitwise:       band, bor, bxor, bnot, bshl, bshr

# String building
let s = build_string do |add|
  add("hello", " ", "world")           # multi-arg, no intermediate allocs
end

# Std library modules
# std/string     split, join, trim, upper, lower, repeat, index_of(s, x, start = 0), contains,
#                starts_with, ends_with, closest(word, candidates) (did-you-mean)
# std/io         lines(path, f), reader(path) -> {next}, read_lines(path)   ("-": stdin; 64 KB blocks, flat memory; a pipe is read whole)
# std/regex      test, find, find_all, replace, replace_first, split, escape, check_pattern   (POSIX ERE + \d \w \s;
#                opts {icase, multiline}; match {text, start, stop, groups}; replace "\1" or fn(m); no \b or \1 in patterns;
#                a bound in double quotes interpolates: write '\d{4}')
# std/base64     encode, encode_url, decode (nil if not base64)     std/hex   encode, decode
# std/crypto     sha256, sha256_hex, hmac_sha256, hmac_sha256_hex, equal (constant time)
# std/utf8       len, valid, chars, codepoints, char(cp), sub(s, start, n), upper, lower, reverse
# std/csv        parse(text, {sep, header}), encode(rows, {sep, columns}), encode_row
# std/args       parse(spec), try_parse(spec, list), help(spec)
#   spec {name, about, usage, options: {top: {short: "n", default: 10, help}, v: {type: "flag"}, x: {multi: true}}, min_args, max_args}
#   -> {top, v, x, args: [positional]}; --help prints help; a mistake prints usage and exits 2
# std/table      each, map, filter, reduce, find, any, all, count, reverse, unique, contains, index_of,
#                slice, concat, copy, flatten, flat_map, zip, group_by, sort
# std/math       min, max, clamp, assert
# std/time       now, format, format_local, iso8601, http_date, date, parse(s) -> ms or nil (ISO 8601, HTTP dates)
#                fmt: strftime plus %L (milliseconds)
# std/log        set_level, debug, info, warn, error
# std/json       parse, encode(v, {indent})   (array = keys 0..n-1, else object with int keys as "42";
#                nesting > 1000 raises; ints past 64 bits parse as floats)
# std/http       response, ok, html, json_response, redirect, not_found, bad_request, server_error,
#                set_cookie, delete_cookie, parse_form, html_escape, router, start, serve, stop
#                (start opts {port, host, max_body = 8 MB, idle_timeout_ms, request_timeout_ms, write_timeout_ms = 30000, nil: none,
#                max_connections, max_per_ip, on_event = nil; on_event(ev): ev.kind open|request|close|refused|crash};
#                req.peer = {ip, port};
#                start returns {pid, port} (port 0: a free one); stop(server) closes every socket; HEAD uses the GET route;
#                set_cookie raises on ; , whitespace or control bytes: url.encode values)
# std/request    get, post, put, patch, delete, request  (http:// and https://; opts {body, headers, timeout_ms = 30000, nil: none, ca_file};
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
# std/gen_server start(module, {name}), call, cast, reply, stop   (name: registered before init)
#   start returns once init has; an init that raises makes it raise
# std/test       case, assert, assert_eq, assert_neq, assert_throws, run  (assert_eq is deep, takes an optional msg; run exits 1 on failure)

# C interop
extern include "stdio.h"                 # a libc fn needs its header; your own: path relative to the .gem file
extern fn puts(s: String) -> Int
extern blocking fn net_read(fd: Int) -> String
# gem -I<dir> -L<dir> -l<lib> app.gem   C flags go before the file; CC, CFLAGS, LDFLAGS from the environment

# Command line
# gem app.gem [args]        build at -O0 (binary cached, see SPEC; GEM_CACHE=off) and run; -- passes options on
# gem app.gem -o app        build at -O2, don't run        gem --check app.gem   errors only
# gem new demo              demo/ with gem.toml (name, main), main.gem, test.gem
# gem test [args]           run the project's test.gem     gem build   main at -O2 into build/<name>
# make install PREFIX=$HOME/.local                          puts gem in $PREFIX/bin
```
