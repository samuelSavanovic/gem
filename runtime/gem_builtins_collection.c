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
    if (tbl.type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "push: expected table as first argument, got %s", gem_type_str(tbl)); gem_error(buf); }
    GemTable *t = tbl.table;
    GemVal val = args[1];
    if (t->len >= t->cap) gem_table_grow(t);
    t->keys[t->len] = gem_int(t->len);
    t->vals[t->len] = val;
    t->len++;
    return val;
}

/* ─── Built-in: __table_key_at / __table_val_at (direct table iteration) ─── */

GemVal gem_table_key_at_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("__table_key_at: expected 2 arguments"); return GEM_NIL; }
    if (args[0].type != VAL_TABLE) { gem_error("__table_key_at: expected table"); return GEM_NIL; }
    GemTable *t = args[0].table;
    int idx = (int)args[1].ival;
    if (idx < 0 || idx >= t->len) return GEM_NIL;
    return t->keys[idx];
}

GemVal gem_table_val_at_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("__table_val_at: expected 2 arguments"); return GEM_NIL; }
    if (args[0].type != VAL_TABLE) { gem_error("__table_val_at: expected table"); return GEM_NIL; }
    GemTable *t = args[0].table;
    int idx = (int)args[1].ival;
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

    /* String key: use hash index */
    if (key.type == VAL_STRING) {
        if (t->str_index != NULL) {
            ptrdiff_t idx = shgeti(t->str_index, key.sval);
            if (idx >= 0) return gem_bool(1);
        }
        return gem_bool(0);
    }

    /* Integer key: try direct array indexing */
    if (key.type == VAL_INT) {
        int64_t ik = key.ival;
        if (ik >= 0 && ik < t->len && t->keys[ik].type == VAL_INT && t->keys[ik].ival == ik) {
            return gem_bool(1);
        }
    }

    /* Fallback: linear scan */
    for (int i = 0; i < t->len; i++) {
        if (gem_val_eq(t->keys[i], key)) return gem_bool(1);
    }
    return gem_bool(0);
}

/* ─── Built-in: in operator (value membership for arrays, key check for tables) ─── */

GemVal gem_in_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("in: expected 2 arguments"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "in: expected table as first argument, got %s", gem_type_str(args[0])); gem_error(buf); }
    GemTable *t = args[0].table;
    GemVal needle = args[1];

    /* Array (no string keys): scan values for membership */
    if (t->str_index == NULL) {
        for (int i = 0; i < t->len; i++) {
            if (gem_val_eq(t->vals[i], needle)) return gem_bool(1);
        }
        return gem_bool(0);
    }

    /* String-keyed table: check if needle is a key */
    if (needle.type == VAL_STRING) {
        ptrdiff_t idx = shgeti(t->str_index, needle.sval);
        return gem_bool(idx >= 0);
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

    if (key.type == VAL_STRING) {
        if (t->str_index != NULL) {
            ptrdiff_t idx = shgeti(t->str_index, key.sval);
            if (idx >= 0) pos = t->str_index[idx].value;
        }
    } else {
        for (int i = 0; i < t->len; i++) {
            if (gem_val_eq(t->keys[i], key)) { pos = i; break; }
        }
    }

    if (pos < 0) return GEM_NIL;
    GemVal removed = t->vals[pos];

    if (key.type == VAL_STRING && t->str_index != NULL) {
        shdel(t->str_index, key.sval);
    }

    int last = t->len - 1;
    if (pos < last) {
        GemVal moved_key = t->keys[last];
        t->keys[pos] = moved_key;
        t->vals[pos] = t->vals[last];
        if (moved_key.type == VAL_STRING && t->str_index != NULL) {
            shput(t->str_index, moved_key.sval, pos);
        }
    }
    t->len--;
    return removed;
}

/* ─── Built-in: pop (remove and return last element) ─── */

GemVal gem_pop_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("pop: expected 1 argument"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "pop: expected table, got %s", gem_type_str(args[0])); gem_error(buf); }
    GemTable *t = args[0].table;
    if (t->len == 0) { gem_error("pop: empty table"); }
    t->len--;
    GemVal removed = t->vals[t->len];
    GemVal removed_key = t->keys[t->len];
    if (removed_key.type == VAL_STRING && t->str_index != NULL) {
        shdel(t->str_index, removed_key.sval);
    }
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

static GemVal gem_sort_cmp_fn_global;

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
        return strcmp(va.sval, vb.sval);
    }
    if ((va.type == VAL_INT || va.type == VAL_FLOAT) &&
        (vb.type == VAL_INT || vb.type == VAL_FLOAT)) {
        double da = va.type == VAL_INT ? (double)va.ival : va.fval;
        double db = vb.type == VAL_INT ? (double)vb.ival : vb.fval;
        return (da > db) - (da < db);
    }
    return (int)va.type - (int)vb.type;
}

static int gem_custom_cmp(const void *a, const void *b) {
    GemVal cmp_args[2] = {*(const GemVal *)a, *(const GemVal *)b};
    GemVal result = gem_sort_cmp_fn_global.fn(gem_sort_cmp_fn_global.env, cmp_args, 2);
    if (result.type == VAL_INT) {
        int64_t v = result.ival;
        return (v > 0) - (v < 0);
    }
    if (result.type == VAL_FLOAT) return (result.fval > 0) - (result.fval < 0);
    return 0;
}

GemVal gem_sort_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("sort: expected 1-2 arguments"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "sort: expected table, got %s", gem_type_str(args[0])); gem_error(buf); }
    GemTable *t = args[0].table;
    if (t->len <= 1) return args[0];

    if (argc >= 2 && args[1].type == VAL_FN) {
        gem_sort_cmp_fn_global = args[1];
        qsort(t->vals, (size_t)t->len, sizeof(GemVal), gem_custom_cmp);
    } else {
        qsort(t->vals, (size_t)t->len, sizeof(GemVal), gem_default_cmp);
    }
    for (int i = 0; i < t->len; i++) {
        t->keys[i] = gem_int(i);
    }
    return args[0];
}

/* ─── Built-in: insert (insert at index in array) ─── */

GemVal gem_insert_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 3) { gem_error("insert: expected 3 arguments (table, index, value)"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "insert: expected table, got %s", gem_type_str(args[0])); gem_error(buf); }
    if (args[1].type != VAL_INT) { gem_error("insert: index must be an integer"); }
    GemTable *t = args[0].table;
    int64_t idx = args[1].ival;
    if (idx < 0 || idx > t->len) { gem_error("insert: index out of bounds"); }
    if (t->len >= t->cap) gem_table_grow(t);
    int pos = (int)idx;
    memmove(&t->vals[pos + 1], &t->vals[pos], ((size_t)(t->len - pos)) * sizeof(GemVal));
    t->vals[pos] = args[2];
    t->len++;
    for (int i = pos; i < t->len; i++) {
        t->keys[i] = gem_int(i);
    }
    return args[0];
}

/* ─── Built-in: remove_at (remove element at index) ─── */

GemVal gem_remove_at_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("remove_at: expected 2 arguments (table, index)"); }
    if (args[0].type != VAL_TABLE) { char buf[128]; snprintf(buf, sizeof(buf), "remove_at: expected table, got %s", gem_type_str(args[0])); gem_error(buf); }
    if (args[1].type != VAL_INT) { gem_error("remove_at: index must be an integer"); }
    GemTable *t = args[0].table;
    int64_t idx = args[1].ival;
    if (idx < 0 || idx >= t->len) { gem_error("remove_at: index out of bounds"); }
    int pos = (int)idx;
    GemVal removed = t->vals[pos];
    memmove(&t->vals[pos], &t->vals[pos + 1], ((size_t)(t->len - pos - 1)) * sizeof(GemVal));
    t->len--;
    for (int i = pos; i < t->len; i++) {
        t->keys[i] = gem_int(i);
    }
    return removed;
}
