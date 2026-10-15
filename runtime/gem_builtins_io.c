/*
 * gem_builtins_io.c — File I/O and filesystem builtins: read_file, write_file,
 *                      append_file, file_exists, dirname, path_join,
 *                      normalize_path, remove_file, mkdir, list_dir, is_dir, exec;
 *                      plus gem_exe_path (the running binary's real path) and
 *                      gem_temp_dir/gem_temp_dir_in/gem_rmdir (the compiler's work directories),
 *                      gem_hash_hex/gem_file_hash/gem_mkdirs/gem_link/gem_file_age
 *                      (the compiler's binary cache).
 */

#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE   /* pipe2 */
#endif
#include "gem.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <spawn.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

/* ─── Built-in: read_file ─── */

/* read_file(path, offset, count): up to `count` bytes from byte `offset`,
   "" past the end. Opens the file for each call, so a caller walking a file
   in blocks holds nothing open between calls. Only a file that keeps its
   bytes between opens (a regular file, a block device) can be read from an
   offset past 0: a pipe, FIFO or terminal would hand each open new data,
   so offset > 0 raises for those. */
#define GEM_NO_OFFSET_FMT "'%s' is not a regular file: it can be read from offset 0 only (read it whole with read_file(path))"

static int gem_keeps_bytes(const struct stat *st) {
    return S_ISREG(st->st_mode) || S_ISBLK(st->st_mode);
}

char *gem_read_file_range(const char *path, int64_t offset, int64_t count,
                          size_t *out_len, char **err_msg) {
    char msg[512];
    *out_len = 0;
    *err_msg = NULL;
    struct stat st;
    /* Refused before the open too: opening a FIFO waits for a writer. */
    if (offset > 0 && stat(path, &st) == 0 && !S_ISDIR(st.st_mode) && !gem_keeps_bytes(&st)) {
        snprintf(msg, sizeof(msg), GEM_NO_OFFSET_FMT, path);
        *err_msg = strdup(msg);
        return NULL;
    }
    FILE *f = fopen(path, "rb");
    if (!f) {
        snprintf(msg, sizeof(msg), "cannot open '%s': %s", path, strerror(errno));
        *err_msg = strdup(msg);
        return NULL;
    }
    if (fstat(fileno(f), &st) != 0) {
        fclose(f);
        snprintf(msg, sizeof(msg), "cannot stat '%s': %s", path, strerror(errno));
        *err_msg = strdup(msg);
        return NULL;
    }
    if (S_ISDIR(st.st_mode)) {
        fclose(f);
        snprintf(msg, sizeof(msg), "'%s' is a directory", path);
        *err_msg = strdup(msg);
        return NULL;
    }
    if (offset > 0 && (!gem_keeps_bytes(&st) || fseeko(f, (off_t)offset, SEEK_SET) != 0)) {
        fclose(f);
        snprintf(msg, sizeof(msg), GEM_NO_OFFSET_FMT, path);
        *err_msg = strdup(msg);
        return NULL;
    }
    /* The buffer starts at what st_size says is left of a regular file
       (unless it reports 0, as procfs files do), else at 64 KB, and grows
       while bytes keep arriving. */
    size_t want = (size_t)count, cap;
    if (S_ISREG(st.st_mode) && st.st_size > 0) {
        int64_t left = (int64_t)st.st_size > offset ? (int64_t)st.st_size - offset : 0;
        cap = (int64_t)want < left ? want : (size_t)left;
    } else {
        cap = want < 65536 ? want : 65536;
    }
    char *data = (char *)malloc(cap + 1);
    size_t len = 0;
    for (;;) {
        len += fread(data + len, 1, cap - len, f);
        if (len < cap || len == want) break;   /* end of file, or done */
        cap = cap ? cap * 2 : 4096;
        if (cap > want) cap = want;
        data = (char *)realloc(data, cap + 1);
    }
    if (ferror(f)) {
        fclose(f);
        free(data);
        snprintf(msg, sizeof(msg), "read failed for '%s'", path);
        *err_msg = strdup(msg);
        return NULL;
    }
    fclose(f);
    data[len] = '\0';
    *out_len = len;
    return data;
}

static GemVal gem_read_file_range_fn(GemVal *args) {
    if (args[1].type != VAL_INT || args[2].type != VAL_INT) {
        char buf[160]; snprintf(buf, sizeof(buf), "read_file: expected (string, int, int), got (string, %s, %s)", gem_type_str(args[1]), gem_type_str(args[2])); gem_error(buf);
    }
    const char *path = args[0].sval;
    int64_t offset = args[1].ival, count = args[2].ival;
    if (offset < 0) gem_error("read_file: negative offset");
    if (count < 0) gem_error("read_file: negative count");
    if (count > (int64_t)GEM_MAX_STRLEN) count = GEM_MAX_STRLEN;
    if (gem_current_pid >= 0) {
        GemIORequest *req = gem_io_submit_range(path, offset, count);
        if (!req) { gem_error("read_file: I/O queue full"); }
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        proc->io_request = req;
        gem_io_pool_yield();
        proc->io_request = NULL;
        if (req->error_msg) {
            char buf[600]; snprintf(buf, sizeof(buf), "read_file: %s", req->error_msg);
            gem_io_release(req);
            gem_error(buf);
        }
        GemVal r = gem_string_with_len(req->result_data, (int64_t)req->result_len);
        gem_io_release(req);
        return r;
    }
    size_t n;
    char *err = NULL;
    char *raw = gem_read_file_range(path, offset, count, &n, &err);
    if (!raw) {
        char buf[600]; snprintf(buf, sizeof(buf), "read_file: %s", err);
        free(err);
        gem_error(buf);
    }
    char *data = (char *)gem_alloc(n + 1);
    memcpy(data, raw, n + 1);
    free(raw);
    GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = data; r.slen = (int)n;
    return r;
}

GemVal gem_read_file_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "read_file: expected string path, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    if (argc == 2 || argc > 3) {
        char buf[128]; snprintf(buf, sizeof(buf), "read_file: expected 1 or 3 arguments (path, offset, count), got %d", argc); gem_error(buf);
    }
    if (argc == 3) return gem_read_file_range_fn(args);
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

/* ─── gem_temp_dir, gem_temp_dir_in, gem_rmdir: work directories ───
 *
 * Not builtins: the compiler calls them through `extern fn`
 * (compiler/main.gem) to keep each compile's C file, and the binary it
 * runs, in a directory no other compile uses. gem_temp_dir makes a new
 * directory with mkdtemp under $TMPDIR (else /tmp), gem_temp_dir_in one
 * under `parent`; both return its path in a static buffer, or NULL when
 * they can't. gem_rmdir removes an empty directory and returns 1, else 0. */

char *gem_temp_dir_in(const char *parent) {
    static char dir[PATH_MAX];
    size_t n = strlen(parent);
    while (n > 1 && parent[n - 1] == '/') n--;
    int len = snprintf(dir, sizeof(dir), "%.*s/gem-XXXXXX", (int)n, parent);
    if (len < 0 || (size_t)len >= sizeof(dir)) return NULL;
    return mkdtemp(dir);
}

char *gem_temp_dir(void) {
    const char *tmp = getenv("TMPDIR");
    return gem_temp_dir_in(tmp && *tmp ? tmp : "/tmp");
}

int64_t gem_rmdir(const char *path) {
    return rmdir(path) == 0 ? 1 : 0;
}

/* ─── The compiler's binary cache (compiler/main.gem, run mode) ───
 *
 * Not builtins either: the compiler calls them through `extern fn`.
 * gem_hash_hex hashes bytes to 32 hex digits (two 64-bit FNV-1a lanes
 * with different offsets, the second with a shift-xor after each byte): a cache
 * key, not a cryptographic hash. gem_file_hash hashes a file's contents,
 * "" when it can't be read. gem_mkdirs is mkdir -p (1 on success, the
 * directory existing included; 0 when the path, or one of its parents,
 * exists and is not a directory); gem_link makes `to` a hard link to
 * `from` (1 on success; 0 when `to` exists or `from` doesn't);
 * gem_file_age is the seconds since a file's inode last changed (its
 * st_ctime, which a new link sets), -1 when it can't be read. */

static void gem_fnv2(const uint8_t *p, size_t n, uint64_t *a, uint64_t *b) {
    for (size_t i = 0; i < n; i++) {
        *a = (*a ^ p[i]) * 0x100000001b3ULL;
        *b = (*b ^ p[i]) * 0x100000001b3ULL;
        *b ^= *b >> 29;
    }
}

char *gem_hash_hex(const uint8_t *data, int64_t len) {
    static char out[33];
    uint64_t a = 0xcbf29ce484222325ULL, b = 0x84222325cbf29ce4ULL;
    gem_fnv2(data, (size_t)len, &a, &b);
    snprintf(out, sizeof(out), "%016llx%016llx", (unsigned long long)a, (unsigned long long)b);
    return out;
}

char *gem_file_hash(const char *path) {
    static char out[33];
    FILE *f = fopen(path, "rb");
    if (!f) { out[0] = 0; return out; }
    uint64_t a = 0xcbf29ce484222325ULL, b = 0x84222325cbf29ce4ULL;
    uint8_t buf[65536];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) gem_fnv2(buf, n, &a, &b);
    fclose(f);
    snprintf(out, sizeof(out), "%016llx%016llx", (unsigned long long)a, (unsigned long long)b);
    return out;
}

int64_t gem_mkdirs(const char *path) {
    char tmp[PATH_MAX];
    size_t n = strlen(path);
    if (n == 0 || n >= sizeof(tmp)) return 0;
    memcpy(tmp, path, n + 1);
    for (size_t i = 1; i < n; i++) {
        if (tmp[i] == '/') {
            tmp[i] = 0;
            if (mkdir(tmp, 0755) != 0 && errno != EEXIST) return 0;
            tmp[i] = '/';
        }
    }
    if (mkdir(tmp, 0755) == 0) return 1;
    struct stat st;
    return errno == EEXIST && stat(tmp, &st) == 0 && S_ISDIR(st.st_mode) ? 1 : 0;
}

int64_t gem_link(const char *from, const char *to) {
    return link(from, to) == 0 ? 1 : 0;
}

int64_t gem_file_age(const char *path) {
    struct stat st;
    if (lstat(path, &st) != 0) return -1;
    return (int64_t)(time(NULL) - st.st_ctime);
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

/* ─── Built-ins: rename, stat ─── */

GemVal gem_rename_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "rename: expected (string, string), got (%s, %s)", argc < 1 ? "nothing" : gem_type_str(args[0]), argc < 2 ? "nothing" : gem_type_str(args[1])); gem_error(buf);
    }
    return gem_bool(rename(args[0].sval, args[1].sval) == 0 ? 1 : 0);
}

/* stat(path): nil when nothing is there, else {size, mtime (ms since the
   epoch), mode (the permission bits), type: "file" | "dir" | "link" |
   "other"}. Follows symlinks, like is_dir; a dangling link is "link". */
GemVal gem_stat_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "stat: expected string path, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    struct stat st;
    if (stat(args[0].sval, &st) != 0) {
        if (lstat(args[0].sval, &st) != 0) return GEM_NIL;
    }
    const char *type = S_ISREG(st.st_mode) ? "file" : S_ISDIR(st.st_mode) ? "dir" : S_ISLNK(st.st_mode) ? "link" : "other";
#ifdef __APPLE__
    int64_t mtime_ms = (int64_t)st.st_mtimespec.tv_sec * 1000 + st.st_mtimespec.tv_nsec / 1000000;
#else
    int64_t mtime_ms = (int64_t)st.st_mtim.tv_sec * 1000 + st.st_mtim.tv_nsec / 1000000;
#endif
    GemVal r = gem_table_new();
    gem_table_set(r, gem_string("size"), gem_int((int64_t)st.st_size));
    gem_table_set(r, gem_string("mtime"), gem_int(mtime_ms));
    gem_table_set(r, gem_string("mode"), gem_int((int64_t)(st.st_mode & 07777)));
    gem_table_set(r, gem_string("type"), gem_string(type));
    return r;
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

/* ─── Built-in: exec (run a shell command, return exit code) ───
 *
 * exec and exec_capture start `/bin/sh -c cmd` with posix_spawn on a pool
 * thread, passing the environment getenv and setenv see
 * (gem_env_snapshot). A child must inherit only its stdin, stdout and
 * stderr. On Linux the pipes and sockets the runtime makes while pool
 * threads may be spawning are close-on-exec from the moment they exist
 * (pipe2, accept4, SOCK_CLOEXEC); macOS has no such calls, so every spawn
 * there sets POSIX_SPAWN_CLOEXEC_DEFAULT, which closes every other
 * descriptor in the child. Otherwise a command started on one worker could hold the write
 * end of another exec_capture's pipe, and that exec_capture would wait
 * until the command exits. */

/* The runtime ignores SIGPIPE (a write to a closed socket returns an
   error instead); a child gets the default back, so `yes | head` ends. */
static void gem_spawn_attr_init(posix_spawnattr_t *attr) {
    posix_spawnattr_init(attr);
    sigset_t def;
    sigemptyset(&def);
    sigaddset(&def, SIGPIPE);
    posix_spawnattr_setsigdefault(attr, &def);
    short flags = POSIX_SPAWN_SETSIGDEF;
#ifdef POSIX_SPAWN_CLOEXEC_DEFAULT
    flags |= POSIX_SPAWN_CLOEXEC_DEFAULT;
#endif
    posix_spawnattr_setflags(attr, flags);
}

/* Starts `/bin/sh -c cmd` with the file actions `fa` (NULL: none) and the
   runtime's environment; 0 or an errno, like posix_spawn. */
static int gem_spawn_shell(pid_t *pid, const char *cmd, posix_spawn_file_actions_t *fa) {
    char *argv[] = {"sh", "-c", (char *)cmd, NULL};
    posix_spawnattr_t attr;
    gem_spawn_attr_init(&attr);
    char **envp = gem_env_snapshot();
    int rc = posix_spawn(pid, "/bin/sh", fa, &attr, argv, envp);
    free(envp);
    posix_spawnattr_destroy(&attr);
    return rc;
}

int gem_run_shell(const char *cmd) {
    pid_t pid;
    posix_spawn_file_actions_t fa;
    posix_spawn_file_actions_init(&fa);
#ifdef POSIX_SPAWN_CLOEXEC_DEFAULT
    /* The child keeps the program's stdin, stdout and stderr. */
    for (int fd = 0; fd <= 2; fd++) posix_spawn_file_actions_addinherit_np(&fa, fd);
#endif
    int rc = gem_spawn_shell(&pid, cmd, &fa);
    posix_spawn_file_actions_destroy(&fa);
    if (rc != 0) return -1;
    int status;
    while (waitpid(pid, &status, 0) < 0)
        if (errno != EINTR) return -1;
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

/* A pipe whose ends close on exec (the dup2 in the file actions clears
   the flag on the child's own copies). */
static int gem_cloexec_pipe(int fds[2]) {
#ifdef __linux__
    return pipe2(fds, O_CLOEXEC);
#else
    if (pipe(fds) != 0) return -1;
    fcntl(fds[0], F_SETFD, FD_CLOEXEC);
    fcntl(fds[1], F_SETFD, FD_CLOEXEC);
    return 0;
#endif
}

typedef struct { char *data; size_t len, cap; int over; } GemGrowBuf;

static void gem_growbuf_read(GemGrowBuf *b, int fd, int *open_flag) {
    if (b->cap - b->len < 65536 + 1) {
        b->cap = b->cap ? b->cap * 2 : 65536 * 2;
        b->data = (char *)realloc(b->data, b->cap);
    }
    ssize_t n = read(fd, b->data + b->len, 65536);
    if (n > 0) {
        b->len += (size_t)n;
        if (b->len > (size_t)GEM_MAX_STRLEN) { b->over = 1; b->len = 0; }
    } else if (n == 0 || (errno != EINTR && errno != EAGAIN)) {
        close(fd);
        *open_flag = 0;
    }
}

int gem_run_capture(const char *cmd, const char *in, size_t in_len,
                    char **out, size_t *out_len, char **err, size_t *err_len,
                    char **errmsg) {
    *out = NULL; *err = NULL; *out_len = 0; *err_len = 0; *errmsg = NULL;
    int pin[2] = {-1, -1}, pout[2], perr[2];
    if (gem_cloexec_pipe(pout) != 0) return -1;
    if (gem_cloexec_pipe(perr) != 0) { close(pout[0]); close(pout[1]); return -1; }
    if (in && gem_cloexec_pipe(pin) != 0) {
        close(pout[0]); close(pout[1]); close(perr[0]); close(perr[1]);
        return -1;
    }
    posix_spawn_file_actions_t fa;
    posix_spawn_file_actions_init(&fa);
    if (in) posix_spawn_file_actions_adddup2(&fa, pin[0], 0);
    else posix_spawn_file_actions_addopen(&fa, 0, "/dev/null", O_RDONLY, 0);
    posix_spawn_file_actions_adddup2(&fa, pout[1], 1);
    posix_spawn_file_actions_adddup2(&fa, perr[1], 2);
    pid_t pid;
    int rc = gem_spawn_shell(&pid, cmd, &fa);
    posix_spawn_file_actions_destroy(&fa);
    close(pout[1]); close(perr[1]);
    if (in) close(pin[0]);
    if (rc != 0) {
        close(pout[0]); close(perr[0]);
        if (in) close(pin[1]);
        return -1;
    }
    GemGrowBuf bo = {0}, be = {0};
    int o_open = 1, e_open = 1, i_open = in != NULL;
    size_t in_off = 0;
    if (i_open) fcntl(pin[1], F_SETFL, fcntl(pin[1], F_GETFL) | O_NONBLOCK);
    if (i_open && in_len == 0) { close(pin[1]); i_open = 0; }
    while (o_open || e_open || i_open) {
        struct pollfd pf[3];
        int n = 0, io = -1, ie = -1, ii = -1;
        if (o_open) { pf[n].fd = pout[0]; pf[n].events = POLLIN; io = n++; }
        if (e_open) { pf[n].fd = perr[0]; pf[n].events = POLLIN; ie = n++; }
        if (i_open) { pf[n].fd = pin[1]; pf[n].events = POLLOUT; ii = n++; }
        if (poll(pf, (nfds_t)n, -1) < 0) {
            if (errno == EINTR) continue;
            break;
        }
        if (io >= 0 && pf[io].revents) gem_growbuf_read(&bo, pout[0], &o_open);
        if (ie >= 0 && pf[ie].revents) gem_growbuf_read(&be, perr[0], &e_open);
        if (ii >= 0 && pf[ii].revents) {
            ssize_t w = write(pin[1], in + in_off, in_len - in_off);
            if (w > 0) in_off += (size_t)w;
            if ((w < 0 && errno != EINTR && errno != EAGAIN) || in_off == in_len) {
                close(pin[1]);
                i_open = 0;
            }
        }
    }
    if (o_open) close(pout[0]);
    if (e_open) close(perr[0]);
    if (i_open) close(pin[1]);
    int status;
    int code = -1;
    while (waitpid(pid, &status, 0) < 0)
        if (errno != EINTR) { status = -1; break; }
    if (status != -1 && WIFEXITED(status)) code = WEXITSTATUS(status);
    if (bo.over || be.over) {
        char msg[200];
        snprintf(msg, sizeof(msg), "the command's %s is over the string limit of %d bytes", bo.over ? "stdout" : "stderr", GEM_MAX_STRLEN);
        *errmsg = strdup(msg);
    }
    if (!bo.data) bo.data = (char *)malloc(1);
    if (!be.data) be.data = (char *)malloc(1);
    bo.data[bo.len] = '\0';
    be.data[be.len] = '\0';
    *out = bo.data; *out_len = bo.len;
    *err = be.data; *err_len = be.len;
    return code;
}

/* ─── Built-in: exec_capture(command, input?) ─── */

GemVal gem_exec_capture_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_STRING) {
        char buf[128]; snprintf(buf, sizeof(buf), "exec_capture: expected string command, got %s", argc < 1 ? "nothing" : gem_type_str(args[0])); gem_error(buf);
    }
    const char *in = NULL;
    size_t in_len = 0;
    if (argc >= 2 && args[1].type != VAL_NIL) {
        if (args[1].type != VAL_STRING) {
            char buf[128]; snprintf(buf, sizeof(buf), "exec_capture: input must be a string, got %s", gem_type_str(args[1])); gem_error(buf);
        }
        in = args[1].sval;
        in_len = (size_t)args[1].slen;
    }
    int code;
    char *out, *err, *errmsg;
    size_t out_len, err_len;
    GemIORequest *req = NULL;
    if (gem_current_pid >= 0) {
        /* An empty input travels as no input: either way the child's
           stdin is at its end at once. */
        req = gem_io_submit(GEM_IO_EXEC_CAPTURE, args[0].sval, in_len ? in : NULL, in_len);
        if (!req) { gem_error("exec_capture: I/O queue full"); }
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        proc->io_request = req;
        gem_io_pool_yield();
        proc->io_request = NULL;
        code = req->exit_code;
        out = req->result_data; out_len = req->result_len;
        err = req->err_data; err_len = req->err_len;
        errmsg = req->error_msg;
    } else {
        code = gem_run_capture(args[0].sval, in, in_len, &out, &out_len, &err, &err_len, &errmsg);
    }
    if (errmsg) {
        char buf[300]; snprintf(buf, sizeof(buf), "exec_capture: %s", errmsg);
        if (req) gem_io_release(req); else { free(out); free(err); free(errmsg); }
        gem_error(buf);
    }
    GemVal r = gem_table_new();
    gem_table_set(r, gem_string("status"), gem_int(code));
    gem_table_set(r, gem_string("stdout"), gem_string_with_len(out ? out : "", (int64_t)out_len));
    gem_table_set(r, gem_string("stderr"), gem_string_with_len(err ? err : "", (int64_t)err_len));
    if (req) gem_io_release(req); else { free(out); free(err); }
    return r;
}

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

    return gem_int(gem_run_shell(args[0].sval));
}
