#!/usr/bin/env bash
# tests/lsp/smoke_completion.sh — exercises Phase 2a completion. Three
# checks: module-import field completion (`string.`), local-table field
# completion (`t.` after a literal init), and identifier completion
# returning at least one builtin.

set -e

cd "$(dirname "$0")/../.."

BIN="${BIN:-./build/gem}"
if [ ! -x "$BIN" ]; then
  echo "smoke: $BIN not found; run 'make build' first" >&2
  exit 1
fi

FIXTURE_DIR="$(mktemp -d -t gem_lsp_comp.XXXXXX)"
trap 'rm -rf "$FIXTURE_DIR"' EXIT
FIXTURE="$FIXTURE_DIR/sample.gem"
# We embed three trailing trigger sites — `string.`, `local.`, and `pri`
# — and ship per-request `text` updates so each completion request sees
# a different trailing cursor context. didChange replaces the full
# document under TextDocumentSyncKind.Full.
cat > "$FIXTURE" <<'GEM'
load "std/string"

let local = {alpha: 1, beta: 2}

fn _placeholder()
  string.
end
GEM

URI="file://$FIXTURE"

make_frame() {
  local body="$1"
  local len="${#body}"
  printf 'Content-Length: %d\r\n\r\n%s' "$len" "$body"
}

# JSON-encode `text` with python — newlines + quotes need escaping.
encode() {
  python3 -c 'import json,sys; print(json.dumps(sys.stdin.read()))'
}

# Build different document texts for each request scenario. Each text
# ends with the trigger context at a known line/column, so the request
# can target it with line/character.
#
# TEXT_MOD: cursor just after `string.` on line 5, char 9.
TEXT_MOD=$(cat <<'GEM'
load "std/string"

let local = {alpha: 1, beta: 2}

fn _placeholder()
  string.
end
GEM
)
# TEXT_LOCAL: cursor after `local.` on line 5, char 8.
TEXT_LOCAL=$(cat <<'GEM'
load "std/string"

let local = {alpha: 1, beta: 2}

fn _placeholder()
  local.
end
GEM
)
# TEXT_IDENT: trailing partial identifier `pri` on line 5, char 5.
TEXT_IDENT=$(cat <<'GEM'
load "std/string"

let local = {alpha: 1, beta: 2}

fn _placeholder()
  pri
end
GEM
)

JSON_MOD=$(printf '%s' "$TEXT_MOD" | encode)
JSON_LOCAL=$(printf '%s' "$TEXT_LOCAL" | encode)
JSON_IDENT=$(printf '%s' "$TEXT_IDENT" | encode)

INIT='{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"processId":null,"rootUri":null,"capabilities":{}}}'
INITED='{"jsonrpc":"2.0","method":"initialized","params":{}}'
OPEN='{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"'"$URI"'","languageId":"gem","version":1,"text":'"$JSON_MOD"'}}}'
COMP_MOD='{"jsonrpc":"2.0","id":20,"method":"textDocument/completion","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":5,"character":9}}}'

CHANGE_LOCAL='{"jsonrpc":"2.0","method":"textDocument/didChange","params":{"textDocument":{"uri":"'"$URI"'","version":2},"contentChanges":[{"text":'"$JSON_LOCAL"'}]}}'
COMP_LOCAL='{"jsonrpc":"2.0","id":21,"method":"textDocument/completion","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":5,"character":8}}}'

CHANGE_IDENT='{"jsonrpc":"2.0","method":"textDocument/didChange","params":{"textDocument":{"uri":"'"$URI"'","version":3},"contentChanges":[{"text":'"$JSON_IDENT"'}]}}'
COMP_IDENT='{"jsonrpc":"2.0","id":22,"method":"textDocument/completion","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":5,"character":5}}}'

SHUT='{"jsonrpc":"2.0","id":99,"method":"shutdown","params":null}'
EXIT='{"jsonrpc":"2.0","method":"exit","params":null}'

INPUT=$(
  make_frame "$INIT"
  make_frame "$INITED"
  make_frame "$OPEN"
  make_frame "$COMP_MOD"
  make_frame "$CHANGE_LOCAL"
  make_frame "$COMP_LOCAL"
  make_frame "$CHANGE_IDENT"
  make_frame "$COMP_IDENT"
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

# Module-field completion: `string.` should surface std/string exports.
assert_contains 20 '"label":"upper"' "module field completion (upper)"
assert_contains 20 '"label":"trim"' "module field completion (trim)"

# Local-table field completion: `local.` should surface keys from the literal.
assert_contains 21 '"label":"alpha"' "local field completion (alpha)"
assert_contains 21 '"label":"beta"' "local field completion (beta)"

# Identifier completion with prefix `pri` should include `print` (builtin).
assert_contains 22 '"label":"print"' "identifier completion (builtin print)"

echo "ok: lsp completion smoke test"
