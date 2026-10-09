# Process-owned sockets: design

Status: **proposal, not implemented.** Phase 2 of `examples/honeypot/PLAN.md`; implements the socket part of ROADMAP "Process-owned resources closed on exit". Measured against `build/gem` at d07ae59.

## Problem

Sockets are plain ints. Nothing closes one when the process holding it dies, so every crash or kill leaks the fd. The honeypot runs one session process per connection and expects sessions to crash on hostile input, so it would leak fds under load.

Measured on Linux (counting `list_dir("/proc/self/fd")`):

| Experiment | fds before | fds after |
|---|---|---|
| An acceptor spawns a session per connection; 500 sessions each read once and `error()` | 7 | 507 |
| Then the acceptor (which holds the listener) is killed with `"kill"` | 507 | 507 |
| 100 processes, each blocked in `tcp_read` on its own `tcp_connect` socket, killed with `"kill"` | 107 | 107 |

Two more facts the design depends on:

- **Main can return while the program keeps running.** A main that opens a listener, spawns an acceptor with it and returns: the acceptor still accepts afterwards. `examples/mini_redis/main.gem` and `examples/stomp_broker/main.gem` work like this: `main` opens the listener, starts the supervisor and returns.
- **A loop's region reset runs only once the loop has allocated about 1 MB** (`GEM_ARENA_RESET_THRESHOLD`, checked by `gem_arena_reset_due`). An accept loop that spawned 50 sessions did no reset at all (`GEM_DIAG=1` printed no `arena_resets` line). So "a socket closes once no process can reach it" can't be built on resets: an acceptor would keep thousands of dropped sockets reachable between resets.

## Decision summary

1. **A socket is a new value type**, `type(s) == "socket"`. The value carries an id into a runtime socket table, not the fd. Copies (spawn, send, module snapshots, region resets) copy the id, like a `make_ref()` ref.
2. **Every socket has one owner process.** `tcp_listen`, `tcp_accept` and `tcp_connect` make the calling process the owner.
3. **The runtime closes a process's sockets when the process exits abnormally**: an error it doesn't catch, `kill`, or an exit signal from a link. A normal exit (the function returns) closes nothing, as today.
4. **Handing a connection to another process moves its ownership**, if the process handing it over is the owner: capturing it in a closure passed to `spawn`/`spawn_link`, or sending it in a message with `send`. **A listening socket never moves**: it stays with the process that opened it.
5. **Any process that has a socket can use and close it.** Ownership only decides who it dies with.
6. **`tcp_close` is idempotent and can't hit a reused fd.** Closing frees the table entry; the stale id then means "closed" forever. Any other use of a closed socket raises `<fn>: socket is closed`.

The honeypot's flow then needs no extra code: the acceptor sends `{sock, ip}` to the registry (the registry owns it), the registry spawns a session with it (the session owns it), and a session that crashes or is killed has its socket closed.

## 1. What a process owns

Sockets from `tcp_listen`, `tcp_accept` and `tcp_connect`. `tcp_connect` registers the socket right after `socket()`, before it yields waiting for the connect, so a process killed mid-connect doesn't leak it (today it does).

**SQLite handles: later, and simpler.** The honeypot has one recorder process that owns its database for its whole life, so sqlite isn't on the critical path. sqlite handles are already ids that are never reused (`gem_sqlite_register`). They can get an owner (the opener, never moving) and be closed on its abnormal exit with no change to their type, since nothing has to recognise them in a copy. That is step 8 below, separate from the socket work. `exec`'s child process (`system()` hides its pid) stays in the ROADMAP entry; it needs `posix_spawn` and is unrelated.

## 2. What closes, and when

| How the owner ends | Its sockets |
|---|---|
| Returns normally | stay open, as today (see "Why not close on normal exit") |
| `exit(code)` (ends the program) | the OS closes everything |
| Uncaught error | closed |
| `kill(pid, reason)` with any reason but `"normal"`, `"kill"` included | closed |
| Exit signal from a link (not trapped) | closed |
| Main ends normally | stay open; main's arena and globals already outlive it |
| Main crashes | the program exits (`exit(1)`), and the OS closes everything |

A process that traps exits receives `EXIT` and doesn't die, so it closes nothing.

The close runs in `gem_free_proc_slot`, after the process is `DEAD` (which takes it off the fd waiter list), through the same path as `tcp_close`: `gem_io_fd_closed(fd)`, then `close(fd)`. A process in another process blocked in `tcp_accept`/`tcp_read`/`tcp_write`/`tcp_connect` on that socket wakes and raises `<fn>: socket is closed`, exactly as when another process calls `tcp_close` today (examples/197).

### Why not close on normal exit

Erlang closes a port on any exit of its owner. Here that breaks two patterns already in the repo, because ownership moves on `spawn`:

- **Reader helpers.** `examples/stomp_broker/connection.gem` (always) and `examples/mini_redis/connection.gem` (once a client subscribes) spawn a linked reader with the socket, so the reader would own it. At end of input the reader sends `eof` and returns; closing then would cut off replies the writer still owes a client that half-closed after its last command.
- **Short helpers.** A `task.async` that writes to the socket, or any `spawn do log_peer(sock) end`, would close the caller's socket when it finishes.
- And `main` returns while its listener is in use (mini_redis, stomp), so main would need an exception.

Closing on crashes and kills only fixes the leak that matters (a process that dies before its cleanup runs) without any of these. The cost: a process that returns without `tcp_close` still leaks, as today. That's a bug in the program; the honeypot's sessions close on every normal path. `GEM_DIAG=1` will report sockets left open by a normal exit (step 6), so the honeypot's metrics can see them.

## 3. Ownership transfer

### Options

**(a) Keep ints; the runtime tracks the owner by fd; an explicit builtin transfers it** (`tcp_give(fd, pid)`, like Erlang's `gen_tcp:controlling_process`).
- Every acceptor in the repo (std/http, tcp_echo, mini_redis, stomp) and the honeypot would have to call it; forgetting it is a silent leak in a process that lives forever, which is the bug we're fixing.
- Worse, closing by fd number on exit makes the fd-reuse race real. std/http's server closes a connection's fd on `DOWN`; once the runtime has already closed that fd at the crash, the number may belong to a new connection, and the server closes the wrong socket. std/http's `_http_closing` message exists to avoid this race today; it would have to be redone.
- Rejected: an extra concept with a silent failure mode, and not sound.

**(b) Keep ints; transfer implicitly when an int equal to an owned fd is sent or captured.**
- Unsound: the runtime can't tell fd 7 from the integer 7. `send(pid, {count: 7})` would transfer fd 7, and so would a port number, a length or a pid that happens to be equal.
- Rejected.

**(c) A socket value type that the copy recognises** (recommended).
- The copy into another process sees `VAL_SOCKET` values and can move ownership exactly; no false positives.
- Closing goes through the table entry, so reuse races are gone (section 5).
- Breaking change for code that treats a socket as an int (below), which in the repo is one test (examples/197) and the docs.

Two rules for **when** (c) moves ownership were considered and rejected:

- *Close when no process can reach the socket any more* (refcounting copies, released at resets and exits): no move rule at all, but the timing depends on resets, which run only every ~1 MB of allocation (measured above). An acceptor would keep thousands of dead sessions' sockets open.
- *Move on every spawn/send, from any process holding it*: std/http's acceptor spawns the connection and then sends the fd to the server for bookkeeping, and the server would end up owning every connection. Moving only from the owner makes that order harmless: the acceptor is no longer the owner when it sends.

### The recommended rule

A `spawn`/`spawn_link` whose closure captures a connection, or a `send` whose message contains one, moves its ownership to the new process / the receiver if the current process owns it. It happens once the spawn or send succeeds: a `spawn` that fails (process table full) or a `send` to a dead process leaves the owner unchanged, so `pcall spawn(...)` followed by `tcp_close(fd)` on failure (std/http, tcp_echo, mini_redis, stomp) keeps working. Copies that aren't handoffs don't move it: module-slot snapshots at `spawn`, messages the runtime itself sends, and `send_after` (its message is copied when the timer is set and delivered later by the scheduler; moving a socket there would hand it to a process at an unpredictable time).

Walking through the programs that pass sockets around:

| Program | Flow | Result |
|---|---|---|
| honeypot (planned) | acceptor `send`s `{sock, ip}` to registry; registry `spawn`s session | session owns it; a crash closes it; a registry crash closes those still in its mailbox |
| std/http | acceptor `spawn`s connection, then `send`s fd to server (`_http_conn`) | connection owns it; the server's `tcp_close` on `DOWN` becomes a no-op on an already-closed socket instead of a possible wrong-fd close |
| tcp_echo | acceptor `spawn`s `echo_loop` | as std/http; main's `tcp_close(listener)` after killing the server still works (main owns the listener) |
| mini_redis, stomp_broker | main opens listener; supervisor's child spec captures it; acceptor `spawn`s connections; connection `spawn_link`s a reader | listener stays with main, so an acceptor restart reuses it and `stop` still "doesn't close the listener"; the reader owns the connection: a reader crash closes it, and a connection crash kills the reader through the link, which closes it |
| std/request | `tcp_connect` in the caller | a caller killed mid-request (a `task.await` timeout) no longer leaks its socket |
| bookmark_app | std/http | as std/http |

No program in the repo needs a code change for correctness. Two optional improvements: std/http can open its listener inside the server process, so a server killed with the untrappable `"kill"` frees its port (its comment says today it doesn't); and its `_http_closing` message and per-connection fd tracking could go, since the runtime now closes a crashed connection's socket. Neither is needed for phase 2.

### Trade-offs of (c)

- **Backwards compatibility.** `type(sock)` was `"int"` and is now `"socket"`; arithmetic on a socket, `to_int`, passing it to an `extern fn` taking `Int` and comparing it with an int all stop working. The tcp builtins reject ints: `tcp_read: expected a socket, got int`. In the repo only examples/197 depends on the int (it prints `type(s3)`, compares two fds for reuse and closes one from C through `extern fn close(fd: Int)`). A new builtin `tcp_fd(sock)` returns the fd number for C interop and debugging; the caller keeps the socket value alive and doesn't close the fd behind the runtime's back (that is what 197 tests, and it still raises cleanly). There is no int → socket conversion (open question 3).
- **Printing.** `#Socket<7>` while open (the fd, which is what `strace` and `/proc/self/fd` show), `#Socket<closed>` once closed, following `#Ref<N>`.
- **Equality and table keys.** Two socket values are equal iff they have the same id, in any process. Usable as a table key, like a ref.
- **Signatures.** Unchanged except the types: `tcp_listen`/`tcp_accept`/`tcp_connect` return a socket; `tcp_read`, `tcp_write`, `tcp_close`, `tcp_peer` take one. `tcp_peer` on a closed socket raises `tcp_peer: socket is closed` (today it raises `getpeername failed: Bad file descriptor`, or reports a reused fd's peer).
- **Module globals.** A socket is an immediate value like an int or a ref: snapshot units copy it, it doesn't make a unit single-use, and a lazy slot copy never moves ownership. A module-level `let LISTENER = tcp_listen(...)` stays with the process that ran the module's top level (main).
- **Region resets, deep copy, deep free.** Nothing to do: the value holds no arena or malloc memory. The only copy change is noting the sockets a spawn or send carries.
- **Two processes holding the same socket.** Both can read, write and close it, as with ints today; reads interleave. The owner decides when it dies. A `tcp_close` from either closes it for both and wakes the other's waits.

## 4. Main and long-lived servers

Main's sockets follow the same rules: main returning closes nothing (main's arena and module state already stay until the program ends), and main crashing ends the program. A listener opened in main and used by acceptors (mini_redis, stomp) keeps working after main returns, and survives any number of acceptor restarts, because listeners don't move. A listener that should close with its server is opened by the server process itself (the honeypot's acceptor does this, so a supervisor restart rebinds the port after a kill).

## 5. fd reuse

The value holds a table id (slot + generation), never the fd. `tcp_close` and the exit close both look the entry up, wake waiters, `close()` the fd once, and free the slot with a new generation. Afterwards every copy of that value, in every process, finds no entry: `tcp_close` returns `nil` without touching any fd, and every other builtin raises `socket is closed`. The runtime only ever closes an fd through its entry, so it can't close a number another socket has taken. Waiters woken by a close are matched by fd number in `gem_io_fd_closed` while the fd is still open, so they can't be confused with a later socket either (as today).

The one remaining way to hit a reused fd is C code given `tcp_fd(sock)` that keeps the number after the socket is closed, which the `tcp_fd` docs warn against.

## 6. What the user sees

Errors (all pcall-catchable, prefixed with the builtin name):

- `tcp_read: socket is closed`: the socket was closed before the call, or while the call waited (replaces today's `read failed: Bad file descriptor` for the second case). Same for `tcp_write`, `tcp_accept`, `tcp_peer`, `tcp_fd`; for `tcp_connect`, `connect failed: socket closed while connecting` stays.
- `tcp_read: expected a socket, got int` for a non-socket argument (likewise the others).
- No new message for the exit close itself: the peer sees EOF or a reset, and a local waiter gets the error above.

Docs:

- **SPEC.md**: TCP section returns/takes sockets; a new "Socket ownership" paragraph with rules 2–6; `type()` gains `"socket"`; `tcp_fd`; equality and keys next to `make_ref`.
- **BEST_PRACTICES.md**: rewrite "The process that opens a handle closes it, on every path" for sockets (crashes and kills are covered; normal paths still close; hand a connection to the process that will own it); a **(trap)** rule: sending your socket to a process that only does bookkeeping makes that process the owner, so send your pid, or send the socket after handing it off (std/http's order); update the `Ptr` rule's comparison with sockets. Trap index rows for both.
- **CHEATSHEET.md**: `tcp_fd`, `type` returning `"socket"`, one line on ownership.
- **ROADMAP.md**: the entry keeps sqlite (if step 8 is deferred) and `exec`.
- Editors: `tcp_fd` in both grammars.

## 7. Tests

- `examples/217_socket_values.gem`: `type`, printing open and closed, equality, as a table key, `tcp_close` twice, every builtin on a closed socket, `tcp_fd`, an int passed to each tcp builtin.
- `examples/218_socket_owner_exit.gem`: the peer reads `""` after its owner crashes, is killed (`"shutdown"` and `"kill"`) or dies from a link; a trapping owner and a normal return leave it open; a process in another process blocked in `tcp_read` on it raises `socket is closed`.
- `examples/219_socket_handoff.gem`: spawn capture and send move it (the new owner's crash closes it, the old owner's doesn't); a non-owner's send doesn't (std/http's order); a listener stays with its opener while an acceptor crashes and a new one accepts on it; a socket in a module global doesn't move; `send_after` doesn't move.
- `examples/197`: expected output changes (`type` line, the reuse check through `tcp_fd`, the C close through `tcp_fd`).
- `tests/check_socket_leak.sh` (Linux only; skips elsewhere), wired into `make test`: the honeypot shape (acceptor → `send` → registry → `spawn` → session), 1,000 sessions that crash, 1,000 killed while blocked in `tcp_read`, and 100 clients killed mid-`tcp_connect` to a non-accepting listener; `/proc/self/fd` must be back to the baseline count. Today this measures 507 and 107 against 7 (table above).
- `tests/check_proc_limit.sh`: a `spawn` that fails under a low `GEM_MAX_PROCS` leaves the socket with the spawner.
- The example apps' own tests (`make test-example-apps`) unchanged.

## 8. Implementation steps

1. **Value type.** `VAL_SOCKET` in `runtime/gem.h` (id in `ival`), equality and hashing in `gem.h`/`gem_core.c`, `type`/print/`to_string`/interpolation in `gem_builtins_core.c` and `gem_builtins_string.c`, the copy case in `gem_copy.c`. Modelled on `VAL_REF`.
2. **Socket table** in `runtime/gem_builtins_tcp.c`: entries `{fd, gen, owner slot, is_listener, prev/next in the owner's list}`, a free list, `GemProcess.sockets` (list head) in `gem.h`. The tcp builtins resolve their argument through it; `tcp_connect` registers before its first yield; `tcp_close` frees the entry. New builtin `tcp_fd`.
3. **Close on abnormal exit** in `gem_free_proc_slot` (`runtime/gem_scheduler.c`), from the exit reason (`exit_reason` set and not `"normal"`), after the state is `DEAD`. A normal exit unlinks the entries and leaves them ownerless.
4. **Moves**: the copies in `gem_spawn_fn` (closure env) and in the `send` builtin (not `gem_send_msg`'s internal callers or timers) collect the sockets they meet; once the spawn or send has succeeded, each one the current process owns and that isn't a listener moves to the new process.
5. **Compiler**: `tcp_fd` in `BUILTIN_FNS` and `LEAF_BUILTINS` (`compiler/builtins.gem`), then `make bootstrap`. Editor grammars.
6. **`GEM_DIAG=1`** reports sockets left open by normal exits (`gem_diag: sockets_orphaned=N`).
7. **Docs and tests** (sections 6 and 7); `make test`.
8. **SQLite** (separate commit; open question 2): owner = opener, never moves, closed through the thread pool on the owner's abnormal exit; ids stay ints.

Optional, after: std/http opens its listener in the server process and drops its fd bookkeeping.

## Open questions for the user

1. **Normal exit**: close only on crash/kill/exit signal (recommended, keeps reader helpers, `task.async` helpers and main-returns-early servers working), or on every exit like Erlang (no leak from a forgotten `tcp_close`, but those patterns break and main needs an exception)?
2. **SQLite now or later**: do step 8 in phase 2, or leave it in the ROADMAP until a program needs it?
3. **An int → socket conversion** (`tcp_from_fd(n)`) for sockets made by C code (a future TLS library, an inherited fd): add it now, or wait for a user?
4. **`send` moving ownership**: keep it (the honeypot's acceptor → registry → session flow needs it), or move only on `spawn` and have the honeypot's acceptor spawn sessions itself after asking the registry? `send` moving is the source of the bookkeeping trap in section 6.
5. **std/http cleanup** (listener in the server process, no fd tracking): part of phase 2, or later?
