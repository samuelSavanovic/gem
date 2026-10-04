#!/usr/bin/env python3
"""examples/jobqueue in Python asyncio: the same design, options, seeded
workload and fault schedule, and summary format.

    python3 jobqueue.py [--jobs N] [--workers W] [--crash P] [--hang P] ...

A queue object owns the jobs and hands them to a bounded pool of worker
tasks. Workers belong to a WorkerSupervisor, which restarts a worker that
dies with anything but a normal exit (transient), and gives up, cancelling
its workers, when more than max_restarts restarts fall within the restart
window; a top-level supervisor then starts a new one. The queue learns of
a worker's death from the task's done callback (its monitor), kills a
worker whose attempt outlives the deadline, retries with exponential
backoff (call_later) and dead-letters a job out of retries.

Where Gem has processes, this has tasks: "processes" in the summary is
len(asyncio.all_tasks()), and the drain checks look at the queue's and the
supervisor's bookkeeping instead of process_info's monitors and links.
"""

import asyncio
import sys
import time
import traceback

MASK = 0xFFFFFFFF
KINDS = ["sum"] * 6 + ["text"] * 3 + ["sleep"]
HANG_S = 86400.0
TICK_MS = 20


def now_ms():
    return int(time.monotonic() * 1000)


# ─── schedule ──────────────────────────────────────────────────

def hash32(x):
    x = ((x >> 16) ^ x) * 73244475 & MASK
    x = ((x >> 16) ^ x) * 73244475 & MASK
    return (x >> 16) ^ x


def hash3(seed, ident, salt):
    h = hash32(seed & MASK)
    h = hash32(h ^ (ident & MASK))
    return hash32(h ^ (salt & MASK))


def unit(seed, ident, salt):
    return hash3(seed, ident, salt) / 4294967296.0


def make_job(opts, ident):
    deadline = opts["deadline_ms"] or None
    return {
        "id": ident,
        "kind": KINDS[hash3(opts["seed"], ident, 1) % 10],
        "payload": 100 + hash3(opts["seed"], ident, 2) % 900,
        "max_retries": opts["retries"],
        "deadline_ms": deadline,
    }


def fault(opts, ident, attempt):
    u = unit(opts["seed"], ident, 1000 + attempt)
    edge = opts["crash"]
    if u < edge:
        return "crash"
    edge = edge + opts["hang"]
    if u < edge:
        return "hang"
    edge = edge + opts["slow"]
    if u < edge:
        return "slow"
    edge = edge + opts["kill"]
    if u < edge:
        return "kill"
    return None


def expected_result(job):
    if job["kind"] == "sum":
        return job["payload"] * (job["payload"] - 1) // 2
    if job["kind"] == "text":
        return 2 * job["payload"]
    return job["payload"]


def expected_failure(opts, job, f):
    if f in ("crash", "kill"):
        return f
    if f == "hang":
        return "deadline"
    if f == "slow" and job["deadline_ms"] is not None and opts["slow_ms"] >= job["deadline_ms"]:
        return "deadline"
    return None


# ─── workers ───────────────────────────────────────────────────

class Killed(Exception):
    """A worker killed by a signal: its reason is the exit reason."""


class Worker:
    def __init__(self, sup, config):
        self.sup = sup
        self.config = config
        self.inbox = asyncio.Queue()
        self.kill_reason = None
        self.task = asyncio.create_task(self.serve())

    def send(self, msg):
        self.inbox.put_nowait(msg)

    def kill(self, reason):
        if not self.task.done() and self.kill_reason is None:
            self.kill_reason = reason
            self.task.cancel()

    def reason(self):
        """The exit reason of the finished task, as Gem would report it."""
        if self.kill_reason is not None:
            return self.kill_reason
        if self.task.cancelled():
            return "killed"
        e = self.task.exception()
        if e is None:
            return "normal"
        if isinstance(e, Killed):
            return str(e)
        return str(e)

    async def run_job(self, job):
        kind = job["kind"]
        if kind == "sum":
            total = 0
            for i in range(job["payload"]):
                total += i
            return total
        if kind == "text":
            return len("".join("ab" for _ in range(job["payload"])))
        if kind == "sleep":
            await asyncio.sleep((1 + job["payload"] % 3) / 1000)
            return job["payload"]
        raise RuntimeError(f"jobqueue: unknown job kind {kind}")

    async def inject(self, job, attempt):
        f = fault(self.config, job["id"], attempt)
        if f == "crash":
            raise RuntimeError(f"jobqueue: injected crash in job {job['id']}")
        if f == "hang":
            await asyncio.sleep(HANG_S)
        elif f == "slow":
            await asyncio.sleep(self.config["slow_ms"] / 1000)
        elif f == "kill":
            raise Killed("killed")

    async def serve(self):
        queue = self.config["queue"]
        queue.worker_up(self)
        idle_s = self.config["idle_ms"] / 1000
        while True:
            try:
                async with asyncio.timeout(idle_s):
                    msg = await self.inbox.get()
            except TimeoutError:
                if self.kill_reason is not None:
                    # The timeout swallowed the cancel a kill made.
                    raise asyncio.CancelledError()
                # The queue says no when it has just sent this worker a job.
                if queue.retire(self):
                    return
                continue
            job, attempt, ref = msg
            await self.inject(job, attempt)
            result = await self.run_job(job)
            queue.done(self, ref, result)


def report_crash(w):
    """Prints a dead worker's error and stack on stderr, as Gem's runtime
    and Elixir's logger do for a crashed process."""
    if w.kill_reason is None and not w.task.cancelled():
        e = w.task.exception()
        if e is not None and not isinstance(e, Killed):
            traceback.print_exception(e, file=sys.stderr)


class WorkerSupervisor:
    """A dynamic supervisor of transient workers with restart intensity."""

    def __init__(self, top, config, max_restarts, window_ms):
        self.top = top
        self.config = config
        self.max_restarts = max_restarts
        self.window_ms = window_ms
        self.restart_times = []
        self.children = []
        self.alive = True

    def start_child(self):
        if not self.alive:
            raise RuntimeError("supervisor exited")
        w = Worker(self, self.config)
        self.children.append(w)
        w.task.add_done_callback(lambda _t, w=w: self.child_down(w))
        return w

    def child_down(self, w):
        if not self.alive or w not in self.children:
            return
        reason = w.reason()
        self.children.remove(w)
        if reason in ("normal", "shutdown"):
            return
        report_crash(w)
        now = now_ms()
        self.restart_times = [t for t in self.restart_times if now - t <= self.window_ms]
        self.restart_times.append(now)
        if len(self.restart_times) > self.max_restarts:
            # Nothing else is restarted from here on.
            self.alive = False
            asyncio.create_task(self.give_up())
            return
        self.start_child()

    async def give_up(self):
        children = self.children
        self.children = []
        for w in reversed(children):
            w.kill("shutdown")
        await asyncio.gather(*(w.task for w in children), return_exceptions=True)
        self.top.workers_down(self)


class TopSupervisor:
    """Restarts the worker supervisor when it gives up (20 in 10 s)."""

    def __init__(self, config, max_restarts, window_ms):
        self.config = config
        self.args = (max_restarts, window_ms)
        self.restart_times = []
        self.queue = None
        self.workers = WorkerSupervisor(self, config, *self.args)

    def workers_down(self, sup):
        now = now_ms()
        self.restart_times = [t for t in self.restart_times if now - t <= 10000]
        self.restart_times.append(now)
        if len(self.restart_times) > 20:
            print("jobqueue: top supervisor gave up", file=sys.stderr)
            self.workers = None
        else:
            self.workers = WorkerSupervisor(self, self.config, *self.args)
        self.queue.sup_down(sup)


# ─── the queue ─────────────────────────────────────────────────

class Fifo:
    def __init__(self):
        self.items = []
        self.head = 0

    def __len__(self):
        return len(self.items) - self.head

    def put(self, x):
        self.items.append(x)

    def take(self):
        if self.head >= len(self.items):
            return None
        x = self.items[self.head]
        self.head += 1
        if self.head * 2 >= len(self.items):
            self.items = self.items[self.head:]
            self.head = 0
        return x


class Queue:
    def __init__(self, top, pool, backoff_ms, backoff_max_ms):
        self.top = top
        self.loop = asyncio.get_running_loop()
        self.pool = pool
        self.backoff_ms = backoff_ms
        self.backoff_max_ms = backoff_max_ms
        self.sup = top.workers
        self.slots = 0
        self.idle = []
        self.busy = {}
        self.killing = set()
        self.workers = set()
        self.pending = Fifo()
        self.jobs = {}
        self.dead = []
        self.stats = dict(submitted=0, completed=0, dead=0, attempts=0, retries=0,
                          late_results=0, worker_starts=0, sup_restarts=0)

    def monitor(self, w):
        w.task.add_done_callback(lambda _t: self.loop.call_soon(self.on_down, w))

    def backoff(self, attempt):
        d = self.backoff_ms
        for _ in range(1, attempt):
            d *= 2
            if d >= self.backoff_max_ms:
                return self.backoff_max_ms
        return d

    def notify(self, rec):
        rec["notify"].put_nowait({
            "id": rec["job"]["id"], "status": rec["status"], "attempts": len(rec["attempts"]),
            "result": rec["result"], "submitted_at": rec["submitted_at"],
            "finished_at": rec["finished_at"]})

    def start_attempt(self, w, ident):
        rec = self.jobs[ident]
        n = len(rec["attempts"]) + 1
        ref = object()
        timer = None
        if rec["job"]["deadline_ms"] is not None:
            timer = self.loop.call_later(rec["job"]["deadline_ms"] / 1000, self.on_deadline, w, ref)
        rec["status"] = "running"
        self.busy[w] = {"id": ident, "attempt": n, "ref": ref, "timer": timer}
        self.stats["attempts"] += 1
        w.send((rec["job"], n, ref))

    def dispatch(self):
        while len(self.pending) > 0 and self.idle:
            self.start_attempt(self.idle.pop(), self.pending.take())
        want = len(self.pending) - len(self.idle)
        while want > 0 and self.slots < self.pool and self.sup is not None:
            try:
                self.sup.start_child()
            except RuntimeError:
                return
            self.slots += 1
            self.stats["worker_starts"] += 1
            want -= 1

    def succeed(self, entry, result):
        rec = self.jobs[entry["id"]]
        rec["attempts"].append("ok")
        rec["status"] = "completed"
        rec["result"] = result
        rec["finished_at"] = now_ms()
        self.stats["completed"] += 1
        self.notify(rec)

    def fail(self, entry, reason):
        rec = self.jobs[entry["id"]]
        rec["attempts"].append(reason)
        rec["error"] = reason
        if entry["attempt"] <= rec["job"]["max_retries"]:
            rec["status"] = "retrying"
            self.stats["retries"] += 1
            self.loop.call_later(self.backoff(entry["attempt"]) / 1000, self.on_retry, entry["id"])
            return
        rec["status"] = "dead"
        rec["finished_at"] = now_ms()
        self.dead.append(entry["id"])
        self.stats["dead"] += 1
        self.notify(rec)

    # Messages from workers, timers and supervisors.

    def worker_up(self, w):
        self.workers.add(w)
        self.monitor(w)
        self.idle.append(w)
        self.dispatch()

    def done(self, w, ref, result):
        entry = self.busy.get(w)
        if entry is None or entry["ref"] is not ref:
            self.stats["late_results"] += 1
            return
        del self.busy[w]
        if entry["timer"] is not None:
            entry["timer"].cancel()
        self.succeed(entry, result)
        self.idle.append(w)
        self.dispatch()

    def retire(self, w):
        if w in self.idle:
            self.idle.remove(w)
            self.workers.discard(w)
            self.slots -= 1
            return True
        return False

    def on_deadline(self, w, ref):
        entry = self.busy.get(w)
        if entry is None or entry["ref"] is not ref:
            return
        del self.busy[w]
        self.killing.add(w)
        w.kill("deadline")
        self.fail(entry, "deadline")

    def on_retry(self, ident):
        self.jobs[ident]["status"] = "queued"
        self.pending.put(ident)
        self.dispatch()

    def on_down(self, w):
        self.workers.discard(w)
        entry = self.busy.pop(w, None)
        if entry is not None:
            if entry["timer"] is not None:
                entry["timer"].cancel()
            self.fail(entry, w.reason())
        elif w in self.killing:
            self.killing.discard(w)
        elif w in self.idle:
            self.idle.remove(w)

    def sup_down(self, _old):
        # The worker supervisor gave up; its workers are gone and their
        # done callbacks have run. The top supervisor started a new one.
        self.loop.call_soon(self._sup_down)

    def _sup_down(self):
        self.slots = 0
        self.idle = []
        self.stats["sup_restarts"] += 1
        self.sup = self.top.workers
        self.dispatch()

    # Calls.

    def submit(self, job, notify):
        if not isinstance(job.get("id"), int):
            return {"ok": False, "error": "a job needs an int id"}
        if job["id"] in self.jobs:
            return {"ok": False, "error": f"job {job['id']} was already submitted"}
        self.jobs[job["id"]] = {"job": job, "status": "queued", "attempts": [], "result": None,
                                "error": None, "notify": notify, "submitted_at": now_ms(),
                                "finished_at": None}
        self.stats["submitted"] += 1
        self.pending.put(job["id"])
        self.dispatch()
        return {"ok": True}

    def stats_now(self):
        st = dict(self.stats)
        st.update(pending=len(self.pending), busy=len(self.busy), idle=len(self.idle), slots=self.slots)
        return st

    def report(self):
        return [{"id": r["job"]["id"], "status": r["status"], "attempts": list(r["attempts"]),
                 "result": r["result"]} for r in self.jobs.values()]


# ─── the driver ────────────────────────────────────────────────

def memory_kb():
    out = {"rss": None, "hwm": None}
    try:
        with open("/proc/self/status") as f:
            for line in f:
                if line.startswith("VmRSS:"):
                    out["rss"] = int(line.split()[1])
                elif line.startswith("VmHWM:"):
                    out["hwm"] = int(line.split()[1])
    except OSError:
        pass
    return out


def sample(top, queue):
    return {
        "processes": len(asyncio.all_tasks()),
        "queue_monitors": len(queue.workers),
        "queue_links": 1,
        "queue_mailbox": 0,
        "workers_links": None if top.workers is None else 1 + len(top.workers.children),
        "rss": memory_kb()["rss"],
    }


def classify(o):
    if o in ("ok", "killed", "deadline", "storm", "shutdown", "noproc"):
        return o
    if o.startswith("jobqueue: injected crash"):
        return "crash"
    return "other"


EXTERNAL = {"storm", "shutdown", "noproc"}
INJECTED = {"crash": "crash", "kill": "killed", "deadline": "deadline"}


def failures(report):
    counts = dict(crash=0, deadline=0, killed=0, storm=0, shutdown=0, noproc=0, other=0)
    for st in report:
        for o in st["attempts"]:
            c = classify(o)
            if c != "ok":
                counts[c] += 1
    return counts


def check_attempts(opts, job, st, out):
    oks = 0
    for i, o in enumerate(st["attempts"]):
        c = classify(o)
        if c == "ok":
            oks += 1
        if c in EXTERNAL:
            continue
        want = expected_failure(opts, job, fault(opts, job["id"], i + 1))
        want_class = "ok" if want is None else INJECTED[want]
        if c != want_class:
            out.append(f"job {job['id']} attempt {i + 1}: {o}, the schedule says {want_class}")
    n = len(st["attempts"])
    if st["status"] == "completed":
        if oks != 1 or st["attempts"][-1] != "ok":
            out.append(f"job {job['id']}: completed with attempts {st['attempts']}")
        if st["result"] != expected_result(job):
            out.append(f"job {job['id']}: result {st['result']}, expected {expected_result(job)}")
    elif st["status"] == "dead":
        if oks != 0 or n != job["max_retries"] + 1:
            out.append(f"job {job['id']}: dead-lettered with attempts {st['attempts']}")
    else:
        out.append(f"job {job['id']}: still {st['status']}")


def check(opts, n, done, report, dead, stats, baseline, final):
    out = []
    by_id = {st["id"]: st for st in report}
    if len(report) != n:
        out.append(f"the queue holds {len(report)} jobs, {n} were submitted")
    attempts = 0
    dead_ids = set()
    for ident in range(1, n + 1):
        notes = done.get(ident)
        if notes is None:
            out.append(f"job {ident}: no job_done (lost)")
        elif len(notes) > 1:
            out.append(f"job {ident}: {len(notes)} job_done notifications")
        st = by_id.get(ident)
        if st is None:
            out.append(f"job {ident}: unknown to the queue")
            continue
        attempts += len(st["attempts"])
        if notes is not None and (notes[0]["status"] != st["status"] or notes[0]["attempts"] != len(st["attempts"])):
            out.append(f"job {ident}: notified {notes[0]['status']} after {notes[0]['attempts']} attempts, "
                       f"the queue says {st['status']} after {len(st['attempts'])}")
        if st["status"] == "dead":
            dead_ids.add(ident)
        check_attempts(opts, make_job(opts, ident), st, out)
    if len(dead) != len(dead_ids) or set(dead) != dead_ids:
        out.append(f"{len(dead)} dead letters, {len(dead_ids)} dead jobs")
    if stats["attempts"] != attempts:
        out.append(f"the queue counted {stats['attempts']} attempts, the jobs record {attempts}")
    if stats["retries"] != attempts - n:
        out.append(f"the queue counted {stats['retries']} retries, the jobs record {attempts - n}")
    for k in ("pending", "busy", "idle", "slots"):
        if stats[k] != 0:
            out.append(f"after the drain the queue has {k} = {stats[k]}")
    for k in ("processes", "queue_monitors", "queue_links", "queue_mailbox", "workers_links"):
        if final[k] != baseline[k]:
            out.append(f"{k}: {final[k]} after the drain, {baseline[k]} before the load")
    return out


async def storm(opts, top):
    for _ in range(opts["storm_bursts"]):
        await asyncio.sleep(opts["storm_every_ms"] / 1000)
        if top.workers is not None:
            for w in list(top.workers.children)[:opts["storm"]]:
                w.kill("storm")


async def produce(opts, queue, notify):
    for ident in range(1, opts["jobs"] + 1):
        r = queue.submit(make_job(opts, ident), notify)
        if not r["ok"]:
            notify.put_nowait({"rejected": r["error"]})
        # A call to a gen_server is a round trip: let the others run.
        await asyncio.sleep(0)


def percentile(xs, q):
    if not xs:
        return 0
    return xs[min(len(xs) - 1, int(len(xs) * q))]


async def run(opts):
    n = opts["jobs"]
    config = {k: opts[k] for k in ("seed", "crash", "hang", "slow", "kill", "slow_ms", "idle_ms")}
    top = TopSupervisor(config, opts["max_restarts"], opts["restart_window_ms"])
    queue = Queue(top, opts["pool"], opts["backoff_ms"], opts["backoff_max_ms"])
    config["queue"] = queue
    top.queue = queue
    baseline = sample(top, queue)
    mem0 = memory_kb()
    storm_task = asyncio.create_task(storm(opts, top)) if opts["storm"] > 0 else None
    notify = asyncio.Queue()
    t0 = now_ms()
    producer = asyncio.create_task(produce(opts, queue, notify))
    done = {}
    rejected = []
    peak = len(asyncio.all_tasks())
    lag = 0
    last_at = now_ms()
    give_up_at = t0 + opts["timeout_ms"]
    due = now_ms() + TICK_MS
    while len(done) < n and now_ms() < give_up_at:
        try:
            async with asyncio.timeout(max(0, due - now_ms()) / 1000):
                msg = await notify.get()
        except TimeoutError:
            now = now_ms()
            lag = max(lag, now - due)
            peak = max(peak, len(asyncio.all_tasks()))
            due = now + TICK_MS
            continue
        if "rejected" in msg:
            rejected.append(msg["rejected"])
            continue
        done.setdefault(msg["id"], []).append(
            {"status": msg["status"], "attempts": msg["attempts"],
             "latency": msg["finished_at"] - msg["submitted_at"]})
        last_at = now_ms()
        if now_ms() >= due:
            now = now_ms()
            lag = max(lag, now - due)
            peak = max(peak, len(asyncio.all_tasks()))
            due = now + TICK_MS
    wall = last_at - t0
    await producer
    if storm_task is not None:
        await storm_task
    until = now_ms() + opts["idle_ms"] * 5 + 2000
    stats = queue.stats_now()
    while (stats["slots"] > 0 or stats["busy"] > 0 or stats["pending"] > 0) and now_ms() <= until:
        await asyncio.sleep(TICK_MS / 1000)
        stats = queue.stats_now()
    final = sample(top, queue)
    mem1 = memory_kb()
    report = queue.report()
    violations = check(opts, n, done, report, list(queue.dead), stats, baseline, final)
    violations += [f"rejected: {e}" for e in rejected]
    queue = None
    top = None
    report_failures = failures(report)
    report = None
    stopped = memory_kb()
    lat = sorted(notes[0]["latency"] for notes in done.values())
    return {
        "jobs": n, "completed": stats["completed"], "dead": stats["dead"],
        "attempts": stats["attempts"], "retries": stats["retries"], "failures": report_failures,
        "late_results": stats["late_results"], "sup_restarts": stats["sup_restarts"],
        "wall_ms": wall, "throughput": n * 1000 // wall if wall > 0 else 0,
        "latency": [percentile(lat, q) for q in (0.5, 0.9, 0.99, 1.0)],
        "lag": lag,
        "processes": (baseline["processes"], peak, final["processes"]),
        "rss": (mem0["rss"], mem1["hwm"], mem1["rss"], stopped["rss"]),
        "violations": violations,
    }


def show(v):
    return "nil" if v is None else str(v)


def format_summary(s):
    f = s["failures"]
    lat = s["latency"]
    p = s["processes"]
    r = [show(x) for x in s["rss"]]
    lines = [
        f"jobs          {s['jobs']}",
        f"completed     {s['completed']}",
        f"dead_letters  {s['dead']}",
        f"attempts      {s['attempts']}",
        f"retries       {s['retries']}",
        f"failures      crash={f['crash']} killed={f['killed']} deadline={f['deadline']} storm={f['storm']} "
        f"shutdown={f['shutdown']} noproc={f['noproc']} other={f['other']}",
        f"late_results  {s['late_results']}",
        f"sup_restarts  {s['sup_restarts']}",
        f"wall_ms       {s['wall_ms']}",
        f"throughput    {s['throughput']} jobs/s",
        f"latency_ms    p50={lat[0]} p90={lat[1]} p99={lat[2]} max={lat[3]}",
        f"tick_lag_ms   {s['lag']}",
        f"processes     baseline={p[0]} peak={p[1]} final={p[2]}",
        f"rss_kb        baseline={r[0]} peak={r[1]} final={r[2]} stopped={r[3]}",
    ]
    if not s["violations"]:
        lines.append("invariants    ok")
    else:
        lines.append(f"invariants    FAILED ({len(s['violations'])})")
        lines += [f"  {v}" for v in s["violations"][:20]]
    return "\n".join(lines)


# ─── options ───────────────────────────────────────────────────

INTS = {
    "--jobs": ("jobs", 0), "--workers": ("pool", 1), "--seed": ("seed", 0),
    "--slow-ms": ("slow_ms", 0), "--retries": ("retries", 0), "--deadline": ("deadline_ms", 0),
    "--backoff": ("backoff_ms", 1), "--backoff-max": ("backoff_max_ms", 1), "--idle": ("idle_ms", 1),
    "--max-restarts": ("max_restarts", 0), "--restart-window": ("restart_window_ms", 1),
    "--storm": ("storm", 0), "--storm-every": ("storm_every_ms", 1), "--storm-bursts": ("storm_bursts", 0),
    "--timeout": ("timeout_ms", 1),
}
FLOATS = {"--crash": "crash", "--hang": "hang", "--slow": "slow", "--kill": "kill"}


def defaults():
    return dict(jobs=10000, pool=16, seed=1, crash=0.0, hang=0.0, slow=0.0, kill=0.0,
                slow_ms=20, retries=3, deadline_ms=100, backoff_ms=10, backoff_max_ms=1000,
                idle_ms=200, max_restarts=1000, restart_window_ms=1000,
                storm=0, storm_every_ms=100, storm_bursts=10, timeout_ms=120000)


def parse(args):
    opts = defaults()
    i = 0
    while i < len(args):
        flag = args[i]
        if flag not in INTS and flag not in FLOATS:
            raise SystemExit(f"unknown option {flag}")
        if i + 1 >= len(args):
            raise SystemExit(f"{flag} needs a value")
        text = args[i + 1]
        if flag in INTS:
            key, least = INTS[flag]
            try:
                v = int(text)
            except ValueError:
                v = least - 1
            if v < least:
                raise SystemExit(f"{flag} expects an int >= {least}, got \"{text}\"")
            opts[key] = v
        else:
            try:
                v = float(text)
            except ValueError:
                v = -1.0
            if not 0.0 <= v <= 1.0:
                raise SystemExit(f"{flag} expects a probability from 0 to 1, got \"{text}\"")
            opts[FLOATS[flag]] = v
        i += 2
    if opts["crash"] + opts["hang"] + opts["slow"] + opts["kill"] > 1.0:
        raise SystemExit("--crash, --hang, --slow and --kill add up to more than 1")
    return opts


def main():
    opts = parse(sys.argv[1:])
    summary = asyncio.run(run(opts))
    print(format_summary(summary))
    sys.exit(1 if summary["violations"] else 0)


if __name__ == "__main__":
    main()
