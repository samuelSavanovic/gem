#!/usr/bin/env bash
# Run the larger example programs, each of which checks itself: the JSON
# parser and the bookmark app and STOMP broker test suites (std/test, exit
# status 1 on a failing case), and the TCP echo program (raises on a bad
# echo). The bookmark app looks its static files up relative to the cwd,
# so its tests run from its own directory.
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
  if ! (cd "$dir" && timeout 60 "$T/$name") > "$T/$name.out" 2>&1; then
    echo "FAIL: $dir/$file:"
    cat "$T/$name.out"
    fails=$((fails + 1))
  fi
}

run json_parser examples json_parser.gem
run tcp_echo examples tcp_echo.gem
run bookmark_app examples/bookmark_app test.gem
run stomp_broker examples/stomp_broker test.gem

# The entry points the tests don't build.
for f in examples/bookmark_app/app.gem examples/stomp_broker/main.gem; do
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
