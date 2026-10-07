/*
 * gem_builtins_collection.c — Table/array builtins: push, pop, keys, values,
 *                              has_key, in, delete, sort, insert, remove_at.
 */

#include "stb_ds.h"
#include "gem.h"

/* ─── Built-in: push ─── */

GemVal gem_push_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("push: expected 2 arguments"); }
    GemVal tbl = args[0];
    if (tbl.type == VAL_BUFFER) return gem_buf_push_fn(_env, args, argc);
    if (tbl.type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "push: expected table or buffer as first argument, got %s", gem_type_str(tbl)); gem_error(buf); }
    GemTable *t = tbl.table;
    GemVal val = args[1];
    if (!t->is_array) {
        /* Key len may already be in the table: push(t, v) is t[len(t)] = v. */
        gem_table_set(tbl, gem_int(t->len), val);
        return val;
    }
    gem_table_check_mutable(t);
    if (t->len >= t->cap) gem_table_grow(t);
    gem_table_written(t);
    t->keys[t->len] = gem_int(t->len);
    t->vals[t->len] = val;
    t->len++;
    return val;
}

/* ─── Built-in: __for_len / __table_key_at / __table_val_at ───
 * The `for` loop lowering (compiler/lower.gem) iterates by index:
 * `__for_len` gives the entry count of a table or the byte count of a
 * string, and raises a `for:` error for anything else; the two-variable
 * form reads entry idx with `__table_key_at` / `__table_val_at` (on a
 * string: the byte index and the one-byte string, which `s[i]` takes
 * from the runtime's char cache, so no allocation per iteration). */

static void gem_for_type_error(GemVal v) {
    char buf[128];
    snprintf(buf, sizeof(buf), "for: expected a table or string to iterate, got %s", gem_type_str(v));
    gem_error(buf);
}

GemVal gem_for_len_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("__for_len: expected 1 argument"); return GEM_NIL; }
    if (args[0].type == VAL_TABLE) return gem_int((int64_t)args[0].table->len);
    if (args[0].type == VAL_STRING) return gem_int((int64_t)args[0].slen);
    gem_for_type_error(args[0]);
    return GEM_NIL;
}

GemVal gem_table_key_at_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("__table_key_at: expected 2 arguments"); return GEM_NIL; }
    int64_t idx = args[1].ival;
    if (args[0].type == VAL_STRING) {
        if (idx < 0 || idx >= (int64_t)args[0].slen) return GEM_NIL;
        return gem_int(idx);
    }
    if (args[0].type != VAL_TABLE) { gem_for_type_error(args[0]); return GEM_NIL; }
    GemTable *t = args[0].table;
    if (idx < 0 || idx >= t->len) return GEM_NIL;
    return t->keys[idx];
}

GemVal gem_table_val_at_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("__table_val_at: expected 2 arguments"); return GEM_NIL; }
    int64_t idx = args[1].ival;
    if (args[0].type == VAL_STRING) {
        if (idx < 0 || idx >= (int64_t)args[0].slen) return GEM_NIL;
        return gem_table_get(args[0], gem_int(idx));   /* cached one-byte string */
    }
    if (args[0].type != VAL_TABLE) { gem_for_type_error(args[0]); return GEM_NIL; }
    GemTable *t = args[0].table;
    if (idx < 0 || idx >= t->len) return GEM_NIL;
    return t->vals[idx];
}

/* ─── Built-in: keys ─── */

GemVal gem_keys(GemVal tbl) {
    if (tbl.type != VAL_TABLE) {
        char buf[128]; snprintf(buf, sizeof(buf), "keys: expected table, got %s", gem_type_str(tbl)); gem_error(buf);
        return GEM_NIL;
    }
    GemTable *t = tbl.table;
    GemVal result = gem_table_new();
    for (int i = 0; i < t->len; i++) {
        gem_table_set(result, gem_int(i), t->keys[i]);
    }
    return result;
}

GemVal gem_keys_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("keys: expected 1 argument"); return GEM_NIL; }
    return gem_keys(args[0]);
}

/* ─── Built-in: has_key ─── */

GemVal gem_has_key_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("has_key: expected 2 arguments"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "has_key: expected table as first argument, got %s", gem_type_str(args[0])); gem_error(buf); }
    GemTable *t = args[0].table;
    GemVal key = args[1];

    if (key.type == VAL_STRING) {
        return gem_bool(gem_table_str_pos(t, key.sval, key.slen) >= 0);
    }

    /* Integer key: try direct array indexing */
    if (key.type == VAL_INT) {
        int64_t ik = key.ival;
        if (ik >= 0 && ik < t->len && t->keys[ik].type == VAL_INT && t->keys[ik].ival == ik) {
            return gem_bool(1);
        }
        if (t->is_array) return gem_bool(0);
    }

    /* Fallback: linear scan */
    for (int i = 0; i < t->len; i++) {
        if (gem_val_eq(t->keys[i], key)) return gem_bool(1);
    }
    return gem_bool(0);
}

/* ─── Internal: __is_array_n (array pattern check) ───
 * True when args[0] is a table with exactly n entries whose keys are the
 * ints 0 .. n-1, n = args[1]. lower() emits it for `[p1, ..., pn]`
 * patterns; not user-visible. Keys in a table are distinct, so n int keys
 * that all fall in [0, n) are exactly 0 .. n-1 -- no lookups needed. */

/* An array: its keys are exactly 0 .. len-1, in any order (keys are
   distinct, so len int keys in that range are all of them). */
static int gem_table_is_array_keys(GemTable *t) {
    if (t->is_array) return 1;
    for (int i = 0; i < t->len; i++) {
        GemVal k = t->keys[i];
        if (k.type != VAL_INT || k.ival < 0 || k.ival >= t->len) return 0;
    }
    return 1;
}

GemVal gem_is_array_n_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_TABLE || args[1].type != VAL_INT) return gem_bool(0);
    GemTable *t = args[0].table;
    if ((int64_t)t->len != args[1].ival) return gem_bool(0);
    return gem_bool(gem_table_is_array_keys(t));
}

/* ─── Built-in: in operator (value membership for arrays, key check for tables) ─── */

GemVal gem_in_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("in: expected 2 arguments"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "in: right operand must be a table, got %s", gem_type_str(args[0])); gem_error(buf); }
    GemTable *t = args[0].table;
    GemVal needle = args[1];
    gem_table_index(t);

    /* No string keys (arrays, int-keyed tables): scan values for membership */
    if (t->nstr == 0) {
        for (int i = 0; i < t->len; i++) {
            if (gem_val_eq(t->vals[i], needle)) return gem_bool(1);
        }
        return gem_bool(0);
    }

    /* String-keyed table: check if needle is a key */
    if (needle.type == VAL_STRING) {
        return gem_bool(gem_table_str_pos(t, needle.sval, needle.slen) >= 0);
    }

    /* Fallback: linear scan of keys */
    for (int i = 0; i < t->len; i++) {
        if (gem_val_eq(t->keys[i], needle)) return gem_bool(1);
    }
    return gem_bool(0);
}

/* ─── Built-in: delete (remove key from table) ─── */

GemVal gem_delete_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("delete: expected 2 arguments"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "delete: expected table, got %s", gem_type_str(args[0])); gem_error(buf); }
    GemTable *t = args[0].table;
    GemVal key = args[1];
    int pos = -1;
    gem_table_check_mutable(t);
    gem_table_index(t);

    if (key.type == VAL_STRING) {
        pos = gem_table_str_pos(t, key.sval, key.slen);
    } else {
        for (int i = 0; i < t->len; i++) {
            if (gem_val_eq(t->keys[i], key)) { pos = i; break; }
        }
    }

    if (pos < 0) return GEM_NIL;
    GemVal removed = t->vals[pos];

    if (key.type == VAL_STRING) {
        gem_str_index_del(t->str_index, key.sval, key.slen);
        t->nstr--;
    }

    int last = t->len - 1;
    if (pos < last) {
        t->is_array = 0;
        GemVal moved_key = t->keys[last];
        t->keys[pos] = moved_key;
        t->vals[pos] = t->vals[last];
        if (moved_key.type == VAL_STRING && t->str_index != NULL) {
            gem_str_index_put(&t->str_index, moved_key.sval, moved_key.slen, pos);
        }
    }
    t->len--;
    if (t->len == 0) t->is_array = 1;
    t->shape_id++;
    return removed;
}

/* ─── Built-in: pop (remove and return last element) ─── */

GemVal gem_pop_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("pop: expected 1 argument"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "pop: expected table, got %s", gem_type_str(args[0])); gem_error(buf); }
    GemTable *t = args[0].table;
    gem_table_check_mutable(t);
    if (t->len == 0) { gem_error("pop: empty table"); }
    gem_table_index(t);
    t->len--;
    GemVal removed = t->vals[t->len];
    GemVal removed_key = t->keys[t->len];
    if (removed_key.type == VAL_STRING) {
        gem_str_index_del(t->str_index, removed_key.sval, removed_key.slen);
        t->nstr--;
    }
    if (t->len == 0) t->is_array = 1;
    t->shape_id++;
    return removed;
}

/* ─── Built-in: values ─── */

GemVal gem_values_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("values: expected 1 argument"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "values: expected table, got %s", gem_type_str(args[0])); gem_error(buf); }
    GemTable *t = args[0].table;
    GemVal result = gem_table_new();
    for (int i = 0; i < t->len; i++) {
        gem_table_set(result, gem_int(i), t->vals[i]);
    }
    return result;
}

/* ─── Built-in: sort (in-place, optional comparator) ─── */

static int gem_default_cmp(const void *a, const void *b) {
    GemVal va = *(const GemVal *)a;
    GemVal vb = *(const GemVal *)b;
    if (va.type == VAL_INT && vb.type == VAL_INT) {
        return (va.ival > vb.ival) - (va.ival < vb.ival);
    }
    if (va.type == VAL_FLOAT && vb.type == VAL_FLOAT) {
        return (va.fval > vb.fval) - (va.fval < vb.fval);
    }
    if (va.type == VAL_STRING && vb.type == VAL_STRING) {
        size_t la = (size_t)va.slen, lb = (size_t)vb.slen;
        size_t n = la < lb ? la : lb;
        int cmp = memcmp(va.sval, vb.sval, n);
        if (cmp != 0) return cmp;
        return (la > lb) - (la < lb);
    }
    if ((va.type == VAL_INT || va.type == VAL_FLOAT) &&
        (vb.type == VAL_INT || vb.type == VAL_FLOAT)) {
        double da = va.type == VAL_INT ? (double)va.ival : va.fval;
        double db = vb.type == VAL_INT ? (double)vb.ival : vb.fval;
        return (da > db) - (da < db);
    }
    return (int)va.type - (int)vb.type;
}

/* A user comparator runs Gem code, which can sort again (a nested sort) or
   yield to another process that sorts (at a loop back-edge), so its state
   lives in this call's C frame, never in a global. It can also raise, which
   longjmps out of the sort: the merge sort below works on an arena copy and
   only writes the table back once it is done, so an error leaves the table
   as it was, and the arena copy needs no freeing. */
static int gem_call_cmp(GemVal cmp, GemVal a, GemVal b) {
    GemVal cmp_args[2] = {a, b};
    GemVal result = cmp.fn(cmp.env, cmp_args, 2);
    if (result.type == VAL_INT) {
        int64_t v = result.ival;
        return (v > 0) - (v < 0);
    }
    if (result.type == VAL_FLOAT) return (result.fval > 0) - (result.fval < 0);
    return 0;
}

/* Stable merge sort of src[0..n) into dst (both n long; src is clobbered). */
static void gem_merge_sort(GemVal cmp, GemVal *src, GemVal *dst, int n) {
    /* Insertion-sort runs of 8 in src, then merge runs back and forth. */
    const int RUN = 8;
    for (int lo = 0; lo < n; lo += RUN) {
        int hi = lo + RUN < n ? lo + RUN : n;
        for (int i = lo + 1; i < hi; i++) {
            GemVal v = src[i];
            int j = i;
            while (j > lo && gem_call_cmp(cmp, src[j - 1], v) > 0) {
                src[j] = src[j - 1];
                j--;
            }
            src[j] = v;
        }
    }
    GemVal *from = src, *to = dst;
    for (int width = RUN; width < n; width *= 2) {
        for (int lo = 0; lo < n; lo += 2 * width) {
            int mid = lo + width < n ? lo + width : n;
            int hi = lo + 2 * width < n ? lo + 2 * width : n;
            int i = lo, j = mid, k = lo;
            while (i < mid && j < hi) {
                if (gem_call_cmp(cmp, from[j], from[i]) < 0) to[k++] = from[j++];
                else to[k++] = from[i++];
            }
            while (i < mid) to[k++] = from[i++];
            while (j < hi) to[k++] = from[j++];
        }
        GemVal *t = from; from = to; to = t;
    }
    if (from != dst) memcpy(dst, from, (size_t)n * sizeof(GemVal));
}

GemVal gem_sort_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("sort: expected 1-2 arguments"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "sort: expected table, got %s", gem_type_str(args[0])); gem_error(buf); }
    if (argc >= 2 && args[1].type != VAL_FN && args[1].type != VAL_NIL) {
        char buf[128]; snprintf(buf, sizeof(buf), "sort: comparator must be a fn, got %s", gem_type_str(args[1])); gem_error(buf);
    }
    GemTable *t = args[0].table;
    gem_table_check_mutable(t);
    if (!gem_table_is_array_keys(t)) gem_error("sort: expected an array (keys 0 .. n-1), got a table with other keys");
    if (t->len == 0) return args[0];

    if (argc >= 2 && args[1].type == VAL_FN) {
        int n = t->len;
        GemVal *buf = (GemVal *)gem_alloc((size_t)n * 2 * sizeof(GemVal));
        memcpy(buf, t->vals, (size_t)n * sizeof(GemVal));
        gem_merge_sort(args[1], buf, buf + n, n);
        /* The comparator may have resized the table meanwhile. */
        if (t->len != n) gem_error("sort: the comparator changed the length of the table being sorted");
        /* Again: the comparator may have spawned, putting the table in a
           module snapshot unit that must not see the sorted order. */
        gem_table_check_mutable(t);
        memcpy(t->vals, buf + n, (size_t)n * sizeof(GemVal));
        gem_table_written(t);
    } else {
        qsort(t->vals, (size_t)t->len, sizeof(GemVal), gem_default_cmp);
    }
    for (int i = 0; i < t->len; i++) {
        t->keys[i] = gem_int(i);
    }
    t->is_array = 1;
    /* No string keys are left: drop their index, whose entries point at
       key strings nothing roots any more. */
    gem_str_index_free(&t->str_index);
    t->nstr = 0;
    t->index_stale = 0;
    t->shape_id++;
    return args[0];
}

/* After keys were renumbered to ints: the string-key index may point at
   key strings that are gone from the table, which nothing roots any more
   (a reset frees them), and nstr may count them. Drop both; gem_table_index
   recomputes them from the string keys left below the renumbered range. */
static void gem_table_drop_str_index(GemTable *t) {
    if (t->str_index == NULL && t->nstr == 0) return;
    gem_str_index_free(&t->str_index);
    t->index_stale = 1;
}

/* ─── Built-in: insert (insert at index in array) ─── */

GemVal gem_insert_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 3) { gem_error("insert: expected 3 arguments (table, index, value)"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "insert: expected table, got %s", gem_type_str(args[0])); gem_error(buf); }
    if (args[1].type != VAL_INT) { gem_error("insert: index must be an integer"); }
    GemTable *t = args[0].table;
    gem_table_check_mutable(t);
    int64_t idx = args[1].ival;
    if (idx < 0 || idx > t->len) { gem_error("insert: index out of bounds"); }
    if (t->len >= t->cap) gem_table_grow(t);
    int pos = (int)idx;
    gem_table_written(t);
    memmove(&t->vals[pos + 1], &t->vals[pos], ((size_t)(t->len - pos)) * sizeof(GemVal));
    t->vals[pos] = args[2];
    t->len++;
    for (int i = pos; i < t->len; i++) {
        t->keys[i] = gem_int(i);
    }
    gem_table_drop_str_index(t);
    t->shape_id++;
    return args[0];
}

/* ─── Built-in: remove_at (remove element at index) ─── */

GemVal gem_remove_at_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("remove_at: expected 2 arguments (table, index)"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "remove_at: expected table, got %s", gem_type_str(args[0])); gem_error(buf); }
    if (args[1].type != VAL_INT) { gem_error("remove_at: index must be an integer"); }
    GemTable *t = args[0].table;
    gem_table_check_mutable(t);
    int64_t idx = args[1].ival;
    if (idx < 0 || idx >= t->len) { gem_error("remove_at: index out of bounds"); }
    int pos = (int)idx;
    GemVal removed = t->vals[pos];
    memmove(&t->vals[pos], &t->vals[pos + 1], ((size_t)(t->len - pos - 1)) * sizeof(GemVal));
    t->len--;
    for (int i = pos; i < t->len; i++) {
        t->keys[i] = gem_int(i);
    }
    gem_table_drop_str_index(t);
    t->shape_id++;
    return removed;
}
