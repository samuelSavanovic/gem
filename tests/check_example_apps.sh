#!/usr/bin/env bash
# Run the larger example programs, each of which checks itself: the JSON
# parser and the bookmark app, STOMP broker, mini_redis, logstat, lox,
# gemgrep and jobqueue test suites (std/test, exit status 1 on a failing
# case), and the TCP echo program (raises on a bad echo). The bookmark app looks its
# static files up relative to the cwd, so its tests run from its own
# directory (as do lox's, which read its bench programs). logstat also
# runs as a program on a generated log, and gemgrep on stdin and a small
# tree, for its output, messages and exit statuses, and jobqueue on a
# small seeded run with every kind of fault.
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
run lox_test examples/lox test.gem
run gemgrep_test examples/gemgrep test.gem
run jobqueue_test examples/jobqueue test.gem

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

# gemgrep end to end: stdin, a tree with -r, a missing file and a bad
# pattern, a bad option; stdout, stderr and the exit status of each.
if "$GEM" examples/gemgrep/main.gem -o "$T/gemgrep" > /dev/null 2>&1; then
  G=$T/gemgrep
  mkdir -p "$T/gg/sub"
  printf 'foo bar\nbaz\n' > "$T/gg/a.txt"
  printf 'x\nFOO\n' > "$T/gg/sub/b.txt"
  printf 'bin\0foo\n' > "$T/gg/sub/c.dat"
  gg_case() {
    local want_status=$1 want_out=$2 want_err=$3
    shift 3
    local status=0
    (cd "$T/gg" && "$G" "$@" < a.txt > "$T/gg.out" 2> "$T/gg.err") || status=$?
    if [ "$status" -ne "$want_status" ] || [ "$(cat "$T/gg.out")" != "$want_out" ] || [ "$(cat "$T/gg.err")" != "$want_err" ]; then
      echo "FAIL: gemgrep $*: status $status (want $want_status)"
      echo "--- stdout:"; cat "$T/gg.out"; echo "--- stderr:"; cat "$T/gg.err"
      fails=$((fails + 1))
    fi
  }
  gg_case 0 "foo bar" "" foo
  gg_case 0 "(standard input):1:foo bar" "" -Hn foo -
  gg_case 0 "a.txt:foo bar
sub/b.txt:FOO" "gemgrep: sub/c.dat: binary file matches" -ri foo
  gg_case 1 "" "" zzz a.txt
  gg_case 2 "a.txt:1" "gemgrep: nope: No such file or directory" -c foo nope a.txt
  gg_case 2 "" "gemgrep: sub: Is a directory" foo sub
  gg_case 2 "" "gemgrep: Unmatched ( or \(" "a(" a.txt
  gg_case 2 "" "gemgrep: invalid option -- 'z'
Usage: gemgrep [OPTION]... PATTERN [FILE]...
Try 'gemgrep --help' for more information." -z foo
else
  echo "FAIL: gemgrep doesn't compile"
  fails=$((fails + 1))
fi

# jobqueue end to end: a seeded run with every fault and a storm must hold
# its invariants (exit 0); a bad option exits 2.
if "$GEM" examples/jobqueue/main.gem -o "$T/jobqueue" > /dev/null 2>&1; then
  status=0
  limit 60 "$T/jobqueue" --jobs 2000 --crash 0.05 --hang 0.02 --slow 0.05 --kill 0.02 \
    --storm 8 --storm-every 50 --storm-bursts 4 --idle 50 > "$T/jq.out" 2> /dev/null || status=$?
  if [ "$status" -ne 0 ] || ! grep -q '^invariants    ok$' "$T/jq.out"; then
    echo "FAIL: jobqueue run: status $status"
    cat "$T/jq.out"
    fails=$((fails + 1))
  fi
  status=0
  "$T/jobqueue" --crash 2 > /dev/null 2>&1 || status=$?
  if [ "$status" -ne 2 ]; then
    echo "FAIL: jobqueue --crash 2: status $status (want 2)"
    fails=$((fails + 1))
  fi
else
  echo "FAIL: jobqueue doesn't compile"
  fails=$((fails + 1))
fi

# The entry points the tests don't build.
for f in examples/bookmark_app/app.gem examples/stomp_broker/main.gem examples/mini_redis/main.gem examples/lox/main.gem examples/gemgrep/main.gem examples/jobqueue/main.gem; do
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
