#!/bin/sh
# make fuzz-parser: builds tests/fuzz/parser_fuzz.gem against libFuzzer and runs it on a corpus seeded
# from the examples and tests/broken.
#
#   tests/fuzz/run_libfuzzer.sh [libFuzzer flags...]   (default -max_total_time=60)
#
# Needs a clang with libclang_rt.fuzzer_no_main (Linux: libclang-rt-<N>-dev; Apple's clang lacks it,
# use Homebrew's: CLANG=$(brew --prefix llvm)/bin/clang).
# Corpus in build/fuzz_corpus/, crashes in build/fuzz_failures/.
set -eu
cd "$(dirname "$0")/../.."

CLANG=${CLANG:-clang}
dir=$("$CLANG" --print-runtime-dir 2>/dev/null || true)
lib=""
arch=$(uname -m)
for f in "$dir/libclang_rt.fuzzer_no_main-$arch.a" "$dir/libclang_rt.fuzzer_no_main.a" \
         "$dir/libclang_rt.fuzzer_no_main_osx.a"; do
  [ -f "$f" ] && lib=$f && break
done
if [ -z "$lib" ]; then
  echo "fuzz-parser: no libclang_rt.fuzzer_no_main in '$dir' ($CLANG): install libFuzzer (see this script's header)" >&2
  exit 2
fi
case $(uname) in
  Darwin) cxx=-lc++ ;;
  *) cxx=-lstdc++ ;;
esac

mkdir -p build/fuzz_corpus build/fuzz_failures
CC=$CLANG CFLAGS="-fsanitize=fuzzer-no-link -O1" LDFLAGS="$lib $cxx" \
  build/gem tests/fuzz/parser_fuzz.gem -o build/parser_fuzz
if [ -z "$(ls build/fuzz_corpus)" ]; then
  cp examples/[0-9]*.gem tests/broken/*.gem build/fuzz_corpus/
fi
if [ $# -eq 0 ]; then
  set -- -max_total_time=60
fi
exec build/parser_fuzz -max_len=8192 -artifact_prefix=build/fuzz_failures/ "$@" build/fuzz_corpus
