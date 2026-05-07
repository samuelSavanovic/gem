#!/usr/bin/env bash
# tests/lsp/smoke_symbols.sh — exercises the Phase 1b symbol table by
# opening a small file, requesting `gem/debug/symbols`, and asserting
# the expected function/var/import names appear. The debug method is
# gated on $GEM_LSP_DEBUG so it isn't reachable from a real editor.

set -e

cd "$(dirname "$0")/../.."

BIN="${BIN:-./build/gem}"
if [ ! -x "$BIN" ]; then
  echo "smoke: $BIN not found; run 'make build' first" >&2
  exit 1
fi

# Write a fixture file the LSP can resolve `load "std/string"` against.
FIXTURE_DIR="$(mktemp -d -t gem_lsp_symbols.XXXXXX)"
trap 'rm -rf "$FIXTURE_DIR"' EXIT
FIXTURE="$FIXTURE_DIR/sample.gem"
cat > "$FIXTURE" <<'GEM'
load "std/string"

let greeting = "hello"

fn shout(msg)
  string.upper(msg)
end

fn count(items)
  let n = 0
  for it in items
    n = n + 1
  end
  n
end
GEM

URI="file://$FIXTURE"
TEXT_CONTENT=$(cat "$FIXTURE")

# JSON-escape the text body. Newlines → \n, quotes → \". Keep the
# Python invocation tiny so this script doesn't become a snowflake.
TEXT_JSON=$(python3 -c 'import json,sys; print(json.dumps(sys.stdin.read()))' <<< "$TEXT_CONTENT")

make_frame() {
  local body="$1"
  local len="${#body}"
  printf 'Content-Length: %d\r\n\r\n%s' "$len" "$body"
}

INIT='{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"processId":null,"rootUri":null,"capabilities":{}}}'
INITED='{"jsonrpc":"2.0","method":"initialized","params":{}}'
OPEN='{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"'"$URI"'","languageId":"gem","version":1,"text":'"$TEXT_JSON"'}}}'
SYMS='{"jsonrpc":"2.0","id":2,"method":"gem/debug/symbols","params":{"uri":"'"$URI"'"}}'
SHUT='{"jsonrpc":"2.0","id":3,"method":"shutdown","params":null}'
EXIT='{"jsonrpc":"2.0","method":"exit","params":null}'

INPUT=$(
  make_frame "$INIT"
  make_frame "$INITED"
  make_frame "$OPEN"
  make_frame "$SYMS"
  make_frame "$SHUT"
  make_frame "$EXIT"
)

OUT=$(printf '%s' "$INPUT" | GEM_LSP_DEBUG=1 "$BIN" lsp)
RC=$?

if [ "$RC" -ne 0 ]; then
  echo "FAIL: server exit code $RC (expected 0 after acked shutdown)" >&2
  printf '%s\n' "$OUT" >&2
  exit 1
fi

# Pull just the response to id=2 (the symbols probe) for assertion
# scoping. Frames are concatenated on stdout; grep for the line that
# contains "id":2 — JSON is a single line per encode.
SYMS_LINE=$(printf '%s' "$OUT" | grep -E '"id":2[,}]' || true)
if [ -z "$SYMS_LINE" ]; then
  echo "FAIL: no response for gem/debug/symbols (id=2)" >&2
  printf '%s\n' "$OUT" >&2
  exit 1
fi

# Sanity-check the response contains the expected names. We're not
# asserting positions — those are exercised by build_symbols' walk; the
# smoke test guarantees the wire path works end-to-end.
for needle in '"name":"shout"' '"name":"count"' '"name":"greeting"' '"name":"msg"' '"name":"items"' '"name":"n"' '"name":"string"'; do
  case "$SYMS_LINE" in
    *"$needle"*) ;;
    *) echo "FAIL: missing $needle in symbols response" >&2; printf '%s\n' "$SYMS_LINE" >&2; exit 1 ;;
  esac
done

# Negative assertion: gensym names must not leak into the table.
for unwanted in '"_for_' '"_pdestr' '"_d1"' '"_d2"'; do
  case "$SYMS_LINE" in
    *"$unwanted"*) echo "FAIL: gensym name $unwanted leaked into symbols" >&2; printf '%s\n' "$SYMS_LINE" >&2; exit 1 ;;
  esac
done

# With $GEM_LSP_DEBUG unset the same probe must be rejected.
INPUT_NODEBUG=$(
  make_frame "$INIT"
  make_frame "$INITED"
  make_frame "$OPEN"
  make_frame "$SYMS"
  make_frame "$SHUT"
  make_frame "$EXIT"
)
OUT_NODEBUG=$(printf '%s' "$INPUT_NODEBUG" | "$BIN" lsp)
case "$OUT_NODEBUG" in
  *'"id":2'*'"error"'*'Method not found'*) ;;
  *) echo "FAIL: gem/debug/symbols not gated on GEM_LSP_DEBUG" >&2; printf '%s\n' "$OUT_NODEBUG" >&2; exit 1 ;;
esac

echo "ok: lsp symbols smoke test"
