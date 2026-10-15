#!/usr/bin/env bash
# make install PREFIX=<dir> (twice, over the first install), then the
# installed gem from elsewhere: a program loading std, gem new, gem test,
# gem build, project names they refuse, and the errors of gem test /
# gem build outside a project.
#
# Run from the repo root: tests/check_install.sh

set -u
cd "$(dirname "$0")/.."
T=$(mktemp -d "${TMPDIR:-/tmp}/gem_install.XXXXXX")
trap 'rm -rf "$T"' EXIT
export GEM_CACHE_DIR="$T/cache"
fails=0
fail() { echo "FAIL: $*"; fails=$((fails + 1)); }

make -s install PREFIX="$T/prefix" > "$T/install.log" 2>&1 || { cat "$T/install.log"; echo "FAIL: make install"; exit 1; }
make -s install PREFIX="$T/prefix" > "$T/install.log" 2>&1 || { cat "$T/install.log"; echo "FAIL: make install over an install"; exit 1; }
G="$T/prefix/bin/gem"
[ -x "$G" ] || { echo "FAIL: no $G"; exit 1; }

mkdir -p "$T/work"
cd "$T/work"
printf 'load "std/string"\nprint(string.upper("ok"))\n' > up.gem
out=$("$G" up.gem 2>&1)
[ "$out" = "OK" ] || fail "installed gem running a program: $out"

out=$("$G" new demo 2>&1) || fail "gem new: $out"
for f in gem.toml main.gem test.gem .gitignore; do
  [ -f demo/$f ] || fail "gem new: no demo/$f"
done
"$G" new demo > /dev/null 2>&1 && fail "gem new over an existing directory succeeded"
"$G" new > /dev/null 2>&1; [ $? = 2 ] || fail "gem new without a name: not a usage error"

cd demo
out=$("$G" main.gem 2>&1)
[ "$out" = "hello, world" ] || fail "demo main: $out"
out=$("$G" test 2>&1)
case $out in *"1 passed / 0 failed"*) ;; *) fail "gem test: $out" ;; esac
echo 'test.case("fails") do test.assert_eq(1, 2) end' > extra.gem
cat test.gem > t2 && sed 's/^test.run()$//' t2 > test.gem && cat extra.gem >> test.gem && echo 'test.run()' >> test.gem
"$G" test > /dev/null 2>&1 && fail "gem test with a failing case exited 0"
out=$("$G" build 2>&1) || fail "gem build: $out"
[ -x build/demo ] || fail "gem build: no build/demo"
out=$(./build/demo)
[ "$out" = "hello, world" ] || fail "built demo: $out"
mkdir -p sub && cd sub
out=$("$G" build 2>&1) || fail "gem build from a subdirectory: $out"
cd ../..

# a gem.toml naming another entry and binary
mkdir -p other && cd other
printf '# a comment\nname = "tool"  # the binary\nmain = src/app.gem # unquoted\n[extra]\n' > gem.toml
mkdir -p src && echo 'print("app")' > src/app.gem
"$G" build > /dev/null 2>&1 || fail "gem build with main = src/app.gem"
[ "$(./build/tool 2>&1)" = app ] || fail "build/tool"
cd ..

# gem test hands every argument to test.gem, options included
mkdir -p targs && cd targs
echo 'name = "targs"' > gem.toml
echo 'print(argv()[1], argv()[2])' > test.gem
out=$("$G" test --check -o 2>&1)
[ "$out" = "--check -o" ] || fail "gem test --check -o: $out"
cd ..

# a name that can't be a file under build/
for bad in '../evil' 'a/b' '.hidden' '-x'; do
  mkdir -p badname && printf 'name = "%s"\n' "$bad" > badname/gem.toml && echo 'print(1)' > badname/main.gem
  out=$(cd badname && "$G" build 2>&1); st=$?
  [ $st = 1 ] && case $out in *"can't name"*) true ;; *) false ;; esac || fail "gem build with name = $bad: $st $out"
done
[ -e evil ] && fail "gem build wrote ../evil"
for bad in '.hidden' 'sub/.x' 'a..b'; do
  out=$("$G" new "$bad" 2>&1); st=$?
  [ $st = 1 ] && [ ! -e "$bad" ] || fail "gem new $bad: $st $out"
done
out=$("$G" new trailing/ 2>&1) && grep -q 'name = "trailing"' trailing/gem.toml || fail "gem new trailing/: $out"

mkdir -p empty && cd empty
out=$("$G" test 2>&1); st=$?
[ $st = 1 ] && case $out in *"no gem.toml"*) true ;; *) false ;; esac || fail "gem test outside a project: $st $out"
out=$("$G" build 2>&1); st=$?
[ $st = 1 ] && case $out in *"no gem.toml"*) true ;; *) false ;; esac || fail "gem build outside a project: $st $out"

if [ $fails -eq 0 ]; then echo "install: OK"; else exit 1; fi
