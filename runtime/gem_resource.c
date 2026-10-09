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

/* For now a process's exit leaves everything it owns open and ownerless. */
void gem_res_proc_exit(int slot) {
    GemProcess *proc = &gem_proc_table[slot];
    while (proc->res_used >= 0) gem_res_use_unlink(proc->res_used);
    while (proc->res_owned >= 0) gem_res_set_owner(proc->res_owned, -1);
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
