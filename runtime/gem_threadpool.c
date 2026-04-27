/*
 * gem_threadpool.c — Worker thread pool for non-blocking file I/O.
 *
 * When a coroutine calls read_file/write_file/append_file/exec, the operation
 * is handed to a worker thread so the scheduler can continue running other
 * coroutines. A wake-pipe notifies the scheduler when work completes.
 */

#include "gem.h"

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#ifndef GEM_POOL_SIZE
#define GEM_POOL_SIZE 4
#endif
#define GEM_IO_QUEUE_CAP 1024

static pthread_t gem_io_workers[GEM_POOL_SIZE];
static GemIORequest *gem_io_queue[GEM_IO_QUEUE_CAP];
static int gem_io_q_head = 0;
static int gem_io_q_tail = 0;
static int gem_io_q_count = 0;
static pthread_mutex_t gem_io_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t gem_io_cond = PTHREAD_COND_INITIALIZER;
static int gem_io_wake_pipe_fds[2] = {-1, -1};
static volatile int gem_io_shutdown_flag = 0;

static void gem_io_do_read(GemIORequest *req) {
    FILE *f = fopen(req->path, "rb");
    if (!f) {
        char buf[512];
        snprintf(buf, sizeof(buf), "cannot open '%s'", req->path);
        req->error_msg = strdup(buf);
        return;
    }
    fseek(f, 0, SEEK_END);
    long flen = ftell(f);
    rewind(f);
    req->result_data = (char *)malloc((size_t)flen + 1);
    req->result_len = fread(req->result_data, 1, (size_t)flen, f);
    req->result_data[req->result_len] = '\0';
    fclose(f);
}

static void gem_io_do_write(GemIORequest *req, const char *mode) {
    FILE *f = fopen(req->path, mode);
    if (!f) {
        char buf[512];
        snprintf(buf, sizeof(buf), "cannot open '%s' for writing", req->path);
        req->error_msg = strdup(buf);
        return;
    }
    size_t nwritten = fwrite(req->content, 1, req->content_len, f);
    fclose(f);
    if (nwritten != req->content_len) {
        char buf[512];
        snprintf(buf, sizeof(buf), "write failed for '%s'", req->path);
        req->error_msg = strdup(buf);
    }
}

static void gem_io_do_exec(GemIORequest *req) {
    int ret = system(req->path);
    req->exit_code = WIFEXITED(ret) ? WEXITSTATUS(ret) : -1;
}

static void gem_io_do_tcp_connect(GemIORequest *req) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "socket failed: %s", strerror(errno));
        req->error_msg = strdup(buf);
        return;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)req->socket_fd);

    if (inet_pton(AF_INET, req->path, &addr.sin_addr) != 1) {
        struct hostent *he = gethostbyname(req->path);
        if (!he) {
            close(fd);
            char buf[256];
            snprintf(buf, sizeof(buf), "cannot resolve '%s'", req->path);
            req->error_msg = strdup(buf);
            return;
        }
        memcpy(&addr.sin_addr, he->h_addr_list[0], (size_t)he->h_length);
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(fd);
        char buf[256];
        snprintf(buf, sizeof(buf), "connect failed: %s", strerror(errno));
        req->error_msg = strdup(buf);
        return;
    }

    req->result_fd = fd;
}

static void gem_io_do_tcp_accept(GemIORequest *req) {
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int fd = accept(req->socket_fd, (struct sockaddr *)&addr, &addr_len);
    if (fd < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "accept failed: %s", strerror(errno));
        req->error_msg = strdup(buf);
        return;
    }
    req->result_fd = fd;
}

static void gem_io_do_tcp_read(GemIORequest *req) {
    size_t cap = req->max_read > 0 ? req->max_read : 4096;
    char *buf = (char *)malloc(cap);
    ssize_t n = read(req->socket_fd, buf, cap);
    if (n < 0) {
        free(buf);
        char errbuf[256];
        snprintf(errbuf, sizeof(errbuf), "tcp read failed: %s", strerror(errno));
        req->error_msg = strdup(errbuf);
        return;
    }
    req->result_data = buf;
    req->result_len = (size_t)n;
}

static void gem_io_do_tcp_write(GemIORequest *req) {
    const char *data = req->content;
    size_t remaining = req->content_len;
    while (remaining > 0) {
        ssize_t n = write(req->socket_fd, data, remaining);
        if (n < 0) {
            char buf[256];
            snprintf(buf, sizeof(buf), "tcp write failed: %s", strerror(errno));
            req->error_msg = strdup(buf);
            return;
        }
        data += n;
        remaining -= (size_t)n;
    }
    req->result_len = req->content_len;
}

static void *gem_io_worker_fn(void *arg) {
    (void)arg;
    for (;;) {
        pthread_mutex_lock(&gem_io_mutex);
        while (gem_io_q_count == 0 && !gem_io_shutdown_flag)
            pthread_cond_wait(&gem_io_cond, &gem_io_mutex);
        if (gem_io_shutdown_flag && gem_io_q_count == 0) {
            pthread_mutex_unlock(&gem_io_mutex);
            break;
        }
        GemIORequest *req = gem_io_queue[gem_io_q_head];
        gem_io_q_head = (gem_io_q_head + 1) % GEM_IO_QUEUE_CAP;
        gem_io_q_count--;
        pthread_mutex_unlock(&gem_io_mutex);

        switch (req->op) {
            case GEM_IO_READ_FILE:   gem_io_do_read(req); break;
            case GEM_IO_WRITE_FILE:  gem_io_do_write(req, "wb"); break;
            case GEM_IO_APPEND_FILE: gem_io_do_write(req, "ab"); break;
            case GEM_IO_EXEC:        gem_io_do_exec(req); break;
            case GEM_IO_EXTERN:      req->extern_fn(req->extern_args); break;
            case GEM_IO_TCP_CONNECT: gem_io_do_tcp_connect(req); break;
            case GEM_IO_TCP_ACCEPT:  gem_io_do_tcp_accept(req); break;
            case GEM_IO_TCP_READ:    gem_io_do_tcp_read(req); break;
            case GEM_IO_TCP_WRITE:   gem_io_do_tcp_write(req); break;
        }

        __sync_synchronize();
        req->done = 1;
        __sync_synchronize();
        char c = 1;
        (void)write(gem_io_wake_pipe_fds[1], &c, 1);
    }
    return NULL;
}

void gem_threadpool_init(void) {
    if (pipe(gem_io_wake_pipe_fds) != 0) {
        perror("gem: pipe failed for threadpool wake-pipe");
        return;
    }
    fcntl(gem_io_wake_pipe_fds[0], F_SETFL, O_NONBLOCK);
    fcntl(gem_io_wake_pipe_fds[1], F_SETFL, O_NONBLOCK);

    gem_io_shutdown_flag = 0;
    for (int i = 0; i < GEM_POOL_SIZE; i++)
        pthread_create(&gem_io_workers[i], NULL, gem_io_worker_fn, NULL);
}

void gem_threadpool_shutdown(void) {
    pthread_mutex_lock(&gem_io_mutex);
    gem_io_shutdown_flag = 1;
    pthread_cond_broadcast(&gem_io_cond);
    pthread_mutex_unlock(&gem_io_mutex);

    for (int i = 0; i < GEM_POOL_SIZE; i++)
        pthread_join(gem_io_workers[i], NULL);

    if (gem_io_wake_pipe_fds[0] >= 0) close(gem_io_wake_pipe_fds[0]);
    if (gem_io_wake_pipe_fds[1] >= 0) close(gem_io_wake_pipe_fds[1]);
    gem_io_wake_pipe_fds[0] = gem_io_wake_pipe_fds[1] = -1;
}

GemIORequest *gem_io_submit(GemIOOp op, const char *path,
                            const char *content, size_t content_len) {
    GemIORequest *req = (GemIORequest *)calloc(1, sizeof(GemIORequest));
    req->op = op;
    req->requester_pid = gem_current_pid;
    req->path = strdup(path);
    if (content && content_len > 0) {
        req->content = (char *)malloc(content_len);
        memcpy(req->content, content, content_len);
        req->content_len = content_len;
    }

    pthread_mutex_lock(&gem_io_mutex);
    if (gem_io_q_count >= GEM_IO_QUEUE_CAP) {
        pthread_mutex_unlock(&gem_io_mutex);
        free(req->path);
        free(req->content);
        free(req);
        return NULL;
    }
    gem_io_queue[gem_io_q_tail] = req;
    gem_io_q_tail = (gem_io_q_tail + 1) % GEM_IO_QUEUE_CAP;
    gem_io_q_count++;
    pthread_cond_signal(&gem_io_cond);
    pthread_mutex_unlock(&gem_io_mutex);
    return req;
}

GemIORequest *gem_io_submit_extern(void (*fn)(void *), void *args) {
    GemIORequest *req = (GemIORequest *)calloc(1, sizeof(GemIORequest));
    req->op = GEM_IO_EXTERN;
    req->requester_pid = gem_current_pid;
    req->extern_fn = fn;
    req->extern_args = args;

    pthread_mutex_lock(&gem_io_mutex);
    if (gem_io_q_count >= GEM_IO_QUEUE_CAP) {
        pthread_mutex_unlock(&gem_io_mutex);
        free(req);
        return NULL;
    }
    gem_io_queue[gem_io_q_tail] = req;
    gem_io_q_tail = (gem_io_q_tail + 1) % GEM_IO_QUEUE_CAP;
    gem_io_q_count++;
    pthread_cond_signal(&gem_io_cond);
    pthread_mutex_unlock(&gem_io_mutex);
    return req;
}

GemIORequest *gem_io_submit_tcp(GemIOOp op, int socket_fd,
                                const char *data, size_t data_len,
                                size_t max_read) {
    GemIORequest *req = (GemIORequest *)calloc(1, sizeof(GemIORequest));
    req->op = op;
    req->requester_pid = gem_current_pid;
    req->socket_fd = socket_fd;
    req->max_read = max_read;
    if (data && data_len > 0) {
        req->content = (char *)malloc(data_len);
        memcpy(req->content, data, data_len);
        req->content_len = data_len;
    }

    pthread_mutex_lock(&gem_io_mutex);
    if (gem_io_q_count >= GEM_IO_QUEUE_CAP) {
        pthread_mutex_unlock(&gem_io_mutex);
        free(req->content);
        free(req);
        return NULL;
    }
    gem_io_queue[gem_io_q_tail] = req;
    gem_io_q_tail = (gem_io_q_tail + 1) % GEM_IO_QUEUE_CAP;
    gem_io_q_count++;
    pthread_cond_signal(&gem_io_cond);
    pthread_mutex_unlock(&gem_io_mutex);
    return req;
}

void gem_io_check_completions(void) {
    if (gem_io_wake_pipe_fds[0] < 0) return;
    char buf[64];
    while (read(gem_io_wake_pipe_fds[0], buf, sizeof(buf)) > 0) {}

    for (int i = 0; i < gem_proc_hwm; i++) {
        GemProcess *proc = &gem_proc_table[i];
        if (proc->state == GEM_PROC_IO_WAIT && proc->io_request != NULL) {
            __sync_synchronize();
            if (proc->io_request->done) {
                proc->state = GEM_PROC_READY;
            }
        }
    }
}

int gem_io_wake_fd(void) {
    return gem_io_wake_pipe_fds[0];
}
