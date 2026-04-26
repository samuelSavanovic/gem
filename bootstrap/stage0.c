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
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t212 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t212 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    }
    GemVal _t213;
    if (!gem_truthy(_t212)) {
        _t213 = _t212;
    } else {
        _t213 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("."));
    }
    GemVal _t214;
    if (!gem_truthy(_t213)) {
        _t214 = _t213;
    } else {
        _t214 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    }
    GemVal _t215;
    if (!gem_truthy(_t214)) {
        _t215 = _t214;
    } else {
        _t215 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("."));
    }
        if (gem_truthy(_t215)) {
#line 436 "compiler/main.gem"
    GemVal _t216 = gem_table_new();
    gem_table_set(_t216, gem_string("type"), gem_string("..."));
    gem_table_set(_t216, gem_string("value"), gem_string("..."));
    gem_table_set(_t216, gem_string("line"), gem_v_line);
    GemVal _t217[] = {gem_v_tokens, _t216};
            (void)(gem_push_fn(NULL, _t217, 2));
#line 437 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
            continue;
        }
#line 440 "compiler/main.gem"
    GemVal _t218;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t218 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t218 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t219;
    if (gem_truthy(_t218)) {
        _t219 = _t218;
    } else {
        _t219 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t220;
    if (gem_truthy(_t219)) {
        _t220 = _t219;
    } else {
        _t220 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t220)) {
#line 441 "compiler/main.gem"
    GemVal _t221 = gem_table_new();
    gem_table_set(_t221, gem_string("type"), gem_v_ch);
    gem_table_set(_t221, gem_string("value"), gem_v_ch);
    gem_table_set(_t221, gem_string("line"), gem_v_line);
    GemVal _t222[] = {gem_v_tokens, _t221};
            (void)(gem_push_fn(NULL, _t222, 2));
#line 442 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 446 "compiler/main.gem"
    GemVal _t223[] = {gem_v_ch};
    GemVal _t224[] = {gem_v_line};
    GemVal _t225[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t223, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t224, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 446, _t225, 1));
    }
#line 449 "compiler/main.gem"
    GemVal _t226 = gem_table_new();
    gem_table_set(_t226, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t226, gem_string("value"), gem_string(""));
    gem_table_set(_t226, gem_string("line"), gem_v_line);
    GemVal _t227[] = {gem_v_tokens, _t226};
    (void)(gem_push_fn(NULL, _t227, 2));
    GemVal _t228 = gem_v_tokens;
    gem_pop_frame();
    return _t228;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 10 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_source", "compiler/main.gem", 10);
#line 11 "compiler/main.gem"
    GemVal _t229[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t229, 1);
#line 12 "compiler/main.gem"
    GemVal _t230[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t230, 1);
#line 13 "compiler/main.gem"
    GemVal _t231 = gem_v_p;
    GemVal _t232 = gem_table_get(_t231, gem_string("parse"));
    GemVal _t233 = _t232.fn(_t232.env, NULL, 0);
    gem_pop_frame();
    return _t233;
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
    GemVal _t234 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t234;
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
    GemVal _t236 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t236;
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
    GemVal _t238 = gem_v_t;
    gem_pop_frame();
    return _t238;
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
    GemVal _t240 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t241 = gem_eq(gem_table_get(_t240, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t241;
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
    GemVal _t243 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t243, gem_string("type")), gem_v_tp))) {
#line 46 "compiler/main.gem"
    GemVal _t244[] = {gem_v_tp};
    GemVal _t245 = gem_v_t;
    GemVal _t246[] = {gem_table_get(_t245, gem_string("type"))};
    GemVal _t247 = gem_v_t;
    GemVal _t248[] = {gem_table_get(_t247, gem_string("line"))};
    GemVal _t249[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t244, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t246, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t248, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 46, _t249, 1));
    }
#line 48 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t250 = gem_v_t;
    gem_pop_frame();
    return _t250;
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
        GemVal _t252 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t252, gem_string("type")), gem_string("NEWLINE")))) break;
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
    GemVal _t254 = gem_table_new();
    GemVal gem_v_params = _t254;
#line 66 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 67 "compiler/main.gem"
    GemVal gem_v_block_param = GEM_NIL;
#line 69 "compiler/main.gem"
    while (1) {
        GemVal _t255 = (*gem_v_peek);
        GemVal _t256 = _t255.fn(_t255.env, NULL, 0);
        GemVal _t259;
        if (gem_truthy(gem_eq(gem_table_get(_t256, gem_string("type")), gem_string("NAME")))) {
                _t259 = gem_eq(gem_table_get(_t256, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t257 = (*gem_v_peek);
                GemVal _t258 = _t257.fn(_t257.env, NULL, 0);
                _t259 = gem_eq(gem_table_get(_t258, gem_string("type")), gem_string("..."));
        }
        if (!gem_truthy(_t259)) break;
#line 70 "compiler/main.gem"
    GemVal _t260 = (*gem_v_peek);
    GemVal _t261 = _t260.fn(_t260.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t261, gem_string("type")), gem_string("...")))) {
#line 71 "compiler/main.gem"
    GemVal _t262 = (*gem_v_advance);
            (void)(_t262.fn(_t262.env, NULL, 0));
#line 72 "compiler/main.gem"
    GemVal _t263 = (*gem_v_expect);
    GemVal _t264[] = {gem_string("NAME")};
    GemVal _t265 = _t263.fn(_t263.env, _t264, 1);
            gem_v_rest_param = gem_table_get(_t265, gem_string("value"));
#line 73 "compiler/main.gem"
    GemVal _t266 = (*gem_v_peek);
    GemVal _t267 = _t266.fn(_t266.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t267, gem_string("type")), gem_string(",")))) {
#line 74 "compiler/main.gem"
    GemVal _t268 = (*gem_v_advance);
                (void)(_t268.fn(_t268.env, NULL, 0));
#line 75 "compiler/main.gem"
    GemVal _t269 = (*gem_v_peek);
    GemVal _t270 = _t269.fn(_t269.env, NULL, 0);
                if (gem_truthy(gem_neq(gem_table_get(_t270, gem_string("type")), gem_string("NAME")))) {
#line 76 "compiler/main.gem"
    GemVal _t271 = (*gem_v_peek);
    GemVal _t272 = _t271.fn(_t271.env, NULL, 0);
    GemVal _t273[] = {gem_table_get(_t272, gem_string("line"))};
    GemVal _t274[] = {gem_add(gem_string("expected parameter name after rest param at line "), gem_to_string_fn(NULL, _t273, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 76, _t274, 1));
                }
#line 78 "compiler/main.gem"
    GemVal _t275 = (*gem_v_advance);
    GemVal _t276 = _t275.fn(_t275.env, NULL, 0);
                gem_v_block_param = gem_table_get(_t276, gem_string("value"));
#line 79 "compiler/main.gem"
    GemVal _t277 = (*gem_v_peek);
    GemVal _t278 = _t277.fn(_t277.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t278, gem_string("type")), gem_string(",")))) {
#line 80 "compiler/main.gem"
    GemVal _t279 = (*gem_v_peek);
    GemVal _t280 = _t279.fn(_t279.env, NULL, 0);
    GemVal _t281[] = {gem_table_get(_t280, gem_string("line"))};
    GemVal _t282[] = {gem_add(gem_string("only one parameter is allowed after a rest param at line "), gem_to_string_fn(NULL, _t281, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 80, _t282, 1));
                }
            }
            break;
        }
#line 85 "compiler/main.gem"
    GemVal _t283 = (*gem_v_advance);
    GemVal _t284 = _t283.fn(_t283.env, NULL, 0);
    GemVal _t285[] = {gem_v_params, gem_table_get(_t284, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t285, 2));
#line 86 "compiler/main.gem"
    GemVal _t286 = (*gem_v_peek);
    GemVal _t287 = _t286.fn(_t286.env, NULL, 0);
        if (gem_truthy(gem_neq(gem_table_get(_t287, gem_string("type")), gem_string(",")))) {
            break;
        }
#line 89 "compiler/main.gem"
    GemVal _t288 = (*gem_v_advance);
        (void)(_t288.fn(_t288.env, NULL, 0));
    }
    GemVal _t289 = gem_table_new();
    gem_table_set(_t289, gem_string("params"), gem_v_params);
    gem_table_set(_t289, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t289, gem_string("block_param"), gem_v_block_param);
    GemVal _t290 = _t289;
    gem_pop_frame();
    return _t290;
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
    GemVal _t292 = gem_table_new();
    GemVal gem_v_stmts = _t292;
#line 108 "compiler/main.gem"
    GemVal _t293 = (*gem_v_skip_nl);
    (void)(_t293.fn(_t293.env, NULL, 0));
#line 109 "compiler/main.gem"
    while (1) {
        GemVal _t294 = (*gem_v_peek);
        GemVal _t295 = _t294.fn(_t294.env, NULL, 0);
        GemVal _t298;
        if (!gem_truthy(gem_neq(gem_table_get(_t295, gem_string("type")), gem_string("end")))) {
                _t298 = gem_neq(gem_table_get(_t295, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t296 = (*gem_v_peek);
                GemVal _t297 = _t296.fn(_t296.env, NULL, 0);
                _t298 = gem_neq(gem_table_get(_t297, gem_string("type")), gem_string("elif"));
        }
        GemVal _t301;
        if (!gem_truthy(_t298)) {
                _t301 = _t298;
        } else {
                GemVal _t299 = (*gem_v_peek);
                GemVal _t300 = _t299.fn(_t299.env, NULL, 0);
                _t301 = gem_neq(gem_table_get(_t300, gem_string("type")), gem_string("else"));
        }
        GemVal _t304;
        if (!gem_truthy(_t301)) {
                _t304 = _t301;
        } else {
                GemVal _t302 = (*gem_v_peek);
                GemVal _t303 = _t302.fn(_t302.env, NULL, 0);
                _t304 = gem_neq(gem_table_get(_t303, gem_string("type")), gem_string("when"));
        }
        GemVal _t306;
        if (!gem_truthy(_t304)) {
                _t306 = _t304;
        } else {
                GemVal _t305 = (*gem_v_at_end);
                _t306 = gem_not(_t305.fn(_t305.env, NULL, 0));
        }
        if (!gem_truthy(_t306)) break;
#line 110 "compiler/main.gem"
    GemVal _t307 = (*gem_v_parse_stmt);
    GemVal _t308[] = {gem_v_stmts, _t307.fn(_t307.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t308, 2));
#line 111 "compiler/main.gem"
    GemVal _t309 = (*gem_v_skip_nl);
        (void)(_t309.fn(_t309.env, NULL, 0));
    }
    GemVal _t310 = gem_v_stmts;
    gem_pop_frame();
    return _t310;
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
    GemVal _t312 = gem_table_new();
    GemVal gem_v_stmts = _t312;
#line 121 "compiler/main.gem"
    GemVal _t313 = (*gem_v_skip_nl);
    (void)(_t313.fn(_t313.env, NULL, 0));
#line 122 "compiler/main.gem"
    while (1) {
        GemVal _t314 = (*gem_v_peek);
        GemVal _t315 = _t314.fn(_t314.env, NULL, 0);
        GemVal _t317;
        if (!gem_truthy(gem_neq(gem_table_get(_t315, gem_string("type")), gem_string("end")))) {
                _t317 = gem_neq(gem_table_get(_t315, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t316 = (*gem_v_at_end);
                _t317 = gem_not(_t316.fn(_t316.env, NULL, 0));
        }
        if (!gem_truthy(_t317)) break;
#line 123 "compiler/main.gem"
    GemVal _t318 = (*gem_v_parse_stmt);
    GemVal _t319[] = {gem_v_stmts, _t318.fn(_t318.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t319, 2));
#line 124 "compiler/main.gem"
    GemVal _t320 = (*gem_v_skip_nl);
        (void)(_t320.fn(_t320.env, NULL, 0));
    }
    GemVal _t321 = gem_v_stmts;
    gem_pop_frame();
    return _t321;
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
    GemVal _t323 = (*gem_v_peek);
    GemVal gem_v_t = _t323.fn(_t323.env, NULL, 0);
#line 136 "compiler/main.gem"
    GemVal _t324 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t324, gem_string("type")), gem_string("NUMBER")))) {
#line 137 "compiler/main.gem"
    GemVal _t325 = (*gem_v_advance);
        (void)(_t325.fn(_t325.env, NULL, 0));
#line 138 "compiler/main.gem"
    GemVal _t326 = gem_v_t;
    GemVal _t327[] = {gem_table_get(_t326, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t327, 1))) {
#line 139 "compiler/main.gem"
    GemVal _t328 = gem_v_t;
    GemVal _t329[] = {gem_table_get(_t328, gem_string("value"))};
    GemVal _t330[] = {gem_fn_atof(NULL, _t329, 1)};
            GemVal _t331 = gem_fn_make_float(NULL, _t330, 1);
            gem_pop_frame();
            return _t331;
        }
#line 141 "compiler/main.gem"
    GemVal _t332 = gem_v_t;
    GemVal _t333[] = {gem_table_get(_t332, gem_string("value"))};
    GemVal _t334[] = {gem_fn_str_to_int(NULL, _t333, 1)};
        GemVal _t335 = gem_fn_make_int(NULL, _t334, 1);
        gem_pop_frame();
        return _t335;
    }
#line 145 "compiler/main.gem"
    GemVal _t336 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t336, gem_string("type")), gem_string("STRING")))) {
#line 146 "compiler/main.gem"
    GemVal _t337 = (*gem_v_advance);
        (void)(_t337.fn(_t337.env, NULL, 0));
#line 147 "compiler/main.gem"
    GemVal _t338 = gem_v_t;
    GemVal _t339[] = {gem_table_get(_t338, gem_string("value"))};
        GemVal _t340 = gem_fn_make_string(NULL, _t339, 1);
        gem_pop_frame();
        return _t340;
    }
#line 151 "compiler/main.gem"
    GemVal _t341 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t341, gem_string("type")), gem_string("INTERP_START")))) {
#line 152 "compiler/main.gem"
    GemVal _t342 = (*gem_v_advance);
        (void)(_t342.fn(_t342.env, NULL, 0));
#line 153 "compiler/main.gem"
    GemVal _t343 = gem_table_new();
        GemVal gem_v_parts = _t343;
#line 154 "compiler/main.gem"
    GemVal _t344 = (*gem_v_skip_nl);
        (void)(_t344.fn(_t344.env, NULL, 0));
#line 155 "compiler/main.gem"
        while (1) {
            GemVal _t345 = (*gem_v_peek);
            GemVal _t346 = _t345.fn(_t345.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t346, gem_string("type")), gem_string("INTERP_END")))) break;
#line 156 "compiler/main.gem"
    GemVal _t347 = (*gem_v_peek);
    GemVal _t348 = _t347.fn(_t347.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t348, gem_string("type")), gem_string("STRING")))) {
#line 157 "compiler/main.gem"
    GemVal _t349 = (*gem_v_advance);
    GemVal _t350 = _t349.fn(_t349.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t350, gem_string("value"));
#line 158 "compiler/main.gem"
    GemVal _t351[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t351, 1), gem_int(0)))) {
#line 159 "compiler/main.gem"
    GemVal _t352[] = {gem_v_sval};
    GemVal _t353[] = {gem_v_parts, gem_fn_make_string(NULL, _t352, 1)};
                    (void)(gem_push_fn(NULL, _t353, 2));
                }
            } else {
#line 162 "compiler/main.gem"
    GemVal _t354 = (*gem_v_parse_expr);
    GemVal _t355[] = {gem_v_parts, _t354.fn(_t354.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t355, 2));
            }
#line 164 "compiler/main.gem"
    GemVal _t356 = (*gem_v_skip_nl);
            (void)(_t356.fn(_t356.env, NULL, 0));
        }
#line 166 "compiler/main.gem"
    GemVal _t357 = (*gem_v_expect);
    GemVal _t358[] = {gem_string("INTERP_END")};
        (void)(_t357.fn(_t357.env, _t358, 1));
#line 167 "compiler/main.gem"
    GemVal _t359[] = {gem_v_parts};
        GemVal _t360 = gem_fn_make_interp(NULL, _t359, 1);
        gem_pop_frame();
        return _t360;
    }
#line 171 "compiler/main.gem"
    GemVal _t361 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t361, gem_string("type")), gem_string("true")))) {
#line 172 "compiler/main.gem"
    GemVal _t362 = (*gem_v_advance);
        (void)(_t362.fn(_t362.env, NULL, 0));
#line 173 "compiler/main.gem"
    GemVal _t363[] = {gem_bool(1)};
        GemVal _t364 = gem_fn_make_bool(NULL, _t363, 1);
        gem_pop_frame();
        return _t364;
    }
#line 175 "compiler/main.gem"
    GemVal _t365 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t365, gem_string("type")), gem_string("false")))) {
#line 176 "compiler/main.gem"
    GemVal _t366 = (*gem_v_advance);
        (void)(_t366.fn(_t366.env, NULL, 0));
#line 177 "compiler/main.gem"
    GemVal _t367[] = {gem_bool(0)};
        GemVal _t368 = gem_fn_make_bool(NULL, _t367, 1);
        gem_pop_frame();
        return _t368;
    }
#line 181 "compiler/main.gem"
    GemVal _t369 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t369, gem_string("type")), gem_string("nil")))) {
#line 182 "compiler/main.gem"
    GemVal _t370 = (*gem_v_advance);
        (void)(_t370.fn(_t370.env, NULL, 0));
#line 183 "compiler/main.gem"
        GemVal _t371 = gem_fn_make_nil_node(NULL, NULL, 0);
        gem_pop_frame();
        return _t371;
    }
#line 187 "compiler/main.gem"
    GemVal _t372 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t372, gem_string("type")), gem_string("fn")))) {
#line 188 "compiler/main.gem"
    GemVal _t373 = (*gem_v_advance);
        (void)(_t373.fn(_t373.env, NULL, 0));
#line 189 "compiler/main.gem"
    GemVal _t374 = (*gem_v_expect);
    GemVal _t375[] = {gem_string("(")};
        (void)(_t374.fn(_t374.env, _t375, 1));
#line 190 "compiler/main.gem"
    GemVal _t376 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t376.fn(_t376.env, NULL, 0);
#line 191 "compiler/main.gem"
    GemVal _t377 = (*gem_v_expect);
    GemVal _t378[] = {gem_string(")")};
        (void)(_t377.fn(_t377.env, _t378, 1));
#line 192 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 193 "compiler/main.gem"
    GemVal _t379 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t379.fn(_t379.env, NULL, 0);
#line 194 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 195 "compiler/main.gem"
    GemVal _t380 = (*gem_v_expect);
    GemVal _t381[] = {gem_string("end")};
        (void)(_t380.fn(_t380.env, _t381, 1));
#line 196 "compiler/main.gem"
    GemVal _t382 = gem_v_pr;
    GemVal _t383 = gem_v_pr;
    GemVal _t384 = gem_v_pr;
    GemVal _t385[] = {gem_table_get(_t382, gem_string("params")), gem_table_get(_t383, gem_string("rest_param")), gem_table_get(_t384, gem_string("block_param")), gem_v_body};
        GemVal _t386 = gem_fn_make_anon_fn(NULL, _t385, 4);
        gem_pop_frame();
        return _t386;
    }
#line 201 "compiler/main.gem"
    GemVal _t387 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t387, gem_string("type")), gem_string("{")))) {
#line 202 "compiler/main.gem"
    GemVal _t388 = (*gem_v_advance);
        (void)(_t388.fn(_t388.env, NULL, 0));
#line 203 "compiler/main.gem"
    GemVal _t389 = (*gem_v_skip_nl);
        (void)(_t389.fn(_t389.env, NULL, 0));
#line 204 "compiler/main.gem"
    GemVal _t390 = gem_table_new();
        GemVal gem_v_entries = _t390;
#line 205 "compiler/main.gem"
        while (1) {
            GemVal _t391 = (*gem_v_peek);
            GemVal _t392 = _t391.fn(_t391.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t392, gem_string("type")), gem_string("}")))) break;
#line 206 "compiler/main.gem"
    GemVal _t393 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t393.fn(_t393.env, NULL, 0);
#line 207 "compiler/main.gem"
    GemVal _t394 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t394, gem_string("type")), gem_string("NAME")))) {
#line 208 "compiler/main.gem"
    GemVal _t395 = (*gem_v_advance);
                (void)(_t395.fn(_t395.env, NULL, 0));
            } else {
#line 209 "compiler/main.gem"
    GemVal _t396 = (*gem_v_peek_at);
    GemVal _t397[] = {gem_int(1)};
    GemVal _t398 = _t396.fn(_t396.env, _t397, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t398, gem_string("type")), gem_string(":")))) {
#line 211 "compiler/main.gem"
    GemVal _t399 = (*gem_v_advance);
                    (void)(_t399.fn(_t399.env, NULL, 0));
                } else {
#line 213 "compiler/main.gem"
    GemVal _t400 = gem_v_key_tok;
    GemVal _t401[] = {gem_table_get(_t400, gem_string("line"))};
    GemVal _t402[] = {gem_add(gem_string("expected table key at line "), gem_to_string_fn(NULL, _t401, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 213, _t402, 1));
                }
            }
#line 215 "compiler/main.gem"
    GemVal _t403 = (*gem_v_expect);
    GemVal _t404[] = {gem_string(":")};
            (void)(_t403.fn(_t403.env, _t404, 1));
#line 216 "compiler/main.gem"
    GemVal _t405 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t405.fn(_t405.env, NULL, 0);
#line 217 "compiler/main.gem"
    GemVal _t406 = gem_v_key_tok;
    GemVal _t407[] = {gem_table_get(_t406, gem_string("value")), gem_v_val};
    GemVal _t408[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t407, 2)};
            (void)(gem_push_fn(NULL, _t408, 2));
#line 218 "compiler/main.gem"
    GemVal _t409 = (*gem_v_skip_nl);
            (void)(_t409.fn(_t409.env, NULL, 0));
#line 219 "compiler/main.gem"
    GemVal _t410 = (*gem_v_peek);
    GemVal _t411 = _t410.fn(_t410.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t411, gem_string("type")), gem_string(",")))) {
#line 220 "compiler/main.gem"
    GemVal _t412 = (*gem_v_advance);
                (void)(_t412.fn(_t412.env, NULL, 0));
#line 221 "compiler/main.gem"
    GemVal _t413 = (*gem_v_skip_nl);
                (void)(_t413.fn(_t413.env, NULL, 0));
            }
        }
#line 224 "compiler/main.gem"
    GemVal _t414 = (*gem_v_expect);
    GemVal _t415[] = {gem_string("}")};
        (void)(_t414.fn(_t414.env, _t415, 1));
#line 225 "compiler/main.gem"
    GemVal _t416[] = {gem_v_entries};
        GemVal _t417 = gem_fn_make_table(NULL, _t416, 1);
        gem_pop_frame();
        return _t417;
    }
#line 229 "compiler/main.gem"
    GemVal _t418 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t418, gem_string("type")), gem_string("[")))) {
#line 230 "compiler/main.gem"
    GemVal _t419 = (*gem_v_advance);
        (void)(_t419.fn(_t419.env, NULL, 0));
#line 231 "compiler/main.gem"
    GemVal _t420 = (*gem_v_skip_nl);
        (void)(_t420.fn(_t420.env, NULL, 0));
#line 232 "compiler/main.gem"
    GemVal _t421 = gem_table_new();
        GemVal gem_v_elements = _t421;
#line 233 "compiler/main.gem"
        while (1) {
            GemVal _t422 = (*gem_v_peek);
            GemVal _t423 = _t422.fn(_t422.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t423, gem_string("type")), gem_string("]")))) break;
#line 234 "compiler/main.gem"
    GemVal _t424 = (*gem_v_parse_expr);
    GemVal _t425[] = {gem_v_elements, _t424.fn(_t424.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t425, 2));
#line 235 "compiler/main.gem"
    GemVal _t426 = (*gem_v_skip_nl);
            (void)(_t426.fn(_t426.env, NULL, 0));
#line 236 "compiler/main.gem"
    GemVal _t427 = (*gem_v_peek);
    GemVal _t428 = _t427.fn(_t427.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t428, gem_string("type")), gem_string(",")))) {
#line 237 "compiler/main.gem"
    GemVal _t429 = (*gem_v_advance);
                (void)(_t429.fn(_t429.env, NULL, 0));
#line 238 "compiler/main.gem"
    GemVal _t430 = (*gem_v_skip_nl);
                (void)(_t430.fn(_t430.env, NULL, 0));
            }
        }
#line 241 "compiler/main.gem"
    GemVal _t431 = (*gem_v_expect);
    GemVal _t432[] = {gem_string("]")};
        (void)(_t431.fn(_t431.env, _t432, 1));
#line 242 "compiler/main.gem"
    GemVal _t433[] = {gem_v_elements};
        GemVal _t434 = gem_fn_make_array(NULL, _t433, 1);
        gem_pop_frame();
        return _t434;
    }
#line 246 "compiler/main.gem"
    GemVal _t435 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t435, gem_string("type")), gem_string("(")))) {
#line 247 "compiler/main.gem"
    GemVal _t436 = (*gem_v_advance);
        (void)(_t436.fn(_t436.env, NULL, 0));
#line 248 "compiler/main.gem"
    GemVal _t437 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t437.fn(_t437.env, NULL, 0);
#line 249 "compiler/main.gem"
    GemVal _t438 = (*gem_v_expect);
    GemVal _t439[] = {gem_string(")")};
        (void)(_t438.fn(_t438.env, _t439, 1));
#line 250 "compiler/main.gem"
        GemVal _t440 = gem_v_e;
        gem_pop_frame();
        return _t440;
    }
#line 254 "compiler/main.gem"
    GemVal _t441 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t441, gem_string("type")), gem_string("NAME")))) {
#line 255 "compiler/main.gem"
    GemVal _t442 = (*gem_v_advance);
        (void)(_t442.fn(_t442.env, NULL, 0));
#line 256 "compiler/main.gem"
    GemVal _t443 = gem_v_t;
    GemVal _t444[] = {gem_table_get(_t443, gem_string("value"))};
        GemVal _t445 = gem_fn_make_var(NULL, _t444, 1);
        gem_pop_frame();
        return _t445;
    }
#line 259 "compiler/main.gem"
    GemVal _t446 = gem_v_t;
    GemVal _t447[] = {gem_table_get(_t446, gem_string("type"))};
    GemVal _t448 = gem_v_t;
    GemVal _t449[] = {gem_table_get(_t448, gem_string("line"))};
    GemVal _t450[] = {gem_add(gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t447, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t449, 1))};
    GemVal _t451 = gem_error_at_fn("compiler/main.gem", 259, _t450, 1);
    gem_pop_frame();
    return _t451;
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
    GemVal _t453 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t453.fn(_t453.env, NULL, 0);
#line 266 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
#line 268 "compiler/main.gem"
    GemVal _t454 = (*gem_v_peek);
    GemVal _t455 = _t454.fn(_t454.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t455, gem_string("type")), gem_string("(")))) {
#line 269 "compiler/main.gem"
    GemVal _t456 = (*gem_v_peek);
    GemVal _t457 = _t456.fn(_t456.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t457, gem_string("line"));
#line 270 "compiler/main.gem"
    GemVal _t458 = (*gem_v_advance);
            (void)(_t458.fn(_t458.env, NULL, 0));
#line 271 "compiler/main.gem"
    GemVal _t459 = (*gem_v_skip_nl);
            (void)(_t459.fn(_t459.env, NULL, 0));
#line 272 "compiler/main.gem"
    GemVal _t460 = gem_table_new();
            GemVal gem_v_args = _t460;
#line 273 "compiler/main.gem"
    GemVal _t461 = (*gem_v_peek);
    GemVal _t462 = _t461.fn(_t461.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t462, gem_string("type")), gem_string(")")))) {
#line 274 "compiler/main.gem"
    GemVal _t463 = (*gem_v_parse_expr);
    GemVal _t464[] = {gem_v_args, _t463.fn(_t463.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t464, 2));
#line 275 "compiler/main.gem"
                while (1) {
                    GemVal _t465 = (*gem_v_peek);
                    GemVal _t466 = _t465.fn(_t465.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t466, gem_string("type")), gem_string(",")))) break;
#line 276 "compiler/main.gem"
    GemVal _t467 = (*gem_v_advance);
                    (void)(_t467.fn(_t467.env, NULL, 0));
#line 277 "compiler/main.gem"
    GemVal _t468 = (*gem_v_skip_nl);
                    (void)(_t468.fn(_t468.env, NULL, 0));
#line 278 "compiler/main.gem"
    GemVal _t469 = (*gem_v_parse_expr);
    GemVal _t470[] = {gem_v_args, _t469.fn(_t469.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t470, 2));
                }
            }
#line 281 "compiler/main.gem"
    GemVal _t471 = (*gem_v_skip_nl);
            (void)(_t471.fn(_t471.env, NULL, 0));
#line 282 "compiler/main.gem"
    GemVal _t472 = (*gem_v_expect);
    GemVal _t473[] = {gem_string(")")};
            (void)(_t472.fn(_t472.env, _t473, 1));
#line 285 "compiler/main.gem"
    GemVal _t474 = (*gem_v_peek);
    GemVal _t475 = _t474.fn(_t474.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t475, gem_string("type")), gem_string("do")))) {
#line 286 "compiler/main.gem"
    GemVal _t476 = (*gem_v_advance);
                (void)(_t476.fn(_t476.env, NULL, 0));
#line 287 "compiler/main.gem"
    GemVal _t477 = gem_table_new();
                GemVal gem_v_bparams = _t477;
#line 288 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 289 "compiler/main.gem"
                GemVal gem_v_bblock = GEM_NIL;
#line 290 "compiler/main.gem"
    GemVal _t478 = (*gem_v_peek);
    GemVal _t479 = _t478.fn(_t478.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t479, gem_string("type")), gem_string("|")))) {
#line 291 "compiler/main.gem"
    GemVal _t480 = (*gem_v_advance);
                    (void)(_t480.fn(_t480.env, NULL, 0));
#line 292 "compiler/main.gem"
    GemVal _t481 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t481.fn(_t481.env, NULL, 0);
#line 293 "compiler/main.gem"
    GemVal _t482 = gem_v_bpr;
                    gem_v_bparams = gem_table_get(_t482, gem_string("params"));
#line 294 "compiler/main.gem"
    GemVal _t483 = gem_v_bpr;
                    gem_v_brest = gem_table_get(_t483, gem_string("rest_param"));
#line 295 "compiler/main.gem"
    GemVal _t484 = gem_v_bpr;
                    gem_v_bblock = gem_table_get(_t484, gem_string("block_param"));
#line 296 "compiler/main.gem"
    GemVal _t485 = (*gem_v_expect);
    GemVal _t486[] = {gem_string("|")};
                    (void)(_t485.fn(_t485.env, _t486, 1));
                }
#line 298 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 299 "compiler/main.gem"
    GemVal _t487 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t487.fn(_t487.env, NULL, 0);
#line 300 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 301 "compiler/main.gem"
    GemVal _t488 = (*gem_v_expect);
    GemVal _t489[] = {gem_string("end")};
                (void)(_t488.fn(_t488.env, _t489, 1));
#line 302 "compiler/main.gem"
    GemVal _t490[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, gem_v_body};
    GemVal _t491[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t490, 4)};
                (void)(gem_push_fn(NULL, _t491, 2));
            } else {
#line 306 "compiler/main.gem"
    GemVal _t492 = (*gem_v_peek);
    GemVal _t493 = _t492.fn(_t492.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t493, gem_string("type")), gem_string("{")))) {
#line 307 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 308 "compiler/main.gem"
    GemVal _t494 = (*gem_v_peek_at);
    GemVal _t495[] = {gem_int(1)};
    GemVal _t496 = _t494.fn(_t494.env, _t495, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t496, gem_string("type")), gem_string("|")))) {
#line 309 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 310 "compiler/main.gem"
    GemVal _t497 = (*gem_v_peek_at);
    GemVal _t498[] = {gem_int(1)};
    GemVal _t499 = _t497.fn(_t497.env, _t498, 1);
    GemVal _t507;
    if (gem_truthy(gem_eq(gem_table_get(_t499, gem_string("type")), gem_string("}")))) {
        _t507 = gem_eq(gem_table_get(_t499, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t500 = (*gem_v_peek_at);
        GemVal _t501[] = {gem_int(1)};
        GemVal _t502 = _t500.fn(_t500.env, _t501, 1);
        GemVal _t506;
        if (!gem_truthy(gem_eq(gem_table_get(_t502, gem_string("type")), gem_string("NAME")))) {
                _t506 = gem_eq(gem_table_get(_t502, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t503 = (*gem_v_peek_at);
                GemVal _t504[] = {gem_int(2)};
                GemVal _t505 = _t503.fn(_t503.env, _t504, 1);
                _t506 = gem_eq(gem_table_get(_t505, gem_string("type")), gem_string(":"));
        }
        _t507 = _t506;
    }
                        if (gem_truthy(_t507)) {
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
    GemVal _t508 = (*gem_v_advance);
                        (void)(_t508.fn(_t508.env, NULL, 0));
#line 317 "compiler/main.gem"
    GemVal _t509 = gem_table_new();
                        GemVal gem_v_bparams = _t509;
#line 318 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 319 "compiler/main.gem"
                        GemVal gem_v_bblock = GEM_NIL;
#line 320 "compiler/main.gem"
    GemVal _t510 = (*gem_v_peek);
    GemVal _t511 = _t510.fn(_t510.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t511, gem_string("type")), gem_string("|")))) {
#line 321 "compiler/main.gem"
    GemVal _t512 = (*gem_v_advance);
                            (void)(_t512.fn(_t512.env, NULL, 0));
#line 322 "compiler/main.gem"
    GemVal _t513 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t513.fn(_t513.env, NULL, 0);
#line 323 "compiler/main.gem"
    GemVal _t514 = gem_v_bpr;
                            gem_v_bparams = gem_table_get(_t514, gem_string("params"));
#line 324 "compiler/main.gem"
    GemVal _t515 = gem_v_bpr;
                            gem_v_brest = gem_table_get(_t515, gem_string("rest_param"));
#line 325 "compiler/main.gem"
    GemVal _t516 = gem_v_bpr;
                            gem_v_bblock = gem_table_get(_t516, gem_string("block_param"));
#line 326 "compiler/main.gem"
    GemVal _t517 = (*gem_v_expect);
    GemVal _t518[] = {gem_string("|")};
                            (void)(_t517.fn(_t517.env, _t518, 1));
                        }
#line 328 "compiler/main.gem"
    GemVal _t519 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t519.fn(_t519.env, NULL, 0);
#line 329 "compiler/main.gem"
    GemVal _t520 = (*gem_v_expect);
    GemVal _t521[] = {gem_string("}")};
                        (void)(_t520.fn(_t520.env, _t521, 1));
#line 330 "compiler/main.gem"
    GemVal _t522 = gem_table_new();
    gem_table_set(_t522, gem_int(0), gem_v_bexpr);
    GemVal _t523[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, _t522};
    GemVal _t524[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t523, 4)};
                        (void)(gem_push_fn(NULL, _t524, 2));
                    }
                }
            }
#line 335 "compiler/main.gem"
    GemVal _t525[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t525, 3);
            continue;
        }
#line 340 "compiler/main.gem"
    GemVal _t526 = (*gem_v_peek);
    GemVal _t527 = _t526.fn(_t526.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t527, gem_string("type")), gem_string(".")))) {
#line 341 "compiler/main.gem"
    GemVal _t528 = (*gem_v_advance);
            (void)(_t528.fn(_t528.env, NULL, 0));
#line 342 "compiler/main.gem"
    GemVal _t529 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t529.fn(_t529.env, NULL, 0);
#line 343 "compiler/main.gem"
    GemVal _t530 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t530, gem_string("type")), gem_string("NAME")))) {
#line 344 "compiler/main.gem"
    GemVal _t531 = (*gem_v_advance);
                (void)(_t531.fn(_t531.env, NULL, 0));
            } else {
#line 347 "compiler/main.gem"
    GemVal _t532 = (*gem_v_advance);
                (void)(_t532.fn(_t532.env, NULL, 0));
            }
#line 349 "compiler/main.gem"
    GemVal _t533 = gem_v_field_tok;
    GemVal _t534[] = {gem_v_node, gem_table_get(_t533, gem_string("value"))};
            gem_v_node = gem_fn_make_dot(NULL, _t534, 2);
            continue;
        }
#line 354 "compiler/main.gem"
    GemVal _t535 = (*gem_v_peek);
    GemVal _t536 = _t535.fn(_t535.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t536, gem_string("type")), gem_string("[")))) {
#line 355 "compiler/main.gem"
    GemVal _t537 = (*gem_v_advance);
            (void)(_t537.fn(_t537.env, NULL, 0));
#line 356 "compiler/main.gem"
    GemVal _t538 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t538.fn(_t538.env, NULL, 0);
#line 357 "compiler/main.gem"
    GemVal _t539 = (*gem_v_expect);
    GemVal _t540[] = {gem_string("]")};
            (void)(_t539.fn(_t539.env, _t540, 1));
#line 358 "compiler/main.gem"
    GemVal _t541[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t541, 2);
            continue;
        }
        break;
    }
    GemVal _t542 = gem_v_node;
    gem_pop_frame();
    return _t542;
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
    GemVal _t544 = (*gem_v_peek);
    GemVal _t545 = _t544.fn(_t544.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t545, gem_string("type")), gem_string("-")))) {
#line 371 "compiler/main.gem"
    GemVal _t546 = (*gem_v_advance);
        (void)(_t546.fn(_t546.env, NULL, 0));
#line 372 "compiler/main.gem"
    GemVal _t547 = (*gem_v_parse_unary);
    GemVal _t548[] = {gem_string("-"), _t547.fn(_t547.env, NULL, 0)};
        GemVal _t549 = gem_fn_make_unop(NULL, _t548, 2);
        gem_pop_frame();
        return _t549;
    }
#line 374 "compiler/main.gem"
    GemVal _t550 = (*gem_v_parse_call);
    GemVal _t551 = _t550.fn(_t550.env, NULL, 0);
    gem_pop_frame();
    return _t551;
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
    GemVal _t553 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t553.fn(_t553.env, NULL, 0);
#line 380 "compiler/main.gem"
    while (1) {
        GemVal _t554 = (*gem_v_peek);
        GemVal _t555 = _t554.fn(_t554.env, NULL, 0);
        GemVal _t558;
        if (gem_truthy(gem_eq(gem_table_get(_t555, gem_string("type")), gem_string("*")))) {
                _t558 = gem_eq(gem_table_get(_t555, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t556 = (*gem_v_peek);
                GemVal _t557 = _t556.fn(_t556.env, NULL, 0);
                _t558 = gem_eq(gem_table_get(_t557, gem_string("type")), gem_string("/"));
        }
        GemVal _t561;
        if (gem_truthy(_t558)) {
                _t561 = _t558;
        } else {
                GemVal _t559 = (*gem_v_peek);
                GemVal _t560 = _t559.fn(_t559.env, NULL, 0);
                _t561 = gem_eq(gem_table_get(_t560, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t561)) break;
#line 381 "compiler/main.gem"
    GemVal _t562 = (*gem_v_advance);
    GemVal _t563 = _t562.fn(_t562.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t563, gem_string("type"));
#line 382 "compiler/main.gem"
    GemVal _t564 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t564.fn(_t564.env, NULL, 0);
#line 383 "compiler/main.gem"
    GemVal _t565[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t565, 3);
    }
    GemVal _t566 = gem_v_left;
    gem_pop_frame();
    return _t566;
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
    GemVal _t568 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t568.fn(_t568.env, NULL, 0);
#line 391 "compiler/main.gem"
    while (1) {
        GemVal _t569 = (*gem_v_peek);
        GemVal _t570 = _t569.fn(_t569.env, NULL, 0);
        GemVal _t573;
        if (gem_truthy(gem_eq(gem_table_get(_t570, gem_string("type")), gem_string("+")))) {
                _t573 = gem_eq(gem_table_get(_t570, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t571 = (*gem_v_peek);
                GemVal _t572 = _t571.fn(_t571.env, NULL, 0);
                _t573 = gem_eq(gem_table_get(_t572, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t573)) break;
#line 392 "compiler/main.gem"
    GemVal _t574 = (*gem_v_advance);
    GemVal _t575 = _t574.fn(_t574.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t575, gem_string("type"));
#line 393 "compiler/main.gem"
    GemVal _t576 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t576.fn(_t576.env, NULL, 0);
#line 394 "compiler/main.gem"
    GemVal _t577[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t577, 3);
    }
    GemVal _t578 = gem_v_left;
    gem_pop_frame();
    return _t578;
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
    GemVal _t580 = (*gem_v_parse_add);
    GemVal gem_v_left = _t580.fn(_t580.env, NULL, 0);
#line 402 "compiler/main.gem"
    while (1) {
        GemVal _t581 = (*gem_v_peek);
        GemVal _t582 = _t581.fn(_t581.env, NULL, 0);
        GemVal _t585;
        if (gem_truthy(gem_eq(gem_table_get(_t582, gem_string("type")), gem_string("==")))) {
                _t585 = gem_eq(gem_table_get(_t582, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t583 = (*gem_v_peek);
                GemVal _t584 = _t583.fn(_t583.env, NULL, 0);
                _t585 = gem_eq(gem_table_get(_t584, gem_string("type")), gem_string("!="));
        }
        GemVal _t588;
        if (gem_truthy(_t585)) {
                _t588 = _t585;
        } else {
                GemVal _t586 = (*gem_v_peek);
                GemVal _t587 = _t586.fn(_t586.env, NULL, 0);
                _t588 = gem_eq(gem_table_get(_t587, gem_string("type")), gem_string("<"));
        }
        GemVal _t591;
        if (gem_truthy(_t588)) {
                _t591 = _t588;
        } else {
                GemVal _t589 = (*gem_v_peek);
                GemVal _t590 = _t589.fn(_t589.env, NULL, 0);
                _t591 = gem_eq(gem_table_get(_t590, gem_string("type")), gem_string(">"));
        }
        GemVal _t594;
        if (gem_truthy(_t591)) {
                _t594 = _t591;
        } else {
                GemVal _t592 = (*gem_v_peek);
                GemVal _t593 = _t592.fn(_t592.env, NULL, 0);
                _t594 = gem_eq(gem_table_get(_t593, gem_string("type")), gem_string("<="));
        }
        GemVal _t597;
        if (gem_truthy(_t594)) {
                _t597 = _t594;
        } else {
                GemVal _t595 = (*gem_v_peek);
                GemVal _t596 = _t595.fn(_t595.env, NULL, 0);
                _t597 = gem_eq(gem_table_get(_t596, gem_string("type")), gem_string(">="));
        }
        GemVal _t600;
        if (gem_truthy(_t597)) {
                _t600 = _t597;
        } else {
                GemVal _t598 = (*gem_v_peek);
                GemVal _t599 = _t598.fn(_t598.env, NULL, 0);
                _t600 = gem_eq(gem_table_get(_t599, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t600)) break;
#line 403 "compiler/main.gem"
    GemVal _t601 = (*gem_v_advance);
    GemVal _t602 = _t601.fn(_t601.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t602, gem_string("type"));
#line 404 "compiler/main.gem"
    GemVal _t603 = (*gem_v_parse_add);
        GemVal gem_v_right = _t603.fn(_t603.env, NULL, 0);
#line 405 "compiler/main.gem"
    GemVal _t604[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t604, 3);
    }
    GemVal _t605 = gem_v_left;
    gem_pop_frame();
    return _t605;
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
    GemVal _t607 = (*gem_v_peek);
    GemVal _t608 = _t607.fn(_t607.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t608, gem_string("type")), gem_string("not")))) {
#line 413 "compiler/main.gem"
    GemVal _t609 = (*gem_v_advance);
        (void)(_t609.fn(_t609.env, NULL, 0));
#line 414 "compiler/main.gem"
    GemVal _t610 = (*gem_v_parse_not);
    GemVal _t611[] = {gem_string("not"), _t610.fn(_t610.env, NULL, 0)};
        GemVal _t612 = gem_fn_make_unop(NULL, _t611, 2);
        gem_pop_frame();
        return _t612;
    }
#line 416 "compiler/main.gem"
    GemVal _t613 = (*gem_v_parse_compare);
    GemVal _t614 = _t613.fn(_t613.env, NULL, 0);
    gem_pop_frame();
    return _t614;
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
    GemVal _t616 = (*gem_v_parse_not);
    GemVal gem_v_left = _t616.fn(_t616.env, NULL, 0);
#line 422 "compiler/main.gem"
    while (1) {
        GemVal _t617 = (*gem_v_peek);
        GemVal _t618 = _t617.fn(_t617.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t618, gem_string("type")), gem_string("and")))) break;
#line 423 "compiler/main.gem"
    GemVal _t619 = (*gem_v_advance);
        (void)(_t619.fn(_t619.env, NULL, 0));
#line 424 "compiler/main.gem"
    GemVal _t620 = (*gem_v_parse_not);
        GemVal gem_v_right = _t620.fn(_t620.env, NULL, 0);
#line 425 "compiler/main.gem"
    GemVal _t621[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t621, 3);
    }
    GemVal _t622 = gem_v_left;
    gem_pop_frame();
    return _t622;
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
    GemVal _t624 = (*gem_v_parse_and);
    GemVal gem_v_left = _t624.fn(_t624.env, NULL, 0);
#line 433 "compiler/main.gem"
    while (1) {
        GemVal _t625 = (*gem_v_peek);
        GemVal _t626 = _t625.fn(_t625.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t626, gem_string("type")), gem_string("or")))) break;
#line 434 "compiler/main.gem"
    GemVal _t627 = (*gem_v_advance);
        (void)(_t627.fn(_t627.env, NULL, 0));
#line 435 "compiler/main.gem"
    GemVal _t628 = (*gem_v_parse_and);
        GemVal gem_v_right = _t628.fn(_t628.env, NULL, 0);
#line 436 "compiler/main.gem"
    GemVal _t629[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t629, 3);
    }
    GemVal _t630 = gem_v_left;
    gem_pop_frame();
    return _t630;
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
    GemVal _t632 = (*gem_v_peek);
    GemVal _t633 = _t632.fn(_t632.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t633, gem_string("line"));
#line 444 "compiler/main.gem"
    GemVal _t634 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t634.fn(_t634.env, NULL, 0);
#line 447 "compiler/main.gem"
    GemVal _t635 = (*gem_v_peek);
    GemVal _t636 = _t635.fn(_t635.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t636, gem_string("type")), gem_string("=")))) {
#line 448 "compiler/main.gem"
    GemVal _t637 = (*gem_v_advance);
        (void)(_t637.fn(_t637.env, NULL, 0));
#line 449 "compiler/main.gem"
    GemVal _t638 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t638.fn(_t638.env, NULL, 0);
#line 450 "compiler/main.gem"
    GemVal _t639 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t639, gem_string("tag")), gem_string("var")))) {
#line 451 "compiler/main.gem"
    GemVal _t640 = gem_v_lhs;
    GemVal _t641[] = {gem_table_get(_t640, gem_string("name")), gem_v_value, gem_v_start_line};
            GemVal _t642 = gem_fn_make_assign(NULL, _t641, 3);
            gem_pop_frame();
            return _t642;
        }
#line 453 "compiler/main.gem"
    GemVal _t643 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t643, gem_string("tag")), gem_string("dot")))) {
#line 454 "compiler/main.gem"
    GemVal _t644 = gem_v_lhs;
    GemVal _t645 = gem_v_lhs;
    GemVal _t646[] = {gem_table_get(_t644, gem_string("object")), gem_table_get(_t645, gem_string("field")), gem_v_value, gem_v_start_line};
            GemVal _t647 = gem_fn_make_dot_assign(NULL, _t646, 4);
            gem_pop_frame();
            return _t647;
        }
#line 456 "compiler/main.gem"
    GemVal _t648 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t648, gem_string("tag")), gem_string("index")))) {
#line 457 "compiler/main.gem"
    GemVal _t649 = gem_v_lhs;
    GemVal _t650 = gem_v_lhs;
    GemVal _t651[] = {gem_table_get(_t649, gem_string("object")), gem_table_get(_t650, gem_string("key")), gem_v_value, gem_v_start_line};
            GemVal _t652 = gem_fn_make_index_assign(NULL, _t651, 4);
            gem_pop_frame();
            return _t652;
        }
#line 459 "compiler/main.gem"
    GemVal _t653 = (*gem_v_peek);
    GemVal _t654 = _t653.fn(_t653.env, NULL, 0);
    GemVal _t655[] = {gem_table_get(_t654, gem_string("line"))};
    GemVal _t656[] = {gem_add(gem_string("invalid assignment target at line "), gem_to_string_fn(NULL, _t655, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 459, _t656, 1));
    }
#line 463 "compiler/main.gem"
    GemVal _t657 = (*gem_v_peek);
    GemVal _t658 = _t657.fn(_t657.env, NULL, 0);
    GemVal _t661;
    if (gem_truthy(gem_eq(gem_table_get(_t658, gem_string("type")), gem_string("+=")))) {
        _t661 = gem_eq(gem_table_get(_t658, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t659 = (*gem_v_peek);
        GemVal _t660 = _t659.fn(_t659.env, NULL, 0);
        _t661 = gem_eq(gem_table_get(_t660, gem_string("type")), gem_string("-="));
    }
    GemVal _t664;
    if (gem_truthy(_t661)) {
        _t664 = _t661;
    } else {
        GemVal _t662 = (*gem_v_peek);
        GemVal _t663 = _t662.fn(_t662.env, NULL, 0);
        _t664 = gem_eq(gem_table_get(_t663, gem_string("type")), gem_string("*="));
    }
    GemVal _t667;
    if (gem_truthy(_t664)) {
        _t667 = _t664;
    } else {
        GemVal _t665 = (*gem_v_peek);
        GemVal _t666 = _t665.fn(_t665.env, NULL, 0);
        _t667 = gem_eq(gem_table_get(_t666, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t667)) {
#line 464 "compiler/main.gem"
    GemVal _t668 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t668.fn(_t668.env, NULL, 0);
#line 465 "compiler/main.gem"
    GemVal _t669 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t669, gem_string("value")), gem_int(0));
#line 466 "compiler/main.gem"
    GemVal _t670 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t670.fn(_t670.env, NULL, 0);
#line 467 "compiler/main.gem"
    GemVal _t671 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t671, gem_string("tag")), gem_string("var")))) {
#line 468 "compiler/main.gem"
    GemVal _t672 = gem_v_op_tok;
    GemVal _t673[] = {gem_table_get(_t672, gem_string("line"))};
    GemVal _t674[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t673, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 468, _t674, 1));
        }
#line 470 "compiler/main.gem"
    GemVal _t675 = gem_v_lhs;
    GemVal _t676 = gem_v_lhs;
    GemVal _t677[] = {gem_table_get(_t676, gem_string("name"))};
    GemVal _t678[] = {gem_v_base_op, gem_fn_make_var(NULL, _t677, 1), gem_v_value};
    GemVal _t679[] = {gem_table_get(_t675, gem_string("name")), gem_fn_make_binop(NULL, _t678, 3), gem_v_start_line};
        GemVal _t680 = gem_fn_make_assign(NULL, _t679, 3);
        gem_pop_frame();
        return _t680;
    }
    GemVal _t681 = gem_v_lhs;
    gem_pop_frame();
    return _t681;
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
    GemVal _t683 = (*gem_v_peek);
    GemVal gem_v_t = _t683.fn(_t683.env, NULL, 0);
#line 485 "compiler/main.gem"
    GemVal _t684 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t684, gem_string("type")), gem_string("let")))) {
#line 486 "compiler/main.gem"
    GemVal _t685 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t685, gem_string("line"));
#line 487 "compiler/main.gem"
    GemVal _t686 = (*gem_v_advance);
        (void)(_t686.fn(_t686.env, NULL, 0));
#line 490 "compiler/main.gem"
    GemVal _t687 = (*gem_v_peek);
    GemVal _t688 = _t687.fn(_t687.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t688, gem_string("type")), gem_string("{")))) {
#line 491 "compiler/main.gem"
    GemVal _t689 = (*gem_v_advance);
            (void)(_t689.fn(_t689.env, NULL, 0));
#line 492 "compiler/main.gem"
    GemVal _t690 = gem_table_new();
            GemVal gem_v_names = _t690;
#line 493 "compiler/main.gem"
    GemVal _t691 = (*gem_v_skip_nl);
            (void)(_t691.fn(_t691.env, NULL, 0));
#line 494 "compiler/main.gem"
            while (1) {
                GemVal _t692 = (*gem_v_peek);
                GemVal _t693 = _t692.fn(_t692.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t693, gem_string("type")), gem_string("}")))) break;
#line 495 "compiler/main.gem"
    GemVal _t694 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t694.fn(_t694.env, NULL, 0);
#line 496 "compiler/main.gem"
    GemVal _t695 = gem_v_field_tok;
                if (gem_truthy(gem_eq(gem_table_get(_t695, gem_string("type")), gem_string("NAME")))) {
#line 497 "compiler/main.gem"
    GemVal _t696 = (*gem_v_advance);
                    (void)(_t696.fn(_t696.env, NULL, 0));
                } else {
#line 500 "compiler/main.gem"
    GemVal _t697 = (*gem_v_advance);
                    (void)(_t697.fn(_t697.env, NULL, 0));
                }
#line 502 "compiler/main.gem"
    GemVal _t698 = gem_v_field_tok;
    GemVal _t699[] = {gem_v_names, gem_table_get(_t698, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t699, 2));
#line 503 "compiler/main.gem"
    GemVal _t700 = (*gem_v_skip_nl);
                (void)(_t700.fn(_t700.env, NULL, 0));
#line 504 "compiler/main.gem"
    GemVal _t701 = (*gem_v_peek);
    GemVal _t702 = _t701.fn(_t701.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t702, gem_string("type")), gem_string(",")))) {
#line 505 "compiler/main.gem"
    GemVal _t703 = (*gem_v_advance);
                    (void)(_t703.fn(_t703.env, NULL, 0));
#line 506 "compiler/main.gem"
    GemVal _t704 = (*gem_v_skip_nl);
                    (void)(_t704.fn(_t704.env, NULL, 0));
                }
            }
#line 509 "compiler/main.gem"
    GemVal _t705 = (*gem_v_expect);
    GemVal _t706[] = {gem_string("}")};
            (void)(_t705.fn(_t705.env, _t706, 1));
#line 510 "compiler/main.gem"
    GemVal _t707 = (*gem_v_expect);
    GemVal _t708[] = {gem_string("=")};
            (void)(_t707.fn(_t707.env, _t708, 1));
#line 511 "compiler/main.gem"
    GemVal _t709 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t709.fn(_t709.env, NULL, 0);
#line 512 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 513 "compiler/main.gem"
    GemVal _t710[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t710, 1));
#line 514 "compiler/main.gem"
    GemVal _t711 = gem_table_new();
    GemVal _t712[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t711, gem_int(0), gem_fn_make_let(NULL, _t712, 3));
            GemVal gem_v_stmts = _t711;
#line 515 "compiler/main.gem"
            GemVal gem_v__for_i_1 = gem_int(0);
#line 515 "compiler/main.gem"
    GemVal _t713[] = {gem_v_names};
            GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t713, 1);
#line 515 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 515 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_1;
#line 515 "compiler/main.gem"
                gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 516 "compiler/main.gem"
    GemVal _t714[] = {gem_v_tmp};
    GemVal _t715[] = {gem_fn_make_var(NULL, _t714, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t716[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_dot(NULL, _t715, 2), gem_v_line};
    GemVal _t717[] = {gem_v_stmts, gem_fn_make_let(NULL, _t716, 3)};
                (void)(gem_push_fn(NULL, _t717, 2));
            }

#line 518 "compiler/main.gem"
    GemVal _t718 = gem_table_new();
    gem_table_set(_t718, gem_string("tag"), gem_string("block"));
    gem_table_set(_t718, gem_string("stmts"), gem_v_stmts);
            GemVal _t719 = _t718;
            gem_pop_frame();
            return _t719;
        }
#line 522 "compiler/main.gem"
    GemVal _t720 = (*gem_v_peek);
    GemVal _t721 = _t720.fn(_t720.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t721, gem_string("type")), gem_string("[")))) {
#line 523 "compiler/main.gem"
    GemVal _t722 = (*gem_v_advance);
            (void)(_t722.fn(_t722.env, NULL, 0));
#line 524 "compiler/main.gem"
    GemVal _t723 = gem_table_new();
            GemVal gem_v_names = _t723;
#line 525 "compiler/main.gem"
    GemVal _t724 = (*gem_v_skip_nl);
            (void)(_t724.fn(_t724.env, NULL, 0));
#line 526 "compiler/main.gem"
            while (1) {
                GemVal _t725 = (*gem_v_peek);
                GemVal _t726 = _t725.fn(_t725.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t726, gem_string("type")), gem_string("]")))) break;
#line 527 "compiler/main.gem"
    GemVal _t727 = (*gem_v_expect);
    GemVal _t728[] = {gem_string("NAME")};
    GemVal _t729 = _t727.fn(_t727.env, _t728, 1);
    GemVal _t730[] = {gem_v_names, gem_table_get(_t729, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t730, 2));
#line 528 "compiler/main.gem"
    GemVal _t731 = (*gem_v_skip_nl);
                (void)(_t731.fn(_t731.env, NULL, 0));
#line 529 "compiler/main.gem"
    GemVal _t732 = (*gem_v_peek);
    GemVal _t733 = _t732.fn(_t732.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t733, gem_string("type")), gem_string(",")))) {
#line 530 "compiler/main.gem"
    GemVal _t734 = (*gem_v_advance);
                    (void)(_t734.fn(_t734.env, NULL, 0));
#line 531 "compiler/main.gem"
    GemVal _t735 = (*gem_v_skip_nl);
                    (void)(_t735.fn(_t735.env, NULL, 0));
                }
            }
#line 534 "compiler/main.gem"
    GemVal _t736 = (*gem_v_expect);
    GemVal _t737[] = {gem_string("]")};
            (void)(_t736.fn(_t736.env, _t737, 1));
#line 535 "compiler/main.gem"
    GemVal _t738 = (*gem_v_expect);
    GemVal _t739[] = {gem_string("=")};
            (void)(_t738.fn(_t738.env, _t739, 1));
#line 536 "compiler/main.gem"
    GemVal _t740 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t740.fn(_t740.env, NULL, 0);
#line 537 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 538 "compiler/main.gem"
    GemVal _t741[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t741, 1));
#line 539 "compiler/main.gem"
    GemVal _t742 = gem_table_new();
    GemVal _t743[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t742, gem_int(0), gem_fn_make_let(NULL, _t743, 3));
            GemVal gem_v_stmts = _t742;
#line 540 "compiler/main.gem"
            GemVal gem_v__for_i_2 = gem_int(0);
#line 540 "compiler/main.gem"
    GemVal _t744[] = {gem_v_names};
            GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t744, 1);
#line 540 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 540 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_2;
#line 540 "compiler/main.gem"
                gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 541 "compiler/main.gem"
    GemVal _t745[] = {gem_v_tmp};
    GemVal _t746[] = {gem_v_di};
    GemVal _t747[] = {gem_fn_make_var(NULL, _t745, 1), gem_fn_make_int(NULL, _t746, 1)};
    GemVal _t748[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_index(NULL, _t747, 2), gem_v_line};
    GemVal _t749[] = {gem_v_stmts, gem_fn_make_let(NULL, _t748, 3)};
                (void)(gem_push_fn(NULL, _t749, 2));
            }

#line 543 "compiler/main.gem"
    GemVal _t750 = gem_table_new();
    gem_table_set(_t750, gem_string("tag"), gem_string("block"));
    gem_table_set(_t750, gem_string("stmts"), gem_v_stmts);
            GemVal _t751 = _t750;
            gem_pop_frame();
            return _t751;
        }
#line 546 "compiler/main.gem"
    GemVal _t752 = (*gem_v_expect);
    GemVal _t753[] = {gem_string("NAME")};
    GemVal _t754 = _t752.fn(_t752.env, _t753, 1);
        GemVal gem_v_name = gem_table_get(_t754, gem_string("value"));
#line 547 "compiler/main.gem"
    GemVal _t755 = (*gem_v_expect);
    GemVal _t756[] = {gem_string("=")};
        (void)(_t755.fn(_t755.env, _t756, 1));
#line 548 "compiler/main.gem"
    GemVal _t757 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t757.fn(_t757.env, NULL, 0);
#line 549 "compiler/main.gem"
    GemVal _t758[] = {gem_v_name, gem_v_value, gem_v_line};
        GemVal _t759 = gem_fn_make_let(NULL, _t758, 3);
        gem_pop_frame();
        return _t759;
    }
#line 553 "compiler/main.gem"
    GemVal _t760 = gem_v_t;
    GemVal _t764;
    if (!gem_truthy(gem_eq(gem_table_get(_t760, gem_string("type")), gem_string("fn")))) {
        _t764 = gem_eq(gem_table_get(_t760, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t761 = (*gem_v_peek_at);
        GemVal _t762[] = {gem_int(1)};
        GemVal _t763 = _t761.fn(_t761.env, _t762, 1);
        _t764 = gem_eq(gem_table_get(_t763, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t764)) {
#line 554 "compiler/main.gem"
    GemVal _t765 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t765, gem_string("line"));
#line 555 "compiler/main.gem"
    GemVal _t766 = (*gem_v_advance);
        (void)(_t766.fn(_t766.env, NULL, 0));
#line 556 "compiler/main.gem"
    GemVal _t767 = (*gem_v_expect);
    GemVal _t768[] = {gem_string("NAME")};
    GemVal _t769 = _t767.fn(_t767.env, _t768, 1);
        GemVal gem_v_name = gem_table_get(_t769, gem_string("value"));
#line 557 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 558 "compiler/main.gem"
    GemVal _t770[] = {gem_v_name};
    GemVal _t771[] = {gem_v_line};
    GemVal _t772[] = {gem_v_name};
    GemVal _t773[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("named fn '"), gem_to_string_fn(NULL, _t770, 1)), gem_string("' inside function body is not supported at line ")), gem_to_string_fn(NULL, _t771, 1)), gem_string(" — use: let ")), gem_to_string_fn(NULL, _t772, 1)), gem_string(" = fn(...) ... end"))};
            (void)(gem_error_at_fn("compiler/main.gem", 558, _t773, 1));
        }
#line 560 "compiler/main.gem"
    GemVal _t774 = (*gem_v_expect);
    GemVal _t775[] = {gem_string("(")};
        (void)(_t774.fn(_t774.env, _t775, 1));
#line 561 "compiler/main.gem"
    GemVal _t776 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t776.fn(_t776.env, NULL, 0);
#line 562 "compiler/main.gem"
    GemVal _t777 = (*gem_v_expect);
    GemVal _t778[] = {gem_string(")")};
        (void)(_t777.fn(_t777.env, _t778, 1));
#line 563 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 564 "compiler/main.gem"
    GemVal _t779 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t779.fn(_t779.env, NULL, 0);
#line 565 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 566 "compiler/main.gem"
    GemVal _t780 = (*gem_v_expect);
    GemVal _t781[] = {gem_string("end")};
        (void)(_t780.fn(_t780.env, _t781, 1));
#line 567 "compiler/main.gem"
    GemVal _t782 = gem_v_pr;
    GemVal _t783 = gem_v_pr;
    GemVal _t784 = gem_v_pr;
    GemVal _t785[] = {gem_v_name, gem_table_get(_t782, gem_string("params")), gem_table_get(_t783, gem_string("rest_param")), gem_table_get(_t784, gem_string("block_param")), gem_v_body, gem_v_line};
        GemVal _t786 = gem_fn_make_fn_def(NULL, _t785, 6);
        gem_pop_frame();
        return _t786;
    }
#line 571 "compiler/main.gem"
    GemVal _t787 = gem_v_t;
    GemVal _t789;
    if (gem_truthy(gem_eq(gem_table_get(_t787, gem_string("type")), gem_string("if")))) {
        _t789 = gem_eq(gem_table_get(_t787, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t788 = gem_v_t;
        _t789 = gem_eq(gem_table_get(_t788, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t789)) {
#line 572 "compiler/main.gem"
    GemVal _t790 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t790, gem_string("line"));
#line 573 "compiler/main.gem"
    GemVal _t791 = (*gem_v_advance);
        (void)(_t791.fn(_t791.env, NULL, 0));
#line 574 "compiler/main.gem"
    GemVal _t792 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t792.fn(_t792.env, NULL, 0);
#line 575 "compiler/main.gem"
    GemVal _t793 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t793.fn(_t793.env, NULL, 0);
#line 576 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 577 "compiler/main.gem"
    GemVal _t794 = (*gem_v_peek);
    GemVal _t795 = _t794.fn(_t794.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t795, gem_string("type")), gem_string("elif")))) {
#line 579 "compiler/main.gem"
    GemVal _t796 = gem_table_new();
    GemVal _t797 = (*gem_v_parse_stmt);
    gem_table_set(_t796, gem_int(0), _t797.fn(_t797.env, NULL, 0));
            gem_v_else_body = _t796;
        } else {
#line 580 "compiler/main.gem"
    GemVal _t798 = (*gem_v_peek);
    GemVal _t799 = _t798.fn(_t798.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t799, gem_string("type")), gem_string("else")))) {
#line 581 "compiler/main.gem"
    GemVal _t800 = (*gem_v_advance);
                (void)(_t800.fn(_t800.env, NULL, 0));
#line 582 "compiler/main.gem"
    GemVal _t801 = (*gem_v_parse_body);
                gem_v_else_body = _t801.fn(_t801.env, NULL, 0);
            }
        }
#line 585 "compiler/main.gem"
    GemVal _t802 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t802, gem_string("type")), gem_string("if")))) {
#line 586 "compiler/main.gem"
    GemVal _t803 = (*gem_v_expect);
    GemVal _t804[] = {gem_string("end")};
            (void)(_t803.fn(_t803.env, _t804, 1));
        }
#line 588 "compiler/main.gem"
    GemVal _t805[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        GemVal _t806 = gem_fn_make_if(NULL, _t805, 4);
        gem_pop_frame();
        return _t806;
    }
#line 592 "compiler/main.gem"
    GemVal _t807 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t807, gem_string("type")), gem_string("while")))) {
#line 593 "compiler/main.gem"
    GemVal _t808 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t808, gem_string("line"));
#line 594 "compiler/main.gem"
    GemVal _t809 = (*gem_v_advance);
        (void)(_t809.fn(_t809.env, NULL, 0));
#line 595 "compiler/main.gem"
    GemVal _t810 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t810.fn(_t810.env, NULL, 0);
#line 596 "compiler/main.gem"
    GemVal _t811 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t811.fn(_t811.env, NULL, 0);
#line 597 "compiler/main.gem"
    GemVal _t812 = (*gem_v_expect);
    GemVal _t813[] = {gem_string("end")};
        (void)(_t812.fn(_t812.env, _t813, 1));
#line 598 "compiler/main.gem"
    GemVal _t814[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        GemVal _t815 = gem_fn_make_while(NULL, _t814, 3);
        gem_pop_frame();
        return _t815;
    }
#line 602 "compiler/main.gem"
    GemVal _t816 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t816, gem_string("type")), gem_string("for")))) {
#line 603 "compiler/main.gem"
    GemVal _t817 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t817, gem_string("line"));
#line 604 "compiler/main.gem"
    GemVal _t818 = (*gem_v_advance);
        (void)(_t818.fn(_t818.env, NULL, 0));
#line 605 "compiler/main.gem"
    GemVal _t819 = (*gem_v_expect);
    GemVal _t820[] = {gem_string("NAME")};
    GemVal _t821 = _t819.fn(_t819.env, _t820, 1);
        GemVal gem_v_var_name = gem_table_get(_t821, gem_string("value"));
#line 607 "compiler/main.gem"
    GemVal _t822 = (*gem_v_peek);
    GemVal _t823 = _t822.fn(_t822.env, NULL, 0);
    GemVal _t827;
    if (!gem_truthy(gem_eq(gem_table_get(_t823, gem_string("type")), gem_string(",")))) {
        _t827 = gem_eq(gem_table_get(_t823, gem_string("type")), gem_string(","));
    } else {
        GemVal _t824 = (*gem_v_peek_at);
        GemVal _t825[] = {gem_int(1)};
        GemVal _t826 = _t824.fn(_t824.env, _t825, 1);
        _t827 = gem_eq(gem_table_get(_t826, gem_string("type")), gem_string("NAME"));
    }
        if (gem_truthy(_t827)) {
#line 609 "compiler/main.gem"
    GemVal _t828 = (*gem_v_advance);
            (void)(_t828.fn(_t828.env, NULL, 0));
#line 610 "compiler/main.gem"
    GemVal _t829 = (*gem_v_expect);
    GemVal _t830[] = {gem_string("NAME")};
    GemVal _t831 = _t829.fn(_t829.env, _t830, 1);
            GemVal gem_v_val_name = gem_table_get(_t831, gem_string("value"));
#line 611 "compiler/main.gem"
    GemVal _t832 = (*gem_v_expect);
    GemVal _t833[] = {gem_string("in")};
            (void)(_t832.fn(_t832.env, _t833, 1));
#line 612 "compiler/main.gem"
    GemVal _t834 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t834.fn(_t834.env, NULL, 0);
#line 613 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 614 "compiler/main.gem"
    GemVal _t835[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_add(gem_string("_for_tbl_"), gem_to_string_fn(NULL, _t835, 1));
#line 615 "compiler/main.gem"
    GemVal _t836[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_keys_var = gem_add(gem_string("_for_keys_"), gem_to_string_fn(NULL, _t836, 1));
#line 616 "compiler/main.gem"
    GemVal _t837[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t837, 1));
#line 617 "compiler/main.gem"
    GemVal _t838 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t838.fn(_t838.env, NULL, 0);
#line 618 "compiler/main.gem"
    GemVal _t839 = (*gem_v_expect);
    GemVal _t840[] = {gem_string("end")};
            (void)(_t839.fn(_t839.env, _t840, 1));
#line 619 "compiler/main.gem"
    GemVal _t841 = gem_table_new();
            GemVal gem_v_inner_stmts = _t841;
#line 620 "compiler/main.gem"
    GemVal _t842[] = {gem_v_keys_var};
    GemVal _t843[] = {gem_v_idx_var};
    GemVal _t844[] = {gem_fn_make_var(NULL, _t842, 1), gem_fn_make_var(NULL, _t843, 1)};
    GemVal _t845[] = {gem_v_var_name, gem_fn_make_index(NULL, _t844, 2), gem_v_line};
    GemVal _t846[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t845, 3)};
            (void)(gem_push_fn(NULL, _t846, 2));
#line 621 "compiler/main.gem"
    GemVal _t847[] = {gem_v_tbl_var};
    GemVal _t848[] = {gem_v_keys_var};
    GemVal _t849[] = {gem_v_idx_var};
    GemVal _t850[] = {gem_fn_make_var(NULL, _t848, 1), gem_fn_make_var(NULL, _t849, 1)};
    GemVal _t851[] = {gem_fn_make_var(NULL, _t847, 1), gem_fn_make_index(NULL, _t850, 2)};
    GemVal _t852[] = {gem_v_val_name, gem_fn_make_index(NULL, _t851, 2), gem_v_line};
    GemVal _t853[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t852, 3)};
            (void)(gem_push_fn(NULL, _t853, 2));
#line 622 "compiler/main.gem"
    GemVal _t854[] = {gem_v_idx_var};
    GemVal _t855[] = {gem_int(1)};
    GemVal _t856[] = {gem_string("+"), gem_fn_make_var(NULL, _t854, 1), gem_fn_make_int(NULL, _t855, 1)};
    GemVal _t857[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t856, 3), gem_v_line};
    GemVal _t858[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t857, 3)};
            (void)(gem_push_fn(NULL, _t858, 2));
#line 623 "compiler/main.gem"
            GemVal gem_v__for_i_3 = gem_int(0);
#line 623 "compiler/main.gem"
    GemVal _t859[] = {gem_v_fbody};
            GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t859, 1);
#line 623 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 623 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_3;
#line 623 "compiler/main.gem"
                gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 624 "compiler/main.gem"
    GemVal _t860[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t860, 2));
            }

#line 626 "compiler/main.gem"
    GemVal _t861[] = {gem_v_idx_var};
    GemVal _t862[] = {gem_string("len")};
    GemVal _t863 = gem_table_new();
    GemVal _t864[] = {gem_v_keys_var};
    gem_table_set(_t863, gem_int(0), gem_fn_make_var(NULL, _t864, 1));
    GemVal _t865[] = {gem_fn_make_var(NULL, _t862, 1), _t863, gem_int(0)};
    GemVal _t866[] = {gem_string("<"), gem_fn_make_var(NULL, _t861, 1), gem_fn_make_call(NULL, _t865, 3)};
    GemVal _t867[] = {gem_fn_make_binop(NULL, _t866, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t867, 3);
#line 631 "compiler/main.gem"
    GemVal _t868 = gem_table_new();
    gem_table_set(_t868, gem_string("tag"), gem_string("block"));
    GemVal _t869 = gem_table_new();
    GemVal _t870[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    gem_table_set(_t869, gem_int(0), gem_fn_make_let(NULL, _t870, 3));
    GemVal _t871[] = {gem_string("keys")};
    GemVal _t872 = gem_table_new();
    GemVal _t873[] = {gem_v_tbl_var};
    gem_table_set(_t872, gem_int(0), gem_fn_make_var(NULL, _t873, 1));
    GemVal _t874[] = {gem_fn_make_var(NULL, _t871, 1), _t872, gem_int(0)};
    GemVal _t875[] = {gem_v_keys_var, gem_fn_make_call(NULL, _t874, 3), gem_v_line};
    gem_table_set(_t869, gem_int(1), gem_fn_make_let(NULL, _t875, 3));
    GemVal _t876[] = {gem_int(0)};
    GemVal _t877[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t876, 1), gem_v_line};
    gem_table_set(_t869, gem_int(2), gem_fn_make_let(NULL, _t877, 3));
    gem_table_set(_t869, gem_int(3), gem_v_while_node);
    gem_table_set(_t868, gem_string("stmts"), _t869);
            GemVal _t878 = _t868;
            gem_pop_frame();
            return _t878;
        } else {
#line 637 "compiler/main.gem"
    GemVal _t879 = (*gem_v_peek);
    GemVal _t880 = _t879.fn(_t879.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t880, gem_string("type")), gem_string("in")))) {
#line 639 "compiler/main.gem"
    GemVal _t881 = (*gem_v_advance);
                (void)(_t881.fn(_t881.env, NULL, 0));
#line 640 "compiler/main.gem"
    GemVal _t882 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t882.fn(_t882.env, NULL, 0);
#line 641 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 642 "compiler/main.gem"
    GemVal _t883[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t883, 1));
#line 643 "compiler/main.gem"
    GemVal _t884[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t884, 1));
#line 644 "compiler/main.gem"
    GemVal _t885 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t885.fn(_t885.env, NULL, 0);
#line 645 "compiler/main.gem"
    GemVal _t886 = (*gem_v_expect);
    GemVal _t887[] = {gem_string("end")};
                (void)(_t886.fn(_t886.env, _t887, 1));
#line 646 "compiler/main.gem"
    GemVal _t888 = gem_table_new();
                GemVal gem_v_inner_stmts = _t888;
#line 647 "compiler/main.gem"
    GemVal _t889[] = {gem_v_items_var};
    GemVal _t890[] = {gem_v_idx_var};
    GemVal _t891[] = {gem_fn_make_var(NULL, _t889, 1), gem_fn_make_var(NULL, _t890, 1)};
    GemVal _t892[] = {gem_v_var_name, gem_fn_make_index(NULL, _t891, 2), gem_v_line};
    GemVal _t893[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t892, 3)};
                (void)(gem_push_fn(NULL, _t893, 2));
#line 648 "compiler/main.gem"
    GemVal _t894[] = {gem_v_idx_var};
    GemVal _t895[] = {gem_int(1)};
    GemVal _t896[] = {gem_string("+"), gem_fn_make_var(NULL, _t894, 1), gem_fn_make_int(NULL, _t895, 1)};
    GemVal _t897[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t896, 3), gem_v_line};
    GemVal _t898[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t897, 3)};
                (void)(gem_push_fn(NULL, _t898, 2));
#line 649 "compiler/main.gem"
                GemVal gem_v__for_i_4 = gem_int(0);
#line 649 "compiler/main.gem"
    GemVal _t899[] = {gem_v_fbody};
                GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t899, 1);
#line 649 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 649 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_4;
#line 649 "compiler/main.gem"
                    gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 650 "compiler/main.gem"
    GemVal _t900[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t900, 2));
                }

#line 652 "compiler/main.gem"
    GemVal _t901[] = {gem_v_idx_var};
    GemVal _t902[] = {gem_string("len")};
    GemVal _t903 = gem_table_new();
    GemVal _t904[] = {gem_v_items_var};
    gem_table_set(_t903, gem_int(0), gem_fn_make_var(NULL, _t904, 1));
    GemVal _t905[] = {gem_fn_make_var(NULL, _t902, 1), _t903, gem_int(0)};
    GemVal _t906[] = {gem_string("<"), gem_fn_make_var(NULL, _t901, 1), gem_fn_make_call(NULL, _t905, 3)};
    GemVal _t907[] = {gem_fn_make_binop(NULL, _t906, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t907, 3);
#line 657 "compiler/main.gem"
    GemVal _t908 = gem_table_new();
    gem_table_set(_t908, gem_string("tag"), gem_string("block"));
    GemVal _t909 = gem_table_new();
    GemVal _t910[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t909, gem_int(0), gem_fn_make_let(NULL, _t910, 3));
    GemVal _t911[] = {gem_int(0)};
    GemVal _t912[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t911, 1), gem_v_line};
    gem_table_set(_t909, gem_int(1), gem_fn_make_let(NULL, _t912, 3));
    gem_table_set(_t909, gem_int(2), gem_v_while_node);
    gem_table_set(_t908, gem_string("stmts"), _t909);
                GemVal _t913 = _t908;
                gem_pop_frame();
                return _t913;
            } else {
#line 662 "compiler/main.gem"
    GemVal _t914 = (*gem_v_peek);
    GemVal _t915 = _t914.fn(_t914.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t915, gem_string("type")), gem_string("=")))) {
#line 664 "compiler/main.gem"
    GemVal _t916 = (*gem_v_advance);
                    (void)(_t916.fn(_t916.env, NULL, 0));
#line 665 "compiler/main.gem"
    GemVal _t917 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t917.fn(_t917.env, NULL, 0);
#line 666 "compiler/main.gem"
    GemVal _t918 = (*gem_v_expect);
    GemVal _t919[] = {gem_string(",")};
                    (void)(_t918.fn(_t918.env, _t919, 1));
#line 667 "compiler/main.gem"
    GemVal _t920 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t920.fn(_t920.env, NULL, 0);
#line 668 "compiler/main.gem"
    GemVal _t921 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t921.fn(_t921.env, NULL, 0);
#line 669 "compiler/main.gem"
    GemVal _t922 = (*gem_v_expect);
    GemVal _t923[] = {gem_string("end")};
                    (void)(_t922.fn(_t922.env, _t923, 1));
#line 670 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 671 "compiler/main.gem"
    GemVal _t924[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t924, 1));
#line 672 "compiler/main.gem"
    GemVal _t925[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t925, 1));
#line 673 "compiler/main.gem"
    GemVal _t926 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t926;
#line 674 "compiler/main.gem"
    GemVal _t927[] = {gem_v_idx_var};
    GemVal _t928[] = {gem_v_var_name, gem_fn_make_var(NULL, _t927, 1), gem_v_line};
    GemVal _t929[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t928, 3)};
                    (void)(gem_push_fn(NULL, _t929, 2));
#line 675 "compiler/main.gem"
    GemVal _t930[] = {gem_v_idx_var};
    GemVal _t931[] = {gem_int(1)};
    GemVal _t932[] = {gem_string("+"), gem_fn_make_var(NULL, _t930, 1), gem_fn_make_int(NULL, _t931, 1)};
    GemVal _t933[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t932, 3), gem_v_line};
    GemVal _t934[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t933, 3)};
                    (void)(gem_push_fn(NULL, _t934, 2));
#line 676 "compiler/main.gem"
                    GemVal gem_v__for_i_5 = gem_int(0);
#line 676 "compiler/main.gem"
    GemVal _t935[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_5 = gem_len_fn(NULL, _t935, 1);
#line 676 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_limit_5))) break;
#line 676 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_5;
#line 676 "compiler/main.gem"
                        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 677 "compiler/main.gem"
    GemVal _t936[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t936, 2));
                    }

#line 679 "compiler/main.gem"
    GemVal _t937[] = {gem_v_idx_var};
    GemVal _t938[] = {gem_v_limit_var};
    GemVal _t939[] = {gem_string("<"), gem_fn_make_var(NULL, _t937, 1), gem_fn_make_var(NULL, _t938, 1)};
    GemVal _t940[] = {gem_fn_make_binop(NULL, _t939, 3), gem_v_inner_stmts, gem_v_line};
                    GemVal gem_v_while_node = gem_fn_make_while(NULL, _t940, 3);
#line 684 "compiler/main.gem"
    GemVal _t941 = gem_table_new();
    gem_table_set(_t941, gem_string("tag"), gem_string("block"));
    GemVal _t942 = gem_table_new();
    GemVal _t943[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t942, gem_int(0), gem_fn_make_let(NULL, _t943, 3));
    GemVal _t944[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t942, gem_int(1), gem_fn_make_let(NULL, _t944, 3));
    gem_table_set(_t942, gem_int(2), gem_v_while_node);
    gem_table_set(_t941, gem_string("stmts"), _t942);
                    GemVal _t945 = _t941;
                    gem_pop_frame();
                    return _t945;
                } else {
#line 690 "compiler/main.gem"
    GemVal _t946 = (*gem_v_peek);
    GemVal _t947 = _t946.fn(_t946.env, NULL, 0);
    GemVal _t948[] = {gem_table_get(_t947, gem_string("line"))};
    GemVal _t949[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t948, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 690, _t949, 1));
                }
            }
        }
    }
#line 695 "compiler/main.gem"
    GemVal _t950 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t950, gem_string("type")), gem_string("match")))) {
#line 696 "compiler/main.gem"
    GemVal _t951 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t951, gem_string("line"));
#line 697 "compiler/main.gem"
    GemVal _t952 = (*gem_v_advance);
        (void)(_t952.fn(_t952.env, NULL, 0));
#line 698 "compiler/main.gem"
    GemVal _t953 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t953.fn(_t953.env, NULL, 0);
#line 699 "compiler/main.gem"
    GemVal _t954 = (*gem_v_skip_nl);
        (void)(_t954.fn(_t954.env, NULL, 0));
#line 700 "compiler/main.gem"
    GemVal _t955 = gem_table_new();
        GemVal gem_v_whens = _t955;
#line 701 "compiler/main.gem"
        while (1) {
            GemVal _t956 = (*gem_v_peek);
            GemVal _t957 = _t956.fn(_t956.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t957, gem_string("type")), gem_string("when")))) break;
#line 702 "compiler/main.gem"
    GemVal _t958 = (*gem_v_advance);
            (void)(_t958.fn(_t958.env, NULL, 0));
#line 703 "compiler/main.gem"
    GemVal _t959 = (*gem_v_parse_expr);
            GemVal gem_v_wval = _t959.fn(_t959.env, NULL, 0);
#line 704 "compiler/main.gem"
    GemVal _t960 = (*gem_v_parse_body);
            GemVal gem_v_wbody = _t960.fn(_t960.env, NULL, 0);
#line 705 "compiler/main.gem"
    GemVal _t961[] = {gem_v_wval, gem_v_wbody};
    GemVal _t962[] = {gem_v_whens, gem_fn_make_when(NULL, _t961, 2)};
            (void)(gem_push_fn(NULL, _t962, 2));
#line 706 "compiler/main.gem"
    GemVal _t963 = (*gem_v_skip_nl);
            (void)(_t963.fn(_t963.env, NULL, 0));
        }
#line 708 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 709 "compiler/main.gem"
    GemVal _t964 = (*gem_v_peek);
    GemVal _t965 = _t964.fn(_t964.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t965, gem_string("type")), gem_string("else")))) {
#line 710 "compiler/main.gem"
    GemVal _t966 = (*gem_v_advance);
            (void)(_t966.fn(_t966.env, NULL, 0));
#line 711 "compiler/main.gem"
    GemVal _t967 = (*gem_v_parse_body);
            gem_v_else_body = _t967.fn(_t967.env, NULL, 0);
        }
#line 713 "compiler/main.gem"
    GemVal _t968 = (*gem_v_expect);
    GemVal _t969[] = {gem_string("end")};
        (void)(_t968.fn(_t968.env, _t969, 1));
#line 714 "compiler/main.gem"
    GemVal _t970[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line};
        GemVal _t971 = gem_fn_make_match(NULL, _t970, 4);
        gem_pop_frame();
        return _t971;
    }
#line 718 "compiler/main.gem"
    GemVal _t972 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t972, gem_string("type")), gem_string("return")))) {
#line 719 "compiler/main.gem"
    GemVal _t973 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t973, gem_string("line"));
#line 720 "compiler/main.gem"
    GemVal _t974 = (*gem_v_advance);
        (void)(_t974.fn(_t974.env, NULL, 0));
#line 721 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 722 "compiler/main.gem"
    GemVal _t975 = (*gem_v_peek);
    GemVal _t976 = _t975.fn(_t975.env, NULL, 0);
    GemVal _t979;
    if (!gem_truthy(gem_neq(gem_table_get(_t976, gem_string("type")), gem_string("NEWLINE")))) {
        _t979 = gem_neq(gem_table_get(_t976, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t977 = (*gem_v_peek);
        GemVal _t978 = _t977.fn(_t977.env, NULL, 0);
        _t979 = gem_neq(gem_table_get(_t978, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t982;
    if (!gem_truthy(_t979)) {
        _t982 = _t979;
    } else {
        GemVal _t980 = (*gem_v_peek);
        GemVal _t981 = _t980.fn(_t980.env, NULL, 0);
        _t982 = gem_neq(gem_table_get(_t981, gem_string("type")), gem_string("end"));
    }
    GemVal _t985;
    if (!gem_truthy(_t982)) {
        _t985 = _t982;
    } else {
        GemVal _t983 = (*gem_v_peek);
        GemVal _t984 = _t983.fn(_t983.env, NULL, 0);
        _t985 = gem_neq(gem_table_get(_t984, gem_string("type")), gem_string("elif"));
    }
    GemVal _t988;
    if (!gem_truthy(_t985)) {
        _t988 = _t985;
    } else {
        GemVal _t986 = (*gem_v_peek);
        GemVal _t987 = _t986.fn(_t986.env, NULL, 0);
        _t988 = gem_neq(gem_table_get(_t987, gem_string("type")), gem_string("else"));
    }
    GemVal _t991;
    if (!gem_truthy(_t988)) {
        _t991 = _t988;
    } else {
        GemVal _t989 = (*gem_v_peek);
        GemVal _t990 = _t989.fn(_t989.env, NULL, 0);
        _t991 = gem_neq(gem_table_get(_t990, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t991)) {
#line 723 "compiler/main.gem"
    GemVal _t992 = (*gem_v_parse_expr);
            gem_v_value = _t992.fn(_t992.env, NULL, 0);
        }
#line 725 "compiler/main.gem"
    GemVal _t993[] = {gem_v_value, gem_v_line};
        GemVal _t994 = gem_fn_make_return(NULL, _t993, 2);
        gem_pop_frame();
        return _t994;
    }
#line 729 "compiler/main.gem"
    GemVal _t995 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t995, gem_string("type")), gem_string("break")))) {
#line 730 "compiler/main.gem"
    GemVal _t996 = (*gem_v_advance);
        (void)(_t996.fn(_t996.env, NULL, 0));
#line 731 "compiler/main.gem"
        GemVal _t997 = gem_fn_make_break(NULL, NULL, 0);
        gem_pop_frame();
        return _t997;
    }
#line 735 "compiler/main.gem"
    GemVal _t998 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t998, gem_string("type")), gem_string("continue")))) {
#line 736 "compiler/main.gem"
    GemVal _t999 = (*gem_v_advance);
        (void)(_t999.fn(_t999.env, NULL, 0));
#line 737 "compiler/main.gem"
        GemVal _t1000 = gem_fn_make_continue(NULL, NULL, 0);
        gem_pop_frame();
        return _t1000;
    }
#line 741 "compiler/main.gem"
    GemVal _t1001 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1001, gem_string("type")), gem_string("load")))) {
#line 742 "compiler/main.gem"
    GemVal _t1002 = (*gem_v_advance);
        (void)(_t1002.fn(_t1002.env, NULL, 0));
#line 743 "compiler/main.gem"
    GemVal _t1003 = (*gem_v_expect);
    GemVal _t1004[] = {gem_string("STRING")};
    GemVal _t1005 = _t1003.fn(_t1003.env, _t1004, 1);
        GemVal gem_v_path = gem_table_get(_t1005, gem_string("value"));
#line 744 "compiler/main.gem"
    GemVal _t1006[] = {gem_v_path};
        GemVal _t1007 = gem_fn_make_load(NULL, _t1006, 1);
        gem_pop_frame();
        return _t1007;
    }
#line 748 "compiler/main.gem"
    GemVal _t1008 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1008, gem_string("type")), gem_string("extern")))) {
#line 749 "compiler/main.gem"
    GemVal _t1009 = (*gem_v_advance);
        (void)(_t1009.fn(_t1009.env, NULL, 0));
#line 750 "compiler/main.gem"
    GemVal _t1010 = (*gem_v_peek);
    GemVal _t1011 = _t1010.fn(_t1010.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1011, gem_string("type")), gem_string("fn")))) {
#line 751 "compiler/main.gem"
    GemVal _t1012 = (*gem_v_advance);
            (void)(_t1012.fn(_t1012.env, NULL, 0));
#line 752 "compiler/main.gem"
    GemVal _t1013 = (*gem_v_expect);
    GemVal _t1014[] = {gem_string("NAME")};
    GemVal _t1015 = _t1013.fn(_t1013.env, _t1014, 1);
            GemVal gem_v_name = gem_table_get(_t1015, gem_string("value"));
#line 753 "compiler/main.gem"
    GemVal _t1016 = (*gem_v_expect);
    GemVal _t1017[] = {gem_string("(")};
            (void)(_t1016.fn(_t1016.env, _t1017, 1));
#line 754 "compiler/main.gem"
    GemVal _t1018 = gem_table_new();
            GemVal gem_v_eparams = _t1018;
#line 755 "compiler/main.gem"
    GemVal _t1019 = (*gem_v_peek);
    GemVal _t1020 = _t1019.fn(_t1019.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t1020, gem_string("type")), gem_string(")")))) {
#line 756 "compiler/main.gem"
    GemVal _t1021 = (*gem_v_expect);
    GemVal _t1022[] = {gem_string("NAME")};
    GemVal _t1023 = _t1021.fn(_t1021.env, _t1022, 1);
                GemVal gem_v_pname = gem_table_get(_t1023, gem_string("value"));
#line 757 "compiler/main.gem"
    GemVal _t1024 = (*gem_v_expect);
    GemVal _t1025[] = {gem_string(":")};
                (void)(_t1024.fn(_t1024.env, _t1025, 1));
#line 758 "compiler/main.gem"
    GemVal _t1026 = (*gem_v_expect);
    GemVal _t1027[] = {gem_string("NAME")};
    GemVal _t1028 = _t1026.fn(_t1026.env, _t1027, 1);
                GemVal gem_v_ptype = gem_table_get(_t1028, gem_string("value"));
#line 759 "compiler/main.gem"
    GemVal _t1029[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1030[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1029, 2)};
                (void)(gem_push_fn(NULL, _t1030, 2));
#line 760 "compiler/main.gem"
                while (1) {
                    GemVal _t1031 = (*gem_v_peek);
                    GemVal _t1032 = _t1031.fn(_t1031.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1032, gem_string("type")), gem_string(",")))) break;
#line 761 "compiler/main.gem"
    GemVal _t1033 = (*gem_v_advance);
                    (void)(_t1033.fn(_t1033.env, NULL, 0));
#line 762 "compiler/main.gem"
    GemVal _t1034 = (*gem_v_expect);
    GemVal _t1035[] = {gem_string("NAME")};
    GemVal _t1036 = _t1034.fn(_t1034.env, _t1035, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t1036, gem_string("value"));
#line 763 "compiler/main.gem"
    GemVal _t1037 = (*gem_v_expect);
    GemVal _t1038[] = {gem_string(":")};
                    (void)(_t1037.fn(_t1037.env, _t1038, 1));
#line 764 "compiler/main.gem"
    GemVal _t1039 = (*gem_v_expect);
    GemVal _t1040[] = {gem_string("NAME")};
    GemVal _t1041 = _t1039.fn(_t1039.env, _t1040, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t1041, gem_string("value"));
#line 765 "compiler/main.gem"
    GemVal _t1042[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1043[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1042, 2)};
                    (void)(gem_push_fn(NULL, _t1043, 2));
                }
            }
#line 768 "compiler/main.gem"
    GemVal _t1044 = (*gem_v_expect);
    GemVal _t1045[] = {gem_string(")")};
            (void)(_t1044.fn(_t1044.env, _t1045, 1));
#line 769 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 770 "compiler/main.gem"
    GemVal _t1046 = (*gem_v_peek);
    GemVal _t1047 = _t1046.fn(_t1046.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1047, gem_string("type")), gem_string("->")))) {
#line 771 "compiler/main.gem"
    GemVal _t1048 = (*gem_v_advance);
                (void)(_t1048.fn(_t1048.env, NULL, 0));
#line 772 "compiler/main.gem"
    GemVal _t1049 = (*gem_v_expect);
    GemVal _t1050[] = {gem_string("NAME")};
    GemVal _t1051 = _t1049.fn(_t1049.env, _t1050, 1);
                gem_v_ret_type = gem_table_get(_t1051, gem_string("value"));
            }
#line 774 "compiler/main.gem"
    GemVal _t1052[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            GemVal _t1053 = gem_fn_make_extern_fn(NULL, _t1052, 3);
            gem_pop_frame();
            return _t1053;
        } else {
#line 775 "compiler/main.gem"
    GemVal _t1054 = (*gem_v_peek);
    GemVal _t1055 = _t1054.fn(_t1054.env, NULL, 0);
    GemVal _t1058;
    if (!gem_truthy(gem_eq(gem_table_get(_t1055, gem_string("type")), gem_string("NAME")))) {
        _t1058 = gem_eq(gem_table_get(_t1055, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1056 = (*gem_v_peek);
        GemVal _t1057 = _t1056.fn(_t1056.env, NULL, 0);
        _t1058 = gem_eq(gem_table_get(_t1057, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t1058)) {
#line 776 "compiler/main.gem"
    GemVal _t1059 = (*gem_v_advance);
                (void)(_t1059.fn(_t1059.env, NULL, 0));
#line 777 "compiler/main.gem"
    GemVal _t1060 = (*gem_v_expect);
    GemVal _t1061[] = {gem_string("STRING")};
    GemVal _t1062 = _t1060.fn(_t1060.env, _t1061, 1);
                GemVal gem_v_path = gem_table_get(_t1062, gem_string("value"));
#line 778 "compiler/main.gem"
    GemVal _t1063[] = {gem_v_path};
                GemVal _t1064 = gem_fn_make_extern_include(NULL, _t1063, 1);
                gem_pop_frame();
                return _t1064;
            } else {
#line 780 "compiler/main.gem"
    GemVal _t1065 = (*gem_v_peek);
    GemVal _t1066 = _t1065.fn(_t1065.env, NULL, 0);
    GemVal _t1067[] = {gem_table_get(_t1066, gem_string("line"))};
    GemVal _t1068[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t1067, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 780, _t1068, 1));
            }
        }
    }
#line 785 "compiler/main.gem"
    GemVal _t1069 = (*gem_v_parse_expr);
    GemVal _t1070 = _t1069.fn(_t1069.env, NULL, 0);
    gem_pop_frame();
    return _t1070;
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
    GemVal _t1072 = gem_table_new();
    GemVal gem_v_stmts = _t1072;
#line 795 "compiler/main.gem"
    GemVal _t1073 = (*gem_v_skip_nl);
    (void)(_t1073.fn(_t1073.env, NULL, 0));
#line 796 "compiler/main.gem"
    while (1) {
        GemVal _t1074 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1074.fn(_t1074.env, NULL, 0)))) break;
#line 797 "compiler/main.gem"
    GemVal _t1075 = (*gem_v_parse_stmt);
    GemVal _t1076[] = {gem_v_stmts, _t1075.fn(_t1075.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1076, 2));
#line 798 "compiler/main.gem"
    GemVal _t1077 = (*gem_v_skip_nl);
        (void)(_t1077.fn(_t1077.env, NULL, 0));
    }
#line 800 "compiler/main.gem"
    GemVal _t1078[] = {gem_v_stmts};
    GemVal _t1079 = gem_fn_make_program(NULL, _t1078, 1);
    gem_pop_frame();
    return _t1079;
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
    struct _closure__anon_1 *_t235 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t235->gem_v_pos = gem_v_pos;
    _t235->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t235);
#line 29 "compiler/main.gem"
    struct _closure__anon_2 *_t237 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t237->gem_v_pos = gem_v_pos;
    _t237->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t237);
#line 33 "compiler/main.gem"
    struct _closure__anon_3 *_t239 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t239->gem_v_pos = gem_v_pos;
    _t239->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t239);
#line 39 "compiler/main.gem"
    struct _closure__anon_4 *_t242 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t242->gem_v_pos = gem_v_pos;
    _t242->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t242);
#line 43 "compiler/main.gem"
    struct _closure__anon_5 *_t251 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t251->gem_v_pos = gem_v_pos;
    _t251->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t251);
#line 52 "compiler/main.gem"
    struct _closure__anon_6 *_t253 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t253->gem_v_pos = gem_v_pos;
    _t253->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t253);
#line 64 "compiler/main.gem"
    struct _closure__anon_7 *_t291 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t291->gem_v_advance = gem_v_advance;
    _t291->gem_v_expect = gem_v_expect;
    _t291->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t291);
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
    struct _closure__anon_8 *_t311 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t311->gem_v_at_end = gem_v_at_end;
    _t311->gem_v_parse_stmt = gem_v_parse_stmt;
    _t311->gem_v_peek = gem_v_peek;
    _t311->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t311);
#line 119 "compiler/main.gem"
    struct _closure__anon_9 *_t322 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t322->gem_v_at_end = gem_v_at_end;
    _t322->gem_v_parse_stmt = gem_v_parse_stmt;
    _t322->gem_v_peek = gem_v_peek;
    _t322->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t322);
#line 132 "compiler/main.gem"
    struct _closure__anon_10 *_t452 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t452->gem_v_advance = gem_v_advance;
    _t452->gem_v_expect = gem_v_expect;
    _t452->gem_v_fn_depth = gem_v_fn_depth;
    _t452->gem_v_parse_expr = gem_v_parse_expr;
    _t452->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t452->gem_v_parse_params = gem_v_parse_params;
    _t452->gem_v_peek = gem_v_peek;
    _t452->gem_v_peek_at = gem_v_peek_at;
    _t452->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t452);
#line 263 "compiler/main.gem"
    struct _closure__anon_11 *_t543 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t543->gem_v_advance = gem_v_advance;
    _t543->gem_v_expect = gem_v_expect;
    _t543->gem_v_fn_depth = gem_v_fn_depth;
    _t543->gem_v_parse_atom = gem_v_parse_atom;
    _t543->gem_v_parse_expr = gem_v_parse_expr;
    _t543->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t543->gem_v_parse_params = gem_v_parse_params;
    _t543->gem_v_peek = gem_v_peek;
    _t543->gem_v_peek_at = gem_v_peek_at;
    _t543->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t543);
#line 369 "compiler/main.gem"
    struct _closure__anon_12 *_t552 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t552->gem_v_advance = gem_v_advance;
    _t552->gem_v_parse_call = gem_v_parse_call;
    _t552->gem_v_parse_unary = gem_v_parse_unary;
    _t552->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t552);
#line 378 "compiler/main.gem"
    struct _closure__anon_13 *_t567 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t567->gem_v_advance = gem_v_advance;
    _t567->gem_v_parse_unary = gem_v_parse_unary;
    _t567->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t567);
#line 389 "compiler/main.gem"
    struct _closure__anon_14 *_t579 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t579->gem_v_advance = gem_v_advance;
    _t579->gem_v_parse_mul = gem_v_parse_mul;
    _t579->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t579);
#line 400 "compiler/main.gem"
    struct _closure__anon_15 *_t606 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t606->gem_v_advance = gem_v_advance;
    _t606->gem_v_parse_add = gem_v_parse_add;
    _t606->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t606);
#line 411 "compiler/main.gem"
    struct _closure__anon_16 *_t615 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t615->gem_v_advance = gem_v_advance;
    _t615->gem_v_parse_compare = gem_v_parse_compare;
    _t615->gem_v_parse_not = gem_v_parse_not;
    _t615->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t615);
#line 420 "compiler/main.gem"
    struct _closure__anon_17 *_t623 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t623->gem_v_advance = gem_v_advance;
    _t623->gem_v_parse_not = gem_v_parse_not;
    _t623->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t623);
#line 431 "compiler/main.gem"
    struct _closure__anon_18 *_t631 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t631->gem_v_advance = gem_v_advance;
    _t631->gem_v_parse_and = gem_v_parse_and;
    _t631->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t631);
#line 442 "compiler/main.gem"
    struct _closure__anon_19 *_t682 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t682->gem_v_advance = gem_v_advance;
    _t682->gem_v_parse_expr = gem_v_parse_expr;
    _t682->gem_v_parse_or = gem_v_parse_or;
    _t682->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t682);
#line 477 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 481 "compiler/main.gem"
    struct _closure__anon_20 *_t1071 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t1071->gem_v_advance = gem_v_advance;
    _t1071->gem_v_expect = gem_v_expect;
    _t1071->gem_v_fn_depth = gem_v_fn_depth;
    _t1071->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1071->gem_v_parse_body = gem_v_parse_body;
    _t1071->gem_v_parse_expr = gem_v_parse_expr;
    _t1071->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1071->gem_v_parse_params = gem_v_parse_params;
    _t1071->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1071->gem_v_peek = gem_v_peek;
    _t1071->gem_v_peek_at = gem_v_peek_at;
    _t1071->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_20, _t1071);
#line 789 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 793 "compiler/main.gem"
    struct _closure__anon_21 *_t1080 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t1080->gem_v_at_end = gem_v_at_end;
    _t1080->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1080->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_21, _t1080);
    GemVal _t1081 = gem_table_new();
    gem_table_set(_t1081, gem_string("parse"), gem_v_parse);
    GemVal _t1082 = _t1081;
    gem_pop_frame();
    return _t1082;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("escape_c_string", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal _t1083[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1083, 1);
#line 11 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 12 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 12 "compiler/main.gem"
    GemVal _t1084[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1084, 1);
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
    GemVal _t1085[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1085, 1), gem_int(0)))) {
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

    GemVal _t1086 = gem_v_result;
    gem_pop_frame();
    return _t1086;
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

    GemVal _t1087 = gem_v_result;
    gem_pop_frame();
    return _t1087;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 41 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("pad_for", "compiler/main.gem", 41);
#line 42 "compiler/main.gem"
    GemVal _t1088[] = {gem_string("    "), gem_v_indent};
    GemVal _t1089 = gem_fn_repeat_str(NULL, _t1088, 2);
    gem_pop_frame();
    return _t1089;
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 45 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("format_float", "compiler/main.gem", 45);
#line 46 "compiler/main.gem"
    GemVal _t1090[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1090, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 49 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 49 "compiler/main.gem"
    GemVal _t1091[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1091, 1);
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
    GemVal _t1092 = gem_v_s;
    gem_pop_frame();
    return _t1092;
}

static GemVal gem_fn_array_contains(void *_env, GemVal *args, int argc) {
#line 62 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("array_contains", "compiler/main.gem", 62);
#line 63 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 63 "compiler/main.gem"
    GemVal _t1093[] = {gem_v_arr};
    GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t1093, 1);
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
            GemVal _t1094 = gem_bool(1);
            gem_pop_frame();
            return _t1094;
        }
    }

    GemVal _t1095 = gem_bool(0);
    gem_pop_frame();
    return _t1095;
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
    GemVal _t1096 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1096;
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 81 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_union", "compiler/main.gem", 81);
#line 82 "compiler/main.gem"
    GemVal _t1097 = gem_table_new();
    GemVal gem_v_result = _t1097;
#line 83 "compiler/main.gem"
    GemVal _t1098[] = {gem_v_a};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t1098, 1);
#line 84 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 85 "compiler/main.gem"
    while (1) {
        GemVal _t1099[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t1099, 1)))) break;
#line 86 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
#line 87 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 89 "compiler/main.gem"
    GemVal _t1100[] = {gem_v_b};
    GemVal gem_v_ks2 = gem_keys_fn(NULL, _t1100, 1);
#line 90 "compiler/main.gem"
    GemVal gem_v_j = gem_int(0);
#line 91 "compiler/main.gem"
    while (1) {
        GemVal _t1101[] = {gem_v_ks2};
        if (!gem_truthy(gem_lt(gem_v_j, gem_len_fn(NULL, _t1101, 1)))) break;
#line 92 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks2, gem_v_j), gem_bool(1));
#line 93 "compiler/main.gem"
        gem_v_j = gem_add(gem_v_j, gem_int(1));
    }
    GemVal _t1102 = gem_v_result;
    gem_pop_frame();
    return _t1102;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 98 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_intersect", "compiler/main.gem", 98);
#line 99 "compiler/main.gem"
    GemVal _t1103 = gem_table_new();
    GemVal gem_v_result = _t1103;
#line 100 "compiler/main.gem"
    GemVal _t1104[] = {gem_v_a};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t1104, 1);
#line 101 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 102 "compiler/main.gem"
    while (1) {
        GemVal _t1105[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t1105, 1)))) break;
#line 103 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_table_get(gem_v_ks, gem_v_i)), gem_bool(1)))) {
#line 104 "compiler/main.gem"
            gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
        }
#line 106 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t1106 = gem_v_result;
    gem_pop_frame();
    return _t1106;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
#line 111 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("set_to_sorted_array", "compiler/main.gem", 111);
#line 112 "compiler/main.gem"
    GemVal _t1107[] = {gem_v_s};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t1107, 1);
#line 114 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 115 "compiler/main.gem"
    while (1) {
        GemVal _t1108[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t1108, 1)))) break;
#line 116 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
#line 117 "compiler/main.gem"
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
#line 118 "compiler/main.gem"
        while (1) {
            GemVal _t1109;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t1109 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t1109 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t1109)) break;
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
    GemVal _t1110 = gem_v_ks;
    gem_pop_frame();
    return _t1110;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 128 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("sorted_array_to_set", "compiler/main.gem", 128);
#line 129 "compiler/main.gem"
    GemVal _t1111 = gem_table_new();
    GemVal gem_v_s = _t1111;
#line 130 "compiler/main.gem"
    GemVal gem_v__for_items_5 = gem_v_arr;
#line 130 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 130 "compiler/main.gem"
    while (1) {
        GemVal _t1112[] = {gem_v__for_items_5};
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1112, 1)))) break;
#line 130 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 130 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 131 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1113 = gem_v_s;
    gem_pop_frame();
    return _t1113;
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
    GemVal _t1114[] = {gem_v_stmts};
        GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t1114, 1);
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
    GemVal _t1115[] = {gem_v_s};
    GemVal _t1117;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1115, 1), gem_string("table")))) {
        _t1117 = gem_eq(gem_type_fn(NULL, _t1115, 1), gem_string("table"));
    } else {
        GemVal _t1116 = gem_v_s;
        _t1117 = gem_eq(gem_table_get(_t1116, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1117)) {
#line 142 "compiler/main.gem"
    GemVal _t1118 = gem_v_s;
    GemVal _t1119[] = {gem_v_result, gem_table_get(_t1118, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1119, 2));
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
    GemVal _t1131[] = {(*gem_v_tmp_counter)};
    GemVal _t1132 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t1131, 1));
    gem_pop_frame();
    return _t1132;
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
    GemVal _t1134[] = {(*gem_v_anon_counter)};
    GemVal _t1135 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t1134, 1));
    gem_pop_frame();
    return _t1135;
}

struct _closure__anon_24 {
};
static GemVal _anon_24(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_24", "compiler/main.gem", 0);
    GemVal _t1137[] = {gem_v_name};
    GemVal _t1138 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t1137, 1));
    gem_pop_frame();
    return _t1138;
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
        GemVal _t1139 = GEM_NIL;
        gem_pop_frame();
        return _t1139;
    }
#line 211 "compiler/main.gem"
    GemVal _t1140[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1140, 1), gem_string("table")))) {
#line 212 "compiler/main.gem"
        GemVal _t1141 = GEM_NIL;
        gem_pop_frame();
        return _t1141;
    }
#line 214 "compiler/main.gem"
    GemVal _t1142 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1142, gem_string("tag"));
#line 215 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 216 "compiler/main.gem"
        GemVal _t1143 = GEM_NIL;
        gem_pop_frame();
        return _t1143;
    }
#line 219 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 220 "compiler/main.gem"
    GemVal _t1144 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1144, gem_string("name"));
#line 221 "compiler/main.gem"
    GemVal _t1145[] = {gem_v_defined, gem_v_name};
    GemVal _t1147;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1145, 2)))) {
        _t1147 = gem_not(gem_fn_set_contains(NULL, _t1145, 2));
    } else {
        GemVal _t1146[] = {(*gem_v_builtins), gem_v_name};
        _t1147 = gem_not(gem_fn_set_contains(NULL, _t1146, 2));
    }
    GemVal _t1149;
    if (!gem_truthy(_t1147)) {
        _t1149 = _t1147;
    } else {
        GemVal _t1148[] = {(*gem_v_defined_fns), gem_v_name};
        _t1149 = gem_not(gem_fn_set_contains(NULL, _t1148, 2));
    }
        if (gem_truthy(_t1149)) {
#line 222 "compiler/main.gem"
    GemVal _t1150[] = {gem_v_free, gem_v_name};
            GemVal _t1151 = gem_fn_set_add(NULL, _t1150, 2);
            gem_pop_frame();
            return _t1151;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
#line 224 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 225 "compiler/main.gem"
    GemVal _t1152 = gem_v_node;
    GemVal _t1153 = (*gem_v_collect_free_node);
    GemVal _t1154[] = {gem_table_get(_t1152, gem_string("value")), gem_v_defined, gem_v_free};
            GemVal _t1155 = _t1153.fn(_t1153.env, _t1154, 3);
            gem_pop_frame();
            return _t1155;
        } else {
#line 226 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 227 "compiler/main.gem"
    GemVal _t1156 = gem_v_node;
                GemVal gem_v_name = gem_table_get(_t1156, gem_string("name"));
#line 228 "compiler/main.gem"
    GemVal _t1157[] = {gem_v_defined, gem_v_name};
    GemVal _t1159;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1157, 2)))) {
        _t1159 = gem_not(gem_fn_set_contains(NULL, _t1157, 2));
    } else {
        GemVal _t1158[] = {(*gem_v_builtins), gem_v_name};
        _t1159 = gem_not(gem_fn_set_contains(NULL, _t1158, 2));
    }
    GemVal _t1161;
    if (!gem_truthy(_t1159)) {
        _t1161 = _t1159;
    } else {
        GemVal _t1160[] = {(*gem_v_defined_fns), gem_v_name};
        _t1161 = gem_not(gem_fn_set_contains(NULL, _t1160, 2));
    }
                if (gem_truthy(_t1161)) {
#line 229 "compiler/main.gem"
    GemVal _t1162[] = {gem_v_free, gem_v_name};
                    (void)(gem_fn_set_add(NULL, _t1162, 2));
                }
#line 231 "compiler/main.gem"
    GemVal _t1163 = gem_v_node;
    GemVal _t1164 = (*gem_v_collect_free_node);
    GemVal _t1165[] = {gem_table_get(_t1163, gem_string("value")), gem_v_defined, gem_v_free};
                GemVal _t1166 = _t1164.fn(_t1164.env, _t1165, 3);
                gem_pop_frame();
                return _t1166;
            } else {
#line 232 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 233 "compiler/main.gem"
    GemVal _t1167 = gem_v_node;
    GemVal _t1168[] = {gem_table_get(_t1167, gem_string("params"))};
    GemVal _t1169[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t1168, 1)};
                    GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t1169, 2);
#line 234 "compiler/main.gem"
    GemVal _t1170 = gem_v_node;
                    if (gem_truthy(gem_neq(gem_table_get(_t1170, gem_string("rest_param")), GEM_NIL))) {
#line 235 "compiler/main.gem"
    GemVal _t1171 = gem_v_node;
    GemVal _t1172[] = {gem_v_inner_defined, gem_table_get(_t1171, gem_string("rest_param"))};
                        (void)(gem_fn_set_add(NULL, _t1172, 2));
                    }
#line 237 "compiler/main.gem"
    GemVal _t1173 = gem_v_node;
                    if (gem_truthy(gem_neq(gem_table_get(_t1173, gem_string("block_param")), GEM_NIL))) {
#line 238 "compiler/main.gem"
    GemVal _t1174 = gem_v_node;
    GemVal _t1175[] = {gem_v_inner_defined, gem_table_get(_t1174, gem_string("block_param"))};
                        (void)(gem_fn_set_add(NULL, _t1175, 2));
                    }
#line 240 "compiler/main.gem"
    GemVal _t1176 = gem_v_node;
    GemVal _t1177 = (*gem_v_collect_free_vars);
    GemVal _t1178[] = {gem_table_get(_t1176, gem_string("body")), gem_v_inner_defined};
                    GemVal gem_v_inner_free = _t1177.fn(_t1177.env, _t1178, 2);
#line 241 "compiler/main.gem"
    GemVal _t1179[] = {gem_v_inner_free};
                    GemVal gem_v_ifks = gem_keys_fn(NULL, _t1179, 1);
#line 242 "compiler/main.gem"
                    GemVal gem_v_ifi = gem_int(0);
#line 243 "compiler/main.gem"
                    while (1) {
                        GemVal _t1180[] = {gem_v_ifks};
                        if (!gem_truthy(gem_lt(gem_v_ifi, gem_len_fn(NULL, _t1180, 1)))) break;
#line 244 "compiler/main.gem"
    GemVal _t1181[] = {gem_v_free, gem_table_get(gem_v_ifks, gem_v_ifi)};
                        (void)(gem_fn_set_add(NULL, _t1181, 2));
#line 245 "compiler/main.gem"
                        gem_v_ifi = gem_add(gem_v_ifi, gem_int(1));
                    }
                    gem_pop_frame();
                    return GEM_NIL;
                } else {
#line 247 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                        gem_pop_frame();
                        return GEM_NIL;
                    } else {
#line 249 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 250 "compiler/main.gem"
    GemVal _t1182 = gem_v_node;
    GemVal _t1183 = (*gem_v_collect_free_node);
    GemVal _t1184[] = {gem_table_get(_t1182, gem_string("cond")), gem_v_defined, gem_v_free};
                            (void)(_t1183.fn(_t1183.env, _t1184, 3));
#line 251 "compiler/main.gem"
    GemVal _t1185 = gem_table_new();
    GemVal _t1186[] = {gem_v_defined, _t1185};
                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1186, 2);
#line 252 "compiler/main.gem"
    GemVal _t1187 = gem_v_node;
                            GemVal gem_v_thens = gem_table_get(_t1187, gem_string("then"));
#line 253 "compiler/main.gem"
                            GemVal gem_v__for_i_8 = gem_int(0);
#line 253 "compiler/main.gem"
    GemVal _t1188[] = {gem_v_thens};
                            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1188, 1);
#line 253 "compiler/main.gem"
                            while (1) {
                                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 253 "compiler/main.gem"
                                GemVal gem_v_i = gem_v__for_i_8;
#line 253 "compiler/main.gem"
                                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 254 "compiler/main.gem"
    GemVal _t1189 = (*gem_v_collect_free_node);
    GemVal _t1190[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
                                (void)(_t1189.fn(_t1189.env, _t1190, 3));
#line 255 "compiler/main.gem"
    GemVal _t1191[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t1193;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1191, 1), gem_string("table")))) {
        _t1193 = gem_eq(gem_type_fn(NULL, _t1191, 1), gem_string("table"));
    } else {
        GemVal _t1192 = gem_table_get(gem_v_thens, gem_v_i);
        _t1193 = gem_eq(gem_table_get(_t1192, gem_string("tag")), gem_string("let"));
    }
                                if (gem_truthy(_t1193)) {
#line 256 "compiler/main.gem"
    GemVal _t1194 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1195[] = {gem_v_d, gem_table_get(_t1194, gem_string("name"))};
                                    (void)(gem_fn_set_add(NULL, _t1195, 2));
                                }
                            }

#line 259 "compiler/main.gem"
    GemVal _t1196 = gem_v_node;
                            GemVal gem_v_el = gem_table_get(_t1196, gem_string("else"));
#line 260 "compiler/main.gem"
                            if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 261 "compiler/main.gem"
    GemVal _t1197 = gem_table_new();
    GemVal _t1198[] = {gem_v_defined, _t1197};
                                gem_v_d = gem_fn_set_union(NULL, _t1198, 2);
                                {
#line 262 "compiler/main.gem"
                                    GemVal gem_v__for_i_9 = gem_int(0);
#line 262 "compiler/main.gem"
    GemVal _t1199[] = {gem_v_el};
                                    GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1199, 1);
#line 262 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 262 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_9;
#line 262 "compiler/main.gem"
                                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 263 "compiler/main.gem"
    GemVal _t1200 = (*gem_v_collect_free_node);
    GemVal _t1201[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t1200.fn(_t1200.env, _t1201, 3));
#line 264 "compiler/main.gem"
    GemVal _t1202[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t1204;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1202, 1), gem_string("table")))) {
        _t1204 = gem_eq(gem_type_fn(NULL, _t1202, 1), gem_string("table"));
    } else {
        GemVal _t1203 = gem_table_get(gem_v_el, gem_v_i);
        _t1204 = gem_eq(gem_table_get(_t1203, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t1204)) {
#line 265 "compiler/main.gem"
    GemVal _t1205 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1206[] = {gem_v_d, gem_table_get(_t1205, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t1206, 2));
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
#line 269 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 270 "compiler/main.gem"
    GemVal _t1207 = gem_v_node;
    GemVal _t1208 = (*gem_v_collect_free_node);
    GemVal _t1209[] = {gem_table_get(_t1207, gem_string("cond")), gem_v_defined, gem_v_free};
                                (void)(_t1208.fn(_t1208.env, _t1209, 3));
#line 271 "compiler/main.gem"
    GemVal _t1210 = gem_table_new();
    GemVal _t1211[] = {gem_v_defined, _t1210};
                                GemVal gem_v_d = gem_fn_set_union(NULL, _t1211, 2);
                                {
#line 272 "compiler/main.gem"
                                    GemVal gem_v__for_i_10 = gem_int(0);
#line 272 "compiler/main.gem"
    GemVal _t1212 = gem_v_node;
    GemVal _t1213[] = {gem_table_get(_t1212, gem_string("body"))};
                                    GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t1213, 1);
#line 272 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
#line 272 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_10;
#line 272 "compiler/main.gem"
                                        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 273 "compiler/main.gem"
    GemVal _t1214 = gem_v_node;
    GemVal _t1215 = (*gem_v_collect_free_node);
    GemVal _t1216[] = {gem_table_get(gem_table_get(_t1214, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t1215.fn(_t1215.env, _t1216, 3));
#line 274 "compiler/main.gem"
    GemVal _t1217 = gem_v_node;
    GemVal _t1218[] = {gem_table_get(gem_table_get(_t1217, gem_string("body")), gem_v_i)};
    GemVal _t1221;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1218, 1), gem_string("table")))) {
        _t1221 = gem_eq(gem_type_fn(NULL, _t1218, 1), gem_string("table"));
    } else {
        GemVal _t1219 = gem_v_node;
        GemVal _t1220 = gem_table_get(gem_table_get(_t1219, gem_string("body")), gem_v_i);
        _t1221 = gem_eq(gem_table_get(_t1220, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t1221)) {
#line 275 "compiler/main.gem"
    GemVal _t1222 = gem_v_node;
    GemVal _t1223 = gem_table_get(gem_table_get(_t1222, gem_string("body")), gem_v_i);
    GemVal _t1224[] = {gem_v_d, gem_table_get(_t1223, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t1224, 2));
                                        }
                                    }
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 278 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 279 "compiler/main.gem"
    GemVal _t1225 = gem_v_node;
    GemVal _t1226 = (*gem_v_collect_free_node);
    GemVal _t1227[] = {gem_table_get(_t1225, gem_string("target")), gem_v_defined, gem_v_free};
                                    (void)(_t1226.fn(_t1226.env, _t1227, 3));
#line 280 "compiler/main.gem"
                                    GemVal gem_v__for_i_12 = gem_int(0);
#line 280 "compiler/main.gem"
    GemVal _t1228 = gem_v_node;
    GemVal _t1229[] = {gem_table_get(_t1228, gem_string("whens"))};
                                    GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t1229, 1);
#line 280 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 280 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_12;
#line 280 "compiler/main.gem"
                                        gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 281 "compiler/main.gem"
    GemVal _t1230 = gem_v_node;
                                        GemVal gem_v_w = gem_table_get(gem_table_get(_t1230, gem_string("whens")), gem_v_i);
#line 282 "compiler/main.gem"
    GemVal _t1231 = gem_v_w;
    GemVal _t1232 = (*gem_v_collect_free_node);
    GemVal _t1233[] = {gem_table_get(_t1231, gem_string("value")), gem_v_defined, gem_v_free};
                                        (void)(_t1232.fn(_t1232.env, _t1233, 3));
#line 283 "compiler/main.gem"
    GemVal _t1234 = gem_table_new();
    GemVal _t1235[] = {gem_v_defined, _t1234};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1235, 2);
#line 284 "compiler/main.gem"
                                        GemVal gem_v__for_i_11 = gem_int(0);
#line 284 "compiler/main.gem"
    GemVal _t1236 = gem_v_w;
    GemVal _t1237[] = {gem_table_get(_t1236, gem_string("body"))};
                                        GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t1237, 1);
#line 284 "compiler/main.gem"
                                        while (1) {
                                            if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
#line 284 "compiler/main.gem"
                                            GemVal gem_v_j = gem_v__for_i_11;
#line 284 "compiler/main.gem"
                                            gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 285 "compiler/main.gem"
    GemVal _t1238 = gem_v_w;
    GemVal _t1239 = (*gem_v_collect_free_node);
    GemVal _t1240[] = {gem_table_get(gem_table_get(_t1238, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
                                            (void)(_t1239.fn(_t1239.env, _t1240, 3));
#line 286 "compiler/main.gem"
    GemVal _t1241 = gem_v_w;
    GemVal _t1242[] = {gem_table_get(gem_table_get(_t1241, gem_string("body")), gem_v_j)};
    GemVal _t1245;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1242, 1), gem_string("table")))) {
        _t1245 = gem_eq(gem_type_fn(NULL, _t1242, 1), gem_string("table"));
    } else {
        GemVal _t1243 = gem_v_w;
        GemVal _t1244 = gem_table_get(gem_table_get(_t1243, gem_string("body")), gem_v_j);
        _t1245 = gem_eq(gem_table_get(_t1244, gem_string("tag")), gem_string("let"));
    }
                                            if (gem_truthy(_t1245)) {
#line 287 "compiler/main.gem"
    GemVal _t1246 = gem_v_w;
    GemVal _t1247 = gem_table_get(gem_table_get(_t1246, gem_string("body")), gem_v_j);
    GemVal _t1248[] = {gem_v_d, gem_table_get(_t1247, gem_string("name"))};
                                                (void)(gem_fn_set_add(NULL, _t1248, 2));
                                            }
                                        }

                                    }

#line 291 "compiler/main.gem"
    GemVal _t1249 = gem_v_node;
                                    GemVal gem_v_el = gem_table_get(_t1249, gem_string("else"));
#line 292 "compiler/main.gem"
                                    if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 293 "compiler/main.gem"
    GemVal _t1250 = gem_table_new();
    GemVal _t1251[] = {gem_v_defined, _t1250};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1251, 2);
                                        {
#line 294 "compiler/main.gem"
                                            GemVal gem_v__for_i_13 = gem_int(0);
#line 294 "compiler/main.gem"
    GemVal _t1252[] = {gem_v_el};
                                            GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1252, 1);
#line 294 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 294 "compiler/main.gem"
                                                GemVal gem_v_j = gem_v__for_i_13;
#line 294 "compiler/main.gem"
                                                gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 295 "compiler/main.gem"
    GemVal _t1253 = (*gem_v_collect_free_node);
    GemVal _t1254[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
                                                (void)(_t1253.fn(_t1253.env, _t1254, 3));
#line 296 "compiler/main.gem"
    GemVal _t1255[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t1257;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1255, 1), gem_string("table")))) {
        _t1257 = gem_eq(gem_type_fn(NULL, _t1255, 1), gem_string("table"));
    } else {
        GemVal _t1256 = gem_table_get(gem_v_el, gem_v_j);
        _t1257 = gem_eq(gem_table_get(_t1256, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1257)) {
#line 297 "compiler/main.gem"
    GemVal _t1258 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t1259[] = {gem_v_d, gem_table_get(_t1258, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1259, 2));
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
#line 301 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("when_clause")))) {
#line 302 "compiler/main.gem"
    GemVal _t1260 = gem_v_node;
    GemVal _t1261 = (*gem_v_collect_free_node);
    GemVal _t1262[] = {gem_table_get(_t1260, gem_string("value")), gem_v_defined, gem_v_free};
                                        (void)(_t1261.fn(_t1261.env, _t1262, 3));
#line 303 "compiler/main.gem"
    GemVal _t1263 = gem_table_new();
    GemVal _t1264[] = {gem_v_defined, _t1263};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1264, 2);
                                        {
#line 304 "compiler/main.gem"
                                            GemVal gem_v__for_i_14 = gem_int(0);
#line 304 "compiler/main.gem"
    GemVal _t1265 = gem_v_node;
    GemVal _t1266[] = {gem_table_get(_t1265, gem_string("body"))};
                                            GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1266, 1);
#line 304 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 304 "compiler/main.gem"
                                                GemVal gem_v_i = gem_v__for_i_14;
#line 304 "compiler/main.gem"
                                                gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 305 "compiler/main.gem"
    GemVal _t1267 = gem_v_node;
    GemVal _t1268 = (*gem_v_collect_free_node);
    GemVal _t1269[] = {gem_table_get(gem_table_get(_t1267, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                                (void)(_t1268.fn(_t1268.env, _t1269, 3));
#line 306 "compiler/main.gem"
    GemVal _t1270 = gem_v_node;
    GemVal _t1271[] = {gem_table_get(gem_table_get(_t1270, gem_string("body")), gem_v_i)};
    GemVal _t1274;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1271, 1), gem_string("table")))) {
        _t1274 = gem_eq(gem_type_fn(NULL, _t1271, 1), gem_string("table"));
    } else {
        GemVal _t1272 = gem_v_node;
        GemVal _t1273 = gem_table_get(gem_table_get(_t1272, gem_string("body")), gem_v_i);
        _t1274 = gem_eq(gem_table_get(_t1273, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1274)) {
#line 307 "compiler/main.gem"
    GemVal _t1275 = gem_v_node;
    GemVal _t1276 = gem_table_get(gem_table_get(_t1275, gem_string("body")), gem_v_i);
    GemVal _t1277[] = {gem_v_d, gem_table_get(_t1276, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1277, 2));
                                                }
                                            }
                                            gem_pop_frame();
                                            return GEM_NIL;
                                        }
                                    } else {
#line 310 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 311 "compiler/main.gem"
    GemVal _t1278 = gem_table_new();
    GemVal _t1279[] = {gem_v_defined, _t1278};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1279, 2);
                                            {
#line 312 "compiler/main.gem"
                                                GemVal gem_v__for_i_15 = gem_int(0);
#line 312 "compiler/main.gem"
    GemVal _t1280 = gem_v_node;
    GemVal _t1281[] = {gem_table_get(_t1280, gem_string("stmts"))};
                                                GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1281, 1);
#line 312 "compiler/main.gem"
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 312 "compiler/main.gem"
                                                    GemVal gem_v_i = gem_v__for_i_15;
#line 312 "compiler/main.gem"
                                                    gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 313 "compiler/main.gem"
    GemVal _t1282 = gem_v_node;
    GemVal _t1283 = (*gem_v_collect_free_node);
    GemVal _t1284[] = {gem_table_get(gem_table_get(_t1282, gem_string("stmts")), gem_v_i), gem_v_d, gem_v_free};
                                                    (void)(_t1283.fn(_t1283.env, _t1284, 3));
#line 314 "compiler/main.gem"
    GemVal _t1285 = gem_v_node;
    GemVal _t1286[] = {gem_table_get(gem_table_get(_t1285, gem_string("stmts")), gem_v_i)};
    GemVal _t1289;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1286, 1), gem_string("table")))) {
        _t1289 = gem_eq(gem_type_fn(NULL, _t1286, 1), gem_string("table"));
    } else {
        GemVal _t1287 = gem_v_node;
        GemVal _t1288 = gem_table_get(gem_table_get(_t1287, gem_string("stmts")), gem_v_i);
        _t1289 = gem_eq(gem_table_get(_t1288, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t1289)) {
#line 315 "compiler/main.gem"
    GemVal _t1290 = gem_v_node;
    GemVal _t1291 = gem_table_get(gem_table_get(_t1290, gem_string("stmts")), gem_v_i);
    GemVal _t1292[] = {gem_v_d, gem_table_get(_t1291, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t1292, 2));
                                                    }
                                                }
                                                gem_pop_frame();
                                                return GEM_NIL;
                                            }
                                        } else {
#line 318 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 319 "compiler/main.gem"
    GemVal _t1293 = gem_v_node;
                                                if (gem_truthy(gem_neq(gem_table_get(_t1293, gem_string("value")), GEM_NIL))) {
#line 320 "compiler/main.gem"
    GemVal _t1294 = gem_v_node;
    GemVal _t1295 = (*gem_v_collect_free_node);
    GemVal _t1296[] = {gem_table_get(_t1294, gem_string("value")), gem_v_defined, gem_v_free};
                                                    GemVal _t1297 = _t1295.fn(_t1295.env, _t1296, 3);
                                                    gem_pop_frame();
                                                    return _t1297;
                                                } else {
                                                    gem_pop_frame();
                                                    return GEM_NIL;
                                                }
                                            } else {
                                                {
#line 324 "compiler/main.gem"
                                                    GemVal gem_v__for_tbl_16 = gem_v_node;
#line 324 "compiler/main.gem"
    GemVal _t1298[] = {gem_v__for_tbl_16};
                                                    GemVal gem_v__for_keys_16 = gem_keys_fn(NULL, _t1298, 1);
#line 324 "compiler/main.gem"
                                                    GemVal gem_v__for_i_16 = gem_int(0);
#line 324 "compiler/main.gem"
                                                    while (1) {
                                                        GemVal _t1299[] = {gem_v__for_keys_16};
                                                        if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_len_fn(NULL, _t1299, 1)))) break;
#line 324 "compiler/main.gem"
                                                        GemVal gem_v_nk = gem_table_get(gem_v__for_keys_16, gem_v__for_i_16);
#line 324 "compiler/main.gem"
                                                        GemVal gem_v_v = gem_table_get(gem_v__for_tbl_16, gem_table_get(gem_v__for_keys_16, gem_v__for_i_16));
#line 324 "compiler/main.gem"
                                                        gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 325 "compiler/main.gem"
                                                        if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 326 "compiler/main.gem"
    GemVal _t1300[] = {gem_v_v};
                                                            if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1300, 1), gem_string("table")))) {
#line 328 "compiler/main.gem"
    GemVal _t1301[] = {gem_v_v};
                                                                GemVal gem_v_vks = gem_keys_fn(NULL, _t1301, 1);
#line 329 "compiler/main.gem"
    GemVal _t1302[] = {gem_v_vks};
    GemVal _t1304;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1302, 1), gem_int(0)))) {
        _t1304 = gem_gt(gem_len_fn(NULL, _t1302, 1), gem_int(0));
    } else {
        GemVal _t1303[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1304 = gem_eq(gem_type_fn(NULL, _t1303, 1), gem_string("int"));
    }
                                                                if (gem_truthy(_t1304)) {
#line 331 "compiler/main.gem"
                                                                    GemVal gem_v_vi = gem_int(0);
#line 332 "compiler/main.gem"
                                                                    while (1) {
                                                                        GemVal _t1305[] = {gem_v_v};
                                                                        if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1305, 1)))) break;
#line 333 "compiler/main.gem"
    GemVal _t1306[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                        if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1306, 1), gem_string("table")))) {
#line 334 "compiler/main.gem"
    GemVal _t1307 = (*gem_v_collect_free_node);
    GemVal _t1308[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
                                                                            (void)(_t1307.fn(_t1307.env, _t1308, 3));
                                                                        }
#line 336 "compiler/main.gem"
                                                                        gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                                    }
                                                                } else {
#line 339 "compiler/main.gem"
    GemVal _t1309 = (*gem_v_collect_free_node);
    GemVal _t1310[] = {gem_v_v, gem_v_defined, gem_v_free};
                                                                    (void)(_t1309.fn(_t1309.env, _t1310, 3));
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
#line 348 "compiler/main.gem"
    GemVal _t1312 = gem_table_new();
    GemVal gem_v_free = _t1312;
#line 349 "compiler/main.gem"
    GemVal _t1313 = gem_table_new();
    GemVal _t1314[] = {gem_v_defined, _t1313};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t1314, 2);
#line 350 "compiler/main.gem"
    GemVal gem_v__for_i_18 = gem_int(0);
#line 350 "compiler/main.gem"
    GemVal _t1315[] = {gem_v_stmts};
    GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1315, 1);
#line 350 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 350 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_18;
#line 350 "compiler/main.gem"
        gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 351 "compiler/main.gem"
    GemVal _t1316 = (*gem_v_collect_free_node);
    GemVal _t1317[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
        (void)(_t1316.fn(_t1316.env, _t1317, 3));
#line 352 "compiler/main.gem"
    GemVal _t1318[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1320;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1318, 1), gem_string("table")))) {
        _t1320 = gem_eq(gem_type_fn(NULL, _t1318, 1), gem_string("table"));
    } else {
        GemVal _t1319 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1320 = gem_eq(gem_table_get(_t1319, gem_string("tag")), gem_string("let"));
    }
        if (gem_truthy(_t1320)) {
#line 353 "compiler/main.gem"
    GemVal _t1321 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1322[] = {gem_v_d, gem_table_get(_t1321, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1322, 2));
        }
#line 355 "compiler/main.gem"
    GemVal _t1323[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1325;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1323, 1), gem_string("table")))) {
        _t1325 = gem_eq(gem_type_fn(NULL, _t1323, 1), gem_string("table"));
    } else {
        GemVal _t1324 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1325 = gem_eq(gem_table_get(_t1324, gem_string("tag")), gem_string("block"));
    }
        if (gem_truthy(_t1325)) {
#line 356 "compiler/main.gem"
            GemVal gem_v__for_i_17 = gem_int(0);
#line 356 "compiler/main.gem"
    GemVal _t1326 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1327[] = {gem_table_get(_t1326, gem_string("stmts"))};
            GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1327, 1);
#line 356 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 356 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_17;
#line 356 "compiler/main.gem"
                gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 357 "compiler/main.gem"
    GemVal _t1328 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1329[] = {gem_table_get(gem_table_get(_t1328, gem_string("stmts")), gem_v_j)};
    GemVal _t1332;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1329, 1), gem_string("table")))) {
        _t1332 = gem_eq(gem_type_fn(NULL, _t1329, 1), gem_string("table"));
    } else {
        GemVal _t1330 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1331 = gem_table_get(gem_table_get(_t1330, gem_string("stmts")), gem_v_j);
        _t1332 = gem_eq(gem_table_get(_t1331, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1332)) {
#line 358 "compiler/main.gem"
    GemVal _t1333 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1334 = gem_table_get(gem_table_get(_t1333, gem_string("stmts")), gem_v_j);
    GemVal _t1335[] = {gem_v_d, gem_table_get(_t1334, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1335, 2));
                }
            }

        }
    }

    GemVal _t1336 = gem_v_free;
    gem_pop_frame();
    return _t1336;
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
#line 372 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 373 "compiler/main.gem"
        GemVal _t1338 = GEM_NIL;
        gem_pop_frame();
        return _t1338;
    }
#line 375 "compiler/main.gem"
    GemVal _t1339[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1339, 1), gem_string("table")))) {
#line 376 "compiler/main.gem"
        GemVal _t1340 = GEM_NIL;
        gem_pop_frame();
        return _t1340;
    }
#line 378 "compiler/main.gem"
    GemVal _t1341 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1341, gem_string("tag"));
#line 379 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 380 "compiler/main.gem"
        GemVal _t1342 = GEM_NIL;
        gem_pop_frame();
        return _t1342;
    }
#line 383 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 384 "compiler/main.gem"
    GemVal _t1343 = gem_v_node;
    GemVal _t1344[] = {gem_table_get(_t1343, gem_string("params"))};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1344, 1);
#line 385 "compiler/main.gem"
    GemVal _t1345 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t1345, gem_string("rest_param")), GEM_NIL))) {
#line 386 "compiler/main.gem"
    GemVal _t1346 = gem_v_node;
    GemVal _t1347[] = {gem_v_inner_defined, gem_table_get(_t1346, gem_string("rest_param"))};
            (void)(gem_fn_set_add(NULL, _t1347, 2));
        }
#line 388 "compiler/main.gem"
    GemVal _t1348 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t1348, gem_string("block_param")), GEM_NIL))) {
#line 389 "compiler/main.gem"
    GemVal _t1349 = gem_v_node;
    GemVal _t1350[] = {gem_v_inner_defined, gem_table_get(_t1349, gem_string("block_param"))};
            (void)(gem_fn_set_add(NULL, _t1350, 2));
        }
#line 391 "compiler/main.gem"
    GemVal _t1351 = gem_v_node;
    GemVal _t1352 = (*gem_v_collect_free_vars);
    GemVal _t1353[] = {gem_table_get(_t1351, gem_string("body")), gem_v_inner_defined};
        GemVal gem_v_free = _t1352.fn(_t1352.env, _t1353, 2);
#line 392 "compiler/main.gem"
    GemVal _t1354[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t1354, 2);
#line 393 "compiler/main.gem"
    GemVal _t1355[] = {gem_v_inter};
        GemVal gem_v_iks = gem_keys_fn(NULL, _t1355, 1);
#line 394 "compiler/main.gem"
        GemVal gem_v_iki = gem_int(0);
#line 395 "compiler/main.gem"
        while (1) {
            GemVal _t1356[] = {gem_v_iks};
            if (!gem_truthy(gem_lt(gem_v_iki, gem_len_fn(NULL, _t1356, 1)))) break;
#line 396 "compiler/main.gem"
    GemVal _t1357[] = {gem_v_captured, gem_table_get(gem_v_iks, gem_v_iki)};
            (void)(gem_fn_set_add(NULL, _t1357, 2));
#line 397 "compiler/main.gem"
            gem_v_iki = gem_add(gem_v_iki, gem_int(1));
        }
#line 399 "compiler/main.gem"
    GemVal _t1358 = gem_v_node;
    GemVal _t1359[] = {gem_v_scope_vars, gem_v_inner_defined};
    GemVal _t1360 = (*gem_v_walk_captures);
    GemVal _t1361[] = {gem_table_get(_t1358, gem_string("body")), gem_fn_set_union(NULL, _t1359, 2), gem_v_captured};
        GemVal _t1362 = _t1360.fn(_t1360.env, _t1361, 3);
        gem_pop_frame();
        return _t1362;
    } else {
#line 400 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
            gem_pop_frame();
            return GEM_NIL;
        } else {
#line 402 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 403 "compiler/main.gem"
    GemVal _t1363 = gem_v_node;
    GemVal _t1364 = (*gem_v_walk_captures_node);
    GemVal _t1365[] = {gem_table_get(_t1363, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1364.fn(_t1364.env, _t1365, 3));
#line 404 "compiler/main.gem"
    GemVal _t1366 = gem_v_node;
    GemVal _t1367 = (*gem_v_walk_captures);
    GemVal _t1368[] = {gem_table_get(_t1366, gem_string("then")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1367.fn(_t1367.env, _t1368, 3));
#line 405 "compiler/main.gem"
    GemVal _t1369 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t1369, gem_string("else")), GEM_NIL))) {
#line 406 "compiler/main.gem"
    GemVal _t1370 = gem_v_node;
    GemVal _t1371 = (*gem_v_walk_captures);
    GemVal _t1372[] = {gem_table_get(_t1370, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                    GemVal _t1373 = _t1371.fn(_t1371.env, _t1372, 3);
                    gem_pop_frame();
                    return _t1373;
                } else {
                    gem_pop_frame();
                    return GEM_NIL;
                }
            } else {
#line 408 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 409 "compiler/main.gem"
    GemVal _t1374 = gem_v_node;
    GemVal _t1375 = (*gem_v_walk_captures_node);
    GemVal _t1376[] = {gem_table_get(_t1374, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                    (void)(_t1375.fn(_t1375.env, _t1376, 3));
#line 410 "compiler/main.gem"
    GemVal _t1377 = gem_v_node;
    GemVal _t1378 = (*gem_v_walk_captures);
    GemVal _t1379[] = {gem_table_get(_t1377, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                    GemVal _t1380 = _t1378.fn(_t1378.env, _t1379, 3);
                    gem_pop_frame();
                    return _t1380;
                } else {
#line 411 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 412 "compiler/main.gem"
    GemVal _t1381 = gem_v_node;
    GemVal _t1382 = (*gem_v_walk_captures_node);
    GemVal _t1383[] = {gem_table_get(_t1381, gem_string("target")), gem_v_scope_vars, gem_v_captured};
                        (void)(_t1382.fn(_t1382.env, _t1383, 3));
#line 413 "compiler/main.gem"
                        GemVal gem_v__for_i_19 = gem_int(0);
#line 413 "compiler/main.gem"
    GemVal _t1384 = gem_v_node;
    GemVal _t1385[] = {gem_table_get(_t1384, gem_string("whens"))};
                        GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1385, 1);
#line 413 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 413 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_19;
#line 413 "compiler/main.gem"
                            gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 414 "compiler/main.gem"
    GemVal _t1386 = gem_v_node;
    GemVal _t1387 = gem_table_get(gem_table_get(_t1386, gem_string("whens")), gem_v_i);
    GemVal _t1388 = (*gem_v_walk_captures_node);
    GemVal _t1389[] = {gem_table_get(_t1387, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                            (void)(_t1388.fn(_t1388.env, _t1389, 3));
#line 415 "compiler/main.gem"
    GemVal _t1390 = gem_v_node;
    GemVal _t1391 = gem_table_get(gem_table_get(_t1390, gem_string("whens")), gem_v_i);
    GemVal _t1392 = (*gem_v_walk_captures);
    GemVal _t1393[] = {gem_table_get(_t1391, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                            (void)(_t1392.fn(_t1392.env, _t1393, 3));
                        }

#line 417 "compiler/main.gem"
    GemVal _t1394 = gem_v_node;
                        if (gem_truthy(gem_neq(gem_table_get(_t1394, gem_string("else")), GEM_NIL))) {
#line 418 "compiler/main.gem"
    GemVal _t1395 = gem_v_node;
    GemVal _t1396 = (*gem_v_walk_captures);
    GemVal _t1397[] = {gem_table_get(_t1395, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                            GemVal _t1398 = _t1396.fn(_t1396.env, _t1397, 3);
                            gem_pop_frame();
                            return _t1398;
                        } else {
                            gem_pop_frame();
                            return GEM_NIL;
                        }
                    } else {
#line 420 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 421 "compiler/main.gem"
    GemVal _t1399 = gem_v_node;
    GemVal _t1400 = (*gem_v_walk_captures);
    GemVal _t1401[] = {gem_table_get(_t1399, gem_string("stmts")), gem_v_scope_vars, gem_v_captured};
                            GemVal _t1402 = _t1400.fn(_t1400.env, _t1401, 3);
                            gem_pop_frame();
                            return _t1402;
                        } else {
#line 422 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 423 "compiler/main.gem"
    GemVal _t1403 = gem_v_node;
                                if (gem_truthy(gem_neq(gem_table_get(_t1403, gem_string("value")), GEM_NIL))) {
#line 424 "compiler/main.gem"
    GemVal _t1404 = gem_v_node;
    GemVal _t1405 = (*gem_v_walk_captures_node);
    GemVal _t1406[] = {gem_table_get(_t1404, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    GemVal _t1407 = _t1405.fn(_t1405.env, _t1406, 3);
                                    gem_pop_frame();
                                    return _t1407;
                                } else {
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 426 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 427 "compiler/main.gem"
    GemVal _t1408 = gem_v_node;
    GemVal _t1409 = (*gem_v_walk_captures_node);
    GemVal _t1410[] = {gem_table_get(_t1408, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    GemVal _t1411 = _t1409.fn(_t1409.env, _t1410, 3);
                                    gem_pop_frame();
                                    return _t1411;
                                } else {
#line 428 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 429 "compiler/main.gem"
    GemVal _t1412 = gem_v_node;
    GemVal _t1413 = (*gem_v_walk_captures_node);
    GemVal _t1414[] = {gem_table_get(_t1412, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                        GemVal _t1415 = _t1413.fn(_t1413.env, _t1414, 3);
                                        gem_pop_frame();
                                        return _t1415;
                                    } else {
                                        {
#line 432 "compiler/main.gem"
                                            GemVal gem_v__for_tbl_20 = gem_v_node;
#line 432 "compiler/main.gem"
    GemVal _t1416[] = {gem_v__for_tbl_20};
                                            GemVal gem_v__for_keys_20 = gem_keys_fn(NULL, _t1416, 1);
#line 432 "compiler/main.gem"
                                            GemVal gem_v__for_i_20 = gem_int(0);
#line 432 "compiler/main.gem"
                                            while (1) {
                                                GemVal _t1417[] = {gem_v__for_keys_20};
                                                if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_len_fn(NULL, _t1417, 1)))) break;
#line 432 "compiler/main.gem"
                                                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_20, gem_v__for_i_20);
#line 432 "compiler/main.gem"
                                                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_20, gem_table_get(gem_v__for_keys_20, gem_v__for_i_20));
#line 432 "compiler/main.gem"
                                                gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 433 "compiler/main.gem"
                                                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 434 "compiler/main.gem"
    GemVal _t1418[] = {gem_v_v};
                                                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1418, 1), gem_string("table")))) {
#line 435 "compiler/main.gem"
    GemVal _t1419[] = {gem_v_v};
                                                        GemVal gem_v_vks = gem_keys_fn(NULL, _t1419, 1);
#line 436 "compiler/main.gem"
    GemVal _t1420[] = {gem_v_vks};
    GemVal _t1422;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1420, 1), gem_int(0)))) {
        _t1422 = gem_gt(gem_len_fn(NULL, _t1420, 1), gem_int(0));
    } else {
        GemVal _t1421[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1422 = gem_eq(gem_type_fn(NULL, _t1421, 1), gem_string("int"));
    }
                                                        if (gem_truthy(_t1422)) {
#line 437 "compiler/main.gem"
                                                            GemVal gem_v_vi = gem_int(0);
#line 438 "compiler/main.gem"
                                                            while (1) {
                                                                GemVal _t1423[] = {gem_v_v};
                                                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1423, 1)))) break;
#line 439 "compiler/main.gem"
    GemVal _t1424[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1424, 1), gem_string("table")))) {
#line 440 "compiler/main.gem"
    GemVal _t1425 = (*gem_v_walk_captures_node);
    GemVal _t1426[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
                                                                    (void)(_t1425.fn(_t1425.env, _t1426, 3));
                                                                }
#line 442 "compiler/main.gem"
                                                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                            }
                                                        } else {
#line 445 "compiler/main.gem"
    GemVal _t1427 = (*gem_v_walk_captures_node);
    GemVal _t1428[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
                                                            (void)(_t1427.fn(_t1427.env, _t1428, 3));
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
#line 454 "compiler/main.gem"
    GemVal _t1430 = gem_table_new();
    GemVal _t1431[] = {gem_v_scope_vars, _t1430};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t1431, 2);
    {
#line 455 "compiler/main.gem"
        GemVal gem_v__for_i_22 = gem_int(0);
#line 455 "compiler/main.gem"
    GemVal _t1432[] = {gem_v_stmts};
        GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1432, 1);
#line 455 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 455 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_22;
#line 455 "compiler/main.gem"
            gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 456 "compiler/main.gem"
    GemVal _t1433 = (*gem_v_walk_captures_node);
    GemVal _t1434[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
            (void)(_t1433.fn(_t1433.env, _t1434, 3));
#line 457 "compiler/main.gem"
    GemVal _t1435[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1437;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1435, 1), gem_string("table")))) {
        _t1437 = gem_eq(gem_type_fn(NULL, _t1435, 1), gem_string("table"));
    } else {
        GemVal _t1436 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1437 = gem_eq(gem_table_get(_t1436, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1437)) {
#line 458 "compiler/main.gem"
    GemVal _t1438 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1439[] = {gem_v_sv, gem_table_get(_t1438, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1439, 2));
            }
#line 460 "compiler/main.gem"
    GemVal _t1440[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1442;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1440, 1), gem_string("table")))) {
        _t1442 = gem_eq(gem_type_fn(NULL, _t1440, 1), gem_string("table"));
    } else {
        GemVal _t1441 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1442 = gem_eq(gem_table_get(_t1441, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1442)) {
#line 461 "compiler/main.gem"
                GemVal gem_v__for_i_21 = gem_int(0);
#line 461 "compiler/main.gem"
    GemVal _t1443 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1444[] = {gem_table_get(_t1443, gem_string("stmts"))};
                GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t1444, 1);
#line 461 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 461 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_21;
#line 461 "compiler/main.gem"
                    gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 462 "compiler/main.gem"
    GemVal _t1445 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1446[] = {gem_table_get(gem_table_get(_t1445, gem_string("stmts")), gem_v_j)};
    GemVal _t1449;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1446, 1), gem_string("table")))) {
        _t1449 = gem_eq(gem_type_fn(NULL, _t1446, 1), gem_string("table"));
    } else {
        GemVal _t1447 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1448 = gem_table_get(gem_table_get(_t1447, gem_string("stmts")), gem_v_j);
        _t1449 = gem_eq(gem_table_get(_t1448, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1449)) {
#line 463 "compiler/main.gem"
    GemVal _t1450 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1451 = gem_table_get(gem_table_get(_t1450, gem_string("stmts")), gem_v_j);
    GemVal _t1452[] = {gem_v_sv, gem_table_get(_t1451, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1452, 2));
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
#line 471 "compiler/main.gem"
    GemVal _t1454 = gem_table_new();
    GemVal gem_v_captured = _t1454;
#line 472 "compiler/main.gem"
    GemVal _t1455 = (*gem_v_walk_captures);
    GemVal _t1456[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    (void)(_t1455.fn(_t1455.env, _t1456, 3));
    GemVal _t1457 = gem_v_captured;
    gem_pop_frame();
    return _t1457;
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
#line 499 "compiler/main.gem"
    GemVal _t1459 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1459, gem_string("tag"));
#line 501 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("int")))) {
#line 502 "compiler/main.gem"
    GemVal _t1460 = gem_table_new();
    GemVal _t1461 = gem_v_node;
    GemVal _t1462[] = {gem_table_get(_t1461, gem_string("value"))};
    gem_table_set(_t1460, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t1462, 1)), gem_string(")")));
    gem_table_set(_t1460, gem_string("setup"), gem_string(""));
        GemVal _t1463 = _t1460;
        gem_pop_frame();
        return _t1463;
    } else {
#line 503 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("float")))) {
#line 504 "compiler/main.gem"
    GemVal _t1464 = gem_table_new();
    GemVal _t1465 = gem_v_node;
    GemVal _t1466[] = {gem_table_get(_t1465, gem_string("value"))};
    GemVal _t1467[] = {gem_fn_format_float(NULL, _t1466, 1)};
    gem_table_set(_t1464, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t1467, 1)), gem_string(")")));
    gem_table_set(_t1464, gem_string("setup"), gem_string(""));
            GemVal _t1468 = _t1464;
            gem_pop_frame();
            return _t1468;
        } else {
#line 505 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("string")))) {
#line 506 "compiler/main.gem"
    GemVal _t1469 = gem_v_node;
    GemVal _t1470[] = {gem_table_get(_t1469, gem_string("value"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1470, 1);
#line 507 "compiler/main.gem"
    GemVal _t1471 = gem_table_new();
    GemVal _t1472[] = {gem_v_escaped};
    gem_table_set(_t1471, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t1472, 1)), gem_string("\")")));
    gem_table_set(_t1471, gem_string("setup"), gem_string(""));
                GemVal _t1473 = _t1471;
                gem_pop_frame();
                return _t1473;
            } else {
#line 508 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("interp")))) {
#line 509 "compiler/main.gem"
    GemVal _t1474 = gem_v_node;
                    GemVal gem_v_parts = gem_table_get(_t1474, gem_string("parts"));
#line 510 "compiler/main.gem"
    GemVal _t1475[] = {gem_v_parts};
                    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1475, 1), gem_int(0)))) {
#line 511 "compiler/main.gem"
    GemVal _t1476 = gem_table_new();
    gem_table_set(_t1476, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t1476, gem_string("setup"), gem_string(""));
                        GemVal _t1477 = _t1476;
                        gem_pop_frame();
                        return _t1477;
                    }
#line 513 "compiler/main.gem"
                    GemVal gem_v_setup = gem_string("");
#line 514 "compiler/main.gem"
    GemVal _t1478 = gem_table_new();
                    GemVal gem_v_compiled = _t1478;
#line 515 "compiler/main.gem"
                    GemVal gem_v__for_i_23 = gem_int(0);
#line 515 "compiler/main.gem"
    GemVal _t1479[] = {gem_v_parts};
                    GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t1479, 1);
#line 515 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 515 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_23;
#line 515 "compiler/main.gem"
                        gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 516 "compiler/main.gem"
                        GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 517 "compiler/main.gem"
    GemVal _t1480 = (*gem_v_compile_expr);
    GemVal _t1481[] = {gem_v_part};
                        GemVal gem_v_r = _t1480.fn(_t1480.env, _t1481, 1);
#line 518 "compiler/main.gem"
    GemVal _t1482 = gem_v_r;
                        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1482, gem_string("setup")));
#line 519 "compiler/main.gem"
    GemVal _t1483 = gem_v_part;
                        if (gem_truthy(gem_eq(gem_table_get(_t1483, gem_string("tag")), gem_string("string")))) {
#line 520 "compiler/main.gem"
    GemVal _t1484 = gem_v_r;
    GemVal _t1485[] = {gem_v_compiled, gem_table_get(_t1484, gem_string("expr"))};
                            (void)(gem_push_fn(NULL, _t1485, 2));
                        } else {
#line 522 "compiler/main.gem"
    GemVal _t1486 = (*gem_v_tmp);
                            GemVal gem_v_ts = _t1486.fn(_t1486.env, NULL, 0);
#line 523 "compiler/main.gem"
    GemVal _t1487[] = {gem_v_ts};
    GemVal _t1488 = gem_v_r;
    GemVal _t1489[] = {gem_table_get(_t1488, gem_string("expr"))};
                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1487, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1489, 1)), gem_string("};\n")));
#line 524 "compiler/main.gem"
    GemVal _t1490[] = {gem_v_ts};
    GemVal _t1491[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t1490, 1)), gem_string(", 1)"))};
                            (void)(gem_push_fn(NULL, _t1491, 2));
                        }
                    }

#line 527 "compiler/main.gem"
                    GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
#line 528 "compiler/main.gem"
                    GemVal gem_v__for_i_24 = gem_int(1);
#line 528 "compiler/main.gem"
    GemVal _t1492[] = {gem_v_compiled};
                    GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1492, 1);
#line 528 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 528 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_24;
#line 528 "compiler/main.gem"
                        gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 529 "compiler/main.gem"
    GemVal _t1493[] = {gem_v_acc};
    GemVal _t1494[] = {gem_table_get(gem_v_compiled, gem_v_i)};
                        gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1493, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1494, 1)), gem_string(")"));
                    }

#line 531 "compiler/main.gem"
    GemVal _t1495 = gem_table_new();
    gem_table_set(_t1495, gem_string("expr"), gem_v_acc);
    gem_table_set(_t1495, gem_string("setup"), gem_v_setup);
                    GemVal _t1496 = _t1495;
                    gem_pop_frame();
                    return _t1496;
                } else {
#line 532 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("bool")))) {
#line 533 "compiler/main.gem"
    GemVal _t1497 = gem_v_node;
                        if (gem_truthy(gem_table_get(_t1497, gem_string("value")))) {
#line 534 "compiler/main.gem"
    GemVal _t1498 = gem_table_new();
    gem_table_set(_t1498, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t1498, gem_string("setup"), gem_string(""));
                            GemVal _t1499 = _t1498;
                            gem_pop_frame();
                            return _t1499;
                        }
#line 536 "compiler/main.gem"
    GemVal _t1500 = gem_table_new();
    gem_table_set(_t1500, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t1500, gem_string("setup"), gem_string(""));
                        GemVal _t1501 = _t1500;
                        gem_pop_frame();
                        return _t1501;
                    } else {
#line 537 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("nil")))) {
#line 538 "compiler/main.gem"
    GemVal _t1502 = gem_table_new();
    gem_table_set(_t1502, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1502, gem_string("setup"), gem_string(""));
                            GemVal _t1503 = _t1502;
                            gem_pop_frame();
                            return _t1503;
                        } else {
#line 539 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 540 "compiler/main.gem"
    GemVal _t1504 = gem_v_node;
                                GemVal gem_v_name = gem_table_get(_t1504, gem_string("name"));
#line 541 "compiler/main.gem"
    GemVal _t1505[] = {(*gem_v_builtin_fns), gem_v_name};
                                if (gem_truthy(gem_has_key_fn(NULL, _t1505, 2))) {
#line 542 "compiler/main.gem"
    GemVal _t1506 = gem_table_new();
    GemVal _t1507[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name)};
    gem_table_set(_t1506, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1507, 1)), gem_string(", NULL)")));
    gem_table_set(_t1506, gem_string("setup"), gem_string(""));
                                    GemVal _t1508 = _t1506;
                                    gem_pop_frame();
                                    return _t1508;
                                } else {
#line 543 "compiler/main.gem"
    GemVal _t1509[] = {(*gem_v_defined_fns), gem_v_name};
                                    if (gem_truthy(gem_fn_set_contains(NULL, _t1509, 2))) {
#line 544 "compiler/main.gem"
    GemVal _t1510 = gem_table_new();
    GemVal _t1511[] = {gem_v_name};
    gem_table_set(_t1510, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t1511, 1)), gem_string(", NULL)")));
    gem_table_set(_t1510, gem_string("setup"), gem_string(""));
                                        GemVal _t1512 = _t1510;
                                        gem_pop_frame();
                                        return _t1512;
                                    }
                                }
#line 546 "compiler/main.gem"
    GemVal _t1513 = (*gem_v_mangle);
    GemVal _t1514[] = {gem_v_name};
                                GemVal gem_v_mname = _t1513.fn(_t1513.env, _t1514, 1);
#line 547 "compiler/main.gem"
    GemVal _t1515[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t1517;
    if (gem_truthy(gem_fn_set_contains(NULL, _t1515, 2))) {
        _t1517 = gem_fn_set_contains(NULL, _t1515, 2);
    } else {
        GemVal _t1516[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t1517 = gem_fn_set_contains(NULL, _t1516, 2);
    }
                                if (gem_truthy(_t1517)) {
#line 548 "compiler/main.gem"
    GemVal _t1518 = gem_table_new();
    GemVal _t1519[] = {gem_v_mname};
    gem_table_set(_t1518, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t1519, 1)), gem_string(")")));
    gem_table_set(_t1518, gem_string("setup"), gem_string(""));
                                    GemVal _t1520 = _t1518;
                                    gem_pop_frame();
                                    return _t1520;
                                }
#line 550 "compiler/main.gem"
    GemVal _t1521 = gem_table_new();
    gem_table_set(_t1521, gem_string("expr"), gem_v_mname);
    gem_table_set(_t1521, gem_string("setup"), gem_string(""));
                                GemVal _t1522 = _t1521;
                                gem_pop_frame();
                                return _t1522;
                            } else {
#line 551 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 552 "compiler/main.gem"
    GemVal _t1523 = (*gem_v_compile_call);
    GemVal _t1524[] = {gem_v_node};
                                    GemVal _t1525 = _t1523.fn(_t1523.env, _t1524, 1);
                                    gem_pop_frame();
                                    return _t1525;
                                } else {
#line 553 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("binop")))) {
#line 554 "compiler/main.gem"
    GemVal _t1526 = (*gem_v_compile_binop);
    GemVal _t1527[] = {gem_v_node};
                                        GemVal _t1528 = _t1526.fn(_t1526.env, _t1527, 1);
                                        gem_pop_frame();
                                        return _t1528;
                                    } else {
#line 555 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("unop")))) {
#line 556 "compiler/main.gem"
    GemVal _t1529 = gem_v_node;
    GemVal _t1530 = (*gem_v_compile_expr);
    GemVal _t1531[] = {gem_table_get(_t1529, gem_string("expr"))};
                                            GemVal gem_v_r = _t1530.fn(_t1530.env, _t1531, 1);
#line 557 "compiler/main.gem"
    GemVal _t1532 = gem_v_node;
                                            if (gem_truthy(gem_eq(gem_table_get(_t1532, gem_string("op")), gem_string("-")))) {
#line 558 "compiler/main.gem"
    GemVal _t1533 = gem_table_new();
    GemVal _t1534 = gem_v_r;
    GemVal _t1535[] = {gem_table_get(_t1534, gem_string("expr"))};
    gem_table_set(_t1533, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t1535, 1)), gem_string(")")));
    GemVal _t1536 = gem_v_r;
    gem_table_set(_t1533, gem_string("setup"), gem_table_get(_t1536, gem_string("setup")));
                                                GemVal _t1537 = _t1533;
                                                gem_pop_frame();
                                                return _t1537;
                                            } else {
#line 559 "compiler/main.gem"
    GemVal _t1538 = gem_v_node;
                                                if (gem_truthy(gem_eq(gem_table_get(_t1538, gem_string("op")), gem_string("not")))) {
#line 560 "compiler/main.gem"
    GemVal _t1539 = gem_table_new();
    GemVal _t1540 = gem_v_r;
    GemVal _t1541[] = {gem_table_get(_t1540, gem_string("expr"))};
    gem_table_set(_t1539, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t1541, 1)), gem_string(")")));
    GemVal _t1542 = gem_v_r;
    gem_table_set(_t1539, gem_string("setup"), gem_table_get(_t1542, gem_string("setup")));
                                                    GemVal _t1543 = _t1539;
                                                    gem_pop_frame();
                                                    return _t1543;
                                                }
                                            }
                                        } else {
#line 562 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 563 "compiler/main.gem"
    GemVal _t1544 = (*gem_v_compile_anon_fn);
    GemVal _t1545[] = {gem_v_node};
                                                GemVal _t1546 = _t1544.fn(_t1544.env, _t1545, 1);
                                                gem_pop_frame();
                                                return _t1546;
                                            } else {
#line 564 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("table")))) {
#line 565 "compiler/main.gem"
    GemVal _t1547 = (*gem_v_compile_table);
    GemVal _t1548[] = {gem_v_node};
                                                    GemVal _t1549 = _t1547.fn(_t1547.env, _t1548, 1);
                                                    gem_pop_frame();
                                                    return _t1549;
                                                } else {
#line 566 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("array")))) {
#line 567 "compiler/main.gem"
    GemVal _t1550 = (*gem_v_compile_array);
    GemVal _t1551[] = {gem_v_node};
                                                        GemVal _t1552 = _t1550.fn(_t1550.env, _t1551, 1);
                                                        gem_pop_frame();
                                                        return _t1552;
                                                    } else {
#line 568 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot")))) {
#line 569 "compiler/main.gem"
    GemVal _t1553 = gem_v_node;
    GemVal _t1554 = (*gem_v_compile_expr);
    GemVal _t1555[] = {gem_table_get(_t1553, gem_string("object"))};
                                                            GemVal gem_v_r = _t1554.fn(_t1554.env, _t1555, 1);
#line 570 "compiler/main.gem"
    GemVal _t1556 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1556.fn(_t1556.env, NULL, 0);
#line 571 "compiler/main.gem"
    GemVal _t1557 = gem_v_r;
    GemVal _t1558[] = {gem_v_t};
    GemVal _t1559 = gem_v_r;
    GemVal _t1560[] = {gem_table_get(_t1559, gem_string("expr"))};
                                                            GemVal gem_v_setup = gem_add(gem_table_get(_t1557, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1558, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1560, 1)), gem_string(";\n")));
#line 572 "compiler/main.gem"
    GemVal _t1561 = gem_v_node;
    GemVal _t1562[] = {gem_table_get(_t1561, gem_string("field"))};
                                                            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1562, 1);
#line 573 "compiler/main.gem"
    GemVal _t1563 = gem_table_new();
    GemVal _t1564[] = {gem_v_t};
    GemVal _t1565[] = {gem_v_escaped};
    gem_table_set(_t1563, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1564, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1565, 1)), gem_string("\"))")));
    gem_table_set(_t1563, gem_string("setup"), gem_v_setup);
                                                            GemVal _t1566 = _t1563;
                                                            gem_pop_frame();
                                                            return _t1566;
                                                        } else {
#line 574 "compiler/main.gem"
                                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("index")))) {
#line 575 "compiler/main.gem"
    GemVal _t1567 = gem_v_node;
    GemVal _t1568 = (*gem_v_compile_expr);
    GemVal _t1569[] = {gem_table_get(_t1567, gem_string("object"))};
                                                                GemVal gem_v_obj_r = _t1568.fn(_t1568.env, _t1569, 1);
#line 576 "compiler/main.gem"
    GemVal _t1570 = gem_v_node;
    GemVal _t1571 = (*gem_v_compile_expr);
    GemVal _t1572[] = {gem_table_get(_t1570, gem_string("key"))};
                                                                GemVal gem_v_key_r = _t1571.fn(_t1571.env, _t1572, 1);
#line 577 "compiler/main.gem"
    GemVal _t1573 = gem_table_new();
    GemVal _t1574 = gem_v_obj_r;
    GemVal _t1575[] = {gem_table_get(_t1574, gem_string("expr"))};
    GemVal _t1576 = gem_v_key_r;
    GemVal _t1577[] = {gem_table_get(_t1576, gem_string("expr"))};
    gem_table_set(_t1573, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1575, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1577, 1)), gem_string(")")));
    GemVal _t1578 = gem_v_obj_r;
    GemVal _t1579 = gem_v_key_r;
    gem_table_set(_t1573, gem_string("setup"), gem_add(gem_table_get(_t1578, gem_string("setup")), gem_table_get(_t1579, gem_string("setup"))));
                                                                GemVal _t1580 = _t1573;
                                                                gem_pop_frame();
                                                                return _t1580;
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
    GemVal _t1581 = gem_table_new();
    gem_table_set(_t1581, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1581, gem_string("setup"), gem_add(gem_add(gem_string("/* unknown node: "), gem_v_tag), gem_string(" */\n")));
    GemVal _t1582 = _t1581;
    gem_pop_frame();
    return _t1582;
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
#line 586 "compiler/main.gem"
    GemVal _t1584 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t1584, gem_string("entries"));
#line 587 "compiler/main.gem"
    GemVal _t1585 = (*gem_v_tmp);
    GemVal gem_v_t = _t1585.fn(_t1585.env, NULL, 0);
#line 588 "compiler/main.gem"
    GemVal _t1586[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1586, 1)), gem_string(" = gem_table_new();\n"));
#line 589 "compiler/main.gem"
    GemVal gem_v__for_items_25 = gem_v_entries;
#line 589 "compiler/main.gem"
    GemVal gem_v__for_i_25 = gem_int(0);
#line 589 "compiler/main.gem"
    while (1) {
        GemVal _t1587[] = {gem_v__for_items_25};
        if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_len_fn(NULL, _t1587, 1)))) break;
#line 589 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_25, gem_v__for_i_25);
#line 589 "compiler/main.gem"
        gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 590 "compiler/main.gem"
    GemVal _t1588 = gem_v_entry;
    GemVal _t1589 = (*gem_v_compile_expr);
    GemVal _t1590[] = {gem_table_get(_t1588, gem_string("value"))};
        GemVal gem_v_val_r = _t1589.fn(_t1589.env, _t1590, 1);
#line 591 "compiler/main.gem"
    GemVal _t1591 = gem_v_entry;
    GemVal _t1592[] = {gem_table_get(_t1591, gem_string("key"))};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1592, 1);
#line 592 "compiler/main.gem"
    GemVal _t1593 = gem_v_val_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1593, gem_string("setup")));
#line 593 "compiler/main.gem"
    GemVal _t1594[] = {gem_v_t};
    GemVal _t1595[] = {gem_v_escaped};
    GemVal _t1596 = gem_v_val_r;
    GemVal _t1597[] = {gem_table_get(_t1596, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1594, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1595, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1597, 1)), gem_string(");\n")));
    }

    GemVal _t1598 = gem_table_new();
    gem_table_set(_t1598, gem_string("expr"), gem_v_t);
    gem_table_set(_t1598, gem_string("setup"), gem_v_setup);
    GemVal _t1599 = _t1598;
    gem_pop_frame();
    return _t1599;
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
#line 601 "compiler/main.gem"
    GemVal _t1601 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t1601, gem_string("elements"));
#line 602 "compiler/main.gem"
    GemVal _t1602 = (*gem_v_tmp);
    GemVal gem_v_t = _t1602.fn(_t1602.env, NULL, 0);
#line 603 "compiler/main.gem"
    GemVal _t1603[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1603, 1)), gem_string(" = gem_table_new();\n"));
#line 604 "compiler/main.gem"
    GemVal gem_v__for_i_26 = gem_int(0);
#line 604 "compiler/main.gem"
    GemVal _t1604[] = {gem_v_elements};
    GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1604, 1);
#line 604 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 604 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_26;
#line 604 "compiler/main.gem"
        gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 605 "compiler/main.gem"
    GemVal _t1605 = (*gem_v_compile_expr);
    GemVal _t1606[] = {gem_table_get(gem_v_elements, gem_v_i)};
        GemVal gem_v_elem_r = _t1605.fn(_t1605.env, _t1606, 1);
#line 606 "compiler/main.gem"
    GemVal _t1607 = gem_v_elem_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1607, gem_string("setup")));
#line 607 "compiler/main.gem"
    GemVal _t1608[] = {gem_v_t};
    GemVal _t1609[] = {gem_v_i};
    GemVal _t1610 = gem_v_elem_r;
    GemVal _t1611[] = {gem_table_get(_t1610, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1608, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t1609, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t1611, 1)), gem_string(");\n")));
    }

    GemVal _t1612 = gem_table_new();
    gem_table_set(_t1612, gem_string("expr"), gem_v_t);
    gem_table_set(_t1612, gem_string("setup"), gem_v_setup);
    GemVal _t1613 = _t1612;
    gem_pop_frame();
    return _t1613;
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
#line 615 "compiler/main.gem"
    GemVal _t1615 = gem_v_node;
    GemVal _t1616[] = {gem_table_get(_t1615, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1616, 1);
#line 616 "compiler/main.gem"
    GemVal _t1617 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1617, gem_string("rest_param")), GEM_NIL))) {
#line 617 "compiler/main.gem"
    GemVal _t1618 = gem_v_node;
    GemVal _t1619[] = {gem_v_inner_defined, gem_table_get(_t1618, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t1619, 2));
    }
#line 619 "compiler/main.gem"
    GemVal _t1620 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1620, gem_string("block_param")), GEM_NIL))) {
#line 620 "compiler/main.gem"
    GemVal _t1621 = gem_v_node;
    GemVal _t1622[] = {gem_v_inner_defined, gem_table_get(_t1621, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t1622, 2));
    }
#line 622 "compiler/main.gem"
    GemVal _t1623 = gem_v_node;
    GemVal _t1624 = (*gem_v_collect_free_vars);
    GemVal _t1625[] = {gem_table_get(_t1623, gem_string("body")), gem_v_inner_defined};
    GemVal gem_v_free = _t1624.fn(_t1624.env, _t1625, 2);
#line 623 "compiler/main.gem"
    GemVal _t1626[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t1626, 1);
#line 625 "compiler/main.gem"
    GemVal _t1627[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1627, 1), gem_int(0)))) {
#line 626 "compiler/main.gem"
    GemVal _t1628 = gem_table_new();
    GemVal _t1629 = (*gem_v_compile_closure_fn);
    GemVal _t1630[] = {gem_v_node, _t1628};
        GemVal gem_v_result = _t1629.fn(_t1629.env, _t1630, 2);
#line 627 "compiler/main.gem"
    GemVal _t1631 = gem_table_new();
    GemVal _t1632 = gem_v_result;
    GemVal _t1633[] = {gem_table_get(_t1632, gem_string("fn_name"))};
    gem_table_set(_t1631, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1633, 1)), gem_string(", NULL)")));
    gem_table_set(_t1631, gem_string("setup"), gem_string(""));
        GemVal _t1634 = _t1631;
        gem_pop_frame();
        return _t1634;
    }
#line 630 "compiler/main.gem"
    GemVal _t1635 = (*gem_v_compile_closure_fn);
    GemVal _t1636[] = {gem_v_node, gem_v_captures};
    GemVal gem_v_result = _t1635.fn(_t1635.env, _t1636, 2);
#line 632 "compiler/main.gem"
    GemVal gem_v_setup = gem_string("");
#line 633 "compiler/main.gem"
    GemVal _t1637 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t1637.fn(_t1637.env, NULL, 0);
#line 634 "compiler/main.gem"
    GemVal _t1638 = gem_v_result;
    GemVal _t1639[] = {gem_table_get(_t1638, gem_string("struct_name"))};
    GemVal _t1640[] = {gem_v_env_tmp};
    GemVal _t1641 = gem_v_result;
    GemVal _t1642[] = {gem_table_get(_t1641, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t1639, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t1640, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t1642, 1)), gem_string("));\n")));
#line 635 "compiler/main.gem"
    GemVal gem_v__for_items_27 = gem_v_captures;
#line 635 "compiler/main.gem"
    GemVal gem_v__for_i_27 = gem_int(0);
#line 635 "compiler/main.gem"
    while (1) {
        GemVal _t1643[] = {gem_v__for_items_27};
        if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_len_fn(NULL, _t1643, 1)))) break;
#line 635 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_27, gem_v__for_i_27);
#line 635 "compiler/main.gem"
        gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 636 "compiler/main.gem"
    GemVal _t1644 = (*gem_v_mangle);
    GemVal _t1645[] = {gem_v_cap};
        GemVal gem_v_mc = _t1644.fn(_t1644.env, _t1645, 1);
#line 637 "compiler/main.gem"
    GemVal _t1646[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1646, 2))) {
#line 638 "compiler/main.gem"
    GemVal _t1647[] = {gem_v_env_tmp};
    GemVal _t1648[] = {gem_v_mc};
    GemVal _t1649[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1647, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1648, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1649, 1)), gem_string(";\n")));
        } else {
#line 640 "compiler/main.gem"
    GemVal _t1650[] = {gem_v_env_tmp};
    GemVal _t1651[] = {gem_v_mc};
    GemVal _t1652[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1650, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1651, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t1652, 1)), gem_string(";\n")));
        }
    }

    GemVal _t1653 = gem_table_new();
    GemVal _t1654 = gem_v_result;
    GemVal _t1655[] = {gem_table_get(_t1654, gem_string("fn_name"))};
    GemVal _t1656[] = {gem_v_env_tmp};
    gem_table_set(_t1653, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1655, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1656, 1)), gem_string(")")));
    gem_table_set(_t1653, gem_string("setup"), gem_v_setup);
    GemVal _t1657 = _t1653;
    gem_pop_frame();
    return _t1657;
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
#line 649 "compiler/main.gem"
    GemVal gem_v__d28 = gem_v_node;
#line 649 "compiler/main.gem"
    GemVal _t1659 = gem_v__d28;
    GemVal gem_v_func = gem_table_get(_t1659, gem_string("func"));
#line 649 "compiler/main.gem"
    GemVal _t1660 = gem_v__d28;
    GemVal gem_v_args = gem_table_get(_t1660, gem_string("args"));

#line 652 "compiler/main.gem"
    GemVal _t1661 = gem_v_func;
    GemVal _t1663;
    if (!gem_truthy(gem_eq(gem_table_get(_t1661, gem_string("tag")), gem_string("dot")))) {
        _t1663 = gem_eq(gem_table_get(_t1661, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t1662 = gem_v_func;
        _t1663 = gem_eq(gem_table_get(_t1662, gem_string("field")), gem_string("len"));
    }
    GemVal _t1665;
    if (!gem_truthy(_t1663)) {
        _t1665 = _t1663;
    } else {
        GemVal _t1664[] = {gem_v_args};
        _t1665 = gem_eq(gem_len_fn(NULL, _t1664, 1), gem_int(0));
    }
    if (gem_truthy(_t1665)) {
#line 653 "compiler/main.gem"
    GemVal _t1666 = gem_v_func;
    GemVal _t1667 = (*gem_v_compile_expr);
    GemVal _t1668[] = {gem_table_get(_t1666, gem_string("object"))};
        GemVal gem_v_obj_r = _t1667.fn(_t1667.env, _t1668, 1);
#line 654 "compiler/main.gem"
    GemVal _t1669 = gem_table_new();
    GemVal _t1670 = gem_v_obj_r;
    GemVal _t1671[] = {gem_table_get(_t1670, gem_string("expr"))};
    gem_table_set(_t1669, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t1671, 1)), gem_string(")")));
    GemVal _t1672 = gem_v_obj_r;
    gem_table_set(_t1669, gem_string("setup"), gem_table_get(_t1672, gem_string("setup")));
        GemVal _t1673 = _t1669;
        gem_pop_frame();
        return _t1673;
    }
#line 658 "compiler/main.gem"
    GemVal _t1674 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t1674, gem_string("tag")), gem_string("var")))) {
#line 659 "compiler/main.gem"
    GemVal _t1675 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t1675, gem_string("name"));
#line 661 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 662 "compiler/main.gem"
    GemVal _t1676 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t1676, gem_string("line"));
#line 663 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 664 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 666 "compiler/main.gem"
    GemVal _t1677[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t1677, 1);
#line 667 "compiler/main.gem"
            GemVal gem_v_arg_setups = gem_string("");
#line 668 "compiler/main.gem"
    GemVal _t1678 = gem_table_new();
            GemVal gem_v_arg_exprs = _t1678;
#line 669 "compiler/main.gem"
            GemVal gem_v__for_i_29 = gem_int(0);
#line 669 "compiler/main.gem"
    GemVal _t1679[] = {gem_v_args};
            GemVal gem_v__for_limit_29 = gem_len_fn(NULL, _t1679, 1);
#line 669 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_v__for_limit_29))) break;
#line 669 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_29;
#line 669 "compiler/main.gem"
                gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 670 "compiler/main.gem"
    GemVal _t1680 = (*gem_v_compile_expr);
    GemVal _t1681[] = {gem_table_get(gem_v_args, gem_v_i)};
                GemVal gem_v_r = _t1680.fn(_t1680.env, _t1681, 1);
#line 671 "compiler/main.gem"
    GemVal _t1682 = gem_v_r;
                gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1682, gem_string("setup")));
#line 672 "compiler/main.gem"
    GemVal _t1683 = gem_v_r;
    GemVal _t1684[] = {gem_v_arg_exprs, gem_table_get(_t1683, gem_string("expr"))};
                (void)(gem_push_fn(NULL, _t1684, 2));
            }

#line 674 "compiler/main.gem"
    GemVal _t1685[] = {gem_v_args};
            GemVal gem_v_argc = gem_len_fn(NULL, _t1685, 1);
#line 675 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 676 "compiler/main.gem"
    GemVal _t1686 = gem_table_new();
    GemVal _t1687[] = {gem_v_escaped_file};
    GemVal _t1688[] = {gem_v_line};
    gem_table_set(_t1686, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1687, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1688, 1)), gem_string(", NULL, 0)")));
    gem_table_set(_t1686, gem_string("setup"), gem_v_arg_setups);
                GemVal _t1689 = _t1686;
                gem_pop_frame();
                return _t1689;
            }
#line 678 "compiler/main.gem"
    GemVal _t1690 = (*gem_v_tmp);
            GemVal gem_v_t = _t1690.fn(_t1690.env, NULL, 0);
#line 679 "compiler/main.gem"
            GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 680 "compiler/main.gem"
            GemVal gem_v__for_i_30 = gem_int(1);
#line 680 "compiler/main.gem"
            GemVal gem_v__for_limit_30 = gem_v_argc;
#line 680 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 680 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_30;
#line 680 "compiler/main.gem"
                gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 681 "compiler/main.gem"
                gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
            }

#line 683 "compiler/main.gem"
    GemVal _t1691[] = {gem_v_t};
    GemVal _t1692[] = {gem_v_arr};
            GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1691, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1692, 1)), gem_string("};\n")));
#line 684 "compiler/main.gem"
    GemVal _t1693[] = {gem_v_escaped_file};
    GemVal _t1694[] = {gem_v_line};
    GemVal _t1695[] = {gem_v_t};
    GemVal _t1696[] = {gem_v_argc};
            GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1693, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1694, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1695, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1696, 1)), gem_string(")"));
#line 685 "compiler/main.gem"
    GemVal _t1697 = gem_table_new();
    gem_table_set(_t1697, gem_string("expr"), gem_v_call);
    gem_table_set(_t1697, gem_string("setup"), gem_v_setup);
            GemVal _t1698 = _t1697;
            gem_pop_frame();
            return _t1698;
        } else {
#line 686 "compiler/main.gem"
    GemVal _t1699[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t1699, 2))) {
#line 687 "compiler/main.gem"
                GemVal gem_v_fn_name = gem_table_get((*gem_v_builtin_fns), gem_v_name);
#line 688 "compiler/main.gem"
                GemVal gem_v_arg_setups = gem_string("");
#line 689 "compiler/main.gem"
    GemVal _t1700 = gem_table_new();
                GemVal gem_v_arg_exprs = _t1700;
#line 690 "compiler/main.gem"
                GemVal gem_v__for_i_31 = gem_int(0);
#line 690 "compiler/main.gem"
    GemVal _t1701[] = {gem_v_args};
                GemVal gem_v__for_limit_31 = gem_len_fn(NULL, _t1701, 1);
#line 690 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_v__for_limit_31))) break;
#line 690 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_31;
#line 690 "compiler/main.gem"
                    gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
#line 691 "compiler/main.gem"
    GemVal _t1702 = (*gem_v_compile_expr);
    GemVal _t1703[] = {gem_table_get(gem_v_args, gem_v_i)};
                    GemVal gem_v_r = _t1702.fn(_t1702.env, _t1703, 1);
#line 692 "compiler/main.gem"
    GemVal _t1704 = gem_v_r;
                    gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1704, gem_string("setup")));
#line 693 "compiler/main.gem"
    GemVal _t1705 = gem_v_r;
    GemVal _t1706[] = {gem_v_arg_exprs, gem_table_get(_t1705, gem_string("expr"))};
                    (void)(gem_push_fn(NULL, _t1706, 2));
                }

#line 695 "compiler/main.gem"
    GemVal _t1707[] = {gem_v_args};
                GemVal gem_v_argc = gem_len_fn(NULL, _t1707, 1);
#line 696 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 697 "compiler/main.gem"
    GemVal _t1708 = gem_table_new();
    GemVal _t1709[] = {gem_v_fn_name};
    gem_table_set(_t1708, gem_string("expr"), gem_add(gem_to_string_fn(NULL, _t1709, 1), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1708, gem_string("setup"), gem_v_arg_setups);
                    GemVal _t1710 = _t1708;
                    gem_pop_frame();
                    return _t1710;
                }
#line 699 "compiler/main.gem"
    GemVal _t1711 = (*gem_v_tmp);
                GemVal gem_v_t = _t1711.fn(_t1711.env, NULL, 0);
#line 700 "compiler/main.gem"
                GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 701 "compiler/main.gem"
                GemVal gem_v__for_i_32 = gem_int(1);
#line 701 "compiler/main.gem"
                GemVal gem_v__for_limit_32 = gem_v_argc;
#line 701 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_v__for_limit_32))) break;
#line 701 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_32;
#line 701 "compiler/main.gem"
                    gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 702 "compiler/main.gem"
                    gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                }

#line 704 "compiler/main.gem"
    GemVal _t1712[] = {gem_v_t};
    GemVal _t1713[] = {gem_v_arr};
                GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1712, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1713, 1)), gem_string("};\n")));
#line 705 "compiler/main.gem"
    GemVal _t1714 = gem_table_new();
    GemVal _t1715[] = {gem_v_fn_name};
    GemVal _t1716[] = {gem_v_t};
    GemVal _t1717[] = {gem_v_argc};
    gem_table_set(_t1714, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1715, 1), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1716, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1717, 1)), gem_string(")")));
    gem_table_set(_t1714, gem_string("setup"), gem_v_setup);
                GemVal _t1718 = _t1714;
                gem_pop_frame();
                return _t1718;
            } else {
#line 706 "compiler/main.gem"
    GemVal _t1719[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t1719, 2))) {
#line 707 "compiler/main.gem"
                    GemVal gem_v_arg_setups = gem_string("");
#line 708 "compiler/main.gem"
    GemVal _t1720 = gem_table_new();
                    GemVal gem_v_arg_exprs = _t1720;
#line 709 "compiler/main.gem"
                    GemVal gem_v__for_i_33 = gem_int(0);
#line 709 "compiler/main.gem"
    GemVal _t1721[] = {gem_v_args};
                    GemVal gem_v__for_limit_33 = gem_len_fn(NULL, _t1721, 1);
#line 709 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
#line 709 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_33;
#line 709 "compiler/main.gem"
                        gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 710 "compiler/main.gem"
    GemVal _t1722 = (*gem_v_compile_expr);
    GemVal _t1723[] = {gem_table_get(gem_v_args, gem_v_i)};
                        GemVal gem_v_r = _t1722.fn(_t1722.env, _t1723, 1);
#line 711 "compiler/main.gem"
    GemVal _t1724 = gem_v_r;
                        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1724, gem_string("setup")));
#line 712 "compiler/main.gem"
    GemVal _t1725 = gem_v_r;
    GemVal _t1726[] = {gem_v_arg_exprs, gem_table_get(_t1725, gem_string("expr"))};
                        (void)(gem_push_fn(NULL, _t1726, 2));
                    }

#line 714 "compiler/main.gem"
    GemVal _t1727[] = {gem_v_args};
                    GemVal gem_v_argc = gem_len_fn(NULL, _t1727, 1);
#line 715 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 716 "compiler/main.gem"
    GemVal _t1728 = gem_table_new();
    GemVal _t1729[] = {gem_v_name};
    gem_table_set(_t1728, gem_string("expr"), gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1729, 1)), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1728, gem_string("setup"), gem_v_arg_setups);
                        GemVal _t1730 = _t1728;
                        gem_pop_frame();
                        return _t1730;
                    }
#line 718 "compiler/main.gem"
    GemVal _t1731 = (*gem_v_tmp);
                    GemVal gem_v_t = _t1731.fn(_t1731.env, NULL, 0);
#line 719 "compiler/main.gem"
                    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 720 "compiler/main.gem"
                    GemVal gem_v__for_i_34 = gem_int(1);
#line 720 "compiler/main.gem"
                    GemVal gem_v__for_limit_34 = gem_v_argc;
#line 720 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
#line 720 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_34;
#line 720 "compiler/main.gem"
                        gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 721 "compiler/main.gem"
                        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                    }

#line 723 "compiler/main.gem"
    GemVal _t1732[] = {gem_v_t};
    GemVal _t1733[] = {gem_v_arr};
                    GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1732, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1733, 1)), gem_string("};\n")));
#line 724 "compiler/main.gem"
    GemVal _t1734 = gem_table_new();
    GemVal _t1735[] = {gem_v_name};
    GemVal _t1736[] = {gem_v_t};
    GemVal _t1737[] = {gem_v_argc};
    gem_table_set(_t1734, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1735, 1)), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1736, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1737, 1)), gem_string(")")));
    gem_table_set(_t1734, gem_string("setup"), gem_v_setup);
                    GemVal _t1738 = _t1734;
                    gem_pop_frame();
                    return _t1738;
                }
            }
        }
    }
#line 729 "compiler/main.gem"
    GemVal _t1739 = (*gem_v_compile_expr);
    GemVal _t1740[] = {gem_v_func};
    GemVal gem_v_func_r = _t1739.fn(_t1739.env, _t1740, 1);
#line 730 "compiler/main.gem"
    GemVal gem_v_arg_setups = gem_string("");
#line 731 "compiler/main.gem"
    GemVal _t1741 = gem_table_new();
    GemVal gem_v_arg_exprs = _t1741;
#line 732 "compiler/main.gem"
    GemVal gem_v__for_i_35 = gem_int(0);
#line 732 "compiler/main.gem"
    GemVal _t1742[] = {gem_v_args};
    GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t1742, 1);
#line 732 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 732 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_35;
#line 732 "compiler/main.gem"
        gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 733 "compiler/main.gem"
    GemVal _t1743 = (*gem_v_compile_expr);
    GemVal _t1744[] = {gem_table_get(gem_v_args, gem_v_i)};
        GemVal gem_v_r = _t1743.fn(_t1743.env, _t1744, 1);
#line 734 "compiler/main.gem"
    GemVal _t1745 = gem_v_r;
        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1745, gem_string("setup")));
#line 735 "compiler/main.gem"
    GemVal _t1746 = gem_v_r;
    GemVal _t1747[] = {gem_v_arg_exprs, gem_table_get(_t1746, gem_string("expr"))};
        (void)(gem_push_fn(NULL, _t1747, 2));
    }

#line 737 "compiler/main.gem"
    GemVal _t1748 = gem_v_func_r;
    GemVal gem_v_setup = gem_add(gem_table_get(_t1748, gem_string("setup")), gem_v_arg_setups);
#line 738 "compiler/main.gem"
    GemVal _t1749[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t1749, 1);
#line 739 "compiler/main.gem"
    GemVal _t1750 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t1750.fn(_t1750.env, NULL, 0);
#line 740 "compiler/main.gem"
    GemVal _t1751[] = {gem_v_tmp_fn};
    GemVal _t1752 = gem_v_func_r;
    GemVal _t1753[] = {gem_table_get(_t1752, gem_string("expr"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1751, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1753, 1)), gem_string(";\n")));
#line 741 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 742 "compiler/main.gem"
    GemVal _t1754 = gem_table_new();
    GemVal _t1755[] = {gem_v_tmp_fn};
    GemVal _t1756[] = {gem_v_tmp_fn};
    gem_table_set(_t1754, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1755, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1756, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t1754, gem_string("setup"), gem_v_setup);
        GemVal _t1757 = _t1754;
        gem_pop_frame();
        return _t1757;
    }
#line 744 "compiler/main.gem"
    GemVal _t1758 = (*gem_v_tmp);
    GemVal gem_v_tmp_args = _t1758.fn(_t1758.env, NULL, 0);
#line 745 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 746 "compiler/main.gem"
    GemVal gem_v__for_i_36 = gem_int(1);
#line 746 "compiler/main.gem"
    GemVal gem_v__for_limit_36 = gem_v_argc;
#line 746 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 746 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_36;
#line 746 "compiler/main.gem"
        gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 747 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

#line 749 "compiler/main.gem"
    GemVal _t1759[] = {gem_v_tmp_args};
    GemVal _t1760[] = {gem_v_arr};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1759, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1760, 1)), gem_string("};\n")));
#line 750 "compiler/main.gem"
    GemVal _t1761[] = {gem_v_tmp_fn};
    GemVal _t1762[] = {gem_v_tmp_fn};
    GemVal _t1763[] = {gem_v_tmp_args};
    GemVal _t1764[] = {gem_v_argc};
    GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1761, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1762, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t1763, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1764, 1)), gem_string(")"));
    GemVal _t1765 = gem_table_new();
    gem_table_set(_t1765, gem_string("expr"), gem_v_call);
    gem_table_set(_t1765, gem_string("setup"), gem_v_setup);
    GemVal _t1766 = _t1765;
    gem_pop_frame();
    return _t1766;
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
#line 757 "compiler/main.gem"
    GemVal _t1768 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t1768, gem_string("op"));
#line 758 "compiler/main.gem"
    GemVal _t1769 = gem_v_node;
    GemVal _t1770 = (*gem_v_compile_expr);
    GemVal _t1771[] = {gem_table_get(_t1769, gem_string("left"))};
    GemVal gem_v_lc_r = _t1770.fn(_t1770.env, _t1771, 1);
#line 759 "compiler/main.gem"
    GemVal _t1772 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t1772, gem_string("expr"));
#line 760 "compiler/main.gem"
    GemVal _t1773 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t1773, gem_string("setup"));
#line 762 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 763 "compiler/main.gem"
    GemVal _t1774 = gem_v_node;
    GemVal _t1775 = (*gem_v_compile_expr);
    GemVal _t1776[] = {gem_table_get(_t1774, gem_string("right"))};
        GemVal gem_v_rc_r = _t1775.fn(_t1775.env, _t1776, 1);
#line 764 "compiler/main.gem"
    GemVal _t1777 = gem_table_new();
    GemVal _t1778[] = {gem_v_lc};
    GemVal _t1779 = gem_v_rc_r;
    GemVal _t1780[] = {gem_table_get(_t1779, gem_string("expr"))};
    gem_table_set(_t1777, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1778, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1780, 1)), gem_string(")")));
    GemVal _t1781 = gem_v_rc_r;
    gem_table_set(_t1777, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1781, gem_string("setup"))));
        GemVal _t1782 = _t1777;
        gem_pop_frame();
        return _t1782;
    } else {
#line 765 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 766 "compiler/main.gem"
    GemVal _t1783 = gem_v_node;
    GemVal _t1784 = (*gem_v_compile_expr);
    GemVal _t1785[] = {gem_table_get(_t1783, gem_string("right"))};
            GemVal gem_v_rc_r = _t1784.fn(_t1784.env, _t1785, 1);
#line 767 "compiler/main.gem"
    GemVal _t1786 = gem_table_new();
    GemVal _t1787[] = {gem_v_lc};
    GemVal _t1788 = gem_v_rc_r;
    GemVal _t1789[] = {gem_table_get(_t1788, gem_string("expr"))};
    gem_table_set(_t1786, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t1787, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1789, 1)), gem_string(")")));
    GemVal _t1790 = gem_v_rc_r;
    gem_table_set(_t1786, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1790, gem_string("setup"))));
            GemVal _t1791 = _t1786;
            gem_pop_frame();
            return _t1791;
        } else {
#line 768 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 769 "compiler/main.gem"
    GemVal _t1792 = gem_v_node;
    GemVal _t1793 = (*gem_v_compile_expr);
    GemVal _t1794[] = {gem_table_get(_t1792, gem_string("right"))};
                GemVal gem_v_rc_r = _t1793.fn(_t1793.env, _t1794, 1);
#line 770 "compiler/main.gem"
    GemVal _t1795 = gem_table_new();
    GemVal _t1796[] = {gem_v_lc};
    GemVal _t1797 = gem_v_rc_r;
    GemVal _t1798[] = {gem_table_get(_t1797, gem_string("expr"))};
    gem_table_set(_t1795, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t1796, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1798, 1)), gem_string(")")));
    GemVal _t1799 = gem_v_rc_r;
    gem_table_set(_t1795, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1799, gem_string("setup"))));
                GemVal _t1800 = _t1795;
                gem_pop_frame();
                return _t1800;
            } else {
#line 771 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 772 "compiler/main.gem"
    GemVal _t1801 = gem_v_node;
    GemVal _t1802 = (*gem_v_compile_expr);
    GemVal _t1803[] = {gem_table_get(_t1801, gem_string("right"))};
                    GemVal gem_v_rc_r = _t1802.fn(_t1802.env, _t1803, 1);
#line 773 "compiler/main.gem"
    GemVal _t1804 = gem_table_new();
    GemVal _t1805[] = {gem_v_lc};
    GemVal _t1806 = gem_v_rc_r;
    GemVal _t1807[] = {gem_table_get(_t1806, gem_string("expr"))};
    gem_table_set(_t1804, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t1805, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1807, 1)), gem_string(")")));
    GemVal _t1808 = gem_v_rc_r;
    gem_table_set(_t1804, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1808, gem_string("setup"))));
                    GemVal _t1809 = _t1804;
                    gem_pop_frame();
                    return _t1809;
                } else {
#line 774 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 775 "compiler/main.gem"
    GemVal _t1810 = gem_v_node;
    GemVal _t1811 = (*gem_v_compile_expr);
    GemVal _t1812[] = {gem_table_get(_t1810, gem_string("right"))};
                        GemVal gem_v_rc_r = _t1811.fn(_t1811.env, _t1812, 1);
#line 776 "compiler/main.gem"
    GemVal _t1813 = gem_table_new();
    GemVal _t1814[] = {gem_v_lc};
    GemVal _t1815 = gem_v_rc_r;
    GemVal _t1816[] = {gem_table_get(_t1815, gem_string("expr"))};
    gem_table_set(_t1813, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t1814, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1816, 1)), gem_string(")")));
    GemVal _t1817 = gem_v_rc_r;
    gem_table_set(_t1813, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1817, gem_string("setup"))));
                        GemVal _t1818 = _t1813;
                        gem_pop_frame();
                        return _t1818;
                    } else {
#line 777 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 778 "compiler/main.gem"
    GemVal _t1819 = gem_v_node;
    GemVal _t1820 = (*gem_v_compile_expr);
    GemVal _t1821[] = {gem_table_get(_t1819, gem_string("right"))};
                            GemVal gem_v_rc_r = _t1820.fn(_t1820.env, _t1821, 1);
#line 779 "compiler/main.gem"
    GemVal _t1822 = (*gem_v_tmp);
                            GemVal gem_v_t = _t1822.fn(_t1822.env, NULL, 0);
#line 780 "compiler/main.gem"
    GemVal _t1823 = gem_v_rc_r;
    GemVal _t1824[] = {gem_v_t};
    GemVal _t1825 = gem_v_rc_r;
    GemVal _t1826[] = {gem_table_get(_t1825, gem_string("expr"))};
    GemVal _t1827[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t1823, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1824, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1826, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1827, 1)), gem_string("};\n")));
#line 781 "compiler/main.gem"
    GemVal _t1828 = gem_table_new();
    GemVal _t1829[] = {gem_v_t};
    gem_table_set(_t1828, gem_string("expr"), gem_add(gem_add(gem_string("gem_has_key_fn(NULL, "), gem_to_string_fn(NULL, _t1829, 1)), gem_string(", 2)")));
    gem_table_set(_t1828, gem_string("setup"), gem_v_setup);
                            GemVal _t1830 = _t1828;
                            gem_pop_frame();
                            return _t1830;
                        } else {
#line 782 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 783 "compiler/main.gem"
    GemVal _t1831 = gem_v_node;
    GemVal _t1832 = (*gem_v_compile_expr);
    GemVal _t1833[] = {gem_table_get(_t1831, gem_string("right"))};
                                GemVal gem_v_rc_r = _t1832.fn(_t1832.env, _t1833, 1);
#line 784 "compiler/main.gem"
    GemVal _t1834 = gem_table_new();
    GemVal _t1835[] = {gem_v_lc};
    GemVal _t1836 = gem_v_rc_r;
    GemVal _t1837[] = {gem_table_get(_t1836, gem_string("expr"))};
    gem_table_set(_t1834, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t1835, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1837, 1)), gem_string(")")));
    GemVal _t1838 = gem_v_rc_r;
    gem_table_set(_t1834, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1838, gem_string("setup"))));
                                GemVal _t1839 = _t1834;
                                gem_pop_frame();
                                return _t1839;
                            } else {
#line 785 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 786 "compiler/main.gem"
    GemVal _t1840 = gem_v_node;
    GemVal _t1841 = (*gem_v_compile_expr);
    GemVal _t1842[] = {gem_table_get(_t1840, gem_string("right"))};
                                    GemVal gem_v_rc_r = _t1841.fn(_t1841.env, _t1842, 1);
#line 787 "compiler/main.gem"
    GemVal _t1843 = gem_table_new();
    GemVal _t1844[] = {gem_v_lc};
    GemVal _t1845 = gem_v_rc_r;
    GemVal _t1846[] = {gem_table_get(_t1845, gem_string("expr"))};
    gem_table_set(_t1843, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t1844, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1846, 1)), gem_string(")")));
    GemVal _t1847 = gem_v_rc_r;
    gem_table_set(_t1843, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1847, gem_string("setup"))));
                                    GemVal _t1848 = _t1843;
                                    gem_pop_frame();
                                    return _t1848;
                                } else {
#line 788 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 789 "compiler/main.gem"
    GemVal _t1849 = gem_v_node;
    GemVal _t1850 = (*gem_v_compile_expr);
    GemVal _t1851[] = {gem_table_get(_t1849, gem_string("right"))};
                                        GemVal gem_v_rc_r = _t1850.fn(_t1850.env, _t1851, 1);
#line 790 "compiler/main.gem"
    GemVal _t1852 = gem_table_new();
    GemVal _t1853[] = {gem_v_lc};
    GemVal _t1854 = gem_v_rc_r;
    GemVal _t1855[] = {gem_table_get(_t1854, gem_string("expr"))};
    gem_table_set(_t1852, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t1853, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1855, 1)), gem_string(")")));
    GemVal _t1856 = gem_v_rc_r;
    gem_table_set(_t1852, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1856, gem_string("setup"))));
                                        GemVal _t1857 = _t1852;
                                        gem_pop_frame();
                                        return _t1857;
                                    } else {
#line 791 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 792 "compiler/main.gem"
    GemVal _t1858 = gem_v_node;
    GemVal _t1859 = (*gem_v_compile_expr);
    GemVal _t1860[] = {gem_table_get(_t1858, gem_string("right"))};
                                            GemVal gem_v_rc_r = _t1859.fn(_t1859.env, _t1860, 1);
#line 793 "compiler/main.gem"
    GemVal _t1861 = gem_table_new();
    GemVal _t1862[] = {gem_v_lc};
    GemVal _t1863 = gem_v_rc_r;
    GemVal _t1864[] = {gem_table_get(_t1863, gem_string("expr"))};
    gem_table_set(_t1861, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t1862, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1864, 1)), gem_string(")")));
    GemVal _t1865 = gem_v_rc_r;
    gem_table_set(_t1861, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1865, gem_string("setup"))));
                                            GemVal _t1866 = _t1861;
                                            gem_pop_frame();
                                            return _t1866;
                                        } else {
#line 794 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 795 "compiler/main.gem"
    GemVal _t1867 = gem_v_node;
    GemVal _t1868 = (*gem_v_compile_expr);
    GemVal _t1869[] = {gem_table_get(_t1867, gem_string("right"))};
                                                GemVal gem_v_rc_r = _t1868.fn(_t1868.env, _t1869, 1);
#line 796 "compiler/main.gem"
    GemVal _t1870 = gem_table_new();
    GemVal _t1871[] = {gem_v_lc};
    GemVal _t1872 = gem_v_rc_r;
    GemVal _t1873[] = {gem_table_get(_t1872, gem_string("expr"))};
    gem_table_set(_t1870, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t1871, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1873, 1)), gem_string(")")));
    GemVal _t1874 = gem_v_rc_r;
    gem_table_set(_t1870, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1874, gem_string("setup"))));
                                                GemVal _t1875 = _t1870;
                                                gem_pop_frame();
                                                return _t1875;
                                            } else {
#line 797 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 798 "compiler/main.gem"
    GemVal _t1876 = gem_v_node;
    GemVal _t1877 = (*gem_v_compile_expr);
    GemVal _t1878[] = {gem_table_get(_t1876, gem_string("right"))};
                                                    GemVal gem_v_rc_r = _t1877.fn(_t1877.env, _t1878, 1);
#line 799 "compiler/main.gem"
    GemVal _t1879 = gem_table_new();
    GemVal _t1880[] = {gem_v_lc};
    GemVal _t1881 = gem_v_rc_r;
    GemVal _t1882[] = {gem_table_get(_t1881, gem_string("expr"))};
    gem_table_set(_t1879, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t1880, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1882, 1)), gem_string(")")));
    GemVal _t1883 = gem_v_rc_r;
    gem_table_set(_t1879, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1883, gem_string("setup"))));
                                                    GemVal _t1884 = _t1879;
                                                    gem_pop_frame();
                                                    return _t1884;
                                                } else {
#line 800 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 801 "compiler/main.gem"
    GemVal _t1885 = gem_v_node;
    GemVal _t1886 = (*gem_v_compile_expr);
    GemVal _t1887[] = {gem_table_get(_t1885, gem_string("right"))};
                                                        GemVal gem_v_rc_r = _t1886.fn(_t1886.env, _t1887, 1);
#line 802 "compiler/main.gem"
    GemVal _t1888 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1888.fn(_t1888.env, NULL, 0);
#line 803 "compiler/main.gem"
                                                        GemVal gem_v_setup = gem_v_ls;
#line 804 "compiler/main.gem"
    GemVal _t1889[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1889, 1)), gem_string(";\n")));
#line 805 "compiler/main.gem"
    GemVal _t1890[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t1890, 1)), gem_string(")) {\n")));
#line 806 "compiler/main.gem"
    GemVal _t1891[] = {gem_v_t};
    GemVal _t1892[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1891, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1892, 1)), gem_string(";\n")));
#line 807 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 808 "compiler/main.gem"
    GemVal _t1893 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t1893, gem_string("setup")), gem_string("")))) {
#line 809 "compiler/main.gem"
    GemVal _t1894 = gem_v_rc_r;
    GemVal _t1895[] = {gem_table_get(_t1894, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t1895, 3));
                                                        }
#line 811 "compiler/main.gem"
    GemVal _t1896[] = {gem_v_t};
    GemVal _t1897 = gem_v_rc_r;
    GemVal _t1898[] = {gem_table_get(_t1897, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1896, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1898, 1)), gem_string(";\n")));
#line 812 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 813 "compiler/main.gem"
    GemVal _t1899 = gem_table_new();
    gem_table_set(_t1899, gem_string("expr"), gem_v_t);
    gem_table_set(_t1899, gem_string("setup"), gem_v_setup);
                                                        GemVal _t1900 = _t1899;
                                                        gem_pop_frame();
                                                        return _t1900;
                                                    } else {
#line 814 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 815 "compiler/main.gem"
    GemVal _t1901 = gem_v_node;
    GemVal _t1902 = (*gem_v_compile_expr);
    GemVal _t1903[] = {gem_table_get(_t1901, gem_string("right"))};
                                                            GemVal gem_v_rc_r = _t1902.fn(_t1902.env, _t1903, 1);
#line 816 "compiler/main.gem"
    GemVal _t1904 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1904.fn(_t1904.env, NULL, 0);
#line 817 "compiler/main.gem"
                                                            GemVal gem_v_setup = gem_v_ls;
#line 818 "compiler/main.gem"
    GemVal _t1905[] = {gem_v_t};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1905, 1)), gem_string(";\n")));
#line 819 "compiler/main.gem"
    GemVal _t1906[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t1906, 1)), gem_string(")) {\n")));
#line 820 "compiler/main.gem"
    GemVal _t1907[] = {gem_v_t};
    GemVal _t1908[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1907, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1908, 1)), gem_string(";\n")));
#line 821 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 822 "compiler/main.gem"
    GemVal _t1909 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t1909, gem_string("setup")), gem_string("")))) {
#line 823 "compiler/main.gem"
    GemVal _t1910 = gem_v_rc_r;
    GemVal _t1911[] = {gem_table_get(_t1910, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                                gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t1911, 3));
                                                            }
#line 825 "compiler/main.gem"
    GemVal _t1912[] = {gem_v_t};
    GemVal _t1913 = gem_v_rc_r;
    GemVal _t1914[] = {gem_table_get(_t1913, gem_string("expr"))};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1912, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1914, 1)), gem_string(";\n")));
#line 826 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 827 "compiler/main.gem"
    GemVal _t1915 = gem_table_new();
    gem_table_set(_t1915, gem_string("expr"), gem_v_t);
    gem_table_set(_t1915, gem_string("setup"), gem_v_setup);
                                                            GemVal _t1916 = _t1915;
                                                            gem_pop_frame();
                                                            return _t1916;
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
#line 830 "compiler/main.gem"
    GemVal _t1917 = gem_v_node;
    GemVal _t1918 = (*gem_v_compile_expr);
    GemVal _t1919[] = {gem_table_get(_t1917, gem_string("right"))};
    GemVal gem_v_rc_r = _t1918.fn(_t1918.env, _t1919, 1);
    GemVal _t1920 = gem_table_new();
    gem_table_set(_t1920, gem_string("expr"), gem_string("GEM_NIL"));
    GemVal _t1921 = gem_v_rc_r;
    gem_table_set(_t1920, gem_string("setup"), gem_add(gem_add(gem_add(gem_add(gem_v_ls, gem_table_get(_t1921, gem_string("setup"))), gem_string("/* unknown op: ")), gem_v_op), gem_string(" */\n")));
    GemVal _t1922 = _t1920;
    gem_pop_frame();
    return _t1922;
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
#line 837 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 838 "compiler/main.gem"
    GemVal gem_v__for_items_37 = gem_v_stmts;
#line 838 "compiler/main.gem"
    GemVal gem_v__for_i_37 = gem_int(0);
#line 838 "compiler/main.gem"
    while (1) {
        GemVal _t1924[] = {gem_v__for_items_37};
        if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_len_fn(NULL, _t1924, 1)))) break;
#line 838 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_37, gem_v__for_i_37);
#line 838 "compiler/main.gem"
        gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 839 "compiler/main.gem"
    GemVal _t1925 = (*gem_v_compile_stmt);
    GemVal _t1926[] = {gem_v_stmt, gem_v_indent};
        gem_v_out = gem_add(gem_add(gem_v_out, _t1925.fn(_t1925.env, _t1926, 2)), gem_string("\n"));
    }

    GemVal _t1927 = gem_v_out;
    gem_pop_frame();
    return _t1927;
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
#line 847 "compiler/main.gem"
    GemVal _t1929[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1929, 1);
#line 848 "compiler/main.gem"
    GemVal _t1930 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1930, gem_string("tag"));
#line 849 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 850 "compiler/main.gem"
    GemVal _t1931 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1931, gem_string("line")), GEM_NIL))) {
#line 851 "compiler/main.gem"
    GemVal _t1932 = gem_v_node;
    GemVal _t1933[] = {gem_table_get(_t1932, gem_string("line"))};
    GemVal _t1934[] = {(*gem_v_source_name)};
    GemVal _t1935[] = {gem_fn_escape_c_string(NULL, _t1934, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t1933, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t1935, 1)), gem_string("\"\n"));
    }
#line 854 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 855 "compiler/main.gem"
    GemVal _t1936 = gem_v_node;
    GemVal _t1937 = (*gem_v_compile_expr);
    GemVal _t1938[] = {gem_table_get(_t1936, gem_string("value"))};
        GemVal gem_v_r = _t1937.fn(_t1937.env, _t1938, 1);
#line 856 "compiler/main.gem"
    GemVal _t1939 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1939, gem_string("name"));
#line 857 "compiler/main.gem"
    GemVal _t1940 = (*gem_v_mangle);
    GemVal _t1941[] = {gem_v_name};
        GemVal gem_v_mname = _t1940.fn(_t1940.env, _t1941, 1);
#line 858 "compiler/main.gem"
    GemVal _t1943;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t1943 = (*gem_v_in_top_level);
    } else {
        GemVal _t1942[] = {(*gem_v_top_level_vars), gem_v_name};
        _t1943 = gem_fn_set_contains(NULL, _t1942, 2);
    }
        if (gem_truthy(_t1943)) {
#line 859 "compiler/main.gem"
    GemVal _t1944[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1944, 2))) {
#line 860 "compiler/main.gem"
    GemVal _t1945[] = {gem_v_line_dir};
    GemVal _t1946 = gem_v_r;
    GemVal _t1947[] = {gem_table_get(_t1946, gem_string("setup"))};
    GemVal _t1948[] = {gem_v_p};
    GemVal _t1949[] = {gem_v_mname};
    GemVal _t1950[] = {gem_v_p};
    GemVal _t1951[] = {gem_v_mname};
    GemVal _t1952 = gem_v_r;
    GemVal _t1953[] = {gem_table_get(_t1952, gem_string("expr"))};
                GemVal _t1954 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1945, 1), gem_to_string_fn(NULL, _t1947, 1)), gem_to_string_fn(NULL, _t1948, 1)), gem_to_string_fn(NULL, _t1949, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1950, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1951, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1953, 1)), gem_string(";"));
                gem_pop_frame();
                return _t1954;
            }
#line 862 "compiler/main.gem"
    GemVal _t1955[] = {gem_v_line_dir};
    GemVal _t1956 = gem_v_r;
    GemVal _t1957[] = {gem_table_get(_t1956, gem_string("setup"))};
    GemVal _t1958[] = {gem_v_p};
    GemVal _t1959[] = {gem_v_mname};
    GemVal _t1960 = gem_v_r;
    GemVal _t1961[] = {gem_table_get(_t1960, gem_string("expr"))};
            GemVal _t1962 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1955, 1), gem_to_string_fn(NULL, _t1957, 1)), gem_to_string_fn(NULL, _t1958, 1)), gem_to_string_fn(NULL, _t1959, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1961, 1)), gem_string(";"));
            gem_pop_frame();
            return _t1962;
        }
#line 864 "compiler/main.gem"
    GemVal _t1963[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1963, 2))) {
#line 865 "compiler/main.gem"
    GemVal _t1964[] = {gem_v_line_dir};
    GemVal _t1965 = gem_v_r;
    GemVal _t1966[] = {gem_table_get(_t1965, gem_string("setup"))};
    GemVal _t1967[] = {gem_v_p};
    GemVal _t1968[] = {gem_v_mname};
    GemVal _t1969[] = {gem_v_p};
    GemVal _t1970[] = {gem_v_mname};
    GemVal _t1971 = gem_v_r;
    GemVal _t1972[] = {gem_table_get(_t1971, gem_string("expr"))};
            GemVal _t1973 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1964, 1), gem_to_string_fn(NULL, _t1966, 1)), gem_to_string_fn(NULL, _t1967, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t1968, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1969, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1970, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1972, 1)), gem_string(";"));
            gem_pop_frame();
            return _t1973;
        }
#line 867 "compiler/main.gem"
    GemVal _t1974[] = {gem_v_line_dir};
    GemVal _t1975 = gem_v_r;
    GemVal _t1976[] = {gem_table_get(_t1975, gem_string("setup"))};
    GemVal _t1977[] = {gem_v_p};
    GemVal _t1978[] = {gem_v_mname};
    GemVal _t1979 = gem_v_r;
    GemVal _t1980[] = {gem_table_get(_t1979, gem_string("expr"))};
        GemVal _t1981 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1974, 1), gem_to_string_fn(NULL, _t1976, 1)), gem_to_string_fn(NULL, _t1977, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1978, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1980, 1)), gem_string(";"));
        gem_pop_frame();
        return _t1981;
    } else {
#line 868 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 869 "compiler/main.gem"
    GemVal _t1982 = gem_v_node;
    GemVal _t1983 = (*gem_v_compile_expr);
    GemVal _t1984[] = {gem_table_get(_t1982, gem_string("value"))};
            GemVal gem_v_r = _t1983.fn(_t1983.env, _t1984, 1);
#line 870 "compiler/main.gem"
    GemVal _t1985 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1985, gem_string("name"));
#line 871 "compiler/main.gem"
    GemVal _t1986 = (*gem_v_mangle);
    GemVal _t1987[] = {gem_v_name};
            GemVal gem_v_mname = _t1986.fn(_t1986.env, _t1987, 1);
#line 872 "compiler/main.gem"
    GemVal _t1988[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t1990;
    if (gem_truthy(gem_fn_set_contains(NULL, _t1988, 2))) {
        _t1990 = gem_fn_set_contains(NULL, _t1988, 2);
    } else {
        GemVal _t1989[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t1990 = gem_fn_set_contains(NULL, _t1989, 2);
    }
            if (gem_truthy(_t1990)) {
#line 873 "compiler/main.gem"
    GemVal _t1991[] = {gem_v_line_dir};
    GemVal _t1992 = gem_v_r;
    GemVal _t1993[] = {gem_table_get(_t1992, gem_string("setup"))};
    GemVal _t1994[] = {gem_v_p};
    GemVal _t1995[] = {gem_v_mname};
    GemVal _t1996 = gem_v_r;
    GemVal _t1997[] = {gem_table_get(_t1996, gem_string("expr"))};
                GemVal _t1998 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1991, 1), gem_to_string_fn(NULL, _t1993, 1)), gem_to_string_fn(NULL, _t1994, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1995, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1997, 1)), gem_string(";"));
                gem_pop_frame();
                return _t1998;
            }
#line 875 "compiler/main.gem"
    GemVal _t1999[] = {gem_v_line_dir};
    GemVal _t2000 = gem_v_r;
    GemVal _t2001[] = {gem_table_get(_t2000, gem_string("setup"))};
    GemVal _t2002[] = {gem_v_p};
    GemVal _t2003[] = {gem_v_mname};
    GemVal _t2004 = gem_v_r;
    GemVal _t2005[] = {gem_table_get(_t2004, gem_string("expr"))};
            GemVal _t2006 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1999, 1), gem_to_string_fn(NULL, _t2001, 1)), gem_to_string_fn(NULL, _t2002, 1)), gem_to_string_fn(NULL, _t2003, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2005, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2006;
        } else {
#line 876 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
#line 877 "compiler/main.gem"
    GemVal _t2007 = gem_v_node;
    GemVal _t2008 = (*gem_v_compile_expr);
    GemVal _t2009[] = {gem_table_get(_t2007, gem_string("object"))};
                GemVal gem_v_obj_r = _t2008.fn(_t2008.env, _t2009, 1);
#line 878 "compiler/main.gem"
    GemVal _t2010 = gem_v_node;
    GemVal _t2011 = (*gem_v_compile_expr);
    GemVal _t2012[] = {gem_table_get(_t2010, gem_string("value"))};
                GemVal gem_v_val_r = _t2011.fn(_t2011.env, _t2012, 1);
#line 879 "compiler/main.gem"
    GemVal _t2013 = gem_v_node;
    GemVal _t2014[] = {gem_table_get(_t2013, gem_string("field"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2014, 1);
#line 880 "compiler/main.gem"
    GemVal _t2015[] = {gem_v_line_dir};
    GemVal _t2016 = gem_v_obj_r;
    GemVal _t2017[] = {gem_table_get(_t2016, gem_string("setup"))};
    GemVal _t2018 = gem_v_val_r;
    GemVal _t2019[] = {gem_table_get(_t2018, gem_string("setup"))};
    GemVal _t2020[] = {gem_v_p};
    GemVal _t2021 = gem_v_obj_r;
    GemVal _t2022[] = {gem_table_get(_t2021, gem_string("expr"))};
    GemVal _t2023[] = {gem_v_escaped};
    GemVal _t2024 = gem_v_val_r;
    GemVal _t2025[] = {gem_table_get(_t2024, gem_string("expr"))};
                GemVal _t2026 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2015, 1), gem_to_string_fn(NULL, _t2017, 1)), gem_to_string_fn(NULL, _t2019, 1)), gem_to_string_fn(NULL, _t2020, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2022, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2023, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2025, 1)), gem_string(");"));
                gem_pop_frame();
                return _t2026;
            } else {
#line 881 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("index_assign")))) {
#line 882 "compiler/main.gem"
    GemVal _t2027 = gem_v_node;
    GemVal _t2028 = (*gem_v_compile_expr);
    GemVal _t2029[] = {gem_table_get(_t2027, gem_string("object"))};
                    GemVal gem_v_obj_r = _t2028.fn(_t2028.env, _t2029, 1);
#line 883 "compiler/main.gem"
    GemVal _t2030 = gem_v_node;
    GemVal _t2031 = (*gem_v_compile_expr);
    GemVal _t2032[] = {gem_table_get(_t2030, gem_string("key"))};
                    GemVal gem_v_key_r = _t2031.fn(_t2031.env, _t2032, 1);
#line 884 "compiler/main.gem"
    GemVal _t2033 = gem_v_node;
    GemVal _t2034 = (*gem_v_compile_expr);
    GemVal _t2035[] = {gem_table_get(_t2033, gem_string("value"))};
                    GemVal gem_v_val_r = _t2034.fn(_t2034.env, _t2035, 1);
#line 885 "compiler/main.gem"
    GemVal _t2036[] = {gem_v_line_dir};
    GemVal _t2037 = gem_v_obj_r;
    GemVal _t2038[] = {gem_table_get(_t2037, gem_string("setup"))};
    GemVal _t2039 = gem_v_key_r;
    GemVal _t2040[] = {gem_table_get(_t2039, gem_string("setup"))};
    GemVal _t2041 = gem_v_val_r;
    GemVal _t2042[] = {gem_table_get(_t2041, gem_string("setup"))};
    GemVal _t2043[] = {gem_v_p};
    GemVal _t2044 = gem_v_obj_r;
    GemVal _t2045[] = {gem_table_get(_t2044, gem_string("expr"))};
    GemVal _t2046 = gem_v_key_r;
    GemVal _t2047[] = {gem_table_get(_t2046, gem_string("expr"))};
    GemVal _t2048 = gem_v_val_r;
    GemVal _t2049[] = {gem_table_get(_t2048, gem_string("expr"))};
                    GemVal _t2050 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2036, 1), gem_to_string_fn(NULL, _t2038, 1)), gem_to_string_fn(NULL, _t2040, 1)), gem_to_string_fn(NULL, _t2042, 1)), gem_to_string_fn(NULL, _t2043, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2045, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2047, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2049, 1)), gem_string(");"));
                    gem_pop_frame();
                    return _t2050;
                } else {
#line 886 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 887 "compiler/main.gem"
    GemVal _t2051[] = {gem_v_line_dir};
    GemVal _t2052 = (*gem_v_compile_if);
    GemVal _t2053[] = {gem_v_node, gem_v_indent};
                        GemVal _t2054 = gem_add(gem_to_string_fn(NULL, _t2051, 1), _t2052.fn(_t2052.env, _t2053, 2));
                        gem_pop_frame();
                        return _t2054;
                    } else {
#line 888 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 889 "compiler/main.gem"
    GemVal _t2055[] = {gem_v_line_dir};
    GemVal _t2056 = (*gem_v_compile_while);
    GemVal _t2057[] = {gem_v_node, gem_v_indent};
                            GemVal _t2058 = gem_add(gem_to_string_fn(NULL, _t2055, 1), _t2056.fn(_t2056.env, _t2057, 2));
                            gem_pop_frame();
                            return _t2058;
                        } else {
#line 890 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 891 "compiler/main.gem"
    GemVal _t2059[] = {gem_v_line_dir};
    GemVal _t2060 = (*gem_v_compile_match);
    GemVal _t2061[] = {gem_v_node, gem_v_indent};
                                GemVal _t2062 = gem_add(gem_to_string_fn(NULL, _t2059, 1), _t2060.fn(_t2060.env, _t2061, 2));
                                gem_pop_frame();
                                return _t2062;
                            } else {
#line 892 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("break")))) {
#line 893 "compiler/main.gem"
                                    GemVal _t2063 = gem_add(gem_v_p, gem_string("break;"));
                                    gem_pop_frame();
                                    return _t2063;
                                } else {
#line 894 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("continue")))) {
#line 895 "compiler/main.gem"
                                        GemVal _t2064 = gem_add(gem_v_p, gem_string("continue;"));
                                        gem_pop_frame();
                                        return _t2064;
                                    } else {
#line 896 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 897 "compiler/main.gem"
    GemVal _t2065 = gem_v_node;
                                            if (gem_truthy(gem_neq(gem_table_get(_t2065, gem_string("value")), GEM_NIL))) {
#line 898 "compiler/main.gem"
    GemVal _t2066 = gem_v_node;
    GemVal _t2067 = (*gem_v_compile_expr);
    GemVal _t2068[] = {gem_table_get(_t2066, gem_string("value"))};
                                                GemVal gem_v_r = _t2067.fn(_t2067.env, _t2068, 1);
#line 899 "compiler/main.gem"
    GemVal _t2069 = (*gem_v_tmp);
                                                GemVal gem_v_t = _t2069.fn(_t2069.env, NULL, 0);
#line 900 "compiler/main.gem"
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
#line 902 "compiler/main.gem"
    GemVal _t2081[] = {gem_v_line_dir};
    GemVal _t2082[] = {gem_v_p};
    GemVal _t2083[] = {gem_v_p};
                                            GemVal _t2084 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2081, 1), gem_to_string_fn(NULL, _t2082, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2083, 1)), gem_string("return GEM_NIL;"));
                                            gem_pop_frame();
                                            return _t2084;
                                        } else {
#line 903 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 904 "compiler/main.gem"
    GemVal _t2085 = (*gem_v_compile_expr);
    GemVal _t2086[] = {gem_v_node};
                                                GemVal gem_v_r = _t2085.fn(_t2085.env, _t2086, 1);
#line 905 "compiler/main.gem"
    GemVal _t2087[] = {gem_v_line_dir};
    GemVal _t2088 = gem_v_r;
    GemVal _t2089[] = {gem_table_get(_t2088, gem_string("setup"))};
    GemVal _t2090[] = {gem_v_p};
    GemVal _t2091 = gem_v_r;
    GemVal _t2092[] = {gem_table_get(_t2091, gem_string("expr"))};
                                                GemVal _t2093 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2087, 1), gem_to_string_fn(NULL, _t2089, 1)), gem_to_string_fn(NULL, _t2090, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2092, 1)), gem_string(");"));
                                                gem_pop_frame();
                                                return _t2093;
                                            } else {
#line 906 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
#line 907 "compiler/main.gem"
                                                    GemVal _t2094 = gem_string("");
                                                    gem_pop_frame();
                                                    return _t2094;
                                                } else {
#line 908 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 909 "compiler/main.gem"
                                                        GemVal gem_v_out = gem_string("");
#line 910 "compiler/main.gem"
    GemVal _t2095 = gem_v_node;
                                                        GemVal gem_v__for_items_38 = gem_table_get(_t2095, gem_string("stmts"));
#line 910 "compiler/main.gem"
                                                        GemVal gem_v__for_i_38 = gem_int(0);
#line 910 "compiler/main.gem"
                                                        while (1) {
                                                            GemVal _t2096[] = {gem_v__for_items_38};
                                                            if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_len_fn(NULL, _t2096, 1)))) break;
#line 910 "compiler/main.gem"
                                                            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_38, gem_v__for_i_38);
#line 910 "compiler/main.gem"
                                                            gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 911 "compiler/main.gem"
    GemVal _t2097 = (*gem_v_compile_stmt);
    GemVal _t2098[] = {gem_v_stmt, gem_v_indent};
                                                            gem_v_out = gem_add(gem_add(gem_v_out, _t2097.fn(_t2097.env, _t2098, 2)), gem_string("\n"));
                                                        }

#line 913 "compiler/main.gem"
                                                        GemVal _t2099 = gem_v_out;
                                                        gem_pop_frame();
                                                        return _t2099;
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
#line 916 "compiler/main.gem"
    GemVal _t2100 = (*gem_v_compile_expr);
    GemVal _t2101[] = {gem_v_node};
    GemVal gem_v_r = _t2100.fn(_t2100.env, _t2101, 1);
    GemVal _t2102[] = {gem_v_line_dir};
    GemVal _t2103 = gem_v_r;
    GemVal _t2104[] = {gem_table_get(_t2103, gem_string("setup"))};
    GemVal _t2105[] = {gem_v_p};
    GemVal _t2106 = gem_v_r;
    GemVal _t2107[] = {gem_table_get(_t2106, gem_string("expr"))};
    GemVal _t2108 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2102, 1), gem_to_string_fn(NULL, _t2104, 1)), gem_to_string_fn(NULL, _t2105, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2107, 1)), gem_string(");"));
    gem_pop_frame();
    return _t2108;
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
#line 923 "compiler/main.gem"
    GemVal _t2110[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2110, 1);
#line 924 "compiler/main.gem"
    GemVal _t2111 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t2111, gem_string("tag"));
#line 925 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 926 "compiler/main.gem"
    GemVal _t2112 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2112, gem_string("line")), GEM_NIL))) {
#line 927 "compiler/main.gem"
    GemVal _t2113 = gem_v_node;
    GemVal _t2114[] = {gem_table_get(_t2113, gem_string("line"))};
    GemVal _t2115[] = {(*gem_v_source_name)};
    GemVal _t2116[] = {gem_fn_escape_c_string(NULL, _t2115, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2114, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2116, 1)), gem_string("\"\n"));
    }
#line 930 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 931 "compiler/main.gem"
    GemVal _t2117 = gem_v_node;
    GemVal _t2118 = (*gem_v_compile_expr);
    GemVal _t2119[] = {gem_table_get(_t2117, gem_string("value"))};
        GemVal gem_v_r = _t2118.fn(_t2118.env, _t2119, 1);
#line 932 "compiler/main.gem"
    GemVal _t2120 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t2120, gem_string("name"));
#line 933 "compiler/main.gem"
    GemVal _t2121 = (*gem_v_mangle);
    GemVal _t2122[] = {gem_v_name};
        GemVal gem_v_mname = _t2121.fn(_t2121.env, _t2122, 1);
#line 934 "compiler/main.gem"
    GemVal _t2123[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2123, 2))) {
#line 935 "compiler/main.gem"
    GemVal _t2124[] = {gem_v_line_dir};
    GemVal _t2125 = gem_v_r;
    GemVal _t2126[] = {gem_table_get(_t2125, gem_string("setup"))};
    GemVal _t2127[] = {gem_v_p};
    GemVal _t2128[] = {gem_v_mname};
    GemVal _t2129[] = {gem_v_p};
    GemVal _t2130[] = {gem_v_mname};
    GemVal _t2131 = gem_v_r;
    GemVal _t2132[] = {gem_table_get(_t2131, gem_string("expr"))};
    GemVal _t2133[] = {gem_v_p};
    GemVal _t2134[] = {gem_v_p};
            GemVal _t2135 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2124, 1), gem_to_string_fn(NULL, _t2126, 1)), gem_to_string_fn(NULL, _t2127, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2128, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2129, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2130, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2132, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2133, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2134, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t2135;
        }
#line 937 "compiler/main.gem"
    GemVal _t2136[] = {gem_v_line_dir};
    GemVal _t2137 = gem_v_r;
    GemVal _t2138[] = {gem_table_get(_t2137, gem_string("setup"))};
    GemVal _t2139[] = {gem_v_p};
    GemVal _t2140[] = {gem_v_mname};
    GemVal _t2141 = gem_v_r;
    GemVal _t2142[] = {gem_table_get(_t2141, gem_string("expr"))};
    GemVal _t2143[] = {gem_v_p};
    GemVal _t2144[] = {gem_v_p};
        GemVal _t2145 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2136, 1), gem_to_string_fn(NULL, _t2138, 1)), gem_to_string_fn(NULL, _t2139, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2140, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2142, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2143, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2144, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2145;
    } else {
#line 938 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 939 "compiler/main.gem"
    GemVal _t2146 = gem_v_node;
    GemVal _t2147 = (*gem_v_compile_expr);
    GemVal _t2148[] = {gem_table_get(_t2146, gem_string("value"))};
            GemVal gem_v_r = _t2147.fn(_t2147.env, _t2148, 1);
#line 940 "compiler/main.gem"
    GemVal _t2149 = (*gem_v_tmp);
            GemVal gem_v_t = _t2149.fn(_t2149.env, NULL, 0);
#line 941 "compiler/main.gem"
    GemVal _t2150 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t2150, gem_string("name"));
#line 942 "compiler/main.gem"
    GemVal _t2151 = (*gem_v_mangle);
    GemVal _t2152[] = {gem_v_name};
            GemVal gem_v_mname = _t2151.fn(_t2151.env, _t2152, 1);
#line 943 "compiler/main.gem"
    GemVal _t2153[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2155;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2153, 2))) {
        _t2155 = gem_fn_set_contains(NULL, _t2153, 2);
    } else {
        GemVal _t2154[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2155 = gem_fn_set_contains(NULL, _t2154, 2);
    }
            if (gem_truthy(_t2155)) {
#line 944 "compiler/main.gem"
    GemVal _t2156[] = {gem_v_line_dir};
    GemVal _t2157 = gem_v_r;
    GemVal _t2158[] = {gem_table_get(_t2157, gem_string("setup"))};
    GemVal _t2159[] = {gem_v_p};
    GemVal _t2160[] = {gem_v_t};
    GemVal _t2161 = gem_v_r;
    GemVal _t2162[] = {gem_table_get(_t2161, gem_string("expr"))};
    GemVal _t2163[] = {gem_v_p};
    GemVal _t2164[] = {gem_v_mname};
    GemVal _t2165[] = {gem_v_t};
    GemVal _t2166[] = {gem_v_p};
    GemVal _t2167[] = {gem_v_p};
    GemVal _t2168[] = {gem_v_t};
                GemVal _t2169 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2156, 1), gem_to_string_fn(NULL, _t2158, 1)), gem_to_string_fn(NULL, _t2159, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2160, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2162, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2163, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2164, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2165, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2166, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2167, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2168, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2169;
            }
#line 946 "compiler/main.gem"
    GemVal _t2170[] = {gem_v_line_dir};
    GemVal _t2171 = gem_v_r;
    GemVal _t2172[] = {gem_table_get(_t2171, gem_string("setup"))};
    GemVal _t2173[] = {gem_v_p};
    GemVal _t2174[] = {gem_v_t};
    GemVal _t2175 = gem_v_r;
    GemVal _t2176[] = {gem_table_get(_t2175, gem_string("expr"))};
    GemVal _t2177[] = {gem_v_p};
    GemVal _t2178[] = {gem_v_mname};
    GemVal _t2179[] = {gem_v_t};
    GemVal _t2180[] = {gem_v_p};
    GemVal _t2181[] = {gem_v_p};
    GemVal _t2182[] = {gem_v_t};
            GemVal _t2183 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2170, 1), gem_to_string_fn(NULL, _t2172, 1)), gem_to_string_fn(NULL, _t2173, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2174, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2176, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2177, 1)), gem_to_string_fn(NULL, _t2178, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2179, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2180, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2181, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2182, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2183;
        } else {
#line 947 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 948 "compiler/main.gem"
    GemVal _t2184 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t2184, gem_string("value")), GEM_NIL))) {
#line 949 "compiler/main.gem"
    GemVal _t2185 = gem_v_node;
    GemVal _t2186 = (*gem_v_compile_expr);
    GemVal _t2187[] = {gem_table_get(_t2185, gem_string("value"))};
                    GemVal gem_v_r = _t2186.fn(_t2186.env, _t2187, 1);
#line 950 "compiler/main.gem"
    GemVal _t2188 = (*gem_v_tmp);
                    GemVal gem_v_t = _t2188.fn(_t2188.env, NULL, 0);
#line 951 "compiler/main.gem"
    GemVal _t2189[] = {gem_v_line_dir};
    GemVal _t2190 = gem_v_r;
    GemVal _t2191[] = {gem_table_get(_t2190, gem_string("setup"))};
    GemVal _t2192[] = {gem_v_p};
    GemVal _t2193[] = {gem_v_t};
    GemVal _t2194 = gem_v_r;
    GemVal _t2195[] = {gem_table_get(_t2194, gem_string("expr"))};
    GemVal _t2196[] = {gem_v_p};
    GemVal _t2197[] = {gem_v_p};
    GemVal _t2198[] = {gem_v_t};
                    GemVal _t2199 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2189, 1), gem_to_string_fn(NULL, _t2191, 1)), gem_to_string_fn(NULL, _t2192, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2193, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2195, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2196, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2197, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2198, 1)), gem_string(";"));
                    gem_pop_frame();
                    return _t2199;
                }
#line 953 "compiler/main.gem"
    GemVal _t2200[] = {gem_v_line_dir};
    GemVal _t2201[] = {gem_v_p};
    GemVal _t2202[] = {gem_v_p};
                GemVal _t2203 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2200, 1), gem_to_string_fn(NULL, _t2201, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2202, 1)), gem_string("return GEM_NIL;"));
                gem_pop_frame();
                return _t2203;
            } else {
#line 954 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 955 "compiler/main.gem"
    GemVal _t2204[] = {gem_v_line_dir};
    GemVal _t2205 = (*gem_v_compile_if_return);
    GemVal _t2206[] = {gem_v_node, gem_v_indent};
                    GemVal _t2207 = gem_add(gem_to_string_fn(NULL, _t2204, 1), _t2205.fn(_t2205.env, _t2206, 2));
                    gem_pop_frame();
                    return _t2207;
                } else {
#line 956 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 957 "compiler/main.gem"
    GemVal _t2208[] = {gem_v_line_dir};
    GemVal _t2209 = (*gem_v_compile_while);
    GemVal _t2210[] = {gem_v_node, gem_v_indent};
    GemVal _t2211[] = {_t2209.fn(_t2209.env, _t2210, 2)};
    GemVal _t2212[] = {gem_v_p};
    GemVal _t2213[] = {gem_v_p};
                        GemVal _t2214 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2208, 1), gem_to_string_fn(NULL, _t2211, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t2212, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2213, 1)), gem_string("return GEM_NIL;"));
                        gem_pop_frame();
                        return _t2214;
                    } else {
#line 958 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 959 "compiler/main.gem"
    GemVal _t2215[] = {gem_v_line_dir};
    GemVal _t2216 = (*gem_v_compile_match_return);
    GemVal _t2217[] = {gem_v_node, gem_v_indent};
                            GemVal _t2218 = gem_add(gem_to_string_fn(NULL, _t2215, 1), _t2216.fn(_t2216.env, _t2217, 2));
                            gem_pop_frame();
                            return _t2218;
                        } else {
#line 960 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 961 "compiler/main.gem"
    GemVal _t2219[] = {gem_v_p};
                                GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2219, 1), gem_string("{\n"));
#line 962 "compiler/main.gem"
                                GemVal gem_v__for_i_39 = gem_int(0);
#line 962 "compiler/main.gem"
    GemVal _t2220 = gem_v_node;
    GemVal _t2221[] = {gem_table_get(_t2220, gem_string("stmts"))};
                                GemVal gem_v__for_limit_39 = gem_sub(gem_len_fn(NULL, _t2221, 1), gem_int(1));
#line 962 "compiler/main.gem"
                                while (1) {
                                    if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
#line 962 "compiler/main.gem"
                                    GemVal gem_v_i = gem_v__for_i_39;
#line 962 "compiler/main.gem"
                                    gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 963 "compiler/main.gem"
    GemVal _t2222 = gem_v_node;
    GemVal _t2223 = (*gem_v_compile_stmt);
    GemVal _t2224[] = {gem_table_get(gem_table_get(_t2222, gem_string("stmts")), gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t2223.fn(_t2223.env, _t2224, 2)), gem_string("\n"));
                                }

#line 965 "compiler/main.gem"
    GemVal _t2225 = gem_v_node;
    GemVal _t2226[] = {gem_table_get(_t2225, gem_string("stmts"))};
                                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2226, 1), gem_int(0)))) {
#line 966 "compiler/main.gem"
    GemVal _t2227 = gem_v_node;
    GemVal _t2228 = gem_v_node;
    GemVal _t2229[] = {gem_table_get(_t2228, gem_string("stmts"))};
    GemVal _t2230 = (*gem_v_compile_stmt_return);
    GemVal _t2231[] = {gem_table_get(gem_table_get(_t2227, gem_string("stmts")), gem_sub(gem_len_fn(NULL, _t2229, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t2230.fn(_t2230.env, _t2231, 2)), gem_string("\n"));
                                }
#line 968 "compiler/main.gem"
    GemVal _t2232[] = {gem_v_p};
                                gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2232, 1), gem_string("}")));
#line 969 "compiler/main.gem"
                                GemVal _t2233 = gem_v_out;
                                gem_pop_frame();
                                return _t2233;
                            } else {
#line 970 "compiler/main.gem"
    GemVal _t2234;
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
        _t2234 = gem_eq(gem_v_tag, gem_string("dot_assign"));
    } else {
        _t2234 = gem_eq(gem_v_tag, gem_string("index_assign"));
    }
    GemVal _t2235;
    if (gem_truthy(_t2234)) {
        _t2235 = _t2234;
    } else {
        _t2235 = gem_eq(gem_v_tag, gem_string("break"));
    }
    GemVal _t2236;
    if (gem_truthy(_t2235)) {
        _t2236 = _t2235;
    } else {
        _t2236 = gem_eq(gem_v_tag, gem_string("continue"));
    }
    GemVal _t2237;
    if (gem_truthy(_t2236)) {
        _t2237 = _t2236;
    } else {
        _t2237 = gem_eq(gem_v_tag, gem_string("fn_def"));
    }
                                if (gem_truthy(_t2237)) {
#line 971 "compiler/main.gem"
    GemVal _t2238 = (*gem_v_compile_stmt);
    GemVal _t2239[] = {gem_v_node, gem_v_indent};
    GemVal _t2240[] = {_t2238.fn(_t2238.env, _t2239, 2)};
    GemVal _t2241[] = {gem_v_p};
    GemVal _t2242[] = {gem_v_p};
                                    GemVal _t2243 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2240, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2241, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2242, 1)), gem_string("return GEM_NIL;"));
                                    gem_pop_frame();
                                    return _t2243;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 974 "compiler/main.gem"
    GemVal _t2244 = (*gem_v_compile_expr);
    GemVal _t2245[] = {gem_v_node};
    GemVal gem_v_r = _t2244.fn(_t2244.env, _t2245, 1);
#line 975 "compiler/main.gem"
    GemVal _t2246 = (*gem_v_tmp);
    GemVal gem_v_t = _t2246.fn(_t2246.env, NULL, 0);
    GemVal _t2247[] = {gem_v_line_dir};
    GemVal _t2248 = gem_v_r;
    GemVal _t2249[] = {gem_table_get(_t2248, gem_string("setup"))};
    GemVal _t2250[] = {gem_v_p};
    GemVal _t2251[] = {gem_v_t};
    GemVal _t2252 = gem_v_r;
    GemVal _t2253[] = {gem_table_get(_t2252, gem_string("expr"))};
    GemVal _t2254[] = {gem_v_p};
    GemVal _t2255[] = {gem_v_p};
    GemVal _t2256[] = {gem_v_t};
    GemVal _t2257 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2247, 1), gem_to_string_fn(NULL, _t2249, 1)), gem_to_string_fn(NULL, _t2250, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2251, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2253, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2254, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2255, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2256, 1)), gem_string(";"));
    gem_pop_frame();
    return _t2257;
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
#line 982 "compiler/main.gem"
    GemVal _t2259[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2259, 1);
#line 983 "compiler/main.gem"
    GemVal _t2260 = gem_v_node;
    GemVal _t2261 = (*gem_v_compile_expr);
    GemVal _t2262[] = {gem_table_get(_t2260, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2261.fn(_t2261.env, _t2262, 1);
#line 984 "compiler/main.gem"
    GemVal _t2263 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2263, gem_string("setup"));
#line 985 "compiler/main.gem"
    GemVal _t2264[] = {gem_v_p};
    GemVal _t2265 = gem_v_cond_r;
    GemVal _t2266[] = {gem_table_get(_t2265, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2264, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2266, 1)), gem_string(")) {\n")));
#line 986 "compiler/main.gem"
    GemVal _t2267 = gem_v_node;
    GemVal _t2268 = (*gem_v_compile_stmts);
    GemVal _t2269[] = {gem_table_get(_t2267, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t2268.fn(_t2268.env, _t2269, 2));
#line 987 "compiler/main.gem"
    GemVal _t2270 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2270, gem_string("else")), GEM_NIL))) {
#line 988 "compiler/main.gem"
    GemVal _t2271[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2271, 1), gem_string("} else {\n")));
#line 989 "compiler/main.gem"
    GemVal _t2272 = gem_v_node;
    GemVal _t2273 = (*gem_v_compile_stmts);
    GemVal _t2274[] = {gem_table_get(_t2272, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t2273.fn(_t2273.env, _t2274, 2));
    }
#line 991 "compiler/main.gem"
    GemVal _t2275[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2275, 1), gem_string("}")));
    GemVal _t2276 = gem_v_out;
    gem_pop_frame();
    return _t2276;
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
#line 998 "compiler/main.gem"
    GemVal _t2278[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2278, 1);
#line 999 "compiler/main.gem"
    GemVal _t2279 = gem_v_node;
    GemVal _t2280 = (*gem_v_compile_expr);
    GemVal _t2281[] = {gem_table_get(_t2279, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2280.fn(_t2280.env, _t2281, 1);
#line 1000 "compiler/main.gem"
    GemVal _t2282 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2282, gem_string("setup"));
#line 1001 "compiler/main.gem"
    GemVal _t2283[] = {gem_v_p};
    GemVal _t2284 = gem_v_cond_r;
    GemVal _t2285[] = {gem_table_get(_t2284, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2283, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2285, 1)), gem_string(")) {\n")));
#line 1002 "compiler/main.gem"
    GemVal _t2286 = gem_v_node;
    GemVal gem_v_then_body = gem_table_get(_t2286, gem_string("then"));
#line 1003 "compiler/main.gem"
    GemVal _t2287[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2287, 1), gem_int(0)))) {
#line 1004 "compiler/main.gem"
        GemVal gem_v__for_i_40 = gem_int(0);
#line 1004 "compiler/main.gem"
    GemVal _t2288[] = {gem_v_then_body};
        GemVal gem_v__for_limit_40 = gem_sub(gem_len_fn(NULL, _t2288, 1), gem_int(1));
#line 1004 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 1004 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_40;
#line 1004 "compiler/main.gem"
            gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 1005 "compiler/main.gem"
    GemVal _t2289 = (*gem_v_compile_stmt);
    GemVal _t2290[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t2289.fn(_t2289.env, _t2290, 2)), gem_string("\n"));
        }

#line 1007 "compiler/main.gem"
    GemVal _t2291[] = {gem_v_then_body};
    GemVal _t2292 = (*gem_v_compile_stmt_return);
    GemVal _t2293[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t2291, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_add(gem_v_out, _t2292.fn(_t2292.env, _t2293, 2)), gem_string("\n"));
    } else {
#line 1009 "compiler/main.gem"
    GemVal _t2294[] = {gem_v_p};
    GemVal _t2295[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2294, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2295, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1011 "compiler/main.gem"
    GemVal _t2296 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2296, gem_string("else")), GEM_NIL))) {
#line 1012 "compiler/main.gem"
    GemVal _t2297[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2297, 1), gem_string("} else {\n")));
#line 1013 "compiler/main.gem"
    GemVal _t2298 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t2298, gem_string("else"));
#line 1014 "compiler/main.gem"
    GemVal _t2299[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2299, 1), gem_int(0)))) {
#line 1015 "compiler/main.gem"
            GemVal gem_v__for_i_41 = gem_int(0);
#line 1015 "compiler/main.gem"
    GemVal _t2300[] = {gem_v_else_body};
            GemVal gem_v__for_limit_41 = gem_sub(gem_len_fn(NULL, _t2300, 1), gem_int(1));
#line 1015 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_limit_41))) break;
#line 1015 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_41;
#line 1015 "compiler/main.gem"
                gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
#line 1016 "compiler/main.gem"
    GemVal _t2301 = (*gem_v_compile_stmt);
    GemVal _t2302[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t2301.fn(_t2301.env, _t2302, 2)), gem_string("\n"));
            }

#line 1018 "compiler/main.gem"
    GemVal _t2303[] = {gem_v_else_body};
    GemVal _t2304 = (*gem_v_compile_stmt_return);
    GemVal _t2305[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2303, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t2304.fn(_t2304.env, _t2305, 2)), gem_string("\n"));
        } else {
#line 1020 "compiler/main.gem"
    GemVal _t2306[] = {gem_v_p};
    GemVal _t2307[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2306, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2307, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1023 "compiler/main.gem"
    GemVal _t2308[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2308, 1), gem_string("} else {\n")));
#line 1024 "compiler/main.gem"
    GemVal _t2309[] = {gem_v_p};
    GemVal _t2310[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2309, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2310, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1026 "compiler/main.gem"
    GemVal _t2311[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2311, 1), gem_string("}")));
    GemVal _t2312 = gem_v_out;
    gem_pop_frame();
    return _t2312;
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
#line 1033 "compiler/main.gem"
    GemVal _t2314[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2314, 1);
#line 1034 "compiler/main.gem"
    GemVal _t2315 = gem_v_node;
    GemVal _t2316 = (*gem_v_compile_expr);
    GemVal _t2317[] = {gem_table_get(_t2315, gem_string("cond"))};
    GemVal gem_v_cond_r = _t2316.fn(_t2316.env, _t2317, 1);
#line 1035 "compiler/main.gem"
    GemVal _t2318[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2318, 1), gem_string("while (1) {\n"));
#line 1036 "compiler/main.gem"
    GemVal _t2319 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t2319, gem_string("setup")), gem_string("")))) {
#line 1037 "compiler/main.gem"
    GemVal _t2320 = gem_v_cond_r;
    GemVal _t2321[] = {gem_v_p};
    GemVal _t2322[] = {gem_table_get(_t2320, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t2321, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_str_replace_fn(NULL, _t2322, 3));
    }
#line 1039 "compiler/main.gem"
    GemVal _t2323[] = {gem_v_p};
    GemVal _t2324 = gem_v_cond_r;
    GemVal _t2325[] = {gem_table_get(_t2324, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2323, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t2325, 1)), gem_string(")) break;\n")));
#line 1040 "compiler/main.gem"
    GemVal _t2326 = gem_v_node;
    GemVal _t2327 = (*gem_v_compile_stmts);
    GemVal _t2328[] = {gem_table_get(_t2326, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t2327.fn(_t2327.env, _t2328, 2));
#line 1041 "compiler/main.gem"
    GemVal _t2329[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2329, 1), gem_string("}")));
    GemVal _t2330 = gem_v_out;
    gem_pop_frame();
    return _t2330;
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
#line 1048 "compiler/main.gem"
    GemVal _t2332[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2332, 1);
#line 1049 "compiler/main.gem"
    GemVal _t2333 = gem_v_node;
    GemVal _t2334 = (*gem_v_compile_expr);
    GemVal _t2335[] = {gem_table_get(_t2333, gem_string("target"))};
    GemVal gem_v_target_r = _t2334.fn(_t2334.env, _t2335, 1);
#line 1050 "compiler/main.gem"
    GemVal _t2336 = (*gem_v_tmp);
    GemVal gem_v_t = _t2336.fn(_t2336.env, NULL, 0);
#line 1051 "compiler/main.gem"
    GemVal _t2337 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2337, gem_string("setup"));
#line 1052 "compiler/main.gem"
    GemVal _t2338[] = {gem_v_p};
    GemVal _t2339[] = {gem_v_t};
    GemVal _t2340 = gem_v_target_r;
    GemVal _t2341[] = {gem_table_get(_t2340, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2338, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2339, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2341, 1)), gem_string(";\n")));
#line 1053 "compiler/main.gem"
    GemVal gem_v__for_i_42 = gem_int(0);
#line 1053 "compiler/main.gem"
    GemVal _t2342 = gem_v_node;
    GemVal _t2343[] = {gem_table_get(_t2342, gem_string("whens"))};
    GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2343, 1);
#line 1053 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 1053 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_42;
#line 1053 "compiler/main.gem"
        gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 1054 "compiler/main.gem"
    GemVal _t2344 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t2344, gem_string("whens")), gem_v_i);
#line 1055 "compiler/main.gem"
    GemVal _t2345 = gem_v_w;
    GemVal _t2346 = (*gem_v_compile_expr);
    GemVal _t2347[] = {gem_table_get(_t2345, gem_string("value"))};
        GemVal gem_v_val_r = _t2346.fn(_t2346.env, _t2347, 1);
#line 1056 "compiler/main.gem"
        GemVal gem_v_keyword = gem_string("if");
#line 1057 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1058 "compiler/main.gem"
            gem_v_keyword = gem_string("} else if");
        }
#line 1060 "compiler/main.gem"
    GemVal _t2348 = gem_v_val_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t2348, gem_string("setup")));
#line 1061 "compiler/main.gem"
    GemVal _t2349[] = {gem_v_p};
    GemVal _t2350[] = {gem_v_keyword};
    GemVal _t2351[] = {gem_v_t};
    GemVal _t2352 = gem_v_val_r;
    GemVal _t2353[] = {gem_table_get(_t2352, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2349, 1), gem_to_string_fn(NULL, _t2350, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2351, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2353, 1)), gem_string("))) {\n")));
#line 1062 "compiler/main.gem"
    GemVal _t2354 = gem_v_w;
    GemVal _t2355 = (*gem_v_compile_stmts);
    GemVal _t2356[] = {gem_table_get(_t2354, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t2355.fn(_t2355.env, _t2356, 2));
    }

#line 1064 "compiler/main.gem"
    GemVal _t2357 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2357, gem_string("else")), GEM_NIL))) {
#line 1065 "compiler/main.gem"
    GemVal _t2358[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2358, 1), gem_string("} else {\n")));
#line 1066 "compiler/main.gem"
    GemVal _t2359 = gem_v_node;
    GemVal _t2360 = (*gem_v_compile_stmts);
    GemVal _t2361[] = {gem_table_get(_t2359, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t2360.fn(_t2360.env, _t2361, 2));
    }
#line 1068 "compiler/main.gem"
    GemVal _t2362[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2362, 1), gem_string("}")));
    GemVal _t2363 = gem_v_out;
    gem_pop_frame();
    return _t2363;
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
#line 1075 "compiler/main.gem"
    GemVal _t2365[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2365, 1);
#line 1076 "compiler/main.gem"
    GemVal _t2366 = gem_v_node;
    GemVal _t2367 = (*gem_v_compile_expr);
    GemVal _t2368[] = {gem_table_get(_t2366, gem_string("target"))};
    GemVal gem_v_target_r = _t2367.fn(_t2367.env, _t2368, 1);
#line 1077 "compiler/main.gem"
    GemVal _t2369 = (*gem_v_tmp);
    GemVal gem_v_t = _t2369.fn(_t2369.env, NULL, 0);
#line 1078 "compiler/main.gem"
    GemVal _t2370 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2370, gem_string("setup"));
#line 1079 "compiler/main.gem"
    GemVal _t2371[] = {gem_v_p};
    GemVal _t2372[] = {gem_v_t};
    GemVal _t2373 = gem_v_target_r;
    GemVal _t2374[] = {gem_table_get(_t2373, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2371, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2372, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2374, 1)), gem_string(";\n")));
#line 1080 "compiler/main.gem"
    GemVal gem_v__for_i_44 = gem_int(0);
#line 1080 "compiler/main.gem"
    GemVal _t2375 = gem_v_node;
    GemVal _t2376[] = {gem_table_get(_t2375, gem_string("whens"))};
    GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t2376, 1);
#line 1080 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 1080 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_44;
#line 1080 "compiler/main.gem"
        gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 1081 "compiler/main.gem"
    GemVal _t2377 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t2377, gem_string("whens")), gem_v_i);
#line 1082 "compiler/main.gem"
    GemVal _t2378 = gem_v_w;
    GemVal _t2379 = (*gem_v_compile_expr);
    GemVal _t2380[] = {gem_table_get(_t2378, gem_string("value"))};
        GemVal gem_v_val_r = _t2379.fn(_t2379.env, _t2380, 1);
#line 1083 "compiler/main.gem"
        GemVal gem_v_keyword = gem_string("if");
#line 1084 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1085 "compiler/main.gem"
            gem_v_keyword = gem_string("} else if");
        }
#line 1087 "compiler/main.gem"
    GemVal _t2381 = gem_v_val_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t2381, gem_string("setup")));
#line 1088 "compiler/main.gem"
    GemVal _t2382[] = {gem_v_p};
    GemVal _t2383[] = {gem_v_keyword};
    GemVal _t2384[] = {gem_v_t};
    GemVal _t2385 = gem_v_val_r;
    GemVal _t2386[] = {gem_table_get(_t2385, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2382, 1), gem_to_string_fn(NULL, _t2383, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2384, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2386, 1)), gem_string("))) {\n")));
#line 1089 "compiler/main.gem"
    GemVal _t2387 = gem_v_w;
        GemVal gem_v_body = gem_table_get(_t2387, gem_string("body"));
#line 1090 "compiler/main.gem"
    GemVal _t2388[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2388, 1), gem_int(0)))) {
#line 1091 "compiler/main.gem"
            GemVal gem_v__for_i_43 = gem_int(0);
#line 1091 "compiler/main.gem"
    GemVal _t2389[] = {gem_v_body};
            GemVal gem_v__for_limit_43 = gem_sub(gem_len_fn(NULL, _t2389, 1), gem_int(1));
#line 1091 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 1091 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_43;
#line 1091 "compiler/main.gem"
                gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 1092 "compiler/main.gem"
    GemVal _t2390 = (*gem_v_compile_stmt);
    GemVal _t2391[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t2390.fn(_t2390.env, _t2391, 2)), gem_string("\n"));
            }

#line 1094 "compiler/main.gem"
    GemVal _t2392[] = {gem_v_body};
    GemVal _t2393 = (*gem_v_compile_stmt_return);
    GemVal _t2394[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2392, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t2393.fn(_t2393.env, _t2394, 2)), gem_string("\n"));
        } else {
#line 1096 "compiler/main.gem"
    GemVal _t2395[] = {gem_v_p};
    GemVal _t2396[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2395, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2396, 1)), gem_string("    return GEM_NIL;\n")));
        }
    }

#line 1099 "compiler/main.gem"
    GemVal _t2397 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2397, gem_string("else")), GEM_NIL))) {
#line 1100 "compiler/main.gem"
    GemVal _t2398[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2398, 1), gem_string("} else {\n")));
#line 1101 "compiler/main.gem"
    GemVal _t2399 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t2399, gem_string("else"));
#line 1102 "compiler/main.gem"
    GemVal _t2400[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2400, 1), gem_int(0)))) {
#line 1103 "compiler/main.gem"
            GemVal gem_v__for_i_45 = gem_int(0);
#line 1103 "compiler/main.gem"
    GemVal _t2401[] = {gem_v_else_body};
            GemVal gem_v__for_limit_45 = gem_sub(gem_len_fn(NULL, _t2401, 1), gem_int(1));
#line 1103 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 1103 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_45;
#line 1103 "compiler/main.gem"
                gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 1104 "compiler/main.gem"
    GemVal _t2402 = (*gem_v_compile_stmt);
    GemVal _t2403[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t2402.fn(_t2402.env, _t2403, 2)), gem_string("\n"));
            }

#line 1106 "compiler/main.gem"
    GemVal _t2404[] = {gem_v_else_body};
    GemVal _t2405 = (*gem_v_compile_stmt_return);
    GemVal _t2406[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2404, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t2405.fn(_t2405.env, _t2406, 2)), gem_string("\n"));
        } else {
#line 1108 "compiler/main.gem"
    GemVal _t2407[] = {gem_v_p};
    GemVal _t2408[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2407, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2408, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1111 "compiler/main.gem"
    GemVal _t2409[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2409, 1), gem_string("} else {\n")));
#line 1112 "compiler/main.gem"
    GemVal _t2410[] = {gem_v_p};
    GemVal _t2411[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2410, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2411, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1114 "compiler/main.gem"
    GemVal _t2412[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2412, 1), gem_string("}")));
    GemVal _t2413 = gem_v_out;
    gem_pop_frame();
    return _t2413;
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
#line 1121 "compiler/main.gem"
    GemVal gem_v__d46 = gem_v_node;
#line 1121 "compiler/main.gem"
    GemVal _t2415 = gem_v__d46;
    GemVal gem_v_name = gem_table_get(_t2415, gem_string("name"));
#line 1121 "compiler/main.gem"
    GemVal _t2416 = gem_v__d46;
    GemVal gem_v_params = gem_table_get(_t2416, gem_string("params"));
#line 1121 "compiler/main.gem"
    GemVal _t2417 = gem_v__d46;
    GemVal gem_v_ret_type = gem_table_get(_t2417, gem_string("ret_type"));

#line 1123 "compiler/main.gem"
    GemVal _t2418[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2418, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1125 "compiler/main.gem"
    GemVal _t2419 = gem_table_new();
    GemVal gem_v_c_args = _t2419;
#line 1126 "compiler/main.gem"
    GemVal gem_v__for_i_47 = gem_int(0);
#line 1126 "compiler/main.gem"
    GemVal _t2420[] = {gem_v_params};
    GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2420, 1);
#line 1126 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 1126 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_47;
#line 1126 "compiler/main.gem"
        gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 1127 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1128 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1129 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1130 "compiler/main.gem"
    GemVal _t2421[] = {gem_v_i};
    GemVal _t2422[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t2421, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2422, 1)), gem_string("].ival;\n")));
#line 1131 "compiler/main.gem"
    GemVal _t2423[] = {gem_v_i};
    GemVal _t2424[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2423, 1))};
            (void)(gem_push_fn(NULL, _t2424, 2));
        } else {
#line 1132 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1133 "compiler/main.gem"
    GemVal _t2425[] = {gem_v_i};
    GemVal _t2426[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t2425, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2426, 1)), gem_string("].fval;\n")));
#line 1134 "compiler/main.gem"
    GemVal _t2427[] = {gem_v_i};
    GemVal _t2428[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2427, 1))};
                (void)(gem_push_fn(NULL, _t2428, 2));
            } else {
#line 1135 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1136 "compiler/main.gem"
    GemVal _t2429[] = {gem_v_i};
    GemVal _t2430[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t2429, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2430, 1)), gem_string("].sval;\n")));
#line 1137 "compiler/main.gem"
    GemVal _t2431[] = {gem_v_i};
    GemVal _t2432[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2431, 1))};
                    (void)(gem_push_fn(NULL, _t2432, 2));
                } else {
#line 1138 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1139 "compiler/main.gem"
    GemVal _t2433[] = {gem_v_i};
    GemVal _t2434[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t2433, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2434, 1)), gem_string("].bval;\n")));
#line 1140 "compiler/main.gem"
    GemVal _t2435[] = {gem_v_i};
    GemVal _t2436[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2435, 1))};
                        (void)(gem_push_fn(NULL, _t2436, 2));
                    } else {
#line 1141 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1142 "compiler/main.gem"
    GemVal _t2437[] = {gem_v_i};
    GemVal _t2438[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t2437, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t2438, 1)), gem_string("].ival;\n")));
#line 1143 "compiler/main.gem"
    GemVal _t2439[] = {gem_v_i};
    GemVal _t2440[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2439, 1))};
                            (void)(gem_push_fn(NULL, _t2440, 2));
                        } else {
#line 1144 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1145 "compiler/main.gem"
    GemVal _t2441[] = {gem_v_i};
    GemVal _t2442[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2441, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t2442, 2));
                            } else {
#line 1147 "compiler/main.gem"
    GemVal _t2443[] = {gem_v_i};
    GemVal _t2444[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2443, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t2444, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1152 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1153 "compiler/main.gem"
    GemVal gem_v__for_i_48 = gem_int(0);
#line 1153 "compiler/main.gem"
    GemVal _t2445[] = {gem_v_c_args};
    GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t2445, 1);
#line 1153 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 1153 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_48;
#line 1153 "compiler/main.gem"
        gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 1154 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1155 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1157 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1160 "compiler/main.gem"
    GemVal _t2446[] = {gem_v_name};
    GemVal _t2447[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2446, 1), gem_string("(")), gem_to_string_fn(NULL, _t2447, 1)), gem_string(")"));
#line 1162 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1163 "compiler/main.gem"
    GemVal _t2448[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2448, 1)), gem_string(";\n")));
#line 1164 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
#line 1165 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1166 "compiler/main.gem"
    GemVal _t2449[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t2449, 1)), gem_string(";\n")));
        } else {
#line 1167 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1168 "compiler/main.gem"
    GemVal _t2450[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t2450, 1)), gem_string(";\n")));
#line 1169 "compiler/main.gem"
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
#line 1170 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1171 "compiler/main.gem"
    GemVal _t2451[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t2451, 1)), gem_string(";\n")));
#line 1172 "compiler/main.gem"
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
#line 1173 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1174 "compiler/main.gem"
    GemVal _t2452[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t2452, 1)), gem_string(";\n")));
#line 1175 "compiler/main.gem"
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
#line 1176 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1177 "compiler/main.gem"
    GemVal _t2453[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t2453, 1)), gem_string(";\n")));
#line 1178 "compiler/main.gem"
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
#line 1179 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1180 "compiler/main.gem"
    GemVal _t2454[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t2454, 1)), gem_string(";\n")));
#line 1181 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
#line 1183 "compiler/main.gem"
    GemVal _t2455[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2455, 1)), gem_string(";\n")));
#line 1184 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1187 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1190 "compiler/main.gem"
    GemVal _t2456[] = {gem_v_name};
    GemVal _t2457[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2456, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t2457, 2));
#line 1193 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1194 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1195 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1196 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1197 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1198 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1199 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1200 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1201 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1202 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1203 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1204 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1205 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1208 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1209 "compiler/main.gem"
    GemVal _t2458[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2458, 1), gem_int(0)))) {
#line 1210 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1212 "compiler/main.gem"
        GemVal gem_v__for_i_49 = gem_int(0);
#line 1212 "compiler/main.gem"
    GemVal _t2459[] = {gem_v_params};
        GemVal gem_v__for_limit_49 = gem_len_fn(NULL, _t2459, 1);
#line 1212 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_v__for_limit_49))) break;
#line 1212 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_49;
#line 1212 "compiler/main.gem"
            gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 1213 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1214 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1216 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1217 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1218 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1219 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1220 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1221 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1222 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1223 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1224 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1225 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1226 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1227 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1228 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1230 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1235 "compiler/main.gem"
    GemVal _t2460[] = {gem_v_c_ret};
    GemVal _t2461[] = {gem_v_name};
    GemVal _t2462[] = {gem_v_c_params};
    GemVal _t2463[] = {(*gem_v_extern_c_decls), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2460, 1), gem_string(" ")), gem_to_string_fn(NULL, _t2461, 1)), gem_string("(")), gem_to_string_fn(NULL, _t2462, 1)), gem_string(");"))};
    (void)(gem_push_fn(NULL, _t2463, 2));
    GemVal _t2464 = gem_v_lines;
    gem_pop_frame();
    return _t2464;
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
#line 1243 "compiler/main.gem"
    GemVal gem_v__d50 = gem_v_node;
#line 1243 "compiler/main.gem"
    GemVal _t2466 = gem_v__d50;
    GemVal gem_v_params = gem_table_get(_t2466, gem_string("params"));
#line 1243 "compiler/main.gem"
    GemVal _t2467 = gem_v__d50;
    GemVal gem_v_body = gem_table_get(_t2467, gem_string("body"));
#line 1243 "compiler/main.gem"
    GemVal _t2468 = gem_v__d50;
    GemVal gem_v_name = gem_table_get(_t2468, gem_string("name"));

#line 1246 "compiler/main.gem"
    GemVal _t2469[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t2469, 1);
#line 1247 "compiler/main.gem"
    GemVal _t2470 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2470, gem_string("rest_param")), GEM_NIL))) {
#line 1248 "compiler/main.gem"
    GemVal _t2471 = gem_v_node;
    GemVal _t2472[] = {gem_v_scope_vars, gem_table_get(_t2471, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t2472, 2));
    }
#line 1250 "compiler/main.gem"
    GemVal _t2473 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2473, gem_string("block_param")), GEM_NIL))) {
#line 1251 "compiler/main.gem"
    GemVal _t2474 = gem_v_node;
    GemVal _t2475[] = {gem_v_scope_vars, gem_table_get(_t2474, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t2475, 2));
    }
#line 1253 "compiler/main.gem"
    GemVal gem_v__for_i_52 = gem_int(0);
#line 1253 "compiler/main.gem"
    GemVal _t2476[] = {gem_v_body};
    GemVal gem_v__for_limit_52 = gem_len_fn(NULL, _t2476, 1);
#line 1253 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 1253 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_52;
#line 1253 "compiler/main.gem"
        gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 1254 "compiler/main.gem"
    GemVal _t2477[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2479;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2477, 1), gem_string("table")))) {
        _t2479 = gem_eq(gem_type_fn(NULL, _t2477, 1), gem_string("table"));
    } else {
        GemVal _t2478 = gem_table_get(gem_v_body, gem_v_i);
        _t2479 = gem_eq(gem_table_get(_t2478, gem_string("tag")), gem_string("let"));
    }
        if (gem_truthy(_t2479)) {
#line 1255 "compiler/main.gem"
    GemVal _t2480 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2481[] = {gem_v_scope_vars, gem_table_get(_t2480, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2481, 2));
        }
#line 1257 "compiler/main.gem"
    GemVal _t2482[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2484;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2482, 1), gem_string("table")))) {
        _t2484 = gem_eq(gem_type_fn(NULL, _t2482, 1), gem_string("table"));
    } else {
        GemVal _t2483 = gem_table_get(gem_v_body, gem_v_i);
        _t2484 = gem_eq(gem_table_get(_t2483, gem_string("tag")), gem_string("block"));
    }
        if (gem_truthy(_t2484)) {
#line 1258 "compiler/main.gem"
            GemVal gem_v__for_i_51 = gem_int(0);
#line 1258 "compiler/main.gem"
    GemVal _t2485 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2486[] = {gem_table_get(_t2485, gem_string("stmts"))};
            GemVal gem_v__for_limit_51 = gem_len_fn(NULL, _t2486, 1);
#line 1258 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_v__for_limit_51))) break;
#line 1258 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_51;
#line 1258 "compiler/main.gem"
                gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 1259 "compiler/main.gem"
    GemVal _t2487 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2488[] = {gem_table_get(gem_table_get(_t2487, gem_string("stmts")), gem_v_j)};
    GemVal _t2491;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2488, 1), gem_string("table")))) {
        _t2491 = gem_eq(gem_type_fn(NULL, _t2488, 1), gem_string("table"));
    } else {
        GemVal _t2489 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t2490 = gem_table_get(gem_table_get(_t2489, gem_string("stmts")), gem_v_j);
        _t2491 = gem_eq(gem_table_get(_t2490, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t2491)) {
#line 1260 "compiler/main.gem"
    GemVal _t2492 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2493 = gem_table_get(gem_table_get(_t2492, gem_string("stmts")), gem_v_j);
    GemVal _t2494[] = {gem_v_scope_vars, gem_table_get(_t2493, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t2494, 2));
                }
            }

        }
    }

#line 1265 "compiler/main.gem"
    GemVal _t2495 = (*gem_v_find_captured_in_scope);
    GemVal _t2496[] = {gem_v_body, gem_v_scope_vars};
    GemVal gem_v_captured = _t2495.fn(_t2495.env, _t2496, 2);
#line 1267 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1268 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1269 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1270 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1272 "compiler/main.gem"
    GemVal _t2497[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2497, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1275 "compiler/main.gem"
    GemVal _t2498 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2498, gem_string("line")), GEM_NIL))) {
#line 1276 "compiler/main.gem"
    GemVal _t2499 = gem_v_node;
    GemVal _t2500[] = {gem_table_get(_t2499, gem_string("line"))};
    GemVal _t2501[] = {(*gem_v_source_name)};
    GemVal _t2502[] = {gem_fn_escape_c_string(NULL, _t2501, 1)};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2500, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2502, 1)), gem_string("\"\n")));
    }
#line 1280 "compiler/main.gem"
    GemVal gem_v__for_i_53 = gem_int(0);
#line 1280 "compiler/main.gem"
    GemVal _t2503[] = {gem_v_params};
    GemVal gem_v__for_limit_53 = gem_len_fn(NULL, _t2503, 1);
#line 1280 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_v__for_limit_53))) break;
#line 1280 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_53;
#line 1280 "compiler/main.gem"
        gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 1281 "compiler/main.gem"
    GemVal _t2504 = (*gem_v_mangle);
    GemVal _t2505[] = {gem_table_get(gem_v_params, gem_v_i)};
        GemVal gem_v_mp = _t2504.fn(_t2504.env, _t2505, 1);
#line 1282 "compiler/main.gem"
    GemVal _t2506[] = {gem_v_captured, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2506, 2))) {
#line 1283 "compiler/main.gem"
    GemVal _t2507[] = {gem_v_mp};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2507, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1284 "compiler/main.gem"
    GemVal _t2508[] = {gem_v_mp};
    GemVal _t2509[] = {gem_v_i};
    GemVal _t2510[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2508, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2509, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2510, 1)), gem_string("] : GEM_NIL;\n")));
        } else {
#line 1286 "compiler/main.gem"
    GemVal _t2511[] = {gem_v_mp};
    GemVal _t2512[] = {gem_v_i};
    GemVal _t2513[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2511, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2512, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2513, 1)), gem_string("] : GEM_NIL;\n")));
        }
    }

#line 1291 "compiler/main.gem"
    GemVal _t2514 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2514, gem_string("rest_param")), GEM_NIL))) {
#line 1292 "compiler/main.gem"
    GemVal _t2515[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t2515, 1);
#line 1293 "compiler/main.gem"
        GemVal gem_v_end_count = gem_int(0);
#line 1294 "compiler/main.gem"
    GemVal _t2516 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2516, gem_string("block_param")), GEM_NIL))) {
#line 1295 "compiler/main.gem"
            gem_v_end_count = gem_int(1);
        }
#line 1297 "compiler/main.gem"
    GemVal _t2517 = gem_v_node;
    GemVal _t2518 = (*gem_v_mangle);
    GemVal _t2519[] = {gem_table_get(_t2517, gem_string("rest_param"))};
        GemVal gem_v_mp_rest = _t2518.fn(_t2518.env, _t2519, 1);
#line 1298 "compiler/main.gem"
    GemVal _t2520 = gem_v_node;
    GemVal _t2521[] = {gem_v_captured, gem_table_get(_t2520, gem_string("rest_param"))};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2521, 2))) {
#line 1299 "compiler/main.gem"
    GemVal _t2522[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2522, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1300 "compiler/main.gem"
    GemVal _t2523[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2523, 1)), gem_string(" = gem_table_new();\n")));
#line 1301 "compiler/main.gem"
    GemVal _t2524[] = {gem_v_N};
    GemVal _t2525[] = {gem_v_end_count};
    GemVal _t2526[] = {gem_v_mp_rest};
    GemVal _t2527[] = {gem_v_N};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t2524, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t2525, 1)), gem_string("; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t2526, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t2527, 1)), gem_string("), args[_ri]);\n")));
        } else {
#line 1303 "compiler/main.gem"
    GemVal _t2528[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2528, 1)), gem_string(" = gem_table_new();\n")));
#line 1304 "compiler/main.gem"
    GemVal _t2529[] = {gem_v_N};
    GemVal _t2530[] = {gem_v_end_count};
    GemVal _t2531[] = {gem_v_mp_rest};
    GemVal _t2532[] = {gem_v_N};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t2529, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t2530, 1)), gem_string("; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t2531, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t2532, 1)), gem_string("), args[_ri]);\n")));
        }
#line 1306 "compiler/main.gem"
    GemVal _t2533 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2533, gem_string("block_param")), GEM_NIL))) {
#line 1307 "compiler/main.gem"
    GemVal _t2534 = gem_v_node;
    GemVal _t2535 = (*gem_v_mangle);
    GemVal _t2536[] = {gem_table_get(_t2534, gem_string("block_param"))};
            GemVal gem_v_mp_block = _t2535.fn(_t2535.env, _t2536, 1);
#line 1308 "compiler/main.gem"
    GemVal _t2537 = gem_v_node;
    GemVal _t2538[] = {gem_v_captured, gem_table_get(_t2537, gem_string("block_param"))};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2538, 2))) {
#line 1309 "compiler/main.gem"
    GemVal _t2539[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2539, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1310 "compiler/main.gem"
    GemVal _t2540[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2540, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            } else {
#line 1312 "compiler/main.gem"
    GemVal _t2541[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2541, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            }
        }
    }
#line 1317 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1318 "compiler/main.gem"
    GemVal _t2542 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2542, gem_string("line")), GEM_NIL))) {
#line 1319 "compiler/main.gem"
    GemVal _t2543 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t2543, gem_string("line"));
    }
#line 1321 "compiler/main.gem"
    GemVal _t2544[] = {gem_v_name};
    GemVal _t2545[] = {gem_fn_escape_c_string(NULL, _t2544, 1)};
    GemVal _t2546[] = {(*gem_v_source_name)};
    GemVal _t2547[] = {gem_fn_escape_c_string(NULL, _t2546, 1)};
    GemVal _t2548[] = {gem_v_fn_line};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2545, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2547, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2548, 1)), gem_string(");\n")));
#line 1324 "compiler/main.gem"
    GemVal _t2549[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2549, 1), gem_int(0)))) {
#line 1325 "compiler/main.gem"
        GemVal gem_v__for_i_54 = gem_int(0);
#line 1325 "compiler/main.gem"
    GemVal _t2550[] = {gem_v_body};
        GemVal gem_v__for_limit_54 = gem_sub(gem_len_fn(NULL, _t2550, 1), gem_int(1));
#line 1325 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_v__for_limit_54))) break;
#line 1325 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_54;
#line 1325 "compiler/main.gem"
            gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 1326 "compiler/main.gem"
    GemVal _t2551 = (*gem_v_compile_stmt);
    GemVal _t2552[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
            gem_v_lines = gem_add(gem_add(gem_v_lines, _t2551.fn(_t2551.env, _t2552, 2)), gem_string("\n"));
        }

#line 1328 "compiler/main.gem"
    GemVal _t2553[] = {gem_v_body};
    GemVal _t2554 = (*gem_v_compile_stmt_return);
    GemVal _t2555[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2553, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t2554.fn(_t2554.env, _t2555, 2)), gem_string("\n"));
    } else {
#line 1330 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
#line 1333 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1334 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1335 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
    GemVal _t2556 = gem_v_lines;
    gem_pop_frame();
    return _t2556;
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
#line 1342 "compiler/main.gem"
    GemVal gem_v__d55 = gem_v_node;
#line 1342 "compiler/main.gem"
    GemVal _t2558 = gem_v__d55;
    GemVal gem_v_params = gem_table_get(_t2558, gem_string("params"));
#line 1342 "compiler/main.gem"
    GemVal _t2559 = gem_v__d55;
    GemVal gem_v_body = gem_table_get(_t2559, gem_string("body"));

#line 1343 "compiler/main.gem"
    GemVal _t2560 = (*gem_v_anon_name);
    GemVal gem_v_name = _t2560.fn(_t2560.env, NULL, 0);
#line 1346 "compiler/main.gem"
    GemVal _t2561[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t2561, 1);
#line 1347 "compiler/main.gem"
    GemVal _t2562 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2562, gem_string("rest_param")), GEM_NIL))) {
#line 1348 "compiler/main.gem"
    GemVal _t2563 = gem_v_node;
    GemVal _t2564[] = {gem_v_inner_scope, gem_table_get(_t2563, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t2564, 2));
    }
#line 1350 "compiler/main.gem"
    GemVal _t2565 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2565, gem_string("block_param")), GEM_NIL))) {
#line 1351 "compiler/main.gem"
    GemVal _t2566 = gem_v_node;
    GemVal _t2567[] = {gem_v_inner_scope, gem_table_get(_t2566, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t2567, 2));
    }
#line 1353 "compiler/main.gem"
    GemVal gem_v__for_i_57 = gem_int(0);
#line 1353 "compiler/main.gem"
    GemVal _t2568[] = {gem_v_body};
    GemVal gem_v__for_limit_57 = gem_len_fn(NULL, _t2568, 1);
#line 1353 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_v__for_limit_57))) break;
#line 1353 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_57;
#line 1353 "compiler/main.gem"
        gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
#line 1354 "compiler/main.gem"
    GemVal _t2569[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2571;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2569, 1), gem_string("table")))) {
        _t2571 = gem_eq(gem_type_fn(NULL, _t2569, 1), gem_string("table"));
    } else {
        GemVal _t2570 = gem_table_get(gem_v_body, gem_v_i);
        _t2571 = gem_eq(gem_table_get(_t2570, gem_string("tag")), gem_string("let"));
    }
        if (gem_truthy(_t2571)) {
#line 1355 "compiler/main.gem"
    GemVal _t2572 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2573[] = {gem_v_inner_scope, gem_table_get(_t2572, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2573, 2));
        }
#line 1357 "compiler/main.gem"
    GemVal _t2574[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t2576;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2574, 1), gem_string("table")))) {
        _t2576 = gem_eq(gem_type_fn(NULL, _t2574, 1), gem_string("table"));
    } else {
        GemVal _t2575 = gem_table_get(gem_v_body, gem_v_i);
        _t2576 = gem_eq(gem_table_get(_t2575, gem_string("tag")), gem_string("block"));
    }
        if (gem_truthy(_t2576)) {
#line 1358 "compiler/main.gem"
            GemVal gem_v__for_i_56 = gem_int(0);
#line 1358 "compiler/main.gem"
    GemVal _t2577 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2578[] = {gem_table_get(_t2577, gem_string("stmts"))};
            GemVal gem_v__for_limit_56 = gem_len_fn(NULL, _t2578, 1);
#line 1358 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_v__for_limit_56))) break;
#line 1358 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_56;
#line 1358 "compiler/main.gem"
                gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1359 "compiler/main.gem"
    GemVal _t2579 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2580[] = {gem_table_get(gem_table_get(_t2579, gem_string("stmts")), gem_v_j)};
    GemVal _t2583;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2580, 1), gem_string("table")))) {
        _t2583 = gem_eq(gem_type_fn(NULL, _t2580, 1), gem_string("table"));
    } else {
        GemVal _t2581 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t2582 = gem_table_get(gem_table_get(_t2581, gem_string("stmts")), gem_v_j);
        _t2583 = gem_eq(gem_table_get(_t2582, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t2583)) {
#line 1360 "compiler/main.gem"
    GemVal _t2584 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2585 = gem_table_get(gem_table_get(_t2584, gem_string("stmts")), gem_v_j);
    GemVal _t2586[] = {gem_v_inner_scope, gem_table_get(_t2585, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t2586, 2));
                }
            }

        }
    }

#line 1365 "compiler/main.gem"
    GemVal _t2587 = (*gem_v_find_captured_in_scope);
    GemVal _t2588[] = {gem_v_body, gem_v_inner_scope};
    GemVal gem_v_inner_captured = _t2587.fn(_t2587.env, _t2588, 2);
#line 1367 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1368 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1369 "compiler/main.gem"
    GemVal _t2589[] = {gem_v_captures};
    GemVal _t2590[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t2589, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t2590, 2);
#line 1370 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1373 "compiler/main.gem"
    GemVal _t2591[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t2591, 1));
#line 1374 "compiler/main.gem"
    GemVal _t2592[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t2592, 1)), gem_string(" {\n"));
#line 1375 "compiler/main.gem"
    GemVal gem_v__for_items_58 = gem_v_captures;
#line 1375 "compiler/main.gem"
    GemVal gem_v__for_i_58 = gem_int(0);
#line 1375 "compiler/main.gem"
    while (1) {
        GemVal _t2593[] = {gem_v__for_items_58};
        if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t2593, 1)))) break;
#line 1375 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1375 "compiler/main.gem"
        gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1376 "compiler/main.gem"
    GemVal _t2594 = (*gem_v_mangle);
    GemVal _t2595[] = {gem_v_cap};
    GemVal _t2596[] = {_t2594.fn(_t2594.env, _t2595, 1)};
        gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2596, 1)), gem_string(";\n")));
    }

#line 1378 "compiler/main.gem"
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
#line 1381 "compiler/main.gem"
    GemVal _t2597[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t2597, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1382 "compiler/main.gem"
    GemVal _t2598[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2598, 1), gem_int(0)))) {
#line 1383 "compiler/main.gem"
    GemVal _t2599[] = {gem_v_struct_name};
    GemVal _t2600[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2599, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t2600, 1)), gem_string(" *)_env;\n")));
#line 1384 "compiler/main.gem"
        GemVal gem_v__for_items_59 = gem_v_captures;
#line 1384 "compiler/main.gem"
        GemVal gem_v__for_i_59 = gem_int(0);
#line 1384 "compiler/main.gem"
        while (1) {
            GemVal _t2601[] = {gem_v__for_items_59};
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_len_fn(NULL, _t2601, 1)))) break;
#line 1384 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_59, gem_v__for_i_59);
#line 1384 "compiler/main.gem"
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1385 "compiler/main.gem"
    GemVal _t2602 = (*gem_v_mangle);
    GemVal _t2603[] = {gem_v_cap};
            GemVal gem_v_mc = _t2602.fn(_t2602.env, _t2603, 1);
#line 1386 "compiler/main.gem"
    GemVal _t2604[] = {gem_v_mc};
    GemVal _t2605[] = {gem_v_mc};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2604, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t2605, 1)), gem_string(";\n")));
        }

    }
#line 1391 "compiler/main.gem"
    GemVal gem_v__for_i_60 = gem_int(0);
#line 1391 "compiler/main.gem"
    GemVal _t2606[] = {gem_v_params};
    GemVal gem_v__for_limit_60 = gem_len_fn(NULL, _t2606, 1);
#line 1391 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1391 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_60;
#line 1391 "compiler/main.gem"
        gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1392 "compiler/main.gem"
    GemVal _t2607 = (*gem_v_mangle);
    GemVal _t2608[] = {gem_table_get(gem_v_params, gem_v_i)};
        GemVal gem_v_mp = _t2607.fn(_t2607.env, _t2608, 1);
#line 1393 "compiler/main.gem"
    GemVal _t2609[] = {gem_v_inner_captured, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2609, 2))) {
#line 1394 "compiler/main.gem"
    GemVal _t2610[] = {gem_v_mp};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2610, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1395 "compiler/main.gem"
    GemVal _t2611[] = {gem_v_mp};
    GemVal _t2612[] = {gem_v_i};
    GemVal _t2613[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2611, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2612, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2613, 1)), gem_string("] : GEM_NIL;\n")));
        } else {
#line 1397 "compiler/main.gem"
    GemVal _t2614[] = {gem_v_mp};
    GemVal _t2615[] = {gem_v_i};
    GemVal _t2616[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2614, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2615, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2616, 1)), gem_string("] : GEM_NIL;\n")));
        }
    }

#line 1402 "compiler/main.gem"
    GemVal _t2617 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2617, gem_string("rest_param")), GEM_NIL))) {
#line 1403 "compiler/main.gem"
    GemVal _t2618[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t2618, 1);
#line 1404 "compiler/main.gem"
        GemVal gem_v_end_count = gem_int(0);
#line 1405 "compiler/main.gem"
    GemVal _t2619 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2619, gem_string("block_param")), GEM_NIL))) {
#line 1406 "compiler/main.gem"
            gem_v_end_count = gem_int(1);
        }
#line 1408 "compiler/main.gem"
    GemVal _t2620 = gem_v_node;
    GemVal _t2621 = (*gem_v_mangle);
    GemVal _t2622[] = {gem_table_get(_t2620, gem_string("rest_param"))};
        GemVal gem_v_mp_rest = _t2621.fn(_t2621.env, _t2622, 1);
#line 1409 "compiler/main.gem"
    GemVal _t2623 = gem_v_node;
    GemVal _t2624[] = {gem_v_inner_captured, gem_table_get(_t2623, gem_string("rest_param"))};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2624, 2))) {
#line 1410 "compiler/main.gem"
    GemVal _t2625[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2625, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1411 "compiler/main.gem"
    GemVal _t2626[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2626, 1)), gem_string(" = gem_table_new();\n")));
#line 1412 "compiler/main.gem"
    GemVal _t2627[] = {gem_v_N};
    GemVal _t2628[] = {gem_v_end_count};
    GemVal _t2629[] = {gem_v_mp_rest};
    GemVal _t2630[] = {gem_v_N};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t2627, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t2628, 1)), gem_string("; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t2629, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t2630, 1)), gem_string("), args[_ri]);\n")));
        } else {
#line 1414 "compiler/main.gem"
    GemVal _t2631[] = {gem_v_mp_rest};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2631, 1)), gem_string(" = gem_table_new();\n")));
#line 1415 "compiler/main.gem"
    GemVal _t2632[] = {gem_v_N};
    GemVal _t2633[] = {gem_v_end_count};
    GemVal _t2634[] = {gem_v_mp_rest};
    GemVal _t2635[] = {gem_v_N};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t2632, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t2633, 1)), gem_string("; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t2634, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t2635, 1)), gem_string("), args[_ri]);\n")));
        }
#line 1417 "compiler/main.gem"
    GemVal _t2636 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2636, gem_string("block_param")), GEM_NIL))) {
#line 1418 "compiler/main.gem"
    GemVal _t2637 = gem_v_node;
    GemVal _t2638 = (*gem_v_mangle);
    GemVal _t2639[] = {gem_table_get(_t2637, gem_string("block_param"))};
            GemVal gem_v_mp_block = _t2638.fn(_t2638.env, _t2639, 1);
#line 1419 "compiler/main.gem"
    GemVal _t2640 = gem_v_node;
    GemVal _t2641[] = {gem_v_inner_captured, gem_table_get(_t2640, gem_string("block_param"))};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2641, 2))) {
#line 1420 "compiler/main.gem"
    GemVal _t2642[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2642, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1421 "compiler/main.gem"
    GemVal _t2643[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2643, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            } else {
#line 1423 "compiler/main.gem"
    GemVal _t2644[] = {gem_v_mp_block};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2644, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            }
        }
    }
#line 1428 "compiler/main.gem"
    GemVal _t2645[] = {gem_v_name};
    GemVal _t2646[] = {gem_fn_escape_c_string(NULL, _t2645, 1)};
    GemVal _t2647[] = {(*gem_v_source_name)};
    GemVal _t2648[] = {gem_fn_escape_c_string(NULL, _t2647, 1)};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2646, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2648, 1)), gem_string("\", 0);\n")));
#line 1430 "compiler/main.gem"
    GemVal _t2649[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2649, 1), gem_int(0)))) {
#line 1431 "compiler/main.gem"
        GemVal gem_v__for_i_61 = gem_int(0);
#line 1431 "compiler/main.gem"
    GemVal _t2650[] = {gem_v_body};
        GemVal gem_v__for_limit_61 = gem_sub(gem_len_fn(NULL, _t2650, 1), gem_int(1));
#line 1431 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_v__for_limit_61))) break;
#line 1431 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_61;
#line 1431 "compiler/main.gem"
            gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1432 "compiler/main.gem"
    GemVal _t2651 = (*gem_v_compile_stmt);
    GemVal _t2652[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
            gem_v_lines = gem_add(gem_add(gem_v_lines, _t2651.fn(_t2651.env, _t2652, 2)), gem_string("\n"));
        }

#line 1434 "compiler/main.gem"
    GemVal _t2653[] = {gem_v_body};
    GemVal _t2654 = (*gem_v_compile_stmt_return);
    GemVal _t2655[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2653, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t2654.fn(_t2654.env, _t2655, 2)), gem_string("\n"));
    } else {
#line 1436 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
#line 1439 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1440 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1441 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1444 "compiler/main.gem"
    GemVal _t2656[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t2656, 2));
    GemVal _t2657 = gem_table_new();
    gem_table_set(_t2657, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t2657, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t2657, gem_string("captures"), gem_v_captures);
    GemVal _t2658 = _t2657;
    gem_pop_frame();
    return _t2658;
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
#line 1453 "compiler/main.gem"
    GemVal _t2660 = gem_table_new();
    GemVal gem_v_fn_defs = _t2660;
#line 1454 "compiler/main.gem"
    GemVal _t2661 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t2661;
#line 1455 "compiler/main.gem"
    GemVal _t2662 = gem_table_new();
    GemVal gem_v_extern_includes = _t2662;
#line 1456 "compiler/main.gem"
    GemVal _t2663 = gem_table_new();
    GemVal gem_v_top_stmts = _t2663;
#line 1458 "compiler/main.gem"
    GemVal _t2664 = gem_v_ast;
    GemVal gem_v__for_items_62 = gem_table_get(_t2664, gem_string("stmts"));
#line 1458 "compiler/main.gem"
    GemVal gem_v__for_i_62 = gem_int(0);
#line 1458 "compiler/main.gem"
    while (1) {
        GemVal _t2665[] = {gem_v__for_items_62};
        if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_len_fn(NULL, _t2665, 1)))) break;
#line 1458 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_62, gem_v__for_i_62);
#line 1458 "compiler/main.gem"
        gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1459 "compiler/main.gem"
    GemVal _t2666 = gem_v_s;
        if (gem_truthy(gem_eq(gem_table_get(_t2666, gem_string("tag")), gem_string("fn_def")))) {
#line 1460 "compiler/main.gem"
    GemVal _t2667[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t2667, 2));
        } else {
#line 1461 "compiler/main.gem"
    GemVal _t2668 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t2668, gem_string("tag")), gem_string("extern_fn")))) {
#line 1462 "compiler/main.gem"
    GemVal _t2669[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t2669, 2));
            } else {
#line 1463 "compiler/main.gem"
    GemVal _t2670 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t2670, gem_string("tag")), gem_string("extern_include")))) {
#line 1464 "compiler/main.gem"
    GemVal _t2671[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t2671, 2));
                } else {
#line 1466 "compiler/main.gem"
    GemVal _t2672[] = {gem_v_top_stmts, gem_v_s};
                    (void)(gem_push_fn(NULL, _t2672, 2));
                }
            }
        }
    }

#line 1471 "compiler/main.gem"
    GemVal gem_v__for_items_63 = gem_v_extern_fns_list;
#line 1471 "compiler/main.gem"
    GemVal gem_v__for_i_63 = gem_int(0);
#line 1471 "compiler/main.gem"
    while (1) {
        GemVal _t2673[] = {gem_v__for_items_63};
        if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_len_fn(NULL, _t2673, 1)))) break;
#line 1471 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_63, gem_v__for_i_63);
#line 1471 "compiler/main.gem"
        gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1472 "compiler/main.gem"
    GemVal _t2674 = gem_v_ef;
    GemVal _t2675[] = {(*gem_v_defined_fns), gem_table_get(_t2674, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t2675, 2));
    }

#line 1476 "compiler/main.gem"
    GemVal gem_v__for_items_64 = gem_v_fn_defs;
#line 1476 "compiler/main.gem"
    GemVal gem_v__for_i_64 = gem_int(0);
#line 1476 "compiler/main.gem"
    while (1) {
        GemVal _t2676[] = {gem_v__for_items_64};
        if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_len_fn(NULL, _t2676, 1)))) break;
#line 1476 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_64, gem_v__for_i_64);
#line 1476 "compiler/main.gem"
        gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1477 "compiler/main.gem"
    GemVal _t2677 = gem_v_fd;
    GemVal _t2678[] = {(*gem_v_defined_fns), gem_table_get(_t2677, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t2678, 2));
#line 1478 "compiler/main.gem"
    GemVal _t2679 = gem_v_fd;
    GemVal _t2680[] = {gem_table_get(_t2679, gem_string("name"))};
        GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2680, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1479 "compiler/main.gem"
    GemVal _t2681[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t2681, 2));
    }

#line 1483 "compiler/main.gem"
    GemVal _t2682[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn_collect_top_let_names(NULL, _t2682, 2));
#line 1484 "compiler/main.gem"
    GemVal _t2683 = gem_table_new();
    GemVal _t2684 = (*gem_v_find_captured_in_scope);
    GemVal _t2685[] = {gem_v_top_stmts, _t2683};
    *gem_v_top_level_boxed = _t2684.fn(_t2684.env, _t2685, 2);
#line 1487 "compiler/main.gem"
    GemVal gem_v__for_items_65 = gem_v_extern_fns_list;
#line 1487 "compiler/main.gem"
    GemVal gem_v__for_i_65 = gem_int(0);
#line 1487 "compiler/main.gem"
    while (1) {
        GemVal _t2686[] = {gem_v__for_items_65};
        if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_len_fn(NULL, _t2686, 1)))) break;
#line 1487 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_65, gem_v__for_i_65);
#line 1487 "compiler/main.gem"
        gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1488 "compiler/main.gem"
    GemVal _t2687 = (*gem_v_compile_extern_fn);
    GemVal _t2688[] = {gem_v_ef};
    GemVal _t2689[] = {(*gem_v_functions), _t2687.fn(_t2687.env, _t2688, 1)};
        (void)(gem_push_fn(NULL, _t2689, 2));
    }

#line 1492 "compiler/main.gem"
    GemVal gem_v__for_items_66 = gem_v_fn_defs;
#line 1492 "compiler/main.gem"
    GemVal gem_v__for_i_66 = gem_int(0);
#line 1492 "compiler/main.gem"
    while (1) {
        GemVal _t2690[] = {gem_v__for_items_66};
        if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_len_fn(NULL, _t2690, 1)))) break;
#line 1492 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_66, gem_v__for_i_66);
#line 1492 "compiler/main.gem"
        gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1493 "compiler/main.gem"
    GemVal _t2691 = (*gem_v_compile_fn);
    GemVal _t2692[] = {gem_v_fd};
    GemVal _t2693[] = {(*gem_v_functions), _t2691.fn(_t2691.env, _t2692, 1)};
        (void)(gem_push_fn(NULL, _t2693, 2));
    }

#line 1497 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1498 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1499 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1500 "compiler/main.gem"
    GemVal _t2694 = (*gem_v_compile_stmts);
    GemVal _t2695[] = {gem_v_top_stmts, gem_int(1)};
    GemVal gem_v_main_body = _t2694.fn(_t2694.env, _t2695, 2);
#line 1501 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1502 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1504 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 1505 "compiler/main.gem"
    GemVal gem_v__for_items_67 = gem_v_fn_defs;
#line 1505 "compiler/main.gem"
    GemVal gem_v__for_i_67 = gem_int(0);
#line 1505 "compiler/main.gem"
    while (1) {
        GemVal _t2696[] = {gem_v__for_items_67};
        if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_len_fn(NULL, _t2696, 1)))) break;
#line 1505 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_67, gem_v__for_i_67);
#line 1505 "compiler/main.gem"
        gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1506 "compiler/main.gem"
    GemVal _t2697 = gem_v_fd;
        if (gem_truthy(gem_eq(gem_table_get(_t2697, gem_string("name")), gem_string("main")))) {
#line 1507 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 1511 "compiler/main.gem"
    GemVal gem_v_main_c = gem_string("int main(void) {\n");
#line 1512 "compiler/main.gem"
    GemVal _t2698[] = {(*gem_v_source_name)};
    GemVal _t2699[] = {gem_fn_escape_c_string(NULL, _t2698, 1)};
    gem_v_main_c = gem_add(gem_v_main_c, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t2699, 1)), gem_string("\", 0);\n")));
#line 1513 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1514 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
#line 1516 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1517 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
#line 1518 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
#line 1520 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_run_scheduler();\n"));
#line 1521 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_pop_frame();\n    return 0;\n}\n"));
#line 1525 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1526 "compiler/main.gem"
    GemVal gem_v__for_items_68 = gem_v_extern_includes;
#line 1526 "compiler/main.gem"
    GemVal gem_v__for_i_68 = gem_int(0);
#line 1526 "compiler/main.gem"
    while (1) {
        GemVal _t2700[] = {gem_v__for_items_68};
        if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_len_fn(NULL, _t2700, 1)))) break;
#line 1526 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_68, gem_v__for_i_68);
#line 1526 "compiler/main.gem"
        gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1527 "compiler/main.gem"
    GemVal _t2701 = gem_v_ei;
    GemVal _t2702[] = {gem_table_get(_t2701, gem_string("path"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t2702, 1)), gem_string("\"\n")));
    }

#line 1529 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
#line 1533 "compiler/main.gem"
    GemVal _t2703[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2703, 1), gem_int(0)))) {
#line 1534 "compiler/main.gem"
        GemVal gem_v__for_i_69 = gem_int(0);
#line 1534 "compiler/main.gem"
    GemVal _t2704[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_69 = gem_len_fn(NULL, _t2704, 1);
#line 1534 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_69, gem_v__for_limit_69))) break;
#line 1534 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_69;
#line 1534 "compiler/main.gem"
            gem_v__for_i_69 = gem_add(gem_v__for_i_69, gem_int(1));
#line 1535 "compiler/main.gem"
            gem_v_out = gem_add(gem_add(gem_v_out, gem_table_get((*gem_v_extern_c_decls), gem_v_i)), gem_string("\n"));
        }

    }
#line 1540 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1540 "compiler/main.gem"
    GemVal _t2705[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_70 = gem_len_fn(NULL, _t2705, 1);
#line 1540 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_70, gem_v__for_limit_70))) break;
#line 1540 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_70;
#line 1540 "compiler/main.gem"
        gem_v__for_i_70 = gem_add(gem_v__for_i_70, gem_int(1));
#line 1541 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
#line 1542 "compiler/main.gem"
    GemVal _t2706[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2706, 1), gem_int(1))))) {
#line 1543 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1546 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1549 "compiler/main.gem"
    GemVal _t2707[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t2707, 1);
#line 1550 "compiler/main.gem"
    GemVal gem_v__for_i_71 = gem_int(0);
#line 1550 "compiler/main.gem"
    GemVal _t2708[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_71 = gem_len_fn(NULL, _t2708, 1);
#line 1550 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_71, gem_v__for_limit_71))) break;
#line 1550 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_71;
#line 1550 "compiler/main.gem"
        gem_v__for_i_71 = gem_add(gem_v__for_i_71, gem_int(1));
#line 1551 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 1552 "compiler/main.gem"
    GemVal _t2709[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2709, 2))) {
#line 1553 "compiler/main.gem"
    GemVal _t2710[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal *gem_v_"), gem_to_string_fn(NULL, _t2710, 1)), gem_string(";\n")));
        } else {
#line 1555 "compiler/main.gem"
    GemVal _t2711[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal gem_v_"), gem_to_string_fn(NULL, _t2711, 1)), gem_string(";\n")));
        }
    }

#line 1558 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1561 "compiler/main.gem"
    GemVal gem_v__for_i_72 = gem_int(0);
#line 1561 "compiler/main.gem"
    GemVal _t2712[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_72 = gem_len_fn(NULL, _t2712, 1);
#line 1561 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_72, gem_v__for_limit_72))) break;
#line 1561 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_72;
#line 1561 "compiler/main.gem"
        gem_v__for_i_72 = gem_add(gem_v__for_i_72, gem_int(1));
#line 1562 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
#line 1563 "compiler/main.gem"
    GemVal _t2713[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2713, 1), gem_int(1))))) {
#line 1564 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
        }
    }

#line 1567 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
#line 1569 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    GemVal _t2714 = gem_v_out;
    gem_pop_frame();
    return _t2714;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 147 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_codegen", "compiler/main.gem", 147);
#line 148 "compiler/main.gem"
    GemVal _t1120 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1120;
#line 149 "compiler/main.gem"
    GemVal _t1121 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1121;
#line 150 "compiler/main.gem"
    GemVal _t1122 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1122;
#line 151 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 152 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 153 "compiler/main.gem"
    GemVal _t1123 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1123;
#line 154 "compiler/main.gem"
    GemVal _t1124 = gem_table_new();
    gem_table_set(_t1124, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1124, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1124, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1124, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1124, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1124, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1124, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1124, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1124, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1124, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1124, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1124, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1124, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1124, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1124, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1124, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1124, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1124, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1124, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1124, gem_string("self"), gem_string("gem_self_builtin"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1124;
#line 176 "compiler/main.gem"
    GemVal _t1125 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1125;
#line 177 "compiler/main.gem"
    GemVal _t1126[] = {(*gem_v_builtin_fns)};
    GemVal gem_v__bkeys = gem_keys_fn(NULL, _t1126, 1);
#line 178 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 178 "compiler/main.gem"
    GemVal _t1127[] = {gem_v__bkeys};
    GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1127, 1);
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

#line 181 "compiler/main.gem"
    GemVal _t1128 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1128;
#line 182 "compiler/main.gem"
    GemVal _t1129 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1129;
#line 183 "compiler/main.gem"
    GemVal _t1130 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1130;
#line 184 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 188 "compiler/main.gem"
    struct _closure__anon_22 *_t1133 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1133->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_22, _t1133);
#line 193 "compiler/main.gem"
    struct _closure__anon_23 *_t1136 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1136->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_23, _t1136);
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
    struct _closure__anon_25 *_t1311 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1311->gem_v_builtins = gem_v_builtins;
    _t1311->gem_v_collect_free_node = gem_v_collect_free_node;
    _t1311->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1311->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_25, _t1311);
#line 347 "compiler/main.gem"
    struct _closure__anon_26 *_t1337 = GC_MALLOC(sizeof(struct _closure__anon_26));
    _t1337->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_26, _t1337);
#line 368 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 369 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 371 "compiler/main.gem"
    struct _closure__anon_27 *_t1429 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1429->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1429->gem_v_walk_captures = gem_v_walk_captures;
    _t1429->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_27, _t1429);
#line 453 "compiler/main.gem"
    struct _closure__anon_28 *_t1453 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1453->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_28, _t1453);
#line 470 "compiler/main.gem"
    struct _closure__anon_29 *_t1458 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t1458->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_29, _t1458);
#line 478 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 479 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 480 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 481 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 482 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 483 "compiler/main.gem"
    GemVal *gem_v_compile_if_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if_return = GEM_NIL;
#line 484 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 485 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 486 "compiler/main.gem"
    GemVal *gem_v_compile_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match_return = GEM_NIL;
#line 487 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 488 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 489 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 490 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 491 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 492 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 493 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 494 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 498 "compiler/main.gem"
    struct _closure__anon_30 *_t1583 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t1583->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1583->gem_v_builtin_fns = gem_v_builtin_fns;
    _t1583->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t1583->gem_v_compile_array = gem_v_compile_array;
    _t1583->gem_v_compile_binop = gem_v_compile_binop;
    _t1583->gem_v_compile_call = gem_v_compile_call;
    _t1583->gem_v_compile_expr = gem_v_compile_expr;
    _t1583->gem_v_compile_table = gem_v_compile_table;
    _t1583->gem_v_defined_fns = gem_v_defined_fns;
    _t1583->gem_v_mangle = gem_v_mangle;
    _t1583->gem_v_tmp = gem_v_tmp;
    _t1583->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_30, _t1583);
#line 585 "compiler/main.gem"
    struct _closure__anon_31 *_t1600 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t1600->gem_v_compile_expr = gem_v_compile_expr;
    _t1600->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_31, _t1600);
#line 600 "compiler/main.gem"
    struct _closure__anon_32 *_t1614 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t1614->gem_v_compile_expr = gem_v_compile_expr;
    _t1614->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_32, _t1614);
#line 614 "compiler/main.gem"
    struct _closure__anon_33 *_t1658 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t1658->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1658->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1658->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t1658->gem_v_mangle = gem_v_mangle;
    _t1658->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_33, _t1658);
#line 648 "compiler/main.gem"
    struct _closure__anon_34 *_t1767 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t1767->gem_v_builtin_fns = gem_v_builtin_fns;
    _t1767->gem_v_compile_expr = gem_v_compile_expr;
    _t1767->gem_v_defined_fns = gem_v_defined_fns;
    _t1767->gem_v_source_name = gem_v_source_name;
    _t1767->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_34, _t1767);
#line 756 "compiler/main.gem"
    struct _closure__anon_35 *_t1923 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t1923->gem_v_compile_expr = gem_v_compile_expr;
    _t1923->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_35, _t1923);
#line 836 "compiler/main.gem"
    struct _closure__anon_36 *_t1928 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t1928->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_36, _t1928);
#line 846 "compiler/main.gem"
    struct _closure__anon_37 *_t2109 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t2109->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2109->gem_v_compile_expr = gem_v_compile_expr;
    _t2109->gem_v_compile_if = gem_v_compile_if;
    _t2109->gem_v_compile_match = gem_v_compile_match;
    _t2109->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2109->gem_v_compile_while = gem_v_compile_while;
    _t2109->gem_v_in_top_level = gem_v_in_top_level;
    _t2109->gem_v_mangle = gem_v_mangle;
    _t2109->gem_v_source_name = gem_v_source_name;
    _t2109->gem_v_tmp = gem_v_tmp;
    _t2109->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t2109->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_37, _t2109);
#line 922 "compiler/main.gem"
    struct _closure__anon_38 *_t2258 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2258->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2258->gem_v_compile_expr = gem_v_compile_expr;
    _t2258->gem_v_compile_if_return = gem_v_compile_if_return;
    _t2258->gem_v_compile_match_return = gem_v_compile_match_return;
    _t2258->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2258->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2258->gem_v_compile_while = gem_v_compile_while;
    _t2258->gem_v_mangle = gem_v_mangle;
    _t2258->gem_v_source_name = gem_v_source_name;
    _t2258->gem_v_tmp = gem_v_tmp;
    _t2258->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_38, _t2258);
#line 981 "compiler/main.gem"
    struct _closure__anon_39 *_t2277 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2277->gem_v_compile_expr = gem_v_compile_expr;
    _t2277->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_39, _t2277);
#line 997 "compiler/main.gem"
    struct _closure__anon_40 *_t2313 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t2313->gem_v_compile_expr = gem_v_compile_expr;
    _t2313->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2313->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_40, _t2313);
#line 1032 "compiler/main.gem"
    struct _closure__anon_41 *_t2331 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2331->gem_v_compile_expr = gem_v_compile_expr;
    _t2331->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_41, _t2331);
#line 1047 "compiler/main.gem"
    struct _closure__anon_42 *_t2364 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t2364->gem_v_compile_expr = gem_v_compile_expr;
    _t2364->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2364->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_42, _t2364);
#line 1074 "compiler/main.gem"
    struct _closure__anon_43 *_t2414 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t2414->gem_v_compile_expr = gem_v_compile_expr;
    _t2414->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2414->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2414->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_43, _t2414);
#line 1120 "compiler/main.gem"
    struct _closure__anon_44 *_t2465 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t2465->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t2465->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_44, _t2465);
#line 1242 "compiler/main.gem"
    struct _closure__anon_45 *_t2557 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t2557->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2557->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2557->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2557->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2557->gem_v_in_top_level = gem_v_in_top_level;
    _t2557->gem_v_mangle = gem_v_mangle;
    _t2557->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_fn = gem_make_fn(_anon_45, _t2557);
#line 1341 "compiler/main.gem"
    struct _closure__anon_46 *_t2659 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t2659->gem_v_anon_name = gem_v_anon_name;
    _t2659->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2659->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2659->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2659->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2659->gem_v_functions = gem_v_functions;
    _t2659->gem_v_in_top_level = gem_v_in_top_level;
    _t2659->gem_v_mangle = gem_v_mangle;
    _t2659->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_46, _t2659);
#line 1451 "compiler/main.gem"
    struct _closure__anon_47 *_t2715 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t2715->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2715->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t2715->gem_v_compile_fn = gem_v_compile_fn;
    _t2715->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2715->gem_v_defined_fns = gem_v_defined_fns;
    _t2715->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t2715->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2715->gem_v_forward_decls = gem_v_forward_decls;
    _t2715->gem_v_functions = gem_v_functions;
    _t2715->gem_v_in_top_level = gem_v_in_top_level;
    _t2715->gem_v_source_name = gem_v_source_name;
    _t2715->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t2715->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_47, _t2715);
    GemVal _t2716 = gem_table_new();
    gem_table_set(_t2716, gem_string("compile"), gem_v_compile);
    GemVal _t2717 = _t2716;
    gem_pop_frame();
    return _t2717;
}

static GemVal gem_fn_split_string(void *_env, GemVal *args, int argc) {
#line 1578 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("split_string", "compiler/main.gem", 1578);
#line 1579 "compiler/main.gem"
    GemVal _t2718 = gem_table_new();
    GemVal gem_v_result = _t2718;
#line 1580 "compiler/main.gem"
    GemVal gem_v_current = gem_string("");
#line 1581 "compiler/main.gem"
    GemVal gem_v__for_i_73 = gem_int(0);
#line 1581 "compiler/main.gem"
    GemVal _t2719[] = {gem_v_s};
    GemVal gem_v__for_limit_73 = gem_len_fn(NULL, _t2719, 1);
#line 1581 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_73, gem_v__for_limit_73))) break;
#line 1581 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_73;
#line 1581 "compiler/main.gem"
        gem_v__for_i_73 = gem_add(gem_v__for_i_73, gem_int(1));
#line 1582 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_v_delim))) {
#line 1583 "compiler/main.gem"
    GemVal _t2720[] = {gem_v_result, gem_v_current};
            (void)(gem_push_fn(NULL, _t2720, 2));
#line 1584 "compiler/main.gem"
            gem_v_current = gem_string("");
        } else {
#line 1586 "compiler/main.gem"
            gem_v_current = gem_add(gem_v_current, gem_table_get(gem_v_s, gem_v_i));
        }
    }

#line 1589 "compiler/main.gem"
    GemVal _t2721[] = {gem_v_result, gem_v_current};
    (void)(gem_push_fn(NULL, _t2721, 2));
    GemVal _t2722 = gem_v_result;
    gem_pop_frame();
    return _t2722;
}

static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc) {
#line 1596 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("str_replace", "compiler/main.gem", 1596);
#line 1597 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 1598 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 1599 "compiler/main.gem"
    GemVal _t2723[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t2723, 1);
#line 1600 "compiler/main.gem"
    GemVal _t2724[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t2724, 1);
#line 1601 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
#line 1602 "compiler/main.gem"
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
#line 1603 "compiler/main.gem"
            GemVal gem_v_found = gem_bool(1);
#line 1604 "compiler/main.gem"
            GemVal gem_v_j = gem_int(0);
#line 1605 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
#line 1606 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
#line 1607 "compiler/main.gem"
                    gem_v_found = gem_bool(0);
                    break;
                }
#line 1610 "compiler/main.gem"
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
#line 1612 "compiler/main.gem"
            if (gem_truthy(gem_v_found)) {
#line 1613 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
#line 1614 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
#line 1618 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
#line 1619 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t2725 = gem_v_result;
    gem_pop_frame();
    return _t2725;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 24 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 24);
#line 25 "compiler/main.gem"
    GemVal _t2726 = gem_table_new();
    GemVal gem_v_new_stmts = _t2726;
#line 26 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 27 "compiler/main.gem"
    GemVal _t2727 = gem_v_ast;
    GemVal gem_v__for_items_1 = gem_table_get(_t2727, gem_string("stmts"));
#line 27 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 27 "compiler/main.gem"
    while (1) {
        GemVal _t2728[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t2728, 1)))) break;
#line 27 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 27 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 28 "compiler/main.gem"
    GemVal _t2729[] = {gem_v_stmt};
    GemVal _t2731;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2729, 1), gem_string("table")))) {
        _t2731 = gem_eq(gem_type_fn(NULL, _t2729, 1), gem_string("table"));
    } else {
        GemVal _t2730 = gem_v_stmt;
        _t2731 = gem_eq(gem_table_get(_t2730, gem_string("tag")), gem_string("load"));
    }
        if (gem_truthy(_t2731)) {
#line 29 "compiler/main.gem"
    GemVal _t2732 = gem_v_stmt;
            GemVal gem_v_path = gem_table_get(_t2732, gem_string("path"));
#line 31 "compiler/main.gem"
    GemVal _t2733[] = {gem_v_path};
    GemVal _t2741;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t2733, 1), gem_int(4)))) {
        _t2741 = gem_lt(gem_len_fn(NULL, _t2733, 1), gem_int(4));
    } else {
        GemVal _t2734[] = {gem_v_path};
        GemVal _t2736;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2734, 1), gem_int(4))), gem_string(".")))) {
                _t2736 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2734, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t2735[] = {gem_v_path};
                _t2736 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2735, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t2738;
        if (gem_truthy(_t2736)) {
                _t2738 = _t2736;
        } else {
                GemVal _t2737[] = {gem_v_path};
                _t2738 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2737, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t2740;
        if (gem_truthy(_t2738)) {
                _t2740 = _t2738;
        } else {
                GemVal _t2739[] = {gem_v_path};
                _t2740 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2739, 1), gem_int(1))), gem_string("m"));
        }
        _t2741 = _t2740;
    }
            if (gem_truthy(_t2741)) {
#line 32 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 36 "compiler/main.gem"
    GemVal _t2742[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_fn_gem_path_join(NULL, _t2742, 2);
#line 39 "compiler/main.gem"
    GemVal _t2743[] = {gem_v_full_path};
            if (gem_truthy(gem_eq(gem_fn_gem_file_exists(NULL, _t2743, 1), gem_int(0)))) {
#line 40 "compiler/main.gem"
    GemVal _t2744[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_fn_gem_path_join(NULL, _t2744, 2);
            }
#line 43 "compiler/main.gem"
    GemVal _t2745[] = {gem_v_full_path};
            gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t2745, 1);
#line 46 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                continue;
            }
#line 49 "compiler/main.gem"
            gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 51 "compiler/main.gem"
    GemVal _t2746[] = {gem_v_full_path};
            GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2746, 1);
#line 52 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 53 "compiler/main.gem"
    GemVal _t2747[] = {gem_v_full_path};
    GemVal _t2748[] = {gem_add(gem_add(gem_string("cannot load '"), gem_to_string_fn(NULL, _t2747, 1)), gem_string("'"))};
                (void)(gem_error_at_fn("compiler/main.gem", 53, _t2748, 1));
            }
#line 56 "compiler/main.gem"
    GemVal _t2749[] = {gem_v_source};
            GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t2749, 1);
#line 57 "compiler/main.gem"
    GemVal _t2750[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t2750, 1);
#line 58 "compiler/main.gem"
    GemVal _t2751[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t2751, 3);
#line 60 "compiler/main.gem"
    GemVal _t2752 = gem_v_resolved;
            GemVal gem_v__for_items_2 = gem_table_get(_t2752, gem_string("stmts"));
#line 60 "compiler/main.gem"
            GemVal gem_v__for_i_2 = gem_int(0);
#line 60 "compiler/main.gem"
            while (1) {
                GemVal _t2753[] = {gem_v__for_items_2};
                if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t2753, 1)))) break;
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
    GemVal _t2754[] = {gem_v_new_stmts};
    GemVal _t2755 = gem_fn_make_program(NULL, _t2754, 1);
    gem_pop_frame();
    return _t2755;
}

int main(void) {
    gem_push_frame("main", "compiler/main.gem", 0);
#line 20 "compiler/main.gem"
    GemVal _t2756[] = {gem_int(0)};
    GemVal _t2757[] = {gem_fn_gem_get_argv(NULL, _t2756, 1)};
    GemVal _t2758[] = {gem_fn_gem_dirname(NULL, _t2757, 1)};
    gem_v_install_root = gem_fn_gem_dirname(NULL, _t2758, 1);
#line 74 "compiler/main.gem"
    gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
#line 75 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 76 "compiler/main.gem"
    GemVal _t2759[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 76, _t2759, 1));
    }
#line 79 "compiler/main.gem"
    GemVal _t2760[] = {gem_int(1)};
    gem_v_src_path = gem_fn_gem_get_argv(NULL, _t2760, 1);
#line 80 "compiler/main.gem"
    GemVal _t2761[] = {gem_v_src_path};
    gem_v_source = gem_fn_gem_read_file(NULL, _t2761, 1);
#line 81 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 82 "compiler/main.gem"
    GemVal _t2762[] = {gem_v_src_path};
    GemVal _t2763[] = {gem_add(gem_add(gem_string("cannot read '"), gem_to_string_fn(NULL, _t2762, 1)), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 82, _t2763, 1));
    }
#line 85 "compiler/main.gem"
    GemVal _t2764[] = {gem_v_source};
    gem_v_ast = gem_fn_parse_source(NULL, _t2764, 1);
#line 86 "compiler/main.gem"
    GemVal _t2765[] = {gem_v_src_path};
    gem_v_base_dir = gem_fn_gem_dirname(NULL, _t2765, 1);
#line 87 "compiler/main.gem"
    GemVal _t2766 = gem_table_new();
    gem_v_loaded = _t2766;
#line 88 "compiler/main.gem"
    GemVal _t2767[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t2767, 3);
#line 90 "compiler/main.gem"
    GemVal _t2768[] = {gem_v_src_path};
    gem_v_cg = gem_fn_make_codegen(NULL, _t2768, 1);
#line 91 "compiler/main.gem"
    GemVal _t2769 = gem_v_cg;
    GemVal _t2770 = gem_table_get(_t2769, gem_string("compile"));
    GemVal _t2771[] = {gem_v_resolved_ast};
    gem_v_c_code = _t2770.fn(_t2770.env, _t2771, 1);
#line 92 "compiler/main.gem"
    GemVal _t2772[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t2772, 1));
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

