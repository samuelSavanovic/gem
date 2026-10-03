/* Test helper for example 170: a header found next to the including .gem
 * file (examples/170_extern_include_relative.gem includes it as
 * "support/ei_helpers.h"). examples/modules/ei_helpers.h has the same name
 * and a different function; each file's include finds its own. */

#include <stdint.h>

static int64_t ei_scale(int64_t x) { return x * 10; }
