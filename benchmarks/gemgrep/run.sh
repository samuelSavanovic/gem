#!/usr/bin/env bash
# Times examples/gemgrep against GNU grep (the control) and the Python twin
# (gemgrep.py) on a fixed set of searches, and checks that all three print
# the same lines.
#
#   benchmarks/gemgrep/run.sh [case...]       # default: all of them
#   MB=256 benchmarks/gemgrep/run.sh          # a bigger corpus (default 128 MB)
#   GEM_DIAG=1 benchmarks/gemgrep/run.sh      # plus each Gem run's arena statistics
#
# The corpus is generated (gen_corpus.py, the same bytes every time) into a
# temporary directory; the `src_*` cases search the repository's own
# sources. Prints one row per run: wall time and peak RSS, then the
# Gem/grep and Gem/Python time ratios. grep walks directories in readdir
# order, gemgrep and the twin in sorted order, so grep's output is sorted
# before the comparison; the twin's must match gemgrep's byte for byte.

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
command -v grep > /dev/null || { echo "grep not found" >&2; exit 1; }

(cd "$APP" && env -u GEM_DIAG "$GEM" main.gem -o "$WORK/gemgrep")
python3 "$SCRIPT_DIR/gen_corpus.py" "$WORK/corpus" "${MB:-128}"
echo "grep: $(grep --version | head -1); $(python3 --version)"

# measure <label> <command...>: runs the command (in the cwd) with stdout to
# $WORK/out and prints its wall time and peak RSS (getrusage of the child);
# leaves the wall time in $WORK/wall. Exit status 1 (nothing selected) is
# not a failure.
measure() {
  local label=$1
  shift
  python3 - "$label" "$WORK" "$@" <<'PY'
import resource, subprocess, sys, time
label, work, *cmd = sys.argv[1:]
start = time.monotonic()
with open(f"{work}/out", "wb") as o, open(f"{work}/err", "wb") as e:
    rc = subprocess.call(cmd, stdout=o, stderr=e)
wall = time.monotonic() - start
peak = resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss
if sys.platform == "darwin":
    peak //= 1024    # bytes on macOS, KB on Linux
print(f"{label:<34} {wall:8.2f} s {peak:9d} KB")
open(f"{work}/wall", "w").write(f"{wall}\n")
if rc > 1:
    sys.stdout.write(open(f"{work}/err").read())
    sys.exit(rc)
PY
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
  measure "gem" "$WORK/gemgrep" "${argv[@]}"
  cp "$WORK/out" "$WORK/gem.txt"
  gem_wall=$(cat "$WORK/wall")
  measure "grep -E" grep -E "${argv[@]}"
  grep_wall=$(cat "$WORK/wall")
  sort "$WORK/out" > "$WORK/grep.txt"
  measure "python" python3 "$SCRIPT_DIR/gemgrep.py" "${argv[@]}"
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
