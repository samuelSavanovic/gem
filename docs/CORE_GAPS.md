# Core Language Gaps

Audit of missing primitives. Philosophy: small C runtime, implement as much as possible in Gem.

## Must be C (can't build in Gem)

| Gap | Why C-only | Status |
|-----|-----------|--------|
| **`delete(tbl, key)`** | No way to remove a key from the stb_ds hash map from Gem. You can set to nil but `has_key` still returns true, `keys()` still lists it. Fundamental hole. | **DONE** |
| **`pop(arr)`** | You can `push` but can't remove the last element. Array length is internal to GemTable — can't shrink it from Gem. Blocks stack/queue patterns. | **DONE** |
| **`sort(arr)` / `sort(arr, cmp)`** | C qsort with a Gem comparison callback. ~10-50x faster than a pure Gem quicksort. The compiler already has a hand-rolled insertion sort (codegen.gem:93). | **DONE** |
| **Negative array indexing** | `arr[-1]` for last element. Change in `gem_table_get`. Currently requires `arr[len(arr) - 1]` — 14 instances in the compiler alone. Huge ergonomic win. | **DONE** |
| **`eprint(...)`** | Print to stderr. The compiler had to hack this with `extern fn gem_eprint` in compiler_helpers.h. Real programs need stdout vs stderr distinction. | **DONE** |
| **`append_file(path, content)`** | read_file and write_file exist but no append mode. Needed for logging, incremental output. | **DONE** |
| **`floor(x)`, `ceil(x)`, `round(x)`** | Need `math.h`. `to_int` truncates toward zero, which is not `floor` for negatives. | **DONE** |
| **`abs(x)`** | Needs `math.h` fabs for float edge cases. Trivial for ints but should handle both. | **DONE** |
| **`pow(x, y)`** | Needs `math.h`. Can't exponentiate floats in Gem. | **DONE** |
| **`sqrt(x)`** | Needs `math.h`. | **DONE** |
| **`random()` / `random(n)`** | Needs C's random number generator. No way to generate randomness from Gem. | **DONE** |
| **Bitwise ops: `band`, `bor`, `bxor`, `bnot`, `bshl`, `bshr`** | Operate on the int64 inside the tagged value — no way to express this in Gem. Could be builtins or operators. | **DONE** |
| **`exit(code)`** | `error()` always exits with 1. The compiler uses `extern fn gem_exit_process`. No clean exit or custom exit codes. | **DONE** |
| **`argv`** | No access to command-line arguments. The compiler uses `extern fn gem_get_argc/gem_get_argv`. Should be a built-in array. | **DONE** — `argv()` returns array |
| **`getenv(name)`** | Needs C stdlib `getenv()`. | **DONE** |
| **`input()` / `read_line()`** | Read from stdin. Needs C stdio. | **DONE** — `input([prompt])` |
| **`values(tbl)`** | Complement to `keys()`. Needs C to iterate the internal arrays efficiently. | **DONE** |
| **`insert(arr, i, val)` / `remove_at(arr, i)`** | stb_ds has `arrins`/`arrdel` underneath but not exposed. Can't shift internal array from Gem. | **DONE** |
| **`file_exists(path)`** | Needs C `fopen` to probe for file existence. Returns bool. | **DONE** |
| **`dirname(path)`** | POSIX `dirname()` — needs C, modifies input buffer. Returns parent directory string. | **DONE** |
| **`path_join(dir, file)`** | String manipulation with `/` separator and absolute path handling. | **DONE** |
| **`normalize_path(path)`** | `realpath()` to resolve symlinks and `.`/`..`. Falls back to original path if file doesn't exist. | **DONE** |
| **`remove_file(path)`** | POSIX `unlink()`. Returns bool success/failure. | **DONE** |
| **`mkdir(path)`** | POSIX `mkdir()` with 0755 permissions. Returns bool. | **DONE** |
| **`list_dir(path)`** | `opendir`/`readdir` loop, returns array of filenames excluding `.` and `..`. | **DONE** |
| **`is_dir(path)`** | `stat()` + `S_ISDIR` macro. Returns bool. | **DONE** |

## Could be Gem stdlib

| Gap | Notes |
|-----|-------|
| **`min(a, b)` / `max(a, b)`** | Comparison + conditional. Could be variadic: `min(1, 2, 3)`. Fundamental for any real program. |
| **`clamp(val, low, high)`** | Trivial with min/max but common enough to include. |
| **`assert(cond, msg)`** | `if not cond then error(msg) end`. Common in tests and invariant checking. |
| **`table.sort(arr, cmp)`** | Gem wrapper around C `sort` builtin with default comparator. |
| **`table.slice(arr, start, len)`** | Easy in Gem with a loop + push. |
| **`table.index_of(arr, val)`** | Linear scan, easy in Gem. |
| **`table.concat(a, b)`** | Loop + push, easy in Gem. |
| **`table.copy(tbl)`** | `for k, v in tbl; copy[k] = v; end`. Shallow copy. |
| **`table.flat_map(arr, fn)`** | Gem, using concat. |
| **`table.zip(a, b)`** | Gem. |
| **`table.unique(arr)`** | Gem, using has_key for dedup. |
| **`table.count(arr, fn)`** | Count elements matching predicate. |
| **`table.flatten(arr)`** | Flatten nested arrays one level. |
| **`table.group_by(arr, fn)`** | Group elements by function result. |
| **`table.last(arr)`** | ~~`arr[len(arr) - 1]`. Trivial but very common pattern. Moot if negative indexing lands.~~ Moot — negative indexing works. |
| **`string.char_at(s, i)`** | ~~`chr(ord(s, i))`. Minor but the round-trip through ord/chr is non-obvious.~~ Moot — `s[i]` indexing works. |

## Evidence from the compiler itself

The compiler (largest Gem program) shows these pain points:

- **Hand-rolled sort** — `set_to_sorted_array` in codegen.gem was an insertion sort; **replaced with `sort(keys(free))` call**
- **14x `[len(x) - 1]`** — accessing the last element is verbose without negative indexing — **resolved: all 14 replaced with `[-1]` after negative indexing landed**
- **`extern fn` hacks** — compiler_helpers.h provided `gem_eprint`, `gem_exit_process`, `gem_get_argv`, `gem_get_argc`; **all replaced with `eprint()`, `exit()`, `argv()` builtins**; path utils (`gem_file_exists`, `gem_dirname`, `gem_path_join`, `gem_normalize_path`) **fully resolved** — promoted to builtins `file_exists()`, `dirname()`, `path_join()`, `normalize_path()`; `extern include "compiler_helpers.h"` and all `extern fn` declarations removed from main.gem
- **Char-by-char `.gem` check** — ~~main.gem still checks the file extension character by character~~ **resolved**: now uses `string.ends_with(path, ".gem")`
- **195x `out = out + ...`** — O(n²) string building in codegen because buf_* API is more verbose than `+`
- **18x `type(x) == "table" and x.tag == ...`** — ~~verbose pattern for what's essentially a tagged union check~~ **resolved**: replaced with `is_node(node, tag)` helper in both codegen.gem and main.gem
