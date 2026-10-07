/*
 * gem_copy.c -- Deep copy machinery, pin-set ops, and arena reset.
 *
 * Three closely related concerns live together because they share the same
 * GemCopyMap walker:
 *   - Deep copy (spawn / send / general value duplication).
 *   - Pin-set bookkeeping (malloc-backed boxes for fn-local mutated captures).
 *   - Per-process arena reset (the rescue+reset path used by long-running
 *     loops and TCO back-edges).
 *
 * The pin-set lives in GemProcess (declared in gem.h) but its lifecycle is
 * driven entirely by reset and process exit, so all three sit here.
 */

#include <stdlib.h>
#include <string.h>

#include "gem.h"
#include "stb_ds.h"

/* gem_shape_counter is defined in gem_core.c — used by gem_copy_shallow to
   stamp fresh shape ids on copied tables. */
extern uint32_t gem_shape_counter;

/* ─── Deep copy (arena-based, for message passing across processes) ─── */

#define GEM_COPY_MAP_INLINE 16

typedef struct {
    void *old_ptr;
    void *new_ptr;
} GemCopyEntry;


/* The memory a reset is about to free, as address ranges sorted by `lo`.
   Arena blocks are separate mmaps, so this is not one contiguous span. */
typedef struct {
    const char *lo;
    const char *hi;
} GemRange;

typedef struct {
    GemRange *ranges;
    int n;
} GemRegion;

static int gem_range_cmp(const void *a, const void *b) {
    const GemRange *x = (const GemRange *)a, *y = (const GemRange *)b;
    return (x->lo > y->lo) - (x->lo < y->lo);
}

static int gem_region_contains(const GemRegion *rg, const void *ptr) {
    const char *p = (const char *)ptr;
    int lo = 0, hi = rg->n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (p < rg->ranges[mid].lo) hi = mid - 1;
        else if (p >= rg->ranges[mid].hi) lo = mid + 1;
        else return 1;
    }
    return 0;
}

/* `first` (may be NULL) is a partial range: [first_lo, first_hi). */
static void gem_region_build(GemRegion *rg, const char *first_lo, const char *first_hi,
                             GemArenaBlock *blocks) {
    int n = 1;
    for (GemArenaBlock *b = blocks; b; b = b->next) n++;
    rg->ranges = (GemRange *)malloc(sizeof(GemRange) * (size_t)n);
    rg->n = 0;
    if (first_lo && first_hi > first_lo) {
        rg->ranges[rg->n].lo = first_lo;
        rg->ranges[rg->n].hi = first_hi;
        rg->n++;
    }
    for (GemArenaBlock *b = blocks; b; b = b->next) {
        rg->ranges[rg->n].lo = b->data;
        rg->ranges[rg->n].hi = b->data + b->cap;
        rg->n++;
    }
    qsort(rg->ranges, (size_t)rg->n, sizeof(GemRange), gem_range_cmp);
}

/* Old -> new pointer map: a linear inline buffer for small copies, then an
   open-addressing hash table (power-of-two capacity, linear probing). */
/* Deferred work of an iterative copy: fill the already-allocated copy
   `dst` of table / closure env `src`. Copies never recurse in C, so the
   depth of the data (a two-million-deep list, say) does not matter. */
typedef struct {
    void *src;
    void *dst;
    int is_env;
} GemCopyTask;

#define GEM_COPY_TASKS_INLINE 32
/* Shells are filled right away (bounded C recursion) up to this nesting
   depth; deeper ones go through the worklist. Keeps small messages off the
   worklist without letting data depth reach the C stack. */
#define GEM_COPY_INLINE_DEPTH 16

typedef struct {
    GemCopyEntry inline_buf[GEM_COPY_MAP_INLINE];
    GemCopyEntry *table;   /* NULL while len <= GEM_COPY_MAP_INLINE */
    GemCopyTask task_buf[GEM_COPY_TASKS_INLINE];
    GemCopyTask *tasks;    /* task_buf, or a malloc'd stack once it outgrows it */
    size_t ntasks, taskcap;
    int depth;             /* current inline fill nesting (< GEM_COPY_INLINE_DEPTH) */
    size_t tcap;
    int len;
    int use_malloc;
    /* When `preserve_external` is set (arena resets), only objects inside
       `region` -- the memory about to be freed -- are copied; any pointer
       outside it (older arena memory, malloc'd pinned boxes) is kept as is, so its identity is preserved. */
    int preserve_external;
    const GemRegion *region;
    /* Process whose values are being copied (-1: none, e.g. a timer's
       malloc'd message). Its pin-set tells which capture boxes are pinned
       (see gem_copy_box_is_pinned). */
    int src_pid;
    /* Building a module snapshot unit (gem_mod_build_units): stamp every
       source table with the unit's gen, and note whether the copy holds
       anything that can change without a table write (buffers, pinned
       capture boxes of `snap_src`), which makes the unit single-use. */
    uint32_t stamp_gen;
    GemProcess *snap_src;
    int saw_mutable;
    /* Copying out of a snapshot unit into the process that references it:
       strings are immutable and the unit outlives the process's use of
       them, so they are shared, not copied. */
    int share_strings;
    /* Copying a spawned closure's env (gem_spawn_module_state): collect the
       snapshot gens of the source tables it reaches, so the module slots
       sharing them are copied with this same map. */
    int probe;
    uint32_t *probe_gens;
    int probe_n, probe_cap;
} GemCopyMap;

static void gem_copy_map_init(GemCopyMap *map, int use_malloc) {
    map->table = NULL;
    map->tcap = 0;
    map->len = 0;
    map->tasks = map->task_buf;
    map->ntasks = 0;
    map->taskcap = GEM_COPY_TASKS_INLINE;
    map->depth = 0;
    map->use_malloc = use_malloc;
    map->preserve_external = 0;
    map->region = NULL;
    map->src_pid = -1;
    map->stamp_gen = 0;
    map->snap_src = NULL;
    map->saw_mutable = 0;
    map->share_strings = 0;
    map->probe = 0;
    map->probe_gens = NULL;
    map->probe_n = map->probe_cap = 0;
}

static int gem_copy_is_external(GemCopyMap *map, const void *ptr) {
    if (!map->preserve_external) return 0;
    return !gem_region_contains(map->region, ptr);
}

static void gem_copy_map_cleanup(GemCopyMap *map) {
    free(map->probe_gens);
    free(map->table);
    if (map->tasks != map->task_buf) free(map->tasks);
}

static void gem_copy_push_task(GemCopyMap *map, void *src, void *dst, int is_env) {
    if (map->ntasks == map->taskcap) {
        size_t ncap = map->taskcap * 2;
        GemCopyTask *nt = (GemCopyTask *)malloc(sizeof(GemCopyTask) * ncap);
        if (!nt) { fprintf(stderr, "gem: out of memory (copy worklist)\n"); exit(1); }
        memcpy(nt, map->tasks, sizeof(GemCopyTask) * map->ntasks);
        if (map->tasks != map->task_buf) free(map->tasks);
        map->tasks = nt;
        map->taskcap = ncap;
    }
    map->tasks[map->ntasks].src = src;
    map->tasks[map->ntasks].dst = dst;
    map->tasks[map->ntasks].is_env = is_env;
    map->ntasks++;
}

static inline size_t gem_copy_hash(const void *p, size_t mask) {
    uint64_t x = (uint64_t)(uintptr_t)p >> 4;
    x *= 0x9E3779B97F4A7C15ull;
    return (size_t)(x >> 32) & mask;
}

static void gem_copy_table_insert(GemCopyMap *map, void *old, void *new_ptr) {
    size_t mask = map->tcap - 1;
    size_t i = gem_copy_hash(old, mask);
    while (map->table[i].old_ptr) i = (i + 1) & mask;
    map->table[i].old_ptr = old;
    map->table[i].new_ptr = new_ptr;
}

static void gem_copy_table_grow(GemCopyMap *map) {
    GemCopyEntry *old = map->table;
    size_t old_cap = map->tcap;
    map->tcap = old_cap ? old_cap * 2 : 64;
    map->table = (GemCopyEntry *)calloc(map->tcap, sizeof(GemCopyEntry));
    if (!map->table) { fprintf(stderr, "gem: out of memory (copy map)\n"); exit(1); }
    if (old) {
        for (size_t i = 0; i < old_cap; i++)
            if (old[i].old_ptr) gem_copy_table_insert(map, old[i].old_ptr, old[i].new_ptr);
        free(old);
    } else {
        for (int i = 0; i < map->len; i++)
            gem_copy_table_insert(map, map->inline_buf[i].old_ptr, map->inline_buf[i].new_ptr);
    }
}

static void *gem_copy_map_find(GemCopyMap *map, void *old) {
    if (map->table) {
        size_t mask = map->tcap - 1;
        size_t i = gem_copy_hash(old, mask);
        while (map->table[i].old_ptr) {
            if (map->table[i].old_ptr == old) return map->table[i].new_ptr;
            i = (i + 1) & mask;
        }
        return NULL;
    }
    for (int i = 0; i < map->len; i++) {
        if (map->inline_buf[i].old_ptr == old) return map->inline_buf[i].new_ptr;
    }
    return NULL;
}

static void gem_copy_map_add(GemCopyMap *map, void *old, void *new_ptr) {
    if (!map->table && map->len < GEM_COPY_MAP_INLINE) {
        map->inline_buf[map->len].old_ptr = old;
        map->inline_buf[map->len].new_ptr = new_ptr;
        map->len++;
        return;
    }
    if (!map->table || (size_t)(map->len + 1) * 2 > map->tcap) gem_copy_table_grow(map);
    gem_copy_table_insert(map, old, new_ptr);
    map->len++;
}

static void *gem_copy_alloc(GemCopyMap *map, size_t size) {
    if (map->use_malloc) return calloc(1, size);
    return gem_arena_alloc(gem_current_arena(), size);
}

static char *gem_copy_strdup(GemCopyMap *map, const char *s, int slen) {
    char *copy = (char *)gem_copy_alloc(map, (size_t)slen + 1);
    memcpy(copy, s, (size_t)slen);
    copy[slen] = '\0';
    return copy;
}

/* ─── Iterative deep copy ───
 *
 * gem_copy_shallow returns the copy of `val`: strings and buffers are copied
 * whole; a table or closure env gets its new shell allocated and recorded in
 * the copy map (so aliasing and cycles are preserved), then filled -- right
 * away while the inline nesting is below GEM_COPY_INLINE_DEPTH, otherwise
 * through a task on the map's worklist, which gem_copy_drain runs until none
 * are left. C recursion is therefore bounded by GEM_COPY_INLINE_DEPTH, not by
 * the depth of the data. */

static void gem_copy_fill_table(GemTable *t, GemTable *nt, GemCopyMap *map);
static void gem_copy_fill_env(void *env, void *new_env, GemCopyMap *map);

/* Fill a new shell now if the inline depth allows, else defer it. */
static inline void gem_copy_fill(void *src, void *dst, int is_env, GemCopyMap *map) {
    if (map->depth < GEM_COPY_INLINE_DEPTH) {
        map->depth++;
        if (is_env) gem_copy_fill_env(src, dst, map);
        else gem_copy_fill_table((GemTable *)src, (GemTable *)dst, map);
        map->depth--;
    } else {
        gem_copy_push_task(map, src, dst, is_env);
    }
}

static GemVal gem_copy_shallow(GemVal val, GemCopyMap *map) {
    switch (val.type) {
        case VAL_NIL:
        case VAL_BOOL:
        case VAL_INT:
        case VAL_FLOAT:
        case VAL_REF:
        case VAL_LAZY:
            return val;
        case VAL_STRING: {
            if (map->share_strings) return val;
            if (gem_copy_is_external(map, val.sval)) return val;
            void *existing = gem_copy_map_find(map, val.sval);
            if (existing) { GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = (char *)existing; r.slen = val.slen; return r; }
            GemVal r;
            r.type = VAL_STRING;
            r.magic = GEM_MAGIC;
            r.sval = gem_copy_strdup(map, val.sval, val.slen);
            r.slen = val.slen;
            gem_copy_map_add(map, val.sval, r.sval);
            return r;
        }
        case VAL_TABLE: {
            GemTable *t = val.table;
            if (gem_copy_is_external(map, t)) return val;
            if (map->stamp_gen) t->snap_gen = map->stamp_gen;
            if (map->probe && t->snap_gen) {
                int seen = 0;
                for (int k = 0; k < map->probe_n; k++) if (map->probe_gens[k] == t->snap_gen) { seen = 1; break; }
                if (!seen) {
                    if (map->probe_n == map->probe_cap) {
                        map->probe_cap = map->probe_cap ? map->probe_cap * 2 : 8;
                        map->probe_gens = (uint32_t *)realloc(map->probe_gens, sizeof(uint32_t) * (size_t)map->probe_cap);
                        if (!map->probe_gens) { fprintf(stderr, "gem: out of memory (spawn)\n"); exit(1); }
                    }
                    map->probe_gens[map->probe_n++] = t->snap_gen;
                }
            }
            void *existing = gem_copy_map_find(map, t);
            if (existing) { GemVal r; r.type = VAL_TABLE; r.magic = GEM_MAGIC; r.table = (GemTable *)existing; return r; }
            GemTable *nt = (GemTable *)gem_copy_alloc(map, sizeof(GemTable));
            gem_copy_map_add(map, t, nt);
            nt->len = t->len;
            nt->cap = t->cap;
            nt->keys = (GemVal *)gem_copy_alloc(map, sizeof(GemVal) * t->cap);
            nt->vals = (GemVal *)gem_copy_alloc(map, sizeof(GemVal) * t->cap);
            nt->str_index = NULL;
            nt->nstr = t->nstr;
            nt->shape_id = gem_shape_counter++;
            nt->immutable = t->immutable;  /* a frozen namespace stays frozen */
            nt->is_array = t->is_array;
            nt->braces = t->braces;
            /* A reset moves the table within its process: it stays part of
               the snapshot unit it was copied into. */
            if (map->preserve_external) nt->snap_gen = t->snap_gen;
            nt->arena_next = NULL;
            /* The string-key index is built on first use (gem_table_index): many
               copies (spawned module state, messages) are never looked up by key. */
            nt->index_stale = (t->str_index != NULL || t->index_stale);
            if (!map->use_malloc) {
                GemArena *a = gem_current_arena();
                nt->arena_next = a->table_list;
                a->table_list = nt;
                nt->mut_seq = gem_mut_clock;
            }
            if (t->len > 0) gem_copy_fill(t, nt, 0, map);
            GemVal r; r.type = VAL_TABLE; r.magic = GEM_MAGIC; r.table = nt; return r;
        }
        case VAL_BUFFER: {
            if (gem_copy_is_external(map, val.buffer)) return val;
            map->saw_mutable = 1;
            void *bex = gem_copy_map_find(map, val.buffer);
            if (bex) { GemVal r; r.type = VAL_BUFFER; r.magic = GEM_MAGIC; r.buffer = (GemBuffer *)bex; return r; }
            GemBuffer *ob = val.buffer;
            GemBuffer *nb = (GemBuffer *)gem_copy_alloc(map, sizeof(GemBuffer));
            gem_copy_map_add(map, ob, nb);
            nb->arena_next = NULL;
            if (!map->use_malloc) {
                GemArena *a = gem_current_arena();
                nb->arena_next = a->buffer_list;
                a->buffer_list = nb;
            }
            nb->len = ob->len;
            nb->cap = ob->cap;
            nb->data = (char *)gem_copy_alloc(map, ob->cap);
            memcpy(nb->data, ob->data, ob->len);
            GemVal r; r.type = VAL_BUFFER; r.magic = GEM_MAGIC; r.buffer = nb; return r;
        }
        case VAL_FN: {
            if (!val.env) return val;
            /* An env outside the reset region was built before the mark, so its
               box pointers (set once at closure creation) also predate it. */
            if (gem_copy_is_external(map, val.env)) return val;
            void *existing = gem_copy_map_find(map, val.env);
            if (existing) return (GemVal){.type = VAL_FN, .magic = GEM_MAGIC, .fn = val.fn, .env = existing};
            intptr_t n = *(intptr_t *)val.env;
            size_t env_size = sizeof(intptr_t) + sizeof(GemVal *) * n;
            void *new_env = gem_copy_alloc(map, env_size);
            gem_copy_map_add(map, val.env, new_env);
            *(intptr_t *)new_env = n;
            if (n > 0) gem_copy_fill(val.env, new_env, 1, map);
            return (GemVal){.type = VAL_FN, .magic = GEM_MAGIC, .fn = val.fn, .env = new_env};
        }
    }
    return val;
}

/* Pinned boxes in malloc'd copies (gem_deep_copy_malloc: timer messages,
   module snapshot units). They belong to no process, so this set records
   which of their boxes stand for pinned ones; gem_deep_free drops them. */
static GemPinEntry *gem_malloc_pinned = NULL;

/* Is `box` a pinned box (a mutated capture, shared by every closure that
   assigns it)? Pinned boxes are malloc'd and listed in their process's
   pin-set, or, in a malloc'd copy, in gem_malloc_pinned. */
static int gem_copy_box_is_pinned(GemCopyMap *map, GemVal *box) {
    if (map->src_pid >= 0) {
        GemProcess *sp = &gem_proc_table[map->src_pid];
        if (sp->pinned_boxes && hmgeti(sp->pinned_boxes, (void *)box) >= 0) return 1;
    }
    return gem_malloc_pinned && hmgeti(gem_malloc_pinned, (void *)box) >= 0;
}

/* A fresh box for the copy of capture box `old`. A pinned source gets a
   pinned copy: closures keep writing it after the copy, and only a pinned
   box (pin-set: walked by every reset of the destination process) keeps the
   values stored into it alive. Arena boxes are write-once, so a plain
   arena box is enough for them. */
static GemVal *gem_copy_new_box(GemCopyMap *map, GemVal *old) {
    if (!map->preserve_external && gem_copy_box_is_pinned(map, old)) {
        if (map->use_malloc) {
            GemVal *b = (GemVal *)calloc(1, sizeof(GemVal));
            GemPinEntry e = { .key = b, .value = 0, .seq = 0 };
            hmputs(gem_malloc_pinned, e);
            return b;
        }
        return gem_box_alloc();  /* registers in the destination's pin-set */
    }
    return (GemVal *)gem_copy_alloc(map, sizeof(GemVal));
}

static void gem_copy_fill_env(void *env, void *new_env, GemCopyMap *map) {
    intptr_t n = *(intptr_t *)env;
    GemVal **old = (GemVal **)((char *)env + sizeof(intptr_t));
    GemVal **new_fields = (GemVal **)((char *)new_env + sizeof(intptr_t));
    for (intptr_t i = 0; i < n; i++) {
        /* Box pointers outside the reset region are kept: malloc'd pinned
           boxes (gem_box_alloc, mutated captures) and older arena boxes. A
           pinned box is registered in the process's pin-set; the first env
           field to reach it this cycle migrates its contents (in place),
           later ones just keep the pointer. */
        if (gem_copy_is_external(map, old[i])) {
            new_fields[i] = old[i];
            if (gem_current_pid >= 0) {
                GemProcess *proc = &gem_proc_table[gem_current_pid];
                if (gem_pin_mark_walked(proc, old[i])) {
                    *old[i] = gem_copy_shallow(*old[i], map);
                }
            }
            continue;
        }
        GemVal *existing = (GemVal *)gem_copy_map_find(map, old[i]);
        if (existing) {
            new_fields[i] = existing;
            continue;
        }
        if (map->snap_src && map->snap_src->pinned_boxes &&
            hmgeti(map->snap_src->pinned_boxes, (void *)old[i]) >= 0)
            map->saw_mutable = 1;
        GemVal *box = gem_copy_new_box(map, old[i]);
        gem_copy_map_add(map, old[i], box);
        *box = gem_copy_shallow(*old[i], map);
        new_fields[i] = box;
    }
}

static void gem_copy_fill_table(GemTable *t, GemTable *nt, GemCopyMap *map) {
    for (int i = 0; i < t->len; i++) {
        nt->keys[i] = gem_copy_shallow(t->keys[i], map);
        nt->vals[i] = gem_copy_shallow(t->vals[i], map);
    }
}

static void gem_copy_drain(GemCopyMap *map) {
    while (map->ntasks > 0) {
        GemCopyTask task = map->tasks[--map->ntasks];
        /* depth is 0 here: a task starts a fresh inline budget */
        if (task.is_env) gem_copy_fill_env(task.src, task.dst, map);
        else gem_copy_fill_table((GemTable *)task.src, (GemTable *)task.dst, map);
    }
}

static GemVal gem_deep_copy_internal(GemVal val, GemCopyMap *map) {
    GemVal r = gem_copy_shallow(val, map);
    gem_copy_drain(map);
    return r;
}

GemVal gem_deep_copy(GemVal val, int src_pid) {
    GemCopyMap map;
    gem_copy_map_init(&map, 0);
    map.src_pid = src_pid;
    GemVal result = gem_deep_copy_internal(val, &map);
    gem_copy_map_cleanup(&map);
    return result;
}

GemVal gem_deep_copy_malloc(GemVal val) {
    GemCopyMap map;
    gem_copy_map_init(&map, 1);
    map.src_pid = gem_current_pid;
    GemVal result = gem_deep_copy_internal(val, &map);
    gem_copy_map_cleanup(&map);
    return result;
}

/* Free a value built by gem_deep_copy_malloc. Iterative (an explicit stack of
   pending values) so deep data cannot overflow the C stack; a visited set
   frees aliased structure once. */
void gem_deep_free(GemVal val) {
    gem_deep_free_n(&val, 1);
}

void gem_deep_free_n(const GemVal *vals, int nvals) {
    GemCopyMap visited;
    gem_copy_map_init(&visited, 1);
    GemVal *stack = NULL;
    size_t n = 0, cap = 0;
#define GEM_FREE_PUSH(v) do { \
        if (n == cap) { cap = cap ? cap * 2 : 64; stack = (GemVal *)realloc(stack, sizeof(GemVal) * cap); \
                        if (!stack) { fprintf(stderr, "gem: out of memory (free)\n"); exit(1); } } \
        stack[n++] = (v); } while (0)
    for (int vi = 0; vi < nvals; vi++) GEM_FREE_PUSH(vals[vi]);
    while (n > 0) {
        GemVal v = stack[--n];
        switch (v.type) {
            case VAL_STRING:
                if (gem_copy_map_find(&visited, v.sval)) break;
                gem_copy_map_add(&visited, v.sval, (void *)1);
                free(v.sval);
                break;
            case VAL_TABLE: {
                GemTable *t = v.table;
                if (gem_copy_map_find(&visited, t)) break;
                gem_copy_map_add(&visited, t, (void *)1);
                for (int i = 0; i < t->len; i++) {
                    GEM_FREE_PUSH(t->keys[i]);
                    GEM_FREE_PUSH(t->vals[i]);
                }
                gem_str_index_free(&t->str_index);
                /* keys/vals were copied onto the stack above */
                free(t->keys);
                free(t->vals);
                free(t);
                break;
            }
            case VAL_BUFFER: {
                GemBuffer *b = v.buffer;
                if (gem_copy_map_find(&visited, b)) break;
                gem_copy_map_add(&visited, b, (void *)1);
                free(b->data);
                free(b);
                break;
            }
            case VAL_FN: {
                if (!v.env) break;
                if (gem_copy_map_find(&visited, v.env)) break;
                gem_copy_map_add(&visited, v.env, (void *)1);
                intptr_t nf = *(intptr_t *)v.env;
                GemVal **fields = (GemVal **)((char *)v.env + sizeof(intptr_t));
                for (intptr_t i = 0; i < nf; i++) {
                    if (gem_copy_map_find(&visited, fields[i])) continue;
                    gem_copy_map_add(&visited, fields[i], (void *)1);
                    GEM_FREE_PUSH(*fields[i]);
                    if (gem_malloc_pinned) (void)hmdel(gem_malloc_pinned, (void *)fields[i]);
                    free(fields[i]);
                }
                free(v.env);
                break;
            }
            default:
                break;
        }
    }
#undef GEM_FREE_PUSH
    free(stack);
    gem_copy_map_cleanup(&visited);
}

/* ─── Pinned-box set ─── */

/* Boxes for fn-local mutated-captured vars are allocated via gem_box_alloc
   (plain malloc) so they survive arena reset. Each is registered in the
   current process's pin-set. At every reset:
     1. Pre-pass: codegen passes the box pointers as `pinned_roots`. We
        mark each as "walked" and deep-copy its contents into the fresh
        arena. This handles boxes that are live via the function's local
        even when no capturing closure is alive.
     2. During the regular roots / mailbox walk, gem_copy_fill_env
        external branch encounters env fields pointing at pinned boxes.
        gem_pin_mark_walked dedups: if not yet walked, walk the contents
        now; if already walked, just preserve the pointer.
     3. Boxes allocated before the reset's point (seq < point.pin_seq) can be
        held by callers' frames: they are always walked and never freed.
     4. Sweep: any newer pin-set entry not marked is unreachable — free it.
        Surviving entries are reset to "untouched" for the next cycle.
   On process exit, gem_pin_free_all releases everything in one shot. */

GemVal *gem_box_alloc(void) {
    GemVal *p = (GemVal *)calloc(1, sizeof(GemVal));
    if (gem_current_pid >= 0) {
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        GemPinEntry e = { .key = p, .value = 0, .seq = proc->arena.pin_seq++ };
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

/* Free every unmarked box allocated at or after `min_seq` (boxes older than
   the reset's mark can be referenced from callers' frames, so they are
   never freed by it); clear all marks for the next cycle. */
static void gem_pin_sweep_from(GemProcess *proc, uint64_t min_seq) {
    if (!proc || !proc->pinned_boxes) return;
    /* Build the free-list first; hmdel rebalances and would invalidate
       indices mid-iteration. */
    void **to_free = NULL;
    size_t n = hmlenu(proc->pinned_boxes);
    for (size_t i = 0; i < n; i++) {
        if (proc->pinned_boxes[i].value == 0 && proc->pinned_boxes[i].seq >= min_seq) {
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

void gem_pin_sweep(GemProcess *proc) {
    gem_pin_sweep_from(proc, 0);
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

/* ─── Region reset ───────────────────────────────────────────────────
 *
 * A loop takes a GemArenaMark when it starts; at its back-edge it calls
 * gem_arena_reset_region with the values live there. The reset copies the
 * reachable part of the memory allocated since one of the mark's points
 * (the "region") into fresh memory and frees the rest of the region. Memory
 * older than that point is never moved or freed.
 *
 * Promotion. A mark has two points: `base`, where the loop started, and
 * `young`, the end of what the loop's earlier resets kept. A reset normally
 * takes its region from `young`, and afterwards moves `young` past the
 * copies it made, so what it kept counts as older from then on and later
 * resets leave it alone: data the loop holds is copied once, not again by
 * every reset (a generational collector's old space). The copies go into
 * the free tail of the block the previous reset's copies ended in, and new
 * allocations start in a fresh block after it, so the kept memory stays
 * dense. Garbage among kept data (a record the loop replaced) stays until a
 * full reset, which takes its region from `base` and copies everything the
 * loop keeps once more: the first reset of a mark, and any reset once the
 * bytes kept since the last full one exceed twice what that one kept (four
 * times, when it kept at least half of its region: the kept data is mostly
 * live), plus GEM_ARENA_RESET_THRESHOLD. Kept memory so stays below about
 * two (four) times what the last full reset kept, and each full copy is
 * paid for by the promotions that led to it.
 *
 * Soundness invariant. After the reset, no reachable pointer refers into the
 * freed region. Every place a pointer into the region can live is covered:
 *
 *   1. The resetting frame's C locals: codegen passes every value live at
 *      the back-edge (liveness.gem) as a root; the rest are dead.
 *   2. Callers' C frames (Gem frames and runtime frames such as pcall, sort,
 *      build_string that called back into Gem): they were suspended before
 *      the mark was taken, so they only hold values allocated before it. A
 *      value created after the mark reaches a caller only by being returned
 *      (after the loop, so after its last reset) or by being stored into an
 *      older mutable object -- case 3.
 *   3. Older mutable objects (older than the region's point: allocated
 *      before the loop, or kept by an earlier reset of it): tables (key/val
 *      arrays and entries), buffers (data), pinned boxes (contents), mailbox
 *      nodes. Each arena keeps a list of its tables and buffers, newest
 *      first, and a point records the list heads, the pinned-box sequence
 *      number and a clock epoch, so every such object older than the point
 *      is found (tables through the write barrier's remembered log) and
 *      fixed up in place. Arena closure envs and capture boxes are written
 *      only when created, and strings are immutable, so older ones cannot
 *      point into the region; the same holds for kept ones, which were
 *      copied before the point moved past them.
 *   4. Process state: module slots (proc->globals), the mailbox, read_buf.
 *   5. Other processes never hold pointers into this arena (spawn and send
 *      deep-copy). A pinned box reaching this process by a copy is a new
 *      pinned box of this process (gem_copy_new_box), so case 3 covers it.
 *
 * Nested loops nest their marks: a reset only frees memory newer than one
 * of its own points, so marks held by enclosing loops (all older: their
 * young points were set before this loop started) stay valid.
 *
 * Return resets (gem_arena_reset_return). A fn records a point at entry
 * and at a return resets from it with the return value as the only root,
 * through a one-shot mark whose young point is its base point (so the
 * reset is full). The cases above hold with "the loop" read as "the
 * call": the frame's other locals are dead at the return (1); callers'
 * frames, including argument values a caller evaluated before the call
 * and runtime frames that called back into Gem, were suspended before the
 * point (2); the point's clock epoch (gem_arena_point bumps
 * gem_mut_clock) makes the first write during the call to any older table
 * log it, so case 3 finds it; and every mark held by an enclosing loop or
 * call is older than the point, since loops in the caller cannot reach
 * their back-edge during the call, while loops in the callee have ended
 * or are left by this return. A non-escaping closure (a pcall or sort
 * argument, mark_non_escaping) can write its creator's C locals, which
 * its loops' resets root but a return reset would not, so it has no
 * return point; nor does a mutual-TCO body, whose return carries
 * the next body's args in the global gem_tail_args: its trampoline resets
 * when the cycle returns. A call left by an error (longjmp) skips its
 * reset.
 *
 * Hysteresis: the next reset for this mark fires after the loop allocates
 * max(GEM_ARENA_RESET_THRESHOLD, 2 * bytes copied + bytes scanned / 2) after
 * a young reset, or max(GEM_ARENA_RESET_THRESHOLD, bytes scanned / 2) after
 * a full one, so each reset's work is paid for by at least as much fresh
 * allocation or promotion. */

/* GEM_DIAG=1 reports reset counts and volumes at exit. */
static uint64_t gem_diag_resets, gem_diag_full, gem_diag_ret, gem_diag_ret_copied, gem_diag_copied, gem_diag_scanned, gem_diag_freed;
static double gem_diag_t_total, gem_diag_t_walk, gem_diag_t_max;
#include <time.h>
static double gem_diag_now(void) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return ts.tv_sec + ts.tv_nsec * 1e-9; }
static int gem_diag_state = -1;

static void gem_diag_reset_report(void) {
    fprintf(stderr, "gem_diag: arena_resets=%llu copied=%llu scanned=%llu freed=%llu time=%.3fs walk=%.3fs full=%llu ret_resets=%llu ret_copied=%llu max=%.3fs\n",
            (unsigned long long)gem_diag_resets, (unsigned long long)gem_diag_copied,
            (unsigned long long)gem_diag_scanned, (unsigned long long)gem_diag_freed,
            gem_diag_t_total, gem_diag_t_walk, (unsigned long long)gem_diag_full,
            (unsigned long long)gem_diag_ret, (unsigned long long)gem_diag_ret_copied, gem_diag_t_max);
}

static void gem_diag_note_reset(size_t copied, size_t scanned, size_t freed, int full) {
    if (gem_diag_state < 0) {
        const char *e = getenv("GEM_DIAG");
        gem_diag_state = (e && e[0] == '1');
        if (gem_diag_state) atexit(gem_diag_reset_report);
    }
    if (!gem_diag_state) return;
    gem_diag_resets++;
    gem_diag_full += full;
    gem_diag_copied += copied;
    gem_diag_scanned += scanned;
    gem_diag_freed += freed;
}

static int gem_val_in_region(GemCopyMap *map, GemVal v) {
    switch (v.type) {
        case VAL_STRING: return gem_region_contains(map->region, v.sval);
        case VAL_TABLE:  return gem_region_contains(map->region, v.table);
        case VAL_BUFFER: return gem_region_contains(map->region, v.buffer);
        case VAL_FN:     return v.env != NULL && gem_region_contains(map->region, v.env);
        default:         return 0;
    }
}

/* A table in the remembered log whose key strings a reset moved: point its
   string index at the copies (the old bytes are still mapped here), so a big
   table that gained a few keys keeps its index. */
static void gem_rekey_index(GemTable *t, const int *moved, int n_moved) {
    if (!t->str_index || t->index_stale) return;
    for (int k = 0; k < n_moved; k++) {
        int i = moved[k];
        gem_str_index_put(&t->str_index, t->keys[i].sval, t->keys[i].slen, i);
    }
}

/* Sets *copied_out, *scanned_out and *region_out to the bytes the reset
   copied, scanned and found in its region. */
static void gem_region_reset_impl(GemArenaMark *mark, GemVal **roots, int n_roots,
                                  GemVal **pinned_roots, int n_pinned,
                                  size_t *copied_out, size_t *scanned_out, size_t *region_out) {
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    GemArena *arena = &proc->arena;

    /* Full reset from the base point before the first promotion and once
       the kept memory has doubled; otherwise only what is newer than the
       young point. */
    int full = mark->young.clock == mark->base.clock || mark->old_bytes > mark->old_limit;
    GemArenaPoint *pt = full ? &mark->base : &mark->young;
    GemArenaBlock *mb = pt->block;

    /* The region: mb's tail past the point, plus every later block. */
    GemArenaBlock *post_blocks = mb->next;
    GemRegion region;
    gem_region_build(&region, mb->data + pt->used, mb->data + mb->used, post_blocks);

    size_t region_used = mb->used - pt->used;
    for (GemArenaBlock *b = post_blocks; b; b = b->next) region_used += b->used;

    /* Detach the region's blocks. The copies go into mb's tail when nothing
       was allocated there since the point (a reset leaves its last copy
       block so, below), else into a fresh block after mb. */
    int tail_free = mb->used == pt->used;
    mb->next = NULL;
    if (tail_free) arena->current = mb;
    else gem_arena_append_block(arena, mb, 0);

    GemTable *post_tables = arena->table_list;
    arena->table_list = pt->tables;
    GemBuffer *post_buffers = arena->buffer_list;
    arena->buffer_list = pt->buffers;
    size_t bytes_before = arena->bytes_allocated;
    size_t scanned = 0;

    GemCopyMap map;
    gem_copy_map_init(&map, 0);
    map.preserve_external = 1;
    map.region = &region;

    for (int i = 0; i < n_pinned; i++) {
        GemVal *box = pinned_roots[i];
        if (gem_pin_mark_walked(proc, box)) *box = gem_deep_copy_internal(*box, &map);
    }
    for (int i = 0; i < n_roots; i++) {
        *roots[i] = gem_deep_copy_internal(*roots[i], &map);
    }
    if (proc->globals) {
        for (int i = 0; i < gem_n_globals; i++) {
            proc->globals[i] = gem_deep_copy_internal(proc->globals[i], &map);
        }
    }
    /* Pinned boxes older than the point may be reachable from callers, or
       from what earlier resets kept. */
    if (proc->pinned_boxes) {
        size_t n = hmlenu(proc->pinned_boxes);
        scanned += n * sizeof(GemPinEntry);
        for (size_t i = 0; i < n; i++) {
            if (proc->pinned_boxes[i].seq < pt->pin_seq && !proc->pinned_boxes[i].value) {
                proc->pinned_boxes[i].value = 1;
                GemVal *box = (GemVal *)proc->pinned_boxes[i].key;
                *box = gem_deep_copy_internal(*box, &map);
            }
        }
    }
    /* Tables older than the point written since it: the suffix of the
       remembered log with epoch >= pt->clock (see gem_table_written).
       Region tables in it are skipped (garbage, or copied above). */
    double tw0 = gem_diag_state > 0 ? gem_diag_now() : 0;
    size_t lo = 0, hi = arena->rem_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arena->rem[mid].epoch < pt->clock) lo = mid + 1; else hi = mid;
    }
    size_t rem_start = lo;
    int *moved = NULL;
    int moved_cap = 0;
    for (size_t ri = rem_start; ri < arena->rem_len; ri++) {
        GemTable *t = arena->rem[ri].t;
        scanned += sizeof(GemRemEntry);
        if (gem_region_contains(&region, t) || t->rem_flag) continue;
        t->rem_flag = 1;
        size_t arr = sizeof(GemVal) * (size_t)t->cap;
        scanned += 2 * arr;
        if (gem_region_contains(&region, t->keys)) {
            GemVal *nk = (GemVal *)gem_arena_alloc(arena, arr);
            memcpy(nk, t->keys, sizeof(GemVal) * (size_t)t->len);
            t->keys = nk;
        }
        if (gem_region_contains(&region, t->vals)) {
            GemVal *nv = (GemVal *)gem_arena_alloc(arena, arr);
            memcpy(nv, t->vals, sizeof(GemVal) * (size_t)t->len);
            t->vals = nv;
        }
        int n_moved = 0;
        for (int i = 0; i < t->len; i++) {
            if (gem_val_in_region(&map, t->keys[i])) {
                t->keys[i] = gem_deep_copy_internal(t->keys[i], &map);
                if (t->keys[i].type == VAL_STRING) {
                    if (n_moved == moved_cap) {
                        moved_cap = moved_cap ? moved_cap * 2 : 64;
                        moved = (int *)realloc(moved, sizeof(int) * (size_t)moved_cap);
                        if (!moved) { fprintf(stderr, "gem: out of memory (reset)\n"); exit(1); }
                    }
                    moved[n_moved++] = i;
                }
            }
            if (gem_val_in_region(&map, t->vals[i])) {
                t->vals[i] = gem_deep_copy_internal(t->vals[i], &map);
            }
        }
        if (n_moved) gem_rekey_index(t, moved, n_moved);
    }
    free(moved);
    /* Compact the log. Entries for tables in the region (about to be
       unmapped) or kept by earlier resets of this loop (newer than the base
       point) are dropped: every live mark other than this one is older than
       the base point, so its resets see those tables as part of their own
       region; this mark's young resets read only entries written after
       this one, and its full resets have those tables in their region. Of
       the entries for tables older than the base point, one per table is
       kept; it still has epoch >= base.clock, so this and every enclosing
       mark (all older) find it. A young reset compacts only the entries
       written since the young point (the ones before it were compacted by
       earlier resets), so a table older than the base point written again
       keeps one entry per reset that saw it; the whole window from the base
       point's epoch is compacted again by full resets and whenever it has
       doubled since its last compaction, which keeps both the log and the
       work linear. */
    {
        size_t blo = 0, bhi = arena->rem_len;
        while (blo < bhi) {
            size_t mid = blo + (bhi - blo) / 2;
            if (arena->rem[mid].epoch < mark->base.clock) blo = mid + 1; else bhi = mid;
        }
        int whole = full || arena->rem_len - blo > 2 * mark->rem_window + 4096;
        size_t from = whole ? blo : rem_start;
        GemArenaBlock *bb = mark->base.block;
        GemRegion kept;
        gem_region_build(&kept, bb->data + mark->base.used, bb->data + bb->used, bb->next);
        size_t w = from;
        for (size_t ri = from; ri < arena->rem_len; ri++) {
            GemTable *t = arena->rem[ri].t;
            if (gem_region_contains(&region, t) || gem_region_contains(&kept, t)) {
                t->rem_flag = 0;
                continue;
            }
            if (t->rem_flag == 2) continue;   /* a later entry for a table already kept */
            t->rem_flag = 2;
            arena->rem[w++] = arena->rem[ri];
        }
        for (size_t ri = from; ri < w; ri++) arena->rem[ri].t->rem_flag = 0;
        /* Each entry visited touches a table that is likely cold: charge a
           cache line for it, so the next reset waits accordingly. */
        scanned += (arena->rem_len - from) * 64;
        arena->rem_len = w;
        if (whole) mark->rem_window = w - blo;
        free(kept.ranges);
    }
    if (gem_diag_state > 0) gem_diag_t_walk += gem_diag_now() - tw0;
    /* Buffers older than the point. */
    for (GemBuffer *b = pt->buffers; b; b = b->arena_next) {
        scanned += sizeof(GemBuffer);
        if (gem_region_contains(&region, b->data)) {
            char *nd = (char *)gem_arena_alloc(arena, (size_t)b->cap);
            memcpy(nd, b->data, (size_t)b->len);
            b->data = nd;
        }
    }
    /* Mailbox: copy region-allocated message values; region nodes are
       re-allocated, older nodes are reused in place. */
    {
        GemMsgNode *n = proc->mailbox.head;
        GemMsgNode *head = NULL, *tail = NULL;
        while (n) {
            GemMsgNode *next = n->next;
            /* A backlog an earlier reset kept is walked again by every
               reset until it is received: charge it, so the resets space
               out as the backlog grows. */
            scanned += 256;
            GemVal v = gem_deep_copy_internal(n->value, &map);
            GemMsgNode *node = n;
            if (gem_region_contains(&region, n)) node = (GemMsgNode *)gem_arena_alloc(arena, sizeof(GemMsgNode));
            node->value = v;
            node->next = NULL;
            if (tail) tail->next = node; else head = node;
            tail = node;
            n = next;
        }
        proc->mailbox.head = head;
        proc->mailbox.tail = tail;
    }
    if (proc->read_buf && gem_region_contains(&region, proc->read_buf)) {
        proc->read_buf = NULL;
        proc->read_buf_cap = 0;
    }
    gem_copy_map_cleanup(&map);

    gem_pin_sweep_from(proc, pt->pin_seq);

    /* Region tables are garbage now (live ones were copied): free their
       malloc'd indexes, then unmap the region's blocks. */
    for (GemTable *t = post_tables; t && t != pt->tables; t = t->arena_next) {
        gem_str_index_free(&t->str_index);
    }
    (void)post_buffers;
    size_t region_bytes = 0;
    for (int i = 0; i < region.n; i++) region_bytes += (size_t)(region.ranges[i].hi - region.ranges[i].lo);
    gem_arena_free_blocks(post_blocks);
    arena->bytes_freed += region_used;
    if (!tail_free) {
        if (mb->dirty < mb->used) mb->dirty = mb->used;
        mb->used = pt->used;
    }
    free(region.ranges);

    size_t copied = arena->bytes_allocated - bytes_before;
    gem_diag_note_reset(copied, scanned, region_bytes, full);

    /* Promote: what this reset kept now counts as older. The young point
       moves to the end of the copies, and new allocations start in a fresh
       block, so the copies' last block keeps its free tail for the next
       reset's copies. A new clock epoch makes the next write to any kept
       table log it again. */
    GemArenaBlock *last = arena->current;
    mark->young.block = last;
    mark->young.used = last->used;
    mark->young.tables = arena->table_list;
    mark->young.buffers = arena->buffer_list;
    mark->young.pin_seq = arena->pin_seq;
    mark->young.clock = ++gem_mut_clock;
    mark->young.arena = arena;
    mark->young.ret_trig = arena->bytes_allocated - arena->bytes_freed + GEM_ARENA_RESET_THRESHOLD;
    gem_arena_append_block(arena, last, 0);
    *copied_out = copied;
    *scanned_out = scanned;
    *region_out = region_used;

    size_t budget;
    if (full) {
        /* A full reset that found little garbage (it kept at least half of
           the region) means the kept data is mostly live: wait for it to
           grow four times before the next one, not twice. */
        size_t factor = (mark->old_bytes > 0 && 2 * copied >= region_bytes) ? 4 : 2;
        mark->old_bytes = copied;
        mark->old_limit = factor * copied + GEM_ARENA_RESET_THRESHOLD;
        /* The full copy is paid for by the promotions that led to it, so
           the next reset waits only for the usual young allocation. */
        budget = scanned / 2;
    } else {
        mark->old_bytes += copied;
        budget = 2 * copied + scanned / 2;
    }
    if (budget < GEM_ARENA_RESET_THRESHOLD) budget = GEM_ARENA_RESET_THRESHOLD;
    mark->trigger = arena->bytes_allocated + budget;
}

void gem_remember_table(GemTable *t) {
    GemArena *a = gem_current_arena();
    if (a->rem_len == a->rem_cap) {
        a->rem_cap = a->rem_cap ? a->rem_cap * 2 : 256;
        a->rem = (GemRemEntry *)realloc(a->rem, sizeof(GemRemEntry) * a->rem_cap);
        if (!a->rem) { fprintf(stderr, "gem: out of memory (remembered log)\n"); exit(1); }
    }
    a->rem[a->rem_len].t = t;
    a->rem[a->rem_len].epoch = gem_mut_clock;
    a->rem_len++;
    /* Count the log as allocation so a loop that only writes tables still
       reaches a reset, which compacts the log. */
    a->bytes_allocated += sizeof(GemRemEntry);
}

void gem_arena_reset_region(GemArenaMark *mark, GemVal **roots, int n_roots,
                            GemVal **pinned_roots, int n_pinned) {
    if (gem_current_pid < 0) return;
    if (!gem_arena_reset_due(mark)) return;
    double t0 = gem_diag_state != 0 ? gem_diag_now() : 0;
    size_t copied, scanned, region;
    gem_region_reset_impl(mark, roots, n_roots, pinned_roots, n_pinned, &copied, &scanned, &region);
    if (gem_diag_state > 0) {
        double dt = gem_diag_now() - t0;
        gem_diag_t_total += dt;
        if (dt > gem_diag_t_max) gem_diag_t_max = dt;
    }
}

void gem_arena_reset_return(const GemArenaPoint *pt, GemVal *ret, int own_frames) {
    if (gem_current_pid < 0 || gem_call_depth <= own_frames) return;
    GemArena *arena = &gem_proc_table[gem_current_pid].arena;
    size_t live = arena->bytes_allocated - arena->bytes_freed - (pt->ret_trig - GEM_ARENA_RESET_THRESHOLD);
    if (live + (arena->bytes_allocated - arena->ret_at) < arena->ret_min) return;
    double t0 = gem_diag_state != 0 ? gem_diag_now() : 0;
    /* A one-shot mark whose young point is its base point: the impl takes
       the full path, with the region everything allocated since `pt`. Its
       promotion and trigger updates land in this dead mark. */
    GemArenaMark mark;
    mark.base = *pt;
    mark.young = *pt;
    mark.trigger = 0;
    mark.old_bytes = 0;
    mark.old_limit = 0;
    mark.rem_window = 0;
    GemVal *roots[1] = { ret };
    size_t copied, scanned, region;
    gem_region_reset_impl(&mark, roots, ret ? 1 : 0, NULL, 0, &copied, &scanned, &region);
    /* The hysteresis is the arena's, not the frame's: a recursion
       returning a growing result up its levels copies it again only once
       the returning frame's region and the allocation since this reset
       add up to twice this copy plus half the bytes scanned (four times
       the copy, when this reset kept at
       least half of its region: the data was mostly live), so its copies
       grow geometrically, as a loop's do, not once per level. Counting
       the allocation since this reset lets a big copy hold back later
       return resets only until as much has been allocated again. */
    size_t factor = 2 * copied >= region ? 4 : 2;
    arena->ret_min = factor * copied + scanned / 2;
    arena->ret_at = arena->bytes_allocated;
    if (gem_diag_state > 0) {
        gem_diag_ret++;
        gem_diag_ret_copied += copied;
        double dt = gem_diag_now() - t0;
        gem_diag_t_total += dt;
        if (dt > gem_diag_t_max) gem_diag_t_max = dt;
    }
}

/* ─── Module globals ───────────────────────────────────────────────
 *
 * Per-process module slots with lazy, per-slot copy at spawn (overview in
 * gem.h, "Module globals").
 *
 * Snapshot units. A unit is an immutable malloc deep copy of a set of a
 * parent's slots that share structure (an aliasing component: slots whose
 * values reach a common table, buffer, closure env or capture box), made
 * with one copy map. It is refcounted: one reference per out[] entry of the
 * parent and per in[] entry of each child (or grandchild) holding it.
 *
 * Parent side. At a spawn the parent (gem_mod_build_units) snapshots every
 * slot holding a heavy value that has no unit yet: it walks those values
 * to find the components, then copies each into a new unit, stamping every
 * source table with the unit's gen. Units stay valid, and are reused by
 * later spawns, until the parent changes them:
 *   - a write to a slot (gem_global_set / gem_global_ref) drops that slot's
 *     unit (gem_global_unshare);
 *   - a change to a stamped table (gem_table_check_mutable, called by every
 *     table mutator) drops the unit with that gen (gem_table_mutate_slow);
 *   - a unit holding a buffer or a pinned capture box (mutable without any
 *     table write) is used by one spawn only.
 * Old arena memory is moved by region resets but keeps its stamp (reset
 * copies carry snap_gen). If the walk of a changed slot reaches a table of a
 * still-valid unit (it was stored there since), that unit is dropped and
 * rebuilt together with it, so components stay disjoint. Light slot values
 * (no unit) are copied at every spawn; a write of a light value over a
 * light value touches no unit.
 *
 * Child side. A heavy slot starts as VAL_LAZY with in[i] = the unit. The
 * first gem_global_get / gem_global_ref of any slot of that unit copies all
 * of the unit's slots that are still lazy in this process (gem_global_fault)
 * into the arena with one copy map. Strings are shared with the unit, which
 * the process references until it exits. A slot still lazy when the process
 * spawns is passed to the grandchild as the same unit (no copy at all).
 *
 * Region resets root all slots: VAL_LAZY slots and unit strings are not in
 * any region, so a reset leaves them as they are. Process exit drops the
 * process's unit references (gem_globals_free). */

GemVal *gem_cur_globals = NULL;
int gem_n_globals = 0;
static uint32_t gem_mod_gen_counter = 0;

struct GemModUnit {
    int refs;          /* one per out[] / in[] entry pointing at it */
    uint32_t gen;      /* stamp on the parent's tables copied into it */
    int shareable;     /* 0: holds a buffer or pinned box, used by one spawn only */
    int n;
    int *slots;        /* slot indices, ascending */
    GemVal *vals;      /* their values, one malloc copy */
};

static const GemVal GEM_LAZY_VAL = { .type = VAL_LAZY, .magic = GEM_MAGIC };

/* Code generated before lazy module copy (the checked-in stage0.c until it
   is regenerated) reads and writes gem_cur_globals[i] directly: it calls
   gem_globals_init, and spawn then copies every unit into the child at once
   and keeps none for later spawns. Current codegen calls
   gem_globals_init_lazy. */
static int gem_mod_lazy = 0;

void gem_globals_init(int n) {
    gem_n_globals = n;
}

void gem_globals_init_lazy(int n) {
    gem_n_globals = n;
    gem_mod_lazy = 1;
}

GemVal *gem_globals_alloc(void) {
    size_t n = (size_t)(gem_n_globals > 0 ? gem_n_globals : 1);
    GemVal *g = (GemVal *)calloc(1, n * sizeof(GemVal) + 2 * n * sizeof(GemModUnit *));
    if (!g) { fprintf(stderr, "gem: out of memory (module slots)\n"); exit(1); }
    for (int i = 0; i < gem_n_globals; i++) g[i] = GEM_NIL;
    return g;
}

/* Worth a unit: values whose copy costs more than a few bytes. */
static int gem_mod_heavy(GemVal v) {
    switch (v.type) {
        case VAL_TABLE:
        case VAL_BUFFER: return 1;
        case VAL_FN: return v.env != NULL;
        case VAL_STRING: return v.slen > GEM_MOD_LIGHT_STRING;
        default: return 0;
    }
}

static void gem_mod_unit_release(GemModUnit *u) {
    if (--u->refs > 0) return;
    gem_deep_free_n(u->vals, u->n);
    free(u->slots);
    free(u->vals);
    free(u);
}

static int gem_mod_unit_index(const GemModUnit *u, int slot) {
    int lo = 0, hi = u->n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (u->slots[mid] < slot) lo = mid + 1;
        else if (u->slots[mid] > slot) hi = mid - 1;
        else return mid;
    }
    return -1;
}

/* Stop sharing unit u from process p with future children. */
static void gem_mod_unshare(GemProcess *p, GemModUnit *u) {
    for (int k = 0; k < p->mod_live_n; k++) {
        if (p->mod_live[k] == u) {
            p->mod_live[k] = p->mod_live[--p->mod_live_n];
            break;
        }
    }
    GemModUnit **out = GEM_GLOBALS_OUT(p->globals);
    u->refs++;  /* hold u while dropping the slot references */
    for (int k = 0; k < u->n; k++) {
        int s = u->slots[k];
        if (out[s] == u) {
            out[s] = NULL;
            gem_mod_unit_release(u);
        }
    }
    gem_mod_unit_release(u);
}

static GemModUnit *gem_mod_live_find(GemProcess *p, uint32_t gen) {
    for (int k = 0; k < p->mod_live_n; k++)
        if (p->mod_live[k]->gen == gen) return p->mod_live[k];
    return NULL;
}

void gem_global_unshare(int i) {
    GemProcess *p = &gem_proc_table[gem_current_pid];
    GemModUnit *u = GEM_GLOBALS_OUT(p->globals)[i];
    if (u) gem_mod_unshare(p, u);
}

void gem_table_mutate_slow(GemTable *t) {
    if (t->immutable) {
        gem_error("cannot modify a module table");
        abort();
    }
    uint32_t gen = t->snap_gen;
    t->snap_gen = 0;
    if (gem_current_pid < 0) return;
    GemProcess *p = &gem_proc_table[gem_current_pid];
    if (!p->globals) return;
    GemModUnit *u = gem_mod_live_find(p, gen);
    if (u) gem_mod_unshare(p, u);
}

GemVal gem_global_fault(int i) {
    GemVal *g = gem_cur_globals;
    GemModUnit **in = GEM_GLOBALS_IN(g);
    GemModUnit *u = in[i];
    if (!u) { g[i] = GEM_NIL; return g[i]; }  /* not reached: a lazy slot has a unit */
    GemCopyMap map;
    gem_copy_map_init(&map, 0);
    map.share_strings = 1;
    for (int k = 0; k < u->n; k++) {
        int s = u->slots[k];
        if (g[s].type == VAL_LAZY && in[s] == u)
            g[s] = gem_deep_copy_internal(u->vals[k], &map);
    }
    gem_copy_map_cleanup(&map);
    return g[i];
}

static int gem_uf_find(int *uf, int x) {
    while (uf[x] != x) { uf[x] = uf[uf[x]]; x = uf[x]; }
    return x;
}

static void gem_uf_union(int *uf, int a, int b) {
    a = gem_uf_find(uf, a);
    b = gem_uf_find(uf, b);
    if (a != b) uf[a < b ? b : a] = a < b ? a : b;
}

static int gem_int_cmp(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

/* Snapshot every heavy slot of p that has no unit (see the overview). */
static void gem_mod_build_units(GemProcess *p) {
    int n = gem_n_globals;
    GemVal *g = p->globals;
    GemModUnit **out = GEM_GLOBALS_OUT(g);
    int *R = NULL;
    char *inR = NULL;
    int nr = 0;
    for (int i = 0; i < n; i++) {
        if (g[i].type == VAL_LAZY || out[i] || !gem_mod_heavy(g[i])) continue;
        if (!R) {
            R = (int *)malloc(sizeof(int) * (size_t)n);
            inR = (char *)calloc((size_t)n, 1);
            if (!R || !inR) { fprintf(stderr, "gem: out of memory (spawn)\n"); exit(1); }
        }
        R[nr++] = i;
        inR[i] = 1;
    }
    if (nr == 0) return;

    /* 1. Components: walk the values; an object reached from two slots
       unites them. A table of a still-valid unit pulls that unit in. */
    int *uf = (int *)malloc(sizeof(int) * (size_t)n);
    for (int i = 0; i < n; i++) uf[i] = i;
    GemCopyMap owner;  /* object -> first slot that reached it, +1 */
    gem_copy_map_init(&owner, 1);
    GemVal *stack = NULL;
    size_t sn = 0, scap = 0;
#define GEM_MOD_PUSH(v) do { GemVal _v = (v); \
        if (_v.type == VAL_TABLE || _v.type == VAL_BUFFER || (_v.type == VAL_FN && _v.env)) { \
            if (sn == scap) { scap = scap ? scap * 2 : 64; stack = (GemVal *)realloc(stack, sizeof(GemVal) * scap); \
                              if (!stack) { fprintf(stderr, "gem: out of memory (spawn)\n"); exit(1); } } \
            stack[sn++] = _v; } } while (0)
    for (int ri = 0; ri < nr; ri++) {
        int i = R[ri];
        GEM_MOD_PUSH(g[i]);
        while (sn > 0) {
            GemVal v = stack[--sn];
            void *key = v.type == VAL_TABLE ? (void *)v.table
                      : v.type == VAL_BUFFER ? (void *)v.buffer : v.env;
            void *o = gem_copy_map_find(&owner, key);
            if (o) { gem_uf_union(uf, i, (int)(intptr_t)o - 1); continue; }
            gem_copy_map_add(&owner, key, (void *)(intptr_t)(i + 1));
            if (v.type == VAL_TABLE) {
                GemTable *t = v.table;
                if (t->snap_gen) {
                    GemModUnit *u = gem_mod_live_find(p, t->snap_gen);
                    if (u) {
                        for (int k = 0; k < u->n; k++) {
                            int s = u->slots[k];
                            if (out[s] == u && !inR[s]) { R[nr++] = s; inR[s] = 1; }
                        }
                        gem_mod_unshare(p, u);
                    }
                }
                for (int j = 0; j < t->len; j++) {
                    GEM_MOD_PUSH(t->keys[j]);
                    GEM_MOD_PUSH(t->vals[j]);
                }
            } else if (v.type == VAL_FN) {
                intptr_t nf = *(intptr_t *)v.env;
                GemVal **fields = (GemVal **)((char *)v.env + sizeof(intptr_t));
                for (intptr_t f = 0; f < nf; f++) {
                    void *ob = gem_copy_map_find(&owner, fields[f]);
                    if (ob) { gem_uf_union(uf, i, (int)(intptr_t)ob - 1); continue; }
                    gem_copy_map_add(&owner, fields[f], (void *)(intptr_t)(i + 1));
                    GEM_MOD_PUSH(*fields[f]);
                }
            }
        }
    }
#undef GEM_MOD_PUSH
    free(stack);
    gem_copy_map_cleanup(&owner);

    /* 2. One unit per component, slots ascending. */
    qsort(R, (size_t)nr, sizeof(int), gem_int_cmp);
    int *cnt = (int *)calloc((size_t)n, sizeof(int));
    GemModUnit **unit_of = (GemModUnit **)calloc((size_t)n, sizeof(GemModUnit *));
    for (int ri = 0; ri < nr; ri++) cnt[gem_uf_find(uf, R[ri])]++;
    for (int ri = 0; ri < nr; ri++) {
        int r = gem_uf_find(uf, R[ri]);
        GemModUnit *u = unit_of[r];
        if (!u) {
            u = (GemModUnit *)calloc(1, sizeof(GemModUnit));
            u->slots = (int *)malloc(sizeof(int) * (size_t)cnt[r]);
            u->vals = (GemVal *)malloc(sizeof(GemVal) * (size_t)cnt[r]);
            unit_of[r] = u;
        }
        u->slots[u->n++] = R[ri];
    }

    /* 3. Copy each unit, stamping its source tables. */
    for (int ri = 0; ri < nr; ri++) {
        GemModUnit *u = unit_of[gem_uf_find(uf, R[ri])];
        if (u->gen) continue;
        if (++gem_mod_gen_counter == 0) ++gem_mod_gen_counter;
        u->gen = gem_mod_gen_counter;
        GemCopyMap map;
        gem_copy_map_init(&map, 1);
        map.stamp_gen = u->gen;
        map.snap_src = p;
        map.src_pid = (int)(p - gem_proc_table);
        for (int k = 0; k < u->n; k++) u->vals[k] = gem_deep_copy_internal(g[u->slots[k]], &map);
        u->shareable = !map.saw_mutable;
        gem_copy_map_cleanup(&map);
        for (int k = 0; k < u->n; k++) {
            out[u->slots[k]] = u;
            u->refs++;
        }
        if (p->mod_live_n == p->mod_live_cap) {
            p->mod_live_cap = p->mod_live_cap ? p->mod_live_cap * 2 : 16;
            p->mod_live = (GemModUnit **)realloc(p->mod_live, sizeof(GemModUnit *) * (size_t)p->mod_live_cap);
            if (!p->mod_live) { fprintf(stderr, "gem: out of memory (spawn)\n"); exit(1); }
        }
        p->mod_live[p->mod_live_n++] = u;
    }
    free(cnt);
    free(unit_of);
    free(uf);
    free(R);
    free(inR);
}

void gem_spawn_module_state(GemVal *fn_val, GemVal *cg, int parent) {
    GemProcess *p = NULL;
    if (parent >= 0 && gem_n_globals > 0 && gem_proc_table[parent].globals) {
        p = &gem_proc_table[parent];
        gem_mod_build_units(p);
    }
    GemVal *pg = p ? p->globals : NULL;
    GemModUnit **cin = GEM_GLOBALS_IN(cg);
    char *eager = NULL;
    if (fn_val) {
        /* The closure env and the module slots it shares structure with are
           copied with one map, so a captured local that refers to a module
           table is still that table in the child. Those units are valid, so
           the parent's slots equal their snapshot and are copied directly. */
        GemCopyMap map;
        gem_copy_map_init(&map, 0);
        map.probe = p != NULL;
        map.src_pid = parent;
        *fn_val = gem_deep_copy_internal(*fn_val, &map);
        for (int gi = 0; gi < map.probe_n; gi++) {
            GemModUnit *u = gem_mod_live_find(p, map.probe_gens[gi]);
            if (!u) continue;  /* a stale stamp */
            if (!eager) eager = (char *)calloc((size_t)gem_n_globals, 1);
            for (int k = 0; k < u->n; k++) {
                int s = u->slots[k];
                if (eager[s]) continue;
                eager[s] = 1;
                cg[s] = gem_deep_copy_internal(pg[s], &map);  /* may add gens */
            }
        }
        gem_copy_map_cleanup(&map);
    }
    if (!p) { free(eager); return; }
    GemModUnit **pout = GEM_GLOBALS_OUT(pg), **pin = GEM_GLOBALS_IN(pg);
    for (int i = 0; i < gem_n_globals; i++) {
        if (eager && eager[i]) continue;
        GemVal v = pg[i];
        if (v.type == VAL_LAZY) {
            cg[i] = v;
            cin[i] = pin[i];
            pin[i]->refs++;
        } else if (gem_mod_heavy(v)) {
            GemModUnit *u = pout[i];
            GemVal uv = u->vals[gem_mod_unit_index(u, i)];
            cg[i] = uv.type == VAL_STRING ? uv : GEM_LAZY_VAL;
            cin[i] = u;
            u->refs++;
        } else if (v.type == VAL_STRING) {
            cg[i] = gem_string_with_len(v.sval, v.slen);  /* child's arena */
        } else {
            cg[i] = v;
        }
    }
    free(eager);
    for (int k = 0; k < p->mod_live_n; ) {
        GemModUnit *u = p->mod_live[k];
        if (!u->shareable || !gem_mod_lazy) gem_mod_unshare(p, u);  /* removes it from mod_live */
        else k++;
    }
    if (!gem_mod_lazy) {
        GemVal *saved = gem_cur_globals;
        gem_cur_globals = cg;
        for (int i = 0; i < gem_n_globals; i++)
            if (cg[i].type == VAL_LAZY) gem_global_fault(i);
        gem_cur_globals = saved;
    }
}

void gem_globals_free(GemProcess *p) {
    if (!p->globals) return;
    while (p->mod_live_n > 0) gem_mod_unshare(p, p->mod_live[0]);
    GemModUnit **out = GEM_GLOBALS_OUT(p->globals), **in = GEM_GLOBALS_IN(p->globals);
    for (int i = 0; i < gem_n_globals; i++) {
        if (out[i]) { gem_mod_unit_release(out[i]); out[i] = NULL; }  /* not reached: unshared above */
        if (in[i]) { gem_mod_unit_release(in[i]); in[i] = NULL; }
    }
    free(p->mod_live);
    p->mod_live = NULL;
    p->mod_live_cap = 0;
    free(p->globals);
    p->globals = NULL;
}
