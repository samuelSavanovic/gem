#!/usr/bin/env bash
# Run all Gem examples and diff against expected output.
# Usage: ./examples/run_all.sh
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(dirname "$SCRIPT_DIR")"
EXPECTED="$SCRIPT_DIR/expected_output.txt"

actual=$( for f in "$SCRIPT_DIR"/[0-9]*.gem; do
    python3 "$ROOT/bootstrap/gem.py" "$f" --run 2>&1
done )

if diff -u "$EXPECTED" <(echo "$actual") > /dev/null 2>&1; then
    echo "ALL EXAMPLES PASSED"
    exit 0
else
    echo "MISMATCH — diff follows:"
    diff -u "$EXPECTED" <(echo "$actual")
    exit 1
fi
