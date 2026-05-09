#!/usr/bin/env bash
# tests/lsp/smoke_diagnostics.sh — verifies Phase 2b: parse/lex errors
# from the Phase 0b error sink surface as publishDiagnostics
# notifications with non-empty diagnostics arrays containing the
# expected message + 0-indexed range.
#
# Two-step check per file: open broken source → assert non-empty
# diagnostics, then change to clean source → assert the diagnostics
# clear. This catches both "errors don't surface" and "errors stick
# after the user fixes them" regressions.

set -e

cd "$(dirname "$0")/../.."

BIN="${BIN:-./build/gem}"
if [ ! -x "$BIN" ]; then
  echo "smoke: $BIN not found; run 'make build' first" >&2
  exit 1
fi

FIXTURE_DIR="$(mktemp -d -t gem_lsp_diag.XXXXXX)"
trap 'rm -rf "$FIXTURE_DIR"' EXIT
FIXTURE="$FIXTURE_DIR/broken.gem"
URI="file://$FIXTURE"

# Source with a parse error: `fn` block missing its `end`. Phase 0b
# parser reports "expected 'end' but got 'EOF'" (or similar) without
# crashing, which is exactly the diagnostics path we're exercising.
BROKEN=$'fn add(a, b)\n  a + b\n'
CLEAN=$'fn add(a, b)\n  a + b\nend\n'

# Write the file so its path is real (the LSP doesn't read it from
# disk — it uses the text in didOpen — but having a valid path keeps
# uri_to_path / find_project_root happy).
printf '%s' "$BROKEN" > "$FIXTURE"

json_escape() {
  python3 -c 'import json,sys; print(json.dumps(sys.stdin.read()))'
}

BROKEN_JSON=$(printf '%s' "$BROKEN" | json_escape)
CLEAN_JSON=$(printf '%s' "$CLEAN" | json_escape)

make_frame() {
  local body="$1"
  local len="${#body}"
  printf 'Content-Length: %d\r\n\r\n%s' "$len" "$body"
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

OUT=$(printf '%s' "$INPUT" | "$BIN" lsp)
RC=$?

if [ "$RC" -ne 0 ]; then
  echo "FAIL: server exit code $RC (expected 0 after acked shutdown)" >&2
  printf '%s\n' "$OUT" >&2
  exit 1
fi

# Two publishDiagnostics notifications expected (one per didOpen/didChange).
PUB_COUNT=$(printf '%s' "$OUT" | grep -c '"method":"textDocument/publishDiagnostics"' || true)
if [ "$PUB_COUNT" -lt 2 ]; then
  echo "FAIL: expected >=2 publishDiagnostics, got $PUB_COUNT" >&2
  printf '%s\n' "$OUT" >&2
  exit 1
fi

# Use python to parse each frame body (keyed by Content-Length headers)
# and pull out the publishDiagnostics payloads in order. Strict shape
# checks are easier in python than with shell pattern matching.
python3 - "$OUT" "$URI" <<'PY'
import json, sys, re

raw = sys.argv[1]
expected_uri = sys.argv[2]

# Split frames: header block ends in \r\n\r\n, body length given by Content-Length.
i = 0
publishes = []
while i < len(raw):
    m = re.match(r'Content-Length:\s*(\d+)\r\n\r\n', raw[i:])
    if not m:
        # No more frames (or non-frame stderr leakage at the boundary).
        break
    length = int(m.group(1))
    i += m.end()
    body = raw[i:i+length]
    i += length
    msg = json.loads(body)
    if msg.get("method") == "textDocument/publishDiagnostics":
        publishes.append(msg["params"])

# We expect at least: one for didOpen (broken → non-empty) and one for
# didChange (clean → empty), each on the doc URI.
broken_pub = None
clean_pub = None
for p in publishes:
    if p["uri"] != expected_uri:
        # Cross-file publishes are allowed (resolve_loads not wired today,
        # so unexpected today, but tolerate without failing).
        continue
    if broken_pub is None:
        broken_pub = p
    else:
        clean_pub = p

if broken_pub is None or clean_pub is None:
    print("FAIL: missing broken or clean publishDiagnostics for doc URI", file=sys.stderr)
    print(json.dumps(publishes, indent=2), file=sys.stderr)
    sys.exit(1)

diags = broken_pub["diagnostics"]
if not diags:
    print("FAIL: broken-source publish had empty diagnostics", file=sys.stderr)
    sys.exit(1)

d = diags[0]
# Severity 1 = Error.
if d.get("severity") != 1:
    print(f"FAIL: severity != 1 (got {d.get('severity')})", file=sys.stderr); sys.exit(1)
if d.get("source") != "gem":
    print(f"FAIL: source != 'gem' (got {d.get('source')})", file=sys.stderr); sys.exit(1)
if "message" not in d or not d["message"]:
    print("FAIL: empty message", file=sys.stderr); sys.exit(1)
# Range fields are 0-indexed integers.
rng = d.get("range", {})
for end in ("start", "end"):
    pos = rng.get(end, {})
    if not isinstance(pos.get("line"), int) or not isinstance(pos.get("character"), int):
        print(f"FAIL: range.{end} missing integer line/character: {pos}", file=sys.stderr)
        sys.exit(1)
# The error is on the missing `end` after line "  a + b\n", so it points
# somewhere on line 2 (0-indexed) or beyond — definitely > 0.
if rng["start"]["line"] < 1:
    print(f"FAIL: expected error on line >= 1 (0-indexed), got {rng['start']['line']}", file=sys.stderr)
    sys.exit(1)
# end position should be >= start.
if (rng["end"]["line"], rng["end"]["character"]) < (rng["start"]["line"], rng["start"]["character"]):
    print(f"FAIL: range end before start: {rng}", file=sys.stderr); sys.exit(1)

# After the change to clean source, diagnostics should be empty.
if clean_pub["diagnostics"]:
    print("FAIL: clean-source publish had non-empty diagnostics:", file=sys.stderr)
    print(json.dumps(clean_pub, indent=2), file=sys.stderr)
    sys.exit(1)

print("ok: lsp diagnostics smoke test")
PY
