/*
 * gem_arena.c — Per-process bump allocator for Gem values.
 */

#include "gem.h"
#include "stb_ds.h"

GemArena gem_global_arena;

#define GEM_ARENA_INITIAL_BLOCK (64 * 1024)
#define GEM_ARENA_MAX_BLOCK    (1024 * 1024)
#define GEM_ARENA_ALIGN        16

static GemArenaBlock *gem_arena_new_block(size_t min_cap) {
    size_t cap = min_cap;
    if (cap < GEM_ARENA_INITIAL_BLOCK) cap = GEM_ARENA_INITIAL_BLOCK;
    GemArenaBlock *block = (GemArenaBlock *)malloc(sizeof(GemArenaBlock) + cap);
    if (!block) {
        fprintf(stderr, "gem_arena: out of memory\n");
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
}

void *gem_arena_alloc(GemArena *arena, size_t size) {
    size = (size + (GEM_ARENA_ALIGN - 1)) & ~(GEM_ARENA_ALIGN - 1);
    if (size == 0) size = GEM_ARENA_ALIGN;

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

    void *ptr = new_block->data + new_block->used;
    new_block->used += size;
    memset(ptr, 0, size);
    return ptr;
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
        free(block);
        block = next;
    }
    arena->head = NULL;
    arena->current = NULL;
}
