/*
 * gem.h -- Gem language runtime public API.
 * Compilers target this header; implementation lives in the runtime directory.
 */

#ifndef GEM_H
#define GEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <setjmp.h>

/* minicoro forward declaration — full impl is in gem_scheduler.c.
   minicoro.h uses this same typedef, so guard against redefinition. */
#ifndef MINICORO_H
typedef struct mco_coro mco_coro;
#endif

/* ─── Per-process arena allocator ─── */

typedef struct GemArenaBlock {
    struct GemArenaBlock *next;
    size_t cap;
    size_t used;
    char data[];
} GemArenaBlock;

typedef struct GemTable GemTable;

typedef struct {
    GemArenaBlock *current;
    GemArenaBlock *head;
    GemTable *table_list;
    char *lo;
    char *hi;
    size_t bytes_allocated;
} GemArena;

void gem_arena_init(GemArena *arena);
void *gem_arena_alloc(GemArena *arena, size_t size);
void gem_arena_destroy(GemArena *arena);

/* Threshold (in bytes_allocated) at which TCO emits an arena reset. */
#ifndef GEM_ARENA_RESET_THRESHOLD
#define GEM_ARENA_RESET_THRESHOLD (1 * 1024 * 1024)
#endif

extern GemArena gem_global_arena;
extern int gem_main_pid;

GemArena *gem_current_arena(void);

static inline void *gem_alloc(size_t n) {
    return gem_arena_alloc(gem_current_arena(), n);
}

#define ALLOC(type)      ((type *)gem_arena_alloc(gem_current_arena(), sizeof(type)))
#define ALLOC_N(type, n) ((type *)gem_arena_alloc(gem_current_arena(), sizeof(type) * (n)))

/* ─── Tagged value ─── */

typedef enum {
    VAL_NIL, VAL_BOOL, VAL_INT, VAL_FLOAT, VAL_STRING, VAL_FN, VAL_TABLE, VAL_BUFFER, VAL_REF,
} GemType;

#define GEM_MAGIC 0x47454D56

typedef struct GemVal GemVal;
typedef GemVal (*GemFnPtr)(void *env, GemVal *args, int argc);

/* String builder — mutable buffer for O(n) string construction */
typedef struct {
    char *data;
    int len;
    int cap;
} GemBuffer;

struct GemVal {
    GemType type;
    uint32_t magic;
    union {
        int64_t ival;
        double fval;
        struct { char *sval; int slen; };  /* slen tracks byte length so strings are binary-safe (may contain embedded NULs); sval[slen] == '\0' is maintained as an invariant for C interop. */
        int bval;
        struct { GemFnPtr fn; void *env; };
        GemTable *table;
        GemBuffer *buffer;
        int64_t rval;  /* unique reference id (VAL_REF) */
    };
};

extern GemVal GEM_NIL;

/* ─── Call stack for stack traces ─── */

#define GEM_MAX_CALL_DEPTH 256

typedef struct {
    const char *name;
    const char *file;
    int line;
} GemFrame;

extern GemFrame gem_call_stack[GEM_MAX_CALL_DEPTH];
extern int gem_call_depth;

static inline void gem_push_frame(const char *name, const char *file, int line) {
    if (gem_call_depth < GEM_MAX_CALL_DEPTH) {
        gem_call_stack[gem_call_depth].name = name;
        gem_call_stack[gem_call_depth].file = file;
        gem_call_stack[gem_call_depth].line = line;
    }
    gem_call_depth++;
}

static inline void gem_pop_frame(void) {
    if (gem_call_depth > 0) gem_call_depth--;
}

/* Update the top frame's line — emitted by codegen before each statement so
 * runtime errors point at the offending expression, not the function header. */
static inline void gem_set_line(int line) {
    if (gem_call_depth > 0 && gem_call_depth <= GEM_MAX_CALL_DEPTH) {
        gem_call_stack[gem_call_depth - 1].line = line;
    }
}

/* ─── Constructors ─── */

GemVal gem_int(int64_t v);
GemVal gem_float(double v);
GemVal gem_bool(int v);
GemVal gem_string(const char *s);
GemVal gem_string_with_len(const char *s, int len);  /* binary-safe; copies len bytes and appends a trailing '\0' */
GemVal gem_make_fn(GemFnPtr f, void *env);
GemVal gem_make_ref(void);

/* ─── Table internals (shared across runtime files) ─── */

/* String key -> index mapping for O(1) lookup (stb_ds) */
typedef struct {
    char *key;       /* stb_ds uses this field name */
    int value;       /* index into keys/vals arrays */
} GemStrIndex;

struct GemTable {
    GemVal *keys;
    GemVal *vals;
    int len;
    int cap;
    GemStrIndex *str_index;  /* stb_ds string hash map (NULL until first string key) */
    uint32_t shape_id;       /* incremented on structural mutations (delete, pop, sort, etc.) */
    GemTable *arena_next;    /* linked list in owning arena's table_list */
    uint8_t immutable;       /* set by gem_table_freeze — shared across processes without copy */
};

/* ─── Table operations ─── */

GemVal gem_table_new(void);
void gem_table_set(GemVal tbl, GemVal key, GemVal val);
GemVal gem_table_get(GemVal tbl, GemVal key);
void gem_table_grow(GemTable *t);
void gem_table_freeze(GemVal tbl);

/* ─── Inline cache for .field access ─── */

typedef struct {
    GemTable *table;
    uint32_t shape_id;
    int val_index;
} GemICacheSlot;

/* ─── Comparison / equality ─── */

int gem_val_eq(GemVal a, GemVal b);
int gem_truthy(GemVal v);

/* ─── Arithmetic / operators ─── */

GemVal gem_add(GemVal a, GemVal b);
GemVal gem_sub(GemVal a, GemVal b);
GemVal gem_mul(GemVal a, GemVal b);
GemVal gem_div(GemVal a, GemVal b);
GemVal gem_mod(GemVal a, GemVal b);
GemVal gem_eq(GemVal a, GemVal b);
GemVal gem_neq(GemVal a, GemVal b);
GemVal gem_lt(GemVal a, GemVal b);
GemVal gem_gt(GemVal a, GemVal b);
GemVal gem_le(GemVal a, GemVal b);
GemVal gem_ge(GemVal a, GemVal b);
GemVal gem_neg(GemVal a);
GemVal gem_not(GemVal a);
void gem_string_append(GemVal *accum, GemVal rhs);
GemVal gem_string_finish(GemVal val);

/* ─── Protected call (pcall) ─── */

#define GEM_MAX_PCALL_DEPTH 64

typedef struct {
    jmp_buf buf;
    int saved_call_depth;
    const char *error_msg;
    GemVal stack_snapshot;
} GemPcallFrame;

extern GemPcallFrame gem_pcall_stack[GEM_MAX_PCALL_DEPTH];
extern int gem_pcall_depth;

void gem_raise_error(const char *msg);
void gem_print_stack_trace(void);
void gem_print_runtime_error(const char *msg);

/* ─── Built-in functions (GemFnPtr signature) ─── */

GemVal gem_print(void *_env, GemVal *args, int argc);
GemVal gem_error_fn(void *_env, GemVal *args, int argc);
GemVal gem_len_fn(void *_env, GemVal *args, int argc);
GemVal gem_type_fn(void *_env, GemVal *args, int argc);
GemVal gem_to_string_fn(void *_env, GemVal *args, int argc);
GemVal gem_push_fn(void *_env, GemVal *args, int argc);
GemVal gem_pcall_fn(void *_env, GemVal *args, int argc);
GemVal gem_keys_fn(void *_env, GemVal *args, int argc);
GemVal gem_table_key_at_fn(void *_env, GemVal *args, int argc);
GemVal gem_table_val_at_fn(void *_env, GemVal *args, int argc);
GemVal gem_str_replace_fn(void *_env, GemVal *args, int argc);
GemVal gem_has_key_fn(void *_env, GemVal *args, int argc);
GemVal gem_in_fn(void *_env, GemVal *args, int argc);
GemVal gem_substr_fn(void *_env, GemVal *args, int argc);
GemVal gem_chr_fn(void *_env, GemVal *args, int argc);
GemVal gem_ord_fn(void *_env, GemVal *args, int argc);
GemVal gem_to_int_fn(void *_env, GemVal *args, int argc);
GemVal gem_to_float_fn(void *_env, GemVal *args, int argc);
GemVal gem_interp(int n, GemVal *parts);

/* ─── Value formatting (readable repr for print/to_string/interpolation) ─── */
/* Appends a human-readable repr of `v` to `out`. Tables and arrays render
 * recursively as `{key: val, ...}` / `[v1, v2, ...]`. Strings are quoted only
 * when nested (`as_repr=1`). Cycles render as `<cycle>`; depth is capped to
 * keep output bounded for huge structures. */
void gem_format_value_to_buf(GemVal v, GemBuffer *out, int as_repr);
/* Convenience: returns a fresh string with `gem_format_value_to_buf(v, _, 0)`. */
GemVal gem_format_value_string(GemVal v);
GemVal gem_buf_new_fn(void *_env, GemVal *args, int argc);
GemVal gem_buf_push_fn(void *_env, GemVal *args, int argc);
GemVal gem_build_string_fn(void *_env, GemVal *args, int argc);
GemVal gem_read_file_fn(void *_env, GemVal *args, int argc);
GemVal gem_write_file_fn(void *_env, GemVal *args, int argc);
GemVal gem_delete_fn(void *_env, GemVal *args, int argc);
GemVal gem_pop_fn(void *_env, GemVal *args, int argc);
GemVal gem_values_fn(void *_env, GemVal *args, int argc);
GemVal gem_eprint_fn(void *_env, GemVal *args, int argc);
GemVal gem_exit_process_fn(void *_env, GemVal *args, int argc);
GemVal gem_argv_fn(void *_env, GemVal *args, int argc);
GemVal gem_sort_fn(void *_env, GemVal *args, int argc);
GemVal gem_floor_fn(void *_env, GemVal *args, int argc);
GemVal gem_ceil_fn(void *_env, GemVal *args, int argc);
GemVal gem_round_fn(void *_env, GemVal *args, int argc);
GemVal gem_abs_fn(void *_env, GemVal *args, int argc);
GemVal gem_pow_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqrt_fn(void *_env, GemVal *args, int argc);
GemVal gem_random_fn(void *_env, GemVal *args, int argc);
GemVal gem_append_file_fn(void *_env, GemVal *args, int argc);
GemVal gem_getenv_fn(void *_env, GemVal *args, int argc);
GemVal gem_input_fn(void *_env, GemVal *args, int argc);
GemVal gem_insert_fn(void *_env, GemVal *args, int argc);
GemVal gem_remove_at_fn(void *_env, GemVal *args, int argc);
GemVal gem_band_fn(void *_env, GemVal *args, int argc);
GemVal gem_bor_fn(void *_env, GemVal *args, int argc);
GemVal gem_bxor_fn(void *_env, GemVal *args, int argc);
GemVal gem_bnot_fn(void *_env, GemVal *args, int argc);
GemVal gem_bshl_fn(void *_env, GemVal *args, int argc);
GemVal gem_bshr_fn(void *_env, GemVal *args, int argc);
GemVal gem_file_exists_fn(void *_env, GemVal *args, int argc);
GemVal gem_dirname_fn(void *_env, GemVal *args, int argc);
GemVal gem_path_join_fn(void *_env, GemVal *args, int argc);
GemVal gem_normalize_path_fn(void *_env, GemVal *args, int argc);
GemVal gem_remove_file_fn(void *_env, GemVal *args, int argc);
GemVal gem_mkdir_fn(void *_env, GemVal *args, int argc);
GemVal gem_list_dir_fn(void *_env, GemVal *args, int argc);
GemVal gem_is_dir_fn(void *_env, GemVal *args, int argc);
GemVal gem_exec_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_connect_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_listen_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_accept_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_read_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_write_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_close_fn(void *_env, GemVal *args, int argc);
GemVal gem_epoch_ms_fn(void *_env, GemVal *args, int argc);
GemVal gem_format_time_fn(void *_env, GemVal *args, int argc);
GemVal gem_format_time_local_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_open_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_close_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_exec_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_query_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_last_insert_id_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_changes_fn(void *_env, GemVal *args, int argc);

/* ─── Deep copy (for message passing between arenas) ─── */

GemVal gem_deep_copy(GemVal val);
GemVal gem_deep_copy_malloc(GemVal val);
void gem_deep_free(GemVal val);

/* Reset the current process's arena, preserving the given root values.
   Used by TCO codegen at tail-call boundaries to bound long-lived process
   memory. Roots are deep-copied to malloc scratch, the arena is torn down
   and re-initialized, then the roots are deep-copied back into the fresh
   arena. Mailbox contents are also preserved. No-op if pcall_depth > 0. */
void gem_arena_reset_with_roots(GemVal **roots, int n_roots);

/* Variant that also migrates pinned-box rescue roots: fn-local mutated-
   captured boxes allocated via gem_box_alloc. Each `pinned_roots[i]` is the
   malloc'd box pointer (a `GemVal *`); contents are deep-copied into the
   fresh arena. Pinned boxes also reachable via env fields are walked at most
   once thanks to the per-process pin-set mark. After the walk, any pin-set
   entry that was NOT marked is unreachable and freed. */
void gem_arena_reset_with_roots_pinned(GemVal **roots, int n_roots,
                                       GemVal **pinned_roots, int n_pinned);

/* Allocate a new pinned box (sizeof(GemVal)) outside the per-process arena.
   The caller is responsible for initializing *box. The pointer is registered
   in the current process's pin-set; sweep at the next arena reset will free
   it if no live root reaches it. */
GemVal *gem_box_alloc(void);


/* ─── Runtime initialization (stores argc/argv, seeds RNG) ─── */

void gem_init(int argc, char **argv);
extern int gem_stored_argc;
extern char **gem_stored_argv;

/* ─── Helpers used by codegen ─── */

const char *gem_type_str(GemVal v);
GemVal gem_len_val(GemVal v);
void gem_error(const char *msg);
GemVal gem_error_at_fn(const char *file, int line, GemVal *args, int argc);
void gem_check_callable(GemVal v, const char *file, int line);
GemVal gem_keys(GemVal tbl);

/* ─── Concurrency: mailbox, process table, scheduler ─── */

/* Mailbox: GC-allocated linked list queue of GemVal messages */
typedef struct GemMsgNode {
    GemVal value;
    struct GemMsgNode *next;
} GemMsgNode;

typedef struct {
    GemMsgNode *head;
    GemMsgNode *tail;
} GemMailbox;

/* Process states */
typedef enum {
    GEM_PROC_FREE,      /* slot available */
    GEM_PROC_READY,     /* can be resumed */
    GEM_PROC_WAITING,   /* blocked on receive() */
    GEM_PROC_IO_WAIT,   /* blocked on I/O (fd not ready) */
    GEM_PROC_DEAD,      /* finished execution */
} GemProcState;

/* Monitor list node */
typedef struct GemMonitorNode {
    int pid;                      /* pid of the monitoring process */
    struct GemMonitorNode *next;
} GemMonitorNode;

/* Link list node — bidirectional */
typedef struct GemLinkNode {
    int pid;                      /* pid of the linked process */
    struct GemLinkNode *next;
} GemLinkNode;

/* Timer entry — stored in a dynamic min-heap keyed by (deadline_ms, seq).
   seq is a monotonic insertion counter that breaks ties so same-deadline
   timers fire in FIFO order rather than heap-position order. */
typedef struct {
    int64_t ref;           /* make_ref() value identifying this timer */
    int target_pid;
    GemVal msg;
    int64_t deadline_ms;
    uint64_t seq;
} GemTimer;

extern GemTimer *gem_timers;
extern int gem_timer_count;

/* ─── Thread pool I/O request ─── */

typedef enum {
    GEM_IO_READ_FILE,
    GEM_IO_WRITE_FILE,
    GEM_IO_APPEND_FILE,
    GEM_IO_EXEC,
    GEM_IO_EXTERN,
} GemIOOp;

typedef struct {
    GemIOOp op;
    int requester_pid;
    char *path;            /* strdup'd input (freed after completion) */
    char *content;         /* strdup'd input for write/append */
    size_t content_len;
    char *result_data;     /* malloc'd output from worker (read_file result) */
    size_t result_len;
    char *error_msg;       /* malloc'd error string, or NULL on success */
    int exit_code;         /* output for exec */
    void (*extern_fn)(void *);  /* for GEM_IO_EXTERN: worker calls this */
    void *extern_args;          /* for GEM_IO_EXTERN: opaque args struct */
    volatile int done;     /* set to 1 by worker thread */
} GemIORequest;

/* Process slot */
typedef struct {
    GemProcState state;
    mco_coro *coro;
    GemMailbox mailbox;
    int pid;
    int wait_fd;        /* fd this process is waiting on (when IO_WAIT) */
    int wait_write;     /* 0 = waiting for read, 1 = waiting for write */
    GemIORequest *io_request;     /* non-NULL when waiting on thread pool I/O */
    GemMonitorNode *monitors;     /* linked list of pids monitoring this process */
    GemLinkNode *links;           /* linked list of pids linked to this process */
    int trap_exit;                /* if true, exit signals become EXIT messages */
    jmp_buf proc_jmp;             /* process-level error handler (crash isolation) */
    const char *exit_reason;      /* NULL while alive, set on exit/crash */
    int64_t deadline_ms;          /* -1 = no deadline; else absolute time in ms */
    int timed_out;                /* set to 1 by scheduler when deadline expires */
    int reductions;               /* reduction counter for preemptive yielding */
    char *read_buf;               /* reusable tcp_read buffer (arena-allocated) */
    size_t read_buf_cap;          /* capacity of read_buf in bytes */
    GemPcallFrame pcall_stack[GEM_MAX_PCALL_DEPTH];
    int pcall_depth;
    int entry_call_depth;         /* gem_call_depth at coro entry — used to gate TCO arena reset for non-process-tail functions */
    int call_depth;               /* saved gem_call_depth at last yield (restored on resume) */
    GemArena arena;               /* per-process bump allocator */
    /* Pinned-box set: boxes for mutated-captured fn-local vars, allocated via
       gem_box_alloc (plain malloc) so they survive arena reset. Mark-and-sweep
       at every reset; freed en masse on process exit. NULL == empty.
       value: 0 = untouched this cycle, 1 = walked. */
    struct GemPinEntry *pinned_boxes;
} GemProcess;

typedef struct GemPinEntry {
    void *key;
    char value;
} GemPinEntry;

/* Pin-set ops. gem_pin_mark_walked transitions a pin-set entry from
   "untouched" to "walked" and returns 1 (caller should recurse into *p);
   returns 0 if the pointer is not in the pin-set or is already walked. */
int  gem_pin_mark_walked(GemProcess *proc, void *p);
void gem_pin_sweep(GemProcess *proc);
void gem_pin_free_all(GemProcess *proc);

#ifndef GEM_MAX_PROCS
#define GEM_MAX_PROCS 1024
#endif

#ifndef GEM_CORO_STACK_SIZE
#define GEM_CORO_STACK_SIZE (16 * 1024)
#endif

extern GemProcess gem_proc_table[GEM_MAX_PROCS];
extern int gem_current_pid;
extern int gem_free_head;
extern int gem_free_tail;
extern int gem_proc_hwm;

/* Preemptive yield check — call at loop back-edges */
void gem_yield_check(void);

/* Core concurrency API */
void gem_scheduler_init(void);
int gem_spawn_fn(GemFnPtr fn, void *env);
void gem_send_msg(int pid, GemVal val);
GemVal gem_receive_msg(void);
int gem_self_pid(void);
void gem_run_scheduler(void);
void gem_run_main(GemFnPtr fn, void *env);

/* Selective receive: remove a specific node from the mailbox */
void gem_mailbox_remove(GemMailbox *mb, GemMsgNode *prev, GemMsgNode *node);

/* Selective receive: yield until new messages arrive or deadline expires.
   Sets deadline_ms on the process. Pass -1 for no timeout. */
void gem_selective_yield(int64_t deadline_ms);

/* Get current monotonic time in milliseconds */
int64_t gem_now_ms(void);

/* Non-blocking I/O: yield current coroutine until fd is ready.
   for_write=0 means wait for readable, for_write=1 means wait for writable. */
void gem_io_yield(int fd, int for_write);

/* Yield the current coroutine for a thread pool I/O request.
   Sets state to IO_WAIT; caller must set proc->io_request first. */
void gem_io_pool_yield(void);

/* Process name registry (stb_ds string hash map: name → pid) */
typedef struct {
    char *key;
    int value;
} GemNameEntry;

extern GemNameEntry *gem_name_registry;

/* Named process API */
void gem_register_name(const char *name, int pid);
int gem_whereis_name(const char *name);        /* returns pid or -1 */
void gem_unregister_name_for_pid(int pid);     /* auto-cleanup on death */

/* Monitor API */
void gem_monitor_fn(int target_pid);
void gem_deliver_down_messages(int pid, const char *reason);

/* Link API */
void gem_link_fn(int target_pid);
void gem_unlink_fn(int target_pid);
/* Propagate an exit signal from `pid` (with `reason`) to all linked processes.
   For each link: if trap_exit is set, deliver an EXIT message; otherwise mark
   the linked process DEAD and recursively propagate. Caller must mark `pid`
   DEAD before invoking this to prevent cycles. */
void gem_propagate_exit(int pid, const char *reason);

/* Built-in function wrappers (GemFnPtr signature) */
GemVal gem_spawn_builtin(void *_env, GemVal *args, int argc);
GemVal gem_send_builtin(void *_env, GemVal *args, int argc);
GemVal gem_receive_builtin(void *_env, GemVal *args, int argc);
GemVal gem_self_builtin(void *_env, GemVal *args, int argc);
GemVal gem_monitor_builtin(void *_env, GemVal *args, int argc);
GemVal gem_spawn_monitor_builtin(void *_env, GemVal *args, int argc);
GemVal gem_register_builtin(void *_env, GemVal *args, int argc);
GemVal gem_whereis_builtin(void *_env, GemVal *args, int argc);
GemVal gem_time_ms_builtin(void *_env, GemVal *args, int argc);
GemVal gem_exit_builtin(void *_env, GemVal *args, int argc);
GemVal gem_link_builtin(void *_env, GemVal *args, int argc);
GemVal gem_unlink_builtin(void *_env, GemVal *args, int argc);
GemVal gem_spawn_link_builtin(void *_env, GemVal *args, int argc);
GemVal gem_process_flag_builtin(void *_env, GemVal *args, int argc);
GemVal gem_make_ref_builtin(void *_env, GemVal *args, int argc);
GemVal gem_sleep_builtin(void *_env, GemVal *args, int argc);
GemVal gem_send_after_builtin(void *_env, GemVal *args, int argc);
GemVal gem_cancel_timer_builtin(void *_env, GemVal *args, int argc);
GemVal gem_processes_builtin(void *_env, GemVal *args, int argc);
GemVal gem_process_info_builtin(void *_env, GemVal *args, int argc);

/* ─── Thread pool for async I/O ─── */

void gem_threadpool_init(void);
void gem_threadpool_shutdown(void);
GemIORequest *gem_io_submit(GemIOOp op, const char *path,
                            const char *content, size_t content_len);
GemIORequest *gem_io_submit_extern(void (*fn)(void *), void *args);
void gem_io_free_request(GemIORequest *req);
void gem_io_check_completions(void);
int gem_io_wake_fd(void);

/* ─── Inline-cached field access (hot path inlined, miss in gem_core.c) ─── */

GemVal gem_table_get_ic_miss(GemTable *t, const char *key, GemICacheSlot *cache);

static inline GemVal gem_table_get_cached(GemVal tbl, const char *key, GemICacheSlot *cache) {
    if (tbl.type != VAL_TABLE) {
        char buf[128];
        snprintf(buf, sizeof(buf), "field access on non-table: got %s", gem_type_str(tbl));
        gem_error(buf);
    }
    GemTable *t = tbl.table;
    if (cache->table == t && cache->shape_id == t->shape_id) {
        return t->vals[cache->val_index];
    }
    return gem_table_get_ic_miss(t, key, cache);
}

#endif /* GEM_H */
