/* rx.h — the C side of regex.gem: a POSIX regex_t behind a Ptr handle.
 *
 * Every function here is a plain `extern fn`: it runs on the scheduler
 * thread and keeps no pointer to Gem memory after it returns. A handle
 * belongs to the process that made it: another process given the number
 * would hold a pointer that the owner frees. */

#include <regex.h>
#include <stdint.h>
#include <stdlib.h>

/* The whole match, and the first two groups: -w wraps the pattern so
 * that group 2 is the user's pattern (regex.gem). */
#define RX_GROUPS 3

typedef struct {
    regex_t re;
    int compiled;
    regmatch_t m[RX_GROUPS];
} rx_handle;

/* NULL when out of memory. */
static void *rx_new(void) {
    return calloc(1, sizeof(rx_handle));
}

/* macOS's regcomp reads `\<`, `\>`, `\b`, `\w`, `\s` and backreferences
 * as glibc's does only under REG_ENHANCED; without it each matches the
 * escaped character itself (`\w` matches `w`). glibc has no such flag. */
#ifdef REG_ENHANCED
#define RX_DIALECT REG_ENHANCED
#else
#define RX_DIALECT 0
#endif

/* 0, or the regcomp error code for rx_error. REG_NEWLINE: a range that
 * spans lines matches `^` and `$` at each newline, and `.` and `[^a]`
 * never match one. */
static int64_t rx_compile(void *h, const char *pattern, int icase) {
    rx_handle *r = h;
    int flags = REG_EXTENDED | REG_NEWLINE | RX_DIALECT | (icase ? REG_ICASE : 0);
    int rc = regcomp(&r->re, pattern, flags);
    r->compiled = rc == 0;
    return rc;
}

/* The message for regcomp's error `code`: glibc's text, which is GNU
 * grep's, for the POSIX codes whatever the libc, else regerror's. Static
 * memory: a plain extern fn's String return is copied, never freed.
 * `char *`, not `const char *`, is what the generated wrapper expects.
 * Call it before rx_free. */
static char *rx_error(void *h, int64_t code) {
    static char msg[256];
    switch (code) {
    case REG_BADPAT: return (char *)"Invalid regular expression";
    case REG_ECOLLATE: return (char *)"Invalid collation character";
    case REG_ECTYPE: return (char *)"Invalid character class name";
    case REG_EESCAPE: return (char *)"Trailing backslash";
    case REG_ESUBREG: return (char *)"Invalid back reference";
    case REG_EBRACK: return (char *)"Unmatched [, [^, [:, [., or [=";
    case REG_EPAREN: return (char *)"Unmatched ( or \\(";
    case REG_EBRACE: return (char *)"Unmatched \\{";
    case REG_BADBR: return (char *)"Invalid content of \\{\\}";
    case REG_ERANGE: return (char *)"Invalid range end";
    case REG_ESPACE: return (char *)"Memory exhausted";
    case REG_BADRPT: return (char *)"Invalid preceding regular expression";
    }
    regerror((int)code, &((rx_handle *)h)->re, msg, sizeof msg);
    return msg;
}

static void rx_free(void *h) {
    rx_handle *r = h;
    if (r == NULL) {
        return;
    }
    if (r->compiled) {
        regfree(&r->re);
    }
    free(r);
}

/* Whether the regex matches in data[start, end). REG_STARTEND reads that
 * range by its length, so NULs in it are bytes like any other and nothing
 * needs a terminator. `notbol`: `start` is not the start of a line, so `^`
 * can't match there. With `offsets` the match and its groups land in the
 * handle (rx_group_start/end); without, regexec skips the submatch work.
 * The caller checks 0 <= start <= end <= len. */
static int rx_exec(void *h, const uint8_t *data, int64_t len, int64_t start, int64_t end, int notbol, int offsets) {
    rx_handle *r = h;
    (void)len;
    r->m[0].rm_so = (regoff_t)start;
    r->m[0].rm_eo = (regoff_t)end;
    int eflags = REG_STARTEND | (notbol ? REG_NOTBOL : 0);
    return regexec(&r->re, (const char *)data, offsets ? RX_GROUPS : 0, r->m, eflags) == 0;
}

/* Offsets into the data of the last rx_exec, -1 for a group that didn't
 * take part in the match. */
static int64_t rx_group_start(void *h, int64_t group) {
    return ((rx_handle *)h)->m[group].rm_so;
}

static int64_t rx_group_end(void *h, int64_t group) {
    return ((rx_handle *)h)->m[group].rm_eo;
}
