#!/usr/bin/env bash
# make test-stress: the numbered examples, example apps and the compiler under GEM_RESET_STRESS, so
# loops reset at almost every back-edge, and they must behave exactly as without it. A reset that
# loses or corrupts a live value shows up as a diff, a crash or a failing test. GEM_RESET_STRESS=<bytes>
# (default 1) is the threshold. Not in `make test`: it takes about as long again.

set -u
cd "$(dirname "$0")/.."

export GEM_RESET_STRESS=${GEM_RESET_STRESS:-1}
export GEM=${GEM:-build/gem}
fails=0

echo "== examples (GEM_RESET_STRESS=$GEM_RESET_STRESS)"
bash examples/run_all.sh || fails=$((fails + 1))

echo "== example apps"
bash tests/check_example_apps.sh || fails=$((fails + 1))

# the compiler compiling itself: the biggest Gem program, with deep recursion and long-lived tables
echo "== compiler self-compile"
T=$(mktemp -d "${TMPDIR:-/tmp}/gem_stress.XXXXXX")
trap 'rm -rf "$T"' EXIT
if ! GEM_RESET_STRESS= "$GEM" compiler/main.gem --emit-c > "$T/plain.c"; then
  echo "FAIL: the compiler doesn't compile itself"
  fails=$((fails + 1))
elif ! "$GEM" compiler/main.gem --emit-c > "$T/stress.c"; then
  echo "FAIL: the compiler doesn't compile itself under GEM_RESET_STRESS"
  fails=$((fails + 1))
elif ! cmp -s "$T/plain.c" "$T/stress.c"; then
  echo "FAIL: the compiler emits different C under GEM_RESET_STRESS"
  diff "$T/plain.c" "$T/stress.c" | head -20
  fails=$((fails + 1))
else
  echo "compiler: same C with and without stress"
fi

if [ "$fails" -eq 0 ]; then
  echo "stress: OK"
else
  echo "stress: $fails part(s) failed"
  exit 1
fi
