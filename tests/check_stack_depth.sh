#!/usr/bin/env bash
# std/json's 1,000-level limits are reached before the stack runs out, in main and in a spawned
# process, in run mode (-O0) and in a binary built with -o (-O2).

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built, run 'make build' first" >&2
  exit 2
fi

dir=$(mktemp -d)
trap 'rm -rf "$dir"' EXIT

cat > "$dir/depth.gem" <<'GEM'
load "std/json"
load "std/string"

fn depth(n)
  if n == 0 then return 0 end
  1 + depth(n - 1)
end

fn checks(who)
  let nested = []
  let cur = nested
  for i = 0, 999
    let next = []
    push(cur, next)
    cur = next
  end
  let cyclic = {}
  cyclic.self = cyclic
  let deep = string.repeat("[", 1001) + string.repeat("]", 1001)
  [
    "{who} depth 15000 {(pcall depth(15000)).ok}",
    "{who} encode 1000 {(pcall json.encode(nested)).ok}",
    "{who} cyclic {(pcall json.encode(cyclic)).error}",
    "{who} parse 1001 {(pcall json.parse(deep)).error}",
  ]
end

for line in checks("main")
  print(line)
end
let me = self()
spawn(fn() send(me, checks("spawned")) end)
for line in receive()
  print(line)
end
GEM

want=''
for who in main spawned; do
  want+="$who depth 15000 true
$who encode 1000 true
$who cyclic json.encode: nesting deeper than 1000 levels (or a cyclic table)
$who parse 1001 json.parse: nesting deeper than 1000 levels at byte 1000
"
done
want=${want%$'\n'}

fails=0
got=$(GEM_CACHE=off "$GEM" "$dir/depth.gem" 2>&1)
if [ "$got" != "$want" ]; then
  echo "FAIL: run mode"
  diff <(printf '%s\n' "$want") <(printf '%s\n' "$got")
  fails=$((fails + 1))
fi
"$GEM" "$dir/depth.gem" -o "$dir/depth" || exit 1
got=$("$dir/depth" 2>&1)
if [ "$got" != "$want" ]; then
  echo "FAIL: built with -o"
  diff <(printf '%s\n' "$want") <(printf '%s\n' "$got")
  fails=$((fails + 1))
fi

if [ "$fails" -ne 0 ]; then
  exit 1
fi
echo "stack depth: ok"
