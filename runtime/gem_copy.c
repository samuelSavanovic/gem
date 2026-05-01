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

/* Unpinned-roots wrapper. Caller asserts that no malloc-backed pinned
   box (gem_box_alloc) is reachable except through `roots[]` — any pinned
   box not encountered during the env-walk will be swept and freed.
   Two callers today:
     - compile_while emits this with (NULL, 0) when liveness's rescue set
       is empty: the loop body has no live locals and no pin-set entries
       to migrate, so the call is just for the bulk-free side effect.
     - emit_tco_continue emits this with the tail-call args as roots, but
       only when the TCO function has zero malloc-boxed locals (guard at
       codegen.gem:2420). For functions that DO capture+mutate locals, the
       TCO reset is skipped entirely. Lifting that restriction is tracked
       under "TCO reset with pinned roots" in docs/OPTIMIZATIONS.md. */
void gem_arena_reset_with_roots(GemVal **roots, int n_roots) {
    gem_arena_reset_with_roots_pinned(roots, n_roots, NULL, 0);
}
