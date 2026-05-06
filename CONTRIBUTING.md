# Contributing

## Workflow

`main` is protected. All changes land via pull request with green CI.

```bash
git checkout -b <topic>
# hack, commit
git push -u origin <topic>
gh pr create --fill
gh pr merge --auto --squash --delete-branch
```

`--auto` merges the PR the moment CI turns green, so the PR step is near-zero friction.

## CI

Every push and PR runs `.github/workflows/ci.yml` on `ubuntu-24.04` (x86_64) and `ubuntu-24.04-arm` (arm64). Each job:

1. `make build` — compile the compiler from `bootstrap/stage0.c`.
2. `make test` — run the numbered examples against `examples/expected_output.txt`.
3. `make bootstrap` + `git diff --exit-code bootstrap/stage0.c` — verify the checked-in `stage0.c` matches what the current compiler regenerates (the bootstrap fixed point).

If you change codegen, run `make bootstrap` locally and commit the regenerated `bootstrap/stage0.c` in the same PR — otherwise CI will fail the diff check.

## Before opening a PR

- `make test` passes locally.
- If you touched the compiler or runtime: `make bootstrap` succeeds and `bootstrap/stage0.c` is committed if it changed.
- If you added a builtin, keyword, syntax construct, or std module: editor extensions (`editors/vscode`, `editors/tree-sitter-gem`), `docs/SPEC.md`, and `docs/CHEATSHEET.md` are updated. See [`CLAUDE.md`](./CLAUDE.md) for per-surface checklists (editor extensions, spec updates, builtin/std module additions).
- Adversarial / edge-case coverage added as a numbered example under `examples/` with expected output appended to `examples/expected_output.txt`.

## Commit messages

Imperative subject under ~70 chars, body explaining the *why* when it isn't obvious from the diff. Match the existing `git log` style.
