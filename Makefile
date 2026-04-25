CC ?= cc
CFLAGS = -std=c11 -O2
GC_FLAGS := $(shell pkg-config --cflags --libs bdw-gc 2>/dev/null || echo "-lgc")

RUNTIME = runtime/gem.c
RUNTIME_DIR = runtime
COMPILER_DIR = compiler
STAGE0 = bootstrap/stage0.c
BUILD_DIR = build
GEM = $(BUILD_DIR)/gem

.PHONY: build test test-json bootstrap clean

# Build the compiler from the checked-in stage0.c
build: $(GEM)

$(GEM): $(STAGE0) $(RUNTIME)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $(STAGE0) $(RUNTIME) -I $(RUNTIME_DIR) -I $(COMPILER_DIR) $(CFLAGS) $(GC_FLAGS)

# Regenerate stage0.c from current compiler sources (requires working build/gem)
bootstrap: $(GEM)
	$(GEM) $(COMPILER_DIR)/main.gem --emit-c > /tmp/gem_stage0_new.c
	@# Verify the new stage0 can compile itself before replacing
	$(CC) -o /tmp/gem_stage0_verify /tmp/gem_stage0_new.c $(RUNTIME) -I $(RUNTIME_DIR) -I $(COMPILER_DIR) $(CFLAGS) $(GC_FLAGS)
	/tmp/gem_stage0_verify $(COMPILER_DIR)/main.gem --emit-c > /tmp/gem_stage0_roundtrip.c
	@diff -q /tmp/gem_stage0_new.c /tmp/gem_stage0_roundtrip.c > /dev/null || (echo "FAILED: stage0.c does not roundtrip" && exit 1)
	cp /tmp/gem_stage0_new.c $(STAGE0)
	@echo "bootstrap/stage0.c regenerated and verified"

test: $(GEM)
	@bash examples/run_all.sh

test-json: $(GEM)
	@$(GEM) examples/json_parser.gem --emit-c > /tmp/gem_json_parser.c
	@$(CC) -o /tmp/gem_json_parser /tmp/gem_json_parser.c $(RUNTIME) -I $(RUNTIME_DIR) $(CFLAGS) $(GC_FLAGS)
	@/tmp/gem_json_parser

clean:
	rm -rf $(BUILD_DIR) /tmp/gem_*
