/* The C side of walk.gem: a plain `extern fn`, one syscall, run inline. */

#include <sys/stat.h>

/* lstat: is_dir follows the link */
static int fs_is_symlink(const char *path) {
    struct stat st;
    return lstat(path, &st) == 0 && S_ISLNK(st.st_mode);
}
