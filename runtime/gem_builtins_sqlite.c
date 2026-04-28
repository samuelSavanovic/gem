/*
 * gem_builtins_sqlite.c — SQLite builtins: sqlite_open, sqlite_close,
 *                          sqlite_exec, sqlite_query, sqlite_last_insert_id,
 *                          sqlite_changes.
 */

#include "gem.h"
#include "sqlite3.h"

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

/* ─── Thread pool args for sqlite_close ─── */

typedef struct {
    sqlite3 *db;
} GemSqliteCloseArgs;

static void gem_sqlite_close_worker(void *arg) {
    GemSqliteCloseArgs *a = (GemSqliteCloseArgs *)arg;
    sqlite3_close(a->db);
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

        GemIORequest *req = gem_io_submit_extern(gem_sqlite_open_worker, a);
        if (!req) { free(a->path); free(a); gem_error("sqlite_open: I/O queue full"); }
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        proc->io_request = req;
        gem_io_pool_yield();
        proc->io_request = NULL;

        sqlite3 *db = a->db;
        char *error = a->error;
        free(a->path);
        gem_io_free_request(req);

        if (error) {
            char buf[512];
            snprintf(buf, sizeof(buf), "sqlite_open: %s", error);
            free(error);
            free(a);
            gem_error(buf);
        }
        free(a);

        GemVal r; r.type = VAL_INT; r.ival = (int64_t)(intptr_t)db;
        return r;
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

    GemVal r; r.type = VAL_INT; r.ival = (int64_t)(intptr_t)db;
    return r;
}

/* ─── Built-in: sqlite_close ─── */

GemVal gem_sqlite_close_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("sqlite_close: expected db handle");
    }
    sqlite3 *db = (sqlite3 *)(intptr_t)args[0].ival;

    if (gem_current_pid >= 0) {
        GemSqliteCloseArgs *a = (GemSqliteCloseArgs *)malloc(sizeof(GemSqliteCloseArgs));
        a->db = db;

        GemIORequest *req = gem_io_submit_extern(gem_sqlite_close_worker, a);
        if (!req) { free(a); gem_error("sqlite_close: I/O queue full"); }
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        proc->io_request = req;
        gem_io_pool_yield();
        proc->io_request = NULL;

        gem_io_free_request(req);
        free(a);
        return GEM_NIL;
    }

    sqlite3_close(db);
    return GEM_NIL;
}

/* ─── Built-in: sqlite_exec ─── */

GemVal gem_sqlite_exec_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_INT || args[1].type != VAL_STRING) {
        gem_error("sqlite_exec: expected (db, sql)");
    }
    sqlite3 *db = (sqlite3 *)(intptr_t)args[0].ival;
    const char *sql = args[1].sval;

    char *errmsg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
    if (rc != SQLITE_OK) {
        char buf[512];
        snprintf(buf, sizeof(buf), "sqlite_exec: %s", errmsg ? errmsg : "unknown error");
        sqlite3_free(errmsg);
        gem_error(buf);
    }
    return GEM_NIL;
}

/* ─── Built-in: sqlite_query ─── */

GemVal gem_sqlite_query_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_INT || args[1].type != VAL_STRING) {
        gem_error("sqlite_query: expected (db, sql[, params])");
    }
    sqlite3 *db = (sqlite3 *)(intptr_t)args[0].ival;
    const char *sql = args[1].sval;

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        char buf[512];
        snprintf(buf, sizeof(buf), "sqlite_query: %s", sqlite3_errmsg(db));
        gem_error(buf);
    }

    if (argc >= 3 && args[2].type == VAL_TABLE) {
        GemTable *params = args[2].table;
        for (int i = 0; i < params->len; i++) {
            GemVal v = params->vals[i];
            int idx = i + 1;
            switch (v.type) {
                case VAL_INT:    sqlite3_bind_int64(stmt, idx, v.ival); break;
                case VAL_FLOAT:  sqlite3_bind_double(stmt, idx, v.fval); break;
                case VAL_STRING: sqlite3_bind_text(stmt, idx, v.sval, -1, SQLITE_TRANSIENT); break;
                case VAL_BOOL:   sqlite3_bind_int64(stmt, idx, v.bval ? 1 : 0); break;
                case VAL_NIL:    sqlite3_bind_null(stmt, idx); break;
                default:         sqlite3_bind_null(stmt, idx); break;
            }
        }
    }

    GemVal result = gem_table_new();
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
                    val = gem_string((const char *)sqlite3_column_text(stmt, c));
                    break;
                case SQLITE_BLOB: {
                    const void *blob = sqlite3_column_blob(stmt, c);
                    int bytes = sqlite3_column_bytes(stmt, c);
                    char *copy = (char *)GC_MALLOC_ATOMIC((size_t)bytes + 1);
                    memcpy(copy, blob, (size_t)bytes);
                    copy[bytes] = '\0';
                    val.type = VAL_STRING;
                    val.sval = copy;
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
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("sqlite_last_insert_id: expected db handle");
    }
    sqlite3 *db = (sqlite3 *)(intptr_t)args[0].ival;
    return gem_int(sqlite3_last_insert_rowid(db));
}

/* ─── Built-in: sqlite_changes ─── */

GemVal gem_sqlite_changes_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("sqlite_changes: expected db handle");
    }
    sqlite3 *db = (sqlite3 *)(intptr_t)args[0].ival;
    return gem_int(sqlite3_changes(db));
}
