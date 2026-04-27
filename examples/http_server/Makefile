CC ?= cc
CFLAGS = -std=c11 -O2
GC_FLAGS := $(shell pkg-config --cflags --libs bdw-gc 2>/dev/null || echo "-lgc")

GEM = ../../build/gem
RUNTIME_DIR = ../../runtime
RUNTIME_SRCS = $(wildcard $(RUNTIME_DIR)/gem_*.c)

.PHONY: build run clean

build: server

server: server.gem $(RUNTIME_SRCS) $(GEM)
	$(GEM) server.gem --emit-c > /tmp/gem_http_server.c
	$(CC) -o server /tmp/gem_http_server.c $(RUNTIME_SRCS) -I $(RUNTIME_DIR) $(CFLAGS) $(GC_FLAGS)

run: server
	./server

clean:
	rm -f server /tmp/gem_http_server.c
