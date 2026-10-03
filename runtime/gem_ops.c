/*
 * gem_ops.c — Arithmetic and comparison operators.
 */

#include "gem.h"

GemVal gem_add(GemVal a, GemVal b) {
    /* Int + - * and unary - wrap on overflow (two's complement): computed in
       uint64_t, since signed overflow is undefined behaviour in C. */
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int((int64_t)((uint64_t)a.ival + (uint64_t)b.ival));
    if (a.type == VAL_FLOAT && b.type == VAL_FLOAT) return gem_float(a.fval + b.fval);
    if (a.type == VAL_INT && b.type == VAL_FLOAT) return gem_float((double)a.ival + b.fval);
    if (a.type == VAL_FLOAT && b.type == VAL_INT) return gem_float(a.fval + (double)b.ival);
    if (a.type == VAL_STRING && b.type == VAL_STRING) {
        size_t la = (size_t)a.slen, lb = (size_t)b.slen;
        char *s = (char *)gem_alloc(la + lb + 1);
        memcpy(s, a.sval, la);
        memcpy(s + la, b.sval, lb);
        s[la + lb] = '\0';
        GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = s; r.slen = (int)(la + lb); return r;
    }
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in +: got %s and %s", gem_type_str(a), gem_type_str(b)); gem_error(buf); } return GEM_NIL;
}

GemVal gem_sub(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int((int64_t)((uint64_t)a.ival - (uint64_t)b.ival));
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_float(fa - fb);
    }
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in -: got %s and %s", gem_type_str(a), gem_type_str(b)); gem_error(buf); } return GEM_NIL;
}

GemVal gem_mul(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int((int64_t)((uint64_t)a.ival * (uint64_t)b.ival));
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
        /* INT64_MIN / -1 overflows (a trap on x86-64): wrap, like + - *. */
        if (b.ival == -1) return gem_int((int64_t)(0 - (uint64_t)a.ival));
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
        if (b.ival == -1) return gem_int(0);   /* INT64_MIN % -1 traps on x86-64 */
        return gem_int(a.ival % b.ival);
    }
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in %%: got %s and %s", gem_type_str(a), gem_type_str(b)); gem_error(buf); } return GEM_NIL;
}

GemVal gem_eq(GemVal a, GemVal b) {
    return gem_bool(gem_val_eq(a, b));
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
    if (a.type == VAL_STRING && b.type == VAL_STRING) {
        size_t la = (size_t)a.slen, lb = (size_t)b.slen;
        size_t n = la < lb ? la : lb;
        int cmp = memcmp(a.sval, b.sval, n);
        if (cmp != 0) return gem_bool(cmp < 0);
        return gem_bool(la < lb);
    }
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in <: got %s and %s", gem_type_str(a), gem_type_str(b)); gem_error(buf); } return GEM_NIL;
}

GemVal gem_gt(GemVal a, GemVal b) { return gem_lt(b, a); }
GemVal gem_le(GemVal a, GemVal b) { return gem_bool(!gem_truthy(gem_gt(a, b))); }
GemVal gem_ge(GemVal a, GemVal b) { return gem_bool(!gem_truthy(gem_lt(a, b))); }

GemVal gem_neg(GemVal a) {
    if (a.type == VAL_INT) return gem_int((int64_t)(0 - (uint64_t)a.ival));
    if (a.type == VAL_FLOAT) return gem_float(-a.fval);
    { char buf[128]; snprintf(buf, sizeof(buf), "type error in unary -: got %s", gem_type_str(a)); gem_error(buf); } return GEM_NIL;
}

void gem_string_append(GemVal *accum, GemVal rhs) {
    /* `s = s + x` with s a string: x must be a string too, as for `+`.
       A buffer here is the string being built, or a user's buf_new()
       buffer (KNOWN_BUGS: "A buffer passed as `s` to `s = s + x`"). */
    if ((accum->type == VAL_BUFFER || accum->type == VAL_STRING) && rhs.type != VAL_STRING) {
        char buf[128];
        snprintf(buf, sizeof(buf), "type error in +: got string and %s", gem_type_str(rhs));
        gem_error(buf);
    }
    if (accum->type == VAL_BUFFER) {
        GemVal args[2] = {*accum, rhs};
        gem_buf_push_fn(NULL, args, 2);
    } else if (accum->type == VAL_STRING) {
        int slen = accum->slen;
        int cap = 64;
        while (cap <= slen) cap *= 2;
        GemBuffer *b = gem_buffer_alloc(cap);
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
        return gem_to_string_fn(NULL, args, 1);
    }
    return val;
}

GemVal gem_not(GemVal a) {
    return gem_bool(!gem_truthy(a));
}
