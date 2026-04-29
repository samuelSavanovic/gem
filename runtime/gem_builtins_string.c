/*
 * gem_builtins_string.c — String builtins: str_replace, substr, chr, ord,
 *                          and the buffer API (buf_new, buf_push, buf_str).
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
    size_t s_len = strlen(s);
    size_t old_len = strlen(old);
    size_t new_len = strlen(new_s);

    if (old_len == 0) return args[0]; /* empty pattern — return original */

    /* Count occurrences to pre-allocate */
    int count = 0;
    const char *p = s;
    while ((p = strstr(p, old)) != NULL) {
        count++;
        p += old_len;
    }
    if (count == 0) return args[0];

    size_t result_len = s_len + (size_t)count * (new_len - old_len);
    char *result = (char *)GC_MALLOC_ATOMIC(result_len + 1);
    char *dst = result;
    p = s;
    while (*p) {
        if (strncmp(p, old, old_len) == 0) {
            memcpy(dst, new_s, new_len);
            dst += new_len;
            p += old_len;
        } else {
            *dst++ = *p++;
        }
    }
    *dst = '\0';
    GemVal r; r.type = VAL_STRING; r.sval = result;
    return r;
}

/* ─── Built-in: substr ─── */

GemVal gem_substr_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("substr: expected 2-3 arguments"); }
    if (args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        char buf[128]; snprintf(buf, sizeof(buf), "substr: expected (string, int[, int]), got (%s, %s)", gem_type_str(args[0]), gem_type_str(args[1])); gem_error(buf);
    }
    const char *s = args[0].sval;
    int64_t start = args[1].ival;
    int64_t slen = (int64_t)strlen(s);

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

    char *buf = (char *)GC_MALLOC_ATOMIC((size_t)count + 1);
    memcpy(buf, s + start, (size_t)count);
    buf[count] = '\0';
    GemVal r; r.type = VAL_STRING; r.sval = buf;
    return r;
}

/* ─── Built-in: chr / ord ─── */

GemVal gem_chr_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) { char buf[128]; snprintf(buf, sizeof(buf), "chr: expected int argument, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    char buf[2];
    buf[0] = (char)(args[0].ival & 0xFF);
    buf[1] = '\0';
    return gem_string(buf);
}

GemVal gem_ord_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) { char buf[128]; snprintf(buf, sizeof(buf), "ord: expected string argument, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    if (argc >= 2) {
        /* ord(s, i) — direct byte access by index, no allocation */
        if (args[1].type != VAL_INT) { gem_error("ord: index must be an integer"); }
        int64_t idx = args[1].ival;
        int64_t slen = (int64_t)strlen(args[0].sval);
        if (idx < 0 || idx >= slen) { gem_error("ord: index out of bounds"); }
        return gem_int((int64_t)(unsigned char)args[0].sval[idx]);
    }
    if (strlen(args[0].sval) == 0) { gem_error("ord: empty string"); }
    return gem_int((int64_t)(unsigned char)args[0].sval[0]);
}

/* ─── String interpolation ─── */

static void interp_append(char **data, int *len, int *cap, const char *s, int slen) {
    while (*len + slen >= *cap) {
        int new_cap = *cap * 2;
        char *new_data = (char *)GC_MALLOC_ATOMIC(new_cap);
        memcpy(new_data, *data, *len);
        *data = new_data;
        *cap = new_cap;
    }
    memcpy(*data + *len, s, slen);
    *len += slen;
}

GemVal gem_interp(int n, GemVal *parts) {
    int cap = 128, len = 0;
    char *data = (char *)GC_MALLOC_ATOMIC(cap);
    char tmp[64];
    for (int i = 0; i < n; i++) {
        const char *s;
        int slen;
        switch (parts[i].type) {
            case VAL_STRING: s = parts[i].sval; slen = (int)strlen(s); break;
            case VAL_INT: slen = snprintf(tmp, sizeof(tmp), "%lld", (long long)parts[i].ival); s = tmp; break;
            case VAL_FLOAT: slen = snprintf(tmp, sizeof(tmp), "%g", parts[i].fval); s = tmp; break;
            case VAL_BOOL: s = parts[i].bval ? "true" : "false"; slen = parts[i].bval ? 4 : 5; break;
            case VAL_NIL: s = "nil"; slen = 3; break;
            case VAL_TABLE: slen = snprintf(tmp, sizeof(tmp), "<table:%d>", parts[i].table->len); s = tmp; break;
            case VAL_FN: s = "<fn>"; slen = 4; break;
            case VAL_BUFFER: slen = snprintf(tmp, sizeof(tmp), "<buffer:%d>", parts[i].buffer->len); s = tmp; break;
            case VAL_REF: slen = snprintf(tmp, sizeof(tmp), "#Ref<%lld>", (long long)parts[i].rval); s = tmp; break;
            default: s = ""; slen = 0; break;
        }
        interp_append(&data, &len, &cap, s, slen);
    }
    char *s = (char *)GC_MALLOC_ATOMIC(len + 1);
    memcpy(s, data, len);
    s[len] = '\0';
    GemVal r;
    r.type = VAL_STRING;
    r.sval = s;
    return r;
}

/* ─── String builder (buf_new / buf_push / buf_str) ─── */

GemVal gem_buf_new_fn(void *_env, GemVal *args, int argc) {
    (void)_env; (void)args; (void)argc;
    GemBuffer *b = (GemBuffer *)GC_MALLOC(sizeof(GemBuffer));
    b->cap = 64;
    b->len = 0;
    b->data = (char *)GC_MALLOC_ATOMIC(b->cap);
    GemVal r;
    r.type = VAL_BUFFER;
    r.buffer = b;
    return r;
}

GemVal gem_buf_push_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_BUFFER) { char buf[128]; snprintf(buf, sizeof(buf), "buf_push: expected buffer as first argument, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    GemBuffer *b = args[0].buffer;
    /* Coerce second argument to string */
    const char *s;
    char tmp[64];
    switch (args[1].type) {
        case VAL_STRING: s = args[1].sval; break;
        case VAL_INT: snprintf(tmp, sizeof(tmp), "%lld", (long long)args[1].ival); s = tmp; break;
        case VAL_FLOAT: snprintf(tmp, sizeof(tmp), "%g", args[1].fval); s = tmp; break;
        case VAL_BOOL: s = args[1].bval ? "true" : "false"; break;
        case VAL_NIL: s = "nil"; break;
        default: s = ""; break;
    }
    int slen = (int)strlen(s);
    /* Grow buffer if needed */
    while (b->len + slen >= b->cap) {
        int new_cap = b->cap * 2;
        char *new_data = (char *)GC_MALLOC_ATOMIC(new_cap);
        memcpy(new_data, b->data, b->len);
        b->data = new_data;
        b->cap = new_cap;
    }
    memcpy(b->data + b->len, s, slen);
    b->len += slen;
    return args[0]; /* return buffer for chaining */
}

GemVal gem_buf_str_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_BUFFER) { char buf[128]; snprintf(buf, sizeof(buf), "buf_str: expected buffer, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    GemBuffer *b = args[0].buffer;
    char *s = (char *)GC_MALLOC_ATOMIC(b->len + 1);
    memcpy(s, b->data, b->len);
    s[b->len] = '\0';
    GemVal r;
    r.type = VAL_STRING;
    r.sval = s;
    return r;
}

/* ─── build_string ─── */

static GemVal build_string_add_fn(void *_env, GemVal *args, int argc) {
    GemVal buf_val;
    buf_val.type = VAL_BUFFER;
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
    GemBuffer *b = (GemBuffer *)GC_MALLOC(sizeof(GemBuffer));
    b->cap = 256;
    b->len = 0;
    b->data = (char *)GC_MALLOC_ATOMIC(b->cap);
    GemVal add_fn;
    add_fn.type = VAL_FN;
    add_fn.fn = build_string_add_fn;
    add_fn.env = b;
    GemVal block_args[1] = {add_fn};
    args[0].fn(args[0].env, block_args, 1);
    char *s = (char *)GC_MALLOC_ATOMIC(b->len + 1);
    memcpy(s, b->data, b->len);
    s[b->len] = '\0';
    GemVal r;
    r.type = VAL_STRING;
    r.sval = s;
    return r;
}
