# Process-owned sockets: design

Status: **agreed, not implemented.** Phase 2 of `examples/honeypot/PLAN.md`; implements ROADMAP "Process-owned resources closed on exit" for sockets and sqlite handles. Measured against `build/gem` at d07ae59.

This is the second version. The first moved ownership implicitly whenever a socket was captured by `spawn` or sent; an adversarial review showed that every short-lived helper (a reader, a `task.async`, a watchdog) then took the socket over by accident, and each fix added another rule. "Rejected designs" below says why each alternative lost.

## Problem

Sockets are plain ints. Nothing closes one when the process holding it dies, so every crash or kill leaks the fd. The honeypot runs one session process per connection and expects sessions to crash on hostile input.

Measured on Linux (counting `list_dir("/proc/self/fd")`):

| Experiment | fds before | fds after |
|---|---|---|
| An acceptor spawns a session per connection; 500 sessions each read once and `error()` | 7 | 507 |
| Then the acceptor (which holds the listener) is killed with `"kill"` | 507 | 507 |
| 100 processes, each blocked in `tcp_read` on its own `tcp_connect` socket, killed with `"kill"` | 107 | 107 |

The int representation also lets one connection read another's data today. A process blocked in `tcp_read` is woken by `poll`; before it runs, another process closes that socket and `tcp_accept`s a new connection, which gets the same fd number; the woken reader then `read()`s the new connection. Reproduced: the reader of the closed socket received `"SECRET-for-s2"`, written to the new one. (`gem_io_fd_closed` only flags processes still on the fd waiter list, and the tcp builtins keep the fd number in a C local across the wait.)

## The rules

1. **A socket is a value of its own type**, `type(s) == "socket"`. It holds an id into a runtime socket table (slot + generation, never reused), not the fd. Copies (spawn, send, module snapshots, region resets) copy the id, like a `make_ref()` ref.
2. **Every socket has one owner**: the process that opened it with `tcp_listen`, `tcp_accept`, `tcp_connect` or `tcp_from_fd`.
3. **`tcp_claim(sock)` makes the calling process the owner**, taking it from whoever owned it. Nothing else changes ownership: `spawn`, `send` and every copy leave it where it is. Returns `sock`.
4. **When the owner crashes, is killed or dies from a link's exit signal, the runtime closes the sockets it owns.** A normal exit (the function returns) closes nothing; the sockets it owned become ownerless.
5. **Any process that has a socket can read and write it. Only the owner can close it** while the owner is alive: `tcp_close` from another process raises `tcp_close: socket is owned by process <pid>; call tcp_claim in the process that should close it`. An ownerless socket can be closed by anyone. Closing a closed socket returns `nil`.
6. **A closed socket stays closed in every process.** Any use other than `tcp_close` raises `<fn>: socket is closed`, including a builtin that was waiting on it when it closed: every tcp builtin re-checks the socket after each wait, so no Gem code ever touches a reused fd number.

SQLite handles get rules 2 and 4 (owner = opener, closed when it crashes or is killed), with no type change and no claim (see "SQLite handles").

### What the user writes

```gem
# acceptor: accept and hand off; the acceptor owns each socket until a session claims it
while true
  let sock = tcp_accept(listener)
  send(registry, {sock: sock, peer: tcp_peer(sock)})
end

# session: claim first, then work; a crash or kill closes the socket
fn session(sock)
  tcp_claim(sock)
  ...
  tcp_close(sock)
end
```

The one line a handoff needs is in the process that takes responsibility, so a relay through any number of processes (acceptor → registry → session) needs no pid plumbing, and there is no window where the giver must wait for the receiver to exist.

## Why this rule

**Helpers never take a socket by accident.** A reader process that reads the connection's socket (stomp_broker, mini_redis), a `task.async` that writes to it, a watchdog that closes it on idle: none of them claim, so their crash, timeout kill or normal exit leaves the owner's socket alone. A helper that does claim has said so in its code.

**Forgetting the claim is loud on the normal path.** A session that never claims fails its own `tcp_close` with the message in rule 5, because the acceptor that still owns the socket is alive. That shows up the first time a test runs a session to completion, not after the honeypot has leaked fds for a week. What stays silent: a session that forgets the claim *and* never reaches its close (it always crashes). `GEM_DIAG=1` reports at exit the open sockets their owner never read, wrote or waited on while another process did (`gem_diag: sockets_unclaimed=N`), which catches that case in a load test.

**Listeners need no special case.** A listener stays with whoever opened it. mini_redis and stomp_broker open theirs in `main`, start a supervisor and return: `main`'s normal exit leaves the listener open and ownerless, and a restarted acceptor keeps accepting on it. A server that should own its listener claims it (std/http, below).

**Normal exit closes nothing, as agreed in the first review.** Closing on every exit, as Erlang does, makes "open a socket in a helper and return it" (a `task.async(fn() tcp_connect(...) end)`) a race: the helper's normal exit would close the socket before the caller can claim it. The leak the honeypot is about is the crash; a normal path ends in `tcp_close`, and rule 5 makes a forgotten claim there loud.

## Rejected designs

- **Keep ints, transfer by an explicit builtin, track the owner by fd.** Closing by fd number on exit hits reused numbers (std/http's server closes a connection's fd on `DOWN`, by which time the runtime may have closed it and the number been reused). Unsound.
- **Keep ints, transfer implicitly when an int equal to an owned fd is sent or captured.** The runtime can't tell fd 7 from the integer 7. Unsound.
- **Socket type, implicit transfer on `spawn`/`send`** (the first version). Helpers take the socket: a reader that returns at EOF leaves it ownerless, a `task.await` timeout that kills its task closes the caller's connection, and a listener captured by a supervised acceptor closes when the acceptor crashes, so every restart fails. Each fix (listeners don't move; hand ownership back on a normal exit) is another rule to learn.
- **Socket type, `tcp_give(sock, pid)` by the current owner** (Erlang's `controlling_process`). Same safety as `tcp_claim`, but a relay chain needs a give at every hop with the next pid at hand, and the give comes from the process that is *not* responsible for closing. A forgotten give is silent: the session's own close succeeds, nothing raises.
- **Shared ownership: close when the last process it was handed to exits.** The acceptor and the registry have held every socket and live forever, so it leaks exactly as today.
- **Close when no process can reach it** (reachability, released at region resets). A loop only resets after about 1 MB of allocation (`GEM_ARENA_RESET_THRESHOLD`); an accept loop that spawned 50 sessions did no reset at all. Dead sessions' sockets would stay open for an unpredictable time, and fds are scarce.

## Details

### What closes, and when

| How the owner ends | Its sockets |
|---|---|
| Returns normally (main included) | stay open, ownerless |
| Uncaught error | closed |
| `kill(pid, reason)`, any reason but `"normal"` (`"kill"` included) | closed |
| Exit signal from a link, not trapped | closed |
| Main crashes, or `exit(code)` | the program ends; the OS closes everything |

The close runs in `gem_free_proc_slot`, after the process is `DEAD` (off the waiter lists), when `exit_reason` is set and isn't `"normal"` (set on every abnormal path and still readable there). It wakes every waiter on any of the closed fds in one scan of the fd waiters, then `close()`s them.

**A process killed while its own `extern blocking fn` runs on the thread pool**: the worker may be using an fd it got through `tcp_fd`, so that process's sockets are closed when its `GemIORequest` is released (the last release, by the worker), not at the kill. Another process's blocking call on an fd it doesn't own isn't covered: the runtime can't know which fds a C call uses. The `tcp_fd` docs say so.

### Transfer and copies

Nothing in the copy machinery changes ownership, so `gem_copy.c` only needs a `VAL_SOCKET` case that copies the value, and module snapshot units treat it like an int (no `saw_mutable`). Region resets, deep copy and deep free have nothing to free: the value holds no arena or malloc memory.

`tcp_claim` on a socket the caller already owns does nothing; on a closed socket it raises `tcp_claim: socket is closed`. A claim never waits and never fails because of the old owner. Two processes claiming in turn is allowed: the last claim wins.

A socket sent to a process that dies before claiming it stays with the sender (the acceptor); a session that crashes before its first line runs leaves it with the acceptor too. These are the gaps `sockets_unclaimed` counts. A permanent child restarted by a supervisor with a socket captured in its spec gets a closed socket after its first crash and raises `socket is closed` on every restart until the restart intensity is reached: loud, and a BEST_PRACTICES rule (open connections in the child, not in the spec).

### fd reuse

- The value is a table id; `tcp_close` and the exit close look the entry up, wake waiters, `close()` the fd once and free the slot with a new generation. Every later use of any copy finds no entry.
- Every tcp builtin re-resolves the id after each `gem_io_yield` and raises `socket is closed` if the entry is gone, instead of retrying `read`/`write`/`accept` on the fd number it holds (the cross-connection read above).
- **An fd closed behind the runtime's back** (C code via `tcp_fd`; examples/197's last case): the table keeps an fd → entry index. When `socket()`, `accept()` or `tcp_from_fd` produces an fd the index still maps to an entry, that entry is retired without `close()` (the number already belongs to the new socket), so a later `tcp_close` of the old value can't close the new connection.
- C code that keeps a `tcp_fd` number after the socket is closed can still hit a reused fd; that is C's side of the contract.

### `tcp_fd` and `tcp_from_fd`

- `tcp_fd(sock)` returns the fd number, for `extern fn`s and debugging. The fd stays valid while the socket is open; C code must not close it (use `tcp_close`) or keep it after the socket closes, which happens when its owner crashes.
- `tcp_from_fd(n)` registers a TCP socket made outside the runtime (a TLS library, an fd inherited from the parent process) and returns it, owned by the caller. It checks `SO_TYPE` is `SOCK_STREAM` (`tcp_from_fd: fd 9 is not a TCP socket`) and that the fd isn't registered already (`tcp_from_fd: fd 9 is already registered as a socket`), and marks it non-blocking and close-on-exec. From then on the runtime owns the fd.

### Value behaviour

- `type(s)` is `"socket"`. Prints as `#Socket<N>`, N the table id: stable for the value's whole life, never reused, unlike the fd.
- Equal iff same id, in any process. Usable as a table key, like a ref.
- The tcp builtins take and return sockets; an int raises `tcp_read: expected a socket, got int`.
- `tcp_peer` on a closed socket raises `tcp_peer: socket is closed` (today: `getpeername failed: Bad file descriptor`, or a reused fd's peer).

### Errors the user sees

- `<fn>: socket is closed`, for `tcp_read`, `tcp_write`, `tcp_accept`, `tcp_peer`, `tcp_fd`, `tcp_claim`; replaces `… failed: Bad file descriptor` for a socket closed while the call waited. `tcp_connect`'s `connect failed: socket closed while connecting` stays.
- `<fn>: expected a socket, got <type>`.
- `tcp_close: socket is owned by process <pid>; call tcp_claim in the process that should close it`.
- `tcp_from_fd: fd <n> is not a TCP socket` / `is already registered as a socket`.

## SQLite handles

The honeypot's recorder opens the database and owns it for its whole life; a recorder crash today leaks the connection on every restart. Handles are already ids that are never reused (`gem_sqlite_register`), so there is no reuse race and no type change: each entry gets an owner, the process that opened it, and the owner's crash or kill closes it. `sqlite_close` stays callable from any process (no claim exists for sqlite, so restricting it would only break code); a close from a non-owner just drops the owner's count.

The exit close goes to the thread pool like `sqlite_close`: the entry is cleared at once, and the runtime releases the requester's side of the request right after submitting it, so the worker's release frees it. If the queue is full it closes inline, keeping its own `db` pointer (a failed `gem_io_submit_extern` has already freed the args). A per-process count of owned handles lets an exit with none skip the registry scan. Killed mid-open and mid-close are handled already (`gem_sqlite_open_free`; the entry is cleared before the yield).

## Existing programs

| Program | Change |
|---|---|
| examples/tcp_echo.gem | `tcp_claim(fd)` at the top of `echo_loop`, or its `tcp_close` raises |
| examples/mini_redis | `tcp_claim(fd)` at the top of `connection.run`; the reader borrows. `trap_exit` there exists so the connection can close its socket when the reader crashes; it can stay (removing it is a later cleanup) |
| examples/stomp_broker | `tcp_claim(fd)` at the top of `connection.run_writer`; same note |
| std/http | `tcp_claim(fd)` at the top of `handle_connection`; `tcp_claim(listen_fd)` at the top of `server_main`. User-visible effect: a server killed with the untrappable `"kill"` now frees its port, and a caller of `http.start` that crashes no longer matters to it. Its `DOWN` → `tcp_close` path becomes a no-op on an already-closed socket; removing it and `_http_closing` is a later cleanup |
| std/request | none: it opens and closes in the caller, which now gets its socket closed if killed mid-request; update its doc comment |
| bookmark_app | none (std/http) |
| examples/197 | expected output: `type` line, the six `Bad file descriptor` lines, the reuse check and the C `close` through `tcp_fd` |
| examples/179, 191 | `serve_once` spawns a server that closes the listener its caller opened: `tcp_claim(lfd)` at the top of the spawned block |
| examples/208 | `exec("test -e /dev/fd/{tcp_fd(l)}")`; as written it would interpolate `#Socket<N>` and pass without testing close-on-exec |
| examples/216 | expected output: `getpeername failed: Bad file descriptor` and `expected int socket fd` lines |

The other numbered examples that use tcp (51, 94, 177, 185, 188, 190, 195, 209, 212) close each socket in the process that opened it, or close a socket whose opener has returned; `make test` finds any miss as a `socket is owned by` error.

## Docs

- **SPEC.md**: TCP section in terms of sockets; a "Socket ownership" paragraph with the six rules; `tcp_claim`, `tcp_fd`, `tcp_from_fd`; `type()` gains `"socket"`; SQLite: a handle closes when the process that opened it crashes or is killed.
- **BEST_PRACTICES.md**: replace "The process that opens a handle closes it, on every path" with "Claim a socket in the process that closes it"; a **(trap)** rule for a socket captured in a restartable child's spec; update the `Ptr` rule's comparison with sockets; trap index rows.
- **CHEATSHEET.md**: the three builtins, `"socket"`, one line on ownership.
- **ROADMAP.md**: the entry keeps `exec`'s child process, plus the std/http, mini_redis and stomp cleanups.
- Editors: `tcp_claim`, `tcp_fd`, `tcp_from_fd` in both grammars.

## Tests

- `examples/217_socket_values.gem`: `type`, printing, equality, table keys, `tcp_close` twice, every builtin on a closed socket, an int passed to each builtin; `tcp_fd`; `tcp_from_fd` on a socket from C `socket()`, on a registered fd, on a pipe, on a closed fd.
- `examples/218_socket_ownership.gem`: the peer reads `""` after the owner crashes, is killed (`"shutdown"`, `"kill"`) or dies from a link; a trapping owner and a normal return leave it open; a borrower's crash, kill and normal exit leave it open; `tcp_claim` moves it (the new owner's crash closes it, the old one's doesn't); a non-owner's `tcp_close` raises while the owner lives and works once it has returned; a waiter in another process raises `socket is closed`.
- `examples/219_socket_reuse_race.gem`: the reviewer's cross-connection read (closer and reader woken by one poll, the closer accepts a connection with the same fd number): the reader raises `socket is closed`. And a C `close()` through `tcp_fd`, then a new socket on that number: `tcp_close` of the old value leaves the new one open.
- `examples/220_sqlite_owner_exit.gem`: a handle whose opener crashes or is killed is closed (`not an open database handle` from another process); one whose opener returns stays open.
- `tests/check_socket_leak.sh`, in `make test`, counting `/dev/fd` (works on Linux and macOS): the honeypot shape (acceptor → `send` → registry → `spawn` → session that claims) with 1,000 crashing sessions; 1,000 sessions killed while blocked in `tcp_read`; 100 processes killed mid-`tcp_connect` (a listener whose 1,024-entry backlog is filled first, since connects to a listener that doesn't accept otherwise complete at once); then the count must equal the baseline. Today: 507 and 107 against 7.
- `GEM_DIAG=1` output for an unclaimed socket in `tests/notes`-style expected stderr, or a line in the leak script.
- `make test-example-apps` with the claim lines added.

## Implementation steps

1. **Value type.** `VAL_SOCKET` in `runtime/gem.h` (id in `ival`); equality and key/hash in `gem.h` and `gem_core.c` (`gem_key_indexable`, `gem_key_hash`); `type`, print, `to_string`, interpolation in `gem_builtins_core.c` and `gem_builtins_string.c`; the copy case in `gem_copy.c`. Modelled on `VAL_REF`.
2. **Socket table** in `runtime/gem_builtins_tcp.c`: entries `{fd, gen, owner slot, owner used it, another process used it, prev/next in the owner's list}`, a free list, an fd → entry index; `GemProcess.sockets` in `gem.h`, initialised in `gem_spawn_fn` and for main. Every builtin resolves its argument; `tcp_connect` registers before its first yield; resolve again after every `gem_io_yield`; `tcp_close` checks the owner. New builtins `tcp_claim`, `tcp_fd`, `tcp_from_fd`.
3. **Exit close** in `gem_free_proc_slot` (`runtime/gem_scheduler.c`) for abnormal exits, one waiter scan for all of the process's fds; deferred to the request's release when the process has a pool request in flight; normal exits unlink the entries (ownerless).
4. **Compiler**: the three builtins in `BUILTIN_FNS`, and in `LEAF_BUILTINS` (they run no Gem code and never yield), in `compiler/builtins.gem`; then `make bootstrap`. Editor grammars.
5. **`GEM_DIAG=1`**: `sockets_unclaimed`.
6. **Claims** in tcp_echo, mini_redis, stomp_broker, std/http; examples 197, 208, 216.
7. **Docs and tests** as above; `make test`.
8. **SQLite** (separate commit): owner slot per registry entry and owned count per process in `runtime/gem_builtins_sqlite.c`, closed through the pool on abnormal exit.

Step 2 also fixes two `docs/KNOWN_BUGS.md` entries, deleted with it: "A woken tcp waiter can read or write a reused fd" (the cross-connection read) and "TCP builtins truncate the fd argument to a C `int`" (an int is no longer accepted).

## Decisions (agreed with the user)

1. **Normal exit closes nothing**; a crash, kill or link exit closes.
2. **SQLite handles are in phase 2** (step 8).
3. **`tcp_from_fd(n)`** is added, with `tcp_fd`.
4. **Ownership changes only through `tcp_claim`** (second version, after the review): no implicit move on `spawn` or `send`, and only the owner closes while it lives.
5. **The std/http cleanups** (dropping its fd bookkeeping and `_http_closing`) **are later.** Its two `tcp_claim` lines are part of phase 2, since without them its `tcp_close` calls would raise.
