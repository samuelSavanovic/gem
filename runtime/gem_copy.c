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

/* gem_shape_counter is defined in gem_core.c — used by gem_deep_copy_table to
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
typedef struct {
    GemCopyEntry inline_buf[GEM_COPY_MAP_INLINE];
    GemCopyEntry *table;   /* NULL while len <= GEM_COPY_MAP_INLINE */
    size_t tcap;
    int len;
    int use_malloc;
    /* When `preserve_external` is set (arena resets), only objects inside
       `region` -- the memory about to be freed -- are copied; any pointer
       outside it (older arena memory, malloc'd pinned boxes, immortal
       module tables) is kept as is, so its identity is preserved. */
    int preserve_external;
    const GemRegion *region;
} GemCopyMap;

static void gem_copy_map_init(GemCopyMap *map, int use_malloc) {
    map->table = NULL;
    map->tcap = 0;
    map->len = 0;
    map->use_malloc = use_malloc;
    map->preserve_external = 0;
    map->region = NULL;
}

static int gem_copy_is_external(GemCopyMap *map, const void *ptr) {
    if (!map->preserve_external) return 0;
    return !gem_region_contains(map->region, ptr);
}

static void gem_copy_map_cleanup(GemCopyMap *map) {
    free(map->table);
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

static GemVal gem_deep_copy_internal(GemVal val, GemCopyMap *map);

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
        nt->mut_seq = gem_mut_clock;
    }

    for (int i = 0; i < t->len; i++) {
        nt->keys[i] = gem_deep_copy_internal(t->keys[i], map);
        nt->vals[i] = gem_deep_copy_internal(t->vals[i], map);
    }
    /* The string-key index is built on first use (gem_table_index): many
       copies (spawned module state, messages) are never looked up by key. */
    nt->index_stale = (t->str_index != NULL || t->index_stale);

    GemVal r; r.type = VAL_TABLE; r.magic = GEM_MAGIC; r.table = nt; return r;
}

static GemVal gem_deep_copy_fn(GemVal fn_val, GemCopyMap *map) {
    if (!fn_val.env) return fn_val;
    /* An env outside the reset region was built before the mark, so its box
       pointers (set once at closure creation) also predate it: keep it. */
    if (gem_copy_is_external(map, fn_val.env)) return fn_val;

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
            if (existing) { GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = (char *)existing; r.slen = val.slen; return r; }
            GemVal r;
            r.type = VAL_STRING;
            r.magic = GEM_MAGIC;
            r.sval = gem_copy_strdup(map, val.sval, val.slen);
            r.slen = val.slen;
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
            if (gem_copy_is_external(map, val.buffer)) return val;
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
     3. Boxes allocated before the reset's mark (seq < mark.pin_seq) can be
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
 * reachable part of the memory allocated since the mark (the "region") into
 * fresh blocks and frees the rest of the region. Memory older than the mark
 * is never moved or freed.
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
 *   3. Older mutable objects: tables (key/val arrays and entries), buffers
 *      (data), pinned boxes (contents), mailbox nodes. Each arena keeps a
 *      list of its tables and buffers, newest first, and the mark records
 *      the list heads and the pinned-box sequence number, so every such
 *      object older than the mark is found and fixed up in place. Arena
 *      closure envs and capture boxes are written only when created, and
 *      strings are immutable, so older ones cannot point into the region.
 *   4. Process state: module slots (proc->globals), the mailbox, read_buf.
 *   5. Other processes never hold pointers into this arena (spawn and send
 *      deep-copy; frozen module tables are immortal malloc copies).
 *
 * Nested loops nest their marks: a reset only frees memory newer than its
 * own mark, so marks held by enclosing loops (all older) stay valid.
 *
 * Hysteresis: the next reset for this mark fires after the loop allocates
 * max(GEM_ARENA_RESET_THRESHOLD, 2 * bytes copied + bytes scanned / 2), so
 * each reset's work is paid for by at least as much fresh allocation. */

/* GEM_DIAG=1 reports reset counts and volumes at exit. */
static uint64_t gem_diag_resets, gem_diag_copied, gem_diag_scanned, gem_diag_freed;
static double gem_diag_t_total, gem_diag_t_walk;
#include <time.h>
static double gem_diag_now(void) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return ts.tv_sec + ts.tv_nsec * 1e-9; }
static int gem_diag_state = -1;

static void gem_diag_reset_report(void) {
    fprintf(stderr, "gem_diag: arena_resets=%llu copied=%llu scanned=%llu freed=%llu time=%.3fs walk=%.3fs\n",
            (unsigned long long)gem_diag_resets, (unsigned long long)gem_diag_copied,
            (unsigned long long)gem_diag_scanned, (unsigned long long)gem_diag_freed,
            gem_diag_t_total, gem_diag_t_walk);
}

static void gem_diag_note_reset(size_t copied, size_t scanned, size_t freed) {
    if (gem_diag_state < 0) {
        const char *e = getenv("GEM_DIAG");
        gem_diag_state = (e && e[0] == '1');
        if (gem_diag_state) atexit(gem_diag_reset_report);
    }
    if (!gem_diag_state) return;
    gem_diag_resets++;
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

static void gem_region_reset_impl(GemArenaMark *mark, GemVal **roots, int n_roots,
                                  GemVal **pinned_roots, int n_pinned) {
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    GemArena *arena = &proc->arena;
    GemArenaBlock *mb = mark->block;

    /* The region: mb's tail past the mark, plus every later block. */
    GemArenaBlock *post_blocks = mb->next;
    GemRegion region;
    gem_region_build(&region, mb->data + mark->used, mb->data + mb->used, post_blocks);

    /* Detach the region's blocks; copies go to a fresh block after mb. */
    mb->next = NULL;
    gem_arena_append_block(arena, mb, 0);

    GemTable *post_tables = arena->table_list;
    arena->table_list = mark->tables;
    GemBuffer *post_buffers = arena->buffer_list;
    arena->buffer_list = mark->buffers;
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
    /* Pinned boxes older than the mark may be reachable from callers. */
    if (proc->pinned_boxes) {
        size_t n = hmlenu(proc->pinned_boxes);
        scanned += n * sizeof(GemPinEntry);
        for (size_t i = 0; i < n; i++) {
            if (proc->pinned_boxes[i].seq < mark->pin_seq && !proc->pinned_boxes[i].value) {
                proc->pinned_boxes[i].value = 1;
                GemVal *box = (GemVal *)proc->pinned_boxes[i].key;
                *box = gem_deep_copy_internal(*box, &map);
            }
        }
    }
    /* Tables older than the mark written since it: the suffix of the
       remembered log with epoch >= mark->clock (see gem_table_written).
       Region tables in it are skipped (garbage, or copied above). */
    double tw0 = gem_diag_state > 0 ? gem_diag_now() : 0;
    size_t lo = 0, hi = arena->rem_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arena->rem[mid].epoch < mark->clock) lo = mid + 1; else hi = mid;
    }
    size_t rem_start = lo;
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
        int rekey = 0;
        for (int i = 0; i < t->len; i++) {
            if (gem_val_in_region(&map, t->keys[i])) {
                t->keys[i] = gem_deep_copy_internal(t->keys[i], &map);
                rekey = 1;
            }
            if (gem_val_in_region(&map, t->vals[i])) {
                t->vals[i] = gem_deep_copy_internal(t->vals[i], &map);
            }
        }
        if (rekey && (t->str_index || t->index_stale)) {
            /* str_index stores key pointers: rebuild it on next use. */
            if (t->str_index) shfree(t->str_index);
            t->str_index = NULL;
            t->index_stale = 1;
        }
    }
    /* Compact the suffix: drop region tables (about to be unmapped) and
       duplicates. A kept entry still has epoch >= mark->clock, so this and
       every enclosing mark (all older) keep finding it. */
    {
        size_t w = rem_start;
        for (size_t ri = rem_start; ri < arena->rem_len; ri++) {
            GemTable *t = arena->rem[ri].t;
            if (gem_region_contains(&region, t) || !t->rem_flag) continue;
            t->rem_flag = 0;
            arena->rem[w++] = arena->rem[ri];
        }
        arena->rem_len = w;
    }
    if (gem_diag_state > 0) gem_diag_t_walk += gem_diag_now() - tw0;
    /* Buffers older than the mark. */
    for (GemBuffer *b = mark->buffers; b; b = b->arena_next) {
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

    gem_pin_sweep_from(proc, mark->pin_seq);

    /* Region tables are garbage now (live ones were copied): free their
       malloc'd string indexes, then unmap the region's blocks. */
    for (GemTable *t = post_tables; t && t != mark->tables; t = t->arena_next) {
        if (t->str_index) {
            shfree(t->str_index);
            t->str_index = NULL;
        }
    }
    (void)post_buffers;
    size_t region_bytes = 0;
    for (int i = 0; i < region.n; i++) region_bytes += (size_t)(region.ranges[i].hi - region.ranges[i].lo);
    gem_arena_free_blocks(post_blocks);
    mb->used = mark->used;
    free(region.ranges);

    size_t copied = arena->bytes_allocated - bytes_before;
    gem_diag_note_reset(copied, scanned, region_bytes);
    size_t budget = 2 * copied + scanned / 2;
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
    gem_region_reset_impl(mark, roots, n_roots, pinned_roots, n_pinned);
    if (gem_diag_state > 0) gem_diag_t_total += gem_diag_now() - t0;
}

/* ─── Module globals ─── */

GemVal *gem_cur_globals = NULL;
int gem_n_globals = 0;

void gem_globals_init(int n) {
    gem_n_globals = n;
}

GemVal *gem_globals_alloc(void) {
    GemVal *g = (GemVal *)malloc(sizeof(GemVal) * (size_t)(gem_n_globals > 0 ? gem_n_globals : 1));
    for (int i = 0; i < gem_n_globals; i++) g[i] = GEM_NIL;
    return g;
}

void gem_spawn_copy(GemVal *fn_val, GemVal *dst, const GemVal *src, int n) {
    GemCopyMap map;
    gem_copy_map_init(&map, 0);
    if (fn_val) *fn_val = gem_deep_copy_internal(*fn_val, &map);
    for (int i = 0; i < n; i++) dst[i] = gem_deep_copy_internal(src[i], &map);
    gem_copy_map_cleanup(&map);
}

