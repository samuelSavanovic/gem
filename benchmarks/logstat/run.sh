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
echo "input: $LINES lines, $(wc -c < "$WORK/access.log" | tr -d " ") bytes"

# measure <label> <stdin file> <command...>: runs the command with stdout to
# $WORK/out and prints its wall time and peak RSS (getrusage of the child).
measure() {
  local label=$1 input=$2
  shift 2
  python3 - "$label" "$input" "$WORK/out" "$WORK/err" "$@" <<'PY'
import resource, subprocess, sys, time
label, inp, out, err, *cmd = sys.argv[1:]
start = time.monotonic()
with open(inp, "rb") as i, open(out, "wb") as o, open(err, "wb") as e:
    rc = subprocess.call(cmd, stdin=i, stdout=o, stderr=e)
wall = time.monotonic() - start
peak = resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss
if sys.platform == "darwin":
    peak //= 1024    # bytes on macOS, KB on Linux
print(f"{label:<28} {wall:8.2f} s {peak:8d} KB")
sys.exit(rc)
PY
  grep '^gem_diag: arena' "$WORK/err" || true
}

for by in ip path hour; do
  measure "gem --by $by (file)" /dev/null "$WORK/logstat" --quiet --by "$by" "$WORK/access.log"
  cp "$WORK/out" "$WORK/gem_file.txt"
  measure "gem --by $by (stdin)" "$WORK/access.log" "$WORK/logstat" --quiet --by "$by"
  cp "$WORK/out" "$WORK/gem_stdin.txt"
  measure "python --by $by" /dev/null python3 "$SCRIPT_DIR/logstat.py" --quiet --by "$by" "$WORK/access.log"
  for run in file stdin; do
    if ! diff -u "$WORK/out" "$WORK/gem_$run.txt" > "$WORK/diff"; then
      echo "MISMATCH for --by $by ($run):"
      head -20 "$WORK/diff"
      exit 1
    fi
  done
done
echo "reports match"
