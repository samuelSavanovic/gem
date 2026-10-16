/* tls_connect, plus read/write/close for the TLS sockets it returns (through gem_tls_ops). GEM_TLS is
   defined by the build when tls_config.sh finds OpenSSL 3+; without it tls_connect just raises.

   A TLS socket is an ordinary socket resource with a GemTls in its ptr, so close, exit close and claim come
   for free. The fd stays non-blocking: on WANT_READ/WANT_WRITE we gem_io_yield and retry, re-looking up the
   entry after every wait since the socket may have been closed (and the session freed) meanwhile. Clear
   OpenSSL's error queue before each call. */

#include "gem.h"

#ifndef GEM_TLS

GemVal gem_tls_connect_fn(void *_env, GemVal *args, int argc) {
    (void)_env; (void)args; (void)argc;
    gem_error("tls_connect: this Gem was built without OpenSSL 3; install it "
              "(libssl-dev, openssl-devel or `brew install openssl@3`) and run `make build` again");
    return GEM_NIL;
}

#else

#include <errno.h>
#include <poll.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509v3.h>

#if OPENSSL_VERSION_NUMBER < 0x30000000L || defined(LIBRESSL_VERSION_NUMBER)
#error "gem_builtins_tls.c needs OpenSSL 3 or later"
#endif

/* max plaintext per SSL_write: one record, so at most one is ever in flight */
#define GEM_TLS_CHUNK 16384

typedef struct {
    SSL *ssl;
    int failed;   /* fatal error seen: skip close_notify */
    /* plaintext of an SSL_write a timed-out tcp_write left unfinished. OpenSSL wants the retry to pass the
       same bytes, so the next tcp_write starts with them (they weren't in the count returned). */
    char *pend;
    int pend_len;
} GemTls;

/* system trust store context, made on first use */
static SSL_CTX *gem_tls_default_ctx = NULL;

static const char *gem_tls_reason(const char *fallback) {
    unsigned long code = ERR_peek_last_error();
    const char *r = code ? ERR_reason_error_string(code) : NULL;
    return r ? r : fallback;
}

static SSL_CTX *gem_tls_new_ctx(void) {
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) return NULL;
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_mode(ctx, SSL_MODE_ENABLE_PARTIAL_WRITE | SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER |
                              SSL_MODE_RELEASE_BUFFERS);
    return ctx;
}

/* deadline is gem_now_ms time or -1. 1 on timeout; raises if the socket got closed while we waited. */
static int gem_tls_wait(GemVal sock, int fd, int for_write, int64_t deadline, const char *who) {
    int timed_out;
    if (gem_current_pid >= 0) {
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        proc->timed_out = 0;
        proc->deadline_ms = deadline;
        gem_io_yield(fd, for_write, sock);
        timed_out = deadline >= 0 && (proc->timed_out || gem_now_ms() >= deadline);
        proc->timed_out = 0;
        proc->deadline_ms = -1;
    } else {
        struct pollfd p = {fd, for_write ? POLLOUT : POLLIN, 0};
        int ms = -1;
        if (deadline >= 0) {
            int64_t left = deadline - gem_now_ms();
            ms = left > 0 ? (int)(left < INT_MAX ? left : INT_MAX) : 0;
        }
        while (poll(&p, 1, ms) < 0 && errno == EINTR) {}
        timed_out = deadline >= 0 && gem_now_ms() >= deadline;
    }
    if (!gem_res_lookup(sock)) {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s: socket is closed", who);
        gem_error(buf);
    }
    return timed_out;
}

static GemTls *gem_tls_of(GemVal sock, int *fd, const char *who) {
    GemResEntry *e = gem_res_get(sock, GEM_RES_SOCKET, who);
    *fd = e->fd;
    return (GemTls *)e->ptr;
}

/* stream ended without the server's close alert */
static void gem_tls_cut(const char *how) {
    char msg[256];
    snprintf(msg, sizeof(msg),
             "tcp_read: the TLS connection was %s before the server's close alert; the data may be cut short", how);
    gem_error(msg);
}

/* SSL_read returns at most one record (16 KB), so buf never needs to be bigger */
static GemVal gem_tls_read(GemVal sock, size_t max_bytes, int has_timeout, int64_t timeout_ms) {
    int64_t deadline = has_timeout && timeout_ms > 0 ? gem_now_ms() + timeout_ms : -1;
    char buf[GEM_TLS_CHUNK];
    int want = max_bytes < sizeof(buf) ? (int)max_bytes : (int)sizeof(buf);
    while (1) {
        int fd;
        GemTls *t = gem_tls_of(sock, &fd, "tcp_read");
        ERR_clear_error();
        errno = 0;
        int n = SSL_read(t->ssl, buf, want);
        if (n > 0) {
            char *s = (char *)gem_alloc((size_t)n + 1);
            memcpy(s, buf, (size_t)n);
            s[n] = '\0';
            GemVal r; r.type = VAL_STRING; r.magic = GEM_MAGIC; r.sval = s; r.slen = n;
            return r;
        }
        int err = SSL_get_error(t->ssl, n);
        if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
            if (has_timeout && timeout_ms <= 0) return GEM_NIL;
            if (gem_tls_wait(sock, fd, err == SSL_ERROR_WANT_WRITE, deadline, "tcp_read")) return GEM_NIL;
            continue;
        }
        if (err == SSL_ERROR_ZERO_RETURN) {
            GemVal eof; eof.type = VAL_STRING; eof.magic = GEM_MAGIC; eof.sval = ""; eof.slen = 0;
            return eof;
        }
        t->failed = 1;
        if (err == SSL_ERROR_SSL && ERR_GET_REASON(ERR_peek_last_error()) == SSL_R_UNEXPECTED_EOF_WHILE_READING)
            gem_tls_cut("closed");
        if (err == SSL_ERROR_SYSCALL && errno == 0) gem_tls_cut("closed");
        if (err == SSL_ERROR_SYSCALL && errno == ECONNRESET) gem_tls_cut("reset");
        char msg[256];
        if (err == SSL_ERROR_SYSCALL)
            snprintf(msg, sizeof(msg), "tcp_read: read failed: %s", strerror(errno));
        else
            snprintf(msg, sizeof(msg), "tcp_read: TLS error: %s", gem_tls_reason("unknown error"));
        gem_error(msg);
    }
}

static void gem_tls_drop_pend(GemTls *t) {
    free(t->pend);
    t->pend = NULL;
    t->pend_len = 0;
}

static GemVal gem_tls_write(GemVal sock, const char *data, size_t total, int has_timeout, int64_t timeout_ms) {
    int64_t deadline = has_timeout && timeout_ms > 0 ? gem_now_ms() + timeout_ms : -1;
    int fd;
    GemTls *t = gem_tls_of(sock, &fd, "tcp_write");
    if (t->pend && total > 0 && (total < (size_t)t->pend_len || memcmp(data, t->pend, (size_t)t->pend_len) != 0)) {
        char msg[256];
        snprintf(msg, sizeof(msg),
                 "tcp_write: an earlier tcp_write on this TLS socket timed out with %d bytes in flight; "
                 "the next write must start with the bytes it didn't count",
                 t->pend_len);
        gem_error(msg);
    }
    size_t sent = 0;
    while (sent < total) {
        t = gem_tls_of(sock, &fd, "tcp_write");
        /* retry the pending record from the caller's data (same bytes) */
        size_t chunk = t->pend ? (size_t)t->pend_len : total - sent;
        if (chunk > GEM_TLS_CHUNK) chunk = GEM_TLS_CHUNK;
        ERR_clear_error();
        errno = 0;
        int n = SSL_write(t->ssl, data + sent, (int)chunk);
        if (n > 0) {
            sent += (size_t)n;
            if (t->pend) gem_tls_drop_pend(t);
            continue;
        }
        int err = SSL_get_error(t->ssl, n);
        if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
            int stop = has_timeout && timeout_ms <= 0;
            if (!stop) {
                stop = gem_tls_wait(sock, fd, err == SSL_ERROR_WANT_WRITE, deadline, "tcp_write");
                t = gem_tls_of(sock, &fd, "tcp_write");
            }
            if (!stop) continue;
            if (!t->pend) {
                t->pend = (char *)malloc(chunk);
                if (!t->pend) gem_error("tcp_write: out of memory");
                memcpy(t->pend, data + sent, chunk);
                t->pend_len = (int)chunk;
            }
            break;
        }
        t->failed = 1;
        gem_tls_drop_pend(t);
        /* peer gone: return the count so far, like plain TCP */
        if (err == SSL_ERROR_ZERO_RETURN ||
            (err == SSL_ERROR_SYSCALL && (errno == 0 || errno == EPIPE || errno == ECONNRESET)))
            break;
        char msg[256];
        if (err == SSL_ERROR_SYSCALL)
            snprintf(msg, sizeof(msg), "tcp_write: write failed: %s", strerror(errno));
        else
            snprintf(msg, sizeof(msg), "tcp_write: TLS error: %s", gem_tls_reason("unknown error"));
        gem_error(msg);
    }
    return gem_int((int64_t)sent);
}

/* close() with unread input (TLS 1.3 tickets, say) sends a RST and can lose our tail: discard up to 1 MB */
static void gem_tls_drain(int fd) {
    char junk[16384];
    for (int i = 0; i < 64; i++) {
        ssize_t n = recv(fd, junk, sizeof(junk), MSG_DONTWAIT);
        if (n > 0) continue;
        if (n < 0 && errno == EINTR) continue;
        break;
    }
}

static void gem_tls_close(void *p) {
    GemTls *t = (GemTls *)p;
    int fd = SSL_get_fd(t->ssl);
    if (fd >= 0) gem_tls_drain(fd);
    if (!t->failed && SSL_is_init_finished(t->ssl)) {
        ERR_clear_error();
        SSL_shutdown(t->ssl);   /* one try, fd is non-blocking */
    }
    SSL_free(t->ssl);
    free(t->pend);
    free(t);
    ERR_clear_error();
}

static const GemTlsOps gem_tls_ops_impl = {gem_tls_read, gem_tls_write, gem_tls_close};

/* `dirs` is colon-separated; OpenSSL reads cert dirs lazily, so an empty store says nothing: look ourselves */
static int gem_tls_dirs_empty(const char *dirs) {
    char path[4096];
    const char *p = dirs;
    while (*p) {
        size_t n = strcspn(p, ":");
        if (n > 0 && n < sizeof(path)) {
            memcpy(path, p, n);
            path[n] = '\0';
            DIR *d = opendir(path);
            if (d) {
                struct dirent *de;
                int any = 0;
                while (!any && (de = readdir(d)) != NULL)
                    any = strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0;
                closedir(d);
                if (any) return 0;
            }
        }
        p += n;
        if (*p == ':') p++;
    }
    return 1;
}

static int gem_tls_store_empty(X509_STORE *store) {
#if OPENSSL_VERSION_NUMBER >= 0x30300000L
    STACK_OF(X509_OBJECT) *objs = X509_STORE_get1_objects(store);
    int empty = !objs || sk_X509_OBJECT_num(objs) == 0;
    sk_X509_OBJECT_pop_free(objs, X509_OBJECT_free);
    return empty;
#else
    return sk_X509_OBJECT_num(X509_STORE_get0_objects(store)) == 0;
#endif
}

/* closing the socket frees the session too */
static void gem_tls_fail(GemVal sock, const char *msg) {
    GemResEntry *e = gem_res_lookup(sock);
    if (e) gem_res_close(e);
    ERR_clear_error();
    gem_error(msg);
}

static int gem_tls_is_ip(const char *s) {
    unsigned char addr[16];
    return inet_pton(AF_INET, s, addr) == 1 || inet_pton(AF_INET6, s, addr) == 1;
}

static const char *gem_tls_opt_string(GemVal opts, const char *key) {
    if (opts.type != VAL_TABLE) return NULL;
    GemVal v = gem_table_get(opts, gem_string(key));
    if (v.type == VAL_NIL) return NULL;
    if (v.type != VAL_STRING || v.slen == 0 || (int)strlen(v.sval) != v.slen) {
        char buf[160];
        snprintf(buf, sizeof(buf), "tls_connect: %s must be a non-empty string without NUL bytes, got %s", key,
                 v.type == VAL_STRING ? "another string" : gem_type_str(v));
        gem_error(buf);
    }
    return v.sval;
}

GemVal gem_tls_connect_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    if (argc < 2 || args[0].type != VAL_STRING || args[1].type != VAL_INT) {
        gem_error("tls_connect: expected (string host, int port[, table opts])");
    }
    const char *host = args[0].sval;
    int port = gem_tcp_port(args[1], "tls_connect");
    GemVal opts = argc >= 3 ? args[2] : GEM_NIL;
    char buf[2048];
    if (opts.type != VAL_TABLE && opts.type != VAL_NIL) {
        snprintf(buf, sizeof(buf), "tls_connect: opts must be a table, got %s", gem_type_str(opts));
        gem_error(buf);
    }
    static const char TLS_OPTS[] = "timeout_ms\0ca_file\0server_name";
    gem_check_opt_keys(opts, "tls_connect", TLS_OPTS, (int)sizeof(TLS_OPTS) - 1);
    int64_t timeout_ms = -1;
    if (opts.type == VAL_TABLE) {
        GemVal t = gem_table_get(opts, gem_string("timeout_ms"));
        if (t.type == VAL_INT && t.ival > 0) {
            timeout_ms = t.ival;
        } else if (t.type == VAL_INT) {
            snprintf(buf, sizeof(buf), "tls_connect: timeout_ms must be an int > 0 (milliseconds) or nil, got %lld",
                     (long long)t.ival);
            gem_error(buf);
        } else if (t.type != VAL_NIL) {
            snprintf(buf, sizeof(buf), "tls_connect: timeout_ms must be an int > 0 (milliseconds) or nil, got %s",
                     gem_type_str(t));
            gem_error(buf);
        }
    }
    const char *ca_file = gem_tls_opt_string(opts, "ca_file");
    const char *name = gem_tls_opt_string(opts, "server_name");
    if (!name) name = host;

    int64_t deadline = timeout_ms > 0 ? gem_now_ms() + timeout_ms : -1;
    GemVal sock = gem_tcp_connect_to(host, port, deadline, "tls_connect");

    SSL_CTX *ctx;
    if (ca_file) {
        ctx = gem_tls_new_ctx();
        if (!ctx) gem_tls_fail(sock, "tls_connect: cannot create a TLS context");
        ERR_clear_error();
        int open_errno = 0;
        FILE *f = gem_fopen(ca_file, "r");
        if (f) fclose(f); else open_errno = errno;
        if (open_errno || SSL_CTX_load_verify_locations(ctx, ca_file, NULL) != 1) {
            snprintf(buf, sizeof(buf), "tls_connect: cannot load ca_file '%s': %s", ca_file,
                     open_errno ? strerror(open_errno) : gem_tls_reason("no certificates in it"));
            SSL_CTX_free(ctx);
            gem_tls_fail(sock, buf);
        }
    } else {
        if (!gem_tls_default_ctx) {
            SSL_CTX *c = gem_tls_new_ctx();
            if (!c) gem_tls_fail(sock, "tls_connect: cannot create a TLS context");
            SSL_CTX_set_default_verify_paths(c);
            ERR_clear_error();
            gem_tls_default_ctx = c;
        }
        ctx = gem_tls_default_ctx;
    }

    GemResEntry *e = gem_res_lookup(sock);
    SSL *ssl = SSL_new(ctx);
    if (ca_file) SSL_CTX_free(ctx);   /* the session holds a reference */
    GemTls *t = ssl ? (GemTls *)malloc(sizeof(GemTls)) : NULL;
    if (!t) {
        if (ssl) SSL_free(ssl);
        gem_tls_fail(sock, "tls_connect: cannot create a TLS session");
    }
    t->ssl = ssl;
    t->failed = 0;
    t->pend = NULL;
    t->pend_len = 0;
    gem_tls_ops = &gem_tls_ops_impl;
    e->ptr = t;   /* closing the socket frees the session from here on */

    if (SSL_set_fd(ssl, e->fd) != 1) gem_tls_fail(sock, "tls_connect: cannot create a TLS session");
    /* SNI is host names only (RFC 6066); the cert check takes either */
    int is_ip = gem_tls_is_ip(name);
    X509_VERIFY_PARAM *param = SSL_get0_param(ssl);
    X509_VERIFY_PARAM_set_hostflags(param, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
    if ((!is_ip && SSL_set_tlsext_host_name(ssl, name) != 1) ||
        (is_ip ? X509_VERIFY_PARAM_set1_ip_asc(param, name) : X509_VERIFY_PARAM_set1_host(param, name, 0)) != 1) {
        snprintf(buf, sizeof(buf), "tls_connect: invalid server name '%s'", name);
        gem_tls_fail(sock, buf);
    }

    while (1) {
        int fd;
        t = gem_tls_of(sock, &fd, "tls_connect");
        ERR_clear_error();
        errno = 0;
        int rc = SSL_connect(t->ssl);
        if (rc == 1) return sock;
        int err = SSL_get_error(t->ssl, rc);
        if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
            if (gem_tls_wait(sock, fd, err == SSL_ERROR_WANT_WRITE, deadline, "tls_connect")) {
                snprintf(buf, sizeof(buf), "tls_connect: handshake with %s:%d timed out", host, port);
                gem_tls_fail(sock, buf);
            }
            continue;
        }
        t->failed = 1;
        long vr = SSL_get_verify_result(t->ssl);
        if (vr != X509_V_OK) {
            char where[300] = "";
            if (strcmp(name, host) != 0) snprintf(where, sizeof(where), " (server name %s)", name);
            /* empty trust store: say where we looked */
            char hint[600] = "";
            const char *file = getenv(X509_get_default_cert_file_env());
            const char *dir = getenv(X509_get_default_cert_dir_env());
            if (!file) file = X509_get_default_cert_file();
            if (!dir) dir = X509_get_default_cert_dir();
            if (vr == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY && !ca_file &&
                gem_tls_store_empty(SSL_CTX_get_cert_store(ctx)) && gem_tls_dirs_empty(dir)) {
                snprintf(hint, sizeof(hint), " (no trusted certificates found in %s or %s; set SSL_CERT_FILE or SSL_CERT_DIR)",
                         file, dir);
            }
            snprintf(buf, sizeof(buf), "tls_connect: certificate verify failed for %s:%d%s: %s%s", host, port, where,
                     X509_verify_cert_error_string(vr), hint);
        } else if (err == SSL_ERROR_SYSCALL || err == SSL_ERROR_ZERO_RETURN) {
            snprintf(buf, sizeof(buf), "tls_connect: handshake with %s:%d failed: %s", host, port,
                     err == SSL_ERROR_SYSCALL && errno ? strerror(errno) : "connection closed by peer");
        } else {
            snprintf(buf, sizeof(buf), "tls_connect: handshake with %s:%d failed: %s", host, port,
                     gem_tls_reason("unknown error"));
        }
        gem_tls_fail(sock, buf);
    }
}

#endif
