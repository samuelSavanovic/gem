/* libFuzzer entry for tests/fuzz/parser_fuzz.gem (make fuzz-parser).
 *
 * The Gem program's main process calls gem_fuzz_main({run: fuzz_one}),
 * which hands the process's own argv to libFuzzer's LLVMFuzzerRunDriver;
 * libFuzzer then calls back with each input, which goes to that fn as a
 * string. Everything runs on the main process's coroutine stack, as any
 * extern fn does. Link with libclang_rt.fuzzer_no_main (the Gem program
 * has its own main). */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "gem.h"

int LLVMFuzzerRunDriver(int *argc, char ***argv, int (*cb)(const uint8_t *data, size_t size));

/* The fn each input goes to: a top-level Gem fn, so its env is NULL and
   nothing in it lives in the arena. */
static GemVal gem_fuzz_target;

static int gem_fuzz_input(const uint8_t *data, size_t size) {
    GemVal src = gem_string_with_len((const char *)data, (int64_t)size);
    gem_fuzz_target.fn(gem_fuzz_target.env, &src, 1);
    return 0;
}

/* An internal error: abort, so libFuzzer saves the input as a crash. */
static int64_t gem_fuzz_abort(void) {
    abort();
}

static int64_t gem_fuzz_main(GemVal target) {
    gem_fuzz_target = gem_table_get(target, GEM_STR_LIT("run", 3));
    if (gem_fuzz_target.type != VAL_FN) gem_error("gem_fuzz_main: target.run must be a fn");
    /* fuzz_one's one-iteration loop resets at its back-edge whatever the
       input allocated (reset stress mode's threshold); else memory would
       grow with every input, as nothing else ever frees it. */
    gem_reset_threshold = 1;
    int argc = gem_stored_argc;
    char **argv = gem_stored_argv;
    return LLVMFuzzerRunDriver(&argc, &argv, gem_fuzz_input);
}
