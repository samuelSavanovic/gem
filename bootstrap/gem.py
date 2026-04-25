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
         | let_decl
         | if_stmt
         | while_stmt
         | match_stmt
         | return_stmt
         | break_stmt
         | continue_stmt
         | load_stmt
         | extern_fn
         | extern_include
         | expr

    break_stmt: "break"
    continue_stmt: "continue"

    load_stmt: "load" STRING
    extern_fn: "extern" "fn" NAME "(" extern_params ")" ("->" extern_type)?
    extern_params: (extern_param ("," extern_param)*)?
    extern_param: NAME ":" extern_type
    extern_type: NAME
    extern_include: "extern" "include" STRING

    return_stmt: "return" expr?

    let_decl: "let" NAME "=" expr

    fn_def: "fn" NAME "(" params ")" fn_body "end"
    params: (NAME ("," NAME)*)?
    body: (_NL+ stmt)*  _NL*
    fn_body: (_NL* stmt)*  _NL*

    if_stmt: "if" expr body ("else" body)? "end"
    while_stmt: "while" expr body "end"
    match_stmt: "match" expr _NL* (when_clause)+ ("else" body)? "end"
    when_clause: "when" expr body

    ?expr: assign

    ?assign: NAME "=" expr            -> assign_stmt
           | NAME ASSIGN_OP expr      -> compound_assign
           | call "." NAME "=" expr   -> dot_assign
           | call "[" expr "]" "=" expr -> index_assign
           | or_expr
    ASSIGN_OP: "+=" | "-=" | "*=" | "/="

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

    ?call: atom postfix*
    ?postfix: "(" args ")" trailing_block?  -> call_postfix
            | "." NAME       -> dot_postfix
            | "[" expr "]"   -> index_postfix
    args: (expr ("," expr)*)?

    ?trailing_block: do_block | brace_block
    do_block: "do" block_params? fn_body "end"
    brace_block: "{" block_params? expr "}"
    block_params: "|" (NAME ("," NAME)*)? "|"

    ?atom: NUMBER        -> number
         | STRING        -> string
         | "true"        -> true_lit
         | "false"       -> false_lit
         | "nil"         -> nil_lit
         | "fn" "(" params ")" fn_body "end" -> anon_fn
         | table_literal
         | array_literal
         | NAME          -> var
         | "(" expr ")"

    table_literal: "{" _NL* table_entries? _NL* "}"
    table_entries: table_entry ((_NL* "," | _NL+) _NL* table_entry)* ","?
    table_entry: NAME ":" expr

    array_literal: "[" _NL* array_entries? _NL* "]"
    array_entries: expr ((_NL* "," | _NL+) _NL* expr)* ","?

    NAME: /(?!(?:fn|end|let|if|else|while|return|break|continue|do|match|when|and|or|not|true|false|nil|load|extern)\b)[a-zA-Z_][a-zA-Z0-9_]*/
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

    def let_decl(self, items):
        name = str(items[0])
        value = items[1]
        return {"tag": "let", "name": name, "value": value}

    def assign_stmt(self, items):
        name = str(items[0])
        value = items[1]
        return {"tag": "assign", "name": name, "value": value}

    def compound_assign(self, items):
        name = str(items[0])
        op = str(items[1])  # "+=", "-=", etc.
        value = items[2]
        base_op = op[0]
        return {"tag": "assign", "name": name, "value": {
            "tag": "binop", "op": base_op,
            "left": {"tag": "var", "name": name},
            "right": value,
        }}

    def dot_assign(self, items):
        obj = items[0]
        field = str(items[1])
        value = items[2]
        return {"tag": "dot_assign", "object": obj, "field": field, "value": value}

    def index_assign(self, items):
        obj = items[0]
        key = items[1]
        value = items[2]
        return {"tag": "index_assign", "object": obj, "key": key, "value": value}

    def load_stmt(self, items):
        raw = str(items[0])
        path = raw[1:-1]  # strip quotes
        return {"tag": "load", "path": path}

    def extern_fn(self, items):
        name = str(items[0])
        params = items[1]
        ret_type = str(items[2]) if len(items) > 2 else "Nil"
        return {"tag": "extern_fn", "name": name, "params": params, "ret_type": ret_type}

    def extern_params(self, items):
        return list(items)

    def extern_param(self, items):
        return {"name": str(items[0]), "type": str(items[1])}

    def extern_type(self, items):
        return str(items[0])

    def extern_include(self, items):
        raw = str(items[0])
        path = raw[1:-1]
        return {"tag": "extern_include", "path": path}

    def return_stmt(self, items):
        value = items[0] if items else None
        return {"tag": "return", "value": value}

    def break_stmt(self, items):
        return {"tag": "break"}

    def continue_stmt(self, items):
        return {"tag": "continue"}

    def if_stmt(self, items):
        cond = items[0]
        then_body = items[1]
        else_body = items[2] if len(items) > 2 else None
        return {"tag": "if", "cond": cond, "then": then_body, "else": else_body}

    def while_stmt(self, items):
        cond = items[0]
        body = items[1]
        return {"tag": "while", "cond": cond, "body": body}

    def match_stmt(self, items):
        target = items[0]
        whens = []
        else_body = None
        for item in items[1:]:
            if isinstance(item, dict) and item.get("tag") == "when_clause":
                whens.append(item)
            elif isinstance(item, list):
                else_body = item
        return {"tag": "match", "target": target, "whens": whens, "else": else_body}

    def when_clause(self, items):
        value = items[0]
        body = items[1]
        return {"tag": "when_clause", "value": value, "body": body}

    def fn_def(self, items):
        name = str(items[0])
        params = items[1]
        body = items[2]
        return {"tag": "fn_def", "name": name, "params": params, "body": body}

    def params(self, items):
        return [str(t) for t in items]

    def body(self, items):
        return items

    def fn_body(self, items):
        return items

    def number(self, items):
        s = str(items[0])
        if "." in s:
            return {"tag": "float", "value": float(s)}
        return {"tag": "int", "value": int(s)}

    def string(self, items):
        raw = str(items[0])
        # Strip quotes and handle escapes with single-pass processing
        s = raw[1:-1]
        result = []
        i = 0
        while i < len(s):
            if s[i] == '\\' and i + 1 < len(s):
                c = s[i + 1]
                if c == 'n':
                    result.append('\n')
                elif c == 't':
                    result.append('\t')
                elif c == '\\':
                    result.append('\\')
                elif c == '"':
                    result.append('"')
                else:
                    result.append('\\')
                    result.append(c)
                i += 2
            else:
                result.append(s[i])
                i += 1
        return {"tag": "string", "value": ''.join(result)}

    def true_lit(self, items):
        return {"tag": "bool", "value": True}

    def false_lit(self, items):
        return {"tag": "bool", "value": False}

    def nil_lit(self, items):
        return {"tag": "nil"}

    def anon_fn(self, items):
        params = items[0]
        body = items[1]
        return {"tag": "anon_fn", "params": params, "body": body}

    def var(self, items):
        return {"tag": "var", "name": str(items[0])}

    def call(self, items):
        # items[0] is the atom, rest are postfix operations
        result = items[0]
        for postfix in items[1:]:
            if postfix["_postfix"] == "call":
                result = {"tag": "call", "func": result, "args": postfix["args"], "line": postfix.get("_line")}
            elif postfix["_postfix"] == "dot":
                result = {"tag": "dot", "object": result, "field": postfix["field"]}
            elif postfix["_postfix"] == "index":
                result = {"tag": "index", "object": result, "key": postfix["key"]}
        return result

    @v_args(meta=True)
    def call_postfix(self, meta, items):
        args = items[0]
        block = items[1] if len(items) > 1 else None
        if block is not None:
            args = args + [block]
        return {"_postfix": "call", "args": args, "_line": meta.line}

    def dot_postfix(self, items):
        return {"_postfix": "dot", "field": str(items[0])}

    def index_postfix(self, items):
        return {"_postfix": "index", "key": items[0]}

    def do_block(self, items):
        params = items[0] if len(items) > 1 else []
        body = items[-1]
        return {"tag": "anon_fn", "params": params, "body": body}

    def brace_block(self, items):
        params = items[0] if len(items) > 1 else []
        expr = items[-1]
        return {"tag": "anon_fn", "params": params, "body": [expr]}

    def block_params(self, items):
        return [str(t) for t in items]

    def args(self, items):
        return list(items)

    def table_literal(self, items):
        return {"tag": "table", "entries": items[0] if items else []}

    def table_entries(self, items):
        return list(items)

    def table_entry(self, items):
        return {"key": str(items[0]), "value": items[1]}

    def array_literal(self, items):
        return {"tag": "array", "elements": items[0] if items else []}

    def array_entries(self, items):
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

C_RUNTIME = '#include "gem.h"\n'

class CodeGen:
    def __init__(self, source_name="<stdin>"):
        self.functions = []     # list of C function definitions
        self.forward_decls = [] # forward declarations
        self.tmp_counter = 0
        self.anon_counter = 0
        self.defined_fns = set() # names of user-defined functions
        self.builtins = {"print", "error", "len", "type", "to_string"}
        self.source_name = source_name
        # Per-scope state — set during function compilation
        self.boxed_vars = set()  # variables that are heap-allocated (captured by closures)

    def tmp(self):
        self.tmp_counter += 1
        return f"_t{self.tmp_counter}"

    def _anon_name(self):
        self.anon_counter += 1
        return f"_anon_{self.anon_counter}"

    def _mangle(self, name):
        """Prefix Gem variable names to avoid C keyword collisions."""
        return f"gem_v_{name}"

    # ─── Free variable analysis ───

    def _collect_free_vars(self, stmts, defined):
        """Return set of variable names used in stmts but not defined in them or in 'defined'."""
        free = set()
        defined = set(defined)
        for stmt in stmts:
            self._collect_free_node(stmt, defined, free)
            if isinstance(stmt, dict) and stmt.get("tag") == "let":
                defined.add(stmt["name"])
        return free

    def _collect_free_node(self, node, defined, free):
        if not isinstance(node, dict):
            return
        tag = node.get("tag")
        if tag == "var":
            name = node["name"]
            if name not in defined and name not in self.builtins and name not in self.defined_fns:
                free.add(name)
        elif tag == "let":
            self._collect_free_node(node["value"], defined, free)
        elif tag == "assign":
            name = node["name"]
            if name not in defined and name not in self.builtins and name not in self.defined_fns:
                free.add(name)
            self._collect_free_node(node["value"], defined, free)
        elif tag == "anon_fn":
            # Recurse into the anon fn body with its own scope
            inner_defined = defined | set(node["params"])
            inner_free = self._collect_free_vars(node["body"], inner_defined)
            free.update(inner_free)
        elif tag == "fn_def":
            # Named functions are global — don't recurse for free var purposes
            pass
        elif tag in ("if",):
            self._collect_free_node(node["cond"], defined, free)
            for s in node["then"]:
                self._collect_free_node(s, defined, free)
                if isinstance(s, dict) and s.get("tag") == "let":
                    defined = defined | {s["name"]}
            if node["else"]:
                for s in node["else"]:
                    self._collect_free_node(s, defined, free)
                    if isinstance(s, dict) and s.get("tag") == "let":
                        defined = defined | {s["name"]}
        elif tag == "while":
            self._collect_free_node(node["cond"], defined, free)
            for s in node["body"]:
                self._collect_free_node(s, defined, free)
                if isinstance(s, dict) and s.get("tag") == "let":
                    defined = defined | {s["name"]}
        elif tag == "match":
            self._collect_free_node(node["target"], defined, free)
            for when in node["whens"]:
                self._collect_free_node(when["value"], defined, free)
                for s in when["body"]:
                    self._collect_free_node(s, defined, free)
                    if isinstance(s, dict) and s.get("tag") == "let":
                        defined = defined | {s["name"]}
            if node["else"]:
                for s in node["else"]:
                    self._collect_free_node(s, defined, free)
                    if isinstance(s, dict) and s.get("tag") == "let":
                        defined = defined | {s["name"]}
        elif tag == "when_clause":
            self._collect_free_node(node["value"], defined, free)
            for s in node["body"]:
                self._collect_free_node(s, defined, free)
                if isinstance(s, dict) and s.get("tag") == "let":
                    defined = defined | {s["name"]}
        elif tag == "return":
            if node["value"] is not None:
                self._collect_free_node(node["value"], defined, free)
        else:
            # Generic recursion for binop, unop, call, etc.
            for k, v in node.items():
                if k == "tag":
                    continue
                if isinstance(v, dict):
                    self._collect_free_node(v, defined, free)
                elif isinstance(v, list):
                    for item in v:
                        if isinstance(item, dict):
                            self._collect_free_node(item, defined, free)

    def _find_captured_in_scope(self, body, scope_vars):
        """Find which variables from scope_vars are captured by anon_fns in body."""
        captured = set()
        self._walk_captures(body, set(scope_vars), captured)
        return captured

    def _walk_captures(self, stmts, scope_vars, captured):
        for stmt in stmts:
            self._walk_captures_node(stmt, scope_vars, captured)
            if isinstance(stmt, dict) and stmt.get("tag") == "let":
                scope_vars = scope_vars | {stmt["name"]}

    def _walk_captures_node(self, node, scope_vars, captured):
        if not isinstance(node, dict):
            return
        tag = node.get("tag")
        if tag == "anon_fn":
            # Find free vars of this anon fn
            inner_defined = set(node["params"])
            free = self._collect_free_vars(node["body"], inner_defined)
            captured.update(free & scope_vars)
            # Also recurse into the anon fn body for nested closures
            # that might capture from our scope through this one
            self._walk_captures(node["body"], scope_vars | set(node["params"]), captured)
        elif tag == "fn_def":
            pass  # named fns are global
        elif tag == "if":
            self._walk_captures_node(node["cond"], scope_vars, captured)
            self._walk_captures(node["then"], scope_vars, captured)
            if node["else"]:
                self._walk_captures(node["else"], scope_vars, captured)
        elif tag == "while":
            self._walk_captures_node(node["cond"], scope_vars, captured)
            self._walk_captures(node["body"], scope_vars, captured)
        elif tag == "match":
            self._walk_captures_node(node["target"], scope_vars, captured)
            for when in node["whens"]:
                self._walk_captures_node(when["value"], scope_vars, captured)
                self._walk_captures(when["body"], scope_vars, captured)
            if node["else"]:
                self._walk_captures(node["else"], scope_vars, captured)
        elif tag == "return":
            if node["value"] is not None:
                self._walk_captures_node(node["value"], scope_vars, captured)
        elif tag == "let":
            self._walk_captures_node(node["value"], scope_vars, captured)
        elif tag == "assign":
            self._walk_captures_node(node["value"], scope_vars, captured)
        else:
            for k, v in node.items():
                if k == "tag":
                    continue
                if isinstance(v, dict):
                    self._walk_captures_node(v, scope_vars, captured)
                elif isinstance(v, list):
                    for item in v:
                        if isinstance(item, dict):
                            self._walk_captures_node(item, scope_vars, captured)

    # ─── Compilation ───

    def compile(self, ast):
        # Separate different statement types
        fn_defs = [s for s in ast["stmts"] if s["tag"] == "fn_def"]
        extern_fns = [s for s in ast["stmts"] if s["tag"] == "extern_fn"]
        extern_includes = [s for s in ast["stmts"] if s["tag"] == "extern_include"]
        top_stmts = [s for s in ast["stmts"] if s["tag"] not in ("fn_def", "extern_fn", "extern_include")]

        # Register extern fns as defined (callable)
        for ef in extern_fns:
            self.defined_fns.add(ef["name"])

        # Emit forward declarations for user-defined functions
        for fn in fn_defs:
            self.defined_fns.add(fn["name"])
            self.forward_decls.append(
                f"static GemVal gem_fn_{fn['name']}(void *_env, GemVal *args, int argc);"
            )

        # Emit extern fn wrappers
        for ef in extern_fns:
            self.functions.append(self._compile_extern_fn(ef))

        # Emit function definitions
        for fn in fn_defs:
            self.functions.append(self._compile_fn(fn))

        # Build main
        saved_boxed = self.boxed_vars
        self.boxed_vars = self._find_captured_in_scope(top_stmts, set())
        main_body = self._compile_stmts(top_stmts, indent=1)
        self.boxed_vars = saved_boxed

        has_main = any(fn["name"] == "main" for fn in fn_defs)

        main_c = "int main(void) {\n"
        if main_body.strip():
            main_c += main_body
        if has_main:
            main_c += "    GemVal _margs[1] = {GEM_NIL};\n"
            main_c += "    gem_fn_main(NULL, _margs, 0);\n"
        main_c += "    return 0;\n}\n"

        # Assemble — extern includes go before the runtime
        out = ""
        for ei in extern_includes:
            out += f'#include "{ei["path"]}"\n'
        out += C_RUNTIME
        out += "\n".join(self.forward_decls) + "\n\n"
        out += "\n\n".join(self.functions) + "\n\n"
        out += main_c
        return out

    def _compile_extern_fn(self, node):
        """Generate a GemVal wrapper around a C function."""
        name = node["name"]
        params = node["params"]
        ret_type = node["ret_type"]

        type_map = {
            "Int": ("int64_t", "ival", "gem_int"),
            "Float": ("double", "fval", "gem_float"),
            "String": ("char*", "sval", "gem_string"),
            "Bool": ("int", "bval", "gem_bool"),
            "Ptr": ("void*", "ival", None),  # opaque pointer stored as ival
        }

        lines = []
        lines.append(f"static GemVal gem_fn_{name}(void *_env, GemVal *args, int argc) {{")

        # Extract args
        c_args = []
        for i, p in enumerate(params):
            ptype = p["type"]
            if ptype in type_map:
                c_type, field, _ = type_map[ptype]
                lines.append(f"    {c_type} _p{i} = args[{i}].{field};")
                c_args.append(f"_p{i}")
            else:
                c_args.append(f"args[{i}]")

        # Call
        call_expr = f"{name}({', '.join(c_args)})"

        if ret_type == "Nil":
            lines.append(f"    {call_expr};")
            lines.append("    return GEM_NIL;")
        elif ret_type in type_map:
            c_type, field, constructor = type_map[ret_type]
            if ret_type == "Ptr":
                lines.append(f"    void *_ret = {call_expr};")
                lines.append("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;")
            else:
                lines.append(f"    {c_type} _ret = {call_expr};")
                lines.append(f"    return {constructor}(_ret);")
        else:
            lines.append(f"    {call_expr};")
            lines.append("    return GEM_NIL;")

        lines.append("}")

        # Also add forward declaration
        self.forward_decls.append(f"static GemVal gem_fn_{name}(void *_env, GemVal *args, int argc);")

        return "\n".join(lines)

    def _compile_fn(self, node):
        params = node["params"]
        body = node["body"]
        name = node["name"]

        # Determine which variables are captured by nested closures
        scope_vars = set(params)
        for stmt in body:
            if isinstance(stmt, dict) and stmt.get("tag") == "let":
                scope_vars.add(stmt["name"])
        captured = self._find_captured_in_scope(body, scope_vars)

        saved_boxed = self.boxed_vars
        self.boxed_vars = captured

        lines = []
        lines.append(f"static GemVal gem_fn_{name}(void *_env, GemVal *args, int argc) {{")

        # Bind parameters
        for i, p in enumerate(params):
            mp = self._mangle(p)
            if p in captured:
                # Box: allocate on heap so closures can capture a pointer
                lines.append(f"    GemVal *{mp} = GC_MALLOC(sizeof(GemVal));")
                lines.append(f"    *{mp} = (argc > {i}) ? args[{i}] : GEM_NIL;")
            else:
                lines.append(f"    GemVal {mp} = (argc > {i}) ? args[{i}] : GEM_NIL;")

        # Compile body — last expression is the return value
        if body:
            for stmt in body[:-1]:
                lines.append(self._compile_stmt(stmt, indent=1))
            last = body[-1]
            lines.append(self._compile_stmt_return(last, indent=1))
        else:
            lines.append("    return GEM_NIL;")

        lines.append("}")
        self.boxed_vars = saved_boxed
        return "\n".join(lines)

    def _compile_closure_fn(self, node, captures):
        """Compile an anonymous function as a static C function with closure env."""
        params = node["params"]
        body = node["body"]
        name = self._anon_name()
        captures = sorted(captures)  # deterministic order

        # Find which vars inside THIS closure are captured by nested closures
        inner_scope = set(params)
        for stmt in body:
            if isinstance(stmt, dict) and stmt.get("tag") == "let":
                inner_scope.add(stmt["name"])
        inner_captured = self._find_captured_in_scope(body, inner_scope)
        # Also: captured vars from outer scope that we receive via env are GemVal*
        # and may be re-captured by inner closures — they're already boxed

        saved_boxed = self.boxed_vars
        self.boxed_vars = inner_captured | set(captures)  # all env vars are boxed (GemVal*)

        # Generate struct
        struct_name = f"_closure_{name}"
        struct_def = f"struct {struct_name} {{\n"
        for c in captures:
            struct_def += f"    GemVal *{self._mangle(c)};\n"
        struct_def += "};\n"

        # Generate function
        lines = []
        lines.append(f"static GemVal {name}(void *_env, GemVal *args, int argc) {{")
        if captures:
            lines.append(f"    struct {struct_name} *_cls = (struct {struct_name} *)_env;")
            for c in captures:
                mc = self._mangle(c)
                lines.append(f"    GemVal *{mc} = _cls->{mc};")

        # Bind parameters
        for i, p in enumerate(params):
            mp = self._mangle(p)
            if p in inner_captured:
                lines.append(f"    GemVal *{mp} = GC_MALLOC(sizeof(GemVal));")
                lines.append(f"    *{mp} = (argc > {i}) ? args[{i}] : GEM_NIL;")
            else:
                lines.append(f"    GemVal {mp} = (argc > {i}) ? args[{i}] : GEM_NIL;")

        if body:
            for stmt in body[:-1]:
                lines.append(self._compile_stmt(stmt, indent=1))
            last = body[-1]
            lines.append(self._compile_stmt_return(last, indent=1))
        else:
            lines.append("    return GEM_NIL;")

        lines.append("}")

        self.boxed_vars = saved_boxed

        # Add struct + function to forward_decls/functions
        self.functions.append(struct_def + "\n".join(lines))

        return name, struct_name, captures

    def _compile_stmts(self, stmts, indent=0):
        out = ""
        for s in stmts:
            out += self._compile_stmt(s, indent) + "\n"
        return out

    def _compile_stmt(self, node, indent=0):
        pad = "    " * indent
        tag = node["tag"]
        if tag == "let":
            expr_code, setup = self._compile_expr(node["value"])
            name = node["name"]
            mname = self._mangle(name)
            if name in self.boxed_vars:
                return setup + f"{pad}GemVal *{mname} = GC_MALLOC(sizeof(GemVal));\n{pad}*{mname} = {expr_code};"
            return setup + f"{pad}GemVal {mname} = {expr_code};"
        elif tag == "assign":
            expr_code, setup = self._compile_expr(node["value"])
            name = node["name"]
            mname = self._mangle(name)
            if name in self.boxed_vars:
                return setup + f"{pad}*{mname} = {expr_code};"
            return setup + f"{pad}{mname} = {expr_code};"
        elif tag == "dot_assign":
            obj_code, obj_setup = self._compile_expr(node["object"])
            val_code, val_setup = self._compile_expr(node["value"])
            escaped = node["field"].replace('"', '\\"')
            return obj_setup + val_setup + f'{pad}gem_table_set({obj_code}, gem_string("{escaped}"), {val_code});'
        elif tag == "index_assign":
            obj_code, obj_setup = self._compile_expr(node["object"])
            key_code, key_setup = self._compile_expr(node["key"])
            val_code, val_setup = self._compile_expr(node["value"])
            return obj_setup + key_setup + val_setup + f"{pad}gem_table_set({obj_code}, {key_code}, {val_code});"
        elif tag == "if":
            return self._compile_if(node, indent)
        elif tag == "while":
            return self._compile_while(node, indent)
        elif tag == "match":
            return self._compile_match(node, indent)
        elif tag == "break":
            return f"{pad}break;"
        elif tag == "continue":
            return f"{pad}continue;"
        elif tag == "return":
            if node["value"] is not None:
                expr_code, setup = self._compile_expr(node["value"])
                return setup + f"{pad}return {expr_code};"
            else:
                return f"{pad}return GEM_NIL;"
        elif tag == "call":
            expr_code, setup = self._compile_expr(node)
            return setup + f"{pad}(void)({expr_code});"
        elif tag == "fn_def":
            return ""
        else:
            expr_code, setup = self._compile_expr(node)
            return setup + f"{pad}(void)({expr_code});"

    def _compile_stmt_return(self, node, indent=0):
        pad = "    " * indent
        tag = node["tag"]
        if tag == "let":
            expr_code, setup = self._compile_expr(node["value"])
            name = node["name"]
            mname = self._mangle(name)
            if name in self.boxed_vars:
                return setup + f"{pad}GemVal *{mname} = GC_MALLOC(sizeof(GemVal));\n{pad}*{mname} = {expr_code};\n{pad}return GEM_NIL;"
            return setup + f"{pad}GemVal {mname} = {expr_code};\n{pad}return GEM_NIL;"
        elif tag == "assign":
            expr_code, setup = self._compile_expr(node["value"])
            tmp = self.tmp()
            name = node["name"]
            mname = self._mangle(name)
            if name in self.boxed_vars:
                return setup + f"{pad}GemVal {tmp} = {expr_code};\n{pad}*{mname} = {tmp};\n{pad}return {tmp};"
            return setup + f"{pad}GemVal {tmp} = {expr_code};\n{pad}{mname} = {tmp};\n{pad}return {tmp};"
        elif tag == "return":
            if node["value"] is not None:
                expr_code, setup = self._compile_expr(node["value"])
                return setup + f"{pad}return {expr_code};"
            else:
                return f"{pad}return GEM_NIL;"
        elif tag == "if":
            return self._compile_if_return(node, indent)
        elif tag == "while":
            return self._compile_stmt(node, indent) + f"\n{pad}return GEM_NIL;"
        elif tag == "match":
            return self._compile_match_return(node, indent)
        else:
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
            if name == "print":
                return "gem_make_fn(gem_print, NULL)", ""
            if name == "error":
                return "gem_make_fn(gem_error_fn, NULL)", ""
            if name == "len":
                return "gem_make_fn(gem_len_fn, NULL)", ""
            if name == "type":
                return "gem_make_fn(gem_type_fn, NULL)", ""
            if name == "to_string":
                return "gem_make_fn(gem_to_string_fn, NULL)", ""
            if name in self.defined_fns:
                return f"gem_make_fn(gem_fn_{name}, NULL)", ""
            mname = self._mangle(name)
            if name in self.boxed_vars:
                return f"(*{mname})", ""
            return mname, ""
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
        elif tag == "anon_fn":
            return self._compile_anon_fn(node)
        elif tag == "table":
            return self._compile_table(node)
        elif tag == "array":
            return self._compile_array(node)
        elif tag == "dot":
            obj_code, obj_setup = self._compile_expr(node["object"])
            tmp = self.tmp()
            setup = obj_setup + f"    GemVal {tmp} = {obj_code};\n"
            escaped = node["field"].replace('"', '\\"')
            return f'gem_table_get({tmp}, gem_string("{escaped}"))', setup
        elif tag == "index":
            obj_code, obj_setup = self._compile_expr(node["object"])
            key_code, key_setup = self._compile_expr(node["key"])
            return f"gem_table_get({obj_code}, {key_code})", obj_setup + key_setup
        else:
            return "GEM_NIL", f"/* unknown node: {tag} */\n"

    def _compile_table(self, node):
        entries = node["entries"]
        tmp = self.tmp()
        setup = f"    GemVal {tmp} = gem_table_new();\n"
        for entry in entries:
            val_code, val_setup = self._compile_expr(entry["value"])
            escaped = entry["key"].replace('"', '\\"')
            setup += val_setup
            setup += f'    gem_table_set({tmp}, gem_string("{escaped}"), {val_code});\n'
        return tmp, setup

    def _compile_array(self, node):
        elements = node["elements"]
        tmp = self.tmp()
        setup = f"    GemVal {tmp} = gem_table_new();\n"
        for i, elem in enumerate(elements):
            elem_code, elem_setup = self._compile_expr(elem)
            setup += elem_setup
            setup += f"    gem_table_set({tmp}, gem_int({i}), {elem_code});\n"
        return tmp, setup

    def _compile_anon_fn(self, node):
        """Compile an anonymous fn expression into a closure."""
        # Find free variables
        inner_defined = set(node["params"])
        free = self._collect_free_vars(node["body"], inner_defined)
        captures = sorted(free)  # variables to capture from enclosing scope

        if not captures:
            # No captures — simple function, no closure needed
            fn_name, _, _ = self._compile_closure_fn(node, [])
            return f"gem_make_fn({fn_name}, NULL)", ""

        # Generate the closure function and struct
        fn_name, struct_name, captures = self._compile_closure_fn(node, captures)

        # At the creation site, allocate env and populate
        setup = ""
        env_tmp = self.tmp()
        setup += f"    struct {struct_name} *{env_tmp} = GC_MALLOC(sizeof(struct {struct_name}));\n"
        for c in captures:
            mc = self._mangle(c)
            if c in self.boxed_vars:
                # Already a pointer — pass it directly
                setup += f"    {env_tmp}->{mc} = {mc};\n"
            else:
                # Need to take address — but this var isn't boxed!
                # This shouldn't happen if capture analysis is correct.
                # The var should have been boxed in the enclosing scope.
                setup += f"    {env_tmp}->{mc} = &{mc};\n"
        return f"gem_make_fn({fn_name}, {env_tmp})", setup

    def _compile_call(self, node):
        func = node["func"]
        args = node["args"]

        # Method call detection: obj.len() → gem_len_val(obj)
        if func["tag"] == "dot" and func["field"] == "len" and len(args) == 0:
            obj_code, obj_setup = self._compile_expr(func["object"])
            return f"gem_len_val({obj_code})", obj_setup

        # Direct call optimization for known functions
        if func["tag"] == "var":
            name = func["name"]
            if name == "error":
                # error() with file/line info
                line = node.get("line") or 0
                escaped_file = self.source_name.replace("\\", "\\\\").replace('"', '\\"')
                arg_setups = []
                arg_exprs = []
                for a in args:
                    ec, s = self._compile_expr(a)
                    arg_setups.append(s)
                    arg_exprs.append(ec)
                setup = "".join(arg_setups)
                argc = len(args)
                if argc == 0:
                    return f'gem_error_at_fn("{escaped_file}", {line}, NULL, 0)', setup
                tmp = self.tmp()
                arr = ", ".join(arg_exprs)
                setup += f"    GemVal {tmp}[] = {{{arr}}};\n"
                return f'gem_error_at_fn("{escaped_file}", {line}, {tmp}, {argc})', setup
            elif name in ("print", "len", "type", "to_string"):
                fn_map = {"print": "gem_print",
                          "len": "gem_len_fn", "type": "gem_type_fn",
                          "to_string": "gem_to_string_fn"}
                fn_name = fn_map[name]
                arg_setups = []
                arg_exprs = []
                for a in args:
                    ec, s = self._compile_expr(a)
                    arg_setups.append(s)
                    arg_exprs.append(ec)
                setup = "".join(arg_setups)
                argc = len(args)
                if argc == 0:
                    return f"{fn_name}(NULL, NULL, 0)", setup
                tmp = self.tmp()
                arr = ", ".join(arg_exprs)
                setup += f"    GemVal {tmp}[] = {{{arr}}};\n"
                return f"{fn_name}(NULL, {tmp}, {argc})", setup
            elif name in self.defined_fns:
                arg_setups = []
                arg_exprs = []
                for a in args:
                    ec, s = self._compile_expr(a)
                    arg_setups.append(s)
                    arg_exprs.append(ec)
                setup = "".join(arg_setups)
                argc = len(args)
                if argc == 0:
                    return f"gem_fn_{name}(NULL, NULL, 0)", setup
                tmp = self.tmp()
                arr = ", ".join(arg_exprs)
                setup += f"    GemVal {tmp}[] = {{{arr}}};\n"
                return f"gem_fn_{name}(NULL, {tmp}, {argc})", setup

        # General case: call through function value (could be closure)
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
            return f"{tmp_fn}.fn({tmp_fn}.env, NULL, 0)", setup
        tmp_args = self.tmp()
        arr = ", ".join(arg_exprs)
        setup += f"    GemVal {tmp_args}[] = {{{arr}}};\n"
        return f"{tmp_fn}.fn({tmp_fn}.env, {tmp_args}, {argc})", setup

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

    def _compile_if(self, node, indent=0):
        pad = "    " * indent
        cond_code, cond_setup = self._compile_expr(node["cond"])
        out = cond_setup
        out += f"{pad}if (gem_truthy({cond_code})) {{\n"
        out += self._compile_stmts(node["then"], indent + 1)
        if node["else"] is not None:
            out += f"{pad}}} else {{\n"
            out += self._compile_stmts(node["else"], indent + 1)
        out += f"{pad}}}"
        return out

    def _compile_if_return(self, node, indent=0):
        """Compile if/else where each branch's last stmt is a return."""
        pad = "    " * indent
        cond_code, cond_setup = self._compile_expr(node["cond"])
        out = cond_setup
        out += f"{pad}if (gem_truthy({cond_code})) {{\n"
        then_body = node["then"]
        if then_body:
            for s in then_body[:-1]:
                out += self._compile_stmt(s, indent + 1) + "\n"
            out += self._compile_stmt_return(then_body[-1], indent + 1) + "\n"
        else:
            out += f"{pad}    return GEM_NIL;\n"
        if node["else"] is not None:
            out += f"{pad}}} else {{\n"
            else_body = node["else"]
            if else_body:
                for s in else_body[:-1]:
                    out += self._compile_stmt(s, indent + 1) + "\n"
                out += self._compile_stmt_return(else_body[-1], indent + 1) + "\n"
            else:
                out += f"{pad}    return GEM_NIL;\n"
        else:
            out += f"{pad}}} else {{\n"
            out += f"{pad}    return GEM_NIL;\n"
        out += f"{pad}}}"
        return out

    def _compile_while(self, node, indent=0):
        pad = "    " * indent
        cond_code, cond_setup = self._compile_expr(node["cond"])
        out = f"{pad}while (1) {{\n"
        out += cond_setup.replace("    ", pad + "    ")  if cond_setup else ""
        out += f"{pad}    if (!gem_truthy({cond_code})) break;\n"
        out += self._compile_stmts(node["body"], indent + 1)
        out += f"{pad}}}"
        return out

    def _compile_match(self, node, indent=0):
        pad = "    " * indent
        target_code, target_setup = self._compile_expr(node["target"])
        tmp = self.tmp()
        out = target_setup
        out += f"{pad}GemVal {tmp} = {target_code};\n"
        for i, when in enumerate(node["whens"]):
            val_code, val_setup = self._compile_expr(when["value"])
            keyword = "if" if i == 0 else "} else if"
            out += val_setup
            out += f"{pad}{keyword} (gem_truthy(gem_eq({tmp}, {val_code}))) {{\n"
            out += self._compile_stmts(when["body"], indent + 1)
        if node["else"] is not None:
            out += f"{pad}}} else {{\n"
            out += self._compile_stmts(node["else"], indent + 1)
        out += f"{pad}}}"
        return out

    def _compile_match_return(self, node, indent=0):
        pad = "    " * indent
        target_code, target_setup = self._compile_expr(node["target"])
        tmp = self.tmp()
        out = target_setup
        out += f"{pad}GemVal {tmp} = {target_code};\n"
        for i, when in enumerate(node["whens"]):
            val_code, val_setup = self._compile_expr(when["value"])
            keyword = "if" if i == 0 else "} else if"
            out += val_setup
            out += f"{pad}{keyword} (gem_truthy(gem_eq({tmp}, {val_code}))) {{\n"
            body = when["body"]
            if body:
                for s in body[:-1]:
                    out += self._compile_stmt(s, indent + 1) + "\n"
                out += self._compile_stmt_return(body[-1], indent + 1) + "\n"
            else:
                out += f"{pad}    return GEM_NIL;\n"
        if node["else"] is not None:
            out += f"{pad}}} else {{\n"
            else_body = node["else"]
            if else_body:
                for s in else_body[:-1]:
                    out += self._compile_stmt(s, indent + 1) + "\n"
                out += self._compile_stmt_return(else_body[-1], indent + 1) + "\n"
            else:
                out += f"{pad}    return GEM_NIL;\n"
        else:
            out += f"{pad}}} else {{\n"
            out += f"{pad}    return GEM_NIL;\n"
        out += f"{pad}}}"
        return out


# ─── Driver ─────────────────────────────────────────────────────────────────

def resolve_loads(ast, base_dir, loaded=None):
    """Recursively resolve load statements by inlining loaded file ASTs."""
    if loaded is None:
        loaded = set()
    new_stmts = []
    for stmt in ast["stmts"]:
        if isinstance(stmt, dict) and stmt.get("tag") == "load":
            path = stmt["path"]
            # Resolve relative to base_dir, add .gem extension if needed
            if not path.endswith(".gem"):
                path += ".gem"
            full_path = os.path.normpath(os.path.join(base_dir, path))
            if full_path in loaded:
                continue  # re-inclusion guard
            loaded.add(full_path)
            if not os.path.exists(full_path):
                print(f"error: cannot load '{full_path}'", file=sys.stderr)
                sys.exit(1)
            with open(full_path) as f:
                source = f.read()
            try:
                tree = parser.parse(source)
            except UnexpectedInput as e:
                line = getattr(e, "line", "?")
                col = getattr(e, "column", "?")
                print(f"{full_path}:{line}:{col}: parse error: {e}", file=sys.stderr)
                sys.exit(1)
            loaded_ast = ASTBuilder().transform(tree)
            loaded_ast = resolve_loads(loaded_ast, os.path.dirname(full_path), loaded)
            new_stmts.extend(loaded_ast["stmts"])
        else:
            new_stmts.append(stmt)
    return {"tag": "program", "stmts": new_stmts}

def compile_gem(source, source_name="<stdin>", base_dir="."):
    try:
        tree = parser.parse(source)
    except UnexpectedInput as e:
        line = getattr(e, "line", "?")
        col = getattr(e, "column", "?")
        print(f"{source_name}:{line}:{col}: parse error: {e}", file=sys.stderr)
        sys.exit(1)
    ast = ASTBuilder().transform(tree)
    ast = resolve_loads(ast, base_dir)
    codegen = CodeGen(source_name=source_name)
    return codegen.compile(ast)

def main():
    if len(sys.argv) < 2:
        print("usage: gem.py <file.gem> [--emit-c] [--run]", file=sys.stderr)
        sys.exit(1)

    src_path = sys.argv[1]
    flags = set(sys.argv[2:])

    with open(src_path) as f:
        source = f.read()

    c_code = compile_gem(source, src_path, os.path.dirname(os.path.abspath(src_path)))

    if "--emit-c" in flags:
        print(c_code)
        return

    # Write C to temp file, compile, and run
    base = os.path.splitext(os.path.basename(src_path))[0]
    out_c = f"/tmp/gem_{base}.c"
    out_bin = f"/tmp/gem_{base}"

    with open(out_c, "w") as f:
        f.write(c_code)

    # Compile — link against the standalone runtime
    cc = os.environ.get("CC", "cc")
    runtime_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "runtime")
    runtime_c = os.path.join(runtime_dir, "gem.c")

    # Get Boehm GC flags via pkg-config
    gc_flags = subprocess.run(
        ["pkg-config", "--cflags", "--libs", "bdw-gc"],
        capture_output=True, text=True
    )
    gc_args = gc_flags.stdout.strip().split() if gc_flags.returncode == 0 else ["-lgc"]

    result = subprocess.run(
        [cc, "-o", out_bin, out_c, runtime_c, "-I", runtime_dir, "-std=c11", "-Wall"] + gc_args,
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
