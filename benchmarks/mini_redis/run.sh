#!/usr/bin/env bash
# Benchmarks examples/mini_redis against a real redis-server with
# redis-benchmark and pubsub_bench.py, and prints the two side by side.
#
#   benchmarks/mini_redis/run.sh            # all phases
#   PHASES="basic pipeline" benchmarks/mini_redis/run.sh
#
# Phases:
#   basic     redis-benchmark's own tests, 50 clients, one command at a time
#   pipeline  the same with 16 commands in flight per client (-P 16)
#   clients   SET/GET with 1000 clients (a process per connection)
#   keyspace  1M keys of 100 bytes, then GET over them; RSS after the fill
#   expire    1M keys with a 2 s TTL; how many are left, and RSS, every 3 s
#   pubsub    fan-out to 1, 100 and 1000 subscribers (pubsub_bench.py)
#
# Env: N (requests per test, default 100000), CLIENTS (50), REDIS_PORT
# (6390), GEM_PORT (6391), OUT (results dir, default
# benchmarks/mini_redis/logs/<timestamp>). Needs redis-server,
# redis-benchmark, redis-cli and python3 (`brew install redis`). The Gem
# server runs with GEM_DIAG=1; its arena statistics are in gem.log.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
N=${N:-100000}
CLIENTS=${CLIENTS:-50}
REDIS_PORT=${REDIS_PORT:-6390}
GEM_PORT=${GEM_PORT:-6391}
PHASES=${PHASES:-"basic pipeline clients keyspace expire pubsub"}
OUT=${OUT:-"$SCRIPT_DIR/logs/$(date +%Y%m%d-%H%M%S)"}
TESTS=ping_inline,ping_mbulk,set,get,incr,lpush,rpush,lpop,rpop,sadd,hset,spop,lrange_100,lrange_600,mset

for tool in redis-server redis-benchmark redis-cli python3; do
  command -v "$tool" > /dev/null || { echo "$tool not found" >&2; exit 1; }
done
[[ -x "$ROOT/build/gem" ]] || { echo "build/gem not found: run 'make build'" >&2; exit 1; }
mkdir -p "$OUT"
"$ROOT/build/gem" "$ROOT/examples/mini_redis/main.gem" -o "$OUT/mini_redis"

{
  echo "date: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo "gem commit: $(git -C "$ROOT" rev-parse --short HEAD)$(git -C "$ROOT" diff --quiet || echo ' (dirty)')"
  echo "redis: $(redis-server --version)"
  echo "system: $(uname -srm)"
  if [[ "$(uname)" == Darwin ]]; then
    echo "cpu: $(sysctl -n machdep.cpu.brand_string), $(sysctl -n hw.ncpu) cores"
  else
    echo "cpu: $(grep -m1 'model name' /proc/cpuinfo | cut -d: -f2 | sed 's/^ //'), $(nproc) cores"
  fi
  echo "N=$N CLIENTS=$CLIENTS PHASES=$PHASES"
} > "$OUT/meta.txt"
cat "$OUT/meta.txt"

redis_pid=""
gem_pid=""
stop_servers() {
  [[ -n "$redis_pid" ]] && kill "$redis_pid" 2> /dev/null || true
  [[ -n "$gem_pid" ]] && kill "$gem_pid" 2> /dev/null || true
  wait 2> /dev/null || true
  redis_pid=""
  gem_pid=""
}
trap stop_servers EXIT

wait_up() {
  for _ in $(seq 100); do
    redis-cli -p "$1" ping > /dev/null 2>&1 && return 0
    sleep 0.1
  done
  echo "server on port $1 did not start" >&2
  exit 1
}

# Fresh servers for each phase, so one phase's keyspace doesn't skew the
# next one's numbers.
start_servers() {
  stop_servers
  for port in "$REDIS_PORT" "$GEM_PORT"; do
    if redis-cli -p "$port" ping > /dev/null 2>&1; then
      echo "port $port is already in use: stop that server first" >&2
      exit 1
    fi
  done
  redis-server --port "$REDIS_PORT" --save "" --appendonly no >> "$OUT/redis.log" 2>&1 &
  redis_pid=$!
  GEM_DIAG=1 "$OUT/mini_redis" --port "$GEM_PORT" >> "$OUT/gem.log" 2>&1 &
  gem_pid=$!
  wait_up "$REDIS_PORT"
  wait_up "$GEM_PORT"
  if ! kill -0 "$redis_pid" 2> /dev/null || ! kill -0 "$gem_pid" 2> /dev/null; then
    echo "a server exited at startup: see $OUT/redis.log and $OUT/gem.log" >&2
    exit 1
  fi
}

# Stops the Gem server with SHUTDOWN, so it prints its GEM_DIAG statistics.
stop_gem() {
  echo "--- end of phase $1" >> "$OUT/gem.log"
  redis-cli -p "$GEM_PORT" shutdown > /dev/null 2>&1 || true
  wait "$gem_pid" 2> /dev/null || true
  gem_pid=""
}

rss_kb() {
  ps -o rss= -p "$1" | tr -d ' '
}

# bench <phase> <redis-benchmark args...>: runs the same benchmark against
# both servers and writes <phase>.redis.csv and <phase>.gem.csv.
bench() {
  local phase=$1
  shift
  redis-benchmark -p "$REDIS_PORT" --csv "$@" > "$OUT/$phase.redis.csv" 2> /dev/null
  redis-benchmark -p "$GEM_PORT" --csv "$@" > "$OUT/$phase.gem.csv" 2> /dev/null
  python3 "$SCRIPT_DIR/compare.py" "$phase" "$OUT/$phase.redis.csv" "$OUT/$phase.gem.csv" | tee -a "$OUT/summary.txt"
}

# memory <label>: prints both servers' RSS and DBSIZE.
memory() {
  local line
  line=$(printf "%-28s redis %8d KB (%s keys)   gem %8d KB (%s keys)" "$1" \
    "$(rss_kb "$redis_pid")" "$(redis-cli -p "$REDIS_PORT" dbsize)" \
    "$(rss_kb "$gem_pid")" "$(redis-cli -p "$GEM_PORT" dbsize)")
  echo "$line" | tee -a "$OUT/summary.txt"
}

for phase in $PHASES; do
  echo | tee -a "$OUT/summary.txt"
  start_servers
  case $phase in
    basic)
      bench basic -q -n "$N" -c "$CLIENTS" -r 100000 -t "$TESTS"
      memory "after basic"
      ;;
    pipeline)
      bench pipeline -q -n "$N" -c "$CLIENTS" -r 100000 -P 16 -t "$TESTS"
      memory "after pipeline"
      ;;
    clients)
      bench clients -q -n "$N" -c 1000 -r 100000 -t set,get
      memory "after 1000 clients"
      ;;
    keyspace)
      bench keyspace_fill -q -n 1000000 -c "$CLIENTS" -r 1000000 -d 100 -P 16 -t set
      memory "1M keys of 100 B"
      bench keyspace_get -q -n "$N" -c "$CLIENTS" -r 1000000 -t get
      ;;
    expire)
      for port in "$REDIS_PORT" "$GEM_PORT"; do
        redis-benchmark -p "$port" -q -n 1000000 -c "$CLIENTS" -r 1000000 -P 16 \
          set key:__rand_int__ value ex 2 > /dev/null 2>&1
      done
      memory "1M keys with a 2 s TTL"
      for t in 3 6 9; do
        sleep 3
        memory "$t s later"
      done
      ;;
    pubsub)
      echo "pubsub (subscribers,messages,payload,publish/s,delivered/s,seconds)" | tee -a "$OUT/summary.txt"
      for spec in "1 50000" "100 2000" "1000 200"; do
        set -- $spec
        echo "  redis $(python3 "$SCRIPT_DIR/pubsub_bench.py" "$REDIS_PORT" "$1" "$2")" | tee -a "$OUT/summary.txt"
        echo "  gem   $(python3 "$SCRIPT_DIR/pubsub_bench.py" "$GEM_PORT" "$1" "$2")" | tee -a "$OUT/summary.txt"
      done
      ;;
    *)
      echo "unknown phase: $phase" >&2
      exit 1
      ;;
  esac
  stop_gem "$phase"
done
echo
echo "results in $OUT"
