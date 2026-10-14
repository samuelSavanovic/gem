/* libFuzzer entry for tests/fuzz/parser_fuzz.gem (make fuzz-parser).
 *
 * The Gem program's main process calls gem_fuzz_main(), which hands the
 * process's own argv to libFuzzer's LLVMFuzzerRunDriver; libFuzzer then
 * calls back with each input, which goes to the Gem fn fuzz_one as a
 * string. Everything runs on the main process's coroutine stack, as any
 * extern fn does. Link with libclang_rt.fuzzer_no_main (the Gem program
 * has its own main). */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "gem.h"

int LLVMFuzzerRunDriver(int *argc, char ***argv, int (*cb)(const uint8_t *data, size_t size));

/* The C name codegen gives the entry file's `fn fuzz_one`. */
static GemVal gem_fn_fuzz_one(void *env, GemVal *args, int argc);


static int gem_fuzz_input(const uint8_t *data, size_t size) {
    GemVal src = gem_string_with_len((const char *)data, (int64_t)size);
    gem_fn_fuzz_one(NULL, &src, 1);
    return 0;
}

/* An internal error: abort, so libFuzzer saves the input as a crash. */
static int64_t gem_fuzz_abort(void) {
    abort();
}

static int64_t gem_fuzz_main(void) {
    /* fuzz_one's one-iteration loop resets at its back-edge whatever the
       input allocated (reset stress mode's threshold); else memory would
       grow with every input, as nothing else ever frees it. */
    gem_reset_threshold = 1;
    int argc = gem_stored_argc;
    char **argv = gem_stored_argv;
    return LLVMFuzzerRunDriver(&argc, &argv, gem_fuzz_input);
}
