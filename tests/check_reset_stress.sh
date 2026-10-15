#!/usr/bin/env bash
# Reset stress mode, quickly (part of make test; make test-stress runs
# everything under it):
#   - 117_arena_reset_pressure prints the same under GEM_RESET_STRESS=1 and
#     =1024 as without;
#   - loops that end right after a reset, with no enclosing loop to free
#     what they leave (a sort comparator's, called from top-level code),
#     leave no memory mappings behind when their resets copy nothing (a
#     mapping kept per reset runs a program like 117 out of
#     Linux's vm.max_map_count at a 1 KB threshold);
#   - a bad GEM_RESET_STRESS value is refused at startup.
#
# Run from the repo root: tests/check_reset_stress.sh

set -u
cd "$(dirname "$0")/.."
GEM=build/gem
T=$(mktemp -d "${TMPDIR:-/tmp}/gem_rstress.XXXXXX")
trap 'rm -rf "$T"' EXIT
fails=0

"$GEM" examples/117_arena_reset_pressure.gem -o "$T/p117" || exit 1
"$T/p117" > "$T/plain.out" 2>&1
for n in 1 1024; do
  GEM_RESET_STRESS=$n "$T/p117" > "$T/stress.out" 2>&1
  if ! cmp -s "$T/plain.out" "$T/stress.out"; then
    echo "FAIL: 117_arena_reset_pressure differs under GEM_RESET_STRESS=$n:"
    diff "$T/plain.out" "$T/stress.out" | head -10
    fails=$((fails + 1))
  fi
done

cat > "$T/cmp.gem" <<'GEM'
fn junk(i)
  let s = "garbage-{i}-" + to_string(i * 7)
  [s, s + "!", {k: s}]
end
fn heavy_cmp(a, b)
  for i = 0, 3
    junk(i)
  end
  a - b
end
let nums = []
for i = 0, 3000
  push(nums, (i * 7919) % 3001)
end
let before = runtime_stats().memory
sort(nums, heavy_cmp)
let grown = runtime_stats().memory - before
print(nums[0], nums[2999], runtime_stats().resets > 10000)
if grown > 16 * 1024 * 1024
  print("grew by {grown} bytes")
end
GEM
"$GEM" "$T/cmp.gem" -o "$T/cmp" || exit 1
out=$(GEM_RESET_STRESS=1 "$T/cmp" 2>&1)
if [ "$out" != "0 3000 true" ]; then
  echo "FAIL: sort with a looping comparator under GEM_RESET_STRESS=1:"
  echo "$out"
  fails=$((fails + 1))
fi

for bad in 0 -5 abc 12k; do
  out=$(GEM_RESET_STRESS=$bad "$T/cmp" 2>&1)
  st=$?
  if [ $st -ne 2 ] || [ "$out" != "gem: GEM_RESET_STRESS must be a positive byte count, got '$bad'" ]; then
    echo "FAIL: GEM_RESET_STRESS=$bad: exit $st, $out"
    fails=$((fails + 1))
  fi
done

if [ $fails -eq 0 ]; then
  echo "reset stress: OK"
else
  exit 1
fi
