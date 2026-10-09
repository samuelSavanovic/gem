# Known bugs

Bugs found and not yet fixed. Each entry has a minimal repro (checked
against `build/gem`), what goes wrong, and where the code is. A fix deletes
its entry in the same change, along with any **(bug)** rule in
`docs/BEST_PRACTICES.md` that exists because of it. Performance problems go in
`docs/OPTIMIZATIONS.md`, missing features in `docs/ROADMAP.md`.

## Compiler

## Runtime

### TCP builtins truncate the fd argument to a C `int`

```gem
let l = tcp_listen("127.0.0.1", 18323)
let c = tcp_connect("127.0.0.1", 18323)
let s = tcp_accept(l)
tcp_write(c, "hi")
print(tcp_read(s + 4294967296, 16, 1000))   # hi
tcp_close(c + 4294967296)                   # closes c
print(tcp_read(s, 16, 1000) == "")          # true
```

Every `tcp_*` builtin casts its fd with `(int)args[0].ival`
(runtime/gem_builtins_tcp.c), so an int outside the C `int` range wraps to
some other, possibly open, fd instead of raising. Extra arguments are
ignored too (`tcp_peer(fd, 1)`). A range check in one shared fd helper
would fix all of them.

### A woken tcp waiter can read or write a reused fd

```gem
let me = self()
let l = tcp_listen("127.0.0.1", 18433)
let c1 = tcp_connect("127.0.0.1", 18433)
let s1 = tcp_accept(l)
let cx = tcp_connect("127.0.0.1", 18433)
let sx = tcp_accept(l)
let c2 = tcp_connect("127.0.0.1", 18433)
spawn do                     # the closer: a lower slot than the reader
  tcp_read(sx)
  tcp_close(s1)
  let s2 = tcp_accept(l)     # takes s1's fd number
  send(me, ["s2 reused s1's fd", s2 == s1])
  tcp_write(c2, "SECRET-for-s2")
end
spawn do
  let r = pcall tcp_read(s1)
  send(me, ["reader of s1 got", r.ok, r.value, r.error])
end
sleep(20)
tcp_write(cx, "go")
tcp_write(c1, "for-s1")
print(receive())             # ["s2 reused s1's fd", true]
print(receive())             # ["reader of s1 got", true, "SECRET-for-s2", nil]
```

One `poll` wakes both processes. The closer runs first, closes `s1` and
accepts a new connection on the same fd number; the reader of `s1` then
`read()`s the new connection's data. `gem_io_fd_closed`
(runtime/gem_scheduler.c) only flags processes still on the fd waiter list,
and the tcp builtins (runtime/gem_builtins_tcp.c) keep the fd number in a C
local across `gem_io_yield`. `tcp_write` and `tcp_accept` have the same
window. The socket table in `docs/design/process_owned_resources.md`
fixes it (every builtin re-checks the socket after each wait).

## Standard library

## Language server

## Example programs
