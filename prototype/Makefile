CC = cc
CFLAGS = -Wall -Wextra -g $(shell pkg-config --cflags bdw-gc)
LDFLAGS = $(shell pkg-config --libs bdw-gc)

.PHONY: all clean run

all: gc_coro_test scheduler_test

gc_coro_test: gc_coro_test.c minicoro.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

scheduler_test: scheduler_test.c minicoro.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

run: all
	./gc_coro_test
	@echo ""
	./scheduler_test

clean:
	rm -f gc_coro_test scheduler_test
