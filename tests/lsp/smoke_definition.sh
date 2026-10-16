#!/usr/bin/env bash
# goto-definition on a same-file fn call, a same-file variable, a std/string member and the module name

set -e

cd "$(dirname "$0")/../.."

BIN="${BIN:-./build/gem}"
if [ ! -x "$BIN" ]; then
  echo "smoke: $BIN not found; run 'make build' first" >&2
  exit 1
fi

FIXTURE_DIR="$(mktemp -d -t gem_lsp_def.XXXXXX)"
trap 'rm -rf "$FIXTURE_DIR"' EXIT
FIXTURE="$FIXTURE_DIR/sample.gem"
# the requests and assertions below use 0-based positions in this text
cat > "$FIXTURE" <<'GEM'
load "std/string"

let greeting = "hello"

fn shout(msg)
  string.upper(msg)
end

fn main()
  shout(greeting)
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
# `shout` in shout(greeting)
DEF_FN='{"jsonrpc":"2.0","id":10,"method":"textDocument/definition","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":9,"character":2}}}'
# `greeting`
DEF_VAR='{"jsonrpc":"2.0","id":11,"method":"textDocument/definition","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":9,"character":8}}}'
# `upper` in string.upper
DEF_MOD='{"jsonrpc":"2.0","id":12,"method":"textDocument/definition","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":5,"character":9}}}'
# `string`, the module name
DEF_MOD_NAME='{"jsonrpc":"2.0","id":13,"method":"textDocument/definition","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":5,"character":2}}}'
SHUT='{"jsonrpc":"2.0","id":99,"method":"shutdown","params":null}'
EXIT='{"jsonrpc":"2.0","method":"exit","params":null}'

INPUT=$(
  make_frame "$INIT"
  make_frame "$INITED"
  make_frame "$OPEN"
  make_frame "$DEF_FN"
  make_frame "$DEF_VAR"
  make_frame "$DEF_MOD"
  make_frame "$DEF_MOD_NAME"
  make_frame "$SHUT"
  make_frame "$EXIT"
)

OUT=$(printf '%s' "$INPUT" | "$BIN" lsp)
RC=$?

if [ "$RC" -ne 0 ]; then
  echo "FAIL: server exit code $RC (expected 0 after acked shutdown)" >&2
  printf '%s\n' "$OUT" >&2
  exit 1
fi

extract_response() {
  local id="$1"
  printf '%s' "$OUT" | grep -E '"id":'"$id"'[,}]' || true
}

assert_contains() {
  local id="$1" needle="$2" desc="$3"
  local line
  line=$(extract_response "$id")
  if [ -z "$line" ]; then
    echo "FAIL: no response for $desc (id=$id)" >&2
    printf '%s\n' "$OUT" >&2
    exit 1
  fi
  case "$line" in
    *"$needle"*) ;;
    *)
      echo "FAIL: $desc response missing $needle" >&2
      printf '%s\n' "$line" >&2
      exit 1
      ;;
  esac
}

assert_contains 10 '"line":4' "goto-def function"
assert_contains 10 "$URI" "goto-def function (uri)"

assert_contains 11 '"line":2' "goto-def variable"
assert_contains 11 "$URI" "goto-def variable (uri)"

assert_contains 12 'std/string.gem' "goto-def std/string.upper"

assert_contains 13 '"line":0' "goto-def module name (line)"
assert_contains 13 'std/string.gem' "goto-def module name (file)"

echo "ok: lsp definition smoke test"
