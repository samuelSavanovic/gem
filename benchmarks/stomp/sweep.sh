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

# Compile broker once up-front so each cell can run the binary directly.
# Running the binary (instead of `gem broker.gem`) means $bpid IS the broker
# process — `wait $bpid` then captures its exit status / signal cleanly,
# and atexit handlers in the broker reach broker.log without the gem
# launcher's diag line muddying the output.
BROKER_BIN="$RUN_DIR/stomp_broker"
echo "compiling broker → $BROKER_BIN"
"$GEM_BIN" -o "$BROKER_BIN" "$BROKER_SRC" 2>&1 | tail -5
[[ -x "$BROKER_BIN" ]] || { echo "broker compile failed"; exit 1; }

cleanup_port() {
  lsof -ti :$PORT 2>/dev/null | xargs -r kill -9 2>/dev/null || true
  sleep 1
}

run_cell() {
  local label="$1" subs="$2" msgs="$3" body="$4" pause="$5"
  cleanup_port
  GEM_DIAG=1 "$BROKER_BIN" > "$RUN_DIR/${label}.broker.log" 2>&1 &
  local bpid=$!
  for _ in 1 2 3 4 5 6 7 8 9 10; do
    nc -z localhost $PORT 2>/dev/null && break
    sleep 0.2
  done
  sleep 0.5

  # RSS sampler — runs while broker is alive, writes CSV next to broker log.
  local rss_csv="$RUN_DIR/${label}.rss.csv"
  (
    echo "elapsed_s,rss_kb"
    start=$(date +%s)
    while kill -0 "$bpid" 2>/dev/null; do
      now=$(date +%s)
      rss=$(ps -o rss= -p "$bpid" 2>/dev/null | tr -d ' ') || break
      [[ -n "$rss" ]] && echo "$((now - start)),${rss}"
      sleep 0.5
    done
  ) > "$rss_csv" &
  local rss_pid=$!

  set +e
  "$PYTHON" "$HARNESS" fanout \
    --n-subs "$subs" --n-msgs "$msgs" --body-size "$body" \
    --publish-pause-s "$pause" \
    > "$RUN_DIR/${label}.json" 2> "$RUN_DIR/${label}.harness.err"
  local rc=$?
  set -e

  local alive="yes"
  local exit_status=""
  if ! kill -0 $bpid 2>/dev/null; then
    alive="DIED"
    set +e
    wait $bpid
    local wstatus=$?
    set -e
    if (( wstatus > 128 )); then
      local sig=$(( wstatus - 128 ))
      local signame
      signame=$(kill -l $sig 2>/dev/null || echo $sig)
      exit_status="signal=$sig($signame)"
    else
      exit_status="exit=$wstatus"
    fi
  fi
  kill -9 $bpid 2>/dev/null || true
  kill $rss_pid 2>/dev/null || true
  wait $rss_pid 2>/dev/null || true

  # RSS summary: peak + final sample (informative when broker dies mid-cell).
  local rss_summary
  rss_summary=$(awk -F, 'NR<=1{next}
    NR==2{first=$2}
    {if($2>peak)peak=$2; last=$2}
    END {
      if (NR<=1) { print "rss=(no samples)" }
      else printf "rss start=%d peak=%d end=%d kB", first, peak, last
    }' "$rss_csv" 2>/dev/null)

  # gem_diag line (atexit) — captured into broker.log. Only present when
  # the broker exited normally (atexit doesn't fire on signal-kill).
  local diag_line
  diag_line=$(grep -m1 '^gem_diag:' "$RUN_DIR/${label}.broker.log" 2>/dev/null || echo "")

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
  printf '  %-30s subs=%4s msgs=%4s body=%-5s pause=%5s  %s  broker=%s  %s  %s  %s\n' \
    "$label" "$subs" "$msgs" "$body" "$pause" "$summary" "$alive" "$exit_status" "$rss_summary" "$diag_line"
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
