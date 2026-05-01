/*
 * gem_core.c -- GemVal constructors, table operations, equality, truthiness.
 */

#include <stdlib.h>
#include <time.h>

#define STB_DS_IMPLEMENTATION
#define STBDS_REALLOC(c, p, s) realloc((p), (s))
#define STBDS_FREE(c, p)       free(p)
#include "stb_ds.h"

#include "gem.h"
#include <signal.h>

/* ─── Globals ─── */

GemVal GEM_NIL = {VAL_NIL, GEM_MAGIC, {0}};

/* ─── Stored argc/argv for argv() builtin ─── */

int gem_stored_argc = 0;
char **gem_stored_argv = NULL;

/* ─── RNG state (xorshift64*) ─── */

static uint64_t gem_rng_state = 0;

uint64_t gem_rng_next(void) {
    if (gem_rng_state == 0) {
        gem_rng_state = (uint64_t)time(NULL) ^ ((uint64_t)clock() << 32);
        if (gem_rng_state == 0) gem_rng_state = 1;
    }
    uint64_t x = gem_rng_state;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    gem_rng_state = x;
    return x * 0x2545F4914F6CDD1DULL;
}

void gem_init_char_cache(void);

void gem_init(int argc, char **argv) {
    gem_stored_argc = argc;
    gem_stored_argv = argv;
    gem_rng_state = (uint64_t)time(NULL) ^ ((uint64_t)clock() << 32);
    if (gem_rng_state == 0) gem_rng_state = 1;
    signal(SIGPIPE, SIG_IGN);

    gem_arena_init(&gem_global_arena);
    gem_init_char_cache();
}

/* ─── Single-character string cache ─── */

static GemVal gem_char_cache[256];
static int gem_char_cache_ready = 0;

void gem_init_char_cache(void) {
    for (int i = 0; i < 256; i++) {
        char *s = (char *)gem_arena_alloc(&gem_global_arena, 2);
        s[0] = (char)i;
        s[1] = '\0';
        gem_char_cache[i].type = VAL_STRING;
        gem_char_cache[i].magic = GEM_MAGIC;
        gem_char_cache[i].sval = s;
    }
    gem_char_cache_ready = 1;
}

/* ─── Constructors ─── */

GemVal gem_int(int64_t v) { return (GemVal){VAL_INT, GEM_MAGIC, {.ival = v}}; }
GemVal gem_float(double v) { GemVal r; r.type = VAL_FLOAT; r.magic = GEM_MAGIC; r.fval = v; return r; }
GemVal gem_bool(int v) { GemVal r; r.type = VAL_BOOL; r.magic = GEM_MAGIC; r.bval = v; return r; }
GemVal gem_make_fn(GemFnPtr f, void *env) { GemVal r; r.type = VAL_FN; r.magic = GEM_MAGIC; r.fn = f; r.env = env; return r; }

/* Global ref counter — single-threaded coroutine model means a plain
   increment is safe. Starts at 1 so refs are never accidentally 0. */
static int64_t gem_ref_counter = 0;
GemVal gem_make_ref(void) {
    GemVal r;
    r.type = VAL_REF;
    r.magic = GEM_MAGIC;
    r.rval = ++gem_ref_counter;
    return r;
}

GemVal gem_string(const char *s) {
    GemVal r;
    r.type = VAL_STRING;
    r.magic = GEM_MAGIC;
    size_t len = strlen(s) + 1;
    r.sval = (char *)gem_alloc(len);
    memcpy(r.sval, s, len);
    return r;
}

/* ─── Table operations ─── */

void gem_table_grow(GemTable *t) {
    int new_cap = t->cap * 2;
    GemVal *new_keys = ALLOC_N(GemVal, new_cap);
    GemVal *new_vals = ALLOC_N(GemVal, new_cap);
    memcpy(new_keys, t->keys, sizeof(GemVal) * t->len);
    memcpy(new_vals, t->vals, sizeof(GemVal) * t->len);
    t->keys = new_keys;
    t->vals = new_vals;
    t->cap = new_cap;
}

/* Shared with gem_copy.c (gem_deep_copy_table stamps fresh shape ids). */
uint32_t gem_shape_counter = 1;

GemVal gem_table_new(void) {
    GemTable *t = ALLOC(GemTable);
    t->len = 0;
    t->cap = 4;
    t->keys = ALLOC_N(GemVal, 4);
    t->vals = ALLOC_N(GemVal, 4);
    t->str_index = NULL;
    t->shape_id = gem_shape_counter++;

    GemArena *a = gem_current_arena();
    t->arena_next = a->table_list;
    a->table_list = t;

    GemVal r; r.type = VAL_TABLE; r.magic = GEM_MAGIC; r.table = t; return r;
}

void gem_table_freeze(GemVal tbl) {
    if (tbl.type == VAL_TABLE) tbl.table->immutable = 1;
}

void gem_table_set(GemVal tbl, GemVal key, GemVal val) {
    if (tbl.type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "index set on non-table: got %s", gem_type_str(tbl)); gem_error(buf); }
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
        if (ik < 0) {
            int64_t resolved = (int64_t)t->len + ik;
            if (resolved < 0) { char buf[128]; snprintf(buf, sizeof(buf), "array index out of bounds: %lld", (long long)ik); gem_error(buf); }
            if (resolved < t->len && t->keys[resolved].type == VAL_INT && t->keys[resolved].ival == resolved) {
                t->vals[resolved] = val;
                return;
            }
            char buf[128]; snprintf(buf, sizeof(buf), "array index out of bounds: %lld", (long long)ik); gem_error(buf);
        }
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
        if (key.type != VAL_INT) { char buf[128]; snprintf(buf, sizeof(buf), "string index must be int, got %s", gem_type_str(key)); gem_error(buf); }
        int64_t idx = key.ival;
        int64_t slen = (int64_t)strlen(tbl.sval);
        if (idx < 0) idx = slen + idx;
        if (idx < 0 || idx >= slen) { gem_error("string index out of bounds"); }
        return gem_char_cache[(unsigned char)tbl.sval[idx]];
    }

    if (tbl.type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "index get on non-table: got %s", gem_type_str(tbl)); gem_error(buf); }
    GemTable *t = tbl.table;

    /* String key: use hash index */
    if (key.type == VAL_STRING) {
        if (t->str_index != NULL) {
            ptrdiff_t idx = shgeti(t->str_index, key.sval);
            if (idx >= 0) return t->vals[t->str_index[idx].value];
        }
        return (GemVal){VAL_NIL, GEM_MAGIC, {0}};
    }

    /* Integer key: try direct array indexing */
    if (key.type == VAL_INT) {
        int64_t ik = key.ival;
        if (ik < 0) {
            int64_t resolved = (int64_t)t->len + ik;
            if (resolved < 0) { char buf[128]; snprintf(buf, sizeof(buf), "array index out of bounds: %lld", (long long)ik); gem_error(buf); }
            if (resolved < t->len && t->keys[resolved].type == VAL_INT && t->keys[resolved].ival == resolved) {
                return t->vals[resolved];
            }
            char buf[128]; snprintf(buf, sizeof(buf), "array index out of bounds: %lld", (long long)ik); gem_error(buf);
        }
        if (ik >= 0 && ik < t->len && t->keys[ik].type == VAL_INT && t->keys[ik].ival == ik) {
            return t->vals[ik];
        }
    }

    /* Fallback: linear scan */
    for (int i = 0; i < t->len; i++) {
        if (gem_val_eq(t->keys[i], key)) return t->vals[i];
    }
    return (GemVal){VAL_NIL, GEM_MAGIC, {0}};
}

/* ─── Inline cache miss path ─── */

GemVal gem_table_get_ic_miss(GemTable *t, const char *key, GemICacheSlot *cache) {
    if (t->str_index != NULL) {
        ptrdiff_t idx = shgeti(t->str_index, key);
        if (idx >= 0) {
            int vi = t->str_index[idx].value;
            cache->table = t;
            cache->shape_id = t->shape_id;
            cache->val_index = vi;
            return t->vals[vi];
        }
    }
    cache->table = NULL;
    return (GemVal){VAL_NIL, GEM_MAGIC, {0}};
}

/* ─── Equality ─── */

int gem_val_eq(GemVal a, GemVal b) {
    if (a.type != b.type) return 0;
    switch (a.type) {
        case VAL_NIL: return 1;
        case VAL_BOOL: return a.bval == b.bval;
        case VAL_INT: return a.ival == b.ival;
        case VAL_FLOAT: return a.fval == b.fval;
        case VAL_STRING: return strcmp(a.sval, b.sval) == 0;
        case VAL_REF: return a.rval == b.rval;
        default: return 0;
    }
}

/* ─── Type name helper ─── */

const char *gem_type_str(GemVal v) {
    switch (v.type) {
        case VAL_NIL:    return "nil";
        case VAL_BOOL:   return "bool";
        case VAL_INT:    return "int";
        case VAL_FLOAT:  return "float";
        case VAL_STRING: return "string";
        case VAL_FN:     return "fn";
        case VAL_TABLE:  return "table";
        case VAL_BUFFER: return "buffer";
        case VAL_REF:    return "ref";
    }
    return "unknown";
}

/* ─── Truthiness ─── */

int gem_truthy(GemVal v) {
    if (v.type == VAL_NIL) return 0;
    if (v.type == VAL_BOOL) return v.bval;
    return 1;
}

