# Contributing

Changes go in through pull requests, and CI has to pass.

## Build and test

```sh
make build       # build/gem from bootstrap/stage0.c
make test        # everything below, plus the numbered examples
```

`make test` runs the numbered programs in `examples/` against `examples/expected_output.txt`, the larger example apps' own tests, the LSP smoke tests and the scripts in `tests/`. `tests/check_rules.sh` is one of them: it checks that every builtin appears in `docs/CHEATSHEET.md` and in both editor grammars.

## Bootstrap

`bootstrap/stage0.c` is the compiler's own C output. After changing anything in `compiler/`, or a std module the compiler loads (`std/string`, `std/json`), regenerate it and commit it with your change:

```sh
make bootstrap
```

It checks that the new `stage0.c` compiles itself to the same C before it replaces the old one. If your change alters the C the compiler emits, that check fails on the first try, because `build/gem` still has the old codegen. Build a new compiler first:

```sh
build/gem compiler/main.gem -o build/gem_stage1 && mv build/gem_stage1 build/gem
make bootstrap
```

## CI

`.github/workflows/ci.yml` runs on every pull request and every push to `main`, on Ubuntu 24.04 x86_64 and arm64:

1. `make build`
2. `make test`
3. `make bootstrap`, then `git diff --exit-code bootstrap/stage0.c`, so a stale `stage0.c` fails.

## Adding things

A builtin:

1. Write the C function in the right `runtime/gem_builtins_*.c` and declare it in `runtime/gem.h`.
2. Add it to `BUILTIN_FNS` in `compiler/builtins.gem`. Also add it to `LEAF_BUILTINS` there if it never calls Gem code and never yields to the scheduler.
3. Add it to `docs/CHEATSHEET.md`, to the `builtin` rule in `editors/vscode/syntaxes/gem.tmLanguage.json` and to the builtin regexes in `editors/tree-sitter-gem/queries/highlights.scm`.
4. Add a numbered example and run `make test`, then `make bootstrap`.

A std module:

1. Write `std/<name>.gem`, loading any std modules it needs with `load` and ending with `export <fn>, ...`. Give the module and each exported function a `##` doc comment.
2. Add it to `docs/CHEATSHEET.md`.
3. Add a numbered example that tests it.

A numbered example is the next free `examples/<number>_<name>.gem`, with its output appended to `examples/expected_output.txt`. A program that should fail to compile goes in `tests/broken/` and gets a line in `tests/check_broken.sh`.

## Commit messages

An imperative subject under about 70 characters, and a body when the reason for the change isn't obvious from the diff.
