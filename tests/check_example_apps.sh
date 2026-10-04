#!/usr/bin/env bash
# Run the larger example programs, each of which checks itself: the JSON
# parser and the bookmark app, STOMP broker, mini_redis and logstat test
# suites (std/test, exit status 1 on a failing case), and the TCP echo program
# (raises on a bad echo). The bookmark app looks its static files up
# relative to the cwd, so its tests run from its own directory. logstat
# also runs as a program on a generated log.
#
# Run from the repo root: tests/check_example_apps.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi
GEM=$(cd "$(dirname "$GEM")" && pwd)/$(basename "$GEM")

T=$(mktemp -d /tmp/gem_example_apps.XXXXXX)
trap 'rm -rf "$T"' EXIT

fails=0

# limit <secs> <cmd...>: run cmd, killed by SIGALRM after secs. perl, since
# macOS has no `timeout`.
limit() {
  perl -e 'alarm shift; exec @ARGV or die "exec $ARGV[0]: $!\n"' "$@"
}

# run <name> <dir> <file>: build <file> in <dir>, run it there with a time
# limit, and show its output only when it fails.
run() {
  local name=$1 dir=$2 file=$3
  if ! (cd "$dir" && "$GEM" "$file" -o "$T/$name") > "$T/$name.out" 2>&1; then
    echo "FAIL: $dir/$file doesn't compile:"
    cat "$T/$name.out"
    fails=$((fails + 1))
    return
  fi
  if ! (cd "$dir" && limit 60 "$T/$name") > "$T/$name.out" 2>&1; then
    echo "FAIL: $dir/$file:"
    cat "$T/$name.out"
    fails=$((fails + 1))
  fi
}

run json_parser examples json_parser.gem
run tcp_echo examples tcp_echo.gem
run bookmark_app examples/bookmark_app test.gem
run stomp_broker examples/stomp_broker test.gem
run mini_redis examples/mini_redis test.gem
run logstat_test examples/logstat test.gem

# logstat end to end: a generated log, read from a file and from stdin, and
# the exit statuses for a bad flag (2) and an unreadable file (1).
L=examples/logstat
if "$GEM" "$L/gen.gem" -o "$T/gen" > /dev/null 2>&1 && "$GEM" "$L/main.gem" -o "$T/logstat" > /dev/null 2>&1; then
  "$T/gen" 30000 > "$T/access.log"
  for mode in file stdin; do
    if [ "$mode" = file ]; then
      "$T/logstat" --by status --sort bytes "$T/access.log" > "$T/report" 2> "$T/stderr"
    else
      "$T/logstat" --by status --sort bytes < "$T/access.log" > "$T/report" 2> "$T/stderr"
    fi
    if ! diff -u "$L/expected_report.txt" "$T/report" || [ "$(wc -l < "$T/stderr")" -ne 2 ]; then
      echo "FAIL: logstat ($mode) report or malformed-line messages differ"
      cat "$T/stderr"
      fails=$((fails + 1))
    fi
  done
  "$T/logstat" --by agent > /dev/null 2>&1
  bad_flag=$?
  "$T/logstat" "$T/missing.log" > /dev/null 2>&1
  missing=$?
  if [ "$bad_flag" -ne 2 ] || [ "$missing" -ne 1 ]; then
    echo "FAIL: logstat exit statuses: bad flag $bad_flag (want 2), missing file $missing (want 1)"
    fails=$((fails + 1))
  fi
else
  echo "FAIL: logstat doesn't compile"
  fails=$((fails + 1))
fi

# The entry points the tests don't build.
for f in examples/bookmark_app/app.gem examples/stomp_broker/main.gem examples/mini_redis/main.gem; do
  if ! "$GEM" --check "$f" > "$T/check.out" 2>&1 || [ -s "$T/check.out" ]; then
    echo "FAIL: gem --check $f:"
    cat "$T/check.out"
    fails=$((fails + 1))
  fi
done

if [ "$fails" -gt 0 ]; then
  exit 1
fi
echo "ALL EXAMPLE APPS PASSED"
