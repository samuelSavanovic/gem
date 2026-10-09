# Known bugs

Bugs found and not yet fixed. Each entry has a minimal repro (checked
against `build/gem`), what goes wrong, and where the code is. A fix deletes
its entry in the same change, along with any **(bug)** rule in
`docs/BEST_PRACTICES.md` that exists because of it. Performance problems go in
`docs/OPTIMIZATIONS.md`, missing features in `docs/ROADMAP.md`.

## Compiler

### A variable named like a compiler temporary collides with it

The parser and `lower()` name their temporaries `_d<N>`, `_pdestr<N>`
(compiler/parser.gem), `_for_<role>_<N>`, `_match_<N>`, `_recv_<N>` and
`_val_<N>` (compiler/lower.gem), counting from 1 per compile. A user variable
of the same name in the same function is the same variable:

```gem
fn f()
  let _val_1 = 5
  let x = if true then _val_1 else 0 end
  x
end
print(f())        # nil, expected 5

fn g(t)
  let _d1 = "mine"
  let {a} = t
  print(_d1, a)   # {a: 1} 1, expected mine 1
end
g({a: 1})

fn h()
  let _match_2 = 7
  match 1
  when 1 then print(_match_2)
  end
end
h()               # C compile error: redefinition of 'gem_v__match_2'
```

The temporaries need names no program can write, or a counter that skips
the names the program uses.

## Runtime

## Standard library

## Language server

## Example programs
