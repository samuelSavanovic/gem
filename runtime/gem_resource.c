/*
 * gem_resource.c — the resource table: sockets and sqlite handles as owned
 *                  resources, claim, and their close when a process exits.
 */

#include "gem.h"
#include <unistd.h>

/* ─── Resource table ───
 *
 * One malloc'd array of entries, reused through a free list. An entry's
 * serial is never reused, so a value whose serial no longer matches its
 * slot's names a closed resource in every process that holds a copy.
 *
 * Lists, by entry index (-1 ends one):
 *   - own: the entries a live process owns (GemProcess.res_owned), or the
 *     entries parked on a killed process's unfinished pool request;
 *   - use: the entries a process used last without owning them
 *     (GemProcess.res_used), for the GEM_DIAG=2 report at its exit.
 * Both are emptied when the process exits, so a slot's next process never
 * sees them. Everything here runs on the scheduler thread. */

static GemResEntry *gem_res = NULL;
static int32_t gem_res_cap = 0;
static int32_t gem_res_hwm = 0;
static int32_t gem_res_free = -1;
static int64_t gem_res_serial = 0;
static int gem_res_open = 0;

static const struct {
    const char *type;     /* type() */
    const char *print;    /* #Socket<N> */
    const char *what;     /* "expected a socket" */
    const char *closed;   /* "<builtin>: socket is closed" */
    const char *claim_closed;
} gem_res_kinds[GEM_RES_KINDS] = {
    [GEM_RES_SOCKET] = {"socket", "Socket", "a socket", "socket is closed", "socket is closed"},
    [GEM_RES_SQLITE] = {"sqlite", "Sqlite", "a database handle", "not an open database handle", "database is closed"},
};

const char *gem_res_type_name(int kind) {
    return kind > 0 && kind < GEM_RES_KINDS ? gem_res_kinds[kind].type : "resource";
}

const char *gem_res_print_name(int kind) {
    return kind > 0 && kind < GEM_RES_KINDS ? gem_res_kinds[kind].print : "Resource";
}

/* Sockets of a process killed while its own extern blocking call runs on
   the pool: the worker may be using their fds, so they close when the
   request is done (gem_res_check_parked). A slot with req NULL is free. */
typedef struct {
    GemIORequest *req;
    int32_t head;
} GemResParked;

static GemResParked *gem_res_parked = NULL;
static int gem_res_parked_n = 0, gem_res_parked_cap = 0, gem_res_parked_live = 0;

static void gem_res_oom(void) {
    fprintf(stderr, "gem: out of memory (resource table)\n");
    exit(1);
}

static int32_t *gem_res_own_head(GemResEntry *e) {
    if (e->parked >= 0) return &gem_res_parked[e->parked].head;
    if (e->owner_slot >= 0) return &gem_proc_table[e->owner_slot].res_owned;
    return NULL;
}

static void gem_res_own_unlink(int32_t i) {
    GemResEntry *e = &gem_res[i];
    int32_t *head = gem_res_own_head(e);
    if (!head) return;
    if (e->own_prev >= 0) gem_res[e->own_prev].own_next = e->own_next;
    else *head = e->own_next;
    if (e->own_next >= 0) gem_res[e->own_next].own_prev = e->own_prev;
    e->own_prev = e->own_next = -1;
    if (e->parked < 0) gem_proc_table[e->owner_slot].res_count--;
    e->parked = -1;
    e->owner_slot = -1;
}

static void gem_res_own_link(int32_t i, int32_t *head) {
    GemResEntry *e = &gem_res[i];
    e->own_prev = -1;
    e->own_next = *head;
    if (*head >= 0) gem_res[*head].own_prev = i;
    *head = i;
}

/* Make the process in `slot` the owner (-1: no owner). */
static void gem_res_set_owner(int32_t i, int slot) {
    gem_res_own_unlink(i);
    GemResEntry *e = &gem_res[i];
    if (slot < 0) {
        e->owner = -1;
        return;
    }
    e->owner = gem_pid_of_slot(slot);
    e->owner_slot = slot;
    gem_res_own_link(i, &gem_proc_table[slot].res_owned);
    gem_proc_table[slot].res_count++;
}

static void gem_res_use_unlink(int32_t i) {
    GemResEntry *e = &gem_res[i];
    if (e->user_slot < 0) return;
    if (e->use_prev >= 0) gem_res[e->use_prev].use_next = e->use_next;
    else gem_proc_table[e->user_slot].res_used = e->use_next;
    if (e->use_next >= 0) gem_res[e->use_next].use_prev = e->use_prev;
    e->use_prev = e->use_next = -1;
    e->user = -1;
    e->user_slot = -1;
}

/* The running process used entry i: remember it as the last user unless
   it owns the entry. */
static void gem_res_note_user(int32_t i) {
    int cur = gem_current_pid;
    GemResEntry *e = &gem_res[i];
    if (cur < 0 || e->owner_slot == cur || e->user_slot == cur) return;
    gem_res_use_unlink(i);
    GemProcess *p = &gem_proc_table[cur];
    e->user = gem_pid_of_slot(cur);
    e->user_slot = cur;
    e->use_prev = -1;
    e->use_next = p->res_used;
    if (p->res_used >= 0) gem_res[p->res_used].use_prev = i;
    p->res_used = i;
}

static GemVal gem_res_value(int32_t i) {
    GemVal v;
    v.type = VAL_RESOURCE;
    v.magic = GEM_MAGIC;
    v.res_id = gem_res[i].serial;
    v.res_slot = i;
    v.res_kind = gem_res[i].kind;
    return v;
}

void gem_res_proc_init(GemProcess *proc) {
    proc->res_owned = -1;
    proc->res_used = -1;
    proc->res_count = 0;
    proc->crashed = 0;
    proc->wait_res = GEM_NIL;
}

GemVal gem_res_new(int kind) {
    int32_t i;
    if (gem_res_free >= 0) {
        i = gem_res_free;
        gem_res_free = gem_res[i].free_next;
    } else {
        if (gem_res_hwm == gem_res_cap) {
            int32_t cap = gem_res_cap ? gem_res_cap * 2 : 64;
            GemResEntry *a = (GemResEntry *)realloc(gem_res, sizeof(GemResEntry) * (size_t)cap);
            if (!a) gem_res_oom();
            gem_res = a;
            gem_res_cap = cap;
        }
        i = gem_res_hwm++;
    }
    GemResEntry *e = &gem_res[i];
    memset(e, 0, sizeof(*e));
    e->serial = ++gem_res_serial;
    e->kind = kind;
    e->owner = -1;
    e->owner_slot = -1;
    e->user = -1;
    e->user_slot = -1;
    e->parked = -1;
    e->own_prev = e->own_next = -1;
    e->use_prev = e->use_next = -1;
    e->free_next = -1;
    e->fd = -1;
    gem_res_open++;
    if (gem_current_pid >= 0) gem_res_set_owner(i, gem_current_pid);
    return gem_res_value(i);
}

GemResEntry *gem_res_lookup(GemVal v) {
    if (v.type != VAL_RESOURCE || v.res_slot < 0 || v.res_slot >= gem_res_hwm) return NULL;
    GemResEntry *e = &gem_res[v.res_slot];
    return e->serial == v.res_id ? e : NULL;
}

GemResEntry *gem_res_get_open(GemVal v, int kind, const char *who) {
    if (v.type != VAL_RESOURCE || v.res_kind != kind) {
        char buf[160];
        snprintf(buf, sizeof(buf), "%s: expected %s, got %s", who, gem_res_kinds[kind].what, gem_type_str(v));
        gem_error(buf);
    }
    GemResEntry *e = gem_res_lookup(v);
    if (e) gem_res_note_user(v.res_slot);
    return e;
}

GemResEntry *gem_res_get(GemVal v, int kind, const char *who) {
    GemResEntry *e = gem_res_get_open(v, kind, who);
    if (!e) {
        char buf[160];
        snprintf(buf, sizeof(buf), "%s: %s", who, gem_res_kinds[kind].closed);
        gem_error(buf);
    }
    return e;
}

int gem_res_closing(GemVal v) {
    GemResEntry *e = gem_res_lookup(v);
    return e && e->closing;
}

static void gem_res_free_entry(int32_t i) {
    gem_res_own_unlink(i);
    gem_res_use_unlink(i);
    gem_res[i].serial = 0;
    gem_res[i].free_next = gem_res_free;
    gem_res_free = i;
    gem_res_open--;
}

void *gem_res_take(GemResEntry *e) {
    void *ptr = e->ptr;
    gem_res_free_entry((int32_t)(e - gem_res));
    return ptr;
}

/* Close n entries: wake the waiters of the sockets among them with one
   scan of the fd waiters, free the entries, then close each payload. */
static void gem_res_close_n(const int32_t *idx, int n) {
    int sockets = 0;
    for (int k = 0; k < n; k++) {
        if (gem_res[idx[k]].kind == GEM_RES_SOCKET) {
            gem_res[idx[k]].closing = 1;
            sockets = 1;
        }
    }
    if (sockets) gem_io_wake_closing();
    for (int k = 0; k < n; k++) {
        GemResEntry e = gem_res[idx[k]];
        gem_res_free_entry(idx[k]);
        if (e.kind == GEM_RES_SOCKET) gem_tcp_close_fd_checked(e.fd, e.dev, e.ino);
    }
}

void gem_res_close(GemResEntry *e) {
    int32_t i = (int32_t)(e - gem_res);
    gem_res_close_n(&i, 1);
}

GemResEntry *gem_res_find_socket(int fd, uint64_t dev, uint64_t ino) {
    for (int32_t i = 0; i < gem_res_hwm; i++) {
        GemResEntry *e = &gem_res[i];
        if (e->serial && e->kind == GEM_RES_SOCKET && e->fd == fd && e->dev == dev && e->ino == ino)
            return e;
    }
    return NULL;
}

void gem_res_diag_counts(int *open, int *ownerless) {
    int none = 0;
    for (int32_t i = 0; i < gem_res_hwm; i++)
        if (gem_res[i].serial && gem_res[i].owner_slot < 0) none++;
    *open = gem_res_open;
    *ownerless = none;
}

/* ─── Process exit ─── */

static int gem_res_diag_level(void) {
    static int level = -1;
    if (level < 0) {
        const char *d = getenv("GEM_DIAG");
        level = d && (d[0] == '1' || d[0] == '2') ? d[0] - '0' : 0;
    }
    return level;
}

/* A process's name for the GEM_DIAG=2 report: its entry function, or the
   function that one called when the entry is a fn literal (`spawn do
   session(sock) end`). Frames past the call depth are stale but kept, and
   the bottom two are cleared at spawn. */
static const char *gem_res_proc_name(int slot) {
    if (slot < 0) return NULL;
    GemFrame *f = gem_proc_table[slot].call_stack;
    const char *n = f[0].name;
    if (n && strcmp(n, "anonymous fn") == 0 && f[1].name) n = f[1].name;
    return n;
}

static void gem_res_print_proc(char *buf, size_t n, int64_t pid, int slot) {
    const char *name = gem_res_proc_name(slot);
    if (name) snprintf(buf, n, "process %lld (%s)", (long long)pid, name);
    else snprintf(buf, n, "process %lld", (long long)pid);
}

static int32_t *gem_res_batch = NULL;
static int gem_res_batch_cap = 0;

static void gem_res_batch_push(int *n, int32_t i) {
    if (*n == gem_res_batch_cap) {
        int cap = gem_res_batch_cap ? gem_res_batch_cap * 2 : 16;
        int32_t *a = (int32_t *)realloc(gem_res_batch, sizeof(int32_t) * (size_t)cap);
        if (!a) gem_res_oom();
        gem_res_batch = a;
        gem_res_batch_cap = cap;
    }
    gem_res_batch[(*n)++] = i;
}

static int gem_res_park_slot(GemIORequest *req) {
    for (int k = 0; k < gem_res_parked_n; k++) {
        if (!gem_res_parked[k].req) {
            gem_res_parked[k].req = req;
            gem_res_parked[k].head = -1;
            gem_res_parked_live++;
            return k;
        }
    }
    if (gem_res_parked_n == gem_res_parked_cap) {
        int cap = gem_res_parked_cap ? gem_res_parked_cap * 2 : 8;
        GemResParked *a = (GemResParked *)realloc(gem_res_parked, sizeof(GemResParked) * (size_t)cap);
        if (!a) gem_res_oom();
        gem_res_parked = a;
        gem_res_parked_cap = cap;
    }
    gem_res_parked[gem_res_parked_n].req = req;
    gem_res_parked[gem_res_parked_n].head = -1;
    gem_res_parked_live++;
    return gem_res_parked_n++;
}

void gem_res_proc_exit(int slot) {
    GemProcess *proc = &gem_proc_table[slot];
    int abnormal = proc->crashed ||
                   (proc->exit_reason && strcmp(proc->exit_reason, "normal") != 0);
    int diag = gem_res_diag_level() == 2;
    int64_t pid = gem_pid_of_slot(slot);
    const char *how = proc->crashed ? "error" : proc->exit_reason ? proc->exit_reason : "normal";
    char me[160], other[160];
    if (diag) gem_res_print_proc(me, sizeof(me), pid, slot);

    /* Resources it used without owning: report the open ones, by owner. */
    while (proc->res_used >= 0) {
        int32_t i = proc->res_used;
        GemResEntry *e = &gem_res[i];
        if (diag) {
            int kind = e->kind, count = 0;
            int64_t owner = e->owner;
            int owner_slot = e->owner_slot;
            for (int32_t j = proc->res_used; j >= 0; ) {
                int32_t next = gem_res[j].use_next;
                if (gem_res[j].kind == kind && gem_res[j].owner == owner) {
                    count++;
                    gem_res_use_unlink(j);
                }
                j = next;
            }
            if (owner < 0) snprintf(other, sizeof(other), "no process");
            else gem_res_print_proc(other, sizeof(other), owner, owner_slot);
            fprintf(stderr, "gem_resources: %s exited (%s) after using %d %s%s owned by %s\n",
                    me, how, count, gem_res_kinds[kind].type, count == 1 ? "" : "s", other);
        } else {
            gem_res_use_unlink(i);
        }
    }

    /* Resources it owns: claimed ones close on any exit, the rest on an
       abnormal one and are left ownerless on a normal one. The sockets of
       a process killed during its own extern blocking call are parked
       until the call returns. */
    GemIORequest *req = proc->io_request;
    int park = req && req->op == GEM_IO_EXTERN && !__atomic_load_n(&req->done, __ATOMIC_ACQUIRE);
    int parked = -1, n = 0, left[GEM_RES_KINDS] = {0};
    while (proc->res_owned >= 0) {
        int32_t i = proc->res_owned;
        GemResEntry *e = &gem_res[i];
        if (!e->claimed && !abnormal) {
            left[e->kind]++;
            gem_res_set_owner(i, -1);
        } else if (park && e->kind == GEM_RES_SOCKET) {
            if (parked < 0) parked = gem_res_park_slot(req);
            gem_res_set_owner(i, -1);
            e->parked = parked;
            gem_res_own_link(i, &gem_res_parked[parked].head);
        } else {
            gem_res_own_unlink(i);
            e->owner = -1;
            gem_res_batch_push(&n, i);
        }
    }
    if (parked >= 0) proc->io_request = NULL;   /* the parked list holds its reference now */
    if (n > 0) gem_res_close_n(gem_res_batch, n);
    if (diag) {
        for (int k = 1; k < GEM_RES_KINDS; k++) {
            if (left[k] == 0) continue;
            fprintf(stderr, "gem_resources: %s exited (%s) leaving %d %s%s open with no owner\n",
                    me, how, left[k], gem_res_kinds[k].type, left[k] == 1 ? "" : "s");
        }
    }
}

void gem_res_check_parked(void) {
    if (gem_res_parked_live == 0) return;
    for (int k = 0; k < gem_res_parked_n; k++) {
        GemIORequest *req = gem_res_parked[k].req;
        if (!req || !__atomic_load_n(&req->done, __ATOMIC_ACQUIRE)) continue;
        int n = 0;
        while (gem_res_parked[k].head >= 0) {
            int32_t i = gem_res_parked[k].head;
            gem_res_own_unlink(i);
            gem_res_batch_push(&n, i);
        }
        gem_res_parked[k].req = NULL;
        gem_res_parked_live--;
        gem_io_release(req);
        if (n > 0) gem_res_close_n(gem_res_batch, n);
    }
}

int gem_res_parked_pending(void) {
    return gem_res_parked_live > 0;
}

/* ─── Built-in: claim ─── */

GemVal gem_claim_fn(void *_env, GemVal *args, int argc) {
    (void)_env;
    GemVal v = argc >= 1 ? args[0] : GEM_NIL;
    if (v.type != VAL_RESOURCE) {
        char buf[160];
        snprintf(buf, sizeof(buf), "claim: expected a socket or a database handle, got %s", gem_type_str(v));
        gem_error(buf);
    }
    GemResEntry *e = gem_res_lookup(v);
    if (!e) {
        char buf[160];
        snprintf(buf, sizeof(buf), "claim: %s", gem_res_kinds[v.res_kind].claim_closed);
        gem_error(buf);
    }
    int cur = gem_current_pid;
    if (cur < 0) return v;
    int32_t i = v.res_slot;
    if (e->user_slot == cur) gem_res_use_unlink(i);
    if (e->owner_slot != cur || e->parked >= 0) gem_res_set_owner(i, cur);
    e->claimed = 1;
    return v;
}
