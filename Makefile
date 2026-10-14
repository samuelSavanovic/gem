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
TLS_OBJ = $(BUILD_DIR)/gem_builtins_tls.o
TLS_CFLAGS = $(BUILD_DIR)/tls_cflags
COMPILER_DIR = compiler
STAGE0 = bootstrap/stage0.c
BUILD_DIR = build
# Programs the tests run (`gem file.gem`) cache their binaries here, not in
# the user's ~/.cache/gem; a GEM_CACHE_DIR set outside wins.
export GEM_CACHE_DIR ?= $(CURDIR)/$(BUILD_DIR)/cache
GEM = $(BUILD_DIR)/gem

.PHONY: FORCE build debug asan test-debug test-reset-stress fuzz-diff fuzz-frontend fuzz-parser test-fuzz test test-rules test-stress test-json test-json-suite test-broken test-notes test-corpus test-cli test-project-root test-exe-path test-trace-source test-crlf test-io test-proc-limit test-socket-leak test-tls test-tls-online test-recursion-memory test-cache test-lsp test-example-apps bootstrap clean

# Build the compiler from the checked-in stage0.c
build: $(GEM)

$(BUILD_DIR)/%.o: $(RUNTIME_DIR)/%.c $(RUNTIME_DIR)/gem.h
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< -I $(RUNTIME_DIR) $(CFLAGS)

# OpenSSL's flags, from runtime/tls_config.sh, which runs on every build
# and rewrites build/tls_cflags and build/tls_libs only when they change.
$(TLS_CFLAGS): FORCE
	@mkdir -p $(BUILD_DIR)
	@CC="$(CC)" sh runtime/tls_config.sh $(BUILD_DIR)

$(TLS_OBJ): $(RUNTIME_DIR)/gem_builtins_tls.c $(RUNTIME_DIR)/gem.h $(TLS_CFLAGS)
	$(CC) -c -o $@ $< -I $(RUNTIME_DIR) $(CFLAGS) $$(cat $(TLS_CFLAGS))

FORCE:

$(SQLITE_OBJ): $(SQLITE_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< -I $(RUNTIME_DIR) $(CFLAGS)

$(RUNTIME_LIB): $(RUNTIME_OBJS) $(SQLITE_OBJ)
	ar rcs $@ $^

$(GEM): $(STAGE0) $(RUNTIME_LIB)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $(STAGE0) -I $(RUNTIME_DIR) -I $(COMPILER_DIR) $(CFLAGS) $(LDFLAGS) $(RUNTIME_LIB) $(LDLIBS)

# Debug runtimes, next to build/libgem_runtime.a: a program built with
# GEM_RUNTIME=debug (or asan) links build/debug (build/asan) instead and is
# compiled with the same flags. debug: -DGEM_DEBUG (invariant asserts,
# poisoned freed memory, heap checks after resets, as many as a budget
# allows; gem.h "Debug runtime"). asan: the same plus AddressSanitizer and
# UBSan.
DEBUG_CFLAGS = -std=gnu11 -O1 -g -fno-omit-frame-pointer -DGEM_DEBUG
ASAN_CFLAGS = $(DEBUG_CFLAGS) -fsanitize=address,undefined -fno-sanitize-recover=undefined

debug: $(GEM) $(BUILD_DIR)/debug/libgem_runtime.a
asan: $(GEM) $(BUILD_DIR)/asan/libgem_runtime.a

$(BUILD_DIR)/debug/%.o: $(RUNTIME_DIR)/%.c $(RUNTIME_DIR)/gem.h $(TLS_CFLAGS)
	@mkdir -p $(BUILD_DIR)/debug
	$(CC) -c -o $@ $< -I $(RUNTIME_DIR) $(DEBUG_CFLAGS) $$(cat $(TLS_CFLAGS))

$(BUILD_DIR)/asan/%.o: $(RUNTIME_DIR)/%.c $(RUNTIME_DIR)/gem.h $(TLS_CFLAGS)
	@mkdir -p $(BUILD_DIR)/asan
	$(CC) -c -o $@ $< -I $(RUNTIME_DIR) $(ASAN_CFLAGS) $$(cat $(TLS_CFLAGS))

$(BUILD_DIR)/debug/libgem_runtime.a: $(patsubst $(RUNTIME_DIR)/%.c,$(BUILD_DIR)/debug/%.o,$(RUNTIME_SRCS)) $(SQLITE_OBJ)
	ar rcs $@ $^

$(BUILD_DIR)/asan/libgem_runtime.a: $(patsubst $(RUNTIME_DIR)/%.c,$(BUILD_DIR)/asan/%.o,$(RUNTIME_SRCS)) $(SQLITE_OBJ)
	ar rcs $@ $^

# The numbered examples on the debug runtime, under reset stress.
test-debug: debug
	@GEM_RUNTIME=debug GEM_RESET_STRESS=1 bash examples/run_all.sh

# Fuzzing (tests/fuzz/; not part of make test except test-fuzz's quick
# run). FUZZ_ARGS passes options: make fuzz-diff FUZZ_ARGS="--seeds 1:1000".
fuzz-diff: $(GEM)
	@python3 tests/fuzz/diff.py $(FUZZ_ARGS)

fuzz-frontend: $(GEM)
	@python3 tests/fuzz/fuzz_frontend.py $(FUZZ_ARGS)

fuzz-parser: $(GEM)
	@sh tests/fuzz/run_libfuzzer.sh $(FUZZ_ARGS)

# A few seeds and mutants on every make test, so the fuzzers keep working.
test-fuzz: $(GEM)
	@python3 tests/fuzz/diff.py --seeds 1:9
	@python3 tests/fuzz/fuzz_frontend.py --iters 300

# Regenerate stage0.c from current compiler sources (requires working build/gem)
bootstrap: $(GEM)
	$(GEM) $(COMPILER_DIR)/main.gem --emit-c > $(BUILD_DIR)/stage0_new.c
	@# Verify the new stage0 can compile itself before replacing
	$(CC) -o $(BUILD_DIR)/stage0_verify $(BUILD_DIR)/stage0_new.c -I $(RUNTIME_DIR) -I $(COMPILER_DIR) $(CFLAGS) $(LDFLAGS) $(RUNTIME_LIB) $(LDLIBS)
	$(BUILD_DIR)/stage0_verify $(COMPILER_DIR)/main.gem --emit-c > $(BUILD_DIR)/stage0_roundtrip.c
	@diff -q $(BUILD_DIR)/stage0_new.c $(BUILD_DIR)/stage0_roundtrip.c > /dev/null || (echo "FAILED: stage0.c does not roundtrip" && exit 1)
	cp $(BUILD_DIR)/stage0_new.c $(STAGE0)
	@echo "bootstrap/stage0.c regenerated and verified"

test: $(GEM) test-rules test-reset-stress test-fuzz test-broken test-notes test-corpus test-cli test-project-root test-exe-path test-trace-source test-crlf test-io test-proc-limit test-socket-leak test-resolve test-tls test-recursion-memory test-cache test-lsp test-example-apps
	@bash examples/run_all.sh

test-rules:
	@bash tests/check_rules.sh

# The run-mode binary cache (tests/check_cache.sh).
test-cache: $(GEM)
	@bash tests/check_cache.sh

# Not part of `make test` (about as long again): the examples, the example
# apps and the compiler compiling itself, with GEM_RESET_STRESS=1 (every
# loop resets at almost every back-edge).
test-stress: $(GEM)
	@bash tests/check_stress.sh

test-reset-stress: $(GEM)
	@bash tests/check_reset_stress.sh

test-broken: $(GEM)
	@bash tests/check_broken.sh

test-notes: $(GEM)
	@bash tests/check_notes.sh

test-trace-source: $(GEM)
	@bash tests/check_trace_source.sh

test-example-apps: $(GEM)
	@bash tests/check_example_apps.sh

test-crlf: $(GEM)
	@bash tests/check_crlf.sh

test-io: $(GEM)
	@bash tests/check_io.sh

test-proc-limit: $(GEM)
	@bash tests/check_proc_limit.sh

test-socket-leak: $(GEM)
	@bash tests/check_socket_leak.sh

test-resolve: $(GEM)
	@bash tests/check_resolve.sh

test-tls: $(GEM)
	@bash tests/check_tls.sh

# Not part of `make test`: needs the internet.
test-tls-online: $(GEM)
	@bash tests/check_tls_online.sh

test-recursion-memory: $(GEM)
	@bash tests/check_recursion_memory.sh

test-corpus: $(GEM)
	@bash tests/check_corpus.sh

test-cli: $(GEM)
	@bash tests/check_cli.sh

test-project-root: $(GEM)
	@bash tests/check_project_root.sh

test-exe-path: $(GEM)
	@bash tests/check_exe_path.sh

test-lsp: $(GEM)
	@bash tests/lsp/smoke.sh
	@bash tests/lsp/smoke_symbols.sh
	@bash tests/lsp/smoke_definition.sh
	@bash tests/lsp/smoke_completion.sh
	@bash tests/lsp/smoke_diagnostics.sh
	@bash tests/lsp/smoke_load_diagnostics.sh
	@bash tests/lsp/smoke_malformed.sh

test-json: $(GEM)
	@$(GEM) examples/json_parser.gem --run

test-json-suite: $(GEM)
	@bash test_json_suite/run.sh

clean:
	rm -rf $(BUILD_DIR)
