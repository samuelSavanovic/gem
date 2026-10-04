#!/usr/bin/env bash
# Times examples/logstat against the Python reference (logstat.py) on a
# generated access log, and checks that both print the same report.
#
#   benchmarks/logstat/run.sh [lines]      # default 1000000
#
# Prints one row per run: wall time and peak RSS. GEM_DIAG=1 adds the
# arena reset statistics of each Gem run.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
GEM="$ROOT/build/gem"
APP="$ROOT/examples/logstat"
LINES="${1:-1000000}"
WORK="$(mktemp -d /tmp/gem_logstat_bench.XXXXXX)"
trap 'rm -rf "$WORK"' EXIT

[[ -x "$GEM" ]] || { echo "build/gem not found: run 'make build'" >&2; exit 1; }
command -v python3 > /dev/null || { echo "python3 not found" >&2; exit 1; }

(cd "$APP" && "$GEM" gen.gem -o "$WORK/gen" && "$GEM" main.gem -o "$WORK/logstat")
"$WORK/gen" "$LINES" > "$WORK/access.log"
echo "input: $LINES lines, $(wc -c < "$WORK/access.log") bytes"

# measure <label> <stdin file> <command...>: runs the command with stdout to
# $WORK/out, prints the wall time and the peak RSS (sampled from /proc
# every 20 ms).
measure() {
  local label=$1 input=$2
  shift 2
  local start end peak=0 rss
  start=$(date +%s.%N)
  "$@" < "$input" > "$WORK/out" 2> "$WORK/err" &
  local pid=$!
  while kill -0 "$pid" 2> /dev/null; do
    rss=$(awk '/VmRSS/ {print $2}' "/proc/$pid/status" 2> /dev/null || true)
    if [[ -n "$rss" && "$rss" -gt "$peak" ]]; then peak=$rss; fi
    sleep 0.02
  done
  wait "$pid"
  end=$(date +%s.%N)
  printf "%-28s %8.2f s %8d KB\n" "$label" "$(echo "$end - $start" | bc)" "$peak"
  grep '^gem_diag: arena' "$WORK/err" || true
}

for by in ip path hour; do
  measure "gem --by $by (file)" /dev/null "$WORK/logstat" --quiet --by "$by" "$WORK/access.log"
  cp "$WORK/out" "$WORK/gem.txt"
  measure "gem --by $by (stdin)" "$WORK/access.log" "$WORK/logstat" --quiet --by "$by"
  measure "python --by $by" /dev/null python3 "$SCRIPT_DIR/logstat.py" --quiet --by "$by" "$WORK/access.log"
  if ! diff -u "$WORK/out" "$WORK/gem.txt" > "$WORK/diff"; then
    echo "MISMATCH for --by $by:"
    head -20 "$WORK/diff"
    exit 1
  fi
done
echo "reports match"
