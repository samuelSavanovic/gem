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
static GemVal gem_fn_collect_top_let_names(void *_env, GemVal *args, int argc);
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
static GemVal gem_fn_gem_file_exists(void *_env, GemVal *args, int argc);
GemVal gem_v_install_root;
GemVal gem_v_argc;
GemVal gem_v_src_path;
GemVal gem_v_source;
GemVal gem_v_ast;
GemVal gem_v_base_dir;
GemVal gem_v_loaded;
GemVal gem_v_resolved_ast;
GemVal gem_v_cg;
GemVal gem_v_c_code;

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

static GemVal gem_fn_gem_file_exists(void *_env, GemVal *args, int argc) {
    char* _p0 = args[0].sval;
    int64_t _ret = gem_file_exists(_p0);
    return gem_int(_ret);
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
#line 112 "compiler/main.gem"
    GemVal _t104[] = {gem_int(0)};
    GemVal gem_v_NUL = gem_chr_fn(NULL, _t104, 1);
#line 114 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 115 "compiler/main.gem"
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
#line 118 "compiler/main.gem"
    GemVal _t105;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t105 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t105 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t105)) {
#line 119 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 124 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 125 "compiler/main.gem"
            while (1) {
                GemVal _t106;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t106 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t106 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t106)) break;
#line 126 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 132 "compiler/main.gem"
    GemVal _t107;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t107 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t107 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t107)) {
#line 133 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 134 "compiler/main.gem"
    GemVal _t108[] = {gem_v_tokens};
    GemVal _t111;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t108, 1), gem_int(0)))) {
        _t111 = gem_gt(gem_len_fn(NULL, _t108, 1), gem_int(0));
    } else {
        GemVal _t109[] = {gem_v_tokens};
        GemVal _t110 = gem_table_get(gem_v_tokens, gem_sub(gem_len_fn(NULL, _t109, 1), gem_int(1)));
        _t111 = gem_eq(gem_table_get(_t110, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t111)) {
#line 135 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 137 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 138 "compiler/main.gem"
    GemVal _t112 = gem_table_new();
    gem_table_set(_t112, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t112, gem_string("value"), gem_string("\n"));
    gem_table_set(_t112, gem_string("line"), gem_v_line);
    GemVal _t113[] = {gem_v_tokens, _t112};
                (void)(gem_push_fn(NULL, _t113, 2));
            }
#line 140 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 141 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
            }
#line 143 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 145 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 146 "compiler/main.gem"
                GemVal gem_v_c2 = gem_table_get(gem_v_source, gem_v_pos);
#line 147 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_c2, gem_string("\n")))) {
#line 148 "compiler/main.gem"
                    gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 149 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                } else {
#line 150 "compiler/main.gem"
    GemVal _t114;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t114 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t114 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t115;
    if (gem_truthy(_t114)) {
        _t115 = _t114;
    } else {
        _t115 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t115)) {
#line 151 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 152 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 153 "compiler/main.gem"
                            while (1) {
                                GemVal _t116;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t116 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t116 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t116)) break;
#line 154 "compiler/main.gem"
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
#line 164 "compiler/main.gem"
    GemVal _t117[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t117, 1))) {
#line 165 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 166 "compiler/main.gem"
            while (1) {
                GemVal _t119;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t119 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t118[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t119 = gem_fn_is_digit(NULL, _t118, 1);
                }
                if (!gem_truthy(_t119)) break;
#line 167 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 168 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 170 "compiler/main.gem"
    GemVal _t120;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t120 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t120 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t120;
#line 171 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 172 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 173 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 174 "compiler/main.gem"
                while (1) {
                    GemVal _t122;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t122 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t121[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t122 = gem_fn_is_digit(NULL, _t121, 1);
                    }
                    if (!gem_truthy(_t122)) break;
#line 175 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 176 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 179 "compiler/main.gem"
    GemVal _t123 = gem_table_new();
    gem_table_set(_t123, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t123, gem_string("value"), gem_v_val);
    gem_table_set(_t123, gem_string("line"), gem_v_line);
    GemVal _t124[] = {gem_v_tokens, _t123};
            (void)(gem_push_fn(NULL, _t124, 2));
            continue;
        }
#line 184 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 185 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 186 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 187 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 188 "compiler/main.gem"
            while (1) {
                GemVal _t125;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t125 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t125 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t125)) break;
#line 189 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 190 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 191 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 192 "compiler/main.gem"
    GemVal _t126[] = {gem_v_line};
    GemVal _t127[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t126, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 192, _t127, 1));
                    }
#line 194 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 195 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 196 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
#line 197 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 198 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_CR);
                        } else {
#line 199 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 200 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                            } else {
#line 201 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 202 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                                } else {
#line 203 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
#line 204 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                    } else {
#line 205 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
#line 206 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                        } else {
#line 207 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
#line 208 "compiler/main.gem"
                                                gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                            } else {
#line 209 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
#line 210 "compiler/main.gem"
                                                    gem_v_val = gem_add(gem_v_val, gem_v_NUL);
                                                } else {
#line 212 "compiler/main.gem"
                                                    gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
#line 214 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 217 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 218 "compiler/main.gem"
    GemVal _t128 = gem_table_new();
    gem_table_set(_t128, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t128, gem_string("value"), gem_string(""));
    gem_table_set(_t128, gem_string("line"), gem_v_line);
    GemVal _t129[] = {gem_v_tokens, _t128};
                    (void)(gem_push_fn(NULL, _t129, 2));
#line 219 "compiler/main.gem"
    GemVal _t130 = gem_table_new();
    gem_table_set(_t130, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t130, gem_string("value"), gem_v_val);
    gem_table_set(_t130, gem_string("line"), gem_v_line);
    GemVal _t131[] = {gem_v_tokens, _t130};
                    (void)(gem_push_fn(NULL, _t131, 2));
#line 220 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 221 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 222 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 223 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 224 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 227 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 228 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 230 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 231 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 232 "compiler/main.gem"
    GemVal _t132[] = {gem_v_line};
    GemVal _t133[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t132, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 232, _t133, 1));
                }
#line 234 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 235 "compiler/main.gem"
    GemVal _t134 = gem_table_new();
    gem_table_set(_t134, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t134, gem_string("value"), gem_v_val);
    gem_table_set(_t134, gem_string("line"), gem_v_line);
    GemVal _t135[] = {gem_v_tokens, _t134};
                (void)(gem_push_fn(NULL, _t135, 2));
            }
            continue;
        }
#line 241 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("'")))) {
#line 242 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 243 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 244 "compiler/main.gem"
            while (1) {
                GemVal _t136;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t136 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t136 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
                }
                if (!gem_truthy(_t136)) break;
#line 245 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 246 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 247 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 248 "compiler/main.gem"
    GemVal _t137[] = {gem_v_line};
    GemVal _t138[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t137, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 248, _t138, 1));
                    }
#line 250 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 251 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 252 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
#line 253 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 254 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_CR);
                        } else {
#line 255 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 256 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                            } else {
#line 257 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 258 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                                } else {
#line 259 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("'")))) {
#line 260 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("'"));
                                    } else {
#line 262 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                    }
                                }
                            }
                        }
                    }
#line 264 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 267 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 268 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 270 "compiler/main.gem"
            if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 271 "compiler/main.gem"
    GemVal _t139[] = {gem_v_line};
    GemVal _t140[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t139, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 271, _t140, 1));
            }
#line 273 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 274 "compiler/main.gem"
    GemVal _t141 = gem_table_new();
    gem_table_set(_t141, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t141, gem_string("value"), gem_v_val);
    gem_table_set(_t141, gem_string("line"), gem_v_line);
    GemVal _t142[] = {gem_v_tokens, _t141};
            (void)(gem_push_fn(NULL, _t142, 2));
            continue;
        }
#line 279 "compiler/main.gem"
    GemVal _t143[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t143, 1))) {
#line 280 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 281 "compiler/main.gem"
            while (1) {
                GemVal _t145;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t145 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t144[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t145 = gem_fn_is_alnum(NULL, _t144, 1);
                }
                if (!gem_truthy(_t145)) break;
#line 282 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 283 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 285 "compiler/main.gem"
    GemVal _t146[] = {gem_v_kw, gem_v_val};
            if (gem_truthy(gem_has_key_fn(NULL, _t146, 2))) {
#line 286 "compiler/main.gem"
    GemVal _t147 = gem_table_new();
    gem_table_set(_t147, gem_string("type"), gem_v_val);
    gem_table_set(_t147, gem_string("value"), gem_v_val);
    gem_table_set(_t147, gem_string("line"), gem_v_line);
    GemVal _t148[] = {gem_v_tokens, _t147};
                (void)(gem_push_fn(NULL, _t148, 2));
            } else {
#line 288 "compiler/main.gem"
    GemVal _t149 = gem_table_new();
    gem_table_set(_t149, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t149, gem_string("value"), gem_v_val);
    gem_table_set(_t149, gem_string("line"), gem_v_line);
    GemVal _t150[] = {gem_v_tokens, _t149};
                (void)(gem_push_fn(NULL, _t150, 2));
            }
            continue;
        }
#line 294 "compiler/main.gem"
        GemVal gem_v_next = gem_string("");
#line 295 "compiler/main.gem"
        if (gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
#line 296 "compiler/main.gem"
            gem_v_next = gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1)));
        }
#line 299 "compiler/main.gem"
    GemVal _t151;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t151 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t151 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t151)) {
#line 300 "compiler/main.gem"
    GemVal _t152 = gem_table_new();
    gem_table_set(_t152, gem_string("type"), gem_string("=="));
    gem_table_set(_t152, gem_string("value"), gem_string("=="));
    gem_table_set(_t152, gem_string("line"), gem_v_line);
    GemVal _t153[] = {gem_v_tokens, _t152};
            (void)(gem_push_fn(NULL, _t153, 2));
#line 301 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 304 "compiler/main.gem"
    GemVal _t154;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t154 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t154 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t154)) {
#line 305 "compiler/main.gem"
    GemVal _t155 = gem_table_new();
    gem_table_set(_t155, gem_string("type"), gem_string("!="));
    gem_table_set(_t155, gem_string("value"), gem_string("!="));
    gem_table_set(_t155, gem_string("line"), gem_v_line);
    GemVal _t156[] = {gem_v_tokens, _t155};
            (void)(gem_push_fn(NULL, _t156, 2));
#line 306 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 309 "compiler/main.gem"
    GemVal _t157;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t157 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t157 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t157)) {
#line 310 "compiler/main.gem"
    GemVal _t158 = gem_table_new();
    gem_table_set(_t158, gem_string("type"), gem_string("<="));
    gem_table_set(_t158, gem_string("value"), gem_string("<="));
    gem_table_set(_t158, gem_string("line"), gem_v_line);
    GemVal _t159[] = {gem_v_tokens, _t158};
            (void)(gem_push_fn(NULL, _t159, 2));
#line 311 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 314 "compiler/main.gem"
    GemVal _t160;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t160 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t160 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t160)) {
#line 315 "compiler/main.gem"
    GemVal _t161 = gem_table_new();
    gem_table_set(_t161, gem_string("type"), gem_string(">="));
    gem_table_set(_t161, gem_string("value"), gem_string(">="));
    gem_table_set(_t161, gem_string("line"), gem_v_line);
    GemVal _t162[] = {gem_v_tokens, _t161};
            (void)(gem_push_fn(NULL, _t162, 2));
#line 316 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 319 "compiler/main.gem"
    GemVal _t163;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t163 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t163 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t163)) {
#line 320 "compiler/main.gem"
    GemVal _t164 = gem_table_new();
    gem_table_set(_t164, gem_string("type"), gem_string("+="));
    gem_table_set(_t164, gem_string("value"), gem_string("+="));
    gem_table_set(_t164, gem_string("line"), gem_v_line);
    GemVal _t165[] = {gem_v_tokens, _t164};
            (void)(gem_push_fn(NULL, _t165, 2));
#line 321 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 324 "compiler/main.gem"
    GemVal _t166;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t166 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t166 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t166)) {
#line 325 "compiler/main.gem"
    GemVal _t167 = gem_table_new();
    gem_table_set(_t167, gem_string("type"), gem_string("-="));
    gem_table_set(_t167, gem_string("value"), gem_string("-="));
    gem_table_set(_t167, gem_string("line"), gem_v_line);
    GemVal _t168[] = {gem_v_tokens, _t167};
            (void)(gem_push_fn(NULL, _t168, 2));
#line 326 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 329 "compiler/main.gem"
    GemVal _t169;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t169 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t169 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t169)) {
#line 330 "compiler/main.gem"
    GemVal _t170 = gem_table_new();
    gem_table_set(_t170, gem_string("type"), gem_string("*="));
    gem_table_set(_t170, gem_string("value"), gem_string("*="));
    gem_table_set(_t170, gem_string("line"), gem_v_line);
    GemVal _t171[] = {gem_v_tokens, _t170};
            (void)(gem_push_fn(NULL, _t171, 2));
#line 331 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 334 "compiler/main.gem"
    GemVal _t172;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t172 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t172 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t172)) {
#line 335 "compiler/main.gem"
    GemVal _t173 = gem_table_new();
    gem_table_set(_t173, gem_string("type"), gem_string("/="));
    gem_table_set(_t173, gem_string("value"), gem_string("/="));
    gem_table_set(_t173, gem_string("line"), gem_v_line);
    GemVal _t174[] = {gem_v_tokens, _t173};
            (void)(gem_push_fn(NULL, _t174, 2));
#line 336 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 339 "compiler/main.gem"
    GemVal _t175;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t175 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t175 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t175)) {
#line 340 "compiler/main.gem"
    GemVal _t176 = gem_table_new();
    gem_table_set(_t176, gem_string("type"), gem_string("->"));
    gem_table_set(_t176, gem_string("value"), gem_string("->"));
    gem_table_set(_t176, gem_string("line"), gem_v_line);
    GemVal _t177[] = {gem_v_tokens, _t176};
            (void)(gem_push_fn(NULL, _t177, 2));
#line 341 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 346 "compiler/main.gem"
    GemVal _t178;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t178 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t178 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t179;
    if (gem_truthy(_t178)) {
        _t179 = _t178;
    } else {
        _t179 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t180;
    if (gem_truthy(_t179)) {
        _t180 = _t179;
    } else {
        _t180 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t181;
    if (gem_truthy(_t180)) {
        _t181 = _t180;
    } else {
        _t181 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t181)) {
#line 347 "compiler/main.gem"
    GemVal _t182 = gem_table_new();
    gem_table_set(_t182, gem_string("type"), gem_v_ch);
    gem_table_set(_t182, gem_string("value"), gem_v_ch);
    gem_table_set(_t182, gem_string("line"), gem_v_line);
    GemVal _t183[] = {gem_v_tokens, _t182};
            (void)(gem_push_fn(NULL, _t183, 2));
#line 348 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 351 "compiler/main.gem"
    GemVal _t184;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t184 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t184 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t185;
    if (gem_truthy(_t184)) {
        _t185 = _t184;
    } else {
        _t185 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t185)) {
#line 352 "compiler/main.gem"
    GemVal _t186 = gem_table_new();
    gem_table_set(_t186, gem_string("type"), gem_v_ch);
    gem_table_set(_t186, gem_string("value"), gem_v_ch);
    gem_table_set(_t186, gem_string("line"), gem_v_line);
    GemVal _t187[] = {gem_v_tokens, _t186};
            (void)(gem_push_fn(NULL, _t187, 2));
#line 353 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 357 "compiler/main.gem"
    GemVal _t188;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t188 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t188 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t188)) {
#line 358 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 359 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 360 "compiler/main.gem"
    GemVal _t189 = gem_table_new();
    gem_table_set(_t189, gem_string("type"), gem_string("{"));
    gem_table_set(_t189, gem_string("value"), gem_string("{"));
    gem_table_set(_t189, gem_string("line"), gem_v_line);
    GemVal _t190[] = {gem_v_tokens, _t189};
            (void)(gem_push_fn(NULL, _t190, 2));
#line 361 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 364 "compiler/main.gem"
    GemVal _t191;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t191 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t191 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t191)) {
#line 365 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 366 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 367 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 368 "compiler/main.gem"
    GemVal _t192 = gem_table_new();
    gem_table_set(_t192, gem_string("type"), gem_string("}"));
    gem_table_set(_t192, gem_string("value"), gem_string("}"));
    gem_table_set(_t192, gem_string("line"), gem_v_line);
    GemVal _t193[] = {gem_v_tokens, _t192};
                (void)(gem_push_fn(NULL, _t193, 2));
#line 369 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                continue;
            }
#line 373 "compiler/main.gem"
            gem_v_interp_depth = gem_sub(gem_v_interp_depth, gem_int(1));
#line 374 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 375 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 376 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 377 "compiler/main.gem"
            while (1) {
                GemVal _t194;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t194 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t194 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t194)) break;
#line 378 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 379 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 380 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 381 "compiler/main.gem"
    GemVal _t195[] = {gem_v_line};
    GemVal _t196[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t195, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 381, _t196, 1));
                    }
#line 383 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 384 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 385 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
#line 386 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 387 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_CR);
                        } else {
#line 388 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 389 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                            } else {
#line 390 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 391 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                                } else {
#line 392 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
#line 393 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                    } else {
#line 394 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
#line 395 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                        } else {
#line 396 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
#line 397 "compiler/main.gem"
                                                gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                            } else {
#line 398 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
#line 399 "compiler/main.gem"
                                                    gem_v_val = gem_add(gem_v_val, gem_v_NUL);
                                                } else {
#line 401 "compiler/main.gem"
                                                    gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
#line 403 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 406 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 408 "compiler/main.gem"
    GemVal _t197 = gem_table_new();
    gem_table_set(_t197, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t197, gem_string("value"), gem_v_val);
    gem_table_set(_t197, gem_string("line"), gem_v_line);
    GemVal _t198[] = {gem_v_tokens, _t197};
                    (void)(gem_push_fn(NULL, _t198, 2));
#line 409 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 410 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 411 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 412 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 413 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 416 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 417 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 419 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 420 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 421 "compiler/main.gem"
    GemVal _t199[] = {gem_v_line};
    GemVal _t200[] = {gem_add(gem_string("unterminated string interpolation at line "), gem_to_string_fn(NULL, _t199, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 421, _t200, 1));
                }
#line 423 "compiler/main.gem"
    GemVal _t201 = gem_table_new();
    gem_table_set(_t201, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t201, gem_string("value"), gem_v_val);
    gem_table_set(_t201, gem_string("line"), gem_v_line);
    GemVal _t202[] = {gem_v_tokens, _t201};
                (void)(gem_push_fn(NULL, _t202, 2));
#line 424 "compiler/main.gem"
    GemVal _t203 = gem_table_new();
    gem_table_set(_t203, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t203, gem_string("value"), gem_string(""));
    gem_table_set(_t203, gem_string("line"), gem_v_line);
    GemVal _t204[] = {gem_v_tokens, _t203};
                (void)(gem_push_fn(NULL, _t204, 2));
#line 425 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 430 "compiler/main.gem"
    GemVal _t205;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t205 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t205 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t206;
    if (gem_truthy(_t205)) {
        _t206 = _t205;
    } else {
        _t206 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t207;
    if (gem_truthy(_t206)) {
        _t207 = _t206;
    } else {
        _t207 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t208;
    if (gem_truthy(_t207)) {
        _t208 = _t207;
    } else {
        _t208 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t209;
    if (gem_truthy(_t208)) {
        _t209 = _t208;
    } else {
        _t209 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t209)) {
#line 431 "compiler/main.gem"
    GemVal _t210 = gem_table_new();
    gem_table_set(_t210, gem_string("type"), gem_v_ch);
    gem_table_set(_t210, gem_string("value"), gem_v_ch);
    gem_table_set(_t210, gem_string("line"), gem_v_line);
    GemVal _t211[] = {gem_v_tokens, _t210};
            (void)(gem_push_fn(NULL, _t211, 2));
#line 432 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 435 "compiler/main.gem"
    GemVal _t212;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t212 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t212 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t213;
    if (gem_truthy(_t212)) {
        _t213 = _t212;
    } else {
        _t213 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t214;
    if (gem_truthy(_t213)) {
        _t214 = _t213;
    } else {
        _t214 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t214)) {
#line 436 "compiler/main.gem"
    GemVal _t215 = gem_table_new();
    gem_table_set(_t215, gem_string("type"), gem_v_ch);
    gem_table_set(_t215, gem_string("value"), gem_v_ch);
    gem_table_set(_t215, gem_string("line"), gem_v_line);
    GemVal _t216[] = {gem_v_tokens, _t215};
            (void)(gem_push_fn(NULL, _t216, 2));
#line 437 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 441 "compiler/main.gem"
    GemVal _t217[] = {gem_v_ch};
    GemVal _t218[] = {gem_v_line};
    GemVal _t219[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t217, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t218, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 441, _t219, 1));
    }
#line 444 "compiler/main.gem"
    GemVal _t220 = gem_table_new();
    gem_table_set(_t220, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t220, gem_string("value"), gem_string(""));
    gem_table_set(_t220, gem_string("line"), gem_v_line);
    GemVal _t221[] = {gem_v_tokens, _t220};
    (void)(gem_push_fn(NULL, _t221, 2));
    GemVal _t222 = gem_v_tokens;
    gem_pop_frame();
    return _t222;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 10 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_source", "compiler/main.gem", 10);
#line 11 "compiler/main.gem"
    GemVal _t223[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t223, 1);
#line 12 "compiler/main.gem"
    GemVal _t224[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t224, 1);
#line 13 "compiler/main.gem"
    GemVal _t225 = gem_v_p;
    GemVal _t226 = gem_table_get(_t225, gem_string("parse"));
    GemVal _t227 = _t226.fn(_t226.env, NULL, 0);
    gem_pop_frame();
    return _t227;
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
    GemVal _t228 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t228;
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
    GemVal _t230 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t230;
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
    GemVal _t232 = gem_v_t;
    gem_pop_frame();
    return _t232;
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
    GemVal _t234 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t235 = gem_eq(gem_table_get(_t234, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t235;
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
    GemVal _t237 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t237, gem_string("type")), gem_v_tp))) {
#line 46 "compiler/main.gem"
    GemVal _t238[] = {gem_v_tp};
    GemVal _t239 = gem_v_t;
    GemVal _t240[] = {gem_table_get(_t239, gem_string("type"))};
    GemVal _t241 = gem_v_t;
    GemVal _t242[] = {gem_table_get(_t241, gem_string("line"))};
    GemVal _t243[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t238, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t240, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t242, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 46, _t243, 1));
    }
#line 48 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t244 = gem_v_t;
    gem_pop_frame();
    return _t244;
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
        GemVal _t246 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t246, gem_string("type")), gem_string("NEWLINE")))) break;
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
    GemVal _t248 = gem_table_new();
    GemVal gem_v_params = _t248;
#line 62 "compiler/main.gem"
    GemVal _t249 = (*gem_v_peek);
    GemVal _t250 = _t249.fn(_t249.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t250, gem_string("type")), gem_string("NAME")))) {
#line 63 "compiler/main.gem"
    GemVal _t251 = (*gem_v_advance);
    GemVal _t252 = _t251.fn(_t251.env, NULL, 0);
    GemVal _t253[] = {gem_v_params, gem_table_get(_t252, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t253, 2));
#line 64 "compiler/main.gem"
        while (1) {
            GemVal _t254 = (*gem_v_peek);
            GemVal _t255 = _t254.fn(_t254.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t255, gem_string("type")), gem_string(",")))) break;
#line 65 "compiler/main.gem"
    GemVal _t256 = (*gem_v_advance);
            (void)(_t256.fn(_t256.env, NULL, 0));
#line 66 "compiler/main.gem"
    GemVal _t257 = (*gem_v_expect);
    GemVal _t258[] = {gem_string("NAME")};
    GemVal _t259 = _t257.fn(_t257.env, _t258, 1);
    GemVal _t260[] = {gem_v_params, gem_table_get(_t259, gem_string("value"))};
            (void)(gem_push_fn(NULL, _t260, 2));
        }
    }
    GemVal _t261 = gem_v_params;
    gem_pop_frame();
    return _t261;
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
    GemVal _t263 = gem_table_new();
    GemVal gem_v_stmts = _t263;
#line 85 "compiler/main.gem"
    GemVal _t264 = (*gem_v_skip_nl);
    (void)(_t264.fn(_t264.env, NULL, 0));
#line 86 "compiler/main.gem"
    while (1) {
        GemVal _t265 = (*gem_v_peek);
        GemVal _t266 = _t265.fn(_t265.env, NULL, 0);
        GemVal _t269;
        if (!gem_truthy(gem_neq(gem_table_get(_t266, gem_string("type")), gem_string("end")))) {
                _t269 = gem_neq(gem_table_get(_t266, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t267 = (*gem_v_peek);
                GemVal _t268 = _t267.fn(_t267.env, NULL, 0);
                _t269 = gem_neq(gem_table_get(_t268, gem_string("type")), gem_string("elif"));
        }
        GemVal _t272;
        if (!gem_truthy(_t269)) {
                _t272 = _t269;
        } else {
                GemVal _t270 = (*gem_v_peek);
                GemVal _t271 = _t270.fn(_t270.env, NULL, 0);
                _t272 = gem_neq(gem_table_get(_t271, gem_string("type")), gem_string("else"));
        }
        GemVal _t275;
        if (!gem_truthy(_t272)) {
                _t275 = _t272;
        } else {
                GemVal _t273 = (*gem_v_peek);
                GemVal _t274 = _t273.fn(_t273.env, NULL, 0);
                _t275 = gem_neq(gem_table_get(_t274, gem_string("type")), gem_string("when"));
        }
        GemVal _t277;
        if (!gem_truthy(_t275)) {
                _t277 = _t275;
        } else {
                GemVal _t276 = (*gem_v_at_end);
                _t277 = gem_not(_t276.fn(_t276.env, NULL, 0));
        }
        if (!gem_truthy(_t277)) break;
#line 87 "compiler/main.gem"
    GemVal _t278 = (*gem_v_parse_stmt);
    GemVal _t279[] = {gem_v_stmts, _t278.fn(_t278.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t279, 2));
#line 88 "compiler/main.gem"
    GemVal _t280 = (*gem_v_skip_nl);
        (void)(_t280.fn(_t280.env, NULL, 0));
    }
    GemVal _t281 = gem_v_stmts;
    gem_pop_frame();
    return _t281;
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
    GemVal _t283 = gem_table_new();
    GemVal gem_v_stmts = _t283;
#line 98 "compiler/main.gem"
    GemVal _t284 = (*gem_v_skip_nl);
    (void)(_t284.fn(_t284.env, NULL, 0));
#line 99 "compiler/main.gem"
    while (1) {
        GemVal _t285 = (*gem_v_peek);
        GemVal _t286 = _t285.fn(_t285.env, NULL, 0);
        GemVal _t288;
        if (!gem_truthy(gem_neq(gem_table_get(_t286, gem_string("type")), gem_string("end")))) {
                _t288 = gem_neq(gem_table_get(_t286, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t287 = (*gem_v_at_end);
                _t288 = gem_not(_t287.fn(_t287.env, NULL, 0));
        }
        if (!gem_truthy(_t288)) break;
#line 100 "compiler/main.gem"
    GemVal _t289 = (*gem_v_parse_stmt);
    GemVal _t290[] = {gem_v_stmts, _t289.fn(_t289.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t290, 2));
#line 101 "compiler/main.gem"
    GemVal _t291 = (*gem_v_skip_nl);
        (void)(_t291.fn(_t291.env, NULL, 0));
    }
    GemVal _t292 = gem_v_stmts;
    gem_pop_frame();
    return _t292;
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
    GemVal _t294 = (*gem_v_peek);
    GemVal gem_v_t = _t294.fn(_t294.env, NULL, 0);
#line 113 "compiler/main.gem"
    GemVal _t295 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t295, gem_string("type")), gem_string("NUMBER")))) {
#line 114 "compiler/main.gem"
    GemVal _t296 = (*gem_v_advance);
        (void)(_t296.fn(_t296.env, NULL, 0));
#line 115 "compiler/main.gem"
    GemVal _t297 = gem_v_t;
    GemVal _t298[] = {gem_table_get(_t297, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t298, 1))) {
#line 116 "compiler/main.gem"
    GemVal _t299 = gem_v_t;
    GemVal _t300[] = {gem_table_get(_t299, gem_string("value"))};
    GemVal _t301[] = {gem_fn_atof(NULL, _t300, 1)};
            GemVal _t302 = gem_fn_make_float(NULL, _t301, 1);
            gem_pop_frame();
            return _t302;
        }
#line 118 "compiler/main.gem"
    GemVal _t303 = gem_v_t;
    GemVal _t304[] = {gem_table_get(_t303, gem_string("value"))};
    GemVal _t305[] = {gem_fn_str_to_int(NULL, _t304, 1)};
        GemVal _t306 = gem_fn_make_int(NULL, _t305, 1);
        gem_pop_frame();
        return _t306;
    }
#line 122 "compiler/main.gem"
    GemVal _t307 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t307, gem_string("type")), gem_string("STRING")))) {
#line 123 "compiler/main.gem"
    GemVal _t308 = (*gem_v_advance);
        (void)(_t308.fn(_t308.env, NULL, 0));
#line 124 "compiler/main.gem"
    GemVal _t309 = gem_v_t;
    GemVal _t310[] = {gem_table_get(_t309, gem_string("value"))};
        GemVal _t311 = gem_fn_make_string(NULL, _t310, 1);
        gem_pop_frame();
        return _t311;
    }
#line 128 "compiler/main.gem"
    GemVal _t312 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t312, gem_string("type")), gem_string("INTERP_START")))) {
#line 129 "compiler/main.gem"
    GemVal _t313 = (*gem_v_advance);
        (void)(_t313.fn(_t313.env, NULL, 0));
#line 130 "compiler/main.gem"
    GemVal _t314 = gem_table_new();
        GemVal gem_v_parts = _t314;
#line 131 "compiler/main.gem"
    GemVal _t315 = (*gem_v_skip_nl);
        (void)(_t315.fn(_t315.env, NULL, 0));
#line 132 "compiler/main.gem"
        while (1) {
            GemVal _t316 = (*gem_v_peek);
            GemVal _t317 = _t316.fn(_t316.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t317, gem_string("type")), gem_string("INTERP_END")))) break;
#line 133 "compiler/main.gem"
    GemVal _t318 = (*gem_v_peek);
    GemVal _t319 = _t318.fn(_t318.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t319, gem_string("type")), gem_string("STRING")))) {
#line 134 "compiler/main.gem"
    GemVal _t320 = (*gem_v_advance);
    GemVal _t321 = _t320.fn(_t320.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t321, gem_string("value"));
#line 135 "compiler/main.gem"
    GemVal _t322[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t322, 1), gem_int(0)))) {
#line 136 "compiler/main.gem"
    GemVal _t323[] = {gem_v_sval};
    GemVal _t324[] = {gem_v_parts, gem_fn_make_string(NULL, _t323, 1)};
                    (void)(gem_push_fn(NULL, _t324, 2));
                }
            } else {
#line 139 "compiler/main.gem"
    GemVal _t325 = (*gem_v_parse_expr);
    GemVal _t326[] = {gem_v_parts, _t325.fn(_t325.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t326, 2));
            }
#line 141 "compiler/main.gem"
    GemVal _t327 = (*gem_v_skip_nl);
            (void)(_t327.fn(_t327.env, NULL, 0));
        }
#line 143 "compiler/main.gem"
    GemVal _t328 = (*gem_v_expect);
    GemVal _t329[] = {gem_string("INTERP_END")};
        (void)(_t328.fn(_t328.env, _t329, 1));
#line 144 "compiler/main.gem"
    GemVal _t330[] = {gem_v_parts};
        GemVal _t331 = gem_fn_make_interp(NULL, _t330, 1);
        gem_pop_frame();
        return _t331;
    }
#line 148 "compiler/main.gem"
    GemVal _t332 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t332, gem_string("type")), gem_string("true")))) {
#line 149 "compiler/main.gem"
    GemVal _t333 = (*gem_v_advance);
        (void)(_t333.fn(_t333.env, NULL, 0));
#line 150 "compiler/main.gem"
    GemVal _t334[] = {gem_bool(1)};
        GemVal _t335 = gem_fn_make_bool(NULL, _t334, 1);
        gem_pop_frame();
        return _t335;
    }
#line 152 "compiler/main.gem"
    GemVal _t336 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t336, gem_string("type")), gem_string("false")))) {
#line 153 "compiler/main.gem"
    GemVal _t337 = (*gem_v_advance);
        (void)(_t337.fn(_t337.env, NULL, 0));
#line 154 "compiler/main.gem"
    GemVal _t338[] = {gem_bool(0)};
        GemVal _t339 = gem_fn_make_bool(NULL, _t338, 1);
        gem_pop_frame();
        return _t339;
    }
#line 158 "compiler/main.gem"
    GemVal _t340 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t340, gem_string("type")), gem_string("nil")))) {
#line 159 "compiler/main.gem"
    GemVal _t341 = (*gem_v_advance);
        (void)(_t341.fn(_t341.env, NULL, 0));
#line 160 "compiler/main.gem"
        GemVal _t342 = gem_fn_make_nil_node(NULL, NULL, 0);
        gem_pop_frame();
        return _t342;
    }
#line 164 "compiler/main.gem"
    GemVal _t343 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t343, gem_string("type")), gem_string("fn")))) {
#line 165 "compiler/main.gem"
    GemVal _t344 = (*gem_v_advance);
        (void)(_t344.fn(_t344.env, NULL, 0));
#line 166 "compiler/main.gem"
    GemVal _t345 = (*gem_v_expect);
    GemVal _t346[] = {gem_string("(")};
        (void)(_t345.fn(_t345.env, _t346, 1));
#line 167 "compiler/main.gem"
    GemVal _t347 = (*gem_v_parse_params);
        GemVal gem_v_params = _t347.fn(_t347.env, NULL, 0);
#line 168 "compiler/main.gem"
    GemVal _t348 = (*gem_v_expect);
    GemVal _t349[] = {gem_string(")")};
        (void)(_t348.fn(_t348.env, _t349, 1));
#line 169 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 170 "compiler/main.gem"
    GemVal _t350 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t350.fn(_t350.env, NULL, 0);
#line 171 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 172 "compiler/main.gem"
    GemVal _t351 = (*gem_v_expect);
    GemVal _t352[] = {gem_string("end")};
        (void)(_t351.fn(_t351.env, _t352, 1));
#line 173 "compiler/main.gem"
    GemVal _t353[] = {gem_v_params, gem_v_body};
        GemVal _t354 = gem_fn_make_anon_fn(NULL, _t353, 2);
        gem_pop_frame();
        return _t354;
    }
#line 178 "compiler/main.gem"
    GemVal _t355 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t355, gem_string("type")), gem_string("{")))) {
#line 179 "compiler/main.gem"
    GemVal _t356 = (*gem_v_advance);
        (void)(_t356.fn(_t356.env, NULL, 0));
#line 180 "compiler/main.gem"
    GemVal _t357 = (*gem_v_skip_nl);
        (void)(_t357.fn(_t357.env, NULL, 0));
#line 181 "compiler/main.gem"
    GemVal _t358 = gem_table_new();
        GemVal gem_v_entries = _t358;
#line 182 "compiler/main.gem"
        while (1) {
            GemVal _t359 = (*gem_v_peek);
            GemVal _t360 = _t359.fn(_t359.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t360, gem_string("type")), gem_string("}")))) break;
#line 183 "compiler/main.gem"
    GemVal _t361 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t361.fn(_t361.env, NULL, 0);
#line 184 "compiler/main.gem"
    GemVal _t362 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t362, gem_string("type")), gem_string("NAME")))) {
#line 185 "compiler/main.gem"
    GemVal _t363 = (*gem_v_advance);
                (void)(_t363.fn(_t363.env, NULL, 0));
            } else {
#line 186 "compiler/main.gem"
    GemVal _t364 = (*gem_v_peek_at);
    GemVal _t365[] = {gem_int(1)};
    GemVal _t366 = _t364.fn(_t364.env, _t365, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t366, gem_string("type")), gem_string(":")))) {
#line 188 "compiler/main.gem"
    GemVal _t367 = (*gem_v_advance);
                    (void)(_t367.fn(_t367.env, NULL, 0));
                } else {
#line 190 "compiler/main.gem"
    GemVal _t368 = gem_v_key_tok;
    GemVal _t369[] = {gem_table_get(_t368, gem_string("line"))};
    GemVal _t370[] = {gem_add(gem_string("expected table key at line "), gem_to_string_fn(NULL, _t369, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 190, _t370, 1));
                }
            }
#line 192 "compiler/main.gem"
    GemVal _t371 = (*gem_v_expect);
    GemVal _t372[] = {gem_string(":")};
            (void)(_t371.fn(_t371.env, _t372, 1));
#line 193 "compiler/main.gem"
    GemVal _t373 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t373.fn(_t373.env, NULL, 0);
#line 194 "compiler/main.gem"
    GemVal _t374 = gem_v_key_tok;
    GemVal _t375[] = {gem_table_get(_t374, gem_string("value")), gem_v_val};
    GemVal _t376[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t375, 2)};
            (void)(gem_push_fn(NULL, _t376, 2));
#line 195 "compiler/main.gem"
    GemVal _t377 = (*gem_v_skip_nl);
            (void)(_t377.fn(_t377.env, NULL, 0));
#line 196 "compiler/main.gem"
    GemVal _t378 = (*gem_v_peek);
    GemVal _t379 = _t378.fn(_t378.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t379, gem_string("type")), gem_string(",")))) {
#line 197 "compiler/main.gem"
    GemVal _t380 = (*gem_v_advance);
                (void)(_t380.fn(_t380.env, NULL, 0));
#line 198 "compiler/main.gem"
    GemVal _t381 = (*gem_v_skip_nl);
                (void)(_t381.fn(_t381.env, NULL, 0));
            }
        }
#line 201 "compiler/main.gem"
    GemVal _t382 = (*gem_v_expect);
    GemVal _t383[] = {gem_string("}")};
        (void)(_t382.fn(_t382.env, _t383, 1));
#line 202 "compiler/main.gem"
    GemVal _t384[] = {gem_v_entries};
        GemVal _t385 = gem_fn_make_table(NULL, _t384, 1);
        gem_pop_frame();
        return _t385;
    }
#line 206 "compiler/main.gem"
    GemVal _t386 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t386, gem_string("type")), gem_string("[")))) {
#line 207 "compiler/main.gem"
    GemVal _t387 = (*gem_v_advance);
        (void)(_t387.fn(_t387.env, NULL, 0));
#line 208 "compiler/main.gem"
    GemVal _t388 = (*gem_v_skip_nl);
        (void)(_t388.fn(_t388.env, NULL, 0));
#line 209 "compiler/main.gem"
    GemVal _t389 = gem_table_new();
        GemVal gem_v_elements = _t389;
#line 210 "compiler/main.gem"
        while (1) {
            GemVal _t390 = (*gem_v_peek);
            GemVal _t391 = _t390.fn(_t390.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t391, gem_string("type")), gem_string("]")))) break;
#line 211 "compiler/main.gem"
    GemVal _t392 = (*gem_v_parse_expr);
    GemVal _t393[] = {gem_v_elements, _t392.fn(_t392.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t393, 2));
#line 212 "compiler/main.gem"
    GemVal _t394 = (*gem_v_skip_nl);
            (void)(_t394.fn(_t394.env, NULL, 0));
#line 213 "compiler/main.gem"
    GemVal _t395 = (*gem_v_peek);
    GemVal _t396 = _t395.fn(_t395.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t396, gem_string("type")), gem_string(",")))) {
#line 214 "compiler/main.gem"
    GemVal _t397 = (*gem_v_advance);
                (void)(_t397.fn(_t397.env, NULL, 0));
#line 215 "compiler/main.gem"
    GemVal _t398 = (*gem_v_skip_nl);
                (void)(_t398.fn(_t398.env, NULL, 0));
            }
        }
#line 218 "compiler/main.gem"
    GemVal _t399 = (*gem_v_expect);
    GemVal _t400[] = {gem_string("]")};
        (void)(_t399.fn(_t399.env, _t400, 1));
#line 219 "compiler/main.gem"
    GemVal _t401[] = {gem_v_elements};
        GemVal _t402 = gem_fn_make_array(NULL, _t401, 1);
        gem_pop_frame();
        return _t402;
    }
#line 223 "compiler/main.gem"
    GemVal _t403 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t403, gem_string("type")), gem_string("(")))) {
#line 224 "compiler/main.gem"
    GemVal _t404 = (*gem_v_advance);
        (void)(_t404.fn(_t404.env, NULL, 0));
#line 225 "compiler/main.gem"
    GemVal _t405 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t405.fn(_t405.env, NULL, 0);
#line 226 "compiler/main.gem"
    GemVal _t406 = (*gem_v_expect);
    GemVal _t407[] = {gem_string(")")};
        (void)(_t406.fn(_t406.env, _t407, 1));
#line 227 "compiler/main.gem"
        GemVal _t408 = gem_v_e;
        gem_pop_frame();
        return _t408;
    }
#line 231 "compiler/main.gem"
    GemVal _t409 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t409, gem_string("type")), gem_string("NAME")))) {
#line 232 "compiler/main.gem"
    GemVal _t410 = (*gem_v_advance);
        (void)(_t410.fn(_t410.env, NULL, 0));
#line 233 "compiler/main.gem"
    GemVal _t411 = gem_v_t;
    GemVal _t412[] = {gem_table_get(_t411, gem_string("value"))};
        GemVal _t413 = gem_fn_make_var(NULL, _t412, 1);
        gem_pop_frame();
        return _t413;
    }
#line 236 "compiler/main.gem"
    GemVal _t414 = gem_v_t;
    GemVal _t415[] = {gem_table_get(_t414, gem_string("type"))};
    GemVal _t416 = gem_v_t;
    GemVal _t417[] = {gem_table_get(_t416, gem_string("line"))};
    GemVal _t418[] = {gem_add(gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t415, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t417, 1))};
    GemVal _t419 = gem_error_at_fn("compiler/main.gem", 236, _t418, 1);
    gem_pop_frame();
    return _t419;
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
    GemVal _t421 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t421.fn(_t421.env, NULL, 0);
#line 243 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
#line 245 "compiler/main.gem"
    GemVal _t422 = (*gem_v_peek);
    GemVal _t423 = _t422.fn(_t422.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t423, gem_string("type")), gem_string("(")))) {
#line 246 "compiler/main.gem"
    GemVal _t424 = (*gem_v_peek);
    GemVal _t425 = _t424.fn(_t424.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t425, gem_string("line"));
#line 247 "compiler/main.gem"
    GemVal _t426 = (*gem_v_advance);
            (void)(_t426.fn(_t426.env, NULL, 0));
#line 248 "compiler/main.gem"
    GemVal _t427 = (*gem_v_skip_nl);
            (void)(_t427.fn(_t427.env, NULL, 0));
#line 249 "compiler/main.gem"
    GemVal _t428 = gem_table_new();
            GemVal gem_v_args = _t428;
#line 250 "compiler/main.gem"
    GemVal _t429 = (*gem_v_peek);
    GemVal _t430 = _t429.fn(_t429.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t430, gem_string("type")), gem_string(")")))) {
#line 251 "compiler/main.gem"
    GemVal _t431 = (*gem_v_parse_expr);
    GemVal _t432[] = {gem_v_args, _t431.fn(_t431.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t432, 2));
#line 252 "compiler/main.gem"
                while (1) {
                    GemVal _t433 = (*gem_v_peek);
                    GemVal _t434 = _t433.fn(_t433.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t434, gem_string("type")), gem_string(",")))) break;
#line 253 "compiler/main.gem"
    GemVal _t435 = (*gem_v_advance);
                    (void)(_t435.fn(_t435.env, NULL, 0));
#line 254 "compiler/main.gem"
    GemVal _t436 = (*gem_v_skip_nl);
                    (void)(_t436.fn(_t436.env, NULL, 0));
#line 255 "compiler/main.gem"
    GemVal _t437 = (*gem_v_parse_expr);
    GemVal _t438[] = {gem_v_args, _t437.fn(_t437.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t438, 2));
                }
            }
#line 258 "compiler/main.gem"
    GemVal _t439 = (*gem_v_skip_nl);
            (void)(_t439.fn(_t439.env, NULL, 0));
#line 259 "compiler/main.gem"
    GemVal _t440 = (*gem_v_expect);
    GemVal _t441[] = {gem_string(")")};
            (void)(_t440.fn(_t440.env, _t441, 1));
#line 262 "compiler/main.gem"
    GemVal _t442 = (*gem_v_peek);
    GemVal _t443 = _t442.fn(_t442.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t443, gem_string("type")), gem_string("do")))) {
#line 263 "compiler/main.gem"
    GemVal _t444 = (*gem_v_advance);
                (void)(_t444.fn(_t444.env, NULL, 0));
#line 264 "compiler/main.gem"
    GemVal _t445 = gem_table_new();
                GemVal gem_v_bparams = _t445;
#line 265 "compiler/main.gem"
    GemVal _t446 = (*gem_v_peek);
    GemVal _t447 = _t446.fn(_t446.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t447, gem_string("type")), gem_string("|")))) {
#line 266 "compiler/main.gem"
    GemVal _t448 = (*gem_v_advance);
                    (void)(_t448.fn(_t448.env, NULL, 0));
#line 267 "compiler/main.gem"
    GemVal _t449 = (*gem_v_parse_params);
                    gem_v_bparams = _t449.fn(_t449.env, NULL, 0);
#line 268 "compiler/main.gem"
    GemVal _t450 = (*gem_v_expect);
    GemVal _t451[] = {gem_string("|")};
                    (void)(_t450.fn(_t450.env, _t451, 1));
                }
#line 270 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 271 "compiler/main.gem"
    GemVal _t452 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t452.fn(_t452.env, NULL, 0);
#line 272 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 273 "compiler/main.gem"
    GemVal _t453 = (*gem_v_expect);
    GemVal _t454[] = {gem_string("end")};
                (void)(_t453.fn(_t453.env, _t454, 1));
#line 274 "compiler/main.gem"
    GemVal _t455[] = {gem_v_bparams, gem_v_body};
    GemVal _t456[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t455, 2)};
                (void)(gem_push_fn(NULL, _t456, 2));
            } else {
#line 278 "compiler/main.gem"
    GemVal _t457 = (*gem_v_peek);
    GemVal _t458 = _t457.fn(_t457.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t458, gem_string("type")), gem_string("{")))) {
#line 279 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 280 "compiler/main.gem"
    GemVal _t459 = (*gem_v_peek_at);
    GemVal _t460[] = {gem_int(1)};
    GemVal _t461 = _t459.fn(_t459.env, _t460, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t461, gem_string("type")), gem_string("|")))) {
#line 281 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 282 "compiler/main.gem"
    GemVal _t462 = (*gem_v_peek_at);
    GemVal _t463[] = {gem_int(1)};
    GemVal _t464 = _t462.fn(_t462.env, _t463, 1);
    GemVal _t472;
    if (gem_truthy(gem_eq(gem_table_get(_t464, gem_string("type")), gem_string("}")))) {
        _t472 = gem_eq(gem_table_get(_t464, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t465 = (*gem_v_peek_at);
        GemVal _t466[] = {gem_int(1)};
        GemVal _t467 = _t465.fn(_t465.env, _t466, 1);
        GemVal _t471;
        if (!gem_truthy(gem_eq(gem_table_get(_t467, gem_string("type")), gem_string("NAME")))) {
                _t471 = gem_eq(gem_table_get(_t467, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t468 = (*gem_v_peek_at);
                GemVal _t469[] = {gem_int(2)};
                GemVal _t470 = _t468.fn(_t468.env, _t469, 1);
                _t471 = gem_eq(gem_table_get(_t470, gem_string("type")), gem_string(":"));
        }
        _t472 = _t471;
    }
                        if (gem_truthy(_t472)) {
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
    GemVal _t473 = (*gem_v_advance);
                        (void)(_t473.fn(_t473.env, NULL, 0));
#line 289 "compiler/main.gem"
    GemVal _t474 = gem_table_new();
                        GemVal gem_v_bparams = _t474;
#line 290 "compiler/main.gem"
    GemVal _t475 = (*gem_v_peek);
    GemVal _t476 = _t475.fn(_t475.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t476, gem_string("type")), gem_string("|")))) {
#line 291 "compiler/main.gem"
    GemVal _t477 = (*gem_v_advance);
                            (void)(_t477.fn(_t477.env, NULL, 0));
#line 292 "compiler/main.gem"
    GemVal _t478 = (*gem_v_parse_params);
                            gem_v_bparams = _t478.fn(_t478.env, NULL, 0);
#line 293 "compiler/main.gem"
    GemVal _t479 = (*gem_v_expect);
    GemVal _t480[] = {gem_string("|")};
                            (void)(_t479.fn(_t479.env, _t480, 1));
                        }
#line 295 "compiler/main.gem"
    GemVal _t481 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t481.fn(_t481.env, NULL, 0);
#line 296 "compiler/main.gem"
    GemVal _t482 = (*gem_v_expect);
    GemVal _t483[] = {gem_string("}")};
                        (void)(_t482.fn(_t482.env, _t483, 1));
#line 297 "compiler/main.gem"
    GemVal _t484 = gem_table_new();
    gem_table_set(_t484, gem_int(0), gem_v_bexpr);
    GemVal _t485[] = {gem_v_bparams, _t484};
    GemVal _t486[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t485, 2)};
                        (void)(gem_push_fn(NULL, _t486, 2));
                    }
                }
            }
#line 302 "compiler/main.gem"
    GemVal _t487[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t487, 3);
            continue;
        }
#line 307 "compiler/main.gem"
    GemVal _t488 = (*gem_v_peek);
    GemVal _t489 = _t488.fn(_t488.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t489, gem_string("type")), gem_string(".")))) {
#line 308 "compiler/main.gem"
    GemVal _t490 = (*gem_v_advance);
            (void)(_t490.fn(_t490.env, NULL, 0));
#line 309 "compiler/main.gem"
    GemVal _t491 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t491.fn(_t491.env, NULL, 0);
#line 310 "compiler/main.gem"
    GemVal _t492 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t492, gem_string("type")), gem_string("NAME")))) {
#line 311 "compiler/main.gem"
    GemVal _t493 = (*gem_v_advance);
                (void)(_t493.fn(_t493.env, NULL, 0));
            } else {
#line 314 "compiler/main.gem"
    GemVal _t494 = (*gem_v_advance);
                (void)(_t494.fn(_t494.env, NULL, 0));
            }
#line 316 "compiler/main.gem"
    GemVal _t495 = gem_v_field_tok;
    GemVal _t496[] = {gem_v_node, gem_table_get(_t495, gem_string("value"))};
            gem_v_node = gem_fn_make_dot(NULL, _t496, 2);
            continue;
        }
#line 321 "compiler/main.gem"
    GemVal _t497 = (*gem_v_peek);
    GemVal _t498 = _t497.fn(_t497.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t498, gem_string("type")), gem_string("[")))) {
#line 322 "compiler/main.gem"
    GemVal _t499 = (*gem_v_advance);
            (void)(_t499.fn(_t499.env, NULL, 0));
#line 323 "compiler/main.gem"
    GemVal _t500 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t500.fn(_t500.env, NULL, 0);
#line 324 "compiler/main.gem"
    GemVal _t501 = (*gem_v_expect);
    GemVal _t502[] = {gem_string("]")};
            (void)(_t501.fn(_t501.env, _t502, 1));
#line 325 "compiler/main.gem"
    GemVal _t503[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t503, 2);
            continue;
        }
        break;
    }
    GemVal _t504 = gem_v_node;
    gem_pop_frame();
    return _t504;
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
    GemVal _t506 = (*gem_v_peek);
    GemVal _t507 = _t506.fn(_t506.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t507, gem_string("type")), gem_string("-")))) {
#line 338 "compiler/main.gem"
    GemVal _t508 = (*gem_v_advance);
        (void)(_t508.fn(_t508.env, NULL, 0));
#line 339 "compiler/main.gem"
    GemVal _t509 = (*gem_v_parse_unary);
    GemVal _t510[] = {gem_string("-"), _t509.fn(_t509.env, NULL, 0)};
        GemVal _t511 = gem_fn_make_unop(NULL, _t510, 2);
        gem_pop_frame();
        return _t511;
    }
#line 341 "compiler/main.gem"
    GemVal _t512 = (*gem_v_parse_call);
    GemVal _t513 = _t512.fn(_t512.env, NULL, 0);
    gem_pop_frame();
    return _t513;
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
    GemVal _t515 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t515.fn(_t515.env, NULL, 0);
#line 347 "compiler/main.gem"
    while (1) {
        GemVal _t516 = (*gem_v_peek);
        GemVal _t517 = _t516.fn(_t516.env, NULL, 0);
        GemVal _t520;
        if (gem_truthy(gem_eq(gem_table_get(_t517, gem_string("type")), gem_string("*")))) {
                _t520 = gem_eq(gem_table_get(_t517, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t518 = (*gem_v_peek);
                GemVal _t519 = _t518.fn(_t518.env, NULL, 0);
                _t520 = gem_eq(gem_table_get(_t519, gem_string("type")), gem_string("/"));
        }
        GemVal _t523;
        if (gem_truthy(_t520)) {
                _t523 = _t520;
        } else {
                GemVal _t521 = (*gem_v_peek);
                GemVal _t522 = _t521.fn(_t521.env, NULL, 0);
                _t523 = gem_eq(gem_table_get(_t522, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t523)) break;
#line 348 "compiler/main.gem"
    GemVal _t524 = (*gem_v_advance);
    GemVal _t525 = _t524.fn(_t524.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t525, gem_string("type"));
#line 349 "compiler/main.gem"
    GemVal _t526 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t526.fn(_t526.env, NULL, 0);
#line 350 "compiler/main.gem"
    GemVal _t527[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t527, 3);
    }
    GemVal _t528 = gem_v_left;
    gem_pop_frame();
    return _t528;
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
    GemVal _t530 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t530.fn(_t530.env, NULL, 0);
#line 358 "compiler/main.gem"
    while (1) {
        GemVal _t531 = (*gem_v_peek);
        GemVal _t532 = _t531.fn(_t531.env, NULL, 0);
        GemVal _t535;
        if (gem_truthy(gem_eq(gem_table_get(_t532, gem_string("type")), gem_string("+")))) {
                _t535 = gem_eq(gem_table_get(_t532, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t533 = (*gem_v_peek);
                GemVal _t534 = _t533.fn(_t533.env, NULL, 0);
                _t535 = gem_eq(gem_table_get(_t534, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t535)) break;
#line 359 "compiler/main.gem"
    GemVal _t536 = (*gem_v_advance);
    GemVal _t537 = _t536.fn(_t536.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t537, gem_string("type"));
#line 360 "compiler/main.gem"
    GemVal _t538 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t538.fn(_t538.env, NULL, 0);
#line 361 "compiler/main.gem"
    GemVal _t539[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t539, 3);
    }
    GemVal _t540 = gem_v_left;
    gem_pop_frame();
    return _t540;
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
    GemVal _t542 = (*gem_v_parse_add);
    GemVal gem_v_left = _t542.fn(_t542.env, NULL, 0);
#line 369 "compiler/main.gem"
    while (1) {
        GemVal _t543 = (*gem_v_peek);
        GemVal _t544 = _t543.fn(_t543.env, NULL, 0);
        GemVal _t547;
        if (gem_truthy(gem_eq(gem_table_get(_t544, gem_string("type")), gem_string("==")))) {
                _t547 = gem_eq(gem_table_get(_t544, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t545 = (*gem_v_peek);
                GemVal _t546 = _t545.fn(_t545.env, NULL, 0);
                _t547 = gem_eq(gem_table_get(_t546, gem_string("type")), gem_string("!="));
        }
        GemVal _t550;
        if (gem_truthy(_t547)) {
                _t550 = _t547;
        } else {
                GemVal _t548 = (*gem_v_peek);
                GemVal _t549 = _t548.fn(_t548.env, NULL, 0);
                _t550 = gem_eq(gem_table_get(_t549, gem_string("type")), gem_string("<"));
        }
        GemVal _t553;
        if (gem_truthy(_t550)) {
                _t553 = _t550;
        } else {
                GemVal _t551 = (*gem_v_peek);
                GemVal _t552 = _t551.fn(_t551.env, NULL, 0);
                _t553 = gem_eq(gem_table_get(_t552, gem_string("type")), gem_string(">"));
        }
        GemVal _t556;
        if (gem_truthy(_t553)) {
                _t556 = _t553;
        } else {
                GemVal _t554 = (*gem_v_peek);
                GemVal _t555 = _t554.fn(_t554.env, NULL, 0);
                _t556 = gem_eq(gem_table_get(_t555, gem_string("type")), gem_string("<="));
        }
        GemVal _t559;
        if (gem_truthy(_t556)) {
                _t559 = _t556;
        } else {
                GemVal _t557 = (*gem_v_peek);
                GemVal _t558 = _t557.fn(_t557.env, NULL, 0);
                _t559 = gem_eq(gem_table_get(_t558, gem_string("type")), gem_string(">="));
        }
        GemVal _t562;
        if (gem_truthy(_t559)) {
                _t562 = _t559;
        } else {
                GemVal _t560 = (*gem_v_peek);
                GemVal _t561 = _t560.fn(_t560.env, NULL, 0);
                _t562 = gem_eq(gem_table_get(_t561, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t562)) break;
#line 370 "compiler/main.gem"
    GemVal _t563 = (*gem_v_advance);
    GemVal _t564 = _t563.fn(_t563.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t564, gem_string("type"));
#line 371 "compiler/main.gem"
    GemVal _t565 = (*gem_v_parse_add);
        GemVal gem_v_right = _t565.fn(_t565.env, NULL, 0);
#line 372 "compiler/main.gem"
    GemVal _t566[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t566, 3);
    }
    GemVal _t567 = gem_v_left;
    gem_pop_frame();
    return _t567;
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
    GemVal _t569 = (*gem_v_peek);
    GemVal _t570 = _t569.fn(_t569.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t570, gem_string("type")), gem_string("not")))) {
#line 380 "compiler/main.gem"
    GemVal _t571 = (*gem_v_advance);
        (void)(_t571.fn(_t571.env, NULL, 0));
#line 381 "compiler/main.gem"
    GemVal _t572 = (*gem_v_parse_not);
    GemVal _t573[] = {gem_string("not"), _t572.fn(_t572.env, NULL, 0)};
        GemVal _t574 = gem_fn_make_unop(NULL, _t573, 2);
        gem_pop_frame();
        return _t574;
    }
#line 383 "compiler/main.gem"
    GemVal _t575 = (*gem_v_parse_compare);
    GemVal _t576 = _t575.fn(_t575.env, NULL, 0);
    gem_pop_frame();
    return _t576;
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
    GemVal _t578 = (*gem_v_parse_not);
    GemVal gem_v_left = _t578.fn(_t578.env, NULL, 0);
#line 389 "compiler/main.gem"
    while (1) {
        GemVal _t579 = (*gem_v_peek);
        GemVal _t580 = _t579.fn(_t579.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t580, gem_string("type")), gem_string("and")))) break;
#line 390 "compiler/main.gem"
    GemVal _t581 = (*gem_v_advance);
        (void)(_t581.fn(_t581.env, NULL, 0));
#line 391 "compiler/main.gem"
    GemVal _t582 = (*gem_v_parse_not);
        GemVal gem_v_right = _t582.fn(_t582.env, NULL, 0);
#line 392 "compiler/main.gem"
    GemVal _t583[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t583, 3);
    }
    GemVal _t584 = gem_v_left;
    gem_pop_frame();
    return _t584;
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
    GemVal _t586 = (*gem_v_parse_and);
    GemVal gem_v_left = _t586.fn(_t586.env, NULL, 0);
#line 400 "compiler/main.gem"
    while (1) {
        GemVal _t587 = (*gem_v_peek);
        GemVal _t588 = _t587.fn(_t587.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t588, gem_string("type")), gem_string("or")))) break;
#line 401 "compiler/main.gem"
    GemVal _t589 = (*gem_v_advance);
        (void)(_t589.fn(_t589.env, NULL, 0));
#line 402 "compiler/main.gem"
    GemVal _t590 = (*gem_v_parse_and);
        GemVal gem_v_right = _t590.fn(_t590.env, NULL, 0);
#line 403 "compiler/main.gem"
    GemVal _t591[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t591, 3);
    }
    GemVal _t592 = gem_v_left;
    gem_pop_frame();
    return _t592;
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
    GemVal _t594 = (*gem_v_peek);
    GemVal _t595 = _t594.fn(_t594.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t595, gem_string("line"));
#line 411 "compiler/main.gem"
    GemVal _t596 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t596.fn(_t596.env, NULL, 0);
#line 414 "compiler/main.gem"
    GemVal _t597 = (*gem_v_peek);
    GemVal _t598 = _t597.fn(_t597.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t598, gem_string("type")), gem_string("=")))) {
#line 415 "compiler/main.gem"
    GemVal _t599 = (*gem_v_advance);
        (void)(_t599.fn(_t599.env, NULL, 0));
#line 416 "compiler/main.gem"
    GemVal _t600 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t600.fn(_t600.env, NULL, 0);
#line 417 "compiler/main.gem"
    GemVal _t601 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t601, gem_string("tag")), gem_string("var")))) {
#line 418 "compiler/main.gem"
    GemVal _t602 = gem_v_lhs;
    GemVal _t603[] = {gem_table_get(_t602, gem_string("name")), gem_v_value, gem_v_start_line};
            GemVal _t604 = gem_fn_make_assign(NULL, _t603, 3);
            gem_pop_frame();
            return _t604;
        }
#line 420 "compiler/main.gem"
    GemVal _t605 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t605, gem_string("tag")), gem_string("dot")))) {
#line 421 "compiler/main.gem"
    GemVal _t606 = gem_v_lhs;
    GemVal _t607 = gem_v_lhs;
    GemVal _t608[] = {gem_table_get(_t606, gem_string("object")), gem_table_get(_t607, gem_string("field")), gem_v_value, gem_v_start_line};
            GemVal _t609 = gem_fn_make_dot_assign(NULL, _t608, 4);
            gem_pop_frame();
            return _t609;
        }
#line 423 "compiler/main.gem"
    GemVal _t610 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t610, gem_string("tag")), gem_string("index")))) {
#line 424 "compiler/main.gem"
    GemVal _t611 = gem_v_lhs;
    GemVal _t612 = gem_v_lhs;
    GemVal _t613[] = {gem_table_get(_t611, gem_string("object")), gem_table_get(_t612, gem_string("key")), gem_v_value, gem_v_start_line};
            GemVal _t614 = gem_fn_make_index_assign(NULL, _t613, 4);
            gem_pop_frame();
            return _t614;
        }
#line 426 "compiler/main.gem"
    GemVal _t615 = (*gem_v_peek);
    GemVal _t616 = _t615.fn(_t615.env, NULL, 0);
    GemVal _t617[] = {gem_table_get(_t616, gem_string("line"))};
    GemVal _t618[] = {gem_add(gem_string("invalid assignment target at line "), gem_to_string_fn(NULL, _t617, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 426, _t618, 1));
    }
#line 430 "compiler/main.gem"
    GemVal _t619 = (*gem_v_peek);
    GemVal _t620 = _t619.fn(_t619.env, NULL, 0);
    GemVal _t623;
    if (gem_truthy(gem_eq(gem_table_get(_t620, gem_string("type")), gem_string("+=")))) {
        _t623 = gem_eq(gem_table_get(_t620, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t621 = (*gem_v_peek);
        GemVal _t622 = _t621.fn(_t621.env, NULL, 0);
        _t623 = gem_eq(gem_table_get(_t622, gem_string("type")), gem_string("-="));
    }
    GemVal _t626;
    if (gem_truthy(_t623)) {
        _t626 = _t623;
    } else {
        GemVal _t624 = (*gem_v_peek);
        GemVal _t625 = _t624.fn(_t624.env, NULL, 0);
        _t626 = gem_eq(gem_table_get(_t625, gem_string("type")), gem_string("*="));
    }
    GemVal _t629;
    if (gem_truthy(_t626)) {
        _t629 = _t626;
    } else {
        GemVal _t627 = (*gem_v_peek);
        GemVal _t628 = _t627.fn(_t627.env, NULL, 0);
        _t629 = gem_eq(gem_table_get(_t628, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t629)) {
#line 431 "compiler/main.gem"
    GemVal _t630 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t630.fn(_t630.env, NULL, 0);
#line 432 "compiler/main.gem"
    GemVal _t631 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t631, gem_string("value")), gem_int(0));
#line 433 "compiler/main.gem"
    GemVal _t632 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t632.fn(_t632.env, NULL, 0);
#line 434 "compiler/main.gem"
    GemVal _t633 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t633, gem_string("tag")), gem_string("var")))) {
#line 435 "compiler/main.gem"
    GemVal _t634 = gem_v_op_tok;
    GemVal _t635[] = {gem_table_get(_t634, gem_string("line"))};
    GemVal _t636[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t635, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 435, _t636, 1));
        }
#line 437 "compiler/main.gem"
    GemVal _t637 = gem_v_lhs;
    GemVal _t638 = gem_v_lhs;
    GemVal _t639[] = {gem_table_get(_t638, gem_string("name"))};
    GemVal _t640[] = {gem_v_base_op, gem_fn_make_var(NULL, _t639, 1), gem_v_value};
    GemVal _t641[] = {gem_table_get(_t637, gem_string("name")), gem_fn_make_binop(NULL, _t640, 3), gem_v_start_line};
        GemVal _t642 = gem_fn_make_assign(NULL, _t641, 3);
        gem_pop_frame();
        return _t642;
    }
    GemVal _t643 = gem_v_lhs;
    gem_pop_frame();
    return _t643;
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
    GemVal _t645 = (*gem_v_peek);
    GemVal gem_v_t = _t645.fn(_t645.env, NULL, 0);
#line 452 "compiler/main.gem"
    GemVal _t646 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t646, gem_string("type")), gem_string("let")))) {
#line 453 "compiler/main.gem"
    GemVal _t647 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t647, gem_string("line"));
#line 454 "compiler/main.gem"
    GemVal _t648 = (*gem_v_advance);
        (void)(_t648.fn(_t648.env, NULL, 0));
#line 455 "compiler/main.gem"
    GemVal _t649 = (*gem_v_expect);
    GemVal _t650[] = {gem_string("NAME")};
    GemVal _t651 = _t649.fn(_t649.env, _t650, 1);
        GemVal gem_v_name = gem_table_get(_t651, gem_string("value"));
#line 456 "compiler/main.gem"
    GemVal _t652 = (*gem_v_expect);
    GemVal _t653[] = {gem_string("=")};
        (void)(_t652.fn(_t652.env, _t653, 1));
#line 457 "compiler/main.gem"
    GemVal _t654 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t654.fn(_t654.env, NULL, 0);
#line 458 "compiler/main.gem"
    GemVal _t655[] = {gem_v_name, gem_v_value, gem_v_line};
        GemVal _t656 = gem_fn_make_let(NULL, _t655, 3);
        gem_pop_frame();
        return _t656;
    }
#line 462 "compiler/main.gem"
    GemVal _t657 = gem_v_t;
    GemVal _t661;
    if (!gem_truthy(gem_eq(gem_table_get(_t657, gem_string("type")), gem_string("fn")))) {
        _t661 = gem_eq(gem_table_get(_t657, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t658 = (*gem_v_peek_at);
        GemVal _t659[] = {gem_int(1)};
        GemVal _t660 = _t658.fn(_t658.env, _t659, 1);
        _t661 = gem_eq(gem_table_get(_t660, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t661)) {
#line 463 "compiler/main.gem"
    GemVal _t662 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t662, gem_string("line"));
#line 464 "compiler/main.gem"
    GemVal _t663 = (*gem_v_advance);
        (void)(_t663.fn(_t663.env, NULL, 0));
#line 465 "compiler/main.gem"
    GemVal _t664 = (*gem_v_expect);
    GemVal _t665[] = {gem_string("NAME")};
    GemVal _t666 = _t664.fn(_t664.env, _t665, 1);
        GemVal gem_v_name = gem_table_get(_t666, gem_string("value"));
#line 466 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 467 "compiler/main.gem"
    GemVal _t667[] = {gem_v_name};
    GemVal _t668[] = {gem_v_line};
    GemVal _t669[] = {gem_v_name};
    GemVal _t670[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("named fn '"), gem_to_string_fn(NULL, _t667, 1)), gem_string("' inside function body is not supported at line ")), gem_to_string_fn(NULL, _t668, 1)), gem_string(" — use: let ")), gem_to_string_fn(NULL, _t669, 1)), gem_string(" = fn(...) ... end"))};
            (void)(gem_error_at_fn("compiler/main.gem", 467, _t670, 1));
        }
#line 469 "compiler/main.gem"
    GemVal _t671 = (*gem_v_expect);
    GemVal _t672[] = {gem_string("(")};
        (void)(_t671.fn(_t671.env, _t672, 1));
#line 470 "compiler/main.gem"
    GemVal _t673 = (*gem_v_parse_params);
        GemVal gem_v_params = _t673.fn(_t673.env, NULL, 0);
#line 471 "compiler/main.gem"
    GemVal _t674 = (*gem_v_expect);
    GemVal _t675[] = {gem_string(")")};
        (void)(_t674.fn(_t674.env, _t675, 1));
#line 472 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 473 "compiler/main.gem"
    GemVal _t676 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t676.fn(_t676.env, NULL, 0);
#line 474 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 475 "compiler/main.gem"
    GemVal _t677 = (*gem_v_expect);
    GemVal _t678[] = {gem_string("end")};
        (void)(_t677.fn(_t677.env, _t678, 1));
#line 476 "compiler/main.gem"
    GemVal _t679[] = {gem_v_name, gem_v_params, gem_v_body, gem_v_line};
        GemVal _t680 = gem_fn_make_fn_def(NULL, _t679, 4);
        gem_pop_frame();
        return _t680;
    }
#line 480 "compiler/main.gem"
    GemVal _t681 = gem_v_t;
    GemVal _t683;
    if (gem_truthy(gem_eq(gem_table_get(_t681, gem_string("type")), gem_string("if")))) {
        _t683 = gem_eq(gem_table_get(_t681, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t682 = gem_v_t;
        _t683 = gem_eq(gem_table_get(_t682, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t683)) {
#line 481 "compiler/main.gem"
    GemVal _t684 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t684, gem_string("line"));
#line 482 "compiler/main.gem"
    GemVal _t685 = (*gem_v_advance);
        (void)(_t685.fn(_t685.env, NULL, 0));
#line 483 "compiler/main.gem"
    GemVal _t686 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t686.fn(_t686.env, NULL, 0);
#line 484 "compiler/main.gem"
    GemVal _t687 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t687.fn(_t687.env, NULL, 0);
#line 485 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 486 "compiler/main.gem"
    GemVal _t688 = (*gem_v_peek);
    GemVal _t689 = _t688.fn(_t688.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t689, gem_string("type")), gem_string("elif")))) {
#line 488 "compiler/main.gem"
    GemVal _t690 = gem_table_new();
    GemVal _t691 = (*gem_v_parse_stmt);
    gem_table_set(_t690, gem_int(0), _t691.fn(_t691.env, NULL, 0));
            gem_v_else_body = _t690;
        } else {
#line 489 "compiler/main.gem"
    GemVal _t692 = (*gem_v_peek);
    GemVal _t693 = _t692.fn(_t692.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t693, gem_string("type")), gem_string("else")))) {
#line 490 "compiler/main.gem"
    GemVal _t694 = (*gem_v_advance);
                (void)(_t694.fn(_t694.env, NULL, 0));
#line 491 "compiler/main.gem"
    GemVal _t695 = (*gem_v_parse_body);
                gem_v_else_body = _t695.fn(_t695.env, NULL, 0);
            }
        }
#line 494 "compiler/main.gem"
    GemVal _t696 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t696, gem_string("type")), gem_string("if")))) {
#line 495 "compiler/main.gem"
    GemVal _t697 = (*gem_v_expect);
    GemVal _t698[] = {gem_string("end")};
            (void)(_t697.fn(_t697.env, _t698, 1));
        }
#line 497 "compiler/main.gem"
    GemVal _t699[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        GemVal _t700 = gem_fn_make_if(NULL, _t699, 4);
        gem_pop_frame();
        return _t700;
    }
#line 501 "compiler/main.gem"
    GemVal _t701 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t701, gem_string("type")), gem_string("while")))) {
#line 502 "compiler/main.gem"
    GemVal _t702 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t702, gem_string("line"));
#line 503 "compiler/main.gem"
    GemVal _t703 = (*gem_v_advance);
        (void)(_t703.fn(_t703.env, NULL, 0));
#line 504 "compiler/main.gem"
    GemVal _t704 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t704.fn(_t704.env, NULL, 0);
#line 505 "compiler/main.gem"
    GemVal _t705 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t705.fn(_t705.env, NULL, 0);
#line 506 "compiler/main.gem"
    GemVal _t706 = (*gem_v_expect);
    GemVal _t707[] = {gem_string("end")};
        (void)(_t706.fn(_t706.env, _t707, 1));
#line 507 "compiler/main.gem"
    GemVal _t708[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        GemVal _t709 = gem_fn_make_while(NULL, _t708, 3);
        gem_pop_frame();
        return _t709;
    }
#line 511 "compiler/main.gem"
    GemVal _t710 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t710, gem_string("type")), gem_string("for")))) {
#line 512 "compiler/main.gem"
    GemVal _t711 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t711, gem_string("line"));
#line 513 "compiler/main.gem"
    GemVal _t712 = (*gem_v_advance);
        (void)(_t712.fn(_t712.env, NULL, 0));
#line 514 "compiler/main.gem"
    GemVal _t713 = (*gem_v_expect);
    GemVal _t714[] = {gem_string("NAME")};
    GemVal _t715 = _t713.fn(_t713.env, _t714, 1);
        GemVal gem_v_var_name = gem_table_get(_t715, gem_string("value"));
#line 516 "compiler/main.gem"
    GemVal _t716 = (*gem_v_peek);
    GemVal _t717 = _t716.fn(_t716.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t717, gem_string("type")), gem_string("in")))) {
#line 518 "compiler/main.gem"
    GemVal _t718 = (*gem_v_advance);
            (void)(_t718.fn(_t718.env, NULL, 0));
#line 519 "compiler/main.gem"
    GemVal _t719 = (*gem_v_parse_expr);
            GemVal gem_v_items_expr = _t719.fn(_t719.env, NULL, 0);
#line 520 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 521 "compiler/main.gem"
    GemVal _t720[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t720, 1));
#line 522 "compiler/main.gem"
    GemVal _t721[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t721, 1));
#line 523 "compiler/main.gem"
    GemVal _t722 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t722.fn(_t722.env, NULL, 0);
#line 524 "compiler/main.gem"
    GemVal _t723 = (*gem_v_expect);
    GemVal _t724[] = {gem_string("end")};
            (void)(_t723.fn(_t723.env, _t724, 1));
#line 525 "compiler/main.gem"
    GemVal _t725 = gem_table_new();
            GemVal gem_v_inner_stmts = _t725;
#line 526 "compiler/main.gem"
    GemVal _t726[] = {gem_v_items_var};
    GemVal _t727[] = {gem_v_idx_var};
    GemVal _t728[] = {gem_fn_make_var(NULL, _t726, 1), gem_fn_make_var(NULL, _t727, 1)};
    GemVal _t729[] = {gem_v_var_name, gem_fn_make_index(NULL, _t728, 2), gem_v_line};
    GemVal _t730[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t729, 3)};
            (void)(gem_push_fn(NULL, _t730, 2));
#line 527 "compiler/main.gem"
    GemVal _t731[] = {gem_v_idx_var};
    GemVal _t732[] = {gem_int(1)};
    GemVal _t733[] = {gem_string("+"), gem_fn_make_var(NULL, _t731, 1), gem_fn_make_int(NULL, _t732, 1)};
    GemVal _t734[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t733, 3), gem_v_line};
    GemVal _t735[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t734, 3)};
            (void)(gem_push_fn(NULL, _t735, 2));
            {
#line 528 "compiler/main.gem"
                GemVal gem_v__for_i_1 = gem_int(0);
#line 528 "compiler/main.gem"
    GemVal _t736[] = {gem_v_fbody};
                GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t736, 1);
#line 528 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 528 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_1;
#line 528 "compiler/main.gem"
                    gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 529 "compiler/main.gem"
    GemVal _t737[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t737, 2));
                }
            }
#line 531 "compiler/main.gem"
    GemVal _t738[] = {gem_v_idx_var};
    GemVal _t739[] = {gem_string("len")};
    GemVal _t740 = gem_table_new();
    GemVal _t741[] = {gem_v_items_var};
    gem_table_set(_t740, gem_int(0), gem_fn_make_var(NULL, _t741, 1));
    GemVal _t742[] = {gem_fn_make_var(NULL, _t739, 1), _t740, gem_int(0)};
    GemVal _t743[] = {gem_string("<"), gem_fn_make_var(NULL, _t738, 1), gem_fn_make_call(NULL, _t742, 3)};
    GemVal _t744[] = {gem_fn_make_binop(NULL, _t743, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t744, 3);
#line 536 "compiler/main.gem"
    GemVal _t745 = gem_table_new();
    gem_table_set(_t745, gem_string("tag"), gem_string("block"));
    GemVal _t746 = gem_table_new();
    GemVal _t747[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t746, gem_int(0), gem_fn_make_let(NULL, _t747, 3));
    GemVal _t748[] = {gem_int(0)};
    GemVal _t749[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t748, 1), gem_v_line};
    gem_table_set(_t746, gem_int(1), gem_fn_make_let(NULL, _t749, 3));
    gem_table_set(_t746, gem_int(2), gem_v_while_node);
    gem_table_set(_t745, gem_string("stmts"), _t746);
            GemVal _t750 = _t745;
            gem_pop_frame();
            return _t750;
        } else {
#line 541 "compiler/main.gem"
    GemVal _t751 = (*gem_v_peek);
    GemVal _t752 = _t751.fn(_t751.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t752, gem_string("type")), gem_string("=")))) {
#line 543 "compiler/main.gem"
    GemVal _t753 = (*gem_v_advance);
                (void)(_t753.fn(_t753.env, NULL, 0));
#line 544 "compiler/main.gem"
    GemVal _t754 = (*gem_v_parse_expr);
                GemVal gem_v_start_expr = _t754.fn(_t754.env, NULL, 0);
#line 545 "compiler/main.gem"
    GemVal _t755 = (*gem_v_expect);
    GemVal _t756[] = {gem_string(",")};
                (void)(_t755.fn(_t755.env, _t756, 1));
#line 546 "compiler/main.gem"
    GemVal _t757 = (*gem_v_parse_expr);
                GemVal gem_v_end_expr = _t757.fn(_t757.env, NULL, 0);
#line 547 "compiler/main.gem"
    GemVal _t758 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t758.fn(_t758.env, NULL, 0);
#line 548 "compiler/main.gem"
    GemVal _t759 = (*gem_v_expect);
    GemVal _t760[] = {gem_string("end")};
                (void)(_t759.fn(_t759.env, _t760, 1));
#line 549 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 550 "compiler/main.gem"
    GemVal _t761[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t761, 1));
#line 551 "compiler/main.gem"
    GemVal _t762[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t762, 1));
#line 552 "compiler/main.gem"
    GemVal _t763 = gem_table_new();
                GemVal gem_v_inner_stmts = _t763;
#line 553 "compiler/main.gem"
    GemVal _t764[] = {gem_v_idx_var};
    GemVal _t765[] = {gem_v_var_name, gem_fn_make_var(NULL, _t764, 1), gem_v_line};
    GemVal _t766[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t765, 3)};
                (void)(gem_push_fn(NULL, _t766, 2));
#line 554 "compiler/main.gem"
    GemVal _t767[] = {gem_v_idx_var};
    GemVal _t768[] = {gem_int(1)};
    GemVal _t769[] = {gem_string("+"), gem_fn_make_var(NULL, _t767, 1), gem_fn_make_int(NULL, _t768, 1)};
    GemVal _t770[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t769, 3), gem_v_line};
    GemVal _t771[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t770, 3)};
                (void)(gem_push_fn(NULL, _t771, 2));
                {
#line 555 "compiler/main.gem"
                    GemVal gem_v__for_i_2 = gem_int(0);
#line 555 "compiler/main.gem"
    GemVal _t772[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t772, 1);
#line 555 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 555 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_2;
#line 555 "compiler/main.gem"
                        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 556 "compiler/main.gem"
    GemVal _t773[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t773, 2));
                    }
                }
#line 558 "compiler/main.gem"
    GemVal _t774[] = {gem_v_idx_var};
    GemVal _t775[] = {gem_v_limit_var};
    GemVal _t776[] = {gem_string("<"), gem_fn_make_var(NULL, _t774, 1), gem_fn_make_var(NULL, _t775, 1)};
    GemVal _t777[] = {gem_fn_make_binop(NULL, _t776, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t777, 3);
#line 563 "compiler/main.gem"
    GemVal _t778 = gem_table_new();
    gem_table_set(_t778, gem_string("tag"), gem_string("block"));
    GemVal _t779 = gem_table_new();
    GemVal _t780[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t779, gem_int(0), gem_fn_make_let(NULL, _t780, 3));
    GemVal _t781[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t779, gem_int(1), gem_fn_make_let(NULL, _t781, 3));
    gem_table_set(_t779, gem_int(2), gem_v_while_node);
    gem_table_set(_t778, gem_string("stmts"), _t779);
                GemVal _t782 = _t778;
                gem_pop_frame();
                return _t782;
            } else {
#line 569 "compiler/main.gem"
    GemVal _t783 = (*gem_v_peek);
    GemVal _t784 = _t783.fn(_t783.env, NULL, 0);
    GemVal _t785[] = {gem_table_get(_t784, gem_string("line"))};
    GemVal _t786[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t785, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 569, _t786, 1));
            }
        }
    }
#line 574 "compiler/main.gem"
    GemVal _t787 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t787, gem_string("type")), gem_string("match")))) {
#line 575 "compiler/main.gem"
    GemVal _t788 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t788, gem_string("line"));
#line 576 "compiler/main.gem"
    GemVal _t789 = (*gem_v_advance);
        (void)(_t789.fn(_t789.env, NULL, 0));
#line 577 "compiler/main.gem"
    GemVal _t790 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t790.fn(_t790.env, NULL, 0);
#line 578 "compiler/main.gem"
    GemVal _t791 = (*gem_v_skip_nl);
        (void)(_t791.fn(_t791.env, NULL, 0));
#line 579 "compiler/main.gem"
    GemVal _t792 = gem_table_new();
        GemVal gem_v_whens = _t792;
#line 580 "compiler/main.gem"
        while (1) {
            GemVal _t793 = (*gem_v_peek);
            GemVal _t794 = _t793.fn(_t793.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t794, gem_string("type")), gem_string("when")))) break;
#line 581 "compiler/main.gem"
    GemVal _t795 = (*gem_v_advance);
            (void)(_t795.fn(_t795.env, NULL, 0));
#line 582 "compiler/main.gem"
    GemVal _t796 = (*gem_v_parse_expr);
            GemVal gem_v_wval = _t796.fn(_t796.env, NULL, 0);
#line 583 "compiler/main.gem"
    GemVal _t797 = (*gem_v_parse_body);
            GemVal gem_v_wbody = _t797.fn(_t797.env, NULL, 0);
#line 584 "compiler/main.gem"
    GemVal _t798[] = {gem_v_wval, gem_v_wbody};
    GemVal _t799[] = {gem_v_whens, gem_fn_make_when(NULL, _t798, 2)};
            (void)(gem_push_fn(NULL, _t799, 2));
#line 585 "compiler/main.gem"
    GemVal _t800 = (*gem_v_skip_nl);
            (void)(_t800.fn(_t800.env, NULL, 0));
        }
#line 587 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 588 "compiler/main.gem"
    GemVal _t801 = (*gem_v_peek);
    GemVal _t802 = _t801.fn(_t801.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t802, gem_string("type")), gem_string("else")))) {
#line 589 "compiler/main.gem"
    GemVal _t803 = (*gem_v_advance);
            (void)(_t803.fn(_t803.env, NULL, 0));
#line 590 "compiler/main.gem"
    GemVal _t804 = (*gem_v_parse_body);
            gem_v_else_body = _t804.fn(_t804.env, NULL, 0);
        }
#line 592 "compiler/main.gem"
    GemVal _t805 = (*gem_v_expect);
    GemVal _t806[] = {gem_string("end")};
        (void)(_t805.fn(_t805.env, _t806, 1));
#line 593 "compiler/main.gem"
    GemVal _t807[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line};
        GemVal _t808 = gem_fn_make_match(NULL, _t807, 4);
        gem_pop_frame();
        return _t808;
    }
#line 597 "compiler/main.gem"
    GemVal _t809 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t809, gem_string("type")), gem_string("return")))) {
#line 598 "compiler/main.gem"
    GemVal _t810 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t810, gem_string("line"));
#line 599 "compiler/main.gem"
    GemVal _t811 = (*gem_v_advance);
        (void)(_t811.fn(_t811.env, NULL, 0));
#line 600 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 601 "compiler/main.gem"
    GemVal _t812 = (*gem_v_peek);
    GemVal _t813 = _t812.fn(_t812.env, NULL, 0);
    GemVal _t816;
    if (!gem_truthy(gem_neq(gem_table_get(_t813, gem_string("type")), gem_string("NEWLINE")))) {
        _t816 = gem_neq(gem_table_get(_t813, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t814 = (*gem_v_peek);
        GemVal _t815 = _t814.fn(_t814.env, NULL, 0);
        _t816 = gem_neq(gem_table_get(_t815, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t819;
    if (!gem_truthy(_t816)) {
        _t819 = _t816;
    } else {
        GemVal _t817 = (*gem_v_peek);
        GemVal _t818 = _t817.fn(_t817.env, NULL, 0);
        _t819 = gem_neq(gem_table_get(_t818, gem_string("type")), gem_string("end"));
    }
    GemVal _t822;
    if (!gem_truthy(_t819)) {
        _t822 = _t819;
    } else {
        GemVal _t820 = (*gem_v_peek);
        GemVal _t821 = _t820.fn(_t820.env, NULL, 0);
        _t822 = gem_neq(gem_table_get(_t821, gem_string("type")), gem_string("elif"));
    }
    GemVal _t825;
    if (!gem_truthy(_t822)) {
        _t825 = _t822;
    } else {
        GemVal _t823 = (*gem_v_peek);
        GemVal _t824 = _t823.fn(_t823.env, NULL, 0);
        _t825 = gem_neq(gem_table_get(_t824, gem_string("type")), gem_string("else"));
    }
    GemVal _t828;
    if (!gem_truthy(_t825)) {
        _t828 = _t825;
    } else {
        GemVal _t826 = (*gem_v_peek);
        GemVal _t827 = _t826.fn(_t826.env, NULL, 0);
        _t828 = gem_neq(gem_table_get(_t827, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t828)) {
#line 602 "compiler/main.gem"
    GemVal _t829 = (*gem_v_parse_expr);
            gem_v_value = _t829.fn(_t829.env, NULL, 0);
        }
#line 604 "compiler/main.gem"
    GemVal _t830[] = {gem_v_value, gem_v_line};
        GemVal _t831 = gem_fn_make_return(NULL, _t830, 2);
        gem_pop_frame();
        return _t831;
    }
#line 608 "compiler/main.gem"
    GemVal _t832 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t832, gem_string("type")), gem_string("break")))) {
#line 609 "compiler/main.gem"
    GemVal _t833 = (*gem_v_advance);
        (void)(_t833.fn(_t833.env, NULL, 0));
#line 610 "compiler/main.gem"
        GemVal _t834 = gem_fn_make_break(NULL, NULL, 0);
        gem_pop_frame();
        return _t834;
    }
#line 614 "compiler/main.gem"
    GemVal _t835 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t835, gem_string("type")), gem_string("continue")))) {
#line 615 "compiler/main.gem"
    GemVal _t836 = (*gem_v_advance);
        (void)(_t836.fn(_t836.env, NULL, 0));
#line 616 "compiler/main.gem"
        GemVal _t837 = gem_fn_make_continue(NULL, NULL, 0);
        gem_pop_frame();
        return _t837;
    }
#line 620 "compiler/main.gem"
    GemVal _t838 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t838, gem_string("type")), gem_string("load")))) {
#line 621 "compiler/main.gem"
    GemVal _t839 = (*gem_v_advance);
        (void)(_t839.fn(_t839.env, NULL, 0));
#line 622 "compiler/main.gem"
    GemVal _t840 = (*gem_v_expect);
    GemVal _t841[] = {gem_string("STRING")};
    GemVal _t842 = _t840.fn(_t840.env, _t841, 1);
        GemVal gem_v_path = gem_table_get(_t842, gem_string("value"));
#line 623 "compiler/main.gem"
    GemVal _t843[] = {gem_v_path};
        GemVal _t844 = gem_fn_make_load(NULL, _t843, 1);
        gem_pop_frame();
        return _t844;
    }
#line 627 "compiler/main.gem"
    GemVal _t845 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t845, gem_string("type")), gem_string("extern")))) {
#line 628 "compiler/main.gem"
    GemVal _t846 = (*gem_v_advance);
        (void)(_t846.fn(_t846.env, NULL, 0));
#line 629 "compiler/main.gem"
    GemVal _t847 = (*gem_v_peek);
    GemVal _t848 = _t847.fn(_t847.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t848, gem_string("type")), gem_string("fn")))) {
#line 630 "compiler/main.gem"
    GemVal _t849 = (*gem_v_advance);
            (void)(_t849.fn(_t849.env, NULL, 0));
#line 631 "compiler/main.gem"
    GemVal _t850 = (*gem_v_expect);
    GemVal _t851[] = {gem_string("NAME")};
    GemVal _t852 = _t850.fn(_t850.env, _t851, 1);
            GemVal gem_v_name = gem_table_get(_t852, gem_string("value"));
#line 632 "compiler/main.gem"
    GemVal _t853 = (*gem_v_expect);
    GemVal _t854[] = {gem_string("(")};
            (void)(_t853.fn(_t853.env, _t854, 1));
#line 633 "compiler/main.gem"
    GemVal _t855 = gem_table_new();
            GemVal gem_v_eparams = _t855;
#line 634 "compiler/main.gem"
    GemVal _t856 = (*gem_v_peek);
    GemVal _t857 = _t856.fn(_t856.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t857, gem_string("type")), gem_string(")")))) {
#line 635 "compiler/main.gem"
    GemVal _t858 = (*gem_v_expect);
    GemVal _t859[] = {gem_string("NAME")};
    GemVal _t860 = _t858.fn(_t858.env, _t859, 1);
                GemVal gem_v_pname = gem_table_get(_t860, gem_string("value"));
#line 636 "compiler/main.gem"
    GemVal _t861 = (*gem_v_expect);
    GemVal _t862[] = {gem_string(":")};
                (void)(_t861.fn(_t861.env, _t862, 1));
#line 637 "compiler/main.gem"
    GemVal _t863 = (*gem_v_expect);
    GemVal _t864[] = {gem_string("NAME")};
    GemVal _t865 = _t863.fn(_t863.env, _t864, 1);
                GemVal gem_v_ptype = gem_table_get(_t865, gem_string("value"));
#line 638 "compiler/main.gem"
    GemVal _t866[] = {gem_v_pname, gem_v_ptype};
    GemVal _t867[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t866, 2)};
                (void)(gem_push_fn(NULL, _t867, 2));
#line 639 "compiler/main.gem"
                while (1) {
                    GemVal _t868 = (*gem_v_peek);
                    GemVal _t869 = _t868.fn(_t868.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t869, gem_string("type")), gem_string(",")))) break;
#line 640 "compiler/main.gem"
    GemVal _t870 = (*gem_v_advance);
                    (void)(_t870.fn(_t870.env, NULL, 0));
#line 641 "compiler/main.gem"
    GemVal _t871 = (*gem_v_expect);
    GemVal _t872[] = {gem_string("NAME")};
    GemVal _t873 = _t871.fn(_t871.env, _t872, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t873, gem_string("value"));
#line 642 "compiler/main.gem"
    GemVal _t874 = (*gem_v_expect);
    GemVal _t875[] = {gem_string(":")};
                    (void)(_t874.fn(_t874.env, _t875, 1));
#line 643 "compiler/main.gem"
    GemVal _t876 = (*gem_v_expect);
    GemVal _t877[] = {gem_string("NAME")};
    GemVal _t878 = _t876.fn(_t876.env, _t877, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t878, gem_string("value"));
#line 644 "compiler/main.gem"
    GemVal _t879[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t880[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t879, 2)};
                    (void)(gem_push_fn(NULL, _t880, 2));
                }
            }
#line 647 "compiler/main.gem"
    GemVal _t881 = (*gem_v_expect);
    GemVal _t882[] = {gem_string(")")};
            (void)(_t881.fn(_t881.env, _t882, 1));
#line 648 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 649 "compiler/main.gem"
    GemVal _t883 = (*gem_v_peek);
    GemVal _t884 = _t883.fn(_t883.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t884, gem_string("type")), gem_string("->")))) {
#line 650 "compiler/main.gem"
    GemVal _t885 = (*gem_v_advance);
                (void)(_t885.fn(_t885.env, NULL, 0));
#line 651 "compiler/main.gem"
    GemVal _t886 = (*gem_v_expect);
    GemVal _t887[] = {gem_string("NAME")};
    GemVal _t888 = _t886.fn(_t886.env, _t887, 1);
                gem_v_ret_type = gem_table_get(_t888, gem_string("value"));
            }
#line 653 "compiler/main.gem"
    GemVal _t889[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            GemVal _t890 = gem_fn_make_extern_fn(NULL, _t889, 3);
            gem_pop_frame();
            return _t890;
        } else {
#line 654 "compiler/main.gem"
    GemVal _t891 = (*gem_v_peek);
    GemVal _t892 = _t891.fn(_t891.env, NULL, 0);
    GemVal _t895;
    if (!gem_truthy(gem_eq(gem_table_get(_t892, gem_string("type")), gem_string("NAME")))) {
        _t895 = gem_eq(gem_table_get(_t892, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t893 = (*gem_v_peek);
        GemVal _t894 = _t893.fn(_t893.env, NULL, 0);
        _t895 = gem_eq(gem_table_get(_t894, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t895)) {
#line 655 "compiler/main.gem"
    GemVal _t896 = (*gem_v_advance);
                (void)(_t896.fn(_t896.env, NULL, 0));
#line 656 "compiler/main.gem"
    GemVal _t897 = (*gem_v_expect);
    GemVal _t898[] = {gem_string("STRING")};
    GemVal _t899 = _t897.fn(_t897.env, _t898, 1);
                GemVal gem_v_path = gem_table_get(_t899, gem_string("value"));
#line 657 "compiler/main.gem"
    GemVal _t900[] = {gem_v_path};
                GemVal _t901 = gem_fn_make_extern_include(NULL, _t900, 1);
                gem_pop_frame();
                return _t901;
            } else {
#line 659 "compiler/main.gem"
    GemVal _t902 = (*gem_v_peek);
    GemVal _t903 = _t902.fn(_t902.env, NULL, 0);
    GemVal _t904[] = {gem_table_get(_t903, gem_string("line"))};
    GemVal _t905[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t904, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 659, _t905, 1));
            }
        }
    }
#line 664 "compiler/main.gem"
    GemVal _t906 = (*gem_v_parse_expr);
    GemVal _t907 = _t906.fn(_t906.env, NULL, 0);
    gem_pop_frame();
    return _t907;
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
    GemVal _t909 = gem_table_new();
    GemVal gem_v_stmts = _t909;
#line 674 "compiler/main.gem"
    GemVal _t910 = (*gem_v_skip_nl);
    (void)(_t910.fn(_t910.env, NULL, 0));
#line 675 "compiler/main.gem"
    while (1) {
        GemVal _t911 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t911.fn(_t911.env, NULL, 0)))) break;
#line 676 "compiler/main.gem"
    GemVal _t912 = (*gem_v_parse_stmt);
    GemVal _t913[] = {gem_v_stmts, _t912.fn(_t912.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t913, 2));
#line 677 "compiler/main.gem"
    GemVal _t914 = (*gem_v_skip_nl);
        (void)(_t914.fn(_t914.env, NULL, 0));
    }
#line 679 "compiler/main.gem"
    GemVal _t915[] = {gem_v_stmts};
    GemVal _t916 = gem_fn_make_program(NULL, _t915, 1);
    gem_pop_frame();
    return _t916;
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
    struct _closure__anon_1 *_t229 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t229->gem_v_pos = gem_v_pos;
    _t229->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t229);
#line 29 "compiler/main.gem"
    struct _closure__anon_2 *_t231 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t231->gem_v_pos = gem_v_pos;
    _t231->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t231);
#line 33 "compiler/main.gem"
    struct _closure__anon_3 *_t233 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t233->gem_v_pos = gem_v_pos;
    _t233->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t233);
#line 39 "compiler/main.gem"
    struct _closure__anon_4 *_t236 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t236->gem_v_pos = gem_v_pos;
    _t236->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t236);
#line 43 "compiler/main.gem"
    struct _closure__anon_5 *_t245 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t245->gem_v_pos = gem_v_pos;
    _t245->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t245);
#line 52 "compiler/main.gem"
    struct _closure__anon_6 *_t247 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t247->gem_v_pos = gem_v_pos;
    _t247->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t247);
#line 60 "compiler/main.gem"
    struct _closure__anon_7 *_t262 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t262->gem_v_advance = gem_v_advance;
    _t262->gem_v_expect = gem_v_expect;
    _t262->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t262);
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
    struct _closure__anon_8 *_t282 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t282->gem_v_at_end = gem_v_at_end;
    _t282->gem_v_parse_stmt = gem_v_parse_stmt;
    _t282->gem_v_peek = gem_v_peek;
    _t282->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t282);
#line 96 "compiler/main.gem"
    struct _closure__anon_9 *_t293 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t293->gem_v_at_end = gem_v_at_end;
    _t293->gem_v_parse_stmt = gem_v_parse_stmt;
    _t293->gem_v_peek = gem_v_peek;
    _t293->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t293);
#line 109 "compiler/main.gem"
    struct _closure__anon_10 *_t420 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t420->gem_v_advance = gem_v_advance;
    _t420->gem_v_expect = gem_v_expect;
    _t420->gem_v_fn_depth = gem_v_fn_depth;
    _t420->gem_v_parse_expr = gem_v_parse_expr;
    _t420->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t420->gem_v_parse_params = gem_v_parse_params;
    _t420->gem_v_peek = gem_v_peek;
    _t420->gem_v_peek_at = gem_v_peek_at;
    _t420->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t420);
#line 240 "compiler/main.gem"
    struct _closure__anon_11 *_t505 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t505->gem_v_advance = gem_v_advance;
    _t505->gem_v_expect = gem_v_expect;
    _t505->gem_v_fn_depth = gem_v_fn_depth;
    _t505->gem_v_parse_atom = gem_v_parse_atom;
    _t505->gem_v_parse_expr = gem_v_parse_expr;
    _t505->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t505->gem_v_parse_params = gem_v_parse_params;
    _t505->gem_v_peek = gem_v_peek;
    _t505->gem_v_peek_at = gem_v_peek_at;
    _t505->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t505);
#line 336 "compiler/main.gem"
    struct _closure__anon_12 *_t514 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t514->gem_v_advance = gem_v_advance;
    _t514->gem_v_parse_call = gem_v_parse_call;
    _t514->gem_v_parse_unary = gem_v_parse_unary;
    _t514->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t514);
#line 345 "compiler/main.gem"
    struct _closure__anon_13 *_t529 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t529->gem_v_advance = gem_v_advance;
    _t529->gem_v_parse_unary = gem_v_parse_unary;
    _t529->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t529);
#line 356 "compiler/main.gem"
    struct _closure__anon_14 *_t541 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t541->gem_v_advance = gem_v_advance;
    _t541->gem_v_parse_mul = gem_v_parse_mul;
    _t541->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t541);
#line 367 "compiler/main.gem"
    struct _closure__anon_15 *_t568 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t568->gem_v_advance = gem_v_advance;
    _t568->gem_v_parse_add = gem_v_parse_add;
    _t568->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t568);
#line 378 "compiler/main.gem"
    struct _closure__anon_16 *_t577 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t577->gem_v_advance = gem_v_advance;
    _t577->gem_v_parse_compare = gem_v_parse_compare;
    _t577->gem_v_parse_not = gem_v_parse_not;
    _t577->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t577);
#line 387 "compiler/main.gem"
    struct _closure__anon_17 *_t585 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t585->gem_v_advance = gem_v_advance;
    _t585->gem_v_parse_not = gem_v_parse_not;
    _t585->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t585);
#line 398 "compiler/main.gem"
    struct _closure__anon_18 *_t593 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t593->gem_v_advance = gem_v_advance;
    _t593->gem_v_parse_and = gem_v_parse_and;
    _t593->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t593);
#line 409 "compiler/main.gem"
    struct _closure__anon_19 *_t644 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t644->gem_v_advance = gem_v_advance;
    _t644->gem_v_parse_expr = gem_v_parse_expr;
    _t644->gem_v_parse_or = gem_v_parse_or;
    _t644->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t644);
#line 444 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 448 "compiler/main.gem"
    struct _closure__anon_20 *_t908 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t908->gem_v_advance = gem_v_advance;
    _t908->gem_v_expect = gem_v_expect;
    _t908->gem_v_fn_depth = gem_v_fn_depth;
    _t908->gem_v_gensym_counter = gem_v_gensym_counter;
    _t908->gem_v_parse_body = gem_v_parse_body;
    _t908->gem_v_parse_expr = gem_v_parse_expr;
    _t908->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t908->gem_v_parse_params = gem_v_parse_params;
    _t908->gem_v_parse_stmt = gem_v_parse_stmt;
    _t908->gem_v_peek = gem_v_peek;
    _t908->gem_v_peek_at = gem_v_peek_at;
    _t908->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_20, _t908);
#line 668 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 672 "compiler/main.gem"
    struct _closure__anon_21 *_t917 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t917->gem_v_at_end = gem_v_at_end;
    _t917->gem_v_parse_stmt = gem_v_parse_stmt;
    _t917->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_21, _t917);
    GemVal _t918 = gem_table_new();
    gem_table_set(_t918, gem_string("parse"), gem_v_parse);
    GemVal _t919 = _t918;
    gem_pop_frame();
    return _t919;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("escape_c_string", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal _t920[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t920, 1);
#line 11 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
    {
#line 12 "compiler/main.gem"
        GemVal gem_v__for_i_1 = gem_int(0);
#line 12 "compiler/main.gem"
    GemVal _t921[] = {gem_v_s};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t921, 1);
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
#line 24 "compiler/main.gem"
    GemVal _t922[] = {gem_v_ch};
                                if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t922, 1), gem_int(0)))) {
#line 25 "compiler/main.gem"
                                    gem_v_result = gem_add(gem_v_result, gem_string("\\0"));
                                } else {
#line 27 "compiler/main.gem"
                                    gem_v_result = gem_add(gem_v_result, gem_v_ch);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t923 = gem_v_result;
    gem_pop_frame();
    return _t923;
}

static GemVal gem_fn_repeat_str(void *_env, GemVal *args, int argc) {
#line 33 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("repeat_str", "compiler/main.gem", 33);
#line 34 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
    {
#line 35 "compiler/main.gem"
        GemVal gem_v__for_i_2 = gem_int(0);
#line 35 "compiler/main.gem"
        GemVal gem_v__for_limit_2 = gem_v_n;
#line 35 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 35 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_2;
#line 35 "compiler/main.gem"
            gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 36 "compiler/main.gem"
            gem_v_result = gem_add(gem_v_result, gem_v_s);
        }
    }
    GemVal _t924 = gem_v_result;
    gem_pop_frame();
    return _t924;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 41 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("pad_for", "compiler/main.gem", 41);
#line 42 "compiler/main.gem"
    GemVal _t925[] = {gem_string("    "), gem_v_indent};
    GemVal _t926 = gem_fn_repeat_str(NULL, _t925, 2);
    gem_pop_frame();
    return _t926;
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 45 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("format_float", "compiler/main.gem", 45);
#line 46 "compiler/main.gem"
    GemVal _t927[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t927, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
    {
#line 49 "compiler/main.gem"
        GemVal gem_v__for_i_3 = gem_int(0);
#line 49 "compiler/main.gem"
    GemVal _t928[] = {gem_v_s};
        GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t928, 1);
#line 49 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 49 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_3;
#line 49 "compiler/main.gem"
            gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 50 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_string(".")))) {
#line 51 "compiler/main.gem"
                gem_v_has_dot = gem_bool(1);
            }
        }
    }
#line 54 "compiler/main.gem"
    if (gem_truthy(gem_not(gem_v_has_dot))) {
#line 55 "compiler/main.gem"
        gem_v_s = gem_add(gem_v_s, gem_string(".0"));
    }
    GemVal _t929 = gem_v_s;
    gem_pop_frame();
    return _t929;
}

static GemVal gem_fn_array_contains(void *_env, GemVal *args, int argc) {
#line 62 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("array_contains", "compiler/main.gem", 62);
    {
#line 63 "compiler/main.gem"
        GemVal gem_v__for_i_4 = gem_int(0);
#line 63 "compiler/main.gem"
    GemVal _t930[] = {gem_v_arr};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t930, 1);
#line 63 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 63 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_4;
#line 63 "compiler/main.gem"
            gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 64 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_arr, gem_v_i), gem_v_val))) {
#line 65 "compiler/main.gem"
                GemVal _t931 = gem_bool(1);
                gem_pop_frame();
                return _t931;
            }
        }
    }
    GemVal _t932 = gem_bool(0);
    gem_pop_frame();
    return _t932;
}

static GemVal gem_fn_set_add(void *_env, GemVal *args, int argc) {
#line 71 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_add", "compiler/main.gem", 71);
#line 72 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1)))) {
#line 73 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_val, gem_bool(1));
        gem_pop_frame();
        return GEM_NIL;
    } else {
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn_set_contains(void *_env, GemVal *args, int argc) {
#line 77 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_contains", "compiler/main.gem", 77);
    GemVal _t933 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t933;
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 81 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_union", "compiler/main.gem", 81);
#line 82 "compiler/main.gem"
    GemVal _t934 = gem_table_new();
    GemVal gem_v_result = _t934;
#line 83 "compiler/main.gem"
    GemVal _t935[] = {gem_v_a};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t935, 1);
#line 84 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 85 "compiler/main.gem"
    while (1) {
        GemVal _t936[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t936, 1)))) break;
#line 86 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
#line 87 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 89 "compiler/main.gem"
    GemVal _t937[] = {gem_v_b};
    GemVal gem_v_ks2 = gem_keys_fn(NULL, _t937, 1);
#line 90 "compiler/main.gem"
    GemVal gem_v_j = gem_int(0);
#line 91 "compiler/main.gem"
    while (1) {
        GemVal _t938[] = {gem_v_ks2};
        if (!gem_truthy(gem_lt(gem_v_j, gem_len_fn(NULL, _t938, 1)))) break;
#line 92 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks2, gem_v_j), gem_bool(1));
#line 93 "compiler/main.gem"
        gem_v_j = gem_add(gem_v_j, gem_int(1));
    }
    GemVal _t939 = gem_v_result;
    gem_pop_frame();
    return _t939;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 98 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_intersect", "compiler/main.gem", 98);
#line 99 "compiler/main.gem"
    GemVal _t940 = gem_table_new();
    GemVal gem_v_result = _t940;
#line 100 "compiler/main.gem"
    GemVal _t941[] = {gem_v_a};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t941, 1);
#line 101 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 102 "compiler/main.gem"
    while (1) {
        GemVal _t942[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t942, 1)))) break;
#line 103 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_table_get(gem_v_ks, gem_v_i)), gem_bool(1)))) {
#line 104 "compiler/main.gem"
            gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
        }
#line 106 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t943 = gem_v_result;
    gem_pop_frame();
    return _t943;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
#line 111 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("set_to_sorted_array", "compiler/main.gem", 111);
#line 112 "compiler/main.gem"
    GemVal _t944[] = {gem_v_s};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t944, 1);
#line 114 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 115 "compiler/main.gem"
    while (1) {
        GemVal _t945[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t945, 1)))) break;
#line 116 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
#line 117 "compiler/main.gem"
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
#line 118 "compiler/main.gem"
        while (1) {
            GemVal _t946;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t946 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t946 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t946)) break;
#line 119 "compiler/main.gem"
            gem_table_set(gem_v_ks, gem_add(gem_v_j, gem_int(1)), gem_table_get(gem_v_ks, gem_v_j));
#line 120 "compiler/main.gem"
            gem_v_j = gem_sub(gem_v_j, gem_int(1));
        }
#line 122 "compiler/main.gem"
        gem_table_set(gem_v_ks, gem_add(gem_v_j, gem_int(1)), gem_v_key);
#line 123 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t947 = gem_v_ks;
    gem_pop_frame();
    return _t947;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 128 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("sorted_array_to_set", "compiler/main.gem", 128);
#line 129 "compiler/main.gem"
    GemVal _t948 = gem_table_new();
    GemVal gem_v_s = _t948;
    {
#line 130 "compiler/main.gem"
        GemVal gem_v__for_items_5 = gem_v_arr;
#line 130 "compiler/main.gem"
        GemVal gem_v__for_i_5 = gem_int(0);
#line 130 "compiler/main.gem"
        while (1) {
            GemVal _t949[] = {gem_v__for_items_5};
            if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t949, 1)))) break;
#line 130 "compiler/main.gem"
            GemVal gem_v_item = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 130 "compiler/main.gem"
            gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 131 "compiler/main.gem"
            gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
        }
    }
    GemVal _t950 = gem_v_s;
    gem_pop_frame();
    return _t950;
}

static GemVal gem_fn_collect_top_let_names(void *_env, GemVal *args, int argc) {
#line 138 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("collect_top_let_names", "compiler/main.gem", 138);
    {
#line 139 "compiler/main.gem"
        GemVal gem_v__for_i_6 = gem_int(0);
#line 139 "compiler/main.gem"
    GemVal _t951[] = {gem_v_stmts};
        GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t951, 1);
#line 139 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
#line 139 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_6;
#line 139 "compiler/main.gem"
            gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 140 "compiler/main.gem"
            GemVal gem_v_s = gem_table_get(gem_v_stmts, gem_v_i);
#line 141 "compiler/main.gem"
    GemVal _t952[] = {gem_v_s};
    GemVal _t954;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t952, 1), gem_string("table")))) {
        _t954 = gem_eq(gem_type_fn(NULL, _t952, 1), gem_string("table"));
    } else {
        GemVal _t953 = gem_v_s;
        _t954 = gem_eq(gem_table_get(_t953, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t954)) {
#line 142 "compiler/main.gem"
    GemVal _t955 = gem_v_s;
    GemVal _t956[] = {gem_v_result, gem_table_get(_t955, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t956, 2));
            }
        }
        gem_pop_frame();
        return GEM_NIL;
    }
}

struct _closure__anon_22 {
    GemVal *gem_v_tmp_counter;
};
static GemVal _anon_22(void *_env, GemVal *args, int argc) {
    struct _closure__anon_22 *_cls = (struct _closure__anon_22 *)_env;
    GemVal *gem_v_tmp_counter = _cls->gem_v_tmp_counter;
    gem_push_frame("_anon_22", "compiler/main.gem", 0);
#line 189 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t968[] = {(*gem_v_tmp_counter)};
    GemVal _t969 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t968, 1));
    gem_pop_frame();
    return _t969;
}

struct _closure__anon_23 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_23", "compiler/main.gem", 0);
#line 194 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t971[] = {(*gem_v_anon_counter)};
    GemVal _t972 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t971, 1));
    gem_pop_frame();
    return _t972;
}

struct _closure__anon_24 {
};
static GemVal _anon_24(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_24", "compiler/main.gem", 0);
    GemVal _t974[] = {gem_v_name};
    GemVal _t975 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t974, 1));
    gem_pop_frame();
    return _t975;
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
#line 208 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 209 "compiler/main.gem"
        GemVal _t976 = GEM_NIL;
        gem_pop_frame();
        return _t976;
    }
#line 211 "compiler/main.gem"
    GemVal _t977[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t977, 1), gem_string("table")))) {
#line 212 "compiler/main.gem"
        GemVal _t978 = GEM_NIL;
        gem_pop_frame();
        return _t978;
    }
#line 214 "compiler/main.gem"
    GemVal _t979 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t979, gem_string("tag"));
#line 215 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 216 "compiler/main.gem"
        GemVal _t980 = GEM_NIL;
        gem_pop_frame();
        return _t980;
    }
#line 219 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 220 "compiler/main.gem"
    GemVal _t981 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t981, gem_string("name"));
#line 221 "compiler/main.gem"
    GemVal _t982[] = {gem_v_defined, gem_v_name};
    GemVal _t984;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t982, 2)))) {
        _t984 = gem_not(gem_fn_set_contains(NULL, _t982, 2));
    } else {
        GemVal _t983[] = {(*gem_v_builtins), gem_v_name};
        _t984 = gem_not(gem_fn_set_contains(NULL, _t983, 2));
    }
    GemVal _t986;
    if (!gem_truthy(_t984)) {
        _t986 = _t984;
    } else {
        GemVal _t985[] = {(*gem_v_defined_fns), gem_v_name};
        _t986 = gem_not(gem_fn_set_contains(NULL, _t985, 2));
    }
        if (gem_truthy(_t986)) {
#line 222 "compiler/main.gem"
    GemVal _t987[] = {gem_v_free, gem_v_name};
            GemVal _t988 = gem_fn_set_add(NULL, _t987, 2);
            gem_pop_frame();
            return _t988;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
#line 224 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 225 "compiler/main.gem"
    GemVal _t989 = gem_v_node;
    GemVal _t990 = (*gem_v_collect_free_node);
    GemVal _t991[] = {gem_table_get(_t989, gem_string("value")), gem_v_defined, gem_v_free};
            GemVal _t992 = _t990.fn(_t990.env, _t991, 3);
            gem_pop_frame();
            return _t992;
        } else {
#line 226 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 227 "compiler/main.gem"
    GemVal _t993 = gem_v_node;
                GemVal gem_v_name = gem_table_get(_t993, gem_string("name"));
#line 228 "compiler/main.gem"
    GemVal _t994[] = {gem_v_defined, gem_v_name};
    GemVal _t996;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t994, 2)))) {
        _t996 = gem_not(gem_fn_set_contains(NULL, _t994, 2));
    } else {
        GemVal _t995[] = {(*gem_v_builtins), gem_v_name};
        _t996 = gem_not(gem_fn_set_contains(NULL, _t995, 2));
    }
    GemVal _t998;
    if (!gem_truthy(_t996)) {
        _t998 = _t996;
    } else {
        GemVal _t997[] = {(*gem_v_defined_fns), gem_v_name};
        _t998 = gem_not(gem_fn_set_contains(NULL, _t997, 2));
    }
                if (gem_truthy(_t998)) {
#line 229 "compiler/main.gem"
    GemVal _t999[] = {gem_v_free, gem_v_name};
                    (void)(gem_fn_set_add(NULL, _t999, 2));
                }
#line 231 "compiler/main.gem"
    GemVal _t1000 = gem_v_node;
    GemVal _t1001 = (*gem_v_collect_free_node);
    GemVal _t1002[] = {gem_table_get(_t1000, gem_string("value")), gem_v_defined, gem_v_free};
                GemVal _t1003 = _t1001.fn(_t1001.env, _t1002, 3);
                gem_pop_frame();
                return _t1003;
            } else {
#line 232 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 233 "compiler/main.gem"
    GemVal _t1004 = gem_v_node;
    GemVal _t1005[] = {gem_table_get(_t1004, gem_string("params"))};
    GemVal _t1006[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t1005, 1)};
                    GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t1006, 2);
#line 234 "compiler/main.gem"
    GemVal _t1007 = gem_v_node;
    GemVal _t1008 = (*gem_v_collect_free_vars);
    GemVal _t1009[] = {gem_table_get(_t1007, gem_string("body")), gem_v_inner_defined};
                    GemVal gem_v_inner_free = _t1008.fn(_t1008.env, _t1009, 2);
#line 235 "compiler/main.gem"
    GemVal _t1010[] = {gem_v_inner_free};
                    GemVal gem_v_ifks = gem_keys_fn(NULL, _t1010, 1);
#line 236 "compiler/main.gem"
                    GemVal gem_v_ifi = gem_int(0);
#line 237 "compiler/main.gem"
                    while (1) {
                        GemVal _t1011[] = {gem_v_ifks};
                        if (!gem_truthy(gem_lt(gem_v_ifi, gem_len_fn(NULL, _t1011, 1)))) break;
#line 238 "compiler/main.gem"
    GemVal _t1012[] = {gem_v_free, gem_table_get(gem_v_ifks, gem_v_ifi)};
                        (void)(gem_fn_set_add(NULL, _t1012, 2));
#line 239 "compiler/main.gem"
                        gem_v_ifi = gem_add(gem_v_ifi, gem_int(1));
                    }
                    gem_pop_frame();
                    return GEM_NIL;
                } else {
#line 241 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                        gem_pop_frame();
                        return GEM_NIL;
                    } else {
#line 243 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 244 "compiler/main.gem"
    GemVal _t1013 = gem_v_node;
    GemVal _t1014 = (*gem_v_collect_free_node);
    GemVal _t1015[] = {gem_table_get(_t1013, gem_string("cond")), gem_v_defined, gem_v_free};
                            (void)(_t1014.fn(_t1014.env, _t1015, 3));
#line 245 "compiler/main.gem"
    GemVal _t1016 = gem_table_new();
    GemVal _t1017[] = {gem_v_defined, _t1016};
                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1017, 2);
#line 246 "compiler/main.gem"
    GemVal _t1018 = gem_v_node;
                            GemVal gem_v_thens = gem_table_get(_t1018, gem_string("then"));
                            {
#line 247 "compiler/main.gem"
                                GemVal gem_v__for_i_8 = gem_int(0);
#line 247 "compiler/main.gem"
    GemVal _t1019[] = {gem_v_thens};
                                GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1019, 1);
#line 247 "compiler/main.gem"
                                while (1) {
                                    if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 247 "compiler/main.gem"
                                    GemVal gem_v_i = gem_v__for_i_8;
#line 247 "compiler/main.gem"
                                    gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 248 "compiler/main.gem"
    GemVal _t1020 = (*gem_v_collect_free_node);
    GemVal _t1021[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
                                    (void)(_t1020.fn(_t1020.env, _t1021, 3));
#line 249 "compiler/main.gem"
    GemVal _t1022[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t1024;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1022, 1), gem_string("table")))) {
        _t1024 = gem_eq(gem_type_fn(NULL, _t1022, 1), gem_string("table"));
    } else {
        GemVal _t1023 = gem_table_get(gem_v_thens, gem_v_i);
        _t1024 = gem_eq(gem_table_get(_t1023, gem_string("tag")), gem_string("let"));
    }
                                    if (gem_truthy(_t1024)) {
#line 250 "compiler/main.gem"
    GemVal _t1025 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1026[] = {gem_v_d, gem_table_get(_t1025, gem_string("name"))};
                                        (void)(gem_fn_set_add(NULL, _t1026, 2));
                                    }
                                }
                            }
#line 253 "compiler/main.gem"
    GemVal _t1027 = gem_v_node;
                            GemVal gem_v_el = gem_table_get(_t1027, gem_string("else"));
#line 254 "compiler/main.gem"
                            if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 255 "compiler/main.gem"
    GemVal _t1028 = gem_table_new();
    GemVal _t1029[] = {gem_v_defined, _t1028};
                                gem_v_d = gem_fn_set_union(NULL, _t1029, 2);
                                {
#line 256 "compiler/main.gem"
                                    GemVal gem_v__for_i_9 = gem_int(0);
#line 256 "compiler/main.gem"
    GemVal _t1030[] = {gem_v_el};
                                    GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1030, 1);
#line 256 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 256 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_9;
#line 256 "compiler/main.gem"
                                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 257 "compiler/main.gem"
    GemVal _t1031 = (*gem_v_collect_free_node);
    GemVal _t1032[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t1031.fn(_t1031.env, _t1032, 3));
#line 258 "compiler/main.gem"
    GemVal _t1033[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t1035;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1033, 1), gem_string("table")))) {
        _t1035 = gem_eq(gem_type_fn(NULL, _t1033, 1), gem_string("table"));
    } else {
        GemVal _t1034 = gem_table_get(gem_v_el, gem_v_i);
        _t1035 = gem_eq(gem_table_get(_t1034, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t1035)) {
#line 259 "compiler/main.gem"
    GemVal _t1036 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1037[] = {gem_v_d, gem_table_get(_t1036, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t1037, 2));
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
#line 263 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 264 "compiler/main.gem"
    GemVal _t1038 = gem_v_node;
    GemVal _t1039 = (*gem_v_collect_free_node);
    GemVal _t1040[] = {gem_table_get(_t1038, gem_string("cond")), gem_v_defined, gem_v_free};
                                (void)(_t1039.fn(_t1039.env, _t1040, 3));
#line 265 "compiler/main.gem"
    GemVal _t1041 = gem_table_new();
    GemVal _t1042[] = {gem_v_defined, _t1041};
                                GemVal gem_v_d = gem_fn_set_union(NULL, _t1042, 2);
                                {
#line 266 "compiler/main.gem"
                                    GemVal gem_v__for_i_10 = gem_int(0);
#line 266 "compiler/main.gem"
    GemVal _t1043 = gem_v_node;
    GemVal _t1044[] = {gem_table_get(_t1043, gem_string("body"))};
                                    GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t1044, 1);
#line 266 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
#line 266 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_10;
#line 266 "compiler/main.gem"
                                        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 267 "compiler/main.gem"
    GemVal _t1045 = gem_v_node;
    GemVal _t1046 = (*gem_v_collect_free_node);
    GemVal _t1047[] = {gem_table_get(gem_table_get(_t1045, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t1046.fn(_t1046.env, _t1047, 3));
#line 268 "compiler/main.gem"
    GemVal _t1048 = gem_v_node;
    GemVal _t1049[] = {gem_table_get(gem_table_get(_t1048, gem_string("body")), gem_v_i)};
    GemVal _t1052;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1049, 1), gem_string("table")))) {
        _t1052 = gem_eq(gem_type_fn(NULL, _t1049, 1), gem_string("table"));
    } else {
        GemVal _t1050 = gem_v_node;
        GemVal _t1051 = gem_table_get(gem_table_get(_t1050, gem_string("body")), gem_v_i);
        _t1052 = gem_eq(gem_table_get(_t1051, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t1052)) {
#line 269 "compiler/main.gem"
    GemVal _t1053 = gem_v_node;
    GemVal _t1054 = gem_table_get(gem_table_get(_t1053, gem_string("body")), gem_v_i);
    GemVal _t1055[] = {gem_v_d, gem_table_get(_t1054, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t1055, 2));
                                        }
                                    }
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 272 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 273 "compiler/main.gem"
    GemVal _t1056 = gem_v_node;
    GemVal _t1057 = (*gem_v_collect_free_node);
    GemVal _t1058[] = {gem_table_get(_t1056, gem_string("target")), gem_v_defined, gem_v_free};
                                    (void)(_t1057.fn(_t1057.env, _t1058, 3));
                                    {
#line 274 "compiler/main.gem"
                                        GemVal gem_v__for_i_12 = gem_int(0);
#line 274 "compiler/main.gem"
    GemVal _t1059 = gem_v_node;
    GemVal _t1060[] = {gem_table_get(_t1059, gem_string("whens"))};
                                        GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t1060, 1);
#line 274 "compiler/main.gem"
                                        while (1) {
                                            if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 274 "compiler/main.gem"
                                            GemVal gem_v_i = gem_v__for_i_12;
#line 274 "compiler/main.gem"
                                            gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 275 "compiler/main.gem"
    GemVal _t1061 = gem_v_node;
                                            GemVal gem_v_w = gem_table_get(gem_table_get(_t1061, gem_string("whens")), gem_v_i);
#line 276 "compiler/main.gem"
    GemVal _t1062 = gem_v_w;
    GemVal _t1063 = (*gem_v_collect_free_node);
    GemVal _t1064[] = {gem_table_get(_t1062, gem_string("value")), gem_v_defined, gem_v_free};
                                            (void)(_t1063.fn(_t1063.env, _t1064, 3));
#line 277 "compiler/main.gem"
    GemVal _t1065 = gem_table_new();
    GemVal _t1066[] = {gem_v_defined, _t1065};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1066, 2);
                                            {
#line 278 "compiler/main.gem"
                                                GemVal gem_v__for_i_11 = gem_int(0);
#line 278 "compiler/main.gem"
    GemVal _t1067 = gem_v_w;
    GemVal _t1068[] = {gem_table_get(_t1067, gem_string("body"))};
                                                GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t1068, 1);
#line 278 "compiler/main.gem"
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
#line 278 "compiler/main.gem"
                                                    GemVal gem_v_j = gem_v__for_i_11;
#line 278 "compiler/main.gem"
                                                    gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 279 "compiler/main.gem"
    GemVal _t1069 = gem_v_w;
    GemVal _t1070 = (*gem_v_collect_free_node);
    GemVal _t1071[] = {gem_table_get(gem_table_get(_t1069, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
                                                    (void)(_t1070.fn(_t1070.env, _t1071, 3));
#line 280 "compiler/main.gem"
    GemVal _t1072 = gem_v_w;
    GemVal _t1073[] = {gem_table_get(gem_table_get(_t1072, gem_string("body")), gem_v_j)};
    GemVal _t1076;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1073, 1), gem_string("table")))) {
        _t1076 = gem_eq(gem_type_fn(NULL, _t1073, 1), gem_string("table"));
    } else {
        GemVal _t1074 = gem_v_w;
        GemVal _t1075 = gem_table_get(gem_table_get(_t1074, gem_string("body")), gem_v_j);
        _t1076 = gem_eq(gem_table_get(_t1075, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t1076)) {
#line 281 "compiler/main.gem"
    GemVal _t1077 = gem_v_w;
    GemVal _t1078 = gem_table_get(gem_table_get(_t1077, gem_string("body")), gem_v_j);
    GemVal _t1079[] = {gem_v_d, gem_table_get(_t1078, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t1079, 2));
                                                    }
                                                }
                                            }
                                        }
                                    }
#line 285 "compiler/main.gem"
    GemVal _t1080 = gem_v_node;
                                    GemVal gem_v_el = gem_table_get(_t1080, gem_string("else"));
#line 286 "compiler/main.gem"
                                    if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 287 "compiler/main.gem"
    GemVal _t1081 = gem_table_new();
    GemVal _t1082[] = {gem_v_defined, _t1081};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1082, 2);
                                        {
#line 288 "compiler/main.gem"
                                            GemVal gem_v__for_i_13 = gem_int(0);
#line 288 "compiler/main.gem"
    GemVal _t1083[] = {gem_v_el};
                                            GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1083, 1);
#line 288 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 288 "compiler/main.gem"
                                                GemVal gem_v_j = gem_v__for_i_13;
#line 288 "compiler/main.gem"
                                                gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 289 "compiler/main.gem"
    GemVal _t1084 = (*gem_v_collect_free_node);
    GemVal _t1085[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
                                                (void)(_t1084.fn(_t1084.env, _t1085, 3));
#line 290 "compiler/main.gem"
    GemVal _t1086[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t1088;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1086, 1), gem_string("table")))) {
        _t1088 = gem_eq(gem_type_fn(NULL, _t1086, 1), gem_string("table"));
    } else {
        GemVal _t1087 = gem_table_get(gem_v_el, gem_v_j);
        _t1088 = gem_eq(gem_table_get(_t1087, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1088)) {
#line 291 "compiler/main.gem"
    GemVal _t1089 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t1090[] = {gem_v_d, gem_table_get(_t1089, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1090, 2));
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
#line 295 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("when_clause")))) {
#line 296 "compiler/main.gem"
    GemVal _t1091 = gem_v_node;
    GemVal _t1092 = (*gem_v_collect_free_node);
    GemVal _t1093[] = {gem_table_get(_t1091, gem_string("value")), gem_v_defined, gem_v_free};
                                        (void)(_t1092.fn(_t1092.env, _t1093, 3));
#line 297 "compiler/main.gem"
    GemVal _t1094 = gem_table_new();
    GemVal _t1095[] = {gem_v_defined, _t1094};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1095, 2);
                                        {
#line 298 "compiler/main.gem"
                                            GemVal gem_v__for_i_14 = gem_int(0);
#line 298 "compiler/main.gem"
    GemVal _t1096 = gem_v_node;
    GemVal _t1097[] = {gem_table_get(_t1096, gem_string("body"))};
                                            GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1097, 1);
#line 298 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 298 "compiler/main.gem"
                                                GemVal gem_v_i = gem_v__for_i_14;
#line 298 "compiler/main.gem"
                                                gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 299 "compiler/main.gem"
    GemVal _t1098 = gem_v_node;
    GemVal _t1099 = (*gem_v_collect_free_node);
    GemVal _t1100[] = {gem_table_get(gem_table_get(_t1098, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                                (void)(_t1099.fn(_t1099.env, _t1100, 3));
#line 300 "compiler/main.gem"
    GemVal _t1101 = gem_v_node;
    GemVal _t1102[] = {gem_table_get(gem_table_get(_t1101, gem_string("body")), gem_v_i)};
    GemVal _t1105;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1102, 1), gem_string("table")))) {
        _t1105 = gem_eq(gem_type_fn(NULL, _t1102, 1), gem_string("table"));
    } else {
        GemVal _t1103 = gem_v_node;
        GemVal _t1104 = gem_table_get(gem_table_get(_t1103, gem_string("body")), gem_v_i);
        _t1105 = gem_eq(gem_table_get(_t1104, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1105)) {
#line 301 "compiler/main.gem"
    GemVal _t1106 = gem_v_node;
    GemVal _t1107 = gem_table_get(gem_table_get(_t1106, gem_string("body")), gem_v_i);
    GemVal _t1108[] = {gem_v_d, gem_table_get(_t1107, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1108, 2));
                                                }
                                            }
                                            gem_pop_frame();
                                            return GEM_NIL;
                                        }
                                    } else {
#line 304 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 305 "compiler/main.gem"
    GemVal _t1109 = gem_table_new();
    GemVal _t1110[] = {gem_v_defined, _t1109};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1110, 2);
                                            {
#line 306 "compiler/main.gem"
                                                GemVal gem_v__for_i_15 = gem_int(0);
#line 306 "compiler/main.gem"
    GemVal _t1111 = gem_v_node;
    GemVal _t1112[] = {gem_table_get(_t1111, gem_string("stmts"))};
                                                GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1112, 1);
#line 306 "compiler/main.gem"
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 306 "compiler/main.gem"
                                                    GemVal gem_v_i = gem_v__for_i_15;
#line 306 "compiler/main.gem"
                                                    gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 307 "compiler/main.gem"
    GemVal _t1113 = gem_v_node;
    GemVal _t1114 = (*gem_v_collect_free_node);
    GemVal _t1115[] = {gem_table_get(gem_table_get(_t1113, gem_string("stmts")), gem_v_i), gem_v_d, gem_v_free};
                                                    (void)(_t1114.fn(_t1114.env, _t1115, 3));
#line 308 "compiler/main.gem"
    GemVal _t1116 = gem_v_node;
    GemVal _t1117[] = {gem_table_get(gem_table_get(_t1116, gem_string("stmts")), gem_v_i)};
    GemVal _t1120;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1117, 1), gem_string("table")))) {
        _t1120 = gem_eq(gem_type_fn(NULL, _t1117, 1), gem_string("table"));
    } else {
        GemVal _t1118 = gem_v_node;
        GemVal _t1119 = gem_table_get(gem_table_get(_t1118, gem_string("stmts")), gem_v_i);
        _t1120 = gem_eq(gem_table_get(_t1119, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t1120)) {
#line 309 "compiler/main.gem"
    GemVal _t1121 = gem_v_node;
    GemVal _t1122 = gem_table_get(gem_table_get(_t1121, gem_string("stmts")), gem_v_i);
    GemVal _t1123[] = {gem_v_d, gem_table_get(_t1122, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t1123, 2));
                                                    }
                                                }
                                                gem_pop_frame();
                                                return GEM_NIL;
                                            }
                                        } else {
#line 312 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 313 "compiler/main.gem"
    GemVal _t1124 = gem_v_node;
                                                if (gem_truthy(gem_neq(gem_table_get(_t1124, gem_string("value")), GEM_NIL))) {
#line 314 "compiler/main.gem"
    GemVal _t1125 = gem_v_node;
    GemVal _t1126 = (*gem_v_collect_free_node);
    GemVal _t1127[] = {gem_table_get(_t1125, gem_string("value")), gem_v_defined, gem_v_free};
                                                    GemVal _t1128 = _t1126.fn(_t1126.env, _t1127, 3);
                                                    gem_pop_frame();
                                                    return _t1128;
                                                } else {
                                                    gem_pop_frame();
                                                    return GEM_NIL;
                                                }
                                            } else {
#line 318 "compiler/main.gem"
    GemVal _t1129[] = {gem_v_node};
                                                GemVal gem_v_nks = gem_keys_fn(NULL, _t1129, 1);
#line 319 "compiler/main.gem"
                                                GemVal gem_v_nki = gem_int(0);
#line 320 "compiler/main.gem"
                                                while (1) {
                                                    GemVal _t1130[] = {gem_v_nks};
                                                    if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t1130, 1)))) break;
#line 321 "compiler/main.gem"
                                                    if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
#line 322 "compiler/main.gem"
                                                        GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
#line 323 "compiler/main.gem"
    GemVal _t1131[] = {gem_v_v};
                                                        if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1131, 1), gem_string("table")))) {
#line 325 "compiler/main.gem"
    GemVal _t1132[] = {gem_v_v};
                                                            GemVal gem_v_vks = gem_keys_fn(NULL, _t1132, 1);
#line 326 "compiler/main.gem"
    GemVal _t1133[] = {gem_v_vks};
    GemVal _t1135;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1133, 1), gem_int(0)))) {
        _t1135 = gem_gt(gem_len_fn(NULL, _t1133, 1), gem_int(0));
    } else {
        GemVal _t1134[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1135 = gem_eq(gem_type_fn(NULL, _t1134, 1), gem_string("int"));
    }
                                                            if (gem_truthy(_t1135)) {
#line 328 "compiler/main.gem"
                                                                GemVal gem_v_vi = gem_int(0);
#line 329 "compiler/main.gem"
                                                                while (1) {
                                                                    GemVal _t1136[] = {gem_v_v};
                                                                    if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1136, 1)))) break;
#line 330 "compiler/main.gem"
    GemVal _t1137[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1137, 1), gem_string("table")))) {
#line 331 "compiler/main.gem"
    GemVal _t1138 = (*gem_v_collect_free_node);
    GemVal _t1139[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
                                                                        (void)(_t1138.fn(_t1138.env, _t1139, 3));
                                                                    }
#line 333 "compiler/main.gem"
                                                                    gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                                }
                                                            } else {
#line 336 "compiler/main.gem"
    GemVal _t1140 = (*gem_v_collect_free_node);
    GemVal _t1141[] = {gem_v_v, gem_v_defined, gem_v_free};
                                                                (void)(_t1140.fn(_t1140.env, _t1141, 3));
                                                            }
                                                        }
                                                    }
#line 340 "compiler/main.gem"
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
#line 346 "compiler/main.gem"
    GemVal _t1143 = gem_table_new();
    GemVal gem_v_free = _t1143;
#line 347 "compiler/main.gem"
    GemVal _t1144 = gem_table_new();
    GemVal _t1145[] = {gem_v_defined, _t1144};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t1145, 2);
    {
#line 348 "compiler/main.gem"
        GemVal gem_v__for_i_17 = gem_int(0);
#line 348 "compiler/main.gem"
    GemVal _t1146[] = {gem_v_stmts};
        GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1146, 1);
#line 348 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 348 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_17;
#line 348 "compiler/main.gem"
            gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 349 "compiler/main.gem"
    GemVal _t1147 = (*gem_v_collect_free_node);
    GemVal _t1148[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
            (void)(_t1147.fn(_t1147.env, _t1148, 3));
#line 350 "compiler/main.gem"
    GemVal _t1149[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1151;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1149, 1), gem_string("table")))) {
        _t1151 = gem_eq(gem_type_fn(NULL, _t1149, 1), gem_string("table"));
    } else {
        GemVal _t1150 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1151 = gem_eq(gem_table_get(_t1150, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1151)) {
#line 351 "compiler/main.gem"
    GemVal _t1152 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1153[] = {gem_v_d, gem_table_get(_t1152, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1153, 2));
            }
#line 353 "compiler/main.gem"
    GemVal _t1154[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1156;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1154, 1), gem_string("table")))) {
        _t1156 = gem_eq(gem_type_fn(NULL, _t1154, 1), gem_string("table"));
    } else {
        GemVal _t1155 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1156 = gem_eq(gem_table_get(_t1155, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1156)) {
                {
#line 354 "compiler/main.gem"
                    GemVal gem_v__for_i_16 = gem_int(0);
#line 354 "compiler/main.gem"
    GemVal _t1157 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1158[] = {gem_table_get(_t1157, gem_string("stmts"))};
                    GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1158, 1);
#line 354 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 354 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_16;
#line 354 "compiler/main.gem"
                        gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 355 "compiler/main.gem"
    GemVal _t1159 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1160[] = {gem_table_get(gem_table_get(_t1159, gem_string("stmts")), gem_v_j)};
    GemVal _t1163;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1160, 1), gem_string("table")))) {
        _t1163 = gem_eq(gem_type_fn(NULL, _t1160, 1), gem_string("table"));
    } else {
        GemVal _t1161 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1162 = gem_table_get(gem_table_get(_t1161, gem_string("stmts")), gem_v_j);
        _t1163 = gem_eq(gem_table_get(_t1162, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1163)) {
#line 356 "compiler/main.gem"
    GemVal _t1164 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1165 = gem_table_get(gem_table_get(_t1164, gem_string("stmts")), gem_v_j);
    GemVal _t1166[] = {gem_v_d, gem_table_get(_t1165, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1166, 2));
                        }
                    }
                }
            }
        }
    }
    GemVal _t1167 = gem_v_free;
    gem_pop_frame();
    return _t1167;
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
#line 370 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 371 "compiler/main.gem"
        GemVal _t1169 = GEM_NIL;
        gem_pop_frame();
        return _t1169;
    }
#line 373 "compiler/main.gem"
    GemVal _t1170[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1170, 1), gem_string("table")))) {
#line 374 "compiler/main.gem"
        GemVal _t1171 = GEM_NIL;
        gem_pop_frame();
        return _t1171;
    }
#line 376 "compiler/main.gem"
    GemVal _t1172 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1172, gem_string("tag"));
#line 377 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 378 "compiler/main.gem"
        GemVal _t1173 = GEM_NIL;
        gem_pop_frame();
        return _t1173;
    }
#line 381 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 382 "compiler/main.gem"
    GemVal _t1174 = gem_v_node;
    GemVal _t1175[] = {gem_table_get(_t1174, gem_string("params"))};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1175, 1);
#line 383 "compiler/main.gem"
    GemVal _t1176 = gem_v_node;
    GemVal _t1177 = (*gem_v_collect_free_vars);
    GemVal _t1178[] = {gem_table_get(_t1176, gem_string("body")), gem_v_inner_defined};
        GemVal gem_v_free = _t1177.fn(_t1177.env, _t1178, 2);
#line 384 "compiler/main.gem"
    GemVal _t1179[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t1179, 2);
#line 385 "compiler/main.gem"
    GemVal _t1180[] = {gem_v_inter};
        GemVal gem_v_iks = gem_keys_fn(NULL, _t1180, 1);
#line 386 "compiler/main.gem"
        GemVal gem_v_iki = gem_int(0);
#line 387 "compiler/main.gem"
        while (1) {
            GemVal _t1181[] = {gem_v_iks};
            if (!gem_truthy(gem_lt(gem_v_iki, gem_len_fn(NULL, _t1181, 1)))) break;
#line 388 "compiler/main.gem"
    GemVal _t1182[] = {gem_v_captured, gem_table_get(gem_v_iks, gem_v_iki)};
            (void)(gem_fn_set_add(NULL, _t1182, 2));
#line 389 "compiler/main.gem"
            gem_v_iki = gem_add(gem_v_iki, gem_int(1));
        }
#line 391 "compiler/main.gem"
    GemVal _t1183 = gem_v_node;
    GemVal _t1184 = gem_v_node;
    GemVal _t1185[] = {gem_table_get(_t1184, gem_string("params"))};
    GemVal _t1186[] = {gem_v_scope_vars, gem_fn_sorted_array_to_set(NULL, _t1185, 1)};
    GemVal _t1187 = (*gem_v_walk_captures);
    GemVal _t1188[] = {gem_table_get(_t1183, gem_string("body")), gem_fn_set_union(NULL, _t1186, 2), gem_v_captured};
        GemVal _t1189 = _t1187.fn(_t1187.env, _t1188, 3);
        gem_pop_frame();
        return _t1189;
    } else {
#line 392 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
            gem_pop_frame();
            return GEM_NIL;
        } else {
#line 394 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 395 "compiler/main.gem"
    GemVal _t1190 = gem_v_node;
    GemVal _t1191 = (*gem_v_walk_captures_node);
    GemVal _t1192[] = {gem_table_get(_t1190, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1191.fn(_t1191.env, _t1192, 3));
#line 396 "compiler/main.gem"
    GemVal _t1193 = gem_v_node;
    GemVal _t1194 = (*gem_v_walk_captures);
    GemVal _t1195[] = {gem_table_get(_t1193, gem_string("then")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1194.fn(_t1194.env, _t1195, 3));
#line 397 "compiler/main.gem"
    GemVal _t1196 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t1196, gem_string("else")), GEM_NIL))) {
#line 398 "compiler/main.gem"
    GemVal _t1197 = gem_v_node;
    GemVal _t1198 = (*gem_v_walk_captures);
    GemVal _t1199[] = {gem_table_get(_t1197, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                    GemVal _t1200 = _t1198.fn(_t1198.env, _t1199, 3);
                    gem_pop_frame();
                    return _t1200;
                } else {
                    gem_pop_frame();
                    return GEM_NIL;
                }
            } else {
#line 400 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 401 "compiler/main.gem"
    GemVal _t1201 = gem_v_node;
    GemVal _t1202 = (*gem_v_walk_captures_node);
    GemVal _t1203[] = {gem_table_get(_t1201, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                    (void)(_t1202.fn(_t1202.env, _t1203, 3));
#line 402 "compiler/main.gem"
    GemVal _t1204 = gem_v_node;
    GemVal _t1205 = (*gem_v_walk_captures);
    GemVal _t1206[] = {gem_table_get(_t1204, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                    GemVal _t1207 = _t1205.fn(_t1205.env, _t1206, 3);
                    gem_pop_frame();
                    return _t1207;
                } else {
#line 403 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 404 "compiler/main.gem"
    GemVal _t1208 = gem_v_node;
    GemVal _t1209 = (*gem_v_walk_captures_node);
    GemVal _t1210[] = {gem_table_get(_t1208, gem_string("target")), gem_v_scope_vars, gem_v_captured};
                        (void)(_t1209.fn(_t1209.env, _t1210, 3));
                        {
#line 405 "compiler/main.gem"
                            GemVal gem_v__for_i_18 = gem_int(0);
#line 405 "compiler/main.gem"
    GemVal _t1211 = gem_v_node;
    GemVal _t1212[] = {gem_table_get(_t1211, gem_string("whens"))};
                            GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1212, 1);
#line 405 "compiler/main.gem"
                            while (1) {
                                if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 405 "compiler/main.gem"
                                GemVal gem_v_i = gem_v__for_i_18;
#line 405 "compiler/main.gem"
                                gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 406 "compiler/main.gem"
    GemVal _t1213 = gem_v_node;
    GemVal _t1214 = gem_table_get(gem_table_get(_t1213, gem_string("whens")), gem_v_i);
    GemVal _t1215 = (*gem_v_walk_captures_node);
    GemVal _t1216[] = {gem_table_get(_t1214, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t1215.fn(_t1215.env, _t1216, 3));
#line 407 "compiler/main.gem"
    GemVal _t1217 = gem_v_node;
    GemVal _t1218 = gem_table_get(gem_table_get(_t1217, gem_string("whens")), gem_v_i);
    GemVal _t1219 = (*gem_v_walk_captures);
    GemVal _t1220[] = {gem_table_get(_t1218, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t1219.fn(_t1219.env, _t1220, 3));
                            }
                        }
#line 409 "compiler/main.gem"
    GemVal _t1221 = gem_v_node;
                        if (gem_truthy(gem_neq(gem_table_get(_t1221, gem_string("else")), GEM_NIL))) {
#line 410 "compiler/main.gem"
    GemVal _t1222 = gem_v_node;
    GemVal _t1223 = (*gem_v_walk_captures);
    GemVal _t1224[] = {gem_table_get(_t1222, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                            GemVal _t1225 = _t1223.fn(_t1223.env, _t1224, 3);
                            gem_pop_frame();
                            return _t1225;
                        } else {
                            gem_pop_frame();
                            return GEM_NIL;
                        }
                    } else {
#line 412 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 413 "compiler/main.gem"
    GemVal _t1226 = gem_v_node;
    GemVal _t1227 = (*gem_v_walk_captures);
    GemVal _t1228[] = {gem_table_get(_t1226, gem_string("stmts")), gem_v_scope_vars, gem_v_captured};
                            GemVal _t1229 = _t1227.fn(_t1227.env, _t1228, 3);
                            gem_pop_frame();
                            return _t1229;
                        } else {
#line 414 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 415 "compiler/main.gem"
    GemVal _t1230 = gem_v_node;
                                if (gem_truthy(gem_neq(gem_table_get(_t1230, gem_string("value")), GEM_NIL))) {
#line 416 "compiler/main.gem"
    GemVal _t1231 = gem_v_node;
    GemVal _t1232 = (*gem_v_walk_captures_node);
    GemVal _t1233[] = {gem_table_get(_t1231, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    GemVal _t1234 = _t1232.fn(_t1232.env, _t1233, 3);
                                    gem_pop_frame();
                                    return _t1234;
                                } else {
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 418 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 419 "compiler/main.gem"
    GemVal _t1235 = gem_v_node;
    GemVal _t1236 = (*gem_v_walk_captures_node);
    GemVal _t1237[] = {gem_table_get(_t1235, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    GemVal _t1238 = _t1236.fn(_t1236.env, _t1237, 3);
                                    gem_pop_frame();
                                    return _t1238;
                                } else {
#line 420 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 421 "compiler/main.gem"
    GemVal _t1239 = gem_v_node;
    GemVal _t1240 = (*gem_v_walk_captures_node);
    GemVal _t1241[] = {gem_table_get(_t1239, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                        GemVal _t1242 = _t1240.fn(_t1240.env, _t1241, 3);
                                        gem_pop_frame();
                                        return _t1242;
                                    } else {
#line 424 "compiler/main.gem"
    GemVal _t1243[] = {gem_v_node};
                                        GemVal gem_v_nks = gem_keys_fn(NULL, _t1243, 1);
#line 425 "compiler/main.gem"
                                        GemVal gem_v_nki = gem_int(0);
#line 426 "compiler/main.gem"
                                        while (1) {
                                            GemVal _t1244[] = {gem_v_nks};
                                            if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t1244, 1)))) break;
#line 427 "compiler/main.gem"
                                            if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
#line 428 "compiler/main.gem"
                                                GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
#line 429 "compiler/main.gem"
    GemVal _t1245[] = {gem_v_v};
                                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1245, 1), gem_string("table")))) {
#line 430 "compiler/main.gem"
    GemVal _t1246[] = {gem_v_v};
                                                    GemVal gem_v_vks = gem_keys_fn(NULL, _t1246, 1);
#line 431 "compiler/main.gem"
    GemVal _t1247[] = {gem_v_vks};
    GemVal _t1249;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1247, 1), gem_int(0)))) {
        _t1249 = gem_gt(gem_len_fn(NULL, _t1247, 1), gem_int(0));
    } else {
        GemVal _t1248[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1249 = gem_eq(gem_type_fn(NULL, _t1248, 1), gem_string("int"));
    }
                                                    if (gem_truthy(_t1249)) {
#line 432 "compiler/main.gem"
                                                        GemVal gem_v_vi = gem_int(0);
#line 433 "compiler/main.gem"
                                                        while (1) {
                                                            GemVal _t1250[] = {gem_v_v};
                                                            if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1250, 1)))) break;
#line 434 "compiler/main.gem"
    GemVal _t1251[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                            if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1251, 1), gem_string("table")))) {
#line 435 "compiler/main.gem"
    GemVal _t1252 = (*gem_v_walk_captures_node);
    GemVal _t1253[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
                                                                (void)(_t1252.fn(_t1252.env, _t1253, 3));
                                                            }
#line 437 "compiler/main.gem"
                                                            gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                        }
                                                    } else {
#line 440 "compiler/main.gem"
    GemVal _t1254 = (*gem_v_walk_captures_node);
    GemVal _t1255[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
                                                        (void)(_t1254.fn(_t1254.env, _t1255, 3));
                                                    }
                                                }
                                            }
#line 444 "compiler/main.gem"
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
#line 450 "compiler/main.gem"
    GemVal _t1257 = gem_table_new();
    GemVal _t1258[] = {gem_v_scope_vars, _t1257};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t1258, 2);
    {
#line 451 "compiler/main.gem"
        GemVal gem_v__for_i_20 = gem_int(0);
#line 451 "compiler/main.gem"
    GemVal _t1259[] = {gem_v_stmts};
        GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1259, 1);
#line 451 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 451 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_20;
#line 451 "compiler/main.gem"
            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 452 "compiler/main.gem"
    GemVal _t1260 = (*gem_v_walk_captures_node);
    GemVal _t1261[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
            (void)(_t1260.fn(_t1260.env, _t1261, 3));
#line 453 "compiler/main.gem"
    GemVal _t1262[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1264;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1262, 1), gem_string("table")))) {
        _t1264 = gem_eq(gem_type_fn(NULL, _t1262, 1), gem_string("table"));
    } else {
        GemVal _t1263 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1264 = gem_eq(gem_table_get(_t1263, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1264)) {
#line 454 "compiler/main.gem"
    GemVal _t1265 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1266[] = {gem_v_sv, gem_table_get(_t1265, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1266, 2));
            }
#line 456 "compiler/main.gem"
    GemVal _t1267[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1269;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1267, 1), gem_string("table")))) {
        _t1269 = gem_eq(gem_type_fn(NULL, _t1267, 1), gem_string("table"));
    } else {
        GemVal _t1268 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1269 = gem_eq(gem_table_get(_t1268, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1269)) {
                {
#line 457 "compiler/main.gem"
                    GemVal gem_v__for_i_19 = gem_int(0);
#line 457 "compiler/main.gem"
    GemVal _t1270 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1271[] = {gem_table_get(_t1270, gem_string("stmts"))};
                    GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1271, 1);
#line 457 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 457 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_19;
#line 457 "compiler/main.gem"
                        gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 458 "compiler/main.gem"
    GemVal _t1272 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1273[] = {gem_table_get(gem_table_get(_t1272, gem_string("stmts")), gem_v_j)};
    GemVal _t1276;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1273, 1), gem_string("table")))) {
        _t1276 = gem_eq(gem_type_fn(NULL, _t1273, 1), gem_string("table"));
    } else {
        GemVal _t1274 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1275 = gem_table_get(gem_table_get(_t1274, gem_string("stmts")), gem_v_j);
        _t1276 = gem_eq(gem_table_get(_t1275, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1276)) {
#line 459 "compiler/main.gem"
    GemVal _t1277 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1278 = gem_table_get(gem_table_get(_t1277, gem_string("stmts")), gem_v_j);
    GemVal _t1279[] = {gem_v_sv, gem_table_get(_t1278, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1279, 2));
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
#line 467 "compiler/main.gem"
    GemVal _t1281 = gem_table_new();
    GemVal gem_v_captured = _t1281;
#line 468 "compiler/main.gem"
    GemVal _t1282 = (*gem_v_walk_captures);
    GemVal _t1283[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    (void)(_t1282.fn(_t1282.env, _t1283, 3));
    GemVal _t1284 = gem_v_captured;
    gem_pop_frame();
    return _t1284;
}

struct _closure__anon_30 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_builtin_fns;
    GemVal *gem_v_compile_anon_fn;
    GemVal *gem_v_compile_array;
    GemVal *gem_v_compile_binop;
    GemVal *gem_v_compile_call;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_table;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
    GemVal *gem_v_top_level_boxed;
};
static GemVal _anon_30(void *_env, GemVal *args, int argc) {
    struct _closure__anon_30 *_cls = (struct _closure__anon_30 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_builtin_fns = _cls->gem_v_builtin_fns;
    GemVal *gem_v_compile_anon_fn = _cls->gem_v_compile_anon_fn;
    GemVal *gem_v_compile_array = _cls->gem_v_compile_array;
    GemVal *gem_v_compile_binop = _cls->gem_v_compile_binop;
    GemVal *gem_v_compile_call = _cls->gem_v_compile_call;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_table = _cls->gem_v_compile_table;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal *gem_v_top_level_boxed = _cls->gem_v_top_level_boxed;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_30", "compiler/main.gem", 0);
#line 495 "compiler/main.gem"
    GemVal _t1286 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1286, gem_string("tag"));
#line 497 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("int")))) {
#line 498 "compiler/main.gem"
    GemVal _t1287 = gem_table_new();
    GemVal _t1288 = gem_v_node;
    GemVal _t1289[] = {gem_table_get(_t1288, gem_string("value"))};
    gem_table_set(_t1287, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t1289, 1)), gem_string(")")));
    gem_table_set(_t1287, gem_string("setup"), gem_string(""));
        GemVal _t1290 = _t1287;
        gem_pop_frame();
        return _t1290;
    } else {
#line 499 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("float")))) {
#line 500 "compiler/main.gem"
    GemVal _t1291 = gem_table_new();
    GemVal _t1292 = gem_v_node;
    GemVal _t1293[] = {gem_table_get(_t1292, gem_string("value"))};
    GemVal _t1294[] = {gem_fn_format_float(NULL, _t1293, 1)};
    gem_table_set(_t1291, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t1294, 1)), gem_string(")")));
    gem_table_set(_t1291, gem_string("setup"), gem_string(""));
            GemVal _t1295 = _t1291;
            gem_pop_frame();
            return _t1295;
        } else {
#line 501 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("string")))) {
#line 502 "compiler/main.gem"
    GemVal _t1296 = gem_v_node;
    GemVal _t1297[] = {gem_table_get(_t1296, gem_string("value"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1297, 1);
#line 503 "compiler/main.gem"
    GemVal _t1298 = gem_table_new();
    GemVal _t1299[] = {gem_v_escaped};
    gem_table_set(_t1298, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t1299, 1)), gem_string("\")")));
    gem_table_set(_t1298, gem_string("setup"), gem_string(""));
                GemVal _t1300 = _t1298;
                gem_pop_frame();
                return _t1300;
            } else {
#line 504 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("interp")))) {
#line 505 "compiler/main.gem"
    GemVal _t1301 = gem_v_node;
                    GemVal gem_v_parts = gem_table_get(_t1301, gem_string("parts"));
#line 506 "compiler/main.gem"
    GemVal _t1302[] = {gem_v_parts};
                    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1302, 1), gem_int(0)))) {
#line 507 "compiler/main.gem"
    GemVal _t1303 = gem_table_new();
    gem_table_set(_t1303, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t1303, gem_string("setup"), gem_string(""));
                        GemVal _t1304 = _t1303;
                        gem_pop_frame();
                        return _t1304;
                    }
#line 509 "compiler/main.gem"
                    GemVal gem_v_setup = gem_string("");
#line 510 "compiler/main.gem"
    GemVal _t1305 = gem_table_new();
                    GemVal gem_v_compiled = _t1305;
                    {
#line 511 "compiler/main.gem"
                        GemVal gem_v__for_i_21 = gem_int(0);
#line 511 "compiler/main.gem"
    GemVal _t1306[] = {gem_v_parts};
                        GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t1306, 1);
#line 511 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 511 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_21;
#line 511 "compiler/main.gem"
                            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 512 "compiler/main.gem"
                            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 513 "compiler/main.gem"
    GemVal _t1307 = (*gem_v_compile_expr);
    GemVal _t1308[] = {gem_v_part};
                            GemVal gem_v_r = _t1307.fn(_t1307.env, _t1308, 1);
#line 514 "compiler/main.gem"
    GemVal _t1309 = gem_v_r;
                            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1309, gem_string("setup")));
#line 515 "compiler/main.gem"
    GemVal _t1310 = gem_v_part;
                            if (gem_truthy(gem_eq(gem_table_get(_t1310, gem_string("tag")), gem_string("string")))) {
#line 516 "compiler/main.gem"
    GemVal _t1311 = gem_v_r;
    GemVal _t1312[] = {gem_v_compiled, gem_table_get(_t1311, gem_string("expr"))};
                                (void)(gem_push_fn(NULL, _t1312, 2));
                            } else {
#line 518 "compiler/main.gem"
    GemVal _t1313 = (*gem_v_tmp);
                                GemVal gem_v_ts = _t1313.fn(_t1313.env, NULL, 0);
#line 519 "compiler/main.gem"
    GemVal _t1314[] = {gem_v_ts};
    GemVal _t1315 = gem_v_r;
    GemVal _t1316[] = {gem_table_get(_t1315, gem_string("expr"))};
                                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1314, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1316, 1)), gem_string("};\n")));
#line 520 "compiler/main.gem"
    GemVal _t1317[] = {gem_v_ts};
    GemVal _t1318[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t1317, 1)), gem_string(", 1)"))};
                                (void)(gem_push_fn(NULL, _t1318, 2));
                            }
                        }
                    }
#line 523 "compiler/main.gem"
                    GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
                    {
#line 524 "compiler/main.gem"
                        GemVal gem_v__for_i_22 = gem_int(1);
#line 524 "compiler/main.gem"
    GemVal _t1319[] = {gem_v_compiled};
                        GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1319, 1);
#line 524 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 524 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_22;
#line 524 "compiler/main.gem"
                            gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 525 "compiler/main.gem"
    GemVal _t1320[] = {gem_v_acc};
    GemVal _t1321[] = {gem_table_get(gem_v_compiled, gem_v_i)};
                            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1320, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1321, 1)), gem_string(")"));
                        }
                    }
#line 527 "compiler/main.gem"
    GemVal _t1322 = gem_table_new();
    gem_table_set(_t1322, gem_string("expr"), gem_v_acc);
    gem_table_set(_t1322, gem_string("setup"), gem_v_setup);
                    GemVal _t1323 = _t1322;
                    gem_pop_frame();
                    return _t1323;
                } else {
#line 528 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("bool")))) {
#line 529 "compiler/main.gem"
    GemVal _t1324 = gem_v_node;
                        if (gem_truthy(gem_table_get(_t1324, gem_string("value")))) {
#line 530 "compiler/main.gem"
    GemVal _t1325 = gem_table_new();
    gem_table_set(_t1325, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t1325, gem_string("setup"), gem_string(""));
                            GemVal _t1326 = _t1325;
                            gem_pop_frame();
                            return _t1326;
                        }
#line 532 "compiler/main.gem"
    GemVal _t1327 = gem_table_new();
    gem_table_set(_t1327, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t1327, gem_string("setup"), gem_string(""));
                        GemVal _t1328 = _t1327;
                        gem_pop_frame();
                        return _t1328;
                    } else {
#line 533 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("nil")))) {
#line 534 "compiler/main.gem"
    GemVal _t1329 = gem_table_new();
    gem_table_set(_t1329, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1329, gem_string("setup"), gem_string(""));
                            GemVal _t1330 = _t1329;
                            gem_pop_frame();
                            return _t1330;
                        } else {
#line 535 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 536 "compiler/main.gem"
    GemVal _t1331 = gem_v_node;
                                GemVal gem_v_name = gem_table_get(_t1331, gem_string("name"));
#line 537 "compiler/main.gem"
    GemVal _t1332[] = {(*gem_v_builtin_fns), gem_v_name};
                                if (gem_truthy(gem_has_key_fn(NULL, _t1332, 2))) {
#line 538 "compiler/main.gem"
    GemVal _t1333 = gem_table_new();
    GemVal _t1334[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name)};
    gem_table_set(_t1333, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1334, 1)), gem_string(", NULL)")));
    gem_table_set(_t1333, gem_string("setup"), gem_string(""));
                                    GemVal _t1335 = _t1333;
                                    gem_pop_frame();
                                    return _t1335;
                                } else {
#line 539 "compiler/main.gem"
    GemVal _t1336[] = {(*gem_v_defined_fns), gem_v_name};
                                    if (gem_truthy(gem_fn_set_contains(NULL, _t1336, 2))) {
#line 540 "compiler/main.gem"
    GemVal _t1337 = gem_table_new();
    GemVal _t1338[] = {gem_v_name};
    gem_table_set(_t1337, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t1338, 1)), gem_string(", NULL)")));
    gem_table_set(_t1337, gem_string("setup"), gem_string(""));
                                        GemVal _t1339 = _t1337;
                                        gem_pop_frame();
                                        return _t1339;
                                    }
                                }
#line 542 "compiler/main.gem"
    GemVal _t1340 = (*gem_v_mangle);
    GemVal _t1341[] = {gem_v_name};
                                GemVal gem_v_mname = _t1340.fn(_t1340.env, _t1341, 1);
#line 543 "compiler/main.gem"
    GemVal _t1342[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t1344;
    if (gem_truthy(gem_fn_set_contains(NULL, _t1342, 2))) {
        _t1344 = gem_fn_set_contains(NULL, _t1342, 2);
    } else {
        GemVal _t1343[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t1344 = gem_fn_set_contains(NULL, _t1343, 2);
    }
                                if (gem_truthy(_t1344)) {
#line 544 "compiler/main.gem"
    GemVal _t1345 = gem_table_new();
    GemVal _t1346[] = {gem_v_mname};
    gem_table_set(_t1345, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t1346, 1)), gem_string(")")));
    gem_table_set(_t1345, gem_string("setup"), gem_string(""));
                                    GemVal _t1347 = _t1345;
                                    gem_pop_frame();
                                    return _t1347;
                                }
#line 546 "compiler/main.gem"
    GemVal _t1348 = gem_table_new();
    gem_table_set(_t1348, gem_string("expr"), gem_v_mname);
    gem_table_set(_t1348, gem_string("setup"), gem_string(""));
                                GemVal _t1349 = _t1348;
                                gem_pop_frame();
                                return _t1349;
                            } else {
#line 547 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 548 "compiler/main.gem"
    GemVal _t1350 = (*gem_v_compile_call);
    GemVal _t1351[] = {gem_v_node};
                                    GemVal _t1352 = _t1350.fn(_t1350.env, _t1351, 1);
                                    gem_pop_frame();
                                    return _t1352;
                                } else {
#line 549 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("binop")))) {
#line 550 "compiler/main.gem"
    GemVal _t1353 = (*gem_v_compile_binop);
    GemVal _t1354[] = {gem_v_node};
                                        GemVal _t1355 = _t1353.fn(_t1353.env, _t1354, 1);
                                        gem_pop_frame();
                                        return _t1355;
                                    } else {
#line 551 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("unop")))) {
#line 552 "compiler/main.gem"
    GemVal _t1356 = gem_v_node;
    GemVal _t1357 = (*gem_v_compile_expr);
    GemVal _t1358[] = {gem_table_get(_t1356, gem_string("expr"))};
                                            GemVal gem_v_r = _t1357.fn(_t1357.env, _t1358, 1);
#line 553 "compiler/main.gem"
    GemVal _t1359 = gem_v_node;
                                            if (gem_truthy(gem_eq(gem_table_get(_t1359, gem_string("op")), gem_string("-")))) {
#line 554 "compiler/main.gem"
    GemVal _t1360 = gem_table_new();
    GemVal _t1361 = gem_v_r;
    GemVal _t1362[] = {gem_table_get(_t1361, gem_string("expr"))};
    gem_table_set(_t1360, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t1362, 1)), gem_string(")")));
    GemVal _t1363 = gem_v_r;
    gem_table_set(_t1360, gem_string("setup"), gem_table_get(_t1363, gem_string("setup")));
                                                GemVal _t1364 = _t1360;
                                                gem_pop_frame();
                                                return _t1364;
                                            } else {
#line 555 "compiler/main.gem"
    GemVal _t1365 = gem_v_node;
                                                if (gem_truthy(gem_eq(gem_table_get(_t1365, gem_string("op")), gem_string("not")))) {
#line 556 "compiler/main.gem"
    GemVal _t1366 = gem_table_new();
    GemVal _t1367 = gem_v_r;
    GemVal _t1368[] = {gem_table_get(_t1367, gem_string("expr"))};
    gem_table_set(_t1366, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t1368, 1)), gem_string(")")));
    GemVal _t1369 = gem_v_r;
    gem_table_set(_t1366, gem_string("setup"), gem_table_get(_t1369, gem_string("setup")));
                                                    GemVal _t1370 = _t1366;
                                                    gem_pop_frame();
                                                    return _t1370;
                                                }
                                            }
                                        } else {
#line 558 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 559 "compiler/main.gem"
    GemVal _t1371 = (*gem_v_compile_anon_fn);
    GemVal _t1372[] = {gem_v_node};
                                                GemVal _t1373 = _t1371.fn(_t1371.env, _t1372, 1);
                                                gem_pop_frame();
                                                return _t1373;
                                            } else {
#line 560 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("table")))) {
#line 561 "compiler/main.gem"
    GemVal _t1374 = (*gem_v_compile_table);
    GemVal _t1375[] = {gem_v_node};
                                                    GemVal _t1376 = _t1374.fn(_t1374.env, _t1375, 1);
                                                    gem_pop_frame();
                                                    return _t1376;
                                                } else {
#line 562 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("array")))) {
#line 563 "compiler/main.gem"
    GemVal _t1377 = (*gem_v_compile_array);
    GemVal _t1378[] = {gem_v_node};
                                                        GemVal _t1379 = _t1377.fn(_t1377.env, _t1378, 1);
                                                        gem_pop_frame();
                                                        return _t1379;
                                                    } else {
#line 564 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot")))) {
#line 565 "compiler/main.gem"
    GemVal _t1380 = gem_v_node;
    GemVal _t1381 = (*gem_v_compile_expr);
    GemVal _t1382[] = {gem_table_get(_t1380, gem_string("object"))};
                                                            GemVal gem_v_r = _t1381.fn(_t1381.env, _t1382, 1);
#line 566 "compiler/main.gem"
    GemVal _t1383 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1383.fn(_t1383.env, NULL, 0);
#line 567 "compiler/main.gem"
    GemVal _t1384 = gem_v_r;
    GemVal _t1385[] = {gem_v_t};
    GemVal _t1386 = gem_v_r;
    GemVal _t1387[] = {gem_table_get(_t1386, gem_string("expr"))};
                                                            GemVal gem_v_setup = gem_add(gem_table_get(_t1384, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1385, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1387, 1)), gem_string(";\n")));
#line 568 "compiler/main.gem"
    GemVal _t1388 = gem_v_node;
    GemVal _t1389[] = {gem_table_get(_t1388, gem_string("field"))};
                                                            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1389, 1);
#line 569 "compiler/main.gem"
    GemVal _t1390 = gem_table_new();
    GemVal _t1391[] = {gem_v_t};
    GemVal _t1392[] = {gem_v_escaped};
    gem_table_set(_t1390, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1391, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1392, 1)), gem_string("\"))")));
    gem_table_set(_t1390, gem_string("setup"), gem_v_setup);
                                                            GemVal _t1393 = _t1390;
                                                            gem_pop_frame();
                                                            return _t1393;
                                                        } else {
#line 570 "compiler/main.gem"
                                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("index")))) {
#line 571 "compiler/main.gem"
    GemVal _t1394 = gem_v_node;
    GemVal _t1395 = (*gem_v_compile_expr);
    GemVal _t1396[] = {gem_table_get(_t1394, gem_string("object"))};
                                                                GemVal gem_v_obj_r = _t1395.fn(_t1395.env, _t1396, 1);
#line 572 "compiler/main.gem"
    GemVal _t1397 = gem_v_node;
    GemVal _t1398 = (*gem_v_compile_expr);
    GemVal _t1399[] = {gem_table_get(_t1397, gem_string("key"))};
                                                                GemVal gem_v_key_r = _t1398.fn(_t1398.env, _t1399, 1);
#line 573 "compiler/main.gem"
    GemVal _t1400 = gem_table_new();
    GemVal _t1401 = gem_v_obj_r;
    GemVal _t1402[] = {gem_table_get(_t1401, gem_string("expr"))};
    GemVal _t1403 = gem_v_key_r;
    GemVal _t1404[] = {gem_table_get(_t1403, gem_string("expr"))};
    gem_table_set(_t1400, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1402, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1404, 1)), gem_string(")")));
    GemVal _t1405 = gem_v_obj_r;
    GemVal _t1406 = gem_v_key_r;
    gem_table_set(_t1400, gem_string("setup"), gem_add(gem_table_get(_t1405, gem_string("setup")), gem_table_get(_t1406, gem_string("setup"))));
                                                                GemVal _t1407 = _t1400;
                                                                gem_pop_frame();
                                                                return _t1407;
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
    GemVal _t1408 = gem_table_new();
    gem_table_set(_t1408, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1408, gem_string("setup"), gem_add(gem_add(gem_string("/* unknown node: "), gem_v_tag), gem_string(" */\n")));
    GemVal _t1409 = _t1408;
    gem_pop_frame();
    return _t1409;
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
#line 582 "compiler/main.gem"
    GemVal _t1411 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t1411, gem_string("entries"));
#line 583 "compiler/main.gem"
    GemVal _t1412 = (*gem_v_tmp);
    GemVal gem_v_t = _t1412.fn(_t1412.env, NULL, 0);
#line 584 "compiler/main.gem"
    GemVal _t1413[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1413, 1)), gem_string(" = gem_table_new();\n"));
    {
#line 585 "compiler/main.gem"
        GemVal gem_v__for_items_23 = gem_v_entries;
#line 585 "compiler/main.gem"
        GemVal gem_v__for_i_23 = gem_int(0);
#line 585 "compiler/main.gem"
        while (1) {
            GemVal _t1414[] = {gem_v__for_items_23};
            if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_len_fn(NULL, _t1414, 1)))) break;
#line 585 "compiler/main.gem"
            GemVal gem_v_entry = gem_table_get(gem_v__for_items_23, gem_v__for_i_23);
#line 585 "compiler/main.gem"
            gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 586 "compiler/main.gem"
    GemVal _t1415 = gem_v_entry;
    GemVal _t1416 = (*gem_v_compile_expr);
    GemVal _t1417[] = {gem_table_get(_t1415, gem_string("value"))};
            GemVal gem_v_val_r = _t1416.fn(_t1416.env, _t1417, 1);
#line 587 "compiler/main.gem"
    GemVal _t1418 = gem_v_entry;
    GemVal _t1419[] = {gem_table_get(_t1418, gem_string("key"))};
            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1419, 1);
#line 588 "compiler/main.gem"
    GemVal _t1420 = gem_v_val_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1420, gem_string("setup")));
#line 589 "compiler/main.gem"
    GemVal _t1421[] = {gem_v_t};
    GemVal _t1422[] = {gem_v_escaped};
    GemVal _t1423 = gem_v_val_r;
    GemVal _t1424[] = {gem_table_get(_t1423, gem_string("expr"))};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1421, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1422, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1424, 1)), gem_string(");\n")));
        }
    }
    GemVal _t1425 = gem_table_new();
    gem_table_set(_t1425, gem_string("expr"), gem_v_t);
    gem_table_set(_t1425, gem_string("setup"), gem_v_setup);
    GemVal _t1426 = _t1425;
    gem_pop_frame();
    return _t1426;
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
#line 597 "compiler/main.gem"
    GemVal _t1428 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t1428, gem_string("elements"));
#line 598 "compiler/main.gem"
    GemVal _t1429 = (*gem_v_tmp);
    GemVal gem_v_t = _t1429.fn(_t1429.env, NULL, 0);
#line 599 "compiler/main.gem"
    GemVal _t1430[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1430, 1)), gem_string(" = gem_table_new();\n"));
    {
#line 600 "compiler/main.gem"
        GemVal gem_v__for_i_24 = gem_int(0);
#line 600 "compiler/main.gem"
    GemVal _t1431[] = {gem_v_elements};
        GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1431, 1);
#line 600 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 600 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_24;
#line 600 "compiler/main.gem"
            gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 601 "compiler/main.gem"
    GemVal _t1432 = (*gem_v_compile_expr);
    GemVal _t1433[] = {gem_table_get(gem_v_elements, gem_v_i)};
            GemVal gem_v_elem_r = _t1432.fn(_t1432.env, _t1433, 1);
#line 602 "compiler/main.gem"
    GemVal _t1434 = gem_v_elem_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1434, gem_string("setup")));
#line 603 "compiler/main.gem"
    GemVal _t1435[] = {gem_v_t};
    GemVal _t1436[] = {gem_v_i};
    GemVal _t1437 = gem_v_elem_r;
    GemVal _t1438[] = {gem_table_get(_t1437, gem_string("expr"))};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1435, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t1436, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t1438, 1)), gem_string(");\n")));
        }
    }
    GemVal _t1439 = gem_table_new();
    gem_table_set(_t1439, gem_string("expr"), gem_v_t);
    gem_table_set(_t1439, gem_string("setup"), gem_v_setup);
    GemVal _t1440 = _t1439;
    gem_pop_frame();
    return _t1440;
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
#line 611 "compiler/main.gem"
    GemVal _t1442 = gem_v_node;
    GemVal _t1443[] = {gem_table_get(_t1442, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1443, 1);
#line 612 "compiler/main.gem"
    GemVal _t1444 = gem_v_node;
    GemVal _t1445 = (*gem_v_collect_free_vars);
    GemVal _t1446[] = {gem_table_get(_t1444, gem_string("body")), gem_v_inner_defined};
    GemVal gem_v_free = _t1445.fn(_t1445.env, _t1446, 2);
#line 613 "compiler/main.gem"
    GemVal _t1447[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t1447, 1);
#line 615 "compiler/main.gem"
    GemVal _t1448[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1448, 1), gem_int(0)))) {
#line 616 "compiler/main.gem"
    GemVal _t1449 = gem_table_new();
    GemVal _t1450 = (*gem_v_compile_closure_fn);
    GemVal _t1451[] = {gem_v_node, _t1449};
        GemVal gem_v_result = _t1450.fn(_t1450.env, _t1451, 2);
#line 617 "compiler/main.gem"
    GemVal _t1452 = gem_table_new();
    GemVal _t1453 = gem_v_result;
    GemVal _t1454[] = {gem_table_get(_t1453, gem_string("fn_name"))};
    gem_table_set(_t1452, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1454, 1)), gem_string(", NULL)")));
    gem_table_set(_t1452, gem_string("setup"), gem_string(""));
        GemVal _t1455 = _t1452;
        gem_pop_frame();
        return _t1455;
    }
#line 620 "compiler/main.gem"
    GemVal _t1456 = (*gem_v_compile_closure_fn);
    GemVal _t1457[] = {gem_v_node, gem_v_captures};
    GemVal gem_v_result = _t1456.fn(_t1456.env, _t1457, 2);
#line 622 "compiler/main.gem"
    GemVal gem_v_setup = gem_string("");
#line 623 "compiler/main.gem"
    GemVal _t1458 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t1458.fn(_t1458.env, NULL, 0);
#line 624 "compiler/main.gem"
    GemVal _t1459 = gem_v_result;
    GemVal _t1460[] = {gem_table_get(_t1459, gem_string("struct_name"))};
    GemVal _t1461[] = {gem_v_env_tmp};
    GemVal _t1462 = gem_v_result;
    GemVal _t1463[] = {gem_table_get(_t1462, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t1460, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t1461, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t1463, 1)), gem_string("));\n")));
    {
#line 625 "compiler/main.gem"
        GemVal gem_v__for_items_25 = gem_v_captures;
#line 625 "compiler/main.gem"
        GemVal gem_v__for_i_25 = gem_int(0);
#line 625 "compiler/main.gem"
        while (1) {
            GemVal _t1464[] = {gem_v__for_items_25};
            if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_len_fn(NULL, _t1464, 1)))) break;
#line 625 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_25, gem_v__for_i_25);
#line 625 "compiler/main.gem"
            gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 626 "compiler/main.gem"
    GemVal _t1465 = (*gem_v_mangle);
    GemVal _t1466[] = {gem_v_cap};
            GemVal gem_v_mc = _t1465.fn(_t1465.env, _t1466, 1);
#line 627 "compiler/main.gem"
    GemVal _t1467[] = {(*gem_v_boxed_vars), gem_v_cap};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1467, 2))) {
#line 628 "compiler/main.gem"
    GemVal _t1468[] = {gem_v_env_tmp};
    GemVal _t1469[] = {gem_v_mc};
    GemVal _t1470[] = {gem_v_mc};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1468, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1469, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1470, 1)), gem_string(";\n")));
            } else {
#line 630 "compiler/main.gem"
    GemVal _t1471[] = {gem_v_env_tmp};
    GemVal _t1472[] = {gem_v_mc};
    GemVal _t1473[] = {gem_v_mc};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1471, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1472, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t1473, 1)), gem_string(";\n")));
            }
        }
    }
    GemVal _t1474 = gem_table_new();
    GemVal _t1475 = gem_v_result;
    GemVal _t1476[] = {gem_table_get(_t1475, gem_string("fn_name"))};
    GemVal _t1477[] = {gem_v_env_tmp};
    gem_table_set(_t1474, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1476, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1477, 1)), gem_string(")")));
    gem_table_set(_t1474, gem_string("setup"), gem_v_setup);
    GemVal _t1478 = _t1474;
    gem_pop_frame();
    return _t1478;
}

struct _closure__anon_34 {
    GemVal *gem_v_builtin_fns;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tmp;
};
static GemVal _anon_34(void *_env, GemVal *args, int argc) {
    struct _closure__anon_34 *_cls = (struct _closure__anon_34 *)_env;
    GemVal *gem_v_builtin_fns = _cls->gem_v_builtin_fns;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_34", "compiler/main.gem", 0);
#line 639 "compiler/main.gem"
    GemVal _t1480 = gem_v_node;
    GemVal gem_v_func = gem_table_get(_t1480, gem_string("func"));
#line 640 "compiler/main.gem"
    GemVal _t1481 = gem_v_node;
    GemVal gem_v_args = gem_table_get(_t1481, gem_string("args"));
#line 643 "compiler/main.gem"
    GemVal _t1482 = gem_v_func;
    GemVal _t1484;
    if (!gem_truthy(gem_eq(gem_table_get(_t1482, gem_string("tag")), gem_string("dot")))) {
        _t1484 = gem_eq(gem_table_get(_t1482, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t1483 = gem_v_func;
        _t1484 = gem_eq(gem_table_get(_t1483, gem_string("field")), gem_string("len"));
    }
    GemVal _t1486;
    if (!gem_truthy(_t1484)) {
        _t1486 = _t1484;
    } else {
        GemVal _t1485[] = {gem_v_args};
        _t1486 = gem_eq(gem_len_fn(NULL, _t1485, 1), gem_int(0));
    }
    if (gem_truthy(_t1486)) {
#line 644 "compiler/main.gem"
    GemVal _t1487 = gem_v_func;
    GemVal _t1488 = (*gem_v_compile_expr);
    GemVal _t1489[] = {gem_table_get(_t1487, gem_string("object"))};
        GemVal gem_v_obj_r = _t1488.fn(_t1488.env, _t1489, 1);
#line 645 "compiler/main.gem"
    GemVal _t1490 = gem_table_new();
    GemVal _t1491 = gem_v_obj_r;
    GemVal _t1492[] = {gem_table_get(_t1491, gem_string("expr"))};
    gem_table_set(_t1490, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t1492, 1)), gem_string(")")));
    GemVal _t1493 = gem_v_obj_r;
    gem_table_set(_t1490, gem_string("setup"), gem_table_get(_t1493, gem_string("setup")));
        GemVal _t1494 = _t1490;
        gem_pop_frame();
        return _t1494;
    }
#line 649 "compiler/main.gem"
    GemVal _t1495 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t1495, gem_string("tag")), gem_string("var")))) {
#line 650 "compiler/main.gem"
    GemVal _t1496 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t1496, gem_string("name"));
#line 652 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 653 "compiler/main.gem"
    GemVal _t1497 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t1497, gem_string("line"));
#line 654 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 655 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 657 "compiler/main.gem"
    GemVal _t1498[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t1498, 1);
#line 658 "compiler/main.gem"
            GemVal gem_v_arg_setups = gem_string("");
#line 659 "compiler/main.gem"
    GemVal _t1499 = gem_table_new();
            GemVal gem_v_arg_exprs = _t1499;
            {
#line 660 "compiler/main.gem"
                GemVal gem_v__for_i_26 = gem_int(0);
#line 660 "compiler/main.gem"
    GemVal _t1500[] = {gem_v_args};
                GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1500, 1);
#line 660 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 660 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_26;
#line 660 "compiler/main.gem"
                    gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 661 "compiler/main.gem"
    GemVal _t1501 = (*gem_v_compile_expr);
    GemVal _t1502[] = {gem_table_get(gem_v_args, gem_v_i)};
                    GemVal gem_v_r = _t1501.fn(_t1501.env, _t1502, 1);
#line 662 "compiler/main.gem"
    GemVal _t1503 = gem_v_r;
                    gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1503, gem_string("setup")));
#line 663 "compiler/main.gem"
    GemVal _t1504 = gem_v_r;
    GemVal _t1505[] = {gem_v_arg_exprs, gem_table_get(_t1504, gem_string("expr"))};
                    (void)(gem_push_fn(NULL, _t1505, 2));
                }
            }
#line 665 "compiler/main.gem"
    GemVal _t1506[] = {gem_v_args};
            GemVal gem_v_argc = gem_len_fn(NULL, _t1506, 1);
#line 666 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 667 "compiler/main.gem"
    GemVal _t1507 = gem_table_new();
    GemVal _t1508[] = {gem_v_escaped_file};
    GemVal _t1509[] = {gem_v_line};
    gem_table_set(_t1507, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1508, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1509, 1)), gem_string(", NULL, 0)")));
    gem_table_set(_t1507, gem_string("setup"), gem_v_arg_setups);
                GemVal _t1510 = _t1507;
                gem_pop_frame();
                return _t1510;
            }
#line 669 "compiler/main.gem"
    GemVal _t1511 = (*gem_v_tmp);
            GemVal gem_v_t = _t1511.fn(_t1511.env, NULL, 0);
#line 670 "compiler/main.gem"
            GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
            {
#line 671 "compiler/main.gem"
                GemVal gem_v__for_i_27 = gem_int(1);
#line 671 "compiler/main.gem"
                GemVal gem_v__for_limit_27 = gem_v_argc;
#line 671 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 671 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_27;
#line 671 "compiler/main.gem"
                    gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 672 "compiler/main.gem"
                    gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                }
            }
#line 674 "compiler/main.gem"
    GemVal _t1512[] = {gem_v_t};
    GemVal _t1513[] = {gem_v_arr};
            GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1512, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1513, 1)), gem_string("};\n")));
#line 675 "compiler/main.gem"
    GemVal _t1514[] = {gem_v_escaped_file};
    GemVal _t1515[] = {gem_v_line};
    GemVal _t1516[] = {gem_v_t};
    GemVal _t1517[] = {gem_v_argc};
            GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1514, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1515, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1516, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1517, 1)), gem_string(")"));
#line 676 "compiler/main.gem"
    GemVal _t1518 = gem_table_new();
    gem_table_set(_t1518, gem_string("expr"), gem_v_call);
    gem_table_set(_t1518, gem_string("setup"), gem_v_setup);
            GemVal _t1519 = _t1518;
            gem_pop_frame();
            return _t1519;
        } else {
#line 677 "compiler/main.gem"
    GemVal _t1520[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t1520, 2))) {
#line 678 "compiler/main.gem"
                GemVal gem_v_fn_name = gem_table_get((*gem_v_builtin_fns), gem_v_name);
#line 679 "compiler/main.gem"
                GemVal gem_v_arg_setups = gem_string("");
#line 680 "compiler/main.gem"
    GemVal _t1521 = gem_table_new();
                GemVal gem_v_arg_exprs = _t1521;
                {
#line 681 "compiler/main.gem"
                    GemVal gem_v__for_i_28 = gem_int(0);
#line 681 "compiler/main.gem"
    GemVal _t1522[] = {gem_v_args};
                    GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1522, 1);
#line 681 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 681 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_28;
#line 681 "compiler/main.gem"
                        gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 682 "compiler/main.gem"
    GemVal _t1523 = (*gem_v_compile_expr);
    GemVal _t1524[] = {gem_table_get(gem_v_args, gem_v_i)};
                        GemVal gem_v_r = _t1523.fn(_t1523.env, _t1524, 1);
#line 683 "compiler/main.gem"
    GemVal _t1525 = gem_v_r;
                        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1525, gem_string("setup")));
#line 684 "compiler/main.gem"
    GemVal _t1526 = gem_v_r;
    GemVal _t1527[] = {gem_v_arg_exprs, gem_table_get(_t1526, gem_string("expr"))};
                        (void)(gem_push_fn(NULL, _t1527, 2));
                    }
                }
#line 686 "compiler/main.gem"
    GemVal _t1528[] = {gem_v_args};
                GemVal gem_v_argc = gem_len_fn(NULL, _t1528, 1);
#line 687 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 688 "compiler/main.gem"
    GemVal _t1529 = gem_table_new();
    GemVal _t1530[] = {gem_v_fn_name};
    gem_table_set(_t1529, gem_string("expr"), gem_add(gem_to_string_fn(NULL, _t1530, 1), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1529, gem_string("setup"), gem_v_arg_setups);
                    GemVal _t1531 = _t1529;
                    gem_pop_frame();
                    return _t1531;
                }
#line 690 "compiler/main.gem"
    GemVal _t1532 = (*gem_v_tmp);
                GemVal gem_v_t = _t1532.fn(_t1532.env, NULL, 0);
#line 691 "compiler/main.gem"
                GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
                {
#line 692 "compiler/main.gem"
                    GemVal gem_v__for_i_29 = gem_int(1);
#line 692 "compiler/main.gem"
                    GemVal gem_v__for_limit_29 = gem_v_argc;
#line 692 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_v__for_limit_29))) break;
#line 692 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_29;
#line 692 "compiler/main.gem"
                        gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 693 "compiler/main.gem"
                        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                    }
                }
#line 695 "compiler/main.gem"
    GemVal _t1533[] = {gem_v_t};
    GemVal _t1534[] = {gem_v_arr};
                GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1533, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1534, 1)), gem_string("};\n")));
#line 696 "compiler/main.gem"
    GemVal _t1535 = gem_table_new();
    GemVal _t1536[] = {gem_v_fn_name};
    GemVal _t1537[] = {gem_v_t};
    GemVal _t1538[] = {gem_v_argc};
    gem_table_set(_t1535, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1536, 1), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1537, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1538, 1)), gem_string(")")));
    gem_table_set(_t1535, gem_string("setup"), gem_v_setup);
                GemVal _t1539 = _t1535;
                gem_pop_frame();
                return _t1539;
            } else {
#line 697 "compiler/main.gem"
    GemVal _t1540[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t1540, 2))) {
#line 698 "compiler/main.gem"
                    GemVal gem_v_arg_setups = gem_string("");
#line 699 "compiler/main.gem"
    GemVal _t1541 = gem_table_new();
                    GemVal gem_v_arg_exprs = _t1541;
                    {
#line 700 "compiler/main.gem"
                        GemVal gem_v__for_i_30 = gem_int(0);
#line 700 "compiler/main.gem"
    GemVal _t1542[] = {gem_v_args};
                        GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t1542, 1);
#line 700 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 700 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_30;
#line 700 "compiler/main.gem"
                            gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 701 "compiler/main.gem"
    GemVal _t1543 = (*gem_v_compile_expr);
    GemVal _t1544[] = {gem_table_get(gem_v_args, gem_v_i)};
                            GemVal gem_v_r = _t1543.fn(_t1543.env, _t1544, 1);
#line 702 "compiler/main.gem"
    GemVal _t1545 = gem_v_r;
                            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1545, gem_string("setup")));
#line 703 "compiler/main.gem"
    GemVal _t1546 = gem_v_r;
    GemVal _t1547[] = {gem_v_arg_exprs, gem_table_get(_t1546, gem_string("expr"))};
                            (void)(gem_push_fn(NULL, _t1547, 2));
                        }
                    }
#line 705 "compiler/main.gem"
    GemVal _t1548[] = {gem_v_args};
                    GemVal gem_v_argc = gem_len_fn(NULL, _t1548, 1);
#line 706 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 707 "compiler/main.gem"
    GemVal _t1549 = gem_table_new();
    GemVal _t1550[] = {gem_v_name};
    gem_table_set(_t1549, gem_string("expr"), gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1550, 1)), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1549, gem_string("setup"), gem_v_arg_setups);
                        GemVal _t1551 = _t1549;
                        gem_pop_frame();
                        return _t1551;
                    }
#line 709 "compiler/main.gem"
    GemVal _t1552 = (*gem_v_tmp);
                    GemVal gem_v_t = _t1552.fn(_t1552.env, NULL, 0);
#line 710 "compiler/main.gem"
                    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
                    {
#line 711 "compiler/main.gem"
                        GemVal gem_v__for_i_31 = gem_int(1);
#line 711 "compiler/main.gem"
                        GemVal gem_v__for_limit_31 = gem_v_argc;
#line 711 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_v__for_limit_31))) break;
#line 711 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_31;
#line 711 "compiler/main.gem"
                            gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
#line 712 "compiler/main.gem"
                            gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                        }
                    }
#line 714 "compiler/main.gem"
    GemVal _t1553[] = {gem_v_t};
    GemVal _t1554[] = {gem_v_arr};
                    GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1553, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1554, 1)), gem_string("};\n")));
#line 715 "compiler/main.gem"
    GemVal _t1555 = gem_table_new();
    GemVal _t1556[] = {gem_v_name};
    GemVal _t1557[] = {gem_v_t};
    GemVal _t1558[] = {gem_v_argc};
    gem_table_set(_t1555, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1556, 1)), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1557, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1558, 1)), gem_string(")")));
    gem_table_set(_t1555, gem_string("setup"), gem_v_setup);
                    GemVal _t1559 = _t1555;
                    gem_pop_frame();
                    return _t1559;
                }
            }
        }
    }
#line 720 "compiler/main.gem"
    GemVal _t1560 = (*gem_v_compile_expr);
    GemVal _t1561[] = {gem_v_func};
    GemVal gem_v_func_r = _t1560.fn(_t1560.env, _t1561, 1);
#line 721 "compiler/main.gem"
    GemVal gem_v_arg_setups = gem_string("");
#line 722 "compiler/main.gem"
    GemVal _t1562 = gem_table_new();
    GemVal gem_v_arg_exprs = _t1562;
    {
#line 723 "compiler/main.gem"
        GemVal gem_v__for_i_32 = gem_int(0);
#line 723 "compiler/main.gem"
    GemVal _t1563[] = {gem_v_args};
        GemVal gem_v__for_limit_32 = gem_len_fn(NULL, _t1563, 1);
#line 723 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_v__for_limit_32))) break;
#line 723 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_32;
#line 723 "compiler/main.gem"
            gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 724 "compiler/main.gem"
    GemVal _t1564 = (*gem_v_compile_expr);
    GemVal _t1565[] = {gem_table_get(gem_v_args, gem_v_i)};
            GemVal gem_v_r = _t1564.fn(_t1564.env, _t1565, 1);
#line 725 "compiler/main.gem"
    GemVal _t1566 = gem_v_r;
            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1566, gem_string("setup")));
#line 726 "compiler/main.gem"
    GemVal _t1567 = gem_v_r;
    GemVal _t1568[] = {gem_v_arg_exprs, gem_table_get(_t1567, gem_string("expr"))};
            (void)(gem_push_fn(NULL, _t1568, 2));
        }
    }
#line 728 "compiler/main.gem"
    GemVal _t1569 = gem_v_func_r;
    GemVal gem_v_setup = gem_add(gem_table_get(_t1569, gem_string("setup")), gem_v_arg_setups);
#line 729 "compiler/main.gem"
    GemVal _t1570[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t1570, 1);
#line 730 "compiler/main.gem"
    GemVal _t1571 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t1571.fn(_t1571.env, NULL, 0);
#line 731 "compiler/main.gem"
    GemVal _t1572[] = {gem_v_tmp_fn};
    GemVal _t1573 = gem_v_func_r;
    GemVal _t1574[] = {gem_table_get(_t1573, gem_string("expr"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1572, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1574, 1)), gem_string(";\n")));
#line 732 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 733 "compiler/main.gem"
    GemVal _t1575 = gem_table_new();
    GemVal _t1576[] = {gem_v_tmp_fn};
    GemVal _t1577[] = {gem_v_tmp_fn};
    gem_table_set(_t1575, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1576, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1577, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t1575, gem_string("setup"), gem_v_setup);
        GemVal _t1578 = _t1575;
        gem_pop_frame();
        return _t1578;
    }
#line 735 "compiler/main.gem"
    GemVal _t1579 = (*gem_v_tmp);
    GemVal gem_v_tmp_args = _t1579.fn(_t1579.env, NULL, 0);
#line 736 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
    {
#line 737 "compiler/main.gem"
        GemVal gem_v__for_i_33 = gem_int(1);
#line 737 "compiler/main.gem"
        GemVal gem_v__for_limit_33 = gem_v_argc;
#line 737 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
#line 737 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_33;
#line 737 "compiler/main.gem"
            gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 738 "compiler/main.gem"
            gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
        }
    }
#line 740 "compiler/main.gem"
    GemVal _t1580[] = {gem_v_tmp_args};
    GemVal _t1581[] = {gem_v_arr};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1580, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1581, 1)), gem_string("};\n")));
#line 741 "compiler/main.gem"
    GemVal _t1582[] = {gem_v_tmp_fn};
    GemVal _t1583[] = {gem_v_tmp_fn};
    GemVal _t1584[] = {gem_v_tmp_args};
    GemVal _t1585[] = {gem_v_argc};
    GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1582, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1583, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t1584, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1585, 1)), gem_string(")"));
    GemVal _t1586 = gem_table_new();
    gem_table_set(_t1586, gem_string("expr"), gem_v_call);
    gem_table_set(_t1586, gem_string("setup"), gem_v_setup);
    GemVal _t1587 = _t1586;
    gem_pop_frame();
    return _t1587;
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
#line 748 "compiler/main.gem"
    GemVal _t1589 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t1589, gem_string("op"));
#line 749 "compiler/main.gem"
    GemVal _t1590 = gem_v_node;
    GemVal _t1591 = (*gem_v_compile_expr);
    GemVal _t1592[] = {gem_table_get(_t1590, gem_string("left"))};
    GemVal gem_v_lc_r = _t1591.fn(_t1591.env, _t1592, 1);
#line 750 "compiler/main.gem"
    GemVal _t1593 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t1593, gem_string("expr"));
#line 751 "compiler/main.gem"
    GemVal _t1594 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t1594, gem_string("setup"));
#line 753 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 754 "compiler/main.gem"
    GemVal _t1595 = gem_v_node;
    GemVal _t1596 = (*gem_v_compile_expr);
    GemVal _t1597[] = {gem_table_get(_t1595, gem_string("right"))};
        GemVal gem_v_rc_r = _t1596.fn(_t1596.env, _t1597, 1);
#line 755 "compiler/main.gem"
    GemVal _t1598 = gem_table_new();
    GemVal _t1599[] = {gem_v_lc};
    GemVal _t1600 = gem_v_rc_r;
    GemVal _t1601[] = {gem_table_get(_t1600, gem_string("expr"))};
    gem_table_set(_t1598, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1599, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1601, 1)), gem_string(")")));
    GemVal _t1602 = gem_v_rc_r;
    gem_table_set(_t1598, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1602, gem_string("setup"))));
        GemVal _t1603 = _t1598;
        gem_pop_frame();
        return _t1603;
    } else {
#line 756 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 757 "compiler/main.gem"
    GemVal _t1604 = gem_v_node;
    GemVal _t1605 = (*gem_v_compile_expr);
    GemVal _t1606[] = {gem_table_get(_t1604, gem_string("right"))};
            GemVal gem_v_rc_r = _t1605.fn(_t1605.env, _t1606, 1);
#line 758 "compiler/main.gem"
    GemVal _t1607 = gem_table_new();
    GemVal _t1608[] = {gem_v_lc};
    GemVal _t1609 = gem_v_rc_r;
    GemVal _t1610[] = {gem_table_get(_t1609, gem_string("expr"))};
    gem_table_set(_t1607, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t1608, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1610, 1)), gem_string(")")));
    GemVal _t1611 = gem_v_rc_r;
    gem_table_set(_t1607, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1611, gem_string("setup"))));
            GemVal _t1612 = _t1607;
            gem_pop_frame();
            return _t1612;
        } else {
#line 759 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 760 "compiler/main.gem"
    GemVal _t1613 = gem_v_node;
    GemVal _t1614 = (*gem_v_compile_expr);
    GemVal _t1615[] = {gem_table_get(_t1613, gem_string("right"))};
                GemVal gem_v_rc_r = _t1614.fn(_t1614.env, _t1615, 1);
#line 761 "compiler/main.gem"
    GemVal _t1616 = gem_table_new();
    GemVal _t1617[] = {gem_v_lc};
    GemVal _t1618 = gem_v_rc_r;
    GemVal _t1619[] = {gem_table_get(_t1618, gem_string("expr"))};
    gem_table_set(_t1616, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t1617, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1619, 1)), gem_string(")")));
    GemVal _t1620 = gem_v_rc_r;
    gem_table_set(_t1616, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1620, gem_string("setup"))));
                GemVal _t1621 = _t1616;
                gem_pop_frame();
                return _t1621;
            } else {
#line 762 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 763 "compiler/main.gem"
    GemVal _t1622 = gem_v_node;
    GemVal _t1623 = (*gem_v_compile_expr);
    GemVal _t1624[] = {gem_table_get(_t1622, gem_string("right"))};
                    GemVal gem_v_rc_r = _t1623.fn(_t1623.env, _t1624, 1);
#line 764 "compiler/main.gem"
    GemVal _t1625 = gem_table_new();
    GemVal _t1626[] = {gem_v_lc};
    GemVal _t1627 = gem_v_rc_r;
    GemVal _t1628[] = {gem_table_get(_t1627, gem_string("expr"))};
    gem_table_set(_t1625, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t1626, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1628, 1)), gem_string(")")));
    GemVal _t1629 = gem_v_rc_r;
    gem_table_set(_t1625, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1629, gem_string("setup"))));
                    GemVal _t1630 = _t1625;
                    gem_pop_frame();
                    return _t1630;
                } else {
#line 765 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 766 "compiler/main.gem"
    GemVal _t1631 = gem_v_node;
    GemVal _t1632 = (*gem_v_compile_expr);
    GemVal _t1633[] = {gem_table_get(_t1631, gem_string("right"))};
                        GemVal gem_v_rc_r = _t1632.fn(_t1632.env, _t1633, 1);
#line 767 "compiler/main.gem"
    GemVal _t1634 = gem_table_new();
    GemVal _t1635[] = {gem_v_lc};
    GemVal _t1636 = gem_v_rc_r;
    GemVal _t1637[] = {gem_table_get(_t1636, gem_string("expr"))};
    gem_table_set(_t1634, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t1635, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1637, 1)), gem_string(")")));
    GemVal _t1638 = gem_v_rc_r;
    gem_table_set(_t1634, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1638, gem_string("setup"))));
                        GemVal _t1639 = _t1634;
                        gem_pop_frame();
                        return _t1639;
                    } else {
#line 768 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 769 "compiler/main.gem"
    GemVal _t1640 = gem_v_node;
    GemVal _t1641 = (*gem_v_compile_expr);
    GemVal _t1642[] = {gem_table_get(_t1640, gem_string("right"))};
                            GemVal gem_v_rc_r = _t1641.fn(_t1641.env, _t1642, 1);
#line 770 "compiler/main.gem"
    GemVal _t1643 = (*gem_v_tmp);
                            GemVal gem_v_t = _t1643.fn(_t1643.env, NULL, 0);
#line 771 "compiler/main.gem"
    GemVal _t1644 = gem_v_rc_r;
    GemVal _t1645[] = {gem_v_t};
    GemVal _t1646 = gem_v_rc_r;
    GemVal _t1647[] = {gem_table_get(_t1646, gem_string("expr"))};
    GemVal _t1648[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t1644, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1645, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1647, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1648, 1)), gem_string("};\n")));
#line 772 "compiler/main.gem"
    GemVal _t1649 = gem_table_new();
    GemVal _t1650[] = {gem_v_t};
    gem_table_set(_t1649, gem_string("expr"), gem_add(gem_add(gem_string("gem_has_key_fn(NULL, "), gem_to_string_fn(NULL, _t1650, 1)), gem_string(", 2)")));
    gem_table_set(_t1649, gem_string("setup"), gem_v_setup);
                            GemVal _t1651 = _t1649;
                            gem_pop_frame();
                            return _t1651;
                        } else {
#line 773 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 774 "compiler/main.gem"
    GemVal _t1652 = gem_v_node;
    GemVal _t1653 = (*gem_v_compile_expr);
    GemVal _t1654[] = {gem_table_get(_t1652, gem_string("right"))};
                                GemVal gem_v_rc_r = _t1653.fn(_t1653.env, _t1654, 1);
#line 775 "compiler/main.gem"
    GemVal _t1655 = gem_table_new();
    GemVal _t1656[] = {gem_v_lc};
    GemVal _t1657 = gem_v_rc_r;
    GemVal _t1658[] = {gem_table_get(_t1657, gem_string("expr"))};
    gem_table_set(_t1655, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t1656, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1658, 1)), gem_string(")")));
    GemVal _t1659 = gem_v_rc_r;
    gem_table_set(_t1655, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1659, gem_string("setup"))));
                                GemVal _t1660 = _t1655;
                                gem_pop_frame();
                                return _t1660;
                            } else {
#line 776 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 777 "compiler/main.gem"
    GemVal _t1661 = gem_v_node;
    GemVal _t1662 = (*gem_v_compile_expr);
    GemVal _t1663[] = {gem_table_get(_t1661, gem_string("right"))};
                                    GemVal gem_v_rc_r = _t1662.fn(_t1662.env, _t1663, 1);
#line 778 "compiler/main.gem"
    GemVal _t1664 = gem_table_new();
    GemVal _t1665[] = {gem_v_lc};
    GemVal _t1666 = gem_v_rc_r;
    GemVal _t1667[] = {gem_table_get(_t1666, gem_string("expr"))};
    gem_table_set(_t1664, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t1665, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1667, 1)), gem_string(")")));
    GemVal _t1668 = gem_v_rc_r;
    gem_table_set(_t1664, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1668, gem_string("setup"))));
                                    GemVal _t1669 = _t1664;
                                    gem_pop_frame();
                                    return _t1669;
                                } else {
#line 779 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 780 "compiler/main.gem"
    GemVal _t1670 = gem_v_node;
    GemVal _t1671 = (*gem_v_compile_expr);
    GemVal _t1672[] = {gem_table_get(_t1670, gem_string("right"))};
                                        GemVal gem_v_rc_r = _t1671.fn(_t1671.env, _t1672, 1);
#line 781 "compiler/main.gem"
    GemVal _t1673 = gem_table_new();
    GemVal _t1674[] = {gem_v_lc};
    GemVal _t1675 = gem_v_rc_r;
    GemVal _t1676[] = {gem_table_get(_t1675, gem_string("expr"))};
    gem_table_set(_t1673, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t1674, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1676, 1)), gem_string(")")));
    GemVal _t1677 = gem_v_rc_r;
    gem_table_set(_t1673, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1677, gem_string("setup"))));
                                        GemVal _t1678 = _t1673;
                                        gem_pop_frame();
                                        return _t1678;
                                    } else {
#line 782 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 783 "compiler/main.gem"
    GemVal _t1679 = gem_v_node;
    GemVal _t1680 = (*gem_v_compile_expr);
    GemVal _t1681[] = {gem_table_get(_t1679, gem_string("right"))};
                                            GemVal gem_v_rc_r = _t1680.fn(_t1680.env, _t1681, 1);
#line 784 "compiler/main.gem"
    GemVal _t1682 = gem_table_new();
    GemVal _t1683[] = {gem_v_lc};
    GemVal _t1684 = gem_v_rc_r;
    GemVal _t1685[] = {gem_table_get(_t1684, gem_string("expr"))};
    gem_table_set(_t1682, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t1683, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1685, 1)), gem_string(")")));
    GemVal _t1686 = gem_v_rc_r;
    gem_table_set(_t1682, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1686, gem_string("setup"))));
                                            GemVal _t1687 = _t1682;
                                            gem_pop_frame();
                                            return _t1687;
                                        } else {
#line 785 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 786 "compiler/main.gem"
    GemVal _t1688 = gem_v_node;
    GemVal _t1689 = (*gem_v_compile_expr);
    GemVal _t1690[] = {gem_table_get(_t1688, gem_string("right"))};
                                                GemVal gem_v_rc_r = _t1689.fn(_t1689.env, _t1690, 1);
#line 787 "compiler/main.gem"
    GemVal _t1691 = gem_table_new();
    GemVal _t1692[] = {gem_v_lc};
    GemVal _t1693 = gem_v_rc_r;
    GemVal _t1694[] = {gem_table_get(_t1693, gem_string("expr"))};
    gem_table_set(_t1691, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t1692, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1694, 1)), gem_string(")")));
    GemVal _t1695 = gem_v_rc_r;
    gem_table_set(_t1691, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1695, gem_string("setup"))));
                                                GemVal _t1696 = _t1691;
                                                gem_pop_frame();
                                                return _t1696;
                                            } else {
#line 788 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 789 "compiler/main.gem"
    GemVal _t1697 = gem_v_node;
    GemVal _t1698 = (*gem_v_compile_expr);
    GemVal _t1699[] = {gem_table_get(_t1697, gem_string("right"))};
                                                    GemVal gem_v_rc_r = _t1698.fn(_t1698.env, _t1699, 1);
#line 790 "compiler/main.gem"
    GemVal _t1700 = gem_table_new();
    GemVal _t1701[] = {gem_v_lc};
    GemVal _t1702 = gem_v_rc_r;
    GemVal _t1703[] = {gem_table_get(_t1702, gem_string("expr"))};
    gem_table_set(_t1700, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t1701, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1703, 1)), gem_string(")")));
    GemVal _t1704 = gem_v_rc_r;
    gem_table_set(_t1700, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1704, gem_string("setup"))));
                                                    GemVal _t1705 = _t1700;
                                                    gem_pop_frame();
                                                    return _t1705;
                                                } else {
#line 791 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 792 "compiler/main.gem"
    GemVal _t1706 = gem_v_node;
    GemVal _t1707 = (*gem_v_compile_expr);
    GemVal _t1708[] = {gem_table_get(_t1706, gem_string("right"))};
                                                        GemVal gem_v_rc_r = _t1707.fn(_t1707.env, _t1708, 1);
#line 793 "compiler/main.gem"
    GemVal _t1709 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1709.fn(_t1709.env, NULL, 0);
#line 794 "compiler/main.gem"
                                                        GemVal gem_v_setup = gem_v_ls;
#line 795 "compiler/main.gem"
    GemVal _t1710[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1710, 1)), gem_string(";\n")));
#line 796 "compiler/main.gem"
    GemVal _t1711[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t1711, 1)), gem_string(")) {\n")));
#line 797 "compiler/main.gem"
    GemVal _t1712[] = {gem_v_t};
    GemVal _t1713[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1712, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1713, 1)), gem_string(";\n")));
#line 798 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 799 "compiler/main.gem"
    GemVal _t1714 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t1714, gem_string("setup")), gem_string("")))) {
#line 800 "compiler/main.gem"
    GemVal _t1715 = gem_v_rc_r;
    GemVal _t1716[] = {gem_table_get(_t1715, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t1716, 3));
                                                        }
#line 802 "compiler/main.gem"
    GemVal _t1717[] = {gem_v_t};
    GemVal _t1718 = gem_v_rc_r;
    GemVal _t1719[] = {gem_table_get(_t1718, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1717, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1719, 1)), gem_string(";\n")));
#line 803 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 804 "compiler/main.gem"
    GemVal _t1720 = gem_table_new();
    gem_table_set(_t1720, gem_string("expr"), gem_v_t);
    gem_table_set(_t1720, gem_string("setup"), gem_v_setup);
                                                        GemVal _t1721 = _t1720;
                                                        gem_pop_frame();
                                                        return _t1721;
                                                    } else {
#line 805 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 806 "compiler/main.gem"
    GemVal _t1722 = gem_v_node;
    GemVal _t1723 = (*gem_v_compile_expr);
    GemVal _t1724[] = {gem_table_get(_t1722, gem_string("right"))};
                                                            GemVal gem_v_rc_r = _t1723.fn(_t1723.env, _t1724, 1);
#line 807 "compiler/main.gem"
    GemVal _t1725 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1725.fn(_t1725.env, NULL, 0);
#line 808 "compiler/main.gem"
                                                            GemVal gem_v_setup = gem_v_ls;
#line 809 "compiler/main.gem"
    GemVal _t1726[] = {gem_v_t};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1726, 1)), gem_string(";\n")));
#line 810 "compiler/main.gem"
    GemVal _t1727[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t1727, 1)), gem_string(")) {\n")));
#line 811 "compiler/main.gem"
    GemVal _t1728[] = {gem_v_t};
    GemVal _t1729[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1728, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1729, 1)), gem_string(";\n")));
#line 812 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 813 "compiler/main.gem"
    GemVal _t1730 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t1730, gem_string("setup")), gem_string("")))) {
#line 814 "compiler/main.gem"
    GemVal _t1731 = gem_v_rc_r;
    GemVal _t1732[] = {gem_table_get(_t1731, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                                gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t1732, 3));
                                                            }
#line 816 "compiler/main.gem"
    GemVal _t1733[] = {gem_v_t};
    GemVal _t1734 = gem_v_rc_r;
    GemVal _t1735[] = {gem_table_get(_t1734, gem_string("expr"))};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1733, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1735, 1)), gem_string(";\n")));
#line 817 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 818 "compiler/main.gem"
    GemVal _t1736 = gem_table_new();
    gem_table_set(_t1736, gem_string("expr"), gem_v_t);
    gem_table_set(_t1736, gem_string("setup"), gem_v_setup);
                                                            GemVal _t1737 = _t1736;
                                                            gem_pop_frame();
                                                            return _t1737;
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
#line 821 "compiler/main.gem"
    GemVal _t1738 = gem_v_node;
    GemVal _t1739 = (*gem_v_compile_expr);
    GemVal _t1740[] = {gem_table_get(_t1738, gem_string("right"))};
    GemVal gem_v_rc_r = _t1739.fn(_t1739.env, _t1740, 1);
    GemVal _t1741 = gem_table_new();
    gem_table_set(_t1741, gem_string("expr"), gem_string("GEM_NIL"));
    GemVal _t1742 = gem_v_rc_r;
    gem_table_set(_t1741, gem_string("setup"), gem_add(gem_add(gem_add(gem_add(gem_v_ls, gem_table_get(_t1742, gem_string("setup"))), gem_string("/* unknown op: ")), gem_v_op), gem_string(" */\n")));
    GemVal _t1743 = _t1741;
    gem_pop_frame();
    return _t1743;
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
#line 828 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
    {
#line 829 "compiler/main.gem"
        GemVal gem_v__for_items_34 = gem_v_stmts;
#line 829 "compiler/main.gem"
        GemVal gem_v__for_i_34 = gem_int(0);
#line 829 "compiler/main.gem"
        while (1) {
            GemVal _t1745[] = {gem_v__for_items_34};
            if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_len_fn(NULL, _t1745, 1)))) break;
#line 829 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_34, gem_v__for_i_34);
#line 829 "compiler/main.gem"
            gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 830 "compiler/main.gem"
    GemVal _t1746 = (*gem_v_compile_stmt);
    GemVal _t1747[] = {gem_v_stmt, gem_v_indent};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1746.fn(_t1746.env, _t1747, 2)), gem_string("\n"));
        }
    }
    GemVal _t1748 = gem_v_out;
    gem_pop_frame();
    return _t1748;
}

struct _closure__anon_37 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_if;
    GemVal *gem_v_compile_match;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_while;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_mangle;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tmp;
    GemVal *gem_v_top_level_boxed;
    GemVal *gem_v_top_level_vars;
};
static GemVal _anon_37(void *_env, GemVal *args, int argc) {
    struct _closure__anon_37 *_cls = (struct _closure__anon_37 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_if = _cls->gem_v_compile_if;
    GemVal *gem_v_compile_match = _cls->gem_v_compile_match;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_while = _cls->gem_v_compile_while;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal *gem_v_top_level_boxed = _cls->gem_v_top_level_boxed;
    GemVal *gem_v_top_level_vars = _cls->gem_v_top_level_vars;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_37", "compiler/main.gem", 0);
#line 838 "compiler/main.gem"
    GemVal _t1750[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1750, 1);
#line 839 "compiler/main.gem"
    GemVal _t1751 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1751, gem_string("tag"));
#line 840 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 841 "compiler/main.gem"
    GemVal _t1752 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1752, gem_string("line")), GEM_NIL))) {
#line 842 "compiler/main.gem"
    GemVal _t1753 = gem_v_node;
    GemVal _t1754[] = {gem_table_get(_t1753, gem_string("line"))};
    GemVal _t1755[] = {(*gem_v_source_name)};
    GemVal _t1756[] = {gem_fn_escape_c_string(NULL, _t1755, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t1754, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t1756, 1)), gem_string("\"\n"));
    }
#line 845 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 846 "compiler/main.gem"
    GemVal _t1757 = gem_v_node;
    GemVal _t1758 = (*gem_v_compile_expr);
    GemVal _t1759[] = {gem_table_get(_t1757, gem_string("value"))};
        GemVal gem_v_r = _t1758.fn(_t1758.env, _t1759, 1);
#line 847 "compiler/main.gem"
    GemVal _t1760 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1760, gem_string("name"));
#line 848 "compiler/main.gem"
    GemVal _t1761 = (*gem_v_mangle);
    GemVal _t1762[] = {gem_v_name};
        GemVal gem_v_mname = _t1761.fn(_t1761.env, _t1762, 1);
#line 849 "compiler/main.gem"
    GemVal _t1764;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t1764 = (*gem_v_in_top_level);
    } else {
        GemVal _t1763[] = {(*gem_v_top_level_vars), gem_v_name};
        _t1764 = gem_fn_set_contains(NULL, _t1763, 2);
    }
        if (gem_truthy(_t1764)) {
#line 850 "compiler/main.gem"
    GemVal _t1765[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1765, 2))) {
#line 851 "compiler/main.gem"
    GemVal _t1766[] = {gem_v_line_dir};
    GemVal _t1767 = gem_v_r;
    GemVal _t1768[] = {gem_table_get(_t1767, gem_string("setup"))};
    GemVal _t1769[] = {gem_v_p};
    GemVal _t1770[] = {gem_v_mname};
    GemVal _t1771[] = {gem_v_p};
    GemVal _t1772[] = {gem_v_mname};
    GemVal _t1773 = gem_v_r;
    GemVal _t1774[] = {gem_table_get(_t1773, gem_string("expr"))};
                GemVal _t1775 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1766, 1), gem_to_string_fn(NULL, _t1768, 1)), gem_to_string_fn(NULL, _t1769, 1)), gem_to_string_fn(NULL, _t1770, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1771, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1772, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1774, 1)), gem_string(";"));
                gem_pop_frame();
                return _t1775;
            }
#line 853 "compiler/main.gem"
    GemVal _t1776[] = {gem_v_line_dir};
    GemVal _t1777 = gem_v_r;
    GemVal _t1778[] = {gem_table_get(_t1777, gem_string("setup"))};
    GemVal _t1779[] = {gem_v_p};
    GemVal _t1780[] = {gem_v_mname};
    GemVal _t1781 = gem_v_r;
    GemVal _t1782[] = {gem_table_get(_t1781, gem_string("expr"))};
            GemVal _t1783 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1776, 1), gem_to_string_fn(NULL, _t1778, 1)), gem_to_string_fn(NULL, _t1779, 1)), gem_to_string_fn(NULL, _t1780, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1782, 1)), gem_string(";"));
            gem_pop_frame();
            return _t1783;
        }
#line 855 "compiler/main.gem"
    GemVal _t1784[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1784, 2))) {
#line 856 "compiler/main.gem"
    GemVal _t1785[] = {gem_v_line_dir};
    GemVal _t1786 = gem_v_r;
    GemVal _t1787[] = {gem_table_get(_t1786, gem_string("setup"))};
    GemVal _t1788[] = {gem_v_p};
    GemVal _t1789[] = {gem_v_mname};
    GemVal _t1790[] = {gem_v_p};
    GemVal _t1791[] = {gem_v_mname};
    GemVal _t1792 = gem_v_r;
    GemVal _t1793[] = {gem_table_get(_t1792, gem_string("expr"))};
            GemVal _t1794 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1785, 1), gem_to_string_fn(NULL, _t1787, 1)), gem_to_string_fn(NULL, _t1788, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t1789, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1790, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1791, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1793, 1)), gem_string(";"));
            gem_pop_frame();
            return _t1794;
        }
#line 858 "compiler/main.gem"
    GemVal _t1795[] = {gem_v_line_dir};
    GemVal _t1796 = gem_v_r;
    GemVal _t1797[] = {gem_table_get(_t1796, gem_string("setup"))};
    GemVal _t1798[] = {gem_v_p};
    GemVal _t1799[] = {gem_v_mname};
    GemVal _t1800 = gem_v_r;
    GemVal _t1801[] = {gem_table_get(_t1800, gem_string("expr"))};
        GemVal _t1802 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1795, 1), gem_to_string_fn(NULL, _t1797, 1)), gem_to_string_fn(NULL, _t1798, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1799, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1801, 1)), gem_string(";"));
        gem_pop_frame();
        return _t1802;
    } else {
#line 859 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 860 "compiler/main.gem"
    GemVal _t1803 = gem_v_node;
    GemVal _t1804 = (*gem_v_compile_expr);
    GemVal _t1805[] = {gem_table_get(_t1803, gem_string("value"))};
            GemVal gem_v_r = _t1804.fn(_t1804.env, _t1805, 1);
#line 861 "compiler/main.gem"
    GemVal _t1806 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1806, gem_string("name"));
#line 862 "compiler/main.gem"
    GemVal _t1807 = (*gem_v_mangle);
    GemVal _t1808[] = {gem_v_name};
            GemVal gem_v_mname = _t1807.fn(_t1807.env, _t1808, 1);
#line 863 "compiler/main.gem"
    GemVal _t1809[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t1811;
    if (gem_truthy(gem_fn_set_contains(NULL, _t1809, 2))) {
        _t1811 = gem_fn_set_contains(NULL, _t1809, 2);
    } else {
        GemVal _t1810[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t1811 = gem_fn_set_contains(NULL, _t1810, 2);
    }
            if (gem_truthy(_t1811)) {
#line 864 "compiler/main.gem"
    GemVal _t1812[] = {gem_v_line_dir};
    GemVal _t1813 = gem_v_r;
    GemVal _t1814[] = {gem_table_get(_t1813, gem_string("setup"))};
    GemVal _t1815[] = {gem_v_p};
    GemVal _t1816[] = {gem_v_mname};
    GemVal _t1817 = gem_v_r;
    GemVal _t1818[] = {gem_table_get(_t1817, gem_string("expr"))};
                GemVal _t1819 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1812, 1), gem_to_string_fn(NULL, _t1814, 1)), gem_to_string_fn(NULL, _t1815, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1816, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1818, 1)), gem_string(";"));
                gem_pop_frame();
                return _t1819;
            }
#line 866 "compiler/main.gem"
    GemVal _t1820[] = {gem_v_line_dir};
    GemVal _t1821 = gem_v_r;
    GemVal _t1822[] = {gem_table_get(_t1821, gem_string("setup"))};
    GemVal _t1823[] = {gem_v_p};
    GemVal _t1824[] = {gem_v_mname};
    GemVal _t1825 = gem_v_r;
    GemVal _t1826[] = {gem_table_get(_t1825, gem_string("expr"))};
            GemVal _t1827 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1820, 1), gem_to_string_fn(NULL, _t1822, 1)), gem_to_string_fn(NULL, _t1823, 1)), gem_to_string_fn(NULL, _t1824, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1826, 1)), gem_string(";"));
            gem_pop_frame();
            return _t1827;
        } else {
#line 867 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
#line 868 "compiler/main.gem"
    GemVal _t1828 = gem_v_node;
    GemVal _t1829 = (*gem_v_compile_expr);
    GemVal _t1830[] = {gem_table_get(_t1828, gem_string("object"))};
                GemVal gem_v_obj_r = _t1829.fn(_t1829.env, _t1830, 1);
#line 869 "compiler/main.gem"
    GemVal _t1831 = gem_v_node;
    GemVal _t1832 = (*gem_v_compile_expr);
    GemVal _t1833[] = {gem_table_get(_t1831, gem_string("value"))};
                GemVal gem_v_val_r = _t1832.fn(_t1832.env, _t1833, 1);
#line 870 "compiler/main.gem"
    GemVal _t1834 = gem_v_node;
    GemVal _t1835[] = {gem_table_get(_t1834, gem_string("field"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1835, 1);
#line 871 "compiler/main.gem"
    GemVal _t1836[] = {gem_v_line_dir};
    GemVal _t1837 = gem_v_obj_r;
    GemVal _t1838[] = {gem_table_get(_t1837, gem_string("setup"))};
    GemVal _t1839 = gem_v_val_r;
    GemVal _t1840[] = {gem_table_get(_t1839, gem_string("setup"))};
    GemVal _t1841[] = {gem_v_p};
    GemVal _t1842 = gem_v_obj_r;
    GemVal _t1843[] = {gem_table_get(_t1842, gem_string("expr"))};
    GemVal _t1844[] = {gem_v_escaped};
    GemVal _t1845 = gem_v_val_r;
    GemVal _t1846[] = {gem_table_get(_t1845, gem_string("expr"))};
                GemVal _t1847 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1836, 1), gem_to_string_fn(NULL, _t1838, 1)), gem_to_string_fn(NULL, _t1840, 1)), gem_to_string_fn(NULL, _t1841, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t1843, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1844, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1846, 1)), gem_string(");"));
                gem_pop_frame();
                return _t1847;
            } else {
#line 872 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("index_assign")))) {
#line 873 "compiler/main.gem"
    GemVal _t1848 = gem_v_node;
    GemVal _t1849 = (*gem_v_compile_expr);
    GemVal _t1850[] = {gem_table_get(_t1848, gem_string("object"))};
                    GemVal gem_v_obj_r = _t1849.fn(_t1849.env, _t1850, 1);
#line 874 "compiler/main.gem"
    GemVal _t1851 = gem_v_node;
    GemVal _t1852 = (*gem_v_compile_expr);
    GemVal _t1853[] = {gem_table_get(_t1851, gem_string("key"))};
                    GemVal gem_v_key_r = _t1852.fn(_t1852.env, _t1853, 1);
#line 875 "compiler/main.gem"
    GemVal _t1854 = gem_v_node;
    GemVal _t1855 = (*gem_v_compile_expr);
    GemVal _t1856[] = {gem_table_get(_t1854, gem_string("value"))};
                    GemVal gem_v_val_r = _t1855.fn(_t1855.env, _t1856, 1);
#line 876 "compiler/main.gem"
    GemVal _t1857[] = {gem_v_line_dir};
    GemVal _t1858 = gem_v_obj_r;
    GemVal _t1859[] = {gem_table_get(_t1858, gem_string("setup"))};
    GemVal _t1860 = gem_v_key_r;
    GemVal _t1861[] = {gem_table_get(_t1860, gem_string("setup"))};
    GemVal _t1862 = gem_v_val_r;
    GemVal _t1863[] = {gem_table_get(_t1862, gem_string("setup"))};
    GemVal _t1864[] = {gem_v_p};
    GemVal _t1865 = gem_v_obj_r;
    GemVal _t1866[] = {gem_table_get(_t1865, gem_string("expr"))};
    GemVal _t1867 = gem_v_key_r;
    GemVal _t1868[] = {gem_table_get(_t1867, gem_string("expr"))};
    GemVal _t1869 = gem_v_val_r;
    GemVal _t1870[] = {gem_table_get(_t1869, gem_string("expr"))};
                    GemVal _t1871 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1857, 1), gem_to_string_fn(NULL, _t1859, 1)), gem_to_string_fn(NULL, _t1861, 1)), gem_to_string_fn(NULL, _t1863, 1)), gem_to_string_fn(NULL, _t1864, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t1866, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1868, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1870, 1)), gem_string(");"));
                    gem_pop_frame();
                    return _t1871;
                } else {
#line 877 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 878 "compiler/main.gem"
    GemVal _t1872[] = {gem_v_line_dir};
    GemVal _t1873 = (*gem_v_compile_if);
    GemVal _t1874[] = {gem_v_node, gem_v_indent};
                        GemVal _t1875 = gem_add(gem_to_string_fn(NULL, _t1872, 1), _t1873.fn(_t1873.env, _t1874, 2));
                        gem_pop_frame();
                        return _t1875;
                    } else {
#line 879 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 880 "compiler/main.gem"
    GemVal _t1876[] = {gem_v_line_dir};
    GemVal _t1877 = (*gem_v_compile_while);
    GemVal _t1878[] = {gem_v_node, gem_v_indent};
                            GemVal _t1879 = gem_add(gem_to_string_fn(NULL, _t1876, 1), _t1877.fn(_t1877.env, _t1878, 2));
                            gem_pop_frame();
                            return _t1879;
                        } else {
#line 881 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 882 "compiler/main.gem"
    GemVal _t1880[] = {gem_v_line_dir};
    GemVal _t1881 = (*gem_v_compile_match);
    GemVal _t1882[] = {gem_v_node, gem_v_indent};
                                GemVal _t1883 = gem_add(gem_to_string_fn(NULL, _t1880, 1), _t1881.fn(_t1881.env, _t1882, 2));
                                gem_pop_frame();
                                return _t1883;
                            } else {
#line 883 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("break")))) {
#line 884 "compiler/main.gem"
                                    GemVal _t1884 = gem_add(gem_v_p, gem_string("break;"));
                                    gem_pop_frame();
                                    return _t1884;
                                } else {
#line 885 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("continue")))) {
#line 886 "compiler/main.gem"
                                        GemVal _t1885 = gem_add(gem_v_p, gem_string("continue;"));
                                        gem_pop_frame();
                                        return _t1885;
                                    } else {
#line 887 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 888 "compiler/main.gem"
    GemVal _t1886 = gem_v_node;
                                            if (gem_truthy(gem_neq(gem_table_get(_t1886, gem_string("value")), GEM_NIL))) {
#line 889 "compiler/main.gem"
    GemVal _t1887 = gem_v_node;
    GemVal _t1888 = (*gem_v_compile_expr);
    GemVal _t1889[] = {gem_table_get(_t1887, gem_string("value"))};
                                                GemVal gem_v_r = _t1888.fn(_t1888.env, _t1889, 1);
#line 890 "compiler/main.gem"
    GemVal _t1890 = (*gem_v_tmp);
                                                GemVal gem_v_t = _t1890.fn(_t1890.env, NULL, 0);
#line 891 "compiler/main.gem"
    GemVal _t1891[] = {gem_v_line_dir};
    GemVal _t1892 = gem_v_r;
    GemVal _t1893[] = {gem_table_get(_t1892, gem_string("setup"))};
    GemVal _t1894[] = {gem_v_p};
    GemVal _t1895[] = {gem_v_t};
    GemVal _t1896 = gem_v_r;
    GemVal _t1897[] = {gem_table_get(_t1896, gem_string("expr"))};
    GemVal _t1898[] = {gem_v_p};
    GemVal _t1899[] = {gem_v_p};
    GemVal _t1900[] = {gem_v_t};
                                                GemVal _t1901 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1891, 1), gem_to_string_fn(NULL, _t1893, 1)), gem_to_string_fn(NULL, _t1894, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1895, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1897, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1898, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1899, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1900, 1)), gem_string(";"));
                                                gem_pop_frame();
                                                return _t1901;
                                            }
#line 893 "compiler/main.gem"
    GemVal _t1902[] = {gem_v_line_dir};
    GemVal _t1903[] = {gem_v_p};
    GemVal _t1904[] = {gem_v_p};
                                            GemVal _t1905 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1902, 1), gem_to_string_fn(NULL, _t1903, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1904, 1)), gem_string("return GEM_NIL;"));
                                            gem_pop_frame();
                                            return _t1905;
                                        } else {
#line 894 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 895 "compiler/main.gem"
    GemVal _t1906 = (*gem_v_compile_expr);
    GemVal _t1907[] = {gem_v_node};
                                                GemVal gem_v_r = _t1906.fn(_t1906.env, _t1907, 1);
#line 896 "compiler/main.gem"
    GemVal _t1908[] = {gem_v_line_dir};
    GemVal _t1909 = gem_v_r;
    GemVal _t1910[] = {gem_table_get(_t1909, gem_string("setup"))};
    GemVal _t1911[] = {gem_v_p};
    GemVal _t1912 = gem_v_r;
    GemVal _t1913[] = {gem_table_get(_t1912, gem_string("expr"))};
                                                GemVal _t1914 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1908, 1), gem_to_string_fn(NULL, _t1910, 1)), gem_to_string_fn(NULL, _t1911, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t1913, 1)), gem_string(");"));
                                                gem_pop_frame();
                                                return _t1914;
                                            } else {
#line 897 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
#line 898 "compiler/main.gem"
                                                    GemVal _t1915 = gem_string("");
                                                    gem_pop_frame();
                                                    return _t1915;
                                                } else {
#line 899 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 900 "compiler/main.gem"
    GemVal _t1916[] = {gem_v_p};
                                                        GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t1916, 1), gem_string("{\n"));
                                                        {
#line 901 "compiler/main.gem"
    GemVal _t1917 = gem_v_node;
                                                            GemVal gem_v__for_items_35 = gem_table_get(_t1917, gem_string("stmts"));
#line 901 "compiler/main.gem"
                                                            GemVal gem_v__for_i_35 = gem_int(0);
#line 901 "compiler/main.gem"
                                                            while (1) {
                                                                GemVal _t1918[] = {gem_v__for_items_35};
                                                                if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_len_fn(NULL, _t1918, 1)))) break;
#line 901 "compiler/main.gem"
                                                                GemVal gem_v_stmt = gem_table_get(gem_v__for_items_35, gem_v__for_i_35);
#line 901 "compiler/main.gem"
                                                                gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 902 "compiler/main.gem"
    GemVal _t1919 = (*gem_v_compile_stmt);
    GemVal _t1920[] = {gem_v_stmt, gem_add(gem_v_indent, gem_int(1))};
                                                                gem_v_out = gem_add(gem_add(gem_v_out, _t1919.fn(_t1919.env, _t1920, 2)), gem_string("\n"));
                                                            }
                                                        }
#line 904 "compiler/main.gem"
    GemVal _t1921[] = {gem_v_p};
                                                        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1921, 1), gem_string("}")));
#line 905 "compiler/main.gem"
                                                        GemVal _t1922 = gem_v_out;
                                                        gem_pop_frame();
                                                        return _t1922;
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
#line 908 "compiler/main.gem"
    GemVal _t1923 = (*gem_v_compile_expr);
    GemVal _t1924[] = {gem_v_node};
    GemVal gem_v_r = _t1923.fn(_t1923.env, _t1924, 1);
    GemVal _t1925[] = {gem_v_line_dir};
    GemVal _t1926 = gem_v_r;
    GemVal _t1927[] = {gem_table_get(_t1926, gem_string("setup"))};
    GemVal _t1928[] = {gem_v_p};
    GemVal _t1929 = gem_v_r;
    GemVal _t1930[] = {gem_table_get(_t1929, gem_string("expr"))};
    GemVal _t1931 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1925, 1), gem_to_string_fn(NULL, _t1927, 1)), gem_to_string_fn(NULL, _t1928, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t1930, 1)), gem_string(");"));
    gem_pop_frame();
    return _t1931;
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
    GemVal *gem_v_top_level_boxed;
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
    GemVal *gem_v_top_level_boxed = _cls->gem_v_top_level_boxed;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_38", "compiler/main.gem", 0);
#line 915 "compiler/main.gem"
    GemVal _t1933[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1933, 1);
#line 916 "compiler/main.gem"
    GemVal _t1934 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1934, gem_string("tag"));
#line 917 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 918 "compiler/main.gem"
    GemVal _t1935 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1935, gem_string("line")), GEM_NIL))) {
#line 919 "compiler/main.gem"
    GemVal _t1936 = gem_v_node;
    GemVal _t1937[] = {gem_table_get(_t1936, gem_string("line"))};
    GemVal _t1938[] = {(*gem_v_source_name)};
    GemVal _t1939[] = {gem_fn_escape_c_string(NULL, _t1938, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t1937, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t1939, 1)), gem_string("\"\n"));
    }
#line 922 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 923 "compiler/main.gem"
    GemVal _t1940 = gem_v_node;
    GemVal _t1941 = (*gem_v_compile_expr);
    GemVal _t1942[] = {gem_table_get(_t1940, gem_string("value"))};
        GemVal gem_v_r = _t1941.fn(_t1941.env, _t1942, 1);
#line 924 "compiler/main.gem"
    GemVal _t1943 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1943, gem_string("name"));
#line 925 "compiler/main.gem"
    GemVal _t1944 = (*gem_v_mangle);
    GemVal _t1945[] = {gem_v_name};
        GemVal gem_v_mname = _t1944.fn(_t1944.env, _t1945, 1);
#line 926 "compiler/main.gem"
    GemVal _t1946[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1946, 2))) {
#line 927 "compiler/main.gem"
    GemVal _t1947[] = {gem_v_line_dir};
    GemVal _t1948 = gem_v_r;
    GemVal _t1949[] = {gem_table_get(_t1948, gem_string("setup"))};
    GemVal _t1950[] = {gem_v_p};
    GemVal _t1951[] = {gem_v_mname};
    GemVal _t1952[] = {gem_v_p};
    GemVal _t1953[] = {gem_v_mname};
    GemVal _t1954 = gem_v_r;
    GemVal _t1955[] = {gem_table_get(_t1954, gem_string("expr"))};
    GemVal _t1956[] = {gem_v_p};
    GemVal _t1957[] = {gem_v_p};
            GemVal _t1958 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1947, 1), gem_to_string_fn(NULL, _t1949, 1)), gem_to_string_fn(NULL, _t1950, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t1951, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1952, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1953, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1955, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1956, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1957, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t1958;
        }
#line 929 "compiler/main.gem"
    GemVal _t1959[] = {gem_v_line_dir};
    GemVal _t1960 = gem_v_r;
    GemVal _t1961[] = {gem_table_get(_t1960, gem_string("setup"))};
    GemVal _t1962[] = {gem_v_p};
    GemVal _t1963[] = {gem_v_mname};
    GemVal _t1964 = gem_v_r;
    GemVal _t1965[] = {gem_table_get(_t1964, gem_string("expr"))};
    GemVal _t1966[] = {gem_v_p};
    GemVal _t1967[] = {gem_v_p};
        GemVal _t1968 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1959, 1), gem_to_string_fn(NULL, _t1961, 1)), gem_to_string_fn(NULL, _t1962, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1963, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1965, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1966, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1967, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t1968;
    } else {
#line 930 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 931 "compiler/main.gem"
    GemVal _t1969 = gem_v_node;
    GemVal _t1970 = (*gem_v_compile_expr);
    GemVal _t1971[] = {gem_table_get(_t1969, gem_string("value"))};
            GemVal gem_v_r = _t1970.fn(_t1970.env, _t1971, 1);
#line 932 "compiler/main.gem"
    GemVal _t1972 = (*gem_v_tmp);
            GemVal gem_v_t = _t1972.fn(_t1972.env, NULL, 0);
#line 933 "compiler/main.gem"
    GemVal _t1973 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1973, gem_string("name"));
#line 934 "compiler/main.gem"
    GemVal _t1974 = (*gem_v_mangle);
    GemVal _t1975[] = {gem_v_name};
            GemVal gem_v_mname = _t1974.fn(_t1974.env, _t1975, 1);
#line 935 "compiler/main.gem"
    GemVal _t1976[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t1978;
    if (gem_truthy(gem_fn_set_contains(NULL, _t1976, 2))) {
        _t1978 = gem_fn_set_contains(NULL, _t1976, 2);
    } else {
        GemVal _t1977[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t1978 = gem_fn_set_contains(NULL, _t1977, 2);
    }
            if (gem_truthy(_t1978)) {
#line 936 "compiler/main.gem"
    GemVal _t1979[] = {gem_v_line_dir};
    GemVal _t1980 = gem_v_r;
    GemVal _t1981[] = {gem_table_get(_t1980, gem_string("setup"))};
    GemVal _t1982[] = {gem_v_p};
    GemVal _t1983[] = {gem_v_t};
    GemVal _t1984 = gem_v_r;
    GemVal _t1985[] = {gem_table_get(_t1984, gem_string("expr"))};
    GemVal _t1986[] = {gem_v_p};
    GemVal _t1987[] = {gem_v_mname};
    GemVal _t1988[] = {gem_v_t};
    GemVal _t1989[] = {gem_v_p};
    GemVal _t1990[] = {gem_v_p};
    GemVal _t1991[] = {gem_v_t};
                GemVal _t1992 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1979, 1), gem_to_string_fn(NULL, _t1981, 1)), gem_to_string_fn(NULL, _t1982, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1983, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1985, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1986, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1987, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1988, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1989, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t1990, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1991, 1)), gem_string(";"));
                gem_pop_frame();
                return _t1992;
            }
#line 938 "compiler/main.gem"
    GemVal _t1993[] = {gem_v_line_dir};
    GemVal _t1994 = gem_v_r;
    GemVal _t1995[] = {gem_table_get(_t1994, gem_string("setup"))};
    GemVal _t1996[] = {gem_v_p};
    GemVal _t1997[] = {gem_v_t};
    GemVal _t1998 = gem_v_r;
    GemVal _t1999[] = {gem_table_get(_t1998, gem_string("expr"))};
    GemVal _t2000[] = {gem_v_p};
    GemVal _t2001[] = {gem_v_mname};
    GemVal _t2002[] = {gem_v_t};
    GemVal _t2003[] = {gem_v_p};
    GemVal _t2004[] = {gem_v_p};
    GemVal _t2005[] = {gem_v_t};
            GemVal _t2006 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1993, 1), gem_to_string_fn(NULL, _t1995, 1)), gem_to_string_fn(NULL, _t1996, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1997, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1999, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2000, 1)), gem_to_string_fn(NULL, _t2001, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2002, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2003, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2004, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2005, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2006;
        } else {
#line 939 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 940 "compiler/main.gem"
    GemVal _t2007 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t2007, gem_string("value")), GEM_NIL))) {
#line 941 "compiler/main.gem"
    GemVal _t2008 = gem_v_node;
    GemVal _t2009 = (*gem_v_compile_expr);
    GemVal _t2010[] = {gem_table_get(_t2008, gem_string("value"))};
                    GemVal gem_v_r = _t2009.fn(_t2009.env, _t2010, 1);
#line 942 "compiler/main.gem"
    GemVal _t2011 = (*gem_v_tmp);
                    GemVal gem_v_t = _t2011.fn(_t2011.env, NULL, 0);
#line 943 "compiler/main.gem"
    GemVal _t2012[] = {gem_v_line_dir};
    GemVal _t2013 = gem_v_r;
    GemVal _t2014[] = {gem_table_get(_t2013, gem_string("setup"))};
    GemVal _t2015[] = {gem_v_p};
    GemVal _t2016[] = {gem_v_t};
    GemVal _t2017 = gem_v_r;
    GemVal _t2018[] = {gem_table_get(_t2017, gem_string("expr"))};
    GemVal _t2019[] = {gem_v_p};
    GemVal _t2020[] = {gem_v_p};
    GemVal _t2021[] = {gem_v_t};
                    GemVal _t2022 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2012, 1), gem_to_string_fn(NULL, _t2014, 1)), gem_to_string_fn(NULL, _t2015, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2016, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2018, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2019, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2020, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2021, 1)), gem_string(";"));
                    gem_pop_frame();
                    return _t2022;
                }
#line 945 "compiler/main.gem"
    GemVal _t2023[] = {gem_v_line_dir};
    GemVal _t2024[] = {gem_v_p};
    GemVal _t2025[] = {gem_v_p};
                GemVal _t2026 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2023, 1), gem_to_string_fn(NULL, _t2024, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2025, 1)), gem_string("return GEM_NIL;"));
                gem_pop_frame();
                return _t2026;
            } else {
#line 946 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 947 "compiler/main.gem"
    GemVal _t2027[] = {gem_v_line_dir};
    GemVal _t2028 = (*gem_v_compile_if_return);
    GemVal _t2029[] = {gem_v_node, gem_v_indent};
                    GemVal _t2030 = gem_add(gem_to_string_fn(NULL, _t2027, 1), _t2028.fn(_t2028.env, _t2029, 2));
                    gem_pop_frame();
                    return _t2030;
                } else {
#line 948 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 949 "compiler/main.gem"
    GemVal _t2031[] = {gem_v_line_dir};
    GemVal _t2032 = (*gem_v_compile_while);
    GemVal _t2033[] = {gem_v_node, gem_v_indent};
    GemVal _t2034[] = {_t2032.fn(_t2032.env, _t2033, 2)};
    GemVal _t2035[] = {gem_v_p};
    GemVal _t2036[] = {gem_v_p};
                        GemVal _t2037 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2031, 1), gem_to_string_fn(NULL, _t2034, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t2035, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2036, 1)), gem_string("return GEM_NIL;"));
                        gem_pop_frame();
                        return _t2037;
                    } else {
#line 950 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 951 "compiler/main.gem"
    GemVal _t2038[] = {gem_v_line_dir};
    GemVal _t2039 = (*gem_v_compile_match_return);
    GemVal _t2040[] = {gem_v_node, gem_v_indent};
                            GemVal _t2041 = gem_add(gem_to_string_fn(NULL, _t2038, 1), _t2039.fn(_t2039.env, _t2040, 2));
                            gem_pop_frame();
                            return _t2041;
                        } else {
#line 952 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 953 "compiler/main.gem"
    GemVal _t2042[] = {gem_v_p};
                                GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2042, 1), gem_string("{\n"));
                                {
#line 954 "compiler/main.gem"
                                    GemVal gem_v__for_i_36 = gem_int(0);
#line 954 "compiler/main.gem"
    GemVal _t2043 = gem_v_node;
    GemVal _t2044[] = {gem_table_get(_t2043, gem_string("stmts"))};
                                    GemVal gem_v__for_limit_36 = gem_sub(gem_len_fn(NULL, _t2044, 1), gem_int(1));
#line 954 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 954 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_36;
#line 954 "compiler/main.gem"
                                        gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 955 "compiler/main.gem"
    GemVal _t2045 = gem_v_node;
    GemVal _t2046 = (*gem_v_compile_stmt);
    GemVal _t2047[] = {gem_table_get(gem_table_get(_t2045, gem_string("stmts")), gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                                        gem_v_out = gem_add(gem_add(gem_v_out, _t2046.fn(_t2046.env, _t2047, 2)), gem_string("\n"));
                                    }
                                }
#line 957 "compiler/main.gem"
    GemVal _t2048 = gem_v_node;
    GemVal _t2049[] = {gem_table_get(_t2048, gem_string("stmts"))};
                                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2049, 1), gem_int(0)))) {
#line 958 "compiler/main.gem"
    GemVal _t2050 = gem_v_node;
    GemVal _t2051 = gem_v_node;
    GemVal _t2052[] = {gem_table_get(_t2051, gem_string("stmts"))};
    GemVal _t2053 = (*gem_v_compile_stmt_return);
    GemVal _t2054[] = {gem_table_get(gem_table_get(_t2050, gem_string("stmts")), gem_sub(gem_len_fn(NULL, _t2052, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t2053.fn(_t2053.env, _t2054, 2)), gem_string("\n"));
                                }
#line 960 "compiler/main.gem"
    GemVal _t2055[] = {gem_v_p};
                                gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2055, 1), gem_string("}")));
#line 961 "compiler/main.gem"
                                GemVal _t2056 = gem_v_out;
                                gem_pop_frame();
                                return _t2056;
                            } else {
#line 962 "compiler/main.gem"
    GemVal _t2057;
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
        _t2057 = gem_eq(gem_v_tag, gem_string("dot_assign"));
    } else {
        _t2057 = gem_eq(gem_v_tag, gem_string("index_assign"));
    }
    GemVal _t2058;
    if (gem_truthy(_t2057)) {
        _t2058 = _t2057;
    } else {
        _t2058 = gem_eq(gem_v_tag, gem_string("break"));
    }
    GemVal _t2059;
    if (gem_truthy(_t2058)) {
        _t2059 = _t2058;
    } else {
        _t2059 = gem_eq(gem_v_tag, gem_string("continue"));
    }
    GemVal _t2060;
    if (gem_truthy(_t2059)) {
        _t2060 = _t2059;
    } else {
        _t2060 = gem_eq(gem_v_tag, gem_string("fn_def"));
    }
                                if (gem_truthy(_t2060)) {
#line 963 "compiler/main.gem"
    GemVal _t2061 = (*gem_v_compile_stmt);
    GemVal _t2062[] = {gem_v_node, gem_v_indent};
    GemVal _t2063[] = {_t2061.fn(_t2061.env, _t2062, 2)};
    GemVal _t2064[] = {gem_v_p};
    GemVal _t2065[] = {gem_v_p};
                                    GemVal _t2066 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2063, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2064, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2065, 1)), gem_string("return GEM_NIL;"));
                                    gem_pop_frame();
                                    return _t2066;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 966 "compiler/main.gem"
    GemVal _t2067 = (*gem_v_compile_expr);
    GemVal _t2068[] = {gem_v_node};
    GemVal gem_v_r = _t2067.fn(_t2067.env, _t2068, 1);
#line 967 "compiler/main.gem"
    GemVal _t2069 = (*gem_v_tmp);
    GemVal gem_v_t = _t2069.fn(_t2069.env, NULL, 0);
    GemVal _t2070[] = {gem_v_line_dir};
    GemVal _t2071 = gem_v_r;
    GemVal _t2072[] = {gem_table_get(_t2071, gem_string("setup"))};
    GemVal _t2073[] = {gem_v_p};
    GemVal _t2074[] = {gem_v_t};
    GemVal _t2075 = gem_v_r;
    GemVal _t2076[] = {gem_table_get(_t2075, gem_string("expr"))};
    GemVal _t2077[] = {gem_v_p};
    GemVal _t2078[] = {gem_v_p};
    GemVal _t2079[] = {gem_v_t};
    GemVal _t2080 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2070, 1), gem_to_string_fn(NULL, _t2072, 1)), gem_to_string_fn(NULL, _t2073, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2074, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2076, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2077, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2078, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2079, 1)), gem_string(";"));
    gem_pop_frame();
    return _t2080;
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
#line 974 "compiler/main.gem"
    GemVal _t2082[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2082, 1);
#line 975 "compiler/main.gem"
    GemVal _t2083 = gem_v_node;
    GemVal _t2084 = (*gem_v_compile_expr);
    GemVal _t2085[] = {gem_table_get(_t2083, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2084.fn(_t2084.env, _t2085, 1);
#line 976 "compiler/main.gem"
    GemVal _t2086 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2086, gem_string("setup"));
#line 977 "compiler/main.gem"
    GemVal _t2087[] = {gem_v_p};
    GemVal _t2088 = gem_v_cond_r;
    GemVal _t2089[] = {gem_table_get(_t2088, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2087, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2089, 1)), gem_string(")) {\n")));
#line 978 "compiler/main.gem"
    GemVal _t2090 = gem_v_node;
    GemVal _t2091 = (*gem_v_compile_stmts);
    GemVal _t2092[] = {gem_table_get(_t2090, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t2091.fn(_t2091.env, _t2092, 2));
#line 979 "compiler/main.gem"
    GemVal _t2093 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2093, gem_string("else")), GEM_NIL))) {
#line 980 "compiler/main.gem"
    GemVal _t2094[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2094, 1), gem_string("} else {\n")));
#line 981 "compiler/main.gem"
    GemVal _t2095 = gem_v_node;
    GemVal _t2096 = (*gem_v_compile_stmts);
    GemVal _t2097[] = {gem_table_get(_t2095, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t2096.fn(_t2096.env, _t2097, 2));
    }
#line 983 "compiler/main.gem"
    GemVal _t2098[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2098, 1), gem_string("}")));
    GemVal _t2099 = gem_v_out;
    gem_pop_frame();
    return _t2099;
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
#line 990 "compiler/main.gem"
    GemVal _t2101[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2101, 1);
#line 991 "compiler/main.gem"
    GemVal _t2102 = gem_v_node;
    GemVal _t2103 = (*gem_v_compile_expr);
    GemVal _t2104[] = {gem_table_get(_t2102, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2103.fn(_t2103.env, _t2104, 1);
#line 992 "compiler/main.gem"
    GemVal _t2105 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2105, gem_string("setup"));
#line 993 "compiler/main.gem"
    GemVal _t2106[] = {gem_v_p};
    GemVal _t2107 = gem_v_cond_r;
    GemVal _t2108[] = {gem_table_get(_t2107, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2106, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2108, 1)), gem_string(")) {\n")));
#line 994 "compiler/main.gem"
    GemVal _t2109 = gem_v_node;
    GemVal gem_v_then_body = gem_table_get(_t2109, gem_string("then"));
#line 995 "compiler/main.gem"
    GemVal _t2110[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2110, 1), gem_int(0)))) {
        {
#line 996 "compiler/main.gem"
            GemVal gem_v__for_i_37 = gem_int(0);
#line 996 "compiler/main.gem"
    GemVal _t2111[] = {gem_v_then_body};
            GemVal gem_v__for_limit_37 = gem_sub(gem_len_fn(NULL, _t2111, 1), gem_int(1));
#line 996 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 996 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_37;
#line 996 "compiler/main.gem"
                gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 997 "compiler/main.gem"
    GemVal _t2112 = (*gem_v_compile_stmt);
    GemVal _t2113[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t2112.fn(_t2112.env, _t2113, 2)), gem_string("\n"));
            }
        }
#line 999 "compiler/main.gem"
    GemVal _t2114[] = {gem_v_then_body};
    GemVal _t2115 = (*gem_v_compile_stmt_return);
    GemVal _t2116[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t2114, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_add(gem_v_out, _t2115.fn(_t2115.env, _t2116, 2)), gem_string("\n"));
    } else {
#line 1001 "compiler/main.gem"
    GemVal _t2117[] = {gem_v_p};
    GemVal _t2118[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2117, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2118, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1003 "compiler/main.gem"
    GemVal _t2119 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2119, gem_string("else")), GEM_NIL))) {
#line 1004 "compiler/main.gem"
    GemVal _t2120[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2120, 1), gem_string("} else {\n")));
#line 1005 "compiler/main.gem"
    GemVal _t2121 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t2121, gem_string("else"));
#line 1006 "compiler/main.gem"
    GemVal _t2122[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2122, 1), gem_int(0)))) {
            {
#line 1007 "compiler/main.gem"
                GemVal gem_v__for_i_38 = gem_int(0);
#line 1007 "compiler/main.gem"
    GemVal _t2123[] = {gem_v_else_body};
                GemVal gem_v__for_limit_38 = gem_sub(gem_len_fn(NULL, _t2123, 1), gem_int(1));
#line 1007 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 1007 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_38;
#line 1007 "compiler/main.gem"
                    gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 1008 "compiler/main.gem"
    GemVal _t2124 = (*gem_v_compile_stmt);
    GemVal _t2125[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t2124.fn(_t2124.env, _t2125, 2)), gem_string("\n"));
                }
            }
#line 1010 "compiler/main.gem"
    GemVal _t2126[] = {gem_v_else_body};
    GemVal _t2127 = (*gem_v_compile_stmt_return);
    GemVal _t2128[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2126, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t2127.fn(_t2127.env, _t2128, 2)), gem_string("\n"));
        } else {
#line 1012 "compiler/main.gem"
    GemVal _t2129[] = {gem_v_p};
    GemVal _t2130[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2129, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2130, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1015 "compiler/main.gem"
    GemVal _t2131[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2131, 1), gem_string("} else {\n")));
#line 1016 "compiler/main.gem"
    GemVal _t2132[] = {gem_v_p};
    GemVal _t2133[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2132, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2133, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1018 "compiler/main.gem"
    GemVal _t2134[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2134, 1), gem_string("}")));
    GemVal _t2135 = gem_v_out;
    gem_pop_frame();
    return _t2135;
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
#line 1025 "compiler/main.gem"
    GemVal _t2137[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2137, 1);
#line 1026 "compiler/main.gem"
    GemVal _t2138 = gem_v_node;
    GemVal _t2139 = (*gem_v_compile_expr);
    GemVal _t2140[] = {gem_table_get(_t2138, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2139.fn(_t2139.env, _t2140, 1);
#line 1027 "compiler/main.gem"
    GemVal _t2141[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2141, 1), gem_string("while (1) {\n"));
#line 1028 "compiler/main.gem"
    GemVal _t2142 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t2142, gem_string("setup")), gem_string("")))) {
#line 1029 "compiler/main.gem"
    GemVal _t2143 = gem_v_cond_r;
    GemVal _t2144[] = {gem_v_p};
    GemVal _t2145[] = {gem_table_get(_t2143, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t2144, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_str_replace_fn(NULL, _t2145, 3));
    }
#line 1031 "compiler/main.gem"
    GemVal _t2146[] = {gem_v_p};
    GemVal _t2147 = gem_v_cond_r;
    GemVal _t2148[] = {gem_table_get(_t2147, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2146, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t2148, 1)), gem_string(")) break;\n")));
#line 1032 "compiler/main.gem"
    GemVal _t2149 = gem_v_node;
    GemVal _t2150 = (*gem_v_compile_stmts);
    GemVal _t2151[] = {gem_table_get(_t2149, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t2150.fn(_t2150.env, _t2151, 2));
#line 1033 "compiler/main.gem"
    GemVal _t2152[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2152, 1), gem_string("}")));
    GemVal _t2153 = gem_v_out;
    gem_pop_frame();
    return _t2153;
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
#line 1040 "compiler/main.gem"
    GemVal _t2155[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2155, 1);
#line 1041 "compiler/main.gem"
    GemVal _t2156 = gem_v_node;
    GemVal _t2157 = (*gem_v_compile_expr);
    GemVal _t2158[] = {gem_table_get(_t2156, gem_string("target"))};
    GemVal gem_v_target_r = _t2157.fn(_t2157.env, _t2158, 1);
#line 1042 "compiler/main.gem"
    GemVal _t2159 = (*gem_v_tmp);
    GemVal gem_v_t = _t2159.fn(_t2159.env, NULL, 0);
#line 1043 "compiler/main.gem"
    GemVal _t2160 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2160, gem_string("setup"));
#line 1044 "compiler/main.gem"
    GemVal _t2161[] = {gem_v_p};
    GemVal _t2162[] = {gem_v_t};
    GemVal _t2163 = gem_v_target_r;
    GemVal _t2164[] = {gem_table_get(_t2163, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2161, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2162, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2164, 1)), gem_string(";\n")));
    {
#line 1045 "compiler/main.gem"
        GemVal gem_v__for_i_39 = gem_int(0);
#line 1045 "compiler/main.gem"
    GemVal _t2165 = gem_v_node;
    GemVal _t2166[] = {gem_table_get(_t2165, gem_string("whens"))};
        GemVal gem_v__for_limit_39 = gem_len_fn(NULL, _t2166, 1);
#line 1045 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
#line 1045 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_39;
#line 1045 "compiler/main.gem"
            gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 1046 "compiler/main.gem"
    GemVal _t2167 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t2167, gem_string("whens")), gem_v_i);
#line 1047 "compiler/main.gem"
    GemVal _t2168 = gem_v_w;
    GemVal _t2169 = (*gem_v_compile_expr);
    GemVal _t2170[] = {gem_table_get(_t2168, gem_string("value"))};
            GemVal gem_v_val_r = _t2169.fn(_t2169.env, _t2170, 1);
#line 1048 "compiler/main.gem"
            GemVal gem_v_keyword = gem_string("if");
#line 1049 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1050 "compiler/main.gem"
                gem_v_keyword = gem_string("} else if");
            }
#line 1052 "compiler/main.gem"
    GemVal _t2171 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t2171, gem_string("setup")));
#line 1053 "compiler/main.gem"
    GemVal _t2172[] = {gem_v_p};
    GemVal _t2173[] = {gem_v_keyword};
    GemVal _t2174[] = {gem_v_t};
    GemVal _t2175 = gem_v_val_r;
    GemVal _t2176[] = {gem_table_get(_t2175, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2172, 1), gem_to_string_fn(NULL, _t2173, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2174, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2176, 1)), gem_string("))) {\n")));
#line 1054 "compiler/main.gem"
    GemVal _t2177 = gem_v_w;
    GemVal _t2178 = (*gem_v_compile_stmts);
    GemVal _t2179[] = {gem_table_get(_t2177, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_v_out, _t2178.fn(_t2178.env, _t2179, 2));
        }
    }
#line 1056 "compiler/main.gem"
    GemVal _t2180 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2180, gem_string("else")), GEM_NIL))) {
#line 1057 "compiler/main.gem"
    GemVal _t2181[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2181, 1), gem_string("} else {\n")));
#line 1058 "compiler/main.gem"
    GemVal _t2182 = gem_v_node;
    GemVal _t2183 = (*gem_v_compile_stmts);
    GemVal _t2184[] = {gem_table_get(_t2182, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t2183.fn(_t2183.env, _t2184, 2));
    }
#line 1060 "compiler/main.gem"
    GemVal _t2185[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2185, 1), gem_string("}")));
    GemVal _t2186 = gem_v_out;
    gem_pop_frame();
    return _t2186;
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
#line 1067 "compiler/main.gem"
    GemVal _t2188[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2188, 1);
#line 1068 "compiler/main.gem"
    GemVal _t2189 = gem_v_node;
    GemVal _t2190 = (*gem_v_compile_expr);
    GemVal _t2191[] = {gem_table_get(_t2189, gem_string("target"))};
    GemVal gem_v_target_r = _t2190.fn(_t2190.env, _t2191, 1);
#line 1069 "compiler/main.gem"
    GemVal _t2192 = (*gem_v_tmp);
    GemVal gem_v_t = _t2192.fn(_t2192.env, NULL, 0);
#line 1070 "compiler/main.gem"
    GemVal _t2193 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2193, gem_string("setup"));
#line 1071 "compiler/main.gem"
    GemVal _t2194[] = {gem_v_p};
    GemVal _t2195[] = {gem_v_t};
    GemVal _t2196 = gem_v_target_r;
    GemVal _t2197[] = {gem_table_get(_t2196, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2194, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2195, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2197, 1)), gem_string(";\n")));
    {
#line 1072 "compiler/main.gem"
        GemVal gem_v__for_i_41 = gem_int(0);
#line 1072 "compiler/main.gem"
    GemVal _t2198 = gem_v_node;
    GemVal _t2199[] = {gem_table_get(_t2198, gem_string("whens"))};
        GemVal gem_v__for_limit_41 = gem_len_fn(NULL, _t2199, 1);
#line 1072 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_limit_41))) break;
#line 1072 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_41;
#line 1072 "compiler/main.gem"
            gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
#line 1073 "compiler/main.gem"
    GemVal _t2200 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t2200, gem_string("whens")), gem_v_i);
#line 1074 "compiler/main.gem"
    GemVal _t2201 = gem_v_w;
    GemVal _t2202 = (*gem_v_compile_expr);
    GemVal _t2203[] = {gem_table_get(_t2201, gem_string("value"))};
            GemVal gem_v_val_r = _t2202.fn(_t2202.env, _t2203, 1);
#line 1075 "compiler/main.gem"
            GemVal gem_v_keyword = gem_string("if");
#line 1076 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1077 "compiler/main.gem"
                gem_v_keyword = gem_string("} else if");
            }
#line 1079 "compiler/main.gem"
    GemVal _t2204 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t2204, gem_string("setup")));
#line 1080 "compiler/main.gem"
    GemVal _t2205[] = {gem_v_p};
    GemVal _t2206[] = {gem_v_keyword};
    GemVal _t2207[] = {gem_v_t};
    GemVal _t2208 = gem_v_val_r;
    GemVal _t2209[] = {gem_table_get(_t2208, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2205, 1), gem_to_string_fn(NULL, _t2206, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2207, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2209, 1)), gem_string("))) {\n")));
#line 1081 "compiler/main.gem"
    GemVal _t2210 = gem_v_w;
            GemVal gem_v_body = gem_table_get(_t2210, gem_string("body"));
#line 1082 "compiler/main.gem"
    GemVal _t2211[] = {gem_v_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2211, 1), gem_int(0)))) {
                {
#line 1083 "compiler/main.gem"
                    GemVal gem_v__for_i_40 = gem_int(0);
#line 1083 "compiler/main.gem"
    GemVal _t2212[] = {gem_v_body};
                    GemVal gem_v__for_limit_40 = gem_sub(gem_len_fn(NULL, _t2212, 1), gem_int(1));
#line 1083 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 1083 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_40;
#line 1083 "compiler/main.gem"
                        gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 1084 "compiler/main.gem"
    GemVal _t2213 = (*gem_v_compile_stmt);
    GemVal _t2214[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
                        gem_v_out = gem_add(gem_add(gem_v_out, _t2213.fn(_t2213.env, _t2214, 2)), gem_string("\n"));
                    }
                }
#line 1086 "compiler/main.gem"
    GemVal _t2215[] = {gem_v_body};
    GemVal _t2216 = (*gem_v_compile_stmt_return);
    GemVal _t2217[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2215, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t2216.fn(_t2216.env, _t2217, 2)), gem_string("\n"));
            } else {
#line 1088 "compiler/main.gem"
    GemVal _t2218[] = {gem_v_p};
    GemVal _t2219[] = {gem_v_p};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2218, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2219, 1)), gem_string("    return GEM_NIL;\n")));
            }
        }
    }
#line 1091 "compiler/main.gem"
    GemVal _t2220 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2220, gem_string("else")), GEM_NIL))) {
#line 1092 "compiler/main.gem"
    GemVal _t2221[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2221, 1), gem_string("} else {\n")));
#line 1093 "compiler/main.gem"
    GemVal _t2222 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t2222, gem_string("else"));
#line 1094 "compiler/main.gem"
    GemVal _t2223[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2223, 1), gem_int(0)))) {
            {
#line 1095 "compiler/main.gem"
                GemVal gem_v__for_i_42 = gem_int(0);
#line 1095 "compiler/main.gem"
    GemVal _t2224[] = {gem_v_else_body};
                GemVal gem_v__for_limit_42 = gem_sub(gem_len_fn(NULL, _t2224, 1), gem_int(1));
#line 1095 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 1095 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_42;
#line 1095 "compiler/main.gem"
                    gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 1096 "compiler/main.gem"
    GemVal _t2225 = (*gem_v_compile_stmt);
    GemVal _t2226[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t2225.fn(_t2225.env, _t2226, 2)), gem_string("\n"));
                }
            }
#line 1098 "compiler/main.gem"
    GemVal _t2227[] = {gem_v_else_body};
    GemVal _t2228 = (*gem_v_compile_stmt_return);
    GemVal _t2229[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2227, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t2228.fn(_t2228.env, _t2229, 2)), gem_string("\n"));
        } else {
#line 1100 "compiler/main.gem"
    GemVal _t2230[] = {gem_v_p};
    GemVal _t2231[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2230, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2231, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1103 "compiler/main.gem"
    GemVal _t2232[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2232, 1), gem_string("} else {\n")));
#line 1104 "compiler/main.gem"
    GemVal _t2233[] = {gem_v_p};
    GemVal _t2234[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2233, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2234, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1106 "compiler/main.gem"
    GemVal _t2235[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2235, 1), gem_string("}")));
    GemVal _t2236 = gem_v_out;
    gem_pop_frame();
    return _t2236;
}

struct _closure__anon_44 {
    GemVal *gem_v_extern_c_decls;
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_extern_c_decls = _cls->gem_v_extern_c_decls;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_44", "compiler/main.gem", 0);
#line 1113 "compiler/main.gem"
    GemVal _t2238 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t2238, gem_string("name"));
#line 1114 "compiler/main.gem"
    GemVal _t2239 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t2239, gem_string("params"));
#line 1115 "compiler/main.gem"
    GemVal _t2240 = gem_v_node;
    GemVal gem_v_ret_type = gem_table_get(_t2240, gem_string("ret_type"));
#line 1117 "compiler/main.gem"
    GemVal _t2241[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2241, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1119 "compiler/main.gem"
    GemVal _t2242 = gem_table_new();
    GemVal gem_v_c_args = _t2242;
    {
#line 1120 "compiler/main.gem"
        GemVal gem_v__for_i_43 = gem_int(0);
#line 1120 "compiler/main.gem"
    GemVal _t2243[] = {gem_v_params};
        GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t2243, 1);
#line 1120 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 1120 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_43;
#line 1120 "compiler/main.gem"
            gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 1121 "compiler/main.gem"
            GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1122 "compiler/main.gem"
            GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1123 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1124 "compiler/main.gem"
    GemVal _t2244[] = {gem_v_i};
    GemVal _t2245[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t2244, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2245, 1)), gem_string("].ival;\n")));
#line 1125 "compiler/main.gem"
    GemVal _t2246[] = {gem_v_i};
    GemVal _t2247[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2246, 1))};
                (void)(gem_push_fn(NULL, _t2247, 2));
            } else {
#line 1126 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1127 "compiler/main.gem"
    GemVal _t2248[] = {gem_v_i};
    GemVal _t2249[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t2248, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2249, 1)), gem_string("].fval;\n")));
#line 1128 "compiler/main.gem"
    GemVal _t2250[] = {gem_v_i};
    GemVal _t2251[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2250, 1))};
                    (void)(gem_push_fn(NULL, _t2251, 2));
                } else {
#line 1129 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1130 "compiler/main.gem"
    GemVal _t2252[] = {gem_v_i};
    GemVal _t2253[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t2252, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2253, 1)), gem_string("].sval;\n")));
#line 1131 "compiler/main.gem"
    GemVal _t2254[] = {gem_v_i};
    GemVal _t2255[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2254, 1))};
                        (void)(gem_push_fn(NULL, _t2255, 2));
                    } else {
#line 1132 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1133 "compiler/main.gem"
    GemVal _t2256[] = {gem_v_i};
    GemVal _t2257[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t2256, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2257, 1)), gem_string("].bval;\n")));
#line 1134 "compiler/main.gem"
    GemVal _t2258[] = {gem_v_i};
    GemVal _t2259[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2258, 1))};
                            (void)(gem_push_fn(NULL, _t2259, 2));
                        } else {
#line 1135 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1136 "compiler/main.gem"
    GemVal _t2260[] = {gem_v_i};
    GemVal _t2261[] = {gem_v_i};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t2260, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t2261, 1)), gem_string("].ival;\n")));
#line 1137 "compiler/main.gem"
    GemVal _t2262[] = {gem_v_i};
    GemVal _t2263[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2262, 1))};
                                (void)(gem_push_fn(NULL, _t2263, 2));
                            } else {
#line 1138 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1139 "compiler/main.gem"
    GemVal _t2264[] = {gem_v_i};
    GemVal _t2265[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2264, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t2265, 2));
                                } else {
#line 1141 "compiler/main.gem"
    GemVal _t2266[] = {gem_v_i};
    GemVal _t2267[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2266, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t2267, 2));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 1146 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
    {
#line 1147 "compiler/main.gem"
        GemVal gem_v__for_i_44 = gem_int(0);
#line 1147 "compiler/main.gem"
    GemVal _t2268[] = {gem_v_c_args};
        GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t2268, 1);
#line 1147 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 1147 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_44;
#line 1147 "compiler/main.gem"
            gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 1148 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1149 "compiler/main.gem"
                gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
            }
#line 1151 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
        }
    }
#line 1154 "compiler/main.gem"
    GemVal _t2269[] = {gem_v_name};
    GemVal _t2270[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2269, 1), gem_string("(")), gem_to_string_fn(NULL, _t2270, 1)), gem_string(")"));
#line 1156 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1157 "compiler/main.gem"
    GemVal _t2271[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2271, 1)), gem_string(";\n")));
#line 1158 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
#line 1159 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1160 "compiler/main.gem"
    GemVal _t2272[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t2272, 1)), gem_string(";\n")));
        } else {
#line 1161 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1162 "compiler/main.gem"
    GemVal _t2273[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t2273, 1)), gem_string(";\n")));
#line 1163 "compiler/main.gem"
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
#line 1164 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1165 "compiler/main.gem"
    GemVal _t2274[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t2274, 1)), gem_string(";\n")));
#line 1166 "compiler/main.gem"
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
#line 1167 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1168 "compiler/main.gem"
    GemVal _t2275[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t2275, 1)), gem_string(";\n")));
#line 1169 "compiler/main.gem"
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
#line 1170 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1171 "compiler/main.gem"
    GemVal _t2276[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t2276, 1)), gem_string(";\n")));
#line 1172 "compiler/main.gem"
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
#line 1173 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1174 "compiler/main.gem"
    GemVal _t2277[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t2277, 1)), gem_string(";\n")));
#line 1175 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
#line 1177 "compiler/main.gem"
    GemVal _t2278[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2278, 1)), gem_string(";\n")));
#line 1178 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1181 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1184 "compiler/main.gem"
    GemVal _t2279[] = {gem_v_name};
    GemVal _t2280[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2279, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t2280, 2));
#line 1187 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1188 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1189 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1190 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1191 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1192 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1193 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1194 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1195 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1196 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1197 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1198 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1199 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1202 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1203 "compiler/main.gem"
    GemVal _t2281[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2281, 1), gem_int(0)))) {
#line 1204 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
        {
#line 1206 "compiler/main.gem"
            GemVal gem_v__for_i_45 = gem_int(0);
#line 1206 "compiler/main.gem"
    GemVal _t2282[] = {gem_v_params};
            GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2282, 1);
#line 1206 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 1206 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_45;
#line 1206 "compiler/main.gem"
                gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 1207 "compiler/main.gem"
                if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1208 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
                }
#line 1210 "compiler/main.gem"
                GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1211 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1212 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
                } else {
#line 1213 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1214 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                    } else {
#line 1215 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1216 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                        } else {
#line 1217 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1218 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                            } else {
#line 1219 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1220 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                                } else {
#line 1221 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1222 "compiler/main.gem"
                                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                    } else {
#line 1224 "compiler/main.gem"
                                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 1229 "compiler/main.gem"
    GemVal _t2283[] = {gem_v_c_ret};
    GemVal _t2284[] = {gem_v_name};
    GemVal _t2285[] = {gem_v_c_params};
    GemVal _t2286[] = {(*gem_v_extern_c_decls), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2283, 1), gem_string(" ")), gem_to_string_fn(NULL, _t2284, 1)), gem_string("(")), gem_to_string_fn(NULL, _t2285, 1)), gem_string(");"))};
    (void)(gem_push_fn(NULL, _t2286, 2));
    GemVal _t2287 = gem_v_lines;
    gem_pop_frame();
    return _t2287;
}

struct _closure__anon_45 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_mangle;
    GemVal *gem_v_source_name;
};
static GemVal _anon_45(void *_env, GemVal *args, int argc) {
    struct _closure__anon_45 *_cls = (struct _closure__anon_45 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_45", "compiler/main.gem", 0);
#line 1237 "compiler/main.gem"
    GemVal _t2289 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t2289, gem_string("params"));
#line 1238 "compiler/main.gem"
    GemVal _t2290 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t2290, gem_string("body"));
#line 1239 "compiler/main.gem"
    GemVal _t2291 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t2291, gem_string("name"));
#line 1242 "compiler/main.gem"
    GemVal _t2292[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t2292, 1);
    {
#line 1243 "compiler/main.gem"
        GemVal gem_v__for_i_47 = gem_int(0);
#line 1243 "compiler/main.gem"
    GemVal _t2293[] = {gem_v_body};
        GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2293, 1);
#line 1243 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 1243 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_47;
#line 1243 "compiler/main.gem"
            gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 1244 "compiler/main.gem"
    GemVal _t2294[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2296;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2294, 1), gem_string("table")))) {
        _t2296 = gem_eq(gem_type_fn(NULL, _t2294, 1), gem_string("table"));
    } else {
        GemVal _t2295 = gem_table_get(gem_v_body, gem_v_i);
        _t2296 = gem_eq(gem_table_get(_t2295, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t2296)) {
#line 1245 "compiler/main.gem"
    GemVal _t2297 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2298[] = {gem_v_scope_vars, gem_table_get(_t2297, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t2298, 2));
            }
#line 1247 "compiler/main.gem"
    GemVal _t2299[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2301;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2299, 1), gem_string("table")))) {
        _t2301 = gem_eq(gem_type_fn(NULL, _t2299, 1), gem_string("table"));
    } else {
        GemVal _t2300 = gem_table_get(gem_v_body, gem_v_i);
        _t2301 = gem_eq(gem_table_get(_t2300, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t2301)) {
                {
#line 1248 "compiler/main.gem"
                    GemVal gem_v__for_i_46 = gem_int(0);
#line 1248 "compiler/main.gem"
    GemVal _t2302 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2303[] = {gem_table_get(_t2302, gem_string("stmts"))};
                    GemVal gem_v__for_limit_46 = gem_len_fn(NULL, _t2303, 1);
#line 1248 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
#line 1248 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_46;
#line 1248 "compiler/main.gem"
                        gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 1249 "compiler/main.gem"
    GemVal _t2304 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2305[] = {gem_table_get(gem_table_get(_t2304, gem_string("stmts")), gem_v_j)};
    GemVal _t2308;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2305, 1), gem_string("table")))) {
        _t2308 = gem_eq(gem_type_fn(NULL, _t2305, 1), gem_string("table"));
    } else {
        GemVal _t2306 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t2307 = gem_table_get(gem_table_get(_t2306, gem_string("stmts")), gem_v_j);
        _t2308 = gem_eq(gem_table_get(_t2307, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t2308)) {
#line 1250 "compiler/main.gem"
    GemVal _t2309 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2310 = gem_table_get(gem_table_get(_t2309, gem_string("stmts")), gem_v_j);
    GemVal _t2311[] = {gem_v_scope_vars, gem_table_get(_t2310, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t2311, 2));
                        }
                    }
                }
            }
        }
    }
#line 1255 "compiler/main.gem"
    GemVal _t2312 = (*gem_v_find_captured_in_scope);
    GemVal _t2313[] = {gem_v_body, gem_v_scope_vars};
    GemVal gem_v_captured = _t2312.fn(_t2312.env, _t2313, 2);
#line 1257 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1258 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1259 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1260 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1262 "compiler/main.gem"
    GemVal _t2314[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2314, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1265 "compiler/main.gem"
    GemVal _t2315 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2315, gem_string("line")), GEM_NIL))) {
#line 1266 "compiler/main.gem"
    GemVal _t2316 = gem_v_node;
    GemVal _t2317[] = {gem_table_get(_t2316, gem_string("line"))};
    GemVal _t2318[] = {(*gem_v_source_name)};
    GemVal _t2319[] = {gem_fn_escape_c_string(NULL, _t2318, 1)};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2317, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2319, 1)), gem_string("\"\n")));
    }
    {
#line 1270 "compiler/main.gem"
        GemVal gem_v__for_i_48 = gem_int(0);
#line 1270 "compiler/main.gem"
    GemVal _t2320[] = {gem_v_params};
        GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t2320, 1);
#line 1270 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 1270 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_48;
#line 1270 "compiler/main.gem"
            gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 1271 "compiler/main.gem"
    GemVal _t2321 = (*gem_v_mangle);
    GemVal _t2322[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t2321.fn(_t2321.env, _t2322, 1);
#line 1272 "compiler/main.gem"
    GemVal _t2323[] = {gem_v_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2323, 2))) {
#line 1273 "compiler/main.gem"
    GemVal _t2324[] = {gem_v_mp};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2324, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1274 "compiler/main.gem"
    GemVal _t2325[] = {gem_v_mp};
    GemVal _t2326[] = {gem_v_i};
    GemVal _t2327[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2325, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2326, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2327, 1)), gem_string("] : GEM_NIL;\n")));
            } else {
#line 1276 "compiler/main.gem"
    GemVal _t2328[] = {gem_v_mp};
    GemVal _t2329[] = {gem_v_i};
    GemVal _t2330[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2328, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2329, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2330, 1)), gem_string("] : GEM_NIL;\n")));
            }
        }
    }
#line 1280 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1281 "compiler/main.gem"
    GemVal _t2331 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2331, gem_string("line")), GEM_NIL))) {
#line 1282 "compiler/main.gem"
    GemVal _t2332 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t2332, gem_string("line"));
    }
#line 1284 "compiler/main.gem"
    GemVal _t2333[] = {gem_v_name};
    GemVal _t2334[] = {gem_fn_escape_c_string(NULL, _t2333, 1)};
    GemVal _t2335[] = {(*gem_v_source_name)};
    GemVal _t2336[] = {gem_fn_escape_c_string(NULL, _t2335, 1)};
    GemVal _t2337[] = {gem_v_fn_line};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2334, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2336, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2337, 1)), gem_string(");\n")));
#line 1287 "compiler/main.gem"
    GemVal _t2338[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2338, 1), gem_int(0)))) {
        {
#line 1288 "compiler/main.gem"
            GemVal gem_v__for_i_49 = gem_int(0);
#line 1288 "compiler/main.gem"
    GemVal _t2339[] = {gem_v_body};
            GemVal gem_v__for_limit_49 = gem_sub(gem_len_fn(NULL, _t2339, 1), gem_int(1));
#line 1288 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_v__for_limit_49))) break;
#line 1288 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_49;
#line 1288 "compiler/main.gem"
                gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 1289 "compiler/main.gem"
    GemVal _t2340 = (*gem_v_compile_stmt);
    GemVal _t2341[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t2340.fn(_t2340.env, _t2341, 2)), gem_string("\n"));
            }
        }
#line 1291 "compiler/main.gem"
    GemVal _t2342[] = {gem_v_body};
    GemVal _t2343 = (*gem_v_compile_stmt_return);
    GemVal _t2344[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2342, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t2343.fn(_t2343.env, _t2344, 2)), gem_string("\n"));
    } else {
#line 1293 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
#line 1296 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1297 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1298 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
    GemVal _t2345 = gem_v_lines;
    gem_pop_frame();
    return _t2345;
}

struct _closure__anon_46 {
    GemVal *gem_v_anon_name;
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_functions;
    GemVal *gem_v_in_top_level;
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
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_captures = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_46", "compiler/main.gem", 0);
#line 1305 "compiler/main.gem"
    GemVal _t2347 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t2347, gem_string("params"));
#line 1306 "compiler/main.gem"
    GemVal _t2348 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t2348, gem_string("body"));
#line 1307 "compiler/main.gem"
    GemVal _t2349 = (*gem_v_anon_name);
    GemVal gem_v_name = _t2349.fn(_t2349.env, NULL, 0);
#line 1310 "compiler/main.gem"
    GemVal _t2350[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t2350, 1);
    {
#line 1311 "compiler/main.gem"
        GemVal gem_v__for_i_51 = gem_int(0);
#line 1311 "compiler/main.gem"
    GemVal _t2351[] = {gem_v_body};
        GemVal gem_v__for_limit_51 = gem_len_fn(NULL, _t2351, 1);
#line 1311 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_v__for_limit_51))) break;
#line 1311 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_51;
#line 1311 "compiler/main.gem"
            gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 1312 "compiler/main.gem"
    GemVal _t2352[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2354;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2352, 1), gem_string("table")))) {
        _t2354 = gem_eq(gem_type_fn(NULL, _t2352, 1), gem_string("table"));
    } else {
        GemVal _t2353 = gem_table_get(gem_v_body, gem_v_i);
        _t2354 = gem_eq(gem_table_get(_t2353, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t2354)) {
#line 1313 "compiler/main.gem"
    GemVal _t2355 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2356[] = {gem_v_inner_scope, gem_table_get(_t2355, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t2356, 2));
            }
#line 1315 "compiler/main.gem"
    GemVal _t2357[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2359;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2357, 1), gem_string("table")))) {
        _t2359 = gem_eq(gem_type_fn(NULL, _t2357, 1), gem_string("table"));
    } else {
        GemVal _t2358 = gem_table_get(gem_v_body, gem_v_i);
        _t2359 = gem_eq(gem_table_get(_t2358, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t2359)) {
                {
#line 1316 "compiler/main.gem"
                    GemVal gem_v__for_i_50 = gem_int(0);
#line 1316 "compiler/main.gem"
    GemVal _t2360 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2361[] = {gem_table_get(_t2360, gem_string("stmts"))};
                    GemVal gem_v__for_limit_50 = gem_len_fn(NULL, _t2361, 1);
#line 1316 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
#line 1316 "compiler/main.gem"
                        GemVal gem_v_j = gem_v__for_i_50;
#line 1316 "compiler/main.gem"
                        gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 1317 "compiler/main.gem"
    GemVal _t2362 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2363[] = {gem_table_get(gem_table_get(_t2362, gem_string("stmts")), gem_v_j)};
    GemVal _t2366;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2363, 1), gem_string("table")))) {
        _t2366 = gem_eq(gem_type_fn(NULL, _t2363, 1), gem_string("table"));
    } else {
        GemVal _t2364 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t2365 = gem_table_get(gem_table_get(_t2364, gem_string("stmts")), gem_v_j);
        _t2366 = gem_eq(gem_table_get(_t2365, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t2366)) {
#line 1318 "compiler/main.gem"
    GemVal _t2367 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2368 = gem_table_get(gem_table_get(_t2367, gem_string("stmts")), gem_v_j);
    GemVal _t2369[] = {gem_v_inner_scope, gem_table_get(_t2368, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t2369, 2));
                        }
                    }
                }
            }
        }
    }
#line 1323 "compiler/main.gem"
    GemVal _t2370 = (*gem_v_find_captured_in_scope);
    GemVal _t2371[] = {gem_v_body, gem_v_inner_scope};
    GemVal gem_v_inner_captured = _t2370.fn(_t2370.env, _t2371, 2);
#line 1325 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1326 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1327 "compiler/main.gem"
    GemVal _t2372[] = {gem_v_captures};
    GemVal _t2373[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t2372, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t2373, 2);
#line 1328 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1331 "compiler/main.gem"
    GemVal _t2374[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t2374, 1));
#line 1332 "compiler/main.gem"
    GemVal _t2375[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t2375, 1)), gem_string(" {\n"));
    {
#line 1333 "compiler/main.gem"
        GemVal gem_v__for_items_52 = gem_v_captures;
#line 1333 "compiler/main.gem"
        GemVal gem_v__for_i_52 = gem_int(0);
#line 1333 "compiler/main.gem"
        while (1) {
            GemVal _t2376[] = {gem_v__for_items_52};
            if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_len_fn(NULL, _t2376, 1)))) break;
#line 1333 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_52, gem_v__for_i_52);
#line 1333 "compiler/main.gem"
            gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 1334 "compiler/main.gem"
    GemVal _t2377 = (*gem_v_mangle);
    GemVal _t2378[] = {gem_v_cap};
    GemVal _t2379[] = {_t2377.fn(_t2377.env, _t2378, 1)};
            gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2379, 1)), gem_string(";\n")));
        }
    }
#line 1336 "compiler/main.gem"
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
#line 1339 "compiler/main.gem"
    GemVal _t2380[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t2380, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1340 "compiler/main.gem"
    GemVal _t2381[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2381, 1), gem_int(0)))) {
#line 1341 "compiler/main.gem"
    GemVal _t2382[] = {gem_v_struct_name};
    GemVal _t2383[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2382, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t2383, 1)), gem_string(" *)_env;\n")));
        {
#line 1342 "compiler/main.gem"
            GemVal gem_v__for_items_53 = gem_v_captures;
#line 1342 "compiler/main.gem"
            GemVal gem_v__for_i_53 = gem_int(0);
#line 1342 "compiler/main.gem"
            while (1) {
                GemVal _t2384[] = {gem_v__for_items_53};
                if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_len_fn(NULL, _t2384, 1)))) break;
#line 1342 "compiler/main.gem"
                GemVal gem_v_cap = gem_table_get(gem_v__for_items_53, gem_v__for_i_53);
#line 1342 "compiler/main.gem"
                gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 1343 "compiler/main.gem"
    GemVal _t2385 = (*gem_v_mangle);
    GemVal _t2386[] = {gem_v_cap};
                GemVal gem_v_mc = _t2385.fn(_t2385.env, _t2386, 1);
#line 1344 "compiler/main.gem"
    GemVal _t2387[] = {gem_v_mc};
    GemVal _t2388[] = {gem_v_mc};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2387, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t2388, 1)), gem_string(";\n")));
            }
        }
    }
    {
#line 1349 "compiler/main.gem"
        GemVal gem_v__for_i_54 = gem_int(0);
#line 1349 "compiler/main.gem"
    GemVal _t2389[] = {gem_v_params};
        GemVal gem_v__for_limit_54 = gem_len_fn(NULL, _t2389, 1);
#line 1349 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_v__for_limit_54))) break;
#line 1349 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_54;
#line 1349 "compiler/main.gem"
            gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 1350 "compiler/main.gem"
    GemVal _t2390 = (*gem_v_mangle);
    GemVal _t2391[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t2390.fn(_t2390.env, _t2391, 1);
#line 1351 "compiler/main.gem"
    GemVal _t2392[] = {gem_v_inner_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2392, 2))) {
#line 1352 "compiler/main.gem"
    GemVal _t2393[] = {gem_v_mp};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2393, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1353 "compiler/main.gem"
    GemVal _t2394[] = {gem_v_mp};
    GemVal _t2395[] = {gem_v_i};
    GemVal _t2396[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2394, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2395, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2396, 1)), gem_string("] : GEM_NIL;\n")));
            } else {
#line 1355 "compiler/main.gem"
    GemVal _t2397[] = {gem_v_mp};
    GemVal _t2398[] = {gem_v_i};
    GemVal _t2399[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2397, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2398, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2399, 1)), gem_string("] : GEM_NIL;\n")));
            }
        }
    }
#line 1359 "compiler/main.gem"
    GemVal _t2400[] = {gem_v_name};
    GemVal _t2401[] = {gem_fn_escape_c_string(NULL, _t2400, 1)};
    GemVal _t2402[] = {(*gem_v_source_name)};
    GemVal _t2403[] = {gem_fn_escape_c_string(NULL, _t2402, 1)};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2401, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2403, 1)), gem_string("\", 0);\n")));
#line 1361 "compiler/main.gem"
    GemVal _t2404[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2404, 1), gem_int(0)))) {
        {
#line 1362 "compiler/main.gem"
            GemVal gem_v__for_i_55 = gem_int(0);
#line 1362 "compiler/main.gem"
    GemVal _t2405[] = {gem_v_body};
            GemVal gem_v__for_limit_55 = gem_sub(gem_len_fn(NULL, _t2405, 1), gem_int(1));
#line 1362 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_v__for_limit_55))) break;
#line 1362 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_55;
#line 1362 "compiler/main.gem"
                gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1363 "compiler/main.gem"
    GemVal _t2406 = (*gem_v_compile_stmt);
    GemVal _t2407[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t2406.fn(_t2406.env, _t2407, 2)), gem_string("\n"));
            }
        }
#line 1365 "compiler/main.gem"
    GemVal _t2408[] = {gem_v_body};
    GemVal _t2409 = (*gem_v_compile_stmt_return);
    GemVal _t2410[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2408, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t2409.fn(_t2409.env, _t2410, 2)), gem_string("\n"));
    } else {
#line 1367 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
#line 1370 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1371 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1372 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1375 "compiler/main.gem"
    GemVal _t2411[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t2411, 2));
    GemVal _t2412 = gem_table_new();
    gem_table_set(_t2412, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t2412, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t2412, gem_string("captures"), gem_v_captures);
    GemVal _t2413 = _t2412;
    gem_pop_frame();
    return _t2413;
}

struct _closure__anon_47 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_extern_fn;
    GemVal *gem_v_compile_fn;
    GemVal *gem_v_compile_stmts;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_extern_c_decls;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_forward_decls;
    GemVal *gem_v_functions;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_source_name;
    GemVal *gem_v_top_level_boxed;
    GemVal *gem_v_top_level_vars;
};
static GemVal _anon_47(void *_env, GemVal *args, int argc) {
    struct _closure__anon_47 *_cls = (struct _closure__anon_47 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_extern_fn = _cls->gem_v_compile_extern_fn;
    GemVal *gem_v_compile_fn = _cls->gem_v_compile_fn;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_extern_c_decls = _cls->gem_v_extern_c_decls;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal *gem_v_functions = _cls->gem_v_functions;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_top_level_boxed = _cls->gem_v_top_level_boxed;
    GemVal *gem_v_top_level_vars = _cls->gem_v_top_level_vars;
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_47", "compiler/main.gem", 0);
#line 1384 "compiler/main.gem"
    GemVal _t2415 = gem_table_new();
    GemVal gem_v_fn_defs = _t2415;
#line 1385 "compiler/main.gem"
    GemVal _t2416 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t2416;
#line 1386 "compiler/main.gem"
    GemVal _t2417 = gem_table_new();
    GemVal gem_v_extern_includes = _t2417;
#line 1387 "compiler/main.gem"
    GemVal _t2418 = gem_table_new();
    GemVal gem_v_top_stmts = _t2418;
    {
#line 1389 "compiler/main.gem"
    GemVal _t2419 = gem_v_ast;
        GemVal gem_v__for_items_56 = gem_table_get(_t2419, gem_string("stmts"));
#line 1389 "compiler/main.gem"
        GemVal gem_v__for_i_56 = gem_int(0);
#line 1389 "compiler/main.gem"
        while (1) {
            GemVal _t2420[] = {gem_v__for_items_56};
            if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_len_fn(NULL, _t2420, 1)))) break;
#line 1389 "compiler/main.gem"
            GemVal gem_v_s = gem_table_get(gem_v__for_items_56, gem_v__for_i_56);
#line 1389 "compiler/main.gem"
            gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1390 "compiler/main.gem"
    GemVal _t2421 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t2421, gem_string("tag")), gem_string("fn_def")))) {
#line 1391 "compiler/main.gem"
    GemVal _t2422[] = {gem_v_fn_defs, gem_v_s};
                (void)(gem_push_fn(NULL, _t2422, 2));
            } else {
#line 1392 "compiler/main.gem"
    GemVal _t2423 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t2423, gem_string("tag")), gem_string("extern_fn")))) {
#line 1393 "compiler/main.gem"
    GemVal _t2424[] = {gem_v_extern_fns_list, gem_v_s};
                    (void)(gem_push_fn(NULL, _t2424, 2));
                } else {
#line 1394 "compiler/main.gem"
    GemVal _t2425 = gem_v_s;
                    if (gem_truthy(gem_eq(gem_table_get(_t2425, gem_string("tag")), gem_string("extern_include")))) {
#line 1395 "compiler/main.gem"
    GemVal _t2426[] = {gem_v_extern_includes, gem_v_s};
                        (void)(gem_push_fn(NULL, _t2426, 2));
                    } else {
#line 1397 "compiler/main.gem"
    GemVal _t2427[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t2427, 2));
                    }
                }
            }
        }
    }
    {
#line 1402 "compiler/main.gem"
        GemVal gem_v__for_items_57 = gem_v_extern_fns_list;
#line 1402 "compiler/main.gem"
        GemVal gem_v__for_i_57 = gem_int(0);
#line 1402 "compiler/main.gem"
        while (1) {
            GemVal _t2428[] = {gem_v__for_items_57};
            if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t2428, 1)))) break;
#line 1402 "compiler/main.gem"
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
#line 1402 "compiler/main.gem"
            gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
#line 1403 "compiler/main.gem"
    GemVal _t2429 = gem_v_ef;
    GemVal _t2430[] = {(*gem_v_defined_fns), gem_table_get(_t2429, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2430, 2));
        }
    }
    {
#line 1407 "compiler/main.gem"
        GemVal gem_v__for_items_58 = gem_v_fn_defs;
#line 1407 "compiler/main.gem"
        GemVal gem_v__for_i_58 = gem_int(0);
#line 1407 "compiler/main.gem"
        while (1) {
            GemVal _t2431[] = {gem_v__for_items_58};
            if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t2431, 1)))) break;
#line 1407 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1407 "compiler/main.gem"
            gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1408 "compiler/main.gem"
    GemVal _t2432 = gem_v_fd;
    GemVal _t2433[] = {(*gem_v_defined_fns), gem_table_get(_t2432, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2433, 2));
#line 1409 "compiler/main.gem"
    GemVal _t2434 = gem_v_fd;
    GemVal _t2435[] = {gem_table_get(_t2434, gem_string("name"))};
            GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2435, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1410 "compiler/main.gem"
    GemVal _t2436[] = {(*gem_v_forward_decls), gem_v_decl};
            (void)(gem_push_fn(NULL, _t2436, 2));
        }
    }
#line 1414 "compiler/main.gem"
    GemVal _t2437[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn_collect_top_let_names(NULL, _t2437, 2));
#line 1415 "compiler/main.gem"
    GemVal _t2438 = gem_table_new();
    GemVal _t2439 = (*gem_v_find_captured_in_scope);
    GemVal _t2440[] = {gem_v_top_stmts, _t2438};
    *gem_v_top_level_boxed = _t2439.fn(_t2439.env, _t2440, 2);
    {
#line 1418 "compiler/main.gem"
        GemVal gem_v__for_items_59 = gem_v_extern_fns_list;
#line 1418 "compiler/main.gem"
        GemVal gem_v__for_i_59 = gem_int(0);
#line 1418 "compiler/main.gem"
        while (1) {
            GemVal _t2441[] = {gem_v__for_items_59};
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_len_fn(NULL, _t2441, 1)))) break;
#line 1418 "compiler/main.gem"
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_59, gem_v__for_i_59);
#line 1418 "compiler/main.gem"
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1419 "compiler/main.gem"
    GemVal _t2442 = (*gem_v_compile_extern_fn);
    GemVal _t2443[] = {gem_v_ef};
    GemVal _t2444[] = {(*gem_v_functions), _t2442.fn(_t2442.env, _t2443, 1)};
            (void)(gem_push_fn(NULL, _t2444, 2));
        }
    }
    {
#line 1423 "compiler/main.gem"
        GemVal gem_v__for_items_60 = gem_v_fn_defs;
#line 1423 "compiler/main.gem"
        GemVal gem_v__for_i_60 = gem_int(0);
#line 1423 "compiler/main.gem"
        while (1) {
            GemVal _t2445[] = {gem_v__for_items_60};
            if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_len_fn(NULL, _t2445, 1)))) break;
#line 1423 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_60, gem_v__for_i_60);
#line 1423 "compiler/main.gem"
            gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1424 "compiler/main.gem"
    GemVal _t2446 = (*gem_v_compile_fn);
    GemVal _t2447[] = {gem_v_fd};
    GemVal _t2448[] = {(*gem_v_functions), _t2446.fn(_t2446.env, _t2447, 1)};
            (void)(gem_push_fn(NULL, _t2448, 2));
        }
    }
#line 1428 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1429 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1430 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1431 "compiler/main.gem"
    GemVal _t2449 = (*gem_v_compile_stmts);
    GemVal _t2450[] = {gem_v_top_stmts, gem_int(1)};
    GemVal gem_v_main_body = _t2449.fn(_t2449.env, _t2450, 2);
#line 1432 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1433 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1435 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
    {
#line 1436 "compiler/main.gem"
        GemVal gem_v__for_items_61 = gem_v_fn_defs;
#line 1436 "compiler/main.gem"
        GemVal gem_v__for_i_61 = gem_int(0);
#line 1436 "compiler/main.gem"
        while (1) {
            GemVal _t2451[] = {gem_v__for_items_61};
            if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_len_fn(NULL, _t2451, 1)))) break;
#line 1436 "compiler/main.gem"
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_61, gem_v__for_i_61);
#line 1436 "compiler/main.gem"
            gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1437 "compiler/main.gem"
    GemVal _t2452 = gem_v_fd;
            if (gem_truthy(gem_eq(gem_table_get(_t2452, gem_string("name")), gem_string("main")))) {
#line 1438 "compiler/main.gem"
                gem_v_has_main = gem_bool(1);
            }
        }
    }
#line 1442 "compiler/main.gem"
    GemVal gem_v_main_c = gem_string("int main(void) {\n");
#line 1443 "compiler/main.gem"
    GemVal _t2453[] = {(*gem_v_source_name)};
    GemVal _t2454[] = {gem_fn_escape_c_string(NULL, _t2453, 1)};
    gem_v_main_c = gem_add(gem_v_main_c, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t2454, 1)), gem_string("\", 0);\n")));
#line 1444 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1445 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
#line 1447 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1448 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
#line 1449 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
#line 1451 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_run_scheduler();\n"));
#line 1452 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_pop_frame();\n    return 0;\n}\n"));
#line 1456 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
    {
#line 1457 "compiler/main.gem"
        GemVal gem_v__for_items_62 = gem_v_extern_includes;
#line 1457 "compiler/main.gem"
        GemVal gem_v__for_i_62 = gem_int(0);
#line 1457 "compiler/main.gem"
        while (1) {
            GemVal _t2455[] = {gem_v__for_items_62};
            if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_len_fn(NULL, _t2455, 1)))) break;
#line 1457 "compiler/main.gem"
            GemVal gem_v_ei = gem_table_get(gem_v__for_items_62, gem_v__for_i_62);
#line 1457 "compiler/main.gem"
            gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1458 "compiler/main.gem"
    GemVal _t2456 = gem_v_ei;
    GemVal _t2457[] = {gem_table_get(_t2456, gem_string("path"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t2457, 1)), gem_string("\"\n")));
        }
    }
#line 1460 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
#line 1464 "compiler/main.gem"
    GemVal _t2458[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2458, 1), gem_int(0)))) {
        {
#line 1465 "compiler/main.gem"
            GemVal gem_v__for_i_63 = gem_int(0);
#line 1465 "compiler/main.gem"
    GemVal _t2459[] = {(*gem_v_extern_c_decls)};
            GemVal gem_v__for_limit_63 = gem_len_fn(NULL, _t2459, 1);
#line 1465 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_v__for_limit_63))) break;
#line 1465 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_63;
#line 1465 "compiler/main.gem"
                gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1466 "compiler/main.gem"
                gem_v_out = gem_add(gem_add(gem_v_out, gem_table_get((*gem_v_extern_c_decls), gem_v_i)), gem_string("\n"));
            }
        }
    }
    {
#line 1471 "compiler/main.gem"
        GemVal gem_v__for_i_64 = gem_int(0);
#line 1471 "compiler/main.gem"
    GemVal _t2460[] = {(*gem_v_forward_decls)};
        GemVal gem_v__for_limit_64 = gem_len_fn(NULL, _t2460, 1);
#line 1471 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_v__for_limit_64))) break;
#line 1471 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_64;
#line 1471 "compiler/main.gem"
            gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1472 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
#line 1473 "compiler/main.gem"
    GemVal _t2461[] = {(*gem_v_forward_decls)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2461, 1), gem_int(1))))) {
#line 1474 "compiler/main.gem"
                gem_v_out = gem_add(gem_v_out, gem_string("\n"));
            }
        }
    }
#line 1477 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1480 "compiler/main.gem"
    GemVal _t2462[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t2462, 1);
    {
#line 1481 "compiler/main.gem"
        GemVal gem_v__for_i_65 = gem_int(0);
#line 1481 "compiler/main.gem"
    GemVal _t2463[] = {gem_v_tlv_keys};
        GemVal gem_v__for_limit_65 = gem_len_fn(NULL, _t2463, 1);
#line 1481 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_v__for_limit_65))) break;
#line 1481 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_65;
#line 1481 "compiler/main.gem"
            gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1482 "compiler/main.gem"
            GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 1483 "compiler/main.gem"
    GemVal _t2464[] = {(*gem_v_top_level_boxed), gem_v_vname};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2464, 2))) {
#line 1484 "compiler/main.gem"
    GemVal _t2465[] = {gem_v_vname};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal *gem_v_"), gem_to_string_fn(NULL, _t2465, 1)), gem_string(";\n")));
            } else {
#line 1486 "compiler/main.gem"
    GemVal _t2466[] = {gem_v_vname};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal gem_v_"), gem_to_string_fn(NULL, _t2466, 1)), gem_string(";\n")));
            }
        }
    }
#line 1489 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
    {
#line 1492 "compiler/main.gem"
        GemVal gem_v__for_i_66 = gem_int(0);
#line 1492 "compiler/main.gem"
    GemVal _t2467[] = {(*gem_v_functions)};
        GemVal gem_v__for_limit_66 = gem_len_fn(NULL, _t2467, 1);
#line 1492 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_v__for_limit_66))) break;
#line 1492 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_66;
#line 1492 "compiler/main.gem"
            gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1493 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
#line 1494 "compiler/main.gem"
    GemVal _t2468[] = {(*gem_v_functions)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2468, 1), gem_int(1))))) {
#line 1495 "compiler/main.gem"
                gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
            }
        }
    }
#line 1498 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
#line 1500 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    GemVal _t2469 = gem_v_out;
    gem_pop_frame();
    return _t2469;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 147 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_codegen", "compiler/main.gem", 147);
#line 148 "compiler/main.gem"
    GemVal _t957 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t957;
#line 149 "compiler/main.gem"
    GemVal _t958 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t958;
#line 150 "compiler/main.gem"
    GemVal _t959 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t959;
#line 151 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 152 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 153 "compiler/main.gem"
    GemVal _t960 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t960;
#line 154 "compiler/main.gem"
    GemVal _t961 = gem_table_new();
    gem_table_set(_t961, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t961, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t961, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t961, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t961, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t961, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t961, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t961, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t961, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t961, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t961, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t961, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t961, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t961, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t961, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t961, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t961, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t961, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t961, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t961, gem_string("self"), gem_string("gem_self_builtin"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t961;
#line 176 "compiler/main.gem"
    GemVal _t962 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t962;
#line 177 "compiler/main.gem"
    GemVal _t963[] = {(*gem_v_builtin_fns)};
    GemVal gem_v__bkeys = gem_keys_fn(NULL, _t963, 1);
    {
#line 178 "compiler/main.gem"
        GemVal gem_v__for_i_7 = gem_int(0);
#line 178 "compiler/main.gem"
    GemVal _t964[] = {gem_v__bkeys};
        GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t964, 1);
#line 178 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 178 "compiler/main.gem"
            GemVal gem_v__bi = gem_v__for_i_7;
#line 178 "compiler/main.gem"
            gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 179 "compiler/main.gem"
            gem_table_set((*gem_v_builtins), gem_table_get(gem_v__bkeys, gem_v__bi), gem_bool(1));
        }
    }
#line 181 "compiler/main.gem"
    GemVal _t965 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t965;
#line 182 "compiler/main.gem"
    GemVal _t966 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t966;
#line 183 "compiler/main.gem"
    GemVal _t967 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t967;
#line 184 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 188 "compiler/main.gem"
    struct _closure__anon_22 *_t970 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t970->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_22, _t970);
#line 193 "compiler/main.gem"
    struct _closure__anon_23 *_t973 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t973->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_23, _t973);
#line 198 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_24, NULL);
#line 204 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 205 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 207 "compiler/main.gem"
    struct _closure__anon_25 *_t1142 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1142->gem_v_builtins = gem_v_builtins;
    _t1142->gem_v_collect_free_node = gem_v_collect_free_node;
    _t1142->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1142->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_25, _t1142);
#line 345 "compiler/main.gem"
    struct _closure__anon_26 *_t1168 = GC_MALLOC(sizeof(struct _closure__anon_26));
    _t1168->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_26, _t1168);
#line 366 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 367 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 369 "compiler/main.gem"
    struct _closure__anon_27 *_t1256 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1256->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1256->gem_v_walk_captures = gem_v_walk_captures;
    _t1256->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_27, _t1256);
#line 449 "compiler/main.gem"
    struct _closure__anon_28 *_t1280 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1280->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_28, _t1280);
#line 466 "compiler/main.gem"
    struct _closure__anon_29 *_t1285 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t1285->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_29, _t1285);
#line 474 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 475 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 476 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 477 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 478 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 479 "compiler/main.gem"
    GemVal *gem_v_compile_if_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if_return = GEM_NIL;
#line 480 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 481 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 482 "compiler/main.gem"
    GemVal *gem_v_compile_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match_return = GEM_NIL;
#line 483 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 484 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 485 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 486 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 487 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 488 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 489 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 490 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 494 "compiler/main.gem"
    struct _closure__anon_30 *_t1410 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t1410->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1410->gem_v_builtin_fns = gem_v_builtin_fns;
    _t1410->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t1410->gem_v_compile_array = gem_v_compile_array;
    _t1410->gem_v_compile_binop = gem_v_compile_binop;
    _t1410->gem_v_compile_call = gem_v_compile_call;
    _t1410->gem_v_compile_expr = gem_v_compile_expr;
    _t1410->gem_v_compile_table = gem_v_compile_table;
    _t1410->gem_v_defined_fns = gem_v_defined_fns;
    _t1410->gem_v_mangle = gem_v_mangle;
    _t1410->gem_v_tmp = gem_v_tmp;
    _t1410->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_30, _t1410);
#line 581 "compiler/main.gem"
    struct _closure__anon_31 *_t1427 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t1427->gem_v_compile_expr = gem_v_compile_expr;
    _t1427->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_31, _t1427);
#line 596 "compiler/main.gem"
    struct _closure__anon_32 *_t1441 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t1441->gem_v_compile_expr = gem_v_compile_expr;
    _t1441->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_32, _t1441);
#line 610 "compiler/main.gem"
    struct _closure__anon_33 *_t1479 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t1479->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1479->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1479->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t1479->gem_v_mangle = gem_v_mangle;
    _t1479->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_33, _t1479);
#line 638 "compiler/main.gem"
    struct _closure__anon_34 *_t1588 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t1588->gem_v_builtin_fns = gem_v_builtin_fns;
    _t1588->gem_v_compile_expr = gem_v_compile_expr;
    _t1588->gem_v_defined_fns = gem_v_defined_fns;
    _t1588->gem_v_source_name = gem_v_source_name;
    _t1588->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_34, _t1588);
#line 747 "compiler/main.gem"
    struct _closure__anon_35 *_t1744 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t1744->gem_v_compile_expr = gem_v_compile_expr;
    _t1744->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_35, _t1744);
#line 827 "compiler/main.gem"
    struct _closure__anon_36 *_t1749 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t1749->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_36, _t1749);
#line 837 "compiler/main.gem"
    struct _closure__anon_37 *_t1932 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t1932->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1932->gem_v_compile_expr = gem_v_compile_expr;
    _t1932->gem_v_compile_if = gem_v_compile_if;
    _t1932->gem_v_compile_match = gem_v_compile_match;
    _t1932->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1932->gem_v_compile_while = gem_v_compile_while;
    _t1932->gem_v_in_top_level = gem_v_in_top_level;
    _t1932->gem_v_mangle = gem_v_mangle;
    _t1932->gem_v_source_name = gem_v_source_name;
    _t1932->gem_v_tmp = gem_v_tmp;
    _t1932->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t1932->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_37, _t1932);
#line 914 "compiler/main.gem"
    struct _closure__anon_38 *_t2081 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2081->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2081->gem_v_compile_expr = gem_v_compile_expr;
    _t2081->gem_v_compile_if_return = gem_v_compile_if_return;
    _t2081->gem_v_compile_match_return = gem_v_compile_match_return;
    _t2081->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2081->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2081->gem_v_compile_while = gem_v_compile_while;
    _t2081->gem_v_mangle = gem_v_mangle;
    _t2081->gem_v_source_name = gem_v_source_name;
    _t2081->gem_v_tmp = gem_v_tmp;
    _t2081->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_38, _t2081);
#line 973 "compiler/main.gem"
    struct _closure__anon_39 *_t2100 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2100->gem_v_compile_expr = gem_v_compile_expr;
    _t2100->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_39, _t2100);
#line 989 "compiler/main.gem"
    struct _closure__anon_40 *_t2136 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t2136->gem_v_compile_expr = gem_v_compile_expr;
    _t2136->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2136->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_40, _t2136);
#line 1024 "compiler/main.gem"
    struct _closure__anon_41 *_t2154 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2154->gem_v_compile_expr = gem_v_compile_expr;
    _t2154->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_41, _t2154);
#line 1039 "compiler/main.gem"
    struct _closure__anon_42 *_t2187 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t2187->gem_v_compile_expr = gem_v_compile_expr;
    _t2187->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2187->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_42, _t2187);
#line 1066 "compiler/main.gem"
    struct _closure__anon_43 *_t2237 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t2237->gem_v_compile_expr = gem_v_compile_expr;
    _t2237->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2237->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2237->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_43, _t2237);
#line 1112 "compiler/main.gem"
    struct _closure__anon_44 *_t2288 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t2288->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t2288->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_44, _t2288);
#line 1236 "compiler/main.gem"
    struct _closure__anon_45 *_t2346 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t2346->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2346->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2346->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2346->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2346->gem_v_in_top_level = gem_v_in_top_level;
    _t2346->gem_v_mangle = gem_v_mangle;
    _t2346->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_fn = gem_make_fn(_anon_45, _t2346);
#line 1304 "compiler/main.gem"
    struct _closure__anon_46 *_t2414 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t2414->gem_v_anon_name = gem_v_anon_name;
    _t2414->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2414->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2414->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2414->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2414->gem_v_functions = gem_v_functions;
    _t2414->gem_v_in_top_level = gem_v_in_top_level;
    _t2414->gem_v_mangle = gem_v_mangle;
    _t2414->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_46, _t2414);
#line 1382 "compiler/main.gem"
    struct _closure__anon_47 *_t2470 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t2470->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2470->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t2470->gem_v_compile_fn = gem_v_compile_fn;
    _t2470->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2470->gem_v_defined_fns = gem_v_defined_fns;
    _t2470->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t2470->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2470->gem_v_forward_decls = gem_v_forward_decls;
    _t2470->gem_v_functions = gem_v_functions;
    _t2470->gem_v_in_top_level = gem_v_in_top_level;
    _t2470->gem_v_source_name = gem_v_source_name;
    _t2470->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t2470->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_47, _t2470);
    GemVal _t2471 = gem_table_new();
    gem_table_set(_t2471, gem_string("compile"), gem_v_compile);
    GemVal _t2472 = _t2471;
    gem_pop_frame();
    return _t2472;
}

static GemVal gem_fn_split_string(void *_env, GemVal *args, int argc) {
#line 1509 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("split_string", "compiler/main.gem", 1509);
#line 1510 "compiler/main.gem"
    GemVal _t2473 = gem_table_new();
    GemVal gem_v_result = _t2473;
#line 1511 "compiler/main.gem"
    GemVal gem_v_current = gem_string("");
    {
#line 1512 "compiler/main.gem"
        GemVal gem_v__for_i_67 = gem_int(0);
#line 1512 "compiler/main.gem"
    GemVal _t2474[] = {gem_v_s};
        GemVal gem_v__for_limit_67 = gem_len_fn(NULL, _t2474, 1);
#line 1512 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_v__for_limit_67))) break;
#line 1512 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_67;
#line 1512 "compiler/main.gem"
            gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1513 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_v_delim))) {
#line 1514 "compiler/main.gem"
    GemVal _t2475[] = {gem_v_result, gem_v_current};
                (void)(gem_push_fn(NULL, _t2475, 2));
#line 1515 "compiler/main.gem"
                gem_v_current = gem_string("");
            } else {
#line 1517 "compiler/main.gem"
                gem_v_current = gem_add(gem_v_current, gem_table_get(gem_v_s, gem_v_i));
            }
        }
    }
#line 1520 "compiler/main.gem"
    GemVal _t2476[] = {gem_v_result, gem_v_current};
    (void)(gem_push_fn(NULL, _t2476, 2));
    GemVal _t2477 = gem_v_result;
    gem_pop_frame();
    return _t2477;
}

static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc) {
#line 1527 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("str_replace", "compiler/main.gem", 1527);
#line 1528 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 1529 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 1530 "compiler/main.gem"
    GemVal _t2478[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t2478, 1);
#line 1531 "compiler/main.gem"
    GemVal _t2479[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t2479, 1);
#line 1532 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
#line 1533 "compiler/main.gem"
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
#line 1534 "compiler/main.gem"
            GemVal gem_v_found = gem_bool(1);
#line 1535 "compiler/main.gem"
            GemVal gem_v_j = gem_int(0);
#line 1536 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
#line 1537 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
#line 1538 "compiler/main.gem"
                    gem_v_found = gem_bool(0);
                    break;
                }
#line 1541 "compiler/main.gem"
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
#line 1543 "compiler/main.gem"
            if (gem_truthy(gem_v_found)) {
#line 1544 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
#line 1545 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
#line 1549 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
#line 1550 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t2480 = gem_v_result;
    gem_pop_frame();
    return _t2480;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 24 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 24);
#line 25 "compiler/main.gem"
    GemVal _t2481 = gem_table_new();
    GemVal gem_v_new_stmts = _t2481;
#line 26 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
    {
#line 27 "compiler/main.gem"
    GemVal _t2482 = gem_v_ast;
        GemVal gem_v__for_items_1 = gem_table_get(_t2482, gem_string("stmts"));
#line 27 "compiler/main.gem"
        GemVal gem_v__for_i_1 = gem_int(0);
#line 27 "compiler/main.gem"
        while (1) {
            GemVal _t2483[] = {gem_v__for_items_1};
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t2483, 1)))) break;
#line 27 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 27 "compiler/main.gem"
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 28 "compiler/main.gem"
    GemVal _t2484[] = {gem_v_stmt};
    GemVal _t2486;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2484, 1), gem_string("table")))) {
        _t2486 = gem_eq(gem_type_fn(NULL, _t2484, 1), gem_string("table"));
    } else {
        GemVal _t2485 = gem_v_stmt;
        _t2486 = gem_eq(gem_table_get(_t2485, gem_string("tag")), gem_string("load"));
    }
            if (gem_truthy(_t2486)) {
#line 29 "compiler/main.gem"
    GemVal _t2487 = gem_v_stmt;
                GemVal gem_v_path = gem_table_get(_t2487, gem_string("path"));
#line 31 "compiler/main.gem"
    GemVal _t2488[] = {gem_v_path};
    GemVal _t2496;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t2488, 1), gem_int(4)))) {
        _t2496 = gem_lt(gem_len_fn(NULL, _t2488, 1), gem_int(4));
    } else {
        GemVal _t2489[] = {gem_v_path};
        GemVal _t2491;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2489, 1), gem_int(4))), gem_string(".")))) {
                _t2491 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2489, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t2490[] = {gem_v_path};
                _t2491 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2490, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t2493;
        if (gem_truthy(_t2491)) {
                _t2493 = _t2491;
        } else {
                GemVal _t2492[] = {gem_v_path};
                _t2493 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2492, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t2495;
        if (gem_truthy(_t2493)) {
                _t2495 = _t2493;
        } else {
                GemVal _t2494[] = {gem_v_path};
                _t2495 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2494, 1), gem_int(1))), gem_string("m"));
        }
        _t2496 = _t2495;
    }
                if (gem_truthy(_t2496)) {
#line 32 "compiler/main.gem"
                    gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
                }
#line 36 "compiler/main.gem"
    GemVal _t2497[] = {gem_v_base_dir, gem_v_path};
                GemVal gem_v_full_path = gem_fn_gem_path_join(NULL, _t2497, 2);
#line 39 "compiler/main.gem"
    GemVal _t2498[] = {gem_v_full_path};
                if (gem_truthy(gem_eq(gem_fn_gem_file_exists(NULL, _t2498, 1), gem_int(0)))) {
#line 40 "compiler/main.gem"
    GemVal _t2499[] = {gem_v_install_root, gem_v_path};
                    gem_v_full_path = gem_fn_gem_path_join(NULL, _t2499, 2);
                }
#line 43 "compiler/main.gem"
    GemVal _t2500[] = {gem_v_full_path};
                gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t2500, 1);
#line 46 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                    continue;
                }
#line 49 "compiler/main.gem"
                gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 51 "compiler/main.gem"
    GemVal _t2501[] = {gem_v_full_path};
                GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2501, 1);
#line 52 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 53 "compiler/main.gem"
    GemVal _t2502[] = {gem_v_full_path};
    GemVal _t2503[] = {gem_add(gem_add(gem_string("cannot load '"), gem_to_string_fn(NULL, _t2502, 1)), gem_string("'"))};
                    (void)(gem_error_at_fn("compiler/main.gem", 53, _t2503, 1));
                }
#line 56 "compiler/main.gem"
    GemVal _t2504[] = {gem_v_source};
                GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t2504, 1);
#line 57 "compiler/main.gem"
    GemVal _t2505[] = {gem_v_full_path};
                GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t2505, 1);
#line 58 "compiler/main.gem"
    GemVal _t2506[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
                GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t2506, 3);
                {
#line 60 "compiler/main.gem"
    GemVal _t2507 = gem_v_resolved;
                    GemVal gem_v__for_items_2 = gem_table_get(_t2507, gem_string("stmts"));
#line 60 "compiler/main.gem"
                    GemVal gem_v__for_i_2 = gem_int(0);
#line 60 "compiler/main.gem"
                    while (1) {
                        GemVal _t2508[] = {gem_v__for_items_2};
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t2508, 1)))) break;
#line 60 "compiler/main.gem"
                        GemVal gem_v_rs = gem_table_get(gem_v__for_items_2, gem_v__for_i_2);
#line 60 "compiler/main.gem"
                        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 61 "compiler/main.gem"
                        gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_rs);
#line 62 "compiler/main.gem"
                        gem_v_count = gem_add(gem_v_count, gem_int(1));
                    }
                }
            } else {
#line 65 "compiler/main.gem"
                gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_stmt);
#line 66 "compiler/main.gem"
                gem_v_count = gem_add(gem_v_count, gem_int(1));
            }
        }
    }
#line 69 "compiler/main.gem"
    GemVal _t2509[] = {gem_v_new_stmts};
    GemVal _t2510 = gem_fn_make_program(NULL, _t2509, 1);
    gem_pop_frame();
    return _t2510;
}

int main(void) {
    gem_push_frame("main", "compiler/main.gem", 0);
#line 20 "compiler/main.gem"
    GemVal _t2511[] = {gem_int(0)};
    GemVal _t2512[] = {gem_fn_gem_get_argv(NULL, _t2511, 1)};
    GemVal _t2513[] = {gem_fn_gem_dirname(NULL, _t2512, 1)};
    gem_v_install_root = gem_fn_gem_dirname(NULL, _t2513, 1);
#line 74 "compiler/main.gem"
    gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
#line 75 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 76 "compiler/main.gem"
    GemVal _t2514[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 76, _t2514, 1));
    }
#line 79 "compiler/main.gem"
    GemVal _t2515[] = {gem_int(1)};
    gem_v_src_path = gem_fn_gem_get_argv(NULL, _t2515, 1);
#line 80 "compiler/main.gem"
    GemVal _t2516[] = {gem_v_src_path};
    gem_v_source = gem_fn_gem_read_file(NULL, _t2516, 1);
#line 81 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 82 "compiler/main.gem"
    GemVal _t2517[] = {gem_v_src_path};
    GemVal _t2518[] = {gem_add(gem_add(gem_string("cannot read '"), gem_to_string_fn(NULL, _t2517, 1)), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 82, _t2518, 1));
    }
#line 85 "compiler/main.gem"
    GemVal _t2519[] = {gem_v_source};
    gem_v_ast = gem_fn_parse_source(NULL, _t2519, 1);
#line 86 "compiler/main.gem"
    GemVal _t2520[] = {gem_v_src_path};
    gem_v_base_dir = gem_fn_gem_dirname(NULL, _t2520, 1);
#line 87 "compiler/main.gem"
    GemVal _t2521 = gem_table_new();
    gem_v_loaded = _t2521;
#line 88 "compiler/main.gem"
    GemVal _t2522[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t2522, 3);
#line 90 "compiler/main.gem"
    GemVal _t2523[] = {gem_v_src_path};
    gem_v_cg = gem_fn_make_codegen(NULL, _t2523, 1);
#line 91 "compiler/main.gem"
    GemVal _t2524 = gem_v_cg;
    GemVal _t2525 = gem_table_get(_t2524, gem_string("compile"));
    GemVal _t2526[] = {gem_v_resolved_ast};
    gem_v_c_code = _t2525.fn(_t2525.env, _t2526, 1);
#line 92 "compiler/main.gem"
    GemVal _t2527[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t2527, 1));
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

