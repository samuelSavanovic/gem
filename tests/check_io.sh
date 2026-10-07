#!/usr/bin/env bash
# input() reads whole lines from stdin: any length, NUL bytes kept, a
# trailing "\n" or "\r\n" stripped, a last line without a newline returned,
# nil at EOF. read_file and read_stdin raise for a string over the limit
# (2,147,483,646 bytes). print and eprint write NUL bytes.
#
# Run from the repo root: tests/check_io.sh

set -u
cd "$(dirname "$0")/.."

GEM=${GEM:-build/gem}
if [ ! -x "$GEM" ]; then
  echo "FAIL: $GEM not built — run 'make build' first" >&2
  exit 2
fi

dir=$(mktemp -d)
trap 'rm -rf "$dir"' EXIT

cat > "$dir/lines.gem" <<'GEM'
while true
  let line = input()
  if line == nil then break end
  print(len(line), substr(line, 0, 3) == "b\0c")
end
print("eof")
GEM
"$GEM" "$dir/lines.gem" -o "$dir/lines" 2>"$dir/err" || { echo "FAIL: lines.gem doesn't compile"; cat "$dir/err"; exit 1; }

fails=0
check() {
  local name=$1 want=$2 got=$3
  if [ "$want" != "$got" ]; then
    echo "FAIL: $name"
    diff <(printf '%s\n' "$want") <(printf '%s\n' "$got")
    fails=$((fails + 1))
  fi
}

got=$(
  { head -c 5000 /dev/zero | tr '\0' a; printf '\n'
    printf 'b\0c\r\n'
    printf '\n'
    head -c 100000 /dev/zero | tr '\0' x; printf '\n'
    printf 'last'
  } | "$dir/lines"
)
check "long lines, NUL, CRLF, empty line, no final newline" "5000 false
3 true
0 false
100000 false
4 false
eof" "$got"

check "empty stdin" "eof" "$(printf '' | "$dir/lines")"
check "a bare CR is kept" "2 false
4 false
eof" "$(printf 'a\r\r\nabc\r' | "$dir/lines")"

cat > "$dir/limits.gem" <<'GEM'
for path in [argv()[1], argv()[2]]
  let r = pcall read_file(path)
  print(r.ok and len(r.value) or r.error)
end
print(pcall(fn() read_stdin(3000000000) end).error)
GEM
"$GEM" "$dir/limits.gem" -o "$dir/limits" 2>"$dir/err" || { echo "FAIL: limits.gem doesn't compile"; cat "$dir/err"; exit 1; }
# Sparse files: the limit is checked against the size before reading.
truncate -s 2147483647 "$dir/over.bin"
truncate -s 2200M "$dir/big.bin"
check "read_file and read_stdin over the string limit" "read_file: '$dir/over.bin' is 2147483647 bytes, over the string limit of 2147483646 bytes
read_file: '$dir/big.bin' is 2306867200 bytes, over the string limit of 2147483646 bytes
read_stdin: a string of 3000000000 bytes is over the limit of 2147483646 bytes" "$("$dir/limits" "$dir/over.bin" "$dir/big.bin" </dev/null)"

cat > "$dir/nul.gem" <<'GEM'
let s = "a\0b"
print(s, [s])
print("x{s}y")
eprint("e\0f")
GEM
"$GEM" "$dir/nul.gem" -o "$dir/nul" 2>"$dir/err" || { echo "FAIL: nul.gem doesn't compile"; cat "$dir/err"; exit 1; }
"$dir/nul" > "$dir/nul.out" 2>&1
printf 'a\0b ["a\\0b"]\nxa\0by\ne\0f\n' > "$dir/nul.want"
if ! cmp -s "$dir/nul.want" "$dir/nul.out"; then
  echo "FAIL: print and eprint write NUL bytes"
  od -c "$dir/nul.out"
  fails=$((fails + 1))
fi

if [ "$fails" -gt 0 ]; then
  exit 1
fi
echo "io: ok"
