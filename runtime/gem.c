/*
 * gem.c — Gem language runtime implementation.
 * Uses Boehm GC for memory management and stb_ds for hash indexing.
 */

#include <gc.h>

#define MINICORO_IMPL
#include "minicoro.h"

#define STB_DS_IMPLEMENTATION
#define STBDS_REALLOC(c, p, s) GC_REALLOC((p), (s))
#define STBDS_FREE(c, p)       ((void)(p))  /* GC handles freeing */
#include "stb_ds.h"

#include <poll.h>
#include <fcntl.h>
#include <errno.h>

#include "gem.h"

/* ─── GC-aware allocators ─── */

#define ALLOC(type) ((type *)GC_MALLOC(sizeof(type)))
#define ALLOC_N(type, n) ((type *)GC_MALLOC(sizeof(type) * (n)))

/* ─── Table internals ─── */

/* String key -> index mapping for O(1) lookup */
typedef struct {
    char *key;       /* stb_ds uses this field name */
    int value;       /* index into keys/vals arrays */
} GemStrIndex;

struct GemTable {
    GemVal *keys;
    GemVal *vals;
    int len;
    int cap;
    GemStrIndex *str_index;  /* stb_ds string hash map (NULL until first string key) */
};

/* ─── Globals ─── */

GemVal GEM_NIL = {VAL_NIL, {0}};

/* ─── Call stack ─── */

GemFrame gem_call_stack[GEM_MAX_CALL_DEPTH];
int gem_call_depth = 0;

/* ─── pcall jump buffer stack ─── */

GemPcallFrame gem_pcall_stack[GEM_MAX_PCALL_DEPTH];
int gem_pcall_depth = 0;

void gem_print_stack_trace(void) {
    int max = gem_call_depth < GEM_MAX_CALL_DEPTH ? gem_call_depth : GEM_MAX_CALL_DEPTH;
    for (int i = max - 1; i >= 0; i--) {
        fprintf(stderr, "  at %s (%s:%d)\n",
            gem_call_stack[i].name,
            gem_call_stack[i].file,
            gem_call_stack[i].line);
    }
}

/* ─── Constructors ─── */

GemVal gem_int(int64_t v) { return (GemVal){VAL_INT, {.ival = v}}; }
GemVal gem_float(double v) { GemVal r; r.type = VAL_FLOAT; r.fval = v; return r; }
GemVal gem_bool(int v) { GemVal r; r.type = VAL_BOOL; r.bval = v; return r; }
GemVal gem_make_fn(GemFnPtr f, void *env) { GemVal r; r.type = VAL_FN; r.fn = f; r.env = env; return r; }

GemVal gem_string(const char *s) {
    GemVal r;
    r.type = VAL_STRING;
    size_t len = strlen(s) + 1;
    r.sval = (char *)GC_MALLOC_ATOMIC(len);
    memcpy(r.sval, s, len);
    return r;
}

/* ─── Forward decl ─── */

void gem_error(const char *msg);

/* ─── Table operations ─── */

int gem_val_eq(GemVal a, GemVal b);

static void gem_table_grow(GemTable *t) {
    int new_cap = t->cap * 2;
    GemVal *new_keys = ALLOC_N(GemVal, new_cap);
    GemVal *new_vals = ALLOC_N(GemVal, new_cap);
    memcpy(new_keys, t->keys, sizeof(GemVal) * t->len);
    memcpy(new_vals, t->vals, sizeof(GemVal) * t->len);
    t->keys = new_keys;
    t->vals = new_vals;
    t->cap = new_cap;
}

GemVal gem_table_new(void) {
    GemTable *t = ALLOC(GemTable);
    t->len = 0;
    t->cap = 4;
    t->keys = ALLOC_N(GemVal, 4);
    t->vals = ALLOC_N(GemVal, 4);
    t->str_index = NULL;
    GemVal r; r.type = VAL_TABLE; r.table = t; return r;
}

void gem_table_set(GemVal tbl, GemVal key, GemVal val) {
    if (tbl.type != VAL_TABLE) { gem_error("index set on non-table"); }
    GemTable *t = tbl.table;

    /* String key: use hash index for O(1) lookup */
    if (key.type == VAL_STRING) {
        if (t->str_index != NULL) {
            ptrdiff_t idx = shgeti(t->str_index, key.sval);
            if (idx >= 0) {
                t->vals[t->str_index[idx].value] = val;
                return;
            }
        }
        /* Not found — append */
        if (t->len >= t->cap) gem_table_grow(t);
        int pos = t->len;
        t->keys[pos] = key;
        t->vals[pos] = val;
        t->len++;
        shput(t->str_index, key.sval, pos);
        return;
    }

    /* Integer key: check for direct array-style indexing */
    if (key.type == VAL_INT) {
        int64_t ik = key.ival;
        /* If key is within existing range, update in place */
        if (ik >= 0 && ik < t->len && t->keys[ik].type == VAL_INT && t->keys[ik].ival == ik) {
            t->vals[ik] = val;
            return;
        }
    }

    /* Fallback: linear scan for non-string, non-array-pattern keys */
    for (int i = 0; i < t->len; i++) {
        if (gem_val_eq(t->keys[i], key)) {
            t->vals[i] = val;
            return;
        }
    }

    /* Append new entry */
    if (t->len >= t->cap) gem_table_grow(t);
    t->keys[t->len] = key;
    t->vals[t->len] = val;
    t->len++;
}

GemVal gem_table_get(GemVal tbl, GemVal key) {
    /* String indexing: tbl[i] on a string returns single char */
    if (tbl.type == VAL_STRING) {
        if (key.type != VAL_INT) { gem_error("string index must be int"); }
        int64_t idx = key.ival;
        int64_t slen = (int64_t)strlen(tbl.sval);
        if (idx < 0 || idx >= slen) { gem_error("string index out of bounds"); }
        char buf[2]; buf[0] = tbl.sval[idx]; buf[1] = '\0';
        return gem_string(buf);
    }

    if (tbl.type != VAL_TABLE) { gem_error("index get on non-table"); }
    GemTable *t = tbl.table;

    /* String key: use hash index */
    if (key.type == VAL_STRING) {
        if (t->str_index != NULL) {
            ptrdiff_t idx = shgeti(t->str_index, key.sval);
            if (idx >= 0) return t->vals[t->str_index[idx].value];
        }
        return (GemVal){VAL_NIL, {0}};
    }

    /* Integer key: try direct array indexing */
    if (key.type == VAL_INT) {
        int64_t ik = key.ival;
        if (ik >= 0 && ik < t->len && t->keys[ik].type == VAL_INT && t->keys[ik].ival == ik) {
            return t->vals[ik];
        }
    }

    /* Fallback: linear scan */
    for (int i = 0; i < t->len; i++) {
        if (gem_val_eq(t->keys[i], key)) return t->vals[i];
    }
    return (GemVal){VAL_NIL, {0}};
}

int gem_val_eq(GemVal a, GemVal b) {
    if (a.type != b.type) return 0;
    switch (a.type) {
        case VAL_NIL: return 1;
        case VAL_BOOL: return a.bval == b.bval;
        case VAL_INT: return a.ival == b.ival;
        case VAL_FLOAT: return a.fval == b.fval;
        case VAL_STRING: return strcmp(a.sval, b.sval) == 0;
        default: return 0;
    }
}

/* ─── Truthiness ─── */

int gem_truthy(GemVal v) {
    if (v.type == VAL_NIL) return 0;
    if (v.type == VAL_BOOL) return v.bval;
    return 1;
}

/* ─── Runtime error ─── */

void gem_raise_error(const char *msg) {
    if (gem_pcall_depth > 0) {
        gem_pcall_depth--;
        GemPcallFrame *frame = &gem_pcall_stack[gem_pcall_depth];
        /* GC-allocate the message so it survives the longjmp */
        size_t len = strlen(msg) + 1;
        char *saved_msg = (char *)GC_MALLOC_ATOMIC(len);
        memcpy(saved_msg, msg, len);
        frame->error_msg = saved_msg;
        /* Snapshot the call stack */
        GemVal stack_snapshot = gem_table_new();
        int max = gem_call_depth < GEM_MAX_CALL_DEPTH ? gem_call_depth : GEM_MAX_CALL_DEPTH;
        int saved = frame->saved_call_depth;
        for (int i = max - 1, idx = 0; i >= saved; i--, idx++) {
            GemVal f = gem_table_new();
            gem_table_set(f, gem_string("name"), gem_string(gem_call_stack[i].name));
            gem_table_set(f, gem_string("file"), gem_string(gem_call_stack[i].file));
            gem_table_set(f, gem_string("line"), gem_int(gem_call_stack[i].line));
            gem_table_set(stack_snapshot, gem_int(idx), f);
        }
        frame->stack_snapshot = stack_snapshot;
        gem_call_depth = saved;
        longjmp(frame->buf, 1);
    }
    fprintf(stderr, "error: %s\n", msg);
    gem_print_stack_trace();
    exit(1);
}

void gem_error(const char *msg) {
    gem_raise_error(msg);
}

/* ─── Arithmetic / operators ─── */

GemVal gem_add(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int(a.ival + b.ival);
    if (a.type == VAL_FLOAT && b.type == VAL_FLOAT) return gem_float(a.fval + b.fval);
    if (a.type == VAL_INT && b.type == VAL_FLOAT) return gem_float((double)a.ival + b.fval);
    if (a.type == VAL_FLOAT && b.type == VAL_INT) return gem_float(a.fval + (double)b.ival);
    if (a.type == VAL_STRING && b.type == VAL_STRING) {
        size_t la = strlen(a.sval), lb = strlen(b.sval);
        char *s = (char *)GC_MALLOC_ATOMIC(la + lb + 1);
        memcpy(s, a.sval, la);
        memcpy(s + la, b.sval, lb + 1);
        GemVal r; r.type = VAL_STRING; r.sval = s; return r;
    }
    gem_error("type error in +"); return GEM_NIL;
}

GemVal gem_sub(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int(a.ival - b.ival);
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_float(fa - fb);
    }
    gem_error("type error in -"); return GEM_NIL;
}

GemVal gem_mul(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int(a.ival * b.ival);
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_float(fa * fb);
    }
    gem_error("type error in *"); return GEM_NIL;
}

GemVal gem_div(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) {
        if (b.ival == 0) gem_error("division by zero");
        return gem_int(a.ival / b.ival);
    }
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        if (fb == 0.0) gem_error("division by zero");
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        return gem_float(fa / fb);
    }
    gem_error("type error in /"); return GEM_NIL;
}

GemVal gem_mod(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) {
        if (b.ival == 0) gem_error("division by zero");
        return gem_int(a.ival % b.ival);
    }
    gem_error("type error in %"); return GEM_NIL;
}

GemVal gem_eq(GemVal a, GemVal b) {
    if (a.type != b.type) return gem_bool(0);
    switch (a.type) {
        case VAL_NIL: return gem_bool(1);
        case VAL_BOOL: return gem_bool(a.bval == b.bval);
        case VAL_INT: return gem_bool(a.ival == b.ival);
        case VAL_FLOAT: return gem_bool(a.fval == b.fval);
        case VAL_STRING: return gem_bool(strcmp(a.sval, b.sval) == 0);
        default: return gem_bool(0);
    }
}

GemVal gem_neq(GemVal a, GemVal b) {
    return gem_bool(!gem_truthy(gem_eq(a, b)));
}

GemVal gem_lt(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_bool(a.ival < b.ival);
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_bool(fa < fb);
    }
    if (a.type == VAL_STRING && b.type == VAL_STRING) return gem_bool(strcmp(a.sval, b.sval) < 0);
    gem_error("type error in <"); return GEM_NIL;
}

GemVal gem_gt(GemVal a, GemVal b) { return gem_lt(b, a); }
GemVal gem_le(GemVal a, GemVal b) { return gem_bool(!gem_truthy(gem_gt(a, b))); }
GemVal gem_ge(GemVal a, GemVal b) { return gem_bool(!gem_truthy(gem_lt(a, b))); }

GemVal gem_neg(GemVal a) {
    if (a.type == VAL_INT) return gem_int(-a.ival);
    if (a.type == VAL_FLOAT) return gem_float(-a.fval);
    gem_error("type error in unary -"); return GEM_NIL;
}

GemVal gem_not(GemVal a) {
    return gem_bool(!gem_truthy(a));
}

/* ─── Built-in: print ─── */

GemVal gem_print(void *_env, GemVal *args, int argc) {
    (void)_env;
    for (int i = 0; i < argc; i++) {
        if (i > 0) printf(" ");
        GemVal v = args[i];
        switch (v.type) {
            case VAL_NIL: printf("nil"); break;
            case VAL_BOOL: printf("%s", v.bval ? "true" : "false"); break;
            case VAL_INT: printf("%lld", (long long)v.ival); break;
            case VAL_FLOAT: printf("%g", v.fval); break;
            case VAL_STRING: printf("%s", v.sval); break;
            case VAL_FN: printf("<fn>"); break;
            case VAL_TABLE: printf("<table:%d>", v.table->len); break;
            case VAL_BUFFER: printf("<buffer:%d>", v.buffer->len); break;
        }
    }
    printf("\n");
    return GEM_NIL;
}

/* ─── Built-in: error ─── */

GemVal gem_error_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc > 0 && args[0].type == VAL_STRING) {
        gem_raise_error(args[0].sval);
    } else {
        gem_raise_error("error");
    }
    return GEM_NIL;
}

/* ─── Built-in: len ─── */

GemVal gem_len_val(GemVal v) {
    if (v.type == VAL_STRING) return gem_int((int64_t)strlen(v.sval));
    if (v.type == VAL_TABLE) return gem_int((int64_t)v.table->len);
    gem_error("len: expected string or table");
    return GEM_NIL;
}

GemVal gem_len_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("len: expected 1 argument"); }
    return gem_len_val(args[0]);
}

/* ─── Built-in: type ─── */

GemVal gem_type_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) return gem_string("nil");
    switch (args[0].type) {
        case VAL_NIL: return gem_string("nil");
        case VAL_BOOL: return gem_string("bool");
        case VAL_INT: return gem_string("int");
        case VAL_FLOAT: return gem_string("float");
        case VAL_STRING: return gem_string("string");
        case VAL_FN: return gem_string("fn");
        case VAL_TABLE: return gem_string("table");
        case VAL_BUFFER: return gem_string("buffer");
    }
    return gem_string("unknown");
}

/* ─── Built-in: to_string ─── */

GemVal gem_to_string_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) return gem_string("");
    GemVal v = args[0];
    char buf[64];
    switch (v.type) {
        case VAL_NIL: return gem_string("nil");
        case VAL_BOOL: return gem_string(v.bval ? "true" : "false");
        case VAL_INT: snprintf(buf, sizeof(buf), "%lld", (long long)v.ival); return gem_string(buf);
        case VAL_FLOAT: snprintf(buf, sizeof(buf), "%g", v.fval); return gem_string(buf);
        case VAL_STRING: return v;
        case VAL_FN: return gem_string("<fn>");
        case VAL_TABLE: snprintf(buf, sizeof(buf), "<table:%d>", v.table->len); return gem_string(buf);
        case VAL_BUFFER: snprintf(buf, sizeof(buf), "<buffer:%d>", v.buffer->len); return gem_string(buf);
    }
    return gem_string("");
}

/* ─── Built-in: push ─── */

GemVal gem_push_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("push: expected 2 arguments"); }
    GemVal tbl = args[0];
    if (tbl.type != VAL_TABLE) { gem_error("push: expected table as first argument"); }
    GemTable *t = tbl.table;
    GemVal val = args[1];
    if (t->len >= t->cap) gem_table_grow(t);
    t->keys[t->len] = gem_int(t->len);
    t->vals[t->len] = val;
    t->len++;
    return val;
}

/* ─── Built-in: keys ─── */

GemVal gem_keys(GemVal tbl) {
    if (tbl.type != VAL_TABLE) {
        gem_error("keys: expected table");
        return GEM_NIL;
    }
    GemTable *t = tbl.table;
    GemVal result = gem_table_new();
    for (int i = 0; i < t->len; i++) {
        gem_table_set(result, gem_int(i), t->keys[i]);
    }
    return result;
}

GemVal gem_keys_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("keys: expected 1 argument"); return GEM_NIL; }
    return gem_keys(args[0]);
}

/* ─── Built-in: has_key ─── */

GemVal gem_has_key_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("has_key: expected 2 arguments"); }
    if (args[0].type != VAL_TABLE) { gem_error("has_key: expected table as first argument"); }
    GemTable *t = args[0].table;
    GemVal key = args[1];

    /* String key: use hash index */
    if (key.type == VAL_STRING) {
        if (t->str_index != NULL) {
            ptrdiff_t idx = shgeti(t->str_index, key.sval);
            if (idx >= 0) return gem_bool(1);
        }
        return gem_bool(0);
    }

    /* Integer key: try direct array indexing */
    if (key.type == VAL_INT) {
        int64_t ik = key.ival;
        if (ik >= 0 && ik < t->len && t->keys[ik].type == VAL_INT && t->keys[ik].ival == ik) {
            return gem_bool(1);
        }
    }

    /* Fallback: linear scan */
    for (int i = 0; i < t->len; i++) {
        if (gem_val_eq(t->keys[i], key)) return gem_bool(1);
    }
    return gem_bool(0);
}

/* ─── Built-in: str_replace ─── */

GemVal gem_str_replace_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 3) { gem_error("str_replace: expected 3 arguments"); }
    if (args[0].type != VAL_STRING || args[1].type != VAL_STRING || args[2].type != VAL_STRING) {
        gem_error("str_replace: all arguments must be strings");
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

/* ─── Built-in: error with location ─── */

GemVal gem_error_at_fn(const char *file, int line, GemVal *args, int argc) {
    if (gem_pcall_depth > 0) {
        /* Caught by pcall — pass just the user's message */
        if (argc > 0 && args[0].type == VAL_STRING) {
            gem_raise_error(args[0].sval);
        } else {
            gem_raise_error("error");
        }
    } else {
        /* No pcall — fatal error with file:line prefix */
        if (argc > 0 && args[0].type == VAL_STRING) {
            fprintf(stderr, "%s:%d: error: %s\n", file, line, args[0].sval);
        } else {
            fprintf(stderr, "%s:%d: error\n", file, line);
        }
        gem_print_stack_trace();
        exit(1);
    }
    return GEM_NIL;
}

/* ─── Built-in: pcall ─── */

GemVal gem_pcall_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_FN) {
        gem_error("pcall: expected function argument");
    }

    GemVal fn = args[0];

    if (gem_pcall_depth >= GEM_MAX_PCALL_DEPTH) {
        gem_error("pcall: too many nested pcall levels");
    }

    int frame_idx = gem_pcall_depth;
    gem_pcall_stack[frame_idx].saved_call_depth = gem_call_depth;
    gem_pcall_depth++;

    volatile GemVal result = gem_table_new();

    if (setjmp(gem_pcall_stack[frame_idx].buf) == 0) {
        /* Normal path — call the function */
        GemVal value = fn.fn(fn.env, NULL, 0);
        gem_pcall_depth--;
        gem_table_set(result, gem_string("ok"), gem_bool(1));
        gem_table_set(result, gem_string("value"), value);
    } else {
        /* Error path — longjmp landed here */
        gem_table_set(result, gem_string("ok"), gem_bool(0));
        gem_table_set(result, gem_string("error"), gem_string(gem_pcall_stack[frame_idx].error_msg));
        gem_table_set(result, gem_string("stack"), gem_pcall_stack[frame_idx].stack_snapshot);
    }

    return result;
}

/* ─── Built-in: substr ─── */

GemVal gem_substr_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("substr: expected 2-3 arguments"); }
    if (args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        gem_error("substr: expected (string, int[, int])");
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
    if (argc < 1 || args[0].type != VAL_INT) { gem_error("chr: expected int argument"); }
    char buf[2];
    buf[0] = (char)(args[0].ival & 0xFF);
    buf[1] = '\0';
    return gem_string(buf);
}

GemVal gem_ord_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) { gem_error("ord: expected string argument"); }
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
    if (argc < 2 || args[0].type != VAL_BUFFER) { gem_error("buf_push: expected buffer and value"); }
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
    if (argc < 1 || args[0].type != VAL_BUFFER) { gem_error("buf_str: expected buffer"); }
    GemBuffer *b = args[0].buffer;
    char *s = (char *)GC_MALLOC_ATOMIC(b->len + 1);
    memcpy(s, b->data, b->len);
    s[b->len] = '\0';
    GemVal r;
    r.type = VAL_STRING;
    r.sval = s;
    return r;
}

/* ─── Concurrency: scheduler + mailbox ─── */

GemProcess gem_proc_table[GEM_MAX_PROCS];
int gem_current_pid = -1;

/* GC-aware stack allocator for minicoro coroutines.
   malloc the stack, then register it as a GC root so Boehm
   can scan coroutine stacks for heap pointers. */
static void *gem_coro_stack_alloc(size_t size, void *udata) {
    (void)udata;
    void *ptr = malloc(size);
    if (!ptr) return NULL;
    GC_add_roots(ptr, (char *)ptr + size);
    return ptr;
}

static void gem_coro_stack_free(void *ptr, size_t size, void *udata) {
    (void)udata;
    GC_remove_roots(ptr, (char *)ptr + size);
    free(ptr);
}

/* Mailbox operations */
static void gem_mailbox_push(GemMailbox *mb, GemVal val) {
    GemMsgNode *node = (GemMsgNode *)GC_MALLOC(sizeof(GemMsgNode));
    node->value = val;
    node->next = NULL;
    if (mb->tail) {
        mb->tail->next = node;
    } else {
        mb->head = node;
    }
    mb->tail = node;
}

static int gem_mailbox_empty(GemMailbox *mb) {
    return mb->head == NULL;
}

static GemVal gem_mailbox_pop(GemMailbox *mb) {
    GemMsgNode *node = mb->head;
    GemVal val = node->value;
    mb->head = node->next;
    if (!mb->head) mb->tail = NULL;
    return val;
}

/* Coroutine entry point — calls the GemFnPtr stored in user_data */
typedef struct {
    GemFnPtr fn;
    void *env;
} GemCoroCtx;

static void gem_coro_entry(mco_coro *co) {
    GemCoroCtx *ctx = (GemCoroCtx *)mco_get_user_data(co);
    ctx->fn(ctx->env, NULL, 0);
}

/* Core API */

int gem_spawn_fn(GemFnPtr fn, void *env) {
    int pid = -1;
    for (int i = 0; i < GEM_MAX_PROCS; i++) {
        if (gem_proc_table[i].state == GEM_PROC_FREE) {
            pid = i;
            break;
        }
    }
    if (pid < 0) {
        gem_error("spawn: process table full");
        return -1;
    }

    GemCoroCtx *ctx = (GemCoroCtx *)GC_MALLOC(sizeof(GemCoroCtx));
    ctx->fn = fn;
    ctx->env = env;

    mco_desc desc = mco_desc_init(gem_coro_entry, 0);
    desc.alloc_cb = gem_coro_stack_alloc;
    desc.dealloc_cb = gem_coro_stack_free;
    desc.user_data = ctx;

    mco_coro *co;
    mco_result res = mco_create(&co, &desc);
    if (res != MCO_SUCCESS) {
        gem_error("spawn: coroutine creation failed");
        return -1;
    }

    gem_proc_table[pid].state = GEM_PROC_READY;
    gem_proc_table[pid].coro = co;
    gem_proc_table[pid].mailbox = (GemMailbox){NULL, NULL};
    gem_proc_table[pid].pid = pid;

    return pid;
}

void gem_send_msg(int pid, GemVal val) {
    if (pid < 0 || pid >= GEM_MAX_PROCS) return;
    GemProcess *proc = &gem_proc_table[pid];
    if (proc->state == GEM_PROC_FREE || proc->state == GEM_PROC_DEAD) return;

    gem_mailbox_push(&proc->mailbox, val);

    if (proc->state == GEM_PROC_WAITING) {
        proc->state = GEM_PROC_READY;
    }
}

GemVal gem_receive_msg(void) {
    if (gem_current_pid < 0 || gem_current_pid >= GEM_MAX_PROCS) {
        gem_error("receive: not inside a spawned process");
        return GEM_NIL;
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];

    while (gem_mailbox_empty(&proc->mailbox)) {
        proc->state = GEM_PROC_WAITING;
        mco_yield(proc->coro);
    }

    return gem_mailbox_pop(&proc->mailbox);
}

int gem_self_pid(void) {
    return gem_current_pid;
}

void gem_io_yield(int fd, int for_write) {
    if (gem_current_pid < 0 || gem_current_pid >= GEM_MAX_PROCS) {
        /* Not inside a coroutine — can't yield, just return and let
           the caller retry with a blocking call or spin. */
        return;
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    proc->state = GEM_PROC_IO_WAIT;
    proc->wait_fd = fd;
    proc->wait_write = for_write;
    mco_yield(proc->coro);
}

void gem_run_scheduler(void) {
    int active = 1;

    while (active) {
        active = 0;
        int has_ready = 0;
        int has_io_wait = 0;
        int has_msg_wait = 0;

        for (int i = 0; i < GEM_MAX_PROCS; i++) {
            GemProcess *proc = &gem_proc_table[i];

            if (proc->state == GEM_PROC_READY) {
                has_ready = 1;
                active = 1;
                gem_current_pid = i;
                mco_resume(proc->coro);

                if (mco_status(proc->coro) == MCO_DEAD) {
                    mco_destroy(proc->coro);
                    proc->coro = NULL;
                    proc->state = GEM_PROC_FREE;
                }
            } else if (proc->state == GEM_PROC_WAITING) {
                has_msg_wait = 1;
                active = 1;
            } else if (proc->state == GEM_PROC_IO_WAIT) {
                has_io_wait = 1;
                active = 1;
            }
        }

        /* If we ran at least one READY process, loop immediately —
           the resumed coroutines may have enqueued messages or spawned
           new work. */
        if (has_ready) continue;

        /* Nothing was READY. Check for I/O waiters. */
        if (has_io_wait) {
            /* Build a pollfd array for all IO_WAIT processes. */
            struct pollfd pfds[GEM_MAX_PROCS];
            int pfd_pid[GEM_MAX_PROCS];  /* maps pollfd index → pid */
            int nfds = 0;

            for (int i = 0; i < GEM_MAX_PROCS; i++) {
                if (gem_proc_table[i].state == GEM_PROC_IO_WAIT) {
                    pfds[nfds].fd = gem_proc_table[i].wait_fd;
                    pfds[nfds].events = gem_proc_table[i].wait_write ? POLLOUT : POLLIN;
                    pfds[nfds].revents = 0;
                    pfd_pid[nfds] = i;
                    nfds++;
                }
            }

            /* Block until at least one fd is ready.
               Use -1 timeout: the only way to make progress is I/O. */
            int ready = poll(pfds, (nfds_t)nfds, -1);
            if (ready > 0) {
                for (int j = 0; j < nfds; j++) {
                    if (pfds[j].revents & (POLLIN | POLLOUT | POLLERR | POLLHUP)) {
                        gem_proc_table[pfd_pid[j]].state = GEM_PROC_READY;
                    }
                }
            }
            /* Loop back to resume the now-READY processes. */
            continue;
        }

        /* Only mailbox waiters remain and nobody can send → deadlock. */
        if (has_msg_wait && !has_io_wait) break;
    }
    gem_current_pid = -1;
}

/* Built-in function wrappers for use from compiled Gem code */

GemVal gem_spawn_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_FN) {
        gem_error("spawn: expected function argument");
    }
    int pid = gem_spawn_fn(args[0].fn, args[0].env);
    return gem_int(pid);
}

GemVal gem_send_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_INT) {
        gem_error("send: expected (pid, value)");
    }
    gem_send_msg((int)args[0].ival, args[1]);
    return GEM_NIL;
}

GemVal gem_receive_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    (void)args;
    (void)argc;
    return gem_receive_msg();
}

GemVal gem_self_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    (void)args;
    (void)argc;
    return gem_int(gem_self_pid());
}
