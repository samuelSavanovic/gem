#!/usr/bin/env bash
# Verify that intentionally malformed programs produce the expected number of
# errors. Each entry: file_basename:min:max — the compiler must emit at least
# `min` errors and no more than `max` (the upper bound catches cascade noise).
#
# Run from the repo root: tests/check_broken.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi

# basename : min : max
expected=(
  "bad_extern_type:4:4"
  "bad_pattern:2:4"
  "bad_pin:3:3"
  "break_in_do:4:4"
  "break_outside_loop:5:5"
  "brace_block_recovery:2:2"
  "block_let_after_block:11:11"
  "block_let_toplevel:7:7"
  "cascade_braces:2:5"
  "closure_undeclared:7:7"
  "closure_undeclared_assign:3:3"
  "closure_later_let:2:2"
  "closure_body_later:3:3"
  "dotdot_concat:1:1"
  "header_block:3:3"
  "interp_bad_expr:2:2"
  "interp_empty:2:2"
  "interp_unterminated:1:1"
  "load_no_export:1:1"
  "load_parse_error:1:1"
  "load_cycle:1:1"
  "load_missing:4:4"
  "load_missing_nested:1:1"
  "load_self:1:1"
  "load_name_clash:1:1"
  "double_typo:2:4"
  "missing_end:2:8"
  "missing_then_branches:1:4"
  "multi_undeclared:3:5"
  "nested_extern:3:3"
  "nested_fn_toplevel:13:13"
  "stray_do:4:4"
  "top_name_clash:7:7"
  "undeclared:1:1"
  "unterminated_string:1:1"
  "when_no_then:1:1"
  "when_no_then_arms:6:6"
)

fails=0
for entry in "${expected[@]}"; do
  name=${entry%%:*}; rest=${entry#*:}
  min=${rest%%:*}; max=${rest##*:}
  src="tests/broken/${name}.gem"
  if [ ! -f "$src" ]; then
    echo "FAIL: missing corpus file $src"
    fails=$((fails + 1))
    continue
  fi
  count=$("$GEM" --check "$src" 2>&1 | grep -c "^\[Compile Error\]")
  if [ "$count" -lt "$min" ] || [ "$count" -gt "$max" ]; then
    echo "FAIL: $name produced $count errors, expected [$min..$max]"
    fails=$((fails + 1))
  else
    echo "OK:   $name ($count errors, in [$min..$max])"
  fi
done

if [ "$fails" -gt 0 ]; then
  echo
  echo "$fails broken-program checks failed"
  exit 1
fi
echo
echo "all broken-program checks passed"
