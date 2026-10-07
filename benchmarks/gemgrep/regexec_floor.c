/* regexec_floor — the C floor under examples/gemgrep: reads every file
 * under a directory, finds its lines with memchr and runs rx_exec (the
 * regex.gem extern, examples/gemgrep/rx.h) on each, and prints the number
 * of matching lines. Its instruction count is what gemgrep's file reading
 * and matching cost with no Gem code around the calls.
 *
 *   cc -O2 -I examples/gemgrep -o /tmp/regexec_floor benchmarks/gemgrep/regexec_floor.c
 *   /usr/bin/time -l /tmp/regexec_floor error <dir>      # macOS: "instructions retired"
 */

#define _GNU_SOURCE
#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "rx.h"

static void *rx;
static long matches;

static int visit(const char *path, const struct stat *st, int kind, struct FTW *ftw) {
    (void)ftw;
    if (kind != FTW_F) {
        return 0;
    }
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return 0;
    }
    uint8_t *data = malloc((size_t)st->st_size + 1);
    size_t len = fread(data, 1, (size_t)st->st_size, f);
    fclose(f);
    size_t start = 0;
    while (start < len) {
        uint8_t *nl = memchr(data + start, '\n', len - start);
        size_t stop = nl ? (size_t)(nl - data) : len;
        matches += rx_exec(rx, data, (int64_t)len, (int64_t)start, (int64_t)stop, 0, 0);
        start = stop + 1;
    }
    free(data);
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: regexec_floor PATTERN DIR\n");
        return 2;
    }
    rx = rx_new();
    int64_t rc = rx_compile(rx, argv[1], 0);
    if (rc != 0) {
        fprintf(stderr, "regexec_floor: %s\n", rx_error(rx, rc));
        return 2;
    }
    if (nftw(argv[2], visit, 16, FTW_PHYS) != 0) {
        perror(argv[2]);
        return 2;
    }
    printf("%ld\n", matches);
    rx_free(rx);
    return 0;
}
