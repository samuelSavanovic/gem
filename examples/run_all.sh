#!/usr/bin/env bash
# Run all Gem examples and diff against expected output.
# Usage: ./examples/run_all.sh
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(dirname "$SCRIPT_DIR")"
EXPECTED="$SCRIPT_DIR/expected_output.txt"
COMPILER="$ROOT/build/gem"

if [[ ! -f "$COMPILER" ]]; then
    echo "error: build/gem not found — run 'make build' first" >&2
    exit 1
fi

cd "$ROOT"
BIN_DIR=$(mktemp -d "${TMPDIR:-/tmp}/gem_examples.XXXXXX")
trap 'rm -rf "$BIN_DIR"' EXIT
# Numeric order (09, 10, ..., 99, 100), independent of the locale's collation.
examples=$(printf '%s\n' examples/[0-9]*.gem | LC_ALL=C sort -t/ -k2,2n)
# NUL bytes are dropped (90_bytes_extern writes some): a shell variable
# can't hold them.
actual=$( for f in $examples; do
    bin="$BIN_DIR/$(basename "$f" .gem)"
    "$COMPILER" "$f" -o "$bin" 2>/dev/null
    { "$bin" 2>&1 || true; } | tr -d '\000'
done )

if diff -u "$EXPECTED" <(echo "$actual") > /dev/null 2>&1; then
    echo "ALL EXAMPLES PASSED"
    exit 0
else
    echo "MISMATCH — diff follows:"
    diff -u "$EXPECTED" <(echo "$actual")
    exit 1
fi
