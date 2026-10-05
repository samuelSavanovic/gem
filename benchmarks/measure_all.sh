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
#   except OUT (each server harness gets $OUT/<section>), PHASES (stomp
#   always runs fanout, slow, queue and soak) and SOAK_DURATION, which only
#   the bookmark harness gets (wrk syntax, e.g. 5m); STOMP_SOAK_DURATION is
#   stomp's soak phase in seconds (30).
#   GUARD_RSS_MB     memory cap per section (default: half the RAM)
#   GUARD_SWAP_MB    swap growth cap per section (default 2048)
#   GUARD_TIMEOUT_S  time cap per section (default 2400)
#
# Each section runs in its own process group, stdin from /dev/null, under a
# watchdog that samples it about every half second and kills the whole group (harness, servers, load
# generators) when the RSS of its processes passes GUARD_RSS_MB, when swap
# in use has grown by GUARD_SWAP_MB since the section started, when the
# system reports critical memory pressure (macOS), or at GUARD_TIMEOUT_S.
# sections.txt gives each section's status, wall time and peak RSS (the
# highest sample), and the reason for a kill.
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
# elixir. On macOS GNU grep (`brew install grep`), when installed, is the
# gemgrep control.

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
if [[ "$(uname)" == Darwin ]]; then
  RAM_MB=$(($(sysctl -n hw.memsize) / 1048576))
else
  RAM_MB=$(($(awk '/MemTotal/ {print $2}' /proc/meminfo) / 1024))
fi
GUARD_RSS_MB=${GUARD_RSS_MB:-$((RAM_MB / 2))}
GUARD_SWAP_MB=${GUARD_SWAP_MB:-2048}
GUARD_TIMEOUT_S=${GUARD_TIMEOUT_S:-2400}

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
  echo "GUARD_RSS_MB=$GUARD_RSS_MB GUARD_SWAP_MB=$GUARD_SWAP_MB GUARD_TIMEOUT_S=$GUARD_TIMEOUT_S"
  echo "SECTIONS=$SECTIONS"
} > "$OUT/meta.txt"
cat "$OUT/meta.txt"

# group_rss_kb <pgid>: the summed RSS of the processes in a process group.
group_rss_kb() {
  ps -A -o pgid=,rss= | awk -v g="$1" '$1 == g { s += $2 } END { print s + 0 }'
}

# swap_used_mb: swap in use, in whole MB.
swap_used_mb() {
  if [[ "$(uname)" == Darwin ]]; then
    sysctl -n vm.swapusage | sed -E 's/.*used = ([0-9]+)(\.[0-9]+)?M.*/\1/'
  else
    awk '/SwapTotal/ {t = $2} /SwapFree/ {f = $2} END {print int((t - f) / 1024)}' /proc/meminfo
  fi
}

# pressure_level: macOS memory pressure (1 normal, 2 warning, 4 critical);
# 0 elsewhere.
pressure_level() {
  sysctl -n kern.memorystatus_vm_pressure_level 2> /dev/null || echo 0
}

# Background jobs get a process group each, so the watchdog can kill a
# section's processes together.
set -m

# section <name> <command...>: runs one harness under the watchdog with
# its output in <name>.txt and records its status, wall time and peak RSS
# (the highest sample) in sections.txt.
section() {
  local name=$1 start pid rc rss peak=0 swap0 why="" now
  shift
  echo
  echo "=== $name ($(date +%H:%M:%S), load $(uptime | sed 's/.*load averages*: //'))"
  start=$(date +%s)
  swap0=$(swap_used_mb)
  "$@" < /dev/null > "$OUT/$name.txt" 2>&1 &
  pid=$!
  while kill -0 "$pid" 2> /dev/null; do
    rss=$(group_rss_kb "$pid")
    ((rss > peak)) && peak=$rss
    now=$(date +%s)
    if ((rss / 1024 > GUARD_RSS_MB)); then
      why="RSS $((rss / 1024)) MB > GUARD_RSS_MB=$GUARD_RSS_MB"
    elif (($(swap_used_mb) - swap0 > GUARD_SWAP_MB)); then
      why="swap grew $(($(swap_used_mb) - swap0)) MB > GUARD_SWAP_MB=$GUARD_SWAP_MB"
    elif (($(pressure_level) >= 4)); then
      why="critical memory pressure"
    elif ((now - start > GUARD_TIMEOUT_S)); then
      why="running for $((now - start)) s > GUARD_TIMEOUT_S=$GUARD_TIMEOUT_S"
    fi
    if [[ -n "$why" ]]; then
      echo "!! killing $name: $why"
      kill -TERM -- "-$pid" 2> /dev/null
      for _ in 1 2 3 4 5 6 7 8 9 10; do
        kill -0 "$pid" 2> /dev/null || break
        sleep 0.5
      done
      kill -KILL -- "-$pid" 2> /dev/null
      break
    fi
    sleep 0.5
  done
  wait "$pid" 2> /dev/null
  rc=$?
  printf '%-14s %-6s %5d s  peak %6d MB%s\n' "$name" \
    "$([[ -n "$why" ]] && echo killed || { [[ $rc == 0 ]] && echo ok || echo "rc=$rc"; })" \
    "$(($(date +%s) - start))" "$((peak / 1024))" "${why:+  ($why)}" | tee -a "$OUT/sections.txt"
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
