# jobqueue

A job queue with a supervised worker pool, run under seeded fault injection. Workers crash, hang, run slow or get killed in the middle of a job, by the hundred per second. The queue retries with backoff, kills workers that outlive their deadline and dead-letters jobs that keep failing, and a supervisor restarts what dies. At the end of every run the program checks that no job was lost or finished twice, that every attempt failed exactly as the fault schedule said, and that the system is back at its baseline. `benchmarks/jobqueue/run.sh` runs the same design in Python asyncio and in Elixir/OTP.

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
wall_ms       247
throughput    40485 jobs/s
latency_ms    p50=0 p90=2 p99=11 max=73
tick_lag_ms   1
processes     baseline=4 peak=21 final=4
rss_kb        baseline=nil peak=nil final=nil stopped=nil
invariants    ok
```

`rss_kb` comes from `/proc/self/status`, so it is `nil` on macOS. Each crashed worker prints its crash report on stderr, as any Gem process does. The exit status is 0 when the invariants hold, 1 when one fails (the summary lists the first 20), and 2 for a bad command line.

## What it supports

A job is `{id, kind, payload, max_retries, deadline_ms}`. The kinds are `sum` (adds up `0 .. payload-1`), `text` (builds a string of `payload` pairs and returns its length) and `sleep` (1 to 3 ms). The queue:

- runs each job on a pool worker, at most `--workers` at once, started on demand under a `dynamic_supervisor` and retired after `--idle` ms without work;
- kills a worker whose attempt outlives the job's deadline (`--deadline`, 0 for none) and counts the attempt as failed;
- retries a failed attempt after `--backoff` ms, doubled for each retry up to `--backoff-max`, at most `--retries` times, then puts the job on the dead-letter list;
- notifies the submitter once per job, when it completes or is dead-lettered, and answers `status(id)`, `stats()`, `dead_letters()` and `report()`.

The driver (`driver.gem`) submits `--jobs` jobs of a seeded workload from a producer process, collects their notifications, waits for the pool to drain, checks the invariants and prints the summary. Faults come from a schedule that is a pure function of `--seed`, the job id and the attempt number (`schedule.gem`). With probability `--crash` an attempt raises, with `--hang` it never finishes (so its deadline kills it), with `--slow` it first sleeps `--slow-ms`, and with `--kill` it kills its own worker. A storm (`--storm N --storm-every MS --storm-bursts K`) kills N workers K times with reason `"storm"`. With `--max-restarts` low enough, the worker supervisor gives up and its own supervisor starts a new one. `./jobqueue --help` lists every option.

The invariants (`invariants.gem`):

- every job got exactly one notification and is either completed or dead-lettered, and the queue agrees with the notifications;
- every attempt ended as the schedule says, except those cut short by a storm or a supervisor shutdown, which are counted apart; a completed job's one successful attempt is its last, a dead job used all its retries, and a completed job's result is right;
- the queue's attempt and retry counters match the jobs' histories;
- after the drain the queue has no pending, busy or idle work and no workers, and the process count, the queue's monitors, links and mailbox, and the worker supervisor's links are what they were before the load.

Memory is reported, not checked: the queue keeps every job's record (that's what `status` answers from), so it grows with the number of jobs.

## Design

```
jobqueue_sup (supervisor, one_for_one, 20 restarts in 10 s)
├── jobqueue_workers (dynamic_supervisor, transient workers)
│   └── worker × up to --workers   receive loop: run, report, retire
└── jobqueue (gen_server)          the jobs, the pool, timers
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

The pool's size is the supervisor's child count. The queue asks the `dynamic_supervisor` for a worker when jobs are waiting and the pool has room, and counts it in `slots`. A worker that crashes keeps its slot: the supervisor restarts it (`restart: "transient"`), and the new process announces itself to the queue with a cast. A cast, not a call, because the supervisor starting it is often busy answering the queue's `start_child`. A worker idle for `--idle` ms asks the queue to retire, and the queue agrees only if it hasn't just sent that worker a job. When the worker supervisor itself gives up, the queue sees its `DOWN`, drops every slot, and looks up the new supervisor by its registered name.

The queue learns about a failure only from the worker's `DOWN`, never from the worker, so a crash, a kill and a storm are the same event with different reasons. Nothing in the worker catches errors.

Each attempt is identified by a ref, not by its job. A `done` from an attempt the queue already failed (the worker finished just as its deadline fired) doesn't match the busy entry's ref and is counted in `late_results`, so a job can't complete twice. Deadlines and backoff are `send_after` timers to the queue. A deadline timer is cancelled when the attempt ends first, and one that fires late finds no matching attempt and does nothing.

The producer is a separate process. A `gen_server.call` waiting for its reply scans past every message already in the caller's mailbox, so submitting from the process that collects the notifications would get slower with each notification it holds.

The pending queue is a FIFO (`fifo.gem`) rather than `remove_at(arr, 0)`, which would shift the whole backlog on every pop.

## Known limits

- Memory grows with the number of jobs run, because the queue keeps every job's record for `status`. A real queue would expire finished records.
- The queue's records live in its loop. Arena resets mostly copy only the records made since the last reset, but now and then a full reset copies all of them, and no other process runs meanwhile. A job whose attempt overlaps one can miss a deadline it would otherwise meet.
- Only one system can run at a time, since the queue and the worker supervisor have fixed registered names.
- The backoff has no jitter, so retries of jobs that failed together run together. This keeps the schedule deterministic.
- The schedule counts a slow attempt as failing only when `--slow-ms` reaches the deadline, so keep it well below the deadline.
