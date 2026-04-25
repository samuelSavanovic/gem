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
static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc);
static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc);
static GemVal gem_fn_atof(void *_env, GemVal *args, int argc);
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
    gem_push_frame("make_program", "compiler/main.gem", 8);
    GemVal _t1 = gem_table_new();
    gem_table_set(_t1, gem_string("tag"), gem_string("program"));
    gem_table_set(_t1, gem_string("stmts"), gem_v_stmts);
    GemVal _t2 = _t1;
    gem_pop_frame();
    return _t2;
}

static GemVal gem_fn_make_int(void *_env, GemVal *args, int argc) {
#line 14 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_int", "compiler/main.gem", 14);
    GemVal _t3 = gem_table_new();
    gem_table_set(_t3, gem_string("tag"), gem_string("int"));
    gem_table_set(_t3, gem_string("value"), gem_v_value);
    GemVal _t4 = _t3;
    gem_pop_frame();
    return _t4;
}

static GemVal gem_fn_make_float(void *_env, GemVal *args, int argc) {
#line 18 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_float", "compiler/main.gem", 18);
    GemVal _t5 = gem_table_new();
    gem_table_set(_t5, gem_string("tag"), gem_string("float"));
    gem_table_set(_t5, gem_string("value"), gem_v_value);
    GemVal _t6 = _t5;
    gem_pop_frame();
    return _t6;
}

static GemVal gem_fn_make_string(void *_env, GemVal *args, int argc) {
#line 22 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_string", "compiler/main.gem", 22);
    GemVal _t7 = gem_table_new();
    gem_table_set(_t7, gem_string("tag"), gem_string("string"));
    gem_table_set(_t7, gem_string("value"), gem_v_value);
    GemVal _t8 = _t7;
    gem_pop_frame();
    return _t8;
}

static GemVal gem_fn_make_bool(void *_env, GemVal *args, int argc) {
#line 26 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_bool", "compiler/main.gem", 26);
    GemVal _t9 = gem_table_new();
    gem_table_set(_t9, gem_string("tag"), gem_string("bool"));
    gem_table_set(_t9, gem_string("value"), gem_v_value);
    GemVal _t10 = _t9;
    gem_pop_frame();
    return _t10;
}

static GemVal gem_fn_make_nil_node(void *_env, GemVal *args, int argc) {
#line 30 "compiler/main.gem"
    gem_push_frame("make_nil_node", "compiler/main.gem", 30);
    GemVal _t11 = gem_table_new();
    gem_table_set(_t11, gem_string("tag"), gem_string("nil"));
    GemVal _t12 = _t11;
    gem_pop_frame();
    return _t12;
}

static GemVal gem_fn_make_var(void *_env, GemVal *args, int argc) {
#line 34 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_var", "compiler/main.gem", 34);
    GemVal _t13 = gem_table_new();
    gem_table_set(_t13, gem_string("tag"), gem_string("var"));
    gem_table_set(_t13, gem_string("name"), gem_v_name);
    GemVal _t14 = _t13;
    gem_pop_frame();
    return _t14;
}

static GemVal gem_fn_make_binop(void *_env, GemVal *args, int argc) {
#line 38 "compiler/main.gem"
    GemVal gem_v_op = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_left = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_right = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_binop", "compiler/main.gem", 38);
    GemVal _t15 = gem_table_new();
    gem_table_set(_t15, gem_string("tag"), gem_string("binop"));
    gem_table_set(_t15, gem_string("op"), gem_v_op);
    gem_table_set(_t15, gem_string("left"), gem_v_left);
    gem_table_set(_t15, gem_string("right"), gem_v_right);
    GemVal _t16 = _t15;
    gem_pop_frame();
    return _t16;
}

static GemVal gem_fn_make_unop(void *_env, GemVal *args, int argc) {
#line 42 "compiler/main.gem"
    GemVal gem_v_op = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_expr = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_unop", "compiler/main.gem", 42);
    GemVal _t17 = gem_table_new();
    gem_table_set(_t17, gem_string("tag"), gem_string("unop"));
    gem_table_set(_t17, gem_string("op"), gem_v_op);
    gem_table_set(_t17, gem_string("expr"), gem_v_expr);
    GemVal _t18 = _t17;
    gem_pop_frame();
    return _t18;
}

static GemVal gem_fn_make_call(void *_env, GemVal *args, int argc) {
#line 46 "compiler/main.gem"
    GemVal gem_v_func = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_args = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_call", "compiler/main.gem", 46);
    GemVal _t19 = gem_table_new();
    gem_table_set(_t19, gem_string("tag"), gem_string("call"));
    gem_table_set(_t19, gem_string("func"), gem_v_func);
    gem_table_set(_t19, gem_string("args"), gem_v_args);
    gem_table_set(_t19, gem_string("line"), gem_v_line);
    GemVal _t20 = _t19;
    gem_pop_frame();
    return _t20;
}

static GemVal gem_fn_make_dot(void *_env, GemVal *args, int argc) {
#line 50 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_dot", "compiler/main.gem", 50);
    GemVal _t21 = gem_table_new();
    gem_table_set(_t21, gem_string("tag"), gem_string("dot"));
    gem_table_set(_t21, gem_string("object"), gem_v_object);
    gem_table_set(_t21, gem_string("field"), gem_v_field);
    GemVal _t22 = _t21;
    gem_pop_frame();
    return _t22;
}

static GemVal gem_fn_make_index(void *_env, GemVal *args, int argc) {
#line 54 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_index", "compiler/main.gem", 54);
    GemVal _t23 = gem_table_new();
    gem_table_set(_t23, gem_string("tag"), gem_string("index"));
    gem_table_set(_t23, gem_string("object"), gem_v_object);
    gem_table_set(_t23, gem_string("key"), gem_v_key);
    GemVal _t24 = _t23;
    gem_pop_frame();
    return _t24;
}

static GemVal gem_fn_make_anon_fn(void *_env, GemVal *args, int argc) {
#line 58 "compiler/main.gem"
    GemVal gem_v_params = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_anon_fn", "compiler/main.gem", 58);
    GemVal _t25 = gem_table_new();
    gem_table_set(_t25, gem_string("tag"), gem_string("anon_fn"));
    gem_table_set(_t25, gem_string("params"), gem_v_params);
    gem_table_set(_t25, gem_string("body"), gem_v_body);
    GemVal _t26 = _t25;
    gem_pop_frame();
    return _t26;
}

static GemVal gem_fn_make_table(void *_env, GemVal *args, int argc) {
#line 62 "compiler/main.gem"
    GemVal gem_v_entries = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_table", "compiler/main.gem", 62);
    GemVal _t27 = gem_table_new();
    gem_table_set(_t27, gem_string("tag"), gem_string("table"));
    gem_table_set(_t27, gem_string("entries"), gem_v_entries);
    GemVal _t28 = _t27;
    gem_pop_frame();
    return _t28;
}

static GemVal gem_fn_make_table_entry(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_key = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_table_entry", "compiler/main.gem", 66);
    GemVal _t29 = gem_table_new();
    gem_table_set(_t29, gem_string("key"), gem_v_key);
    gem_table_set(_t29, gem_string("value"), gem_v_value);
    GemVal _t30 = _t29;
    gem_pop_frame();
    return _t30;
}

static GemVal gem_fn_make_array(void *_env, GemVal *args, int argc) {
#line 70 "compiler/main.gem"
    GemVal gem_v_elements = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_array", "compiler/main.gem", 70);
    GemVal _t31 = gem_table_new();
    gem_table_set(_t31, gem_string("tag"), gem_string("array"));
    gem_table_set(_t31, gem_string("elements"), gem_v_elements);
    GemVal _t32 = _t31;
    gem_pop_frame();
    return _t32;
}

static GemVal gem_fn_make_interp(void *_env, GemVal *args, int argc) {
#line 74 "compiler/main.gem"
    GemVal gem_v_parts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_interp", "compiler/main.gem", 74);
    GemVal _t33 = gem_table_new();
    gem_table_set(_t33, gem_string("tag"), gem_string("interp"));
    gem_table_set(_t33, gem_string("parts"), gem_v_parts);
    GemVal _t34 = _t33;
    gem_pop_frame();
    return _t34;
}

static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc) {
#line 80 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_let", "compiler/main.gem", 80);
    GemVal _t35 = gem_table_new();
    gem_table_set(_t35, gem_string("tag"), gem_string("let"));
    gem_table_set(_t35, gem_string("name"), gem_v_name);
    gem_table_set(_t35, gem_string("value"), gem_v_value);
    gem_table_set(_t35, gem_string("line"), gem_v_line);
    GemVal _t36 = _t35;
    gem_pop_frame();
    return _t36;
}

static GemVal gem_fn_make_assign(void *_env, GemVal *args, int argc) {
#line 84 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_assign", "compiler/main.gem", 84);
    GemVal _t37 = gem_table_new();
    gem_table_set(_t37, gem_string("tag"), gem_string("assign"));
    gem_table_set(_t37, gem_string("name"), gem_v_name);
    gem_table_set(_t37, gem_string("value"), gem_v_value);
    gem_table_set(_t37, gem_string("line"), gem_v_line);
    GemVal _t38 = _t37;
    gem_pop_frame();
    return _t38;
}

static GemVal gem_fn_make_dot_assign(void *_env, GemVal *args, int argc) {
#line 88 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_dot_assign", "compiler/main.gem", 88);
    GemVal _t39 = gem_table_new();
    gem_table_set(_t39, gem_string("tag"), gem_string("dot_assign"));
    gem_table_set(_t39, gem_string("object"), gem_v_object);
    gem_table_set(_t39, gem_string("field"), gem_v_field);
    gem_table_set(_t39, gem_string("value"), gem_v_value);
    gem_table_set(_t39, gem_string("line"), gem_v_line);
    GemVal _t40 = _t39;
    gem_pop_frame();
    return _t40;
}

static GemVal gem_fn_make_index_assign(void *_env, GemVal *args, int argc) {
#line 92 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_index_assign", "compiler/main.gem", 92);
    GemVal _t41 = gem_table_new();
    gem_table_set(_t41, gem_string("tag"), gem_string("index_assign"));
    gem_table_set(_t41, gem_string("object"), gem_v_object);
    gem_table_set(_t41, gem_string("key"), gem_v_key);
    gem_table_set(_t41, gem_string("value"), gem_v_value);
    gem_table_set(_t41, gem_string("line"), gem_v_line);
    GemVal _t42 = _t41;
    gem_pop_frame();
    return _t42;
}

static GemVal gem_fn_make_fn_def(void *_env, GemVal *args, int argc) {
#line 96 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_fn_def", "compiler/main.gem", 96);
    GemVal _t43 = gem_table_new();
    gem_table_set(_t43, gem_string("tag"), gem_string("fn_def"));
    gem_table_set(_t43, gem_string("name"), gem_v_name);
    gem_table_set(_t43, gem_string("params"), gem_v_params);
    gem_table_set(_t43, gem_string("body"), gem_v_body);
    gem_table_set(_t43, gem_string("line"), gem_v_line);
    GemVal _t44 = _t43;
    gem_pop_frame();
    return _t44;
}

static GemVal gem_fn_make_if(void *_env, GemVal *args, int argc) {
#line 100 "compiler/main.gem"
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_then_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_if", "compiler/main.gem", 100);
#line 101 "compiler/main.gem"
    GemVal _t45 = gem_table_new();
    gem_table_set(_t45, gem_string("tag"), gem_string("if"));
    gem_table_set(_t45, gem_string("cond"), gem_v_cond);
    gem_table_set(_t45, gem_string("then"), gem_v_then_body);
    gem_table_set(_t45, gem_string("line"), gem_v_line);
    GemVal gem_v_node = _t45;
#line 102 "compiler/main.gem"
    gem_table_set(gem_v_node, gem_string("else"), gem_v_else_body);
    GemVal _t46 = gem_v_node;
    gem_pop_frame();
    return _t46;
}

static GemVal gem_fn_make_while(void *_env, GemVal *args, int argc) {
#line 106 "compiler/main.gem"
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_while", "compiler/main.gem", 106);
    GemVal _t47 = gem_table_new();
    gem_table_set(_t47, gem_string("tag"), gem_string("while"));
    gem_table_set(_t47, gem_string("cond"), gem_v_cond);
    gem_table_set(_t47, gem_string("body"), gem_v_body);
    gem_table_set(_t47, gem_string("line"), gem_v_line);
    GemVal _t48 = _t47;
    gem_pop_frame();
    return _t48;
}

static GemVal gem_fn_make_match(void *_env, GemVal *args, int argc) {
#line 110 "compiler/main.gem"
    GemVal gem_v_target = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_whens = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_match", "compiler/main.gem", 110);
#line 111 "compiler/main.gem"
    GemVal _t49 = gem_table_new();
    gem_table_set(_t49, gem_string("tag"), gem_string("match"));
    gem_table_set(_t49, gem_string("target"), gem_v_target);
    gem_table_set(_t49, gem_string("whens"), gem_v_whens);
    gem_table_set(_t49, gem_string("line"), gem_v_line);
    GemVal gem_v_node = _t49;
#line 112 "compiler/main.gem"
    gem_table_set(gem_v_node, gem_string("else"), gem_v_else_body);
    GemVal _t50 = gem_v_node;
    gem_pop_frame();
    return _t50;
}

static GemVal gem_fn_make_when(void *_env, GemVal *args, int argc) {
#line 116 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_when", "compiler/main.gem", 116);
    GemVal _t51 = gem_table_new();
    gem_table_set(_t51, gem_string("tag"), gem_string("when_clause"));
    gem_table_set(_t51, gem_string("value"), gem_v_value);
    gem_table_set(_t51, gem_string("body"), gem_v_body);
    GemVal _t52 = _t51;
    gem_pop_frame();
    return _t52;
}

static GemVal gem_fn_make_return(void *_env, GemVal *args, int argc) {
#line 120 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_line = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_return", "compiler/main.gem", 120);
    GemVal _t53 = gem_table_new();
    gem_table_set(_t53, gem_string("tag"), gem_string("return"));
    gem_table_set(_t53, gem_string("value"), gem_v_value);
    gem_table_set(_t53, gem_string("line"), gem_v_line);
    GemVal _t54 = _t53;
    gem_pop_frame();
    return _t54;
}

static GemVal gem_fn_make_break(void *_env, GemVal *args, int argc) {
#line 124 "compiler/main.gem"
    gem_push_frame("make_break", "compiler/main.gem", 124);
    GemVal _t55 = gem_table_new();
    gem_table_set(_t55, gem_string("tag"), gem_string("break"));
    GemVal _t56 = _t55;
    gem_pop_frame();
    return _t56;
}

static GemVal gem_fn_make_continue(void *_env, GemVal *args, int argc) {
#line 128 "compiler/main.gem"
    gem_push_frame("make_continue", "compiler/main.gem", 128);
    GemVal _t57 = gem_table_new();
    gem_table_set(_t57, gem_string("tag"), gem_string("continue"));
    GemVal _t58 = _t57;
    gem_pop_frame();
    return _t58;
}

static GemVal gem_fn_make_load(void *_env, GemVal *args, int argc) {
#line 132 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_load", "compiler/main.gem", 132);
    GemVal _t59 = gem_table_new();
    gem_table_set(_t59, gem_string("tag"), gem_string("load"));
    gem_table_set(_t59, gem_string("path"), gem_v_path);
    GemVal _t60 = _t59;
    gem_pop_frame();
    return _t60;
}

static GemVal gem_fn_make_extern_fn(void *_env, GemVal *args, int argc) {
#line 136 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_extern_fn", "compiler/main.gem", 136);
    GemVal _t61 = gem_table_new();
    gem_table_set(_t61, gem_string("tag"), gem_string("extern_fn"));
    gem_table_set(_t61, gem_string("name"), gem_v_name);
    gem_table_set(_t61, gem_string("params"), gem_v_params);
    gem_table_set(_t61, gem_string("ret_type"), gem_v_ret_type);
    GemVal _t62 = _t61;
    gem_pop_frame();
    return _t62;
}

static GemVal gem_fn_make_extern_param(void *_env, GemVal *args, int argc) {
#line 140 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_type_name = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_extern_param", "compiler/main.gem", 140);
    GemVal _t63 = gem_table_new();
    gem_table_set(_t63, gem_string("name"), gem_v_name);
    gem_table_set(_t63, gem_string("type"), gem_v_type_name);
    GemVal _t64 = _t63;
    gem_pop_frame();
    return _t64;
}

static GemVal gem_fn_make_extern_include(void *_env, GemVal *args, int argc) {
#line 144 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_extern_include", "compiler/main.gem", 144);
    GemVal _t65 = gem_table_new();
    gem_table_set(_t65, gem_string("tag"), gem_string("extern_include"));
    gem_table_set(_t65, gem_string("path"), gem_v_path);
    GemVal _t66 = _t65;
    gem_pop_frame();
    return _t66;
}

static GemVal gem_fn_is_digit(void *_env, GemVal *args, int argc) {
#line 13 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_digit", "compiler/main.gem", 13);
    GemVal _t67;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("0")))) {
        _t67 = gem_ge(gem_v_ch, gem_string("0"));
    } else {
        _t67 = gem_le(gem_v_ch, gem_string("9"));
    }
    GemVal _t68 = _t67;
    gem_pop_frame();
    return _t68;
}

static GemVal gem_fn_is_alpha(void *_env, GemVal *args, int argc) {
#line 17 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_alpha", "compiler/main.gem", 17);
    GemVal _t69;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("a")))) {
        _t69 = gem_ge(gem_v_ch, gem_string("a"));
    } else {
        _t69 = gem_le(gem_v_ch, gem_string("z"));
    }
    GemVal _t71;
    if (gem_truthy(_t69)) {
        _t71 = _t69;
    } else {
        GemVal _t70;
        if (!gem_truthy(gem_ge(gem_v_ch, gem_string("A")))) {
                _t70 = gem_ge(gem_v_ch, gem_string("A"));
        } else {
                _t70 = gem_le(gem_v_ch, gem_string("Z"));
        }
        _t71 = _t70;
    }
    GemVal _t72;
    if (gem_truthy(_t71)) {
        _t72 = _t71;
    } else {
        _t72 = gem_eq(gem_v_ch, gem_string("_"));
    }
    GemVal _t73 = _t72;
    gem_pop_frame();
    return _t73;
}

static GemVal gem_fn_is_alnum(void *_env, GemVal *args, int argc) {
#line 21 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_alnum", "compiler/main.gem", 21);
    GemVal _t74[] = {gem_v_ch};
    GemVal _t76;
    if (gem_truthy(gem_fn_is_alpha(NULL, _t74, 1))) {
        _t76 = gem_fn_is_alpha(NULL, _t74, 1);
    } else {
        GemVal _t75[] = {gem_v_ch};
        _t76 = gem_fn_is_digit(NULL, _t75, 1);
    }
    GemVal _t77 = _t76;
    gem_pop_frame();
    return _t77;
}

static GemVal gem_fn_digit_val(void *_env, GemVal *args, int argc) {
#line 27 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("digit_val", "compiler/main.gem", 27);
#line 28 "compiler/main.gem"
    GemVal _t78 = gem_v_ch;
    if (gem_truthy(gem_eq(_t78, gem_string("0")))) {
        GemVal _t79 = gem_int(0);
        gem_pop_frame();
        return _t79;
    } else if (gem_truthy(gem_eq(_t78, gem_string("1")))) {
        GemVal _t80 = gem_int(1);
        gem_pop_frame();
        return _t80;
    } else if (gem_truthy(gem_eq(_t78, gem_string("2")))) {
        GemVal _t81 = gem_int(2);
        gem_pop_frame();
        return _t81;
    } else if (gem_truthy(gem_eq(_t78, gem_string("3")))) {
        GemVal _t82 = gem_int(3);
        gem_pop_frame();
        return _t82;
    } else if (gem_truthy(gem_eq(_t78, gem_string("4")))) {
        GemVal _t83 = gem_int(4);
        gem_pop_frame();
        return _t83;
    } else if (gem_truthy(gem_eq(_t78, gem_string("5")))) {
        GemVal _t84 = gem_int(5);
        gem_pop_frame();
        return _t84;
    } else if (gem_truthy(gem_eq(_t78, gem_string("6")))) {
        GemVal _t85 = gem_int(6);
        gem_pop_frame();
        return _t85;
    } else if (gem_truthy(gem_eq(_t78, gem_string("7")))) {
        GemVal _t86 = gem_int(7);
        gem_pop_frame();
        return _t86;
    } else if (gem_truthy(gem_eq(_t78, gem_string("8")))) {
        GemVal _t87 = gem_int(8);
        gem_pop_frame();
        return _t87;
    } else if (gem_truthy(gem_eq(_t78, gem_string("9")))) {
        GemVal _t88 = gem_int(9);
        gem_pop_frame();
        return _t88;
    } else {
#line 50 "compiler/main.gem"
    GemVal _t89[] = {gem_v_ch};
    GemVal _t90[] = {gem_add(gem_string("not a digit: "), gem_to_string_fn(NULL, _t89, 1))};
        GemVal _t91 = gem_error_at_fn("compiler/main.gem", 50, _t90, 1);
        gem_pop_frame();
        return _t91;
    }
}

static GemVal gem_fn_str_to_int(void *_env, GemVal *args, int argc) {
#line 54 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("str_to_int", "compiler/main.gem", 54);
#line 55 "compiler/main.gem"
    GemVal gem_v_result = gem_int(0);
    {
#line 56 "compiler/main.gem"
        GemVal gem_v__for_i_1 = gem_int(0);
#line 56 "compiler/main.gem"
    GemVal _t92[] = {gem_v_s};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t92, 1);
#line 56 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 56 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_1;
#line 56 "compiler/main.gem"
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 57 "compiler/main.gem"
    GemVal _t93[] = {gem_table_get(gem_v_s, gem_v_i)};
            gem_v_result = gem_add(gem_mul(gem_v_result, gem_int(10)), gem_fn_digit_val(NULL, _t93, 1));
        }
    }
    GemVal _t94 = gem_v_result;
    gem_pop_frame();
    return _t94;
}

static GemVal gem_fn_contains_dot(void *_env, GemVal *args, int argc) {
#line 64 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("contains_dot", "compiler/main.gem", 64);
    {
#line 65 "compiler/main.gem"
        GemVal gem_v__for_i_2 = gem_int(0);
#line 65 "compiler/main.gem"
    GemVal _t95[] = {gem_v_s};
        GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t95, 1);
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
                GemVal _t96 = gem_bool(1);
                gem_pop_frame();
                return _t96;
            }
        }
    }
    GemVal _t97 = gem_bool(0);
    gem_pop_frame();
    return _t97;
}

static GemVal gem_fn_build_keyword_set(void *_env, GemVal *args, int argc) {
#line 75 "compiler/main.gem"
    gem_push_frame("build_keyword_set", "compiler/main.gem", 75);
#line 76 "compiler/main.gem"
    GemVal _t98 = gem_table_new();
    GemVal gem_v_t = _t98;
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
    GemVal _t99 = gem_v_t;
    gem_pop_frame();
    return _t99;
}

static GemVal gem_fn_tokenize(void *_env, GemVal *args, int argc) {
#line 103 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("tokenize", "compiler/main.gem", 103);
#line 104 "compiler/main.gem"
    GemVal gem_v_kw = gem_fn_build_keyword_set(NULL, NULL, 0);
#line 105 "compiler/main.gem"
    GemVal _t100 = gem_table_new();
    GemVal gem_v_tokens = _t100;
#line 106 "compiler/main.gem"
    GemVal gem_v_pos = gem_int(0);
#line 107 "compiler/main.gem"
    GemVal gem_v_line = gem_int(1);
#line 108 "compiler/main.gem"
    GemVal _t101[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t101, 1);
#line 109 "compiler/main.gem"
    GemVal gem_v_interp_depth = gem_int(0);
#line 110 "compiler/main.gem"
    GemVal _t102 = gem_table_new();
    GemVal gem_v_interp_brace = _t102;
#line 112 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 113 "compiler/main.gem"
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
#line 116 "compiler/main.gem"
    GemVal _t103;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t103 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t103 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t103)) {
#line 117 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 122 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 123 "compiler/main.gem"
            while (1) {
                GemVal _t104;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t104 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t104 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t104)) break;
#line 124 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 130 "compiler/main.gem"
    GemVal _t105;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t105 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t105 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t105)) {
#line 131 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 132 "compiler/main.gem"
    GemVal _t106[] = {gem_v_tokens};
    GemVal _t109;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t106, 1), gem_int(0)))) {
        _t109 = gem_gt(gem_len_fn(NULL, _t106, 1), gem_int(0));
    } else {
        GemVal _t107[] = {gem_v_tokens};
        GemVal _t108 = gem_table_get(gem_v_tokens, gem_sub(gem_len_fn(NULL, _t107, 1), gem_int(1)));
        _t109 = gem_eq(gem_table_get(_t108, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t109)) {
#line 133 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 135 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 136 "compiler/main.gem"
    GemVal _t110 = gem_table_new();
    gem_table_set(_t110, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t110, gem_string("value"), gem_string("\n"));
    gem_table_set(_t110, gem_string("line"), gem_v_line);
    GemVal _t111[] = {gem_v_tokens, _t110};
                (void)(gem_push_fn(NULL, _t111, 2));
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
    GemVal _t112;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t112 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t112 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t113;
    if (gem_truthy(_t112)) {
        _t113 = _t112;
    } else {
        _t113 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t113)) {
#line 149 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 150 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 151 "compiler/main.gem"
                            while (1) {
                                GemVal _t114;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t114 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t114 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t114)) break;
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
    GemVal _t115[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t115, 1))) {
#line 163 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 164 "compiler/main.gem"
            while (1) {
                GemVal _t117;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t117 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t116[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t117 = gem_fn_is_digit(NULL, _t116, 1);
                }
                if (!gem_truthy(_t117)) break;
#line 165 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 166 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 168 "compiler/main.gem"
    GemVal _t118;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t118 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t118 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t118;
#line 169 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 170 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 171 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 172 "compiler/main.gem"
                while (1) {
                    GemVal _t120;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t120 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t119[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t120 = gem_fn_is_digit(NULL, _t119, 1);
                    }
                    if (!gem_truthy(_t120)) break;
#line 173 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 174 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 177 "compiler/main.gem"
    GemVal _t121 = gem_table_new();
    gem_table_set(_t121, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t121, gem_string("value"), gem_v_val);
    gem_table_set(_t121, gem_string("line"), gem_v_line);
    GemVal _t122[] = {gem_v_tokens, _t121};
            (void)(gem_push_fn(NULL, _t122, 2));
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
                GemVal _t123;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t123 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t123 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t123)) break;
#line 187 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 188 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 189 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 190 "compiler/main.gem"
    GemVal _t124[] = {gem_v_line};
    GemVal _t125[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t124, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 190, _t125, 1));
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
    GemVal _t126 = gem_table_new();
    gem_table_set(_t126, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t126, gem_string("value"), gem_string(""));
    gem_table_set(_t126, gem_string("line"), gem_v_line);
    GemVal _t127[] = {gem_v_tokens, _t126};
                    (void)(gem_push_fn(NULL, _t127, 2));
#line 213 "compiler/main.gem"
    GemVal _t128 = gem_table_new();
    gem_table_set(_t128, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t128, gem_string("value"), gem_v_val);
    gem_table_set(_t128, gem_string("line"), gem_v_line);
    GemVal _t129[] = {gem_v_tokens, _t128};
                    (void)(gem_push_fn(NULL, _t129, 2));
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
    GemVal _t130[] = {gem_v_line};
    GemVal _t131[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t130, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 226, _t131, 1));
                }
#line 228 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 229 "compiler/main.gem"
    GemVal _t132 = gem_table_new();
    gem_table_set(_t132, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t132, gem_string("value"), gem_v_val);
    gem_table_set(_t132, gem_string("line"), gem_v_line);
    GemVal _t133[] = {gem_v_tokens, _t132};
                (void)(gem_push_fn(NULL, _t133, 2));
            }
            continue;
        }
#line 235 "compiler/main.gem"
    GemVal _t134[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t134, 1))) {
#line 236 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 237 "compiler/main.gem"
            while (1) {
                GemVal _t136;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t136 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t135[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t136 = gem_fn_is_alnum(NULL, _t135, 1);
                }
                if (!gem_truthy(_t136)) break;
#line 238 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 239 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 241 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_kw, gem_v_val), gem_bool(1)))) {
#line 242 "compiler/main.gem"
    GemVal _t137 = gem_table_new();
    gem_table_set(_t137, gem_string("type"), gem_v_val);
    gem_table_set(_t137, gem_string("value"), gem_v_val);
    gem_table_set(_t137, gem_string("line"), gem_v_line);
    GemVal _t138[] = {gem_v_tokens, _t137};
                (void)(gem_push_fn(NULL, _t138, 2));
            } else {
#line 244 "compiler/main.gem"
    GemVal _t139 = gem_table_new();
    gem_table_set(_t139, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t139, gem_string("value"), gem_v_val);
    gem_table_set(_t139, gem_string("line"), gem_v_line);
    GemVal _t140[] = {gem_v_tokens, _t139};
                (void)(gem_push_fn(NULL, _t140, 2));
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
    GemVal _t141;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t141 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t141 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t141)) {
#line 256 "compiler/main.gem"
    GemVal _t142 = gem_table_new();
    gem_table_set(_t142, gem_string("type"), gem_string("=="));
    gem_table_set(_t142, gem_string("value"), gem_string("=="));
    gem_table_set(_t142, gem_string("line"), gem_v_line);
    GemVal _t143[] = {gem_v_tokens, _t142};
            (void)(gem_push_fn(NULL, _t143, 2));
#line 257 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 260 "compiler/main.gem"
    GemVal _t144;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t144 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t144 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t144)) {
#line 261 "compiler/main.gem"
    GemVal _t145 = gem_table_new();
    gem_table_set(_t145, gem_string("type"), gem_string("!="));
    gem_table_set(_t145, gem_string("value"), gem_string("!="));
    gem_table_set(_t145, gem_string("line"), gem_v_line);
    GemVal _t146[] = {gem_v_tokens, _t145};
            (void)(gem_push_fn(NULL, _t146, 2));
#line 262 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 265 "compiler/main.gem"
    GemVal _t147;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t147 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t147 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t147)) {
#line 266 "compiler/main.gem"
    GemVal _t148 = gem_table_new();
    gem_table_set(_t148, gem_string("type"), gem_string("<="));
    gem_table_set(_t148, gem_string("value"), gem_string("<="));
    gem_table_set(_t148, gem_string("line"), gem_v_line);
    GemVal _t149[] = {gem_v_tokens, _t148};
            (void)(gem_push_fn(NULL, _t149, 2));
#line 267 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 270 "compiler/main.gem"
    GemVal _t150;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t150 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t150 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t150)) {
#line 271 "compiler/main.gem"
    GemVal _t151 = gem_table_new();
    gem_table_set(_t151, gem_string("type"), gem_string(">="));
    gem_table_set(_t151, gem_string("value"), gem_string(">="));
    gem_table_set(_t151, gem_string("line"), gem_v_line);
    GemVal _t152[] = {gem_v_tokens, _t151};
            (void)(gem_push_fn(NULL, _t152, 2));
#line 272 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 275 "compiler/main.gem"
    GemVal _t153;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t153 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t153 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t153)) {
#line 276 "compiler/main.gem"
    GemVal _t154 = gem_table_new();
    gem_table_set(_t154, gem_string("type"), gem_string("+="));
    gem_table_set(_t154, gem_string("value"), gem_string("+="));
    gem_table_set(_t154, gem_string("line"), gem_v_line);
    GemVal _t155[] = {gem_v_tokens, _t154};
            (void)(gem_push_fn(NULL, _t155, 2));
#line 277 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 280 "compiler/main.gem"
    GemVal _t156;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t156 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t156 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t156)) {
#line 281 "compiler/main.gem"
    GemVal _t157 = gem_table_new();
    gem_table_set(_t157, gem_string("type"), gem_string("-="));
    gem_table_set(_t157, gem_string("value"), gem_string("-="));
    gem_table_set(_t157, gem_string("line"), gem_v_line);
    GemVal _t158[] = {gem_v_tokens, _t157};
            (void)(gem_push_fn(NULL, _t158, 2));
#line 282 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 285 "compiler/main.gem"
    GemVal _t159;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t159 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t159 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t159)) {
#line 286 "compiler/main.gem"
    GemVal _t160 = gem_table_new();
    gem_table_set(_t160, gem_string("type"), gem_string("*="));
    gem_table_set(_t160, gem_string("value"), gem_string("*="));
    gem_table_set(_t160, gem_string("line"), gem_v_line);
    GemVal _t161[] = {gem_v_tokens, _t160};
            (void)(gem_push_fn(NULL, _t161, 2));
#line 287 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 290 "compiler/main.gem"
    GemVal _t162;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t162 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t162 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t162)) {
#line 291 "compiler/main.gem"
    GemVal _t163 = gem_table_new();
    gem_table_set(_t163, gem_string("type"), gem_string("/="));
    gem_table_set(_t163, gem_string("value"), gem_string("/="));
    gem_table_set(_t163, gem_string("line"), gem_v_line);
    GemVal _t164[] = {gem_v_tokens, _t163};
            (void)(gem_push_fn(NULL, _t164, 2));
#line 292 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 295 "compiler/main.gem"
    GemVal _t165;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t165 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t165 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t165)) {
#line 296 "compiler/main.gem"
    GemVal _t166 = gem_table_new();
    gem_table_set(_t166, gem_string("type"), gem_string("->"));
    gem_table_set(_t166, gem_string("value"), gem_string("->"));
    gem_table_set(_t166, gem_string("line"), gem_v_line);
    GemVal _t167[] = {gem_v_tokens, _t166};
            (void)(gem_push_fn(NULL, _t167, 2));
#line 297 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 302 "compiler/main.gem"
    GemVal _t168;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t168 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t168 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t169;
    if (gem_truthy(_t168)) {
        _t169 = _t168;
    } else {
        _t169 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t170;
    if (gem_truthy(_t169)) {
        _t170 = _t169;
    } else {
        _t170 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t171;
    if (gem_truthy(_t170)) {
        _t171 = _t170;
    } else {
        _t171 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t171)) {
#line 303 "compiler/main.gem"
    GemVal _t172 = gem_table_new();
    gem_table_set(_t172, gem_string("type"), gem_v_ch);
    gem_table_set(_t172, gem_string("value"), gem_v_ch);
    gem_table_set(_t172, gem_string("line"), gem_v_line);
    GemVal _t173[] = {gem_v_tokens, _t172};
            (void)(gem_push_fn(NULL, _t173, 2));
#line 304 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 307 "compiler/main.gem"
    GemVal _t174;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t174 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t174 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t175;
    if (gem_truthy(_t174)) {
        _t175 = _t174;
    } else {
        _t175 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t175)) {
#line 308 "compiler/main.gem"
    GemVal _t176 = gem_table_new();
    gem_table_set(_t176, gem_string("type"), gem_v_ch);
    gem_table_set(_t176, gem_string("value"), gem_v_ch);
    gem_table_set(_t176, gem_string("line"), gem_v_line);
    GemVal _t177[] = {gem_v_tokens, _t176};
            (void)(gem_push_fn(NULL, _t177, 2));
#line 309 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 313 "compiler/main.gem"
    GemVal _t178;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t178 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t178 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t178)) {
#line 314 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 315 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 316 "compiler/main.gem"
    GemVal _t179 = gem_table_new();
    gem_table_set(_t179, gem_string("type"), gem_string("{"));
    gem_table_set(_t179, gem_string("value"), gem_string("{"));
    gem_table_set(_t179, gem_string("line"), gem_v_line);
    GemVal _t180[] = {gem_v_tokens, _t179};
            (void)(gem_push_fn(NULL, _t180, 2));
#line 317 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 320 "compiler/main.gem"
    GemVal _t181;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t181 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t181 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t181)) {
#line 321 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 322 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 323 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 324 "compiler/main.gem"
    GemVal _t182 = gem_table_new();
    gem_table_set(_t182, gem_string("type"), gem_string("}"));
    gem_table_set(_t182, gem_string("value"), gem_string("}"));
    gem_table_set(_t182, gem_string("line"), gem_v_line);
    GemVal _t183[] = {gem_v_tokens, _t182};
                (void)(gem_push_fn(NULL, _t183, 2));
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
                GemVal _t184;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t184 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t184 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t184)) break;
#line 334 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 335 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 336 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 337 "compiler/main.gem"
    GemVal _t185[] = {gem_v_line};
    GemVal _t186[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t185, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 337, _t186, 1));
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
    GemVal _t187 = gem_table_new();
    gem_table_set(_t187, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t187, gem_string("value"), gem_v_val);
    gem_table_set(_t187, gem_string("line"), gem_v_line);
    GemVal _t188[] = {gem_v_tokens, _t187};
                    (void)(gem_push_fn(NULL, _t188, 2));
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
    GemVal _t189[] = {gem_v_line};
    GemVal _t190[] = {gem_add(gem_string("unterminated string interpolation at line "), gem_to_string_fn(NULL, _t189, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 373, _t190, 1));
                }
#line 375 "compiler/main.gem"
    GemVal _t191 = gem_table_new();
    gem_table_set(_t191, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t191, gem_string("value"), gem_v_val);
    gem_table_set(_t191, gem_string("line"), gem_v_line);
    GemVal _t192[] = {gem_v_tokens, _t191};
                (void)(gem_push_fn(NULL, _t192, 2));
#line 376 "compiler/main.gem"
    GemVal _t193 = gem_table_new();
    gem_table_set(_t193, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t193, gem_string("value"), gem_string(""));
    gem_table_set(_t193, gem_string("line"), gem_v_line);
    GemVal _t194[] = {gem_v_tokens, _t193};
                (void)(gem_push_fn(NULL, _t194, 2));
#line 377 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 382 "compiler/main.gem"
    GemVal _t195;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t195 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t195 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t196;
    if (gem_truthy(_t195)) {
        _t196 = _t195;
    } else {
        _t196 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t197;
    if (gem_truthy(_t196)) {
        _t197 = _t196;
    } else {
        _t197 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t198;
    if (gem_truthy(_t197)) {
        _t198 = _t197;
    } else {
        _t198 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t199;
    if (gem_truthy(_t198)) {
        _t199 = _t198;
    } else {
        _t199 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t199)) {
#line 383 "compiler/main.gem"
    GemVal _t200 = gem_table_new();
    gem_table_set(_t200, gem_string("type"), gem_v_ch);
    gem_table_set(_t200, gem_string("value"), gem_v_ch);
    gem_table_set(_t200, gem_string("line"), gem_v_line);
    GemVal _t201[] = {gem_v_tokens, _t200};
            (void)(gem_push_fn(NULL, _t201, 2));
#line 384 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 387 "compiler/main.gem"
    GemVal _t202;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t202 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t202 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t203;
    if (gem_truthy(_t202)) {
        _t203 = _t202;
    } else {
        _t203 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t204;
    if (gem_truthy(_t203)) {
        _t204 = _t203;
    } else {
        _t204 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t204)) {
#line 388 "compiler/main.gem"
    GemVal _t205 = gem_table_new();
    gem_table_set(_t205, gem_string("type"), gem_v_ch);
    gem_table_set(_t205, gem_string("value"), gem_v_ch);
    gem_table_set(_t205, gem_string("line"), gem_v_line);
    GemVal _t206[] = {gem_v_tokens, _t205};
            (void)(gem_push_fn(NULL, _t206, 2));
#line 389 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 393 "compiler/main.gem"
    GemVal _t207[] = {gem_v_ch};
    GemVal _t208[] = {gem_v_line};
    GemVal _t209[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t207, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t208, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 393, _t209, 1));
    }
#line 396 "compiler/main.gem"
    GemVal _t210 = gem_table_new();
    gem_table_set(_t210, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t210, gem_string("value"), gem_string(""));
    gem_table_set(_t210, gem_string("line"), gem_v_line);
    GemVal _t211[] = {gem_v_tokens, _t210};
    (void)(gem_push_fn(NULL, _t211, 2));
    GemVal _t212 = gem_v_tokens;
    gem_pop_frame();
    return _t212;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 10 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_source", "compiler/main.gem", 10);
#line 11 "compiler/main.gem"
    GemVal _t213[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t213, 1);
#line 12 "compiler/main.gem"
    GemVal _t214[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t214, 1);
#line 13 "compiler/main.gem"
    GemVal _t215 = gem_v_p;
    GemVal _t216 = gem_table_get(_t215, gem_string("parse"));
    GemVal _t217 = _t216.fn(_t216.env, NULL, 0);
    gem_pop_frame();
    return _t217;
}

struct _closure__anon_1 {
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_1(void *_env, GemVal *args, int argc) {
    struct _closure__anon_1 *_cls = (struct _closure__anon_1 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    gem_push_frame("_anon_1", "compiler/main.gem", 0);
    GemVal _t218 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t218;
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
    gem_push_frame("_anon_2", "compiler/main.gem", 0);
    GemVal _t220 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t220;
}

struct _closure__anon_3 {
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_3(void *_env, GemVal *args, int argc) {
    struct _closure__anon_3 *_cls = (struct _closure__anon_3 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    gem_push_frame("_anon_3", "compiler/main.gem", 0);
#line 34 "compiler/main.gem"
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
#line 35 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t222 = gem_v_t;
    gem_pop_frame();
    return _t222;
}

struct _closure__anon_4 {
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_4(void *_env, GemVal *args, int argc) {
    struct _closure__anon_4 *_cls = (struct _closure__anon_4 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    gem_push_frame("_anon_4", "compiler/main.gem", 0);
    GemVal _t224 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t225 = gem_eq(gem_table_get(_t224, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t225;
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
    gem_push_frame("_anon_5", "compiler/main.gem", 0);
#line 44 "compiler/main.gem"
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
#line 45 "compiler/main.gem"
    GemVal _t227 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t227, gem_string("type")), gem_v_tp))) {
#line 46 "compiler/main.gem"
    GemVal _t228[] = {gem_v_tp};
    GemVal _t229 = gem_v_t;
    GemVal _t230[] = {gem_table_get(_t229, gem_string("type"))};
    GemVal _t231 = gem_v_t;
    GemVal _t232[] = {gem_table_get(_t231, gem_string("line"))};
    GemVal _t233[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t228, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t230, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t232, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 46, _t233, 1));
    }
#line 48 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t234 = gem_v_t;
    gem_pop_frame();
    return _t234;
}

struct _closure__anon_6 {
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_6(void *_env, GemVal *args, int argc) {
    struct _closure__anon_6 *_cls = (struct _closure__anon_6 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    gem_push_frame("_anon_6", "compiler/main.gem", 0);
#line 53 "compiler/main.gem"
    while (1) {
        GemVal _t236 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t236, gem_string("type")), gem_string("NEWLINE")))) break;
#line 54 "compiler/main.gem"
        *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    }
    gem_pop_frame();
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
    gem_push_frame("_anon_7", "compiler/main.gem", 0);
#line 61 "compiler/main.gem"
    GemVal _t238 = gem_table_new();
    GemVal gem_v_params = _t238;
#line 62 "compiler/main.gem"
    GemVal _t239 = (*gem_v_peek);
    GemVal _t240 = _t239.fn(_t239.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t240, gem_string("type")), gem_string("NAME")))) {
#line 63 "compiler/main.gem"
    GemVal _t241 = (*gem_v_advance);
    GemVal _t242 = _t241.fn(_t241.env, NULL, 0);
    GemVal _t243[] = {gem_v_params, gem_table_get(_t242, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t243, 2));
#line 64 "compiler/main.gem"
        while (1) {
            GemVal _t244 = (*gem_v_peek);
            GemVal _t245 = _t244.fn(_t244.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t245, gem_string("type")), gem_string(",")))) break;
#line 65 "compiler/main.gem"
    GemVal _t246 = (*gem_v_advance);
            (void)(_t246.fn(_t246.env, NULL, 0));
#line 66 "compiler/main.gem"
    GemVal _t247 = (*gem_v_expect);
    GemVal _t248[] = {gem_string("NAME")};
    GemVal _t249 = _t247.fn(_t247.env, _t248, 1);
    GemVal _t250[] = {gem_v_params, gem_table_get(_t249, gem_string("value"))};
            (void)(gem_push_fn(NULL, _t250, 2));
        }
    }
    GemVal _t251 = gem_v_params;
    gem_pop_frame();
    return _t251;
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
    gem_push_frame("_anon_8", "compiler/main.gem", 0);
#line 84 "compiler/main.gem"
    GemVal _t253 = gem_table_new();
    GemVal gem_v_stmts = _t253;
#line 85 "compiler/main.gem"
    GemVal _t254 = (*gem_v_skip_nl);
    (void)(_t254.fn(_t254.env, NULL, 0));
#line 86 "compiler/main.gem"
    while (1) {
        GemVal _t255 = (*gem_v_peek);
        GemVal _t256 = _t255.fn(_t255.env, NULL, 0);
        GemVal _t259;
        if (!gem_truthy(gem_neq(gem_table_get(_t256, gem_string("type")), gem_string("end")))) {
                _t259 = gem_neq(gem_table_get(_t256, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t257 = (*gem_v_peek);
                GemVal _t258 = _t257.fn(_t257.env, NULL, 0);
                _t259 = gem_neq(gem_table_get(_t258, gem_string("type")), gem_string("elif"));
        }
        GemVal _t262;
        if (!gem_truthy(_t259)) {
                _t262 = _t259;
        } else {
                GemVal _t260 = (*gem_v_peek);
                GemVal _t261 = _t260.fn(_t260.env, NULL, 0);
                _t262 = gem_neq(gem_table_get(_t261, gem_string("type")), gem_string("else"));
        }
        GemVal _t265;
        if (!gem_truthy(_t262)) {
                _t265 = _t262;
        } else {
                GemVal _t263 = (*gem_v_peek);
                GemVal _t264 = _t263.fn(_t263.env, NULL, 0);
                _t265 = gem_neq(gem_table_get(_t264, gem_string("type")), gem_string("when"));
        }
        GemVal _t267;
        if (!gem_truthy(_t265)) {
                _t267 = _t265;
        } else {
                GemVal _t266 = (*gem_v_at_end);
                _t267 = gem_not(_t266.fn(_t266.env, NULL, 0));
        }
        if (!gem_truthy(_t267)) break;
#line 87 "compiler/main.gem"
    GemVal _t268 = (*gem_v_parse_stmt);
    GemVal _t269[] = {gem_v_stmts, _t268.fn(_t268.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t269, 2));
#line 88 "compiler/main.gem"
    GemVal _t270 = (*gem_v_skip_nl);
        (void)(_t270.fn(_t270.env, NULL, 0));
    }
    GemVal _t271 = gem_v_stmts;
    gem_pop_frame();
    return _t271;
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
    gem_push_frame("_anon_9", "compiler/main.gem", 0);
#line 97 "compiler/main.gem"
    GemVal _t273 = gem_table_new();
    GemVal gem_v_stmts = _t273;
#line 98 "compiler/main.gem"
    GemVal _t274 = (*gem_v_skip_nl);
    (void)(_t274.fn(_t274.env, NULL, 0));
#line 99 "compiler/main.gem"
    while (1) {
        GemVal _t275 = (*gem_v_peek);
        GemVal _t276 = _t275.fn(_t275.env, NULL, 0);
        GemVal _t278;
        if (!gem_truthy(gem_neq(gem_table_get(_t276, gem_string("type")), gem_string("end")))) {
                _t278 = gem_neq(gem_table_get(_t276, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t277 = (*gem_v_at_end);
                _t278 = gem_not(_t277.fn(_t277.env, NULL, 0));
        }
        if (!gem_truthy(_t278)) break;
#line 100 "compiler/main.gem"
    GemVal _t279 = (*gem_v_parse_stmt);
    GemVal _t280[] = {gem_v_stmts, _t279.fn(_t279.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t280, 2));
#line 101 "compiler/main.gem"
    GemVal _t281 = (*gem_v_skip_nl);
        (void)(_t281.fn(_t281.env, NULL, 0));
    }
    GemVal _t282 = gem_v_stmts;
    gem_pop_frame();
    return _t282;
}

struct _closure__anon_10 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_fn_depth;
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
    GemVal *gem_v_fn_depth = _cls->gem_v_fn_depth;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body = _cls->gem_v_parse_fn_body;
    GemVal *gem_v_parse_params = _cls->gem_v_parse_params;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    gem_push_frame("_anon_10", "compiler/main.gem", 0);
#line 110 "compiler/main.gem"
    GemVal _t284 = (*gem_v_peek);
    GemVal gem_v_t = _t284.fn(_t284.env, NULL, 0);
#line 113 "compiler/main.gem"
    GemVal _t285 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t285, gem_string("type")), gem_string("NUMBER")))) {
#line 114 "compiler/main.gem"
    GemVal _t286 = (*gem_v_advance);
        (void)(_t286.fn(_t286.env, NULL, 0));
#line 115 "compiler/main.gem"
    GemVal _t287 = gem_v_t;
    GemVal _t288[] = {gem_table_get(_t287, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t288, 1))) {
#line 116 "compiler/main.gem"
    GemVal _t289 = gem_v_t;
    GemVal _t290[] = {gem_table_get(_t289, gem_string("value"))};
    GemVal _t291[] = {gem_fn_atof(NULL, _t290, 1)};
            GemVal _t292 = gem_fn_make_float(NULL, _t291, 1);
            gem_pop_frame();
            return _t292;
        }
#line 118 "compiler/main.gem"
    GemVal _t293 = gem_v_t;
    GemVal _t294[] = {gem_table_get(_t293, gem_string("value"))};
    GemVal _t295[] = {gem_fn_str_to_int(NULL, _t294, 1)};
        GemVal _t296 = gem_fn_make_int(NULL, _t295, 1);
        gem_pop_frame();
        return _t296;
    }
#line 122 "compiler/main.gem"
    GemVal _t297 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t297, gem_string("type")), gem_string("STRING")))) {
#line 123 "compiler/main.gem"
    GemVal _t298 = (*gem_v_advance);
        (void)(_t298.fn(_t298.env, NULL, 0));
#line 124 "compiler/main.gem"
    GemVal _t299 = gem_v_t;
    GemVal _t300[] = {gem_table_get(_t299, gem_string("value"))};
        GemVal _t301 = gem_fn_make_string(NULL, _t300, 1);
        gem_pop_frame();
        return _t301;
    }
#line 128 "compiler/main.gem"
    GemVal _t302 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t302, gem_string("type")), gem_string("INTERP_START")))) {
#line 129 "compiler/main.gem"
    GemVal _t303 = (*gem_v_advance);
        (void)(_t303.fn(_t303.env, NULL, 0));
#line 130 "compiler/main.gem"
    GemVal _t304 = gem_table_new();
        GemVal gem_v_parts = _t304;
#line 131 "compiler/main.gem"
    GemVal _t305 = (*gem_v_skip_nl);
        (void)(_t305.fn(_t305.env, NULL, 0));
#line 132 "compiler/main.gem"
        while (1) {
            GemVal _t306 = (*gem_v_peek);
            GemVal _t307 = _t306.fn(_t306.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t307, gem_string("type")), gem_string("INTERP_END")))) break;
#line 133 "compiler/main.gem"
    GemVal _t308 = (*gem_v_peek);
    GemVal _t309 = _t308.fn(_t308.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t309, gem_string("type")), gem_string("STRING")))) {
#line 134 "compiler/main.gem"
    GemVal _t310 = (*gem_v_advance);
    GemVal _t311 = _t310.fn(_t310.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t311, gem_string("value"));
#line 135 "compiler/main.gem"
    GemVal _t312[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t312, 1), gem_int(0)))) {
#line 136 "compiler/main.gem"
    GemVal _t313[] = {gem_v_sval};
    GemVal _t314[] = {gem_v_parts, gem_fn_make_string(NULL, _t313, 1)};
                    (void)(gem_push_fn(NULL, _t314, 2));
                }
            } else {
#line 139 "compiler/main.gem"
    GemVal _t315 = (*gem_v_parse_expr);
    GemVal _t316[] = {gem_v_parts, _t315.fn(_t315.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t316, 2));
            }
#line 141 "compiler/main.gem"
    GemVal _t317 = (*gem_v_skip_nl);
            (void)(_t317.fn(_t317.env, NULL, 0));
        }
#line 143 "compiler/main.gem"
    GemVal _t318 = (*gem_v_expect);
    GemVal _t319[] = {gem_string("INTERP_END")};
        (void)(_t318.fn(_t318.env, _t319, 1));
#line 144 "compiler/main.gem"
    GemVal _t320[] = {gem_v_parts};
        GemVal _t321 = gem_fn_make_interp(NULL, _t320, 1);
        gem_pop_frame();
        return _t321;
    }
#line 148 "compiler/main.gem"
    GemVal _t322 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t322, gem_string("type")), gem_string("true")))) {
#line 149 "compiler/main.gem"
    GemVal _t323 = (*gem_v_advance);
        (void)(_t323.fn(_t323.env, NULL, 0));
#line 150 "compiler/main.gem"
    GemVal _t324[] = {gem_bool(1)};
        GemVal _t325 = gem_fn_make_bool(NULL, _t324, 1);
        gem_pop_frame();
        return _t325;
    }
#line 152 "compiler/main.gem"
    GemVal _t326 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t326, gem_string("type")), gem_string("false")))) {
#line 153 "compiler/main.gem"
    GemVal _t327 = (*gem_v_advance);
        (void)(_t327.fn(_t327.env, NULL, 0));
#line 154 "compiler/main.gem"
    GemVal _t328[] = {gem_bool(0)};
        GemVal _t329 = gem_fn_make_bool(NULL, _t328, 1);
        gem_pop_frame();
        return _t329;
    }
#line 158 "compiler/main.gem"
    GemVal _t330 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t330, gem_string("type")), gem_string("nil")))) {
#line 159 "compiler/main.gem"
    GemVal _t331 = (*gem_v_advance);
        (void)(_t331.fn(_t331.env, NULL, 0));
#line 160 "compiler/main.gem"
        GemVal _t332 = gem_fn_make_nil_node(NULL, NULL, 0);
        gem_pop_frame();
        return _t332;
    }
#line 164 "compiler/main.gem"
    GemVal _t333 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t333, gem_string("type")), gem_string("fn")))) {
#line 165 "compiler/main.gem"
    GemVal _t334 = (*gem_v_advance);
        (void)(_t334.fn(_t334.env, NULL, 0));
#line 166 "compiler/main.gem"
    GemVal _t335 = (*gem_v_expect);
    GemVal _t336[] = {gem_string("(")};
        (void)(_t335.fn(_t335.env, _t336, 1));
#line 167 "compiler/main.gem"
    GemVal _t337 = (*gem_v_parse_params);
        GemVal gem_v_params = _t337.fn(_t337.env, NULL, 0);
#line 168 "compiler/main.gem"
    GemVal _t338 = (*gem_v_expect);
    GemVal _t339[] = {gem_string(")")};
        (void)(_t338.fn(_t338.env, _t339, 1));
#line 169 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 170 "compiler/main.gem"
    GemVal _t340 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t340.fn(_t340.env, NULL, 0);
#line 171 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 172 "compiler/main.gem"
    GemVal _t341 = (*gem_v_expect);
    GemVal _t342[] = {gem_string("end")};
        (void)(_t341.fn(_t341.env, _t342, 1));
#line 173 "compiler/main.gem"
    GemVal _t343[] = {gem_v_params, gem_v_body};
        GemVal _t344 = gem_fn_make_anon_fn(NULL, _t343, 2);
        gem_pop_frame();
        return _t344;
    }
#line 177 "compiler/main.gem"
    GemVal _t345 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t345, gem_string("type")), gem_string("{")))) {
#line 178 "compiler/main.gem"
    GemVal _t346 = (*gem_v_advance);
        (void)(_t346.fn(_t346.env, NULL, 0));
#line 179 "compiler/main.gem"
    GemVal _t347 = (*gem_v_skip_nl);
        (void)(_t347.fn(_t347.env, NULL, 0));
#line 180 "compiler/main.gem"
    GemVal _t348 = gem_table_new();
        GemVal gem_v_entries = _t348;
#line 181 "compiler/main.gem"
        while (1) {
            GemVal _t349 = (*gem_v_peek);
            GemVal _t350 = _t349.fn(_t349.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t350, gem_string("type")), gem_string("}")))) break;
#line 182 "compiler/main.gem"
    GemVal _t351 = (*gem_v_expect);
    GemVal _t352[] = {gem_string("NAME")};
    GemVal _t353 = _t351.fn(_t351.env, _t352, 1);
            GemVal gem_v_key = gem_table_get(_t353, gem_string("value"));
#line 183 "compiler/main.gem"
    GemVal _t354 = (*gem_v_expect);
    GemVal _t355[] = {gem_string(":")};
            (void)(_t354.fn(_t354.env, _t355, 1));
#line 184 "compiler/main.gem"
    GemVal _t356 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t356.fn(_t356.env, NULL, 0);
#line 185 "compiler/main.gem"
    GemVal _t357[] = {gem_v_key, gem_v_val};
    GemVal _t358[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t357, 2)};
            (void)(gem_push_fn(NULL, _t358, 2));
#line 186 "compiler/main.gem"
    GemVal _t359 = (*gem_v_skip_nl);
            (void)(_t359.fn(_t359.env, NULL, 0));
#line 187 "compiler/main.gem"
    GemVal _t360 = (*gem_v_peek);
    GemVal _t361 = _t360.fn(_t360.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t361, gem_string("type")), gem_string(",")))) {
#line 188 "compiler/main.gem"
    GemVal _t362 = (*gem_v_advance);
                (void)(_t362.fn(_t362.env, NULL, 0));
#line 189 "compiler/main.gem"
    GemVal _t363 = (*gem_v_skip_nl);
                (void)(_t363.fn(_t363.env, NULL, 0));
            }
        }
#line 192 "compiler/main.gem"
    GemVal _t364 = (*gem_v_expect);
    GemVal _t365[] = {gem_string("}")};
        (void)(_t364.fn(_t364.env, _t365, 1));
#line 193 "compiler/main.gem"
    GemVal _t366[] = {gem_v_entries};
        GemVal _t367 = gem_fn_make_table(NULL, _t366, 1);
        gem_pop_frame();
        return _t367;
    }
#line 197 "compiler/main.gem"
    GemVal _t368 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t368, gem_string("type")), gem_string("[")))) {
#line 198 "compiler/main.gem"
    GemVal _t369 = (*gem_v_advance);
        (void)(_t369.fn(_t369.env, NULL, 0));
#line 199 "compiler/main.gem"
    GemVal _t370 = (*gem_v_skip_nl);
        (void)(_t370.fn(_t370.env, NULL, 0));
#line 200 "compiler/main.gem"
    GemVal _t371 = gem_table_new();
        GemVal gem_v_elements = _t371;
#line 201 "compiler/main.gem"
        while (1) {
            GemVal _t372 = (*gem_v_peek);
            GemVal _t373 = _t372.fn(_t372.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t373, gem_string("type")), gem_string("]")))) break;
#line 202 "compiler/main.gem"
    GemVal _t374 = (*gem_v_parse_expr);
    GemVal _t375[] = {gem_v_elements, _t374.fn(_t374.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t375, 2));
#line 203 "compiler/main.gem"
    GemVal _t376 = (*gem_v_skip_nl);
            (void)(_t376.fn(_t376.env, NULL, 0));
#line 204 "compiler/main.gem"
    GemVal _t377 = (*gem_v_peek);
    GemVal _t378 = _t377.fn(_t377.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t378, gem_string("type")), gem_string(",")))) {
#line 205 "compiler/main.gem"
    GemVal _t379 = (*gem_v_advance);
                (void)(_t379.fn(_t379.env, NULL, 0));
#line 206 "compiler/main.gem"
    GemVal _t380 = (*gem_v_skip_nl);
                (void)(_t380.fn(_t380.env, NULL, 0));
            }
        }
#line 209 "compiler/main.gem"
    GemVal _t381 = (*gem_v_expect);
    GemVal _t382[] = {gem_string("]")};
        (void)(_t381.fn(_t381.env, _t382, 1));
#line 210 "compiler/main.gem"
    GemVal _t383[] = {gem_v_elements};
        GemVal _t384 = gem_fn_make_array(NULL, _t383, 1);
        gem_pop_frame();
        return _t384;
    }
#line 214 "compiler/main.gem"
    GemVal _t385 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t385, gem_string("type")), gem_string("(")))) {
#line 215 "compiler/main.gem"
    GemVal _t386 = (*gem_v_advance);
        (void)(_t386.fn(_t386.env, NULL, 0));
#line 216 "compiler/main.gem"
    GemVal _t387 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t387.fn(_t387.env, NULL, 0);
#line 217 "compiler/main.gem"
    GemVal _t388 = (*gem_v_expect);
    GemVal _t389[] = {gem_string(")")};
        (void)(_t388.fn(_t388.env, _t389, 1));
#line 218 "compiler/main.gem"
        GemVal _t390 = gem_v_e;
        gem_pop_frame();
        return _t390;
    }
#line 222 "compiler/main.gem"
    GemVal _t391 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t391, gem_string("type")), gem_string("NAME")))) {
#line 223 "compiler/main.gem"
    GemVal _t392 = (*gem_v_advance);
        (void)(_t392.fn(_t392.env, NULL, 0));
#line 224 "compiler/main.gem"
    GemVal _t393 = gem_v_t;
    GemVal _t394[] = {gem_table_get(_t393, gem_string("value"))};
        GemVal _t395 = gem_fn_make_var(NULL, _t394, 1);
        gem_pop_frame();
        return _t395;
    }
#line 227 "compiler/main.gem"
    GemVal _t396 = gem_v_t;
    GemVal _t397[] = {gem_table_get(_t396, gem_string("type"))};
    GemVal _t398 = gem_v_t;
    GemVal _t399[] = {gem_table_get(_t398, gem_string("line"))};
    GemVal _t400[] = {gem_add(gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t397, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t399, 1))};
    GemVal _t401 = gem_error_at_fn("compiler/main.gem", 227, _t400, 1);
    gem_pop_frame();
    return _t401;
}

struct _closure__anon_11 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_fn_depth;
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
    GemVal *gem_v_fn_depth = _cls->gem_v_fn_depth;
    GemVal *gem_v_parse_atom = _cls->gem_v_parse_atom;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body = _cls->gem_v_parse_fn_body;
    GemVal *gem_v_parse_params = _cls->gem_v_parse_params;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_peek_at = _cls->gem_v_peek_at;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    gem_push_frame("_anon_11", "compiler/main.gem", 0);
#line 232 "compiler/main.gem"
    GemVal _t403 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t403.fn(_t403.env, NULL, 0);
#line 234 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
#line 236 "compiler/main.gem"
    GemVal _t404 = (*gem_v_peek);
    GemVal _t405 = _t404.fn(_t404.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t405, gem_string("type")), gem_string("(")))) {
#line 237 "compiler/main.gem"
    GemVal _t406 = (*gem_v_peek);
    GemVal _t407 = _t406.fn(_t406.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t407, gem_string("line"));
#line 238 "compiler/main.gem"
    GemVal _t408 = (*gem_v_advance);
            (void)(_t408.fn(_t408.env, NULL, 0));
#line 239 "compiler/main.gem"
    GemVal _t409 = (*gem_v_skip_nl);
            (void)(_t409.fn(_t409.env, NULL, 0));
#line 240 "compiler/main.gem"
    GemVal _t410 = gem_table_new();
            GemVal gem_v_args = _t410;
#line 241 "compiler/main.gem"
    GemVal _t411 = (*gem_v_peek);
    GemVal _t412 = _t411.fn(_t411.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t412, gem_string("type")), gem_string(")")))) {
#line 242 "compiler/main.gem"
    GemVal _t413 = (*gem_v_parse_expr);
    GemVal _t414[] = {gem_v_args, _t413.fn(_t413.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t414, 2));
#line 243 "compiler/main.gem"
                while (1) {
                    GemVal _t415 = (*gem_v_peek);
                    GemVal _t416 = _t415.fn(_t415.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t416, gem_string("type")), gem_string(",")))) break;
#line 244 "compiler/main.gem"
    GemVal _t417 = (*gem_v_advance);
                    (void)(_t417.fn(_t417.env, NULL, 0));
#line 245 "compiler/main.gem"
    GemVal _t418 = (*gem_v_skip_nl);
                    (void)(_t418.fn(_t418.env, NULL, 0));
#line 246 "compiler/main.gem"
    GemVal _t419 = (*gem_v_parse_expr);
    GemVal _t420[] = {gem_v_args, _t419.fn(_t419.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t420, 2));
                }
            }
#line 249 "compiler/main.gem"
    GemVal _t421 = (*gem_v_skip_nl);
            (void)(_t421.fn(_t421.env, NULL, 0));
#line 250 "compiler/main.gem"
    GemVal _t422 = (*gem_v_expect);
    GemVal _t423[] = {gem_string(")")};
            (void)(_t422.fn(_t422.env, _t423, 1));
#line 253 "compiler/main.gem"
    GemVal _t424 = (*gem_v_peek);
    GemVal _t425 = _t424.fn(_t424.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t425, gem_string("type")), gem_string("do")))) {
#line 254 "compiler/main.gem"
    GemVal _t426 = (*gem_v_advance);
                (void)(_t426.fn(_t426.env, NULL, 0));
#line 255 "compiler/main.gem"
    GemVal _t427 = gem_table_new();
                GemVal gem_v_bparams = _t427;
#line 256 "compiler/main.gem"
    GemVal _t428 = (*gem_v_peek);
    GemVal _t429 = _t428.fn(_t428.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t429, gem_string("type")), gem_string("|")))) {
#line 257 "compiler/main.gem"
    GemVal _t430 = (*gem_v_advance);
                    (void)(_t430.fn(_t430.env, NULL, 0));
#line 258 "compiler/main.gem"
    GemVal _t431 = (*gem_v_parse_params);
                    gem_v_bparams = _t431.fn(_t431.env, NULL, 0);
#line 259 "compiler/main.gem"
    GemVal _t432 = (*gem_v_expect);
    GemVal _t433[] = {gem_string("|")};
                    (void)(_t432.fn(_t432.env, _t433, 1));
                }
#line 261 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 262 "compiler/main.gem"
    GemVal _t434 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t434.fn(_t434.env, NULL, 0);
#line 263 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 264 "compiler/main.gem"
    GemVal _t435 = (*gem_v_expect);
    GemVal _t436[] = {gem_string("end")};
                (void)(_t435.fn(_t435.env, _t436, 1));
#line 265 "compiler/main.gem"
    GemVal _t437[] = {gem_v_bparams, gem_v_body};
    GemVal _t438[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t437, 2)};
                (void)(gem_push_fn(NULL, _t438, 2));
            } else {
#line 269 "compiler/main.gem"
    GemVal _t439 = (*gem_v_peek);
    GemVal _t440 = _t439.fn(_t439.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t440, gem_string("type")), gem_string("{")))) {
#line 270 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 271 "compiler/main.gem"
    GemVal _t441 = (*gem_v_peek_at);
    GemVal _t442[] = {gem_int(1)};
    GemVal _t443 = _t441.fn(_t441.env, _t442, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t443, gem_string("type")), gem_string("|")))) {
#line 272 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 273 "compiler/main.gem"
    GemVal _t444 = (*gem_v_peek_at);
    GemVal _t445[] = {gem_int(1)};
    GemVal _t446 = _t444.fn(_t444.env, _t445, 1);
    GemVal _t454;
    if (gem_truthy(gem_eq(gem_table_get(_t446, gem_string("type")), gem_string("}")))) {
        _t454 = gem_eq(gem_table_get(_t446, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t447 = (*gem_v_peek_at);
        GemVal _t448[] = {gem_int(1)};
        GemVal _t449 = _t447.fn(_t447.env, _t448, 1);
        GemVal _t453;
        if (!gem_truthy(gem_eq(gem_table_get(_t449, gem_string("type")), gem_string("NAME")))) {
                _t453 = gem_eq(gem_table_get(_t449, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t450 = (*gem_v_peek_at);
                GemVal _t451[] = {gem_int(2)};
                GemVal _t452 = _t450.fn(_t450.env, _t451, 1);
                _t453 = gem_eq(gem_table_get(_t452, gem_string("type")), gem_string(":"));
        }
        _t454 = _t453;
    }
                        if (gem_truthy(_t454)) {
#line 274 "compiler/main.gem"
                            gem_v_is_block = gem_bool(0);
                        } else {
#line 276 "compiler/main.gem"
                            gem_v_is_block = gem_bool(1);
                        }
                    }
#line 278 "compiler/main.gem"
                    if (gem_truthy(gem_v_is_block)) {
#line 279 "compiler/main.gem"
    GemVal _t455 = (*gem_v_advance);
                        (void)(_t455.fn(_t455.env, NULL, 0));
#line 280 "compiler/main.gem"
    GemVal _t456 = gem_table_new();
                        GemVal gem_v_bparams = _t456;
#line 281 "compiler/main.gem"
    GemVal _t457 = (*gem_v_peek);
    GemVal _t458 = _t457.fn(_t457.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t458, gem_string("type")), gem_string("|")))) {
#line 282 "compiler/main.gem"
    GemVal _t459 = (*gem_v_advance);
                            (void)(_t459.fn(_t459.env, NULL, 0));
#line 283 "compiler/main.gem"
    GemVal _t460 = (*gem_v_parse_params);
                            gem_v_bparams = _t460.fn(_t460.env, NULL, 0);
#line 284 "compiler/main.gem"
    GemVal _t461 = (*gem_v_expect);
    GemVal _t462[] = {gem_string("|")};
                            (void)(_t461.fn(_t461.env, _t462, 1));
                        }
#line 286 "compiler/main.gem"
    GemVal _t463 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t463.fn(_t463.env, NULL, 0);
#line 287 "compiler/main.gem"
    GemVal _t464 = (*gem_v_expect);
    GemVal _t465[] = {gem_string("}")};
                        (void)(_t464.fn(_t464.env, _t465, 1));
#line 288 "compiler/main.gem"
    GemVal _t466 = gem_table_new();
    gem_table_set(_t466, gem_int(0), gem_v_bexpr);
    GemVal _t467[] = {gem_v_bparams, _t466};
    GemVal _t468[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t467, 2)};
                        (void)(gem_push_fn(NULL, _t468, 2));
                    }
                }
            }
#line 293 "compiler/main.gem"
    GemVal _t469[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t469, 3);
            continue;
        }
#line 298 "compiler/main.gem"
    GemVal _t470 = (*gem_v_peek);
    GemVal _t471 = _t470.fn(_t470.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t471, gem_string("type")), gem_string(".")))) {
#line 299 "compiler/main.gem"
    GemVal _t472 = (*gem_v_advance);
            (void)(_t472.fn(_t472.env, NULL, 0));
#line 300 "compiler/main.gem"
    GemVal _t473 = (*gem_v_expect);
    GemVal _t474[] = {gem_string("NAME")};
    GemVal _t475 = _t473.fn(_t473.env, _t474, 1);
            GemVal gem_v_field = gem_table_get(_t475, gem_string("value"));
#line 301 "compiler/main.gem"
    GemVal _t476[] = {gem_v_node, gem_v_field};
            gem_v_node = gem_fn_make_dot(NULL, _t476, 2);
            continue;
        }
#line 306 "compiler/main.gem"
    GemVal _t477 = (*gem_v_peek);
    GemVal _t478 = _t477.fn(_t477.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t478, gem_string("type")), gem_string("[")))) {
#line 307 "compiler/main.gem"
    GemVal _t479 = (*gem_v_advance);
            (void)(_t479.fn(_t479.env, NULL, 0));
#line 308 "compiler/main.gem"
    GemVal _t480 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t480.fn(_t480.env, NULL, 0);
#line 309 "compiler/main.gem"
    GemVal _t481 = (*gem_v_expect);
    GemVal _t482[] = {gem_string("]")};
            (void)(_t481.fn(_t481.env, _t482, 1));
#line 310 "compiler/main.gem"
    GemVal _t483[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t483, 2);
            continue;
        }
        break;
    }
    GemVal _t484 = gem_v_node;
    gem_pop_frame();
    return _t484;
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
    gem_push_frame("_anon_12", "compiler/main.gem", 0);
#line 322 "compiler/main.gem"
    GemVal _t486 = (*gem_v_peek);
    GemVal _t487 = _t486.fn(_t486.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t487, gem_string("type")), gem_string("-")))) {
#line 323 "compiler/main.gem"
    GemVal _t488 = (*gem_v_advance);
        (void)(_t488.fn(_t488.env, NULL, 0));
#line 324 "compiler/main.gem"
    GemVal _t489 = (*gem_v_parse_unary);
    GemVal _t490[] = {gem_string("-"), _t489.fn(_t489.env, NULL, 0)};
        GemVal _t491 = gem_fn_make_unop(NULL, _t490, 2);
        gem_pop_frame();
        return _t491;
    }
#line 326 "compiler/main.gem"
    GemVal _t492 = (*gem_v_parse_call);
    GemVal _t493 = _t492.fn(_t492.env, NULL, 0);
    gem_pop_frame();
    return _t493;
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
    gem_push_frame("_anon_13", "compiler/main.gem", 0);
#line 331 "compiler/main.gem"
    GemVal _t495 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t495.fn(_t495.env, NULL, 0);
#line 332 "compiler/main.gem"
    while (1) {
        GemVal _t496 = (*gem_v_peek);
        GemVal _t497 = _t496.fn(_t496.env, NULL, 0);
        GemVal _t500;
        if (gem_truthy(gem_eq(gem_table_get(_t497, gem_string("type")), gem_string("*")))) {
                _t500 = gem_eq(gem_table_get(_t497, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t498 = (*gem_v_peek);
                GemVal _t499 = _t498.fn(_t498.env, NULL, 0);
                _t500 = gem_eq(gem_table_get(_t499, gem_string("type")), gem_string("/"));
        }
        GemVal _t503;
        if (gem_truthy(_t500)) {
                _t503 = _t500;
        } else {
                GemVal _t501 = (*gem_v_peek);
                GemVal _t502 = _t501.fn(_t501.env, NULL, 0);
                _t503 = gem_eq(gem_table_get(_t502, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t503)) break;
#line 333 "compiler/main.gem"
    GemVal _t504 = (*gem_v_advance);
    GemVal _t505 = _t504.fn(_t504.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t505, gem_string("type"));
#line 334 "compiler/main.gem"
    GemVal _t506 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t506.fn(_t506.env, NULL, 0);
#line 335 "compiler/main.gem"
    GemVal _t507[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t507, 3);
    }
    GemVal _t508 = gem_v_left;
    gem_pop_frame();
    return _t508;
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
    gem_push_frame("_anon_14", "compiler/main.gem", 0);
#line 342 "compiler/main.gem"
    GemVal _t510 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t510.fn(_t510.env, NULL, 0);
#line 343 "compiler/main.gem"
    while (1) {
        GemVal _t511 = (*gem_v_peek);
        GemVal _t512 = _t511.fn(_t511.env, NULL, 0);
        GemVal _t515;
        if (gem_truthy(gem_eq(gem_table_get(_t512, gem_string("type")), gem_string("+")))) {
                _t515 = gem_eq(gem_table_get(_t512, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t513 = (*gem_v_peek);
                GemVal _t514 = _t513.fn(_t513.env, NULL, 0);
                _t515 = gem_eq(gem_table_get(_t514, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t515)) break;
#line 344 "compiler/main.gem"
    GemVal _t516 = (*gem_v_advance);
    GemVal _t517 = _t516.fn(_t516.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t517, gem_string("type"));
#line 345 "compiler/main.gem"
    GemVal _t518 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t518.fn(_t518.env, NULL, 0);
#line 346 "compiler/main.gem"
    GemVal _t519[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t519, 3);
    }
    GemVal _t520 = gem_v_left;
    gem_pop_frame();
    return _t520;
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
    gem_push_frame("_anon_15", "compiler/main.gem", 0);
#line 353 "compiler/main.gem"
    GemVal _t522 = (*gem_v_parse_add);
    GemVal gem_v_left = _t522.fn(_t522.env, NULL, 0);
#line 354 "compiler/main.gem"
    while (1) {
        GemVal _t523 = (*gem_v_peek);
        GemVal _t524 = _t523.fn(_t523.env, NULL, 0);
        GemVal _t527;
        if (gem_truthy(gem_eq(gem_table_get(_t524, gem_string("type")), gem_string("==")))) {
                _t527 = gem_eq(gem_table_get(_t524, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t525 = (*gem_v_peek);
                GemVal _t526 = _t525.fn(_t525.env, NULL, 0);
                _t527 = gem_eq(gem_table_get(_t526, gem_string("type")), gem_string("!="));
        }
        GemVal _t530;
        if (gem_truthy(_t527)) {
                _t530 = _t527;
        } else {
                GemVal _t528 = (*gem_v_peek);
                GemVal _t529 = _t528.fn(_t528.env, NULL, 0);
                _t530 = gem_eq(gem_table_get(_t529, gem_string("type")), gem_string("<"));
        }
        GemVal _t533;
        if (gem_truthy(_t530)) {
                _t533 = _t530;
        } else {
                GemVal _t531 = (*gem_v_peek);
                GemVal _t532 = _t531.fn(_t531.env, NULL, 0);
                _t533 = gem_eq(gem_table_get(_t532, gem_string("type")), gem_string(">"));
        }
        GemVal _t536;
        if (gem_truthy(_t533)) {
                _t536 = _t533;
        } else {
                GemVal _t534 = (*gem_v_peek);
                GemVal _t535 = _t534.fn(_t534.env, NULL, 0);
                _t536 = gem_eq(gem_table_get(_t535, gem_string("type")), gem_string("<="));
        }
        GemVal _t539;
        if (gem_truthy(_t536)) {
                _t539 = _t536;
        } else {
                GemVal _t537 = (*gem_v_peek);
                GemVal _t538 = _t537.fn(_t537.env, NULL, 0);
                _t539 = gem_eq(gem_table_get(_t538, gem_string("type")), gem_string(">="));
        }
        if (!gem_truthy(_t539)) break;
#line 355 "compiler/main.gem"
    GemVal _t540 = (*gem_v_advance);
    GemVal _t541 = _t540.fn(_t540.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t541, gem_string("type"));
#line 356 "compiler/main.gem"
    GemVal _t542 = (*gem_v_parse_add);
        GemVal gem_v_right = _t542.fn(_t542.env, NULL, 0);
#line 357 "compiler/main.gem"
    GemVal _t543[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t543, 3);
    }
    GemVal _t544 = gem_v_left;
    gem_pop_frame();
    return _t544;
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
    gem_push_frame("_anon_16", "compiler/main.gem", 0);
#line 364 "compiler/main.gem"
    GemVal _t546 = (*gem_v_peek);
    GemVal _t547 = _t546.fn(_t546.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t547, gem_string("type")), gem_string("not")))) {
#line 365 "compiler/main.gem"
    GemVal _t548 = (*gem_v_advance);
        (void)(_t548.fn(_t548.env, NULL, 0));
#line 366 "compiler/main.gem"
    GemVal _t549 = (*gem_v_parse_not);
    GemVal _t550[] = {gem_string("not"), _t549.fn(_t549.env, NULL, 0)};
        GemVal _t551 = gem_fn_make_unop(NULL, _t550, 2);
        gem_pop_frame();
        return _t551;
    }
#line 368 "compiler/main.gem"
    GemVal _t552 = (*gem_v_parse_compare);
    GemVal _t553 = _t552.fn(_t552.env, NULL, 0);
    gem_pop_frame();
    return _t553;
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
    gem_push_frame("_anon_17", "compiler/main.gem", 0);
#line 373 "compiler/main.gem"
    GemVal _t555 = (*gem_v_parse_not);
    GemVal gem_v_left = _t555.fn(_t555.env, NULL, 0);
#line 374 "compiler/main.gem"
    while (1) {
        GemVal _t556 = (*gem_v_peek);
        GemVal _t557 = _t556.fn(_t556.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t557, gem_string("type")), gem_string("and")))) break;
#line 375 "compiler/main.gem"
    GemVal _t558 = (*gem_v_advance);
        (void)(_t558.fn(_t558.env, NULL, 0));
#line 376 "compiler/main.gem"
    GemVal _t559 = (*gem_v_parse_not);
        GemVal gem_v_right = _t559.fn(_t559.env, NULL, 0);
#line 377 "compiler/main.gem"
    GemVal _t560[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t560, 3);
    }
    GemVal _t561 = gem_v_left;
    gem_pop_frame();
    return _t561;
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
    gem_push_frame("_anon_18", "compiler/main.gem", 0);
#line 384 "compiler/main.gem"
    GemVal _t563 = (*gem_v_parse_and);
    GemVal gem_v_left = _t563.fn(_t563.env, NULL, 0);
#line 385 "compiler/main.gem"
    while (1) {
        GemVal _t564 = (*gem_v_peek);
        GemVal _t565 = _t564.fn(_t564.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t565, gem_string("type")), gem_string("or")))) break;
#line 386 "compiler/main.gem"
    GemVal _t566 = (*gem_v_advance);
        (void)(_t566.fn(_t566.env, NULL, 0));
#line 387 "compiler/main.gem"
    GemVal _t567 = (*gem_v_parse_and);
        GemVal gem_v_right = _t567.fn(_t567.env, NULL, 0);
#line 388 "compiler/main.gem"
    GemVal _t568[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t568, 3);
    }
    GemVal _t569 = gem_v_left;
    gem_pop_frame();
    return _t569;
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
    gem_push_frame("_anon_19", "compiler/main.gem", 0);
#line 395 "compiler/main.gem"
    GemVal _t571 = (*gem_v_peek);
    GemVal _t572 = _t571.fn(_t571.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t572, gem_string("line"));
#line 396 "compiler/main.gem"
    GemVal _t573 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t573.fn(_t573.env, NULL, 0);
#line 399 "compiler/main.gem"
    GemVal _t574 = (*gem_v_peek);
    GemVal _t575 = _t574.fn(_t574.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t575, gem_string("type")), gem_string("=")))) {
#line 400 "compiler/main.gem"
    GemVal _t576 = (*gem_v_advance);
        (void)(_t576.fn(_t576.env, NULL, 0));
#line 401 "compiler/main.gem"
    GemVal _t577 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t577.fn(_t577.env, NULL, 0);
#line 402 "compiler/main.gem"
    GemVal _t578 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t578, gem_string("tag")), gem_string("var")))) {
#line 403 "compiler/main.gem"
    GemVal _t579 = gem_v_lhs;
    GemVal _t580[] = {gem_table_get(_t579, gem_string("name")), gem_v_value, gem_v_start_line};
            GemVal _t581 = gem_fn_make_assign(NULL, _t580, 3);
            gem_pop_frame();
            return _t581;
        }
#line 405 "compiler/main.gem"
    GemVal _t582 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t582, gem_string("tag")), gem_string("dot")))) {
#line 406 "compiler/main.gem"
    GemVal _t583 = gem_v_lhs;
    GemVal _t584 = gem_v_lhs;
    GemVal _t585[] = {gem_table_get(_t583, gem_string("object")), gem_table_get(_t584, gem_string("field")), gem_v_value, gem_v_start_line};
            GemVal _t586 = gem_fn_make_dot_assign(NULL, _t585, 4);
            gem_pop_frame();
            return _t586;
        }
#line 408 "compiler/main.gem"
    GemVal _t587 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t587, gem_string("tag")), gem_string("index")))) {
#line 409 "compiler/main.gem"
    GemVal _t588 = gem_v_lhs;
    GemVal _t589 = gem_v_lhs;
    GemVal _t590[] = {gem_table_get(_t588, gem_string("object")), gem_table_get(_t589, gem_string("key")), gem_v_value, gem_v_start_line};
            GemVal _t591 = gem_fn_make_index_assign(NULL, _t590, 4);
            gem_pop_frame();
            return _t591;
        }
#line 411 "compiler/main.gem"
    GemVal _t592 = (*gem_v_peek);
    GemVal _t593 = _t592.fn(_t592.env, NULL, 0);
    GemVal _t594[] = {gem_table_get(_t593, gem_string("line"))};
    GemVal _t595[] = {gem_add(gem_string("invalid assignment target at line "), gem_to_string_fn(NULL, _t594, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 411, _t595, 1));
    }
#line 415 "compiler/main.gem"
    GemVal _t596 = (*gem_v_peek);
    GemVal _t597 = _t596.fn(_t596.env, NULL, 0);
    GemVal _t600;
    if (gem_truthy(gem_eq(gem_table_get(_t597, gem_string("type")), gem_string("+=")))) {
        _t600 = gem_eq(gem_table_get(_t597, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t598 = (*gem_v_peek);
        GemVal _t599 = _t598.fn(_t598.env, NULL, 0);
        _t600 = gem_eq(gem_table_get(_t599, gem_string("type")), gem_string("-="));
    }
    GemVal _t603;
    if (gem_truthy(_t600)) {
        _t603 = _t600;
    } else {
        GemVal _t601 = (*gem_v_peek);
        GemVal _t602 = _t601.fn(_t601.env, NULL, 0);
        _t603 = gem_eq(gem_table_get(_t602, gem_string("type")), gem_string("*="));
    }
    GemVal _t606;
    if (gem_truthy(_t603)) {
        _t606 = _t603;
    } else {
        GemVal _t604 = (*gem_v_peek);
        GemVal _t605 = _t604.fn(_t604.env, NULL, 0);
        _t606 = gem_eq(gem_table_get(_t605, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t606)) {
#line 416 "compiler/main.gem"
    GemVal _t607 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t607.fn(_t607.env, NULL, 0);
#line 417 "compiler/main.gem"
    GemVal _t608 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t608, gem_string("value")), gem_int(0));
#line 418 "compiler/main.gem"
    GemVal _t609 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t609.fn(_t609.env, NULL, 0);
#line 419 "compiler/main.gem"
    GemVal _t610 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t610, gem_string("tag")), gem_string("var")))) {
#line 420 "compiler/main.gem"
    GemVal _t611 = gem_v_op_tok;
    GemVal _t612[] = {gem_table_get(_t611, gem_string("line"))};
    GemVal _t613[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t612, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 420, _t613, 1));
        }
#line 422 "compiler/main.gem"
    GemVal _t614 = gem_v_lhs;
    GemVal _t615 = gem_v_lhs;
    GemVal _t616[] = {gem_table_get(_t615, gem_string("name"))};
    GemVal _t617[] = {gem_v_base_op, gem_fn_make_var(NULL, _t616, 1), gem_v_value};
    GemVal _t618[] = {gem_table_get(_t614, gem_string("name")), gem_fn_make_binop(NULL, _t617, 3), gem_v_start_line};
        GemVal _t619 = gem_fn_make_assign(NULL, _t618, 3);
        gem_pop_frame();
        return _t619;
    }
    GemVal _t620 = gem_v_lhs;
    gem_pop_frame();
    return _t620;
}

struct _closure__anon_20 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_fn_depth;
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
    GemVal *gem_v_fn_depth = _cls->gem_v_fn_depth;
    GemVal *gem_v_gensym_counter = _cls->gem_v_gensym_counter;
    GemVal *gem_v_parse_body = _cls->gem_v_parse_body;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body = _cls->gem_v_parse_fn_body;
    GemVal *gem_v_parse_params = _cls->gem_v_parse_params;
    GemVal *gem_v_parse_stmt = _cls->gem_v_parse_stmt;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_peek_at = _cls->gem_v_peek_at;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    gem_push_frame("_anon_20", "compiler/main.gem", 0);
#line 434 "compiler/main.gem"
    GemVal _t622 = (*gem_v_peek);
    GemVal gem_v_t = _t622.fn(_t622.env, NULL, 0);
#line 437 "compiler/main.gem"
    GemVal _t623 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t623, gem_string("type")), gem_string("let")))) {
#line 438 "compiler/main.gem"
    GemVal _t624 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t624, gem_string("line"));
#line 439 "compiler/main.gem"
    GemVal _t625 = (*gem_v_advance);
        (void)(_t625.fn(_t625.env, NULL, 0));
#line 440 "compiler/main.gem"
    GemVal _t626 = (*gem_v_expect);
    GemVal _t627[] = {gem_string("NAME")};
    GemVal _t628 = _t626.fn(_t626.env, _t627, 1);
        GemVal gem_v_name = gem_table_get(_t628, gem_string("value"));
#line 441 "compiler/main.gem"
    GemVal _t629 = (*gem_v_expect);
    GemVal _t630[] = {gem_string("=")};
        (void)(_t629.fn(_t629.env, _t630, 1));
#line 442 "compiler/main.gem"
    GemVal _t631 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t631.fn(_t631.env, NULL, 0);
#line 443 "compiler/main.gem"
    GemVal _t632[] = {gem_v_name, gem_v_value, gem_v_line};
        GemVal _t633 = gem_fn_make_let(NULL, _t632, 3);
        gem_pop_frame();
        return _t633;
    }
#line 447 "compiler/main.gem"
    GemVal _t634 = gem_v_t;
    GemVal _t638;
    if (!gem_truthy(gem_eq(gem_table_get(_t634, gem_string("type")), gem_string("fn")))) {
        _t638 = gem_eq(gem_table_get(_t634, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t635 = (*gem_v_peek_at);
        GemVal _t636[] = {gem_int(1)};
        GemVal _t637 = _t635.fn(_t635.env, _t636, 1);
        _t638 = gem_eq(gem_table_get(_t637, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t638)) {
#line 448 "compiler/main.gem"
    GemVal _t639 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t639, gem_string("line"));
#line 449 "compiler/main.gem"
    GemVal _t640 = (*gem_v_advance);
        (void)(_t640.fn(_t640.env, NULL, 0));
#line 450 "compiler/main.gem"
    GemVal _t641 = (*gem_v_expect);
    GemVal _t642[] = {gem_string("NAME")};
    GemVal _t643 = _t641.fn(_t641.env, _t642, 1);
        GemVal gem_v_name = gem_table_get(_t643, gem_string("value"));
#line 451 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 452 "compiler/main.gem"
    GemVal _t644[] = {gem_v_name};
    GemVal _t645[] = {gem_v_line};
    GemVal _t646[] = {gem_v_name};
    GemVal _t647[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("named fn '"), gem_to_string_fn(NULL, _t644, 1)), gem_string("' inside function body is not supported at line ")), gem_to_string_fn(NULL, _t645, 1)), gem_string(" — use: let ")), gem_to_string_fn(NULL, _t646, 1)), gem_string(" = fn(...) ... end"))};
            (void)(gem_error_at_fn("compiler/main.gem", 452, _t647, 1));
        }
#line 454 "compiler/main.gem"
    GemVal _t648 = (*gem_v_expect);
    GemVal _t649[] = {gem_string("(")};
        (void)(_t648.fn(_t648.env, _t649, 1));
#line 455 "compiler/main.gem"
    GemVal _t650 = (*gem_v_parse_params);
        GemVal gem_v_params = _t650.fn(_t650.env, NULL, 0);
#line 456 "compiler/main.gem"
    GemVal _t651 = (*gem_v_expect);
    GemVal _t652[] = {gem_string(")")};
        (void)(_t651.fn(_t651.env, _t652, 1));
#line 457 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 458 "compiler/main.gem"
    GemVal _t653 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t653.fn(_t653.env, NULL, 0);
#line 459 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 460 "compiler/main.gem"
    GemVal _t654 = (*gem_v_expect);
    GemVal _t655[] = {gem_string("end")};
        (void)(_t654.fn(_t654.env, _t655, 1));
#line 461 "compiler/main.gem"
    GemVal _t656[] = {gem_v_name, gem_v_params, gem_v_body, gem_v_line};
        GemVal _t657 = gem_fn_make_fn_def(NULL, _t656, 4);
        gem_pop_frame();
        return _t657;
    }
#line 465 "compiler/main.gem"
    GemVal _t658 = gem_v_t;
    GemVal _t660;
    if (gem_truthy(gem_eq(gem_table_get(_t658, gem_string("type")), gem_string("if")))) {
        _t660 = gem_eq(gem_table_get(_t658, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t659 = gem_v_t;
        _t660 = gem_eq(gem_table_get(_t659, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t660)) {
#line 466 "compiler/main.gem"
    GemVal _t661 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t661, gem_string("line"));
#line 467 "compiler/main.gem"
    GemVal _t662 = (*gem_v_advance);
        (void)(_t662.fn(_t662.env, NULL, 0));
#line 468 "compiler/main.gem"
    GemVal _t663 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t663.fn(_t663.env, NULL, 0);
#line 469 "compiler/main.gem"
    GemVal _t664 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t664.fn(_t664.env, NULL, 0);
#line 470 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 471 "compiler/main.gem"
    GemVal _t665 = (*gem_v_peek);
    GemVal _t666 = _t665.fn(_t665.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t666, gem_string("type")), gem_string("elif")))) {
#line 473 "compiler/main.gem"
    GemVal _t667 = gem_table_new();
    GemVal _t668 = (*gem_v_parse_stmt);
    gem_table_set(_t667, gem_int(0), _t668.fn(_t668.env, NULL, 0));
            gem_v_else_body = _t667;
        } else {
#line 474 "compiler/main.gem"
    GemVal _t669 = (*gem_v_peek);
    GemVal _t670 = _t669.fn(_t669.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t670, gem_string("type")), gem_string("else")))) {
#line 475 "compiler/main.gem"
    GemVal _t671 = (*gem_v_advance);
                (void)(_t671.fn(_t671.env, NULL, 0));
#line 476 "compiler/main.gem"
    GemVal _t672 = (*gem_v_parse_body);
                gem_v_else_body = _t672.fn(_t672.env, NULL, 0);
            }
        }
#line 479 "compiler/main.gem"
    GemVal _t673 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t673, gem_string("type")), gem_string("if")))) {
#line 480 "compiler/main.gem"
    GemVal _t674 = (*gem_v_expect);
    GemVal _t675[] = {gem_string("end")};
            (void)(_t674.fn(_t674.env, _t675, 1));
        }
#line 482 "compiler/main.gem"
    GemVal _t676[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        GemVal _t677 = gem_fn_make_if(NULL, _t676, 4);
        gem_pop_frame();
        return _t677;
    }
#line 486 "compiler/main.gem"
    GemVal _t678 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t678, gem_string("type")), gem_string("while")))) {
#line 487 "compiler/main.gem"
    GemVal _t679 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t679, gem_string("line"));
#line 488 "compiler/main.gem"
    GemVal _t680 = (*gem_v_advance);
        (void)(_t680.fn(_t680.env, NULL, 0));
#line 489 "compiler/main.gem"
    GemVal _t681 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t681.fn(_t681.env, NULL, 0);
#line 490 "compiler/main.gem"
    GemVal _t682 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t682.fn(_t682.env, NULL, 0);
#line 491 "compiler/main.gem"
    GemVal _t683 = (*gem_v_expect);
    GemVal _t684[] = {gem_string("end")};
        (void)(_t683.fn(_t683.env, _t684, 1));
#line 492 "compiler/main.gem"
    GemVal _t685[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        GemVal _t686 = gem_fn_make_while(NULL, _t685, 3);
        gem_pop_frame();
        return _t686;
    }
#line 496 "compiler/main.gem"
    GemVal _t687 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t687, gem_string("type")), gem_string("for")))) {
#line 497 "compiler/main.gem"
    GemVal _t688 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t688, gem_string("line"));
#line 498 "compiler/main.gem"
    GemVal _t689 = (*gem_v_advance);
        (void)(_t689.fn(_t689.env, NULL, 0));
#line 499 "compiler/main.gem"
    GemVal _t690 = (*gem_v_expect);
    GemVal _t691[] = {gem_string("NAME")};
    GemVal _t692 = _t690.fn(_t690.env, _t691, 1);
        GemVal gem_v_var_name = gem_table_get(_t692, gem_string("value"));
#line 501 "compiler/main.gem"
    GemVal _t693 = (*gem_v_peek);
    GemVal _t694 = _t693.fn(_t693.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t694, gem_string("type")), gem_string("in")))) {
#line 503 "compiler/main.gem"
    GemVal _t695 = (*gem_v_advance);
            (void)(_t695.fn(_t695.env, NULL, 0));
#line 504 "compiler/main.gem"
    GemVal _t696 = (*gem_v_parse_expr);
            GemVal gem_v_items_expr = _t696.fn(_t696.env, NULL, 0);
#line 505 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 506 "compiler/main.gem"
    GemVal _t697[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t697, 1));
#line 507 "compiler/main.gem"
    GemVal _t698[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t698, 1));
#line 508 "compiler/main.gem"
    GemVal _t699 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t699.fn(_t699.env, NULL, 0);
#line 509 "compiler/main.gem"
    GemVal _t700 = (*gem_v_expect);
    GemVal _t701[] = {gem_string("end")};
            (void)(_t700.fn(_t700.env, _t701, 1));
#line 510 "compiler/main.gem"
    GemVal _t702 = gem_table_new();
            GemVal gem_v_inner_stmts = _t702;
#line 511 "compiler/main.gem"
    GemVal _t703[] = {gem_v_items_var};
    GemVal _t704[] = {gem_v_idx_var};
    GemVal _t705[] = {gem_fn_make_var(NULL, _t703, 1), gem_fn_make_var(NULL, _t704, 1)};
    GemVal _t706[] = {gem_v_var_name, gem_fn_make_index(NULL, _t705, 2), gem_v_line};
    GemVal _t707[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t706, 3)};
            (void)(gem_push_fn(NULL, _t707, 2));
#line 512 "compiler/main.gem"
    GemVal _t708[] = {gem_v_idx_var};
    GemVal _t709[] = {gem_int(1)};
    GemVal _t710[] = {gem_string("+"), gem_fn_make_var(NULL, _t708, 1), gem_fn_make_int(NULL, _t709, 1)};
    GemVal _t711[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t710, 3), gem_v_line};
    GemVal _t712[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t711, 3)};
            (void)(gem_push_fn(NULL, _t712, 2));
            {
#line 513 "compiler/main.gem"
                GemVal gem_v__for_i_1 = gem_int(0);
#line 513 "compiler/main.gem"
    GemVal _t713[] = {gem_v_fbody};
                GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t713, 1);
#line 513 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 513 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_1;
#line 513 "compiler/main.gem"
                    gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 514 "compiler/main.gem"
    GemVal _t714[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t714, 2));
                }
            }
#line 516 "compiler/main.gem"
    GemVal _t715[] = {gem_v_idx_var};
    GemVal _t716[] = {gem_string("len")};
    GemVal _t717 = gem_table_new();
    GemVal _t718[] = {gem_v_items_var};
    gem_table_set(_t717, gem_int(0), gem_fn_make_var(NULL, _t718, 1));
    GemVal _t719[] = {gem_fn_make_var(NULL, _t716, 1), _t717, gem_int(0)};
    GemVal _t720[] = {gem_string("<"), gem_fn_make_var(NULL, _t715, 1), gem_fn_make_call(NULL, _t719, 3)};
    GemVal _t721[] = {gem_fn_make_binop(NULL, _t720, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t721, 3);
#line 521 "compiler/main.gem"
    GemVal _t722 = gem_table_new();
    gem_table_set(_t722, gem_string("tag"), gem_string("block"));
    GemVal _t723 = gem_table_new();
    GemVal _t724[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t723, gem_int(0), gem_fn_make_let(NULL, _t724, 3));
    GemVal _t725[] = {gem_int(0)};
    GemVal _t726[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t725, 1), gem_v_line};
    gem_table_set(_t723, gem_int(1), gem_fn_make_let(NULL, _t726, 3));
    gem_table_set(_t723, gem_int(2), gem_v_while_node);
    gem_table_set(_t722, gem_string("stmts"), _t723);
            GemVal _t727 = _t722;
            gem_pop_frame();
            return _t727;
        } else {
#line 526 "compiler/main.gem"
    GemVal _t728 = (*gem_v_peek);
    GemVal _t729 = _t728.fn(_t728.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t729, gem_string("type")), gem_string("=")))) {
#line 528 "compiler/main.gem"
    GemVal _t730 = (*gem_v_advance);
                (void)(_t730.fn(_t730.env, NULL, 0));
#line 529 "compiler/main.gem"
    GemVal _t731 = (*gem_v_parse_expr);
                GemVal gem_v_start_expr = _t731.fn(_t731.env, NULL, 0);
#line 530 "compiler/main.gem"
    GemVal _t732 = (*gem_v_expect);
    GemVal _t733[] = {gem_string(",")};
                (void)(_t732.fn(_t732.env, _t733, 1));
#line 531 "compiler/main.gem"
    GemVal _t734 = (*gem_v_parse_expr);
                GemVal gem_v_end_expr = _t734.fn(_t734.env, NULL, 0);
#line 532 "compiler/main.gem"
    GemVal _t735 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t735.fn(_t735.env, NULL, 0);
#line 533 "compiler/main.gem"
    GemVal _t736 = (*gem_v_expect);
    GemVal _t737[] = {gem_string("end")};
                (void)(_t736.fn(_t736.env, _t737, 1));
#line 534 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 535 "compiler/main.gem"
    GemVal _t738[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t738, 1));
#line 536 "compiler/main.gem"
    GemVal _t739[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t739, 1));
#line 537 "compiler/main.gem"
    GemVal _t740 = gem_table_new();
                GemVal gem_v_inner_stmts = _t740;
#line 538 "compiler/main.gem"
    GemVal _t741[] = {gem_v_idx_var};
    GemVal _t742[] = {gem_v_var_name, gem_fn_make_var(NULL, _t741, 1), gem_v_line};
    GemVal _t743[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t742, 3)};
                (void)(gem_push_fn(NULL, _t743, 2));
#line 539 "compiler/main.gem"
    GemVal _t744[] = {gem_v_idx_var};
    GemVal _t745[] = {gem_int(1)};
    GemVal _t746[] = {gem_string("+"), gem_fn_make_var(NULL, _t744, 1), gem_fn_make_int(NULL, _t745, 1)};
    GemVal _t747[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t746, 3), gem_v_line};
    GemVal _t748[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t747, 3)};
                (void)(gem_push_fn(NULL, _t748, 2));
                {
#line 540 "compiler/main.gem"
                    GemVal gem_v__for_i_2 = gem_int(0);
#line 540 "compiler/main.gem"
    GemVal _t749[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t749, 1);
#line 540 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 540 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_2;
#line 540 "compiler/main.gem"
                        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 541 "compiler/main.gem"
    GemVal _t750[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t750, 2));
                    }
                }
#line 543 "compiler/main.gem"
    GemVal _t751[] = {gem_v_idx_var};
    GemVal _t752[] = {gem_v_limit_var};
    GemVal _t753[] = {gem_string("<"), gem_fn_make_var(NULL, _t751, 1), gem_fn_make_var(NULL, _t752, 1)};
    GemVal _t754[] = {gem_fn_make_binop(NULL, _t753, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t754, 3);
#line 548 "compiler/main.gem"
    GemVal _t755 = gem_table_new();
    gem_table_set(_t755, gem_string("tag"), gem_string("block"));
    GemVal _t756 = gem_table_new();
    GemVal _t757[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t756, gem_int(0), gem_fn_make_let(NULL, _t757, 3));
    GemVal _t758[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t756, gem_int(1), gem_fn_make_let(NULL, _t758, 3));
    gem_table_set(_t756, gem_int(2), gem_v_while_node);
    gem_table_set(_t755, gem_string("stmts"), _t756);
                GemVal _t759 = _t755;
                gem_pop_frame();
                return _t759;
            } else {
#line 554 "compiler/main.gem"
    GemVal _t760 = (*gem_v_peek);
    GemVal _t761 = _t760.fn(_t760.env, NULL, 0);
    GemVal _t762[] = {gem_table_get(_t761, gem_string("line"))};
    GemVal _t763[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t762, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 554, _t763, 1));
            }
        }
    }
#line 559 "compiler/main.gem"
    GemVal _t764 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t764, gem_string("type")), gem_string("match")))) {
#line 560 "compiler/main.gem"
    GemVal _t765 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t765, gem_string("line"));
#line 561 "compiler/main.gem"
    GemVal _t766 = (*gem_v_advance);
        (void)(_t766.fn(_t766.env, NULL, 0));
#line 562 "compiler/main.gem"
    GemVal _t767 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t767.fn(_t767.env, NULL, 0);
#line 563 "compiler/main.gem"
    GemVal _t768 = (*gem_v_skip_nl);
        (void)(_t768.fn(_t768.env, NULL, 0));
#line 564 "compiler/main.gem"
    GemVal _t769 = gem_table_new();
        GemVal gem_v_whens = _t769;
#line 565 "compiler/main.gem"
        while (1) {
            GemVal _t770 = (*gem_v_peek);
            GemVal _t771 = _t770.fn(_t770.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t771, gem_string("type")), gem_string("when")))) break;
#line 566 "compiler/main.gem"
    GemVal _t772 = (*gem_v_advance);
            (void)(_t772.fn(_t772.env, NULL, 0));
#line 567 "compiler/main.gem"
    GemVal _t773 = (*gem_v_parse_expr);
            GemVal gem_v_wval = _t773.fn(_t773.env, NULL, 0);
#line 568 "compiler/main.gem"
    GemVal _t774 = (*gem_v_parse_body);
            GemVal gem_v_wbody = _t774.fn(_t774.env, NULL, 0);
#line 569 "compiler/main.gem"
    GemVal _t775[] = {gem_v_wval, gem_v_wbody};
    GemVal _t776[] = {gem_v_whens, gem_fn_make_when(NULL, _t775, 2)};
            (void)(gem_push_fn(NULL, _t776, 2));
#line 570 "compiler/main.gem"
    GemVal _t777 = (*gem_v_skip_nl);
            (void)(_t777.fn(_t777.env, NULL, 0));
        }
#line 572 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 573 "compiler/main.gem"
    GemVal _t778 = (*gem_v_peek);
    GemVal _t779 = _t778.fn(_t778.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t779, gem_string("type")), gem_string("else")))) {
#line 574 "compiler/main.gem"
    GemVal _t780 = (*gem_v_advance);
            (void)(_t780.fn(_t780.env, NULL, 0));
#line 575 "compiler/main.gem"
    GemVal _t781 = (*gem_v_parse_body);
            gem_v_else_body = _t781.fn(_t781.env, NULL, 0);
        }
#line 577 "compiler/main.gem"
    GemVal _t782 = (*gem_v_expect);
    GemVal _t783[] = {gem_string("end")};
        (void)(_t782.fn(_t782.env, _t783, 1));
#line 578 "compiler/main.gem"
    GemVal _t784[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line};
        GemVal _t785 = gem_fn_make_match(NULL, _t784, 4);
        gem_pop_frame();
        return _t785;
    }
#line 582 "compiler/main.gem"
    GemVal _t786 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t786, gem_string("type")), gem_string("return")))) {
#line 583 "compiler/main.gem"
    GemVal _t787 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t787, gem_string("line"));
#line 584 "compiler/main.gem"
    GemVal _t788 = (*gem_v_advance);
        (void)(_t788.fn(_t788.env, NULL, 0));
#line 585 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 586 "compiler/main.gem"
    GemVal _t789 = (*gem_v_peek);
    GemVal _t790 = _t789.fn(_t789.env, NULL, 0);
    GemVal _t793;
    if (!gem_truthy(gem_neq(gem_table_get(_t790, gem_string("type")), gem_string("NEWLINE")))) {
        _t793 = gem_neq(gem_table_get(_t790, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t791 = (*gem_v_peek);
        GemVal _t792 = _t791.fn(_t791.env, NULL, 0);
        _t793 = gem_neq(gem_table_get(_t792, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t796;
    if (!gem_truthy(_t793)) {
        _t796 = _t793;
    } else {
        GemVal _t794 = (*gem_v_peek);
        GemVal _t795 = _t794.fn(_t794.env, NULL, 0);
        _t796 = gem_neq(gem_table_get(_t795, gem_string("type")), gem_string("end"));
    }
    GemVal _t799;
    if (!gem_truthy(_t796)) {
        _t799 = _t796;
    } else {
        GemVal _t797 = (*gem_v_peek);
        GemVal _t798 = _t797.fn(_t797.env, NULL, 0);
        _t799 = gem_neq(gem_table_get(_t798, gem_string("type")), gem_string("elif"));
    }
    GemVal _t802;
    if (!gem_truthy(_t799)) {
        _t802 = _t799;
    } else {
        GemVal _t800 = (*gem_v_peek);
        GemVal _t801 = _t800.fn(_t800.env, NULL, 0);
        _t802 = gem_neq(gem_table_get(_t801, gem_string("type")), gem_string("else"));
    }
    GemVal _t805;
    if (!gem_truthy(_t802)) {
        _t805 = _t802;
    } else {
        GemVal _t803 = (*gem_v_peek);
        GemVal _t804 = _t803.fn(_t803.env, NULL, 0);
        _t805 = gem_neq(gem_table_get(_t804, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t805)) {
#line 587 "compiler/main.gem"
    GemVal _t806 = (*gem_v_parse_expr);
            gem_v_value = _t806.fn(_t806.env, NULL, 0);
        }
#line 589 "compiler/main.gem"
    GemVal _t807[] = {gem_v_value, gem_v_line};
        GemVal _t808 = gem_fn_make_return(NULL, _t807, 2);
        gem_pop_frame();
        return _t808;
    }
#line 593 "compiler/main.gem"
    GemVal _t809 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t809, gem_string("type")), gem_string("break")))) {
#line 594 "compiler/main.gem"
    GemVal _t810 = (*gem_v_advance);
        (void)(_t810.fn(_t810.env, NULL, 0));
#line 595 "compiler/main.gem"
        GemVal _t811 = gem_fn_make_break(NULL, NULL, 0);
        gem_pop_frame();
        return _t811;
    }
#line 599 "compiler/main.gem"
    GemVal _t812 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t812, gem_string("type")), gem_string("continue")))) {
#line 600 "compiler/main.gem"
    GemVal _t813 = (*gem_v_advance);
        (void)(_t813.fn(_t813.env, NULL, 0));
#line 601 "compiler/main.gem"
        GemVal _t814 = gem_fn_make_continue(NULL, NULL, 0);
        gem_pop_frame();
        return _t814;
    }
#line 605 "compiler/main.gem"
    GemVal _t815 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t815, gem_string("type")), gem_string("load")))) {
#line 606 "compiler/main.gem"
    GemVal _t816 = (*gem_v_advance);
        (void)(_t816.fn(_t816.env, NULL, 0));
#line 607 "compiler/main.gem"
    GemVal _t817 = (*gem_v_expect);
    GemVal _t818[] = {gem_string("STRING")};
    GemVal _t819 = _t817.fn(_t817.env, _t818, 1);
        GemVal gem_v_path = gem_table_get(_t819, gem_string("value"));
#line 608 "compiler/main.gem"
    GemVal _t820[] = {gem_v_path};
        GemVal _t821 = gem_fn_make_load(NULL, _t820, 1);
        gem_pop_frame();
        return _t821;
    }
#line 612 "compiler/main.gem"
    GemVal _t822 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t822, gem_string("type")), gem_string("extern")))) {
#line 613 "compiler/main.gem"
    GemVal _t823 = (*gem_v_advance);
        (void)(_t823.fn(_t823.env, NULL, 0));
#line 614 "compiler/main.gem"
    GemVal _t824 = (*gem_v_peek);
    GemVal _t825 = _t824.fn(_t824.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t825, gem_string("type")), gem_string("fn")))) {
#line 615 "compiler/main.gem"
    GemVal _t826 = (*gem_v_advance);
            (void)(_t826.fn(_t826.env, NULL, 0));
#line 616 "compiler/main.gem"
    GemVal _t827 = (*gem_v_expect);
    GemVal _t828[] = {gem_string("NAME")};
    GemVal _t829 = _t827.fn(_t827.env, _t828, 1);
            GemVal gem_v_name = gem_table_get(_t829, gem_string("value"));
#line 617 "compiler/main.gem"
    GemVal _t830 = (*gem_v_expect);
    GemVal _t831[] = {gem_string("(")};
            (void)(_t830.fn(_t830.env, _t831, 1));
#line 618 "compiler/main.gem"
    GemVal _t832 = gem_table_new();
            GemVal gem_v_eparams = _t832;
#line 619 "compiler/main.gem"
    GemVal _t833 = (*gem_v_peek);
    GemVal _t834 = _t833.fn(_t833.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t834, gem_string("type")), gem_string(")")))) {
#line 620 "compiler/main.gem"
    GemVal _t835 = (*gem_v_expect);
    GemVal _t836[] = {gem_string("NAME")};
    GemVal _t837 = _t835.fn(_t835.env, _t836, 1);
                GemVal gem_v_pname = gem_table_get(_t837, gem_string("value"));
#line 621 "compiler/main.gem"
    GemVal _t838 = (*gem_v_expect);
    GemVal _t839[] = {gem_string(":")};
                (void)(_t838.fn(_t838.env, _t839, 1));
#line 622 "compiler/main.gem"
    GemVal _t840 = (*gem_v_expect);
    GemVal _t841[] = {gem_string("NAME")};
    GemVal _t842 = _t840.fn(_t840.env, _t841, 1);
                GemVal gem_v_ptype = gem_table_get(_t842, gem_string("value"));
#line 623 "compiler/main.gem"
    GemVal _t843[] = {gem_v_pname, gem_v_ptype};
    GemVal _t844[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t843, 2)};
                (void)(gem_push_fn(NULL, _t844, 2));
#line 624 "compiler/main.gem"
                while (1) {
                    GemVal _t845 = (*gem_v_peek);
                    GemVal _t846 = _t845.fn(_t845.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t846, gem_string("type")), gem_string(",")))) break;
#line 625 "compiler/main.gem"
    GemVal _t847 = (*gem_v_advance);
                    (void)(_t847.fn(_t847.env, NULL, 0));
#line 626 "compiler/main.gem"
    GemVal _t848 = (*gem_v_expect);
    GemVal _t849[] = {gem_string("NAME")};
    GemVal _t850 = _t848.fn(_t848.env, _t849, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t850, gem_string("value"));
#line 627 "compiler/main.gem"
    GemVal _t851 = (*gem_v_expect);
    GemVal _t852[] = {gem_string(":")};
                    (void)(_t851.fn(_t851.env, _t852, 1));
#line 628 "compiler/main.gem"
    GemVal _t853 = (*gem_v_expect);
    GemVal _t854[] = {gem_string("NAME")};
    GemVal _t855 = _t853.fn(_t853.env, _t854, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t855, gem_string("value"));
#line 629 "compiler/main.gem"
    GemVal _t856[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t857[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t856, 2)};
                    (void)(gem_push_fn(NULL, _t857, 2));
                }
            }
#line 632 "compiler/main.gem"
    GemVal _t858 = (*gem_v_expect);
    GemVal _t859[] = {gem_string(")")};
            (void)(_t858.fn(_t858.env, _t859, 1));
#line 633 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 634 "compiler/main.gem"
    GemVal _t860 = (*gem_v_peek);
    GemVal _t861 = _t860.fn(_t860.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t861, gem_string("type")), gem_string("->")))) {
#line 635 "compiler/main.gem"
    GemVal _t862 = (*gem_v_advance);
                (void)(_t862.fn(_t862.env, NULL, 0));
#line 636 "compiler/main.gem"
    GemVal _t863 = (*gem_v_expect);
    GemVal _t864[] = {gem_string("NAME")};
    GemVal _t865 = _t863.fn(_t863.env, _t864, 1);
                gem_v_ret_type = gem_table_get(_t865, gem_string("value"));
            }
#line 638 "compiler/main.gem"
    GemVal _t866[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            GemVal _t867 = gem_fn_make_extern_fn(NULL, _t866, 3);
            gem_pop_frame();
            return _t867;
        } else {
#line 639 "compiler/main.gem"
    GemVal _t868 = (*gem_v_peek);
    GemVal _t869 = _t868.fn(_t868.env, NULL, 0);
    GemVal _t872;
    if (!gem_truthy(gem_eq(gem_table_get(_t869, gem_string("type")), gem_string("NAME")))) {
        _t872 = gem_eq(gem_table_get(_t869, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t870 = (*gem_v_peek);
        GemVal _t871 = _t870.fn(_t870.env, NULL, 0);
        _t872 = gem_eq(gem_table_get(_t871, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t872)) {
#line 640 "compiler/main.gem"
    GemVal _t873 = (*gem_v_advance);
                (void)(_t873.fn(_t873.env, NULL, 0));
#line 641 "compiler/main.gem"
    GemVal _t874 = (*gem_v_expect);
    GemVal _t875[] = {gem_string("STRING")};
    GemVal _t876 = _t874.fn(_t874.env, _t875, 1);
                GemVal gem_v_path = gem_table_get(_t876, gem_string("value"));
#line 642 "compiler/main.gem"
    GemVal _t877[] = {gem_v_path};
                GemVal _t878 = gem_fn_make_extern_include(NULL, _t877, 1);
                gem_pop_frame();
                return _t878;
            } else {
#line 644 "compiler/main.gem"
    GemVal _t879 = (*gem_v_peek);
    GemVal _t880 = _t879.fn(_t879.env, NULL, 0);
    GemVal _t881[] = {gem_table_get(_t880, gem_string("line"))};
    GemVal _t882[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t881, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 644, _t882, 1));
            }
        }
    }
#line 649 "compiler/main.gem"
    GemVal _t883 = (*gem_v_parse_expr);
    GemVal _t884 = _t883.fn(_t883.env, NULL, 0);
    gem_pop_frame();
    return _t884;
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
    gem_push_frame("_anon_21", "compiler/main.gem", 0);
#line 658 "compiler/main.gem"
    GemVal _t886 = gem_table_new();
    GemVal gem_v_stmts = _t886;
#line 659 "compiler/main.gem"
    GemVal _t887 = (*gem_v_skip_nl);
    (void)(_t887.fn(_t887.env, NULL, 0));
#line 660 "compiler/main.gem"
    while (1) {
        GemVal _t888 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t888.fn(_t888.env, NULL, 0)))) break;
#line 661 "compiler/main.gem"
    GemVal _t889 = (*gem_v_parse_stmt);
    GemVal _t890[] = {gem_v_stmts, _t889.fn(_t889.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t890, 2));
#line 662 "compiler/main.gem"
    GemVal _t891 = (*gem_v_skip_nl);
        (void)(_t891.fn(_t891.env, NULL, 0));
    }
#line 664 "compiler/main.gem"
    GemVal _t892[] = {gem_v_stmts};
    GemVal _t893 = gem_fn_make_program(NULL, _t892, 1);
    gem_pop_frame();
    return _t893;
}

static GemVal gem_fn_make_parser(void *_env, GemVal *args, int argc) {
#line 18 "compiler/main.gem"
    GemVal *gem_v_tokens = GC_MALLOC(sizeof(GemVal));
    *gem_v_tokens = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_parser", "compiler/main.gem", 18);
#line 19 "compiler/main.gem"
    GemVal *gem_v_pos = GC_MALLOC(sizeof(GemVal));
    *gem_v_pos = gem_int(0);
#line 20 "compiler/main.gem"
    GemVal *gem_v_gensym_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_gensym_counter = gem_int(0);
#line 21 "compiler/main.gem"
    GemVal *gem_v_fn_depth = GC_MALLOC(sizeof(GemVal));
    *gem_v_fn_depth = gem_int(0);
#line 25 "compiler/main.gem"
    struct _closure__anon_1 *_t219 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t219->gem_v_pos = gem_v_pos;
    _t219->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t219);
#line 29 "compiler/main.gem"
    struct _closure__anon_2 *_t221 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t221->gem_v_pos = gem_v_pos;
    _t221->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t221);
#line 33 "compiler/main.gem"
    struct _closure__anon_3 *_t223 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t223->gem_v_pos = gem_v_pos;
    _t223->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t223);
#line 39 "compiler/main.gem"
    struct _closure__anon_4 *_t226 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t226->gem_v_pos = gem_v_pos;
    _t226->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t226);
#line 43 "compiler/main.gem"
    struct _closure__anon_5 *_t235 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t235->gem_v_pos = gem_v_pos;
    _t235->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t235);
#line 52 "compiler/main.gem"
    struct _closure__anon_6 *_t237 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t237->gem_v_pos = gem_v_pos;
    _t237->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t237);
#line 60 "compiler/main.gem"
    struct _closure__anon_7 *_t252 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t252->gem_v_advance = gem_v_advance;
    _t252->gem_v_expect = gem_v_expect;
    _t252->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t252);
#line 74 "compiler/main.gem"
    GemVal *gem_v_parse_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_expr = GEM_NIL;
#line 75 "compiler/main.gem"
    GemVal *gem_v_parse_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_stmt = GEM_NIL;
#line 76 "compiler/main.gem"
    GemVal *gem_v_parse_unary = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_unary = GEM_NIL;
#line 77 "compiler/main.gem"
    GemVal *gem_v_parse_not = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_not = GEM_NIL;
#line 83 "compiler/main.gem"
    struct _closure__anon_8 *_t272 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t272->gem_v_at_end = gem_v_at_end;
    _t272->gem_v_parse_stmt = gem_v_parse_stmt;
    _t272->gem_v_peek = gem_v_peek;
    _t272->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t272);
#line 96 "compiler/main.gem"
    struct _closure__anon_9 *_t283 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t283->gem_v_at_end = gem_v_at_end;
    _t283->gem_v_parse_stmt = gem_v_parse_stmt;
    _t283->gem_v_peek = gem_v_peek;
    _t283->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t283);
#line 109 "compiler/main.gem"
    struct _closure__anon_10 *_t402 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t402->gem_v_advance = gem_v_advance;
    _t402->gem_v_expect = gem_v_expect;
    _t402->gem_v_fn_depth = gem_v_fn_depth;
    _t402->gem_v_parse_expr = gem_v_parse_expr;
    _t402->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t402->gem_v_parse_params = gem_v_parse_params;
    _t402->gem_v_peek = gem_v_peek;
    _t402->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t402);
#line 231 "compiler/main.gem"
    struct _closure__anon_11 *_t485 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t485->gem_v_advance = gem_v_advance;
    _t485->gem_v_expect = gem_v_expect;
    _t485->gem_v_fn_depth = gem_v_fn_depth;
    _t485->gem_v_parse_atom = gem_v_parse_atom;
    _t485->gem_v_parse_expr = gem_v_parse_expr;
    _t485->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t485->gem_v_parse_params = gem_v_parse_params;
    _t485->gem_v_peek = gem_v_peek;
    _t485->gem_v_peek_at = gem_v_peek_at;
    _t485->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t485);
#line 321 "compiler/main.gem"
    struct _closure__anon_12 *_t494 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t494->gem_v_advance = gem_v_advance;
    _t494->gem_v_parse_call = gem_v_parse_call;
    _t494->gem_v_parse_unary = gem_v_parse_unary;
    _t494->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t494);
#line 330 "compiler/main.gem"
    struct _closure__anon_13 *_t509 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t509->gem_v_advance = gem_v_advance;
    _t509->gem_v_parse_unary = gem_v_parse_unary;
    _t509->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t509);
#line 341 "compiler/main.gem"
    struct _closure__anon_14 *_t521 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t521->gem_v_advance = gem_v_advance;
    _t521->gem_v_parse_mul = gem_v_parse_mul;
    _t521->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t521);
#line 352 "compiler/main.gem"
    struct _closure__anon_15 *_t545 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t545->gem_v_advance = gem_v_advance;
    _t545->gem_v_parse_add = gem_v_parse_add;
    _t545->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t545);
#line 363 "compiler/main.gem"
    struct _closure__anon_16 *_t554 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t554->gem_v_advance = gem_v_advance;
    _t554->gem_v_parse_compare = gem_v_parse_compare;
    _t554->gem_v_parse_not = gem_v_parse_not;
    _t554->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t554);
#line 372 "compiler/main.gem"
    struct _closure__anon_17 *_t562 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t562->gem_v_advance = gem_v_advance;
    _t562->gem_v_parse_not = gem_v_parse_not;
    _t562->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t562);
#line 383 "compiler/main.gem"
    struct _closure__anon_18 *_t570 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t570->gem_v_advance = gem_v_advance;
    _t570->gem_v_parse_and = gem_v_parse_and;
    _t570->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t570);
#line 394 "compiler/main.gem"
    struct _closure__anon_19 *_t621 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t621->gem_v_advance = gem_v_advance;
    _t621->gem_v_parse_expr = gem_v_parse_expr;
    _t621->gem_v_parse_or = gem_v_parse_or;
    _t621->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t621);
#line 429 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 433 "compiler/main.gem"
    struct _closure__anon_20 *_t885 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t885->gem_v_advance = gem_v_advance;
    _t885->gem_v_expect = gem_v_expect;
    _t885->gem_v_fn_depth = gem_v_fn_depth;
    _t885->gem_v_gensym_counter = gem_v_gensym_counter;
    _t885->gem_v_parse_body = gem_v_parse_body;
    _t885->gem_v_parse_expr = gem_v_parse_expr;
    _t885->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t885->gem_v_parse_params = gem_v_parse_params;
    _t885->gem_v_parse_stmt = gem_v_parse_stmt;
    _t885->gem_v_peek = gem_v_peek;
    _t885->gem_v_peek_at = gem_v_peek_at;
    _t885->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_20, _t885);
#line 653 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 657 "compiler/main.gem"
    struct _closure__anon_21 *_t894 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t894->gem_v_at_end = gem_v_at_end;
    _t894->gem_v_parse_stmt = gem_v_parse_stmt;
    _t894->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_21, _t894);
    GemVal _t895 = gem_table_new();
    gem_table_set(_t895, gem_string("parse"), gem_v_parse);
    GemVal _t896 = _t895;
    gem_pop_frame();
    return _t896;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("escape_c_string", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
    {
#line 11 "compiler/main.gem"
        GemVal gem_v__for_i_1 = gem_int(0);
#line 11 "compiler/main.gem"
    GemVal _t897[] = {gem_v_s};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t897, 1);
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
    GemVal _t898 = gem_v_result;
    gem_pop_frame();
    return _t898;
}

static GemVal gem_fn_repeat_str(void *_env, GemVal *args, int argc) {
#line 28 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("repeat_str", "compiler/main.gem", 28);
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
    GemVal _t899 = gem_v_result;
    gem_pop_frame();
    return _t899;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 36 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("pad_for", "compiler/main.gem", 36);
#line 37 "compiler/main.gem"
    GemVal _t900[] = {gem_string("    "), gem_v_indent};
    GemVal _t901 = gem_fn_repeat_str(NULL, _t900, 2);
    gem_pop_frame();
    return _t901;
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 40 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("format_float", "compiler/main.gem", 40);
#line 41 "compiler/main.gem"
    GemVal _t902[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t902, 1);
#line 43 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
    {
#line 44 "compiler/main.gem"
        GemVal gem_v__for_i_3 = gem_int(0);
#line 44 "compiler/main.gem"
    GemVal _t903[] = {gem_v_s};
        GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t903, 1);
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
    GemVal _t904 = gem_v_s;
    gem_pop_frame();
    return _t904;
}

static GemVal gem_fn_array_contains(void *_env, GemVal *args, int argc) {
#line 57 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("array_contains", "compiler/main.gem", 57);
    {
#line 58 "compiler/main.gem"
        GemVal gem_v__for_i_4 = gem_int(0);
#line 58 "compiler/main.gem"
    GemVal _t905[] = {gem_v_arr};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t905, 1);
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
                GemVal _t906 = gem_bool(1);
                gem_pop_frame();
                return _t906;
            }
        }
    }
    GemVal _t907 = gem_bool(0);
    gem_pop_frame();
    return _t907;
}

static GemVal gem_fn_set_add(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_add", "compiler/main.gem", 66);
#line 67 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1)))) {
#line 68 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_val, gem_bool(1));
        gem_pop_frame();
        return GEM_NIL;
    } else {
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn_set_contains(void *_env, GemVal *args, int argc) {
#line 72 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_contains", "compiler/main.gem", 72);
    GemVal _t908 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t908;
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 76 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_union", "compiler/main.gem", 76);
#line 77 "compiler/main.gem"
    GemVal _t909 = gem_table_new();
    GemVal gem_v_result = _t909;
#line 78 "compiler/main.gem"
    GemVal _t910[] = {gem_v_a};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t910, 1);
#line 79 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 80 "compiler/main.gem"
    while (1) {
        GemVal _t911[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t911, 1)))) break;
#line 81 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
#line 82 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 84 "compiler/main.gem"
    GemVal _t912[] = {gem_v_b};
    GemVal gem_v_ks2 = gem_keys_fn(NULL, _t912, 1);
#line 85 "compiler/main.gem"
    GemVal gem_v_j = gem_int(0);
#line 86 "compiler/main.gem"
    while (1) {
        GemVal _t913[] = {gem_v_ks2};
        if (!gem_truthy(gem_lt(gem_v_j, gem_len_fn(NULL, _t913, 1)))) break;
#line 87 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks2, gem_v_j), gem_bool(1));
#line 88 "compiler/main.gem"
        gem_v_j = gem_add(gem_v_j, gem_int(1));
    }
    GemVal _t914 = gem_v_result;
    gem_pop_frame();
    return _t914;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_intersect", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t915 = gem_table_new();
    GemVal gem_v_result = _t915;
#line 95 "compiler/main.gem"
    GemVal _t916[] = {gem_v_a};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t916, 1);
#line 96 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 97 "compiler/main.gem"
    while (1) {
        GemVal _t917[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t917, 1)))) break;
#line 98 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_table_get(gem_v_ks, gem_v_i)), gem_bool(1)))) {
#line 99 "compiler/main.gem"
            gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
        }
#line 101 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t918 = gem_v_result;
    gem_pop_frame();
    return _t918;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
#line 106 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("set_to_sorted_array", "compiler/main.gem", 106);
#line 107 "compiler/main.gem"
    GemVal _t919[] = {gem_v_s};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t919, 1);
#line 109 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 110 "compiler/main.gem"
    while (1) {
        GemVal _t920[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t920, 1)))) break;
#line 111 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
#line 112 "compiler/main.gem"
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
#line 113 "compiler/main.gem"
        while (1) {
            GemVal _t921;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t921 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t921 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t921)) break;
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
    GemVal _t922 = gem_v_ks;
    gem_pop_frame();
    return _t922;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 123 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("sorted_array_to_set", "compiler/main.gem", 123);
#line 124 "compiler/main.gem"
    GemVal _t923 = gem_table_new();
    GemVal gem_v_s = _t923;
    {
#line 125 "compiler/main.gem"
        GemVal gem_v__for_items_5 = gem_v_arr;
#line 125 "compiler/main.gem"
        GemVal gem_v__for_i_5 = gem_int(0);
#line 125 "compiler/main.gem"
        while (1) {
            GemVal _t924[] = {gem_v__for_items_5};
            if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t924, 1)))) break;
#line 125 "compiler/main.gem"
            GemVal gem_v_item = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 125 "compiler/main.gem"
            gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 126 "compiler/main.gem"
            gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
        }
    }
    GemVal _t925 = gem_v_s;
    gem_pop_frame();
    return _t925;
}

struct _closure__anon_22 {
    GemVal *gem_v_tmp_counter;
};
static GemVal _anon_22(void *_env, GemVal *args, int argc) {
    struct _closure__anon_22 *_cls = (struct _closure__anon_22 *)_env;
    GemVal *gem_v_tmp_counter = _cls->gem_v_tmp_counter;
    gem_push_frame("_anon_22", "compiler/main.gem", 0);
#line 155 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t931[] = {(*gem_v_tmp_counter)};
    GemVal _t932 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t931, 1));
    gem_pop_frame();
    return _t932;
}

struct _closure__anon_23 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_23", "compiler/main.gem", 0);
#line 160 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t934[] = {(*gem_v_anon_counter)};
    GemVal _t935 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t934, 1));
    gem_pop_frame();
    return _t935;
}

struct _closure__anon_24 {
};
static GemVal _anon_24(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_24", "compiler/main.gem", 0);
    GemVal _t937[] = {gem_v_name};
    GemVal _t938 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t937, 1));
    gem_pop_frame();
    return _t938;
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
    gem_push_frame("_anon_25", "compiler/main.gem", 0);
#line 174 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 175 "compiler/main.gem"
        GemVal _t939 = GEM_NIL;
        gem_pop_frame();
        return _t939;
    }
#line 177 "compiler/main.gem"
    GemVal _t940[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t940, 1), gem_string("table")))) {
#line 178 "compiler/main.gem"
        GemVal _t941 = GEM_NIL;
        gem_pop_frame();
        return _t941;
    }
#line 180 "compiler/main.gem"
    GemVal _t942 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t942, gem_string("tag"));
#line 181 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 182 "compiler/main.gem"
        GemVal _t943 = GEM_NIL;
        gem_pop_frame();
        return _t943;
    }
#line 185 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 186 "compiler/main.gem"
    GemVal _t944 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t944, gem_string("name"));
#line 187 "compiler/main.gem"
    GemVal _t945[] = {gem_v_defined, gem_v_name};
    GemVal _t947;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t945, 2)))) {
        _t947 = gem_not(gem_fn_set_contains(NULL, _t945, 2));
    } else {
        GemVal _t946[] = {(*gem_v_builtins), gem_v_name};
        _t947 = gem_not(gem_fn_set_contains(NULL, _t946, 2));
    }
    GemVal _t949;
    if (!gem_truthy(_t947)) {
        _t949 = _t947;
    } else {
        GemVal _t948[] = {(*gem_v_defined_fns), gem_v_name};
        _t949 = gem_not(gem_fn_set_contains(NULL, _t948, 2));
    }
        if (gem_truthy(_t949)) {
#line 188 "compiler/main.gem"
    GemVal _t950[] = {gem_v_free, gem_v_name};
            GemVal _t951 = gem_fn_set_add(NULL, _t950, 2);
            gem_pop_frame();
            return _t951;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
#line 190 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 191 "compiler/main.gem"
    GemVal _t952 = gem_v_node;
    GemVal _t953 = (*gem_v_collect_free_node);
    GemVal _t954[] = {gem_table_get(_t952, gem_string("value")), gem_v_defined, gem_v_free};
            GemVal _t955 = _t953.fn(_t953.env, _t954, 3);
            gem_pop_frame();
            return _t955;
        } else {
#line 192 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 193 "compiler/main.gem"
    GemVal _t956 = gem_v_node;
                GemVal gem_v_name = gem_table_get(_t956, gem_string("name"));
#line 194 "compiler/main.gem"
    GemVal _t957[] = {gem_v_defined, gem_v_name};
    GemVal _t959;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t957, 2)))) {
        _t959 = gem_not(gem_fn_set_contains(NULL, _t957, 2));
    } else {
        GemVal _t958[] = {(*gem_v_builtins), gem_v_name};
        _t959 = gem_not(gem_fn_set_contains(NULL, _t958, 2));
    }
    GemVal _t961;
    if (!gem_truthy(_t959)) {
        _t961 = _t959;
    } else {
        GemVal _t960[] = {(*gem_v_defined_fns), gem_v_name};
        _t961 = gem_not(gem_fn_set_contains(NULL, _t960, 2));
    }
                if (gem_truthy(_t961)) {
#line 195 "compiler/main.gem"
    GemVal _t962[] = {gem_v_free, gem_v_name};
                    (void)(gem_fn_set_add(NULL, _t962, 2));
                }
#line 197 "compiler/main.gem"
    GemVal _t963 = gem_v_node;
    GemVal _t964 = (*gem_v_collect_free_node);
    GemVal _t965[] = {gem_table_get(_t963, gem_string("value")), gem_v_defined, gem_v_free};
                GemVal _t966 = _t964.fn(_t964.env, _t965, 3);
                gem_pop_frame();
                return _t966;
            } else {
#line 198 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 199 "compiler/main.gem"
    GemVal _t967 = gem_v_node;
    GemVal _t968[] = {gem_table_get(_t967, gem_string("params"))};
    GemVal _t969[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t968, 1)};
                    GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t969, 2);
#line 200 "compiler/main.gem"
    GemVal _t970 = gem_v_node;
    GemVal _t971 = (*gem_v_collect_free_vars);
    GemVal _t972[] = {gem_table_get(_t970, gem_string("body")), gem_v_inner_defined};
                    GemVal gem_v_inner_free = _t971.fn(_t971.env, _t972, 2);
#line 201 "compiler/main.gem"
    GemVal _t973[] = {gem_v_inner_free};
                    GemVal gem_v_ifks = gem_keys_fn(NULL, _t973, 1);
#line 202 "compiler/main.gem"
                    GemVal gem_v_ifi = gem_int(0);
#line 203 "compiler/main.gem"
                    while (1) {
                        GemVal _t974[] = {gem_v_ifks};
                        if (!gem_truthy(gem_lt(gem_v_ifi, gem_len_fn(NULL, _t974, 1)))) break;
#line 204 "compiler/main.gem"
    GemVal _t975[] = {gem_v_free, gem_table_get(gem_v_ifks, gem_v_ifi)};
                        (void)(gem_fn_set_add(NULL, _t975, 2));
#line 205 "compiler/main.gem"
                        gem_v_ifi = gem_add(gem_v_ifi, gem_int(1));
                    }
                    gem_pop_frame();
                    return GEM_NIL;
                } else {
#line 207 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                        gem_pop_frame();
                        return GEM_NIL;
                    } else {
#line 209 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 210 "compiler/main.gem"
    GemVal _t976 = gem_v_node;
    GemVal _t977 = (*gem_v_collect_free_node);
    GemVal _t978[] = {gem_table_get(_t976, gem_string("cond")), gem_v_defined, gem_v_free};
                            (void)(_t977.fn(_t977.env, _t978, 3));
#line 211 "compiler/main.gem"
    GemVal _t979 = gem_table_new();
    GemVal _t980[] = {gem_v_defined, _t979};
                            GemVal gem_v_d = gem_fn_set_union(NULL, _t980, 2);
#line 212 "compiler/main.gem"
                            GemVal gem_v_thens = gem_table_get(gem_v_node, gem_string("then"));
                            {
#line 213 "compiler/main.gem"
                                GemVal gem_v__for_i_6 = gem_int(0);
#line 213 "compiler/main.gem"
    GemVal _t981[] = {gem_v_thens};
                                GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t981, 1);
#line 213 "compiler/main.gem"
                                while (1) {
                                    if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
#line 213 "compiler/main.gem"
                                    GemVal gem_v_i = gem_v__for_i_6;
#line 213 "compiler/main.gem"
                                    gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 214 "compiler/main.gem"
    GemVal _t982 = (*gem_v_collect_free_node);
    GemVal _t983[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
                                    (void)(_t982.fn(_t982.env, _t983, 3));
#line 215 "compiler/main.gem"
    GemVal _t984[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t986;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t984, 1), gem_string("table")))) {
        _t986 = gem_eq(gem_type_fn(NULL, _t984, 1), gem_string("table"));
    } else {
        GemVal _t985 = gem_table_get(gem_v_thens, gem_v_i);
        _t986 = gem_eq(gem_table_get(_t985, gem_string("tag")), gem_string("let"));
    }
                                    if (gem_truthy(_t986)) {
#line 216 "compiler/main.gem"
    GemVal _t987 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t988[] = {gem_v_d, gem_table_get(_t987, gem_string("name"))};
                                        (void)(gem_fn_set_add(NULL, _t988, 2));
                                    }
                                }
                            }
#line 219 "compiler/main.gem"
                            GemVal gem_v_el = gem_table_get(gem_v_node, gem_string("else"));
#line 220 "compiler/main.gem"
                            if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 221 "compiler/main.gem"
    GemVal _t989 = gem_table_new();
    GemVal _t990[] = {gem_v_defined, _t989};
                                gem_v_d = gem_fn_set_union(NULL, _t990, 2);
                                {
#line 222 "compiler/main.gem"
                                    GemVal gem_v__for_i_7 = gem_int(0);
#line 222 "compiler/main.gem"
    GemVal _t991[] = {gem_v_el};
                                    GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t991, 1);
#line 222 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 222 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_7;
#line 222 "compiler/main.gem"
                                        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 223 "compiler/main.gem"
    GemVal _t992 = (*gem_v_collect_free_node);
    GemVal _t993[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t992.fn(_t992.env, _t993, 3));
#line 224 "compiler/main.gem"
    GemVal _t994[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t996;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t994, 1), gem_string("table")))) {
        _t996 = gem_eq(gem_type_fn(NULL, _t994, 1), gem_string("table"));
    } else {
        GemVal _t995 = gem_table_get(gem_v_el, gem_v_i);
        _t996 = gem_eq(gem_table_get(_t995, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t996)) {
#line 225 "compiler/main.gem"
    GemVal _t997 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t998[] = {gem_v_d, gem_table_get(_t997, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t998, 2));
                                        }
                                    }
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
                                gem_pop_frame();
                                return GEM_NIL;
                            }
                        } else {
#line 229 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 230 "compiler/main.gem"
    GemVal _t999 = gem_v_node;
    GemVal _t1000 = (*gem_v_collect_free_node);
    GemVal _t1001[] = {gem_table_get(_t999, gem_string("cond")), gem_v_defined, gem_v_free};
                                (void)(_t1000.fn(_t1000.env, _t1001, 3));
#line 231 "compiler/main.gem"
    GemVal _t1002 = gem_table_new();
    GemVal _t1003[] = {gem_v_defined, _t1002};
                                GemVal gem_v_d = gem_fn_set_union(NULL, _t1003, 2);
                                {
#line 232 "compiler/main.gem"
                                    GemVal gem_v__for_i_8 = gem_int(0);
#line 232 "compiler/main.gem"
    GemVal _t1004 = gem_v_node;
    GemVal _t1005[] = {gem_table_get(_t1004, gem_string("body"))};
                                    GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1005, 1);
#line 232 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 232 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_8;
#line 232 "compiler/main.gem"
                                        gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 233 "compiler/main.gem"
    GemVal _t1006 = gem_v_node;
    GemVal _t1007 = (*gem_v_collect_free_node);
    GemVal _t1008[] = {gem_table_get(gem_table_get(_t1006, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t1007.fn(_t1007.env, _t1008, 3));
#line 234 "compiler/main.gem"
    GemVal _t1009 = gem_v_node;
    GemVal _t1010[] = {gem_table_get(gem_table_get(_t1009, gem_string("body")), gem_v_i)};
    GemVal _t1013;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1010, 1), gem_string("table")))) {
        _t1013 = gem_eq(gem_type_fn(NULL, _t1010, 1), gem_string("table"));
    } else {
        GemVal _t1011 = gem_v_node;
        GemVal _t1012 = gem_table_get(gem_table_get(_t1011, gem_string("body")), gem_v_i);
        _t1013 = gem_eq(gem_table_get(_t1012, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t1013)) {
#line 235 "compiler/main.gem"
    GemVal _t1014 = gem_v_node;
    GemVal _t1015 = gem_table_get(gem_table_get(_t1014, gem_string("body")), gem_v_i);
    GemVal _t1016[] = {gem_v_d, gem_table_get(_t1015, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t1016, 2));
                                        }
                                    }
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 238 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 239 "compiler/main.gem"
    GemVal _t1017 = gem_v_node;
    GemVal _t1018 = (*gem_v_collect_free_node);
    GemVal _t1019[] = {gem_table_get(_t1017, gem_string("target")), gem_v_defined, gem_v_free};
                                    (void)(_t1018.fn(_t1018.env, _t1019, 3));
                                    {
#line 240 "compiler/main.gem"
                                        GemVal gem_v__for_i_10 = gem_int(0);
#line 240 "compiler/main.gem"
    GemVal _t1020 = gem_v_node;
    GemVal _t1021[] = {gem_table_get(_t1020, gem_string("whens"))};
                                        GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t1021, 1);
#line 240 "compiler/main.gem"
                                        while (1) {
                                            if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
#line 240 "compiler/main.gem"
                                            GemVal gem_v_i = gem_v__for_i_10;
#line 240 "compiler/main.gem"
                                            gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 241 "compiler/main.gem"
    GemVal _t1022 = gem_v_node;
                                            GemVal gem_v_w = gem_table_get(gem_table_get(_t1022, gem_string("whens")), gem_v_i);
#line 242 "compiler/main.gem"
    GemVal _t1023 = gem_v_w;
    GemVal _t1024 = (*gem_v_collect_free_node);
    GemVal _t1025[] = {gem_table_get(_t1023, gem_string("value")), gem_v_defined, gem_v_free};
                                            (void)(_t1024.fn(_t1024.env, _t1025, 3));
#line 243 "compiler/main.gem"
    GemVal _t1026 = gem_table_new();
    GemVal _t1027[] = {gem_v_defined, _t1026};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1027, 2);
                                            {
#line 244 "compiler/main.gem"
                                                GemVal gem_v__for_i_9 = gem_int(0);
#line 244 "compiler/main.gem"
    GemVal _t1028 = gem_v_w;
    GemVal _t1029[] = {gem_table_get(_t1028, gem_string("body"))};
                                                GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1029, 1);
#line 244 "compiler/main.gem"
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 244 "compiler/main.gem"
                                                    GemVal gem_v_j = gem_v__for_i_9;
#line 244 "compiler/main.gem"
                                                    gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 245 "compiler/main.gem"
    GemVal _t1030 = gem_v_w;
    GemVal _t1031 = (*gem_v_collect_free_node);
    GemVal _t1032[] = {gem_table_get(gem_table_get(_t1030, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
                                                    (void)(_t1031.fn(_t1031.env, _t1032, 3));
#line 246 "compiler/main.gem"
    GemVal _t1033 = gem_v_w;
    GemVal _t1034[] = {gem_table_get(gem_table_get(_t1033, gem_string("body")), gem_v_j)};
    GemVal _t1037;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1034, 1), gem_string("table")))) {
        _t1037 = gem_eq(gem_type_fn(NULL, _t1034, 1), gem_string("table"));
    } else {
        GemVal _t1035 = gem_v_w;
        GemVal _t1036 = gem_table_get(gem_table_get(_t1035, gem_string("body")), gem_v_j);
        _t1037 = gem_eq(gem_table_get(_t1036, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t1037)) {
#line 247 "compiler/main.gem"
    GemVal _t1038 = gem_v_w;
    GemVal _t1039 = gem_table_get(gem_table_get(_t1038, gem_string("body")), gem_v_j);
    GemVal _t1040[] = {gem_v_d, gem_table_get(_t1039, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t1040, 2));
                                                    }
                                                }
                                            }
                                        }
                                    }
#line 251 "compiler/main.gem"
                                    GemVal gem_v_el = gem_table_get(gem_v_node, gem_string("else"));
#line 252 "compiler/main.gem"
                                    if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 253 "compiler/main.gem"
    GemVal _t1041 = gem_table_new();
    GemVal _t1042[] = {gem_v_defined, _t1041};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1042, 2);
                                        {
#line 254 "compiler/main.gem"
                                            GemVal gem_v__for_i_11 = gem_int(0);
#line 254 "compiler/main.gem"
    GemVal _t1043[] = {gem_v_el};
                                            GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t1043, 1);
#line 254 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
#line 254 "compiler/main.gem"
                                                GemVal gem_v_j = gem_v__for_i_11;
#line 254 "compiler/main.gem"
                                                gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 255 "compiler/main.gem"
    GemVal _t1044 = (*gem_v_collect_free_node);
    GemVal _t1045[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
                                                (void)(_t1044.fn(_t1044.env, _t1045, 3));
#line 256 "compiler/main.gem"
    GemVal _t1046[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t1048;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1046, 1), gem_string("table")))) {
        _t1048 = gem_eq(gem_type_fn(NULL, _t1046, 1), gem_string("table"));
    } else {
        GemVal _t1047 = gem_table_get(gem_v_el, gem_v_j);
        _t1048 = gem_eq(gem_table_get(_t1047, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1048)) {
#line 257 "compiler/main.gem"
    GemVal _t1049 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t1050[] = {gem_v_d, gem_table_get(_t1049, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1050, 2));
                                                }
                                            }
                                            gem_pop_frame();
                                            return GEM_NIL;
                                        }
                                    } else {
                                        gem_pop_frame();
                                        return GEM_NIL;
                                    }
                                } else {
#line 261 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("when_clause")))) {
#line 262 "compiler/main.gem"
    GemVal _t1051 = gem_v_node;
    GemVal _t1052 = (*gem_v_collect_free_node);
    GemVal _t1053[] = {gem_table_get(_t1051, gem_string("value")), gem_v_defined, gem_v_free};
                                        (void)(_t1052.fn(_t1052.env, _t1053, 3));
#line 263 "compiler/main.gem"
    GemVal _t1054 = gem_table_new();
    GemVal _t1055[] = {gem_v_defined, _t1054};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1055, 2);
                                        {
#line 264 "compiler/main.gem"
                                            GemVal gem_v__for_i_12 = gem_int(0);
#line 264 "compiler/main.gem"
    GemVal _t1056 = gem_v_node;
    GemVal _t1057[] = {gem_table_get(_t1056, gem_string("body"))};
                                            GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t1057, 1);
#line 264 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 264 "compiler/main.gem"
                                                GemVal gem_v_i = gem_v__for_i_12;
#line 264 "compiler/main.gem"
                                                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 265 "compiler/main.gem"
    GemVal _t1058 = gem_v_node;
    GemVal _t1059 = (*gem_v_collect_free_node);
    GemVal _t1060[] = {gem_table_get(gem_table_get(_t1058, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                                (void)(_t1059.fn(_t1059.env, _t1060, 3));
#line 266 "compiler/main.gem"
    GemVal _t1061 = gem_v_node;
    GemVal _t1062[] = {gem_table_get(gem_table_get(_t1061, gem_string("body")), gem_v_i)};
    GemVal _t1065;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1062, 1), gem_string("table")))) {
        _t1065 = gem_eq(gem_type_fn(NULL, _t1062, 1), gem_string("table"));
    } else {
        GemVal _t1063 = gem_v_node;
        GemVal _t1064 = gem_table_get(gem_table_get(_t1063, gem_string("body")), gem_v_i);
        _t1065 = gem_eq(gem_table_get(_t1064, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1065)) {
#line 267 "compiler/main.gem"
    GemVal _t1066 = gem_v_node;
    GemVal _t1067 = gem_table_get(gem_table_get(_t1066, gem_string("body")), gem_v_i);
    GemVal _t1068[] = {gem_v_d, gem_table_get(_t1067, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1068, 2));
                                                }
                                            }
                                            gem_pop_frame();
                                            return GEM_NIL;
                                        }
                                    } else {
#line 270 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 271 "compiler/main.gem"
    GemVal _t1069 = gem_table_new();
    GemVal _t1070[] = {gem_v_defined, _t1069};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1070, 2);
                                            {
#line 272 "compiler/main.gem"
                                                GemVal gem_v__for_i_13 = gem_int(0);
#line 272 "compiler/main.gem"
    GemVal _t1071 = gem_v_node;
    GemVal _t1072[] = {gem_table_get(_t1071, gem_string("stmts"))};
                                                GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1072, 1);
#line 272 "compiler/main.gem"
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 272 "compiler/main.gem"
                                                    GemVal gem_v_i = gem_v__for_i_13;
#line 272 "compiler/main.gem"
                                                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 273 "compiler/main.gem"
    GemVal _t1073 = gem_v_node;
    GemVal _t1074 = (*gem_v_collect_free_node);
    GemVal _t1075[] = {gem_table_get(gem_table_get(_t1073, gem_string("stmts")), gem_v_i), gem_v_d, gem_v_free};
                                                    (void)(_t1074.fn(_t1074.env, _t1075, 3));
#line 274 "compiler/main.gem"
    GemVal _t1076 = gem_v_node;
    GemVal _t1077[] = {gem_table_get(gem_table_get(_t1076, gem_string("stmts")), gem_v_i)};
    GemVal _t1080;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1077, 1), gem_string("table")))) {
        _t1080 = gem_eq(gem_type_fn(NULL, _t1077, 1), gem_string("table"));
    } else {
        GemVal _t1078 = gem_v_node;
        GemVal _t1079 = gem_table_get(gem_table_get(_t1078, gem_string("stmts")), gem_v_i);
        _t1080 = gem_eq(gem_table_get(_t1079, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t1080)) {
#line 275 "compiler/main.gem"
    GemVal _t1081 = gem_v_node;
    GemVal _t1082 = gem_table_get(gem_table_get(_t1081, gem_string("stmts")), gem_v_i);
    GemVal _t1083[] = {gem_v_d, gem_table_get(_t1082, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t1083, 2));
                                                    }
                                                }
                                                gem_pop_frame();
                                                return GEM_NIL;
                                            }
                                        } else {
#line 278 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 279 "compiler/main.gem"
    GemVal _t1084 = gem_v_node;
                                                if (gem_truthy(gem_neq(gem_table_get(_t1084, gem_string("value")), GEM_NIL))) {
#line 280 "compiler/main.gem"
    GemVal _t1085 = gem_v_node;
    GemVal _t1086 = (*gem_v_collect_free_node);
    GemVal _t1087[] = {gem_table_get(_t1085, gem_string("value")), gem_v_defined, gem_v_free};
                                                    GemVal _t1088 = _t1086.fn(_t1086.env, _t1087, 3);
                                                    gem_pop_frame();
                                                    return _t1088;
                                                } else {
                                                    gem_pop_frame();
                                                    return GEM_NIL;
                                                }
                                            } else {
#line 284 "compiler/main.gem"
    GemVal _t1089[] = {gem_v_node};
                                                GemVal gem_v_nks = gem_keys_fn(NULL, _t1089, 1);
#line 285 "compiler/main.gem"
                                                GemVal gem_v_nki = gem_int(0);
#line 286 "compiler/main.gem"
                                                while (1) {
                                                    GemVal _t1090[] = {gem_v_nks};
                                                    if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t1090, 1)))) break;
#line 287 "compiler/main.gem"
                                                    if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
#line 288 "compiler/main.gem"
                                                        GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
#line 289 "compiler/main.gem"
    GemVal _t1091[] = {gem_v_v};
                                                        if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1091, 1), gem_string("table")))) {
#line 291 "compiler/main.gem"
    GemVal _t1092[] = {gem_v_v};
                                                            GemVal gem_v_vks = gem_keys_fn(NULL, _t1092, 1);
#line 292 "compiler/main.gem"
    GemVal _t1093[] = {gem_v_vks};
    GemVal _t1095;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1093, 1), gem_int(0)))) {
        _t1095 = gem_gt(gem_len_fn(NULL, _t1093, 1), gem_int(0));
    } else {
        GemVal _t1094[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1095 = gem_eq(gem_type_fn(NULL, _t1094, 1), gem_string("int"));
    }
                                                            if (gem_truthy(_t1095)) {
#line 294 "compiler/main.gem"
                                                                GemVal gem_v_vi = gem_int(0);
#line 295 "compiler/main.gem"
                                                                while (1) {
                                                                    GemVal _t1096[] = {gem_v_v};
                                                                    if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1096, 1)))) break;
#line 296 "compiler/main.gem"
    GemVal _t1097[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1097, 1), gem_string("table")))) {
#line 297 "compiler/main.gem"
    GemVal _t1098 = (*gem_v_collect_free_node);
    GemVal _t1099[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
                                                                        (void)(_t1098.fn(_t1098.env, _t1099, 3));
                                                                    }
#line 299 "compiler/main.gem"
                                                                    gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                                }
                                                            } else {
#line 302 "compiler/main.gem"
    GemVal _t1100 = (*gem_v_collect_free_node);
    GemVal _t1101[] = {gem_v_v, gem_v_defined, gem_v_free};
                                                                (void)(_t1100.fn(_t1100.env, _t1101, 3));
                                                            }
                                                        }
                                                    }
#line 306 "compiler/main.gem"
                                                    gem_v_nki = gem_add(gem_v_nki, gem_int(1));
                                                }
                                                gem_pop_frame();
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
    gem_push_frame("_anon_26", "compiler/main.gem", 0);
#line 312 "compiler/main.gem"
    GemVal _t1103 = gem_table_new();
    GemVal gem_v_free = _t1103;
#line 313 "compiler/main.gem"
    GemVal _t1104 = gem_table_new();
    GemVal _t1105[] = {gem_v_defined, _t1104};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t1105, 2);
    {
#line 314 "compiler/main.gem"
        GemVal gem_v__for_i_15 = gem_int(0);
#line 314 "compiler/main.gem"
    GemVal _t1106[] = {gem_v_stmts};
        GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1106, 1);
#line 314 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 314 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_15;
#line 314 "compiler/main.gem"
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 315 "compiler/main.gem"
    GemVal _t1107 = (*gem_v_collect_free_node);
    GemVal _t1108[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
            (void)(_t1107.fn(_t1107.env, _t1108, 3));
#line 316 "compiler/main.gem"
    GemVal _t1109[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1111;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1109, 1), gem_string("table")))) {
        _t1111 = gem_eq(gem_type_fn(NULL, _t1109, 1), gem_string("table"));
    } else {
        GemVal _t1110 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1111 = gem_eq(gem_table_get(_t1110, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1111)) {
#line 317 "compiler/main.gem"
    GemVal _t1112 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1113[] = {gem_v_d, gem_table_get(_t1112, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1113, 2));
            }
#line 319 "compiler/main.gem"
    GemVal _t1114[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1116;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1114, 1), gem_string("table")))) {
        _t1116 = gem_eq(gem_type_fn(NULL, _t1114, 1), gem_string("table"));
    } else {
        GemVal _t1115 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1116 = gem_eq(gem_table_get(_t1115, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1116)) {
                {
#line 320 "compiler/main.gem"
                    GemVal gem_v__for_i_14 = gem_int(0);
#line 320 "compiler/main.gem"
    GemVal _t1117 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1118[] = {gem_table_get(_t1117, gem_string("stmts"))};
                    GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1118, 1);
#line 320 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 320 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_14;
#line 320 "compiler/main.gem"
                        gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 321 "compiler/main.gem"
    GemVal _t1119 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1120[] = {gem_table_get(gem_table_get(_t1119, gem_string("stmts")), gem_v_j)};
    GemVal _t1123;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1120, 1), gem_string("table")))) {
        _t1123 = gem_eq(gem_type_fn(NULL, _t1120, 1), gem_string("table"));
    } else {
        GemVal _t1121 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1122 = gem_table_get(gem_table_get(_t1121, gem_string("stmts")), gem_v_j);
        _t1123 = gem_eq(gem_table_get(_t1122, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1123)) {
#line 322 "compiler/main.gem"
    GemVal _t1124 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1125 = gem_table_get(gem_table_get(_t1124, gem_string("stmts")), gem_v_j);
    GemVal _t1126[] = {gem_v_d, gem_table_get(_t1125, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1126, 2));
                        }
                    }
                }
            }
        }
    }
    GemVal _t1127 = gem_v_free;
    gem_pop_frame();
    return _t1127;
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
    gem_push_frame("_anon_27", "compiler/main.gem", 0);
#line 336 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 337 "compiler/main.gem"
        GemVal _t1129 = GEM_NIL;
        gem_pop_frame();
        return _t1129;
    }
#line 339 "compiler/main.gem"
    GemVal _t1130[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1130, 1), gem_string("table")))) {
#line 340 "compiler/main.gem"
        GemVal _t1131 = GEM_NIL;
        gem_pop_frame();
        return _t1131;
    }
#line 342 "compiler/main.gem"
    GemVal _t1132 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1132, gem_string("tag"));
#line 343 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 344 "compiler/main.gem"
        GemVal _t1133 = GEM_NIL;
        gem_pop_frame();
        return _t1133;
    }
#line 347 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 348 "compiler/main.gem"
    GemVal _t1134 = gem_v_node;
    GemVal _t1135[] = {gem_table_get(_t1134, gem_string("params"))};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1135, 1);
#line 349 "compiler/main.gem"
    GemVal _t1136 = gem_v_node;
    GemVal _t1137 = (*gem_v_collect_free_vars);
    GemVal _t1138[] = {gem_table_get(_t1136, gem_string("body")), gem_v_inner_defined};
        GemVal gem_v_free = _t1137.fn(_t1137.env, _t1138, 2);
#line 350 "compiler/main.gem"
    GemVal _t1139[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t1139, 2);
#line 351 "compiler/main.gem"
    GemVal _t1140[] = {gem_v_inter};
        GemVal gem_v_iks = gem_keys_fn(NULL, _t1140, 1);
#line 352 "compiler/main.gem"
        GemVal gem_v_iki = gem_int(0);
#line 353 "compiler/main.gem"
        while (1) {
            GemVal _t1141[] = {gem_v_iks};
            if (!gem_truthy(gem_lt(gem_v_iki, gem_len_fn(NULL, _t1141, 1)))) break;
#line 354 "compiler/main.gem"
    GemVal _t1142[] = {gem_v_captured, gem_table_get(gem_v_iks, gem_v_iki)};
            (void)(gem_fn_set_add(NULL, _t1142, 2));
#line 355 "compiler/main.gem"
            gem_v_iki = gem_add(gem_v_iki, gem_int(1));
        }
#line 357 "compiler/main.gem"
    GemVal _t1143 = gem_v_node;
    GemVal _t1144 = gem_v_node;
    GemVal _t1145[] = {gem_table_get(_t1144, gem_string("params"))};
    GemVal _t1146[] = {gem_v_scope_vars, gem_fn_sorted_array_to_set(NULL, _t1145, 1)};
    GemVal _t1147 = (*gem_v_walk_captures);
    GemVal _t1148[] = {gem_table_get(_t1143, gem_string("body")), gem_fn_set_union(NULL, _t1146, 2), gem_v_captured};
        GemVal _t1149 = _t1147.fn(_t1147.env, _t1148, 3);
        gem_pop_frame();
        return _t1149;
    } else {
#line 358 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
            gem_pop_frame();
            return GEM_NIL;
        } else {
#line 360 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 361 "compiler/main.gem"
    GemVal _t1150 = gem_v_node;
    GemVal _t1151 = (*gem_v_walk_captures_node);
    GemVal _t1152[] = {gem_table_get(_t1150, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1151.fn(_t1151.env, _t1152, 3));
#line 362 "compiler/main.gem"
    GemVal _t1153 = (*gem_v_walk_captures);
    GemVal _t1154[] = {gem_table_get(gem_v_node, gem_string("then")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1153.fn(_t1153.env, _t1154, 3));
#line 363 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 364 "compiler/main.gem"
    GemVal _t1155 = (*gem_v_walk_captures);
    GemVal _t1156[] = {gem_table_get(gem_v_node, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                    GemVal _t1157 = _t1155.fn(_t1155.env, _t1156, 3);
                    gem_pop_frame();
                    return _t1157;
                } else {
                    gem_pop_frame();
                    return GEM_NIL;
                }
            } else {
#line 366 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 367 "compiler/main.gem"
    GemVal _t1158 = gem_v_node;
    GemVal _t1159 = (*gem_v_walk_captures_node);
    GemVal _t1160[] = {gem_table_get(_t1158, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                    (void)(_t1159.fn(_t1159.env, _t1160, 3));
#line 368 "compiler/main.gem"
    GemVal _t1161 = gem_v_node;
    GemVal _t1162 = (*gem_v_walk_captures);
    GemVal _t1163[] = {gem_table_get(_t1161, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                    GemVal _t1164 = _t1162.fn(_t1162.env, _t1163, 3);
                    gem_pop_frame();
                    return _t1164;
                } else {
#line 369 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 370 "compiler/main.gem"
    GemVal _t1165 = gem_v_node;
    GemVal _t1166 = (*gem_v_walk_captures_node);
    GemVal _t1167[] = {gem_table_get(_t1165, gem_string("target")), gem_v_scope_vars, gem_v_captured};
                        (void)(_t1166.fn(_t1166.env, _t1167, 3));
                        {
#line 371 "compiler/main.gem"
                            GemVal gem_v__for_i_16 = gem_int(0);
#line 371 "compiler/main.gem"
    GemVal _t1168 = gem_v_node;
    GemVal _t1169[] = {gem_table_get(_t1168, gem_string("whens"))};
                            GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1169, 1);
#line 371 "compiler/main.gem"
                            while (1) {
                                if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 371 "compiler/main.gem"
                                GemVal gem_v_i = gem_v__for_i_16;
#line 371 "compiler/main.gem"
                                gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 372 "compiler/main.gem"
    GemVal _t1170 = gem_v_node;
    GemVal _t1171 = gem_table_get(gem_table_get(_t1170, gem_string("whens")), gem_v_i);
    GemVal _t1172 = (*gem_v_walk_captures_node);
    GemVal _t1173[] = {gem_table_get(_t1171, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t1172.fn(_t1172.env, _t1173, 3));
#line 373 "compiler/main.gem"
    GemVal _t1174 = gem_v_node;
    GemVal _t1175 = gem_table_get(gem_table_get(_t1174, gem_string("whens")), gem_v_i);
    GemVal _t1176 = (*gem_v_walk_captures);
    GemVal _t1177[] = {gem_table_get(_t1175, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t1176.fn(_t1176.env, _t1177, 3));
                            }
                        }
#line 375 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 376 "compiler/main.gem"
    GemVal _t1178 = (*gem_v_walk_captures);
    GemVal _t1179[] = {gem_table_get(gem_v_node, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                            GemVal _t1180 = _t1178.fn(_t1178.env, _t1179, 3);
                            gem_pop_frame();
                            return _t1180;
                        } else {
                            gem_pop_frame();
                            return GEM_NIL;
                        }
                    } else {
#line 378 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 379 "compiler/main.gem"
    GemVal _t1181 = gem_v_node;
    GemVal _t1182 = (*gem_v_walk_captures);
    GemVal _t1183[] = {gem_table_get(_t1181, gem_string("stmts")), gem_v_scope_vars, gem_v_captured};
                            GemVal _t1184 = _t1182.fn(_t1182.env, _t1183, 3);
                            gem_pop_frame();
                            return _t1184;
                        } else {
#line 380 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 381 "compiler/main.gem"
    GemVal _t1185 = gem_v_node;
                                if (gem_truthy(gem_neq(gem_table_get(_t1185, gem_string("value")), GEM_NIL))) {
#line 382 "compiler/main.gem"
    GemVal _t1186 = gem_v_node;
    GemVal _t1187 = (*gem_v_walk_captures_node);
    GemVal _t1188[] = {gem_table_get(_t1186, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    GemVal _t1189 = _t1187.fn(_t1187.env, _t1188, 3);
                                    gem_pop_frame();
                                    return _t1189;
                                } else {
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 384 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 385 "compiler/main.gem"
    GemVal _t1190 = gem_v_node;
    GemVal _t1191 = (*gem_v_walk_captures_node);
    GemVal _t1192[] = {gem_table_get(_t1190, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    GemVal _t1193 = _t1191.fn(_t1191.env, _t1192, 3);
                                    gem_pop_frame();
                                    return _t1193;
                                } else {
#line 386 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 387 "compiler/main.gem"
    GemVal _t1194 = gem_v_node;
    GemVal _t1195 = (*gem_v_walk_captures_node);
    GemVal _t1196[] = {gem_table_get(_t1194, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                        GemVal _t1197 = _t1195.fn(_t1195.env, _t1196, 3);
                                        gem_pop_frame();
                                        return _t1197;
                                    } else {
#line 390 "compiler/main.gem"
    GemVal _t1198[] = {gem_v_node};
                                        GemVal gem_v_nks = gem_keys_fn(NULL, _t1198, 1);
#line 391 "compiler/main.gem"
                                        GemVal gem_v_nki = gem_int(0);
#line 392 "compiler/main.gem"
                                        while (1) {
                                            GemVal _t1199[] = {gem_v_nks};
                                            if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t1199, 1)))) break;
#line 393 "compiler/main.gem"
                                            if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
#line 394 "compiler/main.gem"
                                                GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
#line 395 "compiler/main.gem"
    GemVal _t1200[] = {gem_v_v};
                                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1200, 1), gem_string("table")))) {
#line 396 "compiler/main.gem"
    GemVal _t1201[] = {gem_v_v};
                                                    GemVal gem_v_vks = gem_keys_fn(NULL, _t1201, 1);
#line 397 "compiler/main.gem"
    GemVal _t1202[] = {gem_v_vks};
    GemVal _t1204;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1202, 1), gem_int(0)))) {
        _t1204 = gem_gt(gem_len_fn(NULL, _t1202, 1), gem_int(0));
    } else {
        GemVal _t1203[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1204 = gem_eq(gem_type_fn(NULL, _t1203, 1), gem_string("int"));
    }
                                                    if (gem_truthy(_t1204)) {
#line 398 "compiler/main.gem"
                                                        GemVal gem_v_vi = gem_int(0);
#line 399 "compiler/main.gem"
                                                        while (1) {
                                                            GemVal _t1205[] = {gem_v_v};
                                                            if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1205, 1)))) break;
#line 400 "compiler/main.gem"
    GemVal _t1206[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                            if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1206, 1), gem_string("table")))) {
#line 401 "compiler/main.gem"
    GemVal _t1207 = (*gem_v_walk_captures_node);
    GemVal _t1208[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
                                                                (void)(_t1207.fn(_t1207.env, _t1208, 3));
                                                            }
#line 403 "compiler/main.gem"
                                                            gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                        }
                                                    } else {
#line 406 "compiler/main.gem"
    GemVal _t1209 = (*gem_v_walk_captures_node);
    GemVal _t1210[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
                                                        (void)(_t1209.fn(_t1209.env, _t1210, 3));
                                                    }
                                                }
                                            }
#line 410 "compiler/main.gem"
                                            gem_v_nki = gem_add(gem_v_nki, gem_int(1));
                                        }
                                        gem_pop_frame();
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
    gem_push_frame("_anon_28", "compiler/main.gem", 0);
#line 416 "compiler/main.gem"
    GemVal _t1212 = gem_table_new();
    GemVal _t1213[] = {gem_v_scope_vars, _t1212};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t1213, 2);
    {
#line 417 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 417 "compiler/main.gem"
    GemVal _t1214[] = {gem_v_stmts};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1214, 1);
#line 417 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 417 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_18;
#line 417 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 418 "compiler/main.gem"
    GemVal _t1215 = (*gem_v_walk_captures_node);
    GemVal _t1216[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
            (void)(_t1215.fn(_t1215.env, _t1216, 3));
#line 419 "compiler/main.gem"
    GemVal _t1217[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1219;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1217, 1), gem_string("table")))) {
        _t1219 = gem_eq(gem_type_fn(NULL, _t1217, 1), gem_string("table"));
    } else {
        GemVal _t1218 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1219 = gem_eq(gem_table_get(_t1218, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1219)) {
#line 420 "compiler/main.gem"
    GemVal _t1220 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1221[] = {gem_v_sv, gem_table_get(_t1220, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1221, 2));
            }
#line 422 "compiler/main.gem"
    GemVal _t1222[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1224;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1222, 1), gem_string("table")))) {
        _t1224 = gem_eq(gem_type_fn(NULL, _t1222, 1), gem_string("table"));
    } else {
        GemVal _t1223 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1224 = gem_eq(gem_table_get(_t1223, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1224)) {
                {
#line 423 "compiler/main.gem"
                    GemVal gem_v__for_i_17 = gem_int(0);
#line 423 "compiler/main.gem"
    GemVal _t1225 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1226[] = {gem_table_get(_t1225, gem_string("stmts"))};
                    GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1226, 1);
#line 423 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 423 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_17;
#line 423 "compiler/main.gem"
                        gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 424 "compiler/main.gem"
    GemVal _t1227 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1228[] = {gem_table_get(gem_table_get(_t1227, gem_string("stmts")), gem_v_j)};
    GemVal _t1231;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1228, 1), gem_string("table")))) {
        _t1231 = gem_eq(gem_type_fn(NULL, _t1228, 1), gem_string("table"));
    } else {
        GemVal _t1229 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1230 = gem_table_get(gem_table_get(_t1229, gem_string("stmts")), gem_v_j);
        _t1231 = gem_eq(gem_table_get(_t1230, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1231)) {
#line 425 "compiler/main.gem"
    GemVal _t1232 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1233 = gem_table_get(gem_table_get(_t1232, gem_string("stmts")), gem_v_j);
    GemVal _t1234[] = {gem_v_sv, gem_table_get(_t1233, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1234, 2));
                        }
                    }
                }
            }
        }
        gem_pop_frame();
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
    gem_push_frame("_anon_29", "compiler/main.gem", 0);
#line 433 "compiler/main.gem"
    GemVal _t1236 = gem_table_new();
    GemVal gem_v_captured = _t1236;
#line 434 "compiler/main.gem"
    GemVal _t1237 = (*gem_v_walk_captures);
    GemVal _t1238[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    (void)(_t1237.fn(_t1237.env, _t1238, 3));
    GemVal _t1239 = gem_v_captured;
    gem_pop_frame();
    return _t1239;
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
    gem_push_frame("_anon_30", "compiler/main.gem", 0);
#line 461 "compiler/main.gem"
    GemVal _t1241 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1241, gem_string("tag"));
#line 463 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("int")))) {
#line 464 "compiler/main.gem"
    GemVal _t1242 = gem_table_new();
    GemVal _t1243 = gem_v_node;
    GemVal _t1244[] = {gem_table_get(_t1243, gem_string("value"))};
    gem_table_set(_t1242, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t1244, 1)), gem_string(")")));
    gem_table_set(_t1242, gem_string("setup"), gem_string(""));
        GemVal _t1245 = _t1242;
        gem_pop_frame();
        return _t1245;
    } else {
#line 465 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("float")))) {
#line 466 "compiler/main.gem"
    GemVal _t1246 = gem_table_new();
    GemVal _t1247 = gem_v_node;
    GemVal _t1248[] = {gem_table_get(_t1247, gem_string("value"))};
    GemVal _t1249[] = {gem_fn_format_float(NULL, _t1248, 1)};
    gem_table_set(_t1246, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t1249, 1)), gem_string(")")));
    gem_table_set(_t1246, gem_string("setup"), gem_string(""));
            GemVal _t1250 = _t1246;
            gem_pop_frame();
            return _t1250;
        } else {
#line 467 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("string")))) {
#line 468 "compiler/main.gem"
    GemVal _t1251 = gem_v_node;
    GemVal _t1252[] = {gem_table_get(_t1251, gem_string("value"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1252, 1);
#line 469 "compiler/main.gem"
    GemVal _t1253 = gem_table_new();
    GemVal _t1254[] = {gem_v_escaped};
    gem_table_set(_t1253, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t1254, 1)), gem_string("\")")));
    gem_table_set(_t1253, gem_string("setup"), gem_string(""));
                GemVal _t1255 = _t1253;
                gem_pop_frame();
                return _t1255;
            } else {
#line 470 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("interp")))) {
#line 471 "compiler/main.gem"
    GemVal _t1256 = gem_v_node;
                    GemVal gem_v_parts = gem_table_get(_t1256, gem_string("parts"));
#line 472 "compiler/main.gem"
    GemVal _t1257[] = {gem_v_parts};
                    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1257, 1), gem_int(0)))) {
#line 473 "compiler/main.gem"
    GemVal _t1258 = gem_table_new();
    gem_table_set(_t1258, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t1258, gem_string("setup"), gem_string(""));
                        GemVal _t1259 = _t1258;
                        gem_pop_frame();
                        return _t1259;
                    }
#line 475 "compiler/main.gem"
                    GemVal gem_v_setup = gem_string("");
#line 476 "compiler/main.gem"
    GemVal _t1260 = gem_table_new();
                    GemVal gem_v_compiled = _t1260;
                    {
#line 477 "compiler/main.gem"
                        GemVal gem_v__for_i_19 = gem_int(0);
#line 477 "compiler/main.gem"
    GemVal _t1261[] = {gem_v_parts};
                        GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1261, 1);
#line 477 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 477 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_19;
#line 477 "compiler/main.gem"
                            gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 478 "compiler/main.gem"
                            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 479 "compiler/main.gem"
    GemVal _t1262 = (*gem_v_compile_expr);
    GemVal _t1263[] = {gem_v_part};
                            GemVal gem_v_r = _t1262.fn(_t1262.env, _t1263, 1);
#line 480 "compiler/main.gem"
    GemVal _t1264 = gem_v_r;
                            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1264, gem_string("setup")));
#line 481 "compiler/main.gem"
    GemVal _t1265 = gem_v_part;
                            if (gem_truthy(gem_eq(gem_table_get(_t1265, gem_string("tag")), gem_string("string")))) {
#line 482 "compiler/main.gem"
    GemVal _t1266 = gem_v_r;
    GemVal _t1267[] = {gem_v_compiled, gem_table_get(_t1266, gem_string("expr"))};
                                (void)(gem_push_fn(NULL, _t1267, 2));
                            } else {
#line 484 "compiler/main.gem"
    GemVal _t1268 = (*gem_v_tmp);
                                GemVal gem_v_ts = _t1268.fn(_t1268.env, NULL, 0);
#line 485 "compiler/main.gem"
    GemVal _t1269[] = {gem_v_ts};
    GemVal _t1270 = gem_v_r;
    GemVal _t1271[] = {gem_table_get(_t1270, gem_string("expr"))};
                                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1269, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1271, 1)), gem_string("};\n")));
#line 486 "compiler/main.gem"
    GemVal _t1272[] = {gem_v_ts};
    GemVal _t1273[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t1272, 1)), gem_string(", 1)"))};
                                (void)(gem_push_fn(NULL, _t1273, 2));
                            }
                        }
                    }
#line 489 "compiler/main.gem"
                    GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
                    {
#line 490 "compiler/main.gem"
                        GemVal gem_v__for_i_20 = gem_int(1);
#line 490 "compiler/main.gem"
    GemVal _t1274[] = {gem_v_compiled};
                        GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1274, 1);
#line 490 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 490 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_20;
#line 490 "compiler/main.gem"
                            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 491 "compiler/main.gem"
    GemVal _t1275[] = {gem_v_acc};
    GemVal _t1276[] = {gem_table_get(gem_v_compiled, gem_v_i)};
                            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1275, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1276, 1)), gem_string(")"));
                        }
                    }
#line 493 "compiler/main.gem"
    GemVal _t1277 = gem_table_new();
    gem_table_set(_t1277, gem_string("expr"), gem_v_acc);
    gem_table_set(_t1277, gem_string("setup"), gem_v_setup);
                    GemVal _t1278 = _t1277;
                    gem_pop_frame();
                    return _t1278;
                } else {
#line 494 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("bool")))) {
#line 495 "compiler/main.gem"
    GemVal _t1279 = gem_v_node;
                        if (gem_truthy(gem_table_get(_t1279, gem_string("value")))) {
#line 496 "compiler/main.gem"
    GemVal _t1280 = gem_table_new();
    gem_table_set(_t1280, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t1280, gem_string("setup"), gem_string(""));
                            GemVal _t1281 = _t1280;
                            gem_pop_frame();
                            return _t1281;
                        }
#line 498 "compiler/main.gem"
    GemVal _t1282 = gem_table_new();
    gem_table_set(_t1282, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t1282, gem_string("setup"), gem_string(""));
                        GemVal _t1283 = _t1282;
                        gem_pop_frame();
                        return _t1283;
                    } else {
#line 499 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("nil")))) {
#line 500 "compiler/main.gem"
    GemVal _t1284 = gem_table_new();
    gem_table_set(_t1284, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1284, gem_string("setup"), gem_string(""));
                            GemVal _t1285 = _t1284;
                            gem_pop_frame();
                            return _t1285;
                        } else {
#line 501 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 502 "compiler/main.gem"
    GemVal _t1286 = gem_v_node;
                                GemVal gem_v_name = gem_table_get(_t1286, gem_string("name"));
#line 503 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
#line 504 "compiler/main.gem"
    GemVal _t1287 = gem_table_new();
    gem_table_set(_t1287, gem_string("expr"), gem_string("gem_make_fn(gem_print, NULL)"));
    gem_table_set(_t1287, gem_string("setup"), gem_string(""));
                                    GemVal _t1288 = _t1287;
                                    gem_pop_frame();
                                    return _t1288;
                                } else {
#line 505 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 506 "compiler/main.gem"
    GemVal _t1289 = gem_table_new();
    gem_table_set(_t1289, gem_string("expr"), gem_string("gem_make_fn(gem_error_fn, NULL)"));
    gem_table_set(_t1289, gem_string("setup"), gem_string(""));
                                        GemVal _t1290 = _t1289;
                                        gem_pop_frame();
                                        return _t1290;
                                    } else {
#line 507 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("len")))) {
#line 508 "compiler/main.gem"
    GemVal _t1291 = gem_table_new();
    gem_table_set(_t1291, gem_string("expr"), gem_string("gem_make_fn(gem_len_fn, NULL)"));
    gem_table_set(_t1291, gem_string("setup"), gem_string(""));
                                            GemVal _t1292 = _t1291;
                                            gem_pop_frame();
                                            return _t1292;
                                        } else {
#line 509 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("type")))) {
#line 510 "compiler/main.gem"
    GemVal _t1293 = gem_table_new();
    gem_table_set(_t1293, gem_string("expr"), gem_string("gem_make_fn(gem_type_fn, NULL)"));
    gem_table_set(_t1293, gem_string("setup"), gem_string(""));
                                                GemVal _t1294 = _t1293;
                                                gem_pop_frame();
                                                return _t1294;
                                            } else {
#line 511 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("to_string")))) {
#line 512 "compiler/main.gem"
    GemVal _t1295 = gem_table_new();
    gem_table_set(_t1295, gem_string("expr"), gem_string("gem_make_fn(gem_to_string_fn, NULL)"));
    gem_table_set(_t1295, gem_string("setup"), gem_string(""));
                                                    GemVal _t1296 = _t1295;
                                                    gem_pop_frame();
                                                    return _t1296;
                                                } else {
#line 513 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("push")))) {
#line 514 "compiler/main.gem"
    GemVal _t1297 = gem_table_new();
    gem_table_set(_t1297, gem_string("expr"), gem_string("gem_make_fn(gem_push_fn, NULL)"));
    gem_table_set(_t1297, gem_string("setup"), gem_string(""));
                                                        GemVal _t1298 = _t1297;
                                                        gem_pop_frame();
                                                        return _t1298;
                                                    } else {
#line 515 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("pcall")))) {
#line 516 "compiler/main.gem"
    GemVal _t1299 = gem_table_new();
    gem_table_set(_t1299, gem_string("expr"), gem_string("gem_make_fn(gem_pcall_fn, NULL)"));
    gem_table_set(_t1299, gem_string("setup"), gem_string(""));
                                                            GemVal _t1300 = _t1299;
                                                            gem_pop_frame();
                                                            return _t1300;
                                                        } else {
#line 517 "compiler/main.gem"
                                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("keys")))) {
#line 518 "compiler/main.gem"
    GemVal _t1301 = gem_table_new();
    gem_table_set(_t1301, gem_string("expr"), gem_string("gem_make_fn(gem_keys_fn, NULL)"));
    gem_table_set(_t1301, gem_string("setup"), gem_string(""));
                                                                GemVal _t1302 = _t1301;
                                                                gem_pop_frame();
                                                                return _t1302;
                                                            } else {
#line 519 "compiler/main.gem"
                                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("str_replace")))) {
#line 520 "compiler/main.gem"
    GemVal _t1303 = gem_table_new();
    gem_table_set(_t1303, gem_string("expr"), gem_string("gem_make_fn(gem_str_replace_fn, NULL)"));
    gem_table_set(_t1303, gem_string("setup"), gem_string(""));
                                                                    GemVal _t1304 = _t1303;
                                                                    gem_pop_frame();
                                                                    return _t1304;
                                                                } else {
#line 521 "compiler/main.gem"
                                                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("has_key")))) {
#line 522 "compiler/main.gem"
    GemVal _t1305 = gem_table_new();
    gem_table_set(_t1305, gem_string("expr"), gem_string("gem_make_fn(gem_has_key_fn, NULL)"));
    gem_table_set(_t1305, gem_string("setup"), gem_string(""));
                                                                        GemVal _t1306 = _t1305;
                                                                        gem_pop_frame();
                                                                        return _t1306;
                                                                    } else {
#line 523 "compiler/main.gem"
    GemVal _t1307[] = {(*gem_v_defined_fns), gem_v_name};
                                                                        if (gem_truthy(gem_fn_set_contains(NULL, _t1307, 2))) {
#line 524 "compiler/main.gem"
    GemVal _t1308 = gem_table_new();
    GemVal _t1309[] = {gem_v_name};
    gem_table_set(_t1308, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t1309, 1)), gem_string(", NULL)")));
    gem_table_set(_t1308, gem_string("setup"), gem_string(""));
                                                                            GemVal _t1310 = _t1308;
                                                                            gem_pop_frame();
                                                                            return _t1310;
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
#line 526 "compiler/main.gem"
    GemVal _t1311 = (*gem_v_mangle);
    GemVal _t1312[] = {gem_v_name};
                                GemVal gem_v_mname = _t1311.fn(_t1311.env, _t1312, 1);
#line 527 "compiler/main.gem"
    GemVal _t1313[] = {(*gem_v_boxed_vars), gem_v_name};
                                if (gem_truthy(gem_fn_set_contains(NULL, _t1313, 2))) {
#line 528 "compiler/main.gem"
    GemVal _t1314 = gem_table_new();
    GemVal _t1315[] = {gem_v_mname};
    gem_table_set(_t1314, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t1315, 1)), gem_string(")")));
    gem_table_set(_t1314, gem_string("setup"), gem_string(""));
                                    GemVal _t1316 = _t1314;
                                    gem_pop_frame();
                                    return _t1316;
                                }
#line 530 "compiler/main.gem"
    GemVal _t1317 = gem_table_new();
    gem_table_set(_t1317, gem_string("expr"), gem_v_mname);
    gem_table_set(_t1317, gem_string("setup"), gem_string(""));
                                GemVal _t1318 = _t1317;
                                gem_pop_frame();
                                return _t1318;
                            } else {
#line 531 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 532 "compiler/main.gem"
    GemVal _t1319 = (*gem_v_compile_call);
    GemVal _t1320[] = {gem_v_node};
                                    GemVal _t1321 = _t1319.fn(_t1319.env, _t1320, 1);
                                    gem_pop_frame();
                                    return _t1321;
                                } else {
#line 533 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("binop")))) {
#line 534 "compiler/main.gem"
    GemVal _t1322 = (*gem_v_compile_binop);
    GemVal _t1323[] = {gem_v_node};
                                        GemVal _t1324 = _t1322.fn(_t1322.env, _t1323, 1);
                                        gem_pop_frame();
                                        return _t1324;
                                    } else {
#line 535 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("unop")))) {
#line 536 "compiler/main.gem"
    GemVal _t1325 = gem_v_node;
    GemVal _t1326 = (*gem_v_compile_expr);
    GemVal _t1327[] = {gem_table_get(_t1325, gem_string("expr"))};
                                            GemVal gem_v_r = _t1326.fn(_t1326.env, _t1327, 1);
#line 537 "compiler/main.gem"
    GemVal _t1328 = gem_v_node;
                                            if (gem_truthy(gem_eq(gem_table_get(_t1328, gem_string("op")), gem_string("-")))) {
#line 538 "compiler/main.gem"
    GemVal _t1329 = gem_table_new();
    GemVal _t1330 = gem_v_r;
    GemVal _t1331[] = {gem_table_get(_t1330, gem_string("expr"))};
    gem_table_set(_t1329, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t1331, 1)), gem_string(")")));
    GemVal _t1332 = gem_v_r;
    gem_table_set(_t1329, gem_string("setup"), gem_table_get(_t1332, gem_string("setup")));
                                                GemVal _t1333 = _t1329;
                                                gem_pop_frame();
                                                return _t1333;
                                            } else {
#line 539 "compiler/main.gem"
    GemVal _t1334 = gem_v_node;
                                                if (gem_truthy(gem_eq(gem_table_get(_t1334, gem_string("op")), gem_string("not")))) {
#line 540 "compiler/main.gem"
    GemVal _t1335 = gem_table_new();
    GemVal _t1336 = gem_v_r;
    GemVal _t1337[] = {gem_table_get(_t1336, gem_string("expr"))};
    gem_table_set(_t1335, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t1337, 1)), gem_string(")")));
    GemVal _t1338 = gem_v_r;
    gem_table_set(_t1335, gem_string("setup"), gem_table_get(_t1338, gem_string("setup")));
                                                    GemVal _t1339 = _t1335;
                                                    gem_pop_frame();
                                                    return _t1339;
                                                }
                                            }
                                        } else {
#line 542 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 543 "compiler/main.gem"
    GemVal _t1340 = (*gem_v_compile_anon_fn);
    GemVal _t1341[] = {gem_v_node};
                                                GemVal _t1342 = _t1340.fn(_t1340.env, _t1341, 1);
                                                gem_pop_frame();
                                                return _t1342;
                                            } else {
#line 544 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("table")))) {
#line 545 "compiler/main.gem"
    GemVal _t1343 = (*gem_v_compile_table);
    GemVal _t1344[] = {gem_v_node};
                                                    GemVal _t1345 = _t1343.fn(_t1343.env, _t1344, 1);
                                                    gem_pop_frame();
                                                    return _t1345;
                                                } else {
#line 546 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("array")))) {
#line 547 "compiler/main.gem"
    GemVal _t1346 = (*gem_v_compile_array);
    GemVal _t1347[] = {gem_v_node};
                                                        GemVal _t1348 = _t1346.fn(_t1346.env, _t1347, 1);
                                                        gem_pop_frame();
                                                        return _t1348;
                                                    } else {
#line 548 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot")))) {
#line 549 "compiler/main.gem"
    GemVal _t1349 = gem_v_node;
    GemVal _t1350 = (*gem_v_compile_expr);
    GemVal _t1351[] = {gem_table_get(_t1349, gem_string("object"))};
                                                            GemVal gem_v_r = _t1350.fn(_t1350.env, _t1351, 1);
#line 550 "compiler/main.gem"
    GemVal _t1352 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1352.fn(_t1352.env, NULL, 0);
#line 551 "compiler/main.gem"
    GemVal _t1353 = gem_v_r;
    GemVal _t1354[] = {gem_v_t};
    GemVal _t1355 = gem_v_r;
    GemVal _t1356[] = {gem_table_get(_t1355, gem_string("expr"))};
                                                            GemVal gem_v_setup = gem_add(gem_table_get(_t1353, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1354, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1356, 1)), gem_string(";\n")));
#line 552 "compiler/main.gem"
    GemVal _t1357 = gem_v_node;
    GemVal _t1358[] = {gem_table_get(_t1357, gem_string("field"))};
                                                            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1358, 1);
#line 553 "compiler/main.gem"
    GemVal _t1359 = gem_table_new();
    GemVal _t1360[] = {gem_v_t};
    GemVal _t1361[] = {gem_v_escaped};
    gem_table_set(_t1359, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1360, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1361, 1)), gem_string("\"))")));
    gem_table_set(_t1359, gem_string("setup"), gem_v_setup);
                                                            GemVal _t1362 = _t1359;
                                                            gem_pop_frame();
                                                            return _t1362;
                                                        } else {
#line 554 "compiler/main.gem"
                                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("index")))) {
#line 555 "compiler/main.gem"
    GemVal _t1363 = gem_v_node;
    GemVal _t1364 = (*gem_v_compile_expr);
    GemVal _t1365[] = {gem_table_get(_t1363, gem_string("object"))};
                                                                GemVal gem_v_obj_r = _t1364.fn(_t1364.env, _t1365, 1);
#line 556 "compiler/main.gem"
    GemVal _t1366 = gem_v_node;
    GemVal _t1367 = (*gem_v_compile_expr);
    GemVal _t1368[] = {gem_table_get(_t1366, gem_string("key"))};
                                                                GemVal gem_v_key_r = _t1367.fn(_t1367.env, _t1368, 1);
#line 557 "compiler/main.gem"
    GemVal _t1369 = gem_table_new();
    GemVal _t1370 = gem_v_obj_r;
    GemVal _t1371[] = {gem_table_get(_t1370, gem_string("expr"))};
    GemVal _t1372 = gem_v_key_r;
    GemVal _t1373[] = {gem_table_get(_t1372, gem_string("expr"))};
    gem_table_set(_t1369, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1371, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1373, 1)), gem_string(")")));
    GemVal _t1374 = gem_v_obj_r;
    GemVal _t1375 = gem_v_key_r;
    gem_table_set(_t1369, gem_string("setup"), gem_add(gem_table_get(_t1374, gem_string("setup")), gem_table_get(_t1375, gem_string("setup"))));
                                                                GemVal _t1376 = _t1369;
                                                                gem_pop_frame();
                                                                return _t1376;
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
    GemVal _t1377 = gem_table_new();
    gem_table_set(_t1377, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1377, gem_string("setup"), gem_add(gem_add(gem_string("/* unknown node: "), gem_v_tag), gem_string(" */\n")));
    GemVal _t1378 = _t1377;
    gem_pop_frame();
    return _t1378;
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
    gem_push_frame("_anon_31", "compiler/main.gem", 0);
#line 566 "compiler/main.gem"
    GemVal _t1380 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t1380, gem_string("entries"));
#line 567 "compiler/main.gem"
    GemVal _t1381 = (*gem_v_tmp);
    GemVal gem_v_t = _t1381.fn(_t1381.env, NULL, 0);
#line 568 "compiler/main.gem"
    GemVal _t1382[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1382, 1)), gem_string(" = gem_table_new();\n"));
    {
#line 569 "compiler/main.gem"
        GemVal gem_v__for_items_21 = gem_v_entries;
#line 569 "compiler/main.gem"
        GemVal gem_v__for_i_21 = gem_int(0);
#line 569 "compiler/main.gem"
        while (1) {
            GemVal _t1383[] = {gem_v__for_items_21};
            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t1383, 1)))) break;
#line 569 "compiler/main.gem"
            GemVal gem_v_entry = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
#line 569 "compiler/main.gem"
            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 570 "compiler/main.gem"
    GemVal _t1384 = gem_v_entry;
    GemVal _t1385 = (*gem_v_compile_expr);
    GemVal _t1386[] = {gem_table_get(_t1384, gem_string("value"))};
            GemVal gem_v_val_r = _t1385.fn(_t1385.env, _t1386, 1);
#line 571 "compiler/main.gem"
    GemVal _t1387 = gem_v_entry;
    GemVal _t1388[] = {gem_table_get(_t1387, gem_string("key"))};
            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1388, 1);
#line 572 "compiler/main.gem"
    GemVal _t1389 = gem_v_val_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1389, gem_string("setup")));
#line 573 "compiler/main.gem"
    GemVal _t1390[] = {gem_v_t};
    GemVal _t1391[] = {gem_v_escaped};
    GemVal _t1392 = gem_v_val_r;
    GemVal _t1393[] = {gem_table_get(_t1392, gem_string("expr"))};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1390, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1391, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1393, 1)), gem_string(");\n")));
        }
    }
    GemVal _t1394 = gem_table_new();
    gem_table_set(_t1394, gem_string("expr"), gem_v_t);
    gem_table_set(_t1394, gem_string("setup"), gem_v_setup);
    GemVal _t1395 = _t1394;
    gem_pop_frame();
    return _t1395;
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
    gem_push_frame("_anon_32", "compiler/main.gem", 0);
#line 581 "compiler/main.gem"
    GemVal _t1397 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t1397, gem_string("elements"));
#line 582 "compiler/main.gem"
    GemVal _t1398 = (*gem_v_tmp);
    GemVal gem_v_t = _t1398.fn(_t1398.env, NULL, 0);
#line 583 "compiler/main.gem"
    GemVal _t1399[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1399, 1)), gem_string(" = gem_table_new();\n"));
    {
#line 584 "compiler/main.gem"
        GemVal gem_v__for_i_22 = gem_int(0);
#line 584 "compiler/main.gem"
    GemVal _t1400[] = {gem_v_elements};
        GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1400, 1);
#line 584 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 584 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_22;
#line 584 "compiler/main.gem"
            gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 585 "compiler/main.gem"
    GemVal _t1401 = (*gem_v_compile_expr);
    GemVal _t1402[] = {gem_table_get(gem_v_elements, gem_v_i)};
            GemVal gem_v_elem_r = _t1401.fn(_t1401.env, _t1402, 1);
#line 586 "compiler/main.gem"
    GemVal _t1403 = gem_v_elem_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1403, gem_string("setup")));
#line 587 "compiler/main.gem"
    GemVal _t1404[] = {gem_v_t};
    GemVal _t1405[] = {gem_v_i};
    GemVal _t1406 = gem_v_elem_r;
    GemVal _t1407[] = {gem_table_get(_t1406, gem_string("expr"))};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1404, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t1405, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t1407, 1)), gem_string(");\n")));
        }
    }
    GemVal _t1408 = gem_table_new();
    gem_table_set(_t1408, gem_string("expr"), gem_v_t);
    gem_table_set(_t1408, gem_string("setup"), gem_v_setup);
    GemVal _t1409 = _t1408;
    gem_pop_frame();
    return _t1409;
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
    gem_push_frame("_anon_33", "compiler/main.gem", 0);
#line 595 "compiler/main.gem"
    GemVal _t1411 = gem_v_node;
    GemVal _t1412[] = {gem_table_get(_t1411, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1412, 1);
#line 596 "compiler/main.gem"
    GemVal _t1413 = gem_v_node;
    GemVal _t1414 = (*gem_v_collect_free_vars);
    GemVal _t1415[] = {gem_table_get(_t1413, gem_string("body")), gem_v_inner_defined};
    GemVal gem_v_free = _t1414.fn(_t1414.env, _t1415, 2);
#line 597 "compiler/main.gem"
    GemVal _t1416[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t1416, 1);
#line 599 "compiler/main.gem"
    GemVal _t1417[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1417, 1), gem_int(0)))) {
#line 600 "compiler/main.gem"
    GemVal _t1418 = gem_table_new();
    GemVal _t1419 = (*gem_v_compile_closure_fn);
    GemVal _t1420[] = {gem_v_node, _t1418};
        GemVal gem_v_result = _t1419.fn(_t1419.env, _t1420, 2);
#line 601 "compiler/main.gem"
    GemVal _t1421 = gem_table_new();
    GemVal _t1422 = gem_v_result;
    GemVal _t1423[] = {gem_table_get(_t1422, gem_string("fn_name"))};
    gem_table_set(_t1421, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1423, 1)), gem_string(", NULL)")));
    gem_table_set(_t1421, gem_string("setup"), gem_string(""));
        GemVal _t1424 = _t1421;
        gem_pop_frame();
        return _t1424;
    }
#line 604 "compiler/main.gem"
    GemVal _t1425 = (*gem_v_compile_closure_fn);
    GemVal _t1426[] = {gem_v_node, gem_v_captures};
    GemVal gem_v_result = _t1425.fn(_t1425.env, _t1426, 2);
#line 606 "compiler/main.gem"
    GemVal gem_v_setup = gem_string("");
#line 607 "compiler/main.gem"
    GemVal _t1427 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t1427.fn(_t1427.env, NULL, 0);
#line 608 "compiler/main.gem"
    GemVal _t1428 = gem_v_result;
    GemVal _t1429[] = {gem_table_get(_t1428, gem_string("struct_name"))};
    GemVal _t1430[] = {gem_v_env_tmp};
    GemVal _t1431 = gem_v_result;
    GemVal _t1432[] = {gem_table_get(_t1431, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t1429, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t1430, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t1432, 1)), gem_string("));\n")));
    {
#line 609 "compiler/main.gem"
        GemVal gem_v__for_items_23 = gem_v_captures;
#line 609 "compiler/main.gem"
        GemVal gem_v__for_i_23 = gem_int(0);
#line 609 "compiler/main.gem"
        while (1) {
            GemVal _t1433[] = {gem_v__for_items_23};
            if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_len_fn(NULL, _t1433, 1)))) break;
#line 609 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_23, gem_v__for_i_23);
#line 609 "compiler/main.gem"
            gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 610 "compiler/main.gem"
    GemVal _t1434 = (*gem_v_mangle);
    GemVal _t1435[] = {gem_v_cap};
            GemVal gem_v_mc = _t1434.fn(_t1434.env, _t1435, 1);
#line 611 "compiler/main.gem"
    GemVal _t1436[] = {(*gem_v_boxed_vars), gem_v_cap};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1436, 2))) {
#line 612 "compiler/main.gem"
    GemVal _t1437[] = {gem_v_env_tmp};
    GemVal _t1438[] = {gem_v_mc};
    GemVal _t1439[] = {gem_v_mc};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1437, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1438, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1439, 1)), gem_string(";\n")));
            } else {
#line 614 "compiler/main.gem"
    GemVal _t1440[] = {gem_v_env_tmp};
    GemVal _t1441[] = {gem_v_mc};
    GemVal _t1442[] = {gem_v_mc};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1440, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1441, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t1442, 1)), gem_string(";\n")));
            }
        }
    }
    GemVal _t1443 = gem_table_new();
    GemVal _t1444 = gem_v_result;
    GemVal _t1445[] = {gem_table_get(_t1444, gem_string("fn_name"))};
    GemVal _t1446[] = {gem_v_env_tmp};
    gem_table_set(_t1443, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1445, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1446, 1)), gem_string(")")));
    gem_table_set(_t1443, gem_string("setup"), gem_v_setup);
    GemVal _t1447 = _t1443;
    gem_pop_frame();
    return _t1447;
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
    gem_push_frame("_anon_34", "compiler/main.gem", 0);
#line 623 "compiler/main.gem"
    GemVal _t1449 = gem_v_node;
    GemVal gem_v_func = gem_table_get(_t1449, gem_string("func"));
#line 624 "compiler/main.gem"
    GemVal _t1450 = gem_v_node;
    GemVal gem_v_args = gem_table_get(_t1450, gem_string("args"));
#line 627 "compiler/main.gem"
    GemVal _t1451 = gem_v_func;
    GemVal _t1453;
    if (!gem_truthy(gem_eq(gem_table_get(_t1451, gem_string("tag")), gem_string("dot")))) {
        _t1453 = gem_eq(gem_table_get(_t1451, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t1452 = gem_v_func;
        _t1453 = gem_eq(gem_table_get(_t1452, gem_string("field")), gem_string("len"));
    }
    GemVal _t1455;
    if (!gem_truthy(_t1453)) {
        _t1455 = _t1453;
    } else {
        GemVal _t1454[] = {gem_v_args};
        _t1455 = gem_eq(gem_len_fn(NULL, _t1454, 1), gem_int(0));
    }
    if (gem_truthy(_t1455)) {
#line 628 "compiler/main.gem"
    GemVal _t1456 = gem_v_func;
    GemVal _t1457 = (*gem_v_compile_expr);
    GemVal _t1458[] = {gem_table_get(_t1456, gem_string("object"))};
        GemVal gem_v_obj_r = _t1457.fn(_t1457.env, _t1458, 1);
#line 629 "compiler/main.gem"
    GemVal _t1459 = gem_table_new();
    GemVal _t1460 = gem_v_obj_r;
    GemVal _t1461[] = {gem_table_get(_t1460, gem_string("expr"))};
    gem_table_set(_t1459, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t1461, 1)), gem_string(")")));
    GemVal _t1462 = gem_v_obj_r;
    gem_table_set(_t1459, gem_string("setup"), gem_table_get(_t1462, gem_string("setup")));
        GemVal _t1463 = _t1459;
        gem_pop_frame();
        return _t1463;
    }
#line 633 "compiler/main.gem"
    GemVal _t1464 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t1464, gem_string("tag")), gem_string("var")))) {
#line 634 "compiler/main.gem"
    GemVal _t1465 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t1465, gem_string("name"));
#line 636 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 637 "compiler/main.gem"
    GemVal _t1466 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t1466, gem_string("line"));
#line 638 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 639 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 641 "compiler/main.gem"
    GemVal _t1467[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t1467, 1);
#line 642 "compiler/main.gem"
            GemVal gem_v_arg_setups = gem_string("");
#line 643 "compiler/main.gem"
    GemVal _t1468 = gem_table_new();
            GemVal gem_v_arg_exprs = _t1468;
            {
#line 644 "compiler/main.gem"
                GemVal gem_v__for_i_24 = gem_int(0);
#line 644 "compiler/main.gem"
    GemVal _t1469[] = {gem_v_args};
                GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1469, 1);
#line 644 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 644 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_24;
#line 644 "compiler/main.gem"
                    gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 645 "compiler/main.gem"
    GemVal _t1470 = (*gem_v_compile_expr);
    GemVal _t1471[] = {gem_table_get(gem_v_args, gem_v_i)};
                    GemVal gem_v_r = _t1470.fn(_t1470.env, _t1471, 1);
#line 646 "compiler/main.gem"
    GemVal _t1472 = gem_v_r;
                    gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1472, gem_string("setup")));
#line 647 "compiler/main.gem"
    GemVal _t1473 = gem_v_r;
    GemVal _t1474[] = {gem_v_arg_exprs, gem_table_get(_t1473, gem_string("expr"))};
                    (void)(gem_push_fn(NULL, _t1474, 2));
                }
            }
#line 649 "compiler/main.gem"
    GemVal _t1475[] = {gem_v_args};
            GemVal gem_v_argc = gem_len_fn(NULL, _t1475, 1);
#line 650 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 651 "compiler/main.gem"
    GemVal _t1476 = gem_table_new();
    GemVal _t1477[] = {gem_v_escaped_file};
    GemVal _t1478[] = {gem_v_line};
    gem_table_set(_t1476, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1477, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1478, 1)), gem_string(", NULL, 0)")));
    gem_table_set(_t1476, gem_string("setup"), gem_v_arg_setups);
                GemVal _t1479 = _t1476;
                gem_pop_frame();
                return _t1479;
            }
#line 653 "compiler/main.gem"
    GemVal _t1480 = (*gem_v_tmp);
            GemVal gem_v_t = _t1480.fn(_t1480.env, NULL, 0);
#line 654 "compiler/main.gem"
            GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
            {
#line 655 "compiler/main.gem"
                GemVal gem_v__for_i_25 = gem_int(1);
#line 655 "compiler/main.gem"
                GemVal gem_v__for_limit_25 = gem_v_argc;
#line 655 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 655 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_25;
#line 655 "compiler/main.gem"
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 656 "compiler/main.gem"
                    gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                }
            }
#line 658 "compiler/main.gem"
    GemVal _t1481[] = {gem_v_t};
    GemVal _t1482[] = {gem_v_arr};
            GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1481, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1482, 1)), gem_string("};\n")));
#line 659 "compiler/main.gem"
    GemVal _t1483[] = {gem_v_escaped_file};
    GemVal _t1484[] = {gem_v_line};
    GemVal _t1485[] = {gem_v_t};
    GemVal _t1486[] = {gem_v_argc};
            GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1483, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1484, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1485, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1486, 1)), gem_string(")"));
#line 660 "compiler/main.gem"
    GemVal _t1487 = gem_table_new();
    gem_table_set(_t1487, gem_string("expr"), gem_v_call);
    gem_table_set(_t1487, gem_string("setup"), gem_v_setup);
            GemVal _t1488 = _t1487;
            gem_pop_frame();
            return _t1488;
        } else {
#line 661 "compiler/main.gem"
    GemVal _t1489;
    if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
        _t1489 = gem_eq(gem_v_name, gem_string("print"));
    } else {
        _t1489 = gem_eq(gem_v_name, gem_string("len"));
    }
    GemVal _t1490;
    if (gem_truthy(_t1489)) {
        _t1490 = _t1489;
    } else {
        _t1490 = gem_eq(gem_v_name, gem_string("type"));
    }
    GemVal _t1491;
    if (gem_truthy(_t1490)) {
        _t1491 = _t1490;
    } else {
        _t1491 = gem_eq(gem_v_name, gem_string("to_string"));
    }
    GemVal _t1492;
    if (gem_truthy(_t1491)) {
        _t1492 = _t1491;
    } else {
        _t1492 = gem_eq(gem_v_name, gem_string("push"));
    }
    GemVal _t1493;
    if (gem_truthy(_t1492)) {
        _t1493 = _t1492;
    } else {
        _t1493 = gem_eq(gem_v_name, gem_string("pcall"));
    }
    GemVal _t1494;
    if (gem_truthy(_t1493)) {
        _t1494 = _t1493;
    } else {
        _t1494 = gem_eq(gem_v_name, gem_string("keys"));
    }
    GemVal _t1495;
    if (gem_truthy(_t1494)) {
        _t1495 = _t1494;
    } else {
        _t1495 = gem_eq(gem_v_name, gem_string("str_replace"));
    }
    GemVal _t1496;
    if (gem_truthy(_t1495)) {
        _t1496 = _t1495;
    } else {
        _t1496 = gem_eq(gem_v_name, gem_string("has_key"));
    }
            if (gem_truthy(_t1496)) {
#line 662 "compiler/main.gem"
                GemVal gem_v_fn_name = gem_string("");
#line 663 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
#line 664 "compiler/main.gem"
                    gem_v_fn_name = gem_string("gem_print");
                } else {
#line 665 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_name, gem_string("len")))) {
#line 666 "compiler/main.gem"
                        gem_v_fn_name = gem_string("gem_len_fn");
                    } else {
#line 667 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_name, gem_string("type")))) {
#line 668 "compiler/main.gem"
                            gem_v_fn_name = gem_string("gem_type_fn");
                        } else {
#line 669 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_name, gem_string("to_string")))) {
#line 670 "compiler/main.gem"
                                gem_v_fn_name = gem_string("gem_to_string_fn");
                            } else {
#line 671 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_name, gem_string("push")))) {
#line 672 "compiler/main.gem"
                                    gem_v_fn_name = gem_string("gem_push_fn");
                                } else {
#line 673 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("pcall")))) {
#line 674 "compiler/main.gem"
                                        gem_v_fn_name = gem_string("gem_pcall_fn");
                                    } else {
#line 675 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("keys")))) {
#line 676 "compiler/main.gem"
                                            gem_v_fn_name = gem_string("gem_keys_fn");
                                        } else {
#line 677 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("str_replace")))) {
#line 678 "compiler/main.gem"
                                                gem_v_fn_name = gem_string("gem_str_replace_fn");
                                            } else {
#line 679 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("has_key")))) {
#line 680 "compiler/main.gem"
                                                    gem_v_fn_name = gem_string("gem_has_key_fn");
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
#line 682 "compiler/main.gem"
                GemVal gem_v_arg_setups = gem_string("");
#line 683 "compiler/main.gem"
    GemVal _t1497 = gem_table_new();
                GemVal gem_v_arg_exprs = _t1497;
                {
#line 684 "compiler/main.gem"
                    GemVal gem_v__for_i_26 = gem_int(0);
#line 684 "compiler/main.gem"
    GemVal _t1498[] = {gem_v_args};
                    GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1498, 1);
#line 684 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 684 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_26;
#line 684 "compiler/main.gem"
                        gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 685 "compiler/main.gem"
    GemVal _t1499 = (*gem_v_compile_expr);
    GemVal _t1500[] = {gem_table_get(gem_v_args, gem_v_i)};
                        GemVal gem_v_r = _t1499.fn(_t1499.env, _t1500, 1);
#line 686 "compiler/main.gem"
    GemVal _t1501 = gem_v_r;
                        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1501, gem_string("setup")));
#line 687 "compiler/main.gem"
    GemVal _t1502 = gem_v_r;
    GemVal _t1503[] = {gem_v_arg_exprs, gem_table_get(_t1502, gem_string("expr"))};
                        (void)(gem_push_fn(NULL, _t1503, 2));
                    }
                }
#line 689 "compiler/main.gem"
    GemVal _t1504[] = {gem_v_args};
                GemVal gem_v_argc = gem_len_fn(NULL, _t1504, 1);
#line 690 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 691 "compiler/main.gem"
    GemVal _t1505 = gem_table_new();
    GemVal _t1506[] = {gem_v_fn_name};
    gem_table_set(_t1505, gem_string("expr"), gem_add(gem_to_string_fn(NULL, _t1506, 1), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1505, gem_string("setup"), gem_v_arg_setups);
                    GemVal _t1507 = _t1505;
                    gem_pop_frame();
                    return _t1507;
                }
#line 693 "compiler/main.gem"
    GemVal _t1508 = (*gem_v_tmp);
                GemVal gem_v_t = _t1508.fn(_t1508.env, NULL, 0);
#line 694 "compiler/main.gem"
                GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
                {
#line 695 "compiler/main.gem"
                    GemVal gem_v__for_i_27 = gem_int(1);
#line 695 "compiler/main.gem"
                    GemVal gem_v__for_limit_27 = gem_v_argc;
#line 695 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 695 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_27;
#line 695 "compiler/main.gem"
                        gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 696 "compiler/main.gem"
                        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                    }
                }
#line 698 "compiler/main.gem"
    GemVal _t1509[] = {gem_v_t};
    GemVal _t1510[] = {gem_v_arr};
                GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1509, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1510, 1)), gem_string("};\n")));
#line 699 "compiler/main.gem"
    GemVal _t1511 = gem_table_new();
    GemVal _t1512[] = {gem_v_fn_name};
    GemVal _t1513[] = {gem_v_t};
    GemVal _t1514[] = {gem_v_argc};
    gem_table_set(_t1511, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1512, 1), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1513, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1514, 1)), gem_string(")")));
    gem_table_set(_t1511, gem_string("setup"), gem_v_setup);
                GemVal _t1515 = _t1511;
                gem_pop_frame();
                return _t1515;
            } else {
#line 700 "compiler/main.gem"
    GemVal _t1516[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t1516, 2))) {
#line 701 "compiler/main.gem"
                    GemVal gem_v_arg_setups = gem_string("");
#line 702 "compiler/main.gem"
    GemVal _t1517 = gem_table_new();
                    GemVal gem_v_arg_exprs = _t1517;
                    {
#line 703 "compiler/main.gem"
                        GemVal gem_v__for_i_28 = gem_int(0);
#line 703 "compiler/main.gem"
    GemVal _t1518[] = {gem_v_args};
                        GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1518, 1);
#line 703 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 703 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_28;
#line 703 "compiler/main.gem"
                            gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 704 "compiler/main.gem"
    GemVal _t1519 = (*gem_v_compile_expr);
    GemVal _t1520[] = {gem_table_get(gem_v_args, gem_v_i)};
                            GemVal gem_v_r = _t1519.fn(_t1519.env, _t1520, 1);
#line 705 "compiler/main.gem"
    GemVal _t1521 = gem_v_r;
                            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1521, gem_string("setup")));
#line 706 "compiler/main.gem"
    GemVal _t1522 = gem_v_r;
    GemVal _t1523[] = {gem_v_arg_exprs, gem_table_get(_t1522, gem_string("expr"))};
                            (void)(gem_push_fn(NULL, _t1523, 2));
                        }
                    }
#line 708 "compiler/main.gem"
    GemVal _t1524[] = {gem_v_args};
                    GemVal gem_v_argc = gem_len_fn(NULL, _t1524, 1);
#line 709 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 710 "compiler/main.gem"
    GemVal _t1525 = gem_table_new();
    GemVal _t1526[] = {gem_v_name};
    gem_table_set(_t1525, gem_string("expr"), gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1526, 1)), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1525, gem_string("setup"), gem_v_arg_setups);
                        GemVal _t1527 = _t1525;
                        gem_pop_frame();
                        return _t1527;
                    }
#line 712 "compiler/main.gem"
    GemVal _t1528 = (*gem_v_tmp);
                    GemVal gem_v_t = _t1528.fn(_t1528.env, NULL, 0);
#line 713 "compiler/main.gem"
                    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
                    {
#line 714 "compiler/main.gem"
                        GemVal gem_v__for_i_29 = gem_int(1);
#line 714 "compiler/main.gem"
                        GemVal gem_v__for_limit_29 = gem_v_argc;
#line 714 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_v__for_limit_29))) break;
#line 714 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_29;
#line 714 "compiler/main.gem"
                            gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 715 "compiler/main.gem"
                            gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                        }
                    }
#line 717 "compiler/main.gem"
    GemVal _t1529[] = {gem_v_t};
    GemVal _t1530[] = {gem_v_arr};
                    GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1529, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1530, 1)), gem_string("};\n")));
#line 718 "compiler/main.gem"
    GemVal _t1531 = gem_table_new();
    GemVal _t1532[] = {gem_v_name};
    GemVal _t1533[] = {gem_v_t};
    GemVal _t1534[] = {gem_v_argc};
    gem_table_set(_t1531, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1532, 1)), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1533, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1534, 1)), gem_string(")")));
    gem_table_set(_t1531, gem_string("setup"), gem_v_setup);
                    GemVal _t1535 = _t1531;
                    gem_pop_frame();
                    return _t1535;
                }
            }
        }
    }
#line 723 "compiler/main.gem"
    GemVal _t1536 = (*gem_v_compile_expr);
    GemVal _t1537[] = {gem_v_func};
    GemVal gem_v_func_r = _t1536.fn(_t1536.env, _t1537, 1);
#line 724 "compiler/main.gem"
    GemVal gem_v_arg_setups = gem_string("");
#line 725 "compiler/main.gem"
    GemVal _t1538 = gem_table_new();
    GemVal gem_v_arg_exprs = _t1538;
    {
#line 726 "compiler/main.gem"
        GemVal gem_v__for_i_30 = gem_int(0);
#line 726 "compiler/main.gem"
    GemVal _t1539[] = {gem_v_args};
        GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t1539, 1);
#line 726 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 726 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_30;
#line 726 "compiler/main.gem"
            gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 727 "compiler/main.gem"
    GemVal _t1540 = (*gem_v_compile_expr);
    GemVal _t1541[] = {gem_table_get(gem_v_args, gem_v_i)};
            GemVal gem_v_r = _t1540.fn(_t1540.env, _t1541, 1);
#line 728 "compiler/main.gem"
    GemVal _t1542 = gem_v_r;
            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1542, gem_string("setup")));
#line 729 "compiler/main.gem"
    GemVal _t1543 = gem_v_r;
    GemVal _t1544[] = {gem_v_arg_exprs, gem_table_get(_t1543, gem_string("expr"))};
            (void)(gem_push_fn(NULL, _t1544, 2));
        }
    }
#line 731 "compiler/main.gem"
    GemVal _t1545 = gem_v_func_r;
    GemVal gem_v_setup = gem_add(gem_table_get(_t1545, gem_string("setup")), gem_v_arg_setups);
#line 732 "compiler/main.gem"
    GemVal _t1546[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t1546, 1);
#line 733 "compiler/main.gem"
    GemVal _t1547 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t1547.fn(_t1547.env, NULL, 0);
#line 734 "compiler/main.gem"
    GemVal _t1548[] = {gem_v_tmp_fn};
    GemVal _t1549 = gem_v_func_r;
    GemVal _t1550[] = {gem_table_get(_t1549, gem_string("expr"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1548, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1550, 1)), gem_string(";\n")));
#line 735 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 736 "compiler/main.gem"
    GemVal _t1551 = gem_table_new();
    GemVal _t1552[] = {gem_v_tmp_fn};
    GemVal _t1553[] = {gem_v_tmp_fn};
    gem_table_set(_t1551, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1552, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1553, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t1551, gem_string("setup"), gem_v_setup);
        GemVal _t1554 = _t1551;
        gem_pop_frame();
        return _t1554;
    }
#line 738 "compiler/main.gem"
    GemVal _t1555 = (*gem_v_tmp);
    GemVal gem_v_tmp_args = _t1555.fn(_t1555.env, NULL, 0);
#line 739 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
    {
#line 740 "compiler/main.gem"
        GemVal gem_v__for_i_31 = gem_int(1);
#line 740 "compiler/main.gem"
        GemVal gem_v__for_limit_31 = gem_v_argc;
#line 740 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_v__for_limit_31))) break;
#line 740 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_31;
#line 740 "compiler/main.gem"
            gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
#line 741 "compiler/main.gem"
            gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
        }
    }
#line 743 "compiler/main.gem"
    GemVal _t1556[] = {gem_v_tmp_args};
    GemVal _t1557[] = {gem_v_arr};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1556, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1557, 1)), gem_string("};\n")));
#line 744 "compiler/main.gem"
    GemVal _t1558[] = {gem_v_tmp_fn};
    GemVal _t1559[] = {gem_v_tmp_fn};
    GemVal _t1560[] = {gem_v_tmp_args};
    GemVal _t1561[] = {gem_v_argc};
    GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1558, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1559, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t1560, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1561, 1)), gem_string(")"));
    GemVal _t1562 = gem_table_new();
    gem_table_set(_t1562, gem_string("expr"), gem_v_call);
    gem_table_set(_t1562, gem_string("setup"), gem_v_setup);
    GemVal _t1563 = _t1562;
    gem_pop_frame();
    return _t1563;
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
    gem_push_frame("_anon_35", "compiler/main.gem", 0);
#line 751 "compiler/main.gem"
    GemVal _t1565 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t1565, gem_string("op"));
#line 752 "compiler/main.gem"
    GemVal _t1566 = gem_v_node;
    GemVal _t1567 = (*gem_v_compile_expr);
    GemVal _t1568[] = {gem_table_get(_t1566, gem_string("left"))};
    GemVal gem_v_lc_r = _t1567.fn(_t1567.env, _t1568, 1);
#line 753 "compiler/main.gem"
    GemVal _t1569 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t1569, gem_string("expr"));
#line 754 "compiler/main.gem"
    GemVal _t1570 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t1570, gem_string("setup"));
#line 756 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 757 "compiler/main.gem"
    GemVal _t1571 = gem_v_node;
    GemVal _t1572 = (*gem_v_compile_expr);
    GemVal _t1573[] = {gem_table_get(_t1571, gem_string("right"))};
        GemVal gem_v_rc_r = _t1572.fn(_t1572.env, _t1573, 1);
#line 758 "compiler/main.gem"
    GemVal _t1574 = gem_table_new();
    GemVal _t1575[] = {gem_v_lc};
    GemVal _t1576 = gem_v_rc_r;
    GemVal _t1577[] = {gem_table_get(_t1576, gem_string("expr"))};
    gem_table_set(_t1574, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1575, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1577, 1)), gem_string(")")));
    GemVal _t1578 = gem_v_rc_r;
    gem_table_set(_t1574, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1578, gem_string("setup"))));
        GemVal _t1579 = _t1574;
        gem_pop_frame();
        return _t1579;
    } else {
#line 759 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 760 "compiler/main.gem"
    GemVal _t1580 = gem_v_node;
    GemVal _t1581 = (*gem_v_compile_expr);
    GemVal _t1582[] = {gem_table_get(_t1580, gem_string("right"))};
            GemVal gem_v_rc_r = _t1581.fn(_t1581.env, _t1582, 1);
#line 761 "compiler/main.gem"
    GemVal _t1583 = gem_table_new();
    GemVal _t1584[] = {gem_v_lc};
    GemVal _t1585 = gem_v_rc_r;
    GemVal _t1586[] = {gem_table_get(_t1585, gem_string("expr"))};
    gem_table_set(_t1583, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t1584, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1586, 1)), gem_string(")")));
    GemVal _t1587 = gem_v_rc_r;
    gem_table_set(_t1583, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1587, gem_string("setup"))));
            GemVal _t1588 = _t1583;
            gem_pop_frame();
            return _t1588;
        } else {
#line 762 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 763 "compiler/main.gem"
    GemVal _t1589 = gem_v_node;
    GemVal _t1590 = (*gem_v_compile_expr);
    GemVal _t1591[] = {gem_table_get(_t1589, gem_string("right"))};
                GemVal gem_v_rc_r = _t1590.fn(_t1590.env, _t1591, 1);
#line 764 "compiler/main.gem"
    GemVal _t1592 = gem_table_new();
    GemVal _t1593[] = {gem_v_lc};
    GemVal _t1594 = gem_v_rc_r;
    GemVal _t1595[] = {gem_table_get(_t1594, gem_string("expr"))};
    gem_table_set(_t1592, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t1593, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1595, 1)), gem_string(")")));
    GemVal _t1596 = gem_v_rc_r;
    gem_table_set(_t1592, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1596, gem_string("setup"))));
                GemVal _t1597 = _t1592;
                gem_pop_frame();
                return _t1597;
            } else {
#line 765 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 766 "compiler/main.gem"
    GemVal _t1598 = gem_v_node;
    GemVal _t1599 = (*gem_v_compile_expr);
    GemVal _t1600[] = {gem_table_get(_t1598, gem_string("right"))};
                    GemVal gem_v_rc_r = _t1599.fn(_t1599.env, _t1600, 1);
#line 767 "compiler/main.gem"
    GemVal _t1601 = gem_table_new();
    GemVal _t1602[] = {gem_v_lc};
    GemVal _t1603 = gem_v_rc_r;
    GemVal _t1604[] = {gem_table_get(_t1603, gem_string("expr"))};
    gem_table_set(_t1601, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t1602, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1604, 1)), gem_string(")")));
    GemVal _t1605 = gem_v_rc_r;
    gem_table_set(_t1601, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1605, gem_string("setup"))));
                    GemVal _t1606 = _t1601;
                    gem_pop_frame();
                    return _t1606;
                } else {
#line 768 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 769 "compiler/main.gem"
    GemVal _t1607 = gem_v_node;
    GemVal _t1608 = (*gem_v_compile_expr);
    GemVal _t1609[] = {gem_table_get(_t1607, gem_string("right"))};
                        GemVal gem_v_rc_r = _t1608.fn(_t1608.env, _t1609, 1);
#line 770 "compiler/main.gem"
    GemVal _t1610 = gem_table_new();
    GemVal _t1611[] = {gem_v_lc};
    GemVal _t1612 = gem_v_rc_r;
    GemVal _t1613[] = {gem_table_get(_t1612, gem_string("expr"))};
    gem_table_set(_t1610, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t1611, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1613, 1)), gem_string(")")));
    GemVal _t1614 = gem_v_rc_r;
    gem_table_set(_t1610, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1614, gem_string("setup"))));
                        GemVal _t1615 = _t1610;
                        gem_pop_frame();
                        return _t1615;
                    } else {
#line 771 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 772 "compiler/main.gem"
    GemVal _t1616 = gem_v_node;
    GemVal _t1617 = (*gem_v_compile_expr);
    GemVal _t1618[] = {gem_table_get(_t1616, gem_string("right"))};
                            GemVal gem_v_rc_r = _t1617.fn(_t1617.env, _t1618, 1);
#line 773 "compiler/main.gem"
    GemVal _t1619 = gem_table_new();
    GemVal _t1620[] = {gem_v_lc};
    GemVal _t1621 = gem_v_rc_r;
    GemVal _t1622[] = {gem_table_get(_t1621, gem_string("expr"))};
    gem_table_set(_t1619, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t1620, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1622, 1)), gem_string(")")));
    GemVal _t1623 = gem_v_rc_r;
    gem_table_set(_t1619, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1623, gem_string("setup"))));
                            GemVal _t1624 = _t1619;
                            gem_pop_frame();
                            return _t1624;
                        } else {
#line 774 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 775 "compiler/main.gem"
    GemVal _t1625 = gem_v_node;
    GemVal _t1626 = (*gem_v_compile_expr);
    GemVal _t1627[] = {gem_table_get(_t1625, gem_string("right"))};
                                GemVal gem_v_rc_r = _t1626.fn(_t1626.env, _t1627, 1);
#line 776 "compiler/main.gem"
    GemVal _t1628 = gem_table_new();
    GemVal _t1629[] = {gem_v_lc};
    GemVal _t1630 = gem_v_rc_r;
    GemVal _t1631[] = {gem_table_get(_t1630, gem_string("expr"))};
    gem_table_set(_t1628, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t1629, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1631, 1)), gem_string(")")));
    GemVal _t1632 = gem_v_rc_r;
    gem_table_set(_t1628, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1632, gem_string("setup"))));
                                GemVal _t1633 = _t1628;
                                gem_pop_frame();
                                return _t1633;
                            } else {
#line 777 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 778 "compiler/main.gem"
    GemVal _t1634 = gem_v_node;
    GemVal _t1635 = (*gem_v_compile_expr);
    GemVal _t1636[] = {gem_table_get(_t1634, gem_string("right"))};
                                    GemVal gem_v_rc_r = _t1635.fn(_t1635.env, _t1636, 1);
#line 779 "compiler/main.gem"
    GemVal _t1637 = gem_table_new();
    GemVal _t1638[] = {gem_v_lc};
    GemVal _t1639 = gem_v_rc_r;
    GemVal _t1640[] = {gem_table_get(_t1639, gem_string("expr"))};
    gem_table_set(_t1637, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t1638, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1640, 1)), gem_string(")")));
    GemVal _t1641 = gem_v_rc_r;
    gem_table_set(_t1637, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1641, gem_string("setup"))));
                                    GemVal _t1642 = _t1637;
                                    gem_pop_frame();
                                    return _t1642;
                                } else {
#line 780 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 781 "compiler/main.gem"
    GemVal _t1643 = gem_v_node;
    GemVal _t1644 = (*gem_v_compile_expr);
    GemVal _t1645[] = {gem_table_get(_t1643, gem_string("right"))};
                                        GemVal gem_v_rc_r = _t1644.fn(_t1644.env, _t1645, 1);
#line 782 "compiler/main.gem"
    GemVal _t1646 = gem_table_new();
    GemVal _t1647[] = {gem_v_lc};
    GemVal _t1648 = gem_v_rc_r;
    GemVal _t1649[] = {gem_table_get(_t1648, gem_string("expr"))};
    gem_table_set(_t1646, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t1647, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1649, 1)), gem_string(")")));
    GemVal _t1650 = gem_v_rc_r;
    gem_table_set(_t1646, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1650, gem_string("setup"))));
                                        GemVal _t1651 = _t1646;
                                        gem_pop_frame();
                                        return _t1651;
                                    } else {
#line 783 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 784 "compiler/main.gem"
    GemVal _t1652 = gem_v_node;
    GemVal _t1653 = (*gem_v_compile_expr);
    GemVal _t1654[] = {gem_table_get(_t1652, gem_string("right"))};
                                            GemVal gem_v_rc_r = _t1653.fn(_t1653.env, _t1654, 1);
#line 785 "compiler/main.gem"
    GemVal _t1655 = gem_table_new();
    GemVal _t1656[] = {gem_v_lc};
    GemVal _t1657 = gem_v_rc_r;
    GemVal _t1658[] = {gem_table_get(_t1657, gem_string("expr"))};
    gem_table_set(_t1655, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t1656, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1658, 1)), gem_string(")")));
    GemVal _t1659 = gem_v_rc_r;
    gem_table_set(_t1655, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1659, gem_string("setup"))));
                                            GemVal _t1660 = _t1655;
                                            gem_pop_frame();
                                            return _t1660;
                                        } else {
#line 786 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 787 "compiler/main.gem"
    GemVal _t1661 = gem_v_node;
    GemVal _t1662 = (*gem_v_compile_expr);
    GemVal _t1663[] = {gem_table_get(_t1661, gem_string("right"))};
                                                GemVal gem_v_rc_r = _t1662.fn(_t1662.env, _t1663, 1);
#line 788 "compiler/main.gem"
    GemVal _t1664 = gem_table_new();
    GemVal _t1665[] = {gem_v_lc};
    GemVal _t1666 = gem_v_rc_r;
    GemVal _t1667[] = {gem_table_get(_t1666, gem_string("expr"))};
    gem_table_set(_t1664, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t1665, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1667, 1)), gem_string(")")));
    GemVal _t1668 = gem_v_rc_r;
    gem_table_set(_t1664, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1668, gem_string("setup"))));
                                                GemVal _t1669 = _t1664;
                                                gem_pop_frame();
                                                return _t1669;
                                            } else {
#line 789 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 790 "compiler/main.gem"
    GemVal _t1670 = gem_v_node;
    GemVal _t1671 = (*gem_v_compile_expr);
    GemVal _t1672[] = {gem_table_get(_t1670, gem_string("right"))};
                                                    GemVal gem_v_rc_r = _t1671.fn(_t1671.env, _t1672, 1);
#line 791 "compiler/main.gem"
    GemVal _t1673 = (*gem_v_tmp);
                                                    GemVal gem_v_t = _t1673.fn(_t1673.env, NULL, 0);
#line 792 "compiler/main.gem"
                                                    GemVal gem_v_setup = gem_v_ls;
#line 793 "compiler/main.gem"
    GemVal _t1674[] = {gem_v_t};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1674, 1)), gem_string(";\n")));
#line 794 "compiler/main.gem"
    GemVal _t1675[] = {gem_v_lc};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t1675, 1)), gem_string(")) {\n")));
#line 795 "compiler/main.gem"
    GemVal _t1676[] = {gem_v_t};
    GemVal _t1677[] = {gem_v_lc};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1676, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1677, 1)), gem_string(";\n")));
#line 796 "compiler/main.gem"
                                                    gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 797 "compiler/main.gem"
    GemVal _t1678 = gem_v_rc_r;
                                                    if (gem_truthy(gem_neq(gem_table_get(_t1678, gem_string("setup")), gem_string("")))) {
#line 798 "compiler/main.gem"
    GemVal _t1679 = gem_v_rc_r;
    GemVal _t1680[] = {gem_table_get(_t1679, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t1680, 3));
                                                    }
#line 800 "compiler/main.gem"
    GemVal _t1681[] = {gem_v_t};
    GemVal _t1682 = gem_v_rc_r;
    GemVal _t1683[] = {gem_table_get(_t1682, gem_string("expr"))};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1681, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1683, 1)), gem_string(";\n")));
#line 801 "compiler/main.gem"
                                                    gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 802 "compiler/main.gem"
    GemVal _t1684 = gem_table_new();
    gem_table_set(_t1684, gem_string("expr"), gem_v_t);
    gem_table_set(_t1684, gem_string("setup"), gem_v_setup);
                                                    GemVal _t1685 = _t1684;
                                                    gem_pop_frame();
                                                    return _t1685;
                                                } else {
#line 803 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 804 "compiler/main.gem"
    GemVal _t1686 = gem_v_node;
    GemVal _t1687 = (*gem_v_compile_expr);
    GemVal _t1688[] = {gem_table_get(_t1686, gem_string("right"))};
                                                        GemVal gem_v_rc_r = _t1687.fn(_t1687.env, _t1688, 1);
#line 805 "compiler/main.gem"
    GemVal _t1689 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1689.fn(_t1689.env, NULL, 0);
#line 806 "compiler/main.gem"
                                                        GemVal gem_v_setup = gem_v_ls;
#line 807 "compiler/main.gem"
    GemVal _t1690[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1690, 1)), gem_string(";\n")));
#line 808 "compiler/main.gem"
    GemVal _t1691[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t1691, 1)), gem_string(")) {\n")));
#line 809 "compiler/main.gem"
    GemVal _t1692[] = {gem_v_t};
    GemVal _t1693[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1692, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1693, 1)), gem_string(";\n")));
#line 810 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 811 "compiler/main.gem"
    GemVal _t1694 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t1694, gem_string("setup")), gem_string("")))) {
#line 812 "compiler/main.gem"
    GemVal _t1695 = gem_v_rc_r;
    GemVal _t1696[] = {gem_table_get(_t1695, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t1696, 3));
                                                        }
#line 814 "compiler/main.gem"
    GemVal _t1697[] = {gem_v_t};
    GemVal _t1698 = gem_v_rc_r;
    GemVal _t1699[] = {gem_table_get(_t1698, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1697, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1699, 1)), gem_string(";\n")));
#line 815 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 816 "compiler/main.gem"
    GemVal _t1700 = gem_table_new();
    gem_table_set(_t1700, gem_string("expr"), gem_v_t);
    gem_table_set(_t1700, gem_string("setup"), gem_v_setup);
                                                        GemVal _t1701 = _t1700;
                                                        gem_pop_frame();
                                                        return _t1701;
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
#line 819 "compiler/main.gem"
    GemVal _t1702 = gem_v_node;
    GemVal _t1703 = (*gem_v_compile_expr);
    GemVal _t1704[] = {gem_table_get(_t1702, gem_string("right"))};
    GemVal gem_v_rc_r = _t1703.fn(_t1703.env, _t1704, 1);
    GemVal _t1705 = gem_table_new();
    gem_table_set(_t1705, gem_string("expr"), gem_string("GEM_NIL"));
    GemVal _t1706 = gem_v_rc_r;
    gem_table_set(_t1705, gem_string("setup"), gem_add(gem_add(gem_add(gem_add(gem_v_ls, gem_table_get(_t1706, gem_string("setup"))), gem_string("/* unknown op: ")), gem_v_op), gem_string(" */\n")));
    GemVal _t1707 = _t1705;
    gem_pop_frame();
    return _t1707;
}

struct _closure__anon_36 {
    GemVal *gem_v_compile_stmt;
};
static GemVal _anon_36(void *_env, GemVal *args, int argc) {
    struct _closure__anon_36 *_cls = (struct _closure__anon_36 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_36", "compiler/main.gem", 0);
#line 826 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
    {
#line 827 "compiler/main.gem"
        GemVal gem_v__for_items_32 = gem_v_stmts;
#line 827 "compiler/main.gem"
        GemVal gem_v__for_i_32 = gem_int(0);
#line 827 "compiler/main.gem"
        while (1) {
            GemVal _t1709[] = {gem_v__for_items_32};
            if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_len_fn(NULL, _t1709, 1)))) break;
#line 827 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_32, gem_v__for_i_32);
#line 827 "compiler/main.gem"
            gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 828 "compiler/main.gem"
    GemVal _t1710 = (*gem_v_compile_stmt);
    GemVal _t1711[] = {gem_v_stmt, gem_v_indent};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1710.fn(_t1710.env, _t1711, 2)), gem_string("\n"));
        }
    }
    GemVal _t1712 = gem_v_out;
    gem_pop_frame();
    return _t1712;
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
    GemVal *gem_v_tmp;
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
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_37", "compiler/main.gem", 0);
#line 836 "compiler/main.gem"
    GemVal _t1714[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1714, 1);
#line 837 "compiler/main.gem"
    GemVal _t1715 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1715, gem_string("tag"));
#line 838 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 839 "compiler/main.gem"
    GemVal _t1716 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1716, gem_string("line")), GEM_NIL))) {
#line 840 "compiler/main.gem"
    GemVal _t1717 = gem_v_node;
    GemVal _t1718[] = {gem_table_get(_t1717, gem_string("line"))};
    GemVal _t1719[] = {(*gem_v_source_name)};
    GemVal _t1720[] = {gem_fn_escape_c_string(NULL, _t1719, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t1718, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t1720, 1)), gem_string("\"\n"));
    }
#line 843 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 844 "compiler/main.gem"
    GemVal _t1721 = gem_v_node;
    GemVal _t1722 = (*gem_v_compile_expr);
    GemVal _t1723[] = {gem_table_get(_t1721, gem_string("value"))};
        GemVal gem_v_r = _t1722.fn(_t1722.env, _t1723, 1);
#line 845 "compiler/main.gem"
    GemVal _t1724 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1724, gem_string("name"));
#line 846 "compiler/main.gem"
    GemVal _t1725 = (*gem_v_mangle);
    GemVal _t1726[] = {gem_v_name};
        GemVal gem_v_mname = _t1725.fn(_t1725.env, _t1726, 1);
#line 847 "compiler/main.gem"
    GemVal _t1727[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1727, 2))) {
#line 848 "compiler/main.gem"
    GemVal _t1728[] = {gem_v_line_dir};
    GemVal _t1729 = gem_v_r;
    GemVal _t1730[] = {gem_table_get(_t1729, gem_string("setup"))};
    GemVal _t1731[] = {gem_v_p};
    GemVal _t1732[] = {gem_v_mname};
    GemVal _t1733[] = {gem_v_p};
    GemVal _t1734[] = {gem_v_mname};
    GemVal _t1735 = gem_v_r;
    GemVal _t1736[] = {gem_table_get(_t1735, gem_string("expr"))};
            GemVal _t1737 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1728, 1), gem_to_string_fn(NULL, _t1730, 1)), gem_to_string_fn(NULL, _t1731, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t1732, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1733, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1734, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1736, 1)), gem_string(";"));
            gem_pop_frame();
            return _t1737;
        }
#line 850 "compiler/main.gem"
    GemVal _t1738[] = {gem_v_line_dir};
    GemVal _t1739 = gem_v_r;
    GemVal _t1740[] = {gem_table_get(_t1739, gem_string("setup"))};
    GemVal _t1741[] = {gem_v_p};
    GemVal _t1742[] = {gem_v_mname};
    GemVal _t1743 = gem_v_r;
    GemVal _t1744[] = {gem_table_get(_t1743, gem_string("expr"))};
        GemVal _t1745 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1738, 1), gem_to_string_fn(NULL, _t1740, 1)), gem_to_string_fn(NULL, _t1741, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1742, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1744, 1)), gem_string(";"));
        gem_pop_frame();
        return _t1745;
    } else {
#line 851 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 852 "compiler/main.gem"
    GemVal _t1746 = gem_v_node;
    GemVal _t1747 = (*gem_v_compile_expr);
    GemVal _t1748[] = {gem_table_get(_t1746, gem_string("value"))};
            GemVal gem_v_r = _t1747.fn(_t1747.env, _t1748, 1);
#line 853 "compiler/main.gem"
    GemVal _t1749 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1749, gem_string("name"));
#line 854 "compiler/main.gem"
    GemVal _t1750 = (*gem_v_mangle);
    GemVal _t1751[] = {gem_v_name};
            GemVal gem_v_mname = _t1750.fn(_t1750.env, _t1751, 1);
#line 855 "compiler/main.gem"
    GemVal _t1752[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1752, 2))) {
#line 856 "compiler/main.gem"
    GemVal _t1753[] = {gem_v_line_dir};
    GemVal _t1754 = gem_v_r;
    GemVal _t1755[] = {gem_table_get(_t1754, gem_string("setup"))};
    GemVal _t1756[] = {gem_v_p};
    GemVal _t1757[] = {gem_v_mname};
    GemVal _t1758 = gem_v_r;
    GemVal _t1759[] = {gem_table_get(_t1758, gem_string("expr"))};
                GemVal _t1760 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1753, 1), gem_to_string_fn(NULL, _t1755, 1)), gem_to_string_fn(NULL, _t1756, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1757, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1759, 1)), gem_string(";"));
                gem_pop_frame();
                return _t1760;
            }
#line 858 "compiler/main.gem"
    GemVal _t1761[] = {gem_v_line_dir};
    GemVal _t1762 = gem_v_r;
    GemVal _t1763[] = {gem_table_get(_t1762, gem_string("setup"))};
    GemVal _t1764[] = {gem_v_p};
    GemVal _t1765[] = {gem_v_mname};
    GemVal _t1766 = gem_v_r;
    GemVal _t1767[] = {gem_table_get(_t1766, gem_string("expr"))};
            GemVal _t1768 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1761, 1), gem_to_string_fn(NULL, _t1763, 1)), gem_to_string_fn(NULL, _t1764, 1)), gem_to_string_fn(NULL, _t1765, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1767, 1)), gem_string(";"));
            gem_pop_frame();
            return _t1768;
        } else {
#line 859 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
#line 860 "compiler/main.gem"
    GemVal _t1769 = gem_v_node;
    GemVal _t1770 = (*gem_v_compile_expr);
    GemVal _t1771[] = {gem_table_get(_t1769, gem_string("object"))};
                GemVal gem_v_obj_r = _t1770.fn(_t1770.env, _t1771, 1);
#line 861 "compiler/main.gem"
    GemVal _t1772 = gem_v_node;
    GemVal _t1773 = (*gem_v_compile_expr);
    GemVal _t1774[] = {gem_table_get(_t1772, gem_string("value"))};
                GemVal gem_v_val_r = _t1773.fn(_t1773.env, _t1774, 1);
#line 862 "compiler/main.gem"
    GemVal _t1775 = gem_v_node;
    GemVal _t1776[] = {gem_table_get(_t1775, gem_string("field"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1776, 1);
#line 863 "compiler/main.gem"
    GemVal _t1777[] = {gem_v_line_dir};
    GemVal _t1778 = gem_v_obj_r;
    GemVal _t1779[] = {gem_table_get(_t1778, gem_string("setup"))};
    GemVal _t1780 = gem_v_val_r;
    GemVal _t1781[] = {gem_table_get(_t1780, gem_string("setup"))};
    GemVal _t1782[] = {gem_v_p};
    GemVal _t1783 = gem_v_obj_r;
    GemVal _t1784[] = {gem_table_get(_t1783, gem_string("expr"))};
    GemVal _t1785[] = {gem_v_escaped};
    GemVal _t1786 = gem_v_val_r;
    GemVal _t1787[] = {gem_table_get(_t1786, gem_string("expr"))};
                GemVal _t1788 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1777, 1), gem_to_string_fn(NULL, _t1779, 1)), gem_to_string_fn(NULL, _t1781, 1)), gem_to_string_fn(NULL, _t1782, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t1784, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1785, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1787, 1)), gem_string(");"));
                gem_pop_frame();
                return _t1788;
            } else {
#line 864 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("index_assign")))) {
#line 865 "compiler/main.gem"
    GemVal _t1789 = gem_v_node;
    GemVal _t1790 = (*gem_v_compile_expr);
    GemVal _t1791[] = {gem_table_get(_t1789, gem_string("object"))};
                    GemVal gem_v_obj_r = _t1790.fn(_t1790.env, _t1791, 1);
#line 866 "compiler/main.gem"
    GemVal _t1792 = gem_v_node;
    GemVal _t1793 = (*gem_v_compile_expr);
    GemVal _t1794[] = {gem_table_get(_t1792, gem_string("key"))};
                    GemVal gem_v_key_r = _t1793.fn(_t1793.env, _t1794, 1);
#line 867 "compiler/main.gem"
    GemVal _t1795 = gem_v_node;
    GemVal _t1796 = (*gem_v_compile_expr);
    GemVal _t1797[] = {gem_table_get(_t1795, gem_string("value"))};
                    GemVal gem_v_val_r = _t1796.fn(_t1796.env, _t1797, 1);
#line 868 "compiler/main.gem"
    GemVal _t1798[] = {gem_v_line_dir};
    GemVal _t1799 = gem_v_obj_r;
    GemVal _t1800[] = {gem_table_get(_t1799, gem_string("setup"))};
    GemVal _t1801 = gem_v_key_r;
    GemVal _t1802[] = {gem_table_get(_t1801, gem_string("setup"))};
    GemVal _t1803 = gem_v_val_r;
    GemVal _t1804[] = {gem_table_get(_t1803, gem_string("setup"))};
    GemVal _t1805[] = {gem_v_p};
    GemVal _t1806 = gem_v_obj_r;
    GemVal _t1807[] = {gem_table_get(_t1806, gem_string("expr"))};
    GemVal _t1808 = gem_v_key_r;
    GemVal _t1809[] = {gem_table_get(_t1808, gem_string("expr"))};
    GemVal _t1810 = gem_v_val_r;
    GemVal _t1811[] = {gem_table_get(_t1810, gem_string("expr"))};
                    GemVal _t1812 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1798, 1), gem_to_string_fn(NULL, _t1800, 1)), gem_to_string_fn(NULL, _t1802, 1)), gem_to_string_fn(NULL, _t1804, 1)), gem_to_string_fn(NULL, _t1805, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t1807, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1809, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1811, 1)), gem_string(");"));
                    gem_pop_frame();
                    return _t1812;
                } else {
#line 869 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 870 "compiler/main.gem"
    GemVal _t1813[] = {gem_v_line_dir};
    GemVal _t1814 = (*gem_v_compile_if);
    GemVal _t1815[] = {gem_v_node, gem_v_indent};
                        GemVal _t1816 = gem_add(gem_to_string_fn(NULL, _t1813, 1), _t1814.fn(_t1814.env, _t1815, 2));
                        gem_pop_frame();
                        return _t1816;
                    } else {
#line 871 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 872 "compiler/main.gem"
    GemVal _t1817[] = {gem_v_line_dir};
    GemVal _t1818 = (*gem_v_compile_while);
    GemVal _t1819[] = {gem_v_node, gem_v_indent};
                            GemVal _t1820 = gem_add(gem_to_string_fn(NULL, _t1817, 1), _t1818.fn(_t1818.env, _t1819, 2));
                            gem_pop_frame();
                            return _t1820;
                        } else {
#line 873 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 874 "compiler/main.gem"
    GemVal _t1821[] = {gem_v_line_dir};
    GemVal _t1822 = (*gem_v_compile_match);
    GemVal _t1823[] = {gem_v_node, gem_v_indent};
                                GemVal _t1824 = gem_add(gem_to_string_fn(NULL, _t1821, 1), _t1822.fn(_t1822.env, _t1823, 2));
                                gem_pop_frame();
                                return _t1824;
                            } else {
#line 875 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("break")))) {
#line 876 "compiler/main.gem"
                                    GemVal _t1825 = gem_add(gem_v_p, gem_string("break;"));
                                    gem_pop_frame();
                                    return _t1825;
                                } else {
#line 877 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("continue")))) {
#line 878 "compiler/main.gem"
                                        GemVal _t1826 = gem_add(gem_v_p, gem_string("continue;"));
                                        gem_pop_frame();
                                        return _t1826;
                                    } else {
#line 879 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 880 "compiler/main.gem"
    GemVal _t1827 = gem_v_node;
                                            if (gem_truthy(gem_neq(gem_table_get(_t1827, gem_string("value")), GEM_NIL))) {
#line 881 "compiler/main.gem"
    GemVal _t1828 = gem_v_node;
    GemVal _t1829 = (*gem_v_compile_expr);
    GemVal _t1830[] = {gem_table_get(_t1828, gem_string("value"))};
                                                GemVal gem_v_r = _t1829.fn(_t1829.env, _t1830, 1);
#line 882 "compiler/main.gem"
    GemVal _t1831 = (*gem_v_tmp);
                                                GemVal gem_v_t = _t1831.fn(_t1831.env, NULL, 0);
#line 883 "compiler/main.gem"
    GemVal _t1832[] = {gem_v_line_dir};
    GemVal _t1833 = gem_v_r;
    GemVal _t1834[] = {gem_table_get(_t1833, gem_string("setup"))};
    GemVal _t1835[] = {gem_v_p};
    GemVal _t1836[] = {gem_v_t};
    GemVal _t1837 = gem_v_r;
    GemVal _t1838[] = {gem_table_get(_t1837, gem_string("expr"))};
    GemVal _t1839[] = {gem_v_p};
    GemVal _t1840[] = {gem_v_p};
    GemVal _t1841[] = {gem_v_t};
                                                GemVal _t1842 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1832, 1), gem_to_string_fn(NULL, _t1834, 1)), gem_to_string_fn(NULL, _t1835, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1836, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1838, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1839, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1840, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1841, 1)), gem_string(";"));
                                                gem_pop_frame();
                                                return _t1842;
                                            }
#line 885 "compiler/main.gem"
    GemVal _t1843[] = {gem_v_line_dir};
    GemVal _t1844[] = {gem_v_p};
    GemVal _t1845[] = {gem_v_p};
                                            GemVal _t1846 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1843, 1), gem_to_string_fn(NULL, _t1844, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1845, 1)), gem_string("return GEM_NIL;"));
                                            gem_pop_frame();
                                            return _t1846;
                                        } else {
#line 886 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 887 "compiler/main.gem"
    GemVal _t1847 = (*gem_v_compile_expr);
    GemVal _t1848[] = {gem_v_node};
                                                GemVal gem_v_r = _t1847.fn(_t1847.env, _t1848, 1);
#line 888 "compiler/main.gem"
    GemVal _t1849[] = {gem_v_line_dir};
    GemVal _t1850 = gem_v_r;
    GemVal _t1851[] = {gem_table_get(_t1850, gem_string("setup"))};
    GemVal _t1852[] = {gem_v_p};
    GemVal _t1853 = gem_v_r;
    GemVal _t1854[] = {gem_table_get(_t1853, gem_string("expr"))};
                                                GemVal _t1855 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1849, 1), gem_to_string_fn(NULL, _t1851, 1)), gem_to_string_fn(NULL, _t1852, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t1854, 1)), gem_string(");"));
                                                gem_pop_frame();
                                                return _t1855;
                                            } else {
#line 889 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
#line 890 "compiler/main.gem"
                                                    GemVal _t1856 = gem_string("");
                                                    gem_pop_frame();
                                                    return _t1856;
                                                } else {
#line 891 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 892 "compiler/main.gem"
    GemVal _t1857[] = {gem_v_p};
                                                        GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t1857, 1), gem_string("{\n"));
                                                        {
#line 893 "compiler/main.gem"
    GemVal _t1858 = gem_v_node;
                                                            GemVal gem_v__for_items_33 = gem_table_get(_t1858, gem_string("stmts"));
#line 893 "compiler/main.gem"
                                                            GemVal gem_v__for_i_33 = gem_int(0);
#line 893 "compiler/main.gem"
                                                            while (1) {
                                                                GemVal _t1859[] = {gem_v__for_items_33};
                                                                if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_len_fn(NULL, _t1859, 1)))) break;
#line 893 "compiler/main.gem"
                                                                GemVal gem_v_stmt = gem_table_get(gem_v__for_items_33, gem_v__for_i_33);
#line 893 "compiler/main.gem"
                                                                gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 894 "compiler/main.gem"
    GemVal _t1860 = (*gem_v_compile_stmt);
    GemVal _t1861[] = {gem_v_stmt, gem_add(gem_v_indent, gem_int(1))};
                                                                gem_v_out = gem_add(gem_add(gem_v_out, _t1860.fn(_t1860.env, _t1861, 2)), gem_string("\n"));
                                                            }
                                                        }
#line 896 "compiler/main.gem"
    GemVal _t1862[] = {gem_v_p};
                                                        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1862, 1), gem_string("}")));
#line 897 "compiler/main.gem"
                                                        GemVal _t1863 = gem_v_out;
                                                        gem_pop_frame();
                                                        return _t1863;
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
#line 900 "compiler/main.gem"
    GemVal _t1864 = (*gem_v_compile_expr);
    GemVal _t1865[] = {gem_v_node};
    GemVal gem_v_r = _t1864.fn(_t1864.env, _t1865, 1);
    GemVal _t1866[] = {gem_v_line_dir};
    GemVal _t1867 = gem_v_r;
    GemVal _t1868[] = {gem_table_get(_t1867, gem_string("setup"))};
    GemVal _t1869[] = {gem_v_p};
    GemVal _t1870 = gem_v_r;
    GemVal _t1871[] = {gem_table_get(_t1870, gem_string("expr"))};
    GemVal _t1872 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1866, 1), gem_to_string_fn(NULL, _t1868, 1)), gem_to_string_fn(NULL, _t1869, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t1871, 1)), gem_string(");"));
    gem_pop_frame();
    return _t1872;
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
    gem_push_frame("_anon_38", "compiler/main.gem", 0);
#line 907 "compiler/main.gem"
    GemVal _t1874[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1874, 1);
#line 908 "compiler/main.gem"
    GemVal _t1875 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1875, gem_string("tag"));
#line 909 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 910 "compiler/main.gem"
    GemVal _t1876 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1876, gem_string("line")), GEM_NIL))) {
#line 911 "compiler/main.gem"
    GemVal _t1877 = gem_v_node;
    GemVal _t1878[] = {gem_table_get(_t1877, gem_string("line"))};
    GemVal _t1879[] = {(*gem_v_source_name)};
    GemVal _t1880[] = {gem_fn_escape_c_string(NULL, _t1879, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t1878, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t1880, 1)), gem_string("\"\n"));
    }
#line 914 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 915 "compiler/main.gem"
    GemVal _t1881 = gem_v_node;
    GemVal _t1882 = (*gem_v_compile_expr);
    GemVal _t1883[] = {gem_table_get(_t1881, gem_string("value"))};
        GemVal gem_v_r = _t1882.fn(_t1882.env, _t1883, 1);
#line 916 "compiler/main.gem"
    GemVal _t1884 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1884, gem_string("name"));
#line 917 "compiler/main.gem"
    GemVal _t1885 = (*gem_v_mangle);
    GemVal _t1886[] = {gem_v_name};
        GemVal gem_v_mname = _t1885.fn(_t1885.env, _t1886, 1);
#line 918 "compiler/main.gem"
    GemVal _t1887[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1887, 2))) {
#line 919 "compiler/main.gem"
    GemVal _t1888[] = {gem_v_line_dir};
    GemVal _t1889 = gem_v_r;
    GemVal _t1890[] = {gem_table_get(_t1889, gem_string("setup"))};
    GemVal _t1891[] = {gem_v_p};
    GemVal _t1892[] = {gem_v_mname};
    GemVal _t1893[] = {gem_v_p};
    GemVal _t1894[] = {gem_v_mname};
    GemVal _t1895 = gem_v_r;
    GemVal _t1896[] = {gem_table_get(_t1895, gem_string("expr"))};
    GemVal _t1897[] = {gem_v_p};
    GemVal _t1898[] = {gem_v_p};
            GemVal _t1899 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1888, 1), gem_to_string_fn(NULL, _t1890, 1)), gem_to_string_fn(NULL, _t1891, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t1892, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1893, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1894, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1896, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1897, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1898, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t1899;
        }
#line 921 "compiler/main.gem"
    GemVal _t1900[] = {gem_v_line_dir};
    GemVal _t1901 = gem_v_r;
    GemVal _t1902[] = {gem_table_get(_t1901, gem_string("setup"))};
    GemVal _t1903[] = {gem_v_p};
    GemVal _t1904[] = {gem_v_mname};
    GemVal _t1905 = gem_v_r;
    GemVal _t1906[] = {gem_table_get(_t1905, gem_string("expr"))};
    GemVal _t1907[] = {gem_v_p};
    GemVal _t1908[] = {gem_v_p};
        GemVal _t1909 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1900, 1), gem_to_string_fn(NULL, _t1902, 1)), gem_to_string_fn(NULL, _t1903, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1904, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1906, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1907, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1908, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t1909;
    } else {
#line 922 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 923 "compiler/main.gem"
    GemVal _t1910 = gem_v_node;
    GemVal _t1911 = (*gem_v_compile_expr);
    GemVal _t1912[] = {gem_table_get(_t1910, gem_string("value"))};
            GemVal gem_v_r = _t1911.fn(_t1911.env, _t1912, 1);
#line 924 "compiler/main.gem"
    GemVal _t1913 = (*gem_v_tmp);
            GemVal gem_v_t = _t1913.fn(_t1913.env, NULL, 0);
#line 925 "compiler/main.gem"
    GemVal _t1914 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1914, gem_string("name"));
#line 926 "compiler/main.gem"
    GemVal _t1915 = (*gem_v_mangle);
    GemVal _t1916[] = {gem_v_name};
            GemVal gem_v_mname = _t1915.fn(_t1915.env, _t1916, 1);
#line 927 "compiler/main.gem"
    GemVal _t1917[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1917, 2))) {
#line 928 "compiler/main.gem"
    GemVal _t1918[] = {gem_v_line_dir};
    GemVal _t1919 = gem_v_r;
    GemVal _t1920[] = {gem_table_get(_t1919, gem_string("setup"))};
    GemVal _t1921[] = {gem_v_p};
    GemVal _t1922[] = {gem_v_t};
    GemVal _t1923 = gem_v_r;
    GemVal _t1924[] = {gem_table_get(_t1923, gem_string("expr"))};
    GemVal _t1925[] = {gem_v_p};
    GemVal _t1926[] = {gem_v_mname};
    GemVal _t1927[] = {gem_v_t};
    GemVal _t1928[] = {gem_v_p};
    GemVal _t1929[] = {gem_v_p};
    GemVal _t1930[] = {gem_v_t};
                GemVal _t1931 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1918, 1), gem_to_string_fn(NULL, _t1920, 1)), gem_to_string_fn(NULL, _t1921, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1922, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1924, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1925, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1926, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1927, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1928, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1929, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1930, 1)), gem_string(";"));
                gem_pop_frame();
                return _t1931;
            }
#line 930 "compiler/main.gem"
    GemVal _t1932[] = {gem_v_line_dir};
    GemVal _t1933 = gem_v_r;
    GemVal _t1934[] = {gem_table_get(_t1933, gem_string("setup"))};
    GemVal _t1935[] = {gem_v_p};
    GemVal _t1936[] = {gem_v_t};
    GemVal _t1937 = gem_v_r;
    GemVal _t1938[] = {gem_table_get(_t1937, gem_string("expr"))};
    GemVal _t1939[] = {gem_v_p};
    GemVal _t1940[] = {gem_v_mname};
    GemVal _t1941[] = {gem_v_t};
    GemVal _t1942[] = {gem_v_p};
    GemVal _t1943[] = {gem_v_p};
    GemVal _t1944[] = {gem_v_t};
            GemVal _t1945 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1932, 1), gem_to_string_fn(NULL, _t1934, 1)), gem_to_string_fn(NULL, _t1935, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1936, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1938, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1939, 1)), gem_to_string_fn(NULL, _t1940, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1941, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1942, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1943, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1944, 1)), gem_string(";"));
            gem_pop_frame();
            return _t1945;
        } else {
#line 931 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 932 "compiler/main.gem"
    GemVal _t1946 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t1946, gem_string("value")), GEM_NIL))) {
#line 933 "compiler/main.gem"
    GemVal _t1947 = gem_v_node;
    GemVal _t1948 = (*gem_v_compile_expr);
    GemVal _t1949[] = {gem_table_get(_t1947, gem_string("value"))};
                    GemVal gem_v_r = _t1948.fn(_t1948.env, _t1949, 1);
#line 934 "compiler/main.gem"
    GemVal _t1950 = (*gem_v_tmp);
                    GemVal gem_v_t = _t1950.fn(_t1950.env, NULL, 0);
#line 935 "compiler/main.gem"
    GemVal _t1951[] = {gem_v_line_dir};
    GemVal _t1952 = gem_v_r;
    GemVal _t1953[] = {gem_table_get(_t1952, gem_string("setup"))};
    GemVal _t1954[] = {gem_v_p};
    GemVal _t1955[] = {gem_v_t};
    GemVal _t1956 = gem_v_r;
    GemVal _t1957[] = {gem_table_get(_t1956, gem_string("expr"))};
    GemVal _t1958[] = {gem_v_p};
    GemVal _t1959[] = {gem_v_p};
    GemVal _t1960[] = {gem_v_t};
                    GemVal _t1961 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1951, 1), gem_to_string_fn(NULL, _t1953, 1)), gem_to_string_fn(NULL, _t1954, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1955, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1957, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1958, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1959, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1960, 1)), gem_string(";"));
                    gem_pop_frame();
                    return _t1961;
                }
#line 937 "compiler/main.gem"
    GemVal _t1962[] = {gem_v_line_dir};
    GemVal _t1963[] = {gem_v_p};
    GemVal _t1964[] = {gem_v_p};
                GemVal _t1965 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1962, 1), gem_to_string_fn(NULL, _t1963, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1964, 1)), gem_string("return GEM_NIL;"));
                gem_pop_frame();
                return _t1965;
            } else {
#line 938 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 939 "compiler/main.gem"
    GemVal _t1966[] = {gem_v_line_dir};
    GemVal _t1967 = (*gem_v_compile_if_return);
    GemVal _t1968[] = {gem_v_node, gem_v_indent};
                    GemVal _t1969 = gem_add(gem_to_string_fn(NULL, _t1966, 1), _t1967.fn(_t1967.env, _t1968, 2));
                    gem_pop_frame();
                    return _t1969;
                } else {
#line 940 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 941 "compiler/main.gem"
    GemVal _t1970[] = {gem_v_line_dir};
    GemVal _t1971 = (*gem_v_compile_while);
    GemVal _t1972[] = {gem_v_node, gem_v_indent};
    GemVal _t1973[] = {_t1971.fn(_t1971.env, _t1972, 2)};
    GemVal _t1974[] = {gem_v_p};
    GemVal _t1975[] = {gem_v_p};
                        GemVal _t1976 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1970, 1), gem_to_string_fn(NULL, _t1973, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t1974, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1975, 1)), gem_string("return GEM_NIL;"));
                        gem_pop_frame();
                        return _t1976;
                    } else {
#line 942 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 943 "compiler/main.gem"
    GemVal _t1977[] = {gem_v_line_dir};
    GemVal _t1978 = (*gem_v_compile_match_return);
    GemVal _t1979[] = {gem_v_node, gem_v_indent};
                            GemVal _t1980 = gem_add(gem_to_string_fn(NULL, _t1977, 1), _t1978.fn(_t1978.env, _t1979, 2));
                            gem_pop_frame();
                            return _t1980;
                        } else {
#line 944 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 945 "compiler/main.gem"
    GemVal _t1981[] = {gem_v_p};
                                GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t1981, 1), gem_string("{\n"));
                                {
#line 946 "compiler/main.gem"
                                    GemVal gem_v__for_i_34 = gem_int(0);
#line 946 "compiler/main.gem"
    GemVal _t1982 = gem_v_node;
    GemVal _t1983[] = {gem_table_get(_t1982, gem_string("stmts"))};
                                    GemVal gem_v__for_limit_34 = gem_sub(gem_len_fn(NULL, _t1983, 1), gem_int(1));
#line 946 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
#line 946 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_34;
#line 946 "compiler/main.gem"
                                        gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 947 "compiler/main.gem"
    GemVal _t1984 = gem_v_node;
    GemVal _t1985 = (*gem_v_compile_stmt);
    GemVal _t1986[] = {gem_table_get(gem_table_get(_t1984, gem_string("stmts")), gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                                        gem_v_out = gem_add(gem_add(gem_v_out, _t1985.fn(_t1985.env, _t1986, 2)), gem_string("\n"));
                                    }
                                }
#line 949 "compiler/main.gem"
    GemVal _t1987 = gem_v_node;
    GemVal _t1988[] = {gem_table_get(_t1987, gem_string("stmts"))};
                                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1988, 1), gem_int(0)))) {
#line 950 "compiler/main.gem"
    GemVal _t1989 = gem_v_node;
    GemVal _t1990 = gem_v_node;
    GemVal _t1991[] = {gem_table_get(_t1990, gem_string("stmts"))};
    GemVal _t1992 = (*gem_v_compile_stmt_return);
    GemVal _t1993[] = {gem_table_get(gem_table_get(_t1989, gem_string("stmts")), gem_sub(gem_len_fn(NULL, _t1991, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t1992.fn(_t1992.env, _t1993, 2)), gem_string("\n"));
                                }
#line 952 "compiler/main.gem"
    GemVal _t1994[] = {gem_v_p};
                                gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1994, 1), gem_string("}")));
#line 953 "compiler/main.gem"
                                GemVal _t1995 = gem_v_out;
                                gem_pop_frame();
                                return _t1995;
                            } else {
#line 954 "compiler/main.gem"
    GemVal _t1996;
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
        _t1996 = gem_eq(gem_v_tag, gem_string("dot_assign"));
    } else {
        _t1996 = gem_eq(gem_v_tag, gem_string("index_assign"));
    }
    GemVal _t1997;
    if (gem_truthy(_t1996)) {
        _t1997 = _t1996;
    } else {
        _t1997 = gem_eq(gem_v_tag, gem_string("break"));
    }
    GemVal _t1998;
    if (gem_truthy(_t1997)) {
        _t1998 = _t1997;
    } else {
        _t1998 = gem_eq(gem_v_tag, gem_string("continue"));
    }
    GemVal _t1999;
    if (gem_truthy(_t1998)) {
        _t1999 = _t1998;
    } else {
        _t1999 = gem_eq(gem_v_tag, gem_string("fn_def"));
    }
                                if (gem_truthy(_t1999)) {
#line 955 "compiler/main.gem"
    GemVal _t2000 = (*gem_v_compile_stmt);
    GemVal _t2001[] = {gem_v_node, gem_v_indent};
    GemVal _t2002[] = {_t2000.fn(_t2000.env, _t2001, 2)};
    GemVal _t2003[] = {gem_v_p};
    GemVal _t2004[] = {gem_v_p};
                                    GemVal _t2005 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2002, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2003, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2004, 1)), gem_string("return GEM_NIL;"));
                                    gem_pop_frame();
                                    return _t2005;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 958 "compiler/main.gem"
    GemVal _t2006 = (*gem_v_compile_expr);
    GemVal _t2007[] = {gem_v_node};
    GemVal gem_v_r = _t2006.fn(_t2006.env, _t2007, 1);
#line 959 "compiler/main.gem"
    GemVal _t2008 = (*gem_v_tmp);
    GemVal gem_v_t = _t2008.fn(_t2008.env, NULL, 0);
    GemVal _t2009[] = {gem_v_line_dir};
    GemVal _t2010 = gem_v_r;
    GemVal _t2011[] = {gem_table_get(_t2010, gem_string("setup"))};
    GemVal _t2012[] = {gem_v_p};
    GemVal _t2013[] = {gem_v_t};
    GemVal _t2014 = gem_v_r;
    GemVal _t2015[] = {gem_table_get(_t2014, gem_string("expr"))};
    GemVal _t2016[] = {gem_v_p};
    GemVal _t2017[] = {gem_v_p};
    GemVal _t2018[] = {gem_v_t};
    GemVal _t2019 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2009, 1), gem_to_string_fn(NULL, _t2011, 1)), gem_to_string_fn(NULL, _t2012, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2013, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2015, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2016, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2017, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2018, 1)), gem_string(";"));
    gem_pop_frame();
    return _t2019;
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
    gem_push_frame("_anon_39", "compiler/main.gem", 0);
#line 966 "compiler/main.gem"
    GemVal _t2021[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2021, 1);
#line 967 "compiler/main.gem"
    GemVal _t2022 = gem_v_node;
    GemVal _t2023 = (*gem_v_compile_expr);
    GemVal _t2024[] = {gem_table_get(_t2022, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2023.fn(_t2023.env, _t2024, 1);
#line 968 "compiler/main.gem"
    GemVal _t2025 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2025, gem_string("setup"));
#line 969 "compiler/main.gem"
    GemVal _t2026[] = {gem_v_p};
    GemVal _t2027 = gem_v_cond_r;
    GemVal _t2028[] = {gem_table_get(_t2027, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2026, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2028, 1)), gem_string(")) {\n")));
#line 970 "compiler/main.gem"
    GemVal _t2029 = (*gem_v_compile_stmts);
    GemVal _t2030[] = {gem_table_get(gem_v_node, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t2029.fn(_t2029.env, _t2030, 2));
#line 971 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 972 "compiler/main.gem"
    GemVal _t2031[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2031, 1), gem_string("} else {\n")));
#line 973 "compiler/main.gem"
    GemVal _t2032 = (*gem_v_compile_stmts);
    GemVal _t2033[] = {gem_table_get(gem_v_node, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t2032.fn(_t2032.env, _t2033, 2));
    }
#line 975 "compiler/main.gem"
    GemVal _t2034[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2034, 1), gem_string("}")));
    GemVal _t2035 = gem_v_out;
    gem_pop_frame();
    return _t2035;
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
    gem_push_frame("_anon_40", "compiler/main.gem", 0);
#line 982 "compiler/main.gem"
    GemVal _t2037[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2037, 1);
#line 983 "compiler/main.gem"
    GemVal _t2038 = gem_v_node;
    GemVal _t2039 = (*gem_v_compile_expr);
    GemVal _t2040[] = {gem_table_get(_t2038, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2039.fn(_t2039.env, _t2040, 1);
#line 984 "compiler/main.gem"
    GemVal _t2041 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2041, gem_string("setup"));
#line 985 "compiler/main.gem"
    GemVal _t2042[] = {gem_v_p};
    GemVal _t2043 = gem_v_cond_r;
    GemVal _t2044[] = {gem_table_get(_t2043, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2042, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2044, 1)), gem_string(")) {\n")));
#line 986 "compiler/main.gem"
    GemVal gem_v_then_body = gem_table_get(gem_v_node, gem_string("then"));
#line 987 "compiler/main.gem"
    GemVal _t2045[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2045, 1), gem_int(0)))) {
        {
#line 988 "compiler/main.gem"
            GemVal gem_v__for_i_35 = gem_int(0);
#line 988 "compiler/main.gem"
    GemVal _t2046[] = {gem_v_then_body};
            GemVal gem_v__for_limit_35 = gem_sub(gem_len_fn(NULL, _t2046, 1), gem_int(1));
#line 988 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 988 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_35;
#line 988 "compiler/main.gem"
                gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 989 "compiler/main.gem"
    GemVal _t2047 = (*gem_v_compile_stmt);
    GemVal _t2048[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t2047.fn(_t2047.env, _t2048, 2)), gem_string("\n"));
            }
        }
#line 991 "compiler/main.gem"
    GemVal _t2049[] = {gem_v_then_body};
    GemVal _t2050 = (*gem_v_compile_stmt_return);
    GemVal _t2051[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t2049, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_add(gem_v_out, _t2050.fn(_t2050.env, _t2051, 2)), gem_string("\n"));
    } else {
#line 993 "compiler/main.gem"
    GemVal _t2052[] = {gem_v_p};
    GemVal _t2053[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2052, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2053, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 995 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 996 "compiler/main.gem"
    GemVal _t2054[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2054, 1), gem_string("} else {\n")));
#line 997 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v_node, gem_string("else"));
#line 998 "compiler/main.gem"
    GemVal _t2055[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2055, 1), gem_int(0)))) {
            {
#line 999 "compiler/main.gem"
                GemVal gem_v__for_i_36 = gem_int(0);
#line 999 "compiler/main.gem"
    GemVal _t2056[] = {gem_v_else_body};
                GemVal gem_v__for_limit_36 = gem_sub(gem_len_fn(NULL, _t2056, 1), gem_int(1));
#line 999 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 999 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_36;
#line 999 "compiler/main.gem"
                    gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 1000 "compiler/main.gem"
    GemVal _t2057 = (*gem_v_compile_stmt);
    GemVal _t2058[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t2057.fn(_t2057.env, _t2058, 2)), gem_string("\n"));
                }
            }
#line 1002 "compiler/main.gem"
    GemVal _t2059[] = {gem_v_else_body};
    GemVal _t2060 = (*gem_v_compile_stmt_return);
    GemVal _t2061[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2059, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t2060.fn(_t2060.env, _t2061, 2)), gem_string("\n"));
        } else {
#line 1004 "compiler/main.gem"
    GemVal _t2062[] = {gem_v_p};
    GemVal _t2063[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2062, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2063, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1007 "compiler/main.gem"
    GemVal _t2064[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2064, 1), gem_string("} else {\n")));
#line 1008 "compiler/main.gem"
    GemVal _t2065[] = {gem_v_p};
    GemVal _t2066[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2065, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2066, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1010 "compiler/main.gem"
    GemVal _t2067[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2067, 1), gem_string("}")));
    GemVal _t2068 = gem_v_out;
    gem_pop_frame();
    return _t2068;
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
    gem_push_frame("_anon_41", "compiler/main.gem", 0);
#line 1017 "compiler/main.gem"
    GemVal _t2070[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2070, 1);
#line 1018 "compiler/main.gem"
    GemVal _t2071 = gem_v_node;
    GemVal _t2072 = (*gem_v_compile_expr);
    GemVal _t2073[] = {gem_table_get(_t2071, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2072.fn(_t2072.env, _t2073, 1);
#line 1019 "compiler/main.gem"
    GemVal _t2074[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2074, 1), gem_string("while (1) {\n"));
#line 1020 "compiler/main.gem"
    GemVal _t2075 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t2075, gem_string("setup")), gem_string("")))) {
#line 1021 "compiler/main.gem"
    GemVal _t2076 = gem_v_cond_r;
    GemVal _t2077[] = {gem_v_p};
    GemVal _t2078[] = {gem_table_get(_t2076, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t2077, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_str_replace_fn(NULL, _t2078, 3));
    }
#line 1023 "compiler/main.gem"
    GemVal _t2079[] = {gem_v_p};
    GemVal _t2080 = gem_v_cond_r;
    GemVal _t2081[] = {gem_table_get(_t2080, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2079, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t2081, 1)), gem_string(")) break;\n")));
#line 1024 "compiler/main.gem"
    GemVal _t2082 = gem_v_node;
    GemVal _t2083 = (*gem_v_compile_stmts);
    GemVal _t2084[] = {gem_table_get(_t2082, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t2083.fn(_t2083.env, _t2084, 2));
#line 1025 "compiler/main.gem"
    GemVal _t2085[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2085, 1), gem_string("}")));
    GemVal _t2086 = gem_v_out;
    gem_pop_frame();
    return _t2086;
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
    gem_push_frame("_anon_42", "compiler/main.gem", 0);
#line 1032 "compiler/main.gem"
    GemVal _t2088[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2088, 1);
#line 1033 "compiler/main.gem"
    GemVal _t2089 = gem_v_node;
    GemVal _t2090 = (*gem_v_compile_expr);
    GemVal _t2091[] = {gem_table_get(_t2089, gem_string("target"))};
    GemVal gem_v_target_r = _t2090.fn(_t2090.env, _t2091, 1);
#line 1034 "compiler/main.gem"
    GemVal _t2092 = (*gem_v_tmp);
    GemVal gem_v_t = _t2092.fn(_t2092.env, NULL, 0);
#line 1035 "compiler/main.gem"
    GemVal _t2093 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2093, gem_string("setup"));
#line 1036 "compiler/main.gem"
    GemVal _t2094[] = {gem_v_p};
    GemVal _t2095[] = {gem_v_t};
    GemVal _t2096 = gem_v_target_r;
    GemVal _t2097[] = {gem_table_get(_t2096, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2094, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2095, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2097, 1)), gem_string(";\n")));
    {
#line 1037 "compiler/main.gem"
        GemVal gem_v__for_i_37 = gem_int(0);
#line 1037 "compiler/main.gem"
    GemVal _t2098 = gem_v_node;
    GemVal _t2099[] = {gem_table_get(_t2098, gem_string("whens"))};
        GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t2099, 1);
#line 1037 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 1037 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_37;
#line 1037 "compiler/main.gem"
            gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 1038 "compiler/main.gem"
    GemVal _t2100 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t2100, gem_string("whens")), gem_v_i);
#line 1039 "compiler/main.gem"
    GemVal _t2101 = gem_v_w;
    GemVal _t2102 = (*gem_v_compile_expr);
    GemVal _t2103[] = {gem_table_get(_t2101, gem_string("value"))};
            GemVal gem_v_val_r = _t2102.fn(_t2102.env, _t2103, 1);
#line 1040 "compiler/main.gem"
            GemVal gem_v_keyword = gem_string("if");
#line 1041 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1042 "compiler/main.gem"
                gem_v_keyword = gem_string("} else if");
            }
#line 1044 "compiler/main.gem"
    GemVal _t2104 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t2104, gem_string("setup")));
#line 1045 "compiler/main.gem"
    GemVal _t2105[] = {gem_v_p};
    GemVal _t2106[] = {gem_v_keyword};
    GemVal _t2107[] = {gem_v_t};
    GemVal _t2108 = gem_v_val_r;
    GemVal _t2109[] = {gem_table_get(_t2108, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2105, 1), gem_to_string_fn(NULL, _t2106, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2107, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2109, 1)), gem_string("))) {\n")));
#line 1046 "compiler/main.gem"
    GemVal _t2110 = gem_v_w;
    GemVal _t2111 = (*gem_v_compile_stmts);
    GemVal _t2112[] = {gem_table_get(_t2110, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_v_out, _t2111.fn(_t2111.env, _t2112, 2));
        }
    }
#line 1048 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 1049 "compiler/main.gem"
    GemVal _t2113[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2113, 1), gem_string("} else {\n")));
#line 1050 "compiler/main.gem"
    GemVal _t2114 = (*gem_v_compile_stmts);
    GemVal _t2115[] = {gem_table_get(gem_v_node, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t2114.fn(_t2114.env, _t2115, 2));
    }
#line 1052 "compiler/main.gem"
    GemVal _t2116[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2116, 1), gem_string("}")));
    GemVal _t2117 = gem_v_out;
    gem_pop_frame();
    return _t2117;
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
    gem_push_frame("_anon_43", "compiler/main.gem", 0);
#line 1059 "compiler/main.gem"
    GemVal _t2119[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2119, 1);
#line 1060 "compiler/main.gem"
    GemVal _t2120 = gem_v_node;
    GemVal _t2121 = (*gem_v_compile_expr);
    GemVal _t2122[] = {gem_table_get(_t2120, gem_string("target"))};
    GemVal gem_v_target_r = _t2121.fn(_t2121.env, _t2122, 1);
#line 1061 "compiler/main.gem"
    GemVal _t2123 = (*gem_v_tmp);
    GemVal gem_v_t = _t2123.fn(_t2123.env, NULL, 0);
#line 1062 "compiler/main.gem"
    GemVal _t2124 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2124, gem_string("setup"));
#line 1063 "compiler/main.gem"
    GemVal _t2125[] = {gem_v_p};
    GemVal _t2126[] = {gem_v_t};
    GemVal _t2127 = gem_v_target_r;
    GemVal _t2128[] = {gem_table_get(_t2127, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2125, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2126, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2128, 1)), gem_string(";\n")));
    {
#line 1064 "compiler/main.gem"
        GemVal gem_v__for_i_39 = gem_int(0);
#line 1064 "compiler/main.gem"
    GemVal _t2129 = gem_v_node;
    GemVal _t2130[] = {gem_table_get(_t2129, gem_string("whens"))};
        GemVal gem_v__for_limit_39 = gem_len_fn(NULL, _t2130, 1);
#line 1064 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
#line 1064 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_39;
#line 1064 "compiler/main.gem"
            gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 1065 "compiler/main.gem"
    GemVal _t2131 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t2131, gem_string("whens")), gem_v_i);
#line 1066 "compiler/main.gem"
    GemVal _t2132 = gem_v_w;
    GemVal _t2133 = (*gem_v_compile_expr);
    GemVal _t2134[] = {gem_table_get(_t2132, gem_string("value"))};
            GemVal gem_v_val_r = _t2133.fn(_t2133.env, _t2134, 1);
#line 1067 "compiler/main.gem"
            GemVal gem_v_keyword = gem_string("if");
#line 1068 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1069 "compiler/main.gem"
                gem_v_keyword = gem_string("} else if");
            }
#line 1071 "compiler/main.gem"
    GemVal _t2135 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t2135, gem_string("setup")));
#line 1072 "compiler/main.gem"
    GemVal _t2136[] = {gem_v_p};
    GemVal _t2137[] = {gem_v_keyword};
    GemVal _t2138[] = {gem_v_t};
    GemVal _t2139 = gem_v_val_r;
    GemVal _t2140[] = {gem_table_get(_t2139, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2136, 1), gem_to_string_fn(NULL, _t2137, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2138, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2140, 1)), gem_string("))) {\n")));
#line 1073 "compiler/main.gem"
    GemVal _t2141 = gem_v_w;
            GemVal gem_v_body = gem_table_get(_t2141, gem_string("body"));
#line 1074 "compiler/main.gem"
    GemVal _t2142[] = {gem_v_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2142, 1), gem_int(0)))) {
                {
#line 1075 "compiler/main.gem"
                    GemVal gem_v__for_i_38 = gem_int(0);
#line 1075 "compiler/main.gem"
    GemVal _t2143[] = {gem_v_body};
                    GemVal gem_v__for_limit_38 = gem_sub(gem_len_fn(NULL, _t2143, 1), gem_int(1));
#line 1075 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 1075 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_38;
#line 1075 "compiler/main.gem"
                        gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 1076 "compiler/main.gem"
    GemVal _t2144 = (*gem_v_compile_stmt);
    GemVal _t2145[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
                        gem_v_out = gem_add(gem_add(gem_v_out, _t2144.fn(_t2144.env, _t2145, 2)), gem_string("\n"));
                    }
                }
#line 1078 "compiler/main.gem"
    GemVal _t2146[] = {gem_v_body};
    GemVal _t2147 = (*gem_v_compile_stmt_return);
    GemVal _t2148[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2146, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t2147.fn(_t2147.env, _t2148, 2)), gem_string("\n"));
            } else {
#line 1080 "compiler/main.gem"
    GemVal _t2149[] = {gem_v_p};
    GemVal _t2150[] = {gem_v_p};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2149, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2150, 1)), gem_string("    return GEM_NIL;\n")));
            }
        }
    }
#line 1083 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
#line 1084 "compiler/main.gem"
    GemVal _t2151[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2151, 1), gem_string("} else {\n")));
#line 1085 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v_node, gem_string("else"));
#line 1086 "compiler/main.gem"
    GemVal _t2152[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2152, 1), gem_int(0)))) {
            {
#line 1087 "compiler/main.gem"
                GemVal gem_v__for_i_40 = gem_int(0);
#line 1087 "compiler/main.gem"
    GemVal _t2153[] = {gem_v_else_body};
                GemVal gem_v__for_limit_40 = gem_sub(gem_len_fn(NULL, _t2153, 1), gem_int(1));
#line 1087 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 1087 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_40;
#line 1087 "compiler/main.gem"
                    gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 1088 "compiler/main.gem"
    GemVal _t2154 = (*gem_v_compile_stmt);
    GemVal _t2155[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t2154.fn(_t2154.env, _t2155, 2)), gem_string("\n"));
                }
            }
#line 1090 "compiler/main.gem"
    GemVal _t2156[] = {gem_v_else_body};
    GemVal _t2157 = (*gem_v_compile_stmt_return);
    GemVal _t2158[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2156, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t2157.fn(_t2157.env, _t2158, 2)), gem_string("\n"));
        } else {
#line 1092 "compiler/main.gem"
    GemVal _t2159[] = {gem_v_p};
    GemVal _t2160[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2159, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2160, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1095 "compiler/main.gem"
    GemVal _t2161[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2161, 1), gem_string("} else {\n")));
#line 1096 "compiler/main.gem"
    GemVal _t2162[] = {gem_v_p};
    GemVal _t2163[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2162, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2163, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1098 "compiler/main.gem"
    GemVal _t2164[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2164, 1), gem_string("}")));
    GemVal _t2165 = gem_v_out;
    gem_pop_frame();
    return _t2165;
}

struct _closure__anon_44 {
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_44", "compiler/main.gem", 0);
#line 1105 "compiler/main.gem"
    GemVal _t2167 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t2167, gem_string("name"));
#line 1106 "compiler/main.gem"
    GemVal _t2168 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t2168, gem_string("params"));
#line 1107 "compiler/main.gem"
    GemVal _t2169 = gem_v_node;
    GemVal gem_v_ret_type = gem_table_get(_t2169, gem_string("ret_type"));
#line 1109 "compiler/main.gem"
    GemVal _t2170[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2170, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1111 "compiler/main.gem"
    GemVal _t2171 = gem_table_new();
    GemVal gem_v_c_args = _t2171;
    {
#line 1112 "compiler/main.gem"
        GemVal gem_v__for_i_41 = gem_int(0);
#line 1112 "compiler/main.gem"
    GemVal _t2172[] = {gem_v_params};
        GemVal gem_v__for_limit_41 = gem_len_fn(NULL, _t2172, 1);
#line 1112 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_limit_41))) break;
#line 1112 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_41;
#line 1112 "compiler/main.gem"
            gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
#line 1113 "compiler/main.gem"
            GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1114 "compiler/main.gem"
            GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1115 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1116 "compiler/main.gem"
    GemVal _t2173[] = {gem_v_i};
    GemVal _t2174[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t2173, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2174, 1)), gem_string("].ival;\n")));
#line 1117 "compiler/main.gem"
    GemVal _t2175[] = {gem_v_i};
    GemVal _t2176[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2175, 1))};
                (void)(gem_push_fn(NULL, _t2176, 2));
            } else {
#line 1118 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1119 "compiler/main.gem"
    GemVal _t2177[] = {gem_v_i};
    GemVal _t2178[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t2177, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2178, 1)), gem_string("].fval;\n")));
#line 1120 "compiler/main.gem"
    GemVal _t2179[] = {gem_v_i};
    GemVal _t2180[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2179, 1))};
                    (void)(gem_push_fn(NULL, _t2180, 2));
                } else {
#line 1121 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1122 "compiler/main.gem"
    GemVal _t2181[] = {gem_v_i};
    GemVal _t2182[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t2181, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2182, 1)), gem_string("].sval;\n")));
#line 1123 "compiler/main.gem"
    GemVal _t2183[] = {gem_v_i};
    GemVal _t2184[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2183, 1))};
                        (void)(gem_push_fn(NULL, _t2184, 2));
                    } else {
#line 1124 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1125 "compiler/main.gem"
    GemVal _t2185[] = {gem_v_i};
    GemVal _t2186[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t2185, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2186, 1)), gem_string("].bval;\n")));
#line 1126 "compiler/main.gem"
    GemVal _t2187[] = {gem_v_i};
    GemVal _t2188[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2187, 1))};
                            (void)(gem_push_fn(NULL, _t2188, 2));
                        } else {
#line 1127 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1128 "compiler/main.gem"
    GemVal _t2189[] = {gem_v_i};
    GemVal _t2190[] = {gem_v_i};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t2189, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t2190, 1)), gem_string("].ival;\n")));
#line 1129 "compiler/main.gem"
    GemVal _t2191[] = {gem_v_i};
    GemVal _t2192[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2191, 1))};
                                (void)(gem_push_fn(NULL, _t2192, 2));
                            } else {
#line 1130 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1131 "compiler/main.gem"
    GemVal _t2193[] = {gem_v_i};
    GemVal _t2194[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2193, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t2194, 2));
                                } else {
#line 1133 "compiler/main.gem"
    GemVal _t2195[] = {gem_v_i};
    GemVal _t2196[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2195, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t2196, 2));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 1138 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
    {
#line 1139 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 1139 "compiler/main.gem"
    GemVal _t2197[] = {gem_v_c_args};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2197, 1);
#line 1139 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 1139 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_42;
#line 1139 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 1140 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1141 "compiler/main.gem"
                gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
            }
#line 1143 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
        }
    }
#line 1146 "compiler/main.gem"
    GemVal _t2198[] = {gem_v_name};
    GemVal _t2199[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2198, 1), gem_string("(")), gem_to_string_fn(NULL, _t2199, 1)), gem_string(")"));
#line 1148 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1149 "compiler/main.gem"
    GemVal _t2200[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2200, 1)), gem_string(";\n")));
#line 1150 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
#line 1151 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1152 "compiler/main.gem"
    GemVal _t2201[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t2201, 1)), gem_string(";\n")));
        } else {
#line 1153 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1154 "compiler/main.gem"
    GemVal _t2202[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t2202, 1)), gem_string(";\n")));
#line 1155 "compiler/main.gem"
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
#line 1156 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1157 "compiler/main.gem"
    GemVal _t2203[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t2203, 1)), gem_string(";\n")));
#line 1158 "compiler/main.gem"
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
#line 1159 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1160 "compiler/main.gem"
    GemVal _t2204[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t2204, 1)), gem_string(";\n")));
#line 1161 "compiler/main.gem"
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
#line 1162 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1163 "compiler/main.gem"
    GemVal _t2205[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t2205, 1)), gem_string(";\n")));
#line 1164 "compiler/main.gem"
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
#line 1165 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1166 "compiler/main.gem"
    GemVal _t2206[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t2206, 1)), gem_string(";\n")));
#line 1167 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
#line 1169 "compiler/main.gem"
    GemVal _t2207[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2207, 1)), gem_string(";\n")));
#line 1170 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1173 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1176 "compiler/main.gem"
    GemVal _t2208[] = {gem_v_name};
    GemVal _t2209[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2208, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t2209, 2));
    GemVal _t2210 = gem_v_lines;
    gem_pop_frame();
    return _t2210;
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
    gem_push_frame("_anon_45", "compiler/main.gem", 0);
#line 1184 "compiler/main.gem"
    GemVal _t2212 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t2212, gem_string("params"));
#line 1185 "compiler/main.gem"
    GemVal _t2213 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t2213, gem_string("body"));
#line 1186 "compiler/main.gem"
    GemVal _t2214 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t2214, gem_string("name"));
#line 1189 "compiler/main.gem"
    GemVal _t2215[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t2215, 1);
    {
#line 1190 "compiler/main.gem"
        GemVal gem_v__for_i_44 = gem_int(0);
#line 1190 "compiler/main.gem"
    GemVal _t2216[] = {gem_v_body};
        GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t2216, 1);
#line 1190 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 1190 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_44;
#line 1190 "compiler/main.gem"
            gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 1191 "compiler/main.gem"
    GemVal _t2217[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2219;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2217, 1), gem_string("table")))) {
        _t2219 = gem_eq(gem_type_fn(NULL, _t2217, 1), gem_string("table"));
    } else {
        GemVal _t2218 = gem_table_get(gem_v_body, gem_v_i);
        _t2219 = gem_eq(gem_table_get(_t2218, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t2219)) {
#line 1192 "compiler/main.gem"
    GemVal _t2220 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2221[] = {gem_v_scope_vars, gem_table_get(_t2220, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t2221, 2));
            }
#line 1194 "compiler/main.gem"
    GemVal _t2222[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2224;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2222, 1), gem_string("table")))) {
        _t2224 = gem_eq(gem_type_fn(NULL, _t2222, 1), gem_string("table"));
    } else {
        GemVal _t2223 = gem_table_get(gem_v_body, gem_v_i);
        _t2224 = gem_eq(gem_table_get(_t2223, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t2224)) {
                {
#line 1195 "compiler/main.gem"
                    GemVal gem_v__for_i_43 = gem_int(0);
#line 1195 "compiler/main.gem"
    GemVal _t2225 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2226[] = {gem_table_get(_t2225, gem_string("stmts"))};
                    GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t2226, 1);
#line 1195 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 1195 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_43;
#line 1195 "compiler/main.gem"
                        gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 1196 "compiler/main.gem"
    GemVal _t2227 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2228[] = {gem_table_get(gem_table_get(_t2227, gem_string("stmts")), gem_v_j)};
    GemVal _t2231;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2228, 1), gem_string("table")))) {
        _t2231 = gem_eq(gem_type_fn(NULL, _t2228, 1), gem_string("table"));
    } else {
        GemVal _t2229 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t2230 = gem_table_get(gem_table_get(_t2229, gem_string("stmts")), gem_v_j);
        _t2231 = gem_eq(gem_table_get(_t2230, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t2231)) {
#line 1197 "compiler/main.gem"
    GemVal _t2232 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2233 = gem_table_get(gem_table_get(_t2232, gem_string("stmts")), gem_v_j);
    GemVal _t2234[] = {gem_v_scope_vars, gem_table_get(_t2233, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t2234, 2));
                        }
                    }
                }
            }
        }
    }
#line 1202 "compiler/main.gem"
    GemVal _t2235 = (*gem_v_find_captured_in_scope);
    GemVal _t2236[] = {gem_v_body, gem_v_scope_vars};
    GemVal gem_v_captured = _t2235.fn(_t2235.env, _t2236, 2);
#line 1204 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1205 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1207 "compiler/main.gem"
    GemVal _t2237[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2237, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1210 "compiler/main.gem"
    GemVal _t2238 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2238, gem_string("line")), GEM_NIL))) {
#line 1211 "compiler/main.gem"
    GemVal _t2239 = gem_v_node;
    GemVal _t2240[] = {gem_table_get(_t2239, gem_string("line"))};
    GemVal _t2241[] = {(*gem_v_source_name)};
    GemVal _t2242[] = {gem_fn_escape_c_string(NULL, _t2241, 1)};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2240, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2242, 1)), gem_string("\"\n")));
    }
    {
#line 1215 "compiler/main.gem"
        GemVal gem_v__for_i_45 = gem_int(0);
#line 1215 "compiler/main.gem"
    GemVal _t2243[] = {gem_v_params};
        GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2243, 1);
#line 1215 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 1215 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_45;
#line 1215 "compiler/main.gem"
            gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 1216 "compiler/main.gem"
    GemVal _t2244 = (*gem_v_mangle);
    GemVal _t2245[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t2244.fn(_t2244.env, _t2245, 1);
#line 1217 "compiler/main.gem"
    GemVal _t2246[] = {gem_v_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2246, 2))) {
#line 1218 "compiler/main.gem"
    GemVal _t2247[] = {gem_v_mp};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2247, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1219 "compiler/main.gem"
    GemVal _t2248[] = {gem_v_mp};
    GemVal _t2249[] = {gem_v_i};
    GemVal _t2250[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2248, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2249, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2250, 1)), gem_string("] : GEM_NIL;\n")));
            } else {
#line 1221 "compiler/main.gem"
    GemVal _t2251[] = {gem_v_mp};
    GemVal _t2252[] = {gem_v_i};
    GemVal _t2253[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2251, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2252, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2253, 1)), gem_string("] : GEM_NIL;\n")));
            }
        }
    }
#line 1225 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1226 "compiler/main.gem"
    GemVal _t2254 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2254, gem_string("line")), GEM_NIL))) {
#line 1227 "compiler/main.gem"
    GemVal _t2255 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t2255, gem_string("line"));
    }
#line 1229 "compiler/main.gem"
    GemVal _t2256[] = {gem_v_name};
    GemVal _t2257[] = {gem_fn_escape_c_string(NULL, _t2256, 1)};
    GemVal _t2258[] = {(*gem_v_source_name)};
    GemVal _t2259[] = {gem_fn_escape_c_string(NULL, _t2258, 1)};
    GemVal _t2260[] = {gem_v_fn_line};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2257, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2259, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2260, 1)), gem_string(");\n")));
#line 1232 "compiler/main.gem"
    GemVal _t2261[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2261, 1), gem_int(0)))) {
        {
#line 1233 "compiler/main.gem"
            GemVal gem_v__for_i_46 = gem_int(0);
#line 1233 "compiler/main.gem"
    GemVal _t2262[] = {gem_v_body};
            GemVal gem_v__for_limit_46 = gem_sub(gem_len_fn(NULL, _t2262, 1), gem_int(1));
#line 1233 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
#line 1233 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_46;
#line 1233 "compiler/main.gem"
                gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 1234 "compiler/main.gem"
    GemVal _t2263 = (*gem_v_compile_stmt);
    GemVal _t2264[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t2263.fn(_t2263.env, _t2264, 2)), gem_string("\n"));
            }
        }
#line 1236 "compiler/main.gem"
    GemVal _t2265[] = {gem_v_body};
    GemVal _t2266 = (*gem_v_compile_stmt_return);
    GemVal _t2267[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2265, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t2266.fn(_t2266.env, _t2267, 2)), gem_string("\n"));
    } else {
#line 1238 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
#line 1241 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1242 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
    GemVal _t2268 = gem_v_lines;
    gem_pop_frame();
    return _t2268;
}

struct _closure__anon_46 {
    GemVal *gem_v_anon_name;
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_functions;
    GemVal *gem_v_mangle;
    GemVal *gem_v_source_name;
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
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_captures = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_46", "compiler/main.gem", 0);
#line 1249 "compiler/main.gem"
    GemVal _t2270 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t2270, gem_string("params"));
#line 1250 "compiler/main.gem"
    GemVal _t2271 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t2271, gem_string("body"));
#line 1251 "compiler/main.gem"
    GemVal _t2272 = (*gem_v_anon_name);
    GemVal gem_v_name = _t2272.fn(_t2272.env, NULL, 0);
#line 1254 "compiler/main.gem"
    GemVal _t2273[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t2273, 1);
    {
#line 1255 "compiler/main.gem"
        GemVal gem_v__for_i_48 = gem_int(0);
#line 1255 "compiler/main.gem"
    GemVal _t2274[] = {gem_v_body};
        GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t2274, 1);
#line 1255 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 1255 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_48;
#line 1255 "compiler/main.gem"
            gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 1256 "compiler/main.gem"
    GemVal _t2275[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2277;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2275, 1), gem_string("table")))) {
        _t2277 = gem_eq(gem_type_fn(NULL, _t2275, 1), gem_string("table"));
    } else {
        GemVal _t2276 = gem_table_get(gem_v_body, gem_v_i);
        _t2277 = gem_eq(gem_table_get(_t2276, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t2277)) {
#line 1257 "compiler/main.gem"
    GemVal _t2278 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2279[] = {gem_v_inner_scope, gem_table_get(_t2278, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t2279, 2));
            }
#line 1259 "compiler/main.gem"
    GemVal _t2280[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2282;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2280, 1), gem_string("table")))) {
        _t2282 = gem_eq(gem_type_fn(NULL, _t2280, 1), gem_string("table"));
    } else {
        GemVal _t2281 = gem_table_get(gem_v_body, gem_v_i);
        _t2282 = gem_eq(gem_table_get(_t2281, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t2282)) {
                {
#line 1260 "compiler/main.gem"
                    GemVal gem_v__for_i_47 = gem_int(0);
#line 1260 "compiler/main.gem"
    GemVal _t2283 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2284[] = {gem_table_get(_t2283, gem_string("stmts"))};
                    GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2284, 1);
#line 1260 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 1260 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_47;
#line 1260 "compiler/main.gem"
                        gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 1261 "compiler/main.gem"
    GemVal _t2285 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2286[] = {gem_table_get(gem_table_get(_t2285, gem_string("stmts")), gem_v_j)};
    GemVal _t2289;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2286, 1), gem_string("table")))) {
        _t2289 = gem_eq(gem_type_fn(NULL, _t2286, 1), gem_string("table"));
    } else {
        GemVal _t2287 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t2288 = gem_table_get(gem_table_get(_t2287, gem_string("stmts")), gem_v_j);
        _t2289 = gem_eq(gem_table_get(_t2288, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t2289)) {
#line 1262 "compiler/main.gem"
    GemVal _t2290 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2291 = gem_table_get(gem_table_get(_t2290, gem_string("stmts")), gem_v_j);
    GemVal _t2292[] = {gem_v_inner_scope, gem_table_get(_t2291, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t2292, 2));
                        }
                    }
                }
            }
        }
    }
#line 1267 "compiler/main.gem"
    GemVal _t2293 = (*gem_v_find_captured_in_scope);
    GemVal _t2294[] = {gem_v_body, gem_v_inner_scope};
    GemVal gem_v_inner_captured = _t2293.fn(_t2293.env, _t2294, 2);
#line 1269 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1270 "compiler/main.gem"
    GemVal _t2295[] = {gem_v_captures};
    GemVal _t2296[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t2295, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t2296, 2);
#line 1273 "compiler/main.gem"
    GemVal _t2297[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t2297, 1));
#line 1274 "compiler/main.gem"
    GemVal _t2298[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t2298, 1)), gem_string(" {\n"));
    {
#line 1275 "compiler/main.gem"
        GemVal gem_v__for_items_49 = gem_v_captures;
#line 1275 "compiler/main.gem"
        GemVal gem_v__for_i_49 = gem_int(0);
#line 1275 "compiler/main.gem"
        while (1) {
            GemVal _t2299[] = {gem_v__for_items_49};
            if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_len_fn(NULL, _t2299, 1)))) break;
#line 1275 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_49, gem_v__for_i_49);
#line 1275 "compiler/main.gem"
            gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 1276 "compiler/main.gem"
    GemVal _t2300 = (*gem_v_mangle);
    GemVal _t2301[] = {gem_v_cap};
    GemVal _t2302[] = {_t2300.fn(_t2300.env, _t2301, 1)};
            gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2302, 1)), gem_string(";\n")));
        }
    }
#line 1278 "compiler/main.gem"
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
#line 1281 "compiler/main.gem"
    GemVal _t2303[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t2303, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1282 "compiler/main.gem"
    GemVal _t2304[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2304, 1), gem_int(0)))) {
#line 1283 "compiler/main.gem"
    GemVal _t2305[] = {gem_v_struct_name};
    GemVal _t2306[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2305, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t2306, 1)), gem_string(" *)_env;\n")));
        {
#line 1284 "compiler/main.gem"
            GemVal gem_v__for_items_50 = gem_v_captures;
#line 1284 "compiler/main.gem"
            GemVal gem_v__for_i_50 = gem_int(0);
#line 1284 "compiler/main.gem"
            while (1) {
                GemVal _t2307[] = {gem_v__for_items_50};
                if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_len_fn(NULL, _t2307, 1)))) break;
#line 1284 "compiler/main.gem"
                GemVal gem_v_cap = gem_table_get(gem_v__for_items_50, gem_v__for_i_50);
#line 1284 "compiler/main.gem"
                gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 1285 "compiler/main.gem"
    GemVal _t2308 = (*gem_v_mangle);
    GemVal _t2309[] = {gem_v_cap};
                GemVal gem_v_mc = _t2308.fn(_t2308.env, _t2309, 1);
#line 1286 "compiler/main.gem"
    GemVal _t2310[] = {gem_v_mc};
    GemVal _t2311[] = {gem_v_mc};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2310, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t2311, 1)), gem_string(";\n")));
            }
        }
    }
    {
#line 1291 "compiler/main.gem"
        GemVal gem_v__for_i_51 = gem_int(0);
#line 1291 "compiler/main.gem"
    GemVal _t2312[] = {gem_v_params};
        GemVal gem_v__for_limit_51 = gem_len_fn(NULL, _t2312, 1);
#line 1291 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_v__for_limit_51))) break;
#line 1291 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_51;
#line 1291 "compiler/main.gem"
            gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 1292 "compiler/main.gem"
    GemVal _t2313 = (*gem_v_mangle);
    GemVal _t2314[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t2313.fn(_t2313.env, _t2314, 1);
#line 1293 "compiler/main.gem"
    GemVal _t2315[] = {gem_v_inner_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2315, 2))) {
#line 1294 "compiler/main.gem"
    GemVal _t2316[] = {gem_v_mp};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2316, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1295 "compiler/main.gem"
    GemVal _t2317[] = {gem_v_mp};
    GemVal _t2318[] = {gem_v_i};
    GemVal _t2319[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2317, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2318, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2319, 1)), gem_string("] : GEM_NIL;\n")));
            } else {
#line 1297 "compiler/main.gem"
    GemVal _t2320[] = {gem_v_mp};
    GemVal _t2321[] = {gem_v_i};
    GemVal _t2322[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2320, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2321, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2322, 1)), gem_string("] : GEM_NIL;\n")));
            }
        }
    }
#line 1301 "compiler/main.gem"
    GemVal _t2323[] = {gem_v_name};
    GemVal _t2324[] = {gem_fn_escape_c_string(NULL, _t2323, 1)};
    GemVal _t2325[] = {(*gem_v_source_name)};
    GemVal _t2326[] = {gem_fn_escape_c_string(NULL, _t2325, 1)};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2324, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2326, 1)), gem_string("\", 0);\n")));
#line 1303 "compiler/main.gem"
    GemVal _t2327[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2327, 1), gem_int(0)))) {
        {
#line 1304 "compiler/main.gem"
            GemVal gem_v__for_i_52 = gem_int(0);
#line 1304 "compiler/main.gem"
    GemVal _t2328[] = {gem_v_body};
            GemVal gem_v__for_limit_52 = gem_sub(gem_len_fn(NULL, _t2328, 1), gem_int(1));
#line 1304 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 1304 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_52;
#line 1304 "compiler/main.gem"
                gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 1305 "compiler/main.gem"
    GemVal _t2329 = (*gem_v_compile_stmt);
    GemVal _t2330[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t2329.fn(_t2329.env, _t2330, 2)), gem_string("\n"));
            }
        }
#line 1307 "compiler/main.gem"
    GemVal _t2331[] = {gem_v_body};
    GemVal _t2332 = (*gem_v_compile_stmt_return);
    GemVal _t2333[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2331, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t2332.fn(_t2332.env, _t2333, 2)), gem_string("\n"));
    } else {
#line 1309 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
#line 1312 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1313 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1316 "compiler/main.gem"
    GemVal _t2334[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t2334, 2));
    GemVal _t2335 = gem_table_new();
    gem_table_set(_t2335, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t2335, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t2335, gem_string("captures"), gem_v_captures);
    GemVal _t2336 = _t2335;
    gem_pop_frame();
    return _t2336;
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
    GemVal *gem_v_source_name;
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
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_47", "compiler/main.gem", 0);
#line 1325 "compiler/main.gem"
    GemVal _t2338 = gem_table_new();
    GemVal gem_v_fn_defs = _t2338;
#line 1326 "compiler/main.gem"
    GemVal _t2339 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t2339;
#line 1327 "compiler/main.gem"
    GemVal _t2340 = gem_table_new();
    GemVal gem_v_extern_includes = _t2340;
#line 1328 "compiler/main.gem"
    GemVal _t2341 = gem_table_new();
    GemVal gem_v_top_stmts = _t2341;
    {
#line 1330 "compiler/main.gem"
    GemVal _t2342 = gem_v_ast;
        GemVal gem_v__for_items_53 = gem_table_get(_t2342, gem_string("stmts"));
#line 1330 "compiler/main.gem"
        GemVal gem_v__for_i_53 = gem_int(0);
#line 1330 "compiler/main.gem"
        while (1) {
            GemVal _t2343[] = {gem_v__for_items_53};
            if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_len_fn(NULL, _t2343, 1)))) break;
#line 1330 "compiler/main.gem"
            GemVal gem_v_s = gem_table_get(gem_v__for_items_53, gem_v__for_i_53);
#line 1330 "compiler/main.gem"
            gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 1331 "compiler/main.gem"
    GemVal _t2344 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t2344, gem_string("tag")), gem_string("fn_def")))) {
#line 1332 "compiler/main.gem"
    GemVal _t2345[] = {gem_v_fn_defs, gem_v_s};
                (void)(gem_push_fn(NULL, _t2345, 2));
            } else {
#line 1333 "compiler/main.gem"
    GemVal _t2346 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t2346, gem_string("tag")), gem_string("extern_fn")))) {
#line 1334 "compiler/main.gem"
    GemVal _t2347[] = {gem_v_extern_fns_list, gem_v_s};
                    (void)(gem_push_fn(NULL, _t2347, 2));
                } else {
#line 1335 "compiler/main.gem"
    GemVal _t2348 = gem_v_s;
                    if (gem_truthy(gem_eq(gem_table_get(_t2348, gem_string("tag")), gem_string("extern_include")))) {
#line 1336 "compiler/main.gem"
    GemVal _t2349[] = {gem_v_extern_includes, gem_v_s};
                        (void)(gem_push_fn(NULL, _t2349, 2));
                    } else {
#line 1338 "compiler/main.gem"
    GemVal _t2350[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t2350, 2));
                    }
                }
            }
        }
    }
    {
#line 1343 "compiler/main.gem"
        GemVal gem_v__for_items_54 = gem_v_extern_fns_list;
#line 1343 "compiler/main.gem"
        GemVal gem_v__for_i_54 = gem_int(0);
#line 1343 "compiler/main.gem"
        while (1) {
            GemVal _t2351[] = {gem_v__for_items_54};
            if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_len_fn(NULL, _t2351, 1)))) break;
#line 1343 "compiler/main.gem"
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_54, gem_v__for_i_54);
#line 1343 "compiler/main.gem"
            gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 1344 "compiler/main.gem"
    GemVal _t2352 = gem_v_ef;
    GemVal _t2353[] = {(*gem_v_defined_fns), gem_table_get(_t2352, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2353, 2));
        }
    }
    {
#line 1348 "compiler/main.gem"
        GemVal gem_v__for_items_55 = gem_v_fn_defs;
#line 1348 "compiler/main.gem"
        GemVal gem_v__for_i_55 = gem_int(0);
#line 1348 "compiler/main.gem"
        while (1) {
            GemVal _t2354[] = {gem_v__for_items_55};
            if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_len_fn(NULL, _t2354, 1)))) break;
#line 1348 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_55, gem_v__for_i_55);
#line 1348 "compiler/main.gem"
            gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1349 "compiler/main.gem"
    GemVal _t2355 = gem_v_fd;
    GemVal _t2356[] = {(*gem_v_defined_fns), gem_table_get(_t2355, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2356, 2));
#line 1350 "compiler/main.gem"
    GemVal _t2357 = gem_v_fd;
    GemVal _t2358[] = {gem_table_get(_t2357, gem_string("name"))};
            GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2358, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1351 "compiler/main.gem"
    GemVal _t2359[] = {(*gem_v_forward_decls), gem_v_decl};
            (void)(gem_push_fn(NULL, _t2359, 2));
        }
    }
    {
#line 1355 "compiler/main.gem"
        GemVal gem_v__for_items_56 = gem_v_extern_fns_list;
#line 1355 "compiler/main.gem"
        GemVal gem_v__for_i_56 = gem_int(0);
#line 1355 "compiler/main.gem"
        while (1) {
            GemVal _t2360[] = {gem_v__for_items_56};
            if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_len_fn(NULL, _t2360, 1)))) break;
#line 1355 "compiler/main.gem"
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_56, gem_v__for_i_56);
#line 1355 "compiler/main.gem"
            gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1356 "compiler/main.gem"
    GemVal _t2361 = (*gem_v_compile_extern_fn);
    GemVal _t2362[] = {gem_v_ef};
    GemVal _t2363[] = {(*gem_v_functions), _t2361.fn(_t2361.env, _t2362, 1)};
            (void)(gem_push_fn(NULL, _t2363, 2));
        }
    }
    {
#line 1360 "compiler/main.gem"
        GemVal gem_v__for_items_57 = gem_v_fn_defs;
#line 1360 "compiler/main.gem"
        GemVal gem_v__for_i_57 = gem_int(0);
#line 1360 "compiler/main.gem"
        while (1) {
            GemVal _t2364[] = {gem_v__for_items_57};
            if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t2364, 1)))) break;
#line 1360 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
#line 1360 "compiler/main.gem"
            gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
#line 1361 "compiler/main.gem"
    GemVal _t2365 = (*gem_v_compile_fn);
    GemVal _t2366[] = {gem_v_fd};
    GemVal _t2367[] = {(*gem_v_functions), _t2365.fn(_t2365.env, _t2366, 1)};
            (void)(gem_push_fn(NULL, _t2367, 2));
        }
    }
#line 1365 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1366 "compiler/main.gem"
    GemVal _t2368 = gem_table_new();
    GemVal _t2369 = (*gem_v_find_captured_in_scope);
    GemVal _t2370[] = {gem_v_top_stmts, _t2368};
    *gem_v_boxed_vars = _t2369.fn(_t2369.env, _t2370, 2);
#line 1367 "compiler/main.gem"
    GemVal _t2371 = (*gem_v_compile_stmts);
    GemVal _t2372[] = {gem_v_top_stmts, gem_int(1)};
    GemVal gem_v_main_body = _t2371.fn(_t2371.env, _t2372, 2);
#line 1368 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1370 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
    {
#line 1371 "compiler/main.gem"
        GemVal gem_v__for_items_58 = gem_v_fn_defs;
#line 1371 "compiler/main.gem"
        GemVal gem_v__for_i_58 = gem_int(0);
#line 1371 "compiler/main.gem"
        while (1) {
            GemVal _t2373[] = {gem_v__for_items_58};
            if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t2373, 1)))) break;
#line 1371 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1371 "compiler/main.gem"
            gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1372 "compiler/main.gem"
    GemVal _t2374 = gem_v_fd;
            if (gem_truthy(gem_eq(gem_table_get(_t2374, gem_string("name")), gem_string("main")))) {
#line 1373 "compiler/main.gem"
                gem_v_has_main = gem_bool(1);
            }
        }
    }
#line 1377 "compiler/main.gem"
    GemVal gem_v_main_c = gem_string("int main(void) {\n");
#line 1378 "compiler/main.gem"
    GemVal _t2375[] = {(*gem_v_source_name)};
    GemVal _t2376[] = {gem_fn_escape_c_string(NULL, _t2375, 1)};
    gem_v_main_c = gem_add(gem_v_main_c, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t2376, 1)), gem_string("\", 0);\n")));
#line 1379 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1380 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
#line 1382 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1383 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
#line 1384 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
#line 1386 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_pop_frame();\n    return 0;\n}\n"));
#line 1390 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
    {
#line 1391 "compiler/main.gem"
        GemVal gem_v__for_items_59 = gem_v_extern_includes;
#line 1391 "compiler/main.gem"
        GemVal gem_v__for_i_59 = gem_int(0);
#line 1391 "compiler/main.gem"
        while (1) {
            GemVal _t2377[] = {gem_v__for_items_59};
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_len_fn(NULL, _t2377, 1)))) break;
#line 1391 "compiler/main.gem"
            GemVal gem_v_ei = gem_table_get(gem_v__for_items_59, gem_v__for_i_59);
#line 1391 "compiler/main.gem"
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1392 "compiler/main.gem"
    GemVal _t2378 = gem_v_ei;
    GemVal _t2379[] = {gem_table_get(_t2378, gem_string("path"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t2379, 1)), gem_string("\"\n")));
        }
    }
#line 1394 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
    {
#line 1397 "compiler/main.gem"
        GemVal gem_v__for_i_60 = gem_int(0);
#line 1397 "compiler/main.gem"
    GemVal _t2380[] = {(*gem_v_forward_decls)};
        GemVal gem_v__for_limit_60 = gem_len_fn(NULL, _t2380, 1);
#line 1397 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1397 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_60;
#line 1397 "compiler/main.gem"
            gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1398 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
#line 1399 "compiler/main.gem"
    GemVal _t2381[] = {(*gem_v_forward_decls)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2381, 1), gem_int(1))))) {
#line 1400 "compiler/main.gem"
                gem_v_out = gem_add(gem_v_out, gem_string("\n"));
            }
        }
    }
#line 1403 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
    {
#line 1406 "compiler/main.gem"
        GemVal gem_v__for_i_61 = gem_int(0);
#line 1406 "compiler/main.gem"
    GemVal _t2382[] = {(*gem_v_functions)};
        GemVal gem_v__for_limit_61 = gem_len_fn(NULL, _t2382, 1);
#line 1406 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_v__for_limit_61))) break;
#line 1406 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_61;
#line 1406 "compiler/main.gem"
            gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1407 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
#line 1408 "compiler/main.gem"
    GemVal _t2383[] = {(*gem_v_functions)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2383, 1), gem_int(1))))) {
#line 1409 "compiler/main.gem"
                gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
            }
        }
    }
#line 1412 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
#line 1414 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    GemVal _t2384 = gem_v_out;
    gem_pop_frame();
    return _t2384;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 133 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_codegen", "compiler/main.gem", 133);
#line 134 "compiler/main.gem"
    GemVal _t926 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t926;
#line 135 "compiler/main.gem"
    GemVal _t927 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t927;
#line 136 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 137 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 138 "compiler/main.gem"
    GemVal _t928 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t928;
#line 139 "compiler/main.gem"
    GemVal _t929 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t929;
#line 140 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("print"), gem_bool(1));
#line 141 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("error"), gem_bool(1));
#line 142 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("len"), gem_bool(1));
#line 143 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("type"), gem_bool(1));
#line 144 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("to_string"), gem_bool(1));
#line 145 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("push"), gem_bool(1));
#line 146 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("pcall"), gem_bool(1));
#line 147 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("keys"), gem_bool(1));
#line 148 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("str_replace"), gem_bool(1));
#line 149 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("has_key"), gem_bool(1));
#line 150 "compiler/main.gem"
    GemVal _t930 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t930;
#line 154 "compiler/main.gem"
    struct _closure__anon_22 *_t933 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t933->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_22, _t933);
#line 159 "compiler/main.gem"
    struct _closure__anon_23 *_t936 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t936->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_23, _t936);
#line 164 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_24, NULL);
#line 170 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 171 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 173 "compiler/main.gem"
    struct _closure__anon_25 *_t1102 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1102->gem_v_builtins = gem_v_builtins;
    _t1102->gem_v_collect_free_node = gem_v_collect_free_node;
    _t1102->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1102->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_25, _t1102);
#line 311 "compiler/main.gem"
    struct _closure__anon_26 *_t1128 = GC_MALLOC(sizeof(struct _closure__anon_26));
    _t1128->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_26, _t1128);
#line 332 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 333 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 335 "compiler/main.gem"
    struct _closure__anon_27 *_t1211 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1211->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1211->gem_v_walk_captures = gem_v_walk_captures;
    _t1211->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_27, _t1211);
#line 415 "compiler/main.gem"
    struct _closure__anon_28 *_t1235 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1235->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_28, _t1235);
#line 432 "compiler/main.gem"
    struct _closure__anon_29 *_t1240 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t1240->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_29, _t1240);
#line 440 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 441 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 442 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 443 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 444 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 445 "compiler/main.gem"
    GemVal *gem_v_compile_if_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if_return = GEM_NIL;
#line 446 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 447 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 448 "compiler/main.gem"
    GemVal *gem_v_compile_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match_return = GEM_NIL;
#line 449 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 450 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 451 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 452 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 453 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 454 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 455 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 456 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 460 "compiler/main.gem"
    struct _closure__anon_30 *_t1379 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t1379->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1379->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t1379->gem_v_compile_array = gem_v_compile_array;
    _t1379->gem_v_compile_binop = gem_v_compile_binop;
    _t1379->gem_v_compile_call = gem_v_compile_call;
    _t1379->gem_v_compile_expr = gem_v_compile_expr;
    _t1379->gem_v_compile_table = gem_v_compile_table;
    _t1379->gem_v_defined_fns = gem_v_defined_fns;
    _t1379->gem_v_mangle = gem_v_mangle;
    _t1379->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_expr = gem_make_fn(_anon_30, _t1379);
#line 565 "compiler/main.gem"
    struct _closure__anon_31 *_t1396 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t1396->gem_v_compile_expr = gem_v_compile_expr;
    _t1396->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_31, _t1396);
#line 580 "compiler/main.gem"
    struct _closure__anon_32 *_t1410 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t1410->gem_v_compile_expr = gem_v_compile_expr;
    _t1410->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_32, _t1410);
#line 594 "compiler/main.gem"
    struct _closure__anon_33 *_t1448 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t1448->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1448->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1448->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t1448->gem_v_mangle = gem_v_mangle;
    _t1448->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_33, _t1448);
#line 622 "compiler/main.gem"
    struct _closure__anon_34 *_t1564 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t1564->gem_v_compile_expr = gem_v_compile_expr;
    _t1564->gem_v_defined_fns = gem_v_defined_fns;
    _t1564->gem_v_source_name = gem_v_source_name;
    _t1564->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_34, _t1564);
#line 750 "compiler/main.gem"
    struct _closure__anon_35 *_t1708 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t1708->gem_v_compile_expr = gem_v_compile_expr;
    _t1708->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_35, _t1708);
#line 825 "compiler/main.gem"
    struct _closure__anon_36 *_t1713 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t1713->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_36, _t1713);
#line 835 "compiler/main.gem"
    struct _closure__anon_37 *_t1873 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t1873->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1873->gem_v_compile_expr = gem_v_compile_expr;
    _t1873->gem_v_compile_if = gem_v_compile_if;
    _t1873->gem_v_compile_match = gem_v_compile_match;
    _t1873->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1873->gem_v_compile_while = gem_v_compile_while;
    _t1873->gem_v_mangle = gem_v_mangle;
    _t1873->gem_v_source_name = gem_v_source_name;
    _t1873->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_stmt = gem_make_fn(_anon_37, _t1873);
#line 906 "compiler/main.gem"
    struct _closure__anon_38 *_t2020 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2020->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2020->gem_v_compile_expr = gem_v_compile_expr;
    _t2020->gem_v_compile_if_return = gem_v_compile_if_return;
    _t2020->gem_v_compile_match_return = gem_v_compile_match_return;
    _t2020->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2020->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2020->gem_v_compile_while = gem_v_compile_while;
    _t2020->gem_v_mangle = gem_v_mangle;
    _t2020->gem_v_source_name = gem_v_source_name;
    _t2020->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_38, _t2020);
#line 965 "compiler/main.gem"
    struct _closure__anon_39 *_t2036 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2036->gem_v_compile_expr = gem_v_compile_expr;
    _t2036->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_39, _t2036);
#line 981 "compiler/main.gem"
    struct _closure__anon_40 *_t2069 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t2069->gem_v_compile_expr = gem_v_compile_expr;
    _t2069->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2069->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_40, _t2069);
#line 1016 "compiler/main.gem"
    struct _closure__anon_41 *_t2087 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2087->gem_v_compile_expr = gem_v_compile_expr;
    _t2087->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_41, _t2087);
#line 1031 "compiler/main.gem"
    struct _closure__anon_42 *_t2118 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t2118->gem_v_compile_expr = gem_v_compile_expr;
    _t2118->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2118->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_42, _t2118);
#line 1058 "compiler/main.gem"
    struct _closure__anon_43 *_t2166 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t2166->gem_v_compile_expr = gem_v_compile_expr;
    _t2166->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2166->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2166->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_43, _t2166);
#line 1104 "compiler/main.gem"
    struct _closure__anon_44 *_t2211 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t2211->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_44, _t2211);
#line 1183 "compiler/main.gem"
    struct _closure__anon_45 *_t2269 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t2269->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2269->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2269->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2269->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2269->gem_v_mangle = gem_v_mangle;
    _t2269->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_fn = gem_make_fn(_anon_45, _t2269);
#line 1248 "compiler/main.gem"
    struct _closure__anon_46 *_t2337 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t2337->gem_v_anon_name = gem_v_anon_name;
    _t2337->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2337->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2337->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2337->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2337->gem_v_functions = gem_v_functions;
    _t2337->gem_v_mangle = gem_v_mangle;
    _t2337->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_46, _t2337);
#line 1323 "compiler/main.gem"
    struct _closure__anon_47 *_t2385 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t2385->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2385->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t2385->gem_v_compile_fn = gem_v_compile_fn;
    _t2385->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2385->gem_v_defined_fns = gem_v_defined_fns;
    _t2385->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2385->gem_v_forward_decls = gem_v_forward_decls;
    _t2385->gem_v_functions = gem_v_functions;
    _t2385->gem_v_source_name = gem_v_source_name;
    GemVal gem_v_compile = gem_make_fn(_anon_47, _t2385);
    GemVal _t2386 = gem_table_new();
    gem_table_set(_t2386, gem_string("compile"), gem_v_compile);
    GemVal _t2387 = _t2386;
    gem_pop_frame();
    return _t2387;
}

static GemVal gem_fn_split_string(void *_env, GemVal *args, int argc) {
#line 1423 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("split_string", "compiler/main.gem", 1423);
#line 1424 "compiler/main.gem"
    GemVal _t2388 = gem_table_new();
    GemVal gem_v_result = _t2388;
#line 1425 "compiler/main.gem"
    GemVal gem_v_current = gem_string("");
    {
#line 1426 "compiler/main.gem"
        GemVal gem_v__for_i_62 = gem_int(0);
#line 1426 "compiler/main.gem"
    GemVal _t2389[] = {gem_v_s};
        GemVal gem_v__for_limit_62 = gem_len_fn(NULL, _t2389, 1);
#line 1426 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
#line 1426 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_62;
#line 1426 "compiler/main.gem"
            gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1427 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_v_delim))) {
#line 1428 "compiler/main.gem"
    GemVal _t2390[] = {gem_v_result, gem_v_current};
                (void)(gem_push_fn(NULL, _t2390, 2));
#line 1429 "compiler/main.gem"
                gem_v_current = gem_string("");
            } else {
#line 1431 "compiler/main.gem"
                gem_v_current = gem_add(gem_v_current, gem_table_get(gem_v_s, gem_v_i));
            }
        }
    }
#line 1434 "compiler/main.gem"
    GemVal _t2391[] = {gem_v_result, gem_v_current};
    (void)(gem_push_fn(NULL, _t2391, 2));
    GemVal _t2392 = gem_v_result;
    gem_pop_frame();
    return _t2392;
}

static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc) {
#line 1441 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("str_replace", "compiler/main.gem", 1441);
#line 1442 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 1443 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 1444 "compiler/main.gem"
    GemVal _t2393[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t2393, 1);
#line 1445 "compiler/main.gem"
    GemVal _t2394[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t2394, 1);
#line 1446 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
#line 1447 "compiler/main.gem"
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
#line 1448 "compiler/main.gem"
            GemVal gem_v_found = gem_bool(1);
#line 1449 "compiler/main.gem"
            GemVal gem_v_j = gem_int(0);
#line 1450 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
#line 1451 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
#line 1452 "compiler/main.gem"
                    gem_v_found = gem_bool(0);
                    break;
                }
#line 1455 "compiler/main.gem"
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
#line 1457 "compiler/main.gem"
            if (gem_truthy(gem_v_found)) {
#line 1458 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
#line 1459 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
#line 1463 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
#line 1464 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t2395 = gem_v_result;
    gem_pop_frame();
    return _t2395;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 19 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 19);
#line 20 "compiler/main.gem"
    GemVal _t2396 = gem_table_new();
    GemVal gem_v_new_stmts = _t2396;
#line 21 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
    {
#line 22 "compiler/main.gem"
    GemVal _t2397 = gem_v_ast;
        GemVal gem_v__for_items_1 = gem_table_get(_t2397, gem_string("stmts"));
#line 22 "compiler/main.gem"
        GemVal gem_v__for_i_1 = gem_int(0);
#line 22 "compiler/main.gem"
        while (1) {
            GemVal _t2398[] = {gem_v__for_items_1};
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t2398, 1)))) break;
#line 22 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 22 "compiler/main.gem"
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 23 "compiler/main.gem"
    GemVal _t2399[] = {gem_v_stmt};
    GemVal _t2401;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2399, 1), gem_string("table")))) {
        _t2401 = gem_eq(gem_type_fn(NULL, _t2399, 1), gem_string("table"));
    } else {
        GemVal _t2400 = gem_v_stmt;
        _t2401 = gem_eq(gem_table_get(_t2400, gem_string("tag")), gem_string("load"));
    }
            if (gem_truthy(_t2401)) {
#line 24 "compiler/main.gem"
    GemVal _t2402 = gem_v_stmt;
                GemVal gem_v_path = gem_table_get(_t2402, gem_string("path"));
#line 26 "compiler/main.gem"
    GemVal _t2403[] = {gem_v_path};
    GemVal _t2411;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t2403, 1), gem_int(4)))) {
        _t2411 = gem_lt(gem_len_fn(NULL, _t2403, 1), gem_int(4));
    } else {
        GemVal _t2404[] = {gem_v_path};
        GemVal _t2406;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2404, 1), gem_int(4))), gem_string(".")))) {
                _t2406 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2404, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t2405[] = {gem_v_path};
                _t2406 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2405, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t2408;
        if (gem_truthy(_t2406)) {
                _t2408 = _t2406;
        } else {
                GemVal _t2407[] = {gem_v_path};
                _t2408 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2407, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t2410;
        if (gem_truthy(_t2408)) {
                _t2410 = _t2408;
        } else {
                GemVal _t2409[] = {gem_v_path};
                _t2410 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2409, 1), gem_int(1))), gem_string("m"));
        }
        _t2411 = _t2410;
    }
                if (gem_truthy(_t2411)) {
#line 27 "compiler/main.gem"
                    gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
                }
#line 29 "compiler/main.gem"
    GemVal _t2412[] = {gem_v_base_dir, gem_v_path};
    GemVal _t2413[] = {gem_fn_gem_path_join(NULL, _t2412, 2)};
                GemVal gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t2413, 1);
#line 32 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                    continue;
                }
#line 35 "compiler/main.gem"
                gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 37 "compiler/main.gem"
    GemVal _t2414[] = {gem_v_full_path};
                GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2414, 1);
#line 38 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 39 "compiler/main.gem"
    GemVal _t2415[] = {gem_v_full_path};
    GemVal _t2416[] = {gem_add(gem_add(gem_string("cannot load '"), gem_to_string_fn(NULL, _t2415, 1)), gem_string("'"))};
                    (void)(gem_error_at_fn("compiler/main.gem", 39, _t2416, 1));
                }
#line 42 "compiler/main.gem"
    GemVal _t2417[] = {gem_v_source};
                GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t2417, 1);
#line 43 "compiler/main.gem"
    GemVal _t2418[] = {gem_v_full_path};
                GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t2418, 1);
#line 44 "compiler/main.gem"
    GemVal _t2419[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
                GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t2419, 3);
                {
#line 46 "compiler/main.gem"
    GemVal _t2420 = gem_v_resolved;
                    GemVal gem_v__for_items_2 = gem_table_get(_t2420, gem_string("stmts"));
#line 46 "compiler/main.gem"
                    GemVal gem_v__for_i_2 = gem_int(0);
#line 46 "compiler/main.gem"
                    while (1) {
                        GemVal _t2421[] = {gem_v__for_items_2};
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t2421, 1)))) break;
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
    GemVal _t2422[] = {gem_v_new_stmts};
    GemVal _t2423 = gem_fn_make_program(NULL, _t2422, 1);
    gem_pop_frame();
    return _t2423;
}

int main(void) {
    gem_push_frame("main", "compiler/main.gem", 0);
#line 60 "compiler/main.gem"
    GemVal gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
#line 61 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 62 "compiler/main.gem"
    GemVal _t2424[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 62, _t2424, 1));
    }
#line 65 "compiler/main.gem"
    GemVal _t2425[] = {gem_int(1)};
    GemVal gem_v_src_path = gem_fn_gem_get_argv(NULL, _t2425, 1);
#line 66 "compiler/main.gem"
    GemVal _t2426[] = {gem_v_src_path};
    GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2426, 1);
#line 67 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 68 "compiler/main.gem"
    GemVal _t2427[] = {gem_v_src_path};
    GemVal _t2428[] = {gem_add(gem_add(gem_string("cannot read '"), gem_to_string_fn(NULL, _t2427, 1)), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 68, _t2428, 1));
    }
#line 71 "compiler/main.gem"
    GemVal _t2429[] = {gem_v_source};
    GemVal gem_v_ast = gem_fn_parse_source(NULL, _t2429, 1);
#line 72 "compiler/main.gem"
    GemVal _t2430[] = {gem_v_src_path};
    GemVal gem_v_base_dir = gem_fn_gem_dirname(NULL, _t2430, 1);
#line 73 "compiler/main.gem"
    GemVal _t2431 = gem_table_new();
    GemVal gem_v_loaded = _t2431;
#line 74 "compiler/main.gem"
    GemVal _t2432[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    GemVal gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t2432, 3);
#line 76 "compiler/main.gem"
    GemVal _t2433[] = {gem_v_src_path};
    GemVal gem_v_cg = gem_fn_make_codegen(NULL, _t2433, 1);
#line 77 "compiler/main.gem"
    GemVal _t2434 = gem_v_cg;
    GemVal _t2435 = gem_table_get(_t2434, gem_string("compile"));
    GemVal _t2436[] = {gem_v_resolved_ast};
    GemVal gem_v_c_code = _t2435.fn(_t2435.env, _t2436, 1);
#line 78 "compiler/main.gem"
    GemVal _t2437[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t2437, 1));
    gem_pop_frame();
    return 0;
}

