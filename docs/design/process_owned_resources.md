# Process-owned resources: design

Status: **agreed, not implemented.** Phase 2 of `examples/honeypot/PLAN.md`; implements ROADMAP "Process-owned resources closed on exit" for sockets and sqlite handles. Measured against `build/gem` at d07ae59.

Third version, after two reviews. The first moved ownership implicitly whenever a resource was captured by `spawn` or sent, so every short-lived helper (a reader, a `task.async`, a watchdog) took sockets over by accident. The second used `tcp_claim` with owner-only close; its review found that normal exits leaked, that owner-only close did no safety work, and that sockets and sqlite followed two models. "Rejected designs" says why each alternative lost.

## Problem

Sockets and sqlite handles are plain ints. Nothing closes one when the process holding it dies, so every crash or kill leaks the fd or the connection. The honeypot runs one session process per connection and expects sessions to crash on hostile input.

Measured on Linux (counting `list_dir("/proc/self/fd")`):

| Experiment | fds before | fds after |
|---|---|---|
| An acceptor spawns a session per connection; 500 sessions each read once and `error()` | 7 | 507 |
| Then the acceptor (which holds the listener) is killed with `"kill"` | 507 | 507 |
| 100 processes, each blocked in `tcp_read` on its own `tcp_connect` socket, killed with `"kill"` | 107 | 107 |

The int representation also lets one connection read another's data today ("A woken tcp waiter can read or write a reused fd" in `docs/KNOWN_BUGS.md`, repro checked): one `poll` wakes a reader and a closer; the closer closes the reader's socket and accepts a new connection with the same fd number; the reader then `read()`s the new connection.

## The rules

1. **An owned resource is a value of its own type**: a socket (`type` is `"socket"`, from `tcp_listen`, `tcp_accept`, `tcp_connect`, `tcp_from_fd`) or a database handle (`type` is `"sqlite"`, from `sqlite_open`). It holds an id into one runtime resource table (slot + generation, never reused), not the fd or the `sqlite3 *`. Copies (spawn, send, module snapshots, region resets) copy the id, like a `make_ref()` ref.
2. **Every resource has an owner**, at first the process that opened it.
3. **`claim(r)` makes the calling process the owner and marks the resource claimed.** Nothing else changes ownership: `spawn`, `send` and every copy leave it where it is. Returns `r`.
4. **A claimed resource is closed when its owner exits, for any reason. An unclaimed one is closed when its opener crashes, is killed or dies from a link's exit signal**; when the opener returns normally it stays open, with no owner.
5. **Any process that has a resource can use it and close it.** Closing a closed resource returns `nil` (`tcp_close`, `sqlite_close`).
6. **A closed resource stays closed in every process.** Any use other than closing raises, including a builtin that was waiting on it when it closed: every tcp builtin re-resolves the socket after each wait, so no Gem code ever touches a reused fd number.

What a user learns: *a resource belongs to whoever opened it or last claimed it; it closes when a process that claimed it exits, or when its opener crashes or is killed; anyone can use or close it.*

### What the user writes

```gem
# acceptor: accept and hand off; it owns each socket until a session claims it
while true
  let sock = tcp_accept(listener)
  send(registry, {sock: sock, peer: tcp_peer(sock)})
end

# session: claim first; any exit (crash, kill, early return, orderly shutdown) closes it
fn session(sock)
  claim(sock)
  ...
end
```

A relay through any number of processes (acceptor → registry → session) needs one line, in the process that takes responsibility, with no pid plumbing and no window where the giver must wait for the receiver to exist.

## Why this rule

**Claiming says "this lives and dies with me", for any exit.** A session that returns early, or shuts down cleanly on a `"shutdown"` EXIT, closes its socket, as Erlang would.

**Opening alone doesn't tie the resource to a normal exit.** A helper that opens a socket and returns it (`task.async(fn() tcp_connect(...) end)`) would otherwise close it before the caller could claim it. mini_redis and stomp_broker open their listener in `main`, start a supervisor and return; the listener stays open, ownerless, and a restarted acceptor keeps accepting on it. A crash still closes what the opener held, which is the leak the honeypot is about.

**Helpers never take a resource by accident.** A reader process that reads the connection's socket (stomp_broker, mini_redis), a `task.async` that writes to it, a watchdog that closes it on idle, a manager that kicks a client by closing its socket: none of them claim, so their crash, timeout kill or normal exit leaves the owner's socket alone.

**Anyone may close.** Restricting close to the owner would only make a forgotten claim raise; with the reuse race fixed it adds no safety, and it forbids managers and watchdogs.

**The one concept is `claim`, and it can't be pushed into the compiler or runtime.** Which process is responsible for a connection is a fact about the program's structure; the runtime can't infer it from copies (the first version tried, and helpers broke it) or from reachability (the timing is unusable, below). Erlang's `controlling_process` is the same concept. Here it is one name, shared by every kind of resource, written in the process that takes the responsibility.

### Visibility of a forgotten claim

A session that forgets `claim` and crashes leaves its socket with the acceptor, open. Nothing raises, so it must be visible instead:

- `process_info(pid).resources`: the number of open resources the process owns. A test, the honeypot's metrics or a user can check that an acceptor's count stays near zero while the server runs.
- `GEM_DIAG=2` prints a `gem_resources:` line when a process exits leaving resources open that it owned or used without owning: `gem_resources: process 412 (session) exited (error) after using 1 socket owned by process 3 (acceptor)`. Each entry records its last user, kept in that process's list. An owner that hands all its I/O to separate reader and writer processes is a false positive; it is a diagnostic, not an error.
- `GEM_DIAG=1` prints `resources_open=N ownerless=M` at exit.
- The leak test (below) runs the honeypot shape and checks the fd count.

## Rejected designs

- **Keep ints, transfer by an explicit builtin, track the owner by fd.** Closing by fd number on exit hits reused numbers. Unsound.
- **Keep ints, transfer implicitly when an int equal to an owned fd is sent or captured.** The runtime can't tell fd 7 from the integer 7. Unsound.
- **Resource type, implicit transfer on `spawn`/`send`** (version 1). Helpers take the socket: a reader that returns at EOF leaves it ownerless, a `task.await` timeout that kills its task closes the caller's connection, and a listener captured by a supervised acceptor closes when the acceptor crashes, so every restart fails. Each fix adds a rule.
- **`tcp_claim` with owner-only close and normal exit closing nothing** (version 2). Early returns and orderly shutdown leak; watchdogs and managers can't close; sqlite gets a second model.
- **`give(r, pid)` by the current owner** (Erlang's `controlling_process`). As safe as `claim`, but a relay chain needs a give at every hop with the next pid at hand, the call is in the process that is not taking the responsibility, and a helper that opens a resource for its caller must give it before it returns.
- **Shared ownership: close when the last process it was handed to exits.** The acceptor and the registry have held every socket and live forever, so it leaks exactly as today.
- **Close when no process can reach it** (reachability, released at region resets). A loop resets only after about 1 MB of allocation (`GEM_ARENA_RESET_THRESHOLD`); an accept loop that spawned 50 sessions did no reset at all. Dead sessions' sockets would stay open for an unpredictable time, and fds are scarce.
- **Per-kind claims (`tcp_claim`, `sqlite_claim`, …).** Every future resource (TLS streams, `exec` children, UDP or Unix sockets, `Ptr`s with a finalizer) would add a name and possibly its own rules; renaming later would touch std and user code.

## Details

### What closes, and when

| How the owner ends | Claimed resources | Unclaimed (opener's) resources |
|---|---|---|
| Returns normally (main included) | closed | stay open, ownerless |
| Uncaught error | closed | closed |
| `kill(pid, reason)`, any reason but `"normal"` (`"kill"` included) | closed | closed |
| Exit signal from a link, not trapped | closed | closed |
| Main crashes, or `exit(code)` | the program ends; the OS closes everything | same |

A process that traps exits receives `EXIT` and doesn't die; if it then returns, that is a normal exit.

The close runs in `gem_free_proc_slot`, after the process is `DEAD` (off the waiter lists), with `exit_reason` deciding "normal" (set on every abnormal path and still readable there). One scan of the fd waiters wakes every waiter on any of the closed sockets, then the fds are `close()`d. Sqlite handles close through the thread pool (below).

**A process killed while its own `extern blocking fn` runs on the thread pool**: the worker may be using an fd it got through `tcp_fd`, so that process's sockets are not closed at the kill. They move to a scheduler-side parked list together with the process's reference to its `GemIORequest` (owners are recorded as full pids, not slots, so the slot's next process can't appear to own them). When the scheduler drains the pool's wake pipe it checks each parked request's `done` flag (set by the worker before it writes the pipe); for each finished one it closes the sockets on the scheduler thread and releases the reference. A `claim` of a parked socket by another process takes it off the list. A hung C call (a DNS lookup, say) keeps that process's sockets, a listener included, open as long as it runs. Another process's blocking call on an fd it doesn't own isn't covered: the runtime can't know which fds a C call uses. A `Socket` extern parameter type would tell it (ROADMAP).

### Transfer and copies

Nothing in the copy machinery changes ownership, so `gem_copy.c` only needs a case that copies the value, and module snapshot units treat it like an int (no `saw_mutable`). Region resets, deep copy and deep free have nothing to free: the value holds no arena or malloc memory.

`claim` on a resource the caller already owns only marks it claimed; on a closed one it raises `claim: socket is closed` (`claim: database is closed`). A claim never waits and never fails because of the old owner; two processes claiming in turn is allowed, and the last claim wins.

Gaps: a socket sent to a process that dies before claiming it stays with the sender, as does one held by a session that crashes before its first line; the visibility tools above show both. A restartable child that **claims** a resource captured in its child spec closes it with its first crash, and every restart then raises on a closed resource until the restart intensity is reached: loud, and a BEST_PRACTICES rule (a child that claims opens its own resource, as std/http's server process does with its listener). A child that only uses a resource its parent opened doesn't close it when it crashes.

### fd reuse

- A value is a table id. Closing looks the entry up, wakes waiters, closes the fd once and frees the slot with a new generation; every later use of any copy finds no entry.
- Every tcp builtin re-resolves the id after each `gem_io_yield` and raises if the entry is gone, instead of retrying `read`/`write`/`accept` on the fd number it holds (the cross-connection read).
- **An fd closed behind the runtime's back** (C code calling `close` on a number from `tcp_fd`; examples/197's last case) breaches C's contract and is only partly covered. Each socket entry records the fd's `st_dev`/`st_ino` at registration; before the runtime closes a socket it `fstat`s the fd and, on a mismatch, retires the entry without `close()`, so the runtime never closes a file, pipe or sqlite database that took the number. A `tcp_read`/`tcp_write` on the stale value can still reach whatever reused it; checking on every I/O would cost a syscall per call for a contract breach.
- C code that keeps a `tcp_fd` number after the socket is closed can likewise hit a reused fd.

### `tcp_fd` and `tcp_from_fd`

- `tcp_fd(sock)` returns the fd number, for `extern fn`s and debugging. The fd stays valid while the socket is open; C code must not close it (use `tcp_close`) or keep it after the socket closes.
- `tcp_from_fd(n)` registers a TCP socket made outside the runtime (a TLS library, an fd inherited from the parent process) and returns it, opened by the caller in the sense of rule 2. It checks `SO_TYPE` is `SOCK_STREAM` (`tcp_from_fd: fd 9 is not a TCP socket`) and that no open socket entry has the same fd and inode (`tcp_from_fd: fd 9 is already registered as a socket`), and marks it non-blocking and close-on-exec. From then on the runtime owns the fd.

### Value behaviour

Like a `make_ref()` ref (checked against refs today):

- `type` is `"socket"` / `"sqlite"`. Prints as `#Socket<N>` / `#Sqlite<N>`, N the table id: stable for the value's whole life and never reused, unlike the fd.
- Equal iff same id, in any process. Usable as a table key.
- `<` raises `type error in <: got socket and socket`; `json.encode` raises `json.encode: cannot encode a socket`; `to_int` raises `to_int: cannot convert socket to int`; `sort` treats them as it treats refs.
- A resource can't be copied to another node: when distribution exists (ROADMAP), sending one there raises.

### Builtin signatures and errors

- The tcp builtins take and return sockets, the sqlite builtins sqlite handles; anything else raises `tcp_read: expected a socket, got int` / `sqlite_query: expected a database handle, got int` (int is newly rejected for sqlite).
- Use of a closed resource: `<fn>: socket is closed` (`tcp_read`, `tcp_write`, `tcp_accept`, `tcp_peer`, `tcp_fd`, `claim`; replaces `… failed: Bad file descriptor` for a socket closed while the call waited, and `tcp_peer`'s `getpeername failed: Bad file descriptor`; `tcp_connect`'s `connect failed: socket closed while connecting` stays). For sqlite, `<fn>: not an open database handle` as today, except that `sqlite_close` of a closed handle now returns `nil`.
- New: `claim(r)`, `tcp_fd(sock)`, `tcp_from_fd(n)` with the errors above. `process_info(pid)` gains `resources`.

## Existing programs

| Program | Change |
|---|---|
| examples/tcp_echo.gem | `claim(fd)` at the top of `echo_loop` (optional: without it a crashed session's socket isn't closed, as today) |
| examples/mini_redis | `claim(fd)` at the top of `connection.run`; the reader only uses the socket. The `trap_exit` there, which exists so the connection can close its socket when the reader crashes, can stay; removing it is a later cleanup |
| examples/stomp_broker | `claim(fd)` at the top of `connection.run_writer`; same note |
| std/http | `claim(fd)` at the top of `handle_connection`; `claim(listen_fd)` at the top of `server_main`. User-visible: a server killed with the untrappable `"kill"` frees its port, and a caller of `http.start` that crashes no longer closes the server's listener. Its `DOWN` → `tcp_close` path becomes a no-op on an already-closed socket; removing it and `_http_closing` is a later cleanup |
| std/request | none: it opens and closes in the caller, which now gets its socket closed if killed mid-request; update its doc comment |
| std/sqlite | doc comments: a handle is a value of type `"sqlite"`, `close` of a closed handle returns `nil`, a handle closes per rule 4 |
| bookmark_app | none |
| examples/180 | expected output: the int and closed-handle lines (`got int`, `sqlite_close` of a closed handle) |
| examples/197 | expected output: `type` line, the six `Bad file descriptor` lines, the reuse check and the C `close` through `tcp_fd` |
| examples/208 | `exec("test -e /dev/fd/{tcp_fd(l)}")`; as written it would interpolate `#Socket<N>` and pass without testing close-on-exec |
| examples/216 | expected output: the `getpeername failed: Bad file descriptor` and `expected int socket fd` lines |

Every other numbered example that uses tcp or sqlite keeps working unchanged, since anyone may close: they open and use each socket in one process or hand it to helpers that only use it, and none compares one with an int or prints one. The step that switches the types greps every `.gem` file for a resource passed to arithmetic, interpolation or an `extern fn` first; `make test` catches the rest.

## Docs

- **SPEC.md**: TCP and SQLite sections in terms of resource values; an "Owned resources" section with the six rules and the exit table; `claim`, `tcp_fd`, `tcp_from_fd`; `type()` gains `"socket"` and `"sqlite"`; `process_info`'s `resources`.
- **BEST_PRACTICES.md**: replace "The process that opens a handle closes it, on every path" with "Claim a resource in the process responsible for it"; a **(trap)** rule for a restartable child that claims a resource from its spec; update the `Ptr` rule's comparison with sockets; trap index rows.
- **CHEATSHEET.md**: `claim`, `tcp_fd`, `tcp_from_fd`, the two types, one line on ownership.
- **ROADMAP.md**: the entry keeps `exec`'s child process (a third resource kind), plus the later std/http, mini_redis and stomp cleanups; new: a `Socket` extern parameter type.
- **CLAUDE.md**: the runtime file list gains `gem_resource.c`; a Key Decisions bullet on owned resources (a new kind registers a close callback; every runtime path that closes one goes through the table).
- Editors: `claim`, `tcp_fd`, `tcp_from_fd` in both grammars.

## Tests

- `examples/217_resource_values.gem`: `type`, printing, equality, table keys, `<`, `json.encode`, `to_int`; closing twice; every builtin on a closed socket and a closed handle; an int passed to each builtin; `tcp_fd`; `tcp_from_fd` on a socket from C `socket()`, on a registered fd, on a pipe and on a closed fd.
- `examples/218_resource_ownership.gem`: unclaimed: the peer reads `""` after the opener crashes, is killed (`"shutdown"`, `"kill"`) or dies from a link, and the opener's normal return leaves it open and ownerless; claimed: closed on the claimer's normal return, crash, kill and orderly `trap_exit` shutdown; a helper that only uses it leaves it open whether it crashes, is killed or returns; a `task.async` that opens a socket and returns it hands over a live socket; a watchdog's `tcp_close` works; a waiter in another process raises `socket is closed`; `process_info(pid).resources` counts.
- `examples/219_socket_reuse_race.gem`: the KNOWN_BUGS repro, where the reader now raises `socket is closed`; a C `close()` through `tcp_fd`, then a new socket and a file on that number: closing the old value leaves both open.
- `examples/220_sqlite_ownership.gem`: an unclaimed handle whose opener crashes or is killed is closed (`not an open database handle` from another process), one whose opener returns stays open; a claimed one closes when its claimer returns; a pool that lends handles and a borrower that claims one.
- `examples/221_resource_pool_kill.gem`: a process killed while its `extern blocking fn` sleeps holds its socket open until the call returns, then it closes; no other process's socket is closed.
- `tests/check_socket_leak.sh`, in `make test`, counting `/dev/fd` (Linux and macOS): the honeypot shape (acceptor → `send` → registry → `spawn` → session that claims) with 1,000 sessions that crash and 1,000 that return early; 1,000 sessions killed while blocked in `tcp_read`; 100 processes killed mid-`tcp_connect` (to a listener whose 1,024-entry backlog is filled first, since connects to a listener that doesn't accept otherwise complete at once). The count must equal the baseline (today: 507 and 107 against 7). Also the `GEM_DIAG=2` line for a session that forgot `claim` and crashed.
- `make test-example-apps` with the claim lines added.

## Implementation steps

1. **Resource table**, new `runtime/gem_resource.c` (the Makefile builds `runtime/gem_*.c`): entries `{kind, gen, owner pid, claimed, last-user pid, owner-list and user-list links, payload: fd + st_dev/st_ino for a socket, sqlite3 * for a handle}`, a free list, a close callback per kind, `GemProcess.resources` and `GemProcess.used` list heads in `gem.h` (initialised in `gem_spawn_fn` and for main), the parked list, `claim`.
2. **Value type**: `VAL_RESOURCE` in `runtime/gem.h` (id in `ival`; the kind is in the entry and, since a closed entry keeps none, encoded in the id too); equality and key/hash in `gem.h` and `gem_core.c` (`gem_key_indexable`, `gem_key_hash`); `type`, print, `to_string`, interpolation in `gem_builtins_core.c` and `gem_builtins_string.c`; the copy case in `gem_copy.c`. Modelled on `VAL_REF`.
3. **TCP** (`runtime/gem_builtins_tcp.c`): every builtin resolves its argument; `tcp_connect` registers before its first yield; re-resolve after every `gem_io_yield`; `tcp_close` through the table with the inode check; `tcp_fd`, `tcp_from_fd`.
4. **Exit close** in `gem_free_proc_slot` (`runtime/gem_scheduler.c`): claimed on any exit, unclaimed on abnormal exit; one waiter scan; the parked list when a pool request is in flight, closed from the wake-pipe drain; normal exits unlink the unclaimed entries (ownerless); the `GEM_DIAG` lines.
5. **SQLite** (`runtime/gem_builtins_sqlite.c`): the registry becomes the table's sqlite kind; `sqlite_close` of a closed handle returns `nil`; the exit close is submitted to the pool, the runtime releasing the requester's side at once; on a full queue it closes inline, keeping its own `db` pointer (a failed `gem_io_submit_extern` has already freed the args). A WAL checkpoint can block the scheduler there, so that is the full-queue fallback only.
6. **`process_info`** gains `resources`.
7. **Compiler**: `claim`, `tcp_fd`, `tcp_from_fd` in `BUILTIN_FNS` and `LEAF_BUILTINS` (`compiler/builtins.gem`), then `make bootstrap`. Editor grammars.
8. **Claim lines** in tcp_echo, mini_redis, stomp_broker, std/http; examples 180, 197, 208, 216.
9. **Docs and tests** as above; `make test`.

Step 3 also fixes two `docs/KNOWN_BUGS.md` entries, deleted with it: "A woken tcp waiter can read or write a reused fd" (the cross-connection read) and "TCP builtins truncate the fd argument to a C `int`" (an int is no longer accepted).

## Decisions (agreed with the user)

1. **Claimed resources close on any exit of their owner; unclaimed ones only on the opener's crash, kill or link exit** (replaces "normal exit closes nothing" after the second review).
2. **SQLite is in phase 2**, on the same model and value type as sockets.
3. **`tcp_fd` and `tcp_from_fd`** are added.
4. **Ownership changes only through `claim`**: no implicit move on `spawn` or `send`; any process may close.
5. **One mechanism for every owned resource**: one table, one value type whose `type` is the kind, a generic `claim`.
6. **Later, not in phase 2**: the std/http cleanups (dropping its fd bookkeeping and `_http_closing`), the `trap_exit` cleanups in mini_redis and stomp_broker, and a `Socket` extern parameter type. std/http's two `claim` lines are part of phase 2.
