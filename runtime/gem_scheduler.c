/*
 * gem_scheduler.c — Concurrency: scheduler, coroutines, mailbox, spawn/send/receive.
 */

/* REG_RIP / REG_RSP in <ucontext.h> (stack-overflow rescue) need _GNU_SOURCE. */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define MINICORO_IMPL
/* minicoro's debug log goes to stdout (puts), into the program's output;
   every failure it logs is reported by the caller anyway. */
#define MCO_LOG(s) ((void)0)
#include "minicoro.h"

#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

#include <limits.h>
#include "gem.h"
#include "stb_ds.h"

/* ─── Globals ─── */

GemProcess *gem_proc_table = NULL;
int gem_current_pid = -1;
/* Freed slots, FIFO, linked through GemProcess.pid (gem_proc_alloc_slot). */
int gem_free_head = -1;
int gem_free_tail = -1;
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
static int gem_proc_cap = 0;        /* slots reserved: the process limit */

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
            gem_proc_hwm, gem_proc_cap);
    fflush(stderr);
}

/* Poll scratch arrays, grown with the fd waiter list (one entry per fd
   waiter plus the thread pool's wake pipe). */
static struct pollfd *gem_poll_fds = NULL;
static int *gem_poll_pids = NULL;

/* revents that make an fd waiter ready. POLLNVAL counts: an fd closed
   without tcp_close (which wakes its waiters itself, gem_io_fd_closed), e.g.
   by C code behind an extern fn, would otherwise report POLLNVAL on every
   pass and spin the scheduler; the waiter's tcp builtin then fails with
   EBADF and raises. */
#define GEM_POLL_WAKE (POLLIN | POLLOUT | POLLERR | POLLHUP | POLLNVAL)

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
   cache can never hold more stacks than were alive at once. It is capped
   (not sized to the process table) so that a burst of many thousands of
   processes doesn't leave their stacks, a few pages and two mappings each,
   cached for good; past the cap, churn maps and unmaps a stack per spawn. */
#ifndef GEM_STACK_CACHE_MAX
#define GEM_STACK_CACHE_MAX 1024
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
    snprintf(msg, sizeof msg, "stack overflow in %s", name);
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

/* ─── Process table ───
 *
 * gem_proc_table is one reservation of address space for GEM_MAX_PROCS
 * slots, mapped PROT_NONE so it costs neither memory nor commit charge.
 * Slots are made accessible in chunks as the high-water mark gem_proc_hwm
 * grows, and the table never moves, so a GemProcess * taken before a spawn
 * stays valid after it. The GEM_MAX_PROCS environment variable can lower
 * the limit (gem_proc_limit). If the address space for every slot can't be
 * reserved (a ulimit -v), the reservation halves until it fits.
 *
 * A new slot comes from the high-water mark until it reaches
 * GEM_PROC_REUSE_MIN, then from the freed slots (FIFO), then from the
 * high-water mark again. So a program that never has more than
 * GEM_PROC_REUSE_MIN - 1 processes alive gets the same slots (and pids) as
 * with the old fixed table of that size, and the table only grows as far
 * as the most processes alive at once (plus the reuse floor).
 */

#ifndef GEM_PROC_REUSE_MIN
#define GEM_PROC_REUSE_MIN 1024
#endif
/* Slots made accessible at a time. */
#define GEM_PROC_COMMIT_SLOTS 64

static int gem_procs_alive = 0;     /* slots handed out and not yet freed */
static int gem_proc_committed = 0;  /* slots accessible (a prefix of the table) */
static size_t gem_proc_committed_bytes = 0;

/* gem_propagate_exit's worklist: each slot at most once, so one entry per
   accessible slot. */
static int *gem_exit_worklist = NULL;
static const char **gem_exit_reasons = NULL;

/* The process limit: GEM_MAX_PROCS, or the GEM_MAX_PROCS environment
   variable when it is set to a smaller number (at least 2: main and one
   more). */
static int gem_proc_limit(void) {
    const char *env = getenv("GEM_MAX_PROCS");
    if (!env || !*env) return GEM_MAX_PROCS;
    char *end;
    long n = strtol(env, &end, 10);
    if (*end != '\0' || n < 2) {
        fprintf(stderr, "gem: GEM_MAX_PROCS must be a number of processes (at least 2), got '%s'\n", env);
        exit(1);
    }
    return n < GEM_MAX_PROCS ? (int)n : GEM_MAX_PROCS;
}

static void gem_proc_table_reserve(void) {
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    int flags = MAP_PRIVATE | MAP_ANON;
#ifdef MAP_NORESERVE
    flags |= MAP_NORESERVE;
#endif
    int limit = gem_proc_limit();
    for (int cap = limit; cap >= 2; cap = cap > GEM_PROC_COMMIT_SLOTS ? cap / 2 : 1) {
        size_t len = ((size_t)cap * sizeof(GemProcess) + page - 1) / page * page;
        void *p = mmap(NULL, len, PROT_NONE, flags, -1, 0);
        if (p != MAP_FAILED) {
            gem_proc_table = (GemProcess *)p;
            gem_proc_cap = cap;
            return;
        }
    }
    fprintf(stderr, "gem: cannot reserve the process table\n");
    exit(1);
}

/* Make slots [0, n) accessible. Returns 0 when the memory isn't there. */
static int gem_proc_table_commit(int n) {
    if (n <= gem_proc_committed) return 1;
    int want = (n + GEM_PROC_COMMIT_SLOTS - 1) / GEM_PROC_COMMIT_SLOTS * GEM_PROC_COMMIT_SLOTS;
    if (want > gem_proc_cap) want = gem_proc_cap;
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    size_t bytes = ((size_t)want * sizeof(GemProcess) + page - 1) / page * page;
    if (mprotect((char *)gem_proc_table + gem_proc_committed_bytes,
                 bytes - gem_proc_committed_bytes, PROT_READ | PROT_WRITE) != 0)
        return 0;
    int *wl = (int *)realloc(gem_exit_worklist, sizeof(int) * (size_t)want);
    if (!wl) return 0;
    gem_exit_worklist = wl;
    const char **rs = (const char **)realloc(gem_exit_reasons, sizeof(char *) * (size_t)want);
    if (!rs) return 0;
    gem_exit_reasons = rs;
    gem_proc_committed_bytes = bytes;
    gem_proc_committed = want;
    return 1;
}

/* A free slot (state GEM_PROC_FREE, below gem_proc_hwm on return), or -1
   when the table is full. */
static int gem_proc_alloc_slot(void) {
    int from_free = gem_free_head >= 0 &&
                    (gem_proc_hwm >= GEM_PROC_REUSE_MIN || gem_proc_hwm >= gem_proc_cap);
    if (!from_free && gem_proc_hwm < gem_proc_cap) {
        if (!gem_proc_table_commit(gem_proc_hwm + 1)) {
            if (gem_free_head < 0) return -1;
        } else {
            gem_procs_alive++;
            return gem_proc_hwm++;
        }
    }
    if (gem_free_head < 0) return -1;
    int slot = gem_free_head;
    gem_free_head = gem_proc_table[slot].pid;
    if (gem_free_head < 0) gem_free_tail = -1;
    gem_procs_alive++;
    return slot;
}

/* ─── Run state ───
 *
 * The scheduler never scans the process table. It keeps:
 *   - the ready set: a three-level bitmap over slots, so a pass can run the
 *     READY processes in slot order (as the old full-table scan did) at a
 *     cost proportional to the READY ones;
 *   - the deadline heap: WAITING processes (receive ... after, sleep) and
 *     fd waiters (a tcp timeout) with a deadline, keyed by deadline_ms;
 *   - the fd waiter list (polled) and the pool waiter list (checked when
 *     the thread pool signals a completion);
 *   - a count of WAITING processes.
 * Every state change goes through gem_proc_set_state, which keeps them in
 * step with GemProcess.state.
 */

#define GEM_RDY_L0 ((GEM_MAX_PROCS + 63) / 64)
#define GEM_RDY_L1 ((GEM_RDY_L0 + 63) / 64)
#define GEM_RDY_L2 ((GEM_RDY_L1 + 63) / 64)
static uint64_t gem_rdy_l0[GEM_RDY_L0];
static uint64_t gem_rdy_l1[GEM_RDY_L1];
static uint64_t gem_rdy_l2[GEM_RDY_L2];

static void gem_ready_set(int i) {
    gem_rdy_l0[i >> 6] |= 1ULL << (i & 63);
    gem_rdy_l1[i >> 12] |= 1ULL << ((i >> 6) & 63);
    gem_rdy_l2[i >> 18] |= 1ULL << ((i >> 12) & 63);
}

static void gem_ready_clear(int i) {
    int w0 = i >> 6;
    gem_rdy_l0[w0] &= ~(1ULL << (i & 63));
    if (gem_rdy_l0[w0]) return;
    int w1 = w0 >> 6;
    gem_rdy_l1[w1] &= ~(1ULL << (w0 & 63));
    if (gem_rdy_l1[w1]) return;
    gem_rdy_l2[w1 >> 6] &= ~(1ULL << (w1 & 63));
}

/* The lowest READY slot >= i, or -1. */
static int gem_ready_next(int i) {
    if (i < 0) i = 0;
    if (i >= GEM_RDY_L0 * 64) return -1;
    int w0 = i >> 6;
    uint64_t m = gem_rdy_l0[w0] & (~0ULL << (i & 63));
    if (m) return (w0 << 6) + __builtin_ctzll(m);
    int j = w0 + 1;                       /* next l0 word */
    if (j >= GEM_RDY_L0) return -1;
    int w1 = j >> 6;
    m = gem_rdy_l1[w1] & (~0ULL << (j & 63));
    if (m) {
        int a = (w1 << 6) + __builtin_ctzll(m);
        return (a << 6) + __builtin_ctzll(gem_rdy_l0[a]);
    }
    int k = w1 + 1;                       /* next l1 word */
    if (k >= GEM_RDY_L1) return -1;
    for (int w2 = k >> 6; w2 < GEM_RDY_L2; w2++) {
        m = gem_rdy_l2[w2];
        if (w2 == (k >> 6)) m &= ~0ULL << (k & 63);
        if (m) {
            int b = (w2 << 6) + __builtin_ctzll(m);
            int a = (b << 6) + __builtin_ctzll(gem_rdy_l1[b]);
            return (a << 6) + __builtin_ctzll(gem_rdy_l0[a]);
        }
    }
    return -1;
}

typedef struct { int64_t dl; int slot; } GemDeadline;
static GemDeadline *gem_dl_heap = NULL;
static int gem_dl_n = 0, gem_dl_cap = 0;

static void gem_dl_place(int i, GemDeadline e) {
    gem_dl_heap[i] = e;
    gem_proc_table[e.slot].dl_idx = i + 1;
}

static void gem_dl_sift_up(int i) {
    GemDeadline e = gem_dl_heap[i];
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (gem_dl_heap[parent].dl <= e.dl) break;
        gem_dl_place(i, gem_dl_heap[parent]);
        i = parent;
    }
    gem_dl_place(i, e);
}

static void gem_dl_sift_down(int i) {
    GemDeadline e = gem_dl_heap[i];
    for (;;) {
        int l = 2 * i + 1, r = l + 1, c = l;
        if (l >= gem_dl_n) break;
        if (r < gem_dl_n && gem_dl_heap[r].dl < gem_dl_heap[l].dl) c = r;
        if (gem_dl_heap[c].dl >= e.dl) break;
        gem_dl_place(i, gem_dl_heap[c]);
        i = c;
    }
    gem_dl_place(i, e);
}

static void gem_dl_push(int slot, int64_t dl) {
    if (gem_dl_n == gem_dl_cap) {
        int cap = gem_dl_cap ? gem_dl_cap * 2 : 64;
        GemDeadline *h = (GemDeadline *)realloc(gem_dl_heap, sizeof(GemDeadline) * (size_t)cap);
        if (!h) { fprintf(stderr, "gem: out of memory (deadline heap)\n"); exit(1); }
        gem_dl_heap = h;
        gem_dl_cap = cap;
    }
    gem_dl_heap[gem_dl_n++] = (GemDeadline){dl, slot};
    gem_dl_sift_up(gem_dl_n - 1);
}

static void gem_dl_remove(int slot) {
    int i = gem_proc_table[slot].dl_idx - 1;
    gem_proc_table[slot].dl_idx = 0;
    gem_dl_n--;
    if (i == gem_dl_n) return;
    gem_dl_heap[i] = gem_dl_heap[gem_dl_n];
    gem_dl_sift_down(i);
    gem_dl_sift_up(gem_proc_table[gem_dl_heap[i].slot].dl_idx - 1);
}

typedef struct { int *slots; int n, cap; } GemWaitList;
static GemWaitList gem_fd_waiters = {NULL, 0, 0};
static GemWaitList gem_pool_waiters = {NULL, 0, 0};
static int gem_poll_cap = 0;
static int gem_n_msg_wait = 0;

static void gem_waitlist_add(GemWaitList *wl, int slot) {
    if (wl->n == wl->cap) {
        int cap = wl->cap ? wl->cap * 2 : 64;
        int *a = (int *)realloc(wl->slots, sizeof(int) * (size_t)cap);
        if (!a) { fprintf(stderr, "gem: out of memory (waiter list)\n"); exit(1); }
        wl->slots = a;
        wl->cap = cap;
    }
    gem_proc_table[slot].wait_idx = wl->n;
    wl->slots[wl->n++] = slot;
    if (wl == &gem_fd_waiters && gem_poll_cap < wl->n + 1) {
        int cap = wl->cap + 1;
        struct pollfd *f = (struct pollfd *)realloc(gem_poll_fds, sizeof(struct pollfd) * (size_t)cap);
        if (f) gem_poll_fds = f;
        int *pp = (int *)realloc(gem_poll_pids, sizeof(int) * (size_t)cap);
        if (pp) gem_poll_pids = pp;
        if (!f || !pp) { fprintf(stderr, "gem: out of memory (poll set)\n"); exit(1); }
        gem_poll_cap = cap;
    }
}

static void gem_waitlist_remove(GemWaitList *wl, int slot) {
    int i = gem_proc_table[slot].wait_idx;
    int last = wl->slots[--wl->n];
    wl->slots[i] = last;
    gem_proc_table[last].wait_idx = i;
}

/* Move the process in `slot` to state `st`, keeping the run-state
   structures in step. Entering WAITING or an fd wait reads deadline_ms;
   entering IO_WAIT reads io_request (set: a thread pool wait). */
static void gem_proc_set_state(int slot, GemProcState st) {
    GemProcess *p = &gem_proc_table[slot];
    switch (p->state) {
        case GEM_PROC_READY:   gem_ready_clear(slot); break;
        case GEM_PROC_WAITING: gem_n_msg_wait--; break;
        case GEM_PROC_IO_WAIT:
            gem_waitlist_remove(p->wait_kind == GEM_WAIT_POOL ? &gem_pool_waiters
                                                              : &gem_fd_waiters, slot);
            p->wait_kind = GEM_WAIT_NONE;
            break;
        default: break;
    }
    if (p->dl_idx) gem_dl_remove(slot);
    p->state = st;
    switch (st) {
        case GEM_PROC_READY: gem_ready_set(slot); break;
        case GEM_PROC_WAITING:
            gem_n_msg_wait++;
            if (p->deadline_ms >= 0) gem_dl_push(slot, p->deadline_ms);
            break;
        case GEM_PROC_IO_WAIT:
            if (p->io_request) {
                p->wait_kind = GEM_WAIT_POOL;
                gem_waitlist_add(&gem_pool_waiters, slot);
            } else {
                p->wait_kind = GEM_WAIT_FD;
                gem_waitlist_add(&gem_fd_waiters, slot);
                if (p->deadline_ms >= 0) gem_dl_push(slot, p->deadline_ms);
            }
            break;
        default: break;
    }
}

/* Wake the processes with the earliest deadline, if it has passed
   (timed_out set). One deadline per pass: when the scheduler falls behind,
   processes whose deadlines have all passed still wake in deadline order,
   the earliest in this pass and the next in the next one, instead of
   together in slot order. */
static void gem_expire_deadlines(void) {
    if (gem_dl_n == 0) return;
    int64_t due = gem_dl_heap[0].dl;
    if (due > gem_now_ms()) return;
    while (gem_dl_n > 0 && gem_dl_heap[0].dl == due) {
        int slot = gem_dl_heap[0].slot;
        GemProcess *p = &gem_proc_table[slot];
        p->timed_out = 1;
        p->deadline_ms = -1;
        gem_proc_set_state(slot, GEM_PROC_READY);
    }
}

/* Wake the pool waiters whose request is done. */
static void gem_wake_pool_waiters(void) {
    for (int k = gem_pool_waiters.n - 1; k >= 0; k--) {
        int slot = gem_pool_waiters.slots[k];
        GemIORequest *req = gem_proc_table[slot].io_request;
        if (req && __atomic_load_n(&req->done, __ATOMIC_ACQUIRE))
            gem_proc_set_state(slot, GEM_PROC_READY);
    }
}

/* Fill the poll scratch arrays with the fd waiters; returns the count. */
static int gem_poll_fill(void) {
    int nfds = 0;
    for (int k = 0; k < gem_fd_waiters.n; k++) {
        int slot = gem_fd_waiters.slots[k];
        GemProcess *p = &gem_proc_table[slot];
        gem_poll_fds[nfds].fd = p->wait_fd;
        gem_poll_fds[nfds].events = p->wait_write ? POLLOUT : POLLIN;
        gem_poll_fds[nfds].revents = 0;
        gem_poll_pids[nfds] = slot;
        nfds++;
    }
    return nfds;
}

/* Make READY the polled fd waiters whose fd reported an event. */
static void gem_poll_wake(int nfds) {
    for (int j = 0; j < nfds; j++) {
        int slot = gem_poll_pids[j];
        if (slot < 0 || !(gem_poll_fds[j].revents & GEM_POLL_WAKE)) continue;
        GemProcess *p = &gem_proc_table[slot];
        if (p->state == GEM_PROC_IO_WAIT && p->wait_kind == GEM_WAIT_FD)
            gem_proc_set_state(slot, GEM_PROC_READY);
    }
}

void gem_scheduler_init(void) {
    gem_proc_table_reserve();
    gem_poll_cap = 65;
    gem_poll_fds = (struct pollfd *)malloc(sizeof(struct pollfd) * (size_t)gem_poll_cap);
    gem_poll_pids = (int *)malloc(sizeof(int) * (size_t)gem_poll_cap);
    if (!gem_poll_fds || !gem_poll_pids) {
        fprintf(stderr, "gem: out of memory (poll set)\n");
        exit(1);
    }
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

    gem_proc_set_state(pid, GEM_PROC_FREE);
    gem_procs_alive--;
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
    /* Main's slot is never handed out again: its arena and globals live
       until the program ends, and with gem_main_pid cleared no later
       process is taken for main (its crash or exit would end the program). */
    if (pid == gem_main_pid) {
        gem_main_pid = -1;
        return;
    }
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
        gem_proc_set_state(gem_current_pid, GEM_PROC_READY);
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

/* The time `ms` milliseconds from now: saturates at INT64_MAX (a deadline
   that never comes) instead of overflowing; a delay <= 0 is due now. */
int64_t gem_deadline_in(int64_t ms) {
    int64_t now = gem_now_ms();
    if (ms <= 0) return now;
    if (ms > INT64_MAX - now) return INT64_MAX;
    return now + ms;
}

/* The deadline of `receive ... after ms`. */
int64_t gem_after_deadline(GemVal ms) {
    if (ms.type != VAL_INT) {
        char buf[128];
        snprintf(buf, sizeof(buf), "receive: after expects an int (milliseconds), got %s", gem_type_str(ms));
        gem_error(buf);
    }
    return gem_deadline_in(ms.ival);
}

/* Yield for selective receive — sets deadline and transitions to WAITING */
void gem_selective_yield(int64_t deadline_ms) {
    if (gem_current_pid < 0 || gem_current_pid >= gem_proc_hwm) {
        gem_error("receive: not inside a spawned process");
        return;
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    proc->deadline_ms = deadline_ms;
    proc->timed_out = 0;
    gem_proc_set_state(gem_current_pid, GEM_PROC_WAITING);
    mco_yield(proc->coro);
    /* Resumed: by a message, or by the deadline (timed_out, which the
       caller reads next). Either way the deadline is spent; left set, a
       later wait of another kind would be woken by it. The receive loop
       sets it again before it yields again. */
    proc->deadline_ms = -1;
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
            if (gem_leaf_site) {
                snprintf(msg, sizeof msg, "stack overflow in native code called from %s",
                         gem_leaf_site->name);
            } else if (gem_call_depth > 0) {
                int top = (gem_call_depth <= GEM_MAX_CALL_DEPTH ? gem_call_depth
                                                                : GEM_MAX_CALL_DEPTH) - 1;
                snprintf(msg, sizeof msg, "stack overflow in native code called from %s",
                         gem_call_stack[top].name);
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
    int pid = gem_proc_alloc_slot();
    if (pid < 0) {
        gem_spawn_overflow_count++;
        gem_error("spawn: process table full");
        return -1;
    }

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
        gem_procs_alive--;
        gem_proc_table[pid].pid = gem_free_head;
        gem_free_head = pid;
        if (gem_free_tail < 0) gem_free_tail = pid;
        char msg[160];
        snprintf(msg, sizeof msg,
                 "spawn: cannot map a stack for a new process (%d processes alive)",
                 gem_procs_alive);
        gem_error(msg);
        return -1;
    }
    gem_proc_table[pid].stack_lo = stack_lo;
    gem_proc_table[pid].stack_overflowed = 0;

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
    gem_proc_table[pid].leaf_site = NULL;
    gem_proc_set_state(pid, GEM_PROC_READY);
    return pid;
}

void gem_send_msg(int pid, GemVal val) {
    if (pid < 0 || pid >= gem_proc_hwm) return;
    GemProcess *proc = &gem_proc_table[pid];
    if (proc->state == GEM_PROC_FREE || proc->state == GEM_PROC_DEAD) return;

    int saved = gem_current_pid;
    gem_current_pid = pid;
    GemVal copied = gem_deep_copy(val, saved);
    gem_mailbox_push(&proc->mailbox, copied);
    gem_current_pid = saved;

    if (proc->state == GEM_PROC_WAITING) gem_proc_set_state(pid, GEM_PROC_READY);
}

GemVal gem_receive_msg(void) {
    if (gem_current_pid < 0 || gem_current_pid >= gem_proc_hwm) {
        gem_error("receive: not inside a spawned process");
        return GEM_NIL;
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];

    while (gem_mailbox_empty(&proc->mailbox)) {
        gem_proc_set_state(gem_current_pid, GEM_PROC_WAITING);
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
    if (slot >= gem_proc_hwm) return -1;
    GemProcess *proc = &gem_proc_table[slot];
    if (proc->gen != pid / GEM_MAX_PROCS) return -1;
    if (proc->state == GEM_PROC_FREE) return -1;
    return slot;
}

void gem_io_pool_yield(void) {
    if (gem_current_pid < 0 || gem_current_pid >= gem_proc_hwm) return;
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    gem_proc_set_state(gem_current_pid, GEM_PROC_IO_WAIT);
    mco_yield(proc->coro);
}

int gem_io_yield(int fd, int for_write) {
    if (gem_current_pid < 0 || gem_current_pid >= gem_proc_hwm) {
        return 0;
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    proc->wait_fd = fd;
    proc->wait_write = for_write;
    proc->wait_fd_closed = 0;
    gem_proc_set_state(gem_current_pid, GEM_PROC_IO_WAIT);
    mco_yield(proc->coro);
    if (proc->wait_fd_closed) {
        proc->wait_fd_closed = 0;
        errno = EBADF;
        return -1;
    }
    return 0;
}

void gem_io_fd_closed(int fd) {
    /* Backwards: waking slot k moves the last entry, already seen, into k. */
    for (int k = gem_fd_waiters.n - 1; k >= 0; k--) {
        int slot = gem_fd_waiters.slots[k];
        GemProcess *proc = &gem_proc_table[slot];
        if (proc->wait_fd == fd) {
            proc->wait_fd_closed = 1;
            gem_proc_set_state(slot, GEM_PROC_READY);
        }
    }
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
    int pid = gem_proc_alloc_slot();
    if (pid < 0) {
        gem_error("spawn: process table full");
        return;
    }

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
    gem_proc_table[pid].leaf_site = NULL;
    gem_proc_table[pid].pinned_boxes = NULL;
    gem_proc_set_state(pid, GEM_PROC_READY);
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

/* Main dies from an exit signal: a linked process exited abnormally
   (`linked`), or another process called kill/exit on it. The program ends
   with main, as for any abnormal exit of main: report it like an uncaught
   error in main, with main's stack (where it was when the signal arrived),
   and exit 1. `from_pid` is the user-visible pid of the sender. */
void gem_report_main_killed(int64_t from_pid, const char *reason, int linked) {
    GemProcess *mp = &gem_proc_table[gem_main_pid];
    if (gem_running_slot != gem_main_pid) {
        /* Main is suspended: its frames are the saved ones. */
        gem_current_pid = gem_main_pid;
        gem_call_stack = mp->call_stack;
        gem_call_depth = mp->call_depth;
        gem_leaf_site = mp->leaf_site;
        gem_leaf_line = mp->leaf_line;
    }
    size_t n = strlen(reason) + 96;
    char *msg = (char *)malloc(n);
    snprintf(msg, n, "main process killed by %sprocess %lld: %s",
             linked ? "linked " : "", (long long)from_pid, reason);
    gem_print_runtime_error(msg);
    exit(1);
}

/* Resume the READY process in slot i until it yields, waits or ends. */
static void gem_run_proc(int i) {
    GemProcess *proc = &gem_proc_table[i];
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
    gem_leaf_site = proc->leaf_site;
    gem_leaf_line = proc->leaf_line;
    gem_cur_globals = proc->globals;
    gem_running_slot = i;
    gem_stack_limit = (uintptr_t)proc->stack_lo + GEM_STACK_RED_ZONE;
    mco_resume(proc->coro);
    gem_stack_limit = 0;
    gem_running_slot = -1;
    proc->call_depth = gem_call_depth;
    proc->leaf_site = gem_leaf_site;
    proc->leaf_line = gem_leaf_line;
    gem_leaf_site = NULL;
    gem_call_depth = saved_global_depth;
    gem_call_stack = saved_global_stack;
    gem_cur_globals = NULL;

    if (mco_status(proc->coro) == MCO_DEAD) {
        mco_destroy(proc->coro);
        proc->coro = NULL;
        const char *reason = proc->exit_reason ? proc->exit_reason : "normal";
        gem_proc_set_state(i, GEM_PROC_DEAD);
        gem_deliver_down_messages(i, reason);
        gem_unregister_name_for_pid(i);
        gem_propagate_exit(i, reason);
    }
}

void gem_run_scheduler(void) {
    for (;;) {
        gem_fire_timers();
        if (gem_io_check_completions()) gem_wake_pool_waiters();
        gem_expire_deadlines();

        /* One pass: run the READY processes in slot order. A process that
           becomes READY during the pass runs in it if its slot is above the
           one running, else in the next pass (as with a scan of the table). */
        int ran = 0;
        for (int i = gem_ready_next(0); i >= 0; i = gem_ready_next(i + 1)) {
            ran = 1;
            gem_run_proc(i);
        }

        if (ran) {
            /* Non-blocking poll: surface fd-readiness even when processes
               stay READY. Without it, a continuously-READY process (e.g. a
               broker reader draining a never-empty TCP buffer) would starve
               every process waiting on another fd (e.g. a writer waiting for
               POLLOUT to a slow client): the blocking poll below would never
               be reached. One syscall per pass while any process waits on
               an fd. */
            if (gem_fd_waiters.n > 0) {
                int nfds = gem_poll_fill();
                if (poll(gem_poll_fds, (nfds_t)nfds, 0) > 0) gem_poll_wake(nfds);
            }
            continue;
        }

        /* Nothing was READY. Earliest wake-up: a process deadline or a timer. */
        int64_t earliest = gem_dl_n > 0 ? gem_dl_heap[0].dl : -1;
        int64_t timer_dl = gem_earliest_timer_deadline();
        if (timer_dl >= 0 && (earliest < 0 || timer_dl < earliest)) earliest = timer_dl;

        /* Block until fd I/O, a thread pool completion or the earliest
           deadline. */
        if (gem_fd_waiters.n > 0 || gem_pool_waiters.n > 0) {
            int nfds = gem_poll_fill();
            /* The thread pool's wake pipe, so poll returns when a worker
               completes a request. */
            int wake_fd = gem_io_wake_fd();
            if (gem_pool_waiters.n > 0 && wake_fd >= 0) {
                gem_poll_fds[nfds].fd = wake_fd;
                gem_poll_fds[nfds].events = POLLIN;
                gem_poll_fds[nfds].revents = 0;
                gem_poll_pids[nfds] = -1;
                nfds++;
            }
            int poll_timeout = -1;
            if (earliest >= 0) {
                int64_t wait_ms = earliest - gem_now_ms();
                if (wait_ms > INT_MAX) wait_ms = INT_MAX;   /* poll again then */
                poll_timeout = (wait_ms > 0) ? (int)wait_ms : 0;
            }
            if (poll(gem_poll_fds, (nfds_t)nfds, poll_timeout) > 0) gem_poll_wake(nfds);
            continue;
        }

        /* Only mailbox waiters (or timers) remain. */
        if (gem_n_msg_wait > 0 || timer_dl >= 0) {
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
            /* Sleep until the earliest deadline; the next pass wakes it. */
            int64_t wait_ms = earliest - gem_now_ms();
            if (wait_ms > 0) {
                struct timespec ts_sleep;
                ts_sleep.tv_sec = wait_ms / 1000;
                ts_sleep.tv_nsec = (wait_ms % 1000) * 1000000;
                nanosleep(&ts_sleep, NULL);
            }
            continue;
        }

        /* No process is alive and no timer is pending. */
        break;
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

int gem_monitor_fn(int64_t target_pid) {
    int caller = gem_current_pid;
    if (caller < 0) {
        gem_error("monitor: not inside a spawned process");
        return 0;
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
        return 1;
    }

    GemProcess *target = &gem_proc_table[target_slot];
    int64_t caller_pid = gem_pid_of_slot(caller);

    /* Deduplicate: return if the caller already monitors the target. The
       same walk unlinks the nodes of monitoring processes that have exited
       (their DOWN would be dropped anyway), so a long-lived target monitored
       by many short-lived processes keeps a list as long as its live
       monitors. A node's pid carries its slot generation, so
       gem_slot_of_pid rejects it once the slot is freed or reused. */
    GemMonitorNode **link = &target->monitors;
    while (*link) {
        GemMonitorNode *node = *link;
        if (node->pid == caller_pid) return 0;  /* already monitoring */
        int watcher = gem_slot_of_pid(node->pid);
        if (watcher < 0 || gem_proc_table[watcher].state == GEM_PROC_DEAD) {
            *link = node->next;
            free(node);
            continue;
        }
        link = &node->next;
    }

    GemMonitorNode *new_node = (GemMonitorNode *)malloc(sizeof(GemMonitorNode));
    new_node->pid = caller_pid;
    new_node->next = target->monitors;
    target->monitors = new_node;
    return 1;
}

/* Removes the caller's monitor of `target_pid`, if any: 1 if there was one.
   A DOWN already delivered stays in the caller's mailbox. */
int gem_demonitor_fn(int64_t target_pid) {
    int caller = gem_current_pid;
    if (caller < 0) return 0;
    int target_slot = gem_slot_of_pid(target_pid);
    if (target_slot < 0 || gem_proc_table[target_slot].state == GEM_PROC_DEAD) return 0;
    int64_t caller_pid = gem_pid_of_slot(caller);
    GemMonitorNode **link = &gem_proc_table[target_slot].monitors;
    while (*link) {
        GemMonitorNode *node = *link;
        if (node->pid == caller_pid) {
            *link = node->next;
            free(node);
            return 1;
        }
        link = &node->next;
    }
    return 0;
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

    /* Target is gone (Erlang semantics): the caller gets an exit signal
       with reason "noproc", as if the target had just exited with it. A
       caller that traps exits receives the EXIT message; any other caller
       dies with that reason (not catchable: it is an exit, not an error). */
    if (target_slot < 0 || gem_proc_table[target_slot].state == GEM_PROC_DEAD) {
        if (self->trap_exit) {
            GemVal msg = gem_table_new();
            gem_table_set(msg, gem_string("tag"), gem_string("EXIT"));
            gem_table_set(msg, gem_string("pid"), gem_int(target_pid));
            gem_table_set(msg, gem_string("reason"), gem_string("noproc"));
            gem_send_msg(caller, msg);
        } else if (caller == gem_main_pid) {
            gem_report_main_killed(target_pid, "noproc", 1);
        } else {
            gem_exit_self("noproc");
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
    int64_t self_kill_from = -1;

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
            if (lpid < 0 || lpid >= gem_proc_hwm) continue;
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
                self_kill_from = gem_pid_of_slot(pid);
            } else if (lpid == gem_main_pid) {
                gem_report_main_killed(gem_pid_of_slot(pid), r, 1);
            } else {
                lproc->exit_reason = strdup(r);
                if (lproc->coro) {
                    mco_destroy(lproc->coro);
                    lproc->coro = NULL;
                }
                gem_deliver_down_messages(lpid, r);
                gem_unregister_name_for_pid(lpid);
                gem_proc_set_state(lpid, GEM_PROC_DEAD);
                if (wl_tail < gem_proc_committed) {
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
        if (self_pid == gem_main_pid)
            gem_report_main_killed(self_kill_from, self_kill_reason, 1);
        gem_exit_self(self_kill_reason);
    }
}

/* ─── Named Process Registry ─── */

void gem_register_name(const char *name, int pid) {
    if (pid < 0 || pid >= gem_proc_hwm) {
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
    return gem_bool(gem_monitor_fn(args[0].ival));
}

GemVal gem_demonitor_builtin(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("demonitor: expected pid (int) argument");
    }
    return gem_bool(gem_demonitor_fn(args[0].ival));
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

    /* As in Erlang, the reason "kill" can't be trapped: the target dies
       with "killed", which is what its monitors and links see (links pass
       "killed" on as an ordinary, trappable reason). */
    int untrappable = strcmp(reason, "kill") == 0;
    if (untrappable) reason = "killed";

    if (proc->trap_exit && !untrappable) {
        GemVal msg = gem_table_new();
        gem_table_set(msg, gem_string("tag"), gem_string("EXIT"));
        gem_table_set(msg, gem_string("pid"), gem_int(gem_pid_of_slot(gem_current_pid)));
        gem_table_set(msg, gem_string("reason"), gem_string(reason));
        gem_send_msg(pid, msg);
        return gem_bool(1);
    }

    /* The running coroutine can't be destroyed from inside itself. */
    if (pid == gem_current_pid) gem_exit_self(reason);
    /* As in Erlang, a "normal" exit signal from another process is ignored
       by a process that doesn't trap exits. */
    if (strcmp(reason, "normal") == 0) return gem_bool(1);
    if (pid == gem_main_pid)
        gem_report_main_killed(gem_pid_of_slot(gem_current_pid), reason, 0);

    proc->exit_reason = strdup(reason);
    if (proc->coro) {
        mco_destroy(proc->coro);
        proc->coro = NULL;
    }
    gem_proc_set_state(pid, GEM_PROC_DEAD);
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
    int64_t deadline = gem_deadline_in(delay_ms);
    do {
        proc->deadline_ms = deadline;
        proc->timed_out = 0;
        gem_proc_set_state(gem_current_pid, GEM_PROC_WAITING);
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
    gem_timers[i].deadline_ms = gem_deadline_in(delay_ms);
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
    for (int i = 0; i < gem_proc_hwm; i++) {
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

    /* monitors — array of the pids of live monitoring processes. The list
       keeps the nodes of monitoring processes that have exited until the
       next monitor() of this process walks it; leave those out. */
    GemVal monitors = gem_table_new();
    GemMonitorNode *mnode = proc->monitors;
    int mi = 0;
    while (mnode) {
        int watcher = gem_slot_of_pid(mnode->pid);
        if (watcher >= 0 && gem_proc_table[watcher].state != GEM_PROC_DEAD)
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
