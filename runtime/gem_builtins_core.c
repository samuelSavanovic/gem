/*
 * gem_builtins_core.c — Core builtins: print, error, len, type, conversions,
 *                        pcall, eprint, exit, argv, getenv, input, make_ref.
 */

#include "gem.h"
#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include <math.h>

/* ─── Value formatting ───
 *
 * Single shared repr used by print/eprint/to_string and string interpolation.
 * Tables render as `{k: v, ...}` (or `[v1, v2, ...]` for dense int-keyed),
 * with cycle detection and depth/breadth caps.
 */

#define GEM_FMT_MAX_DEPTH 8
#define GEM_FMT_MAX_SEEN 32
#define GEM_FMT_MAX_ENTRIES 64  /* per-table cap; overflow renders as `, ...` */

typedef struct {
    GemTable *stack[GEM_FMT_MAX_SEEN];
    int len;
} GemFmtSeen;

static void fmt_buf_appendn(GemBuffer *b, const char *s, int n) {
    gem_buffer_reserve(b, (size_t)n, "to_string");
    memcpy(b->data + b->len, s, n);
    b->len += n;
}

static void fmt_buf_append(GemBuffer *b, const char *s) {
    fmt_buf_appendn(b, s, (int)strlen(s));
}

/* Conservative bare-key check: identifier-shaped (NAME ::= [A-Za-z_][A-Za-z0-9_]*).
 * Anything else gets quoted to keep output unambiguous. */
static int fmt_is_bare_key(const char *s, int64_t len) {
    if (!s || len == 0) return 0;
    unsigned char c = (unsigned char)*s;
    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')) return 0;
    for (int64_t i = 1; i < len; i++) {
        unsigned char d = (unsigned char)s[i];
        if (!((d >= 'a' && d <= 'z') || (d >= 'A' && d <= 'Z') ||
              (d >= '0' && d <= '9') || d == '_')) return 0;
    }
    return 1;
}

/* Strings are written by their `slen` bytes: an embedded NUL is kept (as
   `\0` when quoted), not taken for the end. */
static void fmt_quoted_string(GemBuffer *out, const char *s, int64_t len) {
    fmt_buf_append(out, "\"");
    for (int64_t i = 0; i < len; i++) {
        char c = s[i];
        switch (c) {
            case '"':  fmt_buf_append(out, "\\\""); break;
            case '\\': fmt_buf_append(out, "\\\\"); break;
            case '\n': fmt_buf_append(out, "\\n"); break;
            case '\t': fmt_buf_append(out, "\\t"); break;
            case '\r': fmt_buf_append(out, "\\r"); break;
            case '\0': fmt_buf_append(out, "\\0"); break;
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
            n = gem_format_float(v.fval, tmp);
            fmt_buf_appendn(out, tmp, n); return;
        case VAL_STRING:
            if (!v.sval) { if (as_repr) fmt_buf_append(out, "\"\""); return; }
            if (as_repr) fmt_quoted_string(out, v.sval, v.slen);
            else fmt_buf_appendn(out, v.sval, (int)v.slen);
            return;
        case VAL_FN:    fmt_buf_append(out, "<fn>"); return;
        case VAL_LAZY:  return;
        case VAL_BUFFER:
            n = snprintf(tmp, sizeof(tmp), "<buffer:%d>", v.buffer ? v.buffer->len : 0);
            fmt_buf_appendn(out, tmp, n); return;
        case VAL_REF:
            n = snprintf(tmp, sizeof(tmp), "#Ref<%lld>", (long long)v.rval);
            fmt_buf_appendn(out, tmp, n); return;
        case VAL_RESOURCE:
            n = snprintf(tmp, sizeof(tmp), "#%s<%lld>", gem_res_print_name(v.res_kind), (long long)v.res_id);
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
                    if (k.type == VAL_STRING && k.sval && fmt_is_bare_key(k.sval, k.slen)) {
                        fmt_buf_appendn(out, k.sval, (int)k.slen);
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
            case VAL_FLOAT: { char fb[GEM_FLOAT_BUF]; gem_format_float(v.fval, fb); fputs(fb, stdout); break; }
            case VAL_STRING: fwrite(v.sval, 1, (size_t)v.slen, stdout); break;
            case VAL_FN: printf("<fn>"); break;
            case VAL_TABLE: {
                GemVal s = gem_format_value_string(v);
                fwrite(s.sval, 1, (size_t)s.slen, stdout);
                break;
            }
            case VAL_BUFFER: printf("<buffer:%d>", v.buffer->len); break;
            case VAL_REF: printf("#Ref<%lld>", (long long)v.rval); break;
            case VAL_RESOURCE: printf("#%s<%lld>", gem_res_print_name(v.res_kind), (long long)v.res_id); break;
            case VAL_LAZY: break;
        }
    }
    printf("\n");
    return GEM_NIL;
}

/* ─── Built-in: error ─── */

/* The message `error(v)` raises: a string as is, any other value as
   `to_string` shows it, and "error" for no argument. */
static const char *gem_error_message(GemVal *args, int argc) {
    if (argc == 0) return "error";
    if (args[0].type == VAL_STRING) return args[0].sval;
    return gem_to_string_fn(NULL, args, 1).sval;
}

GemVal gem_error_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    gem_raise_error(gem_error_message(args, argc));
    return GEM_NIL;
}

/* ─── Internal: a `match` no arm of which matched ─── */

/* lower() gives a `match` with no `else` an else branch calling
   `__match_fail(<target>)`: it raises "no match for <value>", the value
   as print shows it nested (strings quoted), cut at 200 bytes. */
GemVal gem_match_fail_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    GemBuffer b;
    b.cap = 64;
    b.len = 0;
    b.data = (char *)gem_alloc(b.cap);
    gem_format_value_to_buf(argc > 0 ? args[0] : GEM_NIL, &b, 1);
    char msg[256];
    if (b.len > 200) snprintf(msg, sizeof(msg), "no match for %.200s...", b.data);
    else snprintf(msg, sizeof(msg), "no match for %.*s", b.len, b.data);
    gem_raise_error(msg);
    return GEM_NIL;
}

/* ─── Built-in: error with location ─── */

GemVal gem_error_at_fn(const char *file, int line, GemVal *args, int argc) {
    /* If pcall or coroutine isolation will catch it, pass just the user's message */
    int will_catch = 0;
    if (gem_current_pid >= 0 && gem_current_pid < gem_proc_hwm) {
        GemProcess *_proc = &gem_proc_table[gem_current_pid];
        if (_proc->state != GEM_PROC_FREE && _proc->state != GEM_PROC_DEAD)
            will_catch = 1;
    } else if (gem_pcall_depth > 0) {
        will_catch = 1;
    }
    if (will_catch) {
        gem_raise_error(gem_error_message(args, argc));
    } else {
        /* No pcall, no coroutine — fatal error with file:line prefix */
        if (argc > 0) {
            fprintf(stderr, "%s:%d: error: %s\n", file, line, gem_error_message(args, argc));
        } else {
            fprintf(stderr, "%s:%d: error\n", file, line);
        }
        gem_print_stack_trace();
        exit(1);
    }
    return GEM_NIL;
}

/* ─── Callable check: error cleanly when calling a non-function value ─── */

void gem_check_callable(GemVal v, const char *file, int line) {
    if (v.type == VAL_FN) return;
    char buf[128];
    snprintf(buf, sizeof(buf), "attempt to call %s value", gem_type_str(v));
    GemVal arg = gem_string(buf);
    gem_error_at_fn(file, line, &arg, 1);
}

/* The same for a call through a field (`t.f()`), naming the field. */
void gem_check_callable_field(GemVal v, const char *field, const char *file, int line) {
    if (v.type == VAL_FN) return;
    char buf[160];
    snprintf(buf, sizeof(buf), "attempt to call %s value (field %.64s)", gem_type_str(v), field);
    GemVal arg = gem_string(buf);
    gem_error_at_fn(file, line, &arg, 1);
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
    if (argc < 1) return GEM_STR_LIT("nil", 3);
    switch (args[0].type) {
        case VAL_NIL: return GEM_STR_LIT("nil", 3);
        case VAL_BOOL: return GEM_STR_LIT("bool", 4);
        case VAL_INT: return GEM_STR_LIT("int", 3);
        case VAL_FLOAT: return GEM_STR_LIT("float", 5);
        case VAL_STRING: return GEM_STR_LIT("string", 6);
        case VAL_FN: return GEM_STR_LIT("fn", 2);
        case VAL_TABLE: return GEM_STR_LIT("table", 5);
        case VAL_BUFFER: return GEM_STR_LIT("buffer", 6);
        case VAL_REF: return GEM_STR_LIT("ref", 3);
        case VAL_RESOURCE: {
            const char *k = gem_res_type_name(args[0].res_kind);
            return GEM_STR_LIT(k, (int)strlen(k));
        }
        case VAL_LAZY: break;
    }
    return GEM_STR_LIT("unknown", 7);
}

/* ─── Built-in: to_string ─── */

GemVal gem_to_string_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) return GEM_STR_LIT("", 0);
    GemVal v = args[0];
    char buf[64];
    switch (v.type) {
        case VAL_NIL: return GEM_STR_LIT("nil", 3);
        case VAL_BOOL: return v.bval ? GEM_STR_LIT("true", 4) : GEM_STR_LIT("false", 5);
        case VAL_INT: snprintf(buf, sizeof(buf), "%lld", (long long)v.ival); return gem_string(buf);
        case VAL_FLOAT: gem_format_float(v.fval, buf); return gem_string(buf);
        case VAL_STRING: return v;
        case VAL_FN: return GEM_STR_LIT("<fn>", 4);
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
        case VAL_RESOURCE:
            snprintf(buf, sizeof(buf), "#%s<%lld>", gem_res_print_name(v.res_kind), (long long)v.res_id);
            return gem_string(buf);
        case VAL_LAZY: break;
    }
    return GEM_STR_LIT("", 0);
}

/* ─── Built-in: to_int / to_float ─── */

GemVal gem_to_int_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1) { gem_error("to_int: expected 1 argument"); }
    GemVal v = args[0];
    if (v.type == VAL_INT) return v;
    if (v.type == VAL_FLOAT) return gem_float_to_int(v.fval, "to_int");
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
        /* ERANGE on a nonzero finite result is a subnormal (gradual
         * underflow), which is exact enough to keep: to_string of a
         * subnormal must read back. Overflow and underflow to zero fail. */
        int range_err = errno == ERANGE && (val == 0.0 || isinf(val));
        if (end == s || *end != '\0' || range_err) {
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
    if (gem_current_pid >= 0 && gem_current_pid < gem_proc_hwm) {
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
            case VAL_FLOAT: { char fb[GEM_FLOAT_BUF]; gem_format_float(v.fval, fb); fputs(fb, stderr); break; }
            case VAL_STRING: fwrite(v.sval, 1, (size_t)v.slen, stderr); break;
            case VAL_FN: fprintf(stderr, "<fn>"); break;
            case VAL_TABLE: {
                GemVal s = gem_format_value_string(v);
                fwrite(s.sval, 1, (size_t)s.slen, stderr);
                break;
            }
            case VAL_BUFFER: fprintf(stderr, "<buffer:%d>", v.buffer->len); break;
            case VAL_REF: fprintf(stderr, "#Ref<%lld>", (long long)v.rval); break;
            case VAL_RESOURCE: fprintf(stderr, "#%s<%lld>", gem_res_print_name(v.res_kind), (long long)v.res_id); break;
            case VAL_LAZY: break;
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

/* ─── The runtime's environment: getenv, setenv ───
 *
 * getenv and setenv work on a copy of the process environment the runtime
 * owns (gem_env, "NAME=VALUE" strings), made from environ when first used;
 * exec and exec_capture pass a snapshot of it to their children
 * (gem_env_snapshot). libc's environ is never changed after startup:
 * other threads read it (getaddrinfo on the resolver lane, sqlite and
 * extern blocking fns on the pool), and libc's setenv could move it under
 * them. gem_env_mu
 * guards gem_env, which pool threads snapshot. */

static pthread_mutex_t gem_env_mu = PTHREAD_MUTEX_INITIALIZER;
static char **gem_env = NULL;     /* malloc'd strings; NULL until first used */
static size_t gem_env_n = 0, gem_env_cap = 0;

extern char **environ;

/* Call with gem_env_mu held. */
static void gem_env_init_locked(void) {
    if (gem_env) return;
    size_t n = 0;
    while (environ && environ[n]) n++;
    gem_env_cap = n + 8;
    gem_env = (char **)malloc(gem_env_cap * sizeof(char *));
    for (size_t i = 0; i < n; i++) gem_env[i] = strdup(environ[i]);
    gem_env_n = n;
}

/* The index of `name` in gem_env, or -1. Call with gem_env_mu held. */
static long gem_env_find_locked(const char *name, size_t nlen) {
    for (size_t i = 0; i < gem_env_n; i++) {
        if (strncmp(gem_env[i], name, nlen) == 0 && gem_env[i][nlen] == '=') return (long)i;
    }
    return -1;
}

char **gem_env_snapshot(void) {
    pthread_mutex_lock(&gem_env_mu);
    gem_env_init_locked();
    size_t bytes = (gem_env_n + 1) * sizeof(char *);
    for (size_t i = 0; i < gem_env_n; i++) bytes += strlen(gem_env[i]) + 1;
    char **out = (char **)malloc(bytes);
    char *p = (char *)(out + gem_env_n + 1);
    for (size_t i = 0; i < gem_env_n; i++) {
        size_t len = strlen(gem_env[i]) + 1;
        memcpy(p, gem_env[i], len);
        out[i] = p;
        p += len;
    }
    out[gem_env_n] = NULL;
    pthread_mutex_unlock(&gem_env_mu);
    return out;
}

/* ─── Built-in: getenv ─── */

GemVal gem_getenv_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) { char buf[128]; snprintf(buf, sizeof(buf), "getenv: expected string, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf); }
    const char *name = args[0].sval;
    size_t nlen = strlen(name);
    if (nlen == 0 || strchr(name, '=') || (int64_t)nlen != args[0].slen) return GEM_NIL;
    char *val = NULL;
    pthread_mutex_lock(&gem_env_mu);
    gem_env_init_locked();
    long i = gem_env_find_locked(name, nlen);
    if (i >= 0) val = strdup(gem_env[i] + nlen + 1);
    pthread_mutex_unlock(&gem_env_mu);
    if (!val) return GEM_NIL;
    GemVal r = gem_string(val);
    free(val);
    return r;
}

/* ─── Built-in: setenv(name, value) ─── */
/* value nil removes the variable. */

GemVal gem_setenv_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || (args[1].type != VAL_STRING && args[1].type != VAL_NIL)) {
        char buf[160]; snprintf(buf, sizeof(buf), "setenv: expected (string, string or nil), got (%s, %s)", argc < 1 ? "nothing" : gem_type_str(args[0]), argc < 2 ? "nothing" : gem_type_str(args[1])); gem_error(buf);
    }
    const char *name = args[0].sval;
    size_t nlen = strlen(name);
    if (nlen == 0 || strchr(name, '=') || (int64_t)nlen != args[0].slen) {
        gem_error("setenv: the name must be non-empty, without '=' or a NUL byte");
    }
    char *entry = NULL;
    if (args[1].type != VAL_NIL) {
        if ((int64_t)strlen(args[1].sval) != args[1].slen) gem_error("setenv: the value contains a NUL byte");
        size_t vlen = (size_t)args[1].slen;
        entry = (char *)malloc(nlen + vlen + 2);
        memcpy(entry, name, nlen);
        entry[nlen] = '=';
        memcpy(entry + nlen + 1, args[1].sval, vlen + 1);
    }
    pthread_mutex_lock(&gem_env_mu);
    gem_env_init_locked();
    long i = gem_env_find_locked(name, nlen);
    if (i >= 0) {
        free(gem_env[i]);
        if (entry) gem_env[i] = entry;
        else gem_env[i] = gem_env[--gem_env_n];
    } else if (entry) {
        if (gem_env_n + 1 >= gem_env_cap) {
            gem_env_cap *= 2;
            gem_env = (char **)realloc(gem_env, gem_env_cap * sizeof(char *));
        }
        gem_env[gem_env_n++] = entry;
    }
    pthread_mutex_unlock(&gem_env_mu);
    return GEM_NIL;
}

/* ─── Built-in: input (read line from stdin) ─── */

GemVal gem_input_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc > 0 && args[0].type == VAL_STRING) {
        fwrite(args[0].sval, 1, (size_t)args[0].slen, stdout);
        fflush(stdout);
    }
    /* getline reads a line of any length and reports its byte count, so a
       NUL inside the line is kept. The buffer is reused across calls. */
    static char *line = NULL;
    static size_t cap = 0;
    ssize_t n = getline(&line, &cap, stdin);
    if (n < 0) return GEM_NIL;
    size_t len = (size_t)n;
    if (len > 0 && line[len - 1] == '\n') {
        len--;
        if (len > 0 && line[len - 1] == '\r') len--;
    }
    gem_strlen_check(len, "input");
    return gem_string_with_len(line, (int64_t)len);
}

/* ─── Built-in: write_stdout (write raw bytes + flush) ─── */
/* Binary-safe (uses slen, not strlen). No trailing newline. Returns once
 * every byte is written (waiting on a non-blocking stdout until it takes
 * more), so framed protocols (e.g. LSP over stdio) see complete frames; a
 * write error, such as a closed reader, drops the rest. It flushes what
 * print left in stdout's buffer and writes the string to fd 1 directly:
 * stdout is line-buffered (gem_init), and macOS's fwrite on a line-buffered
 * stream makes one write(2) per line. Returns nil. */

GemVal gem_write_stdout_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "write_stdout: expected string, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    fflush(stdout);
    const char *p = args[0].sval;
    size_t left = (size_t)args[0].slen;
    while (left > 0) {
        ssize_t w = write(STDOUT_FILENO, p, left);
        if (w < 0) {
            if (errno == EINTR) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                struct pollfd pfd = { .fd = STDOUT_FILENO, .events = POLLOUT };
                poll(&pfd, 1, -1);
                continue;
            }
            break;
        }
        p += w;
        left -= (size_t)w;
    }
    return GEM_NIL;
}

/* ─── Built-in: read_stdin (read exactly N bytes from stdin) ─── */
/* Binary-safe; intended for LSP-style framed I/O where the message body
 * length is known up-front from a header. Returns "" on EOF before any
 * bytes were read; returns a short string if EOF interrupts mid-read. */

GemVal gem_read_stdin_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type == VAL_NIL) {
        /* No count: everything up to the end of stdin. */
        size_t cap = 65536, len = 0;
        char *buf = (char *)malloc(cap);
        for (;;) {
            if (len == cap) {
                if (len > (size_t)GEM_MAX_STRLEN) { free(buf); gem_strlen_check(len, "read_stdin"); }
                cap *= 2;
                buf = (char *)realloc(buf, cap);
            }
            size_t got = fread(buf + len, 1, cap - len, stdin);
            if (got == 0) break;
            len += got;
        }
        if (len > (size_t)GEM_MAX_STRLEN) { free(buf); gem_strlen_check(len, "read_stdin"); }
        GemVal r = gem_string_with_len(buf, (int64_t)len);
        free(buf);
        return r;
    }
    if (args[0].type != VAL_INT) {
        char buf[128]; snprintf(buf, sizeof(buf), "read_stdin: expected int, got %s", gem_type_str(args[0])); gem_error(buf);
    }
    int64_t n = args[0].ival;
    if (n < 0) { gem_error("read_stdin: negative length"); }
    if (n == 0) { GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = gem_alloc(1); r.sval[0] = '\0'; r.slen = 0; return r; }
    gem_strlen_check((size_t)n, "read_stdin");
    char *data = (char *)gem_alloc((size_t)n + 1);
    size_t got = fread(data, 1, (size_t)n, stdin);
    data[got] = '\0';
    GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = data; r.slen = (int)got;
    return r;
}

/* ─── Built-in: make_ref ─── */

GemVal gem_make_ref_builtin(void *_env, GemVal *args, int argc) {
    (void)_env; (void)args; (void)argc;
    return gem_make_ref();
}

/* ─── Internal: __check_opts, __opts_rest (option bags) ───
 * The parser emits these for a destructured parameter shaped like an
 * option bag (one with an outer default or a field default):
 * __check_opts(bag, names) raises for a key the pattern doesn't name,
 * with the closest name; a pattern with `...rest` gets
 * __opts_rest(bag, names), the other entries, instead. `names` is the
 * pattern's field names separated by NUL bytes. A bag that isn't a table
 * is left to the field reads, which raise for it. */

/* Byte edit distance, for "did you mean"; names past 63 bytes compare as
   far apart. */
static int gem_edit_distance(const char *a, int an, const char *b, int bn) {
    /* Optimal string alignment: insert, delete, change, or swap two
       neighbouring bytes, one edit each. */
    if (an > 63 || bn > 63) return 99;
    int pp[64], prev[64], cur[64];
    for (int j = 0; j <= bn; j++) prev[j] = j;
    for (int i = 1; i <= an; i++) {
        cur[0] = i;
        for (int j = 1; j <= bn; j++) {
            int c = prev[j - 1] + (a[i - 1] != b[j - 1]);
            if (prev[j] + 1 < c) c = prev[j] + 1;
            if (cur[j - 1] + 1 < c) c = cur[j - 1] + 1;
            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1] && pp[j - 2] + 1 < c) c = pp[j - 2] + 1;
            cur[j] = c;
        }
        memcpy(pp, prev, sizeof(int) * (size_t)(bn + 1));
        memcpy(prev, cur, sizeof(int) * (size_t)(bn + 1));
    }
    return prev[bn];
}

/* Whether `key` (len klen) is one of the NUL-separated `names`. */
static int gem_opt_named(const char *names, int nlen, const char *key, int klen) {
    int start = 0;
    for (int i = 0; i <= nlen; i++) {
        if (i == nlen || names[i] == '\0') {
            if (i - start == klen && memcmp(names + start, key, (size_t)klen) == 0) return 1;
            start = i + 1;
        }
    }
    return 0;
}

void gem_check_opt_keys(GemVal bag, const char *who, const char *names, int nlen) {
    if (bag.type != VAL_TABLE) return;
    GemTable *t = bag.table;
    for (int i = 0; i < t->len; i++) {
        GemVal k = t->keys[i];
        if (k.type == VAL_STRING && gem_opt_named(names, nlen, k.sval, k.slen)) continue;
        GemVal ks = k.type == VAL_STRING ? k : gem_to_string_fn(NULL, &k, 1);
        const char *best = NULL;
        int best_len = 0, best_d = 99, start = 0;
        for (int j = 0; j <= nlen; j++) {
            if (j == nlen || names[j] == '\0') {
                int d = gem_edit_distance(ks.sval, ks.slen, names + start, j - start);
                if (d < best_d) { best_d = d; best = names + start; best_len = j - start; }
                start = j + 1;
            }
        }
        int limit = ks.slen <= 4 ? 1 : 2;
        if (!(best && best_d <= limit)) {
            /* `timeout` for `timeout_ms`: a name that is the key, `_` and more. */
            best = NULL;
            start = 0;
            for (int j = 0; j <= nlen && ks.slen >= 3; j++) {
                if (j == nlen || names[j] == '\0') {
                    int l = j - start;
                    if (l > ks.slen && names[start + ks.slen] == '_' && memcmp(names + start, ks.sval, (size_t)ks.slen) == 0) {
                        best = names + start; best_len = l;
                        break;
                    }
                    start = j + 1;
                }
            }
            best_d = 0;
        }
        char hint[160] = "";
        if (best && best_d <= limit) snprintf(hint, sizeof(hint), " (did you mean \"%.*s\"?)", best_len, best);
        char buf[512];
        snprintf(buf, sizeof(buf), "%s%sunknown option \"%.*s\"%s", who ? who : "", who ? ": " : "",
                 ks.slen > 100 ? 100 : (int)ks.slen, ks.sval, hint);
        gem_error(buf);
    }
}

GemVal gem_check_opts_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_TABLE) return GEM_NIL;
    /* The function the bag was passed to: the leaf running, else the
       innermost frame. */
    const char *fname = gem_leaf_site ? gem_leaf_site->name
        : (gem_call_depth > 0 && gem_call_depth <= GEM_MAX_CALL_DEPTH) ? gem_call_stack[gem_call_depth - 1].name : NULL;
    gem_check_opt_keys(args[0], fname, args[1].sval, args[1].slen);
    return GEM_NIL;
}

GemVal gem_opts_rest_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    GemVal out = gem_table_new_braces();
    if (argc < 2 || args[0].type != VAL_TABLE) return out;
    GemTable *t = args[0].table;
    const char *names = args[1].sval;
    int nlen = args[1].slen;
    for (int i = 0; i < t->len; i++) {
        GemVal k = t->keys[i];
        if (k.type == VAL_STRING && gem_opt_named(names, nlen, k.sval, k.slen)) continue;
        gem_table_set(out, k, t->vals[i]);
    }
    return out;
}
