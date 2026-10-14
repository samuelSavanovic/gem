#!/usr/bin/env bash
# Name lookups don't stop other processes: tcp_connect, tls_connect and
# tcp_listen given a host name wait for getaddrinfo on the thread pool's
# resolver lane. A shim interposed on getaddrinfo (DYLD_INSERT_LIBRARIES on
# macOS, LD_PRELOAD elsewhere) makes "<ms>.slow.test" and
# "<ms>.<label>.slow.test" answer 127.0.0.1 after <ms> milliseconds and
# "<ms>.nx.test" fail after <ms>, and appends each of these names it is
# asked for to the file SHIM_LOG names. Checks: other processes keep running during a
# lookup, tcp_connect's and std/request's timeout_ms cover it, a failed
# lookup raises "cannot resolve", processes asking for one name meanwhile
# share one getaddrinfo call, the lane runs 16 lookups at once and queues
# a 17th, and a process killed mid-lookup ends at once and the
# program's end doesn't wait for its lookup.
#
# Run from the repo root: tests/check_resolve.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi

dir=$(mktemp -d)
trap 'rm -rf "$dir"' EXIT

cat > "$dir/shim.c" <<'C'
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dlfcn.h>

static int ends_with(const char *s, const char *suffix) {
    size_t n = strlen(s), m = strlen(suffix);
    return n >= m && strcmp(s + n - m, suffix) == 0;
}

/* The delay in ms of a test name, after sleeping it, else -1; *nx says
   whether the name fails. Logs the name to $SHIM_LOG. */
static long shim_delay(const char *host, int *nx) {
    if (!host) return -1;
    char *end;
    long ms = strtol(host, &end, 10);
    if (end == host || *end != '.') return -1;
    if (ends_with(end, ".slow.test")) *nx = 0;
    else if (strcmp(end, ".nx.test") == 0) *nx = 1;
    else return -1;
    const char *log = getenv("SHIM_LOG");
    FILE *f = log ? fopen(log, "a") : NULL;
    if (f) { fprintf(f, "%s\n", host); fclose(f); }
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };
    nanosleep(&ts, NULL);
    return ms;
}

#ifdef __APPLE__
static int shim_getaddrinfo(const char *host, const char *serv,
                            const struct addrinfo *hints, struct addrinfo **res) {
    int nx = 0;
    if (shim_delay(host, &nx) >= 0) {
        if (nx) return EAI_NONAME;
        host = "127.0.0.1";
    }
    return getaddrinfo(host, serv, hints, res);
}
__attribute__((used)) static struct { const void *repl, *orig; } shim_interpose
    __attribute__((section("__DATA,__interpose"))) = { (const void *)shim_getaddrinfo, (const void *)getaddrinfo };
#else
int getaddrinfo(const char *host, const char *serv,
                const struct addrinfo *hints, struct addrinfo **res) {
    static int (*real)(const char *, const char *, const struct addrinfo *, struct addrinfo **);
    if (!real) real = dlsym(RTLD_NEXT, "getaddrinfo");
    int nx = 0;
    if (shim_delay(host, &nx) >= 0) {
        if (nx) return EAI_NONAME;
        host = "127.0.0.1";
    }
    return real(host, serv, hints, res);
}
#endif
C

if [ "$(uname)" = Darwin ]; then
  shim="$dir/shim.dylib"
  cc -dynamiclib -o "$shim" "$dir/shim.c" || exit 2
  preload=(env DYLD_INSERT_LIBRARIES="$shim")
else
  shim="$dir/shim.so"
  cc -shared -fPIC -o "$shim" "$dir/shim.c" -ldl || exit 2
  preload=(env LD_PRELOAD="$shim")
fi

fails=0
check() {
  local name=$1 want=$2 got=$3
  if [ "$want" != "$got" ]; then
    echo "FAIL: $name"
    diff <(printf '%s\n' "$want") <(printf '%s\n' "$got")
    fails=$((fails + 1))
  fi
}

run() {
  local name=$1
  shift
  if ! "$GEM" "$dir/$name.gem" -o "$dir/$name" 2> "$dir/$name.err"; then
    echo "FAIL: $name does not compile"; cat "$dir/$name.err"
    fails=$((fails + 1))
    return 1
  fi
  : > "$dir/shim.log"
  "${preload[@]}" SHIM_LOG="$dir/shim.log" "$dir/$name" "$@" 2>&1
}

# A ticker process records the longest gap between its 10 ms ticks while
# the main process makes the lookups (after a sleep, so it is ticking).
cat > "$dir/ticker.gem" <<'GEM'
fn ticker(parent)
  let last = time_ms()
  let worst = 0
  while true
    let stop = receive
      when "stop" then true
      after 10 then false
    end
    let now = time_ms()
    if now - last > worst
      worst = now - last
    end
    last = now
    if stop
      send(parent, worst)
      return nil
    end
  end
end

fn worst_gap(t)
  send(t, "stop")
  return receive()
end

export ticker, worst_gap
GEM

cat > "$dir/connect.gem" <<'GEM'
load "./ticker"
let server = tcp_listen("127.0.0.1", 0)
let port = tcp_local_port(server)
let me = self()
let t = spawn do ticker.ticker(me) end
sleep(30)
let t0 = time_ms()
let sock = tcp_connect("600.slow.test", port)
let took = time_ms() - t0
print("connected: {type(sock)}")
print("waited for the lookup: {took >= 550}")
print("ticker kept running: {ticker.worst_gap(t) < 300}")
GEM
check "tcp_connect to a slow name" "connected: socket
waited for the lookup: true
ticker kept running: true" "$(run connect)"

cat > "$dir/listen.gem" <<'GEM'
load "./ticker"
let me = self()
let t = spawn do ticker.ticker(me) end
sleep(30)
let server = tcp_listen("400.slow.test", 0)
print("listening: {tcp_local_port(server) > 0}")
print("ticker kept running: {ticker.worst_gap(t) < 300}")
GEM
check "tcp_listen on a slow name" "listening: true
ticker kept running: true" "$(run listen)"

cat > "$dir/timeout.gem" <<'GEM'
let t0 = time_ms()
let r = pcall tcp_connect("2000.slow.test", 9, 200)
let took = time_ms() - t0
print(r.error)
print("gave up at the deadline: {took >= 190 and took < 1000}")
let r2 = pcall tcp_connect("2000.slow.test", 9, 0)
print(r2.error)
GEM
check "timeout_ms covers the lookup" "tcp_connect: connect to 2000.slow.test:9 timed out
gave up at the deadline: true
tcp_connect: connect to 2000.slow.test:9 timed out" "$(run timeout)"

cat > "$dir/request.gem" <<'GEM'
load "std/request"
let r = pcall request.get("http://2000.slow.test:9/", {timeout_ms: 200})
print(r.error)
GEM
check "std/request's timeout_ms covers the lookup" \
  "request.get: timed out after 200 ms waiting for http://2000.slow.test:9/" "$(run request)"

cat > "$dir/nx.gem" <<'GEM'
let r = pcall tcp_connect("100.nx.test", 80)
print(r.error)
let r2 = pcall tcp_listen("100.nx.test", 0)
print(r2.error)
GEM
check "a failed lookup" "tcp_connect: cannot resolve '100.nx.test'
tcp_listen: cannot resolve '100.nx.test'" "$(run nx)"

# Twenty processes ask for one name while its lookup runs: one
# getaddrinfo call answers them all.
cat > "$dir/same.gem" <<'GEM'
let server = tcp_listen("127.0.0.1", 0)
let port = tcp_local_port(server)
let me = self()
for i = 0, 20
  spawn do
    let r = pcall tcp_connect("300.slow.test", port)
    send(me, r.ok)
  end
end
let ok = 0
for i = 0, 20
  if receive() then ok += 1 end
end
print("connected: {ok}")
GEM
check "one lookup for one name" "connected: 20
lookups: 1" "$(run same; echo "lookups: $(wc -l < "$dir/shim.log" | tr -d ' ')")"

# Lookups of 300 ms for <n> names at once: 16 end together after about
# 300 ms, and a 17th waits for a worker, so 17 take about 600.
cat > "$dir/many.gem" <<'GEM'
let n = to_int(argv()[1])
let server = tcp_listen("127.0.0.1", 0)
let port = tcp_local_port(server)
let me = self()
let t0 = time_ms()
for i = 0, n
  spawn do
    let r = pcall tcp_connect("300.n{i}.slow.test", port)
    send(me, r.ok)
  end
end
let ok = 0
for i = 0, n
  if receive() then ok += 1 end
end
let took = time_ms() - t0
print("connected: {ok}")
print("rounds: {if took < 500 then 1 else 2 end}")
GEM
check "16 lookups at once" "connected: 16
rounds: 1" "$(run many 16)"
check "a 17th lookup waits" "connected: 17
rounds: 2" "$(run many 17)"

# A process killed during its lookup ends at once, and the program's end
# doesn't wait for the lookup it left on its worker.
cat > "$dir/kill.gem" <<'GEM'
let t0 = time_ms()
let p = spawn do tcp_connect("3000.slow.test", 80) end
sleep(50)
print("killed: {kill(p, "kill")}")
sleep(50)
print("gone: {process_info(p) == nil}")
print("main ends in time: {time_ms() - t0 < 1000}")
GEM
t0=$(date +%s)
out=$(run kill)
t1=$(date +%s)
check "killed mid-lookup" "killed: true
gone: true
main ends in time: true" "$out"
if [ $((t1 - t0)) -ge 3 ]; then
  echo "FAIL: the program waited for a lookup in flight at its end ($((t1 - t0)) s)"
  fails=$((fails + 1))
fi

if [ "$fails" -gt 0 ]; then
  echo "check_resolve: $fails failed"
  exit 1
fi
echo "check_resolve: all passed"
