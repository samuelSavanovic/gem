#!/usr/bin/env bash
# The compiler finds its install root (std/, runtime/, build/libgem_runtime.a)
# two levels above the binary's real path, however it was started: a
# symlink on PATH, a symlink by absolute or relative path, a chain of
# symlinks, a bare name found on PATH, a relative path to the real binary.
# Each case compiles and runs a program that loads std/string, from a
# directory with no std/ of its own and with GEM_STDLIB unset.
#
# Run from the repo root: tests/check_exe_path.sh

set -u
cd "$(dirname "$0")/.."
ROOT=$(pwd -P)

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi
GEM_ABS=$(cd "$(dirname "$GEM")" && pwd -P)/$(basename "$GEM")

tmp=$(mktemp -d "${TMPDIR:-/tmp}/gem_exe_path.XXXXXX")
trap 'rm -rf "$tmp"' EXIT
mkdir -p "$tmp/bin" "$tmp/bin2" "$tmp/work"
ln -s "$GEM_ABS" "$tmp/bin/gem"
ln -s ../bin/gem "$tmp/bin2/gem"
cat > "$tmp/work/prog.gem" <<'GEM'
load "std/string"
print(string.upper("exe path ok"))
GEM
want="EXE PATH OK"

unset GEM_STDLIB
fails=0
total=0
check() {
  local label=$1; shift
  total=$((total + 1))
  local got
  got=$("$@" 2>&1)
  if [ "$got" != "$want" ]; then
    echo "FAIL: $label"
    echo "$got" | head -5
    fails=$((fails + 1))
  fi
}

check "symlink on PATH, bare name" \
  env PATH="$tmp/bin:$PATH" sh -c "cd '$tmp/work' && gem prog.gem"
check "symlink by absolute path" \
  sh -c "cd '$tmp/work' && '$tmp/bin/gem' prog.gem"
check "symlink by relative path" \
  sh -c "cd '$tmp' && bin/gem work/prog.gem"
check "relative symlink to a symlink, on PATH" \
  env PATH="$tmp/bin2:$PATH" sh -c "cd '$tmp/work' && gem prog.gem"
check "real binary by bare name on PATH" \
  env PATH="$(dirname "$GEM_ABS"):$PATH" sh -c "cd '$tmp/work' && gem prog.gem"
case $GEM in
  /*) rel=$GEM ;;
  *) rel=../$GEM ;;
esac
check "real binary by relative path" \
  sh -c "cd '$ROOT/examples' && '$rel' '$tmp/work/prog.gem'"

if [ $fails -eq 0 ]; then
  echo "exe path: $total/$total ok"
else
  echo "exe path: $fails/$total failed"
  exit 1
fi
