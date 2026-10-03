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

struct GemBuffer;

typedef struct {
    GemArenaBlock *current;
    GemArenaBlock *head;
    GemTable *table_list;
    struct GemBuffer *buffer_list;  /* every GemBuffer allocated here, newest first */
    char *lo;
    char *hi;
    size_t bytes_allocated;         /* monotonic count of bytes handed out */
    struct GemRemEntry *rem;             /* remembered log: tables written per clock epoch (see gem_table_written) */
    size_t rem_len, rem_cap;
    uint64_t pin_seq;               /* stamp for the next pinned box (see GemPinEntry.seq) */
} GemArena;

void gem_arena_init(GemArena *arena);
void *gem_arena_alloc(GemArena *arena, size_t size);
void gem_arena_destroy(GemArena *arena);
GemArenaBlock *gem_arena_append_block(GemArena *arena, GemArenaBlock *after, size_t min_cap);
void gem_arena_free_blocks(GemArenaBlock *block);  /* munmap a block chain */

/* Minimum bytes a loop allocates between two arena resets. The actual
   distance grows with the cost of the previous reset (hysteresis, see
   gem_arena_reset_region in gem_copy.c), so reset work stays amortized
   O(1) per allocated byte however much data is live. */
#ifndef GEM_ARENA_RESET_THRESHOLD
#define GEM_ARENA_RESET_THRESHOLD (1 * 1024 * 1024)
#endif

/* ─── Region marks (per-iteration arena reset) ───
 *
 * Codegen takes a mark where a loop starts (a `while` loop's entry, a TCO
 * function's entry, a mutual-TCO trampoline's entry) and at the loop's
 * back-edge calls gem_arena_reset_region with the values live there. A
 * region reset frees only memory allocated AFTER the mark; everything
 * allocated before it -- which is everything the callers' C frames can
 * hold -- stays where it is. Soundness argument: gem_copy.c. */
typedef struct {
    GemArenaBlock *block;      /* arena->current at mark time */
    size_t used;               /* block->used at mark time */
    GemTable *tables;          /* arena->table_list at mark time */
    struct GemBuffer *buffers; /* arena->buffer_list at mark time */
    uint64_t pin_seq;          /* arena->pin_seq at mark time */
    uint64_t clock;            /* gem_mut_clock value this mark started (see GEM_TABLE_WRITTEN) */
    size_t trigger;            /* reset once bytes_allocated exceeds this */
} GemArenaMark;

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
    /* Only in a module slot of a spawned process that has not touched it
       yet: the value is in the slot's snapshot unit (see "Module globals").
       Reading the slot through gem_global_get copies it in. Never a value
       user code sees. */
    VAL_LAZY,
} GemType;

#define GEM_MAGIC 0x47454D56

typedef struct GemVal GemVal;
typedef GemVal (*GemFnPtr)(void *env, GemVal *args, int argc);

/* String builder — mutable buffer for O(n) string construction */
typedef struct GemBuffer {
    char *data;
    int len;
    int cap;
    struct GemBuffer *arena_next;  /* owning arena's buffer_list (unused for stack buffers) */
} GemBuffer;

/* Allocate a buffer (struct + `cap` data bytes) in the current arena and
   track it in the arena's buffer_list, which region resets scan. */
GemBuffer *gem_buffer_alloc(int cap);

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

/* Frames of the running process. Each process owns its frames
 * (GemProcess.call_stack); the scheduler points this at them on resume. */
extern GemFrame *gem_call_stack;
extern int gem_call_depth;

/* Leaf functions (no calls in the body, see codegen.gem `body_is_leaf`)
 * push no frame. Instead each one has a static GemLeafSite; on entry it sets
 * gem_leaf_site to it, updates gem_leaf_line before each statement, and
 * clears gem_leaf_site on return. A leaf calls nothing, so it is always the
 * innermost frame: error reports and pcall's `stack` show it on top of
 * gem_call_stack. Per-process like the frames (the scheduler saves and
 * restores both on every resume, since a leaf's loop can yield); cleared
 * when an error unwinds (pcall, process death). */
typedef struct {
    const char *name;
    const char *file;
} GemLeafSite;
extern const GemLeafSite *gem_leaf_site;
extern int gem_leaf_line;

/* ─── Mutual-TCO trampoline TLB ───
 *
 * The codegen-emitted body of an SCC member, when it makes an intra-SCC
 * tail call, writes (gem_tail_fn, gem_tail_env, gem_tail_args, gem_tail_argc)
 * and returns; the wrapper around the body loops while gem_tail_fn != NULL,
 * dispatching the next body. The TLB is shared by all processes, so it is
 * only valid between a body's tail-set and the wrapper's read, a window with
 * no yield point: the wrapper consumes it straight away, copying the args
 * into its own frame and clearing gem_tail_fn, before the back-edge reset
 * and yield check (or any yield inside the next body) can let another
 * process write it. gem_tail_fn is therefore NULL whenever a process is
 * suspended, so a body that returns normally always reads NULL.
 *
 * GEM_MAX_TAIL_ARGS sets a hard ceiling on parameter count for SCC merging;
 * if any SCC member has more params, the SCC is left as-is (regular calls).
 */
#define GEM_MAX_TAIL_ARGS 16

extern GemFnPtr gem_tail_fn;
extern void *gem_tail_env;
extern int gem_tail_argc;
extern GemVal gem_tail_args[GEM_MAX_TAIL_ARGS];

/* Soft stack limit of the running process: the lowest address a Gem
 * function's frame may sit at before calls start failing with a catchable
 * "stack overflow" error. The scheduler sets it to the process's stack floor
 * plus GEM_STACK_RED_ZONE on every resume and clears it to 0 (no check) while
 * it runs on the OS stack itself. The red zone below the limit leaves room
 * for the error path and for C runtime code called near the limit; deep
 * recursion inside native code (extern fns) runs into the guard page instead (see
 * gem_scheduler.c, "Process stacks"). */
extern uintptr_t gem_stack_limit;
#if defined(__GNUC__)
__attribute__((noreturn, cold))
#endif
void gem_stack_overflow(const char *name);

static inline void gem_push_frame(const char *name, const char *file, int line) {
#if defined(__GNUC__)
    if (__builtin_expect((uintptr_t)__builtin_frame_address(0) < gem_stack_limit, 0))
        gem_stack_overflow(name);
#else
    { char probe; if ((uintptr_t)&probe < gem_stack_limit) gem_stack_overflow(name); }
#endif
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
/* Float -> text: the shortest digits that read back (strtod) to the same
 * double, with a decimal point on integral values ("2.0", "-0.0") and
 * exponent form outside 1e-4 <= |v| < 1e16 ("1e+16", "1.5e-07"); "inf",
 * "-inf", "nan" for the non-finite values. Writes a NUL-terminated string
 * of at most GEM_FLOAT_BUF - 1 bytes to `out` and returns its length. */
#define GEM_FLOAT_BUF 40
int gem_format_float(double v, char *out);
GemVal gem_bool(int v);
GemVal gem_string(const char *s);
GemVal gem_string_with_len(const char *s, int len);  /* binary-safe; copies len bytes and appends a trailing '\0' */
GemVal gem_make_fn(GemFnPtr f, void *env);

/* ─── extern fn `Bytes` marshaling ───
 *
 * `extern fn foo(b: Bytes) -> Bytes` declares a parameter that is binary-safe
 * across the C boundary. A `Bytes` parameter expands to TWO C parameters:
 *     foo(const uint8_t *b, int64_t b_len)
 * A `Bytes` return value is the small struct below; the runtime copies the
 * data into the calling process's arena via gem_string_with_len().
 *
 * Ownership of the returned `data` mirrors `String`:
 *   - non-blocking `extern fn`     : runtime does NOT free the original.
 *                                    Use static storage or accept the leak.
 *   - blocking `extern blocking fn`: runtime frees with free(); the C side
 *                                    must return a malloc'd pointer (or
 *                                    {NULL, 0} for empty).
 */
typedef struct {
    const uint8_t *data;
    int64_t len;
} GemBytes;

static inline GemBytes gem_bytes(const void *data, int64_t len) {
    GemBytes b; b.data = (const uint8_t *)data; b.len = len; return b;
}
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
    uint8_t immutable;       /* frozen module namespace table (gem_table_freeze); copies keep the flag */
    uint8_t rem_flag;        /* scratch bit for a reset's remembered-log compaction */
    uint8_t index_stale;     /* str_index not built yet (deep copies build it on first string-key use) */
    uint32_t snap_gen;       /* module snapshot unit this table was copied into (0: none); see gem_table_check_mutable */
    uint64_t mut_seq;        /* gem_mut_clock at creation or the last logged write (see gem_table_written) */
};

/* ─── Write barrier for region resets ───
 *
 * gem_mut_clock advances at every gem_arena_mark. Every runtime path that
 * stores a value into a table, or reallocates its key/val arrays, calls
 * gem_table_written(t): on the first such write in the current clock epoch
 * it appends (t, clock) to the owning arena's remembered log. The log is
 * therefore sorted by epoch, and the tables written since mark M -- the only
 * tables older than M that can point into M's reset region -- are found in
 * the log's suffix with epoch >= M.clock. Paths that only permute values
 * already in the table (sort, delete, pop, remove_at) need no barrier. */
extern uint64_t gem_mut_clock;

typedef struct GemRemEntry {
    GemTable *t;
    uint64_t epoch;
} GemRemEntry;

void gem_remember_table(GemTable *t);

static inline void gem_table_written(GemTable *t) {
    if (t->mut_seq != gem_mut_clock) {
        t->mut_seq = gem_mut_clock;
        gem_remember_table(t);
    }
}

/* ─── Table operations ─── */

void gem_table_rebuild_index(GemTable *t);
/* Call before touching t->str_index. */
static inline void gem_table_index(GemTable *t) {
    if (t->index_stale) gem_table_rebuild_index(t);
}

GemVal gem_table_new(void);
void gem_table_set(GemVal tbl, GemVal key, GemVal val);
GemVal gem_table_get(GemVal tbl, GemVal key);
void gem_table_grow(GemTable *t);
/* Freeze a module namespace table: its own entries can no longer be
   changed (the values it holds stay as mutable as they were). The table is
   an ordinary per-process value: spawn, send and resets copy it like any
   other table, and the copy stays frozen. */
void gem_table_freeze(GemVal tbl);
/* After a module reassigns exported binding `field`: store its new value
   in namespace table `ns` (frozen, so user code cannot). No-op unless `ns`
   is a frozen table holding `field`. */
void gem_ns_refresh(GemVal ns, const char *field, GemVal val);
/* Older generated code (bootstrap/stage0.c until it is regenerated) calls
   this; it freezes `tbl` in place and returns it. */
GemVal gem_table_freeze_static(GemVal tbl);

/* Every runtime path that changes a table's entries (store, append, insert,
   delete, pop, sort, remove_at) calls this first. It refuses a frozen
   namespace table, and when the table is part of a module snapshot unit
   this process still shares with future children (snap_gen), it drops that
   unit, so the next spawn snapshots the changed state. */
void gem_table_mutate_slow(GemTable *t);
static inline void gem_table_check_mutable(GemTable *t) {
    if (__builtin_expect(t->immutable | (t->snap_gen != 0), 0)) gem_table_mutate_slow(t);
}

/* ─── Inline cache for .field access ─── */

typedef struct {
    GemTable *table;
    uint32_t shape_id;
    int val_index;
} GemICacheSlot;

/* ─── Comparison / equality ─── */

int gem_val_eq(GemVal a, GemVal b);
/* The identity of a table as an int (0 for any other value), for code that
   must index tables by identity in O(1): tables as table keys are found by
   a linear scan. std/test reaches it through `extern fn` to memoize the
   table pairs of a deep comparison. A table keeps its id only while it is
   not moved: a region reset (or a copy to another process) can give it a
   new one, so keep ids only for tables older than the loop that uses them,
   such as a function's arguments. */
int64_t gem_table_id(GemVal v);
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
void gem_print_runtime_error_as(const char *head, const char *msg);
void gem_report_process_crash(int slot, const char *msg);

/* ─── Built-in functions (GemFnPtr signature) ─── */

GemVal gem_print(void *_env, GemVal *args, int argc);
GemVal gem_error_fn(void *_env, GemVal *args, int argc);
GemVal gem_len_fn(void *_env, GemVal *args, int argc);
GemVal gem_type_fn(void *_env, GemVal *args, int argc);
GemVal gem_to_string_fn(void *_env, GemVal *args, int argc);
GemVal gem_push_fn(void *_env, GemVal *args, int argc);
GemVal gem_pcall_fn(void *_env, GemVal *args, int argc);
GemVal gem_keys_fn(void *_env, GemVal *args, int argc);
GemVal gem_for_len_fn(void *_env, GemVal *args, int argc);
GemVal gem_table_key_at_fn(void *_env, GemVal *args, int argc);
GemVal gem_table_val_at_fn(void *_env, GemVal *args, int argc);
GemVal gem_str_replace_fn(void *_env, GemVal *args, int argc);
GemVal gem_has_key_fn(void *_env, GemVal *args, int argc);
GemVal gem_is_array_n_fn(void *_env, GemVal *args, int argc);
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
GemVal gem_read_stdin_fn(void *_env, GemVal *args, int argc);
GemVal gem_write_stdout_fn(void *_env, GemVal *args, int argc);
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

/* Copy `val` into the current arena. `src_pid` is the process that owns
   `val` (-1 if none): its pinned capture boxes are copied as pinned boxes
   of the current process, so closures that assign a capture still share
   one box that the current process's resets keep alive. */
GemVal gem_deep_copy(GemVal val, int src_pid);
/* Copy `val` into malloc'd memory (from the current process). Pinned boxes
   stay marked as pinned, so a later gem_deep_copy pins them again. */
GemVal gem_deep_copy_malloc(GemVal val);
void gem_deep_free(GemVal val);
/* Free n values built by one gem_deep_copy_malloc-style copy (shared
   structure is freed once). */
void gem_deep_free_n(const GemVal *vals, int n);

/* Region reset (see GemArenaMark): if the arena has passed the mark's
   trigger, copy everything allocated since `mark` that is still reachable
   from `roots`, `pinned_roots` (malloc'd boxes from gem_box_alloc, whose
   contents are copied), the process's module slots, its mailbox,
   pinned boxes older than the mark, and tables/buffers older than the mark,
   into fresh blocks, then free the rest of the post-mark memory. Values
   allocated before the mark are never moved or freed. */
void gem_arena_reset_region(GemArenaMark *mark, GemVal **roots, int n_roots,
                            GemVal **pinned_roots, int n_pinned);

/* ─── Module globals (per-process module state) ───
 *
 * Top-level `let` bindings compile to slots gem_cur_globals[i], read with
 * gem_global_get, written with gem_global_set (gem_global_ref for
 * read-modify-write). Every process owns its slots; the scheduler points
 * gem_cur_globals at the running process's array. Writes stay in the
 * process.
 *
 * Spawn copies module state lazily, per slot (runtime/gem_copy.c, "Module
 * globals"). A child must see its parent's module state as of the spawn.
 * Light values (nil, numbers, bools, refs, fns without env, strings up to
 * GEM_MOD_LIGHT_STRING bytes) are copied into the child at spawn. Every
 * other slot value lives in a snapshot unit: an immutable, refcounted malloc
 * copy of the parent's slots that share structure (one aliasing component).
 * The child's slot holds VAL_LAZY and a reference to the unit; its first
 * read copies the whole unit into the child's arena with one copy map, so
 * aliasing between slots survives. A parent keeps its units for later
 * spawns until it changes them: a slot write (gem_global_set) or a change
 * to a table in the unit (gem_table_check_mutable, via the snap_gen stamp)
 * drops the unit; the next spawn rebuilds just that one. A slot the child
 * never touched is handed on to its own children by reference.
 *
 * Array layout (one malloc): GemVal slots[n], then GemModUnit *out[n]
 * (unit this process shares slot i through, for later spawns), then
 * GemModUnit *in[n] (unit a VAL_LAZY slot i comes from). */
typedef struct GemModUnit GemModUnit;
extern GemVal *gem_cur_globals;
extern int gem_n_globals;
#define GEM_MOD_LIGHT_STRING 64
#define GEM_GLOBALS_OUT(g) ((GemModUnit **)((g) + gem_n_globals))
#define GEM_GLOBALS_IN(g)  (GEM_GLOBALS_OUT(g) + gem_n_globals)
/* Called by generated main() before gem_run_main (gem_globals_init: code
   generated before lazy module copy, see gem_copy.c). */
void gem_globals_init_lazy(int n);
void gem_globals_init(int n);
/* malloc'd array of gem_n_globals slots (all nil) plus the unit arrays. */
GemVal *gem_globals_alloc(void);
/* Give the child (the current process) the module state of `parent` and its
   own copy of the closure value `*fn_val` (env may be NULL). */
void gem_spawn_module_state(GemVal *fn_val, GemVal *child_globals, int parent);
#if defined(__GNUC__)
__attribute__((cold))
#endif
GemVal gem_global_fault(int i);
#if defined(__GNUC__)
__attribute__((cold))
#endif
void gem_global_unshare(int i);
static inline GemVal gem_global_get(int i) {
    /* Test the tag in place, then copy the slot whole: copying first and
       testing the copy makes gcc patch the copy's tag and reload it, a
       store-forwarding stall on every read. */
    if (__builtin_expect(gem_cur_globals[i].type == VAL_LAZY, 0)) return gem_global_fault(i);
    return gem_cur_globals[i];
}
static inline void gem_global_set(int i, GemVal v) {
    if (__builtin_expect(GEM_GLOBALS_OUT(gem_cur_globals)[i] != NULL, 0)) gem_global_unshare(i);
    gem_cur_globals[i] = v;
}
static inline GemVal *gem_global_ref(int i) {
    if (__builtin_expect(gem_cur_globals[i].type == VAL_LAZY, 0)) gem_global_fault(i);
    if (__builtin_expect(GEM_GLOBALS_OUT(gem_cur_globals)[i] != NULL, 0)) gem_global_unshare(i);
    return &gem_cur_globals[i];
}

/* Allocate a new pinned box (sizeof(GemVal)) outside the per-process arena.
   The caller is responsible for initializing *box. The pointer is registered
   in the current process's pin-set; sweep at the next arena reset will free
   it if no live root reaches it. */
GemVal *gem_box_alloc(void);


/* ─── Runtime initialization (stores argc/argv, seeds RNG) ─── */

void gem_init(int argc, char **argv);
extern int gem_stored_argc;
extern char **gem_stored_argv;

/* Canonical (realpath) path of the running executable: /proc/self/exe,
   _NSGetExecutablePath, else argv[0] resolved via PATH. Static buffer,
   never NULL. Used by the compiler/LSP through an `extern fn` to find the
   install root; not a Gem builtin. */
char *gem_exe_path(void);

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
    int64_t pid;                  /* Gem-visible pid of the monitoring process (see gem_pid_of_slot) */
    struct GemMonitorNode *next;
} GemMonitorNode;

/* Link list node — bidirectional */
typedef struct GemLinkNode {
    int pid;                      /* slot of the linked process (links are removed on exit) */
    struct GemLinkNode *next;
} GemLinkNode;

/* Timer entry — stored in a dynamic min-heap keyed by (deadline_ms, seq).
   seq is a monotonic insertion counter that breaks ties so same-deadline
   timers fire in FIFO order rather than heap-position order. */
typedef struct {
    int64_t ref;           /* make_ref() value identifying this timer */
    int64_t target_pid;    /* Gem-visible pid (see gem_pid_of_slot) */
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

/* A request is shared by the worker thread and the requesting process. Each
   side calls gem_io_release once when it is finished with it, and the second
   release frees the request and everything it owns. If the requester is
   killed while waiting, gem_free_proc_slot releases on its behalf. Every
   field below is malloc-owned, never arena memory, so the request outlives
   the requester's arena. */
typedef struct {
    GemIOOp op;
    int requester_pid;
    char *path;            /* strdup'd input (command line for exec) */
    char *content;         /* malloc'd input for write/append */
    size_t content_len;
    char *result_data;     /* malloc'd output from worker (read_file result) */
    size_t result_len;
    char *error_msg;       /* malloc'd error string, or NULL on success */
    int exit_code;         /* output for exec */
    void (*extern_fn)(void *);  /* for GEM_IO_EXTERN: worker calls this */
    void *extern_args;          /* for GEM_IO_EXTERN: malloc'd args struct */
    void (*free_extern)(void *); /* frees extern_args and whatever it still owns */
    int refs;              /* outstanding releases; 2 at submit */
    int done;              /* set to 1 by worker thread (atomic release/acquire) */
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
    char *stack_lo;               /* lowest usable byte of the coroutine stack; the guard sits just below */
    int stack_overflowed;         /* set when the guard page caught an overflow (exit reason "stack overflow") */
    const char *exit_reason;      /* NULL while alive, set on exit/crash */
    int64_t deadline_ms;          /* -1 = no deadline; else absolute time in ms */
    int timed_out;                /* set to 1 by scheduler when deadline expires */
    int reductions;               /* reduction counter for preemptive yielding */
    char *read_buf;               /* reusable tcp_read buffer (arena-allocated) */
    size_t read_buf_cap;          /* capacity of read_buf in bytes */
    GemPcallFrame pcall_stack[GEM_MAX_PCALL_DEPTH];
    int pcall_depth;
    int call_depth;               /* saved gem_call_depth at last yield (restored on resume) */
    const GemLeafSite *leaf_site; /* saved gem_leaf_site / gem_leaf_line at last yield */
    int leaf_line;
    int64_t gen;                  /* slot generation; advanced when the slot is freed */
    int pending_timers;           /* send_after timers that target this process */
    GemFrame call_stack[GEM_MAX_CALL_DEPTH];  /* this process's frames for stack traces */
    GemArena arena;               /* per-process bump allocator */
    /* Pinned-box set: boxes for mutated-captured fn-local vars, allocated via
       gem_box_alloc (plain malloc) so they survive arena reset. Mark-and-sweep
       at every reset; freed en masse on process exit. NULL == empty.
       value: 0 = untouched this cycle, 1 = walked. */
    struct GemPinEntry *pinned_boxes;
    /* This process's module-level bindings (gem_globals_alloc layout:
       slots, then out/in snapshot units). gem_cur_globals points here while
       the process runs. */
    GemVal *globals;
    /* Snapshot units this process still shares with future children (its
       out units, each once), searched by gem_table_mutate_slow. */
    GemModUnit **mod_live;
    int mod_live_n, mod_live_cap;
} GemProcess;

/* Drop a process's module slots and its snapshot unit references. */
void gem_globals_free(GemProcess *proc);

typedef struct GemPinEntry {
    void *key;
    char value;
    uint64_t seq;  /* arena pin_seq at allocation; a box older than a mark outlives that mark's resets */
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
/* 8 MB, the same as the main process and a default OS thread stack, so a
 * function recurses as deep in a spawned process as in main. Stacks are
 * mmap'd (gem_scheduler.c, "Process stacks"): this is reserved address
 * space, and only the pages a process actually touches cost memory (an
 * idle process touches a few KB). 1024 processes reserve 8 GB of virtual
 * address space, which 64-bit Linux and macOS hand out freely. */
#define GEM_CORO_STACK_SIZE (8 * 1024 * 1024)
#endif

#ifndef GEM_STACK_RED_ZONE
/* Bytes at the bottom of every process stack that Gem function calls may
 * not enter: a call whose frame would land there raises "stack overflow"
 * (catchable by pcall) instead. Leaves room for the error path itself and
 * for C runtime code (and libc) called by the deepest Gem frame. */
#define GEM_STACK_RED_ZONE (256 * 1024)
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
/* Runtime code addresses processes by slot index into gem_proc_table. Gem
 * code sees pids of the form slot + generation * GEM_MAX_PROCS; a slot's
 * generation advances when the slot is freed, so a pid never refers to a
 * later process that reuses its slot. */
int64_t gem_pid_of_slot(int slot);
/* Slot for a Gem-visible pid, or -1 when the pid is malformed or its process
 * no longer occupies the slot. */
int gem_slot_of_pid(int64_t pid);
void gem_run_scheduler(void);
/* Ends the running process with `reason`, past any pcall; the scheduler then
 * reports the exit (DOWN, links, name) as for any other process death. The
 * main process exiting with a reason other than "normal" prints the reason as
 * a runtime error and exits the program with status 1. */
__attribute__((noreturn)) void gem_exit_self(const char *reason);
/* Main dies from an exit signal sent by from_pid (a link when `linked`):
   report it like an uncaught error in main and exit 1. */
__attribute__((noreturn)) void gem_report_main_killed(int64_t from_pid, const char *reason, int linked);
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
void gem_monitor_fn(int64_t target_pid);
void gem_deliver_down_messages(int pid, const char *reason);

/* Link API */
void gem_link_fn(int64_t target_pid);
void gem_unlink_fn(int64_t target_pid);
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
/* Takes ownership of `args`: on success it is freed by `free_args` when the
   request is released for the last time; if the queue is full, `free_args`
   runs before NULL is returned. */
GemIORequest *gem_io_submit_extern(void (*fn)(void *), void *args,
                                   void (*free_args)(void *));
void gem_io_release(GemIORequest *req);
/* Read the whole file at `path` into a malloc'd, NUL-terminated buffer
   (*out_len bytes, binary-safe). Regular files are read with one fread sized
   by fstat; anything else (procfs, pipes, devices) is read until EOF.
   Returns NULL with a malloc'd message in *err_msg on failure (cannot open,
   directory, read error). Safe to call from a worker thread. */
char *gem_read_whole_file(const char *path, size_t *out_len, char **err_msg);
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

/* ─── Region marks: hot-path helpers (emitted at every loop entry/back-edge) ─── */

static inline GemArena *gem_arena_of_current(void) {
    return gem_current_pid >= 0 ? &gem_proc_table[gem_current_pid].arena : &gem_global_arena;
}

static inline void gem_arena_mark(GemArenaMark *m) {
    GemArena *a = gem_arena_of_current();
    m->block = a->current;
    m->used = a->current->used;
    m->tables = a->table_list;
    m->buffers = a->buffer_list;
    m->pin_seq = a->pin_seq;
    m->clock = ++gem_mut_clock;
    m->trigger = a->bytes_allocated + GEM_ARENA_RESET_THRESHOLD;
}

static inline int gem_arena_reset_due(const GemArenaMark *m) {
    return gem_current_pid >= 0 &&
           gem_proc_table[gem_current_pid].arena.bytes_allocated > m->trigger;
}

#endif /* GEM_H */
