#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
APP_DIR="$PROJECT_ROOT/examples/bookmark_app"
LOGS_DIR="$SCRIPT_DIR/logs"

PORT=8080
URL="http://localhost:$PORT"

# Configurable parameters
WARMUP_DURATION="${WARMUP_DURATION:-5s}"
BURST_DURATION="${BURST_DURATION:-30s}"
BURST_THREADS="${BURST_THREADS:-2}"
BURST_CONNECTIONS="${BURST_CONNECTIONS:-10}"
SOAK_DURATION="${SOAK_DURATION:-5m}"
SOAK_THREADS="${SOAK_THREADS:-1}"
SOAK_CONNECTIONS="${SOAK_CONNECTIONS:-4}"
WRITE_BURST_DURATION="${WRITE_BURST_DURATION:-10s}"
WRITE_BURST_CONNECTIONS="${WRITE_BURST_CONNECTIONS:-4}"
RSS_INTERVAL="${RSS_INTERVAL:-0.5}"

# --- helpers ---

timestamp() { date +"%Y-%m-%d_%H-%M-%S"; }

die() { echo "FATAL: $1" >&2; cleanup; exit 1; }

cleanup() {
  [[ -n "${RSS_PID:-}" ]] && kill "$RSS_PID" 2>/dev/null || true
  [[ -n "${APP_PID:-}" ]] && kill "$APP_PID" 2>/dev/null || true
  wait 2>/dev/null || true
}
trap cleanup EXIT

wait_for_server() {
  local attempts=0
  while ! curl -s -o /dev/null "$URL/" 2>/dev/null; do
    attempts=$((attempts + 1))
    if [ "$attempts" -ge 40 ]; then
      die "server failed to start after 20s"
    fi
    sleep 0.5
  done
}

start_app() {
  rm -f "$APP_DIR/bookmarks.db" "$APP_DIR/bookmarks.db-shm" "$APP_DIR/bookmarks.db-wal"
  cd "$APP_DIR"
  ./app > "$RUN_DIR/app_stdout.log" 2>&1 &
  APP_PID=$!
  wait_for_server
}

stop_app() {
  [[ -n "${APP_PID:-}" ]] && kill "$APP_PID" 2>/dev/null || true
  wait "$APP_PID" 2>/dev/null || true
  unset APP_PID
}

sample_rss() {
  local pid=$1 outfile=$2
  echo "elapsed_s,rss_kb" > "$outfile"
  local start
  start=$(date +%s)
  while kill -0 "$pid" 2>/dev/null; do
    local now rss elapsed
    now=$(date +%s)
    elapsed=$((now - start))
    rss=$(ps -o rss= -p "$pid" 2>/dev/null | tr -d ' ') || break
    [[ -n "$rss" ]] && echo "${elapsed},${rss}" >> "$outfile"
    sleep "$RSS_INTERVAL"
  done
}

check_alive() {
  if ! kill -0 "$APP_PID" 2>/dev/null; then
    echo "  !! App crashed during this phase (PID $APP_PID)"
    return 1
  fi
}

# --- preflight ---

command -v wrk >/dev/null || die "wrk not found (brew install wrk)"
[[ -x "$APP_DIR/app" ]] || die "bookmark app not built — run: cd $APP_DIR && $PROJECT_ROOT/build/gem app.gem -o app"

if curl -s -o /dev/null "$URL/" 2>/dev/null; then
  die "port $PORT already in use"
fi

# --- setup run directory ---

RUN_ID="$(timestamp)"
RUN_DIR="$LOGS_DIR/$RUN_ID"
mkdir -p "$RUN_DIR"

echo "=== Gem Bookmark App Benchmark ==="
echo "Run: $RUN_ID"
echo "Logs: $RUN_DIR"
echo ""

# --- start app with fresh database ---

start_app
echo "App PID: $APP_PID"
echo ""

# --- start RSS sampling ---

sample_rss "$APP_PID" "$RUN_DIR/rss.csv" &
RSS_PID=$!

# --- seed data for read tests (small dataset) ---

for i in $(seq 1 20); do
  curl -s -o /dev/null -X POST "$URL/bookmarks" \
    -d "url=https://example.com/seed-${i}&title=Seed+Bookmark+${i}&tags=seed"
done
echo "Seeded 20 bookmarks"
echo ""

# --- warmup ---

echo "--- Warmup ($WARMUP_DURATION) ---"
wrk -t1 -c2 -d"$WARMUP_DURATION" "$URL/bookmarks" > /dev/null 2>&1
echo "Done"
echo ""

# =============================================
# Phase 1: Read bursts (small dataset, 20 rows)
# =============================================

echo "--- Burst: GET /bookmarks ($BURST_DURATION, ${BURST_CONNECTIONS}c) ---"
wrk -t"$BURST_THREADS" -c"$BURST_CONNECTIONS" -d"$BURST_DURATION" \
  --latency "$URL/bookmarks" \
  | tee "$RUN_DIR/burst_get_bookmarks.txt"
check_alive || die "app died during GET /bookmarks burst"
echo ""

echo "--- Burst: GET / ($BURST_DURATION, ${BURST_CONNECTIONS}c) ---"
wrk -t"$BURST_THREADS" -c"$BURST_CONNECTIONS" -d"$BURST_DURATION" \
  --latency "$URL/" \
  | tee "$RUN_DIR/burst_get_index.txt"
check_alive || die "app died during GET / burst"
echo ""

# =============================================
# Phase 2: Soak test (low intensity, long run)
# =============================================

echo "--- Soak: GET /bookmarks ($SOAK_DURATION, ${SOAK_CONNECTIONS}c) ---"
echo "  (watching for latency drift and memory growth)"
wrk -t"$SOAK_THREADS" -c"$SOAK_CONNECTIONS" -d"$SOAK_DURATION" \
  --latency "$URL/bookmarks" \
  | tee "$RUN_DIR/soak_get_bookmarks.txt"
check_alive || echo "  !! App crashed during soak"
echo ""

# =============================================
# Phase 3: Write burst (short, measures insert throughput)
# =============================================

echo "--- Burst: POST /bookmarks ($WRITE_BURST_DURATION, ${WRITE_BURST_CONNECTIONS}c) ---"
wrk -t1 -c"$WRITE_BURST_CONNECTIONS" -d"$WRITE_BURST_DURATION" \
  --latency -s "$SCRIPT_DIR/wrk_post_bookmark.lua" "$URL/bookmarks" \
  | tee "$RUN_DIR/burst_post_bookmarks.txt"
check_alive || echo "  !! App crashed during write burst"
echo ""

# --- stop RSS sampling, stop app ---

kill "$RSS_PID" 2>/dev/null; wait "$RSS_PID" 2>/dev/null || true
unset RSS_PID

# --- RSS summary ---

echo "--- RSS Summary ---"
if [[ -f "$RUN_DIR/rss.csv" ]]; then
  awk -F, 'NR==1{next} {
    if(min=="" || $2<min) min=$2;
    if($2>max) max=$2;
    sum+=$2; n++;
    last=$2; last_t=$1
  } END {
    printf "  Start:   %d KB\n", min
    printf "  End:     %d KB\n", last
    printf "  Peak:    %d KB\n", max
    printf "  Mean:    %d KB\n", sum/n
    printf "  Growth:  %d KB over %ds\n", last-min, last_t
  }' "$RUN_DIR/rss.csv" | tee "$RUN_DIR/rss_summary.txt"
fi
echo ""

# --- collect system info ---

{
  echo "date: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo "hostname: $(hostname)"
  echo "os: $(uname -srm)"
  echo "cpu: $(sysctl -n machdep.cpu.brand_string 2>/dev/null || echo unknown)"
  echo "cores: $(sysctl -n hw.ncpu 2>/dev/null || nproc)"
  echo "memory_gb: $(( $(sysctl -n hw.memsize 2>/dev/null || echo 0) / 1073741824 ))"
  echo "wrk: $(wrk --version 2>&1 | head -1)"
  echo "gem_commit: $(git -C "$PROJECT_ROOT" rev-parse --short HEAD)"
  echo ""
  echo "parameters:"
  echo "  warmup: $WARMUP_DURATION"
  echo "  burst_duration: $BURST_DURATION"
  echo "  burst_threads: $BURST_THREADS"
  echo "  burst_connections: $BURST_CONNECTIONS"
  echo "  soak_duration: $SOAK_DURATION"
  echo "  soak_threads: $SOAK_THREADS"
  echo "  soak_connections: $SOAK_CONNECTIONS"
  echo "  write_burst_duration: $WRITE_BURST_DURATION"
  echo "  write_burst_connections: $WRITE_BURST_CONNECTIONS"
  echo "  rss_interval: ${RSS_INTERVAL}s"
} > "$RUN_DIR/meta.txt"

echo "=== Done ==="
echo "Results saved to: $RUN_DIR"
