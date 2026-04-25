/*
 * Prototype: Boehm GC + minicoro coroutine integration
 *
 * Original plan used libdill, but it's abandoned (2019) and crashes on
 * arm64 macOS. minicoro is a single-header coroutine library that works
 * everywhere. We build channels on top.
 *
 * Key question: can Boehm GC scan minicoro's coroutine stacks for roots
 * so that GC-allocated objects reachable only from a coroutine survive?
 *
 * Strategy: allocate coroutine stacks, register with GC_add_roots().
 * minicoro lets us provide our own stack memory via mco_desc.allocator_data.
 *
 * Tests:
 *   1. Basic GC in main context
 *   2. GC objects survive inside coroutines
 *   3. Cross-coroutine references survive collection
 *   4. Stress: many coroutines x many allocs
 */

#define MCO_API static
#define MINICORO_IMPL
#include "minicoro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc.h>

/* ---------- GC-aware coroutine stack management ---------- */

typedef struct {
    void *stack_mem;
    size_t stack_size;
} gc_stack_t;

/* Custom allocator for minicoro that registers stacks as GC roots */
static void *coro_stack_alloc(size_t size, void *udata) {
    (void)udata;
    void *ptr = malloc(size);
    if (!ptr) return NULL;
    GC_add_roots(ptr, (char *)ptr + size);
    return ptr;
}

static void coro_stack_free(void *ptr, size_t size, void *udata) {
    (void)udata;
    GC_remove_roots(ptr, (char *)ptr + size);
    free(ptr);
}

/* Helper: create a coroutine with GC-registered stack */
static mco_coro *create_coro(void (*func)(mco_coro *), void *user_data,
                              size_t stack_size) {
    mco_desc desc = mco_desc_init(func, stack_size);
    desc.alloc_cb = coro_stack_alloc;
    desc.dealloc_cb = coro_stack_free;
    desc.user_data = user_data;

    mco_coro *co;
    mco_result res = mco_create(&co, &desc);
    if (res != MCO_SUCCESS) {
        fprintf(stderr, "mco_create failed: %s\n", mco_result_description(res));
        abort();
    }
    return co;
}

/* Helper: run a coroutine to completion, pumping it manually */
static void run_coro(mco_coro *co) {
    while (mco_status(co) == MCO_SUSPENDED) {
        mco_result res = mco_resume(co);
        if (res != MCO_SUCCESS) {
            fprintf(stderr, "mco_resume failed: %s\n", mco_result_description(res));
            abort();
        }
    }
}

/* ---------- test 1: basic GC ---------- */

static void test_basic_gc(void) {
    printf("test 1: basic GC allocation... ");

    char *s = GC_malloc(128);
    snprintf(s, 128, "hello from GC");

    GC_gcollect();

    if (strcmp(s, "hello from GC") == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
        exit(1);
    }
}

/* ---------- test 2: GC objects survive in a coroutine ---------- */

static void test2_func(mco_coro *co) {
    /* Allocate GC object inside the coroutine */
    char *s = GC_malloc(256);
    snprintf(s, 256, "allocated-in-coroutine-%d", 42);

    /* Force GC cycles while yielding — object only reachable from
       this coroutine's stack */
    for (int i = 0; i < 5; i++) {
        GC_gcollect();
        mco_yield(co);
    }

    /* Check it survived */
    int *result = (int *)mco_get_user_data(co);
    *result = (strcmp(s, "allocated-in-coroutine-42") == 0) ? 1 : 0;
}

static void test_gc_in_coroutine(void) {
    printf("test 2: GC objects survive in coroutine... ");

    int result = 0;
    mco_coro *co = create_coro(test2_func, &result, 0);
    run_coro(co);
    mco_destroy(co);

    if (result) {
        printf("PASS\n");
    } else {
        printf("FAIL - GC collected coroutine-local object!\n");
        exit(1);
    }
}

/* ---------- test 3: cross-coroutine references ---------- */

struct shared_obj {
    int value;
    char name[64];
};

struct test3_ctx {
    struct shared_obj *shared;
    int result;
};

static void test3_func(mco_coro *co) {
    struct test3_ctx *ctx = (struct test3_ctx *)mco_get_user_data(co);
    struct shared_obj *local_ref = ctx->shared;

    /* Force heavy GC while only this coroutine holds the reference */
    for (int i = 0; i < 10; i++) {
        GC_gcollect();
        mco_yield(co);
    }

    ctx->result = (local_ref->value == 999 &&
                   strcmp(local_ref->name, "cross-coro-test") == 0) ? 1 : 0;
}

static void test_cross_coroutine_refs(void) {
    printf("test 3: cross-coroutine GC references... ");

    struct shared_obj *obj = GC_malloc(sizeof(struct shared_obj));
    obj->value = 999;
    snprintf(obj->name, 64, "cross-coro-test");

    struct test3_ctx ctx = { .shared = obj, .result = 0 };
    mco_coro *co = create_coro(test3_func, &ctx, 0);

    /* Resume once so the coroutine captures the pointer on its stack */
    mco_resume(co);

    /* Drop main's reference — only the coroutine stack has it now */
    obj = NULL;
    GC_gcollect();

    /* Finish the coroutine */
    run_coro(co);
    mco_destroy(co);

    if (ctx.result) {
        printf("PASS\n");
    } else {
        printf("FAIL - shared object collected while coroutine held it!\n");
        exit(1);
    }
}

/* ---------- test 4: stress test ---------- */

#define NUM_COROS 50
#define ALLOCS_PER_CORO 200

struct test4_ctx {
    int id;
    int result;
};

static void test4_func(mco_coro *co) {
    struct test4_ctx *ctx = (struct test4_ctx *)mco_get_user_data(co);

    char **ptrs = GC_malloc(sizeof(char *) * ALLOCS_PER_CORO);

    for (int i = 0; i < ALLOCS_PER_CORO; i++) {
        ptrs[i] = GC_malloc(64);
        snprintf(ptrs[i], 64, "coro-%d-alloc-%d", ctx->id, i);
        if (i % 20 == 0) {
            GC_gcollect();
            mco_yield(co);
        }
    }

    /* Verify all survived */
    ctx->result = 1;
    for (int i = 0; i < ALLOCS_PER_CORO; i++) {
        char expected[64];
        snprintf(expected, 64, "coro-%d-alloc-%d", ctx->id, i);
        if (strcmp(ptrs[i], expected) != 0) {
            ctx->result = 0;
            break;
        }
    }
}

static void test_many_coroutines(void) {
    printf("test 4: %d coroutines x %d allocs each... ", NUM_COROS, ALLOCS_PER_CORO);

    mco_coro *coros[NUM_COROS];
    struct test4_ctx ctxs[NUM_COROS];

    for (int i = 0; i < NUM_COROS; i++) {
        ctxs[i].id = i;
        ctxs[i].result = 0;
        coros[i] = create_coro(test4_func, &ctxs[i], 0);
    }

    /* Round-robin schedule: resume each coroutine in turn until all done */
    int active = NUM_COROS;
    while (active > 0) {
        active = 0;
        for (int i = 0; i < NUM_COROS; i++) {
            if (mco_status(coros[i]) == MCO_SUSPENDED) {
                mco_resume(coros[i]);
                active++;
            }
        }
    }

    int all_passed = 1;
    for (int i = 0; i < NUM_COROS; i++) {
        if (!ctxs[i].result) {
            printf("FAIL - coroutine %d lost objects\n", i);
            all_passed = 0;
        }
        mco_destroy(coros[i]);
    }

    if (all_passed) {
        printf("PASS\n");
    } else {
        exit(1);
    }
}

/* ---------- test 5: WITHOUT GC roots registration (negative test) ------- */

static void *bad_stack_alloc(size_t size, void *udata) {
    (void)udata;
    /* Deliberately do NOT register with GC */
    return malloc(size);
}

static void bad_stack_free(void *ptr, size_t size, void *udata) {
    (void)udata;
    (void)size;
    free(ptr);
}

static void test5_func(mco_coro *co) {
    /* Allocate many GC objects, only reachable from this unregistered stack */
    volatile char *ptrs[50];
    for (int i = 0; i < 50; i++) {
        ptrs[i] = GC_malloc(128);
        snprintf((char *)ptrs[i], 128, "unregistered-coro-alloc-%d", i);
    }

    /* Aggressive GC */
    for (int i = 0; i < 20; i++) {
        GC_gcollect();
        mco_yield(co);
    }

    /* Check if any got corrupted (they might, since GC can't see this stack) */
    int *result = (int *)mco_get_user_data(co);
    *result = 1;  /* assume pass */
    for (int i = 0; i < 50; i++) {
        char expected[128];
        snprintf(expected, 128, "unregistered-coro-alloc-%d", i);
        if (strcmp((char *)ptrs[i], expected) != 0) {
            *result = 0;  /* corrupted — GC collected it */
            break;
        }
    }
}

static void test_without_registration(void) {
    printf("test 5: unregistered stack (negative test)... ");

    int result = 0;
    mco_desc desc = mco_desc_init(test5_func, 0);
    desc.alloc_cb = bad_stack_alloc;
    desc.dealloc_cb = bad_stack_free;
    desc.user_data = &result;

    mco_coro *co;
    mco_create(&co, &desc);
    run_coro(co);
    mco_destroy(co);

    if (!result) {
        printf("EXPECTED FAIL (proves GC root registration is necessary)\n");
    } else {
        printf("PASS (GC happened not to collect — conservative GC can be lucky)\n");
        printf("       (this doesn't mean registration is unnecessary!)\n");
    }
}

/* ---------- main ---------- */

int main(void) {
    GC_INIT();

    printf("=== Boehm GC + minicoro integration prototype ===\n");
    printf("GC version: %d.%d.%d\n",
           GC_VERSION_MAJOR, GC_VERSION_MINOR, GC_VERSION_MICRO);
    printf("\n");

    test_basic_gc();
    test_gc_in_coroutine();
    test_cross_coroutine_refs();
    test_many_coroutines();
    test_without_registration();

    printf("\n=== Results ===\n");
    printf("All critical tests passed.\n\n");
    printf("Architecture for Gem runtime:\n");
    printf("  - Use minicoro (single header) instead of libdill for coroutines\n");
    printf("  - Custom stack allocator: malloc + GC_add_roots()\n");
    printf("  - Custom stack deallocator: GC_remove_roots() + free()\n");
    printf("  - Build channels/mailboxes on top of minicoro yields\n");
    printf("  - Round-robin scheduler resumes suspended coroutines\n");

    return 0;
}
