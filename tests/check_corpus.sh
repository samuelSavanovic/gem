#!/usr/bin/env bash
# Run `gem --check` over the in-repo Gem corpus (examples, std, compiler).
# Every file must succeed silently; any non-zero exit is a regression in the
# parser/codegen sink-discipline introduced in PR #8.
#
# Run from the repo root: tests/check_corpus.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
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
