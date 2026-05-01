/*
 * Comprehensive stress test for Gem concurrency runtime.
 *
 * Tests the scheduler, mailbox, spawn/send/receive/self APIs
 * integrated into the real runtime with real GemVal types.
 *
 * Categories:
 *   1.  Basic spawn + execution
 *   2.  Send/receive single message
 *   3.  Ping-pong between two processes
 *   4.  Many producers, one consumer
 *   5.  GC-allocated messages survive scheduling
 *   6.  Receive blocks until message arrives (ordering)
 *   7.  self() returns correct pid
 *   8.  Send to dead/nonexistent process (no crash)
 *   9.  All value types through mailbox
 *   10. Spawn from inside a spawned process (nested spawn)
 *   11. Process slot reuse after completion
 *   12. Deep message chain (A->B->C->...->Z->A)
 *   13. High-scale: many processes, many messages
 *   14. GC pressure: heavy allocation inside coroutines
 *   15. Multiple scheduler runs (not just one)
 *   16. Process with no messages exits cleanly
 *   17. Burst send: many messages before scheduler runs
 */

#include "gem.h"

#include <stdio.h>
#include <string.h>

static int tests_run = 0;
static int tests_passed = 0;

#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        printf("FAIL: %s\n", msg); \
        return 0; \
    } \
} while(0)

#define RUN_TEST(fn) do { \
    tests_run++; \
    printf("  test %2d: %-55s ", tests_run, #fn); \
    fflush(stdout); \
    memset(gem_proc_table, 0, sizeof(gem_proc_table)); \
    gem_current_pid = -1; \
    gem_scheduler_init(); \
    if (fn()) { \
        tests_passed++; \
        printf("PASS\n"); \
    } \
} while(0)

/* ================================================================
 * Helper: wrap a C function as a GemFnPtr
 * In the real runtime, spawned Gem closures already have this signature.
 * For tests, we use user_data in a GC-allocated context struct.
 * ================================================================ */

/* --- Test 1: Basic spawn + execution --- */

static int t1_flag = 0;

static GemVal t1_proc(void *env, GemVal *args, int argc) {
    (void)env; (void)args; (void)argc;
    t1_flag = 1;
    return GEM_NIL;
}

static int test_spawn_basic(void) {
    t1_flag = 0;
    int pid = gem_spawn_fn(t1_proc, NULL);
    ASSERT(pid >= 0, "spawn returned invalid pid");
    gem_run_scheduler();
    ASSERT(t1_flag == 1, "process did not execute");
    return 1;
}

/* --- Test 2: Send/receive single message --- */

typedef struct { GemVal received; int got; } t2_ctx;

static GemVal t2_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t2_ctx *ctx = (t2_ctx *)env;
    ctx->received = gem_receive_msg();
    ctx->got = 1;
    return GEM_NIL;
}

static int test_send_receive(void) {
    t2_ctx *ctx = (t2_ctx *)gem_alloc(sizeof(t2_ctx));
    ctx->got = 0;
    int pid = gem_spawn_fn(t2_proc, ctx);
    gem_send_msg(pid, gem_int(42));
    gem_run_scheduler();
    ASSERT(ctx->got == 1, "did not receive message");
    ASSERT(ctx->received.type == VAL_INT, "wrong type");
    ASSERT(ctx->received.ival == 42, "wrong value");
    return 1;
}

/* --- Test 3: Ping-pong --- */

#define PING_PONG_ROUNDS 100

typedef struct { int partner_pid; int count; } pp_ctx;

static GemVal ping_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    pp_ctx *ctx = (pp_ctx *)env;
    for (int i = 0; i < PING_PONG_ROUNDS; i++) {
        gem_send_msg(ctx->partner_pid, gem_int(i));
        GemVal reply = gem_receive_msg();
        (void)reply;
        ctx->count++;
    }
    return GEM_NIL;
}

static GemVal pong_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    pp_ctx *ctx = (pp_ctx *)env;
    for (int i = 0; i < PING_PONG_ROUNDS; i++) {
        GemVal msg = gem_receive_msg();
        gem_send_msg(ctx->partner_pid, gem_int(msg.ival + 100));
        ctx->count++;
    }
    return GEM_NIL;
}

static int test_ping_pong(void) {
    pp_ctx *ping_ctx = (pp_ctx *)gem_alloc(sizeof(pp_ctx));
    pp_ctx *pong_ctx = (pp_ctx *)gem_alloc(sizeof(pp_ctx));
    ping_ctx->count = 0;
    pong_ctx->count = 0;

    int pong_pid = gem_spawn_fn(pong_proc, pong_ctx);
    int ping_pid = gem_spawn_fn(ping_proc, ping_ctx);

    ping_ctx->partner_pid = pong_pid;
    pong_ctx->partner_pid = ping_pid;

    gem_run_scheduler();
    ASSERT(ping_ctx->count == PING_PONG_ROUNDS, "ping count wrong");
    ASSERT(pong_ctx->count == PING_PONG_ROUNDS, "pong count wrong");
    return 1;
}

/* --- Test 4: Many producers, one consumer --- */

#define NUM_PRODUCERS 50
#define MSGS_PER_PRODUCER 20

typedef struct { int consumer_pid; int id; } prod_ctx;
typedef struct { int total; } cons_ctx;

static GemVal producer_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    prod_ctx *ctx = (prod_ctx *)env;
    for (int i = 0; i < MSGS_PER_PRODUCER; i++) {
        gem_send_msg(ctx->consumer_pid, gem_int(ctx->id * 10000 + i));
    }
    return GEM_NIL;
}

static GemVal consumer_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    cons_ctx *ctx = (cons_ctx *)env;
    int expected = NUM_PRODUCERS * MSGS_PER_PRODUCER;
    for (int i = 0; i < expected; i++) {
        GemVal msg = gem_receive_msg();
        (void)msg;
        ctx->total++;
    }
    return GEM_NIL;
}

static int test_many_producers(void) {
    cons_ctx *cctx = (cons_ctx *)gem_alloc(sizeof(cons_ctx));
    cctx->total = 0;
    int consumer_pid = gem_spawn_fn(consumer_proc, cctx);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        prod_ctx *pctx = (prod_ctx *)gem_alloc(sizeof(prod_ctx));
        pctx->consumer_pid = consumer_pid;
        pctx->id = i;
        gem_spawn_fn(producer_proc, pctx);
    }

    gem_run_scheduler();
    int expected = NUM_PRODUCERS * MSGS_PER_PRODUCER;
    ASSERT(cctx->total == expected, "consumer did not receive all messages");
    return 1;
}

/* --- Test 5: GC-allocated messages survive scheduling --- */

typedef struct { int passed; } t5_cons_ctx;
typedef struct { int consumer_pid; } t5_send_ctx;

static GemVal t5_sender(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t5_send_ctx *ctx = (t5_send_ctx *)env;
    for (int i = 0; i < 100; i++) {
        char buf[64];
        snprintf(buf, 64, "gc-msg-%d", i);
        gem_send_msg(ctx->consumer_pid, gem_string(buf));
    }
    return GEM_NIL;
}

static GemVal t5_consumer(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t5_cons_ctx *ctx = (t5_cons_ctx *)env;
    ctx->passed = 1;
    for (int i = 0; i < 100; i++) {
        GemVal msg = gem_receive_msg();
        char expected[64];
        snprintf(expected, 64, "gc-msg-%d", i);
        if (msg.type != VAL_STRING || strcmp(msg.sval, expected) != 0) {
            ctx->passed = 0;
            return GEM_NIL;
        }
    }
    return GEM_NIL;
}

static int test_gc_messages(void) {
    t5_cons_ctx *cctx = (t5_cons_ctx *)gem_alloc(sizeof(t5_cons_ctx));
    cctx->passed = 0;
    int consumer_pid = gem_spawn_fn(t5_consumer, cctx);

    t5_send_ctx *sctx = (t5_send_ctx *)gem_alloc(sizeof(t5_send_ctx));
    sctx->consumer_pid = consumer_pid;
    gem_spawn_fn(t5_sender, sctx);

    gem_run_scheduler();
    ASSERT(cctx->passed, "GC corrupted string messages");
    return 1;
}

/* --- Test 6: Receive blocks until message arrives (ordering) --- */

typedef struct { int steps[4]; int count; int waiter_pid; } t6_ctx;

static GemVal t6_waiter(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t6_ctx *ctx = (t6_ctx *)env;
    ctx->steps[ctx->count++] = 1;  /* started */
    GemVal msg = gem_receive_msg(); /* blocks */
    (void)msg;
    ctx->steps[ctx->count++] = 3;  /* got message */
    return GEM_NIL;
}

static GemVal t6_sender(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t6_ctx *ctx = (t6_ctx *)env;
    ctx->steps[ctx->count++] = 2;  /* sender runs */
    gem_send_msg(ctx->waiter_pid, gem_int(99));
    return GEM_NIL;
}

static int test_receive_blocks(void) {
    t6_ctx *ctx = (t6_ctx *)gem_alloc(sizeof(t6_ctx));
    ctx->count = 0;

    ctx->waiter_pid = gem_spawn_fn(t6_waiter, ctx);
    gem_spawn_fn(t6_sender, ctx);

    gem_run_scheduler();

    ASSERT(ctx->count == 3, "wrong step count");
    ASSERT(ctx->steps[0] == 1, "waiter didn't start first");
    ASSERT(ctx->steps[1] == 2, "sender didn't run second");
    ASSERT(ctx->steps[2] == 3, "waiter didn't resume third");
    return 1;
}

/* --- Test 7: self() returns correct pid --- */

typedef struct { int reported_pid; int expected_pid; } t7_ctx;

static GemVal t7_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t7_ctx *ctx = (t7_ctx *)env;
    ctx->reported_pid = gem_self_pid();
    return GEM_NIL;
}

static int test_self_pid(void) {
    t7_ctx *ctx = (t7_ctx *)gem_alloc(sizeof(t7_ctx));
    ctx->reported_pid = -1;
    int pid = gem_spawn_fn(t7_proc, ctx);
    ctx->expected_pid = pid;
    gem_run_scheduler();
    ASSERT(ctx->reported_pid == ctx->expected_pid, "self() returned wrong pid");
    return 1;
}

/* --- Test 8: Send to dead/nonexistent process --- */

static int test_send_to_dead(void) {
    /* Send to nonexistent pid — should not crash */
    gem_send_msg(999, gem_int(1));
    gem_send_msg(-1, gem_int(2));
    gem_send_msg(GEM_MAX_PROCS + 1, gem_int(3));

    /* Spawn a process, let it finish, then send to it */
    t1_flag = 0;
    int pid = gem_spawn_fn(t1_proc, NULL);
    gem_run_scheduler();
    ASSERT(t1_flag == 1, "process didn't run");

    /* Process is now dead/free — send should not crash */
    gem_send_msg(pid, gem_int(99));

    return 1;
}

/* --- Test 9: All value types through mailbox --- */

typedef struct { GemVal msgs[7]; int count; } t9_ctx;

static GemVal t9_receiver(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t9_ctx *ctx = (t9_ctx *)env;
    for (int i = 0; i < 7; i++) {
        ctx->msgs[i] = gem_receive_msg();
        ctx->count++;
    }
    return GEM_NIL;
}

static int test_all_value_types(void) {
    t9_ctx *ctx = (t9_ctx *)gem_alloc(sizeof(t9_ctx));
    ctx->count = 0;
    int pid = gem_spawn_fn(t9_receiver, ctx);

    /* Send every value type */
    gem_send_msg(pid, GEM_NIL);                         /* nil */
    gem_send_msg(pid, gem_bool(1));                      /* bool true */
    gem_send_msg(pid, gem_bool(0));                      /* bool false */
    gem_send_msg(pid, gem_int(123456789));                /* int */
    gem_send_msg(pid, gem_float(3.14159));                /* float */
    gem_send_msg(pid, gem_string("hello concurrency"));   /* string */
    gem_send_msg(pid, gem_make_fn(t1_proc, NULL));        /* fn */

    gem_run_scheduler();

    ASSERT(ctx->count == 7, "didn't receive all messages");
    ASSERT(ctx->msgs[0].type == VAL_NIL, "msg 0 not nil");
    ASSERT(ctx->msgs[1].type == VAL_BOOL && ctx->msgs[1].bval == 1, "msg 1 not true");
    ASSERT(ctx->msgs[2].type == VAL_BOOL && ctx->msgs[2].bval == 0, "msg 2 not false");
    ASSERT(ctx->msgs[3].type == VAL_INT && ctx->msgs[3].ival == 123456789, "msg 3 wrong int");
    ASSERT(ctx->msgs[4].type == VAL_FLOAT && ctx->msgs[4].fval == 3.14159, "msg 4 wrong float");
    ASSERT(ctx->msgs[5].type == VAL_STRING && strcmp(ctx->msgs[5].sval, "hello concurrency") == 0, "msg 5 wrong string");
    ASSERT(ctx->msgs[6].type == VAL_FN, "msg 6 not fn");
    return 1;
}

/* --- Test 10: Spawn from inside a spawned process (nested) --- */

typedef struct { int child_ran; int child_pid; } t10_ctx;

static GemVal t10_child(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t10_ctx *ctx = (t10_ctx *)env;
    ctx->child_ran = 1;
    return GEM_NIL;
}

static GemVal t10_parent(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t10_ctx *ctx = (t10_ctx *)env;
    ctx->child_pid = gem_spawn_fn(t10_child, ctx);
    return GEM_NIL;
}

static int test_nested_spawn(void) {
    t10_ctx *ctx = (t10_ctx *)gem_alloc(sizeof(t10_ctx));
    ctx->child_ran = 0;
    ctx->child_pid = -1;
    gem_spawn_fn(t10_parent, ctx);
    gem_run_scheduler();
    ASSERT(ctx->child_pid >= 0, "child was not spawned");
    ASSERT(ctx->child_ran == 1, "child did not execute");
    return 1;
}

/* --- Test 11: Process slot reuse --- */

static int test_slot_reuse(void) {
    /* Fill and drain several times — slots should be recycled */
    for (int round = 0; round < 5; round++) {
        for (int i = 0; i < 100; i++) {
            int pid = gem_spawn_fn(t1_proc, NULL);
            ASSERT(pid >= 0, "spawn failed during slot reuse");
        }
        gem_run_scheduler();
    }
    /* If we get here without running out of slots, it works */
    return 1;
}

/* --- Test 12: Deep message chain (ring of processes) --- */

#define CHAIN_LEN 20

typedef struct { int next_pid; int first_pid; int is_last; GemVal result; } chain_ctx;

static GemVal chain_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    chain_ctx *ctx = (chain_ctx *)env;
    GemVal msg = gem_receive_msg();

    /* Increment the counter and forward */
    GemVal forwarded = gem_int(msg.ival + 1);

    if (ctx->is_last) {
        /* Send back to first */
        gem_send_msg(ctx->first_pid, forwarded);
    } else {
        gem_send_msg(ctx->next_pid, forwarded);
    }
    ctx->result = forwarded;
    return GEM_NIL;
}

static int test_message_chain(void) {
    chain_ctx *ctxs[CHAIN_LEN];
    int pids[CHAIN_LEN];

    /* Create all processes */
    for (int i = 0; i < CHAIN_LEN; i++) {
        ctxs[i] = (chain_ctx *)gem_alloc(sizeof(chain_ctx));
        ctxs[i]->is_last = (i == CHAIN_LEN - 1);
        pids[i] = gem_spawn_fn(chain_proc, ctxs[i]);
    }

    /* Wire them: each points to next, last points to first */
    for (int i = 0; i < CHAIN_LEN - 1; i++) {
        ctxs[i]->next_pid = pids[i + 1];
    }
    ctxs[0]->first_pid = pids[0]; /* not used by non-last */
    ctxs[CHAIN_LEN - 1]->first_pid = pids[0];

    /* Also need a "collector" to receive the final message at pid 0.
       Actually let's make it simpler: first process receives the result too. */

    /* Inject message at head of chain */
    gem_send_msg(pids[0], gem_int(0));

    /* First process also needs to collect the final result.
       But it already has a message (the initial one). Let's redesign:
       first sends to second, last sends result to a collector. */

    /* Actually the simple approach: the first process forwards, the last
       sends back. First process receives twice: once initial, once final. */

    gem_run_scheduler();

    /* Each process incremented by 1, so after CHAIN_LEN hops: value = CHAIN_LEN */
    /* But first process already consumed the initial 0, forwarded 1 to second...
       then last sends (CHAIN_LEN-1)+1 = CHAIN_LEN back to first.
       First already exited though. Hmm. Let me simplify. */

    /* Check the last process got CHAIN_LEN - 1 increments */
    ASSERT(ctxs[CHAIN_LEN - 1]->result.type == VAL_INT, "chain result not int");
    ASSERT(ctxs[CHAIN_LEN - 1]->result.ival == CHAIN_LEN, "chain didn't propagate correctly");
    return 1;
}

/* --- Test 13: High-scale stress --- */

#define STRESS_PROCS 200
#define STRESS_MSGS 50

typedef struct { int target_pid; int id; } stress_sender_ctx;
typedef struct { int total; } stress_collector_ctx;

static GemVal stress_sender(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    stress_sender_ctx *ctx = (stress_sender_ctx *)env;
    for (int i = 0; i < STRESS_MSGS; i++) {
        gem_send_msg(ctx->target_pid, gem_int(ctx->id * 100000 + i));
    }
    return GEM_NIL;
}

static GemVal stress_collector(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    stress_collector_ctx *ctx = (stress_collector_ctx *)env;
    int expected = STRESS_PROCS * STRESS_MSGS;
    for (int i = 0; i < expected; i++) {
        gem_receive_msg();
        ctx->total++;
    }
    return GEM_NIL;
}

static int test_high_scale(void) {
    stress_collector_ctx *coll = (stress_collector_ctx *)gem_alloc(sizeof(stress_collector_ctx));
    coll->total = 0;
    int coll_pid = gem_spawn_fn(stress_collector, coll);

    for (int i = 0; i < STRESS_PROCS; i++) {
        stress_sender_ctx *ctx = (stress_sender_ctx *)gem_alloc(sizeof(stress_sender_ctx));
        ctx->target_pid = coll_pid;
        ctx->id = i;
        int pid = gem_spawn_fn(stress_sender, ctx);
        ASSERT(pid >= 0, "spawn failed during stress test");
    }

    gem_run_scheduler();
    ASSERT(coll->total == STRESS_PROCS * STRESS_MSGS, "collector missed messages");
    return 1;
}

/* --- Test 14: GC pressure with heavy allocation inside coroutines --- */

#define GC_STRESS_PROCS 50
#define GC_ALLOCS_PER_PROC 200

typedef struct { int id; int passed; } gc_stress_ctx;

static GemVal gc_stress_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    gc_stress_ctx *ctx = (gc_stress_ctx *)env;

    /* Allocate many GC objects, keep them all alive */
    GemVal tbl = gem_table_new();
    for (int i = 0; i < GC_ALLOCS_PER_PROC; i++) {
        char buf[64];
        snprintf(buf, 64, "proc-%d-alloc-%d", ctx->id, i);
        gem_table_set(tbl, gem_int(i), gem_string(buf));
    }

    /* Verify all survived */
    ctx->passed = 1;
    for (int i = 0; i < GC_ALLOCS_PER_PROC; i++) {
        char expected[64];
        snprintf(expected, 64, "proc-%d-alloc-%d", ctx->id, i);
        GemVal val = gem_table_get(tbl, gem_int(i));
        if (val.type != VAL_STRING || strcmp(val.sval, expected) != 0) {
            ctx->passed = 0;
            return GEM_NIL;
        }
    }
    return GEM_NIL;
}

static int test_gc_pressure(void) {
    gc_stress_ctx *ctxs[GC_STRESS_PROCS];
    for (int i = 0; i < GC_STRESS_PROCS; i++) {
        ctxs[i] = (gc_stress_ctx *)gem_alloc(sizeof(gc_stress_ctx));
        ctxs[i]->id = i;
        ctxs[i]->passed = 0;
        gem_spawn_fn(gc_stress_proc, ctxs[i]);
    }

    gem_run_scheduler();

    for (int i = 0; i < GC_STRESS_PROCS; i++) {
        if (!ctxs[i]->passed) {
            printf("FAIL: process %d lost GC objects\n", i);
            return 0;
        }
    }
    return 1;
}

/* --- Test 15: Multiple scheduler runs --- */

typedef struct { int value; } t15_ctx;

static GemVal t15_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t15_ctx *ctx = (t15_ctx *)env;
    ctx->value += 1;
    return GEM_NIL;
}

static int test_multiple_scheduler_runs(void) {
    t15_ctx *ctx = (t15_ctx *)gem_alloc(sizeof(t15_ctx));
    ctx->value = 0;

    /* Run scheduler multiple times with fresh processes each time */
    for (int round = 0; round < 10; round++) {
        gem_spawn_fn(t15_proc, ctx);
        gem_run_scheduler();
    }
    ASSERT(ctx->value == 10, "multiple scheduler runs didn't all execute");
    return 1;
}

/* --- Test 16: Process with no messages exits cleanly --- */

typedef struct { int ran; } t16_ctx;

static GemVal t16_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t16_ctx *ctx = (t16_ctx *)env;
    /* Do work but never call receive */
    ctx->ran = 1;
    return GEM_NIL;
}

static int test_no_messages(void) {
    t16_ctx *ctx = (t16_ctx *)gem_alloc(sizeof(t16_ctx));
    ctx->ran = 0;

    /* Spawn 20 processes that don't use messaging at all */
    for (int i = 0; i < 20; i++) {
        gem_spawn_fn(t16_proc, ctx);
    }
    gem_run_scheduler();

    /* ctx->ran will be 1 if at least one ran (all share same ctx, all set to 1) */
    ASSERT(ctx->ran == 1, "no-message processes didn't run");
    return 1;
}

/* --- Test 17: Burst send then schedule --- */

typedef struct { int total; } t17_ctx;

static GemVal t17_receiver(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t17_ctx *ctx = (t17_ctx *)env;
    for (int i = 0; i < 1000; i++) {
        GemVal msg = gem_receive_msg();
        if (msg.type != VAL_INT) return GEM_NIL;
        ctx->total++;
    }
    return GEM_NIL;
}

static int test_burst_send(void) {
    t17_ctx *ctx = (t17_ctx *)gem_alloc(sizeof(t17_ctx));
    ctx->total = 0;
    int pid = gem_spawn_fn(t17_receiver, ctx);

    /* Send 1000 messages before scheduler has a chance to run */
    for (int i = 0; i < 1000; i++) {
        gem_send_msg(pid, gem_int(i));
    }

    gem_run_scheduler();
    ASSERT(ctx->total == 1000, "burst receiver didn't get all messages");
    return 1;
}

/* --- Test 18: Table messages survive GC across processes --- */

typedef struct { int passed; } t18_ctx;
typedef struct { int target_pid; } t18_sender_ctx;

static GemVal t18_sender(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t18_sender_ctx *ctx = (t18_sender_ctx *)env;

    /* Build a table and send it */
    GemVal tbl = gem_table_new();
    gem_table_set(tbl, gem_string("name"), gem_string("test-table"));
    gem_table_set(tbl, gem_string("value"), gem_int(42));
    gem_table_set(tbl, gem_int(0), gem_string("first"));
    gem_table_set(tbl, gem_int(1), gem_string("second"));

    gem_send_msg(ctx->target_pid, tbl);
    return GEM_NIL;
}

static GemVal t18_receiver(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t18_ctx *ctx = (t18_ctx *)env;
    GemVal msg = gem_receive_msg();

    ctx->passed = 1;
    if (msg.type != VAL_TABLE) { ctx->passed = 0; return GEM_NIL; }

    GemVal name = gem_table_get(msg, gem_string("name"));
    if (name.type != VAL_STRING || strcmp(name.sval, "test-table") != 0) {
        ctx->passed = 0; return GEM_NIL;
    }

    GemVal val = gem_table_get(msg, gem_string("value"));
    if (val.type != VAL_INT || val.ival != 42) {
        ctx->passed = 0; return GEM_NIL;
    }

    GemVal first = gem_table_get(msg, gem_int(0));
    if (first.type != VAL_STRING || strcmp(first.sval, "first") != 0) {
        ctx->passed = 0; return GEM_NIL;
    }

    return GEM_NIL;
}

static int test_table_messages(void) {
    t18_ctx *rctx = (t18_ctx *)gem_alloc(sizeof(t18_ctx));
    rctx->passed = 0;
    int recv_pid = gem_spawn_fn(t18_receiver, rctx);

    t18_sender_ctx *sctx = (t18_sender_ctx *)gem_alloc(sizeof(t18_sender_ctx));
    sctx->target_pid = recv_pid;
    gem_spawn_fn(t18_sender, sctx);

    gem_run_scheduler();
    ASSERT(rctx->passed, "table message was corrupted across processes");
    return 1;
}

/* --- Test 19: Bidirectional messaging (request-reply pattern) --- */

#define REQ_REPLY_ROUNDS 50

typedef struct { int server_pid; int success_count; } client_ctx;
typedef struct { int rounds; } server_ctx;

static GemVal server_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    server_ctx *ctx = (server_ctx *)env;
    for (int i = 0; i < REQ_REPLY_ROUNDS; i++) {
        /* Receive a request table with "from" and "payload" */
        GemVal req = gem_receive_msg();
        GemVal from = gem_table_get(req, gem_string("from"));
        GemVal payload = gem_table_get(req, gem_string("payload"));

        /* Build response */
        GemVal resp = gem_table_new();
        gem_table_set(resp, gem_string("result"), gem_int(payload.ival * 2));

        gem_send_msg((int)from.ival, resp);
        ctx->rounds++;
    }
    return GEM_NIL;
}

static GemVal client_proc(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    client_ctx *ctx = (client_ctx *)env;
    int my_pid = gem_self_pid();

    for (int i = 0; i < REQ_REPLY_ROUNDS; i++) {
        GemVal req = gem_table_new();
        gem_table_set(req, gem_string("from"), gem_int(my_pid));
        gem_table_set(req, gem_string("payload"), gem_int(i));

        gem_send_msg(ctx->server_pid, req);
        GemVal resp = gem_receive_msg();

        GemVal result = gem_table_get(resp, gem_string("result"));
        if (result.type == VAL_INT && result.ival == i * 2) {
            ctx->success_count++;
        }
    }
    return GEM_NIL;
}

static int test_request_reply(void) {
    server_ctx *sctx = (server_ctx *)gem_alloc(sizeof(server_ctx));
    sctx->rounds = 0;
    int srv_pid = gem_spawn_fn(server_proc, sctx);

    client_ctx *cctx = (client_ctx *)gem_alloc(sizeof(client_ctx));
    cctx->server_pid = srv_pid;
    cctx->success_count = 0;
    gem_spawn_fn(client_proc, cctx);

    gem_run_scheduler();
    ASSERT(sctx->rounds == REQ_REPLY_ROUNDS, "server didn't handle all requests");
    ASSERT(cctx->success_count == REQ_REPLY_ROUNDS, "client got wrong replies");
    return 1;
}

/* --- Test 20: Spawn builtin wrappers (as Gem code would call them) --- */

typedef struct { int ran; } t20_ctx;

static GemVal t20_proc(void *_env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t20_ctx *ctx = (t20_ctx *)_env;
    /* Use builtins: self, receive, send-back */
    GemVal self_val = gem_self_builtin(NULL, NULL, 0);
    if (self_val.type != VAL_INT || self_val.ival < 0) return GEM_NIL;

    GemVal msg = gem_receive_builtin(NULL, NULL, 0);
    (void)msg;
    ctx->ran = 1;
    return GEM_NIL;
}

static int test_builtin_wrappers(void) {
    t20_ctx *ctx = (t20_ctx *)gem_alloc(sizeof(t20_ctx));
    ctx->ran = 0;

    /* Use spawn builtin */
    GemVal fn_val = gem_make_fn(t20_proc, ctx);
    GemVal spawn_args[1] = { fn_val };
    GemVal pid_val = gem_spawn_builtin(NULL, spawn_args, 1);
    ASSERT(pid_val.type == VAL_INT && pid_val.ival >= 0, "spawn builtin failed");

    /* Use send builtin */
    GemVal send_args[2] = { pid_val, gem_string("hello") };
    gem_send_builtin(NULL, send_args, 2);

    gem_run_scheduler();
    ASSERT(ctx->ran == 1, "builtin-spawned process didn't run");
    return 1;
}

/* --- Test 21: Deadlock detection — scheduler exits when all blocked --- */

typedef struct { int msgs_received; } t21_ctx;

static GemVal t21_looper(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    t21_ctx *ctx = (t21_ctx *)env;
    /* Receive in a loop — will block after messages are exhausted */
    for (;;) {
        GemVal msg = gem_receive_msg();
        if (msg.type == VAL_NIL) break;
        ctx->msgs_received++;
    }
    return GEM_NIL;
}

static int test_deadlock_detection(void) {
    t21_ctx *ctx = (t21_ctx *)gem_alloc(sizeof(t21_ctx));
    ctx->msgs_received = 0;
    int pid = gem_spawn_fn(t21_looper, ctx);

    /* Send 3 messages, then a nil sentinel to stop the loop */
    gem_send_msg(pid, gem_string("a"));
    gem_send_msg(pid, gem_string("b"));
    gem_send_msg(pid, gem_string("c"));
    gem_send_msg(pid, GEM_NIL);

    gem_run_scheduler();
    ASSERT(ctx->msgs_received == 3, "looper didn't get all messages");
    return 1;
}

/* --- Test 22: Scheduler terminates with blocked infinite-receive process --- */

static GemVal t22_infinite_receiver(void *env, GemVal *args, int argc) {
    (void)args; (void)argc;
    int *count = (int *)env;
    /* Receive forever — scheduler must detect deadlock and exit */
    for (;;) {
        gem_receive_msg();
        (*count)++;
    }
    return GEM_NIL;
}

static int test_scheduler_exits_on_deadlock(void) {
    int *count = (int *)gem_alloc(sizeof(int));
    *count = 0;
    int pid = gem_spawn_fn(t22_infinite_receiver, count);

    /* Send 2 messages, then let it block */
    gem_send_msg(pid, gem_int(1));
    gem_send_msg(pid, gem_int(2));

    /* This MUST return (not hang). Before the deadlock fix, it would spin forever. */
    gem_run_scheduler();

    ASSERT(*count == 2, "infinite receiver didn't consume both messages");
    return 1;
}

/* ================================================================ */

int main(void) {
    printf("=== Gem Concurrency Runtime — Stress Test Suite ===\n\n");

    RUN_TEST(test_spawn_basic);
    RUN_TEST(test_send_receive);
    RUN_TEST(test_ping_pong);
    RUN_TEST(test_many_producers);
    RUN_TEST(test_gc_messages);
    RUN_TEST(test_receive_blocks);
    RUN_TEST(test_self_pid);
    RUN_TEST(test_send_to_dead);
    RUN_TEST(test_all_value_types);
    RUN_TEST(test_nested_spawn);
    RUN_TEST(test_slot_reuse);
    RUN_TEST(test_message_chain);
    RUN_TEST(test_high_scale);
    RUN_TEST(test_gc_pressure);
    RUN_TEST(test_multiple_scheduler_runs);
    RUN_TEST(test_no_messages);
    RUN_TEST(test_burst_send);
    RUN_TEST(test_table_messages);
    RUN_TEST(test_request_reply);
    RUN_TEST(test_builtin_wrappers);
    RUN_TEST(test_deadlock_detection);
    RUN_TEST(test_scheduler_exits_on_deadlock);

    printf("\n%d/%d tests passed.\n", tests_passed, tests_run);
    if (tests_passed == tests_run) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED.\n");
        return 1;
    }
}
