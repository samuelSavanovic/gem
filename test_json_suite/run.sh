#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
SUITE_DIR="${1:-/tmp/JSONTestSuite/test_parsing}"
GEM="$PROJECT_DIR/build/gem"
TEST_SRC="$SCRIPT_DIR/test_file.gem"

if [ ! -d "$SUITE_DIR" ]; then
    echo "Cloning JSONTestSuite..."
    git clone --depth 1 https://github.com/nst/JSONTestSuite /tmp/JSONTestSuite
fi

echo "Compiling test harness..."
BINARY="/tmp/gem_test_json_suite"
$GEM $TEST_SRC -o "$BINARY"

y_pass=0; y_fail=0; y_total=0
n_pass=0; n_fail=0; n_total=0
i_pass=0; i_fail=0; i_total=0

y_failures=""
n_failures=""

for f in "$SUITE_DIR"/*.json; do
    name=$(basename "$f")
    prefix=${name:0:2}

    output=$("$BINARY" "$f" 2>/dev/null) || true
    parsed_ok=false
    if echo "$output" | grep -q "^PASS$"; then
        parsed_ok=true
    fi

    case "$prefix" in
        y_)
            y_total=$((y_total + 1))
            if $parsed_ok; then
                y_pass=$((y_pass + 1))
            else
                y_fail=$((y_fail + 1))
                y_failures="$y_failures  FAIL (should accept): $name\n"
            fi
            ;;
        n_)
            n_total=$((n_total + 1))
            if $parsed_ok; then
                n_fail=$((n_fail + 1))
                n_failures="$n_failures  FAIL (should reject): $name\n"
            else
                n_pass=$((n_pass + 1))
            fi
            ;;
        i_)
            i_total=$((i_total + 1))
            if $parsed_ok; then
                i_pass=$((i_pass + 1))
            else
                i_fail=$((i_fail + 1))
            fi
            ;;
    esac
done

echo ""
echo "=== JSONTestSuite Results ==="
echo ""
echo "y_ (must accept):  $y_pass/$y_total passed"
echo "n_ (must reject):  $n_pass/$n_total passed"
echo "i_ (impl-defined): $i_pass accepted, $i_fail rejected (out of $i_total)"
echo ""

if [ -n "$y_failures" ]; then
    echo "--- Must-accept failures ---"
    echo -e "$y_failures"
fi

if [ -n "$n_failures" ]; then
    echo "--- Must-reject failures ---"
    echo -e "$n_failures"
fi

total_correct=$((y_pass + n_pass))
total_required=$((y_total + n_total))
echo "Overall (required): $total_correct/$total_required"
