#!/usr/bin/env bash
# A runtime error's `-->` source line must be found when the binary runs
# from another directory (trace paths are project-relative). The runtime
# looks under $GEM_SOURCE_ROOT, the cwd, the executable's directory and its
# ancestors, then the cwd's ancestors; a file too short to have the line is
# skipped. These cases depend on the cwd, so they live here rather than in
# examples/.
#
# Run from the repo root: tests/check_trace_source.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi
GEM=$(cd "$(dirname "$GEM")" && pwd)/$(basename "$GEM")

T=$(mktemp -d /tmp/gem_trace_src.XXXXXX)
trap 'rm -rf "$T"' EXIT

mkdir -p "$T/proj/src" "$T/proj/lib" "$T/proj/build" "$T/proj/src/deep/er" \
         "$T/elsewhere" "$T/decoy/lib" "$T/flat"
touch "$T/proj/gem.toml"
cat > "$T/proj/lib/util.gem" <<'GEM'
# helper module

fn check(x)
  if x > 1
    error("too big: {x}")
  end
  return x
end

export check
GEM
cat > "$T/proj/src/app.gem" <<'GEM'
load "lib/util"
print(util.check(1))
print(util.check(2))
GEM
cat > "$T/flat/app.gem" <<'GEM'
fn f(x)
  error("flat {x}")
end
f(3)
GEM
echo "# one line" > "$T/decoy/lib/util.gem"

(cd "$T/proj" && "$GEM" src/app.gem -o build/app) || { echo "FAIL: compile"; exit 1; }
cp "$T/proj/build/app" "$T/elsewhere/app"
"$GEM" "$T/flat/app.gem" -o "$T/flat/app" || { echo "FAIL: compile flat"; exit 1; }

SRC='  --> lib/util.gem:5'
fails=0
total=0

# check <name> <expect: yes|no> <expected --> line> <cwd> <cmd...>
check() {
  local name=$1 want=$2 arrow=$3 dir=$4; shift 4
  total=$((total + 1))
  local out
  out=$(cd "$dir" && "$@" 2>&1 >/dev/null)
  local has=no
  if grep -qxF -- "$arrow" <<<"$out"; then has=yes; fi
  if [ "$has" != "$want" ] || ! grep -q "^Stack trace:" <<<"$out"; then
    echo "FAIL: $name (source line expected: $want)"
    echo "$out" | sed 's/^/    /'
    fails=$((fails + 1))
    return
  fi
  if [ "$want" = yes ] && ! grep -qF 'error("' <<<"$out"; then
    echo "FAIL: $name printed the wrong source line"
    echo "$out" | sed 's/^/    /'
    fails=$((fails + 1))
  fi
}

check "from the project root"         yes "$SRC" "$T/proj"          "$T/proj/build/app"
check "from outside (exe dir parent)" yes "$SRC" "$T"               "$T/proj/build/app"
check "from /"                        yes "$SRC" /                  "$T/proj/build/app"
check "copied binary, cwd ancestor"   yes "$SRC" "$T/proj/src/deep/er" "$T/elsewhere/app"
check "copied binary, no source"      no  "$SRC" "$T"               "$T/elsewhere/app"
check "short decoy is skipped"        no  "$SRC" "$T/decoy"         "$T/elsewhere/app"
check "GEM_SOURCE_ROOT"               yes "$SRC" /  env GEM_SOURCE_ROOT="$T/proj" "$T/elsewhere/app"
check "GEM_SOURCE_ROOT wins over decoy" yes "$SRC" "$T/decoy" env GEM_SOURCE_ROOT="$T/proj" "$T/elsewhere/app"
check "absolute path, from /"         yes "  --> $T/flat/app.gem:2" / "$T/flat/app"

if [ "$fails" -gt 0 ]; then
  echo
  echo "$fails / $total trace source checks failed"
  exit 1
fi
echo "all $total trace source checks passed"
