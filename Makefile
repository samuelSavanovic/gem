CC ?= cc
CFLAGS = -std=gnu11 -O2
LDFLAGS = -pthread
LDLIBS = -lm

RUNTIME_DIR = runtime
RUNTIME_SRCS = $(wildcard $(RUNTIME_DIR)/gem_*.c)
RUNTIME_OBJS = $(patsubst $(RUNTIME_DIR)/%.c,$(BUILD_DIR)/%.o,$(RUNTIME_SRCS))
SQLITE_SRC = $(RUNTIME_DIR)/sqlite3.c
SQLITE_OBJ = $(BUILD_DIR)/sqlite3.o
RUNTIME_LIB = $(BUILD_DIR)/libgem_runtime.a
COMPILER_DIR = compiler
STAGE0 = bootstrap/stage0.c
BUILD_DIR = build
GEM = $(BUILD_DIR)/gem

.PHONY: build test test-json test-json-suite test-broken test-corpus test-lsp bootstrap clean

# Build the compiler from the checked-in stage0.c
build: $(GEM)

$(BUILD_DIR)/%.o: $(RUNTIME_DIR)/%.c $(RUNTIME_DIR)/gem.h
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< -I $(RUNTIME_DIR) $(CFLAGS)

$(SQLITE_OBJ): $(SQLITE_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< -I $(RUNTIME_DIR) $(CFLAGS)

$(RUNTIME_LIB): $(RUNTIME_OBJS) $(SQLITE_OBJ)
	ar rcs $@ $^

$(GEM): $(STAGE0) $(RUNTIME_LIB)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $(STAGE0) -I $(RUNTIME_DIR) -I $(COMPILER_DIR) $(CFLAGS) $(LDFLAGS) $(RUNTIME_LIB) $(LDLIBS)

# Regenerate stage0.c from current compiler sources (requires working build/gem)
bootstrap: $(GEM)
	$(GEM) $(COMPILER_DIR)/main.gem --emit-c > /tmp/gem_stage0_new.c
	@# Verify the new stage0 can compile itself before replacing
	$(CC) -o /tmp/gem_stage0_verify /tmp/gem_stage0_new.c -I $(RUNTIME_DIR) -I $(COMPILER_DIR) $(CFLAGS) $(LDFLAGS) $(RUNTIME_LIB) $(LDLIBS)
	/tmp/gem_stage0_verify $(COMPILER_DIR)/main.gem --emit-c > /tmp/gem_stage0_roundtrip.c
	@diff -q /tmp/gem_stage0_new.c /tmp/gem_stage0_roundtrip.c > /dev/null || (echo "FAILED: stage0.c does not roundtrip" && exit 1)
	cp /tmp/gem_stage0_new.c $(STAGE0)
	@echo "bootstrap/stage0.c regenerated and verified"

test: $(GEM) test-broken test-corpus test-lsp
	@bash examples/run_all.sh

test-broken: $(GEM)
	@bash tests/check_broken.sh

test-corpus: $(GEM)
	@bash tests/check_corpus.sh

test-lsp: $(GEM)
	@bash tests/lsp/smoke.sh
	@bash tests/lsp/smoke_symbols.sh
	@bash tests/lsp/smoke_definition.sh
	@bash tests/lsp/smoke_completion.sh

test-json: $(GEM)
	@$(GEM) examples/json_parser.gem --run

test-json-suite: $(GEM)
	@bash test_json_suite/run.sh

clean:
	rm -rf $(BUILD_DIR) /tmp/gem_*
