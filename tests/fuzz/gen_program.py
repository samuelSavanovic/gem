#!/usr/bin/env python3
"""Random Gem program generator for differential testing (tests/fuzz/diff.py).

    gen_program.py <seed>    # prints a program to stdout

The programs are deterministic (no time, pids, refs, table identity or
random()), always terminate (loops have small constant bounds, recursion
takes a depth that only goes down), and print what they compute, so two
builds of one program must print the same: default against
GEM_RESET_STRESS=1, -O2 against -O0. They lean on what region resets have
to get right: values kept across loop iterations, tables created before a
loop and filled inside it, module state written in loops, closures that
assign their captures, self tail calls, a mutual tail-call chain of up
to 50,000 calls, non-tail recursion (up to 5 levels), pcall, string
building, and messages between processes.

Sums, products and differences are reduced mod MOD (a prime; MOD is added
before a difference is reduced), and every other int is a small literal,
a counter, a length or a stored copy of such a result, so no int comes
near int64's range and the only divisor is MOD; no floats are used, so fused
multiply-adds can't make -O2 differ from -O0.
"""

import random
import sys

MOD = 10007


class Gen:
    def __init__(self, seed):
        self.r = random.Random(seed)
        self.seed = seed
        self.n = 0
        self.fns = []        # (name, nargs): user fns taking (d, ints...) and returning an int
        self.lines = []
        self.work = 1        # iterations of the loops around the code being generated

    def fresh(self, base):
        self.n += 1
        return "{}{}".format(base, self.n)

    def chance(self, p):
        return self.r.random() < p

    # ── Expressions over typed scopes ──
    # scope: {"int": [...], "str": [...], "arr": [...], "rec": [...]}

    def int_expr(self, sc, depth=0):
        r = self.r
        opts = ["lit", "lit"]
        if sc["int"]:
            opts += ["var", "var", "var"]
        if depth < 3:
            opts += ["add", "mul", "sub"]
            if sc["str"]:
                opts.append("len_s")
            if sc["arr"]:
                opts += ["len_a", "at", "sum"]
            if sc["rec"]:
                opts.append("field")
            if self.fns and depth < 2 and self.work <= 100:
                opts.append("call")
            opts.append("ifexpr")
            if depth < 2:
                opts.append("match")
        k = r.choice(opts)
        if k == "lit":
            return str(r.randrange(0, 100))
        if k == "var":
            return r.choice(sc["int"])
        if k == "add":
            return "({} + {}) % {}".format(self.int_expr(sc, depth + 1), self.int_expr(sc, depth + 1), MOD)
        if k == "mul":
            return "({} * {}) % {}".format(self.int_expr(sc, depth + 1), self.int_expr(sc, depth + 1), MOD)
        if k == "sub":
            return "({} - {} + {}) % {}".format(self.int_expr(sc, depth + 1), self.int_expr(sc, depth + 1), MOD, MOD)
        if k == "len_s":
            return "len({})".format(r.choice(sc["str"]))
        if k == "len_a":
            return "len({})".format(r.choice(sc["arr"]))
        if k == "at":
            return "at({}, {})".format(r.choice(sc["arr"]), self.int_expr(sc, depth + 1))
        if k == "sum":
            return "checksum({})".format(r.choice(sc["arr"]))
        if k == "field":
            return "{}.n".format(r.choice(sc["rec"]))
        if k == "call":
            name, nargs = r.choice(self.fns)
            args = [str(r.randrange(0, 6))] + [self.int_expr(sc, depth + 1) for _ in range(nargs)]
            return "{}({})".format(name, ", ".join(args))
        if k == "ifexpr":
            return "(if {} then {} else {} end)".format(self.cond(sc, depth + 1), self.int_expr(sc, depth + 1), self.int_expr(sc, depth + 1))
        if k == "match":
            return "(match {} % 4 when 0 then {} when 1 then {} else {} end)".format(
                self.int_expr(sc, depth + 1), self.int_expr(sc, depth + 2), self.int_expr(sc, depth + 2), self.int_expr(sc, depth + 2))
        raise AssertionError(k)

    def cond(self, sc, depth=0):
        op = self.r.choice(["<", "==", "!=", ">="])
        c = "{} {} {}".format(self.int_expr(sc, depth + 1), op, self.int_expr(sc, depth + 1))
        if depth < 2 and self.chance(0.2):
            c = "{} {} {}".format(c, self.r.choice(["and", "or"]), self.cond(sc, depth + 1))
        return c

    def str_expr(self, sc, depth=0):
        r = self.r
        opts = ["lit", "interp", "tostr"]
        if sc["str"]:
            opts += ["var", "var", "cat"]
        if sc["rec"]:
            opts.append("field")
        k = r.choice(opts)
        if k == "lit":
            return '"{}"'.format(r.choice(["a", "bc", "kept", "x-y", "", "zz9"]))
        if k == "interp":
            return '"{}{{{}}}"'.format(r.choice(["s", "v-", "item:"]), self.int_expr(sc, depth + 1))
        if k == "tostr":
            return "to_string({})".format(self.int_expr(sc, depth + 1))
        if k == "var":
            return r.choice(sc["str"])
        if k == "cat":
            # bounded: strings never grow without limit across iterations
            return "clip({} + {})".format(r.choice(sc["str"]), self.str_expr(sc, depth + 1) if depth < 2 else '"+"')
        if k == "field":
            return "{}.s".format(r.choice(sc["rec"]))
        raise AssertionError(k)

    def arr_expr(self, sc, depth=0):
        r = self.r
        k = r.choice(["lit", "lit", "range"] + (["var"] if sc["arr"] else []))
        if k == "lit":
            return "[{}]".format(", ".join(self.int_expr(sc, depth + 2) for _ in range(r.randrange(0, 4))))
        if k == "range":
            return "make_list({}, {})".format(r.randrange(0, 12), self.int_expr(sc, depth + 2))
        return r.choice(sc["arr"])

    def rec_expr(self, sc):
        return "{{n: {}, s: {}, tag: \"r\"}}".format(self.int_expr(sc, 1), self.str_expr(sc, 1))

    # ── Statements ──

    def emit(self, ind, line):
        self.lines.append("  " * ind + line)

    def copy_scope(self, sc):
        return {k: list(v) for k, v in sc.items()}

    def block(self, sc, ind, n, depth):
        for _ in range(n):
            self.stmt(sc, ind, depth)

    def stmt(self, sc, ind, depth):
        r = self.r
        opts = ["let_int", "let_str", "assign", "print"]
        if depth < 3:
            opts += ["if", "for", "while", "let_arr", "let_rec"]
            if depth < 2:
                opts += ["closure", "pcall", "build", "forin"]
        if sc["arr"]:
            opts += ["push", "push"]
        if sc["rec"]:
            opts.append("setfield")
        opts.append("global")
        k = r.choice(opts)
        if k == "let_int":
            v = self.fresh("i")
            self.emit(ind, "let {} = {}".format(v, self.int_expr(sc)))
            sc["int"].append(v)
        elif k == "let_str":
            v = self.fresh("s")
            self.emit(ind, "let {} = {}".format(v, self.str_expr(sc)))
            sc["str"].append(v)
        elif k == "let_arr":
            v = self.fresh("a")
            self.emit(ind, "let {} = {}".format(v, self.arr_expr(sc)))
            sc["arr"].append(v)
        elif k == "let_rec":
            v = self.fresh("r")
            self.emit(ind, "let {} = {}".format(v, self.rec_expr(sc)))
            sc["rec"].append(v)
        elif k == "assign":
            # never a loop counter or variable (k, w, e) or the depth d:
            # the loops must stay bounded
            ints = [v for v in sc["int"] if v[0] in "ixc"]
            if ints and (not sc["str"] or self.chance(0.6)):
                v = r.choice(ints)
                if self.chance(0.3):
                    self.emit(ind, "{} = ({} + {}) % {}".format(v, v, self.int_expr(sc), MOD))
                else:
                    self.emit(ind, "{} = {}".format(v, self.int_expr(sc)))
            elif sc["str"]:
                self.emit(ind, "{} = {}".format(r.choice(sc["str"]), self.str_expr(sc)))
        elif k == "print":
            if self.work <= 20:
                self.print_some(sc, ind)
        elif k == "if":
            self.emit(ind, "if {}".format(self.cond(sc)))
            self.block(self.copy_scope(sc), ind + 1, r.randrange(1, 4), depth + 1)
            if self.chance(0.3):
                self.emit(ind, "elif {}".format(self.cond(sc)))
                self.block(self.copy_scope(sc), ind + 1, r.randrange(1, 3), depth + 1)
            if self.chance(0.5):
                self.emit(ind, "else")
                self.block(self.copy_scope(sc), ind + 1, r.randrange(1, 3), depth + 1)
            self.emit(ind, "end")
        elif k == "for":
            v = self.fresh("k")
            n = self.bound([3, 10, 50, 200, 1000])
            self.emit(ind, "for {} = 0, {}".format(v, n))
            inner = self.copy_scope(sc)
            inner["int"].append(v)
            self.in_loop(n, lambda: self.loop_body(inner, ind + 1, depth))
            self.emit(ind, "end")
        elif k == "while":
            v = self.fresh("w")
            self.emit(ind, "let {} = 0".format(v))
            n = self.bound([2, 20, 300])
            self.emit(ind, "while {} < {}".format(v, n))
            inner = self.copy_scope(sc)
            inner["int"].append(v)
            self.in_loop(n, lambda: self.loop_body(inner, ind + 1, depth))
            self.emit(ind + 1, "{} += 1".format(v))
            self.emit(ind, "end")
        elif k == "forin":
            arr = self.arr_expr(sc)
            v = self.fresh("e")
            self.emit(ind, "for {} in {}".format(v, arr))
            inner = self.copy_scope(sc)
            inner["int"].append(v)
            # only reads and other tables inside: no growing what we iterate
            inner["arr"] = []
            self.in_loop(64, lambda: self.block(inner, ind + 1, r.randrange(1, 3), depth + 1))
            self.emit(ind, "end")
        elif k == "push":
            a = r.choice(sc["arr"])
            self.emit(ind, "if len({}) < 64 then push({}, {}) end".format(a, a, self.int_expr(sc)))
        elif k == "setfield":
            rv = r.choice(sc["rec"])
            if self.chance(0.5):
                self.emit(ind, "{}.n = {}".format(rv, self.int_expr(sc)))
            else:
                self.emit(ind, "{}.s = {}".format(rv, self.str_expr(sc)))
        elif k == "global":
            g = r.choice(["g_log", "g_tab", "g_count"])
            if g == "g_log":
                self.emit(ind, "if len(g_log) < 200 then push(g_log, {}) end".format(self.str_expr(sc)))
            elif g == "g_tab":
                self.emit(ind, 'g_tab["k{{{} % 16}}"] = {}'.format(self.int_expr(sc), self.str_expr(sc)))
            else:
                self.emit(ind, "g_count = (g_count + {}) % {}".format(self.int_expr(sc), MOD))
        elif k == "closure":
            cnt = self.fresh("c")
            last = self.fresh("l")
            f = self.fresh("f")
            self.emit(ind, "let {} = 0".format(cnt))
            self.emit(ind, 'let {} = ""'.format(last))
            self.emit(ind, "let {} = fn(x)".format(f))
            self.emit(ind + 1, "{} = ({} + x) % {}".format(cnt, cnt, MOD))
            self.emit(ind + 1, '{} = "c{{x}}"'.format(last))
            self.emit(ind + 1, "[x, {}]".format(last))
            self.emit(ind, "end")
            v = self.fresh("k")
            self.emit(ind, "for {} = 0, {}".format(v, self.bound([5, 100, 700])))
            self.emit(ind + 1, "{}(({} * 7) % {})".format(f, v, MOD))
            self.emit(ind + 1, "junk({})".format(v))
            self.emit(ind, "end")
            if self.work <= 20:
                self.emit(ind, "print(\"closure\", {}, {})".format(cnt, last))
            sc["int"].append(cnt)
            sc["str"].append(last)
        elif k == "pcall":
            v = self.fresh("p")
            self.emit(ind, "let {} = pcall boom({})".format(v, self.int_expr(sc)))
            if self.work <= 20:
                self.emit(ind, "print(\"pcall\", {}.ok, if {}.ok then {}.value else {}.error end)".format(v, v, v, v))
        elif k == "build":
            v = self.fresh("b")
            n = self.bound([3, 40, 400])
            self.emit(ind, "let {} = build_string do |add|".format(v))
            kk = self.fresh("k")
            self.emit(ind + 1, "for {} = 0, {}".format(kk, n))
            self.emit(ind + 2, 'add("{{{}}},")'.format(kk))
            self.emit(ind + 2, "junk({})".format(kk))
            self.emit(ind + 1, "end")
            self.emit(ind, "end")
            if self.work <= 20:
                self.emit(ind, "print(\"built\", len({}), substr({}, 0, 12))".format(v, v))

    # A loop bound that keeps the iterations of all enclosing loops (and
    # the calls to fns, which may loop too) under about 20,000.
    def bound(self, choices):
        ok = [n for n in choices if self.work * n <= 20000]
        return self.r.choice(ok or [min(choices)])

    def in_loop(self, n, body):
        saved = self.work
        self.work *= max(n, 1)
        body()
        self.work = saved

    def loop_body(self, sc, ind, depth):
        # allocate garbage in most loops, so resets have work to do
        if self.chance(0.7):
            self.emit(ind, "junk({})".format(self.int_expr(sc)))
        self.block(sc, ind, self.r.randrange(1, 4), depth + 1)

    def print_some(self, sc, ind):
        items = []
        for kind in ("int", "str", "arr", "rec"):
            if sc[kind] and self.chance(0.6):
                v = self.r.choice(sc[kind])
                if kind == "arr":
                    items.append("len({}), checksum({})".format(v, v))
                elif kind == "rec":
                    items.append("{}.n, {}.s".format(v, v))
                else:
                    items.append(v)
        if items:
            self.emit(ind, 'print("at", {})'.format(", ".join(items)))

    # ── Functions ──

    def gen_fn(self, idx):
        r = self.r
        name = "f{}".format(idx)
        nargs = r.randrange(1, 3)
        params = ["x{}".format(i) for i in range(nargs)]
        self.emit(0, "fn {}(d, {})".format(name, ", ".join(params)))
        sc = {"int": ["d"] + params, "str": [], "arr": [], "rec": []}
        shape = r.choice(["tail", "nontail", "plain", "plain"])
        # a fn body's loops get a smaller budget: callers may loop around
        # it; and fns call no fns (main does), so calls can't nest into
        # exponential work
        self.work = 100
        saved_fns, self.fns = self.fns, []
        if shape == "tail":
            # self tail call carrying an accumulator through the iterations
            self.emit(1, "if d <= 0 then return {} end".format(self.int_expr(sc, 1)))
            self.block(sc, 1, r.randrange(1, 3), 2)
            args = ["d - 1"] + ["({} + {}) % {}".format(p, self.int_expr(sc, 1), MOD) for p in params]
            self.emit(1, "junk(d)")
            self.emit(1, "{}({})".format(name, ", ".join(args)))
        elif shape == "nontail":
            self.emit(1, "if d <= 0 then return {} end".format(self.int_expr(sc, 1)))
            self.block(sc, 1, r.randrange(0, 2), 2)
            args = ["d - 1"] + params
            self.emit(1, "let sub = {}({})".format(name, ", ".join(args)))
            self.emit(1, "(sub + {}) % {}".format(self.int_expr(sc, 1), MOD))
        else:
            self.block(sc, 1, r.randrange(1, 4), 1)
            self.emit(1, self.int_expr(sc, 1))
        self.emit(0, "end")
        self.emit(0, "")
        self.work = 1
        self.fns = saved_fns + [(name, nargs)]

    def program(self):
        r = self.r
        self.emit(0, "# generated by tests/fuzz/gen_program.py {}".format(self.seed))
        self.emit(0, PRELUDE)
        for i in range(r.randrange(1, 5)):
            self.gen_fn(i)
        # mutual tail recursion over a decreasing counter
        self.emit(0, "fn ping(n, acc, pad)")
        self.emit(1, "if n <= 0 then return acc end")
        self.emit(1, "pong(n - 1, (acc * 3 + n) % {}, \"p{{n}}\")".format(MOD))
        self.emit(0, "end")
        self.emit(0, "fn pong(n, acc, pad)")
        self.emit(1, "if n <= 0 then return acc + len(pad) end")
        self.emit(1, "ping(n - 1, (acc + n * 5) % {}, \"q{{n}}\")".format(MOD))
        self.emit(0, "end")
        self.emit(0, "")
        self.emit(0, "fn main()")
        sc = {"int": [], "str": [], "arr": [], "rec": []}
        self.block(sc, 1, r.randrange(6, 16), 0)
        self.emit(1, "print(\"mutual\", ping({}, 1, \"\"))".format(r.choice([10, 1000, 50000])))
        if self.chance(0.5):
            self.emit(1, "let me = self()")
            self.emit(1, "spawn do")
            self.emit(2, "let acc = []")
            self.emit(2, "for k = 0, 300")
            self.emit(3, "junk(k)")
            self.emit(3, "if k % 10 == 0 then push(acc, \"m{k}\") end")
            self.emit(2, "end")
            self.emit(2, "send(me, {tag: \"done\", n: len(acc), last: acc[-1]})")
            self.emit(1, "end")
            self.emit(1, "receive")
            self.emit(1, "when {tag: \"done\", n: n, last: last}")
            self.emit(2, "print(\"worker\", n, last)")
            self.emit(1, "end")
        self.emit(1, "print(\"globals\", len(g_log), g_count, len(keys(g_tab)))")
        self.emit(1, "let ks = keys(g_tab)")
        self.emit(1, "sort(ks, fn(a, b) if a < b then -1 elif a > b then 1 else 0 end end)")
        self.emit(1, "for k in ks")
        self.emit(2, "print(k, g_tab[k])")
        self.emit(1, "end")
        self.emit(1, "for line in g_log")
        self.emit(2, "print(line)")
        self.emit(1, "end")
        self.emit(0, "end")
        return "\n".join(self.lines) + "\n"


PRELUDE = """let g_log = []
let g_tab = {}
let g_count = 0

# ~200 bytes of garbage
fn junk(i)
  let s = "junk-{i}-" + to_string(i * 7)
  [s, s + "!", {k: s}]
end

fn clip(s)
  if len(s) > 40 then substr(s, 0, 40) else s end
end

fn at(a, i)
  if len(a) == 0 then return 0 end
  let v = a[i % len(a)]
  if type(v) == "int" then v else 0 end
end

fn checksum(a)
  let t = 0
  for v in a
    if type(v) == "int" then t = (t * 31 + v) % 10007 end
  end
  t
end

fn make_list(n, seed)
  let out = []
  for i = 0, n
    push(out, (seed + i * 13) % 10007)
  end
  out
end

fn boom(n)
  if n % 3 == 0 then error("boom {n}") end
  n * 2
end
"""


if __name__ == "__main__":
    seed = int(sys.argv[1]) if len(sys.argv) > 1 else 1
    sys.stdout.write(Gen(seed).program())
