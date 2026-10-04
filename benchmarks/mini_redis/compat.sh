#!/usr/bin/env bash
# Runs compat_commands.txt through redis-cli against a real redis-server
# and against examples/mini_redis, and diffs the two outputs.
#
#   benchmarks/mini_redis/compat.sh
#
# Needs redis-server and redis-cli on PATH (`brew install redis`).

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
REDIS_PORT=${REDIS_PORT:-6390}
GEM_PORT=${GEM_PORT:-6391}
WORK="$(mktemp -d /tmp/gem_mini_redis_compat.XXXXXX)"
pids=()
cleanup() {
  for p in "${pids[@]}"; do kill "$p" 2> /dev/null || true; done
  rm -rf "$WORK"
}
trap cleanup EXIT

for tool in redis-server redis-cli; do
  command -v "$tool" > /dev/null || { echo "$tool not found" >&2; exit 1; }
done
"$ROOT/build/gem" "$ROOT/examples/mini_redis/main.gem" -o "$WORK/mini_redis"

redis-server --port "$REDIS_PORT" --save "" --appendonly no > "$WORK/redis.log" 2>&1 &
pids+=($!)
"$WORK/mini_redis" --port "$GEM_PORT" > "$WORK/gem.log" 2>&1 &
pids+=($!)
for port in "$REDIS_PORT" "$GEM_PORT"; do
  for _ in $(seq 50); do
    redis-cli -p "$port" ping > /dev/null 2>&1 && break
    sleep 0.1
  done
done

redis-cli -p "$REDIS_PORT" < "$SCRIPT_DIR/compat_commands.txt" > "$WORK/redis.out" 2>&1
redis-cli -p "$GEM_PORT" < "$SCRIPT_DIR/compat_commands.txt" > "$WORK/gem.out" 2>&1
if diff -u "$WORK/redis.out" "$WORK/gem.out"; then
  echo "compat: $(grep -c . "$SCRIPT_DIR/compat_commands.txt") commands, same replies as redis-server"
else
  echo "compat: replies differ (- redis-server, + mini_redis)"
  exit 1
fi
