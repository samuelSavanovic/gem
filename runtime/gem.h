/* gem.h: the runtime API that generated C compiles against. */

#ifndef GEM_H
#define GEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <setjmp.h>
#include <stdatomic.h>

/* minicoro.h has the same typedef */
#ifndef MINICORO_H
typedef struct mco_coro mco_coro;
#endif


typedef struct GemArenaBlock {
    struct GemArenaBlock *next;
    size_t cap;
    size_t used;
    size_t dirty;   /* data[used..dirty) was handed out before a reset rewound `used`; past it is mmap's zero fill */
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
    size_t bytes_freed;             /* monotonic count of bytes region resets freed */
    struct GemRemEntry *rem;             /* remembered log: tables written per clock epoch (see gem_table_written) */
    size_t rem_len, rem_cap;
    uint64_t pin_seq;               /* stamp for the next pinned box (see GemPinEntry.seq) */
    size_t ret_min;                 /* return-reset hysteresis (gem_arena_reset_return) */
    size_t ret_at;                  /* bytes_allocated after the last return reset */
    uint64_t wait_clock;            /* gem_mut_clock + 1 when the process last blocked in receive or sleep, or on a socket */
} GemArena;

void gem_arena_init(GemArena *arena);
void *gem_arena_alloc(GemArena *arena, size_t size);
void gem_arena_destroy(GemArena *arena);
GemArenaBlock *gem_arena_append_block(GemArena *arena, GemArenaBlock *after, size_t min_cap);
void gem_arena_free_blocks(GemArenaBlock *block);

/* minimum allocation between two resets of a loop; the real gap grows with the last reset's cost
   (gem_arena_reset_region), so reset work stays amortized O(1) per allocated byte */
#ifndef GEM_ARENA_RESET_THRESHOLD
#define GEM_ARENA_RESET_THRESHOLD (1 * 1024 * 1024)
#endif
/* the minimum for a loop whose process has blocked in receive or sleep, or on a socket, since the loop's last reset */
#ifndef GEM_ARENA_IDLE_RESET_THRESHOLD
#define GEM_ARENA_IDLE_RESET_THRESHOLD (64 * 1024)
#endif
/* debug runtime (-DGEM_DEBUG, make debug / make asan): GEM_ASSERT checks invariants, resets poison what
   they give back, freed blocks stay PROT_NONE for a while, and resets run a budgeted heap check
   (gem_debug_check_reset). */
#ifdef GEM_DEBUG
#define GEM_ASSERT(cond, what) \
    do { if (__builtin_expect(!(cond), 0)) gem_assert_fail(#cond, (what), __FILE__, __LINE__); } while (0)
#else
#define GEM_ASSERT(cond, what) ((void)0)
#endif
#if defined(__GNUC__)
__attribute__((noreturn, cold))
#endif
void gem_assert_fail(const char *cond, const char *what, const char *file, int line);
#define GEM_POISON_BYTE 0xDB

/* GEM_ASAN_UNPOISON(p, n): mark n bytes addressable for ASan; no-op without it */
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define GEM_ASAN 1
#endif
#endif
#if defined(__SANITIZE_ADDRESS__) && !defined(GEM_ASAN)
#define GEM_ASAN 1
#endif
#ifdef GEM_ASAN
void __asan_unpoison_memory_region(void const volatile *addr, size_t size);
#define GEM_ASAN_UNPOISON(p, n) __asan_unpoison_memory_region((p), (n))
#else
#define GEM_ASAN_UNPOISON(p, n) ((void)0)
#endif

/* GEM_ARENA_RESET_THRESHOLD, or GEM_RESET_STRESS bytes read at init (=1 resets at almost every back-edge) */
extern size_t gem_reset_threshold;
/* GEM_ARENA_IDLE_RESET_THRESHOLD, or gem_reset_threshold when that is lower */
extern size_t gem_idle_reset_threshold;
/* gem_reset_threshold - gem_idle_reset_threshold */
extern size_t gem_idle_reset_gap;

/* Region marks. A loop (while, TCO fn, mutual-TCO trampoline) takes a mark at entry and calls
   gem_arena_reset_region at its back-edge with the values live there. A reset only frees memory allocated
   after the mark, so nothing the callers' C frames hold ever moves.

   A fn that can recurse (mark_recursive_fns) also records a point at entry, after its params and
   gem_push_frame (TCO fns and trampolines reuse their mark's base). At each return, when gem_ret_reset_due,
   gem_arena_reset_return resets with the return value as the only root, so a recursion frees as it unwinds. */
typedef struct {
    GemArenaBlock *block;      /* with the next four: the arena's current, used, table_list, buffer_list, pin_seq then */
    size_t used;
    GemTable *tables;
    struct GemBuffer *buffers;
    uint64_t pin_seq;
    uint64_t clock;            /* gem_mut_clock epoch that began there (see gem_table_written) */
    GemArena *arena;
    size_t ret_trig;           /* bytes_allocated - bytes_freed then, plus gem_reset_threshold (gem_ret_reset_due) */
} GemArenaPoint;

/* `base` is where the loop started, `young` the end of what earlier resets kept. A reset normally copies
   only what's newer than young, then moves young past the copies, so data the loop holds is copied once.
   When the kept part has doubled since the last full reset, a full reset from base clears its garbage. */
typedef struct {
    GemArenaPoint base;
    GemArenaPoint young;       /* == base until the first reset */
    size_t trigger;            /* reset once bytes_allocated exceeds this */
    size_t old_bytes;          /* bytes kept between base and young */
    size_t old_limit;          /* full reset once old_bytes exceeds this */
    /* the two 32-bit fields keep a mark at 160 bytes; at 168, setting one up at every loop entry takes more instructions */
    uint32_t rem_window;       /* remembered-log entries from base.clock on after its last whole compaction */
    uint32_t idle_extra;       /* see gem_arena_idle_due */
} GemArenaMark;

extern GemArena gem_global_arena;
extern int gem_main_pid;

/* running process's arena, else gem_global_arena; defined after GemProcess */
static inline GemArena *gem_current_arena(void);

static inline void *gem_alloc(size_t n) {
    return gem_arena_alloc(gem_current_arena(), n);
}

#define ALLOC(type)      ((type *)gem_arena_alloc(gem_current_arena(), sizeof(type)))
#define ALLOC_N(type, n) ((type *)gem_arena_alloc(gem_current_arena(), sizeof(type) * (n)))


typedef enum {
    VAL_NIL, VAL_BOOL, VAL_INT, VAL_FLOAT, VAL_STRING, VAL_FN, VAL_TABLE, VAL_BUFFER, VAL_REF,
    VAL_RESOURCE,
    /* only in a spawned process's module slot it hasn't read yet: the value is in a snapshot unit and
       gem_global_get copies it in. User code never sees one. */
    VAL_LAZY,
} GemType;

#define GEM_MAGIC 0x47454D56

typedef struct GemVal GemVal;
typedef GemVal (*GemFnPtr)(void *env, GemVal *args, int argc);

typedef struct GemBuffer {
    char *data;
    int len;
    int cap;
    struct GemBuffer *arena_next;  /* owning arena's buffer_list (unused for stack buffers) */
} GemBuffer;

/* in the current arena, linked on its buffer_list (resets scan it) */
GemBuffer *gem_buffer_alloc(int cap);

/* slen, len and cap are ints, and a buffer keeps cap > len */
#define GEM_MAX_STRLEN (INT_MAX - 1)

void gem_strlen_error(size_t n, const char *who);

static inline int gem_strlen_check(size_t n, const char *who) {
    if (__builtin_expect(n > (size_t)GEM_MAX_STRLEN, 0)) gem_strlen_error(n, who);
    return (int)n;
}

/* grow so len + extra < cap; the old data is copied, not freed */
void gem_buffer_reserve(GemBuffer *b, size_t extra, const char *who);

struct GemVal {
    GemType type;
    uint32_t magic;
    union {
        int64_t ival;
        double fval;
        struct { char *sval; int slen; };  /* byte length; may hold NULs, and sval[slen] is always '\0' */
        int bval;
        struct { GemFnPtr fn; void *env; };
        GemTable *table;
        GemBuffer *buffer;
        int64_t rval;
        /* VAL_RESOURCE: serial (never reused; equality and hashing use it), slot, and kind (outlives the entry) */
        struct { int64_t res_id; int32_t res_slot; int32_t res_kind; };
    };
};

extern GemVal GEM_NIL;

/* string over static bytes (a C literal, NUL at s[n]), no allocation. OK because the runtime never writes a
   string in place and only frees strings it copied; resets skip these and cross-process copies duplicate them. */
#define GEM_STR_LIT(s, n) \
    ((GemVal){ .type = VAL_STRING, .magic = GEM_MAGIC, .sval = (char *)(s), .slen = (n) })


#define GEM_MAX_CALL_DEPTH 256

typedef struct {
    const char *name;
    const char *file;
    int line;
} GemFrame;

/* the running process's GemProcess.call_stack, swapped on resume */
extern GemFrame *gem_call_stack;
extern int gem_call_depth;

/* Leaf fns (fn_is_leaf in codegen.gem) push no frame. Each has a static GemLeafSite: set gem_leaf_site on
   entry, gem_leaf_line before each statement, clear on return. A leaf calls no Gem code, so it's always the
   innermost frame and traces put it on top. Saved per process like the frames (a leaf's loop can yield),
   cleared on unwind. */
typedef struct {
    const char *name;
    const char *file;
} GemLeafSite;
extern const GemLeafSite *gem_leaf_site;
extern int gem_leaf_line;

/* Mutual-TCO trampoline. An SCC member makes an intra-SCC tail call by filling these and returning; the
   wrapper loops while gem_tail_fn is set. They're shared by every process, so the wrapper copies the args
   into its frame and clears gem_tail_fn before its reset and yield check: it's NULL whenever a process is
   suspended. An SCC with a member over GEM_MAX_TAIL_ARGS params isn't merged. */
#define GEM_MAX_TAIL_ARGS 16

extern GemFnPtr gem_tail_fn;
extern void *gem_tail_env;
extern int gem_tail_argc;
extern GemVal gem_tail_args[GEM_MAX_TAIL_ARGS];

/* lowest address a Gem frame may sit at before calls raise a catchable "stack overflow": stack floor +
   GEM_STACK_RED_ZONE, set on resume, or 0 (no check) on the OS stack. Native recursion doesn't see it and
   hits the guard page instead. */
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

static inline void gem_set_line(int line) {
    if (gem_call_depth > 0 && gem_call_depth <= GEM_MAX_CALL_DEPTH) {
        gem_call_stack[gem_call_depth - 1].line = line;
    }
}

/* emitted right before a call into `extern fn` C code: when cc can see the callee it may sink the
   call-stack stores past the call, and the stack overflow handler would report a stale line */
static inline void gem_native_call_fence(void) {
    atomic_signal_fence(memory_order_seq_cst);
}


static inline GemVal gem_int(int64_t v) { return (GemVal){ .type = VAL_INT, .magic = GEM_MAGIC, .ival = v }; }
static inline GemVal gem_float(double v) { return (GemVal){ .type = VAL_FLOAT, .magic = GEM_MAGIC, .fval = v }; }
/* shortest digits that strtod back to v; integral values keep ".0", exponent form outside 1e-4 <= |v| < 1e16,
   "inf"/"-inf"/"nan". Writes at most GEM_FLOAT_BUF - 1 bytes plus a NUL, returns the length. */
#define GEM_FLOAT_BUF 40
int gem_format_float(double v, char *out);
static inline GemVal gem_bool(int v) { return (GemVal){ .type = VAL_BOOL, .magic = GEM_MAGIC, .bval = v }; }
GemVal gem_string(const char *s);
GemVal gem_string_with_len(const char *s, int64_t len);  /* copies len bytes and adds a '\0'; raises past GEM_MAX_STRLEN */
GemVal gem_make_fn(GemFnPtr f, void *env);

/* `Bytes` in extern fns: a param becomes (const uint8_t *b, int64_t b_len), a return is a GemBytes copied
   into the caller's arena. A plain `extern fn`'s returned data is never freed (use static storage); an
   `extern blocking fn`'s is free()d, so return malloc'd data or {NULL, 0}. */
typedef struct {
    const uint8_t *data;
    int64_t len;
} GemBytes;

static inline GemBytes gem_bytes(const void *data, int64_t len) {
    GemBytes b; b.data = (const uint8_t *)data; b.len = len; return b;
}
GemVal gem_make_ref(void);


/* string key -> position in keys/vals; open addressing, malloc'd, keys compared on all slen bytes.
   Entries point at the strings in `keys`, so a reset that moves those re-points them (gem_rekey_index). */
typedef struct {
    const char *key;   /* NULL: empty slot */
    int64_t len;
    uint64_t hash;
    int value;         /* position in keys/vals; -1: deleted slot */
} GemStrSlot;

typedef struct GemKeyIndex GemKeyIndex;

typedef struct {
    int cap;           /* power of two, or 0 for an index that only holds `keyix` (no string keys) */
    int used;          /* live + deleted slots */
    GemKeyIndex *keyix;  /* value key index or NULL (see GemKeyIndex), freed with this index */
    GemStrSlot slots[];
} GemStrIndex;

int gem_str_index_get(const GemStrIndex *ix, const char *key, int64_t len);
void gem_str_index_put(GemStrIndex **ix, const char *key, int64_t len, int pos);
void gem_str_index_del(GemStrIndex *ix, const char *key, int64_t len);
void gem_str_index_free(GemStrIndex **ix);

/* Value key index: ints, floats, bools and refs -> position in keys[]. Slots hold no key, so moving the
   keys array doesn't invalidate it; table, buffer and fn keys go by identity and can move, so they're found
   by a scan. Lives at t->str_index->keyix (a capacity-0 string index without string keys), so dropping the
   string index drops it too. The first value-key lookup that misses the array fast path in a table over
   GEM_TABLE_SCAN_MAX entries builds it; from then on it covers every value key. */

struct GemTable {
    GemVal *keys;
    GemVal *vals;
    int len;
    int cap;
    GemStrIndex *str_index;  /* string key index, or NULL (see GEM_TABLE_SCAN_MAX) */
    uint32_t shape_id;       /* incremented on structural mutations (delete, pop, sort, etc.) */
    int nstr;
    GemTable *arena_next;
    uint8_t immutable;       /* frozen module namespace table (gem_table_freeze); copies keep the flag */
    uint8_t rem_flag : 2;    /* scratch for a reset's remembered-log compaction (0, 1 or 2) */
    uint8_t braces : 1;      /* made by a `{...}` literal (std/json writes it empty as `{}`); copies keep it */
    uint8_t index_stale;     /* str_index and nstr not built yet (gem_table_index builds them) */
    uint8_t is_array;        /* key i at entry i for every i, so t[len] = v appends without a search; other appends
                                and deletes that move entries clear it, sort sets it */
    uint32_t snap_gen;       /* module snapshot unit this table was copied into (0: none); see gem_table_check_mutable */
    uint64_t mut_seq;        /* gem_mut_clock at creation or the last logged write (see gem_table_written) */
};

/* Write barrier. Every runtime path that stores into a table or reallocates its arrays must call
   gem_table_written(t). gem_mut_clock ticks at every arena point and the first write in an epoch logs
   (t, epoch), so the log is sorted and the older tables that can point into a mark's region are its suffix
   from the mark's clock on. Just permuting existing values (sort, delete, pop, remove_at) needs no barrier. */
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

/* up to this many entries there's no string index and keys[] is scanned. An append past it (with a string
   key present) builds one, kept through deletes; sort, insert, remove_at and copies recompute it. */
#ifndef GEM_TABLE_SCAN_MAX
#define GEM_TABLE_SCAN_MAX 8
#endif

void gem_table_rebuild_index(GemTable *t);
/* position of a non-string key, or -1 */
int gem_table_key_pos(GemTable *t, GemVal key);
void gem_key_index_put(GemTable *t, GemVal key, int pos);
void gem_key_index_del(GemTable *t, GemVal key);

/* call before touching t->str_index or t->nstr */
static inline void gem_table_index(GemTable *t) {
    if (t->index_stale) gem_table_rebuild_index(t);
}
int gem_table_str_pos(GemTable *t, const char *key, int64_t len);

GemVal gem_table_new(void);
void gem_table_set(GemVal tbl, GemVal key, GemVal val);
GemVal gem_table_get(GemVal tbl, GemVal key);
void gem_table_grow(GemTable *t);
/* module namespace tables: their own entries become read-only (not the values); copies stay frozen */
void gem_table_freeze(GemVal tbl);
/* a module reassigned exported `field`: update it in ns (no-op unless ns is frozen and has field) */
void gem_ns_refresh(GemVal ns, const char *field, GemVal val);
/* gem_table_freeze that returns tbl; codegen doesn't emit it */
GemVal gem_table_freeze_static(GemVal tbl);

/* Every runtime path that changes a table's entries must call gem_table_check_mutable first. The slow
   path refuses frozen namespaces, and drops a module snapshot unit the table is in that's still shared
   with future children (snap_gen), so the next spawn snapshots the new state. */
void gem_table_mutate_slow(GemTable *t);
static inline void gem_table_check_mutable(GemTable *t) {
    if (__builtin_expect(t->immutable | (t->snap_gen != 0), 0)) gem_table_mutate_slow(t);
}


typedef struct {
    GemTable *table;
    uint32_t shape_id;
    int val_index;
} GemICacheSlot;

static inline int gem_val_eq(GemVal a, GemVal b) {
    if (a.type != b.type) return 0;
    switch (a.type) {
        case VAL_NIL: return 1;
        case VAL_BOOL: return a.bval == b.bval;
        case VAL_INT: return a.ival == b.ival;
        case VAL_FLOAT: return a.fval == b.fval;
        case VAL_STRING: return a.slen == b.slen && memcmp(a.sval, b.sval, (size_t)a.slen) == 0;
        case VAL_REF: return a.rval == b.rval;
        case VAL_RESOURCE: return a.res_id == b.res_id;
        case VAL_TABLE: return a.table == b.table;
        case VAL_BUFFER: return a.buffer == b.buffer;
        case VAL_FN: return a.fn == b.fn && a.env == b.env;
        default: return 0;
    }
}
/* a table's identity as an int (0 otherwise), for O(1) identity maps since table keys are scanned (std/test).
   A reset or copy can move a table and change its id, so only hold ids of tables older than the loop using them. */
int64_t gem_table_id(GemVal v);
/* `{...}` literal tables, so std/json (extern fn) can write an empty one as {} */
GemVal gem_table_new_braces(void);
int gem_table_braces(GemVal v);
/* binary-safe strspn, for std/http and std/request (extern fn) to validate heads, header names and cookies
   in one pass; the last few byte sets are cached */
int64_t gem_bytes_span(const uint8_t *s, int64_t n, const uint8_t *accept, int64_t accept_n);
static inline int gem_truthy(GemVal v) {
    if (v.type == VAL_NIL) return 0;
    if (v.type == VAL_BOOL) return v.bval;
    return 1;
}

/* int (and float +) inline, the rest and type errors in gem_<op>_slow. Int + - * wrap, via uint64_t (no UB) */

GemVal gem_add_slow(GemVal a, GemVal b);
GemVal gem_sub_slow(GemVal a, GemVal b);
GemVal gem_mul_slow(GemVal a, GemVal b);
GemVal gem_lt_slow(GemVal a, GemVal b);

static inline GemVal gem_add(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int((int64_t)((uint64_t)a.ival + (uint64_t)b.ival));
    if (a.type == VAL_FLOAT && b.type == VAL_FLOAT) return gem_float(a.fval + b.fval);
    return gem_add_slow(a, b);
}

static inline GemVal gem_sub(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int((int64_t)((uint64_t)a.ival - (uint64_t)b.ival));
    return gem_sub_slow(a, b);
}

static inline GemVal gem_mul(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int((int64_t)((uint64_t)a.ival * (uint64_t)b.ival));
    return gem_mul_slow(a, b);
}

GemVal gem_div(GemVal a, GemVal b);
GemVal gem_mod(GemVal a, GemVal b);

static inline GemVal gem_eq(GemVal a, GemVal b) { return gem_bool(gem_val_eq(a, b)); }
static inline GemVal gem_neq(GemVal a, GemVal b) { return gem_bool(!gem_val_eq(a, b)); }

static inline GemVal gem_lt(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_bool(a.ival < b.ival);
    return gem_lt_slow(a, b);
}
static inline GemVal gem_gt(GemVal a, GemVal b) { return gem_lt(b, a); }
/* not (b < a) / not (a < b), so with a NaN operand both are true */
static inline GemVal gem_le(GemVal a, GemVal b) { return gem_bool(!gem_lt(b, a).bval); }
static inline GemVal gem_ge(GemVal a, GemVal b) { return gem_bool(!gem_lt(a, b).bval); }

GemVal gem_neg(GemVal a);
static inline GemVal gem_not(GemVal a) { return gem_bool(!gem_truthy(a)); }
/* `s = s + rhs` in a loop that only appends to s (compile_while): the first append copies s into a buffer
   and sets *built (0 before the loop), later ones append in place, and the loop ends with
   `if (built) s = gem_string_finish(s)`. Any other s gets plain `+`. */
void gem_string_append_to(GemVal *accum, GemVal rhs, int *built);
GemVal gem_string_finish(GemVal val);


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


GemVal gem_print(void *_env, GemVal *args, int argc);
GemVal gem_error_fn(void *_env, GemVal *args, int argc);
GemVal gem_len_fn(void *_env, GemVal *args, int argc);
GemVal gem_type_fn(void *_env, GemVal *args, int argc);
GemVal gem_to_string_fn(void *_env, GemVal *args, int argc);
GemVal gem_check_opts_fn(void *_env, GemVal *args, int argc);
/* raise `<who>: unknown option "<k>"` (naming a close one if any) for a key of bag not in names (NUL-separated) */
void gem_check_opt_keys(GemVal bag, const char *who, const char *names, int nlen);
GemVal gem_opts_rest_fn(void *_env, GemVal *args, int argc);
GemVal gem_push_fn(void *_env, GemVal *args, int argc);
GemVal gem_pcall_fn(void *_env, GemVal *args, int argc);
GemVal gem_keys_fn(void *_env, GemVal *args, int argc);
GemVal gem_for_len_fn(void *_env, GemVal *args, int argc);
GemVal gem_for_items_fn(void *_env, GemVal *args, int argc);
GemVal gem_table_key_at_fn(void *_env, GemVal *args, int argc);
GemVal gem_table_val_at_fn(void *_env, GemVal *args, int argc);
GemVal gem_str_replace_fn(void *_env, GemVal *args, int argc);
GemVal gem_has_key_fn(void *_env, GemVal *args, int argc);
GemVal gem_is_array_n_fn(void *_env, GemVal *args, int argc);
GemVal gem_in_fn(void *_env, GemVal *args, int argc);
GemVal gem_substr_fn(void *_env, GemVal *args, int argc);
GemVal gem_match_fail_fn(void *_env, GemVal *args, int argc);
GemVal gem_find_fn(void *_env, GemVal *args, int argc);
GemVal gem_chr_fn(void *_env, GemVal *args, int argc);
GemVal gem_ord_fn(void *_env, GemVal *args, int argc);
GemVal gem_to_int_fn(void *_env, GemVal *args, int argc);
GemVal gem_to_float_fn(void *_env, GemVal *args, int argc);
GemVal gem_interp(int n, GemVal *parts);

/* text of v for print/to_string/interpolation; strings quoted only nested (as_repr), cycles as <cycle> */
void gem_format_value_to_buf(GemVal v, GemBuffer *out, int as_repr);
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
GemVal gem_float_to_int(double d, const char *who);
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
GemVal gem_exec_capture_fn(void *_env, GemVal *args, int argc);
GemVal gem_rename_fn(void *_env, GemVal *args, int argc);
GemVal gem_stat_fn(void *_env, GemVal *args, int argc);
GemVal gem_setenv_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_connect_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_listen_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_accept_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_read_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_write_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_close_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_peer_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_local_port_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_fd_fn(void *_env, GemVal *args, int argc);
GemVal gem_tcp_from_fd_fn(void *_env, GemVal *args, int argc);
GemVal gem_claim_fn(void *_env, GemVal *args, int argc);
GemVal gem_tls_connect_fn(void *_env, GemVal *args, int argc);
GemVal gem_epoch_ms_fn(void *_env, GemVal *args, int argc);
GemVal gem_format_time_fn(void *_env, GemVal *args, int argc);
GemVal gem_format_time_local_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_open_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_close_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_exec_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_query_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_last_insert_id_fn(void *_env, GemVal *args, int argc);
GemVal gem_sqlite_changes_fn(void *_env, GemVal *args, int argc);


/* into the current arena; src_pid's pinned boxes (-1: none) stay pinned here, so closures still share them */
GemVal gem_deep_copy(GemVal val, int src_pid);
/* into malloc'd memory; pinned boxes stay marked, so gem_deep_copy re-pins them */
GemVal gem_deep_copy_malloc(GemVal val);
void gem_deep_free(GemVal val);
void gem_deep_free_n(const GemVal *vals, int n);

/* Past the mark's trigger: copy what's newer than the young point (base, for a full reset) and reachable
   from roots, pinned_roots (gem_box_alloc boxes, contents copied), module slots, the mailbox, older pinned
   boxes and older tables/buffers, and free the rest. Nothing older than the point moves; the copies then
   count as old (young moves past them). */
void gem_arena_reset_region(GemArenaMark *mark, GemVal **roots, int n_roots,
                            GemVal **pinned_roots, int n_pinned);
/* Called when gem_ret_reset_due. Does nothing for a process body's return (exit frees the arena anyway;
   own_frames is the fn's own frame count, 1, or 0 for a trampoline) or while the call's unfreed memory plus
   allocation since the last return reset is under ret_min. Otherwise a full reset from pt with *ret as the
   only root; ret_min becomes 2 * copied + scanned / 2 (4 * copied when it kept at least half its region). */
void gem_arena_reset_return(const GemArenaPoint *pt, GemVal *ret, int own_frames);

/* program-wide reset totals; `full` includes return resets, `scanned` only counts under GEM_DIAG; seconds */
typedef struct {
    uint64_t resets, full, ret, ret_copied, copied, scanned, freed;
    double t_total, t_max;
    uint64_t over_1ms, over_10ms, over_100ms;
} GemResetStats;
void gem_reset_stats_get(GemResetStats *out);

size_t gem_arena_mapped_bytes(const GemArena *arena);

/* Module globals: top-level lets are per-process slots. gem_cur_globals points at the running process's
   array. Generated code reads with gem_global_get and writes with gem_global_set (gem_global_ref for
   read-modify-write), never indexing gem_cur_globals: reads must fault in lazy slots, writes must drop
   the slot's snapshot unit.

   A child sees its parent's module state as of the spawn. Light values (nil, numbers, bools, refs, env-less
   fns, strings up to GEM_MOD_LIGHT_STRING) are copied at spawn; the rest go in refcounted snapshot units that
   the child's VAL_LAZY slots fault in on first read (gem_spawn_module_state, gem_global_fault).

   Layout, one malloc: GemVal slots[n], GemModUnit *out[n] (unit slot i is shared through),
   GemModUnit *in[n] (unit a VAL_LAZY slot comes from). */
typedef struct GemModUnit GemModUnit;
extern GemVal *gem_cur_globals;
extern int gem_n_globals;
#define GEM_MOD_LIGHT_STRING 64
#define GEM_GLOBALS_OUT(g) ((GemModUnit **)((g) + gem_n_globals))
#define GEM_GLOBALS_IN(g)  (GEM_GLOBALS_OUT(g) + gem_n_globals)
/* main() calls the lazy one before gem_run_main; gem_globals_init is eager (spawn copies every unit, keeps none) */
void gem_globals_init_lazy(int n);
void gem_globals_init(int n);
GemVal *gem_globals_alloc(void);
/* give the child (current process) the parent's module state and its own copy of *fn_val */
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
    GEM_ASSERT(i >= 0 && i < gem_n_globals && gem_cur_globals, "a module slot read outside the slot table");
    /* test the tag in place: testing a copy makes gcc patch its tag and reload it (store-forwarding stall) */
    if (__builtin_expect(gem_cur_globals[i].type == VAL_LAZY, 0)) return gem_global_fault(i);
    return gem_cur_globals[i];
}
static inline void gem_global_set(int i, GemVal v) {
    GEM_ASSERT(i >= 0 && i < gem_n_globals && gem_cur_globals, "a module slot write outside the slot table");
    if (__builtin_expect(GEM_GLOBALS_OUT(gem_cur_globals)[i] != NULL, 0)) gem_global_unshare(i);
    gem_cur_globals[i] = v;
}
static inline GemVal *gem_global_ref(int i) {
    if (__builtin_expect(gem_cur_globals[i].type == VAL_LAZY, 0)) gem_global_fault(i);
    if (__builtin_expect(GEM_GLOBALS_OUT(gem_cur_globals)[i] != NULL, 0)) gem_global_unshare(i);
    return &gem_cur_globals[i];
}

/* malloc'd box outside the arena, so resets never move it. It joins the process's pin-set, and a reset's
   sweep frees it once nothing reaches it. Caller initializes it. */
GemVal *gem_box_alloc(void);

void gem_init(int argc, char **argv);
extern int gem_stored_argc;
extern char **gem_stored_argv;

/* realpath of the running binary (/proc/self/exe, _NSGetExecutablePath, else argv[0] via PATH) in a static
   buffer, never NULL; the compiler finds its install root with it */
char *gem_exe_path(void);

/* for the compiler's work dirs and binary cache, through extern fn. gem_temp_dir*: a new dir under $TMPDIR
   (else /tmp) or parent, in a static buffer, or NULL. gem_rmdir: 1 on success. */
char *gem_temp_dir(void);
char *gem_temp_dir_in(const char *parent);
int64_t gem_rmdir(const char *path);
char *gem_hash_hex(const uint8_t *data, int64_t len);
char *gem_file_hash(const char *path);
int64_t gem_mkdirs(const char *path);
int64_t gem_link(const char *from, const char *to);
int64_t gem_rename(const char *from, const char *to);
int64_t gem_file_age(const char *path);
/* run mode: exec path with args (each NUL-terminated), passing on every fd that isn't close-on-exec. With
   replace it takes over this process (-1 only on failure), else it waits for a child and returns its status
   (128 + signal, -1 if it couldn't start). */
int64_t gem_run_program(const char *path, const uint8_t *args, int64_t len, int64_t replace);


const char *gem_type_str(GemVal v);
GemVal gem_len_val(GemVal v);
void gem_error(const char *msg);
GemVal gem_error_at_fn(const char *file, int line, GemVal *args, int argc);
void gem_check_callable(GemVal v, const char *file, int line);
void gem_check_callable_field(GemVal v, const char *field, const char *file, int line);
GemVal gem_keys(GemVal tbl);

/* DIRECT_BUILTINS fast paths: common arg types inline, anything else goes to the full builtin, which raises */
static inline GemVal gem_len_1(GemVal v) {
    if (v.type == VAL_STRING) return gem_int((int64_t)v.slen);
    if (v.type == VAL_TABLE) return gem_int((int64_t)v.table->len);
    return gem_len_val(v);
}
static inline GemVal gem_type_1(GemVal v) {
    switch (v.type) {
        case VAL_NIL: return GEM_STR_LIT("nil", 3);
        case VAL_BOOL: return GEM_STR_LIT("bool", 4);
        case VAL_INT: return GEM_STR_LIT("int", 3);
        case VAL_FLOAT: return GEM_STR_LIT("float", 5);
        case VAL_STRING: return GEM_STR_LIT("string", 6);
        case VAL_FN: return GEM_STR_LIT("fn", 2);
        case VAL_TABLE: return GEM_STR_LIT("table", 5);
        case VAL_BUFFER: return GEM_STR_LIT("buffer", 6);
        default: return gem_type_fn(NULL, &v, 1);
    }
}
static inline GemVal gem_ord_1(GemVal s) {
    if (s.type == VAL_STRING && s.slen > 0) return gem_int((int64_t)(unsigned char)s.sval[0]);
    return gem_ord_fn(NULL, &s, 1);
}
static inline GemVal gem_ord_2(GemVal s, GemVal i) {
    if (s.type == VAL_STRING && i.type == VAL_INT && (uint64_t)i.ival < (uint64_t)s.slen)
        return gem_int((int64_t)(unsigned char)s.sval[i.ival]);
    GemVal args[2] = {s, i};
    return gem_ord_fn(NULL, args, 2);
}
static inline GemVal gem_for_len_1(GemVal v) {
    if (v.type == VAL_TABLE) return gem_int((int64_t)v.table->len);
    return gem_for_len_fn(NULL, &v, 1);
}
static inline GemVal gem_for_items_1(GemVal v) {
    if (v.type == VAL_TABLE && v.table->is_array) return v;
    return gem_for_items_fn(NULL, &v, 1);
}
static inline GemVal gem_table_key_at_2(GemVal t, GemVal i) {
    if (t.type == VAL_TABLE && i.type == VAL_INT && (uint64_t)i.ival < (uint64_t)t.table->len)
        return t.table->keys[i.ival];
    GemVal args[2] = {t, i};
    return gem_table_key_at_fn(NULL, args, 2);
}
static inline GemVal gem_table_val_at_2(GemVal t, GemVal i) {
    if (t.type == VAL_TABLE && i.type == VAL_INT && (uint64_t)i.ival < (uint64_t)t.table->len)
        return t.table->vals[i.ival];
    GemVal args[2] = {t, i};
    return gem_table_val_at_fn(NULL, args, 2);
}


typedef struct GemMsgNode {
    GemVal value;
    struct GemMsgNode *next;
} GemMsgNode;

typedef struct {
    GemMsgNode *head;
    GemMsgNode *tail;
} GemMailbox;

typedef enum {
    GEM_PROC_FREE,
    GEM_PROC_READY,
    GEM_PROC_WAITING,
    GEM_PROC_IO_WAIT,
    GEM_PROC_DEAD,
} GemProcState;

enum { GEM_WAIT_NONE, GEM_WAIT_FD, GEM_WAIT_POOL };

typedef struct GemMonitorNode {
    int64_t pid;                  /* Gem-visible pid of the monitoring process (see gem_pid_of_slot) */
    struct GemMonitorNode *next;
} GemMonitorNode;

typedef struct GemLinkNode {
    int pid;                      /* slot of the linked process (links are removed on exit) */
    struct GemLinkNode *next;
} GemLinkNode;

/* min-heap on (deadline_ms, seq); seq breaks ties so equal deadlines fire FIFO */
typedef struct {
    int64_t ref;
    int64_t target_pid;    /* Gem-visible pid (see gem_pid_of_slot) */
    GemVal msg;
    int64_t deadline_ms;
    uint64_t seq;
} GemTimer;

extern GemTimer *gem_timers;
extern int gem_timer_count;


typedef enum {
    GEM_IO_READ_FILE,
    GEM_IO_WRITE_FILE,
    GEM_IO_APPEND_FILE,
    GEM_IO_EXEC,
    GEM_IO_EXTERN,
    GEM_IO_RESOLVE,      /* getaddrinfo of `path`, on the resolver lane */
    GEM_IO_READ_RANGE,
    GEM_IO_EXEC_CAPTURE, /* exec_capture: `content` is the child's stdin */
} GemIOOp;

/* Shared by the worker and the requester: each calls gem_io_release once, and the last release frees it and
   all it owns. A name lookup has more holders (each waiting process and the in-flight lookup table), one ref
   each. If the requester is killed, gem_free_proc_slot releases for it. Every field is malloc'd, never arena
   memory, so the request can outlive the requester. */
typedef struct {
    GemIOOp op;
    int requester_pid;
    char *path;            /* strdup'd input (command line for exec) */
    char *content;         /* malloc'd input for write/append */
    size_t content_len;
    char *result_data;     /* malloc'd output from worker (read_file result) */
    size_t result_len;
    char *error_msg;       /* malloc'd error string, or NULL on success */
    int exit_code;
    int64_t offset, count;
    char *err_data;        /* malloc'd output: exec_capture's stderr */
    size_t err_len;
    uint32_t addr4;        /* output for resolve: the IPv4 address, network byte order */
    void (*extern_fn)(void *);  /* for GEM_IO_EXTERN: worker calls this */
    void *extern_args;          /* for GEM_IO_EXTERN: malloc'd args struct */
    void (*free_extern)(void *); /* frees extern_args and whatever it still owns */
    int refs;              /* outstanding releases; 2 at submit */
    int done;              /* set to 1 by worker thread (atomic release/acquire) */
    /* an `extern blocking fn`'s Socket params, busy until it returns (scheduler thread only, freed with it) */
    struct GemResBusy *busy;
    int nbusy;
    int runtime;           /* GEM_IO_EXTERN work of the runtime's own (sqlite), not an
                              `extern blocking fn`: it uses none of the caller's sockets */
} GemIORequest;

typedef struct {
    GemProcState state;           /* change only through the scheduler (gem_proc_set_state) */
    mco_coro *coro;
    GemMailbox mailbox;
    int pid;
    int wait_fd;        /* fd this process is waiting on (when IO_WAIT) */
    int wait_write;     /* 0 = waiting for read, 1 = waiting for write */
    GemVal wait_res;    /* the socket whose fd that is (closing it wakes the process) */
    /* wait_kind and dl_idx are 0 when in none of the scheduler's lists; wait_idx only counts while wait_kind is set */
    int wait_kind;      /* IO_WAIT only: GEM_WAIT_FD or GEM_WAIT_POOL */
    int wait_idx;       /* index in the fd or pool waiter list */
    int dl_idx;         /* 1 + index in the deadline heap, 0 = not in it */
    GemIORequest *io_request;
    GemMonitorNode *monitors;
    GemLinkNode *links;
    int trap_exit;
    jmp_buf proc_jmp;             /* process-level error handler (crash isolation) */
    char *stack_lo;               /* lowest usable byte of the coroutine stack; the guard sits just below */
    int stack_overflowed;         /* set when the guard page caught an overflow (exit reason "stack overflow") */
    int crashed;                  /* died of an uncaught error (whatever exit_reason says) */
    const char *exit_reason;      /* NULL while alive, set on exit/crash */
    int64_t deadline_ms;          /* -1 = no deadline; else absolute time in ms */
    int timed_out;
    int reductions;
    GemPcallFrame pcall_stack[GEM_MAX_PCALL_DEPTH];
    int pcall_depth;
    int call_depth;               /* saved gem_call_depth at last yield (restored on resume) */
    const GemLeafSite *leaf_site; /* saved gem_leaf_site / gem_leaf_line at last yield */
    int leaf_line;
    int64_t gen;                  /* slot generation; advanced when the slot is freed */
    int pending_timers;           /* send_after timers that target this process */
    GemFrame call_stack[GEM_MAX_CALL_DEPTH];
    GemArena arena;
    /* pin-set: gem_box_alloc boxes, swept at every reset and freed at exit (value: 0 untouched, 1 walked) */
    struct GemPinEntry *pinned_boxes;
    /* module slots (gem_globals_alloc layout); gem_cur_globals while running */
    GemVal *globals;
    /* snapshot units still shared with future children, once each (gem_table_mutate_slow searches them) */
    GemModUnit **mod_live;
    int mod_live_n, mod_live_cap;
    /* resource lists: entries owned, entries last used without owning (-1: none), and the owned count */
    int32_t res_owned, res_used;
    int res_count;
} GemProcess;

void gem_globals_free(GemProcess *proc);

typedef struct GemPinEntry {
    void *key;
    char value;
    uint64_t seq;  /* arena pin_seq at allocation; a box older than a mark outlives that mark's resets */
} GemPinEntry;

/* 1 if p is in the pin-set and untouched (now marked walked: recurse into it), else 0 */
int  gem_pin_mark_walked(GemProcess *proc, void *p);
void gem_pin_sweep(GemProcess *proc);
void gem_pin_free_all(GemProcess *proc);

#ifndef GEM_MAX_PROCS
/* max live processes, main included, and the pid modulus (pid = slot + generation * GEM_MAX_PROCS).
   The table reserves address space for this many slots and touches only those below the high-water mark. */
#define GEM_MAX_PROCS 262144
#endif

/* ASan frames are bigger (redzones, -O1), so stacks and red zones scale up there */
#ifndef GEM_STACK_SCALE
#ifdef GEM_ASAN
#define GEM_STACK_SCALE 4
#else
#define GEM_STACK_SCALE 1
#endif
#endif

/* every process's stack, main included; mmap'd, so only touched pages cost memory. The program's main sets
   it to GEM_PROGRAM_STACK_SIZE before gem_scheduler_init. */
extern size_t gem_stack_size;

/* evaluated in the program's own C: a program built without optimization has larger frames and gets
   twice the stack */
#ifndef GEM_PROGRAM_STACK_SIZE
#ifdef __OPTIMIZE__
#define GEM_PROGRAM_STACK_SIZE (GEM_STACK_SCALE * 8 * 1024 * 1024)
#else
#define GEM_PROGRAM_STACK_SIZE (GEM_STACK_SCALE * 16 * 1024 * 1024)
#endif
#endif

#ifndef GEM_STACK_RED_ZONE
/* bottom of each stack that Gem calls may not enter (they raise a catchable "stack overflow"), leaving
   room for the error path and for C/libc called from the deepest Gem frame */
#define GEM_STACK_RED_ZONE (GEM_STACK_SCALE * 256 * 1024)
#endif

/* by slot, never moves (a GemProcess * stays valid); check a slot against gem_proc_hwm before indexing */
extern GemProcess *gem_proc_table;
extern long gem_runtime_maps;   /* gem_arena.c: memory mappings the runtime holds */
extern int gem_current_pid;
extern int gem_free_head;
extern int gem_free_tail;
extern int gem_proc_hwm;

void gem_yield_check(void);

void gem_scheduler_init(void);
int gem_spawn_fn(GemFnPtr fn, void *env);
void gem_send_msg(int pid, GemVal val);
GemVal gem_receive_msg(void);
int gem_self_pid(void);
/* Gem sees slot + generation * GEM_MAX_PROCS; the generation advances when a slot is freed, so an old pid
   never reaches a new process in its slot */
int64_t gem_pid_of_slot(int slot);
/* -1 if the pid is malformed or its process is gone */
int gem_slot_of_pid(int64_t pid);
void gem_run_scheduler(void);
/* end the running process with reason, past any pcall, handling DOWN, links and names as for any death;
   main with a non-"normal" reason prints it as a runtime error and exits 1 */
__attribute__((noreturn)) void gem_exit_self(const char *reason);
/* main got an exit signal from from_pid: report like an uncaught error, exit 1 */
__attribute__((noreturn)) void gem_report_main_killed(int64_t from_pid, const char *reason, int linked);
void gem_run_main(GemFnPtr fn, void *env);

void gem_mailbox_remove(GemMailbox *mb, GemMsgNode *prev, GemMsgNode *node);

void gem_selective_yield(int64_t deadline_ms);
int64_t gem_deadline_in(int64_t ms);
int64_t gem_after_deadline(GemVal ms);

int64_t gem_now_ms(void);

/* park until sock's fd may be readable (for_write 0) or writable (1). Closing the socket wakes it too, so
   the caller must re-resolve sock (gem_res_lookup) and never touch the old fd: it may name a new file. */
void gem_io_yield(int fd, int for_write, GemVal sock);

/* wake gem_io_yield waiters on a socket being closed (gem_res_closing) */
void gem_io_wake_closing(void);

/* wait for proc->io_request (set it first) */
void gem_io_pool_yield(void);
/* same, until deadline (gem_now_ms, -1: none): 1 done, 0 timed out (the worker still holds its ref) */
int gem_io_pool_yield_until(int64_t deadline);

typedef struct {
    char *key;
    int value;
} GemNameEntry;

extern GemNameEntry *gem_name_registry;

void gem_register_name(const char *name, int pid);
int gem_whereis_name(const char *name);        /* pid or -1 */
void gem_unregister_name_for_pid(int pid);

int gem_monitor_fn(int64_t target_pid);
int gem_demonitor_fn(int64_t target_pid);
void gem_deliver_down_messages(int pid, const char *reason);

void gem_link_fn(int64_t target_pid);
void gem_unlink_fn(int64_t target_pid);
/* pid's exit goes to its links: an EXIT message where trap_exit is set, otherwise a non-"normal" reason
   kills the linked process too, and so on down its links */
void gem_propagate_exit(int pid, const char *reason);

GemVal gem_spawn_builtin(void *_env, GemVal *args, int argc);
GemVal gem_send_builtin(void *_env, GemVal *args, int argc);
GemVal gem_receive_builtin(void *_env, GemVal *args, int argc);
GemVal gem_self_builtin(void *_env, GemVal *args, int argc);
GemVal gem_monitor_builtin(void *_env, GemVal *args, int argc);
GemVal gem_demonitor_builtin(void *_env, GemVal *args, int argc);
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
GemVal gem_runtime_stats_builtin(void *_env, GemVal *args, int argc);

/* Owned resources (gem_resource.c). Sockets and sqlite handles live in one table; a VAL_RESOURCE names an
   entry by serial and is that entry only while the serials match, so a closed resource stays closed in every
   copy. Each entry has an owner pid (or none), at first its opener; claim() takes it over and marks it
   claimed. At exit gem_res_proc_exit closes what the process owns if it claimed it or the exit is abnormal,
   and disowns the rest. Every close goes through gem_res_close / gem_res_take. */

enum { GEM_RES_SOCKET = 1, GEM_RES_SQLITE = 2, GEM_RES_KINDS };

typedef struct {
    int64_t serial;      /* 0: free slot */
    int kind;
    int64_t owner;       /* Gem-visible pid, or -1 for none */
    int owner_slot;
    int claimed;
    int64_t user;        /* last process that used it without owning it, or -1 */
    int user_slot;
    int parked;          /* index of the parked request it waits for, or -1 */
    int closing;         /* set while gem_res_close wakes its waiters */
    int busy;            /* `extern blocking fn` calls using its fd right now */
    int dead;            /* closed for Gem code, fd closed when busy reaches 0 */
    int32_t own_prev, own_next;  /* owner's list (or the parked request's) */
    int32_t use_prev, use_next;
    int32_t free_next;
    int fd;              /* GEM_RES_SOCKET */
    uint64_t dev, ino;   /* st_dev / st_ino of the fd when registered (on macOS a socket's kernel id for ino) */
    void *ptr;           /* sqlite3 * for GEM_RES_SQLITE; a socket's TLS session, or NULL for plain TCP */
} GemResEntry;

/* owned by the running process (none outside one); fill in the payload via gem_res_lookup */
GemVal gem_res_new(int kind);
/* NULL if closed or not a resource; valid until the next gem_res_new */
GemResEntry *gem_res_lookup(GemVal v);
/* the open entry for builtin `who`, recording the caller as last user; raises on a wrong or closed value */
GemResEntry *gem_res_get(GemVal v, int kind, const char *who);
/* same, but NULL for a closed one */
GemResEntry *gem_res_get_open(GemVal v, int kind, const char *who);
/* wake waiters, close the fd unless it now names something else (dev/ino), free the entry */
void gem_res_close(GemResEntry *e);
/* remove the entry and return the payload, for kinds that close themselves (sqlite_close, on the pool) */
void *gem_res_take(GemResEntry *e);
/* 1 while gem_res_close is closing v (for the waiter scan) */
int gem_res_closing(GemVal v);
void gem_res_proc_exit(int slot);
void gem_res_proc_init(GemProcess *proc);
/* scheduler, after draining the wake pipe: close sockets of finished parked requests */
void gem_res_check_parked(void);
int gem_res_parked_pending(void);
/* fd for a Socket param (arg argi of extern fn `fn`); raises for a non-socket or a closed one */
int gem_extern_socket_fd(GemVal v, const char *fn, int argi);
/* an `extern blocking fn` hands sockets args[idx[0..n)] to C: mark them busy for req (after submit).
   Closing one meanwhile closes it for Gem at once, but its fd only at gem_res_busy_end (after the call,
   or from the scheduler for a killed caller's request). */
void gem_res_busy_begin(GemIORequest *req, const GemVal *args, const int *idx, int n);
void gem_res_busy_end(GemIORequest *req);
GemResEntry *gem_res_find_socket(int fd, uint64_t dev, uint64_t ino);
const char *gem_res_type_name(int kind);
const char *gem_res_print_name(int kind);
void gem_res_diag_counts(int *open, int *ownerless);
/* close fd unless it now names some other file (dev/ino) */
void gem_tcp_close_fd_checked(int fd, uint64_t dev, uint64_t ino);

/* TLS (gem_builtins_tls.c). Socket builtins and the resource table reach it only through gem_tls_ops, set
   by the first tls_connect, so a program that never names tls_connect links without OpenSSL. read/write back
   tcp_read and tcp_write (args already checked); close sends close_notify if it can without waiting and
   frees the session (GemResEntry.ptr) before the fd closes. */
typedef struct {
    GemVal (*read)(GemVal sock, size_t max_bytes, int has_timeout, int64_t timeout_ms);
    GemVal (*write)(GemVal sock, const char *data, size_t total, int has_timeout, int64_t timeout_ms);
    void (*close)(void *tls);
} GemTlsOps;
extern const GemTlsOps *gem_tls_ops;
/* tcp_connect for builtin `who`, with the port check; deadline (gem_now_ms, -1: none) bounds the connect */
GemVal gem_tcp_connect_to(const char *host, int port, int64_t deadline, const char *who);
int gem_tcp_port(GemVal v, const char *who);
void gem_sqlite_exit_close(void *db);


void gem_threadpool_init(void);
void gem_threadpool_shutdown(void);
GemIORequest *gem_io_submit(GemIOOp op, const char *path,
                            const char *content, size_t content_len);
/* takes args: free_args frees it at the last release, or at once when the queue is full (NULL returned) */
GemIORequest *gem_io_submit_extern(void (*fn)(void *), void *args,
                                   void (*free_args)(void *));
GemIORequest *gem_io_submit_range(const char *path, int64_t offset, int64_t count);
void gem_io_release(GemIORequest *req);
/* whole file, malloc'd and NUL-terminated: regular files take one fread sized by fstat, anything else
   (procfs, pipes) is read to EOF. NULL and a malloc'd *err_msg on failure. Worker-safe. */
char *gem_read_whole_file(const char *path, size_t *out_len, char **err_msg);
/* every runtime fopen: close-on-exec from the start, so another thread's command can't inherit it */
FILE *gem_fopen(const char *path, const char *mode);
/* first IPv4 address of host (blocking getaddrinfo), network order; NULL or a static error string. Worker-safe. */
const char *gem_resolve4(const char *host, uint32_t *addr);
/* /bin/sh -c cmd and wait: exit status, -1 if it didn't start or exit normally; thread-safe, unlike system() */
int gem_run_shell(const char *cmd);
GemVal gem_regex_exec(const char *pat, int64_t flags, const uint8_t *s, int64_t slen, int64_t start);
int64_t gem_regex_groups(const char *pat, int64_t flags);
GemVal gem_base64_encode(const uint8_t *s, int64_t n, int64_t url);
GemVal gem_base64_decode(const uint8_t *s, int64_t n);
GemVal gem_hex_encode(const uint8_t *s, int64_t n);
GemVal gem_hex_decode(const uint8_t *s, int64_t n);
GemVal gem_sha256(const uint8_t *s, int64_t n);
GemVal gem_hmac_sha256(const uint8_t *key, int64_t klen, const uint8_t *msg, int64_t mlen);
int gem_bytes_equal_ct(const uint8_t *a, int64_t an, const uint8_t *b, int64_t bn);
int64_t gem_utf8_len(const uint8_t *s, int64_t n);
int gem_utf8_valid(const uint8_t *s, int64_t n);
GemVal gem_utf8_split(const uint8_t *s, int64_t n, int64_t as_strings);
GemVal gem_utf8_char(int64_t cp);
int64_t gem_utf8_offset(const uint8_t *s, int64_t n, int64_t index);
GemVal gem_utf8_case(const uint8_t *s, int64_t n, int64_t upper);
/* the current env as an envp for posix_spawn; one malloc'd block. Thread-safe. */
char **gem_env_snapshot(void);
/* /bin/sh -c cmd with `in` on stdin (/dev/null if NULL), stdout and stderr collected into malloc'd buffers.
   Exit status or -1; *errmsg (malloc'd) if an output went over the string limit. Thread-safe. */
int gem_run_capture(const char *cmd, const char *in, size_t in_len,
                    char **out, size_t *out_len, char **err, size_t *err_len,
                    char **errmsg);
/* up to count bytes from offset, malloc'd ("" past the end); NULL and *err_msg on failure. Worker-safe. */
char *gem_read_file_range(const char *path, int64_t offset, int64_t count,
                          size_t *out_len, char **err_msg);
/* drain the wake pipe; 1 if a request may have finished since the last call */
int gem_io_check_completions(void);
int gem_io_wake_fd(void);


GemVal gem_table_get_ic_miss(GemTable *t, const char *key, GemICacheSlot *cache);

static inline GemVal gem_table_get_cached(GemVal tbl, const char *key, GemICacheSlot *cache) {
    if (tbl.type != VAL_TABLE) {
        char buf[128];
        snprintf(buf, sizeof(buf), "field access on non-table: got %s (reading .%.64s)", gem_type_str(tbl), key);
        gem_error(buf);
    }
    GemTable *t = tbl.table;
    if (cache->table == t && cache->shape_id == t->shape_id) {
        return t->vals[cache->val_index];
    }
    return gem_table_get_ic_miss(t, key, cache);
}

static inline GemArena *gem_current_arena(void) {
    return gem_current_pid >= 0 ? &gem_proc_table[gem_current_pid].arena : &gem_global_arena;
}

/* region mark helpers, emitted at every loop entry and back-edge */

/* new clock epoch: the first write after here to an older table gets logged, so resets from here find it */
static inline void gem_arena_point(GemArenaPoint *pt) {
    GemArena *a = gem_current_arena();
    pt->block = a->current;
    pt->used = a->current->used;
    pt->tables = a->table_list;
    pt->buffers = a->buffer_list;
    pt->pin_seq = a->pin_seq;
    pt->clock = ++gem_mut_clock;
    pt->arena = a;
    pt->ret_trig = a->bytes_allocated - a->bytes_freed + gem_reset_threshold;
}

static inline void gem_arena_mark(GemArenaMark *m) {
    GemArena *a = gem_current_arena();
    gem_arena_point(&m->base);
    m->young = m->base;
    m->trigger = a->bytes_allocated + gem_reset_threshold;
    m->old_bytes = 0;
    m->old_limit = 0;
    m->rem_window = 0;
    m->idle_extra = 0;
}

/* the process blocked since the loop's last reset (or its start) and the loop has allocated
   max(gem_idle_reset_threshold, the last reset's budget) since. trigger is that point plus
   max(gem_reset_threshold, budget), and idle_extra is clamp(budget - idle threshold, 0, gap) (at most UINT32_MAX),
   0 for a fresh mark. */
static inline int gem_arena_idle_due(const GemArena *a, const GemArenaMark *m) {
    return a->wait_clock > m->young.clock &&
           a->bytes_allocated > m->trigger - gem_idle_reset_gap + m->idle_extra;
}

static inline int gem_arena_reset_due(const GemArenaMark *m) {
    if (gem_current_pid < 0) return 0;
    const GemArena *a = &gem_proc_table[gem_current_pid].arena;
    return a->bytes_allocated > m->trigger || gem_arena_idle_due(a, m);
}

/* due once the call's unfreed memory reaches the threshold: resets during the call only free memory newer
   than pt, so allocated - freed shows it (remembered-log entries count). gem_arena_reset_return checks the rest. */
static inline int gem_ret_reset_due(const GemArenaPoint *pt) {
    const GemArena *a = pt->arena;
    return a->bytes_allocated - a->bytes_freed >= pt->ret_trig;
}

#endif /* GEM_H */
