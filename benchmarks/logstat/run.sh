#!/usr/bin/env bash
# Times examples/logstat against the Python reference (logstat.py) on a
# generated access log, and checks that both print the same report.
#
#   benchmarks/logstat/run.sh [lines]      # default 1000000
#
# Prints one row per case and program: wall time, peak RSS and (on macOS)
# instructions retired, over BENCH_REPS runs (measure.py, which also
# writes BENCH_CSV). GEM_DIAG=1 adds the arena reset statistics of each
# Gem row's last run.

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
echo "input: $LINES lines, $(wc -c < "$WORK/access.log" | tr -d " ") bytes"

# measure <impl> <case> <stdin file> <command...>: runs the command with
# stdout to $WORK/out and prints its wall time, peak RSS and instruction
# count (benchmarks/measure.py: BENCH_REPS, BENCH_CSV).
measure() {
  local impl=$1 case=$2 input=$3
  shift 3
  python3 "$SCRIPT_DIR/../measure.py" --bench logstat --impl "$impl" --case "$case" \
    --stdin "$input" --out "$WORK/out" --err "$WORK/err" --width 24 "$impl $case" -- "$@"
  grep '^gem_diag: arena' "$WORK/err" || true
}

for by in ip path hour; do
  measure gem_file "--by $by" /dev/null "$WORK/logstat" --quiet --by "$by" "$WORK/access.log"
  cp "$WORK/out" "$WORK/gem_file.txt"
  measure gem_stdin "--by $by" "$WORK/access.log" "$WORK/logstat" --quiet --by "$by"
  cp "$WORK/out" "$WORK/gem_stdin.txt"
  measure python "--by $by" /dev/null python3 "$SCRIPT_DIR/logstat.py" --quiet --by "$by" "$WORK/access.log"
  for run in file stdin; do
    if ! diff -u "$WORK/out" "$WORK/gem_$run.txt" > "$WORK/diff"; then
      echo "MISMATCH for --by $by ($run):"
      head -20 "$WORK/diff"
      exit 1
    fi
  done
done
echo "reports match"
