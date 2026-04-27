/*
 * gem_builtins_math.c — Math builtins (floor, ceil, round, abs, pow, sqrt,
 *                        random) and bitwise operations (band..bshr).
 */

#include "gem.h"
#include <math.h>

/* ─── Helper: coerce GemVal to double ─── */

static double gem_to_num(GemVal v, const char *fn_name) {
    if (v.type == VAL_INT) return (double)v.ival;
    if (v.type == VAL_FLOAT) return v.fval;
    char buf[128]; snprintf(buf, sizeof(buf), "%s: expected number, got %s", fn_name, gem_type_str(v));
    gem_error(buf);
    return 0;
}

/* ─── Math builtins ─── */

GemVal gem_floor_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("floor: expected 1 argument"); }
    if (args[0].type == VAL_INT) return args[0];
    return gem_int((int64_t)floor(gem_to_num(args[0], "floor")));
}

GemVal gem_ceil_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("ceil: expected 1 argument"); }
    if (args[0].type == VAL_INT) return args[0];
    return gem_int((int64_t)ceil(gem_to_num(args[0], "ceil")));
}

GemVal gem_round_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("round: expected 1 argument"); }
    if (args[0].type == VAL_INT) return args[0];
    return gem_int((int64_t)round(gem_to_num(args[0], "round")));
}

GemVal gem_abs_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("abs: expected 1 argument"); }
    if (args[0].type == VAL_INT) {
        int64_t v = args[0].ival;
        return gem_int(v < 0 ? -v : v);
    }
    return gem_float(fabs(gem_to_num(args[0], "abs")));
}

GemVal gem_pow_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("pow: expected 2 arguments"); }
    double base = gem_to_num(args[0], "pow");
    double exp = gem_to_num(args[1], "pow");
    double result = pow(base, exp);
    if (args[0].type == VAL_INT && args[1].type == VAL_INT && args[1].ival >= 0) {
        return gem_int((int64_t)result);
    }
    return gem_float(result);
}

GemVal gem_sqrt_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("sqrt: expected 1 argument"); }
    double v = gem_to_num(args[0], "sqrt");
    if (v < 0) { gem_error("sqrt: negative argument"); }
    return gem_float(sqrt(v));
}

/* ─── Built-in: random ─── */

extern uint64_t gem_rng_next(void);

GemVal gem_random_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc > 0 && args[0].type == VAL_INT) {
        int64_t n = args[0].ival;
        if (n <= 0) { gem_error("random: argument must be positive"); }
        return gem_int((int64_t)(gem_rng_next() % (uint64_t)n));
    }
    return gem_float((double)gem_rng_next() / (double)UINT64_MAX);
}

/* ─── Bitwise operations ─── */

static int64_t gem_require_int(GemVal v, const char *fn_name) {
    if (v.type != VAL_INT) {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s: expected int, got %s", fn_name, gem_type_str(v));
        gem_error(buf);
    }
    return v.ival;
}

GemVal gem_band_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("band: expected 2 arguments"); }
    return gem_int(gem_require_int(args[0], "band") & gem_require_int(args[1], "band"));
}

GemVal gem_bor_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("bor: expected 2 arguments"); }
    return gem_int(gem_require_int(args[0], "bor") | gem_require_int(args[1], "bor"));
}

GemVal gem_bxor_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("bxor: expected 2 arguments"); }
    return gem_int(gem_require_int(args[0], "bxor") ^ gem_require_int(args[1], "bxor"));
}

GemVal gem_bnot_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("bnot: expected 1 argument"); }
    return gem_int(~gem_require_int(args[0], "bnot"));
}

GemVal gem_bshl_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("bshl: expected 2 arguments"); }
    int64_t a = gem_require_int(args[0], "bshl");
    int64_t n = gem_require_int(args[1], "bshl");
    if (n < 0 || n >= 64) { gem_error("bshl: shift amount out of range (0..63)"); }
    return gem_int((int64_t)((uint64_t)a << (uint64_t)n));
}

GemVal gem_bshr_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("bshr: expected 2 arguments"); }
    int64_t a = gem_require_int(args[0], "bshr");
    int64_t n = gem_require_int(args[1], "bshr");
    if (n < 0 || n >= 64) { gem_error("bshr: shift amount out of range (0..63)"); }
    return gem_int((int64_t)((uint64_t)a >> (uint64_t)n));
}
