#!/usr/bin/env bash
# completion of a module's fields (`string.`), a local table's (`local.`) and a partial name (`pri`,
# which must offer print); an empty prefix in a file with many names gets a cut list marked incomplete

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
# each request sends its own full text with didChange (sync is Full)
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

encode() {
  python3 -c 'import json,sys; print(json.dumps(sys.stdin.read()))'
}

# cursor right after `string.`: line 5, char 9
TEXT_MOD=$(cat <<'GEM'
load "std/string"

let local = {alpha: 1, beta: 2}

fn _placeholder()
  string.
end
GEM
)
# after `local.`: line 5, char 8
TEXT_LOCAL=$(cat <<'GEM'
load "std/string"

let local = {alpha: 1, beta: 2}

fn _placeholder()
  local.
end
GEM
)
# after `pri`: line 5, char 5
TEXT_IDENT=$(cat <<'GEM'
load "std/string"

let local = {alpha: 1, beta: 2}

fn _placeholder()
  pri
end
GEM
)

# 150 lets plus the builtins: more than the server sends at once; cursor on the empty line 151
TEXT_MANY=$(for i in $(seq 1 150); do echo "let name_$i = $i"; done; echo; echo)

JSON_MOD=$(printf '%s' "$TEXT_MOD" | encode)
JSON_LOCAL=$(printf '%s' "$TEXT_LOCAL" | encode)
JSON_IDENT=$(printf '%s' "$TEXT_IDENT" | encode)
JSON_MANY=$(printf '%s\n' "$TEXT_MANY" | encode)

INIT='{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"processId":null,"rootUri":null,"capabilities":{}}}'
INITED='{"jsonrpc":"2.0","method":"initialized","params":{}}'
OPEN='{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"'"$URI"'","languageId":"gem","version":1,"text":'"$JSON_MOD"'}}}'
COMP_MOD='{"jsonrpc":"2.0","id":20,"method":"textDocument/completion","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":5,"character":9}}}'

CHANGE_LOCAL='{"jsonrpc":"2.0","method":"textDocument/didChange","params":{"textDocument":{"uri":"'"$URI"'","version":2},"contentChanges":[{"text":'"$JSON_LOCAL"'}]}}'
COMP_LOCAL='{"jsonrpc":"2.0","id":21,"method":"textDocument/completion","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":5,"character":8}}}'

CHANGE_IDENT='{"jsonrpc":"2.0","method":"textDocument/didChange","params":{"textDocument":{"uri":"'"$URI"'","version":3},"contentChanges":[{"text":'"$JSON_IDENT"'}]}}'
COMP_IDENT='{"jsonrpc":"2.0","id":22,"method":"textDocument/completion","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":5,"character":5}}}'

CHANGE_MANY='{"jsonrpc":"2.0","method":"textDocument/didChange","params":{"textDocument":{"uri":"'"$URI"'","version":4},"contentChanges":[{"text":'"$JSON_MANY"'}]}}'
COMP_MANY='{"jsonrpc":"2.0","id":23,"method":"textDocument/completion","params":{"textDocument":{"uri":"'"$URI"'"},"position":{"line":151,"character":0}}}'

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
  make_frame "$CHANGE_MANY"
  make_frame "$COMP_MANY"
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

assert_contains 20 '"label":"upper"' "module field completion (upper)"
assert_contains 20 '"label":"trim"' "module field completion (trim)"

assert_contains 21 '"label":"alpha"' "local field completion (alpha)"
assert_contains 21 '"label":"beta"' "local field completion (beta)"

assert_contains 22 '"label":"print"' "identifier completion (builtin print)"
assert_contains 22 '"isIncomplete":false' "identifier completion (whole list)"

assert_contains 23 '"isIncomplete":true' "identifier completion past the cap (cut list)"

echo "ok: lsp completion smoke test"
