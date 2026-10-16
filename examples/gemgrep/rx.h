/* The C side of regex.gem: a POSIX regex_t behind a Ptr handle. Every function is a plain
 * `extern fn`, runs on the scheduler thread and keeps no pointer to Gem memory. A handle belongs
 * to the process that made it: another process given the number would hold a pointer the owner frees. */

#include <regex.h>
#include <stdint.h>
#include <stdlib.h>

/* the whole match and the first two groups: -w wraps the pattern so group 2 is the user's */
#define RX_GROUPS 3

typedef struct {
    regex_t re;
    int compiled;
    regmatch_t m[RX_GROUPS];
} rx_handle;

static void *rx_new(void) {
    return calloc(1, sizeof(rx_handle));
}

/* macOS's regcomp reads `\<`, `\>`, `\b`, `\w`, `\s` and backreferences as glibc does only under
 * REG_ENHANCED; without it each matches the escaped character (`\w` matches `w`). */
#ifdef REG_ENHANCED
#define RX_DIALECT REG_ENHANCED
#else
#define RX_DIALECT 0
#endif

/* 0, or the regcomp error code for rx_error. REG_NEWLINE: `^` and `$` match at each newline in a
 * range, and `.` and `[^a]` never match one. */
static int64_t rx_compile(void *h, const char *pattern, int icase) {
    rx_handle *r = h;
    int flags = REG_EXTENDED | REG_NEWLINE | RX_DIALECT | (icase ? REG_ICASE : 0);
    int rc = regcomp(&r->re, pattern, flags);
    r->compiled = rc == 0;
    return rc;
}

/* glibc's text (GNU grep's) for the POSIX codes whatever the libc, else regerror's. Static memory,
 * since a plain extern fn's String return is copied, never freed. Call it before rx_free. */
static const char *rx_error(void *h, int64_t code) {
    static char msg[256];
    switch (code) {
    case REG_BADPAT: return "Invalid regular expression";
    case REG_ECOLLATE: return "Invalid collation character";
    case REG_ECTYPE: return "Invalid character class name";
    case REG_EESCAPE: return "Trailing backslash";
    case REG_ESUBREG: return "Invalid back reference";
    case REG_EBRACK: return "Unmatched [, [^, [:, [., or [=";
    case REG_EPAREN: return "Unmatched ( or \\(";
    case REG_EBRACE: return "Unmatched \\{";
    case REG_BADBR: return "Invalid content of \\{\\}";
    case REG_ERANGE: return "Invalid range end";
    case REG_ESPACE: return "Memory exhausted";
    case REG_BADRPT: return "Invalid preceding regular expression";
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

/* Whether the regex matches in data[start, end). REG_STARTEND reads the range by length, so NULs
 * are ordinary bytes and nothing needs a terminator. `notbol`: `start` isn't a line start, so `^`
 * can't match there. With `offsets` the match and groups land in the handle; without, regexec skips
 * the submatch work. The caller checks 0 <= start <= end <= len. */
static int rx_exec(void *h, const uint8_t *data, int64_t len, int64_t start, int64_t end, int notbol, int offsets) {
    rx_handle *r = h;
    (void)len;
    r->m[0].rm_so = (regoff_t)start;
    r->m[0].rm_eo = (regoff_t)end;
    int eflags = REG_STARTEND | (notbol ? REG_NOTBOL : 0);
    return regexec(&r->re, (const char *)data, offsets ? RX_GROUPS : 0, r->m, eflags) == 0;
}

/* offsets into the data of the last rx_exec, -1 for a group that didn't take part */
static int64_t rx_group_start(void *h, int64_t group) {
    return ((rx_handle *)h)->m[group].rm_so;
}

static int64_t rx_group_end(void *h, int64_t group) {
    return ((rx_handle *)h)->m[group].rm_eo;
}
