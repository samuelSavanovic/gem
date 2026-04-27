# Pain Points — HTTP Server Stress Test

Issues discovered while building an HTTP/1.1 server in Gem.

## 1. `\r` escape was broken (FIXED)

**Severity:** Blocker — HTTP requires `\r\n` line endings.

The `\r` escape was added to the lexer but didn't actually work due to a bootstrap
chicken-and-egg problem. When a new escape sequence is added via `"\r"` in the lexer
source, the *old* compiler that compiles the new lexer doesn't know `\r` yet, so it
treats `"\r"` as a two-char literal (backslash + r). The bootstrapped output reaches
a fixed point with the wrong semantics — `\r` in user code produces `\` + `r` instead
of byte 0x0d.

**Root cause:** Self-referential string literal — the escape's *definition* uses the
escape it's defining.

**Fix:** Use `chr(13)` (integer → character builtin) instead of `"\r"` in both the
lexer and codegen's `escape_c_string`. This sidesteps the bootstrap cycle since `chr`
is a C runtime function, not a string literal.

**Lesson:** Any new escape sequence added to a self-hosting lexer must be bootstrapped
via a non-literal mechanism (like `chr(N)`). The string literal `"\X"` can't define
itself. This applies to any future escapes too (e.g., `\0`, `\x41`, `\u{...}`).

## 2. `extern fn` requires a separate .h file for declarations (FIXED)

**Severity:** Minor friction.

The Gem compiler emits `extern fn` wrappers that call the C function directly, but
the generated C file only `#include`s `gem.h`. If the C functions are defined in a
separate .c file, the C compiler can't find the declarations.

**Fix:** The compiler now auto-generates C forward declarations from the `extern fn`
type signatures. `extern include "net.h"` and the `net.h` header are no longer needed
for function declarations. `extern include` remains available for structs, typedefs,
and system headers.

## 3. JSON strings are awkward — FIXED

**Severity:** Cosmetic but real.

Producing `{"key": "value"}` in a Gem string is painful because `{` starts
interpolation. You must write `"\{\"key\": \"value\"}"` mixing brace escapes
with quote escapes.

**Fix:** Single-quoted strings (`'...'`) with no interpolation were added.
`'{"key": "value"}'` works directly.

## 4. No `\0` null byte escape (FIXED)

**Severity:** Not needed for HTTP but will matter for binary protocols.

**Fix:** `\0` escape sequence added to the lexer using the same `chr(0)` bootstrap
trick as `\r`. The codegen detects null bytes via `ord(ch) == 0` and emits `\0` in C
output. Note: embedded null bytes are truncated by the runtime's `strlen`-based string
representation — full support requires adding a length field to GemVal strings.

## 5. Builtin registration is 3 separate code locations — FIXED

**Severity:** Developer friction.

**Fix:** Refactored to a single data table mapping builtin name → C function name.
Adding a new builtin is now a one-line addition.

## 6. Server is single-threaded (no spawn per connection) — FIXED

**Severity:** Architectural limitation for stress testing.

The current server handles connections sequentially in the main loop. Using `spawn`
per connection would exercise the concurrency runtime, but `accept()` is a blocking
call that doesn't yield to the scheduler. Would need non-blocking sockets + scheduler
integration to make this work properly.

**Fix:** Added non-blocking I/O support to the runtime scheduler:
- New `GEM_PROC_IO_WAIT` process state in the scheduler
- `gem_io_yield(fd, for_write)` — marks current coroutine as waiting on an fd and yields
- Scheduler uses `poll()` to check fd readiness, avoids busy-spinning
- TCP builtins (`tcp_listen`, `tcp_accept`, `tcp_read`, `tcp_write`, `tcp_close`) handle non-blocking I/O natively
- Server now spawns a green thread per accepted connection

Performance: ~24K req/s at 100 concurrent connections, ~21K req/s at 400 connections.

**Update:** The original `extern fn` + `net.c` approach was replaced with the built-in
TCP builtins, which provide identical performance with no separate C file needed.

## Not pain points (things that worked well)

- TCP builtins worked seamlessly for POSIX sockets
- `pcall` caught malformed request errors gracefully
- String interpolation made HTTP response building clean
- `string.split`, `substr`, `string.index_of` worked well for HTTP parsing (`split`/`index_of` now live in `std/string`)
- `extern include` made C header inclusion straightforward
- The table-as-object pattern (`router.add`, `router.dispatch`) felt natural
