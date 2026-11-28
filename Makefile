CC ?= cc
CFLAGS = -std=gnu11 -O2
LDFLAGS = -pthread
LDLIBS = -lm

RUNTIME_DIR = runtime
RUNTIME_SRCS = $(wildcard $(RUNTIME_DIR)/gem_*.c)
RUNTIME_HDRS = $(RUNTIME_DIR)/gem.h $(RUNTIME_DIR)/stb_ds.h $(RUNTIME_DIR)/minicoro.h $(RUNTIME_DIR)/sqlite3.h
RUNTIME_OBJS = $(patsubst $(RUNTIME_DIR)/%.c,$(BUILD_DIR)/%.o,$(RUNTIME_SRCS))
SQLITE_SRC = $(RUNTIME_DIR)/sqlite3.c
SQLITE_OBJ = $(BUILD_DIR)/sqlite3.o
RUNTIME_LIB = $(BUILD_DIR)/libgem_runtime.a
TLS_OBJ = $(BUILD_DIR)/gem_builtins_tls.o
TLS_CFLAGS = $(BUILD_DIR)/tls_cflags
COMPILER_DIR = compiler
STAGE0 = bootstrap/stage0.c
BUILD_DIR = build
# keep test runs out of ~/.cache/gem (unless GEM_CACHE_DIR is already set)
export GEM_CACHE_DIR ?= $(CURDIR)/$(BUILD_DIR)/cache
GEM = $(BUILD_DIR)/gem

.PHONY: FORCE build install uninstall debug asan test-debug test-reset-stress fuzz-diff fuzz-frontend fuzz-parser test-fuzz test test-rules test-stress test-json test-json-suite test-broken test-notes test-corpus test-cli test-project-root test-exe-path test-trace-source test-crlf test-io test-proc-limit test-socket-leak test-tls test-tls-online test-recursion-memory test-stack-depth test-cache test-install test-lsp test-example-apps bootstrap clean

# the compiler, from the checked-in stage0.c
build: $(GEM)

$(BUILD_DIR)/%.o: $(RUNTIME_DIR)/%.c $(RUNTIME_HDRS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< -I $(RUNTIME_DIR) $(CFLAGS)

# tls_config.sh runs every build but only rewrites build/tls_cflags and build/tls_libs when they change
$(TLS_CFLAGS): FORCE
	@mkdir -p $(BUILD_DIR)
	@CC="$(CC)" sh runtime/tls_config.sh $(BUILD_DIR)

$(TLS_OBJ): $(RUNTIME_DIR)/gem_builtins_tls.c $(RUNTIME_HDRS) $(TLS_CFLAGS)
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

# $(PREFIX)/lib/gem is laid out like the repo: the compiler looks for std/ and runtime/ two levels above
# its real path. Old files go first: on arm64 macOS a signed binary overwritten in place gets killed.
PREFIX ?= /usr/local
INSTALL_ROOT = $(DESTDIR)$(PREFIX)/lib/gem

install: $(GEM)
	@mkdir -p $(INSTALL_ROOT)/build $(INSTALL_ROOT)/runtime $(DESTDIR)$(PREFIX)/bin
	rm -f $(INSTALL_ROOT)/build/gem $(INSTALL_ROOT)/build/libgem_runtime.a $(INSTALL_ROOT)/build/debug/libgem_runtime.a $(INSTALL_ROOT)/build/asan/libgem_runtime.a
	cp $(GEM) $(RUNTIME_LIB) $(INSTALL_ROOT)/build/
	@if [ -f $(BUILD_DIR)/tls_libs ]; then cp $(BUILD_DIR)/tls_libs $(INSTALL_ROOT)/build/; fi
	@for k in debug asan; do if [ -f $(BUILD_DIR)/$$k/libgem_runtime.a ]; then mkdir -p $(INSTALL_ROOT)/build/$$k && cp $(BUILD_DIR)/$$k/libgem_runtime.a $(INSTALL_ROOT)/build/$$k/; fi; done
	cp $(RUNTIME_DIR)/*.h $(INSTALL_ROOT)/runtime/
	rm -rf $(INSTALL_ROOT)/std
	cp -R std $(INSTALL_ROOT)/std
	ln -sf ../lib/gem/build/gem $(DESTDIR)$(PREFIX)/bin/gem
	@echo "installed: $(DESTDIR)$(PREFIX)/bin/gem"

uninstall:
	rm -rf $(INSTALL_ROOT) $(DESTDIR)$(PREFIX)/bin/gem

# GEM_RUNTIME=debug or asan links a program with build/debug or build/asan instead. debug: GEM_ASSERTs,
# poisoned freed memory, budgeted heap checks after resets. asan: the same plus ASan and UBSan.
DEBUG_CFLAGS = -std=gnu11 -O1 -g -fno-omit-frame-pointer -DGEM_DEBUG
ASAN_CFLAGS = $(DEBUG_CFLAGS) -fsanitize=address,undefined -fno-sanitize-recover=undefined

debug: $(GEM) $(BUILD_DIR)/debug/libgem_runtime.a
asan: $(GEM) $(BUILD_DIR)/asan/libgem_runtime.a

$(BUILD_DIR)/debug/%.o: $(RUNTIME_DIR)/%.c $(RUNTIME_HDRS) $(TLS_CFLAGS)
	@mkdir -p $(BUILD_DIR)/debug
	$(CC) -c -o $@ $< -I $(RUNTIME_DIR) $(DEBUG_CFLAGS) $$(cat $(TLS_CFLAGS))

$(BUILD_DIR)/asan/%.o: $(RUNTIME_DIR)/%.c $(RUNTIME_HDRS) $(TLS_CFLAGS)
	@mkdir -p $(BUILD_DIR)/asan
	$(CC) -c -o $@ $< -I $(RUNTIME_DIR) $(ASAN_CFLAGS) $$(cat $(TLS_CFLAGS))

$(BUILD_DIR)/debug/libgem_runtime.a: $(patsubst $(RUNTIME_DIR)/%.c,$(BUILD_DIR)/debug/%.o,$(RUNTIME_SRCS)) $(SQLITE_OBJ)
	ar rcs $@ $^

$(BUILD_DIR)/asan/libgem_runtime.a: $(patsubst $(RUNTIME_DIR)/%.c,$(BUILD_DIR)/asan/%.o,$(RUNTIME_SRCS)) $(SQLITE_OBJ)
	ar rcs $@ $^

test-debug: debug
	@GEM_RUNTIME=debug GEM_RESET_STRESS=1 bash examples/run_all.sh

# e.g. make fuzz-diff FUZZ_ARGS="--seeds 1:1000"
fuzz-diff: $(GEM)
	@python3 tests/fuzz/diff.py $(FUZZ_ARGS)

fuzz-frontend: $(GEM)
	@python3 tests/fuzz/fuzz_frontend.py $(FUZZ_ARGS)

fuzz-parser: $(GEM)
	@sh tests/fuzz/run_libfuzzer.sh $(FUZZ_ARGS)

# a quick run in make test so the fuzzers don't rot
test-fuzz: $(GEM)
	@python3 tests/fuzz/diff.py --seeds 1:9
	@python3 tests/fuzz/fuzz_frontend.py --iters 300

# regenerate stage0.c, but only if the new one compiles itself to the same C
bootstrap: $(GEM)
	$(GEM) $(COMPILER_DIR)/main.gem --emit-c > $(BUILD_DIR)/stage0_new.c
	$(CC) -o $(BUILD_DIR)/stage0_verify $(BUILD_DIR)/stage0_new.c -I $(RUNTIME_DIR) -I $(COMPILER_DIR) $(CFLAGS) $(LDFLAGS) $(RUNTIME_LIB) $(LDLIBS)
	$(BUILD_DIR)/stage0_verify $(COMPILER_DIR)/main.gem --emit-c > $(BUILD_DIR)/stage0_roundtrip.c
	@diff -q $(BUILD_DIR)/stage0_new.c $(BUILD_DIR)/stage0_roundtrip.c > /dev/null || (echo "FAILED: stage0.c does not roundtrip" && exit 1)
	cp $(BUILD_DIR)/stage0_new.c $(STAGE0)
	@echo "bootstrap/stage0.c regenerated and verified"

test: $(GEM) test-rules test-reset-stress test-fuzz test-broken test-notes test-corpus test-cli test-project-root test-exe-path test-trace-source test-crlf test-io test-proc-limit test-socket-leak test-resolve test-tls test-recursion-memory test-stack-depth test-cache test-install test-lsp test-example-apps
	@bash examples/run_all.sh
	@echo "examples at -O0 (run mode's build):"
	@CFLAGS=-O0 bash examples/run_all.sh

test-rules:
	@bash tests/check_rules.sh

test-cache: $(GEM)
	@bash tests/check_cache.sh

test-install: $(GEM)
	@bash tests/check_install.sh

# not in `make test`: takes about as long again
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

# not in `make test`: needs the internet
test-tls-online: $(GEM)
	@bash tests/check_tls_online.sh

test-recursion-memory: $(GEM)
	@bash tests/check_recursion_memory.sh

test-stack-depth: $(GEM)
	@bash tests/check_stack_depth.sh

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
