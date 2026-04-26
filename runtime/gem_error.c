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
    fprintf(stderr, "error: %s\n", msg);
    gem_print_stack_trace();
    exit(1);
}

void gem_error(const char *msg) {
    gem_raise_error(msg);
}
