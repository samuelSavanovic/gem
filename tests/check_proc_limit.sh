#!/usr/bin/env bash
# The process limit: GEM_MAX_PROCS lowers it, spawn past it raises
# "spawn: process table full" (catchable), std/http answers a connection it
# can't spawn a process for with 503 and keeps accepting, and a bad
# GEM_MAX_PROCS stops the program before it runs. Also: spawn's headroom
# under Linux's vm.max_map_count, and thread-pool work from more processes
# than the old table held.
#
# Run from the repo root: tests/check_proc_limit.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi

dir=$(mktemp -d)
trap 'rm -rf "$dir"' EXIT

fails=0
check() {
  local name=$1 want=$2 got=$3
  if [ "$want" != "$got" ]; then
    echo "FAIL: $name"
    diff <(printf '%s\n' "$want") <(printf '%s\n' "$got")
    fails=$((fails + 1))
  fi
}

cat > "$dir/fill.gem" <<'GEM'
let pids = []
let err = nil
while err == nil
  let r = pcall spawn(fn() receive() end)
  if r.ok then push(pids, r.value) else err = r.error end
end
print(len(pids), err)
# A freed slot is usable again.
kill(pids[0], "kill")
let r = pcall spawn(fn() nil end)
print("after a kill:", r.ok)
for p in pids
  kill(p, "kill")
end
GEM
"$GEM" "$dir/fill.gem" -o "$dir/fill" 2>"$dir/err" || { echo "FAIL: fill.gem doesn't compile"; cat "$dir/err"; exit 1; }

check "limit 8" "7 spawn: process table full
after a kill: true" "$(GEM_MAX_PROCS=8 "$dir/fill" 2>/dev/null)"
check "limit 2" "1 spawn: process table full
after a kill: true" "$(GEM_MAX_PROCS=2 "$dir/fill" 2>/dev/null)"
check "limit 100" "99 spawn: process table full
after a kill: true" "$(GEM_MAX_PROCS=100 "$dir/fill" 2>/dev/null)"

for bad in 1 0 -5 abc 12x; do
  out=$(GEM_MAX_PROCS=$bad "$dir/fill" 2>&1)
  code=$?
  check "GEM_MAX_PROCS=$bad" "gem: GEM_MAX_PROCS must be a number of processes (at least 2), got '$bad'
exit 1" "$out
exit $code"
done

# std/http with a full table: the acceptor answers 503, says why on stderr,
# and serves again once a slot is free.
cat > "$dir/http.gem" <<'GEM'
load "std/http"
load "std/string"

let PORT = 18201
let app = http.router()
app.get("/hello") do |req|
  http.ok("hello")
end

fn get()
  let fd = tcp_connect("127.0.0.1", PORT)
  tcp_write(fd, "GET /hello HTTP/1.1\r\nHost: t\r\nConnection: close\r\n\r\n")
  let acc = buf_new()
  let chunk = tcp_read(fd, 8192, 2000)
  while chunk != nil and chunk != ""
    buf_push(acc, chunk)
    chunk = tcp_read(fd, 8192, 2000)
  end
  tcp_close(fd)
  let raw = to_string(acc)
  let line_end = string.index_of(raw, "\r\n")
  if line_end < 0 then return "no response" end
  substr(raw, 0, line_end)
end

fn main()
  let server = http.start(app, {port: PORT, host: "127.0.0.1"})
  let fillers = []
  let full = false
  while not full
    let r = pcall spawn(fn()
      receive
      when {tag: "stop"} then nil
      end
    end)
    if r.ok then push(fillers, r.value) else full = true end
  end
  print(get())
  send(fillers[0], {tag: "stop"})
  sleep(10)
  print(get())
  for i = 1, len(fillers)
    send(fillers[i], {tag: "stop"})
  end
  kill(server.pid, "shutdown")
end
GEM
"$GEM" "$dir/http.gem" -o "$dir/http" 2>"$dir/err" || { echo "FAIL: http.gem doesn't compile"; cat "$dir/err"; exit 1; }
out=$(GEM_MAX_PROCS=16 "$dir/http" 2>"$dir/http_err")
check "http 503 when full" "HTTP/1.1 503 Service Unavailable
HTTP/1.1 200 OK" "$out"
check "http stderr" "http: spawn: process table full; refusing a connection" \
  "$(grep '^http:' "$dir/http_err")"

# Linux: spawn stops with headroom left under vm.max_map_count, so the
# processes already running (main here) can still allocate.
if [ -r /proc/sys/vm/max_map_count ]; then
  cat > "$dir/maps.gem" <<'GEM'
load "std/string"
fn main()
  let pids = []
  let err = nil
  while err == nil
    let r = pcall spawn(fn() receive() end)
    if r.ok then push(pids, r.value) else err = r.error end
  end
  print(string.starts_with(err, "spawn: too many processes for the system's memory-mapping limit"))
  kill(pids[0], "kill")
  let again = pcall spawn(fn() nil end)
  print("spawn after an exit:", again.ok)
  let big = []
  for i = 0, 200000
    push(big, "x{i}")
  end
  print("main still allocates", len(big))
  for p in pids
    kill(p, "kill")
  end
end
GEM
  "$GEM" "$dir/maps.gem" -o "$dir/maps" 2>"$dir/err" || { echo "FAIL: maps.gem doesn't compile"; cat "$dir/err"; exit 1; }
  check "map limit headroom" "true
spawn after an exit: true
main still allocates 200000" "$("$dir/maps" 2>/dev/null)"
fi

# A process count past the old 1024 doing thread-pool work at once: the
# pool's queue grows instead of failing with "I/O queue full".
cat > "$dir/pool.gem" <<'GEM'
fn main()
  let me = self()
  write_file("pool_input.txt", "abc")
  for i = 0, 2000
    spawn do
      let r = pcall read_file("pool_input.txt")
      send(me, r.ok and r.value == "abc")
    end
  end
  let ok = 0
  for i = 0, 2000
    if receive() then ok += 1 end
  end
  print(ok)
  remove_file("pool_input.txt")
end
GEM
"$GEM" "$dir/pool.gem" -o "$dir/pool" 2>"$dir/err" || { echo "FAIL: pool.gem doesn't compile"; cat "$dir/err"; exit 1; }
check "2000 concurrent read_file" "2000" "$(cd "$dir" && ./pool 2>&1)"

for bad in "" " 5" "+3" "5 "; do
  out=$(GEM_MAX_PROCS=$bad "$dir/fill" 2>&1)
  code=$?
  check "GEM_MAX_PROCS='$bad'" "gem: GEM_MAX_PROCS must be a number of processes (at least 2), got '$bad'
exit 1" "$out
exit $code"
done
cat > "$dir/one.gem" <<'GEM'
print(len(processes()))
GEM
"$GEM" "$dir/one.gem" -o "$dir/one" 2>"$dir/err" || { echo "FAIL: one.gem doesn't compile"; cat "$dir/err"; exit 1; }
check "GEM_MAX_PROCS above the maximum" "1" \
  "$(GEM_MAX_PROCS=99999999999999999999 "$dir/one" 2>&1)"

if [ "$fails" -eq 0 ]; then
  echo "proc limit: ok"
else
  exit 1
fi
