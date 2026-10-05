# jobqueue

A job queue with a supervised worker pool, run under seeded fault
injection. It is the yardstick for failure under load: the part of Gem's
OTP model ("let it crash") that the other examples exercise only on the
happy path. Workers crash, hang, run slow or get killed in the middle of
a job, by the hundred per second; the queue retries with backoff, kills
workers that outlive their deadline and dead-letters jobs that keep
failing, and a supervisor restarts what dies. At the end of every run the
program checks that no job was lost or finished twice, that every attempt
failed exactly as the fault schedule said, and that the system is back at
its baseline. `benchmarks/jobqueue/run.sh` runs the same design in Python
asyncio and in Elixir/OTP.

```sh
cd examples/jobqueue
../../build/gem main.gem -o jobqueue
./jobqueue --jobs 10000 --crash 0.05 2> /dev/null
../../build/gem test.gem              # the tests (part of make test)
```

```
jobs          10000
completed     10000
dead_letters  0
attempts      10487
retries       487
failures      crash=487 killed=0 deadline=0 storm=0 shutdown=0 noproc=0 other=0
late_results  0
sup_restarts  0
wall_ms       698
throughput    14326 jobs/s
latency_ms    p50=0 p90=2 p99=14 max=75
tick_lag_ms   28
processes     baseline=4 peak=21 final=4
rss_kb        baseline=3292 peak=70152 final=63252 stopped=47012
invariants    ok
```

Each crashed worker prints its crash report on stderr, as any Gem process
does. The exit status is 0 when the invariants hold, 1 when one fails
(the summary lists the first 20), 2 for a bad command line.

## What it supports

A job is `{id, kind, payload, max_retries, deadline_ms}`. Kinds: `sum`
(adds up `0 .. payload-1`), `text` (builds a string of `payload` pairs and
returns its length) and `sleep` (1 to 3 ms). The queue:

- runs each job on a pool worker, at most `--workers` at once, started
  on demand under a `dynamic_supervisor` and retired after `--idle` ms
  without work;
- kills a worker whose attempt outlives the job's deadline (`--deadline`,
  0 for none) and counts the attempt as failed;
- retries a failed attempt after `--backoff` ms, doubled for each retry
  up to `--backoff-max`, at most `--retries` times, then puts the job on
  the dead-letter list;
- notifies the submitter once per job, when it completes or is
  dead-lettered, and answers `status(id)`, `stats()`, `dead_letters()`
  and `report()`.

The driver (`driver.gem`) submits `--jobs` jobs of a seeded workload from
a producer process, collects their notifications, waits for the pool to
drain, checks the invariants and prints the summary above. Faults come
from a schedule that is a pure function of `--seed`, the job id and the
attempt number (`schedule.gem`): with probability `--crash` an attempt
raises, `--hang` it never finishes (its deadline kills it), `--slow` it
first sleeps `--slow-ms`, `--kill` it kills its own worker
(`kill(self(), "kill")`). A storm (`--storm N --storm-every MS
--storm-bursts K`) kills N workers K times, with reason `"storm"`; with
`--max-restarts` low enough, the worker supervisor gives up and its own
supervisor starts a new one. `./jobqueue --help` lists every option.

The invariants (`invariants.gem`):

- every job got exactly one notification and is either completed or on
  the dead-letter list, and the queue agrees with the notifications;
- every attempt ended as the schedule says (completed, crashed, killed,
  or killed by its deadline), except those a storm or a supervisor
  shutdown cut short, which are counted apart; a completed job has one
  successful attempt and it is the last; a dead job used all its retries;
  a completed job's result is right;
- the queue's attempt and retry counters match the jobs' histories;
- after the drain, the queue has no pending, busy or idle work and no
  workers; the process count, the queue's monitors, links and mailbox and
  the worker supervisor's links are what they were before the load.

RSS is reported, not checked: the queue keeps every job's record (that is
what `status` answers from), so memory grows with the jobs run.

## Design

```
jobqueue_sup (supervisor, one_for_one, 20 restarts in 10 s)
├── jobqueue_workers (dynamic_supervisor, transient workers)
│   └── worker × up to --workers   ── receive loop: run, report, retire
└── jobqueue (gen_server)          ── the jobs, the pool, timers
```

| File | What it holds |
|---|---|
| `main.gem` | entry point: options, run, print |
| `options.gem` | the command line |
| `jobqueue.gem` | the supervision tree and the public API |
| `queue.gem` | the gen_server: dispatch, deadlines, retries, dead letters |
| `worker.gem` | the pool worker and the job kinds; injects the faults |
| `fifo.gem` | the pending queue |
| `schedule.gem` | the seeded workload and fault schedule |
| `invariants.gem` | the checks at the end of a run |
| `driver.gem` | a whole run: producer, collector, storm, drain, summary |
| `test.gem` | unit tests, the queue's behaviours, small seeded runs |

Choices worth knowing:

- **The pool's size is the supervisor's child count.** The queue asks the
  `dynamic_supervisor` for a worker when jobs wait and the pool has room,
  and counts it in `slots`. A worker that crashes keeps its slot: the
  supervisor restarts it (`restart: "transient"`), and the new process
  announces itself to the queue with a cast. A worker idle for `--idle`
  ms asks the queue to retire; the queue says yes only if it hasn't just
  sent that worker a job, and frees the slot. When the worker supervisor
  itself gives up, the queue sees its `DOWN`, drops every slot, and looks
  the new supervisor up by name.
- **The queue learns of a failure from the worker's `DOWN`**, never from
  the worker: a crash, a kill and a storm are all the same event with a
  different reason. Nothing in the worker catches errors; letting it
  crash is the point.
- **An attempt is identified by a ref**, not by its job. A `done` from
  an attempt the queue has already failed (the worker finished just as
  its deadline fired) doesn't match the busy entry's ref and is counted
  as `late_results`, so a job can't complete twice.
- **Deadlines and backoff are `send_after` timers to the queue.** A
  deadline timer is cancelled when the attempt ends first; one that
  fires late finds no matching attempt and does nothing.
- **The producer is its own process.** The driver first submitted the
  jobs itself and collected their notifications afterwards: every
  `gen_server.call`'s reply wait then scanned past all the notifications
  already queued in its mailbox, and 10,000 jobs took 21 s instead of
  0.7 s (BEST_PRACTICES.md, "A process that makes calls doesn't also
  collect a stream").
- **The pending queue is a fifo** (`fifo.gem`), not `remove_at(arr, 0)`,
  which shifts the whole backlog on every pop.

## The OTP rules it illustrates

- Let it crash: the worker has no `pcall`; the supervisor restarts it and
  the monitor tells the queue what happened (BEST_PRACTICES.md, "`pcall`
  at boundaries, not everywhere").
- A supervised child's `start` must not wait for its starter: a worker
  announces itself with a cast, never a call, because the supervisor
  starting it is often busy answering the queue's `start_child`.
- A server's `init` asks nothing of its supervisor: the queue looks the
  worker supervisor up by its registered name (started first) and
  monitors it.
- Restart intensity escalates: past `--max-restarts` in
  `--restart-window` the worker supervisor exits, and its supervisor
  restarts it without touching the queue (one_for_one).
- Request/reply carries a ref and a pin; a main loop has a catch-all; a
  timer for periodic work is `send_after`, not `after` in a busy loop.

## Known limits

- **Memory grows with the jobs run**: the queue keeps every job's record
  for `status`, and a small record costs about 1.2 KB in Gem (0.4 KB in
  Python, 0.14 KB in Elixir). 100,000 jobs peak at about 800 MB, against
  166 MB for the Python twin and 218 MB for Elixir. A real queue would
  expire finished records.
- **Big state means pauses.** The queue's records live in its loop.
  Arena resets promote what they keep, so most of them copy only the
  records made since the last one, but a full reset now and then copies
  all of them, and no other process runs meanwhile. With 20,000 jobs the
  longest reset takes about 0.09 s, with 100,000 0.25–0.5 s; a job whose
  attempt overlaps one can miss a 100 ms deadline it would otherwise meet
  (`--jobs 20000 --slow 0.1`: 0–5 healthy attempts killed per run,
  `--jobs 40000`: 14). The benchmark scenarios give deadlines room; see
  docs/OPTIMIZATIONS.md, "Full resets still copy all a loop keeps".
- **One system at a time**: the queue and the worker supervisor have
  fixed registered names.
- **No jitter in the backoff**, so retries of jobs that failed together
  run together; the schedule stays deterministic that way.
- **A slow attempt near its deadline is a race.** The schedule counts a
  slow attempt as failing only when `--slow-ms` reaches the deadline;
  keep it well below.
