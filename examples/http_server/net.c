/*
 * net.c — Non-blocking POSIX socket wrappers for Gem's extern fn system.
 * Each function takes/returns GemVal. Ptrs are smuggled as ival (int64_t).
 *
 * When running inside the Gem scheduler (gem_current_pid >= 0), blocking
 * calls yield to other coroutines via gem_io_yield() instead of stalling
 * the whole process.
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include "gem.h"

static void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags >= 0) fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/* net_listen(port: Int) -> Int (fd)
   Creates a TCP socket, binds to 0.0.0.0:port, listens. Returns fd or -1.
   The listening socket is set non-blocking so accept() can yield. */
int64_t net_listen(int64_t port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return -1;

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons((uint16_t)port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(fd);
        return -1;
    }

    if (listen(fd, 1024) < 0) {
        close(fd);
        return -1;
    }

    set_nonblocking(fd);
    return (int64_t)fd;
}

/* net_accept(server_fd: Int) -> Int (client fd, or -1)
   Yields to the scheduler on EAGAIN instead of blocking. */
int64_t net_accept(int64_t server_fd) {
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    while (1) {
        int client_fd = accept((int)server_fd, (struct sockaddr *)&client_addr, &len);
        if (client_fd >= 0) {
            set_nonblocking(client_fd);
            return (int64_t)client_fd;
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            gem_io_yield((int)server_fd, 0);  /* wait for readable = incoming connection */
            continue;
        }
        return -1;  /* real error */
    }
}

/* net_read(fd: Int, max_bytes: Int) -> String (data read, or "" on error/EOF)
   Yields to the scheduler on EAGAIN instead of blocking. */
char *net_read(int64_t fd, int64_t max_bytes) {
    char *buf = (char *)GC_MALLOC_ATOMIC((size_t)max_bytes + 1);

    while (1) {
        ssize_t n = read((int)fd, buf, (size_t)max_bytes);
        if (n > 0) {
            buf[n] = '\0';
            return buf;
        }
        if (n == 0) {
            /* EOF */
            buf[0] = '\0';
            return buf;
        }
        /* n < 0 */
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            gem_io_yield((int)fd, 0);  /* wait for readable */
            continue;
        }
        buf[0] = '\0';
        return buf;  /* real error */
    }
}

/* net_write(fd: Int, data: String) -> Int (bytes written, or -1)
   Yields to the scheduler on EAGAIN instead of blocking. */
int64_t net_write(int64_t fd, char *data) {
    size_t total = strlen(data);
    size_t sent = 0;
    while (sent < total) {
        ssize_t n = write((int)fd, data + sent, total - sent);
        if (n > 0) {
            sent += (size_t)n;
            continue;
        }
        if (n == 0) continue;
        /* n < 0 */
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            gem_io_yield((int)fd, 1);  /* wait for writable */
            continue;
        }
        return -1;  /* real error */
    }
    return (int64_t)sent;
}

/* net_close(fd: Int) -> Nil */
void net_close(int64_t fd) {
    close((int)fd);
}
