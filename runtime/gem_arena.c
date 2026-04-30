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
    block->next = NULL;
    block->cap = cap;
    block->used = 0;
    return block;
}

GemArena *gem_current_arena(void) {
    if (gem_current_pid >= 0)
        return &gem_proc_table[gem_current_pid].arena;
    return &gem_global_arena;
}

void gem_arena_init(GemArena *arena) {
    GemArenaBlock *block = gem_arena_new_block(GEM_ARENA_INITIAL_BLOCK);
    arena->current = block;
    arena->head = block;
    arena->table_list = NULL;
    arena->lo = block->data;
    arena->hi = block->data + block->cap;
    arena->bytes_allocated = 0;
}

void *gem_arena_alloc(GemArena *arena, size_t size) {
    size = (size + (GEM_ARENA_ALIGN - 1)) & ~(GEM_ARENA_ALIGN - 1);
    if (size == 0) size = GEM_ARENA_ALIGN;
    arena->bytes_allocated += size;

    GemArenaBlock *block = arena->current;
    if (block->used + size <= block->cap) {
        void *ptr = block->data + block->used;
        block->used += size;
        memset(ptr, 0, size);
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

    void *ptr = new_block->data + new_block->used;
    new_block->used += size;
    memset(ptr, 0, size);
    return ptr;
}

int gem_in_main_arena(const void *ptr) {
    if (gem_main_pid < 0) return 0;
    GemArena *arena = &gem_proc_table[gem_main_pid].arena;
    const char *p = (const char *)ptr;
    if (p < arena->lo || p >= arena->hi) return 0;
    for (GemArenaBlock *block = arena->head; block; block = block->next) {
        if (p >= block->data && p < block->data + block->used)
            return 1;
    }
    return 0;
}

void gem_arena_destroy(GemArena *arena) {
    GemTable *t = arena->table_list;
    while (t) {
        GemTable *next = t->arena_next;
        if (t->str_index) {
            shfree(t->str_index);
            t->str_index = NULL;
        }
        t = next;
    }
    arena->table_list = NULL;

    GemArenaBlock *block = arena->head;
    while (block) {
        GemArenaBlock *next = block->next;
        size_t total = sizeof(GemArenaBlock) + block->cap;
        munmap(block, total);
        block = next;
    }
    arena->head = NULL;
    arena->current = NULL;
}
