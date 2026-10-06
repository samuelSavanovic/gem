#!/usr/bin/env bash
# Times examples/gemgrep against GNU grep (the control) and the Python twin
# (gemgrep.py) on a fixed set of searches, and checks that all three print
# the same lines.
#
#   benchmarks/gemgrep/run.sh [case...]       # default: all of them
#   MB=256 benchmarks/gemgrep/run.sh          # a bigger corpus (default 128 MB)
#   GEM_DIAG=1 benchmarks/gemgrep/run.sh      # plus arena statistics (of each Gem row's last run)
#
# The corpus is generated (gen_corpus.py, the same bytes every time) into a
# temporary directory; the `src_*` cases search the repository's own
# sources. Prints one row per search and program: wall time, peak RSS and
# (on macOS) instructions retired, over BENCH_REPS runs (measure.py, which
# also writes BENCH_CSV); then the Gem/grep and Gem/Python time ratios. The control is `ggrep` when
# it is on PATH (GNU grep on macOS, `brew install grep`), else `grep`.
# grep walks directories in readdir order, gemgrep and the twin in sorted
# order, so grep's output is sorted before the comparison; the twin's must
# match gemgrep's byte for byte.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
GEM="$ROOT/build/gem"
APP="$ROOT/examples/gemgrep"
WORK="$(mktemp -d /tmp/gem_gemgrep_bench.XXXXXX)"
trap 'rm -rf "$WORK"' EXIT
export LC_ALL=C

# <name> <dir: corpus or src> <gemgrep args...>
CASES=(
  "literal corpus -r handler"
  "icase corpus -ri timeout"
  "alternation corpus -r connect(ed|ion)|socket"
  "word corpus -rw id"
  "count corpus -rc error"
  "list corpus -rl deadbeef"
  "invert corpus -rv e"
  "few corpus -rn deadbeef"
  "many corpus -rn e"
  "src_literal src -rn gem_table_set compiler runtime std lsp examples"
  "src_icase src -rin todo|fixme compiler runtime std lsp examples"
)
SELECT=("$@")

[[ -x "$GEM" ]] || { echo "build/gem not found: run 'make build'" >&2; exit 1; }
command -v python3 > /dev/null || { echo "python3 not found" >&2; exit 1; }
GREP=$(command -v ggrep || command -v grep) || { echo "grep not found" >&2; exit 1; }

(cd "$APP" && env -u GEM_DIAG "$GEM" main.gem -o "$WORK/gemgrep")
python3 "$SCRIPT_DIR/gen_corpus.py" "$WORK/corpus" "${MB:-128}"
echo "grep: $("$GREP" --version | head -1); $(python3 --version)"

# measure <impl> <case> <command...>: runs the command (in the cwd) with
# stdout to $WORK/out and prints its wall time, peak RSS and instruction
# count (benchmarks/measure.py: BENCH_REPS, BENCH_CSV); leaves the wall
# time in $WORK/wall. Exit status 1 (nothing selected) is not a failure;
# the output comparisons catch a run that exits 1 for another reason.
measure() {
  local impl=$1 case=$2
  shift 2
  python3 "$SCRIPT_DIR/../measure.py" --bench gemgrep --impl "$impl" --case "$case" \
    --ok-max 1 --width 34 --out "$WORK/out" --err "$WORK/err" --wall-file "$WORK/wall" \
    "$impl" -- "$@"
  grep '^gem_diag: arena' "$WORK/err" || true
}

ratio() {
  python3 -c "import sys; print(f'{sys.argv[1]:<34} {float(sys.argv[2]) / float(sys.argv[3]):8.2f}')" "$@"
}

for c in "${CASES[@]}"; do
  read -r name where args <<< "$c"
  if [[ ${#SELECT[@]} -gt 0 && ! " ${SELECT[*]} " =~ " $name " ]]; then
    continue
  fi
  read -r -a argv <<< "$args"
  if [[ $where == corpus ]]; then
    cd "$WORK/corpus"
    argv+=(.)
  else
    cd "$ROOT"
  fi
  echo "== $name: gemgrep ${argv[*]}"
  measure gem "$name" "$WORK/gemgrep" "${argv[@]}"
  cp "$WORK/out" "$WORK/gem.txt"
  gem_wall=$(cat "$WORK/wall")
  measure grep "$name" "$GREP" -E "${argv[@]}"
  grep_wall=$(cat "$WORK/wall")
  sort "$WORK/out" > "$WORK/grep.txt"
  measure python "$name" python3 "$SCRIPT_DIR/gemgrep.py" "${argv[@]}"
  py_wall=$(cat "$WORK/wall")
  if ! sort "$WORK/gem.txt" | cmp -s - "$WORK/grep.txt"; then
    echo "MISMATCH with grep for $name:"
    sort "$WORK/gem.txt" | diff - "$WORK/grep.txt" | head -10
    exit 1
  fi
  if ! cmp -s "$WORK/gem.txt" "$WORK/out"; then
    echo "MISMATCH with python for $name:"
    diff "$WORK/gem.txt" "$WORK/out" | head -10
    exit 1
  fi
  echo "$(wc -l < "$WORK/gem.txt") lines, $(wc -c < "$WORK/gem.txt") bytes of output"
  ratio "gem/grep" "$gem_wall" "$grep_wall"
  ratio "gem/python" "$gem_wall" "$py_wall"
done
echo "outputs match"
