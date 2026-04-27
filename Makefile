CC ?= cc
CFLAGS = -std=c11 -O2
GC_FLAGS := $(shell pkg-config --cflags --libs bdw-gc 2>/dev/null || echo "-lgc")
LDFLAGS = -lm -pthread

RUNTIME_DIR = runtime
RUNTIME_SRCS = $(wildcard $(RUNTIME_DIR)/gem_*.c)
RUNTIME_OBJS = $(patsubst $(RUNTIME_DIR)/%.c,$(BUILD_DIR)/%.o,$(RUNTIME_SRCS))
COMPILER_DIR = compiler
STAGE0 = bootstrap/stage0.c
BUILD_DIR = build
GEM = $(BUILD_DIR)/gem

.PHONY: build test test-json bootstrap clean

# Build the compiler from the checked-in stage0.c
build: $(GEM)

$(BUILD_DIR)/%.o: $(RUNTIME_DIR)/%.c $(RUNTIME_DIR)/gem.h
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< -I $(RUNTIME_DIR) $(CFLAGS) $(GC_FLAGS)

$(GEM): $(STAGE0) $(RUNTIME_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $(STAGE0) $(RUNTIME_OBJS) -I $(RUNTIME_DIR) -I $(COMPILER_DIR) $(CFLAGS) $(GC_FLAGS) $(LDFLAGS)

# Regenerate stage0.c from current compiler sources (requires working build/gem)
bootstrap: $(GEM)
	$(GEM) $(COMPILER_DIR)/main.gem --emit-c > /tmp/gem_stage0_new.c
	@# Verify the new stage0 can compile itself before replacing
	$(CC) -o /tmp/gem_stage0_verify /tmp/gem_stage0_new.c $(RUNTIME_SRCS) -I $(RUNTIME_DIR) -I $(COMPILER_DIR) $(CFLAGS) $(GC_FLAGS) $(LDFLAGS)
	/tmp/gem_stage0_verify $(COMPILER_DIR)/main.gem --emit-c > /tmp/gem_stage0_roundtrip.c
	@diff -q /tmp/gem_stage0_new.c /tmp/gem_stage0_roundtrip.c > /dev/null || (echo "FAILED: stage0.c does not roundtrip" && exit 1)
	cp /tmp/gem_stage0_new.c $(STAGE0)
	@echo "bootstrap/stage0.c regenerated and verified"

test: $(GEM)
	@bash examples/run_all.sh

test-concurrency:
	$(CC) -o /tmp/gem_test_concurrency runtime/test_concurrency.c $(RUNTIME_SRCS) -I $(RUNTIME_DIR) $(CFLAGS) $(GC_FLAGS) $(LDFLAGS)
	/tmp/gem_test_concurrency

test-json: $(GEM)
	@$(GEM) examples/json_parser.gem --run

clean:
	rm -rf $(BUILD_DIR) /tmp/gem_*
