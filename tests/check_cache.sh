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

# $TMPDIR elsewhere than the cache: the entry is still made (the binary is
# built and linked under the cache's own directory)
mkdir -p "$T/othertmp"
echo 'print("tmp")' > "$T/p/tmp.gem"
out=$(TMPDIR="$T/othertmp" "$GEM" "$T/p/tmp.gem")
[ "$out" = tmp ] && [ "$(entries)" = 5 ] || fail "TMPDIR elsewhere: $out, $(entries) entries, expected 5"
# and on another file system, when one is at hand
devof() { stat -c %d "$1" 2>/dev/null || stat -f %d "$1"; }
other=""
for d in /dev/shm "${XDG_RUNTIME_DIR:-}"; do
  if [ -n "$d" ] && [ -d "$d" ] && [ -w "$d" ] && [ "$(devof "$d")" != "$(devof "$T")" ]; then other=$d; break; fi
done
if [ -n "$other" ]; then
  O=$(mktemp -d "$other/gem_cache.XXXXXX")
  echo 'print("fs")' > "$T/p/fs.gem"
  out=$(TMPDIR="$O" "$GEM" "$T/p/fs.gem")
  [ "$out" = fs ] && [ "$(entries)" = 6 ] || fail "TMPDIR on another file system: $out, $(entries) entries, expected 6"
  rm -rf "$O"
else
  echo "cache: no second file system at hand, skipping the cross-file-system case"
fi
# a run leaves nothing in .tmp
[ -z "$(ls "$GEM_CACHE_DIR/.tmp")" ] || fail ".tmp not empty: $(ls "$GEM_CACHE_DIR/.tmp")"

# an entry removed after its run: the next run builds it again
rm -rf "$GEM_CACHE_DIR"/*
out=$("$GEM" "$T/p/tmp.gem")
[ "$out" = tmp ] && [ "$(entries)" = 1 ] || fail "removed entry: $out, $(entries) entries, expected 1"

# 256 entries: the next new one empties the cache first
for i in $(seq 1 255); do mkdir "$GEM_CACHE_DIR/fake$i"; done
echo 'print("full")' > "$T/p/full.gem"
"$GEM" "$T/p/full.gem" > /dev/null || fail "full cache: the run failed"
[ "$(entries)" = 1 ] || fail "full cache: $(entries) entries after a new one, expected 1"
[ -d "$GEM_CACHE_DIR/.tmp" ] || fail "full cache: .tmp removed"

# the runtime's headers are part of the key: a copy of the install root
# whose gem.h changes
R="$T/root"
mkdir -p "$R/build"
cp -R runtime std "$R/"
cp build/libgem_runtime.a "$R/build/"
if [ -f build/tls_libs ]; then cp build/tls_libs "$R/build/"; fi
cp "$GEM" "$R/build/gem"
rm -rf "$GEM_CACHE_DIR"/*
"$R/build/gem" "$T/p/tmp.gem" > /dev/null || fail "copied install root"
echo '/* changed */' >> "$R/runtime/gem.h"
out=$("$R/build/gem" "$T/p/tmp.gem")
[ "$out" = tmp ] && [ "$(entries)" = 2 ] || fail "runtime header changed: $out, $(entries) entries, expected 2"

# the program gets gem's descriptors (a <(cmd) path), new, cached or not;
# a SIGTERM ends a cached one itself, and gem exits 143 for an uncached one
echo 'print(read_file(argv()[1]))' > "$T/p/fd.gem"
for mode in new cached off; do
  if [ $mode = off ]; then export GEM_CACHE=off; fi
  out=$("$GEM" "$T/p/fd.gem" <(printf 'piped'))
  [ "$out" = piped ] || fail "<(cmd) path, $mode: $out"
  echo 'exec("kill -TERM $PPID")' > "$T/p/sig.gem"
  echo 'sleep(5000)' >> "$T/p/sig.gem"
  "$GEM" "$T/p/sig.gem"
  st=$?
  [ $st = 143 ] || fail "killed by SIGTERM, $mode: exit status $st, expected 143"
  unset GEM_CACHE
done

# argv()[0] is the entry's binary, which replaced gem
cat > "$T/p/self.gem" <<'GEM'
print(argv()[0])
GEM
p1=$("$GEM" "$T/p/self.gem")
p2=$("$GEM" "$T/p/self.gem")
case "$p1" in
  "$GEM_CACHE_DIR"/[!.]*/self) ;;
  *) fail "argv()[0] of a new entry: $p1" ;;
esac
[ "$p1" = "$p2" ] && [ -x "$p2" ] || fail "argv()[0] of a cached run: $p2, first $p1"

# runs of a new program at the same time all build and run it
echo 'print("same")' > "$T/p/same.gem"
for i in 1 2 3 4 5 6; do ("$GEM" "$T/p/same.gem" > "$T/same.$i" 2>&1; echo $? >> "$T/same.$i") & done
wait
for i in 1 2 3 4 5 6; do
  [ "$(cat "$T/same.$i")" = "$(printf 'same\n0')" ] || fail "concurrent run $i: $(cat "$T/same.$i")"
done
[ -z "$(ls "$GEM_CACHE_DIR/.tmp" | grep '^gem-')" ] || fail "run directories left after concurrent runs: $(ls "$GEM_CACHE_DIR/.tmp")"

# runs of a cached program while another run empties the cache (moving
# the entries into .tmp): each runs it, from the cache or built again
echo 'print("emptied")' > "$T/p/emptied.gem"
"$GEM" "$T/p/emptied.gem" > /dev/null
for round in 1 2 3 4 5; do
  for i in $(seq 1 256); do mkdir -p "$GEM_CACHE_DIR/fake$i"; done
  echo "print($round)" > "$T/p/new$round.gem"
  for i in 1 2 3 4 5 6; do ("$GEM" "$T/p/emptied.gem" > "$T/emptied.$i" 2>&1; echo $? >> "$T/emptied.$i") & done
  "$GEM" "$T/p/new$round.gem" > /dev/null || fail "emptying run $round failed"
  wait
  for i in 1 2 3 4 5 6; do
    [ "$(cat "$T/emptied.$i")" = "$(printf 'emptied\n0')" ] || fail "run while the cache is emptied, round $round, $i: $(cat "$T/emptied.$i")"
  done
done
ls -d "$GEM_CACHE_DIR"/.tmp/fake1.* > /dev/null 2>&1 || fail "emptying the cache: fake1 not moved into .tmp"

# emptying the cache twice within a minute, the same entry built again in
# between: both of its binaries are kept under .tmp, none removed in place
rm -rf "$GEM_CACHE_DIR"
echo 'print("twice")' > "$T/p/twice.gem"
for round in 1 2; do
  "$GEM" "$T/p/twice.gem" > /dev/null || fail "twice: run $round failed"
  for i in $(seq 1 256); do mkdir -p "$GEM_CACHE_DIR/fake$i"; done
  echo "print($round)" > "$T/p/wipe$round.gem"
  "$GEM" "$T/p/wipe$round.gem" > /dev/null || fail "twice: emptying run $round failed"
done
n=$(ls "$GEM_CACHE_DIR"/.tmp/*/twice 2>/dev/null | wc -l | tr -d ' ')
[ "$n" = 2 ] || fail "emptying the cache twice: $n moved binaries of the entry, expected 2"

if [ $fails -eq 0 ]; then echo "cache: OK"; else exit 1; fi
