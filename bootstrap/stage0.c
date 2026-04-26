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
    GemVal gem_v_target_var = (argc > 4) ? args[4] : GEM_NIL;
    gem_push_frame("make_match", "compiler/main.gem", 108);
    GemVal _t49 = gem_table_new();
    gem_table_set(_t49, gem_string("tag"), gem_string("match"));
    gem_table_set(_t49, gem_string("target"), gem_v_target);
    gem_table_set(_t49, gem_string("whens"), gem_v_whens);
    gem_table_set(_t49, gem_string("else"), gem_v_else_body);
    gem_table_set(_t49, gem_string("line"), gem_v_line);
    gem_table_set(_t49, gem_string("target_var"), gem_v_target_var);
    GemVal _t50 = _t49;
    gem_pop_frame();
    return _t50;
}

static GemVal gem_fn_make_when(void *_env, GemVal *args, int argc) {
#line 112 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_bindings = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_when", "compiler/main.gem", 112);
    GemVal _t51 = gem_table_new();
    gem_table_set(_t51, gem_string("tag"), gem_string("when_clause"));
    gem_table_set(_t51, gem_string("value"), gem_v_value);
    gem_table_set(_t51, gem_string("body"), gem_v_body);
    gem_table_set(_t51, gem_string("bindings"), gem_v_bindings);
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
    GemVal gem_v__match_1 = gem_v_ch;
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("0")))) {
        GemVal _t78 = gem_int(0);
        gem_pop_frame();
        return _t78;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("1")))) {
        GemVal _t79 = gem_int(1);
        gem_pop_frame();
        return _t79;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("2")))) {
        GemVal _t80 = gem_int(2);
        gem_pop_frame();
        return _t80;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("3")))) {
        GemVal _t81 = gem_int(3);
        gem_pop_frame();
        return _t81;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("4")))) {
        GemVal _t82 = gem_int(4);
        gem_pop_frame();
        return _t82;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("5")))) {
        GemVal _t83 = gem_int(5);
        gem_pop_frame();
        return _t83;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("6")))) {
        GemVal _t84 = gem_int(6);
        gem_pop_frame();
        return _t84;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("7")))) {
        GemVal _t85 = gem_int(7);
        gem_pop_frame();
        return _t85;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("8")))) {
        GemVal _t86 = gem_int(8);
        gem_pop_frame();
        return _t86;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("9")))) {
        GemVal _t87 = gem_int(9);
        gem_pop_frame();
        return _t87;
    } else {
#line 50 "compiler/main.gem"
    GemVal _t88[] = {gem_v_ch};
    GemVal _t89[] = {gem_add(gem_string("not a digit: "), gem_to_string_fn(NULL, _t88, 1))};
        GemVal _t90 = gem_error_at_fn("compiler/main.gem", 50, _t89, 1);
        gem_pop_frame();
        return _t90;
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

static GemVal gem_fn_str_to_int(void *_env, GemVal *args, int argc) {
#line 54 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("str_to_int", "compiler/main.gem", 54);
#line 55 "compiler/main.gem"
    GemVal gem_v_result = gem_int(0);
#line 56 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 56 "compiler/main.gem"
    GemVal _t91[] = {gem_v_s};
    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t91, 1);
#line 56 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 56 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_2;
#line 56 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 57 "compiler/main.gem"
    GemVal _t92[] = {gem_table_get(gem_v_s, gem_v_i)};
        gem_v_result = gem_add(gem_mul(gem_v_result, gem_int(10)), gem_fn_digit_val(NULL, _t92, 1));
    }

    GemVal _t93 = gem_v_result;
    gem_pop_frame();
    return _t93;
}

static GemVal gem_fn_contains_dot(void *_env, GemVal *args, int argc) {
#line 64 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("contains_dot", "compiler/main.gem", 64);
#line 65 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 65 "compiler/main.gem"
    GemVal _t94[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t94, 1);
#line 65 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 65 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_3;
#line 65 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 66 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_string(".")))) {
#line 67 "compiler/main.gem"
            GemVal _t95 = gem_bool(1);
            gem_pop_frame();
            return _t95;
        }
    }

    GemVal _t96 = gem_bool(0);
    gem_pop_frame();
    return _t96;
}

static GemVal gem_fn_apply_escape(void *_env, GemVal *args, int argc) {
#line 78 "compiler/main.gem"
    GemVal gem_v_esc = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_mode = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("apply_escape", "compiler/main.gem", 78);
#line 79 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 80 "compiler/main.gem"
        GemVal _t97 = gem_string("\n");
        gem_pop_frame();
        return _t97;
    } else {
#line 81 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 82 "compiler/main.gem"
    GemVal _t98[] = {gem_int(13)};
            GemVal _t99 = gem_chr_fn(NULL, _t98, 1);
            gem_pop_frame();
            return _t99;
        } else {
#line 83 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 84 "compiler/main.gem"
                GemVal _t100 = gem_string("\t");
                gem_pop_frame();
                return _t100;
            } else {
#line 85 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 86 "compiler/main.gem"
                    GemVal _t101 = gem_string("\\");
                    gem_pop_frame();
                    return _t101;
                } else {
#line 87 "compiler/main.gem"
    GemVal _t102;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
        _t102 = gem_eq(gem_v_esc, gem_string("0"));
    } else {
        _t102 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                    if (gem_truthy(_t102)) {
#line 88 "compiler/main.gem"
    GemVal _t103[] = {gem_int(0)};
                        GemVal _t104 = gem_chr_fn(NULL, _t103, 1);
                        gem_pop_frame();
                        return _t104;
                    } else {
#line 89 "compiler/main.gem"
    GemVal _t105;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
        _t105 = gem_eq(gem_v_esc, gem_string("\""));
    } else {
        _t105 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                        if (gem_truthy(_t105)) {
#line 90 "compiler/main.gem"
                            GemVal _t106 = gem_string("\"");
                            gem_pop_frame();
                            return _t106;
                        } else {
#line 91 "compiler/main.gem"
    GemVal _t107;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
        _t107 = gem_eq(gem_v_esc, gem_string("{"));
    } else {
        _t107 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                            if (gem_truthy(_t107)) {
#line 92 "compiler/main.gem"
                                GemVal _t108 = gem_string("{");
                                gem_pop_frame();
                                return _t108;
                            } else {
#line 93 "compiler/main.gem"
    GemVal _t109;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
        _t109 = gem_eq(gem_v_esc, gem_string("}"));
    } else {
        _t109 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                                if (gem_truthy(_t109)) {
#line 94 "compiler/main.gem"
                                    GemVal _t110 = gem_string("}");
                                    gem_pop_frame();
                                    return _t110;
                                } else {
#line 95 "compiler/main.gem"
    GemVal _t111;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("'")))) {
        _t111 = gem_eq(gem_v_esc, gem_string("'"));
    } else {
        _t111 = gem_eq(gem_v_mode, gem_string("sq"));
    }
                                    if (gem_truthy(_t111)) {
#line 96 "compiler/main.gem"
                                        GemVal _t112 = gem_string("'");
                                        gem_pop_frame();
                                        return _t112;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t113 = GEM_NIL;
    gem_pop_frame();
    return _t113;
}

static GemVal gem_fn_build_keyword_set(void *_env, GemVal *args, int argc) {
#line 103 "compiler/main.gem"
    gem_push_frame("build_keyword_set", "compiler/main.gem", 103);
#line 104 "compiler/main.gem"
    GemVal _t114 = gem_table_new();
    GemVal gem_v_t = _t114;
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
    GemVal _t115 = gem_v_t;
    gem_pop_frame();
    return _t115;
}

static GemVal gem_fn_tokenize(void *_env, GemVal *args, int argc) {
#line 131 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("tokenize", "compiler/main.gem", 131);
#line 132 "compiler/main.gem"
    GemVal gem_v_kw = gem_fn_build_keyword_set(NULL, NULL, 0);
#line 133 "compiler/main.gem"
    GemVal _t116 = gem_table_new();
    GemVal gem_v_tokens = _t116;
#line 134 "compiler/main.gem"
    GemVal gem_v_pos = gem_int(0);
#line 135 "compiler/main.gem"
    GemVal gem_v_line = gem_int(1);
#line 136 "compiler/main.gem"
    GemVal _t117[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t117, 1);
#line 137 "compiler/main.gem"
    GemVal gem_v_interp_depth = gem_int(0);
#line 138 "compiler/main.gem"
    GemVal _t118 = gem_table_new();
    GemVal gem_v_interp_brace = _t118;
#line 139 "compiler/main.gem"
    GemVal _t119[] = {gem_int(13)};
    GemVal gem_v_CR = gem_chr_fn(NULL, _t119, 1);
#line 140 "compiler/main.gem"
    GemVal _t120[] = {gem_int(0)};
    GemVal gem_v_NUL = gem_chr_fn(NULL, _t120, 1);
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
    GemVal _t121;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t121 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t121 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t121)) {
#line 149 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 154 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 155 "compiler/main.gem"
            while (1) {
                GemVal _t122;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t122 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t122 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t122)) break;
#line 156 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 162 "compiler/main.gem"
    GemVal _t123;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t123 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t123 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t123)) {
#line 163 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 164 "compiler/main.gem"
    GemVal _t124[] = {gem_v_tokens};
    GemVal _t127;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t124, 1), gem_int(0)))) {
        _t127 = gem_gt(gem_len_fn(NULL, _t124, 1), gem_int(0));
    } else {
        GemVal _t125[] = {gem_v_tokens};
        GemVal _t126 = gem_table_get(gem_v_tokens, gem_sub(gem_len_fn(NULL, _t125, 1), gem_int(1)));
        _t127 = gem_eq(gem_table_get(_t126, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t127)) {
#line 165 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 167 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 168 "compiler/main.gem"
    GemVal _t128 = gem_table_new();
    gem_table_set(_t128, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t128, gem_string("value"), gem_string("\n"));
    gem_table_set(_t128, gem_string("line"), gem_v_line);
    GemVal _t129[] = {gem_v_tokens, _t128};
                (void)(gem_push_fn(NULL, _t129, 2));
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
    GemVal _t130;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t130 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t130 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t131;
    if (gem_truthy(_t130)) {
        _t131 = _t130;
    } else {
        _t131 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t131)) {
#line 181 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 182 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 183 "compiler/main.gem"
                            while (1) {
                                GemVal _t132;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t132 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t132 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t132)) break;
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
    GemVal _t133[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t133, 1))) {
#line 195 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 196 "compiler/main.gem"
            while (1) {
                GemVal _t135;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t135 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t134[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t135 = gem_fn_is_digit(NULL, _t134, 1);
                }
                if (!gem_truthy(_t135)) break;
#line 197 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 198 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 200 "compiler/main.gem"
    GemVal _t136;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t136 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t136 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t136;
#line 201 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 202 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 203 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 204 "compiler/main.gem"
                while (1) {
                    GemVal _t138;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t138 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t137[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t138 = gem_fn_is_digit(NULL, _t137, 1);
                    }
                    if (!gem_truthy(_t138)) break;
#line 205 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 206 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 209 "compiler/main.gem"
    GemVal _t139 = gem_table_new();
    gem_table_set(_t139, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t139, gem_string("value"), gem_v_val);
    gem_table_set(_t139, gem_string("line"), gem_v_line);
    GemVal _t140[] = {gem_v_tokens, _t139};
            (void)(gem_push_fn(NULL, _t140, 2));
            continue;
        }
#line 214 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 215 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 218 "compiler/main.gem"
    GemVal _t141;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t141 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t141 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t142;
    if (!gem_truthy(_t141)) {
        _t142 = _t141;
    } else {
        _t142 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
            if (gem_truthy(_t142)) {
#line 219 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 221 "compiler/main.gem"
                while (1) {
                    GemVal _t144;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t144 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t143;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t143 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t143 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t144 = _t143;
                    }
                    if (!gem_truthy(_t144)) break;
#line 222 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 224 "compiler/main.gem"
    GemVal _t145;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t145 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t145 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t145)) {
#line 225 "compiler/main.gem"
    GemVal _t146[] = {gem_v_line};
    GemVal _t147[] = {gem_add(gem_string("triple-quoted string must be followed by a newline at line "), gem_to_string_fn(NULL, _t146, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 225, _t147, 1));
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
                    GemVal _t148;
                    if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                        _t148 = gem_lt(gem_v_tq_scan, gem_v_length);
                    } else {
                                        _t148 = gem_not(gem_v_tq_found);
                    }
                    if (!gem_truthy(_t148)) break;
#line 235 "compiler/main.gem"
                    GemVal gem_v_tq_ind = gem_int(0);
#line 236 "compiler/main.gem"
                    while (1) {
                        GemVal _t149;
                        if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                _t149 = gem_lt(gem_v_tq_scan, gem_v_length);
                        } else {
                                                _t149 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t149)) break;
#line 237 "compiler/main.gem"
                        gem_v_tq_ind = gem_add(gem_v_tq_ind, gem_int(1));
#line 238 "compiler/main.gem"
                        gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                    }
#line 240 "compiler/main.gem"
    GemVal _t150;
    if (!gem_truthy(gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length))) {
        _t150 = gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length);
    } else {
        _t150 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\""));
    }
    GemVal _t151;
    if (!gem_truthy(_t150)) {
        _t151 = _t150;
    } else {
        _t151 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(1))), gem_string("\""));
    }
    GemVal _t152;
    if (!gem_truthy(_t151)) {
        _t152 = _t151;
    } else {
        _t152 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(2))), gem_string("\""));
    }
                    if (gem_truthy(_t152)) {
#line 241 "compiler/main.gem"
                        gem_v_tq_bi = gem_v_tq_ind;
#line 242 "compiler/main.gem"
                        gem_v_tq_found = gem_bool(1);
                    } else {
#line 244 "compiler/main.gem"
                        while (1) {
                            GemVal _t153;
                            if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                        _t153 = gem_lt(gem_v_tq_scan, gem_v_length);
                            } else {
                                                        _t153 = gem_neq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t153)) break;
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
    GemVal _t154[] = {gem_v_line};
    GemVal _t155[] = {gem_add(gem_string("unterminated triple-quoted string at line "), gem_to_string_fn(NULL, _t154, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 253, _t155, 1));
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
                            GemVal _t156;
                            if (!gem_truthy(gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent))) {
                                                        _t156 = gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t156 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t157;
                            if (!gem_truthy(_t156)) {
                                                        _t157 = _t156;
                            } else {
                                                        _t157 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t157)) break;
#line 267 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 268 "compiler/main.gem"
                            gem_v_tq_skip = gem_add(gem_v_tq_skip, gem_int(1));
                        }
#line 271 "compiler/main.gem"
    GemVal _t158;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t158 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t158 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t159;
    if (!gem_truthy(_t158)) {
        _t159 = _t158;
    } else {
        _t159 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t160;
    if (!gem_truthy(_t159)) {
        _t160 = _t159;
    } else {
        _t160 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t160)) {
#line 272 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 274 "compiler/main.gem"
    GemVal _t161[] = {gem_v_val};
    GemVal _t163;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t161, 1), gem_int(0)))) {
        _t163 = gem_gt(gem_len_fn(NULL, _t161, 1), gem_int(0));
    } else {
        GemVal _t162[] = {gem_v_val};
        _t163 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t162, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t163)) {
#line 275 "compiler/main.gem"
    GemVal _t164[] = {gem_v_val};
    GemVal _t165[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t164, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t165, 3);
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
    GemVal _t166[] = {gem_v_line};
    GemVal _t167[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t166, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 292, _t167, 1));
                        }
#line 294 "compiler/main.gem"
                        GemVal gem_v_tqe = gem_table_get(gem_v_source, gem_v_pos);
#line 295 "compiler/main.gem"
    GemVal _t168[] = {gem_v_tqe, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t168, 2);
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
    GemVal _t169 = gem_table_new();
    gem_table_set(_t169, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t169, gem_string("value"), gem_string(""));
    gem_table_set(_t169, gem_string("line"), gem_v_line);
    GemVal _t170[] = {gem_v_tokens, _t169};
                        (void)(gem_push_fn(NULL, _t170, 2));
#line 306 "compiler/main.gem"
    GemVal _t171 = gem_table_new();
    gem_table_set(_t171, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t171, gem_string("value"), gem_v_val);
    gem_table_set(_t171, gem_string("line"), gem_v_line);
    GemVal _t172[] = {gem_v_tokens, _t171};
                        (void)(gem_push_fn(NULL, _t172, 2));
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
    GemVal _t173 = gem_table_new();
    gem_table_set(_t173, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t173, gem_string("value"), gem_v_val);
    gem_table_set(_t173, gem_string("line"), gem_v_line);
    GemVal _t174[] = {gem_v_tokens, _t173};
                    (void)(gem_push_fn(NULL, _t174, 2));
                }
                continue;
            }
#line 326 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 327 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 328 "compiler/main.gem"
            while (1) {
                GemVal _t175;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t175 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t175 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t175)) break;
#line 329 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 330 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 331 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 332 "compiler/main.gem"
    GemVal _t176[] = {gem_v_line};
    GemVal _t177[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t176, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 332, _t177, 1));
                    }
#line 334 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 335 "compiler/main.gem"
    GemVal _t178[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t178, 2);
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
    GemVal _t179 = gem_table_new();
    gem_table_set(_t179, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t179, gem_string("value"), gem_string(""));
    gem_table_set(_t179, gem_string("line"), gem_v_line);
    GemVal _t180[] = {gem_v_tokens, _t179};
                    (void)(gem_push_fn(NULL, _t180, 2));
#line 346 "compiler/main.gem"
    GemVal _t181 = gem_table_new();
    gem_table_set(_t181, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t181, gem_string("value"), gem_v_val);
    gem_table_set(_t181, gem_string("line"), gem_v_line);
    GemVal _t182[] = {gem_v_tokens, _t181};
                    (void)(gem_push_fn(NULL, _t182, 2));
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
    GemVal _t183[] = {gem_v_line};
    GemVal _t184[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t183, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 359, _t184, 1));
                }
#line 361 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 362 "compiler/main.gem"
    GemVal _t185 = gem_table_new();
    gem_table_set(_t185, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t185, gem_string("value"), gem_v_val);
    gem_table_set(_t185, gem_string("line"), gem_v_line);
    GemVal _t186[] = {gem_v_tokens, _t185};
                (void)(gem_push_fn(NULL, _t186, 2));
            }
            continue;
        }
#line 368 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("'")))) {
#line 369 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 372 "compiler/main.gem"
    GemVal _t187;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t187 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t187 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t188;
    if (!gem_truthy(_t187)) {
        _t188 = _t187;
    } else {
        _t188 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
            if (gem_truthy(_t188)) {
#line 373 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 375 "compiler/main.gem"
                while (1) {
                    GemVal _t190;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t190 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t189;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t189 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t189 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t190 = _t189;
                    }
                    if (!gem_truthy(_t190)) break;
#line 376 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 378 "compiler/main.gem"
    GemVal _t191;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t191 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t191 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t191)) {
#line 379 "compiler/main.gem"
    GemVal _t192[] = {gem_v_line};
    GemVal _t193[] = {gem_add(gem_string("triple-quoted string must be followed by a newline at line "), gem_to_string_fn(NULL, _t192, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 379, _t193, 1));
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
                    GemVal _t194;
                    if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                        _t194 = gem_lt(gem_v_sq_scan, gem_v_length);
                    } else {
                                        _t194 = gem_not(gem_v_sq_found);
                    }
                    if (!gem_truthy(_t194)) break;
#line 389 "compiler/main.gem"
                    GemVal gem_v_sq_ind = gem_int(0);
#line 390 "compiler/main.gem"
                    while (1) {
                        GemVal _t195;
                        if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                _t195 = gem_lt(gem_v_sq_scan, gem_v_length);
                        } else {
                                                _t195 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t195)) break;
#line 391 "compiler/main.gem"
                        gem_v_sq_ind = gem_add(gem_v_sq_ind, gem_int(1));
#line 392 "compiler/main.gem"
                        gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                    }
#line 394 "compiler/main.gem"
    GemVal _t196;
    if (!gem_truthy(gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length))) {
        _t196 = gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length);
    } else {
        _t196 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("'"));
    }
    GemVal _t197;
    if (!gem_truthy(_t196)) {
        _t197 = _t196;
    } else {
        _t197 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(1))), gem_string("'"));
    }
    GemVal _t198;
    if (!gem_truthy(_t197)) {
        _t198 = _t197;
    } else {
        _t198 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(2))), gem_string("'"));
    }
                    if (gem_truthy(_t198)) {
#line 395 "compiler/main.gem"
                        gem_v_sq_bi = gem_v_sq_ind;
#line 396 "compiler/main.gem"
                        gem_v_sq_found = gem_bool(1);
                    } else {
#line 398 "compiler/main.gem"
                        while (1) {
                            GemVal _t199;
                            if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                        _t199 = gem_lt(gem_v_sq_scan, gem_v_length);
                            } else {
                                                        _t199 = gem_neq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t199)) break;
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
    GemVal _t200[] = {gem_v_line};
    GemVal _t201[] = {gem_add(gem_string("unterminated triple-quoted string at line "), gem_to_string_fn(NULL, _t200, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 407, _t201, 1));
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
                            GemVal _t202;
                            if (!gem_truthy(gem_lt(gem_v_sq_skip, gem_v_sq_bi))) {
                                                        _t202 = gem_lt(gem_v_sq_skip, gem_v_sq_bi);
                            } else {
                                                        _t202 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t203;
                            if (!gem_truthy(_t202)) {
                                                        _t203 = _t202;
                            } else {
                                                        _t203 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t203)) break;
#line 419 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 420 "compiler/main.gem"
                            gem_v_sq_skip = gem_add(gem_v_sq_skip, gem_int(1));
                        }
#line 423 "compiler/main.gem"
    GemVal _t204;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t204 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t204 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t205;
    if (!gem_truthy(_t204)) {
        _t205 = _t204;
    } else {
        _t205 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
    GemVal _t206;
    if (!gem_truthy(_t205)) {
        _t206 = _t205;
    } else {
        _t206 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("'"));
    }
                        if (gem_truthy(_t206)) {
#line 424 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 426 "compiler/main.gem"
    GemVal _t207[] = {gem_v_val};
    GemVal _t209;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t207, 1), gem_int(0)))) {
        _t209 = gem_gt(gem_len_fn(NULL, _t207, 1), gem_int(0));
    } else {
        GemVal _t208[] = {gem_v_val};
        _t209 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t208, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t209)) {
#line 427 "compiler/main.gem"
    GemVal _t210[] = {gem_v_val};
    GemVal _t211[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t210, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t211, 3);
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
    GemVal _t212[] = {gem_v_line};
    GemVal _t213[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t212, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 444, _t213, 1));
                        }
#line 446 "compiler/main.gem"
                        GemVal gem_v_sqe = gem_table_get(gem_v_source, gem_v_pos);
#line 447 "compiler/main.gem"
    GemVal _t214[] = {gem_v_sqe, gem_string("sq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t214, 2);
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
    GemVal _t215 = gem_table_new();
    gem_table_set(_t215, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t215, gem_string("value"), gem_v_val);
    gem_table_set(_t215, gem_string("line"), gem_v_line);
    GemVal _t216[] = {gem_v_tokens, _t215};
                (void)(gem_push_fn(NULL, _t216, 2));
                continue;
            }
#line 465 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 466 "compiler/main.gem"
            while (1) {
                GemVal _t217;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t217 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t217 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
                }
                if (!gem_truthy(_t217)) break;
#line 467 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 468 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 469 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 470 "compiler/main.gem"
    GemVal _t218[] = {gem_v_line};
    GemVal _t219[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t218, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 470, _t219, 1));
                    }
#line 472 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 473 "compiler/main.gem"
    GemVal _t220[] = {gem_v_esc, gem_string("sq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t220, 2);
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
    GemVal _t221[] = {gem_v_line};
    GemVal _t222[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t221, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 486, _t222, 1));
            }
#line 488 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 489 "compiler/main.gem"
    GemVal _t223 = gem_table_new();
    gem_table_set(_t223, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t223, gem_string("value"), gem_v_val);
    gem_table_set(_t223, gem_string("line"), gem_v_line);
    GemVal _t224[] = {gem_v_tokens, _t223};
            (void)(gem_push_fn(NULL, _t224, 2));
            continue;
        }
#line 494 "compiler/main.gem"
    GemVal _t225[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t225, 1))) {
#line 495 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 496 "compiler/main.gem"
            while (1) {
                GemVal _t227;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t227 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t226[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t227 = gem_fn_is_alnum(NULL, _t226, 1);
                }
                if (!gem_truthy(_t227)) break;
#line 497 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 498 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 500 "compiler/main.gem"
    GemVal _t228[] = {gem_v_kw, gem_v_val};
            if (gem_truthy(gem_has_key_fn(NULL, _t228, 2))) {
#line 501 "compiler/main.gem"
    GemVal _t229 = gem_table_new();
    gem_table_set(_t229, gem_string("type"), gem_v_val);
    gem_table_set(_t229, gem_string("value"), gem_v_val);
    gem_table_set(_t229, gem_string("line"), gem_v_line);
    GemVal _t230[] = {gem_v_tokens, _t229};
                (void)(gem_push_fn(NULL, _t230, 2));
            } else {
#line 503 "compiler/main.gem"
    GemVal _t231 = gem_table_new();
    gem_table_set(_t231, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t231, gem_string("value"), gem_v_val);
    gem_table_set(_t231, gem_string("line"), gem_v_line);
    GemVal _t232[] = {gem_v_tokens, _t231};
                (void)(gem_push_fn(NULL, _t232, 2));
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
    GemVal _t233;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t233 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t233 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t233)) {
#line 515 "compiler/main.gem"
    GemVal _t234 = gem_table_new();
    gem_table_set(_t234, gem_string("type"), gem_string("=="));
    gem_table_set(_t234, gem_string("value"), gem_string("=="));
    gem_table_set(_t234, gem_string("line"), gem_v_line);
    GemVal _t235[] = {gem_v_tokens, _t234};
            (void)(gem_push_fn(NULL, _t235, 2));
#line 516 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 519 "compiler/main.gem"
    GemVal _t236;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t236 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t236 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t236)) {
#line 520 "compiler/main.gem"
    GemVal _t237 = gem_table_new();
    gem_table_set(_t237, gem_string("type"), gem_string("!="));
    gem_table_set(_t237, gem_string("value"), gem_string("!="));
    gem_table_set(_t237, gem_string("line"), gem_v_line);
    GemVal _t238[] = {gem_v_tokens, _t237};
            (void)(gem_push_fn(NULL, _t238, 2));
#line 521 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 524 "compiler/main.gem"
    GemVal _t239;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t239 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t239 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t239)) {
#line 525 "compiler/main.gem"
    GemVal _t240 = gem_table_new();
    gem_table_set(_t240, gem_string("type"), gem_string("<="));
    gem_table_set(_t240, gem_string("value"), gem_string("<="));
    gem_table_set(_t240, gem_string("line"), gem_v_line);
    GemVal _t241[] = {gem_v_tokens, _t240};
            (void)(gem_push_fn(NULL, _t241, 2));
#line 526 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 529 "compiler/main.gem"
    GemVal _t242;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t242 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t242 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t242)) {
#line 530 "compiler/main.gem"
    GemVal _t243 = gem_table_new();
    gem_table_set(_t243, gem_string("type"), gem_string(">="));
    gem_table_set(_t243, gem_string("value"), gem_string(">="));
    gem_table_set(_t243, gem_string("line"), gem_v_line);
    GemVal _t244[] = {gem_v_tokens, _t243};
            (void)(gem_push_fn(NULL, _t244, 2));
#line 531 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 534 "compiler/main.gem"
    GemVal _t245;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t245 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t245 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t245)) {
#line 535 "compiler/main.gem"
    GemVal _t246 = gem_table_new();
    gem_table_set(_t246, gem_string("type"), gem_string("+="));
    gem_table_set(_t246, gem_string("value"), gem_string("+="));
    gem_table_set(_t246, gem_string("line"), gem_v_line);
    GemVal _t247[] = {gem_v_tokens, _t246};
            (void)(gem_push_fn(NULL, _t247, 2));
#line 536 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 539 "compiler/main.gem"
    GemVal _t248;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t248 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t248 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t248)) {
#line 540 "compiler/main.gem"
    GemVal _t249 = gem_table_new();
    gem_table_set(_t249, gem_string("type"), gem_string("-="));
    gem_table_set(_t249, gem_string("value"), gem_string("-="));
    gem_table_set(_t249, gem_string("line"), gem_v_line);
    GemVal _t250[] = {gem_v_tokens, _t249};
            (void)(gem_push_fn(NULL, _t250, 2));
#line 541 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 544 "compiler/main.gem"
    GemVal _t251;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t251 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t251 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t251)) {
#line 545 "compiler/main.gem"
    GemVal _t252 = gem_table_new();
    gem_table_set(_t252, gem_string("type"), gem_string("*="));
    gem_table_set(_t252, gem_string("value"), gem_string("*="));
    gem_table_set(_t252, gem_string("line"), gem_v_line);
    GemVal _t253[] = {gem_v_tokens, _t252};
            (void)(gem_push_fn(NULL, _t253, 2));
#line 546 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 549 "compiler/main.gem"
    GemVal _t254;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t254 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t254 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t254)) {
#line 550 "compiler/main.gem"
    GemVal _t255 = gem_table_new();
    gem_table_set(_t255, gem_string("type"), gem_string("/="));
    gem_table_set(_t255, gem_string("value"), gem_string("/="));
    gem_table_set(_t255, gem_string("line"), gem_v_line);
    GemVal _t256[] = {gem_v_tokens, _t255};
            (void)(gem_push_fn(NULL, _t256, 2));
#line 551 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 554 "compiler/main.gem"
    GemVal _t257;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t257 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t257 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t257)) {
#line 555 "compiler/main.gem"
    GemVal _t258 = gem_table_new();
    gem_table_set(_t258, gem_string("type"), gem_string("->"));
    gem_table_set(_t258, gem_string("value"), gem_string("->"));
    gem_table_set(_t258, gem_string("line"), gem_v_line);
    GemVal _t259[] = {gem_v_tokens, _t258};
            (void)(gem_push_fn(NULL, _t259, 2));
#line 556 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 561 "compiler/main.gem"
    GemVal _t260;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t260 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t260 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t261;
    if (gem_truthy(_t260)) {
        _t261 = _t260;
    } else {
        _t261 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t262;
    if (gem_truthy(_t261)) {
        _t262 = _t261;
    } else {
        _t262 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t263;
    if (gem_truthy(_t262)) {
        _t263 = _t262;
    } else {
        _t263 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t263)) {
#line 562 "compiler/main.gem"
    GemVal _t264 = gem_table_new();
    gem_table_set(_t264, gem_string("type"), gem_v_ch);
    gem_table_set(_t264, gem_string("value"), gem_v_ch);
    gem_table_set(_t264, gem_string("line"), gem_v_line);
    GemVal _t265[] = {gem_v_tokens, _t264};
            (void)(gem_push_fn(NULL, _t265, 2));
#line 563 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 566 "compiler/main.gem"
    GemVal _t266;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t266 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t266 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t267;
    if (gem_truthy(_t266)) {
        _t267 = _t266;
    } else {
        _t267 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t267)) {
#line 567 "compiler/main.gem"
    GemVal _t268 = gem_table_new();
    gem_table_set(_t268, gem_string("type"), gem_v_ch);
    gem_table_set(_t268, gem_string("value"), gem_v_ch);
    gem_table_set(_t268, gem_string("line"), gem_v_line);
    GemVal _t269[] = {gem_v_tokens, _t268};
            (void)(gem_push_fn(NULL, _t269, 2));
#line 568 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 572 "compiler/main.gem"
    GemVal _t270;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t270 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t270 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t270)) {
#line 573 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 574 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 575 "compiler/main.gem"
    GemVal _t271 = gem_table_new();
    gem_table_set(_t271, gem_string("type"), gem_string("{"));
    gem_table_set(_t271, gem_string("value"), gem_string("{"));
    gem_table_set(_t271, gem_string("line"), gem_v_line);
    GemVal _t272[] = {gem_v_tokens, _t271};
            (void)(gem_push_fn(NULL, _t272, 2));
#line 576 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 579 "compiler/main.gem"
    GemVal _t273;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t273 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t273 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t273)) {
#line 580 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 581 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 582 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 583 "compiler/main.gem"
    GemVal _t274 = gem_table_new();
    gem_table_set(_t274, gem_string("type"), gem_string("}"));
    gem_table_set(_t274, gem_string("value"), gem_string("}"));
    gem_table_set(_t274, gem_string("line"), gem_v_line);
    GemVal _t275[] = {gem_v_tokens, _t274};
                (void)(gem_push_fn(NULL, _t275, 2));
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
                            GemVal _t276;
                            if (!gem_truthy(gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent))) {
                                                        _t276 = gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t276 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t277;
                            if (!gem_truthy(_t276)) {
                                                        _t277 = _t276;
                            } else {
                                                        _t277 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t277)) break;
#line 603 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 604 "compiler/main.gem"
                            gem_v_tqr_skip = gem_add(gem_v_tqr_skip, gem_int(1));
                        }
#line 607 "compiler/main.gem"
    GemVal _t278;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t278 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t278 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t279;
    if (!gem_truthy(_t278)) {
        _t279 = _t278;
    } else {
        _t279 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t280;
    if (!gem_truthy(_t279)) {
        _t280 = _t279;
    } else {
        _t280 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t280)) {
#line 608 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 609 "compiler/main.gem"
    GemVal _t281[] = {gem_v_val};
    GemVal _t283;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t281, 1), gem_int(0)))) {
        _t283 = gem_gt(gem_len_fn(NULL, _t281, 1), gem_int(0));
    } else {
        GemVal _t282[] = {gem_v_val};
        _t283 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t282, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t283)) {
#line 610 "compiler/main.gem"
    GemVal _t284[] = {gem_v_val};
    GemVal _t285[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t284, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t285, 3);
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
    GemVal _t286[] = {gem_v_line};
    GemVal _t287[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t286, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 627, _t287, 1));
                        }
#line 629 "compiler/main.gem"
                        GemVal gem_v_tqre = gem_table_get(gem_v_source, gem_v_pos);
#line 630 "compiler/main.gem"
    GemVal _t288[] = {gem_v_tqre, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t288, 2);
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
    GemVal _t289 = gem_table_new();
    gem_table_set(_t289, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t289, gem_string("value"), gem_v_val);
    gem_table_set(_t289, gem_string("line"), gem_v_line);
    GemVal _t290[] = {gem_v_tokens, _t289};
                        (void)(gem_push_fn(NULL, _t290, 2));
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
    GemVal _t291[] = {gem_v_line};
    GemVal _t292[] = {gem_add(gem_string("unterminated triple-quoted string interpolation at line "), gem_to_string_fn(NULL, _t291, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 655, _t292, 1));
                    }
#line 657 "compiler/main.gem"
    GemVal _t293 = gem_table_new();
    gem_table_set(_t293, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t293, gem_string("value"), gem_v_val);
    gem_table_set(_t293, gem_string("line"), gem_v_line);
    GemVal _t294[] = {gem_v_tokens, _t293};
                    (void)(gem_push_fn(NULL, _t294, 2));
#line 658 "compiler/main.gem"
    GemVal _t295 = gem_table_new();
    gem_table_set(_t295, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t295, gem_string("value"), gem_string(""));
    gem_table_set(_t295, gem_string("line"), gem_v_line);
    GemVal _t296[] = {gem_v_tokens, _t295};
                    (void)(gem_push_fn(NULL, _t296, 2));
                }
                continue;
            }
#line 664 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 665 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 666 "compiler/main.gem"
            while (1) {
                GemVal _t297;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t297 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t297 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t297)) break;
#line 667 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 668 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 669 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 670 "compiler/main.gem"
    GemVal _t298[] = {gem_v_line};
    GemVal _t299[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t298, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 670, _t299, 1));
                    }
#line 672 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 673 "compiler/main.gem"
    GemVal _t300[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t300, 2);
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
    GemVal _t301 = gem_table_new();
    gem_table_set(_t301, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t301, gem_string("value"), gem_v_val);
    gem_table_set(_t301, gem_string("line"), gem_v_line);
    GemVal _t302[] = {gem_v_tokens, _t301};
                    (void)(gem_push_fn(NULL, _t302, 2));
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
    GemVal _t303[] = {gem_v_line};
    GemVal _t304[] = {gem_add(gem_string("unterminated string interpolation at line "), gem_to_string_fn(NULL, _t303, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 697, _t304, 1));
                }
#line 699 "compiler/main.gem"
    GemVal _t305 = gem_table_new();
    gem_table_set(_t305, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t305, gem_string("value"), gem_v_val);
    gem_table_set(_t305, gem_string("line"), gem_v_line);
    GemVal _t306[] = {gem_v_tokens, _t305};
                (void)(gem_push_fn(NULL, _t306, 2));
#line 700 "compiler/main.gem"
    GemVal _t307 = gem_table_new();
    gem_table_set(_t307, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t307, gem_string("value"), gem_string(""));
    gem_table_set(_t307, gem_string("line"), gem_v_line);
    GemVal _t308[] = {gem_v_tokens, _t307};
                (void)(gem_push_fn(NULL, _t308, 2));
#line 701 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 706 "compiler/main.gem"
    GemVal _t309;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t309 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t309 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t310;
    if (gem_truthy(_t309)) {
        _t310 = _t309;
    } else {
        _t310 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t311;
    if (gem_truthy(_t310)) {
        _t311 = _t310;
    } else {
        _t311 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t312;
    if (gem_truthy(_t311)) {
        _t312 = _t311;
    } else {
        _t312 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t313;
    if (gem_truthy(_t312)) {
        _t313 = _t312;
    } else {
        _t313 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t313)) {
#line 707 "compiler/main.gem"
    GemVal _t314 = gem_table_new();
    gem_table_set(_t314, gem_string("type"), gem_v_ch);
    gem_table_set(_t314, gem_string("value"), gem_v_ch);
    gem_table_set(_t314, gem_string("line"), gem_v_line);
    GemVal _t315[] = {gem_v_tokens, _t314};
            (void)(gem_push_fn(NULL, _t315, 2));
#line 708 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 711 "compiler/main.gem"
    GemVal _t316;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t316 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t316 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    }
    GemVal _t317;
    if (!gem_truthy(_t316)) {
        _t317 = _t316;
    } else {
        _t317 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("."));
    }
    GemVal _t318;
    if (!gem_truthy(_t317)) {
        _t318 = _t317;
    } else {
        _t318 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    }
    GemVal _t319;
    if (!gem_truthy(_t318)) {
        _t319 = _t318;
    } else {
        _t319 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("."));
    }
        if (gem_truthy(_t319)) {
#line 712 "compiler/main.gem"
    GemVal _t320 = gem_table_new();
    gem_table_set(_t320, gem_string("type"), gem_string("..."));
    gem_table_set(_t320, gem_string("value"), gem_string("..."));
    gem_table_set(_t320, gem_string("line"), gem_v_line);
    GemVal _t321[] = {gem_v_tokens, _t320};
            (void)(gem_push_fn(NULL, _t321, 2));
#line 713 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
            continue;
        }
#line 716 "compiler/main.gem"
    GemVal _t322;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t322 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t322 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t323;
    if (gem_truthy(_t322)) {
        _t323 = _t322;
    } else {
        _t323 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t324;
    if (gem_truthy(_t323)) {
        _t324 = _t323;
    } else {
        _t324 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t324)) {
#line 717 "compiler/main.gem"
    GemVal _t325 = gem_table_new();
    gem_table_set(_t325, gem_string("type"), gem_v_ch);
    gem_table_set(_t325, gem_string("value"), gem_v_ch);
    gem_table_set(_t325, gem_string("line"), gem_v_line);
    GemVal _t326[] = {gem_v_tokens, _t325};
            (void)(gem_push_fn(NULL, _t326, 2));
#line 718 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 722 "compiler/main.gem"
    GemVal _t327[] = {gem_v_ch};
    GemVal _t328[] = {gem_v_line};
    GemVal _t329[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t327, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t328, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 722, _t329, 1));
    }
#line 725 "compiler/main.gem"
    GemVal _t330 = gem_table_new();
    gem_table_set(_t330, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t330, gem_string("value"), gem_string(""));
    gem_table_set(_t330, gem_string("line"), gem_v_line);
    GemVal _t331[] = {gem_v_tokens, _t330};
    (void)(gem_push_fn(NULL, _t331, 2));
    GemVal _t332 = gem_v_tokens;
    gem_pop_frame();
    return _t332;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 10 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_source", "compiler/main.gem", 10);
#line 11 "compiler/main.gem"
    GemVal _t333[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t333, 1);
#line 12 "compiler/main.gem"
    GemVal _t334[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t334, 1);
#line 13 "compiler/main.gem"
    GemVal _t335 = gem_v_p;
    GemVal _t336 = gem_table_get(_t335, gem_string("parse"));
    GemVal _t337 = _t336.fn(_t336.env, NULL, 0);
    gem_pop_frame();
    return _t337;
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
    GemVal _t338 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t338;
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
    GemVal _t340 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t340;
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
    GemVal _t342 = gem_v_t;
    gem_pop_frame();
    return _t342;
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
    GemVal _t344 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t345 = gem_eq(gem_table_get(_t344, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t345;
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
    GemVal _t347 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t347, gem_string("type")), gem_v_tp))) {
#line 46 "compiler/main.gem"
    GemVal _t348[] = {gem_v_tp};
    GemVal _t349 = gem_v_t;
    GemVal _t350[] = {gem_table_get(_t349, gem_string("type"))};
    GemVal _t351 = gem_v_t;
    GemVal _t352[] = {gem_table_get(_t351, gem_string("line"))};
    GemVal _t353[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t348, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t350, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t352, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 46, _t353, 1));
    }
#line 48 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t354 = gem_v_t;
    gem_pop_frame();
    return _t354;
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
        GemVal _t356 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t356, gem_string("type")), gem_string("NEWLINE")))) break;
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
    GemVal _t358 = gem_table_new();
    GemVal gem_v_params = _t358;
#line 66 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 67 "compiler/main.gem"
    GemVal gem_v_block_param = GEM_NIL;
#line 69 "compiler/main.gem"
    while (1) {
        GemVal _t359 = (*gem_v_peek);
        GemVal _t360 = _t359.fn(_t359.env, NULL, 0);
        GemVal _t363;
        if (gem_truthy(gem_eq(gem_table_get(_t360, gem_string("type")), gem_string("NAME")))) {
                _t363 = gem_eq(gem_table_get(_t360, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t361 = (*gem_v_peek);
                GemVal _t362 = _t361.fn(_t361.env, NULL, 0);
                _t363 = gem_eq(gem_table_get(_t362, gem_string("type")), gem_string("..."));
        }
        if (!gem_truthy(_t363)) break;
#line 70 "compiler/main.gem"
    GemVal _t364 = (*gem_v_peek);
    GemVal _t365 = _t364.fn(_t364.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t365, gem_string("type")), gem_string("...")))) {
#line 71 "compiler/main.gem"
    GemVal _t366 = (*gem_v_advance);
            (void)(_t366.fn(_t366.env, NULL, 0));
#line 72 "compiler/main.gem"
    GemVal _t367[] = {gem_string("NAME")};
    GemVal _t368 = (*gem_v_expect);
    GemVal _t369 = _t368.fn(_t368.env, _t367, 1);
            gem_v_rest_param = gem_table_get(_t369, gem_string("value"));
#line 73 "compiler/main.gem"
    GemVal _t370 = (*gem_v_peek);
    GemVal _t371 = _t370.fn(_t370.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t371, gem_string("type")), gem_string(",")))) {
#line 74 "compiler/main.gem"
    GemVal _t372 = (*gem_v_advance);
                (void)(_t372.fn(_t372.env, NULL, 0));
#line 75 "compiler/main.gem"
    GemVal _t373 = (*gem_v_peek);
    GemVal _t374 = _t373.fn(_t373.env, NULL, 0);
                if (gem_truthy(gem_neq(gem_table_get(_t374, gem_string("type")), gem_string("NAME")))) {
#line 76 "compiler/main.gem"
    GemVal _t375 = (*gem_v_peek);
    GemVal _t376 = _t375.fn(_t375.env, NULL, 0);
    GemVal _t377[] = {gem_table_get(_t376, gem_string("line"))};
    GemVal _t378[] = {gem_add(gem_string("expected parameter name after rest param at line "), gem_to_string_fn(NULL, _t377, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 76, _t378, 1));
                }
#line 78 "compiler/main.gem"
    GemVal _t379 = (*gem_v_advance);
    GemVal _t380 = _t379.fn(_t379.env, NULL, 0);
                gem_v_block_param = gem_table_get(_t380, gem_string("value"));
#line 79 "compiler/main.gem"
    GemVal _t381 = (*gem_v_peek);
    GemVal _t382 = _t381.fn(_t381.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t382, gem_string("type")), gem_string(",")))) {
#line 80 "compiler/main.gem"
    GemVal _t383 = (*gem_v_peek);
    GemVal _t384 = _t383.fn(_t383.env, NULL, 0);
    GemVal _t385[] = {gem_table_get(_t384, gem_string("line"))};
    GemVal _t386[] = {gem_add(gem_string("only one parameter is allowed after a rest param at line "), gem_to_string_fn(NULL, _t385, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 80, _t386, 1));
                }
            }
            break;
        }
#line 85 "compiler/main.gem"
    GemVal _t387 = (*gem_v_advance);
    GemVal _t388 = _t387.fn(_t387.env, NULL, 0);
    GemVal _t389[] = {gem_v_params, gem_table_get(_t388, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t389, 2));
#line 86 "compiler/main.gem"
    GemVal _t390 = (*gem_v_peek);
    GemVal _t391 = _t390.fn(_t390.env, NULL, 0);
        if (gem_truthy(gem_neq(gem_table_get(_t391, gem_string("type")), gem_string(",")))) {
            break;
        }
#line 89 "compiler/main.gem"
    GemVal _t392 = (*gem_v_advance);
        (void)(_t392.fn(_t392.env, NULL, 0));
    }
    GemVal _t393 = gem_table_new();
    gem_table_set(_t393, gem_string("params"), gem_v_params);
    gem_table_set(_t393, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t393, gem_string("block_param"), gem_v_block_param);
    GemVal _t394 = _t393;
    gem_pop_frame();
    return _t394;
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
    GemVal _t396 = gem_table_new();
    GemVal gem_v_stmts = _t396;
#line 108 "compiler/main.gem"
    GemVal _t397 = (*gem_v_skip_nl);
    (void)(_t397.fn(_t397.env, NULL, 0));
#line 109 "compiler/main.gem"
    while (1) {
        GemVal _t398 = (*gem_v_peek);
        GemVal _t399 = _t398.fn(_t398.env, NULL, 0);
        GemVal _t402;
        if (!gem_truthy(gem_neq(gem_table_get(_t399, gem_string("type")), gem_string("end")))) {
                _t402 = gem_neq(gem_table_get(_t399, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t400 = (*gem_v_peek);
                GemVal _t401 = _t400.fn(_t400.env, NULL, 0);
                _t402 = gem_neq(gem_table_get(_t401, gem_string("type")), gem_string("elif"));
        }
        GemVal _t405;
        if (!gem_truthy(_t402)) {
                _t405 = _t402;
        } else {
                GemVal _t403 = (*gem_v_peek);
                GemVal _t404 = _t403.fn(_t403.env, NULL, 0);
                _t405 = gem_neq(gem_table_get(_t404, gem_string("type")), gem_string("else"));
        }
        GemVal _t408;
        if (!gem_truthy(_t405)) {
                _t408 = _t405;
        } else {
                GemVal _t406 = (*gem_v_peek);
                GemVal _t407 = _t406.fn(_t406.env, NULL, 0);
                _t408 = gem_neq(gem_table_get(_t407, gem_string("type")), gem_string("when"));
        }
        GemVal _t410;
        if (!gem_truthy(_t408)) {
                _t410 = _t408;
        } else {
                GemVal _t409 = (*gem_v_at_end);
                _t410 = gem_not(_t409.fn(_t409.env, NULL, 0));
        }
        if (!gem_truthy(_t410)) break;
#line 110 "compiler/main.gem"
    GemVal _t411 = (*gem_v_parse_stmt);
    GemVal _t412[] = {gem_v_stmts, _t411.fn(_t411.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t412, 2));
#line 111 "compiler/main.gem"
    GemVal _t413 = (*gem_v_skip_nl);
        (void)(_t413.fn(_t413.env, NULL, 0));
    }
    GemVal _t414 = gem_v_stmts;
    gem_pop_frame();
    return _t414;
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
    GemVal _t416 = gem_table_new();
    GemVal gem_v_stmts = _t416;
#line 121 "compiler/main.gem"
    GemVal _t417 = (*gem_v_skip_nl);
    (void)(_t417.fn(_t417.env, NULL, 0));
#line 122 "compiler/main.gem"
    while (1) {
        GemVal _t418 = (*gem_v_peek);
        GemVal _t419 = _t418.fn(_t418.env, NULL, 0);
        GemVal _t421;
        if (!gem_truthy(gem_neq(gem_table_get(_t419, gem_string("type")), gem_string("end")))) {
                _t421 = gem_neq(gem_table_get(_t419, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t420 = (*gem_v_at_end);
                _t421 = gem_not(_t420.fn(_t420.env, NULL, 0));
        }
        if (!gem_truthy(_t421)) break;
#line 123 "compiler/main.gem"
    GemVal _t422 = (*gem_v_parse_stmt);
    GemVal _t423[] = {gem_v_stmts, _t422.fn(_t422.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t423, 2));
#line 124 "compiler/main.gem"
    GemVal _t424 = (*gem_v_skip_nl);
        (void)(_t424.fn(_t424.env, NULL, 0));
    }
    GemVal _t425 = gem_v_stmts;
    gem_pop_frame();
    return _t425;
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
    GemVal _t427 = (*gem_v_peek);
    GemVal gem_v_t = _t427.fn(_t427.env, NULL, 0);
#line 136 "compiler/main.gem"
    GemVal _t428 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t428, gem_string("type")), gem_string("NUMBER")))) {
#line 137 "compiler/main.gem"
    GemVal _t429 = (*gem_v_advance);
        (void)(_t429.fn(_t429.env, NULL, 0));
#line 138 "compiler/main.gem"
    GemVal _t430 = gem_v_t;
    GemVal _t431[] = {gem_table_get(_t430, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t431, 1))) {
#line 139 "compiler/main.gem"
    GemVal _t432 = gem_v_t;
    GemVal _t433[] = {gem_table_get(_t432, gem_string("value"))};
    GemVal _t434[] = {gem_fn_atof(NULL, _t433, 1)};
            GemVal _t435 = gem_fn_make_float(NULL, _t434, 1);
            gem_pop_frame();
            return _t435;
        }
#line 141 "compiler/main.gem"
    GemVal _t436 = gem_v_t;
    GemVal _t437[] = {gem_table_get(_t436, gem_string("value"))};
    GemVal _t438[] = {gem_fn_str_to_int(NULL, _t437, 1)};
        GemVal _t439 = gem_fn_make_int(NULL, _t438, 1);
        gem_pop_frame();
        return _t439;
    }
#line 145 "compiler/main.gem"
    GemVal _t440 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t440, gem_string("type")), gem_string("STRING")))) {
#line 146 "compiler/main.gem"
    GemVal _t441 = (*gem_v_advance);
        (void)(_t441.fn(_t441.env, NULL, 0));
#line 147 "compiler/main.gem"
    GemVal _t442 = gem_v_t;
    GemVal _t443[] = {gem_table_get(_t442, gem_string("value"))};
        GemVal _t444 = gem_fn_make_string(NULL, _t443, 1);
        gem_pop_frame();
        return _t444;
    }
#line 151 "compiler/main.gem"
    GemVal _t445 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t445, gem_string("type")), gem_string("INTERP_START")))) {
#line 152 "compiler/main.gem"
    GemVal _t446 = (*gem_v_advance);
        (void)(_t446.fn(_t446.env, NULL, 0));
#line 153 "compiler/main.gem"
    GemVal _t447 = gem_table_new();
        GemVal gem_v_parts = _t447;
#line 154 "compiler/main.gem"
    GemVal _t448 = (*gem_v_skip_nl);
        (void)(_t448.fn(_t448.env, NULL, 0));
#line 155 "compiler/main.gem"
        while (1) {
            GemVal _t449 = (*gem_v_peek);
            GemVal _t450 = _t449.fn(_t449.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t450, gem_string("type")), gem_string("INTERP_END")))) break;
#line 156 "compiler/main.gem"
    GemVal _t451 = (*gem_v_peek);
    GemVal _t452 = _t451.fn(_t451.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t452, gem_string("type")), gem_string("STRING")))) {
#line 157 "compiler/main.gem"
    GemVal _t453 = (*gem_v_advance);
    GemVal _t454 = _t453.fn(_t453.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t454, gem_string("value"));
#line 158 "compiler/main.gem"
    GemVal _t455[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t455, 1), gem_int(0)))) {
#line 159 "compiler/main.gem"
    GemVal _t456[] = {gem_v_sval};
    GemVal _t457[] = {gem_v_parts, gem_fn_make_string(NULL, _t456, 1)};
                    (void)(gem_push_fn(NULL, _t457, 2));
                }
            } else {
#line 162 "compiler/main.gem"
    GemVal _t458 = (*gem_v_parse_expr);
    GemVal _t459[] = {gem_v_parts, _t458.fn(_t458.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t459, 2));
            }
#line 164 "compiler/main.gem"
    GemVal _t460 = (*gem_v_skip_nl);
            (void)(_t460.fn(_t460.env, NULL, 0));
        }
#line 166 "compiler/main.gem"
    GemVal _t461[] = {gem_string("INTERP_END")};
    GemVal _t462 = (*gem_v_expect);
        (void)(_t462.fn(_t462.env, _t461, 1));
#line 167 "compiler/main.gem"
    GemVal _t463[] = {gem_v_parts};
        GemVal _t464 = gem_fn_make_interp(NULL, _t463, 1);
        gem_pop_frame();
        return _t464;
    }
#line 171 "compiler/main.gem"
    GemVal _t465 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t465, gem_string("type")), gem_string("true")))) {
#line 172 "compiler/main.gem"
    GemVal _t466 = (*gem_v_advance);
        (void)(_t466.fn(_t466.env, NULL, 0));
#line 173 "compiler/main.gem"
    GemVal _t467[] = {gem_bool(1)};
        GemVal _t468 = gem_fn_make_bool(NULL, _t467, 1);
        gem_pop_frame();
        return _t468;
    }
#line 175 "compiler/main.gem"
    GemVal _t469 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t469, gem_string("type")), gem_string("false")))) {
#line 176 "compiler/main.gem"
    GemVal _t470 = (*gem_v_advance);
        (void)(_t470.fn(_t470.env, NULL, 0));
#line 177 "compiler/main.gem"
    GemVal _t471[] = {gem_bool(0)};
        GemVal _t472 = gem_fn_make_bool(NULL, _t471, 1);
        gem_pop_frame();
        return _t472;
    }
#line 181 "compiler/main.gem"
    GemVal _t473 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t473, gem_string("type")), gem_string("nil")))) {
#line 182 "compiler/main.gem"
    GemVal _t474 = (*gem_v_advance);
        (void)(_t474.fn(_t474.env, NULL, 0));
#line 183 "compiler/main.gem"
        GemVal _t475 = gem_fn_make_nil_node(NULL, NULL, 0);
        gem_pop_frame();
        return _t475;
    }
#line 187 "compiler/main.gem"
    GemVal _t476 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t476, gem_string("type")), gem_string("fn")))) {
#line 188 "compiler/main.gem"
    GemVal _t477 = (*gem_v_advance);
        (void)(_t477.fn(_t477.env, NULL, 0));
#line 189 "compiler/main.gem"
    GemVal _t478[] = {gem_string("(")};
    GemVal _t479 = (*gem_v_expect);
        (void)(_t479.fn(_t479.env, _t478, 1));
#line 190 "compiler/main.gem"
    GemVal _t480 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t480.fn(_t480.env, NULL, 0);
#line 191 "compiler/main.gem"
    GemVal _t481[] = {gem_string(")")};
    GemVal _t482 = (*gem_v_expect);
        (void)(_t482.fn(_t482.env, _t481, 1));
#line 192 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 193 "compiler/main.gem"
    GemVal _t483 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t483.fn(_t483.env, NULL, 0);
#line 194 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 195 "compiler/main.gem"
    GemVal _t484[] = {gem_string("end")};
    GemVal _t485 = (*gem_v_expect);
        (void)(_t485.fn(_t485.env, _t484, 1));
#line 196 "compiler/main.gem"
    GemVal _t486 = gem_v_pr;
    GemVal _t487 = gem_v_pr;
    GemVal _t488 = gem_v_pr;
    GemVal _t489[] = {gem_table_get(_t486, gem_string("params")), gem_table_get(_t487, gem_string("rest_param")), gem_table_get(_t488, gem_string("block_param")), gem_v_body};
        GemVal _t490 = gem_fn_make_anon_fn(NULL, _t489, 4);
        gem_pop_frame();
        return _t490;
    }
#line 201 "compiler/main.gem"
    GemVal _t491 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t491, gem_string("type")), gem_string("{")))) {
#line 202 "compiler/main.gem"
    GemVal _t492 = (*gem_v_advance);
        (void)(_t492.fn(_t492.env, NULL, 0));
#line 203 "compiler/main.gem"
    GemVal _t493 = (*gem_v_skip_nl);
        (void)(_t493.fn(_t493.env, NULL, 0));
#line 204 "compiler/main.gem"
    GemVal _t494 = gem_table_new();
        GemVal gem_v_entries = _t494;
#line 205 "compiler/main.gem"
        while (1) {
            GemVal _t495 = (*gem_v_peek);
            GemVal _t496 = _t495.fn(_t495.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t496, gem_string("type")), gem_string("}")))) break;
#line 206 "compiler/main.gem"
    GemVal _t497 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t497.fn(_t497.env, NULL, 0);
#line 207 "compiler/main.gem"
    GemVal _t498 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t498, gem_string("type")), gem_string("NAME")))) {
#line 208 "compiler/main.gem"
    GemVal _t499 = (*gem_v_advance);
                (void)(_t499.fn(_t499.env, NULL, 0));
            } else {
#line 209 "compiler/main.gem"
    GemVal _t500[] = {gem_int(1)};
    GemVal _t501 = (*gem_v_peek_at);
    GemVal _t502 = _t501.fn(_t501.env, _t500, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t502, gem_string("type")), gem_string(":")))) {
#line 211 "compiler/main.gem"
    GemVal _t503 = (*gem_v_advance);
                    (void)(_t503.fn(_t503.env, NULL, 0));
                } else {
#line 213 "compiler/main.gem"
    GemVal _t504 = gem_v_key_tok;
    GemVal _t505[] = {gem_table_get(_t504, gem_string("type"))};
    GemVal _t506 = gem_v_key_tok;
    GemVal _t507[] = {gem_table_get(_t506, gem_string("line"))};
    GemVal _t508[] = {gem_add(gem_add(gem_add(gem_string("expected table key (name or keyword) but got '"), gem_to_string_fn(NULL, _t505, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t507, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 213, _t508, 1));
                }
            }
#line 215 "compiler/main.gem"
    GemVal _t509[] = {gem_string(":")};
    GemVal _t510 = (*gem_v_expect);
            (void)(_t510.fn(_t510.env, _t509, 1));
#line 216 "compiler/main.gem"
    GemVal _t511 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t511.fn(_t511.env, NULL, 0);
#line 217 "compiler/main.gem"
    GemVal _t512 = gem_v_key_tok;
    GemVal _t513[] = {gem_table_get(_t512, gem_string("value")), gem_v_val};
    GemVal _t514[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t513, 2)};
            (void)(gem_push_fn(NULL, _t514, 2));
#line 218 "compiler/main.gem"
    GemVal _t515 = (*gem_v_skip_nl);
            (void)(_t515.fn(_t515.env, NULL, 0));
#line 219 "compiler/main.gem"
    GemVal _t516 = (*gem_v_peek);
    GemVal _t517 = _t516.fn(_t516.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t517, gem_string("type")), gem_string(",")))) {
#line 220 "compiler/main.gem"
    GemVal _t518 = (*gem_v_advance);
                (void)(_t518.fn(_t518.env, NULL, 0));
#line 221 "compiler/main.gem"
    GemVal _t519 = (*gem_v_skip_nl);
                (void)(_t519.fn(_t519.env, NULL, 0));
            }
        }
#line 224 "compiler/main.gem"
    GemVal _t520[] = {gem_string("}")};
    GemVal _t521 = (*gem_v_expect);
        (void)(_t521.fn(_t521.env, _t520, 1));
#line 225 "compiler/main.gem"
    GemVal _t522[] = {gem_v_entries};
        GemVal _t523 = gem_fn_make_table(NULL, _t522, 1);
        gem_pop_frame();
        return _t523;
    }
#line 229 "compiler/main.gem"
    GemVal _t524 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t524, gem_string("type")), gem_string("[")))) {
#line 230 "compiler/main.gem"
    GemVal _t525 = (*gem_v_advance);
        (void)(_t525.fn(_t525.env, NULL, 0));
#line 231 "compiler/main.gem"
    GemVal _t526 = (*gem_v_skip_nl);
        (void)(_t526.fn(_t526.env, NULL, 0));
#line 232 "compiler/main.gem"
    GemVal _t527 = gem_table_new();
        GemVal gem_v_elements = _t527;
#line 233 "compiler/main.gem"
        while (1) {
            GemVal _t528 = (*gem_v_peek);
            GemVal _t529 = _t528.fn(_t528.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t529, gem_string("type")), gem_string("]")))) break;
#line 234 "compiler/main.gem"
    GemVal _t530 = (*gem_v_parse_expr);
    GemVal _t531[] = {gem_v_elements, _t530.fn(_t530.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t531, 2));
#line 235 "compiler/main.gem"
    GemVal _t532 = (*gem_v_skip_nl);
            (void)(_t532.fn(_t532.env, NULL, 0));
#line 236 "compiler/main.gem"
    GemVal _t533 = (*gem_v_peek);
    GemVal _t534 = _t533.fn(_t533.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t534, gem_string("type")), gem_string(",")))) {
#line 237 "compiler/main.gem"
    GemVal _t535 = (*gem_v_advance);
                (void)(_t535.fn(_t535.env, NULL, 0));
#line 238 "compiler/main.gem"
    GemVal _t536 = (*gem_v_skip_nl);
                (void)(_t536.fn(_t536.env, NULL, 0));
            }
        }
#line 241 "compiler/main.gem"
    GemVal _t537[] = {gem_string("]")};
    GemVal _t538 = (*gem_v_expect);
        (void)(_t538.fn(_t538.env, _t537, 1));
#line 242 "compiler/main.gem"
    GemVal _t539[] = {gem_v_elements};
        GemVal _t540 = gem_fn_make_array(NULL, _t539, 1);
        gem_pop_frame();
        return _t540;
    }
#line 246 "compiler/main.gem"
    GemVal _t541 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t541, gem_string("type")), gem_string("(")))) {
#line 247 "compiler/main.gem"
    GemVal _t542 = (*gem_v_advance);
        (void)(_t542.fn(_t542.env, NULL, 0));
#line 248 "compiler/main.gem"
    GemVal _t543 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t543.fn(_t543.env, NULL, 0);
#line 249 "compiler/main.gem"
    GemVal _t544[] = {gem_string(")")};
    GemVal _t545 = (*gem_v_expect);
        (void)(_t545.fn(_t545.env, _t544, 1));
#line 250 "compiler/main.gem"
        GemVal _t546 = gem_v_e;
        gem_pop_frame();
        return _t546;
    }
#line 254 "compiler/main.gem"
    GemVal _t547 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t547, gem_string("type")), gem_string("NAME")))) {
#line 255 "compiler/main.gem"
    GemVal _t548 = (*gem_v_advance);
        (void)(_t548.fn(_t548.env, NULL, 0));
#line 256 "compiler/main.gem"
    GemVal _t549 = gem_v_t;
    GemVal _t550[] = {gem_table_get(_t549, gem_string("value"))};
        GemVal _t551 = gem_fn_make_var(NULL, _t550, 1);
        gem_pop_frame();
        return _t551;
    }
#line 259 "compiler/main.gem"
    GemVal _t552 = gem_v_t;
    GemVal _t553[] = {gem_table_get(_t552, gem_string("type"))};
    GemVal _t554 = gem_v_t;
    GemVal _t555[] = {gem_table_get(_t554, gem_string("line"))};
    GemVal _t556[] = {gem_add(gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t553, 1)), gem_string("' (expected expression) at line ")), gem_to_string_fn(NULL, _t555, 1))};
    GemVal _t557 = gem_error_at_fn("compiler/main.gem", 259, _t556, 1);
    gem_pop_frame();
    return _t557;
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
    GemVal _t559 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t559.fn(_t559.env, NULL, 0);
#line 266 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
#line 268 "compiler/main.gem"
    GemVal _t560 = (*gem_v_peek);
    GemVal _t561 = _t560.fn(_t560.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t561, gem_string("type")), gem_string("(")))) {
#line 269 "compiler/main.gem"
    GemVal _t562 = (*gem_v_peek);
    GemVal _t563 = _t562.fn(_t562.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t563, gem_string("line"));
#line 270 "compiler/main.gem"
    GemVal _t564 = (*gem_v_advance);
            (void)(_t564.fn(_t564.env, NULL, 0));
#line 271 "compiler/main.gem"
    GemVal _t565 = (*gem_v_skip_nl);
            (void)(_t565.fn(_t565.env, NULL, 0));
#line 272 "compiler/main.gem"
    GemVal _t566 = gem_table_new();
            GemVal gem_v_args = _t566;
#line 273 "compiler/main.gem"
    GemVal _t567 = (*gem_v_peek);
    GemVal _t568 = _t567.fn(_t567.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t568, gem_string("type")), gem_string(")")))) {
#line 274 "compiler/main.gem"
    GemVal _t569 = (*gem_v_parse_expr);
    GemVal _t570[] = {gem_v_args, _t569.fn(_t569.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t570, 2));
#line 275 "compiler/main.gem"
                while (1) {
                    GemVal _t571 = (*gem_v_peek);
                    GemVal _t572 = _t571.fn(_t571.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t572, gem_string("type")), gem_string(",")))) break;
#line 276 "compiler/main.gem"
    GemVal _t573 = (*gem_v_advance);
                    (void)(_t573.fn(_t573.env, NULL, 0));
#line 277 "compiler/main.gem"
    GemVal _t574 = (*gem_v_skip_nl);
                    (void)(_t574.fn(_t574.env, NULL, 0));
#line 278 "compiler/main.gem"
    GemVal _t575 = (*gem_v_parse_expr);
    GemVal _t576[] = {gem_v_args, _t575.fn(_t575.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t576, 2));
                }
            }
#line 281 "compiler/main.gem"
    GemVal _t577 = (*gem_v_skip_nl);
            (void)(_t577.fn(_t577.env, NULL, 0));
#line 282 "compiler/main.gem"
    GemVal _t578[] = {gem_string(")")};
    GemVal _t579 = (*gem_v_expect);
            (void)(_t579.fn(_t579.env, _t578, 1));
#line 285 "compiler/main.gem"
    GemVal _t580 = (*gem_v_peek);
    GemVal _t581 = _t580.fn(_t580.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t581, gem_string("type")), gem_string("do")))) {
#line 286 "compiler/main.gem"
    GemVal _t582 = (*gem_v_advance);
                (void)(_t582.fn(_t582.env, NULL, 0));
#line 287 "compiler/main.gem"
    GemVal _t583 = gem_table_new();
                GemVal gem_v_bparams = _t583;
#line 288 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 289 "compiler/main.gem"
                GemVal gem_v_bblock = GEM_NIL;
#line 290 "compiler/main.gem"
    GemVal _t584 = (*gem_v_peek);
    GemVal _t585 = _t584.fn(_t584.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t585, gem_string("type")), gem_string("|")))) {
#line 291 "compiler/main.gem"
    GemVal _t586 = (*gem_v_advance);
                    (void)(_t586.fn(_t586.env, NULL, 0));
#line 292 "compiler/main.gem"
    GemVal _t587 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t587.fn(_t587.env, NULL, 0);
#line 293 "compiler/main.gem"
    GemVal _t588 = gem_v_bpr;
                    gem_v_bparams = gem_table_get(_t588, gem_string("params"));
#line 294 "compiler/main.gem"
    GemVal _t589 = gem_v_bpr;
                    gem_v_brest = gem_table_get(_t589, gem_string("rest_param"));
#line 295 "compiler/main.gem"
    GemVal _t590 = gem_v_bpr;
                    gem_v_bblock = gem_table_get(_t590, gem_string("block_param"));
#line 296 "compiler/main.gem"
    GemVal _t591[] = {gem_string("|")};
    GemVal _t592 = (*gem_v_expect);
                    (void)(_t592.fn(_t592.env, _t591, 1));
                }
#line 298 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 299 "compiler/main.gem"
    GemVal _t593 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t593.fn(_t593.env, NULL, 0);
#line 300 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 301 "compiler/main.gem"
    GemVal _t594[] = {gem_string("end")};
    GemVal _t595 = (*gem_v_expect);
                (void)(_t595.fn(_t595.env, _t594, 1));
#line 302 "compiler/main.gem"
    GemVal _t596[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, gem_v_body};
    GemVal _t597[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t596, 4)};
                (void)(gem_push_fn(NULL, _t597, 2));
            } else {
#line 306 "compiler/main.gem"
    GemVal _t598 = (*gem_v_peek);
    GemVal _t599 = _t598.fn(_t598.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t599, gem_string("type")), gem_string("{")))) {
#line 307 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 308 "compiler/main.gem"
    GemVal _t600[] = {gem_int(1)};
    GemVal _t601 = (*gem_v_peek_at);
    GemVal _t602 = _t601.fn(_t601.env, _t600, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t602, gem_string("type")), gem_string("|")))) {
#line 309 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 310 "compiler/main.gem"
    GemVal _t603[] = {gem_int(1)};
    GemVal _t604 = (*gem_v_peek_at);
    GemVal _t605 = _t604.fn(_t604.env, _t603, 1);
    GemVal _t613;
    if (gem_truthy(gem_eq(gem_table_get(_t605, gem_string("type")), gem_string("}")))) {
        _t613 = gem_eq(gem_table_get(_t605, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t606[] = {gem_int(1)};
        GemVal _t607 = (*gem_v_peek_at);
        GemVal _t608 = _t607.fn(_t607.env, _t606, 1);
        GemVal _t612;
        if (!gem_truthy(gem_eq(gem_table_get(_t608, gem_string("type")), gem_string("NAME")))) {
                _t612 = gem_eq(gem_table_get(_t608, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t609[] = {gem_int(2)};
                GemVal _t610 = (*gem_v_peek_at);
                GemVal _t611 = _t610.fn(_t610.env, _t609, 1);
                _t612 = gem_eq(gem_table_get(_t611, gem_string("type")), gem_string(":"));
        }
        _t613 = _t612;
    }
                        if (gem_truthy(_t613)) {
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
    GemVal _t614 = (*gem_v_advance);
                        (void)(_t614.fn(_t614.env, NULL, 0));
#line 317 "compiler/main.gem"
    GemVal _t615 = gem_table_new();
                        GemVal gem_v_bparams = _t615;
#line 318 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 319 "compiler/main.gem"
                        GemVal gem_v_bblock = GEM_NIL;
#line 320 "compiler/main.gem"
    GemVal _t616 = (*gem_v_peek);
    GemVal _t617 = _t616.fn(_t616.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t617, gem_string("type")), gem_string("|")))) {
#line 321 "compiler/main.gem"
    GemVal _t618 = (*gem_v_advance);
                            (void)(_t618.fn(_t618.env, NULL, 0));
#line 322 "compiler/main.gem"
    GemVal _t619 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t619.fn(_t619.env, NULL, 0);
#line 323 "compiler/main.gem"
    GemVal _t620 = gem_v_bpr;
                            gem_v_bparams = gem_table_get(_t620, gem_string("params"));
#line 324 "compiler/main.gem"
    GemVal _t621 = gem_v_bpr;
                            gem_v_brest = gem_table_get(_t621, gem_string("rest_param"));
#line 325 "compiler/main.gem"
    GemVal _t622 = gem_v_bpr;
                            gem_v_bblock = gem_table_get(_t622, gem_string("block_param"));
#line 326 "compiler/main.gem"
    GemVal _t623[] = {gem_string("|")};
    GemVal _t624 = (*gem_v_expect);
                            (void)(_t624.fn(_t624.env, _t623, 1));
                        }
#line 328 "compiler/main.gem"
    GemVal _t625 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t625.fn(_t625.env, NULL, 0);
#line 329 "compiler/main.gem"
    GemVal _t626[] = {gem_string("}")};
    GemVal _t627 = (*gem_v_expect);
                        (void)(_t627.fn(_t627.env, _t626, 1));
#line 330 "compiler/main.gem"
    GemVal _t628 = gem_table_new();
    gem_table_set(_t628, gem_int(0), gem_v_bexpr);
    GemVal _t629[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, _t628};
    GemVal _t630[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t629, 4)};
                        (void)(gem_push_fn(NULL, _t630, 2));
                    }
                }
            }
#line 335 "compiler/main.gem"
    GemVal _t631[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t631, 3);
            continue;
        }
#line 340 "compiler/main.gem"
    GemVal _t632 = (*gem_v_peek);
    GemVal _t633 = _t632.fn(_t632.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t633, gem_string("type")), gem_string(".")))) {
#line 341 "compiler/main.gem"
    GemVal _t634 = (*gem_v_advance);
            (void)(_t634.fn(_t634.env, NULL, 0));
#line 342 "compiler/main.gem"
    GemVal _t635 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t635.fn(_t635.env, NULL, 0);
#line 343 "compiler/main.gem"
    GemVal _t636 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t636, gem_string("type")), gem_string("NAME")))) {
#line 344 "compiler/main.gem"
    GemVal _t637 = (*gem_v_advance);
                (void)(_t637.fn(_t637.env, NULL, 0));
            } else {
#line 347 "compiler/main.gem"
    GemVal _t638 = (*gem_v_advance);
                (void)(_t638.fn(_t638.env, NULL, 0));
            }
#line 349 "compiler/main.gem"
    GemVal _t639 = gem_v_field_tok;
    GemVal _t640[] = {gem_v_node, gem_table_get(_t639, gem_string("value"))};
            gem_v_node = gem_fn_make_dot(NULL, _t640, 2);
            continue;
        }
#line 354 "compiler/main.gem"
    GemVal _t641 = (*gem_v_peek);
    GemVal _t642 = _t641.fn(_t641.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t642, gem_string("type")), gem_string("[")))) {
#line 355 "compiler/main.gem"
    GemVal _t643 = (*gem_v_advance);
            (void)(_t643.fn(_t643.env, NULL, 0));
#line 356 "compiler/main.gem"
    GemVal _t644 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t644.fn(_t644.env, NULL, 0);
#line 357 "compiler/main.gem"
    GemVal _t645[] = {gem_string("]")};
    GemVal _t646 = (*gem_v_expect);
            (void)(_t646.fn(_t646.env, _t645, 1));
#line 358 "compiler/main.gem"
    GemVal _t647[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t647, 2);
            continue;
        }
        break;
    }
    GemVal _t648 = gem_v_node;
    gem_pop_frame();
    return _t648;
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
    GemVal _t650 = (*gem_v_peek);
    GemVal _t651 = _t650.fn(_t650.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t651, gem_string("type")), gem_string("-")))) {
#line 371 "compiler/main.gem"
    GemVal _t652 = (*gem_v_advance);
        (void)(_t652.fn(_t652.env, NULL, 0));
#line 372 "compiler/main.gem"
    GemVal _t653 = (*gem_v_parse_unary);
    GemVal _t654[] = {gem_string("-"), _t653.fn(_t653.env, NULL, 0)};
        GemVal _t655 = gem_fn_make_unop(NULL, _t654, 2);
        gem_pop_frame();
        return _t655;
    }
#line 374 "compiler/main.gem"
    GemVal _t656 = (*gem_v_parse_call);
    GemVal _t657 = _t656.fn(_t656.env, NULL, 0);
    gem_pop_frame();
    return _t657;
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
    GemVal _t659 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t659.fn(_t659.env, NULL, 0);
#line 380 "compiler/main.gem"
    while (1) {
        GemVal _t660 = (*gem_v_peek);
        GemVal _t661 = _t660.fn(_t660.env, NULL, 0);
        GemVal _t664;
        if (gem_truthy(gem_eq(gem_table_get(_t661, gem_string("type")), gem_string("*")))) {
                _t664 = gem_eq(gem_table_get(_t661, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t662 = (*gem_v_peek);
                GemVal _t663 = _t662.fn(_t662.env, NULL, 0);
                _t664 = gem_eq(gem_table_get(_t663, gem_string("type")), gem_string("/"));
        }
        GemVal _t667;
        if (gem_truthy(_t664)) {
                _t667 = _t664;
        } else {
                GemVal _t665 = (*gem_v_peek);
                GemVal _t666 = _t665.fn(_t665.env, NULL, 0);
                _t667 = gem_eq(gem_table_get(_t666, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t667)) break;
#line 381 "compiler/main.gem"
    GemVal _t668 = (*gem_v_advance);
    GemVal _t669 = _t668.fn(_t668.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t669, gem_string("type"));
#line 382 "compiler/main.gem"
    GemVal _t670 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t670.fn(_t670.env, NULL, 0);
#line 383 "compiler/main.gem"
    GemVal _t671[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t671, 3);
    }
    GemVal _t672 = gem_v_left;
    gem_pop_frame();
    return _t672;
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
    GemVal _t674 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t674.fn(_t674.env, NULL, 0);
#line 391 "compiler/main.gem"
    while (1) {
        GemVal _t675 = (*gem_v_peek);
        GemVal _t676 = _t675.fn(_t675.env, NULL, 0);
        GemVal _t679;
        if (gem_truthy(gem_eq(gem_table_get(_t676, gem_string("type")), gem_string("+")))) {
                _t679 = gem_eq(gem_table_get(_t676, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t677 = (*gem_v_peek);
                GemVal _t678 = _t677.fn(_t677.env, NULL, 0);
                _t679 = gem_eq(gem_table_get(_t678, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t679)) break;
#line 392 "compiler/main.gem"
    GemVal _t680 = (*gem_v_advance);
    GemVal _t681 = _t680.fn(_t680.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t681, gem_string("type"));
#line 393 "compiler/main.gem"
    GemVal _t682 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t682.fn(_t682.env, NULL, 0);
#line 394 "compiler/main.gem"
    GemVal _t683[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t683, 3);
    }
    GemVal _t684 = gem_v_left;
    gem_pop_frame();
    return _t684;
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
    GemVal _t686 = (*gem_v_parse_add);
    GemVal gem_v_left = _t686.fn(_t686.env, NULL, 0);
#line 402 "compiler/main.gem"
    while (1) {
        GemVal _t687 = (*gem_v_peek);
        GemVal _t688 = _t687.fn(_t687.env, NULL, 0);
        GemVal _t691;
        if (gem_truthy(gem_eq(gem_table_get(_t688, gem_string("type")), gem_string("==")))) {
                _t691 = gem_eq(gem_table_get(_t688, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t689 = (*gem_v_peek);
                GemVal _t690 = _t689.fn(_t689.env, NULL, 0);
                _t691 = gem_eq(gem_table_get(_t690, gem_string("type")), gem_string("!="));
        }
        GemVal _t694;
        if (gem_truthy(_t691)) {
                _t694 = _t691;
        } else {
                GemVal _t692 = (*gem_v_peek);
                GemVal _t693 = _t692.fn(_t692.env, NULL, 0);
                _t694 = gem_eq(gem_table_get(_t693, gem_string("type")), gem_string("<"));
        }
        GemVal _t697;
        if (gem_truthy(_t694)) {
                _t697 = _t694;
        } else {
                GemVal _t695 = (*gem_v_peek);
                GemVal _t696 = _t695.fn(_t695.env, NULL, 0);
                _t697 = gem_eq(gem_table_get(_t696, gem_string("type")), gem_string(">"));
        }
        GemVal _t700;
        if (gem_truthy(_t697)) {
                _t700 = _t697;
        } else {
                GemVal _t698 = (*gem_v_peek);
                GemVal _t699 = _t698.fn(_t698.env, NULL, 0);
                _t700 = gem_eq(gem_table_get(_t699, gem_string("type")), gem_string("<="));
        }
        GemVal _t703;
        if (gem_truthy(_t700)) {
                _t703 = _t700;
        } else {
                GemVal _t701 = (*gem_v_peek);
                GemVal _t702 = _t701.fn(_t701.env, NULL, 0);
                _t703 = gem_eq(gem_table_get(_t702, gem_string("type")), gem_string(">="));
        }
        GemVal _t706;
        if (gem_truthy(_t703)) {
                _t706 = _t703;
        } else {
                GemVal _t704 = (*gem_v_peek);
                GemVal _t705 = _t704.fn(_t704.env, NULL, 0);
                _t706 = gem_eq(gem_table_get(_t705, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t706)) break;
#line 403 "compiler/main.gem"
    GemVal _t707 = (*gem_v_advance);
    GemVal _t708 = _t707.fn(_t707.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t708, gem_string("type"));
#line 404 "compiler/main.gem"
    GemVal _t709 = (*gem_v_parse_add);
        GemVal gem_v_right = _t709.fn(_t709.env, NULL, 0);
#line 405 "compiler/main.gem"
    GemVal _t710[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t710, 3);
    }
    GemVal _t711 = gem_v_left;
    gem_pop_frame();
    return _t711;
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
    GemVal _t713 = (*gem_v_peek);
    GemVal _t714 = _t713.fn(_t713.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t714, gem_string("type")), gem_string("not")))) {
#line 413 "compiler/main.gem"
    GemVal _t715 = (*gem_v_advance);
        (void)(_t715.fn(_t715.env, NULL, 0));
#line 414 "compiler/main.gem"
    GemVal _t716 = (*gem_v_parse_not);
    GemVal _t717[] = {gem_string("not"), _t716.fn(_t716.env, NULL, 0)};
        GemVal _t718 = gem_fn_make_unop(NULL, _t717, 2);
        gem_pop_frame();
        return _t718;
    }
#line 416 "compiler/main.gem"
    GemVal _t719 = (*gem_v_parse_compare);
    GemVal _t720 = _t719.fn(_t719.env, NULL, 0);
    gem_pop_frame();
    return _t720;
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
    GemVal _t722 = (*gem_v_parse_not);
    GemVal gem_v_left = _t722.fn(_t722.env, NULL, 0);
#line 422 "compiler/main.gem"
    while (1) {
        GemVal _t723 = (*gem_v_peek);
        GemVal _t724 = _t723.fn(_t723.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t724, gem_string("type")), gem_string("and")))) break;
#line 423 "compiler/main.gem"
    GemVal _t725 = (*gem_v_advance);
        (void)(_t725.fn(_t725.env, NULL, 0));
#line 424 "compiler/main.gem"
    GemVal _t726 = (*gem_v_parse_not);
        GemVal gem_v_right = _t726.fn(_t726.env, NULL, 0);
#line 425 "compiler/main.gem"
    GemVal _t727[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t727, 3);
    }
    GemVal _t728 = gem_v_left;
    gem_pop_frame();
    return _t728;
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
    GemVal _t730 = (*gem_v_parse_and);
    GemVal gem_v_left = _t730.fn(_t730.env, NULL, 0);
#line 433 "compiler/main.gem"
    while (1) {
        GemVal _t731 = (*gem_v_peek);
        GemVal _t732 = _t731.fn(_t731.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t732, gem_string("type")), gem_string("or")))) break;
#line 434 "compiler/main.gem"
    GemVal _t733 = (*gem_v_advance);
        (void)(_t733.fn(_t733.env, NULL, 0));
#line 435 "compiler/main.gem"
    GemVal _t734 = (*gem_v_parse_and);
        GemVal gem_v_right = _t734.fn(_t734.env, NULL, 0);
#line 436 "compiler/main.gem"
    GemVal _t735[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t735, 3);
    }
    GemVal _t736 = gem_v_left;
    gem_pop_frame();
    return _t736;
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
    GemVal _t738 = (*gem_v_peek);
    GemVal _t739 = _t738.fn(_t738.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t739, gem_string("line"));
#line 444 "compiler/main.gem"
    GemVal _t740 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t740.fn(_t740.env, NULL, 0);
#line 447 "compiler/main.gem"
    GemVal _t741 = (*gem_v_peek);
    GemVal _t742 = _t741.fn(_t741.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t742, gem_string("type")), gem_string("=")))) {
#line 448 "compiler/main.gem"
    GemVal _t743 = (*gem_v_advance);
        (void)(_t743.fn(_t743.env, NULL, 0));
#line 449 "compiler/main.gem"
    GemVal _t744 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t744.fn(_t744.env, NULL, 0);
#line 450 "compiler/main.gem"
    GemVal _t745 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t745, gem_string("tag")), gem_string("var")))) {
#line 451 "compiler/main.gem"
    GemVal _t746 = gem_v_lhs;
    GemVal _t747[] = {gem_table_get(_t746, gem_string("name")), gem_v_value, gem_v_start_line};
            GemVal _t748 = gem_fn_make_assign(NULL, _t747, 3);
            gem_pop_frame();
            return _t748;
        }
#line 453 "compiler/main.gem"
    GemVal _t749 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t749, gem_string("tag")), gem_string("dot")))) {
#line 454 "compiler/main.gem"
    GemVal _t750 = gem_v_lhs;
    GemVal _t751 = gem_v_lhs;
    GemVal _t752[] = {gem_table_get(_t750, gem_string("object")), gem_table_get(_t751, gem_string("field")), gem_v_value, gem_v_start_line};
            GemVal _t753 = gem_fn_make_dot_assign(NULL, _t752, 4);
            gem_pop_frame();
            return _t753;
        }
#line 456 "compiler/main.gem"
    GemVal _t754 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t754, gem_string("tag")), gem_string("index")))) {
#line 457 "compiler/main.gem"
    GemVal _t755 = gem_v_lhs;
    GemVal _t756 = gem_v_lhs;
    GemVal _t757[] = {gem_table_get(_t755, gem_string("object")), gem_table_get(_t756, gem_string("key")), gem_v_value, gem_v_start_line};
            GemVal _t758 = gem_fn_make_index_assign(NULL, _t757, 4);
            gem_pop_frame();
            return _t758;
        }
#line 459 "compiler/main.gem"
    GemVal _t759[] = {gem_v_start_line};
    GemVal _t760[] = {gem_add(gem_string("invalid assignment target (expected variable, field, or index) at line "), gem_to_string_fn(NULL, _t759, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 459, _t760, 1));
    }
#line 463 "compiler/main.gem"
    GemVal _t761 = (*gem_v_peek);
    GemVal _t762 = _t761.fn(_t761.env, NULL, 0);
    GemVal _t765;
    if (gem_truthy(gem_eq(gem_table_get(_t762, gem_string("type")), gem_string("+=")))) {
        _t765 = gem_eq(gem_table_get(_t762, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t763 = (*gem_v_peek);
        GemVal _t764 = _t763.fn(_t763.env, NULL, 0);
        _t765 = gem_eq(gem_table_get(_t764, gem_string("type")), gem_string("-="));
    }
    GemVal _t768;
    if (gem_truthy(_t765)) {
        _t768 = _t765;
    } else {
        GemVal _t766 = (*gem_v_peek);
        GemVal _t767 = _t766.fn(_t766.env, NULL, 0);
        _t768 = gem_eq(gem_table_get(_t767, gem_string("type")), gem_string("*="));
    }
    GemVal _t771;
    if (gem_truthy(_t768)) {
        _t771 = _t768;
    } else {
        GemVal _t769 = (*gem_v_peek);
        GemVal _t770 = _t769.fn(_t769.env, NULL, 0);
        _t771 = gem_eq(gem_table_get(_t770, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t771)) {
#line 464 "compiler/main.gem"
    GemVal _t772 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t772.fn(_t772.env, NULL, 0);
#line 465 "compiler/main.gem"
    GemVal _t773 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t773, gem_string("value")), gem_int(0));
#line 466 "compiler/main.gem"
    GemVal _t774 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t774.fn(_t774.env, NULL, 0);
#line 467 "compiler/main.gem"
    GemVal _t775 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t775, gem_string("tag")), gem_string("var")))) {
#line 468 "compiler/main.gem"
    GemVal _t776 = gem_v_op_tok;
    GemVal _t777[] = {gem_table_get(_t776, gem_string("line"))};
    GemVal _t778[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t777, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 468, _t778, 1));
        }
#line 470 "compiler/main.gem"
    GemVal _t779 = gem_v_lhs;
    GemVal _t780 = gem_v_lhs;
    GemVal _t781[] = {gem_table_get(_t780, gem_string("name"))};
    GemVal _t782[] = {gem_v_base_op, gem_fn_make_var(NULL, _t781, 1), gem_v_value};
    GemVal _t783[] = {gem_table_get(_t779, gem_string("name")), gem_fn_make_binop(NULL, _t782, 3), gem_v_start_line};
        GemVal _t784 = gem_fn_make_assign(NULL, _t783, 3);
        gem_pop_frame();
        return _t784;
    }
    GemVal _t785 = gem_v_lhs;
    gem_pop_frame();
    return _t785;
}

struct _closure__anon_21 {
};
static GemVal _anon_21(void *_env, GemVal *args, int argc) {
    GemVal gem_v_conditions = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_21", "compiler/main.gem", 0);
#line 485 "compiler/main.gem"
    GemVal _t787[] = {gem_v_conditions};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t787, 1), gem_int(0)))) {
#line 486 "compiler/main.gem"
    GemVal _t788[] = {gem_bool(1)};
        GemVal _t789 = gem_fn_make_bool(NULL, _t788, 1);
        gem_pop_frame();
        return _t789;
    }
#line 488 "compiler/main.gem"
    GemVal gem_v_result = gem_table_get(gem_v_conditions, gem_int(0));
#line 489 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(1);
#line 489 "compiler/main.gem"
    GemVal _t790[] = {gem_v_conditions};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t790, 1);
#line 489 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 489 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_1;
#line 489 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 490 "compiler/main.gem"
    GemVal _t791[] = {gem_string("and"), gem_v_result, gem_table_get(gem_v_conditions, gem_v_i)};
        gem_v_result = gem_fn_make_binop(NULL, _t791, 3);
    }

    GemVal _t792 = gem_v_result;
    gem_pop_frame();
    return _t792;
}

struct _closure__anon_20 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_parse_pattern;
    GemVal *gem_v_peek;
    GemVal *gem_v_peek_at;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_20(void *_env, GemVal *args, int argc) {
    struct _closure__anon_20 *_cls = (struct _closure__anon_20 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_parse_pattern = _cls->gem_v_parse_pattern;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_peek_at = _cls->gem_v_peek_at;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    GemVal gem_v_target_expr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_20", "compiler/main.gem", 0);
#line 484 "compiler/main.gem"
    GemVal gem_v_and_chain = gem_make_fn(_anon_21, NULL);
#line 494 "compiler/main.gem"
    GemVal _t793 = (*gem_v_peek);
    GemVal gem_v_t = _t793.fn(_t793.env, NULL, 0);
#line 497 "compiler/main.gem"
    GemVal _t794 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t794, gem_string("type")), gem_string("{")))) {
#line 498 "compiler/main.gem"
    GemVal _t795 = (*gem_v_advance);
        (void)(_t795.fn(_t795.env, NULL, 0));
#line 499 "compiler/main.gem"
    GemVal _t796 = (*gem_v_skip_nl);
        (void)(_t796.fn(_t796.env, NULL, 0));
#line 500 "compiler/main.gem"
    GemVal _t797 = gem_table_new();
    GemVal _t798[] = {gem_string("type")};
    GemVal _t799 = gem_table_new();
    gem_table_set(_t799, gem_int(0), gem_v_target_expr);
    GemVal _t800[] = {gem_fn_make_var(NULL, _t798, 1), _t799, gem_int(0)};
    GemVal _t801[] = {gem_string("table")};
    GemVal _t802[] = {gem_string("=="), gem_fn_make_call(NULL, _t800, 3), gem_fn_make_string(NULL, _t801, 1)};
    gem_table_set(_t797, gem_int(0), gem_fn_make_binop(NULL, _t802, 3));
        GemVal gem_v_conditions = _t797;
#line 501 "compiler/main.gem"
    GemVal _t803 = gem_table_new();
        GemVal gem_v_bindings = _t803;
#line 502 "compiler/main.gem"
        while (1) {
            GemVal _t804 = (*gem_v_peek);
            GemVal _t805 = _t804.fn(_t804.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t805, gem_string("type")), gem_string("}")))) break;
#line 503 "compiler/main.gem"
    GemVal _t806 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t806.fn(_t806.env, NULL, 0);
#line 504 "compiler/main.gem"
    GemVal _t807 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t807, gem_string("type")), gem_string("NAME")))) {
#line 505 "compiler/main.gem"
    GemVal _t808 = (*gem_v_advance);
                (void)(_t808.fn(_t808.env, NULL, 0));
            } else {
#line 506 "compiler/main.gem"
    GemVal _t809[] = {gem_int(1)};
    GemVal _t810 = (*gem_v_peek_at);
    GemVal _t811 = _t810.fn(_t810.env, _t809, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t811, gem_string("type")), gem_string(":")))) {
#line 508 "compiler/main.gem"
    GemVal _t812 = (*gem_v_advance);
                    (void)(_t812.fn(_t812.env, NULL, 0));
                } else {
#line 510 "compiler/main.gem"
    GemVal _t813 = gem_v_key_tok;
    GemVal _t814[] = {gem_table_get(_t813, gem_string("line"))};
    GemVal _t815[] = {gem_add(gem_string("expected key name in table pattern at line "), gem_to_string_fn(NULL, _t814, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 510, _t815, 1));
                }
            }
#line 512 "compiler/main.gem"
    GemVal _t816 = gem_v_key_tok;
            GemVal gem_v_key_name = gem_table_get(_t816, gem_string("value"));
#line 513 "compiler/main.gem"
    GemVal _t817[] = {gem_v_key_name};
            GemVal gem_v_key_str = gem_fn_make_string(NULL, _t817, 1);
#line 514 "compiler/main.gem"
    GemVal _t818[] = {gem_string("has_key")};
    GemVal _t819 = gem_table_new();
    gem_table_set(_t819, gem_int(0), gem_v_target_expr);
    gem_table_set(_t819, gem_int(1), gem_v_key_str);
    GemVal _t820[] = {gem_fn_make_var(NULL, _t818, 1), _t819, gem_int(0)};
    GemVal _t821[] = {gem_v_conditions, gem_fn_make_call(NULL, _t820, 3)};
            (void)(gem_push_fn(NULL, _t821, 2));
#line 515 "compiler/main.gem"
    GemVal _t822[] = {gem_v_target_expr, gem_v_key_str};
            GemVal gem_v_sub_target = gem_fn_make_index(NULL, _t822, 2);
#line 516 "compiler/main.gem"
    GemVal _t823 = (*gem_v_peek);
    GemVal _t824 = _t823.fn(_t823.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t824, gem_string("type")), gem_string(":")))) {
#line 517 "compiler/main.gem"
    GemVal _t825 = (*gem_v_advance);
                (void)(_t825.fn(_t825.env, NULL, 0));
#line 518 "compiler/main.gem"
    GemVal _t826 = (*gem_v_skip_nl);
                (void)(_t826.fn(_t826.env, NULL, 0));
#line 519 "compiler/main.gem"
    GemVal _t827[] = {gem_v_sub_target};
    GemVal _t828 = (*gem_v_parse_pattern);
                GemVal gem_v_sub = _t828.fn(_t828.env, _t827, 1);
#line 520 "compiler/main.gem"
    GemVal _t829 = gem_v_sub;
    GemVal _t830 = gem_table_get(_t829, gem_string("condition"));
    GemVal _t833;
    if (gem_truthy(gem_neq(gem_table_get(_t830, gem_string("tag")), gem_string("bool")))) {
        _t833 = gem_neq(gem_table_get(_t830, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t831 = gem_v_sub;
        GemVal _t832 = gem_table_get(_t831, gem_string("condition"));
        _t833 = gem_neq(gem_table_get(_t832, gem_string("value")), gem_bool(1));
    }
                if (gem_truthy(_t833)) {
#line 521 "compiler/main.gem"
    GemVal _t834 = gem_v_sub;
    GemVal _t835[] = {gem_v_conditions, gem_table_get(_t834, gem_string("condition"))};
                    (void)(gem_push_fn(NULL, _t835, 2));
                }
#line 523 "compiler/main.gem"
    GemVal _t836 = gem_v_sub;
                GemVal gem_v__for_items_2 = gem_table_get(_t836, gem_string("bindings"));
#line 523 "compiler/main.gem"
                GemVal gem_v__for_i_2 = gem_int(0);
#line 523 "compiler/main.gem"
                while (1) {
                    GemVal _t837[] = {gem_v__for_items_2};
                    if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t837, 1)))) break;
#line 523 "compiler/main.gem"
                    GemVal gem_v_b = gem_table_get(gem_v__for_items_2, gem_v__for_i_2);
#line 523 "compiler/main.gem"
                    gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 524 "compiler/main.gem"
    GemVal _t838[] = {gem_v_bindings, gem_v_b};
                    (void)(gem_push_fn(NULL, _t838, 2));
                }

            } else {
#line 528 "compiler/main.gem"
    GemVal _t839[] = {gem_v_key_name, gem_v_sub_target, gem_int(0)};
    GemVal _t840[] = {gem_v_bindings, gem_fn_make_let(NULL, _t839, 3)};
                (void)(gem_push_fn(NULL, _t840, 2));
            }
#line 530 "compiler/main.gem"
    GemVal _t841 = (*gem_v_skip_nl);
            (void)(_t841.fn(_t841.env, NULL, 0));
#line 531 "compiler/main.gem"
    GemVal _t842 = (*gem_v_peek);
    GemVal _t843 = _t842.fn(_t842.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t843, gem_string("type")), gem_string(",")))) {
#line 532 "compiler/main.gem"
    GemVal _t844 = (*gem_v_advance);
                (void)(_t844.fn(_t844.env, NULL, 0));
#line 533 "compiler/main.gem"
    GemVal _t845 = (*gem_v_skip_nl);
                (void)(_t845.fn(_t845.env, NULL, 0));
            }
        }
#line 536 "compiler/main.gem"
    GemVal _t846[] = {gem_string("}")};
    GemVal _t847 = (*gem_v_expect);
        (void)(_t847.fn(_t847.env, _t846, 1));
#line 537 "compiler/main.gem"
    GemVal _t848 = gem_table_new();
    GemVal _t849[] = {gem_v_conditions};
    GemVal _t850 = gem_v_and_chain;
    gem_table_set(_t848, gem_string("condition"), _t850.fn(_t850.env, _t849, 1));
    gem_table_set(_t848, gem_string("bindings"), gem_v_bindings);
        GemVal _t851 = _t848;
        gem_pop_frame();
        return _t851;
    }
#line 541 "compiler/main.gem"
    GemVal _t852 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t852, gem_string("type")), gem_string("[")))) {
#line 542 "compiler/main.gem"
    GemVal _t853 = (*gem_v_advance);
        (void)(_t853.fn(_t853.env, NULL, 0));
#line 543 "compiler/main.gem"
    GemVal _t854 = (*gem_v_skip_nl);
        (void)(_t854.fn(_t854.env, NULL, 0));
#line 544 "compiler/main.gem"
    GemVal _t855 = gem_table_new();
        GemVal gem_v_sub_patterns = _t855;
#line 545 "compiler/main.gem"
        GemVal gem_v_idx = gem_int(0);
#line 546 "compiler/main.gem"
        while (1) {
            GemVal _t856 = (*gem_v_peek);
            GemVal _t857 = _t856.fn(_t856.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t857, gem_string("type")), gem_string("]")))) break;
#line 547 "compiler/main.gem"
    GemVal _t858[] = {gem_v_idx};
    GemVal _t859[] = {gem_v_target_expr, gem_fn_make_int(NULL, _t858, 1)};
    GemVal _t860[] = {gem_fn_make_index(NULL, _t859, 2)};
    GemVal _t861 = (*gem_v_parse_pattern);
            GemVal gem_v_sub = _t861.fn(_t861.env, _t860, 1);
#line 548 "compiler/main.gem"
    GemVal _t862[] = {gem_v_sub_patterns, gem_v_sub};
            (void)(gem_push_fn(NULL, _t862, 2));
#line 549 "compiler/main.gem"
            gem_v_idx = gem_add(gem_v_idx, gem_int(1));
#line 550 "compiler/main.gem"
    GemVal _t863 = (*gem_v_skip_nl);
            (void)(_t863.fn(_t863.env, NULL, 0));
#line 551 "compiler/main.gem"
    GemVal _t864 = (*gem_v_peek);
    GemVal _t865 = _t864.fn(_t864.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t865, gem_string("type")), gem_string(",")))) {
#line 552 "compiler/main.gem"
    GemVal _t866 = (*gem_v_advance);
                (void)(_t866.fn(_t866.env, NULL, 0));
#line 553 "compiler/main.gem"
    GemVal _t867 = (*gem_v_skip_nl);
                (void)(_t867.fn(_t867.env, NULL, 0));
            }
        }
#line 556 "compiler/main.gem"
    GemVal _t868[] = {gem_string("]")};
    GemVal _t869 = (*gem_v_expect);
        (void)(_t869.fn(_t869.env, _t868, 1));
#line 557 "compiler/main.gem"
    GemVal _t870 = gem_table_new();
    GemVal _t871[] = {gem_string("type")};
    GemVal _t872 = gem_table_new();
    gem_table_set(_t872, gem_int(0), gem_v_target_expr);
    GemVal _t873[] = {gem_fn_make_var(NULL, _t871, 1), _t872, gem_int(0)};
    GemVal _t874[] = {gem_string("table")};
    GemVal _t875[] = {gem_string("=="), gem_fn_make_call(NULL, _t873, 3), gem_fn_make_string(NULL, _t874, 1)};
    gem_table_set(_t870, gem_int(0), gem_fn_make_binop(NULL, _t875, 3));
        GemVal gem_v_conditions = _t870;
#line 558 "compiler/main.gem"
    GemVal _t876[] = {gem_string("len")};
    GemVal _t877 = gem_table_new();
    gem_table_set(_t877, gem_int(0), gem_v_target_expr);
    GemVal _t878[] = {gem_fn_make_var(NULL, _t876, 1), _t877, gem_int(0)};
    GemVal _t879[] = {gem_v_idx};
    GemVal _t880[] = {gem_string("=="), gem_fn_make_call(NULL, _t878, 3), gem_fn_make_int(NULL, _t879, 1)};
    GemVal _t881[] = {gem_v_conditions, gem_fn_make_binop(NULL, _t880, 3)};
        (void)(gem_push_fn(NULL, _t881, 2));
#line 559 "compiler/main.gem"
    GemVal _t882 = gem_table_new();
        GemVal gem_v_bindings = _t882;
#line 560 "compiler/main.gem"
        GemVal gem_v__for_i_4 = gem_int(0);
#line 560 "compiler/main.gem"
    GemVal _t883[] = {gem_v_sub_patterns};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t883, 1);
#line 560 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 560 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_4;
#line 560 "compiler/main.gem"
            gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 561 "compiler/main.gem"
            GemVal gem_v_sub = gem_table_get(gem_v_sub_patterns, gem_v_i);
#line 562 "compiler/main.gem"
    GemVal _t884 = gem_v_sub;
    GemVal _t885 = gem_table_get(_t884, gem_string("condition"));
    GemVal _t888;
    if (gem_truthy(gem_neq(gem_table_get(_t885, gem_string("tag")), gem_string("bool")))) {
        _t888 = gem_neq(gem_table_get(_t885, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t886 = gem_v_sub;
        GemVal _t887 = gem_table_get(_t886, gem_string("condition"));
        _t888 = gem_neq(gem_table_get(_t887, gem_string("value")), gem_bool(1));
    }
            if (gem_truthy(_t888)) {
#line 563 "compiler/main.gem"
    GemVal _t889 = gem_v_sub;
    GemVal _t890[] = {gem_v_conditions, gem_table_get(_t889, gem_string("condition"))};
                (void)(gem_push_fn(NULL, _t890, 2));
            }
#line 565 "compiler/main.gem"
    GemVal _t891 = gem_v_sub;
            GemVal gem_v__for_items_3 = gem_table_get(_t891, gem_string("bindings"));
#line 565 "compiler/main.gem"
            GemVal gem_v__for_i_3 = gem_int(0);
#line 565 "compiler/main.gem"
            while (1) {
                GemVal _t892[] = {gem_v__for_items_3};
                if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_len_fn(NULL, _t892, 1)))) break;
#line 565 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_3, gem_v__for_i_3);
#line 565 "compiler/main.gem"
                gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 566 "compiler/main.gem"
    GemVal _t893[] = {gem_v_bindings, gem_v_b};
                (void)(gem_push_fn(NULL, _t893, 2));
            }

        }

#line 569 "compiler/main.gem"
    GemVal _t894 = gem_table_new();
    GemVal _t895[] = {gem_v_conditions};
    GemVal _t896 = gem_v_and_chain;
    gem_table_set(_t894, gem_string("condition"), _t896.fn(_t896.env, _t895, 1));
    gem_table_set(_t894, gem_string("bindings"), gem_v_bindings);
        GemVal _t897 = _t894;
        gem_pop_frame();
        return _t897;
    }
#line 573 "compiler/main.gem"
    GemVal _t898 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t898, gem_string("type")), gem_string("NUMBER")))) {
#line 574 "compiler/main.gem"
    GemVal _t899 = (*gem_v_advance);
        (void)(_t899.fn(_t899.env, NULL, 0));
#line 575 "compiler/main.gem"
        GemVal gem_v_lit = GEM_NIL;
#line 576 "compiler/main.gem"
    GemVal _t900 = gem_v_t;
    GemVal _t901[] = {gem_table_get(_t900, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t901, 1))) {
#line 577 "compiler/main.gem"
    GemVal _t902 = gem_v_t;
    GemVal _t903[] = {gem_table_get(_t902, gem_string("value"))};
    GemVal _t904[] = {gem_fn_atof(NULL, _t903, 1)};
            gem_v_lit = gem_fn_make_float(NULL, _t904, 1);
        } else {
#line 579 "compiler/main.gem"
    GemVal _t905 = gem_v_t;
    GemVal _t906[] = {gem_table_get(_t905, gem_string("value"))};
    GemVal _t907[] = {gem_fn_str_to_int(NULL, _t906, 1)};
            gem_v_lit = gem_fn_make_int(NULL, _t907, 1);
        }
#line 581 "compiler/main.gem"
    GemVal _t908 = gem_table_new();
    GemVal _t909[] = {gem_string("=="), gem_v_target_expr, gem_v_lit};
    gem_table_set(_t908, gem_string("condition"), gem_fn_make_binop(NULL, _t909, 3));
    GemVal _t910 = gem_table_new();
    gem_table_set(_t908, gem_string("bindings"), _t910);
        GemVal _t911 = _t908;
        gem_pop_frame();
        return _t911;
    }
#line 583 "compiler/main.gem"
    GemVal _t912 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t912, gem_string("type")), gem_string("STRING")))) {
#line 584 "compiler/main.gem"
    GemVal _t913 = (*gem_v_advance);
        (void)(_t913.fn(_t913.env, NULL, 0));
#line 585 "compiler/main.gem"
    GemVal _t914 = gem_table_new();
    GemVal _t915 = gem_v_t;
    GemVal _t916[] = {gem_table_get(_t915, gem_string("value"))};
    GemVal _t917[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_string(NULL, _t916, 1)};
    gem_table_set(_t914, gem_string("condition"), gem_fn_make_binop(NULL, _t917, 3));
    GemVal _t918 = gem_table_new();
    gem_table_set(_t914, gem_string("bindings"), _t918);
        GemVal _t919 = _t914;
        gem_pop_frame();
        return _t919;
    }
#line 587 "compiler/main.gem"
    GemVal _t920 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t920, gem_string("type")), gem_string("true")))) {
#line 588 "compiler/main.gem"
    GemVal _t921 = (*gem_v_advance);
        (void)(_t921.fn(_t921.env, NULL, 0));
#line 589 "compiler/main.gem"
    GemVal _t922 = gem_table_new();
    GemVal _t923[] = {gem_bool(1)};
    GemVal _t924[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_bool(NULL, _t923, 1)};
    gem_table_set(_t922, gem_string("condition"), gem_fn_make_binop(NULL, _t924, 3));
    GemVal _t925 = gem_table_new();
    gem_table_set(_t922, gem_string("bindings"), _t925);
        GemVal _t926 = _t922;
        gem_pop_frame();
        return _t926;
    }
#line 591 "compiler/main.gem"
    GemVal _t927 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t927, gem_string("type")), gem_string("false")))) {
#line 592 "compiler/main.gem"
    GemVal _t928 = (*gem_v_advance);
        (void)(_t928.fn(_t928.env, NULL, 0));
#line 593 "compiler/main.gem"
    GemVal _t929 = gem_table_new();
    GemVal _t930[] = {gem_bool(0)};
    GemVal _t931[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_bool(NULL, _t930, 1)};
    gem_table_set(_t929, gem_string("condition"), gem_fn_make_binop(NULL, _t931, 3));
    GemVal _t932 = gem_table_new();
    gem_table_set(_t929, gem_string("bindings"), _t932);
        GemVal _t933 = _t929;
        gem_pop_frame();
        return _t933;
    }
#line 595 "compiler/main.gem"
    GemVal _t934 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t934, gem_string("type")), gem_string("nil")))) {
#line 596 "compiler/main.gem"
    GemVal _t935 = (*gem_v_advance);
        (void)(_t935.fn(_t935.env, NULL, 0));
#line 597 "compiler/main.gem"
    GemVal _t936 = gem_table_new();
    GemVal _t937[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_nil_node(NULL, NULL, 0)};
    gem_table_set(_t936, gem_string("condition"), gem_fn_make_binop(NULL, _t937, 3));
    GemVal _t938 = gem_table_new();
    gem_table_set(_t936, gem_string("bindings"), _t938);
        GemVal _t939 = _t936;
        gem_pop_frame();
        return _t939;
    }
#line 601 "compiler/main.gem"
    GemVal _t940 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t940, gem_string("type")), gem_string("NAME")))) {
#line 602 "compiler/main.gem"
    GemVal _t941 = (*gem_v_advance);
        (void)(_t941.fn(_t941.env, NULL, 0));
#line 603 "compiler/main.gem"
    GemVal _t942 = gem_table_new();
    GemVal _t943[] = {gem_bool(1)};
    gem_table_set(_t942, gem_string("condition"), gem_fn_make_bool(NULL, _t943, 1));
    GemVal _t944 = gem_table_new();
    GemVal _t945 = gem_v_t;
    GemVal _t946[] = {gem_table_get(_t945, gem_string("value")), gem_v_target_expr, gem_int(0)};
    gem_table_set(_t944, gem_int(0), gem_fn_make_let(NULL, _t946, 3));
    gem_table_set(_t942, gem_string("bindings"), _t944);
        GemVal _t947 = _t942;
        gem_pop_frame();
        return _t947;
    }
#line 606 "compiler/main.gem"
    GemVal _t948 = gem_v_t;
    GemVal _t949[] = {gem_table_get(_t948, gem_string("type"))};
    GemVal _t950 = gem_v_t;
    GemVal _t951[] = {gem_table_get(_t950, gem_string("line"))};
    GemVal _t952[] = {gem_add(gem_add(gem_add(gem_string("expected pattern but got '"), gem_to_string_fn(NULL, _t949, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t951, 1))};
    GemVal _t953 = gem_error_at_fn("compiler/main.gem", 606, _t952, 1);
    gem_pop_frame();
    return _t953;
}

struct _closure__anon_22 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_fn_depth;
    GemVal *gem_v_gensym_counter;
    GemVal *gem_v_parse_body;
    GemVal *gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body;
    GemVal *gem_v_parse_params;
    GemVal *gem_v_parse_pattern;
    GemVal *gem_v_parse_stmt;
    GemVal *gem_v_peek;
    GemVal *gem_v_peek_at;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_22(void *_env, GemVal *args, int argc) {
    struct _closure__anon_22 *_cls = (struct _closure__anon_22 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_fn_depth = _cls->gem_v_fn_depth;
    GemVal *gem_v_gensym_counter = _cls->gem_v_gensym_counter;
    GemVal *gem_v_parse_body = _cls->gem_v_parse_body;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body = _cls->gem_v_parse_fn_body;
    GemVal *gem_v_parse_params = _cls->gem_v_parse_params;
    GemVal *gem_v_parse_pattern = _cls->gem_v_parse_pattern;
    GemVal *gem_v_parse_stmt = _cls->gem_v_parse_stmt;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_peek_at = _cls->gem_v_peek_at;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    gem_push_frame("_anon_22", "compiler/main.gem", 0);
#line 612 "compiler/main.gem"
    GemVal _t955 = (*gem_v_peek);
    GemVal gem_v_t = _t955.fn(_t955.env, NULL, 0);
#line 615 "compiler/main.gem"
    GemVal _t956 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t956, gem_string("type")), gem_string("let")))) {
#line 616 "compiler/main.gem"
    GemVal _t957 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t957, gem_string("line"));
#line 617 "compiler/main.gem"
    GemVal _t958 = (*gem_v_advance);
        (void)(_t958.fn(_t958.env, NULL, 0));
#line 620 "compiler/main.gem"
    GemVal _t959 = (*gem_v_peek);
    GemVal _t960 = _t959.fn(_t959.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t960, gem_string("type")), gem_string("{")))) {
#line 621 "compiler/main.gem"
    GemVal _t961 = (*gem_v_advance);
            (void)(_t961.fn(_t961.env, NULL, 0));
#line 622 "compiler/main.gem"
    GemVal _t962 = gem_table_new();
            GemVal gem_v_names = _t962;
#line 623 "compiler/main.gem"
    GemVal _t963 = (*gem_v_skip_nl);
            (void)(_t963.fn(_t963.env, NULL, 0));
#line 624 "compiler/main.gem"
            while (1) {
                GemVal _t964 = (*gem_v_peek);
                GemVal _t965 = _t964.fn(_t964.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t965, gem_string("type")), gem_string("}")))) break;
#line 625 "compiler/main.gem"
    GemVal _t966 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t966.fn(_t966.env, NULL, 0);
#line 626 "compiler/main.gem"
    GemVal _t967 = gem_v_field_tok;
                if (gem_truthy(gem_eq(gem_table_get(_t967, gem_string("type")), gem_string("NAME")))) {
#line 627 "compiler/main.gem"
    GemVal _t968 = (*gem_v_advance);
                    (void)(_t968.fn(_t968.env, NULL, 0));
                } else {
#line 630 "compiler/main.gem"
    GemVal _t969 = (*gem_v_advance);
                    (void)(_t969.fn(_t969.env, NULL, 0));
                }
#line 632 "compiler/main.gem"
    GemVal _t970 = gem_v_field_tok;
    GemVal _t971[] = {gem_v_names, gem_table_get(_t970, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t971, 2));
#line 633 "compiler/main.gem"
    GemVal _t972 = (*gem_v_skip_nl);
                (void)(_t972.fn(_t972.env, NULL, 0));
#line 634 "compiler/main.gem"
    GemVal _t973 = (*gem_v_peek);
    GemVal _t974 = _t973.fn(_t973.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t974, gem_string("type")), gem_string(",")))) {
#line 635 "compiler/main.gem"
    GemVal _t975 = (*gem_v_advance);
                    (void)(_t975.fn(_t975.env, NULL, 0));
#line 636 "compiler/main.gem"
    GemVal _t976 = (*gem_v_skip_nl);
                    (void)(_t976.fn(_t976.env, NULL, 0));
                }
            }
#line 639 "compiler/main.gem"
    GemVal _t977[] = {gem_string("}")};
    GemVal _t978 = (*gem_v_expect);
            (void)(_t978.fn(_t978.env, _t977, 1));
#line 640 "compiler/main.gem"
    GemVal _t979[] = {gem_string("=")};
    GemVal _t980 = (*gem_v_expect);
            (void)(_t980.fn(_t980.env, _t979, 1));
#line 641 "compiler/main.gem"
    GemVal _t981 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t981.fn(_t981.env, NULL, 0);
#line 642 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 643 "compiler/main.gem"
    GemVal _t982[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t982, 1));
#line 644 "compiler/main.gem"
    GemVal _t983 = gem_table_new();
    GemVal _t984[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t983, gem_int(0), gem_fn_make_let(NULL, _t984, 3));
            GemVal gem_v_stmts = _t983;
#line 645 "compiler/main.gem"
            GemVal gem_v__for_i_5 = gem_int(0);
#line 645 "compiler/main.gem"
    GemVal _t985[] = {gem_v_names};
            GemVal gem_v__for_limit_5 = gem_len_fn(NULL, _t985, 1);
#line 645 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_limit_5))) break;
#line 645 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_5;
#line 645 "compiler/main.gem"
                gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 646 "compiler/main.gem"
    GemVal _t986[] = {gem_v_tmp};
    GemVal _t987[] = {gem_fn_make_var(NULL, _t986, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t988[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_dot(NULL, _t987, 2), gem_v_line};
    GemVal _t989[] = {gem_v_stmts, gem_fn_make_let(NULL, _t988, 3)};
                (void)(gem_push_fn(NULL, _t989, 2));
            }

#line 648 "compiler/main.gem"
    GemVal _t990 = gem_table_new();
    gem_table_set(_t990, gem_string("tag"), gem_string("block"));
    gem_table_set(_t990, gem_string("stmts"), gem_v_stmts);
            GemVal _t991 = _t990;
            gem_pop_frame();
            return _t991;
        }
#line 652 "compiler/main.gem"
    GemVal _t992 = (*gem_v_peek);
    GemVal _t993 = _t992.fn(_t992.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t993, gem_string("type")), gem_string("[")))) {
#line 653 "compiler/main.gem"
    GemVal _t994 = (*gem_v_advance);
            (void)(_t994.fn(_t994.env, NULL, 0));
#line 654 "compiler/main.gem"
    GemVal _t995 = gem_table_new();
            GemVal gem_v_names = _t995;
#line 655 "compiler/main.gem"
    GemVal _t996 = (*gem_v_skip_nl);
            (void)(_t996.fn(_t996.env, NULL, 0));
#line 656 "compiler/main.gem"
            while (1) {
                GemVal _t997 = (*gem_v_peek);
                GemVal _t998 = _t997.fn(_t997.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t998, gem_string("type")), gem_string("]")))) break;
#line 657 "compiler/main.gem"
    GemVal _t999[] = {gem_string("NAME")};
    GemVal _t1000 = (*gem_v_expect);
    GemVal _t1001 = _t1000.fn(_t1000.env, _t999, 1);
    GemVal _t1002[] = {gem_v_names, gem_table_get(_t1001, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1002, 2));
#line 658 "compiler/main.gem"
    GemVal _t1003 = (*gem_v_skip_nl);
                (void)(_t1003.fn(_t1003.env, NULL, 0));
#line 659 "compiler/main.gem"
    GemVal _t1004 = (*gem_v_peek);
    GemVal _t1005 = _t1004.fn(_t1004.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1005, gem_string("type")), gem_string(",")))) {
#line 660 "compiler/main.gem"
    GemVal _t1006 = (*gem_v_advance);
                    (void)(_t1006.fn(_t1006.env, NULL, 0));
#line 661 "compiler/main.gem"
    GemVal _t1007 = (*gem_v_skip_nl);
                    (void)(_t1007.fn(_t1007.env, NULL, 0));
                }
            }
#line 664 "compiler/main.gem"
    GemVal _t1008[] = {gem_string("]")};
    GemVal _t1009 = (*gem_v_expect);
            (void)(_t1009.fn(_t1009.env, _t1008, 1));
#line 665 "compiler/main.gem"
    GemVal _t1010[] = {gem_string("=")};
    GemVal _t1011 = (*gem_v_expect);
            (void)(_t1011.fn(_t1011.env, _t1010, 1));
#line 666 "compiler/main.gem"
    GemVal _t1012 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1012.fn(_t1012.env, NULL, 0);
#line 667 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 668 "compiler/main.gem"
    GemVal _t1013[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1013, 1));
#line 669 "compiler/main.gem"
    GemVal _t1014 = gem_table_new();
    GemVal _t1015[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t1014, gem_int(0), gem_fn_make_let(NULL, _t1015, 3));
            GemVal gem_v_stmts = _t1014;
#line 670 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 670 "compiler/main.gem"
    GemVal _t1016[] = {gem_v_names};
            GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t1016, 1);
#line 670 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
#line 670 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_6;
#line 670 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 671 "compiler/main.gem"
    GemVal _t1017[] = {gem_v_tmp};
    GemVal _t1018[] = {gem_v_di};
    GemVal _t1019[] = {gem_fn_make_var(NULL, _t1017, 1), gem_fn_make_int(NULL, _t1018, 1)};
    GemVal _t1020[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_index(NULL, _t1019, 2), gem_v_line};
    GemVal _t1021[] = {gem_v_stmts, gem_fn_make_let(NULL, _t1020, 3)};
                (void)(gem_push_fn(NULL, _t1021, 2));
            }

#line 673 "compiler/main.gem"
    GemVal _t1022 = gem_table_new();
    gem_table_set(_t1022, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1022, gem_string("stmts"), gem_v_stmts);
            GemVal _t1023 = _t1022;
            gem_pop_frame();
            return _t1023;
        }
#line 676 "compiler/main.gem"
    GemVal _t1024[] = {gem_string("NAME")};
    GemVal _t1025 = (*gem_v_expect);
    GemVal _t1026 = _t1025.fn(_t1025.env, _t1024, 1);
        GemVal gem_v_name = gem_table_get(_t1026, gem_string("value"));
#line 677 "compiler/main.gem"
    GemVal _t1027[] = {gem_string("=")};
    GemVal _t1028 = (*gem_v_expect);
        (void)(_t1028.fn(_t1028.env, _t1027, 1));
#line 678 "compiler/main.gem"
    GemVal _t1029 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t1029.fn(_t1029.env, NULL, 0);
#line 679 "compiler/main.gem"
    GemVal _t1030[] = {gem_v_name, gem_v_value, gem_v_line};
        GemVal _t1031 = gem_fn_make_let(NULL, _t1030, 3);
        gem_pop_frame();
        return _t1031;
    }
#line 683 "compiler/main.gem"
    GemVal _t1032 = gem_v_t;
    GemVal _t1036;
    if (!gem_truthy(gem_eq(gem_table_get(_t1032, gem_string("type")), gem_string("fn")))) {
        _t1036 = gem_eq(gem_table_get(_t1032, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t1033[] = {gem_int(1)};
        GemVal _t1034 = (*gem_v_peek_at);
        GemVal _t1035 = _t1034.fn(_t1034.env, _t1033, 1);
        _t1036 = gem_eq(gem_table_get(_t1035, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t1036)) {
#line 684 "compiler/main.gem"
    GemVal _t1037 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1037, gem_string("line"));
#line 685 "compiler/main.gem"
    GemVal _t1038 = (*gem_v_advance);
        (void)(_t1038.fn(_t1038.env, NULL, 0));
#line 686 "compiler/main.gem"
    GemVal _t1039[] = {gem_string("NAME")};
    GemVal _t1040 = (*gem_v_expect);
    GemVal _t1041 = _t1040.fn(_t1040.env, _t1039, 1);
        GemVal gem_v_name = gem_table_get(_t1041, gem_string("value"));
#line 687 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 688 "compiler/main.gem"
    GemVal _t1042[] = {gem_v_name};
    GemVal _t1043[] = {gem_v_line};
    GemVal _t1044[] = {gem_v_name};
    GemVal _t1045[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("named fn '"), gem_to_string_fn(NULL, _t1042, 1)), gem_string("' inside function body is not supported at line ")), gem_to_string_fn(NULL, _t1043, 1)), gem_string(" — use: let ")), gem_to_string_fn(NULL, _t1044, 1)), gem_string(" = fn(...) ... end"))};
            (void)(gem_error_at_fn("compiler/main.gem", 688, _t1045, 1));
        }
#line 690 "compiler/main.gem"
    GemVal _t1046[] = {gem_string("(")};
    GemVal _t1047 = (*gem_v_expect);
        (void)(_t1047.fn(_t1047.env, _t1046, 1));
#line 691 "compiler/main.gem"
    GemVal _t1048 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t1048.fn(_t1048.env, NULL, 0);
#line 692 "compiler/main.gem"
    GemVal _t1049[] = {gem_string(")")};
    GemVal _t1050 = (*gem_v_expect);
        (void)(_t1050.fn(_t1050.env, _t1049, 1));
#line 693 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 694 "compiler/main.gem"
    GemVal _t1051 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t1051.fn(_t1051.env, NULL, 0);
#line 695 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 696 "compiler/main.gem"
    GemVal _t1052[] = {gem_string("end")};
    GemVal _t1053 = (*gem_v_expect);
        (void)(_t1053.fn(_t1053.env, _t1052, 1));
#line 697 "compiler/main.gem"
    GemVal _t1054 = gem_v_pr;
    GemVal _t1055 = gem_v_pr;
    GemVal _t1056 = gem_v_pr;
    GemVal _t1057[] = {gem_v_name, gem_table_get(_t1054, gem_string("params")), gem_table_get(_t1055, gem_string("rest_param")), gem_table_get(_t1056, gem_string("block_param")), gem_v_body, gem_v_line};
        GemVal _t1058 = gem_fn_make_fn_def(NULL, _t1057, 6);
        gem_pop_frame();
        return _t1058;
    }
#line 701 "compiler/main.gem"
    GemVal _t1059 = gem_v_t;
    GemVal _t1061;
    if (gem_truthy(gem_eq(gem_table_get(_t1059, gem_string("type")), gem_string("if")))) {
        _t1061 = gem_eq(gem_table_get(_t1059, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t1060 = gem_v_t;
        _t1061 = gem_eq(gem_table_get(_t1060, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t1061)) {
#line 702 "compiler/main.gem"
    GemVal _t1062 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1062, gem_string("line"));
#line 703 "compiler/main.gem"
    GemVal _t1063 = (*gem_v_advance);
        (void)(_t1063.fn(_t1063.env, NULL, 0));
#line 704 "compiler/main.gem"
    GemVal _t1064 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1064.fn(_t1064.env, NULL, 0);
#line 705 "compiler/main.gem"
    GemVal _t1065 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t1065.fn(_t1065.env, NULL, 0);
#line 706 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 707 "compiler/main.gem"
    GemVal _t1066 = (*gem_v_peek);
    GemVal _t1067 = _t1066.fn(_t1066.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1067, gem_string("type")), gem_string("elif")))) {
#line 709 "compiler/main.gem"
    GemVal _t1068 = gem_table_new();
    GemVal _t1069 = (*gem_v_parse_stmt);
    gem_table_set(_t1068, gem_int(0), _t1069.fn(_t1069.env, NULL, 0));
            gem_v_else_body = _t1068;
        } else {
#line 710 "compiler/main.gem"
    GemVal _t1070 = (*gem_v_peek);
    GemVal _t1071 = _t1070.fn(_t1070.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1071, gem_string("type")), gem_string("else")))) {
#line 711 "compiler/main.gem"
    GemVal _t1072 = (*gem_v_advance);
                (void)(_t1072.fn(_t1072.env, NULL, 0));
#line 712 "compiler/main.gem"
    GemVal _t1073 = (*gem_v_parse_body);
                gem_v_else_body = _t1073.fn(_t1073.env, NULL, 0);
            }
        }
#line 715 "compiler/main.gem"
    GemVal _t1074 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t1074, gem_string("type")), gem_string("if")))) {
#line 716 "compiler/main.gem"
    GemVal _t1075[] = {gem_string("end")};
    GemVal _t1076 = (*gem_v_expect);
            (void)(_t1076.fn(_t1076.env, _t1075, 1));
        }
#line 718 "compiler/main.gem"
    GemVal _t1077[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        GemVal _t1078 = gem_fn_make_if(NULL, _t1077, 4);
        gem_pop_frame();
        return _t1078;
    }
#line 722 "compiler/main.gem"
    GemVal _t1079 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1079, gem_string("type")), gem_string("while")))) {
#line 723 "compiler/main.gem"
    GemVal _t1080 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1080, gem_string("line"));
#line 724 "compiler/main.gem"
    GemVal _t1081 = (*gem_v_advance);
        (void)(_t1081.fn(_t1081.env, NULL, 0));
#line 725 "compiler/main.gem"
    GemVal _t1082 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1082.fn(_t1082.env, NULL, 0);
#line 726 "compiler/main.gem"
    GemVal _t1083 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t1083.fn(_t1083.env, NULL, 0);
#line 727 "compiler/main.gem"
    GemVal _t1084[] = {gem_string("end")};
    GemVal _t1085 = (*gem_v_expect);
        (void)(_t1085.fn(_t1085.env, _t1084, 1));
#line 728 "compiler/main.gem"
    GemVal _t1086[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        GemVal _t1087 = gem_fn_make_while(NULL, _t1086, 3);
        gem_pop_frame();
        return _t1087;
    }
#line 732 "compiler/main.gem"
    GemVal _t1088 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1088, gem_string("type")), gem_string("for")))) {
#line 733 "compiler/main.gem"
    GemVal _t1089 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1089, gem_string("line"));
#line 734 "compiler/main.gem"
    GemVal _t1090 = (*gem_v_advance);
        (void)(_t1090.fn(_t1090.env, NULL, 0));
#line 735 "compiler/main.gem"
    GemVal _t1091[] = {gem_string("NAME")};
    GemVal _t1092 = (*gem_v_expect);
    GemVal _t1093 = _t1092.fn(_t1092.env, _t1091, 1);
        GemVal gem_v_var_name = gem_table_get(_t1093, gem_string("value"));
#line 737 "compiler/main.gem"
    GemVal _t1094 = (*gem_v_peek);
    GemVal _t1095 = _t1094.fn(_t1094.env, NULL, 0);
    GemVal _t1099;
    if (!gem_truthy(gem_eq(gem_table_get(_t1095, gem_string("type")), gem_string(",")))) {
        _t1099 = gem_eq(gem_table_get(_t1095, gem_string("type")), gem_string(","));
    } else {
        GemVal _t1096[] = {gem_int(1)};
        GemVal _t1097 = (*gem_v_peek_at);
        GemVal _t1098 = _t1097.fn(_t1097.env, _t1096, 1);
        _t1099 = gem_eq(gem_table_get(_t1098, gem_string("type")), gem_string("NAME"));
    }
        if (gem_truthy(_t1099)) {
#line 739 "compiler/main.gem"
    GemVal _t1100 = (*gem_v_advance);
            (void)(_t1100.fn(_t1100.env, NULL, 0));
#line 740 "compiler/main.gem"
    GemVal _t1101[] = {gem_string("NAME")};
    GemVal _t1102 = (*gem_v_expect);
    GemVal _t1103 = _t1102.fn(_t1102.env, _t1101, 1);
            GemVal gem_v_val_name = gem_table_get(_t1103, gem_string("value"));
#line 741 "compiler/main.gem"
    GemVal _t1104[] = {gem_string("in")};
    GemVal _t1105 = (*gem_v_expect);
            (void)(_t1105.fn(_t1105.env, _t1104, 1));
#line 742 "compiler/main.gem"
    GemVal _t1106 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t1106.fn(_t1106.env, NULL, 0);
#line 743 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 744 "compiler/main.gem"
    GemVal _t1107[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_add(gem_string("_for_tbl_"), gem_to_string_fn(NULL, _t1107, 1));
#line 745 "compiler/main.gem"
    GemVal _t1108[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_keys_var = gem_add(gem_string("_for_keys_"), gem_to_string_fn(NULL, _t1108, 1));
#line 746 "compiler/main.gem"
    GemVal _t1109[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1109, 1));
#line 747 "compiler/main.gem"
    GemVal _t1110 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t1110.fn(_t1110.env, NULL, 0);
#line 748 "compiler/main.gem"
    GemVal _t1111[] = {gem_string("end")};
    GemVal _t1112 = (*gem_v_expect);
            (void)(_t1112.fn(_t1112.env, _t1111, 1));
#line 749 "compiler/main.gem"
    GemVal _t1113 = gem_table_new();
            GemVal gem_v_inner_stmts = _t1113;
#line 750 "compiler/main.gem"
    GemVal _t1114[] = {gem_v_keys_var};
    GemVal _t1115[] = {gem_v_idx_var};
    GemVal _t1116[] = {gem_fn_make_var(NULL, _t1114, 1), gem_fn_make_var(NULL, _t1115, 1)};
    GemVal _t1117[] = {gem_v_var_name, gem_fn_make_index(NULL, _t1116, 2), gem_v_line};
    GemVal _t1118[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1117, 3)};
            (void)(gem_push_fn(NULL, _t1118, 2));
#line 751 "compiler/main.gem"
    GemVal _t1119[] = {gem_v_tbl_var};
    GemVal _t1120[] = {gem_v_keys_var};
    GemVal _t1121[] = {gem_v_idx_var};
    GemVal _t1122[] = {gem_fn_make_var(NULL, _t1120, 1), gem_fn_make_var(NULL, _t1121, 1)};
    GemVal _t1123[] = {gem_fn_make_var(NULL, _t1119, 1), gem_fn_make_index(NULL, _t1122, 2)};
    GemVal _t1124[] = {gem_v_val_name, gem_fn_make_index(NULL, _t1123, 2), gem_v_line};
    GemVal _t1125[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1124, 3)};
            (void)(gem_push_fn(NULL, _t1125, 2));
#line 752 "compiler/main.gem"
    GemVal _t1126[] = {gem_v_idx_var};
    GemVal _t1127[] = {gem_int(1)};
    GemVal _t1128[] = {gem_string("+"), gem_fn_make_var(NULL, _t1126, 1), gem_fn_make_int(NULL, _t1127, 1)};
    GemVal _t1129[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1128, 3), gem_v_line};
    GemVal _t1130[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1129, 3)};
            (void)(gem_push_fn(NULL, _t1130, 2));
#line 753 "compiler/main.gem"
            GemVal gem_v__for_i_7 = gem_int(0);
#line 753 "compiler/main.gem"
    GemVal _t1131[] = {gem_v_fbody};
            GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1131, 1);
#line 753 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 753 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_7;
#line 753 "compiler/main.gem"
                gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 754 "compiler/main.gem"
    GemVal _t1132[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t1132, 2));
            }

#line 756 "compiler/main.gem"
    GemVal _t1133[] = {gem_v_idx_var};
    GemVal _t1134[] = {gem_string("len")};
    GemVal _t1135 = gem_table_new();
    GemVal _t1136[] = {gem_v_keys_var};
    gem_table_set(_t1135, gem_int(0), gem_fn_make_var(NULL, _t1136, 1));
    GemVal _t1137[] = {gem_fn_make_var(NULL, _t1134, 1), _t1135, gem_int(0)};
    GemVal _t1138[] = {gem_string("<"), gem_fn_make_var(NULL, _t1133, 1), gem_fn_make_call(NULL, _t1137, 3)};
    GemVal _t1139[] = {gem_fn_make_binop(NULL, _t1138, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1139, 3);
#line 761 "compiler/main.gem"
    GemVal _t1140 = gem_table_new();
    gem_table_set(_t1140, gem_string("tag"), gem_string("block"));
    GemVal _t1141 = gem_table_new();
    GemVal _t1142[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    gem_table_set(_t1141, gem_int(0), gem_fn_make_let(NULL, _t1142, 3));
    GemVal _t1143[] = {gem_string("keys")};
    GemVal _t1144 = gem_table_new();
    GemVal _t1145[] = {gem_v_tbl_var};
    gem_table_set(_t1144, gem_int(0), gem_fn_make_var(NULL, _t1145, 1));
    GemVal _t1146[] = {gem_fn_make_var(NULL, _t1143, 1), _t1144, gem_int(0)};
    GemVal _t1147[] = {gem_v_keys_var, gem_fn_make_call(NULL, _t1146, 3), gem_v_line};
    gem_table_set(_t1141, gem_int(1), gem_fn_make_let(NULL, _t1147, 3));
    GemVal _t1148[] = {gem_int(0)};
    GemVal _t1149[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t1148, 1), gem_v_line};
    gem_table_set(_t1141, gem_int(2), gem_fn_make_let(NULL, _t1149, 3));
    gem_table_set(_t1141, gem_int(3), gem_v_while_node);
    gem_table_set(_t1140, gem_string("stmts"), _t1141);
            GemVal _t1150 = _t1140;
            gem_pop_frame();
            return _t1150;
        } else {
#line 767 "compiler/main.gem"
    GemVal _t1151 = (*gem_v_peek);
    GemVal _t1152 = _t1151.fn(_t1151.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1152, gem_string("type")), gem_string("in")))) {
#line 769 "compiler/main.gem"
    GemVal _t1153 = (*gem_v_advance);
                (void)(_t1153.fn(_t1153.env, NULL, 0));
#line 770 "compiler/main.gem"
    GemVal _t1154 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t1154.fn(_t1154.env, NULL, 0);
#line 771 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 772 "compiler/main.gem"
    GemVal _t1155[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t1155, 1));
#line 773 "compiler/main.gem"
    GemVal _t1156[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1156, 1));
#line 774 "compiler/main.gem"
    GemVal _t1157 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t1157.fn(_t1157.env, NULL, 0);
#line 775 "compiler/main.gem"
    GemVal _t1158[] = {gem_string("end")};
    GemVal _t1159 = (*gem_v_expect);
                (void)(_t1159.fn(_t1159.env, _t1158, 1));
#line 776 "compiler/main.gem"
    GemVal _t1160 = gem_table_new();
                GemVal gem_v_inner_stmts = _t1160;
#line 777 "compiler/main.gem"
    GemVal _t1161[] = {gem_v_items_var};
    GemVal _t1162[] = {gem_v_idx_var};
    GemVal _t1163[] = {gem_fn_make_var(NULL, _t1161, 1), gem_fn_make_var(NULL, _t1162, 1)};
    GemVal _t1164[] = {gem_v_var_name, gem_fn_make_index(NULL, _t1163, 2), gem_v_line};
    GemVal _t1165[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1164, 3)};
                (void)(gem_push_fn(NULL, _t1165, 2));
#line 778 "compiler/main.gem"
    GemVal _t1166[] = {gem_v_idx_var};
    GemVal _t1167[] = {gem_int(1)};
    GemVal _t1168[] = {gem_string("+"), gem_fn_make_var(NULL, _t1166, 1), gem_fn_make_int(NULL, _t1167, 1)};
    GemVal _t1169[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1168, 3), gem_v_line};
    GemVal _t1170[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1169, 3)};
                (void)(gem_push_fn(NULL, _t1170, 2));
#line 779 "compiler/main.gem"
                GemVal gem_v__for_i_8 = gem_int(0);
#line 779 "compiler/main.gem"
    GemVal _t1171[] = {gem_v_fbody};
                GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1171, 1);
#line 779 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 779 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_8;
#line 779 "compiler/main.gem"
                    gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 780 "compiler/main.gem"
    GemVal _t1172[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t1172, 2));
                }

#line 782 "compiler/main.gem"
    GemVal _t1173[] = {gem_v_idx_var};
    GemVal _t1174[] = {gem_string("len")};
    GemVal _t1175 = gem_table_new();
    GemVal _t1176[] = {gem_v_items_var};
    gem_table_set(_t1175, gem_int(0), gem_fn_make_var(NULL, _t1176, 1));
    GemVal _t1177[] = {gem_fn_make_var(NULL, _t1174, 1), _t1175, gem_int(0)};
    GemVal _t1178[] = {gem_string("<"), gem_fn_make_var(NULL, _t1173, 1), gem_fn_make_call(NULL, _t1177, 3)};
    GemVal _t1179[] = {gem_fn_make_binop(NULL, _t1178, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1179, 3);
#line 787 "compiler/main.gem"
    GemVal _t1180 = gem_table_new();
    gem_table_set(_t1180, gem_string("tag"), gem_string("block"));
    GemVal _t1181 = gem_table_new();
    GemVal _t1182[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t1181, gem_int(0), gem_fn_make_let(NULL, _t1182, 3));
    GemVal _t1183[] = {gem_int(0)};
    GemVal _t1184[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t1183, 1), gem_v_line};
    gem_table_set(_t1181, gem_int(1), gem_fn_make_let(NULL, _t1184, 3));
    gem_table_set(_t1181, gem_int(2), gem_v_while_node);
    gem_table_set(_t1180, gem_string("stmts"), _t1181);
                GemVal _t1185 = _t1180;
                gem_pop_frame();
                return _t1185;
            } else {
#line 792 "compiler/main.gem"
    GemVal _t1186 = (*gem_v_peek);
    GemVal _t1187 = _t1186.fn(_t1186.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1187, gem_string("type")), gem_string("=")))) {
#line 794 "compiler/main.gem"
    GemVal _t1188 = (*gem_v_advance);
                    (void)(_t1188.fn(_t1188.env, NULL, 0));
#line 795 "compiler/main.gem"
    GemVal _t1189 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t1189.fn(_t1189.env, NULL, 0);
#line 796 "compiler/main.gem"
    GemVal _t1190[] = {gem_string(",")};
    GemVal _t1191 = (*gem_v_expect);
                    (void)(_t1191.fn(_t1191.env, _t1190, 1));
#line 797 "compiler/main.gem"
    GemVal _t1192 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1192.fn(_t1192.env, NULL, 0);
#line 798 "compiler/main.gem"
    GemVal _t1193 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1193.fn(_t1193.env, NULL, 0);
#line 799 "compiler/main.gem"
    GemVal _t1194[] = {gem_string("end")};
    GemVal _t1195 = (*gem_v_expect);
                    (void)(_t1195.fn(_t1195.env, _t1194, 1));
#line 800 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 801 "compiler/main.gem"
    GemVal _t1196[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1196, 1));
#line 802 "compiler/main.gem"
    GemVal _t1197[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t1197, 1));
#line 803 "compiler/main.gem"
    GemVal _t1198 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1198;
#line 804 "compiler/main.gem"
    GemVal _t1199[] = {gem_v_idx_var};
    GemVal _t1200[] = {gem_v_var_name, gem_fn_make_var(NULL, _t1199, 1), gem_v_line};
    GemVal _t1201[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1200, 3)};
                    (void)(gem_push_fn(NULL, _t1201, 2));
#line 805 "compiler/main.gem"
    GemVal _t1202[] = {gem_v_idx_var};
    GemVal _t1203[] = {gem_int(1)};
    GemVal _t1204[] = {gem_string("+"), gem_fn_make_var(NULL, _t1202, 1), gem_fn_make_int(NULL, _t1203, 1)};
    GemVal _t1205[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1204, 3), gem_v_line};
    GemVal _t1206[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1205, 3)};
                    (void)(gem_push_fn(NULL, _t1206, 2));
#line 806 "compiler/main.gem"
                    GemVal gem_v__for_i_9 = gem_int(0);
#line 806 "compiler/main.gem"
    GemVal _t1207[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1207, 1);
#line 806 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 806 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_9;
#line 806 "compiler/main.gem"
                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 807 "compiler/main.gem"
    GemVal _t1208[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1208, 2));
                    }

#line 809 "compiler/main.gem"
    GemVal _t1209[] = {gem_v_idx_var};
    GemVal _t1210[] = {gem_v_limit_var};
    GemVal _t1211[] = {gem_string("<"), gem_fn_make_var(NULL, _t1209, 1), gem_fn_make_var(NULL, _t1210, 1)};
    GemVal _t1212[] = {gem_fn_make_binop(NULL, _t1211, 3), gem_v_inner_stmts, gem_v_line};
                    GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1212, 3);
#line 814 "compiler/main.gem"
    GemVal _t1213 = gem_table_new();
    gem_table_set(_t1213, gem_string("tag"), gem_string("block"));
    GemVal _t1214 = gem_table_new();
    GemVal _t1215[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t1214, gem_int(0), gem_fn_make_let(NULL, _t1215, 3));
    GemVal _t1216[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t1214, gem_int(1), gem_fn_make_let(NULL, _t1216, 3));
    gem_table_set(_t1214, gem_int(2), gem_v_while_node);
    gem_table_set(_t1213, gem_string("stmts"), _t1214);
                    GemVal _t1217 = _t1213;
                    gem_pop_frame();
                    return _t1217;
                } else {
#line 820 "compiler/main.gem"
    GemVal _t1218 = (*gem_v_peek);
    GemVal _t1219 = _t1218.fn(_t1218.env, NULL, 0);
    GemVal _t1220[] = {gem_table_get(_t1219, gem_string("line"))};
    GemVal _t1221[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t1220, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 820, _t1221, 1));
                }
            }
        }
    }
#line 825 "compiler/main.gem"
    GemVal _t1222 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1222, gem_string("type")), gem_string("match")))) {
#line 826 "compiler/main.gem"
    GemVal _t1223 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1223, gem_string("line"));
#line 827 "compiler/main.gem"
    GemVal _t1224 = (*gem_v_advance);
        (void)(_t1224.fn(_t1224.env, NULL, 0));
#line 828 "compiler/main.gem"
    GemVal _t1225 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1225.fn(_t1225.env, NULL, 0);
#line 829 "compiler/main.gem"
    GemVal _t1226 = (*gem_v_skip_nl);
        (void)(_t1226.fn(_t1226.env, NULL, 0));
#line 830 "compiler/main.gem"
        *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 831 "compiler/main.gem"
    GemVal _t1227[] = {(*gem_v_gensym_counter)};
        GemVal gem_v_match_var = gem_add(gem_string("_match_"), gem_to_string_fn(NULL, _t1227, 1));
#line 832 "compiler/main.gem"
    GemVal _t1228[] = {gem_v_match_var};
        GemVal gem_v_target_var_expr = gem_fn_make_var(NULL, _t1228, 1);
#line 833 "compiler/main.gem"
    GemVal _t1229 = gem_table_new();
        GemVal gem_v_whens = _t1229;
#line 834 "compiler/main.gem"
        while (1) {
            GemVal _t1230 = (*gem_v_peek);
            GemVal _t1231 = _t1230.fn(_t1230.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1231, gem_string("type")), gem_string("when")))) break;
#line 835 "compiler/main.gem"
    GemVal _t1232 = (*gem_v_advance);
            (void)(_t1232.fn(_t1232.env, NULL, 0));
#line 837 "compiler/main.gem"
    GemVal _t1233 = (*gem_v_peek);
    GemVal _t1234 = _t1233.fn(_t1233.env, NULL, 0);
            GemVal gem_v_pt = gem_table_get(_t1234, gem_string("type"));
#line 838 "compiler/main.gem"
    GemVal _t1235[] = {gem_int(1)};
    GemVal _t1236 = (*gem_v_peek_at);
    GemVal _t1237 = _t1236.fn(_t1236.env, _t1235, 1);
            GemVal gem_v_next = gem_table_get(_t1237, gem_string("type"));
#line 839 "compiler/main.gem"
    GemVal _t1238;
    if (gem_truthy(gem_eq(gem_v_pt, gem_string("{")))) {
        _t1238 = gem_eq(gem_v_pt, gem_string("{"));
    } else {
        _t1238 = gem_eq(gem_v_pt, gem_string("["));
    }
            if (gem_truthy(_t1238)) {
#line 840 "compiler/main.gem"
    GemVal _t1239[] = {gem_v_target_var_expr};
    GemVal _t1240 = (*gem_v_parse_pattern);
                GemVal gem_v_pat = _t1240.fn(_t1240.env, _t1239, 1);
#line 841 "compiler/main.gem"
    GemVal _t1241 = (*gem_v_parse_body);
                GemVal gem_v_wbody = _t1241.fn(_t1241.env, NULL, 0);
#line 842 "compiler/main.gem"
    GemVal _t1242 = gem_v_pat;
    GemVal _t1243 = gem_v_pat;
    GemVal _t1244[] = {gem_table_get(_t1242, gem_string("condition")), gem_v_wbody, gem_table_get(_t1243, gem_string("bindings"))};
    GemVal _t1245[] = {gem_v_whens, gem_fn_make_when(NULL, _t1244, 3)};
                (void)(gem_push_fn(NULL, _t1245, 2));
            } else {
#line 843 "compiler/main.gem"
    GemVal _t1250;
    if (!gem_truthy(gem_eq(gem_v_pt, gem_string("NAME")))) {
        _t1250 = gem_eq(gem_v_pt, gem_string("NAME"));
    } else {
        GemVal _t1246;
        if (gem_truthy(gem_eq(gem_v_next, gem_string("NEWLINE")))) {
                _t1246 = gem_eq(gem_v_next, gem_string("NEWLINE"));
        } else {
                _t1246 = gem_eq(gem_v_next, gem_string("EOF"));
        }
        GemVal _t1247;
        if (gem_truthy(_t1246)) {
                _t1247 = _t1246;
        } else {
                _t1247 = gem_eq(gem_v_next, gem_string("end"));
        }
        GemVal _t1248;
        if (gem_truthy(_t1247)) {
                _t1248 = _t1247;
        } else {
                _t1248 = gem_eq(gem_v_next, gem_string("else"));
        }
        GemVal _t1249;
        if (gem_truthy(_t1248)) {
                _t1249 = _t1248;
        } else {
                _t1249 = gem_eq(gem_v_next, gem_string("when"));
        }
        _t1250 = _t1249;
    }
                if (gem_truthy(_t1250)) {
#line 845 "compiler/main.gem"
    GemVal _t1251[] = {gem_v_target_var_expr};
    GemVal _t1252 = (*gem_v_parse_pattern);
                    GemVal gem_v_pat = _t1252.fn(_t1252.env, _t1251, 1);
#line 846 "compiler/main.gem"
    GemVal _t1253 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1253.fn(_t1253.env, NULL, 0);
#line 847 "compiler/main.gem"
    GemVal _t1254 = gem_v_pat;
    GemVal _t1255 = gem_v_pat;
    GemVal _t1256[] = {gem_table_get(_t1254, gem_string("condition")), gem_v_wbody, gem_table_get(_t1255, gem_string("bindings"))};
    GemVal _t1257[] = {gem_v_whens, gem_fn_make_when(NULL, _t1256, 3)};
                    (void)(gem_push_fn(NULL, _t1257, 2));
                } else {
#line 850 "compiler/main.gem"
    GemVal _t1258 = (*gem_v_parse_expr);
                    GemVal gem_v_wval = _t1258.fn(_t1258.env, NULL, 0);
#line 851 "compiler/main.gem"
    GemVal _t1259 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1259.fn(_t1259.env, NULL, 0);
#line 852 "compiler/main.gem"
    GemVal _t1260[] = {gem_string("=="), gem_v_target_var_expr, gem_v_wval};
    GemVal _t1261 = gem_table_new();
    GemVal _t1262[] = {gem_fn_make_binop(NULL, _t1260, 3), gem_v_wbody, _t1261};
    GemVal _t1263[] = {gem_v_whens, gem_fn_make_when(NULL, _t1262, 3)};
                    (void)(gem_push_fn(NULL, _t1263, 2));
                }
            }
#line 854 "compiler/main.gem"
    GemVal _t1264 = (*gem_v_skip_nl);
            (void)(_t1264.fn(_t1264.env, NULL, 0));
        }
#line 856 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 857 "compiler/main.gem"
    GemVal _t1265 = (*gem_v_peek);
    GemVal _t1266 = _t1265.fn(_t1265.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1266, gem_string("type")), gem_string("else")))) {
#line 858 "compiler/main.gem"
    GemVal _t1267 = (*gem_v_advance);
            (void)(_t1267.fn(_t1267.env, NULL, 0));
#line 859 "compiler/main.gem"
    GemVal _t1268 = (*gem_v_parse_body);
            gem_v_else_body = _t1268.fn(_t1268.env, NULL, 0);
        }
#line 861 "compiler/main.gem"
    GemVal _t1269[] = {gem_string("end")};
    GemVal _t1270 = (*gem_v_expect);
        (void)(_t1270.fn(_t1270.env, _t1269, 1));
#line 862 "compiler/main.gem"
    GemVal _t1271[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line, gem_v_match_var};
        GemVal _t1272 = gem_fn_make_match(NULL, _t1271, 5);
        gem_pop_frame();
        return _t1272;
    }
#line 866 "compiler/main.gem"
    GemVal _t1273 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1273, gem_string("type")), gem_string("return")))) {
#line 867 "compiler/main.gem"
    GemVal _t1274 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1274, gem_string("line"));
#line 868 "compiler/main.gem"
    GemVal _t1275 = (*gem_v_advance);
        (void)(_t1275.fn(_t1275.env, NULL, 0));
#line 869 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 870 "compiler/main.gem"
    GemVal _t1276 = (*gem_v_peek);
    GemVal _t1277 = _t1276.fn(_t1276.env, NULL, 0);
    GemVal _t1280;
    if (!gem_truthy(gem_neq(gem_table_get(_t1277, gem_string("type")), gem_string("NEWLINE")))) {
        _t1280 = gem_neq(gem_table_get(_t1277, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t1278 = (*gem_v_peek);
        GemVal _t1279 = _t1278.fn(_t1278.env, NULL, 0);
        _t1280 = gem_neq(gem_table_get(_t1279, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t1283;
    if (!gem_truthy(_t1280)) {
        _t1283 = _t1280;
    } else {
        GemVal _t1281 = (*gem_v_peek);
        GemVal _t1282 = _t1281.fn(_t1281.env, NULL, 0);
        _t1283 = gem_neq(gem_table_get(_t1282, gem_string("type")), gem_string("end"));
    }
    GemVal _t1286;
    if (!gem_truthy(_t1283)) {
        _t1286 = _t1283;
    } else {
        GemVal _t1284 = (*gem_v_peek);
        GemVal _t1285 = _t1284.fn(_t1284.env, NULL, 0);
        _t1286 = gem_neq(gem_table_get(_t1285, gem_string("type")), gem_string("elif"));
    }
    GemVal _t1289;
    if (!gem_truthy(_t1286)) {
        _t1289 = _t1286;
    } else {
        GemVal _t1287 = (*gem_v_peek);
        GemVal _t1288 = _t1287.fn(_t1287.env, NULL, 0);
        _t1289 = gem_neq(gem_table_get(_t1288, gem_string("type")), gem_string("else"));
    }
    GemVal _t1292;
    if (!gem_truthy(_t1289)) {
        _t1292 = _t1289;
    } else {
        GemVal _t1290 = (*gem_v_peek);
        GemVal _t1291 = _t1290.fn(_t1290.env, NULL, 0);
        _t1292 = gem_neq(gem_table_get(_t1291, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t1292)) {
#line 871 "compiler/main.gem"
    GemVal _t1293 = (*gem_v_parse_expr);
            gem_v_value = _t1293.fn(_t1293.env, NULL, 0);
        }
#line 873 "compiler/main.gem"
    GemVal _t1294[] = {gem_v_value, gem_v_line};
        GemVal _t1295 = gem_fn_make_return(NULL, _t1294, 2);
        gem_pop_frame();
        return _t1295;
    }
#line 877 "compiler/main.gem"
    GemVal _t1296 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1296, gem_string("type")), gem_string("break")))) {
#line 878 "compiler/main.gem"
    GemVal _t1297 = (*gem_v_advance);
        (void)(_t1297.fn(_t1297.env, NULL, 0));
#line 879 "compiler/main.gem"
        GemVal _t1298 = gem_fn_make_break(NULL, NULL, 0);
        gem_pop_frame();
        return _t1298;
    }
#line 883 "compiler/main.gem"
    GemVal _t1299 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1299, gem_string("type")), gem_string("continue")))) {
#line 884 "compiler/main.gem"
    GemVal _t1300 = (*gem_v_advance);
        (void)(_t1300.fn(_t1300.env, NULL, 0));
#line 885 "compiler/main.gem"
        GemVal _t1301 = gem_fn_make_continue(NULL, NULL, 0);
        gem_pop_frame();
        return _t1301;
    }
#line 889 "compiler/main.gem"
    GemVal _t1302 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1302, gem_string("type")), gem_string("load")))) {
#line 890 "compiler/main.gem"
    GemVal _t1303 = (*gem_v_advance);
        (void)(_t1303.fn(_t1303.env, NULL, 0));
#line 891 "compiler/main.gem"
    GemVal _t1304[] = {gem_string("STRING")};
    GemVal _t1305 = (*gem_v_expect);
    GemVal _t1306 = _t1305.fn(_t1305.env, _t1304, 1);
        GemVal gem_v_path = gem_table_get(_t1306, gem_string("value"));
#line 892 "compiler/main.gem"
    GemVal _t1307[] = {gem_v_path};
        GemVal _t1308 = gem_fn_make_load(NULL, _t1307, 1);
        gem_pop_frame();
        return _t1308;
    }
#line 896 "compiler/main.gem"
    GemVal _t1309 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1309, gem_string("type")), gem_string("extern")))) {
#line 897 "compiler/main.gem"
    GemVal _t1310 = (*gem_v_advance);
        (void)(_t1310.fn(_t1310.env, NULL, 0));
#line 898 "compiler/main.gem"
    GemVal _t1311 = (*gem_v_peek);
    GemVal _t1312 = _t1311.fn(_t1311.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1312, gem_string("type")), gem_string("fn")))) {
#line 899 "compiler/main.gem"
    GemVal _t1313 = (*gem_v_advance);
            (void)(_t1313.fn(_t1313.env, NULL, 0));
#line 900 "compiler/main.gem"
    GemVal _t1314[] = {gem_string("NAME")};
    GemVal _t1315 = (*gem_v_expect);
    GemVal _t1316 = _t1315.fn(_t1315.env, _t1314, 1);
            GemVal gem_v_name = gem_table_get(_t1316, gem_string("value"));
#line 901 "compiler/main.gem"
    GemVal _t1317[] = {gem_string("(")};
    GemVal _t1318 = (*gem_v_expect);
            (void)(_t1318.fn(_t1318.env, _t1317, 1));
#line 902 "compiler/main.gem"
    GemVal _t1319 = gem_table_new();
            GemVal gem_v_eparams = _t1319;
#line 903 "compiler/main.gem"
    GemVal _t1320 = (*gem_v_peek);
    GemVal _t1321 = _t1320.fn(_t1320.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t1321, gem_string("type")), gem_string(")")))) {
#line 904 "compiler/main.gem"
    GemVal _t1322[] = {gem_string("NAME")};
    GemVal _t1323 = (*gem_v_expect);
    GemVal _t1324 = _t1323.fn(_t1323.env, _t1322, 1);
                GemVal gem_v_pname = gem_table_get(_t1324, gem_string("value"));
#line 905 "compiler/main.gem"
    GemVal _t1325[] = {gem_string(":")};
    GemVal _t1326 = (*gem_v_expect);
                (void)(_t1326.fn(_t1326.env, _t1325, 1));
#line 906 "compiler/main.gem"
    GemVal _t1327[] = {gem_string("NAME")};
    GemVal _t1328 = (*gem_v_expect);
    GemVal _t1329 = _t1328.fn(_t1328.env, _t1327, 1);
                GemVal gem_v_ptype = gem_table_get(_t1329, gem_string("value"));
#line 907 "compiler/main.gem"
    GemVal _t1330[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1331[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1330, 2)};
                (void)(gem_push_fn(NULL, _t1331, 2));
#line 908 "compiler/main.gem"
                while (1) {
                    GemVal _t1332 = (*gem_v_peek);
                    GemVal _t1333 = _t1332.fn(_t1332.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1333, gem_string("type")), gem_string(",")))) break;
#line 909 "compiler/main.gem"
    GemVal _t1334 = (*gem_v_advance);
                    (void)(_t1334.fn(_t1334.env, NULL, 0));
#line 910 "compiler/main.gem"
    GemVal _t1335[] = {gem_string("NAME")};
    GemVal _t1336 = (*gem_v_expect);
    GemVal _t1337 = _t1336.fn(_t1336.env, _t1335, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t1337, gem_string("value"));
#line 911 "compiler/main.gem"
    GemVal _t1338[] = {gem_string(":")};
    GemVal _t1339 = (*gem_v_expect);
                    (void)(_t1339.fn(_t1339.env, _t1338, 1));
#line 912 "compiler/main.gem"
    GemVal _t1340[] = {gem_string("NAME")};
    GemVal _t1341 = (*gem_v_expect);
    GemVal _t1342 = _t1341.fn(_t1341.env, _t1340, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t1342, gem_string("value"));
#line 913 "compiler/main.gem"
    GemVal _t1343[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1344[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1343, 2)};
                    (void)(gem_push_fn(NULL, _t1344, 2));
                }
            }
#line 916 "compiler/main.gem"
    GemVal _t1345[] = {gem_string(")")};
    GemVal _t1346 = (*gem_v_expect);
            (void)(_t1346.fn(_t1346.env, _t1345, 1));
#line 917 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 918 "compiler/main.gem"
    GemVal _t1347 = (*gem_v_peek);
    GemVal _t1348 = _t1347.fn(_t1347.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1348, gem_string("type")), gem_string("->")))) {
#line 919 "compiler/main.gem"
    GemVal _t1349 = (*gem_v_advance);
                (void)(_t1349.fn(_t1349.env, NULL, 0));
#line 920 "compiler/main.gem"
    GemVal _t1350[] = {gem_string("NAME")};
    GemVal _t1351 = (*gem_v_expect);
    GemVal _t1352 = _t1351.fn(_t1351.env, _t1350, 1);
                gem_v_ret_type = gem_table_get(_t1352, gem_string("value"));
            }
#line 922 "compiler/main.gem"
    GemVal _t1353[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            GemVal _t1354 = gem_fn_make_extern_fn(NULL, _t1353, 3);
            gem_pop_frame();
            return _t1354;
        } else {
#line 923 "compiler/main.gem"
    GemVal _t1355 = (*gem_v_peek);
    GemVal _t1356 = _t1355.fn(_t1355.env, NULL, 0);
    GemVal _t1359;
    if (!gem_truthy(gem_eq(gem_table_get(_t1356, gem_string("type")), gem_string("NAME")))) {
        _t1359 = gem_eq(gem_table_get(_t1356, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1357 = (*gem_v_peek);
        GemVal _t1358 = _t1357.fn(_t1357.env, NULL, 0);
        _t1359 = gem_eq(gem_table_get(_t1358, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t1359)) {
#line 924 "compiler/main.gem"
    GemVal _t1360 = (*gem_v_advance);
                (void)(_t1360.fn(_t1360.env, NULL, 0));
#line 925 "compiler/main.gem"
    GemVal _t1361[] = {gem_string("STRING")};
    GemVal _t1362 = (*gem_v_expect);
    GemVal _t1363 = _t1362.fn(_t1362.env, _t1361, 1);
                GemVal gem_v_path = gem_table_get(_t1363, gem_string("value"));
#line 926 "compiler/main.gem"
    GemVal _t1364[] = {gem_v_path};
                GemVal _t1365 = gem_fn_make_extern_include(NULL, _t1364, 1);
                gem_pop_frame();
                return _t1365;
            } else {
#line 928 "compiler/main.gem"
    GemVal _t1366 = (*gem_v_peek);
    GemVal _t1367 = _t1366.fn(_t1366.env, NULL, 0);
    GemVal _t1368[] = {gem_table_get(_t1367, gem_string("line"))};
    GemVal _t1369[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t1368, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 928, _t1369, 1));
            }
        }
    }
#line 933 "compiler/main.gem"
    GemVal _t1370 = (*gem_v_parse_expr);
    GemVal _t1371 = _t1370.fn(_t1370.env, NULL, 0);
    gem_pop_frame();
    return _t1371;
}

struct _closure__anon_23 {
    GemVal *gem_v_at_end;
    GemVal *gem_v_parse_stmt;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
    GemVal *gem_v_at_end = _cls->gem_v_at_end;
    GemVal *gem_v_parse_stmt = _cls->gem_v_parse_stmt;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    gem_push_frame("_anon_23", "compiler/main.gem", 0);
#line 942 "compiler/main.gem"
    GemVal _t1373 = gem_table_new();
    GemVal gem_v_stmts = _t1373;
#line 943 "compiler/main.gem"
    GemVal _t1374 = (*gem_v_skip_nl);
    (void)(_t1374.fn(_t1374.env, NULL, 0));
#line 944 "compiler/main.gem"
    while (1) {
        GemVal _t1375 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1375.fn(_t1375.env, NULL, 0)))) break;
#line 945 "compiler/main.gem"
    GemVal _t1376 = (*gem_v_parse_stmt);
    GemVal _t1377[] = {gem_v_stmts, _t1376.fn(_t1376.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1377, 2));
#line 946 "compiler/main.gem"
    GemVal _t1378 = (*gem_v_skip_nl);
        (void)(_t1378.fn(_t1378.env, NULL, 0));
    }
#line 948 "compiler/main.gem"
    GemVal _t1379[] = {gem_v_stmts};
    GemVal _t1380 = gem_fn_make_program(NULL, _t1379, 1);
    gem_pop_frame();
    return _t1380;
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
    struct _closure__anon_1 *_t339 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t339->gem_v_pos = gem_v_pos;
    _t339->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t339);
#line 29 "compiler/main.gem"
    struct _closure__anon_2 *_t341 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t341->gem_v_pos = gem_v_pos;
    _t341->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t341);
#line 33 "compiler/main.gem"
    struct _closure__anon_3 *_t343 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t343->gem_v_pos = gem_v_pos;
    _t343->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t343);
#line 39 "compiler/main.gem"
    struct _closure__anon_4 *_t346 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t346->gem_v_pos = gem_v_pos;
    _t346->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t346);
#line 43 "compiler/main.gem"
    struct _closure__anon_5 *_t355 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t355->gem_v_pos = gem_v_pos;
    _t355->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t355);
#line 52 "compiler/main.gem"
    struct _closure__anon_6 *_t357 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t357->gem_v_pos = gem_v_pos;
    _t357->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t357);
#line 64 "compiler/main.gem"
    struct _closure__anon_7 *_t395 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t395->gem_v_advance = gem_v_advance;
    _t395->gem_v_expect = gem_v_expect;
    _t395->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t395);
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
    struct _closure__anon_8 *_t415 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t415->gem_v_at_end = gem_v_at_end;
    _t415->gem_v_parse_stmt = gem_v_parse_stmt;
    _t415->gem_v_peek = gem_v_peek;
    _t415->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t415);
#line 119 "compiler/main.gem"
    struct _closure__anon_9 *_t426 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t426->gem_v_at_end = gem_v_at_end;
    _t426->gem_v_parse_stmt = gem_v_parse_stmt;
    _t426->gem_v_peek = gem_v_peek;
    _t426->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t426);
#line 132 "compiler/main.gem"
    struct _closure__anon_10 *_t558 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t558->gem_v_advance = gem_v_advance;
    _t558->gem_v_expect = gem_v_expect;
    _t558->gem_v_fn_depth = gem_v_fn_depth;
    _t558->gem_v_parse_expr = gem_v_parse_expr;
    _t558->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t558->gem_v_parse_params = gem_v_parse_params;
    _t558->gem_v_peek = gem_v_peek;
    _t558->gem_v_peek_at = gem_v_peek_at;
    _t558->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t558);
#line 263 "compiler/main.gem"
    struct _closure__anon_11 *_t649 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t649->gem_v_advance = gem_v_advance;
    _t649->gem_v_expect = gem_v_expect;
    _t649->gem_v_fn_depth = gem_v_fn_depth;
    _t649->gem_v_parse_atom = gem_v_parse_atom;
    _t649->gem_v_parse_expr = gem_v_parse_expr;
    _t649->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t649->gem_v_parse_params = gem_v_parse_params;
    _t649->gem_v_peek = gem_v_peek;
    _t649->gem_v_peek_at = gem_v_peek_at;
    _t649->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t649);
#line 369 "compiler/main.gem"
    struct _closure__anon_12 *_t658 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t658->gem_v_advance = gem_v_advance;
    _t658->gem_v_parse_call = gem_v_parse_call;
    _t658->gem_v_parse_unary = gem_v_parse_unary;
    _t658->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t658);
#line 378 "compiler/main.gem"
    struct _closure__anon_13 *_t673 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t673->gem_v_advance = gem_v_advance;
    _t673->gem_v_parse_unary = gem_v_parse_unary;
    _t673->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t673);
#line 389 "compiler/main.gem"
    struct _closure__anon_14 *_t685 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t685->gem_v_advance = gem_v_advance;
    _t685->gem_v_parse_mul = gem_v_parse_mul;
    _t685->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t685);
#line 400 "compiler/main.gem"
    struct _closure__anon_15 *_t712 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t712->gem_v_advance = gem_v_advance;
    _t712->gem_v_parse_add = gem_v_parse_add;
    _t712->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t712);
#line 411 "compiler/main.gem"
    struct _closure__anon_16 *_t721 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t721->gem_v_advance = gem_v_advance;
    _t721->gem_v_parse_compare = gem_v_parse_compare;
    _t721->gem_v_parse_not = gem_v_parse_not;
    _t721->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t721);
#line 420 "compiler/main.gem"
    struct _closure__anon_17 *_t729 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t729->gem_v_advance = gem_v_advance;
    _t729->gem_v_parse_not = gem_v_parse_not;
    _t729->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t729);
#line 431 "compiler/main.gem"
    struct _closure__anon_18 *_t737 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t737->gem_v_advance = gem_v_advance;
    _t737->gem_v_parse_and = gem_v_parse_and;
    _t737->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t737);
#line 442 "compiler/main.gem"
    struct _closure__anon_19 *_t786 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t786->gem_v_advance = gem_v_advance;
    _t786->gem_v_parse_expr = gem_v_parse_expr;
    _t786->gem_v_parse_or = gem_v_parse_or;
    _t786->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t786);
#line 477 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 481 "compiler/main.gem"
    GemVal *gem_v_parse_pattern = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_pattern = GEM_NIL;
#line 483 "compiler/main.gem"
    struct _closure__anon_20 *_t954 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t954->gem_v_advance = gem_v_advance;
    _t954->gem_v_expect = gem_v_expect;
    _t954->gem_v_parse_pattern = gem_v_parse_pattern;
    _t954->gem_v_peek = gem_v_peek;
    _t954->gem_v_peek_at = gem_v_peek_at;
    _t954->gem_v_skip_nl = gem_v_skip_nl;
    *gem_v_parse_pattern = gem_make_fn(_anon_20, _t954);
#line 611 "compiler/main.gem"
    struct _closure__anon_22 *_t1372 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1372->gem_v_advance = gem_v_advance;
    _t1372->gem_v_expect = gem_v_expect;
    _t1372->gem_v_fn_depth = gem_v_fn_depth;
    _t1372->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1372->gem_v_parse_body = gem_v_parse_body;
    _t1372->gem_v_parse_expr = gem_v_parse_expr;
    _t1372->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1372->gem_v_parse_params = gem_v_parse_params;
    _t1372->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1372->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1372->gem_v_peek = gem_v_peek;
    _t1372->gem_v_peek_at = gem_v_peek_at;
    _t1372->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_22, _t1372);
#line 937 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 941 "compiler/main.gem"
    struct _closure__anon_23 *_t1381 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1381->gem_v_at_end = gem_v_at_end;
    _t1381->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1381->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_23, _t1381);
    GemVal _t1382 = gem_table_new();
    gem_table_set(_t1382, gem_string("parse"), gem_v_parse);
    GemVal _t1383 = _t1382;
    gem_pop_frame();
    return _t1383;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("escape_c_string", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal _t1384[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1384, 1);
#line 11 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 12 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 12 "compiler/main.gem"
    GemVal _t1385[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1385, 1);
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
    GemVal _t1386[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1386, 1), gem_int(0)))) {
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

    GemVal _t1387 = gem_v_result;
    gem_pop_frame();
    return _t1387;
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

    GemVal _t1388 = gem_v_result;
    gem_pop_frame();
    return _t1388;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 41 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("pad_for", "compiler/main.gem", 41);
#line 42 "compiler/main.gem"
    GemVal _t1389[] = {gem_string("    "), gem_v_indent};
    GemVal _t1390 = gem_fn_repeat_str(NULL, _t1389, 2);
    gem_pop_frame();
    return _t1390;
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 45 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("format_float", "compiler/main.gem", 45);
#line 46 "compiler/main.gem"
    GemVal _t1391[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1391, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 49 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 49 "compiler/main.gem"
    GemVal _t1392[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1392, 1);
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
    GemVal _t1393 = gem_v_s;
    gem_pop_frame();
    return _t1393;
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
    GemVal _t1394 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1394;
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 72 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_union", "compiler/main.gem", 72);
#line 73 "compiler/main.gem"
    GemVal _t1395 = gem_table_new();
    GemVal gem_v_result = _t1395;
#line 74 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 74 "compiler/main.gem"
    GemVal _t1396[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_keys_4 = gem_keys_fn(NULL, _t1396, 1);
#line 74 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 74 "compiler/main.gem"
    while (1) {
        GemVal _t1397[] = {gem_v__for_keys_4};
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_len_fn(NULL, _t1397, 1)))) break;
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
    GemVal _t1398[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_keys_5 = gem_keys_fn(NULL, _t1398, 1);
#line 77 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 77 "compiler/main.gem"
    while (1) {
        GemVal _t1399[] = {gem_v__for_keys_5};
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1399, 1)))) break;
#line 77 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_5, gem_v__for_i_5);
#line 77 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_5, gem_table_get(gem_v__for_keys_5, gem_v__for_i_5));
#line 77 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 78 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1400 = gem_v_result;
    gem_pop_frame();
    return _t1400;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 83 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_intersect", "compiler/main.gem", 83);
#line 84 "compiler/main.gem"
    GemVal _t1401 = gem_table_new();
    GemVal gem_v_result = _t1401;
#line 85 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 85 "compiler/main.gem"
    GemVal _t1402[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_keys_6 = gem_keys_fn(NULL, _t1402, 1);
#line 85 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 85 "compiler/main.gem"
    while (1) {
        GemVal _t1403[] = {gem_v__for_keys_6};
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1403, 1)))) break;
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

    GemVal _t1404 = gem_v_result;
    gem_pop_frame();
    return _t1404;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("set_to_sorted_array", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t1405[] = {gem_v_s};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t1405, 1);
#line 96 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 97 "compiler/main.gem"
    while (1) {
        GemVal _t1406[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t1406, 1)))) break;
#line 98 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
#line 99 "compiler/main.gem"
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
#line 100 "compiler/main.gem"
        while (1) {
            GemVal _t1407;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t1407 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t1407 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t1407)) break;
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
    GemVal _t1408 = gem_v_ks;
    gem_pop_frame();
    return _t1408;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 110 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("sorted_array_to_set", "compiler/main.gem", 110);
#line 111 "compiler/main.gem"
    GemVal _t1409 = gem_table_new();
    GemVal gem_v_s = _t1409;
#line 112 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 112 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 112 "compiler/main.gem"
    while (1) {
        GemVal _t1410[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1410, 1)))) break;
#line 112 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 112 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 113 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1411 = gem_v_s;
    gem_pop_frame();
    return _t1411;
}

static GemVal gem_fn_collect_block_lets(void *_env, GemVal *args, int argc) {
#line 120 "compiler/main.gem"
    GemVal gem_v_stmt = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("collect_block_lets", "compiler/main.gem", 120);
#line 121 "compiler/main.gem"
    GemVal _t1412[] = {gem_v_stmt};
    GemVal _t1414;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1412, 1), gem_string("table")))) {
        _t1414 = gem_eq(gem_type_fn(NULL, _t1412, 1), gem_string("table"));
    } else {
        GemVal _t1413 = gem_v_stmt;
        _t1414 = gem_eq(gem_table_get(_t1413, gem_string("tag")), gem_string("block"));
    }
    if (gem_truthy(_t1414)) {
        {
#line 122 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 122 "compiler/main.gem"
    GemVal _t1415 = gem_v_stmt;
    GemVal _t1416[] = {gem_table_get(_t1415, gem_string("stmts"))};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1416, 1);
#line 122 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 122 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_8;
#line 122 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 123 "compiler/main.gem"
    GemVal _t1417 = gem_v_stmt;
    GemVal _t1418[] = {gem_table_get(gem_table_get(_t1417, gem_string("stmts")), gem_v_j)};
    GemVal _t1421;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1418, 1), gem_string("table")))) {
        _t1421 = gem_eq(gem_type_fn(NULL, _t1418, 1), gem_string("table"));
    } else {
        GemVal _t1419 = gem_v_stmt;
        GemVal _t1420 = gem_table_get(gem_table_get(_t1419, gem_string("stmts")), gem_v_j);
        _t1421 = gem_eq(gem_table_get(_t1420, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1421)) {
#line 124 "compiler/main.gem"
    GemVal _t1422 = gem_v_stmt;
    GemVal _t1423 = gem_table_get(gem_table_get(_t1422, gem_string("stmts")), gem_v_j);
    GemVal _t1424[] = {gem_v_result, gem_table_get(_t1423, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1424, 2));
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
    GemVal _t1425[] = {gem_v_stmts};
        GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1425, 1);
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
    GemVal _t1426[] = {gem_v_s};
    GemVal _t1428;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1426, 1), gem_string("table")))) {
        _t1428 = gem_eq(gem_type_fn(NULL, _t1426, 1), gem_string("table"));
    } else {
        GemVal _t1427 = gem_v_s;
        _t1428 = gem_eq(gem_table_get(_t1427, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1428)) {
#line 134 "compiler/main.gem"
    GemVal _t1429 = gem_v_s;
    GemVal _t1430[] = {gem_v_result, gem_table_get(_t1429, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1430, 2));
            }
#line 136 "compiler/main.gem"
    GemVal _t1431[] = {gem_v_s, gem_v_result};
            (void)(gem_fn_collect_block_lets(NULL, _t1431, 2));
        }
        gem_pop_frame();
        return GEM_NIL;
    }
}

struct _closure__anon_24 {
    GemVal *gem_v_tmp_counter;
};
static GemVal _anon_24(void *_env, GemVal *args, int argc) {
    struct _closure__anon_24 *_cls = (struct _closure__anon_24 *)_env;
    GemVal *gem_v_tmp_counter = _cls->gem_v_tmp_counter;
    gem_push_frame("_anon_24", "compiler/main.gem", 0);
#line 185 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t1443[] = {(*gem_v_tmp_counter)};
    GemVal _t1444 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t1443, 1));
    gem_pop_frame();
    return _t1444;
}

struct _closure__anon_25 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_25(void *_env, GemVal *args, int argc) {
    struct _closure__anon_25 *_cls = (struct _closure__anon_25 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_25", "compiler/main.gem", 0);
#line 190 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t1446[] = {(*gem_v_anon_counter)};
    GemVal _t1447 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t1446, 1));
    gem_pop_frame();
    return _t1447;
}

struct _closure__anon_26 {
};
static GemVal _anon_26(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_26", "compiler/main.gem", 0);
    GemVal _t1449[] = {gem_v_name};
    GemVal _t1450 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t1449, 1));
    gem_pop_frame();
    return _t1450;
}

struct _closure__anon_27 {
    GemVal *gem_v_builtins;
    GemVal *gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_defined_fns;
};
static GemVal _anon_27(void *_env, GemVal *args, int argc) {
    struct _closure__anon_27 *_cls = (struct _closure__anon_27 *)_env;
    GemVal *gem_v_builtins = _cls->gem_v_builtins;
    GemVal *gem_v_collect_free_node = _cls->gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_defined = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_free = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_27", "compiler/main.gem", 0);
#line 204 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 205 "compiler/main.gem"
        GemVal _t1451 = GEM_NIL;
        gem_pop_frame();
        return _t1451;
    }
#line 207 "compiler/main.gem"
    GemVal _t1452[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1452, 1), gem_string("table")))) {
#line 208 "compiler/main.gem"
        GemVal _t1453 = GEM_NIL;
        gem_pop_frame();
        return _t1453;
    }
#line 210 "compiler/main.gem"
    GemVal _t1454 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1454, gem_string("tag")), GEM_NIL))) {
#line 211 "compiler/main.gem"
        GemVal _t1455 = GEM_NIL;
        gem_pop_frame();
        return _t1455;
    }
#line 214 "compiler/main.gem"
    GemVal gem_v__match_11 = gem_v_node;
    GemVal _t1456[] = {gem_v__match_11};
    GemVal _t1458;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1456, 1), gem_string("table")))) {
        _t1458 = gem_eq(gem_type_fn(NULL, _t1456, 1), gem_string("table"));
    } else {
        GemVal _t1457[] = {gem_v__match_11, gem_string("tag")};
        _t1458 = gem_has_key_fn(NULL, _t1457, 2);
    }
    GemVal _t1459;
    if (!gem_truthy(_t1458)) {
        _t1459 = _t1458;
    } else {
        _t1459 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("var"));
    }
    GemVal _t1461;
    if (!gem_truthy(_t1459)) {
        _t1461 = _t1459;
    } else {
        GemVal _t1460[] = {gem_v__match_11, gem_string("name")};
        _t1461 = gem_has_key_fn(NULL, _t1460, 2);
    }
    if (gem_truthy(_t1461)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 216 "compiler/main.gem"
    GemVal _t1462[] = {gem_v_defined, gem_v_name};
    GemVal _t1464;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1462, 2)))) {
        _t1464 = gem_not(gem_fn_set_contains(NULL, _t1462, 2));
    } else {
        GemVal _t1463[] = {(*gem_v_builtins), gem_v_name};
        _t1464 = gem_not(gem_fn_set_contains(NULL, _t1463, 2));
    }
    GemVal _t1466;
    if (!gem_truthy(_t1464)) {
        _t1466 = _t1464;
    } else {
        GemVal _t1465[] = {(*gem_v_defined_fns), gem_v_name};
        _t1466 = gem_not(gem_fn_set_contains(NULL, _t1465, 2));
    }
        if (gem_truthy(_t1466)) {
#line 217 "compiler/main.gem"
    GemVal _t1467[] = {gem_v_free, gem_v_name};
            GemVal _t1468 = gem_fn_set_add(NULL, _t1467, 2);
            gem_pop_frame();
            return _t1468;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1469[] = {gem_v__match_11};
    GemVal _t1471;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1469, 1), gem_string("table")))) {
        _t1471 = gem_eq(gem_type_fn(NULL, _t1469, 1), gem_string("table"));
    } else {
        GemVal _t1470[] = {gem_v__match_11, gem_string("tag")};
        _t1471 = gem_has_key_fn(NULL, _t1470, 2);
    }
    GemVal _t1472;
    if (!gem_truthy(_t1471)) {
        _t1472 = _t1471;
    } else {
        _t1472 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("let"));
    }
    GemVal _t1474;
    if (!gem_truthy(_t1472)) {
        _t1474 = _t1472;
    } else {
        GemVal _t1473[] = {gem_v__match_11, gem_string("value")};
        _t1474 = gem_has_key_fn(NULL, _t1473, 2);
    }
    if (gem_truthy(_t1474)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 220 "compiler/main.gem"
    GemVal _t1475[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1476 = (*gem_v_collect_free_node);
        GemVal _t1477 = _t1476.fn(_t1476.env, _t1475, 3);
        gem_pop_frame();
        return _t1477;
    } else {
    GemVal _t1478[] = {gem_v__match_11};
    GemVal _t1480;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1478, 1), gem_string("table")))) {
        _t1480 = gem_eq(gem_type_fn(NULL, _t1478, 1), gem_string("table"));
    } else {
        GemVal _t1479[] = {gem_v__match_11, gem_string("tag")};
        _t1480 = gem_has_key_fn(NULL, _t1479, 2);
    }
    GemVal _t1481;
    if (!gem_truthy(_t1480)) {
        _t1481 = _t1480;
    } else {
        _t1481 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t1483;
    if (!gem_truthy(_t1481)) {
        _t1483 = _t1481;
    } else {
        GemVal _t1482[] = {gem_v__match_11, gem_string("name")};
        _t1483 = gem_has_key_fn(NULL, _t1482, 2);
    }
    GemVal _t1485;
    if (!gem_truthy(_t1483)) {
        _t1485 = _t1483;
    } else {
        GemVal _t1484[] = {gem_v__match_11, gem_string("value")};
        _t1485 = gem_has_key_fn(NULL, _t1484, 2);
    }
    if (gem_truthy(_t1485)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 222 "compiler/main.gem"
    GemVal _t1486[] = {gem_v_defined, gem_v_name};
    GemVal _t1488;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1486, 2)))) {
        _t1488 = gem_not(gem_fn_set_contains(NULL, _t1486, 2));
    } else {
        GemVal _t1487[] = {(*gem_v_builtins), gem_v_name};
        _t1488 = gem_not(gem_fn_set_contains(NULL, _t1487, 2));
    }
    GemVal _t1490;
    if (!gem_truthy(_t1488)) {
        _t1490 = _t1488;
    } else {
        GemVal _t1489[] = {(*gem_v_defined_fns), gem_v_name};
        _t1490 = gem_not(gem_fn_set_contains(NULL, _t1489, 2));
    }
        if (gem_truthy(_t1490)) {
#line 223 "compiler/main.gem"
    GemVal _t1491[] = {gem_v_free, gem_v_name};
            (void)(gem_fn_set_add(NULL, _t1491, 2));
        }
#line 225 "compiler/main.gem"
    GemVal _t1492[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1493 = (*gem_v_collect_free_node);
        GemVal _t1494 = _t1493.fn(_t1493.env, _t1492, 3);
        gem_pop_frame();
        return _t1494;
    } else {
    GemVal _t1495[] = {gem_v__match_11};
    GemVal _t1497;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1495, 1), gem_string("table")))) {
        _t1497 = gem_eq(gem_type_fn(NULL, _t1495, 1), gem_string("table"));
    } else {
        GemVal _t1496[] = {gem_v__match_11, gem_string("tag")};
        _t1497 = gem_has_key_fn(NULL, _t1496, 2);
    }
    GemVal _t1498;
    if (!gem_truthy(_t1497)) {
        _t1498 = _t1497;
    } else {
        _t1498 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1500;
    if (!gem_truthy(_t1498)) {
        _t1500 = _t1498;
    } else {
        GemVal _t1499[] = {gem_v__match_11, gem_string("params")};
        _t1500 = gem_has_key_fn(NULL, _t1499, 2);
    }
    GemVal _t1502;
    if (!gem_truthy(_t1500)) {
        _t1502 = _t1500;
    } else {
        GemVal _t1501[] = {gem_v__match_11, gem_string("rest_param")};
        _t1502 = gem_has_key_fn(NULL, _t1501, 2);
    }
    GemVal _t1504;
    if (!gem_truthy(_t1502)) {
        _t1504 = _t1502;
    } else {
        GemVal _t1503[] = {gem_v__match_11, gem_string("block_param")};
        _t1504 = gem_has_key_fn(NULL, _t1503, 2);
    }
    GemVal _t1506;
    if (!gem_truthy(_t1504)) {
        _t1506 = _t1504;
    } else {
        GemVal _t1505[] = {gem_v__match_11, gem_string("body")};
        _t1506 = gem_has_key_fn(NULL, _t1505, 2);
    }
    if (gem_truthy(_t1506)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_11, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_11, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_11, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 227 "compiler/main.gem"
    GemVal _t1507[] = {gem_v_params};
    GemVal _t1508[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t1507, 1)};
        GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t1508, 2);
#line 228 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 229 "compiler/main.gem"
    GemVal _t1509[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn_set_add(NULL, _t1509, 2));
        }
#line 231 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 232 "compiler/main.gem"
    GemVal _t1510[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn_set_add(NULL, _t1510, 2));
        }
#line 234 "compiler/main.gem"
    GemVal _t1511[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1512 = (*gem_v_collect_free_vars);
        GemVal gem_v_inner_free = _t1512.fn(_t1512.env, _t1511, 2);
        {
#line 235 "compiler/main.gem"
            GemVal gem_v__for_tbl_12 = gem_v_inner_free;
#line 235 "compiler/main.gem"
    GemVal _t1513[] = {gem_v__for_tbl_12};
            GemVal gem_v__for_keys_12 = gem_keys_fn(NULL, _t1513, 1);
#line 235 "compiler/main.gem"
            GemVal gem_v__for_i_12 = gem_int(0);
#line 235 "compiler/main.gem"
            while (1) {
                GemVal _t1514[] = {gem_v__for_keys_12};
                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t1514, 1)))) break;
#line 235 "compiler/main.gem"
                GemVal gem_v_ifk = gem_table_get(gem_v__for_keys_12, gem_v__for_i_12);
#line 235 "compiler/main.gem"
                GemVal gem_v__ = gem_table_get(gem_v__for_tbl_12, gem_table_get(gem_v__for_keys_12, gem_v__for_i_12));
#line 235 "compiler/main.gem"
                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 236 "compiler/main.gem"
    GemVal _t1515[] = {gem_v_free, gem_v_ifk};
                (void)(gem_fn_set_add(NULL, _t1515, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1516[] = {gem_v__match_11};
    GemVal _t1518;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1516, 1), gem_string("table")))) {
        _t1518 = gem_eq(gem_type_fn(NULL, _t1516, 1), gem_string("table"));
    } else {
        GemVal _t1517[] = {gem_v__match_11, gem_string("tag")};
        _t1518 = gem_has_key_fn(NULL, _t1517, 2);
    }
    GemVal _t1519;
    if (!gem_truthy(_t1518)) {
        _t1519 = _t1518;
    } else {
        _t1519 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1519)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1520[] = {gem_v__match_11};
    GemVal _t1522;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1520, 1), gem_string("table")))) {
        _t1522 = gem_eq(gem_type_fn(NULL, _t1520, 1), gem_string("table"));
    } else {
        GemVal _t1521[] = {gem_v__match_11, gem_string("tag")};
        _t1522 = gem_has_key_fn(NULL, _t1521, 2);
    }
    GemVal _t1523;
    if (!gem_truthy(_t1522)) {
        _t1523 = _t1522;
    } else {
        _t1523 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1525;
    if (!gem_truthy(_t1523)) {
        _t1525 = _t1523;
    } else {
        GemVal _t1524[] = {gem_v__match_11, gem_string("cond")};
        _t1525 = gem_has_key_fn(NULL, _t1524, 2);
    }
    GemVal _t1527;
    if (!gem_truthy(_t1525)) {
        _t1527 = _t1525;
    } else {
        GemVal _t1526[] = {gem_v__match_11, gem_string("then")};
        _t1527 = gem_has_key_fn(NULL, _t1526, 2);
    }
    GemVal _t1529;
    if (!gem_truthy(_t1527)) {
        _t1529 = _t1527;
    } else {
        GemVal _t1528[] = {gem_v__match_11, gem_string("else")};
        _t1529 = gem_has_key_fn(NULL, _t1528, 2);
    }
    if (gem_truthy(_t1529)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_11, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 241 "compiler/main.gem"
    GemVal _t1530[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1531 = (*gem_v_collect_free_node);
        (void)(_t1531.fn(_t1531.env, _t1530, 3));
#line 242 "compiler/main.gem"
    GemVal _t1532 = gem_table_new();
    GemVal _t1533[] = {gem_v_defined, _t1532};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1533, 2);
#line 243 "compiler/main.gem"
        GemVal gem_v__for_i_13 = gem_int(0);
#line 243 "compiler/main.gem"
    GemVal _t1534[] = {gem_v_thens};
        GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1534, 1);
#line 243 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 243 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_13;
#line 243 "compiler/main.gem"
            gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 244 "compiler/main.gem"
    GemVal _t1535[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1536 = (*gem_v_collect_free_node);
            (void)(_t1536.fn(_t1536.env, _t1535, 3));
#line 245 "compiler/main.gem"
    GemVal _t1537[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t1539;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1537, 1), gem_string("table")))) {
        _t1539 = gem_eq(gem_type_fn(NULL, _t1537, 1), gem_string("table"));
    } else {
        GemVal _t1538 = gem_table_get(gem_v_thens, gem_v_i);
        _t1539 = gem_eq(gem_table_get(_t1538, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1539)) {
#line 246 "compiler/main.gem"
    GemVal _t1540 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1541[] = {gem_v_d, gem_table_get(_t1540, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1541, 2));
            }
        }

#line 249 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 250 "compiler/main.gem"
    GemVal _t1542 = gem_table_new();
    GemVal _t1543[] = {gem_v_defined, _t1542};
            gem_v_d = gem_fn_set_union(NULL, _t1543, 2);
            {
#line 251 "compiler/main.gem"
                GemVal gem_v__for_i_14 = gem_int(0);
#line 251 "compiler/main.gem"
    GemVal _t1544[] = {gem_v_el};
                GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1544, 1);
#line 251 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 251 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_14;
#line 251 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 252 "compiler/main.gem"
    GemVal _t1545[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1546 = (*gem_v_collect_free_node);
                    (void)(_t1546.fn(_t1546.env, _t1545, 3));
#line 253 "compiler/main.gem"
    GemVal _t1547[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t1549;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1547, 1), gem_string("table")))) {
        _t1549 = gem_eq(gem_type_fn(NULL, _t1547, 1), gem_string("table"));
    } else {
        GemVal _t1548 = gem_table_get(gem_v_el, gem_v_i);
        _t1549 = gem_eq(gem_table_get(_t1548, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1549)) {
#line 254 "compiler/main.gem"
    GemVal _t1550 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1551[] = {gem_v_d, gem_table_get(_t1550, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1551, 2));
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
    GemVal _t1552[] = {gem_v__match_11};
    GemVal _t1554;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1552, 1), gem_string("table")))) {
        _t1554 = gem_eq(gem_type_fn(NULL, _t1552, 1), gem_string("table"));
    } else {
        GemVal _t1553[] = {gem_v__match_11, gem_string("tag")};
        _t1554 = gem_has_key_fn(NULL, _t1553, 2);
    }
    GemVal _t1555;
    if (!gem_truthy(_t1554)) {
        _t1555 = _t1554;
    } else {
        _t1555 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1557;
    if (!gem_truthy(_t1555)) {
        _t1557 = _t1555;
    } else {
        GemVal _t1556[] = {gem_v__match_11, gem_string("cond")};
        _t1557 = gem_has_key_fn(NULL, _t1556, 2);
    }
    GemVal _t1559;
    if (!gem_truthy(_t1557)) {
        _t1559 = _t1557;
    } else {
        GemVal _t1558[] = {gem_v__match_11, gem_string("body")};
        _t1559 = gem_has_key_fn(NULL, _t1558, 2);
    }
    if (gem_truthy(_t1559)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 259 "compiler/main.gem"
    GemVal _t1560[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1561 = (*gem_v_collect_free_node);
        (void)(_t1561.fn(_t1561.env, _t1560, 3));
#line 260 "compiler/main.gem"
    GemVal _t1562 = gem_table_new();
    GemVal _t1563[] = {gem_v_defined, _t1562};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1563, 2);
        {
#line 261 "compiler/main.gem"
            GemVal gem_v__for_i_15 = gem_int(0);
#line 261 "compiler/main.gem"
    GemVal _t1564[] = {gem_v_body};
            GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1564, 1);
#line 261 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 261 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_15;
#line 261 "compiler/main.gem"
                gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 262 "compiler/main.gem"
    GemVal _t1565[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1566 = (*gem_v_collect_free_node);
                (void)(_t1566.fn(_t1566.env, _t1565, 3));
#line 263 "compiler/main.gem"
    GemVal _t1567[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1569;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1567, 1), gem_string("table")))) {
        _t1569 = gem_eq(gem_type_fn(NULL, _t1567, 1), gem_string("table"));
    } else {
        GemVal _t1568 = gem_table_get(gem_v_body, gem_v_i);
        _t1569 = gem_eq(gem_table_get(_t1568, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1569)) {
#line 264 "compiler/main.gem"
    GemVal _t1570 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1571[] = {gem_v_d, gem_table_get(_t1570, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1571, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1572[] = {gem_v__match_11};
    GemVal _t1574;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1572, 1), gem_string("table")))) {
        _t1574 = gem_eq(gem_type_fn(NULL, _t1572, 1), gem_string("table"));
    } else {
        GemVal _t1573[] = {gem_v__match_11, gem_string("tag")};
        _t1574 = gem_has_key_fn(NULL, _t1573, 2);
    }
    GemVal _t1575;
    if (!gem_truthy(_t1574)) {
        _t1575 = _t1574;
    } else {
        _t1575 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("match"));
    }
    GemVal _t1577;
    if (!gem_truthy(_t1575)) {
        _t1577 = _t1575;
    } else {
        GemVal _t1576[] = {gem_v__match_11, gem_string("target")};
        _t1577 = gem_has_key_fn(NULL, _t1576, 2);
    }
    GemVal _t1579;
    if (!gem_truthy(_t1577)) {
        _t1579 = _t1577;
    } else {
        GemVal _t1578[] = {gem_v__match_11, gem_string("target_var")};
        _t1579 = gem_has_key_fn(NULL, _t1578, 2);
    }
    GemVal _t1581;
    if (!gem_truthy(_t1579)) {
        _t1581 = _t1579;
    } else {
        GemVal _t1580[] = {gem_v__match_11, gem_string("whens")};
        _t1581 = gem_has_key_fn(NULL, _t1580, 2);
    }
    GemVal _t1583;
    if (!gem_truthy(_t1581)) {
        _t1583 = _t1581;
    } else {
        GemVal _t1582[] = {gem_v__match_11, gem_string("else")};
        _t1583 = gem_has_key_fn(NULL, _t1582, 2);
    }
    if (gem_truthy(_t1583)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_11, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_target_var = gem_table_get(gem_v__match_11, gem_string("target_var"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_11, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 268 "compiler/main.gem"
    GemVal _t1584[] = {gem_v_target, gem_v_defined, gem_v_free};
    GemVal _t1585 = (*gem_v_collect_free_node);
        (void)(_t1585.fn(_t1585.env, _t1584, 3));
#line 269 "compiler/main.gem"
    GemVal _t1586 = gem_table_new();
    GemVal _t1587[] = {gem_v_defined, _t1586};
        GemVal gem_v_match_def = gem_fn_set_union(NULL, _t1587, 2);
#line 270 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_target_var, GEM_NIL))) {
#line 271 "compiler/main.gem"
    GemVal _t1588[] = {gem_v_match_def, gem_v_target_var};
            (void)(gem_fn_set_add(NULL, _t1588, 2));
        }
#line 273 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 273 "compiler/main.gem"
    GemVal _t1589[] = {gem_v_whens};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1589, 1);
#line 273 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 273 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_18;
#line 273 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 274 "compiler/main.gem"
            GemVal gem_v_w = gem_table_get(gem_v_whens, gem_v_i);
#line 275 "compiler/main.gem"
    GemVal _t1590 = gem_v_w;
    GemVal _t1591[] = {gem_table_get(_t1590, gem_string("value")), gem_v_match_def, gem_v_free};
    GemVal _t1592 = (*gem_v_collect_free_node);
            (void)(_t1592.fn(_t1592.env, _t1591, 3));
#line 276 "compiler/main.gem"
    GemVal _t1593 = gem_table_new();
    GemVal _t1594[] = {gem_v_match_def, _t1593};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1594, 2);
#line 277 "compiler/main.gem"
    GemVal _t1595 = gem_v_w;
            if (gem_truthy(gem_neq(gem_table_get(_t1595, gem_string("bindings")), GEM_NIL))) {
#line 278 "compiler/main.gem"
                GemVal gem_v__for_i_16 = gem_int(0);
#line 278 "compiler/main.gem"
    GemVal _t1596 = gem_v_w;
    GemVal _t1597[] = {gem_table_get(_t1596, gem_string("bindings"))};
                GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1597, 1);
#line 278 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 278 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_16;
#line 278 "compiler/main.gem"
                    gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 279 "compiler/main.gem"
    GemVal _t1598 = gem_v_w;
    GemVal _t1599[] = {gem_table_get(gem_table_get(_t1598, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1600 = (*gem_v_collect_free_node);
                    (void)(_t1600.fn(_t1600.env, _t1599, 3));
#line 280 "compiler/main.gem"
    GemVal _t1601 = gem_v_w;
    GemVal _t1602[] = {gem_table_get(gem_table_get(_t1601, gem_string("bindings")), gem_v_bi)};
    GemVal _t1605;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1602, 1), gem_string("table")))) {
        _t1605 = gem_eq(gem_type_fn(NULL, _t1602, 1), gem_string("table"));
    } else {
        GemVal _t1603 = gem_v_w;
        GemVal _t1604 = gem_table_get(gem_table_get(_t1603, gem_string("bindings")), gem_v_bi);
        _t1605 = gem_eq(gem_table_get(_t1604, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1605)) {
#line 281 "compiler/main.gem"
    GemVal _t1606 = gem_v_w;
    GemVal _t1607 = gem_table_get(gem_table_get(_t1606, gem_string("bindings")), gem_v_bi);
    GemVal _t1608[] = {gem_v_d, gem_table_get(_t1607, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1608, 2));
                    }
                }

            }
#line 285 "compiler/main.gem"
            GemVal gem_v__for_i_17 = gem_int(0);
#line 285 "compiler/main.gem"
    GemVal _t1609 = gem_v_w;
    GemVal _t1610[] = {gem_table_get(_t1609, gem_string("body"))};
            GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1610, 1);
#line 285 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 285 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_17;
#line 285 "compiler/main.gem"
                gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 286 "compiler/main.gem"
    GemVal _t1611 = gem_v_w;
    GemVal _t1612[] = {gem_table_get(gem_table_get(_t1611, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1613 = (*gem_v_collect_free_node);
                (void)(_t1613.fn(_t1613.env, _t1612, 3));
#line 287 "compiler/main.gem"
    GemVal _t1614 = gem_v_w;
    GemVal _t1615[] = {gem_table_get(gem_table_get(_t1614, gem_string("body")), gem_v_j)};
    GemVal _t1618;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1615, 1), gem_string("table")))) {
        _t1618 = gem_eq(gem_type_fn(NULL, _t1615, 1), gem_string("table"));
    } else {
        GemVal _t1616 = gem_v_w;
        GemVal _t1617 = gem_table_get(gem_table_get(_t1616, gem_string("body")), gem_v_j);
        _t1618 = gem_eq(gem_table_get(_t1617, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1618)) {
#line 288 "compiler/main.gem"
    GemVal _t1619 = gem_v_w;
    GemVal _t1620 = gem_table_get(gem_table_get(_t1619, gem_string("body")), gem_v_j);
    GemVal _t1621[] = {gem_v_d, gem_table_get(_t1620, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1621, 2));
                }
            }

        }

#line 292 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 293 "compiler/main.gem"
    GemVal _t1622 = gem_table_new();
    GemVal _t1623[] = {gem_v_defined, _t1622};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1623, 2);
            {
#line 294 "compiler/main.gem"
                GemVal gem_v__for_i_19 = gem_int(0);
#line 294 "compiler/main.gem"
    GemVal _t1624[] = {gem_v_el};
                GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1624, 1);
#line 294 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 294 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_19;
#line 294 "compiler/main.gem"
                    gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 295 "compiler/main.gem"
    GemVal _t1625[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1626 = (*gem_v_collect_free_node);
                    (void)(_t1626.fn(_t1626.env, _t1625, 3));
#line 296 "compiler/main.gem"
    GemVal _t1627[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t1629;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1627, 1), gem_string("table")))) {
        _t1629 = gem_eq(gem_type_fn(NULL, _t1627, 1), gem_string("table"));
    } else {
        GemVal _t1628 = gem_table_get(gem_v_el, gem_v_j);
        _t1629 = gem_eq(gem_table_get(_t1628, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1629)) {
#line 297 "compiler/main.gem"
    GemVal _t1630 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t1631[] = {gem_v_d, gem_table_get(_t1630, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1631, 2));
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
    GemVal _t1632[] = {gem_v__match_11};
    GemVal _t1634;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1632, 1), gem_string("table")))) {
        _t1634 = gem_eq(gem_type_fn(NULL, _t1632, 1), gem_string("table"));
    } else {
        GemVal _t1633[] = {gem_v__match_11, gem_string("tag")};
        _t1634 = gem_has_key_fn(NULL, _t1633, 2);
    }
    GemVal _t1635;
    if (!gem_truthy(_t1634)) {
        _t1635 = _t1634;
    } else {
        _t1635 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("when_clause"));
    }
    GemVal _t1637;
    if (!gem_truthy(_t1635)) {
        _t1637 = _t1635;
    } else {
        GemVal _t1636[] = {gem_v__match_11, gem_string("value")};
        _t1637 = gem_has_key_fn(NULL, _t1636, 2);
    }
    GemVal _t1639;
    if (!gem_truthy(_t1637)) {
        _t1639 = _t1637;
    } else {
        GemVal _t1638[] = {gem_v__match_11, gem_string("bindings")};
        _t1639 = gem_has_key_fn(NULL, _t1638, 2);
    }
    GemVal _t1641;
    if (!gem_truthy(_t1639)) {
        _t1641 = _t1639;
    } else {
        GemVal _t1640[] = {gem_v__match_11, gem_string("body")};
        _t1641 = gem_has_key_fn(NULL, _t1640, 2);
    }
    if (gem_truthy(_t1641)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_bindings = gem_table_get(gem_v__match_11, gem_string("bindings"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 302 "compiler/main.gem"
    GemVal _t1642[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1643 = (*gem_v_collect_free_node);
        (void)(_t1643.fn(_t1643.env, _t1642, 3));
#line 303 "compiler/main.gem"
    GemVal _t1644 = gem_table_new();
    GemVal _t1645[] = {gem_v_defined, _t1644};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1645, 2);
#line 304 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_bindings, GEM_NIL))) {
#line 305 "compiler/main.gem"
            GemVal gem_v__for_i_20 = gem_int(0);
#line 305 "compiler/main.gem"
    GemVal _t1646[] = {gem_v_bindings};
            GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1646, 1);
#line 305 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 305 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_20;
#line 305 "compiler/main.gem"
                gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 306 "compiler/main.gem"
    GemVal _t1647[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1648 = (*gem_v_collect_free_node);
                (void)(_t1648.fn(_t1648.env, _t1647, 3));
#line 307 "compiler/main.gem"
    GemVal _t1649[] = {gem_table_get(gem_v_bindings, gem_v_bi)};
    GemVal _t1651;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1649, 1), gem_string("table")))) {
        _t1651 = gem_eq(gem_type_fn(NULL, _t1649, 1), gem_string("table"));
    } else {
        GemVal _t1650 = gem_table_get(gem_v_bindings, gem_v_bi);
        _t1651 = gem_eq(gem_table_get(_t1650, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1651)) {
#line 308 "compiler/main.gem"
    GemVal _t1652 = gem_table_get(gem_v_bindings, gem_v_bi);
    GemVal _t1653[] = {gem_v_d, gem_table_get(_t1652, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1653, 2));
                }
            }

        }
        {
#line 312 "compiler/main.gem"
            GemVal gem_v__for_i_21 = gem_int(0);
#line 312 "compiler/main.gem"
    GemVal _t1654[] = {gem_v_body};
            GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t1654, 1);
#line 312 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 312 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_21;
#line 312 "compiler/main.gem"
                gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 313 "compiler/main.gem"
    GemVal _t1655[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1656 = (*gem_v_collect_free_node);
                (void)(_t1656.fn(_t1656.env, _t1655, 3));
#line 314 "compiler/main.gem"
    GemVal _t1657[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1659;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1657, 1), gem_string("table")))) {
        _t1659 = gem_eq(gem_type_fn(NULL, _t1657, 1), gem_string("table"));
    } else {
        GemVal _t1658 = gem_table_get(gem_v_body, gem_v_i);
        _t1659 = gem_eq(gem_table_get(_t1658, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1659)) {
#line 315 "compiler/main.gem"
    GemVal _t1660 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1661[] = {gem_v_d, gem_table_get(_t1660, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1661, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1662[] = {gem_v__match_11};
    GemVal _t1664;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1662, 1), gem_string("table")))) {
        _t1664 = gem_eq(gem_type_fn(NULL, _t1662, 1), gem_string("table"));
    } else {
        GemVal _t1663[] = {gem_v__match_11, gem_string("tag")};
        _t1664 = gem_has_key_fn(NULL, _t1663, 2);
    }
    GemVal _t1665;
    if (!gem_truthy(_t1664)) {
        _t1665 = _t1664;
    } else {
        _t1665 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("block"));
    }
    GemVal _t1667;
    if (!gem_truthy(_t1665)) {
        _t1667 = _t1665;
    } else {
        GemVal _t1666[] = {gem_v__match_11, gem_string("stmts")};
        _t1667 = gem_has_key_fn(NULL, _t1666, 2);
    }
    if (gem_truthy(_t1667)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_11, gem_string("stmts"));
#line 319 "compiler/main.gem"
    GemVal _t1668 = gem_table_new();
    GemVal _t1669[] = {gem_v_defined, _t1668};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1669, 2);
        {
#line 320 "compiler/main.gem"
            GemVal gem_v__for_i_22 = gem_int(0);
#line 320 "compiler/main.gem"
    GemVal _t1670[] = {gem_v_stmts};
            GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1670, 1);
#line 320 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 320 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_22;
#line 320 "compiler/main.gem"
                gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 321 "compiler/main.gem"
    GemVal _t1671[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1672 = (*gem_v_collect_free_node);
                (void)(_t1672.fn(_t1672.env, _t1671, 3));
#line 322 "compiler/main.gem"
    GemVal _t1673[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1675;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1673, 1), gem_string("table")))) {
        _t1675 = gem_eq(gem_type_fn(NULL, _t1673, 1), gem_string("table"));
    } else {
        GemVal _t1674 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1675 = gem_eq(gem_table_get(_t1674, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1675)) {
#line 323 "compiler/main.gem"
    GemVal _t1676 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1677[] = {gem_v_d, gem_table_get(_t1676, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1677, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1678[] = {gem_v__match_11};
    GemVal _t1680;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1678, 1), gem_string("table")))) {
        _t1680 = gem_eq(gem_type_fn(NULL, _t1678, 1), gem_string("table"));
    } else {
        GemVal _t1679[] = {gem_v__match_11, gem_string("tag")};
        _t1680 = gem_has_key_fn(NULL, _t1679, 2);
    }
    GemVal _t1681;
    if (!gem_truthy(_t1680)) {
        _t1681 = _t1680;
    } else {
        _t1681 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("return"));
    }
    GemVal _t1683;
    if (!gem_truthy(_t1681)) {
        _t1683 = _t1681;
    } else {
        GemVal _t1682[] = {gem_v__match_11, gem_string("value")};
        _t1683 = gem_has_key_fn(NULL, _t1682, 2);
    }
    if (gem_truthy(_t1683)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 327 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 328 "compiler/main.gem"
    GemVal _t1684[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1685 = (*gem_v_collect_free_node);
            GemVal _t1686 = _t1685.fn(_t1685.env, _t1684, 3);
            gem_pop_frame();
            return _t1686;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
        {
#line 332 "compiler/main.gem"
            GemVal gem_v__for_tbl_23 = gem_v_node;
#line 332 "compiler/main.gem"
    GemVal _t1687[] = {gem_v__for_tbl_23};
            GemVal gem_v__for_keys_23 = gem_keys_fn(NULL, _t1687, 1);
#line 332 "compiler/main.gem"
            GemVal gem_v__for_i_23 = gem_int(0);
#line 332 "compiler/main.gem"
            while (1) {
                GemVal _t1688[] = {gem_v__for_keys_23};
                if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_len_fn(NULL, _t1688, 1)))) break;
#line 332 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_23, gem_v__for_i_23);
#line 332 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_23, gem_table_get(gem_v__for_keys_23, gem_v__for_i_23));
#line 332 "compiler/main.gem"
                gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 333 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 334 "compiler/main.gem"
    GemVal _t1689[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1689, 1), gem_string("table")))) {
#line 335 "compiler/main.gem"
    GemVal _t1690[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t1690, 1);
#line 336 "compiler/main.gem"
    GemVal _t1691[] = {gem_v_vks};
    GemVal _t1693;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1691, 1), gem_int(0)))) {
        _t1693 = gem_gt(gem_len_fn(NULL, _t1691, 1), gem_int(0));
    } else {
        GemVal _t1692[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1693 = gem_eq(gem_type_fn(NULL, _t1692, 1), gem_string("int"));
    }
                        if (gem_truthy(_t1693)) {
#line 337 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 338 "compiler/main.gem"
                            while (1) {
                                GemVal _t1694[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1694, 1)))) break;
#line 339 "compiler/main.gem"
    GemVal _t1695[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1695, 1), gem_string("table")))) {
#line 340 "compiler/main.gem"
    GemVal _t1696[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t1697 = (*gem_v_collect_free_node);
                                    (void)(_t1697.fn(_t1697.env, _t1696, 3));
                                }
#line 342 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 345 "compiler/main.gem"
    GemVal _t1698[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t1699 = (*gem_v_collect_free_node);
                            (void)(_t1699.fn(_t1699.env, _t1698, 3));
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

struct _closure__anon_28 {
    GemVal *gem_v_collect_free_node;
};
static GemVal _anon_28(void *_env, GemVal *args, int argc) {
    struct _closure__anon_28 *_cls = (struct _closure__anon_28 *)_env;
    GemVal *gem_v_collect_free_node = _cls->gem_v_collect_free_node;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_defined = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_28", "compiler/main.gem", 0);
#line 354 "compiler/main.gem"
    GemVal _t1701 = gem_table_new();
    GemVal gem_v_free = _t1701;
#line 355 "compiler/main.gem"
    GemVal _t1702 = gem_table_new();
    GemVal _t1703[] = {gem_v_defined, _t1702};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t1703, 2);
#line 356 "compiler/main.gem"
    GemVal gem_v__for_i_24 = gem_int(0);
#line 356 "compiler/main.gem"
    GemVal _t1704[] = {gem_v_stmts};
    GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1704, 1);
#line 356 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 356 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_24;
#line 356 "compiler/main.gem"
        gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 357 "compiler/main.gem"
    GemVal _t1705[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1706 = (*gem_v_collect_free_node);
        (void)(_t1706.fn(_t1706.env, _t1705, 3));
#line 358 "compiler/main.gem"
    GemVal _t1707[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1709;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1707, 1), gem_string("table")))) {
        _t1709 = gem_eq(gem_type_fn(NULL, _t1707, 1), gem_string("table"));
    } else {
        GemVal _t1708 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1709 = gem_eq(gem_table_get(_t1708, gem_string("tag")), gem_string("let"));
    }
        if (gem_truthy(_t1709)) {
#line 359 "compiler/main.gem"
    GemVal _t1710 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1711[] = {gem_v_d, gem_table_get(_t1710, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1711, 2));
        }
#line 361 "compiler/main.gem"
    GemVal _t1712[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d};
        (void)(gem_fn_collect_block_lets(NULL, _t1712, 2));
    }

    GemVal _t1713 = gem_v_free;
    gem_pop_frame();
    return _t1713;
}

struct _closure__anon_29 {
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_walk_captures;
    GemVal *gem_v_walk_captures_node;
};
static GemVal _anon_29(void *_env, GemVal *args, int argc) {
    struct _closure__anon_29 *_cls = (struct _closure__anon_29 *)_env;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_walk_captures = _cls->gem_v_walk_captures;
    GemVal *gem_v_walk_captures_node = _cls->gem_v_walk_captures_node;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_scope_vars = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_captured = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_29", "compiler/main.gem", 0);
#line 372 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 373 "compiler/main.gem"
        GemVal _t1715 = GEM_NIL;
        gem_pop_frame();
        return _t1715;
    }
#line 375 "compiler/main.gem"
    GemVal _t1716[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1716, 1), gem_string("table")))) {
#line 376 "compiler/main.gem"
        GemVal _t1717 = GEM_NIL;
        gem_pop_frame();
        return _t1717;
    }
#line 378 "compiler/main.gem"
    GemVal _t1718 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1718, gem_string("tag")), GEM_NIL))) {
#line 379 "compiler/main.gem"
        GemVal _t1719 = GEM_NIL;
        gem_pop_frame();
        return _t1719;
    }
#line 382 "compiler/main.gem"
    GemVal gem_v__match_25 = gem_v_node;
    GemVal _t1720[] = {gem_v__match_25};
    GemVal _t1722;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1720, 1), gem_string("table")))) {
        _t1722 = gem_eq(gem_type_fn(NULL, _t1720, 1), gem_string("table"));
    } else {
        GemVal _t1721[] = {gem_v__match_25, gem_string("tag")};
        _t1722 = gem_has_key_fn(NULL, _t1721, 2);
    }
    GemVal _t1723;
    if (!gem_truthy(_t1722)) {
        _t1723 = _t1722;
    } else {
        _t1723 = gem_eq(gem_table_get(gem_v__match_25, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1725;
    if (!gem_truthy(_t1723)) {
        _t1725 = _t1723;
    } else {
        GemVal _t1724[] = {gem_v__match_25, gem_string("params")};
        _t1725 = gem_has_key_fn(NULL, _t1724, 2);
    }
    GemVal _t1727;
    if (!gem_truthy(_t1725)) {
        _t1727 = _t1725;
    } else {
        GemVal _t1726[] = {gem_v__match_25, gem_string("rest_param")};
        _t1727 = gem_has_key_fn(NULL, _t1726, 2);
    }
    GemVal _t1729;
    if (!gem_truthy(_t1727)) {
        _t1729 = _t1727;
    } else {
        GemVal _t1728[] = {gem_v__match_25, gem_string("block_param")};
        _t1729 = gem_has_key_fn(NULL, _t1728, 2);
    }
    GemVal _t1731;
    if (!gem_truthy(_t1729)) {
        _t1731 = _t1729;
    } else {
        GemVal _t1730[] = {gem_v__match_25, gem_string("body")};
        _t1731 = gem_has_key_fn(NULL, _t1730, 2);
    }
    if (gem_truthy(_t1731)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_25, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_25, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_25, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_25, gem_string("body"));
#line 384 "compiler/main.gem"
    GemVal _t1732[] = {gem_v_params};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1732, 1);
#line 385 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 386 "compiler/main.gem"
    GemVal _t1733[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn_set_add(NULL, _t1733, 2));
        }
#line 388 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 389 "compiler/main.gem"
    GemVal _t1734[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn_set_add(NULL, _t1734, 2));
        }
#line 391 "compiler/main.gem"
    GemVal _t1735[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1736 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t1736.fn(_t1736.env, _t1735, 2);
#line 392 "compiler/main.gem"
    GemVal _t1737[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t1737, 2);
#line 393 "compiler/main.gem"
        GemVal gem_v__for_tbl_26 = gem_v_inter;
#line 393 "compiler/main.gem"
    GemVal _t1738[] = {gem_v__for_tbl_26};
        GemVal gem_v__for_keys_26 = gem_keys_fn(NULL, _t1738, 1);
#line 393 "compiler/main.gem"
        GemVal gem_v__for_i_26 = gem_int(0);
#line 393 "compiler/main.gem"
        while (1) {
            GemVal _t1739[] = {gem_v__for_keys_26};
            if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_len_fn(NULL, _t1739, 1)))) break;
#line 393 "compiler/main.gem"
            GemVal gem_v_ik = gem_table_get(gem_v__for_keys_26, gem_v__for_i_26);
#line 393 "compiler/main.gem"
            GemVal gem_v__ = gem_table_get(gem_v__for_tbl_26, gem_table_get(gem_v__for_keys_26, gem_v__for_i_26));
#line 393 "compiler/main.gem"
            gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 394 "compiler/main.gem"
    GemVal _t1740[] = {gem_v_captured, gem_v_ik};
            (void)(gem_fn_set_add(NULL, _t1740, 2));
        }

#line 396 "compiler/main.gem"
    GemVal _t1741[] = {gem_v_scope_vars, gem_v_inner_defined};
    GemVal _t1742[] = {gem_v_body, gem_fn_set_union(NULL, _t1741, 2), gem_v_captured};
    GemVal _t1743 = (*gem_v_walk_captures);
        GemVal _t1744 = _t1743.fn(_t1743.env, _t1742, 3);
        gem_pop_frame();
        return _t1744;
    } else {
    GemVal _t1745[] = {gem_v__match_25};
    GemVal _t1747;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1745, 1), gem_string("table")))) {
        _t1747 = gem_eq(gem_type_fn(NULL, _t1745, 1), gem_string("table"));
    } else {
        GemVal _t1746[] = {gem_v__match_25, gem_string("tag")};
        _t1747 = gem_has_key_fn(NULL, _t1746, 2);
    }
    GemVal _t1748;
    if (!gem_truthy(_t1747)) {
        _t1748 = _t1747;
    } else {
        _t1748 = gem_eq(gem_table_get(gem_v__match_25, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1748)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1749[] = {gem_v__match_25};
    GemVal _t1751;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1749, 1), gem_string("table")))) {
        _t1751 = gem_eq(gem_type_fn(NULL, _t1749, 1), gem_string("table"));
    } else {
        GemVal _t1750[] = {gem_v__match_25, gem_string("tag")};
        _t1751 = gem_has_key_fn(NULL, _t1750, 2);
    }
    GemVal _t1752;
    if (!gem_truthy(_t1751)) {
        _t1752 = _t1751;
    } else {
        _t1752 = gem_eq(gem_table_get(gem_v__match_25, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1754;
    if (!gem_truthy(_t1752)) {
        _t1754 = _t1752;
    } else {
        GemVal _t1753[] = {gem_v__match_25, gem_string("cond")};
        _t1754 = gem_has_key_fn(NULL, _t1753, 2);
    }
    GemVal _t1756;
    if (!gem_truthy(_t1754)) {
        _t1756 = _t1754;
    } else {
        GemVal _t1755[] = {gem_v__match_25, gem_string("then")};
        _t1756 = gem_has_key_fn(NULL, _t1755, 2);
    }
    GemVal _t1758;
    if (!gem_truthy(_t1756)) {
        _t1758 = _t1756;
    } else {
        GemVal _t1757[] = {gem_v__match_25, gem_string("else")};
        _t1758 = gem_has_key_fn(NULL, _t1757, 2);
    }
    if (gem_truthy(_t1758)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_25, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_25, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_25, gem_string("else"));
#line 400 "compiler/main.gem"
    GemVal _t1759[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t1760 = (*gem_v_walk_captures_node);
        (void)(_t1760.fn(_t1760.env, _t1759, 3));
#line 401 "compiler/main.gem"
    GemVal _t1761[] = {gem_v_thens, gem_v_scope_vars, gem_v_captured};
    GemVal _t1762 = (*gem_v_walk_captures);
        (void)(_t1762.fn(_t1762.env, _t1761, 3));
#line 402 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 403 "compiler/main.gem"
    GemVal _t1763[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t1764 = (*gem_v_walk_captures);
            GemVal _t1765 = _t1764.fn(_t1764.env, _t1763, 3);
            gem_pop_frame();
            return _t1765;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1766[] = {gem_v__match_25};
    GemVal _t1768;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1766, 1), gem_string("table")))) {
        _t1768 = gem_eq(gem_type_fn(NULL, _t1766, 1), gem_string("table"));
    } else {
        GemVal _t1767[] = {gem_v__match_25, gem_string("tag")};
        _t1768 = gem_has_key_fn(NULL, _t1767, 2);
    }
    GemVal _t1769;
    if (!gem_truthy(_t1768)) {
        _t1769 = _t1768;
    } else {
        _t1769 = gem_eq(gem_table_get(gem_v__match_25, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1771;
    if (!gem_truthy(_t1769)) {
        _t1771 = _t1769;
    } else {
        GemVal _t1770[] = {gem_v__match_25, gem_string("cond")};
        _t1771 = gem_has_key_fn(NULL, _t1770, 2);
    }
    GemVal _t1773;
    if (!gem_truthy(_t1771)) {
        _t1773 = _t1771;
    } else {
        GemVal _t1772[] = {gem_v__match_25, gem_string("body")};
        _t1773 = gem_has_key_fn(NULL, _t1772, 2);
    }
    if (gem_truthy(_t1773)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_25, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_25, gem_string("body"));
#line 406 "compiler/main.gem"
    GemVal _t1774[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t1775 = (*gem_v_walk_captures_node);
        (void)(_t1775.fn(_t1775.env, _t1774, 3));
#line 407 "compiler/main.gem"
    GemVal _t1776[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t1777 = (*gem_v_walk_captures);
        GemVal _t1778 = _t1777.fn(_t1777.env, _t1776, 3);
        gem_pop_frame();
        return _t1778;
    } else {
    GemVal _t1779[] = {gem_v__match_25};
    GemVal _t1781;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1779, 1), gem_string("table")))) {
        _t1781 = gem_eq(gem_type_fn(NULL, _t1779, 1), gem_string("table"));
    } else {
        GemVal _t1780[] = {gem_v__match_25, gem_string("tag")};
        _t1781 = gem_has_key_fn(NULL, _t1780, 2);
    }
    GemVal _t1782;
    if (!gem_truthy(_t1781)) {
        _t1782 = _t1781;
    } else {
        _t1782 = gem_eq(gem_table_get(gem_v__match_25, gem_string("tag")), gem_string("match"));
    }
    GemVal _t1784;
    if (!gem_truthy(_t1782)) {
        _t1784 = _t1782;
    } else {
        GemVal _t1783[] = {gem_v__match_25, gem_string("target")};
        _t1784 = gem_has_key_fn(NULL, _t1783, 2);
    }
    GemVal _t1786;
    if (!gem_truthy(_t1784)) {
        _t1786 = _t1784;
    } else {
        GemVal _t1785[] = {gem_v__match_25, gem_string("whens")};
        _t1786 = gem_has_key_fn(NULL, _t1785, 2);
    }
    GemVal _t1788;
    if (!gem_truthy(_t1786)) {
        _t1788 = _t1786;
    } else {
        GemVal _t1787[] = {gem_v__match_25, gem_string("else")};
        _t1788 = gem_has_key_fn(NULL, _t1787, 2);
    }
    if (gem_truthy(_t1788)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_25, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_25, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_25, gem_string("else"));
#line 409 "compiler/main.gem"
    GemVal _t1789[] = {gem_v_target, gem_v_scope_vars, gem_v_captured};
    GemVal _t1790 = (*gem_v_walk_captures_node);
        (void)(_t1790.fn(_t1790.env, _t1789, 3));
#line 410 "compiler/main.gem"
        GemVal gem_v__for_i_28 = gem_int(0);
#line 410 "compiler/main.gem"
    GemVal _t1791[] = {gem_v_whens};
        GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1791, 1);
#line 410 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 410 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_28;
#line 410 "compiler/main.gem"
            gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 411 "compiler/main.gem"
    GemVal _t1792 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1793[] = {gem_table_get(_t1792, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1794 = (*gem_v_walk_captures_node);
            (void)(_t1794.fn(_t1794.env, _t1793, 3));
#line 412 "compiler/main.gem"
    GemVal _t1795 = gem_table_get(gem_v_whens, gem_v_i);
            if (gem_truthy(gem_neq(gem_table_get(_t1795, gem_string("bindings")), GEM_NIL))) {
#line 413 "compiler/main.gem"
                GemVal gem_v__for_i_27 = gem_int(0);
#line 413 "compiler/main.gem"
    GemVal _t1796 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1797[] = {gem_table_get(_t1796, gem_string("bindings"))};
                GemVal gem_v__for_limit_27 = gem_len_fn(NULL, _t1797, 1);
#line 413 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 413 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_27;
#line 413 "compiler/main.gem"
                    gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 414 "compiler/main.gem"
    GemVal _t1798 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1799[] = {gem_table_get(gem_table_get(_t1798, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t1800 = (*gem_v_walk_captures_node);
                    (void)(_t1800.fn(_t1800.env, _t1799, 3));
                }

            }
#line 417 "compiler/main.gem"
    GemVal _t1801 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1802[] = {gem_table_get(_t1801, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1803 = (*gem_v_walk_captures);
            (void)(_t1803.fn(_t1803.env, _t1802, 3));
        }

#line 419 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 420 "compiler/main.gem"
    GemVal _t1804[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t1805 = (*gem_v_walk_captures);
            GemVal _t1806 = _t1805.fn(_t1805.env, _t1804, 3);
            gem_pop_frame();
            return _t1806;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1807[] = {gem_v__match_25};
    GemVal _t1809;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1807, 1), gem_string("table")))) {
        _t1809 = gem_eq(gem_type_fn(NULL, _t1807, 1), gem_string("table"));
    } else {
        GemVal _t1808[] = {gem_v__match_25, gem_string("tag")};
        _t1809 = gem_has_key_fn(NULL, _t1808, 2);
    }
    GemVal _t1810;
    if (!gem_truthy(_t1809)) {
        _t1810 = _t1809;
    } else {
        _t1810 = gem_eq(gem_table_get(gem_v__match_25, gem_string("tag")), gem_string("block"));
    }
    GemVal _t1812;
    if (!gem_truthy(_t1810)) {
        _t1812 = _t1810;
    } else {
        GemVal _t1811[] = {gem_v__match_25, gem_string("stmts")};
        _t1812 = gem_has_key_fn(NULL, _t1811, 2);
    }
    if (gem_truthy(_t1812)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_25, gem_string("stmts"));
#line 423 "compiler/main.gem"
    GemVal _t1813[] = {gem_v_stmts, gem_v_scope_vars, gem_v_captured};
    GemVal _t1814 = (*gem_v_walk_captures);
        GemVal _t1815 = _t1814.fn(_t1814.env, _t1813, 3);
        gem_pop_frame();
        return _t1815;
    } else {
    GemVal _t1816[] = {gem_v__match_25};
    GemVal _t1818;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1816, 1), gem_string("table")))) {
        _t1818 = gem_eq(gem_type_fn(NULL, _t1816, 1), gem_string("table"));
    } else {
        GemVal _t1817[] = {gem_v__match_25, gem_string("tag")};
        _t1818 = gem_has_key_fn(NULL, _t1817, 2);
    }
    GemVal _t1819;
    if (!gem_truthy(_t1818)) {
        _t1819 = _t1818;
    } else {
        _t1819 = gem_eq(gem_table_get(gem_v__match_25, gem_string("tag")), gem_string("return"));
    }
    GemVal _t1821;
    if (!gem_truthy(_t1819)) {
        _t1821 = _t1819;
    } else {
        GemVal _t1820[] = {gem_v__match_25, gem_string("value")};
        _t1821 = gem_has_key_fn(NULL, _t1820, 2);
    }
    if (gem_truthy(_t1821)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_25, gem_string("value"));
#line 425 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 426 "compiler/main.gem"
    GemVal _t1822[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t1823 = (*gem_v_walk_captures_node);
            GemVal _t1824 = _t1823.fn(_t1823.env, _t1822, 3);
            gem_pop_frame();
            return _t1824;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1825[] = {gem_v__match_25};
    GemVal _t1827;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1825, 1), gem_string("table")))) {
        _t1827 = gem_eq(gem_type_fn(NULL, _t1825, 1), gem_string("table"));
    } else {
        GemVal _t1826[] = {gem_v__match_25, gem_string("tag")};
        _t1827 = gem_has_key_fn(NULL, _t1826, 2);
    }
    GemVal _t1828;
    if (!gem_truthy(_t1827)) {
        _t1828 = _t1827;
    } else {
        _t1828 = gem_eq(gem_table_get(gem_v__match_25, gem_string("tag")), gem_string("let"));
    }
    GemVal _t1830;
    if (!gem_truthy(_t1828)) {
        _t1830 = _t1828;
    } else {
        GemVal _t1829[] = {gem_v__match_25, gem_string("value")};
        _t1830 = gem_has_key_fn(NULL, _t1829, 2);
    }
    if (gem_truthy(_t1830)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_25, gem_string("value"));
#line 429 "compiler/main.gem"
    GemVal _t1831[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t1832 = (*gem_v_walk_captures_node);
        GemVal _t1833 = _t1832.fn(_t1832.env, _t1831, 3);
        gem_pop_frame();
        return _t1833;
    } else {
    GemVal _t1834[] = {gem_v__match_25};
    GemVal _t1836;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1834, 1), gem_string("table")))) {
        _t1836 = gem_eq(gem_type_fn(NULL, _t1834, 1), gem_string("table"));
    } else {
        GemVal _t1835[] = {gem_v__match_25, gem_string("tag")};
        _t1836 = gem_has_key_fn(NULL, _t1835, 2);
    }
    GemVal _t1837;
    if (!gem_truthy(_t1836)) {
        _t1837 = _t1836;
    } else {
        _t1837 = gem_eq(gem_table_get(gem_v__match_25, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t1839;
    if (!gem_truthy(_t1837)) {
        _t1839 = _t1837;
    } else {
        GemVal _t1838[] = {gem_v__match_25, gem_string("value")};
        _t1839 = gem_has_key_fn(NULL, _t1838, 2);
    }
    if (gem_truthy(_t1839)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_25, gem_string("value"));
#line 431 "compiler/main.gem"
    GemVal _t1840[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t1841 = (*gem_v_walk_captures_node);
        GemVal _t1842 = _t1841.fn(_t1841.env, _t1840, 3);
        gem_pop_frame();
        return _t1842;
    } else {
        {
#line 434 "compiler/main.gem"
            GemVal gem_v__for_tbl_29 = gem_v_node;
#line 434 "compiler/main.gem"
    GemVal _t1843[] = {gem_v__for_tbl_29};
            GemVal gem_v__for_keys_29 = gem_keys_fn(NULL, _t1843, 1);
#line 434 "compiler/main.gem"
            GemVal gem_v__for_i_29 = gem_int(0);
#line 434 "compiler/main.gem"
            while (1) {
                GemVal _t1844[] = {gem_v__for_keys_29};
                if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_len_fn(NULL, _t1844, 1)))) break;
#line 434 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_29, gem_v__for_i_29);
#line 434 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_29, gem_table_get(gem_v__for_keys_29, gem_v__for_i_29));
#line 434 "compiler/main.gem"
                gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 435 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 436 "compiler/main.gem"
    GemVal _t1845[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1845, 1), gem_string("table")))) {
#line 437 "compiler/main.gem"
    GemVal _t1846[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t1846, 1);
#line 438 "compiler/main.gem"
    GemVal _t1847[] = {gem_v_vks};
    GemVal _t1849;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1847, 1), gem_int(0)))) {
        _t1849 = gem_gt(gem_len_fn(NULL, _t1847, 1), gem_int(0));
    } else {
        GemVal _t1848[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1849 = gem_eq(gem_type_fn(NULL, _t1848, 1), gem_string("int"));
    }
                        if (gem_truthy(_t1849)) {
#line 439 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 440 "compiler/main.gem"
                            while (1) {
                                GemVal _t1850[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1850, 1)))) break;
#line 441 "compiler/main.gem"
    GemVal _t1851[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1851, 1), gem_string("table")))) {
#line 442 "compiler/main.gem"
    GemVal _t1852[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t1853 = (*gem_v_walk_captures_node);
                                    (void)(_t1853.fn(_t1853.env, _t1852, 3));
                                }
#line 444 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 447 "compiler/main.gem"
    GemVal _t1854[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t1855 = (*gem_v_walk_captures_node);
                            (void)(_t1855.fn(_t1855.env, _t1854, 3));
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

struct _closure__anon_30 {
    GemVal *gem_v_walk_captures_node;
};
static GemVal _anon_30(void *_env, GemVal *args, int argc) {
    struct _closure__anon_30 *_cls = (struct _closure__anon_30 *)_env;
    GemVal *gem_v_walk_captures_node = _cls->gem_v_walk_captures_node;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_scope_vars = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_captured = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_30", "compiler/main.gem", 0);
#line 456 "compiler/main.gem"
    GemVal _t1857 = gem_table_new();
    GemVal _t1858[] = {gem_v_scope_vars, _t1857};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t1858, 2);
    {
#line 457 "compiler/main.gem"
        GemVal gem_v__for_i_30 = gem_int(0);
#line 457 "compiler/main.gem"
    GemVal _t1859[] = {gem_v_stmts};
        GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t1859, 1);
#line 457 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 457 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_30;
#line 457 "compiler/main.gem"
            gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 458 "compiler/main.gem"
    GemVal _t1860[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t1861 = (*gem_v_walk_captures_node);
            (void)(_t1861.fn(_t1861.env, _t1860, 3));
#line 459 "compiler/main.gem"
    GemVal _t1862[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1864;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1862, 1), gem_string("table")))) {
        _t1864 = gem_eq(gem_type_fn(NULL, _t1862, 1), gem_string("table"));
    } else {
        GemVal _t1863 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1864 = gem_eq(gem_table_get(_t1863, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1864)) {
#line 460 "compiler/main.gem"
    GemVal _t1865 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1866[] = {gem_v_sv, gem_table_get(_t1865, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1866, 2));
            }
#line 462 "compiler/main.gem"
    GemVal _t1867[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv};
            (void)(gem_fn_collect_block_lets(NULL, _t1867, 2));
        }
        gem_pop_frame();
        return GEM_NIL;
    }
}

struct _closure__anon_31 {
    GemVal *gem_v_walk_captures;
};
static GemVal _anon_31(void *_env, GemVal *args, int argc) {
    struct _closure__anon_31 *_cls = (struct _closure__anon_31 *)_env;
    GemVal *gem_v_walk_captures = _cls->gem_v_walk_captures;
    GemVal gem_v_body = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_scope_vars = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_31", "compiler/main.gem", 0);
#line 467 "compiler/main.gem"
    GemVal _t1869 = gem_table_new();
    GemVal gem_v_captured = _t1869;
#line 468 "compiler/main.gem"
    GemVal _t1870[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t1871 = (*gem_v_walk_captures);
    (void)(_t1871.fn(_t1871.env, _t1870, 3));
    GemVal _t1872 = gem_v_captured;
    gem_pop_frame();
    return _t1872;
}

struct _closure__anon_32 {
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
static GemVal _anon_32(void *_env, GemVal *args, int argc) {
    struct _closure__anon_32 *_cls = (struct _closure__anon_32 *)_env;
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
    gem_push_frame("_anon_32", "compiler/main.gem", 0);
#line 495 "compiler/main.gem"
    GemVal gem_v__match_31 = gem_v_node;
    GemVal _t1874[] = {gem_v__match_31};
    GemVal _t1876;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1874, 1), gem_string("table")))) {
        _t1876 = gem_eq(gem_type_fn(NULL, _t1874, 1), gem_string("table"));
    } else {
        GemVal _t1875[] = {gem_v__match_31, gem_string("tag")};
        _t1876 = gem_has_key_fn(NULL, _t1875, 2);
    }
    GemVal _t1877;
    if (!gem_truthy(_t1876)) {
        _t1877 = _t1876;
    } else {
        _t1877 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("int"));
    }
    GemVal _t1879;
    if (!gem_truthy(_t1877)) {
        _t1879 = _t1877;
    } else {
        GemVal _t1878[] = {gem_v__match_31, gem_string("value")};
        _t1879 = gem_has_key_fn(NULL, _t1878, 2);
    }
    if (gem_truthy(_t1879)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 497 "compiler/main.gem"
    GemVal _t1880 = gem_table_new();
    GemVal _t1881[] = {gem_v_value};
    gem_table_set(_t1880, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t1881, 1)), gem_string(")")));
    gem_table_set(_t1880, gem_string("setup"), gem_string(""));
        GemVal _t1882 = _t1880;
        gem_pop_frame();
        return _t1882;
    } else {
    GemVal _t1883[] = {gem_v__match_31};
    GemVal _t1885;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1883, 1), gem_string("table")))) {
        _t1885 = gem_eq(gem_type_fn(NULL, _t1883, 1), gem_string("table"));
    } else {
        GemVal _t1884[] = {gem_v__match_31, gem_string("tag")};
        _t1885 = gem_has_key_fn(NULL, _t1884, 2);
    }
    GemVal _t1886;
    if (!gem_truthy(_t1885)) {
        _t1886 = _t1885;
    } else {
        _t1886 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("float"));
    }
    GemVal _t1888;
    if (!gem_truthy(_t1886)) {
        _t1888 = _t1886;
    } else {
        GemVal _t1887[] = {gem_v__match_31, gem_string("value")};
        _t1888 = gem_has_key_fn(NULL, _t1887, 2);
    }
    if (gem_truthy(_t1888)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 499 "compiler/main.gem"
    GemVal _t1889 = gem_table_new();
    GemVal _t1890[] = {gem_v_value};
    GemVal _t1891[] = {gem_fn_format_float(NULL, _t1890, 1)};
    gem_table_set(_t1889, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t1891, 1)), gem_string(")")));
    gem_table_set(_t1889, gem_string("setup"), gem_string(""));
        GemVal _t1892 = _t1889;
        gem_pop_frame();
        return _t1892;
    } else {
    GemVal _t1893[] = {gem_v__match_31};
    GemVal _t1895;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1893, 1), gem_string("table")))) {
        _t1895 = gem_eq(gem_type_fn(NULL, _t1893, 1), gem_string("table"));
    } else {
        GemVal _t1894[] = {gem_v__match_31, gem_string("tag")};
        _t1895 = gem_has_key_fn(NULL, _t1894, 2);
    }
    GemVal _t1896;
    if (!gem_truthy(_t1895)) {
        _t1896 = _t1895;
    } else {
        _t1896 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("string"));
    }
    GemVal _t1898;
    if (!gem_truthy(_t1896)) {
        _t1898 = _t1896;
    } else {
        GemVal _t1897[] = {gem_v__match_31, gem_string("value")};
        _t1898 = gem_has_key_fn(NULL, _t1897, 2);
    }
    if (gem_truthy(_t1898)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 501 "compiler/main.gem"
    GemVal _t1899[] = {gem_v_value};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1899, 1);
#line 502 "compiler/main.gem"
    GemVal _t1900 = gem_table_new();
    GemVal _t1901[] = {gem_v_escaped};
    gem_table_set(_t1900, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t1901, 1)), gem_string("\")")));
    gem_table_set(_t1900, gem_string("setup"), gem_string(""));
        GemVal _t1902 = _t1900;
        gem_pop_frame();
        return _t1902;
    } else {
    GemVal _t1903[] = {gem_v__match_31};
    GemVal _t1905;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1903, 1), gem_string("table")))) {
        _t1905 = gem_eq(gem_type_fn(NULL, _t1903, 1), gem_string("table"));
    } else {
        GemVal _t1904[] = {gem_v__match_31, gem_string("tag")};
        _t1905 = gem_has_key_fn(NULL, _t1904, 2);
    }
    GemVal _t1906;
    if (!gem_truthy(_t1905)) {
        _t1906 = _t1905;
    } else {
        _t1906 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t1908;
    if (!gem_truthy(_t1906)) {
        _t1908 = _t1906;
    } else {
        GemVal _t1907[] = {gem_v__match_31, gem_string("parts")};
        _t1908 = gem_has_key_fn(NULL, _t1907, 2);
    }
    if (gem_truthy(_t1908)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_parts = gem_table_get(gem_v__match_31, gem_string("parts"));
#line 504 "compiler/main.gem"
    GemVal _t1909[] = {gem_v_parts};
        if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1909, 1), gem_int(0)))) {
#line 505 "compiler/main.gem"
    GemVal _t1910 = gem_table_new();
    gem_table_set(_t1910, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t1910, gem_string("setup"), gem_string(""));
            GemVal _t1911 = _t1910;
            gem_pop_frame();
            return _t1911;
        }
#line 507 "compiler/main.gem"
        GemVal gem_v_setup = gem_string("");
#line 508 "compiler/main.gem"
    GemVal _t1912 = gem_table_new();
        GemVal gem_v_compiled = _t1912;
#line 509 "compiler/main.gem"
        GemVal gem_v__for_i_32 = gem_int(0);
#line 509 "compiler/main.gem"
    GemVal _t1913[] = {gem_v_parts};
        GemVal gem_v__for_limit_32 = gem_len_fn(NULL, _t1913, 1);
#line 509 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_v__for_limit_32))) break;
#line 509 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_32;
#line 509 "compiler/main.gem"
            gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 510 "compiler/main.gem"
            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 511 "compiler/main.gem"
    GemVal _t1914[] = {gem_v_part};
    GemVal _t1915 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t1915.fn(_t1915.env, _t1914, 1);
#line 512 "compiler/main.gem"
    GemVal _t1916 = gem_v_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1916, gem_string("setup")));
#line 513 "compiler/main.gem"
    GemVal _t1917 = gem_v_part;
            if (gem_truthy(gem_eq(gem_table_get(_t1917, gem_string("tag")), gem_string("string")))) {
#line 514 "compiler/main.gem"
    GemVal _t1918 = gem_v_r;
    GemVal _t1919[] = {gem_v_compiled, gem_table_get(_t1918, gem_string("expr"))};
                (void)(gem_push_fn(NULL, _t1919, 2));
            } else {
#line 516 "compiler/main.gem"
    GemVal _t1920 = (*gem_v_tmp);
                GemVal gem_v_ts = _t1920.fn(_t1920.env, NULL, 0);
#line 517 "compiler/main.gem"
    GemVal _t1921[] = {gem_v_ts};
    GemVal _t1922 = gem_v_r;
    GemVal _t1923[] = {gem_table_get(_t1922, gem_string("expr"))};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1921, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1923, 1)), gem_string("};\n")));
#line 518 "compiler/main.gem"
    GemVal _t1924[] = {gem_v_ts};
    GemVal _t1925[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t1924, 1)), gem_string(", 1)"))};
                (void)(gem_push_fn(NULL, _t1925, 2));
            }
        }

#line 521 "compiler/main.gem"
        GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
#line 522 "compiler/main.gem"
        GemVal gem_v__for_i_33 = gem_int(1);
#line 522 "compiler/main.gem"
    GemVal _t1926[] = {gem_v_compiled};
        GemVal gem_v__for_limit_33 = gem_len_fn(NULL, _t1926, 1);
#line 522 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
#line 522 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_33;
#line 522 "compiler/main.gem"
            gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 523 "compiler/main.gem"
    GemVal _t1927[] = {gem_v_acc};
    GemVal _t1928[] = {gem_table_get(gem_v_compiled, gem_v_i)};
            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1927, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1928, 1)), gem_string(")"));
        }

#line 525 "compiler/main.gem"
    GemVal _t1929 = gem_table_new();
    gem_table_set(_t1929, gem_string("expr"), gem_v_acc);
    gem_table_set(_t1929, gem_string("setup"), gem_v_setup);
        GemVal _t1930 = _t1929;
        gem_pop_frame();
        return _t1930;
    } else {
    GemVal _t1931[] = {gem_v__match_31};
    GemVal _t1933;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1931, 1), gem_string("table")))) {
        _t1933 = gem_eq(gem_type_fn(NULL, _t1931, 1), gem_string("table"));
    } else {
        GemVal _t1932[] = {gem_v__match_31, gem_string("tag")};
        _t1933 = gem_has_key_fn(NULL, _t1932, 2);
    }
    GemVal _t1934;
    if (!gem_truthy(_t1933)) {
        _t1934 = _t1933;
    } else {
        _t1934 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("bool"));
    }
    GemVal _t1936;
    if (!gem_truthy(_t1934)) {
        _t1936 = _t1934;
    } else {
        GemVal _t1935[] = {gem_v__match_31, gem_string("value")};
        _t1936 = gem_has_key_fn(NULL, _t1935, 2);
    }
    if (gem_truthy(_t1936)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 527 "compiler/main.gem"
        if (gem_truthy(gem_v_value)) {
#line 528 "compiler/main.gem"
    GemVal _t1937 = gem_table_new();
    gem_table_set(_t1937, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t1937, gem_string("setup"), gem_string(""));
            GemVal _t1938 = _t1937;
            gem_pop_frame();
            return _t1938;
        }
#line 530 "compiler/main.gem"
    GemVal _t1939 = gem_table_new();
    gem_table_set(_t1939, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t1939, gem_string("setup"), gem_string(""));
        GemVal _t1940 = _t1939;
        gem_pop_frame();
        return _t1940;
    } else {
    GemVal _t1941[] = {gem_v__match_31};
    GemVal _t1943;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1941, 1), gem_string("table")))) {
        _t1943 = gem_eq(gem_type_fn(NULL, _t1941, 1), gem_string("table"));
    } else {
        GemVal _t1942[] = {gem_v__match_31, gem_string("tag")};
        _t1943 = gem_has_key_fn(NULL, _t1942, 2);
    }
    GemVal _t1944;
    if (!gem_truthy(_t1943)) {
        _t1944 = _t1943;
    } else {
        _t1944 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("nil"));
    }
    if (gem_truthy(_t1944)) {
#line 532 "compiler/main.gem"
    GemVal _t1945 = gem_table_new();
    gem_table_set(_t1945, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1945, gem_string("setup"), gem_string(""));
        GemVal _t1946 = _t1945;
        gem_pop_frame();
        return _t1946;
    } else {
    GemVal _t1947[] = {gem_v__match_31};
    GemVal _t1949;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1947, 1), gem_string("table")))) {
        _t1949 = gem_eq(gem_type_fn(NULL, _t1947, 1), gem_string("table"));
    } else {
        GemVal _t1948[] = {gem_v__match_31, gem_string("tag")};
        _t1949 = gem_has_key_fn(NULL, _t1948, 2);
    }
    GemVal _t1950;
    if (!gem_truthy(_t1949)) {
        _t1950 = _t1949;
    } else {
        _t1950 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("var"));
    }
    GemVal _t1952;
    if (!gem_truthy(_t1950)) {
        _t1952 = _t1950;
    } else {
        GemVal _t1951[] = {gem_v__match_31, gem_string("name")};
        _t1952 = gem_has_key_fn(NULL, _t1951, 2);
    }
    if (gem_truthy(_t1952)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_31, gem_string("name"));
#line 534 "compiler/main.gem"
    GemVal _t1953[] = {(*gem_v_builtin_fns), gem_v_name};
        if (gem_truthy(gem_has_key_fn(NULL, _t1953, 2))) {
#line 535 "compiler/main.gem"
    GemVal _t1954 = gem_table_new();
    GemVal _t1955[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name)};
    gem_table_set(_t1954, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1955, 1)), gem_string(", NULL)")));
    gem_table_set(_t1954, gem_string("setup"), gem_string(""));
            GemVal _t1956 = _t1954;
            gem_pop_frame();
            return _t1956;
        } else {
#line 536 "compiler/main.gem"
    GemVal _t1957[] = {(*gem_v_defined_fns), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1957, 2))) {
#line 537 "compiler/main.gem"
    GemVal _t1958 = gem_table_new();
    GemVal _t1959[] = {gem_v_name};
    gem_table_set(_t1958, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t1959, 1)), gem_string(", NULL)")));
    gem_table_set(_t1958, gem_string("setup"), gem_string(""));
                GemVal _t1960 = _t1958;
                gem_pop_frame();
                return _t1960;
            }
        }
#line 539 "compiler/main.gem"
    GemVal _t1961[] = {gem_v_name};
    GemVal _t1962 = (*gem_v_mangle);
        GemVal gem_v_mname = _t1962.fn(_t1962.env, _t1961, 1);
#line 540 "compiler/main.gem"
    GemVal _t1963[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t1965;
    if (gem_truthy(gem_fn_set_contains(NULL, _t1963, 2))) {
        _t1965 = gem_fn_set_contains(NULL, _t1963, 2);
    } else {
        GemVal _t1964[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t1965 = gem_fn_set_contains(NULL, _t1964, 2);
    }
        if (gem_truthy(_t1965)) {
#line 541 "compiler/main.gem"
    GemVal _t1966 = gem_table_new();
    GemVal _t1967[] = {gem_v_mname};
    gem_table_set(_t1966, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t1967, 1)), gem_string(")")));
    gem_table_set(_t1966, gem_string("setup"), gem_string(""));
            GemVal _t1968 = _t1966;
            gem_pop_frame();
            return _t1968;
        }
#line 543 "compiler/main.gem"
    GemVal _t1969 = gem_table_new();
    gem_table_set(_t1969, gem_string("expr"), gem_v_mname);
    gem_table_set(_t1969, gem_string("setup"), gem_string(""));
        GemVal _t1970 = _t1969;
        gem_pop_frame();
        return _t1970;
    } else {
    GemVal _t1971[] = {gem_v__match_31};
    GemVal _t1973;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1971, 1), gem_string("table")))) {
        _t1973 = gem_eq(gem_type_fn(NULL, _t1971, 1), gem_string("table"));
    } else {
        GemVal _t1972[] = {gem_v__match_31, gem_string("tag")};
        _t1973 = gem_has_key_fn(NULL, _t1972, 2);
    }
    GemVal _t1974;
    if (!gem_truthy(_t1973)) {
        _t1974 = _t1973;
    } else {
        _t1974 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t1974)) {
#line 545 "compiler/main.gem"
    GemVal _t1975[] = {gem_v_node};
    GemVal _t1976 = (*gem_v_compile_call);
        GemVal _t1977 = _t1976.fn(_t1976.env, _t1975, 1);
        gem_pop_frame();
        return _t1977;
    } else {
    GemVal _t1978[] = {gem_v__match_31};
    GemVal _t1980;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1978, 1), gem_string("table")))) {
        _t1980 = gem_eq(gem_type_fn(NULL, _t1978, 1), gem_string("table"));
    } else {
        GemVal _t1979[] = {gem_v__match_31, gem_string("tag")};
        _t1980 = gem_has_key_fn(NULL, _t1979, 2);
    }
    GemVal _t1981;
    if (!gem_truthy(_t1980)) {
        _t1981 = _t1980;
    } else {
        _t1981 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("binop"));
    }
    if (gem_truthy(_t1981)) {
#line 547 "compiler/main.gem"
    GemVal _t1982[] = {gem_v_node};
    GemVal _t1983 = (*gem_v_compile_binop);
        GemVal _t1984 = _t1983.fn(_t1983.env, _t1982, 1);
        gem_pop_frame();
        return _t1984;
    } else {
    GemVal _t1985[] = {gem_v__match_31};
    GemVal _t1987;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1985, 1), gem_string("table")))) {
        _t1987 = gem_eq(gem_type_fn(NULL, _t1985, 1), gem_string("table"));
    } else {
        GemVal _t1986[] = {gem_v__match_31, gem_string("tag")};
        _t1987 = gem_has_key_fn(NULL, _t1986, 2);
    }
    GemVal _t1988;
    if (!gem_truthy(_t1987)) {
        _t1988 = _t1987;
    } else {
        _t1988 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t1990;
    if (!gem_truthy(_t1988)) {
        _t1990 = _t1988;
    } else {
        GemVal _t1989[] = {gem_v__match_31, gem_string("expr")};
        _t1990 = gem_has_key_fn(NULL, _t1989, 2);
    }
    GemVal _t1992;
    if (!gem_truthy(_t1990)) {
        _t1992 = _t1990;
    } else {
        GemVal _t1991[] = {gem_v__match_31, gem_string("op")};
        _t1992 = gem_has_key_fn(NULL, _t1991, 2);
    }
    if (gem_truthy(_t1992)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_uexpr = gem_table_get(gem_v__match_31, gem_string("expr"));
#line 0 "compiler/main.gem"
        GemVal gem_v_op = gem_table_get(gem_v__match_31, gem_string("op"));
#line 549 "compiler/main.gem"
    GemVal _t1993[] = {gem_v_uexpr};
    GemVal _t1994 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t1994.fn(_t1994.env, _t1993, 1);
#line 550 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 551 "compiler/main.gem"
    GemVal _t1995 = gem_table_new();
    GemVal _t1996 = gem_v_r;
    GemVal _t1997[] = {gem_table_get(_t1996, gem_string("expr"))};
    gem_table_set(_t1995, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t1997, 1)), gem_string(")")));
    GemVal _t1998 = gem_v_r;
    gem_table_set(_t1995, gem_string("setup"), gem_table_get(_t1998, gem_string("setup")));
            GemVal _t1999 = _t1995;
            gem_pop_frame();
            return _t1999;
        } else {
#line 552 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("not")))) {
#line 553 "compiler/main.gem"
    GemVal _t2000 = gem_table_new();
    GemVal _t2001 = gem_v_r;
    GemVal _t2002[] = {gem_table_get(_t2001, gem_string("expr"))};
    gem_table_set(_t2000, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t2002, 1)), gem_string(")")));
    GemVal _t2003 = gem_v_r;
    gem_table_set(_t2000, gem_string("setup"), gem_table_get(_t2003, gem_string("setup")));
                GemVal _t2004 = _t2000;
                gem_pop_frame();
                return _t2004;
            }
        }
#line 555 "compiler/main.gem"
    GemVal _t2005[] = {gem_v_op};
    GemVal _t2006[] = {gem_add(gem_string("unknown unary operator: "), gem_to_string_fn(NULL, _t2005, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 555, _t2006, 1));
    } else {
    GemVal _t2007[] = {gem_v__match_31};
    GemVal _t2009;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2007, 1), gem_string("table")))) {
        _t2009 = gem_eq(gem_type_fn(NULL, _t2007, 1), gem_string("table"));
    } else {
        GemVal _t2008[] = {gem_v__match_31, gem_string("tag")};
        _t2009 = gem_has_key_fn(NULL, _t2008, 2);
    }
    GemVal _t2010;
    if (!gem_truthy(_t2009)) {
        _t2010 = _t2009;
    } else {
        _t2010 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("anon_fn"));
    }
    if (gem_truthy(_t2010)) {
#line 557 "compiler/main.gem"
    GemVal _t2011[] = {gem_v_node};
    GemVal _t2012 = (*gem_v_compile_anon_fn);
        GemVal _t2013 = _t2012.fn(_t2012.env, _t2011, 1);
        gem_pop_frame();
        return _t2013;
    } else {
    GemVal _t2014[] = {gem_v__match_31};
    GemVal _t2016;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2014, 1), gem_string("table")))) {
        _t2016 = gem_eq(gem_type_fn(NULL, _t2014, 1), gem_string("table"));
    } else {
        GemVal _t2015[] = {gem_v__match_31, gem_string("tag")};
        _t2016 = gem_has_key_fn(NULL, _t2015, 2);
    }
    GemVal _t2017;
    if (!gem_truthy(_t2016)) {
        _t2017 = _t2016;
    } else {
        _t2017 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("table"));
    }
    if (gem_truthy(_t2017)) {
#line 559 "compiler/main.gem"
    GemVal _t2018[] = {gem_v_node};
    GemVal _t2019 = (*gem_v_compile_table);
        GemVal _t2020 = _t2019.fn(_t2019.env, _t2018, 1);
        gem_pop_frame();
        return _t2020;
    } else {
    GemVal _t2021[] = {gem_v__match_31};
    GemVal _t2023;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2021, 1), gem_string("table")))) {
        _t2023 = gem_eq(gem_type_fn(NULL, _t2021, 1), gem_string("table"));
    } else {
        GemVal _t2022[] = {gem_v__match_31, gem_string("tag")};
        _t2023 = gem_has_key_fn(NULL, _t2022, 2);
    }
    GemVal _t2024;
    if (!gem_truthy(_t2023)) {
        _t2024 = _t2023;
    } else {
        _t2024 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("array"));
    }
    if (gem_truthy(_t2024)) {
#line 561 "compiler/main.gem"
    GemVal _t2025[] = {gem_v_node};
    GemVal _t2026 = (*gem_v_compile_array);
        GemVal _t2027 = _t2026.fn(_t2026.env, _t2025, 1);
        gem_pop_frame();
        return _t2027;
    } else {
    GemVal _t2028[] = {gem_v__match_31};
    GemVal _t2030;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2028, 1), gem_string("table")))) {
        _t2030 = gem_eq(gem_type_fn(NULL, _t2028, 1), gem_string("table"));
    } else {
        GemVal _t2029[] = {gem_v__match_31, gem_string("tag")};
        _t2030 = gem_has_key_fn(NULL, _t2029, 2);
    }
    GemVal _t2031;
    if (!gem_truthy(_t2030)) {
        _t2031 = _t2030;
    } else {
        _t2031 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t2033;
    if (!gem_truthy(_t2031)) {
        _t2033 = _t2031;
    } else {
        GemVal _t2032[] = {gem_v__match_31, gem_string("object")};
        _t2033 = gem_has_key_fn(NULL, _t2032, 2);
    }
    GemVal _t2035;
    if (!gem_truthy(_t2033)) {
        _t2035 = _t2033;
    } else {
        GemVal _t2034[] = {gem_v__match_31, gem_string("field")};
        _t2035 = gem_has_key_fn(NULL, _t2034, 2);
    }
    if (gem_truthy(_t2035)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_31, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_31, gem_string("field"));
#line 563 "compiler/main.gem"
    GemVal _t2036[] = {gem_v_object};
    GemVal _t2037 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2037.fn(_t2037.env, _t2036, 1);
#line 564 "compiler/main.gem"
    GemVal _t2038 = (*gem_v_tmp);
        GemVal gem_v_t = _t2038.fn(_t2038.env, NULL, 0);
#line 565 "compiler/main.gem"
    GemVal _t2039 = gem_v_r;
    GemVal _t2040[] = {gem_v_t};
    GemVal _t2041 = gem_v_r;
    GemVal _t2042[] = {gem_table_get(_t2041, gem_string("expr"))};
        GemVal gem_v_setup = gem_add(gem_table_get(_t2039, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2040, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2042, 1)), gem_string(";\n")));
#line 566 "compiler/main.gem"
    GemVal _t2043[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2043, 1);
#line 567 "compiler/main.gem"
    GemVal _t2044 = gem_table_new();
    GemVal _t2045[] = {gem_v_t};
    GemVal _t2046[] = {gem_v_escaped};
    gem_table_set(_t2044, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2045, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2046, 1)), gem_string("\"))")));
    gem_table_set(_t2044, gem_string("setup"), gem_v_setup);
        GemVal _t2047 = _t2044;
        gem_pop_frame();
        return _t2047;
    } else {
    GemVal _t2048[] = {gem_v__match_31};
    GemVal _t2050;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2048, 1), gem_string("table")))) {
        _t2050 = gem_eq(gem_type_fn(NULL, _t2048, 1), gem_string("table"));
    } else {
        GemVal _t2049[] = {gem_v__match_31, gem_string("tag")};
        _t2050 = gem_has_key_fn(NULL, _t2049, 2);
    }
    GemVal _t2051;
    if (!gem_truthy(_t2050)) {
        _t2051 = _t2050;
    } else {
        _t2051 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("index"));
    }
    GemVal _t2053;
    if (!gem_truthy(_t2051)) {
        _t2053 = _t2051;
    } else {
        GemVal _t2052[] = {gem_v__match_31, gem_string("object")};
        _t2053 = gem_has_key_fn(NULL, _t2052, 2);
    }
    GemVal _t2055;
    if (!gem_truthy(_t2053)) {
        _t2055 = _t2053;
    } else {
        GemVal _t2054[] = {gem_v__match_31, gem_string("key")};
        _t2055 = gem_has_key_fn(NULL, _t2054, 2);
    }
    if (gem_truthy(_t2055)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_31, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_31, gem_string("key"));
#line 569 "compiler/main.gem"
    GemVal _t2056[] = {gem_v_object};
    GemVal _t2057 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2057.fn(_t2057.env, _t2056, 1);
#line 570 "compiler/main.gem"
    GemVal _t2058[] = {gem_v_key};
    GemVal _t2059 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2059.fn(_t2059.env, _t2058, 1);
#line 571 "compiler/main.gem"
    GemVal _t2060 = gem_table_new();
    GemVal _t2061 = gem_v_obj_r;
    GemVal _t2062[] = {gem_table_get(_t2061, gem_string("expr"))};
    GemVal _t2063 = gem_v_key_r;
    GemVal _t2064[] = {gem_table_get(_t2063, gem_string("expr"))};
    gem_table_set(_t2060, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2062, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2064, 1)), gem_string(")")));
    GemVal _t2065 = gem_v_obj_r;
    GemVal _t2066 = gem_v_key_r;
    gem_table_set(_t2060, gem_string("setup"), gem_add(gem_table_get(_t2065, gem_string("setup")), gem_table_get(_t2066, gem_string("setup"))));
        GemVal _t2067 = _t2060;
        gem_pop_frame();
        return _t2067;
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
#line 574 "compiler/main.gem"
    GemVal _t2068 = gem_v_node;
    GemVal _t2069[] = {gem_table_get(_t2068, gem_string("tag"))};
    GemVal _t2070[] = {gem_add(gem_string("unknown expression node: "), gem_to_string_fn(NULL, _t2069, 1))};
    GemVal _t2071 = gem_error_at_fn("compiler/main.gem", 574, _t2070, 1);
    gem_pop_frame();
    return _t2071;
}

struct _closure__anon_33 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_33(void *_env, GemVal *args, int argc) {
    struct _closure__anon_33 *_cls = (struct _closure__anon_33 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_33", "compiler/main.gem", 0);
#line 580 "compiler/main.gem"
    GemVal _t2073 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t2073, gem_string("entries"));
#line 581 "compiler/main.gem"
    GemVal _t2074 = (*gem_v_tmp);
    GemVal gem_v_t = _t2074.fn(_t2074.env, NULL, 0);
#line 582 "compiler/main.gem"
    GemVal _t2075[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2075, 1)), gem_string(" = gem_table_new();\n"));
#line 583 "compiler/main.gem"
    GemVal gem_v__for_items_34 = gem_v_entries;
#line 583 "compiler/main.gem"
    GemVal gem_v__for_i_34 = gem_int(0);
#line 583 "compiler/main.gem"
    while (1) {
        GemVal _t2076[] = {gem_v__for_items_34};
        if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_len_fn(NULL, _t2076, 1)))) break;
#line 583 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_34, gem_v__for_i_34);
#line 583 "compiler/main.gem"
        gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 584 "compiler/main.gem"
    GemVal _t2077 = gem_v_entry;
    GemVal _t2078[] = {gem_table_get(_t2077, gem_string("value"))};
    GemVal _t2079 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2079.fn(_t2079.env, _t2078, 1);
#line 585 "compiler/main.gem"
    GemVal _t2080 = gem_v_entry;
    GemVal _t2081[] = {gem_table_get(_t2080, gem_string("key"))};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2081, 1);
#line 586 "compiler/main.gem"
    GemVal _t2082 = gem_v_val_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2082, gem_string("setup")));
#line 587 "compiler/main.gem"
    GemVal _t2083[] = {gem_v_t};
    GemVal _t2084[] = {gem_v_escaped};
    GemVal _t2085 = gem_v_val_r;
    GemVal _t2086[] = {gem_table_get(_t2085, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2083, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2084, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2086, 1)), gem_string(");\n")));
    }

    GemVal _t2087 = gem_table_new();
    gem_table_set(_t2087, gem_string("expr"), gem_v_t);
    gem_table_set(_t2087, gem_string("setup"), gem_v_setup);
    GemVal _t2088 = _t2087;
    gem_pop_frame();
    return _t2088;
}

struct _closure__anon_34 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_34(void *_env, GemVal *args, int argc) {
    struct _closure__anon_34 *_cls = (struct _closure__anon_34 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_34", "compiler/main.gem", 0);
#line 595 "compiler/main.gem"
    GemVal _t2090 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t2090, gem_string("elements"));
#line 596 "compiler/main.gem"
    GemVal _t2091 = (*gem_v_tmp);
    GemVal gem_v_t = _t2091.fn(_t2091.env, NULL, 0);
#line 597 "compiler/main.gem"
    GemVal _t2092[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2092, 1)), gem_string(" = gem_table_new();\n"));
#line 598 "compiler/main.gem"
    GemVal gem_v__for_i_35 = gem_int(0);
#line 598 "compiler/main.gem"
    GemVal _t2093[] = {gem_v_elements};
    GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t2093, 1);
#line 598 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 598 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_35;
#line 598 "compiler/main.gem"
        gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 599 "compiler/main.gem"
    GemVal _t2094[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t2095 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t2095.fn(_t2095.env, _t2094, 1);
#line 600 "compiler/main.gem"
    GemVal _t2096 = gem_v_elem_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2096, gem_string("setup")));
#line 601 "compiler/main.gem"
    GemVal _t2097[] = {gem_v_t};
    GemVal _t2098[] = {gem_v_i};
    GemVal _t2099 = gem_v_elem_r;
    GemVal _t2100[] = {gem_table_get(_t2099, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2097, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t2098, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t2100, 1)), gem_string(");\n")));
    }

    GemVal _t2101 = gem_table_new();
    gem_table_set(_t2101, gem_string("expr"), gem_v_t);
    gem_table_set(_t2101, gem_string("setup"), gem_v_setup);
    GemVal _t2102 = _t2101;
    gem_pop_frame();
    return _t2102;
}

struct _closure__anon_35 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_compile_closure_fn;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_35(void *_env, GemVal *args, int argc) {
    struct _closure__anon_35 *_cls = (struct _closure__anon_35 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_compile_closure_fn = _cls->gem_v_compile_closure_fn;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_35", "compiler/main.gem", 0);
#line 609 "compiler/main.gem"
    GemVal _t2104 = gem_v_node;
    GemVal _t2105[] = {gem_table_get(_t2104, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t2105, 1);
#line 610 "compiler/main.gem"
    GemVal _t2106 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2106, gem_string("rest_param")), GEM_NIL))) {
#line 611 "compiler/main.gem"
    GemVal _t2107 = gem_v_node;
    GemVal _t2108[] = {gem_v_inner_defined, gem_table_get(_t2107, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t2108, 2));
    }
#line 613 "compiler/main.gem"
    GemVal _t2109 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2109, gem_string("block_param")), GEM_NIL))) {
#line 614 "compiler/main.gem"
    GemVal _t2110 = gem_v_node;
    GemVal _t2111[] = {gem_v_inner_defined, gem_table_get(_t2110, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t2111, 2));
    }
#line 616 "compiler/main.gem"
    GemVal _t2112 = gem_v_node;
    GemVal _t2113[] = {gem_table_get(_t2112, gem_string("body")), gem_v_inner_defined};
    GemVal _t2114 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t2114.fn(_t2114.env, _t2113, 2);
#line 617 "compiler/main.gem"
    GemVal _t2115[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t2115, 1);
#line 619 "compiler/main.gem"
    GemVal _t2116[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2116, 1), gem_int(0)))) {
#line 620 "compiler/main.gem"
    GemVal _t2117 = gem_table_new();
    GemVal _t2118[] = {gem_v_node, _t2117};
    GemVal _t2119 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t2119.fn(_t2119.env, _t2118, 2);
#line 621 "compiler/main.gem"
    GemVal _t2120 = gem_table_new();
    GemVal _t2121 = gem_v_result;
    GemVal _t2122[] = {gem_table_get(_t2121, gem_string("fn_name"))};
    gem_table_set(_t2120, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2122, 1)), gem_string(", NULL)")));
    gem_table_set(_t2120, gem_string("setup"), gem_string(""));
        GemVal _t2123 = _t2120;
        gem_pop_frame();
        return _t2123;
    }
#line 624 "compiler/main.gem"
    GemVal _t2124[] = {gem_v_node, gem_v_captures};
    GemVal _t2125 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t2125.fn(_t2125.env, _t2124, 2);
#line 626 "compiler/main.gem"
    GemVal gem_v_setup = gem_string("");
#line 627 "compiler/main.gem"
    GemVal _t2126 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t2126.fn(_t2126.env, NULL, 0);
#line 628 "compiler/main.gem"
    GemVal _t2127 = gem_v_result;
    GemVal _t2128[] = {gem_table_get(_t2127, gem_string("struct_name"))};
    GemVal _t2129[] = {gem_v_env_tmp};
    GemVal _t2130 = gem_v_result;
    GemVal _t2131[] = {gem_table_get(_t2130, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2128, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t2129, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t2131, 1)), gem_string("));\n")));
#line 629 "compiler/main.gem"
    GemVal gem_v__for_items_36 = gem_v_captures;
#line 629 "compiler/main.gem"
    GemVal gem_v__for_i_36 = gem_int(0);
#line 629 "compiler/main.gem"
    while (1) {
        GemVal _t2132[] = {gem_v__for_items_36};
        if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_len_fn(NULL, _t2132, 1)))) break;
#line 629 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_36, gem_v__for_i_36);
#line 629 "compiler/main.gem"
        gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 630 "compiler/main.gem"
    GemVal _t2133[] = {gem_v_cap};
    GemVal _t2134 = (*gem_v_mangle);
        GemVal gem_v_mc = _t2134.fn(_t2134.env, _t2133, 1);
#line 631 "compiler/main.gem"
    GemVal _t2135[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2135, 2))) {
#line 632 "compiler/main.gem"
    GemVal _t2136[] = {gem_v_env_tmp};
    GemVal _t2137[] = {gem_v_mc};
    GemVal _t2138[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2136, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2137, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2138, 1)), gem_string(";\n")));
        } else {
#line 634 "compiler/main.gem"
    GemVal _t2139[] = {gem_v_env_tmp};
    GemVal _t2140[] = {gem_v_mc};
    GemVal _t2141[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2139, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2140, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t2141, 1)), gem_string(";\n")));
        }
    }

    GemVal _t2142 = gem_table_new();
    GemVal _t2143 = gem_v_result;
    GemVal _t2144[] = {gem_table_get(_t2143, gem_string("fn_name"))};
    GemVal _t2145[] = {gem_v_env_tmp};
    gem_table_set(_t2142, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2144, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2145, 1)), gem_string(")")));
    gem_table_set(_t2142, gem_string("setup"), gem_v_setup);
    GemVal _t2146 = _t2142;
    gem_pop_frame();
    return _t2146;
}

struct _closure__anon_36 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_36(void *_env, GemVal *args, int argc) {
    struct _closure__anon_36 *_cls = (struct _closure__anon_36 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_args = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_36", "compiler/main.gem", 0);
#line 644 "compiler/main.gem"
    GemVal gem_v_arg_setups = gem_string("");
#line 645 "compiler/main.gem"
    GemVal _t2148 = gem_table_new();
    GemVal gem_v_arg_exprs = _t2148;
#line 646 "compiler/main.gem"
    GemVal gem_v__for_i_37 = gem_int(0);
#line 646 "compiler/main.gem"
    GemVal _t2149[] = {gem_v_args};
    GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t2149, 1);
#line 646 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 646 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_37;
#line 646 "compiler/main.gem"
        gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 647 "compiler/main.gem"
    GemVal _t2150[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2151 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2151.fn(_t2151.env, _t2150, 1);
#line 648 "compiler/main.gem"
    GemVal _t2152 = gem_v_r;
        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t2152, gem_string("setup")));
#line 649 "compiler/main.gem"
    GemVal _t2153 = gem_v_r;
    GemVal _t2154[] = {gem_v_arg_exprs, gem_table_get(_t2153, gem_string("expr"))};
        (void)(gem_push_fn(NULL, _t2154, 2));
    }

#line 651 "compiler/main.gem"
    GemVal _t2155[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t2155, 1);
#line 652 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 653 "compiler/main.gem"
    GemVal _t2156 = gem_table_new();
    gem_table_set(_t2156, gem_string("setup"), gem_v_arg_setups);
    gem_table_set(_t2156, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t2156, gem_string("argc"), gem_int(0));
        GemVal _t2157 = _t2156;
        gem_pop_frame();
        return _t2157;
    }
#line 655 "compiler/main.gem"
    GemVal _t2158 = (*gem_v_tmp);
    GemVal gem_v_t = _t2158.fn(_t2158.env, NULL, 0);
#line 656 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 657 "compiler/main.gem"
    GemVal gem_v__for_i_38 = gem_int(1);
#line 657 "compiler/main.gem"
    GemVal gem_v__for_limit_38 = gem_v_argc;
#line 657 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 657 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_38;
#line 657 "compiler/main.gem"
        gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 658 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t2159 = gem_table_new();
    GemVal _t2160[] = {gem_v_t};
    GemVal _t2161[] = {gem_v_arr};
    gem_table_set(_t2159, gem_string("setup"), gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2160, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2161, 1)), gem_string("};\n"))));
    gem_table_set(_t2159, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t2159, gem_string("argc"), gem_v_argc);
    GemVal _t2162 = _t2159;
    gem_pop_frame();
    return _t2162;
}

struct _closure__anon_37 {
};
static GemVal _anon_37(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_37", "compiler/main.gem", 0);
#line 665 "compiler/main.gem"
    GemVal _t2164 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2164, gem_string("argc")), gem_int(0)))) {
#line 666 "compiler/main.gem"
    GemVal _t2165 = gem_table_new();
    GemVal _t2166[] = {gem_v_fn_name};
    GemVal _t2167[] = {gem_v_env};
    gem_table_set(_t2165, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2166, 1), gem_string("(")), gem_to_string_fn(NULL, _t2167, 1)), gem_string(", NULL, 0)")));
    GemVal _t2168 = gem_v_ca;
    gem_table_set(_t2165, gem_string("setup"), gem_table_get(_t2168, gem_string("setup")));
        GemVal _t2169 = _t2165;
        gem_pop_frame();
        return _t2169;
    }
    GemVal _t2170 = gem_table_new();
    GemVal _t2171[] = {gem_v_fn_name};
    GemVal _t2172[] = {gem_v_env};
    GemVal _t2173 = gem_v_ca;
    GemVal _t2174[] = {gem_table_get(_t2173, gem_string("arr_name"))};
    GemVal _t2175 = gem_v_ca;
    GemVal _t2176[] = {gem_table_get(_t2175, gem_string("argc"))};
    gem_table_set(_t2170, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2171, 1), gem_string("(")), gem_to_string_fn(NULL, _t2172, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2174, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2176, 1)), gem_string(")")));
    GemVal _t2177 = gem_v_ca;
    gem_table_set(_t2170, gem_string("setup"), gem_table_get(_t2177, gem_string("setup")));
    GemVal _t2178 = _t2170;
    gem_pop_frame();
    return _t2178;
}

struct _closure__anon_38 {
    GemVal *gem_v_builtin_fns;
    GemVal *gem_v_compile_args;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_emit_direct_call;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tmp;
};
static GemVal _anon_38(void *_env, GemVal *args, int argc) {
    struct _closure__anon_38 *_cls = (struct _closure__anon_38 *)_env;
    GemVal *gem_v_builtin_fns = _cls->gem_v_builtin_fns;
    GemVal *gem_v_compile_args = _cls->gem_v_compile_args;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_emit_direct_call = _cls->gem_v_emit_direct_call;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_38", "compiler/main.gem", 0);
#line 674 "compiler/main.gem"
    GemVal gem_v__d39 = gem_v_node;
#line 674 "compiler/main.gem"
    GemVal _t2179 = gem_v__d39;
    GemVal gem_v_func = gem_table_get(_t2179, gem_string("func"));
#line 674 "compiler/main.gem"
    GemVal _t2180 = gem_v__d39;
    GemVal gem_v_args = gem_table_get(_t2180, gem_string("args"));

#line 677 "compiler/main.gem"
    GemVal _t2181 = gem_v_func;
    GemVal _t2183;
    if (!gem_truthy(gem_eq(gem_table_get(_t2181, gem_string("tag")), gem_string("dot")))) {
        _t2183 = gem_eq(gem_table_get(_t2181, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t2182 = gem_v_func;
        _t2183 = gem_eq(gem_table_get(_t2182, gem_string("field")), gem_string("len"));
    }
    GemVal _t2185;
    if (!gem_truthy(_t2183)) {
        _t2185 = _t2183;
    } else {
        GemVal _t2184[] = {gem_v_args};
        _t2185 = gem_eq(gem_len_fn(NULL, _t2184, 1), gem_int(0));
    }
    if (gem_truthy(_t2185)) {
#line 678 "compiler/main.gem"
    GemVal _t2186 = gem_v_func;
    GemVal _t2187[] = {gem_table_get(_t2186, gem_string("object"))};
    GemVal _t2188 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2188.fn(_t2188.env, _t2187, 1);
#line 679 "compiler/main.gem"
    GemVal _t2189 = gem_table_new();
    GemVal _t2190 = gem_v_obj_r;
    GemVal _t2191[] = {gem_table_get(_t2190, gem_string("expr"))};
    gem_table_set(_t2189, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t2191, 1)), gem_string(")")));
    GemVal _t2192 = gem_v_obj_r;
    gem_table_set(_t2189, gem_string("setup"), gem_table_get(_t2192, gem_string("setup")));
        GemVal _t2193 = _t2189;
        gem_pop_frame();
        return _t2193;
    }
#line 683 "compiler/main.gem"
    GemVal _t2194 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t2194, gem_string("tag")), gem_string("var")))) {
#line 684 "compiler/main.gem"
    GemVal _t2195 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t2195, gem_string("name"));
#line 686 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 687 "compiler/main.gem"
    GemVal _t2196 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t2196, gem_string("line"));
#line 688 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 689 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 691 "compiler/main.gem"
    GemVal _t2197[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t2197, 1);
#line 692 "compiler/main.gem"
    GemVal _t2198[] = {gem_v_args};
    GemVal _t2199 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t2199.fn(_t2199.env, _t2198, 1);
#line 693 "compiler/main.gem"
    GemVal _t2200 = gem_v_ca;
            if (gem_truthy(gem_eq(gem_table_get(_t2200, gem_string("argc")), gem_int(0)))) {
#line 694 "compiler/main.gem"
    GemVal _t2201 = gem_table_new();
    GemVal _t2202[] = {gem_v_escaped_file};
    GemVal _t2203[] = {gem_v_line};
    gem_table_set(_t2201, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2202, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2203, 1)), gem_string(", NULL, 0)")));
    GemVal _t2204 = gem_v_ca;
    gem_table_set(_t2201, gem_string("setup"), gem_table_get(_t2204, gem_string("setup")));
                GemVal _t2205 = _t2201;
                gem_pop_frame();
                return _t2205;
            }
#line 696 "compiler/main.gem"
    GemVal _t2206 = gem_table_new();
    GemVal _t2207[] = {gem_v_escaped_file};
    GemVal _t2208[] = {gem_v_line};
    GemVal _t2209 = gem_v_ca;
    GemVal _t2210[] = {gem_table_get(_t2209, gem_string("arr_name"))};
    GemVal _t2211 = gem_v_ca;
    GemVal _t2212[] = {gem_table_get(_t2211, gem_string("argc"))};
    gem_table_set(_t2206, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2207, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2208, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2210, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2212, 1)), gem_string(")")));
    GemVal _t2213 = gem_v_ca;
    gem_table_set(_t2206, gem_string("setup"), gem_table_get(_t2213, gem_string("setup")));
            GemVal _t2214 = _t2206;
            gem_pop_frame();
            return _t2214;
        } else {
#line 697 "compiler/main.gem"
    GemVal _t2215[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t2215, 2))) {
#line 698 "compiler/main.gem"
    GemVal _t2216[] = {gem_v_args};
    GemVal _t2217 = (*gem_v_compile_args);
    GemVal _t2218[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t2217.fn(_t2217.env, _t2216, 1)};
    GemVal _t2219 = (*gem_v_emit_direct_call);
                GemVal _t2220 = _t2219.fn(_t2219.env, _t2218, 3);
                gem_pop_frame();
                return _t2220;
            } else {
#line 699 "compiler/main.gem"
    GemVal _t2221[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t2221, 2))) {
#line 700 "compiler/main.gem"
    GemVal _t2222[] = {gem_v_name};
    GemVal _t2223[] = {gem_v_args};
    GemVal _t2224 = (*gem_v_compile_args);
    GemVal _t2225[] = {gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t2222, 1)), gem_string("NULL"), _t2224.fn(_t2224.env, _t2223, 1)};
    GemVal _t2226 = (*gem_v_emit_direct_call);
                    GemVal _t2227 = _t2226.fn(_t2226.env, _t2225, 3);
                    gem_pop_frame();
                    return _t2227;
                }
            }
        }
    }
#line 705 "compiler/main.gem"
    GemVal _t2228[] = {gem_v_func};
    GemVal _t2229 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t2229.fn(_t2229.env, _t2228, 1);
#line 706 "compiler/main.gem"
    GemVal _t2230[] = {gem_v_args};
    GemVal _t2231 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t2231.fn(_t2231.env, _t2230, 1);
#line 707 "compiler/main.gem"
    GemVal _t2232 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t2232.fn(_t2232.env, NULL, 0);
#line 708 "compiler/main.gem"
    GemVal _t2233 = gem_v_func_r;
    GemVal _t2234 = gem_v_ca;
    GemVal _t2235[] = {gem_v_tmp_fn};
    GemVal _t2236 = gem_v_func_r;
    GemVal _t2237[] = {gem_table_get(_t2236, gem_string("expr"))};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get(_t2233, gem_string("setup")), gem_table_get(_t2234, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2235, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2237, 1)), gem_string(";\n")));
#line 709 "compiler/main.gem"
    GemVal _t2238 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2238, gem_string("argc")), gem_int(0)))) {
#line 710 "compiler/main.gem"
    GemVal _t2239 = gem_table_new();
    GemVal _t2240[] = {gem_v_tmp_fn};
    GemVal _t2241[] = {gem_v_tmp_fn};
    gem_table_set(_t2239, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2240, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2241, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t2239, gem_string("setup"), gem_v_setup);
        GemVal _t2242 = _t2239;
        gem_pop_frame();
        return _t2242;
    }
    GemVal _t2243 = gem_table_new();
    GemVal _t2244[] = {gem_v_tmp_fn};
    GemVal _t2245[] = {gem_v_tmp_fn};
    GemVal _t2246 = gem_v_ca;
    GemVal _t2247[] = {gem_table_get(_t2246, gem_string("arr_name"))};
    GemVal _t2248 = gem_v_ca;
    GemVal _t2249[] = {gem_table_get(_t2248, gem_string("argc"))};
    gem_table_set(_t2243, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2244, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2245, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t2247, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2249, 1)), gem_string(")")));
    gem_table_set(_t2243, gem_string("setup"), gem_v_setup);
    GemVal _t2250 = _t2243;
    gem_pop_frame();
    return _t2250;
}

struct _closure__anon_39 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_39(void *_env, GemVal *args, int argc) {
    struct _closure__anon_39 *_cls = (struct _closure__anon_39 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_39", "compiler/main.gem", 0);
#line 718 "compiler/main.gem"
    GemVal _t2252 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t2252, gem_string("op"));
#line 719 "compiler/main.gem"
    GemVal _t2253 = gem_v_node;
    GemVal _t2254[] = {gem_table_get(_t2253, gem_string("left"))};
    GemVal _t2255 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t2255.fn(_t2255.env, _t2254, 1);
#line 720 "compiler/main.gem"
    GemVal _t2256 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t2256, gem_string("expr"));
#line 721 "compiler/main.gem"
    GemVal _t2257 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t2257, gem_string("setup"));
#line 723 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 724 "compiler/main.gem"
    GemVal _t2258 = gem_v_node;
    GemVal _t2259[] = {gem_table_get(_t2258, gem_string("right"))};
    GemVal _t2260 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t2260.fn(_t2260.env, _t2259, 1);
#line 725 "compiler/main.gem"
    GemVal _t2261 = gem_table_new();
    GemVal _t2262[] = {gem_v_lc};
    GemVal _t2263 = gem_v_rc_r;
    GemVal _t2264[] = {gem_table_get(_t2263, gem_string("expr"))};
    gem_table_set(_t2261, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2262, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2264, 1)), gem_string(")")));
    GemVal _t2265 = gem_v_rc_r;
    gem_table_set(_t2261, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2265, gem_string("setup"))));
        GemVal _t2266 = _t2261;
        gem_pop_frame();
        return _t2266;
    } else {
#line 726 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 727 "compiler/main.gem"
    GemVal _t2267 = gem_v_node;
    GemVal _t2268[] = {gem_table_get(_t2267, gem_string("right"))};
    GemVal _t2269 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t2269.fn(_t2269.env, _t2268, 1);
#line 728 "compiler/main.gem"
    GemVal _t2270 = gem_table_new();
    GemVal _t2271[] = {gem_v_lc};
    GemVal _t2272 = gem_v_rc_r;
    GemVal _t2273[] = {gem_table_get(_t2272, gem_string("expr"))};
    gem_table_set(_t2270, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t2271, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2273, 1)), gem_string(")")));
    GemVal _t2274 = gem_v_rc_r;
    gem_table_set(_t2270, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2274, gem_string("setup"))));
            GemVal _t2275 = _t2270;
            gem_pop_frame();
            return _t2275;
        } else {
#line 729 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 730 "compiler/main.gem"
    GemVal _t2276 = gem_v_node;
    GemVal _t2277[] = {gem_table_get(_t2276, gem_string("right"))};
    GemVal _t2278 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t2278.fn(_t2278.env, _t2277, 1);
#line 731 "compiler/main.gem"
    GemVal _t2279 = gem_table_new();
    GemVal _t2280[] = {gem_v_lc};
    GemVal _t2281 = gem_v_rc_r;
    GemVal _t2282[] = {gem_table_get(_t2281, gem_string("expr"))};
    gem_table_set(_t2279, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t2280, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2282, 1)), gem_string(")")));
    GemVal _t2283 = gem_v_rc_r;
    gem_table_set(_t2279, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2283, gem_string("setup"))));
                GemVal _t2284 = _t2279;
                gem_pop_frame();
                return _t2284;
            } else {
#line 732 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 733 "compiler/main.gem"
    GemVal _t2285 = gem_v_node;
    GemVal _t2286[] = {gem_table_get(_t2285, gem_string("right"))};
    GemVal _t2287 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t2287.fn(_t2287.env, _t2286, 1);
#line 734 "compiler/main.gem"
    GemVal _t2288 = gem_table_new();
    GemVal _t2289[] = {gem_v_lc};
    GemVal _t2290 = gem_v_rc_r;
    GemVal _t2291[] = {gem_table_get(_t2290, gem_string("expr"))};
    gem_table_set(_t2288, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t2289, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2291, 1)), gem_string(")")));
    GemVal _t2292 = gem_v_rc_r;
    gem_table_set(_t2288, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2292, gem_string("setup"))));
                    GemVal _t2293 = _t2288;
                    gem_pop_frame();
                    return _t2293;
                } else {
#line 735 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 736 "compiler/main.gem"
    GemVal _t2294 = gem_v_node;
    GemVal _t2295[] = {gem_table_get(_t2294, gem_string("right"))};
    GemVal _t2296 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t2296.fn(_t2296.env, _t2295, 1);
#line 737 "compiler/main.gem"
    GemVal _t2297 = gem_table_new();
    GemVal _t2298[] = {gem_v_lc};
    GemVal _t2299 = gem_v_rc_r;
    GemVal _t2300[] = {gem_table_get(_t2299, gem_string("expr"))};
    gem_table_set(_t2297, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t2298, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2300, 1)), gem_string(")")));
    GemVal _t2301 = gem_v_rc_r;
    gem_table_set(_t2297, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2301, gem_string("setup"))));
                        GemVal _t2302 = _t2297;
                        gem_pop_frame();
                        return _t2302;
                    } else {
#line 738 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 739 "compiler/main.gem"
    GemVal _t2303 = gem_v_node;
    GemVal _t2304[] = {gem_table_get(_t2303, gem_string("right"))};
    GemVal _t2305 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t2305.fn(_t2305.env, _t2304, 1);
#line 740 "compiler/main.gem"
    GemVal _t2306 = (*gem_v_tmp);
                            GemVal gem_v_t = _t2306.fn(_t2306.env, NULL, 0);
#line 741 "compiler/main.gem"
    GemVal _t2307 = gem_v_rc_r;
    GemVal _t2308[] = {gem_v_t};
    GemVal _t2309 = gem_v_rc_r;
    GemVal _t2310[] = {gem_table_get(_t2309, gem_string("expr"))};
    GemVal _t2311[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t2307, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2308, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2310, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2311, 1)), gem_string("};\n")));
#line 742 "compiler/main.gem"
    GemVal _t2312 = gem_table_new();
    GemVal _t2313[] = {gem_v_t};
    gem_table_set(_t2312, gem_string("expr"), gem_add(gem_add(gem_string("gem_has_key_fn(NULL, "), gem_to_string_fn(NULL, _t2313, 1)), gem_string(", 2)")));
    gem_table_set(_t2312, gem_string("setup"), gem_v_setup);
                            GemVal _t2314 = _t2312;
                            gem_pop_frame();
                            return _t2314;
                        } else {
#line 743 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 744 "compiler/main.gem"
    GemVal _t2315 = gem_v_node;
    GemVal _t2316[] = {gem_table_get(_t2315, gem_string("right"))};
    GemVal _t2317 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t2317.fn(_t2317.env, _t2316, 1);
#line 745 "compiler/main.gem"
    GemVal _t2318 = gem_table_new();
    GemVal _t2319[] = {gem_v_lc};
    GemVal _t2320 = gem_v_rc_r;
    GemVal _t2321[] = {gem_table_get(_t2320, gem_string("expr"))};
    gem_table_set(_t2318, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t2319, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2321, 1)), gem_string(")")));
    GemVal _t2322 = gem_v_rc_r;
    gem_table_set(_t2318, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2322, gem_string("setup"))));
                                GemVal _t2323 = _t2318;
                                gem_pop_frame();
                                return _t2323;
                            } else {
#line 746 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 747 "compiler/main.gem"
    GemVal _t2324 = gem_v_node;
    GemVal _t2325[] = {gem_table_get(_t2324, gem_string("right"))};
    GemVal _t2326 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t2326.fn(_t2326.env, _t2325, 1);
#line 748 "compiler/main.gem"
    GemVal _t2327 = gem_table_new();
    GemVal _t2328[] = {gem_v_lc};
    GemVal _t2329 = gem_v_rc_r;
    GemVal _t2330[] = {gem_table_get(_t2329, gem_string("expr"))};
    gem_table_set(_t2327, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t2328, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2330, 1)), gem_string(")")));
    GemVal _t2331 = gem_v_rc_r;
    gem_table_set(_t2327, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2331, gem_string("setup"))));
                                    GemVal _t2332 = _t2327;
                                    gem_pop_frame();
                                    return _t2332;
                                } else {
#line 749 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 750 "compiler/main.gem"
    GemVal _t2333 = gem_v_node;
    GemVal _t2334[] = {gem_table_get(_t2333, gem_string("right"))};
    GemVal _t2335 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t2335.fn(_t2335.env, _t2334, 1);
#line 751 "compiler/main.gem"
    GemVal _t2336 = gem_table_new();
    GemVal _t2337[] = {gem_v_lc};
    GemVal _t2338 = gem_v_rc_r;
    GemVal _t2339[] = {gem_table_get(_t2338, gem_string("expr"))};
    gem_table_set(_t2336, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t2337, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2339, 1)), gem_string(")")));
    GemVal _t2340 = gem_v_rc_r;
    gem_table_set(_t2336, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2340, gem_string("setup"))));
                                        GemVal _t2341 = _t2336;
                                        gem_pop_frame();
                                        return _t2341;
                                    } else {
#line 752 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 753 "compiler/main.gem"
    GemVal _t2342 = gem_v_node;
    GemVal _t2343[] = {gem_table_get(_t2342, gem_string("right"))};
    GemVal _t2344 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t2344.fn(_t2344.env, _t2343, 1);
#line 754 "compiler/main.gem"
    GemVal _t2345 = gem_table_new();
    GemVal _t2346[] = {gem_v_lc};
    GemVal _t2347 = gem_v_rc_r;
    GemVal _t2348[] = {gem_table_get(_t2347, gem_string("expr"))};
    gem_table_set(_t2345, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t2346, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2348, 1)), gem_string(")")));
    GemVal _t2349 = gem_v_rc_r;
    gem_table_set(_t2345, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2349, gem_string("setup"))));
                                            GemVal _t2350 = _t2345;
                                            gem_pop_frame();
                                            return _t2350;
                                        } else {
#line 755 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 756 "compiler/main.gem"
    GemVal _t2351 = gem_v_node;
    GemVal _t2352[] = {gem_table_get(_t2351, gem_string("right"))};
    GemVal _t2353 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t2353.fn(_t2353.env, _t2352, 1);
#line 757 "compiler/main.gem"
    GemVal _t2354 = gem_table_new();
    GemVal _t2355[] = {gem_v_lc};
    GemVal _t2356 = gem_v_rc_r;
    GemVal _t2357[] = {gem_table_get(_t2356, gem_string("expr"))};
    gem_table_set(_t2354, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t2355, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2357, 1)), gem_string(")")));
    GemVal _t2358 = gem_v_rc_r;
    gem_table_set(_t2354, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2358, gem_string("setup"))));
                                                GemVal _t2359 = _t2354;
                                                gem_pop_frame();
                                                return _t2359;
                                            } else {
#line 758 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 759 "compiler/main.gem"
    GemVal _t2360 = gem_v_node;
    GemVal _t2361[] = {gem_table_get(_t2360, gem_string("right"))};
    GemVal _t2362 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t2362.fn(_t2362.env, _t2361, 1);
#line 760 "compiler/main.gem"
    GemVal _t2363 = gem_table_new();
    GemVal _t2364[] = {gem_v_lc};
    GemVal _t2365 = gem_v_rc_r;
    GemVal _t2366[] = {gem_table_get(_t2365, gem_string("expr"))};
    gem_table_set(_t2363, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t2364, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2366, 1)), gem_string(")")));
    GemVal _t2367 = gem_v_rc_r;
    gem_table_set(_t2363, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2367, gem_string("setup"))));
                                                    GemVal _t2368 = _t2363;
                                                    gem_pop_frame();
                                                    return _t2368;
                                                } else {
#line 761 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 762 "compiler/main.gem"
    GemVal _t2369 = gem_v_node;
    GemVal _t2370[] = {gem_table_get(_t2369, gem_string("right"))};
    GemVal _t2371 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t2371.fn(_t2371.env, _t2370, 1);
#line 763 "compiler/main.gem"
    GemVal _t2372 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t2372.fn(_t2372.env, NULL, 0);
#line 764 "compiler/main.gem"
                                                        GemVal gem_v_setup = gem_v_ls;
#line 765 "compiler/main.gem"
    GemVal _t2373[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2373, 1)), gem_string(";\n")));
#line 766 "compiler/main.gem"
    GemVal _t2374[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t2374, 1)), gem_string(")) {\n")));
#line 767 "compiler/main.gem"
    GemVal _t2375[] = {gem_v_t};
    GemVal _t2376[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2375, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2376, 1)), gem_string(";\n")));
#line 768 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 769 "compiler/main.gem"
    GemVal _t2377 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t2377, gem_string("setup")), gem_string("")))) {
#line 770 "compiler/main.gem"
    GemVal _t2378 = gem_v_rc_r;
    GemVal _t2379[] = {gem_table_get(_t2378, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t2379, 3));
                                                        }
#line 772 "compiler/main.gem"
    GemVal _t2380[] = {gem_v_t};
    GemVal _t2381 = gem_v_rc_r;
    GemVal _t2382[] = {gem_table_get(_t2381, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2380, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2382, 1)), gem_string(";\n")));
#line 773 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 774 "compiler/main.gem"
    GemVal _t2383 = gem_table_new();
    gem_table_set(_t2383, gem_string("expr"), gem_v_t);
    gem_table_set(_t2383, gem_string("setup"), gem_v_setup);
                                                        GemVal _t2384 = _t2383;
                                                        gem_pop_frame();
                                                        return _t2384;
                                                    } else {
#line 775 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 776 "compiler/main.gem"
    GemVal _t2385 = gem_v_node;
    GemVal _t2386[] = {gem_table_get(_t2385, gem_string("right"))};
    GemVal _t2387 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t2387.fn(_t2387.env, _t2386, 1);
#line 777 "compiler/main.gem"
    GemVal _t2388 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t2388.fn(_t2388.env, NULL, 0);
#line 778 "compiler/main.gem"
                                                            GemVal gem_v_setup = gem_v_ls;
#line 779 "compiler/main.gem"
    GemVal _t2389[] = {gem_v_t};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2389, 1)), gem_string(";\n")));
#line 780 "compiler/main.gem"
    GemVal _t2390[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t2390, 1)), gem_string(")) {\n")));
#line 781 "compiler/main.gem"
    GemVal _t2391[] = {gem_v_t};
    GemVal _t2392[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2391, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2392, 1)), gem_string(";\n")));
#line 782 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 783 "compiler/main.gem"
    GemVal _t2393 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t2393, gem_string("setup")), gem_string("")))) {
#line 784 "compiler/main.gem"
    GemVal _t2394 = gem_v_rc_r;
    GemVal _t2395[] = {gem_table_get(_t2394, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                                gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t2395, 3));
                                                            }
#line 786 "compiler/main.gem"
    GemVal _t2396[] = {gem_v_t};
    GemVal _t2397 = gem_v_rc_r;
    GemVal _t2398[] = {gem_table_get(_t2397, gem_string("expr"))};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2396, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2398, 1)), gem_string(";\n")));
#line 787 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 788 "compiler/main.gem"
    GemVal _t2399 = gem_table_new();
    gem_table_set(_t2399, gem_string("expr"), gem_v_t);
    gem_table_set(_t2399, gem_string("setup"), gem_v_setup);
                                                            GemVal _t2400 = _t2399;
                                                            gem_pop_frame();
                                                            return _t2400;
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
#line 791 "compiler/main.gem"
    GemVal _t2401[] = {gem_v_op};
    GemVal _t2402[] = {gem_add(gem_string("unknown binary operator: "), gem_to_string_fn(NULL, _t2401, 1))};
    GemVal _t2403 = gem_error_at_fn("compiler/main.gem", 791, _t2402, 1);
    gem_pop_frame();
    return _t2403;
}

struct _closure__anon_40 {
    GemVal *gem_v_compile_stmt;
};
static GemVal _anon_40(void *_env, GemVal *args, int argc) {
    struct _closure__anon_40 *_cls = (struct _closure__anon_40 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_40", "compiler/main.gem", 0);
#line 797 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 798 "compiler/main.gem"
    GemVal gem_v__for_items_40 = gem_v_stmts;
#line 798 "compiler/main.gem"
    GemVal gem_v__for_i_40 = gem_int(0);
#line 798 "compiler/main.gem"
    while (1) {
        GemVal _t2405[] = {gem_v__for_items_40};
        if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_len_fn(NULL, _t2405, 1)))) break;
#line 798 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_40, gem_v__for_i_40);
#line 798 "compiler/main.gem"
        gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 799 "compiler/main.gem"
    GemVal _t2406[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2407 = (*gem_v_compile_stmt);
        gem_v_out = gem_add(gem_add(gem_v_out, _t2407.fn(_t2407.env, _t2406, 2)), gem_string("\n"));
    }

    GemVal _t2408 = gem_v_out;
    gem_pop_frame();
    return _t2408;
}

struct _closure__anon_41 {
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
static GemVal _anon_41(void *_env, GemVal *args, int argc) {
    struct _closure__anon_41 *_cls = (struct _closure__anon_41 *)_env;
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
    gem_push_frame("_anon_41", "compiler/main.gem", 0);
#line 807 "compiler/main.gem"
    GemVal _t2410[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2410, 1);
#line 808 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 809 "compiler/main.gem"
    GemVal _t2411 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2411, gem_string("line")), GEM_NIL))) {
#line 810 "compiler/main.gem"
    GemVal _t2412 = gem_v_node;
    GemVal _t2413[] = {gem_table_get(_t2412, gem_string("line"))};
    GemVal _t2414[] = {(*gem_v_source_name)};
    GemVal _t2415[] = {gem_fn_escape_c_string(NULL, _t2414, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2413, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2415, 1)), gem_string("\"\n"));
    }
#line 813 "compiler/main.gem"
    GemVal gem_v__match_41 = gem_v_node;
    GemVal _t2416[] = {gem_v__match_41};
    GemVal _t2418;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2416, 1), gem_string("table")))) {
        _t2418 = gem_eq(gem_type_fn(NULL, _t2416, 1), gem_string("table"));
    } else {
        GemVal _t2417[] = {gem_v__match_41, gem_string("tag")};
        _t2418 = gem_has_key_fn(NULL, _t2417, 2);
    }
    GemVal _t2419;
    if (!gem_truthy(_t2418)) {
        _t2419 = _t2418;
    } else {
        _t2419 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2421;
    if (!gem_truthy(_t2419)) {
        _t2421 = _t2419;
    } else {
        GemVal _t2420[] = {gem_v__match_41, gem_string("name")};
        _t2421 = gem_has_key_fn(NULL, _t2420, 2);
    }
    GemVal _t2423;
    if (!gem_truthy(_t2421)) {
        _t2423 = _t2421;
    } else {
        GemVal _t2422[] = {gem_v__match_41, gem_string("value")};
        _t2423 = gem_has_key_fn(NULL, _t2422, 2);
    }
    if (gem_truthy(_t2423)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_41, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 815 "compiler/main.gem"
    GemVal _t2424[] = {gem_v_value};
    GemVal _t2425 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2425.fn(_t2425.env, _t2424, 1);
#line 816 "compiler/main.gem"
    GemVal _t2426[] = {gem_v_name};
    GemVal _t2427 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2427.fn(_t2427.env, _t2426, 1);
#line 817 "compiler/main.gem"
    GemVal _t2429;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t2429 = (*gem_v_in_top_level);
    } else {
        GemVal _t2428[] = {(*gem_v_top_level_vars), gem_v_name};
        _t2429 = gem_fn_set_contains(NULL, _t2428, 2);
    }
        if (gem_truthy(_t2429)) {
#line 818 "compiler/main.gem"
    GemVal _t2430[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2430, 2))) {
#line 819 "compiler/main.gem"
    GemVal _t2431[] = {gem_v_line_dir};
    GemVal _t2432 = gem_v_r;
    GemVal _t2433[] = {gem_table_get(_t2432, gem_string("setup"))};
    GemVal _t2434[] = {gem_v_p};
    GemVal _t2435[] = {gem_v_mname};
    GemVal _t2436[] = {gem_v_p};
    GemVal _t2437[] = {gem_v_mname};
    GemVal _t2438 = gem_v_r;
    GemVal _t2439[] = {gem_table_get(_t2438, gem_string("expr"))};
                GemVal _t2440 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2431, 1), gem_to_string_fn(NULL, _t2433, 1)), gem_to_string_fn(NULL, _t2434, 1)), gem_to_string_fn(NULL, _t2435, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2436, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2437, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2439, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2440;
            }
#line 821 "compiler/main.gem"
    GemVal _t2441[] = {gem_v_line_dir};
    GemVal _t2442 = gem_v_r;
    GemVal _t2443[] = {gem_table_get(_t2442, gem_string("setup"))};
    GemVal _t2444[] = {gem_v_p};
    GemVal _t2445[] = {gem_v_mname};
    GemVal _t2446 = gem_v_r;
    GemVal _t2447[] = {gem_table_get(_t2446, gem_string("expr"))};
            GemVal _t2448 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2441, 1), gem_to_string_fn(NULL, _t2443, 1)), gem_to_string_fn(NULL, _t2444, 1)), gem_to_string_fn(NULL, _t2445, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2447, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2448;
        }
#line 823 "compiler/main.gem"
    GemVal _t2449[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2449, 2))) {
#line 824 "compiler/main.gem"
    GemVal _t2450[] = {gem_v_line_dir};
    GemVal _t2451 = gem_v_r;
    GemVal _t2452[] = {gem_table_get(_t2451, gem_string("setup"))};
    GemVal _t2453[] = {gem_v_p};
    GemVal _t2454[] = {gem_v_mname};
    GemVal _t2455[] = {gem_v_p};
    GemVal _t2456[] = {gem_v_mname};
    GemVal _t2457 = gem_v_r;
    GemVal _t2458[] = {gem_table_get(_t2457, gem_string("expr"))};
            GemVal _t2459 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2450, 1), gem_to_string_fn(NULL, _t2452, 1)), gem_to_string_fn(NULL, _t2453, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2454, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2455, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2456, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2458, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2459;
        }
#line 826 "compiler/main.gem"
    GemVal _t2460[] = {gem_v_line_dir};
    GemVal _t2461 = gem_v_r;
    GemVal _t2462[] = {gem_table_get(_t2461, gem_string("setup"))};
    GemVal _t2463[] = {gem_v_p};
    GemVal _t2464[] = {gem_v_mname};
    GemVal _t2465 = gem_v_r;
    GemVal _t2466[] = {gem_table_get(_t2465, gem_string("expr"))};
        GemVal _t2467 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2460, 1), gem_to_string_fn(NULL, _t2462, 1)), gem_to_string_fn(NULL, _t2463, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2464, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2466, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2467;
    } else {
    GemVal _t2468[] = {gem_v__match_41};
    GemVal _t2470;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2468, 1), gem_string("table")))) {
        _t2470 = gem_eq(gem_type_fn(NULL, _t2468, 1), gem_string("table"));
    } else {
        GemVal _t2469[] = {gem_v__match_41, gem_string("tag")};
        _t2470 = gem_has_key_fn(NULL, _t2469, 2);
    }
    GemVal _t2471;
    if (!gem_truthy(_t2470)) {
        _t2471 = _t2470;
    } else {
        _t2471 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2473;
    if (!gem_truthy(_t2471)) {
        _t2473 = _t2471;
    } else {
        GemVal _t2472[] = {gem_v__match_41, gem_string("name")};
        _t2473 = gem_has_key_fn(NULL, _t2472, 2);
    }
    GemVal _t2475;
    if (!gem_truthy(_t2473)) {
        _t2475 = _t2473;
    } else {
        GemVal _t2474[] = {gem_v__match_41, gem_string("value")};
        _t2475 = gem_has_key_fn(NULL, _t2474, 2);
    }
    if (gem_truthy(_t2475)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_41, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 828 "compiler/main.gem"
    GemVal _t2476[] = {gem_v_value};
    GemVal _t2477 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2477.fn(_t2477.env, _t2476, 1);
#line 829 "compiler/main.gem"
    GemVal _t2478[] = {gem_v_name};
    GemVal _t2479 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2479.fn(_t2479.env, _t2478, 1);
#line 830 "compiler/main.gem"
    GemVal _t2480[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2482;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2480, 2))) {
        _t2482 = gem_fn_set_contains(NULL, _t2480, 2);
    } else {
        GemVal _t2481[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2482 = gem_fn_set_contains(NULL, _t2481, 2);
    }
        if (gem_truthy(_t2482)) {
#line 831 "compiler/main.gem"
    GemVal _t2483[] = {gem_v_line_dir};
    GemVal _t2484 = gem_v_r;
    GemVal _t2485[] = {gem_table_get(_t2484, gem_string("setup"))};
    GemVal _t2486[] = {gem_v_p};
    GemVal _t2487[] = {gem_v_mname};
    GemVal _t2488 = gem_v_r;
    GemVal _t2489[] = {gem_table_get(_t2488, gem_string("expr"))};
            GemVal _t2490 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2483, 1), gem_to_string_fn(NULL, _t2485, 1)), gem_to_string_fn(NULL, _t2486, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2487, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2489, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2490;
        }
#line 833 "compiler/main.gem"
    GemVal _t2491[] = {gem_v_line_dir};
    GemVal _t2492 = gem_v_r;
    GemVal _t2493[] = {gem_table_get(_t2492, gem_string("setup"))};
    GemVal _t2494[] = {gem_v_p};
    GemVal _t2495[] = {gem_v_mname};
    GemVal _t2496 = gem_v_r;
    GemVal _t2497[] = {gem_table_get(_t2496, gem_string("expr"))};
        GemVal _t2498 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2491, 1), gem_to_string_fn(NULL, _t2493, 1)), gem_to_string_fn(NULL, _t2494, 1)), gem_to_string_fn(NULL, _t2495, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2497, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2498;
    } else {
    GemVal _t2499[] = {gem_v__match_41};
    GemVal _t2501;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2499, 1), gem_string("table")))) {
        _t2501 = gem_eq(gem_type_fn(NULL, _t2499, 1), gem_string("table"));
    } else {
        GemVal _t2500[] = {gem_v__match_41, gem_string("tag")};
        _t2501 = gem_has_key_fn(NULL, _t2500, 2);
    }
    GemVal _t2502;
    if (!gem_truthy(_t2501)) {
        _t2502 = _t2501;
    } else {
        _t2502 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t2504;
    if (!gem_truthy(_t2502)) {
        _t2504 = _t2502;
    } else {
        GemVal _t2503[] = {gem_v__match_41, gem_string("object")};
        _t2504 = gem_has_key_fn(NULL, _t2503, 2);
    }
    GemVal _t2506;
    if (!gem_truthy(_t2504)) {
        _t2506 = _t2504;
    } else {
        GemVal _t2505[] = {gem_v__match_41, gem_string("value")};
        _t2506 = gem_has_key_fn(NULL, _t2505, 2);
    }
    GemVal _t2508;
    if (!gem_truthy(_t2506)) {
        _t2508 = _t2506;
    } else {
        GemVal _t2507[] = {gem_v__match_41, gem_string("field")};
        _t2508 = gem_has_key_fn(NULL, _t2507, 2);
    }
    if (gem_truthy(_t2508)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_41, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_41, gem_string("field"));
#line 835 "compiler/main.gem"
    GemVal _t2509[] = {gem_v_object};
    GemVal _t2510 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2510.fn(_t2510.env, _t2509, 1);
#line 836 "compiler/main.gem"
    GemVal _t2511[] = {gem_v_value};
    GemVal _t2512 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2512.fn(_t2512.env, _t2511, 1);
#line 837 "compiler/main.gem"
    GemVal _t2513[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2513, 1);
#line 838 "compiler/main.gem"
    GemVal _t2514[] = {gem_v_line_dir};
    GemVal _t2515 = gem_v_obj_r;
    GemVal _t2516[] = {gem_table_get(_t2515, gem_string("setup"))};
    GemVal _t2517 = gem_v_val_r;
    GemVal _t2518[] = {gem_table_get(_t2517, gem_string("setup"))};
    GemVal _t2519[] = {gem_v_p};
    GemVal _t2520 = gem_v_obj_r;
    GemVal _t2521[] = {gem_table_get(_t2520, gem_string("expr"))};
    GemVal _t2522[] = {gem_v_escaped};
    GemVal _t2523 = gem_v_val_r;
    GemVal _t2524[] = {gem_table_get(_t2523, gem_string("expr"))};
        GemVal _t2525 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2514, 1), gem_to_string_fn(NULL, _t2516, 1)), gem_to_string_fn(NULL, _t2518, 1)), gem_to_string_fn(NULL, _t2519, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2521, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2522, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2524, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2525;
    } else {
    GemVal _t2526[] = {gem_v__match_41};
    GemVal _t2528;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2526, 1), gem_string("table")))) {
        _t2528 = gem_eq(gem_type_fn(NULL, _t2526, 1), gem_string("table"));
    } else {
        GemVal _t2527[] = {gem_v__match_41, gem_string("tag")};
        _t2528 = gem_has_key_fn(NULL, _t2527, 2);
    }
    GemVal _t2529;
    if (!gem_truthy(_t2528)) {
        _t2529 = _t2528;
    } else {
        _t2529 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t2531;
    if (!gem_truthy(_t2529)) {
        _t2531 = _t2529;
    } else {
        GemVal _t2530[] = {gem_v__match_41, gem_string("object")};
        _t2531 = gem_has_key_fn(NULL, _t2530, 2);
    }
    GemVal _t2533;
    if (!gem_truthy(_t2531)) {
        _t2533 = _t2531;
    } else {
        GemVal _t2532[] = {gem_v__match_41, gem_string("key")};
        _t2533 = gem_has_key_fn(NULL, _t2532, 2);
    }
    GemVal _t2535;
    if (!gem_truthy(_t2533)) {
        _t2535 = _t2533;
    } else {
        GemVal _t2534[] = {gem_v__match_41, gem_string("value")};
        _t2535 = gem_has_key_fn(NULL, _t2534, 2);
    }
    if (gem_truthy(_t2535)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_41, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_41, gem_string("key"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 840 "compiler/main.gem"
    GemVal _t2536[] = {gem_v_object};
    GemVal _t2537 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2537.fn(_t2537.env, _t2536, 1);
#line 841 "compiler/main.gem"
    GemVal _t2538[] = {gem_v_key};
    GemVal _t2539 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2539.fn(_t2539.env, _t2538, 1);
#line 842 "compiler/main.gem"
    GemVal _t2540[] = {gem_v_value};
    GemVal _t2541 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2541.fn(_t2541.env, _t2540, 1);
#line 843 "compiler/main.gem"
    GemVal _t2542[] = {gem_v_line_dir};
    GemVal _t2543 = gem_v_obj_r;
    GemVal _t2544[] = {gem_table_get(_t2543, gem_string("setup"))};
    GemVal _t2545 = gem_v_key_r;
    GemVal _t2546[] = {gem_table_get(_t2545, gem_string("setup"))};
    GemVal _t2547 = gem_v_val_r;
    GemVal _t2548[] = {gem_table_get(_t2547, gem_string("setup"))};
    GemVal _t2549[] = {gem_v_p};
    GemVal _t2550 = gem_v_obj_r;
    GemVal _t2551[] = {gem_table_get(_t2550, gem_string("expr"))};
    GemVal _t2552 = gem_v_key_r;
    GemVal _t2553[] = {gem_table_get(_t2552, gem_string("expr"))};
    GemVal _t2554 = gem_v_val_r;
    GemVal _t2555[] = {gem_table_get(_t2554, gem_string("expr"))};
        GemVal _t2556 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2542, 1), gem_to_string_fn(NULL, _t2544, 1)), gem_to_string_fn(NULL, _t2546, 1)), gem_to_string_fn(NULL, _t2548, 1)), gem_to_string_fn(NULL, _t2549, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2551, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2553, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2555, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2556;
    } else {
    GemVal _t2557[] = {gem_v__match_41};
    GemVal _t2559;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2557, 1), gem_string("table")))) {
        _t2559 = gem_eq(gem_type_fn(NULL, _t2557, 1), gem_string("table"));
    } else {
        GemVal _t2558[] = {gem_v__match_41, gem_string("tag")};
        _t2559 = gem_has_key_fn(NULL, _t2558, 2);
    }
    GemVal _t2560;
    if (!gem_truthy(_t2559)) {
        _t2560 = _t2559;
    } else {
        _t2560 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t2560)) {
#line 845 "compiler/main.gem"
    GemVal _t2561[] = {gem_v_line_dir};
    GemVal _t2562[] = {gem_v_node, gem_v_indent};
    GemVal _t2563 = (*gem_v_compile_if);
        GemVal _t2564 = gem_add(gem_to_string_fn(NULL, _t2561, 1), _t2563.fn(_t2563.env, _t2562, 2));
        gem_pop_frame();
        return _t2564;
    } else {
    GemVal _t2565[] = {gem_v__match_41};
    GemVal _t2567;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2565, 1), gem_string("table")))) {
        _t2567 = gem_eq(gem_type_fn(NULL, _t2565, 1), gem_string("table"));
    } else {
        GemVal _t2566[] = {gem_v__match_41, gem_string("tag")};
        _t2567 = gem_has_key_fn(NULL, _t2566, 2);
    }
    GemVal _t2568;
    if (!gem_truthy(_t2567)) {
        _t2568 = _t2567;
    } else {
        _t2568 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t2568)) {
#line 847 "compiler/main.gem"
    GemVal _t2569[] = {gem_v_line_dir};
    GemVal _t2570[] = {gem_v_node, gem_v_indent};
    GemVal _t2571 = (*gem_v_compile_while);
        GemVal _t2572 = gem_add(gem_to_string_fn(NULL, _t2569, 1), _t2571.fn(_t2571.env, _t2570, 2));
        gem_pop_frame();
        return _t2572;
    } else {
    GemVal _t2573[] = {gem_v__match_41};
    GemVal _t2575;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2573, 1), gem_string("table")))) {
        _t2575 = gem_eq(gem_type_fn(NULL, _t2573, 1), gem_string("table"));
    } else {
        GemVal _t2574[] = {gem_v__match_41, gem_string("tag")};
        _t2575 = gem_has_key_fn(NULL, _t2574, 2);
    }
    GemVal _t2576;
    if (!gem_truthy(_t2575)) {
        _t2576 = _t2575;
    } else {
        _t2576 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t2576)) {
#line 849 "compiler/main.gem"
    GemVal _t2577[] = {gem_v_line_dir};
    GemVal _t2578[] = {gem_v_node, gem_v_indent};
    GemVal _t2579 = (*gem_v_compile_match);
        GemVal _t2580 = gem_add(gem_to_string_fn(NULL, _t2577, 1), _t2579.fn(_t2579.env, _t2578, 2));
        gem_pop_frame();
        return _t2580;
    } else {
    GemVal _t2581[] = {gem_v__match_41};
    GemVal _t2583;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2581, 1), gem_string("table")))) {
        _t2583 = gem_eq(gem_type_fn(NULL, _t2581, 1), gem_string("table"));
    } else {
        GemVal _t2582[] = {gem_v__match_41, gem_string("tag")};
        _t2583 = gem_has_key_fn(NULL, _t2582, 2);
    }
    GemVal _t2584;
    if (!gem_truthy(_t2583)) {
        _t2584 = _t2583;
    } else {
        _t2584 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t2584)) {
#line 851 "compiler/main.gem"
        GemVal _t2585 = gem_add(gem_v_p, gem_string("break;"));
        gem_pop_frame();
        return _t2585;
    } else {
    GemVal _t2586[] = {gem_v__match_41};
    GemVal _t2588;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2586, 1), gem_string("table")))) {
        _t2588 = gem_eq(gem_type_fn(NULL, _t2586, 1), gem_string("table"));
    } else {
        GemVal _t2587[] = {gem_v__match_41, gem_string("tag")};
        _t2588 = gem_has_key_fn(NULL, _t2587, 2);
    }
    GemVal _t2589;
    if (!gem_truthy(_t2588)) {
        _t2589 = _t2588;
    } else {
        _t2589 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t2589)) {
#line 853 "compiler/main.gem"
        GemVal _t2590 = gem_add(gem_v_p, gem_string("continue;"));
        gem_pop_frame();
        return _t2590;
    } else {
    GemVal _t2591[] = {gem_v__match_41};
    GemVal _t2593;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2591, 1), gem_string("table")))) {
        _t2593 = gem_eq(gem_type_fn(NULL, _t2591, 1), gem_string("table"));
    } else {
        GemVal _t2592[] = {gem_v__match_41, gem_string("tag")};
        _t2593 = gem_has_key_fn(NULL, _t2592, 2);
    }
    GemVal _t2594;
    if (!gem_truthy(_t2593)) {
        _t2594 = _t2593;
    } else {
        _t2594 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2596;
    if (!gem_truthy(_t2594)) {
        _t2596 = _t2594;
    } else {
        GemVal _t2595[] = {gem_v__match_41, gem_string("value")};
        _t2596 = gem_has_key_fn(NULL, _t2595, 2);
    }
    if (gem_truthy(_t2596)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 855 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 856 "compiler/main.gem"
    GemVal _t2597[] = {gem_v_value};
    GemVal _t2598 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2598.fn(_t2598.env, _t2597, 1);
#line 857 "compiler/main.gem"
    GemVal _t2599 = (*gem_v_tmp);
            GemVal gem_v_t = _t2599.fn(_t2599.env, NULL, 0);
#line 858 "compiler/main.gem"
    GemVal _t2600[] = {gem_v_line_dir};
    GemVal _t2601 = gem_v_r;
    GemVal _t2602[] = {gem_table_get(_t2601, gem_string("setup"))};
    GemVal _t2603[] = {gem_v_p};
    GemVal _t2604[] = {gem_v_t};
    GemVal _t2605 = gem_v_r;
    GemVal _t2606[] = {gem_table_get(_t2605, gem_string("expr"))};
    GemVal _t2607[] = {gem_v_p};
    GemVal _t2608[] = {gem_v_p};
    GemVal _t2609[] = {gem_v_t};
            GemVal _t2610 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2600, 1), gem_to_string_fn(NULL, _t2602, 1)), gem_to_string_fn(NULL, _t2603, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2604, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2606, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2607, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2608, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2609, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2610;
        }
#line 860 "compiler/main.gem"
    GemVal _t2611[] = {gem_v_line_dir};
    GemVal _t2612[] = {gem_v_p};
    GemVal _t2613[] = {gem_v_p};
        GemVal _t2614 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2611, 1), gem_to_string_fn(NULL, _t2612, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2613, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2614;
    } else {
    GemVal _t2615[] = {gem_v__match_41};
    GemVal _t2617;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2615, 1), gem_string("table")))) {
        _t2617 = gem_eq(gem_type_fn(NULL, _t2615, 1), gem_string("table"));
    } else {
        GemVal _t2616[] = {gem_v__match_41, gem_string("tag")};
        _t2617 = gem_has_key_fn(NULL, _t2616, 2);
    }
    GemVal _t2618;
    if (!gem_truthy(_t2617)) {
        _t2618 = _t2617;
    } else {
        _t2618 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2618)) {
#line 862 "compiler/main.gem"
    GemVal _t2619[] = {gem_v_node};
    GemVal _t2620 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2620.fn(_t2620.env, _t2619, 1);
#line 863 "compiler/main.gem"
    GemVal _t2621[] = {gem_v_line_dir};
    GemVal _t2622 = gem_v_r;
    GemVal _t2623[] = {gem_table_get(_t2622, gem_string("setup"))};
    GemVal _t2624[] = {gem_v_p};
    GemVal _t2625 = gem_v_r;
    GemVal _t2626[] = {gem_table_get(_t2625, gem_string("expr"))};
        GemVal _t2627 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2621, 1), gem_to_string_fn(NULL, _t2623, 1)), gem_to_string_fn(NULL, _t2624, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2626, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2627;
    } else {
    GemVal _t2628[] = {gem_v__match_41};
    GemVal _t2630;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2628, 1), gem_string("table")))) {
        _t2630 = gem_eq(gem_type_fn(NULL, _t2628, 1), gem_string("table"));
    } else {
        GemVal _t2629[] = {gem_v__match_41, gem_string("tag")};
        _t2630 = gem_has_key_fn(NULL, _t2629, 2);
    }
    GemVal _t2631;
    if (!gem_truthy(_t2630)) {
        _t2631 = _t2630;
    } else {
        _t2631 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t2631)) {
#line 865 "compiler/main.gem"
        GemVal _t2632 = gem_string("");
        gem_pop_frame();
        return _t2632;
    } else {
    GemVal _t2633[] = {gem_v__match_41};
    GemVal _t2635;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2633, 1), gem_string("table")))) {
        _t2635 = gem_eq(gem_type_fn(NULL, _t2633, 1), gem_string("table"));
    } else {
        GemVal _t2634[] = {gem_v__match_41, gem_string("tag")};
        _t2635 = gem_has_key_fn(NULL, _t2634, 2);
    }
    GemVal _t2636;
    if (!gem_truthy(_t2635)) {
        _t2636 = _t2635;
    } else {
        _t2636 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2638;
    if (!gem_truthy(_t2636)) {
        _t2638 = _t2636;
    } else {
        GemVal _t2637[] = {gem_v__match_41, gem_string("stmts")};
        _t2638 = gem_has_key_fn(NULL, _t2637, 2);
    }
    if (gem_truthy(_t2638)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_41, gem_string("stmts"));
#line 867 "compiler/main.gem"
        GemVal gem_v_out = gem_string("");
#line 868 "compiler/main.gem"
        GemVal gem_v__for_items_42 = gem_v_stmts;
#line 868 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 868 "compiler/main.gem"
        while (1) {
            GemVal _t2639[] = {gem_v__for_items_42};
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_len_fn(NULL, _t2639, 1)))) break;
#line 868 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_42, gem_v__for_i_42);
#line 868 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 869 "compiler/main.gem"
    GemVal _t2640[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2641 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2641.fn(_t2641.env, _t2640, 2)), gem_string("\n"));
        }

#line 871 "compiler/main.gem"
        GemVal _t2642 = gem_v_out;
        gem_pop_frame();
        return _t2642;
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
#line 874 "compiler/main.gem"
    GemVal _t2643[] = {gem_v_node};
    GemVal _t2644 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t2644.fn(_t2644.env, _t2643, 1);
    GemVal _t2645[] = {gem_v_line_dir};
    GemVal _t2646 = gem_v_r;
    GemVal _t2647[] = {gem_table_get(_t2646, gem_string("setup"))};
    GemVal _t2648[] = {gem_v_p};
    GemVal _t2649 = gem_v_r;
    GemVal _t2650[] = {gem_table_get(_t2649, gem_string("expr"))};
    GemVal _t2651 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2645, 1), gem_to_string_fn(NULL, _t2647, 1)), gem_to_string_fn(NULL, _t2648, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2650, 1)), gem_string(");"));
    gem_pop_frame();
    return _t2651;
}

struct _closure__anon_42 {
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
static GemVal _anon_42(void *_env, GemVal *args, int argc) {
    struct _closure__anon_42 *_cls = (struct _closure__anon_42 *)_env;
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
    gem_push_frame("_anon_42", "compiler/main.gem", 0);
#line 881 "compiler/main.gem"
    GemVal _t2653[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2653, 1);
#line 882 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 883 "compiler/main.gem"
    GemVal _t2654 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2654, gem_string("line")), GEM_NIL))) {
#line 884 "compiler/main.gem"
    GemVal _t2655 = gem_v_node;
    GemVal _t2656[] = {gem_table_get(_t2655, gem_string("line"))};
    GemVal _t2657[] = {(*gem_v_source_name)};
    GemVal _t2658[] = {gem_fn_escape_c_string(NULL, _t2657, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2656, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2658, 1)), gem_string("\"\n"));
    }
#line 887 "compiler/main.gem"
    GemVal gem_v__match_43 = gem_v_node;
    GemVal _t2659[] = {gem_v__match_43};
    GemVal _t2661;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2659, 1), gem_string("table")))) {
        _t2661 = gem_eq(gem_type_fn(NULL, _t2659, 1), gem_string("table"));
    } else {
        GemVal _t2660[] = {gem_v__match_43, gem_string("tag")};
        _t2661 = gem_has_key_fn(NULL, _t2660, 2);
    }
    GemVal _t2662;
    if (!gem_truthy(_t2661)) {
        _t2662 = _t2661;
    } else {
        _t2662 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2664;
    if (!gem_truthy(_t2662)) {
        _t2664 = _t2662;
    } else {
        GemVal _t2663[] = {gem_v__match_43, gem_string("name")};
        _t2664 = gem_has_key_fn(NULL, _t2663, 2);
    }
    GemVal _t2666;
    if (!gem_truthy(_t2664)) {
        _t2666 = _t2664;
    } else {
        GemVal _t2665[] = {gem_v__match_43, gem_string("value")};
        _t2666 = gem_has_key_fn(NULL, _t2665, 2);
    }
    if (gem_truthy(_t2666)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_43, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_43, gem_string("value"));
#line 889 "compiler/main.gem"
    GemVal _t2667[] = {gem_v_value};
    GemVal _t2668 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2668.fn(_t2668.env, _t2667, 1);
#line 890 "compiler/main.gem"
    GemVal _t2669[] = {gem_v_name};
    GemVal _t2670 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2670.fn(_t2670.env, _t2669, 1);
#line 891 "compiler/main.gem"
    GemVal _t2671[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2671, 2))) {
#line 892 "compiler/main.gem"
    GemVal _t2672[] = {gem_v_line_dir};
    GemVal _t2673 = gem_v_r;
    GemVal _t2674[] = {gem_table_get(_t2673, gem_string("setup"))};
    GemVal _t2675[] = {gem_v_p};
    GemVal _t2676[] = {gem_v_mname};
    GemVal _t2677[] = {gem_v_p};
    GemVal _t2678[] = {gem_v_mname};
    GemVal _t2679 = gem_v_r;
    GemVal _t2680[] = {gem_table_get(_t2679, gem_string("expr"))};
    GemVal _t2681[] = {gem_v_p};
    GemVal _t2682[] = {gem_v_p};
            GemVal _t2683 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2672, 1), gem_to_string_fn(NULL, _t2674, 1)), gem_to_string_fn(NULL, _t2675, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2676, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2677, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2678, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2680, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2681, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2682, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t2683;
        }
#line 894 "compiler/main.gem"
    GemVal _t2684[] = {gem_v_line_dir};
    GemVal _t2685 = gem_v_r;
    GemVal _t2686[] = {gem_table_get(_t2685, gem_string("setup"))};
    GemVal _t2687[] = {gem_v_p};
    GemVal _t2688[] = {gem_v_mname};
    GemVal _t2689 = gem_v_r;
    GemVal _t2690[] = {gem_table_get(_t2689, gem_string("expr"))};
    GemVal _t2691[] = {gem_v_p};
    GemVal _t2692[] = {gem_v_p};
        GemVal _t2693 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2684, 1), gem_to_string_fn(NULL, _t2686, 1)), gem_to_string_fn(NULL, _t2687, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2688, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2690, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2691, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2692, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2693;
    } else {
    GemVal _t2694[] = {gem_v__match_43};
    GemVal _t2696;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2694, 1), gem_string("table")))) {
        _t2696 = gem_eq(gem_type_fn(NULL, _t2694, 1), gem_string("table"));
    } else {
        GemVal _t2695[] = {gem_v__match_43, gem_string("tag")};
        _t2696 = gem_has_key_fn(NULL, _t2695, 2);
    }
    GemVal _t2697;
    if (!gem_truthy(_t2696)) {
        _t2697 = _t2696;
    } else {
        _t2697 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2699;
    if (!gem_truthy(_t2697)) {
        _t2699 = _t2697;
    } else {
        GemVal _t2698[] = {gem_v__match_43, gem_string("name")};
        _t2699 = gem_has_key_fn(NULL, _t2698, 2);
    }
    GemVal _t2701;
    if (!gem_truthy(_t2699)) {
        _t2701 = _t2699;
    } else {
        GemVal _t2700[] = {gem_v__match_43, gem_string("value")};
        _t2701 = gem_has_key_fn(NULL, _t2700, 2);
    }
    if (gem_truthy(_t2701)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_43, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_43, gem_string("value"));
#line 896 "compiler/main.gem"
    GemVal _t2702[] = {gem_v_value};
    GemVal _t2703 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2703.fn(_t2703.env, _t2702, 1);
#line 897 "compiler/main.gem"
    GemVal _t2704 = (*gem_v_tmp);
        GemVal gem_v_t = _t2704.fn(_t2704.env, NULL, 0);
#line 898 "compiler/main.gem"
    GemVal _t2705[] = {gem_v_name};
    GemVal _t2706 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2706.fn(_t2706.env, _t2705, 1);
#line 899 "compiler/main.gem"
    GemVal _t2707[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2709;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2707, 2))) {
        _t2709 = gem_fn_set_contains(NULL, _t2707, 2);
    } else {
        GemVal _t2708[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2709 = gem_fn_set_contains(NULL, _t2708, 2);
    }
        if (gem_truthy(_t2709)) {
#line 900 "compiler/main.gem"
    GemVal _t2710[] = {gem_v_line_dir};
    GemVal _t2711 = gem_v_r;
    GemVal _t2712[] = {gem_table_get(_t2711, gem_string("setup"))};
    GemVal _t2713[] = {gem_v_p};
    GemVal _t2714[] = {gem_v_t};
    GemVal _t2715 = gem_v_r;
    GemVal _t2716[] = {gem_table_get(_t2715, gem_string("expr"))};
    GemVal _t2717[] = {gem_v_p};
    GemVal _t2718[] = {gem_v_mname};
    GemVal _t2719[] = {gem_v_t};
    GemVal _t2720[] = {gem_v_p};
    GemVal _t2721[] = {gem_v_p};
    GemVal _t2722[] = {gem_v_t};
            GemVal _t2723 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2710, 1), gem_to_string_fn(NULL, _t2712, 1)), gem_to_string_fn(NULL, _t2713, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2714, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2716, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2717, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2718, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2719, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2720, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2721, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2722, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2723;
        }
#line 902 "compiler/main.gem"
    GemVal _t2724[] = {gem_v_line_dir};
    GemVal _t2725 = gem_v_r;
    GemVal _t2726[] = {gem_table_get(_t2725, gem_string("setup"))};
    GemVal _t2727[] = {gem_v_p};
    GemVal _t2728[] = {gem_v_t};
    GemVal _t2729 = gem_v_r;
    GemVal _t2730[] = {gem_table_get(_t2729, gem_string("expr"))};
    GemVal _t2731[] = {gem_v_p};
    GemVal _t2732[] = {gem_v_mname};
    GemVal _t2733[] = {gem_v_t};
    GemVal _t2734[] = {gem_v_p};
    GemVal _t2735[] = {gem_v_p};
    GemVal _t2736[] = {gem_v_t};
        GemVal _t2737 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2724, 1), gem_to_string_fn(NULL, _t2726, 1)), gem_to_string_fn(NULL, _t2727, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2728, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2730, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2731, 1)), gem_to_string_fn(NULL, _t2732, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2733, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2734, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2735, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2736, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2737;
    } else {
    GemVal _t2738[] = {gem_v__match_43};
    GemVal _t2740;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2738, 1), gem_string("table")))) {
        _t2740 = gem_eq(gem_type_fn(NULL, _t2738, 1), gem_string("table"));
    } else {
        GemVal _t2739[] = {gem_v__match_43, gem_string("tag")};
        _t2740 = gem_has_key_fn(NULL, _t2739, 2);
    }
    GemVal _t2741;
    if (!gem_truthy(_t2740)) {
        _t2741 = _t2740;
    } else {
        _t2741 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2743;
    if (!gem_truthy(_t2741)) {
        _t2743 = _t2741;
    } else {
        GemVal _t2742[] = {gem_v__match_43, gem_string("value")};
        _t2743 = gem_has_key_fn(NULL, _t2742, 2);
    }
    if (gem_truthy(_t2743)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_43, gem_string("value"));
#line 904 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 905 "compiler/main.gem"
    GemVal _t2744[] = {gem_v_value};
    GemVal _t2745 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2745.fn(_t2745.env, _t2744, 1);
#line 906 "compiler/main.gem"
    GemVal _t2746 = (*gem_v_tmp);
            GemVal gem_v_t = _t2746.fn(_t2746.env, NULL, 0);
#line 907 "compiler/main.gem"
    GemVal _t2747[] = {gem_v_line_dir};
    GemVal _t2748 = gem_v_r;
    GemVal _t2749[] = {gem_table_get(_t2748, gem_string("setup"))};
    GemVal _t2750[] = {gem_v_p};
    GemVal _t2751[] = {gem_v_t};
    GemVal _t2752 = gem_v_r;
    GemVal _t2753[] = {gem_table_get(_t2752, gem_string("expr"))};
    GemVal _t2754[] = {gem_v_p};
    GemVal _t2755[] = {gem_v_p};
    GemVal _t2756[] = {gem_v_t};
            GemVal _t2757 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2747, 1), gem_to_string_fn(NULL, _t2749, 1)), gem_to_string_fn(NULL, _t2750, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2751, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2753, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2754, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2755, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2756, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2757;
        }
#line 909 "compiler/main.gem"
    GemVal _t2758[] = {gem_v_line_dir};
    GemVal _t2759[] = {gem_v_p};
    GemVal _t2760[] = {gem_v_p};
        GemVal _t2761 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2758, 1), gem_to_string_fn(NULL, _t2759, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2760, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2761;
    } else {
    GemVal _t2762[] = {gem_v__match_43};
    GemVal _t2764;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2762, 1), gem_string("table")))) {
        _t2764 = gem_eq(gem_type_fn(NULL, _t2762, 1), gem_string("table"));
    } else {
        GemVal _t2763[] = {gem_v__match_43, gem_string("tag")};
        _t2764 = gem_has_key_fn(NULL, _t2763, 2);
    }
    GemVal _t2765;
    if (!gem_truthy(_t2764)) {
        _t2765 = _t2764;
    } else {
        _t2765 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t2765)) {
#line 911 "compiler/main.gem"
    GemVal _t2766[] = {gem_v_line_dir};
    GemVal _t2767[] = {gem_v_node, gem_v_indent};
    GemVal _t2768 = (*gem_v_compile_if_return);
        GemVal _t2769 = gem_add(gem_to_string_fn(NULL, _t2766, 1), _t2768.fn(_t2768.env, _t2767, 2));
        gem_pop_frame();
        return _t2769;
    } else {
    GemVal _t2770[] = {gem_v__match_43};
    GemVal _t2772;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2770, 1), gem_string("table")))) {
        _t2772 = gem_eq(gem_type_fn(NULL, _t2770, 1), gem_string("table"));
    } else {
        GemVal _t2771[] = {gem_v__match_43, gem_string("tag")};
        _t2772 = gem_has_key_fn(NULL, _t2771, 2);
    }
    GemVal _t2773;
    if (!gem_truthy(_t2772)) {
        _t2773 = _t2772;
    } else {
        _t2773 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t2773)) {
#line 913 "compiler/main.gem"
    GemVal _t2774[] = {gem_v_line_dir};
    GemVal _t2775[] = {gem_v_node, gem_v_indent};
    GemVal _t2776 = (*gem_v_compile_while);
    GemVal _t2777[] = {_t2776.fn(_t2776.env, _t2775, 2)};
    GemVal _t2778[] = {gem_v_p};
    GemVal _t2779[] = {gem_v_p};
        GemVal _t2780 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2774, 1), gem_to_string_fn(NULL, _t2777, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t2778, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2779, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2780;
    } else {
    GemVal _t2781[] = {gem_v__match_43};
    GemVal _t2783;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2781, 1), gem_string("table")))) {
        _t2783 = gem_eq(gem_type_fn(NULL, _t2781, 1), gem_string("table"));
    } else {
        GemVal _t2782[] = {gem_v__match_43, gem_string("tag")};
        _t2783 = gem_has_key_fn(NULL, _t2782, 2);
    }
    GemVal _t2784;
    if (!gem_truthy(_t2783)) {
        _t2784 = _t2783;
    } else {
        _t2784 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t2784)) {
#line 915 "compiler/main.gem"
    GemVal _t2785[] = {gem_v_line_dir};
    GemVal _t2786[] = {gem_v_node, gem_v_indent};
    GemVal _t2787 = (*gem_v_compile_match_return);
        GemVal _t2788 = gem_add(gem_to_string_fn(NULL, _t2785, 1), _t2787.fn(_t2787.env, _t2786, 2));
        gem_pop_frame();
        return _t2788;
    } else {
    GemVal _t2789[] = {gem_v__match_43};
    GemVal _t2791;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2789, 1), gem_string("table")))) {
        _t2791 = gem_eq(gem_type_fn(NULL, _t2789, 1), gem_string("table"));
    } else {
        GemVal _t2790[] = {gem_v__match_43, gem_string("tag")};
        _t2791 = gem_has_key_fn(NULL, _t2790, 2);
    }
    GemVal _t2792;
    if (!gem_truthy(_t2791)) {
        _t2792 = _t2791;
    } else {
        _t2792 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2794;
    if (!gem_truthy(_t2792)) {
        _t2794 = _t2792;
    } else {
        GemVal _t2793[] = {gem_v__match_43, gem_string("stmts")};
        _t2794 = gem_has_key_fn(NULL, _t2793, 2);
    }
    if (gem_truthy(_t2794)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_43, gem_string("stmts"));
#line 917 "compiler/main.gem"
    GemVal _t2795[] = {gem_v_p};
        GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2795, 1), gem_string("{\n"));
#line 918 "compiler/main.gem"
        GemVal gem_v__for_i_44 = gem_int(0);
#line 918 "compiler/main.gem"
    GemVal _t2796[] = {gem_v_stmts};
        GemVal gem_v__for_limit_44 = gem_sub(gem_len_fn(NULL, _t2796, 1), gem_int(1));
#line 918 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 918 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_44;
#line 918 "compiler/main.gem"
            gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 919 "compiler/main.gem"
    GemVal _t2797[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2798 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2798.fn(_t2798.env, _t2797, 2)), gem_string("\n"));
        }

#line 921 "compiler/main.gem"
    GemVal _t2799[] = {gem_v_stmts};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2799, 1), gem_int(0)))) {
#line 922 "compiler/main.gem"
    GemVal _t2800[] = {gem_v_stmts};
    GemVal _t2801[] = {gem_table_get(gem_v_stmts, gem_sub(gem_len_fn(NULL, _t2800, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2802 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2802.fn(_t2802.env, _t2801, 2)), gem_string("\n"));
        }
#line 924 "compiler/main.gem"
    GemVal _t2803[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2803, 1), gem_string("}")));
#line 925 "compiler/main.gem"
        GemVal _t2804 = gem_v_out;
        gem_pop_frame();
        return _t2804;
    } else {
    GemVal _t2805[] = {gem_v__match_43};
    GemVal _t2807;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2805, 1), gem_string("table")))) {
        _t2807 = gem_eq(gem_type_fn(NULL, _t2805, 1), gem_string("table"));
    } else {
        GemVal _t2806[] = {gem_v__match_43, gem_string("tag")};
        _t2807 = gem_has_key_fn(NULL, _t2806, 2);
    }
    GemVal _t2808;
    if (!gem_truthy(_t2807)) {
        _t2808 = _t2807;
    } else {
        _t2808 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("dot_assign"));
    }
    if (gem_truthy(_t2808)) {
#line 927 "compiler/main.gem"
    GemVal _t2809[] = {gem_v_node, gem_v_indent};
    GemVal _t2810 = (*gem_v_compile_stmt);
    GemVal _t2811[] = {_t2810.fn(_t2810.env, _t2809, 2)};
    GemVal _t2812[] = {gem_v_p};
    GemVal _t2813[] = {gem_v_p};
        GemVal _t2814 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2811, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2812, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2813, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2814;
    } else {
    GemVal _t2815[] = {gem_v__match_43};
    GemVal _t2817;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2815, 1), gem_string("table")))) {
        _t2817 = gem_eq(gem_type_fn(NULL, _t2815, 1), gem_string("table"));
    } else {
        GemVal _t2816[] = {gem_v__match_43, gem_string("tag")};
        _t2817 = gem_has_key_fn(NULL, _t2816, 2);
    }
    GemVal _t2818;
    if (!gem_truthy(_t2817)) {
        _t2818 = _t2817;
    } else {
        _t2818 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("index_assign"));
    }
    if (gem_truthy(_t2818)) {
#line 929 "compiler/main.gem"
    GemVal _t2819[] = {gem_v_node, gem_v_indent};
    GemVal _t2820 = (*gem_v_compile_stmt);
    GemVal _t2821[] = {_t2820.fn(_t2820.env, _t2819, 2)};
    GemVal _t2822[] = {gem_v_p};
    GemVal _t2823[] = {gem_v_p};
        GemVal _t2824 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2821, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2822, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2823, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2824;
    } else {
    GemVal _t2825[] = {gem_v__match_43};
    GemVal _t2827;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2825, 1), gem_string("table")))) {
        _t2827 = gem_eq(gem_type_fn(NULL, _t2825, 1), gem_string("table"));
    } else {
        GemVal _t2826[] = {gem_v__match_43, gem_string("tag")};
        _t2827 = gem_has_key_fn(NULL, _t2826, 2);
    }
    GemVal _t2828;
    if (!gem_truthy(_t2827)) {
        _t2828 = _t2827;
    } else {
        _t2828 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t2828)) {
#line 931 "compiler/main.gem"
    GemVal _t2829[] = {gem_v_node, gem_v_indent};
    GemVal _t2830 = (*gem_v_compile_stmt);
    GemVal _t2831[] = {_t2830.fn(_t2830.env, _t2829, 2)};
    GemVal _t2832[] = {gem_v_p};
    GemVal _t2833[] = {gem_v_p};
        GemVal _t2834 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2831, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2832, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2833, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2834;
    } else {
    GemVal _t2835[] = {gem_v__match_43};
    GemVal _t2837;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2835, 1), gem_string("table")))) {
        _t2837 = gem_eq(gem_type_fn(NULL, _t2835, 1), gem_string("table"));
    } else {
        GemVal _t2836[] = {gem_v__match_43, gem_string("tag")};
        _t2837 = gem_has_key_fn(NULL, _t2836, 2);
    }
    GemVal _t2838;
    if (!gem_truthy(_t2837)) {
        _t2838 = _t2837;
    } else {
        _t2838 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t2838)) {
#line 933 "compiler/main.gem"
    GemVal _t2839[] = {gem_v_node, gem_v_indent};
    GemVal _t2840 = (*gem_v_compile_stmt);
    GemVal _t2841[] = {_t2840.fn(_t2840.env, _t2839, 2)};
    GemVal _t2842[] = {gem_v_p};
    GemVal _t2843[] = {gem_v_p};
        GemVal _t2844 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2841, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2842, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2843, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2844;
    } else {
    GemVal _t2845[] = {gem_v__match_43};
    GemVal _t2847;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2845, 1), gem_string("table")))) {
        _t2847 = gem_eq(gem_type_fn(NULL, _t2845, 1), gem_string("table"));
    } else {
        GemVal _t2846[] = {gem_v__match_43, gem_string("tag")};
        _t2847 = gem_has_key_fn(NULL, _t2846, 2);
    }
    GemVal _t2848;
    if (!gem_truthy(_t2847)) {
        _t2848 = _t2847;
    } else {
        _t2848 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t2848)) {
#line 935 "compiler/main.gem"
    GemVal _t2849[] = {gem_v_node, gem_v_indent};
    GemVal _t2850 = (*gem_v_compile_stmt);
    GemVal _t2851[] = {_t2850.fn(_t2850.env, _t2849, 2)};
    GemVal _t2852[] = {gem_v_p};
    GemVal _t2853[] = {gem_v_p};
        GemVal _t2854 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2851, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2852, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2853, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2854;
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
#line 938 "compiler/main.gem"
    GemVal _t2855[] = {gem_v_node};
    GemVal _t2856 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t2856.fn(_t2856.env, _t2855, 1);
#line 939 "compiler/main.gem"
    GemVal _t2857 = (*gem_v_tmp);
    GemVal gem_v_t = _t2857.fn(_t2857.env, NULL, 0);
    GemVal _t2858[] = {gem_v_line_dir};
    GemVal _t2859 = gem_v_r;
    GemVal _t2860[] = {gem_table_get(_t2859, gem_string("setup"))};
    GemVal _t2861[] = {gem_v_p};
    GemVal _t2862[] = {gem_v_t};
    GemVal _t2863 = gem_v_r;
    GemVal _t2864[] = {gem_table_get(_t2863, gem_string("expr"))};
    GemVal _t2865[] = {gem_v_p};
    GemVal _t2866[] = {gem_v_p};
    GemVal _t2867[] = {gem_v_t};
    GemVal _t2868 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2858, 1), gem_to_string_fn(NULL, _t2860, 1)), gem_to_string_fn(NULL, _t2861, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2862, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2864, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2865, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2866, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2867, 1)), gem_string(";"));
    gem_pop_frame();
    return _t2868;
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
#line 946 "compiler/main.gem"
    GemVal _t2870[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2870, 1);
#line 947 "compiler/main.gem"
    GemVal _t2871 = gem_v_node;
    GemVal _t2872[] = {gem_table_get(_t2871, gem_string("cond"))};
    GemVal _t2873 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t2873.fn(_t2873.env, _t2872, 1);
#line 948 "compiler/main.gem"
    GemVal _t2874 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2874, gem_string("setup"));
#line 949 "compiler/main.gem"
    GemVal _t2875[] = {gem_v_p};
    GemVal _t2876 = gem_v_cond_r;
    GemVal _t2877[] = {gem_table_get(_t2876, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2875, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2877, 1)), gem_string(")) {\n")));
#line 950 "compiler/main.gem"
    GemVal _t2878 = gem_v_node;
    GemVal _t2879[] = {gem_table_get(_t2878, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2880 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t2880.fn(_t2880.env, _t2879, 2));
#line 951 "compiler/main.gem"
    GemVal _t2881 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2881, gem_string("else")), GEM_NIL))) {
#line 952 "compiler/main.gem"
    GemVal _t2882[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2882, 1), gem_string("} else {\n")));
#line 953 "compiler/main.gem"
    GemVal _t2883 = gem_v_node;
    GemVal _t2884[] = {gem_table_get(_t2883, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2885 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t2885.fn(_t2885.env, _t2884, 2));
    }
#line 955 "compiler/main.gem"
    GemVal _t2886[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2886, 1), gem_string("}")));
    GemVal _t2887 = gem_v_out;
    gem_pop_frame();
    return _t2887;
}

struct _closure__anon_44 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_44", "compiler/main.gem", 0);
#line 962 "compiler/main.gem"
    GemVal _t2889[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2889, 1);
#line 963 "compiler/main.gem"
    GemVal _t2890 = gem_v_node;
    GemVal _t2891[] = {gem_table_get(_t2890, gem_string("cond"))};
    GemVal _t2892 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t2892.fn(_t2892.env, _t2891, 1);
#line 964 "compiler/main.gem"
    GemVal _t2893 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t2893, gem_string("setup"));
#line 965 "compiler/main.gem"
    GemVal _t2894[] = {gem_v_p};
    GemVal _t2895 = gem_v_cond_r;
    GemVal _t2896[] = {gem_table_get(_t2895, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2894, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2896, 1)), gem_string(")) {\n")));
#line 966 "compiler/main.gem"
    GemVal _t2897 = gem_v_node;
    GemVal gem_v_then_body = gem_table_get(_t2897, gem_string("then"));
#line 967 "compiler/main.gem"
    GemVal _t2898[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2898, 1), gem_int(0)))) {
#line 968 "compiler/main.gem"
        GemVal gem_v__for_i_45 = gem_int(0);
#line 968 "compiler/main.gem"
    GemVal _t2899[] = {gem_v_then_body};
        GemVal gem_v__for_limit_45 = gem_sub(gem_len_fn(NULL, _t2899, 1), gem_int(1));
#line 968 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 968 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_45;
#line 968 "compiler/main.gem"
            gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 969 "compiler/main.gem"
    GemVal _t2900[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2901 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2901.fn(_t2901.env, _t2900, 2)), gem_string("\n"));
        }

#line 971 "compiler/main.gem"
    GemVal _t2902[] = {gem_v_then_body};
    GemVal _t2903[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t2902, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2904 = (*gem_v_compile_stmt_return);
        gem_v_out = gem_add(gem_add(gem_v_out, _t2904.fn(_t2904.env, _t2903, 2)), gem_string("\n"));
    } else {
#line 973 "compiler/main.gem"
    GemVal _t2905[] = {gem_v_p};
    GemVal _t2906[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2905, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2906, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 975 "compiler/main.gem"
    GemVal _t2907 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2907, gem_string("else")), GEM_NIL))) {
#line 976 "compiler/main.gem"
    GemVal _t2908[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2908, 1), gem_string("} else {\n")));
#line 977 "compiler/main.gem"
    GemVal _t2909 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t2909, gem_string("else"));
#line 978 "compiler/main.gem"
    GemVal _t2910[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2910, 1), gem_int(0)))) {
#line 979 "compiler/main.gem"
            GemVal gem_v__for_i_46 = gem_int(0);
#line 979 "compiler/main.gem"
    GemVal _t2911[] = {gem_v_else_body};
            GemVal gem_v__for_limit_46 = gem_sub(gem_len_fn(NULL, _t2911, 1), gem_int(1));
#line 979 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
#line 979 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_46;
#line 979 "compiler/main.gem"
                gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 980 "compiler/main.gem"
    GemVal _t2912[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2913 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t2913.fn(_t2913.env, _t2912, 2)), gem_string("\n"));
            }

#line 982 "compiler/main.gem"
    GemVal _t2914[] = {gem_v_else_body};
    GemVal _t2915[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t2914, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2916 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2916.fn(_t2916.env, _t2915, 2)), gem_string("\n"));
        } else {
#line 984 "compiler/main.gem"
    GemVal _t2917[] = {gem_v_p};
    GemVal _t2918[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2917, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2918, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 987 "compiler/main.gem"
    GemVal _t2919[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2919, 1), gem_string("} else {\n")));
#line 988 "compiler/main.gem"
    GemVal _t2920[] = {gem_v_p};
    GemVal _t2921[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2920, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2921, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 990 "compiler/main.gem"
    GemVal _t2922[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2922, 1), gem_string("}")));
    GemVal _t2923 = gem_v_out;
    gem_pop_frame();
    return _t2923;
}

struct _closure__anon_45 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts;
};
static GemVal _anon_45(void *_env, GemVal *args, int argc) {
    struct _closure__anon_45 *_cls = (struct _closure__anon_45 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_45", "compiler/main.gem", 0);
#line 997 "compiler/main.gem"
    GemVal _t2925[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2925, 1);
#line 998 "compiler/main.gem"
    GemVal _t2926 = gem_v_node;
    GemVal _t2927[] = {gem_table_get(_t2926, gem_string("cond"))};
    GemVal _t2928 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t2928.fn(_t2928.env, _t2927, 1);
#line 999 "compiler/main.gem"
    GemVal _t2929[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2929, 1), gem_string("while (1) {\n"));
#line 1000 "compiler/main.gem"
    GemVal _t2930 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t2930, gem_string("setup")), gem_string("")))) {
#line 1001 "compiler/main.gem"
    GemVal _t2931 = gem_v_cond_r;
    GemVal _t2932[] = {gem_v_p};
    GemVal _t2933[] = {gem_table_get(_t2931, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t2932, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_str_replace_fn(NULL, _t2933, 3));
    }
#line 1003 "compiler/main.gem"
    GemVal _t2934[] = {gem_v_p};
    GemVal _t2935 = gem_v_cond_r;
    GemVal _t2936[] = {gem_table_get(_t2935, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2934, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t2936, 1)), gem_string(")) break;\n")));
#line 1004 "compiler/main.gem"
    GemVal _t2937 = gem_v_node;
    GemVal _t2938[] = {gem_table_get(_t2937, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2939 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t2939.fn(_t2939.env, _t2938, 2));
#line 1005 "compiler/main.gem"
    GemVal _t2940[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2940, 1), gem_string("}")));
    GemVal _t2941 = gem_v_out;
    gem_pop_frame();
    return _t2941;
}

struct _closure__anon_46 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_46(void *_env, GemVal *args, int argc) {
    struct _closure__anon_46 *_cls = (struct _closure__anon_46 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_46", "compiler/main.gem", 0);
#line 1012 "compiler/main.gem"
    GemVal _t2943[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2943, 1);
#line 1013 "compiler/main.gem"
    GemVal _t2944 = gem_v_node;
    GemVal _t2945[] = {gem_table_get(_t2944, gem_string("target"))};
    GemVal _t2946 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t2946.fn(_t2946.env, _t2945, 1);
#line 1014 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1015 "compiler/main.gem"
    GemVal _t2947 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2947, gem_string("target_var")), GEM_NIL))) {
#line 1016 "compiler/main.gem"
    GemVal _t2948 = gem_v_node;
    GemVal _t2949[] = {gem_table_get(_t2948, gem_string("target_var"))};
    GemVal _t2950 = (*gem_v_mangle);
        gem_v_t = _t2950.fn(_t2950.env, _t2949, 1);
    } else {
#line 1018 "compiler/main.gem"
    GemVal _t2951 = (*gem_v_tmp);
        gem_v_t = _t2951.fn(_t2951.env, NULL, 0);
    }
#line 1020 "compiler/main.gem"
    GemVal _t2952 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t2952, gem_string("setup"));
#line 1021 "compiler/main.gem"
    GemVal _t2953[] = {gem_v_p};
    GemVal _t2954[] = {gem_v_t};
    GemVal _t2955 = gem_v_target_r;
    GemVal _t2956[] = {gem_table_get(_t2955, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2953, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2954, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2956, 1)), gem_string(";\n")));
#line 1022 "compiler/main.gem"
    GemVal _t2957 = gem_v_node;
    GemVal _t2958[] = {gem_table_get(_t2957, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2958, 1), gem_int(0)))) {
#line 1024 "compiler/main.gem"
    GemVal _t2959 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2959, gem_string("else")), GEM_NIL))) {
#line 1025 "compiler/main.gem"
    GemVal _t2960 = gem_v_node;
    GemVal _t2961[] = {gem_table_get(_t2960, gem_string("else")), gem_v_indent};
    GemVal _t2962 = (*gem_v_compile_stmts);
            gem_v_out = gem_add(gem_v_out, _t2962.fn(_t2962.env, _t2961, 2));
        }
#line 1027 "compiler/main.gem"
    GemVal _t2963[] = {gem_v_p};
    GemVal _t2964[] = {gem_v_t};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2963, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t2964, 1)), gem_string(";")));
#line 1028 "compiler/main.gem"
        GemVal _t2965 = gem_v_out;
        gem_pop_frame();
        return _t2965;
    }
#line 1030 "compiler/main.gem"
    GemVal gem_v__for_i_48 = gem_int(0);
#line 1030 "compiler/main.gem"
    GemVal _t2966 = gem_v_node;
    GemVal _t2967[] = {gem_table_get(_t2966, gem_string("whens"))};
    GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t2967, 1);
#line 1030 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 1030 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_48;
#line 1030 "compiler/main.gem"
        gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 1031 "compiler/main.gem"
    GemVal _t2968 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t2968, gem_string("whens")), gem_v_i);
#line 1032 "compiler/main.gem"
    GemVal _t2969 = gem_v_w;
    GemVal _t2970[] = {gem_table_get(_t2969, gem_string("value"))};
    GemVal _t2971 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2971.fn(_t2971.env, _t2970, 1);
#line 1033 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1034 "compiler/main.gem"
    GemVal _t2972 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t2972, gem_string("setup")));
#line 1035 "compiler/main.gem"
    GemVal _t2973[] = {gem_v_p};
    GemVal _t2974 = gem_v_val_r;
    GemVal _t2975[] = {gem_table_get(_t2974, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2973, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2975, 1)), gem_string(")) {\n")));
        } else {
#line 1037 "compiler/main.gem"
    GemVal _t2976[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2976, 1), gem_string("} else {\n")));
#line 1038 "compiler/main.gem"
    GemVal _t2977 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t2977, gem_string("setup")));
#line 1039 "compiler/main.gem"
    GemVal _t2978[] = {gem_v_p};
    GemVal _t2979 = gem_v_val_r;
    GemVal _t2980[] = {gem_table_get(_t2979, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2978, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t2980, 1)), gem_string(")) {\n")));
        }
#line 1041 "compiler/main.gem"
    GemVal _t2981 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t2981, gem_string("bindings")), GEM_NIL))) {
#line 1042 "compiler/main.gem"
    GemVal _t2982 = gem_v_w;
            GemVal gem_v__for_items_47 = gem_table_get(_t2982, gem_string("bindings"));
#line 1042 "compiler/main.gem"
            GemVal gem_v__for_i_47 = gem_int(0);
#line 1042 "compiler/main.gem"
            while (1) {
                GemVal _t2983[] = {gem_v__for_items_47};
                if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_len_fn(NULL, _t2983, 1)))) break;
#line 1042 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_47, gem_v__for_i_47);
#line 1042 "compiler/main.gem"
                gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 1043 "compiler/main.gem"
    GemVal _t2984[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2985 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t2985.fn(_t2985.env, _t2984, 2)), gem_string("\n"));
            }

        }
#line 1046 "compiler/main.gem"
    GemVal _t2986 = gem_v_w;
    GemVal _t2987[] = {gem_table_get(_t2986, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2988 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t2988.fn(_t2988.env, _t2987, 2));
    }

#line 1048 "compiler/main.gem"
    GemVal _t2989 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2989, gem_string("else")), GEM_NIL))) {
#line 1049 "compiler/main.gem"
    GemVal _t2990[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2990, 1), gem_string("} else {\n")));
#line 1050 "compiler/main.gem"
    GemVal _t2991 = gem_v_node;
    GemVal _t2992[] = {gem_table_get(_t2991, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2993 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t2993.fn(_t2993.env, _t2992, 2));
    }
#line 1053 "compiler/main.gem"
    GemVal _t2994[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2994, 1), gem_string("}")));
#line 1054 "compiler/main.gem"
    GemVal gem_v__for_i_49 = gem_int(1);
#line 1054 "compiler/main.gem"
    GemVal _t2995 = gem_v_node;
    GemVal _t2996[] = {gem_table_get(_t2995, gem_string("whens"))};
    GemVal gem_v__for_limit_49 = gem_len_fn(NULL, _t2996, 1);
#line 1054 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_v__for_limit_49))) break;
#line 1054 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_49;
#line 1054 "compiler/main.gem"
        gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 1055 "compiler/main.gem"
    GemVal _t2997[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("\n"), gem_to_string_fn(NULL, _t2997, 1)), gem_string("}")));
    }

    GemVal _t2998 = gem_v_out;
    gem_pop_frame();
    return _t2998;
}

struct _closure__anon_47 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_47(void *_env, GemVal *args, int argc) {
    struct _closure__anon_47 *_cls = (struct _closure__anon_47 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_47", "compiler/main.gem", 0);
#line 1063 "compiler/main.gem"
    GemVal _t3000[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3000, 1);
#line 1064 "compiler/main.gem"
    GemVal _t3001 = gem_v_node;
    GemVal _t3002[] = {gem_table_get(_t3001, gem_string("target"))};
    GemVal _t3003 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3003.fn(_t3003.env, _t3002, 1);
#line 1065 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1066 "compiler/main.gem"
    GemVal _t3004 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3004, gem_string("target_var")), GEM_NIL))) {
#line 1067 "compiler/main.gem"
    GemVal _t3005 = gem_v_node;
    GemVal _t3006[] = {gem_table_get(_t3005, gem_string("target_var"))};
    GemVal _t3007 = (*gem_v_mangle);
        gem_v_t = _t3007.fn(_t3007.env, _t3006, 1);
    } else {
#line 1069 "compiler/main.gem"
    GemVal _t3008 = (*gem_v_tmp);
        gem_v_t = _t3008.fn(_t3008.env, NULL, 0);
    }
#line 1071 "compiler/main.gem"
    GemVal _t3009 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t3009, gem_string("setup"));
#line 1072 "compiler/main.gem"
    GemVal _t3010[] = {gem_v_p};
    GemVal _t3011[] = {gem_v_t};
    GemVal _t3012 = gem_v_target_r;
    GemVal _t3013[] = {gem_table_get(_t3012, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3010, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3011, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3013, 1)), gem_string(";\n")));
#line 1073 "compiler/main.gem"
    GemVal _t3014 = gem_v_node;
    GemVal _t3015[] = {gem_table_get(_t3014, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3015, 1), gem_int(0)))) {
#line 1075 "compiler/main.gem"
    GemVal _t3016 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3016, gem_string("else")), GEM_NIL))) {
#line 1076 "compiler/main.gem"
    GemVal _t3017 = gem_v_node;
            GemVal gem_v_else_body = gem_table_get(_t3017, gem_string("else"));
#line 1077 "compiler/main.gem"
    GemVal _t3018[] = {gem_v_else_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3018, 1), gem_int(0)))) {
#line 1078 "compiler/main.gem"
                GemVal gem_v__for_i_50 = gem_int(0);
#line 1078 "compiler/main.gem"
    GemVal _t3019[] = {gem_v_else_body};
                GemVal gem_v__for_limit_50 = gem_sub(gem_len_fn(NULL, _t3019, 1), gem_int(1));
#line 1078 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
#line 1078 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_50;
#line 1078 "compiler/main.gem"
                    gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 1079 "compiler/main.gem"
    GemVal _t3020[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_v_indent};
    GemVal _t3021 = (*gem_v_compile_stmt);
                    gem_v_out = gem_add(gem_add(gem_v_out, _t3021.fn(_t3021.env, _t3020, 2)), gem_string("\n"));
                }

#line 1081 "compiler/main.gem"
    GemVal _t3022[] = {gem_v_else_body};
    GemVal _t3023[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t3022, 1), gem_int(1))), gem_v_indent};
    GemVal _t3024 = (*gem_v_compile_stmt_return);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3024.fn(_t3024.env, _t3023, 2)), gem_string("\n"));
            } else {
#line 1083 "compiler/main.gem"
    GemVal _t3025[] = {gem_v_p};
    GemVal _t3026[] = {gem_v_p};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3025, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3026, 1)), gem_string("return GEM_NIL;\n")));
            }
        } else {
#line 1086 "compiler/main.gem"
    GemVal _t3027[] = {gem_v_p};
    GemVal _t3028[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3027, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3028, 1)), gem_string("return GEM_NIL;\n")));
        }
#line 1088 "compiler/main.gem"
    GemVal _t3029[] = {gem_v_p};
    GemVal _t3030[] = {gem_v_t};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3029, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t3030, 1)), gem_string(";")));
#line 1089 "compiler/main.gem"
        GemVal _t3031 = gem_v_out;
        gem_pop_frame();
        return _t3031;
    }
#line 1091 "compiler/main.gem"
    GemVal gem_v__for_i_53 = gem_int(0);
#line 1091 "compiler/main.gem"
    GemVal _t3032 = gem_v_node;
    GemVal _t3033[] = {gem_table_get(_t3032, gem_string("whens"))};
    GemVal gem_v__for_limit_53 = gem_len_fn(NULL, _t3033, 1);
#line 1091 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_v__for_limit_53))) break;
#line 1091 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_53;
#line 1091 "compiler/main.gem"
        gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 1092 "compiler/main.gem"
    GemVal _t3034 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t3034, gem_string("whens")), gem_v_i);
#line 1093 "compiler/main.gem"
    GemVal _t3035 = gem_v_w;
    GemVal _t3036[] = {gem_table_get(_t3035, gem_string("value"))};
    GemVal _t3037 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3037.fn(_t3037.env, _t3036, 1);
#line 1094 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1095 "compiler/main.gem"
    GemVal _t3038 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3038, gem_string("setup")));
#line 1096 "compiler/main.gem"
    GemVal _t3039[] = {gem_v_p};
    GemVal _t3040 = gem_v_val_r;
    GemVal _t3041[] = {gem_table_get(_t3040, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3039, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3041, 1)), gem_string(")) {\n")));
        } else {
#line 1098 "compiler/main.gem"
    GemVal _t3042[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3042, 1), gem_string("} else {\n")));
#line 1099 "compiler/main.gem"
    GemVal _t3043 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3043, gem_string("setup")));
#line 1100 "compiler/main.gem"
    GemVal _t3044[] = {gem_v_p};
    GemVal _t3045 = gem_v_val_r;
    GemVal _t3046[] = {gem_table_get(_t3045, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3044, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3046, 1)), gem_string(")) {\n")));
        }
#line 1102 "compiler/main.gem"
    GemVal _t3047 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t3047, gem_string("bindings")), GEM_NIL))) {
#line 1103 "compiler/main.gem"
    GemVal _t3048 = gem_v_w;
            GemVal gem_v__for_items_51 = gem_table_get(_t3048, gem_string("bindings"));
#line 1103 "compiler/main.gem"
            GemVal gem_v__for_i_51 = gem_int(0);
#line 1103 "compiler/main.gem"
            while (1) {
                GemVal _t3049[] = {gem_v__for_items_51};
                if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_len_fn(NULL, _t3049, 1)))) break;
#line 1103 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_51, gem_v__for_i_51);
#line 1103 "compiler/main.gem"
                gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 1104 "compiler/main.gem"
    GemVal _t3050[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3051 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3051.fn(_t3051.env, _t3050, 2)), gem_string("\n"));
            }

        }
#line 1107 "compiler/main.gem"
    GemVal _t3052 = gem_v_w;
        GemVal gem_v_body = gem_table_get(_t3052, gem_string("body"));
#line 1108 "compiler/main.gem"
    GemVal _t3053[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3053, 1), gem_int(0)))) {
#line 1109 "compiler/main.gem"
            GemVal gem_v__for_i_52 = gem_int(0);
#line 1109 "compiler/main.gem"
    GemVal _t3054[] = {gem_v_body};
            GemVal gem_v__for_limit_52 = gem_sub(gem_len_fn(NULL, _t3054, 1), gem_int(1));
#line 1109 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 1109 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_52;
#line 1109 "compiler/main.gem"
                gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 1110 "compiler/main.gem"
    GemVal _t3055[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3056 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3056.fn(_t3056.env, _t3055, 2)), gem_string("\n"));
            }

#line 1112 "compiler/main.gem"
    GemVal _t3057[] = {gem_v_body};
    GemVal _t3058[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t3057, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3059 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3059.fn(_t3059.env, _t3058, 2)), gem_string("\n"));
        } else {
#line 1114 "compiler/main.gem"
    GemVal _t3060[] = {gem_v_p};
    GemVal _t3061[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3060, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3061, 1)), gem_string("    return GEM_NIL;\n")));
        }
    }

#line 1117 "compiler/main.gem"
    GemVal _t3062 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3062, gem_string("else")), GEM_NIL))) {
#line 1118 "compiler/main.gem"
    GemVal _t3063[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3063, 1), gem_string("} else {\n")));
#line 1119 "compiler/main.gem"
    GemVal _t3064 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t3064, gem_string("else"));
#line 1120 "compiler/main.gem"
    GemVal _t3065[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3065, 1), gem_int(0)))) {
#line 1121 "compiler/main.gem"
            GemVal gem_v__for_i_54 = gem_int(0);
#line 1121 "compiler/main.gem"
    GemVal _t3066[] = {gem_v_else_body};
            GemVal gem_v__for_limit_54 = gem_sub(gem_len_fn(NULL, _t3066, 1), gem_int(1));
#line 1121 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_v__for_limit_54))) break;
#line 1121 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_54;
#line 1121 "compiler/main.gem"
                gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 1122 "compiler/main.gem"
    GemVal _t3067[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3068 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3068.fn(_t3068.env, _t3067, 2)), gem_string("\n"));
            }

#line 1124 "compiler/main.gem"
    GemVal _t3069[] = {gem_v_else_body};
    GemVal _t3070[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t3069, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3071 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3071.fn(_t3071.env, _t3070, 2)), gem_string("\n"));
        } else {
#line 1126 "compiler/main.gem"
    GemVal _t3072[] = {gem_v_p};
    GemVal _t3073[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3072, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3073, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1129 "compiler/main.gem"
    GemVal _t3074[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3074, 1), gem_string("} else {\n")));
#line 1130 "compiler/main.gem"
    GemVal _t3075[] = {gem_v_p};
    GemVal _t3076[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3075, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3076, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1132 "compiler/main.gem"
    GemVal _t3077[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3077, 1), gem_string("}")));
#line 1133 "compiler/main.gem"
    GemVal gem_v__for_i_55 = gem_int(1);
#line 1133 "compiler/main.gem"
    GemVal _t3078 = gem_v_node;
    GemVal _t3079[] = {gem_table_get(_t3078, gem_string("whens"))};
    GemVal gem_v__for_limit_55 = gem_len_fn(NULL, _t3079, 1);
#line 1133 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_v__for_limit_55))) break;
#line 1133 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_55;
#line 1133 "compiler/main.gem"
        gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1134 "compiler/main.gem"
    GemVal _t3080[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("\n"), gem_to_string_fn(NULL, _t3080, 1)), gem_string("}")));
    }

    GemVal _t3081 = gem_v_out;
    gem_pop_frame();
    return _t3081;
}

struct _closure__anon_48 {
    GemVal *gem_v_extern_c_decls;
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_48(void *_env, GemVal *args, int argc) {
    struct _closure__anon_48 *_cls = (struct _closure__anon_48 *)_env;
    GemVal *gem_v_extern_c_decls = _cls->gem_v_extern_c_decls;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_48", "compiler/main.gem", 0);
#line 1142 "compiler/main.gem"
    GemVal gem_v__d56 = gem_v_node;
#line 1142 "compiler/main.gem"
    GemVal _t3083 = gem_v__d56;
    GemVal gem_v_name = gem_table_get(_t3083, gem_string("name"));
#line 1142 "compiler/main.gem"
    GemVal _t3084 = gem_v__d56;
    GemVal gem_v_params = gem_table_get(_t3084, gem_string("params"));
#line 1142 "compiler/main.gem"
    GemVal _t3085 = gem_v__d56;
    GemVal gem_v_ret_type = gem_table_get(_t3085, gem_string("ret_type"));

#line 1144 "compiler/main.gem"
    GemVal _t3086[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3086, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1146 "compiler/main.gem"
    GemVal _t3087 = gem_table_new();
    GemVal gem_v_c_args = _t3087;
#line 1147 "compiler/main.gem"
    GemVal gem_v__for_i_57 = gem_int(0);
#line 1147 "compiler/main.gem"
    GemVal _t3088[] = {gem_v_params};
    GemVal gem_v__for_limit_57 = gem_len_fn(NULL, _t3088, 1);
#line 1147 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_v__for_limit_57))) break;
#line 1147 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_57;
#line 1147 "compiler/main.gem"
        gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
#line 1148 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1149 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1150 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1151 "compiler/main.gem"
    GemVal _t3089[] = {gem_v_i};
    GemVal _t3090[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t3089, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3090, 1)), gem_string("].ival;\n")));
#line 1152 "compiler/main.gem"
    GemVal _t3091[] = {gem_v_i};
    GemVal _t3092[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3091, 1))};
            (void)(gem_push_fn(NULL, _t3092, 2));
        } else {
#line 1153 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1154 "compiler/main.gem"
    GemVal _t3093[] = {gem_v_i};
    GemVal _t3094[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t3093, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3094, 1)), gem_string("].fval;\n")));
#line 1155 "compiler/main.gem"
    GemVal _t3095[] = {gem_v_i};
    GemVal _t3096[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3095, 1))};
                (void)(gem_push_fn(NULL, _t3096, 2));
            } else {
#line 1156 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1157 "compiler/main.gem"
    GemVal _t3097[] = {gem_v_i};
    GemVal _t3098[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t3097, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3098, 1)), gem_string("].sval;\n")));
#line 1158 "compiler/main.gem"
    GemVal _t3099[] = {gem_v_i};
    GemVal _t3100[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3099, 1))};
                    (void)(gem_push_fn(NULL, _t3100, 2));
                } else {
#line 1159 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1160 "compiler/main.gem"
    GemVal _t3101[] = {gem_v_i};
    GemVal _t3102[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t3101, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3102, 1)), gem_string("].bval;\n")));
#line 1161 "compiler/main.gem"
    GemVal _t3103[] = {gem_v_i};
    GemVal _t3104[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3103, 1))};
                        (void)(gem_push_fn(NULL, _t3104, 2));
                    } else {
#line 1162 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1163 "compiler/main.gem"
    GemVal _t3105[] = {gem_v_i};
    GemVal _t3106[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t3105, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t3106, 1)), gem_string("].ival;\n")));
#line 1164 "compiler/main.gem"
    GemVal _t3107[] = {gem_v_i};
    GemVal _t3108[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3107, 1))};
                            (void)(gem_push_fn(NULL, _t3108, 2));
                        } else {
#line 1165 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1166 "compiler/main.gem"
    GemVal _t3109[] = {gem_v_i};
    GemVal _t3110[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3109, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3110, 2));
                            } else {
#line 1168 "compiler/main.gem"
    GemVal _t3111[] = {gem_v_i};
    GemVal _t3112[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3111, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3112, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1173 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1174 "compiler/main.gem"
    GemVal gem_v__for_i_58 = gem_int(0);
#line 1174 "compiler/main.gem"
    GemVal _t3113[] = {gem_v_c_args};
    GemVal gem_v__for_limit_58 = gem_len_fn(NULL, _t3113, 1);
#line 1174 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_v__for_limit_58))) break;
#line 1174 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_58;
#line 1174 "compiler/main.gem"
        gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1175 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1176 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1178 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1181 "compiler/main.gem"
    GemVal _t3114[] = {gem_v_name};
    GemVal _t3115[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3114, 1), gem_string("(")), gem_to_string_fn(NULL, _t3115, 1)), gem_string(")"));
#line 1183 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1184 "compiler/main.gem"
    GemVal _t3116[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3116, 1)), gem_string(";\n")));
#line 1185 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
#line 1186 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1187 "compiler/main.gem"
    GemVal _t3117[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t3117, 1)), gem_string(";\n")));
        } else {
#line 1188 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1189 "compiler/main.gem"
    GemVal _t3118[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t3118, 1)), gem_string(";\n")));
#line 1190 "compiler/main.gem"
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
#line 1191 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1192 "compiler/main.gem"
    GemVal _t3119[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t3119, 1)), gem_string(";\n")));
#line 1193 "compiler/main.gem"
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
#line 1194 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1195 "compiler/main.gem"
    GemVal _t3120[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t3120, 1)), gem_string(";\n")));
#line 1196 "compiler/main.gem"
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
#line 1197 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1198 "compiler/main.gem"
    GemVal _t3121[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t3121, 1)), gem_string(";\n")));
#line 1199 "compiler/main.gem"
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
#line 1200 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1201 "compiler/main.gem"
    GemVal _t3122[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t3122, 1)), gem_string(";\n")));
#line 1202 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
#line 1204 "compiler/main.gem"
    GemVal _t3123[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3123, 1)), gem_string(";\n")));
#line 1205 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1208 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1211 "compiler/main.gem"
    GemVal _t3124[] = {gem_v_name};
    GemVal _t3125[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3124, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t3125, 2));
#line 1214 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1215 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1216 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1217 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1218 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1219 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1220 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1221 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1222 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1223 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1224 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1225 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1226 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1229 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1230 "compiler/main.gem"
    GemVal _t3126[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3126, 1), gem_int(0)))) {
#line 1231 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1233 "compiler/main.gem"
        GemVal gem_v__for_i_59 = gem_int(0);
#line 1233 "compiler/main.gem"
    GemVal _t3127[] = {gem_v_params};
        GemVal gem_v__for_limit_59 = gem_len_fn(NULL, _t3127, 1);
#line 1233 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_v__for_limit_59))) break;
#line 1233 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_59;
#line 1233 "compiler/main.gem"
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1234 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1235 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1237 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1238 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1239 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1240 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1241 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1242 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1243 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1244 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1245 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1246 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1247 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1248 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1249 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1251 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1256 "compiler/main.gem"
    GemVal _t3128[] = {gem_v_c_ret};
    GemVal _t3129[] = {gem_v_name};
    GemVal _t3130[] = {gem_v_c_params};
    GemVal _t3131[] = {(*gem_v_extern_c_decls), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3128, 1), gem_string(" ")), gem_to_string_fn(NULL, _t3129, 1)), gem_string("(")), gem_to_string_fn(NULL, _t3130, 1)), gem_string(");"))};
    (void)(gem_push_fn(NULL, _t3131, 2));
    GemVal _t3132 = gem_v_lines;
    gem_pop_frame();
    return _t3132;
}

struct _closure__anon_49 {
    GemVal *gem_v_mangle;
};
static GemVal _anon_49(void *_env, GemVal *args, int argc) {
    struct _closure__anon_49 *_cls = (struct _closure__anon_49 *)_env;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_cap_set = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_49", "compiler/main.gem", 0);
#line 1264 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1266 "compiler/main.gem"
    GemVal gem_v__for_i_60 = gem_int(0);
#line 1266 "compiler/main.gem"
    GemVal _t3134[] = {gem_v_params};
    GemVal gem_v__for_limit_60 = gem_len_fn(NULL, _t3134, 1);
#line 1266 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1266 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_60;
#line 1266 "compiler/main.gem"
        gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1267 "compiler/main.gem"
    GemVal _t3135[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t3136 = (*gem_v_mangle);
        GemVal gem_v_mp = _t3136.fn(_t3136.env, _t3135, 1);
#line 1268 "compiler/main.gem"
    GemVal _t3137[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3137, 2))) {
#line 1269 "compiler/main.gem"
    GemVal _t3138[] = {gem_v_mp};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3138, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1270 "compiler/main.gem"
    GemVal _t3139[] = {gem_v_mp};
    GemVal _t3140[] = {gem_v_i};
    GemVal _t3141[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3139, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t3140, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t3141, 1)), gem_string("] : GEM_NIL;\n")));
        } else {
#line 1272 "compiler/main.gem"
    GemVal _t3142[] = {gem_v_mp};
    GemVal _t3143[] = {gem_v_i};
    GemVal _t3144[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3142, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t3143, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t3144, 1)), gem_string("] : GEM_NIL;\n")));
        }
    }

#line 1277 "compiler/main.gem"
    GemVal _t3145 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3145, gem_string("rest_param")), GEM_NIL))) {
#line 1278 "compiler/main.gem"
    GemVal _t3146[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t3146, 1);
#line 1279 "compiler/main.gem"
        GemVal gem_v_end_count = gem_int(0);
#line 1280 "compiler/main.gem"
    GemVal _t3147 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3147, gem_string("block_param")), GEM_NIL))) {
#line 1281 "compiler/main.gem"
            gem_v_end_count = gem_int(1);
        }
#line 1283 "compiler/main.gem"
    GemVal _t3148 = gem_v_node;
    GemVal _t3149[] = {gem_table_get(_t3148, gem_string("rest_param"))};
    GemVal _t3150 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t3150.fn(_t3150.env, _t3149, 1);
#line 1284 "compiler/main.gem"
    GemVal _t3151 = gem_v_node;
    GemVal _t3152[] = {gem_v_cap_set, gem_table_get(_t3151, gem_string("rest_param"))};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3152, 2))) {
#line 1285 "compiler/main.gem"
    GemVal _t3153[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3153, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1286 "compiler/main.gem"
    GemVal _t3154[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3154, 1)), gem_string(" = gem_table_new();\n")));
#line 1287 "compiler/main.gem"
    GemVal _t3155[] = {gem_v_N};
    GemVal _t3156[] = {gem_v_end_count};
    GemVal _t3157[] = {gem_v_mp_rest};
    GemVal _t3158[] = {gem_v_N};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t3155, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t3156, 1)), gem_string("; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t3157, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t3158, 1)), gem_string("), args[_ri]);\n")));
        } else {
#line 1289 "compiler/main.gem"
    GemVal _t3159[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3159, 1)), gem_string(" = gem_table_new();\n")));
#line 1290 "compiler/main.gem"
    GemVal _t3160[] = {gem_v_N};
    GemVal _t3161[] = {gem_v_end_count};
    GemVal _t3162[] = {gem_v_mp_rest};
    GemVal _t3163[] = {gem_v_N};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t3160, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t3161, 1)), gem_string("; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t3162, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t3163, 1)), gem_string("), args[_ri]);\n")));
        }
#line 1292 "compiler/main.gem"
    GemVal _t3164 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3164, gem_string("block_param")), GEM_NIL))) {
#line 1293 "compiler/main.gem"
    GemVal _t3165 = gem_v_node;
    GemVal _t3166[] = {gem_table_get(_t3165, gem_string("block_param"))};
    GemVal _t3167 = (*gem_v_mangle);
            GemVal gem_v_mp_block = _t3167.fn(_t3167.env, _t3166, 1);
#line 1294 "compiler/main.gem"
    GemVal _t3168 = gem_v_node;
    GemVal _t3169[] = {gem_v_cap_set, gem_table_get(_t3168, gem_string("block_param"))};
            if (gem_truthy(gem_fn_set_contains(NULL, _t3169, 2))) {
#line 1295 "compiler/main.gem"
    GemVal _t3170[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3170, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1296 "compiler/main.gem"
    GemVal _t3171[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3171, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            } else {
#line 1298 "compiler/main.gem"
    GemVal _t3172[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3172, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            }
        }
    }
    GemVal _t3173 = gem_v_out;
    gem_pop_frame();
    return _t3173;
}

struct _closure__anon_50 {
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
};
static GemVal _anon_50(void *_env, GemVal *args, int argc) {
    struct _closure__anon_50 *_cls = (struct _closure__anon_50 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal gem_v_body = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_50", "compiler/main.gem", 0);
#line 1308 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1309 "compiler/main.gem"
    GemVal _t3175[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3175, 1), gem_int(0)))) {
#line 1310 "compiler/main.gem"
        GemVal gem_v__for_i_61 = gem_int(0);
#line 1310 "compiler/main.gem"
    GemVal _t3176[] = {gem_v_body};
        GemVal gem_v__for_limit_61 = gem_sub(gem_len_fn(NULL, _t3176, 1), gem_int(1));
#line 1310 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_v__for_limit_61))) break;
#line 1310 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_61;
#line 1310 "compiler/main.gem"
            gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1311 "compiler/main.gem"
    GemVal _t3177[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t3178 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3178.fn(_t3178.env, _t3177, 2)), gem_string("\n"));
        }

#line 1313 "compiler/main.gem"
    GemVal _t3179[] = {gem_v_body};
    GemVal _t3180[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t3179, 1), gem_int(1))), gem_int(1)};
    GemVal _t3181 = (*gem_v_compile_stmt_return);
        gem_v_out = gem_add(gem_add(gem_v_out, _t3181.fn(_t3181.env, _t3180, 2)), gem_string("\n"));
    } else {
#line 1315 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
    GemVal _t3182 = gem_v_out;
    gem_pop_frame();
    return _t3182;
}

struct _closure__anon_51 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_source_name;
};
static GemVal _anon_51(void *_env, GemVal *args, int argc) {
    struct _closure__anon_51 *_cls = (struct _closure__anon_51 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_emit_fn_body = _cls->gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings = _cls->gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_51", "compiler/main.gem", 0);
#line 1323 "compiler/main.gem"
    GemVal gem_v__d62 = gem_v_node;
#line 1323 "compiler/main.gem"
    GemVal _t3184 = gem_v__d62;
    GemVal gem_v_params = gem_table_get(_t3184, gem_string("params"));
#line 1323 "compiler/main.gem"
    GemVal _t3185 = gem_v__d62;
    GemVal gem_v_body = gem_table_get(_t3185, gem_string("body"));
#line 1323 "compiler/main.gem"
    GemVal _t3186 = gem_v__d62;
    GemVal gem_v_name = gem_table_get(_t3186, gem_string("name"));

#line 1326 "compiler/main.gem"
    GemVal _t3187[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t3187, 1);
#line 1327 "compiler/main.gem"
    GemVal _t3188 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3188, gem_string("rest_param")), GEM_NIL))) {
#line 1328 "compiler/main.gem"
    GemVal _t3189 = gem_v_node;
    GemVal _t3190[] = {gem_v_scope_vars, gem_table_get(_t3189, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t3190, 2));
    }
#line 1330 "compiler/main.gem"
    GemVal _t3191 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3191, gem_string("block_param")), GEM_NIL))) {
#line 1331 "compiler/main.gem"
    GemVal _t3192 = gem_v_node;
    GemVal _t3193[] = {gem_v_scope_vars, gem_table_get(_t3192, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t3193, 2));
    }
#line 1333 "compiler/main.gem"
    GemVal _t3194[] = {gem_v_body, gem_v_scope_vars};
    (void)(gem_fn_collect_top_let_names(NULL, _t3194, 2));
#line 1334 "compiler/main.gem"
    GemVal _t3195[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t3196 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t3196.fn(_t3196.env, _t3195, 2);
#line 1336 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1337 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1338 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1339 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1341 "compiler/main.gem"
    GemVal _t3197[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3197, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1344 "compiler/main.gem"
    GemVal _t3198 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3198, gem_string("line")), GEM_NIL))) {
#line 1345 "compiler/main.gem"
    GemVal _t3199 = gem_v_node;
    GemVal _t3200[] = {gem_table_get(_t3199, gem_string("line"))};
    GemVal _t3201[] = {(*gem_v_source_name)};
    GemVal _t3202[] = {gem_fn_escape_c_string(NULL, _t3201, 1)};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t3200, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t3202, 1)), gem_string("\"\n")));
    }
#line 1348 "compiler/main.gem"
    GemVal _t3203[] = {gem_v_node, gem_v_params, gem_v_captured};
    GemVal _t3204 = (*gem_v_emit_param_bindings);
    gem_v_lines = gem_add(gem_v_lines, _t3204.fn(_t3204.env, _t3203, 3));
#line 1350 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1351 "compiler/main.gem"
    GemVal _t3205 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3205, gem_string("line")), GEM_NIL))) {
#line 1352 "compiler/main.gem"
    GemVal _t3206 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t3206, gem_string("line"));
    }
#line 1354 "compiler/main.gem"
    GemVal _t3207[] = {gem_v_name};
    GemVal _t3208[] = {gem_fn_escape_c_string(NULL, _t3207, 1)};
    GemVal _t3209[] = {(*gem_v_source_name)};
    GemVal _t3210[] = {gem_fn_escape_c_string(NULL, _t3209, 1)};
    GemVal _t3211[] = {gem_v_fn_line};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t3208, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t3210, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t3211, 1)), gem_string(");\n")));
#line 1356 "compiler/main.gem"
    GemVal _t3212[] = {gem_v_body};
    GemVal _t3213 = (*gem_v_emit_fn_body);
    gem_v_lines = gem_add(gem_v_lines, _t3213.fn(_t3213.env, _t3212, 1));
#line 1358 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1359 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1360 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
    GemVal _t3214 = gem_v_lines;
    gem_pop_frame();
    return _t3214;
}

struct _closure__anon_52 {
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
static GemVal _anon_52(void *_env, GemVal *args, int argc) {
    struct _closure__anon_52 *_cls = (struct _closure__anon_52 *)_env;
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
    gem_push_frame("_anon_52", "compiler/main.gem", 0);
#line 1367 "compiler/main.gem"
    GemVal gem_v__d63 = gem_v_node;
#line 1367 "compiler/main.gem"
    GemVal _t3216 = gem_v__d63;
    GemVal gem_v_params = gem_table_get(_t3216, gem_string("params"));
#line 1367 "compiler/main.gem"
    GemVal _t3217 = gem_v__d63;
    GemVal gem_v_body = gem_table_get(_t3217, gem_string("body"));

#line 1368 "compiler/main.gem"
    GemVal _t3218 = (*gem_v_anon_name);
    GemVal gem_v_name = _t3218.fn(_t3218.env, NULL, 0);
#line 1371 "compiler/main.gem"
    GemVal _t3219[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t3219, 1);
#line 1372 "compiler/main.gem"
    GemVal _t3220 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3220, gem_string("rest_param")), GEM_NIL))) {
#line 1373 "compiler/main.gem"
    GemVal _t3221 = gem_v_node;
    GemVal _t3222[] = {gem_v_inner_scope, gem_table_get(_t3221, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t3222, 2));
    }
#line 1375 "compiler/main.gem"
    GemVal _t3223 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3223, gem_string("block_param")), GEM_NIL))) {
#line 1376 "compiler/main.gem"
    GemVal _t3224 = gem_v_node;
    GemVal _t3225[] = {gem_v_inner_scope, gem_table_get(_t3224, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t3225, 2));
    }
#line 1378 "compiler/main.gem"
    GemVal _t3226[] = {gem_v_body, gem_v_inner_scope};
    (void)(gem_fn_collect_top_let_names(NULL, _t3226, 2));
#line 1379 "compiler/main.gem"
    GemVal _t3227[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t3228 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t3228.fn(_t3228.env, _t3227, 2);
#line 1381 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1382 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1383 "compiler/main.gem"
    GemVal _t3229[] = {gem_v_captures};
    GemVal _t3230[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t3229, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t3230, 2);
#line 1384 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1387 "compiler/main.gem"
    GemVal _t3231[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t3231, 1));
#line 1388 "compiler/main.gem"
    GemVal _t3232[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t3232, 1)), gem_string(" {\n"));
#line 1389 "compiler/main.gem"
    GemVal gem_v__for_items_64 = gem_v_captures;
#line 1389 "compiler/main.gem"
    GemVal gem_v__for_i_64 = gem_int(0);
#line 1389 "compiler/main.gem"
    while (1) {
        GemVal _t3233[] = {gem_v__for_items_64};
        if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_len_fn(NULL, _t3233, 1)))) break;
#line 1389 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_64, gem_v__for_i_64);
#line 1389 "compiler/main.gem"
        gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1390 "compiler/main.gem"
    GemVal _t3234[] = {gem_v_cap};
    GemVal _t3235 = (*gem_v_mangle);
    GemVal _t3236[] = {_t3235.fn(_t3235.env, _t3234, 1)};
        gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3236, 1)), gem_string(";\n")));
    }

#line 1392 "compiler/main.gem"
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
#line 1395 "compiler/main.gem"
    GemVal _t3237[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t3237, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1396 "compiler/main.gem"
    GemVal _t3238[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3238, 1), gem_int(0)))) {
#line 1397 "compiler/main.gem"
    GemVal _t3239[] = {gem_v_struct_name};
    GemVal _t3240[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t3239, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t3240, 1)), gem_string(" *)_env;\n")));
#line 1398 "compiler/main.gem"
        GemVal gem_v__for_items_65 = gem_v_captures;
#line 1398 "compiler/main.gem"
        GemVal gem_v__for_i_65 = gem_int(0);
#line 1398 "compiler/main.gem"
        while (1) {
            GemVal _t3241[] = {gem_v__for_items_65};
            if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_len_fn(NULL, _t3241, 1)))) break;
#line 1398 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_65, gem_v__for_i_65);
#line 1398 "compiler/main.gem"
            gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1399 "compiler/main.gem"
    GemVal _t3242[] = {gem_v_cap};
    GemVal _t3243 = (*gem_v_mangle);
            GemVal gem_v_mc = _t3243.fn(_t3243.env, _t3242, 1);
#line 1400 "compiler/main.gem"
    GemVal _t3244[] = {gem_v_mc};
    GemVal _t3245[] = {gem_v_mc};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3244, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t3245, 1)), gem_string(";\n")));
        }

    }
#line 1404 "compiler/main.gem"
    GemVal _t3246[] = {gem_v_node, gem_v_params, gem_v_inner_captured};
    GemVal _t3247 = (*gem_v_emit_param_bindings);
    gem_v_lines = gem_add(gem_v_lines, _t3247.fn(_t3247.env, _t3246, 3));
#line 1406 "compiler/main.gem"
    GemVal _t3248[] = {gem_v_name};
    GemVal _t3249[] = {gem_fn_escape_c_string(NULL, _t3248, 1)};
    GemVal _t3250[] = {(*gem_v_source_name)};
    GemVal _t3251[] = {gem_fn_escape_c_string(NULL, _t3250, 1)};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t3249, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t3251, 1)), gem_string("\", 0);\n")));
#line 1408 "compiler/main.gem"
    GemVal _t3252[] = {gem_v_body};
    GemVal _t3253 = (*gem_v_emit_fn_body);
    gem_v_lines = gem_add(gem_v_lines, _t3253.fn(_t3253.env, _t3252, 1));
#line 1410 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1411 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1412 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1415 "compiler/main.gem"
    GemVal _t3254[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t3254, 2));
    GemVal _t3255 = gem_table_new();
    gem_table_set(_t3255, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t3255, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t3255, gem_string("captures"), gem_v_captures);
    GemVal _t3256 = _t3255;
    gem_pop_frame();
    return _t3256;
}

struct _closure__anon_53 {
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
static GemVal _anon_53(void *_env, GemVal *args, int argc) {
    struct _closure__anon_53 *_cls = (struct _closure__anon_53 *)_env;
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
    gem_push_frame("_anon_53", "compiler/main.gem", 0);
#line 1424 "compiler/main.gem"
    GemVal _t3258 = gem_table_new();
    GemVal gem_v_fn_defs = _t3258;
#line 1425 "compiler/main.gem"
    GemVal _t3259 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t3259;
#line 1426 "compiler/main.gem"
    GemVal _t3260 = gem_table_new();
    GemVal gem_v_extern_includes = _t3260;
#line 1427 "compiler/main.gem"
    GemVal _t3261 = gem_table_new();
    GemVal gem_v_top_stmts = _t3261;
#line 1429 "compiler/main.gem"
    GemVal _t3262 = gem_v_ast;
    GemVal gem_v__for_items_66 = gem_table_get(_t3262, gem_string("stmts"));
#line 1429 "compiler/main.gem"
    GemVal gem_v__for_i_66 = gem_int(0);
#line 1429 "compiler/main.gem"
    while (1) {
        GemVal _t3263[] = {gem_v__for_items_66};
        if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_len_fn(NULL, _t3263, 1)))) break;
#line 1429 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_66, gem_v__for_i_66);
#line 1429 "compiler/main.gem"
        gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1430 "compiler/main.gem"
    GemVal _t3264 = gem_v_s;
        if (gem_truthy(gem_eq(gem_table_get(_t3264, gem_string("tag")), gem_string("fn_def")))) {
#line 1431 "compiler/main.gem"
    GemVal _t3265[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t3265, 2));
        } else {
#line 1432 "compiler/main.gem"
    GemVal _t3266 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t3266, gem_string("tag")), gem_string("extern_fn")))) {
#line 1433 "compiler/main.gem"
    GemVal _t3267[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t3267, 2));
            } else {
#line 1434 "compiler/main.gem"
    GemVal _t3268 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t3268, gem_string("tag")), gem_string("extern_include")))) {
#line 1435 "compiler/main.gem"
    GemVal _t3269[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t3269, 2));
                } else {
#line 1437 "compiler/main.gem"
    GemVal _t3270[] = {gem_v_top_stmts, gem_v_s};
                    (void)(gem_push_fn(NULL, _t3270, 2));
                }
            }
        }
    }

#line 1442 "compiler/main.gem"
    GemVal gem_v__for_items_67 = gem_v_extern_fns_list;
#line 1442 "compiler/main.gem"
    GemVal gem_v__for_i_67 = gem_int(0);
#line 1442 "compiler/main.gem"
    while (1) {
        GemVal _t3271[] = {gem_v__for_items_67};
        if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_len_fn(NULL, _t3271, 1)))) break;
#line 1442 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_67, gem_v__for_i_67);
#line 1442 "compiler/main.gem"
        gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1443 "compiler/main.gem"
    GemVal _t3272 = gem_v_ef;
    GemVal _t3273[] = {(*gem_v_defined_fns), gem_table_get(_t3272, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t3273, 2));
    }

#line 1447 "compiler/main.gem"
    GemVal gem_v__for_items_68 = gem_v_fn_defs;
#line 1447 "compiler/main.gem"
    GemVal gem_v__for_i_68 = gem_int(0);
#line 1447 "compiler/main.gem"
    while (1) {
        GemVal _t3274[] = {gem_v__for_items_68};
        if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_len_fn(NULL, _t3274, 1)))) break;
#line 1447 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_68, gem_v__for_i_68);
#line 1447 "compiler/main.gem"
        gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1448 "compiler/main.gem"
    GemVal _t3275 = gem_v_fd;
    GemVal _t3276[] = {(*gem_v_defined_fns), gem_table_get(_t3275, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t3276, 2));
#line 1449 "compiler/main.gem"
    GemVal _t3277 = gem_v_fd;
    GemVal _t3278[] = {gem_table_get(_t3277, gem_string("name"))};
        GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3278, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1450 "compiler/main.gem"
    GemVal _t3279[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t3279, 2));
    }

#line 1454 "compiler/main.gem"
    GemVal _t3280[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn_collect_top_let_names(NULL, _t3280, 2));
#line 1455 "compiler/main.gem"
    GemVal _t3281 = gem_table_new();
    GemVal _t3282[] = {gem_v_top_stmts, _t3281};
    GemVal _t3283 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t3283.fn(_t3283.env, _t3282, 2);
#line 1458 "compiler/main.gem"
    GemVal gem_v__for_items_69 = gem_v_extern_fns_list;
#line 1458 "compiler/main.gem"
    GemVal gem_v__for_i_69 = gem_int(0);
#line 1458 "compiler/main.gem"
    while (1) {
        GemVal _t3284[] = {gem_v__for_items_69};
        if (!gem_truthy(gem_lt(gem_v__for_i_69, gem_len_fn(NULL, _t3284, 1)))) break;
#line 1458 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_69, gem_v__for_i_69);
#line 1458 "compiler/main.gem"
        gem_v__for_i_69 = gem_add(gem_v__for_i_69, gem_int(1));
#line 1459 "compiler/main.gem"
    GemVal _t3285[] = {gem_v_ef};
    GemVal _t3286 = (*gem_v_compile_extern_fn);
    GemVal _t3287[] = {(*gem_v_functions), _t3286.fn(_t3286.env, _t3285, 1)};
        (void)(gem_push_fn(NULL, _t3287, 2));
    }

#line 1463 "compiler/main.gem"
    GemVal gem_v__for_items_70 = gem_v_fn_defs;
#line 1463 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1463 "compiler/main.gem"
    while (1) {
        GemVal _t3288[] = {gem_v__for_items_70};
        if (!gem_truthy(gem_lt(gem_v__for_i_70, gem_len_fn(NULL, _t3288, 1)))) break;
#line 1463 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_70, gem_v__for_i_70);
#line 1463 "compiler/main.gem"
        gem_v__for_i_70 = gem_add(gem_v__for_i_70, gem_int(1));
#line 1464 "compiler/main.gem"
    GemVal _t3289[] = {gem_v_fd};
    GemVal _t3290 = (*gem_v_compile_fn);
    GemVal _t3291[] = {(*gem_v_functions), _t3290.fn(_t3290.env, _t3289, 1)};
        (void)(gem_push_fn(NULL, _t3291, 2));
    }

#line 1468 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1469 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1470 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1471 "compiler/main.gem"
    GemVal _t3292[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t3293 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t3293.fn(_t3293.env, _t3292, 2);
#line 1472 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1473 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1475 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 1476 "compiler/main.gem"
    GemVal gem_v__for_items_71 = gem_v_fn_defs;
#line 1476 "compiler/main.gem"
    GemVal gem_v__for_i_71 = gem_int(0);
#line 1476 "compiler/main.gem"
    while (1) {
        GemVal _t3294[] = {gem_v__for_items_71};
        if (!gem_truthy(gem_lt(gem_v__for_i_71, gem_len_fn(NULL, _t3294, 1)))) break;
#line 1476 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_71, gem_v__for_i_71);
#line 1476 "compiler/main.gem"
        gem_v__for_i_71 = gem_add(gem_v__for_i_71, gem_int(1));
#line 1477 "compiler/main.gem"
    GemVal _t3295 = gem_v_fd;
        if (gem_truthy(gem_eq(gem_table_get(_t3295, gem_string("name")), gem_string("main")))) {
#line 1478 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 1482 "compiler/main.gem"
    GemVal gem_v_main_c = gem_string("int main(void) {\n");
#line 1483 "compiler/main.gem"
    GemVal _t3296[] = {(*gem_v_source_name)};
    GemVal _t3297[] = {gem_fn_escape_c_string(NULL, _t3296, 1)};
    gem_v_main_c = gem_add(gem_v_main_c, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t3297, 1)), gem_string("\", 0);\n")));
#line 1484 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1485 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
#line 1487 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1488 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
#line 1489 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
#line 1491 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_run_scheduler();\n"));
#line 1492 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_pop_frame();\n    return 0;\n}\n"));
#line 1496 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1497 "compiler/main.gem"
    GemVal gem_v__for_items_72 = gem_v_extern_includes;
#line 1497 "compiler/main.gem"
    GemVal gem_v__for_i_72 = gem_int(0);
#line 1497 "compiler/main.gem"
    while (1) {
        GemVal _t3298[] = {gem_v__for_items_72};
        if (!gem_truthy(gem_lt(gem_v__for_i_72, gem_len_fn(NULL, _t3298, 1)))) break;
#line 1497 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_72, gem_v__for_i_72);
#line 1497 "compiler/main.gem"
        gem_v__for_i_72 = gem_add(gem_v__for_i_72, gem_int(1));
#line 1498 "compiler/main.gem"
    GemVal _t3299 = gem_v_ei;
    GemVal _t3300[] = {gem_table_get(_t3299, gem_string("path"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t3300, 1)), gem_string("\"\n")));
    }

#line 1500 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
#line 1504 "compiler/main.gem"
    GemVal _t3301[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3301, 1), gem_int(0)))) {
#line 1505 "compiler/main.gem"
        GemVal gem_v__for_i_73 = gem_int(0);
#line 1505 "compiler/main.gem"
    GemVal _t3302[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_73 = gem_len_fn(NULL, _t3302, 1);
#line 1505 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_73, gem_v__for_limit_73))) break;
#line 1505 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_73;
#line 1505 "compiler/main.gem"
            gem_v__for_i_73 = gem_add(gem_v__for_i_73, gem_int(1));
#line 1506 "compiler/main.gem"
            gem_v_out = gem_add(gem_add(gem_v_out, gem_table_get((*gem_v_extern_c_decls), gem_v_i)), gem_string("\n"));
        }

    }
#line 1511 "compiler/main.gem"
    GemVal gem_v__for_i_74 = gem_int(0);
#line 1511 "compiler/main.gem"
    GemVal _t3303[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_74 = gem_len_fn(NULL, _t3303, 1);
#line 1511 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_74, gem_v__for_limit_74))) break;
#line 1511 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_74;
#line 1511 "compiler/main.gem"
        gem_v__for_i_74 = gem_add(gem_v__for_i_74, gem_int(1));
#line 1512 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
#line 1513 "compiler/main.gem"
    GemVal _t3304[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3304, 1), gem_int(1))))) {
#line 1514 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1517 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1520 "compiler/main.gem"
    GemVal _t3305[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t3305, 1);
#line 1521 "compiler/main.gem"
    GemVal gem_v__for_i_75 = gem_int(0);
#line 1521 "compiler/main.gem"
    GemVal _t3306[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_75 = gem_len_fn(NULL, _t3306, 1);
#line 1521 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_75, gem_v__for_limit_75))) break;
#line 1521 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_75;
#line 1521 "compiler/main.gem"
        gem_v__for_i_75 = gem_add(gem_v__for_i_75, gem_int(1));
#line 1522 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 1523 "compiler/main.gem"
    GemVal _t3307[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3307, 2))) {
#line 1524 "compiler/main.gem"
    GemVal _t3308[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal *gem_v_"), gem_to_string_fn(NULL, _t3308, 1)), gem_string(";\n")));
        } else {
#line 1526 "compiler/main.gem"
    GemVal _t3309[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal gem_v_"), gem_to_string_fn(NULL, _t3309, 1)), gem_string(";\n")));
        }
    }

#line 1529 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1532 "compiler/main.gem"
    GemVal gem_v__for_i_76 = gem_int(0);
#line 1532 "compiler/main.gem"
    GemVal _t3310[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_76 = gem_len_fn(NULL, _t3310, 1);
#line 1532 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_76, gem_v__for_limit_76))) break;
#line 1532 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_76;
#line 1532 "compiler/main.gem"
        gem_v__for_i_76 = gem_add(gem_v__for_i_76, gem_int(1));
#line 1533 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
#line 1534 "compiler/main.gem"
    GemVal _t3311[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3311, 1), gem_int(1))))) {
#line 1535 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
        }
    }

#line 1538 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
#line 1540 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    GemVal _t3312 = gem_v_out;
    gem_pop_frame();
    return _t3312;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 140 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_codegen", "compiler/main.gem", 140);
#line 141 "compiler/main.gem"
    GemVal _t1432 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1432;
#line 142 "compiler/main.gem"
    GemVal _t1433 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1433;
#line 143 "compiler/main.gem"
    GemVal _t1434 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1434;
#line 144 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 145 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 146 "compiler/main.gem"
    GemVal _t1435 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1435;
#line 147 "compiler/main.gem"
    GemVal _t1436 = gem_table_new();
    gem_table_set(_t1436, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1436, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1436, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1436, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1436, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1436, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1436, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1436, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1436, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1436, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1436, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1436, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1436, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1436, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1436, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1436, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1436, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1436, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1436, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1436, gem_string("self"), gem_string("gem_self_builtin"));
    gem_table_set(_t1436, gem_string("monitor"), gem_string("gem_monitor_builtin"));
    gem_table_set(_t1436, gem_string("spawn_monitor"), gem_string("gem_spawn_monitor_builtin"));
    gem_table_set(_t1436, gem_string("register"), gem_string("gem_register_builtin"));
    gem_table_set(_t1436, gem_string("whereis"), gem_string("gem_whereis_builtin"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1436;
#line 173 "compiler/main.gem"
    GemVal _t1437 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1437;
#line 174 "compiler/main.gem"
    GemVal gem_v__for_tbl_10 = (*gem_v_builtin_fns);
#line 174 "compiler/main.gem"
    GemVal _t1438[] = {gem_v__for_tbl_10};
    GemVal gem_v__for_keys_10 = gem_keys_fn(NULL, _t1438, 1);
#line 174 "compiler/main.gem"
    GemVal gem_v__for_i_10 = gem_int(0);
#line 174 "compiler/main.gem"
    while (1) {
        GemVal _t1439[] = {gem_v__for_keys_10};
        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t1439, 1)))) break;
#line 174 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_10, gem_v__for_i_10);
#line 174 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_10, gem_table_get(gem_v__for_keys_10, gem_v__for_i_10));
#line 174 "compiler/main.gem"
        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 175 "compiler/main.gem"
        gem_table_set((*gem_v_builtins), gem_v_k, gem_bool(1));
    }

#line 177 "compiler/main.gem"
    GemVal _t1440 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1440;
#line 178 "compiler/main.gem"
    GemVal _t1441 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1441;
#line 179 "compiler/main.gem"
    GemVal _t1442 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1442;
#line 180 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 184 "compiler/main.gem"
    struct _closure__anon_24 *_t1445 = GC_MALLOC(sizeof(struct _closure__anon_24));
    _t1445->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_24, _t1445);
#line 189 "compiler/main.gem"
    struct _closure__anon_25 *_t1448 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1448->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_25, _t1448);
#line 194 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_26, NULL);
#line 200 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 201 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 203 "compiler/main.gem"
    struct _closure__anon_27 *_t1700 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1700->gem_v_builtins = gem_v_builtins;
    _t1700->gem_v_collect_free_node = gem_v_collect_free_node;
    _t1700->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1700->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_27, _t1700);
#line 353 "compiler/main.gem"
    struct _closure__anon_28 *_t1714 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1714->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_28, _t1714);
#line 368 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 369 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 371 "compiler/main.gem"
    struct _closure__anon_29 *_t1856 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t1856->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1856->gem_v_walk_captures = gem_v_walk_captures;
    _t1856->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_29, _t1856);
#line 455 "compiler/main.gem"
    struct _closure__anon_30 *_t1868 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t1868->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_30, _t1868);
#line 466 "compiler/main.gem"
    struct _closure__anon_31 *_t1873 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t1873->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_31, _t1873);
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
    struct _closure__anon_32 *_t2072 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t2072->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2072->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2072->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t2072->gem_v_compile_array = gem_v_compile_array;
    _t2072->gem_v_compile_binop = gem_v_compile_binop;
    _t2072->gem_v_compile_call = gem_v_compile_call;
    _t2072->gem_v_compile_expr = gem_v_compile_expr;
    _t2072->gem_v_compile_table = gem_v_compile_table;
    _t2072->gem_v_defined_fns = gem_v_defined_fns;
    _t2072->gem_v_mangle = gem_v_mangle;
    _t2072->gem_v_tmp = gem_v_tmp;
    _t2072->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_32, _t2072);
#line 579 "compiler/main.gem"
    struct _closure__anon_33 *_t2089 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t2089->gem_v_compile_expr = gem_v_compile_expr;
    _t2089->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_33, _t2089);
#line 594 "compiler/main.gem"
    struct _closure__anon_34 *_t2103 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t2103->gem_v_compile_expr = gem_v_compile_expr;
    _t2103->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_34, _t2103);
#line 608 "compiler/main.gem"
    struct _closure__anon_35 *_t2147 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t2147->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2147->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2147->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t2147->gem_v_mangle = gem_v_mangle;
    _t2147->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_35, _t2147);
#line 643 "compiler/main.gem"
    struct _closure__anon_36 *_t2163 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t2163->gem_v_compile_expr = gem_v_compile_expr;
    _t2163->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_36, _t2163);
#line 664 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_37, NULL);
#line 673 "compiler/main.gem"
    struct _closure__anon_38 *_t2251 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2251->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2251->gem_v_compile_args = gem_v_compile_args;
    _t2251->gem_v_compile_expr = gem_v_compile_expr;
    _t2251->gem_v_defined_fns = gem_v_defined_fns;
    _t2251->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t2251->gem_v_source_name = gem_v_source_name;
    _t2251->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_38, _t2251);
#line 717 "compiler/main.gem"
    struct _closure__anon_39 *_t2404 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2404->gem_v_compile_expr = gem_v_compile_expr;
    _t2404->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_39, _t2404);
#line 796 "compiler/main.gem"
    struct _closure__anon_40 *_t2409 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t2409->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_40, _t2409);
#line 806 "compiler/main.gem"
    struct _closure__anon_41 *_t2652 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2652->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2652->gem_v_compile_expr = gem_v_compile_expr;
    _t2652->gem_v_compile_if = gem_v_compile_if;
    _t2652->gem_v_compile_match = gem_v_compile_match;
    _t2652->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2652->gem_v_compile_while = gem_v_compile_while;
    _t2652->gem_v_in_top_level = gem_v_in_top_level;
    _t2652->gem_v_mangle = gem_v_mangle;
    _t2652->gem_v_source_name = gem_v_source_name;
    _t2652->gem_v_tmp = gem_v_tmp;
    _t2652->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t2652->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_41, _t2652);
#line 880 "compiler/main.gem"
    struct _closure__anon_42 *_t2869 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t2869->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2869->gem_v_compile_expr = gem_v_compile_expr;
    _t2869->gem_v_compile_if_return = gem_v_compile_if_return;
    _t2869->gem_v_compile_match_return = gem_v_compile_match_return;
    _t2869->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2869->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2869->gem_v_compile_while = gem_v_compile_while;
    _t2869->gem_v_mangle = gem_v_mangle;
    _t2869->gem_v_source_name = gem_v_source_name;
    _t2869->gem_v_tmp = gem_v_tmp;
    _t2869->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_42, _t2869);
#line 945 "compiler/main.gem"
    struct _closure__anon_43 *_t2888 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t2888->gem_v_compile_expr = gem_v_compile_expr;
    _t2888->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_43, _t2888);
#line 961 "compiler/main.gem"
    struct _closure__anon_44 *_t2924 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t2924->gem_v_compile_expr = gem_v_compile_expr;
    _t2924->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2924->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_44, _t2924);
#line 996 "compiler/main.gem"
    struct _closure__anon_45 *_t2942 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t2942->gem_v_compile_expr = gem_v_compile_expr;
    _t2942->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_45, _t2942);
#line 1011 "compiler/main.gem"
    struct _closure__anon_46 *_t2999 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t2999->gem_v_compile_expr = gem_v_compile_expr;
    _t2999->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2999->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2999->gem_v_mangle = gem_v_mangle;
    _t2999->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_46, _t2999);
#line 1062 "compiler/main.gem"
    struct _closure__anon_47 *_t3082 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t3082->gem_v_compile_expr = gem_v_compile_expr;
    _t3082->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3082->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3082->gem_v_mangle = gem_v_mangle;
    _t3082->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_47, _t3082);
#line 1141 "compiler/main.gem"
    struct _closure__anon_48 *_t3133 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t3133->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t3133->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_48, _t3133);
#line 1263 "compiler/main.gem"
    struct _closure__anon_49 *_t3174 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t3174->gem_v_mangle = gem_v_mangle;
    GemVal *gem_v_emit_param_bindings = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_param_bindings = gem_make_fn(_anon_49, _t3174);
#line 1307 "compiler/main.gem"
    struct _closure__anon_50 *_t3183 = GC_MALLOC(sizeof(struct _closure__anon_50));
    _t3183->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3183->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_fn_body = gem_make_fn(_anon_50, _t3183);
#line 1322 "compiler/main.gem"
    struct _closure__anon_51 *_t3215 = GC_MALLOC(sizeof(struct _closure__anon_51));
    _t3215->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3215->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t3215->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t3215->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3215->gem_v_in_top_level = gem_v_in_top_level;
    _t3215->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_fn = gem_make_fn(_anon_51, _t3215);
#line 1366 "compiler/main.gem"
    struct _closure__anon_52 *_t3257 = GC_MALLOC(sizeof(struct _closure__anon_52));
    _t3257->gem_v_anon_name = gem_v_anon_name;
    _t3257->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3257->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t3257->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t3257->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3257->gem_v_functions = gem_v_functions;
    _t3257->gem_v_in_top_level = gem_v_in_top_level;
    _t3257->gem_v_mangle = gem_v_mangle;
    _t3257->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_52, _t3257);
#line 1422 "compiler/main.gem"
    struct _closure__anon_53 *_t3313 = GC_MALLOC(sizeof(struct _closure__anon_53));
    _t3313->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3313->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t3313->gem_v_compile_fn = gem_v_compile_fn;
    _t3313->gem_v_compile_stmts = gem_v_compile_stmts;
    _t3313->gem_v_defined_fns = gem_v_defined_fns;
    _t3313->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t3313->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3313->gem_v_forward_decls = gem_v_forward_decls;
    _t3313->gem_v_functions = gem_v_functions;
    _t3313->gem_v_in_top_level = gem_v_in_top_level;
    _t3313->gem_v_source_name = gem_v_source_name;
    _t3313->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t3313->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_53, _t3313);
    GemVal _t3314 = gem_table_new();
    gem_table_set(_t3314, gem_string("compile"), gem_v_compile);
    GemVal _t3315 = _t3314;
    gem_pop_frame();
    return _t3315;
}

static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc) {
#line 1550 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("str_replace", "compiler/main.gem", 1550);
#line 1551 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 1552 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 1553 "compiler/main.gem"
    GemVal _t3316[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t3316, 1);
#line 1554 "compiler/main.gem"
    GemVal _t3317[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t3317, 1);
#line 1555 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
#line 1556 "compiler/main.gem"
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
#line 1557 "compiler/main.gem"
            GemVal gem_v_found = gem_bool(1);
#line 1558 "compiler/main.gem"
            GemVal gem_v_j = gem_int(0);
#line 1559 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
#line 1560 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
#line 1561 "compiler/main.gem"
                    gem_v_found = gem_bool(0);
                    break;
                }
#line 1564 "compiler/main.gem"
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
#line 1566 "compiler/main.gem"
            if (gem_truthy(gem_v_found)) {
#line 1567 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
#line 1568 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
#line 1572 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
#line 1573 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t3318 = gem_v_result;
    gem_pop_frame();
    return _t3318;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 24 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 24);
#line 25 "compiler/main.gem"
    GemVal _t3319 = gem_table_new();
    GemVal gem_v_new_stmts = _t3319;
#line 26 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 27 "compiler/main.gem"
    GemVal _t3320 = gem_v_ast;
    GemVal gem_v__for_items_1 = gem_table_get(_t3320, gem_string("stmts"));
#line 27 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 27 "compiler/main.gem"
    while (1) {
        GemVal _t3321[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t3321, 1)))) break;
#line 27 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 27 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 28 "compiler/main.gem"
    GemVal _t3322[] = {gem_v_stmt};
    GemVal _t3324;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3322, 1), gem_string("table")))) {
        _t3324 = gem_eq(gem_type_fn(NULL, _t3322, 1), gem_string("table"));
    } else {
        GemVal _t3323 = gem_v_stmt;
        _t3324 = gem_eq(gem_table_get(_t3323, gem_string("tag")), gem_string("load"));
    }
        if (gem_truthy(_t3324)) {
#line 29 "compiler/main.gem"
    GemVal _t3325 = gem_v_stmt;
            GemVal gem_v_path = gem_table_get(_t3325, gem_string("path"));
#line 31 "compiler/main.gem"
    GemVal _t3326[] = {gem_v_path};
    GemVal _t3334;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t3326, 1), gem_int(4)))) {
        _t3334 = gem_lt(gem_len_fn(NULL, _t3326, 1), gem_int(4));
    } else {
        GemVal _t3327[] = {gem_v_path};
        GemVal _t3329;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3327, 1), gem_int(4))), gem_string(".")))) {
                _t3329 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3327, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t3328[] = {gem_v_path};
                _t3329 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3328, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t3331;
        if (gem_truthy(_t3329)) {
                _t3331 = _t3329;
        } else {
                GemVal _t3330[] = {gem_v_path};
                _t3331 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3330, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t3333;
        if (gem_truthy(_t3331)) {
                _t3333 = _t3331;
        } else {
                GemVal _t3332[] = {gem_v_path};
                _t3333 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3332, 1), gem_int(1))), gem_string("m"));
        }
        _t3334 = _t3333;
    }
            if (gem_truthy(_t3334)) {
#line 32 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 36 "compiler/main.gem"
    GemVal _t3335[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_fn_gem_path_join(NULL, _t3335, 2);
#line 39 "compiler/main.gem"
    GemVal _t3336[] = {gem_v_full_path};
            if (gem_truthy(gem_eq(gem_fn_gem_file_exists(NULL, _t3336, 1), gem_int(0)))) {
#line 40 "compiler/main.gem"
    GemVal _t3337[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_fn_gem_path_join(NULL, _t3337, 2);
            }
#line 43 "compiler/main.gem"
    GemVal _t3338[] = {gem_v_full_path};
            gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t3338, 1);
#line 46 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                continue;
            }
#line 49 "compiler/main.gem"
            gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 51 "compiler/main.gem"
    GemVal _t3339[] = {gem_v_full_path};
            GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t3339, 1);
#line 52 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 53 "compiler/main.gem"
    GemVal _t3340[] = {gem_v_full_path};
    GemVal _t3341[] = {gem_add(gem_add(gem_string("cannot load '"), gem_to_string_fn(NULL, _t3340, 1)), gem_string("'"))};
                (void)(gem_error_at_fn("compiler/main.gem", 53, _t3341, 1));
            }
#line 56 "compiler/main.gem"
    GemVal _t3342[] = {gem_v_source};
            GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t3342, 1);
#line 57 "compiler/main.gem"
    GemVal _t3343[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t3343, 1);
#line 58 "compiler/main.gem"
    GemVal _t3344[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t3344, 3);
#line 60 "compiler/main.gem"
    GemVal _t3345 = gem_v_resolved;
            GemVal gem_v__for_items_2 = gem_table_get(_t3345, gem_string("stmts"));
#line 60 "compiler/main.gem"
            GemVal gem_v__for_i_2 = gem_int(0);
#line 60 "compiler/main.gem"
            while (1) {
                GemVal _t3346[] = {gem_v__for_items_2};
                if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t3346, 1)))) break;
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
    GemVal _t3347[] = {gem_v_new_stmts};
    GemVal _t3348 = gem_fn_make_program(NULL, _t3347, 1);
    gem_pop_frame();
    return _t3348;
}

int main(void) {
    gem_push_frame("main", "compiler/main.gem", 0);
#line 20 "compiler/main.gem"
    GemVal _t3349[] = {gem_int(0)};
    GemVal _t3350[] = {gem_fn_gem_get_argv(NULL, _t3349, 1)};
    GemVal _t3351[] = {gem_fn_gem_dirname(NULL, _t3350, 1)};
    gem_v_install_root = gem_fn_gem_dirname(NULL, _t3351, 1);
#line 74 "compiler/main.gem"
    gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
#line 75 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 76 "compiler/main.gem"
    GemVal _t3352[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 76, _t3352, 1));
    }
#line 79 "compiler/main.gem"
    GemVal _t3353[] = {gem_int(1)};
    gem_v_src_path = gem_fn_gem_get_argv(NULL, _t3353, 1);
#line 80 "compiler/main.gem"
    GemVal _t3354[] = {gem_v_src_path};
    gem_v_source = gem_fn_gem_read_file(NULL, _t3354, 1);
#line 81 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 82 "compiler/main.gem"
    GemVal _t3355[] = {gem_v_src_path};
    GemVal _t3356[] = {gem_add(gem_add(gem_string("cannot read '"), gem_to_string_fn(NULL, _t3355, 1)), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 82, _t3356, 1));
    }
#line 85 "compiler/main.gem"
    GemVal _t3357[] = {gem_v_source};
    gem_v_ast = gem_fn_parse_source(NULL, _t3357, 1);
#line 86 "compiler/main.gem"
    GemVal _t3358[] = {gem_v_src_path};
    gem_v_base_dir = gem_fn_gem_dirname(NULL, _t3358, 1);
#line 87 "compiler/main.gem"
    GemVal _t3359 = gem_table_new();
    gem_v_loaded = _t3359;
#line 88 "compiler/main.gem"
    GemVal _t3360[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t3360, 3);
#line 90 "compiler/main.gem"
    GemVal _t3361[] = {gem_v_src_path};
    gem_v_cg = gem_fn_make_codegen(NULL, _t3361, 1);
#line 91 "compiler/main.gem"
    GemVal _t3362 = gem_v_cg;
    GemVal _t3363[] = {gem_v_resolved_ast};
    GemVal _t3364 = gem_table_get(_t3362, gem_string("compile"));
    gem_v_c_code = _t3364.fn(_t3364.env, _t3363, 1);
#line 92 "compiler/main.gem"
    GemVal _t3365[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t3365, 1));
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

