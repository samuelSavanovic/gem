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
    GemVal _t45 = gem_table_new();
    gem_table_set(_t45, gem_string("tag"), gem_string("if"));
    gem_table_set(_t45, gem_string("cond"), gem_v_cond);
    gem_table_set(_t45, gem_string("then"), gem_v_then_body);
    gem_table_set(_t45, gem_string("else"), gem_v_else_body);
    gem_table_set(_t45, gem_string("line"), gem_v_line);
    GemVal _t46 = _t45;
    gem_pop_frame();
    return _t46;
}

static GemVal gem_fn_make_while(void *_env, GemVal *args, int argc) {
#line 104 "compiler/main.gem"
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_while", "compiler/main.gem", 104);
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
#line 108 "compiler/main.gem"
    GemVal gem_v_target = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_whens = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_match", "compiler/main.gem", 108);
    GemVal _t49 = gem_table_new();
    gem_table_set(_t49, gem_string("tag"), gem_string("match"));
    gem_table_set(_t49, gem_string("target"), gem_v_target);
    gem_table_set(_t49, gem_string("whens"), gem_v_whens);
    gem_table_set(_t49, gem_string("else"), gem_v_else_body);
    gem_table_set(_t49, gem_string("line"), gem_v_line);
    GemVal _t50 = _t49;
    gem_pop_frame();
    return _t50;
}

static GemVal gem_fn_make_when(void *_env, GemVal *args, int argc) {
#line 112 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_when", "compiler/main.gem", 112);
    GemVal _t51 = gem_table_new();
    gem_table_set(_t51, gem_string("tag"), gem_string("when_clause"));
    gem_table_set(_t51, gem_string("value"), gem_v_value);
    gem_table_set(_t51, gem_string("body"), gem_v_body);
    GemVal _t52 = _t51;
    gem_pop_frame();
    return _t52;
}

static GemVal gem_fn_make_return(void *_env, GemVal *args, int argc) {
#line 116 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_line = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_return", "compiler/main.gem", 116);
    GemVal _t53 = gem_table_new();
    gem_table_set(_t53, gem_string("tag"), gem_string("return"));
    gem_table_set(_t53, gem_string("value"), gem_v_value);
    gem_table_set(_t53, gem_string("line"), gem_v_line);
    GemVal _t54 = _t53;
    gem_pop_frame();
    return _t54;
}

static GemVal gem_fn_make_break(void *_env, GemVal *args, int argc) {
#line 120 "compiler/main.gem"
    gem_push_frame("make_break", "compiler/main.gem", 120);
    GemVal _t55 = gem_table_new();
    gem_table_set(_t55, gem_string("tag"), gem_string("break"));
    GemVal _t56 = _t55;
    gem_pop_frame();
    return _t56;
}

static GemVal gem_fn_make_continue(void *_env, GemVal *args, int argc) {
#line 124 "compiler/main.gem"
    gem_push_frame("make_continue", "compiler/main.gem", 124);
    GemVal _t57 = gem_table_new();
    gem_table_set(_t57, gem_string("tag"), gem_string("continue"));
    GemVal _t58 = _t57;
    gem_pop_frame();
    return _t58;
}

static GemVal gem_fn_make_load(void *_env, GemVal *args, int argc) {
#line 128 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_load", "compiler/main.gem", 128);
    GemVal _t59 = gem_table_new();
    gem_table_set(_t59, gem_string("tag"), gem_string("load"));
    gem_table_set(_t59, gem_string("path"), gem_v_path);
    GemVal _t60 = _t59;
    gem_pop_frame();
    return _t60;
}

static GemVal gem_fn_make_extern_fn(void *_env, GemVal *args, int argc) {
#line 132 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_extern_fn", "compiler/main.gem", 132);
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
#line 136 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_type_name = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_extern_param", "compiler/main.gem", 136);
    GemVal _t63 = gem_table_new();
    gem_table_set(_t63, gem_string("name"), gem_v_name);
    gem_table_set(_t63, gem_string("type"), gem_v_type_name);
    GemVal _t64 = _t63;
    gem_pop_frame();
    return _t64;
}

static GemVal gem_fn_make_extern_include(void *_env, GemVal *args, int argc) {
#line 140 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_extern_include", "compiler/main.gem", 140);
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
#line 111 "compiler/main.gem"
    GemVal _t103[] = {gem_int(13)};
    GemVal gem_v_CR = gem_chr_fn(NULL, _t103, 1);
#line 113 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 114 "compiler/main.gem"
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
#line 117 "compiler/main.gem"
    GemVal _t104;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t104 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t104 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t104)) {
#line 118 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 123 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 124 "compiler/main.gem"
            while (1) {
                GemVal _t105;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t105 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t105 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t105)) break;
#line 125 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 131 "compiler/main.gem"
    GemVal _t106;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t106 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t106 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t106)) {
#line 132 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 133 "compiler/main.gem"
    GemVal _t107[] = {gem_v_tokens};
    GemVal _t110;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t107, 1), gem_int(0)))) {
        _t110 = gem_gt(gem_len_fn(NULL, _t107, 1), gem_int(0));
    } else {
        GemVal _t108[] = {gem_v_tokens};
        GemVal _t109 = gem_table_get(gem_v_tokens, gem_sub(gem_len_fn(NULL, _t108, 1), gem_int(1)));
        _t110 = gem_eq(gem_table_get(_t109, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t110)) {
#line 134 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 136 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 137 "compiler/main.gem"
    GemVal _t111 = gem_table_new();
    gem_table_set(_t111, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t111, gem_string("value"), gem_string("\n"));
    gem_table_set(_t111, gem_string("line"), gem_v_line);
    GemVal _t112[] = {gem_v_tokens, _t111};
                (void)(gem_push_fn(NULL, _t112, 2));
            }
#line 139 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 140 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
            }
#line 142 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 144 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 145 "compiler/main.gem"
                GemVal gem_v_c2 = gem_table_get(gem_v_source, gem_v_pos);
#line 146 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_c2, gem_string("\n")))) {
#line 147 "compiler/main.gem"
                    gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 148 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                } else {
#line 149 "compiler/main.gem"
    GemVal _t113;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t113 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t113 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t114;
    if (gem_truthy(_t113)) {
        _t114 = _t113;
    } else {
        _t114 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t114)) {
#line 150 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 151 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 152 "compiler/main.gem"
                            while (1) {
                                GemVal _t115;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t115 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t115 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t115)) break;
#line 153 "compiler/main.gem"
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
#line 163 "compiler/main.gem"
    GemVal _t116[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t116, 1))) {
#line 164 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 165 "compiler/main.gem"
            while (1) {
                GemVal _t118;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t118 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t117[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t118 = gem_fn_is_digit(NULL, _t117, 1);
                }
                if (!gem_truthy(_t118)) break;
#line 166 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 167 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 169 "compiler/main.gem"
    GemVal _t119;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t119 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t119 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t119;
#line 170 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 171 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 172 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 173 "compiler/main.gem"
                while (1) {
                    GemVal _t121;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t121 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t120[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t121 = gem_fn_is_digit(NULL, _t120, 1);
                    }
                    if (!gem_truthy(_t121)) break;
#line 174 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 175 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 178 "compiler/main.gem"
    GemVal _t122 = gem_table_new();
    gem_table_set(_t122, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t122, gem_string("value"), gem_v_val);
    gem_table_set(_t122, gem_string("line"), gem_v_line);
    GemVal _t123[] = {gem_v_tokens, _t122};
            (void)(gem_push_fn(NULL, _t123, 2));
            continue;
        }
#line 183 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 184 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 185 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 186 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 187 "compiler/main.gem"
            while (1) {
                GemVal _t124;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t124 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t124 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t124)) break;
#line 188 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 189 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 190 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 191 "compiler/main.gem"
    GemVal _t125[] = {gem_v_line};
    GemVal _t126[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t125, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 191, _t126, 1));
                    }
#line 193 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 194 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 195 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
#line 196 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 197 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_CR);
                        } else {
#line 198 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 199 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                            } else {
#line 200 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 201 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                                } else {
#line 202 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
#line 203 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                    } else {
#line 204 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
#line 205 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                        } else {
#line 206 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
#line 207 "compiler/main.gem"
                                                gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                            } else {
#line 209 "compiler/main.gem"
                                                gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
#line 211 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 214 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 215 "compiler/main.gem"
    GemVal _t127 = gem_table_new();
    gem_table_set(_t127, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t127, gem_string("value"), gem_string(""));
    gem_table_set(_t127, gem_string("line"), gem_v_line);
    GemVal _t128[] = {gem_v_tokens, _t127};
                    (void)(gem_push_fn(NULL, _t128, 2));
#line 216 "compiler/main.gem"
    GemVal _t129 = gem_table_new();
    gem_table_set(_t129, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t129, gem_string("value"), gem_v_val);
    gem_table_set(_t129, gem_string("line"), gem_v_line);
    GemVal _t130[] = {gem_v_tokens, _t129};
                    (void)(gem_push_fn(NULL, _t130, 2));
#line 217 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 218 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 219 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 220 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 221 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 224 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 225 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 227 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 228 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 229 "compiler/main.gem"
    GemVal _t131[] = {gem_v_line};
    GemVal _t132[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t131, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 229, _t132, 1));
                }
#line 231 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 232 "compiler/main.gem"
    GemVal _t133 = gem_table_new();
    gem_table_set(_t133, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t133, gem_string("value"), gem_v_val);
    gem_table_set(_t133, gem_string("line"), gem_v_line);
    GemVal _t134[] = {gem_v_tokens, _t133};
                (void)(gem_push_fn(NULL, _t134, 2));
            }
            continue;
        }
#line 238 "compiler/main.gem"
    GemVal _t135[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t135, 1))) {
#line 239 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 240 "compiler/main.gem"
            while (1) {
                GemVal _t137;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t137 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t136[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t137 = gem_fn_is_alnum(NULL, _t136, 1);
                }
                if (!gem_truthy(_t137)) break;
#line 241 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 242 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 244 "compiler/main.gem"
    GemVal _t138[] = {gem_v_kw, gem_v_val};
            if (gem_truthy(gem_has_key_fn(NULL, _t138, 2))) {
#line 245 "compiler/main.gem"
    GemVal _t139 = gem_table_new();
    gem_table_set(_t139, gem_string("type"), gem_v_val);
    gem_table_set(_t139, gem_string("value"), gem_v_val);
    gem_table_set(_t139, gem_string("line"), gem_v_line);
    GemVal _t140[] = {gem_v_tokens, _t139};
                (void)(gem_push_fn(NULL, _t140, 2));
            } else {
#line 247 "compiler/main.gem"
    GemVal _t141 = gem_table_new();
    gem_table_set(_t141, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t141, gem_string("value"), gem_v_val);
    gem_table_set(_t141, gem_string("line"), gem_v_line);
    GemVal _t142[] = {gem_v_tokens, _t141};
                (void)(gem_push_fn(NULL, _t142, 2));
            }
            continue;
        }
#line 253 "compiler/main.gem"
        GemVal gem_v_next = gem_string("");
#line 254 "compiler/main.gem"
        if (gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
#line 255 "compiler/main.gem"
            gem_v_next = gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1)));
        }
#line 258 "compiler/main.gem"
    GemVal _t143;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t143 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t143 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t143)) {
#line 259 "compiler/main.gem"
    GemVal _t144 = gem_table_new();
    gem_table_set(_t144, gem_string("type"), gem_string("=="));
    gem_table_set(_t144, gem_string("value"), gem_string("=="));
    gem_table_set(_t144, gem_string("line"), gem_v_line);
    GemVal _t145[] = {gem_v_tokens, _t144};
            (void)(gem_push_fn(NULL, _t145, 2));
#line 260 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 263 "compiler/main.gem"
    GemVal _t146;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t146 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t146 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t146)) {
#line 264 "compiler/main.gem"
    GemVal _t147 = gem_table_new();
    gem_table_set(_t147, gem_string("type"), gem_string("!="));
    gem_table_set(_t147, gem_string("value"), gem_string("!="));
    gem_table_set(_t147, gem_string("line"), gem_v_line);
    GemVal _t148[] = {gem_v_tokens, _t147};
            (void)(gem_push_fn(NULL, _t148, 2));
#line 265 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 268 "compiler/main.gem"
    GemVal _t149;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t149 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t149 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t149)) {
#line 269 "compiler/main.gem"
    GemVal _t150 = gem_table_new();
    gem_table_set(_t150, gem_string("type"), gem_string("<="));
    gem_table_set(_t150, gem_string("value"), gem_string("<="));
    gem_table_set(_t150, gem_string("line"), gem_v_line);
    GemVal _t151[] = {gem_v_tokens, _t150};
            (void)(gem_push_fn(NULL, _t151, 2));
#line 270 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 273 "compiler/main.gem"
    GemVal _t152;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t152 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t152 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t152)) {
#line 274 "compiler/main.gem"
    GemVal _t153 = gem_table_new();
    gem_table_set(_t153, gem_string("type"), gem_string(">="));
    gem_table_set(_t153, gem_string("value"), gem_string(">="));
    gem_table_set(_t153, gem_string("line"), gem_v_line);
    GemVal _t154[] = {gem_v_tokens, _t153};
            (void)(gem_push_fn(NULL, _t154, 2));
#line 275 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 278 "compiler/main.gem"
    GemVal _t155;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t155 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t155 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t155)) {
#line 279 "compiler/main.gem"
    GemVal _t156 = gem_table_new();
    gem_table_set(_t156, gem_string("type"), gem_string("+="));
    gem_table_set(_t156, gem_string("value"), gem_string("+="));
    gem_table_set(_t156, gem_string("line"), gem_v_line);
    GemVal _t157[] = {gem_v_tokens, _t156};
            (void)(gem_push_fn(NULL, _t157, 2));
#line 280 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 283 "compiler/main.gem"
    GemVal _t158;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t158 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t158 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t158)) {
#line 284 "compiler/main.gem"
    GemVal _t159 = gem_table_new();
    gem_table_set(_t159, gem_string("type"), gem_string("-="));
    gem_table_set(_t159, gem_string("value"), gem_string("-="));
    gem_table_set(_t159, gem_string("line"), gem_v_line);
    GemVal _t160[] = {gem_v_tokens, _t159};
            (void)(gem_push_fn(NULL, _t160, 2));
#line 285 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 288 "compiler/main.gem"
    GemVal _t161;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t161 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t161 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t161)) {
#line 289 "compiler/main.gem"
    GemVal _t162 = gem_table_new();
    gem_table_set(_t162, gem_string("type"), gem_string("*="));
    gem_table_set(_t162, gem_string("value"), gem_string("*="));
    gem_table_set(_t162, gem_string("line"), gem_v_line);
    GemVal _t163[] = {gem_v_tokens, _t162};
            (void)(gem_push_fn(NULL, _t163, 2));
#line 290 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 293 "compiler/main.gem"
    GemVal _t164;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t164 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t164 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t164)) {
#line 294 "compiler/main.gem"
    GemVal _t165 = gem_table_new();
    gem_table_set(_t165, gem_string("type"), gem_string("/="));
    gem_table_set(_t165, gem_string("value"), gem_string("/="));
    gem_table_set(_t165, gem_string("line"), gem_v_line);
    GemVal _t166[] = {gem_v_tokens, _t165};
            (void)(gem_push_fn(NULL, _t166, 2));
#line 295 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 298 "compiler/main.gem"
    GemVal _t167;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t167 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t167 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t167)) {
#line 299 "compiler/main.gem"
    GemVal _t168 = gem_table_new();
    gem_table_set(_t168, gem_string("type"), gem_string("->"));
    gem_table_set(_t168, gem_string("value"), gem_string("->"));
    gem_table_set(_t168, gem_string("line"), gem_v_line);
    GemVal _t169[] = {gem_v_tokens, _t168};
            (void)(gem_push_fn(NULL, _t169, 2));
#line 300 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 305 "compiler/main.gem"
    GemVal _t170;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t170 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t170 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t171;
    if (gem_truthy(_t170)) {
        _t171 = _t170;
    } else {
        _t171 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t172;
    if (gem_truthy(_t171)) {
        _t172 = _t171;
    } else {
        _t172 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t173;
    if (gem_truthy(_t172)) {
        _t173 = _t172;
    } else {
        _t173 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t173)) {
#line 306 "compiler/main.gem"
    GemVal _t174 = gem_table_new();
    gem_table_set(_t174, gem_string("type"), gem_v_ch);
    gem_table_set(_t174, gem_string("value"), gem_v_ch);
    gem_table_set(_t174, gem_string("line"), gem_v_line);
    GemVal _t175[] = {gem_v_tokens, _t174};
            (void)(gem_push_fn(NULL, _t175, 2));
#line 307 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 310 "compiler/main.gem"
    GemVal _t176;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t176 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t176 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t177;
    if (gem_truthy(_t176)) {
        _t177 = _t176;
    } else {
        _t177 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t177)) {
#line 311 "compiler/main.gem"
    GemVal _t178 = gem_table_new();
    gem_table_set(_t178, gem_string("type"), gem_v_ch);
    gem_table_set(_t178, gem_string("value"), gem_v_ch);
    gem_table_set(_t178, gem_string("line"), gem_v_line);
    GemVal _t179[] = {gem_v_tokens, _t178};
            (void)(gem_push_fn(NULL, _t179, 2));
#line 312 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 316 "compiler/main.gem"
    GemVal _t180;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t180 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t180 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t180)) {
#line 317 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 318 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 319 "compiler/main.gem"
    GemVal _t181 = gem_table_new();
    gem_table_set(_t181, gem_string("type"), gem_string("{"));
    gem_table_set(_t181, gem_string("value"), gem_string("{"));
    gem_table_set(_t181, gem_string("line"), gem_v_line);
    GemVal _t182[] = {gem_v_tokens, _t181};
            (void)(gem_push_fn(NULL, _t182, 2));
#line 320 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 323 "compiler/main.gem"
    GemVal _t183;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t183 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t183 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t183)) {
#line 324 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 325 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 326 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 327 "compiler/main.gem"
    GemVal _t184 = gem_table_new();
    gem_table_set(_t184, gem_string("type"), gem_string("}"));
    gem_table_set(_t184, gem_string("value"), gem_string("}"));
    gem_table_set(_t184, gem_string("line"), gem_v_line);
    GemVal _t185[] = {gem_v_tokens, _t184};
                (void)(gem_push_fn(NULL, _t185, 2));
#line 328 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                continue;
            }
#line 332 "compiler/main.gem"
            gem_v_interp_depth = gem_sub(gem_v_interp_depth, gem_int(1));
#line 333 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 334 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 335 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 336 "compiler/main.gem"
            while (1) {
                GemVal _t186;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t186 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t186 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t186)) break;
#line 337 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 338 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 339 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 340 "compiler/main.gem"
    GemVal _t187[] = {gem_v_line};
    GemVal _t188[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t187, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 340, _t188, 1));
                    }
#line 342 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 343 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 344 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
#line 345 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 346 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_CR);
                        } else {
#line 347 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 348 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                            } else {
#line 349 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 350 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                                } else {
#line 351 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
#line 352 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                    } else {
#line 353 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
#line 354 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                        } else {
#line 355 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
#line 356 "compiler/main.gem"
                                                gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                            } else {
#line 358 "compiler/main.gem"
                                                gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
#line 360 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 363 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 365 "compiler/main.gem"
    GemVal _t189 = gem_table_new();
    gem_table_set(_t189, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t189, gem_string("value"), gem_v_val);
    gem_table_set(_t189, gem_string("line"), gem_v_line);
    GemVal _t190[] = {gem_v_tokens, _t189};
                    (void)(gem_push_fn(NULL, _t190, 2));
#line 366 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 367 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 368 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 369 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 370 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 373 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 374 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 376 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 377 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 378 "compiler/main.gem"
    GemVal _t191[] = {gem_v_line};
    GemVal _t192[] = {gem_add(gem_string("unterminated string interpolation at line "), gem_to_string_fn(NULL, _t191, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 378, _t192, 1));
                }
#line 380 "compiler/main.gem"
    GemVal _t193 = gem_table_new();
    gem_table_set(_t193, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t193, gem_string("value"), gem_v_val);
    gem_table_set(_t193, gem_string("line"), gem_v_line);
    GemVal _t194[] = {gem_v_tokens, _t193};
                (void)(gem_push_fn(NULL, _t194, 2));
#line 381 "compiler/main.gem"
    GemVal _t195 = gem_table_new();
    gem_table_set(_t195, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t195, gem_string("value"), gem_string(""));
    gem_table_set(_t195, gem_string("line"), gem_v_line);
    GemVal _t196[] = {gem_v_tokens, _t195};
                (void)(gem_push_fn(NULL, _t196, 2));
#line 382 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 387 "compiler/main.gem"
    GemVal _t197;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t197 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t197 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t198;
    if (gem_truthy(_t197)) {
        _t198 = _t197;
    } else {
        _t198 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t199;
    if (gem_truthy(_t198)) {
        _t199 = _t198;
    } else {
        _t199 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t200;
    if (gem_truthy(_t199)) {
        _t200 = _t199;
    } else {
        _t200 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t201;
    if (gem_truthy(_t200)) {
        _t201 = _t200;
    } else {
        _t201 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t201)) {
#line 388 "compiler/main.gem"
    GemVal _t202 = gem_table_new();
    gem_table_set(_t202, gem_string("type"), gem_v_ch);
    gem_table_set(_t202, gem_string("value"), gem_v_ch);
    gem_table_set(_t202, gem_string("line"), gem_v_line);
    GemVal _t203[] = {gem_v_tokens, _t202};
            (void)(gem_push_fn(NULL, _t203, 2));
#line 389 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 392 "compiler/main.gem"
    GemVal _t204;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t204 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t204 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t205;
    if (gem_truthy(_t204)) {
        _t205 = _t204;
    } else {
        _t205 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t206;
    if (gem_truthy(_t205)) {
        _t206 = _t205;
    } else {
        _t206 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t206)) {
#line 393 "compiler/main.gem"
    GemVal _t207 = gem_table_new();
    gem_table_set(_t207, gem_string("type"), gem_v_ch);
    gem_table_set(_t207, gem_string("value"), gem_v_ch);
    gem_table_set(_t207, gem_string("line"), gem_v_line);
    GemVal _t208[] = {gem_v_tokens, _t207};
            (void)(gem_push_fn(NULL, _t208, 2));
#line 394 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 398 "compiler/main.gem"
    GemVal _t209[] = {gem_v_ch};
    GemVal _t210[] = {gem_v_line};
    GemVal _t211[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t209, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t210, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 398, _t211, 1));
    }
#line 401 "compiler/main.gem"
    GemVal _t212 = gem_table_new();
    gem_table_set(_t212, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t212, gem_string("value"), gem_string(""));
    gem_table_set(_t212, gem_string("line"), gem_v_line);
    GemVal _t213[] = {gem_v_tokens, _t212};
    (void)(gem_push_fn(NULL, _t213, 2));
    GemVal _t214 = gem_v_tokens;
    gem_pop_frame();
    return _t214;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 10 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_source", "compiler/main.gem", 10);
#line 11 "compiler/main.gem"
    GemVal _t215[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t215, 1);
#line 12 "compiler/main.gem"
    GemVal _t216[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t216, 1);
#line 13 "compiler/main.gem"
    GemVal _t217 = gem_v_p;
    GemVal _t218 = gem_table_get(_t217, gem_string("parse"));
    GemVal _t219 = _t218.fn(_t218.env, NULL, 0);
    gem_pop_frame();
    return _t219;
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
    GemVal _t220 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t220;
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
    GemVal _t222 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t222;
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
    GemVal _t224 = gem_v_t;
    gem_pop_frame();
    return _t224;
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
    GemVal _t226 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t227 = gem_eq(gem_table_get(_t226, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t227;
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
    GemVal _t229 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t229, gem_string("type")), gem_v_tp))) {
#line 46 "compiler/main.gem"
    GemVal _t230[] = {gem_v_tp};
    GemVal _t231 = gem_v_t;
    GemVal _t232[] = {gem_table_get(_t231, gem_string("type"))};
    GemVal _t233 = gem_v_t;
    GemVal _t234[] = {gem_table_get(_t233, gem_string("line"))};
    GemVal _t235[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t230, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t232, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t234, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 46, _t235, 1));
    }
#line 48 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t236 = gem_v_t;
    gem_pop_frame();
    return _t236;
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
        GemVal _t238 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t238, gem_string("type")), gem_string("NEWLINE")))) break;
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
    GemVal _t240 = gem_table_new();
    GemVal gem_v_params = _t240;
#line 62 "compiler/main.gem"
    GemVal _t241 = (*gem_v_peek);
    GemVal _t242 = _t241.fn(_t241.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t242, gem_string("type")), gem_string("NAME")))) {
#line 63 "compiler/main.gem"
    GemVal _t243 = (*gem_v_advance);
    GemVal _t244 = _t243.fn(_t243.env, NULL, 0);
    GemVal _t245[] = {gem_v_params, gem_table_get(_t244, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t245, 2));
#line 64 "compiler/main.gem"
        while (1) {
            GemVal _t246 = (*gem_v_peek);
            GemVal _t247 = _t246.fn(_t246.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t247, gem_string("type")), gem_string(",")))) break;
#line 65 "compiler/main.gem"
    GemVal _t248 = (*gem_v_advance);
            (void)(_t248.fn(_t248.env, NULL, 0));
#line 66 "compiler/main.gem"
    GemVal _t249 = (*gem_v_expect);
    GemVal _t250[] = {gem_string("NAME")};
    GemVal _t251 = _t249.fn(_t249.env, _t250, 1);
    GemVal _t252[] = {gem_v_params, gem_table_get(_t251, gem_string("value"))};
            (void)(gem_push_fn(NULL, _t252, 2));
        }
    }
    GemVal _t253 = gem_v_params;
    gem_pop_frame();
    return _t253;
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
    GemVal _t255 = gem_table_new();
    GemVal gem_v_stmts = _t255;
#line 85 "compiler/main.gem"
    GemVal _t256 = (*gem_v_skip_nl);
    (void)(_t256.fn(_t256.env, NULL, 0));
#line 86 "compiler/main.gem"
    while (1) {
        GemVal _t257 = (*gem_v_peek);
        GemVal _t258 = _t257.fn(_t257.env, NULL, 0);
        GemVal _t261;
        if (!gem_truthy(gem_neq(gem_table_get(_t258, gem_string("type")), gem_string("end")))) {
                _t261 = gem_neq(gem_table_get(_t258, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t259 = (*gem_v_peek);
                GemVal _t260 = _t259.fn(_t259.env, NULL, 0);
                _t261 = gem_neq(gem_table_get(_t260, gem_string("type")), gem_string("elif"));
        }
        GemVal _t264;
        if (!gem_truthy(_t261)) {
                _t264 = _t261;
        } else {
                GemVal _t262 = (*gem_v_peek);
                GemVal _t263 = _t262.fn(_t262.env, NULL, 0);
                _t264 = gem_neq(gem_table_get(_t263, gem_string("type")), gem_string("else"));
        }
        GemVal _t267;
        if (!gem_truthy(_t264)) {
                _t267 = _t264;
        } else {
                GemVal _t265 = (*gem_v_peek);
                GemVal _t266 = _t265.fn(_t265.env, NULL, 0);
                _t267 = gem_neq(gem_table_get(_t266, gem_string("type")), gem_string("when"));
        }
        GemVal _t269;
        if (!gem_truthy(_t267)) {
                _t269 = _t267;
        } else {
                GemVal _t268 = (*gem_v_at_end);
                _t269 = gem_not(_t268.fn(_t268.env, NULL, 0));
        }
        if (!gem_truthy(_t269)) break;
#line 87 "compiler/main.gem"
    GemVal _t270 = (*gem_v_parse_stmt);
    GemVal _t271[] = {gem_v_stmts, _t270.fn(_t270.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t271, 2));
#line 88 "compiler/main.gem"
    GemVal _t272 = (*gem_v_skip_nl);
        (void)(_t272.fn(_t272.env, NULL, 0));
    }
    GemVal _t273 = gem_v_stmts;
    gem_pop_frame();
    return _t273;
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
    GemVal _t275 = gem_table_new();
    GemVal gem_v_stmts = _t275;
#line 98 "compiler/main.gem"
    GemVal _t276 = (*gem_v_skip_nl);
    (void)(_t276.fn(_t276.env, NULL, 0));
#line 99 "compiler/main.gem"
    while (1) {
        GemVal _t277 = (*gem_v_peek);
        GemVal _t278 = _t277.fn(_t277.env, NULL, 0);
        GemVal _t280;
        if (!gem_truthy(gem_neq(gem_table_get(_t278, gem_string("type")), gem_string("end")))) {
                _t280 = gem_neq(gem_table_get(_t278, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t279 = (*gem_v_at_end);
                _t280 = gem_not(_t279.fn(_t279.env, NULL, 0));
        }
        if (!gem_truthy(_t280)) break;
#line 100 "compiler/main.gem"
    GemVal _t281 = (*gem_v_parse_stmt);
    GemVal _t282[] = {gem_v_stmts, _t281.fn(_t281.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t282, 2));
#line 101 "compiler/main.gem"
    GemVal _t283 = (*gem_v_skip_nl);
        (void)(_t283.fn(_t283.env, NULL, 0));
    }
    GemVal _t284 = gem_v_stmts;
    gem_pop_frame();
    return _t284;
}

struct _closure__anon_10 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_fn_depth;
    GemVal *gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body;
    GemVal *gem_v_parse_params;
    GemVal *gem_v_peek;
    GemVal *gem_v_peek_at;
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
    GemVal *gem_v_peek_at = _cls->gem_v_peek_at;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    gem_push_frame("_anon_10", "compiler/main.gem", 0);
#line 110 "compiler/main.gem"
    GemVal _t286 = (*gem_v_peek);
    GemVal gem_v_t = _t286.fn(_t286.env, NULL, 0);
#line 113 "compiler/main.gem"
    GemVal _t287 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t287, gem_string("type")), gem_string("NUMBER")))) {
#line 114 "compiler/main.gem"
    GemVal _t288 = (*gem_v_advance);
        (void)(_t288.fn(_t288.env, NULL, 0));
#line 115 "compiler/main.gem"
    GemVal _t289 = gem_v_t;
    GemVal _t290[] = {gem_table_get(_t289, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t290, 1))) {
#line 116 "compiler/main.gem"
    GemVal _t291 = gem_v_t;
    GemVal _t292[] = {gem_table_get(_t291, gem_string("value"))};
    GemVal _t293[] = {gem_fn_atof(NULL, _t292, 1)};
            GemVal _t294 = gem_fn_make_float(NULL, _t293, 1);
            gem_pop_frame();
            return _t294;
        }
#line 118 "compiler/main.gem"
    GemVal _t295 = gem_v_t;
    GemVal _t296[] = {gem_table_get(_t295, gem_string("value"))};
    GemVal _t297[] = {gem_fn_str_to_int(NULL, _t296, 1)};
        GemVal _t298 = gem_fn_make_int(NULL, _t297, 1);
        gem_pop_frame();
        return _t298;
    }
#line 122 "compiler/main.gem"
    GemVal _t299 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t299, gem_string("type")), gem_string("STRING")))) {
#line 123 "compiler/main.gem"
    GemVal _t300 = (*gem_v_advance);
        (void)(_t300.fn(_t300.env, NULL, 0));
#line 124 "compiler/main.gem"
    GemVal _t301 = gem_v_t;
    GemVal _t302[] = {gem_table_get(_t301, gem_string("value"))};
        GemVal _t303 = gem_fn_make_string(NULL, _t302, 1);
        gem_pop_frame();
        return _t303;
    }
#line 128 "compiler/main.gem"
    GemVal _t304 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t304, gem_string("type")), gem_string("INTERP_START")))) {
#line 129 "compiler/main.gem"
    GemVal _t305 = (*gem_v_advance);
        (void)(_t305.fn(_t305.env, NULL, 0));
#line 130 "compiler/main.gem"
    GemVal _t306 = gem_table_new();
        GemVal gem_v_parts = _t306;
#line 131 "compiler/main.gem"
    GemVal _t307 = (*gem_v_skip_nl);
        (void)(_t307.fn(_t307.env, NULL, 0));
#line 132 "compiler/main.gem"
        while (1) {
            GemVal _t308 = (*gem_v_peek);
            GemVal _t309 = _t308.fn(_t308.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t309, gem_string("type")), gem_string("INTERP_END")))) break;
#line 133 "compiler/main.gem"
    GemVal _t310 = (*gem_v_peek);
    GemVal _t311 = _t310.fn(_t310.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t311, gem_string("type")), gem_string("STRING")))) {
#line 134 "compiler/main.gem"
    GemVal _t312 = (*gem_v_advance);
    GemVal _t313 = _t312.fn(_t312.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t313, gem_string("value"));
#line 135 "compiler/main.gem"
    GemVal _t314[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t314, 1), gem_int(0)))) {
#line 136 "compiler/main.gem"
    GemVal _t315[] = {gem_v_sval};
    GemVal _t316[] = {gem_v_parts, gem_fn_make_string(NULL, _t315, 1)};
                    (void)(gem_push_fn(NULL, _t316, 2));
                }
            } else {
#line 139 "compiler/main.gem"
    GemVal _t317 = (*gem_v_parse_expr);
    GemVal _t318[] = {gem_v_parts, _t317.fn(_t317.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t318, 2));
            }
#line 141 "compiler/main.gem"
    GemVal _t319 = (*gem_v_skip_nl);
            (void)(_t319.fn(_t319.env, NULL, 0));
        }
#line 143 "compiler/main.gem"
    GemVal _t320 = (*gem_v_expect);
    GemVal _t321[] = {gem_string("INTERP_END")};
        (void)(_t320.fn(_t320.env, _t321, 1));
#line 144 "compiler/main.gem"
    GemVal _t322[] = {gem_v_parts};
        GemVal _t323 = gem_fn_make_interp(NULL, _t322, 1);
        gem_pop_frame();
        return _t323;
    }
#line 148 "compiler/main.gem"
    GemVal _t324 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t324, gem_string("type")), gem_string("true")))) {
#line 149 "compiler/main.gem"
    GemVal _t325 = (*gem_v_advance);
        (void)(_t325.fn(_t325.env, NULL, 0));
#line 150 "compiler/main.gem"
    GemVal _t326[] = {gem_bool(1)};
        GemVal _t327 = gem_fn_make_bool(NULL, _t326, 1);
        gem_pop_frame();
        return _t327;
    }
#line 152 "compiler/main.gem"
    GemVal _t328 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t328, gem_string("type")), gem_string("false")))) {
#line 153 "compiler/main.gem"
    GemVal _t329 = (*gem_v_advance);
        (void)(_t329.fn(_t329.env, NULL, 0));
#line 154 "compiler/main.gem"
    GemVal _t330[] = {gem_bool(0)};
        GemVal _t331 = gem_fn_make_bool(NULL, _t330, 1);
        gem_pop_frame();
        return _t331;
    }
#line 158 "compiler/main.gem"
    GemVal _t332 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t332, gem_string("type")), gem_string("nil")))) {
#line 159 "compiler/main.gem"
    GemVal _t333 = (*gem_v_advance);
        (void)(_t333.fn(_t333.env, NULL, 0));
#line 160 "compiler/main.gem"
        GemVal _t334 = gem_fn_make_nil_node(NULL, NULL, 0);
        gem_pop_frame();
        return _t334;
    }
#line 164 "compiler/main.gem"
    GemVal _t335 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t335, gem_string("type")), gem_string("fn")))) {
#line 165 "compiler/main.gem"
    GemVal _t336 = (*gem_v_advance);
        (void)(_t336.fn(_t336.env, NULL, 0));
#line 166 "compiler/main.gem"
    GemVal _t337 = (*gem_v_expect);
    GemVal _t338[] = {gem_string("(")};
        (void)(_t337.fn(_t337.env, _t338, 1));
#line 167 "compiler/main.gem"
    GemVal _t339 = (*gem_v_parse_params);
        GemVal gem_v_params = _t339.fn(_t339.env, NULL, 0);
#line 168 "compiler/main.gem"
    GemVal _t340 = (*gem_v_expect);
    GemVal _t341[] = {gem_string(")")};
        (void)(_t340.fn(_t340.env, _t341, 1));
#line 169 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 170 "compiler/main.gem"
    GemVal _t342 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t342.fn(_t342.env, NULL, 0);
#line 171 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 172 "compiler/main.gem"
    GemVal _t343 = (*gem_v_expect);
    GemVal _t344[] = {gem_string("end")};
        (void)(_t343.fn(_t343.env, _t344, 1));
#line 173 "compiler/main.gem"
    GemVal _t345[] = {gem_v_params, gem_v_body};
        GemVal _t346 = gem_fn_make_anon_fn(NULL, _t345, 2);
        gem_pop_frame();
        return _t346;
    }
#line 178 "compiler/main.gem"
    GemVal _t347 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t347, gem_string("type")), gem_string("{")))) {
#line 179 "compiler/main.gem"
    GemVal _t348 = (*gem_v_advance);
        (void)(_t348.fn(_t348.env, NULL, 0));
#line 180 "compiler/main.gem"
    GemVal _t349 = (*gem_v_skip_nl);
        (void)(_t349.fn(_t349.env, NULL, 0));
#line 181 "compiler/main.gem"
    GemVal _t350 = gem_table_new();
        GemVal gem_v_entries = _t350;
#line 182 "compiler/main.gem"
        while (1) {
            GemVal _t351 = (*gem_v_peek);
            GemVal _t352 = _t351.fn(_t351.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t352, gem_string("type")), gem_string("}")))) break;
#line 183 "compiler/main.gem"
    GemVal _t353 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t353.fn(_t353.env, NULL, 0);
#line 184 "compiler/main.gem"
    GemVal _t354 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t354, gem_string("type")), gem_string("NAME")))) {
#line 185 "compiler/main.gem"
    GemVal _t355 = (*gem_v_advance);
                (void)(_t355.fn(_t355.env, NULL, 0));
            } else {
#line 186 "compiler/main.gem"
    GemVal _t356 = (*gem_v_peek_at);
    GemVal _t357[] = {gem_int(1)};
    GemVal _t358 = _t356.fn(_t356.env, _t357, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t358, gem_string("type")), gem_string(":")))) {
#line 188 "compiler/main.gem"
    GemVal _t359 = (*gem_v_advance);
                    (void)(_t359.fn(_t359.env, NULL, 0));
                } else {
#line 190 "compiler/main.gem"
    GemVal _t360 = gem_v_key_tok;
    GemVal _t361[] = {gem_table_get(_t360, gem_string("line"))};
    GemVal _t362[] = {gem_add(gem_string("expected table key at line "), gem_to_string_fn(NULL, _t361, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 190, _t362, 1));
                }
            }
#line 192 "compiler/main.gem"
    GemVal _t363 = (*gem_v_expect);
    GemVal _t364[] = {gem_string(":")};
            (void)(_t363.fn(_t363.env, _t364, 1));
#line 193 "compiler/main.gem"
    GemVal _t365 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t365.fn(_t365.env, NULL, 0);
#line 194 "compiler/main.gem"
    GemVal _t366 = gem_v_key_tok;
    GemVal _t367[] = {gem_table_get(_t366, gem_string("value")), gem_v_val};
    GemVal _t368[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t367, 2)};
            (void)(gem_push_fn(NULL, _t368, 2));
#line 195 "compiler/main.gem"
    GemVal _t369 = (*gem_v_skip_nl);
            (void)(_t369.fn(_t369.env, NULL, 0));
#line 196 "compiler/main.gem"
    GemVal _t370 = (*gem_v_peek);
    GemVal _t371 = _t370.fn(_t370.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t371, gem_string("type")), gem_string(",")))) {
#line 197 "compiler/main.gem"
    GemVal _t372 = (*gem_v_advance);
                (void)(_t372.fn(_t372.env, NULL, 0));
#line 198 "compiler/main.gem"
    GemVal _t373 = (*gem_v_skip_nl);
                (void)(_t373.fn(_t373.env, NULL, 0));
            }
        }
#line 201 "compiler/main.gem"
    GemVal _t374 = (*gem_v_expect);
    GemVal _t375[] = {gem_string("}")};
        (void)(_t374.fn(_t374.env, _t375, 1));
#line 202 "compiler/main.gem"
    GemVal _t376[] = {gem_v_entries};
        GemVal _t377 = gem_fn_make_table(NULL, _t376, 1);
        gem_pop_frame();
        return _t377;
    }
#line 206 "compiler/main.gem"
    GemVal _t378 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t378, gem_string("type")), gem_string("[")))) {
#line 207 "compiler/main.gem"
    GemVal _t379 = (*gem_v_advance);
        (void)(_t379.fn(_t379.env, NULL, 0));
#line 208 "compiler/main.gem"
    GemVal _t380 = (*gem_v_skip_nl);
        (void)(_t380.fn(_t380.env, NULL, 0));
#line 209 "compiler/main.gem"
    GemVal _t381 = gem_table_new();
        GemVal gem_v_elements = _t381;
#line 210 "compiler/main.gem"
        while (1) {
            GemVal _t382 = (*gem_v_peek);
            GemVal _t383 = _t382.fn(_t382.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t383, gem_string("type")), gem_string("]")))) break;
#line 211 "compiler/main.gem"
    GemVal _t384 = (*gem_v_parse_expr);
    GemVal _t385[] = {gem_v_elements, _t384.fn(_t384.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t385, 2));
#line 212 "compiler/main.gem"
    GemVal _t386 = (*gem_v_skip_nl);
            (void)(_t386.fn(_t386.env, NULL, 0));
#line 213 "compiler/main.gem"
    GemVal _t387 = (*gem_v_peek);
    GemVal _t388 = _t387.fn(_t387.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t388, gem_string("type")), gem_string(",")))) {
#line 214 "compiler/main.gem"
    GemVal _t389 = (*gem_v_advance);
                (void)(_t389.fn(_t389.env, NULL, 0));
#line 215 "compiler/main.gem"
    GemVal _t390 = (*gem_v_skip_nl);
                (void)(_t390.fn(_t390.env, NULL, 0));
            }
        }
#line 218 "compiler/main.gem"
    GemVal _t391 = (*gem_v_expect);
    GemVal _t392[] = {gem_string("]")};
        (void)(_t391.fn(_t391.env, _t392, 1));
#line 219 "compiler/main.gem"
    GemVal _t393[] = {gem_v_elements};
        GemVal _t394 = gem_fn_make_array(NULL, _t393, 1);
        gem_pop_frame();
        return _t394;
    }
#line 223 "compiler/main.gem"
    GemVal _t395 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t395, gem_string("type")), gem_string("(")))) {
#line 224 "compiler/main.gem"
    GemVal _t396 = (*gem_v_advance);
        (void)(_t396.fn(_t396.env, NULL, 0));
#line 225 "compiler/main.gem"
    GemVal _t397 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t397.fn(_t397.env, NULL, 0);
#line 226 "compiler/main.gem"
    GemVal _t398 = (*gem_v_expect);
    GemVal _t399[] = {gem_string(")")};
        (void)(_t398.fn(_t398.env, _t399, 1));
#line 227 "compiler/main.gem"
        GemVal _t400 = gem_v_e;
        gem_pop_frame();
        return _t400;
    }
#line 231 "compiler/main.gem"
    GemVal _t401 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t401, gem_string("type")), gem_string("NAME")))) {
#line 232 "compiler/main.gem"
    GemVal _t402 = (*gem_v_advance);
        (void)(_t402.fn(_t402.env, NULL, 0));
#line 233 "compiler/main.gem"
    GemVal _t403 = gem_v_t;
    GemVal _t404[] = {gem_table_get(_t403, gem_string("value"))};
        GemVal _t405 = gem_fn_make_var(NULL, _t404, 1);
        gem_pop_frame();
        return _t405;
    }
#line 236 "compiler/main.gem"
    GemVal _t406 = gem_v_t;
    GemVal _t407[] = {gem_table_get(_t406, gem_string("type"))};
    GemVal _t408 = gem_v_t;
    GemVal _t409[] = {gem_table_get(_t408, gem_string("line"))};
    GemVal _t410[] = {gem_add(gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t407, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t409, 1))};
    GemVal _t411 = gem_error_at_fn("compiler/main.gem", 236, _t410, 1);
    gem_pop_frame();
    return _t411;
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
#line 241 "compiler/main.gem"
    GemVal _t413 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t413.fn(_t413.env, NULL, 0);
#line 243 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
#line 245 "compiler/main.gem"
    GemVal _t414 = (*gem_v_peek);
    GemVal _t415 = _t414.fn(_t414.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t415, gem_string("type")), gem_string("(")))) {
#line 246 "compiler/main.gem"
    GemVal _t416 = (*gem_v_peek);
    GemVal _t417 = _t416.fn(_t416.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t417, gem_string("line"));
#line 247 "compiler/main.gem"
    GemVal _t418 = (*gem_v_advance);
            (void)(_t418.fn(_t418.env, NULL, 0));
#line 248 "compiler/main.gem"
    GemVal _t419 = (*gem_v_skip_nl);
            (void)(_t419.fn(_t419.env, NULL, 0));
#line 249 "compiler/main.gem"
    GemVal _t420 = gem_table_new();
            GemVal gem_v_args = _t420;
#line 250 "compiler/main.gem"
    GemVal _t421 = (*gem_v_peek);
    GemVal _t422 = _t421.fn(_t421.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t422, gem_string("type")), gem_string(")")))) {
#line 251 "compiler/main.gem"
    GemVal _t423 = (*gem_v_parse_expr);
    GemVal _t424[] = {gem_v_args, _t423.fn(_t423.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t424, 2));
#line 252 "compiler/main.gem"
                while (1) {
                    GemVal _t425 = (*gem_v_peek);
                    GemVal _t426 = _t425.fn(_t425.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t426, gem_string("type")), gem_string(",")))) break;
#line 253 "compiler/main.gem"
    GemVal _t427 = (*gem_v_advance);
                    (void)(_t427.fn(_t427.env, NULL, 0));
#line 254 "compiler/main.gem"
    GemVal _t428 = (*gem_v_skip_nl);
                    (void)(_t428.fn(_t428.env, NULL, 0));
#line 255 "compiler/main.gem"
    GemVal _t429 = (*gem_v_parse_expr);
    GemVal _t430[] = {gem_v_args, _t429.fn(_t429.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t430, 2));
                }
            }
#line 258 "compiler/main.gem"
    GemVal _t431 = (*gem_v_skip_nl);
            (void)(_t431.fn(_t431.env, NULL, 0));
#line 259 "compiler/main.gem"
    GemVal _t432 = (*gem_v_expect);
    GemVal _t433[] = {gem_string(")")};
            (void)(_t432.fn(_t432.env, _t433, 1));
#line 262 "compiler/main.gem"
    GemVal _t434 = (*gem_v_peek);
    GemVal _t435 = _t434.fn(_t434.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t435, gem_string("type")), gem_string("do")))) {
#line 263 "compiler/main.gem"
    GemVal _t436 = (*gem_v_advance);
                (void)(_t436.fn(_t436.env, NULL, 0));
#line 264 "compiler/main.gem"
    GemVal _t437 = gem_table_new();
                GemVal gem_v_bparams = _t437;
#line 265 "compiler/main.gem"
    GemVal _t438 = (*gem_v_peek);
    GemVal _t439 = _t438.fn(_t438.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t439, gem_string("type")), gem_string("|")))) {
#line 266 "compiler/main.gem"
    GemVal _t440 = (*gem_v_advance);
                    (void)(_t440.fn(_t440.env, NULL, 0));
#line 267 "compiler/main.gem"
    GemVal _t441 = (*gem_v_parse_params);
                    gem_v_bparams = _t441.fn(_t441.env, NULL, 0);
#line 268 "compiler/main.gem"
    GemVal _t442 = (*gem_v_expect);
    GemVal _t443[] = {gem_string("|")};
                    (void)(_t442.fn(_t442.env, _t443, 1));
                }
#line 270 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 271 "compiler/main.gem"
    GemVal _t444 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t444.fn(_t444.env, NULL, 0);
#line 272 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 273 "compiler/main.gem"
    GemVal _t445 = (*gem_v_expect);
    GemVal _t446[] = {gem_string("end")};
                (void)(_t445.fn(_t445.env, _t446, 1));
#line 274 "compiler/main.gem"
    GemVal _t447[] = {gem_v_bparams, gem_v_body};
    GemVal _t448[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t447, 2)};
                (void)(gem_push_fn(NULL, _t448, 2));
            } else {
#line 278 "compiler/main.gem"
    GemVal _t449 = (*gem_v_peek);
    GemVal _t450 = _t449.fn(_t449.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t450, gem_string("type")), gem_string("{")))) {
#line 279 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 280 "compiler/main.gem"
    GemVal _t451 = (*gem_v_peek_at);
    GemVal _t452[] = {gem_int(1)};
    GemVal _t453 = _t451.fn(_t451.env, _t452, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t453, gem_string("type")), gem_string("|")))) {
#line 281 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 282 "compiler/main.gem"
    GemVal _t454 = (*gem_v_peek_at);
    GemVal _t455[] = {gem_int(1)};
    GemVal _t456 = _t454.fn(_t454.env, _t455, 1);
    GemVal _t464;
    if (gem_truthy(gem_eq(gem_table_get(_t456, gem_string("type")), gem_string("}")))) {
        _t464 = gem_eq(gem_table_get(_t456, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t457 = (*gem_v_peek_at);
        GemVal _t458[] = {gem_int(1)};
        GemVal _t459 = _t457.fn(_t457.env, _t458, 1);
        GemVal _t463;
        if (!gem_truthy(gem_eq(gem_table_get(_t459, gem_string("type")), gem_string("NAME")))) {
                _t463 = gem_eq(gem_table_get(_t459, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t460 = (*gem_v_peek_at);
                GemVal _t461[] = {gem_int(2)};
                GemVal _t462 = _t460.fn(_t460.env, _t461, 1);
                _t463 = gem_eq(gem_table_get(_t462, gem_string("type")), gem_string(":"));
        }
        _t464 = _t463;
    }
                        if (gem_truthy(_t464)) {
#line 283 "compiler/main.gem"
                            gem_v_is_block = gem_bool(0);
                        } else {
#line 285 "compiler/main.gem"
                            gem_v_is_block = gem_bool(1);
                        }
                    }
#line 287 "compiler/main.gem"
                    if (gem_truthy(gem_v_is_block)) {
#line 288 "compiler/main.gem"
    GemVal _t465 = (*gem_v_advance);
                        (void)(_t465.fn(_t465.env, NULL, 0));
#line 289 "compiler/main.gem"
    GemVal _t466 = gem_table_new();
                        GemVal gem_v_bparams = _t466;
#line 290 "compiler/main.gem"
    GemVal _t467 = (*gem_v_peek);
    GemVal _t468 = _t467.fn(_t467.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t468, gem_string("type")), gem_string("|")))) {
#line 291 "compiler/main.gem"
    GemVal _t469 = (*gem_v_advance);
                            (void)(_t469.fn(_t469.env, NULL, 0));
#line 292 "compiler/main.gem"
    GemVal _t470 = (*gem_v_parse_params);
                            gem_v_bparams = _t470.fn(_t470.env, NULL, 0);
#line 293 "compiler/main.gem"
    GemVal _t471 = (*gem_v_expect);
    GemVal _t472[] = {gem_string("|")};
                            (void)(_t471.fn(_t471.env, _t472, 1));
                        }
#line 295 "compiler/main.gem"
    GemVal _t473 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t473.fn(_t473.env, NULL, 0);
#line 296 "compiler/main.gem"
    GemVal _t474 = (*gem_v_expect);
    GemVal _t475[] = {gem_string("}")};
                        (void)(_t474.fn(_t474.env, _t475, 1));
#line 297 "compiler/main.gem"
    GemVal _t476 = gem_table_new();
    gem_table_set(_t476, gem_int(0), gem_v_bexpr);
    GemVal _t477[] = {gem_v_bparams, _t476};
    GemVal _t478[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t477, 2)};
                        (void)(gem_push_fn(NULL, _t478, 2));
                    }
                }
            }
#line 302 "compiler/main.gem"
    GemVal _t479[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t479, 3);
            continue;
        }
#line 307 "compiler/main.gem"
    GemVal _t480 = (*gem_v_peek);
    GemVal _t481 = _t480.fn(_t480.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t481, gem_string("type")), gem_string(".")))) {
#line 308 "compiler/main.gem"
    GemVal _t482 = (*gem_v_advance);
            (void)(_t482.fn(_t482.env, NULL, 0));
#line 309 "compiler/main.gem"
    GemVal _t483 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t483.fn(_t483.env, NULL, 0);
#line 310 "compiler/main.gem"
    GemVal _t484 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t484, gem_string("type")), gem_string("NAME")))) {
#line 311 "compiler/main.gem"
    GemVal _t485 = (*gem_v_advance);
                (void)(_t485.fn(_t485.env, NULL, 0));
            } else {
#line 314 "compiler/main.gem"
    GemVal _t486 = (*gem_v_advance);
                (void)(_t486.fn(_t486.env, NULL, 0));
            }
#line 316 "compiler/main.gem"
    GemVal _t487 = gem_v_field_tok;
    GemVal _t488[] = {gem_v_node, gem_table_get(_t487, gem_string("value"))};
            gem_v_node = gem_fn_make_dot(NULL, _t488, 2);
            continue;
        }
#line 321 "compiler/main.gem"
    GemVal _t489 = (*gem_v_peek);
    GemVal _t490 = _t489.fn(_t489.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t490, gem_string("type")), gem_string("[")))) {
#line 322 "compiler/main.gem"
    GemVal _t491 = (*gem_v_advance);
            (void)(_t491.fn(_t491.env, NULL, 0));
#line 323 "compiler/main.gem"
    GemVal _t492 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t492.fn(_t492.env, NULL, 0);
#line 324 "compiler/main.gem"
    GemVal _t493 = (*gem_v_expect);
    GemVal _t494[] = {gem_string("]")};
            (void)(_t493.fn(_t493.env, _t494, 1));
#line 325 "compiler/main.gem"
    GemVal _t495[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t495, 2);
            continue;
        }
        break;
    }
    GemVal _t496 = gem_v_node;
    gem_pop_frame();
    return _t496;
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
#line 337 "compiler/main.gem"
    GemVal _t498 = (*gem_v_peek);
    GemVal _t499 = _t498.fn(_t498.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t499, gem_string("type")), gem_string("-")))) {
#line 338 "compiler/main.gem"
    GemVal _t500 = (*gem_v_advance);
        (void)(_t500.fn(_t500.env, NULL, 0));
#line 339 "compiler/main.gem"
    GemVal _t501 = (*gem_v_parse_unary);
    GemVal _t502[] = {gem_string("-"), _t501.fn(_t501.env, NULL, 0)};
        GemVal _t503 = gem_fn_make_unop(NULL, _t502, 2);
        gem_pop_frame();
        return _t503;
    }
#line 341 "compiler/main.gem"
    GemVal _t504 = (*gem_v_parse_call);
    GemVal _t505 = _t504.fn(_t504.env, NULL, 0);
    gem_pop_frame();
    return _t505;
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
#line 346 "compiler/main.gem"
    GemVal _t507 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t507.fn(_t507.env, NULL, 0);
#line 347 "compiler/main.gem"
    while (1) {
        GemVal _t508 = (*gem_v_peek);
        GemVal _t509 = _t508.fn(_t508.env, NULL, 0);
        GemVal _t512;
        if (gem_truthy(gem_eq(gem_table_get(_t509, gem_string("type")), gem_string("*")))) {
                _t512 = gem_eq(gem_table_get(_t509, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t510 = (*gem_v_peek);
                GemVal _t511 = _t510.fn(_t510.env, NULL, 0);
                _t512 = gem_eq(gem_table_get(_t511, gem_string("type")), gem_string("/"));
        }
        GemVal _t515;
        if (gem_truthy(_t512)) {
                _t515 = _t512;
        } else {
                GemVal _t513 = (*gem_v_peek);
                GemVal _t514 = _t513.fn(_t513.env, NULL, 0);
                _t515 = gem_eq(gem_table_get(_t514, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t515)) break;
#line 348 "compiler/main.gem"
    GemVal _t516 = (*gem_v_advance);
    GemVal _t517 = _t516.fn(_t516.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t517, gem_string("type"));
#line 349 "compiler/main.gem"
    GemVal _t518 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t518.fn(_t518.env, NULL, 0);
#line 350 "compiler/main.gem"
    GemVal _t519[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t519, 3);
    }
    GemVal _t520 = gem_v_left;
    gem_pop_frame();
    return _t520;
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
#line 357 "compiler/main.gem"
    GemVal _t522 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t522.fn(_t522.env, NULL, 0);
#line 358 "compiler/main.gem"
    while (1) {
        GemVal _t523 = (*gem_v_peek);
        GemVal _t524 = _t523.fn(_t523.env, NULL, 0);
        GemVal _t527;
        if (gem_truthy(gem_eq(gem_table_get(_t524, gem_string("type")), gem_string("+")))) {
                _t527 = gem_eq(gem_table_get(_t524, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t525 = (*gem_v_peek);
                GemVal _t526 = _t525.fn(_t525.env, NULL, 0);
                _t527 = gem_eq(gem_table_get(_t526, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t527)) break;
#line 359 "compiler/main.gem"
    GemVal _t528 = (*gem_v_advance);
    GemVal _t529 = _t528.fn(_t528.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t529, gem_string("type"));
#line 360 "compiler/main.gem"
    GemVal _t530 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t530.fn(_t530.env, NULL, 0);
#line 361 "compiler/main.gem"
    GemVal _t531[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t531, 3);
    }
    GemVal _t532 = gem_v_left;
    gem_pop_frame();
    return _t532;
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
#line 368 "compiler/main.gem"
    GemVal _t534 = (*gem_v_parse_add);
    GemVal gem_v_left = _t534.fn(_t534.env, NULL, 0);
#line 369 "compiler/main.gem"
    while (1) {
        GemVal _t535 = (*gem_v_peek);
        GemVal _t536 = _t535.fn(_t535.env, NULL, 0);
        GemVal _t539;
        if (gem_truthy(gem_eq(gem_table_get(_t536, gem_string("type")), gem_string("==")))) {
                _t539 = gem_eq(gem_table_get(_t536, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t537 = (*gem_v_peek);
                GemVal _t538 = _t537.fn(_t537.env, NULL, 0);
                _t539 = gem_eq(gem_table_get(_t538, gem_string("type")), gem_string("!="));
        }
        GemVal _t542;
        if (gem_truthy(_t539)) {
                _t542 = _t539;
        } else {
                GemVal _t540 = (*gem_v_peek);
                GemVal _t541 = _t540.fn(_t540.env, NULL, 0);
                _t542 = gem_eq(gem_table_get(_t541, gem_string("type")), gem_string("<"));
        }
        GemVal _t545;
        if (gem_truthy(_t542)) {
                _t545 = _t542;
        } else {
                GemVal _t543 = (*gem_v_peek);
                GemVal _t544 = _t543.fn(_t543.env, NULL, 0);
                _t545 = gem_eq(gem_table_get(_t544, gem_string("type")), gem_string(">"));
        }
        GemVal _t548;
        if (gem_truthy(_t545)) {
                _t548 = _t545;
        } else {
                GemVal _t546 = (*gem_v_peek);
                GemVal _t547 = _t546.fn(_t546.env, NULL, 0);
                _t548 = gem_eq(gem_table_get(_t547, gem_string("type")), gem_string("<="));
        }
        GemVal _t551;
        if (gem_truthy(_t548)) {
                _t551 = _t548;
        } else {
                GemVal _t549 = (*gem_v_peek);
                GemVal _t550 = _t549.fn(_t549.env, NULL, 0);
                _t551 = gem_eq(gem_table_get(_t550, gem_string("type")), gem_string(">="));
        }
        GemVal _t554;
        if (gem_truthy(_t551)) {
                _t554 = _t551;
        } else {
                GemVal _t552 = (*gem_v_peek);
                GemVal _t553 = _t552.fn(_t552.env, NULL, 0);
                _t554 = gem_eq(gem_table_get(_t553, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t554)) break;
#line 370 "compiler/main.gem"
    GemVal _t555 = (*gem_v_advance);
    GemVal _t556 = _t555.fn(_t555.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t556, gem_string("type"));
#line 371 "compiler/main.gem"
    GemVal _t557 = (*gem_v_parse_add);
        GemVal gem_v_right = _t557.fn(_t557.env, NULL, 0);
#line 372 "compiler/main.gem"
    GemVal _t558[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t558, 3);
    }
    GemVal _t559 = gem_v_left;
    gem_pop_frame();
    return _t559;
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
#line 379 "compiler/main.gem"
    GemVal _t561 = (*gem_v_peek);
    GemVal _t562 = _t561.fn(_t561.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t562, gem_string("type")), gem_string("not")))) {
#line 380 "compiler/main.gem"
    GemVal _t563 = (*gem_v_advance);
        (void)(_t563.fn(_t563.env, NULL, 0));
#line 381 "compiler/main.gem"
    GemVal _t564 = (*gem_v_parse_not);
    GemVal _t565[] = {gem_string("not"), _t564.fn(_t564.env, NULL, 0)};
        GemVal _t566 = gem_fn_make_unop(NULL, _t565, 2);
        gem_pop_frame();
        return _t566;
    }
#line 383 "compiler/main.gem"
    GemVal _t567 = (*gem_v_parse_compare);
    GemVal _t568 = _t567.fn(_t567.env, NULL, 0);
    gem_pop_frame();
    return _t568;
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
#line 388 "compiler/main.gem"
    GemVal _t570 = (*gem_v_parse_not);
    GemVal gem_v_left = _t570.fn(_t570.env, NULL, 0);
#line 389 "compiler/main.gem"
    while (1) {
        GemVal _t571 = (*gem_v_peek);
        GemVal _t572 = _t571.fn(_t571.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t572, gem_string("type")), gem_string("and")))) break;
#line 390 "compiler/main.gem"
    GemVal _t573 = (*gem_v_advance);
        (void)(_t573.fn(_t573.env, NULL, 0));
#line 391 "compiler/main.gem"
    GemVal _t574 = (*gem_v_parse_not);
        GemVal gem_v_right = _t574.fn(_t574.env, NULL, 0);
#line 392 "compiler/main.gem"
    GemVal _t575[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t575, 3);
    }
    GemVal _t576 = gem_v_left;
    gem_pop_frame();
    return _t576;
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
#line 399 "compiler/main.gem"
    GemVal _t578 = (*gem_v_parse_and);
    GemVal gem_v_left = _t578.fn(_t578.env, NULL, 0);
#line 400 "compiler/main.gem"
    while (1) {
        GemVal _t579 = (*gem_v_peek);
        GemVal _t580 = _t579.fn(_t579.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t580, gem_string("type")), gem_string("or")))) break;
#line 401 "compiler/main.gem"
    GemVal _t581 = (*gem_v_advance);
        (void)(_t581.fn(_t581.env, NULL, 0));
#line 402 "compiler/main.gem"
    GemVal _t582 = (*gem_v_parse_and);
        GemVal gem_v_right = _t582.fn(_t582.env, NULL, 0);
#line 403 "compiler/main.gem"
    GemVal _t583[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t583, 3);
    }
    GemVal _t584 = gem_v_left;
    gem_pop_frame();
    return _t584;
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
#line 410 "compiler/main.gem"
    GemVal _t586 = (*gem_v_peek);
    GemVal _t587 = _t586.fn(_t586.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t587, gem_string("line"));
#line 411 "compiler/main.gem"
    GemVal _t588 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t588.fn(_t588.env, NULL, 0);
#line 414 "compiler/main.gem"
    GemVal _t589 = (*gem_v_peek);
    GemVal _t590 = _t589.fn(_t589.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t590, gem_string("type")), gem_string("=")))) {
#line 415 "compiler/main.gem"
    GemVal _t591 = (*gem_v_advance);
        (void)(_t591.fn(_t591.env, NULL, 0));
#line 416 "compiler/main.gem"
    GemVal _t592 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t592.fn(_t592.env, NULL, 0);
#line 417 "compiler/main.gem"
    GemVal _t593 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t593, gem_string("tag")), gem_string("var")))) {
#line 418 "compiler/main.gem"
    GemVal _t594 = gem_v_lhs;
    GemVal _t595[] = {gem_table_get(_t594, gem_string("name")), gem_v_value, gem_v_start_line};
            GemVal _t596 = gem_fn_make_assign(NULL, _t595, 3);
            gem_pop_frame();
            return _t596;
        }
#line 420 "compiler/main.gem"
    GemVal _t597 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t597, gem_string("tag")), gem_string("dot")))) {
#line 421 "compiler/main.gem"
    GemVal _t598 = gem_v_lhs;
    GemVal _t599 = gem_v_lhs;
    GemVal _t600[] = {gem_table_get(_t598, gem_string("object")), gem_table_get(_t599, gem_string("field")), gem_v_value, gem_v_start_line};
            GemVal _t601 = gem_fn_make_dot_assign(NULL, _t600, 4);
            gem_pop_frame();
            return _t601;
        }
#line 423 "compiler/main.gem"
    GemVal _t602 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t602, gem_string("tag")), gem_string("index")))) {
#line 424 "compiler/main.gem"
    GemVal _t603 = gem_v_lhs;
    GemVal _t604 = gem_v_lhs;
    GemVal _t605[] = {gem_table_get(_t603, gem_string("object")), gem_table_get(_t604, gem_string("key")), gem_v_value, gem_v_start_line};
            GemVal _t606 = gem_fn_make_index_assign(NULL, _t605, 4);
            gem_pop_frame();
            return _t606;
        }
#line 426 "compiler/main.gem"
    GemVal _t607 = (*gem_v_peek);
    GemVal _t608 = _t607.fn(_t607.env, NULL, 0);
    GemVal _t609[] = {gem_table_get(_t608, gem_string("line"))};
    GemVal _t610[] = {gem_add(gem_string("invalid assignment target at line "), gem_to_string_fn(NULL, _t609, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 426, _t610, 1));
    }
#line 430 "compiler/main.gem"
    GemVal _t611 = (*gem_v_peek);
    GemVal _t612 = _t611.fn(_t611.env, NULL, 0);
    GemVal _t615;
    if (gem_truthy(gem_eq(gem_table_get(_t612, gem_string("type")), gem_string("+=")))) {
        _t615 = gem_eq(gem_table_get(_t612, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t613 = (*gem_v_peek);
        GemVal _t614 = _t613.fn(_t613.env, NULL, 0);
        _t615 = gem_eq(gem_table_get(_t614, gem_string("type")), gem_string("-="));
    }
    GemVal _t618;
    if (gem_truthy(_t615)) {
        _t618 = _t615;
    } else {
        GemVal _t616 = (*gem_v_peek);
        GemVal _t617 = _t616.fn(_t616.env, NULL, 0);
        _t618 = gem_eq(gem_table_get(_t617, gem_string("type")), gem_string("*="));
    }
    GemVal _t621;
    if (gem_truthy(_t618)) {
        _t621 = _t618;
    } else {
        GemVal _t619 = (*gem_v_peek);
        GemVal _t620 = _t619.fn(_t619.env, NULL, 0);
        _t621 = gem_eq(gem_table_get(_t620, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t621)) {
#line 431 "compiler/main.gem"
    GemVal _t622 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t622.fn(_t622.env, NULL, 0);
#line 432 "compiler/main.gem"
    GemVal _t623 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t623, gem_string("value")), gem_int(0));
#line 433 "compiler/main.gem"
    GemVal _t624 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t624.fn(_t624.env, NULL, 0);
#line 434 "compiler/main.gem"
    GemVal _t625 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t625, gem_string("tag")), gem_string("var")))) {
#line 435 "compiler/main.gem"
    GemVal _t626 = gem_v_op_tok;
    GemVal _t627[] = {gem_table_get(_t626, gem_string("line"))};
    GemVal _t628[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t627, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 435, _t628, 1));
        }
#line 437 "compiler/main.gem"
    GemVal _t629 = gem_v_lhs;
    GemVal _t630 = gem_v_lhs;
    GemVal _t631[] = {gem_table_get(_t630, gem_string("name"))};
    GemVal _t632[] = {gem_v_base_op, gem_fn_make_var(NULL, _t631, 1), gem_v_value};
    GemVal _t633[] = {gem_table_get(_t629, gem_string("name")), gem_fn_make_binop(NULL, _t632, 3), gem_v_start_line};
        GemVal _t634 = gem_fn_make_assign(NULL, _t633, 3);
        gem_pop_frame();
        return _t634;
    }
    GemVal _t635 = gem_v_lhs;
    gem_pop_frame();
    return _t635;
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
#line 449 "compiler/main.gem"
    GemVal _t637 = (*gem_v_peek);
    GemVal gem_v_t = _t637.fn(_t637.env, NULL, 0);
#line 452 "compiler/main.gem"
    GemVal _t638 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t638, gem_string("type")), gem_string("let")))) {
#line 453 "compiler/main.gem"
    GemVal _t639 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t639, gem_string("line"));
#line 454 "compiler/main.gem"
    GemVal _t640 = (*gem_v_advance);
        (void)(_t640.fn(_t640.env, NULL, 0));
#line 455 "compiler/main.gem"
    GemVal _t641 = (*gem_v_expect);
    GemVal _t642[] = {gem_string("NAME")};
    GemVal _t643 = _t641.fn(_t641.env, _t642, 1);
        GemVal gem_v_name = gem_table_get(_t643, gem_string("value"));
#line 456 "compiler/main.gem"
    GemVal _t644 = (*gem_v_expect);
    GemVal _t645[] = {gem_string("=")};
        (void)(_t644.fn(_t644.env, _t645, 1));
#line 457 "compiler/main.gem"
    GemVal _t646 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t646.fn(_t646.env, NULL, 0);
#line 458 "compiler/main.gem"
    GemVal _t647[] = {gem_v_name, gem_v_value, gem_v_line};
        GemVal _t648 = gem_fn_make_let(NULL, _t647, 3);
        gem_pop_frame();
        return _t648;
    }
#line 462 "compiler/main.gem"
    GemVal _t649 = gem_v_t;
    GemVal _t653;
    if (!gem_truthy(gem_eq(gem_table_get(_t649, gem_string("type")), gem_string("fn")))) {
        _t653 = gem_eq(gem_table_get(_t649, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t650 = (*gem_v_peek_at);
        GemVal _t651[] = {gem_int(1)};
        GemVal _t652 = _t650.fn(_t650.env, _t651, 1);
        _t653 = gem_eq(gem_table_get(_t652, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t653)) {
#line 463 "compiler/main.gem"
    GemVal _t654 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t654, gem_string("line"));
#line 464 "compiler/main.gem"
    GemVal _t655 = (*gem_v_advance);
        (void)(_t655.fn(_t655.env, NULL, 0));
#line 465 "compiler/main.gem"
    GemVal _t656 = (*gem_v_expect);
    GemVal _t657[] = {gem_string("NAME")};
    GemVal _t658 = _t656.fn(_t656.env, _t657, 1);
        GemVal gem_v_name = gem_table_get(_t658, gem_string("value"));
#line 466 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 467 "compiler/main.gem"
    GemVal _t659[] = {gem_v_name};
    GemVal _t660[] = {gem_v_line};
    GemVal _t661[] = {gem_v_name};
    GemVal _t662[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("named fn '"), gem_to_string_fn(NULL, _t659, 1)), gem_string("' inside function body is not supported at line ")), gem_to_string_fn(NULL, _t660, 1)), gem_string(" — use: let ")), gem_to_string_fn(NULL, _t661, 1)), gem_string(" = fn(...) ... end"))};
            (void)(gem_error_at_fn("compiler/main.gem", 467, _t662, 1));
        }
#line 469 "compiler/main.gem"
    GemVal _t663 = (*gem_v_expect);
    GemVal _t664[] = {gem_string("(")};
        (void)(_t663.fn(_t663.env, _t664, 1));
#line 470 "compiler/main.gem"
    GemVal _t665 = (*gem_v_parse_params);
        GemVal gem_v_params = _t665.fn(_t665.env, NULL, 0);
#line 471 "compiler/main.gem"
    GemVal _t666 = (*gem_v_expect);
    GemVal _t667[] = {gem_string(")")};
        (void)(_t666.fn(_t666.env, _t667, 1));
#line 472 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 473 "compiler/main.gem"
    GemVal _t668 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t668.fn(_t668.env, NULL, 0);
#line 474 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 475 "compiler/main.gem"
    GemVal _t669 = (*gem_v_expect);
    GemVal _t670[] = {gem_string("end")};
        (void)(_t669.fn(_t669.env, _t670, 1));
#line 476 "compiler/main.gem"
    GemVal _t671[] = {gem_v_name, gem_v_params, gem_v_body, gem_v_line};
        GemVal _t672 = gem_fn_make_fn_def(NULL, _t671, 4);
        gem_pop_frame();
        return _t672;
    }
#line 480 "compiler/main.gem"
    GemVal _t673 = gem_v_t;
    GemVal _t675;
    if (gem_truthy(gem_eq(gem_table_get(_t673, gem_string("type")), gem_string("if")))) {
        _t675 = gem_eq(gem_table_get(_t673, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t674 = gem_v_t;
        _t675 = gem_eq(gem_table_get(_t674, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t675)) {
#line 481 "compiler/main.gem"
    GemVal _t676 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t676, gem_string("line"));
#line 482 "compiler/main.gem"
    GemVal _t677 = (*gem_v_advance);
        (void)(_t677.fn(_t677.env, NULL, 0));
#line 483 "compiler/main.gem"
    GemVal _t678 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t678.fn(_t678.env, NULL, 0);
#line 484 "compiler/main.gem"
    GemVal _t679 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t679.fn(_t679.env, NULL, 0);
#line 485 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 486 "compiler/main.gem"
    GemVal _t680 = (*gem_v_peek);
    GemVal _t681 = _t680.fn(_t680.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t681, gem_string("type")), gem_string("elif")))) {
#line 488 "compiler/main.gem"
    GemVal _t682 = gem_table_new();
    GemVal _t683 = (*gem_v_parse_stmt);
    gem_table_set(_t682, gem_int(0), _t683.fn(_t683.env, NULL, 0));
            gem_v_else_body = _t682;
        } else {
#line 489 "compiler/main.gem"
    GemVal _t684 = (*gem_v_peek);
    GemVal _t685 = _t684.fn(_t684.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t685, gem_string("type")), gem_string("else")))) {
#line 490 "compiler/main.gem"
    GemVal _t686 = (*gem_v_advance);
                (void)(_t686.fn(_t686.env, NULL, 0));
#line 491 "compiler/main.gem"
    GemVal _t687 = (*gem_v_parse_body);
                gem_v_else_body = _t687.fn(_t687.env, NULL, 0);
            }
        }
#line 494 "compiler/main.gem"
    GemVal _t688 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t688, gem_string("type")), gem_string("if")))) {
#line 495 "compiler/main.gem"
    GemVal _t689 = (*gem_v_expect);
    GemVal _t690[] = {gem_string("end")};
            (void)(_t689.fn(_t689.env, _t690, 1));
        }
#line 497 "compiler/main.gem"
    GemVal _t691[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        GemVal _t692 = gem_fn_make_if(NULL, _t691, 4);
        gem_pop_frame();
        return _t692;
    }
#line 501 "compiler/main.gem"
    GemVal _t693 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t693, gem_string("type")), gem_string("while")))) {
#line 502 "compiler/main.gem"
    GemVal _t694 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t694, gem_string("line"));
#line 503 "compiler/main.gem"
    GemVal _t695 = (*gem_v_advance);
        (void)(_t695.fn(_t695.env, NULL, 0));
#line 504 "compiler/main.gem"
    GemVal _t696 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t696.fn(_t696.env, NULL, 0);
#line 505 "compiler/main.gem"
    GemVal _t697 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t697.fn(_t697.env, NULL, 0);
#line 506 "compiler/main.gem"
    GemVal _t698 = (*gem_v_expect);
    GemVal _t699[] = {gem_string("end")};
        (void)(_t698.fn(_t698.env, _t699, 1));
#line 507 "compiler/main.gem"
    GemVal _t700[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        GemVal _t701 = gem_fn_make_while(NULL, _t700, 3);
        gem_pop_frame();
        return _t701;
    }
#line 511 "compiler/main.gem"
    GemVal _t702 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t702, gem_string("type")), gem_string("for")))) {
#line 512 "compiler/main.gem"
    GemVal _t703 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t703, gem_string("line"));
#line 513 "compiler/main.gem"
    GemVal _t704 = (*gem_v_advance);
        (void)(_t704.fn(_t704.env, NULL, 0));
#line 514 "compiler/main.gem"
    GemVal _t705 = (*gem_v_expect);
    GemVal _t706[] = {gem_string("NAME")};
    GemVal _t707 = _t705.fn(_t705.env, _t706, 1);
        GemVal gem_v_var_name = gem_table_get(_t707, gem_string("value"));
#line 516 "compiler/main.gem"
    GemVal _t708 = (*gem_v_peek);
    GemVal _t709 = _t708.fn(_t708.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t709, gem_string("type")), gem_string("in")))) {
#line 518 "compiler/main.gem"
    GemVal _t710 = (*gem_v_advance);
            (void)(_t710.fn(_t710.env, NULL, 0));
#line 519 "compiler/main.gem"
    GemVal _t711 = (*gem_v_parse_expr);
            GemVal gem_v_items_expr = _t711.fn(_t711.env, NULL, 0);
#line 520 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 521 "compiler/main.gem"
    GemVal _t712[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t712, 1));
#line 522 "compiler/main.gem"
    GemVal _t713[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t713, 1));
#line 523 "compiler/main.gem"
    GemVal _t714 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t714.fn(_t714.env, NULL, 0);
#line 524 "compiler/main.gem"
    GemVal _t715 = (*gem_v_expect);
    GemVal _t716[] = {gem_string("end")};
            (void)(_t715.fn(_t715.env, _t716, 1));
#line 525 "compiler/main.gem"
    GemVal _t717 = gem_table_new();
            GemVal gem_v_inner_stmts = _t717;
#line 526 "compiler/main.gem"
    GemVal _t718[] = {gem_v_items_var};
    GemVal _t719[] = {gem_v_idx_var};
    GemVal _t720[] = {gem_fn_make_var(NULL, _t718, 1), gem_fn_make_var(NULL, _t719, 1)};
    GemVal _t721[] = {gem_v_var_name, gem_fn_make_index(NULL, _t720, 2), gem_v_line};
    GemVal _t722[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t721, 3)};
            (void)(gem_push_fn(NULL, _t722, 2));
#line 527 "compiler/main.gem"
    GemVal _t723[] = {gem_v_idx_var};
    GemVal _t724[] = {gem_int(1)};
    GemVal _t725[] = {gem_string("+"), gem_fn_make_var(NULL, _t723, 1), gem_fn_make_int(NULL, _t724, 1)};
    GemVal _t726[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t725, 3), gem_v_line};
    GemVal _t727[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t726, 3)};
            (void)(gem_push_fn(NULL, _t727, 2));
            {
#line 528 "compiler/main.gem"
                GemVal gem_v__for_i_1 = gem_int(0);
#line 528 "compiler/main.gem"
    GemVal _t728[] = {gem_v_fbody};
                GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t728, 1);
#line 528 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 528 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_1;
#line 528 "compiler/main.gem"
                    gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 529 "compiler/main.gem"
    GemVal _t729[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t729, 2));
                }
            }
#line 531 "compiler/main.gem"
    GemVal _t730[] = {gem_v_idx_var};
    GemVal _t731[] = {gem_string("len")};
    GemVal _t732 = gem_table_new();
    GemVal _t733[] = {gem_v_items_var};
    gem_table_set(_t732, gem_int(0), gem_fn_make_var(NULL, _t733, 1));
    GemVal _t734[] = {gem_fn_make_var(NULL, _t731, 1), _t732, gem_int(0)};
    GemVal _t735[] = {gem_string("<"), gem_fn_make_var(NULL, _t730, 1), gem_fn_make_call(NULL, _t734, 3)};
    GemVal _t736[] = {gem_fn_make_binop(NULL, _t735, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t736, 3);
#line 536 "compiler/main.gem"
    GemVal _t737 = gem_table_new();
    gem_table_set(_t737, gem_string("tag"), gem_string("block"));
    GemVal _t738 = gem_table_new();
    GemVal _t739[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t738, gem_int(0), gem_fn_make_let(NULL, _t739, 3));
    GemVal _t740[] = {gem_int(0)};
    GemVal _t741[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t740, 1), gem_v_line};
    gem_table_set(_t738, gem_int(1), gem_fn_make_let(NULL, _t741, 3));
    gem_table_set(_t738, gem_int(2), gem_v_while_node);
    gem_table_set(_t737, gem_string("stmts"), _t738);
            GemVal _t742 = _t737;
            gem_pop_frame();
            return _t742;
        } else {
#line 541 "compiler/main.gem"
    GemVal _t743 = (*gem_v_peek);
    GemVal _t744 = _t743.fn(_t743.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t744, gem_string("type")), gem_string("=")))) {
#line 543 "compiler/main.gem"
    GemVal _t745 = (*gem_v_advance);
                (void)(_t745.fn(_t745.env, NULL, 0));
#line 544 "compiler/main.gem"
    GemVal _t746 = (*gem_v_parse_expr);
                GemVal gem_v_start_expr = _t746.fn(_t746.env, NULL, 0);
#line 545 "compiler/main.gem"
    GemVal _t747 = (*gem_v_expect);
    GemVal _t748[] = {gem_string(",")};
                (void)(_t747.fn(_t747.env, _t748, 1));
#line 546 "compiler/main.gem"
    GemVal _t749 = (*gem_v_parse_expr);
                GemVal gem_v_end_expr = _t749.fn(_t749.env, NULL, 0);
#line 547 "compiler/main.gem"
    GemVal _t750 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t750.fn(_t750.env, NULL, 0);
#line 548 "compiler/main.gem"
    GemVal _t751 = (*gem_v_expect);
    GemVal _t752[] = {gem_string("end")};
                (void)(_t751.fn(_t751.env, _t752, 1));
#line 549 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 550 "compiler/main.gem"
    GemVal _t753[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t753, 1));
#line 551 "compiler/main.gem"
    GemVal _t754[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t754, 1));
#line 552 "compiler/main.gem"
    GemVal _t755 = gem_table_new();
                GemVal gem_v_inner_stmts = _t755;
#line 553 "compiler/main.gem"
    GemVal _t756[] = {gem_v_idx_var};
    GemVal _t757[] = {gem_v_var_name, gem_fn_make_var(NULL, _t756, 1), gem_v_line};
    GemVal _t758[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t757, 3)};
                (void)(gem_push_fn(NULL, _t758, 2));
#line 554 "compiler/main.gem"
    GemVal _t759[] = {gem_v_idx_var};
    GemVal _t760[] = {gem_int(1)};
    GemVal _t761[] = {gem_string("+"), gem_fn_make_var(NULL, _t759, 1), gem_fn_make_int(NULL, _t760, 1)};
    GemVal _t762[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t761, 3), gem_v_line};
    GemVal _t763[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t762, 3)};
                (void)(gem_push_fn(NULL, _t763, 2));
                {
#line 555 "compiler/main.gem"
                    GemVal gem_v__for_i_2 = gem_int(0);
#line 555 "compiler/main.gem"
    GemVal _t764[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t764, 1);
#line 555 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 555 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_2;
#line 555 "compiler/main.gem"
                        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 556 "compiler/main.gem"
    GemVal _t765[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t765, 2));
                    }
                }
#line 558 "compiler/main.gem"
    GemVal _t766[] = {gem_v_idx_var};
    GemVal _t767[] = {gem_v_limit_var};
    GemVal _t768[] = {gem_string("<"), gem_fn_make_var(NULL, _t766, 1), gem_fn_make_var(NULL, _t767, 1)};
    GemVal _t769[] = {gem_fn_make_binop(NULL, _t768, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t769, 3);
#line 563 "compiler/main.gem"
    GemVal _t770 = gem_table_new();
    gem_table_set(_t770, gem_string("tag"), gem_string("block"));
    GemVal _t771 = gem_table_new();
    GemVal _t772[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t771, gem_int(0), gem_fn_make_let(NULL, _t772, 3));
    GemVal _t773[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t771, gem_int(1), gem_fn_make_let(NULL, _t773, 3));
    gem_table_set(_t771, gem_int(2), gem_v_while_node);
    gem_table_set(_t770, gem_string("stmts"), _t771);
                GemVal _t774 = _t770;
                gem_pop_frame();
                return _t774;
            } else {
#line 569 "compiler/main.gem"
    GemVal _t775 = (*gem_v_peek);
    GemVal _t776 = _t775.fn(_t775.env, NULL, 0);
    GemVal _t777[] = {gem_table_get(_t776, gem_string("line"))};
    GemVal _t778[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t777, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 569, _t778, 1));
            }
        }
    }
#line 574 "compiler/main.gem"
    GemVal _t779 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t779, gem_string("type")), gem_string("match")))) {
#line 575 "compiler/main.gem"
    GemVal _t780 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t780, gem_string("line"));
#line 576 "compiler/main.gem"
    GemVal _t781 = (*gem_v_advance);
        (void)(_t781.fn(_t781.env, NULL, 0));
#line 577 "compiler/main.gem"
    GemVal _t782 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t782.fn(_t782.env, NULL, 0);
#line 578 "compiler/main.gem"
    GemVal _t783 = (*gem_v_skip_nl);
        (void)(_t783.fn(_t783.env, NULL, 0));
#line 579 "compiler/main.gem"
    GemVal _t784 = gem_table_new();
        GemVal gem_v_whens = _t784;
#line 580 "compiler/main.gem"
        while (1) {
            GemVal _t785 = (*gem_v_peek);
            GemVal _t786 = _t785.fn(_t785.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t786, gem_string("type")), gem_string("when")))) break;
#line 581 "compiler/main.gem"
    GemVal _t787 = (*gem_v_advance);
            (void)(_t787.fn(_t787.env, NULL, 0));
#line 582 "compiler/main.gem"
    GemVal _t788 = (*gem_v_parse_expr);
            GemVal gem_v_wval = _t788.fn(_t788.env, NULL, 0);
#line 583 "compiler/main.gem"
    GemVal _t789 = (*gem_v_parse_body);
            GemVal gem_v_wbody = _t789.fn(_t789.env, NULL, 0);
#line 584 "compiler/main.gem"
    GemVal _t790[] = {gem_v_wval, gem_v_wbody};
    GemVal _t791[] = {gem_v_whens, gem_fn_make_when(NULL, _t790, 2)};
            (void)(gem_push_fn(NULL, _t791, 2));
#line 585 "compiler/main.gem"
    GemVal _t792 = (*gem_v_skip_nl);
            (void)(_t792.fn(_t792.env, NULL, 0));
        }
#line 587 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 588 "compiler/main.gem"
    GemVal _t793 = (*gem_v_peek);
    GemVal _t794 = _t793.fn(_t793.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t794, gem_string("type")), gem_string("else")))) {
#line 589 "compiler/main.gem"
    GemVal _t795 = (*gem_v_advance);
            (void)(_t795.fn(_t795.env, NULL, 0));
#line 590 "compiler/main.gem"
    GemVal _t796 = (*gem_v_parse_body);
            gem_v_else_body = _t796.fn(_t796.env, NULL, 0);
        }
#line 592 "compiler/main.gem"
    GemVal _t797 = (*gem_v_expect);
    GemVal _t798[] = {gem_string("end")};
        (void)(_t797.fn(_t797.env, _t798, 1));
#line 593 "compiler/main.gem"
    GemVal _t799[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line};
        GemVal _t800 = gem_fn_make_match(NULL, _t799, 4);
        gem_pop_frame();
        return _t800;
    }
#line 597 "compiler/main.gem"
    GemVal _t801 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t801, gem_string("type")), gem_string("return")))) {
#line 598 "compiler/main.gem"
    GemVal _t802 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t802, gem_string("line"));
#line 599 "compiler/main.gem"
    GemVal _t803 = (*gem_v_advance);
        (void)(_t803.fn(_t803.env, NULL, 0));
#line 600 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 601 "compiler/main.gem"
    GemVal _t804 = (*gem_v_peek);
    GemVal _t805 = _t804.fn(_t804.env, NULL, 0);
    GemVal _t808;
    if (!gem_truthy(gem_neq(gem_table_get(_t805, gem_string("type")), gem_string("NEWLINE")))) {
        _t808 = gem_neq(gem_table_get(_t805, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t806 = (*gem_v_peek);
        GemVal _t807 = _t806.fn(_t806.env, NULL, 0);
        _t808 = gem_neq(gem_table_get(_t807, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t811;
    if (!gem_truthy(_t808)) {
        _t811 = _t808;
    } else {
        GemVal _t809 = (*gem_v_peek);
        GemVal _t810 = _t809.fn(_t809.env, NULL, 0);
        _t811 = gem_neq(gem_table_get(_t810, gem_string("type")), gem_string("end"));
    }
    GemVal _t814;
    if (!gem_truthy(_t811)) {
        _t814 = _t811;
    } else {
        GemVal _t812 = (*gem_v_peek);
        GemVal _t813 = _t812.fn(_t812.env, NULL, 0);
        _t814 = gem_neq(gem_table_get(_t813, gem_string("type")), gem_string("elif"));
    }
    GemVal _t817;
    if (!gem_truthy(_t814)) {
        _t817 = _t814;
    } else {
        GemVal _t815 = (*gem_v_peek);
        GemVal _t816 = _t815.fn(_t815.env, NULL, 0);
        _t817 = gem_neq(gem_table_get(_t816, gem_string("type")), gem_string("else"));
    }
    GemVal _t820;
    if (!gem_truthy(_t817)) {
        _t820 = _t817;
    } else {
        GemVal _t818 = (*gem_v_peek);
        GemVal _t819 = _t818.fn(_t818.env, NULL, 0);
        _t820 = gem_neq(gem_table_get(_t819, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t820)) {
#line 602 "compiler/main.gem"
    GemVal _t821 = (*gem_v_parse_expr);
            gem_v_value = _t821.fn(_t821.env, NULL, 0);
        }
#line 604 "compiler/main.gem"
    GemVal _t822[] = {gem_v_value, gem_v_line};
        GemVal _t823 = gem_fn_make_return(NULL, _t822, 2);
        gem_pop_frame();
        return _t823;
    }
#line 608 "compiler/main.gem"
    GemVal _t824 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t824, gem_string("type")), gem_string("break")))) {
#line 609 "compiler/main.gem"
    GemVal _t825 = (*gem_v_advance);
        (void)(_t825.fn(_t825.env, NULL, 0));
#line 610 "compiler/main.gem"
        GemVal _t826 = gem_fn_make_break(NULL, NULL, 0);
        gem_pop_frame();
        return _t826;
    }
#line 614 "compiler/main.gem"
    GemVal _t827 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t827, gem_string("type")), gem_string("continue")))) {
#line 615 "compiler/main.gem"
    GemVal _t828 = (*gem_v_advance);
        (void)(_t828.fn(_t828.env, NULL, 0));
#line 616 "compiler/main.gem"
        GemVal _t829 = gem_fn_make_continue(NULL, NULL, 0);
        gem_pop_frame();
        return _t829;
    }
#line 620 "compiler/main.gem"
    GemVal _t830 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t830, gem_string("type")), gem_string("load")))) {
#line 621 "compiler/main.gem"
    GemVal _t831 = (*gem_v_advance);
        (void)(_t831.fn(_t831.env, NULL, 0));
#line 622 "compiler/main.gem"
    GemVal _t832 = (*gem_v_expect);
    GemVal _t833[] = {gem_string("STRING")};
    GemVal _t834 = _t832.fn(_t832.env, _t833, 1);
        GemVal gem_v_path = gem_table_get(_t834, gem_string("value"));
#line 623 "compiler/main.gem"
    GemVal _t835[] = {gem_v_path};
        GemVal _t836 = gem_fn_make_load(NULL, _t835, 1);
        gem_pop_frame();
        return _t836;
    }
#line 627 "compiler/main.gem"
    GemVal _t837 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t837, gem_string("type")), gem_string("extern")))) {
#line 628 "compiler/main.gem"
    GemVal _t838 = (*gem_v_advance);
        (void)(_t838.fn(_t838.env, NULL, 0));
#line 629 "compiler/main.gem"
    GemVal _t839 = (*gem_v_peek);
    GemVal _t840 = _t839.fn(_t839.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t840, gem_string("type")), gem_string("fn")))) {
#line 630 "compiler/main.gem"
    GemVal _t841 = (*gem_v_advance);
            (void)(_t841.fn(_t841.env, NULL, 0));
#line 631 "compiler/main.gem"
    GemVal _t842 = (*gem_v_expect);
    GemVal _t843[] = {gem_string("NAME")};
    GemVal _t844 = _t842.fn(_t842.env, _t843, 1);
            GemVal gem_v_name = gem_table_get(_t844, gem_string("value"));
#line 632 "compiler/main.gem"
    GemVal _t845 = (*gem_v_expect);
    GemVal _t846[] = {gem_string("(")};
            (void)(_t845.fn(_t845.env, _t846, 1));
#line 633 "compiler/main.gem"
    GemVal _t847 = gem_table_new();
            GemVal gem_v_eparams = _t847;
#line 634 "compiler/main.gem"
    GemVal _t848 = (*gem_v_peek);
    GemVal _t849 = _t848.fn(_t848.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t849, gem_string("type")), gem_string(")")))) {
#line 635 "compiler/main.gem"
    GemVal _t850 = (*gem_v_expect);
    GemVal _t851[] = {gem_string("NAME")};
    GemVal _t852 = _t850.fn(_t850.env, _t851, 1);
                GemVal gem_v_pname = gem_table_get(_t852, gem_string("value"));
#line 636 "compiler/main.gem"
    GemVal _t853 = (*gem_v_expect);
    GemVal _t854[] = {gem_string(":")};
                (void)(_t853.fn(_t853.env, _t854, 1));
#line 637 "compiler/main.gem"
    GemVal _t855 = (*gem_v_expect);
    GemVal _t856[] = {gem_string("NAME")};
    GemVal _t857 = _t855.fn(_t855.env, _t856, 1);
                GemVal gem_v_ptype = gem_table_get(_t857, gem_string("value"));
#line 638 "compiler/main.gem"
    GemVal _t858[] = {gem_v_pname, gem_v_ptype};
    GemVal _t859[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t858, 2)};
                (void)(gem_push_fn(NULL, _t859, 2));
#line 639 "compiler/main.gem"
                while (1) {
                    GemVal _t860 = (*gem_v_peek);
                    GemVal _t861 = _t860.fn(_t860.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t861, gem_string("type")), gem_string(",")))) break;
#line 640 "compiler/main.gem"
    GemVal _t862 = (*gem_v_advance);
                    (void)(_t862.fn(_t862.env, NULL, 0));
#line 641 "compiler/main.gem"
    GemVal _t863 = (*gem_v_expect);
    GemVal _t864[] = {gem_string("NAME")};
    GemVal _t865 = _t863.fn(_t863.env, _t864, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t865, gem_string("value"));
#line 642 "compiler/main.gem"
    GemVal _t866 = (*gem_v_expect);
    GemVal _t867[] = {gem_string(":")};
                    (void)(_t866.fn(_t866.env, _t867, 1));
#line 643 "compiler/main.gem"
    GemVal _t868 = (*gem_v_expect);
    GemVal _t869[] = {gem_string("NAME")};
    GemVal _t870 = _t868.fn(_t868.env, _t869, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t870, gem_string("value"));
#line 644 "compiler/main.gem"
    GemVal _t871[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t872[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t871, 2)};
                    (void)(gem_push_fn(NULL, _t872, 2));
                }
            }
#line 647 "compiler/main.gem"
    GemVal _t873 = (*gem_v_expect);
    GemVal _t874[] = {gem_string(")")};
            (void)(_t873.fn(_t873.env, _t874, 1));
#line 648 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 649 "compiler/main.gem"
    GemVal _t875 = (*gem_v_peek);
    GemVal _t876 = _t875.fn(_t875.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t876, gem_string("type")), gem_string("->")))) {
#line 650 "compiler/main.gem"
    GemVal _t877 = (*gem_v_advance);
                (void)(_t877.fn(_t877.env, NULL, 0));
#line 651 "compiler/main.gem"
    GemVal _t878 = (*gem_v_expect);
    GemVal _t879[] = {gem_string("NAME")};
    GemVal _t880 = _t878.fn(_t878.env, _t879, 1);
                gem_v_ret_type = gem_table_get(_t880, gem_string("value"));
            }
#line 653 "compiler/main.gem"
    GemVal _t881[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            GemVal _t882 = gem_fn_make_extern_fn(NULL, _t881, 3);
            gem_pop_frame();
            return _t882;
        } else {
#line 654 "compiler/main.gem"
    GemVal _t883 = (*gem_v_peek);
    GemVal _t884 = _t883.fn(_t883.env, NULL, 0);
    GemVal _t887;
    if (!gem_truthy(gem_eq(gem_table_get(_t884, gem_string("type")), gem_string("NAME")))) {
        _t887 = gem_eq(gem_table_get(_t884, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t885 = (*gem_v_peek);
        GemVal _t886 = _t885.fn(_t885.env, NULL, 0);
        _t887 = gem_eq(gem_table_get(_t886, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t887)) {
#line 655 "compiler/main.gem"
    GemVal _t888 = (*gem_v_advance);
                (void)(_t888.fn(_t888.env, NULL, 0));
#line 656 "compiler/main.gem"
    GemVal _t889 = (*gem_v_expect);
    GemVal _t890[] = {gem_string("STRING")};
    GemVal _t891 = _t889.fn(_t889.env, _t890, 1);
                GemVal gem_v_path = gem_table_get(_t891, gem_string("value"));
#line 657 "compiler/main.gem"
    GemVal _t892[] = {gem_v_path};
                GemVal _t893 = gem_fn_make_extern_include(NULL, _t892, 1);
                gem_pop_frame();
                return _t893;
            } else {
#line 659 "compiler/main.gem"
    GemVal _t894 = (*gem_v_peek);
    GemVal _t895 = _t894.fn(_t894.env, NULL, 0);
    GemVal _t896[] = {gem_table_get(_t895, gem_string("line"))};
    GemVal _t897[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t896, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 659, _t897, 1));
            }
        }
    }
#line 664 "compiler/main.gem"
    GemVal _t898 = (*gem_v_parse_expr);
    GemVal _t899 = _t898.fn(_t898.env, NULL, 0);
    gem_pop_frame();
    return _t899;
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
#line 673 "compiler/main.gem"
    GemVal _t901 = gem_table_new();
    GemVal gem_v_stmts = _t901;
#line 674 "compiler/main.gem"
    GemVal _t902 = (*gem_v_skip_nl);
    (void)(_t902.fn(_t902.env, NULL, 0));
#line 675 "compiler/main.gem"
    while (1) {
        GemVal _t903 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t903.fn(_t903.env, NULL, 0)))) break;
#line 676 "compiler/main.gem"
    GemVal _t904 = (*gem_v_parse_stmt);
    GemVal _t905[] = {gem_v_stmts, _t904.fn(_t904.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t905, 2));
#line 677 "compiler/main.gem"
    GemVal _t906 = (*gem_v_skip_nl);
        (void)(_t906.fn(_t906.env, NULL, 0));
    }
#line 679 "compiler/main.gem"
    GemVal _t907[] = {gem_v_stmts};
    GemVal _t908 = gem_fn_make_program(NULL, _t907, 1);
    gem_pop_frame();
    return _t908;
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
    struct _closure__anon_1 *_t221 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t221->gem_v_pos = gem_v_pos;
    _t221->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t221);
#line 29 "compiler/main.gem"
    struct _closure__anon_2 *_t223 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t223->gem_v_pos = gem_v_pos;
    _t223->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t223);
#line 33 "compiler/main.gem"
    struct _closure__anon_3 *_t225 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t225->gem_v_pos = gem_v_pos;
    _t225->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t225);
#line 39 "compiler/main.gem"
    struct _closure__anon_4 *_t228 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t228->gem_v_pos = gem_v_pos;
    _t228->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t228);
#line 43 "compiler/main.gem"
    struct _closure__anon_5 *_t237 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t237->gem_v_pos = gem_v_pos;
    _t237->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t237);
#line 52 "compiler/main.gem"
    struct _closure__anon_6 *_t239 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t239->gem_v_pos = gem_v_pos;
    _t239->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t239);
#line 60 "compiler/main.gem"
    struct _closure__anon_7 *_t254 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t254->gem_v_advance = gem_v_advance;
    _t254->gem_v_expect = gem_v_expect;
    _t254->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t254);
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
    struct _closure__anon_8 *_t274 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t274->gem_v_at_end = gem_v_at_end;
    _t274->gem_v_parse_stmt = gem_v_parse_stmt;
    _t274->gem_v_peek = gem_v_peek;
    _t274->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t274);
#line 96 "compiler/main.gem"
    struct _closure__anon_9 *_t285 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t285->gem_v_at_end = gem_v_at_end;
    _t285->gem_v_parse_stmt = gem_v_parse_stmt;
    _t285->gem_v_peek = gem_v_peek;
    _t285->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t285);
#line 109 "compiler/main.gem"
    struct _closure__anon_10 *_t412 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t412->gem_v_advance = gem_v_advance;
    _t412->gem_v_expect = gem_v_expect;
    _t412->gem_v_fn_depth = gem_v_fn_depth;
    _t412->gem_v_parse_expr = gem_v_parse_expr;
    _t412->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t412->gem_v_parse_params = gem_v_parse_params;
    _t412->gem_v_peek = gem_v_peek;
    _t412->gem_v_peek_at = gem_v_peek_at;
    _t412->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t412);
#line 240 "compiler/main.gem"
    struct _closure__anon_11 *_t497 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t497->gem_v_advance = gem_v_advance;
    _t497->gem_v_expect = gem_v_expect;
    _t497->gem_v_fn_depth = gem_v_fn_depth;
    _t497->gem_v_parse_atom = gem_v_parse_atom;
    _t497->gem_v_parse_expr = gem_v_parse_expr;
    _t497->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t497->gem_v_parse_params = gem_v_parse_params;
    _t497->gem_v_peek = gem_v_peek;
    _t497->gem_v_peek_at = gem_v_peek_at;
    _t497->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t497);
#line 336 "compiler/main.gem"
    struct _closure__anon_12 *_t506 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t506->gem_v_advance = gem_v_advance;
    _t506->gem_v_parse_call = gem_v_parse_call;
    _t506->gem_v_parse_unary = gem_v_parse_unary;
    _t506->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t506);
#line 345 "compiler/main.gem"
    struct _closure__anon_13 *_t521 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t521->gem_v_advance = gem_v_advance;
    _t521->gem_v_parse_unary = gem_v_parse_unary;
    _t521->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t521);
#line 356 "compiler/main.gem"
    struct _closure__anon_14 *_t533 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t533->gem_v_advance = gem_v_advance;
    _t533->gem_v_parse_mul = gem_v_parse_mul;
    _t533->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t533);
#line 367 "compiler/main.gem"
    struct _closure__anon_15 *_t560 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t560->gem_v_advance = gem_v_advance;
    _t560->gem_v_parse_add = gem_v_parse_add;
    _t560->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t560);
#line 378 "compiler/main.gem"
    struct _closure__anon_16 *_t569 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t569->gem_v_advance = gem_v_advance;
    _t569->gem_v_parse_compare = gem_v_parse_compare;
    _t569->gem_v_parse_not = gem_v_parse_not;
    _t569->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t569);
#line 387 "compiler/main.gem"
    struct _closure__anon_17 *_t577 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t577->gem_v_advance = gem_v_advance;
    _t577->gem_v_parse_not = gem_v_parse_not;
    _t577->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t577);
#line 398 "compiler/main.gem"
    struct _closure__anon_18 *_t585 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t585->gem_v_advance = gem_v_advance;
    _t585->gem_v_parse_and = gem_v_parse_and;
    _t585->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t585);
#line 409 "compiler/main.gem"
    struct _closure__anon_19 *_t636 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t636->gem_v_advance = gem_v_advance;
    _t636->gem_v_parse_expr = gem_v_parse_expr;
    _t636->gem_v_parse_or = gem_v_parse_or;
    _t636->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t636);
#line 444 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 448 "compiler/main.gem"
    struct _closure__anon_20 *_t900 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t900->gem_v_advance = gem_v_advance;
    _t900->gem_v_expect = gem_v_expect;
    _t900->gem_v_fn_depth = gem_v_fn_depth;
    _t900->gem_v_gensym_counter = gem_v_gensym_counter;
    _t900->gem_v_parse_body = gem_v_parse_body;
    _t900->gem_v_parse_expr = gem_v_parse_expr;
    _t900->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t900->gem_v_parse_params = gem_v_parse_params;
    _t900->gem_v_parse_stmt = gem_v_parse_stmt;
    _t900->gem_v_peek = gem_v_peek;
    _t900->gem_v_peek_at = gem_v_peek_at;
    _t900->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_20, _t900);
#line 668 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 672 "compiler/main.gem"
    struct _closure__anon_21 *_t909 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t909->gem_v_at_end = gem_v_at_end;
    _t909->gem_v_parse_stmt = gem_v_parse_stmt;
    _t909->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_21, _t909);
    GemVal _t910 = gem_table_new();
    gem_table_set(_t910, gem_string("parse"), gem_v_parse);
    GemVal _t911 = _t910;
    gem_pop_frame();
    return _t911;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("escape_c_string", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal _t912[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t912, 1);
#line 11 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
    {
#line 12 "compiler/main.gem"
        GemVal gem_v__for_i_1 = gem_int(0);
#line 12 "compiler/main.gem"
    GemVal _t913[] = {gem_v_s};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t913, 1);
#line 12 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 12 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_1;
#line 12 "compiler/main.gem"
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 13 "compiler/main.gem"
            GemVal gem_v_ch = gem_table_get(gem_v_s, gem_v_i);
#line 14 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\\")))) {
#line 15 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_string("\\\\"));
            } else {
#line 16 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 17 "compiler/main.gem"
                    gem_v_result = gem_add(gem_v_result, gem_string("\\\""));
                } else {
#line 18 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 19 "compiler/main.gem"
                        gem_v_result = gem_add(gem_v_result, gem_string("\\n"));
                    } else {
#line 20 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ch, gem_v_cr))) {
#line 21 "compiler/main.gem"
                            gem_v_result = gem_add(gem_v_result, gem_string("\\r"));
                        } else {
#line 22 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\t")))) {
#line 23 "compiler/main.gem"
                                gem_v_result = gem_add(gem_v_result, gem_string("\\t"));
                            } else {
#line 25 "compiler/main.gem"
                                gem_v_result = gem_add(gem_v_result, gem_v_ch);
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t914 = gem_v_result;
    gem_pop_frame();
    return _t914;
}

static GemVal gem_fn_repeat_str(void *_env, GemVal *args, int argc) {
#line 31 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("repeat_str", "compiler/main.gem", 31);
#line 32 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
    {
#line 33 "compiler/main.gem"
        GemVal gem_v__for_i_2 = gem_int(0);
#line 33 "compiler/main.gem"
        GemVal gem_v__for_limit_2 = gem_v_n;
#line 33 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 33 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_2;
#line 33 "compiler/main.gem"
            gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 34 "compiler/main.gem"
            gem_v_result = gem_add(gem_v_result, gem_v_s);
        }
    }
    GemVal _t915 = gem_v_result;
    gem_pop_frame();
    return _t915;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 39 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("pad_for", "compiler/main.gem", 39);
#line 40 "compiler/main.gem"
    GemVal _t916[] = {gem_string("    "), gem_v_indent};
    GemVal _t917 = gem_fn_repeat_str(NULL, _t916, 2);
    gem_pop_frame();
    return _t917;
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 43 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("format_float", "compiler/main.gem", 43);
#line 44 "compiler/main.gem"
    GemVal _t918[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t918, 1);
#line 46 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
    {
#line 47 "compiler/main.gem"
        GemVal gem_v__for_i_3 = gem_int(0);
#line 47 "compiler/main.gem"
    GemVal _t919[] = {gem_v_s};
        GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t919, 1);
#line 47 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 47 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_3;
#line 47 "compiler/main.gem"
            gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 48 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_string(".")))) {
#line 49 "compiler/main.gem"
                gem_v_has_dot = gem_bool(1);
            }
        }
    }
#line 52 "compiler/main.gem"
    if (gem_truthy(gem_not(gem_v_has_dot))) {
#line 53 "compiler/main.gem"
        gem_v_s = gem_add(gem_v_s, gem_string(".0"));
    }
    GemVal _t920 = gem_v_s;
    gem_pop_frame();
    return _t920;
}

static GemVal gem_fn_array_contains(void *_env, GemVal *args, int argc) {
#line 60 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("array_contains", "compiler/main.gem", 60);
    {
#line 61 "compiler/main.gem"
        GemVal gem_v__for_i_4 = gem_int(0);
#line 61 "compiler/main.gem"
    GemVal _t921[] = {gem_v_arr};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t921, 1);
#line 61 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 61 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_4;
#line 61 "compiler/main.gem"
            gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 62 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_arr, gem_v_i), gem_v_val))) {
#line 63 "compiler/main.gem"
                GemVal _t922 = gem_bool(1);
                gem_pop_frame();
                return _t922;
            }
        }
    }
    GemVal _t923 = gem_bool(0);
    gem_pop_frame();
    return _t923;
}

static GemVal gem_fn_set_add(void *_env, GemVal *args, int argc) {
#line 69 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_add", "compiler/main.gem", 69);
#line 70 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1)))) {
#line 71 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_val, gem_bool(1));
        gem_pop_frame();
        return GEM_NIL;
    } else {
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn_set_contains(void *_env, GemVal *args, int argc) {
#line 75 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_contains", "compiler/main.gem", 75);
    GemVal _t924 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t924;
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 79 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_union", "compiler/main.gem", 79);
#line 80 "compiler/main.gem"
    GemVal _t925 = gem_table_new();
    GemVal gem_v_result = _t925;
#line 81 "compiler/main.gem"
    GemVal _t926[] = {gem_v_a};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t926, 1);
#line 82 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 83 "compiler/main.gem"
    while (1) {
        GemVal _t927[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t927, 1)))) break;
#line 84 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
#line 85 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 87 "compiler/main.gem"
    GemVal _t928[] = {gem_v_b};
    GemVal gem_v_ks2 = gem_keys_fn(NULL, _t928, 1);
#line 88 "compiler/main.gem"
    GemVal gem_v_j = gem_int(0);
#line 89 "compiler/main.gem"
    while (1) {
        GemVal _t929[] = {gem_v_ks2};
        if (!gem_truthy(gem_lt(gem_v_j, gem_len_fn(NULL, _t929, 1)))) break;
#line 90 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks2, gem_v_j), gem_bool(1));
#line 91 "compiler/main.gem"
        gem_v_j = gem_add(gem_v_j, gem_int(1));
    }
    GemVal _t930 = gem_v_result;
    gem_pop_frame();
    return _t930;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 96 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_intersect", "compiler/main.gem", 96);
#line 97 "compiler/main.gem"
    GemVal _t931 = gem_table_new();
    GemVal gem_v_result = _t931;
#line 98 "compiler/main.gem"
    GemVal _t932[] = {gem_v_a};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t932, 1);
#line 99 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 100 "compiler/main.gem"
    while (1) {
        GemVal _t933[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t933, 1)))) break;
#line 101 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_table_get(gem_v_ks, gem_v_i)), gem_bool(1)))) {
#line 102 "compiler/main.gem"
            gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
        }
#line 104 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t934 = gem_v_result;
    gem_pop_frame();
    return _t934;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
#line 109 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("set_to_sorted_array", "compiler/main.gem", 109);
#line 110 "compiler/main.gem"
    GemVal _t935[] = {gem_v_s};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t935, 1);
#line 112 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 113 "compiler/main.gem"
    while (1) {
        GemVal _t936[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t936, 1)))) break;
#line 114 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
#line 115 "compiler/main.gem"
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
#line 116 "compiler/main.gem"
        while (1) {
            GemVal _t937;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t937 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t937 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t937)) break;
#line 117 "compiler/main.gem"
            gem_table_set(gem_v_ks, gem_add(gem_v_j, gem_int(1)), gem_table_get(gem_v_ks, gem_v_j));
#line 118 "compiler/main.gem"
            gem_v_j = gem_sub(gem_v_j, gem_int(1));
        }
#line 120 "compiler/main.gem"
        gem_table_set(gem_v_ks, gem_add(gem_v_j, gem_int(1)), gem_v_key);
#line 121 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t938 = gem_v_ks;
    gem_pop_frame();
    return _t938;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 126 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("sorted_array_to_set", "compiler/main.gem", 126);
#line 127 "compiler/main.gem"
    GemVal _t939 = gem_table_new();
    GemVal gem_v_s = _t939;
    {
#line 128 "compiler/main.gem"
        GemVal gem_v__for_items_5 = gem_v_arr;
#line 128 "compiler/main.gem"
        GemVal gem_v__for_i_5 = gem_int(0);
#line 128 "compiler/main.gem"
        while (1) {
            GemVal _t940[] = {gem_v__for_items_5};
            if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t940, 1)))) break;
#line 128 "compiler/main.gem"
            GemVal gem_v_item = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 128 "compiler/main.gem"
            gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 129 "compiler/main.gem"
            gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
        }
    }
    GemVal _t941 = gem_v_s;
    gem_pop_frame();
    return _t941;
}

struct _closure__anon_22 {
    GemVal *gem_v_tmp_counter;
};
static GemVal _anon_22(void *_env, GemVal *args, int argc) {
    struct _closure__anon_22 *_cls = (struct _closure__anon_22 *)_env;
    GemVal *gem_v_tmp_counter = _cls->gem_v_tmp_counter;
    gem_push_frame("_anon_22", "compiler/main.gem", 0);
#line 167 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t947[] = {(*gem_v_tmp_counter)};
    GemVal _t948 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t947, 1));
    gem_pop_frame();
    return _t948;
}

struct _closure__anon_23 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_23", "compiler/main.gem", 0);
#line 172 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t950[] = {(*gem_v_anon_counter)};
    GemVal _t951 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t950, 1));
    gem_pop_frame();
    return _t951;
}

struct _closure__anon_24 {
};
static GemVal _anon_24(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_24", "compiler/main.gem", 0);
    GemVal _t953[] = {gem_v_name};
    GemVal _t954 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t953, 1));
    gem_pop_frame();
    return _t954;
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
#line 186 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 187 "compiler/main.gem"
        GemVal _t955 = GEM_NIL;
        gem_pop_frame();
        return _t955;
    }
#line 189 "compiler/main.gem"
    GemVal _t956[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t956, 1), gem_string("table")))) {
#line 190 "compiler/main.gem"
        GemVal _t957 = GEM_NIL;
        gem_pop_frame();
        return _t957;
    }
#line 192 "compiler/main.gem"
    GemVal _t958 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t958, gem_string("tag"));
#line 193 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 194 "compiler/main.gem"
        GemVal _t959 = GEM_NIL;
        gem_pop_frame();
        return _t959;
    }
#line 197 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 198 "compiler/main.gem"
    GemVal _t960 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t960, gem_string("name"));
#line 199 "compiler/main.gem"
    GemVal _t961[] = {gem_v_defined, gem_v_name};
    GemVal _t963;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t961, 2)))) {
        _t963 = gem_not(gem_fn_set_contains(NULL, _t961, 2));
    } else {
        GemVal _t962[] = {(*gem_v_builtins), gem_v_name};
        _t963 = gem_not(gem_fn_set_contains(NULL, _t962, 2));
    }
    GemVal _t965;
    if (!gem_truthy(_t963)) {
        _t965 = _t963;
    } else {
        GemVal _t964[] = {(*gem_v_defined_fns), gem_v_name};
        _t965 = gem_not(gem_fn_set_contains(NULL, _t964, 2));
    }
        if (gem_truthy(_t965)) {
#line 200 "compiler/main.gem"
    GemVal _t966[] = {gem_v_free, gem_v_name};
            GemVal _t967 = gem_fn_set_add(NULL, _t966, 2);
            gem_pop_frame();
            return _t967;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
#line 202 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 203 "compiler/main.gem"
    GemVal _t968 = gem_v_node;
    GemVal _t969 = (*gem_v_collect_free_node);
    GemVal _t970[] = {gem_table_get(_t968, gem_string("value")), gem_v_defined, gem_v_free};
            GemVal _t971 = _t969.fn(_t969.env, _t970, 3);
            gem_pop_frame();
            return _t971;
        } else {
#line 204 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 205 "compiler/main.gem"
    GemVal _t972 = gem_v_node;
                GemVal gem_v_name = gem_table_get(_t972, gem_string("name"));
#line 206 "compiler/main.gem"
    GemVal _t973[] = {gem_v_defined, gem_v_name};
    GemVal _t975;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t973, 2)))) {
        _t975 = gem_not(gem_fn_set_contains(NULL, _t973, 2));
    } else {
        GemVal _t974[] = {(*gem_v_builtins), gem_v_name};
        _t975 = gem_not(gem_fn_set_contains(NULL, _t974, 2));
    }
    GemVal _t977;
    if (!gem_truthy(_t975)) {
        _t977 = _t975;
    } else {
        GemVal _t976[] = {(*gem_v_defined_fns), gem_v_name};
        _t977 = gem_not(gem_fn_set_contains(NULL, _t976, 2));
    }
                if (gem_truthy(_t977)) {
#line 207 "compiler/main.gem"
    GemVal _t978[] = {gem_v_free, gem_v_name};
                    (void)(gem_fn_set_add(NULL, _t978, 2));
                }
#line 209 "compiler/main.gem"
    GemVal _t979 = gem_v_node;
    GemVal _t980 = (*gem_v_collect_free_node);
    GemVal _t981[] = {gem_table_get(_t979, gem_string("value")), gem_v_defined, gem_v_free};
                GemVal _t982 = _t980.fn(_t980.env, _t981, 3);
                gem_pop_frame();
                return _t982;
            } else {
#line 210 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 211 "compiler/main.gem"
    GemVal _t983 = gem_v_node;
    GemVal _t984[] = {gem_table_get(_t983, gem_string("params"))};
    GemVal _t985[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t984, 1)};
                    GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t985, 2);
#line 212 "compiler/main.gem"
    GemVal _t986 = gem_v_node;
    GemVal _t987 = (*gem_v_collect_free_vars);
    GemVal _t988[] = {gem_table_get(_t986, gem_string("body")), gem_v_inner_defined};
                    GemVal gem_v_inner_free = _t987.fn(_t987.env, _t988, 2);
#line 213 "compiler/main.gem"
    GemVal _t989[] = {gem_v_inner_free};
                    GemVal gem_v_ifks = gem_keys_fn(NULL, _t989, 1);
#line 214 "compiler/main.gem"
                    GemVal gem_v_ifi = gem_int(0);
#line 215 "compiler/main.gem"
                    while (1) {
                        GemVal _t990[] = {gem_v_ifks};
                        if (!gem_truthy(gem_lt(gem_v_ifi, gem_len_fn(NULL, _t990, 1)))) break;
#line 216 "compiler/main.gem"
    GemVal _t991[] = {gem_v_free, gem_table_get(gem_v_ifks, gem_v_ifi)};
                        (void)(gem_fn_set_add(NULL, _t991, 2));
#line 217 "compiler/main.gem"
                        gem_v_ifi = gem_add(gem_v_ifi, gem_int(1));
                    }
                    gem_pop_frame();
                    return GEM_NIL;
                } else {
#line 219 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                        gem_pop_frame();
                        return GEM_NIL;
                    } else {
#line 221 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 222 "compiler/main.gem"
    GemVal _t992 = gem_v_node;
    GemVal _t993 = (*gem_v_collect_free_node);
    GemVal _t994[] = {gem_table_get(_t992, gem_string("cond")), gem_v_defined, gem_v_free};
                            (void)(_t993.fn(_t993.env, _t994, 3));
#line 223 "compiler/main.gem"
    GemVal _t995 = gem_table_new();
    GemVal _t996[] = {gem_v_defined, _t995};
                            GemVal gem_v_d = gem_fn_set_union(NULL, _t996, 2);
#line 224 "compiler/main.gem"
    GemVal _t997 = gem_v_node;
                            GemVal gem_v_thens = gem_table_get(_t997, gem_string("then"));
                            {
#line 225 "compiler/main.gem"
                                GemVal gem_v__for_i_6 = gem_int(0);
#line 225 "compiler/main.gem"
    GemVal _t998[] = {gem_v_thens};
                                GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t998, 1);
#line 225 "compiler/main.gem"
                                while (1) {
                                    if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
#line 225 "compiler/main.gem"
                                    GemVal gem_v_i = gem_v__for_i_6;
#line 225 "compiler/main.gem"
                                    gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 226 "compiler/main.gem"
    GemVal _t999 = (*gem_v_collect_free_node);
    GemVal _t1000[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
                                    (void)(_t999.fn(_t999.env, _t1000, 3));
#line 227 "compiler/main.gem"
    GemVal _t1001[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t1003;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1001, 1), gem_string("table")))) {
        _t1003 = gem_eq(gem_type_fn(NULL, _t1001, 1), gem_string("table"));
    } else {
        GemVal _t1002 = gem_table_get(gem_v_thens, gem_v_i);
        _t1003 = gem_eq(gem_table_get(_t1002, gem_string("tag")), gem_string("let"));
    }
                                    if (gem_truthy(_t1003)) {
#line 228 "compiler/main.gem"
    GemVal _t1004 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1005[] = {gem_v_d, gem_table_get(_t1004, gem_string("name"))};
                                        (void)(gem_fn_set_add(NULL, _t1005, 2));
                                    }
                                }
                            }
#line 231 "compiler/main.gem"
    GemVal _t1006 = gem_v_node;
                            GemVal gem_v_el = gem_table_get(_t1006, gem_string("else"));
#line 232 "compiler/main.gem"
                            if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 233 "compiler/main.gem"
    GemVal _t1007 = gem_table_new();
    GemVal _t1008[] = {gem_v_defined, _t1007};
                                gem_v_d = gem_fn_set_union(NULL, _t1008, 2);
                                {
#line 234 "compiler/main.gem"
                                    GemVal gem_v__for_i_7 = gem_int(0);
#line 234 "compiler/main.gem"
    GemVal _t1009[] = {gem_v_el};
                                    GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1009, 1);
#line 234 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 234 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_7;
#line 234 "compiler/main.gem"
                                        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 235 "compiler/main.gem"
    GemVal _t1010 = (*gem_v_collect_free_node);
    GemVal _t1011[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t1010.fn(_t1010.env, _t1011, 3));
#line 236 "compiler/main.gem"
    GemVal _t1012[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t1014;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1012, 1), gem_string("table")))) {
        _t1014 = gem_eq(gem_type_fn(NULL, _t1012, 1), gem_string("table"));
    } else {
        GemVal _t1013 = gem_table_get(gem_v_el, gem_v_i);
        _t1014 = gem_eq(gem_table_get(_t1013, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t1014)) {
#line 237 "compiler/main.gem"
    GemVal _t1015 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1016[] = {gem_v_d, gem_table_get(_t1015, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t1016, 2));
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
#line 241 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 242 "compiler/main.gem"
    GemVal _t1017 = gem_v_node;
    GemVal _t1018 = (*gem_v_collect_free_node);
    GemVal _t1019[] = {gem_table_get(_t1017, gem_string("cond")), gem_v_defined, gem_v_free};
                                (void)(_t1018.fn(_t1018.env, _t1019, 3));
#line 243 "compiler/main.gem"
    GemVal _t1020 = gem_table_new();
    GemVal _t1021[] = {gem_v_defined, _t1020};
                                GemVal gem_v_d = gem_fn_set_union(NULL, _t1021, 2);
                                {
#line 244 "compiler/main.gem"
                                    GemVal gem_v__for_i_8 = gem_int(0);
#line 244 "compiler/main.gem"
    GemVal _t1022 = gem_v_node;
    GemVal _t1023[] = {gem_table_get(_t1022, gem_string("body"))};
                                    GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1023, 1);
#line 244 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 244 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_8;
#line 244 "compiler/main.gem"
                                        gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 245 "compiler/main.gem"
    GemVal _t1024 = gem_v_node;
    GemVal _t1025 = (*gem_v_collect_free_node);
    GemVal _t1026[] = {gem_table_get(gem_table_get(_t1024, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t1025.fn(_t1025.env, _t1026, 3));
#line 246 "compiler/main.gem"
    GemVal _t1027 = gem_v_node;
    GemVal _t1028[] = {gem_table_get(gem_table_get(_t1027, gem_string("body")), gem_v_i)};
    GemVal _t1031;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1028, 1), gem_string("table")))) {
        _t1031 = gem_eq(gem_type_fn(NULL, _t1028, 1), gem_string("table"));
    } else {
        GemVal _t1029 = gem_v_node;
        GemVal _t1030 = gem_table_get(gem_table_get(_t1029, gem_string("body")), gem_v_i);
        _t1031 = gem_eq(gem_table_get(_t1030, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t1031)) {
#line 247 "compiler/main.gem"
    GemVal _t1032 = gem_v_node;
    GemVal _t1033 = gem_table_get(gem_table_get(_t1032, gem_string("body")), gem_v_i);
    GemVal _t1034[] = {gem_v_d, gem_table_get(_t1033, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t1034, 2));
                                        }
                                    }
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 250 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 251 "compiler/main.gem"
    GemVal _t1035 = gem_v_node;
    GemVal _t1036 = (*gem_v_collect_free_node);
    GemVal _t1037[] = {gem_table_get(_t1035, gem_string("target")), gem_v_defined, gem_v_free};
                                    (void)(_t1036.fn(_t1036.env, _t1037, 3));
                                    {
#line 252 "compiler/main.gem"
                                        GemVal gem_v__for_i_10 = gem_int(0);
#line 252 "compiler/main.gem"
    GemVal _t1038 = gem_v_node;
    GemVal _t1039[] = {gem_table_get(_t1038, gem_string("whens"))};
                                        GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t1039, 1);
#line 252 "compiler/main.gem"
                                        while (1) {
                                            if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
#line 252 "compiler/main.gem"
                                            GemVal gem_v_i = gem_v__for_i_10;
#line 252 "compiler/main.gem"
                                            gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 253 "compiler/main.gem"
    GemVal _t1040 = gem_v_node;
                                            GemVal gem_v_w = gem_table_get(gem_table_get(_t1040, gem_string("whens")), gem_v_i);
#line 254 "compiler/main.gem"
    GemVal _t1041 = gem_v_w;
    GemVal _t1042 = (*gem_v_collect_free_node);
    GemVal _t1043[] = {gem_table_get(_t1041, gem_string("value")), gem_v_defined, gem_v_free};
                                            (void)(_t1042.fn(_t1042.env, _t1043, 3));
#line 255 "compiler/main.gem"
    GemVal _t1044 = gem_table_new();
    GemVal _t1045[] = {gem_v_defined, _t1044};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1045, 2);
                                            {
#line 256 "compiler/main.gem"
                                                GemVal gem_v__for_i_9 = gem_int(0);
#line 256 "compiler/main.gem"
    GemVal _t1046 = gem_v_w;
    GemVal _t1047[] = {gem_table_get(_t1046, gem_string("body"))};
                                                GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1047, 1);
#line 256 "compiler/main.gem"
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 256 "compiler/main.gem"
                                                    GemVal gem_v_j = gem_v__for_i_9;
#line 256 "compiler/main.gem"
                                                    gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 257 "compiler/main.gem"
    GemVal _t1048 = gem_v_w;
    GemVal _t1049 = (*gem_v_collect_free_node);
    GemVal _t1050[] = {gem_table_get(gem_table_get(_t1048, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
                                                    (void)(_t1049.fn(_t1049.env, _t1050, 3));
#line 258 "compiler/main.gem"
    GemVal _t1051 = gem_v_w;
    GemVal _t1052[] = {gem_table_get(gem_table_get(_t1051, gem_string("body")), gem_v_j)};
    GemVal _t1055;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1052, 1), gem_string("table")))) {
        _t1055 = gem_eq(gem_type_fn(NULL, _t1052, 1), gem_string("table"));
    } else {
        GemVal _t1053 = gem_v_w;
        GemVal _t1054 = gem_table_get(gem_table_get(_t1053, gem_string("body")), gem_v_j);
        _t1055 = gem_eq(gem_table_get(_t1054, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t1055)) {
#line 259 "compiler/main.gem"
    GemVal _t1056 = gem_v_w;
    GemVal _t1057 = gem_table_get(gem_table_get(_t1056, gem_string("body")), gem_v_j);
    GemVal _t1058[] = {gem_v_d, gem_table_get(_t1057, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t1058, 2));
                                                    }
                                                }
                                            }
                                        }
                                    }
#line 263 "compiler/main.gem"
    GemVal _t1059 = gem_v_node;
                                    GemVal gem_v_el = gem_table_get(_t1059, gem_string("else"));
#line 264 "compiler/main.gem"
                                    if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 265 "compiler/main.gem"
    GemVal _t1060 = gem_table_new();
    GemVal _t1061[] = {gem_v_defined, _t1060};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1061, 2);
                                        {
#line 266 "compiler/main.gem"
                                            GemVal gem_v__for_i_11 = gem_int(0);
#line 266 "compiler/main.gem"
    GemVal _t1062[] = {gem_v_el};
                                            GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t1062, 1);
#line 266 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
#line 266 "compiler/main.gem"
                                                GemVal gem_v_j = gem_v__for_i_11;
#line 266 "compiler/main.gem"
                                                gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 267 "compiler/main.gem"
    GemVal _t1063 = (*gem_v_collect_free_node);
    GemVal _t1064[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
                                                (void)(_t1063.fn(_t1063.env, _t1064, 3));
#line 268 "compiler/main.gem"
    GemVal _t1065[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t1067;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1065, 1), gem_string("table")))) {
        _t1067 = gem_eq(gem_type_fn(NULL, _t1065, 1), gem_string("table"));
    } else {
        GemVal _t1066 = gem_table_get(gem_v_el, gem_v_j);
        _t1067 = gem_eq(gem_table_get(_t1066, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1067)) {
#line 269 "compiler/main.gem"
    GemVal _t1068 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t1069[] = {gem_v_d, gem_table_get(_t1068, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1069, 2));
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
#line 273 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("when_clause")))) {
#line 274 "compiler/main.gem"
    GemVal _t1070 = gem_v_node;
    GemVal _t1071 = (*gem_v_collect_free_node);
    GemVal _t1072[] = {gem_table_get(_t1070, gem_string("value")), gem_v_defined, gem_v_free};
                                        (void)(_t1071.fn(_t1071.env, _t1072, 3));
#line 275 "compiler/main.gem"
    GemVal _t1073 = gem_table_new();
    GemVal _t1074[] = {gem_v_defined, _t1073};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1074, 2);
                                        {
#line 276 "compiler/main.gem"
                                            GemVal gem_v__for_i_12 = gem_int(0);
#line 276 "compiler/main.gem"
    GemVal _t1075 = gem_v_node;
    GemVal _t1076[] = {gem_table_get(_t1075, gem_string("body"))};
                                            GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t1076, 1);
#line 276 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 276 "compiler/main.gem"
                                                GemVal gem_v_i = gem_v__for_i_12;
#line 276 "compiler/main.gem"
                                                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 277 "compiler/main.gem"
    GemVal _t1077 = gem_v_node;
    GemVal _t1078 = (*gem_v_collect_free_node);
    GemVal _t1079[] = {gem_table_get(gem_table_get(_t1077, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                                (void)(_t1078.fn(_t1078.env, _t1079, 3));
#line 278 "compiler/main.gem"
    GemVal _t1080 = gem_v_node;
    GemVal _t1081[] = {gem_table_get(gem_table_get(_t1080, gem_string("body")), gem_v_i)};
    GemVal _t1084;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1081, 1), gem_string("table")))) {
        _t1084 = gem_eq(gem_type_fn(NULL, _t1081, 1), gem_string("table"));
    } else {
        GemVal _t1082 = gem_v_node;
        GemVal _t1083 = gem_table_get(gem_table_get(_t1082, gem_string("body")), gem_v_i);
        _t1084 = gem_eq(gem_table_get(_t1083, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1084)) {
#line 279 "compiler/main.gem"
    GemVal _t1085 = gem_v_node;
    GemVal _t1086 = gem_table_get(gem_table_get(_t1085, gem_string("body")), gem_v_i);
    GemVal _t1087[] = {gem_v_d, gem_table_get(_t1086, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1087, 2));
                                                }
                                            }
                                            gem_pop_frame();
                                            return GEM_NIL;
                                        }
                                    } else {
#line 282 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 283 "compiler/main.gem"
    GemVal _t1088 = gem_table_new();
    GemVal _t1089[] = {gem_v_defined, _t1088};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1089, 2);
                                            {
#line 284 "compiler/main.gem"
                                                GemVal gem_v__for_i_13 = gem_int(0);
#line 284 "compiler/main.gem"
    GemVal _t1090 = gem_v_node;
    GemVal _t1091[] = {gem_table_get(_t1090, gem_string("stmts"))};
                                                GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1091, 1);
#line 284 "compiler/main.gem"
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 284 "compiler/main.gem"
                                                    GemVal gem_v_i = gem_v__for_i_13;
#line 284 "compiler/main.gem"
                                                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 285 "compiler/main.gem"
    GemVal _t1092 = gem_v_node;
    GemVal _t1093 = (*gem_v_collect_free_node);
    GemVal _t1094[] = {gem_table_get(gem_table_get(_t1092, gem_string("stmts")), gem_v_i), gem_v_d, gem_v_free};
                                                    (void)(_t1093.fn(_t1093.env, _t1094, 3));
#line 286 "compiler/main.gem"
    GemVal _t1095 = gem_v_node;
    GemVal _t1096[] = {gem_table_get(gem_table_get(_t1095, gem_string("stmts")), gem_v_i)};
    GemVal _t1099;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1096, 1), gem_string("table")))) {
        _t1099 = gem_eq(gem_type_fn(NULL, _t1096, 1), gem_string("table"));
    } else {
        GemVal _t1097 = gem_v_node;
        GemVal _t1098 = gem_table_get(gem_table_get(_t1097, gem_string("stmts")), gem_v_i);
        _t1099 = gem_eq(gem_table_get(_t1098, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t1099)) {
#line 287 "compiler/main.gem"
    GemVal _t1100 = gem_v_node;
    GemVal _t1101 = gem_table_get(gem_table_get(_t1100, gem_string("stmts")), gem_v_i);
    GemVal _t1102[] = {gem_v_d, gem_table_get(_t1101, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t1102, 2));
                                                    }
                                                }
                                                gem_pop_frame();
                                                return GEM_NIL;
                                            }
                                        } else {
#line 290 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 291 "compiler/main.gem"
    GemVal _t1103 = gem_v_node;
                                                if (gem_truthy(gem_neq(gem_table_get(_t1103, gem_string("value")), GEM_NIL))) {
#line 292 "compiler/main.gem"
    GemVal _t1104 = gem_v_node;
    GemVal _t1105 = (*gem_v_collect_free_node);
    GemVal _t1106[] = {gem_table_get(_t1104, gem_string("value")), gem_v_defined, gem_v_free};
                                                    GemVal _t1107 = _t1105.fn(_t1105.env, _t1106, 3);
                                                    gem_pop_frame();
                                                    return _t1107;
                                                } else {
                                                    gem_pop_frame();
                                                    return GEM_NIL;
                                                }
                                            } else {
#line 296 "compiler/main.gem"
    GemVal _t1108[] = {gem_v_node};
                                                GemVal gem_v_nks = gem_keys_fn(NULL, _t1108, 1);
#line 297 "compiler/main.gem"
                                                GemVal gem_v_nki = gem_int(0);
#line 298 "compiler/main.gem"
                                                while (1) {
                                                    GemVal _t1109[] = {gem_v_nks};
                                                    if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t1109, 1)))) break;
#line 299 "compiler/main.gem"
                                                    if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
#line 300 "compiler/main.gem"
                                                        GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
#line 301 "compiler/main.gem"
    GemVal _t1110[] = {gem_v_v};
                                                        if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1110, 1), gem_string("table")))) {
#line 303 "compiler/main.gem"
    GemVal _t1111[] = {gem_v_v};
                                                            GemVal gem_v_vks = gem_keys_fn(NULL, _t1111, 1);
#line 304 "compiler/main.gem"
    GemVal _t1112[] = {gem_v_vks};
    GemVal _t1114;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1112, 1), gem_int(0)))) {
        _t1114 = gem_gt(gem_len_fn(NULL, _t1112, 1), gem_int(0));
    } else {
        GemVal _t1113[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1114 = gem_eq(gem_type_fn(NULL, _t1113, 1), gem_string("int"));
    }
                                                            if (gem_truthy(_t1114)) {
#line 306 "compiler/main.gem"
                                                                GemVal gem_v_vi = gem_int(0);
#line 307 "compiler/main.gem"
                                                                while (1) {
                                                                    GemVal _t1115[] = {gem_v_v};
                                                                    if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1115, 1)))) break;
#line 308 "compiler/main.gem"
    GemVal _t1116[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1116, 1), gem_string("table")))) {
#line 309 "compiler/main.gem"
    GemVal _t1117 = (*gem_v_collect_free_node);
    GemVal _t1118[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
                                                                        (void)(_t1117.fn(_t1117.env, _t1118, 3));
                                                                    }
#line 311 "compiler/main.gem"
                                                                    gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                                }
                                                            } else {
#line 314 "compiler/main.gem"
    GemVal _t1119 = (*gem_v_collect_free_node);
    GemVal _t1120[] = {gem_v_v, gem_v_defined, gem_v_free};
                                                                (void)(_t1119.fn(_t1119.env, _t1120, 3));
                                                            }
                                                        }
                                                    }
#line 318 "compiler/main.gem"
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
#line 324 "compiler/main.gem"
    GemVal _t1122 = gem_table_new();
    GemVal gem_v_free = _t1122;
#line 325 "compiler/main.gem"
    GemVal _t1123 = gem_table_new();
    GemVal _t1124[] = {gem_v_defined, _t1123};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t1124, 2);
    {
#line 326 "compiler/main.gem"
        GemVal gem_v__for_i_15 = gem_int(0);
#line 326 "compiler/main.gem"
    GemVal _t1125[] = {gem_v_stmts};
        GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1125, 1);
#line 326 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 326 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_15;
#line 326 "compiler/main.gem"
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 327 "compiler/main.gem"
    GemVal _t1126 = (*gem_v_collect_free_node);
    GemVal _t1127[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
            (void)(_t1126.fn(_t1126.env, _t1127, 3));
#line 328 "compiler/main.gem"
    GemVal _t1128[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1130;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1128, 1), gem_string("table")))) {
        _t1130 = gem_eq(gem_type_fn(NULL, _t1128, 1), gem_string("table"));
    } else {
        GemVal _t1129 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1130 = gem_eq(gem_table_get(_t1129, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1130)) {
#line 329 "compiler/main.gem"
    GemVal _t1131 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1132[] = {gem_v_d, gem_table_get(_t1131, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1132, 2));
            }
#line 331 "compiler/main.gem"
    GemVal _t1133[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1135;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1133, 1), gem_string("table")))) {
        _t1135 = gem_eq(gem_type_fn(NULL, _t1133, 1), gem_string("table"));
    } else {
        GemVal _t1134 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1135 = gem_eq(gem_table_get(_t1134, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1135)) {
                {
#line 332 "compiler/main.gem"
                    GemVal gem_v__for_i_14 = gem_int(0);
#line 332 "compiler/main.gem"
    GemVal _t1136 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1137[] = {gem_table_get(_t1136, gem_string("stmts"))};
                    GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1137, 1);
#line 332 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 332 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_14;
#line 332 "compiler/main.gem"
                        gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 333 "compiler/main.gem"
    GemVal _t1138 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1139[] = {gem_table_get(gem_table_get(_t1138, gem_string("stmts")), gem_v_j)};
    GemVal _t1142;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1139, 1), gem_string("table")))) {
        _t1142 = gem_eq(gem_type_fn(NULL, _t1139, 1), gem_string("table"));
    } else {
        GemVal _t1140 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1141 = gem_table_get(gem_table_get(_t1140, gem_string("stmts")), gem_v_j);
        _t1142 = gem_eq(gem_table_get(_t1141, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1142)) {
#line 334 "compiler/main.gem"
    GemVal _t1143 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1144 = gem_table_get(gem_table_get(_t1143, gem_string("stmts")), gem_v_j);
    GemVal _t1145[] = {gem_v_d, gem_table_get(_t1144, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1145, 2));
                        }
                    }
                }
            }
        }
    }
    GemVal _t1146 = gem_v_free;
    gem_pop_frame();
    return _t1146;
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
#line 348 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 349 "compiler/main.gem"
        GemVal _t1148 = GEM_NIL;
        gem_pop_frame();
        return _t1148;
    }
#line 351 "compiler/main.gem"
    GemVal _t1149[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1149, 1), gem_string("table")))) {
#line 352 "compiler/main.gem"
        GemVal _t1150 = GEM_NIL;
        gem_pop_frame();
        return _t1150;
    }
#line 354 "compiler/main.gem"
    GemVal _t1151 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1151, gem_string("tag"));
#line 355 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 356 "compiler/main.gem"
        GemVal _t1152 = GEM_NIL;
        gem_pop_frame();
        return _t1152;
    }
#line 359 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 360 "compiler/main.gem"
    GemVal _t1153 = gem_v_node;
    GemVal _t1154[] = {gem_table_get(_t1153, gem_string("params"))};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1154, 1);
#line 361 "compiler/main.gem"
    GemVal _t1155 = gem_v_node;
    GemVal _t1156 = (*gem_v_collect_free_vars);
    GemVal _t1157[] = {gem_table_get(_t1155, gem_string("body")), gem_v_inner_defined};
        GemVal gem_v_free = _t1156.fn(_t1156.env, _t1157, 2);
#line 362 "compiler/main.gem"
    GemVal _t1158[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t1158, 2);
#line 363 "compiler/main.gem"
    GemVal _t1159[] = {gem_v_inter};
        GemVal gem_v_iks = gem_keys_fn(NULL, _t1159, 1);
#line 364 "compiler/main.gem"
        GemVal gem_v_iki = gem_int(0);
#line 365 "compiler/main.gem"
        while (1) {
            GemVal _t1160[] = {gem_v_iks};
            if (!gem_truthy(gem_lt(gem_v_iki, gem_len_fn(NULL, _t1160, 1)))) break;
#line 366 "compiler/main.gem"
    GemVal _t1161[] = {gem_v_captured, gem_table_get(gem_v_iks, gem_v_iki)};
            (void)(gem_fn_set_add(NULL, _t1161, 2));
#line 367 "compiler/main.gem"
            gem_v_iki = gem_add(gem_v_iki, gem_int(1));
        }
#line 369 "compiler/main.gem"
    GemVal _t1162 = gem_v_node;
    GemVal _t1163 = gem_v_node;
    GemVal _t1164[] = {gem_table_get(_t1163, gem_string("params"))};
    GemVal _t1165[] = {gem_v_scope_vars, gem_fn_sorted_array_to_set(NULL, _t1164, 1)};
    GemVal _t1166 = (*gem_v_walk_captures);
    GemVal _t1167[] = {gem_table_get(_t1162, gem_string("body")), gem_fn_set_union(NULL, _t1165, 2), gem_v_captured};
        GemVal _t1168 = _t1166.fn(_t1166.env, _t1167, 3);
        gem_pop_frame();
        return _t1168;
    } else {
#line 370 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
            gem_pop_frame();
            return GEM_NIL;
        } else {
#line 372 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 373 "compiler/main.gem"
    GemVal _t1169 = gem_v_node;
    GemVal _t1170 = (*gem_v_walk_captures_node);
    GemVal _t1171[] = {gem_table_get(_t1169, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1170.fn(_t1170.env, _t1171, 3));
#line 374 "compiler/main.gem"
    GemVal _t1172 = gem_v_node;
    GemVal _t1173 = (*gem_v_walk_captures);
    GemVal _t1174[] = {gem_table_get(_t1172, gem_string("then")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1173.fn(_t1173.env, _t1174, 3));
#line 375 "compiler/main.gem"
    GemVal _t1175 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t1175, gem_string("else")), GEM_NIL))) {
#line 376 "compiler/main.gem"
    GemVal _t1176 = gem_v_node;
    GemVal _t1177 = (*gem_v_walk_captures);
    GemVal _t1178[] = {gem_table_get(_t1176, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                    GemVal _t1179 = _t1177.fn(_t1177.env, _t1178, 3);
                    gem_pop_frame();
                    return _t1179;
                } else {
                    gem_pop_frame();
                    return GEM_NIL;
                }
            } else {
#line 378 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 379 "compiler/main.gem"
    GemVal _t1180 = gem_v_node;
    GemVal _t1181 = (*gem_v_walk_captures_node);
    GemVal _t1182[] = {gem_table_get(_t1180, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                    (void)(_t1181.fn(_t1181.env, _t1182, 3));
#line 380 "compiler/main.gem"
    GemVal _t1183 = gem_v_node;
    GemVal _t1184 = (*gem_v_walk_captures);
    GemVal _t1185[] = {gem_table_get(_t1183, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                    GemVal _t1186 = _t1184.fn(_t1184.env, _t1185, 3);
                    gem_pop_frame();
                    return _t1186;
                } else {
#line 381 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 382 "compiler/main.gem"
    GemVal _t1187 = gem_v_node;
    GemVal _t1188 = (*gem_v_walk_captures_node);
    GemVal _t1189[] = {gem_table_get(_t1187, gem_string("target")), gem_v_scope_vars, gem_v_captured};
                        (void)(_t1188.fn(_t1188.env, _t1189, 3));
                        {
#line 383 "compiler/main.gem"
                            GemVal gem_v__for_i_16 = gem_int(0);
#line 383 "compiler/main.gem"
    GemVal _t1190 = gem_v_node;
    GemVal _t1191[] = {gem_table_get(_t1190, gem_string("whens"))};
                            GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1191, 1);
#line 383 "compiler/main.gem"
                            while (1) {
                                if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 383 "compiler/main.gem"
                                GemVal gem_v_i = gem_v__for_i_16;
#line 383 "compiler/main.gem"
                                gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 384 "compiler/main.gem"
    GemVal _t1192 = gem_v_node;
    GemVal _t1193 = gem_table_get(gem_table_get(_t1192, gem_string("whens")), gem_v_i);
    GemVal _t1194 = (*gem_v_walk_captures_node);
    GemVal _t1195[] = {gem_table_get(_t1193, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t1194.fn(_t1194.env, _t1195, 3));
#line 385 "compiler/main.gem"
    GemVal _t1196 = gem_v_node;
    GemVal _t1197 = gem_table_get(gem_table_get(_t1196, gem_string("whens")), gem_v_i);
    GemVal _t1198 = (*gem_v_walk_captures);
    GemVal _t1199[] = {gem_table_get(_t1197, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t1198.fn(_t1198.env, _t1199, 3));
                            }
                        }
#line 387 "compiler/main.gem"
    GemVal _t1200 = gem_v_node;
                        if (gem_truthy(gem_neq(gem_table_get(_t1200, gem_string("else")), GEM_NIL))) {
#line 388 "compiler/main.gem"
    GemVal _t1201 = gem_v_node;
    GemVal _t1202 = (*gem_v_walk_captures);
    GemVal _t1203[] = {gem_table_get(_t1201, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                            GemVal _t1204 = _t1202.fn(_t1202.env, _t1203, 3);
                            gem_pop_frame();
                            return _t1204;
                        } else {
                            gem_pop_frame();
                            return GEM_NIL;
                        }
                    } else {
#line 390 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 391 "compiler/main.gem"
    GemVal _t1205 = gem_v_node;
    GemVal _t1206 = (*gem_v_walk_captures);
    GemVal _t1207[] = {gem_table_get(_t1205, gem_string("stmts")), gem_v_scope_vars, gem_v_captured};
                            GemVal _t1208 = _t1206.fn(_t1206.env, _t1207, 3);
                            gem_pop_frame();
                            return _t1208;
                        } else {
#line 392 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 393 "compiler/main.gem"
    GemVal _t1209 = gem_v_node;
                                if (gem_truthy(gem_neq(gem_table_get(_t1209, gem_string("value")), GEM_NIL))) {
#line 394 "compiler/main.gem"
    GemVal _t1210 = gem_v_node;
    GemVal _t1211 = (*gem_v_walk_captures_node);
    GemVal _t1212[] = {gem_table_get(_t1210, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    GemVal _t1213 = _t1211.fn(_t1211.env, _t1212, 3);
                                    gem_pop_frame();
                                    return _t1213;
                                } else {
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 396 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 397 "compiler/main.gem"
    GemVal _t1214 = gem_v_node;
    GemVal _t1215 = (*gem_v_walk_captures_node);
    GemVal _t1216[] = {gem_table_get(_t1214, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    GemVal _t1217 = _t1215.fn(_t1215.env, _t1216, 3);
                                    gem_pop_frame();
                                    return _t1217;
                                } else {
#line 398 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 399 "compiler/main.gem"
    GemVal _t1218 = gem_v_node;
    GemVal _t1219 = (*gem_v_walk_captures_node);
    GemVal _t1220[] = {gem_table_get(_t1218, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                        GemVal _t1221 = _t1219.fn(_t1219.env, _t1220, 3);
                                        gem_pop_frame();
                                        return _t1221;
                                    } else {
#line 402 "compiler/main.gem"
    GemVal _t1222[] = {gem_v_node};
                                        GemVal gem_v_nks = gem_keys_fn(NULL, _t1222, 1);
#line 403 "compiler/main.gem"
                                        GemVal gem_v_nki = gem_int(0);
#line 404 "compiler/main.gem"
                                        while (1) {
                                            GemVal _t1223[] = {gem_v_nks};
                                            if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t1223, 1)))) break;
#line 405 "compiler/main.gem"
                                            if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
#line 406 "compiler/main.gem"
                                                GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
#line 407 "compiler/main.gem"
    GemVal _t1224[] = {gem_v_v};
                                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1224, 1), gem_string("table")))) {
#line 408 "compiler/main.gem"
    GemVal _t1225[] = {gem_v_v};
                                                    GemVal gem_v_vks = gem_keys_fn(NULL, _t1225, 1);
#line 409 "compiler/main.gem"
    GemVal _t1226[] = {gem_v_vks};
    GemVal _t1228;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1226, 1), gem_int(0)))) {
        _t1228 = gem_gt(gem_len_fn(NULL, _t1226, 1), gem_int(0));
    } else {
        GemVal _t1227[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1228 = gem_eq(gem_type_fn(NULL, _t1227, 1), gem_string("int"));
    }
                                                    if (gem_truthy(_t1228)) {
#line 410 "compiler/main.gem"
                                                        GemVal gem_v_vi = gem_int(0);
#line 411 "compiler/main.gem"
                                                        while (1) {
                                                            GemVal _t1229[] = {gem_v_v};
                                                            if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1229, 1)))) break;
#line 412 "compiler/main.gem"
    GemVal _t1230[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                            if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1230, 1), gem_string("table")))) {
#line 413 "compiler/main.gem"
    GemVal _t1231 = (*gem_v_walk_captures_node);
    GemVal _t1232[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
                                                                (void)(_t1231.fn(_t1231.env, _t1232, 3));
                                                            }
#line 415 "compiler/main.gem"
                                                            gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                        }
                                                    } else {
#line 418 "compiler/main.gem"
    GemVal _t1233 = (*gem_v_walk_captures_node);
    GemVal _t1234[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
                                                        (void)(_t1233.fn(_t1233.env, _t1234, 3));
                                                    }
                                                }
                                            }
#line 422 "compiler/main.gem"
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
#line 428 "compiler/main.gem"
    GemVal _t1236 = gem_table_new();
    GemVal _t1237[] = {gem_v_scope_vars, _t1236};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t1237, 2);
    {
#line 429 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 429 "compiler/main.gem"
    GemVal _t1238[] = {gem_v_stmts};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1238, 1);
#line 429 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 429 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_18;
#line 429 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 430 "compiler/main.gem"
    GemVal _t1239 = (*gem_v_walk_captures_node);
    GemVal _t1240[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
            (void)(_t1239.fn(_t1239.env, _t1240, 3));
#line 431 "compiler/main.gem"
    GemVal _t1241[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1243;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1241, 1), gem_string("table")))) {
        _t1243 = gem_eq(gem_type_fn(NULL, _t1241, 1), gem_string("table"));
    } else {
        GemVal _t1242 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1243 = gem_eq(gem_table_get(_t1242, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1243)) {
#line 432 "compiler/main.gem"
    GemVal _t1244 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1245[] = {gem_v_sv, gem_table_get(_t1244, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1245, 2));
            }
#line 434 "compiler/main.gem"
    GemVal _t1246[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1248;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1246, 1), gem_string("table")))) {
        _t1248 = gem_eq(gem_type_fn(NULL, _t1246, 1), gem_string("table"));
    } else {
        GemVal _t1247 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1248 = gem_eq(gem_table_get(_t1247, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1248)) {
                {
#line 435 "compiler/main.gem"
                    GemVal gem_v__for_i_17 = gem_int(0);
#line 435 "compiler/main.gem"
    GemVal _t1249 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1250[] = {gem_table_get(_t1249, gem_string("stmts"))};
                    GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1250, 1);
#line 435 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 435 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_17;
#line 435 "compiler/main.gem"
                        gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 436 "compiler/main.gem"
    GemVal _t1251 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1252[] = {gem_table_get(gem_table_get(_t1251, gem_string("stmts")), gem_v_j)};
    GemVal _t1255;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1252, 1), gem_string("table")))) {
        _t1255 = gem_eq(gem_type_fn(NULL, _t1252, 1), gem_string("table"));
    } else {
        GemVal _t1253 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1254 = gem_table_get(gem_table_get(_t1253, gem_string("stmts")), gem_v_j);
        _t1255 = gem_eq(gem_table_get(_t1254, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1255)) {
#line 437 "compiler/main.gem"
    GemVal _t1256 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1257 = gem_table_get(gem_table_get(_t1256, gem_string("stmts")), gem_v_j);
    GemVal _t1258[] = {gem_v_sv, gem_table_get(_t1257, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1258, 2));
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
#line 445 "compiler/main.gem"
    GemVal _t1260 = gem_table_new();
    GemVal gem_v_captured = _t1260;
#line 446 "compiler/main.gem"
    GemVal _t1261 = (*gem_v_walk_captures);
    GemVal _t1262[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    (void)(_t1261.fn(_t1261.env, _t1262, 3));
    GemVal _t1263 = gem_v_captured;
    gem_pop_frame();
    return _t1263;
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
#line 473 "compiler/main.gem"
    GemVal _t1265 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1265, gem_string("tag"));
#line 475 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("int")))) {
#line 476 "compiler/main.gem"
    GemVal _t1266 = gem_table_new();
    GemVal _t1267 = gem_v_node;
    GemVal _t1268[] = {gem_table_get(_t1267, gem_string("value"))};
    gem_table_set(_t1266, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t1268, 1)), gem_string(")")));
    gem_table_set(_t1266, gem_string("setup"), gem_string(""));
        GemVal _t1269 = _t1266;
        gem_pop_frame();
        return _t1269;
    } else {
#line 477 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("float")))) {
#line 478 "compiler/main.gem"
    GemVal _t1270 = gem_table_new();
    GemVal _t1271 = gem_v_node;
    GemVal _t1272[] = {gem_table_get(_t1271, gem_string("value"))};
    GemVal _t1273[] = {gem_fn_format_float(NULL, _t1272, 1)};
    gem_table_set(_t1270, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t1273, 1)), gem_string(")")));
    gem_table_set(_t1270, gem_string("setup"), gem_string(""));
            GemVal _t1274 = _t1270;
            gem_pop_frame();
            return _t1274;
        } else {
#line 479 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("string")))) {
#line 480 "compiler/main.gem"
    GemVal _t1275 = gem_v_node;
    GemVal _t1276[] = {gem_table_get(_t1275, gem_string("value"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1276, 1);
#line 481 "compiler/main.gem"
    GemVal _t1277 = gem_table_new();
    GemVal _t1278[] = {gem_v_escaped};
    gem_table_set(_t1277, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t1278, 1)), gem_string("\")")));
    gem_table_set(_t1277, gem_string("setup"), gem_string(""));
                GemVal _t1279 = _t1277;
                gem_pop_frame();
                return _t1279;
            } else {
#line 482 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("interp")))) {
#line 483 "compiler/main.gem"
    GemVal _t1280 = gem_v_node;
                    GemVal gem_v_parts = gem_table_get(_t1280, gem_string("parts"));
#line 484 "compiler/main.gem"
    GemVal _t1281[] = {gem_v_parts};
                    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1281, 1), gem_int(0)))) {
#line 485 "compiler/main.gem"
    GemVal _t1282 = gem_table_new();
    gem_table_set(_t1282, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t1282, gem_string("setup"), gem_string(""));
                        GemVal _t1283 = _t1282;
                        gem_pop_frame();
                        return _t1283;
                    }
#line 487 "compiler/main.gem"
                    GemVal gem_v_setup = gem_string("");
#line 488 "compiler/main.gem"
    GemVal _t1284 = gem_table_new();
                    GemVal gem_v_compiled = _t1284;
                    {
#line 489 "compiler/main.gem"
                        GemVal gem_v__for_i_19 = gem_int(0);
#line 489 "compiler/main.gem"
    GemVal _t1285[] = {gem_v_parts};
                        GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1285, 1);
#line 489 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 489 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_19;
#line 489 "compiler/main.gem"
                            gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 490 "compiler/main.gem"
                            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 491 "compiler/main.gem"
    GemVal _t1286 = (*gem_v_compile_expr);
    GemVal _t1287[] = {gem_v_part};
                            GemVal gem_v_r = _t1286.fn(_t1286.env, _t1287, 1);
#line 492 "compiler/main.gem"
    GemVal _t1288 = gem_v_r;
                            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1288, gem_string("setup")));
#line 493 "compiler/main.gem"
    GemVal _t1289 = gem_v_part;
                            if (gem_truthy(gem_eq(gem_table_get(_t1289, gem_string("tag")), gem_string("string")))) {
#line 494 "compiler/main.gem"
    GemVal _t1290 = gem_v_r;
    GemVal _t1291[] = {gem_v_compiled, gem_table_get(_t1290, gem_string("expr"))};
                                (void)(gem_push_fn(NULL, _t1291, 2));
                            } else {
#line 496 "compiler/main.gem"
    GemVal _t1292 = (*gem_v_tmp);
                                GemVal gem_v_ts = _t1292.fn(_t1292.env, NULL, 0);
#line 497 "compiler/main.gem"
    GemVal _t1293[] = {gem_v_ts};
    GemVal _t1294 = gem_v_r;
    GemVal _t1295[] = {gem_table_get(_t1294, gem_string("expr"))};
                                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1293, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1295, 1)), gem_string("};\n")));
#line 498 "compiler/main.gem"
    GemVal _t1296[] = {gem_v_ts};
    GemVal _t1297[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t1296, 1)), gem_string(", 1)"))};
                                (void)(gem_push_fn(NULL, _t1297, 2));
                            }
                        }
                    }
#line 501 "compiler/main.gem"
                    GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
                    {
#line 502 "compiler/main.gem"
                        GemVal gem_v__for_i_20 = gem_int(1);
#line 502 "compiler/main.gem"
    GemVal _t1298[] = {gem_v_compiled};
                        GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1298, 1);
#line 502 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 502 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_20;
#line 502 "compiler/main.gem"
                            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 503 "compiler/main.gem"
    GemVal _t1299[] = {gem_v_acc};
    GemVal _t1300[] = {gem_table_get(gem_v_compiled, gem_v_i)};
                            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1299, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1300, 1)), gem_string(")"));
                        }
                    }
#line 505 "compiler/main.gem"
    GemVal _t1301 = gem_table_new();
    gem_table_set(_t1301, gem_string("expr"), gem_v_acc);
    gem_table_set(_t1301, gem_string("setup"), gem_v_setup);
                    GemVal _t1302 = _t1301;
                    gem_pop_frame();
                    return _t1302;
                } else {
#line 506 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("bool")))) {
#line 507 "compiler/main.gem"
    GemVal _t1303 = gem_v_node;
                        if (gem_truthy(gem_table_get(_t1303, gem_string("value")))) {
#line 508 "compiler/main.gem"
    GemVal _t1304 = gem_table_new();
    gem_table_set(_t1304, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t1304, gem_string("setup"), gem_string(""));
                            GemVal _t1305 = _t1304;
                            gem_pop_frame();
                            return _t1305;
                        }
#line 510 "compiler/main.gem"
    GemVal _t1306 = gem_table_new();
    gem_table_set(_t1306, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t1306, gem_string("setup"), gem_string(""));
                        GemVal _t1307 = _t1306;
                        gem_pop_frame();
                        return _t1307;
                    } else {
#line 511 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("nil")))) {
#line 512 "compiler/main.gem"
    GemVal _t1308 = gem_table_new();
    gem_table_set(_t1308, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1308, gem_string("setup"), gem_string(""));
                            GemVal _t1309 = _t1308;
                            gem_pop_frame();
                            return _t1309;
                        } else {
#line 513 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 514 "compiler/main.gem"
    GemVal _t1310 = gem_v_node;
                                GemVal gem_v_name = gem_table_get(_t1310, gem_string("name"));
#line 515 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
#line 516 "compiler/main.gem"
    GemVal _t1311 = gem_table_new();
    gem_table_set(_t1311, gem_string("expr"), gem_string("gem_make_fn(gem_print, NULL)"));
    gem_table_set(_t1311, gem_string("setup"), gem_string(""));
                                    GemVal _t1312 = _t1311;
                                    gem_pop_frame();
                                    return _t1312;
                                } else {
#line 517 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 518 "compiler/main.gem"
    GemVal _t1313 = gem_table_new();
    gem_table_set(_t1313, gem_string("expr"), gem_string("gem_make_fn(gem_error_fn, NULL)"));
    gem_table_set(_t1313, gem_string("setup"), gem_string(""));
                                        GemVal _t1314 = _t1313;
                                        gem_pop_frame();
                                        return _t1314;
                                    } else {
#line 519 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("len")))) {
#line 520 "compiler/main.gem"
    GemVal _t1315 = gem_table_new();
    gem_table_set(_t1315, gem_string("expr"), gem_string("gem_make_fn(gem_len_fn, NULL)"));
    gem_table_set(_t1315, gem_string("setup"), gem_string(""));
                                            GemVal _t1316 = _t1315;
                                            gem_pop_frame();
                                            return _t1316;
                                        } else {
#line 521 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("type")))) {
#line 522 "compiler/main.gem"
    GemVal _t1317 = gem_table_new();
    gem_table_set(_t1317, gem_string("expr"), gem_string("gem_make_fn(gem_type_fn, NULL)"));
    gem_table_set(_t1317, gem_string("setup"), gem_string(""));
                                                GemVal _t1318 = _t1317;
                                                gem_pop_frame();
                                                return _t1318;
                                            } else {
#line 523 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("to_string")))) {
#line 524 "compiler/main.gem"
    GemVal _t1319 = gem_table_new();
    gem_table_set(_t1319, gem_string("expr"), gem_string("gem_make_fn(gem_to_string_fn, NULL)"));
    gem_table_set(_t1319, gem_string("setup"), gem_string(""));
                                                    GemVal _t1320 = _t1319;
                                                    gem_pop_frame();
                                                    return _t1320;
                                                } else {
#line 525 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("push")))) {
#line 526 "compiler/main.gem"
    GemVal _t1321 = gem_table_new();
    gem_table_set(_t1321, gem_string("expr"), gem_string("gem_make_fn(gem_push_fn, NULL)"));
    gem_table_set(_t1321, gem_string("setup"), gem_string(""));
                                                        GemVal _t1322 = _t1321;
                                                        gem_pop_frame();
                                                        return _t1322;
                                                    } else {
#line 527 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("pcall")))) {
#line 528 "compiler/main.gem"
    GemVal _t1323 = gem_table_new();
    gem_table_set(_t1323, gem_string("expr"), gem_string("gem_make_fn(gem_pcall_fn, NULL)"));
    gem_table_set(_t1323, gem_string("setup"), gem_string(""));
                                                            GemVal _t1324 = _t1323;
                                                            gem_pop_frame();
                                                            return _t1324;
                                                        } else {
#line 529 "compiler/main.gem"
                                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("keys")))) {
#line 530 "compiler/main.gem"
    GemVal _t1325 = gem_table_new();
    gem_table_set(_t1325, gem_string("expr"), gem_string("gem_make_fn(gem_keys_fn, NULL)"));
    gem_table_set(_t1325, gem_string("setup"), gem_string(""));
                                                                GemVal _t1326 = _t1325;
                                                                gem_pop_frame();
                                                                return _t1326;
                                                            } else {
#line 531 "compiler/main.gem"
                                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("str_replace")))) {
#line 532 "compiler/main.gem"
    GemVal _t1327 = gem_table_new();
    gem_table_set(_t1327, gem_string("expr"), gem_string("gem_make_fn(gem_str_replace_fn, NULL)"));
    gem_table_set(_t1327, gem_string("setup"), gem_string(""));
                                                                    GemVal _t1328 = _t1327;
                                                                    gem_pop_frame();
                                                                    return _t1328;
                                                                } else {
#line 533 "compiler/main.gem"
                                                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("has_key")))) {
#line 534 "compiler/main.gem"
    GemVal _t1329 = gem_table_new();
    gem_table_set(_t1329, gem_string("expr"), gem_string("gem_make_fn(gem_has_key_fn, NULL)"));
    gem_table_set(_t1329, gem_string("setup"), gem_string(""));
                                                                        GemVal _t1330 = _t1329;
                                                                        gem_pop_frame();
                                                                        return _t1330;
                                                                    } else {
#line 535 "compiler/main.gem"
                                                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("split")))) {
#line 536 "compiler/main.gem"
    GemVal _t1331 = gem_table_new();
    gem_table_set(_t1331, gem_string("expr"), gem_string("gem_make_fn(gem_split_fn, NULL)"));
    gem_table_set(_t1331, gem_string("setup"), gem_string(""));
                                                                            GemVal _t1332 = _t1331;
                                                                            gem_pop_frame();
                                                                            return _t1332;
                                                                        } else {
#line 537 "compiler/main.gem"
                                                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("substr")))) {
#line 538 "compiler/main.gem"
    GemVal _t1333 = gem_table_new();
    gem_table_set(_t1333, gem_string("expr"), gem_string("gem_make_fn(gem_substr_fn, NULL)"));
    gem_table_set(_t1333, gem_string("setup"), gem_string(""));
                                                                                GemVal _t1334 = _t1333;
                                                                                gem_pop_frame();
                                                                                return _t1334;
                                                                            } else {
#line 539 "compiler/main.gem"
                                                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("index_of")))) {
#line 540 "compiler/main.gem"
    GemVal _t1335 = gem_table_new();
    gem_table_set(_t1335, gem_string("expr"), gem_string("gem_make_fn(gem_index_of_fn, NULL)"));
    gem_table_set(_t1335, gem_string("setup"), gem_string(""));
                                                                                    GemVal _t1336 = _t1335;
                                                                                    gem_pop_frame();
                                                                                    return _t1336;
                                                                                } else {
#line 541 "compiler/main.gem"
                                                                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("chr")))) {
#line 542 "compiler/main.gem"
    GemVal _t1337 = gem_table_new();
    gem_table_set(_t1337, gem_string("expr"), gem_string("gem_make_fn(gem_chr_fn, NULL)"));
    gem_table_set(_t1337, gem_string("setup"), gem_string(""));
                                                                                        GemVal _t1338 = _t1337;
                                                                                        gem_pop_frame();
                                                                                        return _t1338;
                                                                                    } else {
#line 543 "compiler/main.gem"
                                                                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("ord")))) {
#line 544 "compiler/main.gem"
    GemVal _t1339 = gem_table_new();
    gem_table_set(_t1339, gem_string("expr"), gem_string("gem_make_fn(gem_ord_fn, NULL)"));
    gem_table_set(_t1339, gem_string("setup"), gem_string(""));
                                                                                            GemVal _t1340 = _t1339;
                                                                                            gem_pop_frame();
                                                                                            return _t1340;
                                                                                        } else {
#line 545 "compiler/main.gem"
                                                                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("spawn")))) {
#line 546 "compiler/main.gem"
    GemVal _t1341 = gem_table_new();
    gem_table_set(_t1341, gem_string("expr"), gem_string("gem_make_fn(gem_spawn_builtin, NULL)"));
    gem_table_set(_t1341, gem_string("setup"), gem_string(""));
                                                                                                GemVal _t1342 = _t1341;
                                                                                                gem_pop_frame();
                                                                                                return _t1342;
                                                                                            } else {
#line 547 "compiler/main.gem"
                                                                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("send")))) {
#line 548 "compiler/main.gem"
    GemVal _t1343 = gem_table_new();
    gem_table_set(_t1343, gem_string("expr"), gem_string("gem_make_fn(gem_send_builtin, NULL)"));
    gem_table_set(_t1343, gem_string("setup"), gem_string(""));
                                                                                                    GemVal _t1344 = _t1343;
                                                                                                    gem_pop_frame();
                                                                                                    return _t1344;
                                                                                                } else {
#line 549 "compiler/main.gem"
                                                                                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("receive")))) {
#line 550 "compiler/main.gem"
    GemVal _t1345 = gem_table_new();
    gem_table_set(_t1345, gem_string("expr"), gem_string("gem_make_fn(gem_receive_builtin, NULL)"));
    gem_table_set(_t1345, gem_string("setup"), gem_string(""));
                                                                                                        GemVal _t1346 = _t1345;
                                                                                                        gem_pop_frame();
                                                                                                        return _t1346;
                                                                                                    } else {
#line 551 "compiler/main.gem"
                                                                                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("self")))) {
#line 552 "compiler/main.gem"
    GemVal _t1347 = gem_table_new();
    gem_table_set(_t1347, gem_string("expr"), gem_string("gem_make_fn(gem_self_builtin, NULL)"));
    gem_table_set(_t1347, gem_string("setup"), gem_string(""));
                                                                                                            GemVal _t1348 = _t1347;
                                                                                                            gem_pop_frame();
                                                                                                            return _t1348;
                                                                                                        } else {
#line 553 "compiler/main.gem"
    GemVal _t1349[] = {(*gem_v_defined_fns), gem_v_name};
                                                                                                            if (gem_truthy(gem_fn_set_contains(NULL, _t1349, 2))) {
#line 554 "compiler/main.gem"
    GemVal _t1350 = gem_table_new();
    GemVal _t1351[] = {gem_v_name};
    gem_table_set(_t1350, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t1351, 1)), gem_string(", NULL)")));
    gem_table_set(_t1350, gem_string("setup"), gem_string(""));
                                                                                                                GemVal _t1352 = _t1350;
                                                                                                                gem_pop_frame();
                                                                                                                return _t1352;
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
                                                }
                                            }
                                        }
                                    }
                                }
#line 556 "compiler/main.gem"
    GemVal _t1353 = (*gem_v_mangle);
    GemVal _t1354[] = {gem_v_name};
                                GemVal gem_v_mname = _t1353.fn(_t1353.env, _t1354, 1);
#line 557 "compiler/main.gem"
    GemVal _t1355[] = {(*gem_v_boxed_vars), gem_v_name};
                                if (gem_truthy(gem_fn_set_contains(NULL, _t1355, 2))) {
#line 558 "compiler/main.gem"
    GemVal _t1356 = gem_table_new();
    GemVal _t1357[] = {gem_v_mname};
    gem_table_set(_t1356, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t1357, 1)), gem_string(")")));
    gem_table_set(_t1356, gem_string("setup"), gem_string(""));
                                    GemVal _t1358 = _t1356;
                                    gem_pop_frame();
                                    return _t1358;
                                }
#line 560 "compiler/main.gem"
    GemVal _t1359 = gem_table_new();
    gem_table_set(_t1359, gem_string("expr"), gem_v_mname);
    gem_table_set(_t1359, gem_string("setup"), gem_string(""));
                                GemVal _t1360 = _t1359;
                                gem_pop_frame();
                                return _t1360;
                            } else {
#line 561 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 562 "compiler/main.gem"
    GemVal _t1361 = (*gem_v_compile_call);
    GemVal _t1362[] = {gem_v_node};
                                    GemVal _t1363 = _t1361.fn(_t1361.env, _t1362, 1);
                                    gem_pop_frame();
                                    return _t1363;
                                } else {
#line 563 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("binop")))) {
#line 564 "compiler/main.gem"
    GemVal _t1364 = (*gem_v_compile_binop);
    GemVal _t1365[] = {gem_v_node};
                                        GemVal _t1366 = _t1364.fn(_t1364.env, _t1365, 1);
                                        gem_pop_frame();
                                        return _t1366;
                                    } else {
#line 565 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("unop")))) {
#line 566 "compiler/main.gem"
    GemVal _t1367 = gem_v_node;
    GemVal _t1368 = (*gem_v_compile_expr);
    GemVal _t1369[] = {gem_table_get(_t1367, gem_string("expr"))};
                                            GemVal gem_v_r = _t1368.fn(_t1368.env, _t1369, 1);
#line 567 "compiler/main.gem"
    GemVal _t1370 = gem_v_node;
                                            if (gem_truthy(gem_eq(gem_table_get(_t1370, gem_string("op")), gem_string("-")))) {
#line 568 "compiler/main.gem"
    GemVal _t1371 = gem_table_new();
    GemVal _t1372 = gem_v_r;
    GemVal _t1373[] = {gem_table_get(_t1372, gem_string("expr"))};
    gem_table_set(_t1371, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t1373, 1)), gem_string(")")));
    GemVal _t1374 = gem_v_r;
    gem_table_set(_t1371, gem_string("setup"), gem_table_get(_t1374, gem_string("setup")));
                                                GemVal _t1375 = _t1371;
                                                gem_pop_frame();
                                                return _t1375;
                                            } else {
#line 569 "compiler/main.gem"
    GemVal _t1376 = gem_v_node;
                                                if (gem_truthy(gem_eq(gem_table_get(_t1376, gem_string("op")), gem_string("not")))) {
#line 570 "compiler/main.gem"
    GemVal _t1377 = gem_table_new();
    GemVal _t1378 = gem_v_r;
    GemVal _t1379[] = {gem_table_get(_t1378, gem_string("expr"))};
    gem_table_set(_t1377, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t1379, 1)), gem_string(")")));
    GemVal _t1380 = gem_v_r;
    gem_table_set(_t1377, gem_string("setup"), gem_table_get(_t1380, gem_string("setup")));
                                                    GemVal _t1381 = _t1377;
                                                    gem_pop_frame();
                                                    return _t1381;
                                                }
                                            }
                                        } else {
#line 572 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 573 "compiler/main.gem"
    GemVal _t1382 = (*gem_v_compile_anon_fn);
    GemVal _t1383[] = {gem_v_node};
                                                GemVal _t1384 = _t1382.fn(_t1382.env, _t1383, 1);
                                                gem_pop_frame();
                                                return _t1384;
                                            } else {
#line 574 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("table")))) {
#line 575 "compiler/main.gem"
    GemVal _t1385 = (*gem_v_compile_table);
    GemVal _t1386[] = {gem_v_node};
                                                    GemVal _t1387 = _t1385.fn(_t1385.env, _t1386, 1);
                                                    gem_pop_frame();
                                                    return _t1387;
                                                } else {
#line 576 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("array")))) {
#line 577 "compiler/main.gem"
    GemVal _t1388 = (*gem_v_compile_array);
    GemVal _t1389[] = {gem_v_node};
                                                        GemVal _t1390 = _t1388.fn(_t1388.env, _t1389, 1);
                                                        gem_pop_frame();
                                                        return _t1390;
                                                    } else {
#line 578 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot")))) {
#line 579 "compiler/main.gem"
    GemVal _t1391 = gem_v_node;
    GemVal _t1392 = (*gem_v_compile_expr);
    GemVal _t1393[] = {gem_table_get(_t1391, gem_string("object"))};
                                                            GemVal gem_v_r = _t1392.fn(_t1392.env, _t1393, 1);
#line 580 "compiler/main.gem"
    GemVal _t1394 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1394.fn(_t1394.env, NULL, 0);
#line 581 "compiler/main.gem"
    GemVal _t1395 = gem_v_r;
    GemVal _t1396[] = {gem_v_t};
    GemVal _t1397 = gem_v_r;
    GemVal _t1398[] = {gem_table_get(_t1397, gem_string("expr"))};
                                                            GemVal gem_v_setup = gem_add(gem_table_get(_t1395, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1396, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1398, 1)), gem_string(";\n")));
#line 582 "compiler/main.gem"
    GemVal _t1399 = gem_v_node;
    GemVal _t1400[] = {gem_table_get(_t1399, gem_string("field"))};
                                                            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1400, 1);
#line 583 "compiler/main.gem"
    GemVal _t1401 = gem_table_new();
    GemVal _t1402[] = {gem_v_t};
    GemVal _t1403[] = {gem_v_escaped};
    gem_table_set(_t1401, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1402, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1403, 1)), gem_string("\"))")));
    gem_table_set(_t1401, gem_string("setup"), gem_v_setup);
                                                            GemVal _t1404 = _t1401;
                                                            gem_pop_frame();
                                                            return _t1404;
                                                        } else {
#line 584 "compiler/main.gem"
                                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("index")))) {
#line 585 "compiler/main.gem"
    GemVal _t1405 = gem_v_node;
    GemVal _t1406 = (*gem_v_compile_expr);
    GemVal _t1407[] = {gem_table_get(_t1405, gem_string("object"))};
                                                                GemVal gem_v_obj_r = _t1406.fn(_t1406.env, _t1407, 1);
#line 586 "compiler/main.gem"
    GemVal _t1408 = gem_v_node;
    GemVal _t1409 = (*gem_v_compile_expr);
    GemVal _t1410[] = {gem_table_get(_t1408, gem_string("key"))};
                                                                GemVal gem_v_key_r = _t1409.fn(_t1409.env, _t1410, 1);
#line 587 "compiler/main.gem"
    GemVal _t1411 = gem_table_new();
    GemVal _t1412 = gem_v_obj_r;
    GemVal _t1413[] = {gem_table_get(_t1412, gem_string("expr"))};
    GemVal _t1414 = gem_v_key_r;
    GemVal _t1415[] = {gem_table_get(_t1414, gem_string("expr"))};
    gem_table_set(_t1411, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1413, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1415, 1)), gem_string(")")));
    GemVal _t1416 = gem_v_obj_r;
    GemVal _t1417 = gem_v_key_r;
    gem_table_set(_t1411, gem_string("setup"), gem_add(gem_table_get(_t1416, gem_string("setup")), gem_table_get(_t1417, gem_string("setup"))));
                                                                GemVal _t1418 = _t1411;
                                                                gem_pop_frame();
                                                                return _t1418;
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
    GemVal _t1419 = gem_table_new();
    gem_table_set(_t1419, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1419, gem_string("setup"), gem_add(gem_add(gem_string("/* unknown node: "), gem_v_tag), gem_string(" */\n")));
    GemVal _t1420 = _t1419;
    gem_pop_frame();
    return _t1420;
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
#line 596 "compiler/main.gem"
    GemVal _t1422 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t1422, gem_string("entries"));
#line 597 "compiler/main.gem"
    GemVal _t1423 = (*gem_v_tmp);
    GemVal gem_v_t = _t1423.fn(_t1423.env, NULL, 0);
#line 598 "compiler/main.gem"
    GemVal _t1424[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1424, 1)), gem_string(" = gem_table_new();\n"));
    {
#line 599 "compiler/main.gem"
        GemVal gem_v__for_items_21 = gem_v_entries;
#line 599 "compiler/main.gem"
        GemVal gem_v__for_i_21 = gem_int(0);
#line 599 "compiler/main.gem"
        while (1) {
            GemVal _t1425[] = {gem_v__for_items_21};
            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t1425, 1)))) break;
#line 599 "compiler/main.gem"
            GemVal gem_v_entry = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
#line 599 "compiler/main.gem"
            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 600 "compiler/main.gem"
    GemVal _t1426 = gem_v_entry;
    GemVal _t1427 = (*gem_v_compile_expr);
    GemVal _t1428[] = {gem_table_get(_t1426, gem_string("value"))};
            GemVal gem_v_val_r = _t1427.fn(_t1427.env, _t1428, 1);
#line 601 "compiler/main.gem"
    GemVal _t1429 = gem_v_entry;
    GemVal _t1430[] = {gem_table_get(_t1429, gem_string("key"))};
            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1430, 1);
#line 602 "compiler/main.gem"
    GemVal _t1431 = gem_v_val_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1431, gem_string("setup")));
#line 603 "compiler/main.gem"
    GemVal _t1432[] = {gem_v_t};
    GemVal _t1433[] = {gem_v_escaped};
    GemVal _t1434 = gem_v_val_r;
    GemVal _t1435[] = {gem_table_get(_t1434, gem_string("expr"))};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1432, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1433, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1435, 1)), gem_string(");\n")));
        }
    }
    GemVal _t1436 = gem_table_new();
    gem_table_set(_t1436, gem_string("expr"), gem_v_t);
    gem_table_set(_t1436, gem_string("setup"), gem_v_setup);
    GemVal _t1437 = _t1436;
    gem_pop_frame();
    return _t1437;
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
#line 611 "compiler/main.gem"
    GemVal _t1439 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t1439, gem_string("elements"));
#line 612 "compiler/main.gem"
    GemVal _t1440 = (*gem_v_tmp);
    GemVal gem_v_t = _t1440.fn(_t1440.env, NULL, 0);
#line 613 "compiler/main.gem"
    GemVal _t1441[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1441, 1)), gem_string(" = gem_table_new();\n"));
    {
#line 614 "compiler/main.gem"
        GemVal gem_v__for_i_22 = gem_int(0);
#line 614 "compiler/main.gem"
    GemVal _t1442[] = {gem_v_elements};
        GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1442, 1);
#line 614 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 614 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_22;
#line 614 "compiler/main.gem"
            gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 615 "compiler/main.gem"
    GemVal _t1443 = (*gem_v_compile_expr);
    GemVal _t1444[] = {gem_table_get(gem_v_elements, gem_v_i)};
            GemVal gem_v_elem_r = _t1443.fn(_t1443.env, _t1444, 1);
#line 616 "compiler/main.gem"
    GemVal _t1445 = gem_v_elem_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1445, gem_string("setup")));
#line 617 "compiler/main.gem"
    GemVal _t1446[] = {gem_v_t};
    GemVal _t1447[] = {gem_v_i};
    GemVal _t1448 = gem_v_elem_r;
    GemVal _t1449[] = {gem_table_get(_t1448, gem_string("expr"))};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1446, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t1447, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t1449, 1)), gem_string(");\n")));
        }
    }
    GemVal _t1450 = gem_table_new();
    gem_table_set(_t1450, gem_string("expr"), gem_v_t);
    gem_table_set(_t1450, gem_string("setup"), gem_v_setup);
    GemVal _t1451 = _t1450;
    gem_pop_frame();
    return _t1451;
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
#line 625 "compiler/main.gem"
    GemVal _t1453 = gem_v_node;
    GemVal _t1454[] = {gem_table_get(_t1453, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1454, 1);
#line 626 "compiler/main.gem"
    GemVal _t1455 = gem_v_node;
    GemVal _t1456 = (*gem_v_collect_free_vars);
    GemVal _t1457[] = {gem_table_get(_t1455, gem_string("body")), gem_v_inner_defined};
    GemVal gem_v_free = _t1456.fn(_t1456.env, _t1457, 2);
#line 627 "compiler/main.gem"
    GemVal _t1458[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t1458, 1);
#line 629 "compiler/main.gem"
    GemVal _t1459[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1459, 1), gem_int(0)))) {
#line 630 "compiler/main.gem"
    GemVal _t1460 = gem_table_new();
    GemVal _t1461 = (*gem_v_compile_closure_fn);
    GemVal _t1462[] = {gem_v_node, _t1460};
        GemVal gem_v_result = _t1461.fn(_t1461.env, _t1462, 2);
#line 631 "compiler/main.gem"
    GemVal _t1463 = gem_table_new();
    GemVal _t1464 = gem_v_result;
    GemVal _t1465[] = {gem_table_get(_t1464, gem_string("fn_name"))};
    gem_table_set(_t1463, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1465, 1)), gem_string(", NULL)")));
    gem_table_set(_t1463, gem_string("setup"), gem_string(""));
        GemVal _t1466 = _t1463;
        gem_pop_frame();
        return _t1466;
    }
#line 634 "compiler/main.gem"
    GemVal _t1467 = (*gem_v_compile_closure_fn);
    GemVal _t1468[] = {gem_v_node, gem_v_captures};
    GemVal gem_v_result = _t1467.fn(_t1467.env, _t1468, 2);
#line 636 "compiler/main.gem"
    GemVal gem_v_setup = gem_string("");
#line 637 "compiler/main.gem"
    GemVal _t1469 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t1469.fn(_t1469.env, NULL, 0);
#line 638 "compiler/main.gem"
    GemVal _t1470 = gem_v_result;
    GemVal _t1471[] = {gem_table_get(_t1470, gem_string("struct_name"))};
    GemVal _t1472[] = {gem_v_env_tmp};
    GemVal _t1473 = gem_v_result;
    GemVal _t1474[] = {gem_table_get(_t1473, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t1471, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t1472, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t1474, 1)), gem_string("));\n")));
    {
#line 639 "compiler/main.gem"
        GemVal gem_v__for_items_23 = gem_v_captures;
#line 639 "compiler/main.gem"
        GemVal gem_v__for_i_23 = gem_int(0);
#line 639 "compiler/main.gem"
        while (1) {
            GemVal _t1475[] = {gem_v__for_items_23};
            if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_len_fn(NULL, _t1475, 1)))) break;
#line 639 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_23, gem_v__for_i_23);
#line 639 "compiler/main.gem"
            gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 640 "compiler/main.gem"
    GemVal _t1476 = (*gem_v_mangle);
    GemVal _t1477[] = {gem_v_cap};
            GemVal gem_v_mc = _t1476.fn(_t1476.env, _t1477, 1);
#line 641 "compiler/main.gem"
    GemVal _t1478[] = {(*gem_v_boxed_vars), gem_v_cap};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1478, 2))) {
#line 642 "compiler/main.gem"
    GemVal _t1479[] = {gem_v_env_tmp};
    GemVal _t1480[] = {gem_v_mc};
    GemVal _t1481[] = {gem_v_mc};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1479, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1480, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1481, 1)), gem_string(";\n")));
            } else {
#line 644 "compiler/main.gem"
    GemVal _t1482[] = {gem_v_env_tmp};
    GemVal _t1483[] = {gem_v_mc};
    GemVal _t1484[] = {gem_v_mc};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1482, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1483, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t1484, 1)), gem_string(";\n")));
            }
        }
    }
    GemVal _t1485 = gem_table_new();
    GemVal _t1486 = gem_v_result;
    GemVal _t1487[] = {gem_table_get(_t1486, gem_string("fn_name"))};
    GemVal _t1488[] = {gem_v_env_tmp};
    gem_table_set(_t1485, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1487, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1488, 1)), gem_string(")")));
    gem_table_set(_t1485, gem_string("setup"), gem_v_setup);
    GemVal _t1489 = _t1485;
    gem_pop_frame();
    return _t1489;
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
#line 653 "compiler/main.gem"
    GemVal _t1491 = gem_v_node;
    GemVal gem_v_func = gem_table_get(_t1491, gem_string("func"));
#line 654 "compiler/main.gem"
    GemVal _t1492 = gem_v_node;
    GemVal gem_v_args = gem_table_get(_t1492, gem_string("args"));
#line 657 "compiler/main.gem"
    GemVal _t1493 = gem_v_func;
    GemVal _t1495;
    if (!gem_truthy(gem_eq(gem_table_get(_t1493, gem_string("tag")), gem_string("dot")))) {
        _t1495 = gem_eq(gem_table_get(_t1493, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t1494 = gem_v_func;
        _t1495 = gem_eq(gem_table_get(_t1494, gem_string("field")), gem_string("len"));
    }
    GemVal _t1497;
    if (!gem_truthy(_t1495)) {
        _t1497 = _t1495;
    } else {
        GemVal _t1496[] = {gem_v_args};
        _t1497 = gem_eq(gem_len_fn(NULL, _t1496, 1), gem_int(0));
    }
    if (gem_truthy(_t1497)) {
#line 658 "compiler/main.gem"
    GemVal _t1498 = gem_v_func;
    GemVal _t1499 = (*gem_v_compile_expr);
    GemVal _t1500[] = {gem_table_get(_t1498, gem_string("object"))};
        GemVal gem_v_obj_r = _t1499.fn(_t1499.env, _t1500, 1);
#line 659 "compiler/main.gem"
    GemVal _t1501 = gem_table_new();
    GemVal _t1502 = gem_v_obj_r;
    GemVal _t1503[] = {gem_table_get(_t1502, gem_string("expr"))};
    gem_table_set(_t1501, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t1503, 1)), gem_string(")")));
    GemVal _t1504 = gem_v_obj_r;
    gem_table_set(_t1501, gem_string("setup"), gem_table_get(_t1504, gem_string("setup")));
        GemVal _t1505 = _t1501;
        gem_pop_frame();
        return _t1505;
    }
#line 663 "compiler/main.gem"
    GemVal _t1506 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t1506, gem_string("tag")), gem_string("var")))) {
#line 664 "compiler/main.gem"
    GemVal _t1507 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t1507, gem_string("name"));
#line 666 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 667 "compiler/main.gem"
    GemVal _t1508 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t1508, gem_string("line"));
#line 668 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 669 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 671 "compiler/main.gem"
    GemVal _t1509[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t1509, 1);
#line 672 "compiler/main.gem"
            GemVal gem_v_arg_setups = gem_string("");
#line 673 "compiler/main.gem"
    GemVal _t1510 = gem_table_new();
            GemVal gem_v_arg_exprs = _t1510;
            {
#line 674 "compiler/main.gem"
                GemVal gem_v__for_i_24 = gem_int(0);
#line 674 "compiler/main.gem"
    GemVal _t1511[] = {gem_v_args};
                GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1511, 1);
#line 674 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 674 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_24;
#line 674 "compiler/main.gem"
                    gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 675 "compiler/main.gem"
    GemVal _t1512 = (*gem_v_compile_expr);
    GemVal _t1513[] = {gem_table_get(gem_v_args, gem_v_i)};
                    GemVal gem_v_r = _t1512.fn(_t1512.env, _t1513, 1);
#line 676 "compiler/main.gem"
    GemVal _t1514 = gem_v_r;
                    gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1514, gem_string("setup")));
#line 677 "compiler/main.gem"
    GemVal _t1515 = gem_v_r;
    GemVal _t1516[] = {gem_v_arg_exprs, gem_table_get(_t1515, gem_string("expr"))};
                    (void)(gem_push_fn(NULL, _t1516, 2));
                }
            }
#line 679 "compiler/main.gem"
    GemVal _t1517[] = {gem_v_args};
            GemVal gem_v_argc = gem_len_fn(NULL, _t1517, 1);
#line 680 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 681 "compiler/main.gem"
    GemVal _t1518 = gem_table_new();
    GemVal _t1519[] = {gem_v_escaped_file};
    GemVal _t1520[] = {gem_v_line};
    gem_table_set(_t1518, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1519, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1520, 1)), gem_string(", NULL, 0)")));
    gem_table_set(_t1518, gem_string("setup"), gem_v_arg_setups);
                GemVal _t1521 = _t1518;
                gem_pop_frame();
                return _t1521;
            }
#line 683 "compiler/main.gem"
    GemVal _t1522 = (*gem_v_tmp);
            GemVal gem_v_t = _t1522.fn(_t1522.env, NULL, 0);
#line 684 "compiler/main.gem"
            GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
            {
#line 685 "compiler/main.gem"
                GemVal gem_v__for_i_25 = gem_int(1);
#line 685 "compiler/main.gem"
                GemVal gem_v__for_limit_25 = gem_v_argc;
#line 685 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 685 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_25;
#line 685 "compiler/main.gem"
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 686 "compiler/main.gem"
                    gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                }
            }
#line 688 "compiler/main.gem"
    GemVal _t1523[] = {gem_v_t};
    GemVal _t1524[] = {gem_v_arr};
            GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1523, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1524, 1)), gem_string("};\n")));
#line 689 "compiler/main.gem"
    GemVal _t1525[] = {gem_v_escaped_file};
    GemVal _t1526[] = {gem_v_line};
    GemVal _t1527[] = {gem_v_t};
    GemVal _t1528[] = {gem_v_argc};
            GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1525, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1526, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1527, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1528, 1)), gem_string(")"));
#line 690 "compiler/main.gem"
    GemVal _t1529 = gem_table_new();
    gem_table_set(_t1529, gem_string("expr"), gem_v_call);
    gem_table_set(_t1529, gem_string("setup"), gem_v_setup);
            GemVal _t1530 = _t1529;
            gem_pop_frame();
            return _t1530;
        } else {
#line 691 "compiler/main.gem"
    GemVal _t1531;
    if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
        _t1531 = gem_eq(gem_v_name, gem_string("print"));
    } else {
        _t1531 = gem_eq(gem_v_name, gem_string("len"));
    }
    GemVal _t1532;
    if (gem_truthy(_t1531)) {
        _t1532 = _t1531;
    } else {
        _t1532 = gem_eq(gem_v_name, gem_string("type"));
    }
    GemVal _t1533;
    if (gem_truthy(_t1532)) {
        _t1533 = _t1532;
    } else {
        _t1533 = gem_eq(gem_v_name, gem_string("to_string"));
    }
    GemVal _t1534;
    if (gem_truthy(_t1533)) {
        _t1534 = _t1533;
    } else {
        _t1534 = gem_eq(gem_v_name, gem_string("push"));
    }
    GemVal _t1535;
    if (gem_truthy(_t1534)) {
        _t1535 = _t1534;
    } else {
        _t1535 = gem_eq(gem_v_name, gem_string("pcall"));
    }
    GemVal _t1536;
    if (gem_truthy(_t1535)) {
        _t1536 = _t1535;
    } else {
        _t1536 = gem_eq(gem_v_name, gem_string("keys"));
    }
    GemVal _t1537;
    if (gem_truthy(_t1536)) {
        _t1537 = _t1536;
    } else {
        _t1537 = gem_eq(gem_v_name, gem_string("str_replace"));
    }
    GemVal _t1538;
    if (gem_truthy(_t1537)) {
        _t1538 = _t1537;
    } else {
        _t1538 = gem_eq(gem_v_name, gem_string("has_key"));
    }
    GemVal _t1539;
    if (gem_truthy(_t1538)) {
        _t1539 = _t1538;
    } else {
        _t1539 = gem_eq(gem_v_name, gem_string("split"));
    }
    GemVal _t1540;
    if (gem_truthy(_t1539)) {
        _t1540 = _t1539;
    } else {
        _t1540 = gem_eq(gem_v_name, gem_string("substr"));
    }
    GemVal _t1541;
    if (gem_truthy(_t1540)) {
        _t1541 = _t1540;
    } else {
        _t1541 = gem_eq(gem_v_name, gem_string("index_of"));
    }
    GemVal _t1542;
    if (gem_truthy(_t1541)) {
        _t1542 = _t1541;
    } else {
        _t1542 = gem_eq(gem_v_name, gem_string("chr"));
    }
    GemVal _t1543;
    if (gem_truthy(_t1542)) {
        _t1543 = _t1542;
    } else {
        _t1543 = gem_eq(gem_v_name, gem_string("ord"));
    }
    GemVal _t1544;
    if (gem_truthy(_t1543)) {
        _t1544 = _t1543;
    } else {
        _t1544 = gem_eq(gem_v_name, gem_string("spawn"));
    }
    GemVal _t1545;
    if (gem_truthy(_t1544)) {
        _t1545 = _t1544;
    } else {
        _t1545 = gem_eq(gem_v_name, gem_string("send"));
    }
    GemVal _t1546;
    if (gem_truthy(_t1545)) {
        _t1546 = _t1545;
    } else {
        _t1546 = gem_eq(gem_v_name, gem_string("receive"));
    }
    GemVal _t1547;
    if (gem_truthy(_t1546)) {
        _t1547 = _t1546;
    } else {
        _t1547 = gem_eq(gem_v_name, gem_string("self"));
    }
            if (gem_truthy(_t1547)) {
#line 692 "compiler/main.gem"
                GemVal gem_v_fn_name = gem_string("");
#line 693 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
#line 694 "compiler/main.gem"
                    gem_v_fn_name = gem_string("gem_print");
                } else {
#line 695 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_name, gem_string("len")))) {
#line 696 "compiler/main.gem"
                        gem_v_fn_name = gem_string("gem_len_fn");
                    } else {
#line 697 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_name, gem_string("type")))) {
#line 698 "compiler/main.gem"
                            gem_v_fn_name = gem_string("gem_type_fn");
                        } else {
#line 699 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_name, gem_string("to_string")))) {
#line 700 "compiler/main.gem"
                                gem_v_fn_name = gem_string("gem_to_string_fn");
                            } else {
#line 701 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_name, gem_string("push")))) {
#line 702 "compiler/main.gem"
                                    gem_v_fn_name = gem_string("gem_push_fn");
                                } else {
#line 703 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("pcall")))) {
#line 704 "compiler/main.gem"
                                        gem_v_fn_name = gem_string("gem_pcall_fn");
                                    } else {
#line 705 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("keys")))) {
#line 706 "compiler/main.gem"
                                            gem_v_fn_name = gem_string("gem_keys_fn");
                                        } else {
#line 707 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("str_replace")))) {
#line 708 "compiler/main.gem"
                                                gem_v_fn_name = gem_string("gem_str_replace_fn");
                                            } else {
#line 709 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("has_key")))) {
#line 710 "compiler/main.gem"
                                                    gem_v_fn_name = gem_string("gem_has_key_fn");
                                                } else {
#line 711 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("split")))) {
#line 712 "compiler/main.gem"
                                                        gem_v_fn_name = gem_string("gem_split_fn");
                                                    } else {
#line 713 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("substr")))) {
#line 714 "compiler/main.gem"
                                                            gem_v_fn_name = gem_string("gem_substr_fn");
                                                        } else {
#line 715 "compiler/main.gem"
                                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("index_of")))) {
#line 716 "compiler/main.gem"
                                                                gem_v_fn_name = gem_string("gem_index_of_fn");
                                                            } else {
#line 717 "compiler/main.gem"
                                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("chr")))) {
#line 718 "compiler/main.gem"
                                                                    gem_v_fn_name = gem_string("gem_chr_fn");
                                                                } else {
#line 719 "compiler/main.gem"
                                                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("ord")))) {
#line 720 "compiler/main.gem"
                                                                        gem_v_fn_name = gem_string("gem_ord_fn");
                                                                    } else {
#line 721 "compiler/main.gem"
                                                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("spawn")))) {
#line 722 "compiler/main.gem"
                                                                            gem_v_fn_name = gem_string("gem_spawn_builtin");
                                                                        } else {
#line 723 "compiler/main.gem"
                                                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("send")))) {
#line 724 "compiler/main.gem"
                                                                                gem_v_fn_name = gem_string("gem_send_builtin");
                                                                            } else {
#line 725 "compiler/main.gem"
                                                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("receive")))) {
#line 726 "compiler/main.gem"
                                                                                    gem_v_fn_name = gem_string("gem_receive_builtin");
                                                                                } else {
#line 727 "compiler/main.gem"
                                                                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("self")))) {
#line 728 "compiler/main.gem"
                                                                                        gem_v_fn_name = gem_string("gem_self_builtin");
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
                        }
                    }
                }
#line 730 "compiler/main.gem"
                GemVal gem_v_arg_setups = gem_string("");
#line 731 "compiler/main.gem"
    GemVal _t1548 = gem_table_new();
                GemVal gem_v_arg_exprs = _t1548;
                {
#line 732 "compiler/main.gem"
                    GemVal gem_v__for_i_26 = gem_int(0);
#line 732 "compiler/main.gem"
    GemVal _t1549[] = {gem_v_args};
                    GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1549, 1);
#line 732 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 732 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_26;
#line 732 "compiler/main.gem"
                        gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 733 "compiler/main.gem"
    GemVal _t1550 = (*gem_v_compile_expr);
    GemVal _t1551[] = {gem_table_get(gem_v_args, gem_v_i)};
                        GemVal gem_v_r = _t1550.fn(_t1550.env, _t1551, 1);
#line 734 "compiler/main.gem"
    GemVal _t1552 = gem_v_r;
                        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1552, gem_string("setup")));
#line 735 "compiler/main.gem"
    GemVal _t1553 = gem_v_r;
    GemVal _t1554[] = {gem_v_arg_exprs, gem_table_get(_t1553, gem_string("expr"))};
                        (void)(gem_push_fn(NULL, _t1554, 2));
                    }
                }
#line 737 "compiler/main.gem"
    GemVal _t1555[] = {gem_v_args};
                GemVal gem_v_argc = gem_len_fn(NULL, _t1555, 1);
#line 738 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 739 "compiler/main.gem"
    GemVal _t1556 = gem_table_new();
    GemVal _t1557[] = {gem_v_fn_name};
    gem_table_set(_t1556, gem_string("expr"), gem_add(gem_to_string_fn(NULL, _t1557, 1), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1556, gem_string("setup"), gem_v_arg_setups);
                    GemVal _t1558 = _t1556;
                    gem_pop_frame();
                    return _t1558;
                }
#line 741 "compiler/main.gem"
    GemVal _t1559 = (*gem_v_tmp);
                GemVal gem_v_t = _t1559.fn(_t1559.env, NULL, 0);
#line 742 "compiler/main.gem"
                GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
                {
#line 743 "compiler/main.gem"
                    GemVal gem_v__for_i_27 = gem_int(1);
#line 743 "compiler/main.gem"
                    GemVal gem_v__for_limit_27 = gem_v_argc;
#line 743 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 743 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_27;
#line 743 "compiler/main.gem"
                        gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 744 "compiler/main.gem"
                        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                    }
                }
#line 746 "compiler/main.gem"
    GemVal _t1560[] = {gem_v_t};
    GemVal _t1561[] = {gem_v_arr};
                GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1560, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1561, 1)), gem_string("};\n")));
#line 747 "compiler/main.gem"
    GemVal _t1562 = gem_table_new();
    GemVal _t1563[] = {gem_v_fn_name};
    GemVal _t1564[] = {gem_v_t};
    GemVal _t1565[] = {gem_v_argc};
    gem_table_set(_t1562, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1563, 1), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1564, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1565, 1)), gem_string(")")));
    gem_table_set(_t1562, gem_string("setup"), gem_v_setup);
                GemVal _t1566 = _t1562;
                gem_pop_frame();
                return _t1566;
            } else {
#line 748 "compiler/main.gem"
    GemVal _t1567[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t1567, 2))) {
#line 749 "compiler/main.gem"
                    GemVal gem_v_arg_setups = gem_string("");
#line 750 "compiler/main.gem"
    GemVal _t1568 = gem_table_new();
                    GemVal gem_v_arg_exprs = _t1568;
                    {
#line 751 "compiler/main.gem"
                        GemVal gem_v__for_i_28 = gem_int(0);
#line 751 "compiler/main.gem"
    GemVal _t1569[] = {gem_v_args};
                        GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1569, 1);
#line 751 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 751 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_28;
#line 751 "compiler/main.gem"
                            gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 752 "compiler/main.gem"
    GemVal _t1570 = (*gem_v_compile_expr);
    GemVal _t1571[] = {gem_table_get(gem_v_args, gem_v_i)};
                            GemVal gem_v_r = _t1570.fn(_t1570.env, _t1571, 1);
#line 753 "compiler/main.gem"
    GemVal _t1572 = gem_v_r;
                            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1572, gem_string("setup")));
#line 754 "compiler/main.gem"
    GemVal _t1573 = gem_v_r;
    GemVal _t1574[] = {gem_v_arg_exprs, gem_table_get(_t1573, gem_string("expr"))};
                            (void)(gem_push_fn(NULL, _t1574, 2));
                        }
                    }
#line 756 "compiler/main.gem"
    GemVal _t1575[] = {gem_v_args};
                    GemVal gem_v_argc = gem_len_fn(NULL, _t1575, 1);
#line 757 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 758 "compiler/main.gem"
    GemVal _t1576 = gem_table_new();
    GemVal _t1577[] = {gem_v_name};
    gem_table_set(_t1576, gem_string("expr"), gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1577, 1)), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1576, gem_string("setup"), gem_v_arg_setups);
                        GemVal _t1578 = _t1576;
                        gem_pop_frame();
                        return _t1578;
                    }
#line 760 "compiler/main.gem"
    GemVal _t1579 = (*gem_v_tmp);
                    GemVal gem_v_t = _t1579.fn(_t1579.env, NULL, 0);
#line 761 "compiler/main.gem"
                    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
                    {
#line 762 "compiler/main.gem"
                        GemVal gem_v__for_i_29 = gem_int(1);
#line 762 "compiler/main.gem"
                        GemVal gem_v__for_limit_29 = gem_v_argc;
#line 762 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_v__for_limit_29))) break;
#line 762 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_29;
#line 762 "compiler/main.gem"
                            gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 763 "compiler/main.gem"
                            gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                        }
                    }
#line 765 "compiler/main.gem"
    GemVal _t1580[] = {gem_v_t};
    GemVal _t1581[] = {gem_v_arr};
                    GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1580, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1581, 1)), gem_string("};\n")));
#line 766 "compiler/main.gem"
    GemVal _t1582 = gem_table_new();
    GemVal _t1583[] = {gem_v_name};
    GemVal _t1584[] = {gem_v_t};
    GemVal _t1585[] = {gem_v_argc};
    gem_table_set(_t1582, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1583, 1)), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1584, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1585, 1)), gem_string(")")));
    gem_table_set(_t1582, gem_string("setup"), gem_v_setup);
                    GemVal _t1586 = _t1582;
                    gem_pop_frame();
                    return _t1586;
                }
            }
        }
    }
#line 771 "compiler/main.gem"
    GemVal _t1587 = (*gem_v_compile_expr);
    GemVal _t1588[] = {gem_v_func};
    GemVal gem_v_func_r = _t1587.fn(_t1587.env, _t1588, 1);
#line 772 "compiler/main.gem"
    GemVal gem_v_arg_setups = gem_string("");
#line 773 "compiler/main.gem"
    GemVal _t1589 = gem_table_new();
    GemVal gem_v_arg_exprs = _t1589;
    {
#line 774 "compiler/main.gem"
        GemVal gem_v__for_i_30 = gem_int(0);
#line 774 "compiler/main.gem"
    GemVal _t1590[] = {gem_v_args};
        GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t1590, 1);
#line 774 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 774 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_30;
#line 774 "compiler/main.gem"
            gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 775 "compiler/main.gem"
    GemVal _t1591 = (*gem_v_compile_expr);
    GemVal _t1592[] = {gem_table_get(gem_v_args, gem_v_i)};
            GemVal gem_v_r = _t1591.fn(_t1591.env, _t1592, 1);
#line 776 "compiler/main.gem"
    GemVal _t1593 = gem_v_r;
            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1593, gem_string("setup")));
#line 777 "compiler/main.gem"
    GemVal _t1594 = gem_v_r;
    GemVal _t1595[] = {gem_v_arg_exprs, gem_table_get(_t1594, gem_string("expr"))};
            (void)(gem_push_fn(NULL, _t1595, 2));
        }
    }
#line 779 "compiler/main.gem"
    GemVal _t1596 = gem_v_func_r;
    GemVal gem_v_setup = gem_add(gem_table_get(_t1596, gem_string("setup")), gem_v_arg_setups);
#line 780 "compiler/main.gem"
    GemVal _t1597[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t1597, 1);
#line 781 "compiler/main.gem"
    GemVal _t1598 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t1598.fn(_t1598.env, NULL, 0);
#line 782 "compiler/main.gem"
    GemVal _t1599[] = {gem_v_tmp_fn};
    GemVal _t1600 = gem_v_func_r;
    GemVal _t1601[] = {gem_table_get(_t1600, gem_string("expr"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1599, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1601, 1)), gem_string(";\n")));
#line 783 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 784 "compiler/main.gem"
    GemVal _t1602 = gem_table_new();
    GemVal _t1603[] = {gem_v_tmp_fn};
    GemVal _t1604[] = {gem_v_tmp_fn};
    gem_table_set(_t1602, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1603, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1604, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t1602, gem_string("setup"), gem_v_setup);
        GemVal _t1605 = _t1602;
        gem_pop_frame();
        return _t1605;
    }
#line 786 "compiler/main.gem"
    GemVal _t1606 = (*gem_v_tmp);
    GemVal gem_v_tmp_args = _t1606.fn(_t1606.env, NULL, 0);
#line 787 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
    {
#line 788 "compiler/main.gem"
        GemVal gem_v__for_i_31 = gem_int(1);
#line 788 "compiler/main.gem"
        GemVal gem_v__for_limit_31 = gem_v_argc;
#line 788 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_v__for_limit_31))) break;
#line 788 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_31;
#line 788 "compiler/main.gem"
            gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
#line 789 "compiler/main.gem"
            gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
        }
    }
#line 791 "compiler/main.gem"
    GemVal _t1607[] = {gem_v_tmp_args};
    GemVal _t1608[] = {gem_v_arr};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1607, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1608, 1)), gem_string("};\n")));
#line 792 "compiler/main.gem"
    GemVal _t1609[] = {gem_v_tmp_fn};
    GemVal _t1610[] = {gem_v_tmp_fn};
    GemVal _t1611[] = {gem_v_tmp_args};
    GemVal _t1612[] = {gem_v_argc};
    GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1609, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1610, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t1611, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1612, 1)), gem_string(")"));
    GemVal _t1613 = gem_table_new();
    gem_table_set(_t1613, gem_string("expr"), gem_v_call);
    gem_table_set(_t1613, gem_string("setup"), gem_v_setup);
    GemVal _t1614 = _t1613;
    gem_pop_frame();
    return _t1614;
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
#line 799 "compiler/main.gem"
    GemVal _t1616 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t1616, gem_string("op"));
#line 800 "compiler/main.gem"
    GemVal _t1617 = gem_v_node;
    GemVal _t1618 = (*gem_v_compile_expr);
    GemVal _t1619[] = {gem_table_get(_t1617, gem_string("left"))};
    GemVal gem_v_lc_r = _t1618.fn(_t1618.env, _t1619, 1);
#line 801 "compiler/main.gem"
    GemVal _t1620 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t1620, gem_string("expr"));
#line 802 "compiler/main.gem"
    GemVal _t1621 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t1621, gem_string("setup"));
#line 804 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 805 "compiler/main.gem"
    GemVal _t1622 = gem_v_node;
    GemVal _t1623 = (*gem_v_compile_expr);
    GemVal _t1624[] = {gem_table_get(_t1622, gem_string("right"))};
        GemVal gem_v_rc_r = _t1623.fn(_t1623.env, _t1624, 1);
#line 806 "compiler/main.gem"
    GemVal _t1625 = gem_table_new();
    GemVal _t1626[] = {gem_v_lc};
    GemVal _t1627 = gem_v_rc_r;
    GemVal _t1628[] = {gem_table_get(_t1627, gem_string("expr"))};
    gem_table_set(_t1625, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1626, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1628, 1)), gem_string(")")));
    GemVal _t1629 = gem_v_rc_r;
    gem_table_set(_t1625, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1629, gem_string("setup"))));
        GemVal _t1630 = _t1625;
        gem_pop_frame();
        return _t1630;
    } else {
#line 807 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 808 "compiler/main.gem"
    GemVal _t1631 = gem_v_node;
    GemVal _t1632 = (*gem_v_compile_expr);
    GemVal _t1633[] = {gem_table_get(_t1631, gem_string("right"))};
            GemVal gem_v_rc_r = _t1632.fn(_t1632.env, _t1633, 1);
#line 809 "compiler/main.gem"
    GemVal _t1634 = gem_table_new();
    GemVal _t1635[] = {gem_v_lc};
    GemVal _t1636 = gem_v_rc_r;
    GemVal _t1637[] = {gem_table_get(_t1636, gem_string("expr"))};
    gem_table_set(_t1634, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t1635, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1637, 1)), gem_string(")")));
    GemVal _t1638 = gem_v_rc_r;
    gem_table_set(_t1634, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1638, gem_string("setup"))));
            GemVal _t1639 = _t1634;
            gem_pop_frame();
            return _t1639;
        } else {
#line 810 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 811 "compiler/main.gem"
    GemVal _t1640 = gem_v_node;
    GemVal _t1641 = (*gem_v_compile_expr);
    GemVal _t1642[] = {gem_table_get(_t1640, gem_string("right"))};
                GemVal gem_v_rc_r = _t1641.fn(_t1641.env, _t1642, 1);
#line 812 "compiler/main.gem"
    GemVal _t1643 = gem_table_new();
    GemVal _t1644[] = {gem_v_lc};
    GemVal _t1645 = gem_v_rc_r;
    GemVal _t1646[] = {gem_table_get(_t1645, gem_string("expr"))};
    gem_table_set(_t1643, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t1644, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1646, 1)), gem_string(")")));
    GemVal _t1647 = gem_v_rc_r;
    gem_table_set(_t1643, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1647, gem_string("setup"))));
                GemVal _t1648 = _t1643;
                gem_pop_frame();
                return _t1648;
            } else {
#line 813 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 814 "compiler/main.gem"
    GemVal _t1649 = gem_v_node;
    GemVal _t1650 = (*gem_v_compile_expr);
    GemVal _t1651[] = {gem_table_get(_t1649, gem_string("right"))};
                    GemVal gem_v_rc_r = _t1650.fn(_t1650.env, _t1651, 1);
#line 815 "compiler/main.gem"
    GemVal _t1652 = gem_table_new();
    GemVal _t1653[] = {gem_v_lc};
    GemVal _t1654 = gem_v_rc_r;
    GemVal _t1655[] = {gem_table_get(_t1654, gem_string("expr"))};
    gem_table_set(_t1652, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t1653, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1655, 1)), gem_string(")")));
    GemVal _t1656 = gem_v_rc_r;
    gem_table_set(_t1652, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1656, gem_string("setup"))));
                    GemVal _t1657 = _t1652;
                    gem_pop_frame();
                    return _t1657;
                } else {
#line 816 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 817 "compiler/main.gem"
    GemVal _t1658 = gem_v_node;
    GemVal _t1659 = (*gem_v_compile_expr);
    GemVal _t1660[] = {gem_table_get(_t1658, gem_string("right"))};
                        GemVal gem_v_rc_r = _t1659.fn(_t1659.env, _t1660, 1);
#line 818 "compiler/main.gem"
    GemVal _t1661 = gem_table_new();
    GemVal _t1662[] = {gem_v_lc};
    GemVal _t1663 = gem_v_rc_r;
    GemVal _t1664[] = {gem_table_get(_t1663, gem_string("expr"))};
    gem_table_set(_t1661, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t1662, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1664, 1)), gem_string(")")));
    GemVal _t1665 = gem_v_rc_r;
    gem_table_set(_t1661, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1665, gem_string("setup"))));
                        GemVal _t1666 = _t1661;
                        gem_pop_frame();
                        return _t1666;
                    } else {
#line 819 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 820 "compiler/main.gem"
    GemVal _t1667 = gem_v_node;
    GemVal _t1668 = (*gem_v_compile_expr);
    GemVal _t1669[] = {gem_table_get(_t1667, gem_string("right"))};
                            GemVal gem_v_rc_r = _t1668.fn(_t1668.env, _t1669, 1);
#line 821 "compiler/main.gem"
    GemVal _t1670 = (*gem_v_tmp);
                            GemVal gem_v_t = _t1670.fn(_t1670.env, NULL, 0);
#line 822 "compiler/main.gem"
    GemVal _t1671 = gem_v_rc_r;
    GemVal _t1672[] = {gem_v_t};
    GemVal _t1673 = gem_v_rc_r;
    GemVal _t1674[] = {gem_table_get(_t1673, gem_string("expr"))};
    GemVal _t1675[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t1671, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1672, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1674, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1675, 1)), gem_string("};\n")));
#line 823 "compiler/main.gem"
    GemVal _t1676 = gem_table_new();
    GemVal _t1677[] = {gem_v_t};
    gem_table_set(_t1676, gem_string("expr"), gem_add(gem_add(gem_string("gem_has_key_fn(NULL, "), gem_to_string_fn(NULL, _t1677, 1)), gem_string(", 2)")));
    gem_table_set(_t1676, gem_string("setup"), gem_v_setup);
                            GemVal _t1678 = _t1676;
                            gem_pop_frame();
                            return _t1678;
                        } else {
#line 824 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 825 "compiler/main.gem"
    GemVal _t1679 = gem_v_node;
    GemVal _t1680 = (*gem_v_compile_expr);
    GemVal _t1681[] = {gem_table_get(_t1679, gem_string("right"))};
                                GemVal gem_v_rc_r = _t1680.fn(_t1680.env, _t1681, 1);
#line 826 "compiler/main.gem"
    GemVal _t1682 = gem_table_new();
    GemVal _t1683[] = {gem_v_lc};
    GemVal _t1684 = gem_v_rc_r;
    GemVal _t1685[] = {gem_table_get(_t1684, gem_string("expr"))};
    gem_table_set(_t1682, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t1683, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1685, 1)), gem_string(")")));
    GemVal _t1686 = gem_v_rc_r;
    gem_table_set(_t1682, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1686, gem_string("setup"))));
                                GemVal _t1687 = _t1682;
                                gem_pop_frame();
                                return _t1687;
                            } else {
#line 827 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 828 "compiler/main.gem"
    GemVal _t1688 = gem_v_node;
    GemVal _t1689 = (*gem_v_compile_expr);
    GemVal _t1690[] = {gem_table_get(_t1688, gem_string("right"))};
                                    GemVal gem_v_rc_r = _t1689.fn(_t1689.env, _t1690, 1);
#line 829 "compiler/main.gem"
    GemVal _t1691 = gem_table_new();
    GemVal _t1692[] = {gem_v_lc};
    GemVal _t1693 = gem_v_rc_r;
    GemVal _t1694[] = {gem_table_get(_t1693, gem_string("expr"))};
    gem_table_set(_t1691, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t1692, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1694, 1)), gem_string(")")));
    GemVal _t1695 = gem_v_rc_r;
    gem_table_set(_t1691, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1695, gem_string("setup"))));
                                    GemVal _t1696 = _t1691;
                                    gem_pop_frame();
                                    return _t1696;
                                } else {
#line 830 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 831 "compiler/main.gem"
    GemVal _t1697 = gem_v_node;
    GemVal _t1698 = (*gem_v_compile_expr);
    GemVal _t1699[] = {gem_table_get(_t1697, gem_string("right"))};
                                        GemVal gem_v_rc_r = _t1698.fn(_t1698.env, _t1699, 1);
#line 832 "compiler/main.gem"
    GemVal _t1700 = gem_table_new();
    GemVal _t1701[] = {gem_v_lc};
    GemVal _t1702 = gem_v_rc_r;
    GemVal _t1703[] = {gem_table_get(_t1702, gem_string("expr"))};
    gem_table_set(_t1700, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t1701, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1703, 1)), gem_string(")")));
    GemVal _t1704 = gem_v_rc_r;
    gem_table_set(_t1700, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1704, gem_string("setup"))));
                                        GemVal _t1705 = _t1700;
                                        gem_pop_frame();
                                        return _t1705;
                                    } else {
#line 833 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 834 "compiler/main.gem"
    GemVal _t1706 = gem_v_node;
    GemVal _t1707 = (*gem_v_compile_expr);
    GemVal _t1708[] = {gem_table_get(_t1706, gem_string("right"))};
                                            GemVal gem_v_rc_r = _t1707.fn(_t1707.env, _t1708, 1);
#line 835 "compiler/main.gem"
    GemVal _t1709 = gem_table_new();
    GemVal _t1710[] = {gem_v_lc};
    GemVal _t1711 = gem_v_rc_r;
    GemVal _t1712[] = {gem_table_get(_t1711, gem_string("expr"))};
    gem_table_set(_t1709, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t1710, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1712, 1)), gem_string(")")));
    GemVal _t1713 = gem_v_rc_r;
    gem_table_set(_t1709, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1713, gem_string("setup"))));
                                            GemVal _t1714 = _t1709;
                                            gem_pop_frame();
                                            return _t1714;
                                        } else {
#line 836 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 837 "compiler/main.gem"
    GemVal _t1715 = gem_v_node;
    GemVal _t1716 = (*gem_v_compile_expr);
    GemVal _t1717[] = {gem_table_get(_t1715, gem_string("right"))};
                                                GemVal gem_v_rc_r = _t1716.fn(_t1716.env, _t1717, 1);
#line 838 "compiler/main.gem"
    GemVal _t1718 = gem_table_new();
    GemVal _t1719[] = {gem_v_lc};
    GemVal _t1720 = gem_v_rc_r;
    GemVal _t1721[] = {gem_table_get(_t1720, gem_string("expr"))};
    gem_table_set(_t1718, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t1719, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1721, 1)), gem_string(")")));
    GemVal _t1722 = gem_v_rc_r;
    gem_table_set(_t1718, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1722, gem_string("setup"))));
                                                GemVal _t1723 = _t1718;
                                                gem_pop_frame();
                                                return _t1723;
                                            } else {
#line 839 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 840 "compiler/main.gem"
    GemVal _t1724 = gem_v_node;
    GemVal _t1725 = (*gem_v_compile_expr);
    GemVal _t1726[] = {gem_table_get(_t1724, gem_string("right"))};
                                                    GemVal gem_v_rc_r = _t1725.fn(_t1725.env, _t1726, 1);
#line 841 "compiler/main.gem"
    GemVal _t1727 = gem_table_new();
    GemVal _t1728[] = {gem_v_lc};
    GemVal _t1729 = gem_v_rc_r;
    GemVal _t1730[] = {gem_table_get(_t1729, gem_string("expr"))};
    gem_table_set(_t1727, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t1728, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1730, 1)), gem_string(")")));
    GemVal _t1731 = gem_v_rc_r;
    gem_table_set(_t1727, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1731, gem_string("setup"))));
                                                    GemVal _t1732 = _t1727;
                                                    gem_pop_frame();
                                                    return _t1732;
                                                } else {
#line 842 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 843 "compiler/main.gem"
    GemVal _t1733 = gem_v_node;
    GemVal _t1734 = (*gem_v_compile_expr);
    GemVal _t1735[] = {gem_table_get(_t1733, gem_string("right"))};
                                                        GemVal gem_v_rc_r = _t1734.fn(_t1734.env, _t1735, 1);
#line 844 "compiler/main.gem"
    GemVal _t1736 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1736.fn(_t1736.env, NULL, 0);
#line 845 "compiler/main.gem"
                                                        GemVal gem_v_setup = gem_v_ls;
#line 846 "compiler/main.gem"
    GemVal _t1737[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1737, 1)), gem_string(";\n")));
#line 847 "compiler/main.gem"
    GemVal _t1738[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t1738, 1)), gem_string(")) {\n")));
#line 848 "compiler/main.gem"
    GemVal _t1739[] = {gem_v_t};
    GemVal _t1740[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1739, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1740, 1)), gem_string(";\n")));
#line 849 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 850 "compiler/main.gem"
    GemVal _t1741 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t1741, gem_string("setup")), gem_string("")))) {
#line 851 "compiler/main.gem"
    GemVal _t1742 = gem_v_rc_r;
    GemVal _t1743[] = {gem_table_get(_t1742, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t1743, 3));
                                                        }
#line 853 "compiler/main.gem"
    GemVal _t1744[] = {gem_v_t};
    GemVal _t1745 = gem_v_rc_r;
    GemVal _t1746[] = {gem_table_get(_t1745, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1744, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1746, 1)), gem_string(";\n")));
#line 854 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 855 "compiler/main.gem"
    GemVal _t1747 = gem_table_new();
    gem_table_set(_t1747, gem_string("expr"), gem_v_t);
    gem_table_set(_t1747, gem_string("setup"), gem_v_setup);
                                                        GemVal _t1748 = _t1747;
                                                        gem_pop_frame();
                                                        return _t1748;
                                                    } else {
#line 856 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 857 "compiler/main.gem"
    GemVal _t1749 = gem_v_node;
    GemVal _t1750 = (*gem_v_compile_expr);
    GemVal _t1751[] = {gem_table_get(_t1749, gem_string("right"))};
                                                            GemVal gem_v_rc_r = _t1750.fn(_t1750.env, _t1751, 1);
#line 858 "compiler/main.gem"
    GemVal _t1752 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1752.fn(_t1752.env, NULL, 0);
#line 859 "compiler/main.gem"
                                                            GemVal gem_v_setup = gem_v_ls;
#line 860 "compiler/main.gem"
    GemVal _t1753[] = {gem_v_t};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1753, 1)), gem_string(";\n")));
#line 861 "compiler/main.gem"
    GemVal _t1754[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t1754, 1)), gem_string(")) {\n")));
#line 862 "compiler/main.gem"
    GemVal _t1755[] = {gem_v_t};
    GemVal _t1756[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1755, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1756, 1)), gem_string(";\n")));
#line 863 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 864 "compiler/main.gem"
    GemVal _t1757 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t1757, gem_string("setup")), gem_string("")))) {
#line 865 "compiler/main.gem"
    GemVal _t1758 = gem_v_rc_r;
    GemVal _t1759[] = {gem_table_get(_t1758, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                                gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t1759, 3));
                                                            }
#line 867 "compiler/main.gem"
    GemVal _t1760[] = {gem_v_t};
    GemVal _t1761 = gem_v_rc_r;
    GemVal _t1762[] = {gem_table_get(_t1761, gem_string("expr"))};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1760, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1762, 1)), gem_string(";\n")));
#line 868 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 869 "compiler/main.gem"
    GemVal _t1763 = gem_table_new();
    gem_table_set(_t1763, gem_string("expr"), gem_v_t);
    gem_table_set(_t1763, gem_string("setup"), gem_v_setup);
                                                            GemVal _t1764 = _t1763;
                                                            gem_pop_frame();
                                                            return _t1764;
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
#line 872 "compiler/main.gem"
    GemVal _t1765 = gem_v_node;
    GemVal _t1766 = (*gem_v_compile_expr);
    GemVal _t1767[] = {gem_table_get(_t1765, gem_string("right"))};
    GemVal gem_v_rc_r = _t1766.fn(_t1766.env, _t1767, 1);
    GemVal _t1768 = gem_table_new();
    gem_table_set(_t1768, gem_string("expr"), gem_string("GEM_NIL"));
    GemVal _t1769 = gem_v_rc_r;
    gem_table_set(_t1768, gem_string("setup"), gem_add(gem_add(gem_add(gem_add(gem_v_ls, gem_table_get(_t1769, gem_string("setup"))), gem_string("/* unknown op: ")), gem_v_op), gem_string(" */\n")));
    GemVal _t1770 = _t1768;
    gem_pop_frame();
    return _t1770;
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
#line 879 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
    {
#line 880 "compiler/main.gem"
        GemVal gem_v__for_items_32 = gem_v_stmts;
#line 880 "compiler/main.gem"
        GemVal gem_v__for_i_32 = gem_int(0);
#line 880 "compiler/main.gem"
        while (1) {
            GemVal _t1772[] = {gem_v__for_items_32};
            if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_len_fn(NULL, _t1772, 1)))) break;
#line 880 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_32, gem_v__for_i_32);
#line 880 "compiler/main.gem"
            gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 881 "compiler/main.gem"
    GemVal _t1773 = (*gem_v_compile_stmt);
    GemVal _t1774[] = {gem_v_stmt, gem_v_indent};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1773.fn(_t1773.env, _t1774, 2)), gem_string("\n"));
        }
    }
    GemVal _t1775 = gem_v_out;
    gem_pop_frame();
    return _t1775;
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
#line 889 "compiler/main.gem"
    GemVal _t1777[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1777, 1);
#line 890 "compiler/main.gem"
    GemVal _t1778 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1778, gem_string("tag"));
#line 891 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 892 "compiler/main.gem"
    GemVal _t1779 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1779, gem_string("line")), GEM_NIL))) {
#line 893 "compiler/main.gem"
    GemVal _t1780 = gem_v_node;
    GemVal _t1781[] = {gem_table_get(_t1780, gem_string("line"))};
    GemVal _t1782[] = {(*gem_v_source_name)};
    GemVal _t1783[] = {gem_fn_escape_c_string(NULL, _t1782, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t1781, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t1783, 1)), gem_string("\"\n"));
    }
#line 896 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 897 "compiler/main.gem"
    GemVal _t1784 = gem_v_node;
    GemVal _t1785 = (*gem_v_compile_expr);
    GemVal _t1786[] = {gem_table_get(_t1784, gem_string("value"))};
        GemVal gem_v_r = _t1785.fn(_t1785.env, _t1786, 1);
#line 898 "compiler/main.gem"
    GemVal _t1787 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1787, gem_string("name"));
#line 899 "compiler/main.gem"
    GemVal _t1788 = (*gem_v_mangle);
    GemVal _t1789[] = {gem_v_name};
        GemVal gem_v_mname = _t1788.fn(_t1788.env, _t1789, 1);
#line 900 "compiler/main.gem"
    GemVal _t1790[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1790, 2))) {
#line 901 "compiler/main.gem"
    GemVal _t1791[] = {gem_v_line_dir};
    GemVal _t1792 = gem_v_r;
    GemVal _t1793[] = {gem_table_get(_t1792, gem_string("setup"))};
    GemVal _t1794[] = {gem_v_p};
    GemVal _t1795[] = {gem_v_mname};
    GemVal _t1796[] = {gem_v_p};
    GemVal _t1797[] = {gem_v_mname};
    GemVal _t1798 = gem_v_r;
    GemVal _t1799[] = {gem_table_get(_t1798, gem_string("expr"))};
            GemVal _t1800 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1791, 1), gem_to_string_fn(NULL, _t1793, 1)), gem_to_string_fn(NULL, _t1794, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t1795, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1796, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1797, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1799, 1)), gem_string(";"));
            gem_pop_frame();
            return _t1800;
        }
#line 903 "compiler/main.gem"
    GemVal _t1801[] = {gem_v_line_dir};
    GemVal _t1802 = gem_v_r;
    GemVal _t1803[] = {gem_table_get(_t1802, gem_string("setup"))};
    GemVal _t1804[] = {gem_v_p};
    GemVal _t1805[] = {gem_v_mname};
    GemVal _t1806 = gem_v_r;
    GemVal _t1807[] = {gem_table_get(_t1806, gem_string("expr"))};
        GemVal _t1808 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1801, 1), gem_to_string_fn(NULL, _t1803, 1)), gem_to_string_fn(NULL, _t1804, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1805, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1807, 1)), gem_string(";"));
        gem_pop_frame();
        return _t1808;
    } else {
#line 904 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 905 "compiler/main.gem"
    GemVal _t1809 = gem_v_node;
    GemVal _t1810 = (*gem_v_compile_expr);
    GemVal _t1811[] = {gem_table_get(_t1809, gem_string("value"))};
            GemVal gem_v_r = _t1810.fn(_t1810.env, _t1811, 1);
#line 906 "compiler/main.gem"
    GemVal _t1812 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1812, gem_string("name"));
#line 907 "compiler/main.gem"
    GemVal _t1813 = (*gem_v_mangle);
    GemVal _t1814[] = {gem_v_name};
            GemVal gem_v_mname = _t1813.fn(_t1813.env, _t1814, 1);
#line 908 "compiler/main.gem"
    GemVal _t1815[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1815, 2))) {
#line 909 "compiler/main.gem"
    GemVal _t1816[] = {gem_v_line_dir};
    GemVal _t1817 = gem_v_r;
    GemVal _t1818[] = {gem_table_get(_t1817, gem_string("setup"))};
    GemVal _t1819[] = {gem_v_p};
    GemVal _t1820[] = {gem_v_mname};
    GemVal _t1821 = gem_v_r;
    GemVal _t1822[] = {gem_table_get(_t1821, gem_string("expr"))};
                GemVal _t1823 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1816, 1), gem_to_string_fn(NULL, _t1818, 1)), gem_to_string_fn(NULL, _t1819, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1820, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1822, 1)), gem_string(";"));
                gem_pop_frame();
                return _t1823;
            }
#line 911 "compiler/main.gem"
    GemVal _t1824[] = {gem_v_line_dir};
    GemVal _t1825 = gem_v_r;
    GemVal _t1826[] = {gem_table_get(_t1825, gem_string("setup"))};
    GemVal _t1827[] = {gem_v_p};
    GemVal _t1828[] = {gem_v_mname};
    GemVal _t1829 = gem_v_r;
    GemVal _t1830[] = {gem_table_get(_t1829, gem_string("expr"))};
            GemVal _t1831 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1824, 1), gem_to_string_fn(NULL, _t1826, 1)), gem_to_string_fn(NULL, _t1827, 1)), gem_to_string_fn(NULL, _t1828, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1830, 1)), gem_string(";"));
            gem_pop_frame();
            return _t1831;
        } else {
#line 912 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
#line 913 "compiler/main.gem"
    GemVal _t1832 = gem_v_node;
    GemVal _t1833 = (*gem_v_compile_expr);
    GemVal _t1834[] = {gem_table_get(_t1832, gem_string("object"))};
                GemVal gem_v_obj_r = _t1833.fn(_t1833.env, _t1834, 1);
#line 914 "compiler/main.gem"
    GemVal _t1835 = gem_v_node;
    GemVal _t1836 = (*gem_v_compile_expr);
    GemVal _t1837[] = {gem_table_get(_t1835, gem_string("value"))};
                GemVal gem_v_val_r = _t1836.fn(_t1836.env, _t1837, 1);
#line 915 "compiler/main.gem"
    GemVal _t1838 = gem_v_node;
    GemVal _t1839[] = {gem_table_get(_t1838, gem_string("field"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1839, 1);
#line 916 "compiler/main.gem"
    GemVal _t1840[] = {gem_v_line_dir};
    GemVal _t1841 = gem_v_obj_r;
    GemVal _t1842[] = {gem_table_get(_t1841, gem_string("setup"))};
    GemVal _t1843 = gem_v_val_r;
    GemVal _t1844[] = {gem_table_get(_t1843, gem_string("setup"))};
    GemVal _t1845[] = {gem_v_p};
    GemVal _t1846 = gem_v_obj_r;
    GemVal _t1847[] = {gem_table_get(_t1846, gem_string("expr"))};
    GemVal _t1848[] = {gem_v_escaped};
    GemVal _t1849 = gem_v_val_r;
    GemVal _t1850[] = {gem_table_get(_t1849, gem_string("expr"))};
                GemVal _t1851 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1840, 1), gem_to_string_fn(NULL, _t1842, 1)), gem_to_string_fn(NULL, _t1844, 1)), gem_to_string_fn(NULL, _t1845, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t1847, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1848, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1850, 1)), gem_string(");"));
                gem_pop_frame();
                return _t1851;
            } else {
#line 917 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("index_assign")))) {
#line 918 "compiler/main.gem"
    GemVal _t1852 = gem_v_node;
    GemVal _t1853 = (*gem_v_compile_expr);
    GemVal _t1854[] = {gem_table_get(_t1852, gem_string("object"))};
                    GemVal gem_v_obj_r = _t1853.fn(_t1853.env, _t1854, 1);
#line 919 "compiler/main.gem"
    GemVal _t1855 = gem_v_node;
    GemVal _t1856 = (*gem_v_compile_expr);
    GemVal _t1857[] = {gem_table_get(_t1855, gem_string("key"))};
                    GemVal gem_v_key_r = _t1856.fn(_t1856.env, _t1857, 1);
#line 920 "compiler/main.gem"
    GemVal _t1858 = gem_v_node;
    GemVal _t1859 = (*gem_v_compile_expr);
    GemVal _t1860[] = {gem_table_get(_t1858, gem_string("value"))};
                    GemVal gem_v_val_r = _t1859.fn(_t1859.env, _t1860, 1);
#line 921 "compiler/main.gem"
    GemVal _t1861[] = {gem_v_line_dir};
    GemVal _t1862 = gem_v_obj_r;
    GemVal _t1863[] = {gem_table_get(_t1862, gem_string("setup"))};
    GemVal _t1864 = gem_v_key_r;
    GemVal _t1865[] = {gem_table_get(_t1864, gem_string("setup"))};
    GemVal _t1866 = gem_v_val_r;
    GemVal _t1867[] = {gem_table_get(_t1866, gem_string("setup"))};
    GemVal _t1868[] = {gem_v_p};
    GemVal _t1869 = gem_v_obj_r;
    GemVal _t1870[] = {gem_table_get(_t1869, gem_string("expr"))};
    GemVal _t1871 = gem_v_key_r;
    GemVal _t1872[] = {gem_table_get(_t1871, gem_string("expr"))};
    GemVal _t1873 = gem_v_val_r;
    GemVal _t1874[] = {gem_table_get(_t1873, gem_string("expr"))};
                    GemVal _t1875 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1861, 1), gem_to_string_fn(NULL, _t1863, 1)), gem_to_string_fn(NULL, _t1865, 1)), gem_to_string_fn(NULL, _t1867, 1)), gem_to_string_fn(NULL, _t1868, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t1870, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1872, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1874, 1)), gem_string(");"));
                    gem_pop_frame();
                    return _t1875;
                } else {
#line 922 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 923 "compiler/main.gem"
    GemVal _t1876[] = {gem_v_line_dir};
    GemVal _t1877 = (*gem_v_compile_if);
    GemVal _t1878[] = {gem_v_node, gem_v_indent};
                        GemVal _t1879 = gem_add(gem_to_string_fn(NULL, _t1876, 1), _t1877.fn(_t1877.env, _t1878, 2));
                        gem_pop_frame();
                        return _t1879;
                    } else {
#line 924 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 925 "compiler/main.gem"
    GemVal _t1880[] = {gem_v_line_dir};
    GemVal _t1881 = (*gem_v_compile_while);
    GemVal _t1882[] = {gem_v_node, gem_v_indent};
                            GemVal _t1883 = gem_add(gem_to_string_fn(NULL, _t1880, 1), _t1881.fn(_t1881.env, _t1882, 2));
                            gem_pop_frame();
                            return _t1883;
                        } else {
#line 926 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 927 "compiler/main.gem"
    GemVal _t1884[] = {gem_v_line_dir};
    GemVal _t1885 = (*gem_v_compile_match);
    GemVal _t1886[] = {gem_v_node, gem_v_indent};
                                GemVal _t1887 = gem_add(gem_to_string_fn(NULL, _t1884, 1), _t1885.fn(_t1885.env, _t1886, 2));
                                gem_pop_frame();
                                return _t1887;
                            } else {
#line 928 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("break")))) {
#line 929 "compiler/main.gem"
                                    GemVal _t1888 = gem_add(gem_v_p, gem_string("break;"));
                                    gem_pop_frame();
                                    return _t1888;
                                } else {
#line 930 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("continue")))) {
#line 931 "compiler/main.gem"
                                        GemVal _t1889 = gem_add(gem_v_p, gem_string("continue;"));
                                        gem_pop_frame();
                                        return _t1889;
                                    } else {
#line 932 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 933 "compiler/main.gem"
    GemVal _t1890 = gem_v_node;
                                            if (gem_truthy(gem_neq(gem_table_get(_t1890, gem_string("value")), GEM_NIL))) {
#line 934 "compiler/main.gem"
    GemVal _t1891 = gem_v_node;
    GemVal _t1892 = (*gem_v_compile_expr);
    GemVal _t1893[] = {gem_table_get(_t1891, gem_string("value"))};
                                                GemVal gem_v_r = _t1892.fn(_t1892.env, _t1893, 1);
#line 935 "compiler/main.gem"
    GemVal _t1894 = (*gem_v_tmp);
                                                GemVal gem_v_t = _t1894.fn(_t1894.env, NULL, 0);
#line 936 "compiler/main.gem"
    GemVal _t1895[] = {gem_v_line_dir};
    GemVal _t1896 = gem_v_r;
    GemVal _t1897[] = {gem_table_get(_t1896, gem_string("setup"))};
    GemVal _t1898[] = {gem_v_p};
    GemVal _t1899[] = {gem_v_t};
    GemVal _t1900 = gem_v_r;
    GemVal _t1901[] = {gem_table_get(_t1900, gem_string("expr"))};
    GemVal _t1902[] = {gem_v_p};
    GemVal _t1903[] = {gem_v_p};
    GemVal _t1904[] = {gem_v_t};
                                                GemVal _t1905 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1895, 1), gem_to_string_fn(NULL, _t1897, 1)), gem_to_string_fn(NULL, _t1898, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1899, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1901, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1902, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1903, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1904, 1)), gem_string(";"));
                                                gem_pop_frame();
                                                return _t1905;
                                            }
#line 938 "compiler/main.gem"
    GemVal _t1906[] = {gem_v_line_dir};
    GemVal _t1907[] = {gem_v_p};
    GemVal _t1908[] = {gem_v_p};
                                            GemVal _t1909 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1906, 1), gem_to_string_fn(NULL, _t1907, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1908, 1)), gem_string("return GEM_NIL;"));
                                            gem_pop_frame();
                                            return _t1909;
                                        } else {
#line 939 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 940 "compiler/main.gem"
    GemVal _t1910 = (*gem_v_compile_expr);
    GemVal _t1911[] = {gem_v_node};
                                                GemVal gem_v_r = _t1910.fn(_t1910.env, _t1911, 1);
#line 941 "compiler/main.gem"
    GemVal _t1912[] = {gem_v_line_dir};
    GemVal _t1913 = gem_v_r;
    GemVal _t1914[] = {gem_table_get(_t1913, gem_string("setup"))};
    GemVal _t1915[] = {gem_v_p};
    GemVal _t1916 = gem_v_r;
    GemVal _t1917[] = {gem_table_get(_t1916, gem_string("expr"))};
                                                GemVal _t1918 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1912, 1), gem_to_string_fn(NULL, _t1914, 1)), gem_to_string_fn(NULL, _t1915, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t1917, 1)), gem_string(");"));
                                                gem_pop_frame();
                                                return _t1918;
                                            } else {
#line 942 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
#line 943 "compiler/main.gem"
                                                    GemVal _t1919 = gem_string("");
                                                    gem_pop_frame();
                                                    return _t1919;
                                                } else {
#line 944 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 945 "compiler/main.gem"
    GemVal _t1920[] = {gem_v_p};
                                                        GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t1920, 1), gem_string("{\n"));
                                                        {
#line 946 "compiler/main.gem"
    GemVal _t1921 = gem_v_node;
                                                            GemVal gem_v__for_items_33 = gem_table_get(_t1921, gem_string("stmts"));
#line 946 "compiler/main.gem"
                                                            GemVal gem_v__for_i_33 = gem_int(0);
#line 946 "compiler/main.gem"
                                                            while (1) {
                                                                GemVal _t1922[] = {gem_v__for_items_33};
                                                                if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_len_fn(NULL, _t1922, 1)))) break;
#line 946 "compiler/main.gem"
                                                                GemVal gem_v_stmt = gem_table_get(gem_v__for_items_33, gem_v__for_i_33);
#line 946 "compiler/main.gem"
                                                                gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 947 "compiler/main.gem"
    GemVal _t1923 = (*gem_v_compile_stmt);
    GemVal _t1924[] = {gem_v_stmt, gem_add(gem_v_indent, gem_int(1))};
                                                                gem_v_out = gem_add(gem_add(gem_v_out, _t1923.fn(_t1923.env, _t1924, 2)), gem_string("\n"));
                                                            }
                                                        }
#line 949 "compiler/main.gem"
    GemVal _t1925[] = {gem_v_p};
                                                        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1925, 1), gem_string("}")));
#line 950 "compiler/main.gem"
                                                        GemVal _t1926 = gem_v_out;
                                                        gem_pop_frame();
                                                        return _t1926;
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
#line 953 "compiler/main.gem"
    GemVal _t1927 = (*gem_v_compile_expr);
    GemVal _t1928[] = {gem_v_node};
    GemVal gem_v_r = _t1927.fn(_t1927.env, _t1928, 1);
    GemVal _t1929[] = {gem_v_line_dir};
    GemVal _t1930 = gem_v_r;
    GemVal _t1931[] = {gem_table_get(_t1930, gem_string("setup"))};
    GemVal _t1932[] = {gem_v_p};
    GemVal _t1933 = gem_v_r;
    GemVal _t1934[] = {gem_table_get(_t1933, gem_string("expr"))};
    GemVal _t1935 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1929, 1), gem_to_string_fn(NULL, _t1931, 1)), gem_to_string_fn(NULL, _t1932, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t1934, 1)), gem_string(");"));
    gem_pop_frame();
    return _t1935;
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
#line 960 "compiler/main.gem"
    GemVal _t1937[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1937, 1);
#line 961 "compiler/main.gem"
    GemVal _t1938 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1938, gem_string("tag"));
#line 962 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 963 "compiler/main.gem"
    GemVal _t1939 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1939, gem_string("line")), GEM_NIL))) {
#line 964 "compiler/main.gem"
    GemVal _t1940 = gem_v_node;
    GemVal _t1941[] = {gem_table_get(_t1940, gem_string("line"))};
    GemVal _t1942[] = {(*gem_v_source_name)};
    GemVal _t1943[] = {gem_fn_escape_c_string(NULL, _t1942, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t1941, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t1943, 1)), gem_string("\"\n"));
    }
#line 967 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 968 "compiler/main.gem"
    GemVal _t1944 = gem_v_node;
    GemVal _t1945 = (*gem_v_compile_expr);
    GemVal _t1946[] = {gem_table_get(_t1944, gem_string("value"))};
        GemVal gem_v_r = _t1945.fn(_t1945.env, _t1946, 1);
#line 969 "compiler/main.gem"
    GemVal _t1947 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1947, gem_string("name"));
#line 970 "compiler/main.gem"
    GemVal _t1948 = (*gem_v_mangle);
    GemVal _t1949[] = {gem_v_name};
        GemVal gem_v_mname = _t1948.fn(_t1948.env, _t1949, 1);
#line 971 "compiler/main.gem"
    GemVal _t1950[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1950, 2))) {
#line 972 "compiler/main.gem"
    GemVal _t1951[] = {gem_v_line_dir};
    GemVal _t1952 = gem_v_r;
    GemVal _t1953[] = {gem_table_get(_t1952, gem_string("setup"))};
    GemVal _t1954[] = {gem_v_p};
    GemVal _t1955[] = {gem_v_mname};
    GemVal _t1956[] = {gem_v_p};
    GemVal _t1957[] = {gem_v_mname};
    GemVal _t1958 = gem_v_r;
    GemVal _t1959[] = {gem_table_get(_t1958, gem_string("expr"))};
    GemVal _t1960[] = {gem_v_p};
    GemVal _t1961[] = {gem_v_p};
            GemVal _t1962 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1951, 1), gem_to_string_fn(NULL, _t1953, 1)), gem_to_string_fn(NULL, _t1954, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t1955, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1956, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1957, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1959, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1960, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1961, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t1962;
        }
#line 974 "compiler/main.gem"
    GemVal _t1963[] = {gem_v_line_dir};
    GemVal _t1964 = gem_v_r;
    GemVal _t1965[] = {gem_table_get(_t1964, gem_string("setup"))};
    GemVal _t1966[] = {gem_v_p};
    GemVal _t1967[] = {gem_v_mname};
    GemVal _t1968 = gem_v_r;
    GemVal _t1969[] = {gem_table_get(_t1968, gem_string("expr"))};
    GemVal _t1970[] = {gem_v_p};
    GemVal _t1971[] = {gem_v_p};
        GemVal _t1972 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1963, 1), gem_to_string_fn(NULL, _t1965, 1)), gem_to_string_fn(NULL, _t1966, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1967, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1969, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1970, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1971, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t1972;
    } else {
#line 975 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 976 "compiler/main.gem"
    GemVal _t1973 = gem_v_node;
    GemVal _t1974 = (*gem_v_compile_expr);
    GemVal _t1975[] = {gem_table_get(_t1973, gem_string("value"))};
            GemVal gem_v_r = _t1974.fn(_t1974.env, _t1975, 1);
#line 977 "compiler/main.gem"
    GemVal _t1976 = (*gem_v_tmp);
            GemVal gem_v_t = _t1976.fn(_t1976.env, NULL, 0);
#line 978 "compiler/main.gem"
    GemVal _t1977 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1977, gem_string("name"));
#line 979 "compiler/main.gem"
    GemVal _t1978 = (*gem_v_mangle);
    GemVal _t1979[] = {gem_v_name};
            GemVal gem_v_mname = _t1978.fn(_t1978.env, _t1979, 1);
#line 980 "compiler/main.gem"
    GemVal _t1980[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1980, 2))) {
#line 981 "compiler/main.gem"
    GemVal _t1981[] = {gem_v_line_dir};
    GemVal _t1982 = gem_v_r;
    GemVal _t1983[] = {gem_table_get(_t1982, gem_string("setup"))};
    GemVal _t1984[] = {gem_v_p};
    GemVal _t1985[] = {gem_v_t};
    GemVal _t1986 = gem_v_r;
    GemVal _t1987[] = {gem_table_get(_t1986, gem_string("expr"))};
    GemVal _t1988[] = {gem_v_p};
    GemVal _t1989[] = {gem_v_mname};
    GemVal _t1990[] = {gem_v_t};
    GemVal _t1991[] = {gem_v_p};
    GemVal _t1992[] = {gem_v_p};
    GemVal _t1993[] = {gem_v_t};
                GemVal _t1994 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1981, 1), gem_to_string_fn(NULL, _t1983, 1)), gem_to_string_fn(NULL, _t1984, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1985, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1987, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1988, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1989, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1990, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1991, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1992, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1993, 1)), gem_string(";"));
                gem_pop_frame();
                return _t1994;
            }
#line 983 "compiler/main.gem"
    GemVal _t1995[] = {gem_v_line_dir};
    GemVal _t1996 = gem_v_r;
    GemVal _t1997[] = {gem_table_get(_t1996, gem_string("setup"))};
    GemVal _t1998[] = {gem_v_p};
    GemVal _t1999[] = {gem_v_t};
    GemVal _t2000 = gem_v_r;
    GemVal _t2001[] = {gem_table_get(_t2000, gem_string("expr"))};
    GemVal _t2002[] = {gem_v_p};
    GemVal _t2003[] = {gem_v_mname};
    GemVal _t2004[] = {gem_v_t};
    GemVal _t2005[] = {gem_v_p};
    GemVal _t2006[] = {gem_v_p};
    GemVal _t2007[] = {gem_v_t};
            GemVal _t2008 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1995, 1), gem_to_string_fn(NULL, _t1997, 1)), gem_to_string_fn(NULL, _t1998, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1999, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2001, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2002, 1)), gem_to_string_fn(NULL, _t2003, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2004, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2005, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2006, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2007, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2008;
        } else {
#line 984 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 985 "compiler/main.gem"
    GemVal _t2009 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t2009, gem_string("value")), GEM_NIL))) {
#line 986 "compiler/main.gem"
    GemVal _t2010 = gem_v_node;
    GemVal _t2011 = (*gem_v_compile_expr);
    GemVal _t2012[] = {gem_table_get(_t2010, gem_string("value"))};
                    GemVal gem_v_r = _t2011.fn(_t2011.env, _t2012, 1);
#line 987 "compiler/main.gem"
    GemVal _t2013 = (*gem_v_tmp);
                    GemVal gem_v_t = _t2013.fn(_t2013.env, NULL, 0);
#line 988 "compiler/main.gem"
    GemVal _t2014[] = {gem_v_line_dir};
    GemVal _t2015 = gem_v_r;
    GemVal _t2016[] = {gem_table_get(_t2015, gem_string("setup"))};
    GemVal _t2017[] = {gem_v_p};
    GemVal _t2018[] = {gem_v_t};
    GemVal _t2019 = gem_v_r;
    GemVal _t2020[] = {gem_table_get(_t2019, gem_string("expr"))};
    GemVal _t2021[] = {gem_v_p};
    GemVal _t2022[] = {gem_v_p};
    GemVal _t2023[] = {gem_v_t};
                    GemVal _t2024 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2014, 1), gem_to_string_fn(NULL, _t2016, 1)), gem_to_string_fn(NULL, _t2017, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2018, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2020, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2021, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2022, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2023, 1)), gem_string(";"));
                    gem_pop_frame();
                    return _t2024;
                }
#line 990 "compiler/main.gem"
    GemVal _t2025[] = {gem_v_line_dir};
    GemVal _t2026[] = {gem_v_p};
    GemVal _t2027[] = {gem_v_p};
                GemVal _t2028 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2025, 1), gem_to_string_fn(NULL, _t2026, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2027, 1)), gem_string("return GEM_NIL;"));
                gem_pop_frame();
                return _t2028;
            } else {
#line 991 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 992 "compiler/main.gem"
    GemVal _t2029[] = {gem_v_line_dir};
    GemVal _t2030 = (*gem_v_compile_if_return);
    GemVal _t2031[] = {gem_v_node, gem_v_indent};
                    GemVal _t2032 = gem_add(gem_to_string_fn(NULL, _t2029, 1), _t2030.fn(_t2030.env, _t2031, 2));
                    gem_pop_frame();
                    return _t2032;
                } else {
#line 993 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 994 "compiler/main.gem"
    GemVal _t2033[] = {gem_v_line_dir};
    GemVal _t2034 = (*gem_v_compile_while);
    GemVal _t2035[] = {gem_v_node, gem_v_indent};
    GemVal _t2036[] = {_t2034.fn(_t2034.env, _t2035, 2)};
    GemVal _t2037[] = {gem_v_p};
    GemVal _t2038[] = {gem_v_p};
                        GemVal _t2039 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2033, 1), gem_to_string_fn(NULL, _t2036, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t2037, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2038, 1)), gem_string("return GEM_NIL;"));
                        gem_pop_frame();
                        return _t2039;
                    } else {
#line 995 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 996 "compiler/main.gem"
    GemVal _t2040[] = {gem_v_line_dir};
    GemVal _t2041 = (*gem_v_compile_match_return);
    GemVal _t2042[] = {gem_v_node, gem_v_indent};
                            GemVal _t2043 = gem_add(gem_to_string_fn(NULL, _t2040, 1), _t2041.fn(_t2041.env, _t2042, 2));
                            gem_pop_frame();
                            return _t2043;
                        } else {
#line 997 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 998 "compiler/main.gem"
    GemVal _t2044[] = {gem_v_p};
                                GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2044, 1), gem_string("{\n"));
                                {
#line 999 "compiler/main.gem"
                                    GemVal gem_v__for_i_34 = gem_int(0);
#line 999 "compiler/main.gem"
    GemVal _t2045 = gem_v_node;
    GemVal _t2046[] = {gem_table_get(_t2045, gem_string("stmts"))};
                                    GemVal gem_v__for_limit_34 = gem_sub(gem_len_fn(NULL, _t2046, 1), gem_int(1));
#line 999 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
#line 999 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_34;
#line 999 "compiler/main.gem"
                                        gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 1000 "compiler/main.gem"
    GemVal _t2047 = gem_v_node;
    GemVal _t2048 = (*gem_v_compile_stmt);
    GemVal _t2049[] = {gem_table_get(gem_table_get(_t2047, gem_string("stmts")), gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                                        gem_v_out = gem_add(gem_add(gem_v_out, _t2048.fn(_t2048.env, _t2049, 2)), gem_string("\n"));
                                    }
                                }
#line 1002 "compiler/main.gem"
    GemVal _t2050 = gem_v_node;
    GemVal _t2051[] = {gem_table_get(_t2050, gem_string("stmts"))};
                                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2051, 1), gem_int(0)))) {
#line 1003 "compiler/main.gem"
    GemVal _t2052 = gem_v_node;
    GemVal _t2053 = gem_v_node;
    GemVal _t2054[] = {gem_table_get(_t2053, gem_string("stmts"))};
    GemVal _t2055 = (*gem_v_compile_stmt_return);
    GemVal _t2056[] = {gem_table_get(gem_table_get(_t2052, gem_string("stmts")), gem_sub(gem_len_fn(NULL, _t2054, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t2055.fn(_t2055.env, _t2056, 2)), gem_string("\n"));
                                }
#line 1005 "compiler/main.gem"
    GemVal _t2057[] = {gem_v_p};
                                gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2057, 1), gem_string("}")));
#line 1006 "compiler/main.gem"
                                GemVal _t2058 = gem_v_out;
                                gem_pop_frame();
                                return _t2058;
                            } else {
#line 1007 "compiler/main.gem"
    GemVal _t2059;
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
        _t2059 = gem_eq(gem_v_tag, gem_string("dot_assign"));
    } else {
        _t2059 = gem_eq(gem_v_tag, gem_string("index_assign"));
    }
    GemVal _t2060;
    if (gem_truthy(_t2059)) {
        _t2060 = _t2059;
    } else {
        _t2060 = gem_eq(gem_v_tag, gem_string("break"));
    }
    GemVal _t2061;
    if (gem_truthy(_t2060)) {
        _t2061 = _t2060;
    } else {
        _t2061 = gem_eq(gem_v_tag, gem_string("continue"));
    }
    GemVal _t2062;
    if (gem_truthy(_t2061)) {
        _t2062 = _t2061;
    } else {
        _t2062 = gem_eq(gem_v_tag, gem_string("fn_def"));
    }
                                if (gem_truthy(_t2062)) {
#line 1008 "compiler/main.gem"
    GemVal _t2063 = (*gem_v_compile_stmt);
    GemVal _t2064[] = {gem_v_node, gem_v_indent};
    GemVal _t2065[] = {_t2063.fn(_t2063.env, _t2064, 2)};
    GemVal _t2066[] = {gem_v_p};
    GemVal _t2067[] = {gem_v_p};
                                    GemVal _t2068 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2065, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2066, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2067, 1)), gem_string("return GEM_NIL;"));
                                    gem_pop_frame();
                                    return _t2068;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 1011 "compiler/main.gem"
    GemVal _t2069 = (*gem_v_compile_expr);
    GemVal _t2070[] = {gem_v_node};
    GemVal gem_v_r = _t2069.fn(_t2069.env, _t2070, 1);
#line 1012 "compiler/main.gem"
    GemVal _t2071 = (*gem_v_tmp);
    GemVal gem_v_t = _t2071.fn(_t2071.env, NULL, 0);
    GemVal _t2072[] = {gem_v_line_dir};
    GemVal _t2073 = gem_v_r;
    GemVal _t2074[] = {gem_table_get(_t2073, gem_string("setup"))};
    GemVal _t2075[] = {gem_v_p};
    GemVal _t2076[] = {gem_v_t};
    GemVal _t2077 = gem_v_r;
    GemVal _t2078[] = {gem_table_get(_t2077, gem_string("expr"))};
    GemVal _t2079[] = {gem_v_p};
    GemVal _t2080[] = {gem_v_p};
    GemVal _t2081[] = {gem_v_t};
    GemVal _t2082 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2072, 1), gem_to_string_fn(NULL, _t2074, 1)), gem_to_string_fn(NULL, _t2075, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2076, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2078, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2079, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2080, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2081, 1)), gem_string(";"));
    gem_pop_frame();
    return _t2082;
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
#line 1019 "compiler/main.gem"
    GemVal _t2084[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2084, 1);
#line 1020 "compiler/main.gem"
    GemVal _t2085 = gem_v_node;
    GemVal _t2086 = (*gem_v_compile_expr);
    GemVal _t2087[] = {gem_table_get(_t2085, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2086.fn(_t2086.env, _t2087, 1);
#line 1021 "compiler/main.gem"
    GemVal _t2088 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2088, gem_string("setup"));
#line 1022 "compiler/main.gem"
    GemVal _t2089[] = {gem_v_p};
    GemVal _t2090 = gem_v_cond_r;
    GemVal _t2091[] = {gem_table_get(_t2090, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2089, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2091, 1)), gem_string(")) {\n")));
#line 1023 "compiler/main.gem"
    GemVal _t2092 = gem_v_node;
    GemVal _t2093 = (*gem_v_compile_stmts);
    GemVal _t2094[] = {gem_table_get(_t2092, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t2093.fn(_t2093.env, _t2094, 2));
#line 1024 "compiler/main.gem"
    GemVal _t2095 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2095, gem_string("else")), GEM_NIL))) {
#line 1025 "compiler/main.gem"
    GemVal _t2096[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2096, 1), gem_string("} else {\n")));
#line 1026 "compiler/main.gem"
    GemVal _t2097 = gem_v_node;
    GemVal _t2098 = (*gem_v_compile_stmts);
    GemVal _t2099[] = {gem_table_get(_t2097, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t2098.fn(_t2098.env, _t2099, 2));
    }
#line 1028 "compiler/main.gem"
    GemVal _t2100[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2100, 1), gem_string("}")));
    GemVal _t2101 = gem_v_out;
    gem_pop_frame();
    return _t2101;
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
#line 1035 "compiler/main.gem"
    GemVal _t2103[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2103, 1);
#line 1036 "compiler/main.gem"
    GemVal _t2104 = gem_v_node;
    GemVal _t2105 = (*gem_v_compile_expr);
    GemVal _t2106[] = {gem_table_get(_t2104, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2105.fn(_t2105.env, _t2106, 1);
#line 1037 "compiler/main.gem"
    GemVal _t2107 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2107, gem_string("setup"));
#line 1038 "compiler/main.gem"
    GemVal _t2108[] = {gem_v_p};
    GemVal _t2109 = gem_v_cond_r;
    GemVal _t2110[] = {gem_table_get(_t2109, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2108, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2110, 1)), gem_string(")) {\n")));
#line 1039 "compiler/main.gem"
    GemVal _t2111 = gem_v_node;
    GemVal gem_v_then_body = gem_table_get(_t2111, gem_string("then"));
#line 1040 "compiler/main.gem"
    GemVal _t2112[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2112, 1), gem_int(0)))) {
        {
#line 1041 "compiler/main.gem"
            GemVal gem_v__for_i_35 = gem_int(0);
#line 1041 "compiler/main.gem"
    GemVal _t2113[] = {gem_v_then_body};
            GemVal gem_v__for_limit_35 = gem_sub(gem_len_fn(NULL, _t2113, 1), gem_int(1));
#line 1041 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 1041 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_35;
#line 1041 "compiler/main.gem"
                gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 1042 "compiler/main.gem"
    GemVal _t2114 = (*gem_v_compile_stmt);
    GemVal _t2115[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t2114.fn(_t2114.env, _t2115, 2)), gem_string("\n"));
            }
        }
#line 1044 "compiler/main.gem"
    GemVal _t2116[] = {gem_v_then_body};
    GemVal _t2117 = (*gem_v_compile_stmt_return);
    GemVal _t2118[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t2116, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_add(gem_v_out, _t2117.fn(_t2117.env, _t2118, 2)), gem_string("\n"));
    } else {
#line 1046 "compiler/main.gem"
    GemVal _t2119[] = {gem_v_p};
    GemVal _t2120[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2119, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2120, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1048 "compiler/main.gem"
    GemVal _t2121 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2121, gem_string("else")), GEM_NIL))) {
#line 1049 "compiler/main.gem"
    GemVal _t2122[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2122, 1), gem_string("} else {\n")));
#line 1050 "compiler/main.gem"
    GemVal _t2123 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t2123, gem_string("else"));
#line 1051 "compiler/main.gem"
    GemVal _t2124[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2124, 1), gem_int(0)))) {
            {
#line 1052 "compiler/main.gem"
                GemVal gem_v__for_i_36 = gem_int(0);
#line 1052 "compiler/main.gem"
    GemVal _t2125[] = {gem_v_else_body};
                GemVal gem_v__for_limit_36 = gem_sub(gem_len_fn(NULL, _t2125, 1), gem_int(1));
#line 1052 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 1052 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_36;
#line 1052 "compiler/main.gem"
                    gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 1053 "compiler/main.gem"
    GemVal _t2126 = (*gem_v_compile_stmt);
    GemVal _t2127[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t2126.fn(_t2126.env, _t2127, 2)), gem_string("\n"));
                }
            }
#line 1055 "compiler/main.gem"
    GemVal _t2128[] = {gem_v_else_body};
    GemVal _t2129 = (*gem_v_compile_stmt_return);
    GemVal _t2130[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2128, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t2129.fn(_t2129.env, _t2130, 2)), gem_string("\n"));
        } else {
#line 1057 "compiler/main.gem"
    GemVal _t2131[] = {gem_v_p};
    GemVal _t2132[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2131, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2132, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1060 "compiler/main.gem"
    GemVal _t2133[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2133, 1), gem_string("} else {\n")));
#line 1061 "compiler/main.gem"
    GemVal _t2134[] = {gem_v_p};
    GemVal _t2135[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2134, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2135, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1063 "compiler/main.gem"
    GemVal _t2136[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2136, 1), gem_string("}")));
    GemVal _t2137 = gem_v_out;
    gem_pop_frame();
    return _t2137;
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
#line 1070 "compiler/main.gem"
    GemVal _t2139[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2139, 1);
#line 1071 "compiler/main.gem"
    GemVal _t2140 = gem_v_node;
    GemVal _t2141 = (*gem_v_compile_expr);
    GemVal _t2142[] = {gem_table_get(_t2140, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2141.fn(_t2141.env, _t2142, 1);
#line 1072 "compiler/main.gem"
    GemVal _t2143[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2143, 1), gem_string("while (1) {\n"));
#line 1073 "compiler/main.gem"
    GemVal _t2144 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t2144, gem_string("setup")), gem_string("")))) {
#line 1074 "compiler/main.gem"
    GemVal _t2145 = gem_v_cond_r;
    GemVal _t2146[] = {gem_v_p};
    GemVal _t2147[] = {gem_table_get(_t2145, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t2146, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_str_replace_fn(NULL, _t2147, 3));
    }
#line 1076 "compiler/main.gem"
    GemVal _t2148[] = {gem_v_p};
    GemVal _t2149 = gem_v_cond_r;
    GemVal _t2150[] = {gem_table_get(_t2149, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2148, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t2150, 1)), gem_string(")) break;\n")));
#line 1077 "compiler/main.gem"
    GemVal _t2151 = gem_v_node;
    GemVal _t2152 = (*gem_v_compile_stmts);
    GemVal _t2153[] = {gem_table_get(_t2151, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t2152.fn(_t2152.env, _t2153, 2));
#line 1078 "compiler/main.gem"
    GemVal _t2154[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2154, 1), gem_string("}")));
    GemVal _t2155 = gem_v_out;
    gem_pop_frame();
    return _t2155;
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
#line 1085 "compiler/main.gem"
    GemVal _t2157[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2157, 1);
#line 1086 "compiler/main.gem"
    GemVal _t2158 = gem_v_node;
    GemVal _t2159 = (*gem_v_compile_expr);
    GemVal _t2160[] = {gem_table_get(_t2158, gem_string("target"))};
    GemVal gem_v_target_r = _t2159.fn(_t2159.env, _t2160, 1);
#line 1087 "compiler/main.gem"
    GemVal _t2161 = (*gem_v_tmp);
    GemVal gem_v_t = _t2161.fn(_t2161.env, NULL, 0);
#line 1088 "compiler/main.gem"
    GemVal _t2162 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2162, gem_string("setup"));
#line 1089 "compiler/main.gem"
    GemVal _t2163[] = {gem_v_p};
    GemVal _t2164[] = {gem_v_t};
    GemVal _t2165 = gem_v_target_r;
    GemVal _t2166[] = {gem_table_get(_t2165, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2163, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2164, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2166, 1)), gem_string(";\n")));
    {
#line 1090 "compiler/main.gem"
        GemVal gem_v__for_i_37 = gem_int(0);
#line 1090 "compiler/main.gem"
    GemVal _t2167 = gem_v_node;
    GemVal _t2168[] = {gem_table_get(_t2167, gem_string("whens"))};
        GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t2168, 1);
#line 1090 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 1090 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_37;
#line 1090 "compiler/main.gem"
            gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 1091 "compiler/main.gem"
    GemVal _t2169 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t2169, gem_string("whens")), gem_v_i);
#line 1092 "compiler/main.gem"
    GemVal _t2170 = gem_v_w;
    GemVal _t2171 = (*gem_v_compile_expr);
    GemVal _t2172[] = {gem_table_get(_t2170, gem_string("value"))};
            GemVal gem_v_val_r = _t2171.fn(_t2171.env, _t2172, 1);
#line 1093 "compiler/main.gem"
            GemVal gem_v_keyword = gem_string("if");
#line 1094 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1095 "compiler/main.gem"
                gem_v_keyword = gem_string("} else if");
            }
#line 1097 "compiler/main.gem"
    GemVal _t2173 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t2173, gem_string("setup")));
#line 1098 "compiler/main.gem"
    GemVal _t2174[] = {gem_v_p};
    GemVal _t2175[] = {gem_v_keyword};
    GemVal _t2176[] = {gem_v_t};
    GemVal _t2177 = gem_v_val_r;
    GemVal _t2178[] = {gem_table_get(_t2177, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2174, 1), gem_to_string_fn(NULL, _t2175, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2176, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2178, 1)), gem_string("))) {\n")));
#line 1099 "compiler/main.gem"
    GemVal _t2179 = gem_v_w;
    GemVal _t2180 = (*gem_v_compile_stmts);
    GemVal _t2181[] = {gem_table_get(_t2179, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_v_out, _t2180.fn(_t2180.env, _t2181, 2));
        }
    }
#line 1101 "compiler/main.gem"
    GemVal _t2182 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2182, gem_string("else")), GEM_NIL))) {
#line 1102 "compiler/main.gem"
    GemVal _t2183[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2183, 1), gem_string("} else {\n")));
#line 1103 "compiler/main.gem"
    GemVal _t2184 = gem_v_node;
    GemVal _t2185 = (*gem_v_compile_stmts);
    GemVal _t2186[] = {gem_table_get(_t2184, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t2185.fn(_t2185.env, _t2186, 2));
    }
#line 1105 "compiler/main.gem"
    GemVal _t2187[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2187, 1), gem_string("}")));
    GemVal _t2188 = gem_v_out;
    gem_pop_frame();
    return _t2188;
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
#line 1112 "compiler/main.gem"
    GemVal _t2190[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2190, 1);
#line 1113 "compiler/main.gem"
    GemVal _t2191 = gem_v_node;
    GemVal _t2192 = (*gem_v_compile_expr);
    GemVal _t2193[] = {gem_table_get(_t2191, gem_string("target"))};
    GemVal gem_v_target_r = _t2192.fn(_t2192.env, _t2193, 1);
#line 1114 "compiler/main.gem"
    GemVal _t2194 = (*gem_v_tmp);
    GemVal gem_v_t = _t2194.fn(_t2194.env, NULL, 0);
#line 1115 "compiler/main.gem"
    GemVal _t2195 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2195, gem_string("setup"));
#line 1116 "compiler/main.gem"
    GemVal _t2196[] = {gem_v_p};
    GemVal _t2197[] = {gem_v_t};
    GemVal _t2198 = gem_v_target_r;
    GemVal _t2199[] = {gem_table_get(_t2198, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2196, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2197, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2199, 1)), gem_string(";\n")));
    {
#line 1117 "compiler/main.gem"
        GemVal gem_v__for_i_39 = gem_int(0);
#line 1117 "compiler/main.gem"
    GemVal _t2200 = gem_v_node;
    GemVal _t2201[] = {gem_table_get(_t2200, gem_string("whens"))};
        GemVal gem_v__for_limit_39 = gem_len_fn(NULL, _t2201, 1);
#line 1117 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
#line 1117 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_39;
#line 1117 "compiler/main.gem"
            gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 1118 "compiler/main.gem"
    GemVal _t2202 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t2202, gem_string("whens")), gem_v_i);
#line 1119 "compiler/main.gem"
    GemVal _t2203 = gem_v_w;
    GemVal _t2204 = (*gem_v_compile_expr);
    GemVal _t2205[] = {gem_table_get(_t2203, gem_string("value"))};
            GemVal gem_v_val_r = _t2204.fn(_t2204.env, _t2205, 1);
#line 1120 "compiler/main.gem"
            GemVal gem_v_keyword = gem_string("if");
#line 1121 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1122 "compiler/main.gem"
                gem_v_keyword = gem_string("} else if");
            }
#line 1124 "compiler/main.gem"
    GemVal _t2206 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t2206, gem_string("setup")));
#line 1125 "compiler/main.gem"
    GemVal _t2207[] = {gem_v_p};
    GemVal _t2208[] = {gem_v_keyword};
    GemVal _t2209[] = {gem_v_t};
    GemVal _t2210 = gem_v_val_r;
    GemVal _t2211[] = {gem_table_get(_t2210, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2207, 1), gem_to_string_fn(NULL, _t2208, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2209, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2211, 1)), gem_string("))) {\n")));
#line 1126 "compiler/main.gem"
    GemVal _t2212 = gem_v_w;
            GemVal gem_v_body = gem_table_get(_t2212, gem_string("body"));
#line 1127 "compiler/main.gem"
    GemVal _t2213[] = {gem_v_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2213, 1), gem_int(0)))) {
                {
#line 1128 "compiler/main.gem"
                    GemVal gem_v__for_i_38 = gem_int(0);
#line 1128 "compiler/main.gem"
    GemVal _t2214[] = {gem_v_body};
                    GemVal gem_v__for_limit_38 = gem_sub(gem_len_fn(NULL, _t2214, 1), gem_int(1));
#line 1128 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 1128 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_38;
#line 1128 "compiler/main.gem"
                        gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 1129 "compiler/main.gem"
    GemVal _t2215 = (*gem_v_compile_stmt);
    GemVal _t2216[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
                        gem_v_out = gem_add(gem_add(gem_v_out, _t2215.fn(_t2215.env, _t2216, 2)), gem_string("\n"));
                    }
                }
#line 1131 "compiler/main.gem"
    GemVal _t2217[] = {gem_v_body};
    GemVal _t2218 = (*gem_v_compile_stmt_return);
    GemVal _t2219[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2217, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t2218.fn(_t2218.env, _t2219, 2)), gem_string("\n"));
            } else {
#line 1133 "compiler/main.gem"
    GemVal _t2220[] = {gem_v_p};
    GemVal _t2221[] = {gem_v_p};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2220, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2221, 1)), gem_string("    return GEM_NIL;\n")));
            }
        }
    }
#line 1136 "compiler/main.gem"
    GemVal _t2222 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2222, gem_string("else")), GEM_NIL))) {
#line 1137 "compiler/main.gem"
    GemVal _t2223[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2223, 1), gem_string("} else {\n")));
#line 1138 "compiler/main.gem"
    GemVal _t2224 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t2224, gem_string("else"));
#line 1139 "compiler/main.gem"
    GemVal _t2225[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2225, 1), gem_int(0)))) {
            {
#line 1140 "compiler/main.gem"
                GemVal gem_v__for_i_40 = gem_int(0);
#line 1140 "compiler/main.gem"
    GemVal _t2226[] = {gem_v_else_body};
                GemVal gem_v__for_limit_40 = gem_sub(gem_len_fn(NULL, _t2226, 1), gem_int(1));
#line 1140 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 1140 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_40;
#line 1140 "compiler/main.gem"
                    gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 1141 "compiler/main.gem"
    GemVal _t2227 = (*gem_v_compile_stmt);
    GemVal _t2228[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t2227.fn(_t2227.env, _t2228, 2)), gem_string("\n"));
                }
            }
#line 1143 "compiler/main.gem"
    GemVal _t2229[] = {gem_v_else_body};
    GemVal _t2230 = (*gem_v_compile_stmt_return);
    GemVal _t2231[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2229, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t2230.fn(_t2230.env, _t2231, 2)), gem_string("\n"));
        } else {
#line 1145 "compiler/main.gem"
    GemVal _t2232[] = {gem_v_p};
    GemVal _t2233[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2232, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2233, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1148 "compiler/main.gem"
    GemVal _t2234[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2234, 1), gem_string("} else {\n")));
#line 1149 "compiler/main.gem"
    GemVal _t2235[] = {gem_v_p};
    GemVal _t2236[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2235, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2236, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1151 "compiler/main.gem"
    GemVal _t2237[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2237, 1), gem_string("}")));
    GemVal _t2238 = gem_v_out;
    gem_pop_frame();
    return _t2238;
}

struct _closure__anon_44 {
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_44", "compiler/main.gem", 0);
#line 1158 "compiler/main.gem"
    GemVal _t2240 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t2240, gem_string("name"));
#line 1159 "compiler/main.gem"
    GemVal _t2241 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t2241, gem_string("params"));
#line 1160 "compiler/main.gem"
    GemVal _t2242 = gem_v_node;
    GemVal gem_v_ret_type = gem_table_get(_t2242, gem_string("ret_type"));
#line 1162 "compiler/main.gem"
    GemVal _t2243[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2243, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1164 "compiler/main.gem"
    GemVal _t2244 = gem_table_new();
    GemVal gem_v_c_args = _t2244;
    {
#line 1165 "compiler/main.gem"
        GemVal gem_v__for_i_41 = gem_int(0);
#line 1165 "compiler/main.gem"
    GemVal _t2245[] = {gem_v_params};
        GemVal gem_v__for_limit_41 = gem_len_fn(NULL, _t2245, 1);
#line 1165 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_limit_41))) break;
#line 1165 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_41;
#line 1165 "compiler/main.gem"
            gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
#line 1166 "compiler/main.gem"
            GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1167 "compiler/main.gem"
            GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1168 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1169 "compiler/main.gem"
    GemVal _t2246[] = {gem_v_i};
    GemVal _t2247[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t2246, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2247, 1)), gem_string("].ival;\n")));
#line 1170 "compiler/main.gem"
    GemVal _t2248[] = {gem_v_i};
    GemVal _t2249[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2248, 1))};
                (void)(gem_push_fn(NULL, _t2249, 2));
            } else {
#line 1171 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1172 "compiler/main.gem"
    GemVal _t2250[] = {gem_v_i};
    GemVal _t2251[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t2250, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2251, 1)), gem_string("].fval;\n")));
#line 1173 "compiler/main.gem"
    GemVal _t2252[] = {gem_v_i};
    GemVal _t2253[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2252, 1))};
                    (void)(gem_push_fn(NULL, _t2253, 2));
                } else {
#line 1174 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1175 "compiler/main.gem"
    GemVal _t2254[] = {gem_v_i};
    GemVal _t2255[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t2254, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2255, 1)), gem_string("].sval;\n")));
#line 1176 "compiler/main.gem"
    GemVal _t2256[] = {gem_v_i};
    GemVal _t2257[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2256, 1))};
                        (void)(gem_push_fn(NULL, _t2257, 2));
                    } else {
#line 1177 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1178 "compiler/main.gem"
    GemVal _t2258[] = {gem_v_i};
    GemVal _t2259[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t2258, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2259, 1)), gem_string("].bval;\n")));
#line 1179 "compiler/main.gem"
    GemVal _t2260[] = {gem_v_i};
    GemVal _t2261[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2260, 1))};
                            (void)(gem_push_fn(NULL, _t2261, 2));
                        } else {
#line 1180 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1181 "compiler/main.gem"
    GemVal _t2262[] = {gem_v_i};
    GemVal _t2263[] = {gem_v_i};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t2262, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t2263, 1)), gem_string("].ival;\n")));
#line 1182 "compiler/main.gem"
    GemVal _t2264[] = {gem_v_i};
    GemVal _t2265[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2264, 1))};
                                (void)(gem_push_fn(NULL, _t2265, 2));
                            } else {
#line 1183 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1184 "compiler/main.gem"
    GemVal _t2266[] = {gem_v_i};
    GemVal _t2267[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2266, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t2267, 2));
                                } else {
#line 1186 "compiler/main.gem"
    GemVal _t2268[] = {gem_v_i};
    GemVal _t2269[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2268, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t2269, 2));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 1191 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
    {
#line 1192 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 1192 "compiler/main.gem"
    GemVal _t2270[] = {gem_v_c_args};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2270, 1);
#line 1192 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 1192 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_42;
#line 1192 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 1193 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1194 "compiler/main.gem"
                gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
            }
#line 1196 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
        }
    }
#line 1199 "compiler/main.gem"
    GemVal _t2271[] = {gem_v_name};
    GemVal _t2272[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2271, 1), gem_string("(")), gem_to_string_fn(NULL, _t2272, 1)), gem_string(")"));
#line 1201 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1202 "compiler/main.gem"
    GemVal _t2273[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2273, 1)), gem_string(";\n")));
#line 1203 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
#line 1204 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1205 "compiler/main.gem"
    GemVal _t2274[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t2274, 1)), gem_string(";\n")));
        } else {
#line 1206 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1207 "compiler/main.gem"
    GemVal _t2275[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t2275, 1)), gem_string(";\n")));
#line 1208 "compiler/main.gem"
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
#line 1209 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1210 "compiler/main.gem"
    GemVal _t2276[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t2276, 1)), gem_string(";\n")));
#line 1211 "compiler/main.gem"
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
#line 1212 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1213 "compiler/main.gem"
    GemVal _t2277[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t2277, 1)), gem_string(";\n")));
#line 1214 "compiler/main.gem"
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
#line 1215 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1216 "compiler/main.gem"
    GemVal _t2278[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t2278, 1)), gem_string(";\n")));
#line 1217 "compiler/main.gem"
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
#line 1218 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1219 "compiler/main.gem"
    GemVal _t2279[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t2279, 1)), gem_string(";\n")));
#line 1220 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
#line 1222 "compiler/main.gem"
    GemVal _t2280[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2280, 1)), gem_string(";\n")));
#line 1223 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1226 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1229 "compiler/main.gem"
    GemVal _t2281[] = {gem_v_name};
    GemVal _t2282[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2281, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t2282, 2));
    GemVal _t2283 = gem_v_lines;
    gem_pop_frame();
    return _t2283;
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
#line 1237 "compiler/main.gem"
    GemVal _t2285 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t2285, gem_string("params"));
#line 1238 "compiler/main.gem"
    GemVal _t2286 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t2286, gem_string("body"));
#line 1239 "compiler/main.gem"
    GemVal _t2287 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t2287, gem_string("name"));
#line 1242 "compiler/main.gem"
    GemVal _t2288[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t2288, 1);
    {
#line 1243 "compiler/main.gem"
        GemVal gem_v__for_i_44 = gem_int(0);
#line 1243 "compiler/main.gem"
    GemVal _t2289[] = {gem_v_body};
        GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t2289, 1);
#line 1243 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 1243 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_44;
#line 1243 "compiler/main.gem"
            gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 1244 "compiler/main.gem"
    GemVal _t2290[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2292;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2290, 1), gem_string("table")))) {
        _t2292 = gem_eq(gem_type_fn(NULL, _t2290, 1), gem_string("table"));
    } else {
        GemVal _t2291 = gem_table_get(gem_v_body, gem_v_i);
        _t2292 = gem_eq(gem_table_get(_t2291, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t2292)) {
#line 1245 "compiler/main.gem"
    GemVal _t2293 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2294[] = {gem_v_scope_vars, gem_table_get(_t2293, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t2294, 2));
            }
#line 1247 "compiler/main.gem"
    GemVal _t2295[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2297;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2295, 1), gem_string("table")))) {
        _t2297 = gem_eq(gem_type_fn(NULL, _t2295, 1), gem_string("table"));
    } else {
        GemVal _t2296 = gem_table_get(gem_v_body, gem_v_i);
        _t2297 = gem_eq(gem_table_get(_t2296, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t2297)) {
                {
#line 1248 "compiler/main.gem"
                    GemVal gem_v__for_i_43 = gem_int(0);
#line 1248 "compiler/main.gem"
    GemVal _t2298 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2299[] = {gem_table_get(_t2298, gem_string("stmts"))};
                    GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t2299, 1);
#line 1248 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 1248 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_43;
#line 1248 "compiler/main.gem"
                        gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 1249 "compiler/main.gem"
    GemVal _t2300 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2301[] = {gem_table_get(gem_table_get(_t2300, gem_string("stmts")), gem_v_j)};
    GemVal _t2304;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2301, 1), gem_string("table")))) {
        _t2304 = gem_eq(gem_type_fn(NULL, _t2301, 1), gem_string("table"));
    } else {
        GemVal _t2302 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t2303 = gem_table_get(gem_table_get(_t2302, gem_string("stmts")), gem_v_j);
        _t2304 = gem_eq(gem_table_get(_t2303, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t2304)) {
#line 1250 "compiler/main.gem"
    GemVal _t2305 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2306 = gem_table_get(gem_table_get(_t2305, gem_string("stmts")), gem_v_j);
    GemVal _t2307[] = {gem_v_scope_vars, gem_table_get(_t2306, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t2307, 2));
                        }
                    }
                }
            }
        }
    }
#line 1255 "compiler/main.gem"
    GemVal _t2308 = (*gem_v_find_captured_in_scope);
    GemVal _t2309[] = {gem_v_body, gem_v_scope_vars};
    GemVal gem_v_captured = _t2308.fn(_t2308.env, _t2309, 2);
#line 1257 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1258 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1260 "compiler/main.gem"
    GemVal _t2310[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2310, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1263 "compiler/main.gem"
    GemVal _t2311 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2311, gem_string("line")), GEM_NIL))) {
#line 1264 "compiler/main.gem"
    GemVal _t2312 = gem_v_node;
    GemVal _t2313[] = {gem_table_get(_t2312, gem_string("line"))};
    GemVal _t2314[] = {(*gem_v_source_name)};
    GemVal _t2315[] = {gem_fn_escape_c_string(NULL, _t2314, 1)};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2313, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2315, 1)), gem_string("\"\n")));
    }
    {
#line 1268 "compiler/main.gem"
        GemVal gem_v__for_i_45 = gem_int(0);
#line 1268 "compiler/main.gem"
    GemVal _t2316[] = {gem_v_params};
        GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2316, 1);
#line 1268 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 1268 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_45;
#line 1268 "compiler/main.gem"
            gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 1269 "compiler/main.gem"
    GemVal _t2317 = (*gem_v_mangle);
    GemVal _t2318[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t2317.fn(_t2317.env, _t2318, 1);
#line 1270 "compiler/main.gem"
    GemVal _t2319[] = {gem_v_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2319, 2))) {
#line 1271 "compiler/main.gem"
    GemVal _t2320[] = {gem_v_mp};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2320, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1272 "compiler/main.gem"
    GemVal _t2321[] = {gem_v_mp};
    GemVal _t2322[] = {gem_v_i};
    GemVal _t2323[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2321, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2322, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2323, 1)), gem_string("] : GEM_NIL;\n")));
            } else {
#line 1274 "compiler/main.gem"
    GemVal _t2324[] = {gem_v_mp};
    GemVal _t2325[] = {gem_v_i};
    GemVal _t2326[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2324, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2325, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2326, 1)), gem_string("] : GEM_NIL;\n")));
            }
        }
    }
#line 1278 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1279 "compiler/main.gem"
    GemVal _t2327 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2327, gem_string("line")), GEM_NIL))) {
#line 1280 "compiler/main.gem"
    GemVal _t2328 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t2328, gem_string("line"));
    }
#line 1282 "compiler/main.gem"
    GemVal _t2329[] = {gem_v_name};
    GemVal _t2330[] = {gem_fn_escape_c_string(NULL, _t2329, 1)};
    GemVal _t2331[] = {(*gem_v_source_name)};
    GemVal _t2332[] = {gem_fn_escape_c_string(NULL, _t2331, 1)};
    GemVal _t2333[] = {gem_v_fn_line};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2330, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2332, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2333, 1)), gem_string(");\n")));
#line 1285 "compiler/main.gem"
    GemVal _t2334[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2334, 1), gem_int(0)))) {
        {
#line 1286 "compiler/main.gem"
            GemVal gem_v__for_i_46 = gem_int(0);
#line 1286 "compiler/main.gem"
    GemVal _t2335[] = {gem_v_body};
            GemVal gem_v__for_limit_46 = gem_sub(gem_len_fn(NULL, _t2335, 1), gem_int(1));
#line 1286 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
#line 1286 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_46;
#line 1286 "compiler/main.gem"
                gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 1287 "compiler/main.gem"
    GemVal _t2336 = (*gem_v_compile_stmt);
    GemVal _t2337[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t2336.fn(_t2336.env, _t2337, 2)), gem_string("\n"));
            }
        }
#line 1289 "compiler/main.gem"
    GemVal _t2338[] = {gem_v_body};
    GemVal _t2339 = (*gem_v_compile_stmt_return);
    GemVal _t2340[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2338, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t2339.fn(_t2339.env, _t2340, 2)), gem_string("\n"));
    } else {
#line 1291 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
#line 1294 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1295 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
    GemVal _t2341 = gem_v_lines;
    gem_pop_frame();
    return _t2341;
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
#line 1302 "compiler/main.gem"
    GemVal _t2343 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t2343, gem_string("params"));
#line 1303 "compiler/main.gem"
    GemVal _t2344 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t2344, gem_string("body"));
#line 1304 "compiler/main.gem"
    GemVal _t2345 = (*gem_v_anon_name);
    GemVal gem_v_name = _t2345.fn(_t2345.env, NULL, 0);
#line 1307 "compiler/main.gem"
    GemVal _t2346[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t2346, 1);
    {
#line 1308 "compiler/main.gem"
        GemVal gem_v__for_i_48 = gem_int(0);
#line 1308 "compiler/main.gem"
    GemVal _t2347[] = {gem_v_body};
        GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t2347, 1);
#line 1308 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 1308 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_48;
#line 1308 "compiler/main.gem"
            gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 1309 "compiler/main.gem"
    GemVal _t2348[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2350;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2348, 1), gem_string("table")))) {
        _t2350 = gem_eq(gem_type_fn(NULL, _t2348, 1), gem_string("table"));
    } else {
        GemVal _t2349 = gem_table_get(gem_v_body, gem_v_i);
        _t2350 = gem_eq(gem_table_get(_t2349, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t2350)) {
#line 1310 "compiler/main.gem"
    GemVal _t2351 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2352[] = {gem_v_inner_scope, gem_table_get(_t2351, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t2352, 2));
            }
#line 1312 "compiler/main.gem"
    GemVal _t2353[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2355;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2353, 1), gem_string("table")))) {
        _t2355 = gem_eq(gem_type_fn(NULL, _t2353, 1), gem_string("table"));
    } else {
        GemVal _t2354 = gem_table_get(gem_v_body, gem_v_i);
        _t2355 = gem_eq(gem_table_get(_t2354, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t2355)) {
                {
#line 1313 "compiler/main.gem"
                    GemVal gem_v__for_i_47 = gem_int(0);
#line 1313 "compiler/main.gem"
    GemVal _t2356 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2357[] = {gem_table_get(_t2356, gem_string("stmts"))};
                    GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2357, 1);
#line 1313 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 1313 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_47;
#line 1313 "compiler/main.gem"
                        gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 1314 "compiler/main.gem"
    GemVal _t2358 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2359[] = {gem_table_get(gem_table_get(_t2358, gem_string("stmts")), gem_v_j)};
    GemVal _t2362;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2359, 1), gem_string("table")))) {
        _t2362 = gem_eq(gem_type_fn(NULL, _t2359, 1), gem_string("table"));
    } else {
        GemVal _t2360 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t2361 = gem_table_get(gem_table_get(_t2360, gem_string("stmts")), gem_v_j);
        _t2362 = gem_eq(gem_table_get(_t2361, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t2362)) {
#line 1315 "compiler/main.gem"
    GemVal _t2363 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2364 = gem_table_get(gem_table_get(_t2363, gem_string("stmts")), gem_v_j);
    GemVal _t2365[] = {gem_v_inner_scope, gem_table_get(_t2364, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t2365, 2));
                        }
                    }
                }
            }
        }
    }
#line 1320 "compiler/main.gem"
    GemVal _t2366 = (*gem_v_find_captured_in_scope);
    GemVal _t2367[] = {gem_v_body, gem_v_inner_scope};
    GemVal gem_v_inner_captured = _t2366.fn(_t2366.env, _t2367, 2);
#line 1322 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1323 "compiler/main.gem"
    GemVal _t2368[] = {gem_v_captures};
    GemVal _t2369[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t2368, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t2369, 2);
#line 1326 "compiler/main.gem"
    GemVal _t2370[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t2370, 1));
#line 1327 "compiler/main.gem"
    GemVal _t2371[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t2371, 1)), gem_string(" {\n"));
    {
#line 1328 "compiler/main.gem"
        GemVal gem_v__for_items_49 = gem_v_captures;
#line 1328 "compiler/main.gem"
        GemVal gem_v__for_i_49 = gem_int(0);
#line 1328 "compiler/main.gem"
        while (1) {
            GemVal _t2372[] = {gem_v__for_items_49};
            if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_len_fn(NULL, _t2372, 1)))) break;
#line 1328 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_49, gem_v__for_i_49);
#line 1328 "compiler/main.gem"
            gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 1329 "compiler/main.gem"
    GemVal _t2373 = (*gem_v_mangle);
    GemVal _t2374[] = {gem_v_cap};
    GemVal _t2375[] = {_t2373.fn(_t2373.env, _t2374, 1)};
            gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2375, 1)), gem_string(";\n")));
        }
    }
#line 1331 "compiler/main.gem"
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
#line 1334 "compiler/main.gem"
    GemVal _t2376[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t2376, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1335 "compiler/main.gem"
    GemVal _t2377[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2377, 1), gem_int(0)))) {
#line 1336 "compiler/main.gem"
    GemVal _t2378[] = {gem_v_struct_name};
    GemVal _t2379[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2378, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t2379, 1)), gem_string(" *)_env;\n")));
        {
#line 1337 "compiler/main.gem"
            GemVal gem_v__for_items_50 = gem_v_captures;
#line 1337 "compiler/main.gem"
            GemVal gem_v__for_i_50 = gem_int(0);
#line 1337 "compiler/main.gem"
            while (1) {
                GemVal _t2380[] = {gem_v__for_items_50};
                if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_len_fn(NULL, _t2380, 1)))) break;
#line 1337 "compiler/main.gem"
                GemVal gem_v_cap = gem_table_get(gem_v__for_items_50, gem_v__for_i_50);
#line 1337 "compiler/main.gem"
                gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 1338 "compiler/main.gem"
    GemVal _t2381 = (*gem_v_mangle);
    GemVal _t2382[] = {gem_v_cap};
                GemVal gem_v_mc = _t2381.fn(_t2381.env, _t2382, 1);
#line 1339 "compiler/main.gem"
    GemVal _t2383[] = {gem_v_mc};
    GemVal _t2384[] = {gem_v_mc};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2383, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t2384, 1)), gem_string(";\n")));
            }
        }
    }
    {
#line 1344 "compiler/main.gem"
        GemVal gem_v__for_i_51 = gem_int(0);
#line 1344 "compiler/main.gem"
    GemVal _t2385[] = {gem_v_params};
        GemVal gem_v__for_limit_51 = gem_len_fn(NULL, _t2385, 1);
#line 1344 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_v__for_limit_51))) break;
#line 1344 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_51;
#line 1344 "compiler/main.gem"
            gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 1345 "compiler/main.gem"
    GemVal _t2386 = (*gem_v_mangle);
    GemVal _t2387[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t2386.fn(_t2386.env, _t2387, 1);
#line 1346 "compiler/main.gem"
    GemVal _t2388[] = {gem_v_inner_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2388, 2))) {
#line 1347 "compiler/main.gem"
    GemVal _t2389[] = {gem_v_mp};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2389, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1348 "compiler/main.gem"
    GemVal _t2390[] = {gem_v_mp};
    GemVal _t2391[] = {gem_v_i};
    GemVal _t2392[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2390, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2391, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2392, 1)), gem_string("] : GEM_NIL;\n")));
            } else {
#line 1350 "compiler/main.gem"
    GemVal _t2393[] = {gem_v_mp};
    GemVal _t2394[] = {gem_v_i};
    GemVal _t2395[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2393, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2394, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2395, 1)), gem_string("] : GEM_NIL;\n")));
            }
        }
    }
#line 1354 "compiler/main.gem"
    GemVal _t2396[] = {gem_v_name};
    GemVal _t2397[] = {gem_fn_escape_c_string(NULL, _t2396, 1)};
    GemVal _t2398[] = {(*gem_v_source_name)};
    GemVal _t2399[] = {gem_fn_escape_c_string(NULL, _t2398, 1)};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2397, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2399, 1)), gem_string("\", 0);\n")));
#line 1356 "compiler/main.gem"
    GemVal _t2400[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2400, 1), gem_int(0)))) {
        {
#line 1357 "compiler/main.gem"
            GemVal gem_v__for_i_52 = gem_int(0);
#line 1357 "compiler/main.gem"
    GemVal _t2401[] = {gem_v_body};
            GemVal gem_v__for_limit_52 = gem_sub(gem_len_fn(NULL, _t2401, 1), gem_int(1));
#line 1357 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 1357 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_52;
#line 1357 "compiler/main.gem"
                gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 1358 "compiler/main.gem"
    GemVal _t2402 = (*gem_v_compile_stmt);
    GemVal _t2403[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t2402.fn(_t2402.env, _t2403, 2)), gem_string("\n"));
            }
        }
#line 1360 "compiler/main.gem"
    GemVal _t2404[] = {gem_v_body};
    GemVal _t2405 = (*gem_v_compile_stmt_return);
    GemVal _t2406[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2404, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t2405.fn(_t2405.env, _t2406, 2)), gem_string("\n"));
    } else {
#line 1362 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
#line 1365 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1366 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1369 "compiler/main.gem"
    GemVal _t2407[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t2407, 2));
    GemVal _t2408 = gem_table_new();
    gem_table_set(_t2408, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t2408, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t2408, gem_string("captures"), gem_v_captures);
    GemVal _t2409 = _t2408;
    gem_pop_frame();
    return _t2409;
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
#line 1378 "compiler/main.gem"
    GemVal _t2411 = gem_table_new();
    GemVal gem_v_fn_defs = _t2411;
#line 1379 "compiler/main.gem"
    GemVal _t2412 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t2412;
#line 1380 "compiler/main.gem"
    GemVal _t2413 = gem_table_new();
    GemVal gem_v_extern_includes = _t2413;
#line 1381 "compiler/main.gem"
    GemVal _t2414 = gem_table_new();
    GemVal gem_v_top_stmts = _t2414;
    {
#line 1383 "compiler/main.gem"
    GemVal _t2415 = gem_v_ast;
        GemVal gem_v__for_items_53 = gem_table_get(_t2415, gem_string("stmts"));
#line 1383 "compiler/main.gem"
        GemVal gem_v__for_i_53 = gem_int(0);
#line 1383 "compiler/main.gem"
        while (1) {
            GemVal _t2416[] = {gem_v__for_items_53};
            if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_len_fn(NULL, _t2416, 1)))) break;
#line 1383 "compiler/main.gem"
            GemVal gem_v_s = gem_table_get(gem_v__for_items_53, gem_v__for_i_53);
#line 1383 "compiler/main.gem"
            gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 1384 "compiler/main.gem"
    GemVal _t2417 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t2417, gem_string("tag")), gem_string("fn_def")))) {
#line 1385 "compiler/main.gem"
    GemVal _t2418[] = {gem_v_fn_defs, gem_v_s};
                (void)(gem_push_fn(NULL, _t2418, 2));
            } else {
#line 1386 "compiler/main.gem"
    GemVal _t2419 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t2419, gem_string("tag")), gem_string("extern_fn")))) {
#line 1387 "compiler/main.gem"
    GemVal _t2420[] = {gem_v_extern_fns_list, gem_v_s};
                    (void)(gem_push_fn(NULL, _t2420, 2));
                } else {
#line 1388 "compiler/main.gem"
    GemVal _t2421 = gem_v_s;
                    if (gem_truthy(gem_eq(gem_table_get(_t2421, gem_string("tag")), gem_string("extern_include")))) {
#line 1389 "compiler/main.gem"
    GemVal _t2422[] = {gem_v_extern_includes, gem_v_s};
                        (void)(gem_push_fn(NULL, _t2422, 2));
                    } else {
#line 1391 "compiler/main.gem"
    GemVal _t2423[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t2423, 2));
                    }
                }
            }
        }
    }
    {
#line 1396 "compiler/main.gem"
        GemVal gem_v__for_items_54 = gem_v_extern_fns_list;
#line 1396 "compiler/main.gem"
        GemVal gem_v__for_i_54 = gem_int(0);
#line 1396 "compiler/main.gem"
        while (1) {
            GemVal _t2424[] = {gem_v__for_items_54};
            if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_len_fn(NULL, _t2424, 1)))) break;
#line 1396 "compiler/main.gem"
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_54, gem_v__for_i_54);
#line 1396 "compiler/main.gem"
            gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 1397 "compiler/main.gem"
    GemVal _t2425 = gem_v_ef;
    GemVal _t2426[] = {(*gem_v_defined_fns), gem_table_get(_t2425, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2426, 2));
        }
    }
    {
#line 1401 "compiler/main.gem"
        GemVal gem_v__for_items_55 = gem_v_fn_defs;
#line 1401 "compiler/main.gem"
        GemVal gem_v__for_i_55 = gem_int(0);
#line 1401 "compiler/main.gem"
        while (1) {
            GemVal _t2427[] = {gem_v__for_items_55};
            if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_len_fn(NULL, _t2427, 1)))) break;
#line 1401 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_55, gem_v__for_i_55);
#line 1401 "compiler/main.gem"
            gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1402 "compiler/main.gem"
    GemVal _t2428 = gem_v_fd;
    GemVal _t2429[] = {(*gem_v_defined_fns), gem_table_get(_t2428, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2429, 2));
#line 1403 "compiler/main.gem"
    GemVal _t2430 = gem_v_fd;
    GemVal _t2431[] = {gem_table_get(_t2430, gem_string("name"))};
            GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2431, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1404 "compiler/main.gem"
    GemVal _t2432[] = {(*gem_v_forward_decls), gem_v_decl};
            (void)(gem_push_fn(NULL, _t2432, 2));
        }
    }
    {
#line 1408 "compiler/main.gem"
        GemVal gem_v__for_items_56 = gem_v_extern_fns_list;
#line 1408 "compiler/main.gem"
        GemVal gem_v__for_i_56 = gem_int(0);
#line 1408 "compiler/main.gem"
        while (1) {
            GemVal _t2433[] = {gem_v__for_items_56};
            if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_len_fn(NULL, _t2433, 1)))) break;
#line 1408 "compiler/main.gem"
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_56, gem_v__for_i_56);
#line 1408 "compiler/main.gem"
            gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1409 "compiler/main.gem"
    GemVal _t2434 = (*gem_v_compile_extern_fn);
    GemVal _t2435[] = {gem_v_ef};
    GemVal _t2436[] = {(*gem_v_functions), _t2434.fn(_t2434.env, _t2435, 1)};
            (void)(gem_push_fn(NULL, _t2436, 2));
        }
    }
    {
#line 1413 "compiler/main.gem"
        GemVal gem_v__for_items_57 = gem_v_fn_defs;
#line 1413 "compiler/main.gem"
        GemVal gem_v__for_i_57 = gem_int(0);
#line 1413 "compiler/main.gem"
        while (1) {
            GemVal _t2437[] = {gem_v__for_items_57};
            if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t2437, 1)))) break;
#line 1413 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
#line 1413 "compiler/main.gem"
            gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
#line 1414 "compiler/main.gem"
    GemVal _t2438 = (*gem_v_compile_fn);
    GemVal _t2439[] = {gem_v_fd};
    GemVal _t2440[] = {(*gem_v_functions), _t2438.fn(_t2438.env, _t2439, 1)};
            (void)(gem_push_fn(NULL, _t2440, 2));
        }
    }
#line 1418 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1419 "compiler/main.gem"
    GemVal _t2441 = gem_table_new();
    GemVal _t2442 = (*gem_v_find_captured_in_scope);
    GemVal _t2443[] = {gem_v_top_stmts, _t2441};
    *gem_v_boxed_vars = _t2442.fn(_t2442.env, _t2443, 2);
#line 1420 "compiler/main.gem"
    GemVal _t2444 = (*gem_v_compile_stmts);
    GemVal _t2445[] = {gem_v_top_stmts, gem_int(1)};
    GemVal gem_v_main_body = _t2444.fn(_t2444.env, _t2445, 2);
#line 1421 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1423 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
    {
#line 1424 "compiler/main.gem"
        GemVal gem_v__for_items_58 = gem_v_fn_defs;
#line 1424 "compiler/main.gem"
        GemVal gem_v__for_i_58 = gem_int(0);
#line 1424 "compiler/main.gem"
        while (1) {
            GemVal _t2446[] = {gem_v__for_items_58};
            if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t2446, 1)))) break;
#line 1424 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1424 "compiler/main.gem"
            gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1425 "compiler/main.gem"
    GemVal _t2447 = gem_v_fd;
            if (gem_truthy(gem_eq(gem_table_get(_t2447, gem_string("name")), gem_string("main")))) {
#line 1426 "compiler/main.gem"
                gem_v_has_main = gem_bool(1);
            }
        }
    }
#line 1430 "compiler/main.gem"
    GemVal gem_v_main_c = gem_string("int main(void) {\n");
#line 1431 "compiler/main.gem"
    GemVal _t2448[] = {(*gem_v_source_name)};
    GemVal _t2449[] = {gem_fn_escape_c_string(NULL, _t2448, 1)};
    gem_v_main_c = gem_add(gem_v_main_c, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t2449, 1)), gem_string("\", 0);\n")));
#line 1432 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1433 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
#line 1435 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1436 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
#line 1437 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
#line 1439 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_run_scheduler();\n"));
#line 1440 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_pop_frame();\n    return 0;\n}\n"));
#line 1444 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
    {
#line 1445 "compiler/main.gem"
        GemVal gem_v__for_items_59 = gem_v_extern_includes;
#line 1445 "compiler/main.gem"
        GemVal gem_v__for_i_59 = gem_int(0);
#line 1445 "compiler/main.gem"
        while (1) {
            GemVal _t2450[] = {gem_v__for_items_59};
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_len_fn(NULL, _t2450, 1)))) break;
#line 1445 "compiler/main.gem"
            GemVal gem_v_ei = gem_table_get(gem_v__for_items_59, gem_v__for_i_59);
#line 1445 "compiler/main.gem"
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1446 "compiler/main.gem"
    GemVal _t2451 = gem_v_ei;
    GemVal _t2452[] = {gem_table_get(_t2451, gem_string("path"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t2452, 1)), gem_string("\"\n")));
        }
    }
#line 1448 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
    {
#line 1451 "compiler/main.gem"
        GemVal gem_v__for_i_60 = gem_int(0);
#line 1451 "compiler/main.gem"
    GemVal _t2453[] = {(*gem_v_forward_decls)};
        GemVal gem_v__for_limit_60 = gem_len_fn(NULL, _t2453, 1);
#line 1451 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1451 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_60;
#line 1451 "compiler/main.gem"
            gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1452 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
#line 1453 "compiler/main.gem"
    GemVal _t2454[] = {(*gem_v_forward_decls)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2454, 1), gem_int(1))))) {
#line 1454 "compiler/main.gem"
                gem_v_out = gem_add(gem_v_out, gem_string("\n"));
            }
        }
    }
#line 1457 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
    {
#line 1460 "compiler/main.gem"
        GemVal gem_v__for_i_61 = gem_int(0);
#line 1460 "compiler/main.gem"
    GemVal _t2455[] = {(*gem_v_functions)};
        GemVal gem_v__for_limit_61 = gem_len_fn(NULL, _t2455, 1);
#line 1460 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_v__for_limit_61))) break;
#line 1460 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_61;
#line 1460 "compiler/main.gem"
            gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1461 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
#line 1462 "compiler/main.gem"
    GemVal _t2456[] = {(*gem_v_functions)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2456, 1), gem_int(1))))) {
#line 1463 "compiler/main.gem"
                gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
            }
        }
    }
#line 1466 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
#line 1468 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    GemVal _t2457 = gem_v_out;
    gem_pop_frame();
    return _t2457;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 136 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_codegen", "compiler/main.gem", 136);
#line 137 "compiler/main.gem"
    GemVal _t942 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t942;
#line 138 "compiler/main.gem"
    GemVal _t943 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t943;
#line 139 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 140 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 141 "compiler/main.gem"
    GemVal _t944 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t944;
#line 142 "compiler/main.gem"
    GemVal _t945 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t945;
#line 143 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("print"), gem_bool(1));
#line 144 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("error"), gem_bool(1));
#line 145 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("len"), gem_bool(1));
#line 146 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("type"), gem_bool(1));
#line 147 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("to_string"), gem_bool(1));
#line 148 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("push"), gem_bool(1));
#line 149 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("pcall"), gem_bool(1));
#line 150 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("keys"), gem_bool(1));
#line 151 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("str_replace"), gem_bool(1));
#line 152 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("has_key"), gem_bool(1));
#line 153 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("split"), gem_bool(1));
#line 154 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("substr"), gem_bool(1));
#line 155 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("index_of"), gem_bool(1));
#line 156 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("chr"), gem_bool(1));
#line 157 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("ord"), gem_bool(1));
#line 158 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("spawn"), gem_bool(1));
#line 159 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("send"), gem_bool(1));
#line 160 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("receive"), gem_bool(1));
#line 161 "compiler/main.gem"
    gem_table_set((*gem_v_builtins), gem_string("self"), gem_bool(1));
#line 162 "compiler/main.gem"
    GemVal _t946 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t946;
#line 166 "compiler/main.gem"
    struct _closure__anon_22 *_t949 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t949->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_22, _t949);
#line 171 "compiler/main.gem"
    struct _closure__anon_23 *_t952 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t952->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_23, _t952);
#line 176 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_24, NULL);
#line 182 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 183 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 185 "compiler/main.gem"
    struct _closure__anon_25 *_t1121 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1121->gem_v_builtins = gem_v_builtins;
    _t1121->gem_v_collect_free_node = gem_v_collect_free_node;
    _t1121->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1121->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_25, _t1121);
#line 323 "compiler/main.gem"
    struct _closure__anon_26 *_t1147 = GC_MALLOC(sizeof(struct _closure__anon_26));
    _t1147->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_26, _t1147);
#line 344 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 345 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 347 "compiler/main.gem"
    struct _closure__anon_27 *_t1235 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1235->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1235->gem_v_walk_captures = gem_v_walk_captures;
    _t1235->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_27, _t1235);
#line 427 "compiler/main.gem"
    struct _closure__anon_28 *_t1259 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1259->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_28, _t1259);
#line 444 "compiler/main.gem"
    struct _closure__anon_29 *_t1264 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t1264->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_29, _t1264);
#line 452 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 453 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 454 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 455 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 456 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 457 "compiler/main.gem"
    GemVal *gem_v_compile_if_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if_return = GEM_NIL;
#line 458 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 459 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 460 "compiler/main.gem"
    GemVal *gem_v_compile_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match_return = GEM_NIL;
#line 461 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 462 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 463 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 464 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 465 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 466 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 467 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 468 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 472 "compiler/main.gem"
    struct _closure__anon_30 *_t1421 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t1421->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1421->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t1421->gem_v_compile_array = gem_v_compile_array;
    _t1421->gem_v_compile_binop = gem_v_compile_binop;
    _t1421->gem_v_compile_call = gem_v_compile_call;
    _t1421->gem_v_compile_expr = gem_v_compile_expr;
    _t1421->gem_v_compile_table = gem_v_compile_table;
    _t1421->gem_v_defined_fns = gem_v_defined_fns;
    _t1421->gem_v_mangle = gem_v_mangle;
    _t1421->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_expr = gem_make_fn(_anon_30, _t1421);
#line 595 "compiler/main.gem"
    struct _closure__anon_31 *_t1438 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t1438->gem_v_compile_expr = gem_v_compile_expr;
    _t1438->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_31, _t1438);
#line 610 "compiler/main.gem"
    struct _closure__anon_32 *_t1452 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t1452->gem_v_compile_expr = gem_v_compile_expr;
    _t1452->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_32, _t1452);
#line 624 "compiler/main.gem"
    struct _closure__anon_33 *_t1490 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t1490->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1490->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1490->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t1490->gem_v_mangle = gem_v_mangle;
    _t1490->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_33, _t1490);
#line 652 "compiler/main.gem"
    struct _closure__anon_34 *_t1615 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t1615->gem_v_compile_expr = gem_v_compile_expr;
    _t1615->gem_v_defined_fns = gem_v_defined_fns;
    _t1615->gem_v_source_name = gem_v_source_name;
    _t1615->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_34, _t1615);
#line 798 "compiler/main.gem"
    struct _closure__anon_35 *_t1771 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t1771->gem_v_compile_expr = gem_v_compile_expr;
    _t1771->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_35, _t1771);
#line 878 "compiler/main.gem"
    struct _closure__anon_36 *_t1776 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t1776->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_36, _t1776);
#line 888 "compiler/main.gem"
    struct _closure__anon_37 *_t1936 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t1936->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1936->gem_v_compile_expr = gem_v_compile_expr;
    _t1936->gem_v_compile_if = gem_v_compile_if;
    _t1936->gem_v_compile_match = gem_v_compile_match;
    _t1936->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1936->gem_v_compile_while = gem_v_compile_while;
    _t1936->gem_v_mangle = gem_v_mangle;
    _t1936->gem_v_source_name = gem_v_source_name;
    _t1936->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_stmt = gem_make_fn(_anon_37, _t1936);
#line 959 "compiler/main.gem"
    struct _closure__anon_38 *_t2083 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2083->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2083->gem_v_compile_expr = gem_v_compile_expr;
    _t2083->gem_v_compile_if_return = gem_v_compile_if_return;
    _t2083->gem_v_compile_match_return = gem_v_compile_match_return;
    _t2083->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2083->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2083->gem_v_compile_while = gem_v_compile_while;
    _t2083->gem_v_mangle = gem_v_mangle;
    _t2083->gem_v_source_name = gem_v_source_name;
    _t2083->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_38, _t2083);
#line 1018 "compiler/main.gem"
    struct _closure__anon_39 *_t2102 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2102->gem_v_compile_expr = gem_v_compile_expr;
    _t2102->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_39, _t2102);
#line 1034 "compiler/main.gem"
    struct _closure__anon_40 *_t2138 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t2138->gem_v_compile_expr = gem_v_compile_expr;
    _t2138->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2138->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_40, _t2138);
#line 1069 "compiler/main.gem"
    struct _closure__anon_41 *_t2156 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2156->gem_v_compile_expr = gem_v_compile_expr;
    _t2156->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_41, _t2156);
#line 1084 "compiler/main.gem"
    struct _closure__anon_42 *_t2189 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t2189->gem_v_compile_expr = gem_v_compile_expr;
    _t2189->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2189->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_42, _t2189);
#line 1111 "compiler/main.gem"
    struct _closure__anon_43 *_t2239 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t2239->gem_v_compile_expr = gem_v_compile_expr;
    _t2239->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2239->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2239->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_43, _t2239);
#line 1157 "compiler/main.gem"
    struct _closure__anon_44 *_t2284 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t2284->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_44, _t2284);
#line 1236 "compiler/main.gem"
    struct _closure__anon_45 *_t2342 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t2342->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2342->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2342->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2342->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2342->gem_v_mangle = gem_v_mangle;
    _t2342->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_fn = gem_make_fn(_anon_45, _t2342);
#line 1301 "compiler/main.gem"
    struct _closure__anon_46 *_t2410 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t2410->gem_v_anon_name = gem_v_anon_name;
    _t2410->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2410->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2410->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2410->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2410->gem_v_functions = gem_v_functions;
    _t2410->gem_v_mangle = gem_v_mangle;
    _t2410->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_46, _t2410);
#line 1376 "compiler/main.gem"
    struct _closure__anon_47 *_t2458 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t2458->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2458->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t2458->gem_v_compile_fn = gem_v_compile_fn;
    _t2458->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2458->gem_v_defined_fns = gem_v_defined_fns;
    _t2458->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2458->gem_v_forward_decls = gem_v_forward_decls;
    _t2458->gem_v_functions = gem_v_functions;
    _t2458->gem_v_source_name = gem_v_source_name;
    GemVal gem_v_compile = gem_make_fn(_anon_47, _t2458);
    GemVal _t2459 = gem_table_new();
    gem_table_set(_t2459, gem_string("compile"), gem_v_compile);
    GemVal _t2460 = _t2459;
    gem_pop_frame();
    return _t2460;
}

static GemVal gem_fn_split_string(void *_env, GemVal *args, int argc) {
#line 1477 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("split_string", "compiler/main.gem", 1477);
#line 1478 "compiler/main.gem"
    GemVal _t2461 = gem_table_new();
    GemVal gem_v_result = _t2461;
#line 1479 "compiler/main.gem"
    GemVal gem_v_current = gem_string("");
    {
#line 1480 "compiler/main.gem"
        GemVal gem_v__for_i_62 = gem_int(0);
#line 1480 "compiler/main.gem"
    GemVal _t2462[] = {gem_v_s};
        GemVal gem_v__for_limit_62 = gem_len_fn(NULL, _t2462, 1);
#line 1480 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
#line 1480 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_62;
#line 1480 "compiler/main.gem"
            gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1481 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_v_delim))) {
#line 1482 "compiler/main.gem"
    GemVal _t2463[] = {gem_v_result, gem_v_current};
                (void)(gem_push_fn(NULL, _t2463, 2));
#line 1483 "compiler/main.gem"
                gem_v_current = gem_string("");
            } else {
#line 1485 "compiler/main.gem"
                gem_v_current = gem_add(gem_v_current, gem_table_get(gem_v_s, gem_v_i));
            }
        }
    }
#line 1488 "compiler/main.gem"
    GemVal _t2464[] = {gem_v_result, gem_v_current};
    (void)(gem_push_fn(NULL, _t2464, 2));
    GemVal _t2465 = gem_v_result;
    gem_pop_frame();
    return _t2465;
}

static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc) {
#line 1495 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("str_replace", "compiler/main.gem", 1495);
#line 1496 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 1497 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 1498 "compiler/main.gem"
    GemVal _t2466[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t2466, 1);
#line 1499 "compiler/main.gem"
    GemVal _t2467[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t2467, 1);
#line 1500 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
#line 1501 "compiler/main.gem"
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
#line 1502 "compiler/main.gem"
            GemVal gem_v_found = gem_bool(1);
#line 1503 "compiler/main.gem"
            GemVal gem_v_j = gem_int(0);
#line 1504 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
#line 1505 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
#line 1506 "compiler/main.gem"
                    gem_v_found = gem_bool(0);
                    break;
                }
#line 1509 "compiler/main.gem"
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
#line 1511 "compiler/main.gem"
            if (gem_truthy(gem_v_found)) {
#line 1512 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
#line 1513 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
#line 1517 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
#line 1518 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t2468 = gem_v_result;
    gem_pop_frame();
    return _t2468;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 19 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 19);
#line 20 "compiler/main.gem"
    GemVal _t2469 = gem_table_new();
    GemVal gem_v_new_stmts = _t2469;
#line 21 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
    {
#line 22 "compiler/main.gem"
    GemVal _t2470 = gem_v_ast;
        GemVal gem_v__for_items_1 = gem_table_get(_t2470, gem_string("stmts"));
#line 22 "compiler/main.gem"
        GemVal gem_v__for_i_1 = gem_int(0);
#line 22 "compiler/main.gem"
        while (1) {
            GemVal _t2471[] = {gem_v__for_items_1};
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t2471, 1)))) break;
#line 22 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 22 "compiler/main.gem"
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 23 "compiler/main.gem"
    GemVal _t2472[] = {gem_v_stmt};
    GemVal _t2474;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2472, 1), gem_string("table")))) {
        _t2474 = gem_eq(gem_type_fn(NULL, _t2472, 1), gem_string("table"));
    } else {
        GemVal _t2473 = gem_v_stmt;
        _t2474 = gem_eq(gem_table_get(_t2473, gem_string("tag")), gem_string("load"));
    }
            if (gem_truthy(_t2474)) {
#line 24 "compiler/main.gem"
    GemVal _t2475 = gem_v_stmt;
                GemVal gem_v_path = gem_table_get(_t2475, gem_string("path"));
#line 26 "compiler/main.gem"
    GemVal _t2476[] = {gem_v_path};
    GemVal _t2484;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t2476, 1), gem_int(4)))) {
        _t2484 = gem_lt(gem_len_fn(NULL, _t2476, 1), gem_int(4));
    } else {
        GemVal _t2477[] = {gem_v_path};
        GemVal _t2479;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2477, 1), gem_int(4))), gem_string(".")))) {
                _t2479 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2477, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t2478[] = {gem_v_path};
                _t2479 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2478, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t2481;
        if (gem_truthy(_t2479)) {
                _t2481 = _t2479;
        } else {
                GemVal _t2480[] = {gem_v_path};
                _t2481 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2480, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t2483;
        if (gem_truthy(_t2481)) {
                _t2483 = _t2481;
        } else {
                GemVal _t2482[] = {gem_v_path};
                _t2483 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2482, 1), gem_int(1))), gem_string("m"));
        }
        _t2484 = _t2483;
    }
                if (gem_truthy(_t2484)) {
#line 27 "compiler/main.gem"
                    gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
                }
#line 29 "compiler/main.gem"
    GemVal _t2485[] = {gem_v_base_dir, gem_v_path};
    GemVal _t2486[] = {gem_fn_gem_path_join(NULL, _t2485, 2)};
                GemVal gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t2486, 1);
#line 32 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                    continue;
                }
#line 35 "compiler/main.gem"
                gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 37 "compiler/main.gem"
    GemVal _t2487[] = {gem_v_full_path};
                GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2487, 1);
#line 38 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 39 "compiler/main.gem"
    GemVal _t2488[] = {gem_v_full_path};
    GemVal _t2489[] = {gem_add(gem_add(gem_string("cannot load '"), gem_to_string_fn(NULL, _t2488, 1)), gem_string("'"))};
                    (void)(gem_error_at_fn("compiler/main.gem", 39, _t2489, 1));
                }
#line 42 "compiler/main.gem"
    GemVal _t2490[] = {gem_v_source};
                GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t2490, 1);
#line 43 "compiler/main.gem"
    GemVal _t2491[] = {gem_v_full_path};
                GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t2491, 1);
#line 44 "compiler/main.gem"
    GemVal _t2492[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
                GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t2492, 3);
                {
#line 46 "compiler/main.gem"
    GemVal _t2493 = gem_v_resolved;
                    GemVal gem_v__for_items_2 = gem_table_get(_t2493, gem_string("stmts"));
#line 46 "compiler/main.gem"
                    GemVal gem_v__for_i_2 = gem_int(0);
#line 46 "compiler/main.gem"
                    while (1) {
                        GemVal _t2494[] = {gem_v__for_items_2};
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t2494, 1)))) break;
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
    GemVal _t2495[] = {gem_v_new_stmts};
    GemVal _t2496 = gem_fn_make_program(NULL, _t2495, 1);
    gem_pop_frame();
    return _t2496;
}

int main(void) {
    gem_push_frame("main", "compiler/main.gem", 0);
#line 60 "compiler/main.gem"
    GemVal gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
#line 61 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 62 "compiler/main.gem"
    GemVal _t2497[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 62, _t2497, 1));
    }
#line 65 "compiler/main.gem"
    GemVal _t2498[] = {gem_int(1)};
    GemVal gem_v_src_path = gem_fn_gem_get_argv(NULL, _t2498, 1);
#line 66 "compiler/main.gem"
    GemVal _t2499[] = {gem_v_src_path};
    GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2499, 1);
#line 67 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 68 "compiler/main.gem"
    GemVal _t2500[] = {gem_v_src_path};
    GemVal _t2501[] = {gem_add(gem_add(gem_string("cannot read '"), gem_to_string_fn(NULL, _t2500, 1)), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 68, _t2501, 1));
    }
#line 71 "compiler/main.gem"
    GemVal _t2502[] = {gem_v_source};
    GemVal gem_v_ast = gem_fn_parse_source(NULL, _t2502, 1);
#line 72 "compiler/main.gem"
    GemVal _t2503[] = {gem_v_src_path};
    GemVal gem_v_base_dir = gem_fn_gem_dirname(NULL, _t2503, 1);
#line 73 "compiler/main.gem"
    GemVal _t2504 = gem_table_new();
    GemVal gem_v_loaded = _t2504;
#line 74 "compiler/main.gem"
    GemVal _t2505[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    GemVal gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t2505, 3);
#line 76 "compiler/main.gem"
    GemVal _t2506[] = {gem_v_src_path};
    GemVal gem_v_cg = gem_fn_make_codegen(NULL, _t2506, 1);
#line 77 "compiler/main.gem"
    GemVal _t2507 = gem_v_cg;
    GemVal _t2508 = gem_table_get(_t2507, gem_string("compile"));
    GemVal _t2509[] = {gem_v_resolved_ast};
    GemVal gem_v_c_code = _t2508.fn(_t2508.env, _t2509, 1);
#line 78 "compiler/main.gem"
    GemVal _t2510[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t2510, 1));
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

