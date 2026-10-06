#!/usr/bin/env bash
# A recursion frees what its calls allocate as they return: peak memory
# grows with the recursion's depth, not with its number of calls. Runs
# three tree recursions making a table per call (a million calls or so:
# returning a table, returning nil, and recursing through a fn value) and
# checks their output and peak resident set size.
#
# Run from the repo root: tests/check_recursion_memory.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi
if [ ! -x /usr/bin/time ]; then
  echo "recursion memory: skipped (no /usr/bin/time)"
  exit 0
fi

dir=$(mktemp -d)
trap 'rm -rf "$dir"' EXIT

# peak_kb <command...>: runs the command, output to $dir/out, and prints
# its peak RSS in KB.
peak_kb() {
  if [ "$(uname)" = Darwin ]; then
    /usr/bin/time -l "$@" > "$dir/out" 2> "$dir/time"
    awk '/maximum resident set size/ { print int($1 / 1024) }' "$dir/time"
  else
    /usr/bin/time -v "$@" > "$dir/out" 2> "$dir/time"
    awk -F: '/Maximum resident set size/ { print int($2) }' "$dir/time"
  fi
}

fails=0
check() {
  local name=$1 want_out=$2 limit_kb=$3 got_kb=$4
  local got_out
  got_out=$(cat "$dir/out")
  if [ "$want_out" != "$got_out" ]; then
    echo "FAIL: $name output"
    diff <(printf '%s\n' "$want_out") <(printf '%s\n' "$got_out")
    fails=$((fails + 1))
  fi
  if [ -z "$got_kb" ] || [ "$got_kb" -gt "$limit_kb" ]; then
    echo "FAIL: $name peak RSS ${got_kb:-?} KB, limit $limit_kb KB"
    fails=$((fails + 1))
  fi
}

# Without return resets this peaks near 600 MB.
cat > "$dir/count.gem" <<'GEM'
fn count(n)
  if n < 2
    return {v: n}
  end
  let a = count(n - 1)
  let b = count(n - 2)
  {v: a.v + b.v}
end
print(count(28).v)
GEM
"$GEM" "$dir/count.gem" -o "$dir/count" || exit 1
check count "317811" 65536 "$(peak_kb "$dir/count")"

# A recursion that returns nothing, through a non-tail call. Without
# return resets this peaks near 460 MB.
cat > "$dir/visit.gem" <<'GEM'
let total = {n: 0}
fn visit(n)
  let t = {v: n, s: "node {n}"}
  if n >= 2
    visit(n - 1)
    visit(n - 2)
  end
  total.n = total.n + t.v % 3
end
visit(27)
print(total.n)
GEM
"$GEM" "$dir/visit.gem" -o "$dir/visit" || exit 1
check visit "574924" 65536 "$(peak_kb "$dir/visit")"

# Recursion through a fn value called by a param named like a builtin.
cat > "$dir/apply.gem" <<'GEM'
fn apply(push, x)
  push(x)
end
fn deep(n)
  if n < 2
    return {v: n}
  end
  let a = apply(deep, n - 1)
  let b = apply(deep, n - 2)
  {v: a.v + b.v}
end
print(deep(28).v)
GEM
"$GEM" "$dir/apply.gem" -o "$dir/apply" || exit 1
check apply "317811" 65536 "$(peak_kb "$dir/apply")"

if [ "$fails" -ne 0 ]; then
  exit 1
fi
echo "recursion memory: ok"
