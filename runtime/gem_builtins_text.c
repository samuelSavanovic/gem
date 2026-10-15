/*
 * gem_builtins_text.c — the C side of std/regex and the encoding modules
 * (std/base64, std/hex, std/crypto, std/utf8). Not builtins: the std
 * modules reach these through `extern fn`, so a program links this file
 * only when it loads one of them.
 *
 * Regex: POSIX extended regexes (libc's regcomp/regexec), rewritten first
 * (gem_rx_translate) into forms glibc and macOS read alike. Compiled
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

/* A bracket expression, collected so it can be written back in the order
   POSIX needs: a literal ']' first, '[' '^' '-' after everything else
   ('-' last), so an escaped one can sit anywhere in the pattern. */
typedef struct {
    GemSb body;
    int close, open, caret, dash;   /* literal ']', '[', '^', '-' */
    int after_range_dash;           /* body ends with a range's '-' */
} GemRxSet;

static const char *GEM_RX_RANGE_ERR =
    "an escaped ']', '[', '^' or '-' inside [...] can't start or end a range";

/* Adds the literal byte `c` to the set; `next` is the pattern from the
   byte after it, `raw`: c was not escaped. NULL, or an error message. An
   unescaped '[', '^' or '-' at either end of a range stays in place, where
   POSIX reads it the same (a range never comes first in the body). */
static const char *rx_set_literal(GemRxSet *set, char c, const char *next, int raw) {
    int special = c == ']' || c == '[' || c == '^' || c == '-';
    int starts_range = next[0] == '-' && next[1] != ']' && next[1] != '\0';
    if (special && raw && (set->after_range_dash || (starts_range && c != '-' && set->body.n > 0))) {
        sb_put(&set->body, &c, 1);
    } else if (special) {
        if (set->after_range_dash || starts_range)
            return raw ? "a range first in [...] can't start with ']', '[', '^' or '-'" : GEM_RX_RANGE_ERR;
        if (c == ']') set->close = 1;
        else if (c == '[') set->open = 1;
        else if (c == '^') set->caret = 1;
        else set->dash = 1;
    } else {
        sb_put(&set->body, &c, 1);
    }
    set->after_range_dash = 0;
    return NULL;
}

/* Translates the bracket expression at pat[i] == '[' onto `b`; returns
   the index of its closing ']', or (size_t)-1 with *err set. Inside it
   \d \w \s become their members, \t \n \r the control bytes, and a
   backslash before punctuation that character itself. */
static size_t gem_rx_bracket(const char *pat, size_t n, size_t i, GemSb *b, const char **err) {
    GemRxSet set = {0};
    sb_put(&set.body, "", 0);
    size_t j = i + 1;
    int neg = 0;
    if (j < n && pat[j] == '^') { neg = 1; j++; }
    size_t first = j;
    while (j < n && (pat[j] != ']' || j == first)) {
        char c = pat[j];
        if (c == '[' && j + 1 < n && (pat[j + 1] == ':' || pat[j + 1] == '.' || pat[j + 1] == '=')) {
            char close[3] = { pat[j + 1], ']', 0 };
            const char *e = strstr(pat + j + 2, close);
            if (!e) { *err = "unclosed [:class:] inside [...]"; goto fail; }
            size_t end = (size_t)(e - pat) + 2;
            sb_put(&set.body, pat + j, end - j);
            set.after_range_dash = 0;
            j = end;
            continue;
        }
        if (c == '-' && j != first && j + 1 < n && pat[j + 1] != ']') {
            if (set.body.n == 0 || set.after_range_dash) { *err = GEM_RX_RANGE_ERR; goto fail; }
            sb_put(&set.body, "-", 1);
            set.after_range_dash = 1;
            j++;
            continue;
        }
        char lit = c;
        size_t step = 1;
        if (c == '\\' && j + 1 < n) {
            char k = pat[j + 1];
            const char *rep = NULL;
            step = 2;
            switch (k) {
                case 'd': rep = "0-9"; break;
                case 'w': rep = "[:alnum:]_"; break;
                case 's': rep = "[:space:]"; break;
                case 'D': case 'W': case 'S':
                    *err = "a negated class (\\D, \\W, \\S) can't go inside [...]";
                    goto fail;
                case 't': lit = '\t'; break;
                case 'n': lit = '\n'; break;
                case 'r': lit = '\r'; break;
                default:
                    if ((k >= '0' && k <= '9') || (k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z') || (unsigned char)k >= 0x80 || k == ' ') {
                        *err = "inside [...] the escapes are \\d \\w \\s \\t \\n \\r and a backslash before punctuation";
                        goto fail;
                    }
                    lit = k;
            }
            if (rep) {
                if (set.after_range_dash) { *err = "a class (\\d, \\w, \\s) can't end a range"; goto fail; }
                sb_puts(&set.body, rep);
                j += step;
                continue;
            }
        }
        const char *e = rx_set_literal(&set, lit, pat + j + step, step == 1);
        if (e) { *err = e; goto fail; }
        j += step;
    }
    if (j >= n) { *err = "unclosed [...]"; goto fail; }
    if (!neg && set.caret && !set.close && !set.open && !set.dash && set.body.n == 0) {
        sb_puts(b, "\\^");                 /* [\^] alone: just the caret */
    } else {
        sb_put(b, "[", 1);
        if (neg) sb_put(b, "^", 1);
        if (set.close) sb_put(b, "]", 1);
        if (!neg && !set.close && set.body.n == 0 && set.caret && !set.open) {
            sb_put(b, "-^", 2);             /* '^' first would negate */
            set.caret = set.dash = 0;
        }
        sb_put(b, set.body.p, set.body.n);
        if (set.open) sb_put(b, "[", 1);
        if (set.caret) sb_put(b, "^", 1);
        if (set.dash) sb_put(b, "-", 1);
        sb_put(b, "]", 1);
    }
    free(set.body.p);
    return j;
fail:
    free(set.body.p);
    return (size_t)-1;
}

/* The pattern rewritten for libc's regcomp, in forms glibc and macOS
   read alike:
   - \d \D \w \W \s \S become bracket expressions, \t \n \r the bytes, and
     bracket expressions are written as gem_rx_bracket writes them;
   - a backslash before any other character that isn't a letter or digit
     (or \< \>, below) is that character: an ERE metacharacter stays
     escaped, and any other loses the backslash;
   - `{,n}` becomes `{0,n}`, and an empty alternative (an empty pattern,
     `a|`, `(|a)`) matches the empty string through `x{0}`;
   - NULL (and a message in *err) for what one of them can't do, such as
     word boundaries (\b \B \< \>), back-references (\1 .. \9), any
     other backslash before a letter or digit, a
     quantifier with nothing to repeat or after another one (`a**`, `a+?`),
     a '{' that doesn't start a bound, a bound past 255, and for a malformed
     pattern (unbalanced parentheses, gem_rx_bracket's errors). */
static char *gem_rx_translate(const char *pat, const char **err) {
    GemSb b = {0};
    sb_put(&b, "", 0);
    size_t n = strlen(pat);
    int atom = 0;        /* the last token can take a quantifier */
    int quantified = 0;  /* the last token is a quantifier */
    int alt_empty = 1;   /* nothing yet in the current alternative */
    int depth = 0;       /* open groups */
#define RX_FAIL(msg) do { free(b.p); *err = (msg); return NULL; } while (0)
    for (size_t i = 0; i < n; i++) {
        char c = pat[i];
        if (c == '|' || c == ')') {
            if (c == ')' && depth-- == 0) RX_FAIL("parentheses not balanced");
            if (alt_empty) sb_puts(&b, "x{0}");
            sb_put(&b, &c, 1);
            alt_empty = c == '|';
            atom = c == ')';
            quantified = 0;
            continue;
        }
        if (c == '*' || c == '+' || c == '?' || c == '{') {
            size_t end = i;
            if (c == '{') {
                size_t j = i + 1;
                long lo = -1, hi = -1;
                while (j < n && pat[j] >= '0' && pat[j] <= '9') { lo = (lo < 0 ? 0 : lo) * 10 + (pat[j] - '0'); if (lo > 100000) lo = 100000; j++; }
                if (j < n && pat[j] == ',') {
                    j++;
                    while (j < n && pat[j] >= '0' && pat[j] <= '9') { hi = (hi < 0 ? 0 : hi) * 10 + (pat[j] - '0'); if (hi > 100000) hi = 100000; j++; }
                } else {
                    hi = lo;
                }
                if (j >= n || pat[j] != '}' || (lo < 0 && hi < 0))
                    RX_FAIL("'{' starts a bound such as {2}, {1,3}, {2,} or {,3}; write \\{ for the character");
                if (lo > 255 || hi > 255) RX_FAIL("a bound can't pass 255");
                if (hi >= 0 && lo > hi) RX_FAIL("a bound's minimum is over its maximum");
                end = j;
            }
            if (quantified) RX_FAIL("a quantifier can't follow another (there are no lazy quantifiers)");
            if (!atom) RX_FAIL("a quantifier needs something before it to repeat");
            if (c == '{' && pat[i + 1] == ',') {
                sb_puts(&b, "{0");
                sb_put(&b, pat + i + 1, end - i);
            } else {
                sb_put(&b, pat + i, end - i + 1);
            }
            i = end;
            atom = 0;
            quantified = 1;
            continue;
        }
        alt_empty = 0;
        quantified = 0;
        atom = 1;
        if (c == '[') {
            size_t end = gem_rx_bracket(pat, n, i, &b, err);
            if (end == (size_t)-1) { free(b.p); return NULL; }
            i = end;
            continue;
        }
        if (c == '(' || c == '^' || c == '$') {
            sb_put(&b, &c, 1);
            atom = 0;
            alt_empty = c == '(';
            if (c == '(') depth++;
            continue;
        }
        if (c == '\\') {
            if (i + 1 >= n) RX_FAIL("the pattern ends in a backslash");
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
                    RX_FAIL("word boundaries (\\b, \\B, \\<, \\>) aren't supported");
                case '1': case '2': case '3': case '4': case '5':
                case '6': case '7': case '8': case '9':
                    RX_FAIL("back-references (\\1 .. \\9) aren't supported");
                case 't': rep = "\t"; break;
                case 'n': rep = "\n"; break;
                case 'r': rep = "\r"; break;
            }
            if (rep) {
                sb_puts(&b, rep);
            } else if ((k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z') || (k >= '0' && k <= '9')) {
                RX_FAIL("unknown escape: a letter or digit after a backslash must be one of d D w W s S t n r");
            } else if (strchr(".[]()*+?{}|^$\\", k)) {
                sb_put(&b, pat + i, 2);
            } else {
                sb_put(&b, &k, 1);
            }
            i++;
            continue;
        }
        sb_put(&b, &c, 1);
    }
    if (depth > 0) RX_FAIL("parentheses not balanced");
#undef RX_FAIL
    if (alt_empty) sb_puts(&b, "x{0}");
    return b.p;
}

/* regcomp's error, in words of our own: glibc's and macOS's regerror
   texts differ. */
static const char *gem_rx_why(int rc) {
    switch (rc) {
        case REG_EBRACK: return "brackets not balanced";
        case REG_EPAREN: return "parentheses not balanced";
        case REG_EBRACE: return "braces not balanced";
        case REG_BADBR: return "bad bound in {...}";
        case REG_ERANGE: return "bad range in [...]";
        case REG_ECTYPE: return "unknown [:class:]";
        case REG_ECOLLATE: return "unknown collating element";
        case REG_EESCAPE: return "bad escape";
        case REG_BADRPT: return "a quantifier needs something before it to repeat";
        case REG_ESPACE: return "out of memory";
        default: return "invalid pattern";
    }
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
        char buf[600]; snprintf(buf, sizeof(buf), "regex: bad pattern '%.200s': %s", pat, gem_rx_why(rc));
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

/* ─── base64 and hex ─── */

static GemVal gem_text_result(char *p, size_t n) {
    GemVal r = gem_string_with_len(p, (int64_t)n);
    free(p);
    return r;
}

static const char GEM_B64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char GEM_B64URL[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

/* base64 of s; `url`: the URL-safe alphabet without padding. */
GemVal gem_base64_encode(const uint8_t *s, int64_t n, int64_t url) {
    const char *alpha = url ? GEM_B64URL : GEM_B64;
    size_t cap = (size_t)((n + 2) / 3) * 4 + 1;
    gem_strlen_check(cap - 1, "base64.encode");
    char *out = (char *)malloc(cap);
    size_t o = 0;
    int64_t i = 0;
    for (; i + 2 < n; i += 3) {
        uint32_t v = ((uint32_t)s[i] << 16) | ((uint32_t)s[i + 1] << 8) | s[i + 2];
        out[o++] = alpha[(v >> 18) & 63];
        out[o++] = alpha[(v >> 12) & 63];
        out[o++] = alpha[(v >> 6) & 63];
        out[o++] = alpha[v & 63];
    }
    if (i < n) {
        uint32_t v = (uint32_t)s[i] << 16;
        if (i + 1 < n) v |= (uint32_t)s[i + 1] << 8;
        out[o++] = alpha[(v >> 18) & 63];
        out[o++] = alpha[(v >> 12) & 63];
        if (i + 1 < n) out[o++] = alpha[(v >> 6) & 63];
        else if (!url) out[o++] = '=';
        if (!url) out[o++] = '=';
    }
    return gem_text_result(out, o);
}

static int gem_b64_val(uint8_t c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+' || c == '-') return 62;
    if (c == '/' || c == '_') return 63;
    return -1;
}

/* Decodes either alphabet, padding optional, ASCII whitespace skipped;
   nil for anything else (a stray character, a lone last character, bits
   after the padding). */
GemVal gem_base64_decode(const uint8_t *s, int64_t n) {
    char *out = (char *)malloc((size_t)(n / 4 + 1) * 3 + 1);
    size_t o = 0;
    uint32_t acc = 0;
    int bits = 0, count = 0, pad = 0;
    for (int64_t i = 0; i < n; i++) {
        uint8_t c = s[i];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') continue;
        if (c == '=') { pad++; continue; }
        int v = gem_b64_val(c);
        if (v < 0 || pad) { free(out); return GEM_NIL; }
        acc = (acc << 6) | (uint32_t)v;
        bits += 6;
        count++;
        if (bits >= 8) {
            bits -= 8;
            out[o++] = (char)((acc >> bits) & 0xff);
        }
    }
    if (count % 4 == 1 || pad > 2 || (pad && (count + pad) % 4 != 0) || (acc & ((1u << bits) - 1)) != 0) {
        free(out);
        return GEM_NIL;
    }
    return gem_text_result(out, o);
}

GemVal gem_hex_encode(const uint8_t *s, int64_t n) {
    static const char digits[] = "0123456789abcdef";
    gem_strlen_check((size_t)n * 2, "hex.encode");
    char *out = (char *)malloc((size_t)n * 2 + 1);
    for (int64_t i = 0; i < n; i++) {
        out[2 * i] = digits[s[i] >> 4];
        out[2 * i + 1] = digits[s[i] & 15];
    }
    return gem_text_result(out, (size_t)n * 2);
}

static int gem_hex_val(uint8_t c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/* nil for an odd length or a non-hex digit. */
GemVal gem_hex_decode(const uint8_t *s, int64_t n) {
    if (n % 2) return GEM_NIL;
    char *out = (char *)malloc((size_t)n / 2 + 1);
    for (int64_t i = 0; i < n; i += 2) {
        int a = gem_hex_val(s[i]), b = gem_hex_val(s[i + 1]);
        if (a < 0 || b < 0) { free(out); return GEM_NIL; }
        out[i / 2] = (char)(a * 16 + b);
    }
    return gem_text_result(out, (size_t)n / 2);
}

/* ─── SHA-256 (FIPS 180-4) and HMAC-SHA256 (RFC 2104) ─── */

typedef struct { uint32_t h[8]; uint8_t buf[64]; uint64_t len; size_t fill; } GemSha256;

static const uint32_t GEM_SHA_K[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

#define GEM_ROR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

static void gem_sha256_block(GemSha256 *c, const uint8_t *p) {
    uint32_t w[64];
    for (int i = 0; i < 16; i++)
        w[i] = ((uint32_t)p[4 * i] << 24) | ((uint32_t)p[4 * i + 1] << 16) | ((uint32_t)p[4 * i + 2] << 8) | p[4 * i + 3];
    for (int i = 16; i < 64; i++) {
        uint32_t s0 = GEM_ROR(w[i - 15], 7) ^ GEM_ROR(w[i - 15], 18) ^ (w[i - 15] >> 3);
        uint32_t s1 = GEM_ROR(w[i - 2], 17) ^ GEM_ROR(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    uint32_t a = c->h[0], b = c->h[1], cc = c->h[2], d = c->h[3], e = c->h[4], f = c->h[5], g = c->h[6], h = c->h[7];
    for (int i = 0; i < 64; i++) {
        uint32_t t1 = h + (GEM_ROR(e, 6) ^ GEM_ROR(e, 11) ^ GEM_ROR(e, 25)) + ((e & f) ^ (~e & g)) + GEM_SHA_K[i] + w[i];
        uint32_t t2 = (GEM_ROR(a, 2) ^ GEM_ROR(a, 13) ^ GEM_ROR(a, 22)) + ((a & b) ^ (a & cc) ^ (b & cc));
        h = g; g = f; f = e; e = d + t1; d = cc; cc = b; b = a; a = t1 + t2;
    }
    c->h[0] += a; c->h[1] += b; c->h[2] += cc; c->h[3] += d;
    c->h[4] += e; c->h[5] += f; c->h[6] += g; c->h[7] += h;
}

static void gem_sha256_init(GemSha256 *c) {
    static const uint32_t iv[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    memcpy(c->h, iv, sizeof(iv));
    c->len = 0;
    c->fill = 0;
}

static void gem_sha256_update(GemSha256 *c, const uint8_t *p, size_t n) {
    c->len += n;
    while (n > 0) {
        if (c->fill == 0 && n >= 64) {
            gem_sha256_block(c, p);
            p += 64; n -= 64;
            continue;
        }
        size_t take = 64 - c->fill < n ? 64 - c->fill : n;
        memcpy(c->buf + c->fill, p, take);
        c->fill += take; p += take; n -= take;
        if (c->fill == 64) { gem_sha256_block(c, c->buf); c->fill = 0; }
    }
}

static void gem_sha256_final(GemSha256 *c, uint8_t out[32]) {
    uint64_t bits = c->len * 8;
    uint8_t pad = 0x80;
    gem_sha256_update(c, &pad, 1);
    uint8_t zero = 0;
    while (c->fill != 56) gem_sha256_update(c, &zero, 1);
    uint8_t lenb[8];
    for (int i = 0; i < 8; i++) lenb[i] = (uint8_t)(bits >> (56 - 8 * i));
    gem_sha256_update(c, lenb, 8);
    for (int i = 0; i < 8; i++) {
        out[4 * i] = (uint8_t)(c->h[i] >> 24); out[4 * i + 1] = (uint8_t)(c->h[i] >> 16);
        out[4 * i + 2] = (uint8_t)(c->h[i] >> 8); out[4 * i + 3] = (uint8_t)c->h[i];
    }
}

/* The SHA-256 digest of s: 32 raw bytes. */
GemVal gem_sha256(const uint8_t *s, int64_t n) {
    GemSha256 c;
    uint8_t d[32];
    gem_sha256_init(&c);
    gem_sha256_update(&c, s, (size_t)n);
    gem_sha256_final(&c, d);
    return gem_string_with_len((const char *)d, 32);
}

/* HMAC-SHA256 of msg under key: 32 raw bytes. */
GemVal gem_hmac_sha256(const uint8_t *key, int64_t klen, const uint8_t *msg, int64_t mlen) {
    uint8_t k[64] = {0}, ipad[64], opad[64], inner[32], d[32];
    if (klen > 64) {
        GemSha256 c;
        gem_sha256_init(&c);
        gem_sha256_update(&c, key, (size_t)klen);
        gem_sha256_final(&c, k);
    } else {
        memcpy(k, key, (size_t)klen);
    }
    for (int i = 0; i < 64; i++) { ipad[i] = k[i] ^ 0x36; opad[i] = k[i] ^ 0x5c; }
    GemSha256 c;
    gem_sha256_init(&c);
    gem_sha256_update(&c, ipad, 64);
    gem_sha256_update(&c, msg, (size_t)mlen);
    gem_sha256_final(&c, inner);
    gem_sha256_init(&c);
    gem_sha256_update(&c, opad, 64);
    gem_sha256_update(&c, inner, 32);
    gem_sha256_final(&c, d);
    return gem_string_with_len((const char *)d, 32);
}

/* 1 when a and b hold the same bytes, in time that depends only on the
   lengths (for comparing MACs and tokens). */
int gem_bytes_equal_ct(const uint8_t *a, int64_t an, const uint8_t *b, int64_t bn) {
    if (an != bn) return 0;
    uint8_t diff = 0;
    for (int64_t i = 0; i < an; i++) diff |= a[i] ^ b[i];
    return diff == 0;
}

/* ─── UTF-8 ─── */

/* Decodes the sequence at s[i]: its code point in *cp and its length;
   an invalid or overlong sequence, a surrogate or a code point past
   U+10FFFF is one byte long with *cp = -1. */
static int gem_utf8_next(const uint8_t *s, int64_t n, int64_t i, int32_t *cp) {
    uint8_t c = s[i];
    if (c < 0x80) { *cp = c; return 1; }
    int len; int32_t v; int32_t min;
    if ((c & 0xe0) == 0xc0) { len = 2; v = c & 0x1f; min = 0x80; }
    else if ((c & 0xf0) == 0xe0) { len = 3; v = c & 0x0f; min = 0x800; }
    else if ((c & 0xf8) == 0xf0) { len = 4; v = c & 0x07; min = 0x10000; }
    else { *cp = -1; return 1; }
    if (i + len > n) { *cp = -1; return 1; }
    for (int k = 1; k < len; k++) {
        if ((s[i + k] & 0xc0) != 0x80) { *cp = -1; return 1; }
        v = (v << 6) | (s[i + k] & 0x3f);
    }
    if (v < min || v > 0x10ffff || (v >= 0xd800 && v <= 0xdfff)) { *cp = -1; return 1; }
    *cp = v;
    return len;
}

static size_t gem_utf8_put(char *out, int32_t cp) {
    if (cp < 0x80) { out[0] = (char)cp; return 1; }
    if (cp < 0x800) { out[0] = (char)(0xc0 | (cp >> 6)); out[1] = (char)(0x80 | (cp & 0x3f)); return 2; }
    if (cp < 0x10000) {
        out[0] = (char)(0xe0 | (cp >> 12)); out[1] = (char)(0x80 | ((cp >> 6) & 0x3f));
        out[2] = (char)(0x80 | (cp & 0x3f)); return 3;
    }
    out[0] = (char)(0xf0 | (cp >> 18)); out[1] = (char)(0x80 | ((cp >> 12) & 0x3f));
    out[2] = (char)(0x80 | ((cp >> 6) & 0x3f)); out[3] = (char)(0x80 | (cp & 0x3f));
    return 4;
}

/* The number of characters (an invalid byte counts as one). */
int64_t gem_utf8_len(const uint8_t *s, int64_t n) {
    int64_t count = 0;
    int32_t cp;
    for (int64_t i = 0; i < n; count++) i += gem_utf8_next(s, n, i, &cp);
    return count;
}

/* 1 when s is valid UTF-8. */
int gem_utf8_valid(const uint8_t *s, int64_t n) {
    int32_t cp;
    for (int64_t i = 0; i < n;) {
        i += gem_utf8_next(s, n, i, &cp);
        if (cp < 0) return 0;
    }
    return 1;
}

/* An array of the code points (`as_strings` 0) or of each character's
   bytes (1); an invalid byte gives 0xFFFD, or its one byte. */
GemVal gem_utf8_split(const uint8_t *s, int64_t n, int64_t as_strings) {
    GemVal out = gem_table_new();
    int64_t k = 0;
    int32_t cp;
    for (int64_t i = 0; i < n;) {
        int len = gem_utf8_next(s, n, i, &cp);
        GemVal v = as_strings ? gem_string_with_len((const char *)s + i, len) : gem_int(cp < 0 ? 0xfffd : cp);
        gem_table_set(out, gem_int(k++), v);
        i += len;
    }
    return out;
}

/* The UTF-8 encoding of code point cp; nil for a surrogate, a negative
   one or one past U+10FFFF. */
GemVal gem_utf8_char(int64_t cp) {
    if (cp < 0 || cp > 0x10ffff || (cp >= 0xd800 && cp <= 0xdfff)) return GEM_NIL;
    char b[4];
    size_t len = gem_utf8_put(b, (int32_t)cp);
    return gem_string_with_len(b, (int64_t)len);
}

/* The byte offset of character `index` (n for index == count); -1 past it. */
int64_t gem_utf8_offset(const uint8_t *s, int64_t n, int64_t index) {
    int64_t i = 0, c = 0;
    int32_t cp;
    while (c < index && i < n) { i += gem_utf8_next(s, n, i, &cp); c++; }
    return c == index ? i : -1;
}

/* Simple case mapping (one code point to one) for the scripts where it is
   a fixed offset or alternating pairs: ASCII, Latin-1, Latin Extended-A,
   Greek, Cyrillic, Armenian, and the fullwidth Latin letters. */
static int32_t gem_case_map(int32_t c, int upper) {
    if (upper) {
        if (c >= 'a' && c <= 'z') return c - 32;
        if (c == 0xb5) return 0x39c;
        if ((c >= 0xe0 && c <= 0xfe && c != 0xf7)) return c - 32;
        if (c == 0xff) return 0x178;
        if (c >= 0x100 && c <= 0x17f && c != 0x130 && c != 0x131 && c != 0x138 && c != 0x149 && c != 0x17f) {
            int odd_low = (c >= 0x139 && c <= 0x148) || (c >= 0x179 && c <= 0x17e);
            if (odd_low ? (c % 2 == 0) : (c % 2 == 1)) return c - 1;
            return c;
        }
        if (c == 0x17f) return 'S';
        if (c >= 0x3b1 && c <= 0x3cb && c != 0x3c2) return c - 32;
        if (c == 0x3c2) return 0x3a3;
        if (c >= 0x3ac && c <= 0x3af) return c == 0x3ac ? 0x386 : c - 37;
        if (c == 0x3cc) return 0x38c;
        if (c == 0x3cd || c == 0x3ce) return c - 63;
        if (c >= 0x430 && c <= 0x44f) return c - 32;
        if (c >= 0x450 && c <= 0x45f) return c - 80;
        if (c == 0x4cf) return 0x4c0;
        if (c >= 0x460 && c <= 0x4ff && c % 2 == 1 && !(c >= 0x482 && c <= 0x489) && !(c >= 0x4c1 && c <= 0x4ce)) return c - 1;
        if (c >= 0x4c2 && c <= 0x4ce && c % 2 == 0) return c - 1;
        if (c >= 0x561 && c <= 0x586) return c - 48;
        if (c >= 0xff41 && c <= 0xff5a) return c - 32;
        return c;
    }
    if (c >= 'A' && c <= 'Z') return c + 32;
    if (c >= 0xc0 && c <= 0xde && c != 0xd7) return c + 32;
    if (c == 0x178) return 0xff;
    if (c >= 0x100 && c <= 0x17f && c != 0x130 && c != 0x131 && c != 0x138 && c != 0x149 && c != 0x17f) {
        int odd_low = (c >= 0x139 && c <= 0x148) || (c >= 0x179 && c <= 0x17e);
        if (odd_low ? (c % 2 == 1) : (c % 2 == 0)) return c + 1;
        return c;
    }
    if (c >= 0x391 && c <= 0x3ab && c != 0x3a2) return c + 32;
    if (c == 0x386) return 0x3ac;
    if (c >= 0x388 && c <= 0x38a) return c + 37;
    if (c == 0x38c) return 0x3cc;
    if (c == 0x38e || c == 0x38f) return c + 63;
    if (c >= 0x410 && c <= 0x42f) return c + 32;
    if (c >= 0x400 && c <= 0x40f) return c + 80;
    if (c >= 0x460 && c <= 0x4ff && c % 2 == 0 && !(c >= 0x482 && c <= 0x489) && !(c >= 0x4c0 && c <= 0x4cf)) return c + 1;
    if (c >= 0x4c1 && c <= 0x4cd && c % 2 == 1) return c + 1;
    if (c == 0x4c0) return 0x4cf;
    if (c >= 0x531 && c <= 0x556) return c + 48;
    if (c >= 0xff21 && c <= 0xff3a) return c + 32;
    return c;
}

/* s with each character case-mapped; invalid bytes are kept as they are. */
GemVal gem_utf8_case(const uint8_t *s, int64_t n, int64_t upper) {
    gem_strlen_check((size_t)n, upper ? "utf8.upper" : "utf8.lower");
    char *out = (char *)malloc((size_t)n + 1);
    size_t o = 0;
    int32_t cp;
    for (int64_t i = 0; i < n;) {
        int len = gem_utf8_next(s, n, i, &cp);
        if (cp < 0) { out[o++] = (char)s[i]; i += 1; continue; }
        int32_t m = gem_case_map(cp, (int)upper);
        /* No mapping makes a character longer (ſ -> S makes one shorter),
           so the output fits in n bytes. */
        o += gem_utf8_put(out + o, m);
        i += len;
    }
    return gem_text_result(out, o);
}
