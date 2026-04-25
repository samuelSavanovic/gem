/*
 * file_io.h — simple file reading helper for the Gem self-hosting compiler tests.
 * Returns file contents as a C string (malloc'd), or "" on failure.
 */

#include <stdio.h>
#include <stdlib.h>

static char* gem_read_file(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return "";
    fseek(f, 0, SEEK_END);
    long flen = ftell(f);
    rewind(f);
    char* buf = (char*)malloc(flen + 1);
    if (!buf) { fclose(f); return ""; }
    size_t nread = fread(buf, 1, flen, f);
    buf[nread] = '\0';
    fclose(f);
    return buf;
}
