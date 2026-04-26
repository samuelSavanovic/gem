/*
 * gem_scheduler.c — Concurrency: scheduler, coroutines, mailbox, spawn/send/receive.
 */

#define MINICORO_IMPL
#include "minicoro.h"

#include <poll.h>
#include <fcntl.h>
#include <errno.h>

#include "gem.h"
#include "stb_ds.h"

/* ─── Globals ─── */

GemProcess gem_proc_table[GEM_MAX_PROCS];
int gem_current_pid = -1;
GemNameEntry *gem_name_registry = NULL;

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

/* Coroutine entry point — calls the GemFnPtr stored in user_data.
   Wraps the call in setjmp for crash isolation: if gem_error is called
   inside a coroutine (and no pcall catches it), we longjmp here instead
   of exit(1), mark the process DEAD, and let the scheduler continue. */
typedef struct {
    GemFnPtr fn;
    void *env;
} GemCoroCtx;

static void gem_coro_entry(mco_coro *co) {
    GemCoroCtx *ctx = (GemCoroCtx *)mco_get_user_data(co);
    int pid = gem_current_pid;
    GemProcess *proc = &gem_proc_table[pid];

    if (setjmp(proc->proc_jmp) == 0) {
        /* Normal path */
        ctx->fn(ctx->env, NULL, 0);
        proc->exit_reason = "normal";
    }
    /* If longjmp landed here, exit_reason was already set by gem_raise_error */
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
    gem_proc_table[pid].monitors = NULL;
    gem_proc_table[pid].exit_reason = NULL;

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
                    const char *reason = proc->exit_reason ? proc->exit_reason : "normal";
                    gem_deliver_down_messages(i, reason);
                    gem_unregister_name_for_pid(i);
                    proc->state = GEM_PROC_DEAD;
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

/* ─── Monitor API ─── */

void gem_deliver_down_messages(int pid, const char *reason) {
    GemProcess *proc = &gem_proc_table[pid];
    GemMonitorNode *mon = proc->monitors;
    while (mon) {
        /* Build {tag: "DOWN", pid: <pid>, reason: <reason>} */
        GemVal msg = gem_table_new();
        gem_table_set(msg, gem_string("tag"), gem_string("DOWN"));
        gem_table_set(msg, gem_string("pid"), gem_int(pid));
        gem_table_set(msg, gem_string("reason"), gem_string(reason));
        gem_send_msg(mon->pid, msg);
        mon = mon->next;
    }
    proc->monitors = NULL;
}

void gem_monitor_fn(int target_pid) {
    if (target_pid < 0 || target_pid >= GEM_MAX_PROCS) return;
    int caller = gem_current_pid;
    if (caller < 0) {
        gem_error("monitor: not inside a spawned process");
        return;
    }
    GemProcess *target = &gem_proc_table[target_pid];

    /* If target is already dead, deliver DOWN immediately */
    if (target->state == GEM_PROC_DEAD || target->state == GEM_PROC_FREE) {
        const char *reason = target->exit_reason ? target->exit_reason : "noproc";
        GemVal msg = gem_table_new();
        gem_table_set(msg, gem_string("tag"), gem_string("DOWN"));
        gem_table_set(msg, gem_string("pid"), gem_int(target_pid));
        gem_table_set(msg, gem_string("reason"), gem_string(reason));
        gem_send_msg(caller, msg);
        return;
    }

    /* Deduplicate: check if caller is already monitoring target */
    GemMonitorNode *node = target->monitors;
    while (node) {
        if (node->pid == caller) return;  /* already monitoring */
        node = node->next;
    }

    /* Add caller to target's monitor list */
    GemMonitorNode *new_node = (GemMonitorNode *)GC_MALLOC(sizeof(GemMonitorNode));
    new_node->pid = caller;
    new_node->next = target->monitors;
    target->monitors = new_node;
}

/* ─── Named Process Registry ─── */

void gem_register_name(const char *name, int pid) {
    if (pid < 0 || pid >= GEM_MAX_PROCS) {
        gem_error("register: invalid pid");
        return;
    }
    /* Check if name is already taken */
    ptrdiff_t idx = shgeti(gem_name_registry, name);
    if (idx >= 0) {
        gem_error("register: name already taken");
        return;
    }
    shput(gem_name_registry, name, pid);
}

int gem_whereis_name(const char *name) {
    ptrdiff_t idx = shgeti(gem_name_registry, name);
    if (idx >= 0) return gem_name_registry[idx].value;
    return -1;
}

void gem_unregister_name_for_pid(int pid) {
    /* Scan registry for entries pointing to this pid and remove them */
    for (int i = (int)shlen(gem_name_registry) - 1; i >= 0; i--) {
        if (gem_name_registry[i].value == pid) {
            shdel(gem_name_registry, gem_name_registry[i].key);
        }
    }
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
    if (argc < 2) {
        gem_error("send: expected (pid_or_name, value)");
    }
    int pid;
    if (args[0].type == VAL_INT) {
        pid = (int)args[0].ival;
    } else if (args[0].type == VAL_STRING) {
        pid = gem_whereis_name(args[0].sval);
        if (pid < 0) {
            gem_error("send: no process registered with that name");
            return GEM_NIL;
        }
    } else {
        gem_error("send: first argument must be pid (int) or name (string)");
        return GEM_NIL;
    }
    gem_send_msg(pid, args[1]);
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

GemVal gem_monitor_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("monitor: expected pid (int) argument");
    }
    gem_monitor_fn((int)args[0].ival);
    return gem_bool(1);
}

GemVal gem_spawn_monitor_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_FN) {
        gem_error("spawn_monitor: expected function argument");
    }
    int pid = gem_spawn_fn(args[0].fn, args[0].env);

    /* Atomically register monitor before the new process gets a chance to run */
    int caller = gem_current_pid;
    if (caller >= 0) {
        GemMonitorNode *new_node = (GemMonitorNode *)GC_MALLOC(sizeof(GemMonitorNode));
        new_node->pid = caller;
        new_node->next = gem_proc_table[pid].monitors;
        gem_proc_table[pid].monitors = new_node;
    }

    GemVal result = gem_table_new();
    gem_table_set(result, gem_string("pid"), gem_int(pid));
    return result;
}

GemVal gem_register_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        gem_error("register: expected (name, pid)");
    }
    gem_register_name(args[0].sval, (int)args[1].ival);
    return gem_bool(1);
}

GemVal gem_whereis_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        gem_error("whereis: expected name (string) argument");
    }
    int pid = gem_whereis_name(args[0].sval);
    if (pid < 0) return GEM_NIL;
    return gem_int(pid);
}
