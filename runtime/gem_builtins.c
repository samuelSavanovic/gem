/*
 * gem_builtins.c — Built-in function wrappers (print, error, len, type, etc.).
 */

#include "stb_ds.h"
#include "gem.h"
#include <errno.h>

/* ─── Built-in: print ─── */

GemVal gem_print(void *_env, GemVal *args, int argc) {
    (void)_env;
    for (int i = 0; i < argc; i++) {
        if (i > 0) printf(" ");
        GemVal v = args[i];
        switch (v.type) {
            case VAL_NIL: printf("nil"); break;
            case VAL_BOOL: printf("%s", v.bval ? "true" : "false"); break;
            case VAL_INT: printf("%lld", (long long)v.ival); break;
            case VAL_FLOAT: printf("%g", v.fval); break;
            case VAL_STRING: printf("%s", v.sval); break;
            case VAL_FN: printf("<fn>"); break;
            case VAL_TABLE: printf("<table:%d>", v.table->len); break;
            case VAL_BUFFER: printf("<buffer:%d>", v.buffer->len); break;
            case VAL_REF: printf("#Ref<%lld>", (long long)v.rval); break;
        }
    }
    printf("\n");
    return GEM_NIL;
}

/* ─── Built-in: error ─── */

GemVal gem_error_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc > 0 && args[0].type == VAL_STRING) {
        gem_raise_error(args[0].sval);
    } else {
        gem_raise_error("error");
    }
    return GEM_NIL;
}

/* ─── Built-in: error with location ─── */

GemVal gem_error_at_fn(const char *file, int line, GemVal *args, int argc) {
    /* If pcall or coroutine isolation will catch it, pass just the user's message */
    int will_catch = 0;
    if (gem_current_pid >= 0 && gem_current_pid < GEM_MAX_PROCS) {
        GemProcess *_proc = &gem_proc_table[gem_current_pid];
        if (_proc->state != GEM_PROC_FREE && _proc->state != GEM_PROC_DEAD)
            will_catch = 1;
    } else if (gem_pcall_depth > 0) {
        will_catch = 1;
    }
    if (will_catch) {
        if (argc > 0 && args[0].type == VAL_STRING) {
            gem_raise_error(args[0].sval);
        } else {
            gem_raise_error("error");
        }
    } else {
        /* No pcall, no coroutine — fatal error with file:line prefix */
        if (argc > 0 && args[0].type == VAL_STRING) {
            fprintf(stderr, "%s:%d: error: %s\n", file, line, args[0].sval);
        } else {
            fprintf(stderr, "%s:%d: error\n", file, line);
        }
        gem_print_stack_trace();
        exit(1);
    }
    return GEM_NIL;
}

/* ─── Built-in: len ─── */

GemVal gem_len_val(GemVal v) {
    if (v.type == VAL_STRING) return gem_int((int64_t)strlen(v.sval));
    if (v.type == VAL_TABLE) return gem_int((int64_t)v.table->len);
    { char buf[128]; snprintf(buf, sizeof(buf), "len: expected string or table, got %s", gem_type_str(v)); gem_error(buf); }
    return GEM_NIL;
}

GemVal gem_len_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("len: expected 1 argument"); }
    return gem_len_val(args[0]);
}

/* ─── Built-in: type ─── */

GemVal gem_type_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) return gem_string("nil");
    switch (args[0].type) {
        case VAL_NIL: return gem_string("nil");
        case VAL_BOOL: return gem_string("bool");
        case VAL_INT: return gem_string("int");
        case VAL_FLOAT: return gem_string("float");
        case VAL_STRING: return gem_string("string");
        case VAL_FN: return gem_string("fn");
        case VAL_TABLE: return gem_string("table");
        case VAL_BUFFER: return gem_string("buffer");
        case VAL_REF: return gem_string("ref");
    }
    return gem_string("unknown");
}

/* ─── Built-in: to_string ─── */

GemVal gem_to_string_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) return gem_string("");
    GemVal v = args[0];
    char buf[64];
    switch (v.type) {
        case VAL_NIL: return gem_string("nil");
        case VAL_BOOL: return gem_string(v.bval ? "true" : "false");
        case VAL_INT: snprintf(buf, sizeof(buf), "%lld", (long long)v.ival); return gem_string(buf);
        case VAL_FLOAT: snprintf(buf, sizeof(buf), "%g", v.fval); return gem_string(buf);
        case VAL_STRING: return v;
        case VAL_FN: return gem_string("<fn>");
        case VAL_TABLE: snprintf(buf, sizeof(buf), "<table:%d>", v.table->len); return gem_string(buf);
        case VAL_BUFFER: snprintf(buf, sizeof(buf), "<buffer:%d>", v.buffer->len); return gem_string(buf);
        case VAL_REF: snprintf(buf, sizeof(buf), "#Ref<%lld>", (long long)v.rval); return gem_string(buf);
    }
    return gem_string("");
}

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

/* ─── Built-in: str_replace ─── */

GemVal gem_str_replace_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 3) { gem_error("str_replace: expected 3 arguments"); }
    if (args[0].type != VAL_STRING || args[1].type != VAL_STRING || args[2].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "str_replace: all arguments must be strings, got %s, %s, %s", gem_type_str(args[0]), gem_type_str(args[1]), gem_type_str(args[2])); gem_error(buf);
    }
    const char *s = args[0].sval;
    const char *old = args[1].sval;
    const char *new_s = args[2].sval;
    size_t s_len = strlen(s);
    size_t old_len = strlen(old);
    size_t new_len = strlen(new_s);

    if (old_len == 0) return args[0]; /* empty pattern — return original */

    /* Count occurrences to pre-allocate */
    int count = 0;
    const char *p = s;
    while ((p = strstr(p, old)) != NULL) {
        count++;
        p += old_len;
    }
    if (count == 0) return args[0];

    size_t result_len = s_len + (size_t)count * (new_len - old_len);
    char *result = (char *)GC_MALLOC_ATOMIC(result_len + 1);
    char *dst = result;
    p = s;
    while (*p) {
        if (strncmp(p, old, old_len) == 0) {
            memcpy(dst, new_s, new_len);
            dst += new_len;
            p += old_len;
        } else {
            *dst++ = *p++;
        }
    }
    *dst = '\0';
    GemVal r; r.type = VAL_STRING; r.sval = result;
    return r;
}

/* ─── Built-in: substr ─── */

GemVal gem_substr_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2) { gem_error("substr: expected 2-3 arguments"); }
    if (args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        char buf[128]; snprintf(buf, sizeof(buf), "substr: expected (string, int[, int]), got (%s, %s)", gem_type_str(args[0]), gem_type_str(args[1])); gem_error(buf);
    }
    const char *s = args[0].sval;
    int64_t start = args[1].ival;
    int64_t slen = (int64_t)strlen(s);

    if (start < 0) start = 0;
    if (start >= slen) return gem_string("");

    int64_t count;
    if (argc >= 3 && args[2].type == VAL_INT) {
        count = args[2].ival;
        if (count < 0) count = 0;
    } else {
        count = slen - start;
    }
    if (start + count > slen) count = slen - start;

    char *buf = (char *)GC_MALLOC_ATOMIC((size_t)count + 1);
    memcpy(buf, s + start, (size_t)count);
    buf[count] = '\0';
    GemVal r; r.type = VAL_STRING; r.sval = buf;
    return r;
}

/* ─── Built-in: chr / ord ─── */

GemVal gem_chr_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) { char buf[128]; snprintf(buf, sizeof(buf), "chr: expected int argument, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    char buf[2];
    buf[0] = (char)(args[0].ival & 0xFF);
    buf[1] = '\0';
    return gem_string(buf);
}

GemVal gem_ord_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) { char buf[128]; snprintf(buf, sizeof(buf), "ord: expected string argument, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    if (argc >= 2) {
        /* ord(s, i) — direct byte access by index, no allocation */
        if (args[1].type != VAL_INT) { gem_error("ord: index must be an integer"); }
        int64_t idx = args[1].ival;
        int64_t slen = (int64_t)strlen(args[0].sval);
        if (idx < 0 || idx >= slen) { gem_error("ord: index out of bounds"); }
        return gem_int((int64_t)(unsigned char)args[0].sval[idx]);
    }
    if (strlen(args[0].sval) == 0) { gem_error("ord: empty string"); }
    return gem_int((int64_t)(unsigned char)args[0].sval[0]);
}

/* ─── Built-in: to_int / to_float ─── */

GemVal gem_to_int_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("to_int: expected 1 argument"); }
    GemVal v = args[0];
    if (v.type == VAL_INT) return v;
    if (v.type == VAL_FLOAT) return gem_int((int64_t)v.fval);
    if (v.type == VAL_BOOL) return gem_int(v.ival ? 1 : 0);
    if (v.type == VAL_STRING) {
        const char *s = v.sval;
        char *end;
        errno = 0;
        long long val = strtoll(s, &end, 10);
        if (end == s || *end != '\0' || errno == ERANGE) {
            char buf[256];
            snprintf(buf, sizeof(buf), "to_int: cannot convert \"%s\" to int", s);
            gem_error(buf);
        }
        return gem_int((int64_t)val);
    }
    char buf[128];
    snprintf(buf, sizeof(buf), "to_int: cannot convert %s to int", gem_type_str(v));
    gem_error(buf);
    return GEM_NIL;
}

GemVal gem_to_float_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("to_float: expected 1 argument"); }
    GemVal v = args[0];
    if (v.type == VAL_FLOAT) return v;
    if (v.type == VAL_INT) return gem_float((double)v.ival);
    if (v.type == VAL_BOOL) return gem_float(v.ival ? 1.0 : 0.0);
    if (v.type == VAL_STRING) {
        const char *s = v.sval;
        char *end;
        errno = 0;
        double val = strtod(s, &end);
        if (end == s || *end != '\0' || errno == ERANGE) {
            char buf[256];
            snprintf(buf, sizeof(buf), "to_float: cannot convert \"%s\" to float", s);
            gem_error(buf);
        }
        return gem_float(val);
    }
    char buf[128];
    snprintf(buf, sizeof(buf), "to_float: cannot convert %s to float", gem_type_str(v));
    gem_error(buf);
    return GEM_NIL;
}

/* ─── String builder (buf_new / buf_push / buf_str) ─── */

GemVal gem_buf_new_fn(void *_env, GemVal *args, int argc) {
    (void)_env; (void)args; (void)argc;
    GemBuffer *b = (GemBuffer *)GC_MALLOC(sizeof(GemBuffer));
    b->cap = 64;
    b->len = 0;
    b->data = (char *)GC_MALLOC_ATOMIC(b->cap);
    GemVal r;
    r.type = VAL_BUFFER;
    r.buffer = b;
    return r;
}

GemVal gem_buf_push_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_BUFFER) { char buf[128]; snprintf(buf, sizeof(buf), "buf_push: expected buffer as first argument, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    GemBuffer *b = args[0].buffer;
    /* Coerce second argument to string */
    const char *s;
    char tmp[64];
    switch (args[1].type) {
        case VAL_STRING: s = args[1].sval; break;
        case VAL_INT: snprintf(tmp, sizeof(tmp), "%lld", (long long)args[1].ival); s = tmp; break;
        case VAL_FLOAT: snprintf(tmp, sizeof(tmp), "%g", args[1].fval); s = tmp; break;
        case VAL_BOOL: s = args[1].bval ? "true" : "false"; break;
        case VAL_NIL: s = "nil"; break;
        default: s = ""; break;
    }
    int slen = (int)strlen(s);
    /* Grow buffer if needed */
    while (b->len + slen >= b->cap) {
        int new_cap = b->cap * 2;
        char *new_data = (char *)GC_MALLOC_ATOMIC(new_cap);
        memcpy(new_data, b->data, b->len);
        b->data = new_data;
        b->cap = new_cap;
    }
    memcpy(b->data + b->len, s, slen);
    b->len += slen;
    return args[0]; /* return buffer for chaining */
}

GemVal gem_buf_str_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_BUFFER) { char buf[128]; snprintf(buf, sizeof(buf), "buf_str: expected buffer, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    GemBuffer *b = args[0].buffer;
    char *s = (char *)GC_MALLOC_ATOMIC(b->len + 1);
    memcpy(s, b->data, b->len);
    s[b->len] = '\0';
    GemVal r;
    r.type = VAL_STRING;
    r.sval = s;
    return r;
}

/* ─── Built-in: make_ref ─── */

GemVal gem_make_ref_builtin(void *_env, GemVal *args, int argc) {
    (void)_env; (void)args; (void)argc;
    return gem_make_ref();
}

/* ─── Built-in: read_file / write_file ─── */

GemVal gem_read_file_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "read_file: expected string path, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    const char *path = args[0].sval;
    FILE *f = fopen(path, "rb");
    if (!f) {
        char buf[512]; snprintf(buf, sizeof(buf), "read_file: cannot open '%s'", path); gem_error(buf);
    }
    fseek(f, 0, SEEK_END);
    long flen = ftell(f);
    rewind(f);
    char *data = (char *)GC_MALLOC_ATOMIC((size_t)flen + 1);
    size_t nread = fread(data, 1, (size_t)flen, f);
    data[nread] = '\0';
    fclose(f);
    GemVal r; r.type = VAL_STRING; r.sval = data;
    return r;
}

GemVal gem_write_file_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "write_file: expected (string, string), got (%s, %s)", argc < 1 ? "nothing" : gem_type_str(args[0]), argc < 2 ? "nothing" : gem_type_str(args[1])); gem_error(buf);
    }
    const char *path = args[0].sval;
    const char *content = args[1].sval;
    FILE *f = fopen(path, "wb");
    if (!f) {
        char buf[512]; snprintf(buf, sizeof(buf), "write_file: cannot open '%s' for writing", path); gem_error(buf);
    }
    size_t len = strlen(content);
    size_t nwritten = fwrite(content, 1, len, f);
    fclose(f);
    if (nwritten != len) {
        char buf[512]; snprintf(buf, sizeof(buf), "write_file: write failed for '%s'", path); gem_error(buf);
    }
    return GEM_NIL;
}

/* ─── Built-in: pcall ─── */

GemVal gem_pcall_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_FN) {
        gem_error("pcall: expected function argument");
    }

    GemVal fn = args[0];

    /* Use per-process pcall stack if inside a process, else global */
    GemPcallFrame *stack;
    int *depth_ptr;
    if (gem_current_pid >= 0 && gem_current_pid < GEM_MAX_PROCS) {
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        stack = proc->pcall_stack;
        depth_ptr = &proc->pcall_depth;
    } else {
        stack = gem_pcall_stack;
        depth_ptr = &gem_pcall_depth;
    }

    if (*depth_ptr >= GEM_MAX_PCALL_DEPTH) {
        gem_error("pcall: too many nested pcall levels");
    }

    int frame_idx = *depth_ptr;
    stack[frame_idx].saved_call_depth = gem_call_depth;
    (*depth_ptr)++;

    volatile GemVal result = gem_table_new();

    if (setjmp(stack[frame_idx].buf) == 0) {
        /* Normal path — call the function */
        GemVal value = fn.fn(fn.env, NULL, 0);
        (*depth_ptr)--;
        gem_table_set(result, gem_string("ok"), gem_bool(1));
        gem_table_set(result, gem_string("value"), value);
    } else {
        /* Error path — longjmp landed here */
        gem_table_set(result, gem_string("ok"), gem_bool(0));
        gem_table_set(result, gem_string("error"), gem_string(stack[frame_idx].error_msg));
        gem_table_set(result, gem_string("stack"), stack[frame_idx].stack_snapshot);
    }

    return result;
}
