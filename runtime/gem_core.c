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

static uint32_t gem_shape_counter = 1;

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

/* ─── Deep copy (arena-based, for message passing across processes) ─── */

#define GEM_COPY_MAP_INLINE 16

typedef struct {
    void *old_ptr;
    void *new_ptr;
} GemCopyEntry;

typedef struct {
    void *key;
    void *value;
} GemCopyHashEntry;

typedef struct {
    GemCopyEntry inline_buf[GEM_COPY_MAP_INLINE];
    GemCopyEntry *entries;
    GemCopyHashEntry *hash_map;
    int len;
    int cap;
    int use_malloc;
    /* When `preserve_external` is set, gem_deep_copy_internal /
       gem_deep_copy_fn leave alone any heap pointer that lies *outside* the
       old arena's mmap blocks. Used by gem_arena_reset_with_roots so a
       closure env that captured a BSS-backed top-level box (or a malloc'd
       pinned box) keeps pointing at that slot instead of being rebound to a
       fresh arena box. The arena's lo/hi span is *not* a contiguous range —
       blocks live at distinct mmap addresses with potential gaps — so we
       walk the block list per pointer lookup (not by range comparison) for soundness. */
    int preserve_external;
    GemArena *old_arena;
} GemCopyMap;

static void gem_copy_map_init(GemCopyMap *map, int use_malloc) {
    map->entries = map->inline_buf;
    map->hash_map = NULL;
    map->len = 0;
    map->cap = GEM_COPY_MAP_INLINE;
    map->use_malloc = use_malloc;
    map->preserve_external = 0;
    map->old_arena = NULL;
}

static int gem_ptr_in_arena(GemArena *arena, const void *ptr) {
    if (!arena) return 0;
    const char *p = (const char *)ptr;
    for (GemArenaBlock *b = arena->head; b; b = b->next) {
        if (p >= b->data && p < b->data + b->cap) return 1;
    }
    return 0;
}

static int gem_copy_is_external(GemCopyMap *map, const void *ptr) {
    if (!map->preserve_external) return 0;
    return !gem_ptr_in_arena(map->old_arena, ptr);
}

static void gem_copy_map_cleanup(GemCopyMap *map) {
    if (map->entries != map->inline_buf) free(map->entries);
    if (map->hash_map) hmfree(map->hash_map);
}

static void *gem_copy_map_find(GemCopyMap *map, void *old) {
    if (map->hash_map) {
        ptrdiff_t idx = hmgeti(map->hash_map, old);
        if (idx >= 0) return map->hash_map[idx].value;
        return NULL;
    }
    for (int i = 0; i < map->len; i++) {
        if (map->entries[i].old_ptr == old) return map->entries[i].new_ptr;
    }
    return NULL;
}

static void gem_copy_map_add(GemCopyMap *map, void *old, void *new_ptr) {
    if (map->len >= map->cap) {
        int new_cap = map->cap * 2;
        GemCopyEntry *new_entries = (GemCopyEntry *)malloc(sizeof(GemCopyEntry) * new_cap);
        memcpy(new_entries, map->entries, sizeof(GemCopyEntry) * map->len);
        if (map->entries != map->inline_buf) free(map->entries);
        map->entries = new_entries;
        map->cap = new_cap;
    }
    map->entries[map->len].old_ptr = old;
    map->entries[map->len].new_ptr = new_ptr;
    map->len++;
    if (map->len > GEM_COPY_MAP_INLINE && !map->hash_map) {
        for (int i = 0; i < map->len; i++)
            hmput(map->hash_map, map->entries[i].old_ptr, map->entries[i].new_ptr);
    } else if (map->hash_map) {
        hmput(map->hash_map, old, new_ptr);
    }
}

static GemVal gem_deep_copy_internal(GemVal val, GemCopyMap *map);

static void *gem_copy_alloc(GemCopyMap *map, size_t size) {
    if (map->use_malloc) return calloc(1, size);
    return gem_arena_alloc(gem_current_arena(), size);
}

static char *gem_copy_strdup(GemCopyMap *map, const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = (char *)gem_copy_alloc(map, len);
    memcpy(copy, s, len);
    return copy;
}

static GemVal gem_deep_copy_table(GemTable *t, GemCopyMap *map) {
    void *existing = gem_copy_map_find(map, t);
    if (existing) {
        GemVal r; r.type = VAL_TABLE; r.magic = GEM_MAGIC; r.table = (GemTable *)existing; return r;
    }

    GemTable *nt = (GemTable *)gem_copy_alloc(map, sizeof(GemTable));
    gem_copy_map_add(map, t, nt);

    nt->len = t->len;
    nt->cap = t->cap;
    nt->keys = (GemVal *)gem_copy_alloc(map, sizeof(GemVal) * t->cap);
    nt->vals = (GemVal *)gem_copy_alloc(map, sizeof(GemVal) * t->cap);
    nt->str_index = NULL;
    nt->shape_id = gem_shape_counter++;
    nt->arena_next = NULL;

    if (!map->use_malloc) {
        GemArena *a = gem_current_arena();
        nt->arena_next = a->table_list;
        a->table_list = nt;
    }

    for (int i = 0; i < t->len; i++) {
        nt->keys[i] = gem_deep_copy_internal(t->keys[i], map);
        nt->vals[i] = gem_deep_copy_internal(t->vals[i], map);
        if (nt->keys[i].type == VAL_STRING) {
            shput(nt->str_index, nt->keys[i].sval, i);
        }
    }

    GemVal r; r.type = VAL_TABLE; r.magic = GEM_MAGIC; r.table = nt; return r;
}

static GemVal gem_deep_copy_fn(GemVal fn_val, GemCopyMap *map) {
    if (!fn_val.env) return fn_val;

    void *existing = gem_copy_map_find(map, fn_val.env);
    if (existing) {
        return (GemVal){.type = VAL_FN, .magic = GEM_MAGIC, .fn = fn_val.fn, .env = existing};
    }

    intptr_t n = *(intptr_t *)fn_val.env;
    GemVal **old = (GemVal **)((char *)fn_val.env + sizeof(intptr_t));
    size_t env_size = sizeof(intptr_t) + sizeof(GemVal *) * n;
    void *new_env = gem_copy_alloc(map, env_size);
    gem_copy_map_add(map, fn_val.env, new_env);
    *(intptr_t *)new_env = n;
    GemVal **new_fields = (GemVal **)((char *)new_env + sizeof(intptr_t));
    for (intptr_t i = 0; i < n; i++) {
        /* Preserve box pointers that live outside the old arena. Two flavors:
             - BSS-backed top-level boxes: their contents are migrated via
               the rescue-roots list (the runtime caller passes the BSS slot
               pointer); not in any pin-set, so the mark call below is a
               no-op and we just preserve the pointer.
             - malloc'd pinned boxes (gem_box_alloc, fn-local mutated-
               captured): registered in the current process's pin-set.
               If this is the first env field reaching the box this cycle,
               mark+recurse migrates its contents. Subsequent encounters
               (via other capturing closures) hit the "already walked"
               short-circuit and just preserve the pointer. */
        if (gem_copy_is_external(map, old[i])) {
            new_fields[i] = old[i];
            if (gem_current_pid >= 0) {
                GemProcess *proc = &gem_proc_table[gem_current_pid];
                if (gem_pin_mark_walked(proc, old[i])) {
                    *old[i] = gem_deep_copy_internal(*old[i], map);
                }
            }
            continue;
        }
        GemVal *existing = (GemVal *)gem_copy_map_find(map, old[i]);
        if (existing) {
            new_fields[i] = existing;
            continue;
        }
        GemVal *box = (GemVal *)gem_copy_alloc(map, sizeof(GemVal));
        gem_copy_map_add(map, old[i], box);
        *box = gem_deep_copy_internal(*old[i], map);
        new_fields[i] = box;
    }
    return (GemVal){.type = VAL_FN, .magic = GEM_MAGIC, .fn = fn_val.fn, .env = new_env};
}

static GemVal gem_deep_copy_internal(GemVal val, GemCopyMap *map) {
    switch (val.type) {
        case VAL_NIL:
        case VAL_BOOL:
        case VAL_INT:
        case VAL_FLOAT:
        case VAL_REF:
            return val;
        case VAL_STRING: {
            if (gem_copy_is_external(map, val.sval)) return val;
            void *existing = gem_copy_map_find(map, val.sval);
            if (existing) { GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = (char *)existing; return r; }
            GemVal r;
            r.type = VAL_STRING;
            r.magic = GEM_MAGIC;
            r.sval = gem_copy_strdup(map, val.sval);
            gem_copy_map_add(map, val.sval, r.sval);
            return r;
        }
        case VAL_TABLE:
            /* Immutable tables are normally shared without copying — safe for
               spawn/send (source arena outlives the receiver). Not safe during
               arena reset: preserve_external means the source arena is about
               to be destroyed, so we must migrate the table unless it lives
               outside the arena bounds. */
            if (gem_copy_is_external(map, val.table)) return val;
            if (!map->use_malloc && !map->preserve_external && val.table->immutable) return val;
            return gem_deep_copy_table(val.table, map);
        case VAL_BUFFER: {
            void *bex = gem_copy_map_find(map, val.buffer);
            if (bex) { GemVal r; r.type = VAL_BUFFER; r.magic = GEM_MAGIC; r.buffer = (GemBuffer *)bex; return r; }
            GemBuffer *ob = val.buffer;
            GemBuffer *nb = (GemBuffer *)gem_copy_alloc(map, sizeof(GemBuffer));
            gem_copy_map_add(map, ob, nb);
            nb->len = ob->len;
            nb->cap = ob->cap;
            nb->data = (char *)gem_copy_alloc(map, ob->cap);
            memcpy(nb->data, ob->data, ob->len);
            GemVal r; r.type = VAL_BUFFER; r.magic = GEM_MAGIC; r.buffer = nb; return r;
        }
        case VAL_FN:
            return gem_deep_copy_fn(val, map);
    }
    return val;
}

GemVal gem_deep_copy(GemVal val) {
    GemCopyMap map;
    gem_copy_map_init(&map, 0);
    GemVal result = gem_deep_copy_internal(val, &map);
    gem_copy_map_cleanup(&map);
    return result;
}

GemVal gem_deep_copy_malloc(GemVal val) {
    GemCopyMap map;
    gem_copy_map_init(&map, 1);
    GemVal result = gem_deep_copy_internal(val, &map);
    gem_copy_map_cleanup(&map);
    return result;
}

/* Track already-freed allocations so aliased structure (the same table reached
   by two different roots) is freed only once. The visited set stores raw
   pointers; types are inferred from the GemVal walk. NULL set means no aliasing
   is possible — used by the public single-value entry point. */
static int gem_freed_set_contains(GemCopyMap *visited, void *ptr) {
    if (!visited) return 0;
    return gem_copy_map_find(visited, ptr) != NULL;
}

static void gem_freed_set_add(GemCopyMap *visited, void *ptr) {
    if (!visited) return;
    /* Sentinel non-NULL value — gem_copy_map_find returns the value, we just
       care about presence. */
    gem_copy_map_add(visited, ptr, (void *)1);
}

static void gem_deep_free_internal(GemVal val, GemCopyMap *visited) {
    switch (val.type) {
        case VAL_STRING:
            if (gem_freed_set_contains(visited, val.sval)) break;
            gem_freed_set_add(visited, val.sval);
            free(val.sval);
            break;
        case VAL_TABLE: {
            GemTable *t = val.table;
            if (gem_freed_set_contains(visited, t)) break;
            gem_freed_set_add(visited, t);
            for (int i = 0; i < t->len; i++) {
                gem_deep_free_internal(t->keys[i], visited);
                gem_deep_free_internal(t->vals[i], visited);
            }
            if (t->str_index) shfree(t->str_index);
            free(t->keys);
            free(t->vals);
            free(t);
            break;
        }
        case VAL_BUFFER: {
            GemBuffer *b = val.buffer;
            if (gem_freed_set_contains(visited, b)) break;
            gem_freed_set_add(visited, b);
            free(b->data);
            free(b);
            break;
        }
        case VAL_FN: {
            if (!val.env) break;
            if (gem_freed_set_contains(visited, val.env)) break;
            gem_freed_set_add(visited, val.env);
            intptr_t n = *(intptr_t *)val.env;
            GemVal **fields = (GemVal **)((char *)val.env + sizeof(intptr_t));
            for (intptr_t i = 0; i < n; i++) {
                gem_deep_free_internal(*fields[i], visited);
                free(fields[i]);
            }
            free(val.env);
            break;
        }
        default:
            break;
    }
}

void gem_deep_free(GemVal val) {
    gem_deep_free_internal(val, NULL);
}

/* ─── Pinned-box set ─── */

/* Boxes for fn-local mutated-captured vars are allocated via gem_box_alloc
   (plain malloc) so they survive arena reset. Each is registered in the
   current process's pin-set. At every reset:
     1. Pre-pass: codegen passes the box pointers as `pinned_roots`. We
        mark each as "walked" and deep-copy its contents into the fresh
        arena. This handles boxes that are live via the function's local
        even when no capturing closure is alive.
     2. During the regular roots / mailbox walk, gem_deep_copy_fn's
        external branch encounters env fields pointing at pinned boxes.
        gem_pin_mark_walked dedups: if not yet walked, walk the contents
        now; if already walked, just preserve the pointer.
     3. Sweep: any pin-set entry not marked is unreachable — free it.
        Surviving entries are reset to "untouched" for the next cycle.
   On process exit, gem_pin_free_all releases everything in one shot. */

GemVal *gem_box_alloc(void) {
    GemVal *p = (GemVal *)calloc(1, sizeof(GemVal));
    if (gem_current_pid >= 0) {
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        GemPinEntry e = { .key = p, .value = 0 };
        hmputs(proc->pinned_boxes, e);
    }
    return p;
}

int gem_pin_mark_walked(GemProcess *proc, void *p) {
    if (!proc || !proc->pinned_boxes) return 0;
    ptrdiff_t idx = hmgeti(proc->pinned_boxes, p);
    if (idx < 0) return 0;
    if (proc->pinned_boxes[idx].value) return 0;
    proc->pinned_boxes[idx].value = 1;
    return 1;
}

void gem_pin_sweep(GemProcess *proc) {
    if (!proc || !proc->pinned_boxes) return;
    /* Build the free-list first; hmdel rebalances and would invalidate
       indices mid-iteration. */
    void **to_free = NULL;
    size_t n = hmlenu(proc->pinned_boxes);
    for (size_t i = 0; i < n; i++) {
        if (proc->pinned_boxes[i].value == 0) {
            arrpush(to_free, proc->pinned_boxes[i].key);
        } else {
            proc->pinned_boxes[i].value = 0;
        }
    }
    for (size_t i = 0; i < arrlenu(to_free); i++) {
        hmdel(proc->pinned_boxes, to_free[i]);
        free(to_free[i]);
    }
    arrfree(to_free);
}

void gem_pin_free_all(GemProcess *proc) {
    if (!proc || !proc->pinned_boxes) return;
    size_t n = hmlenu(proc->pinned_boxes);
    for (size_t i = 0; i < n; i++) {
        free(proc->pinned_boxes[i].key);
    }
    hmfree(proc->pinned_boxes);
    proc->pinned_boxes = NULL;
}

/* Reset the current process's arena, deep-copying `roots` and pinned-box `pinned_roots` into a fresh arena. See gem_arena.c file comment for the mmap/munmap rationale. */
void gem_arena_reset_with_roots_pinned(GemVal **roots, int n_roots,
                                       GemVal **pinned_roots, int n_pinned) {
    GemArena *arena = gem_current_arena();
    GemProcess *proc = NULL;
    if (gem_current_pid >= 0) {
        proc = &gem_proc_table[gem_current_pid];
        /* Skip while inside a pcall — its stack_snapshot may reference the arena. */
        if (proc->pcall_depth > 0) return;
    }
    /* Save old arena state and install a fresh arena in its place. The old
       blocks remain mmapped and readable until we destroy them at the end —
       deep_copy reads from them while the fresh arena receives the copies. */
    GemArena old_arena = *arena;
    gem_arena_init(arena);

    GemMailbox old_mailbox = {NULL, NULL};
    if (proc) {
        old_mailbox = proc->mailbox;
        proc->mailbox.head = NULL;
        proc->mailbox.tail = NULL;
        proc->read_buf = NULL;
        proc->read_buf_cap = 0;
    }

    GemCopyMap to_arena;
    gem_copy_map_init(&to_arena, 0);
    to_arena.preserve_external = 1;
    to_arena.old_arena = &old_arena;

    /* Pinned-root pre-pass: walk-and-mark each pinned box. This migrates
       contents into the fresh arena and seeds the copy_map so a later
       env-walk encountering the same internal pointers reuses the
       migrated copies instead of duplicating them. */
    for (int i = 0; i < n_pinned; i++) {
        GemVal *box = pinned_roots[i];
        if (gem_pin_mark_walked(proc, box)) {
            *box = gem_deep_copy_internal(*box, &to_arena);
        }
    }

    for (int i = 0; i < n_roots; i++) {
        *roots[i] = gem_deep_copy_internal(*roots[i], &to_arena);
    }

    if (proc) {
        for (GemMsgNode *n = old_mailbox.head; n; n = n->next) {
            GemVal v = gem_deep_copy_internal(n->value, &to_arena);
            GemMsgNode *node = ALLOC(GemMsgNode);
            node->value = v;
            node->next = NULL;
            if (proc->mailbox.tail) {
                proc->mailbox.tail->next = node;
            } else {
                proc->mailbox.head = node;
            }
            proc->mailbox.tail = node;
        }
    }
    gem_copy_map_cleanup(&to_arena);

    /* Sweep pin-set: any pinned box not marked this cycle is unreachable.
       Surviving entries are reset to untouched. Order is independent of
       arena destruction since pinned boxes are malloc-backed. */
    if (proc) gem_pin_sweep(proc);

    /* munmap the old blocks now that nothing references them. */
    gem_arena_destroy(&old_arena);
}

void gem_arena_reset_with_roots(GemVal **roots, int n_roots) {
    gem_arena_reset_with_roots_pinned(roots, n_roots, NULL, 0);
}
