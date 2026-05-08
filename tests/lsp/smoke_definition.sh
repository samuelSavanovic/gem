#!/usr/bin/env bash
# tests/lsp/smoke_definition.sh — exercises Phase 2a goto-definition.
# Three positions are checked: a same-file function call, a same-file
# var reference, and a cross-file member access into std/string.

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
# Layout matters — assertions reference 0-based line numbers.
#   line 0: load "std/string"
#   line 1: <blank>
#   line 2: let greeting = "hello"
#   line 3: <blank>
#   line 4: fn shout(msg)
#   line 5:   string.upper(msg)
#   line 6: end
#   line 7: <blank>
#   line 8: fn main()
#   line 9:   shout(greeting)
#   line 10: end
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
# Cursor on `shout` at line 9 col 2 (0-based) — first 's' of `shout(greeting)`.
DEF_FN='{"jsonrpc":"2.0","id":10,"method":"textDocument/definition","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":9,"character":2}}}'
# Cursor on `greeting` at line 9 col 8 (0-based) — first 'g' of `greeting`.
DEF_VAR='{"jsonrpc":"2.0","id":11,"method":"textDocument/definition","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":9,"character":8}}}'
# Cursor on `upper` at line 5 col 9 (0-based) — first 'u' of `upper` after `string.`.
DEF_MOD='{"jsonrpc":"2.0","id":12,"method":"textDocument/definition","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":5,"character":9}}}'
# Cursor on `string` at line 5 col 2 (0-based) — module name itself, jumps to module top.
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

# Same-file function: definition at line 4 (0-based) where `fn shout(msg)` sits.
assert_contains 10 '"line":4' "goto-def function"
assert_contains 10 "$URI" "goto-def function (uri)"

# Same-file var: definition at line 2 where `let greeting` sits.
assert_contains 11 '"line":2' "goto-def variable"
assert_contains 11 "$URI" "goto-def variable (uri)"

# Cross-file member: should land in std/string.gem.
assert_contains 12 'std/string.gem' "goto-def std/string.upper"

# Module name itself: jumps to the top of the imported file (line 0).
assert_contains 13 '"line":0' "goto-def module name (line)"
assert_contains 13 'std/string.gem' "goto-def module name (file)"

echo "ok: lsp definition smoke test"
