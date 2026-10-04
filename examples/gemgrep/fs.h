/* fs.h — the C side of walk.gem: what the builtins don't say about a
 * path. Plain `extern fn`s: each is one syscall, run inline. */

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/* Whether `path` itself is a symbolic link (lstat; is_dir follows it). */
static int fs_is_symlink(const char *path) {
    struct stat st;
    return lstat(path, &st) == 0 && S_ISLNK(st.st_mode);
}

/* Why `path` can't be opened for reading: strerror's text ("No such file
 * or directory"), or NULL (nil) when it can. read_file's error says only
 * "cannot open". strerror's buffer is static: copied, never freed. */
static char *fs_open_error(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd >= 0) {
        close(fd);
        return NULL;
    }
    return strerror(errno);
}
