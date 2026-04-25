#include "file_io.h"
#include "compiler_helpers.h"
#include "gem.h"
static GemVal gem_fn_make_program(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_int(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_float(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_string(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_bool(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_nil_node(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_var(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_binop(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_unop(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_call(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_dot(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_index(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_anon_fn(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_table(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_table_entry(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_array(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_interp(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_assign(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_dot_assign(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_index_assign(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_fn_def(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_if(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_while(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_match(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_when(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_return(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_break(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_continue(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_load(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_extern_fn(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_extern_param(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_extern_include(void *_env, GemVal *args, int argc);
static GemVal gem_fn_is_digit(void *_env, GemVal *args, int argc);
static GemVal gem_fn_is_alpha(void *_env, GemVal *args, int argc);
static GemVal gem_fn_is_alnum(void *_env, GemVal *args, int argc);
static GemVal gem_fn_digit_val(void *_env, GemVal *args, int argc);
static GemVal gem_fn_str_to_int(void *_env, GemVal *args, int argc);
static GemVal gem_fn_contains_dot(void *_env, GemVal *args, int argc);
static GemVal gem_fn_build_keyword_set(void *_env, GemVal *args, int argc);
static GemVal gem_fn_tokenize(void *_env, GemVal *args, int argc);
static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_parser(void *_env, GemVal *args, int argc);
static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc);
static GemVal gem_fn_repeat_str(void *_env, GemVal *args, int argc);
static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc);
static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc);
static GemVal gem_fn_array_contains(void *_env, GemVal *args, int argc);
static GemVal gem_fn_set_add(void *_env, GemVal *args, int argc);
static GemVal gem_fn_set_contains(void *_env, GemVal *args, int argc);
static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc);
static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc);
static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc);
static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc);
static GemVal gem_fn_split_string(void *_env, GemVal *args, int argc);
static GemVal gem_fn_str_replace_all(void *_env, GemVal *args, int argc);
static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc);
static GemVal gem_fn_atof(void *_env, GemVal *args, int argc);
static GemVal gem_fn_keys(void *_env, GemVal *args, int argc);
static GemVal gem_fn_gem_read_file(void *_env, GemVal *args, int argc);
static GemVal gem_fn_puts(void *_env, GemVal *args, int argc);
static GemVal gem_fn_gem_get_argv(void *_env, GemVal *args, int argc);
static GemVal gem_fn_gem_get_argc(void *_env, GemVal *args, int argc);
static GemVal gem_fn_gem_dirname(void *_env, GemVal *args, int argc);
static GemVal gem_fn_gem_path_join(void *_env, GemVal *args, int argc);
static GemVal gem_fn_gem_normalize_path(void *_env, GemVal *args, int argc);

static GemVal gem_fn_atof(void *_env, GemVal *args, int argc) {
    char* _p0 = args[0].sval;
    double _ret = atof(_p0);
    return gem_float(_ret);
}

static GemVal gem_fn_keys(void *_env, GemVal *args, int argc) {
    return keys(args[0]);
}

static GemVal gem_fn_gem_read_file(void *_env, GemVal *args, int argc) {
    char* _p0 = args[0].sval;
    char* _ret = gem_read_file(_p0);
    return gem_string(_ret);
}

static GemVal gem_fn_puts(void *_env, GemVal *args, int argc) {
    char* _p0 = args[0].sval;
    int64_t _ret = puts(_p0);
    return gem_int(_ret);
}

static GemVal gem_fn_gem_get_argv(void *_env, GemVal *args, int argc) {
    int64_t _p0 = args[0].ival;
    char* _ret = gem_get_argv(_p0);
    return gem_string(_ret);
}

static GemVal gem_fn_gem_get_argc(void *_env, GemVal *args, int argc) {
    int64_t _ret = gem_get_argc();
    return gem_int(_ret);
}

static GemVal gem_fn_gem_dirname(void *_env, GemVal *args, int argc) {
    char* _p0 = args[0].sval;
    char* _ret = gem_dirname(_p0);
    return gem_string(_ret);
}

static GemVal gem_fn_gem_path_join(void *_env, GemVal *args, int argc) {
    char* _p0 = args[0].sval;
    char* _p1 = args[1].sval;
    char* _ret = gem_path_join(_p0, _p1);
    return gem_string(_ret);
}

static GemVal gem_fn_gem_normalize_path(void *_env, GemVal *args, int argc) {
    char* _p0 = args[0].sval;
    char* _ret = gem_normalize_path(_p0);
    return gem_string(_ret);
}

static GemVal gem_fn_make_program(void *_env, GemVal *args, int argc) {
#line 8 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t1 = gem_table_new();
    gem_table_set(_t1, gem_string("tag"), gem_string("program"));
    gem_table_set(_t1, gem_string("stmts"), gem_v_stmts);
    return _t1;
}

static GemVal gem_fn_make_int(void *_env, GemVal *args, int argc) {
#line 14 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t2 = gem_table_new();
    gem_table_set(_t2, gem_string("tag"), gem_string("int"));
    gem_table_set(_t2, gem_string("value"), gem_v_value);
    return _t2;
}

static GemVal gem_fn_make_float(void *_env, GemVal *args, int argc) {
#line 18 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t3 = gem_table_new();
    gem_table_set(_t3, gem_string("tag"), gem_string("float"));
    gem_table_set(_t3, gem_string("value"), gem_v_value);
    return _t3;
}

static GemVal gem_fn_make_string(void *_env, GemVal *args, int argc) {
#line 22 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t4 = gem_table_new();
    gem_table_set(_t4, gem_string("tag"), gem_string("string"));
    gem_table_set(_t4, gem_string("value"), gem_v_value);
    return _t4;
}

static GemVal gem_fn_make_bool(void *_env, GemVal *args, int argc) {
#line 26 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t5 = gem_table_new();
    gem_table_set(_t5, gem_string("tag"), gem_string("bool"));
    gem_table_set(_t5, gem_string("value"), gem_v_value);
    return _t5;
}

static GemVal gem_fn_make_nil_node(void *_env, GemVal *args, int argc) {
#line 30 "compiler/main.gem"
    GemVal _t6 = gem_table_new();
    gem_table_set(_t6, gem_string("tag"), gem_string("nil"));
    return _t6;
}

static GemVal gem_fn_make_var(void *_env, GemVal *args, int argc) {
#line 34 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t7 = gem_table_new();
    gem_table_set(_t7, gem_string("tag"), gem_string("var"));
    gem_table_set(_t7, gem_string("name"), gem_v_name);
    return _t7;
}

static GemVal gem_fn_make_binop(void *_env, GemVal *args, int argc) {
#line 38 "compiler/main.gem"
    GemVal gem_v_op = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_left = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_right = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t8 = gem_table_new();
    gem_table_set(_t8, gem_string("tag"), gem_string("binop"));
    gem_table_set(_t8, gem_string("op"), gem_v_op);
    gem_table_set(_t8, gem_string("left"), gem_v_left);
    gem_table_set(_t8, gem_string("right"), gem_v_right);
    return _t8;
}

static GemVal gem_fn_make_unop(void *_env, GemVal *args, int argc) {
#line 42 "compiler/main.gem"
    GemVal gem_v_op = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_expr = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t9 = gem_table_new();
    gem_table_set(_t9, gem_string("tag"), gem_string("unop"));
    gem_table_set(_t9, gem_string("op"), gem_v_op);
    gem_table_set(_t9, gem_string("expr"), gem_v_expr);
    return _t9;
}

static GemVal gem_fn_make_call(void *_env, GemVal *args, int argc) {
#line 46 "compiler/main.gem"
    GemVal gem_v_func = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_args = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t10 = gem_table_new();
    gem_table_set(_t10, gem_string("tag"), gem_string("call"));
    gem_table_set(_t10, gem_string("func"), gem_v_func);
    gem_table_set(_t10, gem_string("args"), gem_v_args);
    gem_table_set(_t10, gem_string("line"), gem_v_line);
    return _t10;
}

static GemVal gem_fn_make_dot(void *_env, GemVal *args, int argc) {
#line 50 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t11 = gem_table_new();
    gem_table_set(_t11, gem_string("tag"), gem_string("dot"));
    gem_table_set(_t11, gem_string("object"), gem_v_object);
    gem_table_set(_t11, gem_string("field"), gem_v_field);
    return _t11;
}

static GemVal gem_fn_make_index(void *_env, GemVal *args, int argc) {
#line 54 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t12 = gem_table_new();
    gem_table_set(_t12, gem_string("tag"), gem_string("index"));
    gem_table_set(_t12, gem_string("object"), gem_v_object);
    gem_table_set(_t12, gem_string("key"), gem_v_key);
    return _t12;
}

static GemVal gem_fn_make_anon_fn(void *_env, GemVal *args, int argc) {
#line 58 "compiler/main.gem"
    GemVal gem_v_params = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t13 = gem_table_new();
    gem_table_set(_t13, gem_string("tag"), gem_string("anon_fn"));
    gem_table_set(_t13, gem_string("params"), gem_v_params);
    gem_table_set(_t13, gem_string("body"), gem_v_body);
    return _t13;
}

static GemVal gem_fn_make_table(void *_env, GemVal *args, int argc) {
#line 62 "compiler/main.gem"
    GemVal gem_v_entries = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t14 = gem_table_new();
    gem_table_set(_t14, gem_string("tag"), gem_string("table"));
    gem_table_set(_t14, gem_string("entries"), gem_v_entries);
    return _t14;
}

static GemVal gem_fn_make_table_entry(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_key = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t15 = gem_table_new();
    gem_table_set(_t15, gem_string("key"), gem_v_key);
    gem_table_set(_t15, gem_string("value"), gem_v_value);
    return _t15;
}

static GemVal gem_fn_make_array(void *_env, GemVal *args, int argc) {
#line 70 "compiler/main.gem"
    GemVal gem_v_elements = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t16 = gem_table_new();
    gem_table_set(_t16, gem_string("tag"), gem_string("array"));
    gem_table_set(_t16, gem_string("elements"), gem_v_elements);
    return _t16;
}

static GemVal gem_fn_make_interp(void *_env, GemVal *args, int argc) {
#line 74 "compiler/main.gem"
    GemVal gem_v_parts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t17 = gem_table_new();
    gem_table_set(_t17, gem_string("tag"), gem_string("interp"));
    gem_table_set(_t17, gem_string("parts"), gem_v_parts);
    return _t17;
}

static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc) {
#line 80 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t18 = gem_table_new();
    gem_table_set(_t18, gem_string("tag"), gem_string("let"));
    gem_table_set(_t18, gem_string("name"), gem_v_name);
    gem_table_set(_t18, gem_string("value"), gem_v_value);
    gem_table_set(_t18, gem_string("line"), gem_v_line);
    return _t18;
}

static GemVal gem_fn_make_assign(void *_env, GemVal *args, int argc) {
#line 84 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t19 = gem_table_new();
    gem_table_set(_t19, gem_string("tag"), gem_string("assign"));
    gem_table_set(_t19, gem_string("name"), gem_v_name);
    gem_table_set(_t19, gem_string("value"), gem_v_value);
    gem_table_set(_t19, gem_string("line"), gem_v_line);
    return _t19;
}

static GemVal gem_fn_make_dot_assign(void *_env, GemVal *args, int argc) {
#line 88 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    GemVal _t20 = gem_table_new();
    gem_table_set(_t20, gem_string("tag"), gem_string("dot_assign"));
    gem_table_set(_t20, gem_string("object"), gem_v_object);
    gem_table_set(_t20, gem_string("field"), gem_v_field);
    gem_table_set(_t20, gem_string("value"), gem_v_value);
    gem_table_set(_t20, gem_string("line"), gem_v_line);
    return _t20;
}

static GemVal gem_fn_make_index_assign(void *_env, GemVal *args, int argc) {
#line 92 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    GemVal _t21 = gem_table_new();
    gem_table_set(_t21, gem_string("tag"), gem_string("index_assign"));
    gem_table_set(_t21, gem_string("object"), gem_v_object);
    gem_table_set(_t21, gem_string("key"), gem_v_key);
    gem_table_set(_t21, gem_string("value"), gem_v_value);
    gem_table_set(_t21, gem_string("line"), gem_v_line);
    return _t21;
}

static GemVal gem_fn_make_fn_def(void *_env, GemVal *args, int argc) {
#line 96 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    GemVal _t22 = gem_table_new();
    gem_table_set(_t22, gem_string("tag"), gem_string("fn_def"));
    gem_table_set(_t22, gem_string("name"), gem_v_name);
    gem_table_set(_t22, gem_string("params"), gem_v_params);
    gem_table_set(_t22, gem_string("body"), gem_v_body);
    gem_table_set(_t22, gem_string("line"), gem_v_line);
    return _t22;
}

static GemVal gem_fn_make_if(void *_env, GemVal *args, int argc) {
#line 100 "compiler/main.gem"
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_then_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
#line 101 "compiler/main.gem"
    GemVal _t23 = gem_table_new();
    gem_table_set(_t23, gem_string("tag"), gem_string("if"));
    gem_table_set(_t23, gem_string("cond"), gem_v_cond);
    gem_table_set(_t23, gem_string("then"), gem_v_then_body);
    gem_table_set(_t23, gem_string("line"), gem_v_line);
    GemVal gem_v_node = _t23;
#line 102 "compiler/main.gem"
    gem_table_set(gem_v_node, gem_string("else"), gem_v_else_body);
    return gem_v_node;
}

static GemVal gem_fn_make_while(void *_env, GemVal *args, int argc) {
#line 106 "compiler/main.gem"
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t24 = gem_table_new();
    gem_table_set(_t24, gem_string("tag"), gem_string("while"));
    gem_table_set(_t24, gem_string("cond"), gem_v_cond);
    gem_table_set(_t24, gem_string("body"), gem_v_body);
    gem_table_set(_t24, gem_string("line"), gem_v_line);
    return _t24;
}

static GemVal gem_fn_make_match(void *_env, GemVal *args, int argc) {
#line 110 "compiler/main.gem"
    GemVal gem_v_target = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_whens = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
#line 111 "compiler/main.gem"
    GemVal _t25 = gem_table_new();
    gem_table_set(_t25, gem_string("tag"), gem_string("match"));
    gem_table_set(_t25, gem_string("target"), gem_v_target);
    gem_table_set(_t25, gem_string("whens"), gem_v_whens);
    gem_table_set(_t25, gem_string("line"), gem_v_line);
    GemVal gem_v_node = _t25;
#line 112 "compiler/main.gem"
    gem_table_set(gem_v_node, gem_string("else"), gem_v_else_body);
    return gem_v_node;
}

static GemVal gem_fn_make_when(void *_env, GemVal *args, int argc) {
#line 116 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t26 = gem_table_new();
    gem_table_set(_t26, gem_string("tag"), gem_string("when_clause"));
    gem_table_set(_t26, gem_string("value"), gem_v_value);
    gem_table_set(_t26, gem_string("body"), gem_v_body);
    return _t26;
}

static GemVal gem_fn_make_return(void *_env, GemVal *args, int argc) {
#line 120 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_line = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t27 = gem_table_new();
    gem_table_set(_t27, gem_string("tag"), gem_string("return"));
    gem_table_set(_t27, gem_string("value"), gem_v_value);
    gem_table_set(_t27, gem_string("line"), gem_v_line);
    return _t27;
}

static GemVal gem_fn_make_break(void *_env, GemVal *args, int argc) {
#line 124 "compiler/main.gem"
    GemVal _t28 = gem_table_new();
    gem_table_set(_t28, gem_string("tag"), gem_string("break"));
    return _t28;
}

static GemVal gem_fn_make_continue(void *_env, GemVal *args, int argc) {
#line 128 "compiler/main.gem"
    GemVal _t29 = gem_table_new();
    gem_table_set(_t29, gem_string("tag"), gem_string("continue"));
    return _t29;
}

static GemVal gem_fn_make_load(void *_env, GemVal *args, int argc) {
#line 132 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t30 = gem_table_new();
    gem_table_set(_t30, gem_string("tag"), gem_string("load"));
    gem_table_set(_t30, gem_string("path"), gem_v_path);
    return _t30;
}

static GemVal gem_fn_make_extern_fn(void *_env, GemVal *args, int argc) {
#line 136 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t31 = gem_table_new();
    gem_table_set(_t31, gem_string("tag"), gem_string("extern_fn"));
    gem_table_set(_t31, gem_string("name"), gem_v_name);
    gem_table_set(_t31, gem_string("params"), gem_v_params);
    gem_table_set(_t31, gem_string("ret_type"), gem_v_ret_type);
    return _t31;
}

static GemVal gem_fn_make_extern_param(void *_env, GemVal *args, int argc) {
#line 140 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_type_name = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t32 = gem_table_new();
    gem_table_set(_t32, gem_string("name"), gem_v_name);
    gem_table_set(_t32, gem_string("type"), gem_v_type_name);
    return _t32;
}

static GemVal gem_fn_make_extern_include(void *_env, GemVal *args, int argc) {
#line 144 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t33 = gem_table_new();
    gem_table_set(_t33, gem_string("tag"), gem_string("extern_include"));
    gem_table_set(_t33, gem_string("path"), gem_v_path);
    return _t33;
}

static GemVal gem_fn_is_digit(void *_env, GemVal *args, int argc) {
#line 13 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t34;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("0")))) {
        _t34 = gem_ge(gem_v_ch, gem_string("0"));
    } else {
        _t34 = gem_le(gem_v_ch, gem_string("9"));
    }
    return _t34;
}

static GemVal gem_fn_is_alpha(void *_env, GemVal *args, int argc) {
#line 17 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t35;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("a")))) {
        _t35 = gem_ge(gem_v_ch, gem_string("a"));
    } else {
        _t35 = gem_le(gem_v_ch, gem_string("z"));
    }
    GemVal _t37;
    if (gem_truthy(_t35)) {
        _t37 = _t35;
    } else {
        GemVal _t36;
        if (!gem_truthy(gem_ge(gem_v_ch, gem_string("A")))) {
                _t36 = gem_ge(gem_v_ch, gem_string("A"));
        } else {
                _t36 = gem_le(gem_v_ch, gem_string("Z"));
        }
        _t37 = _t36;
    }
    GemVal _t38;
    if (gem_truthy(_t37)) {
        _t38 = _t37;
    } else {
        _t38 = gem_eq(gem_v_ch, gem_string("_"));
    }
    return _t38;
}

static GemVal gem_fn_is_alnum(void *_env, GemVal *args, int argc) {
#line 21 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t39[] = {gem_v_ch};
    GemVal _t41;
    if (gem_truthy(gem_fn_is_alpha(NULL, _t39, 1))) {
        _t41 = gem_fn_is_alpha(NULL, _t39, 1);
    } else {
        GemVal _t40[] = {gem_v_ch};
        _t41 = gem_fn_is_digit(NULL, _t40, 1);
    }
    return _t41;
}

static GemVal gem_fn_digit_val(void *_env, GemVal *args, int argc) {
#line 27 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
#line 28 "compiler/main.gem"
    GemVal _t42 = gem_v_ch;
    if (gem_truthy(gem_eq(_t42, gem_string("0")))) {
        return gem_int(0);
    } else if (gem_truthy(gem_eq(_t42, gem_string("1")))) {
        return gem_int(1);
    } else if (gem_truthy(gem_eq(_t42, gem_string("2")))) {
        return gem_int(2);
    } else if (gem_truthy(gem_eq(_t42, gem_string("3")))) {
        return gem_int(3);
    } else if (gem_truthy(gem_eq(_t42, gem_string("4")))) {
        return gem_int(4);
    } else if (gem_truthy(gem_eq(_t42, gem_string("5")))) {
        return gem_int(5);
    } else if (gem_truthy(gem_eq(_t42, gem_string("6")))) {
        return gem_int(6);
    } else if (gem_truthy(gem_eq(_t42, gem_string("7")))) {
        return gem_int(7);
    } else if (gem_truthy(gem_eq(_t42, gem_string("8")))) {
        return gem_int(8);
    } else if (gem_truthy(gem_eq(_t42, gem_string("9")))) {
        return gem_int(9);
    } else {
#line 50 "compiler/main.gem"
    GemVal _t43[] = {gem_v_ch};
    GemVal _t44[] = {gem_add(gem_string("not a digit: "), gem_to_string_fn(NULL, _t43, 1))};
        return gem_error_at_fn("compiler/main.gem", 50, _t44, 1);
    }
}

static GemVal gem_fn_str_to_int(void *_env, GemVal *args, int argc) {
#line 54 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
#line 55 "compiler/main.gem"
    GemVal gem_v_result = gem_int(0);
    {
#line 56 "compiler/main.gem"
        GemVal gem_v__for_i_1 = gem_int(0);
#line 56 "compiler/main.gem"
    GemVal _t45[] = {gem_v_s};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t45, 1);
#line 56 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 56 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_1;
#line 56 "compiler/main.gem"
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 57 "compiler/main.gem"
    GemVal _t46[] = {gem_table_get(gem_v_s, gem_v_i)};
            gem_v_result = gem_add(gem_mul(gem_v_result, gem_int(10)), gem_fn_digit_val(NULL, _t46, 1));
        }
    }
    return gem_v_result;
}

static GemVal gem_fn_contains_dot(void *_env, GemVal *args, int argc) {
#line 64 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    {
#line 65 "compiler/main.gem"
        GemVal gem_v__for_i_2 = gem_int(0);
#line 65 "compiler/main.gem"
    GemVal _t47[] = {gem_v_s};
        GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t47, 1);
#line 65 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 65 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_2;
#line 65 "compiler/main.gem"
            gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 66 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_string(".")))) {
#line 67 "compiler/main.gem"
                return gem_bool(1);
            }
        }
    }
    return gem_bool(0);
}

static GemVal gem_fn_build_keyword_set(void *_env, GemVal *args, int argc) {
#line 75 "compiler/main.gem"
#line 76 "compiler/main.gem"
    GemVal _t48 = gem_table_new();
    GemVal gem_v_t = _t48;
#line 77 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("fn"), gem_bool(1));
#line 78 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("end"), gem_bool(1));
#line 79 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("let"), gem_bool(1));
#line 80 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("if"), gem_bool(1));
#line 81 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("elif"), gem_bool(1));
#line 82 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("else"), gem_bool(1));
#line 83 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("while"), gem_bool(1));
#line 84 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("return"), gem_bool(1));
#line 85 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("break"), gem_bool(1));
#line 86 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("continue"), gem_bool(1));
#line 87 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("do"), gem_bool(1));
#line 88 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("match"), gem_bool(1));
#line 89 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("when"), gem_bool(1));
#line 90 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("and"), gem_bool(1));
#line 91 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("or"), gem_bool(1));
#line 92 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("not"), gem_bool(1));
#line 93 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("true"), gem_bool(1));
#line 94 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("false"), gem_bool(1));
#line 95 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("nil"), gem_bool(1));
#line 96 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("load"), gem_bool(1));
#line 97 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("extern"), gem_bool(1));
#line 98 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("for"), gem_bool(1));
#line 99 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("in"), gem_bool(1));
    return gem_v_t;
}

static GemVal gem_fn_tokenize(void *_env, GemVal *args, int argc) {
#line 103 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
#line 104 "compiler/main.gem"
    GemVal gem_v_kw = gem_fn_build_keyword_set(NULL, NULL, 0);
#line 105 "compiler/main.gem"
    GemVal _t49 = gem_table_new();
    GemVal gem_v_tokens = _t49;
#line 106 "compiler/main.gem"
    GemVal gem_v_pos = gem_int(0);
#line 107 "compiler/main.gem"
    GemVal gem_v_line = gem_int(1);
#line 108 "compiler/main.gem"
    GemVal _t50[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t50, 1);
#line 109 "compiler/main.gem"
    GemVal gem_v_interp_depth = gem_int(0);
#line 110 "compiler/main.gem"
    GemVal _t51 = gem_table_new();
    GemVal gem_v_interp_brace = _t51;
#line 112 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 113 "compiler/main.gem"
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
#line 116 "compiler/main.gem"
    GemVal _t52;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t52 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t52 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t52)) {
#line 117 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 122 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 123 "compiler/main.gem"
            while (1) {
                GemVal _t53;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t53 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t53 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t53)) break;
#line 124 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 130 "compiler/main.gem"
    GemVal _t54;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t54 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t54 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t54)) {
#line 131 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 132 "compiler/main.gem"
    GemVal _t55[] = {gem_v_tokens};
    GemVal _t58;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t55, 1), gem_int(0)))) {
        _t58 = gem_gt(gem_len_fn(NULL, _t55, 1), gem_int(0));
    } else {
        GemVal _t56[] = {gem_v_tokens};
        GemVal _t57 = gem_table_get(gem_v_tokens, gem_sub(gem_len_fn(NULL, _t56, 1), gem_int(1)));
        _t58 = gem_eq(gem_table_get(_t57, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t58)) {
#line 133 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 135 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 136 "compiler/main.gem"
    GemVal _t59 = gem_table_new();
    gem_table_set(_t59, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t59, gem_string("value"), gem_string("\n"));
    gem_table_set(_t59, gem_string("line"), gem_v_line);
    GemVal _t60[] = {gem_v_tokens, _t59};
                (void)(gem_push_fn(NULL, _t60, 2));
            }
#line 138 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 139 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
            }
#line 141 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 143 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 144 "compiler/main.gem"
                GemVal gem_v_c2 = gem_table_get(gem_v_source, gem_v_pos);
#line 145 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_c2, gem_string("\n")))) {
#line 146 "compiler/main.gem"
                    gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 147 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                } else {
#line 148 "compiler/main.gem"
    GemVal _t61;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t61 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t61 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t62;
    if (gem_truthy(_t61)) {
        _t62 = _t61;
    } else {
        _t62 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t62)) {
#line 149 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 150 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 151 "compiler/main.gem"
                            while (1) {
                                GemVal _t63;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t63 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t63 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t63)) break;
#line 152 "compiler/main.gem"
                                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                            }
                        } else {
                            break;
                        }
                    }
                }
            }
            continue;
        }
#line 162 "compiler/main.gem"
    GemVal _t64[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t64, 1))) {
#line 163 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 164 "compiler/main.gem"
            while (1) {
                GemVal _t66;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t66 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t65[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t66 = gem_fn_is_digit(NULL, _t65, 1);
                }
                if (!gem_truthy(_t66)) break;
#line 165 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 166 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 168 "compiler/main.gem"
    GemVal _t67;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t67 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t67 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t67;
#line 169 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 170 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 171 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 172 "compiler/main.gem"
                while (1) {
                    GemVal _t69;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t69 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t68[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t69 = gem_fn_is_digit(NULL, _t68, 1);
                    }
                    if (!gem_truthy(_t69)) break;
#line 173 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 174 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 177 "compiler/main.gem"
    GemVal _t70 = gem_table_new();
    gem_table_set(_t70, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t70, gem_string("value"), gem_v_val);
    gem_table_set(_t70, gem_string("line"), gem_v_line);
    GemVal _t71[] = {gem_v_tokens, _t70};
            (void)(gem_push_fn(NULL, _t71, 2));
            continue;
        }
#line 182 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 183 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 184 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 185 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 186 "compiler/main.gem"
            while (1) {
                GemVal _t72;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t72 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t72 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t72)) break;
#line 187 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 188 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 189 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 190 "compiler/main.gem"
    GemVal _t73[] = {gem_v_line};
    GemVal _t74[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t73, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 190, _t74, 1));
                    }
#line 192 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 193 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 194 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
#line 195 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 196 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                        } else {
#line 197 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 198 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                            } else {
#line 199 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
#line 200 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                } else {
#line 201 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
#line 202 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                    } else {
#line 203 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
#line 204 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                        } else {
#line 206 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                        }
                                    }
                                }
                            }
                        }
                    }
#line 208 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 211 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 212 "compiler/main.gem"
    GemVal _t75 = gem_table_new();
    gem_table_set(_t75, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t75, gem_string("value"), gem_string(""));
    gem_table_set(_t75, gem_string("line"), gem_v_line);
    GemVal _t76[] = {gem_v_tokens, _t75};
                    (void)(gem_push_fn(NULL, _t76, 2));
#line 213 "compiler/main.gem"
    GemVal _t77 = gem_table_new();
    gem_table_set(_t77, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t77, gem_string("value"), gem_v_val);
    gem_table_set(_t77, gem_string("line"), gem_v_line);
    GemVal _t78[] = {gem_v_tokens, _t77};
                    (void)(gem_push_fn(NULL, _t78, 2));
#line 214 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 215 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 216 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 217 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 218 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 221 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 222 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 224 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 225 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 226 "compiler/main.gem"
    GemVal _t79[] = {gem_v_line};
    GemVal _t80[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t79, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 226, _t80, 1));
                }
#line 228 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 229 "compiler/main.gem"
    GemVal _t81 = gem_table_new();
    gem_table_set(_t81, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t81, gem_string("value"), gem_v_val);
    gem_table_set(_t81, gem_string("line"), gem_v_line);
    GemVal _t82[] = {gem_v_tokens, _t81};
                (void)(gem_push_fn(NULL, _t82, 2));
            }
            continue;
        }
#line 235 "compiler/main.gem"
    GemVal _t83[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t83, 1))) {
#line 236 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 237 "compiler/main.gem"
            while (1) {
                GemVal _t85;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t85 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t84[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t85 = gem_fn_is_alnum(NULL, _t84, 1);
                }
                if (!gem_truthy(_t85)) break;
#line 238 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 239 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 241 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_kw, gem_v_val), gem_bool(1)))) {
#line 242 "compiler/main.gem"
    GemVal _t86 = gem_table_new();
    gem_table_set(_t86, gem_string("type"), gem_v_val);
    gem_table_set(_t86, gem_string("value"), gem_v_val);
    gem_table_set(_t86, gem_string("line"), gem_v_line);
    GemVal _t87[] = {gem_v_tokens, _t86};
                (void)(gem_push_fn(NULL, _t87, 2));
            } else {
#line 244 "compiler/main.gem"
    GemVal _t88 = gem_table_new();
    gem_table_set(_t88, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t88, gem_string("value"), gem_v_val);
    gem_table_set(_t88, gem_string("line"), gem_v_line);
    GemVal _t89[] = {gem_v_tokens, _t88};
                (void)(gem_push_fn(NULL, _t89, 2));
            }
            continue;
        }
#line 250 "compiler/main.gem"
        GemVal gem_v_next = gem_string("");
#line 251 "compiler/main.gem"
        if (gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
#line 252 "compiler/main.gem"
            gem_v_next = gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1)));
        }
#line 255 "compiler/main.gem"
    GemVal _t90;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t90 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t90 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t90)) {
#line 256 "compiler/main.gem"
    GemVal _t91 = gem_table_new();
    gem_table_set(_t91, gem_string("type"), gem_string("=="));
    gem_table_set(_t91, gem_string("value"), gem_string("=="));
    gem_table_set(_t91, gem_string("line"), gem_v_line);
    GemVal _t92[] = {gem_v_tokens, _t91};
            (void)(gem_push_fn(NULL, _t92, 2));
#line 257 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 260 "compiler/main.gem"
    GemVal _t93;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t93 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t93 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t93)) {
#line 261 "compiler/main.gem"
    GemVal _t94 = gem_table_new();
    gem_table_set(_t94, gem_string("type"), gem_string("!="));
    gem_table_set(_t94, gem_string("value"), gem_string("!="));
    gem_table_set(_t94, gem_string("line"), gem_v_line);
    GemVal _t95[] = {gem_v_tokens, _t94};
            (void)(gem_push_fn(NULL, _t95, 2));
#line 262 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 265 "compiler/main.gem"
    GemVal _t96;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t96 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t96 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t96)) {
#line 266 "compiler/main.gem"
    GemVal _t97 = gem_table_new();
    gem_table_set(_t97, gem_string("type"), gem_string("<="));
    gem_table_set(_t97, gem_string("value"), gem_string("<="));
    gem_table_set(_t97, gem_string("line"), gem_v_line);
    GemVal _t98[] = {gem_v_tokens, _t97};
            (void)(gem_push_fn(NULL, _t98, 2));
#line 267 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 270 "compiler/main.gem"
    GemVal _t99;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t99 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t99 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t99)) {
#line 271 "compiler/main.gem"
    GemVal _t100 = gem_table_new();
    gem_table_set(_t100, gem_string("type"), gem_string(">="));
    gem_table_set(_t100, gem_string("value"), gem_string(">="));
    gem_table_set(_t100, gem_string("line"), gem_v_line);
    GemVal _t101[] = {gem_v_tokens, _t100};
            (void)(gem_push_fn(NULL, _t101, 2));
#line 272 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 275 "compiler/main.gem"
    GemVal _t102;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t102 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t102 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t102)) {
#line 276 "compiler/main.gem"
    GemVal _t103 = gem_table_new();
    gem_table_set(_t103, gem_string("type"), gem_string("+="));
    gem_table_set(_t103, gem_string("value"), gem_string("+="));
    gem_table_set(_t103, gem_string("line"), gem_v_line);
    GemVal _t104[] = {gem_v_tokens, _t103};
            (void)(gem_push_fn(NULL, _t104, 2));
#line 277 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 280 "compiler/main.gem"
    GemVal _t105;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t105 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t105 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t105)) {
#line 281 "compiler/main.gem"
    GemVal _t106 = gem_table_new();
    gem_table_set(_t106, gem_string("type"), gem_string("-="));
    gem_table_set(_t106, gem_string("value"), gem_string("-="));
    gem_table_set(_t106, gem_string("line"), gem_v_line);
    GemVal _t107[] = {gem_v_tokens, _t106};
            (void)(gem_push_fn(NULL, _t107, 2));
#line 282 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 285 "compiler/main.gem"
    GemVal _t108;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t108 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t108 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t108)) {
#line 286 "compiler/main.gem"
    GemVal _t109 = gem_table_new();
    gem_table_set(_t109, gem_string("type"), gem_string("*="));
    gem_table_set(_t109, gem_string("value"), gem_string("*="));
    gem_table_set(_t109, gem_string("line"), gem_v_line);
    GemVal _t110[] = {gem_v_tokens, _t109};
            (void)(gem_push_fn(NULL, _t110, 2));
#line 287 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 290 "compiler/main.gem"
    GemVal _t111;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t111 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t111 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t111)) {
#line 291 "compiler/main.gem"
    GemVal _t112 = gem_table_new();
    gem_table_set(_t112, gem_string("type"), gem_string("/="));
    gem_table_set(_t112, gem_string("value"), gem_string("/="));
    gem_table_set(_t112, gem_string("line"), gem_v_line);
    GemVal _t113[] = {gem_v_tokens, _t112};
            (void)(gem_push_fn(NULL, _t113, 2));
#line 292 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 295 "compiler/main.gem"
    GemVal _t114;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t114 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t114 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t114)) {
#line 296 "compiler/main.gem"
    GemVal _t115 = gem_table_new();
    gem_table_set(_t115, gem_string("type"), gem_string("->"));
    gem_table_set(_t115, gem_string("value"), gem_string("->"));
    gem_table_set(_t115, gem_string("line"), gem_v_line);
    GemVal _t116[] = {gem_v_tokens, _t115};
            (void)(gem_push_fn(NULL, _t116, 2));
#line 297 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 302 "compiler/main.gem"
    GemVal _t117;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t117 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t117 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t118;
    if (gem_truthy(_t117)) {
        _t118 = _t117;
    } else {
        _t118 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t119;
    if (gem_truthy(_t118)) {
        _t119 = _t118;
    } else {
        _t119 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t120;
    if (gem_truthy(_t119)) {
        _t120 = _t119;
    } else {
        _t120 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t120)) {
#line 303 "compiler/main.gem"
    GemVal _t121 = gem_table_new();
    gem_table_set(_t121, gem_string("type"), gem_v_ch);
    gem_table_set(_t121, gem_string("value"), gem_v_ch);
    gem_table_set(_t121, gem_string("line"), gem_v_line);
    GemVal _t122[] = {gem_v_tokens, _t121};
            (void)(gem_push_fn(NULL, _t122, 2));
#line 304 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 307 "compiler/main.gem"
    GemVal _t123;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t123 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t123 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t124;
    if (gem_truthy(_t123)) {
        _t124 = _t123;
    } else {
        _t124 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t124)) {
#line 308 "compiler/main.gem"
    GemVal _t125 = gem_table_new();
    gem_table_set(_t125, gem_string("type"), gem_v_ch);
    gem_table_set(_t125, gem_string("value"), gem_v_ch);
    gem_table_set(_t125, gem_string("line"), gem_v_line);
    GemVal _t126[] = {gem_v_tokens, _t125};
            (void)(gem_push_fn(NULL, _t126, 2));
#line 309 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 313 "compiler/main.gem"
    GemVal _t127;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t127 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t127 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t127)) {
#line 314 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 315 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 316 "compiler/main.gem"
    GemVal _t128 = gem_table_new();
    gem_table_set(_t128, gem_string("type"), gem_string("{"));
    gem_table_set(_t128, gem_string("value"), gem_string("{"));
    gem_table_set(_t128, gem_string("line"), gem_v_line);
    GemVal _t129[] = {gem_v_tokens, _t128};
            (void)(gem_push_fn(NULL, _t129, 2));
#line 317 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 320 "compiler/main.gem"
    GemVal _t130;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t130 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t130 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t130)) {
#line 321 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 322 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 323 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 324 "compiler/main.gem"
    GemVal _t131 = gem_table_new();
    gem_table_set(_t131, gem_string("type"), gem_string("}"));
    gem_table_set(_t131, gem_string("value"), gem_string("}"));
    gem_table_set(_t131, gem_string("line"), gem_v_line);
    GemVal _t132[] = {gem_v_tokens, _t131};
                (void)(gem_push_fn(NULL, _t132, 2));
#line 325 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                continue;
            }
#line 329 "compiler/main.gem"
            gem_v_interp_depth = gem_sub(gem_v_interp_depth, gem_int(1));
#line 330 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 331 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 332 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 333 "compiler/main.gem"
            while (1) {
                GemVal _t133;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t133 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t133 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t133)) break;
#line 334 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 335 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 336 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 337 "compiler/main.gem"
    GemVal _t134[] = {gem_v_line};
    GemVal _t135[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t134, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 337, _t135, 1));
                    }
#line 339 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 340 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 341 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
#line 342 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 343 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                        } else {
#line 344 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 345 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                            } else {
#line 346 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
#line 347 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                } else {
#line 348 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
#line 349 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                    } else {
#line 350 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
#line 351 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                        } else {
#line 353 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                        }
                                    }
                                }
                            }
                        }
                    }
#line 355 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 358 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 360 "compiler/main.gem"
    GemVal _t136 = gem_table_new();
    gem_table_set(_t136, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t136, gem_string("value"), gem_v_val);
    gem_table_set(_t136, gem_string("line"), gem_v_line);
    GemVal _t137[] = {gem_v_tokens, _t136};
                    (void)(gem_push_fn(NULL, _t137, 2));
#line 361 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 362 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 363 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 364 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 365 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 368 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 369 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 371 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 372 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 373 "compiler/main.gem"
    GemVal _t138[] = {gem_v_line};
    GemVal _t139[] = {gem_add(gem_string("unterminated string interpolation at line "), gem_to_string_fn(NULL, _t138, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 373, _t139, 1));
                }
#line 375 "compiler/main.gem"
    GemVal _t140 = gem_table_new();
    gem_table_set(_t140, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t140, gem_string("value"), gem_v_val);
    gem_table_set(_t140, gem_string("line"), gem_v_line);
    GemVal _t141[] = {gem_v_tokens, _t140};
                (void)(gem_push_fn(NULL, _t141, 2));
#line 376 "compiler/main.gem"
    GemVal _t142 = gem_table_new();
    gem_table_set(_t142, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t142, gem_string("value"), gem_string(""));
    gem_table_set(_t142, gem_string("line"), gem_v_line);
    GemVal _t143[] = {gem_v_tokens, _t142};
                (void)(gem_push_fn(NULL, _t143, 2));
#line 377 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 382 "compiler/main.gem"
    GemVal _t144;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t144 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t144 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t145;
    if (gem_truthy(_t144)) {
        _t145 = _t144;
    } else {
        _t145 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t146;
    if (gem_truthy(_t145)) {
        _t146 = _t145;
    } else {
        _t146 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t147;
    if (gem_truthy(_t146)) {
        _t147 = _t146;
    } else {
        _t147 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t148;
    if (gem_truthy(_t147)) {
        _t148 = _t147;
    } else {
        _t148 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t148)) {
#line 383 "compiler/main.gem"
    GemVal _t149 = gem_table_new();
    gem_table_set(_t149, gem_string("type"), gem_v_ch);
    gem_table_set(_t149, gem_string("value"), gem_v_ch);
    gem_table_set(_t149, gem_string("line"), gem_v_line);
    GemVal _t150[] = {gem_v_tokens, _t149};
            (void)(gem_push_fn(NULL, _t150, 2));
#line 384 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 387 "compiler/main.gem"
    GemVal _t151;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t151 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t151 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t152;
    if (gem_truthy(_t151)) {
        _t152 = _t151;
    } else {
        _t152 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t153;
    if (gem_truthy(_t152)) {
        _t153 = _t152;
    } else {
        _t153 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t153)) {
#line 388 "compiler/main.gem"
    GemVal _t154 = gem_table_new();
    gem_table_set(_t154, gem_string("type"), gem_v_ch);
    gem_table_set(_t154, gem_string("value"), gem_v_ch);
    gem_table_set(_t154, gem_string("line"), gem_v_line);
    GemVal _t155[] = {gem_v_tokens, _t154};
            (void)(gem_push_fn(NULL, _t155, 2));
#line 389 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 393 "compiler/main.gem"
    GemVal _t156[] = {gem_v_ch};
    GemVal _t157[] = {gem_v_line};
    GemVal _t158[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t156, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t157, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 393, _t158, 1));
    }
#line 396 "compiler/main.gem"
    GemVal _t159 = gem_table_new();
    gem_table_set(_t159, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t159, gem_string("value"), gem_string(""));
    gem_table_set(_t159, gem_string("line"), gem_v_line);
    GemVal _t160[] = {gem_v_tokens, _t159};
    (void)(gem_push_fn(NULL, _t160, 2));
    return gem_v_tokens;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 10 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
#line 11 "compiler/main.gem"
    GemVal _t161[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t161, 1);
#line 12 "compiler/main.gem"
    GemVal _t162[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t162, 1);
#line 13 "compiler/main.gem"
    GemVal _t163 = gem_v_p;
    GemVal _t164 = gem_table_get(_t163, gem_string("parse"));
    return _t164.fn(_t164.env, NULL, 0);
}

struct _closure__anon_1 {
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_1(void *_env, GemVal *args, int argc) {
    struct _closure__anon_1 *_cls = (struct _closure__anon_1 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    return gem_table_get((*gem_v_tokens), (*gem_v_pos));
}

struct _closure__anon_2 {
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_2(void *_env, GemVal *args, int argc) {
    struct _closure__anon_2 *_cls = (struct _closure__anon_2 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    GemVal gem_v_n = (argc > 0) ? args[0] : GEM_NIL;
    return gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
}

struct _closure__anon_3 {
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_3(void *_env, GemVal *args, int argc) {
    struct _closure__anon_3 *_cls = (struct _closure__anon_3 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
#line 33 "compiler/main.gem"
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
#line 34 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    return gem_v_t;
}

struct _closure__anon_4 {
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_4(void *_env, GemVal *args, int argc) {
    struct _closure__anon_4 *_cls = (struct _closure__anon_4 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    GemVal _t168 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    return gem_eq(gem_table_get(_t168, gem_string("type")), gem_string("EOF"));
}

struct _closure__anon_5 {
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_5(void *_env, GemVal *args, int argc) {
    struct _closure__anon_5 *_cls = (struct _closure__anon_5 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    GemVal gem_v_tp = (argc > 0) ? args[0] : GEM_NIL;
#line 43 "compiler/main.gem"
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
#line 44 "compiler/main.gem"
    GemVal _t170 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t170, gem_string("type")), gem_v_tp))) {
#line 45 "compiler/main.gem"
    GemVal _t171[] = {gem_v_tp};
    GemVal _t172 = gem_v_t;
    GemVal _t173[] = {gem_table_get(_t172, gem_string("type"))};
    GemVal _t174 = gem_v_t;
    GemVal _t175[] = {gem_table_get(_t174, gem_string("line"))};
    GemVal _t176[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t171, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t173, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t175, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 45, _t176, 1));
    }
#line 47 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    return gem_v_t;
}

struct _closure__anon_6 {
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_6(void *_env, GemVal *args, int argc) {
    struct _closure__anon_6 *_cls = (struct _closure__anon_6 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
#line 52 "compiler/main.gem"
    while (1) {
        GemVal _t178 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t178, gem_string("type")), gem_string("NEWLINE")))) break;
#line 53 "compiler/main.gem"
        *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    }
    return GEM_NIL;
}

struct _closure__anon_7 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_peek;
};
static GemVal _anon_7(void *_env, GemVal *args, int argc) {
    struct _closure__anon_7 *_cls = (struct _closure__anon_7 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_peek = _cls->gem_v_peek;
#line 60 "compiler/main.gem"
    GemVal _t180 = gem_table_new();
    GemVal gem_v_params = _t180;
#line 61 "compiler/main.gem"
    GemVal _t181 = (*gem_v_peek);
    GemVal _t182 = _t181.fn(_t181.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t182, gem_string("type")), gem_string("NAME")))) {
#line 62 "compiler/main.gem"
    GemVal _t183 = (*gem_v_advance);
    GemVal _t184 = _t183.fn(_t183.env, NULL, 0);
    GemVal _t185[] = {gem_v_params, gem_table_get(_t184, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t185, 2));
#line 63 "compiler/main.gem"
        while (1) {
            GemVal _t186 = (*gem_v_peek);
            GemVal _t187 = _t186.fn(_t186.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t187, gem_string("type")), gem_string(",")))) break;
#line 64 "compiler/main.gem"
    GemVal _t188 = (*gem_v_advance);
            (void)(_t188.fn(_t188.env, NULL, 0));
#line 65 "compiler/main.gem"
    GemVal _t189 = (*gem_v_expect);
    GemVal _t190[] = {gem_string("NAME")};
    GemVal _t191 = _t189.fn(_t189.env, _t190, 1);
    GemVal _t192[] = {gem_v_params, gem_table_get(_t191, gem_string("value"))};
            (void)(gem_push_fn(NULL, _t192, 2));
        }
    }
    return gem_v_params;
}

struct _closure__anon_8 {
    GemVal *gem_v_at_end;
    GemVal *gem_v_parse_stmt;
    GemVal *gem_v_peek;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_8(void *_env, GemVal *args, int argc) {
    struct _closure__anon_8 *_cls = (struct _closure__anon_8 *)_env;
    GemVal *gem_v_at_end = _cls->gem_v_at_end;
    GemVal *gem_v_parse_stmt = _cls->gem_v_parse_stmt;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
#line 83 "compiler/main.gem"
    GemVal _t194 = gem_table_new();
    GemVal gem_v_stmts = _t194;
#line 84 "compiler/main.gem"
    GemVal _t195 = (*gem_v_skip_nl);
    (void)(_t195.fn(_t195.env, NULL, 0));
#line 85 "compiler/main.gem"
    while (1) {
        GemVal _t196 = (*gem_v_peek);
        GemVal _t197 = _t196.fn(_t196.env, NULL, 0);
        GemVal _t200;
        if (!gem_truthy(gem_neq(gem_table_get(_t197, gem_string("type")), gem_string("end")))) {
                _t200 = gem_neq(gem_table_get(_t197, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t198 = (*gem_v_peek);
                GemVal _t199 = _t198.fn(_t198.env, NULL, 0);
                _t200 = gem_neq(gem_table_get(_t199, gem_string("type")), gem_string("elif"));
        }
        GemVal _t203;
        if (!gem_truthy(_t200)) {
                _t203 = _t200;
        } else {
                GemVal _t201 = (*gem_v_peek);
                GemVal _t202 = _t201.fn(_t201.env, NULL, 0);
                _t203 = gem_neq(gem_table_get(_t202, gem_string("type")), gem_string("else"));
        }
        GemVal _t206;
        if (!gem_truthy(_t203)) {
                _t206 = _t203;
        } else {
                GemVal _t204 = (*gem_v_peek);
                GemVal _t205 = _t204.fn(_t204.env, NULL, 0);
                _t206 = gem_neq(gem_table_get(_t205, gem_string("type")), gem_string("when"));
        }
        GemVal _t208;
        if (!gem_truthy(_t206)) {
                _t208 = _t206;
        } else {
                GemVal _t207 = (*gem_v_at_end);
                _t208 = gem_not(_t207.fn(_t207.env, NULL, 0));
        }
        if (!gem_truthy(_t208)) break;
#line 86 "compiler/main.gem"
    GemVal _t209 = (*gem_v_parse_stmt);
    GemVal _t210[] = {gem_v_stmts, _t209.fn(_t209.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t210, 2));
#line 87 "compiler/main.gem"
    GemVal _t211 = (*gem_v_skip_nl);
        (void)(_t211.fn(_t211.env, NULL, 0));
    }
    return gem_v_stmts;
}

struct _closure__anon_9 {
    GemVal *gem_v_at_end;
    GemVal *gem_v_parse_stmt;
    GemVal *gem_v_peek;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_9(void *_env, GemVal *args, int argc) {
    struct _closure__anon_9 *_cls = (struct _closure__anon_9 *)_env;
    GemVal *gem_v_at_end = _cls->gem_v_at_end;
    GemVal *gem_v_parse_stmt = _cls->gem_v_parse_stmt;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
#line 96 "compiler/main.gem"
    GemVal _t213 = gem_table_new();
    GemVal gem_v_stmts = _t213;
#line 97 "compiler/main.gem"
    GemVal _t214 = (*gem_v_skip_nl);
    (void)(_t214.fn(_t214.env, NULL, 0));
#line 98 "compiler/main.gem"
    while (1) {
        GemVal _t215 = (*gem_v_peek);
        GemVal _t216 = _t215.fn(_t215.env, NULL, 0);
        GemVal _t218;
        if (!gem_truthy(gem_neq(gem_table_get(_t216, gem_string("type")), gem_string("end")))) {
                _t218 = gem_neq(gem_table_get(_t216, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t217 = (*gem_v_at_end);
                _t218 = gem_not(_t217.fn(_t217.env, NULL, 0));
        }
        if (!gem_truthy(_t218)) break;
#line 99 "compiler/main.gem"
    GemVal _t219 = (*gem_v_parse_stmt);
    GemVal _t220[] = {gem_v_stmts, _t219.fn(_t219.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t220, 2));
#line 100 "compiler/main.gem"
    GemVal _t221 = (*gem_v_skip_nl);
        (void)(_t221.fn(_t221.env, NULL, 0));
    }
    return gem_v_stmts;
}

struct _closure__anon_10 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body;
    GemVal *gem_v_parse_params;
    GemVal *gem_v_peek;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_10(void *_env, GemVal *args, int argc) {
    struct _closure__anon_10 *_cls = (struct _closure__anon_10 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body = _cls->gem_v_parse_fn_body;
    GemVal *gem_v_parse_params = _cls->gem_v_parse_params;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
#line 109 "compiler/main.gem"
    GemVal _t223 = (*gem_v_peek);
    GemVal gem_v_t = _t223.fn(_t223.env, NULL, 0);
#line 112 "compiler/main.gem"
    GemVal _t224 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t224, gem_string("type")), gem_string("NUMBER")))) {
#line 113 "compiler/main.gem"
    GemVal _t225 = (*gem_v_advance);
        (void)(_t225.fn(_t225.env, NULL, 0));
#line 114 "compiler/main.gem"
    GemVal _t226 = gem_v_t;
    GemVal _t227[] = {gem_table_get(_t226, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t227, 1))) {
#line 115 "compiler/main.gem"
    GemVal _t228 = gem_v_t;
    GemVal _t229[] = {gem_table_get(_t228, gem_string("value"))};
    GemVal _t230[] = {gem_fn_atof(NULL, _t229, 1)};
            return gem_fn_make_float(NULL, _t230, 1);
        }
#line 117 "compiler/main.gem"
    GemVal _t231 = gem_v_t;
    GemVal _t232[] = {gem_table_get(_t231, gem_string("value"))};
    GemVal _t233[] = {gem_fn_str_to_int(NULL, _t232, 1)};
        return gem_fn_make_int(NULL, _t233, 1);
    }
#line 121 "compiler/main.gem"
    GemVal _t234 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t234, gem_string("type")), gem_string("STRING")))) {
#line 122 "compiler/main.gem"
    GemVal _t235 = (*gem_v_advance);
        (void)(_t235.fn(_t235.env, NULL, 0));
#line 123 "compiler/main.gem"
    GemVal _t236 = gem_v_t;
    GemVal _t237[] = {gem_table_get(_t236, gem_string("value"))};
        return gem_fn_make_string(NULL, _t237, 1);
    }
#line 127 "compiler/main.gem"
    GemVal _t238 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t238, gem_string("type")), gem_string("INTERP_START")))) {
#line 128 "compiler/main.gem"
    GemVal _t239 = (*gem_v_advance);
        (void)(_t239.fn(_t239.env, NULL, 0));
#line 129 "compiler/main.gem"
    GemVal _t240 = gem_table_new();
        GemVal gem_v_parts = _t240;
#line 130 "compiler/main.gem"
    GemVal _t241 = (*gem_v_skip_nl);
        (void)(_t241.fn(_t241.env, NULL, 0));
#line 131 "compiler/main.gem"
        while (1) {
            GemVal _t242 = (*gem_v_peek);
            GemVal _t243 = _t242.fn(_t242.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t243, gem_string("type")), gem_string("INTERP_END")))) break;
#line 132 "compiler/main.gem"
    GemVal _t244 = (*gem_v_peek);
    GemVal _t245 = _t244.fn(_t244.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t245, gem_string("type")), gem_string("STRING")))) {
#line 133 "compiler/main.gem"
    GemVal _t246 = (*gem_v_advance);
    GemVal _t247 = _t246.fn(_t246.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t247, gem_string("value"));
#line 134 "compiler/main.gem"
    GemVal _t248[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t248, 1), gem_int(0)))) {
#line 135 "compiler/main.gem"
    GemVal _t249[] = {gem_v_sval};
    GemVal _t250[] = {gem_v_parts, gem_fn_make_string(NULL, _t249, 1)};
                    (void)(gem_push_fn(NULL, _t250, 2));
                }
            } else {
#line 138 "compiler/main.gem"
    GemVal _t251 = (*gem_v_parse_expr);
    GemVal _t252[] = {gem_v_parts, _t251.fn(_t251.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t252, 2));
            }
#line 140 "compiler/main.gem"
    GemVal _t253 = (*gem_v_skip_nl);
            (void)(_t253.fn(_t253.env, NULL, 0));
        }
#line 142 "compiler/main.gem"
    GemVal _t254 = (*gem_v_expect);
    GemVal _t255[] = {gem_string("INTERP_END")};
        (void)(_t254.fn(_t254.env, _t255, 1));
#line 143 "compiler/main.gem"
    GemVal _t256[] = {gem_v_parts};
        return gem_fn_make_interp(NULL, _t256, 1);
    }
#line 147 "compiler/main.gem"
    GemVal _t257 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t257, gem_string("type")), gem_string("true")))) {
#line 148 "compiler/main.gem"
    GemVal _t258 = (*gem_v_advance);
        (void)(_t258.fn(_t258.env, NULL, 0));
#line 149 "compiler/main.gem"
    GemVal _t259[] = {gem_bool(1)};
        return gem_fn_make_bool(NULL, _t259, 1);
    }
#line 151 "compiler/main.gem"
    GemVal _t260 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t260, gem_string("type")), gem_string("false")))) {
#line 152 "compiler/main.gem"
    GemVal _t261 = (*gem_v_advance);
        (void)(_t261.fn(_t261.env, NULL, 0));
#line 153 "compiler/main.gem"
    GemVal _t262[] = {gem_bool(0)};
        return gem_fn_make_bool(NULL, _t262, 1);
    }
#line 157 "compiler/main.gem"
    GemVal _t263 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t263, gem_string("type")), gem_string("nil")))) {
#line 158 "compiler/main.gem"
    GemVal _t264 = (*gem_v_advance);
        (void)(_t264.fn(_t264.env, NULL, 0));
#line 159 "compiler/main.gem"
        return gem_fn_make_nil_node(NULL, NULL, 0);
    }
#line 163 "compiler/main.gem"
    GemVal _t265 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t265, gem_string("type")), gem_string("fn")))) {
#line 164 "compiler/main.gem"
    GemVal _t266 = (*gem_v_advance);
        (void)(_t266.fn(_t266.env, NULL, 0));
#line 165 "compiler/main.gem"
    GemVal _t267 = (*gem_v_expect);
    GemVal _t268[] = {gem_string("(")};
        (void)(_t267.fn(_t267.env, _t268, 1));
#line 166 "compiler/main.gem"
    GemVal _t269 = (*gem_v_parse_params);
        GemVal gem_v_params = _t269.fn(_t269.env, NULL, 0);
#line 167 "compiler/main.gem"
    GemVal _t270 = (*gem_v_expect);
    GemVal _t271[] = {gem_string(")")};
        (void)(_t270.fn(_t270.env, _t271, 1));
#line 168 "compiler/main.gem"
    GemVal _t272 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t272.fn(_t272.env, NULL, 0);
#line 169 "compiler/main.gem"
    GemVal _t273 = (*gem_v_expect);
    GemVal _t274[] = {gem_string("end")};
        (void)(_t273.fn(_t273.env, _t274, 1));
#line 170 "compiler/main.gem"
    GemVal _t275[] = {gem_v_params, gem_v_body};
        return gem_fn_make_anon_fn(NULL, _t275, 2);
    }
#line 174 "compiler/main.gem"
    GemVal _t276 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t276, gem_string("type")), gem_string("{")))) {
#line 175 "compiler/main.gem"
    GemVal _t277 = (*gem_v_advance);
        (void)(_t277.fn(_t277.env, NULL, 0));
#line 176 "compiler/main.gem"
    GemVal _t278 = (*gem_v_skip_nl);
        (void)(_t278.fn(_t278.env, NULL, 0));
#line 177 "compiler/main.gem"
    GemVal _t279 = gem_table_new();
        GemVal gem_v_entries = _t279;
#line 178 "compiler/main.gem"
        while (1) {
            GemVal _t280 = (*gem_v_peek);
            GemVal _t281 = _t280.fn(_t280.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t281, gem_string("type")), gem_string("}")))) break;
#line 179 "compiler/main.gem"
    GemVal _t282 = (*gem_v_expect);
    GemVal _t283[] = {gem_string("NAME")};
    GemVal _t284 = _t282.fn(_t282.env, _t283, 1);
            GemVal gem_v_key = gem_table_get(_t284, gem_string("value"));
#line 180 "compiler/main.gem"
    GemVal _t285 = (*gem_v_expect);
    GemVal _t286[] = {gem_string(":")};
            (void)(_t285.fn(_t285.env, _t286, 1));
#line 181 "compiler/main.gem"
    GemVal _t287 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t287.fn(_t287.env, NULL, 0);
#line 182 "compiler/main.gem"
    GemVal _t288[] = {gem_v_key, gem_v_val};
    GemVal _t289[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t288, 2)};
            (void)(gem_push_fn(NULL, _t289, 2));
#line 183 "compiler/main.gem"
    GemVal _t290 = (*gem_v_skip_nl);
            (void)(_t290.fn(_t290.env, NULL, 0));
#line 184 "compiler/main.gem"
    GemVal _t291 = (*gem_v_peek);
    GemVal _t292 = _t291.fn(_t291.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t292, gem_string("type")), gem_string(",")))) {
#line 185 "compiler/main.gem"
    GemVal _t293 = (*gem_v_advance);
                (void)(_t293.fn(_t293.env, NULL, 0));
#line 186 "compiler/main.gem"
    GemVal _t294 = (*gem_v_skip_nl);
                (void)(_t294.fn(_t294.env, NULL, 0));
            }
        }
#line 189 "compiler/main.gem"
    GemVal _t295 = (*gem_v_expect);
    GemVal _t296[] = {gem_string("}")};
        (void)(_t295.fn(_t295.env, _t296, 1));
#line 190 "compiler/main.gem"
    GemVal _t297[] = {gem_v_entries};
        return gem_fn_make_table(NULL, _t297, 1);
    }
#line 194 "compiler/main.gem"
    GemVal _t298 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t298, gem_string("type")), gem_string("[")))) {
#line 195 "compiler/main.gem"
    GemVal _t299 = (*gem_v_advance);
        (void)(_t299.fn(_t299.env, NULL, 0));
#line 196 "compiler/main.gem"
    GemVal _t300 = (*gem_v_skip_nl);
        (void)(_t300.fn(_t300.env, NULL, 0));
#line 197 "compiler/main.gem"
    GemVal _t301 = gem_table_new();
        GemVal gem_v_elements = _t301;
#line 198 "compiler/main.gem"
        while (1) {
            GemVal _t302 = (*gem_v_peek);
            GemVal _t303 = _t302.fn(_t302.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t303, gem_string("type")), gem_string("]")))) break;
#line 199 "compiler/main.gem"
    GemVal _t304 = (*gem_v_parse_expr);
    GemVal _t305[] = {gem_v_elements, _t304.fn(_t304.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t305, 2));
#line 200 "compiler/main.gem"
    GemVal _t306 = (*gem_v_skip_nl);
            (void)(_t306.fn(_t306.env, NULL, 0));
#line 201 "compiler/main.gem"
    GemVal _t307 = (*gem_v_peek);
    GemVal _t308 = _t307.fn(_t307.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t308, gem_string("type")), gem_string(",")))) {
#line 202 "compiler/main.gem"
    GemVal _t309 = (*gem_v_advance);
                (void)(_t309.fn(_t309.env, NULL, 0));
#line 203 "compiler/main.gem"
    GemVal _t310 = (*gem_v_skip_nl);
                (void)(_t310.fn(_t310.env, NULL, 0));
            }
        }
#line 206 "compiler/main.gem"
    GemVal _t311 = (*gem_v_expect);
    GemVal _t312[] = {gem_string("]")};
        (void)(_t311.fn(_t311.env, _t312, 1));
#line 207 "compiler/main.gem"
    GemVal _t313[] = {gem_v_elements};
        return gem_fn_make_array(NULL, _t313, 1);
    }
#line 211 "compiler/main.gem"
    GemVal _t314 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t314, gem_string("type")), gem_string("(")))) {
#line 212 "compiler/main.gem"
    GemVal _t315 = (*gem_v_advance);
        (void)(_t315.fn(_t315.env, NULL, 0));
#line 213 "compiler/main.gem"
    GemVal _t316 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t316.fn(_t316.env, NULL, 0);
#line 214 "compiler/main.gem"
    GemVal _t317 = (*gem_v_expect);
    GemVal _t318[] = {gem_string(")")};
        (void)(_t317.fn(_t317.env, _t318, 1));
#line 215 "compiler/main.gem"
        return gem_v_e;
    }
#line 219 "compiler/main.gem"
    GemVal _t319 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t319, gem_string("type")), gem_string("NAME")))) {
#line 220 "compiler/main.gem"
    GemVal _t320 = (*gem_v_advance);
        (void)(_t320.fn(_t320.env, NULL, 0));
#line 221 "compiler/main.gem"
    GemVal _t321 = gem_v_t;
    GemVal _t322[] = {gem_table_get(_t321, gem_string("value"))};
        return gem_fn_make_var(NULL, _t322, 1);
    }
#line 224 "compiler/main.gem"
    GemVal _t323 = gem_v_t;
    GemVal _t324[] = {gem_table_get(_t323, gem_string("type"))};
    GemVal _t325 = gem_v_t;
    GemVal _t326[] = {gem_table_get(_t325, gem_string("line"))};
    GemVal _t327[] = {gem_add(gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t324, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t326, 1))};
    return gem_error_at_fn("compiler/main.gem", 224, _t327, 1);
}

struct _closure__anon_11 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_parse_atom;
    GemVal *gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body;
    GemVal *gem_v_parse_params;
    GemVal *gem_v_peek;
    GemVal *gem_v_peek_at;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_11(void *_env, GemVal *args, int argc) {
    struct _closure__anon_11 *_cls = (struct _closure__anon_11 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_parse_atom = _cls->gem_v_parse_atom;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body = _cls->gem_v_parse_fn_body;
    GemVal *gem_v_parse_params = _cls->gem_v_parse_params;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_peek_at = _cls->gem_v_peek_at;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
#line 229 "compiler/main.gem"
    GemVal _t329 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t329.fn(_t329.env, NULL, 0);
#line 231 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
#line 233 "compiler/main.gem"
    GemVal _t330 = (*gem_v_peek);
    GemVal _t331 = _t330.fn(_t330.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t331, gem_string("type")), gem_string("(")))) {
#line 234 "compiler/main.gem"
    GemVal _t332 = (*gem_v_peek);
    GemVal _t333 = _t332.fn(_t332.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t333, gem_string("line"));
#line 235 "compiler/main.gem"
    GemVal _t334 = (*gem_v_advance);
            (void)(_t334.fn(_t334.env, NULL, 0));
#line 236 "compiler/main.gem"
    GemVal _t335 = (*gem_v_skip_nl);
            (void)(_t335.fn(_t335.env, NULL, 0));
#line 237 "compiler/main.gem"
    GemVal _t336 = gem_table_new();
            GemVal gem_v_args = _t336;
#line 238 "compiler/main.gem"
    GemVal _t337 = (*gem_v_peek);
    GemVal _t338 = _t337.fn(_t337.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t338, gem_string("type")), gem_string(")")))) {
#line 239 "compiler/main.gem"
    GemVal _t339 = (*gem_v_parse_expr);
    GemVal _t340[] = {gem_v_args, _t339.fn(_t339.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t340, 2));
#line 240 "compiler/main.gem"
                while (1) {
                    GemVal _t341 = (*gem_v_peek);
                    GemVal _t342 = _t341.fn(_t341.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t342, gem_string("type")), gem_string(",")))) break;
#line 241 "compiler/main.gem"
    GemVal _t343 = (*gem_v_advance);
                    (void)(_t343.fn(_t343.env, NULL, 0));
#line 242 "compiler/main.gem"
    GemVal _t344 = (*gem_v_skip_nl);
                    (void)(_t344.fn(_t344.env, NULL, 0));
#line 243 "compiler/main.gem"
    GemVal _t345 = (*gem_v_parse_expr);
    GemVal _t346[] = {gem_v_args, _t345.fn(_t345.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t346, 2));
                }
            }
#line 246 "compiler/main.gem"
    GemVal _t347 = (*gem_v_skip_nl);
            (void)(_t347.fn(_t347.env, NULL, 0));
#line 247 "compiler/main.gem"
    GemVal _t348 = (*gem_v_expect);
    GemVal _t349[] = {gem_string(")")};
            (void)(_t348.fn(_t348.env, _t349, 1));
#line 250 "compiler/main.gem"
    GemVal _t350 = (*gem_v_peek);
    GemVal _t351 = _t350.fn(_t350.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t351, gem_string("type")), gem_string("do")))) {
#line 251 "compiler/main.gem"
    GemVal _t352 = (*gem_v_advance);
                (void)(_t352.fn(_t352.env, NULL, 0));
#line 252 "compiler/main.gem"
    GemVal _t353 = gem_table_new();
                GemVal gem_v_bparams = _t353;
#line 253 "compiler/main.gem"
    GemVal _t354 = (*gem_v_peek);
    GemVal _t355 = _t354.fn(_t354.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t355, gem_string("type")), gem_string("|")))) {
#line 254 "compiler/main.gem"
    GemVal _t356 = (*gem_v_advance);
                    (void)(_t356.fn(_t356.env, NULL, 0));
#line 255 "compiler/main.gem"
    GemVal _t357 = (*gem_v_parse_params);
                    gem_v_bparams = _t357.fn(_t357.env, NULL, 0);
#line 256 "compiler/main.gem"
    GemVal _t358 = (*gem_v_expect);
    GemVal _t359[] = {gem_string("|")};
                    (void)(_t358.fn(_t358.env, _t359, 1));
                }
#line 258 "compiler/main.gem"
    GemVal _t360 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t360.fn(_t360.env, NULL, 0);
#line 259 "compiler/main.gem"
    GemVal _t361 = (*gem_v_expect);
    GemVal _t362[] = {gem_string("end")};
                (void)(_t361.fn(_t361.env, _t362, 1));
#line 260 "compiler/main.gem"
    GemVal _t363[] = {gem_v_bparams, gem_v_body};
    GemVal _t364[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t363, 2)};
                (void)(gem_push_fn(NULL, _t364, 2));
            } else {
#line 264 "compiler/main.gem"
    GemVal _t365 = (*gem_v_peek);
    GemVal _t366 = _t365.fn(_t365.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t366, gem_string("type")), gem_string("{")))) {
#line 265 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 266 "compiler/main.gem"
    GemVal _t367 = (*gem_v_peek_at);
    GemVal _t368[] = {gem_int(1)};
    GemVal _t369 = _t367.fn(_t367.env, _t368, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t369, gem_string("type")), gem_string("|")))) {
#line 267 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 268 "compiler/main.gem"
    GemVal _t370 = (*gem_v_peek_at);
    GemVal _t371[] = {gem_int(1)};
    GemVal _t372 = _t370.fn(_t370.env, _t371, 1);
    GemVal _t380;
    if (gem_truthy(gem_eq(gem_table_get(_t372, gem_string("type")), gem_string("}")))) {
        _t380 = gem_eq(gem_table_get(_t372, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t373 = (*gem_v_peek_at);
        GemVal _t374[] = {gem_int(1)};
        GemVal _t375 = _t373.fn(_t373.env, _t374, 1);
        GemVal _t379;
        if (!gem_truthy(gem_eq(gem_table_get(_t375, gem_string("type")), gem_string("NAME")))) {
                _t379 = gem_eq(gem_table_get(_t375, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t376 = (*gem_v_peek_at);
                GemVal _t377[] = {gem_int(2)};
                GemVal _t378 = _t376.fn(_t376.env, _t377, 1);
                _t379 = gem_eq(gem_table_get(_t378, gem_string("type")), gem_string(":"));
        }
        _t380 = _t379;
    }
                        if (gem_truthy(_t380)) {
#line 269 "compiler/main.gem"
                            gem_v_is_block = gem_bool(0);
                        } else {
#line 271 "compiler/main.gem"
                            gem_v_is_block = gem_bool(1);
                        }
                    }
#line 273 "compiler/main.gem"
                    if (gem_truthy(gem_v_is_block)) {
#line 274 "compiler/main.gem"
    GemVal _t381 = (*gem_v_advance);
                        (void)(_t381.fn(_t381.env, NULL, 0));
#line 275 "compiler/main.gem"
    GemVal _t382 = gem_table_new();
                        GemVal gem_v_bparams = _t382;
#line 276 "compiler/main.gem"
    GemVal _t383 = (*gem_v_peek);
    GemVal _t384 = _t383.fn(_t383.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t384, gem_string("type")), gem_string("|")))) {
#line 277 "compiler/main.gem"
    GemVal _t385 = (*gem_v_advance);
                            (void)(_t385.fn(_t385.env, NULL, 0));
#line 278 "compiler/main.gem"
    GemVal _t386 = (*gem_v_parse_params);
                            gem_v_bparams = _t386.fn(_t386.env, NULL, 0);
#line 279 "compiler/main.gem"
    GemVal _t387 = (*gem_v_expect);
    GemVal _t388[] = {gem_string("|")};
                            (void)(_t387.fn(_t387.env, _t388, 1));
                        }
#line 281 "compiler/main.gem"
    GemVal _t389 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t389.fn(_t389.env, NULL, 0);
#line 282 "compiler/main.gem"
    GemVal _t390 = (*gem_v_expect);
    GemVal _t391[] = {gem_string("}")};
                        (void)(_t390.fn(_t390.env, _t391, 1));
#line 283 "compiler/main.gem"
    GemVal _t392 = gem_table_new();
    gem_table_set(_t392, gem_int(0), gem_v_bexpr);
    GemVal _t393[] = {gem_v_bparams, _t392};
    GemVal _t394[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t393, 2)};
                        (void)(gem_push_fn(NULL, _t394, 2));
                    }
                }
            }
#line 288 "compiler/main.gem"
    GemVal _t395[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t395, 3);
            continue;
        }
#line 293 "compiler/main.gem"
    GemVal _t396 = (*gem_v_peek);
    GemVal _t397 = _t396.fn(_t396.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t397, gem_string("type")), gem_string(".")))) {
#line 294 "compiler/main.gem"
    GemVal _t398 = (*gem_v_advance);
            (void)(_t398.fn(_t398.env, NULL, 0));
#line 295 "compiler/main.gem"
    GemVal _t399 = (*gem_v_expect);
    GemVal _t400[] = {gem_string("NAME")};
    GemVal _t401 = _t399.fn(_t399.env, _t400, 1);
            GemVal gem_v_field = gem_table_get(_t401, gem_string("value"));
#line 296 "compiler/main.gem"
    GemVal _t402[] = {gem_v_node, gem_v_field};
            gem_v_node = gem_fn_make_dot(NULL, _t402, 2);
            continue;
        }
#line 301 "compiler/main.gem"
    GemVal _t403 = (*gem_v_peek);
    GemVal _t404 = _t403.fn(_t403.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t404, gem_string("type")), gem_string("[")))) {
#line 302 "compiler/main.gem"
    GemVal _t405 = (*gem_v_advance);
            (void)(_t405.fn(_t405.env, NULL, 0));
#line 303 "compiler/main.gem"
    GemVal _t406 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t406.fn(_t406.env, NULL, 0);
#line 304 "compiler/main.gem"
    GemVal _t407 = (*gem_v_expect);
    GemVal _t408[] = {gem_string("]")};
            (void)(_t407.fn(_t407.env, _t408, 1));
#line 305 "compiler/main.gem"
    GemVal _t409[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t409, 2);
            continue;
        }
        break;
    }
    return gem_v_node;
}

struct _closure__anon_12 {
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_call;
    GemVal *gem_v_parse_unary;
    GemVal *gem_v_peek;
};
static GemVal _anon_12(void *_env, GemVal *args, int argc) {
    struct _closure__anon_12 *_cls = (struct _closure__anon_12 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_call = _cls->gem_v_parse_call;
    GemVal *gem_v_parse_unary = _cls->gem_v_parse_unary;
    GemVal *gem_v_peek = _cls->gem_v_peek;
#line 317 "compiler/main.gem"
    GemVal _t411 = (*gem_v_peek);
    GemVal _t412 = _t411.fn(_t411.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t412, gem_string("type")), gem_string("-")))) {
#line 318 "compiler/main.gem"
    GemVal _t413 = (*gem_v_advance);
        (void)(_t413.fn(_t413.env, NULL, 0));
#line 319 "compiler/main.gem"
    GemVal _t414 = (*gem_v_parse_unary);
    GemVal _t415[] = {gem_string("-"), _t414.fn(_t414.env, NULL, 0)};
        return gem_fn_make_unop(NULL, _t415, 2);
    }
#line 321 "compiler/main.gem"
    GemVal _t416 = (*gem_v_parse_call);
    return _t416.fn(_t416.env, NULL, 0);
}

struct _closure__anon_13 {
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_unary;
    GemVal *gem_v_peek;
};
static GemVal _anon_13(void *_env, GemVal *args, int argc) {
    struct _closure__anon_13 *_cls = (struct _closure__anon_13 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_unary = _cls->gem_v_parse_unary;
    GemVal *gem_v_peek = _cls->gem_v_peek;
#line 326 "compiler/main.gem"
    GemVal _t418 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t418.fn(_t418.env, NULL, 0);
#line 327 "compiler/main.gem"
    while (1) {
        GemVal _t419 = (*gem_v_peek);
        GemVal _t420 = _t419.fn(_t419.env, NULL, 0);
        GemVal _t423;
        if (gem_truthy(gem_eq(gem_table_get(_t420, gem_string("type")), gem_string("*")))) {
                _t423 = gem_eq(gem_table_get(_t420, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t421 = (*gem_v_peek);
                GemVal _t422 = _t421.fn(_t421.env, NULL, 0);
                _t423 = gem_eq(gem_table_get(_t422, gem_string("type")), gem_string("/"));
        }
        GemVal _t426;
        if (gem_truthy(_t423)) {
                _t426 = _t423;
        } else {
                GemVal _t424 = (*gem_v_peek);
                GemVal _t425 = _t424.fn(_t424.env, NULL, 0);
                _t426 = gem_eq(gem_table_get(_t425, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t426)) break;
#line 328 "compiler/main.gem"
    GemVal _t427 = (*gem_v_advance);
    GemVal _t428 = _t427.fn(_t427.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t428, gem_string("type"));
#line 329 "compiler/main.gem"
    GemVal _t429 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t429.fn(_t429.env, NULL, 0);
#line 330 "compiler/main.gem"
    GemVal _t430[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t430, 3);
    }
    return gem_v_left;
}

struct _closure__anon_14 {
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_mul;
    GemVal *gem_v_peek;
};
static GemVal _anon_14(void *_env, GemVal *args, int argc) {
    struct _closure__anon_14 *_cls = (struct _closure__anon_14 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_mul = _cls->gem_v_parse_mul;
    GemVal *gem_v_peek = _cls->gem_v_peek;
#line 337 "compiler/main.gem"
    GemVal _t432 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t432.fn(_t432.env, NULL, 0);
#line 338 "compiler/main.gem"
    while (1) {
        GemVal _t433 = (*gem_v_peek);
        GemVal _t434 = _t433.fn(_t433.env, NULL, 0);
        GemVal _t437;
        if (gem_truthy(gem_eq(gem_table_get(_t434, gem_string("type")), gem_string("+")))) {
                _t437 = gem_eq(gem_table_get(_t434, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t435 = (*gem_v_peek);
                GemVal _t436 = _t435.fn(_t435.env, NULL, 0);
                _t437 = gem_eq(gem_table_get(_t436, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t437)) break;
#line 339 "compiler/main.gem"
    GemVal _t438 = (*gem_v_advance);
    GemVal _t439 = _t438.fn(_t438.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t439, gem_string("type"));
#line 340 "compiler/main.gem"
    GemVal _t440 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t440.fn(_t440.env, NULL, 0);
#line 341 "compiler/main.gem"
    GemVal _t441[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t441, 3);
    }
    return gem_v_left;
}

struct _closure__anon_15 {
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_add;
    GemVal *gem_v_peek;
};
static GemVal _anon_15(void *_env, GemVal *args, int argc) {
    struct _closure__anon_15 *_cls = (struct _closure__anon_15 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_add = _cls->gem_v_parse_add;
    GemVal *gem_v_peek = _cls->gem_v_peek;
#line 348 "compiler/main.gem"
    GemVal _t443 = (*gem_v_parse_add);
    GemVal gem_v_left = _t443.fn(_t443.env, NULL, 0);
#line 349 "compiler/main.gem"
    while (1) {
        GemVal _t444 = (*gem_v_peek);
        GemVal _t445 = _t444.fn(_t444.env, NULL, 0);
        GemVal _t448;
        if (gem_truthy(gem_eq(gem_table_get(_t445, gem_string("type")), gem_string("==")))) {
                _t448 = gem_eq(gem_table_get(_t445, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t446 = (*gem_v_peek);
                GemVal _t447 = _t446.fn(_t446.env, NULL, 0);
                _t448 = gem_eq(gem_table_get(_t447, gem_string("type")), gem_string("!="));
        }
        GemVal _t451;
        if (gem_truthy(_t448)) {
                _t451 = _t448;
        } else {
                GemVal _t449 = (*gem_v_peek);
                GemVal _t450 = _t449.fn(_t449.env, NULL, 0);
                _t451 = gem_eq(gem_table_get(_t450, gem_string("type")), gem_string("<"));
        }
        GemVal _t454;
        if (gem_truthy(_t451)) {
                _t454 = _t451;
        } else {
                GemVal _t452 = (*gem_v_peek);
                GemVal _t453 = _t452.fn(_t452.env, NULL, 0);
                _t454 = gem_eq(gem_table_get(_t453, gem_string("type")), gem_string(">"));
        }
        GemVal _t457;
        if (gem_truthy(_t454)) {
                _t457 = _t454;
        } else {
                GemVal _t455 = (*gem_v_peek);
                GemVal _t456 = _t455.fn(_t455.env, NULL, 0);
                _t457 = gem_eq(gem_table_get(_t456, gem_string("type")), gem_string("<="));
        }
        GemVal _t460;
        if (gem_truthy(_t457)) {
                _t460 = _t457;
        } else {
                GemVal _t458 = (*gem_v_peek);
                GemVal _t459 = _t458.fn(_t458.env, NULL, 0);
                _t460 = gem_eq(gem_table_get(_t459, gem_string("type")), gem_string(">="));
        }
        if (!gem_truthy(_t460)) break;
#line 350 "compiler/main.gem"
    GemVal _t461 = (*gem_v_advance);
    GemVal _t462 = _t461.fn(_t461.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t462, gem_string("type"));
#line 351 "compiler/main.gem"
    GemVal _t463 = (*gem_v_parse_add);
        GemVal gem_v_right = _t463.fn(_t463.env, NULL, 0);
#line 352 "compiler/main.gem"
    GemVal _t464[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t464, 3);
    }
    return gem_v_left;
}

struct _closure__anon_16 {
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_compare;
    GemVal *gem_v_parse_not;
    GemVal *gem_v_peek;
};
static GemVal _anon_16(void *_env, GemVal *args, int argc) {
    struct _closure__anon_16 *_cls = (struct _closure__anon_16 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_compare = _cls->gem_v_parse_compare;
    GemVal *gem_v_parse_not = _cls->gem_v_parse_not;
    GemVal *gem_v_peek = _cls->gem_v_peek;
#line 359 "compiler/main.gem"
    GemVal _t466 = (*gem_v_peek);
    GemVal _t467 = _t466.fn(_t466.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t467, gem_string("type")), gem_string("not")))) {
#line 360 "compiler/main.gem"
    GemVal _t468 = (*gem_v_advance);
        (void)(_t468.fn(_t468.env, NULL, 0));
#line 361 "compiler/main.gem"
    GemVal _t469 = (*gem_v_parse_not);
    GemVal _t470[] = {gem_string("not"), _t469.fn(_t469.env, NULL, 0)};
        return gem_fn_make_unop(NULL, _t470, 2);
    }
#line 363 "compiler/main.gem"
    GemVal _t471 = (*gem_v_parse_compare);
    return _t471.fn(_t471.env, NULL, 0);
}

struct _closure__anon_17 {
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_not;
    GemVal *gem_v_peek;
};
static GemVal _anon_17(void *_env, GemVal *args, int argc) {
    struct _closure__anon_17 *_cls = (struct _closure__anon_17 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_not = _cls->gem_v_parse_not;
    GemVal *gem_v_peek = _cls->gem_v_peek;
#line 368 "compiler/main.gem"
    GemVal _t473 = (*gem_v_parse_not);
    GemVal gem_v_left = _t473.fn(_t473.env, NULL, 0);
#line 369 "compiler/main.gem"
    while (1) {
        GemVal _t474 = (*gem_v_peek);
        GemVal _t475 = _t474.fn(_t474.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t475, gem_string("type")), gem_string("and")))) break;
#line 370 "compiler/main.gem"
    GemVal _t476 = (*gem_v_advance);
        (void)(_t476.fn(_t476.env, NULL, 0));
#line 371 "compiler/main.gem"
    GemVal _t477 = (*gem_v_parse_not);
        GemVal gem_v_right = _t477.fn(_t477.env, NULL, 0);
#line 372 "compiler/main.gem"
    GemVal _t478[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t478, 3);
    }
    return gem_v_left;
}

struct _closure__anon_18 {
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_and;
    GemVal *gem_v_peek;
};
static GemVal _anon_18(void *_env, GemVal *args, int argc) {
    struct _closure__anon_18 *_cls = (struct _closure__anon_18 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_and = _cls->gem_v_parse_and;
    GemVal *gem_v_peek = _cls->gem_v_peek;
#line 379 "compiler/main.gem"
    GemVal _t480 = (*gem_v_parse_and);
    GemVal gem_v_left = _t480.fn(_t480.env, NULL, 0);
#line 380 "compiler/main.gem"
    while (1) {
        GemVal _t481 = (*gem_v_peek);
        GemVal _t482 = _t481.fn(_t481.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t482, gem_string("type")), gem_string("or")))) break;
#line 381 "compiler/main.gem"
    GemVal _t483 = (*gem_v_advance);
        (void)(_t483.fn(_t483.env, NULL, 0));
#line 382 "compiler/main.gem"
    GemVal _t484 = (*gem_v_parse_and);
        GemVal gem_v_right = _t484.fn(_t484.env, NULL, 0);
#line 383 "compiler/main.gem"
    GemVal _t485[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t485, 3);
    }
    return gem_v_left;
}

struct _closure__anon_19 {
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_expr;
    GemVal *gem_v_parse_or;
    GemVal *gem_v_peek;
};
static GemVal _anon_19(void *_env, GemVal *args, int argc) {
    struct _closure__anon_19 *_cls = (struct _closure__anon_19 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_parse_or = _cls->gem_v_parse_or;
    GemVal *gem_v_peek = _cls->gem_v_peek;
#line 390 "compiler/main.gem"
    GemVal _t487 = (*gem_v_peek);
    GemVal _t488 = _t487.fn(_t487.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t488, gem_string("line"));
#line 391 "compiler/main.gem"
    GemVal _t489 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t489.fn(_t489.env, NULL, 0);
#line 394 "compiler/main.gem"
    GemVal _t490 = (*gem_v_peek);
    GemVal _t491 = _t490.fn(_t490.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t491, gem_string("type")), gem_string("=")))) {
#line 395 "compiler/main.gem"
    GemVal _t492 = (*gem_v_advance);
        (void)(_t492.fn(_t492.env, NULL, 0));
#line 396 "compiler/main.gem"
    GemVal _t493 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t493.fn(_t493.env, NULL, 0);
#line 397 "compiler/main.gem"
    GemVal _t494 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t494, gem_string("tag")), gem_string("var")))) {
#line 398 "compiler/main.gem"
    GemVal _t495 = gem_v_lhs;
    GemVal _t496[] = {gem_table_get(_t495, gem_string("name")), gem_v_value, gem_v_start_line};
            return gem_fn_make_assign(NULL, _t496, 3);
        }
#line 400 "compiler/main.gem"
    GemVal _t497 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t497, gem_string("tag")), gem_string("dot")))) {
#line 401 "compiler/main.gem"
    GemVal _t498 = gem_v_lhs;
    GemVal _t499 = gem_v_lhs;
    GemVal _t500[] = {gem_table_get(_t498, gem_string("object")), gem_table_get(_t499, gem_string("field")), gem_v_value, gem_v_start_line};
            return gem_fn_make_dot_assign(NULL, _t500, 4);
        }
#line 403 "compiler/main.gem"
    GemVal _t501 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t501, gem_string("tag")), gem_string("index")))) {
#line 404 "compiler/main.gem"
    GemVal _t502 = gem_v_lhs;
    GemVal _t503 = gem_v_lhs;
    GemVal _t504[] = {gem_table_get(_t502, gem_string("object")), gem_table_get(_t503, gem_string("key")), gem_v_value, gem_v_start_line};
            return gem_fn_make_index_assign(NULL, _t504, 4);
        }
#line 406 "compiler/main.gem"
    GemVal _t505 = (*gem_v_peek);
    GemVal _t506 = _t505.fn(_t505.env, NULL, 0);
    GemVal _t507[] = {gem_table_get(_t506, gem_string("line"))};
    GemVal _t508[] = {gem_add(gem_string("invalid assignment target at line "), gem_to_string_fn(NULL, _t507, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 406, _t508, 1));
    }
#line 410 "compiler/main.gem"
    GemVal _t509 = (*gem_v_peek);
    GemVal _t510 = _t509.fn(_t509.env, NULL, 0);
    GemVal _t513;
    if (gem_truthy(gem_eq(gem_table_get(_t510, gem_string("type")), gem_string("+=")))) {
        _t513 = gem_eq(gem_table_get(_t510, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t511 = (*gem_v_peek);
        GemVal _t512 = _t511.fn(_t511.env, NULL, 0);
        _t513 = gem_eq(gem_table_get(_t512, gem_string("type")), gem_string("-="));
    }
    GemVal _t516;
    if (gem_truthy(_t513)) {
        _t516 = _t513;
    } else {
        GemVal _t514 = (*gem_v_peek);
        GemVal _t515 = _t514.fn(_t514.env, NULL, 0);
        _t516 = gem_eq(gem_table_get(_t515, gem_string("type")), gem_string("*="));
    }
    GemVal _t519;
    if (gem_truthy(_t516)) {
        _t519 = _t516;
    } else {
        GemVal _t517 = (*gem_v_peek);
        GemVal _t518 = _t517.fn(_t517.env, NULL, 0);
        _t519 = gem_eq(gem_table_get(_t518, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t519)) {
#line 411 "compiler/main.gem"
    GemVal _t520 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t520.fn(_t520.env, NULL, 0);
#line 412 "compiler/main.gem"
    GemVal _t521 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t521, gem_string("value")), gem_int(0));
#line 413 "compiler/main.gem"
    GemVal _t522 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t522.fn(_t522.env, NULL, 0);
#line 414 "compiler/main.gem"
    GemVal _t523 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t523, gem_string("tag")), gem_string("var")))) {
#line 415 "compiler/main.gem"
    GemVal _t524 = gem_v_op_tok;
    GemVal _t525[] = {gem_table_get(_t524, gem_string("line"))};
    GemVal _t526[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t525, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 415, _t526, 1));
        }
#line 417 "compiler/main.gem"
    GemVal _t527 = gem_v_lhs;
    GemVal _t528 = gem_v_lhs;
    GemVal _t529[] = {gem_table_get(_t528, gem_string("name"))};
    GemVal _t530[] = {gem_v_base_op, gem_fn_make_var(NULL, _t529, 1), gem_v_value};
    GemVal _t531[] = {gem_table_get(_t527, gem_string("name")), gem_fn_make_binop(NULL, _t530, 3), gem_v_start_line};
        return gem_fn_make_assign(NULL, _t531, 3);
    }
    return gem_v_lhs;
}

struct _closure__anon_20 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_gensym_counter;
    GemVal *gem_v_parse_body;
    GemVal *gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body;
    GemVal *gem_v_parse_params;
    GemVal *gem_v_parse_stmt;
    GemVal *gem_v_peek;
    GemVal *gem_v_peek_at;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_20(void *_env, GemVal *args, int argc) {
    struct _closure__anon_20 *_cls = (struct _closure__anon_20 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_gensym_counter = _cls->gem_v_gensym_counter;
    GemVal *gem_v_parse_body = _cls->gem_v_parse_body;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body = _cls->gem_v_parse_fn_body;
    GemVal *gem_v_parse_params = _cls->gem_v_parse_params;
    GemVal *gem_v_parse_stmt = _cls->gem_v_parse_stmt;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_peek_at = _cls->gem_v_peek_at;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
#line 429 "compiler/main.gem"
    GemVal _t533 = (*gem_v_peek);
    GemVal gem_v_t = _t533.fn(_t533.env, NULL, 0);
#line 432 "compiler/main.gem"
    GemVal _t534 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t534, gem_string("type")), gem_string("let")))) {
#line 433 "compiler/main.gem"
    GemVal _t535 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t535, gem_string("line"));
#line 434 "compiler/main.gem"
    GemVal _t536 = (*gem_v_advance);
        (void)(_t536.fn(_t536.env, NULL, 0));
#line 435 "compiler/main.gem"
    GemVal _t537 = (*gem_v_expect);
    GemVal _t538[] = {gem_string("NAME")};
    GemVal _t539 = _t537.fn(_t537.env, _t538, 1);
        GemVal gem_v_name = gem_table_get(_t539, gem_string("value"));
#line 436 "compiler/main.gem"
    GemVal _t540 = (*gem_v_expect);
    GemVal _t541[] = {gem_string("=")};
        (void)(_t540.fn(_t540.env, _t541, 1));
#line 437 "compiler/main.gem"
    GemVal _t542 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t542.fn(_t542.env, NULL, 0);
#line 438 "compiler/main.gem"
    GemVal _t543[] = {gem_v_name, gem_v_value, gem_v_line};
        return gem_fn_make_let(NULL, _t543, 3);
    }
#line 442 "compiler/main.gem"
    GemVal _t544 = gem_v_t;
    GemVal _t548;
    if (!gem_truthy(gem_eq(gem_table_get(_t544, gem_string("type")), gem_string("fn")))) {
        _t548 = gem_eq(gem_table_get(_t544, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t545 = (*gem_v_peek_at);
        GemVal _t546[] = {gem_int(1)};
        GemVal _t547 = _t545.fn(_t545.env, _t546, 1);
        _t548 = gem_eq(gem_table_get(_t547, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t548)) {
#line 443 "compiler/main.gem"
    GemVal _t549 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t549, gem_string("line"));
#line 444 "compiler/main.gem"
    GemVal _t550 = (*gem_v_advance);
        (void)(_t550.fn(_t550.env, NULL, 0));
#line 445 "compiler/main.gem"
    GemVal _t551 = (*gem_v_expect);
    GemVal _t552[] = {gem_string("NAME")};
    GemVal _t553 = _t551.fn(_t551.env, _t552, 1);
        GemVal gem_v_name = gem_table_get(_t553, gem_string("value"));
#line 446 "compiler/main.gem"
    GemVal _t554 = (*gem_v_expect);
    GemVal _t555[] = {gem_string("(")};
        (void)(_t554.fn(_t554.env, _t555, 1));
#line 447 "compiler/main.gem"
    GemVal _t556 = (*gem_v_parse_params);
        GemVal gem_v_params = _t556.fn(_t556.env, NULL, 0);
#line 448 "compiler/main.gem"
    GemVal _t557 = (*gem_v_expect);
    GemVal _t558[] = {gem_string(")")};
        (void)(_t557.fn(_t557.env, _t558, 1));
#line 449 "compiler/main.gem"
    GemVal _t559 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t559.fn(_t559.env, NULL, 0);
#line 450 "compiler/main.gem"
    GemVal _t560 = (*gem_v_expect);
    GemVal _t561[] = {gem_string("end")};
        (void)(_t560.fn(_t560.env, _t561, 1));
#line 451 "compiler/main.gem"
    GemVal _t562[] = {gem_v_name, gem_v_params, gem_v_body, gem_v_line};
        return gem_fn_make_fn_def(NULL, _t562, 4);
    }
#line 455 "compiler/main.gem"
    GemVal _t563 = gem_v_t;
    GemVal _t565;
    if (gem_truthy(gem_eq(gem_table_get(_t563, gem_string("type")), gem_string("if")))) {
        _t565 = gem_eq(gem_table_get(_t563, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t564 = gem_v_t;
        _t565 = gem_eq(gem_table_get(_t564, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t565)) {
#line 456 "compiler/main.gem"
    GemVal _t566 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t566, gem_string("line"));
#line 457 "compiler/main.gem"
    GemVal _t567 = (*gem_v_advance);
        (void)(_t567.fn(_t567.env, NULL, 0));
#line 458 "compiler/main.gem"
    GemVal _t568 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t568.fn(_t568.env, NULL, 0);
#line 459 "compiler/main.gem"
    GemVal _t569 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t569.fn(_t569.env, NULL, 0);
#line 460 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 461 "compiler/main.gem"
    GemVal _t570 = (*gem_v_peek);
    GemVal _t571 = _t570.fn(_t570.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t571, gem_string("type")), gem_string("elif")))) {
#line 463 "compiler/main.gem"
    GemVal _t572 = gem_table_new();
    GemVal _t573 = (*gem_v_parse_stmt);
    gem_table_set(_t572, gem_int(0), _t573.fn(_t573.env, NULL, 0));
            gem_v_else_body = _t572;
        } else {
#line 464 "compiler/main.gem"
    GemVal _t574 = (*gem_v_peek);
    GemVal _t575 = _t574.fn(_t574.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t575, gem_string("type")), gem_string("else")))) {
#line 465 "compiler/main.gem"
    GemVal _t576 = (*gem_v_advance);
                (void)(_t576.fn(_t576.env, NULL, 0));
#line 466 "compiler/main.gem"
    GemVal _t577 = (*gem_v_parse_body);
                gem_v_else_body = _t577.fn(_t577.env, NULL, 0);
            }
        }
#line 469 "compiler/main.gem"
    GemVal _t578 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t578, gem_string("type")), gem_string("if")))) {
#line 470 "compiler/main.gem"
    GemVal _t579 = (*gem_v_expect);
    GemVal _t580[] = {gem_string("end")};
            (void)(_t579.fn(_t579.env, _t580, 1));
        }
#line 472 "compiler/main.gem"
    GemVal _t581[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        return gem_fn_make_if(NULL, _t581, 4);
    }
#line 476 "compiler/main.gem"
    GemVal _t582 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t582, gem_string("type")), gem_string("while")))) {
#line 477 "compiler/main.gem"
    GemVal _t583 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t583, gem_string("line"));
#line 478 "compiler/main.gem"
    GemVal _t584 = (*gem_v_advance);
        (void)(_t584.fn(_t584.env, NULL, 0));
#line 479 "compiler/main.gem"
    GemVal _t585 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t585.fn(_t585.env, NULL, 0);
#line 480 "compiler/main.gem"
    GemVal _t586 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t586.fn(_t586.env, NULL, 0);
#line 481 "compiler/main.gem"
    GemVal _t587 = (*gem_v_expect);
    GemVal _t588[] = {gem_string("end")};
        (void)(_t587.fn(_t587.env, _t588, 1));
#line 482 "compiler/main.gem"
    GemVal _t589[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        return gem_fn_make_while(NULL, _t589, 3);
    }
#line 486 "compiler/main.gem"
    GemVal _t590 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t590, gem_string("type")), gem_string("for")))) {
#line 487 "compiler/main.gem"
    GemVal _t591 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t591, gem_string("line"));
#line 488 "compiler/main.gem"
    GemVal _t592 = (*gem_v_advance);
        (void)(_t592.fn(_t592.env, NULL, 0));
#line 489 "compiler/main.gem"
    GemVal _t593 = (*gem_v_expect);
    GemVal _t594[] = {gem_string("NAME")};
    GemVal _t595 = _t593.fn(_t593.env, _t594, 1);
        GemVal gem_v_var_name = gem_table_get(_t595, gem_string("value"));
#line 491 "compiler/main.gem"
    GemVal _t596 = (*gem_v_peek);
    GemVal _t597 = _t596.fn(_t596.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t597, gem_string("type")), gem_string("in")))) {
#line 493 "compiler/main.gem"
    GemVal _t598 = (*gem_v_advance);
            (void)(_t598.fn(_t598.env, NULL, 0));
#line 494 "compiler/main.gem"
    GemVal _t599 = (*gem_v_parse_expr);
            GemVal gem_v_items_expr = _t599.fn(_t599.env, NULL, 0);
#line 495 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 496 "compiler/main.gem"
    GemVal _t600[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t600, 1));
#line 497 "compiler/main.gem"
    GemVal _t601[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t601, 1));
#line 498 "compiler/main.gem"
    GemVal _t602 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t602.fn(_t602.env, NULL, 0);
#line 499 "compiler/main.gem"
    GemVal _t603 = (*gem_v_expect);
    GemVal _t604[] = {gem_string("end")};
            (void)(_t603.fn(_t603.env, _t604, 1));
#line 500 "compiler/main.gem"
    GemVal _t605 = gem_table_new();
            GemVal gem_v_inner_stmts = _t605;
#line 501 "compiler/main.gem"
    GemVal _t606[] = {gem_v_items_var};
    GemVal _t607[] = {gem_v_idx_var};
    GemVal _t608[] = {gem_fn_make_var(NULL, _t606, 1), gem_fn_make_var(NULL, _t607, 1)};
    GemVal _t609[] = {gem_v_var_name, gem_fn_make_index(NULL, _t608, 2), gem_v_line};
    GemVal _t610[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t609, 3)};
            (void)(gem_push_fn(NULL, _t610, 2));
#line 502 "compiler/main.gem"
    GemVal _t611[] = {gem_v_idx_var};
    GemVal _t612[] = {gem_int(1)};
    GemVal _t613[] = {gem_string("+"), gem_fn_make_var(NULL, _t611, 1), gem_fn_make_int(NULL, _t612, 1)};
    GemVal _t614[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t613, 3), gem_v_line};
    GemVal _t615[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t614, 3)};
            (void)(gem_push_fn(NULL, _t615, 2));
            {
#line 503 "compiler/main.gem"
                GemVal gem_v__for_i_1 = gem_int(0);
#line 503 "compiler/main.gem"
    GemVal _t616[] = {gem_v_fbody};
                GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t616, 1);
#line 503 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 503 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_1;
#line 503 "compiler/main.gem"
                    gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 504 "compiler/main.gem"
    GemVal _t617[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t617, 2));
                }
            }
#line 506 "compiler/main.gem"
    GemVal _t618[] = {gem_v_idx_var};
    GemVal _t619[] = {gem_string("len")};
    GemVal _t620 = gem_table_new();
    GemVal _t621[] = {gem_v_items_var};
    gem_table_set(_t620, gem_int(0), gem_fn_make_var(NULL, _t621, 1));
    GemVal _t622[] = {gem_fn_make_var(NULL, _t619, 1), _t620, gem_int(0)};
    GemVal _t623[] = {gem_string("<"), gem_fn_make_var(NULL, _t618, 1), gem_fn_make_call(NULL, _t622, 3)};
    GemVal _t624[] = {gem_fn_make_binop(NULL, _t623, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t624, 3);
#line 511 "compiler/main.gem"
    GemVal _t625 = gem_table_new();
    gem_table_set(_t625, gem_string("tag"), gem_string("block"));
    GemVal _t626 = gem_table_new();
    GemVal _t627[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t626, gem_int(0), gem_fn_make_let(NULL, _t627, 3));
    GemVal _t628[] = {gem_int(0)};
    GemVal _t629[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t628, 1), gem_v_line};
    gem_table_set(_t626, gem_int(1), gem_fn_make_let(NULL, _t629, 3));
    gem_table_set(_t626, gem_int(2), gem_v_while_node);
    gem_table_set(_t625, gem_string("stmts"), _t626);
            return _t625;
        } else {
#line 516 "compiler/main.gem"
    GemVal _t630 = (*gem_v_peek);
    GemVal _t631 = _t630.fn(_t630.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t631, gem_string("type")), gem_string("=")))) {
#line 518 "compiler/main.gem"
    GemVal _t632 = (*gem_v_advance);
                (void)(_t632.fn(_t632.env, NULL, 0));
#line 519 "compiler/main.gem"
    GemVal _t633 = (*gem_v_parse_expr);
                GemVal gem_v_start_expr = _t633.fn(_t633.env, NULL, 0);
#line 520 "compiler/main.gem"
    GemVal _t634 = (*gem_v_expect);
    GemVal _t635[] = {gem_string(",")};
                (void)(_t634.fn(_t634.env, _t635, 1));
#line 521 "compiler/main.gem"
    GemVal _t636 = (*gem_v_parse_expr);
                GemVal gem_v_end_expr = _t636.fn(_t636.env, NULL, 0);
#line 522 "compiler/main.gem"
    GemVal _t637 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t637.fn(_t637.env, NULL, 0);
#line 523 "compiler/main.gem"
    GemVal _t638 = (*gem_v_expect);
    GemVal _t639[] = {gem_string("end")};
                (void)(_t638.fn(_t638.env, _t639, 1));
#line 524 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 525 "compiler/main.gem"
    GemVal _t640[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t640, 1));
#line 526 "compiler/main.gem"
    GemVal _t641[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t641, 1));
#line 527 "compiler/main.gem"
    GemVal _t642 = gem_table_new();
                GemVal gem_v_inner_stmts = _t642;
#line 528 "compiler/main.gem"
    GemVal _t643[] = {gem_v_idx_var};
    GemVal _t644[] = {gem_v_var_name, gem_fn_make_var(NULL, _t643, 1), gem_v_line};
    GemVal _t645[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t644, 3)};
                (void)(gem_push_fn(NULL, _t645, 2));
#line 529 "compiler/main.gem"
    GemVal _t646[] = {gem_v_idx_var};
    GemVal _t647[] = {gem_int(1)};
    GemVal _t648[] = {gem_string("+"), gem_fn_make_var(NULL, _t646, 1), gem_fn_make_int(NULL, _t647, 1)};
    GemVal _t649[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t648, 3), gem_v_line};
    GemVal _t650[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t649, 3)};
                (void)(gem_push_fn(NULL, _t650, 2));
                {
#line 530 "compiler/main.gem"
                    GemVal gem_v__for_i_2 = gem_int(0);
#line 530 "compiler/main.gem"
    GemVal _t651[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t651, 1);
#line 530 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 530 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_2;
#line 530 "compiler/main.gem"
                        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 531 "compiler/main.gem"
    GemVal _t652[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t652, 2));
                    }
                }
#line 533 "compiler/main.gem"
    GemVal _t653[] = {gem_v_idx_var};
    GemVal _t654[] = {gem_v_limit_var};
    GemVal _t655[] = {gem_string("<"), gem_fn_make_var(NULL, _t653, 1), gem_fn_make_var(NULL, _t654, 1)};
    GemVal _t656[] = {gem_fn_make_binop(NULL, _t655, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t656, 3);
#line 538 "compiler/main.gem"
    GemVal _t657 = gem_table_new();
    gem_table_set(_t657, gem_string("tag"), gem_string("block"));
    GemVal _t658 = gem_table_new();
    GemVal _t659[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t658, gem_int(0), gem_fn_make_let(NULL, _t659, 3));
    GemVal _t660[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t658, gem_int(1), gem_fn_make_let(NULL, _t660, 3));
    gem_table_set(_t658, gem_int(2), gem_v_while_node);
    gem_table_set(_t657, gem_string("stmts"), _t658);
                return _t657;
            } else {
#line 544 "compiler/main.gem"
    GemVal _t661 = (*gem_v_peek);
    GemVal _t662 = _t661.fn(_t661.env, NULL, 0);
    GemVal _t663[] = {gem_table_get(_t662, gem_string("line"))};
    GemVal _t664[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t663, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 544, _t664, 1));
            }
        }
    }
#line 549 "compiler/main.gem"
    GemVal _t665 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t665, gem_string("type")), gem_string("match")))) {
#line 550 "compiler/main.gem"
    GemVal _t666 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t666, gem_string("line"));
#line 551 "compiler/main.gem"
    GemVal _t667 = (*gem_v_advance);
        (void)(_t667.fn(_t667.env, NULL, 0));
#line 552 "compiler/main.gem"
    GemVal _t668 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t668.fn(_t668.env, NULL, 0);
#line 553 "compiler/main.gem"
    GemVal _t669 = (*gem_v_skip_nl);
        (void)(_t669.fn(_t669.env, NULL, 0));
#line 554 "compiler/main.gem"
    GemVal _t670 = gem_table_new();
        GemVal gem_v_whens = _t670;
#line 555 "compiler/main.gem"
        while (1) {
            GemVal _t671 = (*gem_v_peek);
            GemVal _t672 = _t671.fn(_t671.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t672, gem_string("type")), gem_string("when")))) break;
#line 556 "compiler/main.gem"
    GemVal _t673 = (*gem_v_advance);
            (void)(_t673.fn(_t673.env, NULL, 0));
#line 557 "compiler/main.gem"
    GemVal _t674 = (*gem_v_parse_expr);
            GemVal gem_v_wval = _t674.fn(_t674.env, NULL, 0);
#line 558 "compiler/main.gem"
    GemVal _t675 = (*gem_v_parse_body);
            GemVal gem_v_wbody = _t675.fn(_t675.env, NULL, 0);
#line 559 "compiler/main.gem"
    GemVal _t676[] = {gem_v_wval, gem_v_wbody};
    GemVal _t677[] = {gem_v_whens, gem_fn_make_when(NULL, _t676, 2)};
            (void)(gem_push_fn(NULL, _t677, 2));
#line 560 "compiler/main.gem"
    GemVal _t678 = (*gem_v_skip_nl);
            (void)(_t678.fn(_t678.env, NULL, 0));
        }
#line 562 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 563 "compiler/main.gem"
    GemVal _t679 = (*gem_v_peek);
    GemVal _t680 = _t679.fn(_t679.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t680, gem_string("type")), gem_string("else")))) {
#line 564 "compiler/main.gem"
    GemVal _t681 = (*gem_v_advance);
            (void)(_t681.fn(_t681.env, NULL, 0));
#line 565 "compiler/main.gem"
    GemVal _t682 = (*gem_v_parse_body);
            gem_v_else_body = _t682.fn(_t682.env, NULL, 0);
        }
#line 567 "compiler/main.gem"
    GemVal _t683 = (*gem_v_expect);
    GemVal _t684[] = {gem_string("end")};
        (void)(_t683.fn(_t683.env, _t684, 1));
#line 568 "compiler/main.gem"
    GemVal _t685[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line};
        return gem_fn_make_match(NULL, _t685, 4);
    }
#line 572 "compiler/main.gem"
    GemVal _t686 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t686, gem_string("type")), gem_string("return")))) {
#line 573 "compiler/main.gem"
    GemVal _t687 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t687, gem_string("line"));
#line 574 "compiler/main.gem"
    GemVal _t688 = (*gem_v_advance);
        (void)(_t688.fn(_t688.env, NULL, 0));
#line 575 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 576 "compiler/main.gem"
    GemVal _t689 = (*gem_v_peek);
    GemVal _t690 = _t689.fn(_t689.env, NULL, 0);
    GemVal _t693;
    if (!gem_truthy(gem_neq(gem_table_get(_t690, gem_string("type")), gem_string("NEWLINE")))) {
        _t693 = gem_neq(gem_table_get(_t690, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t691 = (*gem_v_peek);
        GemVal _t692 = _t691.fn(_t691.env, NULL, 0);
        _t693 = gem_neq(gem_table_get(_t692, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t696;
    if (!gem_truthy(_t693)) {
        _t696 = _t693;
    } else {
        GemVal _t694 = (*gem_v_peek);
        GemVal _t695 = _t694.fn(_t694.env, NULL, 0);
        _t696 = gem_neq(gem_table_get(_t695, gem_string("type")), gem_string("end"));
    }
    GemVal _t699;
    if (!gem_truthy(_t696)) {
        _t699 = _t696;
    } else {
        GemVal _t697 = (*gem_v_peek);
        GemVal _t698 = _t697.fn(_t697.env, NULL, 0);
        _t699 = gem_neq(gem_table_get(_t698, gem_string("type")), gem_string("elif"));
    }
    GemVal _t702;
    if (!gem_truthy(_t699)) {
        _t702 = _t699;
    } else {
        GemVal _t700 = (*gem_v_peek);
        GemVal _t701 = _t700.fn(_t700.env, NULL, 0);
        _t702 = gem_neq(gem_table_get(_t701, gem_string("type")), gem_string("else"));
    }
    GemVal _t705;
    if (!gem_truthy(_t702)) {
        _t705 = _t702;
    } else {
        GemVal _t703 = (*gem_v_peek);
        GemVal _t704 = _t703.fn(_t703.env, NULL, 0);
        _t705 = gem_neq(gem_table_get(_t704, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t705)) {
#line 577 "compiler/main.gem"
    GemVal _t706 = (*gem_v_parse_expr);
            gem_v_value = _t706.fn(_t706.env, NULL, 0);
        }
#line 579 "compiler/main.gem"
    GemVal _t707[] = {gem_v_value, gem_v_line};
        return gem_fn_make_return(NULL, _t707, 2);
    }
#line 583 "compiler/main.gem"
    GemVal _t708 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t708, gem_string("type")), gem_string("break")))) {
#line 584 "compiler/main.gem"
    GemVal _t709 = (*gem_v_advance);
        (void)(_t709.fn(_t709.env, NULL, 0));
#line 585 "compiler/main.gem"
        return gem_fn_make_break(NULL, NULL, 0);
    }
#line 589 "compiler/main.gem"
    GemVal _t710 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t710, gem_string("type")), gem_string("continue")))) {
#line 590 "compiler/main.gem"
    GemVal _t711 = (*gem_v_advance);
        (void)(_t711.fn(_t711.env, NULL, 0));
#line 591 "compiler/main.gem"
        return gem_fn_make_continue(NULL, NULL, 0);
    }
#line 595 "compiler/main.gem"
    GemVal _t712 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t712, gem_string("type")), gem_string("load")))) {
#line 596 "compiler/main.gem"
    GemVal _t713 = (*gem_v_advance);
        (void)(_t713.fn(_t713.env, NULL, 0));
#line 597 "compiler/main.gem"
    GemVal _t714 = (*gem_v_expect);
    GemVal _t715[] = {gem_string("STRING")};
    GemVal _t716 = _t714.fn(_t714.env, _t715, 1);
        GemVal gem_v_path = gem_table_get(_t716, gem_string("value"));
#line 598 "compiler/main.gem"
    GemVal _t717[] = {gem_v_path};
        return gem_fn_make_load(NULL, _t717, 1);
    }
#line 602 "compiler/main.gem"
    GemVal _t718 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t718, gem_string("type")), gem_string("extern")))) {
#line 603 "compiler/main.gem"
    GemVal _t719 = (*gem_v_advance);
        (void)(_t719.fn(_t719.env, NULL, 0));
#line 604 "compiler/main.gem"
    GemVal _t720 = (*gem_v_peek);
    GemVal _t721 = _t720.fn(_t720.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t721, gem_string("type")), gem_string("fn")))) {
#line 605 "compiler/main.gem"
    GemVal _t722 = (*gem_v_advance);
            (void)(_t722.fn(_t722.env, NULL, 0));
#line 606 "compiler/main.gem"
    GemVal _t723 = (*gem_v_expect);
    GemVal _t724[] = {gem_string("NAME")};
    GemVal _t725 = _t723.fn(_t723.env, _t724, 1);
            GemVal gem_v_name = gem_table_get(_t725, gem_string("value"));
#line 607 "compiler/main.gem"
    GemVal _t726 = (*gem_v_expect);
    GemVal _t727[] = {gem_string("(")};
            (void)(_t726.fn(_t726.env, _t727, 1));
#line 608 "compiler/main.gem"
    GemVal _t728 = gem_table_new();
            GemVal gem_v_eparams = _t728;
#line 609 "compiler/main.gem"
    GemVal _t729 = (*gem_v_peek);
    GemVal _t730 = _t729.fn(_t729.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t730, gem_string("type")), gem_string(")")))) {
#line 610 "compiler/main.gem"
    GemVal _t731 = (*gem_v_expect);
    GemVal _t732[] = {gem_string("NAME")};
    GemVal _t733 = _t731.fn(_t731.env, _t732, 1);
                GemVal gem_v_pname = gem_table_get(_t733, gem_string("value"));
#line 611 "compiler/main.gem"
    GemVal _t734 = (*gem_v_expect);
    GemVal _t735[] = {gem_string(":")};
                (void)(_t734.fn(_t734.env, _t735, 1));
#line 612 "compiler/main.gem"
    GemVal _t736 = (*gem_v_expect);
    GemVal _t737[] = {gem_string("NAME")};
    GemVal _t738 = _t736.fn(_t736.env, _t737, 1);
                GemVal gem_v_ptype = gem_table_get(_t738, gem_string("value"));
#line 613 "compiler/main.gem"
    GemVal _t739[] = {gem_v_pname, gem_v_ptype};
    GemVal _t740[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t739, 2)};
                (void)(gem_push_fn(NULL, _t740, 2));
#line 614 "compiler/main.gem"
                while (1) {
                    GemVal _t741 = (*gem_v_peek);
                    GemVal _t742 = _t741.fn(_t741.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t742, gem_string("type")), gem_string(",")))) break;
#line 615 "compiler/main.gem"
    GemVal _t743 = (*gem_v_advance);
                    (void)(_t743.fn(_t743.env, NULL, 0));
#line 616 "compiler/main.gem"
    GemVal _t744 = (*gem_v_expect);
    GemVal _t745[] = {gem_string("NAME")};
    GemVal _t746 = _t744.fn(_t744.env, _t745, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t746, gem_string("value"));
#line 617 "compiler/main.gem"
    GemVal _t747 = (*gem_v_expect);
    GemVal _t748[] = {gem_string(":")};
                    (void)(_t747.fn(_t747.env, _t748, 1));
#line 618 "compiler/main.gem"
    GemVal _t749 = (*gem_v_expect);
    GemVal _t750[] = {gem_string("NAME")};
    GemVal _t751 = _t749.fn(_t749.env, _t750, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t751, gem_string("value"));
#line 619 "compiler/main.gem"
    GemVal _t752[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t753[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t752, 2)};
                    (void)(gem_push_fn(NULL, _t753, 2));
                }
            }
#line 622 "compiler/main.gem"
    GemVal _t754 = (*gem_v_expect);
    GemVal _t755[] = {gem_string(")")};
            (void)(_t754.fn(_t754.env, _t755, 1));
#line 623 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 624 "compiler/main.gem"
    GemVal _t756 = (*gem_v_peek);
    GemVal _t757 = _t756.fn(_t756.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t757, gem_string("type")), gem_string("->")))) {
#line 625 "compiler/main.gem"
    GemVal _t758 = (*gem_v_advance);
                (void)(_t758.fn(_t758.env, NULL, 0));
#line 626 "compiler/main.gem"
    GemVal _t759 = (*gem_v_expect);
    GemVal _t760[] = {gem_string("NAME")};
    GemVal _t761 = _t759.fn(_t759.env, _t760, 1);
                gem_v_ret_type = gem_table_get(_t761, gem_string("value"));
            }
#line 628 "compiler/main.gem"
    GemVal _t762[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            return gem_fn_make_extern_fn(NULL, _t762, 3);
        } else {
#line 629 "compiler/main.gem"
    GemVal _t763 = (*gem_v_peek);
    GemVal _t764 = _t763.fn(_t763.env, NULL, 0);
    GemVal _t767;
    if (!gem_truthy(gem_eq(gem_table_get(_t764, gem_string("type")), gem_string("NAME")))) {
        _t767 = gem_eq(gem_table_get(_t764, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t765 = (*gem_v_peek);
        GemVal _t766 = _t765.fn(_t765.env, NULL, 0);
        _t767 = gem_eq(gem_table_get(_t766, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t767)) {
#line 630 "compiler/main.gem"
    GemVal _t768 = (*gem_v_advance);
                (void)(_t768.fn(_t768.env, NULL, 0));
#line 631 "compiler/main.gem"
    GemVal _t769 = (*gem_v_expect);
    GemVal _t770[] = {gem_string("STRING")};
    GemVal _t771 = _t769.fn(_t769.env, _t770, 1);
                GemVal gem_v_path = gem_table_get(_t771, gem_string("value"));
#line 632 "compiler/main.gem"
    GemVal _t772[] = {gem_v_path};
                return gem_fn_make_extern_include(NULL, _t772, 1);
            } else {
#line 634 "compiler/main.gem"
    GemVal _t773 = (*gem_v_peek);
    GemVal _t774 = _t773.fn(_t773.env, NULL, 0);
    GemVal _t775[] = {gem_table_get(_t774, gem_string("line"))};
    GemVal _t776[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t775, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 634, _t776, 1));
            }
        }
    }
#line 639 "compiler/main.gem"
    GemVal _t777 = (*gem_v_parse_expr);
    return _t777.fn(_t777.env, NULL, 0);
}

struct _closure__anon_21 {
    GemVal *gem_v_at_end;
    GemVal *gem_v_parse_stmt;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_21(void *_env, GemVal *args, int argc) {
    struct _closure__anon_21 *_cls = (struct _closure__anon_21 *)_env;
    GemVal *gem_v_at_end = _cls->gem_v_at_end;
    GemVal *gem_v_parse_stmt = _cls->gem_v_parse_stmt;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
#line 648 "compiler/main.gem"
    GemVal _t779 = gem_table_new();
    GemVal gem_v_stmts = _t779;
#line 649 "compiler/main.gem"
    GemVal _t780 = (*gem_v_skip_nl);
    (void)(_t780.fn(_t780.env, NULL, 0));
#line 650 "compiler/main.gem"
    while (1) {
        GemVal _t781 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t781.fn(_t781.env, NULL, 0)))) break;
#line 651 "compiler/main.gem"
    GemVal _t782 = (*gem_v_parse_stmt);
    GemVal _t783[] = {gem_v_stmts, _t782.fn(_t782.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t783, 2));
#line 652 "compiler/main.gem"
    GemVal _t784 = (*gem_v_skip_nl);
        (void)(_t784.fn(_t784.env, NULL, 0));
    }
#line 654 "compiler/main.gem"
    GemVal _t785[] = {gem_v_stmts};
    return gem_fn_make_program(NULL, _t785, 1);
}

static GemVal gem_fn_make_parser(void *_env, GemVal *args, int argc) {
#line 18 "compiler/main.gem"
    GemVal *gem_v_tokens = GC_MALLOC(sizeof(GemVal));
    *gem_v_tokens = (argc > 0) ? args[0] : GEM_NIL;
#line 19 "compiler/main.gem"
    GemVal *gem_v_pos = GC_MALLOC(sizeof(GemVal));
    *gem_v_pos = gem_int(0);
#line 20 "compiler/main.gem"
    GemVal *gem_v_gensym_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_gensym_counter = gem_int(0);
#line 24 "compiler/main.gem"
    struct _closure__anon_1 *_t165 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t165->gem_v_pos = gem_v_pos;
    _t165->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t165);
#line 28 "compiler/main.gem"
    struct _closure__anon_2 *_t166 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t166->gem_v_pos = gem_v_pos;
    _t166->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t166);
#line 32 "compiler/main.gem"
    struct _closure__anon_3 *_t167 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t167->gem_v_pos = gem_v_pos;
    _t167->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t167);
#line 38 "compiler/main.gem"
    struct _closure__anon_4 *_t169 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t169->gem_v_pos = gem_v_pos;
    _t169->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t169);
#line 42 "compiler/main.gem"
    struct _closure__anon_5 *_t177 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t177->gem_v_pos = gem_v_pos;
    _t177->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t177);
#line 51 "compiler/main.gem"
    struct _closure__anon_6 *_t179 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t179->gem_v_pos = gem_v_pos;
    _t179->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t179);
#line 59 "compiler/main.gem"
    struct _closure__anon_7 *_t193 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t193->gem_v_advance = gem_v_advance;
    _t193->gem_v_expect = gem_v_expect;
    _t193->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t193);
#line 73 "compiler/main.gem"
    GemVal *gem_v_parse_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_expr = GEM_NIL;
#line 74 "compiler/main.gem"
    GemVal *gem_v_parse_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_stmt = GEM_NIL;
#line 75 "compiler/main.gem"
    GemVal *gem_v_parse_unary = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_unary = GEM_NIL;
#line 76 "compiler/main.gem"
    GemVal *gem_v_parse_not = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_not = GEM_NIL;
#line 82 "compiler/main.gem"
    struct _closure__anon_8 *_t212 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t212->gem_v_at_end = gem_v_at_end;
    _t212->gem_v_parse_stmt = gem_v_parse_stmt;
    _t212->gem_v_peek = gem_v_peek;
    _t212->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t212);
#line 95 "compiler/main.gem"
    struct _closure__anon_9 *_t222 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t222->gem_v_at_end = gem_v_at_end;
    _t222->gem_v_parse_stmt = gem_v_parse_stmt;
    _t222->gem_v_peek = gem_v_peek;
    _t222->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t222);
#line 108 "compiler/main.gem"
    struct _closure__anon_10 *_t328 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t328->gem_v_advance = gem_v_advance;
    _t328->gem_v_expect = gem_v_expect;
    _t328->gem_v_parse_expr = gem_v_parse_expr;
    _t328->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t328->gem_v_parse_params = gem_v_parse_params;
    _t328->gem_v_peek = gem_v_peek;
    _t328->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t328);
#line 228 "compiler/main.gem"
    struct _closure__anon_11 *_t410 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t410->gem_v_advance = gem_v_advance;
    _t410->gem_v_expect = gem_v_expect;
    _t410->gem_v_parse_atom = gem_v_parse_atom;
    _t410->gem_v_parse_expr = gem_v_parse_expr;
    _t410->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t410->gem_v_parse_params = gem_v_parse_params;
    _t410->gem_v_peek = gem_v_peek;
    _t410->gem_v_peek_at = gem_v_peek_at;
    _t410->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t410);
#line 316 "compiler/main.gem"
    struct _closure__anon_12 *_t417 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t417->gem_v_advance = gem_v_advance;
    _t417->gem_v_parse_call = gem_v_parse_call;
    _t417->gem_v_parse_unary = gem_v_parse_unary;
    _t417->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t417);
#line 325 "compiler/main.gem"
    struct _closure__anon_13 *_t431 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t431->gem_v_advance = gem_v_advance;
    _t431->gem_v_parse_unary = gem_v_parse_unary;
    _t431->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t431);
#line 336 "compiler/main.gem"
    struct _closure__anon_14 *_t442 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t442->gem_v_advance = gem_v_advance;
    _t442->gem_v_parse_mul = gem_v_parse_mul;
    _t442->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t442);
#line 347 "compiler/main.gem"
    struct _closure__anon_15 *_t465 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t465->gem_v_advance = gem_v_advance;
    _t465->gem_v_parse_add = gem_v_parse_add;
    _t465->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t465);
#line 358 "compiler/main.gem"
    struct _closure__anon_16 *_t472 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t472->gem_v_advance = gem_v_advance;
    _t472->gem_v_parse_compare = gem_v_parse_compare;
    _t472->gem_v_parse_not = gem_v_parse_not;
    _t472->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t472);
#line 367 "compiler/main.gem"
    struct _closure__anon_17 *_t479 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t479->gem_v_advance = gem_v_advance;
    _t479->gem_v_parse_not = gem_v_parse_not;
    _t479->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t479);
#line 378 "compiler/main.gem"
    struct _closure__anon_18 *_t486 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t486->gem_v_advance = gem_v_advance;
    _t486->gem_v_parse_and = gem_v_parse_and;
    _t486->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t486);
#line 389 "compiler/main.gem"
    struct _closure__anon_19 *_t532 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t532->gem_v_advance = gem_v_advance;
    _t532->gem_v_parse_expr = gem_v_parse_expr;
    _t532->gem_v_parse_or = gem_v_parse_or;
    _t532->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t532);
#line 424 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 428 "compiler/main.gem"
    struct _closure__anon_20 *_t778 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t778->gem_v_advance = gem_v_advance;
    _t778->gem_v_expect = gem_v_expect;
    _t778->gem_v_gensym_counter = gem_v_gensym_counter;
    _t778->gem_v_parse_body = gem_v_parse_body;
    _t778->gem_v_parse_expr = gem_v_parse_expr;
    _t778->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t778->gem_v_parse_params = gem_v_parse_params;
    _t778->gem_v_parse_stmt = gem_v_parse_stmt;
    _t778->gem_v_peek = gem_v_peek;
    _t778->gem_v_peek_at = gem_v_peek_at;
    _t778->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_20, _t778);
#line 643 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 647 "compiler/main.gem"
    struct _closure__anon_21 *_t786 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t786->gem_v_at_end = gem_v_at_end;
    _t786->gem_v_parse_stmt = gem_v_parse_stmt;
    _t786->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_21, _t786);
    GemVal _t787 = gem_table_new();
    gem_table_set(_t787, gem_string("parse"), gem_v_parse);
    return _t787;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
#line 10 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
    {
#line 11 "compiler/main.gem"
        GemVal gem_v__for_i_1 = gem_int(0);
#line 11 "compiler/main.gem"
    GemVal _t788[] = {gem_v_s};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t788, 1);
#line 11 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 11 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_1;
#line 11 "compiler/main.gem"
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 12 "compiler/main.gem"
            GemVal gem_v_ch = gem_table_get(gem_v_s, gem_v_i);
#line 13 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\\")))) {
#line 14 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_string("\\\\"));
            } else {
#line 15 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 16 "compiler/main.gem"
                    gem_v_result = gem_add(gem_v_result, gem_string("\\\""));
                } else {
#line 17 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 18 "compiler/main.gem"
                        gem_v_result = gem_add(gem_v_result, gem_string("\\n"));
                    } else {
#line 19 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\t")))) {
#line 20 "compiler/main.gem"
                            gem_v_result = gem_add(gem_v_result, gem_string("\\t"));
                        } else {
#line 22 "compiler/main.gem"
                            gem_v_result = gem_add(gem_v_result, gem_v_ch);
                        }
                    }
                }
            }
        }
    }
    return gem_v_result;
}

static GemVal gem_fn_repeat_str(void *_env, GemVal *args, int argc) {
#line 28 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
#line 29 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
    {
#line 30 "compiler/main.gem"
        GemVal gem_v__for_i_2 = gem_int(0);
#line 30 "compiler/main.gem"
        GemVal gem_v__for_limit_2 = gem_v_n;
#line 30 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 30 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_2;
#line 30 "compiler/main.gem"
            gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 31 "compiler/main.gem"
            gem_v_result = gem_add(gem_v_result, gem_v_s);
        }
    }
    return gem_v_result;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 36 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
#line 37 "compiler/main.gem"
    GemVal _t789[] = {gem_string("    "), gem_v_indent};
    return gem_fn_repeat_str(NULL, _t789, 2);
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 40 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
#line 41 "compiler/main.gem"
    GemVal _t790[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t790, 1);
#line 43 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
    {
#line 44 "compiler/main.gem"
        GemVal gem_v__for_i_3 = gem_int(0);
#line 44 "compiler/main.gem"
    GemVal _t791[] = {gem_v_s};
        GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t791, 1);
#line 44 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 44 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_3;
#line 44 "compiler/main.gem"
            gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 45 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_string(".")))) {
#line 46 "compiler/main.gem"
                gem_v_has_dot = gem_bool(1);
            }
        }
    }
#line 49 "compiler/main.gem"
    if (gem_truthy(gem_not(gem_v_has_dot))) {
#line 50 "compiler/main.gem"
        gem_v_s = gem_add(gem_v_s, gem_string(".0"));
    }
    return gem_v_s;
}

static GemVal gem_fn_array_contains(void *_env, GemVal *args, int argc) {
#line 57 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    {
#line 58 "compiler/main.gem"
        GemVal gem_v__for_i_4 = gem_int(0);
#line 58 "compiler/main.gem"
    GemVal _t792[] = {gem_v_arr};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t792, 1);
#line 58 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 58 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_4;
#line 58 "compiler/main.gem"
            gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 59 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_arr, gem_v_i), gem_v_val))) {
#line 60 "compiler/main.gem"
                return gem_bool(1);
            }
        }
    }
    return gem_bool(0);
}

static GemVal gem_fn_set_add(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
#line 67 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1)))) {
#line 68 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_val, gem_bool(1));
        return GEM_NIL;
    } else {
        return GEM_NIL;
    }
}

static GemVal gem_fn_set_contains(void *_env, GemVal *args, int argc) {
#line 72 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    return gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 76 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
#line 77 "compiler/main.gem"
    GemVal _t793 = gem_table_new();
    GemVal gem_v_result = _t793;
#line 78 "compiler/main.gem"
    GemVal _t794[] = {gem_v_a};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t794, 1);
#line 79 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 80 "compiler/main.gem"
    while (1) {
        GemVal _t795[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t795, 1)))) break;
#line 81 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
#line 82 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 84 "compiler/main.gem"
    GemVal _t796[] = {gem_v_b};
    GemVal gem_v_ks2 = gem_fn_keys(NULL, _t796, 1);
#line 85 "compiler/main.gem"
    GemVal gem_v_j = gem_int(0);
#line 86 "compiler/main.gem"
    while (1) {
        GemVal _t797[] = {gem_v_ks2};
        if (!gem_truthy(gem_lt(gem_v_j, gem_len_fn(NULL, _t797, 1)))) break;
#line 87 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks2, gem_v_j), gem_bool(1));
#line 88 "compiler/main.gem"
        gem_v_j = gem_add(gem_v_j, gem_int(1));
    }
    return gem_v_result;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
#line 94 "compiler/main.gem"
    GemVal _t798 = gem_table_new();
    GemVal gem_v_result = _t798;
#line 95 "compiler/main.gem"
    GemVal _t799[] = {gem_v_a};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t799, 1);
#line 96 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 97 "compiler/main.gem"
    while (1) {
        GemVal _t800[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t800, 1)))) break;
#line 98 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_table_get(gem_v_ks, gem_v_i)), gem_bool(1)))) {
#line 99 "compiler/main.gem"
            gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
        }
#line 101 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    return gem_v_result;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
#line 106 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
#line 107 "compiler/main.gem"
    GemVal _t801[] = {gem_v_s};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t801, 1);
#line 109 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 110 "compiler/main.gem"
    while (1) {
        GemVal _t802[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t802, 1)))) break;
#line 111 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
#line 112 "compiler/main.gem"
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
#line 113 "compiler/main.gem"
        while (1) {
            GemVal _t803;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t803 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t803 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t803)) break;
#line 114 "compiler/main.gem"
            gem_table_set(gem_v_ks, gem_add(gem_v_j, gem_int(1)), gem_table_get(gem_v_ks, gem_v_j));
#line 115 "compiler/main.gem"
            gem_v_j = gem_sub(gem_v_j, gem_int(1));
        }
#line 117 "compiler/main.gem"
        gem_table_set(gem_v_ks, gem_add(gem_v_j, gem_int(1)), gem_v_key);
#line 118 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    return gem_v_ks;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 123 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
#line 124 "compiler/main.gem"
    GemVal _t804 = gem_table_new();
    GemVal gem_v_s = _t804;
    {
#line 125 "compiler/main.gem"
        GemVal gem_v__for_items_5 = gem_v_arr;
#line 125 "compiler/main.gem"
        GemVal gem_v__for_i_5 = gem_int(0);
#line 125 "compiler/main.gem"
        while (1) {
            GemVal _t805[] = {gem_v__for_items_5};
            if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t805, 1)))) break;
#line 125 "compiler/main.gem"
            GemVal gem_v_item = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 125 "compiler/main.gem"
            gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 126 "compiler/main.gem"
            gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
        }
    }
    return gem_v_s;
}

struct _closure__anon_22 {
    GemVal *gem_v_tmp_counter;
};
static GemVal _anon_22(void *_env, GemVal *args, int argc) {
    struct _closure__anon_22 *_cls = (struct _closure__anon_22 *)_env;
    GemVal *gem_v_tmp_counter = _cls->gem_v_tmp_counter;
#line 153 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t811[] = {(*gem_v_tmp_counter)};
    return gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t811, 1));
}

struct _closure__anon_23 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
#line 158 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t813[] = {(*gem_v_anon_counter)};
    return gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t813, 1));
}

struct _closure__anon_24 {
};
static GemVal _anon_24(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t815[] = {gem_v_name};
    return gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t815, 1));
}

struct _closure__anon_25 {
    GemVal *gem_v_builtins;
    GemVal *gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_defined_fns;
};
static GemVal _anon_25(void *_env, GemVal *args, int argc) {
    struct _closure__anon_25 *_cls = (struct _closure__anon_25 *)_env;
    GemVal *gem_v_builtins = _cls->gem_v_builtins;
    GemVal *gem_v_collect_free_node = _cls->gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_defined = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_free = (argc > 2) ? args[2] : GEM_NIL;
#line 172 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 173 "compiler/main.gem"
        return GEM_NIL;
    }
#line 175 "compiler/main.gem"
    GemVal _t816[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t816, 1), gem_string("table")))) {
#line 176 "compiler/main.gem"
        return GEM_NIL;
    }
#line 178 "compiler/main.gem"
    GemVal _t817 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t817, gem_string("tag"));
#line 179 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 180 "compiler/main.gem"
        return GEM_NIL;
    }
#line 183 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 184 "compiler/main.gem"
    GemVal _t818 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t818, gem_string("name"));
#line 185 "compiler/main.gem"
    GemVal _t819[] = {gem_v_defined, gem_v_name};
    GemVal _t821;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t819, 2)))) {
        _t821 = gem_not(gem_fn_set_contains(NULL, _t819, 2));
    } else {
        GemVal _t820[] = {(*gem_v_builtins), gem_v_name};
        _t821 = gem_not(gem_fn_set_contains(NULL, _t820, 2));
    }
    GemVal _t823;
    if (!gem_truthy(_t821)) {
        _t823 = _t821;
    } else {
        GemVal _t822[] = {(*gem_v_defined_fns), gem_v_name};
        _t823 = gem_not(gem_fn_set_contains(NULL, _t822, 2));
    }
        if (gem_truthy(_t823)) {
#line 186 "compiler/main.gem"
    GemVal _t824[] = {gem_v_free, gem_v_name};
            return gem_fn_set_add(NULL, _t824, 2);
        } else {
            return GEM_NIL;
        }
    } else {
#line 188 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 189 "compiler/main.gem"
    GemVal _t825 = gem_v_node;
    GemVal _t826 = (*gem_v_collect_free_node);
    GemVal _t827[] = {gem_table_get(_t825, gem_string("value")), gem_v_defined, gem_v_free};
            return _t826.fn(_t826.env, _t827, 3);
        } else {
#line 190 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 191 "compiler/main.gem"
    GemVal _t828 = gem_v_node;
                GemVal gem_v_name = gem_table_get(_t828, gem_string("name"));
#line 192 "compiler/main.gem"
    GemVal _t829[] = {gem_v_defined, gem_v_name};
    GemVal _t831;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t829, 2)))) {
        _t831 = gem_not(gem_fn_set_contains(NULL, _t829, 2));
    } else {
        GemVal _t830[] = {(*gem_v_builtins), gem_v_name};
        _t831 = gem_not(gem_fn_set_contains(NULL, _t830, 2));
    }
    GemVal _t833;
    if (!gem_truthy(_t831)) {
        _t833 = _t831;
    } else {
        GemVal _t832[] = {(*gem_v_defined_fns), gem_v_name};
        _t833 = gem_not(gem_fn_set_contains(NULL, _t832, 2));
    }
                if (gem_truthy(_t833)) {
#line 193 "compiler/main.gem"
    GemVal _t834[] = {gem_v_free, gem_v_name};
                    (void)(gem_fn_set_add(NULL, _t834, 2));
                }
#line 195 "compiler/main.gem"
    GemVal _t835 = gem_v_node;
    GemVal _t836 = (*gem_v_collect_free_node);
    GemVal _t837[] = {gem_table_get(_t835, gem_string("value")), gem_v_defined, gem_v_free};
                return _t836.fn(_t836.env, _t837, 3);
            } else {
#line 196 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 197 "compiler/main.gem"
    GemVal _t838 = gem_v_node;
    GemVal _t839[] = {gem_table_get(_t838, gem_string("params"))};
    GemVal _t840[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t839, 1)};
                    GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t840, 2);
#line 198 "compiler/main.gem"
    GemVal _t841 = gem_v_node;
    GemVal _t842 = (*gem_v_collect_free_vars);
    GemVal _t843[] = {gem_table_get(_t841, gem_string("body")), gem_v_inner_defined};
                    GemVal gem_v_inner_free = _t842.fn(_t842.env, _t843, 2);
#line 199 "compiler/main.gem"
    GemVal _t844[] = {gem_v_inner_free};
                    GemVal gem_v_ifks = gem_fn_keys(NULL, _t844, 1);
#line 200 "compiler/main.gem"
                    GemVal gem_v_ifi = gem_int(0);
#line 201 "compiler/main.gem"
                    while (1) {
                        GemVal _t845[] = {gem_v_ifks};
                        if (!gem_truthy(gem_lt(gem_v_ifi, gem_len_fn(NULL, _t845, 1)))) break;
#line 202 "compiler/main.gem"
    GemVal _t846[] = {gem_v_free, gem_table_get(gem_v_ifks, gem_v_ifi)};
                        (void)(gem_fn_set_add(NULL, _t846, 2));
#line 203 "compiler/main.gem"
                        gem_v_ifi = gem_add(gem_v_ifi, gem_int(1));
                    }
                    return GEM_NIL;
                } else {
#line 205 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                        return GEM_NIL;
                    } else {
#line 207 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 208 "compiler/main.gem"
    GemVal _t847 = gem_v_node;
    GemVal _t848 = (*gem_v_collect_free_node);
    GemVal _t849[] = {gem_table_get(_t847, gem_string("cond")), gem_v_defined, gem_v_free};
                            (void)(_t848.fn(_t848.env, _t849, 3));
#line 209 "compiler/main.gem"
    GemVal _t850 = gem_table_new();
    GemVal _t851[] = {gem_v_defined, _t850};
                            GemVal gem_v_d = gem_fn_set_union(NULL, _t851, 2);
#line 210 "compiler/main.gem"
                            GemVal gem_v_thens = gem_table_get(gem_v_node, gem_string("then"));
                            {
#line 211 "compiler/main.gem"
                                GemVal gem_v__for_i_6 = gem_int(0);
#line 211 "compiler/main.gem"
    GemVal _t852[] = {gem_v_thens};
                                GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t852, 1);
#line 211 "compiler/main.gem"
                                while (1) {
                                    if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
#line 211 "compiler/main.gem"
                                    GemVal gem_v_i = gem_v__for_i_6;
#line 211 "compiler/main.gem"
                                    gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 212 "compiler/main.gem"
    GemVal _t853 = (*gem_v_collect_free_node);
    GemVal _t854[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
                                    (void)(_t853.fn(_t853.env, _t854, 3));
#line 213 "compiler/main.gem"
    GemVal _t855[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t857;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t855, 1), gem_string("table")))) {
        _t857 = gem_eq(gem_type_fn(NULL, _t855, 1), gem_string("table"));
    } else {
        GemVal _t856 = gem_table_get(gem_v_thens, gem_v_i);
        _t857 = gem_eq(gem_table_get(_t856, gem_string("tag")), gem_string("let"));
    }
                                    if (gem_truthy(_t857)) {
#line 214 "compiler/main.gem"
    GemVal _t858 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t859[] = {gem_v_d, gem_table_get(_t858, gem_string("name"))};
                                        (void)(gem_fn_set_add(NULL, _t859, 2));
                                    }
                                }
                            }
#line 217 "compiler/main.gem"
                            GemVal gem_v_el = gem_table_get(gem_v_node, gem_string("else"));
#line 218 "compiler/main.gem"
                            if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 219 "compiler/main.gem"
    GemVal _t860 = gem_table_new();
    GemVal _t861[] = {gem_v_defined, _t860};
                                gem_v_d = gem_fn_set_union(NULL, _t861, 2);
                                {
#line 220 "compiler/main.gem"
                                    GemVal gem_v__for_i_7 = gem_int(0);
#line 220 "compiler/main.gem"
    GemVal _t862[] = {gem_v_el};
                                    GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t862, 1);
#line 220 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 220 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_7;
#line 220 "compiler/main.gem"
                                        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 221 "compiler/main.gem"
    GemVal _t863 = (*gem_v_collect_free_node);
    GemVal _t864[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t863.fn(_t863.env, _t864, 3));
#line 222 "compiler/main.gem"
    GemVal _t865[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t867;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t865, 1), gem_string("table")))) {
        _t867 = gem_eq(gem_type_fn(NULL, _t865, 1), gem_string("table"));
    } else {
        GemVal _t866 = gem_table_get(gem_v_el, gem_v_i);
        _t867 = gem_eq(gem_table_get(_t866, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t867)) {
#line 223 "compiler/main.gem"
    GemVal _t868 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t869[] = {gem_v_d, gem_table_get(_t868, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t869, 2));
                                        }
                                    }
                                    return GEM_NIL;
                                }
                            } else {
                                return GEM_NIL;
                            }
                        } else {
#line 227 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 228 "compiler/main.gem"
    GemVal _t870 = gem_v_node;
    GemVal _t871 = (*gem_v_collect_free_node);
    GemVal _t872[] = {gem_table_get(_t870, gem_string("cond")), gem_v_defined, gem_v_free};
                                (void)(_t871.fn(_t871.env, _t872, 3));
#line 229 "compiler/main.gem"
    GemVal _t873 = gem_table_new();
    GemVal _t874[] = {gem_v_defined, _t873};
                                GemVal gem_v_d = gem_fn_set_union(NULL, _t874, 2);
                                {
#line 230 "compiler/main.gem"
                                    GemVal gem_v__for_i_8 = gem_int(0);
#line 230 "compiler/main.gem"
    GemVal _t875 = gem_v_node;
    GemVal _t876[] = {gem_table_get(_t875, gem_string("body"))};
                                    GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t876, 1);
#line 230 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 230 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_8;
#line 230 "compiler/main.gem"
                                        gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 231 "compiler/main.gem"
    GemVal _t877 = gem_v_node;
    GemVal _t878 = (*gem_v_collect_free_node);
    GemVal _t879[] = {gem_table_get(gem_table_get(_t877, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t878.fn(_t878.env, _t879, 3));
#line 232 "compiler/main.gem"
    GemVal _t880 = gem_v_node;
    GemVal _t881[] = {gem_table_get(gem_table_get(_t880, gem_string("body")), gem_v_i)};
    GemVal _t884;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t881, 1), gem_string("table")))) {
        _t884 = gem_eq(gem_type_fn(NULL, _t881, 1), gem_string("table"));
    } else {
        GemVal _t882 = gem_v_node;
        GemVal _t883 = gem_table_get(gem_table_get(_t882, gem_string("body")), gem_v_i);
        _t884 = gem_eq(gem_table_get(_t883, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t884)) {
#line 233 "compiler/main.gem"
    GemVal _t885 = gem_v_node;
    GemVal _t886 = gem_table_get(gem_table_get(_t885, gem_string("body")), gem_v_i);
    GemVal _t887[] = {gem_v_d, gem_table_get(_t886, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t887, 2));
                                        }
                                    }
                                    return GEM_NIL;
                                }
                            } else {
#line 236 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 237 "compiler/main.gem"
    GemVal _t888 = gem_v_node;
    GemVal _t889 = (*gem_v_collect_free_node);
    GemVal _t890[] = {gem_table_get(_t888, gem_string("target")), gem_v_defined, gem_v_free};
                                    (void)(_t889.fn(_t889.env, _t890, 3));
                                    {
#line 238 "compiler/main.gem"
                                        GemVal gem_v__for_i_10 = gem_int(0);
#line 238 "compiler/main.gem"
    GemVal _t891 = gem_v_node;
    GemVal _t892[] = {gem_table_get(_t891, gem_string("whens"))};
                                        GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t892, 1);
#line 238 "compiler/main.gem"
                                        while (1) {
                                            if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
#line 238 "compiler/main.gem"
                                            GemVal gem_v_i = gem_v__for_i_10;
#line 238 "compiler/main.gem"
                                            gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 239 "compiler/main.gem"
    GemVal _t893 = gem_v_node;
                                            GemVal gem_v_w = gem_table_get(gem_table_get(_t893, gem_string("whens")), gem_v_i);
#line 240 "compiler/main.gem"
    GemVal _t894 = gem_v_w;
    GemVal _t895 = (*gem_v_collect_free_node);
    GemVal _t896[] = {gem_table_get(_t894, gem_string("value")), gem_v_defined, gem_v_free};
                                            (void)(_t895.fn(_t895.env, _t896, 3));
#line 241 "compiler/main.gem"
    GemVal _t897 = gem_table_new();
    GemVal _t898[] = {gem_v_defined, _t897};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t898, 2);
                                            {
#line 242 "compiler/main.gem"
                                                GemVal gem_v__for_i_9 = gem_int(0);
#line 242 "compiler/main.gem"
    GemVal _t899 = gem_v_w;
    GemVal _t900[] = {gem_table_get(_t899, gem_string("body"))};
                                                GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t900, 1);
#line 242 "compiler/main.gem"
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 242 "compiler/main.gem"
                                                    GemVal gem_v_j = gem_v__for_i_9;
#line 242 "compiler/main.gem"
                                                    gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 243 "compiler/main.gem"
    GemVal _t901 = gem_v_w;
    GemVal _t902 = (*gem_v_collect_free_node);
    GemVal _t903[] = {gem_table_get(gem_table_get(_t901, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
                                                    (void)(_t902.fn(_t902.env, _t903, 3));
#line 244 "compiler/main.gem"
    GemVal _t904 = gem_v_w;
    GemVal _t905[] = {gem_table_get(gem_table_get(_t904, gem_string("body")), gem_v_j)};
    GemVal _t908;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t905, 1), gem_string("table")))) {
        _t908 = gem_eq(gem_type_fn(NULL, _t905, 1), gem_string("table"));
    } else {
        GemVal _t906 = gem_v_w;
        GemVal _t907 = gem_table_get(gem_table_get(_t906, gem_string("body")), gem_v_j);
        _t908 = gem_eq(gem_table_get(_t907, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t908)) {
#line 245 "compiler/main.gem"
    GemVal _t909 = gem_v_w;
    GemVal _t910 = gem_table_get(gem_table_get(_t909, gem_string("body")), gem_v_j);
    GemVal _t911[] = {gem_v_d, gem_table_get(_t910, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t911, 2));
                                                    }
                                                }
                                            }
                                        }
                                    }
#line 249 "compiler/main.gem"
                                    GemVal gem_v_el = gem_table_get(gem_v_node, gem_string("else"));
#line 250 "compiler/main.gem"
                                    if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 251 "compiler/main.gem"
    GemVal _t912 = gem_table_new();
    GemVal _t913[] = {gem_v_defined, _t912};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t913, 2);
                                        {
#line 252 "compiler/main.gem"
                                            GemVal gem_v__for_i_11 = gem_int(0);
#line 252 "compiler/main.gem"
    GemVal _t914[] = {gem_v_el};
                                            GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t914, 1);
#line 252 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
#line 252 "compiler/main.gem"
                                                GemVal gem_v_j = gem_v__for_i_11;
#line 252 "compiler/main.gem"
                                                gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 253 "compiler/main.gem"
    GemVal _t915 = (*gem_v_collect_free_node);
    GemVal _t916[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
                                                (void)(_t915.fn(_t915.env, _t916, 3));
#line 254 "compiler/main.gem"
    GemVal _t917[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t919;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t917, 1), gem_string("table")))) {
        _t919 = gem_eq(gem_type_fn(NULL, _t917, 1), gem_string("table"));
    } else {
        GemVal _t918 = gem_table_get(gem_v_el, gem_v_j);
        _t919 = gem_eq(gem_table_get(_t918, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t919)) {
#line 255 "compiler/main.gem"
    GemVal _t920 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t921[] = {gem_v_d, gem_table_get(_t920, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t921, 2));
                                                }
                                            }
                                            return GEM_NIL;
                                        }
                                    } else {
                                        return GEM_NIL;
                                    }
                                } else {
#line 259 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("when_clause")))) {
#line 260 "compiler/main.gem"
    GemVal _t922 = gem_v_node;
    GemVal _t923 = (*gem_v_collect_free_node);
    GemVal _t924[] = {gem_table_get(_t922, gem_string("value")), gem_v_defined, gem_v_free};
                                        (void)(_t923.fn(_t923.env, _t924, 3));
#line 261 "compiler/main.gem"
    GemVal _t925 = gem_table_new();
    GemVal _t926[] = {gem_v_defined, _t925};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t926, 2);
                                        {
#line 262 "compiler/main.gem"
                                            GemVal gem_v__for_i_12 = gem_int(0);
#line 262 "compiler/main.gem"
    GemVal _t927 = gem_v_node;
    GemVal _t928[] = {gem_table_get(_t927, gem_string("body"))};
                                            GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t928, 1);
#line 262 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 262 "compiler/main.gem"
                                                GemVal gem_v_i = gem_v__for_i_12;
#line 262 "compiler/main.gem"
                                                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 263 "compiler/main.gem"
    GemVal _t929 = gem_v_node;
    GemVal _t930 = (*gem_v_collect_free_node);
    GemVal _t931[] = {gem_table_get(gem_table_get(_t929, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                                (void)(_t930.fn(_t930.env, _t931, 3));
#line 264 "compiler/main.gem"
    GemVal _t932 = gem_v_node;
    GemVal _t933[] = {gem_table_get(gem_table_get(_t932, gem_string("body")), gem_v_i)};
    GemVal _t936;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t933, 1), gem_string("table")))) {
        _t936 = gem_eq(gem_type_fn(NULL, _t933, 1), gem_string("table"));
    } else {
        GemVal _t934 = gem_v_node;
        GemVal _t935 = gem_table_get(gem_table_get(_t934, gem_string("body")), gem_v_i);
        _t936 = gem_eq(gem_table_get(_t935, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t936)) {
#line 265 "compiler/main.gem"
    GemVal _t937 = gem_v_node;
    GemVal _t938 = gem_table_get(gem_table_get(_t937, gem_string("body")), gem_v_i);
    GemVal _t939[] = {gem_v_d, gem_table_get(_t938, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t939, 2));
                                                }
                                            }
                                            return GEM_NIL;
                                        }
                                    } else {
#line 268 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 269 "compiler/main.gem"
    GemVal _t940 = gem_table_new();
    GemVal _t941[] = {gem_v_defined, _t940};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t941, 2);
                                            {
#line 270 "compiler/main.gem"
                                                GemVal gem_v__for_i_13 = gem_int(0);
#line 270 "compiler/main.gem"
    GemVal _t942 = gem_v_node;
    GemVal _t943[] = {gem_table_get(_t942, gem_string("stmts"))};
                                                GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t943, 1);
#line 270 "compiler/main.gem"
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 270 "compiler/main.gem"
                                                    GemVal gem_v_i = gem_v__for_i_13;
#line 270 "compiler/main.gem"
                                                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 271 "compiler/main.gem"
    GemVal _t944 = gem_v_node;
    GemVal _t945 = (*gem_v_collect_free_node);
    GemVal _t946[] = {gem_table_get(gem_table_get(_t944, gem_string("stmts")), gem_v_i), gem_v_d, gem_v_free};
                                                    (void)(_t945.fn(_t945.env, _t946, 3));
#line 272 "compiler/main.gem"
    GemVal _t947 = gem_v_node;
    GemVal _t948[] = {gem_table_get(gem_table_get(_t947, gem_string("stmts")), gem_v_i)};
    GemVal _t951;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t948, 1), gem_string("table")))) {
        _t951 = gem_eq(gem_type_fn(NULL, _t948, 1), gem_string("table"));
    } else {
        GemVal _t949 = gem_v_node;
        GemVal _t950 = gem_table_get(gem_table_get(_t949, gem_string("stmts")), gem_v_i);
        _t951 = gem_eq(gem_table_get(_t950, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t951)) {
#line 273 "compiler/main.gem"
    GemVal _t952 = gem_v_node;
    GemVal _t953 = gem_table_get(gem_table_get(_t952, gem_string("stmts")), gem_v_i);
    GemVal _t954[] = {gem_v_d, gem_table_get(_t953, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t954, 2));
                                                    }
                                                }
                                                return GEM_NIL;
                                            }
                                        } else {
#line 276 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 277 "compiler/main.gem"
    GemVal _t955 = gem_v_node;
                                                if (gem_truthy(gem_neq(gem_table_get(_t955, gem_string("value")), GEM_NIL))) {
#line 278 "compiler/main.gem"
    GemVal _t956 = gem_v_node;
    GemVal _t957 = (*gem_v_collect_free_node);
    GemVal _t958[] = {gem_table_get(_t956, gem_string("value")), gem_v_defined, gem_v_free};
                                                    return _t957.fn(_t957.env, _t958, 3);
                                                } else {
                                                    return GEM_NIL;
                                                }
                                            } else {
#line 282 "compiler/main.gem"
    GemVal _t959[] = {gem_v_node};
                                                GemVal gem_v_nks = gem_fn_keys(NULL, _t959, 1);
#line 283 "compiler/main.gem"
                                                GemVal gem_v_nki = gem_int(0);
#line 284 "compiler/main.gem"
                                                while (1) {
                                                    GemVal _t960[] = {gem_v_nks};
                                                    if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t960, 1)))) break;
#line 285 "compiler/main.gem"
                                                    if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
#line 286 "compiler/main.gem"
                                                        GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
#line 287 "compiler/main.gem"
    GemVal _t961[] = {gem_v_v};
                                                        if (gem_truthy(gem_eq(gem_type_fn(NULL, _t961, 1), gem_string("table")))) {
#line 289 "compiler/main.gem"
    GemVal _t962[] = {gem_v_v};
                                                            GemVal gem_v_vks = gem_fn_keys(NULL, _t962, 1);
#line 290 "compiler/main.gem"
    GemVal _t963[] = {gem_v_vks};
    GemVal _t965;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t963, 1), gem_int(0)))) {
        _t965 = gem_gt(gem_len_fn(NULL, _t963, 1), gem_int(0));
    } else {
        GemVal _t964[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t965 = gem_eq(gem_type_fn(NULL, _t964, 1), gem_string("int"));
    }
                                                            if (gem_truthy(_t965)) {
#line 292 "compiler/main.gem"
                                                                GemVal gem_v_vi = gem_int(0);
#line 293 "compiler/main.gem"
                                                                while (1) {
                                                                    GemVal _t966[] = {gem_v_v};
                                                                    if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t966, 1)))) break;
#line 294 "compiler/main.gem"
    GemVal _t967[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t967, 1), gem_string("table")))) {
#line 295 "compiler/main.gem"
    GemVal _t968 = (*gem_v_collect_free_node);
    GemVal _t969[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
                                                                        (void)(_t968.fn(_t968.env, _t969, 3));
                                                                    }
#line 297 "compiler/main.gem"
                                                                    gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                                }
                                                            } else {
#line 300 "compiler/main.gem"
    GemVal _t970 = (*gem_v_collect_free_node);
    GemVal _t971[] = {gem_v_v, gem_v_defined, gem_v_free};
                                                                (void)(_t970.fn(_t970.env, _t971, 3));
                                                            }
                                                        }
                                                    }
#line 304 "compiler/main.gem"
                                                    gem_v_nki = gem_add(gem_v_nki, gem_int(1));
                                                }
                                                return GEM_NIL;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

struct _closure__anon_26 {
    GemVal *gem_v_collect_free_node;
};
static GemVal _anon_26(void *_env, GemVal *args, int argc) {
    struct _closure__anon_26 *_cls = (struct _closure__anon_26 *)_env;
    GemVal *gem_v_collect_free_node = _cls->gem_v_collect_free_node;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_defined = (argc > 1) ? args[1] : GEM_NIL;
#line 310 "compiler/main.gem"
    GemVal _t973 = gem_table_new();
    GemVal gem_v_free = _t973;
#line 311 "compiler/main.gem"
    GemVal _t974 = gem_table_new();
    GemVal _t975[] = {gem_v_defined, _t974};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t975, 2);
    {
#line 312 "compiler/main.gem"
        GemVal gem_v__for_i_15 = gem_int(0);
#line 312 "compiler/main.gem"
    GemVal _t976[] = {gem_v_stmts};
        GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t976, 1);
#line 312 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 312 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_15;
#line 312 "compiler/main.gem"
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 313 "compiler/main.gem"
    GemVal _t977 = (*gem_v_collect_free_node);
    GemVal _t978[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
            (void)(_t977.fn(_t977.env, _t978, 3));
#line 314 "compiler/main.gem"
    GemVal _t979[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t981;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t979, 1), gem_string("table")))) {
        _t981 = gem_eq(gem_type_fn(NULL, _t979, 1), gem_string("table"));
    } else {
        GemVal _t980 = gem_table_get(gem_v_stmts, gem_v_i);
        _t981 = gem_eq(gem_table_get(_t980, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t981)) {
#line 315 "compiler/main.gem"
    GemVal _t982 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t983[] = {gem_v_d, gem_table_get(_t982, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t983, 2));
            }
#line 317 "compiler/main.gem"
    GemVal _t984[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t986;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t984, 1), gem_string("table")))) {
        _t986 = gem_eq(gem_type_fn(NULL, _t984, 1), gem_string("table"));
    } else {
        GemVal _t985 = gem_table_get(gem_v_stmts, gem_v_i);
        _t986 = gem_eq(gem_table_get(_t985, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t986)) {
                {
#line 318 "compiler/main.gem"
                    GemVal gem_v__for_i_14 = gem_int(0);
#line 318 "compiler/main.gem"
    GemVal _t987 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t988[] = {gem_table_get(_t987, gem_string("stmts"))};
                    GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t988, 1);
#line 318 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 318 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_14;
#line 318 "compiler/main.gem"
                        gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 319 "compiler/main.gem"
    GemVal _t989 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t990[] = {gem_table_get(gem_table_get(_t989, gem_string("stmts")), gem_v_j)};
    GemVal _t993;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t990, 1), gem_string("table")))) {
        _t993 = gem_eq(gem_type_fn(NULL, _t990, 1), gem_string("table"));
    } else {
        GemVal _t991 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t992 = gem_table_get(gem_table_get(_t991, gem_string("stmts")), gem_v_j);
        _t993 = gem_eq(gem_table_get(_t992, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t993)) {
#line 320 "compiler/main.gem"
    GemVal _t994 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t995 = gem_table_get(gem_table_get(_t994, gem_string("stmts")), gem_v_j);
    GemVal _t996[] = {gem_v_d, gem_table_get(_t995, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t996, 2));
                        }
                    }
                }
            }
        }
    }
    return gem_v_free;
}

struct _closure__anon_27 {
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_walk_captures;
    GemVal *gem_v_walk_captures_node;
};
static GemVal _anon_27(void *_env, GemVal *args, int argc) {
    struct _closure__anon_27 *_cls = (struct _closure__anon_27 *)_env;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_walk_captures = _cls->gem_v_walk_captures;
    GemVal *gem_v_walk_captures_node = _cls->gem_v_walk_captures_node;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_scope_vars = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_captured = (argc > 2) ? args[2] : GEM_NIL;
#line 334 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 335 "compiler/main.gem"
        return GEM_NIL;
    }
#line 337 "compiler/main.gem"
    GemVal _t998[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t998, 1), gem_string("table")))) {
#line 338 "compiler/main.gem"
        return GEM_NIL;
    }
#line 340 "compiler/main.gem"
    GemVal _t999 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t999, gem_string("tag"));
#line 341 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 342 "compiler/main.gem"
        return GEM_NIL;
    }
#line 345 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 346 "compiler/main.gem"
    GemVal _t1000 = gem_v_node;
    GemVal _t1001[] = {gem_table_get(_t1000, gem_string("params"))};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1001, 1);
#line 347 "compiler/main.gem"
    GemVal _t1002 = gem_v_node;
    GemVal _t1003 = (*gem_v_collect_free_vars);
    GemVal _t1004[] = {gem_table_get(_t1002, gem_string("body")), gem_v_inner_defined};
        GemVal gem_v_free = _t1003.fn(_t1003.env, _t1004, 2);
#line 348 "compiler/main.gem"
    GemVal _t1005[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t1005, 2);
#line 349 "compiler/main.gem"
    GemVal _t1006[] = {gem_v_inter};
        GemVal gem_v_iks = gem_fn_keys(NULL, _t1006, 1);
#line 350 "compiler/main.gem"
        GemVal gem_v_iki = gem_int(0);
#line 351 "compiler/main.gem"
        while (1) {
            GemVal _t1007[] = {gem_v_iks};
            if (!gem_truthy(gem_lt(gem_v_iki, gem_len_fn(NULL, _t1007, 1)))) break;
#line 352 "compiler/main.gem"
    GemVal _t1008[] = {gem_v_captured, gem_table_get(gem_v_iks, gem_v_iki)};
            (void)(gem_fn_set_add(NULL, _t1008, 2));
#line 353 "compiler/main.gem"
            gem_v_iki = gem_add(gem_v_iki, gem_int(1));
        }
#line 355 "compiler/main.gem"
    GemVal _t1009 = gem_v_node;
    GemVal _t1010 = gem_v_node;
    GemVal _t1011[] = {gem_table_get(_t1010, gem_string("params"))};
    GemVal _t1012[] = {gem_v_scope_vars, gem_fn_sorted_array_to_set(NULL, _t1011, 1)};
    GemVal _t1013 = (*gem_v_walk_captures);
    GemVal _t1014[] = {gem_table_get(_t1009, gem_string("body")), gem_fn_set_union(NULL, _t1012, 2), gem_v_captured};
        return _t1013.fn(_t1013.env, _t1014, 3);
    } else {
#line 356 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
            return GEM_NIL;
        } else {
#line 358 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 359 "compiler/main.gem"
    GemVal _t1015 = gem_v_node;
    GemVal _t1016 = (*gem_v_walk_captures_node);
    GemVal _t1017[] = {gem_table_get(_t1015, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1016.fn(_t1016.env, _t1017, 3));
#line 360 "compiler/main.gem"
    GemVal _t1018 = (*gem_v_walk_captures);
    GemVal _t1019[] = {gem_table_get(gem_v_node, gem_string("then")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1018.fn(_t1018.env, _t1019, 3));
#line 361 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 362 "compiler/main.gem"
    GemVal _t1020 = (*gem_v_walk_captures);
    GemVal _t1021[] = {gem_table_get(gem_v_node, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                    return _t1020.fn(_t1020.env, _t1021, 3);
                } else {
                    return GEM_NIL;
                }
            } else {
#line 364 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 365 "compiler/main.gem"
    GemVal _t1022 = gem_v_node;
    GemVal _t1023 = (*gem_v_walk_captures_node);
    GemVal _t1024[] = {gem_table_get(_t1022, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                    (void)(_t1023.fn(_t1023.env, _t1024, 3));
#line 366 "compiler/main.gem"
    GemVal _t1025 = gem_v_node;
    GemVal _t1026 = (*gem_v_walk_captures);
    GemVal _t1027[] = {gem_table_get(_t1025, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                    return _t1026.fn(_t1026.env, _t1027, 3);
                } else {
#line 367 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 368 "compiler/main.gem"
    GemVal _t1028 = gem_v_node;
    GemVal _t1029 = (*gem_v_walk_captures_node);
    GemVal _t1030[] = {gem_table_get(_t1028, gem_string("target")), gem_v_scope_vars, gem_v_captured};
                        (void)(_t1029.fn(_t1029.env, _t1030, 3));
                        {
#line 369 "compiler/main.gem"
                            GemVal gem_v__for_i_16 = gem_int(0);
#line 369 "compiler/main.gem"
    GemVal _t1031 = gem_v_node;
    GemVal _t1032[] = {gem_table_get(_t1031, gem_string("whens"))};
                            GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1032, 1);
#line 369 "compiler/main.gem"
                            while (1) {
                                if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 369 "compiler/main.gem"
                                GemVal gem_v_i = gem_v__for_i_16;
#line 369 "compiler/main.gem"
                                gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 370 "compiler/main.gem"
    GemVal _t1033 = gem_v_node;
    GemVal _t1034 = gem_table_get(gem_table_get(_t1033, gem_string("whens")), gem_v_i);
    GemVal _t1035 = (*gem_v_walk_captures_node);
    GemVal _t1036[] = {gem_table_get(_t1034, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t1035.fn(_t1035.env, _t1036, 3));
#line 371 "compiler/main.gem"
    GemVal _t1037 = gem_v_node;
    GemVal _t1038 = gem_table_get(gem_table_get(_t1037, gem_string("whens")), gem_v_i);
    GemVal _t1039 = (*gem_v_walk_captures);
    GemVal _t1040[] = {gem_table_get(_t1038, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t1039.fn(_t1039.env, _t1040, 3));
                            }
                        }
#line 373 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 374 "compiler/main.gem"
    GemVal _t1041 = (*gem_v_walk_captures);
    GemVal _t1042[] = {gem_table_get(gem_v_node, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                            return _t1041.fn(_t1041.env, _t1042, 3);
                        } else {
                            return GEM_NIL;
                        }
                    } else {
#line 376 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 377 "compiler/main.gem"
    GemVal _t1043 = gem_v_node;
    GemVal _t1044 = (*gem_v_walk_captures);
    GemVal _t1045[] = {gem_table_get(_t1043, gem_string("stmts")), gem_v_scope_vars, gem_v_captured};
                            return _t1044.fn(_t1044.env, _t1045, 3);
                        } else {
#line 378 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 379 "compiler/main.gem"
    GemVal _t1046 = gem_v_node;
                                if (gem_truthy(gem_neq(gem_table_get(_t1046, gem_string("value")), GEM_NIL))) {
#line 380 "compiler/main.gem"
    GemVal _t1047 = gem_v_node;
    GemVal _t1048 = (*gem_v_walk_captures_node);
    GemVal _t1049[] = {gem_table_get(_t1047, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    return _t1048.fn(_t1048.env, _t1049, 3);
                                } else {
                                    return GEM_NIL;
                                }
                            } else {
#line 382 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 383 "compiler/main.gem"
    GemVal _t1050 = gem_v_node;
    GemVal _t1051 = (*gem_v_walk_captures_node);
    GemVal _t1052[] = {gem_table_get(_t1050, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    return _t1051.fn(_t1051.env, _t1052, 3);
                                } else {
#line 384 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 385 "compiler/main.gem"
    GemVal _t1053 = gem_v_node;
    GemVal _t1054 = (*gem_v_walk_captures_node);
    GemVal _t1055[] = {gem_table_get(_t1053, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                        return _t1054.fn(_t1054.env, _t1055, 3);
                                    } else {
#line 388 "compiler/main.gem"
    GemVal _t1056[] = {gem_v_node};
                                        GemVal gem_v_nks = gem_fn_keys(NULL, _t1056, 1);
#line 389 "compiler/main.gem"
                                        GemVal gem_v_nki = gem_int(0);
#line 390 "compiler/main.gem"
                                        while (1) {
                                            GemVal _t1057[] = {gem_v_nks};
                                            if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t1057, 1)))) break;
#line 391 "compiler/main.gem"
                                            if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
#line 392 "compiler/main.gem"
                                                GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
#line 393 "compiler/main.gem"
    GemVal _t1058[] = {gem_v_v};
                                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1058, 1), gem_string("table")))) {
#line 394 "compiler/main.gem"
    GemVal _t1059[] = {gem_v_v};
                                                    GemVal gem_v_vks = gem_fn_keys(NULL, _t1059, 1);
#line 395 "compiler/main.gem"
    GemVal _t1060[] = {gem_v_vks};
    GemVal _t1062;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1060, 1), gem_int(0)))) {
        _t1062 = gem_gt(gem_len_fn(NULL, _t1060, 1), gem_int(0));
    } else {
        GemVal _t1061[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1062 = gem_eq(gem_type_fn(NULL, _t1061, 1), gem_string("int"));
    }
                                                    if (gem_truthy(_t1062)) {
#line 396 "compiler/main.gem"
                                                        GemVal gem_v_vi = gem_int(0);
#line 397 "compiler/main.gem"
                                                        while (1) {
                                                            GemVal _t1063[] = {gem_v_v};
                                                            if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1063, 1)))) break;
#line 398 "compiler/main.gem"
    GemVal _t1064[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                            if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1064, 1), gem_string("table")))) {
#line 399 "compiler/main.gem"
    GemVal _t1065 = (*gem_v_walk_captures_node);
    GemVal _t1066[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
                                                                (void)(_t1065.fn(_t1065.env, _t1066, 3));
                                                            }
#line 401 "compiler/main.gem"
                                                            gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                        }
                                                    } else {
#line 404 "compiler/main.gem"
    GemVal _t1067 = (*gem_v_walk_captures_node);
    GemVal _t1068[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
                                                        (void)(_t1067.fn(_t1067.env, _t1068, 3));
                                                    }
                                                }
                                            }
#line 408 "compiler/main.gem"
                                            gem_v_nki = gem_add(gem_v_nki, gem_int(1));
                                        }
                                        return GEM_NIL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

struct _closure__anon_28 {
    GemVal *gem_v_walk_captures_node;
};
static GemVal _anon_28(void *_env, GemVal *args, int argc) {
    struct _closure__anon_28 *_cls = (struct _closure__anon_28 *)_env;
    GemVal *gem_v_walk_captures_node = _cls->gem_v_walk_captures_node;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_scope_vars = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_captured = (argc > 2) ? args[2] : GEM_NIL;
#line 414 "compiler/main.gem"
    GemVal _t1070 = gem_table_new();
    GemVal _t1071[] = {gem_v_scope_vars, _t1070};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t1071, 2);
    {
#line 415 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 415 "compiler/main.gem"
    GemVal _t1072[] = {gem_v_stmts};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1072, 1);
#line 415 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 415 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_18;
#line 415 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 416 "compiler/main.gem"
    GemVal _t1073 = (*gem_v_walk_captures_node);
    GemVal _t1074[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
            (void)(_t1073.fn(_t1073.env, _t1074, 3));
#line 417 "compiler/main.gem"
    GemVal _t1075[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1077;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1075, 1), gem_string("table")))) {
        _t1077 = gem_eq(gem_type_fn(NULL, _t1075, 1), gem_string("table"));
    } else {
        GemVal _t1076 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1077 = gem_eq(gem_table_get(_t1076, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1077)) {
#line 418 "compiler/main.gem"
    GemVal _t1078 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1079[] = {gem_v_sv, gem_table_get(_t1078, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1079, 2));
            }
#line 420 "compiler/main.gem"
    GemVal _t1080[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1082;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1080, 1), gem_string("table")))) {
        _t1082 = gem_eq(gem_type_fn(NULL, _t1080, 1), gem_string("table"));
    } else {
        GemVal _t1081 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1082 = gem_eq(gem_table_get(_t1081, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1082)) {
                {
#line 421 "compiler/main.gem"
                    GemVal gem_v__for_i_17 = gem_int(0);
#line 421 "compiler/main.gem"
    GemVal _t1083 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1084[] = {gem_table_get(_t1083, gem_string("stmts"))};
                    GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1084, 1);
#line 421 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 421 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_17;
#line 421 "compiler/main.gem"
                        gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 422 "compiler/main.gem"
    GemVal _t1085 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1086[] = {gem_table_get(gem_table_get(_t1085, gem_string("stmts")), gem_v_j)};
    GemVal _t1089;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1086, 1), gem_string("table")))) {
        _t1089 = gem_eq(gem_type_fn(NULL, _t1086, 1), gem_string("table"));
    } else {
        GemVal _t1087 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1088 = gem_table_get(gem_table_get(_t1087, gem_string("stmts")), gem_v_j);
        _t1089 = gem_eq(gem_table_get(_t1088, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1089)) {
#line 423 "compiler/main.gem"
    GemVal _t1090 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1091 = gem_table_get(gem_table_get(_t1090, gem_string("stmts")), gem_v_j);
    GemVal _t1092[] = {gem_v_sv, gem_table_get(_t1091, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1092, 2));
                        }
                    }
                }
            }
        }
        return GEM_NIL;
    }
}

struct _closure__anon_29 {
    GemVal *gem_v_walk_captures;
};
static GemVal _anon_29(void *_env, GemVal *args, int argc) {
    struct _closure__anon_29 *_cls = (struct _closure__anon_29 *)_env;
    GemVal *gem_v_walk_captures = _cls->gem_v_walk_captures;
    GemVal gem_v_body = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_scope_vars = (argc > 1) ? args[1] : GEM_NIL;
#line 431 "compiler/main.gem"
    GemVal _t1094 = gem_table_new();
    GemVal gem_v_captured = _t1094;
#line 432 "compiler/main.gem"
    GemVal _t1095 = (*gem_v_walk_captures);
    GemVal _t1096[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    (void)(_t1095.fn(_t1095.env, _t1096, 3));
    return gem_v_captured;
}

struct _closure__anon_30 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_anon_fn;
    GemVal *gem_v_compile_array;
    GemVal *gem_v_compile_binop;
    GemVal *gem_v_compile_call;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_table;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_30(void *_env, GemVal *args, int argc) {
    struct _closure__anon_30 *_cls = (struct _closure__anon_30 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_anon_fn = _cls->gem_v_compile_anon_fn;
    GemVal *gem_v_compile_array = _cls->gem_v_compile_array;
    GemVal *gem_v_compile_binop = _cls->gem_v_compile_binop;
    GemVal *gem_v_compile_call = _cls->gem_v_compile_call;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_table = _cls->gem_v_compile_table;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
#line 459 "compiler/main.gem"
    GemVal _t1098 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1098, gem_string("tag"));
#line 461 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("int")))) {
#line 462 "compiler/main.gem"
    GemVal _t1099 = gem_table_new();
    GemVal _t1100 = gem_v_node;
    GemVal _t1101[] = {gem_table_get(_t1100, gem_string("value"))};
    gem_table_set(_t1099, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t1101, 1)), gem_string(")")));
    gem_table_set(_t1099, gem_string("setup"), gem_string(""));
        return _t1099;
    } else {
#line 463 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("float")))) {
#line 464 "compiler/main.gem"
    GemVal _t1102 = gem_table_new();
    GemVal _t1103 = gem_v_node;
    GemVal _t1104[] = {gem_table_get(_t1103, gem_string("value"))};
    GemVal _t1105[] = {gem_fn_format_float(NULL, _t1104, 1)};
    gem_table_set(_t1102, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t1105, 1)), gem_string(")")));
    gem_table_set(_t1102, gem_string("setup"), gem_string(""));
            return _t1102;
        } else {
#line 465 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("string")))) {
#line 466 "compiler/main.gem"
    GemVal _t1106 = gem_v_node;
    GemVal _t1107[] = {gem_table_get(_t1106, gem_string("value"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1107, 1);
#line 467 "compiler/main.gem"
    GemVal _t1108 = gem_table_new();
    GemVal _t1109[] = {gem_v_escaped};
    gem_table_set(_t1108, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t1109, 1)), gem_string("\")")));
    gem_table_set(_t1108, gem_string("setup"), gem_string(""));
                return _t1108;
            } else {
#line 468 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("interp")))) {
#line 469 "compiler/main.gem"
    GemVal _t1110 = gem_v_node;
                    GemVal gem_v_parts = gem_table_get(_t1110, gem_string("parts"));
#line 470 "compiler/main.gem"
    GemVal _t1111[] = {gem_v_parts};
                    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1111, 1), gem_int(0)))) {
#line 471 "compiler/main.gem"
    GemVal _t1112 = gem_table_new();
    gem_table_set(_t1112, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t1112, gem_string("setup"), gem_string(""));
                        return _t1112;
                    }
#line 473 "compiler/main.gem"
                    GemVal gem_v_setup = gem_string("");
#line 474 "compiler/main.gem"
    GemVal _t1113 = gem_table_new();
                    GemVal gem_v_compiled = _t1113;
                    {
#line 475 "compiler/main.gem"
                        GemVal gem_v__for_i_19 = gem_int(0);
#line 475 "compiler/main.gem"
    GemVal _t1114[] = {gem_v_parts};
                        GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1114, 1);
#line 475 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 475 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_19;
#line 475 "compiler/main.gem"
                            gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 476 "compiler/main.gem"
                            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 477 "compiler/main.gem"
    GemVal _t1115 = (*gem_v_compile_expr);
    GemVal _t1116[] = {gem_v_part};
                            GemVal gem_v_r = _t1115.fn(_t1115.env, _t1116, 1);
#line 478 "compiler/main.gem"
    GemVal _t1117 = gem_v_r;
                            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1117, gem_string("setup")));
#line 479 "compiler/main.gem"
    GemVal _t1118 = gem_v_part;
                            if (gem_truthy(gem_eq(gem_table_get(_t1118, gem_string("tag")), gem_string("string")))) {
#line 480 "compiler/main.gem"
    GemVal _t1119 = gem_v_r;
    GemVal _t1120[] = {gem_v_compiled, gem_table_get(_t1119, gem_string("expr"))};
                                (void)(gem_push_fn(NULL, _t1120, 2));
                            } else {
#line 482 "compiler/main.gem"
    GemVal _t1121 = (*gem_v_tmp);
                                GemVal gem_v_ts = _t1121.fn(_t1121.env, NULL, 0);
#line 483 "compiler/main.gem"
    GemVal _t1122[] = {gem_v_ts};
    GemVal _t1123 = gem_v_r;
    GemVal _t1124[] = {gem_table_get(_t1123, gem_string("expr"))};
                                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1122, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1124, 1)), gem_string("};\n")));
#line 484 "compiler/main.gem"
    GemVal _t1125[] = {gem_v_ts};
    GemVal _t1126[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t1125, 1)), gem_string(", 1)"))};
                                (void)(gem_push_fn(NULL, _t1126, 2));
                            }
                        }
                    }
#line 487 "compiler/main.gem"
                    GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
                    {
#line 488 "compiler/main.gem"
                        GemVal gem_v__for_i_20 = gem_int(1);
#line 488 "compiler/main.gem"
    GemVal _t1127[] = {gem_v_compiled};
                        GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1127, 1);
#line 488 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 488 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_20;
#line 488 "compiler/main.gem"
                            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 489 "compiler/main.gem"
    GemVal _t1128[] = {gem_v_acc};
    GemVal _t1129[] = {gem_table_get(gem_v_compiled, gem_v_i)};
                            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1128, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1129, 1)), gem_string(")"));
                        }
                    }
#line 491 "compiler/main.gem"
    GemVal _t1130 = gem_table_new();
    gem_table_set(_t1130, gem_string("expr"), gem_v_acc);
    gem_table_set(_t1130, gem_string("setup"), gem_v_setup);
                    return _t1130;
                } else {
#line 492 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("bool")))) {
#line 493 "compiler/main.gem"
    GemVal _t1131 = gem_v_node;
                        if (gem_truthy(gem_table_get(_t1131, gem_string("value")))) {
#line 494 "compiler/main.gem"
    GemVal _t1132 = gem_table_new();
    gem_table_set(_t1132, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t1132, gem_string("setup"), gem_string(""));
                            return _t1132;
                        }
#line 496 "compiler/main.gem"
    GemVal _t1133 = gem_table_new();
    gem_table_set(_t1133, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t1133, gem_string("setup"), gem_string(""));
                        return _t1133;
                    } else {
#line 497 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("nil")))) {
#line 498 "compiler/main.gem"
    GemVal _t1134 = gem_table_new();
    gem_table_set(_t1134, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1134, gem_string("setup"), gem_string(""));
                            return _t1134;
                        } else {
#line 499 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 500 "compiler/main.gem"
    GemVal _t1135 = gem_v_node;
                                GemVal gem_v_name = gem_table_get(_t1135, gem_string("name"));
#line 501 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
#line 502 "compiler/main.gem"
    GemVal _t1136 = gem_table_new();
    gem_table_set(_t1136, gem_string("expr"), gem_string("gem_make_fn(gem_print, NULL)"));
    gem_table_set(_t1136, gem_string("setup"), gem_string(""));
                                    return _t1136;
                                } else {
#line 503 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 504 "compiler/main.gem"
    GemVal _t1137 = gem_table_new();
    gem_table_set(_t1137, gem_string("expr"), gem_string("gem_make_fn(gem_error_fn, NULL)"));
    gem_table_set(_t1137, gem_string("setup"), gem_string(""));
                                        return _t1137;
                                    } else {
#line 505 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("len")))) {
#line 506 "compiler/main.gem"
    GemVal _t1138 = gem_table_new();
    gem_table_set(_t1138, gem_string("expr"), gem_string("gem_make_fn(gem_len_fn, NULL)"));
    gem_table_set(_t1138, gem_string("setup"), gem_string(""));
                                            return _t1138;
                                        } else {
#line 507 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("type")))) {
#line 508 "compiler/main.gem"
    GemVal _t1139 = gem_table_new();
    gem_table_set(_t1139, gem_string("expr"), gem_string("gem_make_fn(gem_type_fn, NULL)"));
    gem_table_set(_t1139, gem_string("setup"), gem_string(""));
                                                return _t1139;
                                            } else {
#line 509 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("to_string")))) {
#line 510 "compiler/main.gem"
    GemVal _t1140 = gem_table_new();
    gem_table_set(_t1140, gem_string("expr"), gem_string("gem_make_fn(gem_to_string_fn, NULL)"));
    gem_table_set(_t1140, gem_string("setup"), gem_string(""));
                                                    return _t1140;
                                                } else {
#line 511 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("push")))) {
#line 512 "compiler/main.gem"
    GemVal _t1141 = gem_table_new();
    gem_table_set(_t1141, gem_string("expr"), gem_string("gem_make_fn(gem_push_fn, NULL)"));
    gem_table_set(_t1141, gem_string("setup"), gem_string(""));
                                                        return _t1141;
                                                    } else {
#line 513 "compiler/main.gem"
    GemVal _t1142[] = {(*gem_v_defined_fns), gem_v_name};
                                                        if (gem_truthy(gem_fn_set_contains(NULL, _t1142, 2))) {
#line 514 "compiler/main.gem"
    GemVal _t1143 = gem_table_new();
    GemVal _t1144[] = {gem_v_name};
    gem_table_set(_t1143, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t1144, 1)), gem_string(", NULL)")));
    gem_table_set(_t1143, gem_string("setup"), gem_string(""));
                                                            return _t1143;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
#line 516 "compiler/main.gem"
    GemVal _t1145 = (*gem_v_mangle);
    GemVal _t1146[] = {gem_v_name};
                                GemVal gem_v_mname = _t1145.fn(_t1145.env, _t1146, 1);
#line 517 "compiler/main.gem"
    GemVal _t1147[] = {(*gem_v_boxed_vars), gem_v_name};
                                if (gem_truthy(gem_fn_set_contains(NULL, _t1147, 2))) {
#line 518 "compiler/main.gem"
    GemVal _t1148 = gem_table_new();
    GemVal _t1149[] = {gem_v_mname};
    gem_table_set(_t1148, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t1149, 1)), gem_string(")")));
    gem_table_set(_t1148, gem_string("setup"), gem_string(""));
                                    return _t1148;
                                }
#line 520 "compiler/main.gem"
    GemVal _t1150 = gem_table_new();
    gem_table_set(_t1150, gem_string("expr"), gem_v_mname);
    gem_table_set(_t1150, gem_string("setup"), gem_string(""));
                                return _t1150;
                            } else {
#line 521 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 522 "compiler/main.gem"
    GemVal _t1151 = (*gem_v_compile_call);
    GemVal _t1152[] = {gem_v_node};
                                    return _t1151.fn(_t1151.env, _t1152, 1);
                                } else {
#line 523 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("binop")))) {
#line 524 "compiler/main.gem"
    GemVal _t1153 = (*gem_v_compile_binop);
    GemVal _t1154[] = {gem_v_node};
                                        return _t1153.fn(_t1153.env, _t1154, 1);
                                    } else {
#line 525 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("unop")))) {
#line 526 "compiler/main.gem"
    GemVal _t1155 = gem_v_node;
    GemVal _t1156 = (*gem_v_compile_expr);
    GemVal _t1157[] = {gem_table_get(_t1155, gem_string("expr"))};
                                            GemVal gem_v_r = _t1156.fn(_t1156.env, _t1157, 1);
#line 527 "compiler/main.gem"
    GemVal _t1158 = gem_v_node;
                                            if (gem_truthy(gem_eq(gem_table_get(_t1158, gem_string("op")), gem_string("-")))) {
#line 528 "compiler/main.gem"
    GemVal _t1159 = gem_table_new();
    GemVal _t1160 = gem_v_r;
    GemVal _t1161[] = {gem_table_get(_t1160, gem_string("expr"))};
    gem_table_set(_t1159, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t1161, 1)), gem_string(")")));
    GemVal _t1162 = gem_v_r;
    gem_table_set(_t1159, gem_string("setup"), gem_table_get(_t1162, gem_string("setup")));
                                                return _t1159;
                                            } else {
#line 529 "compiler/main.gem"
    GemVal _t1163 = gem_v_node;
                                                if (gem_truthy(gem_eq(gem_table_get(_t1163, gem_string("op")), gem_string("not")))) {
#line 530 "compiler/main.gem"
    GemVal _t1164 = gem_table_new();
    GemVal _t1165 = gem_v_r;
    GemVal _t1166[] = {gem_table_get(_t1165, gem_string("expr"))};
    gem_table_set(_t1164, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t1166, 1)), gem_string(")")));
    GemVal _t1167 = gem_v_r;
    gem_table_set(_t1164, gem_string("setup"), gem_table_get(_t1167, gem_string("setup")));
                                                    return _t1164;
                                                }
                                            }
                                        } else {
#line 532 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 533 "compiler/main.gem"
    GemVal _t1168 = (*gem_v_compile_anon_fn);
    GemVal _t1169[] = {gem_v_node};
                                                return _t1168.fn(_t1168.env, _t1169, 1);
                                            } else {
#line 534 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("table")))) {
#line 535 "compiler/main.gem"
    GemVal _t1170 = (*gem_v_compile_table);
    GemVal _t1171[] = {gem_v_node};
                                                    return _t1170.fn(_t1170.env, _t1171, 1);
                                                } else {
#line 536 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("array")))) {
#line 537 "compiler/main.gem"
    GemVal _t1172 = (*gem_v_compile_array);
    GemVal _t1173[] = {gem_v_node};
                                                        return _t1172.fn(_t1172.env, _t1173, 1);
                                                    } else {
#line 538 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot")))) {
#line 539 "compiler/main.gem"
    GemVal _t1174 = gem_v_node;
    GemVal _t1175 = (*gem_v_compile_expr);
    GemVal _t1176[] = {gem_table_get(_t1174, gem_string("object"))};
                                                            GemVal gem_v_r = _t1175.fn(_t1175.env, _t1176, 1);
#line 540 "compiler/main.gem"
    GemVal _t1177 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1177.fn(_t1177.env, NULL, 0);
#line 541 "compiler/main.gem"
    GemVal _t1178 = gem_v_r;
    GemVal _t1179[] = {gem_v_t};
    GemVal _t1180 = gem_v_r;
    GemVal _t1181[] = {gem_table_get(_t1180, gem_string("expr"))};
                                                            GemVal gem_v_setup = gem_add(gem_table_get(_t1178, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1179, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1181, 1)), gem_string(";\n")));
#line 542 "compiler/main.gem"
    GemVal _t1182 = gem_v_node;
    GemVal _t1183[] = {gem_table_get(_t1182, gem_string("field"))};
                                                            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1183, 1);
#line 543 "compiler/main.gem"
    GemVal _t1184 = gem_table_new();
    GemVal _t1185[] = {gem_v_t};
    GemVal _t1186[] = {gem_v_escaped};
    gem_table_set(_t1184, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1185, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1186, 1)), gem_string("\"))")));
    gem_table_set(_t1184, gem_string("setup"), gem_v_setup);
                                                            return _t1184;
                                                        } else {
#line 544 "compiler/main.gem"
                                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("index")))) {
#line 545 "compiler/main.gem"
    GemVal _t1187 = gem_v_node;
    GemVal _t1188 = (*gem_v_compile_expr);
    GemVal _t1189[] = {gem_table_get(_t1187, gem_string("object"))};
                                                                GemVal gem_v_obj_r = _t1188.fn(_t1188.env, _t1189, 1);
#line 546 "compiler/main.gem"
    GemVal _t1190 = gem_v_node;
    GemVal _t1191 = (*gem_v_compile_expr);
    GemVal _t1192[] = {gem_table_get(_t1190, gem_string("key"))};
                                                                GemVal gem_v_key_r = _t1191.fn(_t1191.env, _t1192, 1);
#line 547 "compiler/main.gem"
    GemVal _t1193 = gem_table_new();
    GemVal _t1194 = gem_v_obj_r;
    GemVal _t1195[] = {gem_table_get(_t1194, gem_string("expr"))};
    GemVal _t1196 = gem_v_key_r;
    GemVal _t1197[] = {gem_table_get(_t1196, gem_string("expr"))};
    gem_table_set(_t1193, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1195, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1197, 1)), gem_string(")")));
    GemVal _t1198 = gem_v_obj_r;
    GemVal _t1199 = gem_v_key_r;
    gem_table_set(_t1193, gem_string("setup"), gem_add(gem_table_get(_t1198, gem_string("setup")), gem_table_get(_t1199, gem_string("setup"))));
                                                                return _t1193;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t1200 = gem_table_new();
    gem_table_set(_t1200, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1200, gem_string("setup"), gem_add(gem_add(gem_string("/* unknown node: "), gem_v_tag), gem_string(" */\n")));
    return _t1200;
}

struct _closure__anon_31 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_31(void *_env, GemVal *args, int argc) {
    struct _closure__anon_31 *_cls = (struct _closure__anon_31 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
#line 556 "compiler/main.gem"
    GemVal _t1202 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t1202, gem_string("entries"));
#line 557 "compiler/main.gem"
    GemVal _t1203 = (*gem_v_tmp);
    GemVal gem_v_t = _t1203.fn(_t1203.env, NULL, 0);
#line 558 "compiler/main.gem"
    GemVal _t1204[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1204, 1)), gem_string(" = gem_table_new();\n"));
    {
#line 559 "compiler/main.gem"
        GemVal gem_v__for_items_21 = gem_v_entries;
#line 559 "compiler/main.gem"
        GemVal gem_v__for_i_21 = gem_int(0);
#line 559 "compiler/main.gem"
        while (1) {
            GemVal _t1205[] = {gem_v__for_items_21};
            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t1205, 1)))) break;
#line 559 "compiler/main.gem"
            GemVal gem_v_entry = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
#line 559 "compiler/main.gem"
            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 560 "compiler/main.gem"
    GemVal _t1206 = gem_v_entry;
    GemVal _t1207 = (*gem_v_compile_expr);
    GemVal _t1208[] = {gem_table_get(_t1206, gem_string("value"))};
            GemVal gem_v_val_r = _t1207.fn(_t1207.env, _t1208, 1);
#line 561 "compiler/main.gem"
    GemVal _t1209 = gem_v_entry;
    GemVal _t1210[] = {gem_table_get(_t1209, gem_string("key"))};
            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1210, 1);
#line 562 "compiler/main.gem"
    GemVal _t1211 = gem_v_val_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1211, gem_string("setup")));
#line 563 "compiler/main.gem"
    GemVal _t1212[] = {gem_v_t};
    GemVal _t1213[] = {gem_v_escaped};
    GemVal _t1214 = gem_v_val_r;
    GemVal _t1215[] = {gem_table_get(_t1214, gem_string("expr"))};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1212, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1213, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1215, 1)), gem_string(");\n")));
        }
    }
    GemVal _t1216 = gem_table_new();
    gem_table_set(_t1216, gem_string("expr"), gem_v_t);
    gem_table_set(_t1216, gem_string("setup"), gem_v_setup);
    return _t1216;
}

struct _closure__anon_32 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_32(void *_env, GemVal *args, int argc) {
    struct _closure__anon_32 *_cls = (struct _closure__anon_32 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
#line 571 "compiler/main.gem"
    GemVal _t1218 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t1218, gem_string("elements"));
#line 572 "compiler/main.gem"
    GemVal _t1219 = (*gem_v_tmp);
    GemVal gem_v_t = _t1219.fn(_t1219.env, NULL, 0);
#line 573 "compiler/main.gem"
    GemVal _t1220[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1220, 1)), gem_string(" = gem_table_new();\n"));
    {
#line 574 "compiler/main.gem"
        GemVal gem_v__for_i_22 = gem_int(0);
#line 574 "compiler/main.gem"
    GemVal _t1221[] = {gem_v_elements};
        GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1221, 1);
#line 574 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 574 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_22;
#line 574 "compiler/main.gem"
            gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 575 "compiler/main.gem"
    GemVal _t1222 = (*gem_v_compile_expr);
    GemVal _t1223[] = {gem_table_get(gem_v_elements, gem_v_i)};
            GemVal gem_v_elem_r = _t1222.fn(_t1222.env, _t1223, 1);
#line 576 "compiler/main.gem"
    GemVal _t1224 = gem_v_elem_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1224, gem_string("setup")));
#line 577 "compiler/main.gem"
    GemVal _t1225[] = {gem_v_t};
    GemVal _t1226[] = {gem_v_i};
    GemVal _t1227 = gem_v_elem_r;
    GemVal _t1228[] = {gem_table_get(_t1227, gem_string("expr"))};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1225, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t1226, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t1228, 1)), gem_string(");\n")));
        }
    }
    GemVal _t1229 = gem_table_new();
    gem_table_set(_t1229, gem_string("expr"), gem_v_t);
    gem_table_set(_t1229, gem_string("setup"), gem_v_setup);
    return _t1229;
}

struct _closure__anon_33 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_compile_closure_fn;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_33(void *_env, GemVal *args, int argc) {
    struct _closure__anon_33 *_cls = (struct _closure__anon_33 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_compile_closure_fn = _cls->gem_v_compile_closure_fn;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
#line 585 "compiler/main.gem"
    GemVal _t1231 = gem_v_node;
    GemVal _t1232[] = {gem_table_get(_t1231, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1232, 1);
#line 586 "compiler/main.gem"
    GemVal _t1233 = gem_v_node;
    GemVal _t1234 = (*gem_v_collect_free_vars);
    GemVal _t1235[] = {gem_table_get(_t1233, gem_string("body")), gem_v_inner_defined};
    GemVal gem_v_free = _t1234.fn(_t1234.env, _t1235, 2);
#line 587 "compiler/main.gem"
    GemVal _t1236[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t1236, 1);
#line 589 "compiler/main.gem"
    GemVal _t1237[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1237, 1), gem_int(0)))) {
#line 590 "compiler/main.gem"
    GemVal _t1238 = gem_table_new();
    GemVal _t1239 = (*gem_v_compile_closure_fn);
    GemVal _t1240[] = {gem_v_node, _t1238};
        GemVal gem_v_result = _t1239.fn(_t1239.env, _t1240, 2);
#line 591 "compiler/main.gem"
    GemVal _t1241 = gem_table_new();
    GemVal _t1242 = gem_v_result;
    GemVal _t1243[] = {gem_table_get(_t1242, gem_string("fn_name"))};
    gem_table_set(_t1241, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1243, 1)), gem_string(", NULL)")));
    gem_table_set(_t1241, gem_string("setup"), gem_string(""));
        return _t1241;
    }
#line 594 "compiler/main.gem"
    GemVal _t1244 = (*gem_v_compile_closure_fn);
    GemVal _t1245[] = {gem_v_node, gem_v_captures};
    GemVal gem_v_result = _t1244.fn(_t1244.env, _t1245, 2);
#line 596 "compiler/main.gem"
    GemVal gem_v_setup = gem_string("");
#line 597 "compiler/main.gem"
    GemVal _t1246 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t1246.fn(_t1246.env, NULL, 0);
#line 598 "compiler/main.gem"
    GemVal _t1247 = gem_v_result;
    GemVal _t1248[] = {gem_table_get(_t1247, gem_string("struct_name"))};
    GemVal _t1249[] = {gem_v_env_tmp};
    GemVal _t1250 = gem_v_result;
    GemVal _t1251[] = {gem_table_get(_t1250, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t1248, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t1249, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t1251, 1)), gem_string("));\n")));
    {
#line 599 "compiler/main.gem"
        GemVal gem_v__for_items_23 = gem_v_captures;
#line 599 "compiler/main.gem"
        GemVal gem_v__for_i_23 = gem_int(0);
#line 599 "compiler/main.gem"
        while (1) {
            GemVal _t1252[] = {gem_v__for_items_23};
            if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_len_fn(NULL, _t1252, 1)))) break;
#line 599 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_23, gem_v__for_i_23);
#line 599 "compiler/main.gem"
            gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 600 "compiler/main.gem"
    GemVal _t1253 = (*gem_v_mangle);
    GemVal _t1254[] = {gem_v_cap};
            GemVal gem_v_mc = _t1253.fn(_t1253.env, _t1254, 1);
#line 601 "compiler/main.gem"
    GemVal _t1255[] = {(*gem_v_boxed_vars), gem_v_cap};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1255, 2))) {
#line 602 "compiler/main.gem"
    GemVal _t1256[] = {gem_v_env_tmp};
    GemVal _t1257[] = {gem_v_mc};
    GemVal _t1258[] = {gem_v_mc};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1256, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1257, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1258, 1)), gem_string(";\n")));
            } else {
#line 604 "compiler/main.gem"
    GemVal _t1259[] = {gem_v_env_tmp};
    GemVal _t1260[] = {gem_v_mc};
    GemVal _t1261[] = {gem_v_mc};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1259, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1260, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t1261, 1)), gem_string(";\n")));
            }
        }
    }
    GemVal _t1262 = gem_table_new();
    GemVal _t1263 = gem_v_result;
    GemVal _t1264[] = {gem_table_get(_t1263, gem_string("fn_name"))};
    GemVal _t1265[] = {gem_v_env_tmp};
    gem_table_set(_t1262, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1264, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1265, 1)), gem_string(")")));
    gem_table_set(_t1262, gem_string("setup"), gem_v_setup);
    return _t1262;
}

struct _closure__anon_34 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tmp;
};
static GemVal _anon_34(void *_env, GemVal *args, int argc) {
    struct _closure__anon_34 *_cls = (struct _closure__anon_34 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
#line 613 "compiler/main.gem"
    GemVal _t1267 = gem_v_node;
    GemVal gem_v_func = gem_table_get(_t1267, gem_string("func"));
#line 614 "compiler/main.gem"
    GemVal _t1268 = gem_v_node;
    GemVal gem_v_args = gem_table_get(_t1268, gem_string("args"));
#line 617 "compiler/main.gem"
    GemVal _t1269 = gem_v_func;
    GemVal _t1271;
    if (!gem_truthy(gem_eq(gem_table_get(_t1269, gem_string("tag")), gem_string("dot")))) {
        _t1271 = gem_eq(gem_table_get(_t1269, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t1270 = gem_v_func;
        _t1271 = gem_eq(gem_table_get(_t1270, gem_string("field")), gem_string("len"));
    }
    GemVal _t1273;
    if (!gem_truthy(_t1271)) {
        _t1273 = _t1271;
    } else {
        GemVal _t1272[] = {gem_v_args};
        _t1273 = gem_eq(gem_len_fn(NULL, _t1272, 1), gem_int(0));
    }
    if (gem_truthy(_t1273)) {
#line 618 "compiler/main.gem"
    GemVal _t1274 = gem_v_func;
    GemVal _t1275 = (*gem_v_compile_expr);
    GemVal _t1276[] = {gem_table_get(_t1274, gem_string("object"))};
        GemVal gem_v_obj_r = _t1275.fn(_t1275.env, _t1276, 1);
#line 619 "compiler/main.gem"
    GemVal _t1277 = gem_table_new();
    GemVal _t1278 = gem_v_obj_r;
    GemVal _t1279[] = {gem_table_get(_t1278, gem_string("expr"))};
    gem_table_set(_t1277, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t1279, 1)), gem_string(")")));
    GemVal _t1280 = gem_v_obj_r;
    gem_table_set(_t1277, gem_string("setup"), gem_table_get(_t1280, gem_string("setup")));
        return _t1277;
    }
#line 623 "compiler/main.gem"
    GemVal _t1281 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t1281, gem_string("tag")), gem_string("var")))) {
#line 624 "compiler/main.gem"
    GemVal _t1282 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t1282, gem_string("name"));
#line 626 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 627 "compiler/main.gem"
    GemVal _t1283 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t1283, gem_string("line"));
#line 628 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 629 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 631 "compiler/main.gem"
    GemVal _t1284[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t1284, 1);
#line 632 "compiler/main.gem"
            GemVal gem_v_arg_setups = gem_string("");
#line 633 "compiler/main.gem"
    GemVal _t1285 = gem_table_new();
            GemVal gem_v_arg_exprs = _t1285;
            {
#line 634 "compiler/main.gem"
                GemVal gem_v__for_i_24 = gem_int(0);
#line 634 "compiler/main.gem"
    GemVal _t1286[] = {gem_v_args};
                GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1286, 1);
#line 634 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 634 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_24;
#line 634 "compiler/main.gem"
                    gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 635 "compiler/main.gem"
    GemVal _t1287 = (*gem_v_compile_expr);
    GemVal _t1288[] = {gem_table_get(gem_v_args, gem_v_i)};
                    GemVal gem_v_r = _t1287.fn(_t1287.env, _t1288, 1);
#line 636 "compiler/main.gem"
    GemVal _t1289 = gem_v_r;
                    gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1289, gem_string("setup")));
#line 637 "compiler/main.gem"
    GemVal _t1290 = gem_v_r;
    GemVal _t1291[] = {gem_v_arg_exprs, gem_table_get(_t1290, gem_string("expr"))};
                    (void)(gem_push_fn(NULL, _t1291, 2));
                }
            }
#line 639 "compiler/main.gem"
    GemVal _t1292[] = {gem_v_args};
            GemVal gem_v_argc = gem_len_fn(NULL, _t1292, 1);
#line 640 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 641 "compiler/main.gem"
    GemVal _t1293 = gem_table_new();
    GemVal _t1294[] = {gem_v_escaped_file};
    GemVal _t1295[] = {gem_v_line};
    gem_table_set(_t1293, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1294, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1295, 1)), gem_string(", NULL, 0)")));
    gem_table_set(_t1293, gem_string("setup"), gem_v_arg_setups);
                return _t1293;
            }
#line 643 "compiler/main.gem"
    GemVal _t1296 = (*gem_v_tmp);
            GemVal gem_v_t = _t1296.fn(_t1296.env, NULL, 0);
#line 644 "compiler/main.gem"
            GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
            {
#line 645 "compiler/main.gem"
                GemVal gem_v__for_i_25 = gem_int(1);
#line 645 "compiler/main.gem"
                GemVal gem_v__for_limit_25 = gem_v_argc;
#line 645 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 645 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_25;
#line 645 "compiler/main.gem"
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 646 "compiler/main.gem"
                    gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                }
            }
#line 648 "compiler/main.gem"
    GemVal _t1297[] = {gem_v_t};
    GemVal _t1298[] = {gem_v_arr};
            GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1297, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1298, 1)), gem_string("};\n")));
#line 649 "compiler/main.gem"
    GemVal _t1299[] = {gem_v_escaped_file};
    GemVal _t1300[] = {gem_v_line};
    GemVal _t1301[] = {gem_v_t};
    GemVal _t1302[] = {gem_v_argc};
            GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1299, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1300, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1301, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1302, 1)), gem_string(")"));
#line 650 "compiler/main.gem"
    GemVal _t1303 = gem_table_new();
    gem_table_set(_t1303, gem_string("expr"), gem_v_call);
    gem_table_set(_t1303, gem_string("setup"), gem_v_setup);
            return _t1303;
        } else {
#line 651 "compiler/main.gem"
    GemVal _t1304;
    if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
        _t1304 = gem_eq(gem_v_name, gem_string("print"));
    } else {
        _t1304 = gem_eq(gem_v_name, gem_string("len"));
    }
    GemVal _t1305;
    if (gem_truthy(_t1304)) {
        _t1305 = _t1304;
    } else {
        _t1305 = gem_eq(gem_v_name, gem_string("type"));
    }
    GemVal _t1306;
    if (gem_truthy(_t1305)) {
        _t1306 = _t1305;
    } else {
        _t1306 = gem_eq(gem_v_name, gem_string("to_string"));
    }
    GemVal _t1307;
    if (gem_truthy(_t1306)) {
        _t1307 = _t1306;
    } else {
        _t1307 = gem_eq(gem_v_name, gem_string("push"));
    }
            if (gem_truthy(_t1307)) {
#line 652 "compiler/main.gem"
                GemVal gem_v_fn_name = gem_string("");
#line 653 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
#line 654 "compiler/main.gem"
                    gem_v_fn_name = gem_string("gem_print");
                } else {
#line 655 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_name, gem_string("len")))) {
#line 656 "compiler/main.gem"
                        gem_v_fn_name = gem_string("gem_len_fn");
                    } else {
#line 657 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_name, gem_string("type")))) {
#line 658 "compiler/main.gem"
                            gem_v_fn_name = gem_string("gem_type_fn");
                        } else {
#line 659 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_name, gem_string("to_string")))) {
#line 660 "compiler/main.gem"
                                gem_v_fn_name = gem_string("gem_to_string_fn");
                            } else {
#line 661 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_name, gem_string("push")))) {
#line 662 "compiler/main.gem"
                                    gem_v_fn_name = gem_string("gem_push_fn");
                                }
                            }
                        }
                    }
                }
#line 664 "compiler/main.gem"
                GemVal gem_v_arg_setups = gem_string("");
#line 665 "compiler/main.gem"
    GemVal _t1308 = gem_table_new();
                GemVal gem_v_arg_exprs = _t1308;
                {
#line 666 "compiler/main.gem"
                    GemVal gem_v__for_i_26 = gem_int(0);
#line 666 "compiler/main.gem"
    GemVal _t1309[] = {gem_v_args};
                    GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1309, 1);
#line 666 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 666 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_26;
#line 666 "compiler/main.gem"
                        gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 667 "compiler/main.gem"
    GemVal _t1310 = (*gem_v_compile_expr);
    GemVal _t1311[] = {gem_table_get(gem_v_args, gem_v_i)};
                        GemVal gem_v_r = _t1310.fn(_t1310.env, _t1311, 1);
#line 668 "compiler/main.gem"
    GemVal _t1312 = gem_v_r;
                        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1312, gem_string("setup")));
#line 669 "compiler/main.gem"
    GemVal _t1313 = gem_v_r;
    GemVal _t1314[] = {gem_v_arg_exprs, gem_table_get(_t1313, gem_string("expr"))};
                        (void)(gem_push_fn(NULL, _t1314, 2));
                    }
                }
#line 671 "compiler/main.gem"
    GemVal _t1315[] = {gem_v_args};
                GemVal gem_v_argc = gem_len_fn(NULL, _t1315, 1);
#line 672 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 673 "compiler/main.gem"
    GemVal _t1316 = gem_table_new();
    GemVal _t1317[] = {gem_v_fn_name};
    gem_table_set(_t1316, gem_string("expr"), gem_add(gem_to_string_fn(NULL, _t1317, 1), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1316, gem_string("setup"), gem_v_arg_setups);
                    return _t1316;
                }
#line 675 "compiler/main.gem"
    GemVal _t1318 = (*gem_v_tmp);
                GemVal gem_v_t = _t1318.fn(_t1318.env, NULL, 0);
#line 676 "compiler/main.gem"
                GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
                {
#line 677 "compiler/main.gem"
                    GemVal gem_v__for_i_27 = gem_int(1);
#line 677 "compiler/main.gem"
                    GemVal gem_v__for_limit_27 = gem_v_argc;
#line 677 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 677 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_27;
#line 677 "compiler/main.gem"
                        gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 678 "compiler/main.gem"
                        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                    }
                }
#line 680 "compiler/main.gem"
    GemVal _t1319[] = {gem_v_t};
    GemVal _t1320[] = {gem_v_arr};
                GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1319, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1320, 1)), gem_string("};\n")));
#line 681 "compiler/main.gem"
    GemVal _t1321 = gem_table_new();
    GemVal _t1322[] = {gem_v_fn_name};
    GemVal _t1323[] = {gem_v_t};
    GemVal _t1324[] = {gem_v_argc};
    gem_table_set(_t1321, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1322, 1), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1323, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1324, 1)), gem_string(")")));
    gem_table_set(_t1321, gem_string("setup"), gem_v_setup);
                return _t1321;
            } else {
#line 682 "compiler/main.gem"
    GemVal _t1325[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t1325, 2))) {
#line 683 "compiler/main.gem"
                    GemVal gem_v_arg_setups = gem_string("");
#line 684 "compiler/main.gem"
    GemVal _t1326 = gem_table_new();
                    GemVal gem_v_arg_exprs = _t1326;
                    {
#line 685 "compiler/main.gem"
                        GemVal gem_v__for_i_28 = gem_int(0);
#line 685 "compiler/main.gem"
    GemVal _t1327[] = {gem_v_args};
                        GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1327, 1);
#line 685 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 685 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_28;
#line 685 "compiler/main.gem"
                            gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 686 "compiler/main.gem"
    GemVal _t1328 = (*gem_v_compile_expr);
    GemVal _t1329[] = {gem_table_get(gem_v_args, gem_v_i)};
                            GemVal gem_v_r = _t1328.fn(_t1328.env, _t1329, 1);
#line 687 "compiler/main.gem"
    GemVal _t1330 = gem_v_r;
                            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1330, gem_string("setup")));
#line 688 "compiler/main.gem"
    GemVal _t1331 = gem_v_r;
    GemVal _t1332[] = {gem_v_arg_exprs, gem_table_get(_t1331, gem_string("expr"))};
                            (void)(gem_push_fn(NULL, _t1332, 2));
                        }
                    }
#line 690 "compiler/main.gem"
    GemVal _t1333[] = {gem_v_args};
                    GemVal gem_v_argc = gem_len_fn(NULL, _t1333, 1);
#line 691 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 692 "compiler/main.gem"
    GemVal _t1334 = gem_table_new();
    GemVal _t1335[] = {gem_v_name};
    gem_table_set(_t1334, gem_string("expr"), gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1335, 1)), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1334, gem_string("setup"), gem_v_arg_setups);
                        return _t1334;
                    }
#line 694 "compiler/main.gem"
    GemVal _t1336 = (*gem_v_tmp);
                    GemVal gem_v_t = _t1336.fn(_t1336.env, NULL, 0);
#line 695 "compiler/main.gem"
                    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
                    {
#line 696 "compiler/main.gem"
                        GemVal gem_v__for_i_29 = gem_int(1);
#line 696 "compiler/main.gem"
                        GemVal gem_v__for_limit_29 = gem_v_argc;
#line 696 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_v__for_limit_29))) break;
#line 696 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_29;
#line 696 "compiler/main.gem"
                            gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 697 "compiler/main.gem"
                            gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                        }
                    }
#line 699 "compiler/main.gem"
    GemVal _t1337[] = {gem_v_t};
    GemVal _t1338[] = {gem_v_arr};
                    GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1337, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1338, 1)), gem_string("};\n")));
#line 700 "compiler/main.gem"
    GemVal _t1339 = gem_table_new();
    GemVal _t1340[] = {gem_v_name};
    GemVal _t1341[] = {gem_v_t};
    GemVal _t1342[] = {gem_v_argc};
    gem_table_set(_t1339, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1340, 1)), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1341, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1342, 1)), gem_string(")")));
    gem_table_set(_t1339, gem_string("setup"), gem_v_setup);
                    return _t1339;
                }
            }
        }
    }
#line 705 "compiler/main.gem"
    GemVal _t1343 = (*gem_v_compile_expr);
    GemVal _t1344[] = {gem_v_func};
    GemVal gem_v_func_r = _t1343.fn(_t1343.env, _t1344, 1);
#line 706 "compiler/main.gem"
    GemVal gem_v_arg_setups = gem_string("");
#line 707 "compiler/main.gem"
    GemVal _t1345 = gem_table_new();
    GemVal gem_v_arg_exprs = _t1345;
    {
#line 708 "compiler/main.gem"
        GemVal gem_v__for_i_30 = gem_int(0);
#line 708 "compiler/main.gem"
    GemVal _t1346[] = {gem_v_args};
        GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t1346, 1);
#line 708 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 708 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_30;
#line 708 "compiler/main.gem"
            gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 709 "compiler/main.gem"
    GemVal _t1347 = (*gem_v_compile_expr);
    GemVal _t1348[] = {gem_table_get(gem_v_args, gem_v_i)};
            GemVal gem_v_r = _t1347.fn(_t1347.env, _t1348, 1);
#line 710 "compiler/main.gem"
    GemVal _t1349 = gem_v_r;
            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1349, gem_string("setup")));
#line 711 "compiler/main.gem"
    GemVal _t1350 = gem_v_r;
    GemVal _t1351[] = {gem_v_arg_exprs, gem_table_get(_t1350, gem_string("expr"))};
            (void)(gem_push_fn(NULL, _t1351, 2));
        }
    }
#line 713 "compiler/main.gem"
    GemVal _t1352 = gem_v_func_r;
    GemVal gem_v_setup = gem_add(gem_table_get(_t1352, gem_string("setup")), gem_v_arg_setups);
#line 714 "compiler/main.gem"
    GemVal _t1353[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t1353, 1);
#line 715 "compiler/main.gem"
    GemVal _t1354 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t1354.fn(_t1354.env, NULL, 0);
#line 716 "compiler/main.gem"
    GemVal _t1355[] = {gem_v_tmp_fn};
    GemVal _t1356 = gem_v_func_r;
    GemVal _t1357[] = {gem_table_get(_t1356, gem_string("expr"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1355, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1357, 1)), gem_string(";\n")));
#line 717 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 718 "compiler/main.gem"
    GemVal _t1358 = gem_table_new();
    GemVal _t1359[] = {gem_v_tmp_fn};
    GemVal _t1360[] = {gem_v_tmp_fn};
    gem_table_set(_t1358, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1359, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1360, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t1358, gem_string("setup"), gem_v_setup);
        return _t1358;
    }
#line 720 "compiler/main.gem"
    GemVal _t1361 = (*gem_v_tmp);
    GemVal gem_v_tmp_args = _t1361.fn(_t1361.env, NULL, 0);
#line 721 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
    {
#line 722 "compiler/main.gem"
        GemVal gem_v__for_i_31 = gem_int(1);
#line 722 "compiler/main.gem"
        GemVal gem_v__for_limit_31 = gem_v_argc;
#line 722 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_v__for_limit_31))) break;
#line 722 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_31;
#line 722 "compiler/main.gem"
            gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
#line 723 "compiler/main.gem"
            gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
        }
    }
#line 725 "compiler/main.gem"
    GemVal _t1362[] = {gem_v_tmp_args};
    GemVal _t1363[] = {gem_v_arr};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1362, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1363, 1)), gem_string("};\n")));
#line 726 "compiler/main.gem"
    GemVal _t1364[] = {gem_v_tmp_fn};
    GemVal _t1365[] = {gem_v_tmp_fn};
    GemVal _t1366[] = {gem_v_tmp_args};
    GemVal _t1367[] = {gem_v_argc};
    GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1364, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1365, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t1366, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1367, 1)), gem_string(")"));
    GemVal _t1368 = gem_table_new();
    gem_table_set(_t1368, gem_string("expr"), gem_v_call);
    gem_table_set(_t1368, gem_string("setup"), gem_v_setup);
    return _t1368;
}

struct _closure__anon_35 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_35(void *_env, GemVal *args, int argc) {
    struct _closure__anon_35 *_cls = (struct _closure__anon_35 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
#line 733 "compiler/main.gem"
    GemVal _t1370 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t1370, gem_string("op"));
#line 734 "compiler/main.gem"
    GemVal _t1371 = gem_v_node;
    GemVal _t1372 = (*gem_v_compile_expr);
    GemVal _t1373[] = {gem_table_get(_t1371, gem_string("left"))};
    GemVal gem_v_lc_r = _t1372.fn(_t1372.env, _t1373, 1);
#line 735 "compiler/main.gem"
    GemVal _t1374 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t1374, gem_string("expr"));
#line 736 "compiler/main.gem"
    GemVal _t1375 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t1375, gem_string("setup"));
#line 738 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 739 "compiler/main.gem"
    GemVal _t1376 = gem_v_node;
    GemVal _t1377 = (*gem_v_compile_expr);
    GemVal _t1378[] = {gem_table_get(_t1376, gem_string("right"))};
        GemVal gem_v_rc_r = _t1377.fn(_t1377.env, _t1378, 1);
#line 740 "compiler/main.gem"
    GemVal _t1379 = gem_table_new();
    GemVal _t1380[] = {gem_v_lc};
    GemVal _t1381 = gem_v_rc_r;
    GemVal _t1382[] = {gem_table_get(_t1381, gem_string("expr"))};
    gem_table_set(_t1379, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1380, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1382, 1)), gem_string(")")));
    GemVal _t1383 = gem_v_rc_r;
    gem_table_set(_t1379, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1383, gem_string("setup"))));
        return _t1379;
    } else {
#line 741 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 742 "compiler/main.gem"
    GemVal _t1384 = gem_v_node;
    GemVal _t1385 = (*gem_v_compile_expr);
    GemVal _t1386[] = {gem_table_get(_t1384, gem_string("right"))};
            GemVal gem_v_rc_r = _t1385.fn(_t1385.env, _t1386, 1);
#line 743 "compiler/main.gem"
    GemVal _t1387 = gem_table_new();
    GemVal _t1388[] = {gem_v_lc};
    GemVal _t1389 = gem_v_rc_r;
    GemVal _t1390[] = {gem_table_get(_t1389, gem_string("expr"))};
    gem_table_set(_t1387, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t1388, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1390, 1)), gem_string(")")));
    GemVal _t1391 = gem_v_rc_r;
    gem_table_set(_t1387, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1391, gem_string("setup"))));
            return _t1387;
        } else {
#line 744 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 745 "compiler/main.gem"
    GemVal _t1392 = gem_v_node;
    GemVal _t1393 = (*gem_v_compile_expr);
    GemVal _t1394[] = {gem_table_get(_t1392, gem_string("right"))};
                GemVal gem_v_rc_r = _t1393.fn(_t1393.env, _t1394, 1);
#line 746 "compiler/main.gem"
    GemVal _t1395 = gem_table_new();
    GemVal _t1396[] = {gem_v_lc};
    GemVal _t1397 = gem_v_rc_r;
    GemVal _t1398[] = {gem_table_get(_t1397, gem_string("expr"))};
    gem_table_set(_t1395, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t1396, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1398, 1)), gem_string(")")));
    GemVal _t1399 = gem_v_rc_r;
    gem_table_set(_t1395, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1399, gem_string("setup"))));
                return _t1395;
            } else {
#line 747 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 748 "compiler/main.gem"
    GemVal _t1400 = gem_v_node;
    GemVal _t1401 = (*gem_v_compile_expr);
    GemVal _t1402[] = {gem_table_get(_t1400, gem_string("right"))};
                    GemVal gem_v_rc_r = _t1401.fn(_t1401.env, _t1402, 1);
#line 749 "compiler/main.gem"
    GemVal _t1403 = gem_table_new();
    GemVal _t1404[] = {gem_v_lc};
    GemVal _t1405 = gem_v_rc_r;
    GemVal _t1406[] = {gem_table_get(_t1405, gem_string("expr"))};
    gem_table_set(_t1403, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t1404, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1406, 1)), gem_string(")")));
    GemVal _t1407 = gem_v_rc_r;
    gem_table_set(_t1403, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1407, gem_string("setup"))));
                    return _t1403;
                } else {
#line 750 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 751 "compiler/main.gem"
    GemVal _t1408 = gem_v_node;
    GemVal _t1409 = (*gem_v_compile_expr);
    GemVal _t1410[] = {gem_table_get(_t1408, gem_string("right"))};
                        GemVal gem_v_rc_r = _t1409.fn(_t1409.env, _t1410, 1);
#line 752 "compiler/main.gem"
    GemVal _t1411 = gem_table_new();
    GemVal _t1412[] = {gem_v_lc};
    GemVal _t1413 = gem_v_rc_r;
    GemVal _t1414[] = {gem_table_get(_t1413, gem_string("expr"))};
    gem_table_set(_t1411, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t1412, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1414, 1)), gem_string(")")));
    GemVal _t1415 = gem_v_rc_r;
    gem_table_set(_t1411, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1415, gem_string("setup"))));
                        return _t1411;
                    } else {
#line 753 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 754 "compiler/main.gem"
    GemVal _t1416 = gem_v_node;
    GemVal _t1417 = (*gem_v_compile_expr);
    GemVal _t1418[] = {gem_table_get(_t1416, gem_string("right"))};
                            GemVal gem_v_rc_r = _t1417.fn(_t1417.env, _t1418, 1);
#line 755 "compiler/main.gem"
    GemVal _t1419 = gem_table_new();
    GemVal _t1420[] = {gem_v_lc};
    GemVal _t1421 = gem_v_rc_r;
    GemVal _t1422[] = {gem_table_get(_t1421, gem_string("expr"))};
    gem_table_set(_t1419, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t1420, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1422, 1)), gem_string(")")));
    GemVal _t1423 = gem_v_rc_r;
    gem_table_set(_t1419, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1423, gem_string("setup"))));
                            return _t1419;
                        } else {
#line 756 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 757 "compiler/main.gem"
    GemVal _t1424 = gem_v_node;
    GemVal _t1425 = (*gem_v_compile_expr);
    GemVal _t1426[] = {gem_table_get(_t1424, gem_string("right"))};
                                GemVal gem_v_rc_r = _t1425.fn(_t1425.env, _t1426, 1);
#line 758 "compiler/main.gem"
    GemVal _t1427 = gem_table_new();
    GemVal _t1428[] = {gem_v_lc};
    GemVal _t1429 = gem_v_rc_r;
    GemVal _t1430[] = {gem_table_get(_t1429, gem_string("expr"))};
    gem_table_set(_t1427, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t1428, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1430, 1)), gem_string(")")));
    GemVal _t1431 = gem_v_rc_r;
    gem_table_set(_t1427, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1431, gem_string("setup"))));
                                return _t1427;
                            } else {
#line 759 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 760 "compiler/main.gem"
    GemVal _t1432 = gem_v_node;
    GemVal _t1433 = (*gem_v_compile_expr);
    GemVal _t1434[] = {gem_table_get(_t1432, gem_string("right"))};
                                    GemVal gem_v_rc_r = _t1433.fn(_t1433.env, _t1434, 1);
#line 761 "compiler/main.gem"
    GemVal _t1435 = gem_table_new();
    GemVal _t1436[] = {gem_v_lc};
    GemVal _t1437 = gem_v_rc_r;
    GemVal _t1438[] = {gem_table_get(_t1437, gem_string("expr"))};
    gem_table_set(_t1435, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t1436, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1438, 1)), gem_string(")")));
    GemVal _t1439 = gem_v_rc_r;
    gem_table_set(_t1435, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1439, gem_string("setup"))));
                                    return _t1435;
                                } else {
#line 762 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 763 "compiler/main.gem"
    GemVal _t1440 = gem_v_node;
    GemVal _t1441 = (*gem_v_compile_expr);
    GemVal _t1442[] = {gem_table_get(_t1440, gem_string("right"))};
                                        GemVal gem_v_rc_r = _t1441.fn(_t1441.env, _t1442, 1);
#line 764 "compiler/main.gem"
    GemVal _t1443 = gem_table_new();
    GemVal _t1444[] = {gem_v_lc};
    GemVal _t1445 = gem_v_rc_r;
    GemVal _t1446[] = {gem_table_get(_t1445, gem_string("expr"))};
    gem_table_set(_t1443, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t1444, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1446, 1)), gem_string(")")));
    GemVal _t1447 = gem_v_rc_r;
    gem_table_set(_t1443, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1447, gem_string("setup"))));
                                        return _t1443;
                                    } else {
#line 765 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 766 "compiler/main.gem"
    GemVal _t1448 = gem_v_node;
    GemVal _t1449 = (*gem_v_compile_expr);
    GemVal _t1450[] = {gem_table_get(_t1448, gem_string("right"))};
                                            GemVal gem_v_rc_r = _t1449.fn(_t1449.env, _t1450, 1);
#line 767 "compiler/main.gem"
    GemVal _t1451 = gem_table_new();
    GemVal _t1452[] = {gem_v_lc};
    GemVal _t1453 = gem_v_rc_r;
    GemVal _t1454[] = {gem_table_get(_t1453, gem_string("expr"))};
    gem_table_set(_t1451, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t1452, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1454, 1)), gem_string(")")));
    GemVal _t1455 = gem_v_rc_r;
    gem_table_set(_t1451, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1455, gem_string("setup"))));
                                            return _t1451;
                                        } else {
#line 768 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 769 "compiler/main.gem"
    GemVal _t1456 = gem_v_node;
    GemVal _t1457 = (*gem_v_compile_expr);
    GemVal _t1458[] = {gem_table_get(_t1456, gem_string("right"))};
                                                GemVal gem_v_rc_r = _t1457.fn(_t1457.env, _t1458, 1);
#line 770 "compiler/main.gem"
    GemVal _t1459 = gem_table_new();
    GemVal _t1460[] = {gem_v_lc};
    GemVal _t1461 = gem_v_rc_r;
    GemVal _t1462[] = {gem_table_get(_t1461, gem_string("expr"))};
    gem_table_set(_t1459, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t1460, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1462, 1)), gem_string(")")));
    GemVal _t1463 = gem_v_rc_r;
    gem_table_set(_t1459, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1463, gem_string("setup"))));
                                                return _t1459;
                                            } else {
#line 771 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 772 "compiler/main.gem"
    GemVal _t1464 = gem_v_node;
    GemVal _t1465 = (*gem_v_compile_expr);
    GemVal _t1466[] = {gem_table_get(_t1464, gem_string("right"))};
                                                    GemVal gem_v_rc_r = _t1465.fn(_t1465.env, _t1466, 1);
#line 773 "compiler/main.gem"
    GemVal _t1467 = (*gem_v_tmp);
                                                    GemVal gem_v_t = _t1467.fn(_t1467.env, NULL, 0);
#line 774 "compiler/main.gem"
                                                    GemVal gem_v_setup = gem_v_ls;
#line 775 "compiler/main.gem"
    GemVal _t1468[] = {gem_v_t};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1468, 1)), gem_string(";\n")));
#line 776 "compiler/main.gem"
    GemVal _t1469[] = {gem_v_lc};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t1469, 1)), gem_string(")) {\n")));
#line 777 "compiler/main.gem"
    GemVal _t1470[] = {gem_v_t};
    GemVal _t1471[] = {gem_v_lc};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1470, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1471, 1)), gem_string(";\n")));
#line 778 "compiler/main.gem"
                                                    gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 779 "compiler/main.gem"
    GemVal _t1472 = gem_v_rc_r;
                                                    if (gem_truthy(gem_neq(gem_table_get(_t1472, gem_string("setup")), gem_string("")))) {
#line 780 "compiler/main.gem"
    GemVal _t1473 = gem_v_rc_r;
    GemVal _t1474[] = {gem_table_get(_t1473, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_fn_str_replace_all(NULL, _t1474, 3));
                                                    }
#line 782 "compiler/main.gem"
    GemVal _t1475[] = {gem_v_t};
    GemVal _t1476 = gem_v_rc_r;
    GemVal _t1477[] = {gem_table_get(_t1476, gem_string("expr"))};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1475, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1477, 1)), gem_string(";\n")));
#line 783 "compiler/main.gem"
                                                    gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 784 "compiler/main.gem"
    GemVal _t1478 = gem_table_new();
    gem_table_set(_t1478, gem_string("expr"), gem_v_t);
    gem_table_set(_t1478, gem_string("setup"), gem_v_setup);
                                                    return _t1478;
                                                } else {
#line 785 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 786 "compiler/main.gem"
    GemVal _t1479 = gem_v_node;
    GemVal _t1480 = (*gem_v_compile_expr);
    GemVal _t1481[] = {gem_table_get(_t1479, gem_string("right"))};
                                                        GemVal gem_v_rc_r = _t1480.fn(_t1480.env, _t1481, 1);
#line 787 "compiler/main.gem"
    GemVal _t1482 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1482.fn(_t1482.env, NULL, 0);
#line 788 "compiler/main.gem"
                                                        GemVal gem_v_setup = gem_v_ls;
#line 789 "compiler/main.gem"
    GemVal _t1483[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1483, 1)), gem_string(";\n")));
#line 790 "compiler/main.gem"
    GemVal _t1484[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t1484, 1)), gem_string(")) {\n")));
#line 791 "compiler/main.gem"
    GemVal _t1485[] = {gem_v_t};
    GemVal _t1486[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1485, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1486, 1)), gem_string(";\n")));
#line 792 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 793 "compiler/main.gem"
    GemVal _t1487 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t1487, gem_string("setup")), gem_string("")))) {
#line 794 "compiler/main.gem"
    GemVal _t1488 = gem_v_rc_r;
    GemVal _t1489[] = {gem_table_get(_t1488, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_fn_str_replace_all(NULL, _t1489, 3));
                                                        }
#line 796 "compiler/main.gem"
    GemVal _t1490[] = {gem_v_t};
    GemVal _t1491 = gem_v_rc_r;
    GemVal _t1492[] = {gem_table_get(_t1491, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1490, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1492, 1)), gem_string(";\n")));
#line 797 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 798 "compiler/main.gem"
    GemVal _t1493 = gem_table_new();
    gem_table_set(_t1493, gem_string("expr"), gem_v_t);
    gem_table_set(_t1493, gem_string("setup"), gem_v_setup);
                                                        return _t1493;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 801 "compiler/main.gem"
    GemVal _t1494 = gem_v_node;
    GemVal _t1495 = (*gem_v_compile_expr);
    GemVal _t1496[] = {gem_table_get(_t1494, gem_string("right"))};
    GemVal gem_v_rc_r = _t1495.fn(_t1495.env, _t1496, 1);
    GemVal _t1497 = gem_table_new();
    gem_table_set(_t1497, gem_string("expr"), gem_string("GEM_NIL"));
    GemVal _t1498 = gem_v_rc_r;
    gem_table_set(_t1497, gem_string("setup"), gem_add(gem_add(gem_add(gem_add(gem_v_ls, gem_table_get(_t1498, gem_string("setup"))), gem_string("/* unknown op: ")), gem_v_op), gem_string(" */\n")));
    return _t1497;
}

struct _closure__anon_36 {
    GemVal *gem_v_compile_stmt;
};
static GemVal _anon_36(void *_env, GemVal *args, int argc) {
    struct _closure__anon_36 *_cls = (struct _closure__anon_36 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
#line 808 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
    {
#line 809 "compiler/main.gem"
        GemVal gem_v__for_items_32 = gem_v_stmts;
#line 809 "compiler/main.gem"
        GemVal gem_v__for_i_32 = gem_int(0);
#line 809 "compiler/main.gem"
        while (1) {
            GemVal _t1500[] = {gem_v__for_items_32};
            if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_len_fn(NULL, _t1500, 1)))) break;
#line 809 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_32, gem_v__for_i_32);
#line 809 "compiler/main.gem"
            gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 810 "compiler/main.gem"
    GemVal _t1501 = (*gem_v_compile_stmt);
    GemVal _t1502[] = {gem_v_stmt, gem_v_indent};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1501.fn(_t1501.env, _t1502, 2)), gem_string("\n"));
        }
    }
    return gem_v_out;
}

struct _closure__anon_37 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_if;
    GemVal *gem_v_compile_match;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_while;
    GemVal *gem_v_mangle;
    GemVal *gem_v_source_name;
};
static GemVal _anon_37(void *_env, GemVal *args, int argc) {
    struct _closure__anon_37 *_cls = (struct _closure__anon_37 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_if = _cls->gem_v_compile_if;
    GemVal *gem_v_compile_match = _cls->gem_v_compile_match;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_while = _cls->gem_v_compile_while;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
#line 818 "compiler/main.gem"
    GemVal _t1504[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1504, 1);
#line 819 "compiler/main.gem"
    GemVal _t1505 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1505, gem_string("tag"));
#line 820 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 821 "compiler/main.gem"
    GemVal _t1506 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1506, gem_string("line")), GEM_NIL))) {
#line 822 "compiler/main.gem"
    GemVal _t1507 = gem_v_node;
    GemVal _t1508[] = {gem_table_get(_t1507, gem_string("line"))};
    GemVal _t1509[] = {(*gem_v_source_name)};
    GemVal _t1510[] = {gem_fn_escape_c_string(NULL, _t1509, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t1508, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t1510, 1)), gem_string("\"\n"));
    }
#line 825 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 826 "compiler/main.gem"
    GemVal _t1511 = gem_v_node;
    GemVal _t1512 = (*gem_v_compile_expr);
    GemVal _t1513[] = {gem_table_get(_t1511, gem_string("value"))};
        GemVal gem_v_r = _t1512.fn(_t1512.env, _t1513, 1);
#line 827 "compiler/main.gem"
    GemVal _t1514 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1514, gem_string("name"));
#line 828 "compiler/main.gem"
    GemVal _t1515 = (*gem_v_mangle);
    GemVal _t1516[] = {gem_v_name};
        GemVal gem_v_mname = _t1515.fn(_t1515.env, _t1516, 1);
#line 829 "compiler/main.gem"
    GemVal _t1517[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1517, 2))) {
#line 830 "compiler/main.gem"
    GemVal _t1518[] = {gem_v_line_dir};
    GemVal _t1519 = gem_v_r;
    GemVal _t1520[] = {gem_table_get(_t1519, gem_string("setup"))};
    GemVal _t1521[] = {gem_v_p};
    GemVal _t1522[] = {gem_v_mname};
    GemVal _t1523[] = {gem_v_p};
    GemVal _t1524[] = {gem_v_mname};
    GemVal _t1525 = gem_v_r;
    GemVal _t1526[] = {gem_table_get(_t1525, gem_string("expr"))};
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1518, 1), gem_to_string_fn(NULL, _t1520, 1)), gem_to_string_fn(NULL, _t1521, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t1522, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1523, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1524, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1526, 1)), gem_string(";"));
        }
#line 832 "compiler/main.gem"
    GemVal _t1527[] = {gem_v_line_dir};
    GemVal _t1528 = gem_v_r;
    GemVal _t1529[] = {gem_table_get(_t1528, gem_string("setup"))};
    GemVal _t1530[] = {gem_v_p};
    GemVal _t1531[] = {gem_v_mname};
    GemVal _t1532 = gem_v_r;
    GemVal _t1533[] = {gem_table_get(_t1532, gem_string("expr"))};
        return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1527, 1), gem_to_string_fn(NULL, _t1529, 1)), gem_to_string_fn(NULL, _t1530, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1531, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1533, 1)), gem_string(";"));
    } else {
#line 833 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 834 "compiler/main.gem"
    GemVal _t1534 = gem_v_node;
    GemVal _t1535 = (*gem_v_compile_expr);
    GemVal _t1536[] = {gem_table_get(_t1534, gem_string("value"))};
            GemVal gem_v_r = _t1535.fn(_t1535.env, _t1536, 1);
#line 835 "compiler/main.gem"
    GemVal _t1537 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1537, gem_string("name"));
#line 836 "compiler/main.gem"
    GemVal _t1538 = (*gem_v_mangle);
    GemVal _t1539[] = {gem_v_name};
            GemVal gem_v_mname = _t1538.fn(_t1538.env, _t1539, 1);
#line 837 "compiler/main.gem"
    GemVal _t1540[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1540, 2))) {
#line 838 "compiler/main.gem"
    GemVal _t1541[] = {gem_v_line_dir};
    GemVal _t1542 = gem_v_r;
    GemVal _t1543[] = {gem_table_get(_t1542, gem_string("setup"))};
    GemVal _t1544[] = {gem_v_p};
    GemVal _t1545[] = {gem_v_mname};
    GemVal _t1546 = gem_v_r;
    GemVal _t1547[] = {gem_table_get(_t1546, gem_string("expr"))};
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1541, 1), gem_to_string_fn(NULL, _t1543, 1)), gem_to_string_fn(NULL, _t1544, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1545, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1547, 1)), gem_string(";"));
            }
#line 840 "compiler/main.gem"
    GemVal _t1548[] = {gem_v_line_dir};
    GemVal _t1549 = gem_v_r;
    GemVal _t1550[] = {gem_table_get(_t1549, gem_string("setup"))};
    GemVal _t1551[] = {gem_v_p};
    GemVal _t1552[] = {gem_v_mname};
    GemVal _t1553 = gem_v_r;
    GemVal _t1554[] = {gem_table_get(_t1553, gem_string("expr"))};
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1548, 1), gem_to_string_fn(NULL, _t1550, 1)), gem_to_string_fn(NULL, _t1551, 1)), gem_to_string_fn(NULL, _t1552, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1554, 1)), gem_string(";"));
        } else {
#line 841 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
#line 842 "compiler/main.gem"
    GemVal _t1555 = gem_v_node;
    GemVal _t1556 = (*gem_v_compile_expr);
    GemVal _t1557[] = {gem_table_get(_t1555, gem_string("object"))};
                GemVal gem_v_obj_r = _t1556.fn(_t1556.env, _t1557, 1);
#line 843 "compiler/main.gem"
    GemVal _t1558 = gem_v_node;
    GemVal _t1559 = (*gem_v_compile_expr);
    GemVal _t1560[] = {gem_table_get(_t1558, gem_string("value"))};
                GemVal gem_v_val_r = _t1559.fn(_t1559.env, _t1560, 1);
#line 844 "compiler/main.gem"
    GemVal _t1561 = gem_v_node;
    GemVal _t1562[] = {gem_table_get(_t1561, gem_string("field"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1562, 1);
#line 845 "compiler/main.gem"
    GemVal _t1563[] = {gem_v_line_dir};
    GemVal _t1564 = gem_v_obj_r;
    GemVal _t1565[] = {gem_table_get(_t1564, gem_string("setup"))};
    GemVal _t1566 = gem_v_val_r;
    GemVal _t1567[] = {gem_table_get(_t1566, gem_string("setup"))};
    GemVal _t1568[] = {gem_v_p};
    GemVal _t1569 = gem_v_obj_r;
    GemVal _t1570[] = {gem_table_get(_t1569, gem_string("expr"))};
    GemVal _t1571[] = {gem_v_escaped};
    GemVal _t1572 = gem_v_val_r;
    GemVal _t1573[] = {gem_table_get(_t1572, gem_string("expr"))};
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1563, 1), gem_to_string_fn(NULL, _t1565, 1)), gem_to_string_fn(NULL, _t1567, 1)), gem_to_string_fn(NULL, _t1568, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t1570, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1571, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1573, 1)), gem_string(");"));
            } else {
#line 846 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("index_assign")))) {
#line 847 "compiler/main.gem"
    GemVal _t1574 = gem_v_node;
    GemVal _t1575 = (*gem_v_compile_expr);
    GemVal _t1576[] = {gem_table_get(_t1574, gem_string("object"))};
                    GemVal gem_v_obj_r = _t1575.fn(_t1575.env, _t1576, 1);
#line 848 "compiler/main.gem"
    GemVal _t1577 = gem_v_node;
    GemVal _t1578 = (*gem_v_compile_expr);
    GemVal _t1579[] = {gem_table_get(_t1577, gem_string("key"))};
                    GemVal gem_v_key_r = _t1578.fn(_t1578.env, _t1579, 1);
#line 849 "compiler/main.gem"
    GemVal _t1580 = gem_v_node;
    GemVal _t1581 = (*gem_v_compile_expr);
    GemVal _t1582[] = {gem_table_get(_t1580, gem_string("value"))};
                    GemVal gem_v_val_r = _t1581.fn(_t1581.env, _t1582, 1);
#line 850 "compiler/main.gem"
    GemVal _t1583[] = {gem_v_line_dir};
    GemVal _t1584 = gem_v_obj_r;
    GemVal _t1585[] = {gem_table_get(_t1584, gem_string("setup"))};
    GemVal _t1586 = gem_v_key_r;
    GemVal _t1587[] = {gem_table_get(_t1586, gem_string("setup"))};
    GemVal _t1588 = gem_v_val_r;
    GemVal _t1589[] = {gem_table_get(_t1588, gem_string("setup"))};
    GemVal _t1590[] = {gem_v_p};
    GemVal _t1591 = gem_v_obj_r;
    GemVal _t1592[] = {gem_table_get(_t1591, gem_string("expr"))};
    GemVal _t1593 = gem_v_key_r;
    GemVal _t1594[] = {gem_table_get(_t1593, gem_string("expr"))};
    GemVal _t1595 = gem_v_val_r;
    GemVal _t1596[] = {gem_table_get(_t1595, gem_string("expr"))};
                    return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1583, 1), gem_to_string_fn(NULL, _t1585, 1)), gem_to_string_fn(NULL, _t1587, 1)), gem_to_string_fn(NULL, _t1589, 1)), gem_to_string_fn(NULL, _t1590, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t1592, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1594, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1596, 1)), gem_string(");"));
                } else {
#line 851 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 852 "compiler/main.gem"
    GemVal _t1597[] = {gem_v_line_dir};
    GemVal _t1598 = (*gem_v_compile_if);
    GemVal _t1599[] = {gem_v_node, gem_v_indent};
                        return gem_add(gem_to_string_fn(NULL, _t1597, 1), _t1598.fn(_t1598.env, _t1599, 2));
                    } else {
#line 853 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 854 "compiler/main.gem"
    GemVal _t1600[] = {gem_v_line_dir};
    GemVal _t1601 = (*gem_v_compile_while);
    GemVal _t1602[] = {gem_v_node, gem_v_indent};
                            return gem_add(gem_to_string_fn(NULL, _t1600, 1), _t1601.fn(_t1601.env, _t1602, 2));
                        } else {
#line 855 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 856 "compiler/main.gem"
    GemVal _t1603[] = {gem_v_line_dir};
    GemVal _t1604 = (*gem_v_compile_match);
    GemVal _t1605[] = {gem_v_node, gem_v_indent};
                                return gem_add(gem_to_string_fn(NULL, _t1603, 1), _t1604.fn(_t1604.env, _t1605, 2));
                            } else {
#line 857 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("break")))) {
#line 858 "compiler/main.gem"
                                    return gem_add(gem_v_p, gem_string("break;"));
                                } else {
#line 859 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("continue")))) {
#line 860 "compiler/main.gem"
                                        return gem_add(gem_v_p, gem_string("continue;"));
                                    } else {
#line 861 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 862 "compiler/main.gem"
    GemVal _t1606 = gem_v_node;
                                            if (gem_truthy(gem_neq(gem_table_get(_t1606, gem_string("value")), GEM_NIL))) {
#line 863 "compiler/main.gem"
    GemVal _t1607 = gem_v_node;
    GemVal _t1608 = (*gem_v_compile_expr);
    GemVal _t1609[] = {gem_table_get(_t1607, gem_string("value"))};
                                                GemVal gem_v_r = _t1608.fn(_t1608.env, _t1609, 1);
#line 864 "compiler/main.gem"
    GemVal _t1610[] = {gem_v_line_dir};
    GemVal _t1611 = gem_v_r;
    GemVal _t1612[] = {gem_table_get(_t1611, gem_string("setup"))};
    GemVal _t1613[] = {gem_v_p};
    GemVal _t1614 = gem_v_r;
    GemVal _t1615[] = {gem_table_get(_t1614, gem_string("expr"))};
                                                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1610, 1), gem_to_string_fn(NULL, _t1612, 1)), gem_to_string_fn(NULL, _t1613, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1615, 1)), gem_string(";"));
                                            }
#line 866 "compiler/main.gem"
    GemVal _t1616[] = {gem_v_line_dir};
    GemVal _t1617[] = {gem_v_p};
                                            return gem_add(gem_add(gem_to_string_fn(NULL, _t1616, 1), gem_to_string_fn(NULL, _t1617, 1)), gem_string("return GEM_NIL;"));
                                        } else {
#line 867 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 868 "compiler/main.gem"
    GemVal _t1618 = (*gem_v_compile_expr);
    GemVal _t1619[] = {gem_v_node};
                                                GemVal gem_v_r = _t1618.fn(_t1618.env, _t1619, 1);
#line 869 "compiler/main.gem"
    GemVal _t1620[] = {gem_v_line_dir};
    GemVal _t1621 = gem_v_r;
    GemVal _t1622[] = {gem_table_get(_t1621, gem_string("setup"))};
    GemVal _t1623[] = {gem_v_p};
    GemVal _t1624 = gem_v_r;
    GemVal _t1625[] = {gem_table_get(_t1624, gem_string("expr"))};
                                                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1620, 1), gem_to_string_fn(NULL, _t1622, 1)), gem_to_string_fn(NULL, _t1623, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t1625, 1)), gem_string(");"));
                                            } else {
#line 870 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
#line 871 "compiler/main.gem"
                                                    return gem_string("");
                                                } else {
#line 872 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 873 "compiler/main.gem"
    GemVal _t1626[] = {gem_v_p};
                                                        GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t1626, 1), gem_string("{\n"));
                                                        {
#line 874 "compiler/main.gem"
    GemVal _t1627 = gem_v_node;
                                                            GemVal gem_v__for_items_33 = gem_table_get(_t1627, gem_string("stmts"));
#line 874 "compiler/main.gem"
                                                            GemVal gem_v__for_i_33 = gem_int(0);
#line 874 "compiler/main.gem"
                                                            while (1) {
                                                                GemVal _t1628[] = {gem_v__for_items_33};
                                                                if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_len_fn(NULL, _t1628, 1)))) break;
#line 874 "compiler/main.gem"
                                                                GemVal gem_v_stmt = gem_table_get(gem_v__for_items_33, gem_v__for_i_33);
#line 874 "compiler/main.gem"
                                                                gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 875 "compiler/main.gem"
    GemVal _t1629 = (*gem_v_compile_stmt);
    GemVal _t1630[] = {gem_v_stmt, gem_add(gem_v_indent, gem_int(1))};
                                                                gem_v_out = gem_add(gem_add(gem_v_out, _t1629.fn(_t1629.env, _t1630, 2)), gem_string("\n"));
                                                            }
                                                        }
#line 877 "compiler/main.gem"
    GemVal _t1631[] = {gem_v_p};
                                                        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1631, 1), gem_string("}")));
#line 878 "compiler/main.gem"
                                                        return gem_v_out;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 881 "compiler/main.gem"
    GemVal _t1632 = (*gem_v_compile_expr);
    GemVal _t1633[] = {gem_v_node};
    GemVal gem_v_r = _t1632.fn(_t1632.env, _t1633, 1);
    GemVal _t1634[] = {gem_v_line_dir};
    GemVal _t1635 = gem_v_r;
    GemVal _t1636[] = {gem_table_get(_t1635, gem_string("setup"))};
    GemVal _t1637[] = {gem_v_p};
    GemVal _t1638 = gem_v_r;
    GemVal _t1639[] = {gem_table_get(_t1638, gem_string("expr"))};
    return gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1634, 1), gem_to_string_fn(NULL, _t1636, 1)), gem_to_string_fn(NULL, _t1637, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t1639, 1)), gem_string(");"));
}

struct _closure__anon_38 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_if_return;
    GemVal *gem_v_compile_match_return;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_compile_while;
    GemVal *gem_v_mangle;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tmp;
};
static GemVal _anon_38(void *_env, GemVal *args, int argc) {
    struct _closure__anon_38 *_cls = (struct _closure__anon_38 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_if_return = _cls->gem_v_compile_if_return;
    GemVal *gem_v_compile_match_return = _cls->gem_v_compile_match_return;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_compile_while = _cls->gem_v_compile_while;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
#line 888 "compiler/main.gem"
    GemVal _t1641[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1641, 1);
#line 889 "compiler/main.gem"
    GemVal _t1642 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1642, gem_string("tag"));
#line 890 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 891 "compiler/main.gem"
    GemVal _t1643 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1643, gem_string("line")), GEM_NIL))) {
#line 892 "compiler/main.gem"
    GemVal _t1644 = gem_v_node;
    GemVal _t1645[] = {gem_table_get(_t1644, gem_string("line"))};
    GemVal _t1646[] = {(*gem_v_source_name)};
    GemVal _t1647[] = {gem_fn_escape_c_string(NULL, _t1646, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t1645, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t1647, 1)), gem_string("\"\n"));
    }
#line 895 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 896 "compiler/main.gem"
    GemVal _t1648 = gem_v_node;
    GemVal _t1649 = (*gem_v_compile_expr);
    GemVal _t1650[] = {gem_table_get(_t1648, gem_string("value"))};
        GemVal gem_v_r = _t1649.fn(_t1649.env, _t1650, 1);
#line 897 "compiler/main.gem"
    GemVal _t1651 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1651, gem_string("name"));
#line 898 "compiler/main.gem"
    GemVal _t1652 = (*gem_v_mangle);
    GemVal _t1653[] = {gem_v_name};
        GemVal gem_v_mname = _t1652.fn(_t1652.env, _t1653, 1);
#line 899 "compiler/main.gem"
    GemVal _t1654[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1654, 2))) {
#line 900 "compiler/main.gem"
    GemVal _t1655[] = {gem_v_line_dir};
    GemVal _t1656 = gem_v_r;
    GemVal _t1657[] = {gem_table_get(_t1656, gem_string("setup"))};
    GemVal _t1658[] = {gem_v_p};
    GemVal _t1659[] = {gem_v_mname};
    GemVal _t1660[] = {gem_v_p};
    GemVal _t1661[] = {gem_v_mname};
    GemVal _t1662 = gem_v_r;
    GemVal _t1663[] = {gem_table_get(_t1662, gem_string("expr"))};
    GemVal _t1664[] = {gem_v_p};
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1655, 1), gem_to_string_fn(NULL, _t1657, 1)), gem_to_string_fn(NULL, _t1658, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t1659, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1660, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1661, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1663, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1664, 1)), gem_string("return GEM_NIL;"));
        }
#line 902 "compiler/main.gem"
    GemVal _t1665[] = {gem_v_line_dir};
    GemVal _t1666 = gem_v_r;
    GemVal _t1667[] = {gem_table_get(_t1666, gem_string("setup"))};
    GemVal _t1668[] = {gem_v_p};
    GemVal _t1669[] = {gem_v_mname};
    GemVal _t1670 = gem_v_r;
    GemVal _t1671[] = {gem_table_get(_t1670, gem_string("expr"))};
    GemVal _t1672[] = {gem_v_p};
        return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1665, 1), gem_to_string_fn(NULL, _t1667, 1)), gem_to_string_fn(NULL, _t1668, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1669, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1671, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1672, 1)), gem_string("return GEM_NIL;"));
    } else {
#line 903 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 904 "compiler/main.gem"
    GemVal _t1673 = gem_v_node;
    GemVal _t1674 = (*gem_v_compile_expr);
    GemVal _t1675[] = {gem_table_get(_t1673, gem_string("value"))};
            GemVal gem_v_r = _t1674.fn(_t1674.env, _t1675, 1);
#line 905 "compiler/main.gem"
    GemVal _t1676 = (*gem_v_tmp);
            GemVal gem_v_t = _t1676.fn(_t1676.env, NULL, 0);
#line 906 "compiler/main.gem"
    GemVal _t1677 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1677, gem_string("name"));
#line 907 "compiler/main.gem"
    GemVal _t1678 = (*gem_v_mangle);
    GemVal _t1679[] = {gem_v_name};
            GemVal gem_v_mname = _t1678.fn(_t1678.env, _t1679, 1);
#line 908 "compiler/main.gem"
    GemVal _t1680[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1680, 2))) {
#line 909 "compiler/main.gem"
    GemVal _t1681[] = {gem_v_line_dir};
    GemVal _t1682 = gem_v_r;
    GemVal _t1683[] = {gem_table_get(_t1682, gem_string("setup"))};
    GemVal _t1684[] = {gem_v_p};
    GemVal _t1685[] = {gem_v_t};
    GemVal _t1686 = gem_v_r;
    GemVal _t1687[] = {gem_table_get(_t1686, gem_string("expr"))};
    GemVal _t1688[] = {gem_v_p};
    GemVal _t1689[] = {gem_v_mname};
    GemVal _t1690[] = {gem_v_t};
    GemVal _t1691[] = {gem_v_p};
    GemVal _t1692[] = {gem_v_t};
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1681, 1), gem_to_string_fn(NULL, _t1683, 1)), gem_to_string_fn(NULL, _t1684, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1685, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1687, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1688, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1689, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1690, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1691, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1692, 1)), gem_string(";"));
            }
#line 911 "compiler/main.gem"
    GemVal _t1693[] = {gem_v_line_dir};
    GemVal _t1694 = gem_v_r;
    GemVal _t1695[] = {gem_table_get(_t1694, gem_string("setup"))};
    GemVal _t1696[] = {gem_v_p};
    GemVal _t1697[] = {gem_v_t};
    GemVal _t1698 = gem_v_r;
    GemVal _t1699[] = {gem_table_get(_t1698, gem_string("expr"))};
    GemVal _t1700[] = {gem_v_p};
    GemVal _t1701[] = {gem_v_mname};
    GemVal _t1702[] = {gem_v_t};
    GemVal _t1703[] = {gem_v_p};
    GemVal _t1704[] = {gem_v_t};
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1693, 1), gem_to_string_fn(NULL, _t1695, 1)), gem_to_string_fn(NULL, _t1696, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1697, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1699, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1700, 1)), gem_to_string_fn(NULL, _t1701, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1702, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1703, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1704, 1)), gem_string(";"));
        } else {
#line 912 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 913 "compiler/main.gem"
    GemVal _t1705 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t1705, gem_string("value")), GEM_NIL))) {
#line 914 "compiler/main.gem"
    GemVal _t1706 = gem_v_node;
    GemVal _t1707 = (*gem_v_compile_expr);
    GemVal _t1708[] = {gem_table_get(_t1706, gem_string("value"))};
                    GemVal gem_v_r = _t1707.fn(_t1707.env, _t1708, 1);
#line 915 "compiler/main.gem"
    GemVal _t1709[] = {gem_v_line_dir};
    GemVal _t1710 = gem_v_r;
    GemVal _t1711[] = {gem_table_get(_t1710, gem_string("setup"))};
    GemVal _t1712[] = {gem_v_p};
    GemVal _t1713 = gem_v_r;
    GemVal _t1714[] = {gem_table_get(_t1713, gem_string("expr"))};
                    return gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1709, 1), gem_to_string_fn(NULL, _t1711, 1)), gem_to_string_fn(NULL, _t1712, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1714, 1)), gem_string(";"));
                }
#line 917 "compiler/main.gem"
    GemVal _t1715[] = {gem_v_line_dir};
    GemVal _t1716[] = {gem_v_p};
                return gem_add(gem_add(gem_to_string_fn(NULL, _t1715, 1), gem_to_string_fn(NULL, _t1716, 1)), gem_string("return GEM_NIL;"));
            } else {
#line 918 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 919 "compiler/main.gem"
    GemVal _t1717[] = {gem_v_line_dir};
    GemVal _t1718 = (*gem_v_compile_if_return);
    GemVal _t1719[] = {gem_v_node, gem_v_indent};
                    return gem_add(gem_to_string_fn(NULL, _t1717, 1), _t1718.fn(_t1718.env, _t1719, 2));
                } else {
#line 920 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 921 "compiler/main.gem"
    GemVal _t1720[] = {gem_v_line_dir};
    GemVal _t1721 = (*gem_v_compile_while);
    GemVal _t1722[] = {gem_v_node, gem_v_indent};
    GemVal _t1723[] = {_t1721.fn(_t1721.env, _t1722, 2)};
    GemVal _t1724[] = {gem_v_p};
                        return gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1720, 1), gem_to_string_fn(NULL, _t1723, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t1724, 1)), gem_string("return GEM_NIL;"));
                    } else {
#line 922 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 923 "compiler/main.gem"
    GemVal _t1725[] = {gem_v_line_dir};
    GemVal _t1726 = (*gem_v_compile_match_return);
    GemVal _t1727[] = {gem_v_node, gem_v_indent};
                            return gem_add(gem_to_string_fn(NULL, _t1725, 1), _t1726.fn(_t1726.env, _t1727, 2));
                        } else {
#line 924 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 925 "compiler/main.gem"
    GemVal _t1728[] = {gem_v_p};
                                GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t1728, 1), gem_string("{\n"));
                                {
#line 926 "compiler/main.gem"
                                    GemVal gem_v__for_i_34 = gem_int(0);
#line 926 "compiler/main.gem"
    GemVal _t1729 = gem_v_node;
    GemVal _t1730[] = {gem_table_get(_t1729, gem_string("stmts"))};
                                    GemVal gem_v__for_limit_34 = gem_sub(gem_len_fn(NULL, _t1730, 1), gem_int(1));
#line 926 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
#line 926 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_34;
#line 926 "compiler/main.gem"
                                        gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 927 "compiler/main.gem"
    GemVal _t1731 = gem_v_node;
    GemVal _t1732 = (*gem_v_compile_stmt);
    GemVal _t1733[] = {gem_table_get(gem_table_get(_t1731, gem_string("stmts")), gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                                        gem_v_out = gem_add(gem_add(gem_v_out, _t1732.fn(_t1732.env, _t1733, 2)), gem_string("\n"));
                                    }
                                }
#line 929 "compiler/main.gem"
    GemVal _t1734 = gem_v_node;
    GemVal _t1735[] = {gem_table_get(_t1734, gem_string("stmts"))};
                                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1735, 1), gem_int(0)))) {
#line 930 "compiler/main.gem"
    GemVal _t1736 = gem_v_node;
    GemVal _t1737 = gem_v_node;
    GemVal _t1738[] = {gem_table_get(_t1737, gem_string("stmts"))};
    GemVal _t1739 = (*gem_v_compile_stmt_return);
    GemVal _t1740[] = {gem_table_get(gem_table_get(_t1736, gem_string("stmts")), gem_sub(gem_len_fn(NULL, _t1738, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t1739.fn(_t1739.env, _t1740, 2)), gem_string("\n"));
                                }
#line 932 "compiler/main.gem"
    GemVal _t1741[] = {gem_v_p};
                                gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1741, 1), gem_string("}")));
#line 933 "compiler/main.gem"
                                return gem_v_out;
                            } else {
#line 934 "compiler/main.gem"
    GemVal _t1742;
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
        _t1742 = gem_eq(gem_v_tag, gem_string("dot_assign"));
    } else {
        _t1742 = gem_eq(gem_v_tag, gem_string("index_assign"));
    }
    GemVal _t1743;
    if (gem_truthy(_t1742)) {
        _t1743 = _t1742;
    } else {
        _t1743 = gem_eq(gem_v_tag, gem_string("break"));
    }
    GemVal _t1744;
    if (gem_truthy(_t1743)) {
        _t1744 = _t1743;
    } else {
        _t1744 = gem_eq(gem_v_tag, gem_string("continue"));
    }
    GemVal _t1745;
    if (gem_truthy(_t1744)) {
        _t1745 = _t1744;
    } else {
        _t1745 = gem_eq(gem_v_tag, gem_string("fn_def"));
    }
                                if (gem_truthy(_t1745)) {
#line 935 "compiler/main.gem"
    GemVal _t1746 = (*gem_v_compile_stmt);
    GemVal _t1747[] = {gem_v_node, gem_v_indent};
    GemVal _t1748[] = {_t1746.fn(_t1746.env, _t1747, 2)};
    GemVal _t1749[] = {gem_v_p};
                                    return gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1748, 1), gem_string("\n")), gem_to_string_fn(NULL, _t1749, 1)), gem_string("return GEM_NIL;"));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 938 "compiler/main.gem"
    GemVal _t1750 = (*gem_v_compile_expr);
    GemVal _t1751[] = {gem_v_node};
    GemVal gem_v_r = _t1750.fn(_t1750.env, _t1751, 1);
    GemVal _t1752[] = {gem_v_line_dir};
    GemVal _t1753 = gem_v_r;
    GemVal _t1754[] = {gem_table_get(_t1753, gem_string("setup"))};
    GemVal _t1755[] = {gem_v_p};
    GemVal _t1756 = gem_v_r;
    GemVal _t1757[] = {gem_table_get(_t1756, gem_string("expr"))};
    return gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1752, 1), gem_to_string_fn(NULL, _t1754, 1)), gem_to_string_fn(NULL, _t1755, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1757, 1)), gem_string(";"));
}

struct _closure__anon_39 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts;
};
static GemVal _anon_39(void *_env, GemVal *args, int argc) {
    struct _closure__anon_39 *_cls = (struct _closure__anon_39 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
#line 945 "compiler/main.gem"
    GemVal _t1759[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1759, 1);
#line 946 "compiler/main.gem"
    GemVal _t1760 = gem_v_node;
    GemVal _t1761 = (*gem_v_compile_expr);
    GemVal _t1762[] = {gem_table_get(_t1760, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1761.fn(_t1761.env, _t1762, 1);
#line 947 "compiler/main.gem"
    GemVal _t1763 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t1763, gem_string("setup"));
#line 948 "compiler/main.gem"
    GemVal _t1764[] = {gem_v_p};
    GemVal _t1765 = gem_v_cond_r;
    GemVal _t1766[] = {gem_table_get(_t1765, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1764, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t1766, 1)), gem_string(")) {\n")));
#line 949 "compiler/main.gem"
    GemVal _t1767 = (*gem_v_compile_stmts);
    GemVal _t1768[] = {gem_table_get(gem_v_node, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t1767.fn(_t1767.env, _t1768, 2));
#line 950 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 951 "compiler/main.gem"
    GemVal _t1769[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1769, 1), gem_string("} else {\n")));
#line 952 "compiler/main.gem"
    GemVal _t1770 = (*gem_v_compile_stmts);
    GemVal _t1771[] = {gem_table_get(gem_v_node, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t1770.fn(_t1770.env, _t1771, 2));
    }
#line 954 "compiler/main.gem"
    GemVal _t1772[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1772, 1), gem_string("}")));
    return gem_v_out;
}

struct _closure__anon_40 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
};
static GemVal _anon_40(void *_env, GemVal *args, int argc) {
    struct _closure__anon_40 *_cls = (struct _closure__anon_40 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
#line 961 "compiler/main.gem"
    GemVal _t1774[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1774, 1);
#line 962 "compiler/main.gem"
    GemVal _t1775 = gem_v_node;
    GemVal _t1776 = (*gem_v_compile_expr);
    GemVal _t1777[] = {gem_table_get(_t1775, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1776.fn(_t1776.env, _t1777, 1);
#line 963 "compiler/main.gem"
    GemVal _t1778 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t1778, gem_string("setup"));
#line 964 "compiler/main.gem"
    GemVal _t1779[] = {gem_v_p};
    GemVal _t1780 = gem_v_cond_r;
    GemVal _t1781[] = {gem_table_get(_t1780, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1779, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t1781, 1)), gem_string(")) {\n")));
#line 965 "compiler/main.gem"
    GemVal gem_v_then_body = gem_table_get(gem_v_node, gem_string("then"));
#line 966 "compiler/main.gem"
    GemVal _t1782[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1782, 1), gem_int(0)))) {
        {
#line 967 "compiler/main.gem"
            GemVal gem_v__for_i_35 = gem_int(0);
#line 967 "compiler/main.gem"
    GemVal _t1783[] = {gem_v_then_body};
            GemVal gem_v__for_limit_35 = gem_sub(gem_len_fn(NULL, _t1783, 1), gem_int(1));
#line 967 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 967 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_35;
#line 967 "compiler/main.gem"
                gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 968 "compiler/main.gem"
    GemVal _t1784 = (*gem_v_compile_stmt);
    GemVal _t1785[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t1784.fn(_t1784.env, _t1785, 2)), gem_string("\n"));
            }
        }
#line 970 "compiler/main.gem"
    GemVal _t1786[] = {gem_v_then_body};
    GemVal _t1787 = (*gem_v_compile_stmt_return);
    GemVal _t1788[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t1786, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_add(gem_v_out, _t1787.fn(_t1787.env, _t1788, 2)), gem_string("\n"));
    } else {
#line 972 "compiler/main.gem"
    GemVal _t1789[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1789, 1), gem_string("    return GEM_NIL;\n")));
    }
#line 974 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 975 "compiler/main.gem"
    GemVal _t1790[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1790, 1), gem_string("} else {\n")));
#line 976 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v_node, gem_string("else"));
#line 977 "compiler/main.gem"
    GemVal _t1791[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1791, 1), gem_int(0)))) {
            {
#line 978 "compiler/main.gem"
                GemVal gem_v__for_i_36 = gem_int(0);
#line 978 "compiler/main.gem"
    GemVal _t1792[] = {gem_v_else_body};
                GemVal gem_v__for_limit_36 = gem_sub(gem_len_fn(NULL, _t1792, 1), gem_int(1));
#line 978 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 978 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_36;
#line 978 "compiler/main.gem"
                    gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 979 "compiler/main.gem"
    GemVal _t1793 = (*gem_v_compile_stmt);
    GemVal _t1794[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t1793.fn(_t1793.env, _t1794, 2)), gem_string("\n"));
                }
            }
#line 981 "compiler/main.gem"
    GemVal _t1795[] = {gem_v_else_body};
    GemVal _t1796 = (*gem_v_compile_stmt_return);
    GemVal _t1797[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t1795, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1796.fn(_t1796.env, _t1797, 2)), gem_string("\n"));
        } else {
#line 983 "compiler/main.gem"
    GemVal _t1798[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1798, 1), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 986 "compiler/main.gem"
    GemVal _t1799[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1799, 1), gem_string("} else {\n")));
#line 987 "compiler/main.gem"
    GemVal _t1800[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1800, 1), gem_string("    return GEM_NIL;\n")));
    }
#line 989 "compiler/main.gem"
    GemVal _t1801[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1801, 1), gem_string("}")));
    return gem_v_out;
}

struct _closure__anon_41 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts;
};
static GemVal _anon_41(void *_env, GemVal *args, int argc) {
    struct _closure__anon_41 *_cls = (struct _closure__anon_41 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
#line 996 "compiler/main.gem"
    GemVal _t1803[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1803, 1);
#line 997 "compiler/main.gem"
    GemVal _t1804 = gem_v_node;
    GemVal _t1805 = (*gem_v_compile_expr);
    GemVal _t1806[] = {gem_table_get(_t1804, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1805.fn(_t1805.env, _t1806, 1);
#line 998 "compiler/main.gem"
    GemVal _t1807[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t1807, 1), gem_string("while (1) {\n"));
#line 999 "compiler/main.gem"
    GemVal _t1808 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t1808, gem_string("setup")), gem_string("")))) {
#line 1000 "compiler/main.gem"
    GemVal _t1809 = gem_v_cond_r;
    GemVal _t1810[] = {gem_v_p};
    GemVal _t1811[] = {gem_table_get(_t1809, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t1810, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_fn_str_replace_all(NULL, _t1811, 3));
    }
#line 1002 "compiler/main.gem"
    GemVal _t1812[] = {gem_v_p};
    GemVal _t1813 = gem_v_cond_r;
    GemVal _t1814[] = {gem_table_get(_t1813, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1812, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t1814, 1)), gem_string(")) break;\n")));
#line 1003 "compiler/main.gem"
    GemVal _t1815 = gem_v_node;
    GemVal _t1816 = (*gem_v_compile_stmts);
    GemVal _t1817[] = {gem_table_get(_t1815, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t1816.fn(_t1816.env, _t1817, 2));
#line 1004 "compiler/main.gem"
    GemVal _t1818[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1818, 1), gem_string("}")));
    return gem_v_out;
}

struct _closure__anon_42 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts;
    GemVal *gem_v_tmp;
};
static GemVal _anon_42(void *_env, GemVal *args, int argc) {
    struct _closure__anon_42 *_cls = (struct _closure__anon_42 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
#line 1011 "compiler/main.gem"
    GemVal _t1820[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1820, 1);
#line 1012 "compiler/main.gem"
    GemVal _t1821 = gem_v_node;
    GemVal _t1822 = (*gem_v_compile_expr);
    GemVal _t1823[] = {gem_table_get(_t1821, gem_string("target"))};
    GemVal gem_v_target_r = _t1822.fn(_t1822.env, _t1823, 1);
#line 1013 "compiler/main.gem"
    GemVal _t1824 = (*gem_v_tmp);
    GemVal gem_v_t = _t1824.fn(_t1824.env, NULL, 0);
#line 1014 "compiler/main.gem"
    GemVal _t1825 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t1825, gem_string("setup"));
#line 1015 "compiler/main.gem"
    GemVal _t1826[] = {gem_v_p};
    GemVal _t1827[] = {gem_v_t};
    GemVal _t1828 = gem_v_target_r;
    GemVal _t1829[] = {gem_table_get(_t1828, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1826, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1827, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1829, 1)), gem_string(";\n")));
    {
#line 1016 "compiler/main.gem"
        GemVal gem_v__for_i_37 = gem_int(0);
#line 1016 "compiler/main.gem"
    GemVal _t1830 = gem_v_node;
    GemVal _t1831[] = {gem_table_get(_t1830, gem_string("whens"))};
        GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t1831, 1);
#line 1016 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 1016 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_37;
#line 1016 "compiler/main.gem"
            gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 1017 "compiler/main.gem"
    GemVal _t1832 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t1832, gem_string("whens")), gem_v_i);
#line 1018 "compiler/main.gem"
    GemVal _t1833 = gem_v_w;
    GemVal _t1834 = (*gem_v_compile_expr);
    GemVal _t1835[] = {gem_table_get(_t1833, gem_string("value"))};
            GemVal gem_v_val_r = _t1834.fn(_t1834.env, _t1835, 1);
#line 1019 "compiler/main.gem"
            GemVal gem_v_keyword = gem_string("if");
#line 1020 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1021 "compiler/main.gem"
                gem_v_keyword = gem_string("} else if");
            }
#line 1023 "compiler/main.gem"
    GemVal _t1836 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t1836, gem_string("setup")));
#line 1024 "compiler/main.gem"
    GemVal _t1837[] = {gem_v_p};
    GemVal _t1838[] = {gem_v_keyword};
    GemVal _t1839[] = {gem_v_t};
    GemVal _t1840 = gem_v_val_r;
    GemVal _t1841[] = {gem_table_get(_t1840, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1837, 1), gem_to_string_fn(NULL, _t1838, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t1839, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1841, 1)), gem_string("))) {\n")));
#line 1025 "compiler/main.gem"
    GemVal _t1842 = gem_v_w;
    GemVal _t1843 = (*gem_v_compile_stmts);
    GemVal _t1844[] = {gem_table_get(_t1842, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_v_out, _t1843.fn(_t1843.env, _t1844, 2));
        }
    }
#line 1027 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 1028 "compiler/main.gem"
    GemVal _t1845[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1845, 1), gem_string("} else {\n")));
#line 1029 "compiler/main.gem"
    GemVal _t1846 = (*gem_v_compile_stmts);
    GemVal _t1847[] = {gem_table_get(gem_v_node, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t1846.fn(_t1846.env, _t1847, 2));
    }
#line 1031 "compiler/main.gem"
    GemVal _t1848[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1848, 1), gem_string("}")));
    return gem_v_out;
}

struct _closure__anon_43 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_tmp;
};
static GemVal _anon_43(void *_env, GemVal *args, int argc) {
    struct _closure__anon_43 *_cls = (struct _closure__anon_43 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
#line 1038 "compiler/main.gem"
    GemVal _t1850[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1850, 1);
#line 1039 "compiler/main.gem"
    GemVal _t1851 = gem_v_node;
    GemVal _t1852 = (*gem_v_compile_expr);
    GemVal _t1853[] = {gem_table_get(_t1851, gem_string("target"))};
    GemVal gem_v_target_r = _t1852.fn(_t1852.env, _t1853, 1);
#line 1040 "compiler/main.gem"
    GemVal _t1854 = (*gem_v_tmp);
    GemVal gem_v_t = _t1854.fn(_t1854.env, NULL, 0);
#line 1041 "compiler/main.gem"
    GemVal _t1855 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t1855, gem_string("setup"));
#line 1042 "compiler/main.gem"
    GemVal _t1856[] = {gem_v_p};
    GemVal _t1857[] = {gem_v_t};
    GemVal _t1858 = gem_v_target_r;
    GemVal _t1859[] = {gem_table_get(_t1858, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1856, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1857, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1859, 1)), gem_string(";\n")));
    {
#line 1043 "compiler/main.gem"
        GemVal gem_v__for_i_39 = gem_int(0);
#line 1043 "compiler/main.gem"
    GemVal _t1860 = gem_v_node;
    GemVal _t1861[] = {gem_table_get(_t1860, gem_string("whens"))};
        GemVal gem_v__for_limit_39 = gem_len_fn(NULL, _t1861, 1);
#line 1043 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
#line 1043 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_39;
#line 1043 "compiler/main.gem"
            gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 1044 "compiler/main.gem"
    GemVal _t1862 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t1862, gem_string("whens")), gem_v_i);
#line 1045 "compiler/main.gem"
    GemVal _t1863 = gem_v_w;
    GemVal _t1864 = (*gem_v_compile_expr);
    GemVal _t1865[] = {gem_table_get(_t1863, gem_string("value"))};
            GemVal gem_v_val_r = _t1864.fn(_t1864.env, _t1865, 1);
#line 1046 "compiler/main.gem"
            GemVal gem_v_keyword = gem_string("if");
#line 1047 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1048 "compiler/main.gem"
                gem_v_keyword = gem_string("} else if");
            }
#line 1050 "compiler/main.gem"
    GemVal _t1866 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t1866, gem_string("setup")));
#line 1051 "compiler/main.gem"
    GemVal _t1867[] = {gem_v_p};
    GemVal _t1868[] = {gem_v_keyword};
    GemVal _t1869[] = {gem_v_t};
    GemVal _t1870 = gem_v_val_r;
    GemVal _t1871[] = {gem_table_get(_t1870, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1867, 1), gem_to_string_fn(NULL, _t1868, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t1869, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1871, 1)), gem_string("))) {\n")));
#line 1052 "compiler/main.gem"
    GemVal _t1872 = gem_v_w;
            GemVal gem_v_body = gem_table_get(_t1872, gem_string("body"));
#line 1053 "compiler/main.gem"
    GemVal _t1873[] = {gem_v_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1873, 1), gem_int(0)))) {
                {
#line 1054 "compiler/main.gem"
                    GemVal gem_v__for_i_38 = gem_int(0);
#line 1054 "compiler/main.gem"
    GemVal _t1874[] = {gem_v_body};
                    GemVal gem_v__for_limit_38 = gem_sub(gem_len_fn(NULL, _t1874, 1), gem_int(1));
#line 1054 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 1054 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_38;
#line 1054 "compiler/main.gem"
                        gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 1055 "compiler/main.gem"
    GemVal _t1875 = (*gem_v_compile_stmt);
    GemVal _t1876[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
                        gem_v_out = gem_add(gem_add(gem_v_out, _t1875.fn(_t1875.env, _t1876, 2)), gem_string("\n"));
                    }
                }
#line 1057 "compiler/main.gem"
    GemVal _t1877[] = {gem_v_body};
    GemVal _t1878 = (*gem_v_compile_stmt_return);
    GemVal _t1879[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t1877, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t1878.fn(_t1878.env, _t1879, 2)), gem_string("\n"));
            } else {
#line 1059 "compiler/main.gem"
    GemVal _t1880[] = {gem_v_p};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1880, 1), gem_string("    return GEM_NIL;\n")));
            }
        }
    }
#line 1062 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 1063 "compiler/main.gem"
    GemVal _t1881[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1881, 1), gem_string("} else {\n")));
#line 1064 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v_node, gem_string("else"));
#line 1065 "compiler/main.gem"
    GemVal _t1882[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1882, 1), gem_int(0)))) {
            {
#line 1066 "compiler/main.gem"
                GemVal gem_v__for_i_40 = gem_int(0);
#line 1066 "compiler/main.gem"
    GemVal _t1883[] = {gem_v_else_body};
                GemVal gem_v__for_limit_40 = gem_sub(gem_len_fn(NULL, _t1883, 1), gem_int(1));
#line 1066 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 1066 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_40;
#line 1066 "compiler/main.gem"
                    gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 1067 "compiler/main.gem"
    GemVal _t1884 = (*gem_v_compile_stmt);
    GemVal _t1885[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t1884.fn(_t1884.env, _t1885, 2)), gem_string("\n"));
                }
            }
#line 1069 "compiler/main.gem"
    GemVal _t1886[] = {gem_v_else_body};
    GemVal _t1887 = (*gem_v_compile_stmt_return);
    GemVal _t1888[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t1886, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1887.fn(_t1887.env, _t1888, 2)), gem_string("\n"));
        } else {
#line 1071 "compiler/main.gem"
    GemVal _t1889[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1889, 1), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1074 "compiler/main.gem"
    GemVal _t1890[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1890, 1), gem_string("} else {\n")));
#line 1075 "compiler/main.gem"
    GemVal _t1891[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1891, 1), gem_string("    return GEM_NIL;\n")));
    }
#line 1077 "compiler/main.gem"
    GemVal _t1892[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1892, 1), gem_string("}")));
    return gem_v_out;
}

struct _closure__anon_44 {
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
#line 1084 "compiler/main.gem"
    GemVal _t1894 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t1894, gem_string("name"));
#line 1085 "compiler/main.gem"
    GemVal _t1895 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1895, gem_string("params"));
#line 1086 "compiler/main.gem"
    GemVal _t1896 = gem_v_node;
    GemVal gem_v_ret_type = gem_table_get(_t1896, gem_string("ret_type"));
#line 1088 "compiler/main.gem"
    GemVal _t1897[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t1897, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1090 "compiler/main.gem"
    GemVal _t1898 = gem_table_new();
    GemVal gem_v_c_args = _t1898;
    {
#line 1091 "compiler/main.gem"
        GemVal gem_v__for_i_41 = gem_int(0);
#line 1091 "compiler/main.gem"
    GemVal _t1899[] = {gem_v_params};
        GemVal gem_v__for_limit_41 = gem_len_fn(NULL, _t1899, 1);
#line 1091 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_limit_41))) break;
#line 1091 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_41;
#line 1091 "compiler/main.gem"
            gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
#line 1092 "compiler/main.gem"
            GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1093 "compiler/main.gem"
            GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1094 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1095 "compiler/main.gem"
    GemVal _t1900[] = {gem_v_i};
    GemVal _t1901[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t1900, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1901, 1)), gem_string("].ival;\n")));
#line 1096 "compiler/main.gem"
    GemVal _t1902[] = {gem_v_i};
    GemVal _t1903[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1902, 1))};
                (void)(gem_push_fn(NULL, _t1903, 2));
            } else {
#line 1097 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1098 "compiler/main.gem"
    GemVal _t1904[] = {gem_v_i};
    GemVal _t1905[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t1904, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1905, 1)), gem_string("].fval;\n")));
#line 1099 "compiler/main.gem"
    GemVal _t1906[] = {gem_v_i};
    GemVal _t1907[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1906, 1))};
                    (void)(gem_push_fn(NULL, _t1907, 2));
                } else {
#line 1100 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1101 "compiler/main.gem"
    GemVal _t1908[] = {gem_v_i};
    GemVal _t1909[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t1908, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1909, 1)), gem_string("].sval;\n")));
#line 1102 "compiler/main.gem"
    GemVal _t1910[] = {gem_v_i};
    GemVal _t1911[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1910, 1))};
                        (void)(gem_push_fn(NULL, _t1911, 2));
                    } else {
#line 1103 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1104 "compiler/main.gem"
    GemVal _t1912[] = {gem_v_i};
    GemVal _t1913[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t1912, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1913, 1)), gem_string("].bval;\n")));
#line 1105 "compiler/main.gem"
    GemVal _t1914[] = {gem_v_i};
    GemVal _t1915[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1914, 1))};
                            (void)(gem_push_fn(NULL, _t1915, 2));
                        } else {
#line 1106 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1107 "compiler/main.gem"
    GemVal _t1916[] = {gem_v_i};
    GemVal _t1917[] = {gem_v_i};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t1916, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t1917, 1)), gem_string("].ival;\n")));
#line 1108 "compiler/main.gem"
    GemVal _t1918[] = {gem_v_i};
    GemVal _t1919[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1918, 1))};
                                (void)(gem_push_fn(NULL, _t1919, 2));
                            } else {
#line 1109 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1110 "compiler/main.gem"
    GemVal _t1920[] = {gem_v_i};
    GemVal _t1921[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t1920, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t1921, 2));
                                } else {
#line 1112 "compiler/main.gem"
    GemVal _t1922[] = {gem_v_i};
    GemVal _t1923[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t1922, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t1923, 2));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 1117 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
    {
#line 1118 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 1118 "compiler/main.gem"
    GemVal _t1924[] = {gem_v_c_args};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t1924, 1);
#line 1118 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 1118 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_42;
#line 1118 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 1119 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1120 "compiler/main.gem"
                gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
            }
#line 1122 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
        }
    }
#line 1125 "compiler/main.gem"
    GemVal _t1925[] = {gem_v_name};
    GemVal _t1926[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1925, 1), gem_string("(")), gem_to_string_fn(NULL, _t1926, 1)), gem_string(")"));
#line 1127 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1128 "compiler/main.gem"
    GemVal _t1927[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1927, 1)), gem_string(";\n")));
#line 1129 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
#line 1130 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1131 "compiler/main.gem"
    GemVal _t1928[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t1928, 1)), gem_string(";\n")));
        } else {
#line 1132 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1133 "compiler/main.gem"
    GemVal _t1929[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t1929, 1)), gem_string(";\n")));
#line 1134 "compiler/main.gem"
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
#line 1135 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1136 "compiler/main.gem"
    GemVal _t1930[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t1930, 1)), gem_string(";\n")));
#line 1137 "compiler/main.gem"
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
#line 1138 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1139 "compiler/main.gem"
    GemVal _t1931[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t1931, 1)), gem_string(";\n")));
#line 1140 "compiler/main.gem"
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
#line 1141 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1142 "compiler/main.gem"
    GemVal _t1932[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t1932, 1)), gem_string(";\n")));
#line 1143 "compiler/main.gem"
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
#line 1144 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1145 "compiler/main.gem"
    GemVal _t1933[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t1933, 1)), gem_string(";\n")));
#line 1146 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
#line 1148 "compiler/main.gem"
    GemVal _t1934[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1934, 1)), gem_string(";\n")));
#line 1149 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1152 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1155 "compiler/main.gem"
    GemVal _t1935[] = {gem_v_name};
    GemVal _t1936[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t1935, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t1936, 2));
    return gem_v_lines;
}

struct _closure__anon_45 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_mangle;
    GemVal *gem_v_source_name;
};
static GemVal _anon_45(void *_env, GemVal *args, int argc) {
    struct _closure__anon_45 *_cls = (struct _closure__anon_45 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
#line 1163 "compiler/main.gem"
    GemVal _t1938 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1938, gem_string("params"));
#line 1164 "compiler/main.gem"
    GemVal _t1939 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t1939, gem_string("body"));
#line 1165 "compiler/main.gem"
    GemVal _t1940 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t1940, gem_string("name"));
#line 1168 "compiler/main.gem"
    GemVal _t1941[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t1941, 1);
    {
#line 1169 "compiler/main.gem"
        GemVal gem_v__for_i_44 = gem_int(0);
#line 1169 "compiler/main.gem"
    GemVal _t1942[] = {gem_v_body};
        GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t1942, 1);
#line 1169 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 1169 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_44;
#line 1169 "compiler/main.gem"
            gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 1170 "compiler/main.gem"
    GemVal _t1943[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1945;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1943, 1), gem_string("table")))) {
        _t1945 = gem_eq(gem_type_fn(NULL, _t1943, 1), gem_string("table"));
    } else {
        GemVal _t1944 = gem_table_get(gem_v_body, gem_v_i);
        _t1945 = gem_eq(gem_table_get(_t1944, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1945)) {
#line 1171 "compiler/main.gem"
    GemVal _t1946 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1947[] = {gem_v_scope_vars, gem_table_get(_t1946, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1947, 2));
            }
#line 1173 "compiler/main.gem"
    GemVal _t1948[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1950;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1948, 1), gem_string("table")))) {
        _t1950 = gem_eq(gem_type_fn(NULL, _t1948, 1), gem_string("table"));
    } else {
        GemVal _t1949 = gem_table_get(gem_v_body, gem_v_i);
        _t1950 = gem_eq(gem_table_get(_t1949, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1950)) {
                {
#line 1174 "compiler/main.gem"
                    GemVal gem_v__for_i_43 = gem_int(0);
#line 1174 "compiler/main.gem"
    GemVal _t1951 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1952[] = {gem_table_get(_t1951, gem_string("stmts"))};
                    GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t1952, 1);
#line 1174 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 1174 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_43;
#line 1174 "compiler/main.gem"
                        gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 1175 "compiler/main.gem"
    GemVal _t1953 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1954[] = {gem_table_get(gem_table_get(_t1953, gem_string("stmts")), gem_v_j)};
    GemVal _t1957;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1954, 1), gem_string("table")))) {
        _t1957 = gem_eq(gem_type_fn(NULL, _t1954, 1), gem_string("table"));
    } else {
        GemVal _t1955 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t1956 = gem_table_get(gem_table_get(_t1955, gem_string("stmts")), gem_v_j);
        _t1957 = gem_eq(gem_table_get(_t1956, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1957)) {
#line 1176 "compiler/main.gem"
    GemVal _t1958 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1959 = gem_table_get(gem_table_get(_t1958, gem_string("stmts")), gem_v_j);
    GemVal _t1960[] = {gem_v_scope_vars, gem_table_get(_t1959, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1960, 2));
                        }
                    }
                }
            }
        }
    }
#line 1181 "compiler/main.gem"
    GemVal _t1961 = (*gem_v_find_captured_in_scope);
    GemVal _t1962[] = {gem_v_body, gem_v_scope_vars};
    GemVal gem_v_captured = _t1961.fn(_t1961.env, _t1962, 2);
#line 1183 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1184 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1186 "compiler/main.gem"
    GemVal _t1963[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t1963, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1189 "compiler/main.gem"
    GemVal _t1964 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1964, gem_string("line")), GEM_NIL))) {
#line 1190 "compiler/main.gem"
    GemVal _t1965 = gem_v_node;
    GemVal _t1966[] = {gem_table_get(_t1965, gem_string("line"))};
    GemVal _t1967[] = {(*gem_v_source_name)};
    GemVal _t1968[] = {gem_fn_escape_c_string(NULL, _t1967, 1)};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t1966, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t1968, 1)), gem_string("\"\n")));
    }
    {
#line 1194 "compiler/main.gem"
        GemVal gem_v__for_i_45 = gem_int(0);
#line 1194 "compiler/main.gem"
    GemVal _t1969[] = {gem_v_params};
        GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t1969, 1);
#line 1194 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 1194 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_45;
#line 1194 "compiler/main.gem"
            gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 1195 "compiler/main.gem"
    GemVal _t1970 = (*gem_v_mangle);
    GemVal _t1971[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t1970.fn(_t1970.env, _t1971, 1);
#line 1196 "compiler/main.gem"
    GemVal _t1972[] = {gem_v_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1972, 2))) {
#line 1197 "compiler/main.gem"
    GemVal _t1973[] = {gem_v_mp};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t1973, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1198 "compiler/main.gem"
    GemVal _t1974[] = {gem_v_mp};
    GemVal _t1975[] = {gem_v_i};
    GemVal _t1976[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t1974, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1975, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1976, 1)), gem_string("] : GEM_NIL;\n")));
            } else {
#line 1200 "compiler/main.gem"
    GemVal _t1977[] = {gem_v_mp};
    GemVal _t1978[] = {gem_v_i};
    GemVal _t1979[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1977, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1978, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1979, 1)), gem_string("] : GEM_NIL;\n")));
            }
        }
    }
#line 1205 "compiler/main.gem"
    GemVal _t1980[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1980, 1), gem_int(0)))) {
        {
#line 1206 "compiler/main.gem"
            GemVal gem_v__for_i_46 = gem_int(0);
#line 1206 "compiler/main.gem"
    GemVal _t1981[] = {gem_v_body};
            GemVal gem_v__for_limit_46 = gem_sub(gem_len_fn(NULL, _t1981, 1), gem_int(1));
#line 1206 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
#line 1206 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_46;
#line 1206 "compiler/main.gem"
                gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 1207 "compiler/main.gem"
    GemVal _t1982 = (*gem_v_compile_stmt);
    GemVal _t1983[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t1982.fn(_t1982.env, _t1983, 2)), gem_string("\n"));
            }
        }
#line 1209 "compiler/main.gem"
    GemVal _t1984[] = {gem_v_body};
    GemVal _t1985 = (*gem_v_compile_stmt_return);
    GemVal _t1986[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t1984, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t1985.fn(_t1985.env, _t1986, 2)), gem_string("\n"));
    } else {
#line 1211 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    }
#line 1214 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1215 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
    return gem_v_lines;
}

struct _closure__anon_46 {
    GemVal *gem_v_anon_name;
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_functions;
    GemVal *gem_v_mangle;
};
static GemVal _anon_46(void *_env, GemVal *args, int argc) {
    struct _closure__anon_46 *_cls = (struct _closure__anon_46 *)_env;
    GemVal *gem_v_anon_name = _cls->gem_v_anon_name;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_functions = _cls->gem_v_functions;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_captures = (argc > 1) ? args[1] : GEM_NIL;
#line 1222 "compiler/main.gem"
    GemVal _t1988 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1988, gem_string("params"));
#line 1223 "compiler/main.gem"
    GemVal _t1989 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t1989, gem_string("body"));
#line 1224 "compiler/main.gem"
    GemVal _t1990 = (*gem_v_anon_name);
    GemVal gem_v_name = _t1990.fn(_t1990.env, NULL, 0);
#line 1227 "compiler/main.gem"
    GemVal _t1991[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t1991, 1);
    {
#line 1228 "compiler/main.gem"
        GemVal gem_v__for_i_48 = gem_int(0);
#line 1228 "compiler/main.gem"
    GemVal _t1992[] = {gem_v_body};
        GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t1992, 1);
#line 1228 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 1228 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_48;
#line 1228 "compiler/main.gem"
            gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 1229 "compiler/main.gem"
    GemVal _t1993[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1995;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1993, 1), gem_string("table")))) {
        _t1995 = gem_eq(gem_type_fn(NULL, _t1993, 1), gem_string("table"));
    } else {
        GemVal _t1994 = gem_table_get(gem_v_body, gem_v_i);
        _t1995 = gem_eq(gem_table_get(_t1994, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1995)) {
#line 1230 "compiler/main.gem"
    GemVal _t1996 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1997[] = {gem_v_inner_scope, gem_table_get(_t1996, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1997, 2));
            }
#line 1232 "compiler/main.gem"
    GemVal _t1998[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2000;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1998, 1), gem_string("table")))) {
        _t2000 = gem_eq(gem_type_fn(NULL, _t1998, 1), gem_string("table"));
    } else {
        GemVal _t1999 = gem_table_get(gem_v_body, gem_v_i);
        _t2000 = gem_eq(gem_table_get(_t1999, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t2000)) {
                {
#line 1233 "compiler/main.gem"
                    GemVal gem_v__for_i_47 = gem_int(0);
#line 1233 "compiler/main.gem"
    GemVal _t2001 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2002[] = {gem_table_get(_t2001, gem_string("stmts"))};
                    GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2002, 1);
#line 1233 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 1233 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_47;
#line 1233 "compiler/main.gem"
                        gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 1234 "compiler/main.gem"
    GemVal _t2003 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2004[] = {gem_table_get(gem_table_get(_t2003, gem_string("stmts")), gem_v_j)};
    GemVal _t2007;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2004, 1), gem_string("table")))) {
        _t2007 = gem_eq(gem_type_fn(NULL, _t2004, 1), gem_string("table"));
    } else {
        GemVal _t2005 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t2006 = gem_table_get(gem_table_get(_t2005, gem_string("stmts")), gem_v_j);
        _t2007 = gem_eq(gem_table_get(_t2006, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t2007)) {
#line 1235 "compiler/main.gem"
    GemVal _t2008 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2009 = gem_table_get(gem_table_get(_t2008, gem_string("stmts")), gem_v_j);
    GemVal _t2010[] = {gem_v_inner_scope, gem_table_get(_t2009, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t2010, 2));
                        }
                    }
                }
            }
        }
    }
#line 1240 "compiler/main.gem"
    GemVal _t2011 = (*gem_v_find_captured_in_scope);
    GemVal _t2012[] = {gem_v_body, gem_v_inner_scope};
    GemVal gem_v_inner_captured = _t2011.fn(_t2011.env, _t2012, 2);
#line 1242 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1243 "compiler/main.gem"
    GemVal _t2013[] = {gem_v_captures};
    GemVal _t2014[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t2013, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t2014, 2);
#line 1246 "compiler/main.gem"
    GemVal _t2015[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t2015, 1));
#line 1247 "compiler/main.gem"
    GemVal _t2016[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t2016, 1)), gem_string(" {\n"));
    {
#line 1248 "compiler/main.gem"
        GemVal gem_v__for_items_49 = gem_v_captures;
#line 1248 "compiler/main.gem"
        GemVal gem_v__for_i_49 = gem_int(0);
#line 1248 "compiler/main.gem"
        while (1) {
            GemVal _t2017[] = {gem_v__for_items_49};
            if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_len_fn(NULL, _t2017, 1)))) break;
#line 1248 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_49, gem_v__for_i_49);
#line 1248 "compiler/main.gem"
            gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 1249 "compiler/main.gem"
    GemVal _t2018 = (*gem_v_mangle);
    GemVal _t2019[] = {gem_v_cap};
    GemVal _t2020[] = {_t2018.fn(_t2018.env, _t2019, 1)};
            gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2020, 1)), gem_string(";\n")));
        }
    }
#line 1251 "compiler/main.gem"
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
#line 1254 "compiler/main.gem"
    GemVal _t2021[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t2021, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1255 "compiler/main.gem"
    GemVal _t2022[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2022, 1), gem_int(0)))) {
#line 1256 "compiler/main.gem"
    GemVal _t2023[] = {gem_v_struct_name};
    GemVal _t2024[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2023, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t2024, 1)), gem_string(" *)_env;\n")));
        {
#line 1257 "compiler/main.gem"
            GemVal gem_v__for_items_50 = gem_v_captures;
#line 1257 "compiler/main.gem"
            GemVal gem_v__for_i_50 = gem_int(0);
#line 1257 "compiler/main.gem"
            while (1) {
                GemVal _t2025[] = {gem_v__for_items_50};
                if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_len_fn(NULL, _t2025, 1)))) break;
#line 1257 "compiler/main.gem"
                GemVal gem_v_cap = gem_table_get(gem_v__for_items_50, gem_v__for_i_50);
#line 1257 "compiler/main.gem"
                gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 1258 "compiler/main.gem"
    GemVal _t2026 = (*gem_v_mangle);
    GemVal _t2027[] = {gem_v_cap};
                GemVal gem_v_mc = _t2026.fn(_t2026.env, _t2027, 1);
#line 1259 "compiler/main.gem"
    GemVal _t2028[] = {gem_v_mc};
    GemVal _t2029[] = {gem_v_mc};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2028, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t2029, 1)), gem_string(";\n")));
            }
        }
    }
    {
#line 1264 "compiler/main.gem"
        GemVal gem_v__for_i_51 = gem_int(0);
#line 1264 "compiler/main.gem"
    GemVal _t2030[] = {gem_v_params};
        GemVal gem_v__for_limit_51 = gem_len_fn(NULL, _t2030, 1);
#line 1264 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_v__for_limit_51))) break;
#line 1264 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_51;
#line 1264 "compiler/main.gem"
            gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 1265 "compiler/main.gem"
    GemVal _t2031 = (*gem_v_mangle);
    GemVal _t2032[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t2031.fn(_t2031.env, _t2032, 1);
#line 1266 "compiler/main.gem"
    GemVal _t2033[] = {gem_v_inner_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2033, 2))) {
#line 1267 "compiler/main.gem"
    GemVal _t2034[] = {gem_v_mp};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2034, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1268 "compiler/main.gem"
    GemVal _t2035[] = {gem_v_mp};
    GemVal _t2036[] = {gem_v_i};
    GemVal _t2037[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2035, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2036, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2037, 1)), gem_string("] : GEM_NIL;\n")));
            } else {
#line 1270 "compiler/main.gem"
    GemVal _t2038[] = {gem_v_mp};
    GemVal _t2039[] = {gem_v_i};
    GemVal _t2040[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2038, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2039, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2040, 1)), gem_string("] : GEM_NIL;\n")));
            }
        }
    }
#line 1274 "compiler/main.gem"
    GemVal _t2041[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2041, 1), gem_int(0)))) {
        {
#line 1275 "compiler/main.gem"
            GemVal gem_v__for_i_52 = gem_int(0);
#line 1275 "compiler/main.gem"
    GemVal _t2042[] = {gem_v_body};
            GemVal gem_v__for_limit_52 = gem_sub(gem_len_fn(NULL, _t2042, 1), gem_int(1));
#line 1275 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 1275 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_52;
#line 1275 "compiler/main.gem"
                gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 1276 "compiler/main.gem"
    GemVal _t2043 = (*gem_v_compile_stmt);
    GemVal _t2044[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t2043.fn(_t2043.env, _t2044, 2)), gem_string("\n"));
            }
        }
#line 1278 "compiler/main.gem"
    GemVal _t2045[] = {gem_v_body};
    GemVal _t2046 = (*gem_v_compile_stmt_return);
    GemVal _t2047[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2045, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t2046.fn(_t2046.env, _t2047, 2)), gem_string("\n"));
    } else {
#line 1280 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    }
#line 1283 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1284 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1287 "compiler/main.gem"
    GemVal _t2048[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t2048, 2));
    GemVal _t2049 = gem_table_new();
    gem_table_set(_t2049, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t2049, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t2049, gem_string("captures"), gem_v_captures);
    return _t2049;
}

struct _closure__anon_47 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_extern_fn;
    GemVal *gem_v_compile_fn;
    GemVal *gem_v_compile_stmts;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_forward_decls;
    GemVal *gem_v_functions;
};
static GemVal _anon_47(void *_env, GemVal *args, int argc) {
    struct _closure__anon_47 *_cls = (struct _closure__anon_47 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_extern_fn = _cls->gem_v_compile_extern_fn;
    GemVal *gem_v_compile_fn = _cls->gem_v_compile_fn;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal *gem_v_functions = _cls->gem_v_functions;
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
#line 1296 "compiler/main.gem"
    GemVal _t2051 = gem_table_new();
    GemVal gem_v_fn_defs = _t2051;
#line 1297 "compiler/main.gem"
    GemVal _t2052 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t2052;
#line 1298 "compiler/main.gem"
    GemVal _t2053 = gem_table_new();
    GemVal gem_v_extern_includes = _t2053;
#line 1299 "compiler/main.gem"
    GemVal _t2054 = gem_table_new();
    GemVal gem_v_top_stmts = _t2054;
    {
#line 1301 "compiler/main.gem"
    GemVal _t2055 = gem_v_ast;
        GemVal gem_v__for_items_53 = gem_table_get(_t2055, gem_string("stmts"));
#line 1301 "compiler/main.gem"
        GemVal gem_v__for_i_53 = gem_int(0);
#line 1301 "compiler/main.gem"
        while (1) {
            GemVal _t2056[] = {gem_v__for_items_53};
            if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_len_fn(NULL, _t2056, 1)))) break;
#line 1301 "compiler/main.gem"
            GemVal gem_v_s = gem_table_get(gem_v__for_items_53, gem_v__for_i_53);
#line 1301 "compiler/main.gem"
            gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 1302 "compiler/main.gem"
    GemVal _t2057 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t2057, gem_string("tag")), gem_string("fn_def")))) {
#line 1303 "compiler/main.gem"
    GemVal _t2058[] = {gem_v_fn_defs, gem_v_s};
                (void)(gem_push_fn(NULL, _t2058, 2));
            } else {
#line 1304 "compiler/main.gem"
    GemVal _t2059 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t2059, gem_string("tag")), gem_string("extern_fn")))) {
#line 1305 "compiler/main.gem"
    GemVal _t2060[] = {gem_v_extern_fns_list, gem_v_s};
                    (void)(gem_push_fn(NULL, _t2060, 2));
                } else {
#line 1306 "compiler/main.gem"
    GemVal _t2061 = gem_v_s;
                    if (gem_truthy(gem_eq(gem_table_get(_t2061, gem_string("tag")), gem_string("extern_include")))) {
#line 1307 "compiler/main.gem"
    GemVal _t2062[] = {gem_v_extern_includes, gem_v_s};
                        (void)(gem_push_fn(NULL, _t2062, 2));
                    } else {
#line 1309 "compiler/main.gem"
    GemVal _t2063[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t2063, 2));
                    }
                }
            }
        }
    }
    {
#line 1314 "compiler/main.gem"
        GemVal gem_v__for_items_54 = gem_v_extern_fns_list;
#line 1314 "compiler/main.gem"
        GemVal gem_v__for_i_54 = gem_int(0);
#line 1314 "compiler/main.gem"
        while (1) {
            GemVal _t2064[] = {gem_v__for_items_54};
            if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_len_fn(NULL, _t2064, 1)))) break;
#line 1314 "compiler/main.gem"
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_54, gem_v__for_i_54);
#line 1314 "compiler/main.gem"
            gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 1315 "compiler/main.gem"
    GemVal _t2065 = gem_v_ef;
    GemVal _t2066[] = {(*gem_v_defined_fns), gem_table_get(_t2065, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2066, 2));
        }
    }
    {
#line 1319 "compiler/main.gem"
        GemVal gem_v__for_items_55 = gem_v_fn_defs;
#line 1319 "compiler/main.gem"
        GemVal gem_v__for_i_55 = gem_int(0);
#line 1319 "compiler/main.gem"
        while (1) {
            GemVal _t2067[] = {gem_v__for_items_55};
            if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_len_fn(NULL, _t2067, 1)))) break;
#line 1319 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_55, gem_v__for_i_55);
#line 1319 "compiler/main.gem"
            gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1320 "compiler/main.gem"
    GemVal _t2068 = gem_v_fd;
    GemVal _t2069[] = {(*gem_v_defined_fns), gem_table_get(_t2068, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2069, 2));
#line 1321 "compiler/main.gem"
    GemVal _t2070 = gem_v_fd;
    GemVal _t2071[] = {gem_table_get(_t2070, gem_string("name"))};
            GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2071, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1322 "compiler/main.gem"
    GemVal _t2072[] = {(*gem_v_forward_decls), gem_v_decl};
            (void)(gem_push_fn(NULL, _t2072, 2));
        }
    }
    {
#line 1326 "compiler/main.gem"
        GemVal gem_v__for_items_56 = gem_v_extern_fns_list;
#line 1326 "compiler/main.gem"
        GemVal gem_v__for_i_56 = gem_int(0);
#line 1326 "compiler/main.gem"
        while (1) {
            GemVal _t2073[] = {gem_v__for_items_56};
            if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_len_fn(NULL, _t2073, 1)))) break;
#line 1326 "compiler/main.gem"
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_56, gem_v__for_i_56);
#line 1326 "compiler/main.gem"
            gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1327 "compiler/main.gem"
    GemVal _t2074 = (*gem_v_compile_extern_fn);
    GemVal _t2075[] = {gem_v_ef};
    GemVal _t2076[] = {(*gem_v_functions), _t2074.fn(_t2074.env, _t2075, 1)};
            (void)(gem_push_fn(NULL, _t2076, 2));
        }
    }
    {
#line 1331 "compiler/main.gem"
        GemVal gem_v__for_items_57 = gem_v_fn_defs;
#line 1331 "compiler/main.gem"
        GemVal gem_v__for_i_57 = gem_int(0);
#line 1331 "compiler/main.gem"
        while (1) {
            GemVal _t2077[] = {gem_v__for_items_57};
            if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t2077, 1)))) break;
#line 1331 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
#line 1331 "compiler/main.gem"
            gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
#line 1332 "compiler/main.gem"
    GemVal _t2078 = (*gem_v_compile_fn);
    GemVal _t2079[] = {gem_v_fd};
    GemVal _t2080[] = {(*gem_v_functions), _t2078.fn(_t2078.env, _t2079, 1)};
            (void)(gem_push_fn(NULL, _t2080, 2));
        }
    }
#line 1336 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1337 "compiler/main.gem"
    GemVal _t2081 = gem_table_new();
    GemVal _t2082 = (*gem_v_find_captured_in_scope);
    GemVal _t2083[] = {gem_v_top_stmts, _t2081};
    *gem_v_boxed_vars = _t2082.fn(_t2082.env, _t2083, 2);
#line 1338 "compiler/main.gem"
    GemVal _t2084 = (*gem_v_compile_stmts);
    GemVal _t2085[] = {gem_v_top_stmts, gem_int(1)};
    GemVal gem_v_main_body = _t2084.fn(_t2084.env, _t2085, 2);
#line 1339 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1341 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
    {
#line 1342 "compiler/main.gem"
        GemVal gem_v__for_items_58 = gem_v_fn_defs;
#line 1342 "compiler/main.gem"
        GemVal gem_v__for_i_58 = gem_int(0);
#line 1342 "compiler/main.gem"
        while (1) {
            GemVal _t2086[] = {gem_v__for_items_58};
            if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t2086, 1)))) break;
#line 1342 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1342 "compiler/main.gem"
            gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1343 "compiler/main.gem"
    GemVal _t2087 = gem_v_fd;
            if (gem_truthy(gem_eq(gem_table_get(_t2087, gem_string("name")), gem_string("main")))) {
#line 1344 "compiler/main.gem"
                gem_v_has_main = gem_bool(1);
            }
        }
    }
#line 1348 "compiler/main.gem"
    GemVal gem_v_main_c = gem_string("int main(void) {\n");
#line 1349 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1350 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
#line 1352 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1353 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
#line 1354 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
#line 1356 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    return 0;\n}\n"));
#line 1360 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
    {
#line 1361 "compiler/main.gem"
        GemVal gem_v__for_items_59 = gem_v_extern_includes;
#line 1361 "compiler/main.gem"
        GemVal gem_v__for_i_59 = gem_int(0);
#line 1361 "compiler/main.gem"
        while (1) {
            GemVal _t2088[] = {gem_v__for_items_59};
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_len_fn(NULL, _t2088, 1)))) break;
#line 1361 "compiler/main.gem"
            GemVal gem_v_ei = gem_table_get(gem_v__for_items_59, gem_v__for_i_59);
#line 1361 "compiler/main.gem"
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1362 "compiler/main.gem"
    GemVal _t2089 = gem_v_ei;
    GemVal _t2090[] = {gem_table_get(_t2089, gem_string("path"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t2090, 1)), gem_string("\"\n")));
        }
    }
#line 1364 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
    {
#line 1367 "compiler/main.gem"
        GemVal gem_v__for_i_60 = gem_int(0);
#line 1367 "compiler/main.gem"
    GemVal _t2091[] = {(*gem_v_forward_decls)};
        GemVal gem_v__for_limit_60 = gem_len_fn(NULL, _t2091, 1);
#line 1367 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1367 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_60;
#line 1367 "compiler/main.gem"
            gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1368 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
#line 1369 "compiler/main.gem"
    GemVal _t2092[] = {(*gem_v_forward_decls)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2092, 1), gem_int(1))))) {
#line 1370 "compiler/main.gem"
                gem_v_out = gem_add(gem_v_out, gem_string("\n"));
            }
        }
    }
#line 1373 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
    {
#line 1376 "compiler/main.gem"
        GemVal gem_v__for_i_61 = gem_int(0);
#line 1376 "compiler/main.gem"
    GemVal _t2093[] = {(*gem_v_functions)};
        GemVal gem_v__for_limit_61 = gem_len_fn(NULL, _t2093, 1);
#line 1376 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_v__for_limit_61))) break;
#line 1376 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_61;
#line 1376 "compiler/main.gem"
            gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1377 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
#line 1378 "compiler/main.gem"
    GemVal _t2094[] = {(*gem_v_functions)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2094, 1), gem_int(1))))) {
#line 1379 "compiler/main.gem"
                gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
            }
        }
    }
#line 1382 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
#line 1384 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    return gem_v_out;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 135 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
#line 136 "compiler/main.gem"
    GemVal _t806 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t806;
#line 137 "compiler/main.gem"
    GemVal _t807 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t807;
#line 138 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 139 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 140 "compiler/main.gem"
    GemVal _t808 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t808;
#line 141 "compiler/main.gem"
    GemVal _t809 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t809;
#line 142 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("print"), gem_bool(1));
#line 143 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("error"), gem_bool(1));
#line 144 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("len"), gem_bool(1));
#line 145 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("type"), gem_bool(1));
#line 146 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("to_string"), gem_bool(1));
#line 147 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("push"), gem_bool(1));
#line 148 "compiler/main.gem"
    GemVal _t810 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t810;
#line 152 "compiler/main.gem"
    struct _closure__anon_22 *_t812 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t812->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_22, _t812);
#line 157 "compiler/main.gem"
    struct _closure__anon_23 *_t814 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t814->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_23, _t814);
#line 162 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_24, NULL);
#line 168 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 169 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 171 "compiler/main.gem"
    struct _closure__anon_25 *_t972 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t972->gem_v_builtins = gem_v_builtins;
    _t972->gem_v_collect_free_node = gem_v_collect_free_node;
    _t972->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t972->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_25, _t972);
#line 309 "compiler/main.gem"
    struct _closure__anon_26 *_t997 = GC_MALLOC(sizeof(struct _closure__anon_26));
    _t997->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_26, _t997);
#line 330 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 331 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 333 "compiler/main.gem"
    struct _closure__anon_27 *_t1069 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1069->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1069->gem_v_walk_captures = gem_v_walk_captures;
    _t1069->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_27, _t1069);
#line 413 "compiler/main.gem"
    struct _closure__anon_28 *_t1093 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1093->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_28, _t1093);
#line 430 "compiler/main.gem"
    struct _closure__anon_29 *_t1097 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t1097->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_29, _t1097);
#line 438 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 439 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 440 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 441 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 442 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 443 "compiler/main.gem"
    GemVal *gem_v_compile_if_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if_return = GEM_NIL;
#line 444 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 445 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 446 "compiler/main.gem"
    GemVal *gem_v_compile_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match_return = GEM_NIL;
#line 447 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 448 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 449 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 450 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 451 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 452 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 453 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 454 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 458 "compiler/main.gem"
    struct _closure__anon_30 *_t1201 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t1201->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1201->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t1201->gem_v_compile_array = gem_v_compile_array;
    _t1201->gem_v_compile_binop = gem_v_compile_binop;
    _t1201->gem_v_compile_call = gem_v_compile_call;
    _t1201->gem_v_compile_expr = gem_v_compile_expr;
    _t1201->gem_v_compile_table = gem_v_compile_table;
    _t1201->gem_v_defined_fns = gem_v_defined_fns;
    _t1201->gem_v_mangle = gem_v_mangle;
    _t1201->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_expr = gem_make_fn(_anon_30, _t1201);
#line 555 "compiler/main.gem"
    struct _closure__anon_31 *_t1217 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t1217->gem_v_compile_expr = gem_v_compile_expr;
    _t1217->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_31, _t1217);
#line 570 "compiler/main.gem"
    struct _closure__anon_32 *_t1230 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t1230->gem_v_compile_expr = gem_v_compile_expr;
    _t1230->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_32, _t1230);
#line 584 "compiler/main.gem"
    struct _closure__anon_33 *_t1266 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t1266->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1266->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1266->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t1266->gem_v_mangle = gem_v_mangle;
    _t1266->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_33, _t1266);
#line 612 "compiler/main.gem"
    struct _closure__anon_34 *_t1369 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t1369->gem_v_compile_expr = gem_v_compile_expr;
    _t1369->gem_v_defined_fns = gem_v_defined_fns;
    _t1369->gem_v_source_name = gem_v_source_name;
    _t1369->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_34, _t1369);
#line 732 "compiler/main.gem"
    struct _closure__anon_35 *_t1499 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t1499->gem_v_compile_expr = gem_v_compile_expr;
    _t1499->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_35, _t1499);
#line 807 "compiler/main.gem"
    struct _closure__anon_36 *_t1503 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t1503->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_36, _t1503);
#line 817 "compiler/main.gem"
    struct _closure__anon_37 *_t1640 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t1640->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1640->gem_v_compile_expr = gem_v_compile_expr;
    _t1640->gem_v_compile_if = gem_v_compile_if;
    _t1640->gem_v_compile_match = gem_v_compile_match;
    _t1640->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1640->gem_v_compile_while = gem_v_compile_while;
    _t1640->gem_v_mangle = gem_v_mangle;
    _t1640->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_stmt = gem_make_fn(_anon_37, _t1640);
#line 887 "compiler/main.gem"
    struct _closure__anon_38 *_t1758 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t1758->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1758->gem_v_compile_expr = gem_v_compile_expr;
    _t1758->gem_v_compile_if_return = gem_v_compile_if_return;
    _t1758->gem_v_compile_match_return = gem_v_compile_match_return;
    _t1758->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1758->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1758->gem_v_compile_while = gem_v_compile_while;
    _t1758->gem_v_mangle = gem_v_mangle;
    _t1758->gem_v_source_name = gem_v_source_name;
    _t1758->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_38, _t1758);
#line 944 "compiler/main.gem"
    struct _closure__anon_39 *_t1773 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t1773->gem_v_compile_expr = gem_v_compile_expr;
    _t1773->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_39, _t1773);
#line 960 "compiler/main.gem"
    struct _closure__anon_40 *_t1802 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t1802->gem_v_compile_expr = gem_v_compile_expr;
    _t1802->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1802->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_40, _t1802);
#line 995 "compiler/main.gem"
    struct _closure__anon_41 *_t1819 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t1819->gem_v_compile_expr = gem_v_compile_expr;
    _t1819->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_41, _t1819);
#line 1010 "compiler/main.gem"
    struct _closure__anon_42 *_t1849 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t1849->gem_v_compile_expr = gem_v_compile_expr;
    _t1849->gem_v_compile_stmts = gem_v_compile_stmts;
    _t1849->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_42, _t1849);
#line 1037 "compiler/main.gem"
    struct _closure__anon_43 *_t1893 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t1893->gem_v_compile_expr = gem_v_compile_expr;
    _t1893->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1893->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1893->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_43, _t1893);
#line 1083 "compiler/main.gem"
    struct _closure__anon_44 *_t1937 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t1937->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_44, _t1937);
#line 1162 "compiler/main.gem"
    struct _closure__anon_45 *_t1987 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t1987->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1987->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1987->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1987->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t1987->gem_v_mangle = gem_v_mangle;
    _t1987->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_fn = gem_make_fn(_anon_45, _t1987);
#line 1221 "compiler/main.gem"
    struct _closure__anon_46 *_t2050 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t2050->gem_v_anon_name = gem_v_anon_name;
    _t2050->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2050->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2050->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2050->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2050->gem_v_functions = gem_v_functions;
    _t2050->gem_v_mangle = gem_v_mangle;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_46, _t2050);
#line 1294 "compiler/main.gem"
    struct _closure__anon_47 *_t2095 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t2095->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2095->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t2095->gem_v_compile_fn = gem_v_compile_fn;
    _t2095->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2095->gem_v_defined_fns = gem_v_defined_fns;
    _t2095->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2095->gem_v_forward_decls = gem_v_forward_decls;
    _t2095->gem_v_functions = gem_v_functions;
    GemVal gem_v_compile = gem_make_fn(_anon_47, _t2095);
    GemVal _t2096 = gem_table_new();
    gem_table_set(_t2096, gem_string("compile"), gem_v_compile);
    return _t2096;
}

static GemVal gem_fn_split_string(void *_env, GemVal *args, int argc) {
#line 1393 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
#line 1394 "compiler/main.gem"
    GemVal _t2097 = gem_table_new();
    GemVal gem_v_result = _t2097;
#line 1395 "compiler/main.gem"
    GemVal gem_v_current = gem_string("");
    {
#line 1396 "compiler/main.gem"
        GemVal gem_v__for_i_62 = gem_int(0);
#line 1396 "compiler/main.gem"
    GemVal _t2098[] = {gem_v_s};
        GemVal gem_v__for_limit_62 = gem_len_fn(NULL, _t2098, 1);
#line 1396 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
#line 1396 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_62;
#line 1396 "compiler/main.gem"
            gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1397 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_v_delim))) {
#line 1398 "compiler/main.gem"
    GemVal _t2099[] = {gem_v_result, gem_v_current};
                (void)(gem_push_fn(NULL, _t2099, 2));
#line 1399 "compiler/main.gem"
                gem_v_current = gem_string("");
            } else {
#line 1401 "compiler/main.gem"
                gem_v_current = gem_add(gem_v_current, gem_table_get(gem_v_s, gem_v_i));
            }
        }
    }
#line 1404 "compiler/main.gem"
    GemVal _t2100[] = {gem_v_result, gem_v_current};
    (void)(gem_push_fn(NULL, _t2100, 2));
    return gem_v_result;
}

static GemVal gem_fn_str_replace_all(void *_env, GemVal *args, int argc) {
#line 1408 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
#line 1409 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 1410 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 1411 "compiler/main.gem"
    GemVal _t2101[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t2101, 1);
#line 1412 "compiler/main.gem"
    GemVal _t2102[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t2102, 1);
#line 1413 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
#line 1415 "compiler/main.gem"
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
#line 1416 "compiler/main.gem"
            GemVal gem_v_found = gem_bool(1);
#line 1417 "compiler/main.gem"
            GemVal gem_v_j = gem_int(0);
#line 1418 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
#line 1419 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
#line 1420 "compiler/main.gem"
                    gem_v_found = gem_bool(0);
                    break;
                }
#line 1423 "compiler/main.gem"
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
#line 1425 "compiler/main.gem"
            if (gem_truthy(gem_v_found)) {
#line 1426 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
#line 1427 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
#line 1431 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
#line 1432 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    return gem_v_result;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 19 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
#line 20 "compiler/main.gem"
    GemVal _t2103 = gem_table_new();
    GemVal gem_v_new_stmts = _t2103;
#line 21 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
    {
#line 22 "compiler/main.gem"
    GemVal _t2104 = gem_v_ast;
        GemVal gem_v__for_items_1 = gem_table_get(_t2104, gem_string("stmts"));
#line 22 "compiler/main.gem"
        GemVal gem_v__for_i_1 = gem_int(0);
#line 22 "compiler/main.gem"
        while (1) {
            GemVal _t2105[] = {gem_v__for_items_1};
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t2105, 1)))) break;
#line 22 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 22 "compiler/main.gem"
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 23 "compiler/main.gem"
    GemVal _t2106[] = {gem_v_stmt};
    GemVal _t2108;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2106, 1), gem_string("table")))) {
        _t2108 = gem_eq(gem_type_fn(NULL, _t2106, 1), gem_string("table"));
    } else {
        GemVal _t2107 = gem_v_stmt;
        _t2108 = gem_eq(gem_table_get(_t2107, gem_string("tag")), gem_string("load"));
    }
            if (gem_truthy(_t2108)) {
#line 24 "compiler/main.gem"
    GemVal _t2109 = gem_v_stmt;
                GemVal gem_v_path = gem_table_get(_t2109, gem_string("path"));
#line 26 "compiler/main.gem"
    GemVal _t2110[] = {gem_v_path};
    GemVal _t2118;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t2110, 1), gem_int(4)))) {
        _t2118 = gem_lt(gem_len_fn(NULL, _t2110, 1), gem_int(4));
    } else {
        GemVal _t2111[] = {gem_v_path};
        GemVal _t2113;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2111, 1), gem_int(4))), gem_string(".")))) {
                _t2113 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2111, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t2112[] = {gem_v_path};
                _t2113 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2112, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t2115;
        if (gem_truthy(_t2113)) {
                _t2115 = _t2113;
        } else {
                GemVal _t2114[] = {gem_v_path};
                _t2115 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2114, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t2117;
        if (gem_truthy(_t2115)) {
                _t2117 = _t2115;
        } else {
                GemVal _t2116[] = {gem_v_path};
                _t2117 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2116, 1), gem_int(1))), gem_string("m"));
        }
        _t2118 = _t2117;
    }
                if (gem_truthy(_t2118)) {
#line 27 "compiler/main.gem"
                    gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
                }
#line 29 "compiler/main.gem"
    GemVal _t2119[] = {gem_v_base_dir, gem_v_path};
    GemVal _t2120[] = {gem_fn_gem_path_join(NULL, _t2119, 2)};
                GemVal gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t2120, 1);
#line 32 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                    continue;
                }
#line 35 "compiler/main.gem"
                gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 37 "compiler/main.gem"
    GemVal _t2121[] = {gem_v_full_path};
                GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2121, 1);
#line 38 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 39 "compiler/main.gem"
    GemVal _t2122[] = {gem_v_full_path};
    GemVal _t2123[] = {gem_add(gem_add(gem_string("cannot load '"), gem_to_string_fn(NULL, _t2122, 1)), gem_string("'"))};
                    (void)(gem_error_at_fn("compiler/main.gem", 39, _t2123, 1));
                }
#line 42 "compiler/main.gem"
    GemVal _t2124[] = {gem_v_source};
                GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t2124, 1);
#line 43 "compiler/main.gem"
    GemVal _t2125[] = {gem_v_full_path};
                GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t2125, 1);
#line 44 "compiler/main.gem"
    GemVal _t2126[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
                GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t2126, 3);
                {
#line 46 "compiler/main.gem"
    GemVal _t2127 = gem_v_resolved;
                    GemVal gem_v__for_items_2 = gem_table_get(_t2127, gem_string("stmts"));
#line 46 "compiler/main.gem"
                    GemVal gem_v__for_i_2 = gem_int(0);
#line 46 "compiler/main.gem"
                    while (1) {
                        GemVal _t2128[] = {gem_v__for_items_2};
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t2128, 1)))) break;
#line 46 "compiler/main.gem"
                        GemVal gem_v_rs = gem_table_get(gem_v__for_items_2, gem_v__for_i_2);
#line 46 "compiler/main.gem"
                        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 47 "compiler/main.gem"
                        gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_rs);
#line 48 "compiler/main.gem"
                        gem_v_count = gem_add(gem_v_count, gem_int(1));
                    }
                }
            } else {
#line 51 "compiler/main.gem"
                gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_stmt);
#line 52 "compiler/main.gem"
                gem_v_count = gem_add(gem_v_count, gem_int(1));
            }
        }
    }
#line 55 "compiler/main.gem"
    GemVal _t2129[] = {gem_v_new_stmts};
    return gem_fn_make_program(NULL, _t2129, 1);
}

int main(void) {
#line 60 "compiler/main.gem"
    GemVal gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
#line 61 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 62 "compiler/main.gem"
    GemVal _t2130[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 62, _t2130, 1));
    }
#line 65 "compiler/main.gem"
    GemVal _t2131[] = {gem_int(1)};
    GemVal gem_v_src_path = gem_fn_gem_get_argv(NULL, _t2131, 1);
#line 66 "compiler/main.gem"
    GemVal _t2132[] = {gem_v_src_path};
    GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2132, 1);
#line 67 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 68 "compiler/main.gem"
    GemVal _t2133[] = {gem_v_src_path};
    GemVal _t2134[] = {gem_add(gem_add(gem_string("cannot read '"), gem_to_string_fn(NULL, _t2133, 1)), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 68, _t2134, 1));
    }
#line 71 "compiler/main.gem"
    GemVal _t2135[] = {gem_v_source};
    GemVal gem_v_ast = gem_fn_parse_source(NULL, _t2135, 1);
#line 72 "compiler/main.gem"
    GemVal _t2136[] = {gem_v_src_path};
    GemVal gem_v_base_dir = gem_fn_gem_dirname(NULL, _t2136, 1);
#line 73 "compiler/main.gem"
    GemVal _t2137 = gem_table_new();
    GemVal gem_v_loaded = _t2137;
#line 74 "compiler/main.gem"
    GemVal _t2138[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    GemVal gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t2138, 3);
#line 76 "compiler/main.gem"
    GemVal _t2139[] = {gem_v_src_path};
    GemVal gem_v_cg = gem_fn_make_codegen(NULL, _t2139, 1);
#line 77 "compiler/main.gem"
    GemVal _t2140 = gem_v_cg;
    GemVal _t2141 = gem_table_get(_t2140, gem_string("compile"));
    GemVal _t2142[] = {gem_v_resolved_ast};
    GemVal gem_v_c_code = _t2141.fn(_t2141.env, _t2142, 1);
#line 78 "compiler/main.gem"
    GemVal _t2143[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t2143, 1));
    return 0;
}

