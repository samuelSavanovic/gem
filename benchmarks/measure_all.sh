#!/usr/bin/env bash
# Runs every yardstick benchmark and writes a baseline directory: each
# harness's raw output, one CSV row per timed batch run, the machine and
# tool versions, and a summary (benchmarks/summarize.py).
#
#   benchmarks/measure_all.sh
#   SECTIONS="logstat lox" REPS=3 benchmarks/measure_all.sh
#
# Env:
#   OUT       the baseline directory (default
#             benchmarks/baselines/<date>_<machine>, e.g. 2026-10-05_m1pro)
#   SECTIONS  what to run (default: all of them, in this order):
#             logstat lox gemgrep jobqueue bookmark bookmark_node mini_redis stomp
#   REPS      timed runs per batch case (default 5); WARMUP untimed runs
#             before them (default 1; jobqueue takes none)
#   The harnesses' own variables pass through (e.g. BURST_DURATION, N),
#   except SOAK_DURATION, which only the bookmark harness gets (wrk syntax,
#   e.g. 5m); STOMP_SOAK_DURATION is stomp's soak phase in seconds (30).
#
# The batch harnesses (logstat, lox, gemgrep, jobqueue) append their runs
# to batch.csv through benchmarks/measure.py; the servers (bookmark and its
# Node twin, mini_redis against redis-server, stomp) run once each, at
# their harnesses' default sizes (stomp with its soak phase added), and
# write into their own subdirectory. The load generators run on the same
# machine as the servers. A section whose harness exits non-zero is
# marked so in sections.txt and the run goes on.
#
# Needs: wrk, redis-server/redis-benchmark/redis-cli, python3 (and
# python3.14 for stomp), node and npm (the Node twin's dependencies),
# elixir. On macOS GNU grep (`brew install grep`) is the gemgrep control.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

machine_slug() {
  if [[ "$(uname)" == Darwin ]]; then
    sysctl -n machdep.cpu.brand_string | tr '[:upper:]' '[:lower:]' | sed -e 's/^apple //' -e 's/[^a-z0-9]//g'
  else
    uname -m
  fi
}

OUT=${OUT:-"$SCRIPT_DIR/baselines/$(date +%Y-%m-%d)_$(machine_slug)"}
SECTIONS=${SECTIONS:-"logstat lox gemgrep jobqueue bookmark bookmark_node mini_redis stomp"}
REPS=${REPS:-5}
WARMUP=${WARMUP:-1}

if [[ -e "$OUT" && -n "$(ls -A "$OUT" 2> /dev/null)" ]]; then
  echo "$OUT is not empty: pick another OUT or remove it" >&2
  exit 1
fi
mkdir -p "$OUT"
OUT="$(cd "$OUT" && pwd)"

(cd "$ROOT" && make build > /dev/null) || { echo "make build failed" >&2; exit 1; }

version() {
  command -v "$1" > /dev/null || { echo "missing"; return; }
  shift
  "$@" 2>&1 | head -1
}

{
  echo "date: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo "gem commit: $(git -C "$ROOT" rev-parse --short HEAD)$(git -C "$ROOT" diff --quiet HEAD || echo ' (dirty)')"
  echo "system: $(uname -srm)"
  if [[ "$(uname)" == Darwin ]]; then
    echo "os: $(sw_vers -productName) $(sw_vers -productVersion) ($(sw_vers -buildVersion))"
    echo "cpu: $(sysctl -n machdep.cpu.brand_string), $(sysctl -n hw.ncpu) cores ($(sysctl -n hw.perflevel0.physicalcpu 2> /dev/null || echo ?) performance)"
    echo "memory: $(($(sysctl -n hw.memsize) / 1073741824)) GB"
    echo "power: $(pmset -g batt | head -1 | sed "s/.*'\(.*\)'.*/\1/")"
    echo "low power mode: $(pmset -g | awk '/lowpowermode/ {print $2}')"
  else
    echo "cpu: $(grep -m1 'model name' /proc/cpuinfo | cut -d: -f2 | sed 's/^ //'), $(nproc) cores"
    echo "memory: $(awk '/MemTotal/ {printf "%d GB", $2 / 1048576}' /proc/meminfo)"
  fi
  echo "load average at start: $(uptime | sed 's/.*load averages*: //')"
  echo "cc: $(version cc cc --version)"
  echo "python3: $(version python3 python3 --version)"
  echo "python3.14: $(version python3.14 python3.14 --version)"
  echo "node: $(version node node --version)"
  echo "elixir: $(version elixir sh -c 'elixir --version | tail -1')"
  echo "redis: $(version redis-server redis-server --version)"
  echo "wrk: $(version wrk wrk --version)"
  echo "grep: $(version ggrep ggrep --version)"
  echo "REPS=$REPS WARMUP=$WARMUP"
  echo "SECTIONS=$SECTIONS"
} > "$OUT/meta.txt"
cat "$OUT/meta.txt"

# section <name> <command...>: runs one harness with its output in
# <name>.txt and records its status and wall time in sections.txt.
section() {
  local name=$1 start rc
  shift
  echo
  echo "=== $name ($(date +%H:%M:%S), load $(uptime | sed 's/.*load averages*: //'))"
  start=$(date +%s)
  "$@" > "$OUT/$name.txt" 2>&1
  rc=$?
  printf '%-14s %-6s %5d s\n' "$name" "$([[ $rc == 0 ]] && echo ok || echo "rc=$rc")" "$(($(date +%s) - start))" \
    | tee -a "$OUT/sections.txt"
}

export BENCH_CSV="$OUT/batch.csv"
for s in $SECTIONS; do
  case $s in
    logstat)
      section logstat env BENCH_REPS="$REPS" BENCH_WARMUP="$WARMUP" "$SCRIPT_DIR/logstat/run.sh" 1000000
      ;;
    lox)
      section lox env BENCH_REPS="$REPS" BENCH_WARMUP="$WARMUP" "$SCRIPT_DIR/lox/run.sh"
      ;;
    gemgrep)
      section gemgrep env BENCH_REPS="$REPS" BENCH_WARMUP="$WARMUP" "$SCRIPT_DIR/gemgrep/run.sh"
      ;;
    jobqueue)
      section jobqueue env BENCH_REPS="$REPS" "$SCRIPT_DIR/jobqueue/run.sh"
      ;;
    bookmark)
      if (cd "$ROOT/examples/bookmark_app" && "$ROOT/build/gem" app.gem -o app); then
        section bookmark env OUT="$OUT/bookmark" "$SCRIPT_DIR/run.sh"
      else
        echo "bookmark       build failed" | tee -a "$OUT/sections.txt"
      fi
      ;;
    bookmark_node)
      [[ -d "$SCRIPT_DIR/node_baseline/node_modules" ]] || (cd "$SCRIPT_DIR/node_baseline" && npm install --silent)
      section bookmark_node env OUT="$OUT/bookmark_node" "$SCRIPT_DIR/node_baseline/run_bench.sh"
      ;;
    mini_redis)
      section mini_redis env OUT="$OUT/mini_redis" "$SCRIPT_DIR/mini_redis/run.sh"
      ;;
    stomp)
      section stomp env OUT="$OUT/stomp" PHASES="fanout slow queue soak" \
        SOAK_DURATION="${STOMP_SOAK_DURATION:-30}" "$SCRIPT_DIR/stomp/run.sh"
      ;;
    *)
      echo "unknown section: $s" >&2
      ;;
  esac
done

# Built binaries are not part of the record.
rm -f "$OUT/mini_redis/mini_redis" "$OUT/stomp/stomp_broker"

echo
python3 "$SCRIPT_DIR/summarize.py" "$OUT"
echo "baseline in $OUT"
