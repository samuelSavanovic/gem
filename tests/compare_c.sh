#!/usr/bin/env bash
# tests/compare_c.sh OLD NEW: both compilers must emit the same C (and the same errors and exit
# status) for every example, example app file, broken program, std module, the compiler and the LSP.
# A refactor that should change no output passes it. Both compilers have to live in build/ to find
# std/ and the runtime, e.g. `cp build/gem build/gem_old` before rebuilding.

set -u
cd "$(dirname "$0")/.."

if [ $# -ne 2 ]; then
  echo "usage: tests/compare_c.sh OLD NEW" >&2
  exit 2
fi
for g in "$1" "$2"; do
  [ -x "$g" ] || { echo "error: $g is not an executable" >&2; exit 2; }
done

T=$(mktemp -d "${TMPDIR:-/tmp}/gem_compare_c.XXXXXX")
trap 'rm -rf "$T"' EXIT

files=$(ls examples/*.gem examples/*/*.gem examples/*/*/*.gem tests/broken/*.gem std/*.gem \
  compiler/main.gem lsp/main.gem 2>/dev/null)

emit() {
  gem=$1 dir=$2 f=$3
  out="$dir/$(echo "$f" | tr / _)"
  "$gem" "$f" --emit-c > "$out.c" 2> "$out.err"
  echo $? > "$out.status"
}
export -f emit
mkdir "$T/old" "$T/new"
printf '%s\n' $files | xargs -P 8 -I{} bash -c 'emit "$0" "$1" {}' "$1" "$T/old"
printf '%s\n' $files | xargs -P 8 -I{} bash -c 'emit "$0" "$1" {}' "$2" "$T/new"

n=$(printf '%s\n' $files | wc -l | tr -d ' ')
if diff -rq "$T/old" "$T/new" > "$T/diffs"; then
  echo "same output for $n files"
else
  echo "FAIL: different output for $(wc -l < "$T/diffs" | tr -d ' ') file(s) of $n:"
  sed "s|$T/old/||; s| and $T/new/.*||; s|^Files ||" "$T/diffs" | head -20
  first=$(head -1 "$T/diffs" | awk '{print $2, $4}')
  diff $first | head -20
  exit 1
fi
