/*
 * gem_error.c — Error handling, stack traces, pcall/setjmp globals.
 */

#include "gem.h"

/* ─── Call stack ─── */

GemFrame gem_call_stack[GEM_MAX_CALL_DEPTH];
int gem_call_depth = 0;

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

void gem_print_stack_trace(void) {
    int max = gem_call_depth < GEM_MAX_CALL_DEPTH ? gem_call_depth : GEM_MAX_CALL_DEPTH;
    for (int i = max - 1; i >= 0; i--) {
        fprintf(stderr, "  at %s (%s:%d)\n",
            gem_call_stack[i].name,
            gem_call_stack[i].file,
            gem_call_stack[i].line);
    }
}

/* Print the source line at file:line with a gutter, mirroring compile-error
 * format. Silently no-op if the file can't be opened or the line doesn't exist. */
static void gem_print_source_context(const char *file, int line) {
    if (!file || line <= 0) return;
    FILE *f = fopen(file, "r");
    if (!f) return;
    char buf[2048];
    int cur = 1;
    while (fgets(buf, sizeof(buf), f)) {
        if (cur == line) {
            size_t l = strlen(buf);
            while (l > 0 && (buf[l-1] == '\n' || buf[l-1] == '\r')) buf[--l] = 0;
            int gw = 1; int n = line; while (n >= 10) { gw++; n /= 10; }
            fprintf(stderr, "  --> %s:%d\n", file, line);
            fprintf(stderr, " %*s |\n", gw, "");
            fprintf(stderr, " %d | %s\n", line, buf);
            fprintf(stderr, " %*s |\n", gw, "");
            fclose(f);
            return;
        }
        cur++;
    }
    fclose(f);
}

void gem_print_runtime_error(const char *msg) {
    fprintf(stderr, "\n[Runtime Error]: %s\n", msg);
    if (gem_call_depth > 0) {
        int top = (gem_call_depth <= GEM_MAX_CALL_DEPTH ? gem_call_depth : GEM_MAX_CALL_DEPTH) - 1;
        gem_print_source_context(gem_call_stack[top].file, gem_call_stack[top].line);
    }
    if (gem_call_depth > 0) {
        fprintf(stderr, "Stack trace:\n");
        gem_print_stack_trace();
    }
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
    for (int i = max - 1, idx = 0; i >= saved; i--, idx++) {
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
    if (gem_current_pid >= 0 && gem_current_pid < GEM_MAX_PROCS) {
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        if (proc->state != GEM_PROC_FREE && proc->state != GEM_PROC_DEAD) {
            if (proc->pcall_depth > 0) {
                proc->pcall_depth--;
                gem_pcall_longjmp(&proc->pcall_stack[proc->pcall_depth], msg);
            }
            /* Uncaught in the main user process: print and exit non-zero.
             * Spawned processes still die silently per Erlang semantics —
             * monitors/links surface the reason through DOWN/EXIT messages. */
            if (gem_current_pid == gem_main_pid) {
                gem_print_runtime_error(msg);
                exit(1);
            }
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
