/* For example 222: writes to a socket's fd, one after a delay, so a blocking call can still be using
 * a socket that Gem code closes. */

#include <stdint.h>
#include <string.h>
#include <unistd.h>

static int64_t gem_example_sock_write(int fd, const char *s) {
    return (int64_t)write(fd, s, strlen(s));
}

static int64_t gem_example_sock_write_later(int fd, int64_t ms, const char *s) {
    usleep((useconds_t)(ms * 1000));
    return (int64_t)write(fd, s, strlen(s));
}
