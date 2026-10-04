/*
 * gem_builtins_sqlite.c — SQLite builtins: sqlite_open, sqlite_close,
 *                          sqlite_exec, sqlite_query, sqlite_last_insert_id,
 *                          sqlite_changes.
 */

#include "gem.h"
#include "sqlite3.h"

/* ─── Handle registry ───
   A Gem handle is a small opaque int id, never a pointer: ids count up from 1
   and are never reused, so a closed handle stays invalid instead of naming a
   later connection. The registry is only read and written on the scheduler
   thread (in the builtins themselves, never in the pool workers or the
   request free functions), so it needs no lock:
   - sqlite_open registers the connection after its worker is done and the
     requester has resumed; a requester killed mid-open never resumes, and
     gem_sqlite_open_free closes the connection with no entry made.
   - sqlite_close removes the entry as soon as the close is queued, before any
     worker can free the connection, so no process can reach it afterwards.
   Queries run inline on the scheduler thread without yielding, so none is in
   progress on a connection when a close is queued. */

static sqlite3 **gem_sqlite_handles = NULL;  /* index id - 1; NULL = closed */
static int64_t gem_sqlite_handle_count = 0;
static int64_t gem_sqlite_handle_cap = 0;

static int64_t gem_sqlite_register(sqlite3 *db) {
    if (gem_sqlite_handle_count == gem_sqlite_handle_cap) {
        int64_t cap = gem_sqlite_handle_cap ? gem_sqlite_handle_cap * 2 : 16;
        sqlite3 **h = (sqlite3 **)realloc(gem_sqlite_handles, (size_t)cap * sizeof(sqlite3 *));
        if (!h) {
            sqlite3_close(db);
            gem_error("sqlite_open: out of memory");
        }
        gem_sqlite_handles = h;
        gem_sqlite_handle_cap = cap;
    }
    gem_sqlite_handles[gem_sqlite_handle_count++] = db;
    return gem_sqlite_handle_count;
}

/* The open connection behind args[0], or raises "<fn>: ..." (pcall-catchable). */
static sqlite3 *gem_sqlite_handle(const char *fn, GemVal *args, int argc) {
    char buf[160];
    if (argc < 1) {
        snprintf(buf, sizeof(buf), "%s: expected a database handle", fn);
        gem_error(buf);
    }
    if (args[0].type != VAL_INT) {
        snprintf(buf, sizeof(buf), "%s: expected a database handle, got %s", fn, gem_type_str(args[0]));
        gem_error(buf);
    }
    int64_t id = args[0].ival;
    if (id < 1 || id > gem_sqlite_handle_count || !gem_sqlite_handles[id - 1]) {
        snprintf(buf, sizeof(buf), "%s: not an open database handle", fn);
        gem_error(buf);
    }
    return gem_sqlite_handles[id - 1];
}

/* ─── Thread pool args for sqlite_open ─── */

typedef struct {
    char *path;
    sqlite3 *db;
    char *error;
} GemSqliteOpenArgs;

static void gem_sqlite_open_worker(void *arg) {
    GemSqliteOpenArgs *a = (GemSqliteOpenArgs *)arg;
    int rc = sqlite3_open(a->path, &a->db);
    if (rc != SQLITE_OK) {
        a->error = strdup(sqlite3_errmsg(a->db));
        sqlite3_close(a->db);
        a->db = NULL;
        return;
    }
    sqlite3_exec(a->db, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
    sqlite3_exec(a->db, "PRAGMA foreign_keys=ON", NULL, NULL, NULL);
}

/* Closes the connection if the requester did not take it, which happens when
   the requester was killed before it resumed. */
static void gem_sqlite_open_free(void *arg) {
    GemSqliteOpenArgs *a = (GemSqliteOpenArgs *)arg;
    if (a->db) sqlite3_close(a->db);
    free(a->path);
    free(a->error);
    free(a);
}

/* ─── Thread pool args for sqlite_close ─── */

typedef struct {
    sqlite3 *db;
} GemSqliteCloseArgs;

static void gem_sqlite_close_worker(void *arg) {
    GemSqliteCloseArgs *a = (GemSqliteCloseArgs *)arg;
    sqlite3_close(a->db);
}

static void gem_sqlite_close_free(void *arg) {
    free(arg);
}

/* ─── Built-in: sqlite_open ─── */

GemVal gem_sqlite_open_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        gem_error("sqlite_open: expected string path");
    }
    const char *path = args[0].sval;

    if (gem_current_pid >= 0) {
        GemSqliteOpenArgs *a = (GemSqliteOpenArgs *)malloc(sizeof(GemSqliteOpenArgs));
        a->path = strdup(path);
        a->db = NULL;
        a->error = NULL;

        GemIORequest *req = gem_io_submit_extern(gem_sqlite_open_worker, a, gem_sqlite_open_free);
        if (!req) { gem_error("sqlite_open: I/O queue full"); }
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        proc->io_request = req;
        gem_io_pool_yield();
        proc->io_request = NULL;

        sqlite3 *db = a->db;
        a->db = NULL;
        if (a->error) {
            char buf[512];
            snprintf(buf, sizeof(buf), "sqlite_open: %s", a->error);
            gem_io_release(req);
            gem_error(buf);
        }
        gem_io_release(req);

        return gem_int(gem_sqlite_register(db));
    }

    sqlite3 *db;
    int rc = sqlite3_open(path, &db);
    if (rc != SQLITE_OK) {
        char buf[512];
        snprintf(buf, sizeof(buf), "sqlite_open: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        gem_error(buf);
    }
    sqlite3_exec(db, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
    sqlite3_exec(db, "PRAGMA foreign_keys=ON", NULL, NULL, NULL);

    return gem_int(gem_sqlite_register(db));
}

/* ─── Built-in: sqlite_close ─── */

GemVal gem_sqlite_close_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    sqlite3 *db = gem_sqlite_handle("sqlite_close", args, argc);
    int64_t id = args[0].ival;

    if (gem_current_pid >= 0) {
        GemSqliteCloseArgs *a = (GemSqliteCloseArgs *)malloc(sizeof(GemSqliteCloseArgs));
        a->db = db;

        GemIORequest *req = gem_io_submit_extern(gem_sqlite_close_worker, a, gem_sqlite_close_free);
        if (!req) { gem_error("sqlite_close: I/O queue full"); }
        gem_sqlite_handles[id - 1] = NULL;  /* queued: unreachable from now on */
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        proc->io_request = req;
        gem_io_pool_yield();
        proc->io_request = NULL;

        gem_io_release(req);
        return GEM_NIL;
    }

    gem_sqlite_handles[id - 1] = NULL;
    sqlite3_close(db);
    return GEM_NIL;
}

/* ─── Built-in: sqlite_exec ─── */

GemVal gem_sqlite_exec_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    sqlite3 *db = gem_sqlite_handle("sqlite_exec", args, argc);
    if (argc < 2 || args[1].type != VAL_STRING) {
        gem_error("sqlite_exec: expected (db, sql)");
    }
    /* Runs the statements one at a time, like sqlite3_exec, but refuses one
       with placeholders: sqlite_exec has nothing to bind them to, and
       sqlite would run it with NULLs. Statements before a failing one have
       run. */
    const char *sql = args[1].sval;
    const char *end = sql + args[1].slen;
    int n = 0;
    while (sql < end) {
        sqlite3_stmt *stmt = NULL;
        const char *tail = NULL;
        int rc = sqlite3_prepare_v2(db, sql, (int)(end - sql), &stmt, &tail);
        if (rc != SQLITE_OK) {
            char buf[512];
            snprintf(buf, sizeof(buf), "sqlite_exec: %s", sqlite3_errmsg(db));
            gem_error(buf);
        }
        if (stmt == NULL) break;   /* only whitespace or comments left */
        n++;
        if (sqlite3_bind_parameter_count(stmt) > 0) {
            char buf[160];
            snprintf(buf, sizeof(buf), "sqlite_exec: statement %d has parameters; use sqlite_query to bind them", n);
            sqlite3_finalize(stmt);
            gem_error(buf);
        }
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {}
        sqlite3_finalize(stmt);
        if (rc != SQLITE_DONE) {
            char buf[512];
            snprintf(buf, sizeof(buf), "sqlite_exec: %s", sqlite3_errmsg(db));
            gem_error(buf);
        }
        sql = tail;
    }
    return GEM_NIL;
}

/* Binds one value to parameter idx; on a bad value or a bind error,
   finalizes the statement and raises. */
static void gem_sqlite_bind_value(sqlite3 *db, sqlite3_stmt *stmt, int idx, GemVal v, const char *what) {
    int rc;
    switch (v.type) {
        case VAL_INT:    rc = sqlite3_bind_int64(stmt, idx, v.ival); break;
        case VAL_FLOAT:  rc = sqlite3_bind_double(stmt, idx, v.fval); break;
        case VAL_STRING: rc = sqlite3_bind_text(stmt, idx, v.sval, v.slen, SQLITE_TRANSIENT); break;
        case VAL_BOOL:   rc = sqlite3_bind_int64(stmt, idx, v.bval ? 1 : 0); break;
        case VAL_NIL:    rc = sqlite3_bind_null(stmt, idx); break;
        default: {
            char buf[320];
            snprintf(buf, sizeof(buf), "sqlite_query: parameter %s is a %s; expected nil, bool, int, float or string",
                     what, gem_type_str(v));
            sqlite3_finalize(stmt);
            gem_error(buf);
            return;
        }
    }
    if (rc != SQLITE_OK) {
        char buf[512];
        snprintf(buf, sizeof(buf), "sqlite_query: parameter %s: %s", what, sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        gem_error(buf);
    }
}

/* An array: the keys are exactly 0..got-1 (got distinct int keys in that
   range), in any insertion order; key k binds parameter k + 1. */
static void gem_sqlite_bind_array(sqlite3 *db, sqlite3_stmt *stmt, GemTable *params, int want) {
    int got = params ? params->len : 0;
    if (want != got) {
        char buf[128];
        snprintf(buf, sizeof(buf), "sqlite_query: statement has %d parameter(s), got %d", want, got);
        sqlite3_finalize(stmt);
        gem_error(buf);
    }
    for (int i = 0; i < got; i++) {
        GemVal key = params->keys[i];
        if (key.type != VAL_INT || key.ival < 0 || key.ival >= got) {
            char buf[192];
            if (key.type == VAL_INT)
                snprintf(buf, sizeof(buf), "sqlite_query: params must be an array (keys 0..%d), got key %lld",
                         got - 1, (long long)key.ival);
            else
                snprintf(buf, sizeof(buf), "sqlite_query: params must be an array, got a %s key", gem_type_str(key));
            sqlite3_finalize(stmt);
            gem_error(buf);
        }
    }
    for (int i = 0; i < got; i++) {
        int idx = (int)params->keys[i].ival + 1;
        char what[32];
        snprintf(what, sizeof(what), "%d", idx);
        gem_sqlite_bind_value(db, stmt, idx, params->vals[i], what);
    }
}

/* A record: each parameter is a named placeholder (`:name`, `@name` or
   `$name`), bound to the record's value at `name` (a present nil binds
   NULL). A positional placeholder, a name the record lacks, or a key that
   names no parameter raises. */
static void gem_sqlite_bind_named(sqlite3 *db, sqlite3_stmt *stmt, GemTable *params, int want) {
    int got = params->len;
    /* Arena memory: an error longjmps out without freeing anything. */
    char *used = (char *)gem_alloc((size_t)got + 1);
    memset(used, 0, (size_t)got + 1);
    for (int idx = 1; idx <= want; idx++) {
        const char *pname = sqlite3_bind_parameter_name(stmt, idx);
        if (pname == NULL || pname[0] == '?') {
            char buf[256];
            snprintf(buf, sizeof(buf), "sqlite_query: parameter %d is positional (?); bind it with a params array, or name it (:name)", idx);
            sqlite3_finalize(stmt);
            gem_error(buf);
        }
        const char *name = pname + 1;
        size_t nlen = strlen(name);
        int found = -1;
        for (int i = 0; i < got; i++) {
            GemVal k = params->keys[i];
            if ((size_t)k.slen == nlen && memcmp(k.sval, name, nlen) == 0) {
                found = i;
                break;
            }
        }
        if (found < 0) {
            char buf[320];
            snprintf(buf, sizeof(buf), "sqlite_query: no value for parameter %s (params has no key \"%s\")", pname, name);
            sqlite3_finalize(stmt);
            gem_error(buf);
        }
        used[found] = 1;
        gem_sqlite_bind_value(db, stmt, idx, params->vals[found], pname);
    }
    for (int i = 0; i < got; i++) {
        if (!used[i]) {
            char buf[320];
            GemVal k = params->keys[i];
            snprintf(buf, sizeof(buf), "sqlite_query: params key \"%.*s\" matches no parameter of the statement",
                     k.slen > 200 ? 200 : k.slen, k.sval);
            sqlite3_finalize(stmt);
            gem_error(buf);
        }
    }
}

/* ─── Built-in: sqlite_query ─── */

GemVal gem_sqlite_query_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    sqlite3 *db = gem_sqlite_handle("sqlite_query", args, argc);
    if (argc < 2 || args[1].type != VAL_STRING) {
        gem_error("sqlite_query: expected (db, sql[, params])");
    }
    const char *sql = args[1].sval;
    const char *end = sql + args[1].slen;

    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc = sqlite3_prepare_v2(db, sql, (int)(end - sql), &stmt, &tail);
    if (rc != SQLITE_OK) {
        char buf[512];
        snprintf(buf, sizeof(buf), "sqlite_query: %s", sqlite3_errmsg(db));
        gem_error(buf);
    }
    /* One statement only: the parameters are for it, and sqlite would
       silently skip the rest. Anything but whitespace and comments after it
       is refused before anything runs. */
    if (stmt != NULL && tail != NULL && tail < end) {
        sqlite3_stmt *next = NULL;
        int rc2 = sqlite3_prepare_v2(db, tail, (int)(end - tail), &next, NULL);
        if (rc2 != SQLITE_OK || next != NULL) {
            if (next) sqlite3_finalize(next);
            sqlite3_finalize(stmt);
            gem_error("sqlite_query: expected one SQL statement, got several (sqlite_exec runs several, without parameters)");
        }
    }

    /* Bind the parameters. Every error path finalizes the statement before
       gem_error longjmps out. */
    GemTable *params = NULL;
    if (argc >= 3 && args[2].type == VAL_TABLE) {
        params = args[2].table;
    } else if (argc >= 3 && args[2].type != VAL_NIL) {
        char buf[128];
        snprintf(buf, sizeof(buf), "sqlite_query: params must be a table, got %s", gem_type_str(args[2]));
        sqlite3_finalize(stmt);
        gem_error(buf);
    }
    /* Empty or comment-only SQL: no statement, no rows. */
    int want = stmt ? sqlite3_bind_parameter_count(stmt) : 0;
    int got = params ? params->len : 0;
    /* A record (string keys) binds by name; anything else is an array. */
    int n_str = 0;
    for (int i = 0; i < got; i++) {
        if (params->keys[i].type == VAL_STRING) n_str++;
    }
    if (n_str > 0 && n_str < got) {
        sqlite3_finalize(stmt);
        gem_error("sqlite_query: params must be an array or a record of names, not both");
    }
    if (n_str > 0) {
        gem_sqlite_bind_named(db, stmt, params, want);
    } else {
        gem_sqlite_bind_array(db, stmt, params, want);
    }

    GemVal result = gem_table_new();
    if (stmt == NULL) return result;
    int row_idx = 0;
    int col_count = sqlite3_column_count(stmt);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        GemVal row = gem_table_new();
        for (int c = 0; c < col_count; c++) {
            const char *col_name = sqlite3_column_name(stmt, c);
            GemVal key = gem_string(col_name);
            GemVal val;

            switch (sqlite3_column_type(stmt, c)) {
                case SQLITE_INTEGER:
                    val = gem_int(sqlite3_column_int64(stmt, c));
                    break;
                case SQLITE_FLOAT:
                    val = gem_float(sqlite3_column_double(stmt, c));
                    break;
                case SQLITE_TEXT:
                {
                    /* column_bytes after column_text: the text's length,
                       NULs included. */
                    const char *text = (const char *)sqlite3_column_text(stmt, c);
                    val = gem_string_with_len(text, sqlite3_column_bytes(stmt, c));
                }
                    break;
                case SQLITE_BLOB: {
                    const void *blob = sqlite3_column_blob(stmt, c);
                    int bytes = sqlite3_column_bytes(stmt, c);
                    char *copy = (char *)gem_alloc((size_t)bytes + 1);
                    memcpy(copy, blob, (size_t)bytes);
                    copy[bytes] = '\0';
                    val.type = VAL_STRING;
                    val.magic = GEM_MAGIC;
                    val.sval = copy;
                    val.slen = bytes;
                    break;
                }
                default:
                    val = GEM_NIL;
                    break;
            }
            gem_table_set(row, key, val);
        }
        gem_table_set(result, gem_int(row_idx++), row);
    }

    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        char buf[512];
        snprintf(buf, sizeof(buf), "sqlite_query: %s", sqlite3_errmsg(db));
        gem_error(buf);
    }

    return result;
}

/* ─── Built-in: sqlite_last_insert_id ─── */

GemVal gem_sqlite_last_insert_id_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    sqlite3 *db = gem_sqlite_handle("sqlite_last_insert_id", args, argc);
    return gem_int(sqlite3_last_insert_rowid(db));
}

/* ─── Built-in: sqlite_changes ─── */

GemVal gem_sqlite_changes_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    sqlite3 *db = gem_sqlite_handle("sqlite_changes", args, argc);
    return gem_int(sqlite3_changes(db));
}
