/* write_count — counts a program's write (fd > 2), writev and send calls
 * and prints the totals to stderr at exit. macOS only (DYLD interposing).
 *
 *   cc -dynamiclib -O2 -o /tmp/write_count.dylib benchmarks/mini_redis/write_count.c
 *   build/gem examples/mini_redis/main.gem -o /tmp/mini_redis
 *   DYLD_INSERT_LIBRARIES=/tmp/write_count.dylib /tmp/mini_redis --port 6399 &
 *   python3 benchmarks/mini_redis/pubsub_bench.py 6399 100 2000
 *   redis-cli -p 6399 shutdown      # the totals print as the server exits
 */

#include <stdatomic.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

static atomic_long n_write, n_writev, n_send;

static ssize_t count_write(int fd, const void *buf, size_t n) {
    if (fd > 2) {
        n_write++;
    }
    return write(fd, buf, n);
}

static ssize_t count_writev(int fd, const struct iovec *iov, int cnt) {
    n_writev++;
    return writev(fd, iov, cnt);
}

static ssize_t count_send(int fd, const void *buf, size_t n, int flags) {
    n_send++;
    return send(fd, buf, n, flags);
}

__attribute__((used, section("__DATA,__interpose"))) static struct {
    const void *replacement, *original;
} interposers[] = {
    {(const void *)count_write, (const void *)write},
    {(const void *)count_writev, (const void *)writev},
    {(const void *)count_send, (const void *)send},
};

__attribute__((destructor)) static void report(void) {
    fprintf(stderr, "write_count: write(fd>2)=%ld writev=%ld send=%ld\n",
            (long)n_write, (long)n_writev, (long)n_send);
}
