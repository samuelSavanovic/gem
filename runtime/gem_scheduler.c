/*
 * gem_scheduler.c — Concurrency: scheduler, coroutines, mailbox, spawn/send/receive.
 */

#define MINICORO_IMPL
#include "minicoro.h"

#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

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

/* Remove a specific node from the mailbox given its predecessor */
void gem_mailbox_remove(GemMailbox *mb, GemMsgNode *prev, GemMsgNode *node) {
    if (prev) {
        prev->next = node->next;
    } else {
        mb->head = node->next;
    }
    if (node == mb->tail) {
        mb->tail = prev;
    }
}

/* Monotonic time in milliseconds */
int64_t gem_now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

/* Yield for selective receive — sets deadline and transitions to WAITING */
void gem_selective_yield(int64_t deadline_ms) {
    if (gem_current_pid < 0 || gem_current_pid >= GEM_MAX_PROCS) {
        gem_error("receive: not inside a spawned process");
        return;
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    proc->deadline_ms = deadline_ms;
    proc->timed_out = 0;
    proc->state = GEM_PROC_WAITING;
    mco_yield(proc->coro);
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
    /* Reclaim dead process slots if no free slots available */
    if (pid < 0) {
        for (int i = 0; i < GEM_MAX_PROCS; i++) {
            if (gem_proc_table[i].state == GEM_PROC_DEAD) {
                gem_proc_table[i].state = GEM_PROC_FREE;
                gem_proc_table[i].coro = NULL;
                gem_proc_table[i].mailbox = (GemMailbox){NULL, NULL};
                gem_proc_table[i].monitors = NULL;
                gem_proc_table[i].links = NULL;
                gem_proc_table[i].trap_exit = 0;
                gem_proc_table[i].exit_reason = NULL;
                gem_proc_table[i].pcall_depth = 0;
                pid = i;
                break;
            }
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
    gem_proc_table[pid].links = NULL;
    gem_proc_table[pid].trap_exit = 0;
    gem_proc_table[pid].exit_reason = NULL;
    gem_proc_table[pid].deadline_ms = -1;
    gem_proc_table[pid].timed_out = 0;
    gem_proc_table[pid].pcall_depth = 0;

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
                    gem_propagate_exit(i, reason);
                }
            } else if (proc->state == GEM_PROC_WAITING) {
                has_msg_wait = 1;
                active = 1;
                /* Check deadline for selective receive with after */
                if (proc->deadline_ms >= 0) {
                    int64_t now = gem_now_ms();
                    if (now >= proc->deadline_ms) {
                        proc->timed_out = 1;
                        proc->deadline_ms = -1;
                        proc->state = GEM_PROC_READY;
                        has_ready = 1;
                    }
                }
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

        /* Only mailbox waiters remain. Check if any have deadlines —
           if so, sleep until the earliest deadline, then loop. */
        if (has_msg_wait && !has_io_wait) {
            int64_t earliest = -1;
            for (int i = 0; i < GEM_MAX_PROCS; i++) {
                if (gem_proc_table[i].state == GEM_PROC_WAITING &&
                    gem_proc_table[i].deadline_ms >= 0) {
                    if (earliest < 0 || gem_proc_table[i].deadline_ms < earliest) {
                        earliest = gem_proc_table[i].deadline_ms;
                    }
                }
            }
            if (earliest < 0) break;  /* true deadlock — no deadlines */
            /* Sleep until earliest deadline */
            int64_t now = gem_now_ms();
            int64_t wait_ms = earliest - now;
            if (wait_ms > 0) {
                struct timespec ts_sleep;
                ts_sleep.tv_sec = wait_ms / 1000;
                ts_sleep.tv_nsec = (wait_ms % 1000) * 1000000;
                nanosleep(&ts_sleep, NULL);
            }
            /* Loop back — the deadline check above will mark it READY */
            continue;
        }
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

/* ─── Link API ─── */

/* Add `pid` to `proc`'s links list (no-op if already present). */
static void gem_link_add(GemProcess *proc, int pid) {
    GemLinkNode *node = proc->links;
    while (node) {
        if (node->pid == pid) return;
        node = node->next;
    }
    GemLinkNode *new_node = (GemLinkNode *)GC_MALLOC(sizeof(GemLinkNode));
    new_node->pid = pid;
    new_node->next = proc->links;
    proc->links = new_node;
}

/* Remove `pid` from `proc`'s links list (no-op if absent). */
static void gem_link_remove(GemProcess *proc, int pid) {
    GemLinkNode **cur = &proc->links;
    while (*cur) {
        if ((*cur)->pid == pid) {
            *cur = (*cur)->next;
            return;
        }
        cur = &(*cur)->next;
    }
}

void gem_link_fn(int target_pid) {
    if (target_pid < 0 || target_pid >= GEM_MAX_PROCS) return;
    int caller = gem_current_pid;
    if (caller < 0) {
        gem_error("link: not inside a spawned process");
        return;
    }
    if (caller == target_pid) return;  /* don't link to self */

    GemProcess *target = &gem_proc_table[target_pid];
    GemProcess *self = &gem_proc_table[caller];

    /* If target is already dead, deliver exit signal immediately */
    if (target->state == GEM_PROC_DEAD || target->state == GEM_PROC_FREE) {
        const char *reason = target->exit_reason ? target->exit_reason : "noproc";
        if (self->trap_exit) {
            GemVal msg = gem_table_new();
            gem_table_set(msg, gem_string("tag"), gem_string("EXIT"));
            gem_table_set(msg, gem_string("pid"), gem_int(target_pid));
            gem_table_set(msg, gem_string("reason"), gem_string(reason));
            gem_send_msg(caller, msg);
        } else if (strcmp(reason, "normal") != 0) {
            /* Caller will die when it next runs; raise now via gem_error path.
               Mark caller's reason and let the outer error machinery handle it. */
            gem_error(reason);
        }
        return;
    }

    gem_link_add(self, target_pid);
    gem_link_add(target, caller);
}

void gem_unlink_fn(int target_pid) {
    if (target_pid < 0 || target_pid >= GEM_MAX_PROCS) return;
    int caller = gem_current_pid;
    if (caller < 0) return;

    gem_link_remove(&gem_proc_table[caller], target_pid);
    GemProcess *target = &gem_proc_table[target_pid];
    if (target->state != GEM_PROC_FREE) {
        gem_link_remove(target, caller);
    }
}

/* Propagate an exit signal from `dead_pid` to all its linked processes.
   `dead_pid` must already be marked DEAD before calling. Linked processes
   that don't trap exits and receive a non-normal reason are themselves
   marked DEAD and propagated transitively.

   Special case: if the propagation would kill the currently-running process
   (e.g. kill(other) from the running coro, where the current process is
   linked to `other`), the current coro must not be destroyed mid-flight.
   We defer that until the rest of the propagation is done, then raise an
   error so the coro's setjmp handler unwinds; the scheduler will pick up
   the death and propagate this process's links normally. */
void gem_propagate_exit(int dead_pid, const char *reason) {
    int worklist[GEM_MAX_PROCS];
    const char *reasons[GEM_MAX_PROCS];
    int wl_head = 0, wl_tail = 0;
    worklist[wl_tail] = dead_pid;
    reasons[wl_tail] = reason;
    wl_tail++;

    int self_pid = gem_current_pid;
    int self_kill_pending = 0;
    const char *self_kill_reason = NULL;

    while (wl_head < wl_tail) {
        int pid = worklist[wl_head];
        const char *r = reasons[wl_head];
        wl_head++;

        GemProcess *proc = &gem_proc_table[pid];
        GemLinkNode *link = proc->links;
        proc->links = NULL;  /* clear so repeated propagation is idempotent */

        while (link) {
            int lpid = link->pid;
            link = link->next;
            if (lpid < 0 || lpid >= GEM_MAX_PROCS) continue;
            GemProcess *lproc = &gem_proc_table[lpid];
            if (lproc->state == GEM_PROC_FREE || lproc->state == GEM_PROC_DEAD) continue;

            /* Remove the back-link so a later propagation doesn't re-visit pid */
            gem_link_remove(lproc, pid);

            if (lproc->trap_exit) {
                /* Deliver EXIT message; do not kill */
                GemVal msg = gem_table_new();
                gem_table_set(msg, gem_string("tag"), gem_string("EXIT"));
                gem_table_set(msg, gem_string("pid"), gem_int(pid));
                gem_table_set(msg, gem_string("reason"), gem_string(r));
                gem_send_msg(lpid, msg);
            } else if (strcmp(r, "normal") == 0) {
                /* Don't propagate normal exits to non-trapping links */
                continue;
            } else if (lpid == self_pid) {
                /* Defer killing the active coroutine until the worklist is
                   drained. The scheduler will propagate self's links after
                   its coroutine unwinds via gem_error below. */
                self_kill_pending = 1;
                self_kill_reason = r;
            } else {
                /* Kill the linked process with the same reason */
                lproc->exit_reason = r;
                if (lproc->coro) {
                    mco_destroy(lproc->coro);
                    lproc->coro = NULL;
                }
                gem_deliver_down_messages(lpid, r);
                gem_unregister_name_for_pid(lpid);
                lproc->state = GEM_PROC_DEAD;
                if (wl_tail < GEM_MAX_PROCS) {
                    worklist[wl_tail] = lpid;
                    reasons[wl_tail] = r;
                    wl_tail++;
                }
            }
        }
    }

    if (self_kill_pending) {
        /* Raise inside the running coroutine; proc_jmp catches it. */
        gem_error(self_kill_reason);
    }
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

GemVal gem_time_ms_builtin(void *_env, GemVal *args, int argc) {
    (void)_env; (void)args; (void)argc;
    return gem_int(gem_now_ms());
}

GemVal gem_link_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("link: expected pid (int) argument");
    }
    gem_link_fn((int)args[0].ival);
    return gem_bool(1);
}

GemVal gem_unlink_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("unlink: expected pid (int) argument");
    }
    gem_unlink_fn((int)args[0].ival);
    return gem_bool(1);
}

GemVal gem_spawn_link_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_FN) {
        gem_error("spawn_link: expected function argument");
    }
    int pid = gem_spawn_fn(args[0].fn, args[0].env);

    /* Atomically link before the new process gets a chance to run */
    int caller = gem_current_pid;
    if (caller >= 0 && caller != pid) {
        gem_link_add(&gem_proc_table[caller], pid);
        gem_link_add(&gem_proc_table[pid], caller);
    }
    return gem_int(pid);
}

GemVal gem_process_flag_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING) {
        gem_error("process_flag: expected (flag_name, value)");
    }
    int caller = gem_current_pid;
    if (caller < 0) {
        gem_error("process_flag: not inside a spawned process");
    }
    GemProcess *proc = &gem_proc_table[caller];
    if (strcmp(args[0].sval, "trap_exit") == 0) {
        int old = proc->trap_exit;
        proc->trap_exit = gem_truthy(args[1]) ? 1 : 0;
        return gem_bool(old);
    }
    gem_error("process_flag: unknown flag");
    return GEM_NIL;
}

GemVal gem_exit_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_INT || args[1].type != VAL_STRING) {
        gem_error("exit: expected (pid, reason)");
    }
    int pid = (int)args[0].ival;
    const char *reason = args[1].sval;
    if (pid < 0 || pid >= GEM_MAX_PROCS) return GEM_NIL;
    GemProcess *proc = &gem_proc_table[pid];
    if (proc->state == GEM_PROC_DEAD || proc->state == GEM_PROC_FREE) return GEM_NIL;

    /* Mark the process as dead, deliver DOWN messages, unregister name */
    proc->exit_reason = reason;
    if (proc->coro) {
        mco_destroy(proc->coro);
        proc->coro = NULL;
    }
    gem_deliver_down_messages(pid, reason);
    gem_unregister_name_for_pid(pid);
    proc->state = GEM_PROC_DEAD;
    gem_propagate_exit(pid, reason);
    return gem_bool(1);
}
