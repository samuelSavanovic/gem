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

## 2. `extern fn` requires a separate .h file for declarations

**Severity:** Minor friction.

The Gem compiler emits `extern fn` wrappers that call the C function directly, but
the generated C file only `#include`s `gem.h`. If the C functions are defined in a
separate .c file, the C compiler can't find the declarations.

**Workaround:** Use `extern include "net.h"` and create a header file with forward
declarations.

**Possible improvement:** Auto-generate C forward declarations from the `extern fn`
type signatures — the compiler already knows the C types.

## 3. JSON strings are awkward

**Severity:** Cosmetic but real.

Producing `{"key": "value"}` in a Gem string is painful because `{` starts
interpolation. You must write `"\{\"key\": \"value\"}"` mixing brace escapes
with quote escapes.

**Possible improvements:**
- Raw string syntax (e.g., `r"..."` or `'...'`) that disables interpolation
- Single-quoted strings with no interpolation

## 4. No `\0` null byte escape

**Severity:** Not needed for HTTP but will matter for binary protocols.

Not supported yet. Would need the same `chr(0)` bootstrap treatment as `\r`.

## 5. Builtin registration is 3 separate code locations

**Severity:** Developer friction.

Adding a new builtin requires editing codegen.gem in 3 places:
1. `builtins["name"] = true` (the table)
2. `elif name == "name" → return {expr: "gem_make_fn(...)", ...}` (var ref)
3. The long `or` chain + `elif name == "name" → fn_name = "..."` (direct call opt)

This is error-prone. A data-driven approach (single table mapping name → C function)
would eliminate the duplication.

## 6. Server is single-threaded (no spawn per connection)

**Severity:** Architectural limitation for stress testing.

The current server handles connections sequentially in the main loop. Using `spawn`
per connection would exercise the concurrency runtime, but `accept()` is a blocking
call that doesn't yield to the scheduler. Would need non-blocking sockets + scheduler
integration to make this work properly.

## Not pain points (things that worked well)

- `extern fn` + `Ptr` interop worked smoothly for POSIX sockets
- `pcall` caught malformed request errors gracefully
- String interpolation made HTTP response building clean
- `split`, `substr`, `index_of` were easy to add as runtime builtins
- `extern include` made C header inclusion straightforward
- The table-as-object pattern (`router.add`, `router.dispatch`) felt natural
