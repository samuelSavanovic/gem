#!/usr/bin/env python3
"""Gem bootstrap compiler — compiles Gem source to C."""

import sys
import os
import subprocess
import tempfile

from lark import Lark, Transformer, v_args
from lark.exceptions import UnexpectedInput

# ─── Grammar ────────────────────────────────────────────────────────────────

GRAMMAR = r"""
    start: (_NL* stmt)*  _NL*

    ?stmt: fn_def
         | expr

    fn_def: "fn" NAME "(" params ")" body "end"
    params: (NAME ("," NAME)*)?
    body: (_NL* stmt)*  _NL*

    ?expr: assign

    ?assign: or_expr

    ?or_expr: and_expr ("or" and_expr)*
    ?and_expr: not_expr ("and" not_expr)*
    ?not_expr: "not" not_expr -> not_op
             | compare
    ?compare: add (COMP_OP add)*
    COMP_OP: "==" | "!=" | "<=" | ">=" | "<" | ">"

    ?add: mul (ADD_OP mul)*
    ADD_OP: "+" | "-"

    ?mul: unary (MUL_OP unary)*
    MUL_OP: "*" | "/" | "%"

    ?unary: "-" unary -> neg
          | call

    ?call: atom ("(" args ")")*
    args: (expr ("," expr)*)?

    ?atom: NUMBER        -> number
         | STRING        -> string
         | "true"        -> true_lit
         | "false"       -> false_lit
         | "nil"         -> nil_lit
         | NAME          -> var
         | "(" expr ")"

    NAME: /(?!(?:fn|end|let|if|else|while|return|do|match|when|and|or|not|true|false|nil|load|extern)\b)[a-zA-Z_][a-zA-Z0-9_]*/
    NUMBER: /[0-9]+(\.[0-9]+)?/
    STRING: /\"([^\"\\]|\\.)*\"/

    COMMENT: /#[^\n]*/

    _NL: /[\n;]+/

    %import common.WS_INLINE
    %ignore WS_INLINE
    %ignore COMMENT
"""

parser = Lark(GRAMMAR, parser="earley", propagate_positions=True)

# ─── AST Transformer ────────────────────────────────────────────────────────

class ASTBuilder(Transformer):
    def start(self, items):
        return {"tag": "program", "stmts": items}

    def fn_def(self, items):
        name = str(items[0])
        params = items[1]
        body = items[2]
        return {"tag": "fn_def", "name": name, "params": params, "body": body}

    def params(self, items):
        return [str(t) for t in items]

    def body(self, items):
        return items

    def number(self, items):
        s = str(items[0])
        if "." in s:
            return {"tag": "float", "value": float(s)}
        return {"tag": "int", "value": int(s)}

    def string(self, items):
        raw = str(items[0])
        # Strip quotes and handle escapes
        return {"tag": "string", "value": raw[1:-1].replace('\\"', '"').replace("\\n", "\n").replace("\\t", "\t").replace("\\\\", "\\")}

    def true_lit(self, items):
        return {"tag": "bool", "value": True}

    def false_lit(self, items):
        return {"tag": "bool", "value": False}

    def nil_lit(self, items):
        return {"tag": "nil"}

    def var(self, items):
        return {"tag": "var", "name": str(items[0])}

    def call(self, items):
        func = items[0]
        for arg_list in items[1:]:
            func = {"tag": "call", "func": func, "args": arg_list}
        return func

    def args(self, items):
        return list(items)

    def not_op(self, items):
        return {"tag": "unop", "op": "not", "expr": items[0]}

    def neg(self, items):
        return {"tag": "unop", "op": "-", "expr": items[0]}

    def or_expr(self, items):
        return self._binop_chain(items, "or")

    def and_expr(self, items):
        return self._binop_chain(items, "and")

    def compare(self, items):
        # items alternate: expr, COMP_OP, expr, COMP_OP, expr...
        if len(items) == 1:
            return items[0]
        left = items[0]
        i = 1
        while i < len(items):
            op = str(items[i])
            right = items[i + 1]
            left = {"tag": "binop", "op": op, "left": left, "right": right}
            i += 2
        return left

    def add(self, items):
        if len(items) == 1:
            return items[0]
        left = items[0]
        i = 1
        while i < len(items):
            op = str(items[i])
            right = items[i + 1]
            left = {"tag": "binop", "op": op, "left": left, "right": right}
            i += 2
        return left

    def mul(self, items):
        if len(items) == 1:
            return items[0]
        left = items[0]
        i = 1
        while i < len(items):
            op = str(items[i])
            right = items[i + 1]
            left = {"tag": "binop", "op": op, "left": left, "right": right}
            i += 2
        return left

    def _binop_chain(self, items, op):
        if len(items) == 1:
            return items[0]
        left = items[0]
        for right in items[1:]:
            left = {"tag": "binop", "op": op, "left": left, "right": right}
        return left


# ─── C Code Generation ─────────────────────────────────────────────────────

C_RUNTIME = r"""
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ─── Tagged value ─── */

typedef enum {
    VAL_NIL, VAL_BOOL, VAL_INT, VAL_FLOAT, VAL_STRING, VAL_FN,
} GemType;

typedef struct GemVal GemVal;
typedef GemVal (*GemFnPtr)(GemVal *args, int argc);

struct GemVal {
    GemType type;
    union {
        int64_t ival;
        double fval;
        char *sval;
        int bval;
        GemFnPtr fn;
    };
};

static GemVal GEM_NIL = {VAL_NIL, {0}};

static GemVal gem_int(int64_t v) { return (GemVal){VAL_INT, {.ival = v}}; }
static GemVal gem_float(double v) { GemVal r; r.type = VAL_FLOAT; r.fval = v; return r; }
static GemVal gem_bool(int v) { GemVal r; r.type = VAL_BOOL; r.bval = v; return r; }
static GemVal gem_fn(GemFnPtr f) { GemVal r; r.type = VAL_FN; r.fn = f; return r; }

static GemVal gem_string(const char *s) {
    GemVal r;
    r.type = VAL_STRING;
    r.sval = malloc(strlen(s) + 1);
    strcpy(r.sval, s);
    return r;
}

/* ─── Truthiness ─── */

static int gem_truthy(GemVal v) {
    if (v.type == VAL_NIL) return 0;
    if (v.type == VAL_BOOL) return v.bval;
    return 1;
}

/* ─── Runtime error ─── */

static void gem_error(const char *msg) {
    fprintf(stderr, "error: %s\n", msg);
    exit(1);
}

/* ─── Arithmetic / operators ─── */

static GemVal gem_add(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int(a.ival + b.ival);
    if (a.type == VAL_FLOAT && b.type == VAL_FLOAT) return gem_float(a.fval + b.fval);
    if (a.type == VAL_INT && b.type == VAL_FLOAT) return gem_float((double)a.ival + b.fval);
    if (a.type == VAL_FLOAT && b.type == VAL_INT) return gem_float(a.fval + (double)b.ival);
    if (a.type == VAL_STRING && b.type == VAL_STRING) {
        size_t la = strlen(a.sval), lb = strlen(b.sval);
        char *s = malloc(la + lb + 1);
        memcpy(s, a.sval, la);
        memcpy(s + la, b.sval, lb + 1);
        GemVal r; r.type = VAL_STRING; r.sval = s; return r;
    }
    gem_error("type error in +"); return GEM_NIL;
}

static GemVal gem_sub(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int(a.ival - b.ival);
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_float(fa - fb);
    }
    gem_error("type error in -"); return GEM_NIL;
}

static GemVal gem_mul(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_int(a.ival * b.ival);
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_float(fa * fb);
    }
    gem_error("type error in *"); return GEM_NIL;
}

static GemVal gem_div(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) {
        if (b.ival == 0) gem_error("division by zero");
        return gem_int(a.ival / b.ival);
    }
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        if (fb == 0.0) gem_error("division by zero");
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        return gem_float(fa / fb);
    }
    gem_error("type error in /"); return GEM_NIL;
}

static GemVal gem_mod(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) {
        if (b.ival == 0) gem_error("division by zero");
        return gem_int(a.ival % b.ival);
    }
    gem_error("type error in %"); return GEM_NIL;
}

static GemVal gem_eq(GemVal a, GemVal b) {
    if (a.type != b.type) return gem_bool(0);
    switch (a.type) {
        case VAL_NIL: return gem_bool(1);
        case VAL_BOOL: return gem_bool(a.bval == b.bval);
        case VAL_INT: return gem_bool(a.ival == b.ival);
        case VAL_FLOAT: return gem_bool(a.fval == b.fval);
        case VAL_STRING: return gem_bool(strcmp(a.sval, b.sval) == 0);
        default: return gem_bool(0);
    }
}

static GemVal gem_neq(GemVal a, GemVal b) {
    return gem_bool(!gem_truthy(gem_eq(a, b)));
}

static GemVal gem_lt(GemVal a, GemVal b) {
    if (a.type == VAL_INT && b.type == VAL_INT) return gem_bool(a.ival < b.ival);
    if (a.type == VAL_FLOAT || b.type == VAL_FLOAT) {
        double fa = a.type == VAL_INT ? (double)a.ival : a.fval;
        double fb = b.type == VAL_INT ? (double)b.ival : b.fval;
        return gem_bool(fa < fb);
    }
    if (a.type == VAL_STRING && b.type == VAL_STRING) return gem_bool(strcmp(a.sval, b.sval) < 0);
    gem_error("type error in <"); return GEM_NIL;
}

static GemVal gem_gt(GemVal a, GemVal b) { return gem_lt(b, a); }
static GemVal gem_le(GemVal a, GemVal b) { return gem_bool(!gem_truthy(gem_gt(a, b))); }
static GemVal gem_ge(GemVal a, GemVal b) { return gem_bool(!gem_truthy(gem_lt(a, b))); }

static GemVal gem_neg(GemVal a) {
    if (a.type == VAL_INT) return gem_int(-a.ival);
    if (a.type == VAL_FLOAT) return gem_float(-a.fval);
    gem_error("type error in unary -"); return GEM_NIL;
}

static GemVal gem_not(GemVal a) {
    return gem_bool(!gem_truthy(a));
}

/* ─── Built-in: print ─── */

static GemVal gem_print(GemVal *args, int argc) {
    for (int i = 0; i < argc; i++) {
        if (i > 0) printf(" ");
        GemVal v = args[i];
        switch (v.type) {
            case VAL_NIL: printf("nil"); break;
            case VAL_BOOL: printf("%s", v.bval ? "true" : "false"); break;
            case VAL_INT: printf("%lld", (long long)v.ival); break;
            case VAL_FLOAT: printf("%g", v.fval); break;
            case VAL_STRING: printf("%s", v.sval); break;
            case VAL_FN: printf("<fn>"); break;
        }
    }
    printf("\n");
    return GEM_NIL;
}

/* ─── Built-in: error ─── */

static GemVal gem_error_fn(GemVal *args, int argc) {
    if (argc > 0 && args[0].type == VAL_STRING) {
        fprintf(stderr, "error: %s\n", args[0].sval);
    } else {
        fprintf(stderr, "error\n");
    }
    exit(1);
    return GEM_NIL;
}

/* ─── Forward declarations ─── */
"""

class CodeGen:
    def __init__(self):
        self.functions = []     # list of C function definitions
        self.forward_decls = [] # forward declarations
        self.tmp_counter = 0
        self.defined_fns = set() # names of user-defined functions
        self.builtins = {"print", "error"}

    def tmp(self):
        self.tmp_counter += 1
        return f"_t{self.tmp_counter}"

    def compile(self, ast):
        # Collect all function defs and top-level statements
        fn_defs = [s for s in ast["stmts"] if s["tag"] == "fn_def"]
        top_stmts = [s for s in ast["stmts"] if s["tag"] != "fn_def"]

        # Emit forward declarations
        for fn in fn_defs:
            self.defined_fns.add(fn["name"])
            self.forward_decls.append(
                f"static GemVal gem_fn_{fn['name']}(GemVal *args, int argc);"
            )

        # Emit function definitions
        for fn in fn_defs:
            self.functions.append(self._compile_fn(fn))

        # Build main
        main_body = self._compile_stmts(top_stmts, indent=1)

        # Check if there's a user-defined main
        has_main = any(fn["name"] == "main" for fn in fn_defs)

        main_c = "int main(void) {\n"
        if main_body.strip():
            main_c += main_body
        if has_main:
            main_c += "    GemVal _margs[1] = {GEM_NIL};\n"
            main_c += "    gem_fn_main(_margs, 0);\n"
        main_c += "    return 0;\n}\n"

        # Assemble
        out = C_RUNTIME
        out += "\n".join(self.forward_decls) + "\n\n"
        out += "\n\n".join(self.functions) + "\n\n"
        out += main_c
        return out

    def _compile_fn(self, node):
        params = node["params"]
        body = node["body"]
        name = node["name"]

        lines = []
        lines.append(f"static GemVal gem_fn_{name}(GemVal *args, int argc) {{")

        # Bind parameters
        for i, p in enumerate(params):
            lines.append(f"    GemVal {p} = (argc > {i}) ? args[{i}] : GEM_NIL;")

        # Compile body — last expression is the return value
        if body:
            for stmt in body[:-1]:
                lines.append(self._compile_stmt(stmt, indent=1))
            # Last statement: return its value
            last = body[-1]
            lines.append(self._compile_stmt_return(last, indent=1))
        else:
            lines.append("    return GEM_NIL;")

        lines.append("}")
        return "\n".join(lines)

    def _compile_stmts(self, stmts, indent=0):
        pad = "    " * indent
        out = ""
        for s in stmts:
            out += self._compile_stmt(s, indent) + "\n"
        return out

    def _compile_stmt(self, node, indent=0):
        pad = "    " * indent
        tag = node["tag"]
        if tag == "call":
            tmp = self.tmp()
            expr_code, setup = self._compile_expr(node)
            return setup + f"{pad}(void)({expr_code});"
        elif tag == "fn_def":
            # Nested function — already handled at top level
            return ""
        else:
            tmp = self.tmp()
            expr_code, setup = self._compile_expr(node)
            return setup + f"{pad}(void)({expr_code});"

    def _compile_stmt_return(self, node, indent=0):
        pad = "    " * indent
        expr_code, setup = self._compile_expr(node)
        return setup + f"{pad}return {expr_code};"

    def _compile_expr(self, node):
        """Returns (expr_string, setup_code)."""
        tag = node["tag"]

        if tag == "int":
            return f"gem_int({node['value']})", ""
        elif tag == "float":
            return f"gem_float({node['value']})", ""
        elif tag == "string":
            escaped = (node["value"]
                       .replace("\\", "\\\\")
                       .replace('"', '\\"')
                       .replace("\n", "\\n")
                       .replace("\t", "\\t"))
            return f'gem_string("{escaped}")', ""
        elif tag == "bool":
            return f"gem_bool({1 if node['value'] else 0})", ""
        elif tag == "nil":
            return "GEM_NIL", ""
        elif tag == "var":
            name = node["name"]
            # Built-in functions
            if name == "print":
                return "gem_fn(gem_print)", ""
            if name == "error":
                return "gem_fn(gem_error_fn)", ""
            return name, ""
        elif tag == "call":
            return self._compile_call(node)
        elif tag == "binop":
            return self._compile_binop(node)
        elif tag == "unop":
            expr_code, setup = self._compile_expr(node["expr"])
            if node["op"] == "-":
                return f"gem_neg({expr_code})", setup
            elif node["op"] == "not":
                return f"gem_not({expr_code})", setup
        else:
            return "GEM_NIL", f"/* unknown node: {tag} */\n"

    def _compile_call(self, node):
        func = node["func"]
        args = node["args"]

        # Direct call optimization for known functions
        if func["tag"] == "var":
            name = func["name"]
            if name == "print" or name == "error":
                fn_name = "gem_print" if name == "print" else "gem_error_fn"
                arg_setups = []
                arg_exprs = []
                for a in args:
                    ec, s = self._compile_expr(a)
                    arg_setups.append(s)
                    arg_exprs.append(ec)
                setup = "".join(arg_setups)
                argc = len(args)
                if argc == 0:
                    return f"{fn_name}(NULL, 0)", setup
                tmp = self.tmp()
                arr = ", ".join(arg_exprs)
                setup += f"    GemVal {tmp}[] = {{{arr}}};\n"
                return f"{fn_name}({tmp}, {argc})", setup
            else:
                # User-defined function — call gem_fn_<name> directly
                if name not in self.defined_fns:
                    print(f"error: undefined function '{name}'", file=sys.stderr)
                    sys.exit(1)
                arg_setups = []
                arg_exprs = []
                for a in args:
                    ec, s = self._compile_expr(a)
                    arg_setups.append(s)
                    arg_exprs.append(ec)
                setup = "".join(arg_setups)
                argc = len(args)
                if argc == 0:
                    return f"gem_fn_{name}(NULL, 0)", setup
                tmp = self.tmp()
                arr = ", ".join(arg_exprs)
                setup += f"    GemVal {tmp}[] = {{{arr}}};\n"
                return f"gem_fn_{name}({tmp}, {argc})", setup

        # General case: call through function value
        func_code, func_setup = self._compile_expr(func)
        arg_setups = []
        arg_exprs = []
        for a in args:
            ec, s = self._compile_expr(a)
            arg_setups.append(s)
            arg_exprs.append(ec)
        setup = func_setup + "".join(arg_setups)
        argc = len(args)
        tmp_fn = self.tmp()
        setup += f"    GemVal {tmp_fn} = {func_code};\n"
        if argc == 0:
            return f"{tmp_fn}.fn(NULL, 0)", setup
        tmp_args = self.tmp()
        arr = ", ".join(arg_exprs)
        setup += f"    GemVal {tmp_args}[] = {{{arr}}};\n"
        return f"{tmp_fn}.fn({tmp_args}, {argc})", setup

    def _compile_binop(self, node):
        op = node["op"]
        lc, ls = self._compile_expr(node["left"])
        rc, rs = self._compile_expr(node["right"])
        setup = ls + rs

        op_map = {
            "+": "gem_add", "-": "gem_sub", "*": "gem_mul",
            "/": "gem_div", "%": "gem_mod",
            "==": "gem_eq", "!=": "gem_neq",
            "<": "gem_lt", ">": "gem_gt",
            "<=": "gem_le", ">=": "gem_ge",
        }

        if op in op_map:
            return f"{op_map[op]}({lc}, {rc})", setup
        elif op == "and":
            tmp = self.tmp()
            setup += f"    GemVal {tmp};\n"
            # Short-circuit: if left is falsy, result is left; else result is right
            return f"(gem_truthy({lc}) ? ({rc}) : ({lc}))", setup
        elif op == "or":
            return f"(gem_truthy({lc}) ? ({lc}) : ({rc}))", setup
        else:
            return "GEM_NIL", setup + f"/* unknown op: {op} */\n"


# ─── Driver ─────────────────────────────────────────────────────────────────

def compile_gem(source, source_name="<stdin>"):
    try:
        tree = parser.parse(source)
    except UnexpectedInput as e:
        line = getattr(e, "line", "?")
        col = getattr(e, "column", "?")
        print(f"{source_name}:{line}:{col}: parse error: {e}", file=sys.stderr)
        sys.exit(1)
    ast = ASTBuilder().transform(tree)
    codegen = CodeGen()
    return codegen.compile(ast)

def main():
    if len(sys.argv) < 2:
        print("usage: gem.py <file.gem> [--emit-c] [--run]", file=sys.stderr)
        sys.exit(1)

    src_path = sys.argv[1]
    flags = set(sys.argv[2:])

    with open(src_path) as f:
        source = f.read()

    c_code = compile_gem(source, src_path)

    if "--emit-c" in flags:
        print(c_code)
        return

    # Write C to temp file, compile, and run
    base = os.path.splitext(os.path.basename(src_path))[0]
    out_c = f"/tmp/gem_{base}.c"
    out_bin = f"/tmp/gem_{base}"

    with open(out_c, "w") as f:
        f.write(c_code)

    # Compile
    cc = os.environ.get("CC", "cc")
    result = subprocess.run(
        [cc, "-o", out_bin, out_c, "-std=c11", "-Wall"],
        capture_output=True, text=True
    )
    if result.returncode != 0:
        print(f"C compilation failed:\n{result.stderr}", file=sys.stderr)
        sys.exit(1)

    if "--run" in flags or "--emit-c" not in flags:
        result = subprocess.run([out_bin])
        sys.exit(result.returncode)

if __name__ == "__main__":
    main()
