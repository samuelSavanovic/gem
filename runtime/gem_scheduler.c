/*
 * gem_scheduler.c — Concurrency: scheduler, coroutines, mailbox, spawn/send/receive.
 */

#define MINICORO_IMPL
#include "minicoro.h"

#include <poll.h>
#include <fcntl.h>
#include <errno.h>

#include "gem.h"

/* ─── Globals ─── */

GemProcess gem_proc_table[GEM_MAX_PROCS];
int gem_current_pid = -1;

/* GC-aware stack allocator for minicoro coroutines.
   malloc the stack, then register it as a GC root so Boehm
   can scan coroutine stacks for heap pointers. */
static void *gem_coro_stack_alloc(size_t size, void *udata) {
    (void)udata;
    void *ptr = malloc(size);
    if (!ptr) return NULL;
    GC_add_roots(ptr, (char *)ptr + size);
    return ptr;
}

static void gem_coro_stack_free(void *ptr, size_t size, void *udata) {
    (void)udata;
    GC_remove_roots(ptr, (char *)ptr + size);
    free(ptr);
}

/* Mailbox operations */
static void gem_mailbox_push(GemMailbox *mb, GemVal val) {
    GemMsgNode *node = (GemMsgNode *)GC_MALLOC(sizeof(GemMsgNode));
    node->value = val;
    node->next = NULL;
    if (mb->tail) {
        mb->tail->next = node;
    } else {
        mb->head = node;
    }
    mb->tail = node;
}

static int gem_mailbox_empty(GemMailbox *mb) {
    return mb->head == NULL;
}

static GemVal gem_mailbox_pop(GemMailbox *mb) {
    GemMsgNode *node = mb->head;
    GemVal val = node->value;
    mb->head = node->next;
    if (!mb->head) mb->tail = NULL;
    return val;
}

/* Coroutine entry point — calls the GemFnPtr stored in user_data */
typedef struct {
    GemFnPtr fn;
    void *env;
} GemCoroCtx;

static void gem_coro_entry(mco_coro *co) {
    GemCoroCtx *ctx = (GemCoroCtx *)mco_get_user_data(co);
    ctx->fn(ctx->env, NULL, 0);
}

/* Core API */

int gem_spawn_fn(GemFnPtr fn, void *env) {
    int pid = -1;
    for (int i = 0; i < GEM_MAX_PROCS; i++) {
        if (gem_proc_table[i].state == GEM_PROC_FREE) {
            pid = i;
            break;
        }
    }
    if (pid < 0) {
        gem_error("spawn: process table full");
        return -1;
    }

    GemCoroCtx *ctx = (GemCoroCtx *)GC_MALLOC(sizeof(GemCoroCtx));
    ctx->fn = fn;
    ctx->env = env;

    mco_desc desc = mco_desc_init(gem_coro_entry, 0);
    desc.alloc_cb = gem_coro_stack_alloc;
    desc.dealloc_cb = gem_coro_stack_free;
    desc.user_data = ctx;

    mco_coro *co;
    mco_result res = mco_create(&co, &desc);
    if (res != MCO_SUCCESS) {
        gem_error("spawn: coroutine creation failed");
        return -1;
    }

    gem_proc_table[pid].state = GEM_PROC_READY;
    gem_proc_table[pid].coro = co;
    gem_proc_table[pid].mailbox = (GemMailbox){NULL, NULL};
    gem_proc_table[pid].pid = pid;

    return pid;
}

void gem_send_msg(int pid, GemVal val) {
    if (pid < 0 || pid >= GEM_MAX_PROCS) return;
    GemProcess *proc = &gem_proc_table[pid];
    if (proc->state == GEM_PROC_FREE || proc->state == GEM_PROC_DEAD) return;

    gem_mailbox_push(&proc->mailbox, val);

    if (proc->state == GEM_PROC_WAITING) {
        proc->state = GEM_PROC_READY;
    }
}

GemVal gem_receive_msg(void) {
    if (gem_current_pid < 0 || gem_current_pid >= GEM_MAX_PROCS) {
        gem_error("receive: not inside a spawned process");
        return GEM_NIL;
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];

    while (gem_mailbox_empty(&proc->mailbox)) {
        proc->state = GEM_PROC_WAITING;
        mco_yield(proc->coro);
    }

    return gem_mailbox_pop(&proc->mailbox);
}

int gem_self_pid(void) {
    return gem_current_pid;
}

void gem_io_yield(int fd, int for_write) {
    if (gem_current_pid < 0 || gem_current_pid >= GEM_MAX_PROCS) {
        /* Not inside a coroutine — can't yield, just return and let
           the caller retry with a blocking call or spin. */
        return;
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    proc->state = GEM_PROC_IO_WAIT;
    proc->wait_fd = fd;
    proc->wait_write = for_write;
    mco_yield(proc->coro);
}

void gem_run_scheduler(void) {
    int active = 1;

    while (active) {
        active = 0;
        int has_ready = 0;
        int has_io_wait = 0;
        int has_msg_wait = 0;

        for (int i = 0; i < GEM_MAX_PROCS; i++) {
            GemProcess *proc = &gem_proc_table[i];

            if (proc->state == GEM_PROC_READY) {
                has_ready = 1;
                active = 1;
                gem_current_pid = i;
                mco_resume(proc->coro);

                if (mco_status(proc->coro) == MCO_DEAD) {
                    mco_destroy(proc->coro);
                    proc->coro = NULL;
                    proc->state = GEM_PROC_FREE;
                }
            } else if (proc->state == GEM_PROC_WAITING) {
                has_msg_wait = 1;
                active = 1;
            } else if (proc->state == GEM_PROC_IO_WAIT) {
                has_io_wait = 1;
                active = 1;
            }
        }

        /* If we ran at least one READY process, loop immediately —
           the resumed coroutines may have enqueued messages or spawned
           new work. */
        if (has_ready) continue;

        /* Nothing was READY. Check for I/O waiters. */
        if (has_io_wait) {
            /* Build a pollfd array for all IO_WAIT processes. */
            struct pollfd pfds[GEM_MAX_PROCS];
            int pfd_pid[GEM_MAX_PROCS];  /* maps pollfd index → pid */
            int nfds = 0;

            for (int i = 0; i < GEM_MAX_PROCS; i++) {
                if (gem_proc_table[i].state == GEM_PROC_IO_WAIT) {
                    pfds[nfds].fd = gem_proc_table[i].wait_fd;
                    pfds[nfds].events = gem_proc_table[i].wait_write ? POLLOUT : POLLIN;
                    pfds[nfds].revents = 0;
                    pfd_pid[nfds] = i;
                    nfds++;
                }
            }

            /* Block until at least one fd is ready.
               Use -1 timeout: the only way to make progress is I/O. */
            int ready = poll(pfds, (nfds_t)nfds, -1);
            if (ready > 0) {
                for (int j = 0; j < nfds; j++) {
                    if (pfds[j].revents & (POLLIN | POLLOUT | POLLERR | POLLHUP)) {
                        gem_proc_table[pfd_pid[j]].state = GEM_PROC_READY;
                    }
                }
            }
            /* Loop back to resume the now-READY processes. */
            continue;
        }

        /* Only mailbox waiters remain and nobody can send → deadlock. */
        if (has_msg_wait && !has_io_wait) break;
    }
    gem_current_pid = -1;
}

/* Built-in function wrappers for use from compiled Gem code */

GemVal gem_spawn_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_FN) {
        gem_error("spawn: expected function argument");
    }
    int pid = gem_spawn_fn(args[0].fn, args[0].env);
    return gem_int(pid);
}

GemVal gem_send_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_INT) {
        gem_error("send: expected (pid, value)");
    }
    gem_send_msg((int)args[0].ival, args[1]);
    return GEM_NIL;
}

GemVal gem_receive_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    (void)args;
    (void)argc;
    return gem_receive_msg();
}

GemVal gem_self_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    (void)args;
    (void)argc;
    return gem_int(gem_self_pid());
}
