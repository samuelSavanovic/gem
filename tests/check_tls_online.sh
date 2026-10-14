#!/usr/bin/env bash
# tls_connect and https:// in std/request against real sites, with the
# system's trust store: requests to example.com and google.com that must
# succeed, badssl.com's broken certificates, which must each fail with
# their reason, and its TLS 1.0-only server, whose handshake must fail.
# Needs the internet, so it is not part of `make test`:
# `make test-tls-online`.
#
# Run from the repo root: tests/check_tls_online.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi

dir=$(mktemp -d)
trap 'rm -rf "$dir"' EXIT

cat > "$dir/online.gem" <<'GEM'
load "std/request"

fn err(r)
  if r.ok
    return "no error"
  end
  r.error
end

fn try_host(host, port)
  err(pcall tls_connect(host, port, {timeout_ms: 15000}))
end

let opts = {timeout_ms: 15000}
let r = request.get("https://example.com/", opts)
print("example.com:", r.status, len(r.body) > 0)
let g = request.get("https://www.google.com/", opts)
print("google.com:", g.status)
print("expired:", try_host("expired.badssl.com", 443))
print("wrong host:", try_host("wrong.host.badssl.com", 443))
print("self-signed:", try_host("self-signed.badssl.com", 443))
print("untrusted root:", try_host("untrusted-root.badssl.com", 443))
print("TLS 1.0 only:", try_host("tls-v1-0.badssl.com", 1010))
GEM

out=$("$GEM" "$dir/online.gem" 2>&1)
echo "$out"
fails=0
# expect <line prefix> <text the line must contain>
expect() {
  if ! printf '%s\n' "$out" | grep -F "$1" | grep -qF "$2"; then
    echo "FAIL: $1 should contain: $2"
    fails=$((fails + 1))
  fi
}
expect "example.com:" "200 true"
expect "google.com:" "200"
expect "expired:" "certificate verify failed for expired.badssl.com:443: certificate has expired"
expect "wrong host:" "certificate verify failed for wrong.host.badssl.com:443: hostname mismatch"
expect "self-signed:" "certificate verify failed for self-signed.badssl.com:443: self-signed certificate"
expect "untrusted root:" "certificate verify failed for untrusted-root.badssl.com:443: self-signed certificate in certificate chain"
expect "TLS 1.0 only:" "tls_connect: handshake with tls-v1-0.badssl.com:1010 failed"

if [ "$fails" -gt 0 ]; then
  echo "$fails online TLS check(s) failed"
  exit 1
fi
echo "all online TLS checks passed"
