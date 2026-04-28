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

GemVal gem_format_time_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_INT || args[1].type != VAL_STRING) {
        char buf[128];
        snprintf(buf, sizeof(buf), "format_time: expected (int, string), got (%s, %s)",
                 argc < 1 ? "nothing" : gem_type_str(args[0]),
                 argc < 2 ? "nothing" : gem_type_str(args[1]));
        gem_error(buf);
    }
    time_t secs = (time_t)(args[0].ival / 1000);
    struct tm tm_buf;
    gmtime_r(&secs, &tm_buf);
    char out[256];
    size_t n = strftime(out, sizeof(out), args[1].sval, &tm_buf);
    if (n == 0) return gem_string("");
    char *result = (char *)GC_MALLOC_ATOMIC(n + 1);
    memcpy(result, out, n + 1);
    return gem_string(result);
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
    time_t secs = (time_t)(args[0].ival / 1000);
    struct tm tm_buf;
    localtime_r(&secs, &tm_buf);
    char out[256];
    size_t n = strftime(out, sizeof(out), args[1].sval, &tm_buf);
    if (n == 0) return gem_string("");
    char *result = (char *)GC_MALLOC_ATOMIC(n + 1);
    memcpy(result, out, n + 1);
    return gem_string(result);
}
