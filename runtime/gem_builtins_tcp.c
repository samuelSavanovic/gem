/*
 * gem_builtins_tcp.c — TCP socket builtins: tcp_listen, tcp_connect,
 *                       tcp_accept, tcp_read, tcp_write, tcp_close.
 */

#include "gem.h"
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>

static void gem_set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags >= 0) fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/* ─── Built-in: tcp_connect ─── */

GemVal gem_tcp_connect_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        gem_error("tcp_connect: expected (string host, int port)");
    }
    const char *host = args[0].sval;
    int port = (int)args[1].ival;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "tcp_connect: socket failed: %s", strerror(errno));
        gem_error(buf);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);

    if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
        struct hostent *he = gethostbyname(host);
        if (!he) {
            close(fd);
            char buf[256];
            snprintf(buf, sizeof(buf), "tcp_connect: cannot resolve '%s'", host);
            gem_error(buf);
        }
        memcpy(&addr.sin_addr, he->h_addr_list[0], (size_t)he->h_length);
    }

    if (gem_current_pid >= 0) {
        gem_set_nonblocking(fd);
        int rc = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
        if (rc < 0 && errno != EINPROGRESS) {
            close(fd);
            char buf[256];
            snprintf(buf, sizeof(buf), "tcp_connect: connect failed: %s", strerror(errno));
            gem_error(buf);
        }
        if (rc < 0) {
            gem_io_yield(fd, 1);
            int err = 0;
            socklen_t errlen = sizeof(err);
            getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &errlen);
            if (err != 0) {
                close(fd);
                char buf[256];
                snprintf(buf, sizeof(buf), "tcp_connect: connect failed: %s", strerror(err));
                gem_error(buf);
            }
        }
        return gem_int(fd);
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(fd);
        char buf[256];
        snprintf(buf, sizeof(buf), "tcp_connect: connect failed: %s", strerror(errno));
        gem_error(buf);
    }
    return gem_int(fd);
}

/* ─── Built-in: tcp_listen ─── */

GemVal gem_tcp_listen_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        gem_error("tcp_listen: expected (string host, int port)");
    }
    const char *host = args[0].sval;
    int port = (int)args[1].ival;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "tcp_listen: socket failed: %s", strerror(errno));
        gem_error(buf);
    }

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);

    if (strcmp(host, "0.0.0.0") == 0) {
        addr.sin_addr.s_addr = INADDR_ANY;
    } else {
        if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
            close(fd);
            char buf[256];
            snprintf(buf, sizeof(buf), "tcp_listen: invalid address '%s'", host);
            gem_error(buf);
        }
    }

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(fd);
        char buf[256];
        snprintf(buf, sizeof(buf), "tcp_listen: bind failed on port %d: %s", port, strerror(errno));
        gem_error(buf);
    }

    if (listen(fd, 1024) < 0) {
        close(fd);
        char buf[256];
        snprintf(buf, sizeof(buf), "tcp_listen: listen failed: %s", strerror(errno));
        gem_error(buf);
    }

    gem_set_nonblocking(fd);
    return gem_int(fd);
}

/* ─── Built-in: tcp_accept ─── */

GemVal gem_tcp_accept_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("tcp_accept: expected int socket fd");
    }
    int server_fd = (int)args[0].ival;

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if (gem_current_pid >= 0) {
        while (1) {
            int fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
            if (fd >= 0) {
                gem_set_nonblocking(fd);
                return gem_int(fd);
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                gem_io_yield(server_fd, 0);
                continue;
            }
            char buf[256];
            snprintf(buf, sizeof(buf), "tcp_accept: accept failed: %s", strerror(errno));
            gem_error(buf);
        }
    }

    int fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    if (fd < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "tcp_accept: accept failed: %s", strerror(errno));
        gem_error(buf);
    }
    return gem_int(fd);
}

/* ─── Built-in: tcp_read ─── */

GemVal gem_tcp_read_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("tcp_read: expected (int socket_fd[, int max_bytes])");
    }
    int fd = (int)args[0].ival;
    size_t max_bytes = 4096;
    if (argc >= 2 && args[1].type == VAL_INT) {
        max_bytes = (size_t)args[1].ival;
    }

    char *buf = (char *)GC_MALLOC_ATOMIC(max_bytes + 1);

    if (gem_current_pid >= 0) {
        while (1) {
            ssize_t n = read(fd, buf, max_bytes);
            if (n > 0) {
                buf[n] = '\0';
                GemVal r; r.type = VAL_STRING; r.sval = buf;
                return r;
            }
            if (n == 0) {
                buf[0] = '\0';
                GemVal r; r.type = VAL_STRING; r.sval = buf;
                return r;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                gem_io_yield(fd, 0);
                continue;
            }
            char errbuf[256];
            snprintf(errbuf, sizeof(errbuf), "tcp_read: read failed: %s", strerror(errno));
            gem_error(errbuf);
        }
    }

    ssize_t n = read(fd, buf, max_bytes);
    if (n < 0) {
        char errbuf[256];
        snprintf(errbuf, sizeof(errbuf), "tcp_read: read failed: %s", strerror(errno));
        gem_error(errbuf);
    }
    buf[n] = '\0';
    GemVal r; r.type = VAL_STRING; r.sval = buf;
    return r;
}

/* ─── Built-in: tcp_write ─── */

GemVal gem_tcp_write_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_INT || args[1].type != VAL_STRING) {
        gem_error("tcp_write: expected (int socket_fd, string data)");
    }
    int fd = (int)args[0].ival;
    const char *data = args[1].sval;
    size_t total = strlen(data);

    if (gem_current_pid >= 0) {
        size_t sent = 0;
        while (sent < total) {
            ssize_t n = write(fd, data + sent, total - sent);
            if (n > 0) {
                sent += (size_t)n;
                continue;
            }
            if (n == 0) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                gem_io_yield(fd, 1);
                continue;
            }
            char buf[256];
            snprintf(buf, sizeof(buf), "tcp_write: write failed: %s", strerror(errno));
            gem_error(buf);
        }
        return gem_int((int64_t)sent);
    }

    size_t sent = 0;
    while (sent < total) {
        ssize_t n = write(fd, data + sent, total - sent);
        if (n < 0) {
            char buf[256];
            snprintf(buf, sizeof(buf), "tcp_write: write failed: %s", strerror(errno));
            gem_error(buf);
        }
        sent += (size_t)n;
    }
    return gem_int((int64_t)total);
}

/* ─── Built-in: tcp_close ─── */

GemVal gem_tcp_close_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 1 || args[0].type != VAL_INT) {
        gem_error("tcp_close: expected int socket fd");
    }
    close((int)args[0].ival);
    return GEM_NIL;
}
