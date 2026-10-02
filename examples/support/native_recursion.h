/* Test helper for examples 108 and 111 (not part of the language or the
 * runtime): a C function that recurses `n` levels deep with a 1 KB frame per
 * level. Called through `extern fn`, it drives native code past the end of a
 * process's stack, into the guard page, which the Gem runtime turns into a
 * "stack overflow in native code" process failure.
 *
 * The examples reach this file as `extern include
 * "../examples/support/native_recursion.h"`: the compiler passes
 * `-I <install root>/runtime`, and the path is resolved against that. */

#include <stdint.h>

static int64_t gem_example_native_recurse(int64_t n) {
    volatile char pad[1024];
    pad[0] = (char)(n & 1);
    pad[sizeof pad - 1] = pad[0];
    if (n <= 0) return 0;
    /* Not a tail call: the addition keeps every frame on the stack. */
    return gem_example_native_recurse(n - 1) + pad[sizeof pad - 1];
}
