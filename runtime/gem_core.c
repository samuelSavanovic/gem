/*
 * gem_core.c -- GemVal constructors, table operations, equality, truthiness.
 */

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

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
    /* Force stdout line-buffered (vs. block-buffered when redirected to a
     * pipe/file) so long-running processes don't lose output on SIGKILL. */
    setvbuf(stdout, NULL, _IOLBF, 0);

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
        gem_char_cache[i].slen = 1;
    }
    gem_char_cache_ready = 1;
}

/* ─── Constructors ─── */


int gem_format_float(double v, char *out) {
    if (isnan(v)) { memcpy(out, "nan", 4); return 3; }
    if (isinf(v)) {
        if (v < 0) { memcpy(out, "-inf", 5); return 4; }
        memcpy(out, "inf", 4); return 3;
    }
    /* Shortest of 15/16/17 significant digits that round-trips (any
     * shorter decimal that round-trips is a prefix of the 15-digit form,
     * so stripping its trailing zeros below finds it). A subnormal has
     * fewer than 15 significant digits of precision: try from 1. */
    char tmp[GEM_FLOAT_BUF];
    int start = (v != 0.0 && fabs(v) < DBL_MIN) ? 1 : 15;
    for (int prec = start; prec <= 17; prec++) {
        snprintf(tmp, sizeof(tmp), "%.*e", prec - 1, v);
        if (prec == 17 || strtod(tmp, NULL) == v) break;
    }
    /* tmp is [-]d.ddddde[+-]xx: split into sign, digits, exponent. */
    const char *p = tmp;
    int neg = 0;
    if (*p == '-') { neg = 1; p++; }
    char digits[24];
    int nd = 0;
    while (*p && *p != 'e') {
        if (*p != '.') digits[nd++] = *p;
        p++;
    }
    int exp10 = atoi(p + 1);
    while (nd > 1 && digits[nd - 1] == '0') nd--;
    int n = 0;
    if (neg) out[n++] = '-';
    if (exp10 >= -4 && exp10 < 16) {
        if (exp10 >= 0) {
            for (int i = 0; i <= exp10; i++) out[n++] = i < nd ? digits[i] : '0';
            out[n++] = '.';
            if (nd > exp10 + 1) {
                for (int i = exp10 + 1; i < nd; i++) out[n++] = digits[i];
            } else {
                out[n++] = '0';
            }
        } else {
            out[n++] = '0';
            out[n++] = '.';
            for (int i = 0; i < -exp10 - 1; i++) out[n++] = '0';
            for (int i = 0; i < nd; i++) out[n++] = digits[i];
        }
        out[n] = '\0';
    } else {
        out[n++] = digits[0];
        if (nd > 1) {
            out[n++] = '.';
            for (int i = 1; i < nd; i++) out[n++] = digits[i];
        }
        n += snprintf(out + n, GEM_FLOAT_BUF - n, "e%c%02d", exp10 < 0 ? '-' : '+', exp10 < 0 ? -exp10 : exp10);
    }
    return n;
}

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
    size_t len = strlen(s);
    r.slen = gem_strlen_check(len, NULL);
    r.sval = (char *)gem_alloc(len + 1);
    memcpy(r.sval, s, len + 1);
    return r;
}

GemVal gem_string_with_len(const char *s, int64_t len) {
    GemVal r;
    r.type = VAL_STRING;
    r.magic = GEM_MAGIC;
    if (len < 0) len = 0;
    r.slen = gem_strlen_check((size_t)len, NULL);
    r.sval = (char *)gem_alloc((size_t)len + 1);
    if (len > 0) memcpy(r.sval, s, (size_t)len);
    r.sval[len] = '\0';
    return r;
}

/* ─── String key index ─── */

static uint64_t gem_str_hash(const char *key, int64_t len) {
    /* FNV-1a, then a final mix so the low bits used for the slot spread. */
    uint64_t h = 1469598103934665603ULL;
    for (int64_t i = 0; i < len; i++) {
        h ^= (unsigned char)key[i];
        h *= 1099511628211ULL;
    }
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    return h;
}

/* The slot holding `key`, or -1. */
static int gem_str_index_find(const GemStrIndex *ix, const char *key, int64_t len, uint64_t h) {
    if (ix->cap == 0) return -1;
    int mask = ix->cap - 1;
    for (int i = (int)(h & (uint64_t)mask);; i = (i + 1) & mask) {
        const GemStrSlot *sl = &ix->slots[i];
        if (sl->key == NULL) return -1;
        if (sl->value >= 0 && sl->hash == h && sl->len == len &&
            (sl->key == key || memcmp(sl->key, key, (size_t)len) == 0))
            return i;
    }
}

int gem_str_index_get(const GemStrIndex *ix, const char *key, int64_t len) {
    if (ix == NULL) return -1;
    int i = gem_str_index_find(ix, key, len, gem_str_hash(key, len));
    return i < 0 ? -1 : ix->slots[i].value;
}

static GemStrIndex *gem_str_index_alloc(int cap) {
    GemStrIndex *ix = (GemStrIndex *)calloc(1, sizeof(GemStrIndex) + (size_t)cap * sizeof(GemStrSlot));
    if (ix == NULL) { fprintf(stderr, "gem: out of memory (string key index)\n"); exit(1); }
    ix->cap = cap;
    return ix;
}

static void gem_str_index_insert_new(GemStrIndex *ix, const char *key, int64_t len, uint64_t h, int pos) {
    int mask = ix->cap - 1;
    int i = (int)(h & (uint64_t)mask);
    while (ix->slots[i].key != NULL) i = (i + 1) & mask;
    ix->slots[i] = (GemStrSlot){key, len, h, pos};
    ix->used++;
}

void gem_str_index_put(GemStrIndex **ixp, const char *key, int64_t len, int pos) {
    GemStrIndex *ix = *ixp;
    uint64_t h = gem_str_hash(key, len);
    if (ix != NULL) {
        int i = gem_str_index_find(ix, key, len, h);
        if (i >= 0) {
            ix->slots[i].key = key;
            ix->slots[i].value = pos;
            return;
        }
    }
    /* Keep the load (deleted slots included) at most 3/4; rehash drops the
       deleted ones. */
    if (ix == NULL || (ix->used + 1) * 4 > ix->cap * 3) {
        int live = 0;
        if (ix) for (int i = 0; i < ix->cap; i++) if (ix->slots[i].key && ix->slots[i].value >= 0) live++;
        int cap = 8;
        while ((live + 1) * 2 > cap) cap *= 2;
        GemStrIndex *nix = gem_str_index_alloc(cap);
        if (ix) {
            nix->keyix = ix->keyix;
            for (int i = 0; i < ix->cap; i++) {
                GemStrSlot *sl = &ix->slots[i];
                if (sl->key && sl->value >= 0) gem_str_index_insert_new(nix, sl->key, sl->len, sl->hash, sl->value);
            }
            free(ix);
        }
        ix = nix;
        *ixp = ix;
    }
    gem_str_index_insert_new(ix, key, len, h, pos);
}

void gem_str_index_del(GemStrIndex *ix, const char *key, int64_t len) {
    if (ix == NULL) return;
    int i = gem_str_index_find(ix, key, len, gem_str_hash(key, len));
    if (i >= 0) ix->slots[i].value = -1;   /* a tombstone keeps probe chains intact */
}

void gem_str_index_free(GemStrIndex **ixp) {
    if (*ixp) free((*ixp)->keyix);
    free(*ixp);
    *ixp = NULL;
}

/* ─── Value key index (see GemKeyIndex in gem.h) ─── */

typedef struct {
    uint64_t hash;
    int pos1;          /* position + 1; 0: empty slot, -1: deleted */
} GemKeySlot;

struct GemKeyIndex {
    int cap;           /* power of 2 */
    int used;          /* live + deleted slots */
    GemKeySlot slots[];
};

static int gem_key_indexable(GemVal k) {
    return k.type == VAL_INT || k.type == VAL_FLOAT || k.type == VAL_BOOL || k.type == VAL_REF ||
           k.type == VAL_RESOURCE;
}

static uint64_t gem_key_hash(GemVal k) {
    uint64_t x;
    switch (k.type) {
        case VAL_INT: x = (uint64_t)k.ival; break;
        case VAL_BOOL: x = (uint64_t)k.bval; break;
        case VAL_REF: x = (uint64_t)k.rval; break;
        case VAL_RESOURCE: x = (uint64_t)k.res_id; break;
        default: {
            double d = k.fval == 0.0 ? 0.0 : k.fval;   /* -0.0 == 0.0 */
            memcpy(&x, &d, sizeof(x));
        }
    }
    x ^= (uint64_t)k.type * 0x9E3779B97F4A7C15ULL;
    x ^= x >> 33; x *= 0xFF51AFD7ED558CCDULL;
    x ^= x >> 33; x *= 0xC4CEB9FE1A85EC53ULL;
    x ^= x >> 33;
    return x;
}

static GemKeyIndex *gem_key_index_of(const GemTable *t) {
    return t->str_index ? t->str_index->keyix : NULL;
}

/* The slot of `ix` holding `key`, or -1. */
static int gem_key_index_find(const GemTable *t, const GemKeyIndex *ix, GemVal key, uint64_t h) {
    int mask = ix->cap - 1;
    int i = (int)(h & (uint64_t)mask);
    while (ix->slots[i].pos1 != 0) {
        int p = ix->slots[i].pos1 - 1;
        if (p >= 0 && ix->slots[i].hash == h && gem_val_eq(t->keys[p], key)) return i;
        i = (i + 1) & mask;
    }
    return -1;
}

static void gem_key_index_insert_new(GemKeyIndex *ix, uint64_t h, int pos) {
    int mask = ix->cap - 1;
    int i = (int)(h & (uint64_t)mask);
    while (ix->slots[i].pos1 != 0) i = (i + 1) & mask;
    ix->slots[i].hash = h;
    ix->slots[i].pos1 = pos + 1;
    ix->used++;
}

/* A fresh index with room for `live` + 1 keys at a load of at most 1/2,
   holding the live slots of `old` (freed). */
static GemKeyIndex *gem_key_index_resize(GemKeyIndex *old, int live) {
    int cap = 16;
    while ((live + 1) * 2 > cap) cap *= 2;
    GemKeyIndex *ix = (GemKeyIndex *)calloc(1, sizeof(GemKeyIndex) + (size_t)cap * sizeof(GemKeySlot));
    if (ix == NULL) { fprintf(stderr, "gem: out of memory (table key index)\n"); exit(1); }
    ix->cap = cap;
    if (old) {
        for (int i = 0; i < old->cap; i++)
            if (old->slots[i].pos1 > 0) gem_key_index_insert_new(ix, old->slots[i].hash, old->slots[i].pos1 - 1);
        free(old);
    }
    return ix;
}

void gem_key_index_put(GemTable *t, GemVal key, int pos) {
    GemKeyIndex *ix = gem_key_index_of(t);
    if (ix == NULL || !gem_key_indexable(key)) return;
    uint64_t h = gem_key_hash(key);
    int i = gem_key_index_find(t, ix, key, h);
    if (i >= 0) { ix->slots[i].pos1 = pos + 1; return; }
    /* Keep the load (deleted slots included) at most 3/4; a resize drops
       the deleted ones. */
    if ((ix->used + 1) * 4 > ix->cap * 3) {
        int live = 0;
        for (int j = 0; j < ix->cap; j++) live += (ix->slots[j].pos1 > 0);
        ix = t->str_index->keyix = gem_key_index_resize(ix, live);
    }
    gem_key_index_insert_new(ix, h, pos);
}

void gem_key_index_del(GemTable *t, GemVal key) {
    GemKeyIndex *ix = gem_key_index_of(t);
    if (ix == NULL || !gem_key_indexable(key)) return;
    int i = gem_key_index_find(t, ix, key, gem_key_hash(key));
    if (i >= 0) ix->slots[i].pos1 = -1;   /* a tombstone keeps probe chains intact */
}

int gem_table_key_pos(GemTable *t, GemVal key) {
    if (gem_key_indexable(key) && t->len > GEM_TABLE_SCAN_MAX) {
        gem_table_index(t);
        GemKeyIndex *ix = gem_key_index_of(t);
        if (ix == NULL) {
            /* A table without string keys has no string index: one of
               capacity 0 holds the key index. */
            if (t->str_index == NULL) t->str_index = gem_str_index_alloc(0);
            ix = t->str_index->keyix = gem_key_index_resize(NULL, t->len);
            for (int i = 0; i < t->len; i++)
                if (gem_key_indexable(t->keys[i])) gem_key_index_insert_new(ix, gem_key_hash(t->keys[i]), i);
        }
        int i = gem_key_index_find(t, ix, key, gem_key_hash(key));
        return i < 0 ? -1 : ix->slots[i].pos1 - 1;
    }
    for (int i = 0; i < t->len; i++)
        if (gem_val_eq(t->keys[i], key)) return i;
    return -1;
}

/* ─── Table operations ─── */

void gem_table_rebuild_index(GemTable *t) {
    t->index_stale = 0;
    gem_str_index_free(&t->str_index);
    int n = 0;
    for (int i = 0; i < t->len; i++) n += (t->keys[i].type == VAL_STRING);
    t->nstr = n;
    if (n == 0 || t->len <= GEM_TABLE_SCAN_MAX) return;
    for (int i = 0; i < t->len; i++) {
        if (t->keys[i].type == VAL_STRING)
            gem_str_index_put(&t->str_index, t->keys[i].sval, t->keys[i].slen, i);
    }
}

int gem_table_str_pos(GemTable *t, const char *key, int64_t len) {
    gem_table_index(t);
    if (t->str_index) return gem_str_index_get(t->str_index, key, len);
    if (t->nstr == 0) return -1;
    for (int i = 0; i < t->len; i++) {
        const GemVal *k = &t->keys[i];
        if (k->type == VAL_STRING && k->slen == len &&
            (k->sval == key || memcmp(k->sval, key, (size_t)len) == 0))
            return i;
    }
    return -1;
}

/* After an append: a table past GEM_TABLE_SCAN_MAX entries with a string key
   in it gets its index. */
static inline void gem_table_appended(GemTable *t) {
    if (t->str_index == NULL && t->nstr > 0 && t->len > GEM_TABLE_SCAN_MAX)
        gem_table_rebuild_index(t);
}

void gem_table_grow(GemTable *t) {
    int new_cap = t->cap * 2;
    GemVal *new_keys = ALLOC_N(GemVal, new_cap);
    GemVal *new_vals = ALLOC_N(GemVal, new_cap);
    memcpy(new_keys, t->keys, sizeof(GemVal) * t->len);
    memcpy(new_vals, t->vals, sizeof(GemVal) * t->len);
    t->keys = new_keys;
    t->vals = new_vals;
    t->cap = new_cap;
    gem_table_written(t);
}

/* Shared with gem_copy.c (gem_deep_copy_table stamps fresh shape ids). */
uint32_t gem_shape_counter = 1;

uint64_t gem_mut_clock = 1;

static inline GemVal gem_table_alloc(int braces) {
    GemTable *t = ALLOC(GemTable);
    t->len = 0;
    t->cap = 4;
    t->keys = ALLOC_N(GemVal, 4);
    t->vals = ALLOC_N(GemVal, 4);
    t->str_index = NULL;
    t->nstr = 0;
    t->shape_id = gem_shape_counter++;
    t->braces = braces;
    t->is_array = 1;

    GemArena *a = gem_current_arena();
    t->arena_next = a->table_list;
    a->table_list = t;
    t->mut_seq = gem_mut_clock;  /* new in this epoch: nothing older can need it logged */

    GemVal r; r.type = VAL_TABLE; r.magic = GEM_MAGIC; r.table = t; return r;
}

GemVal gem_table_new(void) {
    return gem_table_alloc(0);
}

GemVal gem_table_new_braces(void) {
    return gem_table_alloc(1);
}

void gem_table_freeze(GemVal tbl) {
    if (tbl.type == VAL_TABLE) tbl.table->immutable = 1;
}

GemVal gem_table_freeze_static(GemVal tbl) {
    gem_table_freeze(tbl);
    return tbl;
}

void gem_ns_refresh(GemVal ns, const char *field, GemVal val) {
    if (ns.type != VAL_TABLE || !ns.table->immutable) return;
    GemTable *t = ns.table;
    for (int i = 0; i < t->len; i++) {
        if (t->keys[i].type == VAL_STRING && strcmp(t->keys[i].sval, field) == 0) {
            if (t->snap_gen) {           /* gem_table_check_mutable minus the freeze */
                t->immutable = 0;
                gem_table_mutate_slow(t);
                t->immutable = 1;
            }
            gem_table_written(t);
            t->vals[i] = val;
            return;
        }
    }
}


void gem_table_set(GemVal tbl, GemVal key, GemVal val) {
    if (tbl.type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "index set on non-table: got %s", gem_type_str(tbl)); gem_error(buf); }
    GemTable *t = tbl.table;
    gem_table_check_mutable(t);
    gem_table_written(t);

    if (key.type == VAL_STRING) {
        int found = gem_table_str_pos(t, key.sval, key.slen);
        if (found >= 0) {
            t->vals[found] = val;
            return;
        }
        /* Not found — append */
        if (t->len >= t->cap) gem_table_grow(t);
        int pos = t->len;
        t->keys[pos] = key;
        t->vals[pos] = val;
        t->len++;
        t->nstr++;
        t->is_array = 0;
        if (t->str_index) gem_str_index_put(&t->str_index, key.sval, key.slen, pos);
        else gem_table_appended(t);
        return;
    }
    gem_table_index(t);

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
        /* An array has no key >= len: t[len] = v appends, anything past it
           is a new key too. */
        if (t->is_array && ik >= t->len) {
            if (t->len >= t->cap) gem_table_grow(t);
            t->keys[t->len] = key;
            t->vals[t->len] = val;
            t->is_array = (ik == t->len);
            gem_key_index_put(t, key, t->len);
            t->len++;
            return;
        }
    }

    /* A nil key reads as a missing one, and a NaN key never equals itself,
       so no lookup could reach either entry. */
    if (key.type == VAL_NIL) gem_error("table key is nil");
    if (key.type == VAL_FLOAT && key.fval != key.fval) gem_error("table key is NaN");

    /* Any other non-string key */
    int found = gem_table_key_pos(t, key);
    if (found >= 0) {
        t->vals[found] = val;
        return;
    }

    /* Append new entry */
    if (t->len >= t->cap) gem_table_grow(t);
    t->keys[t->len] = key;
    t->vals[t->len] = val;
    t->is_array = 0;
    gem_key_index_put(t, key, t->len);
    t->len++;
    gem_table_appended(t);
}

GemVal gem_table_get(GemVal tbl, GemVal key) {
    /* String indexing: tbl[i] on a string returns single char */
    if (tbl.type == VAL_STRING) {
        if (key.type != VAL_INT) { char buf[128]; snprintf(buf, sizeof(buf), "string index must be int, got %s", gem_type_str(key)); gem_error(buf); }
        int64_t idx = key.ival;
        int64_t slen = (int64_t)tbl.slen;
        if (idx < 0) idx = slen + idx;
        if (idx < 0 || idx >= slen) { gem_error("string index out of bounds"); }
        return gem_char_cache[(unsigned char)tbl.sval[idx]];
    }

    if (tbl.type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "index get on non-table: got %s", gem_type_str(tbl)); gem_error(buf); }
    GemTable *t = tbl.table;

    if (key.type == VAL_STRING) {
        int found = gem_table_str_pos(t, key.sval, key.slen);
        if (found >= 0) return t->vals[found];
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
        if (t->is_array) return (GemVal){VAL_NIL, GEM_MAGIC, {0}};
    }

    int found = gem_table_key_pos(t, key);
    if (found >= 0) return t->vals[found];
    return (GemVal){VAL_NIL, GEM_MAGIC, {0}};
}

/* ─── Inline cache miss path ─── */

GemVal gem_table_get_ic_miss(GemTable *t, const char *key, GemICacheSlot *cache) {
    {
        /* A field name from the source: no NUL inside. */
        int vi = gem_table_str_pos(t, key, (int64_t)strlen(key));
        if (vi >= 0) {
            cache->table = t;
            cache->shape_id = t->shape_id;
            cache->val_index = vi;
            return t->vals[vi];
        }
    }
    cache->table = NULL;
    return (GemVal){VAL_NIL, GEM_MAGIC, {0}};
}

/* ─── Table identity ─── */

int64_t gem_table_id(GemVal v) {
    return v.type == VAL_TABLE ? (int64_t)(intptr_t)v.table : 0;
}

int gem_table_braces(GemVal v) {
    return v.type == VAL_TABLE && v.table->braces;
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
        case VAL_RESOURCE: return gem_res_type_name(v.res_kind);
        case VAL_LAZY:   break;
    }
    return "unknown";
}


