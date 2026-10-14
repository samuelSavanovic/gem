/*
 * gem_builtins_tls.c — tls_connect, and tcp_read, tcp_write and the close
 *                      of the TLS sockets it returns (through gem_tls_ops).
 *
 * Compiled against OpenSSL 3 or later when GEM_TLS is defined: the Makefile defines
 * it, with OpenSSL's include path, when runtime/tls_config.sh finds a
 * usable OpenSSL (build/tls_cflags). Without it, tls_connect raises.
 *
 * A TLS socket is a plain socket resource whose entry's ptr holds a
 * GemTls, so closing it, its owner's exit and claim work as for any socket.
 * The fd stays non-blocking and OpenSSL reads and writes it directly. When
 * a call returns SSL_ERROR_WANT_READ or SSL_ERROR_WANT_WRITE, the process
 * waits (gem_io_yield) for the direction OpenSSL asked for, then repeats
 * the call; the entry is looked up again after every wait, and a socket
 * closed meanwhile raises "<builtin>: socket is closed" without touching
 * the freed session. Everything runs on the scheduler thread, so
 * OpenSSL's per-thread error queue is cleared before each call and read
 * right after it.
 */

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
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509v3.h>

typedef struct {
    SSL *ssl;
    int failed;   /* a fatal error happened: no close_notify (SSL_shutdown) */
} GemTls;

/* The context for the system trust store, made at the first tls_connect
   without a ca_file. Loading the store reads the CA bundle on the
   scheduler thread, once. */
static SSL_CTX *gem_tls_default_ctx = NULL;

/* The newest message in OpenSSL's error queue, or `fallback`. */
static const char *gem_tls_reason(const char *fallback) {
    unsigned long code = ERR_peek_last_error();
    const char *r = code ? ERR_reason_error_string(code) : NULL;
    return r ? r : fallback;
}

static SSL_CTX *gem_tls_new_ctx(void) {
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) gem_error("tls_connect: cannot create a TLS context");
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    /* A peer that closes without close_notify reads as end of stream, as
       a plain TCP close does. */
    SSL_CTX_set_options(ctx, SSL_OP_IGNORE_UNEXPECTED_EOF);
    SSL_CTX_set_mode(ctx, SSL_MODE_ENABLE_PARTIAL_WRITE | SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER |
                              SSL_MODE_RELEASE_BUFFERS);
    return ctx;
}

/* Waits until the socket is ready for the direction OpenSSL asked for, or
   until `deadline` (gem_now_ms time, -1 for none). Returns 1 when the
   deadline has passed. Raises "<who>: socket is closed" when the socket
   was closed meanwhile. */
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

/* ─── tcp_read on a TLS socket ─── */

static GemVal gem_tls_read(GemVal sock, size_t max_bytes, int has_timeout, int64_t timeout_ms) {
    int64_t deadline = has_timeout && timeout_ms > 0 ? gem_now_ms() + timeout_ms : -1;
    char *buf;
    if (gem_current_pid >= 0) {
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        if (proc->read_buf_cap < max_bytes) {
            proc->read_buf = (char *)gem_alloc(max_bytes);
            proc->read_buf_cap = max_bytes;
        }
        buf = proc->read_buf;
    } else {
        buf = (char *)gem_alloc(max_bytes);
    }
    int want = max_bytes > INT_MAX ? INT_MAX : (int)max_bytes;
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
        GemVal eof; eof.type = VAL_STRING; eof.magic = GEM_MAGIC; eof.sval = ""; eof.slen = 0;
        if (err == SSL_ERROR_ZERO_RETURN) return eof;
        t->failed = 1;
        if (err == SSL_ERROR_SYSCALL && (errno == 0 || errno == ECONNRESET)) return eof;
        char msg[256];
        if (err == SSL_ERROR_SYSCALL)
            snprintf(msg, sizeof(msg), "tcp_read: read failed: %s", strerror(errno));
        else
            snprintf(msg, sizeof(msg), "tcp_read: TLS error: %s", gem_tls_reason("unknown error"));
        gem_error(msg);
    }
}

/* ─── tcp_write on a TLS socket ─── */

static GemVal gem_tls_write(GemVal sock, const char *data, size_t total, int has_timeout, int64_t timeout_ms) {
    int64_t deadline = has_timeout && timeout_ms > 0 ? gem_now_ms() + timeout_ms : -1;
    size_t sent = 0;
    while (sent < total) {
        int fd;
        GemTls *t = gem_tls_of(sock, &fd, "tcp_write");
        size_t chunk = total - sent;
        if (chunk > INT_MAX) chunk = INT_MAX;
        ERR_clear_error();
        errno = 0;
        int n = SSL_write(t->ssl, data + sent, (int)chunk);
        if (n > 0) {
            sent += (size_t)n;
            continue;
        }
        int err = SSL_get_error(t->ssl, n);
        if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
            if (has_timeout && timeout_ms <= 0) break;
            if (gem_tls_wait(sock, fd, err == SSL_ERROR_WANT_WRITE, deadline, "tcp_write")) break;
            continue;
        }
        t->failed = 1;
        /* A peer that has gone: the count so far, as for plain TCP. */
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

/* ─── Close ─── */

static void gem_tls_close(void *p) {
    GemTls *t = (GemTls *)p;
    if (!t->failed && SSL_is_init_finished(t->ssl)) {
        ERR_clear_error();
        SSL_shutdown(t->ssl);   /* one try: the fd is non-blocking */
    }
    SSL_free(t->ssl);
    free(t);
    ERR_clear_error();
}

static const GemTlsOps gem_tls_ops_impl = {gem_tls_read, gem_tls_write, gem_tls_close};

/* ─── Built-in: tls_connect ─── */

/* Closes the socket (which frees its session) and raises `msg`. */
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

/* opts[key] when it is a non-empty string, NULL when it is nil; raises
   otherwise. */
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
    char buf[640];
    if (opts.type != VAL_TABLE && opts.type != VAL_NIL) {
        snprintf(buf, sizeof(buf), "tls_connect: opts must be a table, got %s", gem_type_str(opts));
        gem_error(buf);
    }
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
        ERR_clear_error();
        FILE *f = fopen(ca_file, "r");
        if (f) fclose(f);
        if (!f || SSL_CTX_load_verify_locations(ctx, ca_file, NULL) != 1) {
            snprintf(buf, sizeof(buf), "tls_connect: cannot load ca_file '%s': %s", ca_file,
                     f ? "no certificates in it" : strerror(errno));
            SSL_CTX_free(ctx);
            gem_tls_fail(sock, buf);
        }
    } else {
        if (!gem_tls_default_ctx) {
            gem_tls_default_ctx = gem_tls_new_ctx();
            /* SSL_CERT_FILE / SSL_CERT_DIR, else OpenSSL's own defaults. */
            SSL_CTX_set_default_verify_paths(gem_tls_default_ctx);
            ERR_clear_error();
        }
        ctx = gem_tls_default_ctx;
    }

    GemResEntry *e = gem_res_lookup(sock);
    SSL *ssl = SSL_new(ctx);
    if (ca_file) SSL_CTX_free(ctx);   /* the session holds its own reference */
    GemTls *t = ssl ? (GemTls *)malloc(sizeof(GemTls)) : NULL;
    if (!t) {
        if (ssl) SSL_free(ssl);
        gem_tls_fail(sock, "tls_connect: cannot create a TLS session");
    }
    t->ssl = ssl;
    t->failed = 0;
    gem_tls_ops = &gem_tls_ops_impl;
    e->ptr = t;   /* from here, closing the socket frees the session */

    SSL_set_fd(ssl, e->fd);
    /* SNI carries a host name only (RFC 6066); the certificate check takes
       a name or an IP address. */
    int is_ip = gem_tls_is_ip(name);
    if (!is_ip) SSL_set_tlsext_host_name(ssl, name);
    X509_VERIFY_PARAM *param = SSL_get0_param(ssl);
    X509_VERIFY_PARAM_set_hostflags(param, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
    if ((is_ip ? X509_VERIFY_PARAM_set1_ip_asc(param, name) : X509_VERIFY_PARAM_set1_host(param, name, 0)) != 1) {
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
        if (vr != X509_V_OK && strcmp(name, host) != 0) {
            snprintf(buf, sizeof(buf), "tls_connect: certificate verify failed for %s:%d (server name %s): %s",
                     host, port, name, X509_verify_cert_error_string(vr));
        } else if (vr != X509_V_OK) {
            snprintf(buf, sizeof(buf), "tls_connect: certificate verify failed for %s:%d: %s", host, port,
                     X509_verify_cert_error_string(vr));
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
