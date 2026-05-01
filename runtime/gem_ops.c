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
        char *s = (char *)gem_alloc(la + lb + 1);
        memcpy(s, a.sval, la);
        memcpy(s + la, b.sval, lb + 1);
        GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = s; return r;
    }
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in +: got %s and %s", gem_type_str(a), gem_type_str(b)); gem_error(buf); } return GEM_NIL;
}

GemVal gem_sub(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int(a.ival - b.ival);
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_float(fa - fb);
    }
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in -: got %s and %s", gem_type_str(a), gem_type_str(b)); gem_error(buf); } return GEM_NIL;
}

GemVal gem_mul(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int(a.ival * b.ival);
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_float(fa * fb);
    }
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in *: got %s and %s", gem_type_str(a), gem_type_str(b)); gem_error(buf); } return GEM_NIL;
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
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in /: got %s and %s", gem_type_str(a), gem_type_str(b)); gem_error(buf); } return GEM_NIL;
}

GemVal gem_mod(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) {
        if (b.ival == 0) gem_error("division by zero");
        return gem_int(a.ival % b.ival);
    }
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in %%: got %s and %s", gem_type_str(a), gem_type_str(b)); gem_error(buf); } return GEM_NIL;
}

GemVal gem_eq(GemVal a, GemVal b) {
    if (a.type != b.type) return gem_bool(0);
    switch (a.type) {
        case VAL_NIL: return gem_bool(1);
        case VAL_BOOL: return gem_bool(a.bval == b.bval);
        case VAL_INT: return gem_bool(a.ival == b.ival);
        case VAL_FLOAT: return gem_bool(a.fval == b.fval);
        case VAL_STRING: return gem_bool(strcmp(a.sval, b.sval) == 0);
        case VAL_REF: return gem_bool(a.rval == b.rval);
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
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in <: got %s and %s", gem_type_str(a), gem_type_str(b)); gem_error(buf); } return GEM_NIL;
}

GemVal gem_gt(GemVal a, GemVal b) { return gem_lt(b, a); }
GemVal gem_le(GemVal a, GemVal b) { return gem_bool(!gem_truthy(gem_gt(a, b))); }
GemVal gem_ge(GemVal a, GemVal b) { return gem_bool(!gem_truthy(gem_lt(a, b))); }

GemVal gem_neg(GemVal a) {
    if (a.type == VAL_INT) return gem_int(-a.ival);
    if (a.type == VAL_FLOAT) return gem_float(-a.fval);
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in unary -: got %s", gem_type_str(a)); gem_error(buf); } return GEM_NIL;
}

void gem_string_append(GemVal *accum, GemVal rhs) {
    if (accum->type == VAL_BUFFER) {
        GemVal args[2] = {*accum, rhs};
        gem_buf_push_fn(NULL, args, 2);
    } else if (accum->type == VAL_STRING) {
        GemBuffer *b = (GemBuffer *)gem_alloc(sizeof(GemBuffer));
        b->cap = 64;
        int slen = (int)strlen(accum->sval);
        while (b->cap <= slen) b->cap *= 2;
        b->data = (char *)gem_alloc(b->cap);
        memcpy(b->data, accum->sval, slen);
        b->len = slen;
        accum->type = VAL_BUFFER;
        accum->buffer = b;
        GemVal args[2] = {*accum, rhs};
        gem_buf_push_fn(NULL, args, 2);
    } else {
        *accum = gem_add(*accum, rhs);
    }
}

GemVal gem_string_finish(GemVal val) {
    if (val.type == VAL_BUFFER) {
        GemVal args[1] = {val};
        return gem_buf_str_fn(NULL, args, 1);
    }
    return val;
}

GemVal gem_not(GemVal a) {
    return gem_bool(!gem_truthy(a));
}
