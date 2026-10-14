#!/usr/bin/env bash
# tls_connect and https:// in std/request against local servers, no
# internet: a test CA and its certificates (good, expired, wrong name),
# `openssl s_server` serving files (-WWW) and swallowing what it reads,
# and listeners in the Gem program itself that never answer or answer in
# plain HTTP. Checks verification and its messages, reads and writes of
# 1 MB, timeouts, a write left short with a record in flight and its
# continuation, a server that dies mid-stream, closed sockets, concurrent
# handshakes, and that killed processes leave no fds behind.
#
# Needs an OpenSSL 3 or later command-line tool (`openssl` on PATH or Homebrew's
# openssl@3); skips without one. A Gem built without OpenSSL only checks
# that tls_connect says so.
#
# Run from the repo root: tests/check_tls.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi

dir=$(mktemp -d)
pids=()
trap '{ kill "${pids[@]}"; wait; } 2>/dev/null; [ -n "${KEEP:-}" ] && echo "kept $dir" || rm -rf "$dir"' EXIT

fails=0
check() {
  local name=$1 want=$2 got=$3
  if [ "$want" != "$got" ]; then
    echo "FAIL: $name"
    diff <(printf '%s\n' "$want") <(printf '%s\n' "$got")
    fails=$((fails + 1))
  fi
}

if [ ! -s build/tls_cflags ] || [ -z "$(tr -d '[:space:]' < build/tls_cflags)" ]; then
  cat > "$dir/stub.gem" <<'GEM'
print((pcall tls_connect("127.0.0.1", 443)).error)
GEM
  got=$("$GEM" "$dir/stub.gem" 2>&1)
  check "built without OpenSSL" "tls_connect: this Gem was built without OpenSSL 3; install it (libssl-dev, openssl-devel or \`brew install openssl@3\`) and run \`make build\` again" "$got"
  [ "$fails" -gt 0 ] && exit 1
  echo "TLS checks skipped: this Gem was built without OpenSSL"
  exit 0
fi

OPENSSL=""
for c in openssl "$(brew --prefix openssl@3 2>/dev/null)/bin/openssl"; do
  if "$c" version 2>/dev/null | grep -qE '^OpenSSL [3-9]'; then
    OPENSSL=$c
    break
  fi
done
if [ -z "$OPENSSL" ]; then
  echo "TLS checks skipped: no OpenSSL 3 or later command-line tool"
  exit 0
fi

# ─── Certificates ───

cd "$dir"
mkdir www
{
  $OPENSSL req -x509 -newkey ec -pkeyopt ec_paramgen_curve:P-256 -nodes -keyout ca.key -out ca.pem \
    -days 2 -subj /CN=gem-test-ca
  $OPENSSL req -newkey ec -pkeyopt ec_paramgen_curve:P-256 -nodes -keyout leaf.key -out leaf.csr -subj /CN=localhost
  printf 'subjectAltName=DNS:localhost,IP:127.0.0.1\n' > good.ext
  printf 'subjectAltName=DNS:other.example\n' > wrong.ext
  $OPENSSL x509 -req -in leaf.csr -CA ca.pem -CAkey ca.key -CAcreateserial -days 2 -extfile good.ext -out good.pem
  $OPENSSL x509 -req -in leaf.csr -CA ca.pem -CAkey ca.key -CAcreateserial -days 2 -extfile wrong.ext -out wrong.pem
} > certs.log 2>&1 || { echo "FAIL: making the test certificates"; cat certs.log; exit 1; }
# -not_before / -not_after need OpenSSL 3.4.
expired=1
$OPENSSL x509 -req -in leaf.csr -CA ca.pem -CAkey ca.key -CAcreateserial -extfile good.ext \
  -not_before 20200101000000Z -not_after 20200102000000Z -out expired.pem >> certs.log 2>&1 || expired=0

printf 'hello over tls\n' > www/hello.txt
head -c 1048576 /dev/urandom > www/big.bin

# A FIFO the script holds open, every s_server's stdin: the one that
# doesn't serve files (sink) never writes or closes first.
mkfifo sink.in
exec 3<> sink.in

# serve <name> <cert> <s_server args...>: starts s_server on a free port
# (its stdin is the FIFO) and sets port_<name> once it accepts.
serve() {
  local name=$1 cert=$2
  shift 2
  (cd www && exec $OPENSSL s_server -accept 127.0.0.1:0 -cert "../$cert" -key ../leaf.key "$@" \
    < ../sink.in > "../$name.log" 2>&1) &
  pids+=($!)
  local p=""
  for _ in $(seq 100); do
    p=$(sed -n 's/^ACCEPT .*:\([0-9][0-9]*\)$/\1/p' "$name.log" 2>/dev/null | head -1)
    [ -n "$p" ] && break
    sleep 0.05
  done
  if [ -z "$p" ]; then
    echo "FAIL: s_server $name did not start"
    cat "$name.log"
    exit 1
  fi
  eval "port_$name=$p"
  eval "pid_$name=$!"
}

serve good good.pem -WWW
serve wrong wrong.pem -WWW
port_expired=0
[ "$expired" = 1 ] && serve expired expired.pem -WWW
# Write what they read to their logs. The client stops `stall` (SIGSTOP)
# so a write fills the socket, and kills `cut` mid-stream.
serve sink good.pem
serve stall good.pem
serve cut good.pem
# Its death (the client kills it) isn't reported.
disown "$pid_cut"
cd - > /dev/null

# ─── The client ───

cat > "$dir/client.gem" <<'GEM'
load "std/request"
load "std/string"

let args = argv()
let ca = args[1]
let good = to_int(args[2])
let wrong = to_int(args[3])
let expired = to_int(args[4])
let sink = to_int(args[5])
let big_file = args[6]
let hello_file = args[7]
let stall = to_int(args[8])
let stall_pid = args[9]
let cut = to_int(args[10])
let cut_pid = args[11]

fn fds()
  len(list_dir("/dev/fd"))
end

fn err(r)
  if r.ok
    return "no error"
  end
  r.error
end

# Accepts connections and reads each one to its end without a byte back.
fn silent_server(l)
  while true
    let s = tcp_accept(l)
    spawn do
      claim(s)
      let data = tcp_read(s, 4096, 10000)
      while data != nil and data != ""
        data = tcp_read(s, 4096, 10000)
      end
    end
  end
end

# Answers every connection in plain HTTP.
fn http_server(l)
  while true
    let s = tcp_accept(l)
    tcp_read(s, 4096, 1000)
    tcp_write(s, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n", 1000)
    tcp_close(s)
  end
end

fn main()
  let opts = {ca_file: ca, timeout_ms: 10000}
  let base = "https://localhost:{good}"
  let r = request.get("{base}/hello.txt", opts)
  print("get:", r.status, r.body)
  let big = request.get("{base}/big.bin", opts)
  print("1 MB body:", big.status, len(big.body), big.body == read_file(big_file))
  print("by IP:", request.get("https://127.0.0.1:{good}/hello.txt", opts).status)
  print("system trust store:", err(pcall request.get("{base}/hello.txt")))
  print("wrong name:", err(pcall tls_connect("localhost", wrong, {ca_file: ca})))
  print("server_name:", err(pcall tls_connect("localhost", good, {ca_file: ca, server_name: "other.example"})))
  if expired > 0
    print("expired:", err(pcall tls_connect("localhost", expired, {ca_file: ca})))
  end
  print("bad ca_file:", err(pcall tls_connect("localhost", good, {ca_file: "/nonexistent/ca.pem"})))
  print("not a PEM file:", err(pcall tls_connect("localhost", good, {ca_file: hello_file})))
  print("bad opts:", err(pcall tls_connect("localhost", good, {timeout_ms: 0})))
  print("opts not a table:", err(pcall tls_connect("localhost", good, 5)))
  print("ca_file not a string:", err(pcall tls_connect("localhost", good, {ca_file: 5})))
  let long_name = string.repeat("a", 300)
  let ln = err(pcall tls_connect("localhost", good, {ca_file: ca, server_name: long_name}))
  print("server_name too long:", str_replace(ln, long_name, "<300 a>"))
  print("refused:", err(pcall tls_connect("127.0.0.1", 1)))

  # Concurrent handshakes and requests from four processes.
  let me = self()
  for i = 0, 4
    spawn do
      let resp = request.get("{base}/hello.txt", opts)
      send(me, {tag: "got", status: resp.status})
    end
  end
  let statuses = []
  for i = 0, 4
    receive
    when {tag: "got", status: status}
      push(statuses, status)
    after 20000
      push(statuses, "timeout")
    end
  end
  print("concurrent:", statuses)

  # Writes and reads on the socket itself.
  let s = tls_connect("localhost", sink, opts)
  print("type:", type(s))
  let mb = build_string do |add|
    for i = 0, 16384
      add("0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef")
    end
  end
  print("write 1 MB:", tcp_write(s, mb, 10000))
  print("read timeout:", tcp_read(s, 100, 200))
  print("read, no wait:", tcp_read(s, 100, 0))
  print("tcp_fd:", err(pcall tcp_fd(s)))
  print("peer port:", tcp_peer(s).port == sink)
  tcp_close(s)
  print("read after close:", err(pcall tcp_read(s, 100)))
  print("write after close:", err(pcall tcp_write(s, "x")))
  tcp_close(s)

  # A write that fills the socket of a stopped server stops short with a
  # record in flight; the next write must start with the bytes it didn't
  # count, and then everything arrives in order.
  let st = tls_connect("localhost", stall, opts)
  let stall_data = build_string do |add|
    for i = 0, 131072
      add("stall-", substr(to_string(1000000 + i), 1), "-0123456789abcdef0123456789abcdef0123456789abcdef---")
    end
  end
  exec("kill -STOP {stall_pid}")
  let n = tcp_write(st, stall_data, 300)
  print("short write:", n < len(stall_data), n % 64)
  print("then other data:", err(pcall tcp_write(st, "other", 300)))
  print("then empty:", tcp_write(st, "", 0))
  let more = tcp_write(st, substr(stall_data, n), 0)
  print("then the rest, no wait:", more % 64)
  n = n + more
  exec("kill -CONT {stall_pid}")
  print("then the rest:", tcp_write(st, substr(stall_data, n), 20000) == len(stall_data) - n)
  tcp_close(st)

  # A server that dies without the TLS close alert: the read raises.
  let c = tls_connect("localhost", cut, opts)
  tcp_write(c, "x")
  sleep(100)
  exec("kill -9 {cut_pid}")
  let cr = err(pcall tcp_read(c, 100, 5000))
  print("server died:", substr(cr, 0, string.index_of(cr, " was ") + 4))
  tcp_close(c)

  let quiet = tcp_listen("127.0.0.1", 0)
  let quiet_port = tcp_local_port(quiet)
  spawn do
    silent_server(quiet)
  end
  print("handshake timeout:", str_replace(err(pcall tls_connect("127.0.0.1", quiet_port, {timeout_ms: 300})), "{quiet_port}", "QUIET"))

  let plain = tcp_listen("127.0.0.1", 0)
  let plain_port = tcp_local_port(plain)
  spawn do
    http_server(plain)
  end
  let e = str_replace(err(pcall tls_connect("127.0.0.1", plain_port, {timeout_ms: 5000})), "{plain_port}", "PLAIN")
  # OpenSSL's reason ("packet length too long", "wrong version number")
  # depends on its version.
  print("plain HTTP server:", substr(e, 0, string.index_of(e, " failed: ") + 8))

  # Processes killed mid-handshake leave no socket open: the silent
  # server's sessions see EOF and close their ends too.
  sleep(100)
  let before = fds()
  let pids = []
  for i = 0, 50
    push(pids, spawn do
      tls_connect("127.0.0.1", quiet_port)
    end)
  end
  sleep(200)
  let during = fds()
  for p in pids
    kill(p, "kill")
  end
  sleep(300)
  print("killed mid-handshake:", during > before, fds() == before)

  # A socket closed by another process while its reader waits.
  let s2 = tls_connect("localhost", sink, opts)
  let reader = spawn do
    send(me, {tag: "read", r: err(pcall tcp_read(s2, 100))})
  end
  sleep(50)
  tcp_close(s2)
  receive
  when {tag: "read", r: got}
    print("closed while reading:", got)
  after 5000
    print("closed while reading: no wake")
  end
  exit(0)
end
GEM

"$GEM" "$dir/client.gem" -o "$dir/client" 2> "$dir/err" || { echo "FAIL: client.gem doesn't compile"; cat "$dir/err"; exit 1; }
got=$("$dir/client" "$dir/ca.pem" "$port_good" "$port_wrong" "$port_expired" "$port_sink" "$dir/www/big.bin" "$dir/www/hello.txt" \
  "$port_stall" "$pid_stall" "$port_cut" "$pid_cut" 2>&1 \
  | sed -e "s/:$port_good/:GOOD/g" -e "s/:$port_wrong/:WRONG/g" -e "s/:$port_expired/:EXPIRED/g")
want_expired=""
[ "$expired" = 1 ] && want_expired="
expired: tls_connect: certificate verify failed for localhost:EXPIRED: certificate has expired"
check "tls client" "get: 200 hello over tls

1 MB body: 200 1048576 true
by IP: 200
system trust store: request.get: cannot connect to localhost:GOOD: tls_connect: certificate verify failed for localhost:GOOD: unable to get local issuer certificate
wrong name: tls_connect: certificate verify failed for localhost:WRONG: hostname mismatch
server_name: tls_connect: certificate verify failed for localhost:GOOD (server name other.example): hostname mismatch$want_expired
bad ca_file: tls_connect: cannot load ca_file '/nonexistent/ca.pem': No such file or directory
not a PEM file: tls_connect: cannot load ca_file '$dir/www/hello.txt': no certificate or crl found
bad opts: tls_connect: timeout_ms must be an int > 0 (milliseconds) or nil, got 0
opts not a table: tls_connect: opts must be a table, got int
ca_file not a string: tls_connect: ca_file must be a non-empty string without NUL bytes, got int
server_name too long: tls_connect: invalid server name '<300 a>'
refused: tls_connect: connect failed: Connection refused
concurrent: [200, 200, 200, 200]
type: socket
write 1 MB: 1048576
read timeout: nil
read, no wait: nil
tcp_fd: tcp_fd: a TLS socket has no fd to give to C
peer port: true
read after close: tcp_read: socket is closed
write after close: tcp_write: socket is closed
short write: true 0
then other data: tcp_write: an earlier tcp_write on this TLS socket timed out with 16384 bytes in flight; the next write must start with the bytes it didn't count
then empty: 0
then the rest, no wait: 0
then the rest: true
server died: tcp_read: the TLS connection was
handshake timeout: tls_connect: handshake with 127.0.0.1:QUIET timed out
plain HTTP server: tls_connect: handshake with 127.0.0.1:PLAIN failed:
killed mid-handshake: true true
closed while reading: tcp_read: socket is closed" "$got"

# The sink got every byte of the 1 MB write: 16384 copies of the
# 64-byte block (it prints what it reads, with lines of its own around).
got=$(grep -o 0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef "$dir/sink.log" | wc -l | tr -d ' ')
check "the sink got the whole write" 16384 "$got"
# The stalled server is still draining its socket when the client exits.
# Its blocks are numbered: each must come once, in order.
for _ in $(seq 100); do
  got=$(grep -o 'stall-[0-9]\{6\}-0123456789abcdef0123456789abcdef0123456789abcdef---' "$dir/stall.log" \
    | awk -F- '$2 + 0 != NR - 1 { print "block " NR - 1 " is " $2; bad = 1; exit } END { if (!bad) print NR }')
  [ "$got" = 131072 ] && break
  sleep 0.1
done
check "the stalled server got the whole write, in order" 131072 "$got"

# No CA certificate where OpenSSL looks: the error names the places.
cat > "$dir/nostore.gem" <<'GEM'
let r = pcall tls_connect("localhost", to_int(argv()[1]))
print(r.error)
GEM
got=$(SSL_CERT_FILE=/nonexistent/cert.pem SSL_CERT_DIR=/nonexistent/certs "$GEM" "$dir/nostore.gem" "$port_good" 2>&1 \
  | sed -e "s/:$port_good/:GOOD/g")
check "no trust store" "tls_connect: certificate verify failed for localhost:GOOD: unable to get local issuer certificate (no trusted certificates found in /nonexistent/cert.pem or /nonexistent/certs; set SSL_CERT_FILE or SSL_CERT_DIR)" "$got"
# A CA directory with certificates, none of them the issuer: no hint.
mkdir "$dir/cadir"
cp "$dir/wrong.pem" "$dir/cadir/unrelated.pem"
got=$(SSL_CERT_FILE=/nonexistent/cert.pem SSL_CERT_DIR="$dir/cadir" "$GEM" "$dir/nostore.gem" "$port_good" 2>&1 \
  | sed -e "s/:$port_good/:GOOD/g")
check "a CA directory without the issuer" "tls_connect: certificate verify failed for localhost:GOOD: unable to get local issuer certificate" "$got"

# A server that ends its responses without the TLS close alert (Python's
# ssl module closes without one): a chunked body is whole at its last
# chunk, a body that runs to the end of the stream may be cut short.
if command -v python3 > /dev/null; then
  cat > "$dir/noalert.py" <<'PY'
import socket, ssl, sys
ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
ctx.load_cert_chain(sys.argv[1], sys.argv[2])
srv = socket.socket()
srv.bind(("127.0.0.1", 0))
srv.listen(8)
print(srv.getsockname()[1], flush=True)
bodies = {
    b"/chunked": b"Transfer-Encoding: chunked\r\n\r\n2\r\nhi\r\n0\r\n",
    b"/to-eof": b"\r\nhi",
}
while True:
    conn, _ = srv.accept()
    try:
        s = ctx.wrap_socket(conn, server_side=True)
        req = b""
        while b"\r\n\r\n" not in req:
            req += s.recv(4096)
        path = req.split(b" ")[1]
        s.sendall(b"HTTP/1.1 200 OK\r\nConnection: close\r\n" + bodies.get(path, b"\r\n"))
        s.close()
    except (ssl.SSLError, OSError):
        conn.close()
PY
  python3 "$dir/noalert.py" "$dir/good.pem" "$dir/leaf.key" > "$dir/noalert.port" 2> "$dir/noalert.log" &
  pids+=($!)
  disown "$!"
  for _ in $(seq 100); do
    [ -s "$dir/noalert.port" ] && break
    sleep 0.05
  done
  port_noalert=$(cat "$dir/noalert.port")
  cat > "$dir/noalert.gem" <<'GEM'
load "std/request"
let opts = {ca_file: argv()[1], timeout_ms: 5000}
let base = "https://localhost:{argv()[2]}"
print("chunked:", request.get("{base}/chunked", opts).body)
print("to the end of the stream:", (pcall request.get("{base}/to-eof", opts)).error)
GEM
  got=$("$GEM" "$dir/noalert.gem" "$dir/ca.pem" "$port_noalert" 2>&1)
  check "no close alert" "chunked: hi
to the end of the stream: request.get: tcp_read: the TLS connection was closed before the server's close alert; the data may be cut short" "$got"
else
  echo "note: no python3, the close-alert checks for std/request are skipped"
fi

if [ "$fails" -gt 0 ]; then
  echo "$fails TLS check(s) failed"
  exit 1
fi
echo "all TLS checks passed"
