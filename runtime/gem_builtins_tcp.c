/* TCP builtins. A socket is a resource value: look its entry up again after every wait, since any process
   (or the owner's exit) may have closed it and the fd number may already belong to another file.
   A socket with a non-NULL entry ptr is TLS and goes through gem_tls_ops. */

#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE   /* accept4 */
#endif
#include "gem.h"
#include "stb_ds.h"
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#ifdef __APPLE__
#include <libproc.h>
#endif

/* TLS code is only reached through this (set by tls_connect), so programs without it don't link OpenSSL */
const GemTlsOps *gem_tls_ops = NULL;

static void gem_set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags >= 0) fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/* Sockets are close-on-exec so `exec` children don't hold them. macOS has no SOCK_CLOEXEC, so there's a
   window before the fcntl; the exec builtins cover it with POSIX_SPAWN_CLOEXEC_DEFAULT. */
#ifdef SOCK_CLOEXEC
#define GEM_SOCK_STREAM (SOCK_STREAM | SOCK_CLOEXEC)
#else
#define GEM_SOCK_STREAM SOCK_STREAM
#endif

static void gem_set_cloexec(int fd) {
    fcntl(fd, F_SETFD, FD_CLOEXEC);
}

static int gem_accept(int server_fd, struct sockaddr *addr, socklen_t *len) {
#if defined(__linux__) && defined(SOCK_CLOEXEC)
    return accept4(server_fd, addr, len, SOCK_CLOEXEC);
#else
    return accept(server_fd, addr, len);
#endif
}

/* dev/ino behind fd, -1 if not open; macOS fstat gives every socket 0/0, so the kernel socket id is the inode */
static int gem_fd_identity(int fd, uint64_t *dev, uint64_t *ino) {
    struct stat st;
    if (fstat(fd, &st) < 0) return -1;
    *dev = (uint64_t)st.st_dev;
    *ino = (uint64_t)st.st_ino;
#ifdef __APPLE__
    if (S_ISSOCK(st.st_mode)) {
        struct socket_fdinfo si;
        if (proc_pidfdinfo(getpid(), fd, PROC_PIDFDSOCKETINFO, &si, sizeof(si)) == (int)sizeof(si))
            *ino = (uint64_t)si.psi.soi_so;
    }
#endif
    return 0;
}

/* records the fd's identity so gem_tcp_close_fd_checked never closes a file that reused the number */
static GemVal gem_tcp_register(int fd) {
    GemVal v = gem_res_new(GEM_RES_SOCKET);
    GemResEntry *e = gem_res_lookup(v);
    e->fd = fd;
    gem_fd_identity(fd, &e->dev, &e->ino);
    return v;
}

void gem_tcp_close_fd_checked(int fd, uint64_t dev, uint64_t ino) {
    uint64_t d, i;
    if (gem_fd_identity(fd, &d, &i) == 0 && d == dev && i == ino)
        close(fd);
}

static GemVal gem_tcp_arg0(GemVal *args, int argc) {
    return argc >= 1 ? args[0] : GEM_NIL;
}

int gem_tcp_port(GemVal v, const char *who) {
    if (v.ival < 0 || v.ival > 65535) {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s: port must be from 0 to 65535, got %lld", who, (long long)v.ival);
        gem_error(buf);
    }
    return (int)v.ival;
}

/* In-flight lookups by host, so concurrent askers share one (scheduler thread only). The table holds a ref
   on each request; done entries leave when a waiter resumes, when the name is asked again, or in a sweep
   once the table doubles (all the waiters may have timed out). */
typedef struct { char *key; GemIORequest *value; } GemLookupEntry;
static GemLookupEntry *gem_lookups = NULL;
static ptrdiff_t gem_lookups_sweep_at = 64;

static int gem_lookup_done(GemIORequest *req) {
    return __atomic_load_n(&req->done, __ATOMIC_ACQUIRE);
}

static void gem_lookup_forget(const char *host, GemIORequest *req) {
    if (shget(gem_lookups, host) != req) return;
    shdel(gem_lookups, host);
    gem_io_release(req);
}

/* returns a request with one ref for the caller, or NULL if it can't queue */
static GemIORequest *gem_lookup_start(const char *host) {
    if (!gem_lookups) sh_new_strdup(gem_lookups);
    GemIORequest *req = shget(gem_lookups, host);
    if (req && !gem_lookup_done(req)) {
        __sync_add_and_fetch(&req->refs, 1);
        return req;
    }
    if (req) gem_lookup_forget(host, req);
    req = gem_io_submit(GEM_IO_RESOLVE, host, NULL, 0);
    if (!req) return NULL;
    __sync_add_and_fetch(&req->refs, 1);
    shput(gem_lookups, host, req);
    if (shlen(gem_lookups) >= gem_lookups_sweep_at) {
        /* backwards: shdel moves the last entry into the hole */
        for (ptrdiff_t i = shlen(gem_lookups) - 1; i >= 0; i--) {
            GemIORequest *r = gem_lookups[i].value;
            if (!gem_lookup_done(r)) continue;
            shdel(gem_lookups, gem_lookups[i].key);
            gem_io_release(r);
        }
        ptrdiff_t n = shlen(gem_lookups) * 2;
        gem_lookups_sweep_at = n > 64 ? n : 64;
    }
    return req;
}

/* resolves on the resolver lane (inline outside a process); past `deadline` raise, leaving the lookup unread */
static void gem_tcp_resolve4(const char *host, int port, int64_t deadline,
                             struct in_addr *out, const char *who) {
    if (inet_pton(AF_INET, host, out) == 1) return;
    char buf[512];
    if (gem_current_pid < 0) {
        uint32_t a;
        if (gem_resolve4(host, &a) != NULL) {
            snprintf(buf, sizeof(buf), "%s: cannot resolve '%s'", who, host);
            gem_error(buf);
        }
        out->s_addr = a;
        return;
    }
    GemIORequest *req = gem_lookup_start(host);
    if (!req) {
        snprintf(buf, sizeof(buf), "%s: cannot resolve '%s': no thread for name lookups", who, host);
        gem_error(buf);
    }
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    proc->io_request = req;
    int done = gem_io_pool_yield_until(deadline);
    proc->io_request = NULL;
    if (!done) {
        snprintf(buf, sizeof(buf), "%s: connect to %s:%d timed out", who, host, port);
        gem_io_release(req);
        gem_error(buf);
    }
    gem_lookup_forget(host, req);
    if (req->error_msg) {
        snprintf(buf, sizeof(buf), "%s: cannot resolve '%s'", who, host);
        gem_io_release(req);
        gem_error(buf);
    }
    out->s_addr = req->addr4;
    gem_io_release(req);
}

/* clear deadline/timed_out a `receive ... after` or `sleep` may have left; a wait sets its own if it has a timeout */
static GemProcess *gem_tcp_begin_wait(void) {
    GemProcess *proc = &gem_proc_table[gem_current_pid];
    proc->deadline_ms = -1;
    proc->timed_out = 0;
    return proc;
}

/* shared by tcp_connect and tls_connect; deadline is gem_now_ms time or -1 */
GemVal gem_tcp_connect_to(const char *host, int port, int64_t deadline, const char *who) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    gem_tcp_resolve4(host, port, deadline, &addr.sin_addr, who);

    int fd = socket(AF_INET, GEM_SOCK_STREAM, 0);
    if (fd < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s: socket failed: %s", who, strerror(errno));
        gem_error(buf);
    }
    gem_set_cloexec(fd);
    /* register before waiting so a kill mid-connect still closes it */
    GemVal sock = gem_tcp_register(fd);

    if (gem_current_pid >= 0) {
        GemProcess *proc = gem_tcp_begin_wait();
        gem_set_nonblocking(fd);
        int rc = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
        if (rc < 0 && errno != EINPROGRESS) {
            char buf[256];
            snprintf(buf, sizeof(buf), "%s: connect failed: %s", who, strerror(errno));
            gem_res_close(gem_res_lookup(sock));
            gem_error(buf);
        }
        if (rc < 0) {
            proc->deadline_ms = deadline;
            gem_io_yield(fd, 1, sock);
            int timed_out = deadline >= 0 && (proc->timed_out || gem_now_ms() >= deadline);
            proc->timed_out = 0;
            proc->deadline_ms = -1;
            GemResEntry *e = gem_res_lookup(sock);
            char buf[512];
            if (!e) {
                snprintf(buf, sizeof(buf), "%s: connect failed: socket closed while connecting", who);
                gem_error(buf);
            }
            int err = 0;
            socklen_t errlen = sizeof(err);
            if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &errlen) < 0) err = errno;
            if (err != 0) {
                snprintf(buf, sizeof(buf), "%s: connect failed: %s", who, strerror(err));
                gem_res_close(e);
                gem_error(buf);
            }
            /* woken by the deadline: connected only if there's a peer */
            struct sockaddr_storage peer;
            socklen_t peer_len = sizeof(peer);
            if (timed_out && getpeername(fd, (struct sockaddr *)&peer, &peer_len) < 0) {
                snprintf(buf, sizeof(buf), "%s: connect to %s:%d timed out", who, host, port);
                gem_res_close(e);
                gem_error(buf);
            }
        }
        return sock;
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s: connect failed: %s", who, strerror(errno));
        gem_res_close(gem_res_lookup(sock));
        gem_error(buf);
    }
    return sock;
}

/* timeout_ms <= 0 gives up unless it connects at once */
GemVal gem_tcp_connect_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        gem_error("tcp_connect: expected (string host, int port[, int timeout_ms])");
    }
    int64_t deadline = -1;
    if (argc >= 3 && args[2].type == VAL_INT) {
        deadline = gem_now_ms() + (args[2].ival > 0 ? args[2].ival : 0);
    } else if (argc >= 3 && args[2].type != VAL_NIL) {
        char errbuf[160];
        snprintf(errbuf, sizeof(errbuf), "tcp_connect: timeout_ms must be an int (milliseconds) or nil, got %s",
                 gem_type_str(args[2]));
        gem_error(errbuf);
    }
    return gem_tcp_connect_to(args[0].sval, gem_tcp_port(args[1], "tcp_connect"), deadline, "tcp_connect");
}

GemVal gem_tcp_listen_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        gem_error("tcp_listen: expected (string host, int port)");
    }
    const char *host = args[0].sval;
    int port = gem_tcp_port(args[1], "tcp_listen");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    gem_tcp_resolve4(host, port, -1, &addr.sin_addr, "tcp_listen");

    int fd = socket(AF_INET, GEM_SOCK_STREAM, 0);
    if (fd < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "tcp_listen: socket failed: %s", strerror(errno));
        gem_error(buf);
    }
    gem_set_cloexec(fd);

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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
    return gem_tcp_register(fd);
}

GemVal gem_tcp_accept_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    GemVal lsock = gem_tcp_arg0(args, argc);
    int server_fd = gem_res_get(lsock, GEM_RES_SOCKET, "tcp_accept")->fd;

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if (gem_current_pid >= 0) {
        gem_tcp_begin_wait();
        while (1) {
            int fd = gem_accept(server_fd, (struct sockaddr *)&addr, &addr_len);
            if (fd >= 0) {
                gem_set_cloexec(fd);
                gem_set_nonblocking(fd);
                return gem_tcp_register(fd);
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                gem_io_yield(server_fd, 0, lsock);
                server_fd = gem_res_get(lsock, GEM_RES_SOCKET, "tcp_accept")->fd;
                continue;
            }
            char buf[256];
            snprintf(buf, sizeof(buf), "tcp_accept: accept failed: %s", strerror(errno));
            gem_error(buf);
        }
    }

    int fd = gem_accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    if (fd < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "tcp_accept: accept failed: %s", strerror(errno));
        gem_error(buf);
    }
    gem_set_cloexec(fd);
    return gem_tcp_register(fd);
}

GemVal gem_tcp_read_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    GemVal sock = gem_tcp_arg0(args, argc);
    GemResEntry *ent = gem_res_get(sock, GEM_RES_SOCKET, "tcp_read");
    int fd = ent->fd;
    size_t max_bytes = 4096;
    if (argc >= 2 && args[1].type == VAL_INT && args[1].ival > 0) {
        max_bytes = (size_t)args[1].ival;
    } else if (argc >= 2 && args[1].type != VAL_NIL) {
        char errbuf[160];
        if (args[1].type == VAL_INT)
            snprintf(errbuf, sizeof(errbuf), "tcp_read: max_bytes must be an int > 0 or nil, got %lld",
                     (long long)args[1].ival);
        else
            snprintf(errbuf, sizeof(errbuf), "tcp_read: max_bytes must be an int > 0 or nil, got %s",
                     gem_type_str(args[1]));
        gem_error(errbuf);
    }
    /* timeout_ms <= 0 only takes what's already there, like `after 0` */
    int has_timeout = 0;
    int64_t timeout_ms = 0;
    if (argc >= 3 && args[2].type == VAL_INT) {
        has_timeout = 1;
        timeout_ms = args[2].ival;
    } else if (argc >= 3 && args[2].type != VAL_NIL) {
        char errbuf[160];
        snprintf(errbuf, sizeof(errbuf), "tcp_read: timeout_ms must be an int (milliseconds) or nil, got %s",
                 gem_type_str(args[2]));
        gem_error(errbuf);
    }
    if (ent->ptr) return gem_tls_ops->read(sock, max_bytes, has_timeout, timeout_ms);

    if (gem_current_pid >= 0) {
        GemProcess *proc = gem_tcp_begin_wait();
        int own_deadline = has_timeout && timeout_ms > 0;

        if (proc->read_buf_cap < max_bytes) {
            proc->read_buf = (char *)gem_alloc(max_bytes);
            proc->read_buf_cap = max_bytes;
        }
        char *buf = proc->read_buf;

        int64_t deadline = own_deadline ? gem_now_ms() + timeout_ms : -1;
        proc->deadline_ms = deadline;
        while (1) {
            ssize_t n = read(fd, buf, max_bytes);
            if (n > 0) {
                proc->deadline_ms = -1;
                char *s = (char *)gem_alloc(n + 1);
                memcpy(s, buf, n);
                s[n] = '\0';
                GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = s; r.slen = (int)n;
                return r;
            }
            if (n == 0) {
                proc->deadline_ms = -1;
                GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = ""; r.slen = 0;
                return r;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                if (has_timeout && timeout_ms <= 0) return GEM_NIL;
                gem_io_yield(fd, 0, sock);
                GemResEntry *e = gem_res_lookup(sock);
                if (!e) {
                    proc->timed_out = 0;
                    proc->deadline_ms = -1;
                    gem_error("tcp_read: socket is closed");
                }
                fd = e->fd;
                if (own_deadline && (proc->timed_out || gem_now_ms() >= deadline)) {
                    proc->timed_out = 0;
                    proc->deadline_ms = -1;
                    /* nil = timeout, "" = EOF */
                    return GEM_NIL;
                }
                proc->timed_out = 0;
                continue;
            }
            if (errno == ECONNRESET) {
                proc->deadline_ms = -1;
                GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = ""; r.slen = 0;
                return r;
            }
            proc->deadline_ms = -1;
            char errbuf[256];
            snprintf(errbuf, sizeof(errbuf), "tcp_read: read failed: %s", strerror(errno));
            gem_error(errbuf);
        }
    }

    char *buf = (char *)gem_alloc(max_bytes + 1);
    ssize_t n = read(fd, buf, max_bytes);
    if (n < 0) {
        char errbuf[256];
        snprintf(errbuf, sizeof(errbuf), "tcp_read: read failed: %s", strerror(errno));
        gem_error(errbuf);
    }
    buf[n] = '\0';
    GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = buf; r.slen = (int)n;
    return r;
}

GemVal gem_tcp_write_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    GemVal sock = gem_tcp_arg0(args, argc);
    GemResEntry *ent = gem_res_get(sock, GEM_RES_SOCKET, "tcp_write");
    int fd = ent->fd;
    if (argc < 2 || (args[1].type != VAL_STRING && args[1].type != VAL_BUFFER)) {
        gem_error("tcp_write: expected (socket, string|buffer data[, int timeout_ms])");
    }
    /* timeout_ms covers the whole write; on timeout return the count so far */
    int has_timeout = 0;
    int64_t timeout_ms = 0;
    if (argc >= 3 && args[2].type == VAL_INT) {
        has_timeout = 1;
        timeout_ms = args[2].ival;
    } else if (argc >= 3 && args[2].type != VAL_NIL) {
        char errbuf[160];
        snprintf(errbuf, sizeof(errbuf), "tcp_write: timeout_ms must be an int (milliseconds) or nil, got %s",
                 gem_type_str(args[2]));
        gem_error(errbuf);
    }
    const char *data;
    size_t total;
    if (args[1].type == VAL_STRING) {
        data = args[1].sval;
        total = (size_t)args[1].slen;
    } else {
        data = args[1].buffer->data;
        total = (size_t)args[1].buffer->len;
    }
    if (ent->ptr) return gem_tls_ops->write(sock, data, total, has_timeout, timeout_ms);

    if (gem_current_pid >= 0) {
        GemProcess *proc = gem_tcp_begin_wait();
        int own_deadline = has_timeout && timeout_ms > 0;
        int64_t deadline = own_deadline ? gem_now_ms() + timeout_ms : -1;
        proc->deadline_ms = deadline;
        size_t sent = 0;
        while (sent < total) {
            ssize_t n = write(fd, data + sent, total - sent);
            if (n > 0) {
                sent += (size_t)n;
                continue;
            }
            if (n == 0) continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                if (has_timeout && timeout_ms <= 0) break;
                gem_io_yield(fd, 1, sock);
                GemResEntry *e = gem_res_lookup(sock);
                if (!e) {
                    proc->timed_out = 0;
                    proc->deadline_ms = -1;
                    gem_error("tcp_write: socket is closed");
                }
                fd = e->fd;
                if (own_deadline && (proc->timed_out || gem_now_ms() >= deadline)) {
                    proc->timed_out = 0;
                    break;
                }
                proc->timed_out = 0;
                continue;
            }
            proc->deadline_ms = -1;
            if (errno == EPIPE || errno == ECONNRESET) {
                return gem_int((int64_t)sent);
            }
            char buf[256];
            snprintf(buf, sizeof(buf), "tcp_write: write failed: %s", strerror(errno));
            gem_error(buf);
        }
        proc->deadline_ms = -1;
        return gem_int((int64_t)sent);
    }

    size_t sent = 0;
    while (sent < total) {
        ssize_t n = write(fd, data + sent, total - sent);
        if (n < 0) {
            if (errno == EPIPE || errno == ECONNRESET) {
                return gem_int((int64_t)sent);
            }
            char buf[256];
            snprintf(buf, sizeof(buf), "tcp_write: write failed: %s", strerror(errno));
            gem_error(buf);
        }
        sent += (size_t)n;
    }
    return gem_int((int64_t)total);
}

GemVal gem_tcp_close_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    GemResEntry *e = gem_res_get_open(gem_tcp_arg0(args, argc), GEM_RES_SOCKET, "tcp_close");
    if (e) gem_res_close(e);
    return GEM_NIL;
}

/* nil for a listening socket or a peer that already reset (Linux ENOTCONN, macOS EINVAL) */
GemVal gem_tcp_peer_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    int fd = gem_res_get(gem_tcp_arg0(args, argc), GEM_RES_SOCKET, "tcp_peer")->fd;
    int sock_type = 0;
    socklen_t type_len = sizeof(sock_type);
    if (getsockopt(fd, SOL_SOCKET, SO_TYPE, &sock_type, &type_len) == 0 && sock_type != SOCK_STREAM) {
        gem_error("tcp_peer: not a TCP socket");
    }
    struct sockaddr_storage addr;
    socklen_t addr_len = sizeof(addr);
    if (getpeername(fd, (struct sockaddr *)&addr, &addr_len) < 0) {
        if (errno == ENOTCONN || errno == EINVAL) return GEM_NIL;
        char buf[256];
        snprintf(buf, sizeof(buf), "tcp_peer: getpeername failed: %s", strerror(errno));
        gem_error(buf);
    }
    char ip[INET6_ADDRSTRLEN] = "";
    int port = 0;
    if (addr.ss_family == AF_INET) {
        struct sockaddr_in *a = (struct sockaddr_in *)&addr;
        inet_ntop(AF_INET, &a->sin_addr, ip, sizeof(ip));
        port = ntohs(a->sin_port);
    } else if (addr.ss_family == AF_INET6) {
        struct sockaddr_in6 *a = (struct sockaddr_in6 *)&addr;
        inet_ntop(AF_INET6, &a->sin6_addr, ip, sizeof(ip));
        port = ntohs(a->sin6_port);
    } else {
        gem_error("tcp_peer: not a TCP socket");
    }
    GemVal result = gem_table_new();
    gem_table_set(result, gem_string("ip"), gem_string(ip));
    gem_table_set(result, gem_string("port"), gem_int(port));
    return result;
}

GemVal gem_tcp_local_port_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    int fd = gem_res_get(gem_tcp_arg0(args, argc), GEM_RES_SOCKET, "tcp_local_port")->fd;
    struct sockaddr_storage addr;
    socklen_t addr_len = sizeof(addr);
    if (getsockname(fd, (struct sockaddr *)&addr, &addr_len) < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "tcp_local_port: getsockname failed: %s", strerror(errno));
        gem_error(buf);
    }
    if (addr.ss_family == AF_INET) return gem_int(ntohs(((struct sockaddr_in *)&addr)->sin_port));
    if (addr.ss_family == AF_INET6) return gem_int(ntohs(((struct sockaddr_in6 *)&addr)->sin6_port));
    gem_error("tcp_local_port: not a TCP socket");
    return GEM_NIL;
}

/* for extern fns; C code must not close it. A TLS fd carries ciphertext, so those are refused. */
GemVal gem_tcp_fd_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    GemResEntry *e = gem_res_get(gem_tcp_arg0(args, argc), GEM_RES_SOCKET, "tcp_fd");
    if (e->ptr) gem_error("tcp_fd: a TLS socket has no fd to give to C");
    return gem_int(e->fd);
}

/* adopt a socket made by C code or inherited; the runtime owns the fd after */
GemVal gem_tcp_from_fd_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    GemVal a = gem_tcp_arg0(args, argc);
    char buf[160];
    if (a.type != VAL_INT) {
        snprintf(buf, sizeof(buf), "tcp_from_fd: expected an int fd, got %s", gem_type_str(a));
        gem_error(buf);
    }
    uint64_t dev, ino;
    if (a.ival < 0 || a.ival > INT_MAX || gem_fd_identity((int)a.ival, &dev, &ino) < 0) {
        snprintf(buf, sizeof(buf), "tcp_from_fd: fd %lld is not open", (long long)a.ival);
        gem_error(buf);
    }
    int fd = (int)a.ival;
    int sock_type = 0;
    socklen_t type_len = sizeof(sock_type);
    struct sockaddr_storage addr;
    socklen_t addr_len = sizeof(addr);
    if (getsockopt(fd, SOL_SOCKET, SO_TYPE, &sock_type, &type_len) < 0 || sock_type != SOCK_STREAM ||
        getsockname(fd, (struct sockaddr *)&addr, &addr_len) < 0 ||
        (addr.ss_family != AF_INET && addr.ss_family != AF_INET6)) {
        snprintf(buf, sizeof(buf), "tcp_from_fd: fd %d is not a TCP socket", fd);
        gem_error(buf);
    }
    if (gem_res_find_socket(fd, dev, ino)) {
        snprintf(buf, sizeof(buf), "tcp_from_fd: fd %d is already registered as a socket", fd);
        gem_error(buf);
    }
    gem_set_cloexec(fd);
    gem_set_nonblocking(fd);
    return gem_tcp_register(fd);
}
