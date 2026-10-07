#!/usr/bin/env bash
# tests/lsp/smoke_malformed.sh — malformed messages don't end the session.
# Each case sends initialize and initialized, its own messages (a bad one,
# after a didOpen for some; the last case opens a document whose source
# ends in `x.`), then shutdown; the server must still answer the shutdown.
# A request that can't be handled gets a JSON-RPC error response with the
# expected code.

set -u

cd "$(dirname "$0")/../.."

BIN="${BIN:-./build/gem}"
if [ ! -x "$BIN" ]; then
  echo "smoke_malformed: $BIN not found; run 'make build' first" >&2
  exit 1
fi

make_frame() {
  local body="$1"
  printf 'Content-Length: %d\r\n\r\n%s' "${#body}" "$body"
}

INIT='{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"processId":null,"rootUri":null,"capabilities":{}}}'
INITED='{"jsonrpc":"2.0","method":"initialized","params":{}}'
OPEN='{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"file:///tmp/m.gem","languageId":"gem","version":1,"text":"let x = 1\nx."}}}'
SHUT='{"jsonrpc":"2.0","id":9,"method":"shutdown","params":null}'

fails=0
# check <name> <expected substring in output or ""> <bad message>...
check() {
  local name=$1 want=$2
  shift 2
  local out
  out=$(
    {
      make_frame "$INIT"
      make_frame "$INITED"
      for m in "$@"; do make_frame "$m"; done
      make_frame "$SHUT"
    } | "$BIN" lsp 2>/dev/null
  )
  if [[ "$out" != *'"id":9,"result":null'* ]]; then
    echo "FAIL: $name: the server didn't answer the shutdown"
    fails=$((fails + 1))
  elif [ -n "$want" ] && [[ "$out" != *"$want"* ]]; then
    echo "FAIL: $name: no $want in the output"
    fails=$((fails + 1))
  fi
}

check "invalid JSON" '"code":-32700' 'null}'
check "a body that isn't an object" '"code":-32600' '42'
check "an id without a method" '"id":3,"error":{"code":-32600' '{"jsonrpc":"2.0","id":3}'
check "definition with params null" '"id":4,"error":{"code":-32603' '{"jsonrpc":"2.0","id":4,"method":"textDocument/definition","params":null}'
check "completion without a character" '"id":5,"error":{"code":-32603' "$OPEN" '{"jsonrpc":"2.0","id":5,"method":"textDocument/completion","params":{"textDocument":{"uri":"file:///tmp/m.gem"},"position":{"line":0}}}'
check "didClose without a document" "" '{"jsonrpc":"2.0","method":"textDocument/didClose","params":{}}'
check "didChange without changes" "" "$OPEN" '{"jsonrpc":"2.0","method":"textDocument/didChange","params":{"textDocument":{"uri":"file:///tmp/m.gem","version":2}}}'
check "a document ending in x." '"method":"textDocument/publishDiagnostics"' "$OPEN"

if [ "$fails" -ne 0 ]; then
  exit 1
fi
echo "ok: lsp malformed-message smoke test"
