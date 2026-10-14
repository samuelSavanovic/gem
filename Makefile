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
GEM = $(BUILD_DIR)/gem

.PHONY: FORCE build test test-json test-json-suite test-broken test-notes test-corpus test-cli test-project-root test-exe-path test-trace-source test-crlf test-io test-proc-limit test-socket-leak test-tls test-tls-online test-recursion-memory test-lsp test-example-apps bootstrap clean

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

# Regenerate stage0.c from current compiler sources (requires working build/gem)
bootstrap: $(GEM)
	$(GEM) $(COMPILER_DIR)/main.gem --emit-c > $(BUILD_DIR)/stage0_new.c
	@# Verify the new stage0 can compile itself before replacing
	$(CC) -o $(BUILD_DIR)/stage0_verify $(BUILD_DIR)/stage0_new.c -I $(RUNTIME_DIR) -I $(COMPILER_DIR) $(CFLAGS) $(LDFLAGS) $(RUNTIME_LIB) $(LDLIBS)
	$(BUILD_DIR)/stage0_verify $(COMPILER_DIR)/main.gem --emit-c > $(BUILD_DIR)/stage0_roundtrip.c
	@diff -q $(BUILD_DIR)/stage0_new.c $(BUILD_DIR)/stage0_roundtrip.c > /dev/null || (echo "FAILED: stage0.c does not roundtrip" && exit 1)
	cp $(BUILD_DIR)/stage0_new.c $(STAGE0)
	@echo "bootstrap/stage0.c regenerated and verified"

test: $(GEM) test-broken test-notes test-corpus test-cli test-project-root test-exe-path test-trace-source test-crlf test-io test-proc-limit test-socket-leak test-resolve test-tls test-recursion-memory test-lsp test-example-apps
	@bash examples/run_all.sh

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
