#!/usr/bin/env bash
# Check the driver's one-line errors for a source path it cannot read:
# exact stderr, empty stdout, exit status 1, no stack trace. Then run
# mode: paths and arguments with spaces and quotes, the work directory;
# and the flags and environment that reach cc.
#
# Run from the repo root: tests/check_cli.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi

fails=0
total=0

# check <expected stderr> <expected status> <gem args...>
check() {
  local want_err=$1 want_status=$2
  shift 2
  total=$((total + 1))
  local out err status
  err=$("$GEM" "$@" 2>&1 >/dev/null)
  out=$("$GEM" "$@" 2>/dev/null)
  status=$?
  if [ "$err" != "$want_err" ] || [ "$status" != "$want_status" ] || [ -n "$out" ]; then
    echo "FAIL: gem $*"
    echo "  want status $want_status, stderr: $want_err"
    echo "  got  status $status, stderr: $err"
    [ -n "$out" ] && echo "  stdout: $out"
    fails=$((fails + 1))
  fi
}

check "gem: cannot open 'tests/no_such_file.gem'" 1 tests/no_such_file.gem
check "gem: cannot open 'tests/no_such_file.gem'" 1 --check tests/no_such_file.gem
check "gem: cannot open 'tests/no_such_dir/x.gem'" 1 tests/no_such_dir/x.gem -o /dev/null
check "gem: 'tests' is a directory, not a source file" 1 tests
check "gem: 'tests/' is a directory, not a source file" 1 --emit-c tests/

# Run mode: the source path and each argument reach cc and the program as
# one word, the program's exit status is gem's, and the work directory
# under $TMPDIR is gone afterwards; when cc fails it stays, holding the C
# file gem names.
tmp=$(mktemp -d "${TMPDIR:-/tmp}/gem_cli.XXXXXX")
trap 'rm -rf "$tmp"' EXIT
mkdir "$tmp/a b" "$tmp/work"
printf 'let a = argv()\nprint("{a[1]}|{a[2]}|{a[3]}|{len(a)}")\nexit(3)\n' > "$tmp/a b/it's.gem"
total=$((total + 1))
out=$(TMPDIR="$tmp/work" "$GEM" "$tmp/a b/it's.gem" 'x y' '$(echo no)' "q'uote" 2>&1)
status=$?
left=$(ls -A "$tmp/work")
if [ "$out" != "x y|\$(echo no)|q'uote|4" ] || [ "$status" != 3 ] || [ -n "$left" ]; then
  echo "FAIL: run mode with spaces and quotes"
  echo "  want status 3, stdout: x y|\$(echo no)|q'uote|4, an empty work directory"
  echo "  got  status $status, stdout: $out, work directory: $left"
  fails=$((fails + 1))
fi
printf 'extern fn gem_no_such_fn() -> Int\nprint(gem_no_such_fn())\n' > "$tmp/a b/bad.gem"
total=$((total + 1))
err=$(TMPDIR="$tmp/work" "$GEM" "$tmp/a b/bad.gem" 2>&1 >/dev/null)
status=$?
c_file=${err##*the C is in }
if [ "$status" != 1 ] || [ "$c_file" = "$err" ] || [ ! -f "$c_file" ] || [ "${c_file#"$tmp/work/"}" = "$c_file" ]; then
  echo "FAIL: cc failure keeps the C file"
  echo "  want status 1, a last line naming the C file under $tmp/work"
  echo "  got  status $status, stderr: $err"
  fails=$((fails + 1))
fi

# C compiler flags: -I, -L and -l before the source file (attached or
# not), CFLAGS and LDFLAGS from the environment, and CC; a library the
# program needs and isn't given fails in cc. After the source file, -l is
# the program's.
mkdir -p "$tmp/c/include" "$tmp/c/lib"
printf '#include <stdint.h>\nint64_t gemcli_add(int64_t a, int64_t b);\n' > "$tmp/c/include/gemcli.h"
printf '#include <stdint.h>\nint64_t gemcli_add(int64_t a, int64_t b) { return a + b + GEMCLI_EXTRA; }\n' > "$tmp/c/gemcli.c"
cc -c -DGEMCLI_EXTRA=0 -o "$tmp/c/gemcli.o" "$tmp/c/gemcli.c" && ar rcs "$tmp/c/lib/libgemcli.a" "$tmp/c/gemcli.o"
printf 'extern include "gemcli.h"\nextern fn gemcli_add(a: Int, b: Int) -> Int\nprint(gemcli_add(2, 3))\n' > "$tmp/c/use.gem"
printf 'print(argv()[1])\n' > "$tmp/c/args.gem"
printf '#!/bin/sh\ntouch "%s/c/cc_used"\nexec cc "$@"\n' "$tmp" > "$tmp/c/mycc"
chmod +x "$tmp/c/mycc"

# run_check <name> <want stdout> <want status> <command...>
run_check() {
  local name=$1 want_out=$2 want_status=$3
  shift 3
  total=$((total + 1))
  local out status
  out=$("$@" 2>/dev/null)
  status=$?
  if [ "$out" != "$want_out" ] || [ "$status" != "$want_status" ]; then
    echo "FAIL: $name"
    echo "  want status $want_status, stdout: $want_out"
    echo "  got  status $status, stdout: $out"
    fails=$((fails + 1))
  fi
}
run_check "-I -L -l" 5 0 "$GEM" -I "$tmp/c/include" -L "$tmp/c/lib" -l gemcli "$tmp/c/use.gem"
run_check "-I<dir> -L<dir> -l<lib>" 5 0 "$GEM" "-I$tmp/c/include" "-L$tmp/c/lib" -lgemcli "$tmp/c/use.gem"
run_check "no -l: cc fails" "" 1 "$GEM" -I "$tmp/c/include" "$tmp/c/use.gem"
run_check "CFLAGS and LDFLAGS" 5 0 env CFLAGS="-I$tmp/c/include" LDFLAGS="-L$tmp/c/lib -lgemcli" "$GEM" "$tmp/c/use.gem"
run_check "CC" 5 0 env CC="$tmp/c/mycc" "$GEM" "-I$tmp/c/include" "-L$tmp/c/lib" -lgemcli "$tmp/c/use.gem"
total=$((total + 1))
if [ ! -f "$tmp/c/cc_used" ]; then
  echo "FAIL: CC names the C compiler"
  fails=$((fails + 1))
fi
run_check "-l after the source is the program's" "-lgemcli" 0 "$GEM" "$tmp/c/args.gem" -lgemcli
check "gem: -l requires an argument
usage: gem <file.gem> [options] [args...]   (gem --help for more)" 2 -l
check "gem: -L requires an argument
usage: gem <file.gem> [options] [args...]   (gem --help for more)" 2 -L ""

if [ "$fails" -gt 0 ]; then
  echo
  echo "$fails / $total CLI checks failed"
  exit 1
fi
echo "all $total CLI checks passed"
