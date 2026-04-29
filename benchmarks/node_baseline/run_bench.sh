#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
PORT=8081
URL="http://localhost:$PORT"

cleanup() {
  [[ -n "${RSS_PID:-}" ]] && kill "$RSS_PID" 2>/dev/null || true
  [[ -n "${APP_PID:-}" ]] && kill "$APP_PID" 2>/dev/null || true
  wait 2>/dev/null || true
}
trap cleanup EXIT

sample_rss() {
  local pid=$1 outfile=$2
  echo "elapsed_s,rss_kb" > "$outfile"
  local start; start=$(date +%s)
  while kill -0 "$pid" 2>/dev/null; do
    local now rss elapsed
    now=$(date +%s); elapsed=$((now - start))
    rss=$(ps -o rss= -p "$pid" 2>/dev/null | tr -d ' ') || break
    [[ -n "$rss" ]] && echo "${elapsed},${rss}" >> "$outfile"
    sleep 0.5
  done
}

RUN_DIR="$SCRIPT_DIR/results"
mkdir -p "$RUN_DIR"

# Start fresh
rm -f "$SCRIPT_DIR/bookmarks.db"
cd "$SCRIPT_DIR"
node app.mjs > "$RUN_DIR/stdout.log" 2>&1 &
APP_PID=$!
sleep 1

# Verify
curl -s -o /dev/null "$URL/" || { echo "FATAL: server not up"; exit 1; }

# Start RSS sampling
sample_rss "$APP_PID" "$RUN_DIR/rss.csv" &
RSS_PID=$!

# Seed 20 bookmarks
for i in $(seq 1 20); do
  curl -s -o /dev/null -X POST "$URL/bookmarks" \
    -d "url=https://example.com/seed-${i}&title=Seed+Bookmark+${i}&tags=seed"
done
echo "Seeded 20 bookmarks"

# Warmup
echo "--- Warmup (5s) ---"
wrk -t1 -c2 -d5s "$URL/bookmarks" > /dev/null 2>&1
echo "Done"

# Burst GET /bookmarks
echo ""
echo "--- Burst: GET /bookmarks (30s, 10c) ---"
wrk -t2 -c10 -d30s --latency "$URL/bookmarks" | tee "$RUN_DIR/burst_get_bookmarks.txt"

# Burst GET /
echo ""
echo "--- Burst: GET / (30s, 10c) ---"
wrk -t2 -c10 -d30s --latency "$URL/" | tee "$RUN_DIR/burst_get_index.txt"

# Soak
echo ""
echo "--- Soak: GET /bookmarks (5m, 4c) ---"
wrk -t1 -c4 -d5m --latency "$URL/bookmarks" | tee "$RUN_DIR/soak_get_bookmarks.txt"

# Write burst
echo ""
echo "--- Burst: POST /bookmarks (10s, 4c) ---"
wrk -t1 -c4 -d10s --latency -s "$PROJECT_ROOT/benchmarks/wrk_post_bookmark.lua" "$URL/bookmarks" \
  | tee "$RUN_DIR/burst_post_bookmarks.txt"

# Stop RSS
kill "$RSS_PID" 2>/dev/null; wait "$RSS_PID" 2>/dev/null || true
unset RSS_PID

# RSS summary
echo ""
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
echo "=== Done ==="
