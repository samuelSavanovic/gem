#!/usr/bin/env bash
# Check the driver's one-line errors for a source path it cannot read:
# exact stderr, empty stdout, exit status 1, no stack trace.
#
# Run from the repo root: tests/check_cli.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi

fails=0
total=0

# check <expected stderr> <expected status> <gem args...>
check() {
  local want_err=$1 want_status=$2
  shift 2
  total=$((total + 1))
  local out err status
  err=$("$GEM" "$@" 2>&1 >/dev/null)
  out=$("$GEM" "$@" 2>/dev/null)
  status=$?
  if [ "$err" != "$want_err" ] || [ "$status" != "$want_status" ] || [ -n "$out" ]; then
    echo "FAIL: gem $*"
    echo "  want status $want_status, stderr: $want_err"
    echo "  got  status $status, stderr: $err"
    [ -n "$out" ] && echo "  stdout: $out"
    fails=$((fails + 1))
  fi
}

check "gem: cannot open 'tests/no_such_file.gem'" 1 tests/no_such_file.gem
check "gem: cannot open 'tests/no_such_file.gem'" 1 --check tests/no_such_file.gem
check "gem: cannot open 'tests/no_such_dir/x.gem'" 1 tests/no_such_dir/x.gem -o /dev/null
check "gem: 'tests' is a directory, not a source file" 1 tests
check "gem: 'tests/' is a directory, not a source file" 1 --emit-c tests/

if [ "$fails" -gt 0 ]; then
  echo
  echo "$fails / $total CLI checks failed"
  exit 1
fi
echo "all $total CLI checks passed"
