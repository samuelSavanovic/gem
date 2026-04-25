/*
 * gem.h — Gem language runtime public API.
 * Compilers target this header; implementation is in gem.c.
 */

#ifndef GEM_H
#define GEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <setjmp.h>
#include <gc.h>

/* ─── Tagged value ─── */

typedef enum {
    VAL_NIL, VAL_BOOL, VAL_INT, VAL_FLOAT, VAL_STRING, VAL_FN, VAL_TABLE,
} GemType;

typedef struct GemVal GemVal;
typedef GemVal (*GemFnPtr)(void *env, GemVal *args, int argc);

typedef struct GemTable GemTable;

struct GemVal {
    GemType type;
    union {
        int64_t ival;
        double fval;
        char *sval;
        int bval;
        struct { GemFnPtr fn; void *env; };
        GemTable *table;
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

/* ─── Table operations ─── */

GemVal gem_table_new(void);
void gem_table_set(GemVal tbl, GemVal key, GemVal val);
GemVal gem_table_get(GemVal tbl, GemVal key);

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

/* ─── Built-in functions (GemFnPtr signature) ─── */

GemVal gem_print(void *_env, GemVal *args, int argc);
GemVal gem_error_fn(void *_env, GemVal *args, int argc);
GemVal gem_len_fn(void *_env, GemVal *args, int argc);
GemVal gem_type_fn(void *_env, GemVal *args, int argc);
GemVal gem_to_string_fn(void *_env, GemVal *args, int argc);
GemVal gem_push_fn(void *_env, GemVal *args, int argc);
GemVal gem_pcall_fn(void *_env, GemVal *args, int argc);
GemVal gem_keys_fn(void *_env, GemVal *args, int argc);

/* ─── Helpers used by codegen ─── */

GemVal gem_len_val(GemVal v);
void gem_error(const char *msg);
GemVal gem_error_at_fn(const char *file, int line, GemVal *args, int argc);
GemVal gem_keys(GemVal tbl);

#endif /* GEM_H */
