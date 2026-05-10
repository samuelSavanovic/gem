#!/usr/bin/env bash
# Driver for the STOMP broker milestone-6 load tests.
#
# Starts the broker, samples its RSS in the background, runs each workload
# via harness.py from a separate process, and writes everything (stdout, RSS
# CSV, harness JSON) into benchmarks/stomp/logs/<timestamp>/.
#
# Each phase is a fresh broker — slow-consumer can wedge the broker and
# we don't want one phase poisoning the next.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
LOGS_DIR="$SCRIPT_DIR/logs"
HARNESS="$SCRIPT_DIR/harness.py"

PYTHON="${PYTHON:-python3.14}"
GEM_BIN="$PROJECT_ROOT/build/gem"
BROKER_SRC="$PROJECT_ROOT/examples/stomp_broker/main.gem"
PORT=61613

RUN_ID="$(date +"%Y-%m-%d_%H-%M-%S")"
RUN_DIR="$LOGS_DIR/$RUN_ID"
mkdir -p "$RUN_DIR"

BROKER_PID=""
RSS_PID=""

cleanup() {
  [[ -n "$RSS_PID" ]] && kill "$RSS_PID" 2>/dev/null || true
  [[ -n "$BROKER_PID" ]] && kill "$BROKER_PID" 2>/dev/null || true
  wait 2>/dev/null || true
}
trap cleanup EXIT

die() { echo "FATAL: $*" >&2; exit 1; }

[[ -x "$GEM_BIN" ]] || die "build/gem missing — run 'make build'"
[[ -f "$BROKER_SRC" ]] || die "broker source missing"
command -v "$PYTHON" >/dev/null || die "$PYTHON not found"

# Compile broker once up-front so each phase can run the binary directly.
# This (a) means $BROKER_PID IS the broker process — wait captures its
# own exit status / signal cleanly — and (b) lets atexit handlers
# (gem_diag) reach broker.log without the gem launcher muddying the output.
BROKER_BIN="$RUN_DIR/stomp_broker"
echo "compiling broker → $BROKER_BIN"
"$GEM_BIN" -o "$BROKER_BIN" "$BROKER_SRC" 2>&1 | tail -5
[[ -x "$BROKER_BIN" ]] || die "broker compile failed"

start_broker() {
  local label="$1"
  local stdout_log="$RUN_DIR/${label}.broker.log"
  local rss_csv="$RUN_DIR/${label}.rss.csv"

  if lsof -ti :$PORT >/dev/null 2>&1; then
    die "port $PORT already in use"
  fi

  GEM_DIAG=1 "$BROKER_BIN" > "$stdout_log" 2>&1 &
  BROKER_PID=$!
  echo "broker pid: $BROKER_PID (log: $stdout_log)"

  # Wait for broker to bind.
  local i=0
  while ! nc -z localhost $PORT 2>/dev/null; do
    i=$((i + 1))
    if [ $i -gt 60 ]; then
      die "broker didn't come up in 6s"
    fi
    sleep 0.1
  done
  echo "broker ready"

  # RSS sampler
  (
    echo "elapsed_s,rss_kb"
    start=$(date +%s)
    while kill -0 "$BROKER_PID" 2>/dev/null; do
      now=$(date +%s)
      rss=$(ps -o rss= -p "$BROKER_PID" 2>/dev/null | tr -d ' ') || break
      [[ -n "$rss" ]] && echo "$((now - start)),${rss}"
      sleep 0.5
    done
  ) > "$rss_csv" &
  RSS_PID=$!
}

stop_broker() {
  [[ -n "$RSS_PID" ]] && kill "$RSS_PID" 2>/dev/null || true
  RSS_PID=""
  # If the broker died on its own (signal or exit), reap its status before
  # we send SIGKILL on top — otherwise wait returns the SIGKILL we just sent
  # and we lose the original signal.
  BROKER_EXIT_STATUS=""
  if [[ -n "$BROKER_PID" ]]; then
    if kill -0 "$BROKER_PID" 2>/dev/null; then
      kill -9 "$BROKER_PID" 2>/dev/null || true
      wait "$BROKER_PID" 2>/dev/null
      BROKER_EXIT_STATUS="killed-by-driver"
    else
      set +e
      wait "$BROKER_PID" 2>/dev/null
      local wstatus=$?
      set -e
      if (( wstatus > 128 )); then
        local sig=$(( wstatus - 128 ))
        local signame
        signame=$(kill -l $sig 2>/dev/null || echo $sig)
        BROKER_EXIT_STATUS="signal=$sig($signame)"
      else
        BROKER_EXIT_STATUS="exit=$wstatus"
      fi
    fi
  fi
  BROKER_PID=""
  # Kill anything still bound to the port (orphaned gem temp binaries).
  for _ in 1 2 3 4 5 6 7 8 9 10; do
    holders=$(lsof -ti :$PORT 2>/dev/null || true)
    [[ -z "$holders" ]] && break
    echo "$holders" | xargs -r kill -9 2>/dev/null || true
    sleep 0.3
  done
  # Final wait for TIME_WAIT-ish lingering connections to clear.
  sleep 1
}

rss_summary() {
  local csv="$1"
  local out="$2"
  awk -F, 'NR==1{next}
    NR==2{min=$2; first=$2; first_t=$1}
    {
      if($2 < min) min=$2
      if($2 > max) max=$2
      sum+=$2; n++
      last=$2; last_t=$1
    }
    END {
      if(n == 0){ print "  (no samples)"; exit }
      printf "  start_kb: %d\n", first
      printf "  end_kb:   %d\n", last
      printf "  peak_kb:  %d\n", max
      printf "  mean_kb:  %d\n", sum/n
      printf "  growth_kb: %d (over %ds)\n", last-first, last_t-first_t
    }' "$csv" | tee "$out"
}

phase() {
  local label="$1"; shift
  echo
  echo "=== phase: $label ==="
  start_broker "$label"
  sleep 1
  local out="$RUN_DIR/${label}.json"
  set +e
  "$PYTHON" "$HARNESS" "$@" | tee "$out"
  local rc=$?
  set -e
  if [ $rc -ne 0 ]; then
    echo "  !! harness exited with $rc"
  fi
  local alive="yes"
  if ! kill -0 "$BROKER_PID" 2>/dev/null; then
    alive="DIED"
    echo "  !! broker died during phase $label"
  fi
  stop_broker
  echo "broker status: alive_at_phase_end=$alive  $BROKER_EXIT_STATUS"
  # gem_diag atexit line — only present when broker exited cleanly enough
  # for atexit (signal-killed coroutines do not fire it).
  local diag_line
  diag_line=$(grep -m1 '^gem_diag:' "$RUN_DIR/${label}.broker.log" 2>/dev/null || echo "")
  [[ -n "$diag_line" ]] && echo "$diag_line"
  echo
  echo "rss summary for $label:"
  rss_summary "$RUN_DIR/${label}.rss.csv" "$RUN_DIR/${label}.rss.summary.txt"
}

# Defaults — override by environment.
FANOUT_SUBS="${FANOUT_SUBS:-1000}"
FANOUT_MSGS="${FANOUT_MSGS:-5}"
FANOUT_BODY="${FANOUT_BODY:-256}"

SLOW_FAST="${SLOW_FAST:-4}"
SLOW_SLOW="${SLOW_SLOW:-1}"
SLOW_DELAY_MS="${SLOW_DELAY_MS:-200}"
SLOW_DURATION="${SLOW_DURATION:-15}"
SLOW_BODY="${SLOW_BODY:-1024}"

QUEUE_PUBS="${QUEUE_PUBS:-10}"
QUEUE_WORKERS="${QUEUE_WORKERS:-4}"
QUEUE_MSGS="${QUEUE_MSGS:-10000}"
QUEUE_BODY="${QUEUE_BODY:-256}"

PHASES="${PHASES:-fanout slow queue}"

echo "Run dir: $RUN_DIR"
echo "Phases: $PHASES"

for p in $PHASES; do
  case "$p" in
    fanout)
      phase "fanout" fanout \
        --n-subs "$FANOUT_SUBS" \
        --n-msgs "$FANOUT_MSGS" \
        --body-size "$FANOUT_BODY"
      ;;
    fanout_small)
      phase "fanout_small" fanout \
        --n-subs 100 --n-msgs 5 --body-size 256
      ;;
    slow)
      phase "slow" slow \
        --n-fast "$SLOW_FAST" \
        --n-slow "$SLOW_SLOW" \
        --slow-delay-ms "$SLOW_DELAY_MS" \
        --duration-s "$SLOW_DURATION" \
        --body-size "$SLOW_BODY" \
        --drain-s 3
      ;;
    queue)
      phase "queue" queue \
        --n-publishers "$QUEUE_PUBS" \
        --n-workers "$QUEUE_WORKERS" \
        --n-msgs "$QUEUE_MSGS" \
        --body-size "$QUEUE_BODY"
      ;;
    soak)
      phase "soak" soak \
        --n-subs "${SOAK_SUBS:-200}" \
        --rate "${SOAK_RATE:-50}" \
        --duration-s "${SOAK_DURATION:-30}" \
        --body-size "${SOAK_BODY:-256}"
      ;;
    *)
      echo "unknown phase: $p" >&2
      exit 1
      ;;
  esac
done

# Meta
{
  echo "date: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo "host: $(hostname)"
  echo "os: $(uname -srm)"
  echo "cpu: $(sysctl -n machdep.cpu.brand_string 2>/dev/null || echo unknown)"
  echo "cores: $(sysctl -n hw.ncpu 2>/dev/null || nproc)"
  echo "gem_commit: $(git -C "$PROJECT_ROOT" rev-parse --short HEAD)"
  echo "gem_branch: $(git -C "$PROJECT_ROOT" rev-parse --abbrev-ref HEAD)"
  echo "python: $($PYTHON --version 2>&1)"
  echo "phases: $PHASES"
} > "$RUN_DIR/meta.txt"

echo
echo "=== done ==="
echo "results: $RUN_DIR"
