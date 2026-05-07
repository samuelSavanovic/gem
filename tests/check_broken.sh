#!/usr/bin/env bash
# Verify that intentionally malformed programs produce the expected number of
# errors. Each entry: file_basename:min:max — the compiler must emit at least
# `min` errors and no more than `max` (the upper bound catches cascade noise).
#
# Run from the repo root: tests/check_broken.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi

# basename : min : max
expected=(
  "bad_pattern:2:4"
  "cascade_braces:2:5"
  "double_typo:2:4"
  "missing_end:2:8"
  "missing_then_branches:1:4"
  "multi_undeclared:3:5"
  "undeclared:1:1"
  "unterminated_string:1:1"
)

fails=0
for entry in "${expected[@]}"; do
  name=${entry%%:*}; rest=${entry#*:}
  min=${rest%%:*}; max=${rest##*:}
  src="tests/broken/${name}.gem"
  if [ ! -f "$src" ]; then
    echo "FAIL: missing corpus file $src"
    fails=$((fails + 1))
    continue
  fi
  count=$("$GEM" --check "$src" 2>&1 | grep -c "^\[Compile Error\]")
  if [ "$count" -lt "$min" ] || [ "$count" -gt "$max" ]; then
    echo "FAIL: $name produced $count errors, expected [$min..$max]"
    fails=$((fails + 1))
  else
    echo "OK:   $name ($count errors, in [$min..$max])"
  fi
done

if [ "$fails" -gt 0 ]; then
  echo
  echo "$fails broken-program checks failed"
  exit 1
fi
echo
echo "all broken-program checks passed"
