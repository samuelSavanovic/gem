#!/usr/bin/env bash
# Reset stress mode (make test-stress): run the numbered examples, the
# example apps and the compiler with GEM_RESET_STRESS set, so every loop
# resets at almost every back-edge and every return of a recursive fn
# resets, and check they behave exactly as without it. A reset that loses
# or corrupts a live value shows up as a diff, a crash or a test failure.
#
# GEM_RESET_STRESS=<bytes> (default 1) is the reset threshold; the
# compiler runs under it too, since it is a Gem program. Not part of
# `make test`: it takes about as long again.
#
# Run from the repo root: tests/check_stress.sh

set -u
cd "$(dirname "$0")/.."

export GEM_RESET_STRESS=${GEM_RESET_STRESS:-1}
GEM=build/gem
fails=0

echo "== examples (GEM_RESET_STRESS=$GEM_RESET_STRESS)"
bash examples/run_all.sh || fails=$((fails + 1))

echo "== example apps"
bash tests/check_example_apps.sh || fails=$((fails + 1))

# The compiler compiling itself: the biggest Gem program there is, with
# deep recursion (the parser, codegen) and long-lived tables.
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
