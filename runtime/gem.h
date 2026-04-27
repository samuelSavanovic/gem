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
#include <gc.h>

/* minicoro forward declaration — full impl is in gem_scheduler.c.
   minicoro.h uses this same typedef, so guard against redefinition. */
#ifndef MINICORO_H
typedef struct mco_coro mco_coro;
#endif

/* ─── GC-aware allocators ─── */

#define ALLOC(type) ((type *)GC_MALLOC(sizeof(type)))
#define ALLOC_N(type, n) ((type *)GC_MALLOC(sizeof(type) * (n)))

/* ─── Tagged value ─── */

typedef enum {
    VAL_NIL, VAL_BOOL, VAL_INT, VAL_FLOAT, VAL_STRING, VAL_FN, VAL_TABLE, VAL_BUFFER, VAL_REF,
} GemType;

typedef struct GemVal GemVal;
typedef GemVal (*GemFnPtr)(void *env, GemVal *args, int argc);

typedef struct GemTable GemTable;

/* String builder — mutable buffer for O(n) string construction */
typedef struct {
    char *data;
    int len;
    int cap;
} GemBuffer;

struct GemVal {
    GemType type;
    union {
        int64_t ival;
        double fval;
        char *sval;
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

/* ─── Constructors ─── */

GemVal gem_int(int64_t v);
GemVal gem_float(double v);
GemVal gem_bool(int v);
GemVal gem_string(const char *s);
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
};

/* ─── Table operations ─── */

GemVal gem_table_new(void);
void gem_table_set(GemVal tbl, GemVal key, GemVal val);
GemVal gem_table_get(GemVal tbl, GemVal key);
void gem_table_grow(GemTable *t);

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

/* ─── Built-in functions (GemFnPtr signature) ─── */

GemVal gem_print(void *_env, GemVal *args, int argc);
GemVal gem_error_fn(void *_env, GemVal *args, int argc);
GemVal gem_len_fn(void *_env, GemVal *args, int argc);
GemVal gem_type_fn(void *_env, GemVal *args, int argc);
GemVal gem_to_string_fn(void *_env, GemVal *args, int argc);
GemVal gem_push_fn(void *_env, GemVal *args, int argc);
GemVal gem_pcall_fn(void *_env, GemVal *args, int argc);
GemVal gem_keys_fn(void *_env, GemVal *args, int argc);
GemVal gem_str_replace_fn(void *_env, GemVal *args, int argc);
GemVal gem_has_key_fn(void *_env, GemVal *args, int argc);
GemVal gem_substr_fn(void *_env, GemVal *args, int argc);
GemVal gem_chr_fn(void *_env, GemVal *args, int argc);
GemVal gem_ord_fn(void *_env, GemVal *args, int argc);
GemVal gem_to_int_fn(void *_env, GemVal *args, int argc);
GemVal gem_to_float_fn(void *_env, GemVal *args, int argc);
GemVal gem_buf_new_fn(void *_env, GemVal *args, int argc);
GemVal gem_buf_push_fn(void *_env, GemVal *args, int argc);
GemVal gem_buf_str_fn(void *_env, GemVal *args, int argc);
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

/* ─── Runtime initialization (stores argc/argv, seeds RNG) ─── */

void gem_init(int argc, char **argv);
extern int gem_stored_argc;
extern char **gem_stored_argv;

/* ─── Helpers used by codegen ─── */

const char *gem_type_str(GemVal v);
GemVal gem_len_val(GemVal v);
void gem_error(const char *msg);
GemVal gem_error_at_fn(const char *file, int line, GemVal *args, int argc);
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

/* Timer entry — global fixed-size array checked by the scheduler */
#define GEM_MAX_TIMERS 256

typedef struct {
    int active;
    int64_t ref;           /* make_ref() value identifying this timer */
    int target_pid;
    GemVal msg;
    int64_t deadline_ms;
} GemTimer;

extern GemTimer gem_timers[GEM_MAX_TIMERS];

/* Process slot */
typedef struct {
    GemProcState state;
    mco_coro *coro;
    GemMailbox mailbox;
    int pid;
    int wait_fd;        /* fd this process is waiting on (when IO_WAIT) */
    int wait_write;     /* 0 = waiting for read, 1 = waiting for write */
    GemMonitorNode *monitors;     /* linked list of pids monitoring this process */
    GemLinkNode *links;           /* linked list of pids linked to this process */
    int trap_exit;                /* if true, exit signals become EXIT messages */
    jmp_buf proc_jmp;             /* process-level error handler (crash isolation) */
    const char *exit_reason;      /* NULL while alive, set on exit/crash */
    int64_t deadline_ms;          /* -1 = no deadline; else absolute time in ms */
    int timed_out;                /* set to 1 by scheduler when deadline expires */
    int reductions;               /* reduction counter for preemptive yielding */
    GemPcallFrame pcall_stack[GEM_MAX_PCALL_DEPTH];
    int pcall_depth;
} GemProcess;

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
GemVal gem_send_after_builtin(void *_env, GemVal *args, int argc);
GemVal gem_cancel_timer_builtin(void *_env, GemVal *args, int argc);
GemVal gem_process_info_builtin(void *_env, GemVal *args, int argc);

#endif /* GEM_H */
