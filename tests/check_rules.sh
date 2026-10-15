#!/usr/bin/env bash
# Lint the rules CLAUDE.md lists under "Key Decisions" that a grep can
# check, and the builtin list against the docs and the editor grammars.
#
# Runtime rules (each match outside its allowlist fails):
#   - GemProcess.state changes only in gem_proc_set_state;
#   - gem_cur_globals[ only in gem.h (and in no compiler output);
#   - close( (a space before the paren too) in the socket code only on an
#     fd no resource entry owns yet, or through gem_tcp_close_fd_checked
#     (gem_res_close is the way);
#   - mco_create only in gem_coro_create.
# An allowlist entry is `file|line text|count`: the trimmed source line and
# how many times it may appear in that file, so a new copy of an allowed
# line fails too. C comments (block, whole-line and trailing //) are
# dropped before matching.
#
# Builtins: every user-visible name in BUILTIN_FNS (compiler/builtins.gem)
# is documented in SPEC.md (as `name(` or `name`) and CHEATSHEET.md (as a
# call `name(` or `name do`, as `name`, or as an entry of one of its
# `# Category: a, b, c` builtin lists), and
# the VS Code and tree-sitter builtin lists hold exactly those names;
# LEAF_BUILTINS and DIRECT_BUILTINS name only builtins.
#
# Run from anywhere: tests/check_rules.sh

set -u
cd "$(dirname "$0")/.."

fails=0
fail() { echo "FAIL: $*"; fails=$((fails + 1)); }

# strip_comments <file>: the file with C comments (and Gem # comments in a
# .gem file) blanked, line count unchanged. A // inside a "string" stays.
strip_comments() {
  case $1 in
    *.gem) sed -e 's/^[[:space:]]*#.*//' "$1" ;;
    *) perl -0pe 's{/\*.*?\*/}{ (my $c = $&) =~ s/[^\n]//g; $c }gse;
                  s{^((?:[^"\n/]|/(?!/)|"(?:[^"\\\n]|\\.)*")*)//[^\n]*}{$1}gm' "$1" ;;
  esac
}

# check_rule <name> <ERE> <allowlist> <files...>
check_rule() {
  local name=$1 re=$2 allow=$3
  shift 3
  local f line count want
  for f in "$@"; do
    # file|line text, comments dropped, text trimmed
    strip_comments "$f" | grep -E "$re" | while IFS= read -r line; do
      line=$(printf '%s' "$line" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')
      [ -n "$line" ] && printf '%s|%s\n' "$f" "$line"
    done
  done | sort | uniq -c | while read -r count key; do
    want=$(printf '%s\n' "$allow" | awk -F'|' -v k="$key" '$1 "|" $2 == k { print $3 }')
    if [ -z "$want" ]; then
      echo "FAIL: $name: $key"
    elif [ "$count" -gt "$want" ]; then
      echo "FAIL: $name: $key appears $count times, allowed $want"
    fi
  done > "$TMP_OUT"
  if [ -s "$TMP_OUT" ]; then
    cat "$TMP_OUT"
    fails=$((fails + $(wc -l < "$TMP_OUT")))
  fi
}

TMP_OUT=$(mktemp "${TMPDIR:-/tmp}/gem_rules.XXXXXX")
trap 'rm -f "$TMP_OUT"' EXIT

RUNTIME_C=$(ls runtime/gem_*.c runtime/gem.h)

check_rule "GemProcess.state is set only through gem_proc_set_state" \
  '(->|\.)state[[:space:]]*=[^=]' \
  'runtime/gem_scheduler.c|p->state = st;|1' \
  $RUNTIME_C

check_rule "gem_cur_globals[ is used only in gem.h" \
  'gem_cur_globals\[' \
  '' \
  $(ls runtime/gem_*.c) compiler/*.gem

check_rule "a socket fd is closed only through the resource table" \
  '(^|[^_[:alnum:]>.])close[[:space:]]*\(' \
  'runtime/gem_builtins_tcp.c|close(fd);|3' \
  runtime/gem_builtins_tcp.c runtime/gem_builtins_tls.c runtime/gem_resource.c runtime/gem_scheduler.c

check_rule "mco_create is called only by gem_coro_create" \
  'mco_create\(' \
  'runtime/gem_scheduler.c|mco_result res = mco_create(out, &desc);|1' \
  $RUNTIME_C

# ── Builtins ──
builtin_block() {   # names of the table literal `let <name> = {` in builtins.gem
  awk -v hdr="let $1 = {" '$0 == hdr { f = 1; next } f && /^}/ { exit }
    f { gsub(/"/, ""); n = split($0, parts, ","); for (i = 1; i <= n; i++) {
          p = parts[i]; sub(/^[[:space:]]*/, "", p); sub(/:.*/, "", p); sub(/\/.*/, "", p)
          if (p != "") print p } }' compiler/builtins.gem
}
all=$(builtin_block BUILTIN_FNS | sort -u)
names=$(printf '%s\n' "$all" | grep -v '^__')
if [ "$(printf '%s\n' "$names" | wc -l)" -lt 50 ]; then
  fail "couldn't read BUILTIN_FNS from compiler/builtins.gem"
fi
for n in $names; do
  grep -qE "\`$n[(\`]" docs/SPEC.md || fail "builtin $n is not in docs/SPEC.md"
  grep -qE "(^|[^_[:alnum:].])$n(\(| do( |\$))|\`$n\`|^# [A-Za-z/ &]+:(.*,)?[[:space:]]+$n[[:space:]]*([,(]|\$)" docs/CHEATSHEET.md \
    || fail "builtin $n is not in docs/CHEATSHEET.md"
done
for table in LEAF_BUILTINS DIRECT_BUILTINS; do
  for n in $(builtin_block $table | sort -u); do
    printf '%s\n' "$all" | grep -qx "$n" || fail "$table names $n, which is not in BUILTIN_FNS"
  done
done
vscode=$(grep -o '"match": "\\\\b(print|[^)]*)' editors/vscode/syntaxes/gem.tmLanguage.json | sed -e 's/.*(//' -e 's/)$//' | tr '|' '\n' | sort -u)
tsitter=$(grep -m1 -o '"\^(print|[^)]*)' editors/tree-sitter-gem/queries/highlights.scm | sed -e 's/.*(//' -e 's/)$//' | tr '|' '\n' | sort -u)
for g in vscode tsitter; do
  case $g in
    vscode) list=$vscode; file=editors/vscode/syntaxes/gem.tmLanguage.json ;;
    tsitter) list=$tsitter; file=editors/tree-sitter-gem/queries/highlights.scm ;;
  esac
  d=$(diff <(printf '%s\n' "$names") <(printf '%s\n' "$list"))
  if [ -n "$d" ]; then
    fail "the builtin list in $file differs from BUILTIN_FNS (< missing, > extra):"
    printf '%s\n' "$d" | grep '^[<>]'
  fi
done
# the block-call list of highlights.scm names only builtins
for n in $(grep -o '"\^(spawn|[^)]*)' editors/tree-sitter-gem/queries/highlights.scm | sed -e 's/.*(//' -e 's/)$//' | tr '|' '\n'); do
  printf '%s\n' "$names" | grep -qx "$n" || fail "highlights.scm block-call list names $n, which is not a builtin"
done

if [ "$fails" -eq 0 ]; then
  echo "rules: OK"
else
  echo "rules: $fails failure(s)"
  exit 1
fi
