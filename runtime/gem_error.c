/*
 * gem_error.c — Error handling, stack traces, pcall/setjmp globals.
 */

#include "gem.h"

/* ─── Call stack ─── */

static GemFrame gem_root_call_stack[GEM_MAX_CALL_DEPTH];
GemFrame *gem_call_stack = gem_root_call_stack;
int gem_call_depth = 0;
const GemLeafSite *gem_leaf_site = NULL;
int gem_leaf_line = 0;

/* ─── pcall jump buffer stack ─── */

GemPcallFrame gem_pcall_stack[GEM_MAX_PCALL_DEPTH];
int gem_pcall_depth = 0;

/* ─── Mutual-TCO trampoline scratch ───
 *
 * Used by the codegen's SCC-trampoline scheme: an SCC member's body sets
 * these globals at an intra-SCC tail-call site and returns; the wrapper
 * loop in gem_fn_<name> reads them, dispatches the next body, and clears
 * the marker. Single-global is safe because the scheduler is co-operative
 * and yields only happen inside bodies — never between a body's TLB-set
 * and its return, and never between the wrapper's TLB-read and dispatch.
 */
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
    /* Only the outermost GEM_MAX_CALL_DEPTH frames are recorded. */
    if (gem_call_depth > GEM_MAX_CALL_DEPTH)
        fprintf(stderr, "  ... (deeper frames not recorded)\n");
    for (int i = max - 1; i >= 0; i--) {
        fprintf(stderr, "  at %s (%s:%d)\n",
            gem_call_stack[i].name,
            gem_call_stack[i].file,
            gem_call_stack[i].line);
        /* Collapse a run of identical frames (deep recursion) to one line. */
        int j = i;
        while (j > 0 && gem_frame_same(&gem_call_stack[j - 1], &gem_call_stack[i])) j--;
        if (i - j > 1) {
            fprintf(stderr, "  ... same frame repeated %d more times\n", i - j);
            i = j;
        }
    }
}

/* ─── Source context for traces ───
 *
 * Trace paths are project-relative (or relative to the directory the
 * program was compiled from), never absolute developer paths, so a binary
 * run from another directory can't open them from the cwd alone. To find
 * the source we try, in order, and take the first file that has the line:
 *   1. $GEM_SOURCE_ROOT/<path>, when that variable is set;
 *   2. <path> from the cwd;
 *   3. <path> under the executable's directory and each of its ancestors
 *      (a binary built into <project>/build/ or <project>/bin/);
 *   4. <path> under each ancestor of the cwd.
 * An absolute path is only opened as is. The only check on a candidate is
 * its length: one too short to have the line is skipped, but a longer,
 * unrelated file at the same relative path is printed. Nothing found: no
 * source line. Buffers are static (this runs once per uncaught error, maybe
 * on a nearly exhausted process stack). */

#include <unistd.h>
#include <limits.h>
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static char gem_src_line[2048];
static char gem_src_cand[PATH_MAX];
static char gem_src_dir[PATH_MAX];

/* Read line `line` of `path` into gem_src_line (newline stripped).
 * Returns 1 if the file opened and has that line. */
static int gem_read_source_line(const char *path, int line) {
    FILE *f = fopen(path, "r");
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
        /* A line longer than the buffer spans several reads: count it once. */
        if (whole) cur++;
    }
    fclose(f);
    return found;
}

/* Try `dir`/`file`. */
static int gem_try_source_in(const char *dir, const char *file, int line) {
    int n = snprintf(gem_src_cand, sizeof(gem_src_cand), "%s%s%s", dir,
        (dir[0] && dir[strlen(dir) - 1] == '/') ? "" : "/", file);
    if (n < 0 || (size_t)n >= sizeof(gem_src_cand)) return 0;
    return gem_read_source_line(gem_src_cand, line);
}

/* Try `file` under `dir` (an absolute directory, modified in place) and
 * each of its ancestors up to the filesystem root. */
static int gem_try_source_upward(char *dir, const char *file, int line) {
    for (;;) {
        if (gem_try_source_in(dir, file, line)) return 1;
        char *slash = strrchr(dir, '/');
        if (!slash || (slash == dir && dir[1] == 0)) return 0;
        if (slash == dir) dir[1] = 0; else *slash = 0;
    }
}

/* The directory holding the running executable (gem_exe_path, in
 * gem_builtins_io.c), into gem_src_dir. Returns 0 where it can't be
 * determined (gem_exe_path then falls back to a relative argv[0]). */
static int gem_exe_dir(void) {
    const char *exe = gem_exe_path();
    if (exe[0] != '/') return 0;
    int n = snprintf(gem_src_dir, sizeof(gem_src_dir), "%s", exe);
    if (n < 0 || (size_t)n >= sizeof(gem_src_dir)) return 0;
    char *slash = strrchr(gem_src_dir, '/');
    if (slash == gem_src_dir) slash[1] = 0; else *slash = 0;
    return 1;
}

/* Find `file` (a trace path) and read line `line` of it into gem_src_line. */
static int gem_find_source_line(const char *file, int line) {
    if (file[0] == '/') return gem_read_source_line(file, line);
    const char *root = getenv("GEM_SOURCE_ROOT");
    if (root && root[0] && gem_try_source_in(root, file, line)) return 1;
    if (gem_read_source_line(file, line)) return 1;
    if (gem_exe_dir() && gem_try_source_upward(gem_src_dir, file, line)) return 1;
    if (getcwd(gem_src_dir, sizeof(gem_src_dir)) && gem_src_dir[0] == '/') {
        /* The cwd itself was tried above; start at its parent. */
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

/* Print the source line at file:line with a gutter, mirroring compile-error
 * format. Silently no-op if the source can't be found (see above). */
static void gem_print_source_context(const char *file, int line) {
    if (!file || !file[0] || line <= 0) return;
    if (!gem_find_source_line(file, line)) return;
    int gw = 1; int n = line; while (n >= 10) { gw++; n /= 10; }
    fprintf(stderr, "  --> %s:%d\n", file, line);
    fprintf(stderr, " %*s |\n", gw, "");
    fprintf(stderr, " %d | %s\n", line, gem_src_line);
    fprintf(stderr, " %*s |\n", gw, "");
}

/* Print an uncaught error: "[Runtime Error]: msg" (or the header `head`
 * when given, e.g. naming a spawned process), the source line of the
 * innermost frame and the stack trace of the current process. */
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

/* ─── Runtime error ─── */

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
    /* If inside a process, use per-process pcall stack */
    if (gem_current_pid >= 0 && gem_current_pid < gem_proc_hwm) {
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        if (proc->state != GEM_PROC_FREE && proc->state != GEM_PROC_DEAD) {
            if (proc->pcall_depth > 0) {
                proc->pcall_depth--;
                gem_pcall_longjmp(&proc->pcall_stack[proc->pcall_depth], msg);
            }
            /* Uncaught in the main user process: print and exit non-zero.
             * A spawned process gets a crash report (like Erlang's error
             * logger) and dies; monitors/links still see the message as
             * the exit reason through DOWN/EXIT messages. */
            if (gem_current_pid == gem_main_pid) {
                gem_print_runtime_error(msg);
                exit(1);
            }
            gem_report_process_crash(gem_current_pid, msg);
            proc->exit_reason = strdup(msg);
            gem_call_depth = 0;
            longjmp(proc->proc_jmp, 1);
        }
    }

    /* Main thread: use global pcall stack */
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
