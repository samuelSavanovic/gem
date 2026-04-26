/*
 * gem_ops.c — Arithmetic and comparison operators.
 */

#include "gem.h"

GemVal gem_add(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int(a.ival + b.ival);
    if (a.type == VAL_FLOAT && b.type == VAL_FLOAT) return gem_float(a.fval + b.fval);
    if (a.type == VAL_INT && b.type == VAL_FLOAT) return gem_float((double)a.ival + b.fval);
    if (a.type == VAL_FLOAT && b.type == VAL_INT) return gem_float(a.fval + (double)b.ival);
    if (a.type == VAL_STRING && b.type == VAL_STRING) {
        size_t la = strlen(a.sval), lb = strlen(b.sval);
        char *s = (char *)GC_MALLOC_ATOMIC(la + lb + 1);
        memcpy(s, a.sval, la);
        memcpy(s + la, b.sval, lb + 1);
        GemVal r; r.type = VAL_STRING; r.sval = s; return r;
    }
    gem_error("type error in +"); return GEM_NIL;
}

GemVal gem_sub(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int(a.ival - b.ival);
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_float(fa - fb);
    }
    gem_error("type error in -"); return GEM_NIL;
}

GemVal gem_mul(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int(a.ival * b.ival);
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_float(fa * fb);
    }
    gem_error("type error in *"); return GEM_NIL;
}

GemVal gem_div(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) {
        if (b.ival == 0) gem_error("division by zero");
        return gem_int(a.ival / b.ival);
    }
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        if (fb == 0.0) gem_error("division by zero");
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        return gem_float(fa / fb);
    }
    gem_error("type error in /"); return GEM_NIL;
}

GemVal gem_mod(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) {
        if (b.ival == 0) gem_error("division by zero");
        return gem_int(a.ival % b.ival);
    }
    gem_error("type error in %"); return GEM_NIL;
}

GemVal gem_eq(GemVal a, GemVal b) {
    if (a.type != b.type) return gem_bool(0);
    switch (a.type) {
        case VAL_NIL: return gem_bool(1);
        case VAL_BOOL: return gem_bool(a.bval == b.bval);
        case VAL_INT: return gem_bool(a.ival == b.ival);
        case VAL_FLOAT: return gem_bool(a.fval == b.fval);
        case VAL_STRING: return gem_bool(strcmp(a.sval, b.sval) == 0);
        default: return gem_bool(0);
    }
}

GemVal gem_neq(GemVal a, GemVal b) {
    return gem_bool(!gem_truthy(gem_eq(a, b)));
}

GemVal gem_lt(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_bool(a.ival < b.ival);
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_bool(fa < fb);
    }
    if (a.type == VAL_STRING && b.type == VAL_STRING) return gem_bool(strcmp(a.sval, b.sval) < 0);
    gem_error("type error in <"); return GEM_NIL;
}

GemVal gem_gt(GemVal a, GemVal b) { return gem_lt(b, a); }
GemVal gem_le(GemVal a, GemVal b) { return gem_bool(!gem_truthy(gem_gt(a, b))); }
GemVal gem_ge(GemVal a, GemVal b) { return gem_bool(!gem_truthy(gem_lt(a, b))); }

GemVal gem_neg(GemVal a) {
    if (a.type == VAL_INT) return gem_int(-a.ival);
    if (a.type == VAL_FLOAT) return gem_float(-a.fval);
    gem_error("type error in unary -"); return GEM_NIL;
}

GemVal gem_not(GemVal a) {
    return gem_bool(!gem_truthy(a));
}
