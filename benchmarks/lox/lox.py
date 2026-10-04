#!/usr/bin/env python3
"""Reference implementation of examples/lox for benchmarks/lox/run.sh.

The same Lox interpreter as the Gem one, module for module: a scanner, a
recursive-descent parser building dict nodes with a "kind", jlox's
resolver, and a tree-walking interpreter that dispatches on the kind and
passes `return` up as a {"value": v} box. Same language, same natives,
same output and error messages, so the two can be timed on the same
programs and their output diffed.

    python3 lox.py script.lox [args...]
"""
import sys
import time

# ─── Lexer ─────────────────────────────────────────────────────

KEYWORDS = {"and", "class", "else", "false", "for", "fun", "if", "nil", "or",
            "print", "return", "super", "this", "true", "var", "while"}
SINGLE = set("(){},.-+;*")
WITH_EQUAL = set("!=<>")


def token(type_, lexeme, literal, line):
    return {"type": type_, "lexeme": lexeme, "literal": literal, "line": line}


def scan(source):
    tokens = []
    errors = []
    n = len(source)
    line = 1
    i = 0
    while i < n:
        ch = source[i]
        start = i
        i += 1
        if ch == "\n":
            line += 1
        elif ch in " \t\r":
            continue
        elif ch in SINGLE:
            tokens.append(token(ch, ch, None, line))
        elif ch in WITH_EQUAL:
            if i < n and source[i] == "=":
                i += 1
                tokens.append(token(ch + "=", ch + "=", None, line))
            else:
                tokens.append(token(ch, ch, None, line))
        elif ch == "/":
            if i < n and source[i] == "/":
                while i < n and source[i] != "\n":
                    i += 1
            else:
                tokens.append(token("/", "/", None, line))
        elif ch == '"':
            first_line = line
            while i < n and source[i] != '"':
                if source[i] == "\n":
                    line += 1
                i += 1
            if i >= n:
                errors.append(f"[line {line}] Error: Unterminated string.")
            else:
                i += 1
                tokens.append(token("string", source[start:i], source[start + 1:i - 1], first_line))
        elif "0" <= ch <= "9":
            while i < n and "0" <= source[i] <= "9":
                i += 1
            if i + 1 < n and source[i] == "." and "0" <= source[i + 1] <= "9":
                i += 1
                while i < n and "0" <= source[i] <= "9":
                    i += 1
            text = source[start:i]
            tokens.append(token("number", text, float(text), line))
        elif ch.isascii() and (ch.isalpha() or ch == "_"):
            while i < n and source[i].isascii() and (source[i].isalnum() or source[i] == "_"):
                i += 1
            text = source[start:i]
            tokens.append(token(text if text in KEYWORDS else "identifier", text, None, line))
        else:
            errors.append(f"[line {line}] Error: Unexpected character.")
    tokens.append(token("eof", "", None, line))
    return tokens, errors


# ─── Parser ────────────────────────────────────────────────────

MAX_ARGS = 255


class ParseError(Exception):
    pass


class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.pos = 0
        self.errors = []

    def parse(self):
        statements = []
        while not self.at_end():
            try:
                statements.append(self.declaration())
            except ParseError:
                self.synchronize()
        return statements

    def peek(self):
        return self.tokens[self.pos]

    def previous(self):
        return self.tokens[self.pos - 1]

    def at_end(self):
        return self.peek()["type"] == "eof"

    def check(self, type_):
        return self.peek()["type"] == type_

    def advance(self):
        t = self.peek()
        if t["type"] != "eof":
            self.pos += 1
        return t

    def accept(self, type_):
        if self.check(type_):
            self.advance()
            return True
        return False

    def expect(self, type_, message):
        if self.check(type_):
            return self.advance()
        self.fail(self.peek(), message)

    def report(self, t, message):
        if t["type"] == "eof":
            self.errors.append(f"[line {t['line']}] Error at end: {message}")
        else:
            self.errors.append(f"[line {t['line']}] Error at '{t['lexeme']}': {message}")

    def fail(self, t, message):
        self.report(t, message)
        raise ParseError()

    def synchronize(self):
        self.advance()
        while not self.at_end():
            if self.previous()["type"] == ";":
                return
            if self.peek()["type"] in ("class", "fun", "var", "for", "if", "while", "print", "return"):
                return
            self.advance()

    def declaration(self):
        if self.accept("class"):
            return self.class_declaration()
        if self.accept("fun"):
            return self.function("function")
        if self.accept("var"):
            return self.var_declaration()
        return self.statement()

    def class_declaration(self):
        name = self.expect("identifier", "Expect class name.")
        superclass = None
        if self.accept("<"):
            s = self.expect("identifier", "Expect superclass name.")
            superclass = {"kind": "variable", "name": s["lexeme"], "line": s["line"], "depth": None}
        self.expect("{", "Expect '{' before class body.")
        methods = []
        while not self.check("}") and not self.at_end():
            methods.append(self.function("method"))
        self.expect("}", "Expect '}' after class body.")
        return {"kind": "class", "name": name["lexeme"], "superclass": superclass,
                "methods": methods, "line": name["line"]}

    def function(self, what):
        name = self.expect("identifier", f"Expect {what} name.")
        self.expect("(", f"Expect '(' after {what} name.")
        params = []
        if not self.check(")"):
            more = True
            while more:
                if len(params) >= MAX_ARGS:
                    self.report(self.peek(), f"Can't have more than {MAX_ARGS} parameters.")
                params.append(self.expect("identifier", "Expect parameter name.")["lexeme"])
                more = self.accept(",")
        self.expect(")", "Expect ')' after parameters.")
        self.expect("{", f"Expect '{{' before {what} body.")
        return {"kind": "function", "name": name["lexeme"], "params": params,
                "body": self.block(), "line": name["line"]}

    def var_declaration(self):
        name = self.expect("identifier", "Expect variable name.")
        init = None
        if self.accept("="):
            init = self.expression()
        self.expect(";", "Expect ';' after variable declaration.")
        return {"kind": "var", "name": name["lexeme"], "init": init, "line": name["line"]}

    def statement(self):
        t = self.peek()
        match t["type"]:
            case "print":
                self.advance()
                value = self.expression()
                self.expect(";", "Expect ';' after value.")
                return {"kind": "print", "expr": value, "line": t["line"]}
            case "{":
                self.advance()
                return {"kind": "block", "statements": self.block(), "line": t["line"]}
            case "if":
                return self.if_statement()
            case "while":
                return self.while_statement()
            case "for":
                return self.for_statement()
            case "return":
                return self.return_statement()
            case _:
                e = self.expression()
                self.expect(";", "Expect ';' after expression.")
                return {"kind": "expr", "expr": e, "line": t["line"]}

    def block(self):
        statements = []
        while not self.check("}") and not self.at_end():
            statements.append(self.declaration())
        self.expect("}", "Expect '}' after block.")
        return statements

    def if_statement(self):
        line = self.advance()["line"]
        self.expect("(", "Expect '(' after 'if'.")
        cond = self.expression()
        self.expect(")", "Expect ')' after if condition.")
        then_branch = self.statement()
        else_branch = None
        if self.accept("else"):
            else_branch = self.statement()
        return {"kind": "if", "cond": cond, "then_branch": then_branch,
                "else_branch": else_branch, "line": line}

    def while_statement(self):
        line = self.advance()["line"]
        self.expect("(", "Expect '(' after 'while'.")
        cond = self.expression()
        self.expect(")", "Expect ')' after condition.")
        return {"kind": "while", "cond": cond, "body": self.statement(), "line": line}

    def for_statement(self):
        line = self.advance()["line"]
        self.expect("(", "Expect '(' after 'for'.")
        init = None
        if self.check(";"):
            self.advance()
        elif self.accept("var"):
            init = self.var_declaration()
        else:
            e = self.expression()
            self.expect(";", "Expect ';' after expression.")
            init = {"kind": "expr", "expr": e, "line": line}
        cond = {"kind": "literal", "value": True, "line": line}
        if not self.check(";"):
            cond = self.expression()
        self.expect(";", "Expect ';' after loop condition.")
        increment = None
        if not self.check(")"):
            increment = self.expression()
        self.expect(")", "Expect ')' after for clauses.")

        body = self.statement()
        if increment is not None:
            step = {"kind": "expr", "expr": increment, "line": increment["line"]}
            body = {"kind": "block", "statements": [body, step], "line": body["line"]}
        loop = {"kind": "while", "cond": cond, "body": body, "line": line}
        if init is None:
            return loop
        return {"kind": "block", "statements": [init, loop], "line": line}

    def return_statement(self):
        line = self.advance()["line"]
        value = None
        if not self.check(";"):
            value = self.expression()
        self.expect(";", "Expect ';' after return value.")
        return {"kind": "return", "value": value, "line": line}

    def expression(self):
        return self.assignment()

    def assignment(self):
        target = self.logic_or()
        if self.check("="):
            equals = self.advance()
            value = self.assignment()
            if target["kind"] == "variable":
                return {"kind": "assign", "name": target["name"], "value": value,
                        "line": target["line"], "depth": None}
            if target["kind"] == "get":
                return {"kind": "set", "object": target["object"], "name": target["name"],
                        "value": value, "line": target["line"]}
            self.report(equals, "Invalid assignment target.")
        return target

    def logic_or(self):
        e = self.logic_and()
        while self.check("or"):
            op = self.advance()
            e = {"kind": "logical", "op": "or", "left": e, "right": self.logic_and(), "line": op["line"]}
        return e

    def logic_and(self):
        e = self.equality()
        while self.check("and"):
            op = self.advance()
            e = {"kind": "logical", "op": "and", "left": e, "right": self.equality(), "line": op["line"]}
        return e

    def binary_level(self, ops, operand):
        e = operand()
        while self.peek()["type"] in ops:
            op = self.advance()
            e = {"kind": "binary", "op": op["type"], "left": e, "right": operand(), "line": op["line"]}
        return e

    def equality(self):
        return self.binary_level(("==", "!="), self.comparison)

    def comparison(self):
        return self.binary_level(("<", "<=", ">", ">="), self.term)

    def term(self):
        return self.binary_level(("+", "-"), self.factor)

    def factor(self):
        return self.binary_level(("*", "/"), self.unary)

    def unary(self):
        if self.check("!") or self.check("-"):
            op = self.advance()
            return {"kind": "unary", "op": op["type"], "right": self.unary(), "line": op["line"]}
        return self.call()

    def call(self):
        e = self.primary()
        while True:
            if self.check("("):
                paren = self.advance()
                e = {"kind": "call", "callee": e, "args": self.arguments(), "line": paren["line"]}
            elif self.accept("."):
                name = self.expect("identifier", "Expect property name after '.'.")
                e = {"kind": "get", "object": e, "name": name["lexeme"], "line": name["line"]}
            else:
                return e

    def arguments(self):
        args = []
        if not self.check(")"):
            more = True
            while more:
                if len(args) >= MAX_ARGS:
                    self.report(self.peek(), f"Can't have more than {MAX_ARGS} arguments.")
                args.append(self.expression())
                more = self.accept(",")
        self.expect(")", "Expect ')' after arguments.")
        return args

    def primary(self):
        t = self.advance()
        line = t["line"]
        match t["type"]:
            case "number" | "string":
                return {"kind": "literal", "value": t["literal"], "line": line}
            case "true":
                return {"kind": "literal", "value": True, "line": line}
            case "false":
                return {"kind": "literal", "value": False, "line": line}
            case "nil":
                return {"kind": "literal", "value": None, "line": line}
            case "identifier":
                return {"kind": "variable", "name": t["lexeme"], "line": line, "depth": None}
            case "this":
                return {"kind": "this", "line": line, "depth": None}
            case "super":
                self.expect(".", "Expect '.' after 'super'.")
                method = self.expect("identifier", "Expect superclass method name.")
                return {"kind": "super", "method": method["lexeme"], "line": line, "depth": None}
            case "(":
                e = self.expression()
                self.expect(")", "Expect ')' after expression.")
                return {"kind": "grouping", "expr": e, "line": line}
            case _:
                if t["type"] != "eof":
                    self.pos -= 1
                self.fail(t, "Expect expression.")


# ─── Resolver ──────────────────────────────────────────────────

class Resolver:
    def __init__(self):
        self.scopes = []
        self.function = "none"
        self.klass = "none"
        self.errors = []

    def report(self, line, where, message):
        self.errors.append(f"[line {line}] Error at '{where}': {message}")

    def declare(self, name, line):
        if not self.scopes:
            return
        scope = self.scopes[-1]
        if name in scope:
            self.report(line, name, "Already a variable with this name in this scope.")
        scope[name] = False

    def define(self, name):
        if self.scopes:
            self.scopes[-1][name] = True

    def resolve_local(self, node, name):
        top = len(self.scopes) - 1
        for i in range(top, -1, -1):
            if name in self.scopes[i]:
                node["depth"] = top - i
                return

    def resolve_all(self, statements):
        for s in statements:
            self.statement(s)

    def statement(self, s):
        match s["kind"]:
            case "expr" | "print":
                self.expression(s["expr"])
            case "var":
                self.declare(s["name"], s["line"])
                if s["init"] is not None:
                    self.expression(s["init"])
                self.define(s["name"])
            case "block":
                self.scopes.append({})
                self.resolve_all(s["statements"])
                self.scopes.pop()
            case "if":
                self.expression(s["cond"])
                self.statement(s["then_branch"])
                if s["else_branch"] is not None:
                    self.statement(s["else_branch"])
            case "while":
                self.expression(s["cond"])
                self.statement(s["body"])
            case "function":
                self.declare(s["name"], s["line"])
                self.define(s["name"])
                self.resolve_function(s, "function")
            case "return":
                if self.function == "none":
                    self.report(s["line"], "return", "Can't return from top-level code.")
                if s["value"] is not None:
                    if self.function == "initializer":
                        self.report(s["line"], "return", "Can't return a value from an initializer.")
                    self.expression(s["value"])
            case "class":
                self.class_declaration(s)
            case kind:
                raise AssertionError(f"resolver: unknown statement {kind}")

    def resolve_function(self, f, kind):
        enclosing = self.function
        self.function = kind
        self.scopes.append({})
        for p in f["params"]:
            self.declare(p, f["line"])
            self.define(p)
        self.resolve_all(f["body"])
        self.scopes.pop()
        self.function = enclosing

    def class_declaration(self, s):
        enclosing = self.klass
        self.klass = "class"
        self.declare(s["name"], s["line"])
        self.define(s["name"])
        superclass = s["superclass"]
        if superclass is not None:
            if superclass["name"] == s["name"]:
                self.report(superclass["line"], s["name"], "A class can't inherit from itself.")
            self.klass = "subclass"
            self.expression(superclass)
            self.scopes.append({"super": True})
        self.scopes.append({"this": True})
        for m in s["methods"]:
            self.resolve_function(m, "initializer" if m["name"] == "init" else "method")
        self.scopes.pop()
        if superclass is not None:
            self.scopes.pop()
        self.klass = enclosing

    def expression(self, e):
        match e["kind"]:
            case "literal":
                pass
            case "variable":
                if self.scopes and self.scopes[-1].get(e["name"]) is False:
                    self.report(e["line"], e["name"], "Can't read local variable in its own initializer.")
                self.resolve_local(e, e["name"])
            case "assign":
                self.expression(e["value"])
                self.resolve_local(e, e["name"])
            case "binary" | "logical":
                self.expression(e["left"])
                self.expression(e["right"])
            case "unary":
                self.expression(e["right"])
            case "grouping":
                self.expression(e["expr"])
            case "call":
                self.expression(e["callee"])
                for a in e["args"]:
                    self.expression(a)
            case "get":
                self.expression(e["object"])
            case "set":
                self.expression(e["value"])
                self.expression(e["object"])
            case "this":
                if self.klass == "none":
                    self.report(e["line"], "this", "Can't use 'this' outside of a class.")
                self.resolve_local(e, "this")
            case "super":
                if self.klass == "none":
                    self.report(e["line"], "super", "Can't use 'super' outside of a class.")
                elif self.klass != "subclass":
                    self.report(e["line"], "super", "Can't use 'super' in a class with no superclass.")
                self.resolve_local(e, "super")
            case kind:
                raise AssertionError(f"resolver: unknown expression {kind}")


# ─── Interpreter ───────────────────────────────────────────────

MAX_CALL_DEPTH = 256


class LoxRuntimeError(Exception):
    def __init__(self, line, message):
        super().__init__(f"{message}\n[line {line}]")


class Environment:
    __slots__ = ("values", "enclosing")

    def __init__(self, enclosing):
        self.values = {}
        self.enclosing = enclosing


class Function:
    __slots__ = ("decl", "closure", "is_init")

    def __init__(self, decl, closure, is_init):
        self.decl = decl
        self.closure = closure
        self.is_init = is_init


class Native:
    __slots__ = ("name", "arity", "impl")

    def __init__(self, name, arity, impl):
        self.name = name
        self.arity = arity
        self.impl = impl


class Class:
    __slots__ = ("name", "superclass", "methods")

    def __init__(self, name, superclass, methods):
        self.name = name
        self.superclass = superclass
        self.methods = methods


class Instance:
    __slots__ = ("klass", "fields")

    def __init__(self, klass):
        self.klass = klass
        self.fields = {}


def number_to_string(x):
    if -1e16 < x < 1e16 and x == int(x):
        return str(int(x))
    return repr(x)


def stringify(v):
    if v is None:
        return "nil"
    if v is True:
        return "true"
    if v is False:
        return "false"
    if type(v) is float:
        return number_to_string(v)
    if type(v) is str:
        return v
    if type(v) is Function:
        return f"<fn {v.decl['name']}>"
    if type(v) is Native:
        return "<native fn>"
    if type(v) is Class:
        return v.name
    return f"{v.klass.name} instance"


def is_equal(a, b):
    # Python's == says True == 1.0; Lox's doesn't.
    return type(a) is type(b) and a == b


def find_method(klass, name):
    while klass is not None:
        m = klass.methods.get(name)
        if m is not None:
            return m
        klass = klass.superclass
    return None


def bind(method, instance):
    env = Environment(method.closure)
    env.values["this"] = instance
    return Function(method.decl, env, method.is_init)


def ancestor(env, depth):
    for _ in range(depth):
        env = env.enclosing
    return env


class Interpreter:
    def __init__(self, out, args):
        self.globals = Environment(None)
        self.call_depth = 0
        self.out = out
        self.define_natives(args)

    def define_natives(self, args):
        v = self.globals.values

        def clock(a, line):
            return time.time()

        def str_(a, line):
            return stringify(a[0])

        def len_(a, line):
            if type(a[0]) is not str:
                raise LoxRuntimeError(line, "len() takes a string.")
            return float(len(a[0].encode()))

        def arg(a, line):
            if type(a[0]) is not float:
                raise LoxRuntimeError(line, "arg() takes a number.")
            i = int(a[0])
            if i < 0 or i >= len(args):
                return None
            try:
                return float(args[i])
            except ValueError:
                return args[i]

        v["clock"] = Native("clock", 0, clock)
        v["str"] = Native("str", 1, str_)
        v["len"] = Native("len", 1, len_)
        v["arg"] = Native("arg", 1, arg)

    def run(self, statements):
        try:
            self.exec_block(statements, self.globals)
        except LoxRuntimeError as e:
            return str(e)
        except RecursionError:
            return "Stack overflow."
        return None

    # Statements return None, or {"value": v} when a `return` ran.
    def exec_block(self, statements, env):
        for s in statements:
            r = self.execute(s, env)
            if r is not None:
                return r
        return None

    def execute(self, s, env):
        match s["kind"]:
            case "expr":
                self.evaluate(s["expr"], env)
                return None
            case "print":
                self.out(stringify(self.evaluate(s["expr"], env)))
                return None
            case "var":
                value = None
                if s["init"] is not None:
                    value = self.evaluate(s["init"], env)
                env.values[s["name"]] = value
                return None
            case "block":
                return self.exec_block(s["statements"], Environment(env))
            case "if":
                if truthy(self.evaluate(s["cond"], env)):
                    return self.execute(s["then_branch"], env)
                if s["else_branch"] is not None:
                    return self.execute(s["else_branch"], env)
                return None
            case "while":
                while truthy(self.evaluate(s["cond"], env)):
                    r = self.execute(s["body"], env)
                    if r is not None:
                        return r
                return None
            case "return":
                value = None
                if s["value"] is not None:
                    value = self.evaluate(s["value"], env)
                return {"value": value}
            case "function":
                env.values[s["name"]] = Function(s, env, False)
                return None
            case "class":
                return self.define_class(s, env)
            case kind:
                raise AssertionError(f"interp: unknown statement {kind}")

    def define_class(self, s, env):
        superclass = None
        if s["superclass"] is not None:
            superclass = self.evaluate(s["superclass"], env)
            if type(superclass) is not Class:
                raise LoxRuntimeError(s["superclass"]["line"], "Superclass must be a class.")
        env.values[s["name"]] = None
        method_env = env
        if superclass is not None:
            method_env = Environment(env)
            method_env.values["super"] = superclass
        methods = {}
        for m in s["methods"]:
            methods[m["name"]] = Function(m, method_env, m["name"] == "init")
        env.values[s["name"]] = Class(s["name"], superclass, methods)
        return None

    def evaluate(self, e, env):
        match e["kind"]:
            case "literal":
                return e["value"]
            case "variable":
                return self.look_up(e, env)
            case "binary":
                return self.binary(e, env)
            case "call":
                return self.call(e, env)
            case "get":
                return self.get_property(e, env)
            case "assign":
                return self.assign(e, env)
            case "logical":
                left = self.evaluate(e["left"], env)
                if e["op"] == "or":
                    if truthy(left):
                        return left
                elif not truthy(left):
                    return left
                return self.evaluate(e["right"], env)
            case "unary":
                return self.unary(e, env)
            case "grouping":
                return self.evaluate(e["expr"], env)
            case "set":
                return self.set_property(e, env)
            case "this":
                return ancestor(env, e["depth"]).values["this"]
            case "super":
                return self.super_method(e, env)
            case kind:
                raise AssertionError(f"interp: unknown expression {kind}")

    def look_up(self, e, env):
        if e["depth"] is not None:
            return ancestor(env, e["depth"]).values[e["name"]]
        values = self.globals.values
        if e["name"] not in values:
            raise LoxRuntimeError(e["line"], f"Undefined variable '{e['name']}'.")
        return values[e["name"]]

    def assign(self, e, env):
        value = self.evaluate(e["value"], env)
        if e["depth"] is not None:
            ancestor(env, e["depth"]).values[e["name"]] = value
            return value
        if e["name"] not in self.globals.values:
            raise LoxRuntimeError(e["line"], f"Undefined variable '{e['name']}'.")
        self.globals.values[e["name"]] = value
        return value

    def unary(self, e, env):
        right = self.evaluate(e["right"], env)
        if e["op"] == "!":
            return not truthy(right)
        if type(right) is not float:
            raise LoxRuntimeError(e["line"], "Operand must be a number.")
        return -right

    def binary(self, e, env):
        a = self.evaluate(e["left"], env)
        b = self.evaluate(e["right"], env)
        match e["op"]:
            case "+":
                if type(a) is float and type(b) is float:
                    return a + b
                if type(a) is str and type(b) is str:
                    return a + b
                raise LoxRuntimeError(e["line"], "Operands must be two numbers or two strings.")
            case "-":
                check_numbers(e, a, b)
                return a - b
            case "*":
                check_numbers(e, a, b)
                return a * b
            case "/":
                check_numbers(e, a, b)
                if b == 0.0:
                    raise LoxRuntimeError(e["line"], "Division by zero.")
                return a / b
            case "<":
                check_numbers(e, a, b)
                return a < b
            case "<=":
                check_numbers(e, a, b)
                return a <= b
            case ">":
                check_numbers(e, a, b)
                return a > b
            case ">=":
                check_numbers(e, a, b)
                return a >= b
            case "==":
                return is_equal(a, b)
            case "!=":
                return not is_equal(a, b)
            case op:
                raise AssertionError(f"interp: unknown operator {op}")

    def call(self, e, env):
        callee = self.evaluate(e["callee"], env)
        args = []
        for a in e["args"]:
            args.append(self.evaluate(a, env))
        kind = type(callee)
        if kind is Function:
            check_arity(e, len(callee.decl["params"]), len(args))
            return self.call_function(callee, args, e["line"])
        if kind is Native:
            check_arity(e, callee.arity, len(args))
            return callee.impl(args, e["line"])
        if kind is Class:
            return self.instantiate(callee, args, e)
        raise LoxRuntimeError(e["line"], "Can only call functions and classes.")

    def call_function(self, f, args, line):
        self.call_depth += 1
        if self.call_depth > MAX_CALL_DEPTH:
            raise LoxRuntimeError(line, "Stack overflow.")
        env = Environment(f.closure)
        params = f.decl["params"]
        for i in range(len(params)):
            env.values[params[i]] = args[i]
        r = self.exec_block(f.decl["body"], env)
        self.call_depth -= 1
        if f.is_init:
            return f.closure.values["this"]
        if r is not None:
            return r["value"]
        return None

    def instantiate(self, klass, args, e):
        instance = Instance(klass)
        init = find_method(klass, "init")
        if init is None:
            check_arity(e, 0, len(args))
        else:
            check_arity(e, len(init.decl["params"]), len(args))
            self.call_function(bind(init, instance), args, e["line"])
        return instance

    def get_property(self, e, env):
        obj = self.evaluate(e["object"], env)
        if type(obj) is not Instance:
            raise LoxRuntimeError(e["line"], "Only instances have properties.")
        if e["name"] in obj.fields:
            return obj.fields[e["name"]]
        method = find_method(obj.klass, e["name"])
        if method is None:
            raise LoxRuntimeError(e["line"], f"Undefined property '{e['name']}'.")
        return bind(method, obj)

    def set_property(self, e, env):
        obj = self.evaluate(e["object"], env)
        if type(obj) is not Instance:
            raise LoxRuntimeError(e["line"], "Only instances have fields.")
        value = self.evaluate(e["value"], env)
        obj.fields[e["name"]] = value
        return value

    def super_method(self, e, env):
        superclass = ancestor(env, e["depth"]).values["super"]
        obj = ancestor(env, e["depth"] - 1).values["this"]
        method = find_method(superclass, e["method"])
        if method is None:
            raise LoxRuntimeError(e["line"], f"Undefined property '{e['method']}'.")
        return bind(method, obj)


def truthy(v):
    return v is not None and v is not False


def check_numbers(e, a, b):
    if type(a) is not float or type(b) is not float:
        raise LoxRuntimeError(e["line"], "Operands must be numbers.")


def check_arity(e, arity, count):
    if arity != count:
        raise LoxRuntimeError(e["line"], f"Expected {arity} arguments but got {count}.")


# ─── Driver ────────────────────────────────────────────────────

def run(source, out=print, args=()):
    """Returns (status, errors), as examples/lox/lox.gem's run does."""
    tokens, errors = scan(source)
    parser = Parser(tokens)
    statements = parser.parse()
    errors += parser.errors
    if errors:
        return 65, errors
    resolver = Resolver()
    resolver.resolve_all(statements)
    if resolver.errors:
        return 65, resolver.errors
    error = Interpreter(out, list(args)).run(statements)
    if error is not None:
        return 70, [error]
    return 0, []


def main():
    if len(sys.argv) < 2:
        print("usage: lox script.lox [args...]", file=sys.stderr)
        sys.exit(64)
    try:
        with open(sys.argv[1], encoding="utf-8") as f:
            source = f.read()
    except OSError as e:
        print(f"lox: cannot read {sys.argv[1]}: {e.strerror}", file=sys.stderr)
        sys.exit(66)
    sys.setrecursionlimit(20000)
    status, errors = run(source, args=sys.argv[2:])
    for e in errors:
        print(e, file=sys.stderr)
    sys.exit(status)


if __name__ == "__main__":
    main()
