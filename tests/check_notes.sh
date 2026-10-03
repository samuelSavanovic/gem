#!/usr/bin/env bash
# Compare the compiler's stderr notes (e.g. module state written from a
# spawned process) against tests/notes/<name>.expected. Each entry compiles
# examples/<name>.gem with --check, once by relative and once by absolute
# path: both must print project-root-relative paths.
#
# Run from the repo root: tests/check_notes.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi

fails=0
total=0
for exp in tests/notes/*.expected; do
  name=$(basename "$exp" .expected)
  for src in "examples/$name.gem" "$(pwd)/examples/$name.gem"; do
    total=$((total + 1))
    if ! "$GEM" --check "$src" 2>&1 >/dev/null | diff -u "$exp" - >/dev/null; then
      echo "FAIL: notes for $src differ from $exp:"
      "$GEM" --check "$src" 2>&1 >/dev/null | diff -u "$exp" -
      fails=$((fails + 1))
    fi
  done
done

if [ "$fails" -gt 0 ]; then
  echo
  echo "$fails / $total note checks failed"
  exit 1
fi
echo "all $total note checks passed"
