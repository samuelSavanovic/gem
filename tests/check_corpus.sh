#!/usr/bin/env bash
# `gem --check` must exit 0 on every .gem file in compiler/, std/ and examples/

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built, run 'make build' first" >&2
  exit 2
fi

shopt -s nullglob
fails=0
total=0
for f in compiler/*.gem std/*.gem examples/*.gem; do
  total=$((total + 1))
  if ! "$GEM" --check "$f" >/dev/null 2>&1; then
    echo "FAIL: $f"
    fails=$((fails + 1))
  fi
done

if [ "$fails" -gt 0 ]; then
  echo
  echo "$fails / $total files failed --check"
  exit 1
fi
echo "all $total files passed --check"
