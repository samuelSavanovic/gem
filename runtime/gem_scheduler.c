/*
 * gem_scheduler.c — Concurrency: scheduler, coroutines, mailbox, spawn/send/receive.
 */

/* REG_RIP / REG_RSP in <ucontext.h> (stack-overflow rescue) need _GNU_SOURCE. */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define MINICORO_IMPL
#include "minicoro.h"

#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

#include "gem.h"
#include "stb_ds.h"

/* ─── Globals ─── */

GemProcess gem_proc_table[GEM_MAX_PROCS];
int gem_current_pid = -1;
int gem_free_head = 0;
int gem_free_tail = GEM_MAX_PROCS - 1;
int gem_proc_hwm = 0;
GemNameEntry *gem_name_registry = NULL;
GemTimer *gem_timers = NULL;
int gem_timer_count = 0;
static int gem_timer_cap = 0;
static uint64_t gem_timer_next_seq = 0;

/* Lex-compare on (deadline_ms, seq) — earlier deadline wins; same deadline
   resolves to insertion order (FIFO). */
static inline int gem_timer_lt(const GemTimer *a, const GemTimer *b) {
    if (a->deadline_ms != b->deadline_ms) return a->deadline_ms < b->deadline_ms;
    return a->seq < b->seq;
}

static void gem_timer_heap_grow(void) {
    int new_cap = gem_timer_cap == 0 ? 16 : gem_timer_cap * 2;
    gem_timers = (GemTimer *)realloc(gem_timers, sizeof(GemTimer) * new_cap);
    if (!gem_timers) gem_error("send_after: out of memory growing timer heap");
    gem_timer_cap = new_cap;
}

static void gem_timer_sift_up(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (gem_timer_lt(&gem_timers[i], &gem_timers[parent])) {
            GemTimer tmp = gem_timers[i];
            gem_timers[i] = gem_timers[parent];
            gem_timers[parent] = tmp;
            i = parent;
        } else {
            break;
        }
    }
}

static void gem_timer_sift_down(int i) {
    for (;;) {
        int l = 2 * i + 1, r = 2 * i + 2, smallest = i;
        if (l < gem_timer_count && gem_timer_lt(&gem_timers[l], &gem_timers[smallest])) smallest = l;
        if (r < gem_timer_count && gem_timer_lt(&gem_timers[r], &gem_timers[smallest])) smallest = r;
        if (smallest == i) break;
        GemTimer tmp = gem_timers[i];
        gem_timers[i] = gem_timers[smallest];
        gem_timers[smallest] = tmp;
        i = smallest;
    }
}

static void gem_timer_remove_at(int i) {
    int last = gem_timer_count - 1;
    if (i != last) {
        gem_timers[i] = gem_timers[last];
        gem_timer_count--;
        gem_timer_sift_down(i);
        gem_timer_sift_up(i);
    } else {
        gem_timer_count--;
    }
}

/* Account for a timer leaving the heap (fired or cancelled). */
static void gem_timer_untrack(const GemTimer *t) {
    int slot = gem_slot_of_pid(t->target_pid);
    if (slot >= 0) gem_proc_table[slot].pending_timers--;
}

/* Drop every pending timer that targets the process in `slot`. */
static void gem_timer_drop_for_slot(int slot) {
    int64_t pid = gem_pid_of_slot(slot);
    int kept = 0;
    for (int i = 0; i < gem_timer_count; i++) {
        if (gem_timers[i].target_pid == pid) {
            gem_deep_free(gem_timers[i].msg);
        } else {
            gem_timers[kept++] = gem_timers[i];
        }
    }
    gem_timer_count = kept;
    for (int i = kept / 2 - 1; i >= 0; i--) gem_timer_sift_down(i);
    gem_proc_table[slot].pending_timers = 0;
}
int gem_main_pid = -1;

/* Diagnostic counters — printed on process exit via atexit handler.
   Used to disambiguate proc-table exhaustion vs other failure modes
   under load (see benchmarks/stomp/sweep.sh). */
static uint64_t gem_spawn_overflow_count = 0;

static void gem_diag_print_on_exit(void) {
    /* Always emit when GEM_DIAG=1, otherwise only when something
       interesting happened. Keeps default test runs quiet but lets
       benchmark drivers force a baseline line. */
    const char *force = getenv("GEM_DIAG");
    int interesting = gem_spawn_overflow_count > 0;
    if (!interesting && !(force && force[0] == '1')) return;
    fprintf(stderr,
            "gem_diag: spawn_overflow=%llu proc_hwm=%d max_procs=%d\n",
            (unsigned long long)gem_spawn_overflow_count,
            gem_proc_hwm, GEM_MAX_PROCS);
    fflush(stderr);
}

/* Pre-allocated poll scratch arrays (avoid malloc/free per scheduler idle) */
static struct pollfd gem_poll_fds[GEM_MAX_PROCS];
static int gem_poll_pids[GEM_MAX_PROCS];

/* ─── Process stacks ───
 *
 * Every process (main included) runs on a minicoro stack that we map
 * ourselves. minicoro lays a coroutine out as one block,
 *     [mco_coro | _mco_context | storage | stack]
 * with the stack growing down toward the header, so an overflow used to
 * scribble over the coroutine's own bookkeeping and then whatever malloc
 * put next to it. We map the block with mmap instead and pad the (unused)
 * storage area so that it ends in a PROT_NONE guard region directly below
 * the stack:
 *
 *     [header page(s) | guard (GEM_STACK_GUARD_BYTES) | stack ............]
 *     ^ mco_coro        ^ storage tail                ^ stack_lo     top ^
 *
 * mmap'd memory is only committed when touched, so a large stack costs
 * address space, not RAM. Overflow is handled at two levels:
 *
 *   1. Soft: gem_push_frame (gem.h) compares the frame address against
 *      gem_stack_limit = stack_lo + GEM_STACK_RED_ZONE and calls
 *      gem_stack_overflow, which raises an ordinary runtime error. pcall
 *      catches it; uncaught, the process dies with that reason.
 *   2. Hard: C code that recurses on its own (a recursive C function
 *      behind an `extern fn`; the runtime's value copies are iterative)
 *      can run through the red zone into the guard. The
 *      SIGSEGV/SIGBUS handler, on an alternate signal stack, checks that the
 *      fault address is in the running process's guard, then rewrites the
 *      interrupted context to resume in gem_stack_overflow_rescue on a
 *      separate rescue stack. Returning from the handler that way restores
 *      the signal mask and the kernel's alternate-stack state normally. The
 *      rescue longjmps to the process's proc_jmp, so the process dies with
 *      reason "stack overflow" (main prints it and exits 1). The C code that
 *      was interrupted is abandoned midway, so this path is never offered to
 *      pcall. Faults anywhere else get the default action, as before.
 */

#include <sys/mman.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <sys/ucontext.h>
#else
#include <ucontext.h>
#endif

#ifndef GEM_STACK_GUARD_BYTES
/* Larger than one page so a C frame of a few KB that steps over the first
   guard page still lands in the guard rather than in the header below it. */
#define GEM_STACK_GUARD_BYTES (64 * 1024)
#endif

uintptr_t gem_stack_limit = 0;
static int gem_running_slot = -1;   /* slot whose coroutine is running, else -1 */
static size_t gem_page_size = 0;
static size_t gem_guard_size = 0;   /* GEM_STACK_GUARD_BYTES rounded up to pages */

static size_t gem_round_up(size_t n, size_t to) {
    return (n + to - 1) / to * to;
}

#if defined(MCO_USE_ASM) || defined(MCO_USE_UCONTEXT)
#define GEM_CORO_HEADER_BYTES \
    (_mco_align_forward(sizeof(mco_coro), 16) + _mco_align_forward(sizeof(_mco_context), 16))
#else
#error "gem: process stacks assume minicoro's asm or ucontext backend"
#endif

/* Released stacks of the standard spawned-process size, kept for reuse:
   mmap + mprotect + munmap per spawn more than doubles the cost of a
   short-lived process. A cached stack keeps its guard; everything below its
   top GEM_STACK_KEEP_BYTES is handed back to the OS on release, so a cached
   stack holds at most a few pages however deep its last owner went. The
   cache can never hold more stacks than were alive at once, and each is
   trimmed on release, so sizing it to the process table costs address
   space, not memory. */
#ifndef GEM_STACK_CACHE_MAX
#define GEM_STACK_CACHE_MAX GEM_MAX_PROCS
#endif
#define GEM_STACK_KEEP_BYTES (16 * 1024)
/* How far below the kept region to look for touched pages before trimming. */
#define GEM_STACK_PROBE_BYTES (64 * 1024)
static void *gem_stack_cache[GEM_STACK_CACHE_MAX];
static int gem_stack_cache_len = 0;
static size_t gem_stack_cache_block = 0;  /* mapping length the cache holds */
static size_t gem_stack_cache_guard_off = 0;

/* allocator_data carries the offset of the guard from the block start. */
static void *gem_coro_stack_alloc(size_t size, void *udata) {
    size_t guard_off = (size_t)udata;
    size_t len = gem_round_up(size, gem_page_size);
    if (len == gem_stack_cache_block && guard_off == gem_stack_cache_guard_off &&
        gem_stack_cache_len > 0)
        return gem_stack_cache[--gem_stack_cache_len];
    int flags = MAP_PRIVATE | MAP_ANON;
#ifdef MAP_NORESERVE
    flags |= MAP_NORESERVE;
#endif
    void *p = mmap(NULL, len, PROT_READ | PROT_WRITE, flags, -1, 0);
    if (p == MAP_FAILED) return NULL;
    if (mprotect((char *)p + guard_off, gem_guard_size, PROT_NONE) != 0) {
        munmap(p, len);
        return NULL;
    }
    return p;
}

/* Whether any page in the GEM_STACK_PROBE_BYTES just below `keep` is
   resident. A stack is touched downward from its top, so if none is,
   nothing deeper was touched either, and the madvise (which costs several
   microseconds over 8 MB on macOS even when nothing is resident) can be
   skipped. A C frame larger than the probe that skipped all of it could
   leave deeper pages resident; they then stay in the cached stack until it
   is reused, which costs memory, not correctness. */
static int gem_stack_pages_resident(char *lo, char *keep) {
    char *from = keep - GEM_STACK_PROBE_BYTES;
    if (from < lo) from = lo;
    size_t npages = (size_t)(keep - from) / gem_page_size;
    unsigned char vec[GEM_STACK_PROBE_BYTES / 4096];
    if (npages == 0) return 0;
    if (npages > sizeof vec) npages = sizeof vec;
    if (mincore(keep - npages * gem_page_size, npages * gem_page_size, (void *)vec) != 0)
        return 1;  /* can't tell: trim as before */
    for (size_t i = 0; i < npages; i++)
        if (vec[i] & 1) return 1;
    return 0;
}

static void gem_coro_stack_free(void *ptr, size_t size, void *udata) {
    size_t len = gem_round_up(size, gem_page_size);
    size_t guard_off = (size_t)udata;
    if (len == gem_stack_cache_block && guard_off == gem_stack_cache_guard_off &&
        gem_stack_cache_len < GEM_STACK_CACHE_MAX) {
        char *lo = (char *)ptr + guard_off + gem_guard_size;
        /* Measure from the stack top, not the end of the mapping: the
           mapping has a trailing page beyond the stack (minicoro's +16),
           and on 16 KB-page systems that page alone would fill the kept
           region. */
        char *top = lo + gem_round_up(GEM_CORO_STACK_SIZE, gem_page_size);
        char *keep = top - gem_round_up(GEM_STACK_KEEP_BYTES, gem_page_size);
        if (keep > lo && !gem_stack_pages_resident(lo, keep)) keep = lo;
        if (keep > lo) {
#if defined(__APPLE__) && defined(MADV_FREE_REUSABLE)
            madvise(lo, (size_t)(keep - lo), MADV_FREE_REUSABLE);
#else
            madvise(lo, (size_t)(keep - lo), MADV_DONTNEED);
#endif
        }
        gem_stack_cache[gem_stack_cache_len++] = ptr;
        return;
    }
    munmap(ptr, len);
}

static void gem_coro_entry(mco_coro *co);

/* Create a coroutine whose stack has `stack_size` usable bytes above a
   guard region. Stores the stack floor in *stack_lo. */
static mco_result gem_coro_create(mco_coro **out, size_t stack_size, void *user_data,
                                  char **stack_lo) {
    size_t hdr = GEM_CORO_HEADER_BYTES;
    size_t guard_off = gem_round_up(hdr, gem_page_size);
    stack_size = gem_round_up(stack_size, gem_page_size);
    mco_desc desc = mco_desc_init(gem_coro_entry, stack_size);
    /* minicoro puts the stack right after the storage area; size storage so
       it runs to the end of the guard. Both terms are multiples of 16, which
       keeps minicoro's own 16-byte rounding from moving the stack. */
    desc.storage_size = guard_off + gem_guard_size - hdr;
    desc.stack_size = stack_size;
    desc.coro_size = hdr + desc.storage_size + stack_size + 16;
    desc.alloc_cb = gem_coro_stack_alloc;
    desc.dealloc_cb = gem_coro_stack_free;
    desc.allocator_data = (void *)guard_off;
    desc.user_data = user_data;
    mco_result res = mco_create(out, &desc);
    if (res != MCO_SUCCESS) return res;
    char *expect = (char *)*out + guard_off + gem_guard_size;
    if ((char *)(*out)->stack_base != expect) {
        /* minicoro's layout changed under us; refuse rather than run
           without a guard. */
        mco_destroy(*out);
        *out = NULL;
        return MCO_INVALID_ARGUMENTS;
    }
    *stack_lo = expect;
    return MCO_SUCCESS;
}

/* Raised by gem_push_frame when a call would enter the red zone. */
void gem_stack_overflow(const char *name) {
    /* The error path below is plain C (no gem_push_frame), and it runs
       inside the red zone, which exists to leave it room. gem_raise_error
       unwinds to a pcall frame in this process or ends the process, so the
       limit stays armed throughout. */
    char msg[256];
    snprintf(msg, sizeof msg, "stack overflow in %s", gem_user_fn_name(name));
    gem_raise_error(msg);
    abort(); /* unreachable */
}

/* Rescue stack for the hard path. Static storage, not mmap: glibc's
   fortified longjmp refuses to jump to a lower stack address unless it runs
   on the signal stack, and .bss sits below every mmap'd process stack. */
static _Alignas(16) char gem_rescue_stack[64 * 1024];
static volatile int gem_rescue_slot = -1;

static void gem_stack_overflow_rescue(void) {
    int slot = gem_rescue_slot;
    GemProcess *proc = &gem_proc_table[slot];
    gem_current_pid = slot;
    gem_stack_limit = 0;
    proc->stack_overflowed = 1;
    longjmp(proc->proc_jmp, 1);
}

/* Point the interrupted context at gem_stack_overflow_rescue on the rescue
   stack. Returns 0 when this platform has no known layout. */
static int gem_redirect_to_rescue(void *uctx_v) {
    uintptr_t top = (uintptr_t)(gem_rescue_stack + sizeof gem_rescue_stack) & ~(uintptr_t)15;
    ucontext_t *uc = (ucontext_t *)uctx_v;
    (void)uc; (void)top;
#if defined(__APPLE__) && defined(__x86_64__)
    uc->uc_mcontext->__ss.__rip = (uint64_t)(uintptr_t)gem_stack_overflow_rescue;
    uc->uc_mcontext->__ss.__rsp = (uint64_t)(top - 8);  /* as if called */
    return 1;
#elif defined(__APPLE__) && defined(__aarch64__) && defined(__darwin_arm_thread_state64_set_sp)
    __darwin_arm_thread_state64_set_pc_fptr(uc->uc_mcontext->__ss, gem_stack_overflow_rescue);
    __darwin_arm_thread_state64_set_sp(uc->uc_mcontext->__ss, top);
    return 1;
#elif defined(__linux__) && defined(__x86_64__) && defined(REG_RIP)
    uc->uc_mcontext.gregs[REG_RIP] = (greg_t)(uintptr_t)gem_stack_overflow_rescue;
    uc->uc_mcontext.gregs[REG_RSP] = (greg_t)(top - 8);  /* as if called */
    return 1;
#elif defined(__linux__) && defined(__aarch64__)
    uc->uc_mcontext.pc = (uint64_t)(uintptr_t)gem_stack_overflow_rescue;
    uc->uc_mcontext.sp = (uint64_t)top;
    return 1;
#else
    return 0;
#endif
}

/* The scheduler thread, saved at startup. Compared with pthread_self()
   instead of reading a __thread flag: on Darwin the first TLS access from a
   thread can allocate, and the handler may run on a pool worker that
   faulted inside malloc. */
static pthread_t gem_sched_thread;
static volatile int gem_sched_thread_set = 0;

static void gem_fault_handler(int sig, siginfo_t *si, void *uctx) {
    int slot = gem_running_slot;
    if (gem_sched_thread_set && pthread_equal(pthread_self(), gem_sched_thread) && slot >= 0) {
        GemProcess *proc = &gem_proc_table[slot];
        uintptr_t addr = (uintptr_t)si->si_addr;
        uintptr_t lo = (uintptr_t)proc->stack_lo;
        if (lo && addr < lo && addr >= lo - gem_guard_size) {
            gem_rescue_slot = slot;
            if (gem_redirect_to_rescue(uctx)) return;
            /* Unknown platform: longjmp straight out of the handler. Unblock
               the signal first, since setjmp may not have saved the mask. */
            sigset_t set;
            sigemptyset(&set);
            sigaddset(&set, sig);
            sigprocmask(SIG_UNBLOCK, &set, NULL);
            gem_stack_overflow_rescue();
        }
    }
    /* Not a process stack overflow: fall back to the default action. The
       faulting instruction re-executes and the signal kills the program. */
    struct sigaction dfl;
    memset(&dfl, 0, sizeof dfl);
    dfl.sa_handler = SIG_DFL;
    sigemptyset(&dfl.sa_mask);
    sigaction(sig, &dfl, NULL);
}

static void gem_install_overflow_handler(void) {
    gem_page_size = (size_t)sysconf(_SC_PAGESIZE);
    if (gem_page_size == 0 || gem_page_size == (size_t)-1) gem_page_size = 4096;
    gem_guard_size = gem_round_up(GEM_STACK_GUARD_BYTES, gem_page_size);
    gem_sched_thread = pthread_self();
    gem_sched_thread_set = 1;
    /* Mirror gem_coro_create's sizing for a spawned process's block. */
    gem_stack_cache_guard_off = gem_round_up(GEM_CORO_HEADER_BYTES, gem_page_size);
    gem_stack_cache_block = gem_round_up(gem_stack_cache_guard_off + gem_guard_size +
                                         gem_round_up(GEM_CORO_STACK_SIZE, gem_page_size) + 16,
                                         gem_page_size);

    stack_t ss;
    size_t alt = 64 * 1024;
#ifdef SIGSTKSZ
    if ((size_t)SIGSTKSZ > alt) alt = (size_t)SIGSTKSZ;
#endif
    ss.ss_sp = malloc(alt);
    ss.ss_size = alt;
    ss.ss_flags = 0;
    if (!ss.ss_sp || sigaltstack(&ss, NULL) != 0) return;

    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = gem_fault_handler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, NULL);
    /* macOS reports some guard-page hits as SIGBUS. */
    sigaction(SIGBUS, &sa, NULL);
}

void gem_scheduler_init(void) {
    for (int i = 0; i < GEM_MAX_PROCS - 1; i++) {
        gem_proc_table[i].pid = i + 1;
    }
    gem_proc_table[GEM_MAX_PROCS - 1].pid = -1;
    gem_free_head = 0;
    gem_free_tail = GEM_MAX_PROCS - 1;
    /* The name registry receives strings from arbitrary process arenas
       (e.g. a destination gen_server registers `name` where `name` lives
       in the *registry* process's arena, then the registry's per-iteration
       arena reset frees that memory). Default stb_ds mode stores the
       caller's pointer verbatim — so we'd be left with dangling keys.
       Use strdup mode so the table owns its keys; shdel frees them. */
    gem_install_overflow_handler();
    sh_new_strdup(gem_name_registry);
    gem_threadpool_init();
    atexit(gem_diag_print_on_exit);
}

static void gem_free_proc_slot(int pid) {
    GemProcess *proc = &gem_proc_table[pid];

    proc->mailbox = (GemMailbox){NULL, NULL};

    GemLinkNode *l = proc->links;
    while (l) { GemLinkNode *next = l->next; free(l); l = next; }
    proc->links = NULL;

    GemMonitorNode *m = proc->monitors;
    while (m) { GemMonitorNode *next = m->next; free(m); m = next; }
    proc->monitors = NULL;

    /* Free pinned boxes en masse before destroying the arena. Pinned-box
       storage is malloc-backed and independent of arena memory, so order is
       flexible — but conceptually they belong to the same process lifecycle. */
    gem_pin_free_all(proc);

    if (pid != gem_main_pid) gem_globals_free(proc);

    if (proc->pending_timers > 0) gem_timer_drop_for_slot(pid);

    if (pid != gem_main_pid)
        gem_arena_destroy(&proc->arena);

    /* A process killed while waiting on the thread pool still holds its
       reference to the request, so release it on the process's behalf. */
    if (proc->io_request) gem_io_release(proc->io_request);

    proc->state = GEM_PROC_FREE;
    proc->coro = NULL;
    proc->stack_lo = NULL;
    proc->stack_overflowed = 0;
    proc->io_request = NULL;
    proc->trap_exit = 0;
    proc->read_buf = NULL;
    proc->read_buf_cap = 0;
    proc->pcall_depth = 0;
    proc->pending_timers = 0;
    proc->gen++;
    proc->pid = -1;
    if (gem_free_tail >= 0) {
        gem_proc_table[gem_free_tail].pid = pid;
    } else {
        gem_free_head = pid;
    }
    gem_free_tail = pid;
}

#define GEM_REDUCTION_LIMIT 4000

void gem_yield_check(void) {
    if (gem_current_pid < 0) return;
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    proc->reductions++;
    if (proc->reductions >= GEM_REDUCTION_LIMIT) {
        proc->reductions = 0;
        proc->state = GEM_PROC_READY;
        mco_yield(proc->coro);
    }
}

/* Mailbox operations */
static void gem_mailbox_push(GemMailbox *mb, GemVal val) {
    GemMsgNode *node = ALLOC(GemMsgNode);
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
        proc->exit_reason = strdup("normal");
    } else {
        /* longjmp landed here: exit_reason was set by gem_raise_error or
           gem_exit_self, except after a guard-page overflow (the rescue
           path cannot allocate). Re-read the process: locals are not
           reliable after longjmp. */
        proc = &gem_proc_table[gem_current_pid];
        if (proc->stack_overflowed) {
            proc->stack_overflowed = 0;
            /* Name the innermost recorded Gem frame: the guard is reached
               by C code (a builtin or extern fn) that it called. */
            char msg[256];
            if (gem_call_depth > 0) {
                int top = (gem_call_depth <= GEM_MAX_CALL_DEPTH ? gem_call_depth
                                                                : GEM_MAX_CALL_DEPTH) - 1;
                snprintf(msg, sizeof msg, "stack overflow in native code called from %s",
                         gem_user_fn_name(gem_call_stack[top].name));
            } else {
                snprintf(msg, sizeof msg, "stack overflow");
            }
            if (gem_current_pid == gem_main_pid) {
                gem_print_runtime_error(msg);
                exit(1);
            }
            gem_report_process_crash(gem_current_pid, msg);
            if (proc->exit_reason) free((char *)proc->exit_reason);
            proc->exit_reason = strdup(msg);
            proc->pcall_depth = 0;
            gem_call_depth = 0;
        }
    }
}

void gem_exit_self(const char *reason) {
    /* The main process ending abnormally is reported like an uncaught error. */
    if (gem_current_pid == gem_main_pid && strcmp(reason, "normal") != 0) {
        gem_print_runtime_error(reason);
        exit(1);
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    proc->exit_reason = strdup(reason);
    proc->pcall_depth = 0;
    gem_call_depth = 0;
    longjmp(proc->proc_jmp, 1);
}

/* Core API */

int gem_spawn_fn(GemFnPtr fn, void *env) {
    if (gem_free_head < 0) {
        gem_spawn_overflow_count++;
        gem_error("spawn: process table full");
        return -1;
    }
    int pid = gem_free_head;
    gem_free_head = gem_proc_table[pid].pid;
    if (gem_free_head < 0) gem_free_tail = -1;

    if (gem_proc_table[pid].exit_reason) {
        free((char *)gem_proc_table[pid].exit_reason);
        gem_proc_table[pid].exit_reason = NULL;
    }

    gem_arena_init(&gem_proc_table[pid].arena);
    /* Before the copy below, which registers pinned boxes in the child. */
    gem_proc_table[pid].pinned_boxes = NULL;

    int saved = gem_current_pid;
    gem_current_pid = pid;

    GemCoroCtx *ctx = ALLOC(GemCoroCtx);
    /* The child gets its own copy of the closure env, and the parent's
       module state as of now: light slots copied, the rest as references to
       snapshot units it copies from on first use (gem_copy.c, "Module
       globals"). */
    GemVal fn_val = gem_make_fn(fn, env);
    GemVal *child_globals = gem_globals_alloc();
    gem_proc_table[pid].globals = child_globals;
    gem_spawn_module_state(env ? &fn_val : NULL, child_globals, saved);
    ctx->fn = fn_val.fn;
    ctx->env = fn_val.env;

    gem_current_pid = saved;

    mco_coro *co;
    char *stack_lo;
    mco_result res = gem_coro_create(&co, GEM_CORO_STACK_SIZE, ctx, &stack_lo);
    if (res != MCO_SUCCESS) {
        gem_arena_destroy(&gem_proc_table[pid].arena);
        gem_pin_free_all(&gem_proc_table[pid]);
        gem_globals_free(&gem_proc_table[pid]);
        /* Put the slot back on the free list before raising. */
        gem_proc_table[pid].pid = gem_free_head;
        gem_free_head = pid;
        if (gem_free_tail < 0) gem_free_tail = pid;
        gem_error("spawn: coroutine creation failed");
        return -1;
    }
    gem_proc_table[pid].stack_lo = stack_lo;
    gem_proc_table[pid].stack_overflowed = 0;

    gem_proc_table[pid].state = GEM_PROC_READY;
    gem_proc_table[pid].coro = co;
    gem_proc_table[pid].mailbox = (GemMailbox){NULL, NULL};
    gem_proc_table[pid].pid = pid;
    gem_proc_table[pid].io_request = NULL;
    gem_proc_table[pid].monitors = NULL;
    gem_proc_table[pid].links = NULL;
    gem_proc_table[pid].trap_exit = 0;
    gem_proc_table[pid].exit_reason = NULL;
    gem_proc_table[pid].deadline_ms = -1;
    gem_proc_table[pid].timed_out = 0;
    gem_proc_table[pid].reductions = 0;
    gem_proc_table[pid].pcall_depth = 0;
    gem_proc_table[pid].call_depth = 0;

    if (pid >= gem_proc_hwm) gem_proc_hwm = pid + 1;
    return pid;
}

void gem_send_msg(int pid, GemVal val) {
    if (pid < 0 || pid >= GEM_MAX_PROCS) return;
    GemProcess *proc = &gem_proc_table[pid];
    if (proc->state == GEM_PROC_FREE || proc->state == GEM_PROC_DEAD) return;

    int saved = gem_current_pid;
    gem_current_pid = pid;
    GemVal copied = gem_deep_copy(val, saved);
    gem_mailbox_push(&proc->mailbox, copied);
    gem_current_pid = saved;

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

int64_t gem_pid_of_slot(int slot) {
    return (int64_t)slot + gem_proc_table[slot].gen * GEM_MAX_PROCS;
}

int gem_slot_of_pid(int64_t pid) {
    if (pid < 0) return -1;
    int slot = (int)(pid % GEM_MAX_PROCS);
    GemProcess *proc = &gem_proc_table[slot];
    if (proc->gen != pid / GEM_MAX_PROCS) return -1;
    if (proc->state == GEM_PROC_FREE) return -1;
    return slot;
}

void gem_io_pool_yield(void) {
    if (gem_current_pid < 0 || gem_current_pid >= GEM_MAX_PROCS) return;
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    proc->state = GEM_PROC_IO_WAIT;
    mco_yield(proc->coro);
}

void gem_io_yield(int fd, int for_write) {
    if (gem_current_pid < 0 || gem_current_pid >= GEM_MAX_PROCS) {
        return;
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    proc->state = GEM_PROC_IO_WAIT;
    proc->wait_fd = fd;
    proc->wait_write = for_write;
    mco_yield(proc->coro);
}

static void gem_fire_timers(void) {
    int64_t now = gem_now_ms();
    while (gem_timer_count > 0 && now >= gem_timers[0].deadline_ms) {
        GemTimer t = gem_timers[0];
        gem_timer_untrack(&t);
        gem_timer_remove_at(0);
        int pid = gem_slot_of_pid(t.target_pid);
        if (pid >= 0) {
            GemProcess *proc = &gem_proc_table[pid];
            if (proc->state != GEM_PROC_FREE && proc->state != GEM_PROC_DEAD) {
                gem_send_msg(pid, t.msg);
            }
        }
        gem_deep_free(t.msg);
    }
}

static int64_t gem_earliest_timer_deadline(void) {
    return gem_timer_count > 0 ? gem_timers[0].deadline_ms : -1;
}

#ifndef GEM_MAIN_STACK_SIZE
#define GEM_MAIN_STACK_SIZE (8 * 1024 * 1024)
#endif

void gem_run_main(GemFnPtr fn, void *env) {
    if (gem_free_head < 0) {
        gem_error("spawn: process table full");
        return;
    }
    int pid = gem_free_head;
    gem_free_head = gem_proc_table[pid].pid;
    if (gem_free_head < 0) gem_free_tail = -1;

    gem_arena_init(&gem_proc_table[pid].arena);
    gem_main_pid = pid;
    gem_proc_table[pid].globals = gem_globals_alloc();

    int saved = gem_current_pid;
    gem_current_pid = pid;

    GemCoroCtx *ctx = ALLOC(GemCoroCtx);
    ctx->fn = fn;
    ctx->env = env;

    gem_current_pid = saved;

    mco_coro *co;
    char *stack_lo;
    mco_result res = gem_coro_create(&co, GEM_MAIN_STACK_SIZE, ctx, &stack_lo);
    if (res != MCO_SUCCESS) {
        gem_arena_destroy(&gem_proc_table[pid].arena);
        gem_error("gem_run_main: coroutine creation failed");
        return;
    }
    gem_proc_table[pid].stack_lo = stack_lo;
    gem_proc_table[pid].stack_overflowed = 0;

    gem_proc_table[pid].state = GEM_PROC_READY;
    gem_proc_table[pid].coro = co;
    gem_proc_table[pid].mailbox = (GemMailbox){NULL, NULL};
    gem_proc_table[pid].pid = pid;
    gem_proc_table[pid].io_request = NULL;
    gem_proc_table[pid].monitors = NULL;
    gem_proc_table[pid].links = NULL;
    gem_proc_table[pid].trap_exit = 0;
    gem_proc_table[pid].exit_reason = NULL;
    gem_proc_table[pid].deadline_ms = -1;
    gem_proc_table[pid].timed_out = 0;
    gem_proc_table[pid].reductions = 0;
    gem_proc_table[pid].pcall_depth = 0;
    gem_proc_table[pid].call_depth = 0;
    gem_proc_table[pid].pinned_boxes = NULL;

    if (pid >= gem_proc_hwm) gem_proc_hwm = pid + 1;
    gem_run_scheduler();
}

/* Main waits in a receive (no `after`) and nothing can ever send to it.
   Print main's stack (the receive's location is its top frame) and exit
   like an uncaught error in main. */
static void gem_report_main_deadlock(void) {
    GemProcess *mp = &gem_proc_table[gem_main_pid];
    int others = 0;
    for (int i = 0; i < gem_proc_hwm; i++)
        if (i != gem_main_pid && gem_proc_table[i].state == GEM_PROC_WAITING) others++;
    gem_current_pid = gem_main_pid;
    gem_call_stack = mp->call_stack;
    gem_call_depth = mp->call_depth;
    char msg[192];
    if (others > 0)
        snprintf(msg, sizeof msg,
                 "deadlock: main process is waiting in receive and the other %d "
                 "process%s %s also waiting in receive",
                 others, others == 1 ? "" : "es", others == 1 ? "is" : "are");
    else
        snprintf(msg, sizeof msg,
                 "deadlock: main process is waiting in receive and no other "
                 "process can send to it");
    fflush(stdout);
    gem_print_runtime_error(msg);
    exit(1);
}

void gem_run_scheduler(void) {
    int active = 1;

    while (active) {
        active = 0;
        int has_ready = 0;
        int has_fd_wait = 0;
        int has_pool_wait = 0;
        int has_msg_wait = 0;

        gem_fire_timers();
        gem_io_check_completions();

        for (int i = 0; i < gem_proc_hwm; i++) {
            GemProcess *proc = &gem_proc_table[i];

            if (proc->state == GEM_PROC_READY) {
                has_ready = 1;
                active = 1;
                gem_current_pid = i;
                proc->reductions = 0;
                /* gem_call_stack / gem_call_depth are globals but logically
                   per-process — point them at this proc's frames and saved
                   depth before resuming, save the depth back after. Stack
                   traces read the frames. gem_cur_globals likewise points
                   at the running process's module slots. */
                int saved_global_depth = gem_call_depth;
                GemFrame *saved_global_stack = gem_call_stack;
                gem_call_depth = proc->call_depth;
                gem_call_stack = proc->call_stack;
                gem_cur_globals = proc->globals;
                gem_running_slot = i;
                gem_stack_limit = (uintptr_t)proc->stack_lo + GEM_STACK_RED_ZONE;
                mco_resume(proc->coro);
                gem_stack_limit = 0;
                gem_running_slot = -1;
                proc->call_depth = gem_call_depth;
                gem_call_depth = saved_global_depth;
                gem_call_stack = saved_global_stack;
                gem_cur_globals = NULL;

                if (mco_status(proc->coro) == MCO_DEAD) {
                    mco_destroy(proc->coro);
                    proc->coro = NULL;
                    const char *reason = proc->exit_reason ? proc->exit_reason : "normal";
                    proc->state = GEM_PROC_DEAD;
                    gem_deliver_down_messages(i, reason);
                    gem_unregister_name_for_pid(i);
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
                active = 1;
                if (proc->io_request)
                    has_pool_wait = 1;
                else {
                    if (proc->deadline_ms >= 0) {
                        int64_t now = gem_now_ms();
                        if (now >= proc->deadline_ms) {
                            proc->timed_out = 1;
                            proc->deadline_ms = -1;
                            proc->state = GEM_PROC_READY;
                            has_ready = 1;
                            continue;
                        }
                    }
                    has_fd_wait = 1;
                }
            }
        }

        /* Non-blocking poll: surface fd-readiness even when other procs
           are READY. Without this, a continuously-READY proc (e.g. a
           broker reader draining a never-empty TCP buffer) starves any
           proc IO_WAIT'ing on a different fd — the scheduler never
           reaches the blocking poll() below. Discovered via the STOMP
           M6 slow-consumer trace: a broker writer's POLLOUT wait wedged
           permanently after the slow client's kernel buffer first
           filled, because the publisher's reader stayed READY and we
           never polled. The cost is one syscall per scheduler scan
           when any proc is fd-waiting; the build/mark loops are O(hwm)
           but already cheap relative to the proc-scan above. */
        if (has_ready && has_fd_wait) {
            int nfds = 0;
            for (int i = 0; i < gem_proc_hwm; i++) {
                if (gem_proc_table[i].state == GEM_PROC_IO_WAIT &&
                    gem_proc_table[i].io_request == NULL) {
                    gem_poll_fds[nfds].fd = gem_proc_table[i].wait_fd;
                    gem_poll_fds[nfds].events = gem_proc_table[i].wait_write ? POLLOUT : POLLIN;
                    gem_poll_fds[nfds].revents = 0;
                    gem_poll_pids[nfds] = i;
                    nfds++;
                }
            }
            if (nfds > 0 && poll(gem_poll_fds, (nfds_t)nfds, 0) > 0) {
                for (int j = 0; j < nfds; j++) {
                    if (gem_poll_fds[j].revents & (POLLIN | POLLOUT | POLLERR | POLLHUP)) {
                        if (gem_proc_table[gem_poll_pids[j]].state == GEM_PROC_IO_WAIT)
                            gem_proc_table[gem_poll_pids[j]].state = GEM_PROC_READY;
                    }
                }
            }
        }

        /* If we ran at least one READY process, loop immediately —
           the resumed coroutines may have enqueued messages or spawned
           new work. */
        if (has_ready) continue;

        /* Nothing was READY. Block until something becomes ready:
           fd I/O, thread pool completions, or a deadline/timer. */
        if (has_fd_wait || has_pool_wait) {
            int nfds = 0;
            for (int i = 0; i < gem_proc_hwm; i++) {
                if (gem_proc_table[i].state == GEM_PROC_IO_WAIT &&
                    gem_proc_table[i].io_request == NULL) {
                    gem_poll_fds[nfds].fd = gem_proc_table[i].wait_fd;
                    gem_poll_fds[nfds].events = gem_proc_table[i].wait_write ? POLLOUT : POLLIN;
                    gem_poll_fds[nfds].revents = 0;
                    gem_poll_pids[nfds] = i;
                    nfds++;
                }
            }

            /* Include the thread pool wake-pipe so poll returns when
               a worker thread completes an I/O operation. */
            int wake_fd = gem_io_wake_fd();
            int wake_idx = -1;
            if (has_pool_wait && wake_fd >= 0) {
                wake_idx = nfds;
                gem_poll_fds[nfds].fd = wake_fd;
                gem_poll_fds[nfds].events = POLLIN;
                gem_poll_fds[nfds].revents = 0;
                gem_poll_pids[nfds] = -1;
                nfds++;
            }

            /* Compute timeout: earliest deadline among WAITING/IO_WAIT procs and timers */
            int poll_timeout = -1;
            int64_t timer_dl = gem_earliest_timer_deadline();
            int64_t earliest = -1;
            for (int i = 0; i < gem_proc_hwm; i++) {
                if ((gem_proc_table[i].state == GEM_PROC_WAITING ||
                     gem_proc_table[i].state == GEM_PROC_IO_WAIT) &&
                    gem_proc_table[i].deadline_ms >= 0) {
                    if (earliest < 0 || gem_proc_table[i].deadline_ms < earliest)
                        earliest = gem_proc_table[i].deadline_ms;
                }
            }
            if (timer_dl >= 0 && (earliest < 0 || timer_dl < earliest))
                earliest = timer_dl;
            if (earliest >= 0) {
                int64_t now = gem_now_ms();
                int64_t wait_ms = earliest - now;
                poll_timeout = (wait_ms > 0) ? (int)wait_ms : 0;
            }

            int ready = poll(gem_poll_fds, (nfds_t)nfds, poll_timeout);
            if (ready > 0) {
                for (int j = 0; j < nfds; j++) {
                    if (gem_poll_fds[j].revents & (POLLIN | POLLOUT | POLLERR | POLLHUP)) {
                        if (j == wake_idx) continue;
                        gem_proc_table[gem_poll_pids[j]].state = GEM_PROC_READY;
                    }
                }
            }
            continue;
        }

        /* Check for pending timers — they keep the scheduler alive */
        int64_t timer_dl = gem_earliest_timer_deadline();
        if (timer_dl >= 0) active = 1;

        /* Only mailbox waiters (or timers) remain. Check if any have deadlines or
           there are pending timers — sleep until the earliest, then loop. */
        if (has_msg_wait || timer_dl >= 0) {
            int64_t earliest = -1;
            for (int i = 0; i < gem_proc_hwm; i++) {
                if (gem_proc_table[i].state == GEM_PROC_WAITING &&
                    gem_proc_table[i].deadline_ms >= 0) {
                    if (earliest < 0 || gem_proc_table[i].deadline_ms < earliest) {
                        earliest = gem_proc_table[i].deadline_ms;
                    }
                }
            }
            if (timer_dl >= 0 && (earliest < 0 || timer_dl < earliest)) {
                earliest = timer_dl;
            }
            if (earliest < 0) {
                /* True deadlock: every live process waits in a receive
                   without `after`, and no timer, fd or pool job can wake
                   any of them. If main is one of them the program can
                   never finish: report it. If main already finished,
                   the remaining receivers are abandoned and the program
                   ends normally. */
                if (gem_main_pid >= 0 &&
                    gem_proc_table[gem_main_pid].state == GEM_PROC_WAITING)
                    gem_report_main_deadlock();
                break;
            }
            /* Sleep until earliest deadline */
            int64_t now = gem_now_ms();
            int64_t wait_ms = earliest - now;
            if (wait_ms > 0) {
                struct timespec ts_sleep;
                ts_sleep.tv_sec = wait_ms / 1000;
                ts_sleep.tv_nsec = (wait_ms % 1000) * 1000000;
                nanosleep(&ts_sleep, NULL);
            }
            /* Loop back — the deadline/timer check above will mark it READY */
            continue;
        }
    }
    gem_current_pid = -1;
    gem_threadpool_shutdown();
}

/* ─── Monitor API ─── */

void gem_deliver_down_messages(int pid, const char *reason) {
    GemProcess *proc = &gem_proc_table[pid];
    GemMonitorNode *mon = proc->monitors;
    while (mon) {
        GemMonitorNode *next = mon->next;
        GemVal msg = gem_table_new();
        gem_table_set(msg, gem_string("tag"), gem_string("DOWN"));
        gem_table_set(msg, gem_string("pid"), gem_int(gem_pid_of_slot(pid)));
        gem_table_set(msg, gem_string("reason"), gem_string(reason));
        int watcher = gem_slot_of_pid(mon->pid);
        if (watcher >= 0) gem_send_msg(watcher, msg);
        free(mon);
        mon = next;
    }
    proc->monitors = NULL;
}

void gem_monitor_fn(int64_t target_pid) {
    int caller = gem_current_pid;
    if (caller < 0) {
        gem_error("monitor: not inside a spawned process");
        return;
    }
    int target_slot = gem_slot_of_pid(target_pid);

    /* If target is already gone, deliver DOWN immediately */
    if (target_slot < 0 || gem_proc_table[target_slot].state == GEM_PROC_DEAD) {
        const char *reason = "noproc";
        if (target_slot >= 0 && gem_proc_table[target_slot].exit_reason)
            reason = gem_proc_table[target_slot].exit_reason;
        GemVal msg = gem_table_new();
        gem_table_set(msg, gem_string("tag"), gem_string("DOWN"));
        gem_table_set(msg, gem_string("pid"), gem_int(target_pid));
        gem_table_set(msg, gem_string("reason"), gem_string(reason));
        gem_send_msg(caller, msg);
        return;
    }

    GemProcess *target = &gem_proc_table[target_slot];
    int64_t caller_pid = gem_pid_of_slot(caller);

    /* Deduplicate: check if caller is already monitoring target */
    GemMonitorNode *node = target->monitors;
    while (node) {
        if (node->pid == caller_pid) return;  /* already monitoring */
        node = node->next;
    }

    GemMonitorNode *new_node = (GemMonitorNode *)malloc(sizeof(GemMonitorNode));
    new_node->pid = caller_pid;
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
    GemLinkNode *new_node = (GemLinkNode *)malloc(sizeof(GemLinkNode));
    new_node->pid = pid;
    new_node->next = proc->links;
    proc->links = new_node;
}

/* Remove `pid` from `proc`'s links list (no-op if absent). */
static void gem_link_remove(GemProcess *proc, int pid) {
    GemLinkNode **cur = &proc->links;
    while (*cur) {
        if ((*cur)->pid == pid) {
            GemLinkNode *to_free = *cur;
            *cur = (*cur)->next;
            free(to_free);
            return;
        }
        cur = &(*cur)->next;
    }
}

void gem_link_fn(int64_t target_pid) {
    int caller = gem_current_pid;
    if (caller < 0) {
        gem_error("link: not inside a spawned process");
        return;
    }
    int target_slot = gem_slot_of_pid(target_pid);
    if (target_slot == caller) return;  /* don't link to self */

    GemProcess *self = &gem_proc_table[caller];

    /* If target is already gone, deliver exit signal immediately */
    if (target_slot < 0 || gem_proc_table[target_slot].state == GEM_PROC_DEAD) {
        const char *reason = "noproc";
        if (target_slot >= 0 && gem_proc_table[target_slot].exit_reason)
            reason = gem_proc_table[target_slot].exit_reason;
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

    gem_link_add(self, target_slot);
    gem_link_add(&gem_proc_table[target_slot], caller);
}

void gem_unlink_fn(int64_t target_pid) {
    int caller = gem_current_pid;
    if (caller < 0) return;
    int target_slot = gem_slot_of_pid(target_pid);
    if (target_slot < 0) return;

    gem_link_remove(&gem_proc_table[caller], target_slot);
    gem_link_remove(&gem_proc_table[target_slot], caller);
}

/* Propagate an exit signal from `dead_pid` to all its linked processes.
   `dead_pid` must already be marked DEAD before calling. Linked processes
   that don't trap exits and receive a non-normal reason are themselves
   marked DEAD and propagated transitively.

   Special case: if the propagation would kill the currently-running process
   (e.g. kill(other) from the running coro, where the current process is
   linked to `other`), the current coro must not be destroyed mid-flight.
   We defer that until the rest of the propagation is done, then end it with
   gem_exit_self, which unwinds to the coro's setjmp handler past any pcall;
   the scheduler will pick up the death and propagate this process's links
   normally. */
static int gem_exit_worklist[GEM_MAX_PROCS];
static const char *gem_exit_reasons[GEM_MAX_PROCS];

void gem_propagate_exit(int dead_pid, const char *reason) {
    int *worklist = gem_exit_worklist;
    const char **reasons = gem_exit_reasons;
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
        proc->links = NULL;

        while (link) {
            int lpid = link->pid;
            GemLinkNode *link_next = link->next;
            free(link);
            link = link_next;
            if (lpid < 0 || lpid >= GEM_MAX_PROCS) continue;
            GemProcess *lproc = &gem_proc_table[lpid];
            if (lproc->state == GEM_PROC_FREE || lproc->state == GEM_PROC_DEAD) continue;

            /* Remove the back-link so a later propagation doesn't re-visit pid */
            gem_link_remove(lproc, pid);

            if (lproc->trap_exit) {
                /* Deliver EXIT message; do not kill */
                GemVal msg = gem_table_new();
                gem_table_set(msg, gem_string("tag"), gem_string("EXIT"));
                gem_table_set(msg, gem_string("pid"), gem_int(gem_pid_of_slot(pid)));
                gem_table_set(msg, gem_string("reason"), gem_string(r));
                gem_send_msg(lpid, msg);
            } else if (strcmp(r, "normal") == 0) {
                /* Don't propagate normal exits to non-trapping links */
                continue;
            } else if (lpid == self_pid) {
                /* Defer killing the active coroutine until the worklist is
                   drained. The scheduler will propagate self's links after
                   its coroutine unwinds via gem_exit_self below. */
                self_kill_pending = 1;
                self_kill_reason = r;
            } else {
                lproc->exit_reason = strdup(r);
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

    /* Free all worklist entries (original dead + transitively killed) */
    for (int i = 0; i < wl_tail; i++) {
        gem_free_proc_slot(worklist[i]);
    }

    if (self_kill_pending) {
        gem_exit_self(self_kill_reason);
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

/* Crash report for a spawned process dying from an uncaught error (the
   error logger's job in Erlang): the main process's error format, with a
   header naming the process by pid and registered name. Called before the
   process unwinds, so the stack trace is still its own. Exits through
   `kill`/`exit`, linked exit signals and normal returns are not reported. */
void gem_report_process_crash(int slot, const char *msg) {
    char head[192];
    const char *name = NULL;
    for (int i = 0; i < (int)shlen(gem_name_registry); i++) {
        if (gem_name_registry[i].value == slot) { name = gem_name_registry[i].key; break; }
    }
    if (name)
        snprintf(head, sizeof head, "Runtime Error in process %lld \"%.100s\"",
                 (long long)gem_pid_of_slot(slot), name);
    else
        snprintf(head, sizeof head, "Runtime Error in process %lld",
                 (long long)gem_pid_of_slot(slot));
    gem_print_runtime_error_as(head, msg);
}

/* Built-in function wrappers for use from compiled Gem code */

GemVal gem_spawn_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_FN) {
        gem_error("spawn: expected function argument");
    }
    int pid = gem_spawn_fn(args[0].fn, args[0].env);
    return gem_int(gem_pid_of_slot(pid));
}

GemVal gem_send_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) {
        gem_error("send: expected (pid_or_name, value)");
    }
    int pid;
    if (args[0].type == VAL_INT) {
        pid = gem_slot_of_pid(args[0].ival);
        if (pid < 0) return GEM_NIL;  /* process is gone: drop */
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
    return gem_int(gem_pid_of_slot(gem_self_pid()));
}

GemVal gem_monitor_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("monitor: expected pid (int) argument");
    }
    gem_monitor_fn(args[0].ival);
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
        GemMonitorNode *mn = (GemMonitorNode *)malloc(sizeof(GemMonitorNode));
        mn->pid = gem_pid_of_slot(caller);
        mn->next = gem_proc_table[pid].monitors;
        gem_proc_table[pid].monitors = mn;
    }

    GemVal result = gem_table_new();
    gem_table_set(result, gem_string("pid"), gem_int(gem_pid_of_slot(pid)));
    return result;
}

GemVal gem_register_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        gem_error("register: expected (name, pid)");
    }
    int slot = gem_slot_of_pid(args[1].ival);
    if (slot < 0) gem_error("register: invalid pid");
    gem_register_name(args[0].sval, slot);
    return gem_bool(1);
}

GemVal gem_whereis_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        gem_error("whereis: expected name (string) argument");
    }
    int pid = gem_whereis_name(args[0].sval);
    if (pid < 0) return GEM_NIL;
    return gem_int(gem_pid_of_slot(pid));
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
    gem_link_fn(args[0].ival);
    return gem_bool(1);
}

GemVal gem_unlink_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("unlink: expected pid (int) argument");
    }
    gem_unlink_fn(args[0].ival);
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
    return gem_int(gem_pid_of_slot(pid));
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
        gem_error("kill: expected (pid, reason)");
    }
    int pid = gem_slot_of_pid(args[0].ival);
    const char *reason = args[1].sval;
    if (pid < 0) return GEM_NIL;
    GemProcess *proc = &gem_proc_table[pid];
    if (proc->state == GEM_PROC_DEAD || proc->state == GEM_PROC_FREE) return GEM_NIL;

    if (proc->trap_exit) {
        GemVal msg = gem_table_new();
        gem_table_set(msg, gem_string("tag"), gem_string("EXIT"));
        gem_table_set(msg, gem_string("pid"), gem_int(gem_pid_of_slot(gem_current_pid)));
        gem_table_set(msg, gem_string("reason"), gem_string(reason));
        gem_send_msg(pid, msg);
        return gem_bool(1);
    }

    /* The running coroutine can't be destroyed from inside itself. */
    if (pid == gem_current_pid) gem_exit_self(reason);

    proc->exit_reason = strdup(reason);
    if (proc->coro) {
        mco_destroy(proc->coro);
        proc->coro = NULL;
    }
    proc->state = GEM_PROC_DEAD;
    gem_deliver_down_messages(pid, proc->exit_reason);
    gem_unregister_name_for_pid(pid);
    gem_propagate_exit(pid, proc->exit_reason);
    return gem_bool(1);
}

/* ─── Sleep ─── */

GemVal gem_sleep_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("sleep: expected (ms)");
    }
    int64_t delay_ms = args[0].ival;
    if (gem_current_pid < 0) {
        gem_error("sleep: must be called inside a spawned process");
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    /* A message arriving while the process is WAITING makes it READY
       (gem_send_msg), so park again until the deadline has passed. The
       do-while always yields at least once, so sleep(0) yields. */
    int64_t deadline = gem_now_ms() + delay_ms;
    do {
        proc->deadline_ms = deadline;
        proc->timed_out = 0;
        proc->state = GEM_PROC_WAITING;
        mco_yield(proc->coro);
    } while (gem_now_ms() < deadline);
    proc->timed_out = 0;
    proc->deadline_ms = -1;
    return GEM_NIL;
}

/* ─── Timer API ─── */

GemVal gem_send_after_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 3 || args[0].type != VAL_INT || args[2].type != VAL_INT) {
        gem_error("send_after: expected (pid, msg, delay_ms)");
    }
    int64_t pid = args[0].ival;
    GemVal msg = args[1];
    int64_t delay_ms = args[2].ival;

    if (gem_timer_count >= gem_timer_cap) gem_timer_heap_grow();

    GemVal ref = gem_make_ref();
    /* A timer for a process that has already exited would never deliver. */
    int slot = gem_slot_of_pid(pid);
    if (slot < 0 || gem_proc_table[slot].state == GEM_PROC_DEAD) return ref;
    gem_proc_table[slot].pending_timers++;
    int i = gem_timer_count++;
    gem_timers[i].ref = ref.rval;
    gem_timers[i].target_pid = pid;
    gem_timers[i].msg = gem_deep_copy_malloc(msg);
    gem_timers[i].deadline_ms = gem_now_ms() + delay_ms;
    gem_timers[i].seq = gem_timer_next_seq++;
    gem_timer_sift_up(i);
    return ref;
}

GemVal gem_cancel_timer_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_REF) {
        gem_error("cancel_timer: expected ref argument");
    }
    int64_t ref = args[0].rval;
    for (int i = 0; i < gem_timer_count; i++) {
        if (gem_timers[i].ref == ref) {
            gem_timer_untrack(&gem_timers[i]);
            gem_deep_free(gem_timers[i].msg);
            gem_timer_remove_at(i);
            return gem_bool(1);
        }
    }
    return gem_bool(0);
}

/* ─── Process Introspection ─── */

GemVal gem_processes_builtin(void *_env, GemVal *args, int argc) {
    (void)_env; (void)args; (void)argc;
    GemVal list = gem_table_new();
    GemTable *t = list.table;
    for (int i = 0; i < GEM_MAX_PROCS; i++) {
        if (gem_proc_table[i].state != GEM_PROC_FREE &&
            gem_proc_table[i].state != GEM_PROC_DEAD) {
            if (t->len >= t->cap) gem_table_grow(t);
            t->keys[t->len] = gem_int(t->len);
            t->vals[t->len] = gem_int(gem_pid_of_slot(i));
            t->len++;
        }
    }
    return list;
}

GemVal gem_process_info_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("process_info: expected pid (int) argument");
    }
    int pid = gem_slot_of_pid(args[0].ival);
    if (pid < 0) return GEM_NIL;

    GemProcess *proc = &gem_proc_table[pid];

    GemVal info = gem_table_new();

    /* state */
    const char *state_str;
    switch (proc->state) {
        case GEM_PROC_READY:    state_str = "ready"; break;
        case GEM_PROC_WAITING:  state_str = "waiting"; break;
        case GEM_PROC_IO_WAIT:  state_str = "waiting"; break;
        case GEM_PROC_DEAD:     state_str = "dead"; break;
        default:                state_str = "free"; break;
    }
    gem_table_set(info, gem_string("state"), gem_string(state_str));

    /* mailbox_len */
    int mlen = 0;
    GemMsgNode *node = proc->mailbox.head;
    while (node) { mlen++; node = node->next; }
    gem_table_set(info, gem_string("mailbox_len"), gem_int(mlen));

    /* links — array of pids */
    GemVal links = gem_table_new();
    GemLinkNode *lnode = proc->links;
    int li = 0;
    while (lnode) {
        gem_table_set(links, gem_int(li++), gem_int(gem_pid_of_slot(lnode->pid)));
        lnode = lnode->next;
    }
    gem_table_set(info, gem_string("links"), links);

    /* monitors — array of pids */
    GemVal monitors = gem_table_new();
    GemMonitorNode *mnode = proc->monitors;
    int mi = 0;
    while (mnode) {
        gem_table_set(monitors, gem_int(mi++), gem_int(mnode->pid));
        mnode = mnode->next;
    }
    gem_table_set(info, gem_string("monitors"), monitors);

    /* trap_exit */
    gem_table_set(info, gem_string("trap_exit"), gem_bool(proc->trap_exit));

    /* exit_reason */
    if (proc->exit_reason) {
        gem_table_set(info, gem_string("exit_reason"), gem_string(proc->exit_reason));
    } else {
        gem_table_set(info, gem_string("exit_reason"), GEM_NIL);
    }

    return info;
}
