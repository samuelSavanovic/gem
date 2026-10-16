#!/usr/bin/env bash
# the symbol table through `gem/debug/symbols` (only with $GEM_LSP_DEBUG set): the fixture's fns,
# variables and imports appear, generated names don't

set -e

cd "$(dirname "$0")/../.."

BIN="${BIN:-./build/gem}"
if [ ! -x "$BIN" ]; then
  echo "smoke: $BIN not found; run 'make build' first" >&2
  exit 1
fi

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

fn label(n)
  let size = if n > 10 then "big" else "small" end
  size
end
GEM

URI="file://$FIXTURE"
TEXT_CONTENT=$(cat "$FIXTURE")

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

# each JSON body is on one line, so grep finds the id=2 response
SYMS_LINE=$(printf '%s' "$OUT" | grep -E '"id":2[,}]' || true)
if [ -z "$SYMS_LINE" ]; then
  echo "FAIL: no response for gem/debug/symbols (id=2)" >&2
  printf '%s\n' "$OUT" >&2
  exit 1
fi

for needle in '"name":"shout"' '"name":"count"' '"name":"greeting"' '"name":"msg"' '"name":"items"' '"name":"n"' '"name":"string"' '"name":"size"'; do
  case "$SYMS_LINE" in
    *"$needle"*) ;;
    *) echo "FAIL: missing $needle in symbols response" >&2; printf '%s\n' "$SYMS_LINE" >&2; exit 1 ;;
  esac
done

for unwanted in '"__'; do
  case "$SYMS_LINE" in
    *"$unwanted"*) echo "FAIL: gensym name $unwanted leaked into symbols" >&2; printf '%s\n' "$SYMS_LINE" >&2; exit 1 ;;
  esac
done

# with $GEM_LSP_DEBUG unset the same probe must be rejected
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
