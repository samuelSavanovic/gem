/*
 * gem_builtins_core.c — Core builtins: print, error, len, type, conversions,
 *                        pcall, eprint, exit, argv, getenv, input, make_ref.
 */

#include "gem.h"
#include <errno.h>

/* ─── Value formatting ───
 *
 * Single shared repr used by print/eprint/to_string and string interpolation.
 * Tables previously rendered as `<table:N>` which made print debugging useless;
 * now they recurse into a `{k: v, ...}` (or `[v1, v2, ...]` for dense int-keyed)
 * form with cycle detection and depth/breadth caps.
 */

#define GEM_FMT_MAX_DEPTH 8
#define GEM_FMT_MAX_SEEN 32
#define GEM_FMT_MAX_ENTRIES 64  /* per-table cap; overflow renders as `, ...` */

typedef struct {
    GemTable *stack[GEM_FMT_MAX_SEEN];
    int len;
} GemFmtSeen;

static void fmt_buf_grow(GemBuffer *b, int need) {
    while (b->len + need >= b->cap) {
        int nc = b->cap * 2;
        if (nc < b->len + need + 1) nc = b->len + need + 1;
        char *nd = (char *)gem_alloc(nc);
        memcpy(nd, b->data, b->len);
        b->data = nd;
        b->cap = nc;
    }
}

static void fmt_buf_appendn(GemBuffer *b, const char *s, int n) {
    fmt_buf_grow(b, n);
    memcpy(b->data + b->len, s, n);
    b->len += n;
}

static void fmt_buf_append(GemBuffer *b, const char *s) {
    fmt_buf_appendn(b, s, (int)strlen(s));
}

/* Conservative bare-key check: identifier-shaped (NAME ::= [A-Za-z_][A-Za-z0-9_]*).
 * Anything else gets quoted to keep output unambiguous. */
static int fmt_is_bare_key(const char *s) {
    if (!s || !*s) return 0;
    unsigned char c = (unsigned char)*s;
    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')) return 0;
    for (const char *p = s + 1; *p; p++) {
        unsigned char d = (unsigned char)*p;
        if (!((d >= 'a' && d <= 'z') || (d >= 'A' && d <= 'Z') ||
              (d >= '0' && d <= '9') || d == '_')) return 0;
    }
    return 1;
}

static void fmt_quoted_string(GemBuffer *out, const char *s) {
    fmt_buf_append(out, "\"");
    for (const char *p = s; *p; p++) {
        char c = *p;
        switch (c) {
            case '"':  fmt_buf_append(out, "\\\""); break;
            case '\\': fmt_buf_append(out, "\\\\"); break;
            case '\n': fmt_buf_append(out, "\\n"); break;
            case '\t': fmt_buf_append(out, "\\t"); break;
            case '\r': fmt_buf_append(out, "\\r"); break;
            default:   fmt_buf_appendn(out, &c, 1); break;
        }
    }
    fmt_buf_append(out, "\"");
}

static void fmt_value(GemVal v, GemBuffer *out, GemFmtSeen *seen, int depth, int as_repr) {
    char tmp[64];
    int n;
    switch (v.type) {
        case VAL_NIL:   fmt_buf_append(out, "nil"); return;
        case VAL_BOOL:  fmt_buf_append(out, v.bval ? "true" : "false"); return;
        case VAL_INT:
            n = snprintf(tmp, sizeof(tmp), "%lld", (long long)v.ival);
            fmt_buf_appendn(out, tmp, n); return;
        case VAL_FLOAT:
            n = snprintf(tmp, sizeof(tmp), "%g", v.fval);
            fmt_buf_appendn(out, tmp, n); return;
        case VAL_STRING:
            if (as_repr) fmt_quoted_string(out, v.sval ? v.sval : "");
            else fmt_buf_append(out, v.sval ? v.sval : "");
            return;
        case VAL_FN:    fmt_buf_append(out, "<fn>"); return;
        case VAL_BUFFER:
            n = snprintf(tmp, sizeof(tmp), "<buffer:%d>", v.buffer ? v.buffer->len : 0);
            fmt_buf_appendn(out, tmp, n); return;
        case VAL_REF:
            n = snprintf(tmp, sizeof(tmp), "#Ref<%lld>", (long long)v.rval);
            fmt_buf_appendn(out, tmp, n); return;
        case VAL_TABLE: {
            GemTable *t = v.table;
            if (!t) { fmt_buf_append(out, "{}"); return; }
            for (int i = 0; i < seen->len; i++) {
                if (seen->stack[i] == t) { fmt_buf_append(out, "<cycle>"); return; }
            }
            if (depth >= GEM_FMT_MAX_DEPTH || seen->len >= GEM_FMT_MAX_SEEN) {
                fmt_buf_append(out, "..."); return;
            }
            seen->stack[seen->len++] = t;

            int is_array = (t->len > 0);
            for (int i = 0; i < t->len; i++) {
                if (t->keys[i].type != VAL_INT || t->keys[i].ival != (int64_t)i) {
                    is_array = 0; break;
                }
            }

            int limit = t->len < GEM_FMT_MAX_ENTRIES ? t->len : GEM_FMT_MAX_ENTRIES;
            if (is_array) {
                fmt_buf_append(out, "[");
                for (int i = 0; i < limit; i++) {
                    if (i > 0) fmt_buf_append(out, ", ");
                    fmt_value(t->vals[i], out, seen, depth + 1, 1);
                }
                if (t->len > limit) fmt_buf_append(out, ", ...");
                fmt_buf_append(out, "]");
            } else {
                fmt_buf_append(out, "{");
                for (int i = 0; i < limit; i++) {
                    if (i > 0) fmt_buf_append(out, ", ");
                    GemVal k = t->keys[i];
                    if (k.type == VAL_STRING && k.sval && fmt_is_bare_key(k.sval)) {
                        fmt_buf_append(out, k.sval);
                    } else {
                        fmt_value(k, out, seen, depth + 1, 1);
                    }
                    fmt_buf_append(out, ": ");
                    fmt_value(t->vals[i], out, seen, depth + 1, 1);
                }
                if (t->len > limit) fmt_buf_append(out, ", ...");
                fmt_buf_append(out, "}");
            }
            seen->len--;
            return;
        }
    }
}

void gem_format_value_to_buf(GemVal v, GemBuffer *out, int as_repr) {
    GemFmtSeen seen; seen.len = 0;
    fmt_value(v, out, &seen, 0, as_repr);
}

GemVal gem_format_value_string(GemVal v) {
    GemBuffer b;
    b.cap = 64;
    b.len = 0;
    b.data = (char *)gem_alloc(b.cap);
    GemFmtSeen seen; seen.len = 0;
    fmt_value(v, &b, &seen, 0, 0);
    char *s = (char *)gem_alloc(b.len + 1);
    memcpy(s, b.data, b.len);
    s[b.len] = '\0';
    GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = s; r.slen = b.len;
    return r;
}

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
            case VAL_TABLE: {
                GemVal s = gem_format_value_string(v);
                fputs(s.sval, stdout);
                break;
            }
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
    if (v.type == VAL_STRING) return gem_int((int64_t)v.slen);
    if (v.type == VAL_TABLE) return gem_int((int64_t)v.table->len);
    if (v.type == VAL_BUFFER) return gem_int((int64_t)v.buffer->len);
    { char buf[128]; snprintf(buf, sizeof(buf), "len: expected string, table, or buffer, got %s", gem_type_str(v)); gem_error(buf); }
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
        case VAL_TABLE: return gem_format_value_string(v);
        case VAL_BUFFER: {
            GemBuffer *b = v.buffer;
            char *s = (char *)gem_alloc(b->len + 1);
            memcpy(s, b->data, b->len);
            s[b->len] = '\0';
            GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = s; r.slen = b->len;
            return r;
        }
        case VAL_REF: snprintf(buf, sizeof(buf), "#Ref<%lld>", (long long)v.rval); return gem_string(buf);
    }
    return gem_string("");
}

/* ─── Built-in: to_int / to_float ─── */

GemVal gem_to_int_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("to_int: expected 1 argument"); }
    GemVal v = args[0];
    if (v.type == VAL_INT) return v;
    if (v.type == VAL_FLOAT) return gem_int((int64_t)v.fval);
    if (v.type == VAL_BOOL) return gem_int(v.bval ? 1 : 0);
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
    if (v.type == VAL_BOOL) return gem_float(v.bval ? 1.0 : 0.0);
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

/* ─── Built-in: eprint (print to stderr) ─── */

GemVal gem_eprint_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    for (int i = 0; i < argc; i++) {
        if (i > 0) fprintf(stderr, " ");
        GemVal v = args[i];
        switch (v.type) {
            case VAL_NIL: fprintf(stderr, "nil"); break;
            case VAL_BOOL: fprintf(stderr, "%s", v.bval ? "true" : "false"); break;
            case VAL_INT: fprintf(stderr, "%lld", (long long)v.ival); break;
            case VAL_FLOAT: fprintf(stderr, "%g", v.fval); break;
            case VAL_STRING: fprintf(stderr, "%s", v.sval); break;
            case VAL_FN: fprintf(stderr, "<fn>"); break;
            case VAL_TABLE: {
                GemVal s = gem_format_value_string(v);
                fputs(s.sval, stderr);
                break;
            }
            case VAL_BUFFER: fprintf(stderr, "<buffer:%d>", v.buffer->len); break;
            case VAL_REF: fprintf(stderr, "#Ref<%lld>", (long long)v.rval); break;
        }
    }
    fprintf(stderr, "\n");
    return GEM_NIL;
}

/* ─── Built-in: exit (exit with code) ─── */

GemVal gem_exit_process_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    int code = 0;
    if (argc > 0 && args[0].type == VAL_INT) code = (int)args[0].ival;
    exit(code);
    return GEM_NIL;
}

/* ─── Built-in: argv (return command-line args as array) ─── */

GemVal gem_argv_fn(void *_env, GemVal *args, int argc) {
    (void)_env; (void)args; (void)argc;
    GemVal result = gem_table_new();
    for (int i = 0; i < gem_stored_argc; i++) {
        gem_table_set(result, gem_int(i), gem_string(gem_stored_argv[i]));
    }
    return result;
}

/* ─── Built-in: getenv ─── */

GemVal gem_getenv_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) { char buf[128]; snprintf(buf, sizeof(buf), "getenv: expected string, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    const char *val = getenv(args[0].sval);
    if (!val) return GEM_NIL;
    return gem_string(val);
}

/* ─── Built-in: input (read line from stdin) ─── */

GemVal gem_input_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc > 0 && args[0].type == VAL_STRING) {
        printf("%s", args[0].sval);
        fflush(stdout);
    }
    char buf[4096];
    if (!fgets(buf, sizeof(buf), stdin)) return GEM_NIL;
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[--len] = '\0';
    if (len > 0 && buf[len - 1] == '\r') buf[--len] = '\0';
    return gem_string(buf);
}

/* ─── Built-in: make_ref ─── */

GemVal gem_make_ref_builtin(void *_env, GemVal *args, int argc) {
    (void)_env; (void)args; (void)argc;
    return gem_make_ref();
}
