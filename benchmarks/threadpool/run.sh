#!/usr/bin/env bash
# Sweeps the I/O thread pool's size (GEM_POOL_SIZE) over bench.gem's
# loads, 3 runs each, and times an empty program's start at each size.
#
#   benchmarks/threadpool/run.sh                  # sizes 1 2 4 8 16 32 64
#   SIZES="4 16" REPS=5 benchmarks/threadpool/run.sh
set -euo pipefail
cd "$(dirname "$0")"
ROOT=../..
SIZES=${SIZES:-"1 2 4 8 16 32 64"}
REPS=${REPS:-3}
T=$(mktemp -d "${TMPDIR:-/tmp}/gem_threadpool.XXXXXX")
trap 'rm -rf "$T"' EXIT

"$ROOT/build/gem" bench.gem -o "$T/bench"
printf 'fn main()\n  nil\nend\n' > "$T/empty.gem"
"$ROOT/build/gem" "$T/empty.gem" -o "$T/empty"

echo "machine: $(uname -sm), $(getconf _NPROCESSORS_ONLN) cores"
for size in $SIZES; do
  for scenario in reads "hol 4" "hol 16" mixed; do
    for rep in $(seq "$REPS"); do
      # shellcheck disable=SC2086 # scenario is a command and its argument
      echo "size=$size $(GEM_POOL_SIZE=$size "$T/bench" $scenario | tr '\n' ' ')"
    done
  done
  start=$(date +%s%N)
  for _ in $(seq 100); do GEM_POOL_SIZE=$size "$T/empty"; done
  echo "size=$size empty_start_us=$(( ($(date +%s%N) - start) / 100000 ))"
done
