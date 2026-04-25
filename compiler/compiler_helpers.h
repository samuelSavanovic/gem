/*
 * compiler_helpers.h — C helper functions for the Gem self-hosting compiler.
 * Provides: argv access, path manipulation, keys() for tables.
 */

#ifndef COMPILER_HELPERS_H
#define COMPILER_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "gem.h"

/* ─── argv access via platform API ─── */

#ifdef __APPLE__
#include <crt_externs.h>
static int gem_get_argc(void) {
    return *_NSGetArgc();
}
static char* gem_get_argv(int64_t idx) {
    int argc = *_NSGetArgc();
    char **argv = *_NSGetArgv();
    if (idx < 0 || idx >= argc) return "";
    return argv[idx];
}
#else
/* Linux: read /proc/self/cmdline */
static int _gem_argc = 0;
static char **_gem_argv = NULL;
static int _gem_argv_loaded = 0;

static void _gem_load_argv(void) {
    if (_gem_argv_loaded) return;
    _gem_argv_loaded = 1;
    FILE *f = fopen("/proc/self/cmdline", "rb");
    if (!f) return;
    char buf[65536];
    size_t n = fread(buf, 1, sizeof(buf)-1, f);
    fclose(f);
    buf[n] = '\0';
    /* Count args */
    int count = 0;
    for (size_t i = 0; i < n; i++) {
        if (buf[i] == '\0') count++;
    }
    _gem_argc = count;
    _gem_argv = (char**)malloc(count * sizeof(char*));
    size_t pos = 0;
    for (int i = 0; i < count; i++) {
        _gem_argv[i] = strdup(buf + pos);
        pos += strlen(buf + pos) + 1;
    }
}

static int gem_get_argc(void) {
    _gem_load_argv();
    return _gem_argc;
}
static char* gem_get_argv(int64_t idx) {
    _gem_load_argv();
    if (idx < 0 || idx >= _gem_argc) return "";
    return _gem_argv[idx];
}
#endif

/* ─── Path manipulation ─── */

static char* gem_dirname(const char *path) {
    char *copy = strdup(path);
    char *dir = dirname(copy);
    char *result = GC_MALLOC(strlen(dir) + 1);
    strcpy(result, dir);
    free(copy);
    return result;
}

static char* gem_path_join(const char *dir, const char *file) {
    if (file[0] == '/') {
        /* Absolute path — return as is */
        char *result = GC_MALLOC(strlen(file) + 1);
        strcpy(result, file);
        return result;
    }
    size_t dlen = strlen(dir);
    size_t flen = strlen(file);
    char *result = GC_MALLOC(dlen + 1 + flen + 1);
    strcpy(result, dir);
    if (dlen > 0 && dir[dlen-1] != '/') {
        strcat(result, "/");
    }
    strcat(result, file);
    return result;
}

static char* gem_normalize_path(const char *path) {
    char *resolved = realpath(path, NULL);
    if (resolved) {
        char *result = GC_MALLOC(strlen(resolved) + 1);
        strcpy(result, resolved);
        free(resolved);
        return result;
    }
    /* If file doesn't exist yet, return the path as-is */
    char *result = GC_MALLOC(strlen(path) + 1);
    strcpy(result, path);
    return result;
}

/* ─── keys() alias ─── */

static GemVal keys(GemVal tbl) {
    return gem_keys(tbl);
}

#endif /* COMPILER_HELPERS_H */
