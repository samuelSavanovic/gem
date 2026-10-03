/*
 * gem_builtins_string.c — String builtins: str_replace, substr, chr, ord,
 *                          and the buffer API (buf_new, buf_push), plus
 *                          gem_bytes_span and gem_bytes_find (extern helpers
 *                          for std/http).
 */

#include "gem.h"

/* ─── gem_bytes_span (extern helper, see gem.h) ─── */

/* std/http calls it with a handful of constant byte sets, many times per
 * request, so the last few sets' lookup tables are kept: a set that is
 * byte-for-byte one of them (memcmp, much cheaper than rebuilding the
 * table) reuses its table. The cache is not locked: only the scheduler
 * thread may call this, so declare it as a plain (non-blocking) extern fn,
 * never `extern blocking fn` (those run on the thread pool). 16 slots hold
 * every set std/http uses (9) with room to spare. */
#define GEM_SPAN_CACHE 16
#define GEM_SPAN_CACHE_MAX 256
static struct {
    int64_t n;           /* a zeroed slot is the (valid) entry for "" */
    uint8_t set[GEM_SPAN_CACHE_MAX];
    uint8_t ok[256];
} gem_span_cache[GEM_SPAN_CACHE];
static int gem_span_cache_next = 0;

static const uint8_t *gem_span_table(const uint8_t *accept, int64_t accept_n, uint8_t *scratch) {
    if (accept_n <= GEM_SPAN_CACHE_MAX) {
        for (int k = 0; k < GEM_SPAN_CACHE; k++) {
            if (gem_span_cache[k].n == accept_n && memcmp(gem_span_cache[k].set, accept, (size_t)accept_n) == 0)
                return gem_span_cache[k].ok;
        }
        int k = gem_span_cache_next;
        gem_span_cache_next = (k + 1) % GEM_SPAN_CACHE;
        scratch = gem_span_cache[k].ok;
        memcpy(gem_span_cache[k].set, accept, (size_t)accept_n);
        gem_span_cache[k].n = accept_n;
    }
    memset(scratch, 0, 256);
    for (int64_t i = 0; i < accept_n; i++) scratch[accept[i]] = 1;
    return scratch;
}

int64_t gem_bytes_span(const uint8_t *s, int64_t n, const uint8_t *accept, int64_t accept_n) {
    uint8_t scratch[256];
    const uint8_t *ok = gem_span_table(accept, accept_n, scratch);
    int64_t i = 0;
    while (i < n && ok[s[i]]) i++;
    return i;
}

/* ─── gem_bytes_find (extern helper, see gem.h) ─── */

int64_t gem_bytes_find(const uint8_t *s, int64_t n, const uint8_t *needle, int64_t nn, int64_t from) {
    if (from < 0) from = 0;
    if (nn == 0) return from <= n ? from : -1;
    if (from > n - nn) return -1;
    const uint8_t *p = s + from;
    const uint8_t *last = s + (n - nn);
    while (p <= last) {
        p = memchr(p, needle[0], (size_t)(last - p) + 1);
        if (!p) return -1;
        if (memcmp(p, needle, (size_t)nn) == 0) return (int64_t)(p - s);
        p++;
    }
    return -1;
}

/* ─── Built-in: str_replace ─── */

GemVal gem_str_replace_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 3) { gem_error("str_replace: expected 3 arguments"); }
    if (args[0].type != VAL_STRING || args[1].type != VAL_STRING || args[2].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "str_replace: all arguments must be strings, got %s, %s, %s", gem_type_str(args[0]), gem_type_str(args[1]), gem_type_str(args[2])); gem_error(buf);
    }
    const char *s = args[0].sval;
    const char *old = args[1].sval;
    const char *new_s = args[2].sval;
    size_t s_len = (size_t)args[0].slen;
    size_t old_len = (size_t)args[1].slen;
    size_t new_len = (size_t)args[2].slen;

    if (old_len == 0) return args[0]; /* empty pattern — return original */

    /* Count occurrences (binary-safe; non-overlapping). */
    int count = 0;
    size_t i = 0;
    while (i + old_len <= s_len) {
        if (memcmp(s + i, old, old_len) == 0) { count++; i += old_len; }
        else { i++; }
    }
    if (count == 0) return args[0];

    /* Modular arithmetic in size_t: when new_len < old_len, the subtraction
     * wraps but the final sum lands at the correct non-negative result. */
    size_t result_len = s_len + (size_t)count * (new_len - old_len);
    char *result = (char *)gem_alloc(result_len + 1);
    char *dst = result;
    i = 0;
    while (i < s_len) {
        if (i + old_len <= s_len && memcmp(s + i, old, old_len) == 0) {
            memcpy(dst, new_s, new_len);
            dst += new_len;
            i += old_len;
        } else {
            *dst++ = s[i++];
        }
    }
    *dst = '\0';
    GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = result; r.slen = (int)(dst - result);
    return r;
}

/* ─── Built-in: substr ─── */

GemVal gem_substr_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("substr: expected 2-3 arguments"); }
    /* Buffers are mutable strings; treat their byte slice the same way. */
    const char *s;
    int64_t slen;
    if (args[0].type == VAL_STRING) {
        s = args[0].sval;
        slen = (int64_t)args[0].slen;
    } else if (args[0].type == VAL_BUFFER) {
        s = args[0].buffer->data;
        slen = (int64_t)args[0].buffer->len;
    } else {
        char buf[128]; snprintf(buf, sizeof(buf), "substr: expected (string|buffer, int[, int]), got (%s, %s)", gem_type_str(args[0]), gem_type_str(args[1])); gem_error(buf);
        return GEM_NIL;
    }
    if (args[1].type != VAL_INT) {
        char buf[128]; snprintf(buf, sizeof(buf), "substr: expected (string|buffer, int[, int]), got (%s, %s)", gem_type_str(args[0]), gem_type_str(args[1])); gem_error(buf);
    }
    int64_t start = args[1].ival;

    if (start < 0) start = 0;
    if (start >= slen) return gem_string("");

    int64_t count;
    if (argc >= 3 && args[2].type == VAL_INT) {
        count = args[2].ival;
        if (count < 0) count = 0;
    } else {
        count = slen - start;
    }
    if (start + count > slen) count = slen - start;

    char *buf = (char *)gem_alloc((size_t)count + 1);
    memcpy(buf, s + start, (size_t)count);
    buf[count] = '\0';
    GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = buf; r.slen = (int)count;
    return r;
}

/* ─── Built-in: chr / ord ─── */

GemVal gem_chr_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) { char buf[128]; snprintf(buf, sizeof(buf), "chr: expected int argument, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    char buf[2];
    buf[0] = (char)(args[0].ival & 0xFF);
    buf[1] = '\0';
    return gem_string_with_len(buf, 1);
}

GemVal gem_ord_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("ord: expected string|buffer argument, got nothing"); }
    const char *data;
    int64_t slen;
    if (args[0].type == VAL_STRING) {
        data = args[0].sval;
        slen = (int64_t)args[0].slen;
    } else if (args[0].type == VAL_BUFFER) {
        data = args[0].buffer->data;
        slen = (int64_t)args[0].buffer->len;
    } else {
        char buf[128]; snprintf(buf, sizeof(buf), "ord: expected string|buffer argument, got %s", gem_type_str(args[0])); gem_error(buf);
        return GEM_NIL;
    }
    if (argc >= 2) {
        /* ord(s, i) — direct byte access by index, no allocation */
        if (args[1].type != VAL_INT) { gem_error("ord: index must be an integer"); }
        int64_t idx = args[1].ival;
        if (idx < 0 || idx >= slen) { gem_error("ord: index out of bounds"); }
        return gem_int((int64_t)(unsigned char)data[idx]);
    }
    if (slen == 0) { gem_error("ord: empty string"); }
    return gem_int((int64_t)(unsigned char)data[0]);
}

/* ─── String interpolation ─── */

static void interp_append(char **data, int *len, int *cap, const char *s, int slen) {
    while (*len + slen >= *cap) {
        int new_cap = *cap * 2;
        char *new_data = (char *)gem_alloc(new_cap);
        memcpy(new_data, *data, *len);
        *data = new_data;
        *cap = new_cap;
    }
    memcpy(*data + *len, s, slen);
    *len += slen;
}

GemVal gem_interp(int n, GemVal *parts) {
    int cap = 128, len = 0;
    char *data = (char *)gem_alloc(cap);
    char tmp[64];
    for (int i = 0; i < n; i++) {
        const char *s;
        int slen;
        switch (parts[i].type) {
            case VAL_STRING: s = parts[i].sval; slen = parts[i].slen; break;
            case VAL_INT: slen = snprintf(tmp, sizeof(tmp), "%lld", (long long)parts[i].ival); s = tmp; break;
            case VAL_FLOAT: slen = gem_format_float(parts[i].fval, tmp); s = tmp; break;
            case VAL_BOOL: s = parts[i].bval ? "true" : "false"; slen = parts[i].bval ? 4 : 5; break;
            case VAL_NIL: s = "nil"; slen = 3; break;
            case VAL_TABLE: {
                /* Format table inline via the shared repr; build the result
                 * into a fresh string and route through `s`/`slen`. */
                GemVal fmt = gem_format_value_string(parts[i]);
                s = fmt.sval;
                slen = fmt.slen;
                break;
            }
            case VAL_FN: s = "<fn>"; slen = 4; break;
            case VAL_BUFFER: slen = snprintf(tmp, sizeof(tmp), "<buffer:%d>", parts[i].buffer->len); s = tmp; break;
            case VAL_REF: slen = snprintf(tmp, sizeof(tmp), "#Ref<%lld>", (long long)parts[i].rval); s = tmp; break;
            default: s = ""; slen = 0; break;
        }
        interp_append(&data, &len, &cap, s, slen);
    }
    char *s = (char *)gem_alloc(len + 1);
    memcpy(s, data, len);
    s[len] = '\0';
    GemVal r;
    r.type = VAL_STRING;
    r.magic = GEM_MAGIC;
    r.sval = s;
    r.slen = len;
    return r;
}

/* ─── String builder (buf_new / buf_push) ───
 *
 * Buffers are finalized to a string via the generic `to_string` builtin
 * (`gem_to_string_fn` handles VAL_BUFFER); there is no longer a dedicated
 * `buf_str`. */

GemVal gem_buf_new_fn(void *_env, GemVal *args, int argc) {
    (void)_env; (void)args; (void)argc;
    GemBuffer *b = gem_buffer_alloc(64);
    GemVal r;
    r.type = VAL_BUFFER;
    r.magic = GEM_MAGIC;
    r.buffer = b;
    return r;
}

/* Appends `n` bytes at `src` to `b`, growing it in the arena. `src` may
 * point into `b->data` itself: the old block is copied, not freed. */
static void buf_append_bytes(GemBuffer *b, const char *src, int n) {
    if (n <= 0) return;
    if (b->len + n >= b->cap) {
        int new_cap = b->cap > 16 ? b->cap : 16;
        while (b->len + n >= new_cap) new_cap *= 2;
        char *new_data = (char *)gem_alloc(new_cap);
        memcpy(new_data, b->data, b->len);
        b->data = new_data;
        b->cap = new_cap;
    }
    memcpy(b->data + b->len, src, n);
    b->len += n;
}

GemVal gem_buf_push_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) gem_error("buf_push: expected 2 arguments (buffer, value)");
    if (args[0].type != VAL_BUFFER) { char buf[128]; snprintf(buf, sizeof(buf), "buf_push: expected buffer as first argument, got %s", gem_type_str(args[0])); gem_error(buf); }
    GemBuffer *b = args[0].buffer;
    GemVal v = args[1];
    switch (v.type) {
        case VAL_STRING:
            buf_append_bytes(b, v.sval, v.slen);
            break;
        case VAL_BUFFER:
            /* Its current contents. `buf_append_bytes` reads the source
             * before it replaces `b->data`, and the old block stays valid
             * (arena memory is not freed mid-call), so pushing a buffer into
             * itself appends a snapshot. */
            buf_append_bytes(b, v.buffer->data, v.buffer->len);
            break;
        case VAL_FLOAT: {
            /* Exactly what to_string gives (floats have one formatter). */
            GemVal s = gem_to_string_fn(NULL, &v, 1);
            buf_append_bytes(b, s.sval, s.slen);
            break;
        }
        default:
            /* nil, bool, int (pids too), fn, ref, table: the shared repr
             * to_string uses, written straight into the buffer. */
            gem_format_value_to_buf(v, b, 0);
            break;
    }
    return args[0]; /* return buffer for chaining */
}

/* ─── build_string ─── */

/* `add`'s env is an ordinary closure env (see gem_copy_fill_env):
 * [n = 2][box: the VAL_BUFFER][box: the owner's Gem-visible pid]. Copies
 * (spawn, send, module snapshot units, region resets) then treat it like any
 * closure. A copy in another process holds a copy of the buffer, so `add`
 * refuses to run there instead of silently filling a buffer nobody reads. */
static int64_t build_string_owner(void) {
    return gem_current_pid >= 0 ? gem_pid_of_slot(gem_current_pid) : -1;
}

static GemVal build_string_add_fn(void *_env, GemVal *args, int argc) {
    GemVal **fields = (GemVal **)((char *)_env + sizeof(intptr_t));
    if (fields[1]->ival != build_string_owner())
        gem_error("build_string: `add` can only be called by the process that created it");
    GemVal buf_val = *fields[0];
    for (int i = 0; i < argc; i++) {
        GemVal push_args[2] = {buf_val, args[i]};
        gem_buf_push_fn(NULL, push_args, 2);
    }
    return GEM_NIL;
}

GemVal gem_build_string_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_FN) {
        gem_error("build_string: expected a function argument");
    }
    GemBuffer *b = gem_buffer_alloc(256);
    GemVal *buf_box = (GemVal *)gem_alloc(sizeof(GemVal));
    buf_box->type = VAL_BUFFER;
    buf_box->magic = GEM_MAGIC;
    buf_box->buffer = b;
    GemVal *owner_box = (GemVal *)gem_alloc(sizeof(GemVal));
    *owner_box = gem_int(build_string_owner());
    void *env = gem_alloc(sizeof(intptr_t) + 2 * sizeof(GemVal *));
    *(intptr_t *)env = 2;
    GemVal **fields = (GemVal **)((char *)env + sizeof(intptr_t));
    fields[0] = buf_box;
    fields[1] = owner_box;
    GemVal add_fn;
    add_fn.type = VAL_FN;
    add_fn.magic = GEM_MAGIC;
    add_fn.fn = build_string_add_fn;
    add_fn.env = env;
    GemVal block_args[1] = {add_fn};
    args[0].fn(args[0].env, block_args, 1);
    char *s = (char *)gem_alloc(b->len + 1);
    memcpy(s, b->data, b->len);
    s[b->len] = '\0';
    GemVal r;
    r.type = VAL_STRING;
    r.magic = GEM_MAGIC;
    r.sval = s;
    r.slen = b->len;
    return r;
}
