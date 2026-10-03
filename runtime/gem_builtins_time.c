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

/* For UTC: `fmt` with %z, %Z and %s replaced by "+0000", "UTC" and the
 * epoch seconds, which strftime would take from the local zone (macOS for
 * all three; glibc for %s, through mktime). "%%" is kept as it is. A
 * trailing \x01 is appended in both cases (see format_tm). Caller frees. */
static char *prepare_fmt(const char *fmt, int64_t secs, int utc) {
    size_t len = strlen(fmt);
    char *out = (char *)malloc(len * 21 + 2);
    if (!out) return NULL;
    size_t o = 0;
    for (size_t i = 0; i < len; i++) {
        if (utc && fmt[i] == '%' && i + 1 < len) {
            char c = fmt[i + 1];
            if (c == 'z') { memcpy(out + o, "+0000", 5); o += 5; i++; continue; }
            if (c == 'Z') { memcpy(out + o, "UTC", 3); o += 3; i++; continue; }
            if (c == 's') { o += (size_t)sprintf(out + o, "%lld", (long long)secs); i++; continue; }
            out[o++] = fmt[i];
            out[o++] = fmt[++i];
            continue;
        }
        out[o++] = fmt[i];
    }
    out[o++] = '\x01';
    out[o] = '\0';
    return out;
}

#define GEM_FORMAT_TIME_MAX (256 * 1024)

/* Formats epoch milliseconds `ms` with strftime. The seconds are rounded
 * down, so -1 ms is 23:59:59 of the day before the epoch, not the epoch. */
static GemVal format_tm(int64_t ms, const char *fmt, int local, const char *who) {
    int64_t secs64 = ms / 1000;
    if (ms % 1000 < 0) secs64 -= 1;
    time_t secs = (time_t)secs64;
    struct tm tm_buf;
    char err[128];
    if ((local ? localtime_r(&secs, &tm_buf) : gmtime_r(&secs, &tm_buf)) == NULL) {
        snprintf(err, sizeof(err), "%s: time %lld ms is out of range", who, (long long)ms);
        gem_error(err);
    }
    if (fmt[0] == '\0') return gem_string("");
    /* strftime returns 0 both for output that doesn't fit and for empty
     * output (such as "%p" in some locales), so the format gets a trailing
     * \x01: a result is never empty, and 0 always means "too small". */
    char *f = prepare_fmt(fmt, secs64, !local);
    if (!f) gem_error("out of memory");
    /* The last try has room for GEM_FORMAT_TIME_MAX bytes, the \x01 and
     * the terminator. */
    const size_t max_cap = GEM_FORMAT_TIME_MAX + 2;
    for (size_t cap = 256;; cap *= 4) {
        if (cap > max_cap) cap = max_cap;
        char *out = (char *)malloc(cap);
        if (!out) break;
        size_t n = strftime(out, cap, f, &tm_buf);
        if (n > 0) {
            GemVal r = gem_string_with_len(out, (int)(n - 1));
            free(out);
            free(f);
            return r;
        }
        free(out);
        if (cap == max_cap) break;
    }
    free(f);
    snprintf(err, sizeof(err), "%s: output is over 256 KB", who);
    gem_error(err);
    return GEM_NIL;
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
