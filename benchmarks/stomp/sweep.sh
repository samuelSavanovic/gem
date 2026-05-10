#!/usr/bin/env bash
# Sweep across (n_subs, n_msgs) for the fanout workload.
#
# Each cell: fresh broker, 256-byte body, 50 ms pause between publishes
# so the destination has a chance to drain. Output: a one-line summary
# per cell on stdout, plus full JSON in logs/<run>/sweep_<i>.json.
#
# We expect cells with msgs * subs > ~25,000 to crash (see NOTES.md).

set -uo pipefail
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
LOGS_DIR="$SCRIPT_DIR/logs"
HARNESS="$SCRIPT_DIR/harness.py"
PYTHON="${PYTHON:-python3.14}"
GEM_BIN="$PROJECT_ROOT/build/gem"
BROKER_SRC="$PROJECT_ROOT/examples/stomp_broker/main.gem"
PORT=61613

RUN_ID="sweep_$(date +"%Y-%m-%d_%H-%M-%S")"
RUN_DIR="$LOGS_DIR/$RUN_ID"
mkdir -p "$RUN_DIR"
echo "logs: $RUN_DIR"

cleanup_port() {
  lsof -ti :$PORT 2>/dev/null | xargs -r kill -9 2>/dev/null || true
  sleep 1
}

run_cell() {
  local label="$1" subs="$2" msgs="$3" body="$4" pause="$5"
  cleanup_port
  "$GEM_BIN" "$BROKER_SRC" > "$RUN_DIR/${label}.broker.log" 2>&1 &
  local bpid=$!
  for _ in 1 2 3 4 5 6 7 8 9 10; do
    nc -z localhost $PORT 2>/dev/null && break
    sleep 0.2
  done
  sleep 0.5

  set +e
  "$PYTHON" "$HARNESS" fanout \
    --n-subs "$subs" --n-msgs "$msgs" --body-size "$body" \
    --publish-pause-s "$pause" \
    > "$RUN_DIR/${label}.json" 2> "$RUN_DIR/${label}.harness.err"
  local rc=$?
  set -e

  local alive="yes"
  kill -0 $bpid 2>/dev/null || alive="DIED"
  kill -9 $bpid 2>/dev/null || true

  # Parse summary
  local summary
  summary=$("$PYTHON" -c "
import json,sys
try:
  d=json.load(open('$RUN_DIR/${label}.json'))
  ok=d['delivered_msgs_total']==d['expected_total']
  print(f\"delivered={d['delivered_msgs_total']}/{d['expected_total']}  pub={d['publish_msgs_per_s']} msg/s  fanout={d['delivered_msgs_per_s']} msg/s  med1st={d['median_first_msg_latency_s']}s  p100={d['p100_first_msg_latency_s']}s  ok={ok}\")
except Exception as e:
  print(f'(no result: {e})')
")
  printf '  %-30s subs=%4s msgs=%4s body=%-5s pause=%5s  %s  broker=%s\n' \
    "$label" "$subs" "$msgs" "$body" "$pause" "$summary" "$alive"
}

echo "Sweep:"
run_cell s500x10x256   500   10  256 0.0
run_cell s500x20x256   500   20  256 0.0
run_cell s500x50x256   500   50  256 0.0
run_cell s500x50x1024  500   50 1024 0.0
run_cell s500x100x256  500  100  256 0.0
run_cell s100x100x256  100  100  256 0.0
run_cell s100x200x256  100  200  256 0.0
run_cell s200x100x256  200  100  256 0.0
run_cell s50x500x256    50  500  256 0.0
run_cell s50x500x1024   50  500 1024 0.0
run_cell s10x1000x256   10 1000  256 0.0

cleanup_port
echo "done"
