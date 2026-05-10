/*
 * gem_builtins_string.c — String builtins: str_replace, substr, chr, ord,
 *                          and the buffer API (buf_new, buf_push).
 */

#include "gem.h"

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
            case VAL_FLOAT: slen = snprintf(tmp, sizeof(tmp), "%g", parts[i].fval); s = tmp; break;
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
    GemBuffer *b = (GemBuffer *)gem_alloc(sizeof(GemBuffer));
    b->cap = 64;
    b->len = 0;
    b->data = (char *)gem_alloc(b->cap);
    GemVal r;
    r.type = VAL_BUFFER;
    r.magic = GEM_MAGIC;
    r.buffer = b;
    return r;
}

GemVal gem_buf_push_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_BUFFER) { char buf[128]; snprintf(buf, sizeof(buf), "buf_push: expected buffer as first argument, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    GemBuffer *b = args[0].buffer;
    /* Coerce second argument to string */
    const char *s;
    int slen;
    char tmp[64];
    GemVal fmt_holder = GEM_NIL;
    switch (args[1].type) {
        case VAL_STRING: s = args[1].sval; slen = args[1].slen; break;
        case VAL_INT: slen = snprintf(tmp, sizeof(tmp), "%lld", (long long)args[1].ival); s = tmp; break;
        case VAL_FLOAT: slen = snprintf(tmp, sizeof(tmp), "%g", args[1].fval); s = tmp; break;
        case VAL_BOOL: s = args[1].bval ? "true" : "false"; slen = args[1].bval ? 4 : 5; break;
        case VAL_NIL: s = "nil"; slen = 3; break;
        case VAL_TABLE:
            fmt_holder = gem_format_value_string(args[1]);
            s = fmt_holder.sval;
            slen = fmt_holder.slen;
            break;
        default: s = ""; slen = 0; break;
    }
    /* Grow buffer if needed */
    while (b->len + slen >= b->cap) {
        int new_cap = b->cap * 2;
        char *new_data = (char *)gem_alloc(new_cap);
        memcpy(new_data, b->data, b->len);
        b->data = new_data;
        b->cap = new_cap;
    }
    memcpy(b->data + b->len, s, slen);
    b->len += slen;
    return args[0]; /* return buffer for chaining */
}

/* ─── build_string ─── */

static GemVal build_string_add_fn(void *_env, GemVal *args, int argc) {
    GemVal buf_val;
    buf_val.type = VAL_BUFFER;
    buf_val.magic = GEM_MAGIC;
    buf_val.buffer = (GemBuffer *)_env;
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
    GemBuffer *b = (GemBuffer *)gem_alloc(sizeof(GemBuffer));
    b->cap = 256;
    b->len = 0;
    b->data = (char *)gem_alloc(b->cap);
    GemVal add_fn;
    add_fn.type = VAL_FN;
    add_fn.magic = GEM_MAGIC;
    add_fn.fn = build_string_add_fn;
    add_fn.env = b;
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
