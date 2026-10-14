#!/usr/bin/env bash
# The run-mode binary cache (compiler/main.gem, "Binary cache"): `gem
# file.gem` reuses the binary of an earlier run with the same C, runtime,
# cc flags and extern headers, and builds a new one when any of them
# changes. Checks the entries the cache holds, not timings.
#
# Run from the repo root: tests/check_cache.sh

set -u
cd "$(dirname "$0")/.."
GEM=$(cd "$(dirname "${GEM:-build/gem}")" && pwd)/$(basename "${GEM:-build/gem}")
T=$(mktemp -d "${TMPDIR:-/tmp}/gem_cache.XXXXXX")
trap 'rm -rf "$T"' EXIT
export GEM_CACHE_DIR="$T/cache"
unset GEM_CACHE
fails=0
fail() { echo "FAIL: $*"; fails=$((fails + 1)); }
entries() { if [ -d "$GEM_CACHE_DIR" ]; then ls "$GEM_CACHE_DIR" | wc -l | tr -d ' '; else echo 0; fi; }

mkdir -p "$T/p"
cat > "$T/p/lib.gem" <<'GEM'
fn word() "one" end
export word
GEM
cat > "$T/p/hdr.h" <<'H'
static long long twice(long long x) { return 2 * x; }
H
cat > "$T/p/main.gem" <<'GEM'
load "./lib"
extern include "hdr.h"
extern fn twice(x: Int) -> Int
print(lib.word(), twice(21), len(argv()) - 1)
exit(if len(argv()) > 2 then 3 else 0 end)
GEM

out=$("$GEM" "$T/p/main.gem")
[ "$out" = "one 42 0" ] || fail "first run: $out"
[ "$(entries)" = 1 ] || fail "first run: $(entries) cache entries, expected 1"

out=$("$GEM" "$T/p/main.gem" a)
[ "$out" = "one 42 1" ] || fail "cached run with an argument: $out"
[ "$(entries)" = 1 ] || fail "cached run: $(entries) entries, expected 1"
"$GEM" "$T/p/main.gem" a b > /dev/null
[ $? = 3 ] || fail "cached run: the exit status is not passed on"

# a loaded module changes: a new binary
sed -i.bak 's/"one"/"two"/' "$T/p/lib.gem"
out=$("$GEM" "$T/p/main.gem")
[ "$out" = "two 42 0" ] || fail "module changed: $out"
[ "$(entries)" = 2 ] || fail "module changed: $(entries) entries, expected 2"

# an extern header changes (the C has its path, not its contents)
sed -i.bak 's/2 \* x/3 * x/' "$T/p/hdr.h"
out=$("$GEM" "$T/p/main.gem")
[ "$out" = "two 63 0" ] || fail "header changed: $out"
[ "$(entries)" = 3 ] || fail "header changed: $(entries) entries, expected 3"

# cc flags are part of the key
out=$(CFLAGS=-DGEM_CACHE_TEST "$GEM" "$T/p/main.gem")
[ "$(entries)" = 4 ] || fail "CFLAGS: $(entries) entries, expected 4"

# -o builds a binary of its own (-O2), outside the cache; GEM_CACHE=off
"$GEM" "$T/p/main.gem" -o "$T/p/out" && [ -x "$T/p/out" ] || fail "-o build"
[ "$(entries)" = 4 ] || fail "-o: $(entries) entries, expected 4"
echo 'print("off")' > "$T/p/off.gem"
out=$(GEM_CACHE=off "$GEM" "$T/p/off.gem")
[ "$out" = off ] && [ "$(entries)" = 4 ] || fail "GEM_CACHE=off: $out, $(entries) entries"

# a program that doesn't compile leaves no entry
echo 'print(undefined_name)' > "$T/p/bad.gem"
"$GEM" "$T/p/bad.gem" > /dev/null 2>&1 && fail "bad program ran"
[ "$(entries)" = 4 ] || fail "bad program: $(entries) entries, expected 4"

# an unwritable cache directory: the program still runs, uncached
out=$(GEM_CACHE_DIR=/dev/null/nope "$GEM" "$T/p/off.gem")
[ "$out" = off ] || fail "unwritable cache: $out"

if [ $fails -eq 0 ]; then echo "cache: OK"; else exit 1; fi
