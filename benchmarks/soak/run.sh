#!/usr/bin/env bash
# Soak test: runs each target server under a steady mixed load for a long
# time (default 1 hour each) and checks that it stays up, answers
# correctly, and keeps its memory, file descriptors and latency flat.
#
#   benchmarks/soak/run.sh                          # all targets, 1 h each
#   DURATION=2m benchmarks/soak/run.sh              # a short check of the harness
#   TARGETS=mini_redis DURATION=4h benchmarks/soak/run.sh
#   python3 benchmarks/soak/report.py <run dir>     # the report again, any time
#
# Targets (in order): mini_redis, stomp (examples/stomp_broker), bookmark
# (examples/bookmark_app). Each gets a fresh server, built once at the
# start, run with GEM_DIAG=1. benchmarks/README.md ("soak") says what each
# load does and what the report checks.
#
# Env:
#   TARGETS        targets to run (default "mini_redis stomp bookmark")
#   DURATION       per target: seconds, or with s/m/h (default 1h)
#   SAMPLE_S       seconds between samples (default DURATION/120, 2..30)
#   GUARD_RSS_MB   kill a server whose RSS passes this (default 4096)
#   OUT            run directory (default benchmarks/soak/logs/<timestamp>)
#   MINI_REDIS_PORT (default 6395); stomp uses 61613 and bookmark 8080,
#                  which those programs fix
#   MINI_REDIS_ARGS, STOMP_ARGS, BOOKMARK_ARGS
#                  extra options for that target's load generator, e.g.
#                  MINI_REDIS_ARGS="--rate 8000 --subs 50" (see --help)
#
# Every sample is flushed and fsynced as it is taken, so stopping a run
# (Ctrl-C, SIGTERM, a crash) keeps everything sampled so far; Ctrl-C stops
# the current target cleanly, skips the rest and writes the report. On
# macOS the run holds off system sleep with caffeinate.
#
# Exit status: 0 when every target passed, 1 when one failed or the run was
# stopped, 2 for a setup problem.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
PYTHON=${PYTHON:-python3}
TARGETS=${TARGETS:-"mini_redis stomp bookmark"}
DURATION=${DURATION:-1h}
GUARD_RSS_MB=${GUARD_RSS_MB:-4096}
MINI_REDIS_PORT=${MINI_REDIS_PORT:-6395}
OUT=${OUT:-"$SCRIPT_DIR/logs/$(date +%Y%m%d-%H%M%S)"}

die() { echo "soak: $*" >&2; exit 2; }

case $DURATION in
  *h) DURATION_S=$(( ${DURATION%h} * 3600 )) ;;
  *m) DURATION_S=$(( ${DURATION%m} * 60 )) ;;
  *s) DURATION_S=${DURATION%s} ;;
  *) DURATION_S=$DURATION ;;
esac
[[ "$DURATION_S" =~ ^[0-9]+$ && "$DURATION_S" -ge 10 ]] || die "DURATION must be at least 10 s: $DURATION"
if [[ -z "${SAMPLE_S:-}" ]]; then
  SAMPLE_S=$(( DURATION_S / 120 ))
  (( SAMPLE_S < 2 )) && SAMPLE_S=2
  (( SAMPLE_S > 30 )) && SAMPLE_S=30
fi

for t in $TARGETS; do
  case $t in
    mini_redis|stomp|bookmark) ;;
    *) die "unknown target '$t' (mini_redis, stomp, bookmark)" ;;
  esac
done
command -v "$PYTHON" > /dev/null || die "$PYTHON not found"
[[ -x "$ROOT/build/gem" ]] || die "build/gem not found: run 'make build'"

port_of() {
  case $1 in
    mini_redis) echo "$MINI_REDIS_PORT" ;;
    stomp) echo 61613 ;;
    bookmark) echo 8080 ;;
  esac
}

# port_open <port>: whether something accepts connections on it.
port_open() {
  "$PYTHON" -c 'import socket,sys; s=socket.socket(); s.settimeout(0.5); sys.exit(s.connect_ex(("127.0.0.1", int(sys.argv[1]))) != 0)' "$1"
}

for t in $TARGETS; do
  port_open "$(port_of "$t")" && die "port $(port_of "$t") ($t) is already in use"
done

mkdir -p "$OUT/bin" || die "cannot create $OUT"

{
  echo "date: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo "gem commit: $(git -C "$ROOT" rev-parse --short HEAD)$(git -C "$ROOT" diff --quiet HEAD || echo ' (dirty)')"
  echo "system: $(uname -srm)"
  if [[ "$(uname)" == Darwin ]]; then
    echo "cpu: $(sysctl -n machdep.cpu.brand_string), $(sysctl -n hw.ncpu) cores"
    echo "memory: $(( $(sysctl -n hw.memsize) / 1073741824 )) GB"
  else
    echo "cpu: $(grep -m1 'model name' /proc/cpuinfo | cut -d: -f2 | sed 's/^ //'), $(nproc) cores"
    echo "memory: $(awk '/MemTotal/ {printf "%d GB", $2 / 1048576}' /proc/meminfo)"
  fi
  echo "python: $("$PYTHON" --version 2>&1)"
  echo "targets: $TARGETS"
  echo "duration per target: ${DURATION_S} s, sample every ${SAMPLE_S} s, guard RSS ${GUARD_RSS_MB} MB"
  [[ -n "${MINI_REDIS_ARGS:-}" ]] && echo "MINI_REDIS_ARGS: $MINI_REDIS_ARGS"
  [[ -n "${STOMP_ARGS:-}" ]] && echo "STOMP_ARGS: $STOMP_ARGS"
  [[ -n "${BOOKMARK_ARGS:-}" ]] && echo "BOOKMARK_ARGS: $BOOKMARK_ARGS"
} > "$OUT/meta.txt"
cat "$OUT/meta.txt"
echo "run directory: $OUT"

# Build everything first: a build error should stop the run now, not an
# hour in.
for t in $TARGETS; do
  case $t in
    mini_redis) src=examples/mini_redis/main.gem ;;
    stomp) src=examples/stomp_broker/main.gem ;;
    bookmark) src=examples/bookmark_app/app.gem ;;
  esac
  echo "building $t"
  "$ROOT/build/gem" "$ROOT/$src" -o "$OUT/bin/$t" > "$OUT/bin/$t.build.log" 2>&1 \
    || { cat "$OUT/bin/$t.build.log" >&2; die "building $t failed"; }
done

if [[ "$(uname)" == Darwin ]] && command -v caffeinate > /dev/null; then
  caffeinate -dims -w $$ &
fi

STOPPING=0
server_pid=""
load_pid=""
sampler_pid=""
on_signal() {
  STOPPING=1
  echo
  echo "soak: stopping (the samples so far are kept)"
  [[ -n "$load_pid" ]] && kill -TERM "$load_pid" 2> /dev/null
}
trap on_signal INT TERM

# wait_for <pid>: its exit status, waiting through the interruptions a
# trapped signal makes.
wait_for() {
  local st
  while :; do
    wait "$1"
    st=$?
    kill -0 "$1" 2> /dev/null || return "$st"
  done
}

# stop_server <target> <dir>: stops it so it exits through exit() and
# prints its GEM_DIAG statistics where it can (mini_redis has SHUTDOWN), with
# SIGTERM otherwise; records how it ended in status.txt.
stop_server() {
  local t=$1 dir=$2 st
  if ! kill -0 "$server_pid" 2> /dev/null; then
    wait_for "$server_pid"
    st=$?
    echo "server_end=exited before the end, status $st" >> "$dir/status.txt"
    return
  fi
  echo "server_end=alive" >> "$dir/status.txt"
  if [[ $t == mini_redis ]]; then
    # shellcheck disable=SC2016 # $8 is RESP's length prefix, not a variable
    "$PYTHON" -c 'import socket,sys; s=socket.create_connection(("127.0.0.1", int(sys.argv[1])), 5); s.sendall(b"*1\r\n$8\r\nSHUTDOWN\r\n"); s.recv(1)' \
      "$MINI_REDIS_PORT" 2> /dev/null
  else
    kill -TERM "$server_pid" 2> /dev/null
  fi
  for _ in $(seq 50); do
    kill -0 "$server_pid" 2> /dev/null || break
    sleep 0.1
  done
  kill -KILL "$server_pid" 2> /dev/null
  wait_for "$server_pid"
}

run_target() {
  local t=$1 dir="$OUT/$1" port load extra st
  port=$(port_of "$t")
  mkdir -p "$dir"
  echo "duration_s=$DURATION_S" > "$dir/status.txt"
  echo "started=$(date -u +%Y-%m-%dT%H:%M:%SZ)" >> "$dir/status.txt"

  # Each server runs in its own directory (bookmark_app opens bookmarks.db
  # and serves ./static from its working directory), with SIGINT ignored:
  # Ctrl-C goes to the whole process group, and the server must outlive it
  # to be shut down cleanly (and print its GEM_DIAG statistics).
  mkdir -p "$dir/work"
  case $t in
    mini_redis)
      (cd "$dir/work" && trap '' INT && GEM_DIAG=1 exec "$OUT/bin/mini_redis" --port "$port") > "$dir/server.log" 2>&1 &
      load=mini_redis_load.py; extra=${MINI_REDIS_ARGS:-} ;;
    stomp)
      (cd "$dir/work" && trap '' INT && GEM_DIAG=1 exec "$OUT/bin/stomp") > "$dir/server.log" 2>&1 &
      load=stomp_load.py; extra=${STOMP_ARGS:-} ;;
    bookmark)
      cp -R "$ROOT/examples/bookmark_app/static" "$dir/work/static"
      (cd "$dir/work" && trap '' INT && GEM_DIAG=1 LOG_LEVEL=warn exec "$OUT/bin/bookmark") > "$dir/server.log" 2>&1 &
      load=bookmark_load.py; extra=${BOOKMARK_ARGS:-} ;;
  esac
  server_pid=$!
  echo "server_pid=$server_pid" >> "$dir/status.txt"

  for _ in $(seq 100); do
    port_open "$port" && break
    kill -0 "$server_pid" 2> /dev/null || break
    sleep 0.1
  done
  if ! port_open "$port"; then
    echo "soak: $t did not start listening on $port, see $dir/server.log" >&2
    echo "server_end=did not start" >> "$dir/status.txt"
    kill -KILL "$server_pid" 2> /dev/null
    wait_for "$server_pid"
    return 1
  fi

  echo
  echo "── $t: ${DURATION_S} s, from $(date +%H:%M:%S) ──"
  # shellcheck disable=SC2086 # $extra is a list of options
  "$PYTHON" "$SCRIPT_DIR/$load" --port "$port" --duration-s "$DURATION_S" \
    --sample-s "$SAMPLE_S" --out "$dir/load.csv" --errors "$dir/errors.log" $extra \
    > "$dir/load.log" 2>&1 &
  load_pid=$!
  (trap '' INT && exec "$PYTHON" "$SCRIPT_DIR/sample.py" --pid "$server_pid" \
    --interval "$SAMPLE_S" --out "$dir/server.csv" --status "$dir/status.txt" \
    --notify "$load_pid" --guard-rss-mb "$GUARD_RSS_MB") > "$dir/sampler.log" 2>&1 &
  sampler_pid=$!

  # Waits for the load generator, printing progress once a minute. If it
  # is still running 5 minutes past its deadline it is hung: SIGTERM, then
  # SIGKILL (the rows it wrote are on disk either way).
  local started=$SECONDS next_progress=$(( SECONDS + 60 )) hung=0
  while kill -0 "$load_pid" 2> /dev/null; do
    sleep 1
    if (( SECONDS >= next_progress )); then
      progress "$dir"
      next_progress=$(( next_progress + 60 ))
    fi
    if (( hung == 0 && SECONDS - started >= DURATION_S + 300 )); then
      hung=1
      echo "soak: the $t load generator is hung, stopping it" >&2
      echo "load_hung=yes" >> "$dir/status.txt"
      kill -TERM "$load_pid" 2> /dev/null
    elif (( hung == 1 && SECONDS - started >= DURATION_S + 330 )); then
      kill -KILL "$load_pid" 2> /dev/null
    fi
  done
  wait_for "$load_pid"
  st=$?
  load_pid=""
  echo "load_exit=$st" >> "$dir/status.txt"
  # The sampler first: it would take the shutdown for a crash.
  kill -TERM "$sampler_pid" 2> /dev/null
  wait_for "$sampler_pid"
  sampler_pid=""
  stop_server "$t" "$dir"
  echo "ended=$(date -u +%Y-%m-%dT%H:%M:%SZ)" >> "$dir/status.txt"
  tail -n 1 "$dir/load.log"
  return 0
}

# progress <dir>: a line with the latest samples, so a long run shows that
# it is alive.
progress() {
  local dir=$1
  "$PYTHON" - "$dir" << 'EOF' 2> /dev/null
import csv, sys
d = sys.argv[1]
def last(path):
    rows = list(csv.DictReader(open(path)))
    return rows[-1] if rows else {}
s, l = last(f"{d}/server.csv"), last(f"{d}/load.csv")
rss = int(s.get("rss_kb") or 0) // 1024
p99 = [k for k in l if k.endswith("_p99_ms") and l[k]]
print(f"  {float(l.get('elapsed_s') or 0):6.0f} s  RSS {rss} MB  fds {s.get('fds', '')}  errors {l.get('errors') or 0}"
      + "".join(f"  {k[:-7]} p99 {float(l[k]):.1f} ms" for k in p99))
EOF
}

for t in $TARGETS; do
  [[ $STOPPING == 1 ]] && break
  run_target "$t"
done

echo
"$PYTHON" "$SCRIPT_DIR/report.py" "$OUT" > /dev/null || die "report failed"
"$PYTHON" - "$OUT/report.md" << 'EOF'
import sys
for line in open(sys.argv[1]):
    if line.startswith("## "):
        print()
        print(line[3:].rstrip())
    elif line.startswith("- **") or line.startswith("Ran "):
        print("  " + line.removeprefix("- ").replace("**", "").rstrip())
EOF
echo
echo "report: $OUT/report.md"
[[ $STOPPING == 0 ]] && ! grep -q -E '^\| [a-z_]+ \| FAIL \|' "$OUT/report.md"
