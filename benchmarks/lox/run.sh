#!/usr/bin/env bash
# Times examples/lox against the Python reference (lox.py) on each of the
# bench programs in examples/lox/bench, and checks that both print the
# same output.
#
#   benchmarks/lox/run.sh [program...]      # default: all of them
#
# Each program runs at the size below (its argument). Prints one row per
# program and implementation: wall time, peak RSS and (on macOS)
# instructions retired, over BENCH_REPS runs (measure.py, which also
# writes BENCH_CSV); then the Gem/Python time ratio. GEM_DIAG=1 adds the
# arena reset statistics of each Gem row's last run.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
GEM="$ROOT/build/gem"
APP="$ROOT/examples/lox"
WORK="$(mktemp -d /tmp/gem_lox_bench.XXXXXX)"
trap 'rm -rf "$WORK"' EXIT

# The argument each program runs with: sized so the Python run takes
# seconds, not minutes.
size_of() {
  case $1 in
    fib) echo 28 ;;
    binary_trees) echo 12 ;;
    closures) echo 100000 ;;
    strings) echo 20000 ;;
    mandelbrot) echo 60 ;;
    methods) echo 3000 ;;
    *) echo "unknown program: $1" >&2; exit 2 ;;
  esac
}
ORDER=(fib binary_trees closures strings mandelbrot methods)
if [[ $# -gt 0 ]]; then
  ORDER=("$@")
fi

[[ -x "$GEM" ]] || { echo "build/gem not found: run 'make build'" >&2; exit 1; }
command -v python3 > /dev/null || { echo "python3 not found" >&2; exit 1; }

(cd "$APP" && env -u GEM_DIAG "$GEM" main.gem -o "$WORK/lox")

# measure <impl> <case> <command...>: runs the command with stdout to
# $WORK/out and prints its wall time, peak RSS and instruction count
# (benchmarks/measure.py: BENCH_REPS, BENCH_CSV); leaves the wall time in
# $WORK/wall.
measure() {
  local impl=$1 case=$2
  shift 2
  python3 "$SCRIPT_DIR/../measure.py" --bench lox --impl "$impl" --case "$case" \
    --out "$WORK/out" --err "$WORK/err" --wall-file "$WORK/wall" "$impl $case" -- "$@"
  grep '^gem_diag: arena' "$WORK/err" || true
}

for name in "${ORDER[@]}"; do
  size=$(size_of "$name")
  prog="$APP/bench/$name.lox"
  measure gem "$name $size" "$WORK/lox" "$prog" "$size"
  cp "$WORK/out" "$WORK/gem.txt"
  gem_wall=$(cat "$WORK/wall")
  measure python "$name $size" python3 "$SCRIPT_DIR/lox.py" "$prog" "$size"
  py_wall=$(cat "$WORK/wall")
  if ! diff -u "$WORK/out" "$WORK/gem.txt" > "$WORK/diff"; then
    echo "MISMATCH for $name:"
    head -20 "$WORK/diff"
    exit 1
  fi
  python3 -c "import sys; print(f'{\"gem/python\":<28} {float(sys.argv[1]) / float(sys.argv[2]):8.2f}')" "$gem_wall" "$py_wall"
done
echo "outputs match"
