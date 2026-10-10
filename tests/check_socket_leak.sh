#!/usr/bin/env bash
# Sockets close when the process responsible for them ends, so a server
# whose sessions crash, return early or are killed holds no more fds than
# it started with (counted in /dev/fd). Three shapes, N sessions each:
#   - the honeypot's: acceptor -> registry -> one session per connection,
#     which claims its socket; half crash, half return early;
#   - sessions killed while blocked in tcp_read;
#   - processes killed mid-tcp_connect (connects to a listener whose
#     backlog is full stay in progress on Linux; macOS resets them, so
#     there the shape only checks the fds of refused connects).
# Also the GEM_DIAG=2 line for a session that forgot its claim and
# crashed, and GEM_DIAG=1's counts.
#
# Run from the repo root: tests/check_socket_leak.sh

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

# 1,000 blocked sessions hold 2,000 fds with their clients, and filling a
# backlog takes 1,025 more: raise the soft limit (macOS's default is 256).
ulimit -n 4096 2>/dev/null || ulimit -n "$(ulimit -Hn)" 2>/dev/null
limit=$(ulimit -n)
N=1000
FILL=1124
if [ "$limit" != unlimited ] && [ "$limit" -lt 2400 ]; then
  N=$(( (limit - 400) / 2 ))
  echo "note: fd limit $limit, running $N sessions per shape"
fi
if [ "$limit" != unlimited ] && [ "$limit" -lt 1400 ]; then
  FILL=0
  echo "note: fd limit $limit, skipping the mid-connect shape"
fi

cat > "$dir/leak.gem" <<'GEM'
let PORT = to_int(argv()[1])
let N = to_int(argv()[2])
let FILL = to_int(argv()[3])
let me = self()

fn fds()
  len(list_dir("/dev/fd"))
end

fn session(sock, mode)
  claim(sock)
  let data = tcp_read(sock, 16, 5000)
  if mode == "crash"
    error("session crashed on {data}")
  end
end

fn registry()
  let n = 0
  while true
    receive
    when {sock: sock}
      let mode = if n % 2 == 0 then "crash" else "return" end
      n += 1
      spawn do
        session(sock, mode)
      end
    end
  end
end

fn acceptor(l, reg)
  while true
    let sock = tcp_accept(l)
    send(reg, {sock: sock, peer: tcp_peer(sock)})
  end
end

fn main()
  let l = tcp_listen("127.0.0.1", PORT)
  let reg = spawn(registry)
  let acc = spawn do
    acceptor(l, reg)
  end
  sleep(10)
  # A client that sees no EOF ends the shape at once: a regression fails
  # in seconds instead of waiting out every client's timeout.
  let before = fds()
  let eofs = 0
  for i = 0, 2 * N
    let c = tcp_connect("127.0.0.1", PORT)
    tcp_write(c, "x")
    let r = tcp_read(c, 16, 5000)
    tcp_close(c)
    if r != ""
      break
    end
    eofs += 1
  end
  sleep(20)
  print("honeypot shape", before, fds(), "eofs", eofs == 2 * N)

  let l2 = tcp_listen("127.0.0.1", PORT + 1)
  before = fds()
  let pids = []
  let clients = []
  for i = 0, N
    push(clients, tcp_connect("127.0.0.1", PORT + 1))
    let s = tcp_accept(l2)
    push(pids, spawn do
      claim(s)
      tcp_read(s)
    end)
  end
  sleep(20)
  for p in pids
    kill(p, "kill")
  end
  eofs = 0
  let seen = true
  for c in clients
    if seen and tcp_read(c, 16, 5000) == ""
      eofs += 1
    else
      seen = false
    end
    tcp_close(c)
  end
  print("killed in tcp_read", before, fds(), "eofs", eofs == N)

  # A listener that never accepts: once its backlog is full, connects to
  # it stay in progress (Linux) or are reset (macOS).
  let l3 = tcp_listen("127.0.0.1", PORT + 2)
  before = fds()
  let connectors = []
  for i = 0, FILL
    push(connectors, spawn do
      let r = pcall tcp_connect("127.0.0.1", PORT + 2)
      send(me, if r.ok then "connected" else "refused" end)
      receive()
    end)
  end
  sleep(300)
  let connected = 0
  let refused = 0
  let more = true
  while more
    receive
    when "connected"
      connected += 1
    when "refused"
      refused += 1
    after 0
      more = false
    end
  end
  for p in connectors
    kill(p, "kill")
  end
  sleep(20)
  print("killed mid-connect", before, fds(), "pending", FILL - connected - refused)
  tcp_close(l3)
  tcp_close(l2)
  kill(acc, "kill")
  kill(reg, "kill")
end
GEM
"$GEM" "$dir/leak.gem" -o "$dir/leak" 2>"$dir/err" || { echo "FAIL: leak.gem doesn't compile"; cat "$dir/err"; exit 1; }

out=$("$dir/leak" 19331 "$N" "$FILL" 2>"$dir/stderr")
echo "$out" | sed 's/^/  /'
crashes=$(grep -c "Runtime Error" "$dir/stderr")
check "half of the honeypot sessions crash" "$N" "$crashes"
while read -r name1 name2 rest; do
  set -- $rest
  case "$name1 $name2" in
    "honeypot shape") check "honeypot shape: fds" "$1" "$2"; check "honeypot shape: every client saw EOF" "eofs true" "$3 $4" ;;
    "killed in") shift; check "killed in tcp_read: fds" "$1" "$2"; check "killed in tcp_read: every client saw EOF" "eofs true" "$3 $4" ;;
    "killed mid-connect") check "killed mid-connect: fds" "$1" "$2"
      if [ "$(uname)" = Linux ] && [ "$FILL" -gt 0 ] && [ "$4" -lt 1 ]; then
        echo "FAIL: killed mid-connect: no connect was still in progress"; fails=$((fails + 1))
      fi ;;
  esac
done <<< "$out"
check "three shapes ran" 3 "$(echo "$out" | grep -c .)"

# A session that forgot its claim and crashed: its socket stays with the
# acceptor, and GEM_DIAG=2 says so.
cat > "$dir/forgot.gem" <<'GEM'
fn session(sock)
  tcp_read(sock, 16, 1000)
  error("crashed")
end

fn acceptor(l)
  let sock = tcp_accept(l)
  spawn do
    session(sock)
  end
  receive()
end

let l = tcp_listen("127.0.0.1", 19334)
let a = spawn do
  acceptor(l)
end
let c = tcp_connect("127.0.0.1", 19334)
tcp_write(c, "x")
sleep(50)
print("acceptor owns", process_info(a).resources)
kill(a, "kill")
print("closed with the acceptor:", tcp_read(c, 16, 1000) == "")
GEM
"$GEM" "$dir/forgot.gem" -o "$dir/forgot" 2>"$dir/err" || { echo "FAIL: forgot.gem doesn't compile"; cat "$dir/err"; exit 1; }
out=$(GEM_DIAG=2 "$dir/forgot" 2>"$dir/stderr")
check "forgotten claim: output" "acceptor owns 1
closed with the acceptor: true" "$out"
got=$(grep '^gem_resources:' "$dir/stderr" | sed 's/process [0-9]*/process P/g')
check "forgotten claim: GEM_DIAG=2 line" "gem_resources: process P (session) exited (error) after using 1 socket owned by process P (acceptor)
gem_resources: process P (acceptor) exited (killed) after using 1 socket owned by process P (main)
gem_resources: process P (main) exited (normal) leaving 2 sockets open with no owner" "$got"

cat > "$dir/count.gem" <<'GEM'
let l = tcp_listen("127.0.0.1", 19335)
let c = tcp_connect("127.0.0.1", 19335)
claim(tcp_accept(l))
GEM
"$GEM" "$dir/count.gem" -o "$dir/count" 2>"$dir/err" || { echo "FAIL: count.gem doesn't compile"; cat "$dir/err"; exit 1; }
GEM_DIAG=1 "$dir/count" 2>"$dir/stderr"
check "GEM_DIAG=1 counts" "resources_open=2 ownerless=2" "$(grep -o 'resources_open=.*' "$dir/stderr")"

if [ "$fails" -gt 0 ]; then
  echo "$fails socket leak check(s) failed"
  exit 1
fi
echo "all socket leak checks passed"
