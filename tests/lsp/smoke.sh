#!/usr/bin/env bash
# a canned session (initialize, didOpen, didChange, shutdown, exit) piped into `gem lsp`, checking the
# framed responses; the source is clean, so both publishes are empty

set -e

cd "$(dirname "$0")/../.."

BIN="${BIN:-./build/gem}"
if [ ! -x "$BIN" ]; then
  echo "smoke: $BIN not found; run 'make build' first" >&2
  exit 1
fi

make_frame() {
  local body="$1"
  local len="${#body}"
  printf 'Content-Length: %d\r\n\r\n%s' "$len" "$body"
}

INIT='{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"processId":null,"rootUri":null,"capabilities":{}}}'
INITED='{"jsonrpc":"2.0","method":"initialized","params":{}}'
OPEN='{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"file:///tmp/x.gem","languageId":"gem","version":1,"text":"let x = 1\n"}}}'
CHANGE='{"jsonrpc":"2.0","method":"textDocument/didChange","params":{"textDocument":{"uri":"file:///tmp/x.gem","version":2},"contentChanges":[{"text":"let x = 2\n"}]}}'
CLOSE='{"jsonrpc":"2.0","method":"textDocument/didClose","params":{"textDocument":{"uri":"file:///tmp/x.gem"}}}'
SHUT='{"jsonrpc":"2.0","id":2,"method":"shutdown","params":null}'
EXIT='{"jsonrpc":"2.0","method":"exit","params":null}'

INPUT=$(
  make_frame "$INIT"
  make_frame "$INITED"
  make_frame "$OPEN"
  make_frame "$CHANGE"
  make_frame "$CLOSE"
  make_frame "$SHUT"
  make_frame "$EXIT"
)

OUT=$(printf '%s' "$INPUT" | "$BIN" lsp)
RC=$?

if [ "$RC" -ne 0 ]; then
  echo "FAIL: server exit code $RC (expected 0 after acked shutdown)" >&2
  echo "---- output ----" >&2
  printf '%s\n' "$OUT" >&2
  exit 1
fi

case "$OUT" in
  *'"id":1'*) ;;
  *) echo "FAIL: no response for initialize (id=1)" >&2; printf '%s\n' "$OUT" >&2; exit 1 ;;
esac
case "$OUT" in
  *'"name":"gem-lsp"'*) ;;
  *) echo "FAIL: missing serverInfo.name=gem-lsp" >&2; printf '%s\n' "$OUT" >&2; exit 1 ;;
esac
case "$OUT" in
  *'"textDocumentSync":1'*) ;;
  *) echo "FAIL: missing textDocumentSync=1 capability" >&2; printf '%s\n' "$OUT" >&2; exit 1 ;;
esac

PUB_COUNT=$(printf '%s' "$OUT" | grep -c 'publishDiagnostics' || true)
if [ "$PUB_COUNT" -lt 2 ]; then
  echo "FAIL: expected >=2 publishDiagnostics, got $PUB_COUNT" >&2
  printf '%s\n' "$OUT" >&2
  exit 1
fi

case "$OUT" in
  *'"id":2'*'"result":null'*) ;;
  *) echo "FAIL: no shutdown response (id=2, result=null)" >&2; printf '%s\n' "$OUT" >&2; exit 1 ;;
esac

echo "ok: lsp smoke test"
