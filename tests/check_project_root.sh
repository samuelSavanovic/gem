#!/usr/bin/env bash
# Project-root discovery and shown paths. Builds a small tree in a temp dir
#   p/gem.toml  p/lib/util.gem  p/app/main.gem (load "lib/util")
#   q/main.gem (load "./mods/m", no gem.toml)  q/mods/m.gem
# and compiles the entry from several working directories and spellings.
# Each run must find the gem.toml (or none), and the stack-trace frames and
# `#line` paths must be project-relative (or, without a gem.toml, relative
# to the entry's directory as typed).
#
# Run from the repo root: tests/check_project_root.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi
case $GEM in /*) ;; *) GEM="$(pwd)/$GEM" ;; esac

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
mkdir -p "$tmp/p/lib" "$tmp/p/app" "$tmp/q/mods"
: > "$tmp/p/gem.toml"
printf 'fn hello()\n  print("hello from util")\nend\nfn boom()\n  error("boom")\nend\nexport hello, boom\n' > "$tmp/p/lib/util.gem"
printf 'load "lib/util"\nutil.hello()\nutil.boom()\n' > "$tmp/p/app/main.gem"
printf 'load "lib/util"\nlet x = 1 +\n' > "$tmp/p/app/bad.gem"
printf 'load "./mods/m"\nm.f()\n' > "$tmp/q/main.gem"
printf 'fn f()\n  error("in m")\nend\nexport f\n' > "$tmp/q/mods/m.gem"

fails=0
total=0

# check <cwd> <entry> <expected frames and #line paths, one per line>
check() {
  local dir=$1 entry=$2 want=$3 got
  total=$((total + 1))
  got=$(cd "$tmp/$dir" && {
    "$GEM" "$entry" 2>&1 | grep -E '^(hello|  at |  --> )' | grep -v -- '-->.*:[0-9]*$'
    "$GEM" "$entry" 2>&1 | grep -E '^  --> .*:[0-9]+:[0-9]+$'
    "$GEM" "$entry" --emit-c 2>/dev/null | grep '^#line' | sed 's/^#line [0-9]* //' | sort -u
  })
  if [ "$got" != "$want" ]; then
    echo "FAIL: (cd $dir && gem $entry)"
    diff <(echo "$want") <(echo "$got")
    fails=$((fails + 1))
  fi
}

P='hello from util
  at util.boom (lib/util.gem:5)
  at main (app/main.gem:3)
"app/main.gem"
"lib/util.gem"'
check p app/main.gem "$P"
check p/app main.gem "$P"
check p/app ./main.gem "$P"
check p/lib ../app/main.gem "$P"
check . "$tmp/p/app/main.gem" "$P"

B='  --> app/bad.gem:2:12'
check p app/bad.gem "$B"
check p/app bad.gem "$B"

check q main.gem '  at m.f (mods/m.gem:2)
  at main (main.gem:2)
"main.gem"
"mods/m.gem"'
check . q/main.gem '  at m.f (q/mods/m.gem:2)
  at main (q/main.gem:2)
"q/main.gem"
"q/mods/m.gem"'
check q/mods ../main.gem '  at m.f (../mods/m.gem:2)
  at main (../main.gem:2)
"../main.gem"
"../mods/m.gem"'

if [ "$fails" -gt 0 ]; then
  echo
  echo "$fails / $total project-root checks failed"
  exit 1
fi
echo "all $total project-root checks passed"
