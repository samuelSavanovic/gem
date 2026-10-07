/* fs.h — the C side of walk.gem: what the builtins don't say about a
 * path. A plain `extern fn`: one syscall, run inline. */

#include <sys/stat.h>

/* Whether `path` itself is a symbolic link (lstat; is_dir follows it). */
static int fs_is_symlink(const char *path) {
    struct stat st;
    return lstat(path, &st) == 0 && S_ISLNK(st.st_mode);
}
