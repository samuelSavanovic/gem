#!/usr/bin/env bash
# Builds and runs the numbered examples and compares each one's output (stdout and stderr, NUL bytes
# dropped) with examples/expected/<name>.out. A non-zero exit is fine; a compile error, a timeout or a
# death by signal fails the example.
#
#   examples/run_all.sh [-j N] [--update] [name...]
#
# A name is a glob over example names without .gem ('2*', '*http*'); a bare number picks that example
# ('109'). --update writes the actual output of the named examples to their expected files.
# GEM picks the compiler; CFLAGS, GEM_RUNTIME and GEM_RESET_STRESS pass through to it and the program.
# EXAMPLE_TIMEOUT (seconds, default 120) bounds each run.
set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(dirname "$SCRIPT_DIR")"
EXPECTED_DIR="$SCRIPT_DIR/expected"
COMPILER="${GEM:-$ROOT/build/gem}"
case $COMPILER in /*) ;; *) COMPILER="$PWD/$COMPILER" ;; esac
TIMEOUT="${EXAMPLE_TIMEOUT:-120}"

usage() {
    echo "usage: $0 [-j N] [--update] [name...]" >&2
    exit 2
}

jobs=$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)
update=0
patterns=()
while [ $# -gt 0 ]; do
    case $1 in
        -j) [ $# -ge 2 ] || usage; jobs=$2; shift 2 ;;
        -j*) jobs=${1#-j}; shift ;;
        --update) update=1; shift ;;
        -h|--help) usage ;;
        -*) usage ;;
        *) patterns+=("$1"); shift ;;
    esac
done
case $jobs in ''|*[!0-9]*|0) usage ;; esac
if [ "$update" = 1 ] && [ ${#patterns[@]} -eq 0 ]; then
    echo "error: --update needs the names of the examples to update" >&2
    exit 2
fi

if [[ ! -f "$COMPILER" ]]; then
    echo "error: $COMPILER not found, run 'make build' first" >&2
    exit 1
fi

cd "$ROOT"
selected=()
for f in $(printf '%s\n' examples/[0-9]*.gem | LC_ALL=C sort -t/ -k2,2n); do
    name=$(basename "$f" .gem)
    if [ ${#patterns[@]} -eq 0 ]; then
        selected+=("$name")
        continue
    fi
    for p in "${patterns[@]}"; do
        case $p in *[!0-9]*) ;; *) p="${p}_*" ;; esac
        # shellcheck disable=SC2053
        if [[ $name == $p ]]; then
            selected+=("$name")
            break
        fi
    done
done
if [ ${#selected[@]} -eq 0 ]; then
    echo "error: no example matches ${patterns[*]}" >&2
    exit 2
fi

WORK=$(mktemp -d "${TMPDIR:-/tmp}/gem_examples.XXXXXX")
trap 'rm -rf "$WORK"' EXIT
export COMPILER WORK TIMEOUT

# writes $WORK/<name>.out (the output) and $WORK/<name>.result: "ok", or why the example failed
run_one() {
    local name=$1 bin="$WORK/$1" status
    if ! "$COMPILER" "examples/$name.gem" -o "$bin" > "$WORK/$name.cc" 2>&1; then
        echo "does not compile" > "$WORK/$name.result"
        return
    fi
    # perl's alarm survives the exec, and the shell sees the program die of SIGALRM
    perl -e 'alarm shift; exec @ARGV or die "exec: $!\n"' "$TIMEOUT" "$bin" 2>&1 \
        | tr -d '\000' > "$WORK/$name.out"
    status=${PIPESTATUS[0]}
    if [ "$status" = 142 ]; then
        echo "timed out after ${TIMEOUT}s" > "$WORK/$name.result"
    elif [ "$status" -gt 128 ]; then
        echo "killed by signal $((status - 128))" > "$WORK/$name.result"
    else
        echo ok > "$WORK/$name.result"
    fi
}
export -f run_one

printf '%s\n' "${selected[@]}" | xargs -P "$jobs" -I{} bash -c 'run_one "$1"' _ {}

failed=()
for name in "${selected[@]}"; do
    result=$(cat "$WORK/$name.result" 2>/dev/null || echo "did not run")
    expected="$EXPECTED_DIR/$name.out"
    if [ "$result" = "does not compile" ]; then
        echo "FAIL $name: does not compile"
        sed 's/^/    /' "$WORK/$name.cc"
        failed+=("$name")
        continue
    fi
    if [ "$update" = 1 ] && [ "$result" = ok ]; then
        mkdir -p "$EXPECTED_DIR"
        if ! cmp -s "$WORK/$name.out" "$expected"; then
            cp "$WORK/$name.out" "$expected"
            echo "updated $name"
        fi
        continue
    fi
    if [ "$result" != ok ]; then
        echo "FAIL $name: $result"
        failed+=("$name")
    elif [ ! -f "$expected" ]; then
        echo "FAIL $name: no examples/expected/$name.out (write it with --update $name)"
        failed+=("$name")
        continue
    elif cmp -s "$WORK/$name.out" "$expected"; then
        continue
    else
        echo "FAIL $name: output differs"
        failed+=("$name")
    fi
    [ -f "$expected" ] && diff -u --label expected --label actual "$expected" "$WORK/$name.out" | sed 's/^/    /'
done

if [ ${#patterns[@]} -eq 0 ]; then
    for f in "$EXPECTED_DIR"/*.out; do
        name=$(basename "$f" .out)
        [ -f "examples/$name.gem" ] || echo "note: examples/expected/$name.out has no example"
    done
fi

if [ ${#failed[@]} -ne 0 ]; then
    echo "${#failed[@]} of ${#selected[@]} examples failed: ${failed[*]}"
    exit 1
fi
if [ "$update" = 1 ]; then
    exit 0
fi
if [ ${#selected[@]} -eq 1 ]; then
    echo "1 example passed"
elif [ ${#patterns[@]} -eq 0 ]; then
    echo "ALL EXAMPLES PASSED"
else
    echo "${#selected[@]} examples passed"
fi
