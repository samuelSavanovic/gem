/*
 * gem_arena.c — Per-process bump allocator for Gem values.
 *
 * Blocks are mmap'd MAP_ANON pages: when a process exits and its arena is
 * destroyed, munmap returns the pages to the OS so RSS drops immediately.
 * (free() on macOS does not reliably return small malloc'd blocks.)
 */

#include <sys/mman.h>
#include <unistd.h>

#include "gem.h"
#include "stb_ds.h"

GemArena gem_global_arena;

#define GEM_ARENA_INITIAL_BLOCK (64 * 1024)
#define GEM_ARENA_MAX_BLOCK    (1024 * 1024)
#define GEM_ARENA_ALIGN        16

static size_t gem_arena_page_size(void) {
    static size_t cached = 0;
    if (cached == 0) {
        long ps = sysconf(_SC_PAGESIZE);
        cached = (ps > 0) ? (size_t)ps : 4096;
    }
    return cached;
}

/* Memory mappings the runtime holds (arena blocks and process stacks).
   spawn compares it against the system's limit (gem_scheduler.c,
   gem_map_limit) so that hitting the limit fails a spawn, not a later
   allocation. */
long gem_runtime_maps = 0;

static GemArenaBlock *gem_arena_new_block(size_t min_cap) {
    size_t cap = min_cap;
    if (cap < GEM_ARENA_INITIAL_BLOCK) cap = GEM_ARENA_INITIAL_BLOCK;
    size_t total = sizeof(GemArenaBlock) + cap;
    size_t ps = gem_arena_page_size();
    total = (total + ps - 1) & ~(ps - 1);
    cap = total - sizeof(GemArenaBlock);

    GemArenaBlock *block = (GemArenaBlock *)mmap(
        NULL, total, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANON, -1, 0);
    if (block == MAP_FAILED) {
        fprintf(stderr, "gem_arena: mmap failed (size=%zu)\n", total);
        exit(1);
    }
    gem_runtime_maps++;
    block->next = NULL;
    block->cap = cap;
    block->used = 0;
    block->dirty = 0;
    return block;
}

void gem_arena_init(GemArena *arena) {
    GemArenaBlock *block = gem_arena_new_block(GEM_ARENA_INITIAL_BLOCK);
    arena->current = block;
    arena->head = block;
    arena->table_list = NULL;
    arena->buffer_list = NULL;
    arena->lo = block->data;
    arena->hi = block->data + block->cap;
    arena->bytes_allocated = 0;
    arena->bytes_freed = 0;
    arena->rem = NULL;
    arena->rem_len = 0;
    arena->rem_cap = 0;
    arena->pin_seq = 0;
    arena->ret_min = 0;
    arena->ret_at = 0;
}

/* Fresh block chained after `after` and made current; used by region resets
   as the copy destination. */
GemArenaBlock *gem_arena_append_block(GemArena *arena, GemArenaBlock *after, size_t min_cap) {
    GemArenaBlock *nb = gem_arena_new_block(min_cap);
    after->next = nb;
    arena->current = nb;
    if (nb->data < arena->lo) arena->lo = nb->data;
    if (nb->data + nb->cap > arena->hi) arena->hi = nb->data + nb->cap;
    return nb;
}

void gem_arena_free_blocks(GemArenaBlock *block) {
    while (block) {
        GemArenaBlock *next = block->next;
        munmap(block, sizeof(GemArenaBlock) + block->cap);
        gem_runtime_maps--;
        block = next;
    }
}

void gem_strlen_error(size_t n, const char *who) {
    char buf[160];
    snprintf(buf, sizeof(buf), "%s%sa string of %zu bytes is over the limit of %d bytes",
             who ? who : "", who ? ": " : "", n, GEM_MAX_STRLEN);
    gem_error(buf);
}

void gem_buffer_reserve(GemBuffer *b, size_t extra, const char *who) {
    size_t need = (size_t)b->len + extra;
    if (need < (size_t)b->cap) return;
    gem_strlen_check(need, who);
    size_t cap = b->cap > 16 ? (size_t)b->cap : 16;
    while (cap <= need) cap *= 2;
    if (cap > (size_t)INT_MAX) cap = (size_t)INT_MAX;
    char *data = (char *)gem_alloc(cap);
    memcpy(data, b->data, (size_t)b->len);
    b->data = data;
    b->cap = (int)cap;
}

GemBuffer *gem_buffer_alloc(int cap) {
    GemArena *a = gem_current_arena();
    GemBuffer *b = (GemBuffer *)gem_arena_alloc(a, sizeof(GemBuffer));
    b->cap = cap;
    b->len = 0;
    b->data = (char *)gem_arena_alloc(a, (size_t)cap);
    b->arena_next = a->buffer_list;
    a->buffer_list = b;
    return b;
}

void *gem_arena_alloc(GemArena *arena, size_t size) {
    size = (size + (GEM_ARENA_ALIGN - 1)) & ~(GEM_ARENA_ALIGN - 1);
    if (size == 0) size = GEM_ARENA_ALIGN;
    arena->bytes_allocated += size;

    GemArenaBlock *block = arena->current;
    if (block->used + size <= block->cap) {
        void *ptr = block->data + block->used;
        /* Allocations come back zeroed; only memory a reset gave back
           needs clearing. */
        if (block->used < block->dirty) {
            size_t n = block->dirty - block->used;
            memset(ptr, 0, n < size ? n : size);
        }
        block->used += size;
        return ptr;
    }

    size_t next_cap = block->cap * 2;
    if (next_cap > GEM_ARENA_MAX_BLOCK) next_cap = GEM_ARENA_MAX_BLOCK;
    if (next_cap < size) next_cap = size;

    GemArenaBlock *new_block = gem_arena_new_block(next_cap);
    block->next = new_block;
    arena->current = new_block;

    if (new_block->data < arena->lo) arena->lo = new_block->data;
    char *end = new_block->data + new_block->cap;
    if (end > arena->hi) arena->hi = end;

    void *ptr = new_block->data + new_block->used;   /* fresh mmap: zeroed */
    new_block->used += size;
    return ptr;
}


size_t gem_arena_mapped_bytes(const GemArena *arena) {
    size_t n = 0;
    for (const GemArenaBlock *b = arena->head; b; b = b->next) n += sizeof(GemArenaBlock) + b->cap;
    return n;
}

void gem_arena_destroy(GemArena *arena) {
    GemTable *t = arena->table_list;
    while (t) {
        GemTable *next = t->arena_next;
        gem_str_index_free(&t->str_index);
        t = next;
    }
    arena->table_list = NULL;
    arena->buffer_list = NULL;
    free(arena->rem);
    arena->rem = NULL;
    arena->rem_len = arena->rem_cap = 0;

    gem_arena_free_blocks(arena->head);
    arena->head = NULL;
    arena->current = NULL;
}
