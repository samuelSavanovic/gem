/*
 * gem_builtins_text.c — the C side of std/regex and the encoding modules
 * (std/base64, std/hex, std/crypto, std/utf8). Not builtins: the std
 * modules reach these through `extern fn`, so a program links this file
 * only when it loads one of them.
 *
 * Regex: POSIX extended regexes (libc's regcomp/regexec) with the Perl
 * shorthands \d \w \s (and \D \W \S) rewritten into bracket expressions
 * first, so a pattern means the same on glibc and macOS. Compiled
 * patterns are cached (GEM_RX_CACHE entries, keyed by pattern and flags,
 * the oldest replaced first); only the scheduler thread runs Gem code,
 * so the cache needs no lock.
 */

#include "gem.h"
#include <regex.h>

/* ─── Regex ─── */

#define GEM_RX_CACHE 64
#define GEM_RX_ICASE 1
#define GEM_RX_MULTILINE 2

typedef struct {
    char *pattern;     /* malloc'd, as given; NULL: free slot */
    int64_t flags;
    regex_t rx;
    size_t ngroups;
} GemRxEntry;

static GemRxEntry gem_rx_cache[GEM_RX_CACHE];
static int gem_rx_next = 0;

/* Appends to a growable malloc'd buffer. */
typedef struct { char *p; size_t n, cap; } GemSb;

static void sb_put(GemSb *b, const char *s, size_t n) {
    if (b->n + n + 1 > b->cap) {
        b->cap = (b->n + n + 1) * 2;
        b->p = (char *)realloc(b->p, b->cap);
    }
    memcpy(b->p + b->n, s, n);
    b->n += n;
    b->p[b->n] = '\0';
}

static void sb_puts(GemSb *b, const char *s) { sb_put(b, s, strlen(s)); }

/* The pattern with \d \D \w \W \s \S rewritten for POSIX; NULL (and a
   message in *err) for an escape POSIX has no way to say (\b \B). Inside
   a bracket expression the positive classes become their [:class:]
   members; a negated one there can't be written and is an error too. */
static char *gem_rx_translate(const char *pat, const char **err) {
    GemSb b = {0};
    sb_put(&b, "", 0);
    size_t n = strlen(pat);
    for (size_t i = 0; i < n; i++) {
        char c = pat[i];
        if (c == '[') {
            /* Copy a bracket expression, rewriting \d \w \s inside it. */
            size_t j = i + 1;
            sb_put(&b, "[", 1);
            if (j < n && pat[j] == '^') { sb_put(&b, "^", 1); j++; }
            if (j < n && pat[j] == ']') { sb_put(&b, "]", 1); j++; }
            while (j < n && pat[j] != ']') {
                if (pat[j] == '[' && j + 1 < n && (pat[j + 1] == ':' || pat[j + 1] == '.' || pat[j + 1] == '=')) {
                    char close[3] = { pat[j + 1], ']', 0 };
                    const char *e = strstr(pat + j + 2, close);
                    size_t end = e ? (size_t)(e - pat) + 2 : n;
                    sb_put(&b, pat + j, end - j);
                    j = end;
                } else if (pat[j] == '\\' && j + 1 < n && strchr("dwsDWS", pat[j + 1])) {
                    char k = pat[j + 1];
                    if (k == 'd') sb_puts(&b, "0-9");
                    else if (k == 'w') sb_puts(&b, "[:alnum:]_");
                    else if (k == 's') sb_puts(&b, "[:space:]");
                    else { free(b.p); *err = "a negated class (\\D, \\W, \\S) can't go inside [...]"; return NULL; }
                    j += 2;
                } else if (pat[j] == '\\' && j + 1 < n && pat[j + 1] == '\\') {
                    sb_put(&b, "\\", 1);   /* inside brackets a backslash is itself */
                    j += 2;
                } else {
                    sb_put(&b, pat + j, 1);
                    j++;
                }
            }
            if (j < n) sb_put(&b, "]", 1);
            i = j;
            continue;
        }
        if (c == '\\' && i + 1 < n) {
            char k = pat[i + 1];
            const char *rep = NULL;
            switch (k) {
                case 'd': rep = "[0-9]"; break;
                case 'D': rep = "[^0-9]"; break;
                case 'w': rep = "[[:alnum:]_]"; break;
                case 'W': rep = "[^[:alnum:]_]"; break;
                case 's': rep = "[[:space:]]"; break;
                case 'S': rep = "[^[:space:]]"; break;
                case 'b': case 'B': case '<': case '>':
                    free(b.p);
                    *err = "word boundaries (\\b, \\B, \\<, \\>) aren't supported";
                    return NULL;
                case 't': rep = "\t"; break;
                case 'n': rep = "\n"; break;
                case 'r': rep = "\r"; break;
            }
            if (rep) sb_puts(&b, rep);
            else sb_put(&b, pat + i, 2);
            i++;
            continue;
        }
        sb_put(&b, &c, 1);
    }
    return b.p;
}

static GemRxEntry *gem_rx_get(const char *pat, int64_t flags) {
    for (int i = 0; i < GEM_RX_CACHE; i++) {
        GemRxEntry *e = &gem_rx_cache[i];
        if (e->pattern && e->flags == flags && strcmp(e->pattern, pat) == 0) return e;
    }
    const char *terr = NULL;
    char *posix = gem_rx_translate(pat, &terr);
    if (!posix) {
        char buf[600]; snprintf(buf, sizeof(buf), "regex: bad pattern '%.200s': %s", pat, terr);
        gem_error(buf);
    }
    int cflags = REG_EXTENDED;
    if (flags & GEM_RX_ICASE) cflags |= REG_ICASE;
    if (flags & GEM_RX_MULTILINE) cflags |= REG_NEWLINE;
    regex_t rx;
    int rc = regcomp(&rx, posix, cflags);
    free(posix);
    if (rc != 0) {
        char why[200];
        regerror(rc, &rx, why, sizeof(why));
        char buf[600]; snprintf(buf, sizeof(buf), "regex: bad pattern '%.200s': %s", pat, why);
        gem_error(buf);
    }
    GemRxEntry *e = &gem_rx_cache[gem_rx_next];
    gem_rx_next = (gem_rx_next + 1) % GEM_RX_CACHE;
    if (e->pattern) { free(e->pattern); regfree(&e->rx); }
    e->pattern = strdup(pat);
    e->flags = flags;
    e->rx = rx;
    e->ngroups = rx.re_nsub;
    return e;
}

/* The first match of `pat` in s[start..slen) as an array of byte offsets
   [start0, end0, start1, end1, ...] (-1, -1 for a group that took no part),
   or nil. Raises for a bad pattern. */
GemVal gem_regex_exec(const char *pat, int64_t flags, const uint8_t *s, int64_t slen, int64_t start) {
    if (start < 0) start = 0;
    if (start > slen) return GEM_NIL;
    GemRxEntry *e = gem_rx_get(pat, flags);
    size_t nm = e->ngroups + 1;
    regmatch_t stackm[16];
    regmatch_t *m = nm <= 16 ? stackm : (regmatch_t *)malloc(nm * sizeof(regmatch_t));
    m[0].rm_so = (regoff_t)start;
    m[0].rm_eo = (regoff_t)slen;
    int eflags = REG_STARTEND;
    if (start > 0 && !((flags & GEM_RX_MULTILINE) && s[start - 1] == '\n')) eflags |= REG_NOTBOL;
    int rc = regexec(&e->rx, (const char *)s, nm, m, eflags);
    if (rc != 0) {
        if (m != stackm) free(m);
        if (rc == REG_NOMATCH) return GEM_NIL;
        gem_error("regex: the match ran out of memory");
    }
    GemVal out = gem_table_new();
    for (size_t g = 0; g < nm; g++) {
        gem_table_set(out, gem_int((int64_t)(2 * g)), gem_int(m[g].rm_so < 0 ? -1 : (int64_t)m[g].rm_so));
        gem_table_set(out, gem_int((int64_t)(2 * g + 1)), gem_int(m[g].rm_so < 0 ? -1 : (int64_t)m[g].rm_eo));
    }
    if (m != stackm) free(m);
    return out;
}

/* The number of groups in `pat` (raises for a bad pattern). */
int64_t gem_regex_groups(const char *pat, int64_t flags) {
    return (int64_t)gem_rx_get(pat, flags)->ngroups;
}
