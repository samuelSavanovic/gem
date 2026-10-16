
#include "stb_ds.h"
#include "gem.h"

GemVal gem_push_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("push: expected 2 arguments"); }
    GemVal tbl = args[0];
    if (tbl.type == VAL_BUFFER) return gem_buf_push_fn(_env, args, argc);
    if (tbl.type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "push: expected table or buffer as first argument, got %s", gem_type_str(tbl)); gem_error(buf); }
    GemTable *t = tbl.table;
    GemVal val = args[1];
    if (!t->is_array) {
        /* push(t, v) is t[len(t)] = v, and key len may already exist */
        gem_table_set(tbl, gem_int(t->len), val);
        return val;
    }
    gem_table_check_mutable(t);
    if (t->len >= t->cap) gem_table_grow(t);
    gem_table_written(t);
    t->keys[t->len] = gem_int(t->len);
    t->vals[t->len] = val;
    gem_key_index_put(t, t->keys[t->len], t->len);
    t->len++;
    return val;
}

/* lowered `for` loops iterate by index with these; a string's entry i is a cached one-byte string, no allocation */

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

static int gem_table_is_array_keys(GemTable *t);

/* one-variable `for`: checked once up front, must be a string or an array */
GemVal gem_for_items_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("__for_items: expected 1 argument"); return GEM_NIL; }
    GemVal v = args[0];
    if (v.type == VAL_STRING) return v;
    if (v.type != VAL_TABLE) { gem_for_type_error(v); return GEM_NIL; }
    GemTable *t = v.table;
    if (gem_table_is_array_keys(t)) return v;
    char key[96];
    for (int i = 0; i < t->len; i++) {
        GemVal k = t->keys[i];
        if (k.type == VAL_STRING) {
            snprintf(key, sizeof(key), "\"%.*s%s\"", k.slen > 40 ? 40 : (int)k.slen, k.sval, k.slen > 40 ? "..." : "");
            break;
        }
        if (k.type != VAL_INT || k.ival < 0 || k.ival >= t->len) {
            if (k.type == VAL_INT) snprintf(key, sizeof(key), "%lld", (long long)k.ival);
            else snprintf(key, sizeof(key), "of type %s", gem_type_str(k));
            break;
        }
    }
    char buf[256];
    snprintf(buf, sizeof(buf), "for: one loop variable needs an array or a string, got a table with the key %s; use two (`for k, v in ...`) for its entries, or iterate `values(...)`", key);
    gem_error(buf);
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
        return gem_table_get(args[0], gem_int(idx));
    }
    if (args[0].type != VAL_TABLE) { gem_for_type_error(args[0]); return GEM_NIL; }
    GemTable *t = args[0].table;
    if (idx < 0 || idx >= t->len) return GEM_NIL;
    return t->vals[idx];
}

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

GemVal gem_has_key_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("has_key: expected 2 arguments"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "has_key: expected table as first argument, got %s", gem_type_str(args[0])); gem_error(buf); }
    GemTable *t = args[0].table;
    GemVal key = args[1];

    if (key.type == VAL_STRING) {
        return gem_bool(gem_table_str_pos(t, key.sval, key.slen) >= 0);
    }

    if (key.type == VAL_INT) {
        int64_t ik = key.ival;
        if (ik >= 0 && ik < t->len && t->keys[ik].type == VAL_INT && t->keys[ik].ival == ik) {
            return gem_bool(1);
        }
        if (t->is_array) return gem_bool(0);
    }

    return gem_bool(gem_table_key_pos(t, key) >= 0);
}

/* keys are exactly 0 .. len-1 in any order; keys are distinct, so len int keys in [0, len) are all of them */
static int gem_table_is_array_keys(GemTable *t) {
    if (t->is_array) return 1;
    for (int i = 0; i < t->len; i++) {
        GemVal k = t->keys[i];
        if (k.type != VAL_INT || k.ival < 0 || k.ival >= t->len) return 0;
    }
    return 1;
}

/* `[p1, ..., pn]` patterns: an array of exactly n */
GemVal gem_is_array_n_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_TABLE || args[1].type != VAL_INT) return gem_bool(0);
    GemTable *t = args[0].table;
    if ((int64_t)t->len != args[1].ival) return gem_bool(0);
    return gem_bool(gem_table_is_array_keys(t));
}

/* `x in t`: searches the values of a non-empty array, else has_key(t, x) */

GemVal gem_in_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("in: expected 2 arguments"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "in: right operand must be a table, got %s", gem_type_str(args[0])); gem_error(buf); }
    GemTable *t = args[0].table;
    GemVal needle = args[1];
    gem_table_index(t);

    if (t->len > 0 && t->nstr == 0 && gem_table_is_array_keys(t)) {
        for (int i = 0; i < t->len; i++) {
            if (gem_val_eq(t->vals[i], needle)) return gem_bool(1);
        }
        return gem_bool(0);
    }

    if (needle.type == VAL_STRING) {
        return gem_bool(gem_table_str_pos(t, needle.sval, needle.slen) >= 0);
    }

    return gem_bool(gem_table_key_pos(t, needle) >= 0);
}

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
        pos = gem_table_key_pos(t, key);
    }

    if (pos < 0) return GEM_NIL;
    GemVal removed = t->vals[pos];

    if (key.type == VAL_STRING) {
        gem_str_index_del(t->str_index, key.sval, key.slen);
        t->nstr--;
    } else {
        gem_key_index_del(t, key);
    }

    int last = t->len - 1;
    if (pos < last) {
        t->is_array = 0;
        GemVal moved_key = t->keys[last];
        t->keys[pos] = moved_key;
        t->vals[pos] = t->vals[last];
        if (moved_key.type == VAL_STRING && t->str_index != NULL) {
            gem_str_index_put(&t->str_index, moved_key.sval, moved_key.slen, pos);
        } else {
            gem_key_index_put(t, moved_key, pos);
        }
    }
    t->len--;
    if (t->len == 0) t->is_array = 1;
    t->shape_id++;
    return removed;
}

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
    } else {
        gem_key_index_del(t, removed_key);
    }
    if (t->len == 0) t->is_array = 1;
    t->shape_id++;
    return removed;
}

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

/* The comparator is Gem code: it can sort again or yield to a process that sorts, so sort state lives on
   the C stack, and it can raise, so we sort an arena copy and write back only at the end. After a comparator
   call we reset to a mark taken after the copy, with no roots: the elements, the copy and the comparator's
   env all predate the mark, and the result is a number. */
__attribute__((noinline, cold, noreturn))
static void gem_sort_bad_result(GemVal result) {
    char buf[160];
    snprintf(buf, sizeof(buf), "sort: the comparator must return a number (negative, zero or positive), got %s", gem_type_str(result));
    gem_error(buf);
    abort();
}

__attribute__((noinline, cold))
static void gem_sort_reset(GemArenaMark *mark) {
    gem_arena_reset_region(mark, NULL, 0, NULL, 0);
}

static inline int gem_call_cmp(GemVal cmp, GemArenaMark *mark, GemVal a, GemVal b) {
    GemVal cmp_args[2] = {a, b};
    GemVal result = cmp.fn(cmp.env, cmp_args, 2);
    int order;
    if (result.type == VAL_INT) order = (result.ival > 0) - (result.ival < 0);
    else if (result.type == VAL_FLOAT) order = (result.fval > 0) - (result.fval < 0);
    else gem_sort_bad_result(result);
    if (__builtin_expect(gem_arena_reset_due(mark), 0)) gem_sort_reset(mark);
    return order;
}

/* stable; result in dst, src clobbered */
static void gem_merge_sort(GemVal cmp, GemArenaMark *mark, GemVal *src, GemVal *dst, int n) {
    const int RUN = 8;
    for (int lo = 0; lo < n; lo += RUN) {
        int hi = lo + RUN < n ? lo + RUN : n;
        for (int i = lo + 1; i < hi; i++) {
            GemVal v = src[i];
            int j = i;
            while (j > lo && gem_call_cmp(cmp, mark, src[j - 1], v) > 0) {
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
                if (gem_call_cmp(cmp, mark, from[j], from[i]) < 0) to[k++] = from[j++];
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
        GemArenaMark mark;
        gem_arena_mark(&mark);
        gem_merge_sort(args[1], &mark, buf, buf + n, n);
        if (t->len != n) gem_error("sort: the comparator changed the length of the table being sorted");
        /* again: the comparator may have spawned and snapshotted the table */
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
    /* no string keys left; the old index points at unrooted key strings */
    gem_str_index_free(&t->str_index);
    t->nstr = 0;
    t->index_stale = 0;
    t->shape_id++;
    return args[0];
}

/* After renumbering keys the string index may point at key strings no longer in the table (a reset frees
   them): mark it stale, and gem_table_index rebuilds it and nstr. */
static void gem_table_drop_str_index(GemTable *t) {
    if (t->str_index == NULL && t->nstr == 0) return;
    gem_str_index_free(&t->str_index);
    t->index_stale = 1;
}

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
