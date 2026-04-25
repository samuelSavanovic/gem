/*
 * Prototype: Scheduler + mailbox system for Gem runtime
 *
 * Builds spawn/send/receive on top of minicoro + Boehm GC.
 * This is what the compiled Gem code will call.
 *
 * Architecture:
 *   - Process table: array of process slots (coroutine + mailbox)
 *   - Mailbox: simple linked list queue of GC-allocated messages
 *   - Scheduler: round-robin, resumes READY processes, skips WAITING ones
 *     until they get a message
 *   - spawn(fn) -> pid
 *   - send(pid, value) -> enqueues message, wakes process if WAITING
 *   - receive() -> dequeues message, or yields with WAITING status
 *
 * Tests:
 *   1. spawn + basic execution
 *   2. send/receive between processes
 *   3. ping-pong between two processes
 *   4. many producers, one consumer
 *   5. process that exits normally
 */

#define MCO_API static
#define MINICORO_IMPL
#include "minicoro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc.h>

/* ================================================================
 * GEM RUNTIME PROTOTYPE
 * This section (~150 lines) is what goes into the real runtime.
 * ================================================================ */

/* --- Tagged values (simplified for prototype) --- */

typedef enum {
    VAL_NIL,
    VAL_INT,
    VAL_STRING,
} val_type_t;

typedef struct {
    val_type_t type;
    union {
        int64_t ival;
        char *sval;
    };
} gem_val_t;

static gem_val_t gem_nil(void) {
    return (gem_val_t){ .type = VAL_NIL };
}

static gem_val_t gem_int(int64_t v) {
    return (gem_val_t){ .type = VAL_INT, .ival = v };
}

static gem_val_t gem_str(const char *s) {
    char *copy = GC_malloc(strlen(s) + 1);
    strcpy(copy, s);
    return (gem_val_t){ .type = VAL_STRING, .sval = copy };
}

/* --- Mailbox (GC-allocated linked list queue) --- */

typedef struct msg_node {
    gem_val_t value;
    struct msg_node *next;
} msg_node_t;

typedef struct {
    msg_node_t *head;
    msg_node_t *tail;
} mailbox_t;

static void mailbox_push(mailbox_t *mb, gem_val_t val) {
    msg_node_t *node = GC_malloc(sizeof(msg_node_t));
    node->value = val;
    node->next = NULL;
    if (mb->tail) {
        mb->tail->next = node;
    } else {
        mb->head = node;
    }
    mb->tail = node;
}

static int mailbox_empty(mailbox_t *mb) {
    return mb->head == NULL;
}

static gem_val_t mailbox_pop(mailbox_t *mb) {
    msg_node_t *node = mb->head;
    gem_val_t val = node->value;
    mb->head = node->next;
    if (!mb->head) mb->tail = NULL;
    /* node is GC-allocated, no free needed */
    return val;
}

/* --- Process table --- */

typedef enum {
    PROC_FREE,      /* slot available */
    PROC_READY,     /* can be resumed */
    PROC_WAITING,   /* blocked on receive() */
    PROC_DEAD,      /* finished execution */
} proc_state_t;

typedef struct {
    proc_state_t state;
    mco_coro *coro;
    mailbox_t mailbox;
    int pid;
} process_t;

#define MAX_PROCS 256

static process_t proc_table[MAX_PROCS];
static int current_pid = -1;  /* pid of the currently running process */

/* GC-aware stack allocator (from gc_coro_test.c) */
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

/* --- Core API: spawn, send, receive --- */

typedef void (*gem_proc_fn)(mco_coro *co);

static int gem_spawn(gem_proc_fn fn, void *user_data) {
    /* Find a free slot */
    int pid = -1;
    for (int i = 0; i < MAX_PROCS; i++) {
        if (proc_table[i].state == PROC_FREE) {
            pid = i;
            break;
        }
    }
    if (pid < 0) {
        fprintf(stderr, "error: process table full\n");
        return -1;
    }

    mco_desc desc = mco_desc_init(fn, 0);
    desc.alloc_cb = coro_stack_alloc;
    desc.dealloc_cb = coro_stack_free;
    desc.user_data = user_data;

    mco_coro *co;
    mco_result res = mco_create(&co, &desc);
    if (res != MCO_SUCCESS) {
        fprintf(stderr, "mco_create failed: %s\n", mco_result_description(res));
        return -1;
    }

    proc_table[pid].state = PROC_READY;
    proc_table[pid].coro = co;
    proc_table[pid].mailbox = (mailbox_t){NULL, NULL};
    proc_table[pid].pid = pid;

    return pid;
}

static void gem_send(int pid, gem_val_t val) {
    if (pid < 0 || pid >= MAX_PROCS) return;
    process_t *proc = &proc_table[pid];
    if (proc->state == PROC_FREE || proc->state == PROC_DEAD) return;

    mailbox_push(&proc->mailbox, val);

    /* Wake up the process if it was blocked on receive */
    if (proc->state == PROC_WAITING) {
        proc->state = PROC_READY;
    }
}

static gem_val_t gem_receive(mco_coro *co) {
    process_t *proc = &proc_table[current_pid];

    /* If mailbox has messages, return immediately */
    while (mailbox_empty(&proc->mailbox)) {
        /* Block: mark as WAITING and yield to scheduler */
        proc->state = PROC_WAITING;
        mco_yield(co);
        /* Scheduler will only resume us when state is READY again
           (i.e., someone called send to our pid) */
    }

    return mailbox_pop(&proc->mailbox);
}

/* Helper: get own pid from inside a coroutine */
static int gem_self(void) {
    return current_pid;
}

/* --- Scheduler --- */

static void gem_run_scheduler(void) {
    int active = 1;

    while (active) {
        active = 0;
        for (int i = 0; i < MAX_PROCS; i++) {
            process_t *proc = &proc_table[i];

            if (proc->state == PROC_READY) {
                active = 1;
                current_pid = i;
                mco_resume(proc->coro);

                /* Check if coroutine finished */
                if (mco_status(proc->coro) == MCO_DEAD) {
                    proc->state = PROC_DEAD;
                    mco_destroy(proc->coro);
                    proc->coro = NULL;
                    proc->state = PROC_FREE;
                }
            } else if (proc->state == PROC_WAITING) {
                /* Still alive, just blocked */
                active = 1;
            }
        }
    }
    current_pid = -1;
}

/* ================================================================
 * TESTS
 * ================================================================ */

/* --- test 1: spawn + basic execution --- */

static void test1_proc(mco_coro *co) {
    (void)co;
    int *flag = mco_get_user_data(co);
    *flag = 1;
}

static void test_spawn_basic(void) {
    printf("test 1: spawn + basic execution... ");

    int flag = 0;
    int pid = gem_spawn(test1_proc, &flag);
    if (pid < 0) { printf("FAIL - spawn failed\n"); exit(1); }

    gem_run_scheduler();

    if (flag) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
        exit(1);
    }
}

/* --- test 2: send/receive --- */

struct test2_ctx {
    gem_val_t received;
    int got_message;
};

static void test2_proc(mco_coro *co) {
    struct test2_ctx *ctx = mco_get_user_data(co);
    ctx->received = gem_receive(co);
    ctx->got_message = 1;
}

static void test_send_receive(void) {
    printf("test 2: send/receive... ");

    struct test2_ctx ctx = { .got_message = 0 };
    int pid = gem_spawn(test2_proc, &ctx);

    /* Send a message before running scheduler — it should be queued */
    gem_send(pid, gem_int(42));
    gem_run_scheduler();

    if (ctx.got_message && ctx.received.type == VAL_INT &&
        ctx.received.ival == 42) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
        exit(1);
    }
}

/* --- test 3: ping-pong between two processes --- */

#define PING_PONG_ROUNDS 10

struct pingpong_ctx {
    int partner_pid;
    int count;
};

static void ping_proc(mco_coro *co) {
    struct pingpong_ctx *ctx = mco_get_user_data(co);

    for (int i = 0; i < PING_PONG_ROUNDS; i++) {
        gem_send(ctx->partner_pid, gem_int(i));
        gem_val_t reply = gem_receive(co);
        (void)reply;
        ctx->count++;
    }
}

static void pong_proc(mco_coro *co) {
    struct pingpong_ctx *ctx = mco_get_user_data(co);

    for (int i = 0; i < PING_PONG_ROUNDS; i++) {
        gem_val_t msg = gem_receive(co);
        gem_send(ctx->partner_pid, gem_int(msg.ival + 100));
        ctx->count++;
    }
}

static void test_ping_pong(void) {
    printf("test 3: ping-pong (%d rounds)... ", PING_PONG_ROUNDS);

    struct pingpong_ctx ping_ctx = { .partner_pid = -1, .count = 0 };
    struct pingpong_ctx pong_ctx = { .partner_pid = -1, .count = 0 };

    int pong_pid = gem_spawn(pong_proc, &pong_ctx);
    int ping_pid = gem_spawn(ping_proc, &ping_ctx);

    /* Wire them together */
    ping_ctx.partner_pid = pong_pid;
    pong_ctx.partner_pid = ping_pid;

    gem_run_scheduler();

    if (ping_ctx.count == PING_PONG_ROUNDS &&
        pong_ctx.count == PING_PONG_ROUNDS) {
        printf("PASS\n");
    } else {
        printf("FAIL (ping=%d, pong=%d)\n", ping_ctx.count, pong_ctx.count);
        exit(1);
    }
}

/* --- test 4: many producers, one consumer --- */

#define NUM_PRODUCERS 20
#define MSGS_PER_PRODUCER 10

struct producer_ctx {
    int consumer_pid;
    int id;
};

static void producer_proc(mco_coro *co) {
    struct producer_ctx *ctx = mco_get_user_data(co);
    for (int i = 0; i < MSGS_PER_PRODUCER; i++) {
        gem_send(ctx->consumer_pid, gem_int(ctx->id * 1000 + i));
    }
}

struct consumer_ctx {
    int total_received;
};

static void consumer_proc(mco_coro *co) {
    struct consumer_ctx *ctx = mco_get_user_data(co);
    int expected = NUM_PRODUCERS * MSGS_PER_PRODUCER;

    for (int i = 0; i < expected; i++) {
        gem_val_t msg = gem_receive(co);
        (void)msg;
        ctx->total_received++;
    }
}

static void test_many_producers(void) {
    printf("test 4: %d producers -> 1 consumer (%d msgs each)... ",
           NUM_PRODUCERS, MSGS_PER_PRODUCER);

    struct consumer_ctx cons_ctx = { .total_received = 0 };
    int consumer_pid = gem_spawn(consumer_proc, &cons_ctx);

    struct producer_ctx prod_ctxs[NUM_PRODUCERS];
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        prod_ctxs[i].consumer_pid = consumer_pid;
        prod_ctxs[i].id = i;
        gem_spawn(producer_proc, &prod_ctxs[i]);
    }

    gem_run_scheduler();

    int expected = NUM_PRODUCERS * MSGS_PER_PRODUCER;
    if (cons_ctx.total_received == expected) {
        printf("PASS\n");
    } else {
        printf("FAIL (got %d, expected %d)\n", cons_ctx.total_received, expected);
        exit(1);
    }
}

/* --- test 5: GC-allocated messages survive scheduling --- */

static void test5_sender(mco_coro *co) {
    int *consumer_pid = mco_get_user_data(co);

    for (int i = 0; i < 50; i++) {
        char buf[64];
        snprintf(buf, 64, "gc-message-%d", i);
        gem_send(*consumer_pid, gem_str(buf));
        /* Force GC between sends */
        GC_gcollect();
        mco_yield(co);
    }
}

struct test5_consumer_ctx {
    int passed;
};

static void test5_consumer(mco_coro *co) {
    struct test5_consumer_ctx *ctx = mco_get_user_data(co);
    ctx->passed = 1;

    for (int i = 0; i < 50; i++) {
        gem_val_t msg = gem_receive(co);
        GC_gcollect();

        char expected[64];
        snprintf(expected, 64, "gc-message-%d", i);
        if (msg.type != VAL_STRING || strcmp(msg.sval, expected) != 0) {
            ctx->passed = 0;
            break;
        }
    }
}

static void test_gc_messages(void) {
    printf("test 5: GC-allocated messages survive scheduling... ");

    struct test5_consumer_ctx cons_ctx = { .passed = 0 };
    int consumer_pid = gem_spawn(test5_consumer, &cons_ctx);

    int cpid = consumer_pid;
    gem_spawn(test5_sender, &cpid);

    gem_run_scheduler();

    if (cons_ctx.passed) {
        printf("PASS\n");
    } else {
        printf("FAIL - GC corrupted messages\n");
        exit(1);
    }
}

/* --- test 6: receive blocks until message arrives --- */

struct test6_ctx {
    int steps[4];
    int step_count;
};

static void test6_waiter(mco_coro *co) {
    struct test6_ctx *ctx = mco_get_user_data(co);
    ctx->steps[ctx->step_count++] = 1;  /* step 1: started */
    gem_val_t msg = gem_receive(co);     /* blocks here */
    (void)msg;
    ctx->steps[ctx->step_count++] = 3;  /* step 3: got message */
}

static void test6_sender(mco_coro *co) {
    struct test6_ctx *ctx = mco_get_user_data(co);
    int *waiter_pid = (int *)((char *)ctx + sizeof(struct test6_ctx));

    ctx->steps[ctx->step_count++] = 2;  /* step 2: sender runs */
    gem_send(*waiter_pid, gem_int(99));
}

static void test_receive_blocks(void) {
    printf("test 6: receive blocks until message arrives... ");

    /* Pack ctx + waiter_pid together */
    struct {
        struct test6_ctx ctx;
        int waiter_pid;
    } data;
    memset(&data, 0, sizeof(data));

    data.waiter_pid = gem_spawn(test6_waiter, &data.ctx);
    gem_spawn(test6_sender, &data.ctx);

    gem_run_scheduler();

    /* Verify ordering: waiter started (1), sender ran (2), waiter got msg (3) */
    if (data.ctx.step_count == 3 &&
        data.ctx.steps[0] == 1 &&
        data.ctx.steps[1] == 2 &&
        data.ctx.steps[2] == 3) {
        printf("PASS\n");
    } else {
        printf("FAIL (steps: ");
        for (int i = 0; i < data.ctx.step_count; i++)
            printf("%d ", data.ctx.steps[i]);
        printf(")\n");
        exit(1);
    }
}

/* ---------- main ---------- */

int main(void) {
    GC_INIT();
    memset(proc_table, 0, sizeof(proc_table));

    printf("=== Scheduler + Mailbox prototype ===\n\n");

    test_spawn_basic();
    test_send_receive();
    test_ping_pong();
    test_many_producers();
    test_gc_messages();
    test_receive_blocks();

    printf("\nAll tests passed.\n\n");
    printf("Runtime API proven:\n");
    printf("  gem_spawn(fn, data) -> pid\n");
    printf("  gem_send(pid, val)  -> enqueue + wake\n");
    printf("  gem_receive(co)     -> dequeue or block\n");
    printf("  gem_self()          -> current pid\n");
    printf("  gem_run_scheduler() -> round-robin until all done\n");

    return 0;
}
