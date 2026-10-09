# Optimization Log

Shipped optimizations with their write-ups, work logs and before/after numbers. Forward-looking TODOs are in [`OPTIMIZATIONS.md`](OPTIMIZATIONS.md).

Kept for historical context — commit refs, design rationale, regression tests, and the work logs that explain *why* the chosen mechanism beat the alternatives. Grouped by the same categories as the active doc. Write-ups of the memory-management designs region resets replaced (Boehm GC, whole-arena resets, the process-tail analysis, pinned-roots TCO) and their benchmark anchors are in [`archive/memory_management_history.md`](archive/memory_management_history.md).

## Arena / Memory

### `GEM_DIAG=2` shows which loop pays for its resets ✓ Done (2026-10-09)
`GEM_DIAG=1` prints the program's reset totals at exit, all processes together, which shows the cost but not the loop. `GEM_DIAG=2` also prints a `gem_reset:` line on stderr for every reset that takes 1 ms or more: the Unix time, the pid, the kind (young, full or return), its duration and the remembered-log walk's share of it, the bytes in its region, copied and scanned, and the innermost Gem function with the line it last ran (`gem_diag_trace` in runtime/gem_copy.c). With `GEM_DIAG=2 TARGETS=mini_redis benchmarks/soak/run.sh` it shows where mini_redis's reset time goes under the soak load: the store's gen_server loop pays nearly all of them, its young resets mostly in the remembered-log walk over `db.data` (OPTIMIZATIONS.md, "A kept table written once is walked whole at every reset") and its long pauses in full resets ("Full resets still copy all a loop keeps, in one pause").

### Free a recursion's garbage at function return ✓ Done (2026-10-06)

Memory was reclaimed only at loop back-edges, so a recursion kept everything its calls allocated until a loop that was running before it finished an iteration: memory grew with the number of calls, not the depth. A function that can recurse records a `GemArenaPoint` at entry, and a return that leaves at least `GEM_ARENA_RESET_THRESHOLD` of the call's allocation unfreed runs a region reset from that point with the return value as the only root; the hysteresis (`ret_min`) makes a recursion that hands a growing result up its levels copy it geometrically less often, not once per level. runtime/gem_copy.c ("Region reset", "Return resets") has the mechanism; "can recurse" is a cycle in the call graph (`mark_recursive_fns` in compiler/callgraph.gem). `GEM_DIAG=1` also prints `ret_resets=` and `ret_copied=`.

Giving every function that calls something a point cost about 20 instructions a call: logstat +6.5% instructions, jobqueue +4–5%, lox +4–6% ("A return point costs about 20 instructions a call" in OPTIMIZATIONS.md). Hence the restriction to functions that can recurse.

Before → after (macOS arm64, M1 Pro; instruction counts and peak RSS against `benchmarks/baselines/2026-10-05_m1pro`, wall times from old and new binaries run back to back on battery power):
- `lox fib.lox 28`: peak RSS 1,990 → 14 MB, wall 1.5–2.0 → 1.2–1.3 s, instructions −2% (2 resets → 1,229, 1,227 of them return resets).
- `lox binary_trees.lox 12`: peak RSS 78 → 46 MB, instructions +5%, wall 2.7 → 2.9 s; 803 return resets copy 319 MB of returned subtrees on top of the loops' 1.36 GB.
- The other lox programs: instructions +2.4–2.8%, wall +0–3%, memory unchanged.
- A Gem `count(28)` making one table per call (the `count` case in `tests/check_recursion_memory.sh`): 597 → 15 MB; the same shape returning nil (`visit`): 461 → 9 MB.
- A recursion of 5,000 levels returning an array that grows by 200 strings a level (1M at the top): 0.15 → 0.20 s, peak 249 → 202 MB; the return resets copy 117 MB in 8 resets.
- logstat +0.2–0.5% instructions, gemgrep +0–0.3%, jobqueue +1.2–3%; their peak RSS unchanged.
- Self-compile (`build/gem compiler/main.gem --emit-c`): +3% instructions, peak RSS 105 → 118 MB (81 return resets copy 56 MB of returned ASTs; "Return resets copy a large live return value whole" in OPTIMIZATIONS.md).

### Promote what a reset keeps ✓ Done (2026-10-04)

A reset used to copy what was live in its loop's region into blocks that still belonged to the region, so every later reset copied the same survivors again: a loop's kept data cost `live size × resets`. A mark now holds a `base` and a `young` point, a reset normally frees only what is newer than `young` and then moves `young` past its copies, and a full reset from `base` drops the garbage among kept data (runtime/gem_copy.c, "Region reset", "Promotion", has the triggers and the remembered-log compaction). Kept objects are older objects for later resets, so the existing fix-ups (barrier, buffer walk, pin `seq`) cover them. Each log entry and mailbox node a reset visits is charged to the budget (64 and 256 bytes), so a loop that writes many tables made before it, or holds a promoted mailbox backlog, spaces its resets out instead of walking them all at every threshold. `GEM_DIAG=1` also prints full resets (`full=`) and the longest reset (`max=`). Kept memory stays below about twice (fourfold, when it is mostly live) what the last full reset kept, instead of 3 × the live data.

Before → after (Linux x86_64 VM, 4 cores, the same binaries rebuilt on each runtime; GEM_DIAG=1):
- `logstat --by ip` on 1M lines from a file: resets 0.87–0.99 s copying 526 MB → 0.32–0.43 s copying 0.7 MB (wall 5.4–5.9 s → 5.1–5.4 s).
- `examples/mini_redis` under `benchmarks/mini_redis/run.sh` (`PHASES="basic pipeline" N=50000`): resets 10.7 s → 3.6 s in `basic`, 12.5 s → 1.5 s in `pipeline`; copied 4.0 / 3.5 GB → 0.4 / 0.4 GB. LRANGE_100 p99 in `basic` 51 → 8.5 ms; LRANGE_600 in `pipeline` 2,074 → 3,903 requests/s (p99 509 → 278 ms). Other commands within the noise.
- `jobqueue` `backlog` (100,000 jobs): longest tick lag 445–555 ms → 250–480 ms (a run before this session's measured 1.4–1.7 s); peak RSS 780–960 MB → 472–475 MB; reset time unchanged at about 2 s, half of it now the walk of the written job table ("A kept table written once is walked whole" in OPTIMIZATIONS.md) and the longest reset a full one (0.25–0.49 s).
- `jobqueue --jobs 20000 --slow 0.1` (healthy 20 ms attempts killed by their 100 ms deadline during a pause): 8–16 per run → 0–5; peak RSS 180–194 → 101–104 MB. At 40,000 jobs 28 → 14.
- `gemgrep -rn e` on the 128 MB corpus: resets 1.33 s copying 2.1 GB → 0.94–1.0 s copying 1.6 GB (655 of 1,693 resets are a fresh mark's full first one).
- `lox binary_trees.lox 12`: copied 1.59 → 1.36 GB; the rest is fresh marks' first resets inside the recursion ("A loop's first reset is full" in OPTIMIZATIONS.md). `closures.lox`, `fib.lox` unchanged.
- Churn (a loop keeping 20,000 records in an older table and replacing or deleting one per iteration, 4M iterations): peak RSS 42 → 44 MB, wall 7.3 → 7.4 s; it copies 1.5 × as much (1.7 against 1.1 GB), since nearly every record lives long enough to be promoted.
- A process keeping one record per message it gets: at 100,000 records, resets 0.29 → 0.20 s, peak RSS 191 → 129 MB; at 300,000, resets 0.61 → 0.78 s and copied 214 → 316 MB (each record is copied at promotion and again by each full reset), peak RSS 381 → 288 MB, longest reset 0.25 s.
- A loop writing once into each of 500,000 tables made before it (20M iterations): 7.9 → 4.5 s, resets 3.5 → 0.54 s. A first version that compacted the log's whole window at every reset and charged nothing for it took 94 s.
- A receiver draining a 400,000-message backlog that arrived during its loop: resets 1.2 → 1.0 s (8.1 s with the walk uncharged); an 800,000-message backlog older than the loop: resets 33.5 → 1.0 s, wall 43.7 → 11.2 s.
- A 50,000-record list rebuilt at every iteration of a loop: peak RSS 152 → 264 MB (each replaced list stays kept until a full reset), time unchanged (13.3 → 14.0 s).

### Zero only the arena memory a reset hands out again ✓ Done (2026-10-04)
`gem_arena_alloc` cleared every allocation with `memset`, although a block comes from `mmap` already zeroed: in a `sample` profile of `examples/logstat` the `memset` was about 9% of the main thread. A block now records the highest `used` a region reset rewound it from (`dirty` in `GemArenaBlock`), and an allocation clears only what lies below it. logstat, 1M lines on stdin, `--by ip` (macOS arm64): 5.41 s → 4.93 s, same report.

### Region resets: sound in any call context, with hysteresis ✓ Done (2026-10-02)

Replaced the whole-arena reset (sound only for loops reachable from a process root through tail calls, guarded elsewhere by a runtime "depth-2 fence") with region resets. Each loop (`while`, TCO fn, mutual-TCO trampoline) takes a `GemArenaMark` at entry; its back-edge reset copies what is reachable from the memory allocated since the mark and unmaps the rest. Older memory is never moved, so callers' frames stay valid; older tables written since the mark are found through a write barrier + remembered log and fixed up in place, old buffers / pinned boxes / module slots / mailbox likewise. The process-tail analysis, the depth fence, the pcall skip and the "TCO function not reachable from a process root" warnings are gone; zero-arg tail calls reset too. The next reset waits for max(1 MB, 2 × copied + scanned / 2) bytes (max(1 MB, scanned / 2) after a full reset; `runtime/gem_copy.c`).

Supersedes "Tighten TCO function not reachable from process root warning (structural decrease)" and "Indirect-spawn PT tagging Stage B" (nothing left to warn about or tag).

Before → after (Linux x86-64, same machine):
- top-level loop pushing 5000 rows once live data > 1 MB: 18.3 s → 12 ms; 1.5 MB `buf_push` at top level: 67.7 s → 14 ms; top-level 5000 rows of 3 fields (review top1): 12.2 s → 9 ms.
- gen_server-style store with 5000-entry state, 1000 calls: 6.35 s → 14 ms.
- `while true` server loop in a spawned process called from a non-tail position (and, before, *any* caller once a non-tail call site existed): ~1 GB/s growth (4.9 GB after 5 s) → 7.9 MB flat; tail-called variant unchanged at 7.9 MB.
- zero-arg receive loop, 400k messages: 160 MB → 7 MB peak.
- 50/100/200 queued 1 MB messages draining through a reset loop: quadratic (0.75/3.2/14.4 s) → linear (200 msgs: 0.13 s).
- self-hosting compile of `compiler/main.gem`: 3.1 s / 1.36 GB peak → 3.8 s / 117 MB peak (1,300 resets, 0.84 s of reset work).
- spawn of 100k short processes with `std/http`, `std/json`, `std/log` loaded: 18 µs → 22 µs per spawn (module state copy; fixed by "Lazy per-slot module copy at spawn" below).

### Lazy per-slot module copy at spawn ✓ Done (2026-10-02)
Module-level bindings are per-process, and `spawn` used to deep-copy every slot of the parent into the child, used or not. A 10k- or 100k-entry top-level table cost every spawn its full size (and `std/http` spawns per connection). Now a spawn copies only light slot values; every other value goes into an immutable, refcounted **snapshot unit** per aliasing component, which the child copies into its arena on its first read of any of the unit's slots, and which the parent reuses for later spawns until it writes a slot of it or mutates a table stamped with it. CLAUDE.md ("Key Decisions", "Module globals are per-process") and `runtime/gem_copy.c` ("Module globals") have the mechanism.

Before → after (Linux x86-64, same machine, median of 3):
- 200 live children, parent holding a 10k-entry top-level array of strings, children never reading it: 317 ms / 199 MB peak → 7 ms / 12.7 MB.
- same with 100k entries: 4.1 s / 1.57 GB → 26 ms / 36 MB.
- same with 100k entries, every child reading the array once (`len(big)`): 4.1 s / 1.57 GB → 0.83 s / 41 MB (strings shared with the unit, one unit for all children).
- spawn + reply of 100k short processes with `std/http`, `std/json`, `std/log` loaded: 29 µs → 24 µs per spawn, the same as with no modules loaded (24–25 µs).
- tight loop reading a module binding (50M iterations): 1.94 s → 1.92 s; `fib(32)`: 270 ms → 275 ms (identical code, noise).
- self-hosting compile of `compiler/main.gem` (`--emit-c`): 4.13 s → 4.29 s mean of 8 (noise-level; 0.26% more instructions under callgrind for the same compiler source, 0.4% including the namespace pass); peak RSS 118 MB → 119 MB.

### Lower default `GEM_ARENA_RESET_THRESHOLD` ✓ Done (2026-04-30)
Default lowered from 16 MB to 1 MB. Threshold sweep at c100 on `/` showed 1 MB strictly dominates: same throughput (28.8k vs 28.9k req/s), p99 −3.6× (26.5ms → 7.3ms), peak RSS −14× (2.04 GB → 139 MB), idle RSS −10× (495 MB → 50 MB). `/bookmarks` validation at c50 (heavier per-request allocation) confirmed no regression: throughput unchanged (4041 vs 4007 req/s), p99 −15% (26.9ms → 23.0ms), peak RSS −14× (728 MB → 52 MB). The hypothesis "smaller threshold = more reset overhead" did not show up in numbers — live set after a request is tiny so reset cost is negligible.

### Reuse tcp_read buffer per process ✓ Done
`GemProcess` has a `read_buf`/`read_buf_cap` pair. `tcp_read` allocates the read buffer once per process and reuses it across calls, copying only the actual bytes read into an exact-size string for the return value.

### String builder for response construction ✓ Done
`build_string` creates a buffer, passes an `add` closure to the block and returns the finished string; `push` and `to_string` work on buffers. `std/http.gem` builds its responses with it.

## Codegen Output

### `x = x + y` auto-optimization ✓ Done
The compiler detects the self-append pattern `assign(name, binary("+", var(name), expr))` inside `while`/`for` loop bodies and emits `gem_string_append_to`/`gem_string_finish` instead of `gem_add`. Eligible variables must only appear in append patterns within the loop body (no non-append reads), and a module-level variable is eligible only in top-level code whose loop calls no user fn and none of `pcall`, `sort`, `build_string` or the `spawn` family (anything else could read its slot mid-loop). A per-loop C flag records that the variable holds the buffer the loop built; only then are appends in place and the variable turned back into a string after the loop, so a buffer the program passed in gets plain `+`. Handles chained concatenation (`x = x + a + b + c`), conditional appends inside `if`/`match`, and nested loops. Runtime fallback for non-string types (integers, floats) ensures correctness without static type information.

### Redundant `gem_push_frame` / `gem_pop_frame` for leaf functions ✓ Done (2026-05-04)
A leaf function — one whose body calls no Gem code and has no `receive_match` — emits its body without `gem_push_frame`/`gem_pop_frame` and without `gem_set_line`. Detection is `fn_is_leaf` in `compiler/codegen.gem`; which builtin calls a leaf may make is in the next entry. Predicates / accessors / pure arithmetic / control-flow / table-and-array literal builders qualify; bookkeeping helpers do not.

State plumbing: a global `in_leaf_fn` is set in `compile_fn` and `compile_closure_fn` and saved/restored alongside the existing `boxed_vars`/`in_top_level`/`local_names`/`fn_scope_locals` quartet. Two helpers (`pop_str` and `setline_str`) replace every `gem_pop_frame()` / `gem_set_line(...)` literal in the codegen templates; both return empty strings when `in_leaf_fn` is set. The `gem_push_frame` calls at fn entry are gated directly. Top-level `main` is never marked leaf so the outermost frame is always present.

Stack-trace impact: originally the leaf was invisible in traces (errors were attributed to the caller's call-site line, and a spawned leaf closure printed no location at all). Since 2026-10 a leaf records itself in a per-process *leaf slot* instead of a frame (gem.h `GemLeafSite`): `gem_leaf_site = &static_site` on entry, `gem_leaf_line = N` per statement, `gem_leaf_site = NULL` on return; reports and pcall's `stack` show it as the innermost frame, and the scheduler saves/restores the slot on every resume (a leaf's loop can yield). Cost, measured by instruction count (cachegrind) on a 1M-iteration loop calling `fn inc(x) x + 1 end`: 247 → 250 instructions per iteration (+3, +1.2%); a full frame push/pop with per-statement `gem_set_line` would have cost +24 (+10%). Wall time differences were within noise.

Coverage on the bootstrapped compiler: 50/121 named fns and 12/112 anonymous closures are leaf-elided. Microbench (5M iter tight loop calling `add(a, b)` and a 4-arm `classify(x)`): 140ms pre → 127ms post (−9.3%, three-run best). Bookmark HTTP `/` burst is within run-to-run noise (~1% delta either direction). The big bench savings would require many leaf calls per request; HTTP handlers mostly aren't leaf.

Regression: `examples/86_leaf_fn_elision.gem` covers a few leaf shapes (predicate, multi-arm if, table accessor) plus a non-leaf caller invoking leaf helpers in a loop.

### Leaf functions may call builtins; direct entry points for `len`, `type`, `ord` and `for` loops ✓ Done (2026-10-07)
A call of a builtin in `LEAF_BUILTINS` (compiler/builtins.gem: builtins that run no Gem code and never yield, such as `len`, `ord`, `find`, `push`, `error`, `print` and the `for` loop's `__for_len` / `__table_val_at`) no longer keeps a function from being a leaf (`leaf_safe_call` in codegen.gem); its arguments still count. An error such a builtin raises reports the leaf on top, from the leaf slot, as an arithmetic error in a leaf's body does (`examples/207_leaf_builtins.gem` checks the traces, the pcall stacks and a leaf whose loop yields). An extern fn call still makes a function non-leaf: its C body can be in the same C file, where the C compiler drops the leaf-slot store before the call as dead, and the guard-page rescue then can't name the function (`examples/108_stack_overflow_containment.gem`, test 5).

`DIRECT_BUILTINS` (compiler/builtins.gem) maps `len/1`, `type/1`, `ord/1`, `ord/2`, `__for_len/1`, `__table_key_at/2` and `__table_val_at/2` to inline functions in runtime/gem.h (`gem_len_1`, `gem_ord_2`, …) that take the arguments as C parameters, handle the argument types each is mostly called with inline (`len`: strings and tables; `ord`: strings; `type`: all; the `for` helpers: tables), and hand anything else to the builtin's `(env, args, argc)` function, so errors are unchanged.

Instructions retired, macOS arm64: logstat `--by ip` on the million-line log `benchmarks/logstat/run.sh` generates 24.29 → 21.11 G (−13%), `--by path` 24.32 → 21.20 G; the compiler compiling itself 11.17 → 10.57 G (−5.4%); `examples/lox` −0.5 to −1.7% on its six bench programs; gemgrep `-rc error` on a 64 MB corpus (`MB=64 benchmarks/gemgrep/run.sh` generates it) 10.23 → 9.95 G (−2.7%; most of the rest is libc's `regexec`). In the bootstrapped compiler's C (`bootstrap/stage0.c`) 188 functions are leaves, against 114 before, and 329 push a frame, against 402.

### Escape analysis for non-escaping closures ✓ Done (2026-04-30)
A pre-codegen pass marks `anon_fn` nodes that appear as the immediate argument of an allowlisted callee (`pcall`, `sort`) as `non_escaping`, provided the closure body contains no nested `anon_fn`s. Marked closures (a) skip contributing their captures to the enclosing function's `captured` set, so the enclosing params/locals stay unboxed, and (b) get a stack-allocated env in `compile_anon_fn` instead of an arena allocation. `spawn`/`spawn_link`/`spawn_monitor`/`send_after` deliberately stay off the allowlist — their closures run asynchronously in another coroutine and must keep the boxing-via-arena path. The conservative "no nested anon_fns" precondition sidesteps the case where an escaping inner closure could smuggle outer captures past the synchronous call boundary.

Scope deferred: block-syntax `each`/`map`/`filter`/`reduce` from std/table aren't allowlisted yet (their callee parses as `dot`, not `var`, and they're regular Gem fns rather than builtins where we can audit storage behavior). Same goes for `build_string`. Add to allowlist when a workload demands it.

### Mutual TCO via tail-edge SCC trampoline ✓ Done (2026-05-10)
Mutual tail-call cycles (`a → tail b → tail a`, through any number of functions) run at constant stack depth. `find_tail_call_sccs` (compiler/tco.gem) finds the strongly connected components of the tail-call graph; each viable member compiles to a `gem_sccbody_<name>` behind a thin trampoline wrapper (`scc_wrapper_for`), and an intra-SCC tail call stores its target and arguments in the global tail-call buffer (`gem_tail_fn`, `gem_tail_args` in runtime/gem_error.c) and returns instead of calling. CLAUDE.md ("Key Decisions", tail call optimization) has the bail-out conditions and the rules for the buffer. The STOMP broker's seven-function `writer_loop ↔ handle_frame ↔ handle_<command>` cycle used to die at a 175-frame ceiling per connection and now iterates at constant depth (`examples/96_mutual_tco.gem`); the original write-up is in [`archive/memory_management_history.md`](archive/memory_management_history.md) and the broker's sweep numbers in [`archive/stomp_broker_m6.md`](archive/stomp_broker_m6.md).

### Constant folding ✓ Done (2026-05-04)
`compiler/fold.gem` runs as a pre-codegen pass on the resolved AST (after load resolution, before `make_codegen`). Folds binop / unop where every operand is a literal: int/float/mixed arithmetic, string concat, all six comparisons, and `not`. `and`/`or` short-circuit on a literal-truthy / literal-falsy left even if the right is non-literal — matches runtime branch semantics.

Soundness corners: skip int `/` and `%` when rhs is `0` so `pcall (1/x)` still errors at runtime when x is statically zero via a non-folded path; unary `-` on a float literal uses `e.value * -1.0` rather than `0.0 - e.value` to preserve IEEE signed zero (`-0.0` test in `examples/25_runtime_edge_cases.gem`); `==`/`!=` across incompatible literal types fold to `false`/`true` (matches `gem_eq`'s type-check), but ordering ops (`< > <= >=`) only fold same-type pairs so the runtime type-error is preserved. Float overflow / int wrap on multiplication: Gem's int64 arithmetic wraps the same as the C runtime, so folded values match.

Pass shape: `fold(node)` recursively folds children first, then `try_fold_binop`/`try_fold_unop` on the parent. Walks into all expression-bearing nodes (call args, table values, array elements, interp parts, fn defaults, control-flow conds and bodies). Regression: `examples/87_constant_folding.gem`. Bootstrap roundtrip clean.

### Self-recursive tail call optimization ✓ Done
Codegen detects self-recursive calls in tail position (last expression in function body, propagating through if/else, match, receive, and block branches) and emits a `while(1)` loop with parameter reassignment + `continue` instead of a recursive call. `gem_push_frame` runs once on entry; `gem_yield_check` runs each iteration for cooperative scheduling. Multi-param reassignment uses temps to avoid ordering issues. A mutated-captured param gets a fresh pinned box per iteration. Functions with default or rest params store the args in `_tco_argv` and rerun the entry bindings (`goto _tco_rebind`); a function whose name a param, rest param or body-level `let` shadows skips TCO (`fn_def_is_tco` in compiler/tco.gem). Covers ~95% of OTP patterns (gen_server loops, supervisor restarts, recursive receive handlers).

## Table Access

### Hash int, float, bool and ref keys ✓ Done (2026-10-07)
An int key that missed the array fast path (entry k holds key k), and every float, bool or ref key, was found by a linear scan of the table's keys, so a set or index of sparse int ids, or an array filled in reverse, was quadratic. Those value keys now have an open-addressing index (`GemKeyIndex`, runtime/gem_core.c) from a key's hash to its position; its slots hold no key, only the hash and the position, and equality is checked against `t->keys`, so a reset that moves the keys array leaves it valid. It is built by the first lookup that misses the array fast path in a table of more than `GEM_TABLE_SCAN_MAX` entries, kept in step by appends, `delete` and `pop`, and hangs off the string-key index (`GemStrIndex.keyix`, with a string index of capacity 0 as its holder when the table has no string keys), so `sort`, `insert`, `remove_at` and copies of the table (a reset's included) drop it with the string index and the table header stays 64 bytes. Table, buffer and fn keys compare by identity and a reset can move them, so they are still scanned.

Before → after (macOS arm64, M1 Pro): filling 80,000 int keys in reverse (`t[n-1]` .. `t[0]`) 4.21 s → 0.01 s; 20,000 sparse int ids (`seen[id] = true`, then `has_key`) 3 ms (the old runtime's 3.1 s was a Linux x86_64 figure); `table.group_by` with 20,000 int groups 6 ms. `examples/lox` bench programs and `examples/logstat` (1M lines): no regression; against the build before, lox runs 4.6–6.2% fewer instructions and logstat 1.6–1.9% fewer, the lox gain from cc inlining `gem_table_str_pos` (OPTIMIZATIONS.md, "String-key table lookups are inlined only by cc's choice").

### Small tables scan their keys instead of indexing them ✓ Done (2026-10-06)
Every table `calloc`'d a string-key index (a hash table outside the arena) at its first string key, so each record cost a `malloc`/`free` pair and an index of 8 slots of 32 bytes (16 slots from its 7th key) plus `malloc`'s overhead, and a reset freed the indexes of the tables it dropped one by one. A table with at most `GEM_TABLE_SCAN_MAX` (8, runtime/gem.h) entries now has no index: `gem_table_str_pos` (runtime/gem_core.c), which every string-key get, set, `has_key`, `in`, `delete` and inline-cache miss goes through, compares each string key's length and then its bytes. A table gets its index when an append takes it past 8 entries with a string key in it, and keeps it through deletes. `sort` drops it (every key is an int afterwards); `insert`, `remove_at` and copies of an indexed table leave it to be rebuilt on the next lookup (`index_stale`, `gem_table_rebuild_index`), which builds one only for a table past the cutoff. Each table also counts its string keys (`nstr`, in what was padding in `GemTable`), so a lookup in a table without string keys answers at once, and `x in t` tests values exactly when the table has no string key, as SPEC.md says (it used to test keys on a record whose string keys had all been deleted).

A cutoff of 4 cost lox 0.5–2.5% more instructions than 8; 16 was within 0.5% of 8.

Before → after (macOS arm64, M1 Pro; binaries built from the commits before and after, run back to back; instructions and peak RSS from `/usr/bin/time -l`):
- `examples/lox`, six bench programs: instructions −9 to −21% (`closures.lox 100000` 27.5 → 22.1 G, `binary_trees.lox 12` 42.8 → 33.7 G, `fib.lox 28` 17.1 → 14.4 G, `mandelbrot` −9%), wall −10 to −24%, peak RSS −24 to −41% (`binary_trees` 44 → 26 MB).
- `examples/jobqueue` `backlog` (100,000 jobs): peak RSS 559 → 396 MB, instructions 27.8 → 24.0 G.
- `examples/logstat` (1M lines, from a file): instructions −10%, wall −10%, peak RSS unchanged (239 MB, most of it the input file, held twice at the peak).
- Self-compile (`build/gem compiler/main.gem --emit-c`): 13.3 → 12.4 G instructions, peak RSS 112 → 91 MB.
- 100,000 seven-field records pushed onto an array (`{id: i, status: "done", attempts: ["ok"], result: i * 2, error: nil, submitted_at: 1.5, finished_at: 2.5}`): peak RSS 121.5 → 104 MB. Reset copies of a table leave its index unbuilt until a lookup, so many of the old runtime's kept records had none either.

### O(1) appends and misses on arrays ✓ Done (2026-10-04)
`t[len(t)] = v` searched every key before appending, so building an array by index, and the runtime's own `keys`, `values`, `list_dir`, `argv` and sqlite result rows (all built that way), were quadratic. A table now carries `is_array` (entry i has key i): on such a table an int key at or past the end is appended, and a lookup or `has_key` past the end is a miss, without a search. `push` uses the flag too; on a table that is not an array it is `t[len(t)] = v`, where it used to add a second entry with an existing key. `keys` + `values` of a 40,000-entry table: 2,110 ms → 2 ms; of 1M entries: 30 ms. Sparse int keys were still searched until "Hash int, float, bool and ref keys" above.

### Inline caching for `.field` access ✓ Done
Codegen emits a `static GemICacheSlot` per `.field` access site. On cache hit (same table + same shape_id), returns `t->vals[cached_index]` directly — no hash, no `gem_string()` allocation. A miss goes through `gem_table_get_ic_miss` (runtime/gem_core.c), which does the full lookup and fills the slot. `shape_id` on `GemTable` is bumped by structural mutations (delete, pop, sort, insert, remove_at) but not by set/push (which don't move existing key→index mappings). Monomorphic (1 slot per site) — sufficient for AST walking where each access site typically sees one table shape.

### `for k, v in tbl` allocates a keys array ✓ Done
Desugaring now uses `__table_key_at` / `__table_val_at` to index directly into the table's storage arrays. No keys array allocation, no per-key re-lookup.

## Runtime Hot Paths

### Inline value constructors and operators ✓ Done (2026-10-04)
`gem_int`, `gem_float`, `gem_bool`, `gem_truthy`, `gem_val_eq`, `gem_eq`/`gem_neq`/`gem_not` and the comparisons are `static inline` in gem.h, and `gem_add`/`gem_sub`/`gem_mul`/`gem_lt` inline their int (for `+` also float) case and call `gem_<op>_slow` in gem_ops.c for every other operand type. Codegen is unchanged: it already emitted these calls for every operator, `if` and literal. In a `sample` profile of `examples/logstat` (`--by ip`, from a file) the out-of-line calls were about 15% of the main thread. logstat, 1M lines (macOS arm64): `--by ip` from a file 3.19 → 2.42 s, from stdin 3.02 → 2.25 s; `--by path` 3.25 → 2.39 s and 3.16 → 2.36 s; `--by hour` 2.72 → 1.85 s and 2.64 → 1.85 s (Python: 2.0 s).

## Runtime I/O

### Thread pool for async I/O (Phase 1) ✓ Done
4 OS worker threads (`runtime/gem_threadpool.c`) handle `read_file`, `write_file`, `append_file`, `exec`, and `extern blocking fn` when called from a spawned process. The coroutine yields on submission; a wake-pipe notifies the scheduler on completion. Top-level (non-coroutine) I/O remains synchronous. The Gem-facing API is unchanged.

### Non-blocking sockets for TCP builtins ✓ Done
TCP builtins (`tcp_accept`, `tcp_read`, `tcp_write`, `tcp_connect`) use non-blocking sockets + `gem_io_yield(fd, direction)` to yield to the scheduler's `poll()` set. No thread pool involvement — the scheduler resumes the coroutine directly when the fd is ready.

**Why not the thread pool?** Benchmarked both approaches with `wrk -t4 -c100 -d10s` against an HTTP server:

| Approach | Req/sec | Notes |
|----------|---------|-------|
| Non-blocking + poll (net.c extern fn) | ~24,000 | Original baseline |
| Thread pool (tcp builtins v1) | ~1,550 | 15x slower — each request needs 3 thread pool round trips (accept/read/write), 4 workers caps throughput |
| Non-blocking + poll (tcp builtins v2) | ~23,800 | Matches baseline |

The thread pool adds per-operation overhead: mutex lock → enqueue → cond signal → worker thread pickup → execute → wake-pipe write → scheduler drain → process scan → resume. For socket ops on localhost where the actual I/O is microseconds, this coordination overhead dominates. With 4 workers and 3 ops/request, max throughput is ~4/3 ≈ 1.3k req/s — matches the 1,550 observed.

**Lesson:** Thread pool is correct for file I/O and `exec` (where the kernel provides no readiness notification). For sockets, always use non-blocking + readiness notification (poll, kqueue, epoll).

### Timer min-heap ✓ Done
Replaced the 256-slot fixed timer array with a dynamic min-heap keyed by `deadline_ms` in `runtime/gem_scheduler.c`. Insert is O(log n), `gem_fire_timers` pops expired entries from the root in O(log n) per fired timer, and `gem_earliest_timer_deadline` is O(1) (peek root). The 256-slot cap and "timer table full" failure mode are gone. Cancel-by-ref is still an O(n) linear scan to locate the ref before a heap remove (sift-down + sift-up); a side index ref→heap-pos would make it O(log n) if cancel ever becomes hot. Verified by `examples/85_timer_heap_capacity.gem` (1000 concurrent timers, in-order fire, 200 cancels).

### Lazy-paged coroutine stacks via mmap ✓ Done (2026-10-02)
Shipped together with stack-overflow containment (SPEC §"Stack depth"; `runtime/gem_scheduler.c` "Process stacks"). Every process stack, main included, is now an mmap'd block with a 64 KB `PROT_NONE` guard between minicoro's header and the stack, and `GEM_CORO_STACK_SIZE` went from 256 KB to 8 MB, the same as main. The old entry assumed malloc'd stacks were committed up front. On Linux glibc they were not: 256 KB is above the mmap threshold, so they were lazily paged too. Measured on Linux x86_64 with `VmRSS`, 1000 idle processes take 22.6 MB both before and after (≈16.5 KB per process, mostly arena and the stack's top pages). What changed is reserved address space: 8 GB with all 1024 slots in use, against 256 MB before. That is free on 64-bit Linux (`MAP_NORESERVE`, default overcommit) and macOS. Under `vm.overcommit_memory=2` it is charged in full, and spawn fails with a catchable "coroutine creation failed" once the commit limit is hit. Build with a smaller `-DGEM_CORO_STACK_SIZE` there.

Spawn cost: mapping, guarding and unmapping a stack per spawn made spawn+exit about 2.3x slower (200k spawn/exit: 1.5 s before, 3.5 s after). Released stacks go to a LIFO cache (`gem_stack_cache`) of at most `GEM_STACK_CACHE_MAX` (1,024) stacks, in runtime/gem_scheduler.c. The cache can't hold more stacks than were alive at once, and each is trimmed on release, so the size costs address space, not memory. An earlier cap of 128 made every exit an 8 MB `munmap` and every spawn an `mmap` + `mprotect` once more than 128 processes churned: 200k spawn/exit with 1000 alive took 3.2 s on macOS and 3.7 s on Linux, against 0.8 s and 1.8 s at the parent commit.

On release, everything below the top 16 KB of the *stack* (not of the mapping, which has a trailing page beyond the stack; on 16 KB-page macOS that page alone filled the kept region, so the real top page was discarded and re-faulted on every spawn) is handed back with `madvise` (`MADV_DONTNEED`, or `MADV_FREE_REUSABLE` on macOS, the only one of the three that lowers `phys_footprint` there). `madvise` over 8 MB costs several microseconds on macOS even when nothing in the range is resident, so a single `mincore` over the 64 KB below the kept region decides first: stacks are touched from the top down, so if none of those pages is resident, nothing deeper is either and the `madvise` is skipped. A C frame larger than 64 KB that skipped the probed pages could leave deeper pages resident in the cached stack until it is reused; that costs memory, not correctness.

Measured after these fixes, 200k spawn/exit: macOS arm64 1.80 / 0.78 / 0.99 s with 1 / 100 / 1000 alive (main: 1.70 / 0.66 / 0.82 s); Linux x86_64 3.7 / 1.3 / 1.5 s (main: 3.7 / 1.6 / 1.8 s). Memory stays bounded: on Linux, four rounds of 128 processes recursing 20,000 deep, then 2000 small spawns, end at 14 MB RSS; on macOS, four rounds of 128 processes recursing about 6 MB deep, then 2000 small spawns, end at 27 MB `phys_footprint`.

On macOS, `ps` and `top` report RSS well above `phys_footprint` after deep processes exit (381 MB against 21 MB in the test above). That is how `MADV_FREE_REUSABLE` works: the pages are reclaimable but stay counted until the system needs them. Use `phys_footprint` (`footprint` or Activity Monitor's Memory column), not RSS, when looking for a stack leak on macOS. Call overhead of the soft limit check in `gem_push_frame` (one load and one compare) is within noise: fib(35) 1.07 s before vs 1.11 s after, averaged over 5 runs with ±10% run-to-run spread; self-compile of `compiler/main.gem` was 3.0–3.3 s in both.

Not done: ASan builds. ASan's own SIGSEGV reporting is replaced by the overflow handler, which hands non-guard faults to the default action, and minicoro's ASan fiber hooks were not exercised with the mmap'd stacks.

## Strings

### Static string literals and `type()` results ✓ Done (2026-10-04)
Codegen emitted `gem_string_with_len("...", N)` for every string literal and `gem_string("k")` (`gem_string_with_len` when the key held a NUL) for every record-literal and field-assignment key, so each evaluation allocated and copied the bytes; `type()` returned a fresh string too, so `type(v) == "string"` allocated two strings per check (std's argument checks run one per argument). Literals and keys now compile to `GEM_STR_LIT("...", N)` (gem.h), a `GemVal` whose `sval` points at the C string constant, and `type()` and the constant cases of `to_string` return such values. Sound because no runtime path writes a string's bytes in place or frees a string it did not copy, region resets leave strings outside the region alone, and copies to another process duplicate the bytes. 1M calls (macOS arm64): `string.index_of` on a 115–120-byte line 174 → 72 ms (`find`: 17 ms); `type(s) == "string"` 131 → 18 ms (with "Inline value constructors and operators": 53 ms and 8 ms). `examples/logstat`, 1M lines (macOS arm64), with `gem_strlen_check` and `gem_current_arena` made `static inline` in the same change (2–3% of it): `--by ip` from a file 5.18 → 3.19 s, from stdin 4.74 → 3.02 s; `--by hour` 3.96 → 2.72 s and 3.77 → 2.64 s (Python: 2.0 s).

### `find` builtin ✓ Done (2026-10-04)
`find(s, needle, start)` searches with `memchr` (+ `memcmp` for longer needles) in C. It replaced std/string's private `find`, which scanned 32 positions in Gem and then tested growing chunks with `str_replace`, and std/http's runtime extern `gem_bytes_find`. `string.index_of`, `contains` and `split` are now one `find` per match. `examples/logstat` (eight `index_of` and one `split` per line), 1M lines, macOS arm64: from a file 11.3–13.1 s → 4.5–6.0 s, from stdin 7.9–9.4 s → 4.3–5.7 s (Python: 2.0 s).

## Compiler

### The lexer builds string tokens in buffers ✓ Done (2026-10-07)
The lexer took each string literal's text a character at a time (`val = val + source[pos]`), and the in-place append doesn't apply to those loops (they also read and reset `val`), so every character copied the text so far: `gem --check` on a file holding one 200 KB string literal took 2.2 s, and one with a 1 MB literal about a minute. The string-scanning loops in compiler/lexer.gem now push into a buffer and take the token's text with `to_string` at the end: the 1 MB literal checks in 0.07 s (macOS arm64).

## std/json

### Fast path for escape-free strings in parse ✓ Done (2026-10-03)
`read_string` (std/json.gem, scanner) first scans for the closing `"`, checking for `\` and control bytes on the way, and returns one `substr` when the string has no escapes; only a string with an escape gets a buffer, which then copies whole runs between escapes instead of pushing byte by byte. Parsing a 2.5 MB string-heavy document (20,000 records of four short strings and a three-string array, best of 5): 514 ms with the fast path disabled, 371 ms with it (1.4x); 940 ms with the parser as it was before the rewrite in commit a6d6942, which pushed every byte into a buffer.

## std/http

### Hardened server back to origin/main's throughput ✓ Done (2026-10-03)
The round-2 hardening of std/http (request validation, deadlines, connection tracking by the server process) cost a quarter to a third of the throughput: `parse_head` alone was about 58k instructions for a 2-line head, spent in std/string's Gem byte loops (`index_of`, `split`, `lower`, `trim`) and in per-request checks. What brought it back:
- A runtime extern helper `gem_bytes_find(s, needle, from)` (memchr + memcmp), next to `gem_bytes_span`, for every search in std/http (head terminator, line ends, request line, chunk lines); the `find` builtin ("`find` builtin" above) has since taken its place. `gem_bytes_span` keeps the lookup tables of its last 16 byte sets (a memcmp of the set instead of rebuilding a 256-byte table per call).
- `parse_head` makes one pass over the lines: per line a find, a span to the colon (which also rejects whitespace in the name), a span check for control bytes (which replaces the whole-head `str_replace` check), a lowercase only when the name has an uppercase letter, and a trim by span. Repeated headers are collected per name and joined once at the end (was `"{prev}, {value}"` per repeat, O(n²)).
- A route pattern without `:` parameters is matched by string equality, and the path is split at most once per match. A target without `?` or `#` skips `url.parse`'s byte loop. `status_text(200)` doesn't read `STATUS_TEXT` (which copies the table into each new connection process on first read). A Connection value that is one lowercase token skips the split.
- Request bodies: `read_exact` pushes each `tcp_read` chunk straight into the result buffer; `fill` used to copy every 8 KB chunk twice (`substr`, then `"{rd.data}{chunk}"`).
- Server bookkeeping: one catch-all receive with a dispatch on the tag (the patterns built a string literal per comparison: 41 per connection), int pid keys instead of `"{pid}"`, and the server takes every queued message before it monitors the connections registered meanwhile: a short connection's `_http_closing` is usually queued right behind its `_http_conn`, so it needs no monitor and sends no DOWN.

Measured on Linux x86_64 (4 cores, shared with other jobs, so ±10%), a minimal app answering `GET /x` with `http.ok("hello")`; `bench.c` load generator with 16 connections, 4 s per run, mean of 3 alternating runs; callgrind on a fixed 2,000 requests:

| | origin/main (pre-hardening) | hardened, before | after |
|---|---|---|---|
| keep-alive, req/s | 14.0k | 10.6k | 14.3k |
| connection per request, req/s | 8.7k | 5.7k | 8.4k |
| keep-alive, instructions/request | 115k | 160k | 79k |
| connection per request, instructions/request | 221k | 339k | 187k |
| 8 MB POST body (Content-Length) | 75 ms | 120 ms | 57 ms |
| 12,000 repeated header lines (60 KB head) | 55 ms | 530 ms | 15 ms |

What is left per connection is mostly runtime work: the spawn (copying the closure env with the router, and the module slots: about 23k instructions), the server process's wakeup, and the arena teardown at exit.

## Scheduler / Concurrency

### Growable process table, scheduler without table scans ✓ Done
The process table was a static array of 1,024 slots, and every scheduler pass scanned all slots up to the high-water mark: picking READY processes, checking deadlines, building the poll set, checking thread-pool completions. Both changed together (runtime/gem_scheduler.c, "Process table" and "Run state"):

- **Table.** `gem_proc_table` is a `PROT_NONE` reservation for `GEM_MAX_PROCS` (262,144) slots, made accessible 64 slots at a time as the high-water mark grows; it never moves, so `GemProcess *` pointers stay valid across spawns. `GEM_MAX_PROCS=<n>` in the environment lowers the limit. New slots come from the high-water mark up to 1,024, then from freed slots (FIFO), then from the mark again, so programs with fewer than 1,024 live processes get the same slots as before (and the same pids until a slot is reused: the pid modulus is now 262,144, so the 1,101st process of a program that spawns them one at a time is pid 262,221, not 1,101), and the table only grows to the larger of 1,024 and the peak live count. The old FIFO over a pre-filled free list would have cycled through every slot of a large table (with a 65,536-slot build of the old scheduler, a spawn/exit churn test timed out after 180 s).
- **Run state.** A three-level bitmap of READY slots lets a pass run them in slot order (the old scan's order, so every recorded interleaving is unchanged) at a cost proportional to the READY ones. Deadlines (`receive ... after`, `sleep`, tcp timeouts) sit in an indexed min-heap; fd waiters and thread-pool waiters in their own lists; WAITING processes are a count. Every state change goes through `gem_proc_set_state`. Expired deadlines wake after a pass, all at once, and only for waits that began in an earlier pass, which reproduces when the old scan noticed them (an adversarial review caught a first version that woke one deadline value per pass: 100 sleepers 1 ms apart beside a CPU-bound process woke up to 1 s late). Pool waiters are checked only when the wake pipe had a byte.

Measured on Linux x86_64 (4 cores), best of 3, binaries only:

| Workload | Before | After |
|---|---|---|
| 200k ping-pong round trips, no idle processes | 0.30 s | 0.29 s |
| same, 1,000 idle processes in `receive` | 0.84 s | 0.35 s |
| same, 10,000 idle (impossible before) | — | 0.52 s incl. spawning them |
| 200k spawn + exit | 4.6 s | 2.1 s |
| 200k spawn + exit, 1,000 alive | 4.8 s | 1.9 s |
| ring of 50,000 × 20 rounds (before: a 65,536-slot build of the old scheduler) | 11.9 s | 6.6 s |

Live processes now stop at memory mappings, not the table: four per process, and `spawn` refuses once the runtime's mappings (`gem_runtime_maps`) would pass 7/8 of Linux's `vm.max_map_count`, about 14,000 processes by default, so the running processes keep room to grow; 100,000 idle processes ran in 2.1 GB with the limit raised (OPTIMIZATIONS.md, "Per-process footprint"). The stack cache stays capped at 1,024 stacks instead of growing with the table, so a burst of tens of thousands of processes doesn't leave their stacks mapped.

An adversarial review of the first version found what more processes newly reach, fixed in the same change: the thread pool's request queue was a fixed 1,024 entries (1,500 concurrent `exec` calls: 453 raised "I/O queue full"; it now grows); a spawn that hit the mapping limit left no mappings for anything else (main's next arena block killed the program; hence the headroom); under `ulimit -v` the 5.5 GB table reservation crowded out the program (it now takes at most 1/16 of `RLIMIT_AS`); and the non-blocking poll after every pass costs O(fd waiters), which with 7,000 idle connections made 20,000 message round trips take 5.9 s on Linux x86_64. It now runs after every pass only up to 64 fd waiters; above that, the next poll waits twice as long as the last one took, at most 1 ms (20,000 round trips beside 5,000 idle connections on macOS arm64: 57–66 ms; spacing polls a fixed 1 ms apart took 36.9 s there, since one poll of 5,000 fds takes over a millisecond).

### `std/supervisor` keeps every restart time ✓ Done (std modernization)
`restart` used to push the time of each restart onto `state.restart_times` and never drop old entries, so restarts were O(n²) in the supervisor's lifetime restart count and its memory grew without bound (8,000 restarts of a permanent child: 2.9 s). Restart times outside `max_seconds` are now dropped at each restart (`note_restart` in std/supervisor, `check_intensity` in std/dynamic_supervisor); what remains (O(restarts in the window) per restart) is tracked in OPTIMIZATIONS.md.

## C Interop Hardening

### Arity / type validation at extern boundary ✓ Done (2026-05-05)
`emit_extern_validation` (`compiler/codegen.gem`) now emits an `argc`/type-tag prelude at the top of every `extern fn` and `extern blocking fn` wrapper. One `if (argc < N) gem_error(...)` plus one `if (args[i].type != VAL_X) gem_error(...)` per typed param, mirroring the diagnostic style of runtime builtins. Tag mapping: Int→VAL_INT, Float→VAL_FLOAT, String/Bytes→VAL_STRING, Bool→VAL_BOOL, Ptr→VAL_INT, Table→VAL_TABLE. Errors mention the declared Gem-level type name (e.g. "expected Bytes, got int") rather than the underlying VAL_ tag, so the message lines up with the `extern fn` signature. Regression: `examples/91_extern_validation.gem` covers missing-arg and wrong-type cases via `pcall`.
