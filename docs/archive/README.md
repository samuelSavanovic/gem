# Archive

Measurements and design notes of earlier designs, kept for the numbers and reasoning they hold. Each file describes the code at the dates (and commits, where given) it names, not the current code; the current state is in `docs/` and the code comments it points to.

| File | Holds |
|---|---|
| [memory_management_history.md](memory_management_history.md) | The Boehm GC, whole-arena and process-tail designs that preceded region resets: their benchmark anchors and the reasoning behind pinned boxes and the TCO reset |
| [benchmark_first_runs_linux_vm.md](benchmark_first_runs_linux_vm.md) | The first recorded runs of the mini_redis, lox, gemgrep and jobqueue yardsticks on a Linux x86_64 VM |
| [stomp_broker_m6.md](stomp_broker_m6.md) | Load-test findings from the STOMP broker's first version: fan-out copy cost, backpressure options, poll fairness |
| [lsp_v1_decisions.md](lsp_v1_decisions.md) | Why the language server is written in Gem and runs as a `gem` subcommand |

Log directories these files cite under `benchmarks/logs/` are not in the repository (the directory is gitignored); the recorded runs that are in it live in `benchmarks/baselines/`.
