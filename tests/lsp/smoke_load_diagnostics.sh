#!/usr/bin/env bash
# tests/lsp/smoke_load_diagnostics.sh — a `load` of a missing module (or a
# directory) surfaces as a publishDiagnostics error at the `load` line,
# without crashing the server, and clears once the load is removed.

set -e

cd "$(dirname "$0")/../.."

BIN="${BIN:-./build/gem}"
if [ ! -x "$BIN" ]; then
  echo "smoke: $BIN not found; run 'make build' first" >&2
  exit 1
fi

FIXTURE_DIR="$(mktemp -d -t gem_lsp_load.XXXXXX)"
trap 'rm -rf "$FIXTURE_DIR"' EXIT
mkdir -p "$FIXTURE_DIR/mods"
FIXTURE="$FIXTURE_DIR/main.gem"
URI="file://$FIXTURE"

BROKEN=$'print(1)\nload "./mods/nope"\nload "std/nope"\nload "./mods"\n'
CLEAN=$'print(1)\n'
printf '%s' "$BROKEN" > "$FIXTURE"

json_escape() {
  python3 -c 'import json,sys; print(json.dumps(sys.stdin.read()))'
}
BROKEN_JSON=$(printf '%s' "$BROKEN" | json_escape)
CLEAN_JSON=$(printf '%s' "$CLEAN" | json_escape)

make_frame() {
  local body="$1"
  printf 'Content-Length: %d\r\n\r\n%s' "${#body}" "$body"
}

INIT='{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"processId":null,"rootUri":null,"capabilities":{}}}'
INITED='{"jsonrpc":"2.0","method":"initialized","params":{}}'
OPEN='{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"'"$URI"'","languageId":"gem","version":1,"text":'"$BROKEN_JSON"'}}}'
CHANGE='{"jsonrpc":"2.0","method":"textDocument/didChange","params":{"textDocument":{"uri":"'"$URI"'","version":2},"contentChanges":[{"text":'"$CLEAN_JSON"'}]}}'
SHUT='{"jsonrpc":"2.0","id":2,"method":"shutdown","params":null}'
EXIT='{"jsonrpc":"2.0","method":"exit","params":null}'

INPUT=$(
  make_frame "$INIT"
  make_frame "$INITED"
  make_frame "$OPEN"
  make_frame "$CHANGE"
  make_frame "$SHUT"
  make_frame "$EXIT"
)

set +e
OUT=$(printf '%s' "$INPUT" | "$BIN" lsp)
RC=$?
set -e
if [ "$RC" -ne 0 ]; then
  echo "FAIL: server exit code $RC (expected 0 after acked shutdown)" >&2
  printf '%s\n' "$OUT" >&2
  exit 1
fi

python3 - "$OUT" "$URI" <<'PY'
import json, sys, re

raw, uri = sys.argv[1], sys.argv[2]
i, publishes = 0, []
while i < len(raw):
    m = re.match(r'Content-Length:\s*(\d+)\r\n\r\n', raw[i:])
    if not m:
        break
    i += m.end()
    n = int(m.group(1))
    msg = json.loads(raw[i:i+n])
    i += n
    if msg.get("method") == "textDocument/publishDiagnostics" and msg["params"]["uri"] == uri:
        publishes.append(msg["params"]["diagnostics"])

if len(publishes) < 2:
    print(f"FAIL: expected 2 publishes for the doc, got {len(publishes)}", file=sys.stderr)
    sys.exit(1)

got = [(d["range"]["start"]["line"], d["range"]["start"]["character"], d["message"]) for d in publishes[0]]
want = [
    (1, "cannot find module `./mods/nope` (no file "),
    (2, "cannot find module `std/nope` (no file "),
    (3, "cannot find module `./mods` (no file "),
]
if len(got) != len(want):
    print(f"FAIL: expected {len(want)} diagnostics, got {got}", file=sys.stderr)
    sys.exit(1)
for (line, col, message), (wline, wprefix) in zip(got, want):
    if line != wline or col != 0 or not message.startswith(wprefix):
        print(f"FAIL: diagnostic {(line, col, message)!r}, expected line {wline} col 0 starting {wprefix!r}", file=sys.stderr)
        sys.exit(1)
if not got[0][2].endswith("/mods/nope.gem)"):
    print(f"FAIL: message should name the path it looked for: {got[0][2]!r}", file=sys.stderr)
    sys.exit(1)

if publishes[1]:
    print(f"FAIL: diagnostics did not clear after the change: {publishes[1]}", file=sys.stderr)
    sys.exit(1)

print("ok: lsp load diagnostics smoke test")
PY
