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
static GemVal gem_fn_set_add(void *_env, GemVal *args, int argc);
static GemVal gem_fn_set_contains(void *_env, GemVal *args, int argc);
static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc);
static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc);
static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc);
static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc);
static GemVal gem_fn_collect_top_let_names(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc);
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
    GemVal gem_v_rest_param = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_block_param = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_body = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_anon_fn", "compiler/main.gem", 58);
    GemVal _t25 = gem_table_new();
    gem_table_set(_t25, gem_string("tag"), gem_string("anon_fn"));
    gem_table_set(_t25, gem_string("params"), gem_v_params);
    gem_table_set(_t25, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t25, gem_string("block_param"), gem_v_block_param);
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
    GemVal gem_v_rest_param = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_block_param = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_body = (argc > 4) ? args[4] : GEM_NIL;
    GemVal gem_v_line = (argc > 5) ? args[5] : GEM_NIL;
    gem_push_frame("make_fn_def", "compiler/main.gem", 96);
    GemVal _t43 = gem_table_new();
    gem_table_set(_t43, gem_string("tag"), gem_string("fn_def"));
    gem_table_set(_t43, gem_string("name"), gem_v_name);
    gem_table_set(_t43, gem_string("params"), gem_v_params);
    gem_table_set(_t43, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t43, gem_string("block_param"), gem_v_block_param);
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

    GemVal _t94 = gem_v_result;
    gem_pop_frame();
    return _t94;
}

static GemVal gem_fn_contains_dot(void *_env, GemVal *args, int argc) {
#line 64 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("contains_dot", "compiler/main.gem", 64);
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
#line 113 "compiler/main.gem"
    GemVal gem_v_in_triple_dq = gem_bool(0);
#line 114 "compiler/main.gem"
    GemVal gem_v_triple_dq_indent = gem_int(0);
#line 116 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 117 "compiler/main.gem"
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
#line 120 "compiler/main.gem"
    GemVal _t105;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t105 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t105 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t105)) {
#line 121 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 126 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 127 "compiler/main.gem"
            while (1) {
                GemVal _t106;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t106 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t106 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t106)) break;
#line 128 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 134 "compiler/main.gem"
    GemVal _t107;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t107 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t107 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t107)) {
#line 135 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 136 "compiler/main.gem"
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
#line 137 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 139 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 140 "compiler/main.gem"
    GemVal _t112 = gem_table_new();
    gem_table_set(_t112, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t112, gem_string("value"), gem_string("\n"));
    gem_table_set(_t112, gem_string("line"), gem_v_line);
    GemVal _t113[] = {gem_v_tokens, _t112};
                (void)(gem_push_fn(NULL, _t113, 2));
            }
#line 142 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 143 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
            }
#line 145 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 147 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 148 "compiler/main.gem"
                GemVal gem_v_c2 = gem_table_get(gem_v_source, gem_v_pos);
#line 149 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_c2, gem_string("\n")))) {
#line 150 "compiler/main.gem"
                    gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 151 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                } else {
#line 152 "compiler/main.gem"
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
#line 153 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 154 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 155 "compiler/main.gem"
                            while (1) {
                                GemVal _t116;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t116 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t116 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t116)) break;
#line 156 "compiler/main.gem"
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
#line 166 "compiler/main.gem"
    GemVal _t117[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t117, 1))) {
#line 167 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 168 "compiler/main.gem"
            while (1) {
                GemVal _t119;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t119 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t118[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t119 = gem_fn_is_digit(NULL, _t118, 1);
                }
                if (!gem_truthy(_t119)) break;
#line 169 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 170 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 172 "compiler/main.gem"
    GemVal _t120;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t120 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t120 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t120;
#line 173 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 174 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 175 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 176 "compiler/main.gem"
                while (1) {
                    GemVal _t122;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t122 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t121[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t122 = gem_fn_is_digit(NULL, _t121, 1);
                    }
                    if (!gem_truthy(_t122)) break;
#line 177 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 178 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 181 "compiler/main.gem"
    GemVal _t123 = gem_table_new();
    gem_table_set(_t123, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t123, gem_string("value"), gem_v_val);
    gem_table_set(_t123, gem_string("line"), gem_v_line);
    GemVal _t124[] = {gem_v_tokens, _t123};
            (void)(gem_push_fn(NULL, _t124, 2));
            continue;
        }
#line 186 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 187 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 190 "compiler/main.gem"
    GemVal _t125;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t125 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t125 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t126;
    if (!gem_truthy(_t125)) {
        _t126 = _t125;
    } else {
        _t126 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
            if (gem_truthy(_t126)) {
#line 191 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 193 "compiler/main.gem"
                while (1) {
                    GemVal _t128;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t128 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t127;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t127 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t127 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t128 = _t127;
                    }
                    if (!gem_truthy(_t128)) break;
#line 194 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 196 "compiler/main.gem"
    GemVal _t129;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t129 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t129 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t129)) {
#line 197 "compiler/main.gem"
    GemVal _t130[] = {gem_v_line};
    GemVal _t131[] = {gem_add(gem_string("triple-quoted string must be followed by a newline at line "), gem_to_string_fn(NULL, _t130, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 197, _t131, 1));
                }
#line 199 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 200 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 203 "compiler/main.gem"
                GemVal gem_v_tq_scan = gem_v_pos;
#line 204 "compiler/main.gem"
                GemVal gem_v_tq_bi = gem_int(0);
#line 205 "compiler/main.gem"
                GemVal gem_v_tq_found = gem_bool(0);
#line 206 "compiler/main.gem"
                while (1) {
                    GemVal _t132;
                    if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                        _t132 = gem_lt(gem_v_tq_scan, gem_v_length);
                    } else {
                                        _t132 = gem_not(gem_v_tq_found);
                    }
                    if (!gem_truthy(_t132)) break;
#line 207 "compiler/main.gem"
                    GemVal gem_v_tq_ind = gem_int(0);
#line 208 "compiler/main.gem"
                    while (1) {
                        GemVal _t133;
                        if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                _t133 = gem_lt(gem_v_tq_scan, gem_v_length);
                        } else {
                                                _t133 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t133)) break;
#line 209 "compiler/main.gem"
                        gem_v_tq_ind = gem_add(gem_v_tq_ind, gem_int(1));
#line 210 "compiler/main.gem"
                        gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                    }
#line 212 "compiler/main.gem"
    GemVal _t134;
    if (!gem_truthy(gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length))) {
        _t134 = gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length);
    } else {
        _t134 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\""));
    }
    GemVal _t135;
    if (!gem_truthy(_t134)) {
        _t135 = _t134;
    } else {
        _t135 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(1))), gem_string("\""));
    }
    GemVal _t136;
    if (!gem_truthy(_t135)) {
        _t136 = _t135;
    } else {
        _t136 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(2))), gem_string("\""));
    }
                    if (gem_truthy(_t136)) {
#line 213 "compiler/main.gem"
                        gem_v_tq_bi = gem_v_tq_ind;
#line 214 "compiler/main.gem"
                        gem_v_tq_found = gem_bool(1);
                    } else {
#line 216 "compiler/main.gem"
                        while (1) {
                            GemVal _t137;
                            if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                        _t137 = gem_lt(gem_v_tq_scan, gem_v_length);
                            } else {
                                                        _t137 = gem_neq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t137)) break;
#line 217 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
#line 219 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
#line 220 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
                    }
                }
#line 224 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_tq_found))) {
#line 225 "compiler/main.gem"
    GemVal _t138[] = {gem_v_line};
    GemVal _t139[] = {gem_add(gem_string("unterminated triple-quoted string at line "), gem_to_string_fn(NULL, _t138, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 225, _t139, 1));
                }
#line 227 "compiler/main.gem"
                gem_v_triple_dq_indent = gem_v_tq_bi;
#line 230 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 231 "compiler/main.gem"
                GemVal gem_v_hit_interp = gem_bool(0);
#line 232 "compiler/main.gem"
                GemVal gem_v_tq_als = gem_bool(1);
#line 234 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 235 "compiler/main.gem"
                    if (gem_truthy(gem_v_tq_als)) {
#line 236 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(0);
#line 237 "compiler/main.gem"
                        GemVal gem_v_tq_skip = gem_int(0);
#line 238 "compiler/main.gem"
                        while (1) {
                            GemVal _t140;
                            if (!gem_truthy(gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent))) {
                                                        _t140 = gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t140 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t141;
                            if (!gem_truthy(_t140)) {
                                                        _t141 = _t140;
                            } else {
                                                        _t141 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t141)) break;
#line 239 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 240 "compiler/main.gem"
                            gem_v_tq_skip = gem_add(gem_v_tq_skip, gem_int(1));
                        }
#line 243 "compiler/main.gem"
    GemVal _t142;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t142 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t142 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t143;
    if (!gem_truthy(_t142)) {
        _t143 = _t142;
    } else {
        _t143 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t144;
    if (!gem_truthy(_t143)) {
        _t144 = _t143;
    } else {
        _t144 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t144)) {
#line 244 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 246 "compiler/main.gem"
    GemVal _t145[] = {gem_v_val};
    GemVal _t147;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t145, 1), gem_int(0)))) {
        _t147 = gem_gt(gem_len_fn(NULL, _t145, 1), gem_int(0));
    } else {
        GemVal _t146[] = {gem_v_val};
        _t147 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t146, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t147)) {
#line 247 "compiler/main.gem"
    GemVal _t148[] = {gem_v_val};
    GemVal _t149[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t148, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t149, 3);
                            }
                            break;
                        }
                    }
#line 253 "compiler/main.gem"
                    GemVal gem_v_tqc = gem_table_get(gem_v_source, gem_v_pos);
#line 254 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\n")))) {
#line 255 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 256 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 257 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 258 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(1);
                        continue;
                    }
#line 261 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\\")))) {
#line 262 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 263 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 264 "compiler/main.gem"
    GemVal _t150[] = {gem_v_line};
    GemVal _t151[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t150, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 264, _t151, 1));
                        }
#line 266 "compiler/main.gem"
                        GemVal gem_v_tqe = gem_table_get(gem_v_source, gem_v_pos);
#line 267 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tqe, gem_string("n")))) {
#line 268 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                        } else {
#line 269 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tqe, gem_string("r")))) {
#line 270 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_v_CR);
                            } else {
#line 271 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tqe, gem_string("t")))) {
#line 272 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                                } else {
#line 273 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tqe, gem_string("\\")))) {
#line 274 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                                    } else {
#line 275 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tqe, gem_string("\"")))) {
#line 276 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                        } else {
#line 277 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tqe, gem_string("{")))) {
#line 278 "compiler/main.gem"
                                                gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                            } else {
#line 279 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tqe, gem_string("}")))) {
#line 280 "compiler/main.gem"
                                                    gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                                } else {
#line 281 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tqe, gem_string("0")))) {
#line 282 "compiler/main.gem"
                                                        gem_v_val = gem_add(gem_v_val, gem_v_NUL);
                                                    } else {
#line 284 "compiler/main.gem"
                                                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqe);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
#line 286 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 289 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("{")))) {
#line 290 "compiler/main.gem"
    GemVal _t152 = gem_table_new();
    gem_table_set(_t152, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t152, gem_string("value"), gem_string(""));
    gem_table_set(_t152, gem_string("line"), gem_v_line);
    GemVal _t153[] = {gem_v_tokens, _t152};
                        (void)(gem_push_fn(NULL, _t153, 2));
#line 291 "compiler/main.gem"
    GemVal _t154 = gem_table_new();
    gem_table_set(_t154, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t154, gem_string("value"), gem_v_val);
    gem_table_set(_t154, gem_string("line"), gem_v_line);
    GemVal _t155[] = {gem_v_tokens, _t154};
                        (void)(gem_push_fn(NULL, _t155, 2));
#line 292 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 293 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 294 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 295 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 296 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 297 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 300 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqc);
#line 301 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 304 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 305 "compiler/main.gem"
    GemVal _t156 = gem_table_new();
    gem_table_set(_t156, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t156, gem_string("value"), gem_v_val);
    gem_table_set(_t156, gem_string("line"), gem_v_line);
    GemVal _t157[] = {gem_v_tokens, _t156};
                    (void)(gem_push_fn(NULL, _t157, 2));
                }
                continue;
            }
#line 311 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 312 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 313 "compiler/main.gem"
            while (1) {
                GemVal _t158;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t158 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t158 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t158)) break;
#line 314 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 315 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 316 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 317 "compiler/main.gem"
    GemVal _t159[] = {gem_v_line};
    GemVal _t160[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t159, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 317, _t160, 1));
                    }
#line 319 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 320 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 321 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
#line 322 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 323 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_CR);
                        } else {
#line 324 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 325 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                            } else {
#line 326 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 327 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                                } else {
#line 328 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
#line 329 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                    } else {
#line 330 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
#line 331 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                        } else {
#line 332 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
#line 333 "compiler/main.gem"
                                                gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                            } else {
#line 334 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
#line 335 "compiler/main.gem"
                                                    gem_v_val = gem_add(gem_v_val, gem_v_NUL);
                                                } else {
#line 337 "compiler/main.gem"
                                                    gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
#line 339 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 342 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 343 "compiler/main.gem"
    GemVal _t161 = gem_table_new();
    gem_table_set(_t161, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t161, gem_string("value"), gem_string(""));
    gem_table_set(_t161, gem_string("line"), gem_v_line);
    GemVal _t162[] = {gem_v_tokens, _t161};
                    (void)(gem_push_fn(NULL, _t162, 2));
#line 344 "compiler/main.gem"
    GemVal _t163 = gem_table_new();
    gem_table_set(_t163, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t163, gem_string("value"), gem_v_val);
    gem_table_set(_t163, gem_string("line"), gem_v_line);
    GemVal _t164[] = {gem_v_tokens, _t163};
                    (void)(gem_push_fn(NULL, _t164, 2));
#line 345 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 346 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 347 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 348 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 349 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 352 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 353 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 355 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 356 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 357 "compiler/main.gem"
    GemVal _t165[] = {gem_v_line};
    GemVal _t166[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t165, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 357, _t166, 1));
                }
#line 359 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 360 "compiler/main.gem"
    GemVal _t167 = gem_table_new();
    gem_table_set(_t167, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t167, gem_string("value"), gem_v_val);
    gem_table_set(_t167, gem_string("line"), gem_v_line);
    GemVal _t168[] = {gem_v_tokens, _t167};
                (void)(gem_push_fn(NULL, _t168, 2));
            }
            continue;
        }
#line 366 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("'")))) {
#line 367 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 370 "compiler/main.gem"
    GemVal _t169;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t169 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t169 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t170;
    if (!gem_truthy(_t169)) {
        _t170 = _t169;
    } else {
        _t170 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
            if (gem_truthy(_t170)) {
#line 371 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 373 "compiler/main.gem"
                while (1) {
                    GemVal _t172;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t172 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t171;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t171 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t171 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t172 = _t171;
                    }
                    if (!gem_truthy(_t172)) break;
#line 374 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 376 "compiler/main.gem"
    GemVal _t173;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t173 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t173 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t173)) {
#line 377 "compiler/main.gem"
    GemVal _t174[] = {gem_v_line};
    GemVal _t175[] = {gem_add(gem_string("triple-quoted string must be followed by a newline at line "), gem_to_string_fn(NULL, _t174, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 377, _t175, 1));
                }
#line 379 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 380 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 383 "compiler/main.gem"
                GemVal gem_v_sq_scan = gem_v_pos;
#line 384 "compiler/main.gem"
                GemVal gem_v_sq_bi = gem_int(0);
#line 385 "compiler/main.gem"
                GemVal gem_v_sq_found = gem_bool(0);
#line 386 "compiler/main.gem"
                while (1) {
                    GemVal _t176;
                    if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                        _t176 = gem_lt(gem_v_sq_scan, gem_v_length);
                    } else {
                                        _t176 = gem_not(gem_v_sq_found);
                    }
                    if (!gem_truthy(_t176)) break;
#line 387 "compiler/main.gem"
                    GemVal gem_v_sq_ind = gem_int(0);
#line 388 "compiler/main.gem"
                    while (1) {
                        GemVal _t177;
                        if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                _t177 = gem_lt(gem_v_sq_scan, gem_v_length);
                        } else {
                                                _t177 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t177)) break;
#line 389 "compiler/main.gem"
                        gem_v_sq_ind = gem_add(gem_v_sq_ind, gem_int(1));
#line 390 "compiler/main.gem"
                        gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                    }
#line 392 "compiler/main.gem"
    GemVal _t178;
    if (!gem_truthy(gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length))) {
        _t178 = gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length);
    } else {
        _t178 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("'"));
    }
    GemVal _t179;
    if (!gem_truthy(_t178)) {
        _t179 = _t178;
    } else {
        _t179 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(1))), gem_string("'"));
    }
    GemVal _t180;
    if (!gem_truthy(_t179)) {
        _t180 = _t179;
    } else {
        _t180 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(2))), gem_string("'"));
    }
                    if (gem_truthy(_t180)) {
#line 393 "compiler/main.gem"
                        gem_v_sq_bi = gem_v_sq_ind;
#line 394 "compiler/main.gem"
                        gem_v_sq_found = gem_bool(1);
                    } else {
#line 396 "compiler/main.gem"
                        while (1) {
                            GemVal _t181;
                            if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                        _t181 = gem_lt(gem_v_sq_scan, gem_v_length);
                            } else {
                                                        _t181 = gem_neq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t181)) break;
#line 397 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
#line 399 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
#line 400 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
                    }
                }
#line 404 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_sq_found))) {
#line 405 "compiler/main.gem"
    GemVal _t182[] = {gem_v_line};
    GemVal _t183[] = {gem_add(gem_string("unterminated triple-quoted string at line "), gem_to_string_fn(NULL, _t182, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 405, _t183, 1));
                }
#line 409 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 410 "compiler/main.gem"
                GemVal gem_v_sq_als = gem_bool(1);
#line 412 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 413 "compiler/main.gem"
                    if (gem_truthy(gem_v_sq_als)) {
#line 414 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(0);
#line 415 "compiler/main.gem"
                        GemVal gem_v_sq_skip = gem_int(0);
#line 416 "compiler/main.gem"
                        while (1) {
                            GemVal _t184;
                            if (!gem_truthy(gem_lt(gem_v_sq_skip, gem_v_sq_bi))) {
                                                        _t184 = gem_lt(gem_v_sq_skip, gem_v_sq_bi);
                            } else {
                                                        _t184 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t185;
                            if (!gem_truthy(_t184)) {
                                                        _t185 = _t184;
                            } else {
                                                        _t185 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t185)) break;
#line 417 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 418 "compiler/main.gem"
                            gem_v_sq_skip = gem_add(gem_v_sq_skip, gem_int(1));
                        }
#line 421 "compiler/main.gem"
    GemVal _t186;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t186 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t186 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t187;
    if (!gem_truthy(_t186)) {
        _t187 = _t186;
    } else {
        _t187 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
    GemVal _t188;
    if (!gem_truthy(_t187)) {
        _t188 = _t187;
    } else {
        _t188 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("'"));
    }
                        if (gem_truthy(_t188)) {
#line 422 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 424 "compiler/main.gem"
    GemVal _t189[] = {gem_v_val};
    GemVal _t191;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t189, 1), gem_int(0)))) {
        _t191 = gem_gt(gem_len_fn(NULL, _t189, 1), gem_int(0));
    } else {
        GemVal _t190[] = {gem_v_val};
        _t191 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t190, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t191)) {
#line 425 "compiler/main.gem"
    GemVal _t192[] = {gem_v_val};
    GemVal _t193[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t192, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t193, 3);
                            }
                            break;
                        }
                    }
#line 431 "compiler/main.gem"
                    GemVal gem_v_sqc = gem_table_get(gem_v_source, gem_v_pos);
#line 432 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\n")))) {
#line 433 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 434 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 435 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 436 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(1);
                        continue;
                    }
#line 439 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\\")))) {
#line 440 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 441 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 442 "compiler/main.gem"
    GemVal _t194[] = {gem_v_line};
    GemVal _t195[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t194, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 442, _t195, 1));
                        }
#line 444 "compiler/main.gem"
                        GemVal gem_v_sqe = gem_table_get(gem_v_source, gem_v_pos);
#line 445 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_sqe, gem_string("n")))) {
#line 446 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                        } else {
#line 447 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_sqe, gem_string("r")))) {
#line 448 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_v_CR);
                            } else {
#line 449 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_sqe, gem_string("t")))) {
#line 450 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                                } else {
#line 451 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_sqe, gem_string("\\")))) {
#line 452 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                                    } else {
#line 453 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_sqe, gem_string("'")))) {
#line 454 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_v_val, gem_string("'"));
                                        } else {
#line 456 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_sqe);
                                        }
                                    }
                                }
                            }
                        }
#line 458 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 461 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_sqc);
#line 462 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 465 "compiler/main.gem"
    GemVal _t196 = gem_table_new();
    gem_table_set(_t196, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t196, gem_string("value"), gem_v_val);
    gem_table_set(_t196, gem_string("line"), gem_v_line);
    GemVal _t197[] = {gem_v_tokens, _t196};
                (void)(gem_push_fn(NULL, _t197, 2));
                continue;
            }
#line 470 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 471 "compiler/main.gem"
            while (1) {
                GemVal _t198;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t198 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t198 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
                }
                if (!gem_truthy(_t198)) break;
#line 472 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 473 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 474 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 475 "compiler/main.gem"
    GemVal _t199[] = {gem_v_line};
    GemVal _t200[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t199, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 475, _t200, 1));
                    }
#line 477 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 478 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 479 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
#line 480 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 481 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_CR);
                        } else {
#line 482 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 483 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                            } else {
#line 484 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 485 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                                } else {
#line 486 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("'")))) {
#line 487 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("'"));
                                    } else {
#line 489 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                    }
                                }
                            }
                        }
                    }
#line 491 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 494 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 495 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 497 "compiler/main.gem"
            if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 498 "compiler/main.gem"
    GemVal _t201[] = {gem_v_line};
    GemVal _t202[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t201, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 498, _t202, 1));
            }
#line 500 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 501 "compiler/main.gem"
    GemVal _t203 = gem_table_new();
    gem_table_set(_t203, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t203, gem_string("value"), gem_v_val);
    gem_table_set(_t203, gem_string("line"), gem_v_line);
    GemVal _t204[] = {gem_v_tokens, _t203};
            (void)(gem_push_fn(NULL, _t204, 2));
            continue;
        }
#line 506 "compiler/main.gem"
    GemVal _t205[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t205, 1))) {
#line 507 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 508 "compiler/main.gem"
            while (1) {
                GemVal _t207;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t207 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t206[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t207 = gem_fn_is_alnum(NULL, _t206, 1);
                }
                if (!gem_truthy(_t207)) break;
#line 509 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 510 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 512 "compiler/main.gem"
    GemVal _t208[] = {gem_v_kw, gem_v_val};
            if (gem_truthy(gem_has_key_fn(NULL, _t208, 2))) {
#line 513 "compiler/main.gem"
    GemVal _t209 = gem_table_new();
    gem_table_set(_t209, gem_string("type"), gem_v_val);
    gem_table_set(_t209, gem_string("value"), gem_v_val);
    gem_table_set(_t209, gem_string("line"), gem_v_line);
    GemVal _t210[] = {gem_v_tokens, _t209};
                (void)(gem_push_fn(NULL, _t210, 2));
            } else {
#line 515 "compiler/main.gem"
    GemVal _t211 = gem_table_new();
    gem_table_set(_t211, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t211, gem_string("value"), gem_v_val);
    gem_table_set(_t211, gem_string("line"), gem_v_line);
    GemVal _t212[] = {gem_v_tokens, _t211};
                (void)(gem_push_fn(NULL, _t212, 2));
            }
            continue;
        }
#line 521 "compiler/main.gem"
        GemVal gem_v_next = gem_string("");
#line 522 "compiler/main.gem"
        if (gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
#line 523 "compiler/main.gem"
            gem_v_next = gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1)));
        }
#line 526 "compiler/main.gem"
    GemVal _t213;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t213 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t213 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t213)) {
#line 527 "compiler/main.gem"
    GemVal _t214 = gem_table_new();
    gem_table_set(_t214, gem_string("type"), gem_string("=="));
    gem_table_set(_t214, gem_string("value"), gem_string("=="));
    gem_table_set(_t214, gem_string("line"), gem_v_line);
    GemVal _t215[] = {gem_v_tokens, _t214};
            (void)(gem_push_fn(NULL, _t215, 2));
#line 528 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 531 "compiler/main.gem"
    GemVal _t216;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t216 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t216 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t216)) {
#line 532 "compiler/main.gem"
    GemVal _t217 = gem_table_new();
    gem_table_set(_t217, gem_string("type"), gem_string("!="));
    gem_table_set(_t217, gem_string("value"), gem_string("!="));
    gem_table_set(_t217, gem_string("line"), gem_v_line);
    GemVal _t218[] = {gem_v_tokens, _t217};
            (void)(gem_push_fn(NULL, _t218, 2));
#line 533 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 536 "compiler/main.gem"
    GemVal _t219;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t219 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t219 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t219)) {
#line 537 "compiler/main.gem"
    GemVal _t220 = gem_table_new();
    gem_table_set(_t220, gem_string("type"), gem_string("<="));
    gem_table_set(_t220, gem_string("value"), gem_string("<="));
    gem_table_set(_t220, gem_string("line"), gem_v_line);
    GemVal _t221[] = {gem_v_tokens, _t220};
            (void)(gem_push_fn(NULL, _t221, 2));
#line 538 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 541 "compiler/main.gem"
    GemVal _t222;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t222 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t222 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t222)) {
#line 542 "compiler/main.gem"
    GemVal _t223 = gem_table_new();
    gem_table_set(_t223, gem_string("type"), gem_string(">="));
    gem_table_set(_t223, gem_string("value"), gem_string(">="));
    gem_table_set(_t223, gem_string("line"), gem_v_line);
    GemVal _t224[] = {gem_v_tokens, _t223};
            (void)(gem_push_fn(NULL, _t224, 2));
#line 543 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 546 "compiler/main.gem"
    GemVal _t225;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t225 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t225 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t225)) {
#line 547 "compiler/main.gem"
    GemVal _t226 = gem_table_new();
    gem_table_set(_t226, gem_string("type"), gem_string("+="));
    gem_table_set(_t226, gem_string("value"), gem_string("+="));
    gem_table_set(_t226, gem_string("line"), gem_v_line);
    GemVal _t227[] = {gem_v_tokens, _t226};
            (void)(gem_push_fn(NULL, _t227, 2));
#line 548 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 551 "compiler/main.gem"
    GemVal _t228;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t228 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t228 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t228)) {
#line 552 "compiler/main.gem"
    GemVal _t229 = gem_table_new();
    gem_table_set(_t229, gem_string("type"), gem_string("-="));
    gem_table_set(_t229, gem_string("value"), gem_string("-="));
    gem_table_set(_t229, gem_string("line"), gem_v_line);
    GemVal _t230[] = {gem_v_tokens, _t229};
            (void)(gem_push_fn(NULL, _t230, 2));
#line 553 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 556 "compiler/main.gem"
    GemVal _t231;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t231 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t231 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t231)) {
#line 557 "compiler/main.gem"
    GemVal _t232 = gem_table_new();
    gem_table_set(_t232, gem_string("type"), gem_string("*="));
    gem_table_set(_t232, gem_string("value"), gem_string("*="));
    gem_table_set(_t232, gem_string("line"), gem_v_line);
    GemVal _t233[] = {gem_v_tokens, _t232};
            (void)(gem_push_fn(NULL, _t233, 2));
#line 558 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 561 "compiler/main.gem"
    GemVal _t234;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t234 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t234 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t234)) {
#line 562 "compiler/main.gem"
    GemVal _t235 = gem_table_new();
    gem_table_set(_t235, gem_string("type"), gem_string("/="));
    gem_table_set(_t235, gem_string("value"), gem_string("/="));
    gem_table_set(_t235, gem_string("line"), gem_v_line);
    GemVal _t236[] = {gem_v_tokens, _t235};
            (void)(gem_push_fn(NULL, _t236, 2));
#line 563 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 566 "compiler/main.gem"
    GemVal _t237;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t237 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t237 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t237)) {
#line 567 "compiler/main.gem"
    GemVal _t238 = gem_table_new();
    gem_table_set(_t238, gem_string("type"), gem_string("->"));
    gem_table_set(_t238, gem_string("value"), gem_string("->"));
    gem_table_set(_t238, gem_string("line"), gem_v_line);
    GemVal _t239[] = {gem_v_tokens, _t238};
            (void)(gem_push_fn(NULL, _t239, 2));
#line 568 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 573 "compiler/main.gem"
    GemVal _t240;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t240 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t240 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t241;
    if (gem_truthy(_t240)) {
        _t241 = _t240;
    } else {
        _t241 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t242;
    if (gem_truthy(_t241)) {
        _t242 = _t241;
    } else {
        _t242 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t243;
    if (gem_truthy(_t242)) {
        _t243 = _t242;
    } else {
        _t243 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t243)) {
#line 574 "compiler/main.gem"
    GemVal _t244 = gem_table_new();
    gem_table_set(_t244, gem_string("type"), gem_v_ch);
    gem_table_set(_t244, gem_string("value"), gem_v_ch);
    gem_table_set(_t244, gem_string("line"), gem_v_line);
    GemVal _t245[] = {gem_v_tokens, _t244};
            (void)(gem_push_fn(NULL, _t245, 2));
#line 575 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 578 "compiler/main.gem"
    GemVal _t246;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t246 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t246 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t247;
    if (gem_truthy(_t246)) {
        _t247 = _t246;
    } else {
        _t247 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t247)) {
#line 579 "compiler/main.gem"
    GemVal _t248 = gem_table_new();
    gem_table_set(_t248, gem_string("type"), gem_v_ch);
    gem_table_set(_t248, gem_string("value"), gem_v_ch);
    gem_table_set(_t248, gem_string("line"), gem_v_line);
    GemVal _t249[] = {gem_v_tokens, _t248};
            (void)(gem_push_fn(NULL, _t249, 2));
#line 580 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 584 "compiler/main.gem"
    GemVal _t250;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t250 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t250 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t250)) {
#line 585 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 586 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 587 "compiler/main.gem"
    GemVal _t251 = gem_table_new();
    gem_table_set(_t251, gem_string("type"), gem_string("{"));
    gem_table_set(_t251, gem_string("value"), gem_string("{"));
    gem_table_set(_t251, gem_string("line"), gem_v_line);
    GemVal _t252[] = {gem_v_tokens, _t251};
            (void)(gem_push_fn(NULL, _t252, 2));
#line 588 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 591 "compiler/main.gem"
    GemVal _t253;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t253 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t253 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t253)) {
#line 592 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 593 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 594 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 595 "compiler/main.gem"
    GemVal _t254 = gem_table_new();
    gem_table_set(_t254, gem_string("type"), gem_string("}"));
    gem_table_set(_t254, gem_string("value"), gem_string("}"));
    gem_table_set(_t254, gem_string("line"), gem_v_line);
    GemVal _t255[] = {gem_v_tokens, _t254};
                (void)(gem_push_fn(NULL, _t255, 2));
#line 596 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                continue;
            }
#line 600 "compiler/main.gem"
            gem_v_interp_depth = gem_sub(gem_v_interp_depth, gem_int(1));
#line 601 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 604 "compiler/main.gem"
            if (gem_truthy(gem_v_in_triple_dq)) {
#line 605 "compiler/main.gem"
                gem_v_in_triple_dq = gem_bool(0);
#line 606 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 607 "compiler/main.gem"
                GemVal gem_v_hit_interp = gem_bool(0);
#line 608 "compiler/main.gem"
                GemVal gem_v_tqr_als = gem_bool(0);
#line 610 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 611 "compiler/main.gem"
                    if (gem_truthy(gem_v_tqr_als)) {
#line 612 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(0);
#line 613 "compiler/main.gem"
                        GemVal gem_v_tqr_skip = gem_int(0);
#line 614 "compiler/main.gem"
                        while (1) {
                            GemVal _t256;
                            if (!gem_truthy(gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent))) {
                                                        _t256 = gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t256 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t257;
                            if (!gem_truthy(_t256)) {
                                                        _t257 = _t256;
                            } else {
                                                        _t257 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t257)) break;
#line 615 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 616 "compiler/main.gem"
                            gem_v_tqr_skip = gem_add(gem_v_tqr_skip, gem_int(1));
                        }
#line 619 "compiler/main.gem"
    GemVal _t258;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t258 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t258 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t259;
    if (!gem_truthy(_t258)) {
        _t259 = _t258;
    } else {
        _t259 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t260;
    if (!gem_truthy(_t259)) {
        _t260 = _t259;
    } else {
        _t260 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t260)) {
#line 620 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 621 "compiler/main.gem"
    GemVal _t261[] = {gem_v_val};
    GemVal _t263;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t261, 1), gem_int(0)))) {
        _t263 = gem_gt(gem_len_fn(NULL, _t261, 1), gem_int(0));
    } else {
        GemVal _t262[] = {gem_v_val};
        _t263 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t262, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t263)) {
#line 622 "compiler/main.gem"
    GemVal _t264[] = {gem_v_val};
    GemVal _t265[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t264, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t265, 3);
                            }
                            break;
                        }
                    }
#line 628 "compiler/main.gem"
                    GemVal gem_v_tqrc = gem_table_get(gem_v_source, gem_v_pos);
#line 629 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("\n")))) {
#line 630 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 631 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 632 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 633 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(1);
                        continue;
                    }
#line 636 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("\\")))) {
#line 637 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 638 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 639 "compiler/main.gem"
    GemVal _t266[] = {gem_v_line};
    GemVal _t267[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t266, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 639, _t267, 1));
                        }
#line 641 "compiler/main.gem"
                        GemVal gem_v_tqre = gem_table_get(gem_v_source, gem_v_pos);
#line 642 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tqre, gem_string("n")))) {
#line 643 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                        } else {
#line 644 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tqre, gem_string("r")))) {
#line 645 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_v_CR);
                            } else {
#line 646 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tqre, gem_string("t")))) {
#line 647 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                                } else {
#line 648 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tqre, gem_string("\\")))) {
#line 649 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                                    } else {
#line 650 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tqre, gem_string("\"")))) {
#line 651 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                        } else {
#line 652 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tqre, gem_string("{")))) {
#line 653 "compiler/main.gem"
                                                gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                            } else {
#line 654 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tqre, gem_string("}")))) {
#line 655 "compiler/main.gem"
                                                    gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                                } else {
#line 656 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tqre, gem_string("0")))) {
#line 657 "compiler/main.gem"
                                                        gem_v_val = gem_add(gem_v_val, gem_v_NUL);
                                                    } else {
#line 659 "compiler/main.gem"
                                                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqre);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
#line 661 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 664 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("{")))) {
#line 665 "compiler/main.gem"
    GemVal _t268 = gem_table_new();
    gem_table_set(_t268, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t268, gem_string("value"), gem_v_val);
    gem_table_set(_t268, gem_string("line"), gem_v_line);
    GemVal _t269[] = {gem_v_tokens, _t268};
                        (void)(gem_push_fn(NULL, _t269, 2));
#line 666 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 667 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 668 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 669 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 670 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 671 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 674 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqrc);
#line 675 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 678 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 679 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 680 "compiler/main.gem"
    GemVal _t270[] = {gem_v_line};
    GemVal _t271[] = {gem_add(gem_string("unterminated triple-quoted string interpolation at line "), gem_to_string_fn(NULL, _t270, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 680, _t271, 1));
                    }
#line 682 "compiler/main.gem"
    GemVal _t272 = gem_table_new();
    gem_table_set(_t272, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t272, gem_string("value"), gem_v_val);
    gem_table_set(_t272, gem_string("line"), gem_v_line);
    GemVal _t273[] = {gem_v_tokens, _t272};
                    (void)(gem_push_fn(NULL, _t273, 2));
#line 683 "compiler/main.gem"
    GemVal _t274 = gem_table_new();
    gem_table_set(_t274, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t274, gem_string("value"), gem_string(""));
    gem_table_set(_t274, gem_string("line"), gem_v_line);
    GemVal _t275[] = {gem_v_tokens, _t274};
                    (void)(gem_push_fn(NULL, _t275, 2));
                }
                continue;
            }
#line 689 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 690 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 691 "compiler/main.gem"
            while (1) {
                GemVal _t276;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t276 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t276 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t276)) break;
#line 692 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 693 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 694 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 695 "compiler/main.gem"
    GemVal _t277[] = {gem_v_line};
    GemVal _t278[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t277, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 695, _t278, 1));
                    }
#line 697 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 698 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 699 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
#line 700 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 701 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_CR);
                        } else {
#line 702 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 703 "compiler/main.gem"
                                gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                            } else {
#line 704 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 705 "compiler/main.gem"
                                    gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                                } else {
#line 706 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
#line 707 "compiler/main.gem"
                                        gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                    } else {
#line 708 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
#line 709 "compiler/main.gem"
                                            gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                        } else {
#line 710 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
#line 711 "compiler/main.gem"
                                                gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                            } else {
#line 712 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
#line 713 "compiler/main.gem"
                                                    gem_v_val = gem_add(gem_v_val, gem_v_NUL);
                                                } else {
#line 715 "compiler/main.gem"
                                                    gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
#line 717 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 720 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 722 "compiler/main.gem"
    GemVal _t279 = gem_table_new();
    gem_table_set(_t279, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t279, gem_string("value"), gem_v_val);
    gem_table_set(_t279, gem_string("line"), gem_v_line);
    GemVal _t280[] = {gem_v_tokens, _t279};
                    (void)(gem_push_fn(NULL, _t280, 2));
#line 723 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 724 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 725 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 726 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 727 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 730 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 731 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 733 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 734 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 735 "compiler/main.gem"
    GemVal _t281[] = {gem_v_line};
    GemVal _t282[] = {gem_add(gem_string("unterminated string interpolation at line "), gem_to_string_fn(NULL, _t281, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 735, _t282, 1));
                }
#line 737 "compiler/main.gem"
    GemVal _t283 = gem_table_new();
    gem_table_set(_t283, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t283, gem_string("value"), gem_v_val);
    gem_table_set(_t283, gem_string("line"), gem_v_line);
    GemVal _t284[] = {gem_v_tokens, _t283};
                (void)(gem_push_fn(NULL, _t284, 2));
#line 738 "compiler/main.gem"
    GemVal _t285 = gem_table_new();
    gem_table_set(_t285, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t285, gem_string("value"), gem_string(""));
    gem_table_set(_t285, gem_string("line"), gem_v_line);
    GemVal _t286[] = {gem_v_tokens, _t285};
                (void)(gem_push_fn(NULL, _t286, 2));
#line 739 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 744 "compiler/main.gem"
    GemVal _t287;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t287 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t287 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t288;
    if (gem_truthy(_t287)) {
        _t288 = _t287;
    } else {
        _t288 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t289;
    if (gem_truthy(_t288)) {
        _t289 = _t288;
    } else {
        _t289 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t290;
    if (gem_truthy(_t289)) {
        _t290 = _t289;
    } else {
        _t290 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t291;
    if (gem_truthy(_t290)) {
        _t291 = _t290;
    } else {
        _t291 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t291)) {
#line 745 "compiler/main.gem"
    GemVal _t292 = gem_table_new();
    gem_table_set(_t292, gem_string("type"), gem_v_ch);
    gem_table_set(_t292, gem_string("value"), gem_v_ch);
    gem_table_set(_t292, gem_string("line"), gem_v_line);
    GemVal _t293[] = {gem_v_tokens, _t292};
            (void)(gem_push_fn(NULL, _t293, 2));
#line 746 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 749 "compiler/main.gem"
    GemVal _t294;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t294 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t294 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    }
    GemVal _t295;
    if (!gem_truthy(_t294)) {
        _t295 = _t294;
    } else {
        _t295 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("."));
    }
    GemVal _t296;
    if (!gem_truthy(_t295)) {
        _t296 = _t295;
    } else {
        _t296 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    }
    GemVal _t297;
    if (!gem_truthy(_t296)) {
        _t297 = _t296;
    } else {
        _t297 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("."));
    }
        if (gem_truthy(_t297)) {
#line 750 "compiler/main.gem"
    GemVal _t298 = gem_table_new();
    gem_table_set(_t298, gem_string("type"), gem_string("..."));
    gem_table_set(_t298, gem_string("value"), gem_string("..."));
    gem_table_set(_t298, gem_string("line"), gem_v_line);
    GemVal _t299[] = {gem_v_tokens, _t298};
            (void)(gem_push_fn(NULL, _t299, 2));
#line 751 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
            continue;
        }
#line 754 "compiler/main.gem"
    GemVal _t300;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t300 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t300 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t301;
    if (gem_truthy(_t300)) {
        _t301 = _t300;
    } else {
        _t301 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t302;
    if (gem_truthy(_t301)) {
        _t302 = _t301;
    } else {
        _t302 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t302)) {
#line 755 "compiler/main.gem"
    GemVal _t303 = gem_table_new();
    gem_table_set(_t303, gem_string("type"), gem_v_ch);
    gem_table_set(_t303, gem_string("value"), gem_v_ch);
    gem_table_set(_t303, gem_string("line"), gem_v_line);
    GemVal _t304[] = {gem_v_tokens, _t303};
            (void)(gem_push_fn(NULL, _t304, 2));
#line 756 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 760 "compiler/main.gem"
    GemVal _t305[] = {gem_v_ch};
    GemVal _t306[] = {gem_v_line};
    GemVal _t307[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t305, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t306, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 760, _t307, 1));
    }
#line 763 "compiler/main.gem"
    GemVal _t308 = gem_table_new();
    gem_table_set(_t308, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t308, gem_string("value"), gem_string(""));
    gem_table_set(_t308, gem_string("line"), gem_v_line);
    GemVal _t309[] = {gem_v_tokens, _t308};
    (void)(gem_push_fn(NULL, _t309, 2));
    GemVal _t310 = gem_v_tokens;
    gem_pop_frame();
    return _t310;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 10 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_source", "compiler/main.gem", 10);
#line 11 "compiler/main.gem"
    GemVal _t311[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t311, 1);
#line 12 "compiler/main.gem"
    GemVal _t312[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t312, 1);
#line 13 "compiler/main.gem"
    GemVal _t313 = gem_v_p;
    GemVal _t314 = gem_table_get(_t313, gem_string("parse"));
    GemVal _t315 = _t314.fn(_t314.env, NULL, 0);
    gem_pop_frame();
    return _t315;
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
    GemVal _t316 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t316;
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
    GemVal _t318 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t318;
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
    GemVal _t320 = gem_v_t;
    gem_pop_frame();
    return _t320;
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
    GemVal _t322 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t323 = gem_eq(gem_table_get(_t322, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t323;
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
    GemVal _t325 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t325, gem_string("type")), gem_v_tp))) {
#line 46 "compiler/main.gem"
    GemVal _t326[] = {gem_v_tp};
    GemVal _t327 = gem_v_t;
    GemVal _t328[] = {gem_table_get(_t327, gem_string("type"))};
    GemVal _t329 = gem_v_t;
    GemVal _t330[] = {gem_table_get(_t329, gem_string("line"))};
    GemVal _t331[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t326, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t328, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t330, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 46, _t331, 1));
    }
#line 48 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t332 = gem_v_t;
    gem_pop_frame();
    return _t332;
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
        GemVal _t334 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t334, gem_string("type")), gem_string("NEWLINE")))) break;
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
#line 65 "compiler/main.gem"
    GemVal _t336 = gem_table_new();
    GemVal gem_v_params = _t336;
#line 66 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 67 "compiler/main.gem"
    GemVal gem_v_block_param = GEM_NIL;
#line 69 "compiler/main.gem"
    while (1) {
        GemVal _t337 = (*gem_v_peek);
        GemVal _t338 = _t337.fn(_t337.env, NULL, 0);
        GemVal _t341;
        if (gem_truthy(gem_eq(gem_table_get(_t338, gem_string("type")), gem_string("NAME")))) {
                _t341 = gem_eq(gem_table_get(_t338, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t339 = (*gem_v_peek);
                GemVal _t340 = _t339.fn(_t339.env, NULL, 0);
                _t341 = gem_eq(gem_table_get(_t340, gem_string("type")), gem_string("..."));
        }
        if (!gem_truthy(_t341)) break;
#line 70 "compiler/main.gem"
    GemVal _t342 = (*gem_v_peek);
    GemVal _t343 = _t342.fn(_t342.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t343, gem_string("type")), gem_string("...")))) {
#line 71 "compiler/main.gem"
    GemVal _t344 = (*gem_v_advance);
            (void)(_t344.fn(_t344.env, NULL, 0));
#line 72 "compiler/main.gem"
    GemVal _t345[] = {gem_string("NAME")};
    GemVal _t346 = (*gem_v_expect);
    GemVal _t347 = _t346.fn(_t346.env, _t345, 1);
            gem_v_rest_param = gem_table_get(_t347, gem_string("value"));
#line 73 "compiler/main.gem"
    GemVal _t348 = (*gem_v_peek);
    GemVal _t349 = _t348.fn(_t348.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t349, gem_string("type")), gem_string(",")))) {
#line 74 "compiler/main.gem"
    GemVal _t350 = (*gem_v_advance);
                (void)(_t350.fn(_t350.env, NULL, 0));
#line 75 "compiler/main.gem"
    GemVal _t351 = (*gem_v_peek);
    GemVal _t352 = _t351.fn(_t351.env, NULL, 0);
                if (gem_truthy(gem_neq(gem_table_get(_t352, gem_string("type")), gem_string("NAME")))) {
#line 76 "compiler/main.gem"
    GemVal _t353 = (*gem_v_peek);
    GemVal _t354 = _t353.fn(_t353.env, NULL, 0);
    GemVal _t355[] = {gem_table_get(_t354, gem_string("line"))};
    GemVal _t356[] = {gem_add(gem_string("expected parameter name after rest param at line "), gem_to_string_fn(NULL, _t355, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 76, _t356, 1));
                }
#line 78 "compiler/main.gem"
    GemVal _t357 = (*gem_v_advance);
    GemVal _t358 = _t357.fn(_t357.env, NULL, 0);
                gem_v_block_param = gem_table_get(_t358, gem_string("value"));
#line 79 "compiler/main.gem"
    GemVal _t359 = (*gem_v_peek);
    GemVal _t360 = _t359.fn(_t359.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t360, gem_string("type")), gem_string(",")))) {
#line 80 "compiler/main.gem"
    GemVal _t361 = (*gem_v_peek);
    GemVal _t362 = _t361.fn(_t361.env, NULL, 0);
    GemVal _t363[] = {gem_table_get(_t362, gem_string("line"))};
    GemVal _t364[] = {gem_add(gem_string("only one parameter is allowed after a rest param at line "), gem_to_string_fn(NULL, _t363, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 80, _t364, 1));
                }
            }
            break;
        }
#line 85 "compiler/main.gem"
    GemVal _t365 = (*gem_v_advance);
    GemVal _t366 = _t365.fn(_t365.env, NULL, 0);
    GemVal _t367[] = {gem_v_params, gem_table_get(_t366, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t367, 2));
#line 86 "compiler/main.gem"
    GemVal _t368 = (*gem_v_peek);
    GemVal _t369 = _t368.fn(_t368.env, NULL, 0);
        if (gem_truthy(gem_neq(gem_table_get(_t369, gem_string("type")), gem_string(",")))) {
            break;
        }
#line 89 "compiler/main.gem"
    GemVal _t370 = (*gem_v_advance);
        (void)(_t370.fn(_t370.env, NULL, 0));
    }
    GemVal _t371 = gem_table_new();
    gem_table_set(_t371, gem_string("params"), gem_v_params);
    gem_table_set(_t371, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t371, gem_string("block_param"), gem_v_block_param);
    GemVal _t372 = _t371;
    gem_pop_frame();
    return _t372;
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
#line 107 "compiler/main.gem"
    GemVal _t374 = gem_table_new();
    GemVal gem_v_stmts = _t374;
#line 108 "compiler/main.gem"
    GemVal _t375 = (*gem_v_skip_nl);
    (void)(_t375.fn(_t375.env, NULL, 0));
#line 109 "compiler/main.gem"
    while (1) {
        GemVal _t376 = (*gem_v_peek);
        GemVal _t377 = _t376.fn(_t376.env, NULL, 0);
        GemVal _t380;
        if (!gem_truthy(gem_neq(gem_table_get(_t377, gem_string("type")), gem_string("end")))) {
                _t380 = gem_neq(gem_table_get(_t377, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t378 = (*gem_v_peek);
                GemVal _t379 = _t378.fn(_t378.env, NULL, 0);
                _t380 = gem_neq(gem_table_get(_t379, gem_string("type")), gem_string("elif"));
        }
        GemVal _t383;
        if (!gem_truthy(_t380)) {
                _t383 = _t380;
        } else {
                GemVal _t381 = (*gem_v_peek);
                GemVal _t382 = _t381.fn(_t381.env, NULL, 0);
                _t383 = gem_neq(gem_table_get(_t382, gem_string("type")), gem_string("else"));
        }
        GemVal _t386;
        if (!gem_truthy(_t383)) {
                _t386 = _t383;
        } else {
                GemVal _t384 = (*gem_v_peek);
                GemVal _t385 = _t384.fn(_t384.env, NULL, 0);
                _t386 = gem_neq(gem_table_get(_t385, gem_string("type")), gem_string("when"));
        }
        GemVal _t388;
        if (!gem_truthy(_t386)) {
                _t388 = _t386;
        } else {
                GemVal _t387 = (*gem_v_at_end);
                _t388 = gem_not(_t387.fn(_t387.env, NULL, 0));
        }
        if (!gem_truthy(_t388)) break;
#line 110 "compiler/main.gem"
    GemVal _t389 = (*gem_v_parse_stmt);
    GemVal _t390[] = {gem_v_stmts, _t389.fn(_t389.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t390, 2));
#line 111 "compiler/main.gem"
    GemVal _t391 = (*gem_v_skip_nl);
        (void)(_t391.fn(_t391.env, NULL, 0));
    }
    GemVal _t392 = gem_v_stmts;
    gem_pop_frame();
    return _t392;
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
#line 120 "compiler/main.gem"
    GemVal _t394 = gem_table_new();
    GemVal gem_v_stmts = _t394;
#line 121 "compiler/main.gem"
    GemVal _t395 = (*gem_v_skip_nl);
    (void)(_t395.fn(_t395.env, NULL, 0));
#line 122 "compiler/main.gem"
    while (1) {
        GemVal _t396 = (*gem_v_peek);
        GemVal _t397 = _t396.fn(_t396.env, NULL, 0);
        GemVal _t399;
        if (!gem_truthy(gem_neq(gem_table_get(_t397, gem_string("type")), gem_string("end")))) {
                _t399 = gem_neq(gem_table_get(_t397, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t398 = (*gem_v_at_end);
                _t399 = gem_not(_t398.fn(_t398.env, NULL, 0));
        }
        if (!gem_truthy(_t399)) break;
#line 123 "compiler/main.gem"
    GemVal _t400 = (*gem_v_parse_stmt);
    GemVal _t401[] = {gem_v_stmts, _t400.fn(_t400.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t401, 2));
#line 124 "compiler/main.gem"
    GemVal _t402 = (*gem_v_skip_nl);
        (void)(_t402.fn(_t402.env, NULL, 0));
    }
    GemVal _t403 = gem_v_stmts;
    gem_pop_frame();
    return _t403;
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
#line 133 "compiler/main.gem"
    GemVal _t405 = (*gem_v_peek);
    GemVal gem_v_t = _t405.fn(_t405.env, NULL, 0);
#line 136 "compiler/main.gem"
    GemVal _t406 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t406, gem_string("type")), gem_string("NUMBER")))) {
#line 137 "compiler/main.gem"
    GemVal _t407 = (*gem_v_advance);
        (void)(_t407.fn(_t407.env, NULL, 0));
#line 138 "compiler/main.gem"
    GemVal _t408 = gem_v_t;
    GemVal _t409[] = {gem_table_get(_t408, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t409, 1))) {
#line 139 "compiler/main.gem"
    GemVal _t410 = gem_v_t;
    GemVal _t411[] = {gem_table_get(_t410, gem_string("value"))};
    GemVal _t412[] = {gem_fn_atof(NULL, _t411, 1)};
            GemVal _t413 = gem_fn_make_float(NULL, _t412, 1);
            gem_pop_frame();
            return _t413;
        }
#line 141 "compiler/main.gem"
    GemVal _t414 = gem_v_t;
    GemVal _t415[] = {gem_table_get(_t414, gem_string("value"))};
    GemVal _t416[] = {gem_fn_str_to_int(NULL, _t415, 1)};
        GemVal _t417 = gem_fn_make_int(NULL, _t416, 1);
        gem_pop_frame();
        return _t417;
    }
#line 145 "compiler/main.gem"
    GemVal _t418 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t418, gem_string("type")), gem_string("STRING")))) {
#line 146 "compiler/main.gem"
    GemVal _t419 = (*gem_v_advance);
        (void)(_t419.fn(_t419.env, NULL, 0));
#line 147 "compiler/main.gem"
    GemVal _t420 = gem_v_t;
    GemVal _t421[] = {gem_table_get(_t420, gem_string("value"))};
        GemVal _t422 = gem_fn_make_string(NULL, _t421, 1);
        gem_pop_frame();
        return _t422;
    }
#line 151 "compiler/main.gem"
    GemVal _t423 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t423, gem_string("type")), gem_string("INTERP_START")))) {
#line 152 "compiler/main.gem"
    GemVal _t424 = (*gem_v_advance);
        (void)(_t424.fn(_t424.env, NULL, 0));
#line 153 "compiler/main.gem"
    GemVal _t425 = gem_table_new();
        GemVal gem_v_parts = _t425;
#line 154 "compiler/main.gem"
    GemVal _t426 = (*gem_v_skip_nl);
        (void)(_t426.fn(_t426.env, NULL, 0));
#line 155 "compiler/main.gem"
        while (1) {
            GemVal _t427 = (*gem_v_peek);
            GemVal _t428 = _t427.fn(_t427.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t428, gem_string("type")), gem_string("INTERP_END")))) break;
#line 156 "compiler/main.gem"
    GemVal _t429 = (*gem_v_peek);
    GemVal _t430 = _t429.fn(_t429.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t430, gem_string("type")), gem_string("STRING")))) {
#line 157 "compiler/main.gem"
    GemVal _t431 = (*gem_v_advance);
    GemVal _t432 = _t431.fn(_t431.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t432, gem_string("value"));
#line 158 "compiler/main.gem"
    GemVal _t433[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t433, 1), gem_int(0)))) {
#line 159 "compiler/main.gem"
    GemVal _t434[] = {gem_v_sval};
    GemVal _t435[] = {gem_v_parts, gem_fn_make_string(NULL, _t434, 1)};
                    (void)(gem_push_fn(NULL, _t435, 2));
                }
            } else {
#line 162 "compiler/main.gem"
    GemVal _t436 = (*gem_v_parse_expr);
    GemVal _t437[] = {gem_v_parts, _t436.fn(_t436.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t437, 2));
            }
#line 164 "compiler/main.gem"
    GemVal _t438 = (*gem_v_skip_nl);
            (void)(_t438.fn(_t438.env, NULL, 0));
        }
#line 166 "compiler/main.gem"
    GemVal _t439[] = {gem_string("INTERP_END")};
    GemVal _t440 = (*gem_v_expect);
        (void)(_t440.fn(_t440.env, _t439, 1));
#line 167 "compiler/main.gem"
    GemVal _t441[] = {gem_v_parts};
        GemVal _t442 = gem_fn_make_interp(NULL, _t441, 1);
        gem_pop_frame();
        return _t442;
    }
#line 171 "compiler/main.gem"
    GemVal _t443 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t443, gem_string("type")), gem_string("true")))) {
#line 172 "compiler/main.gem"
    GemVal _t444 = (*gem_v_advance);
        (void)(_t444.fn(_t444.env, NULL, 0));
#line 173 "compiler/main.gem"
    GemVal _t445[] = {gem_bool(1)};
        GemVal _t446 = gem_fn_make_bool(NULL, _t445, 1);
        gem_pop_frame();
        return _t446;
    }
#line 175 "compiler/main.gem"
    GemVal _t447 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t447, gem_string("type")), gem_string("false")))) {
#line 176 "compiler/main.gem"
    GemVal _t448 = (*gem_v_advance);
        (void)(_t448.fn(_t448.env, NULL, 0));
#line 177 "compiler/main.gem"
    GemVal _t449[] = {gem_bool(0)};
        GemVal _t450 = gem_fn_make_bool(NULL, _t449, 1);
        gem_pop_frame();
        return _t450;
    }
#line 181 "compiler/main.gem"
    GemVal _t451 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t451, gem_string("type")), gem_string("nil")))) {
#line 182 "compiler/main.gem"
    GemVal _t452 = (*gem_v_advance);
        (void)(_t452.fn(_t452.env, NULL, 0));
#line 183 "compiler/main.gem"
        GemVal _t453 = gem_fn_make_nil_node(NULL, NULL, 0);
        gem_pop_frame();
        return _t453;
    }
#line 187 "compiler/main.gem"
    GemVal _t454 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t454, gem_string("type")), gem_string("fn")))) {
#line 188 "compiler/main.gem"
    GemVal _t455 = (*gem_v_advance);
        (void)(_t455.fn(_t455.env, NULL, 0));
#line 189 "compiler/main.gem"
    GemVal _t456[] = {gem_string("(")};
    GemVal _t457 = (*gem_v_expect);
        (void)(_t457.fn(_t457.env, _t456, 1));
#line 190 "compiler/main.gem"
    GemVal _t458 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t458.fn(_t458.env, NULL, 0);
#line 191 "compiler/main.gem"
    GemVal _t459[] = {gem_string(")")};
    GemVal _t460 = (*gem_v_expect);
        (void)(_t460.fn(_t460.env, _t459, 1));
#line 192 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 193 "compiler/main.gem"
    GemVal _t461 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t461.fn(_t461.env, NULL, 0);
#line 194 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 195 "compiler/main.gem"
    GemVal _t462[] = {gem_string("end")};
    GemVal _t463 = (*gem_v_expect);
        (void)(_t463.fn(_t463.env, _t462, 1));
#line 196 "compiler/main.gem"
    GemVal _t464 = gem_v_pr;
    GemVal _t465 = gem_v_pr;
    GemVal _t466 = gem_v_pr;
    GemVal _t467[] = {gem_table_get(_t464, gem_string("params")), gem_table_get(_t465, gem_string("rest_param")), gem_table_get(_t466, gem_string("block_param")), gem_v_body};
        GemVal _t468 = gem_fn_make_anon_fn(NULL, _t467, 4);
        gem_pop_frame();
        return _t468;
    }
#line 201 "compiler/main.gem"
    GemVal _t469 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t469, gem_string("type")), gem_string("{")))) {
#line 202 "compiler/main.gem"
    GemVal _t470 = (*gem_v_advance);
        (void)(_t470.fn(_t470.env, NULL, 0));
#line 203 "compiler/main.gem"
    GemVal _t471 = (*gem_v_skip_nl);
        (void)(_t471.fn(_t471.env, NULL, 0));
#line 204 "compiler/main.gem"
    GemVal _t472 = gem_table_new();
        GemVal gem_v_entries = _t472;
#line 205 "compiler/main.gem"
        while (1) {
            GemVal _t473 = (*gem_v_peek);
            GemVal _t474 = _t473.fn(_t473.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t474, gem_string("type")), gem_string("}")))) break;
#line 206 "compiler/main.gem"
    GemVal _t475 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t475.fn(_t475.env, NULL, 0);
#line 207 "compiler/main.gem"
    GemVal _t476 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t476, gem_string("type")), gem_string("NAME")))) {
#line 208 "compiler/main.gem"
    GemVal _t477 = (*gem_v_advance);
                (void)(_t477.fn(_t477.env, NULL, 0));
            } else {
#line 209 "compiler/main.gem"
    GemVal _t478[] = {gem_int(1)};
    GemVal _t479 = (*gem_v_peek_at);
    GemVal _t480 = _t479.fn(_t479.env, _t478, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t480, gem_string("type")), gem_string(":")))) {
#line 211 "compiler/main.gem"
    GemVal _t481 = (*gem_v_advance);
                    (void)(_t481.fn(_t481.env, NULL, 0));
                } else {
#line 213 "compiler/main.gem"
    GemVal _t482 = gem_v_key_tok;
    GemVal _t483[] = {gem_table_get(_t482, gem_string("line"))};
    GemVal _t484[] = {gem_add(gem_string("expected table key at line "), gem_to_string_fn(NULL, _t483, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 213, _t484, 1));
                }
            }
#line 215 "compiler/main.gem"
    GemVal _t485[] = {gem_string(":")};
    GemVal _t486 = (*gem_v_expect);
            (void)(_t486.fn(_t486.env, _t485, 1));
#line 216 "compiler/main.gem"
    GemVal _t487 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t487.fn(_t487.env, NULL, 0);
#line 217 "compiler/main.gem"
    GemVal _t488 = gem_v_key_tok;
    GemVal _t489[] = {gem_table_get(_t488, gem_string("value")), gem_v_val};
    GemVal _t490[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t489, 2)};
            (void)(gem_push_fn(NULL, _t490, 2));
#line 218 "compiler/main.gem"
    GemVal _t491 = (*gem_v_skip_nl);
            (void)(_t491.fn(_t491.env, NULL, 0));
#line 219 "compiler/main.gem"
    GemVal _t492 = (*gem_v_peek);
    GemVal _t493 = _t492.fn(_t492.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t493, gem_string("type")), gem_string(",")))) {
#line 220 "compiler/main.gem"
    GemVal _t494 = (*gem_v_advance);
                (void)(_t494.fn(_t494.env, NULL, 0));
#line 221 "compiler/main.gem"
    GemVal _t495 = (*gem_v_skip_nl);
                (void)(_t495.fn(_t495.env, NULL, 0));
            }
        }
#line 224 "compiler/main.gem"
    GemVal _t496[] = {gem_string("}")};
    GemVal _t497 = (*gem_v_expect);
        (void)(_t497.fn(_t497.env, _t496, 1));
#line 225 "compiler/main.gem"
    GemVal _t498[] = {gem_v_entries};
        GemVal _t499 = gem_fn_make_table(NULL, _t498, 1);
        gem_pop_frame();
        return _t499;
    }
#line 229 "compiler/main.gem"
    GemVal _t500 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t500, gem_string("type")), gem_string("[")))) {
#line 230 "compiler/main.gem"
    GemVal _t501 = (*gem_v_advance);
        (void)(_t501.fn(_t501.env, NULL, 0));
#line 231 "compiler/main.gem"
    GemVal _t502 = (*gem_v_skip_nl);
        (void)(_t502.fn(_t502.env, NULL, 0));
#line 232 "compiler/main.gem"
    GemVal _t503 = gem_table_new();
        GemVal gem_v_elements = _t503;
#line 233 "compiler/main.gem"
        while (1) {
            GemVal _t504 = (*gem_v_peek);
            GemVal _t505 = _t504.fn(_t504.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t505, gem_string("type")), gem_string("]")))) break;
#line 234 "compiler/main.gem"
    GemVal _t506 = (*gem_v_parse_expr);
    GemVal _t507[] = {gem_v_elements, _t506.fn(_t506.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t507, 2));
#line 235 "compiler/main.gem"
    GemVal _t508 = (*gem_v_skip_nl);
            (void)(_t508.fn(_t508.env, NULL, 0));
#line 236 "compiler/main.gem"
    GemVal _t509 = (*gem_v_peek);
    GemVal _t510 = _t509.fn(_t509.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t510, gem_string("type")), gem_string(",")))) {
#line 237 "compiler/main.gem"
    GemVal _t511 = (*gem_v_advance);
                (void)(_t511.fn(_t511.env, NULL, 0));
#line 238 "compiler/main.gem"
    GemVal _t512 = (*gem_v_skip_nl);
                (void)(_t512.fn(_t512.env, NULL, 0));
            }
        }
#line 241 "compiler/main.gem"
    GemVal _t513[] = {gem_string("]")};
    GemVal _t514 = (*gem_v_expect);
        (void)(_t514.fn(_t514.env, _t513, 1));
#line 242 "compiler/main.gem"
    GemVal _t515[] = {gem_v_elements};
        GemVal _t516 = gem_fn_make_array(NULL, _t515, 1);
        gem_pop_frame();
        return _t516;
    }
#line 246 "compiler/main.gem"
    GemVal _t517 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t517, gem_string("type")), gem_string("(")))) {
#line 247 "compiler/main.gem"
    GemVal _t518 = (*gem_v_advance);
        (void)(_t518.fn(_t518.env, NULL, 0));
#line 248 "compiler/main.gem"
    GemVal _t519 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t519.fn(_t519.env, NULL, 0);
#line 249 "compiler/main.gem"
    GemVal _t520[] = {gem_string(")")};
    GemVal _t521 = (*gem_v_expect);
        (void)(_t521.fn(_t521.env, _t520, 1));
#line 250 "compiler/main.gem"
        GemVal _t522 = gem_v_e;
        gem_pop_frame();
        return _t522;
    }
#line 254 "compiler/main.gem"
    GemVal _t523 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t523, gem_string("type")), gem_string("NAME")))) {
#line 255 "compiler/main.gem"
    GemVal _t524 = (*gem_v_advance);
        (void)(_t524.fn(_t524.env, NULL, 0));
#line 256 "compiler/main.gem"
    GemVal _t525 = gem_v_t;
    GemVal _t526[] = {gem_table_get(_t525, gem_string("value"))};
        GemVal _t527 = gem_fn_make_var(NULL, _t526, 1);
        gem_pop_frame();
        return _t527;
    }
#line 259 "compiler/main.gem"
    GemVal _t528 = gem_v_t;
    GemVal _t529[] = {gem_table_get(_t528, gem_string("type"))};
    GemVal _t530 = gem_v_t;
    GemVal _t531[] = {gem_table_get(_t530, gem_string("line"))};
    GemVal _t532[] = {gem_add(gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t529, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t531, 1))};
    GemVal _t533 = gem_error_at_fn("compiler/main.gem", 259, _t532, 1);
    gem_pop_frame();
    return _t533;
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
#line 264 "compiler/main.gem"
    GemVal _t535 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t535.fn(_t535.env, NULL, 0);
#line 266 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
#line 268 "compiler/main.gem"
    GemVal _t536 = (*gem_v_peek);
    GemVal _t537 = _t536.fn(_t536.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t537, gem_string("type")), gem_string("(")))) {
#line 269 "compiler/main.gem"
    GemVal _t538 = (*gem_v_peek);
    GemVal _t539 = _t538.fn(_t538.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t539, gem_string("line"));
#line 270 "compiler/main.gem"
    GemVal _t540 = (*gem_v_advance);
            (void)(_t540.fn(_t540.env, NULL, 0));
#line 271 "compiler/main.gem"
    GemVal _t541 = (*gem_v_skip_nl);
            (void)(_t541.fn(_t541.env, NULL, 0));
#line 272 "compiler/main.gem"
    GemVal _t542 = gem_table_new();
            GemVal gem_v_args = _t542;
#line 273 "compiler/main.gem"
    GemVal _t543 = (*gem_v_peek);
    GemVal _t544 = _t543.fn(_t543.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t544, gem_string("type")), gem_string(")")))) {
#line 274 "compiler/main.gem"
    GemVal _t545 = (*gem_v_parse_expr);
    GemVal _t546[] = {gem_v_args, _t545.fn(_t545.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t546, 2));
#line 275 "compiler/main.gem"
                while (1) {
                    GemVal _t547 = (*gem_v_peek);
                    GemVal _t548 = _t547.fn(_t547.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t548, gem_string("type")), gem_string(",")))) break;
#line 276 "compiler/main.gem"
    GemVal _t549 = (*gem_v_advance);
                    (void)(_t549.fn(_t549.env, NULL, 0));
#line 277 "compiler/main.gem"
    GemVal _t550 = (*gem_v_skip_nl);
                    (void)(_t550.fn(_t550.env, NULL, 0));
#line 278 "compiler/main.gem"
    GemVal _t551 = (*gem_v_parse_expr);
    GemVal _t552[] = {gem_v_args, _t551.fn(_t551.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t552, 2));
                }
            }
#line 281 "compiler/main.gem"
    GemVal _t553 = (*gem_v_skip_nl);
            (void)(_t553.fn(_t553.env, NULL, 0));
#line 282 "compiler/main.gem"
    GemVal _t554[] = {gem_string(")")};
    GemVal _t555 = (*gem_v_expect);
            (void)(_t555.fn(_t555.env, _t554, 1));
#line 285 "compiler/main.gem"
    GemVal _t556 = (*gem_v_peek);
    GemVal _t557 = _t556.fn(_t556.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t557, gem_string("type")), gem_string("do")))) {
#line 286 "compiler/main.gem"
    GemVal _t558 = (*gem_v_advance);
                (void)(_t558.fn(_t558.env, NULL, 0));
#line 287 "compiler/main.gem"
    GemVal _t559 = gem_table_new();
                GemVal gem_v_bparams = _t559;
#line 288 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 289 "compiler/main.gem"
                GemVal gem_v_bblock = GEM_NIL;
#line 290 "compiler/main.gem"
    GemVal _t560 = (*gem_v_peek);
    GemVal _t561 = _t560.fn(_t560.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t561, gem_string("type")), gem_string("|")))) {
#line 291 "compiler/main.gem"
    GemVal _t562 = (*gem_v_advance);
                    (void)(_t562.fn(_t562.env, NULL, 0));
#line 292 "compiler/main.gem"
    GemVal _t563 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t563.fn(_t563.env, NULL, 0);
#line 293 "compiler/main.gem"
    GemVal _t564 = gem_v_bpr;
                    gem_v_bparams = gem_table_get(_t564, gem_string("params"));
#line 294 "compiler/main.gem"
    GemVal _t565 = gem_v_bpr;
                    gem_v_brest = gem_table_get(_t565, gem_string("rest_param"));
#line 295 "compiler/main.gem"
    GemVal _t566 = gem_v_bpr;
                    gem_v_bblock = gem_table_get(_t566, gem_string("block_param"));
#line 296 "compiler/main.gem"
    GemVal _t567[] = {gem_string("|")};
    GemVal _t568 = (*gem_v_expect);
                    (void)(_t568.fn(_t568.env, _t567, 1));
                }
#line 298 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 299 "compiler/main.gem"
    GemVal _t569 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t569.fn(_t569.env, NULL, 0);
#line 300 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 301 "compiler/main.gem"
    GemVal _t570[] = {gem_string("end")};
    GemVal _t571 = (*gem_v_expect);
                (void)(_t571.fn(_t571.env, _t570, 1));
#line 302 "compiler/main.gem"
    GemVal _t572[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, gem_v_body};
    GemVal _t573[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t572, 4)};
                (void)(gem_push_fn(NULL, _t573, 2));
            } else {
#line 306 "compiler/main.gem"
    GemVal _t574 = (*gem_v_peek);
    GemVal _t575 = _t574.fn(_t574.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t575, gem_string("type")), gem_string("{")))) {
#line 307 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 308 "compiler/main.gem"
    GemVal _t576[] = {gem_int(1)};
    GemVal _t577 = (*gem_v_peek_at);
    GemVal _t578 = _t577.fn(_t577.env, _t576, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t578, gem_string("type")), gem_string("|")))) {
#line 309 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 310 "compiler/main.gem"
    GemVal _t579[] = {gem_int(1)};
    GemVal _t580 = (*gem_v_peek_at);
    GemVal _t581 = _t580.fn(_t580.env, _t579, 1);
    GemVal _t589;
    if (gem_truthy(gem_eq(gem_table_get(_t581, gem_string("type")), gem_string("}")))) {
        _t589 = gem_eq(gem_table_get(_t581, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t582[] = {gem_int(1)};
        GemVal _t583 = (*gem_v_peek_at);
        GemVal _t584 = _t583.fn(_t583.env, _t582, 1);
        GemVal _t588;
        if (!gem_truthy(gem_eq(gem_table_get(_t584, gem_string("type")), gem_string("NAME")))) {
                _t588 = gem_eq(gem_table_get(_t584, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t585[] = {gem_int(2)};
                GemVal _t586 = (*gem_v_peek_at);
                GemVal _t587 = _t586.fn(_t586.env, _t585, 1);
                _t588 = gem_eq(gem_table_get(_t587, gem_string("type")), gem_string(":"));
        }
        _t589 = _t588;
    }
                        if (gem_truthy(_t589)) {
#line 311 "compiler/main.gem"
                            gem_v_is_block = gem_bool(0);
                        } else {
#line 313 "compiler/main.gem"
                            gem_v_is_block = gem_bool(1);
                        }
                    }
#line 315 "compiler/main.gem"
                    if (gem_truthy(gem_v_is_block)) {
#line 316 "compiler/main.gem"
    GemVal _t590 = (*gem_v_advance);
                        (void)(_t590.fn(_t590.env, NULL, 0));
#line 317 "compiler/main.gem"
    GemVal _t591 = gem_table_new();
                        GemVal gem_v_bparams = _t591;
#line 318 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 319 "compiler/main.gem"
                        GemVal gem_v_bblock = GEM_NIL;
#line 320 "compiler/main.gem"
    GemVal _t592 = (*gem_v_peek);
    GemVal _t593 = _t592.fn(_t592.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t593, gem_string("type")), gem_string("|")))) {
#line 321 "compiler/main.gem"
    GemVal _t594 = (*gem_v_advance);
                            (void)(_t594.fn(_t594.env, NULL, 0));
#line 322 "compiler/main.gem"
    GemVal _t595 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t595.fn(_t595.env, NULL, 0);
#line 323 "compiler/main.gem"
    GemVal _t596 = gem_v_bpr;
                            gem_v_bparams = gem_table_get(_t596, gem_string("params"));
#line 324 "compiler/main.gem"
    GemVal _t597 = gem_v_bpr;
                            gem_v_brest = gem_table_get(_t597, gem_string("rest_param"));
#line 325 "compiler/main.gem"
    GemVal _t598 = gem_v_bpr;
                            gem_v_bblock = gem_table_get(_t598, gem_string("block_param"));
#line 326 "compiler/main.gem"
    GemVal _t599[] = {gem_string("|")};
    GemVal _t600 = (*gem_v_expect);
                            (void)(_t600.fn(_t600.env, _t599, 1));
                        }
#line 328 "compiler/main.gem"
    GemVal _t601 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t601.fn(_t601.env, NULL, 0);
#line 329 "compiler/main.gem"
    GemVal _t602[] = {gem_string("}")};
    GemVal _t603 = (*gem_v_expect);
                        (void)(_t603.fn(_t603.env, _t602, 1));
#line 330 "compiler/main.gem"
    GemVal _t604 = gem_table_new();
    gem_table_set(_t604, gem_int(0), gem_v_bexpr);
    GemVal _t605[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, _t604};
    GemVal _t606[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t605, 4)};
                        (void)(gem_push_fn(NULL, _t606, 2));
                    }
                }
            }
#line 335 "compiler/main.gem"
    GemVal _t607[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t607, 3);
            continue;
        }
#line 340 "compiler/main.gem"
    GemVal _t608 = (*gem_v_peek);
    GemVal _t609 = _t608.fn(_t608.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t609, gem_string("type")), gem_string(".")))) {
#line 341 "compiler/main.gem"
    GemVal _t610 = (*gem_v_advance);
            (void)(_t610.fn(_t610.env, NULL, 0));
#line 342 "compiler/main.gem"
    GemVal _t611 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t611.fn(_t611.env, NULL, 0);
#line 343 "compiler/main.gem"
    GemVal _t612 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t612, gem_string("type")), gem_string("NAME")))) {
#line 344 "compiler/main.gem"
    GemVal _t613 = (*gem_v_advance);
                (void)(_t613.fn(_t613.env, NULL, 0));
            } else {
#line 347 "compiler/main.gem"
    GemVal _t614 = (*gem_v_advance);
                (void)(_t614.fn(_t614.env, NULL, 0));
            }
#line 349 "compiler/main.gem"
    GemVal _t615 = gem_v_field_tok;
    GemVal _t616[] = {gem_v_node, gem_table_get(_t615, gem_string("value"))};
            gem_v_node = gem_fn_make_dot(NULL, _t616, 2);
            continue;
        }
#line 354 "compiler/main.gem"
    GemVal _t617 = (*gem_v_peek);
    GemVal _t618 = _t617.fn(_t617.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t618, gem_string("type")), gem_string("[")))) {
#line 355 "compiler/main.gem"
    GemVal _t619 = (*gem_v_advance);
            (void)(_t619.fn(_t619.env, NULL, 0));
#line 356 "compiler/main.gem"
    GemVal _t620 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t620.fn(_t620.env, NULL, 0);
#line 357 "compiler/main.gem"
    GemVal _t621[] = {gem_string("]")};
    GemVal _t622 = (*gem_v_expect);
            (void)(_t622.fn(_t622.env, _t621, 1));
#line 358 "compiler/main.gem"
    GemVal _t623[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t623, 2);
            continue;
        }
        break;
    }
    GemVal _t624 = gem_v_node;
    gem_pop_frame();
    return _t624;
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
#line 370 "compiler/main.gem"
    GemVal _t626 = (*gem_v_peek);
    GemVal _t627 = _t626.fn(_t626.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t627, gem_string("type")), gem_string("-")))) {
#line 371 "compiler/main.gem"
    GemVal _t628 = (*gem_v_advance);
        (void)(_t628.fn(_t628.env, NULL, 0));
#line 372 "compiler/main.gem"
    GemVal _t629 = (*gem_v_parse_unary);
    GemVal _t630[] = {gem_string("-"), _t629.fn(_t629.env, NULL, 0)};
        GemVal _t631 = gem_fn_make_unop(NULL, _t630, 2);
        gem_pop_frame();
        return _t631;
    }
#line 374 "compiler/main.gem"
    GemVal _t632 = (*gem_v_parse_call);
    GemVal _t633 = _t632.fn(_t632.env, NULL, 0);
    gem_pop_frame();
    return _t633;
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
#line 379 "compiler/main.gem"
    GemVal _t635 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t635.fn(_t635.env, NULL, 0);
#line 380 "compiler/main.gem"
    while (1) {
        GemVal _t636 = (*gem_v_peek);
        GemVal _t637 = _t636.fn(_t636.env, NULL, 0);
        GemVal _t640;
        if (gem_truthy(gem_eq(gem_table_get(_t637, gem_string("type")), gem_string("*")))) {
                _t640 = gem_eq(gem_table_get(_t637, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t638 = (*gem_v_peek);
                GemVal _t639 = _t638.fn(_t638.env, NULL, 0);
                _t640 = gem_eq(gem_table_get(_t639, gem_string("type")), gem_string("/"));
        }
        GemVal _t643;
        if (gem_truthy(_t640)) {
                _t643 = _t640;
        } else {
                GemVal _t641 = (*gem_v_peek);
                GemVal _t642 = _t641.fn(_t641.env, NULL, 0);
                _t643 = gem_eq(gem_table_get(_t642, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t643)) break;
#line 381 "compiler/main.gem"
    GemVal _t644 = (*gem_v_advance);
    GemVal _t645 = _t644.fn(_t644.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t645, gem_string("type"));
#line 382 "compiler/main.gem"
    GemVal _t646 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t646.fn(_t646.env, NULL, 0);
#line 383 "compiler/main.gem"
    GemVal _t647[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t647, 3);
    }
    GemVal _t648 = gem_v_left;
    gem_pop_frame();
    return _t648;
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
#line 390 "compiler/main.gem"
    GemVal _t650 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t650.fn(_t650.env, NULL, 0);
#line 391 "compiler/main.gem"
    while (1) {
        GemVal _t651 = (*gem_v_peek);
        GemVal _t652 = _t651.fn(_t651.env, NULL, 0);
        GemVal _t655;
        if (gem_truthy(gem_eq(gem_table_get(_t652, gem_string("type")), gem_string("+")))) {
                _t655 = gem_eq(gem_table_get(_t652, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t653 = (*gem_v_peek);
                GemVal _t654 = _t653.fn(_t653.env, NULL, 0);
                _t655 = gem_eq(gem_table_get(_t654, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t655)) break;
#line 392 "compiler/main.gem"
    GemVal _t656 = (*gem_v_advance);
    GemVal _t657 = _t656.fn(_t656.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t657, gem_string("type"));
#line 393 "compiler/main.gem"
    GemVal _t658 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t658.fn(_t658.env, NULL, 0);
#line 394 "compiler/main.gem"
    GemVal _t659[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t659, 3);
    }
    GemVal _t660 = gem_v_left;
    gem_pop_frame();
    return _t660;
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
#line 401 "compiler/main.gem"
    GemVal _t662 = (*gem_v_parse_add);
    GemVal gem_v_left = _t662.fn(_t662.env, NULL, 0);
#line 402 "compiler/main.gem"
    while (1) {
        GemVal _t663 = (*gem_v_peek);
        GemVal _t664 = _t663.fn(_t663.env, NULL, 0);
        GemVal _t667;
        if (gem_truthy(gem_eq(gem_table_get(_t664, gem_string("type")), gem_string("==")))) {
                _t667 = gem_eq(gem_table_get(_t664, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t665 = (*gem_v_peek);
                GemVal _t666 = _t665.fn(_t665.env, NULL, 0);
                _t667 = gem_eq(gem_table_get(_t666, gem_string("type")), gem_string("!="));
        }
        GemVal _t670;
        if (gem_truthy(_t667)) {
                _t670 = _t667;
        } else {
                GemVal _t668 = (*gem_v_peek);
                GemVal _t669 = _t668.fn(_t668.env, NULL, 0);
                _t670 = gem_eq(gem_table_get(_t669, gem_string("type")), gem_string("<"));
        }
        GemVal _t673;
        if (gem_truthy(_t670)) {
                _t673 = _t670;
        } else {
                GemVal _t671 = (*gem_v_peek);
                GemVal _t672 = _t671.fn(_t671.env, NULL, 0);
                _t673 = gem_eq(gem_table_get(_t672, gem_string("type")), gem_string(">"));
        }
        GemVal _t676;
        if (gem_truthy(_t673)) {
                _t676 = _t673;
        } else {
                GemVal _t674 = (*gem_v_peek);
                GemVal _t675 = _t674.fn(_t674.env, NULL, 0);
                _t676 = gem_eq(gem_table_get(_t675, gem_string("type")), gem_string("<="));
        }
        GemVal _t679;
        if (gem_truthy(_t676)) {
                _t679 = _t676;
        } else {
                GemVal _t677 = (*gem_v_peek);
                GemVal _t678 = _t677.fn(_t677.env, NULL, 0);
                _t679 = gem_eq(gem_table_get(_t678, gem_string("type")), gem_string(">="));
        }
        GemVal _t682;
        if (gem_truthy(_t679)) {
                _t682 = _t679;
        } else {
                GemVal _t680 = (*gem_v_peek);
                GemVal _t681 = _t680.fn(_t680.env, NULL, 0);
                _t682 = gem_eq(gem_table_get(_t681, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t682)) break;
#line 403 "compiler/main.gem"
    GemVal _t683 = (*gem_v_advance);
    GemVal _t684 = _t683.fn(_t683.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t684, gem_string("type"));
#line 404 "compiler/main.gem"
    GemVal _t685 = (*gem_v_parse_add);
        GemVal gem_v_right = _t685.fn(_t685.env, NULL, 0);
#line 405 "compiler/main.gem"
    GemVal _t686[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t686, 3);
    }
    GemVal _t687 = gem_v_left;
    gem_pop_frame();
    return _t687;
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
#line 412 "compiler/main.gem"
    GemVal _t689 = (*gem_v_peek);
    GemVal _t690 = _t689.fn(_t689.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t690, gem_string("type")), gem_string("not")))) {
#line 413 "compiler/main.gem"
    GemVal _t691 = (*gem_v_advance);
        (void)(_t691.fn(_t691.env, NULL, 0));
#line 414 "compiler/main.gem"
    GemVal _t692 = (*gem_v_parse_not);
    GemVal _t693[] = {gem_string("not"), _t692.fn(_t692.env, NULL, 0)};
        GemVal _t694 = gem_fn_make_unop(NULL, _t693, 2);
        gem_pop_frame();
        return _t694;
    }
#line 416 "compiler/main.gem"
    GemVal _t695 = (*gem_v_parse_compare);
    GemVal _t696 = _t695.fn(_t695.env, NULL, 0);
    gem_pop_frame();
    return _t696;
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
#line 421 "compiler/main.gem"
    GemVal _t698 = (*gem_v_parse_not);
    GemVal gem_v_left = _t698.fn(_t698.env, NULL, 0);
#line 422 "compiler/main.gem"
    while (1) {
        GemVal _t699 = (*gem_v_peek);
        GemVal _t700 = _t699.fn(_t699.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t700, gem_string("type")), gem_string("and")))) break;
#line 423 "compiler/main.gem"
    GemVal _t701 = (*gem_v_advance);
        (void)(_t701.fn(_t701.env, NULL, 0));
#line 424 "compiler/main.gem"
    GemVal _t702 = (*gem_v_parse_not);
        GemVal gem_v_right = _t702.fn(_t702.env, NULL, 0);
#line 425 "compiler/main.gem"
    GemVal _t703[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t703, 3);
    }
    GemVal _t704 = gem_v_left;
    gem_pop_frame();
    return _t704;
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
#line 432 "compiler/main.gem"
    GemVal _t706 = (*gem_v_parse_and);
    GemVal gem_v_left = _t706.fn(_t706.env, NULL, 0);
#line 433 "compiler/main.gem"
    while (1) {
        GemVal _t707 = (*gem_v_peek);
        GemVal _t708 = _t707.fn(_t707.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t708, gem_string("type")), gem_string("or")))) break;
#line 434 "compiler/main.gem"
    GemVal _t709 = (*gem_v_advance);
        (void)(_t709.fn(_t709.env, NULL, 0));
#line 435 "compiler/main.gem"
    GemVal _t710 = (*gem_v_parse_and);
        GemVal gem_v_right = _t710.fn(_t710.env, NULL, 0);
#line 436 "compiler/main.gem"
    GemVal _t711[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t711, 3);
    }
    GemVal _t712 = gem_v_left;
    gem_pop_frame();
    return _t712;
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
#line 443 "compiler/main.gem"
    GemVal _t714 = (*gem_v_peek);
    GemVal _t715 = _t714.fn(_t714.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t715, gem_string("line"));
#line 444 "compiler/main.gem"
    GemVal _t716 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t716.fn(_t716.env, NULL, 0);
#line 447 "compiler/main.gem"
    GemVal _t717 = (*gem_v_peek);
    GemVal _t718 = _t717.fn(_t717.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t718, gem_string("type")), gem_string("=")))) {
#line 448 "compiler/main.gem"
    GemVal _t719 = (*gem_v_advance);
        (void)(_t719.fn(_t719.env, NULL, 0));
#line 449 "compiler/main.gem"
    GemVal _t720 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t720.fn(_t720.env, NULL, 0);
#line 450 "compiler/main.gem"
    GemVal _t721 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t721, gem_string("tag")), gem_string("var")))) {
#line 451 "compiler/main.gem"
    GemVal _t722 = gem_v_lhs;
    GemVal _t723[] = {gem_table_get(_t722, gem_string("name")), gem_v_value, gem_v_start_line};
            GemVal _t724 = gem_fn_make_assign(NULL, _t723, 3);
            gem_pop_frame();
            return _t724;
        }
#line 453 "compiler/main.gem"
    GemVal _t725 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t725, gem_string("tag")), gem_string("dot")))) {
#line 454 "compiler/main.gem"
    GemVal _t726 = gem_v_lhs;
    GemVal _t727 = gem_v_lhs;
    GemVal _t728[] = {gem_table_get(_t726, gem_string("object")), gem_table_get(_t727, gem_string("field")), gem_v_value, gem_v_start_line};
            GemVal _t729 = gem_fn_make_dot_assign(NULL, _t728, 4);
            gem_pop_frame();
            return _t729;
        }
#line 456 "compiler/main.gem"
    GemVal _t730 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t730, gem_string("tag")), gem_string("index")))) {
#line 457 "compiler/main.gem"
    GemVal _t731 = gem_v_lhs;
    GemVal _t732 = gem_v_lhs;
    GemVal _t733[] = {gem_table_get(_t731, gem_string("object")), gem_table_get(_t732, gem_string("key")), gem_v_value, gem_v_start_line};
            GemVal _t734 = gem_fn_make_index_assign(NULL, _t733, 4);
            gem_pop_frame();
            return _t734;
        }
#line 459 "compiler/main.gem"
    GemVal _t735 = (*gem_v_peek);
    GemVal _t736 = _t735.fn(_t735.env, NULL, 0);
    GemVal _t737[] = {gem_table_get(_t736, gem_string("line"))};
    GemVal _t738[] = {gem_add(gem_string("invalid assignment target at line "), gem_to_string_fn(NULL, _t737, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 459, _t738, 1));
    }
#line 463 "compiler/main.gem"
    GemVal _t739 = (*gem_v_peek);
    GemVal _t740 = _t739.fn(_t739.env, NULL, 0);
    GemVal _t743;
    if (gem_truthy(gem_eq(gem_table_get(_t740, gem_string("type")), gem_string("+=")))) {
        _t743 = gem_eq(gem_table_get(_t740, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t741 = (*gem_v_peek);
        GemVal _t742 = _t741.fn(_t741.env, NULL, 0);
        _t743 = gem_eq(gem_table_get(_t742, gem_string("type")), gem_string("-="));
    }
    GemVal _t746;
    if (gem_truthy(_t743)) {
        _t746 = _t743;
    } else {
        GemVal _t744 = (*gem_v_peek);
        GemVal _t745 = _t744.fn(_t744.env, NULL, 0);
        _t746 = gem_eq(gem_table_get(_t745, gem_string("type")), gem_string("*="));
    }
    GemVal _t749;
    if (gem_truthy(_t746)) {
        _t749 = _t746;
    } else {
        GemVal _t747 = (*gem_v_peek);
        GemVal _t748 = _t747.fn(_t747.env, NULL, 0);
        _t749 = gem_eq(gem_table_get(_t748, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t749)) {
#line 464 "compiler/main.gem"
    GemVal _t750 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t750.fn(_t750.env, NULL, 0);
#line 465 "compiler/main.gem"
    GemVal _t751 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t751, gem_string("value")), gem_int(0));
#line 466 "compiler/main.gem"
    GemVal _t752 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t752.fn(_t752.env, NULL, 0);
#line 467 "compiler/main.gem"
    GemVal _t753 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t753, gem_string("tag")), gem_string("var")))) {
#line 468 "compiler/main.gem"
    GemVal _t754 = gem_v_op_tok;
    GemVal _t755[] = {gem_table_get(_t754, gem_string("line"))};
    GemVal _t756[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t755, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 468, _t756, 1));
        }
#line 470 "compiler/main.gem"
    GemVal _t757 = gem_v_lhs;
    GemVal _t758 = gem_v_lhs;
    GemVal _t759[] = {gem_table_get(_t758, gem_string("name"))};
    GemVal _t760[] = {gem_v_base_op, gem_fn_make_var(NULL, _t759, 1), gem_v_value};
    GemVal _t761[] = {gem_table_get(_t757, gem_string("name")), gem_fn_make_binop(NULL, _t760, 3), gem_v_start_line};
        GemVal _t762 = gem_fn_make_assign(NULL, _t761, 3);
        gem_pop_frame();
        return _t762;
    }
    GemVal _t763 = gem_v_lhs;
    gem_pop_frame();
    return _t763;
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
#line 482 "compiler/main.gem"
    GemVal _t765 = (*gem_v_peek);
    GemVal gem_v_t = _t765.fn(_t765.env, NULL, 0);
#line 485 "compiler/main.gem"
    GemVal _t766 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t766, gem_string("type")), gem_string("let")))) {
#line 486 "compiler/main.gem"
    GemVal _t767 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t767, gem_string("line"));
#line 487 "compiler/main.gem"
    GemVal _t768 = (*gem_v_advance);
        (void)(_t768.fn(_t768.env, NULL, 0));
#line 490 "compiler/main.gem"
    GemVal _t769 = (*gem_v_peek);
    GemVal _t770 = _t769.fn(_t769.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t770, gem_string("type")), gem_string("{")))) {
#line 491 "compiler/main.gem"
    GemVal _t771 = (*gem_v_advance);
            (void)(_t771.fn(_t771.env, NULL, 0));
#line 492 "compiler/main.gem"
    GemVal _t772 = gem_table_new();
            GemVal gem_v_names = _t772;
#line 493 "compiler/main.gem"
    GemVal _t773 = (*gem_v_skip_nl);
            (void)(_t773.fn(_t773.env, NULL, 0));
#line 494 "compiler/main.gem"
            while (1) {
                GemVal _t774 = (*gem_v_peek);
                GemVal _t775 = _t774.fn(_t774.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t775, gem_string("type")), gem_string("}")))) break;
#line 495 "compiler/main.gem"
    GemVal _t776 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t776.fn(_t776.env, NULL, 0);
#line 496 "compiler/main.gem"
    GemVal _t777 = gem_v_field_tok;
                if (gem_truthy(gem_eq(gem_table_get(_t777, gem_string("type")), gem_string("NAME")))) {
#line 497 "compiler/main.gem"
    GemVal _t778 = (*gem_v_advance);
                    (void)(_t778.fn(_t778.env, NULL, 0));
                } else {
#line 500 "compiler/main.gem"
    GemVal _t779 = (*gem_v_advance);
                    (void)(_t779.fn(_t779.env, NULL, 0));
                }
#line 502 "compiler/main.gem"
    GemVal _t780 = gem_v_field_tok;
    GemVal _t781[] = {gem_v_names, gem_table_get(_t780, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t781, 2));
#line 503 "compiler/main.gem"
    GemVal _t782 = (*gem_v_skip_nl);
                (void)(_t782.fn(_t782.env, NULL, 0));
#line 504 "compiler/main.gem"
    GemVal _t783 = (*gem_v_peek);
    GemVal _t784 = _t783.fn(_t783.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t784, gem_string("type")), gem_string(",")))) {
#line 505 "compiler/main.gem"
    GemVal _t785 = (*gem_v_advance);
                    (void)(_t785.fn(_t785.env, NULL, 0));
#line 506 "compiler/main.gem"
    GemVal _t786 = (*gem_v_skip_nl);
                    (void)(_t786.fn(_t786.env, NULL, 0));
                }
            }
#line 509 "compiler/main.gem"
    GemVal _t787[] = {gem_string("}")};
    GemVal _t788 = (*gem_v_expect);
            (void)(_t788.fn(_t788.env, _t787, 1));
#line 510 "compiler/main.gem"
    GemVal _t789[] = {gem_string("=")};
    GemVal _t790 = (*gem_v_expect);
            (void)(_t790.fn(_t790.env, _t789, 1));
#line 511 "compiler/main.gem"
    GemVal _t791 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t791.fn(_t791.env, NULL, 0);
#line 512 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 513 "compiler/main.gem"
    GemVal _t792[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t792, 1));
#line 514 "compiler/main.gem"
    GemVal _t793 = gem_table_new();
    GemVal _t794[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t793, gem_int(0), gem_fn_make_let(NULL, _t794, 3));
            GemVal gem_v_stmts = _t793;
#line 515 "compiler/main.gem"
            GemVal gem_v__for_i_1 = gem_int(0);
#line 515 "compiler/main.gem"
    GemVal _t795[] = {gem_v_names};
            GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t795, 1);
#line 515 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 515 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_1;
#line 515 "compiler/main.gem"
                gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 516 "compiler/main.gem"
    GemVal _t796[] = {gem_v_tmp};
    GemVal _t797[] = {gem_fn_make_var(NULL, _t796, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t798[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_dot(NULL, _t797, 2), gem_v_line};
    GemVal _t799[] = {gem_v_stmts, gem_fn_make_let(NULL, _t798, 3)};
                (void)(gem_push_fn(NULL, _t799, 2));
            }

#line 518 "compiler/main.gem"
    GemVal _t800 = gem_table_new();
    gem_table_set(_t800, gem_string("tag"), gem_string("block"));
    gem_table_set(_t800, gem_string("stmts"), gem_v_stmts);
            GemVal _t801 = _t800;
            gem_pop_frame();
            return _t801;
        }
#line 522 "compiler/main.gem"
    GemVal _t802 = (*gem_v_peek);
    GemVal _t803 = _t802.fn(_t802.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t803, gem_string("type")), gem_string("[")))) {
#line 523 "compiler/main.gem"
    GemVal _t804 = (*gem_v_advance);
            (void)(_t804.fn(_t804.env, NULL, 0));
#line 524 "compiler/main.gem"
    GemVal _t805 = gem_table_new();
            GemVal gem_v_names = _t805;
#line 525 "compiler/main.gem"
    GemVal _t806 = (*gem_v_skip_nl);
            (void)(_t806.fn(_t806.env, NULL, 0));
#line 526 "compiler/main.gem"
            while (1) {
                GemVal _t807 = (*gem_v_peek);
                GemVal _t808 = _t807.fn(_t807.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t808, gem_string("type")), gem_string("]")))) break;
#line 527 "compiler/main.gem"
    GemVal _t809[] = {gem_string("NAME")};
    GemVal _t810 = (*gem_v_expect);
    GemVal _t811 = _t810.fn(_t810.env, _t809, 1);
    GemVal _t812[] = {gem_v_names, gem_table_get(_t811, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t812, 2));
#line 528 "compiler/main.gem"
    GemVal _t813 = (*gem_v_skip_nl);
                (void)(_t813.fn(_t813.env, NULL, 0));
#line 529 "compiler/main.gem"
    GemVal _t814 = (*gem_v_peek);
    GemVal _t815 = _t814.fn(_t814.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t815, gem_string("type")), gem_string(",")))) {
#line 530 "compiler/main.gem"
    GemVal _t816 = (*gem_v_advance);
                    (void)(_t816.fn(_t816.env, NULL, 0));
#line 531 "compiler/main.gem"
    GemVal _t817 = (*gem_v_skip_nl);
                    (void)(_t817.fn(_t817.env, NULL, 0));
                }
            }
#line 534 "compiler/main.gem"
    GemVal _t818[] = {gem_string("]")};
    GemVal _t819 = (*gem_v_expect);
            (void)(_t819.fn(_t819.env, _t818, 1));
#line 535 "compiler/main.gem"
    GemVal _t820[] = {gem_string("=")};
    GemVal _t821 = (*gem_v_expect);
            (void)(_t821.fn(_t821.env, _t820, 1));
#line 536 "compiler/main.gem"
    GemVal _t822 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t822.fn(_t822.env, NULL, 0);
#line 537 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 538 "compiler/main.gem"
    GemVal _t823[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t823, 1));
#line 539 "compiler/main.gem"
    GemVal _t824 = gem_table_new();
    GemVal _t825[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t824, gem_int(0), gem_fn_make_let(NULL, _t825, 3));
            GemVal gem_v_stmts = _t824;
#line 540 "compiler/main.gem"
            GemVal gem_v__for_i_2 = gem_int(0);
#line 540 "compiler/main.gem"
    GemVal _t826[] = {gem_v_names};
            GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t826, 1);
#line 540 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 540 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_2;
#line 540 "compiler/main.gem"
                gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 541 "compiler/main.gem"
    GemVal _t827[] = {gem_v_tmp};
    GemVal _t828[] = {gem_v_di};
    GemVal _t829[] = {gem_fn_make_var(NULL, _t827, 1), gem_fn_make_int(NULL, _t828, 1)};
    GemVal _t830[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_index(NULL, _t829, 2), gem_v_line};
    GemVal _t831[] = {gem_v_stmts, gem_fn_make_let(NULL, _t830, 3)};
                (void)(gem_push_fn(NULL, _t831, 2));
            }

#line 543 "compiler/main.gem"
    GemVal _t832 = gem_table_new();
    gem_table_set(_t832, gem_string("tag"), gem_string("block"));
    gem_table_set(_t832, gem_string("stmts"), gem_v_stmts);
            GemVal _t833 = _t832;
            gem_pop_frame();
            return _t833;
        }
#line 546 "compiler/main.gem"
    GemVal _t834[] = {gem_string("NAME")};
    GemVal _t835 = (*gem_v_expect);
    GemVal _t836 = _t835.fn(_t835.env, _t834, 1);
        GemVal gem_v_name = gem_table_get(_t836, gem_string("value"));
#line 547 "compiler/main.gem"
    GemVal _t837[] = {gem_string("=")};
    GemVal _t838 = (*gem_v_expect);
        (void)(_t838.fn(_t838.env, _t837, 1));
#line 548 "compiler/main.gem"
    GemVal _t839 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t839.fn(_t839.env, NULL, 0);
#line 549 "compiler/main.gem"
    GemVal _t840[] = {gem_v_name, gem_v_value, gem_v_line};
        GemVal _t841 = gem_fn_make_let(NULL, _t840, 3);
        gem_pop_frame();
        return _t841;
    }
#line 553 "compiler/main.gem"
    GemVal _t842 = gem_v_t;
    GemVal _t846;
    if (!gem_truthy(gem_eq(gem_table_get(_t842, gem_string("type")), gem_string("fn")))) {
        _t846 = gem_eq(gem_table_get(_t842, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t843[] = {gem_int(1)};
        GemVal _t844 = (*gem_v_peek_at);
        GemVal _t845 = _t844.fn(_t844.env, _t843, 1);
        _t846 = gem_eq(gem_table_get(_t845, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t846)) {
#line 554 "compiler/main.gem"
    GemVal _t847 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t847, gem_string("line"));
#line 555 "compiler/main.gem"
    GemVal _t848 = (*gem_v_advance);
        (void)(_t848.fn(_t848.env, NULL, 0));
#line 556 "compiler/main.gem"
    GemVal _t849[] = {gem_string("NAME")};
    GemVal _t850 = (*gem_v_expect);
    GemVal _t851 = _t850.fn(_t850.env, _t849, 1);
        GemVal gem_v_name = gem_table_get(_t851, gem_string("value"));
#line 557 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 558 "compiler/main.gem"
    GemVal _t852[] = {gem_v_name};
    GemVal _t853[] = {gem_v_line};
    GemVal _t854[] = {gem_v_name};
    GemVal _t855[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("named fn '"), gem_to_string_fn(NULL, _t852, 1)), gem_string("' inside function body is not supported at line ")), gem_to_string_fn(NULL, _t853, 1)), gem_string(" — use: let ")), gem_to_string_fn(NULL, _t854, 1)), gem_string(" = fn(...) ... end"))};
            (void)(gem_error_at_fn("compiler/main.gem", 558, _t855, 1));
        }
#line 560 "compiler/main.gem"
    GemVal _t856[] = {gem_string("(")};
    GemVal _t857 = (*gem_v_expect);
        (void)(_t857.fn(_t857.env, _t856, 1));
#line 561 "compiler/main.gem"
    GemVal _t858 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t858.fn(_t858.env, NULL, 0);
#line 562 "compiler/main.gem"
    GemVal _t859[] = {gem_string(")")};
    GemVal _t860 = (*gem_v_expect);
        (void)(_t860.fn(_t860.env, _t859, 1));
#line 563 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 564 "compiler/main.gem"
    GemVal _t861 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t861.fn(_t861.env, NULL, 0);
#line 565 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 566 "compiler/main.gem"
    GemVal _t862[] = {gem_string("end")};
    GemVal _t863 = (*gem_v_expect);
        (void)(_t863.fn(_t863.env, _t862, 1));
#line 567 "compiler/main.gem"
    GemVal _t864 = gem_v_pr;
    GemVal _t865 = gem_v_pr;
    GemVal _t866 = gem_v_pr;
    GemVal _t867[] = {gem_v_name, gem_table_get(_t864, gem_string("params")), gem_table_get(_t865, gem_string("rest_param")), gem_table_get(_t866, gem_string("block_param")), gem_v_body, gem_v_line};
        GemVal _t868 = gem_fn_make_fn_def(NULL, _t867, 6);
        gem_pop_frame();
        return _t868;
    }
#line 571 "compiler/main.gem"
    GemVal _t869 = gem_v_t;
    GemVal _t871;
    if (gem_truthy(gem_eq(gem_table_get(_t869, gem_string("type")), gem_string("if")))) {
        _t871 = gem_eq(gem_table_get(_t869, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t870 = gem_v_t;
        _t871 = gem_eq(gem_table_get(_t870, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t871)) {
#line 572 "compiler/main.gem"
    GemVal _t872 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t872, gem_string("line"));
#line 573 "compiler/main.gem"
    GemVal _t873 = (*gem_v_advance);
        (void)(_t873.fn(_t873.env, NULL, 0));
#line 574 "compiler/main.gem"
    GemVal _t874 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t874.fn(_t874.env, NULL, 0);
#line 575 "compiler/main.gem"
    GemVal _t875 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t875.fn(_t875.env, NULL, 0);
#line 576 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 577 "compiler/main.gem"
    GemVal _t876 = (*gem_v_peek);
    GemVal _t877 = _t876.fn(_t876.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t877, gem_string("type")), gem_string("elif")))) {
#line 579 "compiler/main.gem"
    GemVal _t878 = gem_table_new();
    GemVal _t879 = (*gem_v_parse_stmt);
    gem_table_set(_t878, gem_int(0), _t879.fn(_t879.env, NULL, 0));
            gem_v_else_body = _t878;
        } else {
#line 580 "compiler/main.gem"
    GemVal _t880 = (*gem_v_peek);
    GemVal _t881 = _t880.fn(_t880.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t881, gem_string("type")), gem_string("else")))) {
#line 581 "compiler/main.gem"
    GemVal _t882 = (*gem_v_advance);
                (void)(_t882.fn(_t882.env, NULL, 0));
#line 582 "compiler/main.gem"
    GemVal _t883 = (*gem_v_parse_body);
                gem_v_else_body = _t883.fn(_t883.env, NULL, 0);
            }
        }
#line 585 "compiler/main.gem"
    GemVal _t884 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t884, gem_string("type")), gem_string("if")))) {
#line 586 "compiler/main.gem"
    GemVal _t885[] = {gem_string("end")};
    GemVal _t886 = (*gem_v_expect);
            (void)(_t886.fn(_t886.env, _t885, 1));
        }
#line 588 "compiler/main.gem"
    GemVal _t887[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        GemVal _t888 = gem_fn_make_if(NULL, _t887, 4);
        gem_pop_frame();
        return _t888;
    }
#line 592 "compiler/main.gem"
    GemVal _t889 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t889, gem_string("type")), gem_string("while")))) {
#line 593 "compiler/main.gem"
    GemVal _t890 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t890, gem_string("line"));
#line 594 "compiler/main.gem"
    GemVal _t891 = (*gem_v_advance);
        (void)(_t891.fn(_t891.env, NULL, 0));
#line 595 "compiler/main.gem"
    GemVal _t892 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t892.fn(_t892.env, NULL, 0);
#line 596 "compiler/main.gem"
    GemVal _t893 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t893.fn(_t893.env, NULL, 0);
#line 597 "compiler/main.gem"
    GemVal _t894[] = {gem_string("end")};
    GemVal _t895 = (*gem_v_expect);
        (void)(_t895.fn(_t895.env, _t894, 1));
#line 598 "compiler/main.gem"
    GemVal _t896[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        GemVal _t897 = gem_fn_make_while(NULL, _t896, 3);
        gem_pop_frame();
        return _t897;
    }
#line 602 "compiler/main.gem"
    GemVal _t898 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t898, gem_string("type")), gem_string("for")))) {
#line 603 "compiler/main.gem"
    GemVal _t899 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t899, gem_string("line"));
#line 604 "compiler/main.gem"
    GemVal _t900 = (*gem_v_advance);
        (void)(_t900.fn(_t900.env, NULL, 0));
#line 605 "compiler/main.gem"
    GemVal _t901[] = {gem_string("NAME")};
    GemVal _t902 = (*gem_v_expect);
    GemVal _t903 = _t902.fn(_t902.env, _t901, 1);
        GemVal gem_v_var_name = gem_table_get(_t903, gem_string("value"));
#line 607 "compiler/main.gem"
    GemVal _t904 = (*gem_v_peek);
    GemVal _t905 = _t904.fn(_t904.env, NULL, 0);
    GemVal _t909;
    if (!gem_truthy(gem_eq(gem_table_get(_t905, gem_string("type")), gem_string(",")))) {
        _t909 = gem_eq(gem_table_get(_t905, gem_string("type")), gem_string(","));
    } else {
        GemVal _t906[] = {gem_int(1)};
        GemVal _t907 = (*gem_v_peek_at);
        GemVal _t908 = _t907.fn(_t907.env, _t906, 1);
        _t909 = gem_eq(gem_table_get(_t908, gem_string("type")), gem_string("NAME"));
    }
        if (gem_truthy(_t909)) {
#line 609 "compiler/main.gem"
    GemVal _t910 = (*gem_v_advance);
            (void)(_t910.fn(_t910.env, NULL, 0));
#line 610 "compiler/main.gem"
    GemVal _t911[] = {gem_string("NAME")};
    GemVal _t912 = (*gem_v_expect);
    GemVal _t913 = _t912.fn(_t912.env, _t911, 1);
            GemVal gem_v_val_name = gem_table_get(_t913, gem_string("value"));
#line 611 "compiler/main.gem"
    GemVal _t914[] = {gem_string("in")};
    GemVal _t915 = (*gem_v_expect);
            (void)(_t915.fn(_t915.env, _t914, 1));
#line 612 "compiler/main.gem"
    GemVal _t916 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t916.fn(_t916.env, NULL, 0);
#line 613 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 614 "compiler/main.gem"
    GemVal _t917[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_add(gem_string("_for_tbl_"), gem_to_string_fn(NULL, _t917, 1));
#line 615 "compiler/main.gem"
    GemVal _t918[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_keys_var = gem_add(gem_string("_for_keys_"), gem_to_string_fn(NULL, _t918, 1));
#line 616 "compiler/main.gem"
    GemVal _t919[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t919, 1));
#line 617 "compiler/main.gem"
    GemVal _t920 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t920.fn(_t920.env, NULL, 0);
#line 618 "compiler/main.gem"
    GemVal _t921[] = {gem_string("end")};
    GemVal _t922 = (*gem_v_expect);
            (void)(_t922.fn(_t922.env, _t921, 1));
#line 619 "compiler/main.gem"
    GemVal _t923 = gem_table_new();
            GemVal gem_v_inner_stmts = _t923;
#line 620 "compiler/main.gem"
    GemVal _t924[] = {gem_v_keys_var};
    GemVal _t925[] = {gem_v_idx_var};
    GemVal _t926[] = {gem_fn_make_var(NULL, _t924, 1), gem_fn_make_var(NULL, _t925, 1)};
    GemVal _t927[] = {gem_v_var_name, gem_fn_make_index(NULL, _t926, 2), gem_v_line};
    GemVal _t928[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t927, 3)};
            (void)(gem_push_fn(NULL, _t928, 2));
#line 621 "compiler/main.gem"
    GemVal _t929[] = {gem_v_tbl_var};
    GemVal _t930[] = {gem_v_keys_var};
    GemVal _t931[] = {gem_v_idx_var};
    GemVal _t932[] = {gem_fn_make_var(NULL, _t930, 1), gem_fn_make_var(NULL, _t931, 1)};
    GemVal _t933[] = {gem_fn_make_var(NULL, _t929, 1), gem_fn_make_index(NULL, _t932, 2)};
    GemVal _t934[] = {gem_v_val_name, gem_fn_make_index(NULL, _t933, 2), gem_v_line};
    GemVal _t935[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t934, 3)};
            (void)(gem_push_fn(NULL, _t935, 2));
#line 622 "compiler/main.gem"
    GemVal _t936[] = {gem_v_idx_var};
    GemVal _t937[] = {gem_int(1)};
    GemVal _t938[] = {gem_string("+"), gem_fn_make_var(NULL, _t936, 1), gem_fn_make_int(NULL, _t937, 1)};
    GemVal _t939[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t938, 3), gem_v_line};
    GemVal _t940[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t939, 3)};
            (void)(gem_push_fn(NULL, _t940, 2));
#line 623 "compiler/main.gem"
            GemVal gem_v__for_i_3 = gem_int(0);
#line 623 "compiler/main.gem"
    GemVal _t941[] = {gem_v_fbody};
            GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t941, 1);
#line 623 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 623 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_3;
#line 623 "compiler/main.gem"
                gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 624 "compiler/main.gem"
    GemVal _t942[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t942, 2));
            }

#line 626 "compiler/main.gem"
    GemVal _t943[] = {gem_v_idx_var};
    GemVal _t944[] = {gem_string("len")};
    GemVal _t945 = gem_table_new();
    GemVal _t946[] = {gem_v_keys_var};
    gem_table_set(_t945, gem_int(0), gem_fn_make_var(NULL, _t946, 1));
    GemVal _t947[] = {gem_fn_make_var(NULL, _t944, 1), _t945, gem_int(0)};
    GemVal _t948[] = {gem_string("<"), gem_fn_make_var(NULL, _t943, 1), gem_fn_make_call(NULL, _t947, 3)};
    GemVal _t949[] = {gem_fn_make_binop(NULL, _t948, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t949, 3);
#line 631 "compiler/main.gem"
    GemVal _t950 = gem_table_new();
    gem_table_set(_t950, gem_string("tag"), gem_string("block"));
    GemVal _t951 = gem_table_new();
    GemVal _t952[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    gem_table_set(_t951, gem_int(0), gem_fn_make_let(NULL, _t952, 3));
    GemVal _t953[] = {gem_string("keys")};
    GemVal _t954 = gem_table_new();
    GemVal _t955[] = {gem_v_tbl_var};
    gem_table_set(_t954, gem_int(0), gem_fn_make_var(NULL, _t955, 1));
    GemVal _t956[] = {gem_fn_make_var(NULL, _t953, 1), _t954, gem_int(0)};
    GemVal _t957[] = {gem_v_keys_var, gem_fn_make_call(NULL, _t956, 3), gem_v_line};
    gem_table_set(_t951, gem_int(1), gem_fn_make_let(NULL, _t957, 3));
    GemVal _t958[] = {gem_int(0)};
    GemVal _t959[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t958, 1), gem_v_line};
    gem_table_set(_t951, gem_int(2), gem_fn_make_let(NULL, _t959, 3));
    gem_table_set(_t951, gem_int(3), gem_v_while_node);
    gem_table_set(_t950, gem_string("stmts"), _t951);
            GemVal _t960 = _t950;
            gem_pop_frame();
            return _t960;
        } else {
#line 637 "compiler/main.gem"
    GemVal _t961 = (*gem_v_peek);
    GemVal _t962 = _t961.fn(_t961.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t962, gem_string("type")), gem_string("in")))) {
#line 639 "compiler/main.gem"
    GemVal _t963 = (*gem_v_advance);
                (void)(_t963.fn(_t963.env, NULL, 0));
#line 640 "compiler/main.gem"
    GemVal _t964 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t964.fn(_t964.env, NULL, 0);
#line 641 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 642 "compiler/main.gem"
    GemVal _t965[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t965, 1));
#line 643 "compiler/main.gem"
    GemVal _t966[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t966, 1));
#line 644 "compiler/main.gem"
    GemVal _t967 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t967.fn(_t967.env, NULL, 0);
#line 645 "compiler/main.gem"
    GemVal _t968[] = {gem_string("end")};
    GemVal _t969 = (*gem_v_expect);
                (void)(_t969.fn(_t969.env, _t968, 1));
#line 646 "compiler/main.gem"
    GemVal _t970 = gem_table_new();
                GemVal gem_v_inner_stmts = _t970;
#line 647 "compiler/main.gem"
    GemVal _t971[] = {gem_v_items_var};
    GemVal _t972[] = {gem_v_idx_var};
    GemVal _t973[] = {gem_fn_make_var(NULL, _t971, 1), gem_fn_make_var(NULL, _t972, 1)};
    GemVal _t974[] = {gem_v_var_name, gem_fn_make_index(NULL, _t973, 2), gem_v_line};
    GemVal _t975[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t974, 3)};
                (void)(gem_push_fn(NULL, _t975, 2));
#line 648 "compiler/main.gem"
    GemVal _t976[] = {gem_v_idx_var};
    GemVal _t977[] = {gem_int(1)};
    GemVal _t978[] = {gem_string("+"), gem_fn_make_var(NULL, _t976, 1), gem_fn_make_int(NULL, _t977, 1)};
    GemVal _t979[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t978, 3), gem_v_line};
    GemVal _t980[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t979, 3)};
                (void)(gem_push_fn(NULL, _t980, 2));
#line 649 "compiler/main.gem"
                GemVal gem_v__for_i_4 = gem_int(0);
#line 649 "compiler/main.gem"
    GemVal _t981[] = {gem_v_fbody};
                GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t981, 1);
#line 649 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 649 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_4;
#line 649 "compiler/main.gem"
                    gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 650 "compiler/main.gem"
    GemVal _t982[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t982, 2));
                }

#line 652 "compiler/main.gem"
    GemVal _t983[] = {gem_v_idx_var};
    GemVal _t984[] = {gem_string("len")};
    GemVal _t985 = gem_table_new();
    GemVal _t986[] = {gem_v_items_var};
    gem_table_set(_t985, gem_int(0), gem_fn_make_var(NULL, _t986, 1));
    GemVal _t987[] = {gem_fn_make_var(NULL, _t984, 1), _t985, gem_int(0)};
    GemVal _t988[] = {gem_string("<"), gem_fn_make_var(NULL, _t983, 1), gem_fn_make_call(NULL, _t987, 3)};
    GemVal _t989[] = {gem_fn_make_binop(NULL, _t988, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t989, 3);
#line 657 "compiler/main.gem"
    GemVal _t990 = gem_table_new();
    gem_table_set(_t990, gem_string("tag"), gem_string("block"));
    GemVal _t991 = gem_table_new();
    GemVal _t992[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t991, gem_int(0), gem_fn_make_let(NULL, _t992, 3));
    GemVal _t993[] = {gem_int(0)};
    GemVal _t994[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t993, 1), gem_v_line};
    gem_table_set(_t991, gem_int(1), gem_fn_make_let(NULL, _t994, 3));
    gem_table_set(_t991, gem_int(2), gem_v_while_node);
    gem_table_set(_t990, gem_string("stmts"), _t991);
                GemVal _t995 = _t990;
                gem_pop_frame();
                return _t995;
            } else {
#line 662 "compiler/main.gem"
    GemVal _t996 = (*gem_v_peek);
    GemVal _t997 = _t996.fn(_t996.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t997, gem_string("type")), gem_string("=")))) {
#line 664 "compiler/main.gem"
    GemVal _t998 = (*gem_v_advance);
                    (void)(_t998.fn(_t998.env, NULL, 0));
#line 665 "compiler/main.gem"
    GemVal _t999 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t999.fn(_t999.env, NULL, 0);
#line 666 "compiler/main.gem"
    GemVal _t1000[] = {gem_string(",")};
    GemVal _t1001 = (*gem_v_expect);
                    (void)(_t1001.fn(_t1001.env, _t1000, 1));
#line 667 "compiler/main.gem"
    GemVal _t1002 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1002.fn(_t1002.env, NULL, 0);
#line 668 "compiler/main.gem"
    GemVal _t1003 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1003.fn(_t1003.env, NULL, 0);
#line 669 "compiler/main.gem"
    GemVal _t1004[] = {gem_string("end")};
    GemVal _t1005 = (*gem_v_expect);
                    (void)(_t1005.fn(_t1005.env, _t1004, 1));
#line 670 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 671 "compiler/main.gem"
    GemVal _t1006[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1006, 1));
#line 672 "compiler/main.gem"
    GemVal _t1007[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t1007, 1));
#line 673 "compiler/main.gem"
    GemVal _t1008 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1008;
#line 674 "compiler/main.gem"
    GemVal _t1009[] = {gem_v_idx_var};
    GemVal _t1010[] = {gem_v_var_name, gem_fn_make_var(NULL, _t1009, 1), gem_v_line};
    GemVal _t1011[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1010, 3)};
                    (void)(gem_push_fn(NULL, _t1011, 2));
#line 675 "compiler/main.gem"
    GemVal _t1012[] = {gem_v_idx_var};
    GemVal _t1013[] = {gem_int(1)};
    GemVal _t1014[] = {gem_string("+"), gem_fn_make_var(NULL, _t1012, 1), gem_fn_make_int(NULL, _t1013, 1)};
    GemVal _t1015[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1014, 3), gem_v_line};
    GemVal _t1016[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1015, 3)};
                    (void)(gem_push_fn(NULL, _t1016, 2));
#line 676 "compiler/main.gem"
                    GemVal gem_v__for_i_5 = gem_int(0);
#line 676 "compiler/main.gem"
    GemVal _t1017[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_5 = gem_len_fn(NULL, _t1017, 1);
#line 676 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_limit_5))) break;
#line 676 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_5;
#line 676 "compiler/main.gem"
                        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 677 "compiler/main.gem"
    GemVal _t1018[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1018, 2));
                    }

#line 679 "compiler/main.gem"
    GemVal _t1019[] = {gem_v_idx_var};
    GemVal _t1020[] = {gem_v_limit_var};
    GemVal _t1021[] = {gem_string("<"), gem_fn_make_var(NULL, _t1019, 1), gem_fn_make_var(NULL, _t1020, 1)};
    GemVal _t1022[] = {gem_fn_make_binop(NULL, _t1021, 3), gem_v_inner_stmts, gem_v_line};
                    GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1022, 3);
#line 684 "compiler/main.gem"
    GemVal _t1023 = gem_table_new();
    gem_table_set(_t1023, gem_string("tag"), gem_string("block"));
    GemVal _t1024 = gem_table_new();
    GemVal _t1025[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t1024, gem_int(0), gem_fn_make_let(NULL, _t1025, 3));
    GemVal _t1026[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t1024, gem_int(1), gem_fn_make_let(NULL, _t1026, 3));
    gem_table_set(_t1024, gem_int(2), gem_v_while_node);
    gem_table_set(_t1023, gem_string("stmts"), _t1024);
                    GemVal _t1027 = _t1023;
                    gem_pop_frame();
                    return _t1027;
                } else {
#line 690 "compiler/main.gem"
    GemVal _t1028 = (*gem_v_peek);
    GemVal _t1029 = _t1028.fn(_t1028.env, NULL, 0);
    GemVal _t1030[] = {gem_table_get(_t1029, gem_string("line"))};
    GemVal _t1031[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t1030, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 690, _t1031, 1));
                }
            }
        }
    }
#line 695 "compiler/main.gem"
    GemVal _t1032 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1032, gem_string("type")), gem_string("match")))) {
#line 696 "compiler/main.gem"
    GemVal _t1033 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1033, gem_string("line"));
#line 697 "compiler/main.gem"
    GemVal _t1034 = (*gem_v_advance);
        (void)(_t1034.fn(_t1034.env, NULL, 0));
#line 698 "compiler/main.gem"
    GemVal _t1035 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1035.fn(_t1035.env, NULL, 0);
#line 699 "compiler/main.gem"
    GemVal _t1036 = (*gem_v_skip_nl);
        (void)(_t1036.fn(_t1036.env, NULL, 0));
#line 700 "compiler/main.gem"
    GemVal _t1037 = gem_table_new();
        GemVal gem_v_whens = _t1037;
#line 701 "compiler/main.gem"
        while (1) {
            GemVal _t1038 = (*gem_v_peek);
            GemVal _t1039 = _t1038.fn(_t1038.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1039, gem_string("type")), gem_string("when")))) break;
#line 702 "compiler/main.gem"
    GemVal _t1040 = (*gem_v_advance);
            (void)(_t1040.fn(_t1040.env, NULL, 0));
#line 703 "compiler/main.gem"
    GemVal _t1041 = (*gem_v_parse_expr);
            GemVal gem_v_wval = _t1041.fn(_t1041.env, NULL, 0);
#line 704 "compiler/main.gem"
    GemVal _t1042 = (*gem_v_parse_body);
            GemVal gem_v_wbody = _t1042.fn(_t1042.env, NULL, 0);
#line 705 "compiler/main.gem"
    GemVal _t1043[] = {gem_v_wval, gem_v_wbody};
    GemVal _t1044[] = {gem_v_whens, gem_fn_make_when(NULL, _t1043, 2)};
            (void)(gem_push_fn(NULL, _t1044, 2));
#line 706 "compiler/main.gem"
    GemVal _t1045 = (*gem_v_skip_nl);
            (void)(_t1045.fn(_t1045.env, NULL, 0));
        }
#line 708 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 709 "compiler/main.gem"
    GemVal _t1046 = (*gem_v_peek);
    GemVal _t1047 = _t1046.fn(_t1046.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1047, gem_string("type")), gem_string("else")))) {
#line 710 "compiler/main.gem"
    GemVal _t1048 = (*gem_v_advance);
            (void)(_t1048.fn(_t1048.env, NULL, 0));
#line 711 "compiler/main.gem"
    GemVal _t1049 = (*gem_v_parse_body);
            gem_v_else_body = _t1049.fn(_t1049.env, NULL, 0);
        }
#line 713 "compiler/main.gem"
    GemVal _t1050[] = {gem_string("end")};
    GemVal _t1051 = (*gem_v_expect);
        (void)(_t1051.fn(_t1051.env, _t1050, 1));
#line 714 "compiler/main.gem"
    GemVal _t1052[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line};
        GemVal _t1053 = gem_fn_make_match(NULL, _t1052, 4);
        gem_pop_frame();
        return _t1053;
    }
#line 718 "compiler/main.gem"
    GemVal _t1054 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1054, gem_string("type")), gem_string("return")))) {
#line 719 "compiler/main.gem"
    GemVal _t1055 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1055, gem_string("line"));
#line 720 "compiler/main.gem"
    GemVal _t1056 = (*gem_v_advance);
        (void)(_t1056.fn(_t1056.env, NULL, 0));
#line 721 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 722 "compiler/main.gem"
    GemVal _t1057 = (*gem_v_peek);
    GemVal _t1058 = _t1057.fn(_t1057.env, NULL, 0);
    GemVal _t1061;
    if (!gem_truthy(gem_neq(gem_table_get(_t1058, gem_string("type")), gem_string("NEWLINE")))) {
        _t1061 = gem_neq(gem_table_get(_t1058, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t1059 = (*gem_v_peek);
        GemVal _t1060 = _t1059.fn(_t1059.env, NULL, 0);
        _t1061 = gem_neq(gem_table_get(_t1060, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t1064;
    if (!gem_truthy(_t1061)) {
        _t1064 = _t1061;
    } else {
        GemVal _t1062 = (*gem_v_peek);
        GemVal _t1063 = _t1062.fn(_t1062.env, NULL, 0);
        _t1064 = gem_neq(gem_table_get(_t1063, gem_string("type")), gem_string("end"));
    }
    GemVal _t1067;
    if (!gem_truthy(_t1064)) {
        _t1067 = _t1064;
    } else {
        GemVal _t1065 = (*gem_v_peek);
        GemVal _t1066 = _t1065.fn(_t1065.env, NULL, 0);
        _t1067 = gem_neq(gem_table_get(_t1066, gem_string("type")), gem_string("elif"));
    }
    GemVal _t1070;
    if (!gem_truthy(_t1067)) {
        _t1070 = _t1067;
    } else {
        GemVal _t1068 = (*gem_v_peek);
        GemVal _t1069 = _t1068.fn(_t1068.env, NULL, 0);
        _t1070 = gem_neq(gem_table_get(_t1069, gem_string("type")), gem_string("else"));
    }
    GemVal _t1073;
    if (!gem_truthy(_t1070)) {
        _t1073 = _t1070;
    } else {
        GemVal _t1071 = (*gem_v_peek);
        GemVal _t1072 = _t1071.fn(_t1071.env, NULL, 0);
        _t1073 = gem_neq(gem_table_get(_t1072, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t1073)) {
#line 723 "compiler/main.gem"
    GemVal _t1074 = (*gem_v_parse_expr);
            gem_v_value = _t1074.fn(_t1074.env, NULL, 0);
        }
#line 725 "compiler/main.gem"
    GemVal _t1075[] = {gem_v_value, gem_v_line};
        GemVal _t1076 = gem_fn_make_return(NULL, _t1075, 2);
        gem_pop_frame();
        return _t1076;
    }
#line 729 "compiler/main.gem"
    GemVal _t1077 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1077, gem_string("type")), gem_string("break")))) {
#line 730 "compiler/main.gem"
    GemVal _t1078 = (*gem_v_advance);
        (void)(_t1078.fn(_t1078.env, NULL, 0));
#line 731 "compiler/main.gem"
        GemVal _t1079 = gem_fn_make_break(NULL, NULL, 0);
        gem_pop_frame();
        return _t1079;
    }
#line 735 "compiler/main.gem"
    GemVal _t1080 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1080, gem_string("type")), gem_string("continue")))) {
#line 736 "compiler/main.gem"
    GemVal _t1081 = (*gem_v_advance);
        (void)(_t1081.fn(_t1081.env, NULL, 0));
#line 737 "compiler/main.gem"
        GemVal _t1082 = gem_fn_make_continue(NULL, NULL, 0);
        gem_pop_frame();
        return _t1082;
    }
#line 741 "compiler/main.gem"
    GemVal _t1083 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1083, gem_string("type")), gem_string("load")))) {
#line 742 "compiler/main.gem"
    GemVal _t1084 = (*gem_v_advance);
        (void)(_t1084.fn(_t1084.env, NULL, 0));
#line 743 "compiler/main.gem"
    GemVal _t1085[] = {gem_string("STRING")};
    GemVal _t1086 = (*gem_v_expect);
    GemVal _t1087 = _t1086.fn(_t1086.env, _t1085, 1);
        GemVal gem_v_path = gem_table_get(_t1087, gem_string("value"));
#line 744 "compiler/main.gem"
    GemVal _t1088[] = {gem_v_path};
        GemVal _t1089 = gem_fn_make_load(NULL, _t1088, 1);
        gem_pop_frame();
        return _t1089;
    }
#line 748 "compiler/main.gem"
    GemVal _t1090 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1090, gem_string("type")), gem_string("extern")))) {
#line 749 "compiler/main.gem"
    GemVal _t1091 = (*gem_v_advance);
        (void)(_t1091.fn(_t1091.env, NULL, 0));
#line 750 "compiler/main.gem"
    GemVal _t1092 = (*gem_v_peek);
    GemVal _t1093 = _t1092.fn(_t1092.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1093, gem_string("type")), gem_string("fn")))) {
#line 751 "compiler/main.gem"
    GemVal _t1094 = (*gem_v_advance);
            (void)(_t1094.fn(_t1094.env, NULL, 0));
#line 752 "compiler/main.gem"
    GemVal _t1095[] = {gem_string("NAME")};
    GemVal _t1096 = (*gem_v_expect);
    GemVal _t1097 = _t1096.fn(_t1096.env, _t1095, 1);
            GemVal gem_v_name = gem_table_get(_t1097, gem_string("value"));
#line 753 "compiler/main.gem"
    GemVal _t1098[] = {gem_string("(")};
    GemVal _t1099 = (*gem_v_expect);
            (void)(_t1099.fn(_t1099.env, _t1098, 1));
#line 754 "compiler/main.gem"
    GemVal _t1100 = gem_table_new();
            GemVal gem_v_eparams = _t1100;
#line 755 "compiler/main.gem"
    GemVal _t1101 = (*gem_v_peek);
    GemVal _t1102 = _t1101.fn(_t1101.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t1102, gem_string("type")), gem_string(")")))) {
#line 756 "compiler/main.gem"
    GemVal _t1103[] = {gem_string("NAME")};
    GemVal _t1104 = (*gem_v_expect);
    GemVal _t1105 = _t1104.fn(_t1104.env, _t1103, 1);
                GemVal gem_v_pname = gem_table_get(_t1105, gem_string("value"));
#line 757 "compiler/main.gem"
    GemVal _t1106[] = {gem_string(":")};
    GemVal _t1107 = (*gem_v_expect);
                (void)(_t1107.fn(_t1107.env, _t1106, 1));
#line 758 "compiler/main.gem"
    GemVal _t1108[] = {gem_string("NAME")};
    GemVal _t1109 = (*gem_v_expect);
    GemVal _t1110 = _t1109.fn(_t1109.env, _t1108, 1);
                GemVal gem_v_ptype = gem_table_get(_t1110, gem_string("value"));
#line 759 "compiler/main.gem"
    GemVal _t1111[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1112[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1111, 2)};
                (void)(gem_push_fn(NULL, _t1112, 2));
#line 760 "compiler/main.gem"
                while (1) {
                    GemVal _t1113 = (*gem_v_peek);
                    GemVal _t1114 = _t1113.fn(_t1113.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1114, gem_string("type")), gem_string(",")))) break;
#line 761 "compiler/main.gem"
    GemVal _t1115 = (*gem_v_advance);
                    (void)(_t1115.fn(_t1115.env, NULL, 0));
#line 762 "compiler/main.gem"
    GemVal _t1116[] = {gem_string("NAME")};
    GemVal _t1117 = (*gem_v_expect);
    GemVal _t1118 = _t1117.fn(_t1117.env, _t1116, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t1118, gem_string("value"));
#line 763 "compiler/main.gem"
    GemVal _t1119[] = {gem_string(":")};
    GemVal _t1120 = (*gem_v_expect);
                    (void)(_t1120.fn(_t1120.env, _t1119, 1));
#line 764 "compiler/main.gem"
    GemVal _t1121[] = {gem_string("NAME")};
    GemVal _t1122 = (*gem_v_expect);
    GemVal _t1123 = _t1122.fn(_t1122.env, _t1121, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t1123, gem_string("value"));
#line 765 "compiler/main.gem"
    GemVal _t1124[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1125[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1124, 2)};
                    (void)(gem_push_fn(NULL, _t1125, 2));
                }
            }
#line 768 "compiler/main.gem"
    GemVal _t1126[] = {gem_string(")")};
    GemVal _t1127 = (*gem_v_expect);
            (void)(_t1127.fn(_t1127.env, _t1126, 1));
#line 769 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 770 "compiler/main.gem"
    GemVal _t1128 = (*gem_v_peek);
    GemVal _t1129 = _t1128.fn(_t1128.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1129, gem_string("type")), gem_string("->")))) {
#line 771 "compiler/main.gem"
    GemVal _t1130 = (*gem_v_advance);
                (void)(_t1130.fn(_t1130.env, NULL, 0));
#line 772 "compiler/main.gem"
    GemVal _t1131[] = {gem_string("NAME")};
    GemVal _t1132 = (*gem_v_expect);
    GemVal _t1133 = _t1132.fn(_t1132.env, _t1131, 1);
                gem_v_ret_type = gem_table_get(_t1133, gem_string("value"));
            }
#line 774 "compiler/main.gem"
    GemVal _t1134[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            GemVal _t1135 = gem_fn_make_extern_fn(NULL, _t1134, 3);
            gem_pop_frame();
            return _t1135;
        } else {
#line 775 "compiler/main.gem"
    GemVal _t1136 = (*gem_v_peek);
    GemVal _t1137 = _t1136.fn(_t1136.env, NULL, 0);
    GemVal _t1140;
    if (!gem_truthy(gem_eq(gem_table_get(_t1137, gem_string("type")), gem_string("NAME")))) {
        _t1140 = gem_eq(gem_table_get(_t1137, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1138 = (*gem_v_peek);
        GemVal _t1139 = _t1138.fn(_t1138.env, NULL, 0);
        _t1140 = gem_eq(gem_table_get(_t1139, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t1140)) {
#line 776 "compiler/main.gem"
    GemVal _t1141 = (*gem_v_advance);
                (void)(_t1141.fn(_t1141.env, NULL, 0));
#line 777 "compiler/main.gem"
    GemVal _t1142[] = {gem_string("STRING")};
    GemVal _t1143 = (*gem_v_expect);
    GemVal _t1144 = _t1143.fn(_t1143.env, _t1142, 1);
                GemVal gem_v_path = gem_table_get(_t1144, gem_string("value"));
#line 778 "compiler/main.gem"
    GemVal _t1145[] = {gem_v_path};
                GemVal _t1146 = gem_fn_make_extern_include(NULL, _t1145, 1);
                gem_pop_frame();
                return _t1146;
            } else {
#line 780 "compiler/main.gem"
    GemVal _t1147 = (*gem_v_peek);
    GemVal _t1148 = _t1147.fn(_t1147.env, NULL, 0);
    GemVal _t1149[] = {gem_table_get(_t1148, gem_string("line"))};
    GemVal _t1150[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t1149, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 780, _t1150, 1));
            }
        }
    }
#line 785 "compiler/main.gem"
    GemVal _t1151 = (*gem_v_parse_expr);
    GemVal _t1152 = _t1151.fn(_t1151.env, NULL, 0);
    gem_pop_frame();
    return _t1152;
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
#line 794 "compiler/main.gem"
    GemVal _t1154 = gem_table_new();
    GemVal gem_v_stmts = _t1154;
#line 795 "compiler/main.gem"
    GemVal _t1155 = (*gem_v_skip_nl);
    (void)(_t1155.fn(_t1155.env, NULL, 0));
#line 796 "compiler/main.gem"
    while (1) {
        GemVal _t1156 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1156.fn(_t1156.env, NULL, 0)))) break;
#line 797 "compiler/main.gem"
    GemVal _t1157 = (*gem_v_parse_stmt);
    GemVal _t1158[] = {gem_v_stmts, _t1157.fn(_t1157.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1158, 2));
#line 798 "compiler/main.gem"
    GemVal _t1159 = (*gem_v_skip_nl);
        (void)(_t1159.fn(_t1159.env, NULL, 0));
    }
#line 800 "compiler/main.gem"
    GemVal _t1160[] = {gem_v_stmts};
    GemVal _t1161 = gem_fn_make_program(NULL, _t1160, 1);
    gem_pop_frame();
    return _t1161;
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
    struct _closure__anon_1 *_t317 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t317->gem_v_pos = gem_v_pos;
    _t317->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t317);
#line 29 "compiler/main.gem"
    struct _closure__anon_2 *_t319 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t319->gem_v_pos = gem_v_pos;
    _t319->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t319);
#line 33 "compiler/main.gem"
    struct _closure__anon_3 *_t321 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t321->gem_v_pos = gem_v_pos;
    _t321->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t321);
#line 39 "compiler/main.gem"
    struct _closure__anon_4 *_t324 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t324->gem_v_pos = gem_v_pos;
    _t324->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t324);
#line 43 "compiler/main.gem"
    struct _closure__anon_5 *_t333 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t333->gem_v_pos = gem_v_pos;
    _t333->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t333);
#line 52 "compiler/main.gem"
    struct _closure__anon_6 *_t335 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t335->gem_v_pos = gem_v_pos;
    _t335->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t335);
#line 64 "compiler/main.gem"
    struct _closure__anon_7 *_t373 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t373->gem_v_advance = gem_v_advance;
    _t373->gem_v_expect = gem_v_expect;
    _t373->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t373);
#line 97 "compiler/main.gem"
    GemVal *gem_v_parse_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_expr = GEM_NIL;
#line 98 "compiler/main.gem"
    GemVal *gem_v_parse_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_stmt = GEM_NIL;
#line 99 "compiler/main.gem"
    GemVal *gem_v_parse_unary = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_unary = GEM_NIL;
#line 100 "compiler/main.gem"
    GemVal *gem_v_parse_not = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_not = GEM_NIL;
#line 106 "compiler/main.gem"
    struct _closure__anon_8 *_t393 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t393->gem_v_at_end = gem_v_at_end;
    _t393->gem_v_parse_stmt = gem_v_parse_stmt;
    _t393->gem_v_peek = gem_v_peek;
    _t393->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t393);
#line 119 "compiler/main.gem"
    struct _closure__anon_9 *_t404 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t404->gem_v_at_end = gem_v_at_end;
    _t404->gem_v_parse_stmt = gem_v_parse_stmt;
    _t404->gem_v_peek = gem_v_peek;
    _t404->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t404);
#line 132 "compiler/main.gem"
    struct _closure__anon_10 *_t534 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t534->gem_v_advance = gem_v_advance;
    _t534->gem_v_expect = gem_v_expect;
    _t534->gem_v_fn_depth = gem_v_fn_depth;
    _t534->gem_v_parse_expr = gem_v_parse_expr;
    _t534->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t534->gem_v_parse_params = gem_v_parse_params;
    _t534->gem_v_peek = gem_v_peek;
    _t534->gem_v_peek_at = gem_v_peek_at;
    _t534->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t534);
#line 263 "compiler/main.gem"
    struct _closure__anon_11 *_t625 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t625->gem_v_advance = gem_v_advance;
    _t625->gem_v_expect = gem_v_expect;
    _t625->gem_v_fn_depth = gem_v_fn_depth;
    _t625->gem_v_parse_atom = gem_v_parse_atom;
    _t625->gem_v_parse_expr = gem_v_parse_expr;
    _t625->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t625->gem_v_parse_params = gem_v_parse_params;
    _t625->gem_v_peek = gem_v_peek;
    _t625->gem_v_peek_at = gem_v_peek_at;
    _t625->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t625);
#line 369 "compiler/main.gem"
    struct _closure__anon_12 *_t634 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t634->gem_v_advance = gem_v_advance;
    _t634->gem_v_parse_call = gem_v_parse_call;
    _t634->gem_v_parse_unary = gem_v_parse_unary;
    _t634->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t634);
#line 378 "compiler/main.gem"
    struct _closure__anon_13 *_t649 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t649->gem_v_advance = gem_v_advance;
    _t649->gem_v_parse_unary = gem_v_parse_unary;
    _t649->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t649);
#line 389 "compiler/main.gem"
    struct _closure__anon_14 *_t661 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t661->gem_v_advance = gem_v_advance;
    _t661->gem_v_parse_mul = gem_v_parse_mul;
    _t661->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t661);
#line 400 "compiler/main.gem"
    struct _closure__anon_15 *_t688 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t688->gem_v_advance = gem_v_advance;
    _t688->gem_v_parse_add = gem_v_parse_add;
    _t688->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t688);
#line 411 "compiler/main.gem"
    struct _closure__anon_16 *_t697 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t697->gem_v_advance = gem_v_advance;
    _t697->gem_v_parse_compare = gem_v_parse_compare;
    _t697->gem_v_parse_not = gem_v_parse_not;
    _t697->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t697);
#line 420 "compiler/main.gem"
    struct _closure__anon_17 *_t705 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t705->gem_v_advance = gem_v_advance;
    _t705->gem_v_parse_not = gem_v_parse_not;
    _t705->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t705);
#line 431 "compiler/main.gem"
    struct _closure__anon_18 *_t713 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t713->gem_v_advance = gem_v_advance;
    _t713->gem_v_parse_and = gem_v_parse_and;
    _t713->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t713);
#line 442 "compiler/main.gem"
    struct _closure__anon_19 *_t764 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t764->gem_v_advance = gem_v_advance;
    _t764->gem_v_parse_expr = gem_v_parse_expr;
    _t764->gem_v_parse_or = gem_v_parse_or;
    _t764->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t764);
#line 477 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 481 "compiler/main.gem"
    struct _closure__anon_20 *_t1153 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t1153->gem_v_advance = gem_v_advance;
    _t1153->gem_v_expect = gem_v_expect;
    _t1153->gem_v_fn_depth = gem_v_fn_depth;
    _t1153->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1153->gem_v_parse_body = gem_v_parse_body;
    _t1153->gem_v_parse_expr = gem_v_parse_expr;
    _t1153->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1153->gem_v_parse_params = gem_v_parse_params;
    _t1153->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1153->gem_v_peek = gem_v_peek;
    _t1153->gem_v_peek_at = gem_v_peek_at;
    _t1153->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_20, _t1153);
#line 789 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 793 "compiler/main.gem"
    struct _closure__anon_21 *_t1162 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t1162->gem_v_at_end = gem_v_at_end;
    _t1162->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1162->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_21, _t1162);
    GemVal _t1163 = gem_table_new();
    gem_table_set(_t1163, gem_string("parse"), gem_v_parse);
    GemVal _t1164 = _t1163;
    gem_pop_frame();
    return _t1164;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("escape_c_string", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal _t1165[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1165, 1);
#line 11 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 12 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 12 "compiler/main.gem"
    GemVal _t1166[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1166, 1);
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
    GemVal _t1167[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1167, 1), gem_int(0)))) {
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

    GemVal _t1168 = gem_v_result;
    gem_pop_frame();
    return _t1168;
}

static GemVal gem_fn_repeat_str(void *_env, GemVal *args, int argc) {
#line 33 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("repeat_str", "compiler/main.gem", 33);
#line 34 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
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

    GemVal _t1169 = gem_v_result;
    gem_pop_frame();
    return _t1169;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 41 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("pad_for", "compiler/main.gem", 41);
#line 42 "compiler/main.gem"
    GemVal _t1170[] = {gem_string("    "), gem_v_indent};
    GemVal _t1171 = gem_fn_repeat_str(NULL, _t1170, 2);
    gem_pop_frame();
    return _t1171;
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 45 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("format_float", "compiler/main.gem", 45);
#line 46 "compiler/main.gem"
    GemVal _t1172[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1172, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 49 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 49 "compiler/main.gem"
    GemVal _t1173[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1173, 1);
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

#line 54 "compiler/main.gem"
    if (gem_truthy(gem_not(gem_v_has_dot))) {
#line 55 "compiler/main.gem"
        gem_v_s = gem_add(gem_v_s, gem_string(".0"));
    }
    GemVal _t1174 = gem_v_s;
    gem_pop_frame();
    return _t1174;
}

static GemVal gem_fn_set_add(void *_env, GemVal *args, int argc) {
#line 62 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_add", "compiler/main.gem", 62);
#line 63 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1)))) {
#line 64 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_val, gem_bool(1));
        gem_pop_frame();
        return GEM_NIL;
    } else {
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn_set_contains(void *_env, GemVal *args, int argc) {
#line 68 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_contains", "compiler/main.gem", 68);
    GemVal _t1175 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1175;
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 72 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_union", "compiler/main.gem", 72);
#line 73 "compiler/main.gem"
    GemVal _t1176 = gem_table_new();
    GemVal gem_v_result = _t1176;
#line 74 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 74 "compiler/main.gem"
    GemVal _t1177[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_keys_4 = gem_keys_fn(NULL, _t1177, 1);
#line 74 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 74 "compiler/main.gem"
    while (1) {
        GemVal _t1178[] = {gem_v__for_keys_4};
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_len_fn(NULL, _t1178, 1)))) break;
#line 74 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_4, gem_v__for_i_4);
#line 74 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_4, gem_table_get(gem_v__for_keys_4, gem_v__for_i_4));
#line 74 "compiler/main.gem"
        gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 75 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

#line 77 "compiler/main.gem"
    GemVal gem_v__for_tbl_5 = gem_v_b;
#line 77 "compiler/main.gem"
    GemVal _t1179[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_keys_5 = gem_keys_fn(NULL, _t1179, 1);
#line 77 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 77 "compiler/main.gem"
    while (1) {
        GemVal _t1180[] = {gem_v__for_keys_5};
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1180, 1)))) break;
#line 77 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_5, gem_v__for_i_5);
#line 77 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_5, gem_table_get(gem_v__for_keys_5, gem_v__for_i_5));
#line 77 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 78 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1181 = gem_v_result;
    gem_pop_frame();
    return _t1181;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 83 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_intersect", "compiler/main.gem", 83);
#line 84 "compiler/main.gem"
    GemVal _t1182 = gem_table_new();
    GemVal gem_v_result = _t1182;
#line 85 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 85 "compiler/main.gem"
    GemVal _t1183[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_keys_6 = gem_keys_fn(NULL, _t1183, 1);
#line 85 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 85 "compiler/main.gem"
    while (1) {
        GemVal _t1184[] = {gem_v__for_keys_6};
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1184, 1)))) break;
#line 85 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_6, gem_v__for_i_6);
#line 85 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_6, gem_table_get(gem_v__for_keys_6, gem_v__for_i_6));
#line 85 "compiler/main.gem"
        gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 86 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_v_k), gem_bool(1)))) {
#line 87 "compiler/main.gem"
            gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
        }
    }

    GemVal _t1185 = gem_v_result;
    gem_pop_frame();
    return _t1185;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("set_to_sorted_array", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t1186[] = {gem_v_s};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t1186, 1);
#line 96 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 97 "compiler/main.gem"
    while (1) {
        GemVal _t1187[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t1187, 1)))) break;
#line 98 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
#line 99 "compiler/main.gem"
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
#line 100 "compiler/main.gem"
        while (1) {
            GemVal _t1188;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t1188 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t1188 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t1188)) break;
#line 101 "compiler/main.gem"
            gem_table_set(gem_v_ks, gem_add(gem_v_j, gem_int(1)), gem_table_get(gem_v_ks, gem_v_j));
#line 102 "compiler/main.gem"
            gem_v_j = gem_sub(gem_v_j, gem_int(1));
        }
#line 104 "compiler/main.gem"
        gem_table_set(gem_v_ks, gem_add(gem_v_j, gem_int(1)), gem_v_key);
#line 105 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t1189 = gem_v_ks;
    gem_pop_frame();
    return _t1189;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 110 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("sorted_array_to_set", "compiler/main.gem", 110);
#line 111 "compiler/main.gem"
    GemVal _t1190 = gem_table_new();
    GemVal gem_v_s = _t1190;
#line 112 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 112 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 112 "compiler/main.gem"
    while (1) {
        GemVal _t1191[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1191, 1)))) break;
#line 112 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 112 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 113 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1192 = gem_v_s;
    gem_pop_frame();
    return _t1192;
}

static GemVal gem_fn_collect_top_let_names(void *_env, GemVal *args, int argc) {
#line 120 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("collect_top_let_names", "compiler/main.gem", 120);
    {
#line 121 "compiler/main.gem"
        GemVal gem_v__for_i_8 = gem_int(0);
#line 121 "compiler/main.gem"
    GemVal _t1193[] = {gem_v_stmts};
        GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1193, 1);
#line 121 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 121 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_8;
#line 121 "compiler/main.gem"
            gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 122 "compiler/main.gem"
            GemVal gem_v_s = gem_table_get(gem_v_stmts, gem_v_i);
#line 123 "compiler/main.gem"
    GemVal _t1194[] = {gem_v_s};
    GemVal _t1196;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1194, 1), gem_string("table")))) {
        _t1196 = gem_eq(gem_type_fn(NULL, _t1194, 1), gem_string("table"));
    } else {
        GemVal _t1195 = gem_v_s;
        _t1196 = gem_eq(gem_table_get(_t1195, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1196)) {
#line 124 "compiler/main.gem"
    GemVal _t1197 = gem_v_s;
    GemVal _t1198[] = {gem_v_result, gem_table_get(_t1197, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1198, 2));
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
#line 170 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t1210[] = {(*gem_v_tmp_counter)};
    GemVal _t1211 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t1210, 1));
    gem_pop_frame();
    return _t1211;
}

struct _closure__anon_23 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_23", "compiler/main.gem", 0);
#line 175 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t1213[] = {(*gem_v_anon_counter)};
    GemVal _t1214 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t1213, 1));
    gem_pop_frame();
    return _t1214;
}

struct _closure__anon_24 {
};
static GemVal _anon_24(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_24", "compiler/main.gem", 0);
    GemVal _t1216[] = {gem_v_name};
    GemVal _t1217 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t1216, 1));
    gem_pop_frame();
    return _t1217;
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
#line 189 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 190 "compiler/main.gem"
        GemVal _t1218 = GEM_NIL;
        gem_pop_frame();
        return _t1218;
    }
#line 192 "compiler/main.gem"
    GemVal _t1219[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1219, 1), gem_string("table")))) {
#line 193 "compiler/main.gem"
        GemVal _t1220 = GEM_NIL;
        gem_pop_frame();
        return _t1220;
    }
#line 195 "compiler/main.gem"
    GemVal _t1221 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1221, gem_string("tag"));
#line 196 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 197 "compiler/main.gem"
        GemVal _t1222 = GEM_NIL;
        gem_pop_frame();
        return _t1222;
    }
#line 200 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 201 "compiler/main.gem"
    GemVal _t1223 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1223, gem_string("name"));
#line 202 "compiler/main.gem"
    GemVal _t1224[] = {gem_v_defined, gem_v_name};
    GemVal _t1226;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1224, 2)))) {
        _t1226 = gem_not(gem_fn_set_contains(NULL, _t1224, 2));
    } else {
        GemVal _t1225[] = {(*gem_v_builtins), gem_v_name};
        _t1226 = gem_not(gem_fn_set_contains(NULL, _t1225, 2));
    }
    GemVal _t1228;
    if (!gem_truthy(_t1226)) {
        _t1228 = _t1226;
    } else {
        GemVal _t1227[] = {(*gem_v_defined_fns), gem_v_name};
        _t1228 = gem_not(gem_fn_set_contains(NULL, _t1227, 2));
    }
        if (gem_truthy(_t1228)) {
#line 203 "compiler/main.gem"
    GemVal _t1229[] = {gem_v_free, gem_v_name};
            GemVal _t1230 = gem_fn_set_add(NULL, _t1229, 2);
            gem_pop_frame();
            return _t1230;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
#line 205 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 206 "compiler/main.gem"
    GemVal _t1231 = gem_v_node;
    GemVal _t1232[] = {gem_table_get(_t1231, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1233 = (*gem_v_collect_free_node);
            GemVal _t1234 = _t1233.fn(_t1233.env, _t1232, 3);
            gem_pop_frame();
            return _t1234;
        } else {
#line 207 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 208 "compiler/main.gem"
    GemVal _t1235 = gem_v_node;
                GemVal gem_v_name = gem_table_get(_t1235, gem_string("name"));
#line 209 "compiler/main.gem"
    GemVal _t1236[] = {gem_v_defined, gem_v_name};
    GemVal _t1238;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1236, 2)))) {
        _t1238 = gem_not(gem_fn_set_contains(NULL, _t1236, 2));
    } else {
        GemVal _t1237[] = {(*gem_v_builtins), gem_v_name};
        _t1238 = gem_not(gem_fn_set_contains(NULL, _t1237, 2));
    }
    GemVal _t1240;
    if (!gem_truthy(_t1238)) {
        _t1240 = _t1238;
    } else {
        GemVal _t1239[] = {(*gem_v_defined_fns), gem_v_name};
        _t1240 = gem_not(gem_fn_set_contains(NULL, _t1239, 2));
    }
                if (gem_truthy(_t1240)) {
#line 210 "compiler/main.gem"
    GemVal _t1241[] = {gem_v_free, gem_v_name};
                    (void)(gem_fn_set_add(NULL, _t1241, 2));
                }
#line 212 "compiler/main.gem"
    GemVal _t1242 = gem_v_node;
    GemVal _t1243[] = {gem_table_get(_t1242, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1244 = (*gem_v_collect_free_node);
                GemVal _t1245 = _t1244.fn(_t1244.env, _t1243, 3);
                gem_pop_frame();
                return _t1245;
            } else {
#line 213 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 214 "compiler/main.gem"
    GemVal _t1246 = gem_v_node;
    GemVal _t1247[] = {gem_table_get(_t1246, gem_string("params"))};
    GemVal _t1248[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t1247, 1)};
                    GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t1248, 2);
#line 215 "compiler/main.gem"
    GemVal _t1249 = gem_v_node;
                    if (gem_truthy(gem_neq(gem_table_get(_t1249, gem_string("rest_param")), GEM_NIL))) {
#line 216 "compiler/main.gem"
    GemVal _t1250 = gem_v_node;
    GemVal _t1251[] = {gem_v_inner_defined, gem_table_get(_t1250, gem_string("rest_param"))};
                        (void)(gem_fn_set_add(NULL, _t1251, 2));
                    }
#line 218 "compiler/main.gem"
    GemVal _t1252 = gem_v_node;
                    if (gem_truthy(gem_neq(gem_table_get(_t1252, gem_string("block_param")), GEM_NIL))) {
#line 219 "compiler/main.gem"
    GemVal _t1253 = gem_v_node;
    GemVal _t1254[] = {gem_v_inner_defined, gem_table_get(_t1253, gem_string("block_param"))};
                        (void)(gem_fn_set_add(NULL, _t1254, 2));
                    }
#line 221 "compiler/main.gem"
    GemVal _t1255 = gem_v_node;
    GemVal _t1256[] = {gem_table_get(_t1255, gem_string("body")), gem_v_inner_defined};
    GemVal _t1257 = (*gem_v_collect_free_vars);
                    GemVal gem_v_inner_free = _t1257.fn(_t1257.env, _t1256, 2);
                    {
#line 222 "compiler/main.gem"
                        GemVal gem_v__for_tbl_10 = gem_v_inner_free;
#line 222 "compiler/main.gem"
    GemVal _t1258[] = {gem_v__for_tbl_10};
                        GemVal gem_v__for_keys_10 = gem_keys_fn(NULL, _t1258, 1);
#line 222 "compiler/main.gem"
                        GemVal gem_v__for_i_10 = gem_int(0);
#line 222 "compiler/main.gem"
                        while (1) {
                            GemVal _t1259[] = {gem_v__for_keys_10};
                            if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t1259, 1)))) break;
#line 222 "compiler/main.gem"
                            GemVal gem_v_ifk = gem_table_get(gem_v__for_keys_10, gem_v__for_i_10);
#line 222 "compiler/main.gem"
                            GemVal gem_v__ = gem_table_get(gem_v__for_tbl_10, gem_table_get(gem_v__for_keys_10, gem_v__for_i_10));
#line 222 "compiler/main.gem"
                            gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 223 "compiler/main.gem"
    GemVal _t1260[] = {gem_v_free, gem_v_ifk};
                            (void)(gem_fn_set_add(NULL, _t1260, 2));
                        }
                        gem_pop_frame();
                        return GEM_NIL;
                    }
                } else {
#line 225 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                        gem_pop_frame();
                        return GEM_NIL;
                    } else {
#line 227 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 228 "compiler/main.gem"
    GemVal _t1261 = gem_v_node;
    GemVal _t1262[] = {gem_table_get(_t1261, gem_string("cond")), gem_v_defined, gem_v_free};
    GemVal _t1263 = (*gem_v_collect_free_node);
                            (void)(_t1263.fn(_t1263.env, _t1262, 3));
#line 229 "compiler/main.gem"
    GemVal _t1264 = gem_table_new();
    GemVal _t1265[] = {gem_v_defined, _t1264};
                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1265, 2);
#line 230 "compiler/main.gem"
    GemVal _t1266 = gem_v_node;
                            GemVal gem_v_thens = gem_table_get(_t1266, gem_string("then"));
#line 231 "compiler/main.gem"
                            GemVal gem_v__for_i_11 = gem_int(0);
#line 231 "compiler/main.gem"
    GemVal _t1267[] = {gem_v_thens};
                            GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t1267, 1);
#line 231 "compiler/main.gem"
                            while (1) {
                                if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
#line 231 "compiler/main.gem"
                                GemVal gem_v_i = gem_v__for_i_11;
#line 231 "compiler/main.gem"
                                gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 232 "compiler/main.gem"
    GemVal _t1268[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1269 = (*gem_v_collect_free_node);
                                (void)(_t1269.fn(_t1269.env, _t1268, 3));
#line 233 "compiler/main.gem"
    GemVal _t1270[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t1272;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1270, 1), gem_string("table")))) {
        _t1272 = gem_eq(gem_type_fn(NULL, _t1270, 1), gem_string("table"));
    } else {
        GemVal _t1271 = gem_table_get(gem_v_thens, gem_v_i);
        _t1272 = gem_eq(gem_table_get(_t1271, gem_string("tag")), gem_string("let"));
    }
                                if (gem_truthy(_t1272)) {
#line 234 "compiler/main.gem"
    GemVal _t1273 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1274[] = {gem_v_d, gem_table_get(_t1273, gem_string("name"))};
                                    (void)(gem_fn_set_add(NULL, _t1274, 2));
                                }
                            }

#line 237 "compiler/main.gem"
    GemVal _t1275 = gem_v_node;
                            GemVal gem_v_el = gem_table_get(_t1275, gem_string("else"));
#line 238 "compiler/main.gem"
                            if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 239 "compiler/main.gem"
    GemVal _t1276 = gem_table_new();
    GemVal _t1277[] = {gem_v_defined, _t1276};
                                gem_v_d = gem_fn_set_union(NULL, _t1277, 2);
                                {
#line 240 "compiler/main.gem"
                                    GemVal gem_v__for_i_12 = gem_int(0);
#line 240 "compiler/main.gem"
    GemVal _t1278[] = {gem_v_el};
                                    GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t1278, 1);
#line 240 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 240 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_12;
#line 240 "compiler/main.gem"
                                        gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 241 "compiler/main.gem"
    GemVal _t1279[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1280 = (*gem_v_collect_free_node);
                                        (void)(_t1280.fn(_t1280.env, _t1279, 3));
#line 242 "compiler/main.gem"
    GemVal _t1281[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t1283;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1281, 1), gem_string("table")))) {
        _t1283 = gem_eq(gem_type_fn(NULL, _t1281, 1), gem_string("table"));
    } else {
        GemVal _t1282 = gem_table_get(gem_v_el, gem_v_i);
        _t1283 = gem_eq(gem_table_get(_t1282, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t1283)) {
#line 243 "compiler/main.gem"
    GemVal _t1284 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1285[] = {gem_v_d, gem_table_get(_t1284, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t1285, 2));
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
#line 247 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 248 "compiler/main.gem"
    GemVal _t1286 = gem_v_node;
    GemVal _t1287[] = {gem_table_get(_t1286, gem_string("cond")), gem_v_defined, gem_v_free};
    GemVal _t1288 = (*gem_v_collect_free_node);
                                (void)(_t1288.fn(_t1288.env, _t1287, 3));
#line 249 "compiler/main.gem"
    GemVal _t1289 = gem_table_new();
    GemVal _t1290[] = {gem_v_defined, _t1289};
                                GemVal gem_v_d = gem_fn_set_union(NULL, _t1290, 2);
                                {
#line 250 "compiler/main.gem"
                                    GemVal gem_v__for_i_13 = gem_int(0);
#line 250 "compiler/main.gem"
    GemVal _t1291 = gem_v_node;
    GemVal _t1292[] = {gem_table_get(_t1291, gem_string("body"))};
                                    GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1292, 1);
#line 250 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 250 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_13;
#line 250 "compiler/main.gem"
                                        gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 251 "compiler/main.gem"
    GemVal _t1293 = gem_v_node;
    GemVal _t1294[] = {gem_table_get(gem_table_get(_t1293, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1295 = (*gem_v_collect_free_node);
                                        (void)(_t1295.fn(_t1295.env, _t1294, 3));
#line 252 "compiler/main.gem"
    GemVal _t1296 = gem_v_node;
    GemVal _t1297[] = {gem_table_get(gem_table_get(_t1296, gem_string("body")), gem_v_i)};
    GemVal _t1300;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1297, 1), gem_string("table")))) {
        _t1300 = gem_eq(gem_type_fn(NULL, _t1297, 1), gem_string("table"));
    } else {
        GemVal _t1298 = gem_v_node;
        GemVal _t1299 = gem_table_get(gem_table_get(_t1298, gem_string("body")), gem_v_i);
        _t1300 = gem_eq(gem_table_get(_t1299, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t1300)) {
#line 253 "compiler/main.gem"
    GemVal _t1301 = gem_v_node;
    GemVal _t1302 = gem_table_get(gem_table_get(_t1301, gem_string("body")), gem_v_i);
    GemVal _t1303[] = {gem_v_d, gem_table_get(_t1302, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t1303, 2));
                                        }
                                    }
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 256 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 257 "compiler/main.gem"
    GemVal _t1304 = gem_v_node;
    GemVal _t1305[] = {gem_table_get(_t1304, gem_string("target")), gem_v_defined, gem_v_free};
    GemVal _t1306 = (*gem_v_collect_free_node);
                                    (void)(_t1306.fn(_t1306.env, _t1305, 3));
#line 258 "compiler/main.gem"
                                    GemVal gem_v__for_i_15 = gem_int(0);
#line 258 "compiler/main.gem"
    GemVal _t1307 = gem_v_node;
    GemVal _t1308[] = {gem_table_get(_t1307, gem_string("whens"))};
                                    GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1308, 1);
#line 258 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 258 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_15;
#line 258 "compiler/main.gem"
                                        gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 259 "compiler/main.gem"
    GemVal _t1309 = gem_v_node;
                                        GemVal gem_v_w = gem_table_get(gem_table_get(_t1309, gem_string("whens")), gem_v_i);
#line 260 "compiler/main.gem"
    GemVal _t1310 = gem_v_w;
    GemVal _t1311[] = {gem_table_get(_t1310, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1312 = (*gem_v_collect_free_node);
                                        (void)(_t1312.fn(_t1312.env, _t1311, 3));
#line 261 "compiler/main.gem"
    GemVal _t1313 = gem_table_new();
    GemVal _t1314[] = {gem_v_defined, _t1313};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1314, 2);
#line 262 "compiler/main.gem"
                                        GemVal gem_v__for_i_14 = gem_int(0);
#line 262 "compiler/main.gem"
    GemVal _t1315 = gem_v_w;
    GemVal _t1316[] = {gem_table_get(_t1315, gem_string("body"))};
                                        GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1316, 1);
#line 262 "compiler/main.gem"
                                        while (1) {
                                            if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 262 "compiler/main.gem"
                                            GemVal gem_v_j = gem_v__for_i_14;
#line 262 "compiler/main.gem"
                                            gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 263 "compiler/main.gem"
    GemVal _t1317 = gem_v_w;
    GemVal _t1318[] = {gem_table_get(gem_table_get(_t1317, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1319 = (*gem_v_collect_free_node);
                                            (void)(_t1319.fn(_t1319.env, _t1318, 3));
#line 264 "compiler/main.gem"
    GemVal _t1320 = gem_v_w;
    GemVal _t1321[] = {gem_table_get(gem_table_get(_t1320, gem_string("body")), gem_v_j)};
    GemVal _t1324;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1321, 1), gem_string("table")))) {
        _t1324 = gem_eq(gem_type_fn(NULL, _t1321, 1), gem_string("table"));
    } else {
        GemVal _t1322 = gem_v_w;
        GemVal _t1323 = gem_table_get(gem_table_get(_t1322, gem_string("body")), gem_v_j);
        _t1324 = gem_eq(gem_table_get(_t1323, gem_string("tag")), gem_string("let"));
    }
                                            if (gem_truthy(_t1324)) {
#line 265 "compiler/main.gem"
    GemVal _t1325 = gem_v_w;
    GemVal _t1326 = gem_table_get(gem_table_get(_t1325, gem_string("body")), gem_v_j);
    GemVal _t1327[] = {gem_v_d, gem_table_get(_t1326, gem_string("name"))};
                                                (void)(gem_fn_set_add(NULL, _t1327, 2));
                                            }
                                        }

                                    }

#line 269 "compiler/main.gem"
    GemVal _t1328 = gem_v_node;
                                    GemVal gem_v_el = gem_table_get(_t1328, gem_string("else"));
#line 270 "compiler/main.gem"
                                    if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 271 "compiler/main.gem"
    GemVal _t1329 = gem_table_new();
    GemVal _t1330[] = {gem_v_defined, _t1329};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1330, 2);
                                        {
#line 272 "compiler/main.gem"
                                            GemVal gem_v__for_i_16 = gem_int(0);
#line 272 "compiler/main.gem"
    GemVal _t1331[] = {gem_v_el};
                                            GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1331, 1);
#line 272 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 272 "compiler/main.gem"
                                                GemVal gem_v_j = gem_v__for_i_16;
#line 272 "compiler/main.gem"
                                                gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 273 "compiler/main.gem"
    GemVal _t1332[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1333 = (*gem_v_collect_free_node);
                                                (void)(_t1333.fn(_t1333.env, _t1332, 3));
#line 274 "compiler/main.gem"
    GemVal _t1334[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t1336;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1334, 1), gem_string("table")))) {
        _t1336 = gem_eq(gem_type_fn(NULL, _t1334, 1), gem_string("table"));
    } else {
        GemVal _t1335 = gem_table_get(gem_v_el, gem_v_j);
        _t1336 = gem_eq(gem_table_get(_t1335, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1336)) {
#line 275 "compiler/main.gem"
    GemVal _t1337 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t1338[] = {gem_v_d, gem_table_get(_t1337, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1338, 2));
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
#line 279 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("when_clause")))) {
#line 280 "compiler/main.gem"
    GemVal _t1339 = gem_v_node;
    GemVal _t1340[] = {gem_table_get(_t1339, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1341 = (*gem_v_collect_free_node);
                                        (void)(_t1341.fn(_t1341.env, _t1340, 3));
#line 281 "compiler/main.gem"
    GemVal _t1342 = gem_table_new();
    GemVal _t1343[] = {gem_v_defined, _t1342};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1343, 2);
                                        {
#line 282 "compiler/main.gem"
                                            GemVal gem_v__for_i_17 = gem_int(0);
#line 282 "compiler/main.gem"
    GemVal _t1344 = gem_v_node;
    GemVal _t1345[] = {gem_table_get(_t1344, gem_string("body"))};
                                            GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1345, 1);
#line 282 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 282 "compiler/main.gem"
                                                GemVal gem_v_i = gem_v__for_i_17;
#line 282 "compiler/main.gem"
                                                gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 283 "compiler/main.gem"
    GemVal _t1346 = gem_v_node;
    GemVal _t1347[] = {gem_table_get(gem_table_get(_t1346, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1348 = (*gem_v_collect_free_node);
                                                (void)(_t1348.fn(_t1348.env, _t1347, 3));
#line 284 "compiler/main.gem"
    GemVal _t1349 = gem_v_node;
    GemVal _t1350[] = {gem_table_get(gem_table_get(_t1349, gem_string("body")), gem_v_i)};
    GemVal _t1353;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1350, 1), gem_string("table")))) {
        _t1353 = gem_eq(gem_type_fn(NULL, _t1350, 1), gem_string("table"));
    } else {
        GemVal _t1351 = gem_v_node;
        GemVal _t1352 = gem_table_get(gem_table_get(_t1351, gem_string("body")), gem_v_i);
        _t1353 = gem_eq(gem_table_get(_t1352, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1353)) {
#line 285 "compiler/main.gem"
    GemVal _t1354 = gem_v_node;
    GemVal _t1355 = gem_table_get(gem_table_get(_t1354, gem_string("body")), gem_v_i);
    GemVal _t1356[] = {gem_v_d, gem_table_get(_t1355, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1356, 2));
                                                }
                                            }
                                            gem_pop_frame();
                                            return GEM_NIL;
                                        }
                                    } else {
#line 288 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 289 "compiler/main.gem"
    GemVal _t1357 = gem_table_new();
    GemVal _t1358[] = {gem_v_defined, _t1357};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1358, 2);
                                            {
#line 290 "compiler/main.gem"
                                                GemVal gem_v__for_i_18 = gem_int(0);
#line 290 "compiler/main.gem"
    GemVal _t1359 = gem_v_node;
    GemVal _t1360[] = {gem_table_get(_t1359, gem_string("stmts"))};
                                                GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1360, 1);
#line 290 "compiler/main.gem"
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 290 "compiler/main.gem"
                                                    GemVal gem_v_i = gem_v__for_i_18;
#line 290 "compiler/main.gem"
                                                    gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 291 "compiler/main.gem"
    GemVal _t1361 = gem_v_node;
    GemVal _t1362[] = {gem_table_get(gem_table_get(_t1361, gem_string("stmts")), gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1363 = (*gem_v_collect_free_node);
                                                    (void)(_t1363.fn(_t1363.env, _t1362, 3));
#line 292 "compiler/main.gem"
    GemVal _t1364 = gem_v_node;
    GemVal _t1365[] = {gem_table_get(gem_table_get(_t1364, gem_string("stmts")), gem_v_i)};
    GemVal _t1368;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1365, 1), gem_string("table")))) {
        _t1368 = gem_eq(gem_type_fn(NULL, _t1365, 1), gem_string("table"));
    } else {
        GemVal _t1366 = gem_v_node;
        GemVal _t1367 = gem_table_get(gem_table_get(_t1366, gem_string("stmts")), gem_v_i);
        _t1368 = gem_eq(gem_table_get(_t1367, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t1368)) {
#line 293 "compiler/main.gem"
    GemVal _t1369 = gem_v_node;
    GemVal _t1370 = gem_table_get(gem_table_get(_t1369, gem_string("stmts")), gem_v_i);
    GemVal _t1371[] = {gem_v_d, gem_table_get(_t1370, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t1371, 2));
                                                    }
                                                }
                                                gem_pop_frame();
                                                return GEM_NIL;
                                            }
                                        } else {
#line 296 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 297 "compiler/main.gem"
    GemVal _t1372 = gem_v_node;
                                                if (gem_truthy(gem_neq(gem_table_get(_t1372, gem_string("value")), GEM_NIL))) {
#line 298 "compiler/main.gem"
    GemVal _t1373 = gem_v_node;
    GemVal _t1374[] = {gem_table_get(_t1373, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1375 = (*gem_v_collect_free_node);
                                                    GemVal _t1376 = _t1375.fn(_t1375.env, _t1374, 3);
                                                    gem_pop_frame();
                                                    return _t1376;
                                                } else {
                                                    gem_pop_frame();
                                                    return GEM_NIL;
                                                }
                                            } else {
                                                {
#line 302 "compiler/main.gem"
                                                    GemVal gem_v__for_tbl_19 = gem_v_node;
#line 302 "compiler/main.gem"
    GemVal _t1377[] = {gem_v__for_tbl_19};
                                                    GemVal gem_v__for_keys_19 = gem_keys_fn(NULL, _t1377, 1);
#line 302 "compiler/main.gem"
                                                    GemVal gem_v__for_i_19 = gem_int(0);
#line 302 "compiler/main.gem"
                                                    while (1) {
                                                        GemVal _t1378[] = {gem_v__for_keys_19};
                                                        if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_len_fn(NULL, _t1378, 1)))) break;
#line 302 "compiler/main.gem"
                                                        GemVal gem_v_nk = gem_table_get(gem_v__for_keys_19, gem_v__for_i_19);
#line 302 "compiler/main.gem"
                                                        GemVal gem_v_v = gem_table_get(gem_v__for_tbl_19, gem_table_get(gem_v__for_keys_19, gem_v__for_i_19));
#line 302 "compiler/main.gem"
                                                        gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 303 "compiler/main.gem"
                                                        if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 304 "compiler/main.gem"
    GemVal _t1379[] = {gem_v_v};
                                                            if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1379, 1), gem_string("table")))) {
#line 306 "compiler/main.gem"
    GemVal _t1380[] = {gem_v_v};
                                                                GemVal gem_v_vks = gem_keys_fn(NULL, _t1380, 1);
#line 307 "compiler/main.gem"
    GemVal _t1381[] = {gem_v_vks};
    GemVal _t1383;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1381, 1), gem_int(0)))) {
        _t1383 = gem_gt(gem_len_fn(NULL, _t1381, 1), gem_int(0));
    } else {
        GemVal _t1382[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1383 = gem_eq(gem_type_fn(NULL, _t1382, 1), gem_string("int"));
    }
                                                                if (gem_truthy(_t1383)) {
#line 309 "compiler/main.gem"
                                                                    GemVal gem_v_vi = gem_int(0);
#line 310 "compiler/main.gem"
                                                                    while (1) {
                                                                        GemVal _t1384[] = {gem_v_v};
                                                                        if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1384, 1)))) break;
#line 311 "compiler/main.gem"
    GemVal _t1385[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                        if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1385, 1), gem_string("table")))) {
#line 312 "compiler/main.gem"
    GemVal _t1386[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t1387 = (*gem_v_collect_free_node);
                                                                            (void)(_t1387.fn(_t1387.env, _t1386, 3));
                                                                        }
#line 314 "compiler/main.gem"
                                                                        gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                                    }
                                                                } else {
#line 317 "compiler/main.gem"
    GemVal _t1388[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t1389 = (*gem_v_collect_free_node);
                                                                    (void)(_t1389.fn(_t1389.env, _t1388, 3));
                                                                }
                                                            }
                                                        }
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
#line 326 "compiler/main.gem"
    GemVal _t1391 = gem_table_new();
    GemVal gem_v_free = _t1391;
#line 327 "compiler/main.gem"
    GemVal _t1392 = gem_table_new();
    GemVal _t1393[] = {gem_v_defined, _t1392};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t1393, 2);
#line 328 "compiler/main.gem"
    GemVal gem_v__for_i_21 = gem_int(0);
#line 328 "compiler/main.gem"
    GemVal _t1394[] = {gem_v_stmts};
    GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t1394, 1);
#line 328 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 328 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_21;
#line 328 "compiler/main.gem"
        gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 329 "compiler/main.gem"
    GemVal _t1395[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1396 = (*gem_v_collect_free_node);
        (void)(_t1396.fn(_t1396.env, _t1395, 3));
#line 330 "compiler/main.gem"
    GemVal _t1397[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1399;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1397, 1), gem_string("table")))) {
        _t1399 = gem_eq(gem_type_fn(NULL, _t1397, 1), gem_string("table"));
    } else {
        GemVal _t1398 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1399 = gem_eq(gem_table_get(_t1398, gem_string("tag")), gem_string("let"));
    }
        if (gem_truthy(_t1399)) {
#line 331 "compiler/main.gem"
    GemVal _t1400 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1401[] = {gem_v_d, gem_table_get(_t1400, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1401, 2));
        }
#line 333 "compiler/main.gem"
    GemVal _t1402[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1404;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1402, 1), gem_string("table")))) {
        _t1404 = gem_eq(gem_type_fn(NULL, _t1402, 1), gem_string("table"));
    } else {
        GemVal _t1403 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1404 = gem_eq(gem_table_get(_t1403, gem_string("tag")), gem_string("block"));
    }
        if (gem_truthy(_t1404)) {
#line 334 "compiler/main.gem"
            GemVal gem_v__for_i_20 = gem_int(0);
#line 334 "compiler/main.gem"
    GemVal _t1405 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1406[] = {gem_table_get(_t1405, gem_string("stmts"))};
            GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1406, 1);
#line 334 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 334 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_20;
#line 334 "compiler/main.gem"
                gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 335 "compiler/main.gem"
    GemVal _t1407 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1408[] = {gem_table_get(gem_table_get(_t1407, gem_string("stmts")), gem_v_j)};
    GemVal _t1411;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1408, 1), gem_string("table")))) {
        _t1411 = gem_eq(gem_type_fn(NULL, _t1408, 1), gem_string("table"));
    } else {
        GemVal _t1409 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1410 = gem_table_get(gem_table_get(_t1409, gem_string("stmts")), gem_v_j);
        _t1411 = gem_eq(gem_table_get(_t1410, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1411)) {
#line 336 "compiler/main.gem"
    GemVal _t1412 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1413 = gem_table_get(gem_table_get(_t1412, gem_string("stmts")), gem_v_j);
    GemVal _t1414[] = {gem_v_d, gem_table_get(_t1413, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1414, 2));
                }
            }

        }
    }

    GemVal _t1415 = gem_v_free;
    gem_pop_frame();
    return _t1415;
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
#line 350 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 351 "compiler/main.gem"
        GemVal _t1417 = GEM_NIL;
        gem_pop_frame();
        return _t1417;
    }
#line 353 "compiler/main.gem"
    GemVal _t1418[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1418, 1), gem_string("table")))) {
#line 354 "compiler/main.gem"
        GemVal _t1419 = GEM_NIL;
        gem_pop_frame();
        return _t1419;
    }
#line 356 "compiler/main.gem"
    GemVal _t1420 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1420, gem_string("tag"));
#line 357 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 358 "compiler/main.gem"
        GemVal _t1421 = GEM_NIL;
        gem_pop_frame();
        return _t1421;
    }
#line 361 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 362 "compiler/main.gem"
    GemVal _t1422 = gem_v_node;
    GemVal _t1423[] = {gem_table_get(_t1422, gem_string("params"))};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1423, 1);
#line 363 "compiler/main.gem"
    GemVal _t1424 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t1424, gem_string("rest_param")), GEM_NIL))) {
#line 364 "compiler/main.gem"
    GemVal _t1425 = gem_v_node;
    GemVal _t1426[] = {gem_v_inner_defined, gem_table_get(_t1425, gem_string("rest_param"))};
            (void)(gem_fn_set_add(NULL, _t1426, 2));
        }
#line 366 "compiler/main.gem"
    GemVal _t1427 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t1427, gem_string("block_param")), GEM_NIL))) {
#line 367 "compiler/main.gem"
    GemVal _t1428 = gem_v_node;
    GemVal _t1429[] = {gem_v_inner_defined, gem_table_get(_t1428, gem_string("block_param"))};
            (void)(gem_fn_set_add(NULL, _t1429, 2));
        }
#line 369 "compiler/main.gem"
    GemVal _t1430 = gem_v_node;
    GemVal _t1431[] = {gem_table_get(_t1430, gem_string("body")), gem_v_inner_defined};
    GemVal _t1432 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t1432.fn(_t1432.env, _t1431, 2);
#line 370 "compiler/main.gem"
    GemVal _t1433[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t1433, 2);
#line 371 "compiler/main.gem"
        GemVal gem_v__for_tbl_22 = gem_v_inter;
#line 371 "compiler/main.gem"
    GemVal _t1434[] = {gem_v__for_tbl_22};
        GemVal gem_v__for_keys_22 = gem_keys_fn(NULL, _t1434, 1);
#line 371 "compiler/main.gem"
        GemVal gem_v__for_i_22 = gem_int(0);
#line 371 "compiler/main.gem"
        while (1) {
            GemVal _t1435[] = {gem_v__for_keys_22};
            if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_len_fn(NULL, _t1435, 1)))) break;
#line 371 "compiler/main.gem"
            GemVal gem_v_ik = gem_table_get(gem_v__for_keys_22, gem_v__for_i_22);
#line 371 "compiler/main.gem"
            GemVal gem_v__ = gem_table_get(gem_v__for_tbl_22, gem_table_get(gem_v__for_keys_22, gem_v__for_i_22));
#line 371 "compiler/main.gem"
            gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 372 "compiler/main.gem"
    GemVal _t1436[] = {gem_v_captured, gem_v_ik};
            (void)(gem_fn_set_add(NULL, _t1436, 2));
        }

#line 374 "compiler/main.gem"
    GemVal _t1437 = gem_v_node;
    GemVal _t1438[] = {gem_v_scope_vars, gem_v_inner_defined};
    GemVal _t1439[] = {gem_table_get(_t1437, gem_string("body")), gem_fn_set_union(NULL, _t1438, 2), gem_v_captured};
    GemVal _t1440 = (*gem_v_walk_captures);
        GemVal _t1441 = _t1440.fn(_t1440.env, _t1439, 3);
        gem_pop_frame();
        return _t1441;
    } else {
#line 375 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
            gem_pop_frame();
            return GEM_NIL;
        } else {
#line 377 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 378 "compiler/main.gem"
    GemVal _t1442 = gem_v_node;
    GemVal _t1443[] = {gem_table_get(_t1442, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1444 = (*gem_v_walk_captures_node);
                (void)(_t1444.fn(_t1444.env, _t1443, 3));
#line 379 "compiler/main.gem"
    GemVal _t1445 = gem_v_node;
    GemVal _t1446[] = {gem_table_get(_t1445, gem_string("then")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1447 = (*gem_v_walk_captures);
                (void)(_t1447.fn(_t1447.env, _t1446, 3));
#line 380 "compiler/main.gem"
    GemVal _t1448 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t1448, gem_string("else")), GEM_NIL))) {
#line 381 "compiler/main.gem"
    GemVal _t1449 = gem_v_node;
    GemVal _t1450[] = {gem_table_get(_t1449, gem_string("else")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1451 = (*gem_v_walk_captures);
                    GemVal _t1452 = _t1451.fn(_t1451.env, _t1450, 3);
                    gem_pop_frame();
                    return _t1452;
                } else {
                    gem_pop_frame();
                    return GEM_NIL;
                }
            } else {
#line 383 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 384 "compiler/main.gem"
    GemVal _t1453 = gem_v_node;
    GemVal _t1454[] = {gem_table_get(_t1453, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1455 = (*gem_v_walk_captures_node);
                    (void)(_t1455.fn(_t1455.env, _t1454, 3));
#line 385 "compiler/main.gem"
    GemVal _t1456 = gem_v_node;
    GemVal _t1457[] = {gem_table_get(_t1456, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1458 = (*gem_v_walk_captures);
                    GemVal _t1459 = _t1458.fn(_t1458.env, _t1457, 3);
                    gem_pop_frame();
                    return _t1459;
                } else {
#line 386 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 387 "compiler/main.gem"
    GemVal _t1460 = gem_v_node;
    GemVal _t1461[] = {gem_table_get(_t1460, gem_string("target")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1462 = (*gem_v_walk_captures_node);
                        (void)(_t1462.fn(_t1462.env, _t1461, 3));
#line 388 "compiler/main.gem"
                        GemVal gem_v__for_i_23 = gem_int(0);
#line 388 "compiler/main.gem"
    GemVal _t1463 = gem_v_node;
    GemVal _t1464[] = {gem_table_get(_t1463, gem_string("whens"))};
                        GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t1464, 1);
#line 388 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 388 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_23;
#line 388 "compiler/main.gem"
                            gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 389 "compiler/main.gem"
    GemVal _t1465 = gem_v_node;
    GemVal _t1466 = gem_table_get(gem_table_get(_t1465, gem_string("whens")), gem_v_i);
    GemVal _t1467[] = {gem_table_get(_t1466, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1468 = (*gem_v_walk_captures_node);
                            (void)(_t1468.fn(_t1468.env, _t1467, 3));
#line 390 "compiler/main.gem"
    GemVal _t1469 = gem_v_node;
    GemVal _t1470 = gem_table_get(gem_table_get(_t1469, gem_string("whens")), gem_v_i);
    GemVal _t1471[] = {gem_table_get(_t1470, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1472 = (*gem_v_walk_captures);
                            (void)(_t1472.fn(_t1472.env, _t1471, 3));
                        }

#line 392 "compiler/main.gem"
    GemVal _t1473 = gem_v_node;
                        if (gem_truthy(gem_neq(gem_table_get(_t1473, gem_string("else")), GEM_NIL))) {
#line 393 "compiler/main.gem"
    GemVal _t1474 = gem_v_node;
    GemVal _t1475[] = {gem_table_get(_t1474, gem_string("else")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1476 = (*gem_v_walk_captures);
                            GemVal _t1477 = _t1476.fn(_t1476.env, _t1475, 3);
                            gem_pop_frame();
                            return _t1477;
                        } else {
                            gem_pop_frame();
                            return GEM_NIL;
                        }
                    } else {
#line 395 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 396 "compiler/main.gem"
    GemVal _t1478 = gem_v_node;
    GemVal _t1479[] = {gem_table_get(_t1478, gem_string("stmts")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1480 = (*gem_v_walk_captures);
                            GemVal _t1481 = _t1480.fn(_t1480.env, _t1479, 3);
                            gem_pop_frame();
                            return _t1481;
                        } else {
#line 397 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 398 "compiler/main.gem"
    GemVal _t1482 = gem_v_node;
                                if (gem_truthy(gem_neq(gem_table_get(_t1482, gem_string("value")), GEM_NIL))) {
#line 399 "compiler/main.gem"
    GemVal _t1483 = gem_v_node;
    GemVal _t1484[] = {gem_table_get(_t1483, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1485 = (*gem_v_walk_captures_node);
                                    GemVal _t1486 = _t1485.fn(_t1485.env, _t1484, 3);
                                    gem_pop_frame();
                                    return _t1486;
                                } else {
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 401 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 402 "compiler/main.gem"
    GemVal _t1487 = gem_v_node;
    GemVal _t1488[] = {gem_table_get(_t1487, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1489 = (*gem_v_walk_captures_node);
                                    GemVal _t1490 = _t1489.fn(_t1489.env, _t1488, 3);
                                    gem_pop_frame();
                                    return _t1490;
                                } else {
#line 403 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 404 "compiler/main.gem"
    GemVal _t1491 = gem_v_node;
    GemVal _t1492[] = {gem_table_get(_t1491, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1493 = (*gem_v_walk_captures_node);
                                        GemVal _t1494 = _t1493.fn(_t1493.env, _t1492, 3);
                                        gem_pop_frame();
                                        return _t1494;
                                    } else {
                                        {
#line 407 "compiler/main.gem"
                                            GemVal gem_v__for_tbl_24 = gem_v_node;
#line 407 "compiler/main.gem"
    GemVal _t1495[] = {gem_v__for_tbl_24};
                                            GemVal gem_v__for_keys_24 = gem_keys_fn(NULL, _t1495, 1);
#line 407 "compiler/main.gem"
                                            GemVal gem_v__for_i_24 = gem_int(0);
#line 407 "compiler/main.gem"
                                            while (1) {
                                                GemVal _t1496[] = {gem_v__for_keys_24};
                                                if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_len_fn(NULL, _t1496, 1)))) break;
#line 407 "compiler/main.gem"
                                                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_24, gem_v__for_i_24);
#line 407 "compiler/main.gem"
                                                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_24, gem_table_get(gem_v__for_keys_24, gem_v__for_i_24));
#line 407 "compiler/main.gem"
                                                gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 408 "compiler/main.gem"
                                                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 409 "compiler/main.gem"
    GemVal _t1497[] = {gem_v_v};
                                                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1497, 1), gem_string("table")))) {
#line 410 "compiler/main.gem"
    GemVal _t1498[] = {gem_v_v};
                                                        GemVal gem_v_vks = gem_keys_fn(NULL, _t1498, 1);
#line 411 "compiler/main.gem"
    GemVal _t1499[] = {gem_v_vks};
    GemVal _t1501;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1499, 1), gem_int(0)))) {
        _t1501 = gem_gt(gem_len_fn(NULL, _t1499, 1), gem_int(0));
    } else {
        GemVal _t1500[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1501 = gem_eq(gem_type_fn(NULL, _t1500, 1), gem_string("int"));
    }
                                                        if (gem_truthy(_t1501)) {
#line 412 "compiler/main.gem"
                                                            GemVal gem_v_vi = gem_int(0);
#line 413 "compiler/main.gem"
                                                            while (1) {
                                                                GemVal _t1502[] = {gem_v_v};
                                                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1502, 1)))) break;
#line 414 "compiler/main.gem"
    GemVal _t1503[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1503, 1), gem_string("table")))) {
#line 415 "compiler/main.gem"
    GemVal _t1504[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t1505 = (*gem_v_walk_captures_node);
                                                                    (void)(_t1505.fn(_t1505.env, _t1504, 3));
                                                                }
#line 417 "compiler/main.gem"
                                                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                            }
                                                        } else {
#line 420 "compiler/main.gem"
    GemVal _t1506[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t1507 = (*gem_v_walk_captures_node);
                                                            (void)(_t1507.fn(_t1507.env, _t1506, 3));
                                                        }
                                                    }
                                                }
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
#line 429 "compiler/main.gem"
    GemVal _t1509 = gem_table_new();
    GemVal _t1510[] = {gem_v_scope_vars, _t1509};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t1510, 2);
    {
#line 430 "compiler/main.gem"
        GemVal gem_v__for_i_26 = gem_int(0);
#line 430 "compiler/main.gem"
    GemVal _t1511[] = {gem_v_stmts};
        GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1511, 1);
#line 430 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 430 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_26;
#line 430 "compiler/main.gem"
            gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 431 "compiler/main.gem"
    GemVal _t1512[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t1513 = (*gem_v_walk_captures_node);
            (void)(_t1513.fn(_t1513.env, _t1512, 3));
#line 432 "compiler/main.gem"
    GemVal _t1514[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1516;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1514, 1), gem_string("table")))) {
        _t1516 = gem_eq(gem_type_fn(NULL, _t1514, 1), gem_string("table"));
    } else {
        GemVal _t1515 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1516 = gem_eq(gem_table_get(_t1515, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1516)) {
#line 433 "compiler/main.gem"
    GemVal _t1517 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1518[] = {gem_v_sv, gem_table_get(_t1517, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1518, 2));
            }
#line 435 "compiler/main.gem"
    GemVal _t1519[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1521;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1519, 1), gem_string("table")))) {
        _t1521 = gem_eq(gem_type_fn(NULL, _t1519, 1), gem_string("table"));
    } else {
        GemVal _t1520 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1521 = gem_eq(gem_table_get(_t1520, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1521)) {
#line 436 "compiler/main.gem"
                GemVal gem_v__for_i_25 = gem_int(0);
#line 436 "compiler/main.gem"
    GemVal _t1522 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1523[] = {gem_table_get(_t1522, gem_string("stmts"))};
                GemVal gem_v__for_limit_25 = gem_len_fn(NULL, _t1523, 1);
#line 436 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 436 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_25;
#line 436 "compiler/main.gem"
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 437 "compiler/main.gem"
    GemVal _t1524 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1525[] = {gem_table_get(gem_table_get(_t1524, gem_string("stmts")), gem_v_j)};
    GemVal _t1528;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1525, 1), gem_string("table")))) {
        _t1528 = gem_eq(gem_type_fn(NULL, _t1525, 1), gem_string("table"));
    } else {
        GemVal _t1526 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1527 = gem_table_get(gem_table_get(_t1526, gem_string("stmts")), gem_v_j);
        _t1528 = gem_eq(gem_table_get(_t1527, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1528)) {
#line 438 "compiler/main.gem"
    GemVal _t1529 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1530 = gem_table_get(gem_table_get(_t1529, gem_string("stmts")), gem_v_j);
    GemVal _t1531[] = {gem_v_sv, gem_table_get(_t1530, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1531, 2));
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
#line 446 "compiler/main.gem"
    GemVal _t1533 = gem_table_new();
    GemVal gem_v_captured = _t1533;
#line 447 "compiler/main.gem"
    GemVal _t1534[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t1535 = (*gem_v_walk_captures);
    (void)(_t1535.fn(_t1535.env, _t1534, 3));
    GemVal _t1536 = gem_v_captured;
    gem_pop_frame();
    return _t1536;
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
#line 474 "compiler/main.gem"
    GemVal _t1538 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1538, gem_string("tag"));
#line 476 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("int")))) {
#line 477 "compiler/main.gem"
    GemVal _t1539 = gem_table_new();
    GemVal _t1540 = gem_v_node;
    GemVal _t1541[] = {gem_table_get(_t1540, gem_string("value"))};
    gem_table_set(_t1539, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t1541, 1)), gem_string(")")));
    gem_table_set(_t1539, gem_string("setup"), gem_string(""));
        GemVal _t1542 = _t1539;
        gem_pop_frame();
        return _t1542;
    } else {
#line 478 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("float")))) {
#line 479 "compiler/main.gem"
    GemVal _t1543 = gem_table_new();
    GemVal _t1544 = gem_v_node;
    GemVal _t1545[] = {gem_table_get(_t1544, gem_string("value"))};
    GemVal _t1546[] = {gem_fn_format_float(NULL, _t1545, 1)};
    gem_table_set(_t1543, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t1546, 1)), gem_string(")")));
    gem_table_set(_t1543, gem_string("setup"), gem_string(""));
            GemVal _t1547 = _t1543;
            gem_pop_frame();
            return _t1547;
        } else {
#line 480 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("string")))) {
#line 481 "compiler/main.gem"
    GemVal _t1548 = gem_v_node;
    GemVal _t1549[] = {gem_table_get(_t1548, gem_string("value"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1549, 1);
#line 482 "compiler/main.gem"
    GemVal _t1550 = gem_table_new();
    GemVal _t1551[] = {gem_v_escaped};
    gem_table_set(_t1550, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t1551, 1)), gem_string("\")")));
    gem_table_set(_t1550, gem_string("setup"), gem_string(""));
                GemVal _t1552 = _t1550;
                gem_pop_frame();
                return _t1552;
            } else {
#line 483 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("interp")))) {
#line 484 "compiler/main.gem"
    GemVal _t1553 = gem_v_node;
                    GemVal gem_v_parts = gem_table_get(_t1553, gem_string("parts"));
#line 485 "compiler/main.gem"
    GemVal _t1554[] = {gem_v_parts};
                    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1554, 1), gem_int(0)))) {
#line 486 "compiler/main.gem"
    GemVal _t1555 = gem_table_new();
    gem_table_set(_t1555, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t1555, gem_string("setup"), gem_string(""));
                        GemVal _t1556 = _t1555;
                        gem_pop_frame();
                        return _t1556;
                    }
#line 488 "compiler/main.gem"
                    GemVal gem_v_setup = gem_string("");
#line 489 "compiler/main.gem"
    GemVal _t1557 = gem_table_new();
                    GemVal gem_v_compiled = _t1557;
#line 490 "compiler/main.gem"
                    GemVal gem_v__for_i_27 = gem_int(0);
#line 490 "compiler/main.gem"
    GemVal _t1558[] = {gem_v_parts};
                    GemVal gem_v__for_limit_27 = gem_len_fn(NULL, _t1558, 1);
#line 490 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 490 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_27;
#line 490 "compiler/main.gem"
                        gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 491 "compiler/main.gem"
                        GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 492 "compiler/main.gem"
    GemVal _t1559[] = {gem_v_part};
    GemVal _t1560 = (*gem_v_compile_expr);
                        GemVal gem_v_r = _t1560.fn(_t1560.env, _t1559, 1);
#line 493 "compiler/main.gem"
    GemVal _t1561 = gem_v_r;
                        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1561, gem_string("setup")));
#line 494 "compiler/main.gem"
    GemVal _t1562 = gem_v_part;
                        if (gem_truthy(gem_eq(gem_table_get(_t1562, gem_string("tag")), gem_string("string")))) {
#line 495 "compiler/main.gem"
    GemVal _t1563 = gem_v_r;
    GemVal _t1564[] = {gem_v_compiled, gem_table_get(_t1563, gem_string("expr"))};
                            (void)(gem_push_fn(NULL, _t1564, 2));
                        } else {
#line 497 "compiler/main.gem"
    GemVal _t1565 = (*gem_v_tmp);
                            GemVal gem_v_ts = _t1565.fn(_t1565.env, NULL, 0);
#line 498 "compiler/main.gem"
    GemVal _t1566[] = {gem_v_ts};
    GemVal _t1567 = gem_v_r;
    GemVal _t1568[] = {gem_table_get(_t1567, gem_string("expr"))};
                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1566, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1568, 1)), gem_string("};\n")));
#line 499 "compiler/main.gem"
    GemVal _t1569[] = {gem_v_ts};
    GemVal _t1570[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t1569, 1)), gem_string(", 1)"))};
                            (void)(gem_push_fn(NULL, _t1570, 2));
                        }
                    }

#line 502 "compiler/main.gem"
                    GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
#line 503 "compiler/main.gem"
                    GemVal gem_v__for_i_28 = gem_int(1);
#line 503 "compiler/main.gem"
    GemVal _t1571[] = {gem_v_compiled};
                    GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1571, 1);
#line 503 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 503 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_28;
#line 503 "compiler/main.gem"
                        gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 504 "compiler/main.gem"
    GemVal _t1572[] = {gem_v_acc};
    GemVal _t1573[] = {gem_table_get(gem_v_compiled, gem_v_i)};
                        gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1572, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1573, 1)), gem_string(")"));
                    }

#line 506 "compiler/main.gem"
    GemVal _t1574 = gem_table_new();
    gem_table_set(_t1574, gem_string("expr"), gem_v_acc);
    gem_table_set(_t1574, gem_string("setup"), gem_v_setup);
                    GemVal _t1575 = _t1574;
                    gem_pop_frame();
                    return _t1575;
                } else {
#line 507 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("bool")))) {
#line 508 "compiler/main.gem"
    GemVal _t1576 = gem_v_node;
                        if (gem_truthy(gem_table_get(_t1576, gem_string("value")))) {
#line 509 "compiler/main.gem"
    GemVal _t1577 = gem_table_new();
    gem_table_set(_t1577, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t1577, gem_string("setup"), gem_string(""));
                            GemVal _t1578 = _t1577;
                            gem_pop_frame();
                            return _t1578;
                        }
#line 511 "compiler/main.gem"
    GemVal _t1579 = gem_table_new();
    gem_table_set(_t1579, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t1579, gem_string("setup"), gem_string(""));
                        GemVal _t1580 = _t1579;
                        gem_pop_frame();
                        return _t1580;
                    } else {
#line 512 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("nil")))) {
#line 513 "compiler/main.gem"
    GemVal _t1581 = gem_table_new();
    gem_table_set(_t1581, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1581, gem_string("setup"), gem_string(""));
                            GemVal _t1582 = _t1581;
                            gem_pop_frame();
                            return _t1582;
                        } else {
#line 514 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 515 "compiler/main.gem"
    GemVal _t1583 = gem_v_node;
                                GemVal gem_v_name = gem_table_get(_t1583, gem_string("name"));
#line 516 "compiler/main.gem"
    GemVal _t1584[] = {(*gem_v_builtin_fns), gem_v_name};
                                if (gem_truthy(gem_has_key_fn(NULL, _t1584, 2))) {
#line 517 "compiler/main.gem"
    GemVal _t1585 = gem_table_new();
    GemVal _t1586[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name)};
    gem_table_set(_t1585, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1586, 1)), gem_string(", NULL)")));
    gem_table_set(_t1585, gem_string("setup"), gem_string(""));
                                    GemVal _t1587 = _t1585;
                                    gem_pop_frame();
                                    return _t1587;
                                } else {
#line 518 "compiler/main.gem"
    GemVal _t1588[] = {(*gem_v_defined_fns), gem_v_name};
                                    if (gem_truthy(gem_fn_set_contains(NULL, _t1588, 2))) {
#line 519 "compiler/main.gem"
    GemVal _t1589 = gem_table_new();
    GemVal _t1590[] = {gem_v_name};
    gem_table_set(_t1589, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t1590, 1)), gem_string(", NULL)")));
    gem_table_set(_t1589, gem_string("setup"), gem_string(""));
                                        GemVal _t1591 = _t1589;
                                        gem_pop_frame();
                                        return _t1591;
                                    }
                                }
#line 521 "compiler/main.gem"
    GemVal _t1592[] = {gem_v_name};
    GemVal _t1593 = (*gem_v_mangle);
                                GemVal gem_v_mname = _t1593.fn(_t1593.env, _t1592, 1);
#line 522 "compiler/main.gem"
    GemVal _t1594[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t1596;
    if (gem_truthy(gem_fn_set_contains(NULL, _t1594, 2))) {
        _t1596 = gem_fn_set_contains(NULL, _t1594, 2);
    } else {
        GemVal _t1595[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t1596 = gem_fn_set_contains(NULL, _t1595, 2);
    }
                                if (gem_truthy(_t1596)) {
#line 523 "compiler/main.gem"
    GemVal _t1597 = gem_table_new();
    GemVal _t1598[] = {gem_v_mname};
    gem_table_set(_t1597, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t1598, 1)), gem_string(")")));
    gem_table_set(_t1597, gem_string("setup"), gem_string(""));
                                    GemVal _t1599 = _t1597;
                                    gem_pop_frame();
                                    return _t1599;
                                }
#line 525 "compiler/main.gem"
    GemVal _t1600 = gem_table_new();
    gem_table_set(_t1600, gem_string("expr"), gem_v_mname);
    gem_table_set(_t1600, gem_string("setup"), gem_string(""));
                                GemVal _t1601 = _t1600;
                                gem_pop_frame();
                                return _t1601;
                            } else {
#line 526 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 527 "compiler/main.gem"
    GemVal _t1602[] = {gem_v_node};
    GemVal _t1603 = (*gem_v_compile_call);
                                    GemVal _t1604 = _t1603.fn(_t1603.env, _t1602, 1);
                                    gem_pop_frame();
                                    return _t1604;
                                } else {
#line 528 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("binop")))) {
#line 529 "compiler/main.gem"
    GemVal _t1605[] = {gem_v_node};
    GemVal _t1606 = (*gem_v_compile_binop);
                                        GemVal _t1607 = _t1606.fn(_t1606.env, _t1605, 1);
                                        gem_pop_frame();
                                        return _t1607;
                                    } else {
#line 530 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("unop")))) {
#line 531 "compiler/main.gem"
    GemVal _t1608 = gem_v_node;
    GemVal _t1609[] = {gem_table_get(_t1608, gem_string("expr"))};
    GemVal _t1610 = (*gem_v_compile_expr);
                                            GemVal gem_v_r = _t1610.fn(_t1610.env, _t1609, 1);
#line 532 "compiler/main.gem"
    GemVal _t1611 = gem_v_node;
                                            if (gem_truthy(gem_eq(gem_table_get(_t1611, gem_string("op")), gem_string("-")))) {
#line 533 "compiler/main.gem"
    GemVal _t1612 = gem_table_new();
    GemVal _t1613 = gem_v_r;
    GemVal _t1614[] = {gem_table_get(_t1613, gem_string("expr"))};
    gem_table_set(_t1612, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t1614, 1)), gem_string(")")));
    GemVal _t1615 = gem_v_r;
    gem_table_set(_t1612, gem_string("setup"), gem_table_get(_t1615, gem_string("setup")));
                                                GemVal _t1616 = _t1612;
                                                gem_pop_frame();
                                                return _t1616;
                                            } else {
#line 534 "compiler/main.gem"
    GemVal _t1617 = gem_v_node;
                                                if (gem_truthy(gem_eq(gem_table_get(_t1617, gem_string("op")), gem_string("not")))) {
#line 535 "compiler/main.gem"
    GemVal _t1618 = gem_table_new();
    GemVal _t1619 = gem_v_r;
    GemVal _t1620[] = {gem_table_get(_t1619, gem_string("expr"))};
    gem_table_set(_t1618, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t1620, 1)), gem_string(")")));
    GemVal _t1621 = gem_v_r;
    gem_table_set(_t1618, gem_string("setup"), gem_table_get(_t1621, gem_string("setup")));
                                                    GemVal _t1622 = _t1618;
                                                    gem_pop_frame();
                                                    return _t1622;
                                                }
                                            }
#line 537 "compiler/main.gem"
    GemVal _t1623 = gem_v_node;
    GemVal _t1624[] = {gem_table_get(_t1623, gem_string("op"))};
    GemVal _t1625[] = {gem_add(gem_string("unknown unary operator: "), gem_to_string_fn(NULL, _t1624, 1))};
                                            (void)(gem_error_at_fn("compiler/main.gem", 537, _t1625, 1));
                                        } else {
#line 538 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 539 "compiler/main.gem"
    GemVal _t1626[] = {gem_v_node};
    GemVal _t1627 = (*gem_v_compile_anon_fn);
                                                GemVal _t1628 = _t1627.fn(_t1627.env, _t1626, 1);
                                                gem_pop_frame();
                                                return _t1628;
                                            } else {
#line 540 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("table")))) {
#line 541 "compiler/main.gem"
    GemVal _t1629[] = {gem_v_node};
    GemVal _t1630 = (*gem_v_compile_table);
                                                    GemVal _t1631 = _t1630.fn(_t1630.env, _t1629, 1);
                                                    gem_pop_frame();
                                                    return _t1631;
                                                } else {
#line 542 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("array")))) {
#line 543 "compiler/main.gem"
    GemVal _t1632[] = {gem_v_node};
    GemVal _t1633 = (*gem_v_compile_array);
                                                        GemVal _t1634 = _t1633.fn(_t1633.env, _t1632, 1);
                                                        gem_pop_frame();
                                                        return _t1634;
                                                    } else {
#line 544 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot")))) {
#line 545 "compiler/main.gem"
    GemVal _t1635 = gem_v_node;
    GemVal _t1636[] = {gem_table_get(_t1635, gem_string("object"))};
    GemVal _t1637 = (*gem_v_compile_expr);
                                                            GemVal gem_v_r = _t1637.fn(_t1637.env, _t1636, 1);
#line 546 "compiler/main.gem"
    GemVal _t1638 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1638.fn(_t1638.env, NULL, 0);
#line 547 "compiler/main.gem"
    GemVal _t1639 = gem_v_r;
    GemVal _t1640[] = {gem_v_t};
    GemVal _t1641 = gem_v_r;
    GemVal _t1642[] = {gem_table_get(_t1641, gem_string("expr"))};
                                                            GemVal gem_v_setup = gem_add(gem_table_get(_t1639, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1640, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1642, 1)), gem_string(";\n")));
#line 548 "compiler/main.gem"
    GemVal _t1643 = gem_v_node;
    GemVal _t1644[] = {gem_table_get(_t1643, gem_string("field"))};
                                                            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1644, 1);
#line 549 "compiler/main.gem"
    GemVal _t1645 = gem_table_new();
    GemVal _t1646[] = {gem_v_t};
    GemVal _t1647[] = {gem_v_escaped};
    gem_table_set(_t1645, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1646, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1647, 1)), gem_string("\"))")));
    gem_table_set(_t1645, gem_string("setup"), gem_v_setup);
                                                            GemVal _t1648 = _t1645;
                                                            gem_pop_frame();
                                                            return _t1648;
                                                        } else {
#line 550 "compiler/main.gem"
                                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("index")))) {
#line 551 "compiler/main.gem"
    GemVal _t1649 = gem_v_node;
    GemVal _t1650[] = {gem_table_get(_t1649, gem_string("object"))};
    GemVal _t1651 = (*gem_v_compile_expr);
                                                                GemVal gem_v_obj_r = _t1651.fn(_t1651.env, _t1650, 1);
#line 552 "compiler/main.gem"
    GemVal _t1652 = gem_v_node;
    GemVal _t1653[] = {gem_table_get(_t1652, gem_string("key"))};
    GemVal _t1654 = (*gem_v_compile_expr);
                                                                GemVal gem_v_key_r = _t1654.fn(_t1654.env, _t1653, 1);
#line 553 "compiler/main.gem"
    GemVal _t1655 = gem_table_new();
    GemVal _t1656 = gem_v_obj_r;
    GemVal _t1657[] = {gem_table_get(_t1656, gem_string("expr"))};
    GemVal _t1658 = gem_v_key_r;
    GemVal _t1659[] = {gem_table_get(_t1658, gem_string("expr"))};
    gem_table_set(_t1655, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1657, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1659, 1)), gem_string(")")));
    GemVal _t1660 = gem_v_obj_r;
    GemVal _t1661 = gem_v_key_r;
    gem_table_set(_t1655, gem_string("setup"), gem_add(gem_table_get(_t1660, gem_string("setup")), gem_table_get(_t1661, gem_string("setup"))));
                                                                GemVal _t1662 = _t1655;
                                                                gem_pop_frame();
                                                                return _t1662;
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
    GemVal _t1663[] = {gem_v_tag};
    GemVal _t1664[] = {gem_add(gem_string("unknown expression node: "), gem_to_string_fn(NULL, _t1663, 1))};
    GemVal _t1665 = gem_error_at_fn("compiler/main.gem", 556, _t1664, 1);
    gem_pop_frame();
    return _t1665;
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
#line 562 "compiler/main.gem"
    GemVal _t1667 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t1667, gem_string("entries"));
#line 563 "compiler/main.gem"
    GemVal _t1668 = (*gem_v_tmp);
    GemVal gem_v_t = _t1668.fn(_t1668.env, NULL, 0);
#line 564 "compiler/main.gem"
    GemVal _t1669[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1669, 1)), gem_string(" = gem_table_new();\n"));
#line 565 "compiler/main.gem"
    GemVal gem_v__for_items_29 = gem_v_entries;
#line 565 "compiler/main.gem"
    GemVal gem_v__for_i_29 = gem_int(0);
#line 565 "compiler/main.gem"
    while (1) {
        GemVal _t1670[] = {gem_v__for_items_29};
        if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_len_fn(NULL, _t1670, 1)))) break;
#line 565 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_29, gem_v__for_i_29);
#line 565 "compiler/main.gem"
        gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 566 "compiler/main.gem"
    GemVal _t1671 = gem_v_entry;
    GemVal _t1672[] = {gem_table_get(_t1671, gem_string("value"))};
    GemVal _t1673 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t1673.fn(_t1673.env, _t1672, 1);
#line 567 "compiler/main.gem"
    GemVal _t1674 = gem_v_entry;
    GemVal _t1675[] = {gem_table_get(_t1674, gem_string("key"))};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1675, 1);
#line 568 "compiler/main.gem"
    GemVal _t1676 = gem_v_val_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1676, gem_string("setup")));
#line 569 "compiler/main.gem"
    GemVal _t1677[] = {gem_v_t};
    GemVal _t1678[] = {gem_v_escaped};
    GemVal _t1679 = gem_v_val_r;
    GemVal _t1680[] = {gem_table_get(_t1679, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1677, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1678, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1680, 1)), gem_string(");\n")));
    }

    GemVal _t1681 = gem_table_new();
    gem_table_set(_t1681, gem_string("expr"), gem_v_t);
    gem_table_set(_t1681, gem_string("setup"), gem_v_setup);
    GemVal _t1682 = _t1681;
    gem_pop_frame();
    return _t1682;
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
#line 577 "compiler/main.gem"
    GemVal _t1684 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t1684, gem_string("elements"));
#line 578 "compiler/main.gem"
    GemVal _t1685 = (*gem_v_tmp);
    GemVal gem_v_t = _t1685.fn(_t1685.env, NULL, 0);
#line 579 "compiler/main.gem"
    GemVal _t1686[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1686, 1)), gem_string(" = gem_table_new();\n"));
#line 580 "compiler/main.gem"
    GemVal gem_v__for_i_30 = gem_int(0);
#line 580 "compiler/main.gem"
    GemVal _t1687[] = {gem_v_elements};
    GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t1687, 1);
#line 580 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 580 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_30;
#line 580 "compiler/main.gem"
        gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 581 "compiler/main.gem"
    GemVal _t1688[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t1689 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t1689.fn(_t1689.env, _t1688, 1);
#line 582 "compiler/main.gem"
    GemVal _t1690 = gem_v_elem_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1690, gem_string("setup")));
#line 583 "compiler/main.gem"
    GemVal _t1691[] = {gem_v_t};
    GemVal _t1692[] = {gem_v_i};
    GemVal _t1693 = gem_v_elem_r;
    GemVal _t1694[] = {gem_table_get(_t1693, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1691, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t1692, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t1694, 1)), gem_string(");\n")));
    }

    GemVal _t1695 = gem_table_new();
    gem_table_set(_t1695, gem_string("expr"), gem_v_t);
    gem_table_set(_t1695, gem_string("setup"), gem_v_setup);
    GemVal _t1696 = _t1695;
    gem_pop_frame();
    return _t1696;
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
#line 591 "compiler/main.gem"
    GemVal _t1698 = gem_v_node;
    GemVal _t1699[] = {gem_table_get(_t1698, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1699, 1);
#line 592 "compiler/main.gem"
    GemVal _t1700 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1700, gem_string("rest_param")), GEM_NIL))) {
#line 593 "compiler/main.gem"
    GemVal _t1701 = gem_v_node;
    GemVal _t1702[] = {gem_v_inner_defined, gem_table_get(_t1701, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t1702, 2));
    }
#line 595 "compiler/main.gem"
    GemVal _t1703 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1703, gem_string("block_param")), GEM_NIL))) {
#line 596 "compiler/main.gem"
    GemVal _t1704 = gem_v_node;
    GemVal _t1705[] = {gem_v_inner_defined, gem_table_get(_t1704, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t1705, 2));
    }
#line 598 "compiler/main.gem"
    GemVal _t1706 = gem_v_node;
    GemVal _t1707[] = {gem_table_get(_t1706, gem_string("body")), gem_v_inner_defined};
    GemVal _t1708 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t1708.fn(_t1708.env, _t1707, 2);
#line 599 "compiler/main.gem"
    GemVal _t1709[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t1709, 1);
#line 601 "compiler/main.gem"
    GemVal _t1710[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1710, 1), gem_int(0)))) {
#line 602 "compiler/main.gem"
    GemVal _t1711 = gem_table_new();
    GemVal _t1712[] = {gem_v_node, _t1711};
    GemVal _t1713 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t1713.fn(_t1713.env, _t1712, 2);
#line 603 "compiler/main.gem"
    GemVal _t1714 = gem_table_new();
    GemVal _t1715 = gem_v_result;
    GemVal _t1716[] = {gem_table_get(_t1715, gem_string("fn_name"))};
    gem_table_set(_t1714, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1716, 1)), gem_string(", NULL)")));
    gem_table_set(_t1714, gem_string("setup"), gem_string(""));
        GemVal _t1717 = _t1714;
        gem_pop_frame();
        return _t1717;
    }
#line 606 "compiler/main.gem"
    GemVal _t1718[] = {gem_v_node, gem_v_captures};
    GemVal _t1719 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t1719.fn(_t1719.env, _t1718, 2);
#line 608 "compiler/main.gem"
    GemVal gem_v_setup = gem_string("");
#line 609 "compiler/main.gem"
    GemVal _t1720 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t1720.fn(_t1720.env, NULL, 0);
#line 610 "compiler/main.gem"
    GemVal _t1721 = gem_v_result;
    GemVal _t1722[] = {gem_table_get(_t1721, gem_string("struct_name"))};
    GemVal _t1723[] = {gem_v_env_tmp};
    GemVal _t1724 = gem_v_result;
    GemVal _t1725[] = {gem_table_get(_t1724, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t1722, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t1723, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t1725, 1)), gem_string("));\n")));
#line 611 "compiler/main.gem"
    GemVal gem_v__for_items_31 = gem_v_captures;
#line 611 "compiler/main.gem"
    GemVal gem_v__for_i_31 = gem_int(0);
#line 611 "compiler/main.gem"
    while (1) {
        GemVal _t1726[] = {gem_v__for_items_31};
        if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_len_fn(NULL, _t1726, 1)))) break;
#line 611 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_31, gem_v__for_i_31);
#line 611 "compiler/main.gem"
        gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
#line 612 "compiler/main.gem"
    GemVal _t1727[] = {gem_v_cap};
    GemVal _t1728 = (*gem_v_mangle);
        GemVal gem_v_mc = _t1728.fn(_t1728.env, _t1727, 1);
#line 613 "compiler/main.gem"
    GemVal _t1729[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1729, 2))) {
#line 614 "compiler/main.gem"
    GemVal _t1730[] = {gem_v_env_tmp};
    GemVal _t1731[] = {gem_v_mc};
    GemVal _t1732[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1730, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1731, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1732, 1)), gem_string(";\n")));
        } else {
#line 616 "compiler/main.gem"
    GemVal _t1733[] = {gem_v_env_tmp};
    GemVal _t1734[] = {gem_v_mc};
    GemVal _t1735[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1733, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1734, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t1735, 1)), gem_string(";\n")));
        }
    }

    GemVal _t1736 = gem_table_new();
    GemVal _t1737 = gem_v_result;
    GemVal _t1738[] = {gem_table_get(_t1737, gem_string("fn_name"))};
    GemVal _t1739[] = {gem_v_env_tmp};
    gem_table_set(_t1736, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1738, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1739, 1)), gem_string(")")));
    gem_table_set(_t1736, gem_string("setup"), gem_v_setup);
    GemVal _t1740 = _t1736;
    gem_pop_frame();
    return _t1740;
}

struct _closure__anon_34 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_34(void *_env, GemVal *args, int argc) {
    struct _closure__anon_34 *_cls = (struct _closure__anon_34 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_args = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_34", "compiler/main.gem", 0);
#line 626 "compiler/main.gem"
    GemVal gem_v_arg_setups = gem_string("");
#line 627 "compiler/main.gem"
    GemVal _t1742 = gem_table_new();
    GemVal gem_v_arg_exprs = _t1742;
#line 628 "compiler/main.gem"
    GemVal gem_v__for_i_32 = gem_int(0);
#line 628 "compiler/main.gem"
    GemVal _t1743[] = {gem_v_args};
    GemVal gem_v__for_limit_32 = gem_len_fn(NULL, _t1743, 1);
#line 628 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_v__for_limit_32))) break;
#line 628 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_32;
#line 628 "compiler/main.gem"
        gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 629 "compiler/main.gem"
    GemVal _t1744[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t1745 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t1745.fn(_t1745.env, _t1744, 1);
#line 630 "compiler/main.gem"
    GemVal _t1746 = gem_v_r;
        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1746, gem_string("setup")));
#line 631 "compiler/main.gem"
    GemVal _t1747 = gem_v_r;
    GemVal _t1748[] = {gem_v_arg_exprs, gem_table_get(_t1747, gem_string("expr"))};
        (void)(gem_push_fn(NULL, _t1748, 2));
    }

#line 633 "compiler/main.gem"
    GemVal _t1749[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t1749, 1);
#line 634 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 635 "compiler/main.gem"
    GemVal _t1750 = gem_table_new();
    gem_table_set(_t1750, gem_string("setup"), gem_v_arg_setups);
    gem_table_set(_t1750, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t1750, gem_string("argc"), gem_int(0));
        GemVal _t1751 = _t1750;
        gem_pop_frame();
        return _t1751;
    }
#line 637 "compiler/main.gem"
    GemVal _t1752 = (*gem_v_tmp);
    GemVal gem_v_t = _t1752.fn(_t1752.env, NULL, 0);
#line 638 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 639 "compiler/main.gem"
    GemVal gem_v__for_i_33 = gem_int(1);
#line 639 "compiler/main.gem"
    GemVal gem_v__for_limit_33 = gem_v_argc;
#line 639 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
#line 639 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_33;
#line 639 "compiler/main.gem"
        gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 640 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t1753 = gem_table_new();
    GemVal _t1754[] = {gem_v_t};
    GemVal _t1755[] = {gem_v_arr};
    gem_table_set(_t1753, gem_string("setup"), gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1754, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1755, 1)), gem_string("};\n"))));
    gem_table_set(_t1753, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t1753, gem_string("argc"), gem_v_argc);
    GemVal _t1756 = _t1753;
    gem_pop_frame();
    return _t1756;
}

struct _closure__anon_35 {
};
static GemVal _anon_35(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_35", "compiler/main.gem", 0);
#line 647 "compiler/main.gem"
    GemVal _t1758 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t1758, gem_string("argc")), gem_int(0)))) {
#line 648 "compiler/main.gem"
    GemVal _t1759 = gem_table_new();
    GemVal _t1760[] = {gem_v_fn_name};
    GemVal _t1761[] = {gem_v_env};
    gem_table_set(_t1759, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1760, 1), gem_string("(")), gem_to_string_fn(NULL, _t1761, 1)), gem_string(", NULL, 0)")));
    GemVal _t1762 = gem_v_ca;
    gem_table_set(_t1759, gem_string("setup"), gem_table_get(_t1762, gem_string("setup")));
        GemVal _t1763 = _t1759;
        gem_pop_frame();
        return _t1763;
    }
    GemVal _t1764 = gem_table_new();
    GemVal _t1765[] = {gem_v_fn_name};
    GemVal _t1766[] = {gem_v_env};
    GemVal _t1767 = gem_v_ca;
    GemVal _t1768[] = {gem_table_get(_t1767, gem_string("arr_name"))};
    GemVal _t1769 = gem_v_ca;
    GemVal _t1770[] = {gem_table_get(_t1769, gem_string("argc"))};
    gem_table_set(_t1764, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1765, 1), gem_string("(")), gem_to_string_fn(NULL, _t1766, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1768, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1770, 1)), gem_string(")")));
    GemVal _t1771 = gem_v_ca;
    gem_table_set(_t1764, gem_string("setup"), gem_table_get(_t1771, gem_string("setup")));
    GemVal _t1772 = _t1764;
    gem_pop_frame();
    return _t1772;
}

struct _closure__anon_36 {
    GemVal *gem_v_builtin_fns;
    GemVal *gem_v_compile_args;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_emit_direct_call;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tmp;
};
static GemVal _anon_36(void *_env, GemVal *args, int argc) {
    struct _closure__anon_36 *_cls = (struct _closure__anon_36 *)_env;
    GemVal *gem_v_builtin_fns = _cls->gem_v_builtin_fns;
    GemVal *gem_v_compile_args = _cls->gem_v_compile_args;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_emit_direct_call = _cls->gem_v_emit_direct_call;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_36", "compiler/main.gem", 0);
#line 656 "compiler/main.gem"
    GemVal gem_v__d34 = gem_v_node;
#line 656 "compiler/main.gem"
    GemVal _t1773 = gem_v__d34;
    GemVal gem_v_func = gem_table_get(_t1773, gem_string("func"));
#line 656 "compiler/main.gem"
    GemVal _t1774 = gem_v__d34;
    GemVal gem_v_args = gem_table_get(_t1774, gem_string("args"));

#line 659 "compiler/main.gem"
    GemVal _t1775 = gem_v_func;
    GemVal _t1777;
    if (!gem_truthy(gem_eq(gem_table_get(_t1775, gem_string("tag")), gem_string("dot")))) {
        _t1777 = gem_eq(gem_table_get(_t1775, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t1776 = gem_v_func;
        _t1777 = gem_eq(gem_table_get(_t1776, gem_string("field")), gem_string("len"));
    }
    GemVal _t1779;
    if (!gem_truthy(_t1777)) {
        _t1779 = _t1777;
    } else {
        GemVal _t1778[] = {gem_v_args};
        _t1779 = gem_eq(gem_len_fn(NULL, _t1778, 1), gem_int(0));
    }
    if (gem_truthy(_t1779)) {
#line 660 "compiler/main.gem"
    GemVal _t1780 = gem_v_func;
    GemVal _t1781[] = {gem_table_get(_t1780, gem_string("object"))};
    GemVal _t1782 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t1782.fn(_t1782.env, _t1781, 1);
#line 661 "compiler/main.gem"
    GemVal _t1783 = gem_table_new();
    GemVal _t1784 = gem_v_obj_r;
    GemVal _t1785[] = {gem_table_get(_t1784, gem_string("expr"))};
    gem_table_set(_t1783, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t1785, 1)), gem_string(")")));
    GemVal _t1786 = gem_v_obj_r;
    gem_table_set(_t1783, gem_string("setup"), gem_table_get(_t1786, gem_string("setup")));
        GemVal _t1787 = _t1783;
        gem_pop_frame();
        return _t1787;
    }
#line 665 "compiler/main.gem"
    GemVal _t1788 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t1788, gem_string("tag")), gem_string("var")))) {
#line 666 "compiler/main.gem"
    GemVal _t1789 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t1789, gem_string("name"));
#line 668 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 669 "compiler/main.gem"
    GemVal _t1790 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t1790, gem_string("line"));
#line 670 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 671 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 673 "compiler/main.gem"
    GemVal _t1791[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t1791, 1);
#line 674 "compiler/main.gem"
    GemVal _t1792[] = {gem_v_args};
    GemVal _t1793 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t1793.fn(_t1793.env, _t1792, 1);
#line 675 "compiler/main.gem"
    GemVal _t1794 = gem_v_ca;
            if (gem_truthy(gem_eq(gem_table_get(_t1794, gem_string("argc")), gem_int(0)))) {
#line 676 "compiler/main.gem"
    GemVal _t1795 = gem_table_new();
    GemVal _t1796[] = {gem_v_escaped_file};
    GemVal _t1797[] = {gem_v_line};
    gem_table_set(_t1795, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1796, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1797, 1)), gem_string(", NULL, 0)")));
    GemVal _t1798 = gem_v_ca;
    gem_table_set(_t1795, gem_string("setup"), gem_table_get(_t1798, gem_string("setup")));
                GemVal _t1799 = _t1795;
                gem_pop_frame();
                return _t1799;
            }
#line 678 "compiler/main.gem"
    GemVal _t1800 = gem_table_new();
    GemVal _t1801[] = {gem_v_escaped_file};
    GemVal _t1802[] = {gem_v_line};
    GemVal _t1803 = gem_v_ca;
    GemVal _t1804[] = {gem_table_get(_t1803, gem_string("arr_name"))};
    GemVal _t1805 = gem_v_ca;
    GemVal _t1806[] = {gem_table_get(_t1805, gem_string("argc"))};
    gem_table_set(_t1800, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1801, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1802, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1804, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1806, 1)), gem_string(")")));
    GemVal _t1807 = gem_v_ca;
    gem_table_set(_t1800, gem_string("setup"), gem_table_get(_t1807, gem_string("setup")));
            GemVal _t1808 = _t1800;
            gem_pop_frame();
            return _t1808;
        } else {
#line 679 "compiler/main.gem"
    GemVal _t1809[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t1809, 2))) {
#line 680 "compiler/main.gem"
    GemVal _t1810[] = {gem_v_args};
    GemVal _t1811 = (*gem_v_compile_args);
    GemVal _t1812[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t1811.fn(_t1811.env, _t1810, 1)};
    GemVal _t1813 = (*gem_v_emit_direct_call);
                GemVal _t1814 = _t1813.fn(_t1813.env, _t1812, 3);
                gem_pop_frame();
                return _t1814;
            } else {
#line 681 "compiler/main.gem"
    GemVal _t1815[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t1815, 2))) {
#line 682 "compiler/main.gem"
    GemVal _t1816[] = {gem_v_name};
    GemVal _t1817[] = {gem_v_args};
    GemVal _t1818 = (*gem_v_compile_args);
    GemVal _t1819[] = {gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1816, 1)), gem_string("NULL"), _t1818.fn(_t1818.env, _t1817, 1)};
    GemVal _t1820 = (*gem_v_emit_direct_call);
                    GemVal _t1821 = _t1820.fn(_t1820.env, _t1819, 3);
                    gem_pop_frame();
                    return _t1821;
                }
            }
        }
    }
#line 687 "compiler/main.gem"
    GemVal _t1822[] = {gem_v_func};
    GemVal _t1823 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t1823.fn(_t1823.env, _t1822, 1);
#line 688 "compiler/main.gem"
    GemVal _t1824[] = {gem_v_args};
    GemVal _t1825 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t1825.fn(_t1825.env, _t1824, 1);
#line 689 "compiler/main.gem"
    GemVal _t1826 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t1826.fn(_t1826.env, NULL, 0);
#line 690 "compiler/main.gem"
    GemVal _t1827 = gem_v_func_r;
    GemVal _t1828 = gem_v_ca;
    GemVal _t1829[] = {gem_v_tmp_fn};
    GemVal _t1830 = gem_v_func_r;
    GemVal _t1831[] = {gem_table_get(_t1830, gem_string("expr"))};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get(_t1827, gem_string("setup")), gem_table_get(_t1828, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1829, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1831, 1)), gem_string(";\n")));
#line 691 "compiler/main.gem"
    GemVal _t1832 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t1832, gem_string("argc")), gem_int(0)))) {
#line 692 "compiler/main.gem"
    GemVal _t1833 = gem_table_new();
    GemVal _t1834[] = {gem_v_tmp_fn};
    GemVal _t1835[] = {gem_v_tmp_fn};
    gem_table_set(_t1833, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1834, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1835, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t1833, gem_string("setup"), gem_v_setup);
        GemVal _t1836 = _t1833;
        gem_pop_frame();
        return _t1836;
    }
    GemVal _t1837 = gem_table_new();
    GemVal _t1838[] = {gem_v_tmp_fn};
    GemVal _t1839[] = {gem_v_tmp_fn};
    GemVal _t1840 = gem_v_ca;
    GemVal _t1841[] = {gem_table_get(_t1840, gem_string("arr_name"))};
    GemVal _t1842 = gem_v_ca;
    GemVal _t1843[] = {gem_table_get(_t1842, gem_string("argc"))};
    gem_table_set(_t1837, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1838, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1839, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t1841, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1843, 1)), gem_string(")")));
    gem_table_set(_t1837, gem_string("setup"), gem_v_setup);
    GemVal _t1844 = _t1837;
    gem_pop_frame();
    return _t1844;
}

struct _closure__anon_37 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_37(void *_env, GemVal *args, int argc) {
    struct _closure__anon_37 *_cls = (struct _closure__anon_37 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_37", "compiler/main.gem", 0);
#line 700 "compiler/main.gem"
    GemVal _t1846 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t1846, gem_string("op"));
#line 701 "compiler/main.gem"
    GemVal _t1847 = gem_v_node;
    GemVal _t1848[] = {gem_table_get(_t1847, gem_string("left"))};
    GemVal _t1849 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t1849.fn(_t1849.env, _t1848, 1);
#line 702 "compiler/main.gem"
    GemVal _t1850 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t1850, gem_string("expr"));
#line 703 "compiler/main.gem"
    GemVal _t1851 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t1851, gem_string("setup"));
#line 705 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 706 "compiler/main.gem"
    GemVal _t1852 = gem_v_node;
    GemVal _t1853[] = {gem_table_get(_t1852, gem_string("right"))};
    GemVal _t1854 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t1854.fn(_t1854.env, _t1853, 1);
#line 707 "compiler/main.gem"
    GemVal _t1855 = gem_table_new();
    GemVal _t1856[] = {gem_v_lc};
    GemVal _t1857 = gem_v_rc_r;
    GemVal _t1858[] = {gem_table_get(_t1857, gem_string("expr"))};
    gem_table_set(_t1855, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1856, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1858, 1)), gem_string(")")));
    GemVal _t1859 = gem_v_rc_r;
    gem_table_set(_t1855, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1859, gem_string("setup"))));
        GemVal _t1860 = _t1855;
        gem_pop_frame();
        return _t1860;
    } else {
#line 708 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 709 "compiler/main.gem"
    GemVal _t1861 = gem_v_node;
    GemVal _t1862[] = {gem_table_get(_t1861, gem_string("right"))};
    GemVal _t1863 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t1863.fn(_t1863.env, _t1862, 1);
#line 710 "compiler/main.gem"
    GemVal _t1864 = gem_table_new();
    GemVal _t1865[] = {gem_v_lc};
    GemVal _t1866 = gem_v_rc_r;
    GemVal _t1867[] = {gem_table_get(_t1866, gem_string("expr"))};
    gem_table_set(_t1864, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t1865, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1867, 1)), gem_string(")")));
    GemVal _t1868 = gem_v_rc_r;
    gem_table_set(_t1864, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1868, gem_string("setup"))));
            GemVal _t1869 = _t1864;
            gem_pop_frame();
            return _t1869;
        } else {
#line 711 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 712 "compiler/main.gem"
    GemVal _t1870 = gem_v_node;
    GemVal _t1871[] = {gem_table_get(_t1870, gem_string("right"))};
    GemVal _t1872 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t1872.fn(_t1872.env, _t1871, 1);
#line 713 "compiler/main.gem"
    GemVal _t1873 = gem_table_new();
    GemVal _t1874[] = {gem_v_lc};
    GemVal _t1875 = gem_v_rc_r;
    GemVal _t1876[] = {gem_table_get(_t1875, gem_string("expr"))};
    gem_table_set(_t1873, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t1874, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1876, 1)), gem_string(")")));
    GemVal _t1877 = gem_v_rc_r;
    gem_table_set(_t1873, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1877, gem_string("setup"))));
                GemVal _t1878 = _t1873;
                gem_pop_frame();
                return _t1878;
            } else {
#line 714 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 715 "compiler/main.gem"
    GemVal _t1879 = gem_v_node;
    GemVal _t1880[] = {gem_table_get(_t1879, gem_string("right"))};
    GemVal _t1881 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t1881.fn(_t1881.env, _t1880, 1);
#line 716 "compiler/main.gem"
    GemVal _t1882 = gem_table_new();
    GemVal _t1883[] = {gem_v_lc};
    GemVal _t1884 = gem_v_rc_r;
    GemVal _t1885[] = {gem_table_get(_t1884, gem_string("expr"))};
    gem_table_set(_t1882, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t1883, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1885, 1)), gem_string(")")));
    GemVal _t1886 = gem_v_rc_r;
    gem_table_set(_t1882, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1886, gem_string("setup"))));
                    GemVal _t1887 = _t1882;
                    gem_pop_frame();
                    return _t1887;
                } else {
#line 717 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 718 "compiler/main.gem"
    GemVal _t1888 = gem_v_node;
    GemVal _t1889[] = {gem_table_get(_t1888, gem_string("right"))};
    GemVal _t1890 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t1890.fn(_t1890.env, _t1889, 1);
#line 719 "compiler/main.gem"
    GemVal _t1891 = gem_table_new();
    GemVal _t1892[] = {gem_v_lc};
    GemVal _t1893 = gem_v_rc_r;
    GemVal _t1894[] = {gem_table_get(_t1893, gem_string("expr"))};
    gem_table_set(_t1891, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t1892, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1894, 1)), gem_string(")")));
    GemVal _t1895 = gem_v_rc_r;
    gem_table_set(_t1891, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1895, gem_string("setup"))));
                        GemVal _t1896 = _t1891;
                        gem_pop_frame();
                        return _t1896;
                    } else {
#line 720 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 721 "compiler/main.gem"
    GemVal _t1897 = gem_v_node;
    GemVal _t1898[] = {gem_table_get(_t1897, gem_string("right"))};
    GemVal _t1899 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t1899.fn(_t1899.env, _t1898, 1);
#line 722 "compiler/main.gem"
    GemVal _t1900 = (*gem_v_tmp);
                            GemVal gem_v_t = _t1900.fn(_t1900.env, NULL, 0);
#line 723 "compiler/main.gem"
    GemVal _t1901 = gem_v_rc_r;
    GemVal _t1902[] = {gem_v_t};
    GemVal _t1903 = gem_v_rc_r;
    GemVal _t1904[] = {gem_table_get(_t1903, gem_string("expr"))};
    GemVal _t1905[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t1901, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1902, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1904, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1905, 1)), gem_string("};\n")));
#line 724 "compiler/main.gem"
    GemVal _t1906 = gem_table_new();
    GemVal _t1907[] = {gem_v_t};
    gem_table_set(_t1906, gem_string("expr"), gem_add(gem_add(gem_string("gem_has_key_fn(NULL, "), gem_to_string_fn(NULL, _t1907, 1)), gem_string(", 2)")));
    gem_table_set(_t1906, gem_string("setup"), gem_v_setup);
                            GemVal _t1908 = _t1906;
                            gem_pop_frame();
                            return _t1908;
                        } else {
#line 725 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 726 "compiler/main.gem"
    GemVal _t1909 = gem_v_node;
    GemVal _t1910[] = {gem_table_get(_t1909, gem_string("right"))};
    GemVal _t1911 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t1911.fn(_t1911.env, _t1910, 1);
#line 727 "compiler/main.gem"
    GemVal _t1912 = gem_table_new();
    GemVal _t1913[] = {gem_v_lc};
    GemVal _t1914 = gem_v_rc_r;
    GemVal _t1915[] = {gem_table_get(_t1914, gem_string("expr"))};
    gem_table_set(_t1912, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t1913, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1915, 1)), gem_string(")")));
    GemVal _t1916 = gem_v_rc_r;
    gem_table_set(_t1912, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1916, gem_string("setup"))));
                                GemVal _t1917 = _t1912;
                                gem_pop_frame();
                                return _t1917;
                            } else {
#line 728 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 729 "compiler/main.gem"
    GemVal _t1918 = gem_v_node;
    GemVal _t1919[] = {gem_table_get(_t1918, gem_string("right"))};
    GemVal _t1920 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t1920.fn(_t1920.env, _t1919, 1);
#line 730 "compiler/main.gem"
    GemVal _t1921 = gem_table_new();
    GemVal _t1922[] = {gem_v_lc};
    GemVal _t1923 = gem_v_rc_r;
    GemVal _t1924[] = {gem_table_get(_t1923, gem_string("expr"))};
    gem_table_set(_t1921, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t1922, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1924, 1)), gem_string(")")));
    GemVal _t1925 = gem_v_rc_r;
    gem_table_set(_t1921, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1925, gem_string("setup"))));
                                    GemVal _t1926 = _t1921;
                                    gem_pop_frame();
                                    return _t1926;
                                } else {
#line 731 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 732 "compiler/main.gem"
    GemVal _t1927 = gem_v_node;
    GemVal _t1928[] = {gem_table_get(_t1927, gem_string("right"))};
    GemVal _t1929 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t1929.fn(_t1929.env, _t1928, 1);
#line 733 "compiler/main.gem"
    GemVal _t1930 = gem_table_new();
    GemVal _t1931[] = {gem_v_lc};
    GemVal _t1932 = gem_v_rc_r;
    GemVal _t1933[] = {gem_table_get(_t1932, gem_string("expr"))};
    gem_table_set(_t1930, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t1931, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1933, 1)), gem_string(")")));
    GemVal _t1934 = gem_v_rc_r;
    gem_table_set(_t1930, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1934, gem_string("setup"))));
                                        GemVal _t1935 = _t1930;
                                        gem_pop_frame();
                                        return _t1935;
                                    } else {
#line 734 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 735 "compiler/main.gem"
    GemVal _t1936 = gem_v_node;
    GemVal _t1937[] = {gem_table_get(_t1936, gem_string("right"))};
    GemVal _t1938 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t1938.fn(_t1938.env, _t1937, 1);
#line 736 "compiler/main.gem"
    GemVal _t1939 = gem_table_new();
    GemVal _t1940[] = {gem_v_lc};
    GemVal _t1941 = gem_v_rc_r;
    GemVal _t1942[] = {gem_table_get(_t1941, gem_string("expr"))};
    gem_table_set(_t1939, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t1940, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1942, 1)), gem_string(")")));
    GemVal _t1943 = gem_v_rc_r;
    gem_table_set(_t1939, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1943, gem_string("setup"))));
                                            GemVal _t1944 = _t1939;
                                            gem_pop_frame();
                                            return _t1944;
                                        } else {
#line 737 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 738 "compiler/main.gem"
    GemVal _t1945 = gem_v_node;
    GemVal _t1946[] = {gem_table_get(_t1945, gem_string("right"))};
    GemVal _t1947 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t1947.fn(_t1947.env, _t1946, 1);
#line 739 "compiler/main.gem"
    GemVal _t1948 = gem_table_new();
    GemVal _t1949[] = {gem_v_lc};
    GemVal _t1950 = gem_v_rc_r;
    GemVal _t1951[] = {gem_table_get(_t1950, gem_string("expr"))};
    gem_table_set(_t1948, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t1949, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1951, 1)), gem_string(")")));
    GemVal _t1952 = gem_v_rc_r;
    gem_table_set(_t1948, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1952, gem_string("setup"))));
                                                GemVal _t1953 = _t1948;
                                                gem_pop_frame();
                                                return _t1953;
                                            } else {
#line 740 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 741 "compiler/main.gem"
    GemVal _t1954 = gem_v_node;
    GemVal _t1955[] = {gem_table_get(_t1954, gem_string("right"))};
    GemVal _t1956 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t1956.fn(_t1956.env, _t1955, 1);
#line 742 "compiler/main.gem"
    GemVal _t1957 = gem_table_new();
    GemVal _t1958[] = {gem_v_lc};
    GemVal _t1959 = gem_v_rc_r;
    GemVal _t1960[] = {gem_table_get(_t1959, gem_string("expr"))};
    gem_table_set(_t1957, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t1958, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1960, 1)), gem_string(")")));
    GemVal _t1961 = gem_v_rc_r;
    gem_table_set(_t1957, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1961, gem_string("setup"))));
                                                    GemVal _t1962 = _t1957;
                                                    gem_pop_frame();
                                                    return _t1962;
                                                } else {
#line 743 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 744 "compiler/main.gem"
    GemVal _t1963 = gem_v_node;
    GemVal _t1964[] = {gem_table_get(_t1963, gem_string("right"))};
    GemVal _t1965 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t1965.fn(_t1965.env, _t1964, 1);
#line 745 "compiler/main.gem"
    GemVal _t1966 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1966.fn(_t1966.env, NULL, 0);
#line 746 "compiler/main.gem"
                                                        GemVal gem_v_setup = gem_v_ls;
#line 747 "compiler/main.gem"
    GemVal _t1967[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1967, 1)), gem_string(";\n")));
#line 748 "compiler/main.gem"
    GemVal _t1968[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t1968, 1)), gem_string(")) {\n")));
#line 749 "compiler/main.gem"
    GemVal _t1969[] = {gem_v_t};
    GemVal _t1970[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1969, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1970, 1)), gem_string(";\n")));
#line 750 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 751 "compiler/main.gem"
    GemVal _t1971 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t1971, gem_string("setup")), gem_string("")))) {
#line 752 "compiler/main.gem"
    GemVal _t1972 = gem_v_rc_r;
    GemVal _t1973[] = {gem_table_get(_t1972, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t1973, 3));
                                                        }
#line 754 "compiler/main.gem"
    GemVal _t1974[] = {gem_v_t};
    GemVal _t1975 = gem_v_rc_r;
    GemVal _t1976[] = {gem_table_get(_t1975, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1974, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1976, 1)), gem_string(";\n")));
#line 755 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 756 "compiler/main.gem"
    GemVal _t1977 = gem_table_new();
    gem_table_set(_t1977, gem_string("expr"), gem_v_t);
    gem_table_set(_t1977, gem_string("setup"), gem_v_setup);
                                                        GemVal _t1978 = _t1977;
                                                        gem_pop_frame();
                                                        return _t1978;
                                                    } else {
#line 757 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 758 "compiler/main.gem"
    GemVal _t1979 = gem_v_node;
    GemVal _t1980[] = {gem_table_get(_t1979, gem_string("right"))};
    GemVal _t1981 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t1981.fn(_t1981.env, _t1980, 1);
#line 759 "compiler/main.gem"
    GemVal _t1982 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1982.fn(_t1982.env, NULL, 0);
#line 760 "compiler/main.gem"
                                                            GemVal gem_v_setup = gem_v_ls;
#line 761 "compiler/main.gem"
    GemVal _t1983[] = {gem_v_t};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1983, 1)), gem_string(";\n")));
#line 762 "compiler/main.gem"
    GemVal _t1984[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t1984, 1)), gem_string(")) {\n")));
#line 763 "compiler/main.gem"
    GemVal _t1985[] = {gem_v_t};
    GemVal _t1986[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1985, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1986, 1)), gem_string(";\n")));
#line 764 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 765 "compiler/main.gem"
    GemVal _t1987 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t1987, gem_string("setup")), gem_string("")))) {
#line 766 "compiler/main.gem"
    GemVal _t1988 = gem_v_rc_r;
    GemVal _t1989[] = {gem_table_get(_t1988, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                                gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t1989, 3));
                                                            }
#line 768 "compiler/main.gem"
    GemVal _t1990[] = {gem_v_t};
    GemVal _t1991 = gem_v_rc_r;
    GemVal _t1992[] = {gem_table_get(_t1991, gem_string("expr"))};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1990, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1992, 1)), gem_string(";\n")));
#line 769 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 770 "compiler/main.gem"
    GemVal _t1993 = gem_table_new();
    gem_table_set(_t1993, gem_string("expr"), gem_v_t);
    gem_table_set(_t1993, gem_string("setup"), gem_v_setup);
                                                            GemVal _t1994 = _t1993;
                                                            gem_pop_frame();
                                                            return _t1994;
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
#line 773 "compiler/main.gem"
    GemVal _t1995[] = {gem_v_op};
    GemVal _t1996[] = {gem_add(gem_string("unknown binary operator: "), gem_to_string_fn(NULL, _t1995, 1))};
    GemVal _t1997 = gem_error_at_fn("compiler/main.gem", 773, _t1996, 1);
    gem_pop_frame();
    return _t1997;
}

struct _closure__anon_38 {
    GemVal *gem_v_compile_stmt;
};
static GemVal _anon_38(void *_env, GemVal *args, int argc) {
    struct _closure__anon_38 *_cls = (struct _closure__anon_38 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_38", "compiler/main.gem", 0);
#line 779 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 780 "compiler/main.gem"
    GemVal gem_v__for_items_35 = gem_v_stmts;
#line 780 "compiler/main.gem"
    GemVal gem_v__for_i_35 = gem_int(0);
#line 780 "compiler/main.gem"
    while (1) {
        GemVal _t1999[] = {gem_v__for_items_35};
        if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_len_fn(NULL, _t1999, 1)))) break;
#line 780 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_35, gem_v__for_i_35);
#line 780 "compiler/main.gem"
        gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 781 "compiler/main.gem"
    GemVal _t2000[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2001 = (*gem_v_compile_stmt);
        gem_v_out = gem_add(gem_add(gem_v_out, _t2001.fn(_t2001.env, _t2000, 2)), gem_string("\n"));
    }

    GemVal _t2002 = gem_v_out;
    gem_pop_frame();
    return _t2002;
}

struct _closure__anon_39 {
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
static GemVal _anon_39(void *_env, GemVal *args, int argc) {
    struct _closure__anon_39 *_cls = (struct _closure__anon_39 *)_env;
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
    gem_push_frame("_anon_39", "compiler/main.gem", 0);
#line 789 "compiler/main.gem"
    GemVal _t2004[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2004, 1);
#line 790 "compiler/main.gem"
    GemVal _t2005 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t2005, gem_string("tag"));
#line 791 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 792 "compiler/main.gem"
    GemVal _t2006 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2006, gem_string("line")), GEM_NIL))) {
#line 793 "compiler/main.gem"
    GemVal _t2007 = gem_v_node;
    GemVal _t2008[] = {gem_table_get(_t2007, gem_string("line"))};
    GemVal _t2009[] = {(*gem_v_source_name)};
    GemVal _t2010[] = {gem_fn_escape_c_string(NULL, _t2009, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2008, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2010, 1)), gem_string("\"\n"));
    }
#line 796 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 797 "compiler/main.gem"
    GemVal _t2011 = gem_v_node;
    GemVal _t2012[] = {gem_table_get(_t2011, gem_string("value"))};
    GemVal _t2013 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2013.fn(_t2013.env, _t2012, 1);
#line 798 "compiler/main.gem"
    GemVal _t2014 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t2014, gem_string("name"));
#line 799 "compiler/main.gem"
    GemVal _t2015[] = {gem_v_name};
    GemVal _t2016 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2016.fn(_t2016.env, _t2015, 1);
#line 800 "compiler/main.gem"
    GemVal _t2018;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t2018 = (*gem_v_in_top_level);
    } else {
        GemVal _t2017[] = {(*gem_v_top_level_vars), gem_v_name};
        _t2018 = gem_fn_set_contains(NULL, _t2017, 2);
    }
        if (gem_truthy(_t2018)) {
#line 801 "compiler/main.gem"
    GemVal _t2019[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2019, 2))) {
#line 802 "compiler/main.gem"
    GemVal _t2020[] = {gem_v_line_dir};
    GemVal _t2021 = gem_v_r;
    GemVal _t2022[] = {gem_table_get(_t2021, gem_string("setup"))};
    GemVal _t2023[] = {gem_v_p};
    GemVal _t2024[] = {gem_v_mname};
    GemVal _t2025[] = {gem_v_p};
    GemVal _t2026[] = {gem_v_mname};
    GemVal _t2027 = gem_v_r;
    GemVal _t2028[] = {gem_table_get(_t2027, gem_string("expr"))};
                GemVal _t2029 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2020, 1), gem_to_string_fn(NULL, _t2022, 1)), gem_to_string_fn(NULL, _t2023, 1)), gem_to_string_fn(NULL, _t2024, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2025, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2026, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2028, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2029;
            }
#line 804 "compiler/main.gem"
    GemVal _t2030[] = {gem_v_line_dir};
    GemVal _t2031 = gem_v_r;
    GemVal _t2032[] = {gem_table_get(_t2031, gem_string("setup"))};
    GemVal _t2033[] = {gem_v_p};
    GemVal _t2034[] = {gem_v_mname};
    GemVal _t2035 = gem_v_r;
    GemVal _t2036[] = {gem_table_get(_t2035, gem_string("expr"))};
            GemVal _t2037 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2030, 1), gem_to_string_fn(NULL, _t2032, 1)), gem_to_string_fn(NULL, _t2033, 1)), gem_to_string_fn(NULL, _t2034, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2036, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2037;
        }
#line 806 "compiler/main.gem"
    GemVal _t2038[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2038, 2))) {
#line 807 "compiler/main.gem"
    GemVal _t2039[] = {gem_v_line_dir};
    GemVal _t2040 = gem_v_r;
    GemVal _t2041[] = {gem_table_get(_t2040, gem_string("setup"))};
    GemVal _t2042[] = {gem_v_p};
    GemVal _t2043[] = {gem_v_mname};
    GemVal _t2044[] = {gem_v_p};
    GemVal _t2045[] = {gem_v_mname};
    GemVal _t2046 = gem_v_r;
    GemVal _t2047[] = {gem_table_get(_t2046, gem_string("expr"))};
            GemVal _t2048 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2039, 1), gem_to_string_fn(NULL, _t2041, 1)), gem_to_string_fn(NULL, _t2042, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2043, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2044, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2045, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2047, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2048;
        }
#line 809 "compiler/main.gem"
    GemVal _t2049[] = {gem_v_line_dir};
    GemVal _t2050 = gem_v_r;
    GemVal _t2051[] = {gem_table_get(_t2050, gem_string("setup"))};
    GemVal _t2052[] = {gem_v_p};
    GemVal _t2053[] = {gem_v_mname};
    GemVal _t2054 = gem_v_r;
    GemVal _t2055[] = {gem_table_get(_t2054, gem_string("expr"))};
        GemVal _t2056 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2049, 1), gem_to_string_fn(NULL, _t2051, 1)), gem_to_string_fn(NULL, _t2052, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2053, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2055, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2056;
    } else {
#line 810 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 811 "compiler/main.gem"
    GemVal _t2057 = gem_v_node;
    GemVal _t2058[] = {gem_table_get(_t2057, gem_string("value"))};
    GemVal _t2059 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2059.fn(_t2059.env, _t2058, 1);
#line 812 "compiler/main.gem"
    GemVal _t2060 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t2060, gem_string("name"));
#line 813 "compiler/main.gem"
    GemVal _t2061[] = {gem_v_name};
    GemVal _t2062 = (*gem_v_mangle);
            GemVal gem_v_mname = _t2062.fn(_t2062.env, _t2061, 1);
#line 814 "compiler/main.gem"
    GemVal _t2063[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2065;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2063, 2))) {
        _t2065 = gem_fn_set_contains(NULL, _t2063, 2);
    } else {
        GemVal _t2064[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2065 = gem_fn_set_contains(NULL, _t2064, 2);
    }
            if (gem_truthy(_t2065)) {
#line 815 "compiler/main.gem"
    GemVal _t2066[] = {gem_v_line_dir};
    GemVal _t2067 = gem_v_r;
    GemVal _t2068[] = {gem_table_get(_t2067, gem_string("setup"))};
    GemVal _t2069[] = {gem_v_p};
    GemVal _t2070[] = {gem_v_mname};
    GemVal _t2071 = gem_v_r;
    GemVal _t2072[] = {gem_table_get(_t2071, gem_string("expr"))};
                GemVal _t2073 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2066, 1), gem_to_string_fn(NULL, _t2068, 1)), gem_to_string_fn(NULL, _t2069, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2070, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2072, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2073;
            }
#line 817 "compiler/main.gem"
    GemVal _t2074[] = {gem_v_line_dir};
    GemVal _t2075 = gem_v_r;
    GemVal _t2076[] = {gem_table_get(_t2075, gem_string("setup"))};
    GemVal _t2077[] = {gem_v_p};
    GemVal _t2078[] = {gem_v_mname};
    GemVal _t2079 = gem_v_r;
    GemVal _t2080[] = {gem_table_get(_t2079, gem_string("expr"))};
            GemVal _t2081 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2074, 1), gem_to_string_fn(NULL, _t2076, 1)), gem_to_string_fn(NULL, _t2077, 1)), gem_to_string_fn(NULL, _t2078, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2080, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2081;
        } else {
#line 818 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
#line 819 "compiler/main.gem"
    GemVal _t2082 = gem_v_node;
    GemVal _t2083[] = {gem_table_get(_t2082, gem_string("object"))};
    GemVal _t2084 = (*gem_v_compile_expr);
                GemVal gem_v_obj_r = _t2084.fn(_t2084.env, _t2083, 1);
#line 820 "compiler/main.gem"
    GemVal _t2085 = gem_v_node;
    GemVal _t2086[] = {gem_table_get(_t2085, gem_string("value"))};
    GemVal _t2087 = (*gem_v_compile_expr);
                GemVal gem_v_val_r = _t2087.fn(_t2087.env, _t2086, 1);
#line 821 "compiler/main.gem"
    GemVal _t2088 = gem_v_node;
    GemVal _t2089[] = {gem_table_get(_t2088, gem_string("field"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2089, 1);
#line 822 "compiler/main.gem"
    GemVal _t2090[] = {gem_v_line_dir};
    GemVal _t2091 = gem_v_obj_r;
    GemVal _t2092[] = {gem_table_get(_t2091, gem_string("setup"))};
    GemVal _t2093 = gem_v_val_r;
    GemVal _t2094[] = {gem_table_get(_t2093, gem_string("setup"))};
    GemVal _t2095[] = {gem_v_p};
    GemVal _t2096 = gem_v_obj_r;
    GemVal _t2097[] = {gem_table_get(_t2096, gem_string("expr"))};
    GemVal _t2098[] = {gem_v_escaped};
    GemVal _t2099 = gem_v_val_r;
    GemVal _t2100[] = {gem_table_get(_t2099, gem_string("expr"))};
                GemVal _t2101 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2090, 1), gem_to_string_fn(NULL, _t2092, 1)), gem_to_string_fn(NULL, _t2094, 1)), gem_to_string_fn(NULL, _t2095, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2097, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2098, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2100, 1)), gem_string(");"));
                gem_pop_frame();
                return _t2101;
            } else {
#line 823 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("index_assign")))) {
#line 824 "compiler/main.gem"
    GemVal _t2102 = gem_v_node;
    GemVal _t2103[] = {gem_table_get(_t2102, gem_string("object"))};
    GemVal _t2104 = (*gem_v_compile_expr);
                    GemVal gem_v_obj_r = _t2104.fn(_t2104.env, _t2103, 1);
#line 825 "compiler/main.gem"
    GemVal _t2105 = gem_v_node;
    GemVal _t2106[] = {gem_table_get(_t2105, gem_string("key"))};
    GemVal _t2107 = (*gem_v_compile_expr);
                    GemVal gem_v_key_r = _t2107.fn(_t2107.env, _t2106, 1);
#line 826 "compiler/main.gem"
    GemVal _t2108 = gem_v_node;
    GemVal _t2109[] = {gem_table_get(_t2108, gem_string("value"))};
    GemVal _t2110 = (*gem_v_compile_expr);
                    GemVal gem_v_val_r = _t2110.fn(_t2110.env, _t2109, 1);
#line 827 "compiler/main.gem"
    GemVal _t2111[] = {gem_v_line_dir};
    GemVal _t2112 = gem_v_obj_r;
    GemVal _t2113[] = {gem_table_get(_t2112, gem_string("setup"))};
    GemVal _t2114 = gem_v_key_r;
    GemVal _t2115[] = {gem_table_get(_t2114, gem_string("setup"))};
    GemVal _t2116 = gem_v_val_r;
    GemVal _t2117[] = {gem_table_get(_t2116, gem_string("setup"))};
    GemVal _t2118[] = {gem_v_p};
    GemVal _t2119 = gem_v_obj_r;
    GemVal _t2120[] = {gem_table_get(_t2119, gem_string("expr"))};
    GemVal _t2121 = gem_v_key_r;
    GemVal _t2122[] = {gem_table_get(_t2121, gem_string("expr"))};
    GemVal _t2123 = gem_v_val_r;
    GemVal _t2124[] = {gem_table_get(_t2123, gem_string("expr"))};
                    GemVal _t2125 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2111, 1), gem_to_string_fn(NULL, _t2113, 1)), gem_to_string_fn(NULL, _t2115, 1)), gem_to_string_fn(NULL, _t2117, 1)), gem_to_string_fn(NULL, _t2118, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2120, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2122, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2124, 1)), gem_string(");"));
                    gem_pop_frame();
                    return _t2125;
                } else {
#line 828 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 829 "compiler/main.gem"
    GemVal _t2126[] = {gem_v_line_dir};
    GemVal _t2127[] = {gem_v_node, gem_v_indent};
    GemVal _t2128 = (*gem_v_compile_if);
                        GemVal _t2129 = gem_add(gem_to_string_fn(NULL, _t2126, 1), _t2128.fn(_t2128.env, _t2127, 2));
                        gem_pop_frame();
                        return _t2129;
                    } else {
#line 830 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 831 "compiler/main.gem"
    GemVal _t2130[] = {gem_v_line_dir};
    GemVal _t2131[] = {gem_v_node, gem_v_indent};
    GemVal _t2132 = (*gem_v_compile_while);
                            GemVal _t2133 = gem_add(gem_to_string_fn(NULL, _t2130, 1), _t2132.fn(_t2132.env, _t2131, 2));
                            gem_pop_frame();
                            return _t2133;
                        } else {
#line 832 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 833 "compiler/main.gem"
    GemVal _t2134[] = {gem_v_line_dir};
    GemVal _t2135[] = {gem_v_node, gem_v_indent};
    GemVal _t2136 = (*gem_v_compile_match);
                                GemVal _t2137 = gem_add(gem_to_string_fn(NULL, _t2134, 1), _t2136.fn(_t2136.env, _t2135, 2));
                                gem_pop_frame();
                                return _t2137;
                            } else {
#line 834 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("break")))) {
#line 835 "compiler/main.gem"
                                    GemVal _t2138 = gem_add(gem_v_p, gem_string("break;"));
                                    gem_pop_frame();
                                    return _t2138;
                                } else {
#line 836 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("continue")))) {
#line 837 "compiler/main.gem"
                                        GemVal _t2139 = gem_add(gem_v_p, gem_string("continue;"));
                                        gem_pop_frame();
                                        return _t2139;
                                    } else {
#line 838 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 839 "compiler/main.gem"
    GemVal _t2140 = gem_v_node;
                                            if (gem_truthy(gem_neq(gem_table_get(_t2140, gem_string("value")), GEM_NIL))) {
#line 840 "compiler/main.gem"
    GemVal _t2141 = gem_v_node;
    GemVal _t2142[] = {gem_table_get(_t2141, gem_string("value"))};
    GemVal _t2143 = (*gem_v_compile_expr);
                                                GemVal gem_v_r = _t2143.fn(_t2143.env, _t2142, 1);
#line 841 "compiler/main.gem"
    GemVal _t2144 = (*gem_v_tmp);
                                                GemVal gem_v_t = _t2144.fn(_t2144.env, NULL, 0);
#line 842 "compiler/main.gem"
    GemVal _t2145[] = {gem_v_line_dir};
    GemVal _t2146 = gem_v_r;
    GemVal _t2147[] = {gem_table_get(_t2146, gem_string("setup"))};
    GemVal _t2148[] = {gem_v_p};
    GemVal _t2149[] = {gem_v_t};
    GemVal _t2150 = gem_v_r;
    GemVal _t2151[] = {gem_table_get(_t2150, gem_string("expr"))};
    GemVal _t2152[] = {gem_v_p};
    GemVal _t2153[] = {gem_v_p};
    GemVal _t2154[] = {gem_v_t};
                                                GemVal _t2155 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2145, 1), gem_to_string_fn(NULL, _t2147, 1)), gem_to_string_fn(NULL, _t2148, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2149, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2151, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2152, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2153, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2154, 1)), gem_string(";"));
                                                gem_pop_frame();
                                                return _t2155;
                                            }
#line 844 "compiler/main.gem"
    GemVal _t2156[] = {gem_v_line_dir};
    GemVal _t2157[] = {gem_v_p};
    GemVal _t2158[] = {gem_v_p};
                                            GemVal _t2159 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2156, 1), gem_to_string_fn(NULL, _t2157, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2158, 1)), gem_string("return GEM_NIL;"));
                                            gem_pop_frame();
                                            return _t2159;
                                        } else {
#line 845 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 846 "compiler/main.gem"
    GemVal _t2160[] = {gem_v_node};
    GemVal _t2161 = (*gem_v_compile_expr);
                                                GemVal gem_v_r = _t2161.fn(_t2161.env, _t2160, 1);
#line 847 "compiler/main.gem"
    GemVal _t2162[] = {gem_v_line_dir};
    GemVal _t2163 = gem_v_r;
    GemVal _t2164[] = {gem_table_get(_t2163, gem_string("setup"))};
    GemVal _t2165[] = {gem_v_p};
    GemVal _t2166 = gem_v_r;
    GemVal _t2167[] = {gem_table_get(_t2166, gem_string("expr"))};
                                                GemVal _t2168 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2162, 1), gem_to_string_fn(NULL, _t2164, 1)), gem_to_string_fn(NULL, _t2165, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2167, 1)), gem_string(");"));
                                                gem_pop_frame();
                                                return _t2168;
                                            } else {
#line 848 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
#line 849 "compiler/main.gem"
                                                    GemVal _t2169 = gem_string("");
                                                    gem_pop_frame();
                                                    return _t2169;
                                                } else {
#line 850 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 851 "compiler/main.gem"
                                                        GemVal gem_v_out = gem_string("");
#line 852 "compiler/main.gem"
    GemVal _t2170 = gem_v_node;
                                                        GemVal gem_v__for_items_36 = gem_table_get(_t2170, gem_string("stmts"));
#line 852 "compiler/main.gem"
                                                        GemVal gem_v__for_i_36 = gem_int(0);
#line 852 "compiler/main.gem"
                                                        while (1) {
                                                            GemVal _t2171[] = {gem_v__for_items_36};
                                                            if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_len_fn(NULL, _t2171, 1)))) break;
#line 852 "compiler/main.gem"
                                                            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_36, gem_v__for_i_36);
#line 852 "compiler/main.gem"
                                                            gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 853 "compiler/main.gem"
    GemVal _t2172[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2173 = (*gem_v_compile_stmt);
                                                            gem_v_out = gem_add(gem_add(gem_v_out, _t2173.fn(_t2173.env, _t2172, 2)), gem_string("\n"));
                                                        }

#line 855 "compiler/main.gem"
                                                        GemVal _t2174 = gem_v_out;
                                                        gem_pop_frame();
                                                        return _t2174;
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
#line 858 "compiler/main.gem"
    GemVal _t2175[] = {gem_v_node};
    GemVal _t2176 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t2176.fn(_t2176.env, _t2175, 1);
    GemVal _t2177[] = {gem_v_line_dir};
    GemVal _t2178 = gem_v_r;
    GemVal _t2179[] = {gem_table_get(_t2178, gem_string("setup"))};
    GemVal _t2180[] = {gem_v_p};
    GemVal _t2181 = gem_v_r;
    GemVal _t2182[] = {gem_table_get(_t2181, gem_string("expr"))};
    GemVal _t2183 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2177, 1), gem_to_string_fn(NULL, _t2179, 1)), gem_to_string_fn(NULL, _t2180, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2182, 1)), gem_string(");"));
    gem_pop_frame();
    return _t2183;
}

struct _closure__anon_40 {
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
static GemVal _anon_40(void *_env, GemVal *args, int argc) {
    struct _closure__anon_40 *_cls = (struct _closure__anon_40 *)_env;
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
    gem_push_frame("_anon_40", "compiler/main.gem", 0);
#line 865 "compiler/main.gem"
    GemVal _t2185[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2185, 1);
#line 866 "compiler/main.gem"
    GemVal _t2186 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t2186, gem_string("tag"));
#line 867 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 868 "compiler/main.gem"
    GemVal _t2187 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2187, gem_string("line")), GEM_NIL))) {
#line 869 "compiler/main.gem"
    GemVal _t2188 = gem_v_node;
    GemVal _t2189[] = {gem_table_get(_t2188, gem_string("line"))};
    GemVal _t2190[] = {(*gem_v_source_name)};
    GemVal _t2191[] = {gem_fn_escape_c_string(NULL, _t2190, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2189, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2191, 1)), gem_string("\"\n"));
    }
#line 872 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 873 "compiler/main.gem"
    GemVal _t2192 = gem_v_node;
    GemVal _t2193[] = {gem_table_get(_t2192, gem_string("value"))};
    GemVal _t2194 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2194.fn(_t2194.env, _t2193, 1);
#line 874 "compiler/main.gem"
    GemVal _t2195 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t2195, gem_string("name"));
#line 875 "compiler/main.gem"
    GemVal _t2196[] = {gem_v_name};
    GemVal _t2197 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2197.fn(_t2197.env, _t2196, 1);
#line 876 "compiler/main.gem"
    GemVal _t2198[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2198, 2))) {
#line 877 "compiler/main.gem"
    GemVal _t2199[] = {gem_v_line_dir};
    GemVal _t2200 = gem_v_r;
    GemVal _t2201[] = {gem_table_get(_t2200, gem_string("setup"))};
    GemVal _t2202[] = {gem_v_p};
    GemVal _t2203[] = {gem_v_mname};
    GemVal _t2204[] = {gem_v_p};
    GemVal _t2205[] = {gem_v_mname};
    GemVal _t2206 = gem_v_r;
    GemVal _t2207[] = {gem_table_get(_t2206, gem_string("expr"))};
    GemVal _t2208[] = {gem_v_p};
    GemVal _t2209[] = {gem_v_p};
            GemVal _t2210 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2199, 1), gem_to_string_fn(NULL, _t2201, 1)), gem_to_string_fn(NULL, _t2202, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2203, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2204, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2205, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2207, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2208, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2209, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t2210;
        }
#line 879 "compiler/main.gem"
    GemVal _t2211[] = {gem_v_line_dir};
    GemVal _t2212 = gem_v_r;
    GemVal _t2213[] = {gem_table_get(_t2212, gem_string("setup"))};
    GemVal _t2214[] = {gem_v_p};
    GemVal _t2215[] = {gem_v_mname};
    GemVal _t2216 = gem_v_r;
    GemVal _t2217[] = {gem_table_get(_t2216, gem_string("expr"))};
    GemVal _t2218[] = {gem_v_p};
    GemVal _t2219[] = {gem_v_p};
        GemVal _t2220 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2211, 1), gem_to_string_fn(NULL, _t2213, 1)), gem_to_string_fn(NULL, _t2214, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2215, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2217, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2218, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2219, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2220;
    } else {
#line 880 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 881 "compiler/main.gem"
    GemVal _t2221 = gem_v_node;
    GemVal _t2222[] = {gem_table_get(_t2221, gem_string("value"))};
    GemVal _t2223 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2223.fn(_t2223.env, _t2222, 1);
#line 882 "compiler/main.gem"
    GemVal _t2224 = (*gem_v_tmp);
            GemVal gem_v_t = _t2224.fn(_t2224.env, NULL, 0);
#line 883 "compiler/main.gem"
    GemVal _t2225 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t2225, gem_string("name"));
#line 884 "compiler/main.gem"
    GemVal _t2226[] = {gem_v_name};
    GemVal _t2227 = (*gem_v_mangle);
            GemVal gem_v_mname = _t2227.fn(_t2227.env, _t2226, 1);
#line 885 "compiler/main.gem"
    GemVal _t2228[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2230;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2228, 2))) {
        _t2230 = gem_fn_set_contains(NULL, _t2228, 2);
    } else {
        GemVal _t2229[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2230 = gem_fn_set_contains(NULL, _t2229, 2);
    }
            if (gem_truthy(_t2230)) {
#line 886 "compiler/main.gem"
    GemVal _t2231[] = {gem_v_line_dir};
    GemVal _t2232 = gem_v_r;
    GemVal _t2233[] = {gem_table_get(_t2232, gem_string("setup"))};
    GemVal _t2234[] = {gem_v_p};
    GemVal _t2235[] = {gem_v_t};
    GemVal _t2236 = gem_v_r;
    GemVal _t2237[] = {gem_table_get(_t2236, gem_string("expr"))};
    GemVal _t2238[] = {gem_v_p};
    GemVal _t2239[] = {gem_v_mname};
    GemVal _t2240[] = {gem_v_t};
    GemVal _t2241[] = {gem_v_p};
    GemVal _t2242[] = {gem_v_p};
    GemVal _t2243[] = {gem_v_t};
                GemVal _t2244 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2231, 1), gem_to_string_fn(NULL, _t2233, 1)), gem_to_string_fn(NULL, _t2234, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2235, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2237, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2238, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2239, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2240, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2241, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2242, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2243, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2244;
            }
#line 888 "compiler/main.gem"
    GemVal _t2245[] = {gem_v_line_dir};
    GemVal _t2246 = gem_v_r;
    GemVal _t2247[] = {gem_table_get(_t2246, gem_string("setup"))};
    GemVal _t2248[] = {gem_v_p};
    GemVal _t2249[] = {gem_v_t};
    GemVal _t2250 = gem_v_r;
    GemVal _t2251[] = {gem_table_get(_t2250, gem_string("expr"))};
    GemVal _t2252[] = {gem_v_p};
    GemVal _t2253[] = {gem_v_mname};
    GemVal _t2254[] = {gem_v_t};
    GemVal _t2255[] = {gem_v_p};
    GemVal _t2256[] = {gem_v_p};
    GemVal _t2257[] = {gem_v_t};
            GemVal _t2258 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2245, 1), gem_to_string_fn(NULL, _t2247, 1)), gem_to_string_fn(NULL, _t2248, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2249, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2251, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2252, 1)), gem_to_string_fn(NULL, _t2253, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2254, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2255, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2256, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2257, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2258;
        } else {
#line 889 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 890 "compiler/main.gem"
    GemVal _t2259 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t2259, gem_string("value")), GEM_NIL))) {
#line 891 "compiler/main.gem"
    GemVal _t2260 = gem_v_node;
    GemVal _t2261[] = {gem_table_get(_t2260, gem_string("value"))};
    GemVal _t2262 = (*gem_v_compile_expr);
                    GemVal gem_v_r = _t2262.fn(_t2262.env, _t2261, 1);
#line 892 "compiler/main.gem"
    GemVal _t2263 = (*gem_v_tmp);
                    GemVal gem_v_t = _t2263.fn(_t2263.env, NULL, 0);
#line 893 "compiler/main.gem"
    GemVal _t2264[] = {gem_v_line_dir};
    GemVal _t2265 = gem_v_r;
    GemVal _t2266[] = {gem_table_get(_t2265, gem_string("setup"))};
    GemVal _t2267[] = {gem_v_p};
    GemVal _t2268[] = {gem_v_t};
    GemVal _t2269 = gem_v_r;
    GemVal _t2270[] = {gem_table_get(_t2269, gem_string("expr"))};
    GemVal _t2271[] = {gem_v_p};
    GemVal _t2272[] = {gem_v_p};
    GemVal _t2273[] = {gem_v_t};
                    GemVal _t2274 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2264, 1), gem_to_string_fn(NULL, _t2266, 1)), gem_to_string_fn(NULL, _t2267, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2268, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2270, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2271, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2272, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2273, 1)), gem_string(";"));
                    gem_pop_frame();
                    return _t2274;
                }
#line 895 "compiler/main.gem"
    GemVal _t2275[] = {gem_v_line_dir};
    GemVal _t2276[] = {gem_v_p};
    GemVal _t2277[] = {gem_v_p};
                GemVal _t2278 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2275, 1), gem_to_string_fn(NULL, _t2276, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2277, 1)), gem_string("return GEM_NIL;"));
                gem_pop_frame();
                return _t2278;
            } else {
#line 896 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 897 "compiler/main.gem"
    GemVal _t2279[] = {gem_v_line_dir};
    GemVal _t2280[] = {gem_v_node, gem_v_indent};
    GemVal _t2281 = (*gem_v_compile_if_return);
                    GemVal _t2282 = gem_add(gem_to_string_fn(NULL, _t2279, 1), _t2281.fn(_t2281.env, _t2280, 2));
                    gem_pop_frame();
                    return _t2282;
                } else {
#line 898 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 899 "compiler/main.gem"
    GemVal _t2283[] = {gem_v_line_dir};
    GemVal _t2284[] = {gem_v_node, gem_v_indent};
    GemVal _t2285 = (*gem_v_compile_while);
    GemVal _t2286[] = {_t2285.fn(_t2285.env, _t2284, 2)};
    GemVal _t2287[] = {gem_v_p};
    GemVal _t2288[] = {gem_v_p};
                        GemVal _t2289 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2283, 1), gem_to_string_fn(NULL, _t2286, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t2287, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2288, 1)), gem_string("return GEM_NIL;"));
                        gem_pop_frame();
                        return _t2289;
                    } else {
#line 900 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 901 "compiler/main.gem"
    GemVal _t2290[] = {gem_v_line_dir};
    GemVal _t2291[] = {gem_v_node, gem_v_indent};
    GemVal _t2292 = (*gem_v_compile_match_return);
                            GemVal _t2293 = gem_add(gem_to_string_fn(NULL, _t2290, 1), _t2292.fn(_t2292.env, _t2291, 2));
                            gem_pop_frame();
                            return _t2293;
                        } else {
#line 902 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 903 "compiler/main.gem"
    GemVal _t2294[] = {gem_v_p};
                                GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2294, 1), gem_string("{\n"));
#line 904 "compiler/main.gem"
                                GemVal gem_v__for_i_37 = gem_int(0);
#line 904 "compiler/main.gem"
    GemVal _t2295 = gem_v_node;
    GemVal _t2296[] = {gem_table_get(_t2295, gem_string("stmts"))};
                                GemVal gem_v__for_limit_37 = gem_sub(gem_len_fn(NULL, _t2296, 1), gem_int(1));
#line 904 "compiler/main.gem"
                                while (1) {
                                    if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 904 "compiler/main.gem"
                                    GemVal gem_v_i = gem_v__for_i_37;
#line 904 "compiler/main.gem"
                                    gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 905 "compiler/main.gem"
    GemVal _t2297 = gem_v_node;
    GemVal _t2298[] = {gem_table_get(gem_table_get(_t2297, gem_string("stmts")), gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2299 = (*gem_v_compile_stmt);
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t2299.fn(_t2299.env, _t2298, 2)), gem_string("\n"));
                                }

#line 907 "compiler/main.gem"
    GemVal _t2300 = gem_v_node;
    GemVal _t2301[] = {gem_table_get(_t2300, gem_string("stmts"))};
                                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2301, 1), gem_int(0)))) {
#line 908 "compiler/main.gem"
    GemVal _t2302 = gem_v_node;
    GemVal _t2303 = gem_v_node;
    GemVal _t2304[] = {gem_table_get(_t2303, gem_string("stmts"))};
    GemVal _t2305[] = {gem_table_get(gem_table_get(_t2302, gem_string("stmts")), gem_sub(gem_len_fn(NULL, _t2304, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2306 = (*gem_v_compile_stmt_return);
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t2306.fn(_t2306.env, _t2305, 2)), gem_string("\n"));
                                }
#line 910 "compiler/main.gem"
    GemVal _t2307[] = {gem_v_p};
                                gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2307, 1), gem_string("}")));
#line 911 "compiler/main.gem"
                                GemVal _t2308 = gem_v_out;
                                gem_pop_frame();
                                return _t2308;
                            } else {
#line 912 "compiler/main.gem"
    GemVal _t2309;
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
        _t2309 = gem_eq(gem_v_tag, gem_string("dot_assign"));
    } else {
        _t2309 = gem_eq(gem_v_tag, gem_string("index_assign"));
    }
    GemVal _t2310;
    if (gem_truthy(_t2309)) {
        _t2310 = _t2309;
    } else {
        _t2310 = gem_eq(gem_v_tag, gem_string("break"));
    }
    GemVal _t2311;
    if (gem_truthy(_t2310)) {
        _t2311 = _t2310;
    } else {
        _t2311 = gem_eq(gem_v_tag, gem_string("continue"));
    }
    GemVal _t2312;
    if (gem_truthy(_t2311)) {
        _t2312 = _t2311;
    } else {
        _t2312 = gem_eq(gem_v_tag, gem_string("fn_def"));
    }
                                if (gem_truthy(_t2312)) {
#line 913 "compiler/main.gem"
    GemVal _t2313[] = {gem_v_node, gem_v_indent};
    GemVal _t2314 = (*gem_v_compile_stmt);
    GemVal _t2315[] = {_t2314.fn(_t2314.env, _t2313, 2)};
    GemVal _t2316[] = {gem_v_p};
    GemVal _t2317[] = {gem_v_p};
                                    GemVal _t2318 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2315, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2316, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2317, 1)), gem_string("return GEM_NIL;"));
                                    gem_pop_frame();
                                    return _t2318;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 916 "compiler/main.gem"
    GemVal _t2319[] = {gem_v_node};
    GemVal _t2320 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t2320.fn(_t2320.env, _t2319, 1);
#line 917 "compiler/main.gem"
    GemVal _t2321 = (*gem_v_tmp);
    GemVal gem_v_t = _t2321.fn(_t2321.env, NULL, 0);
    GemVal _t2322[] = {gem_v_line_dir};
    GemVal _t2323 = gem_v_r;
    GemVal _t2324[] = {gem_table_get(_t2323, gem_string("setup"))};
    GemVal _t2325[] = {gem_v_p};
    GemVal _t2326[] = {gem_v_t};
    GemVal _t2327 = gem_v_r;
    GemVal _t2328[] = {gem_table_get(_t2327, gem_string("expr"))};
    GemVal _t2329[] = {gem_v_p};
    GemVal _t2330[] = {gem_v_p};
    GemVal _t2331[] = {gem_v_t};
    GemVal _t2332 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2322, 1), gem_to_string_fn(NULL, _t2324, 1)), gem_to_string_fn(NULL, _t2325, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2326, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2328, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2329, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2330, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2331, 1)), gem_string(";"));
    gem_pop_frame();
    return _t2332;
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
#line 924 "compiler/main.gem"
    GemVal _t2334[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2334, 1);
#line 925 "compiler/main.gem"
    GemVal _t2335 = gem_v_node;
    GemVal _t2336[] = {gem_table_get(_t2335, gem_string("cond"))};
    GemVal _t2337 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t2337.fn(_t2337.env, _t2336, 1);
#line 926 "compiler/main.gem"
    GemVal _t2338 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2338, gem_string("setup"));
#line 927 "compiler/main.gem"
    GemVal _t2339[] = {gem_v_p};
    GemVal _t2340 = gem_v_cond_r;
    GemVal _t2341[] = {gem_table_get(_t2340, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2339, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2341, 1)), gem_string(")) {\n")));
#line 928 "compiler/main.gem"
    GemVal _t2342 = gem_v_node;
    GemVal _t2343[] = {gem_table_get(_t2342, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2344 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t2344.fn(_t2344.env, _t2343, 2));
#line 929 "compiler/main.gem"
    GemVal _t2345 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2345, gem_string("else")), GEM_NIL))) {
#line 930 "compiler/main.gem"
    GemVal _t2346[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2346, 1), gem_string("} else {\n")));
#line 931 "compiler/main.gem"
    GemVal _t2347 = gem_v_node;
    GemVal _t2348[] = {gem_table_get(_t2347, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2349 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t2349.fn(_t2349.env, _t2348, 2));
    }
#line 933 "compiler/main.gem"
    GemVal _t2350[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2350, 1), gem_string("}")));
    GemVal _t2351 = gem_v_out;
    gem_pop_frame();
    return _t2351;
}

struct _closure__anon_42 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
};
static GemVal _anon_42(void *_env, GemVal *args, int argc) {
    struct _closure__anon_42 *_cls = (struct _closure__anon_42 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_42", "compiler/main.gem", 0);
#line 940 "compiler/main.gem"
    GemVal _t2353[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2353, 1);
#line 941 "compiler/main.gem"
    GemVal _t2354 = gem_v_node;
    GemVal _t2355[] = {gem_table_get(_t2354, gem_string("cond"))};
    GemVal _t2356 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t2356.fn(_t2356.env, _t2355, 1);
#line 942 "compiler/main.gem"
    GemVal _t2357 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2357, gem_string("setup"));
#line 943 "compiler/main.gem"
    GemVal _t2358[] = {gem_v_p};
    GemVal _t2359 = gem_v_cond_r;
    GemVal _t2360[] = {gem_table_get(_t2359, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2358, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2360, 1)), gem_string(")) {\n")));
#line 944 "compiler/main.gem"
    GemVal _t2361 = gem_v_node;
    GemVal gem_v_then_body = gem_table_get(_t2361, gem_string("then"));
#line 945 "compiler/main.gem"
    GemVal _t2362[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2362, 1), gem_int(0)))) {
#line 946 "compiler/main.gem"
        GemVal gem_v__for_i_38 = gem_int(0);
#line 946 "compiler/main.gem"
    GemVal _t2363[] = {gem_v_then_body};
        GemVal gem_v__for_limit_38 = gem_sub(gem_len_fn(NULL, _t2363, 1), gem_int(1));
#line 946 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 946 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_38;
#line 946 "compiler/main.gem"
            gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 947 "compiler/main.gem"
    GemVal _t2364[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2365 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2365.fn(_t2365.env, _t2364, 2)), gem_string("\n"));
        }

#line 949 "compiler/main.gem"
    GemVal _t2366[] = {gem_v_then_body};
    GemVal _t2367[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t2366, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2368 = (*gem_v_compile_stmt_return);
        gem_v_out = gem_add(gem_add(gem_v_out, _t2368.fn(_t2368.env, _t2367, 2)), gem_string("\n"));
    } else {
#line 951 "compiler/main.gem"
    GemVal _t2369[] = {gem_v_p};
    GemVal _t2370[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2369, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2370, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 953 "compiler/main.gem"
    GemVal _t2371 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2371, gem_string("else")), GEM_NIL))) {
#line 954 "compiler/main.gem"
    GemVal _t2372[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2372, 1), gem_string("} else {\n")));
#line 955 "compiler/main.gem"
    GemVal _t2373 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t2373, gem_string("else"));
#line 956 "compiler/main.gem"
    GemVal _t2374[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2374, 1), gem_int(0)))) {
#line 957 "compiler/main.gem"
            GemVal gem_v__for_i_39 = gem_int(0);
#line 957 "compiler/main.gem"
    GemVal _t2375[] = {gem_v_else_body};
            GemVal gem_v__for_limit_39 = gem_sub(gem_len_fn(NULL, _t2375, 1), gem_int(1));
#line 957 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
#line 957 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_39;
#line 957 "compiler/main.gem"
                gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 958 "compiler/main.gem"
    GemVal _t2376[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2377 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t2377.fn(_t2377.env, _t2376, 2)), gem_string("\n"));
            }

#line 960 "compiler/main.gem"
    GemVal _t2378[] = {gem_v_else_body};
    GemVal _t2379[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2378, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2380 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2380.fn(_t2380.env, _t2379, 2)), gem_string("\n"));
        } else {
#line 962 "compiler/main.gem"
    GemVal _t2381[] = {gem_v_p};
    GemVal _t2382[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2381, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2382, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 965 "compiler/main.gem"
    GemVal _t2383[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2383, 1), gem_string("} else {\n")));
#line 966 "compiler/main.gem"
    GemVal _t2384[] = {gem_v_p};
    GemVal _t2385[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2384, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2385, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 968 "compiler/main.gem"
    GemVal _t2386[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2386, 1), gem_string("}")));
    GemVal _t2387 = gem_v_out;
    gem_pop_frame();
    return _t2387;
}

struct _closure__anon_43 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts;
};
static GemVal _anon_43(void *_env, GemVal *args, int argc) {
    struct _closure__anon_43 *_cls = (struct _closure__anon_43 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_43", "compiler/main.gem", 0);
#line 975 "compiler/main.gem"
    GemVal _t2389[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2389, 1);
#line 976 "compiler/main.gem"
    GemVal _t2390 = gem_v_node;
    GemVal _t2391[] = {gem_table_get(_t2390, gem_string("cond"))};
    GemVal _t2392 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t2392.fn(_t2392.env, _t2391, 1);
#line 977 "compiler/main.gem"
    GemVal _t2393[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2393, 1), gem_string("while (1) {\n"));
#line 978 "compiler/main.gem"
    GemVal _t2394 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t2394, gem_string("setup")), gem_string("")))) {
#line 979 "compiler/main.gem"
    GemVal _t2395 = gem_v_cond_r;
    GemVal _t2396[] = {gem_v_p};
    GemVal _t2397[] = {gem_table_get(_t2395, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t2396, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_str_replace_fn(NULL, _t2397, 3));
    }
#line 981 "compiler/main.gem"
    GemVal _t2398[] = {gem_v_p};
    GemVal _t2399 = gem_v_cond_r;
    GemVal _t2400[] = {gem_table_get(_t2399, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2398, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t2400, 1)), gem_string(")) break;\n")));
#line 982 "compiler/main.gem"
    GemVal _t2401 = gem_v_node;
    GemVal _t2402[] = {gem_table_get(_t2401, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2403 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t2403.fn(_t2403.env, _t2402, 2));
#line 983 "compiler/main.gem"
    GemVal _t2404[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2404, 1), gem_string("}")));
    GemVal _t2405 = gem_v_out;
    gem_pop_frame();
    return _t2405;
}

struct _closure__anon_44 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts;
    GemVal *gem_v_tmp;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_44", "compiler/main.gem", 0);
#line 990 "compiler/main.gem"
    GemVal _t2407[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2407, 1);
#line 991 "compiler/main.gem"
    GemVal _t2408 = gem_v_node;
    GemVal _t2409[] = {gem_table_get(_t2408, gem_string("target"))};
    GemVal _t2410 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t2410.fn(_t2410.env, _t2409, 1);
#line 992 "compiler/main.gem"
    GemVal _t2411 = (*gem_v_tmp);
    GemVal gem_v_t = _t2411.fn(_t2411.env, NULL, 0);
#line 993 "compiler/main.gem"
    GemVal _t2412 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2412, gem_string("setup"));
#line 994 "compiler/main.gem"
    GemVal _t2413[] = {gem_v_p};
    GemVal _t2414[] = {gem_v_t};
    GemVal _t2415 = gem_v_target_r;
    GemVal _t2416[] = {gem_table_get(_t2415, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2413, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2414, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2416, 1)), gem_string(";\n")));
#line 995 "compiler/main.gem"
    GemVal gem_v__for_i_40 = gem_int(0);
#line 995 "compiler/main.gem"
    GemVal _t2417 = gem_v_node;
    GemVal _t2418[] = {gem_table_get(_t2417, gem_string("whens"))};
    GemVal gem_v__for_limit_40 = gem_len_fn(NULL, _t2418, 1);
#line 995 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 995 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_40;
#line 995 "compiler/main.gem"
        gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 996 "compiler/main.gem"
    GemVal _t2419 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t2419, gem_string("whens")), gem_v_i);
#line 997 "compiler/main.gem"
    GemVal _t2420 = gem_v_w;
    GemVal _t2421[] = {gem_table_get(_t2420, gem_string("value"))};
    GemVal _t2422 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2422.fn(_t2422.env, _t2421, 1);
#line 998 "compiler/main.gem"
        GemVal gem_v_keyword = gem_string("if");
#line 999 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1000 "compiler/main.gem"
            gem_v_keyword = gem_string("} else if");
        }
#line 1002 "compiler/main.gem"
    GemVal _t2423 = gem_v_val_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t2423, gem_string("setup")));
#line 1003 "compiler/main.gem"
    GemVal _t2424[] = {gem_v_p};
    GemVal _t2425[] = {gem_v_keyword};
    GemVal _t2426[] = {gem_v_t};
    GemVal _t2427 = gem_v_val_r;
    GemVal _t2428[] = {gem_table_get(_t2427, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2424, 1), gem_to_string_fn(NULL, _t2425, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2426, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2428, 1)), gem_string("))) {\n")));
#line 1004 "compiler/main.gem"
    GemVal _t2429 = gem_v_w;
    GemVal _t2430[] = {gem_table_get(_t2429, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2431 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t2431.fn(_t2431.env, _t2430, 2));
    }

#line 1006 "compiler/main.gem"
    GemVal _t2432 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2432, gem_string("else")), GEM_NIL))) {
#line 1007 "compiler/main.gem"
    GemVal _t2433[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2433, 1), gem_string("} else {\n")));
#line 1008 "compiler/main.gem"
    GemVal _t2434 = gem_v_node;
    GemVal _t2435[] = {gem_table_get(_t2434, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2436 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t2436.fn(_t2436.env, _t2435, 2));
    }
#line 1010 "compiler/main.gem"
    GemVal _t2437[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2437, 1), gem_string("}")));
    GemVal _t2438 = gem_v_out;
    gem_pop_frame();
    return _t2438;
}

struct _closure__anon_45 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_tmp;
};
static GemVal _anon_45(void *_env, GemVal *args, int argc) {
    struct _closure__anon_45 *_cls = (struct _closure__anon_45 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_45", "compiler/main.gem", 0);
#line 1017 "compiler/main.gem"
    GemVal _t2440[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2440, 1);
#line 1018 "compiler/main.gem"
    GemVal _t2441 = gem_v_node;
    GemVal _t2442[] = {gem_table_get(_t2441, gem_string("target"))};
    GemVal _t2443 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t2443.fn(_t2443.env, _t2442, 1);
#line 1019 "compiler/main.gem"
    GemVal _t2444 = (*gem_v_tmp);
    GemVal gem_v_t = _t2444.fn(_t2444.env, NULL, 0);
#line 1020 "compiler/main.gem"
    GemVal _t2445 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2445, gem_string("setup"));
#line 1021 "compiler/main.gem"
    GemVal _t2446[] = {gem_v_p};
    GemVal _t2447[] = {gem_v_t};
    GemVal _t2448 = gem_v_target_r;
    GemVal _t2449[] = {gem_table_get(_t2448, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2446, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2447, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2449, 1)), gem_string(";\n")));
#line 1022 "compiler/main.gem"
    GemVal gem_v__for_i_42 = gem_int(0);
#line 1022 "compiler/main.gem"
    GemVal _t2450 = gem_v_node;
    GemVal _t2451[] = {gem_table_get(_t2450, gem_string("whens"))};
    GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2451, 1);
#line 1022 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 1022 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_42;
#line 1022 "compiler/main.gem"
        gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 1023 "compiler/main.gem"
    GemVal _t2452 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t2452, gem_string("whens")), gem_v_i);
#line 1024 "compiler/main.gem"
    GemVal _t2453 = gem_v_w;
    GemVal _t2454[] = {gem_table_get(_t2453, gem_string("value"))};
    GemVal _t2455 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2455.fn(_t2455.env, _t2454, 1);
#line 1025 "compiler/main.gem"
        GemVal gem_v_keyword = gem_string("if");
#line 1026 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1027 "compiler/main.gem"
            gem_v_keyword = gem_string("} else if");
        }
#line 1029 "compiler/main.gem"
    GemVal _t2456 = gem_v_val_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t2456, gem_string("setup")));
#line 1030 "compiler/main.gem"
    GemVal _t2457[] = {gem_v_p};
    GemVal _t2458[] = {gem_v_keyword};
    GemVal _t2459[] = {gem_v_t};
    GemVal _t2460 = gem_v_val_r;
    GemVal _t2461[] = {gem_table_get(_t2460, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2457, 1), gem_to_string_fn(NULL, _t2458, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2459, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2461, 1)), gem_string("))) {\n")));
#line 1031 "compiler/main.gem"
    GemVal _t2462 = gem_v_w;
        GemVal gem_v_body = gem_table_get(_t2462, gem_string("body"));
#line 1032 "compiler/main.gem"
    GemVal _t2463[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2463, 1), gem_int(0)))) {
#line 1033 "compiler/main.gem"
            GemVal gem_v__for_i_41 = gem_int(0);
#line 1033 "compiler/main.gem"
    GemVal _t2464[] = {gem_v_body};
            GemVal gem_v__for_limit_41 = gem_sub(gem_len_fn(NULL, _t2464, 1), gem_int(1));
#line 1033 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_limit_41))) break;
#line 1033 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_41;
#line 1033 "compiler/main.gem"
                gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
#line 1034 "compiler/main.gem"
    GemVal _t2465[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2466 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t2466.fn(_t2466.env, _t2465, 2)), gem_string("\n"));
            }

#line 1036 "compiler/main.gem"
    GemVal _t2467[] = {gem_v_body};
    GemVal _t2468[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2467, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2469 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2469.fn(_t2469.env, _t2468, 2)), gem_string("\n"));
        } else {
#line 1038 "compiler/main.gem"
    GemVal _t2470[] = {gem_v_p};
    GemVal _t2471[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2470, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2471, 1)), gem_string("    return GEM_NIL;\n")));
        }
    }

#line 1041 "compiler/main.gem"
    GemVal _t2472 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2472, gem_string("else")), GEM_NIL))) {
#line 1042 "compiler/main.gem"
    GemVal _t2473[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2473, 1), gem_string("} else {\n")));
#line 1043 "compiler/main.gem"
    GemVal _t2474 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t2474, gem_string("else"));
#line 1044 "compiler/main.gem"
    GemVal _t2475[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2475, 1), gem_int(0)))) {
#line 1045 "compiler/main.gem"
            GemVal gem_v__for_i_43 = gem_int(0);
#line 1045 "compiler/main.gem"
    GemVal _t2476[] = {gem_v_else_body};
            GemVal gem_v__for_limit_43 = gem_sub(gem_len_fn(NULL, _t2476, 1), gem_int(1));
#line 1045 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 1045 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_43;
#line 1045 "compiler/main.gem"
                gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 1046 "compiler/main.gem"
    GemVal _t2477[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2478 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t2478.fn(_t2478.env, _t2477, 2)), gem_string("\n"));
            }

#line 1048 "compiler/main.gem"
    GemVal _t2479[] = {gem_v_else_body};
    GemVal _t2480[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2479, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2481 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2481.fn(_t2481.env, _t2480, 2)), gem_string("\n"));
        } else {
#line 1050 "compiler/main.gem"
    GemVal _t2482[] = {gem_v_p};
    GemVal _t2483[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2482, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2483, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1053 "compiler/main.gem"
    GemVal _t2484[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2484, 1), gem_string("} else {\n")));
#line 1054 "compiler/main.gem"
    GemVal _t2485[] = {gem_v_p};
    GemVal _t2486[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2485, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2486, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1056 "compiler/main.gem"
    GemVal _t2487[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2487, 1), gem_string("}")));
    GemVal _t2488 = gem_v_out;
    gem_pop_frame();
    return _t2488;
}

struct _closure__anon_46 {
    GemVal *gem_v_extern_c_decls;
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_46(void *_env, GemVal *args, int argc) {
    struct _closure__anon_46 *_cls = (struct _closure__anon_46 *)_env;
    GemVal *gem_v_extern_c_decls = _cls->gem_v_extern_c_decls;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_46", "compiler/main.gem", 0);
#line 1063 "compiler/main.gem"
    GemVal gem_v__d44 = gem_v_node;
#line 1063 "compiler/main.gem"
    GemVal _t2490 = gem_v__d44;
    GemVal gem_v_name = gem_table_get(_t2490, gem_string("name"));
#line 1063 "compiler/main.gem"
    GemVal _t2491 = gem_v__d44;
    GemVal gem_v_params = gem_table_get(_t2491, gem_string("params"));
#line 1063 "compiler/main.gem"
    GemVal _t2492 = gem_v__d44;
    GemVal gem_v_ret_type = gem_table_get(_t2492, gem_string("ret_type"));

#line 1065 "compiler/main.gem"
    GemVal _t2493[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2493, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1067 "compiler/main.gem"
    GemVal _t2494 = gem_table_new();
    GemVal gem_v_c_args = _t2494;
#line 1068 "compiler/main.gem"
    GemVal gem_v__for_i_45 = gem_int(0);
#line 1068 "compiler/main.gem"
    GemVal _t2495[] = {gem_v_params};
    GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2495, 1);
#line 1068 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 1068 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_45;
#line 1068 "compiler/main.gem"
        gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 1069 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1070 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1071 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1072 "compiler/main.gem"
    GemVal _t2496[] = {gem_v_i};
    GemVal _t2497[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t2496, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2497, 1)), gem_string("].ival;\n")));
#line 1073 "compiler/main.gem"
    GemVal _t2498[] = {gem_v_i};
    GemVal _t2499[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2498, 1))};
            (void)(gem_push_fn(NULL, _t2499, 2));
        } else {
#line 1074 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1075 "compiler/main.gem"
    GemVal _t2500[] = {gem_v_i};
    GemVal _t2501[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t2500, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2501, 1)), gem_string("].fval;\n")));
#line 1076 "compiler/main.gem"
    GemVal _t2502[] = {gem_v_i};
    GemVal _t2503[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2502, 1))};
                (void)(gem_push_fn(NULL, _t2503, 2));
            } else {
#line 1077 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1078 "compiler/main.gem"
    GemVal _t2504[] = {gem_v_i};
    GemVal _t2505[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t2504, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2505, 1)), gem_string("].sval;\n")));
#line 1079 "compiler/main.gem"
    GemVal _t2506[] = {gem_v_i};
    GemVal _t2507[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2506, 1))};
                    (void)(gem_push_fn(NULL, _t2507, 2));
                } else {
#line 1080 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1081 "compiler/main.gem"
    GemVal _t2508[] = {gem_v_i};
    GemVal _t2509[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t2508, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2509, 1)), gem_string("].bval;\n")));
#line 1082 "compiler/main.gem"
    GemVal _t2510[] = {gem_v_i};
    GemVal _t2511[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2510, 1))};
                        (void)(gem_push_fn(NULL, _t2511, 2));
                    } else {
#line 1083 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1084 "compiler/main.gem"
    GemVal _t2512[] = {gem_v_i};
    GemVal _t2513[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t2512, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t2513, 1)), gem_string("].ival;\n")));
#line 1085 "compiler/main.gem"
    GemVal _t2514[] = {gem_v_i};
    GemVal _t2515[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2514, 1))};
                            (void)(gem_push_fn(NULL, _t2515, 2));
                        } else {
#line 1086 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1087 "compiler/main.gem"
    GemVal _t2516[] = {gem_v_i};
    GemVal _t2517[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2516, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t2517, 2));
                            } else {
#line 1089 "compiler/main.gem"
    GemVal _t2518[] = {gem_v_i};
    GemVal _t2519[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2518, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t2519, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1094 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1095 "compiler/main.gem"
    GemVal gem_v__for_i_46 = gem_int(0);
#line 1095 "compiler/main.gem"
    GemVal _t2520[] = {gem_v_c_args};
    GemVal gem_v__for_limit_46 = gem_len_fn(NULL, _t2520, 1);
#line 1095 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
#line 1095 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_46;
#line 1095 "compiler/main.gem"
        gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 1096 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1097 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1099 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1102 "compiler/main.gem"
    GemVal _t2521[] = {gem_v_name};
    GemVal _t2522[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2521, 1), gem_string("(")), gem_to_string_fn(NULL, _t2522, 1)), gem_string(")"));
#line 1104 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1105 "compiler/main.gem"
    GemVal _t2523[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2523, 1)), gem_string(";\n")));
#line 1106 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
#line 1107 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1108 "compiler/main.gem"
    GemVal _t2524[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t2524, 1)), gem_string(";\n")));
        } else {
#line 1109 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1110 "compiler/main.gem"
    GemVal _t2525[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t2525, 1)), gem_string(";\n")));
#line 1111 "compiler/main.gem"
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
#line 1112 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1113 "compiler/main.gem"
    GemVal _t2526[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t2526, 1)), gem_string(";\n")));
#line 1114 "compiler/main.gem"
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
#line 1115 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1116 "compiler/main.gem"
    GemVal _t2527[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t2527, 1)), gem_string(";\n")));
#line 1117 "compiler/main.gem"
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
#line 1118 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1119 "compiler/main.gem"
    GemVal _t2528[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t2528, 1)), gem_string(";\n")));
#line 1120 "compiler/main.gem"
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
#line 1121 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1122 "compiler/main.gem"
    GemVal _t2529[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t2529, 1)), gem_string(";\n")));
#line 1123 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
#line 1125 "compiler/main.gem"
    GemVal _t2530[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2530, 1)), gem_string(";\n")));
#line 1126 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1129 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1132 "compiler/main.gem"
    GemVal _t2531[] = {gem_v_name};
    GemVal _t2532[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2531, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t2532, 2));
#line 1135 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1136 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1137 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1138 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1139 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1140 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1141 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1142 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1143 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1144 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1145 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1146 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1147 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1150 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1151 "compiler/main.gem"
    GemVal _t2533[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2533, 1), gem_int(0)))) {
#line 1152 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1154 "compiler/main.gem"
        GemVal gem_v__for_i_47 = gem_int(0);
#line 1154 "compiler/main.gem"
    GemVal _t2534[] = {gem_v_params};
        GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2534, 1);
#line 1154 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 1154 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_47;
#line 1154 "compiler/main.gem"
            gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 1155 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1156 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1158 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1159 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1160 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1161 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1162 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1163 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1164 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1165 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1166 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1167 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1168 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1169 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1170 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1172 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1177 "compiler/main.gem"
    GemVal _t2535[] = {gem_v_c_ret};
    GemVal _t2536[] = {gem_v_name};
    GemVal _t2537[] = {gem_v_c_params};
    GemVal _t2538[] = {(*gem_v_extern_c_decls), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2535, 1), gem_string(" ")), gem_to_string_fn(NULL, _t2536, 1)), gem_string("(")), gem_to_string_fn(NULL, _t2537, 1)), gem_string(");"))};
    (void)(gem_push_fn(NULL, _t2538, 2));
    GemVal _t2539 = gem_v_lines;
    gem_pop_frame();
    return _t2539;
}

struct _closure__anon_47 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_mangle;
    GemVal *gem_v_source_name;
};
static GemVal _anon_47(void *_env, GemVal *args, int argc) {
    struct _closure__anon_47 *_cls = (struct _closure__anon_47 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_47", "compiler/main.gem", 0);
#line 1185 "compiler/main.gem"
    GemVal gem_v__d48 = gem_v_node;
#line 1185 "compiler/main.gem"
    GemVal _t2541 = gem_v__d48;
    GemVal gem_v_params = gem_table_get(_t2541, gem_string("params"));
#line 1185 "compiler/main.gem"
    GemVal _t2542 = gem_v__d48;
    GemVal gem_v_body = gem_table_get(_t2542, gem_string("body"));
#line 1185 "compiler/main.gem"
    GemVal _t2543 = gem_v__d48;
    GemVal gem_v_name = gem_table_get(_t2543, gem_string("name"));

#line 1188 "compiler/main.gem"
    GemVal _t2544[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t2544, 1);
#line 1189 "compiler/main.gem"
    GemVal _t2545 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2545, gem_string("rest_param")), GEM_NIL))) {
#line 1190 "compiler/main.gem"
    GemVal _t2546 = gem_v_node;
    GemVal _t2547[] = {gem_v_scope_vars, gem_table_get(_t2546, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t2547, 2));
    }
#line 1192 "compiler/main.gem"
    GemVal _t2548 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2548, gem_string("block_param")), GEM_NIL))) {
#line 1193 "compiler/main.gem"
    GemVal _t2549 = gem_v_node;
    GemVal _t2550[] = {gem_v_scope_vars, gem_table_get(_t2549, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t2550, 2));
    }
#line 1195 "compiler/main.gem"
    GemVal gem_v__for_i_50 = gem_int(0);
#line 1195 "compiler/main.gem"
    GemVal _t2551[] = {gem_v_body};
    GemVal gem_v__for_limit_50 = gem_len_fn(NULL, _t2551, 1);
#line 1195 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
#line 1195 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_50;
#line 1195 "compiler/main.gem"
        gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 1196 "compiler/main.gem"
    GemVal _t2552[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2554;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2552, 1), gem_string("table")))) {
        _t2554 = gem_eq(gem_type_fn(NULL, _t2552, 1), gem_string("table"));
    } else {
        GemVal _t2553 = gem_table_get(gem_v_body, gem_v_i);
        _t2554 = gem_eq(gem_table_get(_t2553, gem_string("tag")), gem_string("let"));
    }
        if (gem_truthy(_t2554)) {
#line 1197 "compiler/main.gem"
    GemVal _t2555 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2556[] = {gem_v_scope_vars, gem_table_get(_t2555, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2556, 2));
        }
#line 1199 "compiler/main.gem"
    GemVal _t2557[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2559;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2557, 1), gem_string("table")))) {
        _t2559 = gem_eq(gem_type_fn(NULL, _t2557, 1), gem_string("table"));
    } else {
        GemVal _t2558 = gem_table_get(gem_v_body, gem_v_i);
        _t2559 = gem_eq(gem_table_get(_t2558, gem_string("tag")), gem_string("block"));
    }
        if (gem_truthy(_t2559)) {
#line 1200 "compiler/main.gem"
            GemVal gem_v__for_i_49 = gem_int(0);
#line 1200 "compiler/main.gem"
    GemVal _t2560 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2561[] = {gem_table_get(_t2560, gem_string("stmts"))};
            GemVal gem_v__for_limit_49 = gem_len_fn(NULL, _t2561, 1);
#line 1200 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_v__for_limit_49))) break;
#line 1200 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_49;
#line 1200 "compiler/main.gem"
                gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 1201 "compiler/main.gem"
    GemVal _t2562 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2563[] = {gem_table_get(gem_table_get(_t2562, gem_string("stmts")), gem_v_j)};
    GemVal _t2566;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2563, 1), gem_string("table")))) {
        _t2566 = gem_eq(gem_type_fn(NULL, _t2563, 1), gem_string("table"));
    } else {
        GemVal _t2564 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t2565 = gem_table_get(gem_table_get(_t2564, gem_string("stmts")), gem_v_j);
        _t2566 = gem_eq(gem_table_get(_t2565, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t2566)) {
#line 1202 "compiler/main.gem"
    GemVal _t2567 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2568 = gem_table_get(gem_table_get(_t2567, gem_string("stmts")), gem_v_j);
    GemVal _t2569[] = {gem_v_scope_vars, gem_table_get(_t2568, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t2569, 2));
                }
            }

        }
    }

#line 1207 "compiler/main.gem"
    GemVal _t2570[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t2571 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t2571.fn(_t2571.env, _t2570, 2);
#line 1209 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1210 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1211 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1212 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1214 "compiler/main.gem"
    GemVal _t2572[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2572, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1217 "compiler/main.gem"
    GemVal _t2573 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2573, gem_string("line")), GEM_NIL))) {
#line 1218 "compiler/main.gem"
    GemVal _t2574 = gem_v_node;
    GemVal _t2575[] = {gem_table_get(_t2574, gem_string("line"))};
    GemVal _t2576[] = {(*gem_v_source_name)};
    GemVal _t2577[] = {gem_fn_escape_c_string(NULL, _t2576, 1)};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2575, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2577, 1)), gem_string("\"\n")));
    }
#line 1222 "compiler/main.gem"
    GemVal gem_v__for_i_51 = gem_int(0);
#line 1222 "compiler/main.gem"
    GemVal _t2578[] = {gem_v_params};
    GemVal gem_v__for_limit_51 = gem_len_fn(NULL, _t2578, 1);
#line 1222 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_v__for_limit_51))) break;
#line 1222 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_51;
#line 1222 "compiler/main.gem"
        gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 1223 "compiler/main.gem"
    GemVal _t2579[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t2580 = (*gem_v_mangle);
        GemVal gem_v_mp = _t2580.fn(_t2580.env, _t2579, 1);
#line 1224 "compiler/main.gem"
    GemVal _t2581[] = {gem_v_captured, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2581, 2))) {
#line 1225 "compiler/main.gem"
    GemVal _t2582[] = {gem_v_mp};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2582, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1226 "compiler/main.gem"
    GemVal _t2583[] = {gem_v_mp};
    GemVal _t2584[] = {gem_v_i};
    GemVal _t2585[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2583, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2584, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2585, 1)), gem_string("] : GEM_NIL;\n")));
        } else {
#line 1228 "compiler/main.gem"
    GemVal _t2586[] = {gem_v_mp};
    GemVal _t2587[] = {gem_v_i};
    GemVal _t2588[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2586, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2587, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2588, 1)), gem_string("] : GEM_NIL;\n")));
        }
    }

#line 1233 "compiler/main.gem"
    GemVal _t2589 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2589, gem_string("rest_param")), GEM_NIL))) {
#line 1234 "compiler/main.gem"
    GemVal _t2590[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t2590, 1);
#line 1235 "compiler/main.gem"
        GemVal gem_v_end_count = gem_int(0);
#line 1236 "compiler/main.gem"
    GemVal _t2591 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2591, gem_string("block_param")), GEM_NIL))) {
#line 1237 "compiler/main.gem"
            gem_v_end_count = gem_int(1);
        }
#line 1239 "compiler/main.gem"
    GemVal _t2592 = gem_v_node;
    GemVal _t2593[] = {gem_table_get(_t2592, gem_string("rest_param"))};
    GemVal _t2594 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t2594.fn(_t2594.env, _t2593, 1);
#line 1240 "compiler/main.gem"
    GemVal _t2595 = gem_v_node;
    GemVal _t2596[] = {gem_v_captured, gem_table_get(_t2595, gem_string("rest_param"))};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2596, 2))) {
#line 1241 "compiler/main.gem"
    GemVal _t2597[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2597, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1242 "compiler/main.gem"
    GemVal _t2598[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2598, 1)), gem_string(" = gem_table_new();\n")));
#line 1243 "compiler/main.gem"
    GemVal _t2599[] = {gem_v_N};
    GemVal _t2600[] = {gem_v_end_count};
    GemVal _t2601[] = {gem_v_mp_rest};
    GemVal _t2602[] = {gem_v_N};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t2599, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t2600, 1)), gem_string("; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t2601, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t2602, 1)), gem_string("), args[_ri]);\n")));
        } else {
#line 1245 "compiler/main.gem"
    GemVal _t2603[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2603, 1)), gem_string(" = gem_table_new();\n")));
#line 1246 "compiler/main.gem"
    GemVal _t2604[] = {gem_v_N};
    GemVal _t2605[] = {gem_v_end_count};
    GemVal _t2606[] = {gem_v_mp_rest};
    GemVal _t2607[] = {gem_v_N};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t2604, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t2605, 1)), gem_string("; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t2606, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t2607, 1)), gem_string("), args[_ri]);\n")));
        }
#line 1248 "compiler/main.gem"
    GemVal _t2608 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2608, gem_string("block_param")), GEM_NIL))) {
#line 1249 "compiler/main.gem"
    GemVal _t2609 = gem_v_node;
    GemVal _t2610[] = {gem_table_get(_t2609, gem_string("block_param"))};
    GemVal _t2611 = (*gem_v_mangle);
            GemVal gem_v_mp_block = _t2611.fn(_t2611.env, _t2610, 1);
#line 1250 "compiler/main.gem"
    GemVal _t2612 = gem_v_node;
    GemVal _t2613[] = {gem_v_captured, gem_table_get(_t2612, gem_string("block_param"))};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2613, 2))) {
#line 1251 "compiler/main.gem"
    GemVal _t2614[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2614, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1252 "compiler/main.gem"
    GemVal _t2615[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2615, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            } else {
#line 1254 "compiler/main.gem"
    GemVal _t2616[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2616, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            }
        }
    }
#line 1259 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1260 "compiler/main.gem"
    GemVal _t2617 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2617, gem_string("line")), GEM_NIL))) {
#line 1261 "compiler/main.gem"
    GemVal _t2618 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t2618, gem_string("line"));
    }
#line 1263 "compiler/main.gem"
    GemVal _t2619[] = {gem_v_name};
    GemVal _t2620[] = {gem_fn_escape_c_string(NULL, _t2619, 1)};
    GemVal _t2621[] = {(*gem_v_source_name)};
    GemVal _t2622[] = {gem_fn_escape_c_string(NULL, _t2621, 1)};
    GemVal _t2623[] = {gem_v_fn_line};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2620, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2622, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2623, 1)), gem_string(");\n")));
#line 1266 "compiler/main.gem"
    GemVal _t2624[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2624, 1), gem_int(0)))) {
#line 1267 "compiler/main.gem"
        GemVal gem_v__for_i_52 = gem_int(0);
#line 1267 "compiler/main.gem"
    GemVal _t2625[] = {gem_v_body};
        GemVal gem_v__for_limit_52 = gem_sub(gem_len_fn(NULL, _t2625, 1), gem_int(1));
#line 1267 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 1267 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_52;
#line 1267 "compiler/main.gem"
            gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 1268 "compiler/main.gem"
    GemVal _t2626[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t2627 = (*gem_v_compile_stmt);
            gem_v_lines = gem_add(gem_add(gem_v_lines, _t2627.fn(_t2627.env, _t2626, 2)), gem_string("\n"));
        }

#line 1270 "compiler/main.gem"
    GemVal _t2628[] = {gem_v_body};
    GemVal _t2629[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2628, 1), gem_int(1))), gem_int(1)};
    GemVal _t2630 = (*gem_v_compile_stmt_return);
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t2630.fn(_t2630.env, _t2629, 2)), gem_string("\n"));
    } else {
#line 1272 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
#line 1275 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1276 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1277 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
    GemVal _t2631 = gem_v_lines;
    gem_pop_frame();
    return _t2631;
}

struct _closure__anon_48 {
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
static GemVal _anon_48(void *_env, GemVal *args, int argc) {
    struct _closure__anon_48 *_cls = (struct _closure__anon_48 *)_env;
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
    gem_push_frame("_anon_48", "compiler/main.gem", 0);
#line 1284 "compiler/main.gem"
    GemVal gem_v__d53 = gem_v_node;
#line 1284 "compiler/main.gem"
    GemVal _t2633 = gem_v__d53;
    GemVal gem_v_params = gem_table_get(_t2633, gem_string("params"));
#line 1284 "compiler/main.gem"
    GemVal _t2634 = gem_v__d53;
    GemVal gem_v_body = gem_table_get(_t2634, gem_string("body"));

#line 1285 "compiler/main.gem"
    GemVal _t2635 = (*gem_v_anon_name);
    GemVal gem_v_name = _t2635.fn(_t2635.env, NULL, 0);
#line 1288 "compiler/main.gem"
    GemVal _t2636[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t2636, 1);
#line 1289 "compiler/main.gem"
    GemVal _t2637 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2637, gem_string("rest_param")), GEM_NIL))) {
#line 1290 "compiler/main.gem"
    GemVal _t2638 = gem_v_node;
    GemVal _t2639[] = {gem_v_inner_scope, gem_table_get(_t2638, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t2639, 2));
    }
#line 1292 "compiler/main.gem"
    GemVal _t2640 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2640, gem_string("block_param")), GEM_NIL))) {
#line 1293 "compiler/main.gem"
    GemVal _t2641 = gem_v_node;
    GemVal _t2642[] = {gem_v_inner_scope, gem_table_get(_t2641, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t2642, 2));
    }
#line 1295 "compiler/main.gem"
    GemVal gem_v__for_i_55 = gem_int(0);
#line 1295 "compiler/main.gem"
    GemVal _t2643[] = {gem_v_body};
    GemVal gem_v__for_limit_55 = gem_len_fn(NULL, _t2643, 1);
#line 1295 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_v__for_limit_55))) break;
#line 1295 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_55;
#line 1295 "compiler/main.gem"
        gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1296 "compiler/main.gem"
    GemVal _t2644[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2646;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2644, 1), gem_string("table")))) {
        _t2646 = gem_eq(gem_type_fn(NULL, _t2644, 1), gem_string("table"));
    } else {
        GemVal _t2645 = gem_table_get(gem_v_body, gem_v_i);
        _t2646 = gem_eq(gem_table_get(_t2645, gem_string("tag")), gem_string("let"));
    }
        if (gem_truthy(_t2646)) {
#line 1297 "compiler/main.gem"
    GemVal _t2647 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2648[] = {gem_v_inner_scope, gem_table_get(_t2647, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2648, 2));
        }
#line 1299 "compiler/main.gem"
    GemVal _t2649[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2651;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2649, 1), gem_string("table")))) {
        _t2651 = gem_eq(gem_type_fn(NULL, _t2649, 1), gem_string("table"));
    } else {
        GemVal _t2650 = gem_table_get(gem_v_body, gem_v_i);
        _t2651 = gem_eq(gem_table_get(_t2650, gem_string("tag")), gem_string("block"));
    }
        if (gem_truthy(_t2651)) {
#line 1300 "compiler/main.gem"
            GemVal gem_v__for_i_54 = gem_int(0);
#line 1300 "compiler/main.gem"
    GemVal _t2652 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2653[] = {gem_table_get(_t2652, gem_string("stmts"))};
            GemVal gem_v__for_limit_54 = gem_len_fn(NULL, _t2653, 1);
#line 1300 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_v__for_limit_54))) break;
#line 1300 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_54;
#line 1300 "compiler/main.gem"
                gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 1301 "compiler/main.gem"
    GemVal _t2654 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2655[] = {gem_table_get(gem_table_get(_t2654, gem_string("stmts")), gem_v_j)};
    GemVal _t2658;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2655, 1), gem_string("table")))) {
        _t2658 = gem_eq(gem_type_fn(NULL, _t2655, 1), gem_string("table"));
    } else {
        GemVal _t2656 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t2657 = gem_table_get(gem_table_get(_t2656, gem_string("stmts")), gem_v_j);
        _t2658 = gem_eq(gem_table_get(_t2657, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t2658)) {
#line 1302 "compiler/main.gem"
    GemVal _t2659 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2660 = gem_table_get(gem_table_get(_t2659, gem_string("stmts")), gem_v_j);
    GemVal _t2661[] = {gem_v_inner_scope, gem_table_get(_t2660, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t2661, 2));
                }
            }

        }
    }

#line 1307 "compiler/main.gem"
    GemVal _t2662[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t2663 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t2663.fn(_t2663.env, _t2662, 2);
#line 1309 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1310 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1311 "compiler/main.gem"
    GemVal _t2664[] = {gem_v_captures};
    GemVal _t2665[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t2664, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t2665, 2);
#line 1312 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1315 "compiler/main.gem"
    GemVal _t2666[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t2666, 1));
#line 1316 "compiler/main.gem"
    GemVal _t2667[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t2667, 1)), gem_string(" {\n"));
#line 1317 "compiler/main.gem"
    GemVal gem_v__for_items_56 = gem_v_captures;
#line 1317 "compiler/main.gem"
    GemVal gem_v__for_i_56 = gem_int(0);
#line 1317 "compiler/main.gem"
    while (1) {
        GemVal _t2668[] = {gem_v__for_items_56};
        if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_len_fn(NULL, _t2668, 1)))) break;
#line 1317 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_56, gem_v__for_i_56);
#line 1317 "compiler/main.gem"
        gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1318 "compiler/main.gem"
    GemVal _t2669[] = {gem_v_cap};
    GemVal _t2670 = (*gem_v_mangle);
    GemVal _t2671[] = {_t2670.fn(_t2670.env, _t2669, 1)};
        gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2671, 1)), gem_string(";\n")));
    }

#line 1320 "compiler/main.gem"
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
#line 1323 "compiler/main.gem"
    GemVal _t2672[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t2672, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1324 "compiler/main.gem"
    GemVal _t2673[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2673, 1), gem_int(0)))) {
#line 1325 "compiler/main.gem"
    GemVal _t2674[] = {gem_v_struct_name};
    GemVal _t2675[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2674, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t2675, 1)), gem_string(" *)_env;\n")));
#line 1326 "compiler/main.gem"
        GemVal gem_v__for_items_57 = gem_v_captures;
#line 1326 "compiler/main.gem"
        GemVal gem_v__for_i_57 = gem_int(0);
#line 1326 "compiler/main.gem"
        while (1) {
            GemVal _t2676[] = {gem_v__for_items_57};
            if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t2676, 1)))) break;
#line 1326 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
#line 1326 "compiler/main.gem"
            gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
#line 1327 "compiler/main.gem"
    GemVal _t2677[] = {gem_v_cap};
    GemVal _t2678 = (*gem_v_mangle);
            GemVal gem_v_mc = _t2678.fn(_t2678.env, _t2677, 1);
#line 1328 "compiler/main.gem"
    GemVal _t2679[] = {gem_v_mc};
    GemVal _t2680[] = {gem_v_mc};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2679, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t2680, 1)), gem_string(";\n")));
        }

    }
#line 1333 "compiler/main.gem"
    GemVal gem_v__for_i_58 = gem_int(0);
#line 1333 "compiler/main.gem"
    GemVal _t2681[] = {gem_v_params};
    GemVal gem_v__for_limit_58 = gem_len_fn(NULL, _t2681, 1);
#line 1333 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_v__for_limit_58))) break;
#line 1333 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_58;
#line 1333 "compiler/main.gem"
        gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1334 "compiler/main.gem"
    GemVal _t2682[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t2683 = (*gem_v_mangle);
        GemVal gem_v_mp = _t2683.fn(_t2683.env, _t2682, 1);
#line 1335 "compiler/main.gem"
    GemVal _t2684[] = {gem_v_inner_captured, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2684, 2))) {
#line 1336 "compiler/main.gem"
    GemVal _t2685[] = {gem_v_mp};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2685, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1337 "compiler/main.gem"
    GemVal _t2686[] = {gem_v_mp};
    GemVal _t2687[] = {gem_v_i};
    GemVal _t2688[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2686, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2687, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2688, 1)), gem_string("] : GEM_NIL;\n")));
        } else {
#line 1339 "compiler/main.gem"
    GemVal _t2689[] = {gem_v_mp};
    GemVal _t2690[] = {gem_v_i};
    GemVal _t2691[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2689, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2690, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2691, 1)), gem_string("] : GEM_NIL;\n")));
        }
    }

#line 1344 "compiler/main.gem"
    GemVal _t2692 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2692, gem_string("rest_param")), GEM_NIL))) {
#line 1345 "compiler/main.gem"
    GemVal _t2693[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t2693, 1);
#line 1346 "compiler/main.gem"
        GemVal gem_v_end_count = gem_int(0);
#line 1347 "compiler/main.gem"
    GemVal _t2694 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2694, gem_string("block_param")), GEM_NIL))) {
#line 1348 "compiler/main.gem"
            gem_v_end_count = gem_int(1);
        }
#line 1350 "compiler/main.gem"
    GemVal _t2695 = gem_v_node;
    GemVal _t2696[] = {gem_table_get(_t2695, gem_string("rest_param"))};
    GemVal _t2697 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t2697.fn(_t2697.env, _t2696, 1);
#line 1351 "compiler/main.gem"
    GemVal _t2698 = gem_v_node;
    GemVal _t2699[] = {gem_v_inner_captured, gem_table_get(_t2698, gem_string("rest_param"))};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2699, 2))) {
#line 1352 "compiler/main.gem"
    GemVal _t2700[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2700, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1353 "compiler/main.gem"
    GemVal _t2701[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2701, 1)), gem_string(" = gem_table_new();\n")));
#line 1354 "compiler/main.gem"
    GemVal _t2702[] = {gem_v_N};
    GemVal _t2703[] = {gem_v_end_count};
    GemVal _t2704[] = {gem_v_mp_rest};
    GemVal _t2705[] = {gem_v_N};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t2702, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t2703, 1)), gem_string("; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t2704, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t2705, 1)), gem_string("), args[_ri]);\n")));
        } else {
#line 1356 "compiler/main.gem"
    GemVal _t2706[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2706, 1)), gem_string(" = gem_table_new();\n")));
#line 1357 "compiler/main.gem"
    GemVal _t2707[] = {gem_v_N};
    GemVal _t2708[] = {gem_v_end_count};
    GemVal _t2709[] = {gem_v_mp_rest};
    GemVal _t2710[] = {gem_v_N};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t2707, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t2708, 1)), gem_string("; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t2709, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t2710, 1)), gem_string("), args[_ri]);\n")));
        }
#line 1359 "compiler/main.gem"
    GemVal _t2711 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2711, gem_string("block_param")), GEM_NIL))) {
#line 1360 "compiler/main.gem"
    GemVal _t2712 = gem_v_node;
    GemVal _t2713[] = {gem_table_get(_t2712, gem_string("block_param"))};
    GemVal _t2714 = (*gem_v_mangle);
            GemVal gem_v_mp_block = _t2714.fn(_t2714.env, _t2713, 1);
#line 1361 "compiler/main.gem"
    GemVal _t2715 = gem_v_node;
    GemVal _t2716[] = {gem_v_inner_captured, gem_table_get(_t2715, gem_string("block_param"))};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2716, 2))) {
#line 1362 "compiler/main.gem"
    GemVal _t2717[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2717, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1363 "compiler/main.gem"
    GemVal _t2718[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2718, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            } else {
#line 1365 "compiler/main.gem"
    GemVal _t2719[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2719, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            }
        }
    }
#line 1370 "compiler/main.gem"
    GemVal _t2720[] = {gem_v_name};
    GemVal _t2721[] = {gem_fn_escape_c_string(NULL, _t2720, 1)};
    GemVal _t2722[] = {(*gem_v_source_name)};
    GemVal _t2723[] = {gem_fn_escape_c_string(NULL, _t2722, 1)};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2721, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2723, 1)), gem_string("\", 0);\n")));
#line 1372 "compiler/main.gem"
    GemVal _t2724[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2724, 1), gem_int(0)))) {
#line 1373 "compiler/main.gem"
        GemVal gem_v__for_i_59 = gem_int(0);
#line 1373 "compiler/main.gem"
    GemVal _t2725[] = {gem_v_body};
        GemVal gem_v__for_limit_59 = gem_sub(gem_len_fn(NULL, _t2725, 1), gem_int(1));
#line 1373 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_v__for_limit_59))) break;
#line 1373 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_59;
#line 1373 "compiler/main.gem"
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1374 "compiler/main.gem"
    GemVal _t2726[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t2727 = (*gem_v_compile_stmt);
            gem_v_lines = gem_add(gem_add(gem_v_lines, _t2727.fn(_t2727.env, _t2726, 2)), gem_string("\n"));
        }

#line 1376 "compiler/main.gem"
    GemVal _t2728[] = {gem_v_body};
    GemVal _t2729[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2728, 1), gem_int(1))), gem_int(1)};
    GemVal _t2730 = (*gem_v_compile_stmt_return);
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t2730.fn(_t2730.env, _t2729, 2)), gem_string("\n"));
    } else {
#line 1378 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
#line 1381 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1382 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1383 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1386 "compiler/main.gem"
    GemVal _t2731[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t2731, 2));
    GemVal _t2732 = gem_table_new();
    gem_table_set(_t2732, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t2732, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t2732, gem_string("captures"), gem_v_captures);
    GemVal _t2733 = _t2732;
    gem_pop_frame();
    return _t2733;
}

struct _closure__anon_49 {
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
static GemVal _anon_49(void *_env, GemVal *args, int argc) {
    struct _closure__anon_49 *_cls = (struct _closure__anon_49 *)_env;
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
    gem_push_frame("_anon_49", "compiler/main.gem", 0);
#line 1395 "compiler/main.gem"
    GemVal _t2735 = gem_table_new();
    GemVal gem_v_fn_defs = _t2735;
#line 1396 "compiler/main.gem"
    GemVal _t2736 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t2736;
#line 1397 "compiler/main.gem"
    GemVal _t2737 = gem_table_new();
    GemVal gem_v_extern_includes = _t2737;
#line 1398 "compiler/main.gem"
    GemVal _t2738 = gem_table_new();
    GemVal gem_v_top_stmts = _t2738;
#line 1400 "compiler/main.gem"
    GemVal _t2739 = gem_v_ast;
    GemVal gem_v__for_items_60 = gem_table_get(_t2739, gem_string("stmts"));
#line 1400 "compiler/main.gem"
    GemVal gem_v__for_i_60 = gem_int(0);
#line 1400 "compiler/main.gem"
    while (1) {
        GemVal _t2740[] = {gem_v__for_items_60};
        if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_len_fn(NULL, _t2740, 1)))) break;
#line 1400 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_60, gem_v__for_i_60);
#line 1400 "compiler/main.gem"
        gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1401 "compiler/main.gem"
    GemVal _t2741 = gem_v_s;
        if (gem_truthy(gem_eq(gem_table_get(_t2741, gem_string("tag")), gem_string("fn_def")))) {
#line 1402 "compiler/main.gem"
    GemVal _t2742[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t2742, 2));
        } else {
#line 1403 "compiler/main.gem"
    GemVal _t2743 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t2743, gem_string("tag")), gem_string("extern_fn")))) {
#line 1404 "compiler/main.gem"
    GemVal _t2744[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t2744, 2));
            } else {
#line 1405 "compiler/main.gem"
    GemVal _t2745 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t2745, gem_string("tag")), gem_string("extern_include")))) {
#line 1406 "compiler/main.gem"
    GemVal _t2746[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t2746, 2));
                } else {
#line 1408 "compiler/main.gem"
    GemVal _t2747[] = {gem_v_top_stmts, gem_v_s};
                    (void)(gem_push_fn(NULL, _t2747, 2));
                }
            }
        }
    }

#line 1413 "compiler/main.gem"
    GemVal gem_v__for_items_61 = gem_v_extern_fns_list;
#line 1413 "compiler/main.gem"
    GemVal gem_v__for_i_61 = gem_int(0);
#line 1413 "compiler/main.gem"
    while (1) {
        GemVal _t2748[] = {gem_v__for_items_61};
        if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_len_fn(NULL, _t2748, 1)))) break;
#line 1413 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_61, gem_v__for_i_61);
#line 1413 "compiler/main.gem"
        gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1414 "compiler/main.gem"
    GemVal _t2749 = gem_v_ef;
    GemVal _t2750[] = {(*gem_v_defined_fns), gem_table_get(_t2749, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t2750, 2));
    }

#line 1418 "compiler/main.gem"
    GemVal gem_v__for_items_62 = gem_v_fn_defs;
#line 1418 "compiler/main.gem"
    GemVal gem_v__for_i_62 = gem_int(0);
#line 1418 "compiler/main.gem"
    while (1) {
        GemVal _t2751[] = {gem_v__for_items_62};
        if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_len_fn(NULL, _t2751, 1)))) break;
#line 1418 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_62, gem_v__for_i_62);
#line 1418 "compiler/main.gem"
        gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1419 "compiler/main.gem"
    GemVal _t2752 = gem_v_fd;
    GemVal _t2753[] = {(*gem_v_defined_fns), gem_table_get(_t2752, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t2753, 2));
#line 1420 "compiler/main.gem"
    GemVal _t2754 = gem_v_fd;
    GemVal _t2755[] = {gem_table_get(_t2754, gem_string("name"))};
        GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2755, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1421 "compiler/main.gem"
    GemVal _t2756[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t2756, 2));
    }

#line 1425 "compiler/main.gem"
    GemVal _t2757[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn_collect_top_let_names(NULL, _t2757, 2));
#line 1426 "compiler/main.gem"
    GemVal _t2758 = gem_table_new();
    GemVal _t2759[] = {gem_v_top_stmts, _t2758};
    GemVal _t2760 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t2760.fn(_t2760.env, _t2759, 2);
#line 1429 "compiler/main.gem"
    GemVal gem_v__for_items_63 = gem_v_extern_fns_list;
#line 1429 "compiler/main.gem"
    GemVal gem_v__for_i_63 = gem_int(0);
#line 1429 "compiler/main.gem"
    while (1) {
        GemVal _t2761[] = {gem_v__for_items_63};
        if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_len_fn(NULL, _t2761, 1)))) break;
#line 1429 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_63, gem_v__for_i_63);
#line 1429 "compiler/main.gem"
        gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1430 "compiler/main.gem"
    GemVal _t2762[] = {gem_v_ef};
    GemVal _t2763 = (*gem_v_compile_extern_fn);
    GemVal _t2764[] = {(*gem_v_functions), _t2763.fn(_t2763.env, _t2762, 1)};
        (void)(gem_push_fn(NULL, _t2764, 2));
    }

#line 1434 "compiler/main.gem"
    GemVal gem_v__for_items_64 = gem_v_fn_defs;
#line 1434 "compiler/main.gem"
    GemVal gem_v__for_i_64 = gem_int(0);
#line 1434 "compiler/main.gem"
    while (1) {
        GemVal _t2765[] = {gem_v__for_items_64};
        if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_len_fn(NULL, _t2765, 1)))) break;
#line 1434 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_64, gem_v__for_i_64);
#line 1434 "compiler/main.gem"
        gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1435 "compiler/main.gem"
    GemVal _t2766[] = {gem_v_fd};
    GemVal _t2767 = (*gem_v_compile_fn);
    GemVal _t2768[] = {(*gem_v_functions), _t2767.fn(_t2767.env, _t2766, 1)};
        (void)(gem_push_fn(NULL, _t2768, 2));
    }

#line 1439 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1440 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1441 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1442 "compiler/main.gem"
    GemVal _t2769[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t2770 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t2770.fn(_t2770.env, _t2769, 2);
#line 1443 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1444 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1446 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 1447 "compiler/main.gem"
    GemVal gem_v__for_items_65 = gem_v_fn_defs;
#line 1447 "compiler/main.gem"
    GemVal gem_v__for_i_65 = gem_int(0);
#line 1447 "compiler/main.gem"
    while (1) {
        GemVal _t2771[] = {gem_v__for_items_65};
        if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_len_fn(NULL, _t2771, 1)))) break;
#line 1447 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_65, gem_v__for_i_65);
#line 1447 "compiler/main.gem"
        gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1448 "compiler/main.gem"
    GemVal _t2772 = gem_v_fd;
        if (gem_truthy(gem_eq(gem_table_get(_t2772, gem_string("name")), gem_string("main")))) {
#line 1449 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 1453 "compiler/main.gem"
    GemVal gem_v_main_c = gem_string("int main(void) {\n");
#line 1454 "compiler/main.gem"
    GemVal _t2773[] = {(*gem_v_source_name)};
    GemVal _t2774[] = {gem_fn_escape_c_string(NULL, _t2773, 1)};
    gem_v_main_c = gem_add(gem_v_main_c, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t2774, 1)), gem_string("\", 0);\n")));
#line 1455 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1456 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
#line 1458 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1459 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
#line 1460 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
#line 1462 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_run_scheduler();\n"));
#line 1463 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_pop_frame();\n    return 0;\n}\n"));
#line 1467 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1468 "compiler/main.gem"
    GemVal gem_v__for_items_66 = gem_v_extern_includes;
#line 1468 "compiler/main.gem"
    GemVal gem_v__for_i_66 = gem_int(0);
#line 1468 "compiler/main.gem"
    while (1) {
        GemVal _t2775[] = {gem_v__for_items_66};
        if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_len_fn(NULL, _t2775, 1)))) break;
#line 1468 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_66, gem_v__for_i_66);
#line 1468 "compiler/main.gem"
        gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1469 "compiler/main.gem"
    GemVal _t2776 = gem_v_ei;
    GemVal _t2777[] = {gem_table_get(_t2776, gem_string("path"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t2777, 1)), gem_string("\"\n")));
    }

#line 1471 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
#line 1475 "compiler/main.gem"
    GemVal _t2778[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2778, 1), gem_int(0)))) {
#line 1476 "compiler/main.gem"
        GemVal gem_v__for_i_67 = gem_int(0);
#line 1476 "compiler/main.gem"
    GemVal _t2779[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_67 = gem_len_fn(NULL, _t2779, 1);
#line 1476 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_v__for_limit_67))) break;
#line 1476 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_67;
#line 1476 "compiler/main.gem"
            gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1477 "compiler/main.gem"
            gem_v_out = gem_add(gem_add(gem_v_out, gem_table_get((*gem_v_extern_c_decls), gem_v_i)), gem_string("\n"));
        }

    }
#line 1482 "compiler/main.gem"
    GemVal gem_v__for_i_68 = gem_int(0);
#line 1482 "compiler/main.gem"
    GemVal _t2780[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_68 = gem_len_fn(NULL, _t2780, 1);
#line 1482 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_v__for_limit_68))) break;
#line 1482 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_68;
#line 1482 "compiler/main.gem"
        gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1483 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
#line 1484 "compiler/main.gem"
    GemVal _t2781[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2781, 1), gem_int(1))))) {
#line 1485 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1488 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1491 "compiler/main.gem"
    GemVal _t2782[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t2782, 1);
#line 1492 "compiler/main.gem"
    GemVal gem_v__for_i_69 = gem_int(0);
#line 1492 "compiler/main.gem"
    GemVal _t2783[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_69 = gem_len_fn(NULL, _t2783, 1);
#line 1492 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_69, gem_v__for_limit_69))) break;
#line 1492 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_69;
#line 1492 "compiler/main.gem"
        gem_v__for_i_69 = gem_add(gem_v__for_i_69, gem_int(1));
#line 1493 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 1494 "compiler/main.gem"
    GemVal _t2784[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2784, 2))) {
#line 1495 "compiler/main.gem"
    GemVal _t2785[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal *gem_v_"), gem_to_string_fn(NULL, _t2785, 1)), gem_string(";\n")));
        } else {
#line 1497 "compiler/main.gem"
    GemVal _t2786[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal gem_v_"), gem_to_string_fn(NULL, _t2786, 1)), gem_string(";\n")));
        }
    }

#line 1500 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1503 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1503 "compiler/main.gem"
    GemVal _t2787[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_70 = gem_len_fn(NULL, _t2787, 1);
#line 1503 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_70, gem_v__for_limit_70))) break;
#line 1503 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_70;
#line 1503 "compiler/main.gem"
        gem_v__for_i_70 = gem_add(gem_v__for_i_70, gem_int(1));
#line 1504 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
#line 1505 "compiler/main.gem"
    GemVal _t2788[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2788, 1), gem_int(1))))) {
#line 1506 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
        }
    }

#line 1509 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
#line 1511 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    GemVal _t2789 = gem_v_out;
    gem_pop_frame();
    return _t2789;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 129 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_codegen", "compiler/main.gem", 129);
#line 130 "compiler/main.gem"
    GemVal _t1199 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1199;
#line 131 "compiler/main.gem"
    GemVal _t1200 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1200;
#line 132 "compiler/main.gem"
    GemVal _t1201 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1201;
#line 133 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 134 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 135 "compiler/main.gem"
    GemVal _t1202 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1202;
#line 136 "compiler/main.gem"
    GemVal _t1203 = gem_table_new();
    gem_table_set(_t1203, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1203, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1203, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1203, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1203, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1203, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1203, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1203, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1203, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1203, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1203, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1203, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1203, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1203, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1203, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1203, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1203, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1203, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1203, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1203, gem_string("self"), gem_string("gem_self_builtin"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1203;
#line 158 "compiler/main.gem"
    GemVal _t1204 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1204;
#line 159 "compiler/main.gem"
    GemVal gem_v__for_tbl_9 = (*gem_v_builtin_fns);
#line 159 "compiler/main.gem"
    GemVal _t1205[] = {gem_v__for_tbl_9};
    GemVal gem_v__for_keys_9 = gem_keys_fn(NULL, _t1205, 1);
#line 159 "compiler/main.gem"
    GemVal gem_v__for_i_9 = gem_int(0);
#line 159 "compiler/main.gem"
    while (1) {
        GemVal _t1206[] = {gem_v__for_keys_9};
        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_len_fn(NULL, _t1206, 1)))) break;
#line 159 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_9, gem_v__for_i_9);
#line 159 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_9, gem_table_get(gem_v__for_keys_9, gem_v__for_i_9));
#line 159 "compiler/main.gem"
        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 160 "compiler/main.gem"
        gem_table_set((*gem_v_builtins), gem_v_k, gem_bool(1));
    }

#line 162 "compiler/main.gem"
    GemVal _t1207 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1207;
#line 163 "compiler/main.gem"
    GemVal _t1208 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1208;
#line 164 "compiler/main.gem"
    GemVal _t1209 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1209;
#line 165 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 169 "compiler/main.gem"
    struct _closure__anon_22 *_t1212 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1212->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_22, _t1212);
#line 174 "compiler/main.gem"
    struct _closure__anon_23 *_t1215 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1215->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_23, _t1215);
#line 179 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_24, NULL);
#line 185 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 186 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 188 "compiler/main.gem"
    struct _closure__anon_25 *_t1390 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1390->gem_v_builtins = gem_v_builtins;
    _t1390->gem_v_collect_free_node = gem_v_collect_free_node;
    _t1390->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1390->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_25, _t1390);
#line 325 "compiler/main.gem"
    struct _closure__anon_26 *_t1416 = GC_MALLOC(sizeof(struct _closure__anon_26));
    _t1416->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_26, _t1416);
#line 346 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 347 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 349 "compiler/main.gem"
    struct _closure__anon_27 *_t1508 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1508->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1508->gem_v_walk_captures = gem_v_walk_captures;
    _t1508->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_27, _t1508);
#line 428 "compiler/main.gem"
    struct _closure__anon_28 *_t1532 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1532->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_28, _t1532);
#line 445 "compiler/main.gem"
    struct _closure__anon_29 *_t1537 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t1537->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_29, _t1537);
#line 453 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 454 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 455 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 456 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 457 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 458 "compiler/main.gem"
    GemVal *gem_v_compile_if_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if_return = GEM_NIL;
#line 459 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 460 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 461 "compiler/main.gem"
    GemVal *gem_v_compile_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match_return = GEM_NIL;
#line 462 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 463 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 464 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 465 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 466 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 467 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 468 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 469 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 473 "compiler/main.gem"
    struct _closure__anon_30 *_t1666 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t1666->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1666->gem_v_builtin_fns = gem_v_builtin_fns;
    _t1666->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t1666->gem_v_compile_array = gem_v_compile_array;
    _t1666->gem_v_compile_binop = gem_v_compile_binop;
    _t1666->gem_v_compile_call = gem_v_compile_call;
    _t1666->gem_v_compile_expr = gem_v_compile_expr;
    _t1666->gem_v_compile_table = gem_v_compile_table;
    _t1666->gem_v_defined_fns = gem_v_defined_fns;
    _t1666->gem_v_mangle = gem_v_mangle;
    _t1666->gem_v_tmp = gem_v_tmp;
    _t1666->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_30, _t1666);
#line 561 "compiler/main.gem"
    struct _closure__anon_31 *_t1683 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t1683->gem_v_compile_expr = gem_v_compile_expr;
    _t1683->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_31, _t1683);
#line 576 "compiler/main.gem"
    struct _closure__anon_32 *_t1697 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t1697->gem_v_compile_expr = gem_v_compile_expr;
    _t1697->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_32, _t1697);
#line 590 "compiler/main.gem"
    struct _closure__anon_33 *_t1741 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t1741->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1741->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1741->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t1741->gem_v_mangle = gem_v_mangle;
    _t1741->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_33, _t1741);
#line 625 "compiler/main.gem"
    struct _closure__anon_34 *_t1757 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t1757->gem_v_compile_expr = gem_v_compile_expr;
    _t1757->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_34, _t1757);
#line 646 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_35, NULL);
#line 655 "compiler/main.gem"
    struct _closure__anon_36 *_t1845 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t1845->gem_v_builtin_fns = gem_v_builtin_fns;
    _t1845->gem_v_compile_args = gem_v_compile_args;
    _t1845->gem_v_compile_expr = gem_v_compile_expr;
    _t1845->gem_v_defined_fns = gem_v_defined_fns;
    _t1845->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t1845->gem_v_source_name = gem_v_source_name;
    _t1845->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_36, _t1845);
#line 699 "compiler/main.gem"
    struct _closure__anon_37 *_t1998 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t1998->gem_v_compile_expr = gem_v_compile_expr;
    _t1998->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_37, _t1998);
#line 778 "compiler/main.gem"
    struct _closure__anon_38 *_t2003 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2003->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_38, _t2003);
#line 788 "compiler/main.gem"
    struct _closure__anon_39 *_t2184 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2184->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2184->gem_v_compile_expr = gem_v_compile_expr;
    _t2184->gem_v_compile_if = gem_v_compile_if;
    _t2184->gem_v_compile_match = gem_v_compile_match;
    _t2184->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2184->gem_v_compile_while = gem_v_compile_while;
    _t2184->gem_v_in_top_level = gem_v_in_top_level;
    _t2184->gem_v_mangle = gem_v_mangle;
    _t2184->gem_v_source_name = gem_v_source_name;
    _t2184->gem_v_tmp = gem_v_tmp;
    _t2184->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t2184->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_39, _t2184);
#line 864 "compiler/main.gem"
    struct _closure__anon_40 *_t2333 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t2333->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2333->gem_v_compile_expr = gem_v_compile_expr;
    _t2333->gem_v_compile_if_return = gem_v_compile_if_return;
    _t2333->gem_v_compile_match_return = gem_v_compile_match_return;
    _t2333->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2333->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2333->gem_v_compile_while = gem_v_compile_while;
    _t2333->gem_v_mangle = gem_v_mangle;
    _t2333->gem_v_source_name = gem_v_source_name;
    _t2333->gem_v_tmp = gem_v_tmp;
    _t2333->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_40, _t2333);
#line 923 "compiler/main.gem"
    struct _closure__anon_41 *_t2352 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2352->gem_v_compile_expr = gem_v_compile_expr;
    _t2352->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_41, _t2352);
#line 939 "compiler/main.gem"
    struct _closure__anon_42 *_t2388 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t2388->gem_v_compile_expr = gem_v_compile_expr;
    _t2388->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2388->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_42, _t2388);
#line 974 "compiler/main.gem"
    struct _closure__anon_43 *_t2406 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t2406->gem_v_compile_expr = gem_v_compile_expr;
    _t2406->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_43, _t2406);
#line 989 "compiler/main.gem"
    struct _closure__anon_44 *_t2439 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t2439->gem_v_compile_expr = gem_v_compile_expr;
    _t2439->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2439->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_44, _t2439);
#line 1016 "compiler/main.gem"
    struct _closure__anon_45 *_t2489 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t2489->gem_v_compile_expr = gem_v_compile_expr;
    _t2489->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2489->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2489->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_45, _t2489);
#line 1062 "compiler/main.gem"
    struct _closure__anon_46 *_t2540 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t2540->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t2540->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_46, _t2540);
#line 1184 "compiler/main.gem"
    struct _closure__anon_47 *_t2632 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t2632->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2632->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2632->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2632->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2632->gem_v_in_top_level = gem_v_in_top_level;
    _t2632->gem_v_mangle = gem_v_mangle;
    _t2632->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_fn = gem_make_fn(_anon_47, _t2632);
#line 1283 "compiler/main.gem"
    struct _closure__anon_48 *_t2734 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t2734->gem_v_anon_name = gem_v_anon_name;
    _t2734->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2734->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2734->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2734->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2734->gem_v_functions = gem_v_functions;
    _t2734->gem_v_in_top_level = gem_v_in_top_level;
    _t2734->gem_v_mangle = gem_v_mangle;
    _t2734->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_48, _t2734);
#line 1393 "compiler/main.gem"
    struct _closure__anon_49 *_t2790 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t2790->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2790->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t2790->gem_v_compile_fn = gem_v_compile_fn;
    _t2790->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2790->gem_v_defined_fns = gem_v_defined_fns;
    _t2790->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t2790->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2790->gem_v_forward_decls = gem_v_forward_decls;
    _t2790->gem_v_functions = gem_v_functions;
    _t2790->gem_v_in_top_level = gem_v_in_top_level;
    _t2790->gem_v_source_name = gem_v_source_name;
    _t2790->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t2790->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_49, _t2790);
    GemVal _t2791 = gem_table_new();
    gem_table_set(_t2791, gem_string("compile"), gem_v_compile);
    GemVal _t2792 = _t2791;
    gem_pop_frame();
    return _t2792;
}

static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc) {
#line 1521 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("str_replace", "compiler/main.gem", 1521);
#line 1522 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 1523 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 1524 "compiler/main.gem"
    GemVal _t2793[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t2793, 1);
#line 1525 "compiler/main.gem"
    GemVal _t2794[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t2794, 1);
#line 1526 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
#line 1527 "compiler/main.gem"
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
#line 1528 "compiler/main.gem"
            GemVal gem_v_found = gem_bool(1);
#line 1529 "compiler/main.gem"
            GemVal gem_v_j = gem_int(0);
#line 1530 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
#line 1531 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
#line 1532 "compiler/main.gem"
                    gem_v_found = gem_bool(0);
                    break;
                }
#line 1535 "compiler/main.gem"
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
#line 1537 "compiler/main.gem"
            if (gem_truthy(gem_v_found)) {
#line 1538 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
#line 1539 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
#line 1543 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
#line 1544 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t2795 = gem_v_result;
    gem_pop_frame();
    return _t2795;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 24 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 24);
#line 25 "compiler/main.gem"
    GemVal _t2796 = gem_table_new();
    GemVal gem_v_new_stmts = _t2796;
#line 26 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 27 "compiler/main.gem"
    GemVal _t2797 = gem_v_ast;
    GemVal gem_v__for_items_1 = gem_table_get(_t2797, gem_string("stmts"));
#line 27 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 27 "compiler/main.gem"
    while (1) {
        GemVal _t2798[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t2798, 1)))) break;
#line 27 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 27 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 28 "compiler/main.gem"
    GemVal _t2799[] = {gem_v_stmt};
    GemVal _t2801;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2799, 1), gem_string("table")))) {
        _t2801 = gem_eq(gem_type_fn(NULL, _t2799, 1), gem_string("table"));
    } else {
        GemVal _t2800 = gem_v_stmt;
        _t2801 = gem_eq(gem_table_get(_t2800, gem_string("tag")), gem_string("load"));
    }
        if (gem_truthy(_t2801)) {
#line 29 "compiler/main.gem"
    GemVal _t2802 = gem_v_stmt;
            GemVal gem_v_path = gem_table_get(_t2802, gem_string("path"));
#line 31 "compiler/main.gem"
    GemVal _t2803[] = {gem_v_path};
    GemVal _t2811;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t2803, 1), gem_int(4)))) {
        _t2811 = gem_lt(gem_len_fn(NULL, _t2803, 1), gem_int(4));
    } else {
        GemVal _t2804[] = {gem_v_path};
        GemVal _t2806;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2804, 1), gem_int(4))), gem_string(".")))) {
                _t2806 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2804, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t2805[] = {gem_v_path};
                _t2806 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2805, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t2808;
        if (gem_truthy(_t2806)) {
                _t2808 = _t2806;
        } else {
                GemVal _t2807[] = {gem_v_path};
                _t2808 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2807, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t2810;
        if (gem_truthy(_t2808)) {
                _t2810 = _t2808;
        } else {
                GemVal _t2809[] = {gem_v_path};
                _t2810 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2809, 1), gem_int(1))), gem_string("m"));
        }
        _t2811 = _t2810;
    }
            if (gem_truthy(_t2811)) {
#line 32 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 36 "compiler/main.gem"
    GemVal _t2812[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_fn_gem_path_join(NULL, _t2812, 2);
#line 39 "compiler/main.gem"
    GemVal _t2813[] = {gem_v_full_path};
            if (gem_truthy(gem_eq(gem_fn_gem_file_exists(NULL, _t2813, 1), gem_int(0)))) {
#line 40 "compiler/main.gem"
    GemVal _t2814[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_fn_gem_path_join(NULL, _t2814, 2);
            }
#line 43 "compiler/main.gem"
    GemVal _t2815[] = {gem_v_full_path};
            gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t2815, 1);
#line 46 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                continue;
            }
#line 49 "compiler/main.gem"
            gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 51 "compiler/main.gem"
    GemVal _t2816[] = {gem_v_full_path};
            GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2816, 1);
#line 52 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 53 "compiler/main.gem"
    GemVal _t2817[] = {gem_v_full_path};
    GemVal _t2818[] = {gem_add(gem_add(gem_string("cannot load '"), gem_to_string_fn(NULL, _t2817, 1)), gem_string("'"))};
                (void)(gem_error_at_fn("compiler/main.gem", 53, _t2818, 1));
            }
#line 56 "compiler/main.gem"
    GemVal _t2819[] = {gem_v_source};
            GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t2819, 1);
#line 57 "compiler/main.gem"
    GemVal _t2820[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t2820, 1);
#line 58 "compiler/main.gem"
    GemVal _t2821[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t2821, 3);
#line 60 "compiler/main.gem"
    GemVal _t2822 = gem_v_resolved;
            GemVal gem_v__for_items_2 = gem_table_get(_t2822, gem_string("stmts"));
#line 60 "compiler/main.gem"
            GemVal gem_v__for_i_2 = gem_int(0);
#line 60 "compiler/main.gem"
            while (1) {
                GemVal _t2823[] = {gem_v__for_items_2};
                if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t2823, 1)))) break;
#line 60 "compiler/main.gem"
                GemVal gem_v_rs = gem_table_get(gem_v__for_items_2, gem_v__for_i_2);
#line 60 "compiler/main.gem"
                gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 61 "compiler/main.gem"
                gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_rs);
#line 62 "compiler/main.gem"
                gem_v_count = gem_add(gem_v_count, gem_int(1));
            }

        } else {
#line 65 "compiler/main.gem"
            gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_stmt);
#line 66 "compiler/main.gem"
            gem_v_count = gem_add(gem_v_count, gem_int(1));
        }
    }

#line 69 "compiler/main.gem"
    GemVal _t2824[] = {gem_v_new_stmts};
    GemVal _t2825 = gem_fn_make_program(NULL, _t2824, 1);
    gem_pop_frame();
    return _t2825;
}

int main(void) {
    gem_push_frame("main", "compiler/main.gem", 0);
#line 20 "compiler/main.gem"
    GemVal _t2826[] = {gem_int(0)};
    GemVal _t2827[] = {gem_fn_gem_get_argv(NULL, _t2826, 1)};
    GemVal _t2828[] = {gem_fn_gem_dirname(NULL, _t2827, 1)};
    gem_v_install_root = gem_fn_gem_dirname(NULL, _t2828, 1);
#line 74 "compiler/main.gem"
    gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
#line 75 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 76 "compiler/main.gem"
    GemVal _t2829[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 76, _t2829, 1));
    }
#line 79 "compiler/main.gem"
    GemVal _t2830[] = {gem_int(1)};
    gem_v_src_path = gem_fn_gem_get_argv(NULL, _t2830, 1);
#line 80 "compiler/main.gem"
    GemVal _t2831[] = {gem_v_src_path};
    gem_v_source = gem_fn_gem_read_file(NULL, _t2831, 1);
#line 81 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 82 "compiler/main.gem"
    GemVal _t2832[] = {gem_v_src_path};
    GemVal _t2833[] = {gem_add(gem_add(gem_string("cannot read '"), gem_to_string_fn(NULL, _t2832, 1)), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 82, _t2833, 1));
    }
#line 85 "compiler/main.gem"
    GemVal _t2834[] = {gem_v_source};
    gem_v_ast = gem_fn_parse_source(NULL, _t2834, 1);
#line 86 "compiler/main.gem"
    GemVal _t2835[] = {gem_v_src_path};
    gem_v_base_dir = gem_fn_gem_dirname(NULL, _t2835, 1);
#line 87 "compiler/main.gem"
    GemVal _t2836 = gem_table_new();
    gem_v_loaded = _t2836;
#line 88 "compiler/main.gem"
    GemVal _t2837[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t2837, 3);
#line 90 "compiler/main.gem"
    GemVal _t2838[] = {gem_v_src_path};
    gem_v_cg = gem_fn_make_codegen(NULL, _t2838, 1);
#line 91 "compiler/main.gem"
    GemVal _t2839 = gem_v_cg;
    GemVal _t2840[] = {gem_v_resolved_ast};
    GemVal _t2841 = gem_table_get(_t2839, gem_string("compile"));
    gem_v_c_code = _t2841.fn(_t2841.env, _t2840, 1);
#line 92 "compiler/main.gem"
    GemVal _t2842[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t2842, 1));
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

