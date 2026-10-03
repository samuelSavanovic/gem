/*
 * gem_builtins_time.c — Time formatting builtins: epoch_ms, format_time, format_time_local.
 */

#include "gem.h"
#include <time.h>
#include <sys/time.h>

GemVal gem_epoch_ms_fn(void *_env, GemVal *args, int argc) {
    (void)_env; (void)args; (void)argc;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return gem_int((int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/* Formats epoch milliseconds `ms` with strftime. The seconds are rounded
 * down, so -1 ms is 23:59:59 of the day before the epoch, not the epoch. */
static GemVal format_tm(int64_t ms, const char *fmt, int local, const char *who) {
    int64_t secs64 = ms / 1000;
    if (ms % 1000 < 0) secs64 -= 1;
    time_t secs = (time_t)secs64;
    struct tm tm_buf;
    if ((local ? localtime_r(&secs, &tm_buf) : gmtime_r(&secs, &tm_buf)) == NULL) {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s: time %lld ms is out of range", who, (long long)ms);
        gem_error(buf);
    }
    if (fmt[0] == '\0') return gem_string("");
    /* strftime returns 0 both for output that doesn't fit and for empty
     * output (such as "%p" in some locales), so grow the buffer a few
     * times before settling on "". */
    char small[256];
    size_t n = strftime(small, sizeof(small), fmt, &tm_buf);
    if (n > 0) return gem_string_with_len(small, (int)n);
    size_t cap = sizeof(small);
    for (int tries = 0; tries < 5; tries++) {
        cap *= 4;
        char *out = (char *)malloc(cap);
        if (!out) break;
        n = strftime(out, cap, fmt, &tm_buf);
        if (n > 0) {
            GemVal r = gem_string_with_len(out, (int)n);
            free(out);
            return r;
        }
        free(out);
    }
    return gem_string("");
}

GemVal gem_format_time_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_INT || args[1].type != VAL_STRING) {
        char buf[128];
        snprintf(buf, sizeof(buf), "format_time: expected (int, string), got (%s, %s)",
                 argc < 1 ? "nothing" : gem_type_str(args[0]),
                 argc < 2 ? "nothing" : gem_type_str(args[1]));
        gem_error(buf);
    }
    return format_tm(args[0].ival, args[1].sval, 0, "format_time");
}

GemVal gem_format_time_local_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_INT || args[1].type != VAL_STRING) {
        char buf[128];
        snprintf(buf, sizeof(buf), "format_time_local: expected (int, string), got (%s, %s)",
                 argc < 1 ? "nothing" : gem_type_str(args[0]),
                 argc < 2 ? "nothing" : gem_type_str(args[1]));
        gem_error(buf);
    }
    return format_tm(args[0].ival, args[1].sval, 1, "format_time_local");
}
