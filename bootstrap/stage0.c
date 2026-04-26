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
static GemVal gem_fn_apply_escape(void *_env, GemVal *args, int argc);
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
static GemVal gem_fn_collect_block_lets(void *_env, GemVal *args, int argc);
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

static GemVal gem_fn_apply_escape(void *_env, GemVal *args, int argc) {
#line 78 "compiler/main.gem"
    GemVal gem_v_esc = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_mode = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("apply_escape", "compiler/main.gem", 78);
#line 79 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 80 "compiler/main.gem"
        GemVal _t98 = gem_string("\n");
        gem_pop_frame();
        return _t98;
    } else {
#line 81 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 82 "compiler/main.gem"
    GemVal _t99[] = {gem_int(13)};
            GemVal _t100 = gem_chr_fn(NULL, _t99, 1);
            gem_pop_frame();
            return _t100;
        } else {
#line 83 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 84 "compiler/main.gem"
                GemVal _t101 = gem_string("\t");
                gem_pop_frame();
                return _t101;
            } else {
#line 85 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 86 "compiler/main.gem"
                    GemVal _t102 = gem_string("\\");
                    gem_pop_frame();
                    return _t102;
                } else {
#line 87 "compiler/main.gem"
    GemVal _t103;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
        _t103 = gem_eq(gem_v_esc, gem_string("0"));
    } else {
        _t103 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                    if (gem_truthy(_t103)) {
#line 88 "compiler/main.gem"
    GemVal _t104[] = {gem_int(0)};
                        GemVal _t105 = gem_chr_fn(NULL, _t104, 1);
                        gem_pop_frame();
                        return _t105;
                    } else {
#line 89 "compiler/main.gem"
    GemVal _t106;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
        _t106 = gem_eq(gem_v_esc, gem_string("\""));
    } else {
        _t106 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                        if (gem_truthy(_t106)) {
#line 90 "compiler/main.gem"
                            GemVal _t107 = gem_string("\"");
                            gem_pop_frame();
                            return _t107;
                        } else {
#line 91 "compiler/main.gem"
    GemVal _t108;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
        _t108 = gem_eq(gem_v_esc, gem_string("{"));
    } else {
        _t108 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                            if (gem_truthy(_t108)) {
#line 92 "compiler/main.gem"
                                GemVal _t109 = gem_string("{");
                                gem_pop_frame();
                                return _t109;
                            } else {
#line 93 "compiler/main.gem"
    GemVal _t110;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
        _t110 = gem_eq(gem_v_esc, gem_string("}"));
    } else {
        _t110 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                                if (gem_truthy(_t110)) {
#line 94 "compiler/main.gem"
                                    GemVal _t111 = gem_string("}");
                                    gem_pop_frame();
                                    return _t111;
                                } else {
#line 95 "compiler/main.gem"
    GemVal _t112;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("'")))) {
        _t112 = gem_eq(gem_v_esc, gem_string("'"));
    } else {
        _t112 = gem_eq(gem_v_mode, gem_string("sq"));
    }
                                    if (gem_truthy(_t112)) {
#line 96 "compiler/main.gem"
                                        GemVal _t113 = gem_string("'");
                                        gem_pop_frame();
                                        return _t113;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t114 = GEM_NIL;
    gem_pop_frame();
    return _t114;
}

static GemVal gem_fn_build_keyword_set(void *_env, GemVal *args, int argc) {
#line 103 "compiler/main.gem"
    gem_push_frame("build_keyword_set", "compiler/main.gem", 103);
#line 104 "compiler/main.gem"
    GemVal _t115 = gem_table_new();
    GemVal gem_v_t = _t115;
#line 105 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("fn"), gem_bool(1));
#line 106 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("end"), gem_bool(1));
#line 107 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("let"), gem_bool(1));
#line 108 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("if"), gem_bool(1));
#line 109 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("elif"), gem_bool(1));
#line 110 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("else"), gem_bool(1));
#line 111 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("while"), gem_bool(1));
#line 112 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("return"), gem_bool(1));
#line 113 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("break"), gem_bool(1));
#line 114 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("continue"), gem_bool(1));
#line 115 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("do"), gem_bool(1));
#line 116 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("match"), gem_bool(1));
#line 117 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("when"), gem_bool(1));
#line 118 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("and"), gem_bool(1));
#line 119 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("or"), gem_bool(1));
#line 120 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("not"), gem_bool(1));
#line 121 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("true"), gem_bool(1));
#line 122 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("false"), gem_bool(1));
#line 123 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("nil"), gem_bool(1));
#line 124 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("load"), gem_bool(1));
#line 125 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("extern"), gem_bool(1));
#line 126 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("for"), gem_bool(1));
#line 127 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("in"), gem_bool(1));
    GemVal _t116 = gem_v_t;
    gem_pop_frame();
    return _t116;
}

static GemVal gem_fn_tokenize(void *_env, GemVal *args, int argc) {
#line 131 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("tokenize", "compiler/main.gem", 131);
#line 132 "compiler/main.gem"
    GemVal gem_v_kw = gem_fn_build_keyword_set(NULL, NULL, 0);
#line 133 "compiler/main.gem"
    GemVal _t117 = gem_table_new();
    GemVal gem_v_tokens = _t117;
#line 134 "compiler/main.gem"
    GemVal gem_v_pos = gem_int(0);
#line 135 "compiler/main.gem"
    GemVal gem_v_line = gem_int(1);
#line 136 "compiler/main.gem"
    GemVal _t118[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t118, 1);
#line 137 "compiler/main.gem"
    GemVal gem_v_interp_depth = gem_int(0);
#line 138 "compiler/main.gem"
    GemVal _t119 = gem_table_new();
    GemVal gem_v_interp_brace = _t119;
#line 139 "compiler/main.gem"
    GemVal _t120[] = {gem_int(13)};
    GemVal gem_v_CR = gem_chr_fn(NULL, _t120, 1);
#line 140 "compiler/main.gem"
    GemVal _t121[] = {gem_int(0)};
    GemVal gem_v_NUL = gem_chr_fn(NULL, _t121, 1);
#line 141 "compiler/main.gem"
    GemVal gem_v_in_triple_dq = gem_bool(0);
#line 142 "compiler/main.gem"
    GemVal gem_v_triple_dq_indent = gem_int(0);
#line 144 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 145 "compiler/main.gem"
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
#line 148 "compiler/main.gem"
    GemVal _t122;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t122 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t122 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t122)) {
#line 149 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 154 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 155 "compiler/main.gem"
            while (1) {
                GemVal _t123;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t123 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t123 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t123)) break;
#line 156 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 162 "compiler/main.gem"
    GemVal _t124;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t124 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t124 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t124)) {
#line 163 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 164 "compiler/main.gem"
    GemVal _t125[] = {gem_v_tokens};
    GemVal _t128;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t125, 1), gem_int(0)))) {
        _t128 = gem_gt(gem_len_fn(NULL, _t125, 1), gem_int(0));
    } else {
        GemVal _t126[] = {gem_v_tokens};
        GemVal _t127 = gem_table_get(gem_v_tokens, gem_sub(gem_len_fn(NULL, _t126, 1), gem_int(1)));
        _t128 = gem_eq(gem_table_get(_t127, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t128)) {
#line 165 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 167 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 168 "compiler/main.gem"
    GemVal _t129 = gem_table_new();
    gem_table_set(_t129, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t129, gem_string("value"), gem_string("\n"));
    gem_table_set(_t129, gem_string("line"), gem_v_line);
    GemVal _t130[] = {gem_v_tokens, _t129};
                (void)(gem_push_fn(NULL, _t130, 2));
            }
#line 170 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 171 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
            }
#line 173 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 175 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 176 "compiler/main.gem"
                GemVal gem_v_c2 = gem_table_get(gem_v_source, gem_v_pos);
#line 177 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_c2, gem_string("\n")))) {
#line 178 "compiler/main.gem"
                    gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 179 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                } else {
#line 180 "compiler/main.gem"
    GemVal _t131;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t131 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t131 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t132;
    if (gem_truthy(_t131)) {
        _t132 = _t131;
    } else {
        _t132 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t132)) {
#line 181 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 182 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 183 "compiler/main.gem"
                            while (1) {
                                GemVal _t133;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t133 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t133 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t133)) break;
#line 184 "compiler/main.gem"
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
#line 194 "compiler/main.gem"
    GemVal _t134[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t134, 1))) {
#line 195 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 196 "compiler/main.gem"
            while (1) {
                GemVal _t136;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t136 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t135[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t136 = gem_fn_is_digit(NULL, _t135, 1);
                }
                if (!gem_truthy(_t136)) break;
#line 197 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 198 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 200 "compiler/main.gem"
    GemVal _t137;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t137 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t137 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t137;
#line 201 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 202 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 203 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 204 "compiler/main.gem"
                while (1) {
                    GemVal _t139;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t139 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t138[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t139 = gem_fn_is_digit(NULL, _t138, 1);
                    }
                    if (!gem_truthy(_t139)) break;
#line 205 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 206 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 209 "compiler/main.gem"
    GemVal _t140 = gem_table_new();
    gem_table_set(_t140, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t140, gem_string("value"), gem_v_val);
    gem_table_set(_t140, gem_string("line"), gem_v_line);
    GemVal _t141[] = {gem_v_tokens, _t140};
            (void)(gem_push_fn(NULL, _t141, 2));
            continue;
        }
#line 214 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 215 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 218 "compiler/main.gem"
    GemVal _t142;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t142 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t142 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t143;
    if (!gem_truthy(_t142)) {
        _t143 = _t142;
    } else {
        _t143 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
            if (gem_truthy(_t143)) {
#line 219 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 221 "compiler/main.gem"
                while (1) {
                    GemVal _t145;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t145 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t144;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t144 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t144 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t145 = _t144;
                    }
                    if (!gem_truthy(_t145)) break;
#line 222 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 224 "compiler/main.gem"
    GemVal _t146;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t146 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t146 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t146)) {
#line 225 "compiler/main.gem"
    GemVal _t147[] = {gem_v_line};
    GemVal _t148[] = {gem_add(gem_string("triple-quoted string must be followed by a newline at line "), gem_to_string_fn(NULL, _t147, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 225, _t148, 1));
                }
#line 227 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 228 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 231 "compiler/main.gem"
                GemVal gem_v_tq_scan = gem_v_pos;
#line 232 "compiler/main.gem"
                GemVal gem_v_tq_bi = gem_int(0);
#line 233 "compiler/main.gem"
                GemVal gem_v_tq_found = gem_bool(0);
#line 234 "compiler/main.gem"
                while (1) {
                    GemVal _t149;
                    if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                        _t149 = gem_lt(gem_v_tq_scan, gem_v_length);
                    } else {
                                        _t149 = gem_not(gem_v_tq_found);
                    }
                    if (!gem_truthy(_t149)) break;
#line 235 "compiler/main.gem"
                    GemVal gem_v_tq_ind = gem_int(0);
#line 236 "compiler/main.gem"
                    while (1) {
                        GemVal _t150;
                        if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                _t150 = gem_lt(gem_v_tq_scan, gem_v_length);
                        } else {
                                                _t150 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t150)) break;
#line 237 "compiler/main.gem"
                        gem_v_tq_ind = gem_add(gem_v_tq_ind, gem_int(1));
#line 238 "compiler/main.gem"
                        gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                    }
#line 240 "compiler/main.gem"
    GemVal _t151;
    if (!gem_truthy(gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length))) {
        _t151 = gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length);
    } else {
        _t151 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\""));
    }
    GemVal _t152;
    if (!gem_truthy(_t151)) {
        _t152 = _t151;
    } else {
        _t152 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(1))), gem_string("\""));
    }
    GemVal _t153;
    if (!gem_truthy(_t152)) {
        _t153 = _t152;
    } else {
        _t153 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(2))), gem_string("\""));
    }
                    if (gem_truthy(_t153)) {
#line 241 "compiler/main.gem"
                        gem_v_tq_bi = gem_v_tq_ind;
#line 242 "compiler/main.gem"
                        gem_v_tq_found = gem_bool(1);
                    } else {
#line 244 "compiler/main.gem"
                        while (1) {
                            GemVal _t154;
                            if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                        _t154 = gem_lt(gem_v_tq_scan, gem_v_length);
                            } else {
                                                        _t154 = gem_neq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t154)) break;
#line 245 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
#line 247 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
#line 248 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
                    }
                }
#line 252 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_tq_found))) {
#line 253 "compiler/main.gem"
    GemVal _t155[] = {gem_v_line};
    GemVal _t156[] = {gem_add(gem_string("unterminated triple-quoted string at line "), gem_to_string_fn(NULL, _t155, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 253, _t156, 1));
                }
#line 255 "compiler/main.gem"
                gem_v_triple_dq_indent = gem_v_tq_bi;
#line 258 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 259 "compiler/main.gem"
                GemVal gem_v_hit_interp = gem_bool(0);
#line 260 "compiler/main.gem"
                GemVal gem_v_tq_als = gem_bool(1);
#line 262 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 263 "compiler/main.gem"
                    if (gem_truthy(gem_v_tq_als)) {
#line 264 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(0);
#line 265 "compiler/main.gem"
                        GemVal gem_v_tq_skip = gem_int(0);
#line 266 "compiler/main.gem"
                        while (1) {
                            GemVal _t157;
                            if (!gem_truthy(gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent))) {
                                                        _t157 = gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t157 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t158;
                            if (!gem_truthy(_t157)) {
                                                        _t158 = _t157;
                            } else {
                                                        _t158 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t158)) break;
#line 267 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 268 "compiler/main.gem"
                            gem_v_tq_skip = gem_add(gem_v_tq_skip, gem_int(1));
                        }
#line 271 "compiler/main.gem"
    GemVal _t159;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t159 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t159 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t160;
    if (!gem_truthy(_t159)) {
        _t160 = _t159;
    } else {
        _t160 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t161;
    if (!gem_truthy(_t160)) {
        _t161 = _t160;
    } else {
        _t161 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t161)) {
#line 272 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 274 "compiler/main.gem"
    GemVal _t162[] = {gem_v_val};
    GemVal _t164;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t162, 1), gem_int(0)))) {
        _t164 = gem_gt(gem_len_fn(NULL, _t162, 1), gem_int(0));
    } else {
        GemVal _t163[] = {gem_v_val};
        _t164 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t163, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t164)) {
#line 275 "compiler/main.gem"
    GemVal _t165[] = {gem_v_val};
    GemVal _t166[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t165, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t166, 3);
                            }
                            break;
                        }
                    }
#line 281 "compiler/main.gem"
                    GemVal gem_v_tqc = gem_table_get(gem_v_source, gem_v_pos);
#line 282 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\n")))) {
#line 283 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 284 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 285 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 286 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(1);
                        continue;
                    }
#line 289 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\\")))) {
#line 290 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 291 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 292 "compiler/main.gem"
    GemVal _t167[] = {gem_v_line};
    GemVal _t168[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t167, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 292, _t168, 1));
                        }
#line 294 "compiler/main.gem"
                        GemVal gem_v_tqe = gem_table_get(gem_v_source, gem_v_pos);
#line 295 "compiler/main.gem"
    GemVal _t169[] = {gem_v_tqe, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t169, 2);
#line 296 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 297 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 299 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqe);
                        }
#line 301 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 304 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("{")))) {
#line 305 "compiler/main.gem"
    GemVal _t170 = gem_table_new();
    gem_table_set(_t170, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t170, gem_string("value"), gem_string(""));
    gem_table_set(_t170, gem_string("line"), gem_v_line);
    GemVal _t171[] = {gem_v_tokens, _t170};
                        (void)(gem_push_fn(NULL, _t171, 2));
#line 306 "compiler/main.gem"
    GemVal _t172 = gem_table_new();
    gem_table_set(_t172, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t172, gem_string("value"), gem_v_val);
    gem_table_set(_t172, gem_string("line"), gem_v_line);
    GemVal _t173[] = {gem_v_tokens, _t172};
                        (void)(gem_push_fn(NULL, _t173, 2));
#line 307 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 308 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 309 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 310 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 311 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 312 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 315 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqc);
#line 316 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 319 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 320 "compiler/main.gem"
    GemVal _t174 = gem_table_new();
    gem_table_set(_t174, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t174, gem_string("value"), gem_v_val);
    gem_table_set(_t174, gem_string("line"), gem_v_line);
    GemVal _t175[] = {gem_v_tokens, _t174};
                    (void)(gem_push_fn(NULL, _t175, 2));
                }
                continue;
            }
#line 326 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 327 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 328 "compiler/main.gem"
            while (1) {
                GemVal _t176;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t176 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t176 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t176)) break;
#line 329 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 330 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 331 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 332 "compiler/main.gem"
    GemVal _t177[] = {gem_v_line};
    GemVal _t178[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t177, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 332, _t178, 1));
                    }
#line 334 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 335 "compiler/main.gem"
    GemVal _t179[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t179, 2);
#line 336 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 337 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 339 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 341 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 344 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 345 "compiler/main.gem"
    GemVal _t180 = gem_table_new();
    gem_table_set(_t180, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t180, gem_string("value"), gem_string(""));
    gem_table_set(_t180, gem_string("line"), gem_v_line);
    GemVal _t181[] = {gem_v_tokens, _t180};
                    (void)(gem_push_fn(NULL, _t181, 2));
#line 346 "compiler/main.gem"
    GemVal _t182 = gem_table_new();
    gem_table_set(_t182, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t182, gem_string("value"), gem_v_val);
    gem_table_set(_t182, gem_string("line"), gem_v_line);
    GemVal _t183[] = {gem_v_tokens, _t182};
                    (void)(gem_push_fn(NULL, _t183, 2));
#line 347 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 348 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 349 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 350 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 351 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 354 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 355 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 357 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 358 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 359 "compiler/main.gem"
    GemVal _t184[] = {gem_v_line};
    GemVal _t185[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t184, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 359, _t185, 1));
                }
#line 361 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 362 "compiler/main.gem"
    GemVal _t186 = gem_table_new();
    gem_table_set(_t186, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t186, gem_string("value"), gem_v_val);
    gem_table_set(_t186, gem_string("line"), gem_v_line);
    GemVal _t187[] = {gem_v_tokens, _t186};
                (void)(gem_push_fn(NULL, _t187, 2));
            }
            continue;
        }
#line 368 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("'")))) {
#line 369 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 372 "compiler/main.gem"
    GemVal _t188;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t188 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t188 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t189;
    if (!gem_truthy(_t188)) {
        _t189 = _t188;
    } else {
        _t189 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
            if (gem_truthy(_t189)) {
#line 373 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 375 "compiler/main.gem"
                while (1) {
                    GemVal _t191;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t191 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t190;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t190 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t190 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t191 = _t190;
                    }
                    if (!gem_truthy(_t191)) break;
#line 376 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 378 "compiler/main.gem"
    GemVal _t192;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t192 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t192 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t192)) {
#line 379 "compiler/main.gem"
    GemVal _t193[] = {gem_v_line};
    GemVal _t194[] = {gem_add(gem_string("triple-quoted string must be followed by a newline at line "), gem_to_string_fn(NULL, _t193, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 379, _t194, 1));
                }
#line 381 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 382 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 385 "compiler/main.gem"
                GemVal gem_v_sq_scan = gem_v_pos;
#line 386 "compiler/main.gem"
                GemVal gem_v_sq_bi = gem_int(0);
#line 387 "compiler/main.gem"
                GemVal gem_v_sq_found = gem_bool(0);
#line 388 "compiler/main.gem"
                while (1) {
                    GemVal _t195;
                    if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                        _t195 = gem_lt(gem_v_sq_scan, gem_v_length);
                    } else {
                                        _t195 = gem_not(gem_v_sq_found);
                    }
                    if (!gem_truthy(_t195)) break;
#line 389 "compiler/main.gem"
                    GemVal gem_v_sq_ind = gem_int(0);
#line 390 "compiler/main.gem"
                    while (1) {
                        GemVal _t196;
                        if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                _t196 = gem_lt(gem_v_sq_scan, gem_v_length);
                        } else {
                                                _t196 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t196)) break;
#line 391 "compiler/main.gem"
                        gem_v_sq_ind = gem_add(gem_v_sq_ind, gem_int(1));
#line 392 "compiler/main.gem"
                        gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                    }
#line 394 "compiler/main.gem"
    GemVal _t197;
    if (!gem_truthy(gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length))) {
        _t197 = gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length);
    } else {
        _t197 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("'"));
    }
    GemVal _t198;
    if (!gem_truthy(_t197)) {
        _t198 = _t197;
    } else {
        _t198 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(1))), gem_string("'"));
    }
    GemVal _t199;
    if (!gem_truthy(_t198)) {
        _t199 = _t198;
    } else {
        _t199 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(2))), gem_string("'"));
    }
                    if (gem_truthy(_t199)) {
#line 395 "compiler/main.gem"
                        gem_v_sq_bi = gem_v_sq_ind;
#line 396 "compiler/main.gem"
                        gem_v_sq_found = gem_bool(1);
                    } else {
#line 398 "compiler/main.gem"
                        while (1) {
                            GemVal _t200;
                            if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                        _t200 = gem_lt(gem_v_sq_scan, gem_v_length);
                            } else {
                                                        _t200 = gem_neq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t200)) break;
#line 399 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
#line 401 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
#line 402 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
                    }
                }
#line 406 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_sq_found))) {
#line 407 "compiler/main.gem"
    GemVal _t201[] = {gem_v_line};
    GemVal _t202[] = {gem_add(gem_string("unterminated triple-quoted string at line "), gem_to_string_fn(NULL, _t201, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 407, _t202, 1));
                }
#line 411 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 412 "compiler/main.gem"
                GemVal gem_v_sq_als = gem_bool(1);
#line 414 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 415 "compiler/main.gem"
                    if (gem_truthy(gem_v_sq_als)) {
#line 416 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(0);
#line 417 "compiler/main.gem"
                        GemVal gem_v_sq_skip = gem_int(0);
#line 418 "compiler/main.gem"
                        while (1) {
                            GemVal _t203;
                            if (!gem_truthy(gem_lt(gem_v_sq_skip, gem_v_sq_bi))) {
                                                        _t203 = gem_lt(gem_v_sq_skip, gem_v_sq_bi);
                            } else {
                                                        _t203 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t204;
                            if (!gem_truthy(_t203)) {
                                                        _t204 = _t203;
                            } else {
                                                        _t204 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t204)) break;
#line 419 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 420 "compiler/main.gem"
                            gem_v_sq_skip = gem_add(gem_v_sq_skip, gem_int(1));
                        }
#line 423 "compiler/main.gem"
    GemVal _t205;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t205 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t205 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t206;
    if (!gem_truthy(_t205)) {
        _t206 = _t205;
    } else {
        _t206 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
    GemVal _t207;
    if (!gem_truthy(_t206)) {
        _t207 = _t206;
    } else {
        _t207 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("'"));
    }
                        if (gem_truthy(_t207)) {
#line 424 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 426 "compiler/main.gem"
    GemVal _t208[] = {gem_v_val};
    GemVal _t210;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t208, 1), gem_int(0)))) {
        _t210 = gem_gt(gem_len_fn(NULL, _t208, 1), gem_int(0));
    } else {
        GemVal _t209[] = {gem_v_val};
        _t210 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t209, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t210)) {
#line 427 "compiler/main.gem"
    GemVal _t211[] = {gem_v_val};
    GemVal _t212[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t211, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t212, 3);
                            }
                            break;
                        }
                    }
#line 433 "compiler/main.gem"
                    GemVal gem_v_sqc = gem_table_get(gem_v_source, gem_v_pos);
#line 434 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\n")))) {
#line 435 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 436 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 437 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 438 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(1);
                        continue;
                    }
#line 441 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\\")))) {
#line 442 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 443 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 444 "compiler/main.gem"
    GemVal _t213[] = {gem_v_line};
    GemVal _t214[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t213, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 444, _t214, 1));
                        }
#line 446 "compiler/main.gem"
                        GemVal gem_v_sqe = gem_table_get(gem_v_source, gem_v_pos);
#line 447 "compiler/main.gem"
    GemVal _t215[] = {gem_v_sqe, gem_string("sq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t215, 2);
#line 448 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 449 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 451 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_sqe);
                        }
#line 453 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 456 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_sqc);
#line 457 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 460 "compiler/main.gem"
    GemVal _t216 = gem_table_new();
    gem_table_set(_t216, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t216, gem_string("value"), gem_v_val);
    gem_table_set(_t216, gem_string("line"), gem_v_line);
    GemVal _t217[] = {gem_v_tokens, _t216};
                (void)(gem_push_fn(NULL, _t217, 2));
                continue;
            }
#line 465 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 466 "compiler/main.gem"
            while (1) {
                GemVal _t218;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t218 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t218 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
                }
                if (!gem_truthy(_t218)) break;
#line 467 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 468 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 469 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 470 "compiler/main.gem"
    GemVal _t219[] = {gem_v_line};
    GemVal _t220[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t219, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 470, _t220, 1));
                    }
#line 472 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 473 "compiler/main.gem"
    GemVal _t221[] = {gem_v_esc, gem_string("sq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t221, 2);
#line 474 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 475 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 477 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 479 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 482 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 483 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 485 "compiler/main.gem"
            if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 486 "compiler/main.gem"
    GemVal _t222[] = {gem_v_line};
    GemVal _t223[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t222, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 486, _t223, 1));
            }
#line 488 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 489 "compiler/main.gem"
    GemVal _t224 = gem_table_new();
    gem_table_set(_t224, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t224, gem_string("value"), gem_v_val);
    gem_table_set(_t224, gem_string("line"), gem_v_line);
    GemVal _t225[] = {gem_v_tokens, _t224};
            (void)(gem_push_fn(NULL, _t225, 2));
            continue;
        }
#line 494 "compiler/main.gem"
    GemVal _t226[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t226, 1))) {
#line 495 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 496 "compiler/main.gem"
            while (1) {
                GemVal _t228;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t228 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t227[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t228 = gem_fn_is_alnum(NULL, _t227, 1);
                }
                if (!gem_truthy(_t228)) break;
#line 497 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 498 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 500 "compiler/main.gem"
    GemVal _t229[] = {gem_v_kw, gem_v_val};
            if (gem_truthy(gem_has_key_fn(NULL, _t229, 2))) {
#line 501 "compiler/main.gem"
    GemVal _t230 = gem_table_new();
    gem_table_set(_t230, gem_string("type"), gem_v_val);
    gem_table_set(_t230, gem_string("value"), gem_v_val);
    gem_table_set(_t230, gem_string("line"), gem_v_line);
    GemVal _t231[] = {gem_v_tokens, _t230};
                (void)(gem_push_fn(NULL, _t231, 2));
            } else {
#line 503 "compiler/main.gem"
    GemVal _t232 = gem_table_new();
    gem_table_set(_t232, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t232, gem_string("value"), gem_v_val);
    gem_table_set(_t232, gem_string("line"), gem_v_line);
    GemVal _t233[] = {gem_v_tokens, _t232};
                (void)(gem_push_fn(NULL, _t233, 2));
            }
            continue;
        }
#line 509 "compiler/main.gem"
        GemVal gem_v_next = gem_string("");
#line 510 "compiler/main.gem"
        if (gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
#line 511 "compiler/main.gem"
            gem_v_next = gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1)));
        }
#line 514 "compiler/main.gem"
    GemVal _t234;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t234 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t234 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t234)) {
#line 515 "compiler/main.gem"
    GemVal _t235 = gem_table_new();
    gem_table_set(_t235, gem_string("type"), gem_string("=="));
    gem_table_set(_t235, gem_string("value"), gem_string("=="));
    gem_table_set(_t235, gem_string("line"), gem_v_line);
    GemVal _t236[] = {gem_v_tokens, _t235};
            (void)(gem_push_fn(NULL, _t236, 2));
#line 516 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 519 "compiler/main.gem"
    GemVal _t237;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t237 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t237 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t237)) {
#line 520 "compiler/main.gem"
    GemVal _t238 = gem_table_new();
    gem_table_set(_t238, gem_string("type"), gem_string("!="));
    gem_table_set(_t238, gem_string("value"), gem_string("!="));
    gem_table_set(_t238, gem_string("line"), gem_v_line);
    GemVal _t239[] = {gem_v_tokens, _t238};
            (void)(gem_push_fn(NULL, _t239, 2));
#line 521 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 524 "compiler/main.gem"
    GemVal _t240;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t240 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t240 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t240)) {
#line 525 "compiler/main.gem"
    GemVal _t241 = gem_table_new();
    gem_table_set(_t241, gem_string("type"), gem_string("<="));
    gem_table_set(_t241, gem_string("value"), gem_string("<="));
    gem_table_set(_t241, gem_string("line"), gem_v_line);
    GemVal _t242[] = {gem_v_tokens, _t241};
            (void)(gem_push_fn(NULL, _t242, 2));
#line 526 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 529 "compiler/main.gem"
    GemVal _t243;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t243 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t243 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t243)) {
#line 530 "compiler/main.gem"
    GemVal _t244 = gem_table_new();
    gem_table_set(_t244, gem_string("type"), gem_string(">="));
    gem_table_set(_t244, gem_string("value"), gem_string(">="));
    gem_table_set(_t244, gem_string("line"), gem_v_line);
    GemVal _t245[] = {gem_v_tokens, _t244};
            (void)(gem_push_fn(NULL, _t245, 2));
#line 531 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 534 "compiler/main.gem"
    GemVal _t246;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t246 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t246 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t246)) {
#line 535 "compiler/main.gem"
    GemVal _t247 = gem_table_new();
    gem_table_set(_t247, gem_string("type"), gem_string("+="));
    gem_table_set(_t247, gem_string("value"), gem_string("+="));
    gem_table_set(_t247, gem_string("line"), gem_v_line);
    GemVal _t248[] = {gem_v_tokens, _t247};
            (void)(gem_push_fn(NULL, _t248, 2));
#line 536 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 539 "compiler/main.gem"
    GemVal _t249;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t249 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t249 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t249)) {
#line 540 "compiler/main.gem"
    GemVal _t250 = gem_table_new();
    gem_table_set(_t250, gem_string("type"), gem_string("-="));
    gem_table_set(_t250, gem_string("value"), gem_string("-="));
    gem_table_set(_t250, gem_string("line"), gem_v_line);
    GemVal _t251[] = {gem_v_tokens, _t250};
            (void)(gem_push_fn(NULL, _t251, 2));
#line 541 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 544 "compiler/main.gem"
    GemVal _t252;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t252 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t252 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t252)) {
#line 545 "compiler/main.gem"
    GemVal _t253 = gem_table_new();
    gem_table_set(_t253, gem_string("type"), gem_string("*="));
    gem_table_set(_t253, gem_string("value"), gem_string("*="));
    gem_table_set(_t253, gem_string("line"), gem_v_line);
    GemVal _t254[] = {gem_v_tokens, _t253};
            (void)(gem_push_fn(NULL, _t254, 2));
#line 546 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 549 "compiler/main.gem"
    GemVal _t255;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t255 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t255 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t255)) {
#line 550 "compiler/main.gem"
    GemVal _t256 = gem_table_new();
    gem_table_set(_t256, gem_string("type"), gem_string("/="));
    gem_table_set(_t256, gem_string("value"), gem_string("/="));
    gem_table_set(_t256, gem_string("line"), gem_v_line);
    GemVal _t257[] = {gem_v_tokens, _t256};
            (void)(gem_push_fn(NULL, _t257, 2));
#line 551 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 554 "compiler/main.gem"
    GemVal _t258;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t258 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t258 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t258)) {
#line 555 "compiler/main.gem"
    GemVal _t259 = gem_table_new();
    gem_table_set(_t259, gem_string("type"), gem_string("->"));
    gem_table_set(_t259, gem_string("value"), gem_string("->"));
    gem_table_set(_t259, gem_string("line"), gem_v_line);
    GemVal _t260[] = {gem_v_tokens, _t259};
            (void)(gem_push_fn(NULL, _t260, 2));
#line 556 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 561 "compiler/main.gem"
    GemVal _t261;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t261 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t261 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t262;
    if (gem_truthy(_t261)) {
        _t262 = _t261;
    } else {
        _t262 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t263;
    if (gem_truthy(_t262)) {
        _t263 = _t262;
    } else {
        _t263 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t264;
    if (gem_truthy(_t263)) {
        _t264 = _t263;
    } else {
        _t264 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t264)) {
#line 562 "compiler/main.gem"
    GemVal _t265 = gem_table_new();
    gem_table_set(_t265, gem_string("type"), gem_v_ch);
    gem_table_set(_t265, gem_string("value"), gem_v_ch);
    gem_table_set(_t265, gem_string("line"), gem_v_line);
    GemVal _t266[] = {gem_v_tokens, _t265};
            (void)(gem_push_fn(NULL, _t266, 2));
#line 563 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 566 "compiler/main.gem"
    GemVal _t267;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t267 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t267 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t268;
    if (gem_truthy(_t267)) {
        _t268 = _t267;
    } else {
        _t268 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t268)) {
#line 567 "compiler/main.gem"
    GemVal _t269 = gem_table_new();
    gem_table_set(_t269, gem_string("type"), gem_v_ch);
    gem_table_set(_t269, gem_string("value"), gem_v_ch);
    gem_table_set(_t269, gem_string("line"), gem_v_line);
    GemVal _t270[] = {gem_v_tokens, _t269};
            (void)(gem_push_fn(NULL, _t270, 2));
#line 568 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 572 "compiler/main.gem"
    GemVal _t271;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t271 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t271 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t271)) {
#line 573 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 574 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 575 "compiler/main.gem"
    GemVal _t272 = gem_table_new();
    gem_table_set(_t272, gem_string("type"), gem_string("{"));
    gem_table_set(_t272, gem_string("value"), gem_string("{"));
    gem_table_set(_t272, gem_string("line"), gem_v_line);
    GemVal _t273[] = {gem_v_tokens, _t272};
            (void)(gem_push_fn(NULL, _t273, 2));
#line 576 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 579 "compiler/main.gem"
    GemVal _t274;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t274 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t274 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t274)) {
#line 580 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 581 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 582 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 583 "compiler/main.gem"
    GemVal _t275 = gem_table_new();
    gem_table_set(_t275, gem_string("type"), gem_string("}"));
    gem_table_set(_t275, gem_string("value"), gem_string("}"));
    gem_table_set(_t275, gem_string("line"), gem_v_line);
    GemVal _t276[] = {gem_v_tokens, _t275};
                (void)(gem_push_fn(NULL, _t276, 2));
#line 584 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                continue;
            }
#line 588 "compiler/main.gem"
            gem_v_interp_depth = gem_sub(gem_v_interp_depth, gem_int(1));
#line 589 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 592 "compiler/main.gem"
            if (gem_truthy(gem_v_in_triple_dq)) {
#line 593 "compiler/main.gem"
                gem_v_in_triple_dq = gem_bool(0);
#line 594 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 595 "compiler/main.gem"
                GemVal gem_v_hit_interp = gem_bool(0);
#line 596 "compiler/main.gem"
                GemVal gem_v_tqr_als = gem_bool(0);
#line 598 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 599 "compiler/main.gem"
                    if (gem_truthy(gem_v_tqr_als)) {
#line 600 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(0);
#line 601 "compiler/main.gem"
                        GemVal gem_v_tqr_skip = gem_int(0);
#line 602 "compiler/main.gem"
                        while (1) {
                            GemVal _t277;
                            if (!gem_truthy(gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent))) {
                                                        _t277 = gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t277 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t278;
                            if (!gem_truthy(_t277)) {
                                                        _t278 = _t277;
                            } else {
                                                        _t278 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t278)) break;
#line 603 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 604 "compiler/main.gem"
                            gem_v_tqr_skip = gem_add(gem_v_tqr_skip, gem_int(1));
                        }
#line 607 "compiler/main.gem"
    GemVal _t279;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t279 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t279 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t280;
    if (!gem_truthy(_t279)) {
        _t280 = _t279;
    } else {
        _t280 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t281;
    if (!gem_truthy(_t280)) {
        _t281 = _t280;
    } else {
        _t281 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t281)) {
#line 608 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 609 "compiler/main.gem"
    GemVal _t282[] = {gem_v_val};
    GemVal _t284;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t282, 1), gem_int(0)))) {
        _t284 = gem_gt(gem_len_fn(NULL, _t282, 1), gem_int(0));
    } else {
        GemVal _t283[] = {gem_v_val};
        _t284 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t283, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t284)) {
#line 610 "compiler/main.gem"
    GemVal _t285[] = {gem_v_val};
    GemVal _t286[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t285, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t286, 3);
                            }
                            break;
                        }
                    }
#line 616 "compiler/main.gem"
                    GemVal gem_v_tqrc = gem_table_get(gem_v_source, gem_v_pos);
#line 617 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("\n")))) {
#line 618 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 619 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 620 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 621 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(1);
                        continue;
                    }
#line 624 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("\\")))) {
#line 625 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 626 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 627 "compiler/main.gem"
    GemVal _t287[] = {gem_v_line};
    GemVal _t288[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t287, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 627, _t288, 1));
                        }
#line 629 "compiler/main.gem"
                        GemVal gem_v_tqre = gem_table_get(gem_v_source, gem_v_pos);
#line 630 "compiler/main.gem"
    GemVal _t289[] = {gem_v_tqre, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t289, 2);
#line 631 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 632 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 634 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqre);
                        }
#line 636 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 639 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("{")))) {
#line 640 "compiler/main.gem"
    GemVal _t290 = gem_table_new();
    gem_table_set(_t290, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t290, gem_string("value"), gem_v_val);
    gem_table_set(_t290, gem_string("line"), gem_v_line);
    GemVal _t291[] = {gem_v_tokens, _t290};
                        (void)(gem_push_fn(NULL, _t291, 2));
#line 641 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 642 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 643 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 644 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 645 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 646 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 649 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqrc);
#line 650 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 653 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 654 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 655 "compiler/main.gem"
    GemVal _t292[] = {gem_v_line};
    GemVal _t293[] = {gem_add(gem_string("unterminated triple-quoted string interpolation at line "), gem_to_string_fn(NULL, _t292, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 655, _t293, 1));
                    }
#line 657 "compiler/main.gem"
    GemVal _t294 = gem_table_new();
    gem_table_set(_t294, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t294, gem_string("value"), gem_v_val);
    gem_table_set(_t294, gem_string("line"), gem_v_line);
    GemVal _t295[] = {gem_v_tokens, _t294};
                    (void)(gem_push_fn(NULL, _t295, 2));
#line 658 "compiler/main.gem"
    GemVal _t296 = gem_table_new();
    gem_table_set(_t296, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t296, gem_string("value"), gem_string(""));
    gem_table_set(_t296, gem_string("line"), gem_v_line);
    GemVal _t297[] = {gem_v_tokens, _t296};
                    (void)(gem_push_fn(NULL, _t297, 2));
                }
                continue;
            }
#line 664 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 665 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 666 "compiler/main.gem"
            while (1) {
                GemVal _t298;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t298 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t298 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t298)) break;
#line 667 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 668 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 669 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 670 "compiler/main.gem"
    GemVal _t299[] = {gem_v_line};
    GemVal _t300[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t299, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 670, _t300, 1));
                    }
#line 672 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 673 "compiler/main.gem"
    GemVal _t301[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t301, 2);
#line 674 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 675 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 677 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 679 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 682 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 684 "compiler/main.gem"
    GemVal _t302 = gem_table_new();
    gem_table_set(_t302, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t302, gem_string("value"), gem_v_val);
    gem_table_set(_t302, gem_string("line"), gem_v_line);
    GemVal _t303[] = {gem_v_tokens, _t302};
                    (void)(gem_push_fn(NULL, _t303, 2));
#line 685 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 686 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 687 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 688 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 689 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 692 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 693 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 695 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 696 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 697 "compiler/main.gem"
    GemVal _t304[] = {gem_v_line};
    GemVal _t305[] = {gem_add(gem_string("unterminated string interpolation at line "), gem_to_string_fn(NULL, _t304, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 697, _t305, 1));
                }
#line 699 "compiler/main.gem"
    GemVal _t306 = gem_table_new();
    gem_table_set(_t306, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t306, gem_string("value"), gem_v_val);
    gem_table_set(_t306, gem_string("line"), gem_v_line);
    GemVal _t307[] = {gem_v_tokens, _t306};
                (void)(gem_push_fn(NULL, _t307, 2));
#line 700 "compiler/main.gem"
    GemVal _t308 = gem_table_new();
    gem_table_set(_t308, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t308, gem_string("value"), gem_string(""));
    gem_table_set(_t308, gem_string("line"), gem_v_line);
    GemVal _t309[] = {gem_v_tokens, _t308};
                (void)(gem_push_fn(NULL, _t309, 2));
#line 701 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 706 "compiler/main.gem"
    GemVal _t310;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t310 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t310 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t311;
    if (gem_truthy(_t310)) {
        _t311 = _t310;
    } else {
        _t311 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t312;
    if (gem_truthy(_t311)) {
        _t312 = _t311;
    } else {
        _t312 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t313;
    if (gem_truthy(_t312)) {
        _t313 = _t312;
    } else {
        _t313 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t314;
    if (gem_truthy(_t313)) {
        _t314 = _t313;
    } else {
        _t314 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t314)) {
#line 707 "compiler/main.gem"
    GemVal _t315 = gem_table_new();
    gem_table_set(_t315, gem_string("type"), gem_v_ch);
    gem_table_set(_t315, gem_string("value"), gem_v_ch);
    gem_table_set(_t315, gem_string("line"), gem_v_line);
    GemVal _t316[] = {gem_v_tokens, _t315};
            (void)(gem_push_fn(NULL, _t316, 2));
#line 708 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 711 "compiler/main.gem"
    GemVal _t317;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t317 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t317 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    }
    GemVal _t318;
    if (!gem_truthy(_t317)) {
        _t318 = _t317;
    } else {
        _t318 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("."));
    }
    GemVal _t319;
    if (!gem_truthy(_t318)) {
        _t319 = _t318;
    } else {
        _t319 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    }
    GemVal _t320;
    if (!gem_truthy(_t319)) {
        _t320 = _t319;
    } else {
        _t320 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("."));
    }
        if (gem_truthy(_t320)) {
#line 712 "compiler/main.gem"
    GemVal _t321 = gem_table_new();
    gem_table_set(_t321, gem_string("type"), gem_string("..."));
    gem_table_set(_t321, gem_string("value"), gem_string("..."));
    gem_table_set(_t321, gem_string("line"), gem_v_line);
    GemVal _t322[] = {gem_v_tokens, _t321};
            (void)(gem_push_fn(NULL, _t322, 2));
#line 713 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
            continue;
        }
#line 716 "compiler/main.gem"
    GemVal _t323;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t323 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t323 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t324;
    if (gem_truthy(_t323)) {
        _t324 = _t323;
    } else {
        _t324 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t325;
    if (gem_truthy(_t324)) {
        _t325 = _t324;
    } else {
        _t325 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t325)) {
#line 717 "compiler/main.gem"
    GemVal _t326 = gem_table_new();
    gem_table_set(_t326, gem_string("type"), gem_v_ch);
    gem_table_set(_t326, gem_string("value"), gem_v_ch);
    gem_table_set(_t326, gem_string("line"), gem_v_line);
    GemVal _t327[] = {gem_v_tokens, _t326};
            (void)(gem_push_fn(NULL, _t327, 2));
#line 718 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 722 "compiler/main.gem"
    GemVal _t328[] = {gem_v_ch};
    GemVal _t329[] = {gem_v_line};
    GemVal _t330[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t328, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t329, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 722, _t330, 1));
    }
#line 725 "compiler/main.gem"
    GemVal _t331 = gem_table_new();
    gem_table_set(_t331, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t331, gem_string("value"), gem_string(""));
    gem_table_set(_t331, gem_string("line"), gem_v_line);
    GemVal _t332[] = {gem_v_tokens, _t331};
    (void)(gem_push_fn(NULL, _t332, 2));
    GemVal _t333 = gem_v_tokens;
    gem_pop_frame();
    return _t333;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 10 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_source", "compiler/main.gem", 10);
#line 11 "compiler/main.gem"
    GemVal _t334[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t334, 1);
#line 12 "compiler/main.gem"
    GemVal _t335[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t335, 1);
#line 13 "compiler/main.gem"
    GemVal _t336 = gem_v_p;
    GemVal _t337 = gem_table_get(_t336, gem_string("parse"));
    GemVal _t338 = _t337.fn(_t337.env, NULL, 0);
    gem_pop_frame();
    return _t338;
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
    GemVal _t339 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t339;
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
    GemVal _t341 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t341;
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
    GemVal _t343 = gem_v_t;
    gem_pop_frame();
    return _t343;
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
    GemVal _t345 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t346 = gem_eq(gem_table_get(_t345, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t346;
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
    GemVal _t348 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t348, gem_string("type")), gem_v_tp))) {
#line 46 "compiler/main.gem"
    GemVal _t349[] = {gem_v_tp};
    GemVal _t350 = gem_v_t;
    GemVal _t351[] = {gem_table_get(_t350, gem_string("type"))};
    GemVal _t352 = gem_v_t;
    GemVal _t353[] = {gem_table_get(_t352, gem_string("line"))};
    GemVal _t354[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t349, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t351, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t353, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 46, _t354, 1));
    }
#line 48 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t355 = gem_v_t;
    gem_pop_frame();
    return _t355;
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
        GemVal _t357 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t357, gem_string("type")), gem_string("NEWLINE")))) break;
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
    GemVal _t359 = gem_table_new();
    GemVal gem_v_params = _t359;
#line 66 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 67 "compiler/main.gem"
    GemVal gem_v_block_param = GEM_NIL;
#line 69 "compiler/main.gem"
    while (1) {
        GemVal _t360 = (*gem_v_peek);
        GemVal _t361 = _t360.fn(_t360.env, NULL, 0);
        GemVal _t364;
        if (gem_truthy(gem_eq(gem_table_get(_t361, gem_string("type")), gem_string("NAME")))) {
                _t364 = gem_eq(gem_table_get(_t361, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t362 = (*gem_v_peek);
                GemVal _t363 = _t362.fn(_t362.env, NULL, 0);
                _t364 = gem_eq(gem_table_get(_t363, gem_string("type")), gem_string("..."));
        }
        if (!gem_truthy(_t364)) break;
#line 70 "compiler/main.gem"
    GemVal _t365 = (*gem_v_peek);
    GemVal _t366 = _t365.fn(_t365.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t366, gem_string("type")), gem_string("...")))) {
#line 71 "compiler/main.gem"
    GemVal _t367 = (*gem_v_advance);
            (void)(_t367.fn(_t367.env, NULL, 0));
#line 72 "compiler/main.gem"
    GemVal _t368[] = {gem_string("NAME")};
    GemVal _t369 = (*gem_v_expect);
    GemVal _t370 = _t369.fn(_t369.env, _t368, 1);
            gem_v_rest_param = gem_table_get(_t370, gem_string("value"));
#line 73 "compiler/main.gem"
    GemVal _t371 = (*gem_v_peek);
    GemVal _t372 = _t371.fn(_t371.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t372, gem_string("type")), gem_string(",")))) {
#line 74 "compiler/main.gem"
    GemVal _t373 = (*gem_v_advance);
                (void)(_t373.fn(_t373.env, NULL, 0));
#line 75 "compiler/main.gem"
    GemVal _t374 = (*gem_v_peek);
    GemVal _t375 = _t374.fn(_t374.env, NULL, 0);
                if (gem_truthy(gem_neq(gem_table_get(_t375, gem_string("type")), gem_string("NAME")))) {
#line 76 "compiler/main.gem"
    GemVal _t376 = (*gem_v_peek);
    GemVal _t377 = _t376.fn(_t376.env, NULL, 0);
    GemVal _t378[] = {gem_table_get(_t377, gem_string("line"))};
    GemVal _t379[] = {gem_add(gem_string("expected parameter name after rest param at line "), gem_to_string_fn(NULL, _t378, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 76, _t379, 1));
                }
#line 78 "compiler/main.gem"
    GemVal _t380 = (*gem_v_advance);
    GemVal _t381 = _t380.fn(_t380.env, NULL, 0);
                gem_v_block_param = gem_table_get(_t381, gem_string("value"));
#line 79 "compiler/main.gem"
    GemVal _t382 = (*gem_v_peek);
    GemVal _t383 = _t382.fn(_t382.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t383, gem_string("type")), gem_string(",")))) {
#line 80 "compiler/main.gem"
    GemVal _t384 = (*gem_v_peek);
    GemVal _t385 = _t384.fn(_t384.env, NULL, 0);
    GemVal _t386[] = {gem_table_get(_t385, gem_string("line"))};
    GemVal _t387[] = {gem_add(gem_string("only one parameter is allowed after a rest param at line "), gem_to_string_fn(NULL, _t386, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 80, _t387, 1));
                }
            }
            break;
        }
#line 85 "compiler/main.gem"
    GemVal _t388 = (*gem_v_advance);
    GemVal _t389 = _t388.fn(_t388.env, NULL, 0);
    GemVal _t390[] = {gem_v_params, gem_table_get(_t389, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t390, 2));
#line 86 "compiler/main.gem"
    GemVal _t391 = (*gem_v_peek);
    GemVal _t392 = _t391.fn(_t391.env, NULL, 0);
        if (gem_truthy(gem_neq(gem_table_get(_t392, gem_string("type")), gem_string(",")))) {
            break;
        }
#line 89 "compiler/main.gem"
    GemVal _t393 = (*gem_v_advance);
        (void)(_t393.fn(_t393.env, NULL, 0));
    }
    GemVal _t394 = gem_table_new();
    gem_table_set(_t394, gem_string("params"), gem_v_params);
    gem_table_set(_t394, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t394, gem_string("block_param"), gem_v_block_param);
    GemVal _t395 = _t394;
    gem_pop_frame();
    return _t395;
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
    GemVal _t397 = gem_table_new();
    GemVal gem_v_stmts = _t397;
#line 108 "compiler/main.gem"
    GemVal _t398 = (*gem_v_skip_nl);
    (void)(_t398.fn(_t398.env, NULL, 0));
#line 109 "compiler/main.gem"
    while (1) {
        GemVal _t399 = (*gem_v_peek);
        GemVal _t400 = _t399.fn(_t399.env, NULL, 0);
        GemVal _t403;
        if (!gem_truthy(gem_neq(gem_table_get(_t400, gem_string("type")), gem_string("end")))) {
                _t403 = gem_neq(gem_table_get(_t400, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t401 = (*gem_v_peek);
                GemVal _t402 = _t401.fn(_t401.env, NULL, 0);
                _t403 = gem_neq(gem_table_get(_t402, gem_string("type")), gem_string("elif"));
        }
        GemVal _t406;
        if (!gem_truthy(_t403)) {
                _t406 = _t403;
        } else {
                GemVal _t404 = (*gem_v_peek);
                GemVal _t405 = _t404.fn(_t404.env, NULL, 0);
                _t406 = gem_neq(gem_table_get(_t405, gem_string("type")), gem_string("else"));
        }
        GemVal _t409;
        if (!gem_truthy(_t406)) {
                _t409 = _t406;
        } else {
                GemVal _t407 = (*gem_v_peek);
                GemVal _t408 = _t407.fn(_t407.env, NULL, 0);
                _t409 = gem_neq(gem_table_get(_t408, gem_string("type")), gem_string("when"));
        }
        GemVal _t411;
        if (!gem_truthy(_t409)) {
                _t411 = _t409;
        } else {
                GemVal _t410 = (*gem_v_at_end);
                _t411 = gem_not(_t410.fn(_t410.env, NULL, 0));
        }
        if (!gem_truthy(_t411)) break;
#line 110 "compiler/main.gem"
    GemVal _t412 = (*gem_v_parse_stmt);
    GemVal _t413[] = {gem_v_stmts, _t412.fn(_t412.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t413, 2));
#line 111 "compiler/main.gem"
    GemVal _t414 = (*gem_v_skip_nl);
        (void)(_t414.fn(_t414.env, NULL, 0));
    }
    GemVal _t415 = gem_v_stmts;
    gem_pop_frame();
    return _t415;
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
    GemVal _t417 = gem_table_new();
    GemVal gem_v_stmts = _t417;
#line 121 "compiler/main.gem"
    GemVal _t418 = (*gem_v_skip_nl);
    (void)(_t418.fn(_t418.env, NULL, 0));
#line 122 "compiler/main.gem"
    while (1) {
        GemVal _t419 = (*gem_v_peek);
        GemVal _t420 = _t419.fn(_t419.env, NULL, 0);
        GemVal _t422;
        if (!gem_truthy(gem_neq(gem_table_get(_t420, gem_string("type")), gem_string("end")))) {
                _t422 = gem_neq(gem_table_get(_t420, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t421 = (*gem_v_at_end);
                _t422 = gem_not(_t421.fn(_t421.env, NULL, 0));
        }
        if (!gem_truthy(_t422)) break;
#line 123 "compiler/main.gem"
    GemVal _t423 = (*gem_v_parse_stmt);
    GemVal _t424[] = {gem_v_stmts, _t423.fn(_t423.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t424, 2));
#line 124 "compiler/main.gem"
    GemVal _t425 = (*gem_v_skip_nl);
        (void)(_t425.fn(_t425.env, NULL, 0));
    }
    GemVal _t426 = gem_v_stmts;
    gem_pop_frame();
    return _t426;
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
    GemVal _t428 = (*gem_v_peek);
    GemVal gem_v_t = _t428.fn(_t428.env, NULL, 0);
#line 136 "compiler/main.gem"
    GemVal _t429 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t429, gem_string("type")), gem_string("NUMBER")))) {
#line 137 "compiler/main.gem"
    GemVal _t430 = (*gem_v_advance);
        (void)(_t430.fn(_t430.env, NULL, 0));
#line 138 "compiler/main.gem"
    GemVal _t431 = gem_v_t;
    GemVal _t432[] = {gem_table_get(_t431, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t432, 1))) {
#line 139 "compiler/main.gem"
    GemVal _t433 = gem_v_t;
    GemVal _t434[] = {gem_table_get(_t433, gem_string("value"))};
    GemVal _t435[] = {gem_fn_atof(NULL, _t434, 1)};
            GemVal _t436 = gem_fn_make_float(NULL, _t435, 1);
            gem_pop_frame();
            return _t436;
        }
#line 141 "compiler/main.gem"
    GemVal _t437 = gem_v_t;
    GemVal _t438[] = {gem_table_get(_t437, gem_string("value"))};
    GemVal _t439[] = {gem_fn_str_to_int(NULL, _t438, 1)};
        GemVal _t440 = gem_fn_make_int(NULL, _t439, 1);
        gem_pop_frame();
        return _t440;
    }
#line 145 "compiler/main.gem"
    GemVal _t441 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t441, gem_string("type")), gem_string("STRING")))) {
#line 146 "compiler/main.gem"
    GemVal _t442 = (*gem_v_advance);
        (void)(_t442.fn(_t442.env, NULL, 0));
#line 147 "compiler/main.gem"
    GemVal _t443 = gem_v_t;
    GemVal _t444[] = {gem_table_get(_t443, gem_string("value"))};
        GemVal _t445 = gem_fn_make_string(NULL, _t444, 1);
        gem_pop_frame();
        return _t445;
    }
#line 151 "compiler/main.gem"
    GemVal _t446 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t446, gem_string("type")), gem_string("INTERP_START")))) {
#line 152 "compiler/main.gem"
    GemVal _t447 = (*gem_v_advance);
        (void)(_t447.fn(_t447.env, NULL, 0));
#line 153 "compiler/main.gem"
    GemVal _t448 = gem_table_new();
        GemVal gem_v_parts = _t448;
#line 154 "compiler/main.gem"
    GemVal _t449 = (*gem_v_skip_nl);
        (void)(_t449.fn(_t449.env, NULL, 0));
#line 155 "compiler/main.gem"
        while (1) {
            GemVal _t450 = (*gem_v_peek);
            GemVal _t451 = _t450.fn(_t450.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t451, gem_string("type")), gem_string("INTERP_END")))) break;
#line 156 "compiler/main.gem"
    GemVal _t452 = (*gem_v_peek);
    GemVal _t453 = _t452.fn(_t452.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t453, gem_string("type")), gem_string("STRING")))) {
#line 157 "compiler/main.gem"
    GemVal _t454 = (*gem_v_advance);
    GemVal _t455 = _t454.fn(_t454.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t455, gem_string("value"));
#line 158 "compiler/main.gem"
    GemVal _t456[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t456, 1), gem_int(0)))) {
#line 159 "compiler/main.gem"
    GemVal _t457[] = {gem_v_sval};
    GemVal _t458[] = {gem_v_parts, gem_fn_make_string(NULL, _t457, 1)};
                    (void)(gem_push_fn(NULL, _t458, 2));
                }
            } else {
#line 162 "compiler/main.gem"
    GemVal _t459 = (*gem_v_parse_expr);
    GemVal _t460[] = {gem_v_parts, _t459.fn(_t459.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t460, 2));
            }
#line 164 "compiler/main.gem"
    GemVal _t461 = (*gem_v_skip_nl);
            (void)(_t461.fn(_t461.env, NULL, 0));
        }
#line 166 "compiler/main.gem"
    GemVal _t462[] = {gem_string("INTERP_END")};
    GemVal _t463 = (*gem_v_expect);
        (void)(_t463.fn(_t463.env, _t462, 1));
#line 167 "compiler/main.gem"
    GemVal _t464[] = {gem_v_parts};
        GemVal _t465 = gem_fn_make_interp(NULL, _t464, 1);
        gem_pop_frame();
        return _t465;
    }
#line 171 "compiler/main.gem"
    GemVal _t466 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t466, gem_string("type")), gem_string("true")))) {
#line 172 "compiler/main.gem"
    GemVal _t467 = (*gem_v_advance);
        (void)(_t467.fn(_t467.env, NULL, 0));
#line 173 "compiler/main.gem"
    GemVal _t468[] = {gem_bool(1)};
        GemVal _t469 = gem_fn_make_bool(NULL, _t468, 1);
        gem_pop_frame();
        return _t469;
    }
#line 175 "compiler/main.gem"
    GemVal _t470 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t470, gem_string("type")), gem_string("false")))) {
#line 176 "compiler/main.gem"
    GemVal _t471 = (*gem_v_advance);
        (void)(_t471.fn(_t471.env, NULL, 0));
#line 177 "compiler/main.gem"
    GemVal _t472[] = {gem_bool(0)};
        GemVal _t473 = gem_fn_make_bool(NULL, _t472, 1);
        gem_pop_frame();
        return _t473;
    }
#line 181 "compiler/main.gem"
    GemVal _t474 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t474, gem_string("type")), gem_string("nil")))) {
#line 182 "compiler/main.gem"
    GemVal _t475 = (*gem_v_advance);
        (void)(_t475.fn(_t475.env, NULL, 0));
#line 183 "compiler/main.gem"
        GemVal _t476 = gem_fn_make_nil_node(NULL, NULL, 0);
        gem_pop_frame();
        return _t476;
    }
#line 187 "compiler/main.gem"
    GemVal _t477 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t477, gem_string("type")), gem_string("fn")))) {
#line 188 "compiler/main.gem"
    GemVal _t478 = (*gem_v_advance);
        (void)(_t478.fn(_t478.env, NULL, 0));
#line 189 "compiler/main.gem"
    GemVal _t479[] = {gem_string("(")};
    GemVal _t480 = (*gem_v_expect);
        (void)(_t480.fn(_t480.env, _t479, 1));
#line 190 "compiler/main.gem"
    GemVal _t481 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t481.fn(_t481.env, NULL, 0);
#line 191 "compiler/main.gem"
    GemVal _t482[] = {gem_string(")")};
    GemVal _t483 = (*gem_v_expect);
        (void)(_t483.fn(_t483.env, _t482, 1));
#line 192 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 193 "compiler/main.gem"
    GemVal _t484 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t484.fn(_t484.env, NULL, 0);
#line 194 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 195 "compiler/main.gem"
    GemVal _t485[] = {gem_string("end")};
    GemVal _t486 = (*gem_v_expect);
        (void)(_t486.fn(_t486.env, _t485, 1));
#line 196 "compiler/main.gem"
    GemVal _t487 = gem_v_pr;
    GemVal _t488 = gem_v_pr;
    GemVal _t489 = gem_v_pr;
    GemVal _t490[] = {gem_table_get(_t487, gem_string("params")), gem_table_get(_t488, gem_string("rest_param")), gem_table_get(_t489, gem_string("block_param")), gem_v_body};
        GemVal _t491 = gem_fn_make_anon_fn(NULL, _t490, 4);
        gem_pop_frame();
        return _t491;
    }
#line 201 "compiler/main.gem"
    GemVal _t492 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t492, gem_string("type")), gem_string("{")))) {
#line 202 "compiler/main.gem"
    GemVal _t493 = (*gem_v_advance);
        (void)(_t493.fn(_t493.env, NULL, 0));
#line 203 "compiler/main.gem"
    GemVal _t494 = (*gem_v_skip_nl);
        (void)(_t494.fn(_t494.env, NULL, 0));
#line 204 "compiler/main.gem"
    GemVal _t495 = gem_table_new();
        GemVal gem_v_entries = _t495;
#line 205 "compiler/main.gem"
        while (1) {
            GemVal _t496 = (*gem_v_peek);
            GemVal _t497 = _t496.fn(_t496.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t497, gem_string("type")), gem_string("}")))) break;
#line 206 "compiler/main.gem"
    GemVal _t498 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t498.fn(_t498.env, NULL, 0);
#line 207 "compiler/main.gem"
    GemVal _t499 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t499, gem_string("type")), gem_string("NAME")))) {
#line 208 "compiler/main.gem"
    GemVal _t500 = (*gem_v_advance);
                (void)(_t500.fn(_t500.env, NULL, 0));
            } else {
#line 209 "compiler/main.gem"
    GemVal _t501[] = {gem_int(1)};
    GemVal _t502 = (*gem_v_peek_at);
    GemVal _t503 = _t502.fn(_t502.env, _t501, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t503, gem_string("type")), gem_string(":")))) {
#line 211 "compiler/main.gem"
    GemVal _t504 = (*gem_v_advance);
                    (void)(_t504.fn(_t504.env, NULL, 0));
                } else {
#line 213 "compiler/main.gem"
    GemVal _t505 = gem_v_key_tok;
    GemVal _t506[] = {gem_table_get(_t505, gem_string("type"))};
    GemVal _t507 = gem_v_key_tok;
    GemVal _t508[] = {gem_table_get(_t507, gem_string("line"))};
    GemVal _t509[] = {gem_add(gem_add(gem_add(gem_string("expected table key (name or keyword) but got '"), gem_to_string_fn(NULL, _t506, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t508, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 213, _t509, 1));
                }
            }
#line 215 "compiler/main.gem"
    GemVal _t510[] = {gem_string(":")};
    GemVal _t511 = (*gem_v_expect);
            (void)(_t511.fn(_t511.env, _t510, 1));
#line 216 "compiler/main.gem"
    GemVal _t512 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t512.fn(_t512.env, NULL, 0);
#line 217 "compiler/main.gem"
    GemVal _t513 = gem_v_key_tok;
    GemVal _t514[] = {gem_table_get(_t513, gem_string("value")), gem_v_val};
    GemVal _t515[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t514, 2)};
            (void)(gem_push_fn(NULL, _t515, 2));
#line 218 "compiler/main.gem"
    GemVal _t516 = (*gem_v_skip_nl);
            (void)(_t516.fn(_t516.env, NULL, 0));
#line 219 "compiler/main.gem"
    GemVal _t517 = (*gem_v_peek);
    GemVal _t518 = _t517.fn(_t517.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t518, gem_string("type")), gem_string(",")))) {
#line 220 "compiler/main.gem"
    GemVal _t519 = (*gem_v_advance);
                (void)(_t519.fn(_t519.env, NULL, 0));
#line 221 "compiler/main.gem"
    GemVal _t520 = (*gem_v_skip_nl);
                (void)(_t520.fn(_t520.env, NULL, 0));
            }
        }
#line 224 "compiler/main.gem"
    GemVal _t521[] = {gem_string("}")};
    GemVal _t522 = (*gem_v_expect);
        (void)(_t522.fn(_t522.env, _t521, 1));
#line 225 "compiler/main.gem"
    GemVal _t523[] = {gem_v_entries};
        GemVal _t524 = gem_fn_make_table(NULL, _t523, 1);
        gem_pop_frame();
        return _t524;
    }
#line 229 "compiler/main.gem"
    GemVal _t525 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t525, gem_string("type")), gem_string("[")))) {
#line 230 "compiler/main.gem"
    GemVal _t526 = (*gem_v_advance);
        (void)(_t526.fn(_t526.env, NULL, 0));
#line 231 "compiler/main.gem"
    GemVal _t527 = (*gem_v_skip_nl);
        (void)(_t527.fn(_t527.env, NULL, 0));
#line 232 "compiler/main.gem"
    GemVal _t528 = gem_table_new();
        GemVal gem_v_elements = _t528;
#line 233 "compiler/main.gem"
        while (1) {
            GemVal _t529 = (*gem_v_peek);
            GemVal _t530 = _t529.fn(_t529.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t530, gem_string("type")), gem_string("]")))) break;
#line 234 "compiler/main.gem"
    GemVal _t531 = (*gem_v_parse_expr);
    GemVal _t532[] = {gem_v_elements, _t531.fn(_t531.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t532, 2));
#line 235 "compiler/main.gem"
    GemVal _t533 = (*gem_v_skip_nl);
            (void)(_t533.fn(_t533.env, NULL, 0));
#line 236 "compiler/main.gem"
    GemVal _t534 = (*gem_v_peek);
    GemVal _t535 = _t534.fn(_t534.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t535, gem_string("type")), gem_string(",")))) {
#line 237 "compiler/main.gem"
    GemVal _t536 = (*gem_v_advance);
                (void)(_t536.fn(_t536.env, NULL, 0));
#line 238 "compiler/main.gem"
    GemVal _t537 = (*gem_v_skip_nl);
                (void)(_t537.fn(_t537.env, NULL, 0));
            }
        }
#line 241 "compiler/main.gem"
    GemVal _t538[] = {gem_string("]")};
    GemVal _t539 = (*gem_v_expect);
        (void)(_t539.fn(_t539.env, _t538, 1));
#line 242 "compiler/main.gem"
    GemVal _t540[] = {gem_v_elements};
        GemVal _t541 = gem_fn_make_array(NULL, _t540, 1);
        gem_pop_frame();
        return _t541;
    }
#line 246 "compiler/main.gem"
    GemVal _t542 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t542, gem_string("type")), gem_string("(")))) {
#line 247 "compiler/main.gem"
    GemVal _t543 = (*gem_v_advance);
        (void)(_t543.fn(_t543.env, NULL, 0));
#line 248 "compiler/main.gem"
    GemVal _t544 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t544.fn(_t544.env, NULL, 0);
#line 249 "compiler/main.gem"
    GemVal _t545[] = {gem_string(")")};
    GemVal _t546 = (*gem_v_expect);
        (void)(_t546.fn(_t546.env, _t545, 1));
#line 250 "compiler/main.gem"
        GemVal _t547 = gem_v_e;
        gem_pop_frame();
        return _t547;
    }
#line 254 "compiler/main.gem"
    GemVal _t548 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t548, gem_string("type")), gem_string("NAME")))) {
#line 255 "compiler/main.gem"
    GemVal _t549 = (*gem_v_advance);
        (void)(_t549.fn(_t549.env, NULL, 0));
#line 256 "compiler/main.gem"
    GemVal _t550 = gem_v_t;
    GemVal _t551[] = {gem_table_get(_t550, gem_string("value"))};
        GemVal _t552 = gem_fn_make_var(NULL, _t551, 1);
        gem_pop_frame();
        return _t552;
    }
#line 259 "compiler/main.gem"
    GemVal _t553 = gem_v_t;
    GemVal _t554[] = {gem_table_get(_t553, gem_string("type"))};
    GemVal _t555 = gem_v_t;
    GemVal _t556[] = {gem_table_get(_t555, gem_string("line"))};
    GemVal _t557[] = {gem_add(gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t554, 1)), gem_string("' (expected expression) at line ")), gem_to_string_fn(NULL, _t556, 1))};
    GemVal _t558 = gem_error_at_fn("compiler/main.gem", 259, _t557, 1);
    gem_pop_frame();
    return _t558;
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
    GemVal _t560 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t560.fn(_t560.env, NULL, 0);
#line 266 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
#line 268 "compiler/main.gem"
    GemVal _t561 = (*gem_v_peek);
    GemVal _t562 = _t561.fn(_t561.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t562, gem_string("type")), gem_string("(")))) {
#line 269 "compiler/main.gem"
    GemVal _t563 = (*gem_v_peek);
    GemVal _t564 = _t563.fn(_t563.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t564, gem_string("line"));
#line 270 "compiler/main.gem"
    GemVal _t565 = (*gem_v_advance);
            (void)(_t565.fn(_t565.env, NULL, 0));
#line 271 "compiler/main.gem"
    GemVal _t566 = (*gem_v_skip_nl);
            (void)(_t566.fn(_t566.env, NULL, 0));
#line 272 "compiler/main.gem"
    GemVal _t567 = gem_table_new();
            GemVal gem_v_args = _t567;
#line 273 "compiler/main.gem"
    GemVal _t568 = (*gem_v_peek);
    GemVal _t569 = _t568.fn(_t568.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t569, gem_string("type")), gem_string(")")))) {
#line 274 "compiler/main.gem"
    GemVal _t570 = (*gem_v_parse_expr);
    GemVal _t571[] = {gem_v_args, _t570.fn(_t570.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t571, 2));
#line 275 "compiler/main.gem"
                while (1) {
                    GemVal _t572 = (*gem_v_peek);
                    GemVal _t573 = _t572.fn(_t572.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t573, gem_string("type")), gem_string(",")))) break;
#line 276 "compiler/main.gem"
    GemVal _t574 = (*gem_v_advance);
                    (void)(_t574.fn(_t574.env, NULL, 0));
#line 277 "compiler/main.gem"
    GemVal _t575 = (*gem_v_skip_nl);
                    (void)(_t575.fn(_t575.env, NULL, 0));
#line 278 "compiler/main.gem"
    GemVal _t576 = (*gem_v_parse_expr);
    GemVal _t577[] = {gem_v_args, _t576.fn(_t576.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t577, 2));
                }
            }
#line 281 "compiler/main.gem"
    GemVal _t578 = (*gem_v_skip_nl);
            (void)(_t578.fn(_t578.env, NULL, 0));
#line 282 "compiler/main.gem"
    GemVal _t579[] = {gem_string(")")};
    GemVal _t580 = (*gem_v_expect);
            (void)(_t580.fn(_t580.env, _t579, 1));
#line 285 "compiler/main.gem"
    GemVal _t581 = (*gem_v_peek);
    GemVal _t582 = _t581.fn(_t581.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t582, gem_string("type")), gem_string("do")))) {
#line 286 "compiler/main.gem"
    GemVal _t583 = (*gem_v_advance);
                (void)(_t583.fn(_t583.env, NULL, 0));
#line 287 "compiler/main.gem"
    GemVal _t584 = gem_table_new();
                GemVal gem_v_bparams = _t584;
#line 288 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 289 "compiler/main.gem"
                GemVal gem_v_bblock = GEM_NIL;
#line 290 "compiler/main.gem"
    GemVal _t585 = (*gem_v_peek);
    GemVal _t586 = _t585.fn(_t585.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t586, gem_string("type")), gem_string("|")))) {
#line 291 "compiler/main.gem"
    GemVal _t587 = (*gem_v_advance);
                    (void)(_t587.fn(_t587.env, NULL, 0));
#line 292 "compiler/main.gem"
    GemVal _t588 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t588.fn(_t588.env, NULL, 0);
#line 293 "compiler/main.gem"
    GemVal _t589 = gem_v_bpr;
                    gem_v_bparams = gem_table_get(_t589, gem_string("params"));
#line 294 "compiler/main.gem"
    GemVal _t590 = gem_v_bpr;
                    gem_v_brest = gem_table_get(_t590, gem_string("rest_param"));
#line 295 "compiler/main.gem"
    GemVal _t591 = gem_v_bpr;
                    gem_v_bblock = gem_table_get(_t591, gem_string("block_param"));
#line 296 "compiler/main.gem"
    GemVal _t592[] = {gem_string("|")};
    GemVal _t593 = (*gem_v_expect);
                    (void)(_t593.fn(_t593.env, _t592, 1));
                }
#line 298 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 299 "compiler/main.gem"
    GemVal _t594 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t594.fn(_t594.env, NULL, 0);
#line 300 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 301 "compiler/main.gem"
    GemVal _t595[] = {gem_string("end")};
    GemVal _t596 = (*gem_v_expect);
                (void)(_t596.fn(_t596.env, _t595, 1));
#line 302 "compiler/main.gem"
    GemVal _t597[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, gem_v_body};
    GemVal _t598[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t597, 4)};
                (void)(gem_push_fn(NULL, _t598, 2));
            } else {
#line 306 "compiler/main.gem"
    GemVal _t599 = (*gem_v_peek);
    GemVal _t600 = _t599.fn(_t599.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t600, gem_string("type")), gem_string("{")))) {
#line 307 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 308 "compiler/main.gem"
    GemVal _t601[] = {gem_int(1)};
    GemVal _t602 = (*gem_v_peek_at);
    GemVal _t603 = _t602.fn(_t602.env, _t601, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t603, gem_string("type")), gem_string("|")))) {
#line 309 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 310 "compiler/main.gem"
    GemVal _t604[] = {gem_int(1)};
    GemVal _t605 = (*gem_v_peek_at);
    GemVal _t606 = _t605.fn(_t605.env, _t604, 1);
    GemVal _t614;
    if (gem_truthy(gem_eq(gem_table_get(_t606, gem_string("type")), gem_string("}")))) {
        _t614 = gem_eq(gem_table_get(_t606, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t607[] = {gem_int(1)};
        GemVal _t608 = (*gem_v_peek_at);
        GemVal _t609 = _t608.fn(_t608.env, _t607, 1);
        GemVal _t613;
        if (!gem_truthy(gem_eq(gem_table_get(_t609, gem_string("type")), gem_string("NAME")))) {
                _t613 = gem_eq(gem_table_get(_t609, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t610[] = {gem_int(2)};
                GemVal _t611 = (*gem_v_peek_at);
                GemVal _t612 = _t611.fn(_t611.env, _t610, 1);
                _t613 = gem_eq(gem_table_get(_t612, gem_string("type")), gem_string(":"));
        }
        _t614 = _t613;
    }
                        if (gem_truthy(_t614)) {
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
    GemVal _t615 = (*gem_v_advance);
                        (void)(_t615.fn(_t615.env, NULL, 0));
#line 317 "compiler/main.gem"
    GemVal _t616 = gem_table_new();
                        GemVal gem_v_bparams = _t616;
#line 318 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 319 "compiler/main.gem"
                        GemVal gem_v_bblock = GEM_NIL;
#line 320 "compiler/main.gem"
    GemVal _t617 = (*gem_v_peek);
    GemVal _t618 = _t617.fn(_t617.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t618, gem_string("type")), gem_string("|")))) {
#line 321 "compiler/main.gem"
    GemVal _t619 = (*gem_v_advance);
                            (void)(_t619.fn(_t619.env, NULL, 0));
#line 322 "compiler/main.gem"
    GemVal _t620 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t620.fn(_t620.env, NULL, 0);
#line 323 "compiler/main.gem"
    GemVal _t621 = gem_v_bpr;
                            gem_v_bparams = gem_table_get(_t621, gem_string("params"));
#line 324 "compiler/main.gem"
    GemVal _t622 = gem_v_bpr;
                            gem_v_brest = gem_table_get(_t622, gem_string("rest_param"));
#line 325 "compiler/main.gem"
    GemVal _t623 = gem_v_bpr;
                            gem_v_bblock = gem_table_get(_t623, gem_string("block_param"));
#line 326 "compiler/main.gem"
    GemVal _t624[] = {gem_string("|")};
    GemVal _t625 = (*gem_v_expect);
                            (void)(_t625.fn(_t625.env, _t624, 1));
                        }
#line 328 "compiler/main.gem"
    GemVal _t626 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t626.fn(_t626.env, NULL, 0);
#line 329 "compiler/main.gem"
    GemVal _t627[] = {gem_string("}")};
    GemVal _t628 = (*gem_v_expect);
                        (void)(_t628.fn(_t628.env, _t627, 1));
#line 330 "compiler/main.gem"
    GemVal _t629 = gem_table_new();
    gem_table_set(_t629, gem_int(0), gem_v_bexpr);
    GemVal _t630[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, _t629};
    GemVal _t631[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t630, 4)};
                        (void)(gem_push_fn(NULL, _t631, 2));
                    }
                }
            }
#line 335 "compiler/main.gem"
    GemVal _t632[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t632, 3);
            continue;
        }
#line 340 "compiler/main.gem"
    GemVal _t633 = (*gem_v_peek);
    GemVal _t634 = _t633.fn(_t633.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t634, gem_string("type")), gem_string(".")))) {
#line 341 "compiler/main.gem"
    GemVal _t635 = (*gem_v_advance);
            (void)(_t635.fn(_t635.env, NULL, 0));
#line 342 "compiler/main.gem"
    GemVal _t636 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t636.fn(_t636.env, NULL, 0);
#line 343 "compiler/main.gem"
    GemVal _t637 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t637, gem_string("type")), gem_string("NAME")))) {
#line 344 "compiler/main.gem"
    GemVal _t638 = (*gem_v_advance);
                (void)(_t638.fn(_t638.env, NULL, 0));
            } else {
#line 347 "compiler/main.gem"
    GemVal _t639 = (*gem_v_advance);
                (void)(_t639.fn(_t639.env, NULL, 0));
            }
#line 349 "compiler/main.gem"
    GemVal _t640 = gem_v_field_tok;
    GemVal _t641[] = {gem_v_node, gem_table_get(_t640, gem_string("value"))};
            gem_v_node = gem_fn_make_dot(NULL, _t641, 2);
            continue;
        }
#line 354 "compiler/main.gem"
    GemVal _t642 = (*gem_v_peek);
    GemVal _t643 = _t642.fn(_t642.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t643, gem_string("type")), gem_string("[")))) {
#line 355 "compiler/main.gem"
    GemVal _t644 = (*gem_v_advance);
            (void)(_t644.fn(_t644.env, NULL, 0));
#line 356 "compiler/main.gem"
    GemVal _t645 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t645.fn(_t645.env, NULL, 0);
#line 357 "compiler/main.gem"
    GemVal _t646[] = {gem_string("]")};
    GemVal _t647 = (*gem_v_expect);
            (void)(_t647.fn(_t647.env, _t646, 1));
#line 358 "compiler/main.gem"
    GemVal _t648[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t648, 2);
            continue;
        }
        break;
    }
    GemVal _t649 = gem_v_node;
    gem_pop_frame();
    return _t649;
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
    GemVal _t651 = (*gem_v_peek);
    GemVal _t652 = _t651.fn(_t651.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t652, gem_string("type")), gem_string("-")))) {
#line 371 "compiler/main.gem"
    GemVal _t653 = (*gem_v_advance);
        (void)(_t653.fn(_t653.env, NULL, 0));
#line 372 "compiler/main.gem"
    GemVal _t654 = (*gem_v_parse_unary);
    GemVal _t655[] = {gem_string("-"), _t654.fn(_t654.env, NULL, 0)};
        GemVal _t656 = gem_fn_make_unop(NULL, _t655, 2);
        gem_pop_frame();
        return _t656;
    }
#line 374 "compiler/main.gem"
    GemVal _t657 = (*gem_v_parse_call);
    GemVal _t658 = _t657.fn(_t657.env, NULL, 0);
    gem_pop_frame();
    return _t658;
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
    GemVal _t660 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t660.fn(_t660.env, NULL, 0);
#line 380 "compiler/main.gem"
    while (1) {
        GemVal _t661 = (*gem_v_peek);
        GemVal _t662 = _t661.fn(_t661.env, NULL, 0);
        GemVal _t665;
        if (gem_truthy(gem_eq(gem_table_get(_t662, gem_string("type")), gem_string("*")))) {
                _t665 = gem_eq(gem_table_get(_t662, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t663 = (*gem_v_peek);
                GemVal _t664 = _t663.fn(_t663.env, NULL, 0);
                _t665 = gem_eq(gem_table_get(_t664, gem_string("type")), gem_string("/"));
        }
        GemVal _t668;
        if (gem_truthy(_t665)) {
                _t668 = _t665;
        } else {
                GemVal _t666 = (*gem_v_peek);
                GemVal _t667 = _t666.fn(_t666.env, NULL, 0);
                _t668 = gem_eq(gem_table_get(_t667, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t668)) break;
#line 381 "compiler/main.gem"
    GemVal _t669 = (*gem_v_advance);
    GemVal _t670 = _t669.fn(_t669.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t670, gem_string("type"));
#line 382 "compiler/main.gem"
    GemVal _t671 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t671.fn(_t671.env, NULL, 0);
#line 383 "compiler/main.gem"
    GemVal _t672[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t672, 3);
    }
    GemVal _t673 = gem_v_left;
    gem_pop_frame();
    return _t673;
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
    GemVal _t675 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t675.fn(_t675.env, NULL, 0);
#line 391 "compiler/main.gem"
    while (1) {
        GemVal _t676 = (*gem_v_peek);
        GemVal _t677 = _t676.fn(_t676.env, NULL, 0);
        GemVal _t680;
        if (gem_truthy(gem_eq(gem_table_get(_t677, gem_string("type")), gem_string("+")))) {
                _t680 = gem_eq(gem_table_get(_t677, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t678 = (*gem_v_peek);
                GemVal _t679 = _t678.fn(_t678.env, NULL, 0);
                _t680 = gem_eq(gem_table_get(_t679, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t680)) break;
#line 392 "compiler/main.gem"
    GemVal _t681 = (*gem_v_advance);
    GemVal _t682 = _t681.fn(_t681.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t682, gem_string("type"));
#line 393 "compiler/main.gem"
    GemVal _t683 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t683.fn(_t683.env, NULL, 0);
#line 394 "compiler/main.gem"
    GemVal _t684[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t684, 3);
    }
    GemVal _t685 = gem_v_left;
    gem_pop_frame();
    return _t685;
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
    GemVal _t687 = (*gem_v_parse_add);
    GemVal gem_v_left = _t687.fn(_t687.env, NULL, 0);
#line 402 "compiler/main.gem"
    while (1) {
        GemVal _t688 = (*gem_v_peek);
        GemVal _t689 = _t688.fn(_t688.env, NULL, 0);
        GemVal _t692;
        if (gem_truthy(gem_eq(gem_table_get(_t689, gem_string("type")), gem_string("==")))) {
                _t692 = gem_eq(gem_table_get(_t689, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t690 = (*gem_v_peek);
                GemVal _t691 = _t690.fn(_t690.env, NULL, 0);
                _t692 = gem_eq(gem_table_get(_t691, gem_string("type")), gem_string("!="));
        }
        GemVal _t695;
        if (gem_truthy(_t692)) {
                _t695 = _t692;
        } else {
                GemVal _t693 = (*gem_v_peek);
                GemVal _t694 = _t693.fn(_t693.env, NULL, 0);
                _t695 = gem_eq(gem_table_get(_t694, gem_string("type")), gem_string("<"));
        }
        GemVal _t698;
        if (gem_truthy(_t695)) {
                _t698 = _t695;
        } else {
                GemVal _t696 = (*gem_v_peek);
                GemVal _t697 = _t696.fn(_t696.env, NULL, 0);
                _t698 = gem_eq(gem_table_get(_t697, gem_string("type")), gem_string(">"));
        }
        GemVal _t701;
        if (gem_truthy(_t698)) {
                _t701 = _t698;
        } else {
                GemVal _t699 = (*gem_v_peek);
                GemVal _t700 = _t699.fn(_t699.env, NULL, 0);
                _t701 = gem_eq(gem_table_get(_t700, gem_string("type")), gem_string("<="));
        }
        GemVal _t704;
        if (gem_truthy(_t701)) {
                _t704 = _t701;
        } else {
                GemVal _t702 = (*gem_v_peek);
                GemVal _t703 = _t702.fn(_t702.env, NULL, 0);
                _t704 = gem_eq(gem_table_get(_t703, gem_string("type")), gem_string(">="));
        }
        GemVal _t707;
        if (gem_truthy(_t704)) {
                _t707 = _t704;
        } else {
                GemVal _t705 = (*gem_v_peek);
                GemVal _t706 = _t705.fn(_t705.env, NULL, 0);
                _t707 = gem_eq(gem_table_get(_t706, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t707)) break;
#line 403 "compiler/main.gem"
    GemVal _t708 = (*gem_v_advance);
    GemVal _t709 = _t708.fn(_t708.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t709, gem_string("type"));
#line 404 "compiler/main.gem"
    GemVal _t710 = (*gem_v_parse_add);
        GemVal gem_v_right = _t710.fn(_t710.env, NULL, 0);
#line 405 "compiler/main.gem"
    GemVal _t711[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t711, 3);
    }
    GemVal _t712 = gem_v_left;
    gem_pop_frame();
    return _t712;
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
    GemVal _t714 = (*gem_v_peek);
    GemVal _t715 = _t714.fn(_t714.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t715, gem_string("type")), gem_string("not")))) {
#line 413 "compiler/main.gem"
    GemVal _t716 = (*gem_v_advance);
        (void)(_t716.fn(_t716.env, NULL, 0));
#line 414 "compiler/main.gem"
    GemVal _t717 = (*gem_v_parse_not);
    GemVal _t718[] = {gem_string("not"), _t717.fn(_t717.env, NULL, 0)};
        GemVal _t719 = gem_fn_make_unop(NULL, _t718, 2);
        gem_pop_frame();
        return _t719;
    }
#line 416 "compiler/main.gem"
    GemVal _t720 = (*gem_v_parse_compare);
    GemVal _t721 = _t720.fn(_t720.env, NULL, 0);
    gem_pop_frame();
    return _t721;
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
    GemVal _t723 = (*gem_v_parse_not);
    GemVal gem_v_left = _t723.fn(_t723.env, NULL, 0);
#line 422 "compiler/main.gem"
    while (1) {
        GemVal _t724 = (*gem_v_peek);
        GemVal _t725 = _t724.fn(_t724.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t725, gem_string("type")), gem_string("and")))) break;
#line 423 "compiler/main.gem"
    GemVal _t726 = (*gem_v_advance);
        (void)(_t726.fn(_t726.env, NULL, 0));
#line 424 "compiler/main.gem"
    GemVal _t727 = (*gem_v_parse_not);
        GemVal gem_v_right = _t727.fn(_t727.env, NULL, 0);
#line 425 "compiler/main.gem"
    GemVal _t728[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t728, 3);
    }
    GemVal _t729 = gem_v_left;
    gem_pop_frame();
    return _t729;
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
    GemVal _t731 = (*gem_v_parse_and);
    GemVal gem_v_left = _t731.fn(_t731.env, NULL, 0);
#line 433 "compiler/main.gem"
    while (1) {
        GemVal _t732 = (*gem_v_peek);
        GemVal _t733 = _t732.fn(_t732.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t733, gem_string("type")), gem_string("or")))) break;
#line 434 "compiler/main.gem"
    GemVal _t734 = (*gem_v_advance);
        (void)(_t734.fn(_t734.env, NULL, 0));
#line 435 "compiler/main.gem"
    GemVal _t735 = (*gem_v_parse_and);
        GemVal gem_v_right = _t735.fn(_t735.env, NULL, 0);
#line 436 "compiler/main.gem"
    GemVal _t736[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t736, 3);
    }
    GemVal _t737 = gem_v_left;
    gem_pop_frame();
    return _t737;
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
    GemVal _t739 = (*gem_v_peek);
    GemVal _t740 = _t739.fn(_t739.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t740, gem_string("line"));
#line 444 "compiler/main.gem"
    GemVal _t741 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t741.fn(_t741.env, NULL, 0);
#line 447 "compiler/main.gem"
    GemVal _t742 = (*gem_v_peek);
    GemVal _t743 = _t742.fn(_t742.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t743, gem_string("type")), gem_string("=")))) {
#line 448 "compiler/main.gem"
    GemVal _t744 = (*gem_v_advance);
        (void)(_t744.fn(_t744.env, NULL, 0));
#line 449 "compiler/main.gem"
    GemVal _t745 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t745.fn(_t745.env, NULL, 0);
#line 450 "compiler/main.gem"
    GemVal _t746 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t746, gem_string("tag")), gem_string("var")))) {
#line 451 "compiler/main.gem"
    GemVal _t747 = gem_v_lhs;
    GemVal _t748[] = {gem_table_get(_t747, gem_string("name")), gem_v_value, gem_v_start_line};
            GemVal _t749 = gem_fn_make_assign(NULL, _t748, 3);
            gem_pop_frame();
            return _t749;
        }
#line 453 "compiler/main.gem"
    GemVal _t750 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t750, gem_string("tag")), gem_string("dot")))) {
#line 454 "compiler/main.gem"
    GemVal _t751 = gem_v_lhs;
    GemVal _t752 = gem_v_lhs;
    GemVal _t753[] = {gem_table_get(_t751, gem_string("object")), gem_table_get(_t752, gem_string("field")), gem_v_value, gem_v_start_line};
            GemVal _t754 = gem_fn_make_dot_assign(NULL, _t753, 4);
            gem_pop_frame();
            return _t754;
        }
#line 456 "compiler/main.gem"
    GemVal _t755 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t755, gem_string("tag")), gem_string("index")))) {
#line 457 "compiler/main.gem"
    GemVal _t756 = gem_v_lhs;
    GemVal _t757 = gem_v_lhs;
    GemVal _t758[] = {gem_table_get(_t756, gem_string("object")), gem_table_get(_t757, gem_string("key")), gem_v_value, gem_v_start_line};
            GemVal _t759 = gem_fn_make_index_assign(NULL, _t758, 4);
            gem_pop_frame();
            return _t759;
        }
#line 459 "compiler/main.gem"
    GemVal _t760[] = {gem_v_start_line};
    GemVal _t761[] = {gem_add(gem_string("invalid assignment target (expected variable, field, or index) at line "), gem_to_string_fn(NULL, _t760, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 459, _t761, 1));
    }
#line 463 "compiler/main.gem"
    GemVal _t762 = (*gem_v_peek);
    GemVal _t763 = _t762.fn(_t762.env, NULL, 0);
    GemVal _t766;
    if (gem_truthy(gem_eq(gem_table_get(_t763, gem_string("type")), gem_string("+=")))) {
        _t766 = gem_eq(gem_table_get(_t763, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t764 = (*gem_v_peek);
        GemVal _t765 = _t764.fn(_t764.env, NULL, 0);
        _t766 = gem_eq(gem_table_get(_t765, gem_string("type")), gem_string("-="));
    }
    GemVal _t769;
    if (gem_truthy(_t766)) {
        _t769 = _t766;
    } else {
        GemVal _t767 = (*gem_v_peek);
        GemVal _t768 = _t767.fn(_t767.env, NULL, 0);
        _t769 = gem_eq(gem_table_get(_t768, gem_string("type")), gem_string("*="));
    }
    GemVal _t772;
    if (gem_truthy(_t769)) {
        _t772 = _t769;
    } else {
        GemVal _t770 = (*gem_v_peek);
        GemVal _t771 = _t770.fn(_t770.env, NULL, 0);
        _t772 = gem_eq(gem_table_get(_t771, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t772)) {
#line 464 "compiler/main.gem"
    GemVal _t773 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t773.fn(_t773.env, NULL, 0);
#line 465 "compiler/main.gem"
    GemVal _t774 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t774, gem_string("value")), gem_int(0));
#line 466 "compiler/main.gem"
    GemVal _t775 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t775.fn(_t775.env, NULL, 0);
#line 467 "compiler/main.gem"
    GemVal _t776 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t776, gem_string("tag")), gem_string("var")))) {
#line 468 "compiler/main.gem"
    GemVal _t777 = gem_v_op_tok;
    GemVal _t778[] = {gem_table_get(_t777, gem_string("line"))};
    GemVal _t779[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t778, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 468, _t779, 1));
        }
#line 470 "compiler/main.gem"
    GemVal _t780 = gem_v_lhs;
    GemVal _t781 = gem_v_lhs;
    GemVal _t782[] = {gem_table_get(_t781, gem_string("name"))};
    GemVal _t783[] = {gem_v_base_op, gem_fn_make_var(NULL, _t782, 1), gem_v_value};
    GemVal _t784[] = {gem_table_get(_t780, gem_string("name")), gem_fn_make_binop(NULL, _t783, 3), gem_v_start_line};
        GemVal _t785 = gem_fn_make_assign(NULL, _t784, 3);
        gem_pop_frame();
        return _t785;
    }
    GemVal _t786 = gem_v_lhs;
    gem_pop_frame();
    return _t786;
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
    GemVal _t788 = (*gem_v_peek);
    GemVal gem_v_t = _t788.fn(_t788.env, NULL, 0);
#line 485 "compiler/main.gem"
    GemVal _t789 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t789, gem_string("type")), gem_string("let")))) {
#line 486 "compiler/main.gem"
    GemVal _t790 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t790, gem_string("line"));
#line 487 "compiler/main.gem"
    GemVal _t791 = (*gem_v_advance);
        (void)(_t791.fn(_t791.env, NULL, 0));
#line 490 "compiler/main.gem"
    GemVal _t792 = (*gem_v_peek);
    GemVal _t793 = _t792.fn(_t792.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t793, gem_string("type")), gem_string("{")))) {
#line 491 "compiler/main.gem"
    GemVal _t794 = (*gem_v_advance);
            (void)(_t794.fn(_t794.env, NULL, 0));
#line 492 "compiler/main.gem"
    GemVal _t795 = gem_table_new();
            GemVal gem_v_names = _t795;
#line 493 "compiler/main.gem"
    GemVal _t796 = (*gem_v_skip_nl);
            (void)(_t796.fn(_t796.env, NULL, 0));
#line 494 "compiler/main.gem"
            while (1) {
                GemVal _t797 = (*gem_v_peek);
                GemVal _t798 = _t797.fn(_t797.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t798, gem_string("type")), gem_string("}")))) break;
#line 495 "compiler/main.gem"
    GemVal _t799 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t799.fn(_t799.env, NULL, 0);
#line 496 "compiler/main.gem"
    GemVal _t800 = gem_v_field_tok;
                if (gem_truthy(gem_eq(gem_table_get(_t800, gem_string("type")), gem_string("NAME")))) {
#line 497 "compiler/main.gem"
    GemVal _t801 = (*gem_v_advance);
                    (void)(_t801.fn(_t801.env, NULL, 0));
                } else {
#line 500 "compiler/main.gem"
    GemVal _t802 = (*gem_v_advance);
                    (void)(_t802.fn(_t802.env, NULL, 0));
                }
#line 502 "compiler/main.gem"
    GemVal _t803 = gem_v_field_tok;
    GemVal _t804[] = {gem_v_names, gem_table_get(_t803, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t804, 2));
#line 503 "compiler/main.gem"
    GemVal _t805 = (*gem_v_skip_nl);
                (void)(_t805.fn(_t805.env, NULL, 0));
#line 504 "compiler/main.gem"
    GemVal _t806 = (*gem_v_peek);
    GemVal _t807 = _t806.fn(_t806.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t807, gem_string("type")), gem_string(",")))) {
#line 505 "compiler/main.gem"
    GemVal _t808 = (*gem_v_advance);
                    (void)(_t808.fn(_t808.env, NULL, 0));
#line 506 "compiler/main.gem"
    GemVal _t809 = (*gem_v_skip_nl);
                    (void)(_t809.fn(_t809.env, NULL, 0));
                }
            }
#line 509 "compiler/main.gem"
    GemVal _t810[] = {gem_string("}")};
    GemVal _t811 = (*gem_v_expect);
            (void)(_t811.fn(_t811.env, _t810, 1));
#line 510 "compiler/main.gem"
    GemVal _t812[] = {gem_string("=")};
    GemVal _t813 = (*gem_v_expect);
            (void)(_t813.fn(_t813.env, _t812, 1));
#line 511 "compiler/main.gem"
    GemVal _t814 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t814.fn(_t814.env, NULL, 0);
#line 512 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 513 "compiler/main.gem"
    GemVal _t815[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t815, 1));
#line 514 "compiler/main.gem"
    GemVal _t816 = gem_table_new();
    GemVal _t817[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t816, gem_int(0), gem_fn_make_let(NULL, _t817, 3));
            GemVal gem_v_stmts = _t816;
#line 515 "compiler/main.gem"
            GemVal gem_v__for_i_1 = gem_int(0);
#line 515 "compiler/main.gem"
    GemVal _t818[] = {gem_v_names};
            GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t818, 1);
#line 515 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 515 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_1;
#line 515 "compiler/main.gem"
                gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 516 "compiler/main.gem"
    GemVal _t819[] = {gem_v_tmp};
    GemVal _t820[] = {gem_fn_make_var(NULL, _t819, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t821[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_dot(NULL, _t820, 2), gem_v_line};
    GemVal _t822[] = {gem_v_stmts, gem_fn_make_let(NULL, _t821, 3)};
                (void)(gem_push_fn(NULL, _t822, 2));
            }

#line 518 "compiler/main.gem"
    GemVal _t823 = gem_table_new();
    gem_table_set(_t823, gem_string("tag"), gem_string("block"));
    gem_table_set(_t823, gem_string("stmts"), gem_v_stmts);
            GemVal _t824 = _t823;
            gem_pop_frame();
            return _t824;
        }
#line 522 "compiler/main.gem"
    GemVal _t825 = (*gem_v_peek);
    GemVal _t826 = _t825.fn(_t825.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t826, gem_string("type")), gem_string("[")))) {
#line 523 "compiler/main.gem"
    GemVal _t827 = (*gem_v_advance);
            (void)(_t827.fn(_t827.env, NULL, 0));
#line 524 "compiler/main.gem"
    GemVal _t828 = gem_table_new();
            GemVal gem_v_names = _t828;
#line 525 "compiler/main.gem"
    GemVal _t829 = (*gem_v_skip_nl);
            (void)(_t829.fn(_t829.env, NULL, 0));
#line 526 "compiler/main.gem"
            while (1) {
                GemVal _t830 = (*gem_v_peek);
                GemVal _t831 = _t830.fn(_t830.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t831, gem_string("type")), gem_string("]")))) break;
#line 527 "compiler/main.gem"
    GemVal _t832[] = {gem_string("NAME")};
    GemVal _t833 = (*gem_v_expect);
    GemVal _t834 = _t833.fn(_t833.env, _t832, 1);
    GemVal _t835[] = {gem_v_names, gem_table_get(_t834, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t835, 2));
#line 528 "compiler/main.gem"
    GemVal _t836 = (*gem_v_skip_nl);
                (void)(_t836.fn(_t836.env, NULL, 0));
#line 529 "compiler/main.gem"
    GemVal _t837 = (*gem_v_peek);
    GemVal _t838 = _t837.fn(_t837.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t838, gem_string("type")), gem_string(",")))) {
#line 530 "compiler/main.gem"
    GemVal _t839 = (*gem_v_advance);
                    (void)(_t839.fn(_t839.env, NULL, 0));
#line 531 "compiler/main.gem"
    GemVal _t840 = (*gem_v_skip_nl);
                    (void)(_t840.fn(_t840.env, NULL, 0));
                }
            }
#line 534 "compiler/main.gem"
    GemVal _t841[] = {gem_string("]")};
    GemVal _t842 = (*gem_v_expect);
            (void)(_t842.fn(_t842.env, _t841, 1));
#line 535 "compiler/main.gem"
    GemVal _t843[] = {gem_string("=")};
    GemVal _t844 = (*gem_v_expect);
            (void)(_t844.fn(_t844.env, _t843, 1));
#line 536 "compiler/main.gem"
    GemVal _t845 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t845.fn(_t845.env, NULL, 0);
#line 537 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 538 "compiler/main.gem"
    GemVal _t846[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t846, 1));
#line 539 "compiler/main.gem"
    GemVal _t847 = gem_table_new();
    GemVal _t848[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t847, gem_int(0), gem_fn_make_let(NULL, _t848, 3));
            GemVal gem_v_stmts = _t847;
#line 540 "compiler/main.gem"
            GemVal gem_v__for_i_2 = gem_int(0);
#line 540 "compiler/main.gem"
    GemVal _t849[] = {gem_v_names};
            GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t849, 1);
#line 540 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 540 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_2;
#line 540 "compiler/main.gem"
                gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 541 "compiler/main.gem"
    GemVal _t850[] = {gem_v_tmp};
    GemVal _t851[] = {gem_v_di};
    GemVal _t852[] = {gem_fn_make_var(NULL, _t850, 1), gem_fn_make_int(NULL, _t851, 1)};
    GemVal _t853[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_index(NULL, _t852, 2), gem_v_line};
    GemVal _t854[] = {gem_v_stmts, gem_fn_make_let(NULL, _t853, 3)};
                (void)(gem_push_fn(NULL, _t854, 2));
            }

#line 543 "compiler/main.gem"
    GemVal _t855 = gem_table_new();
    gem_table_set(_t855, gem_string("tag"), gem_string("block"));
    gem_table_set(_t855, gem_string("stmts"), gem_v_stmts);
            GemVal _t856 = _t855;
            gem_pop_frame();
            return _t856;
        }
#line 546 "compiler/main.gem"
    GemVal _t857[] = {gem_string("NAME")};
    GemVal _t858 = (*gem_v_expect);
    GemVal _t859 = _t858.fn(_t858.env, _t857, 1);
        GemVal gem_v_name = gem_table_get(_t859, gem_string("value"));
#line 547 "compiler/main.gem"
    GemVal _t860[] = {gem_string("=")};
    GemVal _t861 = (*gem_v_expect);
        (void)(_t861.fn(_t861.env, _t860, 1));
#line 548 "compiler/main.gem"
    GemVal _t862 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t862.fn(_t862.env, NULL, 0);
#line 549 "compiler/main.gem"
    GemVal _t863[] = {gem_v_name, gem_v_value, gem_v_line};
        GemVal _t864 = gem_fn_make_let(NULL, _t863, 3);
        gem_pop_frame();
        return _t864;
    }
#line 553 "compiler/main.gem"
    GemVal _t865 = gem_v_t;
    GemVal _t869;
    if (!gem_truthy(gem_eq(gem_table_get(_t865, gem_string("type")), gem_string("fn")))) {
        _t869 = gem_eq(gem_table_get(_t865, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t866[] = {gem_int(1)};
        GemVal _t867 = (*gem_v_peek_at);
        GemVal _t868 = _t867.fn(_t867.env, _t866, 1);
        _t869 = gem_eq(gem_table_get(_t868, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t869)) {
#line 554 "compiler/main.gem"
    GemVal _t870 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t870, gem_string("line"));
#line 555 "compiler/main.gem"
    GemVal _t871 = (*gem_v_advance);
        (void)(_t871.fn(_t871.env, NULL, 0));
#line 556 "compiler/main.gem"
    GemVal _t872[] = {gem_string("NAME")};
    GemVal _t873 = (*gem_v_expect);
    GemVal _t874 = _t873.fn(_t873.env, _t872, 1);
        GemVal gem_v_name = gem_table_get(_t874, gem_string("value"));
#line 557 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 558 "compiler/main.gem"
    GemVal _t875[] = {gem_v_name};
    GemVal _t876[] = {gem_v_line};
    GemVal _t877[] = {gem_v_name};
    GemVal _t878[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("named fn '"), gem_to_string_fn(NULL, _t875, 1)), gem_string("' inside function body is not supported at line ")), gem_to_string_fn(NULL, _t876, 1)), gem_string(" — use: let ")), gem_to_string_fn(NULL, _t877, 1)), gem_string(" = fn(...) ... end"))};
            (void)(gem_error_at_fn("compiler/main.gem", 558, _t878, 1));
        }
#line 560 "compiler/main.gem"
    GemVal _t879[] = {gem_string("(")};
    GemVal _t880 = (*gem_v_expect);
        (void)(_t880.fn(_t880.env, _t879, 1));
#line 561 "compiler/main.gem"
    GemVal _t881 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t881.fn(_t881.env, NULL, 0);
#line 562 "compiler/main.gem"
    GemVal _t882[] = {gem_string(")")};
    GemVal _t883 = (*gem_v_expect);
        (void)(_t883.fn(_t883.env, _t882, 1));
#line 563 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 564 "compiler/main.gem"
    GemVal _t884 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t884.fn(_t884.env, NULL, 0);
#line 565 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 566 "compiler/main.gem"
    GemVal _t885[] = {gem_string("end")};
    GemVal _t886 = (*gem_v_expect);
        (void)(_t886.fn(_t886.env, _t885, 1));
#line 567 "compiler/main.gem"
    GemVal _t887 = gem_v_pr;
    GemVal _t888 = gem_v_pr;
    GemVal _t889 = gem_v_pr;
    GemVal _t890[] = {gem_v_name, gem_table_get(_t887, gem_string("params")), gem_table_get(_t888, gem_string("rest_param")), gem_table_get(_t889, gem_string("block_param")), gem_v_body, gem_v_line};
        GemVal _t891 = gem_fn_make_fn_def(NULL, _t890, 6);
        gem_pop_frame();
        return _t891;
    }
#line 571 "compiler/main.gem"
    GemVal _t892 = gem_v_t;
    GemVal _t894;
    if (gem_truthy(gem_eq(gem_table_get(_t892, gem_string("type")), gem_string("if")))) {
        _t894 = gem_eq(gem_table_get(_t892, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t893 = gem_v_t;
        _t894 = gem_eq(gem_table_get(_t893, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t894)) {
#line 572 "compiler/main.gem"
    GemVal _t895 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t895, gem_string("line"));
#line 573 "compiler/main.gem"
    GemVal _t896 = (*gem_v_advance);
        (void)(_t896.fn(_t896.env, NULL, 0));
#line 574 "compiler/main.gem"
    GemVal _t897 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t897.fn(_t897.env, NULL, 0);
#line 575 "compiler/main.gem"
    GemVal _t898 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t898.fn(_t898.env, NULL, 0);
#line 576 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 577 "compiler/main.gem"
    GemVal _t899 = (*gem_v_peek);
    GemVal _t900 = _t899.fn(_t899.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t900, gem_string("type")), gem_string("elif")))) {
#line 579 "compiler/main.gem"
    GemVal _t901 = gem_table_new();
    GemVal _t902 = (*gem_v_parse_stmt);
    gem_table_set(_t901, gem_int(0), _t902.fn(_t902.env, NULL, 0));
            gem_v_else_body = _t901;
        } else {
#line 580 "compiler/main.gem"
    GemVal _t903 = (*gem_v_peek);
    GemVal _t904 = _t903.fn(_t903.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t904, gem_string("type")), gem_string("else")))) {
#line 581 "compiler/main.gem"
    GemVal _t905 = (*gem_v_advance);
                (void)(_t905.fn(_t905.env, NULL, 0));
#line 582 "compiler/main.gem"
    GemVal _t906 = (*gem_v_parse_body);
                gem_v_else_body = _t906.fn(_t906.env, NULL, 0);
            }
        }
#line 585 "compiler/main.gem"
    GemVal _t907 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t907, gem_string("type")), gem_string("if")))) {
#line 586 "compiler/main.gem"
    GemVal _t908[] = {gem_string("end")};
    GemVal _t909 = (*gem_v_expect);
            (void)(_t909.fn(_t909.env, _t908, 1));
        }
#line 588 "compiler/main.gem"
    GemVal _t910[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        GemVal _t911 = gem_fn_make_if(NULL, _t910, 4);
        gem_pop_frame();
        return _t911;
    }
#line 592 "compiler/main.gem"
    GemVal _t912 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t912, gem_string("type")), gem_string("while")))) {
#line 593 "compiler/main.gem"
    GemVal _t913 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t913, gem_string("line"));
#line 594 "compiler/main.gem"
    GemVal _t914 = (*gem_v_advance);
        (void)(_t914.fn(_t914.env, NULL, 0));
#line 595 "compiler/main.gem"
    GemVal _t915 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t915.fn(_t915.env, NULL, 0);
#line 596 "compiler/main.gem"
    GemVal _t916 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t916.fn(_t916.env, NULL, 0);
#line 597 "compiler/main.gem"
    GemVal _t917[] = {gem_string("end")};
    GemVal _t918 = (*gem_v_expect);
        (void)(_t918.fn(_t918.env, _t917, 1));
#line 598 "compiler/main.gem"
    GemVal _t919[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        GemVal _t920 = gem_fn_make_while(NULL, _t919, 3);
        gem_pop_frame();
        return _t920;
    }
#line 602 "compiler/main.gem"
    GemVal _t921 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t921, gem_string("type")), gem_string("for")))) {
#line 603 "compiler/main.gem"
    GemVal _t922 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t922, gem_string("line"));
#line 604 "compiler/main.gem"
    GemVal _t923 = (*gem_v_advance);
        (void)(_t923.fn(_t923.env, NULL, 0));
#line 605 "compiler/main.gem"
    GemVal _t924[] = {gem_string("NAME")};
    GemVal _t925 = (*gem_v_expect);
    GemVal _t926 = _t925.fn(_t925.env, _t924, 1);
        GemVal gem_v_var_name = gem_table_get(_t926, gem_string("value"));
#line 607 "compiler/main.gem"
    GemVal _t927 = (*gem_v_peek);
    GemVal _t928 = _t927.fn(_t927.env, NULL, 0);
    GemVal _t932;
    if (!gem_truthy(gem_eq(gem_table_get(_t928, gem_string("type")), gem_string(",")))) {
        _t932 = gem_eq(gem_table_get(_t928, gem_string("type")), gem_string(","));
    } else {
        GemVal _t929[] = {gem_int(1)};
        GemVal _t930 = (*gem_v_peek_at);
        GemVal _t931 = _t930.fn(_t930.env, _t929, 1);
        _t932 = gem_eq(gem_table_get(_t931, gem_string("type")), gem_string("NAME"));
    }
        if (gem_truthy(_t932)) {
#line 609 "compiler/main.gem"
    GemVal _t933 = (*gem_v_advance);
            (void)(_t933.fn(_t933.env, NULL, 0));
#line 610 "compiler/main.gem"
    GemVal _t934[] = {gem_string("NAME")};
    GemVal _t935 = (*gem_v_expect);
    GemVal _t936 = _t935.fn(_t935.env, _t934, 1);
            GemVal gem_v_val_name = gem_table_get(_t936, gem_string("value"));
#line 611 "compiler/main.gem"
    GemVal _t937[] = {gem_string("in")};
    GemVal _t938 = (*gem_v_expect);
            (void)(_t938.fn(_t938.env, _t937, 1));
#line 612 "compiler/main.gem"
    GemVal _t939 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t939.fn(_t939.env, NULL, 0);
#line 613 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 614 "compiler/main.gem"
    GemVal _t940[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_add(gem_string("_for_tbl_"), gem_to_string_fn(NULL, _t940, 1));
#line 615 "compiler/main.gem"
    GemVal _t941[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_keys_var = gem_add(gem_string("_for_keys_"), gem_to_string_fn(NULL, _t941, 1));
#line 616 "compiler/main.gem"
    GemVal _t942[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t942, 1));
#line 617 "compiler/main.gem"
    GemVal _t943 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t943.fn(_t943.env, NULL, 0);
#line 618 "compiler/main.gem"
    GemVal _t944[] = {gem_string("end")};
    GemVal _t945 = (*gem_v_expect);
            (void)(_t945.fn(_t945.env, _t944, 1));
#line 619 "compiler/main.gem"
    GemVal _t946 = gem_table_new();
            GemVal gem_v_inner_stmts = _t946;
#line 620 "compiler/main.gem"
    GemVal _t947[] = {gem_v_keys_var};
    GemVal _t948[] = {gem_v_idx_var};
    GemVal _t949[] = {gem_fn_make_var(NULL, _t947, 1), gem_fn_make_var(NULL, _t948, 1)};
    GemVal _t950[] = {gem_v_var_name, gem_fn_make_index(NULL, _t949, 2), gem_v_line};
    GemVal _t951[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t950, 3)};
            (void)(gem_push_fn(NULL, _t951, 2));
#line 621 "compiler/main.gem"
    GemVal _t952[] = {gem_v_tbl_var};
    GemVal _t953[] = {gem_v_keys_var};
    GemVal _t954[] = {gem_v_idx_var};
    GemVal _t955[] = {gem_fn_make_var(NULL, _t953, 1), gem_fn_make_var(NULL, _t954, 1)};
    GemVal _t956[] = {gem_fn_make_var(NULL, _t952, 1), gem_fn_make_index(NULL, _t955, 2)};
    GemVal _t957[] = {gem_v_val_name, gem_fn_make_index(NULL, _t956, 2), gem_v_line};
    GemVal _t958[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t957, 3)};
            (void)(gem_push_fn(NULL, _t958, 2));
#line 622 "compiler/main.gem"
    GemVal _t959[] = {gem_v_idx_var};
    GemVal _t960[] = {gem_int(1)};
    GemVal _t961[] = {gem_string("+"), gem_fn_make_var(NULL, _t959, 1), gem_fn_make_int(NULL, _t960, 1)};
    GemVal _t962[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t961, 3), gem_v_line};
    GemVal _t963[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t962, 3)};
            (void)(gem_push_fn(NULL, _t963, 2));
#line 623 "compiler/main.gem"
            GemVal gem_v__for_i_3 = gem_int(0);
#line 623 "compiler/main.gem"
    GemVal _t964[] = {gem_v_fbody};
            GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t964, 1);
#line 623 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 623 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_3;
#line 623 "compiler/main.gem"
                gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 624 "compiler/main.gem"
    GemVal _t965[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t965, 2));
            }

#line 626 "compiler/main.gem"
    GemVal _t966[] = {gem_v_idx_var};
    GemVal _t967[] = {gem_string("len")};
    GemVal _t968 = gem_table_new();
    GemVal _t969[] = {gem_v_keys_var};
    gem_table_set(_t968, gem_int(0), gem_fn_make_var(NULL, _t969, 1));
    GemVal _t970[] = {gem_fn_make_var(NULL, _t967, 1), _t968, gem_int(0)};
    GemVal _t971[] = {gem_string("<"), gem_fn_make_var(NULL, _t966, 1), gem_fn_make_call(NULL, _t970, 3)};
    GemVal _t972[] = {gem_fn_make_binop(NULL, _t971, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t972, 3);
#line 631 "compiler/main.gem"
    GemVal _t973 = gem_table_new();
    gem_table_set(_t973, gem_string("tag"), gem_string("block"));
    GemVal _t974 = gem_table_new();
    GemVal _t975[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    gem_table_set(_t974, gem_int(0), gem_fn_make_let(NULL, _t975, 3));
    GemVal _t976[] = {gem_string("keys")};
    GemVal _t977 = gem_table_new();
    GemVal _t978[] = {gem_v_tbl_var};
    gem_table_set(_t977, gem_int(0), gem_fn_make_var(NULL, _t978, 1));
    GemVal _t979[] = {gem_fn_make_var(NULL, _t976, 1), _t977, gem_int(0)};
    GemVal _t980[] = {gem_v_keys_var, gem_fn_make_call(NULL, _t979, 3), gem_v_line};
    gem_table_set(_t974, gem_int(1), gem_fn_make_let(NULL, _t980, 3));
    GemVal _t981[] = {gem_int(0)};
    GemVal _t982[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t981, 1), gem_v_line};
    gem_table_set(_t974, gem_int(2), gem_fn_make_let(NULL, _t982, 3));
    gem_table_set(_t974, gem_int(3), gem_v_while_node);
    gem_table_set(_t973, gem_string("stmts"), _t974);
            GemVal _t983 = _t973;
            gem_pop_frame();
            return _t983;
        } else {
#line 637 "compiler/main.gem"
    GemVal _t984 = (*gem_v_peek);
    GemVal _t985 = _t984.fn(_t984.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t985, gem_string("type")), gem_string("in")))) {
#line 639 "compiler/main.gem"
    GemVal _t986 = (*gem_v_advance);
                (void)(_t986.fn(_t986.env, NULL, 0));
#line 640 "compiler/main.gem"
    GemVal _t987 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t987.fn(_t987.env, NULL, 0);
#line 641 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 642 "compiler/main.gem"
    GemVal _t988[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t988, 1));
#line 643 "compiler/main.gem"
    GemVal _t989[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t989, 1));
#line 644 "compiler/main.gem"
    GemVal _t990 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t990.fn(_t990.env, NULL, 0);
#line 645 "compiler/main.gem"
    GemVal _t991[] = {gem_string("end")};
    GemVal _t992 = (*gem_v_expect);
                (void)(_t992.fn(_t992.env, _t991, 1));
#line 646 "compiler/main.gem"
    GemVal _t993 = gem_table_new();
                GemVal gem_v_inner_stmts = _t993;
#line 647 "compiler/main.gem"
    GemVal _t994[] = {gem_v_items_var};
    GemVal _t995[] = {gem_v_idx_var};
    GemVal _t996[] = {gem_fn_make_var(NULL, _t994, 1), gem_fn_make_var(NULL, _t995, 1)};
    GemVal _t997[] = {gem_v_var_name, gem_fn_make_index(NULL, _t996, 2), gem_v_line};
    GemVal _t998[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t997, 3)};
                (void)(gem_push_fn(NULL, _t998, 2));
#line 648 "compiler/main.gem"
    GemVal _t999[] = {gem_v_idx_var};
    GemVal _t1000[] = {gem_int(1)};
    GemVal _t1001[] = {gem_string("+"), gem_fn_make_var(NULL, _t999, 1), gem_fn_make_int(NULL, _t1000, 1)};
    GemVal _t1002[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1001, 3), gem_v_line};
    GemVal _t1003[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1002, 3)};
                (void)(gem_push_fn(NULL, _t1003, 2));
#line 649 "compiler/main.gem"
                GemVal gem_v__for_i_4 = gem_int(0);
#line 649 "compiler/main.gem"
    GemVal _t1004[] = {gem_v_fbody};
                GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t1004, 1);
#line 649 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 649 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_4;
#line 649 "compiler/main.gem"
                    gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 650 "compiler/main.gem"
    GemVal _t1005[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t1005, 2));
                }

#line 652 "compiler/main.gem"
    GemVal _t1006[] = {gem_v_idx_var};
    GemVal _t1007[] = {gem_string("len")};
    GemVal _t1008 = gem_table_new();
    GemVal _t1009[] = {gem_v_items_var};
    gem_table_set(_t1008, gem_int(0), gem_fn_make_var(NULL, _t1009, 1));
    GemVal _t1010[] = {gem_fn_make_var(NULL, _t1007, 1), _t1008, gem_int(0)};
    GemVal _t1011[] = {gem_string("<"), gem_fn_make_var(NULL, _t1006, 1), gem_fn_make_call(NULL, _t1010, 3)};
    GemVal _t1012[] = {gem_fn_make_binop(NULL, _t1011, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1012, 3);
#line 657 "compiler/main.gem"
    GemVal _t1013 = gem_table_new();
    gem_table_set(_t1013, gem_string("tag"), gem_string("block"));
    GemVal _t1014 = gem_table_new();
    GemVal _t1015[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t1014, gem_int(0), gem_fn_make_let(NULL, _t1015, 3));
    GemVal _t1016[] = {gem_int(0)};
    GemVal _t1017[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t1016, 1), gem_v_line};
    gem_table_set(_t1014, gem_int(1), gem_fn_make_let(NULL, _t1017, 3));
    gem_table_set(_t1014, gem_int(2), gem_v_while_node);
    gem_table_set(_t1013, gem_string("stmts"), _t1014);
                GemVal _t1018 = _t1013;
                gem_pop_frame();
                return _t1018;
            } else {
#line 662 "compiler/main.gem"
    GemVal _t1019 = (*gem_v_peek);
    GemVal _t1020 = _t1019.fn(_t1019.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1020, gem_string("type")), gem_string("=")))) {
#line 664 "compiler/main.gem"
    GemVal _t1021 = (*gem_v_advance);
                    (void)(_t1021.fn(_t1021.env, NULL, 0));
#line 665 "compiler/main.gem"
    GemVal _t1022 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t1022.fn(_t1022.env, NULL, 0);
#line 666 "compiler/main.gem"
    GemVal _t1023[] = {gem_string(",")};
    GemVal _t1024 = (*gem_v_expect);
                    (void)(_t1024.fn(_t1024.env, _t1023, 1));
#line 667 "compiler/main.gem"
    GemVal _t1025 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1025.fn(_t1025.env, NULL, 0);
#line 668 "compiler/main.gem"
    GemVal _t1026 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1026.fn(_t1026.env, NULL, 0);
#line 669 "compiler/main.gem"
    GemVal _t1027[] = {gem_string("end")};
    GemVal _t1028 = (*gem_v_expect);
                    (void)(_t1028.fn(_t1028.env, _t1027, 1));
#line 670 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 671 "compiler/main.gem"
    GemVal _t1029[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1029, 1));
#line 672 "compiler/main.gem"
    GemVal _t1030[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t1030, 1));
#line 673 "compiler/main.gem"
    GemVal _t1031 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1031;
#line 674 "compiler/main.gem"
    GemVal _t1032[] = {gem_v_idx_var};
    GemVal _t1033[] = {gem_v_var_name, gem_fn_make_var(NULL, _t1032, 1), gem_v_line};
    GemVal _t1034[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1033, 3)};
                    (void)(gem_push_fn(NULL, _t1034, 2));
#line 675 "compiler/main.gem"
    GemVal _t1035[] = {gem_v_idx_var};
    GemVal _t1036[] = {gem_int(1)};
    GemVal _t1037[] = {gem_string("+"), gem_fn_make_var(NULL, _t1035, 1), gem_fn_make_int(NULL, _t1036, 1)};
    GemVal _t1038[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1037, 3), gem_v_line};
    GemVal _t1039[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1038, 3)};
                    (void)(gem_push_fn(NULL, _t1039, 2));
#line 676 "compiler/main.gem"
                    GemVal gem_v__for_i_5 = gem_int(0);
#line 676 "compiler/main.gem"
    GemVal _t1040[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_5 = gem_len_fn(NULL, _t1040, 1);
#line 676 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_limit_5))) break;
#line 676 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_5;
#line 676 "compiler/main.gem"
                        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 677 "compiler/main.gem"
    GemVal _t1041[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1041, 2));
                    }

#line 679 "compiler/main.gem"
    GemVal _t1042[] = {gem_v_idx_var};
    GemVal _t1043[] = {gem_v_limit_var};
    GemVal _t1044[] = {gem_string("<"), gem_fn_make_var(NULL, _t1042, 1), gem_fn_make_var(NULL, _t1043, 1)};
    GemVal _t1045[] = {gem_fn_make_binop(NULL, _t1044, 3), gem_v_inner_stmts, gem_v_line};
                    GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1045, 3);
#line 684 "compiler/main.gem"
    GemVal _t1046 = gem_table_new();
    gem_table_set(_t1046, gem_string("tag"), gem_string("block"));
    GemVal _t1047 = gem_table_new();
    GemVal _t1048[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t1047, gem_int(0), gem_fn_make_let(NULL, _t1048, 3));
    GemVal _t1049[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t1047, gem_int(1), gem_fn_make_let(NULL, _t1049, 3));
    gem_table_set(_t1047, gem_int(2), gem_v_while_node);
    gem_table_set(_t1046, gem_string("stmts"), _t1047);
                    GemVal _t1050 = _t1046;
                    gem_pop_frame();
                    return _t1050;
                } else {
#line 690 "compiler/main.gem"
    GemVal _t1051 = (*gem_v_peek);
    GemVal _t1052 = _t1051.fn(_t1051.env, NULL, 0);
    GemVal _t1053[] = {gem_table_get(_t1052, gem_string("line"))};
    GemVal _t1054[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t1053, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 690, _t1054, 1));
                }
            }
        }
    }
#line 695 "compiler/main.gem"
    GemVal _t1055 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1055, gem_string("type")), gem_string("match")))) {
#line 696 "compiler/main.gem"
    GemVal _t1056 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1056, gem_string("line"));
#line 697 "compiler/main.gem"
    GemVal _t1057 = (*gem_v_advance);
        (void)(_t1057.fn(_t1057.env, NULL, 0));
#line 698 "compiler/main.gem"
    GemVal _t1058 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1058.fn(_t1058.env, NULL, 0);
#line 699 "compiler/main.gem"
    GemVal _t1059 = (*gem_v_skip_nl);
        (void)(_t1059.fn(_t1059.env, NULL, 0));
#line 700 "compiler/main.gem"
    GemVal _t1060 = gem_table_new();
        GemVal gem_v_whens = _t1060;
#line 701 "compiler/main.gem"
        while (1) {
            GemVal _t1061 = (*gem_v_peek);
            GemVal _t1062 = _t1061.fn(_t1061.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1062, gem_string("type")), gem_string("when")))) break;
#line 702 "compiler/main.gem"
    GemVal _t1063 = (*gem_v_advance);
            (void)(_t1063.fn(_t1063.env, NULL, 0));
#line 703 "compiler/main.gem"
    GemVal _t1064 = (*gem_v_parse_expr);
            GemVal gem_v_wval = _t1064.fn(_t1064.env, NULL, 0);
#line 704 "compiler/main.gem"
    GemVal _t1065 = (*gem_v_parse_body);
            GemVal gem_v_wbody = _t1065.fn(_t1065.env, NULL, 0);
#line 705 "compiler/main.gem"
    GemVal _t1066[] = {gem_v_wval, gem_v_wbody};
    GemVal _t1067[] = {gem_v_whens, gem_fn_make_when(NULL, _t1066, 2)};
            (void)(gem_push_fn(NULL, _t1067, 2));
#line 706 "compiler/main.gem"
    GemVal _t1068 = (*gem_v_skip_nl);
            (void)(_t1068.fn(_t1068.env, NULL, 0));
        }
#line 708 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 709 "compiler/main.gem"
    GemVal _t1069 = (*gem_v_peek);
    GemVal _t1070 = _t1069.fn(_t1069.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1070, gem_string("type")), gem_string("else")))) {
#line 710 "compiler/main.gem"
    GemVal _t1071 = (*gem_v_advance);
            (void)(_t1071.fn(_t1071.env, NULL, 0));
#line 711 "compiler/main.gem"
    GemVal _t1072 = (*gem_v_parse_body);
            gem_v_else_body = _t1072.fn(_t1072.env, NULL, 0);
        }
#line 713 "compiler/main.gem"
    GemVal _t1073[] = {gem_string("end")};
    GemVal _t1074 = (*gem_v_expect);
        (void)(_t1074.fn(_t1074.env, _t1073, 1));
#line 714 "compiler/main.gem"
    GemVal _t1075[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line};
        GemVal _t1076 = gem_fn_make_match(NULL, _t1075, 4);
        gem_pop_frame();
        return _t1076;
    }
#line 718 "compiler/main.gem"
    GemVal _t1077 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1077, gem_string("type")), gem_string("return")))) {
#line 719 "compiler/main.gem"
    GemVal _t1078 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1078, gem_string("line"));
#line 720 "compiler/main.gem"
    GemVal _t1079 = (*gem_v_advance);
        (void)(_t1079.fn(_t1079.env, NULL, 0));
#line 721 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 722 "compiler/main.gem"
    GemVal _t1080 = (*gem_v_peek);
    GemVal _t1081 = _t1080.fn(_t1080.env, NULL, 0);
    GemVal _t1084;
    if (!gem_truthy(gem_neq(gem_table_get(_t1081, gem_string("type")), gem_string("NEWLINE")))) {
        _t1084 = gem_neq(gem_table_get(_t1081, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t1082 = (*gem_v_peek);
        GemVal _t1083 = _t1082.fn(_t1082.env, NULL, 0);
        _t1084 = gem_neq(gem_table_get(_t1083, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t1087;
    if (!gem_truthy(_t1084)) {
        _t1087 = _t1084;
    } else {
        GemVal _t1085 = (*gem_v_peek);
        GemVal _t1086 = _t1085.fn(_t1085.env, NULL, 0);
        _t1087 = gem_neq(gem_table_get(_t1086, gem_string("type")), gem_string("end"));
    }
    GemVal _t1090;
    if (!gem_truthy(_t1087)) {
        _t1090 = _t1087;
    } else {
        GemVal _t1088 = (*gem_v_peek);
        GemVal _t1089 = _t1088.fn(_t1088.env, NULL, 0);
        _t1090 = gem_neq(gem_table_get(_t1089, gem_string("type")), gem_string("elif"));
    }
    GemVal _t1093;
    if (!gem_truthy(_t1090)) {
        _t1093 = _t1090;
    } else {
        GemVal _t1091 = (*gem_v_peek);
        GemVal _t1092 = _t1091.fn(_t1091.env, NULL, 0);
        _t1093 = gem_neq(gem_table_get(_t1092, gem_string("type")), gem_string("else"));
    }
    GemVal _t1096;
    if (!gem_truthy(_t1093)) {
        _t1096 = _t1093;
    } else {
        GemVal _t1094 = (*gem_v_peek);
        GemVal _t1095 = _t1094.fn(_t1094.env, NULL, 0);
        _t1096 = gem_neq(gem_table_get(_t1095, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t1096)) {
#line 723 "compiler/main.gem"
    GemVal _t1097 = (*gem_v_parse_expr);
            gem_v_value = _t1097.fn(_t1097.env, NULL, 0);
        }
#line 725 "compiler/main.gem"
    GemVal _t1098[] = {gem_v_value, gem_v_line};
        GemVal _t1099 = gem_fn_make_return(NULL, _t1098, 2);
        gem_pop_frame();
        return _t1099;
    }
#line 729 "compiler/main.gem"
    GemVal _t1100 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1100, gem_string("type")), gem_string("break")))) {
#line 730 "compiler/main.gem"
    GemVal _t1101 = (*gem_v_advance);
        (void)(_t1101.fn(_t1101.env, NULL, 0));
#line 731 "compiler/main.gem"
        GemVal _t1102 = gem_fn_make_break(NULL, NULL, 0);
        gem_pop_frame();
        return _t1102;
    }
#line 735 "compiler/main.gem"
    GemVal _t1103 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1103, gem_string("type")), gem_string("continue")))) {
#line 736 "compiler/main.gem"
    GemVal _t1104 = (*gem_v_advance);
        (void)(_t1104.fn(_t1104.env, NULL, 0));
#line 737 "compiler/main.gem"
        GemVal _t1105 = gem_fn_make_continue(NULL, NULL, 0);
        gem_pop_frame();
        return _t1105;
    }
#line 741 "compiler/main.gem"
    GemVal _t1106 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1106, gem_string("type")), gem_string("load")))) {
#line 742 "compiler/main.gem"
    GemVal _t1107 = (*gem_v_advance);
        (void)(_t1107.fn(_t1107.env, NULL, 0));
#line 743 "compiler/main.gem"
    GemVal _t1108[] = {gem_string("STRING")};
    GemVal _t1109 = (*gem_v_expect);
    GemVal _t1110 = _t1109.fn(_t1109.env, _t1108, 1);
        GemVal gem_v_path = gem_table_get(_t1110, gem_string("value"));
#line 744 "compiler/main.gem"
    GemVal _t1111[] = {gem_v_path};
        GemVal _t1112 = gem_fn_make_load(NULL, _t1111, 1);
        gem_pop_frame();
        return _t1112;
    }
#line 748 "compiler/main.gem"
    GemVal _t1113 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1113, gem_string("type")), gem_string("extern")))) {
#line 749 "compiler/main.gem"
    GemVal _t1114 = (*gem_v_advance);
        (void)(_t1114.fn(_t1114.env, NULL, 0));
#line 750 "compiler/main.gem"
    GemVal _t1115 = (*gem_v_peek);
    GemVal _t1116 = _t1115.fn(_t1115.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1116, gem_string("type")), gem_string("fn")))) {
#line 751 "compiler/main.gem"
    GemVal _t1117 = (*gem_v_advance);
            (void)(_t1117.fn(_t1117.env, NULL, 0));
#line 752 "compiler/main.gem"
    GemVal _t1118[] = {gem_string("NAME")};
    GemVal _t1119 = (*gem_v_expect);
    GemVal _t1120 = _t1119.fn(_t1119.env, _t1118, 1);
            GemVal gem_v_name = gem_table_get(_t1120, gem_string("value"));
#line 753 "compiler/main.gem"
    GemVal _t1121[] = {gem_string("(")};
    GemVal _t1122 = (*gem_v_expect);
            (void)(_t1122.fn(_t1122.env, _t1121, 1));
#line 754 "compiler/main.gem"
    GemVal _t1123 = gem_table_new();
            GemVal gem_v_eparams = _t1123;
#line 755 "compiler/main.gem"
    GemVal _t1124 = (*gem_v_peek);
    GemVal _t1125 = _t1124.fn(_t1124.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t1125, gem_string("type")), gem_string(")")))) {
#line 756 "compiler/main.gem"
    GemVal _t1126[] = {gem_string("NAME")};
    GemVal _t1127 = (*gem_v_expect);
    GemVal _t1128 = _t1127.fn(_t1127.env, _t1126, 1);
                GemVal gem_v_pname = gem_table_get(_t1128, gem_string("value"));
#line 757 "compiler/main.gem"
    GemVal _t1129[] = {gem_string(":")};
    GemVal _t1130 = (*gem_v_expect);
                (void)(_t1130.fn(_t1130.env, _t1129, 1));
#line 758 "compiler/main.gem"
    GemVal _t1131[] = {gem_string("NAME")};
    GemVal _t1132 = (*gem_v_expect);
    GemVal _t1133 = _t1132.fn(_t1132.env, _t1131, 1);
                GemVal gem_v_ptype = gem_table_get(_t1133, gem_string("value"));
#line 759 "compiler/main.gem"
    GemVal _t1134[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1135[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1134, 2)};
                (void)(gem_push_fn(NULL, _t1135, 2));
#line 760 "compiler/main.gem"
                while (1) {
                    GemVal _t1136 = (*gem_v_peek);
                    GemVal _t1137 = _t1136.fn(_t1136.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1137, gem_string("type")), gem_string(",")))) break;
#line 761 "compiler/main.gem"
    GemVal _t1138 = (*gem_v_advance);
                    (void)(_t1138.fn(_t1138.env, NULL, 0));
#line 762 "compiler/main.gem"
    GemVal _t1139[] = {gem_string("NAME")};
    GemVal _t1140 = (*gem_v_expect);
    GemVal _t1141 = _t1140.fn(_t1140.env, _t1139, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t1141, gem_string("value"));
#line 763 "compiler/main.gem"
    GemVal _t1142[] = {gem_string(":")};
    GemVal _t1143 = (*gem_v_expect);
                    (void)(_t1143.fn(_t1143.env, _t1142, 1));
#line 764 "compiler/main.gem"
    GemVal _t1144[] = {gem_string("NAME")};
    GemVal _t1145 = (*gem_v_expect);
    GemVal _t1146 = _t1145.fn(_t1145.env, _t1144, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t1146, gem_string("value"));
#line 765 "compiler/main.gem"
    GemVal _t1147[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1148[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1147, 2)};
                    (void)(gem_push_fn(NULL, _t1148, 2));
                }
            }
#line 768 "compiler/main.gem"
    GemVal _t1149[] = {gem_string(")")};
    GemVal _t1150 = (*gem_v_expect);
            (void)(_t1150.fn(_t1150.env, _t1149, 1));
#line 769 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 770 "compiler/main.gem"
    GemVal _t1151 = (*gem_v_peek);
    GemVal _t1152 = _t1151.fn(_t1151.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1152, gem_string("type")), gem_string("->")))) {
#line 771 "compiler/main.gem"
    GemVal _t1153 = (*gem_v_advance);
                (void)(_t1153.fn(_t1153.env, NULL, 0));
#line 772 "compiler/main.gem"
    GemVal _t1154[] = {gem_string("NAME")};
    GemVal _t1155 = (*gem_v_expect);
    GemVal _t1156 = _t1155.fn(_t1155.env, _t1154, 1);
                gem_v_ret_type = gem_table_get(_t1156, gem_string("value"));
            }
#line 774 "compiler/main.gem"
    GemVal _t1157[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            GemVal _t1158 = gem_fn_make_extern_fn(NULL, _t1157, 3);
            gem_pop_frame();
            return _t1158;
        } else {
#line 775 "compiler/main.gem"
    GemVal _t1159 = (*gem_v_peek);
    GemVal _t1160 = _t1159.fn(_t1159.env, NULL, 0);
    GemVal _t1163;
    if (!gem_truthy(gem_eq(gem_table_get(_t1160, gem_string("type")), gem_string("NAME")))) {
        _t1163 = gem_eq(gem_table_get(_t1160, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1161 = (*gem_v_peek);
        GemVal _t1162 = _t1161.fn(_t1161.env, NULL, 0);
        _t1163 = gem_eq(gem_table_get(_t1162, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t1163)) {
#line 776 "compiler/main.gem"
    GemVal _t1164 = (*gem_v_advance);
                (void)(_t1164.fn(_t1164.env, NULL, 0));
#line 777 "compiler/main.gem"
    GemVal _t1165[] = {gem_string("STRING")};
    GemVal _t1166 = (*gem_v_expect);
    GemVal _t1167 = _t1166.fn(_t1166.env, _t1165, 1);
                GemVal gem_v_path = gem_table_get(_t1167, gem_string("value"));
#line 778 "compiler/main.gem"
    GemVal _t1168[] = {gem_v_path};
                GemVal _t1169 = gem_fn_make_extern_include(NULL, _t1168, 1);
                gem_pop_frame();
                return _t1169;
            } else {
#line 780 "compiler/main.gem"
    GemVal _t1170 = (*gem_v_peek);
    GemVal _t1171 = _t1170.fn(_t1170.env, NULL, 0);
    GemVal _t1172[] = {gem_table_get(_t1171, gem_string("line"))};
    GemVal _t1173[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t1172, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 780, _t1173, 1));
            }
        }
    }
#line 785 "compiler/main.gem"
    GemVal _t1174 = (*gem_v_parse_expr);
    GemVal _t1175 = _t1174.fn(_t1174.env, NULL, 0);
    gem_pop_frame();
    return _t1175;
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
    GemVal _t1177 = gem_table_new();
    GemVal gem_v_stmts = _t1177;
#line 795 "compiler/main.gem"
    GemVal _t1178 = (*gem_v_skip_nl);
    (void)(_t1178.fn(_t1178.env, NULL, 0));
#line 796 "compiler/main.gem"
    while (1) {
        GemVal _t1179 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1179.fn(_t1179.env, NULL, 0)))) break;
#line 797 "compiler/main.gem"
    GemVal _t1180 = (*gem_v_parse_stmt);
    GemVal _t1181[] = {gem_v_stmts, _t1180.fn(_t1180.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1181, 2));
#line 798 "compiler/main.gem"
    GemVal _t1182 = (*gem_v_skip_nl);
        (void)(_t1182.fn(_t1182.env, NULL, 0));
    }
#line 800 "compiler/main.gem"
    GemVal _t1183[] = {gem_v_stmts};
    GemVal _t1184 = gem_fn_make_program(NULL, _t1183, 1);
    gem_pop_frame();
    return _t1184;
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
    struct _closure__anon_1 *_t340 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t340->gem_v_pos = gem_v_pos;
    _t340->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t340);
#line 29 "compiler/main.gem"
    struct _closure__anon_2 *_t342 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t342->gem_v_pos = gem_v_pos;
    _t342->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t342);
#line 33 "compiler/main.gem"
    struct _closure__anon_3 *_t344 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t344->gem_v_pos = gem_v_pos;
    _t344->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t344);
#line 39 "compiler/main.gem"
    struct _closure__anon_4 *_t347 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t347->gem_v_pos = gem_v_pos;
    _t347->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t347);
#line 43 "compiler/main.gem"
    struct _closure__anon_5 *_t356 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t356->gem_v_pos = gem_v_pos;
    _t356->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t356);
#line 52 "compiler/main.gem"
    struct _closure__anon_6 *_t358 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t358->gem_v_pos = gem_v_pos;
    _t358->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t358);
#line 64 "compiler/main.gem"
    struct _closure__anon_7 *_t396 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t396->gem_v_advance = gem_v_advance;
    _t396->gem_v_expect = gem_v_expect;
    _t396->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t396);
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
    struct _closure__anon_8 *_t416 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t416->gem_v_at_end = gem_v_at_end;
    _t416->gem_v_parse_stmt = gem_v_parse_stmt;
    _t416->gem_v_peek = gem_v_peek;
    _t416->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t416);
#line 119 "compiler/main.gem"
    struct _closure__anon_9 *_t427 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t427->gem_v_at_end = gem_v_at_end;
    _t427->gem_v_parse_stmt = gem_v_parse_stmt;
    _t427->gem_v_peek = gem_v_peek;
    _t427->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t427);
#line 132 "compiler/main.gem"
    struct _closure__anon_10 *_t559 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t559->gem_v_advance = gem_v_advance;
    _t559->gem_v_expect = gem_v_expect;
    _t559->gem_v_fn_depth = gem_v_fn_depth;
    _t559->gem_v_parse_expr = gem_v_parse_expr;
    _t559->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t559->gem_v_parse_params = gem_v_parse_params;
    _t559->gem_v_peek = gem_v_peek;
    _t559->gem_v_peek_at = gem_v_peek_at;
    _t559->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t559);
#line 263 "compiler/main.gem"
    struct _closure__anon_11 *_t650 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t650->gem_v_advance = gem_v_advance;
    _t650->gem_v_expect = gem_v_expect;
    _t650->gem_v_fn_depth = gem_v_fn_depth;
    _t650->gem_v_parse_atom = gem_v_parse_atom;
    _t650->gem_v_parse_expr = gem_v_parse_expr;
    _t650->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t650->gem_v_parse_params = gem_v_parse_params;
    _t650->gem_v_peek = gem_v_peek;
    _t650->gem_v_peek_at = gem_v_peek_at;
    _t650->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t650);
#line 369 "compiler/main.gem"
    struct _closure__anon_12 *_t659 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t659->gem_v_advance = gem_v_advance;
    _t659->gem_v_parse_call = gem_v_parse_call;
    _t659->gem_v_parse_unary = gem_v_parse_unary;
    _t659->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t659);
#line 378 "compiler/main.gem"
    struct _closure__anon_13 *_t674 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t674->gem_v_advance = gem_v_advance;
    _t674->gem_v_parse_unary = gem_v_parse_unary;
    _t674->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t674);
#line 389 "compiler/main.gem"
    struct _closure__anon_14 *_t686 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t686->gem_v_advance = gem_v_advance;
    _t686->gem_v_parse_mul = gem_v_parse_mul;
    _t686->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t686);
#line 400 "compiler/main.gem"
    struct _closure__anon_15 *_t713 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t713->gem_v_advance = gem_v_advance;
    _t713->gem_v_parse_add = gem_v_parse_add;
    _t713->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t713);
#line 411 "compiler/main.gem"
    struct _closure__anon_16 *_t722 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t722->gem_v_advance = gem_v_advance;
    _t722->gem_v_parse_compare = gem_v_parse_compare;
    _t722->gem_v_parse_not = gem_v_parse_not;
    _t722->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t722);
#line 420 "compiler/main.gem"
    struct _closure__anon_17 *_t730 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t730->gem_v_advance = gem_v_advance;
    _t730->gem_v_parse_not = gem_v_parse_not;
    _t730->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t730);
#line 431 "compiler/main.gem"
    struct _closure__anon_18 *_t738 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t738->gem_v_advance = gem_v_advance;
    _t738->gem_v_parse_and = gem_v_parse_and;
    _t738->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t738);
#line 442 "compiler/main.gem"
    struct _closure__anon_19 *_t787 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t787->gem_v_advance = gem_v_advance;
    _t787->gem_v_parse_expr = gem_v_parse_expr;
    _t787->gem_v_parse_or = gem_v_parse_or;
    _t787->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t787);
#line 477 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 481 "compiler/main.gem"
    struct _closure__anon_20 *_t1176 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t1176->gem_v_advance = gem_v_advance;
    _t1176->gem_v_expect = gem_v_expect;
    _t1176->gem_v_fn_depth = gem_v_fn_depth;
    _t1176->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1176->gem_v_parse_body = gem_v_parse_body;
    _t1176->gem_v_parse_expr = gem_v_parse_expr;
    _t1176->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1176->gem_v_parse_params = gem_v_parse_params;
    _t1176->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1176->gem_v_peek = gem_v_peek;
    _t1176->gem_v_peek_at = gem_v_peek_at;
    _t1176->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_20, _t1176);
#line 789 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 793 "compiler/main.gem"
    struct _closure__anon_21 *_t1185 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t1185->gem_v_at_end = gem_v_at_end;
    _t1185->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1185->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_21, _t1185);
    GemVal _t1186 = gem_table_new();
    gem_table_set(_t1186, gem_string("parse"), gem_v_parse);
    GemVal _t1187 = _t1186;
    gem_pop_frame();
    return _t1187;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("escape_c_string", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal _t1188[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1188, 1);
#line 11 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 12 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 12 "compiler/main.gem"
    GemVal _t1189[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1189, 1);
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
    GemVal _t1190[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1190, 1), gem_int(0)))) {
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

    GemVal _t1191 = gem_v_result;
    gem_pop_frame();
    return _t1191;
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

    GemVal _t1192 = gem_v_result;
    gem_pop_frame();
    return _t1192;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 41 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("pad_for", "compiler/main.gem", 41);
#line 42 "compiler/main.gem"
    GemVal _t1193[] = {gem_string("    "), gem_v_indent};
    GemVal _t1194 = gem_fn_repeat_str(NULL, _t1193, 2);
    gem_pop_frame();
    return _t1194;
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 45 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("format_float", "compiler/main.gem", 45);
#line 46 "compiler/main.gem"
    GemVal _t1195[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1195, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 49 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 49 "compiler/main.gem"
    GemVal _t1196[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1196, 1);
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
    GemVal _t1197 = gem_v_s;
    gem_pop_frame();
    return _t1197;
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
    GemVal _t1198 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1198;
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 72 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_union", "compiler/main.gem", 72);
#line 73 "compiler/main.gem"
    GemVal _t1199 = gem_table_new();
    GemVal gem_v_result = _t1199;
#line 74 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 74 "compiler/main.gem"
    GemVal _t1200[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_keys_4 = gem_keys_fn(NULL, _t1200, 1);
#line 74 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 74 "compiler/main.gem"
    while (1) {
        GemVal _t1201[] = {gem_v__for_keys_4};
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_len_fn(NULL, _t1201, 1)))) break;
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
    GemVal _t1202[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_keys_5 = gem_keys_fn(NULL, _t1202, 1);
#line 77 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 77 "compiler/main.gem"
    while (1) {
        GemVal _t1203[] = {gem_v__for_keys_5};
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1203, 1)))) break;
#line 77 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_5, gem_v__for_i_5);
#line 77 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_5, gem_table_get(gem_v__for_keys_5, gem_v__for_i_5));
#line 77 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 78 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1204 = gem_v_result;
    gem_pop_frame();
    return _t1204;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 83 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_intersect", "compiler/main.gem", 83);
#line 84 "compiler/main.gem"
    GemVal _t1205 = gem_table_new();
    GemVal gem_v_result = _t1205;
#line 85 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 85 "compiler/main.gem"
    GemVal _t1206[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_keys_6 = gem_keys_fn(NULL, _t1206, 1);
#line 85 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 85 "compiler/main.gem"
    while (1) {
        GemVal _t1207[] = {gem_v__for_keys_6};
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1207, 1)))) break;
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

    GemVal _t1208 = gem_v_result;
    gem_pop_frame();
    return _t1208;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("set_to_sorted_array", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t1209[] = {gem_v_s};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t1209, 1);
#line 96 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 97 "compiler/main.gem"
    while (1) {
        GemVal _t1210[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t1210, 1)))) break;
#line 98 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
#line 99 "compiler/main.gem"
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
#line 100 "compiler/main.gem"
        while (1) {
            GemVal _t1211;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t1211 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t1211 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t1211)) break;
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
    GemVal _t1212 = gem_v_ks;
    gem_pop_frame();
    return _t1212;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 110 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("sorted_array_to_set", "compiler/main.gem", 110);
#line 111 "compiler/main.gem"
    GemVal _t1213 = gem_table_new();
    GemVal gem_v_s = _t1213;
#line 112 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 112 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 112 "compiler/main.gem"
    while (1) {
        GemVal _t1214[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1214, 1)))) break;
#line 112 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 112 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 113 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1215 = gem_v_s;
    gem_pop_frame();
    return _t1215;
}

static GemVal gem_fn_collect_block_lets(void *_env, GemVal *args, int argc) {
#line 120 "compiler/main.gem"
    GemVal gem_v_stmt = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("collect_block_lets", "compiler/main.gem", 120);
#line 121 "compiler/main.gem"
    GemVal _t1216[] = {gem_v_stmt};
    GemVal _t1218;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1216, 1), gem_string("table")))) {
        _t1218 = gem_eq(gem_type_fn(NULL, _t1216, 1), gem_string("table"));
    } else {
        GemVal _t1217 = gem_v_stmt;
        _t1218 = gem_eq(gem_table_get(_t1217, gem_string("tag")), gem_string("block"));
    }
    if (gem_truthy(_t1218)) {
        {
#line 122 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 122 "compiler/main.gem"
    GemVal _t1219 = gem_v_stmt;
    GemVal _t1220[] = {gem_table_get(_t1219, gem_string("stmts"))};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1220, 1);
#line 122 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 122 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_8;
#line 122 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 123 "compiler/main.gem"
    GemVal _t1221 = gem_v_stmt;
    GemVal _t1222[] = {gem_table_get(gem_table_get(_t1221, gem_string("stmts")), gem_v_j)};
    GemVal _t1225;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1222, 1), gem_string("table")))) {
        _t1225 = gem_eq(gem_type_fn(NULL, _t1222, 1), gem_string("table"));
    } else {
        GemVal _t1223 = gem_v_stmt;
        GemVal _t1224 = gem_table_get(gem_table_get(_t1223, gem_string("stmts")), gem_v_j);
        _t1225 = gem_eq(gem_table_get(_t1224, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1225)) {
#line 124 "compiler/main.gem"
    GemVal _t1226 = gem_v_stmt;
    GemVal _t1227 = gem_table_get(gem_table_get(_t1226, gem_string("stmts")), gem_v_j);
    GemVal _t1228[] = {gem_v_result, gem_table_get(_t1227, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1228, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn_collect_top_let_names(void *_env, GemVal *args, int argc) {
#line 130 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("collect_top_let_names", "compiler/main.gem", 130);
    {
#line 131 "compiler/main.gem"
        GemVal gem_v__for_i_9 = gem_int(0);
#line 131 "compiler/main.gem"
    GemVal _t1229[] = {gem_v_stmts};
        GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1229, 1);
#line 131 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 131 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_9;
#line 131 "compiler/main.gem"
            gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 132 "compiler/main.gem"
            GemVal gem_v_s = gem_table_get(gem_v_stmts, gem_v_i);
#line 133 "compiler/main.gem"
    GemVal _t1230[] = {gem_v_s};
    GemVal _t1232;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1230, 1), gem_string("table")))) {
        _t1232 = gem_eq(gem_type_fn(NULL, _t1230, 1), gem_string("table"));
    } else {
        GemVal _t1231 = gem_v_s;
        _t1232 = gem_eq(gem_table_get(_t1231, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1232)) {
#line 134 "compiler/main.gem"
    GemVal _t1233 = gem_v_s;
    GemVal _t1234[] = {gem_v_result, gem_table_get(_t1233, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1234, 2));
            }
#line 136 "compiler/main.gem"
    GemVal _t1235[] = {gem_v_s, gem_v_result};
            (void)(gem_fn_collect_block_lets(NULL, _t1235, 2));
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
#line 181 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t1247[] = {(*gem_v_tmp_counter)};
    GemVal _t1248 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t1247, 1));
    gem_pop_frame();
    return _t1248;
}

struct _closure__anon_23 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_23", "compiler/main.gem", 0);
#line 186 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t1250[] = {(*gem_v_anon_counter)};
    GemVal _t1251 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t1250, 1));
    gem_pop_frame();
    return _t1251;
}

struct _closure__anon_24 {
};
static GemVal _anon_24(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_24", "compiler/main.gem", 0);
    GemVal _t1253[] = {gem_v_name};
    GemVal _t1254 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t1253, 1));
    gem_pop_frame();
    return _t1254;
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
#line 200 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 201 "compiler/main.gem"
        GemVal _t1255 = GEM_NIL;
        gem_pop_frame();
        return _t1255;
    }
#line 203 "compiler/main.gem"
    GemVal _t1256[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1256, 1), gem_string("table")))) {
#line 204 "compiler/main.gem"
        GemVal _t1257 = GEM_NIL;
        gem_pop_frame();
        return _t1257;
    }
#line 206 "compiler/main.gem"
    GemVal _t1258 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1258, gem_string("tag"));
#line 207 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 208 "compiler/main.gem"
        GemVal _t1259 = GEM_NIL;
        gem_pop_frame();
        return _t1259;
    }
#line 211 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 212 "compiler/main.gem"
    GemVal _t1260 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1260, gem_string("name"));
#line 213 "compiler/main.gem"
    GemVal _t1261[] = {gem_v_defined, gem_v_name};
    GemVal _t1263;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1261, 2)))) {
        _t1263 = gem_not(gem_fn_set_contains(NULL, _t1261, 2));
    } else {
        GemVal _t1262[] = {(*gem_v_builtins), gem_v_name};
        _t1263 = gem_not(gem_fn_set_contains(NULL, _t1262, 2));
    }
    GemVal _t1265;
    if (!gem_truthy(_t1263)) {
        _t1265 = _t1263;
    } else {
        GemVal _t1264[] = {(*gem_v_defined_fns), gem_v_name};
        _t1265 = gem_not(gem_fn_set_contains(NULL, _t1264, 2));
    }
        if (gem_truthy(_t1265)) {
#line 214 "compiler/main.gem"
    GemVal _t1266[] = {gem_v_free, gem_v_name};
            GemVal _t1267 = gem_fn_set_add(NULL, _t1266, 2);
            gem_pop_frame();
            return _t1267;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
#line 216 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 217 "compiler/main.gem"
    GemVal _t1268 = gem_v_node;
    GemVal _t1269[] = {gem_table_get(_t1268, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1270 = (*gem_v_collect_free_node);
            GemVal _t1271 = _t1270.fn(_t1270.env, _t1269, 3);
            gem_pop_frame();
            return _t1271;
        } else {
#line 218 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 219 "compiler/main.gem"
    GemVal _t1272 = gem_v_node;
                GemVal gem_v_name = gem_table_get(_t1272, gem_string("name"));
#line 220 "compiler/main.gem"
    GemVal _t1273[] = {gem_v_defined, gem_v_name};
    GemVal _t1275;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1273, 2)))) {
        _t1275 = gem_not(gem_fn_set_contains(NULL, _t1273, 2));
    } else {
        GemVal _t1274[] = {(*gem_v_builtins), gem_v_name};
        _t1275 = gem_not(gem_fn_set_contains(NULL, _t1274, 2));
    }
    GemVal _t1277;
    if (!gem_truthy(_t1275)) {
        _t1277 = _t1275;
    } else {
        GemVal _t1276[] = {(*gem_v_defined_fns), gem_v_name};
        _t1277 = gem_not(gem_fn_set_contains(NULL, _t1276, 2));
    }
                if (gem_truthy(_t1277)) {
#line 221 "compiler/main.gem"
    GemVal _t1278[] = {gem_v_free, gem_v_name};
                    (void)(gem_fn_set_add(NULL, _t1278, 2));
                }
#line 223 "compiler/main.gem"
    GemVal _t1279 = gem_v_node;
    GemVal _t1280[] = {gem_table_get(_t1279, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1281 = (*gem_v_collect_free_node);
                GemVal _t1282 = _t1281.fn(_t1281.env, _t1280, 3);
                gem_pop_frame();
                return _t1282;
            } else {
#line 224 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 225 "compiler/main.gem"
    GemVal _t1283 = gem_v_node;
    GemVal _t1284[] = {gem_table_get(_t1283, gem_string("params"))};
    GemVal _t1285[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t1284, 1)};
                    GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t1285, 2);
#line 226 "compiler/main.gem"
    GemVal _t1286 = gem_v_node;
                    if (gem_truthy(gem_neq(gem_table_get(_t1286, gem_string("rest_param")), GEM_NIL))) {
#line 227 "compiler/main.gem"
    GemVal _t1287 = gem_v_node;
    GemVal _t1288[] = {gem_v_inner_defined, gem_table_get(_t1287, gem_string("rest_param"))};
                        (void)(gem_fn_set_add(NULL, _t1288, 2));
                    }
#line 229 "compiler/main.gem"
    GemVal _t1289 = gem_v_node;
                    if (gem_truthy(gem_neq(gem_table_get(_t1289, gem_string("block_param")), GEM_NIL))) {
#line 230 "compiler/main.gem"
    GemVal _t1290 = gem_v_node;
    GemVal _t1291[] = {gem_v_inner_defined, gem_table_get(_t1290, gem_string("block_param"))};
                        (void)(gem_fn_set_add(NULL, _t1291, 2));
                    }
#line 232 "compiler/main.gem"
    GemVal _t1292 = gem_v_node;
    GemVal _t1293[] = {gem_table_get(_t1292, gem_string("body")), gem_v_inner_defined};
    GemVal _t1294 = (*gem_v_collect_free_vars);
                    GemVal gem_v_inner_free = _t1294.fn(_t1294.env, _t1293, 2);
                    {
#line 233 "compiler/main.gem"
                        GemVal gem_v__for_tbl_11 = gem_v_inner_free;
#line 233 "compiler/main.gem"
    GemVal _t1295[] = {gem_v__for_tbl_11};
                        GemVal gem_v__for_keys_11 = gem_keys_fn(NULL, _t1295, 1);
#line 233 "compiler/main.gem"
                        GemVal gem_v__for_i_11 = gem_int(0);
#line 233 "compiler/main.gem"
                        while (1) {
                            GemVal _t1296[] = {gem_v__for_keys_11};
                            if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_len_fn(NULL, _t1296, 1)))) break;
#line 233 "compiler/main.gem"
                            GemVal gem_v_ifk = gem_table_get(gem_v__for_keys_11, gem_v__for_i_11);
#line 233 "compiler/main.gem"
                            GemVal gem_v__ = gem_table_get(gem_v__for_tbl_11, gem_table_get(gem_v__for_keys_11, gem_v__for_i_11));
#line 233 "compiler/main.gem"
                            gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 234 "compiler/main.gem"
    GemVal _t1297[] = {gem_v_free, gem_v_ifk};
                            (void)(gem_fn_set_add(NULL, _t1297, 2));
                        }
                        gem_pop_frame();
                        return GEM_NIL;
                    }
                } else {
#line 236 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                        gem_pop_frame();
                        return GEM_NIL;
                    } else {
#line 238 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 239 "compiler/main.gem"
    GemVal _t1298 = gem_v_node;
    GemVal _t1299[] = {gem_table_get(_t1298, gem_string("cond")), gem_v_defined, gem_v_free};
    GemVal _t1300 = (*gem_v_collect_free_node);
                            (void)(_t1300.fn(_t1300.env, _t1299, 3));
#line 240 "compiler/main.gem"
    GemVal _t1301 = gem_table_new();
    GemVal _t1302[] = {gem_v_defined, _t1301};
                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1302, 2);
#line 241 "compiler/main.gem"
    GemVal _t1303 = gem_v_node;
                            GemVal gem_v_thens = gem_table_get(_t1303, gem_string("then"));
#line 242 "compiler/main.gem"
                            GemVal gem_v__for_i_12 = gem_int(0);
#line 242 "compiler/main.gem"
    GemVal _t1304[] = {gem_v_thens};
                            GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t1304, 1);
#line 242 "compiler/main.gem"
                            while (1) {
                                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 242 "compiler/main.gem"
                                GemVal gem_v_i = gem_v__for_i_12;
#line 242 "compiler/main.gem"
                                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 243 "compiler/main.gem"
    GemVal _t1305[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1306 = (*gem_v_collect_free_node);
                                (void)(_t1306.fn(_t1306.env, _t1305, 3));
#line 244 "compiler/main.gem"
    GemVal _t1307[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t1309;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1307, 1), gem_string("table")))) {
        _t1309 = gem_eq(gem_type_fn(NULL, _t1307, 1), gem_string("table"));
    } else {
        GemVal _t1308 = gem_table_get(gem_v_thens, gem_v_i);
        _t1309 = gem_eq(gem_table_get(_t1308, gem_string("tag")), gem_string("let"));
    }
                                if (gem_truthy(_t1309)) {
#line 245 "compiler/main.gem"
    GemVal _t1310 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1311[] = {gem_v_d, gem_table_get(_t1310, gem_string("name"))};
                                    (void)(gem_fn_set_add(NULL, _t1311, 2));
                                }
                            }

#line 248 "compiler/main.gem"
    GemVal _t1312 = gem_v_node;
                            GemVal gem_v_el = gem_table_get(_t1312, gem_string("else"));
#line 249 "compiler/main.gem"
                            if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 250 "compiler/main.gem"
    GemVal _t1313 = gem_table_new();
    GemVal _t1314[] = {gem_v_defined, _t1313};
                                gem_v_d = gem_fn_set_union(NULL, _t1314, 2);
                                {
#line 251 "compiler/main.gem"
                                    GemVal gem_v__for_i_13 = gem_int(0);
#line 251 "compiler/main.gem"
    GemVal _t1315[] = {gem_v_el};
                                    GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1315, 1);
#line 251 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 251 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_13;
#line 251 "compiler/main.gem"
                                        gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 252 "compiler/main.gem"
    GemVal _t1316[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1317 = (*gem_v_collect_free_node);
                                        (void)(_t1317.fn(_t1317.env, _t1316, 3));
#line 253 "compiler/main.gem"
    GemVal _t1318[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t1320;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1318, 1), gem_string("table")))) {
        _t1320 = gem_eq(gem_type_fn(NULL, _t1318, 1), gem_string("table"));
    } else {
        GemVal _t1319 = gem_table_get(gem_v_el, gem_v_i);
        _t1320 = gem_eq(gem_table_get(_t1319, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t1320)) {
#line 254 "compiler/main.gem"
    GemVal _t1321 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1322[] = {gem_v_d, gem_table_get(_t1321, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t1322, 2));
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
#line 258 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 259 "compiler/main.gem"
    GemVal _t1323 = gem_v_node;
    GemVal _t1324[] = {gem_table_get(_t1323, gem_string("cond")), gem_v_defined, gem_v_free};
    GemVal _t1325 = (*gem_v_collect_free_node);
                                (void)(_t1325.fn(_t1325.env, _t1324, 3));
#line 260 "compiler/main.gem"
    GemVal _t1326 = gem_table_new();
    GemVal _t1327[] = {gem_v_defined, _t1326};
                                GemVal gem_v_d = gem_fn_set_union(NULL, _t1327, 2);
                                {
#line 261 "compiler/main.gem"
                                    GemVal gem_v__for_i_14 = gem_int(0);
#line 261 "compiler/main.gem"
    GemVal _t1328 = gem_v_node;
    GemVal _t1329[] = {gem_table_get(_t1328, gem_string("body"))};
                                    GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1329, 1);
#line 261 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 261 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_14;
#line 261 "compiler/main.gem"
                                        gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 262 "compiler/main.gem"
    GemVal _t1330 = gem_v_node;
    GemVal _t1331[] = {gem_table_get(gem_table_get(_t1330, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1332 = (*gem_v_collect_free_node);
                                        (void)(_t1332.fn(_t1332.env, _t1331, 3));
#line 263 "compiler/main.gem"
    GemVal _t1333 = gem_v_node;
    GemVal _t1334[] = {gem_table_get(gem_table_get(_t1333, gem_string("body")), gem_v_i)};
    GemVal _t1337;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1334, 1), gem_string("table")))) {
        _t1337 = gem_eq(gem_type_fn(NULL, _t1334, 1), gem_string("table"));
    } else {
        GemVal _t1335 = gem_v_node;
        GemVal _t1336 = gem_table_get(gem_table_get(_t1335, gem_string("body")), gem_v_i);
        _t1337 = gem_eq(gem_table_get(_t1336, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t1337)) {
#line 264 "compiler/main.gem"
    GemVal _t1338 = gem_v_node;
    GemVal _t1339 = gem_table_get(gem_table_get(_t1338, gem_string("body")), gem_v_i);
    GemVal _t1340[] = {gem_v_d, gem_table_get(_t1339, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t1340, 2));
                                        }
                                    }
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 267 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 268 "compiler/main.gem"
    GemVal _t1341 = gem_v_node;
    GemVal _t1342[] = {gem_table_get(_t1341, gem_string("target")), gem_v_defined, gem_v_free};
    GemVal _t1343 = (*gem_v_collect_free_node);
                                    (void)(_t1343.fn(_t1343.env, _t1342, 3));
#line 269 "compiler/main.gem"
                                    GemVal gem_v__for_i_16 = gem_int(0);
#line 269 "compiler/main.gem"
    GemVal _t1344 = gem_v_node;
    GemVal _t1345[] = {gem_table_get(_t1344, gem_string("whens"))};
                                    GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1345, 1);
#line 269 "compiler/main.gem"
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 269 "compiler/main.gem"
                                        GemVal gem_v_i = gem_v__for_i_16;
#line 269 "compiler/main.gem"
                                        gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 270 "compiler/main.gem"
    GemVal _t1346 = gem_v_node;
                                        GemVal gem_v_w = gem_table_get(gem_table_get(_t1346, gem_string("whens")), gem_v_i);
#line 271 "compiler/main.gem"
    GemVal _t1347 = gem_v_w;
    GemVal _t1348[] = {gem_table_get(_t1347, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1349 = (*gem_v_collect_free_node);
                                        (void)(_t1349.fn(_t1349.env, _t1348, 3));
#line 272 "compiler/main.gem"
    GemVal _t1350 = gem_table_new();
    GemVal _t1351[] = {gem_v_defined, _t1350};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1351, 2);
#line 273 "compiler/main.gem"
                                        GemVal gem_v__for_i_15 = gem_int(0);
#line 273 "compiler/main.gem"
    GemVal _t1352 = gem_v_w;
    GemVal _t1353[] = {gem_table_get(_t1352, gem_string("body"))};
                                        GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1353, 1);
#line 273 "compiler/main.gem"
                                        while (1) {
                                            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 273 "compiler/main.gem"
                                            GemVal gem_v_j = gem_v__for_i_15;
#line 273 "compiler/main.gem"
                                            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 274 "compiler/main.gem"
    GemVal _t1354 = gem_v_w;
    GemVal _t1355[] = {gem_table_get(gem_table_get(_t1354, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1356 = (*gem_v_collect_free_node);
                                            (void)(_t1356.fn(_t1356.env, _t1355, 3));
#line 275 "compiler/main.gem"
    GemVal _t1357 = gem_v_w;
    GemVal _t1358[] = {gem_table_get(gem_table_get(_t1357, gem_string("body")), gem_v_j)};
    GemVal _t1361;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1358, 1), gem_string("table")))) {
        _t1361 = gem_eq(gem_type_fn(NULL, _t1358, 1), gem_string("table"));
    } else {
        GemVal _t1359 = gem_v_w;
        GemVal _t1360 = gem_table_get(gem_table_get(_t1359, gem_string("body")), gem_v_j);
        _t1361 = gem_eq(gem_table_get(_t1360, gem_string("tag")), gem_string("let"));
    }
                                            if (gem_truthy(_t1361)) {
#line 276 "compiler/main.gem"
    GemVal _t1362 = gem_v_w;
    GemVal _t1363 = gem_table_get(gem_table_get(_t1362, gem_string("body")), gem_v_j);
    GemVal _t1364[] = {gem_v_d, gem_table_get(_t1363, gem_string("name"))};
                                                (void)(gem_fn_set_add(NULL, _t1364, 2));
                                            }
                                        }

                                    }

#line 280 "compiler/main.gem"
    GemVal _t1365 = gem_v_node;
                                    GemVal gem_v_el = gem_table_get(_t1365, gem_string("else"));
#line 281 "compiler/main.gem"
                                    if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 282 "compiler/main.gem"
    GemVal _t1366 = gem_table_new();
    GemVal _t1367[] = {gem_v_defined, _t1366};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1367, 2);
                                        {
#line 283 "compiler/main.gem"
                                            GemVal gem_v__for_i_17 = gem_int(0);
#line 283 "compiler/main.gem"
    GemVal _t1368[] = {gem_v_el};
                                            GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1368, 1);
#line 283 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 283 "compiler/main.gem"
                                                GemVal gem_v_j = gem_v__for_i_17;
#line 283 "compiler/main.gem"
                                                gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 284 "compiler/main.gem"
    GemVal _t1369[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1370 = (*gem_v_collect_free_node);
                                                (void)(_t1370.fn(_t1370.env, _t1369, 3));
#line 285 "compiler/main.gem"
    GemVal _t1371[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t1373;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1371, 1), gem_string("table")))) {
        _t1373 = gem_eq(gem_type_fn(NULL, _t1371, 1), gem_string("table"));
    } else {
        GemVal _t1372 = gem_table_get(gem_v_el, gem_v_j);
        _t1373 = gem_eq(gem_table_get(_t1372, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1373)) {
#line 286 "compiler/main.gem"
    GemVal _t1374 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t1375[] = {gem_v_d, gem_table_get(_t1374, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1375, 2));
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
#line 290 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("when_clause")))) {
#line 291 "compiler/main.gem"
    GemVal _t1376 = gem_v_node;
    GemVal _t1377[] = {gem_table_get(_t1376, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1378 = (*gem_v_collect_free_node);
                                        (void)(_t1378.fn(_t1378.env, _t1377, 3));
#line 292 "compiler/main.gem"
    GemVal _t1379 = gem_table_new();
    GemVal _t1380[] = {gem_v_defined, _t1379};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t1380, 2);
                                        {
#line 293 "compiler/main.gem"
                                            GemVal gem_v__for_i_18 = gem_int(0);
#line 293 "compiler/main.gem"
    GemVal _t1381 = gem_v_node;
    GemVal _t1382[] = {gem_table_get(_t1381, gem_string("body"))};
                                            GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1382, 1);
#line 293 "compiler/main.gem"
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 293 "compiler/main.gem"
                                                GemVal gem_v_i = gem_v__for_i_18;
#line 293 "compiler/main.gem"
                                                gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 294 "compiler/main.gem"
    GemVal _t1383 = gem_v_node;
    GemVal _t1384[] = {gem_table_get(gem_table_get(_t1383, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1385 = (*gem_v_collect_free_node);
                                                (void)(_t1385.fn(_t1385.env, _t1384, 3));
#line 295 "compiler/main.gem"
    GemVal _t1386 = gem_v_node;
    GemVal _t1387[] = {gem_table_get(gem_table_get(_t1386, gem_string("body")), gem_v_i)};
    GemVal _t1390;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1387, 1), gem_string("table")))) {
        _t1390 = gem_eq(gem_type_fn(NULL, _t1387, 1), gem_string("table"));
    } else {
        GemVal _t1388 = gem_v_node;
        GemVal _t1389 = gem_table_get(gem_table_get(_t1388, gem_string("body")), gem_v_i);
        _t1390 = gem_eq(gem_table_get(_t1389, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t1390)) {
#line 296 "compiler/main.gem"
    GemVal _t1391 = gem_v_node;
    GemVal _t1392 = gem_table_get(gem_table_get(_t1391, gem_string("body")), gem_v_i);
    GemVal _t1393[] = {gem_v_d, gem_table_get(_t1392, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t1393, 2));
                                                }
                                            }
                                            gem_pop_frame();
                                            return GEM_NIL;
                                        }
                                    } else {
#line 299 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 300 "compiler/main.gem"
    GemVal _t1394 = gem_table_new();
    GemVal _t1395[] = {gem_v_defined, _t1394};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t1395, 2);
                                            {
#line 301 "compiler/main.gem"
                                                GemVal gem_v__for_i_19 = gem_int(0);
#line 301 "compiler/main.gem"
    GemVal _t1396 = gem_v_node;
    GemVal _t1397[] = {gem_table_get(_t1396, gem_string("stmts"))};
                                                GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1397, 1);
#line 301 "compiler/main.gem"
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 301 "compiler/main.gem"
                                                    GemVal gem_v_i = gem_v__for_i_19;
#line 301 "compiler/main.gem"
                                                    gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 302 "compiler/main.gem"
    GemVal _t1398 = gem_v_node;
    GemVal _t1399[] = {gem_table_get(gem_table_get(_t1398, gem_string("stmts")), gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1400 = (*gem_v_collect_free_node);
                                                    (void)(_t1400.fn(_t1400.env, _t1399, 3));
#line 303 "compiler/main.gem"
    GemVal _t1401 = gem_v_node;
    GemVal _t1402[] = {gem_table_get(gem_table_get(_t1401, gem_string("stmts")), gem_v_i)};
    GemVal _t1405;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1402, 1), gem_string("table")))) {
        _t1405 = gem_eq(gem_type_fn(NULL, _t1402, 1), gem_string("table"));
    } else {
        GemVal _t1403 = gem_v_node;
        GemVal _t1404 = gem_table_get(gem_table_get(_t1403, gem_string("stmts")), gem_v_i);
        _t1405 = gem_eq(gem_table_get(_t1404, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t1405)) {
#line 304 "compiler/main.gem"
    GemVal _t1406 = gem_v_node;
    GemVal _t1407 = gem_table_get(gem_table_get(_t1406, gem_string("stmts")), gem_v_i);
    GemVal _t1408[] = {gem_v_d, gem_table_get(_t1407, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t1408, 2));
                                                    }
                                                }
                                                gem_pop_frame();
                                                return GEM_NIL;
                                            }
                                        } else {
#line 307 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 308 "compiler/main.gem"
    GemVal _t1409 = gem_v_node;
                                                if (gem_truthy(gem_neq(gem_table_get(_t1409, gem_string("value")), GEM_NIL))) {
#line 309 "compiler/main.gem"
    GemVal _t1410 = gem_v_node;
    GemVal _t1411[] = {gem_table_get(_t1410, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1412 = (*gem_v_collect_free_node);
                                                    GemVal _t1413 = _t1412.fn(_t1412.env, _t1411, 3);
                                                    gem_pop_frame();
                                                    return _t1413;
                                                } else {
                                                    gem_pop_frame();
                                                    return GEM_NIL;
                                                }
                                            } else {
                                                {
#line 313 "compiler/main.gem"
                                                    GemVal gem_v__for_tbl_20 = gem_v_node;
#line 313 "compiler/main.gem"
    GemVal _t1414[] = {gem_v__for_tbl_20};
                                                    GemVal gem_v__for_keys_20 = gem_keys_fn(NULL, _t1414, 1);
#line 313 "compiler/main.gem"
                                                    GemVal gem_v__for_i_20 = gem_int(0);
#line 313 "compiler/main.gem"
                                                    while (1) {
                                                        GemVal _t1415[] = {gem_v__for_keys_20};
                                                        if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_len_fn(NULL, _t1415, 1)))) break;
#line 313 "compiler/main.gem"
                                                        GemVal gem_v_nk = gem_table_get(gem_v__for_keys_20, gem_v__for_i_20);
#line 313 "compiler/main.gem"
                                                        GemVal gem_v_v = gem_table_get(gem_v__for_tbl_20, gem_table_get(gem_v__for_keys_20, gem_v__for_i_20));
#line 313 "compiler/main.gem"
                                                        gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 314 "compiler/main.gem"
                                                        if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 315 "compiler/main.gem"
    GemVal _t1416[] = {gem_v_v};
                                                            if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1416, 1), gem_string("table")))) {
#line 317 "compiler/main.gem"
    GemVal _t1417[] = {gem_v_v};
                                                                GemVal gem_v_vks = gem_keys_fn(NULL, _t1417, 1);
#line 318 "compiler/main.gem"
    GemVal _t1418[] = {gem_v_vks};
    GemVal _t1420;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1418, 1), gem_int(0)))) {
        _t1420 = gem_gt(gem_len_fn(NULL, _t1418, 1), gem_int(0));
    } else {
        GemVal _t1419[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1420 = gem_eq(gem_type_fn(NULL, _t1419, 1), gem_string("int"));
    }
                                                                if (gem_truthy(_t1420)) {
#line 320 "compiler/main.gem"
                                                                    GemVal gem_v_vi = gem_int(0);
#line 321 "compiler/main.gem"
                                                                    while (1) {
                                                                        GemVal _t1421[] = {gem_v_v};
                                                                        if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1421, 1)))) break;
#line 322 "compiler/main.gem"
    GemVal _t1422[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                        if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1422, 1), gem_string("table")))) {
#line 323 "compiler/main.gem"
    GemVal _t1423[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t1424 = (*gem_v_collect_free_node);
                                                                            (void)(_t1424.fn(_t1424.env, _t1423, 3));
                                                                        }
#line 325 "compiler/main.gem"
                                                                        gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                                    }
                                                                } else {
#line 328 "compiler/main.gem"
    GemVal _t1425[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t1426 = (*gem_v_collect_free_node);
                                                                    (void)(_t1426.fn(_t1426.env, _t1425, 3));
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
#line 337 "compiler/main.gem"
    GemVal _t1428 = gem_table_new();
    GemVal gem_v_free = _t1428;
#line 338 "compiler/main.gem"
    GemVal _t1429 = gem_table_new();
    GemVal _t1430[] = {gem_v_defined, _t1429};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t1430, 2);
#line 339 "compiler/main.gem"
    GemVal gem_v__for_i_21 = gem_int(0);
#line 339 "compiler/main.gem"
    GemVal _t1431[] = {gem_v_stmts};
    GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t1431, 1);
#line 339 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 339 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_21;
#line 339 "compiler/main.gem"
        gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 340 "compiler/main.gem"
    GemVal _t1432[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1433 = (*gem_v_collect_free_node);
        (void)(_t1433.fn(_t1433.env, _t1432, 3));
#line 341 "compiler/main.gem"
    GemVal _t1434[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1436;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1434, 1), gem_string("table")))) {
        _t1436 = gem_eq(gem_type_fn(NULL, _t1434, 1), gem_string("table"));
    } else {
        GemVal _t1435 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1436 = gem_eq(gem_table_get(_t1435, gem_string("tag")), gem_string("let"));
    }
        if (gem_truthy(_t1436)) {
#line 342 "compiler/main.gem"
    GemVal _t1437 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1438[] = {gem_v_d, gem_table_get(_t1437, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1438, 2));
        }
#line 344 "compiler/main.gem"
    GemVal _t1439[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d};
        (void)(gem_fn_collect_block_lets(NULL, _t1439, 2));
    }

    GemVal _t1440 = gem_v_free;
    gem_pop_frame();
    return _t1440;
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
#line 355 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 356 "compiler/main.gem"
        GemVal _t1442 = GEM_NIL;
        gem_pop_frame();
        return _t1442;
    }
#line 358 "compiler/main.gem"
    GemVal _t1443[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1443, 1), gem_string("table")))) {
#line 359 "compiler/main.gem"
        GemVal _t1444 = GEM_NIL;
        gem_pop_frame();
        return _t1444;
    }
#line 361 "compiler/main.gem"
    GemVal _t1445 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1445, gem_string("tag"));
#line 362 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
#line 363 "compiler/main.gem"
        GemVal _t1446 = GEM_NIL;
        gem_pop_frame();
        return _t1446;
    }
#line 366 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 367 "compiler/main.gem"
    GemVal _t1447 = gem_v_node;
    GemVal _t1448[] = {gem_table_get(_t1447, gem_string("params"))};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1448, 1);
#line 368 "compiler/main.gem"
    GemVal _t1449 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t1449, gem_string("rest_param")), GEM_NIL))) {
#line 369 "compiler/main.gem"
    GemVal _t1450 = gem_v_node;
    GemVal _t1451[] = {gem_v_inner_defined, gem_table_get(_t1450, gem_string("rest_param"))};
            (void)(gem_fn_set_add(NULL, _t1451, 2));
        }
#line 371 "compiler/main.gem"
    GemVal _t1452 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t1452, gem_string("block_param")), GEM_NIL))) {
#line 372 "compiler/main.gem"
    GemVal _t1453 = gem_v_node;
    GemVal _t1454[] = {gem_v_inner_defined, gem_table_get(_t1453, gem_string("block_param"))};
            (void)(gem_fn_set_add(NULL, _t1454, 2));
        }
#line 374 "compiler/main.gem"
    GemVal _t1455 = gem_v_node;
    GemVal _t1456[] = {gem_table_get(_t1455, gem_string("body")), gem_v_inner_defined};
    GemVal _t1457 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t1457.fn(_t1457.env, _t1456, 2);
#line 375 "compiler/main.gem"
    GemVal _t1458[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t1458, 2);
#line 376 "compiler/main.gem"
        GemVal gem_v__for_tbl_22 = gem_v_inter;
#line 376 "compiler/main.gem"
    GemVal _t1459[] = {gem_v__for_tbl_22};
        GemVal gem_v__for_keys_22 = gem_keys_fn(NULL, _t1459, 1);
#line 376 "compiler/main.gem"
        GemVal gem_v__for_i_22 = gem_int(0);
#line 376 "compiler/main.gem"
        while (1) {
            GemVal _t1460[] = {gem_v__for_keys_22};
            if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_len_fn(NULL, _t1460, 1)))) break;
#line 376 "compiler/main.gem"
            GemVal gem_v_ik = gem_table_get(gem_v__for_keys_22, gem_v__for_i_22);
#line 376 "compiler/main.gem"
            GemVal gem_v__ = gem_table_get(gem_v__for_tbl_22, gem_table_get(gem_v__for_keys_22, gem_v__for_i_22));
#line 376 "compiler/main.gem"
            gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 377 "compiler/main.gem"
    GemVal _t1461[] = {gem_v_captured, gem_v_ik};
            (void)(gem_fn_set_add(NULL, _t1461, 2));
        }

#line 379 "compiler/main.gem"
    GemVal _t1462 = gem_v_node;
    GemVal _t1463[] = {gem_v_scope_vars, gem_v_inner_defined};
    GemVal _t1464[] = {gem_table_get(_t1462, gem_string("body")), gem_fn_set_union(NULL, _t1463, 2), gem_v_captured};
    GemVal _t1465 = (*gem_v_walk_captures);
        GemVal _t1466 = _t1465.fn(_t1465.env, _t1464, 3);
        gem_pop_frame();
        return _t1466;
    } else {
#line 380 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
            gem_pop_frame();
            return GEM_NIL;
        } else {
#line 382 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 383 "compiler/main.gem"
    GemVal _t1467 = gem_v_node;
    GemVal _t1468[] = {gem_table_get(_t1467, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1469 = (*gem_v_walk_captures_node);
                (void)(_t1469.fn(_t1469.env, _t1468, 3));
#line 384 "compiler/main.gem"
    GemVal _t1470 = gem_v_node;
    GemVal _t1471[] = {gem_table_get(_t1470, gem_string("then")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1472 = (*gem_v_walk_captures);
                (void)(_t1472.fn(_t1472.env, _t1471, 3));
#line 385 "compiler/main.gem"
    GemVal _t1473 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t1473, gem_string("else")), GEM_NIL))) {
#line 386 "compiler/main.gem"
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
#line 388 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 389 "compiler/main.gem"
    GemVal _t1478 = gem_v_node;
    GemVal _t1479[] = {gem_table_get(_t1478, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1480 = (*gem_v_walk_captures_node);
                    (void)(_t1480.fn(_t1480.env, _t1479, 3));
#line 390 "compiler/main.gem"
    GemVal _t1481 = gem_v_node;
    GemVal _t1482[] = {gem_table_get(_t1481, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1483 = (*gem_v_walk_captures);
                    GemVal _t1484 = _t1483.fn(_t1483.env, _t1482, 3);
                    gem_pop_frame();
                    return _t1484;
                } else {
#line 391 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 392 "compiler/main.gem"
    GemVal _t1485 = gem_v_node;
    GemVal _t1486[] = {gem_table_get(_t1485, gem_string("target")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1487 = (*gem_v_walk_captures_node);
                        (void)(_t1487.fn(_t1487.env, _t1486, 3));
#line 393 "compiler/main.gem"
                        GemVal gem_v__for_i_23 = gem_int(0);
#line 393 "compiler/main.gem"
    GemVal _t1488 = gem_v_node;
    GemVal _t1489[] = {gem_table_get(_t1488, gem_string("whens"))};
                        GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t1489, 1);
#line 393 "compiler/main.gem"
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 393 "compiler/main.gem"
                            GemVal gem_v_i = gem_v__for_i_23;
#line 393 "compiler/main.gem"
                            gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 394 "compiler/main.gem"
    GemVal _t1490 = gem_v_node;
    GemVal _t1491 = gem_table_get(gem_table_get(_t1490, gem_string("whens")), gem_v_i);
    GemVal _t1492[] = {gem_table_get(_t1491, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1493 = (*gem_v_walk_captures_node);
                            (void)(_t1493.fn(_t1493.env, _t1492, 3));
#line 395 "compiler/main.gem"
    GemVal _t1494 = gem_v_node;
    GemVal _t1495 = gem_table_get(gem_table_get(_t1494, gem_string("whens")), gem_v_i);
    GemVal _t1496[] = {gem_table_get(_t1495, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1497 = (*gem_v_walk_captures);
                            (void)(_t1497.fn(_t1497.env, _t1496, 3));
                        }

#line 397 "compiler/main.gem"
    GemVal _t1498 = gem_v_node;
                        if (gem_truthy(gem_neq(gem_table_get(_t1498, gem_string("else")), GEM_NIL))) {
#line 398 "compiler/main.gem"
    GemVal _t1499 = gem_v_node;
    GemVal _t1500[] = {gem_table_get(_t1499, gem_string("else")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1501 = (*gem_v_walk_captures);
                            GemVal _t1502 = _t1501.fn(_t1501.env, _t1500, 3);
                            gem_pop_frame();
                            return _t1502;
                        } else {
                            gem_pop_frame();
                            return GEM_NIL;
                        }
                    } else {
#line 400 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 401 "compiler/main.gem"
    GemVal _t1503 = gem_v_node;
    GemVal _t1504[] = {gem_table_get(_t1503, gem_string("stmts")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1505 = (*gem_v_walk_captures);
                            GemVal _t1506 = _t1505.fn(_t1505.env, _t1504, 3);
                            gem_pop_frame();
                            return _t1506;
                        } else {
#line 402 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 403 "compiler/main.gem"
    GemVal _t1507 = gem_v_node;
                                if (gem_truthy(gem_neq(gem_table_get(_t1507, gem_string("value")), GEM_NIL))) {
#line 404 "compiler/main.gem"
    GemVal _t1508 = gem_v_node;
    GemVal _t1509[] = {gem_table_get(_t1508, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1510 = (*gem_v_walk_captures_node);
                                    GemVal _t1511 = _t1510.fn(_t1510.env, _t1509, 3);
                                    gem_pop_frame();
                                    return _t1511;
                                } else {
                                    gem_pop_frame();
                                    return GEM_NIL;
                                }
                            } else {
#line 406 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 407 "compiler/main.gem"
    GemVal _t1512 = gem_v_node;
    GemVal _t1513[] = {gem_table_get(_t1512, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1514 = (*gem_v_walk_captures_node);
                                    GemVal _t1515 = _t1514.fn(_t1514.env, _t1513, 3);
                                    gem_pop_frame();
                                    return _t1515;
                                } else {
#line 408 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 409 "compiler/main.gem"
    GemVal _t1516 = gem_v_node;
    GemVal _t1517[] = {gem_table_get(_t1516, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1518 = (*gem_v_walk_captures_node);
                                        GemVal _t1519 = _t1518.fn(_t1518.env, _t1517, 3);
                                        gem_pop_frame();
                                        return _t1519;
                                    } else {
                                        {
#line 412 "compiler/main.gem"
                                            GemVal gem_v__for_tbl_24 = gem_v_node;
#line 412 "compiler/main.gem"
    GemVal _t1520[] = {gem_v__for_tbl_24};
                                            GemVal gem_v__for_keys_24 = gem_keys_fn(NULL, _t1520, 1);
#line 412 "compiler/main.gem"
                                            GemVal gem_v__for_i_24 = gem_int(0);
#line 412 "compiler/main.gem"
                                            while (1) {
                                                GemVal _t1521[] = {gem_v__for_keys_24};
                                                if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_len_fn(NULL, _t1521, 1)))) break;
#line 412 "compiler/main.gem"
                                                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_24, gem_v__for_i_24);
#line 412 "compiler/main.gem"
                                                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_24, gem_table_get(gem_v__for_keys_24, gem_v__for_i_24));
#line 412 "compiler/main.gem"
                                                gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 413 "compiler/main.gem"
                                                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 414 "compiler/main.gem"
    GemVal _t1522[] = {gem_v_v};
                                                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1522, 1), gem_string("table")))) {
#line 415 "compiler/main.gem"
    GemVal _t1523[] = {gem_v_v};
                                                        GemVal gem_v_vks = gem_keys_fn(NULL, _t1523, 1);
#line 416 "compiler/main.gem"
    GemVal _t1524[] = {gem_v_vks};
    GemVal _t1526;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1524, 1), gem_int(0)))) {
        _t1526 = gem_gt(gem_len_fn(NULL, _t1524, 1), gem_int(0));
    } else {
        GemVal _t1525[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1526 = gem_eq(gem_type_fn(NULL, _t1525, 1), gem_string("int"));
    }
                                                        if (gem_truthy(_t1526)) {
#line 417 "compiler/main.gem"
                                                            GemVal gem_v_vi = gem_int(0);
#line 418 "compiler/main.gem"
                                                            while (1) {
                                                                GemVal _t1527[] = {gem_v_v};
                                                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1527, 1)))) break;
#line 419 "compiler/main.gem"
    GemVal _t1528[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1528, 1), gem_string("table")))) {
#line 420 "compiler/main.gem"
    GemVal _t1529[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t1530 = (*gem_v_walk_captures_node);
                                                                    (void)(_t1530.fn(_t1530.env, _t1529, 3));
                                                                }
#line 422 "compiler/main.gem"
                                                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                            }
                                                        } else {
#line 425 "compiler/main.gem"
    GemVal _t1531[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t1532 = (*gem_v_walk_captures_node);
                                                            (void)(_t1532.fn(_t1532.env, _t1531, 3));
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
#line 434 "compiler/main.gem"
    GemVal _t1534 = gem_table_new();
    GemVal _t1535[] = {gem_v_scope_vars, _t1534};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t1535, 2);
    {
#line 435 "compiler/main.gem"
        GemVal gem_v__for_i_25 = gem_int(0);
#line 435 "compiler/main.gem"
    GemVal _t1536[] = {gem_v_stmts};
        GemVal gem_v__for_limit_25 = gem_len_fn(NULL, _t1536, 1);
#line 435 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 435 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_25;
#line 435 "compiler/main.gem"
            gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 436 "compiler/main.gem"
    GemVal _t1537[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t1538 = (*gem_v_walk_captures_node);
            (void)(_t1538.fn(_t1538.env, _t1537, 3));
#line 437 "compiler/main.gem"
    GemVal _t1539[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1541;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1539, 1), gem_string("table")))) {
        _t1541 = gem_eq(gem_type_fn(NULL, _t1539, 1), gem_string("table"));
    } else {
        GemVal _t1540 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1541 = gem_eq(gem_table_get(_t1540, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1541)) {
#line 438 "compiler/main.gem"
    GemVal _t1542 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1543[] = {gem_v_sv, gem_table_get(_t1542, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1543, 2));
            }
#line 440 "compiler/main.gem"
    GemVal _t1544[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv};
            (void)(gem_fn_collect_block_lets(NULL, _t1544, 2));
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
    GemVal _t1546 = gem_table_new();
    GemVal gem_v_captured = _t1546;
#line 446 "compiler/main.gem"
    GemVal _t1547[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t1548 = (*gem_v_walk_captures);
    (void)(_t1548.fn(_t1548.env, _t1547, 3));
    GemVal _t1549 = gem_v_captured;
    gem_pop_frame();
    return _t1549;
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
#line 473 "compiler/main.gem"
    GemVal _t1551 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1551, gem_string("tag"));
#line 475 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("int")))) {
#line 476 "compiler/main.gem"
    GemVal _t1552 = gem_table_new();
    GemVal _t1553 = gem_v_node;
    GemVal _t1554[] = {gem_table_get(_t1553, gem_string("value"))};
    gem_table_set(_t1552, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t1554, 1)), gem_string(")")));
    gem_table_set(_t1552, gem_string("setup"), gem_string(""));
        GemVal _t1555 = _t1552;
        gem_pop_frame();
        return _t1555;
    } else {
#line 477 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("float")))) {
#line 478 "compiler/main.gem"
    GemVal _t1556 = gem_table_new();
    GemVal _t1557 = gem_v_node;
    GemVal _t1558[] = {gem_table_get(_t1557, gem_string("value"))};
    GemVal _t1559[] = {gem_fn_format_float(NULL, _t1558, 1)};
    gem_table_set(_t1556, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t1559, 1)), gem_string(")")));
    gem_table_set(_t1556, gem_string("setup"), gem_string(""));
            GemVal _t1560 = _t1556;
            gem_pop_frame();
            return _t1560;
        } else {
#line 479 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("string")))) {
#line 480 "compiler/main.gem"
    GemVal _t1561 = gem_v_node;
    GemVal _t1562[] = {gem_table_get(_t1561, gem_string("value"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1562, 1);
#line 481 "compiler/main.gem"
    GemVal _t1563 = gem_table_new();
    GemVal _t1564[] = {gem_v_escaped};
    gem_table_set(_t1563, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t1564, 1)), gem_string("\")")));
    gem_table_set(_t1563, gem_string("setup"), gem_string(""));
                GemVal _t1565 = _t1563;
                gem_pop_frame();
                return _t1565;
            } else {
#line 482 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("interp")))) {
#line 483 "compiler/main.gem"
    GemVal _t1566 = gem_v_node;
                    GemVal gem_v_parts = gem_table_get(_t1566, gem_string("parts"));
#line 484 "compiler/main.gem"
    GemVal _t1567[] = {gem_v_parts};
                    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1567, 1), gem_int(0)))) {
#line 485 "compiler/main.gem"
    GemVal _t1568 = gem_table_new();
    gem_table_set(_t1568, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t1568, gem_string("setup"), gem_string(""));
                        GemVal _t1569 = _t1568;
                        gem_pop_frame();
                        return _t1569;
                    }
#line 487 "compiler/main.gem"
                    GemVal gem_v_setup = gem_string("");
#line 488 "compiler/main.gem"
    GemVal _t1570 = gem_table_new();
                    GemVal gem_v_compiled = _t1570;
#line 489 "compiler/main.gem"
                    GemVal gem_v__for_i_26 = gem_int(0);
#line 489 "compiler/main.gem"
    GemVal _t1571[] = {gem_v_parts};
                    GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1571, 1);
#line 489 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 489 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_26;
#line 489 "compiler/main.gem"
                        gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 490 "compiler/main.gem"
                        GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 491 "compiler/main.gem"
    GemVal _t1572[] = {gem_v_part};
    GemVal _t1573 = (*gem_v_compile_expr);
                        GemVal gem_v_r = _t1573.fn(_t1573.env, _t1572, 1);
#line 492 "compiler/main.gem"
    GemVal _t1574 = gem_v_r;
                        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1574, gem_string("setup")));
#line 493 "compiler/main.gem"
    GemVal _t1575 = gem_v_part;
                        if (gem_truthy(gem_eq(gem_table_get(_t1575, gem_string("tag")), gem_string("string")))) {
#line 494 "compiler/main.gem"
    GemVal _t1576 = gem_v_r;
    GemVal _t1577[] = {gem_v_compiled, gem_table_get(_t1576, gem_string("expr"))};
                            (void)(gem_push_fn(NULL, _t1577, 2));
                        } else {
#line 496 "compiler/main.gem"
    GemVal _t1578 = (*gem_v_tmp);
                            GemVal gem_v_ts = _t1578.fn(_t1578.env, NULL, 0);
#line 497 "compiler/main.gem"
    GemVal _t1579[] = {gem_v_ts};
    GemVal _t1580 = gem_v_r;
    GemVal _t1581[] = {gem_table_get(_t1580, gem_string("expr"))};
                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1579, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1581, 1)), gem_string("};\n")));
#line 498 "compiler/main.gem"
    GemVal _t1582[] = {gem_v_ts};
    GemVal _t1583[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t1582, 1)), gem_string(", 1)"))};
                            (void)(gem_push_fn(NULL, _t1583, 2));
                        }
                    }

#line 501 "compiler/main.gem"
                    GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
#line 502 "compiler/main.gem"
                    GemVal gem_v__for_i_27 = gem_int(1);
#line 502 "compiler/main.gem"
    GemVal _t1584[] = {gem_v_compiled};
                    GemVal gem_v__for_limit_27 = gem_len_fn(NULL, _t1584, 1);
#line 502 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 502 "compiler/main.gem"
                        GemVal gem_v_i = gem_v__for_i_27;
#line 502 "compiler/main.gem"
                        gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 503 "compiler/main.gem"
    GemVal _t1585[] = {gem_v_acc};
    GemVal _t1586[] = {gem_table_get(gem_v_compiled, gem_v_i)};
                        gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1585, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1586, 1)), gem_string(")"));
                    }

#line 505 "compiler/main.gem"
    GemVal _t1587 = gem_table_new();
    gem_table_set(_t1587, gem_string("expr"), gem_v_acc);
    gem_table_set(_t1587, gem_string("setup"), gem_v_setup);
                    GemVal _t1588 = _t1587;
                    gem_pop_frame();
                    return _t1588;
                } else {
#line 506 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("bool")))) {
#line 507 "compiler/main.gem"
    GemVal _t1589 = gem_v_node;
                        if (gem_truthy(gem_table_get(_t1589, gem_string("value")))) {
#line 508 "compiler/main.gem"
    GemVal _t1590 = gem_table_new();
    gem_table_set(_t1590, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t1590, gem_string("setup"), gem_string(""));
                            GemVal _t1591 = _t1590;
                            gem_pop_frame();
                            return _t1591;
                        }
#line 510 "compiler/main.gem"
    GemVal _t1592 = gem_table_new();
    gem_table_set(_t1592, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t1592, gem_string("setup"), gem_string(""));
                        GemVal _t1593 = _t1592;
                        gem_pop_frame();
                        return _t1593;
                    } else {
#line 511 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("nil")))) {
#line 512 "compiler/main.gem"
    GemVal _t1594 = gem_table_new();
    gem_table_set(_t1594, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1594, gem_string("setup"), gem_string(""));
                            GemVal _t1595 = _t1594;
                            gem_pop_frame();
                            return _t1595;
                        } else {
#line 513 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
#line 514 "compiler/main.gem"
    GemVal _t1596 = gem_v_node;
                                GemVal gem_v_name = gem_table_get(_t1596, gem_string("name"));
#line 515 "compiler/main.gem"
    GemVal _t1597[] = {(*gem_v_builtin_fns), gem_v_name};
                                if (gem_truthy(gem_has_key_fn(NULL, _t1597, 2))) {
#line 516 "compiler/main.gem"
    GemVal _t1598 = gem_table_new();
    GemVal _t1599[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name)};
    gem_table_set(_t1598, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1599, 1)), gem_string(", NULL)")));
    gem_table_set(_t1598, gem_string("setup"), gem_string(""));
                                    GemVal _t1600 = _t1598;
                                    gem_pop_frame();
                                    return _t1600;
                                } else {
#line 517 "compiler/main.gem"
    GemVal _t1601[] = {(*gem_v_defined_fns), gem_v_name};
                                    if (gem_truthy(gem_fn_set_contains(NULL, _t1601, 2))) {
#line 518 "compiler/main.gem"
    GemVal _t1602 = gem_table_new();
    GemVal _t1603[] = {gem_v_name};
    gem_table_set(_t1602, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t1603, 1)), gem_string(", NULL)")));
    gem_table_set(_t1602, gem_string("setup"), gem_string(""));
                                        GemVal _t1604 = _t1602;
                                        gem_pop_frame();
                                        return _t1604;
                                    }
                                }
#line 520 "compiler/main.gem"
    GemVal _t1605[] = {gem_v_name};
    GemVal _t1606 = (*gem_v_mangle);
                                GemVal gem_v_mname = _t1606.fn(_t1606.env, _t1605, 1);
#line 521 "compiler/main.gem"
    GemVal _t1607[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t1609;
    if (gem_truthy(gem_fn_set_contains(NULL, _t1607, 2))) {
        _t1609 = gem_fn_set_contains(NULL, _t1607, 2);
    } else {
        GemVal _t1608[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t1609 = gem_fn_set_contains(NULL, _t1608, 2);
    }
                                if (gem_truthy(_t1609)) {
#line 522 "compiler/main.gem"
    GemVal _t1610 = gem_table_new();
    GemVal _t1611[] = {gem_v_mname};
    gem_table_set(_t1610, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t1611, 1)), gem_string(")")));
    gem_table_set(_t1610, gem_string("setup"), gem_string(""));
                                    GemVal _t1612 = _t1610;
                                    gem_pop_frame();
                                    return _t1612;
                                }
#line 524 "compiler/main.gem"
    GemVal _t1613 = gem_table_new();
    gem_table_set(_t1613, gem_string("expr"), gem_v_mname);
    gem_table_set(_t1613, gem_string("setup"), gem_string(""));
                                GemVal _t1614 = _t1613;
                                gem_pop_frame();
                                return _t1614;
                            } else {
#line 525 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 526 "compiler/main.gem"
    GemVal _t1615[] = {gem_v_node};
    GemVal _t1616 = (*gem_v_compile_call);
                                    GemVal _t1617 = _t1616.fn(_t1616.env, _t1615, 1);
                                    gem_pop_frame();
                                    return _t1617;
                                } else {
#line 527 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("binop")))) {
#line 528 "compiler/main.gem"
    GemVal _t1618[] = {gem_v_node};
    GemVal _t1619 = (*gem_v_compile_binop);
                                        GemVal _t1620 = _t1619.fn(_t1619.env, _t1618, 1);
                                        gem_pop_frame();
                                        return _t1620;
                                    } else {
#line 529 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("unop")))) {
#line 530 "compiler/main.gem"
    GemVal _t1621 = gem_v_node;
    GemVal _t1622[] = {gem_table_get(_t1621, gem_string("expr"))};
    GemVal _t1623 = (*gem_v_compile_expr);
                                            GemVal gem_v_r = _t1623.fn(_t1623.env, _t1622, 1);
#line 531 "compiler/main.gem"
    GemVal _t1624 = gem_v_node;
                                            if (gem_truthy(gem_eq(gem_table_get(_t1624, gem_string("op")), gem_string("-")))) {
#line 532 "compiler/main.gem"
    GemVal _t1625 = gem_table_new();
    GemVal _t1626 = gem_v_r;
    GemVal _t1627[] = {gem_table_get(_t1626, gem_string("expr"))};
    gem_table_set(_t1625, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t1627, 1)), gem_string(")")));
    GemVal _t1628 = gem_v_r;
    gem_table_set(_t1625, gem_string("setup"), gem_table_get(_t1628, gem_string("setup")));
                                                GemVal _t1629 = _t1625;
                                                gem_pop_frame();
                                                return _t1629;
                                            } else {
#line 533 "compiler/main.gem"
    GemVal _t1630 = gem_v_node;
                                                if (gem_truthy(gem_eq(gem_table_get(_t1630, gem_string("op")), gem_string("not")))) {
#line 534 "compiler/main.gem"
    GemVal _t1631 = gem_table_new();
    GemVal _t1632 = gem_v_r;
    GemVal _t1633[] = {gem_table_get(_t1632, gem_string("expr"))};
    gem_table_set(_t1631, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t1633, 1)), gem_string(")")));
    GemVal _t1634 = gem_v_r;
    gem_table_set(_t1631, gem_string("setup"), gem_table_get(_t1634, gem_string("setup")));
                                                    GemVal _t1635 = _t1631;
                                                    gem_pop_frame();
                                                    return _t1635;
                                                }
                                            }
#line 536 "compiler/main.gem"
    GemVal _t1636 = gem_v_node;
    GemVal _t1637[] = {gem_table_get(_t1636, gem_string("op"))};
    GemVal _t1638[] = {gem_add(gem_string("unknown unary operator: "), gem_to_string_fn(NULL, _t1637, 1))};
                                            (void)(gem_error_at_fn("compiler/main.gem", 536, _t1638, 1));
                                        } else {
#line 537 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
#line 538 "compiler/main.gem"
    GemVal _t1639[] = {gem_v_node};
    GemVal _t1640 = (*gem_v_compile_anon_fn);
                                                GemVal _t1641 = _t1640.fn(_t1640.env, _t1639, 1);
                                                gem_pop_frame();
                                                return _t1641;
                                            } else {
#line 539 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("table")))) {
#line 540 "compiler/main.gem"
    GemVal _t1642[] = {gem_v_node};
    GemVal _t1643 = (*gem_v_compile_table);
                                                    GemVal _t1644 = _t1643.fn(_t1643.env, _t1642, 1);
                                                    gem_pop_frame();
                                                    return _t1644;
                                                } else {
#line 541 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("array")))) {
#line 542 "compiler/main.gem"
    GemVal _t1645[] = {gem_v_node};
    GemVal _t1646 = (*gem_v_compile_array);
                                                        GemVal _t1647 = _t1646.fn(_t1646.env, _t1645, 1);
                                                        gem_pop_frame();
                                                        return _t1647;
                                                    } else {
#line 543 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot")))) {
#line 544 "compiler/main.gem"
    GemVal _t1648 = gem_v_node;
    GemVal _t1649[] = {gem_table_get(_t1648, gem_string("object"))};
    GemVal _t1650 = (*gem_v_compile_expr);
                                                            GemVal gem_v_r = _t1650.fn(_t1650.env, _t1649, 1);
#line 545 "compiler/main.gem"
    GemVal _t1651 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1651.fn(_t1651.env, NULL, 0);
#line 546 "compiler/main.gem"
    GemVal _t1652 = gem_v_r;
    GemVal _t1653[] = {gem_v_t};
    GemVal _t1654 = gem_v_r;
    GemVal _t1655[] = {gem_table_get(_t1654, gem_string("expr"))};
                                                            GemVal gem_v_setup = gem_add(gem_table_get(_t1652, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1653, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1655, 1)), gem_string(";\n")));
#line 547 "compiler/main.gem"
    GemVal _t1656 = gem_v_node;
    GemVal _t1657[] = {gem_table_get(_t1656, gem_string("field"))};
                                                            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1657, 1);
#line 548 "compiler/main.gem"
    GemVal _t1658 = gem_table_new();
    GemVal _t1659[] = {gem_v_t};
    GemVal _t1660[] = {gem_v_escaped};
    gem_table_set(_t1658, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1659, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1660, 1)), gem_string("\"))")));
    gem_table_set(_t1658, gem_string("setup"), gem_v_setup);
                                                            GemVal _t1661 = _t1658;
                                                            gem_pop_frame();
                                                            return _t1661;
                                                        } else {
#line 549 "compiler/main.gem"
                                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("index")))) {
#line 550 "compiler/main.gem"
    GemVal _t1662 = gem_v_node;
    GemVal _t1663[] = {gem_table_get(_t1662, gem_string("object"))};
    GemVal _t1664 = (*gem_v_compile_expr);
                                                                GemVal gem_v_obj_r = _t1664.fn(_t1664.env, _t1663, 1);
#line 551 "compiler/main.gem"
    GemVal _t1665 = gem_v_node;
    GemVal _t1666[] = {gem_table_get(_t1665, gem_string("key"))};
    GemVal _t1667 = (*gem_v_compile_expr);
                                                                GemVal gem_v_key_r = _t1667.fn(_t1667.env, _t1666, 1);
#line 552 "compiler/main.gem"
    GemVal _t1668 = gem_table_new();
    GemVal _t1669 = gem_v_obj_r;
    GemVal _t1670[] = {gem_table_get(_t1669, gem_string("expr"))};
    GemVal _t1671 = gem_v_key_r;
    GemVal _t1672[] = {gem_table_get(_t1671, gem_string("expr"))};
    gem_table_set(_t1668, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1670, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1672, 1)), gem_string(")")));
    GemVal _t1673 = gem_v_obj_r;
    GemVal _t1674 = gem_v_key_r;
    gem_table_set(_t1668, gem_string("setup"), gem_add(gem_table_get(_t1673, gem_string("setup")), gem_table_get(_t1674, gem_string("setup"))));
                                                                GemVal _t1675 = _t1668;
                                                                gem_pop_frame();
                                                                return _t1675;
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
#line 555 "compiler/main.gem"
    GemVal _t1676[] = {gem_v_tag};
    GemVal _t1677[] = {gem_add(gem_string("unknown expression node: "), gem_to_string_fn(NULL, _t1676, 1))};
    GemVal _t1678 = gem_error_at_fn("compiler/main.gem", 555, _t1677, 1);
    gem_pop_frame();
    return _t1678;
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
#line 561 "compiler/main.gem"
    GemVal _t1680 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t1680, gem_string("entries"));
#line 562 "compiler/main.gem"
    GemVal _t1681 = (*gem_v_tmp);
    GemVal gem_v_t = _t1681.fn(_t1681.env, NULL, 0);
#line 563 "compiler/main.gem"
    GemVal _t1682[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1682, 1)), gem_string(" = gem_table_new();\n"));
#line 564 "compiler/main.gem"
    GemVal gem_v__for_items_28 = gem_v_entries;
#line 564 "compiler/main.gem"
    GemVal gem_v__for_i_28 = gem_int(0);
#line 564 "compiler/main.gem"
    while (1) {
        GemVal _t1683[] = {gem_v__for_items_28};
        if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_len_fn(NULL, _t1683, 1)))) break;
#line 564 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_28, gem_v__for_i_28);
#line 564 "compiler/main.gem"
        gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 565 "compiler/main.gem"
    GemVal _t1684 = gem_v_entry;
    GemVal _t1685[] = {gem_table_get(_t1684, gem_string("value"))};
    GemVal _t1686 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t1686.fn(_t1686.env, _t1685, 1);
#line 566 "compiler/main.gem"
    GemVal _t1687 = gem_v_entry;
    GemVal _t1688[] = {gem_table_get(_t1687, gem_string("key"))};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1688, 1);
#line 567 "compiler/main.gem"
    GemVal _t1689 = gem_v_val_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1689, gem_string("setup")));
#line 568 "compiler/main.gem"
    GemVal _t1690[] = {gem_v_t};
    GemVal _t1691[] = {gem_v_escaped};
    GemVal _t1692 = gem_v_val_r;
    GemVal _t1693[] = {gem_table_get(_t1692, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1690, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1691, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1693, 1)), gem_string(");\n")));
    }

    GemVal _t1694 = gem_table_new();
    gem_table_set(_t1694, gem_string("expr"), gem_v_t);
    gem_table_set(_t1694, gem_string("setup"), gem_v_setup);
    GemVal _t1695 = _t1694;
    gem_pop_frame();
    return _t1695;
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
#line 576 "compiler/main.gem"
    GemVal _t1697 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t1697, gem_string("elements"));
#line 577 "compiler/main.gem"
    GemVal _t1698 = (*gem_v_tmp);
    GemVal gem_v_t = _t1698.fn(_t1698.env, NULL, 0);
#line 578 "compiler/main.gem"
    GemVal _t1699[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1699, 1)), gem_string(" = gem_table_new();\n"));
#line 579 "compiler/main.gem"
    GemVal gem_v__for_i_29 = gem_int(0);
#line 579 "compiler/main.gem"
    GemVal _t1700[] = {gem_v_elements};
    GemVal gem_v__for_limit_29 = gem_len_fn(NULL, _t1700, 1);
#line 579 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_v__for_limit_29))) break;
#line 579 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_29;
#line 579 "compiler/main.gem"
        gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 580 "compiler/main.gem"
    GemVal _t1701[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t1702 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t1702.fn(_t1702.env, _t1701, 1);
#line 581 "compiler/main.gem"
    GemVal _t1703 = gem_v_elem_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1703, gem_string("setup")));
#line 582 "compiler/main.gem"
    GemVal _t1704[] = {gem_v_t};
    GemVal _t1705[] = {gem_v_i};
    GemVal _t1706 = gem_v_elem_r;
    GemVal _t1707[] = {gem_table_get(_t1706, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1704, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t1705, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t1707, 1)), gem_string(");\n")));
    }

    GemVal _t1708 = gem_table_new();
    gem_table_set(_t1708, gem_string("expr"), gem_v_t);
    gem_table_set(_t1708, gem_string("setup"), gem_v_setup);
    GemVal _t1709 = _t1708;
    gem_pop_frame();
    return _t1709;
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
#line 590 "compiler/main.gem"
    GemVal _t1711 = gem_v_node;
    GemVal _t1712[] = {gem_table_get(_t1711, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1712, 1);
#line 591 "compiler/main.gem"
    GemVal _t1713 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1713, gem_string("rest_param")), GEM_NIL))) {
#line 592 "compiler/main.gem"
    GemVal _t1714 = gem_v_node;
    GemVal _t1715[] = {gem_v_inner_defined, gem_table_get(_t1714, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t1715, 2));
    }
#line 594 "compiler/main.gem"
    GemVal _t1716 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t1716, gem_string("block_param")), GEM_NIL))) {
#line 595 "compiler/main.gem"
    GemVal _t1717 = gem_v_node;
    GemVal _t1718[] = {gem_v_inner_defined, gem_table_get(_t1717, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t1718, 2));
    }
#line 597 "compiler/main.gem"
    GemVal _t1719 = gem_v_node;
    GemVal _t1720[] = {gem_table_get(_t1719, gem_string("body")), gem_v_inner_defined};
    GemVal _t1721 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t1721.fn(_t1721.env, _t1720, 2);
#line 598 "compiler/main.gem"
    GemVal _t1722[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t1722, 1);
#line 600 "compiler/main.gem"
    GemVal _t1723[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1723, 1), gem_int(0)))) {
#line 601 "compiler/main.gem"
    GemVal _t1724 = gem_table_new();
    GemVal _t1725[] = {gem_v_node, _t1724};
    GemVal _t1726 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t1726.fn(_t1726.env, _t1725, 2);
#line 602 "compiler/main.gem"
    GemVal _t1727 = gem_table_new();
    GemVal _t1728 = gem_v_result;
    GemVal _t1729[] = {gem_table_get(_t1728, gem_string("fn_name"))};
    gem_table_set(_t1727, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1729, 1)), gem_string(", NULL)")));
    gem_table_set(_t1727, gem_string("setup"), gem_string(""));
        GemVal _t1730 = _t1727;
        gem_pop_frame();
        return _t1730;
    }
#line 605 "compiler/main.gem"
    GemVal _t1731[] = {gem_v_node, gem_v_captures};
    GemVal _t1732 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t1732.fn(_t1732.env, _t1731, 2);
#line 607 "compiler/main.gem"
    GemVal gem_v_setup = gem_string("");
#line 608 "compiler/main.gem"
    GemVal _t1733 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t1733.fn(_t1733.env, NULL, 0);
#line 609 "compiler/main.gem"
    GemVal _t1734 = gem_v_result;
    GemVal _t1735[] = {gem_table_get(_t1734, gem_string("struct_name"))};
    GemVal _t1736[] = {gem_v_env_tmp};
    GemVal _t1737 = gem_v_result;
    GemVal _t1738[] = {gem_table_get(_t1737, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t1735, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t1736, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t1738, 1)), gem_string("));\n")));
#line 610 "compiler/main.gem"
    GemVal gem_v__for_items_30 = gem_v_captures;
#line 610 "compiler/main.gem"
    GemVal gem_v__for_i_30 = gem_int(0);
#line 610 "compiler/main.gem"
    while (1) {
        GemVal _t1739[] = {gem_v__for_items_30};
        if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_len_fn(NULL, _t1739, 1)))) break;
#line 610 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_30, gem_v__for_i_30);
#line 610 "compiler/main.gem"
        gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 611 "compiler/main.gem"
    GemVal _t1740[] = {gem_v_cap};
    GemVal _t1741 = (*gem_v_mangle);
        GemVal gem_v_mc = _t1741.fn(_t1741.env, _t1740, 1);
#line 612 "compiler/main.gem"
    GemVal _t1742[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1742, 2))) {
#line 613 "compiler/main.gem"
    GemVal _t1743[] = {gem_v_env_tmp};
    GemVal _t1744[] = {gem_v_mc};
    GemVal _t1745[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1743, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1744, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1745, 1)), gem_string(";\n")));
        } else {
#line 615 "compiler/main.gem"
    GemVal _t1746[] = {gem_v_env_tmp};
    GemVal _t1747[] = {gem_v_mc};
    GemVal _t1748[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1746, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1747, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t1748, 1)), gem_string(";\n")));
        }
    }

    GemVal _t1749 = gem_table_new();
    GemVal _t1750 = gem_v_result;
    GemVal _t1751[] = {gem_table_get(_t1750, gem_string("fn_name"))};
    GemVal _t1752[] = {gem_v_env_tmp};
    gem_table_set(_t1749, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1751, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1752, 1)), gem_string(")")));
    gem_table_set(_t1749, gem_string("setup"), gem_v_setup);
    GemVal _t1753 = _t1749;
    gem_pop_frame();
    return _t1753;
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
#line 625 "compiler/main.gem"
    GemVal gem_v_arg_setups = gem_string("");
#line 626 "compiler/main.gem"
    GemVal _t1755 = gem_table_new();
    GemVal gem_v_arg_exprs = _t1755;
#line 627 "compiler/main.gem"
    GemVal gem_v__for_i_31 = gem_int(0);
#line 627 "compiler/main.gem"
    GemVal _t1756[] = {gem_v_args};
    GemVal gem_v__for_limit_31 = gem_len_fn(NULL, _t1756, 1);
#line 627 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_v__for_limit_31))) break;
#line 627 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_31;
#line 627 "compiler/main.gem"
        gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
#line 628 "compiler/main.gem"
    GemVal _t1757[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t1758 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t1758.fn(_t1758.env, _t1757, 1);
#line 629 "compiler/main.gem"
    GemVal _t1759 = gem_v_r;
        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1759, gem_string("setup")));
#line 630 "compiler/main.gem"
    GemVal _t1760 = gem_v_r;
    GemVal _t1761[] = {gem_v_arg_exprs, gem_table_get(_t1760, gem_string("expr"))};
        (void)(gem_push_fn(NULL, _t1761, 2));
    }

#line 632 "compiler/main.gem"
    GemVal _t1762[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t1762, 1);
#line 633 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 634 "compiler/main.gem"
    GemVal _t1763 = gem_table_new();
    gem_table_set(_t1763, gem_string("setup"), gem_v_arg_setups);
    gem_table_set(_t1763, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t1763, gem_string("argc"), gem_int(0));
        GemVal _t1764 = _t1763;
        gem_pop_frame();
        return _t1764;
    }
#line 636 "compiler/main.gem"
    GemVal _t1765 = (*gem_v_tmp);
    GemVal gem_v_t = _t1765.fn(_t1765.env, NULL, 0);
#line 637 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 638 "compiler/main.gem"
    GemVal gem_v__for_i_32 = gem_int(1);
#line 638 "compiler/main.gem"
    GemVal gem_v__for_limit_32 = gem_v_argc;
#line 638 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_v__for_limit_32))) break;
#line 638 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_32;
#line 638 "compiler/main.gem"
        gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 639 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t1766 = gem_table_new();
    GemVal _t1767[] = {gem_v_t};
    GemVal _t1768[] = {gem_v_arr};
    gem_table_set(_t1766, gem_string("setup"), gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1767, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1768, 1)), gem_string("};\n"))));
    gem_table_set(_t1766, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t1766, gem_string("argc"), gem_v_argc);
    GemVal _t1769 = _t1766;
    gem_pop_frame();
    return _t1769;
}

struct _closure__anon_35 {
};
static GemVal _anon_35(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_35", "compiler/main.gem", 0);
#line 646 "compiler/main.gem"
    GemVal _t1771 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t1771, gem_string("argc")), gem_int(0)))) {
#line 647 "compiler/main.gem"
    GemVal _t1772 = gem_table_new();
    GemVal _t1773[] = {gem_v_fn_name};
    GemVal _t1774[] = {gem_v_env};
    gem_table_set(_t1772, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1773, 1), gem_string("(")), gem_to_string_fn(NULL, _t1774, 1)), gem_string(", NULL, 0)")));
    GemVal _t1775 = gem_v_ca;
    gem_table_set(_t1772, gem_string("setup"), gem_table_get(_t1775, gem_string("setup")));
        GemVal _t1776 = _t1772;
        gem_pop_frame();
        return _t1776;
    }
    GemVal _t1777 = gem_table_new();
    GemVal _t1778[] = {gem_v_fn_name};
    GemVal _t1779[] = {gem_v_env};
    GemVal _t1780 = gem_v_ca;
    GemVal _t1781[] = {gem_table_get(_t1780, gem_string("arr_name"))};
    GemVal _t1782 = gem_v_ca;
    GemVal _t1783[] = {gem_table_get(_t1782, gem_string("argc"))};
    gem_table_set(_t1777, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1778, 1), gem_string("(")), gem_to_string_fn(NULL, _t1779, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1781, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1783, 1)), gem_string(")")));
    GemVal _t1784 = gem_v_ca;
    gem_table_set(_t1777, gem_string("setup"), gem_table_get(_t1784, gem_string("setup")));
    GemVal _t1785 = _t1777;
    gem_pop_frame();
    return _t1785;
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
#line 655 "compiler/main.gem"
    GemVal gem_v__d33 = gem_v_node;
#line 655 "compiler/main.gem"
    GemVal _t1786 = gem_v__d33;
    GemVal gem_v_func = gem_table_get(_t1786, gem_string("func"));
#line 655 "compiler/main.gem"
    GemVal _t1787 = gem_v__d33;
    GemVal gem_v_args = gem_table_get(_t1787, gem_string("args"));

#line 658 "compiler/main.gem"
    GemVal _t1788 = gem_v_func;
    GemVal _t1790;
    if (!gem_truthy(gem_eq(gem_table_get(_t1788, gem_string("tag")), gem_string("dot")))) {
        _t1790 = gem_eq(gem_table_get(_t1788, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t1789 = gem_v_func;
        _t1790 = gem_eq(gem_table_get(_t1789, gem_string("field")), gem_string("len"));
    }
    GemVal _t1792;
    if (!gem_truthy(_t1790)) {
        _t1792 = _t1790;
    } else {
        GemVal _t1791[] = {gem_v_args};
        _t1792 = gem_eq(gem_len_fn(NULL, _t1791, 1), gem_int(0));
    }
    if (gem_truthy(_t1792)) {
#line 659 "compiler/main.gem"
    GemVal _t1793 = gem_v_func;
    GemVal _t1794[] = {gem_table_get(_t1793, gem_string("object"))};
    GemVal _t1795 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t1795.fn(_t1795.env, _t1794, 1);
#line 660 "compiler/main.gem"
    GemVal _t1796 = gem_table_new();
    GemVal _t1797 = gem_v_obj_r;
    GemVal _t1798[] = {gem_table_get(_t1797, gem_string("expr"))};
    gem_table_set(_t1796, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t1798, 1)), gem_string(")")));
    GemVal _t1799 = gem_v_obj_r;
    gem_table_set(_t1796, gem_string("setup"), gem_table_get(_t1799, gem_string("setup")));
        GemVal _t1800 = _t1796;
        gem_pop_frame();
        return _t1800;
    }
#line 664 "compiler/main.gem"
    GemVal _t1801 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t1801, gem_string("tag")), gem_string("var")))) {
#line 665 "compiler/main.gem"
    GemVal _t1802 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t1802, gem_string("name"));
#line 667 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 668 "compiler/main.gem"
    GemVal _t1803 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t1803, gem_string("line"));
#line 669 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 670 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 672 "compiler/main.gem"
    GemVal _t1804[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t1804, 1);
#line 673 "compiler/main.gem"
    GemVal _t1805[] = {gem_v_args};
    GemVal _t1806 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t1806.fn(_t1806.env, _t1805, 1);
#line 674 "compiler/main.gem"
    GemVal _t1807 = gem_v_ca;
            if (gem_truthy(gem_eq(gem_table_get(_t1807, gem_string("argc")), gem_int(0)))) {
#line 675 "compiler/main.gem"
    GemVal _t1808 = gem_table_new();
    GemVal _t1809[] = {gem_v_escaped_file};
    GemVal _t1810[] = {gem_v_line};
    gem_table_set(_t1808, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1809, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1810, 1)), gem_string(", NULL, 0)")));
    GemVal _t1811 = gem_v_ca;
    gem_table_set(_t1808, gem_string("setup"), gem_table_get(_t1811, gem_string("setup")));
                GemVal _t1812 = _t1808;
                gem_pop_frame();
                return _t1812;
            }
#line 677 "compiler/main.gem"
    GemVal _t1813 = gem_table_new();
    GemVal _t1814[] = {gem_v_escaped_file};
    GemVal _t1815[] = {gem_v_line};
    GemVal _t1816 = gem_v_ca;
    GemVal _t1817[] = {gem_table_get(_t1816, gem_string("arr_name"))};
    GemVal _t1818 = gem_v_ca;
    GemVal _t1819[] = {gem_table_get(_t1818, gem_string("argc"))};
    gem_table_set(_t1813, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1814, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1815, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1817, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1819, 1)), gem_string(")")));
    GemVal _t1820 = gem_v_ca;
    gem_table_set(_t1813, gem_string("setup"), gem_table_get(_t1820, gem_string("setup")));
            GemVal _t1821 = _t1813;
            gem_pop_frame();
            return _t1821;
        } else {
#line 678 "compiler/main.gem"
    GemVal _t1822[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t1822, 2))) {
#line 679 "compiler/main.gem"
    GemVal _t1823[] = {gem_v_args};
    GemVal _t1824 = (*gem_v_compile_args);
    GemVal _t1825[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t1824.fn(_t1824.env, _t1823, 1)};
    GemVal _t1826 = (*gem_v_emit_direct_call);
                GemVal _t1827 = _t1826.fn(_t1826.env, _t1825, 3);
                gem_pop_frame();
                return _t1827;
            } else {
#line 680 "compiler/main.gem"
    GemVal _t1828[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t1828, 2))) {
#line 681 "compiler/main.gem"
    GemVal _t1829[] = {gem_v_name};
    GemVal _t1830[] = {gem_v_args};
    GemVal _t1831 = (*gem_v_compile_args);
    GemVal _t1832[] = {gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1829, 1)), gem_string("NULL"), _t1831.fn(_t1831.env, _t1830, 1)};
    GemVal _t1833 = (*gem_v_emit_direct_call);
                    GemVal _t1834 = _t1833.fn(_t1833.env, _t1832, 3);
                    gem_pop_frame();
                    return _t1834;
                }
            }
        }
    }
#line 686 "compiler/main.gem"
    GemVal _t1835[] = {gem_v_func};
    GemVal _t1836 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t1836.fn(_t1836.env, _t1835, 1);
#line 687 "compiler/main.gem"
    GemVal _t1837[] = {gem_v_args};
    GemVal _t1838 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t1838.fn(_t1838.env, _t1837, 1);
#line 688 "compiler/main.gem"
    GemVal _t1839 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t1839.fn(_t1839.env, NULL, 0);
#line 689 "compiler/main.gem"
    GemVal _t1840 = gem_v_func_r;
    GemVal _t1841 = gem_v_ca;
    GemVal _t1842[] = {gem_v_tmp_fn};
    GemVal _t1843 = gem_v_func_r;
    GemVal _t1844[] = {gem_table_get(_t1843, gem_string("expr"))};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get(_t1840, gem_string("setup")), gem_table_get(_t1841, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1842, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1844, 1)), gem_string(";\n")));
#line 690 "compiler/main.gem"
    GemVal _t1845 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t1845, gem_string("argc")), gem_int(0)))) {
#line 691 "compiler/main.gem"
    GemVal _t1846 = gem_table_new();
    GemVal _t1847[] = {gem_v_tmp_fn};
    GemVal _t1848[] = {gem_v_tmp_fn};
    gem_table_set(_t1846, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1847, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1848, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t1846, gem_string("setup"), gem_v_setup);
        GemVal _t1849 = _t1846;
        gem_pop_frame();
        return _t1849;
    }
    GemVal _t1850 = gem_table_new();
    GemVal _t1851[] = {gem_v_tmp_fn};
    GemVal _t1852[] = {gem_v_tmp_fn};
    GemVal _t1853 = gem_v_ca;
    GemVal _t1854[] = {gem_table_get(_t1853, gem_string("arr_name"))};
    GemVal _t1855 = gem_v_ca;
    GemVal _t1856[] = {gem_table_get(_t1855, gem_string("argc"))};
    gem_table_set(_t1850, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1851, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1852, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t1854, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1856, 1)), gem_string(")")));
    gem_table_set(_t1850, gem_string("setup"), gem_v_setup);
    GemVal _t1857 = _t1850;
    gem_pop_frame();
    return _t1857;
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
#line 699 "compiler/main.gem"
    GemVal _t1859 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t1859, gem_string("op"));
#line 700 "compiler/main.gem"
    GemVal _t1860 = gem_v_node;
    GemVal _t1861[] = {gem_table_get(_t1860, gem_string("left"))};
    GemVal _t1862 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t1862.fn(_t1862.env, _t1861, 1);
#line 701 "compiler/main.gem"
    GemVal _t1863 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t1863, gem_string("expr"));
#line 702 "compiler/main.gem"
    GemVal _t1864 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t1864, gem_string("setup"));
#line 704 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 705 "compiler/main.gem"
    GemVal _t1865 = gem_v_node;
    GemVal _t1866[] = {gem_table_get(_t1865, gem_string("right"))};
    GemVal _t1867 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t1867.fn(_t1867.env, _t1866, 1);
#line 706 "compiler/main.gem"
    GemVal _t1868 = gem_table_new();
    GemVal _t1869[] = {gem_v_lc};
    GemVal _t1870 = gem_v_rc_r;
    GemVal _t1871[] = {gem_table_get(_t1870, gem_string("expr"))};
    gem_table_set(_t1868, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1869, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1871, 1)), gem_string(")")));
    GemVal _t1872 = gem_v_rc_r;
    gem_table_set(_t1868, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1872, gem_string("setup"))));
        GemVal _t1873 = _t1868;
        gem_pop_frame();
        return _t1873;
    } else {
#line 707 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 708 "compiler/main.gem"
    GemVal _t1874 = gem_v_node;
    GemVal _t1875[] = {gem_table_get(_t1874, gem_string("right"))};
    GemVal _t1876 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t1876.fn(_t1876.env, _t1875, 1);
#line 709 "compiler/main.gem"
    GemVal _t1877 = gem_table_new();
    GemVal _t1878[] = {gem_v_lc};
    GemVal _t1879 = gem_v_rc_r;
    GemVal _t1880[] = {gem_table_get(_t1879, gem_string("expr"))};
    gem_table_set(_t1877, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t1878, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1880, 1)), gem_string(")")));
    GemVal _t1881 = gem_v_rc_r;
    gem_table_set(_t1877, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1881, gem_string("setup"))));
            GemVal _t1882 = _t1877;
            gem_pop_frame();
            return _t1882;
        } else {
#line 710 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 711 "compiler/main.gem"
    GemVal _t1883 = gem_v_node;
    GemVal _t1884[] = {gem_table_get(_t1883, gem_string("right"))};
    GemVal _t1885 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t1885.fn(_t1885.env, _t1884, 1);
#line 712 "compiler/main.gem"
    GemVal _t1886 = gem_table_new();
    GemVal _t1887[] = {gem_v_lc};
    GemVal _t1888 = gem_v_rc_r;
    GemVal _t1889[] = {gem_table_get(_t1888, gem_string("expr"))};
    gem_table_set(_t1886, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t1887, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1889, 1)), gem_string(")")));
    GemVal _t1890 = gem_v_rc_r;
    gem_table_set(_t1886, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1890, gem_string("setup"))));
                GemVal _t1891 = _t1886;
                gem_pop_frame();
                return _t1891;
            } else {
#line 713 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 714 "compiler/main.gem"
    GemVal _t1892 = gem_v_node;
    GemVal _t1893[] = {gem_table_get(_t1892, gem_string("right"))};
    GemVal _t1894 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t1894.fn(_t1894.env, _t1893, 1);
#line 715 "compiler/main.gem"
    GemVal _t1895 = gem_table_new();
    GemVal _t1896[] = {gem_v_lc};
    GemVal _t1897 = gem_v_rc_r;
    GemVal _t1898[] = {gem_table_get(_t1897, gem_string("expr"))};
    gem_table_set(_t1895, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t1896, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1898, 1)), gem_string(")")));
    GemVal _t1899 = gem_v_rc_r;
    gem_table_set(_t1895, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1899, gem_string("setup"))));
                    GemVal _t1900 = _t1895;
                    gem_pop_frame();
                    return _t1900;
                } else {
#line 716 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 717 "compiler/main.gem"
    GemVal _t1901 = gem_v_node;
    GemVal _t1902[] = {gem_table_get(_t1901, gem_string("right"))};
    GemVal _t1903 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t1903.fn(_t1903.env, _t1902, 1);
#line 718 "compiler/main.gem"
    GemVal _t1904 = gem_table_new();
    GemVal _t1905[] = {gem_v_lc};
    GemVal _t1906 = gem_v_rc_r;
    GemVal _t1907[] = {gem_table_get(_t1906, gem_string("expr"))};
    gem_table_set(_t1904, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t1905, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1907, 1)), gem_string(")")));
    GemVal _t1908 = gem_v_rc_r;
    gem_table_set(_t1904, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1908, gem_string("setup"))));
                        GemVal _t1909 = _t1904;
                        gem_pop_frame();
                        return _t1909;
                    } else {
#line 719 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 720 "compiler/main.gem"
    GemVal _t1910 = gem_v_node;
    GemVal _t1911[] = {gem_table_get(_t1910, gem_string("right"))};
    GemVal _t1912 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t1912.fn(_t1912.env, _t1911, 1);
#line 721 "compiler/main.gem"
    GemVal _t1913 = (*gem_v_tmp);
                            GemVal gem_v_t = _t1913.fn(_t1913.env, NULL, 0);
#line 722 "compiler/main.gem"
    GemVal _t1914 = gem_v_rc_r;
    GemVal _t1915[] = {gem_v_t};
    GemVal _t1916 = gem_v_rc_r;
    GemVal _t1917[] = {gem_table_get(_t1916, gem_string("expr"))};
    GemVal _t1918[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t1914, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1915, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1917, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1918, 1)), gem_string("};\n")));
#line 723 "compiler/main.gem"
    GemVal _t1919 = gem_table_new();
    GemVal _t1920[] = {gem_v_t};
    gem_table_set(_t1919, gem_string("expr"), gem_add(gem_add(gem_string("gem_has_key_fn(NULL, "), gem_to_string_fn(NULL, _t1920, 1)), gem_string(", 2)")));
    gem_table_set(_t1919, gem_string("setup"), gem_v_setup);
                            GemVal _t1921 = _t1919;
                            gem_pop_frame();
                            return _t1921;
                        } else {
#line 724 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 725 "compiler/main.gem"
    GemVal _t1922 = gem_v_node;
    GemVal _t1923[] = {gem_table_get(_t1922, gem_string("right"))};
    GemVal _t1924 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t1924.fn(_t1924.env, _t1923, 1);
#line 726 "compiler/main.gem"
    GemVal _t1925 = gem_table_new();
    GemVal _t1926[] = {gem_v_lc};
    GemVal _t1927 = gem_v_rc_r;
    GemVal _t1928[] = {gem_table_get(_t1927, gem_string("expr"))};
    gem_table_set(_t1925, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t1926, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1928, 1)), gem_string(")")));
    GemVal _t1929 = gem_v_rc_r;
    gem_table_set(_t1925, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1929, gem_string("setup"))));
                                GemVal _t1930 = _t1925;
                                gem_pop_frame();
                                return _t1930;
                            } else {
#line 727 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 728 "compiler/main.gem"
    GemVal _t1931 = gem_v_node;
    GemVal _t1932[] = {gem_table_get(_t1931, gem_string("right"))};
    GemVal _t1933 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t1933.fn(_t1933.env, _t1932, 1);
#line 729 "compiler/main.gem"
    GemVal _t1934 = gem_table_new();
    GemVal _t1935[] = {gem_v_lc};
    GemVal _t1936 = gem_v_rc_r;
    GemVal _t1937[] = {gem_table_get(_t1936, gem_string("expr"))};
    gem_table_set(_t1934, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t1935, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1937, 1)), gem_string(")")));
    GemVal _t1938 = gem_v_rc_r;
    gem_table_set(_t1934, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1938, gem_string("setup"))));
                                    GemVal _t1939 = _t1934;
                                    gem_pop_frame();
                                    return _t1939;
                                } else {
#line 730 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 731 "compiler/main.gem"
    GemVal _t1940 = gem_v_node;
    GemVal _t1941[] = {gem_table_get(_t1940, gem_string("right"))};
    GemVal _t1942 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t1942.fn(_t1942.env, _t1941, 1);
#line 732 "compiler/main.gem"
    GemVal _t1943 = gem_table_new();
    GemVal _t1944[] = {gem_v_lc};
    GemVal _t1945 = gem_v_rc_r;
    GemVal _t1946[] = {gem_table_get(_t1945, gem_string("expr"))};
    gem_table_set(_t1943, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t1944, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1946, 1)), gem_string(")")));
    GemVal _t1947 = gem_v_rc_r;
    gem_table_set(_t1943, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1947, gem_string("setup"))));
                                        GemVal _t1948 = _t1943;
                                        gem_pop_frame();
                                        return _t1948;
                                    } else {
#line 733 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 734 "compiler/main.gem"
    GemVal _t1949 = gem_v_node;
    GemVal _t1950[] = {gem_table_get(_t1949, gem_string("right"))};
    GemVal _t1951 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t1951.fn(_t1951.env, _t1950, 1);
#line 735 "compiler/main.gem"
    GemVal _t1952 = gem_table_new();
    GemVal _t1953[] = {gem_v_lc};
    GemVal _t1954 = gem_v_rc_r;
    GemVal _t1955[] = {gem_table_get(_t1954, gem_string("expr"))};
    gem_table_set(_t1952, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t1953, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1955, 1)), gem_string(")")));
    GemVal _t1956 = gem_v_rc_r;
    gem_table_set(_t1952, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1956, gem_string("setup"))));
                                            GemVal _t1957 = _t1952;
                                            gem_pop_frame();
                                            return _t1957;
                                        } else {
#line 736 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 737 "compiler/main.gem"
    GemVal _t1958 = gem_v_node;
    GemVal _t1959[] = {gem_table_get(_t1958, gem_string("right"))};
    GemVal _t1960 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t1960.fn(_t1960.env, _t1959, 1);
#line 738 "compiler/main.gem"
    GemVal _t1961 = gem_table_new();
    GemVal _t1962[] = {gem_v_lc};
    GemVal _t1963 = gem_v_rc_r;
    GemVal _t1964[] = {gem_table_get(_t1963, gem_string("expr"))};
    gem_table_set(_t1961, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t1962, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1964, 1)), gem_string(")")));
    GemVal _t1965 = gem_v_rc_r;
    gem_table_set(_t1961, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1965, gem_string("setup"))));
                                                GemVal _t1966 = _t1961;
                                                gem_pop_frame();
                                                return _t1966;
                                            } else {
#line 739 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 740 "compiler/main.gem"
    GemVal _t1967 = gem_v_node;
    GemVal _t1968[] = {gem_table_get(_t1967, gem_string("right"))};
    GemVal _t1969 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t1969.fn(_t1969.env, _t1968, 1);
#line 741 "compiler/main.gem"
    GemVal _t1970 = gem_table_new();
    GemVal _t1971[] = {gem_v_lc};
    GemVal _t1972 = gem_v_rc_r;
    GemVal _t1973[] = {gem_table_get(_t1972, gem_string("expr"))};
    gem_table_set(_t1970, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t1971, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1973, 1)), gem_string(")")));
    GemVal _t1974 = gem_v_rc_r;
    gem_table_set(_t1970, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1974, gem_string("setup"))));
                                                    GemVal _t1975 = _t1970;
                                                    gem_pop_frame();
                                                    return _t1975;
                                                } else {
#line 742 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 743 "compiler/main.gem"
    GemVal _t1976 = gem_v_node;
    GemVal _t1977[] = {gem_table_get(_t1976, gem_string("right"))};
    GemVal _t1978 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t1978.fn(_t1978.env, _t1977, 1);
#line 744 "compiler/main.gem"
    GemVal _t1979 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1979.fn(_t1979.env, NULL, 0);
#line 745 "compiler/main.gem"
                                                        GemVal gem_v_setup = gem_v_ls;
#line 746 "compiler/main.gem"
    GemVal _t1980[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1980, 1)), gem_string(";\n")));
#line 747 "compiler/main.gem"
    GemVal _t1981[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t1981, 1)), gem_string(")) {\n")));
#line 748 "compiler/main.gem"
    GemVal _t1982[] = {gem_v_t};
    GemVal _t1983[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1982, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1983, 1)), gem_string(";\n")));
#line 749 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 750 "compiler/main.gem"
    GemVal _t1984 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t1984, gem_string("setup")), gem_string("")))) {
#line 751 "compiler/main.gem"
    GemVal _t1985 = gem_v_rc_r;
    GemVal _t1986[] = {gem_table_get(_t1985, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t1986, 3));
                                                        }
#line 753 "compiler/main.gem"
    GemVal _t1987[] = {gem_v_t};
    GemVal _t1988 = gem_v_rc_r;
    GemVal _t1989[] = {gem_table_get(_t1988, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1987, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1989, 1)), gem_string(";\n")));
#line 754 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 755 "compiler/main.gem"
    GemVal _t1990 = gem_table_new();
    gem_table_set(_t1990, gem_string("expr"), gem_v_t);
    gem_table_set(_t1990, gem_string("setup"), gem_v_setup);
                                                        GemVal _t1991 = _t1990;
                                                        gem_pop_frame();
                                                        return _t1991;
                                                    } else {
#line 756 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 757 "compiler/main.gem"
    GemVal _t1992 = gem_v_node;
    GemVal _t1993[] = {gem_table_get(_t1992, gem_string("right"))};
    GemVal _t1994 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t1994.fn(_t1994.env, _t1993, 1);
#line 758 "compiler/main.gem"
    GemVal _t1995 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1995.fn(_t1995.env, NULL, 0);
#line 759 "compiler/main.gem"
                                                            GemVal gem_v_setup = gem_v_ls;
#line 760 "compiler/main.gem"
    GemVal _t1996[] = {gem_v_t};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1996, 1)), gem_string(";\n")));
#line 761 "compiler/main.gem"
    GemVal _t1997[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t1997, 1)), gem_string(")) {\n")));
#line 762 "compiler/main.gem"
    GemVal _t1998[] = {gem_v_t};
    GemVal _t1999[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1998, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1999, 1)), gem_string(";\n")));
#line 763 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 764 "compiler/main.gem"
    GemVal _t2000 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t2000, gem_string("setup")), gem_string("")))) {
#line 765 "compiler/main.gem"
    GemVal _t2001 = gem_v_rc_r;
    GemVal _t2002[] = {gem_table_get(_t2001, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                                gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t2002, 3));
                                                            }
#line 767 "compiler/main.gem"
    GemVal _t2003[] = {gem_v_t};
    GemVal _t2004 = gem_v_rc_r;
    GemVal _t2005[] = {gem_table_get(_t2004, gem_string("expr"))};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2003, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2005, 1)), gem_string(";\n")));
#line 768 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 769 "compiler/main.gem"
    GemVal _t2006 = gem_table_new();
    gem_table_set(_t2006, gem_string("expr"), gem_v_t);
    gem_table_set(_t2006, gem_string("setup"), gem_v_setup);
                                                            GemVal _t2007 = _t2006;
                                                            gem_pop_frame();
                                                            return _t2007;
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
#line 772 "compiler/main.gem"
    GemVal _t2008[] = {gem_v_op};
    GemVal _t2009[] = {gem_add(gem_string("unknown binary operator: "), gem_to_string_fn(NULL, _t2008, 1))};
    GemVal _t2010 = gem_error_at_fn("compiler/main.gem", 772, _t2009, 1);
    gem_pop_frame();
    return _t2010;
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
#line 778 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 779 "compiler/main.gem"
    GemVal gem_v__for_items_34 = gem_v_stmts;
#line 779 "compiler/main.gem"
    GemVal gem_v__for_i_34 = gem_int(0);
#line 779 "compiler/main.gem"
    while (1) {
        GemVal _t2012[] = {gem_v__for_items_34};
        if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_len_fn(NULL, _t2012, 1)))) break;
#line 779 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_34, gem_v__for_i_34);
#line 779 "compiler/main.gem"
        gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 780 "compiler/main.gem"
    GemVal _t2013[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2014 = (*gem_v_compile_stmt);
        gem_v_out = gem_add(gem_add(gem_v_out, _t2014.fn(_t2014.env, _t2013, 2)), gem_string("\n"));
    }

    GemVal _t2015 = gem_v_out;
    gem_pop_frame();
    return _t2015;
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
#line 788 "compiler/main.gem"
    GemVal _t2017[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2017, 1);
#line 789 "compiler/main.gem"
    GemVal _t2018 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t2018, gem_string("tag"));
#line 790 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 791 "compiler/main.gem"
    GemVal _t2019 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2019, gem_string("line")), GEM_NIL))) {
#line 792 "compiler/main.gem"
    GemVal _t2020 = gem_v_node;
    GemVal _t2021[] = {gem_table_get(_t2020, gem_string("line"))};
    GemVal _t2022[] = {(*gem_v_source_name)};
    GemVal _t2023[] = {gem_fn_escape_c_string(NULL, _t2022, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2021, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2023, 1)), gem_string("\"\n"));
    }
#line 795 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 796 "compiler/main.gem"
    GemVal _t2024 = gem_v_node;
    GemVal _t2025[] = {gem_table_get(_t2024, gem_string("value"))};
    GemVal _t2026 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2026.fn(_t2026.env, _t2025, 1);
#line 797 "compiler/main.gem"
    GemVal _t2027 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t2027, gem_string("name"));
#line 798 "compiler/main.gem"
    GemVal _t2028[] = {gem_v_name};
    GemVal _t2029 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2029.fn(_t2029.env, _t2028, 1);
#line 799 "compiler/main.gem"
    GemVal _t2031;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t2031 = (*gem_v_in_top_level);
    } else {
        GemVal _t2030[] = {(*gem_v_top_level_vars), gem_v_name};
        _t2031 = gem_fn_set_contains(NULL, _t2030, 2);
    }
        if (gem_truthy(_t2031)) {
#line 800 "compiler/main.gem"
    GemVal _t2032[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2032, 2))) {
#line 801 "compiler/main.gem"
    GemVal _t2033[] = {gem_v_line_dir};
    GemVal _t2034 = gem_v_r;
    GemVal _t2035[] = {gem_table_get(_t2034, gem_string("setup"))};
    GemVal _t2036[] = {gem_v_p};
    GemVal _t2037[] = {gem_v_mname};
    GemVal _t2038[] = {gem_v_p};
    GemVal _t2039[] = {gem_v_mname};
    GemVal _t2040 = gem_v_r;
    GemVal _t2041[] = {gem_table_get(_t2040, gem_string("expr"))};
                GemVal _t2042 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2033, 1), gem_to_string_fn(NULL, _t2035, 1)), gem_to_string_fn(NULL, _t2036, 1)), gem_to_string_fn(NULL, _t2037, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2038, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2039, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2041, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2042;
            }
#line 803 "compiler/main.gem"
    GemVal _t2043[] = {gem_v_line_dir};
    GemVal _t2044 = gem_v_r;
    GemVal _t2045[] = {gem_table_get(_t2044, gem_string("setup"))};
    GemVal _t2046[] = {gem_v_p};
    GemVal _t2047[] = {gem_v_mname};
    GemVal _t2048 = gem_v_r;
    GemVal _t2049[] = {gem_table_get(_t2048, gem_string("expr"))};
            GemVal _t2050 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2043, 1), gem_to_string_fn(NULL, _t2045, 1)), gem_to_string_fn(NULL, _t2046, 1)), gem_to_string_fn(NULL, _t2047, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2049, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2050;
        }
#line 805 "compiler/main.gem"
    GemVal _t2051[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2051, 2))) {
#line 806 "compiler/main.gem"
    GemVal _t2052[] = {gem_v_line_dir};
    GemVal _t2053 = gem_v_r;
    GemVal _t2054[] = {gem_table_get(_t2053, gem_string("setup"))};
    GemVal _t2055[] = {gem_v_p};
    GemVal _t2056[] = {gem_v_mname};
    GemVal _t2057[] = {gem_v_p};
    GemVal _t2058[] = {gem_v_mname};
    GemVal _t2059 = gem_v_r;
    GemVal _t2060[] = {gem_table_get(_t2059, gem_string("expr"))};
            GemVal _t2061 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2052, 1), gem_to_string_fn(NULL, _t2054, 1)), gem_to_string_fn(NULL, _t2055, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2056, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2057, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2058, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2060, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2061;
        }
#line 808 "compiler/main.gem"
    GemVal _t2062[] = {gem_v_line_dir};
    GemVal _t2063 = gem_v_r;
    GemVal _t2064[] = {gem_table_get(_t2063, gem_string("setup"))};
    GemVal _t2065[] = {gem_v_p};
    GemVal _t2066[] = {gem_v_mname};
    GemVal _t2067 = gem_v_r;
    GemVal _t2068[] = {gem_table_get(_t2067, gem_string("expr"))};
        GemVal _t2069 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2062, 1), gem_to_string_fn(NULL, _t2064, 1)), gem_to_string_fn(NULL, _t2065, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2066, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2068, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2069;
    } else {
#line 809 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 810 "compiler/main.gem"
    GemVal _t2070 = gem_v_node;
    GemVal _t2071[] = {gem_table_get(_t2070, gem_string("value"))};
    GemVal _t2072 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2072.fn(_t2072.env, _t2071, 1);
#line 811 "compiler/main.gem"
    GemVal _t2073 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t2073, gem_string("name"));
#line 812 "compiler/main.gem"
    GemVal _t2074[] = {gem_v_name};
    GemVal _t2075 = (*gem_v_mangle);
            GemVal gem_v_mname = _t2075.fn(_t2075.env, _t2074, 1);
#line 813 "compiler/main.gem"
    GemVal _t2076[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2078;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2076, 2))) {
        _t2078 = gem_fn_set_contains(NULL, _t2076, 2);
    } else {
        GemVal _t2077[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2078 = gem_fn_set_contains(NULL, _t2077, 2);
    }
            if (gem_truthy(_t2078)) {
#line 814 "compiler/main.gem"
    GemVal _t2079[] = {gem_v_line_dir};
    GemVal _t2080 = gem_v_r;
    GemVal _t2081[] = {gem_table_get(_t2080, gem_string("setup"))};
    GemVal _t2082[] = {gem_v_p};
    GemVal _t2083[] = {gem_v_mname};
    GemVal _t2084 = gem_v_r;
    GemVal _t2085[] = {gem_table_get(_t2084, gem_string("expr"))};
                GemVal _t2086 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2079, 1), gem_to_string_fn(NULL, _t2081, 1)), gem_to_string_fn(NULL, _t2082, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2083, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2085, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2086;
            }
#line 816 "compiler/main.gem"
    GemVal _t2087[] = {gem_v_line_dir};
    GemVal _t2088 = gem_v_r;
    GemVal _t2089[] = {gem_table_get(_t2088, gem_string("setup"))};
    GemVal _t2090[] = {gem_v_p};
    GemVal _t2091[] = {gem_v_mname};
    GemVal _t2092 = gem_v_r;
    GemVal _t2093[] = {gem_table_get(_t2092, gem_string("expr"))};
            GemVal _t2094 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2087, 1), gem_to_string_fn(NULL, _t2089, 1)), gem_to_string_fn(NULL, _t2090, 1)), gem_to_string_fn(NULL, _t2091, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2093, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2094;
        } else {
#line 817 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
#line 818 "compiler/main.gem"
    GemVal _t2095 = gem_v_node;
    GemVal _t2096[] = {gem_table_get(_t2095, gem_string("object"))};
    GemVal _t2097 = (*gem_v_compile_expr);
                GemVal gem_v_obj_r = _t2097.fn(_t2097.env, _t2096, 1);
#line 819 "compiler/main.gem"
    GemVal _t2098 = gem_v_node;
    GemVal _t2099[] = {gem_table_get(_t2098, gem_string("value"))};
    GemVal _t2100 = (*gem_v_compile_expr);
                GemVal gem_v_val_r = _t2100.fn(_t2100.env, _t2099, 1);
#line 820 "compiler/main.gem"
    GemVal _t2101 = gem_v_node;
    GemVal _t2102[] = {gem_table_get(_t2101, gem_string("field"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2102, 1);
#line 821 "compiler/main.gem"
    GemVal _t2103[] = {gem_v_line_dir};
    GemVal _t2104 = gem_v_obj_r;
    GemVal _t2105[] = {gem_table_get(_t2104, gem_string("setup"))};
    GemVal _t2106 = gem_v_val_r;
    GemVal _t2107[] = {gem_table_get(_t2106, gem_string("setup"))};
    GemVal _t2108[] = {gem_v_p};
    GemVal _t2109 = gem_v_obj_r;
    GemVal _t2110[] = {gem_table_get(_t2109, gem_string("expr"))};
    GemVal _t2111[] = {gem_v_escaped};
    GemVal _t2112 = gem_v_val_r;
    GemVal _t2113[] = {gem_table_get(_t2112, gem_string("expr"))};
                GemVal _t2114 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2103, 1), gem_to_string_fn(NULL, _t2105, 1)), gem_to_string_fn(NULL, _t2107, 1)), gem_to_string_fn(NULL, _t2108, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2110, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2111, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2113, 1)), gem_string(");"));
                gem_pop_frame();
                return _t2114;
            } else {
#line 822 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("index_assign")))) {
#line 823 "compiler/main.gem"
    GemVal _t2115 = gem_v_node;
    GemVal _t2116[] = {gem_table_get(_t2115, gem_string("object"))};
    GemVal _t2117 = (*gem_v_compile_expr);
                    GemVal gem_v_obj_r = _t2117.fn(_t2117.env, _t2116, 1);
#line 824 "compiler/main.gem"
    GemVal _t2118 = gem_v_node;
    GemVal _t2119[] = {gem_table_get(_t2118, gem_string("key"))};
    GemVal _t2120 = (*gem_v_compile_expr);
                    GemVal gem_v_key_r = _t2120.fn(_t2120.env, _t2119, 1);
#line 825 "compiler/main.gem"
    GemVal _t2121 = gem_v_node;
    GemVal _t2122[] = {gem_table_get(_t2121, gem_string("value"))};
    GemVal _t2123 = (*gem_v_compile_expr);
                    GemVal gem_v_val_r = _t2123.fn(_t2123.env, _t2122, 1);
#line 826 "compiler/main.gem"
    GemVal _t2124[] = {gem_v_line_dir};
    GemVal _t2125 = gem_v_obj_r;
    GemVal _t2126[] = {gem_table_get(_t2125, gem_string("setup"))};
    GemVal _t2127 = gem_v_key_r;
    GemVal _t2128[] = {gem_table_get(_t2127, gem_string("setup"))};
    GemVal _t2129 = gem_v_val_r;
    GemVal _t2130[] = {gem_table_get(_t2129, gem_string("setup"))};
    GemVal _t2131[] = {gem_v_p};
    GemVal _t2132 = gem_v_obj_r;
    GemVal _t2133[] = {gem_table_get(_t2132, gem_string("expr"))};
    GemVal _t2134 = gem_v_key_r;
    GemVal _t2135[] = {gem_table_get(_t2134, gem_string("expr"))};
    GemVal _t2136 = gem_v_val_r;
    GemVal _t2137[] = {gem_table_get(_t2136, gem_string("expr"))};
                    GemVal _t2138 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2124, 1), gem_to_string_fn(NULL, _t2126, 1)), gem_to_string_fn(NULL, _t2128, 1)), gem_to_string_fn(NULL, _t2130, 1)), gem_to_string_fn(NULL, _t2131, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2133, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2135, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2137, 1)), gem_string(");"));
                    gem_pop_frame();
                    return _t2138;
                } else {
#line 827 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 828 "compiler/main.gem"
    GemVal _t2139[] = {gem_v_line_dir};
    GemVal _t2140[] = {gem_v_node, gem_v_indent};
    GemVal _t2141 = (*gem_v_compile_if);
                        GemVal _t2142 = gem_add(gem_to_string_fn(NULL, _t2139, 1), _t2141.fn(_t2141.env, _t2140, 2));
                        gem_pop_frame();
                        return _t2142;
                    } else {
#line 829 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 830 "compiler/main.gem"
    GemVal _t2143[] = {gem_v_line_dir};
    GemVal _t2144[] = {gem_v_node, gem_v_indent};
    GemVal _t2145 = (*gem_v_compile_while);
                            GemVal _t2146 = gem_add(gem_to_string_fn(NULL, _t2143, 1), _t2145.fn(_t2145.env, _t2144, 2));
                            gem_pop_frame();
                            return _t2146;
                        } else {
#line 831 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 832 "compiler/main.gem"
    GemVal _t2147[] = {gem_v_line_dir};
    GemVal _t2148[] = {gem_v_node, gem_v_indent};
    GemVal _t2149 = (*gem_v_compile_match);
                                GemVal _t2150 = gem_add(gem_to_string_fn(NULL, _t2147, 1), _t2149.fn(_t2149.env, _t2148, 2));
                                gem_pop_frame();
                                return _t2150;
                            } else {
#line 833 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("break")))) {
#line 834 "compiler/main.gem"
                                    GemVal _t2151 = gem_add(gem_v_p, gem_string("break;"));
                                    gem_pop_frame();
                                    return _t2151;
                                } else {
#line 835 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("continue")))) {
#line 836 "compiler/main.gem"
                                        GemVal _t2152 = gem_add(gem_v_p, gem_string("continue;"));
                                        gem_pop_frame();
                                        return _t2152;
                                    } else {
#line 837 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 838 "compiler/main.gem"
    GemVal _t2153 = gem_v_node;
                                            if (gem_truthy(gem_neq(gem_table_get(_t2153, gem_string("value")), GEM_NIL))) {
#line 839 "compiler/main.gem"
    GemVal _t2154 = gem_v_node;
    GemVal _t2155[] = {gem_table_get(_t2154, gem_string("value"))};
    GemVal _t2156 = (*gem_v_compile_expr);
                                                GemVal gem_v_r = _t2156.fn(_t2156.env, _t2155, 1);
#line 840 "compiler/main.gem"
    GemVal _t2157 = (*gem_v_tmp);
                                                GemVal gem_v_t = _t2157.fn(_t2157.env, NULL, 0);
#line 841 "compiler/main.gem"
    GemVal _t2158[] = {gem_v_line_dir};
    GemVal _t2159 = gem_v_r;
    GemVal _t2160[] = {gem_table_get(_t2159, gem_string("setup"))};
    GemVal _t2161[] = {gem_v_p};
    GemVal _t2162[] = {gem_v_t};
    GemVal _t2163 = gem_v_r;
    GemVal _t2164[] = {gem_table_get(_t2163, gem_string("expr"))};
    GemVal _t2165[] = {gem_v_p};
    GemVal _t2166[] = {gem_v_p};
    GemVal _t2167[] = {gem_v_t};
                                                GemVal _t2168 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2158, 1), gem_to_string_fn(NULL, _t2160, 1)), gem_to_string_fn(NULL, _t2161, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2162, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2164, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2165, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2166, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2167, 1)), gem_string(";"));
                                                gem_pop_frame();
                                                return _t2168;
                                            }
#line 843 "compiler/main.gem"
    GemVal _t2169[] = {gem_v_line_dir};
    GemVal _t2170[] = {gem_v_p};
    GemVal _t2171[] = {gem_v_p};
                                            GemVal _t2172 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2169, 1), gem_to_string_fn(NULL, _t2170, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2171, 1)), gem_string("return GEM_NIL;"));
                                            gem_pop_frame();
                                            return _t2172;
                                        } else {
#line 844 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
#line 845 "compiler/main.gem"
    GemVal _t2173[] = {gem_v_node};
    GemVal _t2174 = (*gem_v_compile_expr);
                                                GemVal gem_v_r = _t2174.fn(_t2174.env, _t2173, 1);
#line 846 "compiler/main.gem"
    GemVal _t2175[] = {gem_v_line_dir};
    GemVal _t2176 = gem_v_r;
    GemVal _t2177[] = {gem_table_get(_t2176, gem_string("setup"))};
    GemVal _t2178[] = {gem_v_p};
    GemVal _t2179 = gem_v_r;
    GemVal _t2180[] = {gem_table_get(_t2179, gem_string("expr"))};
                                                GemVal _t2181 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2175, 1), gem_to_string_fn(NULL, _t2177, 1)), gem_to_string_fn(NULL, _t2178, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2180, 1)), gem_string(");"));
                                                gem_pop_frame();
                                                return _t2181;
                                            } else {
#line 847 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
#line 848 "compiler/main.gem"
                                                    GemVal _t2182 = gem_string("");
                                                    gem_pop_frame();
                                                    return _t2182;
                                                } else {
#line 849 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 850 "compiler/main.gem"
                                                        GemVal gem_v_out = gem_string("");
#line 851 "compiler/main.gem"
    GemVal _t2183 = gem_v_node;
                                                        GemVal gem_v__for_items_35 = gem_table_get(_t2183, gem_string("stmts"));
#line 851 "compiler/main.gem"
                                                        GemVal gem_v__for_i_35 = gem_int(0);
#line 851 "compiler/main.gem"
                                                        while (1) {
                                                            GemVal _t2184[] = {gem_v__for_items_35};
                                                            if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_len_fn(NULL, _t2184, 1)))) break;
#line 851 "compiler/main.gem"
                                                            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_35, gem_v__for_i_35);
#line 851 "compiler/main.gem"
                                                            gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 852 "compiler/main.gem"
    GemVal _t2185[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2186 = (*gem_v_compile_stmt);
                                                            gem_v_out = gem_add(gem_add(gem_v_out, _t2186.fn(_t2186.env, _t2185, 2)), gem_string("\n"));
                                                        }

#line 854 "compiler/main.gem"
                                                        GemVal _t2187 = gem_v_out;
                                                        gem_pop_frame();
                                                        return _t2187;
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
#line 857 "compiler/main.gem"
    GemVal _t2188[] = {gem_v_node};
    GemVal _t2189 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t2189.fn(_t2189.env, _t2188, 1);
    GemVal _t2190[] = {gem_v_line_dir};
    GemVal _t2191 = gem_v_r;
    GemVal _t2192[] = {gem_table_get(_t2191, gem_string("setup"))};
    GemVal _t2193[] = {gem_v_p};
    GemVal _t2194 = gem_v_r;
    GemVal _t2195[] = {gem_table_get(_t2194, gem_string("expr"))};
    GemVal _t2196 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2190, 1), gem_to_string_fn(NULL, _t2192, 1)), gem_to_string_fn(NULL, _t2193, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2195, 1)), gem_string(");"));
    gem_pop_frame();
    return _t2196;
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
#line 864 "compiler/main.gem"
    GemVal _t2198[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2198, 1);
#line 865 "compiler/main.gem"
    GemVal _t2199 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t2199, gem_string("tag"));
#line 866 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 867 "compiler/main.gem"
    GemVal _t2200 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2200, gem_string("line")), GEM_NIL))) {
#line 868 "compiler/main.gem"
    GemVal _t2201 = gem_v_node;
    GemVal _t2202[] = {gem_table_get(_t2201, gem_string("line"))};
    GemVal _t2203[] = {(*gem_v_source_name)};
    GemVal _t2204[] = {gem_fn_escape_c_string(NULL, _t2203, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2202, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2204, 1)), gem_string("\"\n"));
    }
#line 871 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
#line 872 "compiler/main.gem"
    GemVal _t2205 = gem_v_node;
    GemVal _t2206[] = {gem_table_get(_t2205, gem_string("value"))};
    GemVal _t2207 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2207.fn(_t2207.env, _t2206, 1);
#line 873 "compiler/main.gem"
    GemVal _t2208 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t2208, gem_string("name"));
#line 874 "compiler/main.gem"
    GemVal _t2209[] = {gem_v_name};
    GemVal _t2210 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2210.fn(_t2210.env, _t2209, 1);
#line 875 "compiler/main.gem"
    GemVal _t2211[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2211, 2))) {
#line 876 "compiler/main.gem"
    GemVal _t2212[] = {gem_v_line_dir};
    GemVal _t2213 = gem_v_r;
    GemVal _t2214[] = {gem_table_get(_t2213, gem_string("setup"))};
    GemVal _t2215[] = {gem_v_p};
    GemVal _t2216[] = {gem_v_mname};
    GemVal _t2217[] = {gem_v_p};
    GemVal _t2218[] = {gem_v_mname};
    GemVal _t2219 = gem_v_r;
    GemVal _t2220[] = {gem_table_get(_t2219, gem_string("expr"))};
    GemVal _t2221[] = {gem_v_p};
    GemVal _t2222[] = {gem_v_p};
            GemVal _t2223 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2212, 1), gem_to_string_fn(NULL, _t2214, 1)), gem_to_string_fn(NULL, _t2215, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2216, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2217, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2218, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2220, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2221, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2222, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t2223;
        }
#line 878 "compiler/main.gem"
    GemVal _t2224[] = {gem_v_line_dir};
    GemVal _t2225 = gem_v_r;
    GemVal _t2226[] = {gem_table_get(_t2225, gem_string("setup"))};
    GemVal _t2227[] = {gem_v_p};
    GemVal _t2228[] = {gem_v_mname};
    GemVal _t2229 = gem_v_r;
    GemVal _t2230[] = {gem_table_get(_t2229, gem_string("expr"))};
    GemVal _t2231[] = {gem_v_p};
    GemVal _t2232[] = {gem_v_p};
        GemVal _t2233 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2224, 1), gem_to_string_fn(NULL, _t2226, 1)), gem_to_string_fn(NULL, _t2227, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2228, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2230, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2231, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2232, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2233;
    } else {
#line 879 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
#line 880 "compiler/main.gem"
    GemVal _t2234 = gem_v_node;
    GemVal _t2235[] = {gem_table_get(_t2234, gem_string("value"))};
    GemVal _t2236 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2236.fn(_t2236.env, _t2235, 1);
#line 881 "compiler/main.gem"
    GemVal _t2237 = (*gem_v_tmp);
            GemVal gem_v_t = _t2237.fn(_t2237.env, NULL, 0);
#line 882 "compiler/main.gem"
    GemVal _t2238 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t2238, gem_string("name"));
#line 883 "compiler/main.gem"
    GemVal _t2239[] = {gem_v_name};
    GemVal _t2240 = (*gem_v_mangle);
            GemVal gem_v_mname = _t2240.fn(_t2240.env, _t2239, 1);
#line 884 "compiler/main.gem"
    GemVal _t2241[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2243;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2241, 2))) {
        _t2243 = gem_fn_set_contains(NULL, _t2241, 2);
    } else {
        GemVal _t2242[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2243 = gem_fn_set_contains(NULL, _t2242, 2);
    }
            if (gem_truthy(_t2243)) {
#line 885 "compiler/main.gem"
    GemVal _t2244[] = {gem_v_line_dir};
    GemVal _t2245 = gem_v_r;
    GemVal _t2246[] = {gem_table_get(_t2245, gem_string("setup"))};
    GemVal _t2247[] = {gem_v_p};
    GemVal _t2248[] = {gem_v_t};
    GemVal _t2249 = gem_v_r;
    GemVal _t2250[] = {gem_table_get(_t2249, gem_string("expr"))};
    GemVal _t2251[] = {gem_v_p};
    GemVal _t2252[] = {gem_v_mname};
    GemVal _t2253[] = {gem_v_t};
    GemVal _t2254[] = {gem_v_p};
    GemVal _t2255[] = {gem_v_p};
    GemVal _t2256[] = {gem_v_t};
                GemVal _t2257 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2244, 1), gem_to_string_fn(NULL, _t2246, 1)), gem_to_string_fn(NULL, _t2247, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2248, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2250, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2251, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2252, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2253, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2254, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2255, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2256, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2257;
            }
#line 887 "compiler/main.gem"
    GemVal _t2258[] = {gem_v_line_dir};
    GemVal _t2259 = gem_v_r;
    GemVal _t2260[] = {gem_table_get(_t2259, gem_string("setup"))};
    GemVal _t2261[] = {gem_v_p};
    GemVal _t2262[] = {gem_v_t};
    GemVal _t2263 = gem_v_r;
    GemVal _t2264[] = {gem_table_get(_t2263, gem_string("expr"))};
    GemVal _t2265[] = {gem_v_p};
    GemVal _t2266[] = {gem_v_mname};
    GemVal _t2267[] = {gem_v_t};
    GemVal _t2268[] = {gem_v_p};
    GemVal _t2269[] = {gem_v_p};
    GemVal _t2270[] = {gem_v_t};
            GemVal _t2271 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2258, 1), gem_to_string_fn(NULL, _t2260, 1)), gem_to_string_fn(NULL, _t2261, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2262, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2264, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2265, 1)), gem_to_string_fn(NULL, _t2266, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2267, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2268, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2269, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2270, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2271;
        } else {
#line 888 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
#line 889 "compiler/main.gem"
    GemVal _t2272 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t2272, gem_string("value")), GEM_NIL))) {
#line 890 "compiler/main.gem"
    GemVal _t2273 = gem_v_node;
    GemVal _t2274[] = {gem_table_get(_t2273, gem_string("value"))};
    GemVal _t2275 = (*gem_v_compile_expr);
                    GemVal gem_v_r = _t2275.fn(_t2275.env, _t2274, 1);
#line 891 "compiler/main.gem"
    GemVal _t2276 = (*gem_v_tmp);
                    GemVal gem_v_t = _t2276.fn(_t2276.env, NULL, 0);
#line 892 "compiler/main.gem"
    GemVal _t2277[] = {gem_v_line_dir};
    GemVal _t2278 = gem_v_r;
    GemVal _t2279[] = {gem_table_get(_t2278, gem_string("setup"))};
    GemVal _t2280[] = {gem_v_p};
    GemVal _t2281[] = {gem_v_t};
    GemVal _t2282 = gem_v_r;
    GemVal _t2283[] = {gem_table_get(_t2282, gem_string("expr"))};
    GemVal _t2284[] = {gem_v_p};
    GemVal _t2285[] = {gem_v_p};
    GemVal _t2286[] = {gem_v_t};
                    GemVal _t2287 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2277, 1), gem_to_string_fn(NULL, _t2279, 1)), gem_to_string_fn(NULL, _t2280, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2281, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2283, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2284, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2285, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2286, 1)), gem_string(";"));
                    gem_pop_frame();
                    return _t2287;
                }
#line 894 "compiler/main.gem"
    GemVal _t2288[] = {gem_v_line_dir};
    GemVal _t2289[] = {gem_v_p};
    GemVal _t2290[] = {gem_v_p};
                GemVal _t2291 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2288, 1), gem_to_string_fn(NULL, _t2289, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2290, 1)), gem_string("return GEM_NIL;"));
                gem_pop_frame();
                return _t2291;
            } else {
#line 895 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
#line 896 "compiler/main.gem"
    GemVal _t2292[] = {gem_v_line_dir};
    GemVal _t2293[] = {gem_v_node, gem_v_indent};
    GemVal _t2294 = (*gem_v_compile_if_return);
                    GemVal _t2295 = gem_add(gem_to_string_fn(NULL, _t2292, 1), _t2294.fn(_t2294.env, _t2293, 2));
                    gem_pop_frame();
                    return _t2295;
                } else {
#line 897 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
#line 898 "compiler/main.gem"
    GemVal _t2296[] = {gem_v_line_dir};
    GemVal _t2297[] = {gem_v_node, gem_v_indent};
    GemVal _t2298 = (*gem_v_compile_while);
    GemVal _t2299[] = {_t2298.fn(_t2298.env, _t2297, 2)};
    GemVal _t2300[] = {gem_v_p};
    GemVal _t2301[] = {gem_v_p};
                        GemVal _t2302 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2296, 1), gem_to_string_fn(NULL, _t2299, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t2300, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2301, 1)), gem_string("return GEM_NIL;"));
                        gem_pop_frame();
                        return _t2302;
                    } else {
#line 899 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
#line 900 "compiler/main.gem"
    GemVal _t2303[] = {gem_v_line_dir};
    GemVal _t2304[] = {gem_v_node, gem_v_indent};
    GemVal _t2305 = (*gem_v_compile_match_return);
                            GemVal _t2306 = gem_add(gem_to_string_fn(NULL, _t2303, 1), _t2305.fn(_t2305.env, _t2304, 2));
                            gem_pop_frame();
                            return _t2306;
                        } else {
#line 901 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
#line 902 "compiler/main.gem"
    GemVal _t2307[] = {gem_v_p};
                                GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2307, 1), gem_string("{\n"));
#line 903 "compiler/main.gem"
                                GemVal gem_v__for_i_36 = gem_int(0);
#line 903 "compiler/main.gem"
    GemVal _t2308 = gem_v_node;
    GemVal _t2309[] = {gem_table_get(_t2308, gem_string("stmts"))};
                                GemVal gem_v__for_limit_36 = gem_sub(gem_len_fn(NULL, _t2309, 1), gem_int(1));
#line 903 "compiler/main.gem"
                                while (1) {
                                    if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 903 "compiler/main.gem"
                                    GemVal gem_v_i = gem_v__for_i_36;
#line 903 "compiler/main.gem"
                                    gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 904 "compiler/main.gem"
    GemVal _t2310 = gem_v_node;
    GemVal _t2311[] = {gem_table_get(gem_table_get(_t2310, gem_string("stmts")), gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2312 = (*gem_v_compile_stmt);
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t2312.fn(_t2312.env, _t2311, 2)), gem_string("\n"));
                                }

#line 906 "compiler/main.gem"
    GemVal _t2313 = gem_v_node;
    GemVal _t2314[] = {gem_table_get(_t2313, gem_string("stmts"))};
                                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2314, 1), gem_int(0)))) {
#line 907 "compiler/main.gem"
    GemVal _t2315 = gem_v_node;
    GemVal _t2316 = gem_v_node;
    GemVal _t2317[] = {gem_table_get(_t2316, gem_string("stmts"))};
    GemVal _t2318[] = {gem_table_get(gem_table_get(_t2315, gem_string("stmts")), gem_sub(gem_len_fn(NULL, _t2317, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2319 = (*gem_v_compile_stmt_return);
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t2319.fn(_t2319.env, _t2318, 2)), gem_string("\n"));
                                }
#line 909 "compiler/main.gem"
    GemVal _t2320[] = {gem_v_p};
                                gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2320, 1), gem_string("}")));
#line 910 "compiler/main.gem"
                                GemVal _t2321 = gem_v_out;
                                gem_pop_frame();
                                return _t2321;
                            } else {
#line 911 "compiler/main.gem"
    GemVal _t2322;
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
        _t2322 = gem_eq(gem_v_tag, gem_string("dot_assign"));
    } else {
        _t2322 = gem_eq(gem_v_tag, gem_string("index_assign"));
    }
    GemVal _t2323;
    if (gem_truthy(_t2322)) {
        _t2323 = _t2322;
    } else {
        _t2323 = gem_eq(gem_v_tag, gem_string("break"));
    }
    GemVal _t2324;
    if (gem_truthy(_t2323)) {
        _t2324 = _t2323;
    } else {
        _t2324 = gem_eq(gem_v_tag, gem_string("continue"));
    }
    GemVal _t2325;
    if (gem_truthy(_t2324)) {
        _t2325 = _t2324;
    } else {
        _t2325 = gem_eq(gem_v_tag, gem_string("fn_def"));
    }
                                if (gem_truthy(_t2325)) {
#line 912 "compiler/main.gem"
    GemVal _t2326[] = {gem_v_node, gem_v_indent};
    GemVal _t2327 = (*gem_v_compile_stmt);
    GemVal _t2328[] = {_t2327.fn(_t2327.env, _t2326, 2)};
    GemVal _t2329[] = {gem_v_p};
    GemVal _t2330[] = {gem_v_p};
                                    GemVal _t2331 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2328, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2329, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2330, 1)), gem_string("return GEM_NIL;"));
                                    gem_pop_frame();
                                    return _t2331;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 915 "compiler/main.gem"
    GemVal _t2332[] = {gem_v_node};
    GemVal _t2333 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t2333.fn(_t2333.env, _t2332, 1);
#line 916 "compiler/main.gem"
    GemVal _t2334 = (*gem_v_tmp);
    GemVal gem_v_t = _t2334.fn(_t2334.env, NULL, 0);
    GemVal _t2335[] = {gem_v_line_dir};
    GemVal _t2336 = gem_v_r;
    GemVal _t2337[] = {gem_table_get(_t2336, gem_string("setup"))};
    GemVal _t2338[] = {gem_v_p};
    GemVal _t2339[] = {gem_v_t};
    GemVal _t2340 = gem_v_r;
    GemVal _t2341[] = {gem_table_get(_t2340, gem_string("expr"))};
    GemVal _t2342[] = {gem_v_p};
    GemVal _t2343[] = {gem_v_p};
    GemVal _t2344[] = {gem_v_t};
    GemVal _t2345 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2335, 1), gem_to_string_fn(NULL, _t2337, 1)), gem_to_string_fn(NULL, _t2338, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2339, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2341, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2342, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2343, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2344, 1)), gem_string(";"));
    gem_pop_frame();
    return _t2345;
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
#line 923 "compiler/main.gem"
    GemVal _t2347[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2347, 1);
#line 924 "compiler/main.gem"
    GemVal _t2348 = gem_v_node;
    GemVal _t2349[] = {gem_table_get(_t2348, gem_string("cond"))};
    GemVal _t2350 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t2350.fn(_t2350.env, _t2349, 1);
#line 925 "compiler/main.gem"
    GemVal _t2351 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2351, gem_string("setup"));
#line 926 "compiler/main.gem"
    GemVal _t2352[] = {gem_v_p};
    GemVal _t2353 = gem_v_cond_r;
    GemVal _t2354[] = {gem_table_get(_t2353, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2352, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2354, 1)), gem_string(")) {\n")));
#line 927 "compiler/main.gem"
    GemVal _t2355 = gem_v_node;
    GemVal _t2356[] = {gem_table_get(_t2355, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2357 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t2357.fn(_t2357.env, _t2356, 2));
#line 928 "compiler/main.gem"
    GemVal _t2358 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2358, gem_string("else")), GEM_NIL))) {
#line 929 "compiler/main.gem"
    GemVal _t2359[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2359, 1), gem_string("} else {\n")));
#line 930 "compiler/main.gem"
    GemVal _t2360 = gem_v_node;
    GemVal _t2361[] = {gem_table_get(_t2360, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2362 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t2362.fn(_t2362.env, _t2361, 2));
    }
#line 932 "compiler/main.gem"
    GemVal _t2363[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2363, 1), gem_string("}")));
    GemVal _t2364 = gem_v_out;
    gem_pop_frame();
    return _t2364;
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
#line 939 "compiler/main.gem"
    GemVal _t2366[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2366, 1);
#line 940 "compiler/main.gem"
    GemVal _t2367 = gem_v_node;
    GemVal _t2368[] = {gem_table_get(_t2367, gem_string("cond"))};
    GemVal _t2369 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t2369.fn(_t2369.env, _t2368, 1);
#line 941 "compiler/main.gem"
    GemVal _t2370 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2370, gem_string("setup"));
#line 942 "compiler/main.gem"
    GemVal _t2371[] = {gem_v_p};
    GemVal _t2372 = gem_v_cond_r;
    GemVal _t2373[] = {gem_table_get(_t2372, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2371, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2373, 1)), gem_string(")) {\n")));
#line 943 "compiler/main.gem"
    GemVal _t2374 = gem_v_node;
    GemVal gem_v_then_body = gem_table_get(_t2374, gem_string("then"));
#line 944 "compiler/main.gem"
    GemVal _t2375[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2375, 1), gem_int(0)))) {
#line 945 "compiler/main.gem"
        GemVal gem_v__for_i_37 = gem_int(0);
#line 945 "compiler/main.gem"
    GemVal _t2376[] = {gem_v_then_body};
        GemVal gem_v__for_limit_37 = gem_sub(gem_len_fn(NULL, _t2376, 1), gem_int(1));
#line 945 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 945 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_37;
#line 945 "compiler/main.gem"
            gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 946 "compiler/main.gem"
    GemVal _t2377[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2378 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2378.fn(_t2378.env, _t2377, 2)), gem_string("\n"));
        }

#line 948 "compiler/main.gem"
    GemVal _t2379[] = {gem_v_then_body};
    GemVal _t2380[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t2379, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2381 = (*gem_v_compile_stmt_return);
        gem_v_out = gem_add(gem_add(gem_v_out, _t2381.fn(_t2381.env, _t2380, 2)), gem_string("\n"));
    } else {
#line 950 "compiler/main.gem"
    GemVal _t2382[] = {gem_v_p};
    GemVal _t2383[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2382, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2383, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 952 "compiler/main.gem"
    GemVal _t2384 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2384, gem_string("else")), GEM_NIL))) {
#line 953 "compiler/main.gem"
    GemVal _t2385[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2385, 1), gem_string("} else {\n")));
#line 954 "compiler/main.gem"
    GemVal _t2386 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t2386, gem_string("else"));
#line 955 "compiler/main.gem"
    GemVal _t2387[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2387, 1), gem_int(0)))) {
#line 956 "compiler/main.gem"
            GemVal gem_v__for_i_38 = gem_int(0);
#line 956 "compiler/main.gem"
    GemVal _t2388[] = {gem_v_else_body};
            GemVal gem_v__for_limit_38 = gem_sub(gem_len_fn(NULL, _t2388, 1), gem_int(1));
#line 956 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 956 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_38;
#line 956 "compiler/main.gem"
                gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 957 "compiler/main.gem"
    GemVal _t2389[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2390 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t2390.fn(_t2390.env, _t2389, 2)), gem_string("\n"));
            }

#line 959 "compiler/main.gem"
    GemVal _t2391[] = {gem_v_else_body};
    GemVal _t2392[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2391, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2393 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2393.fn(_t2393.env, _t2392, 2)), gem_string("\n"));
        } else {
#line 961 "compiler/main.gem"
    GemVal _t2394[] = {gem_v_p};
    GemVal _t2395[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2394, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2395, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 964 "compiler/main.gem"
    GemVal _t2396[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2396, 1), gem_string("} else {\n")));
#line 965 "compiler/main.gem"
    GemVal _t2397[] = {gem_v_p};
    GemVal _t2398[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2397, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2398, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 967 "compiler/main.gem"
    GemVal _t2399[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2399, 1), gem_string("}")));
    GemVal _t2400 = gem_v_out;
    gem_pop_frame();
    return _t2400;
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
#line 974 "compiler/main.gem"
    GemVal _t2402[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2402, 1);
#line 975 "compiler/main.gem"
    GemVal _t2403 = gem_v_node;
    GemVal _t2404[] = {gem_table_get(_t2403, gem_string("cond"))};
    GemVal _t2405 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t2405.fn(_t2405.env, _t2404, 1);
#line 976 "compiler/main.gem"
    GemVal _t2406[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2406, 1), gem_string("while (1) {\n"));
#line 977 "compiler/main.gem"
    GemVal _t2407 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t2407, gem_string("setup")), gem_string("")))) {
#line 978 "compiler/main.gem"
    GemVal _t2408 = gem_v_cond_r;
    GemVal _t2409[] = {gem_v_p};
    GemVal _t2410[] = {gem_table_get(_t2408, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t2409, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_str_replace_fn(NULL, _t2410, 3));
    }
#line 980 "compiler/main.gem"
    GemVal _t2411[] = {gem_v_p};
    GemVal _t2412 = gem_v_cond_r;
    GemVal _t2413[] = {gem_table_get(_t2412, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2411, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t2413, 1)), gem_string(")) break;\n")));
#line 981 "compiler/main.gem"
    GemVal _t2414 = gem_v_node;
    GemVal _t2415[] = {gem_table_get(_t2414, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2416 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t2416.fn(_t2416.env, _t2415, 2));
#line 982 "compiler/main.gem"
    GemVal _t2417[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2417, 1), gem_string("}")));
    GemVal _t2418 = gem_v_out;
    gem_pop_frame();
    return _t2418;
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
#line 989 "compiler/main.gem"
    GemVal _t2420[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2420, 1);
#line 990 "compiler/main.gem"
    GemVal _t2421 = gem_v_node;
    GemVal _t2422[] = {gem_table_get(_t2421, gem_string("target"))};
    GemVal _t2423 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t2423.fn(_t2423.env, _t2422, 1);
#line 991 "compiler/main.gem"
    GemVal _t2424 = (*gem_v_tmp);
    GemVal gem_v_t = _t2424.fn(_t2424.env, NULL, 0);
#line 992 "compiler/main.gem"
    GemVal _t2425 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2425, gem_string("setup"));
#line 993 "compiler/main.gem"
    GemVal _t2426[] = {gem_v_p};
    GemVal _t2427[] = {gem_v_t};
    GemVal _t2428 = gem_v_target_r;
    GemVal _t2429[] = {gem_table_get(_t2428, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2426, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2427, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2429, 1)), gem_string(";\n")));
#line 994 "compiler/main.gem"
    GemVal _t2430 = gem_v_node;
    GemVal _t2431[] = {gem_table_get(_t2430, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2431, 1), gem_int(0)))) {
#line 996 "compiler/main.gem"
    GemVal _t2432 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2432, gem_string("else")), GEM_NIL))) {
#line 997 "compiler/main.gem"
    GemVal _t2433 = gem_v_node;
    GemVal _t2434[] = {gem_table_get(_t2433, gem_string("else")), gem_v_indent};
    GemVal _t2435 = (*gem_v_compile_stmts);
            gem_v_out = gem_add(gem_v_out, _t2435.fn(_t2435.env, _t2434, 2));
        }
#line 999 "compiler/main.gem"
    GemVal _t2436[] = {gem_v_p};
    GemVal _t2437[] = {gem_v_t};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2436, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t2437, 1)), gem_string(";")));
#line 1000 "compiler/main.gem"
        GemVal _t2438 = gem_v_out;
        gem_pop_frame();
        return _t2438;
    }
#line 1002 "compiler/main.gem"
    GemVal gem_v__for_i_39 = gem_int(0);
#line 1002 "compiler/main.gem"
    GemVal _t2439 = gem_v_node;
    GemVal _t2440[] = {gem_table_get(_t2439, gem_string("whens"))};
    GemVal gem_v__for_limit_39 = gem_len_fn(NULL, _t2440, 1);
#line 1002 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
#line 1002 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_39;
#line 1002 "compiler/main.gem"
        gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 1003 "compiler/main.gem"
    GemVal _t2441 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t2441, gem_string("whens")), gem_v_i);
#line 1004 "compiler/main.gem"
    GemVal _t2442 = gem_v_w;
    GemVal _t2443[] = {gem_table_get(_t2442, gem_string("value"))};
    GemVal _t2444 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2444.fn(_t2444.env, _t2443, 1);
#line 1005 "compiler/main.gem"
        GemVal gem_v_keyword = gem_string("if");
#line 1006 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1007 "compiler/main.gem"
            gem_v_keyword = gem_string("} else if");
        }
#line 1009 "compiler/main.gem"
    GemVal _t2445 = gem_v_val_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t2445, gem_string("setup")));
#line 1010 "compiler/main.gem"
    GemVal _t2446[] = {gem_v_p};
    GemVal _t2447[] = {gem_v_keyword};
    GemVal _t2448[] = {gem_v_t};
    GemVal _t2449 = gem_v_val_r;
    GemVal _t2450[] = {gem_table_get(_t2449, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2446, 1), gem_to_string_fn(NULL, _t2447, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2448, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2450, 1)), gem_string("))) {\n")));
#line 1011 "compiler/main.gem"
    GemVal _t2451 = gem_v_w;
    GemVal _t2452[] = {gem_table_get(_t2451, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2453 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t2453.fn(_t2453.env, _t2452, 2));
    }

#line 1013 "compiler/main.gem"
    GemVal _t2454 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2454, gem_string("else")), GEM_NIL))) {
#line 1014 "compiler/main.gem"
    GemVal _t2455[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2455, 1), gem_string("} else {\n")));
#line 1015 "compiler/main.gem"
    GemVal _t2456 = gem_v_node;
    GemVal _t2457[] = {gem_table_get(_t2456, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2458 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t2458.fn(_t2458.env, _t2457, 2));
    }
#line 1017 "compiler/main.gem"
    GemVal _t2459[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2459, 1), gem_string("}")));
    GemVal _t2460 = gem_v_out;
    gem_pop_frame();
    return _t2460;
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
#line 1024 "compiler/main.gem"
    GemVal _t2462[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2462, 1);
#line 1025 "compiler/main.gem"
    GemVal _t2463 = gem_v_node;
    GemVal _t2464[] = {gem_table_get(_t2463, gem_string("target"))};
    GemVal _t2465 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t2465.fn(_t2465.env, _t2464, 1);
#line 1026 "compiler/main.gem"
    GemVal _t2466 = (*gem_v_tmp);
    GemVal gem_v_t = _t2466.fn(_t2466.env, NULL, 0);
#line 1027 "compiler/main.gem"
    GemVal _t2467 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2467, gem_string("setup"));
#line 1028 "compiler/main.gem"
    GemVal _t2468[] = {gem_v_p};
    GemVal _t2469[] = {gem_v_t};
    GemVal _t2470 = gem_v_target_r;
    GemVal _t2471[] = {gem_table_get(_t2470, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2468, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2469, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2471, 1)), gem_string(";\n")));
#line 1029 "compiler/main.gem"
    GemVal _t2472 = gem_v_node;
    GemVal _t2473[] = {gem_table_get(_t2472, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2473, 1), gem_int(0)))) {
#line 1031 "compiler/main.gem"
    GemVal _t2474 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2474, gem_string("else")), GEM_NIL))) {
#line 1032 "compiler/main.gem"
    GemVal _t2475 = gem_v_node;
            GemVal gem_v_else_body = gem_table_get(_t2475, gem_string("else"));
#line 1033 "compiler/main.gem"
    GemVal _t2476[] = {gem_v_else_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2476, 1), gem_int(0)))) {
#line 1034 "compiler/main.gem"
                GemVal gem_v__for_i_40 = gem_int(0);
#line 1034 "compiler/main.gem"
    GemVal _t2477[] = {gem_v_else_body};
                GemVal gem_v__for_limit_40 = gem_sub(gem_len_fn(NULL, _t2477, 1), gem_int(1));
#line 1034 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 1034 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_40;
#line 1034 "compiler/main.gem"
                    gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 1035 "compiler/main.gem"
    GemVal _t2478[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_v_indent};
    GemVal _t2479 = (*gem_v_compile_stmt);
                    gem_v_out = gem_add(gem_add(gem_v_out, _t2479.fn(_t2479.env, _t2478, 2)), gem_string("\n"));
                }

#line 1037 "compiler/main.gem"
    GemVal _t2480[] = {gem_v_else_body};
    GemVal _t2481[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2480, 1), gem_int(1))), gem_v_indent};
    GemVal _t2482 = (*gem_v_compile_stmt_return);
                gem_v_out = gem_add(gem_add(gem_v_out, _t2482.fn(_t2482.env, _t2481, 2)), gem_string("\n"));
            } else {
#line 1039 "compiler/main.gem"
    GemVal _t2483[] = {gem_v_p};
    GemVal _t2484[] = {gem_v_p};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2483, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2484, 1)), gem_string("return GEM_NIL;\n")));
            }
        } else {
#line 1042 "compiler/main.gem"
    GemVal _t2485[] = {gem_v_p};
    GemVal _t2486[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2485, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2486, 1)), gem_string("return GEM_NIL;\n")));
        }
#line 1044 "compiler/main.gem"
    GemVal _t2487[] = {gem_v_p};
    GemVal _t2488[] = {gem_v_t};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2487, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t2488, 1)), gem_string(";")));
#line 1045 "compiler/main.gem"
        GemVal _t2489 = gem_v_out;
        gem_pop_frame();
        return _t2489;
    }
#line 1047 "compiler/main.gem"
    GemVal gem_v__for_i_42 = gem_int(0);
#line 1047 "compiler/main.gem"
    GemVal _t2490 = gem_v_node;
    GemVal _t2491[] = {gem_table_get(_t2490, gem_string("whens"))};
    GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2491, 1);
#line 1047 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 1047 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_42;
#line 1047 "compiler/main.gem"
        gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 1048 "compiler/main.gem"
    GemVal _t2492 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t2492, gem_string("whens")), gem_v_i);
#line 1049 "compiler/main.gem"
    GemVal _t2493 = gem_v_w;
    GemVal _t2494[] = {gem_table_get(_t2493, gem_string("value"))};
    GemVal _t2495 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2495.fn(_t2495.env, _t2494, 1);
#line 1050 "compiler/main.gem"
        GemVal gem_v_keyword = gem_string("if");
#line 1051 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1052 "compiler/main.gem"
            gem_v_keyword = gem_string("} else if");
        }
#line 1054 "compiler/main.gem"
    GemVal _t2496 = gem_v_val_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t2496, gem_string("setup")));
#line 1055 "compiler/main.gem"
    GemVal _t2497[] = {gem_v_p};
    GemVal _t2498[] = {gem_v_keyword};
    GemVal _t2499[] = {gem_v_t};
    GemVal _t2500 = gem_v_val_r;
    GemVal _t2501[] = {gem_table_get(_t2500, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2497, 1), gem_to_string_fn(NULL, _t2498, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t2499, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2501, 1)), gem_string("))) {\n")));
#line 1056 "compiler/main.gem"
    GemVal _t2502 = gem_v_w;
        GemVal gem_v_body = gem_table_get(_t2502, gem_string("body"));
#line 1057 "compiler/main.gem"
    GemVal _t2503[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2503, 1), gem_int(0)))) {
#line 1058 "compiler/main.gem"
            GemVal gem_v__for_i_41 = gem_int(0);
#line 1058 "compiler/main.gem"
    GemVal _t2504[] = {gem_v_body};
            GemVal gem_v__for_limit_41 = gem_sub(gem_len_fn(NULL, _t2504, 1), gem_int(1));
#line 1058 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_limit_41))) break;
#line 1058 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_41;
#line 1058 "compiler/main.gem"
                gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
#line 1059 "compiler/main.gem"
    GemVal _t2505[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2506 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t2506.fn(_t2506.env, _t2505, 2)), gem_string("\n"));
            }

#line 1061 "compiler/main.gem"
    GemVal _t2507[] = {gem_v_body};
    GemVal _t2508[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2507, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2509 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2509.fn(_t2509.env, _t2508, 2)), gem_string("\n"));
        } else {
#line 1063 "compiler/main.gem"
    GemVal _t2510[] = {gem_v_p};
    GemVal _t2511[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2510, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2511, 1)), gem_string("    return GEM_NIL;\n")));
        }
    }

#line 1066 "compiler/main.gem"
    GemVal _t2512 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2512, gem_string("else")), GEM_NIL))) {
#line 1067 "compiler/main.gem"
    GemVal _t2513[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2513, 1), gem_string("} else {\n")));
#line 1068 "compiler/main.gem"
    GemVal _t2514 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t2514, gem_string("else"));
#line 1069 "compiler/main.gem"
    GemVal _t2515[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2515, 1), gem_int(0)))) {
#line 1070 "compiler/main.gem"
            GemVal gem_v__for_i_43 = gem_int(0);
#line 1070 "compiler/main.gem"
    GemVal _t2516[] = {gem_v_else_body};
            GemVal gem_v__for_limit_43 = gem_sub(gem_len_fn(NULL, _t2516, 1), gem_int(1));
#line 1070 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 1070 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_43;
#line 1070 "compiler/main.gem"
                gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 1071 "compiler/main.gem"
    GemVal _t2517[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2518 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t2518.fn(_t2518.env, _t2517, 2)), gem_string("\n"));
            }

#line 1073 "compiler/main.gem"
    GemVal _t2519[] = {gem_v_else_body};
    GemVal _t2520[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2519, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2521 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2521.fn(_t2521.env, _t2520, 2)), gem_string("\n"));
        } else {
#line 1075 "compiler/main.gem"
    GemVal _t2522[] = {gem_v_p};
    GemVal _t2523[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2522, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2523, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1078 "compiler/main.gem"
    GemVal _t2524[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2524, 1), gem_string("} else {\n")));
#line 1079 "compiler/main.gem"
    GemVal _t2525[] = {gem_v_p};
    GemVal _t2526[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2525, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2526, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1081 "compiler/main.gem"
    GemVal _t2527[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2527, 1), gem_string("}")));
    GemVal _t2528 = gem_v_out;
    gem_pop_frame();
    return _t2528;
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
#line 1088 "compiler/main.gem"
    GemVal gem_v__d44 = gem_v_node;
#line 1088 "compiler/main.gem"
    GemVal _t2530 = gem_v__d44;
    GemVal gem_v_name = gem_table_get(_t2530, gem_string("name"));
#line 1088 "compiler/main.gem"
    GemVal _t2531 = gem_v__d44;
    GemVal gem_v_params = gem_table_get(_t2531, gem_string("params"));
#line 1088 "compiler/main.gem"
    GemVal _t2532 = gem_v__d44;
    GemVal gem_v_ret_type = gem_table_get(_t2532, gem_string("ret_type"));

#line 1090 "compiler/main.gem"
    GemVal _t2533[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2533, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1092 "compiler/main.gem"
    GemVal _t2534 = gem_table_new();
    GemVal gem_v_c_args = _t2534;
#line 1093 "compiler/main.gem"
    GemVal gem_v__for_i_45 = gem_int(0);
#line 1093 "compiler/main.gem"
    GemVal _t2535[] = {gem_v_params};
    GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2535, 1);
#line 1093 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 1093 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_45;
#line 1093 "compiler/main.gem"
        gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 1094 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1095 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1096 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1097 "compiler/main.gem"
    GemVal _t2536[] = {gem_v_i};
    GemVal _t2537[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t2536, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2537, 1)), gem_string("].ival;\n")));
#line 1098 "compiler/main.gem"
    GemVal _t2538[] = {gem_v_i};
    GemVal _t2539[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2538, 1))};
            (void)(gem_push_fn(NULL, _t2539, 2));
        } else {
#line 1099 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1100 "compiler/main.gem"
    GemVal _t2540[] = {gem_v_i};
    GemVal _t2541[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t2540, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2541, 1)), gem_string("].fval;\n")));
#line 1101 "compiler/main.gem"
    GemVal _t2542[] = {gem_v_i};
    GemVal _t2543[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2542, 1))};
                (void)(gem_push_fn(NULL, _t2543, 2));
            } else {
#line 1102 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1103 "compiler/main.gem"
    GemVal _t2544[] = {gem_v_i};
    GemVal _t2545[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t2544, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2545, 1)), gem_string("].sval;\n")));
#line 1104 "compiler/main.gem"
    GemVal _t2546[] = {gem_v_i};
    GemVal _t2547[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2546, 1))};
                    (void)(gem_push_fn(NULL, _t2547, 2));
                } else {
#line 1105 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1106 "compiler/main.gem"
    GemVal _t2548[] = {gem_v_i};
    GemVal _t2549[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t2548, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t2549, 1)), gem_string("].bval;\n")));
#line 1107 "compiler/main.gem"
    GemVal _t2550[] = {gem_v_i};
    GemVal _t2551[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2550, 1))};
                        (void)(gem_push_fn(NULL, _t2551, 2));
                    } else {
#line 1108 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1109 "compiler/main.gem"
    GemVal _t2552[] = {gem_v_i};
    GemVal _t2553[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t2552, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t2553, 1)), gem_string("].ival;\n")));
#line 1110 "compiler/main.gem"
    GemVal _t2554[] = {gem_v_i};
    GemVal _t2555[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t2554, 1))};
                            (void)(gem_push_fn(NULL, _t2555, 2));
                        } else {
#line 1111 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1112 "compiler/main.gem"
    GemVal _t2556[] = {gem_v_i};
    GemVal _t2557[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2556, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t2557, 2));
                            } else {
#line 1114 "compiler/main.gem"
    GemVal _t2558[] = {gem_v_i};
    GemVal _t2559[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t2558, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t2559, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1119 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1120 "compiler/main.gem"
    GemVal gem_v__for_i_46 = gem_int(0);
#line 1120 "compiler/main.gem"
    GemVal _t2560[] = {gem_v_c_args};
    GemVal gem_v__for_limit_46 = gem_len_fn(NULL, _t2560, 1);
#line 1120 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
#line 1120 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_46;
#line 1120 "compiler/main.gem"
        gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 1121 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1122 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1124 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1127 "compiler/main.gem"
    GemVal _t2561[] = {gem_v_name};
    GemVal _t2562[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2561, 1), gem_string("(")), gem_to_string_fn(NULL, _t2562, 1)), gem_string(")"));
#line 1129 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1130 "compiler/main.gem"
    GemVal _t2563[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2563, 1)), gem_string(";\n")));
#line 1131 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
#line 1132 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1133 "compiler/main.gem"
    GemVal _t2564[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t2564, 1)), gem_string(";\n")));
        } else {
#line 1134 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1135 "compiler/main.gem"
    GemVal _t2565[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t2565, 1)), gem_string(";\n")));
#line 1136 "compiler/main.gem"
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
#line 1137 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1138 "compiler/main.gem"
    GemVal _t2566[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t2566, 1)), gem_string(";\n")));
#line 1139 "compiler/main.gem"
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
#line 1140 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1141 "compiler/main.gem"
    GemVal _t2567[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t2567, 1)), gem_string(";\n")));
#line 1142 "compiler/main.gem"
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
#line 1143 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1144 "compiler/main.gem"
    GemVal _t2568[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t2568, 1)), gem_string(";\n")));
#line 1145 "compiler/main.gem"
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
#line 1146 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1147 "compiler/main.gem"
    GemVal _t2569[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t2569, 1)), gem_string(";\n")));
#line 1148 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
#line 1150 "compiler/main.gem"
    GemVal _t2570[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2570, 1)), gem_string(";\n")));
#line 1151 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1154 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1157 "compiler/main.gem"
    GemVal _t2571[] = {gem_v_name};
    GemVal _t2572[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2571, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t2572, 2));
#line 1160 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1161 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1162 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1163 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1164 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1165 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1166 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1167 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1168 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1169 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1170 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1171 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1172 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1175 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1176 "compiler/main.gem"
    GemVal _t2573[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2573, 1), gem_int(0)))) {
#line 1177 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1179 "compiler/main.gem"
        GemVal gem_v__for_i_47 = gem_int(0);
#line 1179 "compiler/main.gem"
    GemVal _t2574[] = {gem_v_params};
        GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2574, 1);
#line 1179 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 1179 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_47;
#line 1179 "compiler/main.gem"
            gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 1180 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1181 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1183 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1184 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1185 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1186 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1187 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1188 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1189 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1190 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1191 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1192 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1193 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1194 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1195 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1197 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1202 "compiler/main.gem"
    GemVal _t2575[] = {gem_v_c_ret};
    GemVal _t2576[] = {gem_v_name};
    GemVal _t2577[] = {gem_v_c_params};
    GemVal _t2578[] = {(*gem_v_extern_c_decls), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2575, 1), gem_string(" ")), gem_to_string_fn(NULL, _t2576, 1)), gem_string("(")), gem_to_string_fn(NULL, _t2577, 1)), gem_string(");"))};
    (void)(gem_push_fn(NULL, _t2578, 2));
    GemVal _t2579 = gem_v_lines;
    gem_pop_frame();
    return _t2579;
}

struct _closure__anon_47 {
    GemVal *gem_v_mangle;
};
static GemVal _anon_47(void *_env, GemVal *args, int argc) {
    struct _closure__anon_47 *_cls = (struct _closure__anon_47 *)_env;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_cap_set = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_47", "compiler/main.gem", 0);
#line 1210 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1212 "compiler/main.gem"
    GemVal gem_v__for_i_48 = gem_int(0);
#line 1212 "compiler/main.gem"
    GemVal _t2581[] = {gem_v_params};
    GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t2581, 1);
#line 1212 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 1212 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_48;
#line 1212 "compiler/main.gem"
        gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 1213 "compiler/main.gem"
    GemVal _t2582[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t2583 = (*gem_v_mangle);
        GemVal gem_v_mp = _t2583.fn(_t2583.env, _t2582, 1);
#line 1214 "compiler/main.gem"
    GemVal _t2584[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2584, 2))) {
#line 1215 "compiler/main.gem"
    GemVal _t2585[] = {gem_v_mp};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2585, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1216 "compiler/main.gem"
    GemVal _t2586[] = {gem_v_mp};
    GemVal _t2587[] = {gem_v_i};
    GemVal _t2588[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2586, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2587, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2588, 1)), gem_string("] : GEM_NIL;\n")));
        } else {
#line 1218 "compiler/main.gem"
    GemVal _t2589[] = {gem_v_mp};
    GemVal _t2590[] = {gem_v_i};
    GemVal _t2591[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2589, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t2590, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t2591, 1)), gem_string("] : GEM_NIL;\n")));
        }
    }

#line 1223 "compiler/main.gem"
    GemVal _t2592 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2592, gem_string("rest_param")), GEM_NIL))) {
#line 1224 "compiler/main.gem"
    GemVal _t2593[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t2593, 1);
#line 1225 "compiler/main.gem"
        GemVal gem_v_end_count = gem_int(0);
#line 1226 "compiler/main.gem"
    GemVal _t2594 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2594, gem_string("block_param")), GEM_NIL))) {
#line 1227 "compiler/main.gem"
            gem_v_end_count = gem_int(1);
        }
#line 1229 "compiler/main.gem"
    GemVal _t2595 = gem_v_node;
    GemVal _t2596[] = {gem_table_get(_t2595, gem_string("rest_param"))};
    GemVal _t2597 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t2597.fn(_t2597.env, _t2596, 1);
#line 1230 "compiler/main.gem"
    GemVal _t2598 = gem_v_node;
    GemVal _t2599[] = {gem_v_cap_set, gem_table_get(_t2598, gem_string("rest_param"))};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2599, 2))) {
#line 1231 "compiler/main.gem"
    GemVal _t2600[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2600, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1232 "compiler/main.gem"
    GemVal _t2601[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2601, 1)), gem_string(" = gem_table_new();\n")));
#line 1233 "compiler/main.gem"
    GemVal _t2602[] = {gem_v_N};
    GemVal _t2603[] = {gem_v_end_count};
    GemVal _t2604[] = {gem_v_mp_rest};
    GemVal _t2605[] = {gem_v_N};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t2602, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t2603, 1)), gem_string("; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t2604, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t2605, 1)), gem_string("), args[_ri]);\n")));
        } else {
#line 1235 "compiler/main.gem"
    GemVal _t2606[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2606, 1)), gem_string(" = gem_table_new();\n")));
#line 1236 "compiler/main.gem"
    GemVal _t2607[] = {gem_v_N};
    GemVal _t2608[] = {gem_v_end_count};
    GemVal _t2609[] = {gem_v_mp_rest};
    GemVal _t2610[] = {gem_v_N};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t2607, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t2608, 1)), gem_string("; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t2609, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t2610, 1)), gem_string("), args[_ri]);\n")));
        }
#line 1238 "compiler/main.gem"
    GemVal _t2611 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2611, gem_string("block_param")), GEM_NIL))) {
#line 1239 "compiler/main.gem"
    GemVal _t2612 = gem_v_node;
    GemVal _t2613[] = {gem_table_get(_t2612, gem_string("block_param"))};
    GemVal _t2614 = (*gem_v_mangle);
            GemVal gem_v_mp_block = _t2614.fn(_t2614.env, _t2613, 1);
#line 1240 "compiler/main.gem"
    GemVal _t2615 = gem_v_node;
    GemVal _t2616[] = {gem_v_cap_set, gem_table_get(_t2615, gem_string("block_param"))};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2616, 2))) {
#line 1241 "compiler/main.gem"
    GemVal _t2617[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2617, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1242 "compiler/main.gem"
    GemVal _t2618[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t2618, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            } else {
#line 1244 "compiler/main.gem"
    GemVal _t2619[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2619, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            }
        }
    }
    GemVal _t2620 = gem_v_out;
    gem_pop_frame();
    return _t2620;
}

struct _closure__anon_48 {
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
};
static GemVal _anon_48(void *_env, GemVal *args, int argc) {
    struct _closure__anon_48 *_cls = (struct _closure__anon_48 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal gem_v_body = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_48", "compiler/main.gem", 0);
#line 1254 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1255 "compiler/main.gem"
    GemVal _t2622[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2622, 1), gem_int(0)))) {
#line 1256 "compiler/main.gem"
        GemVal gem_v__for_i_49 = gem_int(0);
#line 1256 "compiler/main.gem"
    GemVal _t2623[] = {gem_v_body};
        GemVal gem_v__for_limit_49 = gem_sub(gem_len_fn(NULL, _t2623, 1), gem_int(1));
#line 1256 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_v__for_limit_49))) break;
#line 1256 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_49;
#line 1256 "compiler/main.gem"
            gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 1257 "compiler/main.gem"
    GemVal _t2624[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t2625 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2625.fn(_t2625.env, _t2624, 2)), gem_string("\n"));
        }

#line 1259 "compiler/main.gem"
    GemVal _t2626[] = {gem_v_body};
    GemVal _t2627[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t2626, 1), gem_int(1))), gem_int(1)};
    GemVal _t2628 = (*gem_v_compile_stmt_return);
        gem_v_out = gem_add(gem_add(gem_v_out, _t2628.fn(_t2628.env, _t2627, 2)), gem_string("\n"));
    } else {
#line 1261 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
    GemVal _t2629 = gem_v_out;
    gem_pop_frame();
    return _t2629;
}

struct _closure__anon_49 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_source_name;
};
static GemVal _anon_49(void *_env, GemVal *args, int argc) {
    struct _closure__anon_49 *_cls = (struct _closure__anon_49 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_emit_fn_body = _cls->gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings = _cls->gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_49", "compiler/main.gem", 0);
#line 1269 "compiler/main.gem"
    GemVal gem_v__d50 = gem_v_node;
#line 1269 "compiler/main.gem"
    GemVal _t2631 = gem_v__d50;
    GemVal gem_v_params = gem_table_get(_t2631, gem_string("params"));
#line 1269 "compiler/main.gem"
    GemVal _t2632 = gem_v__d50;
    GemVal gem_v_body = gem_table_get(_t2632, gem_string("body"));
#line 1269 "compiler/main.gem"
    GemVal _t2633 = gem_v__d50;
    GemVal gem_v_name = gem_table_get(_t2633, gem_string("name"));

#line 1272 "compiler/main.gem"
    GemVal _t2634[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t2634, 1);
#line 1273 "compiler/main.gem"
    GemVal _t2635 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2635, gem_string("rest_param")), GEM_NIL))) {
#line 1274 "compiler/main.gem"
    GemVal _t2636 = gem_v_node;
    GemVal _t2637[] = {gem_v_scope_vars, gem_table_get(_t2636, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t2637, 2));
    }
#line 1276 "compiler/main.gem"
    GemVal _t2638 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2638, gem_string("block_param")), GEM_NIL))) {
#line 1277 "compiler/main.gem"
    GemVal _t2639 = gem_v_node;
    GemVal _t2640[] = {gem_v_scope_vars, gem_table_get(_t2639, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t2640, 2));
    }
#line 1279 "compiler/main.gem"
    GemVal _t2641[] = {gem_v_body, gem_v_scope_vars};
    (void)(gem_fn_collect_top_let_names(NULL, _t2641, 2));
#line 1280 "compiler/main.gem"
    GemVal _t2642[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t2643 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t2643.fn(_t2643.env, _t2642, 2);
#line 1282 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1283 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1284 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1285 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1287 "compiler/main.gem"
    GemVal _t2644[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2644, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1290 "compiler/main.gem"
    GemVal _t2645 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2645, gem_string("line")), GEM_NIL))) {
#line 1291 "compiler/main.gem"
    GemVal _t2646 = gem_v_node;
    GemVal _t2647[] = {gem_table_get(_t2646, gem_string("line"))};
    GemVal _t2648[] = {(*gem_v_source_name)};
    GemVal _t2649[] = {gem_fn_escape_c_string(NULL, _t2648, 1)};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2647, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2649, 1)), gem_string("\"\n")));
    }
#line 1294 "compiler/main.gem"
    GemVal _t2650[] = {gem_v_node, gem_v_params, gem_v_captured};
    GemVal _t2651 = (*gem_v_emit_param_bindings);
    gem_v_lines = gem_add(gem_v_lines, _t2651.fn(_t2651.env, _t2650, 3));
#line 1296 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1297 "compiler/main.gem"
    GemVal _t2652 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2652, gem_string("line")), GEM_NIL))) {
#line 1298 "compiler/main.gem"
    GemVal _t2653 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t2653, gem_string("line"));
    }
#line 1300 "compiler/main.gem"
    GemVal _t2654[] = {gem_v_name};
    GemVal _t2655[] = {gem_fn_escape_c_string(NULL, _t2654, 1)};
    GemVal _t2656[] = {(*gem_v_source_name)};
    GemVal _t2657[] = {gem_fn_escape_c_string(NULL, _t2656, 1)};
    GemVal _t2658[] = {gem_v_fn_line};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2655, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2657, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2658, 1)), gem_string(");\n")));
#line 1302 "compiler/main.gem"
    GemVal _t2659[] = {gem_v_body};
    GemVal _t2660 = (*gem_v_emit_fn_body);
    gem_v_lines = gem_add(gem_v_lines, _t2660.fn(_t2660.env, _t2659, 1));
#line 1304 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1305 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1306 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
    GemVal _t2661 = gem_v_lines;
    gem_pop_frame();
    return _t2661;
}

struct _closure__anon_50 {
    GemVal *gem_v_anon_name;
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_functions;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_mangle;
    GemVal *gem_v_source_name;
};
static GemVal _anon_50(void *_env, GemVal *args, int argc) {
    struct _closure__anon_50 *_cls = (struct _closure__anon_50 *)_env;
    GemVal *gem_v_anon_name = _cls->gem_v_anon_name;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_emit_fn_body = _cls->gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings = _cls->gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_functions = _cls->gem_v_functions;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_captures = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_50", "compiler/main.gem", 0);
#line 1313 "compiler/main.gem"
    GemVal gem_v__d51 = gem_v_node;
#line 1313 "compiler/main.gem"
    GemVal _t2663 = gem_v__d51;
    GemVal gem_v_params = gem_table_get(_t2663, gem_string("params"));
#line 1313 "compiler/main.gem"
    GemVal _t2664 = gem_v__d51;
    GemVal gem_v_body = gem_table_get(_t2664, gem_string("body"));

#line 1314 "compiler/main.gem"
    GemVal _t2665 = (*gem_v_anon_name);
    GemVal gem_v_name = _t2665.fn(_t2665.env, NULL, 0);
#line 1317 "compiler/main.gem"
    GemVal _t2666[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t2666, 1);
#line 1318 "compiler/main.gem"
    GemVal _t2667 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2667, gem_string("rest_param")), GEM_NIL))) {
#line 1319 "compiler/main.gem"
    GemVal _t2668 = gem_v_node;
    GemVal _t2669[] = {gem_v_inner_scope, gem_table_get(_t2668, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t2669, 2));
    }
#line 1321 "compiler/main.gem"
    GemVal _t2670 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2670, gem_string("block_param")), GEM_NIL))) {
#line 1322 "compiler/main.gem"
    GemVal _t2671 = gem_v_node;
    GemVal _t2672[] = {gem_v_inner_scope, gem_table_get(_t2671, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t2672, 2));
    }
#line 1324 "compiler/main.gem"
    GemVal _t2673[] = {gem_v_body, gem_v_inner_scope};
    (void)(gem_fn_collect_top_let_names(NULL, _t2673, 2));
#line 1325 "compiler/main.gem"
    GemVal _t2674[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t2675 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t2675.fn(_t2675.env, _t2674, 2);
#line 1327 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1328 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1329 "compiler/main.gem"
    GemVal _t2676[] = {gem_v_captures};
    GemVal _t2677[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t2676, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t2677, 2);
#line 1330 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1333 "compiler/main.gem"
    GemVal _t2678[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t2678, 1));
#line 1334 "compiler/main.gem"
    GemVal _t2679[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t2679, 1)), gem_string(" {\n"));
#line 1335 "compiler/main.gem"
    GemVal gem_v__for_items_52 = gem_v_captures;
#line 1335 "compiler/main.gem"
    GemVal gem_v__for_i_52 = gem_int(0);
#line 1335 "compiler/main.gem"
    while (1) {
        GemVal _t2680[] = {gem_v__for_items_52};
        if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_len_fn(NULL, _t2680, 1)))) break;
#line 1335 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_52, gem_v__for_i_52);
#line 1335 "compiler/main.gem"
        gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 1336 "compiler/main.gem"
    GemVal _t2681[] = {gem_v_cap};
    GemVal _t2682 = (*gem_v_mangle);
    GemVal _t2683[] = {_t2682.fn(_t2682.env, _t2681, 1)};
        gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2683, 1)), gem_string(";\n")));
    }

#line 1338 "compiler/main.gem"
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
#line 1341 "compiler/main.gem"
    GemVal _t2684[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t2684, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1342 "compiler/main.gem"
    GemVal _t2685[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2685, 1), gem_int(0)))) {
#line 1343 "compiler/main.gem"
    GemVal _t2686[] = {gem_v_struct_name};
    GemVal _t2687[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2686, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t2687, 1)), gem_string(" *)_env;\n")));
#line 1344 "compiler/main.gem"
        GemVal gem_v__for_items_53 = gem_v_captures;
#line 1344 "compiler/main.gem"
        GemVal gem_v__for_i_53 = gem_int(0);
#line 1344 "compiler/main.gem"
        while (1) {
            GemVal _t2688[] = {gem_v__for_items_53};
            if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_len_fn(NULL, _t2688, 1)))) break;
#line 1344 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_53, gem_v__for_i_53);
#line 1344 "compiler/main.gem"
            gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 1345 "compiler/main.gem"
    GemVal _t2689[] = {gem_v_cap};
    GemVal _t2690 = (*gem_v_mangle);
            GemVal gem_v_mc = _t2690.fn(_t2690.env, _t2689, 1);
#line 1346 "compiler/main.gem"
    GemVal _t2691[] = {gem_v_mc};
    GemVal _t2692[] = {gem_v_mc};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t2691, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t2692, 1)), gem_string(";\n")));
        }

    }
#line 1350 "compiler/main.gem"
    GemVal _t2693[] = {gem_v_node, gem_v_params, gem_v_inner_captured};
    GemVal _t2694 = (*gem_v_emit_param_bindings);
    gem_v_lines = gem_add(gem_v_lines, _t2694.fn(_t2694.env, _t2693, 3));
#line 1352 "compiler/main.gem"
    GemVal _t2695[] = {gem_v_name};
    GemVal _t2696[] = {gem_fn_escape_c_string(NULL, _t2695, 1)};
    GemVal _t2697[] = {(*gem_v_source_name)};
    GemVal _t2698[] = {gem_fn_escape_c_string(NULL, _t2697, 1)};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t2696, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t2698, 1)), gem_string("\", 0);\n")));
#line 1354 "compiler/main.gem"
    GemVal _t2699[] = {gem_v_body};
    GemVal _t2700 = (*gem_v_emit_fn_body);
    gem_v_lines = gem_add(gem_v_lines, _t2700.fn(_t2700.env, _t2699, 1));
#line 1356 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1357 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1358 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1361 "compiler/main.gem"
    GemVal _t2701[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t2701, 2));
    GemVal _t2702 = gem_table_new();
    gem_table_set(_t2702, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t2702, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t2702, gem_string("captures"), gem_v_captures);
    GemVal _t2703 = _t2702;
    gem_pop_frame();
    return _t2703;
}

struct _closure__anon_51 {
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
static GemVal _anon_51(void *_env, GemVal *args, int argc) {
    struct _closure__anon_51 *_cls = (struct _closure__anon_51 *)_env;
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
    gem_push_frame("_anon_51", "compiler/main.gem", 0);
#line 1370 "compiler/main.gem"
    GemVal _t2705 = gem_table_new();
    GemVal gem_v_fn_defs = _t2705;
#line 1371 "compiler/main.gem"
    GemVal _t2706 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t2706;
#line 1372 "compiler/main.gem"
    GemVal _t2707 = gem_table_new();
    GemVal gem_v_extern_includes = _t2707;
#line 1373 "compiler/main.gem"
    GemVal _t2708 = gem_table_new();
    GemVal gem_v_top_stmts = _t2708;
#line 1375 "compiler/main.gem"
    GemVal _t2709 = gem_v_ast;
    GemVal gem_v__for_items_54 = gem_table_get(_t2709, gem_string("stmts"));
#line 1375 "compiler/main.gem"
    GemVal gem_v__for_i_54 = gem_int(0);
#line 1375 "compiler/main.gem"
    while (1) {
        GemVal _t2710[] = {gem_v__for_items_54};
        if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_len_fn(NULL, _t2710, 1)))) break;
#line 1375 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_54, gem_v__for_i_54);
#line 1375 "compiler/main.gem"
        gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 1376 "compiler/main.gem"
    GemVal _t2711 = gem_v_s;
        if (gem_truthy(gem_eq(gem_table_get(_t2711, gem_string("tag")), gem_string("fn_def")))) {
#line 1377 "compiler/main.gem"
    GemVal _t2712[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t2712, 2));
        } else {
#line 1378 "compiler/main.gem"
    GemVal _t2713 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t2713, gem_string("tag")), gem_string("extern_fn")))) {
#line 1379 "compiler/main.gem"
    GemVal _t2714[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t2714, 2));
            } else {
#line 1380 "compiler/main.gem"
    GemVal _t2715 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t2715, gem_string("tag")), gem_string("extern_include")))) {
#line 1381 "compiler/main.gem"
    GemVal _t2716[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t2716, 2));
                } else {
#line 1383 "compiler/main.gem"
    GemVal _t2717[] = {gem_v_top_stmts, gem_v_s};
                    (void)(gem_push_fn(NULL, _t2717, 2));
                }
            }
        }
    }

#line 1388 "compiler/main.gem"
    GemVal gem_v__for_items_55 = gem_v_extern_fns_list;
#line 1388 "compiler/main.gem"
    GemVal gem_v__for_i_55 = gem_int(0);
#line 1388 "compiler/main.gem"
    while (1) {
        GemVal _t2718[] = {gem_v__for_items_55};
        if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_len_fn(NULL, _t2718, 1)))) break;
#line 1388 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_55, gem_v__for_i_55);
#line 1388 "compiler/main.gem"
        gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1389 "compiler/main.gem"
    GemVal _t2719 = gem_v_ef;
    GemVal _t2720[] = {(*gem_v_defined_fns), gem_table_get(_t2719, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t2720, 2));
    }

#line 1393 "compiler/main.gem"
    GemVal gem_v__for_items_56 = gem_v_fn_defs;
#line 1393 "compiler/main.gem"
    GemVal gem_v__for_i_56 = gem_int(0);
#line 1393 "compiler/main.gem"
    while (1) {
        GemVal _t2721[] = {gem_v__for_items_56};
        if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_len_fn(NULL, _t2721, 1)))) break;
#line 1393 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_56, gem_v__for_i_56);
#line 1393 "compiler/main.gem"
        gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1394 "compiler/main.gem"
    GemVal _t2722 = gem_v_fd;
    GemVal _t2723[] = {(*gem_v_defined_fns), gem_table_get(_t2722, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t2723, 2));
#line 1395 "compiler/main.gem"
    GemVal _t2724 = gem_v_fd;
    GemVal _t2725[] = {gem_table_get(_t2724, gem_string("name"))};
        GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2725, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1396 "compiler/main.gem"
    GemVal _t2726[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t2726, 2));
    }

#line 1400 "compiler/main.gem"
    GemVal _t2727[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn_collect_top_let_names(NULL, _t2727, 2));
#line 1401 "compiler/main.gem"
    GemVal _t2728 = gem_table_new();
    GemVal _t2729[] = {gem_v_top_stmts, _t2728};
    GemVal _t2730 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t2730.fn(_t2730.env, _t2729, 2);
#line 1404 "compiler/main.gem"
    GemVal gem_v__for_items_57 = gem_v_extern_fns_list;
#line 1404 "compiler/main.gem"
    GemVal gem_v__for_i_57 = gem_int(0);
#line 1404 "compiler/main.gem"
    while (1) {
        GemVal _t2731[] = {gem_v__for_items_57};
        if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t2731, 1)))) break;
#line 1404 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
#line 1404 "compiler/main.gem"
        gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
#line 1405 "compiler/main.gem"
    GemVal _t2732[] = {gem_v_ef};
    GemVal _t2733 = (*gem_v_compile_extern_fn);
    GemVal _t2734[] = {(*gem_v_functions), _t2733.fn(_t2733.env, _t2732, 1)};
        (void)(gem_push_fn(NULL, _t2734, 2));
    }

#line 1409 "compiler/main.gem"
    GemVal gem_v__for_items_58 = gem_v_fn_defs;
#line 1409 "compiler/main.gem"
    GemVal gem_v__for_i_58 = gem_int(0);
#line 1409 "compiler/main.gem"
    while (1) {
        GemVal _t2735[] = {gem_v__for_items_58};
        if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t2735, 1)))) break;
#line 1409 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1409 "compiler/main.gem"
        gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1410 "compiler/main.gem"
    GemVal _t2736[] = {gem_v_fd};
    GemVal _t2737 = (*gem_v_compile_fn);
    GemVal _t2738[] = {(*gem_v_functions), _t2737.fn(_t2737.env, _t2736, 1)};
        (void)(gem_push_fn(NULL, _t2738, 2));
    }

#line 1414 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1415 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1416 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1417 "compiler/main.gem"
    GemVal _t2739[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t2740 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t2740.fn(_t2740.env, _t2739, 2);
#line 1418 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1419 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1421 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 1422 "compiler/main.gem"
    GemVal gem_v__for_items_59 = gem_v_fn_defs;
#line 1422 "compiler/main.gem"
    GemVal gem_v__for_i_59 = gem_int(0);
#line 1422 "compiler/main.gem"
    while (1) {
        GemVal _t2741[] = {gem_v__for_items_59};
        if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_len_fn(NULL, _t2741, 1)))) break;
#line 1422 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_59, gem_v__for_i_59);
#line 1422 "compiler/main.gem"
        gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1423 "compiler/main.gem"
    GemVal _t2742 = gem_v_fd;
        if (gem_truthy(gem_eq(gem_table_get(_t2742, gem_string("name")), gem_string("main")))) {
#line 1424 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 1428 "compiler/main.gem"
    GemVal gem_v_main_c = gem_string("int main(void) {\n");
#line 1429 "compiler/main.gem"
    GemVal _t2743[] = {(*gem_v_source_name)};
    GemVal _t2744[] = {gem_fn_escape_c_string(NULL, _t2743, 1)};
    gem_v_main_c = gem_add(gem_v_main_c, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t2744, 1)), gem_string("\", 0);\n")));
#line 1430 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1431 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
#line 1433 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1434 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
#line 1435 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
#line 1437 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_run_scheduler();\n"));
#line 1438 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_pop_frame();\n    return 0;\n}\n"));
#line 1442 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1443 "compiler/main.gem"
    GemVal gem_v__for_items_60 = gem_v_extern_includes;
#line 1443 "compiler/main.gem"
    GemVal gem_v__for_i_60 = gem_int(0);
#line 1443 "compiler/main.gem"
    while (1) {
        GemVal _t2745[] = {gem_v__for_items_60};
        if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_len_fn(NULL, _t2745, 1)))) break;
#line 1443 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_60, gem_v__for_i_60);
#line 1443 "compiler/main.gem"
        gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1444 "compiler/main.gem"
    GemVal _t2746 = gem_v_ei;
    GemVal _t2747[] = {gem_table_get(_t2746, gem_string("path"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t2747, 1)), gem_string("\"\n")));
    }

#line 1446 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
#line 1450 "compiler/main.gem"
    GemVal _t2748[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2748, 1), gem_int(0)))) {
#line 1451 "compiler/main.gem"
        GemVal gem_v__for_i_61 = gem_int(0);
#line 1451 "compiler/main.gem"
    GemVal _t2749[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_61 = gem_len_fn(NULL, _t2749, 1);
#line 1451 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_v__for_limit_61))) break;
#line 1451 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_61;
#line 1451 "compiler/main.gem"
            gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1452 "compiler/main.gem"
            gem_v_out = gem_add(gem_add(gem_v_out, gem_table_get((*gem_v_extern_c_decls), gem_v_i)), gem_string("\n"));
        }

    }
#line 1457 "compiler/main.gem"
    GemVal gem_v__for_i_62 = gem_int(0);
#line 1457 "compiler/main.gem"
    GemVal _t2750[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_62 = gem_len_fn(NULL, _t2750, 1);
#line 1457 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
#line 1457 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_62;
#line 1457 "compiler/main.gem"
        gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1458 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
#line 1459 "compiler/main.gem"
    GemVal _t2751[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2751, 1), gem_int(1))))) {
#line 1460 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1463 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1466 "compiler/main.gem"
    GemVal _t2752[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t2752, 1);
#line 1467 "compiler/main.gem"
    GemVal gem_v__for_i_63 = gem_int(0);
#line 1467 "compiler/main.gem"
    GemVal _t2753[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_63 = gem_len_fn(NULL, _t2753, 1);
#line 1467 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_v__for_limit_63))) break;
#line 1467 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_63;
#line 1467 "compiler/main.gem"
        gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1468 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 1469 "compiler/main.gem"
    GemVal _t2754[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2754, 2))) {
#line 1470 "compiler/main.gem"
    GemVal _t2755[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal *gem_v_"), gem_to_string_fn(NULL, _t2755, 1)), gem_string(";\n")));
        } else {
#line 1472 "compiler/main.gem"
    GemVal _t2756[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal gem_v_"), gem_to_string_fn(NULL, _t2756, 1)), gem_string(";\n")));
        }
    }

#line 1475 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1478 "compiler/main.gem"
    GemVal gem_v__for_i_64 = gem_int(0);
#line 1478 "compiler/main.gem"
    GemVal _t2757[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_64 = gem_len_fn(NULL, _t2757, 1);
#line 1478 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_v__for_limit_64))) break;
#line 1478 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_64;
#line 1478 "compiler/main.gem"
        gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1479 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
#line 1480 "compiler/main.gem"
    GemVal _t2758[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2758, 1), gem_int(1))))) {
#line 1481 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
        }
    }

#line 1484 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
#line 1486 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    GemVal _t2759 = gem_v_out;
    gem_pop_frame();
    return _t2759;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 140 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_codegen", "compiler/main.gem", 140);
#line 141 "compiler/main.gem"
    GemVal _t1236 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1236;
#line 142 "compiler/main.gem"
    GemVal _t1237 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1237;
#line 143 "compiler/main.gem"
    GemVal _t1238 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1238;
#line 144 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 145 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 146 "compiler/main.gem"
    GemVal _t1239 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1239;
#line 147 "compiler/main.gem"
    GemVal _t1240 = gem_table_new();
    gem_table_set(_t1240, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1240, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1240, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1240, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1240, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1240, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1240, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1240, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1240, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1240, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1240, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1240, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1240, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1240, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1240, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1240, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1240, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1240, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1240, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1240, gem_string("self"), gem_string("gem_self_builtin"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1240;
#line 169 "compiler/main.gem"
    GemVal _t1241 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1241;
#line 170 "compiler/main.gem"
    GemVal gem_v__for_tbl_10 = (*gem_v_builtin_fns);
#line 170 "compiler/main.gem"
    GemVal _t1242[] = {gem_v__for_tbl_10};
    GemVal gem_v__for_keys_10 = gem_keys_fn(NULL, _t1242, 1);
#line 170 "compiler/main.gem"
    GemVal gem_v__for_i_10 = gem_int(0);
#line 170 "compiler/main.gem"
    while (1) {
        GemVal _t1243[] = {gem_v__for_keys_10};
        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t1243, 1)))) break;
#line 170 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_10, gem_v__for_i_10);
#line 170 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_10, gem_table_get(gem_v__for_keys_10, gem_v__for_i_10));
#line 170 "compiler/main.gem"
        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 171 "compiler/main.gem"
        gem_table_set((*gem_v_builtins), gem_v_k, gem_bool(1));
    }

#line 173 "compiler/main.gem"
    GemVal _t1244 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1244;
#line 174 "compiler/main.gem"
    GemVal _t1245 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1245;
#line 175 "compiler/main.gem"
    GemVal _t1246 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1246;
#line 176 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 180 "compiler/main.gem"
    struct _closure__anon_22 *_t1249 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1249->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_22, _t1249);
#line 185 "compiler/main.gem"
    struct _closure__anon_23 *_t1252 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1252->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_23, _t1252);
#line 190 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_24, NULL);
#line 196 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 197 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 199 "compiler/main.gem"
    struct _closure__anon_25 *_t1427 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1427->gem_v_builtins = gem_v_builtins;
    _t1427->gem_v_collect_free_node = gem_v_collect_free_node;
    _t1427->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1427->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_25, _t1427);
#line 336 "compiler/main.gem"
    struct _closure__anon_26 *_t1441 = GC_MALLOC(sizeof(struct _closure__anon_26));
    _t1441->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_26, _t1441);
#line 351 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 352 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 354 "compiler/main.gem"
    struct _closure__anon_27 *_t1533 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1533->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1533->gem_v_walk_captures = gem_v_walk_captures;
    _t1533->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_27, _t1533);
#line 433 "compiler/main.gem"
    struct _closure__anon_28 *_t1545 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1545->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_28, _t1545);
#line 444 "compiler/main.gem"
    struct _closure__anon_29 *_t1550 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t1550->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_29, _t1550);
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
    struct _closure__anon_30 *_t1679 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t1679->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1679->gem_v_builtin_fns = gem_v_builtin_fns;
    _t1679->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t1679->gem_v_compile_array = gem_v_compile_array;
    _t1679->gem_v_compile_binop = gem_v_compile_binop;
    _t1679->gem_v_compile_call = gem_v_compile_call;
    _t1679->gem_v_compile_expr = gem_v_compile_expr;
    _t1679->gem_v_compile_table = gem_v_compile_table;
    _t1679->gem_v_defined_fns = gem_v_defined_fns;
    _t1679->gem_v_mangle = gem_v_mangle;
    _t1679->gem_v_tmp = gem_v_tmp;
    _t1679->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_30, _t1679);
#line 560 "compiler/main.gem"
    struct _closure__anon_31 *_t1696 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t1696->gem_v_compile_expr = gem_v_compile_expr;
    _t1696->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_31, _t1696);
#line 575 "compiler/main.gem"
    struct _closure__anon_32 *_t1710 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t1710->gem_v_compile_expr = gem_v_compile_expr;
    _t1710->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_32, _t1710);
#line 589 "compiler/main.gem"
    struct _closure__anon_33 *_t1754 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t1754->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1754->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1754->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t1754->gem_v_mangle = gem_v_mangle;
    _t1754->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_33, _t1754);
#line 624 "compiler/main.gem"
    struct _closure__anon_34 *_t1770 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t1770->gem_v_compile_expr = gem_v_compile_expr;
    _t1770->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_34, _t1770);
#line 645 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_35, NULL);
#line 654 "compiler/main.gem"
    struct _closure__anon_36 *_t1858 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t1858->gem_v_builtin_fns = gem_v_builtin_fns;
    _t1858->gem_v_compile_args = gem_v_compile_args;
    _t1858->gem_v_compile_expr = gem_v_compile_expr;
    _t1858->gem_v_defined_fns = gem_v_defined_fns;
    _t1858->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t1858->gem_v_source_name = gem_v_source_name;
    _t1858->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_36, _t1858);
#line 698 "compiler/main.gem"
    struct _closure__anon_37 *_t2011 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t2011->gem_v_compile_expr = gem_v_compile_expr;
    _t2011->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_37, _t2011);
#line 777 "compiler/main.gem"
    struct _closure__anon_38 *_t2016 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2016->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_38, _t2016);
#line 787 "compiler/main.gem"
    struct _closure__anon_39 *_t2197 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2197->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2197->gem_v_compile_expr = gem_v_compile_expr;
    _t2197->gem_v_compile_if = gem_v_compile_if;
    _t2197->gem_v_compile_match = gem_v_compile_match;
    _t2197->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2197->gem_v_compile_while = gem_v_compile_while;
    _t2197->gem_v_in_top_level = gem_v_in_top_level;
    _t2197->gem_v_mangle = gem_v_mangle;
    _t2197->gem_v_source_name = gem_v_source_name;
    _t2197->gem_v_tmp = gem_v_tmp;
    _t2197->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t2197->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_39, _t2197);
#line 863 "compiler/main.gem"
    struct _closure__anon_40 *_t2346 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t2346->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2346->gem_v_compile_expr = gem_v_compile_expr;
    _t2346->gem_v_compile_if_return = gem_v_compile_if_return;
    _t2346->gem_v_compile_match_return = gem_v_compile_match_return;
    _t2346->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2346->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2346->gem_v_compile_while = gem_v_compile_while;
    _t2346->gem_v_mangle = gem_v_mangle;
    _t2346->gem_v_source_name = gem_v_source_name;
    _t2346->gem_v_tmp = gem_v_tmp;
    _t2346->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_40, _t2346);
#line 922 "compiler/main.gem"
    struct _closure__anon_41 *_t2365 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2365->gem_v_compile_expr = gem_v_compile_expr;
    _t2365->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_41, _t2365);
#line 938 "compiler/main.gem"
    struct _closure__anon_42 *_t2401 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t2401->gem_v_compile_expr = gem_v_compile_expr;
    _t2401->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2401->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_42, _t2401);
#line 973 "compiler/main.gem"
    struct _closure__anon_43 *_t2419 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t2419->gem_v_compile_expr = gem_v_compile_expr;
    _t2419->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_43, _t2419);
#line 988 "compiler/main.gem"
    struct _closure__anon_44 *_t2461 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t2461->gem_v_compile_expr = gem_v_compile_expr;
    _t2461->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2461->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_44, _t2461);
#line 1023 "compiler/main.gem"
    struct _closure__anon_45 *_t2529 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t2529->gem_v_compile_expr = gem_v_compile_expr;
    _t2529->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2529->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2529->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_45, _t2529);
#line 1087 "compiler/main.gem"
    struct _closure__anon_46 *_t2580 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t2580->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t2580->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_46, _t2580);
#line 1209 "compiler/main.gem"
    struct _closure__anon_47 *_t2621 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t2621->gem_v_mangle = gem_v_mangle;
    GemVal *gem_v_emit_param_bindings = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_param_bindings = gem_make_fn(_anon_47, _t2621);
#line 1253 "compiler/main.gem"
    struct _closure__anon_48 *_t2630 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t2630->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2630->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_fn_body = gem_make_fn(_anon_48, _t2630);
#line 1268 "compiler/main.gem"
    struct _closure__anon_49 *_t2662 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t2662->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2662->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t2662->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t2662->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2662->gem_v_in_top_level = gem_v_in_top_level;
    _t2662->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_fn = gem_make_fn(_anon_49, _t2662);
#line 1312 "compiler/main.gem"
    struct _closure__anon_50 *_t2704 = GC_MALLOC(sizeof(struct _closure__anon_50));
    _t2704->gem_v_anon_name = gem_v_anon_name;
    _t2704->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2704->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t2704->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t2704->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2704->gem_v_functions = gem_v_functions;
    _t2704->gem_v_in_top_level = gem_v_in_top_level;
    _t2704->gem_v_mangle = gem_v_mangle;
    _t2704->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_50, _t2704);
#line 1368 "compiler/main.gem"
    struct _closure__anon_51 *_t2760 = GC_MALLOC(sizeof(struct _closure__anon_51));
    _t2760->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2760->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t2760->gem_v_compile_fn = gem_v_compile_fn;
    _t2760->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2760->gem_v_defined_fns = gem_v_defined_fns;
    _t2760->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t2760->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2760->gem_v_forward_decls = gem_v_forward_decls;
    _t2760->gem_v_functions = gem_v_functions;
    _t2760->gem_v_in_top_level = gem_v_in_top_level;
    _t2760->gem_v_source_name = gem_v_source_name;
    _t2760->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t2760->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_51, _t2760);
    GemVal _t2761 = gem_table_new();
    gem_table_set(_t2761, gem_string("compile"), gem_v_compile);
    GemVal _t2762 = _t2761;
    gem_pop_frame();
    return _t2762;
}

static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc) {
#line 1496 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("str_replace", "compiler/main.gem", 1496);
#line 1497 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 1498 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 1499 "compiler/main.gem"
    GemVal _t2763[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t2763, 1);
#line 1500 "compiler/main.gem"
    GemVal _t2764[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t2764, 1);
#line 1501 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
#line 1502 "compiler/main.gem"
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
#line 1503 "compiler/main.gem"
            GemVal gem_v_found = gem_bool(1);
#line 1504 "compiler/main.gem"
            GemVal gem_v_j = gem_int(0);
#line 1505 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
#line 1506 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
#line 1507 "compiler/main.gem"
                    gem_v_found = gem_bool(0);
                    break;
                }
#line 1510 "compiler/main.gem"
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
#line 1512 "compiler/main.gem"
            if (gem_truthy(gem_v_found)) {
#line 1513 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
#line 1514 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
#line 1518 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
#line 1519 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t2765 = gem_v_result;
    gem_pop_frame();
    return _t2765;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 24 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 24);
#line 25 "compiler/main.gem"
    GemVal _t2766 = gem_table_new();
    GemVal gem_v_new_stmts = _t2766;
#line 26 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 27 "compiler/main.gem"
    GemVal _t2767 = gem_v_ast;
    GemVal gem_v__for_items_1 = gem_table_get(_t2767, gem_string("stmts"));
#line 27 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 27 "compiler/main.gem"
    while (1) {
        GemVal _t2768[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t2768, 1)))) break;
#line 27 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 27 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 28 "compiler/main.gem"
    GemVal _t2769[] = {gem_v_stmt};
    GemVal _t2771;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2769, 1), gem_string("table")))) {
        _t2771 = gem_eq(gem_type_fn(NULL, _t2769, 1), gem_string("table"));
    } else {
        GemVal _t2770 = gem_v_stmt;
        _t2771 = gem_eq(gem_table_get(_t2770, gem_string("tag")), gem_string("load"));
    }
        if (gem_truthy(_t2771)) {
#line 29 "compiler/main.gem"
    GemVal _t2772 = gem_v_stmt;
            GemVal gem_v_path = gem_table_get(_t2772, gem_string("path"));
#line 31 "compiler/main.gem"
    GemVal _t2773[] = {gem_v_path};
    GemVal _t2781;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t2773, 1), gem_int(4)))) {
        _t2781 = gem_lt(gem_len_fn(NULL, _t2773, 1), gem_int(4));
    } else {
        GemVal _t2774[] = {gem_v_path};
        GemVal _t2776;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2774, 1), gem_int(4))), gem_string(".")))) {
                _t2776 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2774, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t2775[] = {gem_v_path};
                _t2776 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2775, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t2778;
        if (gem_truthy(_t2776)) {
                _t2778 = _t2776;
        } else {
                GemVal _t2777[] = {gem_v_path};
                _t2778 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2777, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t2780;
        if (gem_truthy(_t2778)) {
                _t2780 = _t2778;
        } else {
                GemVal _t2779[] = {gem_v_path};
                _t2780 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2779, 1), gem_int(1))), gem_string("m"));
        }
        _t2781 = _t2780;
    }
            if (gem_truthy(_t2781)) {
#line 32 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 36 "compiler/main.gem"
    GemVal _t2782[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_fn_gem_path_join(NULL, _t2782, 2);
#line 39 "compiler/main.gem"
    GemVal _t2783[] = {gem_v_full_path};
            if (gem_truthy(gem_eq(gem_fn_gem_file_exists(NULL, _t2783, 1), gem_int(0)))) {
#line 40 "compiler/main.gem"
    GemVal _t2784[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_fn_gem_path_join(NULL, _t2784, 2);
            }
#line 43 "compiler/main.gem"
    GemVal _t2785[] = {gem_v_full_path};
            gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t2785, 1);
#line 46 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                continue;
            }
#line 49 "compiler/main.gem"
            gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 51 "compiler/main.gem"
    GemVal _t2786[] = {gem_v_full_path};
            GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2786, 1);
#line 52 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 53 "compiler/main.gem"
    GemVal _t2787[] = {gem_v_full_path};
    GemVal _t2788[] = {gem_add(gem_add(gem_string("cannot load '"), gem_to_string_fn(NULL, _t2787, 1)), gem_string("'"))};
                (void)(gem_error_at_fn("compiler/main.gem", 53, _t2788, 1));
            }
#line 56 "compiler/main.gem"
    GemVal _t2789[] = {gem_v_source};
            GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t2789, 1);
#line 57 "compiler/main.gem"
    GemVal _t2790[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t2790, 1);
#line 58 "compiler/main.gem"
    GemVal _t2791[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t2791, 3);
#line 60 "compiler/main.gem"
    GemVal _t2792 = gem_v_resolved;
            GemVal gem_v__for_items_2 = gem_table_get(_t2792, gem_string("stmts"));
#line 60 "compiler/main.gem"
            GemVal gem_v__for_i_2 = gem_int(0);
#line 60 "compiler/main.gem"
            while (1) {
                GemVal _t2793[] = {gem_v__for_items_2};
                if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t2793, 1)))) break;
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
    GemVal _t2794[] = {gem_v_new_stmts};
    GemVal _t2795 = gem_fn_make_program(NULL, _t2794, 1);
    gem_pop_frame();
    return _t2795;
}

int main(void) {
    gem_push_frame("main", "compiler/main.gem", 0);
#line 20 "compiler/main.gem"
    GemVal _t2796[] = {gem_int(0)};
    GemVal _t2797[] = {gem_fn_gem_get_argv(NULL, _t2796, 1)};
    GemVal _t2798[] = {gem_fn_gem_dirname(NULL, _t2797, 1)};
    gem_v_install_root = gem_fn_gem_dirname(NULL, _t2798, 1);
#line 74 "compiler/main.gem"
    gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
#line 75 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 76 "compiler/main.gem"
    GemVal _t2799[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 76, _t2799, 1));
    }
#line 79 "compiler/main.gem"
    GemVal _t2800[] = {gem_int(1)};
    gem_v_src_path = gem_fn_gem_get_argv(NULL, _t2800, 1);
#line 80 "compiler/main.gem"
    GemVal _t2801[] = {gem_v_src_path};
    gem_v_source = gem_fn_gem_read_file(NULL, _t2801, 1);
#line 81 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 82 "compiler/main.gem"
    GemVal _t2802[] = {gem_v_src_path};
    GemVal _t2803[] = {gem_add(gem_add(gem_string("cannot read '"), gem_to_string_fn(NULL, _t2802, 1)), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 82, _t2803, 1));
    }
#line 85 "compiler/main.gem"
    GemVal _t2804[] = {gem_v_source};
    gem_v_ast = gem_fn_parse_source(NULL, _t2804, 1);
#line 86 "compiler/main.gem"
    GemVal _t2805[] = {gem_v_src_path};
    gem_v_base_dir = gem_fn_gem_dirname(NULL, _t2805, 1);
#line 87 "compiler/main.gem"
    GemVal _t2806 = gem_table_new();
    gem_v_loaded = _t2806;
#line 88 "compiler/main.gem"
    GemVal _t2807[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t2807, 3);
#line 90 "compiler/main.gem"
    GemVal _t2808[] = {gem_v_src_path};
    gem_v_cg = gem_fn_make_codegen(NULL, _t2808, 1);
#line 91 "compiler/main.gem"
    GemVal _t2809 = gem_v_cg;
    GemVal _t2810[] = {gem_v_resolved_ast};
    GemVal _t2811 = gem_table_get(_t2809, gem_string("compile"));
    gem_v_c_code = _t2811.fn(_t2811.env, _t2810, 1);
#line 92 "compiler/main.gem"
    GemVal _t2812[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t2812, 1));
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

