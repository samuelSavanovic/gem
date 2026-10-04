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
# Then, per scenario, the Gem/Python and Gem/Elixir throughput ratios.
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

# name: arguments. Deadlines stay well above a slow attempt's 20 ms.
declare -A SCENARIOS=(
  [none]="--jobs 20000"
  [crash5]="--jobs 20000 --crash 0.05"
  [hang5]="--jobs 10000 --hang 0.05"
  [mixed]="--jobs 10000 --crash 0.05 --hang 0.02 --slow 0.1 --kill 0.03 --deadline 200"
  [storm]="--jobs 10000 --slow 0.1 --deadline 250 --storm 16 --storm-every 100 --storm-bursts 10 --max-restarts 20"
  [backlog]="--jobs 100000 --workers 4 --slow 0.05 --slow-ms 4 --deadline 0"
)
ORDER="none crash5 hang5 mixed storm backlog"
WANT=${*:-$ORDER}

[[ -x "$GEM" ]] || { echo "build/gem not found: run 'make build'" >&2; exit 1; }
command -v python3 > /dev/null || { echo "python3 not found" >&2; exit 1; }
if [[ " $IMPLS " == *" elixir"* ]] && ! command -v elixir > /dev/null; then
  echo "elixir not found: install it or set IMPLS=\"gem python\"" >&2
  exit 1
fi

(cd "$ROOT/examples/jobqueue" && "$GEM" main.gem -o "$WORK/jobqueue")
echo "commit: $(git -C "$ROOT" rev-parse --short HEAD)$(git -C "$ROOT" diff --quiet || echo ' (dirty)')"
echo "system: $(uname -srm), $(nproc 2> /dev/null || sysctl -n hw.ncpu) cores; $(python3 --version)$(command -v elixir > /dev/null && echo "; $(elixir --version | tail -1)")"

# measure <label> <command...>: runs the command with the summary to
# $WORK/out and stderr (crash reports) to $WORK/err, and prints one row:
# wall time and peak RSS of the child (getrusage), and the summary's
# numbers. Exits non-zero when the invariants failed.
measure() {
  local label=$1
  shift
  python3 - "$label" "$WORK/out" "$WORK/err" "$@" <<'PY'
import re, resource, subprocess, sys, time
label, out, err, *cmd = sys.argv[1:]
start = time.monotonic()
with open(out, "wb") as o, open(err, "wb") as e:
    rc = subprocess.call(cmd, stdout=o, stderr=e)
wall = time.monotonic() - start
peak = resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss
if sys.platform == "darwin":
    peak //= 1024
text = open(out).read()
def field(name, pat=r"(\S+)"):
    m = re.search(rf"^{name}\s+{pat}", text, re.M)
    return m.group(1) if m else "?"
lat = re.search(r"p50=(\d+) p90=(\d+) p99=(\d+) max=(\d+)", text)
procs = re.search(r"processes\s+baseline=(\d+) peak=(\d+)", text)
ok = field("invariants")
print(f"{label:<11} {wall:7.2f} s {field('throughput'):>7} jobs/s  "
      f"p50 {lat.group(1) if lat else '?':>5}  p99 {lat.group(3) if lat else '?':>5} ms  "
      f"lag {field('tick_lag_ms'):>5} ms  {peak // 1024:5d} MB  "
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
  args=${SCENARIOS[$name]:-}
  [[ -n "$args" ]] || { echo "unknown scenario $name (one of: $ORDER)" >&2; exit 1; }
  echo "── $name: $args"
  declare -A tp=()
  for impl in $IMPLS; do
    case $impl in
      gem)        cmd=(env GEM_DIAG=1 "$WORK/jobqueue") ;;
      python)     cmd=(python3 "$SCRIPT_DIR/jobqueue.py") ;;
      elixir)     cmd=(elixir "$SCRIPT_DIR/jobqueue.exs") ;;
      elixir_s1)  cmd=(env ELIXIR_ERL_OPTIONS="+S 1" elixir "$SCRIPT_DIR/jobqueue.exs") ;;
      *) echo "unknown implementation $impl" >&2; exit 1 ;;
    esac
    # shellcheck disable=SC2086
    if ! measure "$impl" "${cmd[@]}" $args; then
      failed=$((failed + 1))
    fi
    tp[$impl]=$(throughput)
    if [[ $impl == gem ]]; then
      grep '^gem_diag: arena' "$WORK/err" | sed 's/^/            /' || true
    fi
  done
  ratios=""
  for other in python elixir elixir_s1; do
    if [[ -n "${tp[gem]:-}" && -n "${tp[$other]:-}" && "${tp[$other]}" != 0 ]]; then
      ratios+="$(python3 -c "print(f'gem/$other {${tp[gem]} / ${tp[$other]}:.2f}')")  "
    fi
  done
  [[ -n "$ratios" ]] && echo "            throughput $ratios"
  unset tp
done

if [[ $failed -gt 0 ]]; then
  echo "invariants FAILED in $failed run(s)"
  exit 1
fi
echo "invariants held in every run"
