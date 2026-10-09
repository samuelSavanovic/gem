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

## Standard library

## Language server

## Example programs
