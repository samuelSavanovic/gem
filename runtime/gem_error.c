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

void gem_print_stack_trace(void) {
    int max = gem_call_depth < GEM_MAX_CALL_DEPTH ? gem_call_depth : GEM_MAX_CALL_DEPTH;
    for (int i = max - 1; i >= 0; i--) {
        fprintf(stderr, "  at %s (%s:%d)\n",
            gem_call_stack[i].name,
            gem_call_stack[i].file,
            gem_call_stack[i].line);
    }
}

/* ─── Runtime error ─── */

void gem_raise_error(const char *msg) {
    /* pcall takes priority — innermost handler first */
    if (gem_pcall_depth > 0) {
        gem_pcall_depth--;
        GemPcallFrame *frame = &gem_pcall_stack[gem_pcall_depth];
        /* GC-allocate the message so it survives the longjmp */
        size_t len = strlen(msg) + 1;
        char *saved_msg = (char *)GC_MALLOC_ATOMIC(len);
        memcpy(saved_msg, msg, len);
        frame->error_msg = saved_msg;
        /* Snapshot the call stack */
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

    /* Process-level crash isolation: if inside a coroutine, longjmp to
       the process handler instead of exit(1). */
    if (gem_current_pid >= 0 && gem_current_pid < GEM_MAX_PROCS) {
        GemProcess *proc = &gem_proc_table[gem_current_pid];
        if (proc->state != GEM_PROC_FREE && proc->state != GEM_PROC_DEAD) {
            /* GC-allocate the reason string so it survives the longjmp */
            size_t len = strlen(msg) + 1;
            char *saved_msg = (char *)GC_MALLOC_ATOMIC(len);
            memcpy(saved_msg, msg, len);
            proc->exit_reason = saved_msg;
            gem_call_depth = 0;
            longjmp(proc->proc_jmp, 1);
        }
    }

    fprintf(stderr, "error: %s\n", msg);
    gem_print_stack_trace();
    exit(1);
}

void gem_error(const char *msg) {
    gem_raise_error(msg);
}
