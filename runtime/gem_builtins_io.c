/*
 * gem_builtins_io.c — File I/O and filesystem builtins: read_file, write_file,
 *                      append_file, file_exists, dirname, path_join,
 *                      normalize_path, remove_file, mkdir, list_dir, is_dir, exec;
 *                      plus gem_exe_path (the running binary's real path).
 */

#include "gem.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

/* ─── Built-in: read_file ─── */

GemVal gem_read_file_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "read_file: expected string path, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    const char *path = args[0].sval;

    if (gem_current_pid >= 0) {
        GemIORequest *req = gem_io_submit(GEM_IO_READ_FILE, path, NULL, 0);
        if (!req) { gem_error("read_file: I/O queue full"); }
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        proc->io_request = req;
        gem_io_pool_yield();
        proc->io_request = NULL;
        if (req->error_msg) {
            char buf[512]; snprintf(buf, sizeof(buf), "read_file: %s", req->error_msg);
            gem_io_release(req);
            gem_error(buf);
        }
        size_t rlen = req->result_len;
        char *data = (char *)gem_alloc(rlen + 1);
        memcpy(data, req->result_data, rlen);
        data[rlen] = '\0';
        gem_io_release(req);
        GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = data; r.slen = (int)rlen;
        return r;
    }

    size_t nread;
    char *err = NULL;
    char *raw = gem_read_whole_file(path, &nread, &err);
    if (!raw) {
        char buf[512]; snprintf(buf, sizeof(buf), "read_file: %s", err);
        free(err);
        gem_error(buf);
    }
    char *data = (char *)gem_alloc(nread + 1);
    memcpy(data, raw, nread + 1);
    free(raw);
    GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = data; r.slen = (int)nread;
    return r;
}

/* ─── Built-in: write_file ─── */

GemVal gem_write_file_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "write_file: expected (string, string), got (%s, %s)", argc < 1 ? "nothing" : gem_type_str(args[0]), argc < 2 ? "nothing" : gem_type_str(args[1])); gem_error(buf);
    }
    const char *path = args[0].sval;
    const char *content = args[1].sval;
    size_t len = (size_t)args[1].slen;

    if (gem_current_pid >= 0) {
        GemIORequest *req = gem_io_submit(GEM_IO_WRITE_FILE, path, content, len);
        if (!req) { gem_error("write_file: I/O queue full"); }
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        proc->io_request = req;
        gem_io_pool_yield();
        proc->io_request = NULL;
        if (req->error_msg) {
            char buf[512]; snprintf(buf, sizeof(buf), "write_file: %s", req->error_msg);
            gem_io_release(req);
            gem_error(buf);
        }
        gem_io_release(req);
        return GEM_NIL;
    }

    FILE *f = fopen(path, "wb");
    if (!f) {
        char buf[512]; snprintf(buf, sizeof(buf), "write_file: cannot open '%s' for writing: %s", path, strerror(errno)); gem_error(buf);
    }
    size_t nwritten = fwrite(content, 1, len, f);
    fclose(f);
    if (nwritten != len) {
        char buf[512]; snprintf(buf, sizeof(buf), "write_file: write failed for '%s'", path); gem_error(buf);
    }
    return GEM_NIL;
}

/* ─── Built-in: append_file ─── */

GemVal gem_append_file_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "append_file: expected (string, string), got (%s, %s)",
            argc < 1 ? "nothing" : gem_type_str(args[0]), argc < 2 ? "nothing" : gem_type_str(args[1]));
        gem_error(buf);
    }
    const char *path = args[0].sval;
    const char *content = args[1].sval;
    size_t len = (size_t)args[1].slen;

    if (gem_current_pid >= 0) {
        GemIORequest *req = gem_io_submit(GEM_IO_APPEND_FILE, path, content, len);
        if (!req) { gem_error("append_file: I/O queue full"); }
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        proc->io_request = req;
        gem_io_pool_yield();
        proc->io_request = NULL;
        if (req->error_msg) {
            char buf[512]; snprintf(buf, sizeof(buf), "append_file: %s", req->error_msg);
            gem_io_release(req);
            gem_error(buf);
        }
        gem_io_release(req);
        return GEM_NIL;
    }

    FILE *f = fopen(path, "ab");
    if (!f) { char buf[512]; snprintf(buf, sizeof(buf), "append_file: cannot open '%s': %s", path, strerror(errno)); gem_error(buf); }
    size_t nwritten = fwrite(content, 1, len, f);
    fclose(f);
    if (nwritten != len) { char buf[512]; snprintf(buf, sizeof(buf), "append_file: write failed for '%s'", path); gem_error(buf); }
    return GEM_NIL;
}

/* ─── Built-in: file_exists ─── */

GemVal gem_file_exists_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "file_exists: expected string path, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    FILE *f = fopen(args[0].sval, "rb");
    if (f) { fclose(f); return gem_bool(1); }
    return gem_bool(0);
}

/* ─── Built-in: dirname ─── */

GemVal gem_dirname_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "dirname: expected string path, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    char *copy = strdup(args[0].sval);
    char *dir = dirname(copy);
    GemVal r = gem_string(dir);
    free(copy);
    return r;
}

/* ─── Built-in: path_join ─── */

GemVal gem_path_join_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "path_join: expected (string, string), got (%s, %s)",
            argc < 1 ? "nothing" : gem_type_str(args[0]), argc < 2 ? "nothing" : gem_type_str(args[1])); gem_error(buf);
    }
    const char *dir = args[0].sval;
    const char *file = args[1].sval;
    size_t dlen = (size_t)args[0].slen;
    size_t flen = (size_t)args[1].slen;
    gem_strlen_check(dlen + 1 + flen, "path_join");
    if (flen > 0 && file[0] == '/') {
        char *copy = (char *)gem_alloc(flen + 1);
        memcpy(copy, file, flen);
        copy[flen] = '\0';
        GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = copy; r.slen = (int)flen;
        return r;
    }
    int needs_sep = (dlen > 0 && dir[dlen-1] != '/');
    size_t total = dlen + (needs_sep ? 1 : 0) + flen;
    char *result = (char *)gem_alloc(total + 1);
    memcpy(result, dir, dlen);
    size_t pos = dlen;
    if (needs_sep) result[pos++] = '/';
    memcpy(result + pos, file, flen);
    result[total] = '\0';
    GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = result; r.slen = (int)total;
    return r;
}

/* ─── Built-in: normalize_path ─── */

GemVal gem_normalize_path_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "normalize_path: expected string path, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    char *resolved = realpath(args[0].sval, NULL);
    if (resolved) {
        GemVal r = gem_string(resolved);
        free(resolved);
        return r;
    }
    return gem_string(args[0].sval);
}

/* ─── gem_exe_path: canonical path of the running executable ───
 *
 * Not a builtin: the compiler and the LSP call it through
 * `extern fn gem_exe_path() -> String` (compiler/loader.gem) to find the
 * install root (std/, runtime/, build/libgem_runtime.a) two levels above
 * the binary, whatever name it was started by (a symlink, a bare name
 * found on PATH, a relative path). Order: the OS's own answer
 * (/proc/self/exe on Linux, _NSGetExecutablePath on macOS), then argv[0]
 * (taken relative to the cwd when it has a '/', else looked up on PATH
 * like execvp does), each passed through realpath so symlinks resolve.
 * When nothing resolves it returns argv[0] as typed. The result is a
 * static buffer, computed once; it is never NULL. */

static int exe_path_try(const char *p, char *out) {
    struct stat st;
    if (!p || !*p) return 0;
    if (!realpath(p, out)) return 0;
    if (stat(out, &st) != 0 || !S_ISREG(st.st_mode)) return 0;
    return 1;
}

char *gem_exe_path(void) {
    static char resolved[PATH_MAX];
    static int done = 0;
    if (done) return resolved;
    char cand[PATH_MAX];
    const char *argv0 = (gem_stored_argc > 0 && gem_stored_argv) ? gem_stored_argv[0] : NULL;
    int ok = 0;
#if defined(__linux__)
    ok = exe_path_try("/proc/self/exe", resolved);
#elif defined(__APPLE__)
    uint32_t sz = (uint32_t)sizeof(cand);
    if (_NSGetExecutablePath(cand, &sz) == 0) ok = exe_path_try(cand, resolved);
#endif
    if (!ok && argv0 && *argv0) {
        if (strchr(argv0, '/')) {
            ok = exe_path_try(argv0, resolved);
        } else {
            const char *path = getenv("PATH");
            const char *p = path ? path : "";
            while (!ok && p) {
                const char *colon = strchr(p, ':');
                size_t dlen = colon ? (size_t)(colon - p) : strlen(p);
                /* An empty PATH entry means the current directory. */
                int n = dlen == 0
                    ? snprintf(cand, sizeof(cand), "./%s", argv0)
                    : snprintf(cand, sizeof(cand), "%.*s/%s", (int)dlen, p, argv0);
                if (n > 0 && (size_t)n < sizeof(cand) && access(cand, X_OK) == 0)
                    ok = exe_path_try(cand, resolved);
                p = colon ? colon + 1 : NULL;
            }
        }
    }
    if (!ok) snprintf(resolved, sizeof(resolved), "%s", argv0 ? argv0 : "");
    done = 1;
    return resolved;
}

/* ─── Built-in: remove_file ─── */

GemVal gem_remove_file_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "remove_file: expected string path, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    return gem_bool(unlink(args[0].sval) == 0 ? 1 : 0);
}

/* ─── Built-in: mkdir ─── */

GemVal gem_mkdir_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "mkdir: expected string path, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    return gem_bool(mkdir(args[0].sval, 0755) == 0 ? 1 : 0);
}

/* ─── Built-in: list_dir ─── */

GemVal gem_list_dir_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "list_dir: expected string path, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    DIR *d = opendir(args[0].sval);
    if (!d) {
        char buf[512]; snprintf(buf, sizeof(buf), "list_dir: cannot open directory '%s': %s", args[0].sval, strerror(errno)); gem_error(buf);
    }
    GemVal result = gem_table_new();
    struct dirent *entry;
    int i = 0;
    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        gem_table_set(result, gem_int(i), gem_string(entry->d_name));
        i++;
    }
    closedir(d);
    return result;
}

/* ─── Built-in: is_dir ─── */

GemVal gem_is_dir_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "is_dir: expected string path, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    struct stat st;
    if (stat(args[0].sval, &st) != 0) return gem_bool(0);
    return gem_bool(S_ISDIR(st.st_mode) ? 1 : 0);
}

/* ─── Built-in: exec (run a shell command, return exit code) ─── */

GemVal gem_exec_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "exec: expected string command, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }

    if (gem_current_pid >= 0) {
        GemIORequest *req = gem_io_submit(GEM_IO_EXEC, args[0].sval, NULL, 0);
        if (!req) { gem_error("exec: I/O queue full"); }
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        proc->io_request = req;
        gem_io_pool_yield();
        proc->io_request = NULL;
        int code = req->exit_code;
        gem_io_release(req);
        return gem_int(code);
    }

    int ret = system(args[0].sval);
    int code = WIFEXITED(ret) ? WEXITSTATUS(ret) : -1;
    return gem_int(code);
}
