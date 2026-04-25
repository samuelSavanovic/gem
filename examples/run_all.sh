#!/usr/bin/env bash
# Run all Gem examples and diff against expected output.
# Usage: ./examples/run_all.sh
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(dirname "$SCRIPT_DIR")"
EXPECTED="$SCRIPT_DIR/expected_output.txt"
RUNTIME_DIR="$ROOT/runtime"
COMPILER="$ROOT/build/gem"

if [[ ! -f "$COMPILER" ]]; then
    echo "error: build/gem not found — run 'make build' first" >&2
    exit 1
fi

actual=$( for f in "$SCRIPT_DIR"/[0-9]*.gem; do
    base=$(basename "$f" .gem)
    c_file="/tmp/gem_${base}.c"
    bin_file="/tmp/gem_${base}"
    "$COMPILER" "$f" --emit-c > "$c_file"
    cc -o "$bin_file" "$c_file" "$RUNTIME_DIR/gem.c" -I "$RUNTIME_DIR" -std=c11 -O2 \
        $(pkg-config --cflags --libs bdw-gc)
    "$bin_file" 2>&1
done )

if diff -u "$EXPECTED" <(echo "$actual") > /dev/null 2>&1; then
    echo "ALL EXAMPLES PASSED"
    exit 0
else
    echo "MISMATCH — diff follows:"
    diff -u "$EXPECTED" <(echo "$actual")
    exit 1
fi
