/* C helpers for bench.gem: a microsecond clock, and a blocking call that
   takes a known time, so the slow work on the pool is not exec's fork. */
#include <stdint.h>
#include <time.h>

static int64_t bench_now_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

static int64_t bench_block_ms(int64_t ms) {
    struct timespec ts = {ms / 1000, (ms % 1000) * 1000000};
    nanosleep(&ts, NULL);
    return ms;
}
