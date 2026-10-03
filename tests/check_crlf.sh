#!/usr/bin/env bash
# A source file saved with CRLF line endings compiles and runs like the
# same file with LF endings: a CRLF reads as LF everywhere (strings,
# comments, triple-quoted strings), and line numbers match.
#
# Run from the repo root: tests/check_crlf.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi

dir=$(mktemp -d)
trap 'rm -rf "$dir"' EXIT

lf=$(cat <<'GEM'
# a comment
let b = "q"
let t = """
  hi
  there
  """
let s = 'one
two'
print(b, t, len(t), s, len(s))
error("x")
GEM
)
printf '%s\n' "$lf" > "$dir/lf.gem"
printf '%s\n' "$lf" | sed 's/$/\r/' > "$dir/crlf.gem"

fails=0
"$GEM" "$dir/lf.gem" -o "$dir/lf_bin" 2>"$dir/lf_err" || { echo "FAIL: LF file doesn't compile"; cat "$dir/lf_err"; exit 1; }
if ! "$GEM" "$dir/crlf.gem" -o "$dir/crlf_bin" 2>"$dir/crlf_err"; then
  echo "FAIL: CRLF file doesn't compile"
  cat "$dir/crlf_err"
  exit 1
fi
want=$("$dir/lf_bin" 2>&1 | sed 's/lf\.gem/X.gem/g')
got=$("$dir/crlf_bin" 2>&1 | sed 's/crlf\.gem/X.gem/g')
if [ "$want" != "$got" ]; then
  echo "FAIL: CRLF output differs from LF output"
  diff <(printf '%s\n' "$want") <(printf '%s\n' "$got")
  fails=1
fi
if [ "$fails" -gt 0 ]; then
  exit 1
fi
echo "crlf: ok"
