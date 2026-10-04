# examples/jobqueue in Elixir/OTP: the true control. The same design,
# options, seeded workload and fault schedule, and summary format.
#
#   elixir jobqueue.exs [--jobs N] [--workers W] [--crash P] [--hang P] ...
#
# The tree is the Gem one: a one_for_one Supervisor (20 restarts in 10 s)
# over a DynamicSupervisor of transient worker GenServers and the queue
# GenServer. A worker announces itself with a cast, runs the jobs the
# queue sends it, and asks the queue to let it stop after idling. The
# queue monitors its workers, kills a worker whose attempt outlives the
# deadline (Process.exit(pid, :deadline)), retries with exponential
# backoff (Process.send_after) and dead-letters a job out of retries.
# DynamicSupervisor's max_seconds is in seconds: --restart-window is
# rounded down to whole seconds (at least 1).

Logger.configure_backend(:console, device: :standard_error)

defmodule JQ.Schedule do
  import Bitwise

  @mask 0xFFFFFFFF
  @kinds {"sum", "sum", "sum", "sum", "sum", "sum", "text", "text", "text", "sleep"}

  def hash32(x) do
    x = band(bxor(x >>> 16, x) * 73_244_475, @mask)
    x = band(bxor(x >>> 16, x) * 73_244_475, @mask)
    bxor(x >>> 16, x)
  end

  def hash(seed, id, salt) do
    h = hash32(band(seed, @mask))
    h = hash32(bxor(h, band(id, @mask)))
    hash32(bxor(h, band(salt, @mask)))
  end

  def unit(seed, id, salt), do: hash(seed, id, salt) / 4_294_967_296.0

  def job(opts, id) do
    %{
      id: id,
      kind: elem(@kinds, rem(hash(opts.seed, id, 1), 10)),
      payload: 100 + rem(hash(opts.seed, id, 2), 900),
      max_retries: opts.retries,
      deadline_ms: if(opts.deadline_ms == 0, do: nil, else: opts.deadline_ms)
    }
  end

  def fault(opts, id, attempt) do
    u = unit(opts.seed, id, 1000 + attempt)
    e1 = opts.crash
    e2 = e1 + opts.hang
    e3 = e2 + opts.slow
    e4 = e3 + opts.kill

    cond do
      u < e1 -> "crash"
      u < e2 -> "hang"
      u < e3 -> "slow"
      u < e4 -> "kill"
      true -> nil
    end
  end

  def expected_result(%{kind: "sum", payload: p}), do: div(p * (p - 1), 2)
  def expected_result(%{kind: "text", payload: p}), do: 2 * p
  def expected_result(%{payload: p}), do: p

  def expected_failure(_opts, _job, f) when f in ["crash", "kill"], do: f
  def expected_failure(_opts, _job, "hang"), do: "deadline"

  def expected_failure(opts, job, "slow") do
    if job.deadline_ms != nil and opts.slow_ms >= job.deadline_ms, do: "deadline", else: nil
  end

  def expected_failure(_opts, _job, _), do: nil
end

defmodule JQ.Worker do
  use GenServer, restart: :transient

  @hang_ms 86_400_000

  def start_link(config), do: GenServer.start_link(__MODULE__, config)

  @impl true
  def init(config) do
    GenServer.cast(JQ.Queue, {:worker_up, self()})
    {:ok, config, config.idle_ms}
  end

  @impl true
  def handle_info({:run, job, attempt, ref}, config) do
    inject(config, job, attempt)
    result = run_job(job)
    GenServer.cast(JQ.Queue, {:done, self(), ref, result})
    {:noreply, config, config.idle_ms}
  end

  def handle_info(:timeout, config) do
    # The queue says no when it has just sent this worker a job.
    if GenServer.call(JQ.Queue, {:retire, self()}) do
      {:stop, :normal, config}
    else
      {:noreply, config, config.idle_ms}
    end
  end

  def handle_info(_other, config), do: {:noreply, config, config.idle_ms}

  def run_job(%{kind: "sum", payload: p}), do: Enum.reduce(0..(p - 1), 0, &+/2)
  def run_job(%{kind: "text", payload: p}), do: byte_size(String.duplicate("ab", p))

  def run_job(%{kind: "sleep", payload: p}) do
    Process.sleep(1 + rem(p, 3))
    p
  end

  def run_job(%{kind: kind}), do: raise("jobqueue: unknown job kind #{kind}")

  defp inject(config, job, attempt) do
    case JQ.Schedule.fault(config, job.id, attempt) do
      "crash" -> raise "jobqueue: injected crash in job #{job.id}"
      "hang" -> Process.sleep(@hang_ms)
      "slow" -> Process.sleep(config.slow_ms)
      "kill" -> Process.exit(self(), :kill)
      nil -> nil
    end
  end
end

defmodule JQ.Queue do
  use GenServer

  @resup_ms 10

  def start_link(opts), do: GenServer.start_link(__MODULE__, opts, name: JQ.Queue)

  @impl true
  def init(opts) do
    s = %{
      worker_config: opts.worker_config,
      pool: opts.pool,
      backoff_ms: opts.backoff_ms,
      backoff_max_ms: opts.backoff_max_ms,
      sup: nil,
      slots: 0,
      idle: [],
      busy: %{},
      killing: MapSet.new(),
      pending: :queue.new(),
      jobs: %{},
      dead: [],
      stats: %{submitted: 0, completed: 0, dead: 0, attempts: 0, retries: 0,
               late_results: 0, worker_starts: 0, sup_restarts: 0}
    }

    {:ok, find_sup(s)}
  end

  defp find_sup(s) do
    case Process.whereis(JQ.Workers) do
      nil ->
        Process.send_after(self(), :resup, @resup_ms)
        s

      pid ->
        Process.monitor(pid)
        %{s | sup: pid}
    end
  end

  defp bump(s, key, by \\ 1), do: put_in(s, [:stats, key], s.stats[key] + by)

  defp backoff(s, attempt) do
    min(s.backoff_max_ms, s.backoff_ms * Integer.pow(2, attempt - 1))
  end

  defp notify(rec) do
    send(rec.notify, {:job_done, rec.job.id, rec.status, length(rec.attempts), rec.result,
                      rec.submitted_at, rec.finished_at})
  end

  defp start_attempt(s, pid, id) do
    rec = s.jobs[id]
    n = length(rec.attempts) + 1
    ref = make_ref()

    timer =
      if rec.job.deadline_ms != nil do
        Process.send_after(self(), {:deadline, pid, ref}, rec.job.deadline_ms)
      end

    send(pid, {:run, rec.job, n, ref})
    entry = %{id: id, attempt: n, ref: ref, timer: timer}

    %{s | busy: Map.put(s.busy, pid, entry), jobs: Map.put(s.jobs, id, %{rec | status: "running"})}
    |> bump(:attempts)
  end

  defp dispatch(s) do
    case {s.idle, :queue.out(s.pending)} do
      {[pid | idle], {{:value, id}, pending}} ->
        dispatch(start_attempt(%{s | idle: idle, pending: pending}, pid, id))

      _ ->
        start_workers(s, :queue.len(s.pending) - length(s.idle))
    end
  end

  defp start_workers(s, want) do
    if want > 0 and s.slots < s.pool and s.sup != nil do
      case safe_start_child(s.sup, s.worker_config) do
        :ok -> start_workers(bump(%{s | slots: s.slots + 1}, :worker_starts), want - 1)
        # The supervisor is going down: its DOWN brings the queue back here.
        :error -> s
      end
    else
      s
    end
  end

  defp safe_start_child(sup, config) do
    case DynamicSupervisor.start_child(sup, {JQ.Worker, config}) do
      {:ok, _pid} -> :ok
      _ -> :error
    end
  catch
    :exit, _ -> :error
  end

  defp succeed(s, entry, result) do
    rec = %{s.jobs[entry.id] | status: "completed", result: result,
            finished_at: now_ms()}
    rec = %{rec | attempts: rec.attempts ++ ["ok"]}
    notify(rec)
    bump(%{s | jobs: Map.put(s.jobs, entry.id, rec)}, :completed)
  end

  defp fail(s, entry, reason) do
    rec = s.jobs[entry.id]
    rec = %{rec | attempts: rec.attempts ++ [reason], error: reason}

    if entry.attempt <= rec.job.max_retries do
      Process.send_after(self(), {:retry, entry.id}, backoff(s, entry.attempt))
      bump(%{s | jobs: Map.put(s.jobs, entry.id, %{rec | status: "retrying"})}, :retries)
    else
      rec = %{rec | status: "dead", finished_at: now_ms()}
      notify(rec)
      s = %{s | jobs: Map.put(s.jobs, entry.id, rec), dead: [entry.id | s.dead]}
      bump(s, :dead)
    end
  end

  defp cancel(nil), do: nil
  defp cancel(timer), do: Process.cancel_timer(timer)

  defp reason_text({%{message: m}, _stack}), do: m
  defp reason_text(:killed), do: "killed"
  defp reason_text(r) when is_atom(r), do: Atom.to_string(r)
  defp reason_text(r), do: inspect(r)

  defp check_job(%{id: id} = job) when is_integer(id) do
    cond do
      not is_binary(job[:kind]) -> "job #{id}: kind must be a string"
      not is_integer(job[:max_retries]) or job.max_retries < 0 -> "job #{id}: max_retries must be an int >= 0"
      job[:deadline_ms] != nil and (not is_integer(job.deadline_ms) or job.deadline_ms <= 0) ->
        "job #{id}: deadline_ms must be a positive int or nil"
      true -> nil
    end
  end

  defp check_job(_), do: "a job needs an int id"

  defp submit(s, job, notify) do
    case check_job(job) do
      nil ->
        if Map.has_key?(s.jobs, job.id) do
          {{:error, "job #{job.id} was already submitted"}, s}
        else
          rec = %{job: job, status: "queued", attempts: [], result: nil, error: nil,
                  notify: notify, submitted_at: now_ms(), finished_at: nil}
          s = %{s | jobs: Map.put(s.jobs, job.id, rec), pending: :queue.in(job.id, s.pending)}
          {:ok, dispatch(bump(s, :submitted))}
        end

      problem ->
        {{:error, problem}, s}
    end
  end

  defp status_of(rec) do
    %{id: rec.job.id, status: rec.status, attempts: rec.attempts, result: rec.result}
  end

  @impl true
  def handle_call({:submit, job, notify}, _from, s) do
    {reply, s} = submit(s, job, notify)
    {:reply, reply, s}
  end

  def handle_call({:retire, pid}, _from, s) do
    if pid in s.idle do
      {:reply, true, %{s | idle: List.delete(s.idle, pid), slots: s.slots - 1}}
    else
      {:reply, false, s}
    end
  end

  def handle_call(:stats, _from, s) do
    st = Map.merge(s.stats, %{pending: :queue.len(s.pending), busy: map_size(s.busy),
                              idle: length(s.idle), slots: s.slots})
    {:reply, st, s}
  end

  def handle_call(:dead_letters, _from, s), do: {:reply, Enum.reverse(s.dead), s}

  def handle_call(:report, _from, s) do
    {:reply, Enum.map(Map.values(s.jobs), &status_of/1), s}
  end

  @impl true
  def handle_cast({:worker_up, pid}, s) do
    Process.monitor(pid)
    {:noreply, dispatch(%{s | idle: [pid | s.idle]})}
  end

  def handle_cast({:done, pid, ref, result}, s) do
    case s.busy do
      %{^pid => %{ref: ^ref} = entry} ->
        cancel(entry.timer)
        s = succeed(%{s | busy: Map.delete(s.busy, pid)}, entry, result)
        {:noreply, dispatch(%{s | idle: [pid | s.idle]})}

      _ ->
        {:noreply, bump(s, :late_results)}
    end
  end

  @impl true
  def handle_info({:DOWN, _mref, :process, pid, _reason}, %{sup: pid} = s) do
    # The worker supervisor gave up; its children are gone, and their DOWNs
    # came first. Its own supervisor starts a new one.
    s = bump(%{s | sup: nil, slots: 0, idle: []}, :sup_restarts)
    {:noreply, dispatch(find_sup(s))}
  end

  def handle_info({:DOWN, _mref, :process, pid, reason}, s) do
    cond do
      Map.has_key?(s.busy, pid) ->
        entry = s.busy[pid]
        cancel(entry.timer)
        {:noreply, fail(%{s | busy: Map.delete(s.busy, pid)}, entry, reason_text(reason))}

      MapSet.member?(s.killing, pid) ->
        {:noreply, %{s | killing: MapSet.delete(s.killing, pid)}}

      true ->
        {:noreply, %{s | idle: List.delete(s.idle, pid)}}
    end
  end

  def handle_info({:deadline, pid, ref}, s) do
    case s.busy do
      %{^pid => %{ref: ^ref} = entry} ->
        Process.exit(pid, :deadline)
        s = %{s | busy: Map.delete(s.busy, pid), killing: MapSet.put(s.killing, pid)}
        {:noreply, fail(s, entry, "deadline")}

      _ ->
        {:noreply, s}
    end
  end

  def handle_info({:retry, id}, s) do
    rec = %{s.jobs[id] | status: "queued"}
    s = %{s | jobs: Map.put(s.jobs, id, rec), pending: :queue.in(id, s.pending)}
    {:noreply, dispatch(s)}
  end

  def handle_info(:resup, s), do: {:noreply, dispatch(find_sup(s))}
  def handle_info(_other, s), do: {:noreply, s}

  def now_ms, do: System.monotonic_time(:millisecond)
end

defmodule JQ.Check do
  alias JQ.Schedule

  @external ["storm", "shutdown", "noproc"]
  @injected %{"crash" => "crash", "kill" => "killed", "deadline" => "deadline"}

  def classify(o) when o in ["ok", "killed", "deadline", "storm", "shutdown", "noproc"], do: o

  def classify(o) do
    if String.starts_with?(o, "jobqueue: injected crash"), do: "crash", else: "other"
  end

  def failures(report) do
    zero = %{"crash" => 0, "deadline" => 0, "killed" => 0, "storm" => 0, "shutdown" => 0,
             "noproc" => 0, "other" => 0}

    for st <- report, o <- st.attempts, classify(o) != "ok", reduce: zero do
      acc -> Map.update!(acc, classify(o), &(&1 + 1))
    end
  end

  defp check_attempts(opts, job, st) do
    wrong =
      for {o, i} <- Enum.with_index(st.attempts, 1),
          c = classify(o),
          c not in @external,
          want = Schedule.expected_failure(opts, job, Schedule.fault(opts, job.id, i)),
          want_class = if(want == nil, do: "ok", else: @injected[want]),
          c != want_class do
        "job #{job.id} attempt #{i}: #{o}, the schedule says #{want_class}"
      end

    oks = Enum.count(st.attempts, &(&1 == "ok"))
    n = length(st.attempts)

    final =
      case st.status do
        "completed" ->
          a = if oks != 1 or List.last(st.attempts) != "ok",
                do: ["job #{job.id}: completed with attempts #{inspect(st.attempts)}"], else: []
          b = if st.result != Schedule.expected_result(job),
                do: ["job #{job.id}: result #{st.result}, expected #{Schedule.expected_result(job)}"], else: []
          a ++ b

        "dead" ->
          if oks != 0 or n != job.max_retries + 1,
            do: ["job #{job.id}: dead-lettered with attempts #{inspect(st.attempts)}"], else: []

        other ->
          ["job #{job.id}: still #{other}"]
      end

    wrong ++ final
  end

  def check(opts, n, done, report, dead, stats, baseline, final) do
    by_id = Map.new(report, &{&1.id, &1})
    out = if length(report) != n, do: ["the queue holds #{length(report)} jobs, #{n} were submitted"], else: []

    {out, attempts, dead_ids} =
      Enum.reduce(1..n//1, {out, 0, MapSet.new()}, fn id, {out, attempts, dead_ids} ->
        notes = Map.get(done, id)
        out =
          cond do
            notes == nil -> out ++ ["job #{id}: no job_done (lost)"]
            length(notes) > 1 -> out ++ ["job #{id}: #{length(notes)} job_done notifications"]
            true -> out
          end

        case Map.get(by_id, id) do
          nil ->
            {out ++ ["job #{id}: unknown to the queue"], attempts, dead_ids}

          st ->
            na = length(st.attempts)
            out =
              if notes != nil and (hd(notes).status != st.status or hd(notes).attempts != na),
                do: out ++ ["job #{id}: notified #{hd(notes).status} after #{hd(notes).attempts} attempts, the queue says #{st.status} after #{na}"],
                else: out
            dead_ids = if st.status == "dead", do: MapSet.put(dead_ids, id), else: dead_ids
            {out ++ check_attempts(opts, Schedule.job(opts, id), st), attempts + na, dead_ids}
        end
      end)

    out = if MapSet.new(dead) != dead_ids or length(dead) != MapSet.size(dead_ids),
            do: out ++ ["#{length(dead)} dead letters, #{MapSet.size(dead_ids)} dead jobs"], else: out
    out = if stats.attempts != attempts,
            do: out ++ ["the queue counted #{stats.attempts} attempts, the jobs record #{attempts}"], else: out
    out = if stats.retries != attempts - n,
            do: out ++ ["the queue counted #{stats.retries} retries, the jobs record #{attempts - n}"], else: out
    out = out ++ for k <- [:pending, :busy, :idle, :slots], stats[k] != 0,
                     do: "after the drain the queue has #{k} = #{stats[k]}"
    out ++ for k <- [:processes, :queue_monitors, :queue_links, :queue_mailbox, :workers_links],
               final[k] != baseline[k],
               do: "#{k}: #{show(final[k])} after the drain, #{show(baseline[k])} before the load"
  end

  def show(nil), do: "nil"
  def show(v), do: to_string(v)
end

defmodule JQ.Driver do
  @tick_ms 20

  def now_ms, do: System.monotonic_time(:millisecond)

  def memory_kb do
    case File.read("/proc/self/status") do
      {:ok, text} ->
        lines = String.split(text, "\n")
        %{rss: field(lines, "VmRSS:"), hwm: field(lines, "VmHWM:")}

      _ ->
        %{rss: nil, hwm: nil}
    end
  end

  defp field(lines, name) do
    case Enum.find(lines, &String.starts_with?(&1, name)) do
      nil -> nil
      line -> line |> String.split() |> Enum.at(1) |> String.to_integer()
    end
  end

  def sample do
    q = Process.whereis(JQ.Queue)
    w = Process.whereis(JQ.Workers)
    qi = Process.info(q, [:monitored_by, :links, :message_queue_len])
    wl = if w, do: length(Process.info(w, :links) |> elem(1)), else: nil

    %{
      processes: length(Process.list()),
      queue_monitors: length(qi[:monitored_by]),
      queue_links: length(qi[:links]),
      queue_mailbox: qi[:message_queue_len],
      workers_links: wl,
      rss: memory_kb().rss
    }
  end

  def storm(opts) do
    for _ <- 1..opts.storm_bursts//1 do
      Process.sleep(opts.storm_every_ms)

      children =
        try do
          DynamicSupervisor.which_children(JQ.Workers)
        catch
          :exit, _ -> []
        end

      for {_, pid, _, _} <- Enum.take(children, opts.storm), is_pid(pid), do: Process.exit(pid, :storm)
    end
  end

  def start(opts) do
    worker_config = Map.take(opts, [:seed, :crash, :hang, :slow, :kill, :slow_ms, :idle_ms])
    queue_opts = %{worker_config: worker_config, pool: opts.pool, backoff_ms: opts.backoff_ms,
                   backoff_max_ms: opts.backoff_max_ms}
    window_s = max(1, div(opts.restart_window_ms, 1000))

    children = [
      %{
        id: :workers,
        type: :supervisor,
        start: {DynamicSupervisor, :start_link,
                [[strategy: :one_for_one, name: JQ.Workers, max_restarts: opts.max_restarts,
                  max_seconds: window_s]]}
      },
      %{id: :queue, start: {JQ.Queue, :start_link, [queue_opts]}}
    ]

    {:ok, sup} = Supervisor.start_link(children, strategy: :one_for_one, max_restarts: 20,
                                       max_seconds: 10, name: JQ.Sup)
    Process.unlink(sup)
    sup
  end

  defp collect(n, give_up_at, acc) do
    if map_size(acc.done) >= n or now_ms() >= give_up_at do
      acc
    else
      receive do
        {:job_done, id, status, attempts, _result, t0, t1} ->
          note = %{status: status, attempts: attempts, latency: t1 - t0}
          done = Map.update(acc.done, id, [note], &(&1 ++ [note]))
          collect(n, give_up_at, %{acc | done: done, last_at: now_ms()})

        {:rejected, e} ->
          collect(n, give_up_at, %{acc | rejected: [e | acc.rejected]})

        :tick ->
          now = now_ms()
          Process.send_after(self(), :tick, @tick_ms)
          acc = %{acc | lag: max(acc.lag, now - acc.due), peak: max(acc.peak, length(Process.list())),
                  due: now + @tick_ms}
          collect(n, give_up_at, acc)
      after
        max(0, give_up_at - now_ms()) -> acc
      end
    end
  end

  defp await_drain(until) do
    st = GenServer.call(JQ.Queue, :stats)

    {:message_queue_len, queued} = Process.info(Process.whereis(JQ.Queue), :message_queue_len)

    if (st.slots > 0 or st.busy > 0 or st.pending > 0 or queued > 0) and now_ms() <= until do
      Process.sleep(@tick_ms)
      await_drain(until)
    else
      st
    end
  end

  defp percentile([], _q, _n), do: 0
  defp percentile(xs, q, n), do: Enum.at(xs, min(n - 1, trunc(n * q)))

  def run(opts) do
    n = opts.jobs
    sup = start(opts)
    baseline = sample()
    mem0 = memory_kb()
    storm = if opts.storm > 0, do: spawn_monitor(fn -> storm(opts) end)
    collector = self()
    t0 = now_ms()

    spawn(fn ->
      for id <- 1..n//1 do
        case GenServer.call(JQ.Queue, {:submit, JQ.Schedule.job(opts, id), collector}) do
          :ok -> nil
          {:error, e} -> send(collector, {:rejected, e})
        end
      end
    end)

    Process.send_after(self(), :tick, @tick_ms)
    c = collect(n, t0 + opts.timeout_ms,
                %{done: %{}, rejected: [], peak: length(Process.list()), lag: 0,
                  last_at: now_ms(), due: now_ms() + @tick_ms})
    wall = c.last_at - t0

    case storm do
      {pid, mref} -> receive do: ({:DOWN, ^mref, :process, ^pid, _} -> nil)
      nil -> nil
    end

    stats = await_drain(now_ms() + opts.idle_ms * 5 + 2000)
    final = sample()
    mem1 = memory_kb()
    report = GenServer.call(JQ.Queue, :report, 60_000)
    dead = GenServer.call(JQ.Queue, :dead_letters)
    violations = JQ.Check.check(opts, n, c.done, report, dead, stats, baseline, final)
    violations = violations ++ Enum.map(Enum.reverse(c.rejected), &"rejected: #{&1}")
    failures = JQ.Check.failures(report)
    Supervisor.stop(sup)
    :erlang.garbage_collect()
    stopped = memory_kb()
    lat = c.done |> Map.values() |> Enum.map(&hd(&1).latency) |> Enum.sort()
    nl = length(lat)

    %{
      jobs: n, completed: stats.completed, dead: stats.dead, attempts: stats.attempts,
      retries: stats.retries, failures: failures, late_results: stats.late_results,
      sup_restarts: stats.sup_restarts, wall_ms: wall,
      throughput: if(wall > 0, do: div(n * 1000, wall), else: 0),
      latency: Enum.map([0.5, 0.9, 0.99, 1.0], &percentile(lat, &1, nl)),
      lag: c.lag,
      processes: {baseline.processes, c.peak, final.processes},
      rss: {mem0.rss, mem1.hwm, mem1.rss, stopped.rss},
      violations: violations
    }
  end

  def format(s) do
    f = s.failures
    [p50, p90, p99, pmax] = s.latency
    {pb, pp, pf} = s.processes
    {r0, r1, r2, r3} = s.rss
    show = &JQ.Check.show/1

    lines = [
      "jobs          #{s.jobs}",
      "completed     #{s.completed}",
      "dead_letters  #{s.dead}",
      "attempts      #{s.attempts}",
      "retries       #{s.retries}",
      "failures      crash=#{f["crash"]} killed=#{f["killed"]} deadline=#{f["deadline"]} storm=#{f["storm"]} shutdown=#{f["shutdown"]} noproc=#{f["noproc"]} other=#{f["other"]}",
      "late_results  #{s.late_results}",
      "sup_restarts  #{s.sup_restarts}",
      "wall_ms       #{s.wall_ms}",
      "throughput    #{s.throughput} jobs/s",
      "latency_ms    p50=#{p50} p90=#{p90} p99=#{p99} max=#{pmax}",
      "tick_lag_ms   #{s.lag}",
      "processes     baseline=#{pb} peak=#{pp} final=#{pf}",
      "rss_kb        baseline=#{show.(r0)} peak=#{show.(r1)} final=#{show.(r2)} stopped=#{show.(r3)}"
    ]

    tail =
      case s.violations do
        [] -> ["invariants    ok"]
        v -> ["invariants    FAILED (#{length(v)})" | Enum.map(Enum.take(v, 20), &"  #{&1}")]
      end

    Enum.join(lines ++ tail, "\n")
  end
end

defmodule JQ.Options do
  @ints %{
    "--jobs" => {:jobs, 0}, "--workers" => {:pool, 1}, "--seed" => {:seed, 0},
    "--slow-ms" => {:slow_ms, 0}, "--retries" => {:retries, 0}, "--deadline" => {:deadline_ms, 0},
    "--backoff" => {:backoff_ms, 1}, "--backoff-max" => {:backoff_max_ms, 1}, "--idle" => {:idle_ms, 1},
    "--max-restarts" => {:max_restarts, 0}, "--restart-window" => {:restart_window_ms, 1},
    "--storm" => {:storm, 0}, "--storm-every" => {:storm_every_ms, 1},
    "--storm-bursts" => {:storm_bursts, 0}, "--timeout" => {:timeout_ms, 1}
  }
  @floats %{"--crash" => :crash, "--hang" => :hang, "--slow" => :slow, "--kill" => :kill}

  def defaults do
    %{jobs: 10000, pool: 16, seed: 1, crash: 0.0, hang: 0.0, slow: 0.0, kill: 0.0,
      slow_ms: 20, retries: 3, deadline_ms: 100, backoff_ms: 10, backoff_max_ms: 1000,
      idle_ms: 200, max_restarts: 1000, restart_window_ms: 1000,
      storm: 0, storm_every_ms: 100, storm_bursts: 10, timeout_ms: 120_000}
  end

  def parse(args), do: parse(args, defaults())

  defp parse([], opts) do
    if opts.crash + opts.hang + opts.slow + opts.kill > 1.0,
      do: fail("--crash, --hang, --slow and --kill add up to more than 1"),
      else: opts
  end

  defp parse([flag], _), do: fail("#{flag} needs a value")

  defp parse([flag, text | rest], opts) do
    cond do
      Map.has_key?(@ints, flag) ->
        {key, least} = @ints[flag]
        case Integer.parse(text) do
          {v, ""} when v >= least -> parse(rest, Map.put(opts, key, v))
          _ -> fail("#{flag} expects an int >= #{least}, got \"#{text}\"")
        end

      Map.has_key?(@floats, flag) ->
        case Float.parse(text) do
          {v, ""} when v >= 0.0 and v <= 1.0 -> parse(rest, Map.put(opts, @floats[flag], v))
          _ -> fail("#{flag} expects a probability from 0 to 1, got \"#{text}\"")
        end

      true ->
        fail("unknown option #{flag}")
    end
  end

  defp fail(msg) do
    IO.puts(:stderr, msg)
    System.halt(2)
  end
end

opts = JQ.Options.parse(System.argv())
summary = JQ.Driver.run(opts)
IO.puts(JQ.Driver.format(summary))
System.halt(if summary.violations == [], do: 0, else: 1)
