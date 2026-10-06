#!/usr/bin/env bash
# Runs examples/jobqueue, its Python asyncio twin (jobqueue.py) and its
# Elixir/OTP control (jobqueue.exs) through a fixed set of seeded fault
# scenarios, and checks that the invariants hold in every one of them.
#
#   benchmarks/jobqueue/run.sh                    # every scenario
#   benchmarks/jobqueue/run.sh crash5 storm       # some of them
#   IMPLS="gem python" benchmarks/jobqueue/run.sh
#
# For each run it prints the wall time of the whole program, the
# throughput and latency the program measured (from the first submit to
# the last job finished), the longest tick lag (how long no process could
# run), peak RSS and peak process count, and whether the invariants held.
# Then, per scenario, the Gem/Python and Gem/Elixir throughput ratios (of
# each one's last run). BENCH_REPS runs each one that many times, a row
# per run; BENCH_CSV is measure.py's.
# Elixir runs twice: as it comes (one scheduler per core) and with +S 1
# (one scheduler, like Gem and asyncio). The Gem runs set GEM_DIAG=1;
# their arena statistics follow their rows. Needs python3 >= 3.11 and
# elixir (apt install elixir); IMPLS leaves out what is missing.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
GEM="$ROOT/build/gem"
WORK="$(mktemp -d /tmp/gem_jobqueue_bench.XXXXXX)"
trap 'rm -rf "$WORK"' EXIT

IMPLS=${IMPLS:-"gem python elixir elixir_s1"}

# args_of <scenario>: its arguments. Deadlines leave room for Gem's reset
# pauses (docs/OPTIMIZATIONS.md, "Full resets still copy all a loop
# keeps"): with the default 100 ms, a 20,000-job run now and then kills a
# healthy attempt.
args_of() {
  case $1 in
    none) echo "--jobs 20000 --deadline 250" ;;
    crash5) echo "--jobs 20000 --crash 0.05 --deadline 250" ;;
    hang5) echo "--jobs 10000 --hang 0.05" ;;
    mixed) echo "--jobs 10000 --crash 0.05 --hang 0.02 --slow 0.1 --kill 0.03 --deadline 200" ;;
    storm) echo "--jobs 10000 --slow 0.1 --deadline 250 --storm 16 --storm-every 100 --storm-bursts 10 --max-restarts 20" ;;
    backlog) echo "--jobs 100000 --workers 4 --slow 0.05 --slow-ms 4 --deadline 0" ;;
  esac
}
ORDER="none crash5 hang5 mixed storm backlog"
WANT=${*:-$ORDER}
REPS=${BENCH_REPS:-1}

[[ -x "$GEM" ]] || { echo "build/gem not found: run 'make build'" >&2; exit 1; }
command -v python3 > /dev/null || { echo "python3 not found" >&2; exit 1; }
if [[ " $IMPLS " == *" elixir"* ]] && ! command -v elixir > /dev/null; then
  echo "elixir not found: install it or set IMPLS=\"gem python\"" >&2
  exit 1
fi

(cd "$ROOT/examples/jobqueue" && "$GEM" main.gem -o "$WORK/jobqueue")
echo "commit: $(git -C "$ROOT" rev-parse --short HEAD)$(git -C "$ROOT" diff --quiet || echo ' (dirty)')"
echo "system: $(uname -srm), $(nproc 2> /dev/null || sysctl -n hw.ncpu) cores; $(python3 --version)$(command -v elixir > /dev/null && echo "; $(elixir --version | tail -1)")"

# measure <impl> <scenario> <command...>: runs the command once with the
# summary to $WORK/out and stderr (crash reports) to $WORK/err, and prints
# one row: wall time and peak RSS (benchmarks/measure.py, which also
# appends to BENCH_CSV), and the summary's numbers. Exit status 1, which
# the three programs give when the invariants fail, still prints the row;
# measure returns non-zero unless the summary says the invariants held.
measure() {
  local impl=$1 scenario=$2 row
  shift 2
  row=$(BENCH_REPS=1 BENCH_WARMUP=0 python3 "$SCRIPT_DIR/../measure.py" --bench jobqueue \
    --impl "$impl" --case "$scenario" --ok-max 1 --out "$WORK/out" --err "$WORK/err" "$impl" -- "$@") || {
    echo "$row"
    return 1
  }
  python3 - "$row" "$WORK/out" <<'PY'
import re, sys
row, out = sys.argv[1:]
f = row.split()
wall, peak_kb = float(f[1]), int(f[3])
text = open(out).read()
def field(name, pat=r"(\S+)"):
    m = re.search(rf"^{name}\s+{pat}", text, re.M)
    return m.group(1) if m else "?"
lat = re.search(r"p50=(\d+) p90=(\d+) p99=(\d+) max=(\d+)", text)
procs = re.search(r"processes\s+baseline=(\d+) peak=(\d+)", text)
ok = field("invariants")
print(f"{f[0]:<11} {wall:7.2f} s {field('throughput'):>7} jobs/s  "
      f"p50 {lat.group(1) if lat else '?':>5}  p99 {lat.group(3) if lat else '?':>5} ms  "
      f"lag {field('tick_lag_ms'):>5} ms  {peak_kb // 1024:5d} MB  "
      f"procs {procs.group(1) + '+' + str(int(procs.group(2)) - int(procs.group(1))) if procs else '?':>7}  "
      f"retries {field('retries'):>5}  sup {field('sup_restarts'):>2}  {ok}")
if ok != "ok":
    print(text, file=sys.stderr)
    sys.exit(1)
PY
}

throughput() {
  grep '^throughput' "$WORK/out" | awk '{print $2}'
}

failed=0
printf '%-11s %9s %14s %9s %10s %11s %8s %9s %13s\n' run wall throughput p50 p99 "tick lag" "peak RSS" "procs" ""
for name in $WANT; do
  args=$(args_of "$name")
  [[ -n "$args" ]] || { echo "unknown scenario $name (one of: $ORDER)" >&2; exit 1; }
  echo "── $name: $args"
  tp_gem=""
  ratios=""
  for impl in $IMPLS; do
    case $impl in
      gem)        cmd=(env GEM_DIAG=1 "$WORK/jobqueue") ;;
      python)     cmd=(python3 "$SCRIPT_DIR/jobqueue.py") ;;
      elixir)     cmd=(elixir "$SCRIPT_DIR/jobqueue.exs") ;;
      elixir_s1)  cmd=(env ELIXIR_ERL_OPTIONS="+S 1" elixir "$SCRIPT_DIR/jobqueue.exs") ;;
      *) echo "unknown implementation $impl" >&2; exit 1 ;;
    esac
    for _ in $(seq "$REPS"); do
      # shellcheck disable=SC2086
      if ! measure "$impl" "$name" "${cmd[@]}" $args; then
        failed=$((failed + 1))
      fi
      if [[ $impl == gem ]]; then
        grep '^gem_diag: arena' "$WORK/err" | sed 's/^/            /' || true
      fi
    done
    tp=$(throughput)
    if [[ $impl == gem ]]; then
      tp_gem=$tp
    elif [[ -n "$tp_gem" && -n "$tp" && "$tp" != 0 ]]; then
      ratios+="$(python3 -c "print(f'gem/$impl {$tp_gem / $tp:.2f}')")  "
    fi
  done
  [[ -n "$ratios" ]] && echo "            throughput $ratios"
done

if [[ $failed -gt 0 ]]; then
  echo "invariants FAILED in $failed run(s)"
  exit 1
fi
echo "invariants held in every run"
