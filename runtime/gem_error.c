/* Runtime errors, stack traces, pcall state. */

#include "gem.h"

static GemFrame gem_root_call_stack[GEM_MAX_CALL_DEPTH];
GemFrame *gem_call_stack = gem_root_call_stack;
int gem_call_depth = 0;
const GemLeafSite *gem_leaf_site = NULL;
int gem_leaf_line = 0;

GemPcallFrame gem_pcall_stack[GEM_MAX_PCALL_DEPTH];
int gem_pcall_depth = 0;

/* Mutual-TCO trampoline scratch: a body sets these at a tail call and returns, the wrapper reads and clears
   them. One global copy is fine: nothing yields between the set and the return, or the read and the dispatch. */
GemFnPtr gem_tail_fn = NULL;
void *gem_tail_env = NULL;
int gem_tail_argc = 0;
GemVal gem_tail_args[GEM_MAX_TAIL_ARGS];

static int gem_frame_same(const GemFrame *a, const GemFrame *b) {
    return a->line == b->line && strcmp(a->name, b->name) == 0 && strcmp(a->file, b->file) == 0;
}

void gem_print_stack_trace(void) {
    if (gem_leaf_site)
        fprintf(stderr, "  at %s (%s:%d)\n",
            gem_leaf_site->name, gem_leaf_site->file, gem_leaf_line);
    int max = gem_call_depth < GEM_MAX_CALL_DEPTH ? gem_call_depth : GEM_MAX_CALL_DEPTH;
    if (gem_call_depth > GEM_MAX_CALL_DEPTH)
        fprintf(stderr, "  ... (deeper frames not recorded)\n");
    for (int i = max - 1; i >= 0; i--) {
        fprintf(stderr, "  at %s (%s:%d)\n",
            gem_call_stack[i].name,
            gem_call_stack[i].file,
            gem_call_stack[i].line);
        /* collapse deep recursion */
        int j = i;
        while (j > 0 && gem_frame_same(&gem_call_stack[j - 1], &gem_call_stack[i])) j--;
        if (i - j > 1) {
            fprintf(stderr, "  ... same frame repeated %d more times\n", i - j);
            i = j;
        }
    }
}

/* Source line for a trace. Trace paths are relative (project root or the compile dir), so search
   $GEM_SOURCE_ROOT, the cwd, the exe's dir and its parents, then the cwd's parents. The first file long
   enough to have the line wins, even an unrelated one. Buffers are static: this can run on a nearly full stack. */

#include <unistd.h>
#include <limits.h>
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static char gem_src_line[2048];
static char gem_src_cand[PATH_MAX];
static char gem_src_dir[PATH_MAX];

static int gem_read_source_line(const char *path, int line) {
    FILE *f = gem_fopen(path, "r");
    if (!f) return 0;
    int cur = 1, found = 0;
    while (fgets(gem_src_line, sizeof(gem_src_line), f)) {
        size_t l = strlen(gem_src_line);
        int whole = l > 0 && gem_src_line[l-1] == '\n';
        if (cur == line) {
            while (l > 0 && (gem_src_line[l-1] == '\n' || gem_src_line[l-1] == '\r'))
                gem_src_line[--l] = 0;
            found = 1;
            break;
        }
        /* a line longer than the buffer takes several reads */
        if (whole) cur++;
    }
    fclose(f);
    return found;
}

static int gem_try_source_in(const char *dir, const char *file, int line) {
    int n = snprintf(gem_src_cand, sizeof(gem_src_cand), "%s%s%s", dir,
        (dir[0] && dir[strlen(dir) - 1] == '/') ? "" : "/", file);
    if (n < 0 || (size_t)n >= sizeof(gem_src_cand)) return 0;
    return gem_read_source_line(gem_src_cand, line);
}

/* dir is modified in place */
static int gem_try_source_upward(char *dir, const char *file, int line) {
    for (;;) {
        if (gem_try_source_in(dir, file, line)) return 1;
        char *slash = strrchr(dir, '/');
        if (!slash || (slash == dir && dir[1] == 0)) return 0;
        if (slash == dir) dir[1] = 0; else *slash = 0;
    }
}

/* exe's directory into gem_src_dir; 0 if gem_exe_path only has a relative argv[0] */
static int gem_exe_dir(void) {
    const char *exe = gem_exe_path();
    if (exe[0] != '/') return 0;
    int n = snprintf(gem_src_dir, sizeof(gem_src_dir), "%s", exe);
    if (n < 0 || (size_t)n >= sizeof(gem_src_dir)) return 0;
    char *slash = strrchr(gem_src_dir, '/');
    if (slash == gem_src_dir) slash[1] = 0; else *slash = 0;
    return 1;
}

static int gem_find_source_line(const char *file, int line) {
    if (file[0] == '/') return gem_read_source_line(file, line);
    const char *root = getenv("GEM_SOURCE_ROOT");
    if (root && root[0] && gem_try_source_in(root, file, line)) return 1;
    if (gem_read_source_line(file, line)) return 1;
    if (gem_exe_dir() && gem_try_source_upward(gem_src_dir, file, line)) return 1;
    if (getcwd(gem_src_dir, sizeof(gem_src_dir)) && gem_src_dir[0] == '/') {
        /* cwd itself was tried above */
        char *slash = strrchr(gem_src_dir, '/');
        if (slash == gem_src_dir) {
            if (gem_src_dir[1] == 0) return 0;
            gem_src_dir[1] = 0;
        } else {
            *slash = 0;
        }
        if (gem_try_source_upward(gem_src_dir, file, line)) return 1;
    }
    return 0;
}

/* same gutter format as compile errors */
static void gem_print_source_context(const char *file, int line) {
    if (!file || !file[0] || line <= 0) return;
    if (!gem_find_source_line(file, line)) return;
    int gw = 1; int n = line; while (n >= 10) { gw++; n /= 10; }
    fprintf(stderr, "  --> %s:%d\n", file, line);
    fprintf(stderr, " %*s |\n", gw, "");
    fprintf(stderr, " %d | %s\n", line, gem_src_line);
    fprintf(stderr, " %*s |\n", gw, "");
}

/* head replaces "Runtime Error" (e.g. to name a spawned process) */
void gem_print_runtime_error_as(const char *head, const char *msg) {
    fflush(stdout);
    fprintf(stderr, "\n[%s]: %s\n", head ? head : "Runtime Error", msg);
    if (gem_leaf_site) {
        gem_print_source_context(gem_leaf_site->file, gem_leaf_line);
    } else if (gem_call_depth > 0) {
        int top = (gem_call_depth <= GEM_MAX_CALL_DEPTH ? gem_call_depth : GEM_MAX_CALL_DEPTH) - 1;
        gem_print_source_context(gem_call_stack[top].file, gem_call_stack[top].line);
    }
    if (gem_leaf_site || gem_call_depth > 0) {
        fprintf(stderr, "Stack trace:\n");
        gem_print_stack_trace();
    }
    fflush(stderr);
}

void gem_print_runtime_error(const char *msg) {
    gem_print_runtime_error_as(NULL, msg);
}

static void gem_pcall_longjmp(GemPcallFrame *frame, const char *msg) {
    size_t len = strlen(msg) + 1;
    char *saved_msg = (char *)gem_alloc(len);
    memcpy(saved_msg, msg, len);
    frame->error_msg = saved_msg;
    GemVal stack_snapshot = gem_table_new();
    int max = gem_call_depth < GEM_MAX_CALL_DEPTH ? gem_call_depth : GEM_MAX_CALL_DEPTH;
    int saved = frame->saved_call_depth;
    int idx = 0;
    if (gem_leaf_site) {
        GemVal f = gem_table_new();
        gem_table_set(f, gem_string("name"), gem_string(gem_leaf_site->name));
        gem_table_set(f, gem_string("file"), gem_string(gem_leaf_site->file));
        gem_table_set(f, gem_string("line"), gem_int(gem_leaf_line));
        gem_table_set(stack_snapshot, gem_int(idx++), f);
        gem_leaf_site = NULL;
    }
    for (int i = max - 1; i >= saved; i--, idx++) {
        GemVal f = gem_table_new();
        gem_table_set(f, gem_string("name"), gem_string(gem_call_stack[i].name));
        gem_table_set(f, gem_string("file"), gem_string(gem_call_stack[i].file));
        gem_table_set(f, gem_string("line"), gem_int(gem_call_stack[i].line));
        gem_table_set(stack_snapshot, gem_int(idx), f);
    }
    frame->stack_snapshot = stack_snapshot;
    gem_call_depth = saved;
    longjmp(frame->buf, 1);
}

void gem_raise_error(const char *msg) {
    if (gem_current_pid >= 0 && gem_current_pid < gem_proc_hwm) {
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        if (proc->state != GEM_PROC_FREE && proc->state != GEM_PROC_DEAD) {
            if (proc->pcall_depth > 0) {
                proc->pcall_depth--;
                gem_pcall_longjmp(&proc->pcall_stack[proc->pcall_depth], msg);
            }
            /* uncaught: main exits; a spawned process logs a crash report and dies with msg as its reason */
            if (gem_current_pid == gem_main_pid) {
                gem_print_runtime_error(msg);
                exit(1);
            }
            gem_report_process_crash(gem_current_pid, msg);
            proc->exit_reason = strdup(msg);
            proc->crashed = 1;
            gem_call_depth = 0;
            longjmp(proc->proc_jmp, 1);
        }
    }

    if (gem_pcall_depth > 0) {
        gem_pcall_depth--;
        gem_pcall_longjmp(&gem_pcall_stack[gem_pcall_depth], msg);
    }

    gem_print_runtime_error(msg);
    exit(1);
}

void gem_error(const char *msg) {
    gem_raise_error(msg);
}

/* debug runtime only; aborts so there's a core/debugger stop */
void gem_assert_fail(const char *cond, const char *what, const char *file, int line) {
    fprintf(stderr, "gem: runtime invariant broken: %s (%s) at %s:%d\n", what, cond, file, line);
    if (gem_call_depth > 0 && gem_call_depth <= GEM_MAX_CALL_DEPTH) {
        GemFrame *f = &gem_call_stack[gem_call_depth - 1];
        fprintf(stderr, "  in %s (%s:%d)\n", f->name, f->file, f->line);
    }
    fflush(stderr);
    abort();
}
