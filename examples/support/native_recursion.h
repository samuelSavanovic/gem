/* For 108 and 111: recurses `n` levels with a 1 KB frame each, past the process stack into the guard
 * page, which the runtime turns into a "stack overflow in native code" failure. */

#include <stdint.h>

static int64_t gem_example_native_recurse(int64_t n) {
    volatile char pad[1024];
    pad[0] = (char)(n & 1);
    pad[sizeof pad - 1] = pad[0];
    if (n <= 0) return 0;
    /* not a tail call: the addition keeps every frame */
    return gem_example_native_recurse(n - 1) + pad[sizeof pad - 1];
}
