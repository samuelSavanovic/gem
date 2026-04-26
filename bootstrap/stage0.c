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
static GemVal gem_fn_make_receive_match(void *_env, GemVal *args, int argc);
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

static GemVal gem_fn_make_receive_match(void *_env, GemVal *args, int argc) {
#line 116 "compiler/main.gem"
    GemVal gem_v_arms = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_after_ms = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_after_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_recv_var = (argc > 4) ? args[4] : GEM_NIL;
    gem_push_frame("make_receive_match", "compiler/main.gem", 116);
    GemVal _t53 = gem_table_new();
    gem_table_set(_t53, gem_string("tag"), gem_string("receive_match"));
    gem_table_set(_t53, gem_string("arms"), gem_v_arms);
    gem_table_set(_t53, gem_string("after_ms"), gem_v_after_ms);
    gem_table_set(_t53, gem_string("after_body"), gem_v_after_body);
    gem_table_set(_t53, gem_string("line"), gem_v_line);
    gem_table_set(_t53, gem_string("recv_var"), gem_v_recv_var);
    GemVal _t54 = _t53;
    gem_pop_frame();
    return _t54;
}

static GemVal gem_fn_make_return(void *_env, GemVal *args, int argc) {
#line 120 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_line = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_return", "compiler/main.gem", 120);
    GemVal _t55 = gem_table_new();
    gem_table_set(_t55, gem_string("tag"), gem_string("return"));
    gem_table_set(_t55, gem_string("value"), gem_v_value);
    gem_table_set(_t55, gem_string("line"), gem_v_line);
    GemVal _t56 = _t55;
    gem_pop_frame();
    return _t56;
}

static GemVal gem_fn_make_break(void *_env, GemVal *args, int argc) {
#line 124 "compiler/main.gem"
    gem_push_frame("make_break", "compiler/main.gem", 124);
    GemVal _t57 = gem_table_new();
    gem_table_set(_t57, gem_string("tag"), gem_string("break"));
    GemVal _t58 = _t57;
    gem_pop_frame();
    return _t58;
}

static GemVal gem_fn_make_continue(void *_env, GemVal *args, int argc) {
#line 128 "compiler/main.gem"
    gem_push_frame("make_continue", "compiler/main.gem", 128);
    GemVal _t59 = gem_table_new();
    gem_table_set(_t59, gem_string("tag"), gem_string("continue"));
    GemVal _t60 = _t59;
    gem_pop_frame();
    return _t60;
}

static GemVal gem_fn_make_load(void *_env, GemVal *args, int argc) {
#line 132 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_load", "compiler/main.gem", 132);
    GemVal _t61 = gem_table_new();
    gem_table_set(_t61, gem_string("tag"), gem_string("load"));
    gem_table_set(_t61, gem_string("path"), gem_v_path);
    GemVal _t62 = _t61;
    gem_pop_frame();
    return _t62;
}

static GemVal gem_fn_make_extern_fn(void *_env, GemVal *args, int argc) {
#line 136 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_extern_fn", "compiler/main.gem", 136);
    GemVal _t63 = gem_table_new();
    gem_table_set(_t63, gem_string("tag"), gem_string("extern_fn"));
    gem_table_set(_t63, gem_string("name"), gem_v_name);
    gem_table_set(_t63, gem_string("params"), gem_v_params);
    gem_table_set(_t63, gem_string("ret_type"), gem_v_ret_type);
    GemVal _t64 = _t63;
    gem_pop_frame();
    return _t64;
}

static GemVal gem_fn_make_extern_param(void *_env, GemVal *args, int argc) {
#line 140 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_type_name = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_extern_param", "compiler/main.gem", 140);
    GemVal _t65 = gem_table_new();
    gem_table_set(_t65, gem_string("name"), gem_v_name);
    gem_table_set(_t65, gem_string("type"), gem_v_type_name);
    GemVal _t66 = _t65;
    gem_pop_frame();
    return _t66;
}

static GemVal gem_fn_make_extern_include(void *_env, GemVal *args, int argc) {
#line 144 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_extern_include", "compiler/main.gem", 144);
    GemVal _t67 = gem_table_new();
    gem_table_set(_t67, gem_string("tag"), gem_string("extern_include"));
    gem_table_set(_t67, gem_string("path"), gem_v_path);
    GemVal _t68 = _t67;
    gem_pop_frame();
    return _t68;
}

static GemVal gem_fn_is_digit(void *_env, GemVal *args, int argc) {
#line 13 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_digit", "compiler/main.gem", 13);
    GemVal _t69;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("0")))) {
        _t69 = gem_ge(gem_v_ch, gem_string("0"));
    } else {
        _t69 = gem_le(gem_v_ch, gem_string("9"));
    }
    GemVal _t70 = _t69;
    gem_pop_frame();
    return _t70;
}

static GemVal gem_fn_is_alpha(void *_env, GemVal *args, int argc) {
#line 17 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_alpha", "compiler/main.gem", 17);
    GemVal _t71;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("a")))) {
        _t71 = gem_ge(gem_v_ch, gem_string("a"));
    } else {
        _t71 = gem_le(gem_v_ch, gem_string("z"));
    }
    GemVal _t73;
    if (gem_truthy(_t71)) {
        _t73 = _t71;
    } else {
        GemVal _t72;
        if (!gem_truthy(gem_ge(gem_v_ch, gem_string("A")))) {
                _t72 = gem_ge(gem_v_ch, gem_string("A"));
        } else {
                _t72 = gem_le(gem_v_ch, gem_string("Z"));
        }
        _t73 = _t72;
    }
    GemVal _t74;
    if (gem_truthy(_t73)) {
        _t74 = _t73;
    } else {
        _t74 = gem_eq(gem_v_ch, gem_string("_"));
    }
    GemVal _t75 = _t74;
    gem_pop_frame();
    return _t75;
}

static GemVal gem_fn_is_alnum(void *_env, GemVal *args, int argc) {
#line 21 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_alnum", "compiler/main.gem", 21);
    GemVal _t76[] = {gem_v_ch};
    GemVal _t78;
    if (gem_truthy(gem_fn_is_alpha(NULL, _t76, 1))) {
        _t78 = gem_fn_is_alpha(NULL, _t76, 1);
    } else {
        GemVal _t77[] = {gem_v_ch};
        _t78 = gem_fn_is_digit(NULL, _t77, 1);
    }
    GemVal _t79 = _t78;
    gem_pop_frame();
    return _t79;
}

static GemVal gem_fn_digit_val(void *_env, GemVal *args, int argc) {
#line 27 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("digit_val", "compiler/main.gem", 27);
#line 28 "compiler/main.gem"
    GemVal gem_v__match_1 = gem_v_ch;
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("0")))) {
        GemVal _t80 = gem_int(0);
        gem_pop_frame();
        return _t80;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("1")))) {
        GemVal _t81 = gem_int(1);
        gem_pop_frame();
        return _t81;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("2")))) {
        GemVal _t82 = gem_int(2);
        gem_pop_frame();
        return _t82;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("3")))) {
        GemVal _t83 = gem_int(3);
        gem_pop_frame();
        return _t83;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("4")))) {
        GemVal _t84 = gem_int(4);
        gem_pop_frame();
        return _t84;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("5")))) {
        GemVal _t85 = gem_int(5);
        gem_pop_frame();
        return _t85;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("6")))) {
        GemVal _t86 = gem_int(6);
        gem_pop_frame();
        return _t86;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("7")))) {
        GemVal _t87 = gem_int(7);
        gem_pop_frame();
        return _t87;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("8")))) {
        GemVal _t88 = gem_int(8);
        gem_pop_frame();
        return _t88;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("9")))) {
        GemVal _t89 = gem_int(9);
        gem_pop_frame();
        return _t89;
    } else {
#line 50 "compiler/main.gem"
    GemVal _t90[] = {gem_v_ch};
    GemVal _t91[] = {gem_add(gem_string("not a digit: "), gem_to_string_fn(NULL, _t90, 1))};
        GemVal _t92 = gem_error_at_fn("compiler/main.gem", 50, _t91, 1);
        gem_pop_frame();
        return _t92;
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
    GemVal _t93[] = {gem_v_s};
    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t93, 1);
#line 56 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 56 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_2;
#line 56 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 57 "compiler/main.gem"
    GemVal _t94[] = {gem_table_get(gem_v_s, gem_v_i)};
        gem_v_result = gem_add(gem_mul(gem_v_result, gem_int(10)), gem_fn_digit_val(NULL, _t94, 1));
    }

    GemVal _t95 = gem_v_result;
    gem_pop_frame();
    return _t95;
}

static GemVal gem_fn_contains_dot(void *_env, GemVal *args, int argc) {
#line 64 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("contains_dot", "compiler/main.gem", 64);
#line 65 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 65 "compiler/main.gem"
    GemVal _t96[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t96, 1);
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
            GemVal _t97 = gem_bool(1);
            gem_pop_frame();
            return _t97;
        }
    }

    GemVal _t98 = gem_bool(0);
    gem_pop_frame();
    return _t98;
}

static GemVal gem_fn_apply_escape(void *_env, GemVal *args, int argc) {
#line 78 "compiler/main.gem"
    GemVal gem_v_esc = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_mode = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("apply_escape", "compiler/main.gem", 78);
#line 79 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 80 "compiler/main.gem"
        GemVal _t99 = gem_string("\n");
        gem_pop_frame();
        return _t99;
    } else {
#line 81 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 82 "compiler/main.gem"
    GemVal _t100[] = {gem_int(13)};
            GemVal _t101 = gem_chr_fn(NULL, _t100, 1);
            gem_pop_frame();
            return _t101;
        } else {
#line 83 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 84 "compiler/main.gem"
                GemVal _t102 = gem_string("\t");
                gem_pop_frame();
                return _t102;
            } else {
#line 85 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 86 "compiler/main.gem"
                    GemVal _t103 = gem_string("\\");
                    gem_pop_frame();
                    return _t103;
                } else {
#line 87 "compiler/main.gem"
    GemVal _t104;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
        _t104 = gem_eq(gem_v_esc, gem_string("0"));
    } else {
        _t104 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                    if (gem_truthy(_t104)) {
#line 88 "compiler/main.gem"
    GemVal _t105[] = {gem_int(0)};
                        GemVal _t106 = gem_chr_fn(NULL, _t105, 1);
                        gem_pop_frame();
                        return _t106;
                    } else {
#line 89 "compiler/main.gem"
    GemVal _t107;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
        _t107 = gem_eq(gem_v_esc, gem_string("\""));
    } else {
        _t107 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                        if (gem_truthy(_t107)) {
#line 90 "compiler/main.gem"
                            GemVal _t108 = gem_string("\"");
                            gem_pop_frame();
                            return _t108;
                        } else {
#line 91 "compiler/main.gem"
    GemVal _t109;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
        _t109 = gem_eq(gem_v_esc, gem_string("{"));
    } else {
        _t109 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                            if (gem_truthy(_t109)) {
#line 92 "compiler/main.gem"
                                GemVal _t110 = gem_string("{");
                                gem_pop_frame();
                                return _t110;
                            } else {
#line 93 "compiler/main.gem"
    GemVal _t111;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
        _t111 = gem_eq(gem_v_esc, gem_string("}"));
    } else {
        _t111 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                                if (gem_truthy(_t111)) {
#line 94 "compiler/main.gem"
                                    GemVal _t112 = gem_string("}");
                                    gem_pop_frame();
                                    return _t112;
                                } else {
#line 95 "compiler/main.gem"
    GemVal _t113;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("'")))) {
        _t113 = gem_eq(gem_v_esc, gem_string("'"));
    } else {
        _t113 = gem_eq(gem_v_mode, gem_string("sq"));
    }
                                    if (gem_truthy(_t113)) {
#line 96 "compiler/main.gem"
                                        GemVal _t114 = gem_string("'");
                                        gem_pop_frame();
                                        return _t114;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t115 = GEM_NIL;
    gem_pop_frame();
    return _t115;
}

static GemVal gem_fn_build_keyword_set(void *_env, GemVal *args, int argc) {
#line 103 "compiler/main.gem"
    gem_push_frame("build_keyword_set", "compiler/main.gem", 103);
#line 104 "compiler/main.gem"
    GemVal _t116 = gem_table_new();
    GemVal gem_v_t = _t116;
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
#line 128 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("after"), gem_bool(1));
    GemVal _t117 = gem_v_t;
    gem_pop_frame();
    return _t117;
}

static GemVal gem_fn_tokenize(void *_env, GemVal *args, int argc) {
#line 132 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("tokenize", "compiler/main.gem", 132);
#line 133 "compiler/main.gem"
    GemVal gem_v_kw = gem_fn_build_keyword_set(NULL, NULL, 0);
#line 134 "compiler/main.gem"
    GemVal _t118 = gem_table_new();
    GemVal gem_v_tokens = _t118;
#line 135 "compiler/main.gem"
    GemVal gem_v_pos = gem_int(0);
#line 136 "compiler/main.gem"
    GemVal gem_v_line = gem_int(1);
#line 137 "compiler/main.gem"
    GemVal _t119[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t119, 1);
#line 138 "compiler/main.gem"
    GemVal gem_v_interp_depth = gem_int(0);
#line 139 "compiler/main.gem"
    GemVal _t120 = gem_table_new();
    GemVal gem_v_interp_brace = _t120;
#line 140 "compiler/main.gem"
    GemVal _t121[] = {gem_int(13)};
    GemVal gem_v_CR = gem_chr_fn(NULL, _t121, 1);
#line 141 "compiler/main.gem"
    GemVal _t122[] = {gem_int(0)};
    GemVal gem_v_NUL = gem_chr_fn(NULL, _t122, 1);
#line 142 "compiler/main.gem"
    GemVal gem_v_in_triple_dq = gem_bool(0);
#line 143 "compiler/main.gem"
    GemVal gem_v_triple_dq_indent = gem_int(0);
#line 145 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 146 "compiler/main.gem"
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
#line 149 "compiler/main.gem"
    GemVal _t123;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t123 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t123 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t123)) {
#line 150 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 155 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 156 "compiler/main.gem"
            while (1) {
                GemVal _t124;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t124 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t124 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t124)) break;
#line 157 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 163 "compiler/main.gem"
    GemVal _t125;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t125 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t125 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t125)) {
#line 164 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 165 "compiler/main.gem"
    GemVal _t126[] = {gem_v_tokens};
    GemVal _t129;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t126, 1), gem_int(0)))) {
        _t129 = gem_gt(gem_len_fn(NULL, _t126, 1), gem_int(0));
    } else {
        GemVal _t127[] = {gem_v_tokens};
        GemVal _t128 = gem_table_get(gem_v_tokens, gem_sub(gem_len_fn(NULL, _t127, 1), gem_int(1)));
        _t129 = gem_eq(gem_table_get(_t128, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t129)) {
#line 166 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 168 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 169 "compiler/main.gem"
    GemVal _t130 = gem_table_new();
    gem_table_set(_t130, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t130, gem_string("value"), gem_string("\n"));
    gem_table_set(_t130, gem_string("line"), gem_v_line);
    GemVal _t131[] = {gem_v_tokens, _t130};
                (void)(gem_push_fn(NULL, _t131, 2));
            }
#line 171 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 172 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
            }
#line 174 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 176 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 177 "compiler/main.gem"
                GemVal gem_v_c2 = gem_table_get(gem_v_source, gem_v_pos);
#line 178 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_c2, gem_string("\n")))) {
#line 179 "compiler/main.gem"
                    gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 180 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                } else {
#line 181 "compiler/main.gem"
    GemVal _t132;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t132 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t132 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t133;
    if (gem_truthy(_t132)) {
        _t133 = _t132;
    } else {
        _t133 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t133)) {
#line 182 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 183 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 184 "compiler/main.gem"
                            while (1) {
                                GemVal _t134;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t134 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t134 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t134)) break;
#line 185 "compiler/main.gem"
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
#line 195 "compiler/main.gem"
    GemVal _t135[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t135, 1))) {
#line 196 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 197 "compiler/main.gem"
            while (1) {
                GemVal _t137;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t137 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t136[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t137 = gem_fn_is_digit(NULL, _t136, 1);
                }
                if (!gem_truthy(_t137)) break;
#line 198 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 199 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 201 "compiler/main.gem"
    GemVal _t138;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t138 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t138 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t138;
#line 202 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 203 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 204 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 205 "compiler/main.gem"
                while (1) {
                    GemVal _t140;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t140 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t139[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t140 = gem_fn_is_digit(NULL, _t139, 1);
                    }
                    if (!gem_truthy(_t140)) break;
#line 206 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 207 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 210 "compiler/main.gem"
    GemVal _t141 = gem_table_new();
    gem_table_set(_t141, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t141, gem_string("value"), gem_v_val);
    gem_table_set(_t141, gem_string("line"), gem_v_line);
    GemVal _t142[] = {gem_v_tokens, _t141};
            (void)(gem_push_fn(NULL, _t142, 2));
            continue;
        }
#line 215 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 216 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 219 "compiler/main.gem"
    GemVal _t143;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t143 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t143 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t144;
    if (!gem_truthy(_t143)) {
        _t144 = _t143;
    } else {
        _t144 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
            if (gem_truthy(_t144)) {
#line 220 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 222 "compiler/main.gem"
                while (1) {
                    GemVal _t146;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t146 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t145;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t145 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t145 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t146 = _t145;
                    }
                    if (!gem_truthy(_t146)) break;
#line 223 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 225 "compiler/main.gem"
    GemVal _t147;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t147 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t147 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t147)) {
#line 226 "compiler/main.gem"
    GemVal _t148[] = {gem_v_line};
    GemVal _t149[] = {gem_add(gem_string("triple-quoted string must be followed by a newline at line "), gem_to_string_fn(NULL, _t148, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 226, _t149, 1));
                }
#line 228 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 229 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 232 "compiler/main.gem"
                GemVal gem_v_tq_scan = gem_v_pos;
#line 233 "compiler/main.gem"
                GemVal gem_v_tq_bi = gem_int(0);
#line 234 "compiler/main.gem"
                GemVal gem_v_tq_found = gem_bool(0);
#line 235 "compiler/main.gem"
                while (1) {
                    GemVal _t150;
                    if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                        _t150 = gem_lt(gem_v_tq_scan, gem_v_length);
                    } else {
                                        _t150 = gem_not(gem_v_tq_found);
                    }
                    if (!gem_truthy(_t150)) break;
#line 236 "compiler/main.gem"
                    GemVal gem_v_tq_ind = gem_int(0);
#line 237 "compiler/main.gem"
                    while (1) {
                        GemVal _t151;
                        if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                _t151 = gem_lt(gem_v_tq_scan, gem_v_length);
                        } else {
                                                _t151 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t151)) break;
#line 238 "compiler/main.gem"
                        gem_v_tq_ind = gem_add(gem_v_tq_ind, gem_int(1));
#line 239 "compiler/main.gem"
                        gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                    }
#line 241 "compiler/main.gem"
    GemVal _t152;
    if (!gem_truthy(gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length))) {
        _t152 = gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length);
    } else {
        _t152 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\""));
    }
    GemVal _t153;
    if (!gem_truthy(_t152)) {
        _t153 = _t152;
    } else {
        _t153 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(1))), gem_string("\""));
    }
    GemVal _t154;
    if (!gem_truthy(_t153)) {
        _t154 = _t153;
    } else {
        _t154 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(2))), gem_string("\""));
    }
                    if (gem_truthy(_t154)) {
#line 242 "compiler/main.gem"
                        gem_v_tq_bi = gem_v_tq_ind;
#line 243 "compiler/main.gem"
                        gem_v_tq_found = gem_bool(1);
                    } else {
#line 245 "compiler/main.gem"
                        while (1) {
                            GemVal _t155;
                            if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                        _t155 = gem_lt(gem_v_tq_scan, gem_v_length);
                            } else {
                                                        _t155 = gem_neq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t155)) break;
#line 246 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
#line 248 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
#line 249 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
                    }
                }
#line 253 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_tq_found))) {
#line 254 "compiler/main.gem"
    GemVal _t156[] = {gem_v_line};
    GemVal _t157[] = {gem_add(gem_string("unterminated triple-quoted string at line "), gem_to_string_fn(NULL, _t156, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 254, _t157, 1));
                }
#line 256 "compiler/main.gem"
                gem_v_triple_dq_indent = gem_v_tq_bi;
#line 259 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 260 "compiler/main.gem"
                GemVal gem_v_hit_interp = gem_bool(0);
#line 261 "compiler/main.gem"
                GemVal gem_v_tq_als = gem_bool(1);
#line 263 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 264 "compiler/main.gem"
                    if (gem_truthy(gem_v_tq_als)) {
#line 265 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(0);
#line 266 "compiler/main.gem"
                        GemVal gem_v_tq_skip = gem_int(0);
#line 267 "compiler/main.gem"
                        while (1) {
                            GemVal _t158;
                            if (!gem_truthy(gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent))) {
                                                        _t158 = gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t158 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t159;
                            if (!gem_truthy(_t158)) {
                                                        _t159 = _t158;
                            } else {
                                                        _t159 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t159)) break;
#line 268 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 269 "compiler/main.gem"
                            gem_v_tq_skip = gem_add(gem_v_tq_skip, gem_int(1));
                        }
#line 272 "compiler/main.gem"
    GemVal _t160;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t160 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t160 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t161;
    if (!gem_truthy(_t160)) {
        _t161 = _t160;
    } else {
        _t161 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t162;
    if (!gem_truthy(_t161)) {
        _t162 = _t161;
    } else {
        _t162 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t162)) {
#line 273 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 275 "compiler/main.gem"
    GemVal _t163[] = {gem_v_val};
    GemVal _t165;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t163, 1), gem_int(0)))) {
        _t165 = gem_gt(gem_len_fn(NULL, _t163, 1), gem_int(0));
    } else {
        GemVal _t164[] = {gem_v_val};
        _t165 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t164, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t165)) {
#line 276 "compiler/main.gem"
    GemVal _t166[] = {gem_v_val};
    GemVal _t167[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t166, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t167, 3);
                            }
                            break;
                        }
                    }
#line 282 "compiler/main.gem"
                    GemVal gem_v_tqc = gem_table_get(gem_v_source, gem_v_pos);
#line 283 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\n")))) {
#line 284 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 285 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 286 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 287 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(1);
                        continue;
                    }
#line 290 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\\")))) {
#line 291 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 292 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 293 "compiler/main.gem"
    GemVal _t168[] = {gem_v_line};
    GemVal _t169[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t168, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 293, _t169, 1));
                        }
#line 295 "compiler/main.gem"
                        GemVal gem_v_tqe = gem_table_get(gem_v_source, gem_v_pos);
#line 296 "compiler/main.gem"
    GemVal _t170[] = {gem_v_tqe, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t170, 2);
#line 297 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 298 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 300 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqe);
                        }
#line 302 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 305 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("{")))) {
#line 306 "compiler/main.gem"
    GemVal _t171 = gem_table_new();
    gem_table_set(_t171, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t171, gem_string("value"), gem_string(""));
    gem_table_set(_t171, gem_string("line"), gem_v_line);
    GemVal _t172[] = {gem_v_tokens, _t171};
                        (void)(gem_push_fn(NULL, _t172, 2));
#line 307 "compiler/main.gem"
    GemVal _t173 = gem_table_new();
    gem_table_set(_t173, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t173, gem_string("value"), gem_v_val);
    gem_table_set(_t173, gem_string("line"), gem_v_line);
    GemVal _t174[] = {gem_v_tokens, _t173};
                        (void)(gem_push_fn(NULL, _t174, 2));
#line 308 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 309 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 310 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 311 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 312 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 313 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 316 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqc);
#line 317 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 320 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 321 "compiler/main.gem"
    GemVal _t175 = gem_table_new();
    gem_table_set(_t175, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t175, gem_string("value"), gem_v_val);
    gem_table_set(_t175, gem_string("line"), gem_v_line);
    GemVal _t176[] = {gem_v_tokens, _t175};
                    (void)(gem_push_fn(NULL, _t176, 2));
                }
                continue;
            }
#line 327 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 328 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 329 "compiler/main.gem"
            while (1) {
                GemVal _t177;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t177 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t177 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t177)) break;
#line 330 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 331 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 332 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 333 "compiler/main.gem"
    GemVal _t178[] = {gem_v_line};
    GemVal _t179[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t178, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 333, _t179, 1));
                    }
#line 335 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 336 "compiler/main.gem"
    GemVal _t180[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t180, 2);
#line 337 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 338 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 340 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 342 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 345 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 346 "compiler/main.gem"
    GemVal _t181 = gem_table_new();
    gem_table_set(_t181, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t181, gem_string("value"), gem_string(""));
    gem_table_set(_t181, gem_string("line"), gem_v_line);
    GemVal _t182[] = {gem_v_tokens, _t181};
                    (void)(gem_push_fn(NULL, _t182, 2));
#line 347 "compiler/main.gem"
    GemVal _t183 = gem_table_new();
    gem_table_set(_t183, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t183, gem_string("value"), gem_v_val);
    gem_table_set(_t183, gem_string("line"), gem_v_line);
    GemVal _t184[] = {gem_v_tokens, _t183};
                    (void)(gem_push_fn(NULL, _t184, 2));
#line 348 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 349 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 350 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 351 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 352 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 355 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 356 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 358 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 359 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 360 "compiler/main.gem"
    GemVal _t185[] = {gem_v_line};
    GemVal _t186[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t185, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 360, _t186, 1));
                }
#line 362 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 363 "compiler/main.gem"
    GemVal _t187 = gem_table_new();
    gem_table_set(_t187, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t187, gem_string("value"), gem_v_val);
    gem_table_set(_t187, gem_string("line"), gem_v_line);
    GemVal _t188[] = {gem_v_tokens, _t187};
                (void)(gem_push_fn(NULL, _t188, 2));
            }
            continue;
        }
#line 369 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("'")))) {
#line 370 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 373 "compiler/main.gem"
    GemVal _t189;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t189 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t189 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t190;
    if (!gem_truthy(_t189)) {
        _t190 = _t189;
    } else {
        _t190 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
            if (gem_truthy(_t190)) {
#line 374 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 376 "compiler/main.gem"
                while (1) {
                    GemVal _t192;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t192 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t191;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t191 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t191 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t192 = _t191;
                    }
                    if (!gem_truthy(_t192)) break;
#line 377 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 379 "compiler/main.gem"
    GemVal _t193;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t193 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t193 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t193)) {
#line 380 "compiler/main.gem"
    GemVal _t194[] = {gem_v_line};
    GemVal _t195[] = {gem_add(gem_string("triple-quoted string must be followed by a newline at line "), gem_to_string_fn(NULL, _t194, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 380, _t195, 1));
                }
#line 382 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 383 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 386 "compiler/main.gem"
                GemVal gem_v_sq_scan = gem_v_pos;
#line 387 "compiler/main.gem"
                GemVal gem_v_sq_bi = gem_int(0);
#line 388 "compiler/main.gem"
                GemVal gem_v_sq_found = gem_bool(0);
#line 389 "compiler/main.gem"
                while (1) {
                    GemVal _t196;
                    if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                        _t196 = gem_lt(gem_v_sq_scan, gem_v_length);
                    } else {
                                        _t196 = gem_not(gem_v_sq_found);
                    }
                    if (!gem_truthy(_t196)) break;
#line 390 "compiler/main.gem"
                    GemVal gem_v_sq_ind = gem_int(0);
#line 391 "compiler/main.gem"
                    while (1) {
                        GemVal _t197;
                        if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                _t197 = gem_lt(gem_v_sq_scan, gem_v_length);
                        } else {
                                                _t197 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t197)) break;
#line 392 "compiler/main.gem"
                        gem_v_sq_ind = gem_add(gem_v_sq_ind, gem_int(1));
#line 393 "compiler/main.gem"
                        gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                    }
#line 395 "compiler/main.gem"
    GemVal _t198;
    if (!gem_truthy(gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length))) {
        _t198 = gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length);
    } else {
        _t198 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("'"));
    }
    GemVal _t199;
    if (!gem_truthy(_t198)) {
        _t199 = _t198;
    } else {
        _t199 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(1))), gem_string("'"));
    }
    GemVal _t200;
    if (!gem_truthy(_t199)) {
        _t200 = _t199;
    } else {
        _t200 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(2))), gem_string("'"));
    }
                    if (gem_truthy(_t200)) {
#line 396 "compiler/main.gem"
                        gem_v_sq_bi = gem_v_sq_ind;
#line 397 "compiler/main.gem"
                        gem_v_sq_found = gem_bool(1);
                    } else {
#line 399 "compiler/main.gem"
                        while (1) {
                            GemVal _t201;
                            if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                        _t201 = gem_lt(gem_v_sq_scan, gem_v_length);
                            } else {
                                                        _t201 = gem_neq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t201)) break;
#line 400 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
#line 402 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
#line 403 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
                    }
                }
#line 407 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_sq_found))) {
#line 408 "compiler/main.gem"
    GemVal _t202[] = {gem_v_line};
    GemVal _t203[] = {gem_add(gem_string("unterminated triple-quoted string at line "), gem_to_string_fn(NULL, _t202, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 408, _t203, 1));
                }
#line 412 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 413 "compiler/main.gem"
                GemVal gem_v_sq_als = gem_bool(1);
#line 415 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 416 "compiler/main.gem"
                    if (gem_truthy(gem_v_sq_als)) {
#line 417 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(0);
#line 418 "compiler/main.gem"
                        GemVal gem_v_sq_skip = gem_int(0);
#line 419 "compiler/main.gem"
                        while (1) {
                            GemVal _t204;
                            if (!gem_truthy(gem_lt(gem_v_sq_skip, gem_v_sq_bi))) {
                                                        _t204 = gem_lt(gem_v_sq_skip, gem_v_sq_bi);
                            } else {
                                                        _t204 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t205;
                            if (!gem_truthy(_t204)) {
                                                        _t205 = _t204;
                            } else {
                                                        _t205 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t205)) break;
#line 420 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 421 "compiler/main.gem"
                            gem_v_sq_skip = gem_add(gem_v_sq_skip, gem_int(1));
                        }
#line 424 "compiler/main.gem"
    GemVal _t206;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t206 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t206 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t207;
    if (!gem_truthy(_t206)) {
        _t207 = _t206;
    } else {
        _t207 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
    GemVal _t208;
    if (!gem_truthy(_t207)) {
        _t208 = _t207;
    } else {
        _t208 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("'"));
    }
                        if (gem_truthy(_t208)) {
#line 425 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 427 "compiler/main.gem"
    GemVal _t209[] = {gem_v_val};
    GemVal _t211;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t209, 1), gem_int(0)))) {
        _t211 = gem_gt(gem_len_fn(NULL, _t209, 1), gem_int(0));
    } else {
        GemVal _t210[] = {gem_v_val};
        _t211 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t210, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t211)) {
#line 428 "compiler/main.gem"
    GemVal _t212[] = {gem_v_val};
    GemVal _t213[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t212, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t213, 3);
                            }
                            break;
                        }
                    }
#line 434 "compiler/main.gem"
                    GemVal gem_v_sqc = gem_table_get(gem_v_source, gem_v_pos);
#line 435 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\n")))) {
#line 436 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 437 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 438 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 439 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(1);
                        continue;
                    }
#line 442 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\\")))) {
#line 443 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 444 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 445 "compiler/main.gem"
    GemVal _t214[] = {gem_v_line};
    GemVal _t215[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t214, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 445, _t215, 1));
                        }
#line 447 "compiler/main.gem"
                        GemVal gem_v_sqe = gem_table_get(gem_v_source, gem_v_pos);
#line 448 "compiler/main.gem"
    GemVal _t216[] = {gem_v_sqe, gem_string("sq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t216, 2);
#line 449 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 450 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 452 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_sqe);
                        }
#line 454 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 457 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_sqc);
#line 458 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 461 "compiler/main.gem"
    GemVal _t217 = gem_table_new();
    gem_table_set(_t217, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t217, gem_string("value"), gem_v_val);
    gem_table_set(_t217, gem_string("line"), gem_v_line);
    GemVal _t218[] = {gem_v_tokens, _t217};
                (void)(gem_push_fn(NULL, _t218, 2));
                continue;
            }
#line 466 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 467 "compiler/main.gem"
            while (1) {
                GemVal _t219;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t219 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t219 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
                }
                if (!gem_truthy(_t219)) break;
#line 468 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 469 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 470 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 471 "compiler/main.gem"
    GemVal _t220[] = {gem_v_line};
    GemVal _t221[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t220, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 471, _t221, 1));
                    }
#line 473 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 474 "compiler/main.gem"
    GemVal _t222[] = {gem_v_esc, gem_string("sq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t222, 2);
#line 475 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 476 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 478 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 480 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 483 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 484 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 486 "compiler/main.gem"
            if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 487 "compiler/main.gem"
    GemVal _t223[] = {gem_v_line};
    GemVal _t224[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t223, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 487, _t224, 1));
            }
#line 489 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 490 "compiler/main.gem"
    GemVal _t225 = gem_table_new();
    gem_table_set(_t225, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t225, gem_string("value"), gem_v_val);
    gem_table_set(_t225, gem_string("line"), gem_v_line);
    GemVal _t226[] = {gem_v_tokens, _t225};
            (void)(gem_push_fn(NULL, _t226, 2));
            continue;
        }
#line 495 "compiler/main.gem"
    GemVal _t227[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t227, 1))) {
#line 496 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 497 "compiler/main.gem"
            while (1) {
                GemVal _t229;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t229 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t228[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t229 = gem_fn_is_alnum(NULL, _t228, 1);
                }
                if (!gem_truthy(_t229)) break;
#line 498 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 499 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 501 "compiler/main.gem"
    GemVal _t230[] = {gem_v_kw, gem_v_val};
            if (gem_truthy(gem_has_key_fn(NULL, _t230, 2))) {
#line 502 "compiler/main.gem"
    GemVal _t231 = gem_table_new();
    gem_table_set(_t231, gem_string("type"), gem_v_val);
    gem_table_set(_t231, gem_string("value"), gem_v_val);
    gem_table_set(_t231, gem_string("line"), gem_v_line);
    GemVal _t232[] = {gem_v_tokens, _t231};
                (void)(gem_push_fn(NULL, _t232, 2));
            } else {
#line 504 "compiler/main.gem"
    GemVal _t233 = gem_table_new();
    gem_table_set(_t233, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t233, gem_string("value"), gem_v_val);
    gem_table_set(_t233, gem_string("line"), gem_v_line);
    GemVal _t234[] = {gem_v_tokens, _t233};
                (void)(gem_push_fn(NULL, _t234, 2));
            }
            continue;
        }
#line 510 "compiler/main.gem"
        GemVal gem_v_next = gem_string("");
#line 511 "compiler/main.gem"
        if (gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
#line 512 "compiler/main.gem"
            gem_v_next = gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1)));
        }
#line 515 "compiler/main.gem"
    GemVal _t235;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t235 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t235 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t235)) {
#line 516 "compiler/main.gem"
    GemVal _t236 = gem_table_new();
    gem_table_set(_t236, gem_string("type"), gem_string("=="));
    gem_table_set(_t236, gem_string("value"), gem_string("=="));
    gem_table_set(_t236, gem_string("line"), gem_v_line);
    GemVal _t237[] = {gem_v_tokens, _t236};
            (void)(gem_push_fn(NULL, _t237, 2));
#line 517 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 520 "compiler/main.gem"
    GemVal _t238;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t238 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t238 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t238)) {
#line 521 "compiler/main.gem"
    GemVal _t239 = gem_table_new();
    gem_table_set(_t239, gem_string("type"), gem_string("!="));
    gem_table_set(_t239, gem_string("value"), gem_string("!="));
    gem_table_set(_t239, gem_string("line"), gem_v_line);
    GemVal _t240[] = {gem_v_tokens, _t239};
            (void)(gem_push_fn(NULL, _t240, 2));
#line 522 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 525 "compiler/main.gem"
    GemVal _t241;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t241 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t241 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t241)) {
#line 526 "compiler/main.gem"
    GemVal _t242 = gem_table_new();
    gem_table_set(_t242, gem_string("type"), gem_string("<="));
    gem_table_set(_t242, gem_string("value"), gem_string("<="));
    gem_table_set(_t242, gem_string("line"), gem_v_line);
    GemVal _t243[] = {gem_v_tokens, _t242};
            (void)(gem_push_fn(NULL, _t243, 2));
#line 527 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 530 "compiler/main.gem"
    GemVal _t244;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t244 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t244 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t244)) {
#line 531 "compiler/main.gem"
    GemVal _t245 = gem_table_new();
    gem_table_set(_t245, gem_string("type"), gem_string(">="));
    gem_table_set(_t245, gem_string("value"), gem_string(">="));
    gem_table_set(_t245, gem_string("line"), gem_v_line);
    GemVal _t246[] = {gem_v_tokens, _t245};
            (void)(gem_push_fn(NULL, _t246, 2));
#line 532 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 535 "compiler/main.gem"
    GemVal _t247;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t247 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t247 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t247)) {
#line 536 "compiler/main.gem"
    GemVal _t248 = gem_table_new();
    gem_table_set(_t248, gem_string("type"), gem_string("+="));
    gem_table_set(_t248, gem_string("value"), gem_string("+="));
    gem_table_set(_t248, gem_string("line"), gem_v_line);
    GemVal _t249[] = {gem_v_tokens, _t248};
            (void)(gem_push_fn(NULL, _t249, 2));
#line 537 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 540 "compiler/main.gem"
    GemVal _t250;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t250 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t250 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t250)) {
#line 541 "compiler/main.gem"
    GemVal _t251 = gem_table_new();
    gem_table_set(_t251, gem_string("type"), gem_string("-="));
    gem_table_set(_t251, gem_string("value"), gem_string("-="));
    gem_table_set(_t251, gem_string("line"), gem_v_line);
    GemVal _t252[] = {gem_v_tokens, _t251};
            (void)(gem_push_fn(NULL, _t252, 2));
#line 542 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 545 "compiler/main.gem"
    GemVal _t253;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t253 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t253 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t253)) {
#line 546 "compiler/main.gem"
    GemVal _t254 = gem_table_new();
    gem_table_set(_t254, gem_string("type"), gem_string("*="));
    gem_table_set(_t254, gem_string("value"), gem_string("*="));
    gem_table_set(_t254, gem_string("line"), gem_v_line);
    GemVal _t255[] = {gem_v_tokens, _t254};
            (void)(gem_push_fn(NULL, _t255, 2));
#line 547 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 550 "compiler/main.gem"
    GemVal _t256;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t256 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t256 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t256)) {
#line 551 "compiler/main.gem"
    GemVal _t257 = gem_table_new();
    gem_table_set(_t257, gem_string("type"), gem_string("/="));
    gem_table_set(_t257, gem_string("value"), gem_string("/="));
    gem_table_set(_t257, gem_string("line"), gem_v_line);
    GemVal _t258[] = {gem_v_tokens, _t257};
            (void)(gem_push_fn(NULL, _t258, 2));
#line 552 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 555 "compiler/main.gem"
    GemVal _t259;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t259 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t259 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t259)) {
#line 556 "compiler/main.gem"
    GemVal _t260 = gem_table_new();
    gem_table_set(_t260, gem_string("type"), gem_string("->"));
    gem_table_set(_t260, gem_string("value"), gem_string("->"));
    gem_table_set(_t260, gem_string("line"), gem_v_line);
    GemVal _t261[] = {gem_v_tokens, _t260};
            (void)(gem_push_fn(NULL, _t261, 2));
#line 557 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 562 "compiler/main.gem"
    GemVal _t262;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t262 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t262 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t263;
    if (gem_truthy(_t262)) {
        _t263 = _t262;
    } else {
        _t263 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t264;
    if (gem_truthy(_t263)) {
        _t264 = _t263;
    } else {
        _t264 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t265;
    if (gem_truthy(_t264)) {
        _t265 = _t264;
    } else {
        _t265 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t265)) {
#line 563 "compiler/main.gem"
    GemVal _t266 = gem_table_new();
    gem_table_set(_t266, gem_string("type"), gem_v_ch);
    gem_table_set(_t266, gem_string("value"), gem_v_ch);
    gem_table_set(_t266, gem_string("line"), gem_v_line);
    GemVal _t267[] = {gem_v_tokens, _t266};
            (void)(gem_push_fn(NULL, _t267, 2));
#line 564 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 567 "compiler/main.gem"
    GemVal _t268;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t268 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t268 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t269;
    if (gem_truthy(_t268)) {
        _t269 = _t268;
    } else {
        _t269 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t269)) {
#line 568 "compiler/main.gem"
    GemVal _t270 = gem_table_new();
    gem_table_set(_t270, gem_string("type"), gem_v_ch);
    gem_table_set(_t270, gem_string("value"), gem_v_ch);
    gem_table_set(_t270, gem_string("line"), gem_v_line);
    GemVal _t271[] = {gem_v_tokens, _t270};
            (void)(gem_push_fn(NULL, _t271, 2));
#line 569 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 573 "compiler/main.gem"
    GemVal _t272;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t272 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t272 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t272)) {
#line 574 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 575 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 576 "compiler/main.gem"
    GemVal _t273 = gem_table_new();
    gem_table_set(_t273, gem_string("type"), gem_string("{"));
    gem_table_set(_t273, gem_string("value"), gem_string("{"));
    gem_table_set(_t273, gem_string("line"), gem_v_line);
    GemVal _t274[] = {gem_v_tokens, _t273};
            (void)(gem_push_fn(NULL, _t274, 2));
#line 577 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 580 "compiler/main.gem"
    GemVal _t275;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t275 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t275 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t275)) {
#line 581 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 582 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 583 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 584 "compiler/main.gem"
    GemVal _t276 = gem_table_new();
    gem_table_set(_t276, gem_string("type"), gem_string("}"));
    gem_table_set(_t276, gem_string("value"), gem_string("}"));
    gem_table_set(_t276, gem_string("line"), gem_v_line);
    GemVal _t277[] = {gem_v_tokens, _t276};
                (void)(gem_push_fn(NULL, _t277, 2));
#line 585 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                continue;
            }
#line 589 "compiler/main.gem"
            gem_v_interp_depth = gem_sub(gem_v_interp_depth, gem_int(1));
#line 590 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 593 "compiler/main.gem"
            if (gem_truthy(gem_v_in_triple_dq)) {
#line 594 "compiler/main.gem"
                gem_v_in_triple_dq = gem_bool(0);
#line 595 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 596 "compiler/main.gem"
                GemVal gem_v_hit_interp = gem_bool(0);
#line 597 "compiler/main.gem"
                GemVal gem_v_tqr_als = gem_bool(0);
#line 599 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 600 "compiler/main.gem"
                    if (gem_truthy(gem_v_tqr_als)) {
#line 601 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(0);
#line 602 "compiler/main.gem"
                        GemVal gem_v_tqr_skip = gem_int(0);
#line 603 "compiler/main.gem"
                        while (1) {
                            GemVal _t278;
                            if (!gem_truthy(gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent))) {
                                                        _t278 = gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t278 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t279;
                            if (!gem_truthy(_t278)) {
                                                        _t279 = _t278;
                            } else {
                                                        _t279 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t279)) break;
#line 604 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 605 "compiler/main.gem"
                            gem_v_tqr_skip = gem_add(gem_v_tqr_skip, gem_int(1));
                        }
#line 608 "compiler/main.gem"
    GemVal _t280;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t280 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t280 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t281;
    if (!gem_truthy(_t280)) {
        _t281 = _t280;
    } else {
        _t281 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t282;
    if (!gem_truthy(_t281)) {
        _t282 = _t281;
    } else {
        _t282 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t282)) {
#line 609 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 610 "compiler/main.gem"
    GemVal _t283[] = {gem_v_val};
    GemVal _t285;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t283, 1), gem_int(0)))) {
        _t285 = gem_gt(gem_len_fn(NULL, _t283, 1), gem_int(0));
    } else {
        GemVal _t284[] = {gem_v_val};
        _t285 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t284, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t285)) {
#line 611 "compiler/main.gem"
    GemVal _t286[] = {gem_v_val};
    GemVal _t287[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t286, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t287, 3);
                            }
                            break;
                        }
                    }
#line 617 "compiler/main.gem"
                    GemVal gem_v_tqrc = gem_table_get(gem_v_source, gem_v_pos);
#line 618 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("\n")))) {
#line 619 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 620 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 621 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 622 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(1);
                        continue;
                    }
#line 625 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("\\")))) {
#line 626 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 627 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 628 "compiler/main.gem"
    GemVal _t288[] = {gem_v_line};
    GemVal _t289[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t288, 1))};
                            (void)(gem_error_at_fn("compiler/main.gem", 628, _t289, 1));
                        }
#line 630 "compiler/main.gem"
                        GemVal gem_v_tqre = gem_table_get(gem_v_source, gem_v_pos);
#line 631 "compiler/main.gem"
    GemVal _t290[] = {gem_v_tqre, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t290, 2);
#line 632 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 633 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 635 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqre);
                        }
#line 637 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 640 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("{")))) {
#line 641 "compiler/main.gem"
    GemVal _t291 = gem_table_new();
    gem_table_set(_t291, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t291, gem_string("value"), gem_v_val);
    gem_table_set(_t291, gem_string("line"), gem_v_line);
    GemVal _t292[] = {gem_v_tokens, _t291};
                        (void)(gem_push_fn(NULL, _t292, 2));
#line 642 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 643 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 644 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 645 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 646 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 647 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 650 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqrc);
#line 651 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 654 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 655 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 656 "compiler/main.gem"
    GemVal _t293[] = {gem_v_line};
    GemVal _t294[] = {gem_add(gem_string("unterminated triple-quoted string interpolation at line "), gem_to_string_fn(NULL, _t293, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 656, _t294, 1));
                    }
#line 658 "compiler/main.gem"
    GemVal _t295 = gem_table_new();
    gem_table_set(_t295, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t295, gem_string("value"), gem_v_val);
    gem_table_set(_t295, gem_string("line"), gem_v_line);
    GemVal _t296[] = {gem_v_tokens, _t295};
                    (void)(gem_push_fn(NULL, _t296, 2));
#line 659 "compiler/main.gem"
    GemVal _t297 = gem_table_new();
    gem_table_set(_t297, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t297, gem_string("value"), gem_string(""));
    gem_table_set(_t297, gem_string("line"), gem_v_line);
    GemVal _t298[] = {gem_v_tokens, _t297};
                    (void)(gem_push_fn(NULL, _t298, 2));
                }
                continue;
            }
#line 665 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 666 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 667 "compiler/main.gem"
            while (1) {
                GemVal _t299;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t299 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t299 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t299)) break;
#line 668 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 669 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 670 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 671 "compiler/main.gem"
    GemVal _t300[] = {gem_v_line};
    GemVal _t301[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t300, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 671, _t301, 1));
                    }
#line 673 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 674 "compiler/main.gem"
    GemVal _t302[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t302, 2);
#line 675 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 676 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 678 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 680 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 683 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 685 "compiler/main.gem"
    GemVal _t303 = gem_table_new();
    gem_table_set(_t303, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t303, gem_string("value"), gem_v_val);
    gem_table_set(_t303, gem_string("line"), gem_v_line);
    GemVal _t304[] = {gem_v_tokens, _t303};
                    (void)(gem_push_fn(NULL, _t304, 2));
#line 686 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 687 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 688 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 689 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 690 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 693 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 694 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 696 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 697 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 698 "compiler/main.gem"
    GemVal _t305[] = {gem_v_line};
    GemVal _t306[] = {gem_add(gem_string("unterminated string interpolation at line "), gem_to_string_fn(NULL, _t305, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 698, _t306, 1));
                }
#line 700 "compiler/main.gem"
    GemVal _t307 = gem_table_new();
    gem_table_set(_t307, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t307, gem_string("value"), gem_v_val);
    gem_table_set(_t307, gem_string("line"), gem_v_line);
    GemVal _t308[] = {gem_v_tokens, _t307};
                (void)(gem_push_fn(NULL, _t308, 2));
#line 701 "compiler/main.gem"
    GemVal _t309 = gem_table_new();
    gem_table_set(_t309, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t309, gem_string("value"), gem_string(""));
    gem_table_set(_t309, gem_string("line"), gem_v_line);
    GemVal _t310[] = {gem_v_tokens, _t309};
                (void)(gem_push_fn(NULL, _t310, 2));
#line 702 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 707 "compiler/main.gem"
    GemVal _t311;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t311 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t311 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t312;
    if (gem_truthy(_t311)) {
        _t312 = _t311;
    } else {
        _t312 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t313;
    if (gem_truthy(_t312)) {
        _t313 = _t312;
    } else {
        _t313 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t314;
    if (gem_truthy(_t313)) {
        _t314 = _t313;
    } else {
        _t314 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t315;
    if (gem_truthy(_t314)) {
        _t315 = _t314;
    } else {
        _t315 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t315)) {
#line 708 "compiler/main.gem"
    GemVal _t316 = gem_table_new();
    gem_table_set(_t316, gem_string("type"), gem_v_ch);
    gem_table_set(_t316, gem_string("value"), gem_v_ch);
    gem_table_set(_t316, gem_string("line"), gem_v_line);
    GemVal _t317[] = {gem_v_tokens, _t316};
            (void)(gem_push_fn(NULL, _t317, 2));
#line 709 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 712 "compiler/main.gem"
    GemVal _t318;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t318 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t318 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    }
    GemVal _t319;
    if (!gem_truthy(_t318)) {
        _t319 = _t318;
    } else {
        _t319 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("."));
    }
    GemVal _t320;
    if (!gem_truthy(_t319)) {
        _t320 = _t319;
    } else {
        _t320 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    }
    GemVal _t321;
    if (!gem_truthy(_t320)) {
        _t321 = _t320;
    } else {
        _t321 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("."));
    }
        if (gem_truthy(_t321)) {
#line 713 "compiler/main.gem"
    GemVal _t322 = gem_table_new();
    gem_table_set(_t322, gem_string("type"), gem_string("..."));
    gem_table_set(_t322, gem_string("value"), gem_string("..."));
    gem_table_set(_t322, gem_string("line"), gem_v_line);
    GemVal _t323[] = {gem_v_tokens, _t322};
            (void)(gem_push_fn(NULL, _t323, 2));
#line 714 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
            continue;
        }
#line 717 "compiler/main.gem"
    GemVal _t324;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t324 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t324 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t325;
    if (gem_truthy(_t324)) {
        _t325 = _t324;
    } else {
        _t325 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t326;
    if (gem_truthy(_t325)) {
        _t326 = _t325;
    } else {
        _t326 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t326)) {
#line 718 "compiler/main.gem"
    GemVal _t327 = gem_table_new();
    gem_table_set(_t327, gem_string("type"), gem_v_ch);
    gem_table_set(_t327, gem_string("value"), gem_v_ch);
    gem_table_set(_t327, gem_string("line"), gem_v_line);
    GemVal _t328[] = {gem_v_tokens, _t327};
            (void)(gem_push_fn(NULL, _t328, 2));
#line 719 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 723 "compiler/main.gem"
    GemVal _t329[] = {gem_v_ch};
    GemVal _t330[] = {gem_v_line};
    GemVal _t331[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t329, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t330, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 723, _t331, 1));
    }
#line 726 "compiler/main.gem"
    GemVal _t332 = gem_table_new();
    gem_table_set(_t332, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t332, gem_string("value"), gem_string(""));
    gem_table_set(_t332, gem_string("line"), gem_v_line);
    GemVal _t333[] = {gem_v_tokens, _t332};
    (void)(gem_push_fn(NULL, _t333, 2));
    GemVal _t334 = gem_v_tokens;
    gem_pop_frame();
    return _t334;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 10 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_source", "compiler/main.gem", 10);
#line 11 "compiler/main.gem"
    GemVal _t335[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t335, 1);
#line 12 "compiler/main.gem"
    GemVal _t336[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t336, 1);
#line 13 "compiler/main.gem"
    GemVal _t337 = gem_v_p;
    GemVal _t338 = gem_table_get(_t337, gem_string("parse"));
    GemVal _t339 = _t338.fn(_t338.env, NULL, 0);
    gem_pop_frame();
    return _t339;
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
    GemVal _t340 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t340;
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
    GemVal _t342 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t342;
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
    GemVal _t344 = gem_v_t;
    gem_pop_frame();
    return _t344;
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
    GemVal _t346 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t347 = gem_eq(gem_table_get(_t346, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t347;
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
    GemVal _t349 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t349, gem_string("type")), gem_v_tp))) {
#line 46 "compiler/main.gem"
    GemVal _t350[] = {gem_v_tp};
    GemVal _t351 = gem_v_t;
    GemVal _t352[] = {gem_table_get(_t351, gem_string("type"))};
    GemVal _t353 = gem_v_t;
    GemVal _t354[] = {gem_table_get(_t353, gem_string("line"))};
    GemVal _t355[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t350, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t352, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t354, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 46, _t355, 1));
    }
#line 48 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t356 = gem_v_t;
    gem_pop_frame();
    return _t356;
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
        GemVal _t358 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t358, gem_string("type")), gem_string("NEWLINE")))) break;
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
    GemVal _t360 = gem_table_new();
    GemVal gem_v_params = _t360;
#line 66 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 67 "compiler/main.gem"
    GemVal gem_v_block_param = GEM_NIL;
#line 69 "compiler/main.gem"
    while (1) {
        GemVal _t361 = (*gem_v_peek);
        GemVal _t362 = _t361.fn(_t361.env, NULL, 0);
        GemVal _t365;
        if (gem_truthy(gem_eq(gem_table_get(_t362, gem_string("type")), gem_string("NAME")))) {
                _t365 = gem_eq(gem_table_get(_t362, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t363 = (*gem_v_peek);
                GemVal _t364 = _t363.fn(_t363.env, NULL, 0);
                _t365 = gem_eq(gem_table_get(_t364, gem_string("type")), gem_string("..."));
        }
        if (!gem_truthy(_t365)) break;
#line 70 "compiler/main.gem"
    GemVal _t366 = (*gem_v_peek);
    GemVal _t367 = _t366.fn(_t366.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t367, gem_string("type")), gem_string("...")))) {
#line 71 "compiler/main.gem"
    GemVal _t368 = (*gem_v_advance);
            (void)(_t368.fn(_t368.env, NULL, 0));
#line 72 "compiler/main.gem"
    GemVal _t369[] = {gem_string("NAME")};
    GemVal _t370 = (*gem_v_expect);
    GemVal _t371 = _t370.fn(_t370.env, _t369, 1);
            gem_v_rest_param = gem_table_get(_t371, gem_string("value"));
#line 73 "compiler/main.gem"
    GemVal _t372 = (*gem_v_peek);
    GemVal _t373 = _t372.fn(_t372.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t373, gem_string("type")), gem_string(",")))) {
#line 74 "compiler/main.gem"
    GemVal _t374 = (*gem_v_advance);
                (void)(_t374.fn(_t374.env, NULL, 0));
#line 75 "compiler/main.gem"
    GemVal _t375 = (*gem_v_peek);
    GemVal _t376 = _t375.fn(_t375.env, NULL, 0);
                if (gem_truthy(gem_neq(gem_table_get(_t376, gem_string("type")), gem_string("NAME")))) {
#line 76 "compiler/main.gem"
    GemVal _t377 = (*gem_v_peek);
    GemVal _t378 = _t377.fn(_t377.env, NULL, 0);
    GemVal _t379[] = {gem_table_get(_t378, gem_string("line"))};
    GemVal _t380[] = {gem_add(gem_string("expected parameter name after rest param at line "), gem_to_string_fn(NULL, _t379, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 76, _t380, 1));
                }
#line 78 "compiler/main.gem"
    GemVal _t381 = (*gem_v_advance);
    GemVal _t382 = _t381.fn(_t381.env, NULL, 0);
                gem_v_block_param = gem_table_get(_t382, gem_string("value"));
#line 79 "compiler/main.gem"
    GemVal _t383 = (*gem_v_peek);
    GemVal _t384 = _t383.fn(_t383.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t384, gem_string("type")), gem_string(",")))) {
#line 80 "compiler/main.gem"
    GemVal _t385 = (*gem_v_peek);
    GemVal _t386 = _t385.fn(_t385.env, NULL, 0);
    GemVal _t387[] = {gem_table_get(_t386, gem_string("line"))};
    GemVal _t388[] = {gem_add(gem_string("only one parameter is allowed after a rest param at line "), gem_to_string_fn(NULL, _t387, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 80, _t388, 1));
                }
            }
            break;
        }
#line 85 "compiler/main.gem"
    GemVal _t389 = (*gem_v_advance);
    GemVal _t390 = _t389.fn(_t389.env, NULL, 0);
    GemVal _t391[] = {gem_v_params, gem_table_get(_t390, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t391, 2));
#line 86 "compiler/main.gem"
    GemVal _t392 = (*gem_v_peek);
    GemVal _t393 = _t392.fn(_t392.env, NULL, 0);
        if (gem_truthy(gem_neq(gem_table_get(_t393, gem_string("type")), gem_string(",")))) {
            break;
        }
#line 89 "compiler/main.gem"
    GemVal _t394 = (*gem_v_advance);
        (void)(_t394.fn(_t394.env, NULL, 0));
    }
    GemVal _t395 = gem_table_new();
    gem_table_set(_t395, gem_string("params"), gem_v_params);
    gem_table_set(_t395, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t395, gem_string("block_param"), gem_v_block_param);
    GemVal _t396 = _t395;
    gem_pop_frame();
    return _t396;
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
#line 108 "compiler/main.gem"
    GemVal _t398 = gem_table_new();
    GemVal gem_v_stmts = _t398;
#line 109 "compiler/main.gem"
    GemVal _t399 = (*gem_v_skip_nl);
    (void)(_t399.fn(_t399.env, NULL, 0));
#line 110 "compiler/main.gem"
    while (1) {
        GemVal _t400 = (*gem_v_peek);
        GemVal _t401 = _t400.fn(_t400.env, NULL, 0);
        GemVal _t404;
        if (!gem_truthy(gem_neq(gem_table_get(_t401, gem_string("type")), gem_string("end")))) {
                _t404 = gem_neq(gem_table_get(_t401, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t402 = (*gem_v_peek);
                GemVal _t403 = _t402.fn(_t402.env, NULL, 0);
                _t404 = gem_neq(gem_table_get(_t403, gem_string("type")), gem_string("elif"));
        }
        GemVal _t407;
        if (!gem_truthy(_t404)) {
                _t407 = _t404;
        } else {
                GemVal _t405 = (*gem_v_peek);
                GemVal _t406 = _t405.fn(_t405.env, NULL, 0);
                _t407 = gem_neq(gem_table_get(_t406, gem_string("type")), gem_string("else"));
        }
        GemVal _t410;
        if (!gem_truthy(_t407)) {
                _t410 = _t407;
        } else {
                GemVal _t408 = (*gem_v_peek);
                GemVal _t409 = _t408.fn(_t408.env, NULL, 0);
                _t410 = gem_neq(gem_table_get(_t409, gem_string("type")), gem_string("when"));
        }
        GemVal _t413;
        if (!gem_truthy(_t410)) {
                _t413 = _t410;
        } else {
                GemVal _t411 = (*gem_v_peek);
                GemVal _t412 = _t411.fn(_t411.env, NULL, 0);
                _t413 = gem_neq(gem_table_get(_t412, gem_string("type")), gem_string("after"));
        }
        GemVal _t415;
        if (!gem_truthy(_t413)) {
                _t415 = _t413;
        } else {
                GemVal _t414 = (*gem_v_at_end);
                _t415 = gem_not(_t414.fn(_t414.env, NULL, 0));
        }
        if (!gem_truthy(_t415)) break;
#line 111 "compiler/main.gem"
    GemVal _t416 = (*gem_v_parse_stmt);
    GemVal _t417[] = {gem_v_stmts, _t416.fn(_t416.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t417, 2));
#line 112 "compiler/main.gem"
    GemVal _t418 = (*gem_v_skip_nl);
        (void)(_t418.fn(_t418.env, NULL, 0));
    }
    GemVal _t419 = gem_v_stmts;
    gem_pop_frame();
    return _t419;
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
#line 121 "compiler/main.gem"
    GemVal _t421 = gem_table_new();
    GemVal gem_v_stmts = _t421;
#line 122 "compiler/main.gem"
    GemVal _t422 = (*gem_v_skip_nl);
    (void)(_t422.fn(_t422.env, NULL, 0));
#line 123 "compiler/main.gem"
    while (1) {
        GemVal _t423 = (*gem_v_peek);
        GemVal _t424 = _t423.fn(_t423.env, NULL, 0);
        GemVal _t426;
        if (!gem_truthy(gem_neq(gem_table_get(_t424, gem_string("type")), gem_string("end")))) {
                _t426 = gem_neq(gem_table_get(_t424, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t425 = (*gem_v_at_end);
                _t426 = gem_not(_t425.fn(_t425.env, NULL, 0));
        }
        if (!gem_truthy(_t426)) break;
#line 124 "compiler/main.gem"
    GemVal _t427 = (*gem_v_parse_stmt);
    GemVal _t428[] = {gem_v_stmts, _t427.fn(_t427.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t428, 2));
#line 125 "compiler/main.gem"
    GemVal _t429 = (*gem_v_skip_nl);
        (void)(_t429.fn(_t429.env, NULL, 0));
    }
    GemVal _t430 = gem_v_stmts;
    gem_pop_frame();
    return _t430;
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
#line 134 "compiler/main.gem"
    GemVal _t432 = (*gem_v_peek);
    GemVal gem_v_t = _t432.fn(_t432.env, NULL, 0);
#line 137 "compiler/main.gem"
    GemVal _t433 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t433, gem_string("type")), gem_string("NUMBER")))) {
#line 138 "compiler/main.gem"
    GemVal _t434 = (*gem_v_advance);
        (void)(_t434.fn(_t434.env, NULL, 0));
#line 139 "compiler/main.gem"
    GemVal _t435 = gem_v_t;
    GemVal _t436[] = {gem_table_get(_t435, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t436, 1))) {
#line 140 "compiler/main.gem"
    GemVal _t437 = gem_v_t;
    GemVal _t438[] = {gem_table_get(_t437, gem_string("value"))};
    GemVal _t439[] = {gem_fn_atof(NULL, _t438, 1)};
            GemVal _t440 = gem_fn_make_float(NULL, _t439, 1);
            gem_pop_frame();
            return _t440;
        }
#line 142 "compiler/main.gem"
    GemVal _t441 = gem_v_t;
    GemVal _t442[] = {gem_table_get(_t441, gem_string("value"))};
    GemVal _t443[] = {gem_fn_str_to_int(NULL, _t442, 1)};
        GemVal _t444 = gem_fn_make_int(NULL, _t443, 1);
        gem_pop_frame();
        return _t444;
    }
#line 146 "compiler/main.gem"
    GemVal _t445 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t445, gem_string("type")), gem_string("STRING")))) {
#line 147 "compiler/main.gem"
    GemVal _t446 = (*gem_v_advance);
        (void)(_t446.fn(_t446.env, NULL, 0));
#line 148 "compiler/main.gem"
    GemVal _t447 = gem_v_t;
    GemVal _t448[] = {gem_table_get(_t447, gem_string("value"))};
        GemVal _t449 = gem_fn_make_string(NULL, _t448, 1);
        gem_pop_frame();
        return _t449;
    }
#line 152 "compiler/main.gem"
    GemVal _t450 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t450, gem_string("type")), gem_string("INTERP_START")))) {
#line 153 "compiler/main.gem"
    GemVal _t451 = (*gem_v_advance);
        (void)(_t451.fn(_t451.env, NULL, 0));
#line 154 "compiler/main.gem"
    GemVal _t452 = gem_table_new();
        GemVal gem_v_parts = _t452;
#line 155 "compiler/main.gem"
    GemVal _t453 = (*gem_v_skip_nl);
        (void)(_t453.fn(_t453.env, NULL, 0));
#line 156 "compiler/main.gem"
        while (1) {
            GemVal _t454 = (*gem_v_peek);
            GemVal _t455 = _t454.fn(_t454.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t455, gem_string("type")), gem_string("INTERP_END")))) break;
#line 157 "compiler/main.gem"
    GemVal _t456 = (*gem_v_peek);
    GemVal _t457 = _t456.fn(_t456.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t457, gem_string("type")), gem_string("STRING")))) {
#line 158 "compiler/main.gem"
    GemVal _t458 = (*gem_v_advance);
    GemVal _t459 = _t458.fn(_t458.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t459, gem_string("value"));
#line 159 "compiler/main.gem"
    GemVal _t460[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t460, 1), gem_int(0)))) {
#line 160 "compiler/main.gem"
    GemVal _t461[] = {gem_v_sval};
    GemVal _t462[] = {gem_v_parts, gem_fn_make_string(NULL, _t461, 1)};
                    (void)(gem_push_fn(NULL, _t462, 2));
                }
            } else {
#line 163 "compiler/main.gem"
    GemVal _t463 = (*gem_v_parse_expr);
    GemVal _t464[] = {gem_v_parts, _t463.fn(_t463.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t464, 2));
            }
#line 165 "compiler/main.gem"
    GemVal _t465 = (*gem_v_skip_nl);
            (void)(_t465.fn(_t465.env, NULL, 0));
        }
#line 167 "compiler/main.gem"
    GemVal _t466[] = {gem_string("INTERP_END")};
    GemVal _t467 = (*gem_v_expect);
        (void)(_t467.fn(_t467.env, _t466, 1));
#line 168 "compiler/main.gem"
    GemVal _t468[] = {gem_v_parts};
        GemVal _t469 = gem_fn_make_interp(NULL, _t468, 1);
        gem_pop_frame();
        return _t469;
    }
#line 172 "compiler/main.gem"
    GemVal _t470 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t470, gem_string("type")), gem_string("true")))) {
#line 173 "compiler/main.gem"
    GemVal _t471 = (*gem_v_advance);
        (void)(_t471.fn(_t471.env, NULL, 0));
#line 174 "compiler/main.gem"
    GemVal _t472[] = {gem_bool(1)};
        GemVal _t473 = gem_fn_make_bool(NULL, _t472, 1);
        gem_pop_frame();
        return _t473;
    }
#line 176 "compiler/main.gem"
    GemVal _t474 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t474, gem_string("type")), gem_string("false")))) {
#line 177 "compiler/main.gem"
    GemVal _t475 = (*gem_v_advance);
        (void)(_t475.fn(_t475.env, NULL, 0));
#line 178 "compiler/main.gem"
    GemVal _t476[] = {gem_bool(0)};
        GemVal _t477 = gem_fn_make_bool(NULL, _t476, 1);
        gem_pop_frame();
        return _t477;
    }
#line 182 "compiler/main.gem"
    GemVal _t478 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t478, gem_string("type")), gem_string("nil")))) {
#line 183 "compiler/main.gem"
    GemVal _t479 = (*gem_v_advance);
        (void)(_t479.fn(_t479.env, NULL, 0));
#line 184 "compiler/main.gem"
        GemVal _t480 = gem_fn_make_nil_node(NULL, NULL, 0);
        gem_pop_frame();
        return _t480;
    }
#line 188 "compiler/main.gem"
    GemVal _t481 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t481, gem_string("type")), gem_string("fn")))) {
#line 189 "compiler/main.gem"
    GemVal _t482 = (*gem_v_advance);
        (void)(_t482.fn(_t482.env, NULL, 0));
#line 190 "compiler/main.gem"
    GemVal _t483[] = {gem_string("(")};
    GemVal _t484 = (*gem_v_expect);
        (void)(_t484.fn(_t484.env, _t483, 1));
#line 191 "compiler/main.gem"
    GemVal _t485 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t485.fn(_t485.env, NULL, 0);
#line 192 "compiler/main.gem"
    GemVal _t486[] = {gem_string(")")};
    GemVal _t487 = (*gem_v_expect);
        (void)(_t487.fn(_t487.env, _t486, 1));
#line 193 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 194 "compiler/main.gem"
    GemVal _t488 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t488.fn(_t488.env, NULL, 0);
#line 195 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 196 "compiler/main.gem"
    GemVal _t489[] = {gem_string("end")};
    GemVal _t490 = (*gem_v_expect);
        (void)(_t490.fn(_t490.env, _t489, 1));
#line 197 "compiler/main.gem"
    GemVal _t491 = gem_v_pr;
    GemVal _t492 = gem_v_pr;
    GemVal _t493 = gem_v_pr;
    GemVal _t494[] = {gem_table_get(_t491, gem_string("params")), gem_table_get(_t492, gem_string("rest_param")), gem_table_get(_t493, gem_string("block_param")), gem_v_body};
        GemVal _t495 = gem_fn_make_anon_fn(NULL, _t494, 4);
        gem_pop_frame();
        return _t495;
    }
#line 202 "compiler/main.gem"
    GemVal _t496 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t496, gem_string("type")), gem_string("{")))) {
#line 203 "compiler/main.gem"
    GemVal _t497 = (*gem_v_advance);
        (void)(_t497.fn(_t497.env, NULL, 0));
#line 204 "compiler/main.gem"
    GemVal _t498 = (*gem_v_skip_nl);
        (void)(_t498.fn(_t498.env, NULL, 0));
#line 205 "compiler/main.gem"
    GemVal _t499 = gem_table_new();
        GemVal gem_v_entries = _t499;
#line 206 "compiler/main.gem"
        while (1) {
            GemVal _t500 = (*gem_v_peek);
            GemVal _t501 = _t500.fn(_t500.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t501, gem_string("type")), gem_string("}")))) break;
#line 207 "compiler/main.gem"
    GemVal _t502 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t502.fn(_t502.env, NULL, 0);
#line 208 "compiler/main.gem"
    GemVal _t503 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t503, gem_string("type")), gem_string("NAME")))) {
#line 209 "compiler/main.gem"
    GemVal _t504 = (*gem_v_advance);
                (void)(_t504.fn(_t504.env, NULL, 0));
            } else {
#line 210 "compiler/main.gem"
    GemVal _t505[] = {gem_int(1)};
    GemVal _t506 = (*gem_v_peek_at);
    GemVal _t507 = _t506.fn(_t506.env, _t505, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t507, gem_string("type")), gem_string(":")))) {
#line 212 "compiler/main.gem"
    GemVal _t508 = (*gem_v_advance);
                    (void)(_t508.fn(_t508.env, NULL, 0));
                } else {
#line 214 "compiler/main.gem"
    GemVal _t509 = gem_v_key_tok;
    GemVal _t510[] = {gem_table_get(_t509, gem_string("type"))};
    GemVal _t511 = gem_v_key_tok;
    GemVal _t512[] = {gem_table_get(_t511, gem_string("line"))};
    GemVal _t513[] = {gem_add(gem_add(gem_add(gem_string("expected table key (name or keyword) but got '"), gem_to_string_fn(NULL, _t510, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t512, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 214, _t513, 1));
                }
            }
#line 216 "compiler/main.gem"
    GemVal _t514[] = {gem_string(":")};
    GemVal _t515 = (*gem_v_expect);
            (void)(_t515.fn(_t515.env, _t514, 1));
#line 217 "compiler/main.gem"
    GemVal _t516 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t516.fn(_t516.env, NULL, 0);
#line 218 "compiler/main.gem"
    GemVal _t517 = gem_v_key_tok;
    GemVal _t518[] = {gem_table_get(_t517, gem_string("value")), gem_v_val};
    GemVal _t519[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t518, 2)};
            (void)(gem_push_fn(NULL, _t519, 2));
#line 219 "compiler/main.gem"
    GemVal _t520 = (*gem_v_skip_nl);
            (void)(_t520.fn(_t520.env, NULL, 0));
#line 220 "compiler/main.gem"
    GemVal _t521 = (*gem_v_peek);
    GemVal _t522 = _t521.fn(_t521.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t522, gem_string("type")), gem_string(",")))) {
#line 221 "compiler/main.gem"
    GemVal _t523 = (*gem_v_advance);
                (void)(_t523.fn(_t523.env, NULL, 0));
#line 222 "compiler/main.gem"
    GemVal _t524 = (*gem_v_skip_nl);
                (void)(_t524.fn(_t524.env, NULL, 0));
            }
        }
#line 225 "compiler/main.gem"
    GemVal _t525[] = {gem_string("}")};
    GemVal _t526 = (*gem_v_expect);
        (void)(_t526.fn(_t526.env, _t525, 1));
#line 226 "compiler/main.gem"
    GemVal _t527[] = {gem_v_entries};
        GemVal _t528 = gem_fn_make_table(NULL, _t527, 1);
        gem_pop_frame();
        return _t528;
    }
#line 230 "compiler/main.gem"
    GemVal _t529 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t529, gem_string("type")), gem_string("[")))) {
#line 231 "compiler/main.gem"
    GemVal _t530 = (*gem_v_advance);
        (void)(_t530.fn(_t530.env, NULL, 0));
#line 232 "compiler/main.gem"
    GemVal _t531 = (*gem_v_skip_nl);
        (void)(_t531.fn(_t531.env, NULL, 0));
#line 233 "compiler/main.gem"
    GemVal _t532 = gem_table_new();
        GemVal gem_v_elements = _t532;
#line 234 "compiler/main.gem"
        while (1) {
            GemVal _t533 = (*gem_v_peek);
            GemVal _t534 = _t533.fn(_t533.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t534, gem_string("type")), gem_string("]")))) break;
#line 235 "compiler/main.gem"
    GemVal _t535 = (*gem_v_parse_expr);
    GemVal _t536[] = {gem_v_elements, _t535.fn(_t535.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t536, 2));
#line 236 "compiler/main.gem"
    GemVal _t537 = (*gem_v_skip_nl);
            (void)(_t537.fn(_t537.env, NULL, 0));
#line 237 "compiler/main.gem"
    GemVal _t538 = (*gem_v_peek);
    GemVal _t539 = _t538.fn(_t538.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t539, gem_string("type")), gem_string(",")))) {
#line 238 "compiler/main.gem"
    GemVal _t540 = (*gem_v_advance);
                (void)(_t540.fn(_t540.env, NULL, 0));
#line 239 "compiler/main.gem"
    GemVal _t541 = (*gem_v_skip_nl);
                (void)(_t541.fn(_t541.env, NULL, 0));
            }
        }
#line 242 "compiler/main.gem"
    GemVal _t542[] = {gem_string("]")};
    GemVal _t543 = (*gem_v_expect);
        (void)(_t543.fn(_t543.env, _t542, 1));
#line 243 "compiler/main.gem"
    GemVal _t544[] = {gem_v_elements};
        GemVal _t545 = gem_fn_make_array(NULL, _t544, 1);
        gem_pop_frame();
        return _t545;
    }
#line 247 "compiler/main.gem"
    GemVal _t546 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t546, gem_string("type")), gem_string("(")))) {
#line 248 "compiler/main.gem"
    GemVal _t547 = (*gem_v_advance);
        (void)(_t547.fn(_t547.env, NULL, 0));
#line 249 "compiler/main.gem"
    GemVal _t548 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t548.fn(_t548.env, NULL, 0);
#line 250 "compiler/main.gem"
    GemVal _t549[] = {gem_string(")")};
    GemVal _t550 = (*gem_v_expect);
        (void)(_t550.fn(_t550.env, _t549, 1));
#line 251 "compiler/main.gem"
        GemVal _t551 = gem_v_e;
        gem_pop_frame();
        return _t551;
    }
#line 255 "compiler/main.gem"
    GemVal _t552 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t552, gem_string("type")), gem_string("NAME")))) {
#line 256 "compiler/main.gem"
    GemVal _t553 = (*gem_v_advance);
        (void)(_t553.fn(_t553.env, NULL, 0));
#line 257 "compiler/main.gem"
    GemVal _t554 = gem_v_t;
    GemVal _t555[] = {gem_table_get(_t554, gem_string("value"))};
        GemVal _t556 = gem_fn_make_var(NULL, _t555, 1);
        gem_pop_frame();
        return _t556;
    }
#line 260 "compiler/main.gem"
    GemVal _t557 = gem_v_t;
    GemVal _t558[] = {gem_table_get(_t557, gem_string("type"))};
    GemVal _t559 = gem_v_t;
    GemVal _t560[] = {gem_table_get(_t559, gem_string("line"))};
    GemVal _t561[] = {gem_add(gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t558, 1)), gem_string("' (expected expression) at line ")), gem_to_string_fn(NULL, _t560, 1))};
    GemVal _t562 = gem_error_at_fn("compiler/main.gem", 260, _t561, 1);
    gem_pop_frame();
    return _t562;
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
#line 265 "compiler/main.gem"
    GemVal _t564 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t564.fn(_t564.env, NULL, 0);
#line 267 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
#line 269 "compiler/main.gem"
    GemVal _t565 = (*gem_v_peek);
    GemVal _t566 = _t565.fn(_t565.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t566, gem_string("type")), gem_string("(")))) {
#line 270 "compiler/main.gem"
    GemVal _t567 = (*gem_v_peek);
    GemVal _t568 = _t567.fn(_t567.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t568, gem_string("line"));
#line 271 "compiler/main.gem"
    GemVal _t569 = (*gem_v_advance);
            (void)(_t569.fn(_t569.env, NULL, 0));
#line 272 "compiler/main.gem"
    GemVal _t570 = (*gem_v_skip_nl);
            (void)(_t570.fn(_t570.env, NULL, 0));
#line 273 "compiler/main.gem"
    GemVal _t571 = gem_table_new();
            GemVal gem_v_args = _t571;
#line 274 "compiler/main.gem"
    GemVal _t572 = (*gem_v_peek);
    GemVal _t573 = _t572.fn(_t572.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t573, gem_string("type")), gem_string(")")))) {
#line 275 "compiler/main.gem"
    GemVal _t574 = (*gem_v_parse_expr);
    GemVal _t575[] = {gem_v_args, _t574.fn(_t574.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t575, 2));
#line 276 "compiler/main.gem"
                while (1) {
                    GemVal _t576 = (*gem_v_peek);
                    GemVal _t577 = _t576.fn(_t576.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t577, gem_string("type")), gem_string(",")))) break;
#line 277 "compiler/main.gem"
    GemVal _t578 = (*gem_v_advance);
                    (void)(_t578.fn(_t578.env, NULL, 0));
#line 278 "compiler/main.gem"
    GemVal _t579 = (*gem_v_skip_nl);
                    (void)(_t579.fn(_t579.env, NULL, 0));
#line 279 "compiler/main.gem"
    GemVal _t580 = (*gem_v_parse_expr);
    GemVal _t581[] = {gem_v_args, _t580.fn(_t580.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t581, 2));
                }
            }
#line 282 "compiler/main.gem"
    GemVal _t582 = (*gem_v_skip_nl);
            (void)(_t582.fn(_t582.env, NULL, 0));
#line 283 "compiler/main.gem"
    GemVal _t583[] = {gem_string(")")};
    GemVal _t584 = (*gem_v_expect);
            (void)(_t584.fn(_t584.env, _t583, 1));
#line 286 "compiler/main.gem"
    GemVal _t585 = (*gem_v_peek);
    GemVal _t586 = _t585.fn(_t585.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t586, gem_string("type")), gem_string("do")))) {
#line 287 "compiler/main.gem"
    GemVal _t587 = (*gem_v_advance);
                (void)(_t587.fn(_t587.env, NULL, 0));
#line 288 "compiler/main.gem"
    GemVal _t588 = gem_table_new();
                GemVal gem_v_bparams = _t588;
#line 289 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 290 "compiler/main.gem"
                GemVal gem_v_bblock = GEM_NIL;
#line 291 "compiler/main.gem"
    GemVal _t589 = (*gem_v_peek);
    GemVal _t590 = _t589.fn(_t589.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t590, gem_string("type")), gem_string("|")))) {
#line 292 "compiler/main.gem"
    GemVal _t591 = (*gem_v_advance);
                    (void)(_t591.fn(_t591.env, NULL, 0));
#line 293 "compiler/main.gem"
    GemVal _t592 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t592.fn(_t592.env, NULL, 0);
#line 294 "compiler/main.gem"
    GemVal _t593 = gem_v_bpr;
                    gem_v_bparams = gem_table_get(_t593, gem_string("params"));
#line 295 "compiler/main.gem"
    GemVal _t594 = gem_v_bpr;
                    gem_v_brest = gem_table_get(_t594, gem_string("rest_param"));
#line 296 "compiler/main.gem"
    GemVal _t595 = gem_v_bpr;
                    gem_v_bblock = gem_table_get(_t595, gem_string("block_param"));
#line 297 "compiler/main.gem"
    GemVal _t596[] = {gem_string("|")};
    GemVal _t597 = (*gem_v_expect);
                    (void)(_t597.fn(_t597.env, _t596, 1));
                }
#line 299 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 300 "compiler/main.gem"
    GemVal _t598 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t598.fn(_t598.env, NULL, 0);
#line 301 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 302 "compiler/main.gem"
    GemVal _t599[] = {gem_string("end")};
    GemVal _t600 = (*gem_v_expect);
                (void)(_t600.fn(_t600.env, _t599, 1));
#line 303 "compiler/main.gem"
    GemVal _t601[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, gem_v_body};
    GemVal _t602[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t601, 4)};
                (void)(gem_push_fn(NULL, _t602, 2));
            } else {
#line 307 "compiler/main.gem"
    GemVal _t603 = (*gem_v_peek);
    GemVal _t604 = _t603.fn(_t603.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t604, gem_string("type")), gem_string("{")))) {
#line 308 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 309 "compiler/main.gem"
    GemVal _t605[] = {gem_int(1)};
    GemVal _t606 = (*gem_v_peek_at);
    GemVal _t607 = _t606.fn(_t606.env, _t605, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t607, gem_string("type")), gem_string("|")))) {
#line 310 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 311 "compiler/main.gem"
    GemVal _t608[] = {gem_int(1)};
    GemVal _t609 = (*gem_v_peek_at);
    GemVal _t610 = _t609.fn(_t609.env, _t608, 1);
    GemVal _t618;
    if (gem_truthy(gem_eq(gem_table_get(_t610, gem_string("type")), gem_string("}")))) {
        _t618 = gem_eq(gem_table_get(_t610, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t611[] = {gem_int(1)};
        GemVal _t612 = (*gem_v_peek_at);
        GemVal _t613 = _t612.fn(_t612.env, _t611, 1);
        GemVal _t617;
        if (!gem_truthy(gem_eq(gem_table_get(_t613, gem_string("type")), gem_string("NAME")))) {
                _t617 = gem_eq(gem_table_get(_t613, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t614[] = {gem_int(2)};
                GemVal _t615 = (*gem_v_peek_at);
                GemVal _t616 = _t615.fn(_t615.env, _t614, 1);
                _t617 = gem_eq(gem_table_get(_t616, gem_string("type")), gem_string(":"));
        }
        _t618 = _t617;
    }
                        if (gem_truthy(_t618)) {
#line 312 "compiler/main.gem"
                            gem_v_is_block = gem_bool(0);
                        } else {
#line 314 "compiler/main.gem"
                            gem_v_is_block = gem_bool(1);
                        }
                    }
#line 316 "compiler/main.gem"
                    if (gem_truthy(gem_v_is_block)) {
#line 317 "compiler/main.gem"
    GemVal _t619 = (*gem_v_advance);
                        (void)(_t619.fn(_t619.env, NULL, 0));
#line 318 "compiler/main.gem"
    GemVal _t620 = gem_table_new();
                        GemVal gem_v_bparams = _t620;
#line 319 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 320 "compiler/main.gem"
                        GemVal gem_v_bblock = GEM_NIL;
#line 321 "compiler/main.gem"
    GemVal _t621 = (*gem_v_peek);
    GemVal _t622 = _t621.fn(_t621.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t622, gem_string("type")), gem_string("|")))) {
#line 322 "compiler/main.gem"
    GemVal _t623 = (*gem_v_advance);
                            (void)(_t623.fn(_t623.env, NULL, 0));
#line 323 "compiler/main.gem"
    GemVal _t624 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t624.fn(_t624.env, NULL, 0);
#line 324 "compiler/main.gem"
    GemVal _t625 = gem_v_bpr;
                            gem_v_bparams = gem_table_get(_t625, gem_string("params"));
#line 325 "compiler/main.gem"
    GemVal _t626 = gem_v_bpr;
                            gem_v_brest = gem_table_get(_t626, gem_string("rest_param"));
#line 326 "compiler/main.gem"
    GemVal _t627 = gem_v_bpr;
                            gem_v_bblock = gem_table_get(_t627, gem_string("block_param"));
#line 327 "compiler/main.gem"
    GemVal _t628[] = {gem_string("|")};
    GemVal _t629 = (*gem_v_expect);
                            (void)(_t629.fn(_t629.env, _t628, 1));
                        }
#line 329 "compiler/main.gem"
    GemVal _t630 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t630.fn(_t630.env, NULL, 0);
#line 330 "compiler/main.gem"
    GemVal _t631[] = {gem_string("}")};
    GemVal _t632 = (*gem_v_expect);
                        (void)(_t632.fn(_t632.env, _t631, 1));
#line 331 "compiler/main.gem"
    GemVal _t633 = gem_table_new();
    gem_table_set(_t633, gem_int(0), gem_v_bexpr);
    GemVal _t634[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, _t633};
    GemVal _t635[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t634, 4)};
                        (void)(gem_push_fn(NULL, _t635, 2));
                    }
                }
            }
#line 336 "compiler/main.gem"
    GemVal _t636[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t636, 3);
            continue;
        }
#line 341 "compiler/main.gem"
    GemVal _t637 = (*gem_v_peek);
    GemVal _t638 = _t637.fn(_t637.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t638, gem_string("type")), gem_string(".")))) {
#line 342 "compiler/main.gem"
    GemVal _t639 = (*gem_v_advance);
            (void)(_t639.fn(_t639.env, NULL, 0));
#line 343 "compiler/main.gem"
    GemVal _t640 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t640.fn(_t640.env, NULL, 0);
#line 344 "compiler/main.gem"
    GemVal _t641 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t641, gem_string("type")), gem_string("NAME")))) {
#line 345 "compiler/main.gem"
    GemVal _t642 = (*gem_v_advance);
                (void)(_t642.fn(_t642.env, NULL, 0));
            } else {
#line 348 "compiler/main.gem"
    GemVal _t643 = (*gem_v_advance);
                (void)(_t643.fn(_t643.env, NULL, 0));
            }
#line 350 "compiler/main.gem"
    GemVal _t644 = gem_v_field_tok;
    GemVal _t645[] = {gem_v_node, gem_table_get(_t644, gem_string("value"))};
            gem_v_node = gem_fn_make_dot(NULL, _t645, 2);
            continue;
        }
#line 355 "compiler/main.gem"
    GemVal _t646 = (*gem_v_peek);
    GemVal _t647 = _t646.fn(_t646.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t647, gem_string("type")), gem_string("[")))) {
#line 356 "compiler/main.gem"
    GemVal _t648 = (*gem_v_advance);
            (void)(_t648.fn(_t648.env, NULL, 0));
#line 357 "compiler/main.gem"
    GemVal _t649 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t649.fn(_t649.env, NULL, 0);
#line 358 "compiler/main.gem"
    GemVal _t650[] = {gem_string("]")};
    GemVal _t651 = (*gem_v_expect);
            (void)(_t651.fn(_t651.env, _t650, 1));
#line 359 "compiler/main.gem"
    GemVal _t652[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t652, 2);
            continue;
        }
        break;
    }
    GemVal _t653 = gem_v_node;
    gem_pop_frame();
    return _t653;
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
#line 371 "compiler/main.gem"
    GemVal _t655 = (*gem_v_peek);
    GemVal _t656 = _t655.fn(_t655.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t656, gem_string("type")), gem_string("-")))) {
#line 372 "compiler/main.gem"
    GemVal _t657 = (*gem_v_advance);
        (void)(_t657.fn(_t657.env, NULL, 0));
#line 373 "compiler/main.gem"
    GemVal _t658 = (*gem_v_parse_unary);
    GemVal _t659[] = {gem_string("-"), _t658.fn(_t658.env, NULL, 0)};
        GemVal _t660 = gem_fn_make_unop(NULL, _t659, 2);
        gem_pop_frame();
        return _t660;
    }
#line 375 "compiler/main.gem"
    GemVal _t661 = (*gem_v_parse_call);
    GemVal _t662 = _t661.fn(_t661.env, NULL, 0);
    gem_pop_frame();
    return _t662;
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
#line 380 "compiler/main.gem"
    GemVal _t664 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t664.fn(_t664.env, NULL, 0);
#line 381 "compiler/main.gem"
    while (1) {
        GemVal _t665 = (*gem_v_peek);
        GemVal _t666 = _t665.fn(_t665.env, NULL, 0);
        GemVal _t669;
        if (gem_truthy(gem_eq(gem_table_get(_t666, gem_string("type")), gem_string("*")))) {
                _t669 = gem_eq(gem_table_get(_t666, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t667 = (*gem_v_peek);
                GemVal _t668 = _t667.fn(_t667.env, NULL, 0);
                _t669 = gem_eq(gem_table_get(_t668, gem_string("type")), gem_string("/"));
        }
        GemVal _t672;
        if (gem_truthy(_t669)) {
                _t672 = _t669;
        } else {
                GemVal _t670 = (*gem_v_peek);
                GemVal _t671 = _t670.fn(_t670.env, NULL, 0);
                _t672 = gem_eq(gem_table_get(_t671, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t672)) break;
#line 382 "compiler/main.gem"
    GemVal _t673 = (*gem_v_advance);
    GemVal _t674 = _t673.fn(_t673.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t674, gem_string("type"));
#line 383 "compiler/main.gem"
    GemVal _t675 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t675.fn(_t675.env, NULL, 0);
#line 384 "compiler/main.gem"
    GemVal _t676[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t676, 3);
    }
    GemVal _t677 = gem_v_left;
    gem_pop_frame();
    return _t677;
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
#line 391 "compiler/main.gem"
    GemVal _t679 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t679.fn(_t679.env, NULL, 0);
#line 392 "compiler/main.gem"
    while (1) {
        GemVal _t680 = (*gem_v_peek);
        GemVal _t681 = _t680.fn(_t680.env, NULL, 0);
        GemVal _t684;
        if (gem_truthy(gem_eq(gem_table_get(_t681, gem_string("type")), gem_string("+")))) {
                _t684 = gem_eq(gem_table_get(_t681, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t682 = (*gem_v_peek);
                GemVal _t683 = _t682.fn(_t682.env, NULL, 0);
                _t684 = gem_eq(gem_table_get(_t683, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t684)) break;
#line 393 "compiler/main.gem"
    GemVal _t685 = (*gem_v_advance);
    GemVal _t686 = _t685.fn(_t685.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t686, gem_string("type"));
#line 394 "compiler/main.gem"
    GemVal _t687 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t687.fn(_t687.env, NULL, 0);
#line 395 "compiler/main.gem"
    GemVal _t688[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t688, 3);
    }
    GemVal _t689 = gem_v_left;
    gem_pop_frame();
    return _t689;
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
#line 402 "compiler/main.gem"
    GemVal _t691 = (*gem_v_parse_add);
    GemVal gem_v_left = _t691.fn(_t691.env, NULL, 0);
#line 403 "compiler/main.gem"
    while (1) {
        GemVal _t692 = (*gem_v_peek);
        GemVal _t693 = _t692.fn(_t692.env, NULL, 0);
        GemVal _t696;
        if (gem_truthy(gem_eq(gem_table_get(_t693, gem_string("type")), gem_string("==")))) {
                _t696 = gem_eq(gem_table_get(_t693, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t694 = (*gem_v_peek);
                GemVal _t695 = _t694.fn(_t694.env, NULL, 0);
                _t696 = gem_eq(gem_table_get(_t695, gem_string("type")), gem_string("!="));
        }
        GemVal _t699;
        if (gem_truthy(_t696)) {
                _t699 = _t696;
        } else {
                GemVal _t697 = (*gem_v_peek);
                GemVal _t698 = _t697.fn(_t697.env, NULL, 0);
                _t699 = gem_eq(gem_table_get(_t698, gem_string("type")), gem_string("<"));
        }
        GemVal _t702;
        if (gem_truthy(_t699)) {
                _t702 = _t699;
        } else {
                GemVal _t700 = (*gem_v_peek);
                GemVal _t701 = _t700.fn(_t700.env, NULL, 0);
                _t702 = gem_eq(gem_table_get(_t701, gem_string("type")), gem_string(">"));
        }
        GemVal _t705;
        if (gem_truthy(_t702)) {
                _t705 = _t702;
        } else {
                GemVal _t703 = (*gem_v_peek);
                GemVal _t704 = _t703.fn(_t703.env, NULL, 0);
                _t705 = gem_eq(gem_table_get(_t704, gem_string("type")), gem_string("<="));
        }
        GemVal _t708;
        if (gem_truthy(_t705)) {
                _t708 = _t705;
        } else {
                GemVal _t706 = (*gem_v_peek);
                GemVal _t707 = _t706.fn(_t706.env, NULL, 0);
                _t708 = gem_eq(gem_table_get(_t707, gem_string("type")), gem_string(">="));
        }
        GemVal _t711;
        if (gem_truthy(_t708)) {
                _t711 = _t708;
        } else {
                GemVal _t709 = (*gem_v_peek);
                GemVal _t710 = _t709.fn(_t709.env, NULL, 0);
                _t711 = gem_eq(gem_table_get(_t710, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t711)) break;
#line 404 "compiler/main.gem"
    GemVal _t712 = (*gem_v_advance);
    GemVal _t713 = _t712.fn(_t712.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t713, gem_string("type"));
#line 405 "compiler/main.gem"
    GemVal _t714 = (*gem_v_parse_add);
        GemVal gem_v_right = _t714.fn(_t714.env, NULL, 0);
#line 406 "compiler/main.gem"
    GemVal _t715[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t715, 3);
    }
    GemVal _t716 = gem_v_left;
    gem_pop_frame();
    return _t716;
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
#line 413 "compiler/main.gem"
    GemVal _t718 = (*gem_v_peek);
    GemVal _t719 = _t718.fn(_t718.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t719, gem_string("type")), gem_string("not")))) {
#line 414 "compiler/main.gem"
    GemVal _t720 = (*gem_v_advance);
        (void)(_t720.fn(_t720.env, NULL, 0));
#line 415 "compiler/main.gem"
    GemVal _t721 = (*gem_v_parse_not);
    GemVal _t722[] = {gem_string("not"), _t721.fn(_t721.env, NULL, 0)};
        GemVal _t723 = gem_fn_make_unop(NULL, _t722, 2);
        gem_pop_frame();
        return _t723;
    }
#line 417 "compiler/main.gem"
    GemVal _t724 = (*gem_v_parse_compare);
    GemVal _t725 = _t724.fn(_t724.env, NULL, 0);
    gem_pop_frame();
    return _t725;
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
#line 422 "compiler/main.gem"
    GemVal _t727 = (*gem_v_parse_not);
    GemVal gem_v_left = _t727.fn(_t727.env, NULL, 0);
#line 423 "compiler/main.gem"
    while (1) {
        GemVal _t728 = (*gem_v_peek);
        GemVal _t729 = _t728.fn(_t728.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t729, gem_string("type")), gem_string("and")))) break;
#line 424 "compiler/main.gem"
    GemVal _t730 = (*gem_v_advance);
        (void)(_t730.fn(_t730.env, NULL, 0));
#line 425 "compiler/main.gem"
    GemVal _t731 = (*gem_v_parse_not);
        GemVal gem_v_right = _t731.fn(_t731.env, NULL, 0);
#line 426 "compiler/main.gem"
    GemVal _t732[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t732, 3);
    }
    GemVal _t733 = gem_v_left;
    gem_pop_frame();
    return _t733;
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
#line 433 "compiler/main.gem"
    GemVal _t735 = (*gem_v_parse_and);
    GemVal gem_v_left = _t735.fn(_t735.env, NULL, 0);
#line 434 "compiler/main.gem"
    while (1) {
        GemVal _t736 = (*gem_v_peek);
        GemVal _t737 = _t736.fn(_t736.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t737, gem_string("type")), gem_string("or")))) break;
#line 435 "compiler/main.gem"
    GemVal _t738 = (*gem_v_advance);
        (void)(_t738.fn(_t738.env, NULL, 0));
#line 436 "compiler/main.gem"
    GemVal _t739 = (*gem_v_parse_and);
        GemVal gem_v_right = _t739.fn(_t739.env, NULL, 0);
#line 437 "compiler/main.gem"
    GemVal _t740[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t740, 3);
    }
    GemVal _t741 = gem_v_left;
    gem_pop_frame();
    return _t741;
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
#line 444 "compiler/main.gem"
    GemVal _t743 = (*gem_v_peek);
    GemVal _t744 = _t743.fn(_t743.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t744, gem_string("line"));
#line 445 "compiler/main.gem"
    GemVal _t745 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t745.fn(_t745.env, NULL, 0);
#line 448 "compiler/main.gem"
    GemVal _t746 = (*gem_v_peek);
    GemVal _t747 = _t746.fn(_t746.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t747, gem_string("type")), gem_string("=")))) {
#line 449 "compiler/main.gem"
    GemVal _t748 = (*gem_v_advance);
        (void)(_t748.fn(_t748.env, NULL, 0));
#line 450 "compiler/main.gem"
    GemVal _t749 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t749.fn(_t749.env, NULL, 0);
#line 451 "compiler/main.gem"
    GemVal _t750 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t750, gem_string("tag")), gem_string("var")))) {
#line 452 "compiler/main.gem"
    GemVal _t751 = gem_v_lhs;
    GemVal _t752[] = {gem_table_get(_t751, gem_string("name")), gem_v_value, gem_v_start_line};
            GemVal _t753 = gem_fn_make_assign(NULL, _t752, 3);
            gem_pop_frame();
            return _t753;
        }
#line 454 "compiler/main.gem"
    GemVal _t754 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t754, gem_string("tag")), gem_string("dot")))) {
#line 455 "compiler/main.gem"
    GemVal _t755 = gem_v_lhs;
    GemVal _t756 = gem_v_lhs;
    GemVal _t757[] = {gem_table_get(_t755, gem_string("object")), gem_table_get(_t756, gem_string("field")), gem_v_value, gem_v_start_line};
            GemVal _t758 = gem_fn_make_dot_assign(NULL, _t757, 4);
            gem_pop_frame();
            return _t758;
        }
#line 457 "compiler/main.gem"
    GemVal _t759 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t759, gem_string("tag")), gem_string("index")))) {
#line 458 "compiler/main.gem"
    GemVal _t760 = gem_v_lhs;
    GemVal _t761 = gem_v_lhs;
    GemVal _t762[] = {gem_table_get(_t760, gem_string("object")), gem_table_get(_t761, gem_string("key")), gem_v_value, gem_v_start_line};
            GemVal _t763 = gem_fn_make_index_assign(NULL, _t762, 4);
            gem_pop_frame();
            return _t763;
        }
#line 460 "compiler/main.gem"
    GemVal _t764[] = {gem_v_start_line};
    GemVal _t765[] = {gem_add(gem_string("invalid assignment target (expected variable, field, or index) at line "), gem_to_string_fn(NULL, _t764, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 460, _t765, 1));
    }
#line 464 "compiler/main.gem"
    GemVal _t766 = (*gem_v_peek);
    GemVal _t767 = _t766.fn(_t766.env, NULL, 0);
    GemVal _t770;
    if (gem_truthy(gem_eq(gem_table_get(_t767, gem_string("type")), gem_string("+=")))) {
        _t770 = gem_eq(gem_table_get(_t767, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t768 = (*gem_v_peek);
        GemVal _t769 = _t768.fn(_t768.env, NULL, 0);
        _t770 = gem_eq(gem_table_get(_t769, gem_string("type")), gem_string("-="));
    }
    GemVal _t773;
    if (gem_truthy(_t770)) {
        _t773 = _t770;
    } else {
        GemVal _t771 = (*gem_v_peek);
        GemVal _t772 = _t771.fn(_t771.env, NULL, 0);
        _t773 = gem_eq(gem_table_get(_t772, gem_string("type")), gem_string("*="));
    }
    GemVal _t776;
    if (gem_truthy(_t773)) {
        _t776 = _t773;
    } else {
        GemVal _t774 = (*gem_v_peek);
        GemVal _t775 = _t774.fn(_t774.env, NULL, 0);
        _t776 = gem_eq(gem_table_get(_t775, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t776)) {
#line 465 "compiler/main.gem"
    GemVal _t777 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t777.fn(_t777.env, NULL, 0);
#line 466 "compiler/main.gem"
    GemVal _t778 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t778, gem_string("value")), gem_int(0));
#line 467 "compiler/main.gem"
    GemVal _t779 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t779.fn(_t779.env, NULL, 0);
#line 468 "compiler/main.gem"
    GemVal _t780 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t780, gem_string("tag")), gem_string("var")))) {
#line 469 "compiler/main.gem"
    GemVal _t781 = gem_v_op_tok;
    GemVal _t782[] = {gem_table_get(_t781, gem_string("line"))};
    GemVal _t783[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t782, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 469, _t783, 1));
        }
#line 471 "compiler/main.gem"
    GemVal _t784 = gem_v_lhs;
    GemVal _t785 = gem_v_lhs;
    GemVal _t786[] = {gem_table_get(_t785, gem_string("name"))};
    GemVal _t787[] = {gem_v_base_op, gem_fn_make_var(NULL, _t786, 1), gem_v_value};
    GemVal _t788[] = {gem_table_get(_t784, gem_string("name")), gem_fn_make_binop(NULL, _t787, 3), gem_v_start_line};
        GemVal _t789 = gem_fn_make_assign(NULL, _t788, 3);
        gem_pop_frame();
        return _t789;
    }
    GemVal _t790 = gem_v_lhs;
    gem_pop_frame();
    return _t790;
}

struct _closure__anon_21 {
};
static GemVal _anon_21(void *_env, GemVal *args, int argc) {
    GemVal gem_v_conditions = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_21", "compiler/main.gem", 0);
#line 484 "compiler/main.gem"
    GemVal _t792[] = {gem_v_conditions};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t792, 1), gem_int(0)))) {
#line 485 "compiler/main.gem"
    GemVal _t793[] = {gem_bool(1)};
        GemVal _t794 = gem_fn_make_bool(NULL, _t793, 1);
        gem_pop_frame();
        return _t794;
    }
#line 487 "compiler/main.gem"
    GemVal gem_v_result = gem_table_get(gem_v_conditions, gem_int(0));
#line 488 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(1);
#line 488 "compiler/main.gem"
    GemVal _t795[] = {gem_v_conditions};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t795, 1);
#line 488 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 488 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_1;
#line 488 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 489 "compiler/main.gem"
    GemVal _t796[] = {gem_string("and"), gem_v_result, gem_table_get(gem_v_conditions, gem_v_i)};
        gem_v_result = gem_fn_make_binop(NULL, _t796, 3);
    }

    GemVal _t797 = gem_v_result;
    gem_pop_frame();
    return _t797;
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
#line 483 "compiler/main.gem"
    GemVal gem_v_and_chain = gem_make_fn(_anon_21, NULL);
#line 493 "compiler/main.gem"
    GemVal _t798 = (*gem_v_peek);
    GemVal gem_v_t = _t798.fn(_t798.env, NULL, 0);
#line 496 "compiler/main.gem"
    GemVal _t799 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t799, gem_string("type")), gem_string("{")))) {
#line 497 "compiler/main.gem"
    GemVal _t800 = (*gem_v_advance);
        (void)(_t800.fn(_t800.env, NULL, 0));
#line 498 "compiler/main.gem"
    GemVal _t801 = (*gem_v_skip_nl);
        (void)(_t801.fn(_t801.env, NULL, 0));
#line 499 "compiler/main.gem"
    GemVal _t802 = gem_table_new();
    GemVal _t803[] = {gem_string("type")};
    GemVal _t804 = gem_table_new();
    gem_table_set(_t804, gem_int(0), gem_v_target_expr);
    GemVal _t805[] = {gem_fn_make_var(NULL, _t803, 1), _t804, gem_int(0)};
    GemVal _t806[] = {gem_string("table")};
    GemVal _t807[] = {gem_string("=="), gem_fn_make_call(NULL, _t805, 3), gem_fn_make_string(NULL, _t806, 1)};
    gem_table_set(_t802, gem_int(0), gem_fn_make_binop(NULL, _t807, 3));
        GemVal gem_v_conditions = _t802;
#line 500 "compiler/main.gem"
    GemVal _t808 = gem_table_new();
        GemVal gem_v_bindings = _t808;
#line 501 "compiler/main.gem"
        while (1) {
            GemVal _t809 = (*gem_v_peek);
            GemVal _t810 = _t809.fn(_t809.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t810, gem_string("type")), gem_string("}")))) break;
#line 502 "compiler/main.gem"
    GemVal _t811 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t811.fn(_t811.env, NULL, 0);
#line 503 "compiler/main.gem"
    GemVal _t812 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t812, gem_string("type")), gem_string("NAME")))) {
#line 504 "compiler/main.gem"
    GemVal _t813 = (*gem_v_advance);
                (void)(_t813.fn(_t813.env, NULL, 0));
            } else {
#line 505 "compiler/main.gem"
    GemVal _t814[] = {gem_int(1)};
    GemVal _t815 = (*gem_v_peek_at);
    GemVal _t816 = _t815.fn(_t815.env, _t814, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t816, gem_string("type")), gem_string(":")))) {
#line 507 "compiler/main.gem"
    GemVal _t817 = (*gem_v_advance);
                    (void)(_t817.fn(_t817.env, NULL, 0));
                } else {
#line 509 "compiler/main.gem"
    GemVal _t818 = gem_v_key_tok;
    GemVal _t819[] = {gem_table_get(_t818, gem_string("line"))};
    GemVal _t820[] = {gem_add(gem_string("expected key name in table pattern at line "), gem_to_string_fn(NULL, _t819, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 509, _t820, 1));
                }
            }
#line 511 "compiler/main.gem"
    GemVal _t821 = gem_v_key_tok;
            GemVal gem_v_key_name = gem_table_get(_t821, gem_string("value"));
#line 512 "compiler/main.gem"
    GemVal _t822[] = {gem_v_key_name};
            GemVal gem_v_key_str = gem_fn_make_string(NULL, _t822, 1);
#line 513 "compiler/main.gem"
    GemVal _t823[] = {gem_string("has_key")};
    GemVal _t824 = gem_table_new();
    gem_table_set(_t824, gem_int(0), gem_v_target_expr);
    gem_table_set(_t824, gem_int(1), gem_v_key_str);
    GemVal _t825[] = {gem_fn_make_var(NULL, _t823, 1), _t824, gem_int(0)};
    GemVal _t826[] = {gem_v_conditions, gem_fn_make_call(NULL, _t825, 3)};
            (void)(gem_push_fn(NULL, _t826, 2));
#line 514 "compiler/main.gem"
    GemVal _t827[] = {gem_v_target_expr, gem_v_key_str};
            GemVal gem_v_sub_target = gem_fn_make_index(NULL, _t827, 2);
#line 515 "compiler/main.gem"
    GemVal _t828 = (*gem_v_peek);
    GemVal _t829 = _t828.fn(_t828.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t829, gem_string("type")), gem_string(":")))) {
#line 516 "compiler/main.gem"
    GemVal _t830 = (*gem_v_advance);
                (void)(_t830.fn(_t830.env, NULL, 0));
#line 517 "compiler/main.gem"
    GemVal _t831 = (*gem_v_skip_nl);
                (void)(_t831.fn(_t831.env, NULL, 0));
#line 518 "compiler/main.gem"
    GemVal _t832[] = {gem_v_sub_target};
    GemVal _t833 = (*gem_v_parse_pattern);
                GemVal gem_v_sub = _t833.fn(_t833.env, _t832, 1);
#line 519 "compiler/main.gem"
    GemVal _t834 = gem_v_sub;
    GemVal _t835 = gem_table_get(_t834, gem_string("condition"));
    GemVal _t838;
    if (gem_truthy(gem_neq(gem_table_get(_t835, gem_string("tag")), gem_string("bool")))) {
        _t838 = gem_neq(gem_table_get(_t835, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t836 = gem_v_sub;
        GemVal _t837 = gem_table_get(_t836, gem_string("condition"));
        _t838 = gem_neq(gem_table_get(_t837, gem_string("value")), gem_bool(1));
    }
                if (gem_truthy(_t838)) {
#line 520 "compiler/main.gem"
    GemVal _t839 = gem_v_sub;
    GemVal _t840[] = {gem_v_conditions, gem_table_get(_t839, gem_string("condition"))};
                    (void)(gem_push_fn(NULL, _t840, 2));
                }
#line 522 "compiler/main.gem"
    GemVal _t841 = gem_v_sub;
                GemVal gem_v__for_items_2 = gem_table_get(_t841, gem_string("bindings"));
#line 522 "compiler/main.gem"
                GemVal gem_v__for_i_2 = gem_int(0);
#line 522 "compiler/main.gem"
                while (1) {
                    GemVal _t842[] = {gem_v__for_items_2};
                    if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t842, 1)))) break;
#line 522 "compiler/main.gem"
                    GemVal gem_v_b = gem_table_get(gem_v__for_items_2, gem_v__for_i_2);
#line 522 "compiler/main.gem"
                    gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 523 "compiler/main.gem"
    GemVal _t843[] = {gem_v_bindings, gem_v_b};
                    (void)(gem_push_fn(NULL, _t843, 2));
                }

            } else {
#line 527 "compiler/main.gem"
    GemVal _t844[] = {gem_v_key_name, gem_v_sub_target, gem_int(0)};
    GemVal _t845[] = {gem_v_bindings, gem_fn_make_let(NULL, _t844, 3)};
                (void)(gem_push_fn(NULL, _t845, 2));
            }
#line 529 "compiler/main.gem"
    GemVal _t846 = (*gem_v_skip_nl);
            (void)(_t846.fn(_t846.env, NULL, 0));
#line 530 "compiler/main.gem"
    GemVal _t847 = (*gem_v_peek);
    GemVal _t848 = _t847.fn(_t847.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t848, gem_string("type")), gem_string(",")))) {
#line 531 "compiler/main.gem"
    GemVal _t849 = (*gem_v_advance);
                (void)(_t849.fn(_t849.env, NULL, 0));
#line 532 "compiler/main.gem"
    GemVal _t850 = (*gem_v_skip_nl);
                (void)(_t850.fn(_t850.env, NULL, 0));
            }
        }
#line 535 "compiler/main.gem"
    GemVal _t851[] = {gem_string("}")};
    GemVal _t852 = (*gem_v_expect);
        (void)(_t852.fn(_t852.env, _t851, 1));
#line 536 "compiler/main.gem"
    GemVal _t853 = gem_table_new();
    GemVal _t854[] = {gem_v_conditions};
    GemVal _t855 = gem_v_and_chain;
    gem_table_set(_t853, gem_string("condition"), _t855.fn(_t855.env, _t854, 1));
    gem_table_set(_t853, gem_string("bindings"), gem_v_bindings);
        GemVal _t856 = _t853;
        gem_pop_frame();
        return _t856;
    }
#line 540 "compiler/main.gem"
    GemVal _t857 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t857, gem_string("type")), gem_string("[")))) {
#line 541 "compiler/main.gem"
    GemVal _t858 = (*gem_v_advance);
        (void)(_t858.fn(_t858.env, NULL, 0));
#line 542 "compiler/main.gem"
    GemVal _t859 = (*gem_v_skip_nl);
        (void)(_t859.fn(_t859.env, NULL, 0));
#line 543 "compiler/main.gem"
    GemVal _t860 = gem_table_new();
        GemVal gem_v_sub_patterns = _t860;
#line 544 "compiler/main.gem"
        GemVal gem_v_idx = gem_int(0);
#line 545 "compiler/main.gem"
        while (1) {
            GemVal _t861 = (*gem_v_peek);
            GemVal _t862 = _t861.fn(_t861.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t862, gem_string("type")), gem_string("]")))) break;
#line 546 "compiler/main.gem"
    GemVal _t863[] = {gem_v_idx};
    GemVal _t864[] = {gem_v_target_expr, gem_fn_make_int(NULL, _t863, 1)};
    GemVal _t865[] = {gem_fn_make_index(NULL, _t864, 2)};
    GemVal _t866 = (*gem_v_parse_pattern);
            GemVal gem_v_sub = _t866.fn(_t866.env, _t865, 1);
#line 547 "compiler/main.gem"
    GemVal _t867[] = {gem_v_sub_patterns, gem_v_sub};
            (void)(gem_push_fn(NULL, _t867, 2));
#line 548 "compiler/main.gem"
            gem_v_idx = gem_add(gem_v_idx, gem_int(1));
#line 549 "compiler/main.gem"
    GemVal _t868 = (*gem_v_skip_nl);
            (void)(_t868.fn(_t868.env, NULL, 0));
#line 550 "compiler/main.gem"
    GemVal _t869 = (*gem_v_peek);
    GemVal _t870 = _t869.fn(_t869.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t870, gem_string("type")), gem_string(",")))) {
#line 551 "compiler/main.gem"
    GemVal _t871 = (*gem_v_advance);
                (void)(_t871.fn(_t871.env, NULL, 0));
#line 552 "compiler/main.gem"
    GemVal _t872 = (*gem_v_skip_nl);
                (void)(_t872.fn(_t872.env, NULL, 0));
            }
        }
#line 555 "compiler/main.gem"
    GemVal _t873[] = {gem_string("]")};
    GemVal _t874 = (*gem_v_expect);
        (void)(_t874.fn(_t874.env, _t873, 1));
#line 556 "compiler/main.gem"
    GemVal _t875 = gem_table_new();
    GemVal _t876[] = {gem_string("type")};
    GemVal _t877 = gem_table_new();
    gem_table_set(_t877, gem_int(0), gem_v_target_expr);
    GemVal _t878[] = {gem_fn_make_var(NULL, _t876, 1), _t877, gem_int(0)};
    GemVal _t879[] = {gem_string("table")};
    GemVal _t880[] = {gem_string("=="), gem_fn_make_call(NULL, _t878, 3), gem_fn_make_string(NULL, _t879, 1)};
    gem_table_set(_t875, gem_int(0), gem_fn_make_binop(NULL, _t880, 3));
        GemVal gem_v_conditions = _t875;
#line 557 "compiler/main.gem"
    GemVal _t881[] = {gem_string("len")};
    GemVal _t882 = gem_table_new();
    gem_table_set(_t882, gem_int(0), gem_v_target_expr);
    GemVal _t883[] = {gem_fn_make_var(NULL, _t881, 1), _t882, gem_int(0)};
    GemVal _t884[] = {gem_v_idx};
    GemVal _t885[] = {gem_string("=="), gem_fn_make_call(NULL, _t883, 3), gem_fn_make_int(NULL, _t884, 1)};
    GemVal _t886[] = {gem_v_conditions, gem_fn_make_binop(NULL, _t885, 3)};
        (void)(gem_push_fn(NULL, _t886, 2));
#line 558 "compiler/main.gem"
    GemVal _t887 = gem_table_new();
        GemVal gem_v_bindings = _t887;
#line 559 "compiler/main.gem"
        GemVal gem_v__for_i_4 = gem_int(0);
#line 559 "compiler/main.gem"
    GemVal _t888[] = {gem_v_sub_patterns};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t888, 1);
#line 559 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 559 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_4;
#line 559 "compiler/main.gem"
            gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 560 "compiler/main.gem"
            GemVal gem_v_sub = gem_table_get(gem_v_sub_patterns, gem_v_i);
#line 561 "compiler/main.gem"
    GemVal _t889 = gem_v_sub;
    GemVal _t890 = gem_table_get(_t889, gem_string("condition"));
    GemVal _t893;
    if (gem_truthy(gem_neq(gem_table_get(_t890, gem_string("tag")), gem_string("bool")))) {
        _t893 = gem_neq(gem_table_get(_t890, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t891 = gem_v_sub;
        GemVal _t892 = gem_table_get(_t891, gem_string("condition"));
        _t893 = gem_neq(gem_table_get(_t892, gem_string("value")), gem_bool(1));
    }
            if (gem_truthy(_t893)) {
#line 562 "compiler/main.gem"
    GemVal _t894 = gem_v_sub;
    GemVal _t895[] = {gem_v_conditions, gem_table_get(_t894, gem_string("condition"))};
                (void)(gem_push_fn(NULL, _t895, 2));
            }
#line 564 "compiler/main.gem"
    GemVal _t896 = gem_v_sub;
            GemVal gem_v__for_items_3 = gem_table_get(_t896, gem_string("bindings"));
#line 564 "compiler/main.gem"
            GemVal gem_v__for_i_3 = gem_int(0);
#line 564 "compiler/main.gem"
            while (1) {
                GemVal _t897[] = {gem_v__for_items_3};
                if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_len_fn(NULL, _t897, 1)))) break;
#line 564 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_3, gem_v__for_i_3);
#line 564 "compiler/main.gem"
                gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 565 "compiler/main.gem"
    GemVal _t898[] = {gem_v_bindings, gem_v_b};
                (void)(gem_push_fn(NULL, _t898, 2));
            }

        }

#line 568 "compiler/main.gem"
    GemVal _t899 = gem_table_new();
    GemVal _t900[] = {gem_v_conditions};
    GemVal _t901 = gem_v_and_chain;
    gem_table_set(_t899, gem_string("condition"), _t901.fn(_t901.env, _t900, 1));
    gem_table_set(_t899, gem_string("bindings"), gem_v_bindings);
        GemVal _t902 = _t899;
        gem_pop_frame();
        return _t902;
    }
#line 572 "compiler/main.gem"
    GemVal _t903 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t903, gem_string("type")), gem_string("NUMBER")))) {
#line 573 "compiler/main.gem"
    GemVal _t904 = (*gem_v_advance);
        (void)(_t904.fn(_t904.env, NULL, 0));
#line 574 "compiler/main.gem"
        GemVal gem_v_lit = GEM_NIL;
#line 575 "compiler/main.gem"
    GemVal _t905 = gem_v_t;
    GemVal _t906[] = {gem_table_get(_t905, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t906, 1))) {
#line 576 "compiler/main.gem"
    GemVal _t907 = gem_v_t;
    GemVal _t908[] = {gem_table_get(_t907, gem_string("value"))};
    GemVal _t909[] = {gem_fn_atof(NULL, _t908, 1)};
            gem_v_lit = gem_fn_make_float(NULL, _t909, 1);
        } else {
#line 578 "compiler/main.gem"
    GemVal _t910 = gem_v_t;
    GemVal _t911[] = {gem_table_get(_t910, gem_string("value"))};
    GemVal _t912[] = {gem_fn_str_to_int(NULL, _t911, 1)};
            gem_v_lit = gem_fn_make_int(NULL, _t912, 1);
        }
#line 580 "compiler/main.gem"
    GemVal _t913 = gem_table_new();
    GemVal _t914[] = {gem_string("=="), gem_v_target_expr, gem_v_lit};
    gem_table_set(_t913, gem_string("condition"), gem_fn_make_binop(NULL, _t914, 3));
    GemVal _t915 = gem_table_new();
    gem_table_set(_t913, gem_string("bindings"), _t915);
        GemVal _t916 = _t913;
        gem_pop_frame();
        return _t916;
    }
#line 582 "compiler/main.gem"
    GemVal _t917 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t917, gem_string("type")), gem_string("STRING")))) {
#line 583 "compiler/main.gem"
    GemVal _t918 = (*gem_v_advance);
        (void)(_t918.fn(_t918.env, NULL, 0));
#line 584 "compiler/main.gem"
    GemVal _t919 = gem_table_new();
    GemVal _t920 = gem_v_t;
    GemVal _t921[] = {gem_table_get(_t920, gem_string("value"))};
    GemVal _t922[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_string(NULL, _t921, 1)};
    gem_table_set(_t919, gem_string("condition"), gem_fn_make_binop(NULL, _t922, 3));
    GemVal _t923 = gem_table_new();
    gem_table_set(_t919, gem_string("bindings"), _t923);
        GemVal _t924 = _t919;
        gem_pop_frame();
        return _t924;
    }
#line 586 "compiler/main.gem"
    GemVal _t925 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t925, gem_string("type")), gem_string("true")))) {
#line 587 "compiler/main.gem"
    GemVal _t926 = (*gem_v_advance);
        (void)(_t926.fn(_t926.env, NULL, 0));
#line 588 "compiler/main.gem"
    GemVal _t927 = gem_table_new();
    GemVal _t928[] = {gem_bool(1)};
    GemVal _t929[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_bool(NULL, _t928, 1)};
    gem_table_set(_t927, gem_string("condition"), gem_fn_make_binop(NULL, _t929, 3));
    GemVal _t930 = gem_table_new();
    gem_table_set(_t927, gem_string("bindings"), _t930);
        GemVal _t931 = _t927;
        gem_pop_frame();
        return _t931;
    }
#line 590 "compiler/main.gem"
    GemVal _t932 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t932, gem_string("type")), gem_string("false")))) {
#line 591 "compiler/main.gem"
    GemVal _t933 = (*gem_v_advance);
        (void)(_t933.fn(_t933.env, NULL, 0));
#line 592 "compiler/main.gem"
    GemVal _t934 = gem_table_new();
    GemVal _t935[] = {gem_bool(0)};
    GemVal _t936[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_bool(NULL, _t935, 1)};
    gem_table_set(_t934, gem_string("condition"), gem_fn_make_binop(NULL, _t936, 3));
    GemVal _t937 = gem_table_new();
    gem_table_set(_t934, gem_string("bindings"), _t937);
        GemVal _t938 = _t934;
        gem_pop_frame();
        return _t938;
    }
#line 594 "compiler/main.gem"
    GemVal _t939 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t939, gem_string("type")), gem_string("nil")))) {
#line 595 "compiler/main.gem"
    GemVal _t940 = (*gem_v_advance);
        (void)(_t940.fn(_t940.env, NULL, 0));
#line 596 "compiler/main.gem"
    GemVal _t941 = gem_table_new();
    GemVal _t942[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_nil_node(NULL, NULL, 0)};
    gem_table_set(_t941, gem_string("condition"), gem_fn_make_binop(NULL, _t942, 3));
    GemVal _t943 = gem_table_new();
    gem_table_set(_t941, gem_string("bindings"), _t943);
        GemVal _t944 = _t941;
        gem_pop_frame();
        return _t944;
    }
#line 600 "compiler/main.gem"
    GemVal _t945 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t945, gem_string("type")), gem_string("NAME")))) {
#line 601 "compiler/main.gem"
    GemVal _t946 = (*gem_v_advance);
        (void)(_t946.fn(_t946.env, NULL, 0));
#line 602 "compiler/main.gem"
    GemVal _t947 = gem_table_new();
    GemVal _t948[] = {gem_bool(1)};
    gem_table_set(_t947, gem_string("condition"), gem_fn_make_bool(NULL, _t948, 1));
    GemVal _t949 = gem_table_new();
    GemVal _t950 = gem_v_t;
    GemVal _t951[] = {gem_table_get(_t950, gem_string("value")), gem_v_target_expr, gem_int(0)};
    gem_table_set(_t949, gem_int(0), gem_fn_make_let(NULL, _t951, 3));
    gem_table_set(_t947, gem_string("bindings"), _t949);
        GemVal _t952 = _t947;
        gem_pop_frame();
        return _t952;
    }
#line 605 "compiler/main.gem"
    GemVal _t953 = gem_v_t;
    GemVal _t954[] = {gem_table_get(_t953, gem_string("type"))};
    GemVal _t955 = gem_v_t;
    GemVal _t956[] = {gem_table_get(_t955, gem_string("line"))};
    GemVal _t957[] = {gem_add(gem_add(gem_add(gem_string("expected pattern but got '"), gem_to_string_fn(NULL, _t954, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t956, 1))};
    GemVal _t958 = gem_error_at_fn("compiler/main.gem", 605, _t957, 1);
    gem_pop_frame();
    return _t958;
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
#line 611 "compiler/main.gem"
    GemVal _t960 = (*gem_v_peek);
    GemVal gem_v_t = _t960.fn(_t960.env, NULL, 0);
#line 614 "compiler/main.gem"
    GemVal _t961 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t961, gem_string("type")), gem_string("let")))) {
#line 615 "compiler/main.gem"
    GemVal _t962 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t962, gem_string("line"));
#line 616 "compiler/main.gem"
    GemVal _t963 = (*gem_v_advance);
        (void)(_t963.fn(_t963.env, NULL, 0));
#line 619 "compiler/main.gem"
    GemVal _t964 = (*gem_v_peek);
    GemVal _t965 = _t964.fn(_t964.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t965, gem_string("type")), gem_string("{")))) {
#line 620 "compiler/main.gem"
    GemVal _t966 = (*gem_v_advance);
            (void)(_t966.fn(_t966.env, NULL, 0));
#line 621 "compiler/main.gem"
    GemVal _t967 = gem_table_new();
            GemVal gem_v_names = _t967;
#line 622 "compiler/main.gem"
    GemVal _t968 = (*gem_v_skip_nl);
            (void)(_t968.fn(_t968.env, NULL, 0));
#line 623 "compiler/main.gem"
            while (1) {
                GemVal _t969 = (*gem_v_peek);
                GemVal _t970 = _t969.fn(_t969.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t970, gem_string("type")), gem_string("}")))) break;
#line 624 "compiler/main.gem"
    GemVal _t971 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t971.fn(_t971.env, NULL, 0);
#line 625 "compiler/main.gem"
    GemVal _t972 = gem_v_field_tok;
                if (gem_truthy(gem_eq(gem_table_get(_t972, gem_string("type")), gem_string("NAME")))) {
#line 626 "compiler/main.gem"
    GemVal _t973 = (*gem_v_advance);
                    (void)(_t973.fn(_t973.env, NULL, 0));
                } else {
#line 629 "compiler/main.gem"
    GemVal _t974 = (*gem_v_advance);
                    (void)(_t974.fn(_t974.env, NULL, 0));
                }
#line 631 "compiler/main.gem"
    GemVal _t975 = gem_v_field_tok;
    GemVal _t976[] = {gem_v_names, gem_table_get(_t975, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t976, 2));
#line 632 "compiler/main.gem"
    GemVal _t977 = (*gem_v_skip_nl);
                (void)(_t977.fn(_t977.env, NULL, 0));
#line 633 "compiler/main.gem"
    GemVal _t978 = (*gem_v_peek);
    GemVal _t979 = _t978.fn(_t978.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t979, gem_string("type")), gem_string(",")))) {
#line 634 "compiler/main.gem"
    GemVal _t980 = (*gem_v_advance);
                    (void)(_t980.fn(_t980.env, NULL, 0));
#line 635 "compiler/main.gem"
    GemVal _t981 = (*gem_v_skip_nl);
                    (void)(_t981.fn(_t981.env, NULL, 0));
                }
            }
#line 638 "compiler/main.gem"
    GemVal _t982[] = {gem_string("}")};
    GemVal _t983 = (*gem_v_expect);
            (void)(_t983.fn(_t983.env, _t982, 1));
#line 639 "compiler/main.gem"
    GemVal _t984[] = {gem_string("=")};
    GemVal _t985 = (*gem_v_expect);
            (void)(_t985.fn(_t985.env, _t984, 1));
#line 640 "compiler/main.gem"
    GemVal _t986 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t986.fn(_t986.env, NULL, 0);
#line 641 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 642 "compiler/main.gem"
    GemVal _t987[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t987, 1));
#line 643 "compiler/main.gem"
    GemVal _t988 = gem_table_new();
    GemVal _t989[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t988, gem_int(0), gem_fn_make_let(NULL, _t989, 3));
            GemVal gem_v_stmts = _t988;
#line 644 "compiler/main.gem"
            GemVal gem_v__for_i_5 = gem_int(0);
#line 644 "compiler/main.gem"
    GemVal _t990[] = {gem_v_names};
            GemVal gem_v__for_limit_5 = gem_len_fn(NULL, _t990, 1);
#line 644 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_limit_5))) break;
#line 644 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_5;
#line 644 "compiler/main.gem"
                gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 645 "compiler/main.gem"
    GemVal _t991[] = {gem_v_tmp};
    GemVal _t992[] = {gem_fn_make_var(NULL, _t991, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t993[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_dot(NULL, _t992, 2), gem_v_line};
    GemVal _t994[] = {gem_v_stmts, gem_fn_make_let(NULL, _t993, 3)};
                (void)(gem_push_fn(NULL, _t994, 2));
            }

#line 647 "compiler/main.gem"
    GemVal _t995 = gem_table_new();
    gem_table_set(_t995, gem_string("tag"), gem_string("block"));
    gem_table_set(_t995, gem_string("stmts"), gem_v_stmts);
            GemVal _t996 = _t995;
            gem_pop_frame();
            return _t996;
        }
#line 651 "compiler/main.gem"
    GemVal _t997 = (*gem_v_peek);
    GemVal _t998 = _t997.fn(_t997.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t998, gem_string("type")), gem_string("[")))) {
#line 652 "compiler/main.gem"
    GemVal _t999 = (*gem_v_advance);
            (void)(_t999.fn(_t999.env, NULL, 0));
#line 653 "compiler/main.gem"
    GemVal _t1000 = gem_table_new();
            GemVal gem_v_names = _t1000;
#line 654 "compiler/main.gem"
    GemVal _t1001 = (*gem_v_skip_nl);
            (void)(_t1001.fn(_t1001.env, NULL, 0));
#line 655 "compiler/main.gem"
            while (1) {
                GemVal _t1002 = (*gem_v_peek);
                GemVal _t1003 = _t1002.fn(_t1002.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1003, gem_string("type")), gem_string("]")))) break;
#line 656 "compiler/main.gem"
    GemVal _t1004[] = {gem_string("NAME")};
    GemVal _t1005 = (*gem_v_expect);
    GemVal _t1006 = _t1005.fn(_t1005.env, _t1004, 1);
    GemVal _t1007[] = {gem_v_names, gem_table_get(_t1006, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1007, 2));
#line 657 "compiler/main.gem"
    GemVal _t1008 = (*gem_v_skip_nl);
                (void)(_t1008.fn(_t1008.env, NULL, 0));
#line 658 "compiler/main.gem"
    GemVal _t1009 = (*gem_v_peek);
    GemVal _t1010 = _t1009.fn(_t1009.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1010, gem_string("type")), gem_string(",")))) {
#line 659 "compiler/main.gem"
    GemVal _t1011 = (*gem_v_advance);
                    (void)(_t1011.fn(_t1011.env, NULL, 0));
#line 660 "compiler/main.gem"
    GemVal _t1012 = (*gem_v_skip_nl);
                    (void)(_t1012.fn(_t1012.env, NULL, 0));
                }
            }
#line 663 "compiler/main.gem"
    GemVal _t1013[] = {gem_string("]")};
    GemVal _t1014 = (*gem_v_expect);
            (void)(_t1014.fn(_t1014.env, _t1013, 1));
#line 664 "compiler/main.gem"
    GemVal _t1015[] = {gem_string("=")};
    GemVal _t1016 = (*gem_v_expect);
            (void)(_t1016.fn(_t1016.env, _t1015, 1));
#line 665 "compiler/main.gem"
    GemVal _t1017 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1017.fn(_t1017.env, NULL, 0);
#line 666 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 667 "compiler/main.gem"
    GemVal _t1018[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1018, 1));
#line 668 "compiler/main.gem"
    GemVal _t1019 = gem_table_new();
    GemVal _t1020[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t1019, gem_int(0), gem_fn_make_let(NULL, _t1020, 3));
            GemVal gem_v_stmts = _t1019;
#line 669 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 669 "compiler/main.gem"
    GemVal _t1021[] = {gem_v_names};
            GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t1021, 1);
#line 669 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
#line 669 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_6;
#line 669 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 670 "compiler/main.gem"
    GemVal _t1022[] = {gem_v_tmp};
    GemVal _t1023[] = {gem_v_di};
    GemVal _t1024[] = {gem_fn_make_var(NULL, _t1022, 1), gem_fn_make_int(NULL, _t1023, 1)};
    GemVal _t1025[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_index(NULL, _t1024, 2), gem_v_line};
    GemVal _t1026[] = {gem_v_stmts, gem_fn_make_let(NULL, _t1025, 3)};
                (void)(gem_push_fn(NULL, _t1026, 2));
            }

#line 672 "compiler/main.gem"
    GemVal _t1027 = gem_table_new();
    gem_table_set(_t1027, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1027, gem_string("stmts"), gem_v_stmts);
            GemVal _t1028 = _t1027;
            gem_pop_frame();
            return _t1028;
        }
#line 675 "compiler/main.gem"
    GemVal _t1029[] = {gem_string("NAME")};
    GemVal _t1030 = (*gem_v_expect);
    GemVal _t1031 = _t1030.fn(_t1030.env, _t1029, 1);
        GemVal gem_v_name = gem_table_get(_t1031, gem_string("value"));
#line 676 "compiler/main.gem"
    GemVal _t1032[] = {gem_string("=")};
    GemVal _t1033 = (*gem_v_expect);
        (void)(_t1033.fn(_t1033.env, _t1032, 1));
#line 677 "compiler/main.gem"
    GemVal _t1034 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t1034.fn(_t1034.env, NULL, 0);
#line 678 "compiler/main.gem"
    GemVal _t1035[] = {gem_v_name, gem_v_value, gem_v_line};
        GemVal _t1036 = gem_fn_make_let(NULL, _t1035, 3);
        gem_pop_frame();
        return _t1036;
    }
#line 682 "compiler/main.gem"
    GemVal _t1037 = gem_v_t;
    GemVal _t1041;
    if (!gem_truthy(gem_eq(gem_table_get(_t1037, gem_string("type")), gem_string("fn")))) {
        _t1041 = gem_eq(gem_table_get(_t1037, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t1038[] = {gem_int(1)};
        GemVal _t1039 = (*gem_v_peek_at);
        GemVal _t1040 = _t1039.fn(_t1039.env, _t1038, 1);
        _t1041 = gem_eq(gem_table_get(_t1040, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t1041)) {
#line 683 "compiler/main.gem"
    GemVal _t1042 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1042, gem_string("line"));
#line 684 "compiler/main.gem"
    GemVal _t1043 = (*gem_v_advance);
        (void)(_t1043.fn(_t1043.env, NULL, 0));
#line 685 "compiler/main.gem"
    GemVal _t1044[] = {gem_string("NAME")};
    GemVal _t1045 = (*gem_v_expect);
    GemVal _t1046 = _t1045.fn(_t1045.env, _t1044, 1);
        GemVal gem_v_name = gem_table_get(_t1046, gem_string("value"));
#line 686 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 687 "compiler/main.gem"
    GemVal _t1047[] = {gem_v_name};
    GemVal _t1048[] = {gem_v_line};
    GemVal _t1049[] = {gem_v_name};
    GemVal _t1050[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("named fn '"), gem_to_string_fn(NULL, _t1047, 1)), gem_string("' inside function body is not supported at line ")), gem_to_string_fn(NULL, _t1048, 1)), gem_string(" — use: let ")), gem_to_string_fn(NULL, _t1049, 1)), gem_string(" = fn(...) ... end"))};
            (void)(gem_error_at_fn("compiler/main.gem", 687, _t1050, 1));
        }
#line 689 "compiler/main.gem"
    GemVal _t1051[] = {gem_string("(")};
    GemVal _t1052 = (*gem_v_expect);
        (void)(_t1052.fn(_t1052.env, _t1051, 1));
#line 690 "compiler/main.gem"
    GemVal _t1053 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t1053.fn(_t1053.env, NULL, 0);
#line 691 "compiler/main.gem"
    GemVal _t1054[] = {gem_string(")")};
    GemVal _t1055 = (*gem_v_expect);
        (void)(_t1055.fn(_t1055.env, _t1054, 1));
#line 692 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 693 "compiler/main.gem"
    GemVal _t1056 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t1056.fn(_t1056.env, NULL, 0);
#line 694 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 695 "compiler/main.gem"
    GemVal _t1057[] = {gem_string("end")};
    GemVal _t1058 = (*gem_v_expect);
        (void)(_t1058.fn(_t1058.env, _t1057, 1));
#line 696 "compiler/main.gem"
    GemVal _t1059 = gem_v_pr;
    GemVal _t1060 = gem_v_pr;
    GemVal _t1061 = gem_v_pr;
    GemVal _t1062[] = {gem_v_name, gem_table_get(_t1059, gem_string("params")), gem_table_get(_t1060, gem_string("rest_param")), gem_table_get(_t1061, gem_string("block_param")), gem_v_body, gem_v_line};
        GemVal _t1063 = gem_fn_make_fn_def(NULL, _t1062, 6);
        gem_pop_frame();
        return _t1063;
    }
#line 700 "compiler/main.gem"
    GemVal _t1064 = gem_v_t;
    GemVal _t1066;
    if (gem_truthy(gem_eq(gem_table_get(_t1064, gem_string("type")), gem_string("if")))) {
        _t1066 = gem_eq(gem_table_get(_t1064, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t1065 = gem_v_t;
        _t1066 = gem_eq(gem_table_get(_t1065, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t1066)) {
#line 701 "compiler/main.gem"
    GemVal _t1067 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1067, gem_string("line"));
#line 702 "compiler/main.gem"
    GemVal _t1068 = (*gem_v_advance);
        (void)(_t1068.fn(_t1068.env, NULL, 0));
#line 703 "compiler/main.gem"
    GemVal _t1069 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1069.fn(_t1069.env, NULL, 0);
#line 704 "compiler/main.gem"
    GemVal _t1070 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t1070.fn(_t1070.env, NULL, 0);
#line 705 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 706 "compiler/main.gem"
    GemVal _t1071 = (*gem_v_peek);
    GemVal _t1072 = _t1071.fn(_t1071.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1072, gem_string("type")), gem_string("elif")))) {
#line 708 "compiler/main.gem"
    GemVal _t1073 = gem_table_new();
    GemVal _t1074 = (*gem_v_parse_stmt);
    gem_table_set(_t1073, gem_int(0), _t1074.fn(_t1074.env, NULL, 0));
            gem_v_else_body = _t1073;
        } else {
#line 709 "compiler/main.gem"
    GemVal _t1075 = (*gem_v_peek);
    GemVal _t1076 = _t1075.fn(_t1075.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1076, gem_string("type")), gem_string("else")))) {
#line 710 "compiler/main.gem"
    GemVal _t1077 = (*gem_v_advance);
                (void)(_t1077.fn(_t1077.env, NULL, 0));
#line 711 "compiler/main.gem"
    GemVal _t1078 = (*gem_v_parse_body);
                gem_v_else_body = _t1078.fn(_t1078.env, NULL, 0);
            }
        }
#line 714 "compiler/main.gem"
    GemVal _t1079 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t1079, gem_string("type")), gem_string("if")))) {
#line 715 "compiler/main.gem"
    GemVal _t1080[] = {gem_string("end")};
    GemVal _t1081 = (*gem_v_expect);
            (void)(_t1081.fn(_t1081.env, _t1080, 1));
        }
#line 717 "compiler/main.gem"
    GemVal _t1082[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        GemVal _t1083 = gem_fn_make_if(NULL, _t1082, 4);
        gem_pop_frame();
        return _t1083;
    }
#line 721 "compiler/main.gem"
    GemVal _t1084 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1084, gem_string("type")), gem_string("while")))) {
#line 722 "compiler/main.gem"
    GemVal _t1085 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1085, gem_string("line"));
#line 723 "compiler/main.gem"
    GemVal _t1086 = (*gem_v_advance);
        (void)(_t1086.fn(_t1086.env, NULL, 0));
#line 724 "compiler/main.gem"
    GemVal _t1087 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1087.fn(_t1087.env, NULL, 0);
#line 725 "compiler/main.gem"
    GemVal _t1088 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t1088.fn(_t1088.env, NULL, 0);
#line 726 "compiler/main.gem"
    GemVal _t1089[] = {gem_string("end")};
    GemVal _t1090 = (*gem_v_expect);
        (void)(_t1090.fn(_t1090.env, _t1089, 1));
#line 727 "compiler/main.gem"
    GemVal _t1091[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        GemVal _t1092 = gem_fn_make_while(NULL, _t1091, 3);
        gem_pop_frame();
        return _t1092;
    }
#line 731 "compiler/main.gem"
    GemVal _t1093 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1093, gem_string("type")), gem_string("for")))) {
#line 732 "compiler/main.gem"
    GemVal _t1094 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1094, gem_string("line"));
#line 733 "compiler/main.gem"
    GemVal _t1095 = (*gem_v_advance);
        (void)(_t1095.fn(_t1095.env, NULL, 0));
#line 734 "compiler/main.gem"
    GemVal _t1096[] = {gem_string("NAME")};
    GemVal _t1097 = (*gem_v_expect);
    GemVal _t1098 = _t1097.fn(_t1097.env, _t1096, 1);
        GemVal gem_v_var_name = gem_table_get(_t1098, gem_string("value"));
#line 736 "compiler/main.gem"
    GemVal _t1099 = (*gem_v_peek);
    GemVal _t1100 = _t1099.fn(_t1099.env, NULL, 0);
    GemVal _t1104;
    if (!gem_truthy(gem_eq(gem_table_get(_t1100, gem_string("type")), gem_string(",")))) {
        _t1104 = gem_eq(gem_table_get(_t1100, gem_string("type")), gem_string(","));
    } else {
        GemVal _t1101[] = {gem_int(1)};
        GemVal _t1102 = (*gem_v_peek_at);
        GemVal _t1103 = _t1102.fn(_t1102.env, _t1101, 1);
        _t1104 = gem_eq(gem_table_get(_t1103, gem_string("type")), gem_string("NAME"));
    }
        if (gem_truthy(_t1104)) {
#line 738 "compiler/main.gem"
    GemVal _t1105 = (*gem_v_advance);
            (void)(_t1105.fn(_t1105.env, NULL, 0));
#line 739 "compiler/main.gem"
    GemVal _t1106[] = {gem_string("NAME")};
    GemVal _t1107 = (*gem_v_expect);
    GemVal _t1108 = _t1107.fn(_t1107.env, _t1106, 1);
            GemVal gem_v_val_name = gem_table_get(_t1108, gem_string("value"));
#line 740 "compiler/main.gem"
    GemVal _t1109[] = {gem_string("in")};
    GemVal _t1110 = (*gem_v_expect);
            (void)(_t1110.fn(_t1110.env, _t1109, 1));
#line 741 "compiler/main.gem"
    GemVal _t1111 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t1111.fn(_t1111.env, NULL, 0);
#line 742 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 743 "compiler/main.gem"
    GemVal _t1112[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_add(gem_string("_for_tbl_"), gem_to_string_fn(NULL, _t1112, 1));
#line 744 "compiler/main.gem"
    GemVal _t1113[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_keys_var = gem_add(gem_string("_for_keys_"), gem_to_string_fn(NULL, _t1113, 1));
#line 745 "compiler/main.gem"
    GemVal _t1114[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1114, 1));
#line 746 "compiler/main.gem"
    GemVal _t1115 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t1115.fn(_t1115.env, NULL, 0);
#line 747 "compiler/main.gem"
    GemVal _t1116[] = {gem_string("end")};
    GemVal _t1117 = (*gem_v_expect);
            (void)(_t1117.fn(_t1117.env, _t1116, 1));
#line 748 "compiler/main.gem"
    GemVal _t1118 = gem_table_new();
            GemVal gem_v_inner_stmts = _t1118;
#line 749 "compiler/main.gem"
    GemVal _t1119[] = {gem_v_keys_var};
    GemVal _t1120[] = {gem_v_idx_var};
    GemVal _t1121[] = {gem_fn_make_var(NULL, _t1119, 1), gem_fn_make_var(NULL, _t1120, 1)};
    GemVal _t1122[] = {gem_v_var_name, gem_fn_make_index(NULL, _t1121, 2), gem_v_line};
    GemVal _t1123[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1122, 3)};
            (void)(gem_push_fn(NULL, _t1123, 2));
#line 750 "compiler/main.gem"
    GemVal _t1124[] = {gem_v_tbl_var};
    GemVal _t1125[] = {gem_v_keys_var};
    GemVal _t1126[] = {gem_v_idx_var};
    GemVal _t1127[] = {gem_fn_make_var(NULL, _t1125, 1), gem_fn_make_var(NULL, _t1126, 1)};
    GemVal _t1128[] = {gem_fn_make_var(NULL, _t1124, 1), gem_fn_make_index(NULL, _t1127, 2)};
    GemVal _t1129[] = {gem_v_val_name, gem_fn_make_index(NULL, _t1128, 2), gem_v_line};
    GemVal _t1130[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1129, 3)};
            (void)(gem_push_fn(NULL, _t1130, 2));
#line 751 "compiler/main.gem"
    GemVal _t1131[] = {gem_v_idx_var};
    GemVal _t1132[] = {gem_int(1)};
    GemVal _t1133[] = {gem_string("+"), gem_fn_make_var(NULL, _t1131, 1), gem_fn_make_int(NULL, _t1132, 1)};
    GemVal _t1134[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1133, 3), gem_v_line};
    GemVal _t1135[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1134, 3)};
            (void)(gem_push_fn(NULL, _t1135, 2));
#line 752 "compiler/main.gem"
            GemVal gem_v__for_i_7 = gem_int(0);
#line 752 "compiler/main.gem"
    GemVal _t1136[] = {gem_v_fbody};
            GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1136, 1);
#line 752 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 752 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_7;
#line 752 "compiler/main.gem"
                gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 753 "compiler/main.gem"
    GemVal _t1137[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t1137, 2));
            }

#line 755 "compiler/main.gem"
    GemVal _t1138[] = {gem_v_idx_var};
    GemVal _t1139[] = {gem_string("len")};
    GemVal _t1140 = gem_table_new();
    GemVal _t1141[] = {gem_v_keys_var};
    gem_table_set(_t1140, gem_int(0), gem_fn_make_var(NULL, _t1141, 1));
    GemVal _t1142[] = {gem_fn_make_var(NULL, _t1139, 1), _t1140, gem_int(0)};
    GemVal _t1143[] = {gem_string("<"), gem_fn_make_var(NULL, _t1138, 1), gem_fn_make_call(NULL, _t1142, 3)};
    GemVal _t1144[] = {gem_fn_make_binop(NULL, _t1143, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1144, 3);
#line 760 "compiler/main.gem"
    GemVal _t1145 = gem_table_new();
    gem_table_set(_t1145, gem_string("tag"), gem_string("block"));
    GemVal _t1146 = gem_table_new();
    GemVal _t1147[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    gem_table_set(_t1146, gem_int(0), gem_fn_make_let(NULL, _t1147, 3));
    GemVal _t1148[] = {gem_string("keys")};
    GemVal _t1149 = gem_table_new();
    GemVal _t1150[] = {gem_v_tbl_var};
    gem_table_set(_t1149, gem_int(0), gem_fn_make_var(NULL, _t1150, 1));
    GemVal _t1151[] = {gem_fn_make_var(NULL, _t1148, 1), _t1149, gem_int(0)};
    GemVal _t1152[] = {gem_v_keys_var, gem_fn_make_call(NULL, _t1151, 3), gem_v_line};
    gem_table_set(_t1146, gem_int(1), gem_fn_make_let(NULL, _t1152, 3));
    GemVal _t1153[] = {gem_int(0)};
    GemVal _t1154[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t1153, 1), gem_v_line};
    gem_table_set(_t1146, gem_int(2), gem_fn_make_let(NULL, _t1154, 3));
    gem_table_set(_t1146, gem_int(3), gem_v_while_node);
    gem_table_set(_t1145, gem_string("stmts"), _t1146);
            GemVal _t1155 = _t1145;
            gem_pop_frame();
            return _t1155;
        } else {
#line 766 "compiler/main.gem"
    GemVal _t1156 = (*gem_v_peek);
    GemVal _t1157 = _t1156.fn(_t1156.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1157, gem_string("type")), gem_string("in")))) {
#line 768 "compiler/main.gem"
    GemVal _t1158 = (*gem_v_advance);
                (void)(_t1158.fn(_t1158.env, NULL, 0));
#line 769 "compiler/main.gem"
    GemVal _t1159 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t1159.fn(_t1159.env, NULL, 0);
#line 770 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 771 "compiler/main.gem"
    GemVal _t1160[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t1160, 1));
#line 772 "compiler/main.gem"
    GemVal _t1161[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1161, 1));
#line 773 "compiler/main.gem"
    GemVal _t1162 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t1162.fn(_t1162.env, NULL, 0);
#line 774 "compiler/main.gem"
    GemVal _t1163[] = {gem_string("end")};
    GemVal _t1164 = (*gem_v_expect);
                (void)(_t1164.fn(_t1164.env, _t1163, 1));
#line 775 "compiler/main.gem"
    GemVal _t1165 = gem_table_new();
                GemVal gem_v_inner_stmts = _t1165;
#line 776 "compiler/main.gem"
    GemVal _t1166[] = {gem_v_items_var};
    GemVal _t1167[] = {gem_v_idx_var};
    GemVal _t1168[] = {gem_fn_make_var(NULL, _t1166, 1), gem_fn_make_var(NULL, _t1167, 1)};
    GemVal _t1169[] = {gem_v_var_name, gem_fn_make_index(NULL, _t1168, 2), gem_v_line};
    GemVal _t1170[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1169, 3)};
                (void)(gem_push_fn(NULL, _t1170, 2));
#line 777 "compiler/main.gem"
    GemVal _t1171[] = {gem_v_idx_var};
    GemVal _t1172[] = {gem_int(1)};
    GemVal _t1173[] = {gem_string("+"), gem_fn_make_var(NULL, _t1171, 1), gem_fn_make_int(NULL, _t1172, 1)};
    GemVal _t1174[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1173, 3), gem_v_line};
    GemVal _t1175[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1174, 3)};
                (void)(gem_push_fn(NULL, _t1175, 2));
#line 778 "compiler/main.gem"
                GemVal gem_v__for_i_8 = gem_int(0);
#line 778 "compiler/main.gem"
    GemVal _t1176[] = {gem_v_fbody};
                GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1176, 1);
#line 778 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 778 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_8;
#line 778 "compiler/main.gem"
                    gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 779 "compiler/main.gem"
    GemVal _t1177[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t1177, 2));
                }

#line 781 "compiler/main.gem"
    GemVal _t1178[] = {gem_v_idx_var};
    GemVal _t1179[] = {gem_string("len")};
    GemVal _t1180 = gem_table_new();
    GemVal _t1181[] = {gem_v_items_var};
    gem_table_set(_t1180, gem_int(0), gem_fn_make_var(NULL, _t1181, 1));
    GemVal _t1182[] = {gem_fn_make_var(NULL, _t1179, 1), _t1180, gem_int(0)};
    GemVal _t1183[] = {gem_string("<"), gem_fn_make_var(NULL, _t1178, 1), gem_fn_make_call(NULL, _t1182, 3)};
    GemVal _t1184[] = {gem_fn_make_binop(NULL, _t1183, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1184, 3);
#line 786 "compiler/main.gem"
    GemVal _t1185 = gem_table_new();
    gem_table_set(_t1185, gem_string("tag"), gem_string("block"));
    GemVal _t1186 = gem_table_new();
    GemVal _t1187[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t1186, gem_int(0), gem_fn_make_let(NULL, _t1187, 3));
    GemVal _t1188[] = {gem_int(0)};
    GemVal _t1189[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t1188, 1), gem_v_line};
    gem_table_set(_t1186, gem_int(1), gem_fn_make_let(NULL, _t1189, 3));
    gem_table_set(_t1186, gem_int(2), gem_v_while_node);
    gem_table_set(_t1185, gem_string("stmts"), _t1186);
                GemVal _t1190 = _t1185;
                gem_pop_frame();
                return _t1190;
            } else {
#line 791 "compiler/main.gem"
    GemVal _t1191 = (*gem_v_peek);
    GemVal _t1192 = _t1191.fn(_t1191.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1192, gem_string("type")), gem_string("=")))) {
#line 793 "compiler/main.gem"
    GemVal _t1193 = (*gem_v_advance);
                    (void)(_t1193.fn(_t1193.env, NULL, 0));
#line 794 "compiler/main.gem"
    GemVal _t1194 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t1194.fn(_t1194.env, NULL, 0);
#line 795 "compiler/main.gem"
    GemVal _t1195[] = {gem_string(",")};
    GemVal _t1196 = (*gem_v_expect);
                    (void)(_t1196.fn(_t1196.env, _t1195, 1));
#line 796 "compiler/main.gem"
    GemVal _t1197 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1197.fn(_t1197.env, NULL, 0);
#line 797 "compiler/main.gem"
    GemVal _t1198 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1198.fn(_t1198.env, NULL, 0);
#line 798 "compiler/main.gem"
    GemVal _t1199[] = {gem_string("end")};
    GemVal _t1200 = (*gem_v_expect);
                    (void)(_t1200.fn(_t1200.env, _t1199, 1));
#line 799 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 800 "compiler/main.gem"
    GemVal _t1201[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1201, 1));
#line 801 "compiler/main.gem"
    GemVal _t1202[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t1202, 1));
#line 802 "compiler/main.gem"
    GemVal _t1203 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1203;
#line 803 "compiler/main.gem"
    GemVal _t1204[] = {gem_v_idx_var};
    GemVal _t1205[] = {gem_v_var_name, gem_fn_make_var(NULL, _t1204, 1), gem_v_line};
    GemVal _t1206[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1205, 3)};
                    (void)(gem_push_fn(NULL, _t1206, 2));
#line 804 "compiler/main.gem"
    GemVal _t1207[] = {gem_v_idx_var};
    GemVal _t1208[] = {gem_int(1)};
    GemVal _t1209[] = {gem_string("+"), gem_fn_make_var(NULL, _t1207, 1), gem_fn_make_int(NULL, _t1208, 1)};
    GemVal _t1210[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1209, 3), gem_v_line};
    GemVal _t1211[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1210, 3)};
                    (void)(gem_push_fn(NULL, _t1211, 2));
#line 805 "compiler/main.gem"
                    GemVal gem_v__for_i_9 = gem_int(0);
#line 805 "compiler/main.gem"
    GemVal _t1212[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1212, 1);
#line 805 "compiler/main.gem"
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 805 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_9;
#line 805 "compiler/main.gem"
                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 806 "compiler/main.gem"
    GemVal _t1213[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1213, 2));
                    }

#line 808 "compiler/main.gem"
    GemVal _t1214[] = {gem_v_idx_var};
    GemVal _t1215[] = {gem_v_limit_var};
    GemVal _t1216[] = {gem_string("<"), gem_fn_make_var(NULL, _t1214, 1), gem_fn_make_var(NULL, _t1215, 1)};
    GemVal _t1217[] = {gem_fn_make_binop(NULL, _t1216, 3), gem_v_inner_stmts, gem_v_line};
                    GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1217, 3);
#line 813 "compiler/main.gem"
    GemVal _t1218 = gem_table_new();
    gem_table_set(_t1218, gem_string("tag"), gem_string("block"));
    GemVal _t1219 = gem_table_new();
    GemVal _t1220[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t1219, gem_int(0), gem_fn_make_let(NULL, _t1220, 3));
    GemVal _t1221[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t1219, gem_int(1), gem_fn_make_let(NULL, _t1221, 3));
    gem_table_set(_t1219, gem_int(2), gem_v_while_node);
    gem_table_set(_t1218, gem_string("stmts"), _t1219);
                    GemVal _t1222 = _t1218;
                    gem_pop_frame();
                    return _t1222;
                } else {
#line 819 "compiler/main.gem"
    GemVal _t1223 = (*gem_v_peek);
    GemVal _t1224 = _t1223.fn(_t1223.env, NULL, 0);
    GemVal _t1225[] = {gem_table_get(_t1224, gem_string("line"))};
    GemVal _t1226[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t1225, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 819, _t1226, 1));
                }
            }
        }
    }
#line 824 "compiler/main.gem"
    GemVal _t1227 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1227, gem_string("type")), gem_string("match")))) {
#line 825 "compiler/main.gem"
    GemVal _t1228 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1228, gem_string("line"));
#line 826 "compiler/main.gem"
    GemVal _t1229 = (*gem_v_advance);
        (void)(_t1229.fn(_t1229.env, NULL, 0));
#line 827 "compiler/main.gem"
    GemVal _t1230 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1230.fn(_t1230.env, NULL, 0);
#line 828 "compiler/main.gem"
    GemVal _t1231 = (*gem_v_skip_nl);
        (void)(_t1231.fn(_t1231.env, NULL, 0));
#line 829 "compiler/main.gem"
        *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 830 "compiler/main.gem"
    GemVal _t1232[] = {(*gem_v_gensym_counter)};
        GemVal gem_v_match_var = gem_add(gem_string("_match_"), gem_to_string_fn(NULL, _t1232, 1));
#line 831 "compiler/main.gem"
    GemVal _t1233[] = {gem_v_match_var};
        GemVal gem_v_target_var_expr = gem_fn_make_var(NULL, _t1233, 1);
#line 832 "compiler/main.gem"
    GemVal _t1234 = gem_table_new();
        GemVal gem_v_whens = _t1234;
#line 833 "compiler/main.gem"
        while (1) {
            GemVal _t1235 = (*gem_v_peek);
            GemVal _t1236 = _t1235.fn(_t1235.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1236, gem_string("type")), gem_string("when")))) break;
#line 834 "compiler/main.gem"
    GemVal _t1237 = (*gem_v_advance);
            (void)(_t1237.fn(_t1237.env, NULL, 0));
#line 836 "compiler/main.gem"
    GemVal _t1238 = (*gem_v_peek);
    GemVal _t1239 = _t1238.fn(_t1238.env, NULL, 0);
            GemVal gem_v_pt = gem_table_get(_t1239, gem_string("type"));
#line 837 "compiler/main.gem"
    GemVal _t1240[] = {gem_int(1)};
    GemVal _t1241 = (*gem_v_peek_at);
    GemVal _t1242 = _t1241.fn(_t1241.env, _t1240, 1);
            GemVal gem_v_next = gem_table_get(_t1242, gem_string("type"));
#line 838 "compiler/main.gem"
    GemVal _t1243;
    if (gem_truthy(gem_eq(gem_v_pt, gem_string("{")))) {
        _t1243 = gem_eq(gem_v_pt, gem_string("{"));
    } else {
        _t1243 = gem_eq(gem_v_pt, gem_string("["));
    }
            if (gem_truthy(_t1243)) {
#line 839 "compiler/main.gem"
    GemVal _t1244[] = {gem_v_target_var_expr};
    GemVal _t1245 = (*gem_v_parse_pattern);
                GemVal gem_v_pat = _t1245.fn(_t1245.env, _t1244, 1);
#line 840 "compiler/main.gem"
    GemVal _t1246 = (*gem_v_parse_body);
                GemVal gem_v_wbody = _t1246.fn(_t1246.env, NULL, 0);
#line 841 "compiler/main.gem"
    GemVal _t1247 = gem_v_pat;
    GemVal _t1248 = gem_v_pat;
    GemVal _t1249[] = {gem_table_get(_t1247, gem_string("condition")), gem_v_wbody, gem_table_get(_t1248, gem_string("bindings"))};
    GemVal _t1250[] = {gem_v_whens, gem_fn_make_when(NULL, _t1249, 3)};
                (void)(gem_push_fn(NULL, _t1250, 2));
            } else {
#line 842 "compiler/main.gem"
    GemVal _t1255;
    if (!gem_truthy(gem_eq(gem_v_pt, gem_string("NAME")))) {
        _t1255 = gem_eq(gem_v_pt, gem_string("NAME"));
    } else {
        GemVal _t1251;
        if (gem_truthy(gem_eq(gem_v_next, gem_string("NEWLINE")))) {
                _t1251 = gem_eq(gem_v_next, gem_string("NEWLINE"));
        } else {
                _t1251 = gem_eq(gem_v_next, gem_string("EOF"));
        }
        GemVal _t1252;
        if (gem_truthy(_t1251)) {
                _t1252 = _t1251;
        } else {
                _t1252 = gem_eq(gem_v_next, gem_string("end"));
        }
        GemVal _t1253;
        if (gem_truthy(_t1252)) {
                _t1253 = _t1252;
        } else {
                _t1253 = gem_eq(gem_v_next, gem_string("else"));
        }
        GemVal _t1254;
        if (gem_truthy(_t1253)) {
                _t1254 = _t1253;
        } else {
                _t1254 = gem_eq(gem_v_next, gem_string("when"));
        }
        _t1255 = _t1254;
    }
                if (gem_truthy(_t1255)) {
#line 844 "compiler/main.gem"
    GemVal _t1256[] = {gem_v_target_var_expr};
    GemVal _t1257 = (*gem_v_parse_pattern);
                    GemVal gem_v_pat = _t1257.fn(_t1257.env, _t1256, 1);
#line 845 "compiler/main.gem"
    GemVal _t1258 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1258.fn(_t1258.env, NULL, 0);
#line 846 "compiler/main.gem"
    GemVal _t1259 = gem_v_pat;
    GemVal _t1260 = gem_v_pat;
    GemVal _t1261[] = {gem_table_get(_t1259, gem_string("condition")), gem_v_wbody, gem_table_get(_t1260, gem_string("bindings"))};
    GemVal _t1262[] = {gem_v_whens, gem_fn_make_when(NULL, _t1261, 3)};
                    (void)(gem_push_fn(NULL, _t1262, 2));
                } else {
#line 849 "compiler/main.gem"
    GemVal _t1263 = (*gem_v_parse_expr);
                    GemVal gem_v_wval = _t1263.fn(_t1263.env, NULL, 0);
#line 850 "compiler/main.gem"
    GemVal _t1264 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1264.fn(_t1264.env, NULL, 0);
#line 851 "compiler/main.gem"
    GemVal _t1265[] = {gem_string("=="), gem_v_target_var_expr, gem_v_wval};
    GemVal _t1266 = gem_table_new();
    GemVal _t1267[] = {gem_fn_make_binop(NULL, _t1265, 3), gem_v_wbody, _t1266};
    GemVal _t1268[] = {gem_v_whens, gem_fn_make_when(NULL, _t1267, 3)};
                    (void)(gem_push_fn(NULL, _t1268, 2));
                }
            }
#line 853 "compiler/main.gem"
    GemVal _t1269 = (*gem_v_skip_nl);
            (void)(_t1269.fn(_t1269.env, NULL, 0));
        }
#line 855 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 856 "compiler/main.gem"
    GemVal _t1270 = (*gem_v_peek);
    GemVal _t1271 = _t1270.fn(_t1270.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1271, gem_string("type")), gem_string("else")))) {
#line 857 "compiler/main.gem"
    GemVal _t1272 = (*gem_v_advance);
            (void)(_t1272.fn(_t1272.env, NULL, 0));
#line 858 "compiler/main.gem"
    GemVal _t1273 = (*gem_v_parse_body);
            gem_v_else_body = _t1273.fn(_t1273.env, NULL, 0);
        }
#line 860 "compiler/main.gem"
    GemVal _t1274[] = {gem_string("end")};
    GemVal _t1275 = (*gem_v_expect);
        (void)(_t1275.fn(_t1275.env, _t1274, 1));
#line 861 "compiler/main.gem"
    GemVal _t1276[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line, gem_v_match_var};
        GemVal _t1277 = gem_fn_make_match(NULL, _t1276, 5);
        gem_pop_frame();
        return _t1277;
    }
#line 865 "compiler/main.gem"
    GemVal _t1278 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1278, gem_string("type")), gem_string("return")))) {
#line 866 "compiler/main.gem"
    GemVal _t1279 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1279, gem_string("line"));
#line 867 "compiler/main.gem"
    GemVal _t1280 = (*gem_v_advance);
        (void)(_t1280.fn(_t1280.env, NULL, 0));
#line 868 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 869 "compiler/main.gem"
    GemVal _t1281 = (*gem_v_peek);
    GemVal _t1282 = _t1281.fn(_t1281.env, NULL, 0);
    GemVal _t1285;
    if (!gem_truthy(gem_neq(gem_table_get(_t1282, gem_string("type")), gem_string("NEWLINE")))) {
        _t1285 = gem_neq(gem_table_get(_t1282, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t1283 = (*gem_v_peek);
        GemVal _t1284 = _t1283.fn(_t1283.env, NULL, 0);
        _t1285 = gem_neq(gem_table_get(_t1284, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t1288;
    if (!gem_truthy(_t1285)) {
        _t1288 = _t1285;
    } else {
        GemVal _t1286 = (*gem_v_peek);
        GemVal _t1287 = _t1286.fn(_t1286.env, NULL, 0);
        _t1288 = gem_neq(gem_table_get(_t1287, gem_string("type")), gem_string("end"));
    }
    GemVal _t1291;
    if (!gem_truthy(_t1288)) {
        _t1291 = _t1288;
    } else {
        GemVal _t1289 = (*gem_v_peek);
        GemVal _t1290 = _t1289.fn(_t1289.env, NULL, 0);
        _t1291 = gem_neq(gem_table_get(_t1290, gem_string("type")), gem_string("elif"));
    }
    GemVal _t1294;
    if (!gem_truthy(_t1291)) {
        _t1294 = _t1291;
    } else {
        GemVal _t1292 = (*gem_v_peek);
        GemVal _t1293 = _t1292.fn(_t1292.env, NULL, 0);
        _t1294 = gem_neq(gem_table_get(_t1293, gem_string("type")), gem_string("else"));
    }
    GemVal _t1297;
    if (!gem_truthy(_t1294)) {
        _t1297 = _t1294;
    } else {
        GemVal _t1295 = (*gem_v_peek);
        GemVal _t1296 = _t1295.fn(_t1295.env, NULL, 0);
        _t1297 = gem_neq(gem_table_get(_t1296, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t1297)) {
#line 870 "compiler/main.gem"
    GemVal _t1298 = (*gem_v_parse_expr);
            gem_v_value = _t1298.fn(_t1298.env, NULL, 0);
        }
#line 872 "compiler/main.gem"
    GemVal _t1299[] = {gem_v_value, gem_v_line};
        GemVal _t1300 = gem_fn_make_return(NULL, _t1299, 2);
        gem_pop_frame();
        return _t1300;
    }
#line 876 "compiler/main.gem"
    GemVal _t1301 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1301, gem_string("type")), gem_string("break")))) {
#line 877 "compiler/main.gem"
    GemVal _t1302 = (*gem_v_advance);
        (void)(_t1302.fn(_t1302.env, NULL, 0));
#line 878 "compiler/main.gem"
        GemVal _t1303 = gem_fn_make_break(NULL, NULL, 0);
        gem_pop_frame();
        return _t1303;
    }
#line 882 "compiler/main.gem"
    GemVal _t1304 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1304, gem_string("type")), gem_string("continue")))) {
#line 883 "compiler/main.gem"
    GemVal _t1305 = (*gem_v_advance);
        (void)(_t1305.fn(_t1305.env, NULL, 0));
#line 884 "compiler/main.gem"
        GemVal _t1306 = gem_fn_make_continue(NULL, NULL, 0);
        gem_pop_frame();
        return _t1306;
    }
#line 888 "compiler/main.gem"
    GemVal _t1307 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1307, gem_string("type")), gem_string("load")))) {
#line 889 "compiler/main.gem"
    GemVal _t1308 = (*gem_v_advance);
        (void)(_t1308.fn(_t1308.env, NULL, 0));
#line 890 "compiler/main.gem"
    GemVal _t1309[] = {gem_string("STRING")};
    GemVal _t1310 = (*gem_v_expect);
    GemVal _t1311 = _t1310.fn(_t1310.env, _t1309, 1);
        GemVal gem_v_path = gem_table_get(_t1311, gem_string("value"));
#line 891 "compiler/main.gem"
    GemVal _t1312[] = {gem_v_path};
        GemVal _t1313 = gem_fn_make_load(NULL, _t1312, 1);
        gem_pop_frame();
        return _t1313;
    }
#line 895 "compiler/main.gem"
    GemVal _t1314 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1314, gem_string("type")), gem_string("extern")))) {
#line 896 "compiler/main.gem"
    GemVal _t1315 = (*gem_v_advance);
        (void)(_t1315.fn(_t1315.env, NULL, 0));
#line 897 "compiler/main.gem"
    GemVal _t1316 = (*gem_v_peek);
    GemVal _t1317 = _t1316.fn(_t1316.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1317, gem_string("type")), gem_string("fn")))) {
#line 898 "compiler/main.gem"
    GemVal _t1318 = (*gem_v_advance);
            (void)(_t1318.fn(_t1318.env, NULL, 0));
#line 899 "compiler/main.gem"
    GemVal _t1319[] = {gem_string("NAME")};
    GemVal _t1320 = (*gem_v_expect);
    GemVal _t1321 = _t1320.fn(_t1320.env, _t1319, 1);
            GemVal gem_v_name = gem_table_get(_t1321, gem_string("value"));
#line 900 "compiler/main.gem"
    GemVal _t1322[] = {gem_string("(")};
    GemVal _t1323 = (*gem_v_expect);
            (void)(_t1323.fn(_t1323.env, _t1322, 1));
#line 901 "compiler/main.gem"
    GemVal _t1324 = gem_table_new();
            GemVal gem_v_eparams = _t1324;
#line 902 "compiler/main.gem"
    GemVal _t1325 = (*gem_v_peek);
    GemVal _t1326 = _t1325.fn(_t1325.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t1326, gem_string("type")), gem_string(")")))) {
#line 903 "compiler/main.gem"
    GemVal _t1327[] = {gem_string("NAME")};
    GemVal _t1328 = (*gem_v_expect);
    GemVal _t1329 = _t1328.fn(_t1328.env, _t1327, 1);
                GemVal gem_v_pname = gem_table_get(_t1329, gem_string("value"));
#line 904 "compiler/main.gem"
    GemVal _t1330[] = {gem_string(":")};
    GemVal _t1331 = (*gem_v_expect);
                (void)(_t1331.fn(_t1331.env, _t1330, 1));
#line 905 "compiler/main.gem"
    GemVal _t1332[] = {gem_string("NAME")};
    GemVal _t1333 = (*gem_v_expect);
    GemVal _t1334 = _t1333.fn(_t1333.env, _t1332, 1);
                GemVal gem_v_ptype = gem_table_get(_t1334, gem_string("value"));
#line 906 "compiler/main.gem"
    GemVal _t1335[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1336[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1335, 2)};
                (void)(gem_push_fn(NULL, _t1336, 2));
#line 907 "compiler/main.gem"
                while (1) {
                    GemVal _t1337 = (*gem_v_peek);
                    GemVal _t1338 = _t1337.fn(_t1337.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1338, gem_string("type")), gem_string(",")))) break;
#line 908 "compiler/main.gem"
    GemVal _t1339 = (*gem_v_advance);
                    (void)(_t1339.fn(_t1339.env, NULL, 0));
#line 909 "compiler/main.gem"
    GemVal _t1340[] = {gem_string("NAME")};
    GemVal _t1341 = (*gem_v_expect);
    GemVal _t1342 = _t1341.fn(_t1341.env, _t1340, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t1342, gem_string("value"));
#line 910 "compiler/main.gem"
    GemVal _t1343[] = {gem_string(":")};
    GemVal _t1344 = (*gem_v_expect);
                    (void)(_t1344.fn(_t1344.env, _t1343, 1));
#line 911 "compiler/main.gem"
    GemVal _t1345[] = {gem_string("NAME")};
    GemVal _t1346 = (*gem_v_expect);
    GemVal _t1347 = _t1346.fn(_t1346.env, _t1345, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t1347, gem_string("value"));
#line 912 "compiler/main.gem"
    GemVal _t1348[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1349[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1348, 2)};
                    (void)(gem_push_fn(NULL, _t1349, 2));
                }
            }
#line 915 "compiler/main.gem"
    GemVal _t1350[] = {gem_string(")")};
    GemVal _t1351 = (*gem_v_expect);
            (void)(_t1351.fn(_t1351.env, _t1350, 1));
#line 916 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 917 "compiler/main.gem"
    GemVal _t1352 = (*gem_v_peek);
    GemVal _t1353 = _t1352.fn(_t1352.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1353, gem_string("type")), gem_string("->")))) {
#line 918 "compiler/main.gem"
    GemVal _t1354 = (*gem_v_advance);
                (void)(_t1354.fn(_t1354.env, NULL, 0));
#line 919 "compiler/main.gem"
    GemVal _t1355[] = {gem_string("NAME")};
    GemVal _t1356 = (*gem_v_expect);
    GemVal _t1357 = _t1356.fn(_t1356.env, _t1355, 1);
                gem_v_ret_type = gem_table_get(_t1357, gem_string("value"));
            }
#line 921 "compiler/main.gem"
    GemVal _t1358[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            GemVal _t1359 = gem_fn_make_extern_fn(NULL, _t1358, 3);
            gem_pop_frame();
            return _t1359;
        } else {
#line 922 "compiler/main.gem"
    GemVal _t1360 = (*gem_v_peek);
    GemVal _t1361 = _t1360.fn(_t1360.env, NULL, 0);
    GemVal _t1364;
    if (!gem_truthy(gem_eq(gem_table_get(_t1361, gem_string("type")), gem_string("NAME")))) {
        _t1364 = gem_eq(gem_table_get(_t1361, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1362 = (*gem_v_peek);
        GemVal _t1363 = _t1362.fn(_t1362.env, NULL, 0);
        _t1364 = gem_eq(gem_table_get(_t1363, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t1364)) {
#line 923 "compiler/main.gem"
    GemVal _t1365 = (*gem_v_advance);
                (void)(_t1365.fn(_t1365.env, NULL, 0));
#line 924 "compiler/main.gem"
    GemVal _t1366[] = {gem_string("STRING")};
    GemVal _t1367 = (*gem_v_expect);
    GemVal _t1368 = _t1367.fn(_t1367.env, _t1366, 1);
                GemVal gem_v_path = gem_table_get(_t1368, gem_string("value"));
#line 925 "compiler/main.gem"
    GemVal _t1369[] = {gem_v_path};
                GemVal _t1370 = gem_fn_make_extern_include(NULL, _t1369, 1);
                gem_pop_frame();
                return _t1370;
            } else {
#line 927 "compiler/main.gem"
    GemVal _t1371 = (*gem_v_peek);
    GemVal _t1372 = _t1371.fn(_t1371.env, NULL, 0);
    GemVal _t1373[] = {gem_table_get(_t1372, gem_string("line"))};
    GemVal _t1374[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t1373, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 927, _t1374, 1));
            }
        }
    }
#line 932 "compiler/main.gem"
    GemVal _t1375 = gem_v_t;
    GemVal _t1377;
    if (!gem_truthy(gem_eq(gem_table_get(_t1375, gem_string("type")), gem_string("NAME")))) {
        _t1377 = gem_eq(gem_table_get(_t1375, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1376 = gem_v_t;
        _t1377 = gem_eq(gem_table_get(_t1376, gem_string("value")), gem_string("receive"));
    }
    if (gem_truthy(_t1377)) {
#line 933 "compiler/main.gem"
        GemVal gem_v_rla = gem_int(1);
#line 934 "compiler/main.gem"
        while (1) {
            GemVal _t1378[] = {gem_v_rla};
            GemVal _t1379 = (*gem_v_peek_at);
            GemVal _t1380 = _t1379.fn(_t1379.env, _t1378, 1);
            if (!gem_truthy(gem_eq(gem_table_get(_t1380, gem_string("type")), gem_string("NEWLINE")))) break;
#line 935 "compiler/main.gem"
            gem_v_rla = gem_add(gem_v_rla, gem_int(1));
        }
#line 937 "compiler/main.gem"
    GemVal _t1381[] = {gem_v_rla};
    GemVal _t1382 = (*gem_v_peek_at);
    GemVal _t1383 = _t1382.fn(_t1382.env, _t1381, 1);
        if (gem_truthy(gem_eq(gem_table_get(_t1383, gem_string("type")), gem_string("when")))) {
#line 938 "compiler/main.gem"
    GemVal _t1384 = gem_v_t;
            GemVal gem_v_rline = gem_table_get(_t1384, gem_string("line"));
#line 939 "compiler/main.gem"
    GemVal _t1385 = (*gem_v_advance);
            (void)(_t1385.fn(_t1385.env, NULL, 0));
#line 940 "compiler/main.gem"
    GemVal _t1386 = (*gem_v_skip_nl);
            (void)(_t1386.fn(_t1386.env, NULL, 0));
#line 941 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 942 "compiler/main.gem"
    GemVal _t1387[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_recv_var = gem_add(gem_string("_recv_"), gem_to_string_fn(NULL, _t1387, 1));
#line 943 "compiler/main.gem"
    GemVal _t1388[] = {gem_v_recv_var};
            GemVal gem_v_recv_var_expr = gem_fn_make_var(NULL, _t1388, 1);
#line 944 "compiler/main.gem"
    GemVal _t1389 = gem_table_new();
            GemVal gem_v_rarms = _t1389;
#line 945 "compiler/main.gem"
            while (1) {
                GemVal _t1390 = (*gem_v_peek);
                GemVal _t1391 = _t1390.fn(_t1390.env, NULL, 0);
                if (!gem_truthy(gem_eq(gem_table_get(_t1391, gem_string("type")), gem_string("when")))) break;
#line 946 "compiler/main.gem"
    GemVal _t1392 = (*gem_v_advance);
                (void)(_t1392.fn(_t1392.env, NULL, 0));
#line 947 "compiler/main.gem"
    GemVal _t1393 = (*gem_v_peek);
    GemVal _t1394 = _t1393.fn(_t1393.env, NULL, 0);
                GemVal gem_v_rpt = gem_table_get(_t1394, gem_string("type"));
#line 948 "compiler/main.gem"
    GemVal _t1395[] = {gem_int(1)};
    GemVal _t1396 = (*gem_v_peek_at);
    GemVal _t1397 = _t1396.fn(_t1396.env, _t1395, 1);
                GemVal gem_v_rnext = gem_table_get(_t1397, gem_string("type"));
#line 949 "compiler/main.gem"
    GemVal _t1398;
    if (gem_truthy(gem_eq(gem_v_rpt, gem_string("{")))) {
        _t1398 = gem_eq(gem_v_rpt, gem_string("{"));
    } else {
        _t1398 = gem_eq(gem_v_rpt, gem_string("["));
    }
                if (gem_truthy(_t1398)) {
#line 950 "compiler/main.gem"
    GemVal _t1399[] = {gem_v_recv_var_expr};
    GemVal _t1400 = (*gem_v_parse_pattern);
                    GemVal gem_v_rpat = _t1400.fn(_t1400.env, _t1399, 1);
#line 951 "compiler/main.gem"
    GemVal _t1401 = (*gem_v_parse_body);
                    GemVal gem_v_rwbody = _t1401.fn(_t1401.env, NULL, 0);
#line 952 "compiler/main.gem"
    GemVal _t1402 = gem_table_new();
    gem_table_set(_t1402, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1402, gem_string("body"), gem_v_rwbody);
    GemVal _t1403[] = {gem_v_rarms, _t1402};
                    (void)(gem_push_fn(NULL, _t1403, 2));
                } else {
#line 953 "compiler/main.gem"
    GemVal _t1409;
    if (!gem_truthy(gem_eq(gem_v_rpt, gem_string("NAME")))) {
        _t1409 = gem_eq(gem_v_rpt, gem_string("NAME"));
    } else {
        GemVal _t1404;
        if (gem_truthy(gem_eq(gem_v_rnext, gem_string("NEWLINE")))) {
                _t1404 = gem_eq(gem_v_rnext, gem_string("NEWLINE"));
        } else {
                _t1404 = gem_eq(gem_v_rnext, gem_string("EOF"));
        }
        GemVal _t1405;
        if (gem_truthy(_t1404)) {
                _t1405 = _t1404;
        } else {
                _t1405 = gem_eq(gem_v_rnext, gem_string("end"));
        }
        GemVal _t1406;
        if (gem_truthy(_t1405)) {
                _t1406 = _t1405;
        } else {
                _t1406 = gem_eq(gem_v_rnext, gem_string("else"));
        }
        GemVal _t1407;
        if (gem_truthy(_t1406)) {
                _t1407 = _t1406;
        } else {
                _t1407 = gem_eq(gem_v_rnext, gem_string("when"));
        }
        GemVal _t1408;
        if (gem_truthy(_t1407)) {
                _t1408 = _t1407;
        } else {
                _t1408 = gem_eq(gem_v_rnext, gem_string("after"));
        }
        _t1409 = _t1408;
    }
                    if (gem_truthy(_t1409)) {
#line 954 "compiler/main.gem"
    GemVal _t1410[] = {gem_v_recv_var_expr};
    GemVal _t1411 = (*gem_v_parse_pattern);
                        GemVal gem_v_rpat = _t1411.fn(_t1411.env, _t1410, 1);
#line 955 "compiler/main.gem"
    GemVal _t1412 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1412.fn(_t1412.env, NULL, 0);
#line 956 "compiler/main.gem"
    GemVal _t1413 = gem_table_new();
    gem_table_set(_t1413, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1413, gem_string("body"), gem_v_rwbody);
    GemVal _t1414[] = {gem_v_rarms, _t1413};
                        (void)(gem_push_fn(NULL, _t1414, 2));
                    } else {
#line 958 "compiler/main.gem"
    GemVal _t1415 = (*gem_v_parse_expr);
                        GemVal gem_v_rwval = _t1415.fn(_t1415.env, NULL, 0);
#line 959 "compiler/main.gem"
    GemVal _t1416 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1416.fn(_t1416.env, NULL, 0);
#line 960 "compiler/main.gem"
    GemVal _t1417 = gem_table_new();
    GemVal _t1418 = gem_table_new();
    GemVal _t1419[] = {gem_string("=="), gem_v_recv_var_expr, gem_v_rwval};
    gem_table_set(_t1418, gem_string("condition"), gem_fn_make_binop(NULL, _t1419, 3));
    GemVal _t1420 = gem_table_new();
    gem_table_set(_t1418, gem_string("bindings"), _t1420);
    gem_table_set(_t1417, gem_string("pattern"), _t1418);
    gem_table_set(_t1417, gem_string("body"), gem_v_rwbody);
    GemVal _t1421[] = {gem_v_rarms, _t1417};
                        (void)(gem_push_fn(NULL, _t1421, 2));
                    }
                }
#line 962 "compiler/main.gem"
    GemVal _t1422 = (*gem_v_skip_nl);
                (void)(_t1422.fn(_t1422.env, NULL, 0));
            }
#line 964 "compiler/main.gem"
            GemVal gem_v_rafter_ms = GEM_NIL;
#line 965 "compiler/main.gem"
            GemVal gem_v_rafter_body = GEM_NIL;
#line 966 "compiler/main.gem"
    GemVal _t1423 = (*gem_v_peek);
    GemVal _t1424 = _t1423.fn(_t1423.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1424, gem_string("type")), gem_string("after")))) {
#line 967 "compiler/main.gem"
    GemVal _t1425 = (*gem_v_advance);
                (void)(_t1425.fn(_t1425.env, NULL, 0));
#line 968 "compiler/main.gem"
    GemVal _t1426 = (*gem_v_parse_expr);
                gem_v_rafter_ms = _t1426.fn(_t1426.env, NULL, 0);
#line 969 "compiler/main.gem"
    GemVal _t1427 = (*gem_v_parse_body);
                gem_v_rafter_body = _t1427.fn(_t1427.env, NULL, 0);
            }
#line 971 "compiler/main.gem"
    GemVal _t1428[] = {gem_string("end")};
    GemVal _t1429 = (*gem_v_expect);
            (void)(_t1429.fn(_t1429.env, _t1428, 1));
#line 972 "compiler/main.gem"
    GemVal _t1430[] = {gem_v_rarms, gem_v_rafter_ms, gem_v_rafter_body, gem_v_rline, gem_v_recv_var};
            GemVal _t1431 = gem_fn_make_receive_match(NULL, _t1430, 5);
            gem_pop_frame();
            return _t1431;
        }
    }
#line 977 "compiler/main.gem"
    GemVal _t1432 = (*gem_v_parse_expr);
    GemVal _t1433 = _t1432.fn(_t1432.env, NULL, 0);
    gem_pop_frame();
    return _t1433;
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
#line 986 "compiler/main.gem"
    GemVal _t1435 = gem_table_new();
    GemVal gem_v_stmts = _t1435;
#line 987 "compiler/main.gem"
    GemVal _t1436 = (*gem_v_skip_nl);
    (void)(_t1436.fn(_t1436.env, NULL, 0));
#line 988 "compiler/main.gem"
    while (1) {
        GemVal _t1437 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1437.fn(_t1437.env, NULL, 0)))) break;
#line 989 "compiler/main.gem"
    GemVal _t1438 = (*gem_v_parse_stmt);
    GemVal _t1439[] = {gem_v_stmts, _t1438.fn(_t1438.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1439, 2));
#line 990 "compiler/main.gem"
    GemVal _t1440 = (*gem_v_skip_nl);
        (void)(_t1440.fn(_t1440.env, NULL, 0));
    }
#line 992 "compiler/main.gem"
    GemVal _t1441[] = {gem_v_stmts};
    GemVal _t1442 = gem_fn_make_program(NULL, _t1441, 1);
    gem_pop_frame();
    return _t1442;
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
    struct _closure__anon_1 *_t341 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t341->gem_v_pos = gem_v_pos;
    _t341->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t341);
#line 29 "compiler/main.gem"
    struct _closure__anon_2 *_t343 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t343->gem_v_pos = gem_v_pos;
    _t343->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t343);
#line 33 "compiler/main.gem"
    struct _closure__anon_3 *_t345 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t345->gem_v_pos = gem_v_pos;
    _t345->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t345);
#line 39 "compiler/main.gem"
    struct _closure__anon_4 *_t348 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t348->gem_v_pos = gem_v_pos;
    _t348->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t348);
#line 43 "compiler/main.gem"
    struct _closure__anon_5 *_t357 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t357->gem_v_pos = gem_v_pos;
    _t357->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t357);
#line 52 "compiler/main.gem"
    struct _closure__anon_6 *_t359 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t359->gem_v_pos = gem_v_pos;
    _t359->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t359);
#line 64 "compiler/main.gem"
    struct _closure__anon_7 *_t397 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t397->gem_v_advance = gem_v_advance;
    _t397->gem_v_expect = gem_v_expect;
    _t397->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t397);
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
#line 101 "compiler/main.gem"
    GemVal *gem_v_parse_pattern = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_pattern = GEM_NIL;
#line 107 "compiler/main.gem"
    struct _closure__anon_8 *_t420 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t420->gem_v_at_end = gem_v_at_end;
    _t420->gem_v_parse_stmt = gem_v_parse_stmt;
    _t420->gem_v_peek = gem_v_peek;
    _t420->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t420);
#line 120 "compiler/main.gem"
    struct _closure__anon_9 *_t431 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t431->gem_v_at_end = gem_v_at_end;
    _t431->gem_v_parse_stmt = gem_v_parse_stmt;
    _t431->gem_v_peek = gem_v_peek;
    _t431->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t431);
#line 133 "compiler/main.gem"
    struct _closure__anon_10 *_t563 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t563->gem_v_advance = gem_v_advance;
    _t563->gem_v_expect = gem_v_expect;
    _t563->gem_v_fn_depth = gem_v_fn_depth;
    _t563->gem_v_parse_expr = gem_v_parse_expr;
    _t563->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t563->gem_v_parse_params = gem_v_parse_params;
    _t563->gem_v_peek = gem_v_peek;
    _t563->gem_v_peek_at = gem_v_peek_at;
    _t563->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t563);
#line 264 "compiler/main.gem"
    struct _closure__anon_11 *_t654 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t654->gem_v_advance = gem_v_advance;
    _t654->gem_v_expect = gem_v_expect;
    _t654->gem_v_fn_depth = gem_v_fn_depth;
    _t654->gem_v_parse_atom = gem_v_parse_atom;
    _t654->gem_v_parse_expr = gem_v_parse_expr;
    _t654->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t654->gem_v_parse_params = gem_v_parse_params;
    _t654->gem_v_peek = gem_v_peek;
    _t654->gem_v_peek_at = gem_v_peek_at;
    _t654->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t654);
#line 370 "compiler/main.gem"
    struct _closure__anon_12 *_t663 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t663->gem_v_advance = gem_v_advance;
    _t663->gem_v_parse_call = gem_v_parse_call;
    _t663->gem_v_parse_unary = gem_v_parse_unary;
    _t663->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t663);
#line 379 "compiler/main.gem"
    struct _closure__anon_13 *_t678 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t678->gem_v_advance = gem_v_advance;
    _t678->gem_v_parse_unary = gem_v_parse_unary;
    _t678->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t678);
#line 390 "compiler/main.gem"
    struct _closure__anon_14 *_t690 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t690->gem_v_advance = gem_v_advance;
    _t690->gem_v_parse_mul = gem_v_parse_mul;
    _t690->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t690);
#line 401 "compiler/main.gem"
    struct _closure__anon_15 *_t717 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t717->gem_v_advance = gem_v_advance;
    _t717->gem_v_parse_add = gem_v_parse_add;
    _t717->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t717);
#line 412 "compiler/main.gem"
    struct _closure__anon_16 *_t726 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t726->gem_v_advance = gem_v_advance;
    _t726->gem_v_parse_compare = gem_v_parse_compare;
    _t726->gem_v_parse_not = gem_v_parse_not;
    _t726->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t726);
#line 421 "compiler/main.gem"
    struct _closure__anon_17 *_t734 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t734->gem_v_advance = gem_v_advance;
    _t734->gem_v_parse_not = gem_v_parse_not;
    _t734->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t734);
#line 432 "compiler/main.gem"
    struct _closure__anon_18 *_t742 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t742->gem_v_advance = gem_v_advance;
    _t742->gem_v_parse_and = gem_v_parse_and;
    _t742->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t742);
#line 443 "compiler/main.gem"
    struct _closure__anon_19 *_t791 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t791->gem_v_advance = gem_v_advance;
    _t791->gem_v_parse_expr = gem_v_parse_expr;
    _t791->gem_v_parse_or = gem_v_parse_or;
    _t791->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t791);
#line 478 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 482 "compiler/main.gem"
    struct _closure__anon_20 *_t959 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t959->gem_v_advance = gem_v_advance;
    _t959->gem_v_expect = gem_v_expect;
    _t959->gem_v_parse_pattern = gem_v_parse_pattern;
    _t959->gem_v_peek = gem_v_peek;
    _t959->gem_v_peek_at = gem_v_peek_at;
    _t959->gem_v_skip_nl = gem_v_skip_nl;
    *gem_v_parse_pattern = gem_make_fn(_anon_20, _t959);
#line 610 "compiler/main.gem"
    struct _closure__anon_22 *_t1434 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1434->gem_v_advance = gem_v_advance;
    _t1434->gem_v_expect = gem_v_expect;
    _t1434->gem_v_fn_depth = gem_v_fn_depth;
    _t1434->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1434->gem_v_parse_body = gem_v_parse_body;
    _t1434->gem_v_parse_expr = gem_v_parse_expr;
    _t1434->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1434->gem_v_parse_params = gem_v_parse_params;
    _t1434->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1434->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1434->gem_v_peek = gem_v_peek;
    _t1434->gem_v_peek_at = gem_v_peek_at;
    _t1434->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_22, _t1434);
#line 981 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 985 "compiler/main.gem"
    struct _closure__anon_23 *_t1443 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1443->gem_v_at_end = gem_v_at_end;
    _t1443->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1443->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_23, _t1443);
    GemVal _t1444 = gem_table_new();
    gem_table_set(_t1444, gem_string("parse"), gem_v_parse);
    GemVal _t1445 = _t1444;
    gem_pop_frame();
    return _t1445;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("escape_c_string", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal _t1446[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1446, 1);
#line 11 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 12 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 12 "compiler/main.gem"
    GemVal _t1447[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1447, 1);
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
    GemVal _t1448[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1448, 1), gem_int(0)))) {
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

    GemVal _t1449 = gem_v_result;
    gem_pop_frame();
    return _t1449;
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

    GemVal _t1450 = gem_v_result;
    gem_pop_frame();
    return _t1450;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 41 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("pad_for", "compiler/main.gem", 41);
#line 42 "compiler/main.gem"
    GemVal _t1451[] = {gem_string("    "), gem_v_indent};
    GemVal _t1452 = gem_fn_repeat_str(NULL, _t1451, 2);
    gem_pop_frame();
    return _t1452;
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 45 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("format_float", "compiler/main.gem", 45);
#line 46 "compiler/main.gem"
    GemVal _t1453[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1453, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 49 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 49 "compiler/main.gem"
    GemVal _t1454[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1454, 1);
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
    GemVal _t1455 = gem_v_s;
    gem_pop_frame();
    return _t1455;
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
    GemVal _t1456 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1456;
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 72 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_union", "compiler/main.gem", 72);
#line 73 "compiler/main.gem"
    GemVal _t1457 = gem_table_new();
    GemVal gem_v_result = _t1457;
#line 74 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 74 "compiler/main.gem"
    GemVal _t1458[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_keys_4 = gem_keys_fn(NULL, _t1458, 1);
#line 74 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 74 "compiler/main.gem"
    while (1) {
        GemVal _t1459[] = {gem_v__for_keys_4};
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_len_fn(NULL, _t1459, 1)))) break;
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
    GemVal _t1460[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_keys_5 = gem_keys_fn(NULL, _t1460, 1);
#line 77 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 77 "compiler/main.gem"
    while (1) {
        GemVal _t1461[] = {gem_v__for_keys_5};
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1461, 1)))) break;
#line 77 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_5, gem_v__for_i_5);
#line 77 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_5, gem_table_get(gem_v__for_keys_5, gem_v__for_i_5));
#line 77 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 78 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1462 = gem_v_result;
    gem_pop_frame();
    return _t1462;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 83 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_intersect", "compiler/main.gem", 83);
#line 84 "compiler/main.gem"
    GemVal _t1463 = gem_table_new();
    GemVal gem_v_result = _t1463;
#line 85 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 85 "compiler/main.gem"
    GemVal _t1464[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_keys_6 = gem_keys_fn(NULL, _t1464, 1);
#line 85 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 85 "compiler/main.gem"
    while (1) {
        GemVal _t1465[] = {gem_v__for_keys_6};
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1465, 1)))) break;
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

    GemVal _t1466 = gem_v_result;
    gem_pop_frame();
    return _t1466;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("set_to_sorted_array", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t1467[] = {gem_v_s};
    GemVal gem_v_ks = gem_keys_fn(NULL, _t1467, 1);
#line 96 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 97 "compiler/main.gem"
    while (1) {
        GemVal _t1468[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t1468, 1)))) break;
#line 98 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
#line 99 "compiler/main.gem"
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
#line 100 "compiler/main.gem"
        while (1) {
            GemVal _t1469;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t1469 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t1469 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t1469)) break;
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
    GemVal _t1470 = gem_v_ks;
    gem_pop_frame();
    return _t1470;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 110 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("sorted_array_to_set", "compiler/main.gem", 110);
#line 111 "compiler/main.gem"
    GemVal _t1471 = gem_table_new();
    GemVal gem_v_s = _t1471;
#line 112 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 112 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 112 "compiler/main.gem"
    while (1) {
        GemVal _t1472[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1472, 1)))) break;
#line 112 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 112 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 113 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1473 = gem_v_s;
    gem_pop_frame();
    return _t1473;
}

static GemVal gem_fn_collect_block_lets(void *_env, GemVal *args, int argc) {
#line 120 "compiler/main.gem"
    GemVal gem_v_stmt = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("collect_block_lets", "compiler/main.gem", 120);
#line 121 "compiler/main.gem"
    GemVal _t1474[] = {gem_v_stmt};
    GemVal _t1476;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1474, 1), gem_string("table")))) {
        _t1476 = gem_eq(gem_type_fn(NULL, _t1474, 1), gem_string("table"));
    } else {
        GemVal _t1475 = gem_v_stmt;
        _t1476 = gem_eq(gem_table_get(_t1475, gem_string("tag")), gem_string("block"));
    }
    if (gem_truthy(_t1476)) {
        {
#line 122 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 122 "compiler/main.gem"
    GemVal _t1477 = gem_v_stmt;
    GemVal _t1478[] = {gem_table_get(_t1477, gem_string("stmts"))};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1478, 1);
#line 122 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 122 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_8;
#line 122 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 123 "compiler/main.gem"
    GemVal _t1479 = gem_v_stmt;
    GemVal _t1480[] = {gem_table_get(gem_table_get(_t1479, gem_string("stmts")), gem_v_j)};
    GemVal _t1483;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1480, 1), gem_string("table")))) {
        _t1483 = gem_eq(gem_type_fn(NULL, _t1480, 1), gem_string("table"));
    } else {
        GemVal _t1481 = gem_v_stmt;
        GemVal _t1482 = gem_table_get(gem_table_get(_t1481, gem_string("stmts")), gem_v_j);
        _t1483 = gem_eq(gem_table_get(_t1482, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1483)) {
#line 124 "compiler/main.gem"
    GemVal _t1484 = gem_v_stmt;
    GemVal _t1485 = gem_table_get(gem_table_get(_t1484, gem_string("stmts")), gem_v_j);
    GemVal _t1486[] = {gem_v_result, gem_table_get(_t1485, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1486, 2));
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
    GemVal _t1487[] = {gem_v_stmts};
        GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1487, 1);
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
    GemVal _t1488[] = {gem_v_s};
    GemVal _t1490;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1488, 1), gem_string("table")))) {
        _t1490 = gem_eq(gem_type_fn(NULL, _t1488, 1), gem_string("table"));
    } else {
        GemVal _t1489 = gem_v_s;
        _t1490 = gem_eq(gem_table_get(_t1489, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1490)) {
#line 134 "compiler/main.gem"
    GemVal _t1491 = gem_v_s;
    GemVal _t1492[] = {gem_v_result, gem_table_get(_t1491, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1492, 2));
            }
#line 136 "compiler/main.gem"
    GemVal _t1493[] = {gem_v_s, gem_v_result};
            (void)(gem_fn_collect_block_lets(NULL, _t1493, 2));
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
    GemVal _t1505[] = {(*gem_v_tmp_counter)};
    GemVal _t1506 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t1505, 1));
    gem_pop_frame();
    return _t1506;
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
    GemVal _t1508[] = {(*gem_v_anon_counter)};
    GemVal _t1509 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t1508, 1));
    gem_pop_frame();
    return _t1509;
}

struct _closure__anon_26 {
};
static GemVal _anon_26(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_26", "compiler/main.gem", 0);
    GemVal _t1511[] = {gem_v_name};
    GemVal _t1512 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t1511, 1));
    gem_pop_frame();
    return _t1512;
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
        GemVal _t1513 = GEM_NIL;
        gem_pop_frame();
        return _t1513;
    }
#line 207 "compiler/main.gem"
    GemVal _t1514[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1514, 1), gem_string("table")))) {
#line 208 "compiler/main.gem"
        GemVal _t1515 = GEM_NIL;
        gem_pop_frame();
        return _t1515;
    }
#line 210 "compiler/main.gem"
    GemVal _t1516 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1516, gem_string("tag")), GEM_NIL))) {
#line 211 "compiler/main.gem"
        GemVal _t1517 = GEM_NIL;
        gem_pop_frame();
        return _t1517;
    }
#line 214 "compiler/main.gem"
    GemVal gem_v__match_11 = gem_v_node;
    GemVal _t1518[] = {gem_v__match_11};
    GemVal _t1520;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1518, 1), gem_string("table")))) {
        _t1520 = gem_eq(gem_type_fn(NULL, _t1518, 1), gem_string("table"));
    } else {
        GemVal _t1519[] = {gem_v__match_11, gem_string("tag")};
        _t1520 = gem_has_key_fn(NULL, _t1519, 2);
    }
    GemVal _t1521;
    if (!gem_truthy(_t1520)) {
        _t1521 = _t1520;
    } else {
        _t1521 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("var"));
    }
    GemVal _t1523;
    if (!gem_truthy(_t1521)) {
        _t1523 = _t1521;
    } else {
        GemVal _t1522[] = {gem_v__match_11, gem_string("name")};
        _t1523 = gem_has_key_fn(NULL, _t1522, 2);
    }
    if (gem_truthy(_t1523)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 216 "compiler/main.gem"
    GemVal _t1524[] = {gem_v_defined, gem_v_name};
    GemVal _t1526;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1524, 2)))) {
        _t1526 = gem_not(gem_fn_set_contains(NULL, _t1524, 2));
    } else {
        GemVal _t1525[] = {(*gem_v_builtins), gem_v_name};
        _t1526 = gem_not(gem_fn_set_contains(NULL, _t1525, 2));
    }
    GemVal _t1528;
    if (!gem_truthy(_t1526)) {
        _t1528 = _t1526;
    } else {
        GemVal _t1527[] = {(*gem_v_defined_fns), gem_v_name};
        _t1528 = gem_not(gem_fn_set_contains(NULL, _t1527, 2));
    }
        if (gem_truthy(_t1528)) {
#line 217 "compiler/main.gem"
    GemVal _t1529[] = {gem_v_free, gem_v_name};
            GemVal _t1530 = gem_fn_set_add(NULL, _t1529, 2);
            gem_pop_frame();
            return _t1530;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1531[] = {gem_v__match_11};
    GemVal _t1533;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1531, 1), gem_string("table")))) {
        _t1533 = gem_eq(gem_type_fn(NULL, _t1531, 1), gem_string("table"));
    } else {
        GemVal _t1532[] = {gem_v__match_11, gem_string("tag")};
        _t1533 = gem_has_key_fn(NULL, _t1532, 2);
    }
    GemVal _t1534;
    if (!gem_truthy(_t1533)) {
        _t1534 = _t1533;
    } else {
        _t1534 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("let"));
    }
    GemVal _t1536;
    if (!gem_truthy(_t1534)) {
        _t1536 = _t1534;
    } else {
        GemVal _t1535[] = {gem_v__match_11, gem_string("value")};
        _t1536 = gem_has_key_fn(NULL, _t1535, 2);
    }
    if (gem_truthy(_t1536)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 220 "compiler/main.gem"
    GemVal _t1537[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1538 = (*gem_v_collect_free_node);
        GemVal _t1539 = _t1538.fn(_t1538.env, _t1537, 3);
        gem_pop_frame();
        return _t1539;
    } else {
    GemVal _t1540[] = {gem_v__match_11};
    GemVal _t1542;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1540, 1), gem_string("table")))) {
        _t1542 = gem_eq(gem_type_fn(NULL, _t1540, 1), gem_string("table"));
    } else {
        GemVal _t1541[] = {gem_v__match_11, gem_string("tag")};
        _t1542 = gem_has_key_fn(NULL, _t1541, 2);
    }
    GemVal _t1543;
    if (!gem_truthy(_t1542)) {
        _t1543 = _t1542;
    } else {
        _t1543 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t1545;
    if (!gem_truthy(_t1543)) {
        _t1545 = _t1543;
    } else {
        GemVal _t1544[] = {gem_v__match_11, gem_string("name")};
        _t1545 = gem_has_key_fn(NULL, _t1544, 2);
    }
    GemVal _t1547;
    if (!gem_truthy(_t1545)) {
        _t1547 = _t1545;
    } else {
        GemVal _t1546[] = {gem_v__match_11, gem_string("value")};
        _t1547 = gem_has_key_fn(NULL, _t1546, 2);
    }
    if (gem_truthy(_t1547)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 222 "compiler/main.gem"
    GemVal _t1548[] = {gem_v_defined, gem_v_name};
    GemVal _t1550;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1548, 2)))) {
        _t1550 = gem_not(gem_fn_set_contains(NULL, _t1548, 2));
    } else {
        GemVal _t1549[] = {(*gem_v_builtins), gem_v_name};
        _t1550 = gem_not(gem_fn_set_contains(NULL, _t1549, 2));
    }
    GemVal _t1552;
    if (!gem_truthy(_t1550)) {
        _t1552 = _t1550;
    } else {
        GemVal _t1551[] = {(*gem_v_defined_fns), gem_v_name};
        _t1552 = gem_not(gem_fn_set_contains(NULL, _t1551, 2));
    }
        if (gem_truthy(_t1552)) {
#line 223 "compiler/main.gem"
    GemVal _t1553[] = {gem_v_free, gem_v_name};
            (void)(gem_fn_set_add(NULL, _t1553, 2));
        }
#line 225 "compiler/main.gem"
    GemVal _t1554[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1555 = (*gem_v_collect_free_node);
        GemVal _t1556 = _t1555.fn(_t1555.env, _t1554, 3);
        gem_pop_frame();
        return _t1556;
    } else {
    GemVal _t1557[] = {gem_v__match_11};
    GemVal _t1559;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1557, 1), gem_string("table")))) {
        _t1559 = gem_eq(gem_type_fn(NULL, _t1557, 1), gem_string("table"));
    } else {
        GemVal _t1558[] = {gem_v__match_11, gem_string("tag")};
        _t1559 = gem_has_key_fn(NULL, _t1558, 2);
    }
    GemVal _t1560;
    if (!gem_truthy(_t1559)) {
        _t1560 = _t1559;
    } else {
        _t1560 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1562;
    if (!gem_truthy(_t1560)) {
        _t1562 = _t1560;
    } else {
        GemVal _t1561[] = {gem_v__match_11, gem_string("params")};
        _t1562 = gem_has_key_fn(NULL, _t1561, 2);
    }
    GemVal _t1564;
    if (!gem_truthy(_t1562)) {
        _t1564 = _t1562;
    } else {
        GemVal _t1563[] = {gem_v__match_11, gem_string("rest_param")};
        _t1564 = gem_has_key_fn(NULL, _t1563, 2);
    }
    GemVal _t1566;
    if (!gem_truthy(_t1564)) {
        _t1566 = _t1564;
    } else {
        GemVal _t1565[] = {gem_v__match_11, gem_string("block_param")};
        _t1566 = gem_has_key_fn(NULL, _t1565, 2);
    }
    GemVal _t1568;
    if (!gem_truthy(_t1566)) {
        _t1568 = _t1566;
    } else {
        GemVal _t1567[] = {gem_v__match_11, gem_string("body")};
        _t1568 = gem_has_key_fn(NULL, _t1567, 2);
    }
    if (gem_truthy(_t1568)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_11, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_11, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_11, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 227 "compiler/main.gem"
    GemVal _t1569[] = {gem_v_params};
    GemVal _t1570[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t1569, 1)};
        GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t1570, 2);
#line 228 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 229 "compiler/main.gem"
    GemVal _t1571[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn_set_add(NULL, _t1571, 2));
        }
#line 231 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 232 "compiler/main.gem"
    GemVal _t1572[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn_set_add(NULL, _t1572, 2));
        }
#line 234 "compiler/main.gem"
    GemVal _t1573[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1574 = (*gem_v_collect_free_vars);
        GemVal gem_v_inner_free = _t1574.fn(_t1574.env, _t1573, 2);
        {
#line 235 "compiler/main.gem"
            GemVal gem_v__for_tbl_12 = gem_v_inner_free;
#line 235 "compiler/main.gem"
    GemVal _t1575[] = {gem_v__for_tbl_12};
            GemVal gem_v__for_keys_12 = gem_keys_fn(NULL, _t1575, 1);
#line 235 "compiler/main.gem"
            GemVal gem_v__for_i_12 = gem_int(0);
#line 235 "compiler/main.gem"
            while (1) {
                GemVal _t1576[] = {gem_v__for_keys_12};
                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t1576, 1)))) break;
#line 235 "compiler/main.gem"
                GemVal gem_v_ifk = gem_table_get(gem_v__for_keys_12, gem_v__for_i_12);
#line 235 "compiler/main.gem"
                GemVal gem_v__ = gem_table_get(gem_v__for_tbl_12, gem_table_get(gem_v__for_keys_12, gem_v__for_i_12));
#line 235 "compiler/main.gem"
                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 236 "compiler/main.gem"
    GemVal _t1577[] = {gem_v_free, gem_v_ifk};
                (void)(gem_fn_set_add(NULL, _t1577, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1578[] = {gem_v__match_11};
    GemVal _t1580;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1578, 1), gem_string("table")))) {
        _t1580 = gem_eq(gem_type_fn(NULL, _t1578, 1), gem_string("table"));
    } else {
        GemVal _t1579[] = {gem_v__match_11, gem_string("tag")};
        _t1580 = gem_has_key_fn(NULL, _t1579, 2);
    }
    GemVal _t1581;
    if (!gem_truthy(_t1580)) {
        _t1581 = _t1580;
    } else {
        _t1581 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1581)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1582[] = {gem_v__match_11};
    GemVal _t1584;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1582, 1), gem_string("table")))) {
        _t1584 = gem_eq(gem_type_fn(NULL, _t1582, 1), gem_string("table"));
    } else {
        GemVal _t1583[] = {gem_v__match_11, gem_string("tag")};
        _t1584 = gem_has_key_fn(NULL, _t1583, 2);
    }
    GemVal _t1585;
    if (!gem_truthy(_t1584)) {
        _t1585 = _t1584;
    } else {
        _t1585 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1587;
    if (!gem_truthy(_t1585)) {
        _t1587 = _t1585;
    } else {
        GemVal _t1586[] = {gem_v__match_11, gem_string("cond")};
        _t1587 = gem_has_key_fn(NULL, _t1586, 2);
    }
    GemVal _t1589;
    if (!gem_truthy(_t1587)) {
        _t1589 = _t1587;
    } else {
        GemVal _t1588[] = {gem_v__match_11, gem_string("then")};
        _t1589 = gem_has_key_fn(NULL, _t1588, 2);
    }
    GemVal _t1591;
    if (!gem_truthy(_t1589)) {
        _t1591 = _t1589;
    } else {
        GemVal _t1590[] = {gem_v__match_11, gem_string("else")};
        _t1591 = gem_has_key_fn(NULL, _t1590, 2);
    }
    if (gem_truthy(_t1591)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_11, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 241 "compiler/main.gem"
    GemVal _t1592[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1593 = (*gem_v_collect_free_node);
        (void)(_t1593.fn(_t1593.env, _t1592, 3));
#line 242 "compiler/main.gem"
    GemVal _t1594 = gem_table_new();
    GemVal _t1595[] = {gem_v_defined, _t1594};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1595, 2);
#line 243 "compiler/main.gem"
        GemVal gem_v__for_i_13 = gem_int(0);
#line 243 "compiler/main.gem"
    GemVal _t1596[] = {gem_v_thens};
        GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1596, 1);
#line 243 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 243 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_13;
#line 243 "compiler/main.gem"
            gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 244 "compiler/main.gem"
    GemVal _t1597[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1598 = (*gem_v_collect_free_node);
            (void)(_t1598.fn(_t1598.env, _t1597, 3));
#line 245 "compiler/main.gem"
    GemVal _t1599[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t1601;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1599, 1), gem_string("table")))) {
        _t1601 = gem_eq(gem_type_fn(NULL, _t1599, 1), gem_string("table"));
    } else {
        GemVal _t1600 = gem_table_get(gem_v_thens, gem_v_i);
        _t1601 = gem_eq(gem_table_get(_t1600, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1601)) {
#line 246 "compiler/main.gem"
    GemVal _t1602 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1603[] = {gem_v_d, gem_table_get(_t1602, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1603, 2));
            }
        }

#line 249 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 250 "compiler/main.gem"
    GemVal _t1604 = gem_table_new();
    GemVal _t1605[] = {gem_v_defined, _t1604};
            gem_v_d = gem_fn_set_union(NULL, _t1605, 2);
            {
#line 251 "compiler/main.gem"
                GemVal gem_v__for_i_14 = gem_int(0);
#line 251 "compiler/main.gem"
    GemVal _t1606[] = {gem_v_el};
                GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1606, 1);
#line 251 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 251 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_14;
#line 251 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 252 "compiler/main.gem"
    GemVal _t1607[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1608 = (*gem_v_collect_free_node);
                    (void)(_t1608.fn(_t1608.env, _t1607, 3));
#line 253 "compiler/main.gem"
    GemVal _t1609[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t1611;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1609, 1), gem_string("table")))) {
        _t1611 = gem_eq(gem_type_fn(NULL, _t1609, 1), gem_string("table"));
    } else {
        GemVal _t1610 = gem_table_get(gem_v_el, gem_v_i);
        _t1611 = gem_eq(gem_table_get(_t1610, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1611)) {
#line 254 "compiler/main.gem"
    GemVal _t1612 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1613[] = {gem_v_d, gem_table_get(_t1612, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1613, 2));
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
    GemVal _t1614[] = {gem_v__match_11};
    GemVal _t1616;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1614, 1), gem_string("table")))) {
        _t1616 = gem_eq(gem_type_fn(NULL, _t1614, 1), gem_string("table"));
    } else {
        GemVal _t1615[] = {gem_v__match_11, gem_string("tag")};
        _t1616 = gem_has_key_fn(NULL, _t1615, 2);
    }
    GemVal _t1617;
    if (!gem_truthy(_t1616)) {
        _t1617 = _t1616;
    } else {
        _t1617 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1619;
    if (!gem_truthy(_t1617)) {
        _t1619 = _t1617;
    } else {
        GemVal _t1618[] = {gem_v__match_11, gem_string("cond")};
        _t1619 = gem_has_key_fn(NULL, _t1618, 2);
    }
    GemVal _t1621;
    if (!gem_truthy(_t1619)) {
        _t1621 = _t1619;
    } else {
        GemVal _t1620[] = {gem_v__match_11, gem_string("body")};
        _t1621 = gem_has_key_fn(NULL, _t1620, 2);
    }
    if (gem_truthy(_t1621)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 259 "compiler/main.gem"
    GemVal _t1622[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1623 = (*gem_v_collect_free_node);
        (void)(_t1623.fn(_t1623.env, _t1622, 3));
#line 260 "compiler/main.gem"
    GemVal _t1624 = gem_table_new();
    GemVal _t1625[] = {gem_v_defined, _t1624};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1625, 2);
        {
#line 261 "compiler/main.gem"
            GemVal gem_v__for_i_15 = gem_int(0);
#line 261 "compiler/main.gem"
    GemVal _t1626[] = {gem_v_body};
            GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1626, 1);
#line 261 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 261 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_15;
#line 261 "compiler/main.gem"
                gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 262 "compiler/main.gem"
    GemVal _t1627[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1628 = (*gem_v_collect_free_node);
                (void)(_t1628.fn(_t1628.env, _t1627, 3));
#line 263 "compiler/main.gem"
    GemVal _t1629[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1631;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1629, 1), gem_string("table")))) {
        _t1631 = gem_eq(gem_type_fn(NULL, _t1629, 1), gem_string("table"));
    } else {
        GemVal _t1630 = gem_table_get(gem_v_body, gem_v_i);
        _t1631 = gem_eq(gem_table_get(_t1630, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1631)) {
#line 264 "compiler/main.gem"
    GemVal _t1632 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1633[] = {gem_v_d, gem_table_get(_t1632, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1633, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1634[] = {gem_v__match_11};
    GemVal _t1636;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1634, 1), gem_string("table")))) {
        _t1636 = gem_eq(gem_type_fn(NULL, _t1634, 1), gem_string("table"));
    } else {
        GemVal _t1635[] = {gem_v__match_11, gem_string("tag")};
        _t1636 = gem_has_key_fn(NULL, _t1635, 2);
    }
    GemVal _t1637;
    if (!gem_truthy(_t1636)) {
        _t1637 = _t1636;
    } else {
        _t1637 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("match"));
    }
    GemVal _t1639;
    if (!gem_truthy(_t1637)) {
        _t1639 = _t1637;
    } else {
        GemVal _t1638[] = {gem_v__match_11, gem_string("target")};
        _t1639 = gem_has_key_fn(NULL, _t1638, 2);
    }
    GemVal _t1641;
    if (!gem_truthy(_t1639)) {
        _t1641 = _t1639;
    } else {
        GemVal _t1640[] = {gem_v__match_11, gem_string("target_var")};
        _t1641 = gem_has_key_fn(NULL, _t1640, 2);
    }
    GemVal _t1643;
    if (!gem_truthy(_t1641)) {
        _t1643 = _t1641;
    } else {
        GemVal _t1642[] = {gem_v__match_11, gem_string("whens")};
        _t1643 = gem_has_key_fn(NULL, _t1642, 2);
    }
    GemVal _t1645;
    if (!gem_truthy(_t1643)) {
        _t1645 = _t1643;
    } else {
        GemVal _t1644[] = {gem_v__match_11, gem_string("else")};
        _t1645 = gem_has_key_fn(NULL, _t1644, 2);
    }
    if (gem_truthy(_t1645)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_11, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_target_var = gem_table_get(gem_v__match_11, gem_string("target_var"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_11, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 268 "compiler/main.gem"
    GemVal _t1646[] = {gem_v_target, gem_v_defined, gem_v_free};
    GemVal _t1647 = (*gem_v_collect_free_node);
        (void)(_t1647.fn(_t1647.env, _t1646, 3));
#line 269 "compiler/main.gem"
    GemVal _t1648 = gem_table_new();
    GemVal _t1649[] = {gem_v_defined, _t1648};
        GemVal gem_v_match_def = gem_fn_set_union(NULL, _t1649, 2);
#line 270 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_target_var, GEM_NIL))) {
#line 271 "compiler/main.gem"
    GemVal _t1650[] = {gem_v_match_def, gem_v_target_var};
            (void)(gem_fn_set_add(NULL, _t1650, 2));
        }
#line 273 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 273 "compiler/main.gem"
    GemVal _t1651[] = {gem_v_whens};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1651, 1);
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
    GemVal _t1652 = gem_v_w;
    GemVal _t1653[] = {gem_table_get(_t1652, gem_string("value")), gem_v_match_def, gem_v_free};
    GemVal _t1654 = (*gem_v_collect_free_node);
            (void)(_t1654.fn(_t1654.env, _t1653, 3));
#line 276 "compiler/main.gem"
    GemVal _t1655 = gem_table_new();
    GemVal _t1656[] = {gem_v_match_def, _t1655};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1656, 2);
#line 277 "compiler/main.gem"
    GemVal _t1657 = gem_v_w;
            if (gem_truthy(gem_neq(gem_table_get(_t1657, gem_string("bindings")), GEM_NIL))) {
#line 278 "compiler/main.gem"
                GemVal gem_v__for_i_16 = gem_int(0);
#line 278 "compiler/main.gem"
    GemVal _t1658 = gem_v_w;
    GemVal _t1659[] = {gem_table_get(_t1658, gem_string("bindings"))};
                GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1659, 1);
#line 278 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 278 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_16;
#line 278 "compiler/main.gem"
                    gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 279 "compiler/main.gem"
    GemVal _t1660 = gem_v_w;
    GemVal _t1661[] = {gem_table_get(gem_table_get(_t1660, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1662 = (*gem_v_collect_free_node);
                    (void)(_t1662.fn(_t1662.env, _t1661, 3));
#line 280 "compiler/main.gem"
    GemVal _t1663 = gem_v_w;
    GemVal _t1664[] = {gem_table_get(gem_table_get(_t1663, gem_string("bindings")), gem_v_bi)};
    GemVal _t1667;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1664, 1), gem_string("table")))) {
        _t1667 = gem_eq(gem_type_fn(NULL, _t1664, 1), gem_string("table"));
    } else {
        GemVal _t1665 = gem_v_w;
        GemVal _t1666 = gem_table_get(gem_table_get(_t1665, gem_string("bindings")), gem_v_bi);
        _t1667 = gem_eq(gem_table_get(_t1666, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1667)) {
#line 281 "compiler/main.gem"
    GemVal _t1668 = gem_v_w;
    GemVal _t1669 = gem_table_get(gem_table_get(_t1668, gem_string("bindings")), gem_v_bi);
    GemVal _t1670[] = {gem_v_d, gem_table_get(_t1669, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1670, 2));
                    }
                }

            }
#line 285 "compiler/main.gem"
            GemVal gem_v__for_i_17 = gem_int(0);
#line 285 "compiler/main.gem"
    GemVal _t1671 = gem_v_w;
    GemVal _t1672[] = {gem_table_get(_t1671, gem_string("body"))};
            GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1672, 1);
#line 285 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 285 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_17;
#line 285 "compiler/main.gem"
                gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 286 "compiler/main.gem"
    GemVal _t1673 = gem_v_w;
    GemVal _t1674[] = {gem_table_get(gem_table_get(_t1673, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1675 = (*gem_v_collect_free_node);
                (void)(_t1675.fn(_t1675.env, _t1674, 3));
#line 287 "compiler/main.gem"
    GemVal _t1676 = gem_v_w;
    GemVal _t1677[] = {gem_table_get(gem_table_get(_t1676, gem_string("body")), gem_v_j)};
    GemVal _t1680;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1677, 1), gem_string("table")))) {
        _t1680 = gem_eq(gem_type_fn(NULL, _t1677, 1), gem_string("table"));
    } else {
        GemVal _t1678 = gem_v_w;
        GemVal _t1679 = gem_table_get(gem_table_get(_t1678, gem_string("body")), gem_v_j);
        _t1680 = gem_eq(gem_table_get(_t1679, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1680)) {
#line 288 "compiler/main.gem"
    GemVal _t1681 = gem_v_w;
    GemVal _t1682 = gem_table_get(gem_table_get(_t1681, gem_string("body")), gem_v_j);
    GemVal _t1683[] = {gem_v_d, gem_table_get(_t1682, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1683, 2));
                }
            }

        }

#line 292 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 293 "compiler/main.gem"
    GemVal _t1684 = gem_table_new();
    GemVal _t1685[] = {gem_v_defined, _t1684};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1685, 2);
            {
#line 294 "compiler/main.gem"
                GemVal gem_v__for_i_19 = gem_int(0);
#line 294 "compiler/main.gem"
    GemVal _t1686[] = {gem_v_el};
                GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1686, 1);
#line 294 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 294 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_19;
#line 294 "compiler/main.gem"
                    gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 295 "compiler/main.gem"
    GemVal _t1687[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1688 = (*gem_v_collect_free_node);
                    (void)(_t1688.fn(_t1688.env, _t1687, 3));
#line 296 "compiler/main.gem"
    GemVal _t1689[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t1691;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1689, 1), gem_string("table")))) {
        _t1691 = gem_eq(gem_type_fn(NULL, _t1689, 1), gem_string("table"));
    } else {
        GemVal _t1690 = gem_table_get(gem_v_el, gem_v_j);
        _t1691 = gem_eq(gem_table_get(_t1690, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1691)) {
#line 297 "compiler/main.gem"
    GemVal _t1692 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t1693[] = {gem_v_d, gem_table_get(_t1692, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1693, 2));
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
    GemVal _t1694[] = {gem_v__match_11};
    GemVal _t1696;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1694, 1), gem_string("table")))) {
        _t1696 = gem_eq(gem_type_fn(NULL, _t1694, 1), gem_string("table"));
    } else {
        GemVal _t1695[] = {gem_v__match_11, gem_string("tag")};
        _t1696 = gem_has_key_fn(NULL, _t1695, 2);
    }
    GemVal _t1697;
    if (!gem_truthy(_t1696)) {
        _t1697 = _t1696;
    } else {
        _t1697 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("when_clause"));
    }
    GemVal _t1699;
    if (!gem_truthy(_t1697)) {
        _t1699 = _t1697;
    } else {
        GemVal _t1698[] = {gem_v__match_11, gem_string("value")};
        _t1699 = gem_has_key_fn(NULL, _t1698, 2);
    }
    GemVal _t1701;
    if (!gem_truthy(_t1699)) {
        _t1701 = _t1699;
    } else {
        GemVal _t1700[] = {gem_v__match_11, gem_string("bindings")};
        _t1701 = gem_has_key_fn(NULL, _t1700, 2);
    }
    GemVal _t1703;
    if (!gem_truthy(_t1701)) {
        _t1703 = _t1701;
    } else {
        GemVal _t1702[] = {gem_v__match_11, gem_string("body")};
        _t1703 = gem_has_key_fn(NULL, _t1702, 2);
    }
    if (gem_truthy(_t1703)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_bindings = gem_table_get(gem_v__match_11, gem_string("bindings"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 302 "compiler/main.gem"
    GemVal _t1704[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1705 = (*gem_v_collect_free_node);
        (void)(_t1705.fn(_t1705.env, _t1704, 3));
#line 303 "compiler/main.gem"
    GemVal _t1706 = gem_table_new();
    GemVal _t1707[] = {gem_v_defined, _t1706};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1707, 2);
#line 304 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_bindings, GEM_NIL))) {
#line 305 "compiler/main.gem"
            GemVal gem_v__for_i_20 = gem_int(0);
#line 305 "compiler/main.gem"
    GemVal _t1708[] = {gem_v_bindings};
            GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1708, 1);
#line 305 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 305 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_20;
#line 305 "compiler/main.gem"
                gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 306 "compiler/main.gem"
    GemVal _t1709[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1710 = (*gem_v_collect_free_node);
                (void)(_t1710.fn(_t1710.env, _t1709, 3));
#line 307 "compiler/main.gem"
    GemVal _t1711[] = {gem_table_get(gem_v_bindings, gem_v_bi)};
    GemVal _t1713;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1711, 1), gem_string("table")))) {
        _t1713 = gem_eq(gem_type_fn(NULL, _t1711, 1), gem_string("table"));
    } else {
        GemVal _t1712 = gem_table_get(gem_v_bindings, gem_v_bi);
        _t1713 = gem_eq(gem_table_get(_t1712, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1713)) {
#line 308 "compiler/main.gem"
    GemVal _t1714 = gem_table_get(gem_v_bindings, gem_v_bi);
    GemVal _t1715[] = {gem_v_d, gem_table_get(_t1714, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1715, 2));
                }
            }

        }
        {
#line 312 "compiler/main.gem"
            GemVal gem_v__for_i_21 = gem_int(0);
#line 312 "compiler/main.gem"
    GemVal _t1716[] = {gem_v_body};
            GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t1716, 1);
#line 312 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 312 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_21;
#line 312 "compiler/main.gem"
                gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 313 "compiler/main.gem"
    GemVal _t1717[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1718 = (*gem_v_collect_free_node);
                (void)(_t1718.fn(_t1718.env, _t1717, 3));
#line 314 "compiler/main.gem"
    GemVal _t1719[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1721;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1719, 1), gem_string("table")))) {
        _t1721 = gem_eq(gem_type_fn(NULL, _t1719, 1), gem_string("table"));
    } else {
        GemVal _t1720 = gem_table_get(gem_v_body, gem_v_i);
        _t1721 = gem_eq(gem_table_get(_t1720, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1721)) {
#line 315 "compiler/main.gem"
    GemVal _t1722 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1723[] = {gem_v_d, gem_table_get(_t1722, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1723, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1724[] = {gem_v__match_11};
    GemVal _t1726;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1724, 1), gem_string("table")))) {
        _t1726 = gem_eq(gem_type_fn(NULL, _t1724, 1), gem_string("table"));
    } else {
        GemVal _t1725[] = {gem_v__match_11, gem_string("tag")};
        _t1726 = gem_has_key_fn(NULL, _t1725, 2);
    }
    GemVal _t1727;
    if (!gem_truthy(_t1726)) {
        _t1727 = _t1726;
    } else {
        _t1727 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t1729;
    if (!gem_truthy(_t1727)) {
        _t1729 = _t1727;
    } else {
        GemVal _t1728[] = {gem_v__match_11, gem_string("arms")};
        _t1729 = gem_has_key_fn(NULL, _t1728, 2);
    }
    GemVal _t1731;
    if (!gem_truthy(_t1729)) {
        _t1731 = _t1729;
    } else {
        GemVal _t1730[] = {gem_v__match_11, gem_string("after_ms")};
        _t1731 = gem_has_key_fn(NULL, _t1730, 2);
    }
    GemVal _t1733;
    if (!gem_truthy(_t1731)) {
        _t1733 = _t1731;
    } else {
        GemVal _t1732[] = {gem_v__match_11, gem_string("after_body")};
        _t1733 = gem_has_key_fn(NULL, _t1732, 2);
    }
    if (gem_truthy(_t1733)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_11, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_11, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_11, gem_string("after_body"));
#line 319 "compiler/main.gem"
    GemVal _t1734 = gem_table_new();
    GemVal _t1735[] = {gem_v_defined, _t1734};
        GemVal gem_v_recv_def = gem_fn_set_union(NULL, _t1735, 2);
#line 320 "compiler/main.gem"
    GemVal _t1736 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t1736, gem_string("recv_var")), GEM_NIL))) {
#line 321 "compiler/main.gem"
    GemVal _t1737 = gem_v_node;
    GemVal _t1738[] = {gem_v_recv_def, gem_table_get(_t1737, gem_string("recv_var"))};
            (void)(gem_fn_set_add(NULL, _t1738, 2));
        }
#line 323 "compiler/main.gem"
        GemVal gem_v__for_i_24 = gem_int(0);
#line 323 "compiler/main.gem"
    GemVal _t1739[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1739, 1);
#line 323 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 323 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_24;
#line 323 "compiler/main.gem"
            gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 324 "compiler/main.gem"
            GemVal gem_v_arm = gem_table_get(gem_v_rm_arms, gem_v_i);
#line 325 "compiler/main.gem"
    GemVal _t1740 = gem_v_arm;
    GemVal _t1741 = gem_table_get(_t1740, gem_string("pattern"));
    GemVal _t1742[] = {gem_table_get(_t1741, gem_string("condition")), gem_v_recv_def, gem_v_free};
    GemVal _t1743 = (*gem_v_collect_free_node);
            (void)(_t1743.fn(_t1743.env, _t1742, 3));
#line 326 "compiler/main.gem"
    GemVal _t1744 = gem_table_new();
    GemVal _t1745[] = {gem_v_recv_def, _t1744};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1745, 2);
#line 327 "compiler/main.gem"
    GemVal _t1746 = gem_v_arm;
    GemVal _t1747 = gem_table_get(_t1746, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t1747, gem_string("bindings")), GEM_NIL))) {
#line 328 "compiler/main.gem"
                GemVal gem_v__for_i_22 = gem_int(0);
#line 328 "compiler/main.gem"
    GemVal _t1748 = gem_v_arm;
    GemVal _t1749 = gem_table_get(_t1748, gem_string("pattern"));
    GemVal _t1750[] = {gem_table_get(_t1749, gem_string("bindings"))};
                GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1750, 1);
#line 328 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 328 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_22;
#line 328 "compiler/main.gem"
                    gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 329 "compiler/main.gem"
    GemVal _t1751 = gem_v_arm;
    GemVal _t1752 = gem_table_get(_t1751, gem_string("pattern"));
    GemVal _t1753[] = {gem_table_get(gem_table_get(_t1752, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1754 = (*gem_v_collect_free_node);
                    (void)(_t1754.fn(_t1754.env, _t1753, 3));
#line 330 "compiler/main.gem"
    GemVal _t1755 = gem_v_arm;
    GemVal _t1756 = gem_table_get(_t1755, gem_string("pattern"));
    GemVal _t1757[] = {gem_table_get(gem_table_get(_t1756, gem_string("bindings")), gem_v_bi)};
    GemVal _t1761;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1757, 1), gem_string("table")))) {
        _t1761 = gem_eq(gem_type_fn(NULL, _t1757, 1), gem_string("table"));
    } else {
        GemVal _t1758 = gem_v_arm;
        GemVal _t1759 = gem_table_get(_t1758, gem_string("pattern"));
        GemVal _t1760 = gem_table_get(gem_table_get(_t1759, gem_string("bindings")), gem_v_bi);
        _t1761 = gem_eq(gem_table_get(_t1760, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1761)) {
#line 331 "compiler/main.gem"
    GemVal _t1762 = gem_v_arm;
    GemVal _t1763 = gem_table_get(_t1762, gem_string("pattern"));
    GemVal _t1764 = gem_table_get(gem_table_get(_t1763, gem_string("bindings")), gem_v_bi);
    GemVal _t1765[] = {gem_v_d, gem_table_get(_t1764, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1765, 2));
                    }
                }

            }
#line 335 "compiler/main.gem"
            GemVal gem_v__for_i_23 = gem_int(0);
#line 335 "compiler/main.gem"
    GemVal _t1766 = gem_v_arm;
    GemVal _t1767[] = {gem_table_get(_t1766, gem_string("body"))};
            GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t1767, 1);
#line 335 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 335 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_23;
#line 335 "compiler/main.gem"
                gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 336 "compiler/main.gem"
    GemVal _t1768 = gem_v_arm;
    GemVal _t1769[] = {gem_table_get(gem_table_get(_t1768, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1770 = (*gem_v_collect_free_node);
                (void)(_t1770.fn(_t1770.env, _t1769, 3));
#line 337 "compiler/main.gem"
    GemVal _t1771 = gem_v_arm;
    GemVal _t1772[] = {gem_table_get(gem_table_get(_t1771, gem_string("body")), gem_v_j)};
    GemVal _t1775;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1772, 1), gem_string("table")))) {
        _t1775 = gem_eq(gem_type_fn(NULL, _t1772, 1), gem_string("table"));
    } else {
        GemVal _t1773 = gem_v_arm;
        GemVal _t1774 = gem_table_get(gem_table_get(_t1773, gem_string("body")), gem_v_j);
        _t1775 = gem_eq(gem_table_get(_t1774, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1775)) {
#line 338 "compiler/main.gem"
    GemVal _t1776 = gem_v_arm;
    GemVal _t1777 = gem_table_get(gem_table_get(_t1776, gem_string("body")), gem_v_j);
    GemVal _t1778[] = {gem_v_d, gem_table_get(_t1777, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1778, 2));
                }
            }

        }

#line 342 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 343 "compiler/main.gem"
    GemVal _t1779[] = {gem_v_rm_after_ms, gem_v_recv_def, gem_v_free};
    GemVal _t1780 = (*gem_v_collect_free_node);
            (void)(_t1780.fn(_t1780.env, _t1779, 3));
        }
#line 345 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 346 "compiler/main.gem"
    GemVal _t1781 = gem_table_new();
    GemVal _t1782[] = {gem_v_recv_def, _t1781};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1782, 2);
            {
#line 347 "compiler/main.gem"
                GemVal gem_v__for_i_25 = gem_int(0);
#line 347 "compiler/main.gem"
    GemVal _t1783[] = {gem_v_rm_after_body};
                GemVal gem_v__for_limit_25 = gem_len_fn(NULL, _t1783, 1);
#line 347 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 347 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_25;
#line 347 "compiler/main.gem"
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 348 "compiler/main.gem"
    GemVal _t1784[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1785 = (*gem_v_collect_free_node);
                    (void)(_t1785.fn(_t1785.env, _t1784, 3));
#line 349 "compiler/main.gem"
    GemVal _t1786[] = {gem_table_get(gem_v_rm_after_body, gem_v_j)};
    GemVal _t1788;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1786, 1), gem_string("table")))) {
        _t1788 = gem_eq(gem_type_fn(NULL, _t1786, 1), gem_string("table"));
    } else {
        GemVal _t1787 = gem_table_get(gem_v_rm_after_body, gem_v_j);
        _t1788 = gem_eq(gem_table_get(_t1787, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1788)) {
#line 350 "compiler/main.gem"
    GemVal _t1789 = gem_table_get(gem_v_rm_after_body, gem_v_j);
    GemVal _t1790[] = {gem_v_d, gem_table_get(_t1789, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1790, 2));
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
    GemVal _t1791[] = {gem_v__match_11};
    GemVal _t1793;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1791, 1), gem_string("table")))) {
        _t1793 = gem_eq(gem_type_fn(NULL, _t1791, 1), gem_string("table"));
    } else {
        GemVal _t1792[] = {gem_v__match_11, gem_string("tag")};
        _t1793 = gem_has_key_fn(NULL, _t1792, 2);
    }
    GemVal _t1794;
    if (!gem_truthy(_t1793)) {
        _t1794 = _t1793;
    } else {
        _t1794 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("block"));
    }
    GemVal _t1796;
    if (!gem_truthy(_t1794)) {
        _t1796 = _t1794;
    } else {
        GemVal _t1795[] = {gem_v__match_11, gem_string("stmts")};
        _t1796 = gem_has_key_fn(NULL, _t1795, 2);
    }
    if (gem_truthy(_t1796)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_11, gem_string("stmts"));
#line 355 "compiler/main.gem"
    GemVal _t1797 = gem_table_new();
    GemVal _t1798[] = {gem_v_defined, _t1797};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1798, 2);
        {
#line 356 "compiler/main.gem"
            GemVal gem_v__for_i_26 = gem_int(0);
#line 356 "compiler/main.gem"
    GemVal _t1799[] = {gem_v_stmts};
            GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1799, 1);
#line 356 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 356 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_26;
#line 356 "compiler/main.gem"
                gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 357 "compiler/main.gem"
    GemVal _t1800[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1801 = (*gem_v_collect_free_node);
                (void)(_t1801.fn(_t1801.env, _t1800, 3));
#line 358 "compiler/main.gem"
    GemVal _t1802[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1804;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1802, 1), gem_string("table")))) {
        _t1804 = gem_eq(gem_type_fn(NULL, _t1802, 1), gem_string("table"));
    } else {
        GemVal _t1803 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1804 = gem_eq(gem_table_get(_t1803, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1804)) {
#line 359 "compiler/main.gem"
    GemVal _t1805 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1806[] = {gem_v_d, gem_table_get(_t1805, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1806, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1807[] = {gem_v__match_11};
    GemVal _t1809;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1807, 1), gem_string("table")))) {
        _t1809 = gem_eq(gem_type_fn(NULL, _t1807, 1), gem_string("table"));
    } else {
        GemVal _t1808[] = {gem_v__match_11, gem_string("tag")};
        _t1809 = gem_has_key_fn(NULL, _t1808, 2);
    }
    GemVal _t1810;
    if (!gem_truthy(_t1809)) {
        _t1810 = _t1809;
    } else {
        _t1810 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("return"));
    }
    GemVal _t1812;
    if (!gem_truthy(_t1810)) {
        _t1812 = _t1810;
    } else {
        GemVal _t1811[] = {gem_v__match_11, gem_string("value")};
        _t1812 = gem_has_key_fn(NULL, _t1811, 2);
    }
    if (gem_truthy(_t1812)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 363 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 364 "compiler/main.gem"
    GemVal _t1813[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1814 = (*gem_v_collect_free_node);
            GemVal _t1815 = _t1814.fn(_t1814.env, _t1813, 3);
            gem_pop_frame();
            return _t1815;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
        {
#line 368 "compiler/main.gem"
            GemVal gem_v__for_tbl_27 = gem_v_node;
#line 368 "compiler/main.gem"
    GemVal _t1816[] = {gem_v__for_tbl_27};
            GemVal gem_v__for_keys_27 = gem_keys_fn(NULL, _t1816, 1);
#line 368 "compiler/main.gem"
            GemVal gem_v__for_i_27 = gem_int(0);
#line 368 "compiler/main.gem"
            while (1) {
                GemVal _t1817[] = {gem_v__for_keys_27};
                if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_len_fn(NULL, _t1817, 1)))) break;
#line 368 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_27, gem_v__for_i_27);
#line 368 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_27, gem_table_get(gem_v__for_keys_27, gem_v__for_i_27));
#line 368 "compiler/main.gem"
                gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 369 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 370 "compiler/main.gem"
    GemVal _t1818[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1818, 1), gem_string("table")))) {
#line 371 "compiler/main.gem"
    GemVal _t1819[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t1819, 1);
#line 372 "compiler/main.gem"
    GemVal _t1820[] = {gem_v_vks};
    GemVal _t1822;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1820, 1), gem_int(0)))) {
        _t1822 = gem_gt(gem_len_fn(NULL, _t1820, 1), gem_int(0));
    } else {
        GemVal _t1821[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1822 = gem_eq(gem_type_fn(NULL, _t1821, 1), gem_string("int"));
    }
                        if (gem_truthy(_t1822)) {
#line 373 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 374 "compiler/main.gem"
                            while (1) {
                                GemVal _t1823[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1823, 1)))) break;
#line 375 "compiler/main.gem"
    GemVal _t1824[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1824, 1), gem_string("table")))) {
#line 376 "compiler/main.gem"
    GemVal _t1825[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t1826 = (*gem_v_collect_free_node);
                                    (void)(_t1826.fn(_t1826.env, _t1825, 3));
                                }
#line 378 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 381 "compiler/main.gem"
    GemVal _t1827[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t1828 = (*gem_v_collect_free_node);
                            (void)(_t1828.fn(_t1828.env, _t1827, 3));
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
#line 390 "compiler/main.gem"
    GemVal _t1830 = gem_table_new();
    GemVal gem_v_free = _t1830;
#line 391 "compiler/main.gem"
    GemVal _t1831 = gem_table_new();
    GemVal _t1832[] = {gem_v_defined, _t1831};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t1832, 2);
#line 392 "compiler/main.gem"
    GemVal gem_v__for_i_28 = gem_int(0);
#line 392 "compiler/main.gem"
    GemVal _t1833[] = {gem_v_stmts};
    GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1833, 1);
#line 392 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 392 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_28;
#line 392 "compiler/main.gem"
        gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 393 "compiler/main.gem"
    GemVal _t1834[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1835 = (*gem_v_collect_free_node);
        (void)(_t1835.fn(_t1835.env, _t1834, 3));
#line 394 "compiler/main.gem"
    GemVal _t1836[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1838;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1836, 1), gem_string("table")))) {
        _t1838 = gem_eq(gem_type_fn(NULL, _t1836, 1), gem_string("table"));
    } else {
        GemVal _t1837 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1838 = gem_eq(gem_table_get(_t1837, gem_string("tag")), gem_string("let"));
    }
        if (gem_truthy(_t1838)) {
#line 395 "compiler/main.gem"
    GemVal _t1839 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1840[] = {gem_v_d, gem_table_get(_t1839, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1840, 2));
        }
#line 397 "compiler/main.gem"
    GemVal _t1841[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d};
        (void)(gem_fn_collect_block_lets(NULL, _t1841, 2));
    }

    GemVal _t1842 = gem_v_free;
    gem_pop_frame();
    return _t1842;
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
#line 408 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 409 "compiler/main.gem"
        GemVal _t1844 = GEM_NIL;
        gem_pop_frame();
        return _t1844;
    }
#line 411 "compiler/main.gem"
    GemVal _t1845[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1845, 1), gem_string("table")))) {
#line 412 "compiler/main.gem"
        GemVal _t1846 = GEM_NIL;
        gem_pop_frame();
        return _t1846;
    }
#line 414 "compiler/main.gem"
    GemVal _t1847 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1847, gem_string("tag")), GEM_NIL))) {
#line 415 "compiler/main.gem"
        GemVal _t1848 = GEM_NIL;
        gem_pop_frame();
        return _t1848;
    }
#line 418 "compiler/main.gem"
    GemVal gem_v__match_29 = gem_v_node;
    GemVal _t1849[] = {gem_v__match_29};
    GemVal _t1851;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1849, 1), gem_string("table")))) {
        _t1851 = gem_eq(gem_type_fn(NULL, _t1849, 1), gem_string("table"));
    } else {
        GemVal _t1850[] = {gem_v__match_29, gem_string("tag")};
        _t1851 = gem_has_key_fn(NULL, _t1850, 2);
    }
    GemVal _t1852;
    if (!gem_truthy(_t1851)) {
        _t1852 = _t1851;
    } else {
        _t1852 = gem_eq(gem_table_get(gem_v__match_29, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1854;
    if (!gem_truthy(_t1852)) {
        _t1854 = _t1852;
    } else {
        GemVal _t1853[] = {gem_v__match_29, gem_string("params")};
        _t1854 = gem_has_key_fn(NULL, _t1853, 2);
    }
    GemVal _t1856;
    if (!gem_truthy(_t1854)) {
        _t1856 = _t1854;
    } else {
        GemVal _t1855[] = {gem_v__match_29, gem_string("rest_param")};
        _t1856 = gem_has_key_fn(NULL, _t1855, 2);
    }
    GemVal _t1858;
    if (!gem_truthy(_t1856)) {
        _t1858 = _t1856;
    } else {
        GemVal _t1857[] = {gem_v__match_29, gem_string("block_param")};
        _t1858 = gem_has_key_fn(NULL, _t1857, 2);
    }
    GemVal _t1860;
    if (!gem_truthy(_t1858)) {
        _t1860 = _t1858;
    } else {
        GemVal _t1859[] = {gem_v__match_29, gem_string("body")};
        _t1860 = gem_has_key_fn(NULL, _t1859, 2);
    }
    if (gem_truthy(_t1860)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_29, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_29, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_29, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_29, gem_string("body"));
#line 420 "compiler/main.gem"
    GemVal _t1861[] = {gem_v_params};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1861, 1);
#line 421 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 422 "compiler/main.gem"
    GemVal _t1862[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn_set_add(NULL, _t1862, 2));
        }
#line 424 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 425 "compiler/main.gem"
    GemVal _t1863[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn_set_add(NULL, _t1863, 2));
        }
#line 427 "compiler/main.gem"
    GemVal _t1864[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1865 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t1865.fn(_t1865.env, _t1864, 2);
#line 428 "compiler/main.gem"
    GemVal _t1866[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t1866, 2);
#line 429 "compiler/main.gem"
        GemVal gem_v__for_tbl_30 = gem_v_inter;
#line 429 "compiler/main.gem"
    GemVal _t1867[] = {gem_v__for_tbl_30};
        GemVal gem_v__for_keys_30 = gem_keys_fn(NULL, _t1867, 1);
#line 429 "compiler/main.gem"
        GemVal gem_v__for_i_30 = gem_int(0);
#line 429 "compiler/main.gem"
        while (1) {
            GemVal _t1868[] = {gem_v__for_keys_30};
            if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_len_fn(NULL, _t1868, 1)))) break;
#line 429 "compiler/main.gem"
            GemVal gem_v_ik = gem_table_get(gem_v__for_keys_30, gem_v__for_i_30);
#line 429 "compiler/main.gem"
            GemVal gem_v__ = gem_table_get(gem_v__for_tbl_30, gem_table_get(gem_v__for_keys_30, gem_v__for_i_30));
#line 429 "compiler/main.gem"
            gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 430 "compiler/main.gem"
    GemVal _t1869[] = {gem_v_captured, gem_v_ik};
            (void)(gem_fn_set_add(NULL, _t1869, 2));
        }

#line 432 "compiler/main.gem"
    GemVal _t1870[] = {gem_v_scope_vars, gem_v_inner_defined};
    GemVal _t1871[] = {gem_v_body, gem_fn_set_union(NULL, _t1870, 2), gem_v_captured};
    GemVal _t1872 = (*gem_v_walk_captures);
        GemVal _t1873 = _t1872.fn(_t1872.env, _t1871, 3);
        gem_pop_frame();
        return _t1873;
    } else {
    GemVal _t1874[] = {gem_v__match_29};
    GemVal _t1876;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1874, 1), gem_string("table")))) {
        _t1876 = gem_eq(gem_type_fn(NULL, _t1874, 1), gem_string("table"));
    } else {
        GemVal _t1875[] = {gem_v__match_29, gem_string("tag")};
        _t1876 = gem_has_key_fn(NULL, _t1875, 2);
    }
    GemVal _t1877;
    if (!gem_truthy(_t1876)) {
        _t1877 = _t1876;
    } else {
        _t1877 = gem_eq(gem_table_get(gem_v__match_29, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1877)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1878[] = {gem_v__match_29};
    GemVal _t1880;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1878, 1), gem_string("table")))) {
        _t1880 = gem_eq(gem_type_fn(NULL, _t1878, 1), gem_string("table"));
    } else {
        GemVal _t1879[] = {gem_v__match_29, gem_string("tag")};
        _t1880 = gem_has_key_fn(NULL, _t1879, 2);
    }
    GemVal _t1881;
    if (!gem_truthy(_t1880)) {
        _t1881 = _t1880;
    } else {
        _t1881 = gem_eq(gem_table_get(gem_v__match_29, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1883;
    if (!gem_truthy(_t1881)) {
        _t1883 = _t1881;
    } else {
        GemVal _t1882[] = {gem_v__match_29, gem_string("cond")};
        _t1883 = gem_has_key_fn(NULL, _t1882, 2);
    }
    GemVal _t1885;
    if (!gem_truthy(_t1883)) {
        _t1885 = _t1883;
    } else {
        GemVal _t1884[] = {gem_v__match_29, gem_string("then")};
        _t1885 = gem_has_key_fn(NULL, _t1884, 2);
    }
    GemVal _t1887;
    if (!gem_truthy(_t1885)) {
        _t1887 = _t1885;
    } else {
        GemVal _t1886[] = {gem_v__match_29, gem_string("else")};
        _t1887 = gem_has_key_fn(NULL, _t1886, 2);
    }
    if (gem_truthy(_t1887)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_29, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_29, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_29, gem_string("else"));
#line 436 "compiler/main.gem"
    GemVal _t1888[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t1889 = (*gem_v_walk_captures_node);
        (void)(_t1889.fn(_t1889.env, _t1888, 3));
#line 437 "compiler/main.gem"
    GemVal _t1890[] = {gem_v_thens, gem_v_scope_vars, gem_v_captured};
    GemVal _t1891 = (*gem_v_walk_captures);
        (void)(_t1891.fn(_t1891.env, _t1890, 3));
#line 438 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 439 "compiler/main.gem"
    GemVal _t1892[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t1893 = (*gem_v_walk_captures);
            GemVal _t1894 = _t1893.fn(_t1893.env, _t1892, 3);
            gem_pop_frame();
            return _t1894;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1895[] = {gem_v__match_29};
    GemVal _t1897;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1895, 1), gem_string("table")))) {
        _t1897 = gem_eq(gem_type_fn(NULL, _t1895, 1), gem_string("table"));
    } else {
        GemVal _t1896[] = {gem_v__match_29, gem_string("tag")};
        _t1897 = gem_has_key_fn(NULL, _t1896, 2);
    }
    GemVal _t1898;
    if (!gem_truthy(_t1897)) {
        _t1898 = _t1897;
    } else {
        _t1898 = gem_eq(gem_table_get(gem_v__match_29, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1900;
    if (!gem_truthy(_t1898)) {
        _t1900 = _t1898;
    } else {
        GemVal _t1899[] = {gem_v__match_29, gem_string("cond")};
        _t1900 = gem_has_key_fn(NULL, _t1899, 2);
    }
    GemVal _t1902;
    if (!gem_truthy(_t1900)) {
        _t1902 = _t1900;
    } else {
        GemVal _t1901[] = {gem_v__match_29, gem_string("body")};
        _t1902 = gem_has_key_fn(NULL, _t1901, 2);
    }
    if (gem_truthy(_t1902)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_29, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_29, gem_string("body"));
#line 442 "compiler/main.gem"
    GemVal _t1903[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t1904 = (*gem_v_walk_captures_node);
        (void)(_t1904.fn(_t1904.env, _t1903, 3));
#line 443 "compiler/main.gem"
    GemVal _t1905[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t1906 = (*gem_v_walk_captures);
        GemVal _t1907 = _t1906.fn(_t1906.env, _t1905, 3);
        gem_pop_frame();
        return _t1907;
    } else {
    GemVal _t1908[] = {gem_v__match_29};
    GemVal _t1910;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1908, 1), gem_string("table")))) {
        _t1910 = gem_eq(gem_type_fn(NULL, _t1908, 1), gem_string("table"));
    } else {
        GemVal _t1909[] = {gem_v__match_29, gem_string("tag")};
        _t1910 = gem_has_key_fn(NULL, _t1909, 2);
    }
    GemVal _t1911;
    if (!gem_truthy(_t1910)) {
        _t1911 = _t1910;
    } else {
        _t1911 = gem_eq(gem_table_get(gem_v__match_29, gem_string("tag")), gem_string("match"));
    }
    GemVal _t1913;
    if (!gem_truthy(_t1911)) {
        _t1913 = _t1911;
    } else {
        GemVal _t1912[] = {gem_v__match_29, gem_string("target")};
        _t1913 = gem_has_key_fn(NULL, _t1912, 2);
    }
    GemVal _t1915;
    if (!gem_truthy(_t1913)) {
        _t1915 = _t1913;
    } else {
        GemVal _t1914[] = {gem_v__match_29, gem_string("whens")};
        _t1915 = gem_has_key_fn(NULL, _t1914, 2);
    }
    GemVal _t1917;
    if (!gem_truthy(_t1915)) {
        _t1917 = _t1915;
    } else {
        GemVal _t1916[] = {gem_v__match_29, gem_string("else")};
        _t1917 = gem_has_key_fn(NULL, _t1916, 2);
    }
    if (gem_truthy(_t1917)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_29, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_29, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_29, gem_string("else"));
#line 445 "compiler/main.gem"
    GemVal _t1918[] = {gem_v_target, gem_v_scope_vars, gem_v_captured};
    GemVal _t1919 = (*gem_v_walk_captures_node);
        (void)(_t1919.fn(_t1919.env, _t1918, 3));
#line 446 "compiler/main.gem"
        GemVal gem_v__for_i_32 = gem_int(0);
#line 446 "compiler/main.gem"
    GemVal _t1920[] = {gem_v_whens};
        GemVal gem_v__for_limit_32 = gem_len_fn(NULL, _t1920, 1);
#line 446 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_v__for_limit_32))) break;
#line 446 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_32;
#line 446 "compiler/main.gem"
            gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 447 "compiler/main.gem"
    GemVal _t1921 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1922[] = {gem_table_get(_t1921, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1923 = (*gem_v_walk_captures_node);
            (void)(_t1923.fn(_t1923.env, _t1922, 3));
#line 448 "compiler/main.gem"
    GemVal _t1924 = gem_table_get(gem_v_whens, gem_v_i);
            if (gem_truthy(gem_neq(gem_table_get(_t1924, gem_string("bindings")), GEM_NIL))) {
#line 449 "compiler/main.gem"
                GemVal gem_v__for_i_31 = gem_int(0);
#line 449 "compiler/main.gem"
    GemVal _t1925 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1926[] = {gem_table_get(_t1925, gem_string("bindings"))};
                GemVal gem_v__for_limit_31 = gem_len_fn(NULL, _t1926, 1);
#line 449 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_v__for_limit_31))) break;
#line 449 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_31;
#line 449 "compiler/main.gem"
                    gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
#line 450 "compiler/main.gem"
    GemVal _t1927 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1928[] = {gem_table_get(gem_table_get(_t1927, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t1929 = (*gem_v_walk_captures_node);
                    (void)(_t1929.fn(_t1929.env, _t1928, 3));
                }

            }
#line 453 "compiler/main.gem"
    GemVal _t1930 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1931[] = {gem_table_get(_t1930, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1932 = (*gem_v_walk_captures);
            (void)(_t1932.fn(_t1932.env, _t1931, 3));
        }

#line 455 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 456 "compiler/main.gem"
    GemVal _t1933[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t1934 = (*gem_v_walk_captures);
            GemVal _t1935 = _t1934.fn(_t1934.env, _t1933, 3);
            gem_pop_frame();
            return _t1935;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1936[] = {gem_v__match_29};
    GemVal _t1938;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1936, 1), gem_string("table")))) {
        _t1938 = gem_eq(gem_type_fn(NULL, _t1936, 1), gem_string("table"));
    } else {
        GemVal _t1937[] = {gem_v__match_29, gem_string("tag")};
        _t1938 = gem_has_key_fn(NULL, _t1937, 2);
    }
    GemVal _t1939;
    if (!gem_truthy(_t1938)) {
        _t1939 = _t1938;
    } else {
        _t1939 = gem_eq(gem_table_get(gem_v__match_29, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t1941;
    if (!gem_truthy(_t1939)) {
        _t1941 = _t1939;
    } else {
        GemVal _t1940[] = {gem_v__match_29, gem_string("arms")};
        _t1941 = gem_has_key_fn(NULL, _t1940, 2);
    }
    GemVal _t1943;
    if (!gem_truthy(_t1941)) {
        _t1943 = _t1941;
    } else {
        GemVal _t1942[] = {gem_v__match_29, gem_string("after_ms")};
        _t1943 = gem_has_key_fn(NULL, _t1942, 2);
    }
    GemVal _t1945;
    if (!gem_truthy(_t1943)) {
        _t1945 = _t1943;
    } else {
        GemVal _t1944[] = {gem_v__match_29, gem_string("after_body")};
        _t1945 = gem_has_key_fn(NULL, _t1944, 2);
    }
    if (gem_truthy(_t1945)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_29, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_29, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_29, gem_string("after_body"));
#line 459 "compiler/main.gem"
        GemVal gem_v__for_i_34 = gem_int(0);
#line 459 "compiler/main.gem"
    GemVal _t1946[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_34 = gem_len_fn(NULL, _t1946, 1);
#line 459 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
#line 459 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_34;
#line 459 "compiler/main.gem"
            gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 460 "compiler/main.gem"
    GemVal _t1947 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t1948 = gem_table_get(_t1947, gem_string("pattern"));
    GemVal _t1949[] = {gem_table_get(_t1948, gem_string("condition")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1950 = (*gem_v_walk_captures_node);
            (void)(_t1950.fn(_t1950.env, _t1949, 3));
#line 461 "compiler/main.gem"
    GemVal _t1951 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t1952 = gem_table_get(_t1951, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t1952, gem_string("bindings")), GEM_NIL))) {
#line 462 "compiler/main.gem"
                GemVal gem_v__for_i_33 = gem_int(0);
#line 462 "compiler/main.gem"
    GemVal _t1953 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t1954 = gem_table_get(_t1953, gem_string("pattern"));
    GemVal _t1955[] = {gem_table_get(_t1954, gem_string("bindings"))};
                GemVal gem_v__for_limit_33 = gem_len_fn(NULL, _t1955, 1);
#line 462 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
#line 462 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_33;
#line 462 "compiler/main.gem"
                    gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 463 "compiler/main.gem"
    GemVal _t1956 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t1957 = gem_table_get(_t1956, gem_string("pattern"));
    GemVal _t1958[] = {gem_table_get(gem_table_get(_t1957, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t1959 = (*gem_v_walk_captures_node);
                    (void)(_t1959.fn(_t1959.env, _t1958, 3));
                }

            }
#line 466 "compiler/main.gem"
    GemVal _t1960 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t1961[] = {gem_table_get(_t1960, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1962 = (*gem_v_walk_captures);
            (void)(_t1962.fn(_t1962.env, _t1961, 3));
        }

#line 468 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 469 "compiler/main.gem"
    GemVal _t1963[] = {gem_v_rm_after_ms, gem_v_scope_vars, gem_v_captured};
    GemVal _t1964 = (*gem_v_walk_captures_node);
            (void)(_t1964.fn(_t1964.env, _t1963, 3));
        }
#line 471 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 472 "compiler/main.gem"
    GemVal _t1965[] = {gem_v_rm_after_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t1966 = (*gem_v_walk_captures);
            GemVal _t1967 = _t1966.fn(_t1966.env, _t1965, 3);
            gem_pop_frame();
            return _t1967;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1968[] = {gem_v__match_29};
    GemVal _t1970;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1968, 1), gem_string("table")))) {
        _t1970 = gem_eq(gem_type_fn(NULL, _t1968, 1), gem_string("table"));
    } else {
        GemVal _t1969[] = {gem_v__match_29, gem_string("tag")};
        _t1970 = gem_has_key_fn(NULL, _t1969, 2);
    }
    GemVal _t1971;
    if (!gem_truthy(_t1970)) {
        _t1971 = _t1970;
    } else {
        _t1971 = gem_eq(gem_table_get(gem_v__match_29, gem_string("tag")), gem_string("block"));
    }
    GemVal _t1973;
    if (!gem_truthy(_t1971)) {
        _t1973 = _t1971;
    } else {
        GemVal _t1972[] = {gem_v__match_29, gem_string("stmts")};
        _t1973 = gem_has_key_fn(NULL, _t1972, 2);
    }
    if (gem_truthy(_t1973)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_29, gem_string("stmts"));
#line 475 "compiler/main.gem"
    GemVal _t1974[] = {gem_v_stmts, gem_v_scope_vars, gem_v_captured};
    GemVal _t1975 = (*gem_v_walk_captures);
        GemVal _t1976 = _t1975.fn(_t1975.env, _t1974, 3);
        gem_pop_frame();
        return _t1976;
    } else {
    GemVal _t1977[] = {gem_v__match_29};
    GemVal _t1979;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1977, 1), gem_string("table")))) {
        _t1979 = gem_eq(gem_type_fn(NULL, _t1977, 1), gem_string("table"));
    } else {
        GemVal _t1978[] = {gem_v__match_29, gem_string("tag")};
        _t1979 = gem_has_key_fn(NULL, _t1978, 2);
    }
    GemVal _t1980;
    if (!gem_truthy(_t1979)) {
        _t1980 = _t1979;
    } else {
        _t1980 = gem_eq(gem_table_get(gem_v__match_29, gem_string("tag")), gem_string("return"));
    }
    GemVal _t1982;
    if (!gem_truthy(_t1980)) {
        _t1982 = _t1980;
    } else {
        GemVal _t1981[] = {gem_v__match_29, gem_string("value")};
        _t1982 = gem_has_key_fn(NULL, _t1981, 2);
    }
    if (gem_truthy(_t1982)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_29, gem_string("value"));
#line 477 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 478 "compiler/main.gem"
    GemVal _t1983[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t1984 = (*gem_v_walk_captures_node);
            GemVal _t1985 = _t1984.fn(_t1984.env, _t1983, 3);
            gem_pop_frame();
            return _t1985;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1986[] = {gem_v__match_29};
    GemVal _t1988;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1986, 1), gem_string("table")))) {
        _t1988 = gem_eq(gem_type_fn(NULL, _t1986, 1), gem_string("table"));
    } else {
        GemVal _t1987[] = {gem_v__match_29, gem_string("tag")};
        _t1988 = gem_has_key_fn(NULL, _t1987, 2);
    }
    GemVal _t1989;
    if (!gem_truthy(_t1988)) {
        _t1989 = _t1988;
    } else {
        _t1989 = gem_eq(gem_table_get(gem_v__match_29, gem_string("tag")), gem_string("let"));
    }
    GemVal _t1991;
    if (!gem_truthy(_t1989)) {
        _t1991 = _t1989;
    } else {
        GemVal _t1990[] = {gem_v__match_29, gem_string("value")};
        _t1991 = gem_has_key_fn(NULL, _t1990, 2);
    }
    if (gem_truthy(_t1991)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_29, gem_string("value"));
#line 481 "compiler/main.gem"
    GemVal _t1992[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t1993 = (*gem_v_walk_captures_node);
        GemVal _t1994 = _t1993.fn(_t1993.env, _t1992, 3);
        gem_pop_frame();
        return _t1994;
    } else {
    GemVal _t1995[] = {gem_v__match_29};
    GemVal _t1997;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1995, 1), gem_string("table")))) {
        _t1997 = gem_eq(gem_type_fn(NULL, _t1995, 1), gem_string("table"));
    } else {
        GemVal _t1996[] = {gem_v__match_29, gem_string("tag")};
        _t1997 = gem_has_key_fn(NULL, _t1996, 2);
    }
    GemVal _t1998;
    if (!gem_truthy(_t1997)) {
        _t1998 = _t1997;
    } else {
        _t1998 = gem_eq(gem_table_get(gem_v__match_29, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2000;
    if (!gem_truthy(_t1998)) {
        _t2000 = _t1998;
    } else {
        GemVal _t1999[] = {gem_v__match_29, gem_string("value")};
        _t2000 = gem_has_key_fn(NULL, _t1999, 2);
    }
    if (gem_truthy(_t2000)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_29, gem_string("value"));
#line 483 "compiler/main.gem"
    GemVal _t2001[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2002 = (*gem_v_walk_captures_node);
        GemVal _t2003 = _t2002.fn(_t2002.env, _t2001, 3);
        gem_pop_frame();
        return _t2003;
    } else {
        {
#line 486 "compiler/main.gem"
            GemVal gem_v__for_tbl_35 = gem_v_node;
#line 486 "compiler/main.gem"
    GemVal _t2004[] = {gem_v__for_tbl_35};
            GemVal gem_v__for_keys_35 = gem_keys_fn(NULL, _t2004, 1);
#line 486 "compiler/main.gem"
            GemVal gem_v__for_i_35 = gem_int(0);
#line 486 "compiler/main.gem"
            while (1) {
                GemVal _t2005[] = {gem_v__for_keys_35};
                if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_len_fn(NULL, _t2005, 1)))) break;
#line 486 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_35, gem_v__for_i_35);
#line 486 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_35, gem_table_get(gem_v__for_keys_35, gem_v__for_i_35));
#line 486 "compiler/main.gem"
                gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 487 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 488 "compiler/main.gem"
    GemVal _t2006[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2006, 1), gem_string("table")))) {
#line 489 "compiler/main.gem"
    GemVal _t2007[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2007, 1);
#line 490 "compiler/main.gem"
    GemVal _t2008[] = {gem_v_vks};
    GemVal _t2010;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2008, 1), gem_int(0)))) {
        _t2010 = gem_gt(gem_len_fn(NULL, _t2008, 1), gem_int(0));
    } else {
        GemVal _t2009[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2010 = gem_eq(gem_type_fn(NULL, _t2009, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2010)) {
#line 491 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 492 "compiler/main.gem"
                            while (1) {
                                GemVal _t2011[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2011, 1)))) break;
#line 493 "compiler/main.gem"
    GemVal _t2012[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2012, 1), gem_string("table")))) {
#line 494 "compiler/main.gem"
    GemVal _t2013[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2014 = (*gem_v_walk_captures_node);
                                    (void)(_t2014.fn(_t2014.env, _t2013, 3));
                                }
#line 496 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 499 "compiler/main.gem"
    GemVal _t2015[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t2016 = (*gem_v_walk_captures_node);
                            (void)(_t2016.fn(_t2016.env, _t2015, 3));
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
#line 508 "compiler/main.gem"
    GemVal _t2018 = gem_table_new();
    GemVal _t2019[] = {gem_v_scope_vars, _t2018};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t2019, 2);
    {
#line 509 "compiler/main.gem"
        GemVal gem_v__for_i_36 = gem_int(0);
#line 509 "compiler/main.gem"
    GemVal _t2020[] = {gem_v_stmts};
        GemVal gem_v__for_limit_36 = gem_len_fn(NULL, _t2020, 1);
#line 509 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 509 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_36;
#line 509 "compiler/main.gem"
            gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 510 "compiler/main.gem"
    GemVal _t2021[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t2022 = (*gem_v_walk_captures_node);
            (void)(_t2022.fn(_t2022.env, _t2021, 3));
#line 511 "compiler/main.gem"
    GemVal _t2023[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t2025;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2023, 1), gem_string("table")))) {
        _t2025 = gem_eq(gem_type_fn(NULL, _t2023, 1), gem_string("table"));
    } else {
        GemVal _t2024 = gem_table_get(gem_v_stmts, gem_v_i);
        _t2025 = gem_eq(gem_table_get(_t2024, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t2025)) {
#line 512 "compiler/main.gem"
    GemVal _t2026 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2027[] = {gem_v_sv, gem_table_get(_t2026, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t2027, 2));
            }
#line 514 "compiler/main.gem"
    GemVal _t2028[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv};
            (void)(gem_fn_collect_block_lets(NULL, _t2028, 2));
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
#line 519 "compiler/main.gem"
    GemVal _t2030 = gem_table_new();
    GemVal gem_v_captured = _t2030;
#line 520 "compiler/main.gem"
    GemVal _t2031[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2032 = (*gem_v_walk_captures);
    (void)(_t2032.fn(_t2032.env, _t2031, 3));
    GemVal _t2033 = gem_v_captured;
    gem_pop_frame();
    return _t2033;
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
#line 549 "compiler/main.gem"
    GemVal gem_v__match_37 = gem_v_node;
    GemVal _t2035[] = {gem_v__match_37};
    GemVal _t2037;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2035, 1), gem_string("table")))) {
        _t2037 = gem_eq(gem_type_fn(NULL, _t2035, 1), gem_string("table"));
    } else {
        GemVal _t2036[] = {gem_v__match_37, gem_string("tag")};
        _t2037 = gem_has_key_fn(NULL, _t2036, 2);
    }
    GemVal _t2038;
    if (!gem_truthy(_t2037)) {
        _t2038 = _t2037;
    } else {
        _t2038 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("int"));
    }
    GemVal _t2040;
    if (!gem_truthy(_t2038)) {
        _t2040 = _t2038;
    } else {
        GemVal _t2039[] = {gem_v__match_37, gem_string("value")};
        _t2040 = gem_has_key_fn(NULL, _t2039, 2);
    }
    if (gem_truthy(_t2040)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_37, gem_string("value"));
#line 551 "compiler/main.gem"
    GemVal _t2041 = gem_table_new();
    GemVal _t2042[] = {gem_v_value};
    gem_table_set(_t2041, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t2042, 1)), gem_string(")")));
    gem_table_set(_t2041, gem_string("setup"), gem_string(""));
        GemVal _t2043 = _t2041;
        gem_pop_frame();
        return _t2043;
    } else {
    GemVal _t2044[] = {gem_v__match_37};
    GemVal _t2046;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2044, 1), gem_string("table")))) {
        _t2046 = gem_eq(gem_type_fn(NULL, _t2044, 1), gem_string("table"));
    } else {
        GemVal _t2045[] = {gem_v__match_37, gem_string("tag")};
        _t2046 = gem_has_key_fn(NULL, _t2045, 2);
    }
    GemVal _t2047;
    if (!gem_truthy(_t2046)) {
        _t2047 = _t2046;
    } else {
        _t2047 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("float"));
    }
    GemVal _t2049;
    if (!gem_truthy(_t2047)) {
        _t2049 = _t2047;
    } else {
        GemVal _t2048[] = {gem_v__match_37, gem_string("value")};
        _t2049 = gem_has_key_fn(NULL, _t2048, 2);
    }
    if (gem_truthy(_t2049)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_37, gem_string("value"));
#line 553 "compiler/main.gem"
    GemVal _t2050 = gem_table_new();
    GemVal _t2051[] = {gem_v_value};
    GemVal _t2052[] = {gem_fn_format_float(NULL, _t2051, 1)};
    gem_table_set(_t2050, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t2052, 1)), gem_string(")")));
    gem_table_set(_t2050, gem_string("setup"), gem_string(""));
        GemVal _t2053 = _t2050;
        gem_pop_frame();
        return _t2053;
    } else {
    GemVal _t2054[] = {gem_v__match_37};
    GemVal _t2056;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2054, 1), gem_string("table")))) {
        _t2056 = gem_eq(gem_type_fn(NULL, _t2054, 1), gem_string("table"));
    } else {
        GemVal _t2055[] = {gem_v__match_37, gem_string("tag")};
        _t2056 = gem_has_key_fn(NULL, _t2055, 2);
    }
    GemVal _t2057;
    if (!gem_truthy(_t2056)) {
        _t2057 = _t2056;
    } else {
        _t2057 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("string"));
    }
    GemVal _t2059;
    if (!gem_truthy(_t2057)) {
        _t2059 = _t2057;
    } else {
        GemVal _t2058[] = {gem_v__match_37, gem_string("value")};
        _t2059 = gem_has_key_fn(NULL, _t2058, 2);
    }
    if (gem_truthy(_t2059)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_37, gem_string("value"));
#line 555 "compiler/main.gem"
    GemVal _t2060[] = {gem_v_value};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2060, 1);
#line 556 "compiler/main.gem"
    GemVal _t2061 = gem_table_new();
    GemVal _t2062[] = {gem_v_escaped};
    gem_table_set(_t2061, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t2062, 1)), gem_string("\")")));
    gem_table_set(_t2061, gem_string("setup"), gem_string(""));
        GemVal _t2063 = _t2061;
        gem_pop_frame();
        return _t2063;
    } else {
    GemVal _t2064[] = {gem_v__match_37};
    GemVal _t2066;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2064, 1), gem_string("table")))) {
        _t2066 = gem_eq(gem_type_fn(NULL, _t2064, 1), gem_string("table"));
    } else {
        GemVal _t2065[] = {gem_v__match_37, gem_string("tag")};
        _t2066 = gem_has_key_fn(NULL, _t2065, 2);
    }
    GemVal _t2067;
    if (!gem_truthy(_t2066)) {
        _t2067 = _t2066;
    } else {
        _t2067 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t2069;
    if (!gem_truthy(_t2067)) {
        _t2069 = _t2067;
    } else {
        GemVal _t2068[] = {gem_v__match_37, gem_string("parts")};
        _t2069 = gem_has_key_fn(NULL, _t2068, 2);
    }
    if (gem_truthy(_t2069)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_parts = gem_table_get(gem_v__match_37, gem_string("parts"));
#line 558 "compiler/main.gem"
    GemVal _t2070[] = {gem_v_parts};
        if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2070, 1), gem_int(0)))) {
#line 559 "compiler/main.gem"
    GemVal _t2071 = gem_table_new();
    gem_table_set(_t2071, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t2071, gem_string("setup"), gem_string(""));
            GemVal _t2072 = _t2071;
            gem_pop_frame();
            return _t2072;
        }
#line 561 "compiler/main.gem"
        GemVal gem_v_setup = gem_string("");
#line 562 "compiler/main.gem"
    GemVal _t2073 = gem_table_new();
        GemVal gem_v_compiled = _t2073;
#line 563 "compiler/main.gem"
        GemVal gem_v__for_i_38 = gem_int(0);
#line 563 "compiler/main.gem"
    GemVal _t2074[] = {gem_v_parts};
        GemVal gem_v__for_limit_38 = gem_len_fn(NULL, _t2074, 1);
#line 563 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 563 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_38;
#line 563 "compiler/main.gem"
            gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 564 "compiler/main.gem"
            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 565 "compiler/main.gem"
    GemVal _t2075[] = {gem_v_part};
    GemVal _t2076 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2076.fn(_t2076.env, _t2075, 1);
#line 566 "compiler/main.gem"
    GemVal _t2077 = gem_v_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2077, gem_string("setup")));
#line 567 "compiler/main.gem"
    GemVal _t2078 = gem_v_part;
            if (gem_truthy(gem_eq(gem_table_get(_t2078, gem_string("tag")), gem_string("string")))) {
#line 568 "compiler/main.gem"
    GemVal _t2079 = gem_v_r;
    GemVal _t2080[] = {gem_v_compiled, gem_table_get(_t2079, gem_string("expr"))};
                (void)(gem_push_fn(NULL, _t2080, 2));
            } else {
#line 570 "compiler/main.gem"
    GemVal _t2081 = (*gem_v_tmp);
                GemVal gem_v_ts = _t2081.fn(_t2081.env, NULL, 0);
#line 571 "compiler/main.gem"
    GemVal _t2082[] = {gem_v_ts};
    GemVal _t2083 = gem_v_r;
    GemVal _t2084[] = {gem_table_get(_t2083, gem_string("expr"))};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2082, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2084, 1)), gem_string("};\n")));
#line 572 "compiler/main.gem"
    GemVal _t2085[] = {gem_v_ts};
    GemVal _t2086[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t2085, 1)), gem_string(", 1)"))};
                (void)(gem_push_fn(NULL, _t2086, 2));
            }
        }

#line 575 "compiler/main.gem"
        GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
#line 576 "compiler/main.gem"
        GemVal gem_v__for_i_39 = gem_int(1);
#line 576 "compiler/main.gem"
    GemVal _t2087[] = {gem_v_compiled};
        GemVal gem_v__for_limit_39 = gem_len_fn(NULL, _t2087, 1);
#line 576 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
#line 576 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_39;
#line 576 "compiler/main.gem"
            gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 577 "compiler/main.gem"
    GemVal _t2088[] = {gem_v_acc};
    GemVal _t2089[] = {gem_table_get(gem_v_compiled, gem_v_i)};
            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2088, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2089, 1)), gem_string(")"));
        }

#line 579 "compiler/main.gem"
    GemVal _t2090 = gem_table_new();
    gem_table_set(_t2090, gem_string("expr"), gem_v_acc);
    gem_table_set(_t2090, gem_string("setup"), gem_v_setup);
        GemVal _t2091 = _t2090;
        gem_pop_frame();
        return _t2091;
    } else {
    GemVal _t2092[] = {gem_v__match_37};
    GemVal _t2094;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2092, 1), gem_string("table")))) {
        _t2094 = gem_eq(gem_type_fn(NULL, _t2092, 1), gem_string("table"));
    } else {
        GemVal _t2093[] = {gem_v__match_37, gem_string("tag")};
        _t2094 = gem_has_key_fn(NULL, _t2093, 2);
    }
    GemVal _t2095;
    if (!gem_truthy(_t2094)) {
        _t2095 = _t2094;
    } else {
        _t2095 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("bool"));
    }
    GemVal _t2097;
    if (!gem_truthy(_t2095)) {
        _t2097 = _t2095;
    } else {
        GemVal _t2096[] = {gem_v__match_37, gem_string("value")};
        _t2097 = gem_has_key_fn(NULL, _t2096, 2);
    }
    if (gem_truthy(_t2097)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_37, gem_string("value"));
#line 581 "compiler/main.gem"
        if (gem_truthy(gem_v_value)) {
#line 582 "compiler/main.gem"
    GemVal _t2098 = gem_table_new();
    gem_table_set(_t2098, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t2098, gem_string("setup"), gem_string(""));
            GemVal _t2099 = _t2098;
            gem_pop_frame();
            return _t2099;
        }
#line 584 "compiler/main.gem"
    GemVal _t2100 = gem_table_new();
    gem_table_set(_t2100, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t2100, gem_string("setup"), gem_string(""));
        GemVal _t2101 = _t2100;
        gem_pop_frame();
        return _t2101;
    } else {
    GemVal _t2102[] = {gem_v__match_37};
    GemVal _t2104;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2102, 1), gem_string("table")))) {
        _t2104 = gem_eq(gem_type_fn(NULL, _t2102, 1), gem_string("table"));
    } else {
        GemVal _t2103[] = {gem_v__match_37, gem_string("tag")};
        _t2104 = gem_has_key_fn(NULL, _t2103, 2);
    }
    GemVal _t2105;
    if (!gem_truthy(_t2104)) {
        _t2105 = _t2104;
    } else {
        _t2105 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("nil"));
    }
    if (gem_truthy(_t2105)) {
#line 586 "compiler/main.gem"
    GemVal _t2106 = gem_table_new();
    gem_table_set(_t2106, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t2106, gem_string("setup"), gem_string(""));
        GemVal _t2107 = _t2106;
        gem_pop_frame();
        return _t2107;
    } else {
    GemVal _t2108[] = {gem_v__match_37};
    GemVal _t2110;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2108, 1), gem_string("table")))) {
        _t2110 = gem_eq(gem_type_fn(NULL, _t2108, 1), gem_string("table"));
    } else {
        GemVal _t2109[] = {gem_v__match_37, gem_string("tag")};
        _t2110 = gem_has_key_fn(NULL, _t2109, 2);
    }
    GemVal _t2111;
    if (!gem_truthy(_t2110)) {
        _t2111 = _t2110;
    } else {
        _t2111 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("var"));
    }
    GemVal _t2113;
    if (!gem_truthy(_t2111)) {
        _t2113 = _t2111;
    } else {
        GemVal _t2112[] = {gem_v__match_37, gem_string("name")};
        _t2113 = gem_has_key_fn(NULL, _t2112, 2);
    }
    if (gem_truthy(_t2113)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_37, gem_string("name"));
#line 588 "compiler/main.gem"
    GemVal _t2114[] = {(*gem_v_builtin_fns), gem_v_name};
        if (gem_truthy(gem_has_key_fn(NULL, _t2114, 2))) {
#line 589 "compiler/main.gem"
    GemVal _t2115 = gem_table_new();
    GemVal _t2116[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name)};
    gem_table_set(_t2115, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2116, 1)), gem_string(", NULL)")));
    gem_table_set(_t2115, gem_string("setup"), gem_string(""));
            GemVal _t2117 = _t2115;
            gem_pop_frame();
            return _t2117;
        } else {
#line 590 "compiler/main.gem"
    GemVal _t2118[] = {(*gem_v_defined_fns), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2118, 2))) {
#line 591 "compiler/main.gem"
    GemVal _t2119 = gem_table_new();
    GemVal _t2120[] = {gem_v_name};
    gem_table_set(_t2119, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t2120, 1)), gem_string(", NULL)")));
    gem_table_set(_t2119, gem_string("setup"), gem_string(""));
                GemVal _t2121 = _t2119;
                gem_pop_frame();
                return _t2121;
            }
        }
#line 593 "compiler/main.gem"
    GemVal _t2122[] = {gem_v_name};
    GemVal _t2123 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2123.fn(_t2123.env, _t2122, 1);
#line 594 "compiler/main.gem"
    GemVal _t2124[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2126;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2124, 2))) {
        _t2126 = gem_fn_set_contains(NULL, _t2124, 2);
    } else {
        GemVal _t2125[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2126 = gem_fn_set_contains(NULL, _t2125, 2);
    }
        if (gem_truthy(_t2126)) {
#line 595 "compiler/main.gem"
    GemVal _t2127 = gem_table_new();
    GemVal _t2128[] = {gem_v_mname};
    gem_table_set(_t2127, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t2128, 1)), gem_string(")")));
    gem_table_set(_t2127, gem_string("setup"), gem_string(""));
            GemVal _t2129 = _t2127;
            gem_pop_frame();
            return _t2129;
        }
#line 597 "compiler/main.gem"
    GemVal _t2130 = gem_table_new();
    gem_table_set(_t2130, gem_string("expr"), gem_v_mname);
    gem_table_set(_t2130, gem_string("setup"), gem_string(""));
        GemVal _t2131 = _t2130;
        gem_pop_frame();
        return _t2131;
    } else {
    GemVal _t2132[] = {gem_v__match_37};
    GemVal _t2134;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2132, 1), gem_string("table")))) {
        _t2134 = gem_eq(gem_type_fn(NULL, _t2132, 1), gem_string("table"));
    } else {
        GemVal _t2133[] = {gem_v__match_37, gem_string("tag")};
        _t2134 = gem_has_key_fn(NULL, _t2133, 2);
    }
    GemVal _t2135;
    if (!gem_truthy(_t2134)) {
        _t2135 = _t2134;
    } else {
        _t2135 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2135)) {
#line 599 "compiler/main.gem"
    GemVal _t2136[] = {gem_v_node};
    GemVal _t2137 = (*gem_v_compile_call);
        GemVal _t2138 = _t2137.fn(_t2137.env, _t2136, 1);
        gem_pop_frame();
        return _t2138;
    } else {
    GemVal _t2139[] = {gem_v__match_37};
    GemVal _t2141;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2139, 1), gem_string("table")))) {
        _t2141 = gem_eq(gem_type_fn(NULL, _t2139, 1), gem_string("table"));
    } else {
        GemVal _t2140[] = {gem_v__match_37, gem_string("tag")};
        _t2141 = gem_has_key_fn(NULL, _t2140, 2);
    }
    GemVal _t2142;
    if (!gem_truthy(_t2141)) {
        _t2142 = _t2141;
    } else {
        _t2142 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("binop"));
    }
    if (gem_truthy(_t2142)) {
#line 601 "compiler/main.gem"
    GemVal _t2143[] = {gem_v_node};
    GemVal _t2144 = (*gem_v_compile_binop);
        GemVal _t2145 = _t2144.fn(_t2144.env, _t2143, 1);
        gem_pop_frame();
        return _t2145;
    } else {
    GemVal _t2146[] = {gem_v__match_37};
    GemVal _t2148;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2146, 1), gem_string("table")))) {
        _t2148 = gem_eq(gem_type_fn(NULL, _t2146, 1), gem_string("table"));
    } else {
        GemVal _t2147[] = {gem_v__match_37, gem_string("tag")};
        _t2148 = gem_has_key_fn(NULL, _t2147, 2);
    }
    GemVal _t2149;
    if (!gem_truthy(_t2148)) {
        _t2149 = _t2148;
    } else {
        _t2149 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t2151;
    if (!gem_truthy(_t2149)) {
        _t2151 = _t2149;
    } else {
        GemVal _t2150[] = {gem_v__match_37, gem_string("expr")};
        _t2151 = gem_has_key_fn(NULL, _t2150, 2);
    }
    GemVal _t2153;
    if (!gem_truthy(_t2151)) {
        _t2153 = _t2151;
    } else {
        GemVal _t2152[] = {gem_v__match_37, gem_string("op")};
        _t2153 = gem_has_key_fn(NULL, _t2152, 2);
    }
    if (gem_truthy(_t2153)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_uexpr = gem_table_get(gem_v__match_37, gem_string("expr"));
#line 0 "compiler/main.gem"
        GemVal gem_v_op = gem_table_get(gem_v__match_37, gem_string("op"));
#line 603 "compiler/main.gem"
    GemVal _t2154[] = {gem_v_uexpr};
    GemVal _t2155 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2155.fn(_t2155.env, _t2154, 1);
#line 604 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 605 "compiler/main.gem"
    GemVal _t2156 = gem_table_new();
    GemVal _t2157 = gem_v_r;
    GemVal _t2158[] = {gem_table_get(_t2157, gem_string("expr"))};
    gem_table_set(_t2156, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t2158, 1)), gem_string(")")));
    GemVal _t2159 = gem_v_r;
    gem_table_set(_t2156, gem_string("setup"), gem_table_get(_t2159, gem_string("setup")));
            GemVal _t2160 = _t2156;
            gem_pop_frame();
            return _t2160;
        } else {
#line 606 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("not")))) {
#line 607 "compiler/main.gem"
    GemVal _t2161 = gem_table_new();
    GemVal _t2162 = gem_v_r;
    GemVal _t2163[] = {gem_table_get(_t2162, gem_string("expr"))};
    gem_table_set(_t2161, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t2163, 1)), gem_string(")")));
    GemVal _t2164 = gem_v_r;
    gem_table_set(_t2161, gem_string("setup"), gem_table_get(_t2164, gem_string("setup")));
                GemVal _t2165 = _t2161;
                gem_pop_frame();
                return _t2165;
            }
        }
#line 609 "compiler/main.gem"
    GemVal _t2166[] = {gem_v_op};
    GemVal _t2167[] = {gem_add(gem_string("unknown unary operator: "), gem_to_string_fn(NULL, _t2166, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 609, _t2167, 1));
    } else {
    GemVal _t2168[] = {gem_v__match_37};
    GemVal _t2170;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2168, 1), gem_string("table")))) {
        _t2170 = gem_eq(gem_type_fn(NULL, _t2168, 1), gem_string("table"));
    } else {
        GemVal _t2169[] = {gem_v__match_37, gem_string("tag")};
        _t2170 = gem_has_key_fn(NULL, _t2169, 2);
    }
    GemVal _t2171;
    if (!gem_truthy(_t2170)) {
        _t2171 = _t2170;
    } else {
        _t2171 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("anon_fn"));
    }
    if (gem_truthy(_t2171)) {
#line 611 "compiler/main.gem"
    GemVal _t2172[] = {gem_v_node};
    GemVal _t2173 = (*gem_v_compile_anon_fn);
        GemVal _t2174 = _t2173.fn(_t2173.env, _t2172, 1);
        gem_pop_frame();
        return _t2174;
    } else {
    GemVal _t2175[] = {gem_v__match_37};
    GemVal _t2177;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2175, 1), gem_string("table")))) {
        _t2177 = gem_eq(gem_type_fn(NULL, _t2175, 1), gem_string("table"));
    } else {
        GemVal _t2176[] = {gem_v__match_37, gem_string("tag")};
        _t2177 = gem_has_key_fn(NULL, _t2176, 2);
    }
    GemVal _t2178;
    if (!gem_truthy(_t2177)) {
        _t2178 = _t2177;
    } else {
        _t2178 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("table"));
    }
    if (gem_truthy(_t2178)) {
#line 613 "compiler/main.gem"
    GemVal _t2179[] = {gem_v_node};
    GemVal _t2180 = (*gem_v_compile_table);
        GemVal _t2181 = _t2180.fn(_t2180.env, _t2179, 1);
        gem_pop_frame();
        return _t2181;
    } else {
    GemVal _t2182[] = {gem_v__match_37};
    GemVal _t2184;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2182, 1), gem_string("table")))) {
        _t2184 = gem_eq(gem_type_fn(NULL, _t2182, 1), gem_string("table"));
    } else {
        GemVal _t2183[] = {gem_v__match_37, gem_string("tag")};
        _t2184 = gem_has_key_fn(NULL, _t2183, 2);
    }
    GemVal _t2185;
    if (!gem_truthy(_t2184)) {
        _t2185 = _t2184;
    } else {
        _t2185 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("array"));
    }
    if (gem_truthy(_t2185)) {
#line 615 "compiler/main.gem"
    GemVal _t2186[] = {gem_v_node};
    GemVal _t2187 = (*gem_v_compile_array);
        GemVal _t2188 = _t2187.fn(_t2187.env, _t2186, 1);
        gem_pop_frame();
        return _t2188;
    } else {
    GemVal _t2189[] = {gem_v__match_37};
    GemVal _t2191;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2189, 1), gem_string("table")))) {
        _t2191 = gem_eq(gem_type_fn(NULL, _t2189, 1), gem_string("table"));
    } else {
        GemVal _t2190[] = {gem_v__match_37, gem_string("tag")};
        _t2191 = gem_has_key_fn(NULL, _t2190, 2);
    }
    GemVal _t2192;
    if (!gem_truthy(_t2191)) {
        _t2192 = _t2191;
    } else {
        _t2192 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t2194;
    if (!gem_truthy(_t2192)) {
        _t2194 = _t2192;
    } else {
        GemVal _t2193[] = {gem_v__match_37, gem_string("object")};
        _t2194 = gem_has_key_fn(NULL, _t2193, 2);
    }
    GemVal _t2196;
    if (!gem_truthy(_t2194)) {
        _t2196 = _t2194;
    } else {
        GemVal _t2195[] = {gem_v__match_37, gem_string("field")};
        _t2196 = gem_has_key_fn(NULL, _t2195, 2);
    }
    if (gem_truthy(_t2196)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_37, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_37, gem_string("field"));
#line 617 "compiler/main.gem"
    GemVal _t2197[] = {gem_v_object};
    GemVal _t2198 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2198.fn(_t2198.env, _t2197, 1);
#line 618 "compiler/main.gem"
    GemVal _t2199 = (*gem_v_tmp);
        GemVal gem_v_t = _t2199.fn(_t2199.env, NULL, 0);
#line 619 "compiler/main.gem"
    GemVal _t2200 = gem_v_r;
    GemVal _t2201[] = {gem_v_t};
    GemVal _t2202 = gem_v_r;
    GemVal _t2203[] = {gem_table_get(_t2202, gem_string("expr"))};
        GemVal gem_v_setup = gem_add(gem_table_get(_t2200, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2201, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2203, 1)), gem_string(";\n")));
#line 620 "compiler/main.gem"
    GemVal _t2204[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2204, 1);
#line 621 "compiler/main.gem"
    GemVal _t2205 = gem_table_new();
    GemVal _t2206[] = {gem_v_t};
    GemVal _t2207[] = {gem_v_escaped};
    gem_table_set(_t2205, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2206, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2207, 1)), gem_string("\"))")));
    gem_table_set(_t2205, gem_string("setup"), gem_v_setup);
        GemVal _t2208 = _t2205;
        gem_pop_frame();
        return _t2208;
    } else {
    GemVal _t2209[] = {gem_v__match_37};
    GemVal _t2211;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2209, 1), gem_string("table")))) {
        _t2211 = gem_eq(gem_type_fn(NULL, _t2209, 1), gem_string("table"));
    } else {
        GemVal _t2210[] = {gem_v__match_37, gem_string("tag")};
        _t2211 = gem_has_key_fn(NULL, _t2210, 2);
    }
    GemVal _t2212;
    if (!gem_truthy(_t2211)) {
        _t2212 = _t2211;
    } else {
        _t2212 = gem_eq(gem_table_get(gem_v__match_37, gem_string("tag")), gem_string("index"));
    }
    GemVal _t2214;
    if (!gem_truthy(_t2212)) {
        _t2214 = _t2212;
    } else {
        GemVal _t2213[] = {gem_v__match_37, gem_string("object")};
        _t2214 = gem_has_key_fn(NULL, _t2213, 2);
    }
    GemVal _t2216;
    if (!gem_truthy(_t2214)) {
        _t2216 = _t2214;
    } else {
        GemVal _t2215[] = {gem_v__match_37, gem_string("key")};
        _t2216 = gem_has_key_fn(NULL, _t2215, 2);
    }
    if (gem_truthy(_t2216)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_37, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_37, gem_string("key"));
#line 623 "compiler/main.gem"
    GemVal _t2217[] = {gem_v_object};
    GemVal _t2218 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2218.fn(_t2218.env, _t2217, 1);
#line 624 "compiler/main.gem"
    GemVal _t2219[] = {gem_v_key};
    GemVal _t2220 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2220.fn(_t2220.env, _t2219, 1);
#line 625 "compiler/main.gem"
    GemVal _t2221 = gem_table_new();
    GemVal _t2222 = gem_v_obj_r;
    GemVal _t2223[] = {gem_table_get(_t2222, gem_string("expr"))};
    GemVal _t2224 = gem_v_key_r;
    GemVal _t2225[] = {gem_table_get(_t2224, gem_string("expr"))};
    gem_table_set(_t2221, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2223, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2225, 1)), gem_string(")")));
    GemVal _t2226 = gem_v_obj_r;
    GemVal _t2227 = gem_v_key_r;
    gem_table_set(_t2221, gem_string("setup"), gem_add(gem_table_get(_t2226, gem_string("setup")), gem_table_get(_t2227, gem_string("setup"))));
        GemVal _t2228 = _t2221;
        gem_pop_frame();
        return _t2228;
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
#line 628 "compiler/main.gem"
    GemVal _t2229 = gem_v_node;
    GemVal _t2230[] = {gem_table_get(_t2229, gem_string("tag"))};
    GemVal _t2231[] = {gem_add(gem_string("unknown expression node: "), gem_to_string_fn(NULL, _t2230, 1))};
    GemVal _t2232 = gem_error_at_fn("compiler/main.gem", 628, _t2231, 1);
    gem_pop_frame();
    return _t2232;
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
#line 634 "compiler/main.gem"
    GemVal _t2234 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t2234, gem_string("entries"));
#line 635 "compiler/main.gem"
    GemVal _t2235 = (*gem_v_tmp);
    GemVal gem_v_t = _t2235.fn(_t2235.env, NULL, 0);
#line 636 "compiler/main.gem"
    GemVal _t2236[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2236, 1)), gem_string(" = gem_table_new();\n"));
#line 637 "compiler/main.gem"
    GemVal gem_v__for_items_40 = gem_v_entries;
#line 637 "compiler/main.gem"
    GemVal gem_v__for_i_40 = gem_int(0);
#line 637 "compiler/main.gem"
    while (1) {
        GemVal _t2237[] = {gem_v__for_items_40};
        if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_len_fn(NULL, _t2237, 1)))) break;
#line 637 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_40, gem_v__for_i_40);
#line 637 "compiler/main.gem"
        gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 638 "compiler/main.gem"
    GemVal _t2238 = gem_v_entry;
    GemVal _t2239[] = {gem_table_get(_t2238, gem_string("value"))};
    GemVal _t2240 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2240.fn(_t2240.env, _t2239, 1);
#line 639 "compiler/main.gem"
    GemVal _t2241 = gem_v_entry;
    GemVal _t2242[] = {gem_table_get(_t2241, gem_string("key"))};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2242, 1);
#line 640 "compiler/main.gem"
    GemVal _t2243 = gem_v_val_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2243, gem_string("setup")));
#line 641 "compiler/main.gem"
    GemVal _t2244[] = {gem_v_t};
    GemVal _t2245[] = {gem_v_escaped};
    GemVal _t2246 = gem_v_val_r;
    GemVal _t2247[] = {gem_table_get(_t2246, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2244, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2245, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2247, 1)), gem_string(");\n")));
    }

    GemVal _t2248 = gem_table_new();
    gem_table_set(_t2248, gem_string("expr"), gem_v_t);
    gem_table_set(_t2248, gem_string("setup"), gem_v_setup);
    GemVal _t2249 = _t2248;
    gem_pop_frame();
    return _t2249;
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
#line 649 "compiler/main.gem"
    GemVal _t2251 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t2251, gem_string("elements"));
#line 650 "compiler/main.gem"
    GemVal _t2252 = (*gem_v_tmp);
    GemVal gem_v_t = _t2252.fn(_t2252.env, NULL, 0);
#line 651 "compiler/main.gem"
    GemVal _t2253[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2253, 1)), gem_string(" = gem_table_new();\n"));
#line 652 "compiler/main.gem"
    GemVal gem_v__for_i_41 = gem_int(0);
#line 652 "compiler/main.gem"
    GemVal _t2254[] = {gem_v_elements};
    GemVal gem_v__for_limit_41 = gem_len_fn(NULL, _t2254, 1);
#line 652 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_limit_41))) break;
#line 652 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_41;
#line 652 "compiler/main.gem"
        gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
#line 653 "compiler/main.gem"
    GemVal _t2255[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t2256 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t2256.fn(_t2256.env, _t2255, 1);
#line 654 "compiler/main.gem"
    GemVal _t2257 = gem_v_elem_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2257, gem_string("setup")));
#line 655 "compiler/main.gem"
    GemVal _t2258[] = {gem_v_t};
    GemVal _t2259[] = {gem_v_i};
    GemVal _t2260 = gem_v_elem_r;
    GemVal _t2261[] = {gem_table_get(_t2260, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2258, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t2259, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t2261, 1)), gem_string(");\n")));
    }

    GemVal _t2262 = gem_table_new();
    gem_table_set(_t2262, gem_string("expr"), gem_v_t);
    gem_table_set(_t2262, gem_string("setup"), gem_v_setup);
    GemVal _t2263 = _t2262;
    gem_pop_frame();
    return _t2263;
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
#line 663 "compiler/main.gem"
    GemVal _t2265 = gem_v_node;
    GemVal _t2266[] = {gem_table_get(_t2265, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t2266, 1);
#line 664 "compiler/main.gem"
    GemVal _t2267 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2267, gem_string("rest_param")), GEM_NIL))) {
#line 665 "compiler/main.gem"
    GemVal _t2268 = gem_v_node;
    GemVal _t2269[] = {gem_v_inner_defined, gem_table_get(_t2268, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t2269, 2));
    }
#line 667 "compiler/main.gem"
    GemVal _t2270 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2270, gem_string("block_param")), GEM_NIL))) {
#line 668 "compiler/main.gem"
    GemVal _t2271 = gem_v_node;
    GemVal _t2272[] = {gem_v_inner_defined, gem_table_get(_t2271, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t2272, 2));
    }
#line 670 "compiler/main.gem"
    GemVal _t2273 = gem_v_node;
    GemVal _t2274[] = {gem_table_get(_t2273, gem_string("body")), gem_v_inner_defined};
    GemVal _t2275 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t2275.fn(_t2275.env, _t2274, 2);
#line 671 "compiler/main.gem"
    GemVal _t2276[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t2276, 1);
#line 673 "compiler/main.gem"
    GemVal _t2277[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2277, 1), gem_int(0)))) {
#line 674 "compiler/main.gem"
    GemVal _t2278 = gem_table_new();
    GemVal _t2279[] = {gem_v_node, _t2278};
    GemVal _t2280 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t2280.fn(_t2280.env, _t2279, 2);
#line 675 "compiler/main.gem"
    GemVal _t2281 = gem_table_new();
    GemVal _t2282 = gem_v_result;
    GemVal _t2283[] = {gem_table_get(_t2282, gem_string("fn_name"))};
    gem_table_set(_t2281, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2283, 1)), gem_string(", NULL)")));
    gem_table_set(_t2281, gem_string("setup"), gem_string(""));
        GemVal _t2284 = _t2281;
        gem_pop_frame();
        return _t2284;
    }
#line 678 "compiler/main.gem"
    GemVal _t2285[] = {gem_v_node, gem_v_captures};
    GemVal _t2286 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t2286.fn(_t2286.env, _t2285, 2);
#line 680 "compiler/main.gem"
    GemVal gem_v_setup = gem_string("");
#line 681 "compiler/main.gem"
    GemVal _t2287 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t2287.fn(_t2287.env, NULL, 0);
#line 682 "compiler/main.gem"
    GemVal _t2288 = gem_v_result;
    GemVal _t2289[] = {gem_table_get(_t2288, gem_string("struct_name"))};
    GemVal _t2290[] = {gem_v_env_tmp};
    GemVal _t2291 = gem_v_result;
    GemVal _t2292[] = {gem_table_get(_t2291, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2289, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t2290, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t2292, 1)), gem_string("));\n")));
#line 683 "compiler/main.gem"
    GemVal gem_v__for_items_42 = gem_v_captures;
#line 683 "compiler/main.gem"
    GemVal gem_v__for_i_42 = gem_int(0);
#line 683 "compiler/main.gem"
    while (1) {
        GemVal _t2293[] = {gem_v__for_items_42};
        if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_len_fn(NULL, _t2293, 1)))) break;
#line 683 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_42, gem_v__for_i_42);
#line 683 "compiler/main.gem"
        gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 684 "compiler/main.gem"
    GemVal _t2294[] = {gem_v_cap};
    GemVal _t2295 = (*gem_v_mangle);
        GemVal gem_v_mc = _t2295.fn(_t2295.env, _t2294, 1);
#line 685 "compiler/main.gem"
    GemVal _t2296[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2296, 2))) {
#line 686 "compiler/main.gem"
    GemVal _t2297[] = {gem_v_env_tmp};
    GemVal _t2298[] = {gem_v_mc};
    GemVal _t2299[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2297, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2298, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2299, 1)), gem_string(";\n")));
        } else {
#line 688 "compiler/main.gem"
    GemVal _t2300[] = {gem_v_env_tmp};
    GemVal _t2301[] = {gem_v_mc};
    GemVal _t2302[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2300, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2301, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t2302, 1)), gem_string(";\n")));
        }
    }

    GemVal _t2303 = gem_table_new();
    GemVal _t2304 = gem_v_result;
    GemVal _t2305[] = {gem_table_get(_t2304, gem_string("fn_name"))};
    GemVal _t2306[] = {gem_v_env_tmp};
    gem_table_set(_t2303, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2305, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2306, 1)), gem_string(")")));
    gem_table_set(_t2303, gem_string("setup"), gem_v_setup);
    GemVal _t2307 = _t2303;
    gem_pop_frame();
    return _t2307;
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
#line 698 "compiler/main.gem"
    GemVal gem_v_arg_setups = gem_string("");
#line 699 "compiler/main.gem"
    GemVal _t2309 = gem_table_new();
    GemVal gem_v_arg_exprs = _t2309;
#line 700 "compiler/main.gem"
    GemVal gem_v__for_i_43 = gem_int(0);
#line 700 "compiler/main.gem"
    GemVal _t2310[] = {gem_v_args};
    GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t2310, 1);
#line 700 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 700 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_43;
#line 700 "compiler/main.gem"
        gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 701 "compiler/main.gem"
    GemVal _t2311[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2312 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2312.fn(_t2312.env, _t2311, 1);
#line 702 "compiler/main.gem"
    GemVal _t2313 = gem_v_r;
        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t2313, gem_string("setup")));
#line 703 "compiler/main.gem"
    GemVal _t2314 = gem_v_r;
    GemVal _t2315[] = {gem_v_arg_exprs, gem_table_get(_t2314, gem_string("expr"))};
        (void)(gem_push_fn(NULL, _t2315, 2));
    }

#line 705 "compiler/main.gem"
    GemVal _t2316[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t2316, 1);
#line 706 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 707 "compiler/main.gem"
    GemVal _t2317 = gem_table_new();
    gem_table_set(_t2317, gem_string("setup"), gem_v_arg_setups);
    gem_table_set(_t2317, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t2317, gem_string("argc"), gem_int(0));
        GemVal _t2318 = _t2317;
        gem_pop_frame();
        return _t2318;
    }
#line 709 "compiler/main.gem"
    GemVal _t2319 = (*gem_v_tmp);
    GemVal gem_v_t = _t2319.fn(_t2319.env, NULL, 0);
#line 710 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 711 "compiler/main.gem"
    GemVal gem_v__for_i_44 = gem_int(1);
#line 711 "compiler/main.gem"
    GemVal gem_v__for_limit_44 = gem_v_argc;
#line 711 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 711 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_44;
#line 711 "compiler/main.gem"
        gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 712 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t2320 = gem_table_new();
    GemVal _t2321[] = {gem_v_t};
    GemVal _t2322[] = {gem_v_arr};
    gem_table_set(_t2320, gem_string("setup"), gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2321, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2322, 1)), gem_string("};\n"))));
    gem_table_set(_t2320, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t2320, gem_string("argc"), gem_v_argc);
    GemVal _t2323 = _t2320;
    gem_pop_frame();
    return _t2323;
}

struct _closure__anon_37 {
};
static GemVal _anon_37(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_37", "compiler/main.gem", 0);
#line 719 "compiler/main.gem"
    GemVal _t2325 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2325, gem_string("argc")), gem_int(0)))) {
#line 720 "compiler/main.gem"
    GemVal _t2326 = gem_table_new();
    GemVal _t2327[] = {gem_v_fn_name};
    GemVal _t2328[] = {gem_v_env};
    gem_table_set(_t2326, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2327, 1), gem_string("(")), gem_to_string_fn(NULL, _t2328, 1)), gem_string(", NULL, 0)")));
    GemVal _t2329 = gem_v_ca;
    gem_table_set(_t2326, gem_string("setup"), gem_table_get(_t2329, gem_string("setup")));
        GemVal _t2330 = _t2326;
        gem_pop_frame();
        return _t2330;
    }
    GemVal _t2331 = gem_table_new();
    GemVal _t2332[] = {gem_v_fn_name};
    GemVal _t2333[] = {gem_v_env};
    GemVal _t2334 = gem_v_ca;
    GemVal _t2335[] = {gem_table_get(_t2334, gem_string("arr_name"))};
    GemVal _t2336 = gem_v_ca;
    GemVal _t2337[] = {gem_table_get(_t2336, gem_string("argc"))};
    gem_table_set(_t2331, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2332, 1), gem_string("(")), gem_to_string_fn(NULL, _t2333, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2335, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2337, 1)), gem_string(")")));
    GemVal _t2338 = gem_v_ca;
    gem_table_set(_t2331, gem_string("setup"), gem_table_get(_t2338, gem_string("setup")));
    GemVal _t2339 = _t2331;
    gem_pop_frame();
    return _t2339;
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
#line 728 "compiler/main.gem"
    GemVal gem_v__d45 = gem_v_node;
#line 728 "compiler/main.gem"
    GemVal _t2340 = gem_v__d45;
    GemVal gem_v_func = gem_table_get(_t2340, gem_string("func"));
#line 728 "compiler/main.gem"
    GemVal _t2341 = gem_v__d45;
    GemVal gem_v_args = gem_table_get(_t2341, gem_string("args"));

#line 731 "compiler/main.gem"
    GemVal _t2342 = gem_v_func;
    GemVal _t2344;
    if (!gem_truthy(gem_eq(gem_table_get(_t2342, gem_string("tag")), gem_string("dot")))) {
        _t2344 = gem_eq(gem_table_get(_t2342, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t2343 = gem_v_func;
        _t2344 = gem_eq(gem_table_get(_t2343, gem_string("field")), gem_string("len"));
    }
    GemVal _t2346;
    if (!gem_truthy(_t2344)) {
        _t2346 = _t2344;
    } else {
        GemVal _t2345[] = {gem_v_args};
        _t2346 = gem_eq(gem_len_fn(NULL, _t2345, 1), gem_int(0));
    }
    if (gem_truthy(_t2346)) {
#line 732 "compiler/main.gem"
    GemVal _t2347 = gem_v_func;
    GemVal _t2348[] = {gem_table_get(_t2347, gem_string("object"))};
    GemVal _t2349 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2349.fn(_t2349.env, _t2348, 1);
#line 733 "compiler/main.gem"
    GemVal _t2350 = gem_table_new();
    GemVal _t2351 = gem_v_obj_r;
    GemVal _t2352[] = {gem_table_get(_t2351, gem_string("expr"))};
    gem_table_set(_t2350, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t2352, 1)), gem_string(")")));
    GemVal _t2353 = gem_v_obj_r;
    gem_table_set(_t2350, gem_string("setup"), gem_table_get(_t2353, gem_string("setup")));
        GemVal _t2354 = _t2350;
        gem_pop_frame();
        return _t2354;
    }
#line 737 "compiler/main.gem"
    GemVal _t2355 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t2355, gem_string("tag")), gem_string("var")))) {
#line 738 "compiler/main.gem"
    GemVal _t2356 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t2356, gem_string("name"));
#line 740 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 741 "compiler/main.gem"
    GemVal _t2357 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t2357, gem_string("line"));
#line 742 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 743 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 745 "compiler/main.gem"
    GemVal _t2358[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t2358, 1);
#line 746 "compiler/main.gem"
    GemVal _t2359[] = {gem_v_args};
    GemVal _t2360 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t2360.fn(_t2360.env, _t2359, 1);
#line 747 "compiler/main.gem"
    GemVal _t2361 = gem_v_ca;
            if (gem_truthy(gem_eq(gem_table_get(_t2361, gem_string("argc")), gem_int(0)))) {
#line 748 "compiler/main.gem"
    GemVal _t2362 = gem_table_new();
    GemVal _t2363[] = {gem_v_escaped_file};
    GemVal _t2364[] = {gem_v_line};
    gem_table_set(_t2362, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2363, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2364, 1)), gem_string(", NULL, 0)")));
    GemVal _t2365 = gem_v_ca;
    gem_table_set(_t2362, gem_string("setup"), gem_table_get(_t2365, gem_string("setup")));
                GemVal _t2366 = _t2362;
                gem_pop_frame();
                return _t2366;
            }
#line 750 "compiler/main.gem"
    GemVal _t2367 = gem_table_new();
    GemVal _t2368[] = {gem_v_escaped_file};
    GemVal _t2369[] = {gem_v_line};
    GemVal _t2370 = gem_v_ca;
    GemVal _t2371[] = {gem_table_get(_t2370, gem_string("arr_name"))};
    GemVal _t2372 = gem_v_ca;
    GemVal _t2373[] = {gem_table_get(_t2372, gem_string("argc"))};
    gem_table_set(_t2367, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2368, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2369, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2371, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2373, 1)), gem_string(")")));
    GemVal _t2374 = gem_v_ca;
    gem_table_set(_t2367, gem_string("setup"), gem_table_get(_t2374, gem_string("setup")));
            GemVal _t2375 = _t2367;
            gem_pop_frame();
            return _t2375;
        } else {
#line 751 "compiler/main.gem"
    GemVal _t2376[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t2376, 2))) {
#line 752 "compiler/main.gem"
    GemVal _t2377[] = {gem_v_args};
    GemVal _t2378 = (*gem_v_compile_args);
    GemVal _t2379[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t2378.fn(_t2378.env, _t2377, 1)};
    GemVal _t2380 = (*gem_v_emit_direct_call);
                GemVal _t2381 = _t2380.fn(_t2380.env, _t2379, 3);
                gem_pop_frame();
                return _t2381;
            } else {
#line 753 "compiler/main.gem"
    GemVal _t2382[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t2382, 2))) {
#line 754 "compiler/main.gem"
    GemVal _t2383[] = {gem_v_name};
    GemVal _t2384[] = {gem_v_args};
    GemVal _t2385 = (*gem_v_compile_args);
    GemVal _t2386[] = {gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t2383, 1)), gem_string("NULL"), _t2385.fn(_t2385.env, _t2384, 1)};
    GemVal _t2387 = (*gem_v_emit_direct_call);
                    GemVal _t2388 = _t2387.fn(_t2387.env, _t2386, 3);
                    gem_pop_frame();
                    return _t2388;
                }
            }
        }
    }
#line 759 "compiler/main.gem"
    GemVal _t2389[] = {gem_v_func};
    GemVal _t2390 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t2390.fn(_t2390.env, _t2389, 1);
#line 760 "compiler/main.gem"
    GemVal _t2391[] = {gem_v_args};
    GemVal _t2392 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t2392.fn(_t2392.env, _t2391, 1);
#line 761 "compiler/main.gem"
    GemVal _t2393 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t2393.fn(_t2393.env, NULL, 0);
#line 762 "compiler/main.gem"
    GemVal _t2394 = gem_v_func_r;
    GemVal _t2395 = gem_v_ca;
    GemVal _t2396[] = {gem_v_tmp_fn};
    GemVal _t2397 = gem_v_func_r;
    GemVal _t2398[] = {gem_table_get(_t2397, gem_string("expr"))};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get(_t2394, gem_string("setup")), gem_table_get(_t2395, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2396, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2398, 1)), gem_string(";\n")));
#line 763 "compiler/main.gem"
    GemVal _t2399 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2399, gem_string("argc")), gem_int(0)))) {
#line 764 "compiler/main.gem"
    GemVal _t2400 = gem_table_new();
    GemVal _t2401[] = {gem_v_tmp_fn};
    GemVal _t2402[] = {gem_v_tmp_fn};
    gem_table_set(_t2400, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2401, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2402, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t2400, gem_string("setup"), gem_v_setup);
        GemVal _t2403 = _t2400;
        gem_pop_frame();
        return _t2403;
    }
    GemVal _t2404 = gem_table_new();
    GemVal _t2405[] = {gem_v_tmp_fn};
    GemVal _t2406[] = {gem_v_tmp_fn};
    GemVal _t2407 = gem_v_ca;
    GemVal _t2408[] = {gem_table_get(_t2407, gem_string("arr_name"))};
    GemVal _t2409 = gem_v_ca;
    GemVal _t2410[] = {gem_table_get(_t2409, gem_string("argc"))};
    gem_table_set(_t2404, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2405, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2406, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t2408, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2410, 1)), gem_string(")")));
    gem_table_set(_t2404, gem_string("setup"), gem_v_setup);
    GemVal _t2411 = _t2404;
    gem_pop_frame();
    return _t2411;
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
#line 772 "compiler/main.gem"
    GemVal _t2413 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t2413, gem_string("op"));
#line 773 "compiler/main.gem"
    GemVal _t2414 = gem_v_node;
    GemVal _t2415[] = {gem_table_get(_t2414, gem_string("left"))};
    GemVal _t2416 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t2416.fn(_t2416.env, _t2415, 1);
#line 774 "compiler/main.gem"
    GemVal _t2417 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t2417, gem_string("expr"));
#line 775 "compiler/main.gem"
    GemVal _t2418 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t2418, gem_string("setup"));
#line 777 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 778 "compiler/main.gem"
    GemVal _t2419 = gem_v_node;
    GemVal _t2420[] = {gem_table_get(_t2419, gem_string("right"))};
    GemVal _t2421 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t2421.fn(_t2421.env, _t2420, 1);
#line 779 "compiler/main.gem"
    GemVal _t2422 = gem_table_new();
    GemVal _t2423[] = {gem_v_lc};
    GemVal _t2424 = gem_v_rc_r;
    GemVal _t2425[] = {gem_table_get(_t2424, gem_string("expr"))};
    gem_table_set(_t2422, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2423, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2425, 1)), gem_string(")")));
    GemVal _t2426 = gem_v_rc_r;
    gem_table_set(_t2422, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2426, gem_string("setup"))));
        GemVal _t2427 = _t2422;
        gem_pop_frame();
        return _t2427;
    } else {
#line 780 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 781 "compiler/main.gem"
    GemVal _t2428 = gem_v_node;
    GemVal _t2429[] = {gem_table_get(_t2428, gem_string("right"))};
    GemVal _t2430 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t2430.fn(_t2430.env, _t2429, 1);
#line 782 "compiler/main.gem"
    GemVal _t2431 = gem_table_new();
    GemVal _t2432[] = {gem_v_lc};
    GemVal _t2433 = gem_v_rc_r;
    GemVal _t2434[] = {gem_table_get(_t2433, gem_string("expr"))};
    gem_table_set(_t2431, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t2432, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2434, 1)), gem_string(")")));
    GemVal _t2435 = gem_v_rc_r;
    gem_table_set(_t2431, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2435, gem_string("setup"))));
            GemVal _t2436 = _t2431;
            gem_pop_frame();
            return _t2436;
        } else {
#line 783 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 784 "compiler/main.gem"
    GemVal _t2437 = gem_v_node;
    GemVal _t2438[] = {gem_table_get(_t2437, gem_string("right"))};
    GemVal _t2439 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t2439.fn(_t2439.env, _t2438, 1);
#line 785 "compiler/main.gem"
    GemVal _t2440 = gem_table_new();
    GemVal _t2441[] = {gem_v_lc};
    GemVal _t2442 = gem_v_rc_r;
    GemVal _t2443[] = {gem_table_get(_t2442, gem_string("expr"))};
    gem_table_set(_t2440, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t2441, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2443, 1)), gem_string(")")));
    GemVal _t2444 = gem_v_rc_r;
    gem_table_set(_t2440, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2444, gem_string("setup"))));
                GemVal _t2445 = _t2440;
                gem_pop_frame();
                return _t2445;
            } else {
#line 786 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 787 "compiler/main.gem"
    GemVal _t2446 = gem_v_node;
    GemVal _t2447[] = {gem_table_get(_t2446, gem_string("right"))};
    GemVal _t2448 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t2448.fn(_t2448.env, _t2447, 1);
#line 788 "compiler/main.gem"
    GemVal _t2449 = gem_table_new();
    GemVal _t2450[] = {gem_v_lc};
    GemVal _t2451 = gem_v_rc_r;
    GemVal _t2452[] = {gem_table_get(_t2451, gem_string("expr"))};
    gem_table_set(_t2449, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t2450, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2452, 1)), gem_string(")")));
    GemVal _t2453 = gem_v_rc_r;
    gem_table_set(_t2449, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2453, gem_string("setup"))));
                    GemVal _t2454 = _t2449;
                    gem_pop_frame();
                    return _t2454;
                } else {
#line 789 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 790 "compiler/main.gem"
    GemVal _t2455 = gem_v_node;
    GemVal _t2456[] = {gem_table_get(_t2455, gem_string("right"))};
    GemVal _t2457 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t2457.fn(_t2457.env, _t2456, 1);
#line 791 "compiler/main.gem"
    GemVal _t2458 = gem_table_new();
    GemVal _t2459[] = {gem_v_lc};
    GemVal _t2460 = gem_v_rc_r;
    GemVal _t2461[] = {gem_table_get(_t2460, gem_string("expr"))};
    gem_table_set(_t2458, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t2459, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2461, 1)), gem_string(")")));
    GemVal _t2462 = gem_v_rc_r;
    gem_table_set(_t2458, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2462, gem_string("setup"))));
                        GemVal _t2463 = _t2458;
                        gem_pop_frame();
                        return _t2463;
                    } else {
#line 792 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 793 "compiler/main.gem"
    GemVal _t2464 = gem_v_node;
    GemVal _t2465[] = {gem_table_get(_t2464, gem_string("right"))};
    GemVal _t2466 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t2466.fn(_t2466.env, _t2465, 1);
#line 794 "compiler/main.gem"
    GemVal _t2467 = (*gem_v_tmp);
                            GemVal gem_v_t = _t2467.fn(_t2467.env, NULL, 0);
#line 795 "compiler/main.gem"
    GemVal _t2468 = gem_v_rc_r;
    GemVal _t2469[] = {gem_v_t};
    GemVal _t2470 = gem_v_rc_r;
    GemVal _t2471[] = {gem_table_get(_t2470, gem_string("expr"))};
    GemVal _t2472[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t2468, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2469, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2471, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2472, 1)), gem_string("};\n")));
#line 796 "compiler/main.gem"
    GemVal _t2473 = gem_table_new();
    GemVal _t2474[] = {gem_v_t};
    gem_table_set(_t2473, gem_string("expr"), gem_add(gem_add(gem_string("gem_has_key_fn(NULL, "), gem_to_string_fn(NULL, _t2474, 1)), gem_string(", 2)")));
    gem_table_set(_t2473, gem_string("setup"), gem_v_setup);
                            GemVal _t2475 = _t2473;
                            gem_pop_frame();
                            return _t2475;
                        } else {
#line 797 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 798 "compiler/main.gem"
    GemVal _t2476 = gem_v_node;
    GemVal _t2477[] = {gem_table_get(_t2476, gem_string("right"))};
    GemVal _t2478 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t2478.fn(_t2478.env, _t2477, 1);
#line 799 "compiler/main.gem"
    GemVal _t2479 = gem_table_new();
    GemVal _t2480[] = {gem_v_lc};
    GemVal _t2481 = gem_v_rc_r;
    GemVal _t2482[] = {gem_table_get(_t2481, gem_string("expr"))};
    gem_table_set(_t2479, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t2480, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2482, 1)), gem_string(")")));
    GemVal _t2483 = gem_v_rc_r;
    gem_table_set(_t2479, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2483, gem_string("setup"))));
                                GemVal _t2484 = _t2479;
                                gem_pop_frame();
                                return _t2484;
                            } else {
#line 800 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 801 "compiler/main.gem"
    GemVal _t2485 = gem_v_node;
    GemVal _t2486[] = {gem_table_get(_t2485, gem_string("right"))};
    GemVal _t2487 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t2487.fn(_t2487.env, _t2486, 1);
#line 802 "compiler/main.gem"
    GemVal _t2488 = gem_table_new();
    GemVal _t2489[] = {gem_v_lc};
    GemVal _t2490 = gem_v_rc_r;
    GemVal _t2491[] = {gem_table_get(_t2490, gem_string("expr"))};
    gem_table_set(_t2488, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t2489, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2491, 1)), gem_string(")")));
    GemVal _t2492 = gem_v_rc_r;
    gem_table_set(_t2488, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2492, gem_string("setup"))));
                                    GemVal _t2493 = _t2488;
                                    gem_pop_frame();
                                    return _t2493;
                                } else {
#line 803 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 804 "compiler/main.gem"
    GemVal _t2494 = gem_v_node;
    GemVal _t2495[] = {gem_table_get(_t2494, gem_string("right"))};
    GemVal _t2496 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t2496.fn(_t2496.env, _t2495, 1);
#line 805 "compiler/main.gem"
    GemVal _t2497 = gem_table_new();
    GemVal _t2498[] = {gem_v_lc};
    GemVal _t2499 = gem_v_rc_r;
    GemVal _t2500[] = {gem_table_get(_t2499, gem_string("expr"))};
    gem_table_set(_t2497, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t2498, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2500, 1)), gem_string(")")));
    GemVal _t2501 = gem_v_rc_r;
    gem_table_set(_t2497, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2501, gem_string("setup"))));
                                        GemVal _t2502 = _t2497;
                                        gem_pop_frame();
                                        return _t2502;
                                    } else {
#line 806 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 807 "compiler/main.gem"
    GemVal _t2503 = gem_v_node;
    GemVal _t2504[] = {gem_table_get(_t2503, gem_string("right"))};
    GemVal _t2505 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t2505.fn(_t2505.env, _t2504, 1);
#line 808 "compiler/main.gem"
    GemVal _t2506 = gem_table_new();
    GemVal _t2507[] = {gem_v_lc};
    GemVal _t2508 = gem_v_rc_r;
    GemVal _t2509[] = {gem_table_get(_t2508, gem_string("expr"))};
    gem_table_set(_t2506, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t2507, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2509, 1)), gem_string(")")));
    GemVal _t2510 = gem_v_rc_r;
    gem_table_set(_t2506, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2510, gem_string("setup"))));
                                            GemVal _t2511 = _t2506;
                                            gem_pop_frame();
                                            return _t2511;
                                        } else {
#line 809 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 810 "compiler/main.gem"
    GemVal _t2512 = gem_v_node;
    GemVal _t2513[] = {gem_table_get(_t2512, gem_string("right"))};
    GemVal _t2514 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t2514.fn(_t2514.env, _t2513, 1);
#line 811 "compiler/main.gem"
    GemVal _t2515 = gem_table_new();
    GemVal _t2516[] = {gem_v_lc};
    GemVal _t2517 = gem_v_rc_r;
    GemVal _t2518[] = {gem_table_get(_t2517, gem_string("expr"))};
    gem_table_set(_t2515, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t2516, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2518, 1)), gem_string(")")));
    GemVal _t2519 = gem_v_rc_r;
    gem_table_set(_t2515, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2519, gem_string("setup"))));
                                                GemVal _t2520 = _t2515;
                                                gem_pop_frame();
                                                return _t2520;
                                            } else {
#line 812 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 813 "compiler/main.gem"
    GemVal _t2521 = gem_v_node;
    GemVal _t2522[] = {gem_table_get(_t2521, gem_string("right"))};
    GemVal _t2523 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t2523.fn(_t2523.env, _t2522, 1);
#line 814 "compiler/main.gem"
    GemVal _t2524 = gem_table_new();
    GemVal _t2525[] = {gem_v_lc};
    GemVal _t2526 = gem_v_rc_r;
    GemVal _t2527[] = {gem_table_get(_t2526, gem_string("expr"))};
    gem_table_set(_t2524, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t2525, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2527, 1)), gem_string(")")));
    GemVal _t2528 = gem_v_rc_r;
    gem_table_set(_t2524, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2528, gem_string("setup"))));
                                                    GemVal _t2529 = _t2524;
                                                    gem_pop_frame();
                                                    return _t2529;
                                                } else {
#line 815 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 816 "compiler/main.gem"
    GemVal _t2530 = gem_v_node;
    GemVal _t2531[] = {gem_table_get(_t2530, gem_string("right"))};
    GemVal _t2532 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t2532.fn(_t2532.env, _t2531, 1);
#line 817 "compiler/main.gem"
    GemVal _t2533 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t2533.fn(_t2533.env, NULL, 0);
#line 818 "compiler/main.gem"
                                                        GemVal gem_v_setup = gem_v_ls;
#line 819 "compiler/main.gem"
    GemVal _t2534[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2534, 1)), gem_string(";\n")));
#line 820 "compiler/main.gem"
    GemVal _t2535[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t2535, 1)), gem_string(")) {\n")));
#line 821 "compiler/main.gem"
    GemVal _t2536[] = {gem_v_t};
    GemVal _t2537[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2536, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2537, 1)), gem_string(";\n")));
#line 822 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 823 "compiler/main.gem"
    GemVal _t2538 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t2538, gem_string("setup")), gem_string("")))) {
#line 824 "compiler/main.gem"
    GemVal _t2539 = gem_v_rc_r;
    GemVal _t2540[] = {gem_table_get(_t2539, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t2540, 3));
                                                        }
#line 826 "compiler/main.gem"
    GemVal _t2541[] = {gem_v_t};
    GemVal _t2542 = gem_v_rc_r;
    GemVal _t2543[] = {gem_table_get(_t2542, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2541, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2543, 1)), gem_string(";\n")));
#line 827 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 828 "compiler/main.gem"
    GemVal _t2544 = gem_table_new();
    gem_table_set(_t2544, gem_string("expr"), gem_v_t);
    gem_table_set(_t2544, gem_string("setup"), gem_v_setup);
                                                        GemVal _t2545 = _t2544;
                                                        gem_pop_frame();
                                                        return _t2545;
                                                    } else {
#line 829 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 830 "compiler/main.gem"
    GemVal _t2546 = gem_v_node;
    GemVal _t2547[] = {gem_table_get(_t2546, gem_string("right"))};
    GemVal _t2548 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t2548.fn(_t2548.env, _t2547, 1);
#line 831 "compiler/main.gem"
    GemVal _t2549 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t2549.fn(_t2549.env, NULL, 0);
#line 832 "compiler/main.gem"
                                                            GemVal gem_v_setup = gem_v_ls;
#line 833 "compiler/main.gem"
    GemVal _t2550[] = {gem_v_t};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2550, 1)), gem_string(";\n")));
#line 834 "compiler/main.gem"
    GemVal _t2551[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t2551, 1)), gem_string(")) {\n")));
#line 835 "compiler/main.gem"
    GemVal _t2552[] = {gem_v_t};
    GemVal _t2553[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2552, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2553, 1)), gem_string(";\n")));
#line 836 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 837 "compiler/main.gem"
    GemVal _t2554 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t2554, gem_string("setup")), gem_string("")))) {
#line 838 "compiler/main.gem"
    GemVal _t2555 = gem_v_rc_r;
    GemVal _t2556[] = {gem_table_get(_t2555, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                                gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t2556, 3));
                                                            }
#line 840 "compiler/main.gem"
    GemVal _t2557[] = {gem_v_t};
    GemVal _t2558 = gem_v_rc_r;
    GemVal _t2559[] = {gem_table_get(_t2558, gem_string("expr"))};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2557, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2559, 1)), gem_string(";\n")));
#line 841 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 842 "compiler/main.gem"
    GemVal _t2560 = gem_table_new();
    gem_table_set(_t2560, gem_string("expr"), gem_v_t);
    gem_table_set(_t2560, gem_string("setup"), gem_v_setup);
                                                            GemVal _t2561 = _t2560;
                                                            gem_pop_frame();
                                                            return _t2561;
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
#line 845 "compiler/main.gem"
    GemVal _t2562[] = {gem_v_op};
    GemVal _t2563[] = {gem_add(gem_string("unknown binary operator: "), gem_to_string_fn(NULL, _t2562, 1))};
    GemVal _t2564 = gem_error_at_fn("compiler/main.gem", 845, _t2563, 1);
    gem_pop_frame();
    return _t2564;
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
#line 851 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 852 "compiler/main.gem"
    GemVal gem_v__for_items_46 = gem_v_stmts;
#line 852 "compiler/main.gem"
    GemVal gem_v__for_i_46 = gem_int(0);
#line 852 "compiler/main.gem"
    while (1) {
        GemVal _t2566[] = {gem_v__for_items_46};
        if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_len_fn(NULL, _t2566, 1)))) break;
#line 852 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_46, gem_v__for_i_46);
#line 852 "compiler/main.gem"
        gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 853 "compiler/main.gem"
    GemVal _t2567[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2568 = (*gem_v_compile_stmt);
        gem_v_out = gem_add(gem_add(gem_v_out, _t2568.fn(_t2568.env, _t2567, 2)), gem_string("\n"));
    }

    GemVal _t2569 = gem_v_out;
    gem_pop_frame();
    return _t2569;
}

struct _closure__anon_41 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_if;
    GemVal *gem_v_compile_match;
    GemVal *gem_v_compile_receive_match;
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
    GemVal *gem_v_compile_receive_match = _cls->gem_v_compile_receive_match;
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
#line 861 "compiler/main.gem"
    GemVal _t2571[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2571, 1);
#line 862 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 863 "compiler/main.gem"
    GemVal _t2572 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2572, gem_string("line")), GEM_NIL))) {
#line 864 "compiler/main.gem"
    GemVal _t2573 = gem_v_node;
    GemVal _t2574[] = {gem_table_get(_t2573, gem_string("line"))};
    GemVal _t2575[] = {(*gem_v_source_name)};
    GemVal _t2576[] = {gem_fn_escape_c_string(NULL, _t2575, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2574, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2576, 1)), gem_string("\"\n"));
    }
#line 867 "compiler/main.gem"
    GemVal gem_v__match_47 = gem_v_node;
    GemVal _t2577[] = {gem_v__match_47};
    GemVal _t2579;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2577, 1), gem_string("table")))) {
        _t2579 = gem_eq(gem_type_fn(NULL, _t2577, 1), gem_string("table"));
    } else {
        GemVal _t2578[] = {gem_v__match_47, gem_string("tag")};
        _t2579 = gem_has_key_fn(NULL, _t2578, 2);
    }
    GemVal _t2580;
    if (!gem_truthy(_t2579)) {
        _t2580 = _t2579;
    } else {
        _t2580 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2582;
    if (!gem_truthy(_t2580)) {
        _t2582 = _t2580;
    } else {
        GemVal _t2581[] = {gem_v__match_47, gem_string("name")};
        _t2582 = gem_has_key_fn(NULL, _t2581, 2);
    }
    GemVal _t2584;
    if (!gem_truthy(_t2582)) {
        _t2584 = _t2582;
    } else {
        GemVal _t2583[] = {gem_v__match_47, gem_string("value")};
        _t2584 = gem_has_key_fn(NULL, _t2583, 2);
    }
    if (gem_truthy(_t2584)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_47, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_47, gem_string("value"));
#line 869 "compiler/main.gem"
    GemVal _t2585[] = {gem_v_value};
    GemVal _t2586 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2586.fn(_t2586.env, _t2585, 1);
#line 870 "compiler/main.gem"
    GemVal _t2587[] = {gem_v_name};
    GemVal _t2588 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2588.fn(_t2588.env, _t2587, 1);
#line 871 "compiler/main.gem"
    GemVal _t2590;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t2590 = (*gem_v_in_top_level);
    } else {
        GemVal _t2589[] = {(*gem_v_top_level_vars), gem_v_name};
        _t2590 = gem_fn_set_contains(NULL, _t2589, 2);
    }
        if (gem_truthy(_t2590)) {
#line 872 "compiler/main.gem"
    GemVal _t2591[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2591, 2))) {
#line 873 "compiler/main.gem"
    GemVal _t2592[] = {gem_v_line_dir};
    GemVal _t2593 = gem_v_r;
    GemVal _t2594[] = {gem_table_get(_t2593, gem_string("setup"))};
    GemVal _t2595[] = {gem_v_p};
    GemVal _t2596[] = {gem_v_mname};
    GemVal _t2597[] = {gem_v_p};
    GemVal _t2598[] = {gem_v_mname};
    GemVal _t2599 = gem_v_r;
    GemVal _t2600[] = {gem_table_get(_t2599, gem_string("expr"))};
                GemVal _t2601 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2592, 1), gem_to_string_fn(NULL, _t2594, 1)), gem_to_string_fn(NULL, _t2595, 1)), gem_to_string_fn(NULL, _t2596, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2597, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2598, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2600, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2601;
            }
#line 875 "compiler/main.gem"
    GemVal _t2602[] = {gem_v_line_dir};
    GemVal _t2603 = gem_v_r;
    GemVal _t2604[] = {gem_table_get(_t2603, gem_string("setup"))};
    GemVal _t2605[] = {gem_v_p};
    GemVal _t2606[] = {gem_v_mname};
    GemVal _t2607 = gem_v_r;
    GemVal _t2608[] = {gem_table_get(_t2607, gem_string("expr"))};
            GemVal _t2609 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2602, 1), gem_to_string_fn(NULL, _t2604, 1)), gem_to_string_fn(NULL, _t2605, 1)), gem_to_string_fn(NULL, _t2606, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2608, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2609;
        }
#line 877 "compiler/main.gem"
    GemVal _t2610[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2610, 2))) {
#line 878 "compiler/main.gem"
    GemVal _t2611[] = {gem_v_line_dir};
    GemVal _t2612 = gem_v_r;
    GemVal _t2613[] = {gem_table_get(_t2612, gem_string("setup"))};
    GemVal _t2614[] = {gem_v_p};
    GemVal _t2615[] = {gem_v_mname};
    GemVal _t2616[] = {gem_v_p};
    GemVal _t2617[] = {gem_v_mname};
    GemVal _t2618 = gem_v_r;
    GemVal _t2619[] = {gem_table_get(_t2618, gem_string("expr"))};
            GemVal _t2620 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2611, 1), gem_to_string_fn(NULL, _t2613, 1)), gem_to_string_fn(NULL, _t2614, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2615, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2616, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2617, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2619, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2620;
        }
#line 880 "compiler/main.gem"
    GemVal _t2621[] = {gem_v_line_dir};
    GemVal _t2622 = gem_v_r;
    GemVal _t2623[] = {gem_table_get(_t2622, gem_string("setup"))};
    GemVal _t2624[] = {gem_v_p};
    GemVal _t2625[] = {gem_v_mname};
    GemVal _t2626 = gem_v_r;
    GemVal _t2627[] = {gem_table_get(_t2626, gem_string("expr"))};
        GemVal _t2628 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2621, 1), gem_to_string_fn(NULL, _t2623, 1)), gem_to_string_fn(NULL, _t2624, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2625, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2627, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2628;
    } else {
    GemVal _t2629[] = {gem_v__match_47};
    GemVal _t2631;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2629, 1), gem_string("table")))) {
        _t2631 = gem_eq(gem_type_fn(NULL, _t2629, 1), gem_string("table"));
    } else {
        GemVal _t2630[] = {gem_v__match_47, gem_string("tag")};
        _t2631 = gem_has_key_fn(NULL, _t2630, 2);
    }
    GemVal _t2632;
    if (!gem_truthy(_t2631)) {
        _t2632 = _t2631;
    } else {
        _t2632 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2634;
    if (!gem_truthy(_t2632)) {
        _t2634 = _t2632;
    } else {
        GemVal _t2633[] = {gem_v__match_47, gem_string("name")};
        _t2634 = gem_has_key_fn(NULL, _t2633, 2);
    }
    GemVal _t2636;
    if (!gem_truthy(_t2634)) {
        _t2636 = _t2634;
    } else {
        GemVal _t2635[] = {gem_v__match_47, gem_string("value")};
        _t2636 = gem_has_key_fn(NULL, _t2635, 2);
    }
    if (gem_truthy(_t2636)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_47, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_47, gem_string("value"));
#line 882 "compiler/main.gem"
    GemVal _t2637[] = {gem_v_value};
    GemVal _t2638 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2638.fn(_t2638.env, _t2637, 1);
#line 883 "compiler/main.gem"
    GemVal _t2639[] = {gem_v_name};
    GemVal _t2640 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2640.fn(_t2640.env, _t2639, 1);
#line 884 "compiler/main.gem"
    GemVal _t2641[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2643;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2641, 2))) {
        _t2643 = gem_fn_set_contains(NULL, _t2641, 2);
    } else {
        GemVal _t2642[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2643 = gem_fn_set_contains(NULL, _t2642, 2);
    }
        if (gem_truthy(_t2643)) {
#line 885 "compiler/main.gem"
    GemVal _t2644[] = {gem_v_line_dir};
    GemVal _t2645 = gem_v_r;
    GemVal _t2646[] = {gem_table_get(_t2645, gem_string("setup"))};
    GemVal _t2647[] = {gem_v_p};
    GemVal _t2648[] = {gem_v_mname};
    GemVal _t2649 = gem_v_r;
    GemVal _t2650[] = {gem_table_get(_t2649, gem_string("expr"))};
            GemVal _t2651 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2644, 1), gem_to_string_fn(NULL, _t2646, 1)), gem_to_string_fn(NULL, _t2647, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2648, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2650, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2651;
        }
#line 887 "compiler/main.gem"
    GemVal _t2652[] = {gem_v_line_dir};
    GemVal _t2653 = gem_v_r;
    GemVal _t2654[] = {gem_table_get(_t2653, gem_string("setup"))};
    GemVal _t2655[] = {gem_v_p};
    GemVal _t2656[] = {gem_v_mname};
    GemVal _t2657 = gem_v_r;
    GemVal _t2658[] = {gem_table_get(_t2657, gem_string("expr"))};
        GemVal _t2659 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2652, 1), gem_to_string_fn(NULL, _t2654, 1)), gem_to_string_fn(NULL, _t2655, 1)), gem_to_string_fn(NULL, _t2656, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2658, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2659;
    } else {
    GemVal _t2660[] = {gem_v__match_47};
    GemVal _t2662;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2660, 1), gem_string("table")))) {
        _t2662 = gem_eq(gem_type_fn(NULL, _t2660, 1), gem_string("table"));
    } else {
        GemVal _t2661[] = {gem_v__match_47, gem_string("tag")};
        _t2662 = gem_has_key_fn(NULL, _t2661, 2);
    }
    GemVal _t2663;
    if (!gem_truthy(_t2662)) {
        _t2663 = _t2662;
    } else {
        _t2663 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t2665;
    if (!gem_truthy(_t2663)) {
        _t2665 = _t2663;
    } else {
        GemVal _t2664[] = {gem_v__match_47, gem_string("object")};
        _t2665 = gem_has_key_fn(NULL, _t2664, 2);
    }
    GemVal _t2667;
    if (!gem_truthy(_t2665)) {
        _t2667 = _t2665;
    } else {
        GemVal _t2666[] = {gem_v__match_47, gem_string("value")};
        _t2667 = gem_has_key_fn(NULL, _t2666, 2);
    }
    GemVal _t2669;
    if (!gem_truthy(_t2667)) {
        _t2669 = _t2667;
    } else {
        GemVal _t2668[] = {gem_v__match_47, gem_string("field")};
        _t2669 = gem_has_key_fn(NULL, _t2668, 2);
    }
    if (gem_truthy(_t2669)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_47, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_47, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_47, gem_string("field"));
#line 889 "compiler/main.gem"
    GemVal _t2670[] = {gem_v_object};
    GemVal _t2671 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2671.fn(_t2671.env, _t2670, 1);
#line 890 "compiler/main.gem"
    GemVal _t2672[] = {gem_v_value};
    GemVal _t2673 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2673.fn(_t2673.env, _t2672, 1);
#line 891 "compiler/main.gem"
    GemVal _t2674[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2674, 1);
#line 892 "compiler/main.gem"
    GemVal _t2675[] = {gem_v_line_dir};
    GemVal _t2676 = gem_v_obj_r;
    GemVal _t2677[] = {gem_table_get(_t2676, gem_string("setup"))};
    GemVal _t2678 = gem_v_val_r;
    GemVal _t2679[] = {gem_table_get(_t2678, gem_string("setup"))};
    GemVal _t2680[] = {gem_v_p};
    GemVal _t2681 = gem_v_obj_r;
    GemVal _t2682[] = {gem_table_get(_t2681, gem_string("expr"))};
    GemVal _t2683[] = {gem_v_escaped};
    GemVal _t2684 = gem_v_val_r;
    GemVal _t2685[] = {gem_table_get(_t2684, gem_string("expr"))};
        GemVal _t2686 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2675, 1), gem_to_string_fn(NULL, _t2677, 1)), gem_to_string_fn(NULL, _t2679, 1)), gem_to_string_fn(NULL, _t2680, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2682, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2683, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2685, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2686;
    } else {
    GemVal _t2687[] = {gem_v__match_47};
    GemVal _t2689;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2687, 1), gem_string("table")))) {
        _t2689 = gem_eq(gem_type_fn(NULL, _t2687, 1), gem_string("table"));
    } else {
        GemVal _t2688[] = {gem_v__match_47, gem_string("tag")};
        _t2689 = gem_has_key_fn(NULL, _t2688, 2);
    }
    GemVal _t2690;
    if (!gem_truthy(_t2689)) {
        _t2690 = _t2689;
    } else {
        _t2690 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t2692;
    if (!gem_truthy(_t2690)) {
        _t2692 = _t2690;
    } else {
        GemVal _t2691[] = {gem_v__match_47, gem_string("object")};
        _t2692 = gem_has_key_fn(NULL, _t2691, 2);
    }
    GemVal _t2694;
    if (!gem_truthy(_t2692)) {
        _t2694 = _t2692;
    } else {
        GemVal _t2693[] = {gem_v__match_47, gem_string("key")};
        _t2694 = gem_has_key_fn(NULL, _t2693, 2);
    }
    GemVal _t2696;
    if (!gem_truthy(_t2694)) {
        _t2696 = _t2694;
    } else {
        GemVal _t2695[] = {gem_v__match_47, gem_string("value")};
        _t2696 = gem_has_key_fn(NULL, _t2695, 2);
    }
    if (gem_truthy(_t2696)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_47, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_47, gem_string("key"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_47, gem_string("value"));
#line 894 "compiler/main.gem"
    GemVal _t2697[] = {gem_v_object};
    GemVal _t2698 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2698.fn(_t2698.env, _t2697, 1);
#line 895 "compiler/main.gem"
    GemVal _t2699[] = {gem_v_key};
    GemVal _t2700 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2700.fn(_t2700.env, _t2699, 1);
#line 896 "compiler/main.gem"
    GemVal _t2701[] = {gem_v_value};
    GemVal _t2702 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2702.fn(_t2702.env, _t2701, 1);
#line 897 "compiler/main.gem"
    GemVal _t2703[] = {gem_v_line_dir};
    GemVal _t2704 = gem_v_obj_r;
    GemVal _t2705[] = {gem_table_get(_t2704, gem_string("setup"))};
    GemVal _t2706 = gem_v_key_r;
    GemVal _t2707[] = {gem_table_get(_t2706, gem_string("setup"))};
    GemVal _t2708 = gem_v_val_r;
    GemVal _t2709[] = {gem_table_get(_t2708, gem_string("setup"))};
    GemVal _t2710[] = {gem_v_p};
    GemVal _t2711 = gem_v_obj_r;
    GemVal _t2712[] = {gem_table_get(_t2711, gem_string("expr"))};
    GemVal _t2713 = gem_v_key_r;
    GemVal _t2714[] = {gem_table_get(_t2713, gem_string("expr"))};
    GemVal _t2715 = gem_v_val_r;
    GemVal _t2716[] = {gem_table_get(_t2715, gem_string("expr"))};
        GemVal _t2717 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2703, 1), gem_to_string_fn(NULL, _t2705, 1)), gem_to_string_fn(NULL, _t2707, 1)), gem_to_string_fn(NULL, _t2709, 1)), gem_to_string_fn(NULL, _t2710, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2712, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2714, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2716, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2717;
    } else {
    GemVal _t2718[] = {gem_v__match_47};
    GemVal _t2720;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2718, 1), gem_string("table")))) {
        _t2720 = gem_eq(gem_type_fn(NULL, _t2718, 1), gem_string("table"));
    } else {
        GemVal _t2719[] = {gem_v__match_47, gem_string("tag")};
        _t2720 = gem_has_key_fn(NULL, _t2719, 2);
    }
    GemVal _t2721;
    if (!gem_truthy(_t2720)) {
        _t2721 = _t2720;
    } else {
        _t2721 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t2721)) {
#line 899 "compiler/main.gem"
    GemVal _t2722[] = {gem_v_line_dir};
    GemVal _t2723[] = {gem_v_node, gem_v_indent};
    GemVal _t2724 = (*gem_v_compile_if);
        GemVal _t2725 = gem_add(gem_to_string_fn(NULL, _t2722, 1), _t2724.fn(_t2724.env, _t2723, 2));
        gem_pop_frame();
        return _t2725;
    } else {
    GemVal _t2726[] = {gem_v__match_47};
    GemVal _t2728;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2726, 1), gem_string("table")))) {
        _t2728 = gem_eq(gem_type_fn(NULL, _t2726, 1), gem_string("table"));
    } else {
        GemVal _t2727[] = {gem_v__match_47, gem_string("tag")};
        _t2728 = gem_has_key_fn(NULL, _t2727, 2);
    }
    GemVal _t2729;
    if (!gem_truthy(_t2728)) {
        _t2729 = _t2728;
    } else {
        _t2729 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t2729)) {
#line 901 "compiler/main.gem"
    GemVal _t2730[] = {gem_v_line_dir};
    GemVal _t2731[] = {gem_v_node, gem_v_indent};
    GemVal _t2732 = (*gem_v_compile_while);
        GemVal _t2733 = gem_add(gem_to_string_fn(NULL, _t2730, 1), _t2732.fn(_t2732.env, _t2731, 2));
        gem_pop_frame();
        return _t2733;
    } else {
    GemVal _t2734[] = {gem_v__match_47};
    GemVal _t2736;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2734, 1), gem_string("table")))) {
        _t2736 = gem_eq(gem_type_fn(NULL, _t2734, 1), gem_string("table"));
    } else {
        GemVal _t2735[] = {gem_v__match_47, gem_string("tag")};
        _t2736 = gem_has_key_fn(NULL, _t2735, 2);
    }
    GemVal _t2737;
    if (!gem_truthy(_t2736)) {
        _t2737 = _t2736;
    } else {
        _t2737 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t2737)) {
#line 903 "compiler/main.gem"
    GemVal _t2738[] = {gem_v_line_dir};
    GemVal _t2739[] = {gem_v_node, gem_v_indent};
    GemVal _t2740 = (*gem_v_compile_match);
        GemVal _t2741 = gem_add(gem_to_string_fn(NULL, _t2738, 1), _t2740.fn(_t2740.env, _t2739, 2));
        gem_pop_frame();
        return _t2741;
    } else {
    GemVal _t2742[] = {gem_v__match_47};
    GemVal _t2744;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2742, 1), gem_string("table")))) {
        _t2744 = gem_eq(gem_type_fn(NULL, _t2742, 1), gem_string("table"));
    } else {
        GemVal _t2743[] = {gem_v__match_47, gem_string("tag")};
        _t2744 = gem_has_key_fn(NULL, _t2743, 2);
    }
    GemVal _t2745;
    if (!gem_truthy(_t2744)) {
        _t2745 = _t2744;
    } else {
        _t2745 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t2745)) {
#line 905 "compiler/main.gem"
    GemVal _t2746[] = {gem_v_line_dir};
    GemVal _t2747[] = {gem_v_node, gem_v_indent};
    GemVal _t2748 = (*gem_v_compile_receive_match);
        GemVal _t2749 = gem_add(gem_to_string_fn(NULL, _t2746, 1), _t2748.fn(_t2748.env, _t2747, 2));
        gem_pop_frame();
        return _t2749;
    } else {
    GemVal _t2750[] = {gem_v__match_47};
    GemVal _t2752;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2750, 1), gem_string("table")))) {
        _t2752 = gem_eq(gem_type_fn(NULL, _t2750, 1), gem_string("table"));
    } else {
        GemVal _t2751[] = {gem_v__match_47, gem_string("tag")};
        _t2752 = gem_has_key_fn(NULL, _t2751, 2);
    }
    GemVal _t2753;
    if (!gem_truthy(_t2752)) {
        _t2753 = _t2752;
    } else {
        _t2753 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t2753)) {
#line 907 "compiler/main.gem"
        GemVal _t2754 = gem_add(gem_v_p, gem_string("break;"));
        gem_pop_frame();
        return _t2754;
    } else {
    GemVal _t2755[] = {gem_v__match_47};
    GemVal _t2757;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2755, 1), gem_string("table")))) {
        _t2757 = gem_eq(gem_type_fn(NULL, _t2755, 1), gem_string("table"));
    } else {
        GemVal _t2756[] = {gem_v__match_47, gem_string("tag")};
        _t2757 = gem_has_key_fn(NULL, _t2756, 2);
    }
    GemVal _t2758;
    if (!gem_truthy(_t2757)) {
        _t2758 = _t2757;
    } else {
        _t2758 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t2758)) {
#line 909 "compiler/main.gem"
        GemVal _t2759 = gem_add(gem_v_p, gem_string("continue;"));
        gem_pop_frame();
        return _t2759;
    } else {
    GemVal _t2760[] = {gem_v__match_47};
    GemVal _t2762;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2760, 1), gem_string("table")))) {
        _t2762 = gem_eq(gem_type_fn(NULL, _t2760, 1), gem_string("table"));
    } else {
        GemVal _t2761[] = {gem_v__match_47, gem_string("tag")};
        _t2762 = gem_has_key_fn(NULL, _t2761, 2);
    }
    GemVal _t2763;
    if (!gem_truthy(_t2762)) {
        _t2763 = _t2762;
    } else {
        _t2763 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2765;
    if (!gem_truthy(_t2763)) {
        _t2765 = _t2763;
    } else {
        GemVal _t2764[] = {gem_v__match_47, gem_string("value")};
        _t2765 = gem_has_key_fn(NULL, _t2764, 2);
    }
    if (gem_truthy(_t2765)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_47, gem_string("value"));
#line 911 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 912 "compiler/main.gem"
    GemVal _t2766[] = {gem_v_value};
    GemVal _t2767 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2767.fn(_t2767.env, _t2766, 1);
#line 913 "compiler/main.gem"
    GemVal _t2768 = (*gem_v_tmp);
            GemVal gem_v_t = _t2768.fn(_t2768.env, NULL, 0);
#line 914 "compiler/main.gem"
    GemVal _t2769[] = {gem_v_line_dir};
    GemVal _t2770 = gem_v_r;
    GemVal _t2771[] = {gem_table_get(_t2770, gem_string("setup"))};
    GemVal _t2772[] = {gem_v_p};
    GemVal _t2773[] = {gem_v_t};
    GemVal _t2774 = gem_v_r;
    GemVal _t2775[] = {gem_table_get(_t2774, gem_string("expr"))};
    GemVal _t2776[] = {gem_v_p};
    GemVal _t2777[] = {gem_v_p};
    GemVal _t2778[] = {gem_v_t};
            GemVal _t2779 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2769, 1), gem_to_string_fn(NULL, _t2771, 1)), gem_to_string_fn(NULL, _t2772, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2773, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2775, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2776, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2777, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2778, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2779;
        }
#line 916 "compiler/main.gem"
    GemVal _t2780[] = {gem_v_line_dir};
    GemVal _t2781[] = {gem_v_p};
    GemVal _t2782[] = {gem_v_p};
        GemVal _t2783 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2780, 1), gem_to_string_fn(NULL, _t2781, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2782, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2783;
    } else {
    GemVal _t2784[] = {gem_v__match_47};
    GemVal _t2786;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2784, 1), gem_string("table")))) {
        _t2786 = gem_eq(gem_type_fn(NULL, _t2784, 1), gem_string("table"));
    } else {
        GemVal _t2785[] = {gem_v__match_47, gem_string("tag")};
        _t2786 = gem_has_key_fn(NULL, _t2785, 2);
    }
    GemVal _t2787;
    if (!gem_truthy(_t2786)) {
        _t2787 = _t2786;
    } else {
        _t2787 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2787)) {
#line 918 "compiler/main.gem"
    GemVal _t2788[] = {gem_v_node};
    GemVal _t2789 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2789.fn(_t2789.env, _t2788, 1);
#line 919 "compiler/main.gem"
    GemVal _t2790[] = {gem_v_line_dir};
    GemVal _t2791 = gem_v_r;
    GemVal _t2792[] = {gem_table_get(_t2791, gem_string("setup"))};
    GemVal _t2793[] = {gem_v_p};
    GemVal _t2794 = gem_v_r;
    GemVal _t2795[] = {gem_table_get(_t2794, gem_string("expr"))};
        GemVal _t2796 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2790, 1), gem_to_string_fn(NULL, _t2792, 1)), gem_to_string_fn(NULL, _t2793, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2795, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2796;
    } else {
    GemVal _t2797[] = {gem_v__match_47};
    GemVal _t2799;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2797, 1), gem_string("table")))) {
        _t2799 = gem_eq(gem_type_fn(NULL, _t2797, 1), gem_string("table"));
    } else {
        GemVal _t2798[] = {gem_v__match_47, gem_string("tag")};
        _t2799 = gem_has_key_fn(NULL, _t2798, 2);
    }
    GemVal _t2800;
    if (!gem_truthy(_t2799)) {
        _t2800 = _t2799;
    } else {
        _t2800 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t2800)) {
#line 921 "compiler/main.gem"
        GemVal _t2801 = gem_string("");
        gem_pop_frame();
        return _t2801;
    } else {
    GemVal _t2802[] = {gem_v__match_47};
    GemVal _t2804;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2802, 1), gem_string("table")))) {
        _t2804 = gem_eq(gem_type_fn(NULL, _t2802, 1), gem_string("table"));
    } else {
        GemVal _t2803[] = {gem_v__match_47, gem_string("tag")};
        _t2804 = gem_has_key_fn(NULL, _t2803, 2);
    }
    GemVal _t2805;
    if (!gem_truthy(_t2804)) {
        _t2805 = _t2804;
    } else {
        _t2805 = gem_eq(gem_table_get(gem_v__match_47, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2807;
    if (!gem_truthy(_t2805)) {
        _t2807 = _t2805;
    } else {
        GemVal _t2806[] = {gem_v__match_47, gem_string("stmts")};
        _t2807 = gem_has_key_fn(NULL, _t2806, 2);
    }
    if (gem_truthy(_t2807)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_47, gem_string("stmts"));
#line 923 "compiler/main.gem"
        GemVal gem_v_out = gem_string("");
#line 924 "compiler/main.gem"
        GemVal gem_v__for_items_48 = gem_v_stmts;
#line 924 "compiler/main.gem"
        GemVal gem_v__for_i_48 = gem_int(0);
#line 924 "compiler/main.gem"
        while (1) {
            GemVal _t2808[] = {gem_v__for_items_48};
            if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_len_fn(NULL, _t2808, 1)))) break;
#line 924 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_48, gem_v__for_i_48);
#line 924 "compiler/main.gem"
            gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 925 "compiler/main.gem"
    GemVal _t2809[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2810 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2810.fn(_t2810.env, _t2809, 2)), gem_string("\n"));
        }

#line 927 "compiler/main.gem"
        GemVal _t2811 = gem_v_out;
        gem_pop_frame();
        return _t2811;
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
#line 930 "compiler/main.gem"
    GemVal _t2812[] = {gem_v_node};
    GemVal _t2813 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t2813.fn(_t2813.env, _t2812, 1);
    GemVal _t2814[] = {gem_v_line_dir};
    GemVal _t2815 = gem_v_r;
    GemVal _t2816[] = {gem_table_get(_t2815, gem_string("setup"))};
    GemVal _t2817[] = {gem_v_p};
    GemVal _t2818 = gem_v_r;
    GemVal _t2819[] = {gem_table_get(_t2818, gem_string("expr"))};
    GemVal _t2820 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2814, 1), gem_to_string_fn(NULL, _t2816, 1)), gem_to_string_fn(NULL, _t2817, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2819, 1)), gem_string(");"));
    gem_pop_frame();
    return _t2820;
}

struct _closure__anon_42 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_if_return;
    GemVal *gem_v_compile_match_return;
    GemVal *gem_v_compile_receive_match_return;
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
    GemVal *gem_v_compile_receive_match_return = _cls->gem_v_compile_receive_match_return;
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
#line 937 "compiler/main.gem"
    GemVal _t2822[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2822, 1);
#line 938 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 939 "compiler/main.gem"
    GemVal _t2823 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2823, gem_string("line")), GEM_NIL))) {
#line 940 "compiler/main.gem"
    GemVal _t2824 = gem_v_node;
    GemVal _t2825[] = {gem_table_get(_t2824, gem_string("line"))};
    GemVal _t2826[] = {(*gem_v_source_name)};
    GemVal _t2827[] = {gem_fn_escape_c_string(NULL, _t2826, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2825, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2827, 1)), gem_string("\"\n"));
    }
#line 943 "compiler/main.gem"
    GemVal gem_v__match_49 = gem_v_node;
    GemVal _t2828[] = {gem_v__match_49};
    GemVal _t2830;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2828, 1), gem_string("table")))) {
        _t2830 = gem_eq(gem_type_fn(NULL, _t2828, 1), gem_string("table"));
    } else {
        GemVal _t2829[] = {gem_v__match_49, gem_string("tag")};
        _t2830 = gem_has_key_fn(NULL, _t2829, 2);
    }
    GemVal _t2831;
    if (!gem_truthy(_t2830)) {
        _t2831 = _t2830;
    } else {
        _t2831 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2833;
    if (!gem_truthy(_t2831)) {
        _t2833 = _t2831;
    } else {
        GemVal _t2832[] = {gem_v__match_49, gem_string("name")};
        _t2833 = gem_has_key_fn(NULL, _t2832, 2);
    }
    GemVal _t2835;
    if (!gem_truthy(_t2833)) {
        _t2835 = _t2833;
    } else {
        GemVal _t2834[] = {gem_v__match_49, gem_string("value")};
        _t2835 = gem_has_key_fn(NULL, _t2834, 2);
    }
    if (gem_truthy(_t2835)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_49, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_49, gem_string("value"));
#line 945 "compiler/main.gem"
    GemVal _t2836[] = {gem_v_value};
    GemVal _t2837 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2837.fn(_t2837.env, _t2836, 1);
#line 946 "compiler/main.gem"
    GemVal _t2838[] = {gem_v_name};
    GemVal _t2839 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2839.fn(_t2839.env, _t2838, 1);
#line 947 "compiler/main.gem"
    GemVal _t2840[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2840, 2))) {
#line 948 "compiler/main.gem"
    GemVal _t2841[] = {gem_v_line_dir};
    GemVal _t2842 = gem_v_r;
    GemVal _t2843[] = {gem_table_get(_t2842, gem_string("setup"))};
    GemVal _t2844[] = {gem_v_p};
    GemVal _t2845[] = {gem_v_mname};
    GemVal _t2846[] = {gem_v_p};
    GemVal _t2847[] = {gem_v_mname};
    GemVal _t2848 = gem_v_r;
    GemVal _t2849[] = {gem_table_get(_t2848, gem_string("expr"))};
    GemVal _t2850[] = {gem_v_p};
    GemVal _t2851[] = {gem_v_p};
            GemVal _t2852 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2841, 1), gem_to_string_fn(NULL, _t2843, 1)), gem_to_string_fn(NULL, _t2844, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2845, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2846, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2847, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2849, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2850, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2851, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t2852;
        }
#line 950 "compiler/main.gem"
    GemVal _t2853[] = {gem_v_line_dir};
    GemVal _t2854 = gem_v_r;
    GemVal _t2855[] = {gem_table_get(_t2854, gem_string("setup"))};
    GemVal _t2856[] = {gem_v_p};
    GemVal _t2857[] = {gem_v_mname};
    GemVal _t2858 = gem_v_r;
    GemVal _t2859[] = {gem_table_get(_t2858, gem_string("expr"))};
    GemVal _t2860[] = {gem_v_p};
    GemVal _t2861[] = {gem_v_p};
        GemVal _t2862 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2853, 1), gem_to_string_fn(NULL, _t2855, 1)), gem_to_string_fn(NULL, _t2856, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2857, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2859, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2860, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2861, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2862;
    } else {
    GemVal _t2863[] = {gem_v__match_49};
    GemVal _t2865;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2863, 1), gem_string("table")))) {
        _t2865 = gem_eq(gem_type_fn(NULL, _t2863, 1), gem_string("table"));
    } else {
        GemVal _t2864[] = {gem_v__match_49, gem_string("tag")};
        _t2865 = gem_has_key_fn(NULL, _t2864, 2);
    }
    GemVal _t2866;
    if (!gem_truthy(_t2865)) {
        _t2866 = _t2865;
    } else {
        _t2866 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2868;
    if (!gem_truthy(_t2866)) {
        _t2868 = _t2866;
    } else {
        GemVal _t2867[] = {gem_v__match_49, gem_string("name")};
        _t2868 = gem_has_key_fn(NULL, _t2867, 2);
    }
    GemVal _t2870;
    if (!gem_truthy(_t2868)) {
        _t2870 = _t2868;
    } else {
        GemVal _t2869[] = {gem_v__match_49, gem_string("value")};
        _t2870 = gem_has_key_fn(NULL, _t2869, 2);
    }
    if (gem_truthy(_t2870)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_49, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_49, gem_string("value"));
#line 952 "compiler/main.gem"
    GemVal _t2871[] = {gem_v_value};
    GemVal _t2872 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2872.fn(_t2872.env, _t2871, 1);
#line 953 "compiler/main.gem"
    GemVal _t2873 = (*gem_v_tmp);
        GemVal gem_v_t = _t2873.fn(_t2873.env, NULL, 0);
#line 954 "compiler/main.gem"
    GemVal _t2874[] = {gem_v_name};
    GemVal _t2875 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2875.fn(_t2875.env, _t2874, 1);
#line 955 "compiler/main.gem"
    GemVal _t2876[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2878;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2876, 2))) {
        _t2878 = gem_fn_set_contains(NULL, _t2876, 2);
    } else {
        GemVal _t2877[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2878 = gem_fn_set_contains(NULL, _t2877, 2);
    }
        if (gem_truthy(_t2878)) {
#line 956 "compiler/main.gem"
    GemVal _t2879[] = {gem_v_line_dir};
    GemVal _t2880 = gem_v_r;
    GemVal _t2881[] = {gem_table_get(_t2880, gem_string("setup"))};
    GemVal _t2882[] = {gem_v_p};
    GemVal _t2883[] = {gem_v_t};
    GemVal _t2884 = gem_v_r;
    GemVal _t2885[] = {gem_table_get(_t2884, gem_string("expr"))};
    GemVal _t2886[] = {gem_v_p};
    GemVal _t2887[] = {gem_v_mname};
    GemVal _t2888[] = {gem_v_t};
    GemVal _t2889[] = {gem_v_p};
    GemVal _t2890[] = {gem_v_p};
    GemVal _t2891[] = {gem_v_t};
            GemVal _t2892 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2879, 1), gem_to_string_fn(NULL, _t2881, 1)), gem_to_string_fn(NULL, _t2882, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2883, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2885, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2886, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2887, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2888, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2889, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2890, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2891, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2892;
        }
#line 958 "compiler/main.gem"
    GemVal _t2893[] = {gem_v_line_dir};
    GemVal _t2894 = gem_v_r;
    GemVal _t2895[] = {gem_table_get(_t2894, gem_string("setup"))};
    GemVal _t2896[] = {gem_v_p};
    GemVal _t2897[] = {gem_v_t};
    GemVal _t2898 = gem_v_r;
    GemVal _t2899[] = {gem_table_get(_t2898, gem_string("expr"))};
    GemVal _t2900[] = {gem_v_p};
    GemVal _t2901[] = {gem_v_mname};
    GemVal _t2902[] = {gem_v_t};
    GemVal _t2903[] = {gem_v_p};
    GemVal _t2904[] = {gem_v_p};
    GemVal _t2905[] = {gem_v_t};
        GemVal _t2906 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2893, 1), gem_to_string_fn(NULL, _t2895, 1)), gem_to_string_fn(NULL, _t2896, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2897, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2899, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2900, 1)), gem_to_string_fn(NULL, _t2901, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2902, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2903, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2904, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2905, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2906;
    } else {
    GemVal _t2907[] = {gem_v__match_49};
    GemVal _t2909;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2907, 1), gem_string("table")))) {
        _t2909 = gem_eq(gem_type_fn(NULL, _t2907, 1), gem_string("table"));
    } else {
        GemVal _t2908[] = {gem_v__match_49, gem_string("tag")};
        _t2909 = gem_has_key_fn(NULL, _t2908, 2);
    }
    GemVal _t2910;
    if (!gem_truthy(_t2909)) {
        _t2910 = _t2909;
    } else {
        _t2910 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2912;
    if (!gem_truthy(_t2910)) {
        _t2912 = _t2910;
    } else {
        GemVal _t2911[] = {gem_v__match_49, gem_string("value")};
        _t2912 = gem_has_key_fn(NULL, _t2911, 2);
    }
    if (gem_truthy(_t2912)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_49, gem_string("value"));
#line 960 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 961 "compiler/main.gem"
    GemVal _t2913[] = {gem_v_value};
    GemVal _t2914 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2914.fn(_t2914.env, _t2913, 1);
#line 962 "compiler/main.gem"
    GemVal _t2915 = (*gem_v_tmp);
            GemVal gem_v_t = _t2915.fn(_t2915.env, NULL, 0);
#line 963 "compiler/main.gem"
    GemVal _t2916[] = {gem_v_line_dir};
    GemVal _t2917 = gem_v_r;
    GemVal _t2918[] = {gem_table_get(_t2917, gem_string("setup"))};
    GemVal _t2919[] = {gem_v_p};
    GemVal _t2920[] = {gem_v_t};
    GemVal _t2921 = gem_v_r;
    GemVal _t2922[] = {gem_table_get(_t2921, gem_string("expr"))};
    GemVal _t2923[] = {gem_v_p};
    GemVal _t2924[] = {gem_v_p};
    GemVal _t2925[] = {gem_v_t};
            GemVal _t2926 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2916, 1), gem_to_string_fn(NULL, _t2918, 1)), gem_to_string_fn(NULL, _t2919, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2920, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2922, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2923, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2924, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2925, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2926;
        }
#line 965 "compiler/main.gem"
    GemVal _t2927[] = {gem_v_line_dir};
    GemVal _t2928[] = {gem_v_p};
    GemVal _t2929[] = {gem_v_p};
        GemVal _t2930 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2927, 1), gem_to_string_fn(NULL, _t2928, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2929, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2930;
    } else {
    GemVal _t2931[] = {gem_v__match_49};
    GemVal _t2933;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2931, 1), gem_string("table")))) {
        _t2933 = gem_eq(gem_type_fn(NULL, _t2931, 1), gem_string("table"));
    } else {
        GemVal _t2932[] = {gem_v__match_49, gem_string("tag")};
        _t2933 = gem_has_key_fn(NULL, _t2932, 2);
    }
    GemVal _t2934;
    if (!gem_truthy(_t2933)) {
        _t2934 = _t2933;
    } else {
        _t2934 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t2934)) {
#line 967 "compiler/main.gem"
    GemVal _t2935[] = {gem_v_line_dir};
    GemVal _t2936[] = {gem_v_node, gem_v_indent};
    GemVal _t2937 = (*gem_v_compile_if_return);
        GemVal _t2938 = gem_add(gem_to_string_fn(NULL, _t2935, 1), _t2937.fn(_t2937.env, _t2936, 2));
        gem_pop_frame();
        return _t2938;
    } else {
    GemVal _t2939[] = {gem_v__match_49};
    GemVal _t2941;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2939, 1), gem_string("table")))) {
        _t2941 = gem_eq(gem_type_fn(NULL, _t2939, 1), gem_string("table"));
    } else {
        GemVal _t2940[] = {gem_v__match_49, gem_string("tag")};
        _t2941 = gem_has_key_fn(NULL, _t2940, 2);
    }
    GemVal _t2942;
    if (!gem_truthy(_t2941)) {
        _t2942 = _t2941;
    } else {
        _t2942 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t2942)) {
#line 969 "compiler/main.gem"
    GemVal _t2943[] = {gem_v_line_dir};
    GemVal _t2944[] = {gem_v_node, gem_v_indent};
    GemVal _t2945 = (*gem_v_compile_while);
    GemVal _t2946[] = {_t2945.fn(_t2945.env, _t2944, 2)};
    GemVal _t2947[] = {gem_v_p};
    GemVal _t2948[] = {gem_v_p};
        GemVal _t2949 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2943, 1), gem_to_string_fn(NULL, _t2946, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t2947, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2948, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2949;
    } else {
    GemVal _t2950[] = {gem_v__match_49};
    GemVal _t2952;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2950, 1), gem_string("table")))) {
        _t2952 = gem_eq(gem_type_fn(NULL, _t2950, 1), gem_string("table"));
    } else {
        GemVal _t2951[] = {gem_v__match_49, gem_string("tag")};
        _t2952 = gem_has_key_fn(NULL, _t2951, 2);
    }
    GemVal _t2953;
    if (!gem_truthy(_t2952)) {
        _t2953 = _t2952;
    } else {
        _t2953 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t2953)) {
#line 971 "compiler/main.gem"
    GemVal _t2954[] = {gem_v_line_dir};
    GemVal _t2955[] = {gem_v_node, gem_v_indent};
    GemVal _t2956 = (*gem_v_compile_match_return);
        GemVal _t2957 = gem_add(gem_to_string_fn(NULL, _t2954, 1), _t2956.fn(_t2956.env, _t2955, 2));
        gem_pop_frame();
        return _t2957;
    } else {
    GemVal _t2958[] = {gem_v__match_49};
    GemVal _t2960;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2958, 1), gem_string("table")))) {
        _t2960 = gem_eq(gem_type_fn(NULL, _t2958, 1), gem_string("table"));
    } else {
        GemVal _t2959[] = {gem_v__match_49, gem_string("tag")};
        _t2960 = gem_has_key_fn(NULL, _t2959, 2);
    }
    GemVal _t2961;
    if (!gem_truthy(_t2960)) {
        _t2961 = _t2960;
    } else {
        _t2961 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t2961)) {
#line 973 "compiler/main.gem"
    GemVal _t2962[] = {gem_v_line_dir};
    GemVal _t2963[] = {gem_v_node, gem_v_indent};
    GemVal _t2964 = (*gem_v_compile_receive_match_return);
        GemVal _t2965 = gem_add(gem_to_string_fn(NULL, _t2962, 1), _t2964.fn(_t2964.env, _t2963, 2));
        gem_pop_frame();
        return _t2965;
    } else {
    GemVal _t2966[] = {gem_v__match_49};
    GemVal _t2968;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2966, 1), gem_string("table")))) {
        _t2968 = gem_eq(gem_type_fn(NULL, _t2966, 1), gem_string("table"));
    } else {
        GemVal _t2967[] = {gem_v__match_49, gem_string("tag")};
        _t2968 = gem_has_key_fn(NULL, _t2967, 2);
    }
    GemVal _t2969;
    if (!gem_truthy(_t2968)) {
        _t2969 = _t2968;
    } else {
        _t2969 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2971;
    if (!gem_truthy(_t2969)) {
        _t2971 = _t2969;
    } else {
        GemVal _t2970[] = {gem_v__match_49, gem_string("stmts")};
        _t2971 = gem_has_key_fn(NULL, _t2970, 2);
    }
    if (gem_truthy(_t2971)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_49, gem_string("stmts"));
#line 975 "compiler/main.gem"
    GemVal _t2972[] = {gem_v_p};
        GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t2972, 1), gem_string("{\n"));
#line 976 "compiler/main.gem"
        GemVal gem_v__for_i_50 = gem_int(0);
#line 976 "compiler/main.gem"
    GemVal _t2973[] = {gem_v_stmts};
        GemVal gem_v__for_limit_50 = gem_sub(gem_len_fn(NULL, _t2973, 1), gem_int(1));
#line 976 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
#line 976 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_50;
#line 976 "compiler/main.gem"
            gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 977 "compiler/main.gem"
    GemVal _t2974[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2975 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2975.fn(_t2975.env, _t2974, 2)), gem_string("\n"));
        }

#line 979 "compiler/main.gem"
    GemVal _t2976[] = {gem_v_stmts};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2976, 1), gem_int(0)))) {
#line 980 "compiler/main.gem"
    GemVal _t2977[] = {gem_v_stmts};
    GemVal _t2978[] = {gem_table_get(gem_v_stmts, gem_sub(gem_len_fn(NULL, _t2977, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t2979 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2979.fn(_t2979.env, _t2978, 2)), gem_string("\n"));
        }
#line 982 "compiler/main.gem"
    GemVal _t2980[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t2980, 1), gem_string("}")));
#line 983 "compiler/main.gem"
        GemVal _t2981 = gem_v_out;
        gem_pop_frame();
        return _t2981;
    } else {
    GemVal _t2982[] = {gem_v__match_49};
    GemVal _t2984;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2982, 1), gem_string("table")))) {
        _t2984 = gem_eq(gem_type_fn(NULL, _t2982, 1), gem_string("table"));
    } else {
        GemVal _t2983[] = {gem_v__match_49, gem_string("tag")};
        _t2984 = gem_has_key_fn(NULL, _t2983, 2);
    }
    GemVal _t2985;
    if (!gem_truthy(_t2984)) {
        _t2985 = _t2984;
    } else {
        _t2985 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("dot_assign"));
    }
    if (gem_truthy(_t2985)) {
#line 985 "compiler/main.gem"
    GemVal _t2986[] = {gem_v_node, gem_v_indent};
    GemVal _t2987 = (*gem_v_compile_stmt);
    GemVal _t2988[] = {_t2987.fn(_t2987.env, _t2986, 2)};
    GemVal _t2989[] = {gem_v_p};
    GemVal _t2990[] = {gem_v_p};
        GemVal _t2991 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2988, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2989, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2990, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2991;
    } else {
    GemVal _t2992[] = {gem_v__match_49};
    GemVal _t2994;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2992, 1), gem_string("table")))) {
        _t2994 = gem_eq(gem_type_fn(NULL, _t2992, 1), gem_string("table"));
    } else {
        GemVal _t2993[] = {gem_v__match_49, gem_string("tag")};
        _t2994 = gem_has_key_fn(NULL, _t2993, 2);
    }
    GemVal _t2995;
    if (!gem_truthy(_t2994)) {
        _t2995 = _t2994;
    } else {
        _t2995 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("index_assign"));
    }
    if (gem_truthy(_t2995)) {
#line 987 "compiler/main.gem"
    GemVal _t2996[] = {gem_v_node, gem_v_indent};
    GemVal _t2997 = (*gem_v_compile_stmt);
    GemVal _t2998[] = {_t2997.fn(_t2997.env, _t2996, 2)};
    GemVal _t2999[] = {gem_v_p};
    GemVal _t3000[] = {gem_v_p};
        GemVal _t3001 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2998, 1), gem_string("\n")), gem_to_string_fn(NULL, _t2999, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3000, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3001;
    } else {
    GemVal _t3002[] = {gem_v__match_49};
    GemVal _t3004;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3002, 1), gem_string("table")))) {
        _t3004 = gem_eq(gem_type_fn(NULL, _t3002, 1), gem_string("table"));
    } else {
        GemVal _t3003[] = {gem_v__match_49, gem_string("tag")};
        _t3004 = gem_has_key_fn(NULL, _t3003, 2);
    }
    GemVal _t3005;
    if (!gem_truthy(_t3004)) {
        _t3005 = _t3004;
    } else {
        _t3005 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3005)) {
#line 989 "compiler/main.gem"
    GemVal _t3006[] = {gem_v_node, gem_v_indent};
    GemVal _t3007 = (*gem_v_compile_stmt);
    GemVal _t3008[] = {_t3007.fn(_t3007.env, _t3006, 2)};
    GemVal _t3009[] = {gem_v_p};
    GemVal _t3010[] = {gem_v_p};
        GemVal _t3011 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3008, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3009, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3010, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3011;
    } else {
    GemVal _t3012[] = {gem_v__match_49};
    GemVal _t3014;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3012, 1), gem_string("table")))) {
        _t3014 = gem_eq(gem_type_fn(NULL, _t3012, 1), gem_string("table"));
    } else {
        GemVal _t3013[] = {gem_v__match_49, gem_string("tag")};
        _t3014 = gem_has_key_fn(NULL, _t3013, 2);
    }
    GemVal _t3015;
    if (!gem_truthy(_t3014)) {
        _t3015 = _t3014;
    } else {
        _t3015 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3015)) {
#line 991 "compiler/main.gem"
    GemVal _t3016[] = {gem_v_node, gem_v_indent};
    GemVal _t3017 = (*gem_v_compile_stmt);
    GemVal _t3018[] = {_t3017.fn(_t3017.env, _t3016, 2)};
    GemVal _t3019[] = {gem_v_p};
    GemVal _t3020[] = {gem_v_p};
        GemVal _t3021 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3018, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3019, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3020, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3021;
    } else {
    GemVal _t3022[] = {gem_v__match_49};
    GemVal _t3024;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3022, 1), gem_string("table")))) {
        _t3024 = gem_eq(gem_type_fn(NULL, _t3022, 1), gem_string("table"));
    } else {
        GemVal _t3023[] = {gem_v__match_49, gem_string("tag")};
        _t3024 = gem_has_key_fn(NULL, _t3023, 2);
    }
    GemVal _t3025;
    if (!gem_truthy(_t3024)) {
        _t3025 = _t3024;
    } else {
        _t3025 = gem_eq(gem_table_get(gem_v__match_49, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3025)) {
#line 993 "compiler/main.gem"
    GemVal _t3026[] = {gem_v_node, gem_v_indent};
    GemVal _t3027 = (*gem_v_compile_stmt);
    GemVal _t3028[] = {_t3027.fn(_t3027.env, _t3026, 2)};
    GemVal _t3029[] = {gem_v_p};
    GemVal _t3030[] = {gem_v_p};
        GemVal _t3031 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3028, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3029, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3030, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3031;
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
#line 996 "compiler/main.gem"
    GemVal _t3032[] = {gem_v_node};
    GemVal _t3033 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3033.fn(_t3033.env, _t3032, 1);
#line 997 "compiler/main.gem"
    GemVal _t3034 = (*gem_v_tmp);
    GemVal gem_v_t = _t3034.fn(_t3034.env, NULL, 0);
    GemVal _t3035[] = {gem_v_line_dir};
    GemVal _t3036 = gem_v_r;
    GemVal _t3037[] = {gem_table_get(_t3036, gem_string("setup"))};
    GemVal _t3038[] = {gem_v_p};
    GemVal _t3039[] = {gem_v_t};
    GemVal _t3040 = gem_v_r;
    GemVal _t3041[] = {gem_table_get(_t3040, gem_string("expr"))};
    GemVal _t3042[] = {gem_v_p};
    GemVal _t3043[] = {gem_v_p};
    GemVal _t3044[] = {gem_v_t};
    GemVal _t3045 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3035, 1), gem_to_string_fn(NULL, _t3037, 1)), gem_to_string_fn(NULL, _t3038, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3039, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3041, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3042, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3043, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3044, 1)), gem_string(";"));
    gem_pop_frame();
    return _t3045;
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
#line 1004 "compiler/main.gem"
    GemVal _t3047[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3047, 1);
#line 1005 "compiler/main.gem"
    GemVal _t3048 = gem_v_node;
    GemVal _t3049[] = {gem_table_get(_t3048, gem_string("cond"))};
    GemVal _t3050 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3050.fn(_t3050.env, _t3049, 1);
#line 1006 "compiler/main.gem"
    GemVal _t3051 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t3051, gem_string("setup"));
#line 1007 "compiler/main.gem"
    GemVal _t3052[] = {gem_v_p};
    GemVal _t3053 = gem_v_cond_r;
    GemVal _t3054[] = {gem_table_get(_t3053, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3052, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3054, 1)), gem_string(")) {\n")));
#line 1008 "compiler/main.gem"
    GemVal _t3055 = gem_v_node;
    GemVal _t3056[] = {gem_table_get(_t3055, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3057 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t3057.fn(_t3057.env, _t3056, 2));
#line 1009 "compiler/main.gem"
    GemVal _t3058 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3058, gem_string("else")), GEM_NIL))) {
#line 1010 "compiler/main.gem"
    GemVal _t3059[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3059, 1), gem_string("} else {\n")));
#line 1011 "compiler/main.gem"
    GemVal _t3060 = gem_v_node;
    GemVal _t3061[] = {gem_table_get(_t3060, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3062 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3062.fn(_t3062.env, _t3061, 2));
    }
#line 1013 "compiler/main.gem"
    GemVal _t3063[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3063, 1), gem_string("}")));
    GemVal _t3064 = gem_v_out;
    gem_pop_frame();
    return _t3064;
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
#line 1020 "compiler/main.gem"
    GemVal _t3066[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3066, 1);
#line 1021 "compiler/main.gem"
    GemVal _t3067 = gem_v_node;
    GemVal _t3068[] = {gem_table_get(_t3067, gem_string("cond"))};
    GemVal _t3069 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3069.fn(_t3069.env, _t3068, 1);
#line 1022 "compiler/main.gem"
    GemVal _t3070 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t3070, gem_string("setup"));
#line 1023 "compiler/main.gem"
    GemVal _t3071[] = {gem_v_p};
    GemVal _t3072 = gem_v_cond_r;
    GemVal _t3073[] = {gem_table_get(_t3072, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3071, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3073, 1)), gem_string(")) {\n")));
#line 1024 "compiler/main.gem"
    GemVal _t3074 = gem_v_node;
    GemVal gem_v_then_body = gem_table_get(_t3074, gem_string("then"));
#line 1025 "compiler/main.gem"
    GemVal _t3075[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3075, 1), gem_int(0)))) {
#line 1026 "compiler/main.gem"
        GemVal gem_v__for_i_51 = gem_int(0);
#line 1026 "compiler/main.gem"
    GemVal _t3076[] = {gem_v_then_body};
        GemVal gem_v__for_limit_51 = gem_sub(gem_len_fn(NULL, _t3076, 1), gem_int(1));
#line 1026 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_v__for_limit_51))) break;
#line 1026 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_51;
#line 1026 "compiler/main.gem"
            gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 1027 "compiler/main.gem"
    GemVal _t3077[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3078 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3078.fn(_t3078.env, _t3077, 2)), gem_string("\n"));
        }

#line 1029 "compiler/main.gem"
    GemVal _t3079[] = {gem_v_then_body};
    GemVal _t3080[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t3079, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3081 = (*gem_v_compile_stmt_return);
        gem_v_out = gem_add(gem_add(gem_v_out, _t3081.fn(_t3081.env, _t3080, 2)), gem_string("\n"));
    } else {
#line 1031 "compiler/main.gem"
    GemVal _t3082[] = {gem_v_p};
    GemVal _t3083[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3082, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3083, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1033 "compiler/main.gem"
    GemVal _t3084 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3084, gem_string("else")), GEM_NIL))) {
#line 1034 "compiler/main.gem"
    GemVal _t3085[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3085, 1), gem_string("} else {\n")));
#line 1035 "compiler/main.gem"
    GemVal _t3086 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t3086, gem_string("else"));
#line 1036 "compiler/main.gem"
    GemVal _t3087[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3087, 1), gem_int(0)))) {
#line 1037 "compiler/main.gem"
            GemVal gem_v__for_i_52 = gem_int(0);
#line 1037 "compiler/main.gem"
    GemVal _t3088[] = {gem_v_else_body};
            GemVal gem_v__for_limit_52 = gem_sub(gem_len_fn(NULL, _t3088, 1), gem_int(1));
#line 1037 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 1037 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_52;
#line 1037 "compiler/main.gem"
                gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 1038 "compiler/main.gem"
    GemVal _t3089[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3090 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3090.fn(_t3090.env, _t3089, 2)), gem_string("\n"));
            }

#line 1040 "compiler/main.gem"
    GemVal _t3091[] = {gem_v_else_body};
    GemVal _t3092[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t3091, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3093 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3093.fn(_t3093.env, _t3092, 2)), gem_string("\n"));
        } else {
#line 1042 "compiler/main.gem"
    GemVal _t3094[] = {gem_v_p};
    GemVal _t3095[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3094, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3095, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1045 "compiler/main.gem"
    GemVal _t3096[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3096, 1), gem_string("} else {\n")));
#line 1046 "compiler/main.gem"
    GemVal _t3097[] = {gem_v_p};
    GemVal _t3098[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3097, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3098, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1048 "compiler/main.gem"
    GemVal _t3099[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3099, 1), gem_string("}")));
    GemVal _t3100 = gem_v_out;
    gem_pop_frame();
    return _t3100;
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
#line 1055 "compiler/main.gem"
    GemVal _t3102[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3102, 1);
#line 1056 "compiler/main.gem"
    GemVal _t3103 = gem_v_node;
    GemVal _t3104[] = {gem_table_get(_t3103, gem_string("cond"))};
    GemVal _t3105 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3105.fn(_t3105.env, _t3104, 1);
#line 1057 "compiler/main.gem"
    GemVal _t3106[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t3106, 1), gem_string("while (1) {\n"));
#line 1058 "compiler/main.gem"
    GemVal _t3107 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t3107, gem_string("setup")), gem_string("")))) {
#line 1059 "compiler/main.gem"
    GemVal _t3108 = gem_v_cond_r;
    GemVal _t3109[] = {gem_v_p};
    GemVal _t3110[] = {gem_table_get(_t3108, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t3109, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_str_replace_fn(NULL, _t3110, 3));
    }
#line 1061 "compiler/main.gem"
    GemVal _t3111[] = {gem_v_p};
    GemVal _t3112 = gem_v_cond_r;
    GemVal _t3113[] = {gem_table_get(_t3112, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3111, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t3113, 1)), gem_string(")) break;\n")));
#line 1062 "compiler/main.gem"
    GemVal _t3114 = gem_v_node;
    GemVal _t3115[] = {gem_table_get(_t3114, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3116 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t3116.fn(_t3116.env, _t3115, 2));
#line 1063 "compiler/main.gem"
    GemVal _t3117[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3117, 1), gem_string("}")));
    GemVal _t3118 = gem_v_out;
    gem_pop_frame();
    return _t3118;
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
#line 1070 "compiler/main.gem"
    GemVal _t3120[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3120, 1);
#line 1071 "compiler/main.gem"
    GemVal _t3121 = gem_v_node;
    GemVal _t3122[] = {gem_table_get(_t3121, gem_string("target"))};
    GemVal _t3123 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3123.fn(_t3123.env, _t3122, 1);
#line 1072 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1073 "compiler/main.gem"
    GemVal _t3124 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3124, gem_string("target_var")), GEM_NIL))) {
#line 1074 "compiler/main.gem"
    GemVal _t3125 = gem_v_node;
    GemVal _t3126[] = {gem_table_get(_t3125, gem_string("target_var"))};
    GemVal _t3127 = (*gem_v_mangle);
        gem_v_t = _t3127.fn(_t3127.env, _t3126, 1);
    } else {
#line 1076 "compiler/main.gem"
    GemVal _t3128 = (*gem_v_tmp);
        gem_v_t = _t3128.fn(_t3128.env, NULL, 0);
    }
#line 1078 "compiler/main.gem"
    GemVal _t3129 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t3129, gem_string("setup"));
#line 1079 "compiler/main.gem"
    GemVal _t3130[] = {gem_v_p};
    GemVal _t3131[] = {gem_v_t};
    GemVal _t3132 = gem_v_target_r;
    GemVal _t3133[] = {gem_table_get(_t3132, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3130, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3131, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3133, 1)), gem_string(";\n")));
#line 1080 "compiler/main.gem"
    GemVal _t3134 = gem_v_node;
    GemVal _t3135[] = {gem_table_get(_t3134, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3135, 1), gem_int(0)))) {
#line 1082 "compiler/main.gem"
    GemVal _t3136 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3136, gem_string("else")), GEM_NIL))) {
#line 1083 "compiler/main.gem"
    GemVal _t3137 = gem_v_node;
    GemVal _t3138[] = {gem_table_get(_t3137, gem_string("else")), gem_v_indent};
    GemVal _t3139 = (*gem_v_compile_stmts);
            gem_v_out = gem_add(gem_v_out, _t3139.fn(_t3139.env, _t3138, 2));
        }
#line 1085 "compiler/main.gem"
    GemVal _t3140[] = {gem_v_p};
    GemVal _t3141[] = {gem_v_t};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3140, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t3141, 1)), gem_string(";")));
#line 1086 "compiler/main.gem"
        GemVal _t3142 = gem_v_out;
        gem_pop_frame();
        return _t3142;
    }
#line 1088 "compiler/main.gem"
    GemVal gem_v__for_i_54 = gem_int(0);
#line 1088 "compiler/main.gem"
    GemVal _t3143 = gem_v_node;
    GemVal _t3144[] = {gem_table_get(_t3143, gem_string("whens"))};
    GemVal gem_v__for_limit_54 = gem_len_fn(NULL, _t3144, 1);
#line 1088 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_v__for_limit_54))) break;
#line 1088 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_54;
#line 1088 "compiler/main.gem"
        gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 1089 "compiler/main.gem"
    GemVal _t3145 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t3145, gem_string("whens")), gem_v_i);
#line 1090 "compiler/main.gem"
    GemVal _t3146 = gem_v_w;
    GemVal _t3147[] = {gem_table_get(_t3146, gem_string("value"))};
    GemVal _t3148 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3148.fn(_t3148.env, _t3147, 1);
#line 1091 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1092 "compiler/main.gem"
    GemVal _t3149 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3149, gem_string("setup")));
#line 1093 "compiler/main.gem"
    GemVal _t3150[] = {gem_v_p};
    GemVal _t3151 = gem_v_val_r;
    GemVal _t3152[] = {gem_table_get(_t3151, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3150, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3152, 1)), gem_string(")) {\n")));
        } else {
#line 1095 "compiler/main.gem"
    GemVal _t3153[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3153, 1), gem_string("} else {\n")));
#line 1096 "compiler/main.gem"
    GemVal _t3154 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3154, gem_string("setup")));
#line 1097 "compiler/main.gem"
    GemVal _t3155[] = {gem_v_p};
    GemVal _t3156 = gem_v_val_r;
    GemVal _t3157[] = {gem_table_get(_t3156, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3155, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3157, 1)), gem_string(")) {\n")));
        }
#line 1099 "compiler/main.gem"
    GemVal _t3158 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t3158, gem_string("bindings")), GEM_NIL))) {
#line 1100 "compiler/main.gem"
    GemVal _t3159 = gem_v_w;
            GemVal gem_v__for_items_53 = gem_table_get(_t3159, gem_string("bindings"));
#line 1100 "compiler/main.gem"
            GemVal gem_v__for_i_53 = gem_int(0);
#line 1100 "compiler/main.gem"
            while (1) {
                GemVal _t3160[] = {gem_v__for_items_53};
                if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_len_fn(NULL, _t3160, 1)))) break;
#line 1100 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_53, gem_v__for_i_53);
#line 1100 "compiler/main.gem"
                gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 1101 "compiler/main.gem"
    GemVal _t3161[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3162 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3162.fn(_t3162.env, _t3161, 2)), gem_string("\n"));
            }

        }
#line 1104 "compiler/main.gem"
    GemVal _t3163 = gem_v_w;
    GemVal _t3164[] = {gem_table_get(_t3163, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3165 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3165.fn(_t3165.env, _t3164, 2));
    }

#line 1106 "compiler/main.gem"
    GemVal _t3166 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3166, gem_string("else")), GEM_NIL))) {
#line 1107 "compiler/main.gem"
    GemVal _t3167[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3167, 1), gem_string("} else {\n")));
#line 1108 "compiler/main.gem"
    GemVal _t3168 = gem_v_node;
    GemVal _t3169[] = {gem_table_get(_t3168, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3170 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3170.fn(_t3170.env, _t3169, 2));
    }
#line 1111 "compiler/main.gem"
    GemVal _t3171[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3171, 1), gem_string("}")));
#line 1112 "compiler/main.gem"
    GemVal gem_v__for_i_55 = gem_int(1);
#line 1112 "compiler/main.gem"
    GemVal _t3172 = gem_v_node;
    GemVal _t3173[] = {gem_table_get(_t3172, gem_string("whens"))};
    GemVal gem_v__for_limit_55 = gem_len_fn(NULL, _t3173, 1);
#line 1112 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_v__for_limit_55))) break;
#line 1112 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_55;
#line 1112 "compiler/main.gem"
        gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1113 "compiler/main.gem"
    GemVal _t3174[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("\n"), gem_to_string_fn(NULL, _t3174, 1)), gem_string("}")));
    }

    GemVal _t3175 = gem_v_out;
    gem_pop_frame();
    return _t3175;
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
#line 1121 "compiler/main.gem"
    GemVal _t3177[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3177, 1);
#line 1122 "compiler/main.gem"
    GemVal _t3178 = gem_v_node;
    GemVal _t3179[] = {gem_table_get(_t3178, gem_string("target"))};
    GemVal _t3180 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3180.fn(_t3180.env, _t3179, 1);
#line 1123 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1124 "compiler/main.gem"
    GemVal _t3181 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3181, gem_string("target_var")), GEM_NIL))) {
#line 1125 "compiler/main.gem"
    GemVal _t3182 = gem_v_node;
    GemVal _t3183[] = {gem_table_get(_t3182, gem_string("target_var"))};
    GemVal _t3184 = (*gem_v_mangle);
        gem_v_t = _t3184.fn(_t3184.env, _t3183, 1);
    } else {
#line 1127 "compiler/main.gem"
    GemVal _t3185 = (*gem_v_tmp);
        gem_v_t = _t3185.fn(_t3185.env, NULL, 0);
    }
#line 1129 "compiler/main.gem"
    GemVal _t3186 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t3186, gem_string("setup"));
#line 1130 "compiler/main.gem"
    GemVal _t3187[] = {gem_v_p};
    GemVal _t3188[] = {gem_v_t};
    GemVal _t3189 = gem_v_target_r;
    GemVal _t3190[] = {gem_table_get(_t3189, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3187, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3188, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3190, 1)), gem_string(";\n")));
#line 1131 "compiler/main.gem"
    GemVal _t3191 = gem_v_node;
    GemVal _t3192[] = {gem_table_get(_t3191, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3192, 1), gem_int(0)))) {
#line 1133 "compiler/main.gem"
    GemVal _t3193 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3193, gem_string("else")), GEM_NIL))) {
#line 1134 "compiler/main.gem"
    GemVal _t3194 = gem_v_node;
            GemVal gem_v_else_body = gem_table_get(_t3194, gem_string("else"));
#line 1135 "compiler/main.gem"
    GemVal _t3195[] = {gem_v_else_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3195, 1), gem_int(0)))) {
#line 1136 "compiler/main.gem"
                GemVal gem_v__for_i_56 = gem_int(0);
#line 1136 "compiler/main.gem"
    GemVal _t3196[] = {gem_v_else_body};
                GemVal gem_v__for_limit_56 = gem_sub(gem_len_fn(NULL, _t3196, 1), gem_int(1));
#line 1136 "compiler/main.gem"
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_v__for_limit_56))) break;
#line 1136 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_56;
#line 1136 "compiler/main.gem"
                    gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1137 "compiler/main.gem"
    GemVal _t3197[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_v_indent};
    GemVal _t3198 = (*gem_v_compile_stmt);
                    gem_v_out = gem_add(gem_add(gem_v_out, _t3198.fn(_t3198.env, _t3197, 2)), gem_string("\n"));
                }

#line 1139 "compiler/main.gem"
    GemVal _t3199[] = {gem_v_else_body};
    GemVal _t3200[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t3199, 1), gem_int(1))), gem_v_indent};
    GemVal _t3201 = (*gem_v_compile_stmt_return);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3201.fn(_t3201.env, _t3200, 2)), gem_string("\n"));
            } else {
#line 1141 "compiler/main.gem"
    GemVal _t3202[] = {gem_v_p};
    GemVal _t3203[] = {gem_v_p};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3202, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3203, 1)), gem_string("return GEM_NIL;\n")));
            }
        } else {
#line 1144 "compiler/main.gem"
    GemVal _t3204[] = {gem_v_p};
    GemVal _t3205[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3204, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3205, 1)), gem_string("return GEM_NIL;\n")));
        }
#line 1146 "compiler/main.gem"
    GemVal _t3206[] = {gem_v_p};
    GemVal _t3207[] = {gem_v_t};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3206, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t3207, 1)), gem_string(";")));
#line 1147 "compiler/main.gem"
        GemVal _t3208 = gem_v_out;
        gem_pop_frame();
        return _t3208;
    }
#line 1149 "compiler/main.gem"
    GemVal gem_v__for_i_59 = gem_int(0);
#line 1149 "compiler/main.gem"
    GemVal _t3209 = gem_v_node;
    GemVal _t3210[] = {gem_table_get(_t3209, gem_string("whens"))};
    GemVal gem_v__for_limit_59 = gem_len_fn(NULL, _t3210, 1);
#line 1149 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_v__for_limit_59))) break;
#line 1149 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_59;
#line 1149 "compiler/main.gem"
        gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1150 "compiler/main.gem"
    GemVal _t3211 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t3211, gem_string("whens")), gem_v_i);
#line 1151 "compiler/main.gem"
    GemVal _t3212 = gem_v_w;
    GemVal _t3213[] = {gem_table_get(_t3212, gem_string("value"))};
    GemVal _t3214 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3214.fn(_t3214.env, _t3213, 1);
#line 1152 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1153 "compiler/main.gem"
    GemVal _t3215 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3215, gem_string("setup")));
#line 1154 "compiler/main.gem"
    GemVal _t3216[] = {gem_v_p};
    GemVal _t3217 = gem_v_val_r;
    GemVal _t3218[] = {gem_table_get(_t3217, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3216, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3218, 1)), gem_string(")) {\n")));
        } else {
#line 1156 "compiler/main.gem"
    GemVal _t3219[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3219, 1), gem_string("} else {\n")));
#line 1157 "compiler/main.gem"
    GemVal _t3220 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3220, gem_string("setup")));
#line 1158 "compiler/main.gem"
    GemVal _t3221[] = {gem_v_p};
    GemVal _t3222 = gem_v_val_r;
    GemVal _t3223[] = {gem_table_get(_t3222, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3221, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3223, 1)), gem_string(")) {\n")));
        }
#line 1160 "compiler/main.gem"
    GemVal _t3224 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t3224, gem_string("bindings")), GEM_NIL))) {
#line 1161 "compiler/main.gem"
    GemVal _t3225 = gem_v_w;
            GemVal gem_v__for_items_57 = gem_table_get(_t3225, gem_string("bindings"));
#line 1161 "compiler/main.gem"
            GemVal gem_v__for_i_57 = gem_int(0);
#line 1161 "compiler/main.gem"
            while (1) {
                GemVal _t3226[] = {gem_v__for_items_57};
                if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t3226, 1)))) break;
#line 1161 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
#line 1161 "compiler/main.gem"
                gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
#line 1162 "compiler/main.gem"
    GemVal _t3227[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3228 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3228.fn(_t3228.env, _t3227, 2)), gem_string("\n"));
            }

        }
#line 1165 "compiler/main.gem"
    GemVal _t3229 = gem_v_w;
        GemVal gem_v_body = gem_table_get(_t3229, gem_string("body"));
#line 1166 "compiler/main.gem"
    GemVal _t3230[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3230, 1), gem_int(0)))) {
#line 1167 "compiler/main.gem"
            GemVal gem_v__for_i_58 = gem_int(0);
#line 1167 "compiler/main.gem"
    GemVal _t3231[] = {gem_v_body};
            GemVal gem_v__for_limit_58 = gem_sub(gem_len_fn(NULL, _t3231, 1), gem_int(1));
#line 1167 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_v__for_limit_58))) break;
#line 1167 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_58;
#line 1167 "compiler/main.gem"
                gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1168 "compiler/main.gem"
    GemVal _t3232[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3233 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3233.fn(_t3233.env, _t3232, 2)), gem_string("\n"));
            }

#line 1170 "compiler/main.gem"
    GemVal _t3234[] = {gem_v_body};
    GemVal _t3235[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t3234, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3236 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3236.fn(_t3236.env, _t3235, 2)), gem_string("\n"));
        } else {
#line 1172 "compiler/main.gem"
    GemVal _t3237[] = {gem_v_p};
    GemVal _t3238[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3237, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3238, 1)), gem_string("    return GEM_NIL;\n")));
        }
    }

#line 1175 "compiler/main.gem"
    GemVal _t3239 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3239, gem_string("else")), GEM_NIL))) {
#line 1176 "compiler/main.gem"
    GemVal _t3240[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3240, 1), gem_string("} else {\n")));
#line 1177 "compiler/main.gem"
    GemVal _t3241 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t3241, gem_string("else"));
#line 1178 "compiler/main.gem"
    GemVal _t3242[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3242, 1), gem_int(0)))) {
#line 1179 "compiler/main.gem"
            GemVal gem_v__for_i_60 = gem_int(0);
#line 1179 "compiler/main.gem"
    GemVal _t3243[] = {gem_v_else_body};
            GemVal gem_v__for_limit_60 = gem_sub(gem_len_fn(NULL, _t3243, 1), gem_int(1));
#line 1179 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1179 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_60;
#line 1179 "compiler/main.gem"
                gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1180 "compiler/main.gem"
    GemVal _t3244[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3245 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3245.fn(_t3245.env, _t3244, 2)), gem_string("\n"));
            }

#line 1182 "compiler/main.gem"
    GemVal _t3246[] = {gem_v_else_body};
    GemVal _t3247[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t3246, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3248 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3248.fn(_t3248.env, _t3247, 2)), gem_string("\n"));
        } else {
#line 1184 "compiler/main.gem"
    GemVal _t3249[] = {gem_v_p};
    GemVal _t3250[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3249, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3250, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1187 "compiler/main.gem"
    GemVal _t3251[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3251, 1), gem_string("} else {\n")));
#line 1188 "compiler/main.gem"
    GemVal _t3252[] = {gem_v_p};
    GemVal _t3253[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3252, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3253, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1190 "compiler/main.gem"
    GemVal _t3254[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3254, 1), gem_string("}")));
#line 1191 "compiler/main.gem"
    GemVal gem_v__for_i_61 = gem_int(1);
#line 1191 "compiler/main.gem"
    GemVal _t3255 = gem_v_node;
    GemVal _t3256[] = {gem_table_get(_t3255, gem_string("whens"))};
    GemVal gem_v__for_limit_61 = gem_len_fn(NULL, _t3256, 1);
#line 1191 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_v__for_limit_61))) break;
#line 1191 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_61;
#line 1191 "compiler/main.gem"
        gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1192 "compiler/main.gem"
    GemVal _t3257[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("\n"), gem_to_string_fn(NULL, _t3257, 1)), gem_string("}")));
    }

    GemVal _t3258 = gem_v_out;
    gem_pop_frame();
    return _t3258;
}

struct _closure__anon_48 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_48(void *_env, GemVal *args, int argc) {
    struct _closure__anon_48 *_cls = (struct _closure__anon_48 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_48", "compiler/main.gem", 0);
#line 1200 "compiler/main.gem"
    GemVal _t3260[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3260, 1);
#line 1201 "compiler/main.gem"
    GemVal _t3261 = gem_v_node;
    GemVal gem_v_arms = gem_table_get(_t3261, gem_string("arms"));
#line 1202 "compiler/main.gem"
    GemVal _t3262 = gem_v_node;
    GemVal gem_v_after_ms = gem_table_get(_t3262, gem_string("after_ms"));
#line 1203 "compiler/main.gem"
    GemVal _t3263 = gem_v_node;
    GemVal gem_v_after_body = gem_table_get(_t3263, gem_string("after_body"));
#line 1206 "compiler/main.gem"
    GemVal _t3264 = gem_v_node;
    GemVal _t3265[] = {gem_table_get(_t3264, gem_string("recv_var"))};
    GemVal _t3266 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3266.fn(_t3266.env, _t3265, 1);
#line 1207 "compiler/main.gem"
    GemVal _t3267 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3267.fn(_t3267.env, NULL, 0);
#line 1208 "compiler/main.gem"
    GemVal _t3268 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3268.fn(_t3268.env, NULL, 0);
#line 1209 "compiler/main.gem"
    GemVal _t3269 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3269.fn(_t3269.env, NULL, 0);
#line 1210 "compiler/main.gem"
    GemVal _t3270 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3270.fn(_t3270.env, NULL, 0);
#line 1211 "compiler/main.gem"
    GemVal _t3271 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3271.fn(_t3271.env, NULL, 0);
#line 1213 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1216 "compiler/main.gem"
    GemVal _t3272[] = {gem_v_p};
    GemVal _t3273[] = {gem_v_recv_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3272, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3273, 1)), gem_string(" = GEM_NIL;\n")));
#line 1217 "compiler/main.gem"
    GemVal _t3274[] = {gem_v_p};
    GemVal _t3275[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3274, 1), gem_string("int ")), gem_to_string_fn(NULL, _t3275, 1)), gem_string(" = -1;\n")));
#line 1220 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1221 "compiler/main.gem"
    GemVal _t3276[] = {gem_v_after_ms};
    GemVal _t3277 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3277.fn(_t3277.env, _t3276, 1);
#line 1222 "compiler/main.gem"
    GemVal _t3278 = gem_v_ms_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t3278, gem_string("setup")));
#line 1223 "compiler/main.gem"
    GemVal _t3279[] = {gem_v_p};
    GemVal _t3280[] = {gem_v_deadline_tmp};
    GemVal _t3281 = gem_v_ms_r;
    GemVal _t3282[] = {gem_table_get(_t3281, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3279, 1), gem_string("int64_t ")), gem_to_string_fn(NULL, _t3280, 1)), gem_string(" = gem_now_ms() + (int64_t)(")), gem_to_string_fn(NULL, _t3282, 1)), gem_string(").ival;\n")));
    }
#line 1227 "compiler/main.gem"
    GemVal _t3283[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3283, 1), gem_string("while (1) {\n")));
#line 1228 "compiler/main.gem"
    GemVal _t3284[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn_pad_for(NULL, _t3284, 1);
#line 1231 "compiler/main.gem"
    GemVal _t3285[] = {gem_v_p2};
    GemVal _t3286[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3285, 1), gem_string("GemProcess *")), gem_to_string_fn(NULL, _t3286, 1)), gem_string(" = &gem_proc_table[gem_current_pid];\n")));
#line 1234 "compiler/main.gem"
    GemVal _t3287[] = {gem_v_p2};
    GemVal _t3288[] = {gem_v_prev_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3287, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3288, 1)), gem_string(" = NULL;\n")));
#line 1235 "compiler/main.gem"
    GemVal _t3289[] = {gem_v_p2};
    GemVal _t3290[] = {gem_v_cur_tmp};
    GemVal _t3291[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3289, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3290, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3291, 1)), gem_string("->mailbox.head;\n")));
#line 1236 "compiler/main.gem"
    GemVal _t3292[] = {gem_v_p2};
    GemVal _t3293[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_to_string_fn(NULL, _t3292, 1), gem_to_string_fn(NULL, _t3293, 1)), gem_string(" = -1;\n")));
#line 1237 "compiler/main.gem"
    GemVal _t3294[] = {gem_v_p2};
    GemVal _t3295[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3294, 1), gem_string("while (")), gem_to_string_fn(NULL, _t3295, 1)), gem_string(") {\n")));
#line 1238 "compiler/main.gem"
    GemVal _t3296[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn_pad_for(NULL, _t3296, 1);
#line 1239 "compiler/main.gem"
    GemVal _t3297[] = {gem_v_p3};
    GemVal _t3298[] = {gem_v_recv_tmp};
    GemVal _t3299[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3297, 1), gem_to_string_fn(NULL, _t3298, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3299, 1)), gem_string("->value;\n")));
#line 1242 "compiler/main.gem"
    GemVal gem_v__for_i_62 = gem_int(0);
#line 1242 "compiler/main.gem"
    GemVal _t3300[] = {gem_v_arms};
    GemVal gem_v__for_limit_62 = gem_len_fn(NULL, _t3300, 1);
#line 1242 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
#line 1242 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_62;
#line 1242 "compiler/main.gem"
        gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1243 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1244 "compiler/main.gem"
    GemVal _t3301 = gem_v_arm;
    GemVal _t3302 = gem_table_get(_t3301, gem_string("pattern"));
    GemVal _t3303[] = {gem_table_get(_t3302, gem_string("condition"))};
    GemVal _t3304 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3304.fn(_t3304.env, _t3303, 1);
#line 1245 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1246 "compiler/main.gem"
    GemVal _t3305 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3305, gem_string("setup")));
#line 1247 "compiler/main.gem"
    GemVal _t3306[] = {gem_v_p3};
    GemVal _t3307 = gem_v_cond_r;
    GemVal _t3308[] = {gem_table_get(_t3307, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3306, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3308, 1)), gem_string(")) {\n")));
        } else {
#line 1249 "compiler/main.gem"
    GemVal _t3309[] = {gem_v_p3};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3309, 1), gem_string("} else {\n")));
#line 1250 "compiler/main.gem"
    GemVal _t3310 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3310, gem_string("setup")));
#line 1251 "compiler/main.gem"
    GemVal _t3311[] = {gem_v_p3};
    GemVal _t3312 = gem_v_cond_r;
    GemVal _t3313[] = {gem_table_get(_t3312, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3311, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3313, 1)), gem_string(")) {\n")));
        }
#line 1253 "compiler/main.gem"
    GemVal _t3314[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3315[] = {gem_fn_pad_for(NULL, _t3314, 1)};
    GemVal _t3316[] = {gem_v_matched_tmp};
    GemVal _t3317[] = {gem_v_i};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3315, 1), gem_to_string_fn(NULL, _t3316, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3317, 1)), gem_string(";\n")));
    }

#line 1256 "compiler/main.gem"
    GemVal gem_v__for_i_63 = gem_int(0);
#line 1256 "compiler/main.gem"
    GemVal _t3318[] = {gem_v_arms};
    GemVal gem_v__for_limit_63 = gem_len_fn(NULL, _t3318, 1);
#line 1256 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_v__for_limit_63))) break;
#line 1256 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_63;
#line 1256 "compiler/main.gem"
        gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1257 "compiler/main.gem"
    GemVal _t3319[] = {gem_v_p3};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3319, 1), gem_string("}")));
#line 1258 "compiler/main.gem"
    GemVal _t3320[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3320, 1), gem_int(1))))) {
#line 1259 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1262 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1265 "compiler/main.gem"
    GemVal _t3321[] = {gem_v_p3};
    GemVal _t3322[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3321, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3322, 1)), gem_string(" >= 0) {\n")));
#line 1266 "compiler/main.gem"
    GemVal _t3323[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn_pad_for(NULL, _t3323, 1);
#line 1267 "compiler/main.gem"
    GemVal _t3324[] = {gem_v_p4};
    GemVal _t3325[] = {gem_v_proc_tmp};
    GemVal _t3326[] = {gem_v_prev_tmp};
    GemVal _t3327[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3324, 1), gem_string("gem_mailbox_remove(&")), gem_to_string_fn(NULL, _t3325, 1)), gem_string("->mailbox, ")), gem_to_string_fn(NULL, _t3326, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3327, 1)), gem_string(");\n")));
#line 1268 "compiler/main.gem"
    GemVal _t3328[] = {gem_v_p4};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3328, 1), gem_string("break;\n")));
#line 1269 "compiler/main.gem"
    GemVal _t3329[] = {gem_v_p3};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3329, 1), gem_string("}\n")));
#line 1272 "compiler/main.gem"
    GemVal _t3330[] = {gem_v_p3};
    GemVal _t3331[] = {gem_v_prev_tmp};
    GemVal _t3332[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3330, 1), gem_to_string_fn(NULL, _t3331, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3332, 1)), gem_string(";\n")));
#line 1273 "compiler/main.gem"
    GemVal _t3333[] = {gem_v_p3};
    GemVal _t3334[] = {gem_v_cur_tmp};
    GemVal _t3335[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3333, 1), gem_to_string_fn(NULL, _t3334, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3335, 1)), gem_string("->next;\n")));
#line 1274 "compiler/main.gem"
    GemVal _t3336[] = {gem_v_p2};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3336, 1), gem_string("}\n")));
#line 1277 "compiler/main.gem"
    GemVal _t3337[] = {gem_v_p2};
    GemVal _t3338[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3337, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3338, 1)), gem_string(" >= 0) break;\n")));
#line 1278 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1279 "compiler/main.gem"
    GemVal _t3339[] = {gem_v_p2};
    GemVal _t3340[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3339, 1), gem_string("if (gem_now_ms() >= ")), gem_to_string_fn(NULL, _t3340, 1)), gem_string(") {\n")));
#line 1280 "compiler/main.gem"
    GemVal _t3341[] = {gem_v_p2};
    GemVal _t3342[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3341, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3342, 1)), gem_string(" = -2;\n")));
#line 1281 "compiler/main.gem"
    GemVal _t3343[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3343, 1), gem_string("    break;\n")));
#line 1282 "compiler/main.gem"
    GemVal _t3344[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3344, 1), gem_string("}\n")));
#line 1283 "compiler/main.gem"
    GemVal _t3345[] = {gem_v_p2};
    GemVal _t3346[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3345, 1), gem_string("gem_selective_yield(")), gem_to_string_fn(NULL, _t3346, 1)), gem_string(");\n")));
#line 1285 "compiler/main.gem"
    GemVal _t3347[] = {gem_v_p2};
    GemVal _t3348[] = {gem_v_proc_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3347, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3348, 1)), gem_string("->timed_out) {\n")));
#line 1286 "compiler/main.gem"
    GemVal _t3349[] = {gem_v_p2};
    GemVal _t3350[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3349, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3350, 1)), gem_string(" = -2;\n")));
#line 1287 "compiler/main.gem"
    GemVal _t3351[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3351, 1), gem_string("    break;\n")));
#line 1288 "compiler/main.gem"
    GemVal _t3352[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3352, 1), gem_string("}\n")));
    } else {
#line 1290 "compiler/main.gem"
    GemVal _t3353[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3353, 1), gem_string("gem_selective_yield(-1);\n")));
    }
#line 1293 "compiler/main.gem"
    GemVal _t3354[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3354, 1), gem_string("}\n")));
#line 1296 "compiler/main.gem"
    GemVal gem_v__for_i_65 = gem_int(0);
#line 1296 "compiler/main.gem"
    GemVal _t3355[] = {gem_v_arms};
    GemVal gem_v__for_limit_65 = gem_len_fn(NULL, _t3355, 1);
#line 1296 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_v__for_limit_65))) break;
#line 1296 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_65;
#line 1296 "compiler/main.gem"
        gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1297 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1298 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1299 "compiler/main.gem"
    GemVal _t3356[] = {gem_v_p};
    GemVal _t3357[] = {gem_v_matched_tmp};
    GemVal _t3358[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3356, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3357, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3358, 1)), gem_string(") {\n")));
        } else {
#line 1301 "compiler/main.gem"
    GemVal _t3359[] = {gem_v_p};
    GemVal _t3360[] = {gem_v_matched_tmp};
    GemVal _t3361[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3359, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3360, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3361, 1)), gem_string(") {\n")));
        }
#line 1304 "compiler/main.gem"
    GemVal _t3362 = gem_v_arm;
    GemVal _t3363 = gem_table_get(_t3362, gem_string("pattern"));
        if (gem_truthy(gem_neq(gem_table_get(_t3363, gem_string("bindings")), GEM_NIL))) {
#line 1305 "compiler/main.gem"
    GemVal _t3364 = gem_v_arm;
    GemVal _t3365 = gem_table_get(_t3364, gem_string("pattern"));
            GemVal gem_v__for_items_64 = gem_table_get(_t3365, gem_string("bindings"));
#line 1305 "compiler/main.gem"
            GemVal gem_v__for_i_64 = gem_int(0);
#line 1305 "compiler/main.gem"
            while (1) {
                GemVal _t3366[] = {gem_v__for_items_64};
                if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_len_fn(NULL, _t3366, 1)))) break;
#line 1305 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_64, gem_v__for_i_64);
#line 1305 "compiler/main.gem"
                gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1306 "compiler/main.gem"
    GemVal _t3367[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3368 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3368.fn(_t3368.env, _t3367, 2)), gem_string("\n"));
            }

        }
#line 1309 "compiler/main.gem"
    GemVal _t3369 = gem_v_arm;
    GemVal _t3370[] = {gem_table_get(_t3369, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3371 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3371.fn(_t3371.env, _t3370, 2));
    }

#line 1313 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1314 "compiler/main.gem"
    GemVal _t3372[] = {gem_v_p};
    GemVal _t3373[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3372, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3373, 1)), gem_string(" == -2) {\n")));
#line 1315 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 1316 "compiler/main.gem"
    GemVal _t3374[] = {gem_v_after_body, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3375 = (*gem_v_compile_stmts);
            gem_v_out = gem_add(gem_v_out, _t3375.fn(_t3375.env, _t3374, 2));
        }
    }
#line 1320 "compiler/main.gem"
    GemVal _t3376[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3376, 1), gem_string("}")));
    GemVal _t3377 = gem_v_out;
    gem_pop_frame();
    return _t3377;
}

struct _closure__anon_49 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_49(void *_env, GemVal *args, int argc) {
    struct _closure__anon_49 *_cls = (struct _closure__anon_49 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_49", "compiler/main.gem", 0);
#line 1325 "compiler/main.gem"
    GemVal _t3379[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3379, 1);
#line 1326 "compiler/main.gem"
    GemVal _t3380 = gem_v_node;
    GemVal gem_v_arms = gem_table_get(_t3380, gem_string("arms"));
#line 1327 "compiler/main.gem"
    GemVal _t3381 = gem_v_node;
    GemVal gem_v_after_ms = gem_table_get(_t3381, gem_string("after_ms"));
#line 1328 "compiler/main.gem"
    GemVal _t3382 = gem_v_node;
    GemVal gem_v_after_body = gem_table_get(_t3382, gem_string("after_body"));
#line 1330 "compiler/main.gem"
    GemVal _t3383 = gem_v_node;
    GemVal _t3384[] = {gem_table_get(_t3383, gem_string("recv_var"))};
    GemVal _t3385 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3385.fn(_t3385.env, _t3384, 1);
#line 1331 "compiler/main.gem"
    GemVal _t3386 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3386.fn(_t3386.env, NULL, 0);
#line 1332 "compiler/main.gem"
    GemVal _t3387 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3387.fn(_t3387.env, NULL, 0);
#line 1333 "compiler/main.gem"
    GemVal _t3388 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3388.fn(_t3388.env, NULL, 0);
#line 1334 "compiler/main.gem"
    GemVal _t3389 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3389.fn(_t3389.env, NULL, 0);
#line 1335 "compiler/main.gem"
    GemVal _t3390 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3390.fn(_t3390.env, NULL, 0);
#line 1337 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1339 "compiler/main.gem"
    GemVal _t3391[] = {gem_v_p};
    GemVal _t3392[] = {gem_v_recv_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3391, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3392, 1)), gem_string(" = GEM_NIL;\n")));
#line 1340 "compiler/main.gem"
    GemVal _t3393[] = {gem_v_p};
    GemVal _t3394[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3393, 1), gem_string("int ")), gem_to_string_fn(NULL, _t3394, 1)), gem_string(" = -1;\n")));
#line 1342 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1343 "compiler/main.gem"
    GemVal _t3395[] = {gem_v_after_ms};
    GemVal _t3396 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3396.fn(_t3396.env, _t3395, 1);
#line 1344 "compiler/main.gem"
    GemVal _t3397 = gem_v_ms_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t3397, gem_string("setup")));
#line 1345 "compiler/main.gem"
    GemVal _t3398[] = {gem_v_p};
    GemVal _t3399[] = {gem_v_deadline_tmp};
    GemVal _t3400 = gem_v_ms_r;
    GemVal _t3401[] = {gem_table_get(_t3400, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3398, 1), gem_string("int64_t ")), gem_to_string_fn(NULL, _t3399, 1)), gem_string(" = gem_now_ms() + (int64_t)(")), gem_to_string_fn(NULL, _t3401, 1)), gem_string(").ival;\n")));
    }
#line 1348 "compiler/main.gem"
    GemVal _t3402[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3402, 1), gem_string("while (1) {\n")));
#line 1349 "compiler/main.gem"
    GemVal _t3403[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn_pad_for(NULL, _t3403, 1);
#line 1351 "compiler/main.gem"
    GemVal _t3404[] = {gem_v_p2};
    GemVal _t3405[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3404, 1), gem_string("GemProcess *")), gem_to_string_fn(NULL, _t3405, 1)), gem_string(" = &gem_proc_table[gem_current_pid];\n")));
#line 1352 "compiler/main.gem"
    GemVal _t3406[] = {gem_v_p2};
    GemVal _t3407[] = {gem_v_prev_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3406, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3407, 1)), gem_string(" = NULL;\n")));
#line 1353 "compiler/main.gem"
    GemVal _t3408[] = {gem_v_p2};
    GemVal _t3409[] = {gem_v_cur_tmp};
    GemVal _t3410[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3408, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3409, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3410, 1)), gem_string("->mailbox.head;\n")));
#line 1354 "compiler/main.gem"
    GemVal _t3411[] = {gem_v_p2};
    GemVal _t3412[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_to_string_fn(NULL, _t3411, 1), gem_to_string_fn(NULL, _t3412, 1)), gem_string(" = -1;\n")));
#line 1355 "compiler/main.gem"
    GemVal _t3413[] = {gem_v_p2};
    GemVal _t3414[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3413, 1), gem_string("while (")), gem_to_string_fn(NULL, _t3414, 1)), gem_string(") {\n")));
#line 1356 "compiler/main.gem"
    GemVal _t3415[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn_pad_for(NULL, _t3415, 1);
#line 1357 "compiler/main.gem"
    GemVal _t3416[] = {gem_v_p3};
    GemVal _t3417[] = {gem_v_recv_tmp};
    GemVal _t3418[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3416, 1), gem_to_string_fn(NULL, _t3417, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3418, 1)), gem_string("->value;\n")));
#line 1359 "compiler/main.gem"
    GemVal gem_v__for_i_66 = gem_int(0);
#line 1359 "compiler/main.gem"
    GemVal _t3419[] = {gem_v_arms};
    GemVal gem_v__for_limit_66 = gem_len_fn(NULL, _t3419, 1);
#line 1359 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_v__for_limit_66))) break;
#line 1359 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_66;
#line 1359 "compiler/main.gem"
        gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1360 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1361 "compiler/main.gem"
    GemVal _t3420 = gem_v_arm;
    GemVal _t3421 = gem_table_get(_t3420, gem_string("pattern"));
    GemVal _t3422[] = {gem_table_get(_t3421, gem_string("condition"))};
    GemVal _t3423 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3423.fn(_t3423.env, _t3422, 1);
#line 1362 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1363 "compiler/main.gem"
    GemVal _t3424 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3424, gem_string("setup")));
#line 1364 "compiler/main.gem"
    GemVal _t3425[] = {gem_v_p3};
    GemVal _t3426 = gem_v_cond_r;
    GemVal _t3427[] = {gem_table_get(_t3426, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3425, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3427, 1)), gem_string(")) {\n")));
        } else {
#line 1366 "compiler/main.gem"
    GemVal _t3428[] = {gem_v_p3};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3428, 1), gem_string("} else {\n")));
#line 1367 "compiler/main.gem"
    GemVal _t3429 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3429, gem_string("setup")));
#line 1368 "compiler/main.gem"
    GemVal _t3430[] = {gem_v_p3};
    GemVal _t3431 = gem_v_cond_r;
    GemVal _t3432[] = {gem_table_get(_t3431, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3430, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3432, 1)), gem_string(")) {\n")));
        }
#line 1370 "compiler/main.gem"
    GemVal _t3433[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3434[] = {gem_fn_pad_for(NULL, _t3433, 1)};
    GemVal _t3435[] = {gem_v_matched_tmp};
    GemVal _t3436[] = {gem_v_i};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3434, 1), gem_to_string_fn(NULL, _t3435, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3436, 1)), gem_string(";\n")));
    }

#line 1372 "compiler/main.gem"
    GemVal gem_v__for_i_67 = gem_int(0);
#line 1372 "compiler/main.gem"
    GemVal _t3437[] = {gem_v_arms};
    GemVal gem_v__for_limit_67 = gem_len_fn(NULL, _t3437, 1);
#line 1372 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_v__for_limit_67))) break;
#line 1372 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_67;
#line 1372 "compiler/main.gem"
        gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1373 "compiler/main.gem"
    GemVal _t3438[] = {gem_v_p3};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3438, 1), gem_string("}")));
#line 1374 "compiler/main.gem"
    GemVal _t3439[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3439, 1), gem_int(1))))) {
#line 1375 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1378 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1380 "compiler/main.gem"
    GemVal _t3440[] = {gem_v_p3};
    GemVal _t3441[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3440, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3441, 1)), gem_string(" >= 0) {\n")));
#line 1381 "compiler/main.gem"
    GemVal _t3442[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn_pad_for(NULL, _t3442, 1);
#line 1382 "compiler/main.gem"
    GemVal _t3443[] = {gem_v_p4};
    GemVal _t3444[] = {gem_v_proc_tmp};
    GemVal _t3445[] = {gem_v_prev_tmp};
    GemVal _t3446[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3443, 1), gem_string("gem_mailbox_remove(&")), gem_to_string_fn(NULL, _t3444, 1)), gem_string("->mailbox, ")), gem_to_string_fn(NULL, _t3445, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3446, 1)), gem_string(");\n")));
#line 1383 "compiler/main.gem"
    GemVal _t3447[] = {gem_v_p4};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3447, 1), gem_string("break;\n")));
#line 1384 "compiler/main.gem"
    GemVal _t3448[] = {gem_v_p3};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3448, 1), gem_string("}\n")));
#line 1386 "compiler/main.gem"
    GemVal _t3449[] = {gem_v_p3};
    GemVal _t3450[] = {gem_v_prev_tmp};
    GemVal _t3451[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3449, 1), gem_to_string_fn(NULL, _t3450, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3451, 1)), gem_string(";\n")));
#line 1387 "compiler/main.gem"
    GemVal _t3452[] = {gem_v_p3};
    GemVal _t3453[] = {gem_v_cur_tmp};
    GemVal _t3454[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3452, 1), gem_to_string_fn(NULL, _t3453, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3454, 1)), gem_string("->next;\n")));
#line 1388 "compiler/main.gem"
    GemVal _t3455[] = {gem_v_p2};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3455, 1), gem_string("}\n")));
#line 1390 "compiler/main.gem"
    GemVal _t3456[] = {gem_v_p2};
    GemVal _t3457[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3456, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3457, 1)), gem_string(" >= 0) break;\n")));
#line 1391 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1392 "compiler/main.gem"
    GemVal _t3458[] = {gem_v_p2};
    GemVal _t3459[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3458, 1), gem_string("if (gem_now_ms() >= ")), gem_to_string_fn(NULL, _t3459, 1)), gem_string(") {\n")));
#line 1393 "compiler/main.gem"
    GemVal _t3460[] = {gem_v_p2};
    GemVal _t3461[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3460, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3461, 1)), gem_string(" = -2;\n")));
#line 1394 "compiler/main.gem"
    GemVal _t3462[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3462, 1), gem_string("    break;\n")));
#line 1395 "compiler/main.gem"
    GemVal _t3463[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3463, 1), gem_string("}\n")));
#line 1396 "compiler/main.gem"
    GemVal _t3464[] = {gem_v_p2};
    GemVal _t3465[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3464, 1), gem_string("gem_selective_yield(")), gem_to_string_fn(NULL, _t3465, 1)), gem_string(");\n")));
#line 1397 "compiler/main.gem"
    GemVal _t3466[] = {gem_v_p2};
    GemVal _t3467[] = {gem_v_proc_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3466, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3467, 1)), gem_string("->timed_out) {\n")));
#line 1398 "compiler/main.gem"
    GemVal _t3468[] = {gem_v_p2};
    GemVal _t3469[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3468, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3469, 1)), gem_string(" = -2;\n")));
#line 1399 "compiler/main.gem"
    GemVal _t3470[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3470, 1), gem_string("    break;\n")));
#line 1400 "compiler/main.gem"
    GemVal _t3471[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3471, 1), gem_string("}\n")));
    } else {
#line 1402 "compiler/main.gem"
    GemVal _t3472[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3472, 1), gem_string("gem_selective_yield(-1);\n")));
    }
#line 1405 "compiler/main.gem"
    GemVal _t3473[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3473, 1), gem_string("}\n")));
#line 1408 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1408 "compiler/main.gem"
    GemVal _t3474[] = {gem_v_arms};
    GemVal gem_v__for_limit_70 = gem_len_fn(NULL, _t3474, 1);
#line 1408 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_70, gem_v__for_limit_70))) break;
#line 1408 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_70;
#line 1408 "compiler/main.gem"
        gem_v__for_i_70 = gem_add(gem_v__for_i_70, gem_int(1));
#line 1409 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1410 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1411 "compiler/main.gem"
    GemVal _t3475[] = {gem_v_p};
    GemVal _t3476[] = {gem_v_matched_tmp};
    GemVal _t3477[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3475, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3476, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3477, 1)), gem_string(") {\n")));
        } else {
#line 1413 "compiler/main.gem"
    GemVal _t3478[] = {gem_v_p};
    GemVal _t3479[] = {gem_v_matched_tmp};
    GemVal _t3480[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3478, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3479, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3480, 1)), gem_string(") {\n")));
        }
#line 1415 "compiler/main.gem"
    GemVal _t3481 = gem_v_arm;
    GemVal _t3482 = gem_table_get(_t3481, gem_string("pattern"));
        if (gem_truthy(gem_neq(gem_table_get(_t3482, gem_string("bindings")), GEM_NIL))) {
#line 1416 "compiler/main.gem"
    GemVal _t3483 = gem_v_arm;
    GemVal _t3484 = gem_table_get(_t3483, gem_string("pattern"));
            GemVal gem_v__for_items_68 = gem_table_get(_t3484, gem_string("bindings"));
#line 1416 "compiler/main.gem"
            GemVal gem_v__for_i_68 = gem_int(0);
#line 1416 "compiler/main.gem"
            while (1) {
                GemVal _t3485[] = {gem_v__for_items_68};
                if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_len_fn(NULL, _t3485, 1)))) break;
#line 1416 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_68, gem_v__for_i_68);
#line 1416 "compiler/main.gem"
                gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1417 "compiler/main.gem"
    GemVal _t3486[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3487 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3487.fn(_t3487.env, _t3486, 2)), gem_string("\n"));
            }

        }
#line 1420 "compiler/main.gem"
    GemVal _t3488 = gem_v_arm;
        GemVal gem_v_body = gem_table_get(_t3488, gem_string("body"));
#line 1421 "compiler/main.gem"
    GemVal _t3489[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3489, 1), gem_int(0)))) {
#line 1422 "compiler/main.gem"
            GemVal gem_v__for_i_69 = gem_int(0);
#line 1422 "compiler/main.gem"
    GemVal _t3490[] = {gem_v_body};
            GemVal gem_v__for_limit_69 = gem_sub(gem_len_fn(NULL, _t3490, 1), gem_int(1));
#line 1422 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_69, gem_v__for_limit_69))) break;
#line 1422 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_69;
#line 1422 "compiler/main.gem"
                gem_v__for_i_69 = gem_add(gem_v__for_i_69, gem_int(1));
#line 1423 "compiler/main.gem"
    GemVal _t3491[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3492 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3492.fn(_t3492.env, _t3491, 2)), gem_string("\n"));
            }

#line 1425 "compiler/main.gem"
    GemVal _t3493[] = {gem_v_body};
    GemVal _t3494[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t3493, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3495 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3495.fn(_t3495.env, _t3494, 2)), gem_string("\n"));
        } else {
#line 1427 "compiler/main.gem"
    GemVal _t3496[] = {gem_v_p};
    GemVal _t3497[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3496, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3497, 1)), gem_string("    return GEM_NIL;\n")));
        }
    }

#line 1431 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1432 "compiler/main.gem"
    GemVal _t3498[] = {gem_v_p};
    GemVal _t3499[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3498, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3499, 1)), gem_string(" == -2) {\n")));
#line 1433 "compiler/main.gem"
    GemVal _t3501;
    if (!gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
        _t3501 = gem_neq(gem_v_after_body, GEM_NIL);
    } else {
        GemVal _t3500[] = {gem_v_after_body};
        _t3501 = gem_gt(gem_len_fn(NULL, _t3500, 1), gem_int(0));
    }
        if (gem_truthy(_t3501)) {
#line 1434 "compiler/main.gem"
            GemVal gem_v__for_i_71 = gem_int(0);
#line 1434 "compiler/main.gem"
    GemVal _t3502[] = {gem_v_after_body};
            GemVal gem_v__for_limit_71 = gem_sub(gem_len_fn(NULL, _t3502, 1), gem_int(1));
#line 1434 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_71, gem_v__for_limit_71))) break;
#line 1434 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_71;
#line 1434 "compiler/main.gem"
                gem_v__for_i_71 = gem_add(gem_v__for_i_71, gem_int(1));
#line 1435 "compiler/main.gem"
    GemVal _t3503[] = {gem_table_get(gem_v_after_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3504 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3504.fn(_t3504.env, _t3503, 2)), gem_string("\n"));
            }

#line 1437 "compiler/main.gem"
    GemVal _t3505[] = {gem_v_after_body};
    GemVal _t3506[] = {gem_table_get(gem_v_after_body, gem_sub(gem_len_fn(NULL, _t3505, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3507 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3507.fn(_t3507.env, _t3506, 2)), gem_string("\n"));
        } else {
#line 1439 "compiler/main.gem"
    GemVal _t3508[] = {gem_v_p};
    GemVal _t3509[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3508, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3509, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1442 "compiler/main.gem"
    GemVal _t3510[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3510, 1), gem_string("} else {\n")));
#line 1443 "compiler/main.gem"
    GemVal _t3511[] = {gem_v_p};
    GemVal _t3512[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3511, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3512, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1446 "compiler/main.gem"
    GemVal _t3513[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3513, 1), gem_string("}")));
    GemVal _t3514 = gem_v_out;
    gem_pop_frame();
    return _t3514;
}

struct _closure__anon_50 {
    GemVal *gem_v_extern_c_decls;
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_50(void *_env, GemVal *args, int argc) {
    struct _closure__anon_50 *_cls = (struct _closure__anon_50 *)_env;
    GemVal *gem_v_extern_c_decls = _cls->gem_v_extern_c_decls;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_50", "compiler/main.gem", 0);
#line 1453 "compiler/main.gem"
    GemVal gem_v__d72 = gem_v_node;
#line 1453 "compiler/main.gem"
    GemVal _t3516 = gem_v__d72;
    GemVal gem_v_name = gem_table_get(_t3516, gem_string("name"));
#line 1453 "compiler/main.gem"
    GemVal _t3517 = gem_v__d72;
    GemVal gem_v_params = gem_table_get(_t3517, gem_string("params"));
#line 1453 "compiler/main.gem"
    GemVal _t3518 = gem_v__d72;
    GemVal gem_v_ret_type = gem_table_get(_t3518, gem_string("ret_type"));

#line 1455 "compiler/main.gem"
    GemVal _t3519[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3519, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1457 "compiler/main.gem"
    GemVal _t3520 = gem_table_new();
    GemVal gem_v_c_args = _t3520;
#line 1458 "compiler/main.gem"
    GemVal gem_v__for_i_73 = gem_int(0);
#line 1458 "compiler/main.gem"
    GemVal _t3521[] = {gem_v_params};
    GemVal gem_v__for_limit_73 = gem_len_fn(NULL, _t3521, 1);
#line 1458 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_73, gem_v__for_limit_73))) break;
#line 1458 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_73;
#line 1458 "compiler/main.gem"
        gem_v__for_i_73 = gem_add(gem_v__for_i_73, gem_int(1));
#line 1459 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1460 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1461 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1462 "compiler/main.gem"
    GemVal _t3522[] = {gem_v_i};
    GemVal _t3523[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t3522, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3523, 1)), gem_string("].ival;\n")));
#line 1463 "compiler/main.gem"
    GemVal _t3524[] = {gem_v_i};
    GemVal _t3525[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3524, 1))};
            (void)(gem_push_fn(NULL, _t3525, 2));
        } else {
#line 1464 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1465 "compiler/main.gem"
    GemVal _t3526[] = {gem_v_i};
    GemVal _t3527[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t3526, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3527, 1)), gem_string("].fval;\n")));
#line 1466 "compiler/main.gem"
    GemVal _t3528[] = {gem_v_i};
    GemVal _t3529[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3528, 1))};
                (void)(gem_push_fn(NULL, _t3529, 2));
            } else {
#line 1467 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1468 "compiler/main.gem"
    GemVal _t3530[] = {gem_v_i};
    GemVal _t3531[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t3530, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3531, 1)), gem_string("].sval;\n")));
#line 1469 "compiler/main.gem"
    GemVal _t3532[] = {gem_v_i};
    GemVal _t3533[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3532, 1))};
                    (void)(gem_push_fn(NULL, _t3533, 2));
                } else {
#line 1470 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1471 "compiler/main.gem"
    GemVal _t3534[] = {gem_v_i};
    GemVal _t3535[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t3534, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3535, 1)), gem_string("].bval;\n")));
#line 1472 "compiler/main.gem"
    GemVal _t3536[] = {gem_v_i};
    GemVal _t3537[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3536, 1))};
                        (void)(gem_push_fn(NULL, _t3537, 2));
                    } else {
#line 1473 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1474 "compiler/main.gem"
    GemVal _t3538[] = {gem_v_i};
    GemVal _t3539[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t3538, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t3539, 1)), gem_string("].ival;\n")));
#line 1475 "compiler/main.gem"
    GemVal _t3540[] = {gem_v_i};
    GemVal _t3541[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3540, 1))};
                            (void)(gem_push_fn(NULL, _t3541, 2));
                        } else {
#line 1476 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1477 "compiler/main.gem"
    GemVal _t3542[] = {gem_v_i};
    GemVal _t3543[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3542, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3543, 2));
                            } else {
#line 1479 "compiler/main.gem"
    GemVal _t3544[] = {gem_v_i};
    GemVal _t3545[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3544, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3545, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1484 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1485 "compiler/main.gem"
    GemVal gem_v__for_i_74 = gem_int(0);
#line 1485 "compiler/main.gem"
    GemVal _t3546[] = {gem_v_c_args};
    GemVal gem_v__for_limit_74 = gem_len_fn(NULL, _t3546, 1);
#line 1485 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_74, gem_v__for_limit_74))) break;
#line 1485 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_74;
#line 1485 "compiler/main.gem"
        gem_v__for_i_74 = gem_add(gem_v__for_i_74, gem_int(1));
#line 1486 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1487 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1489 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1492 "compiler/main.gem"
    GemVal _t3547[] = {gem_v_name};
    GemVal _t3548[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3547, 1), gem_string("(")), gem_to_string_fn(NULL, _t3548, 1)), gem_string(")"));
#line 1494 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1495 "compiler/main.gem"
    GemVal _t3549[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3549, 1)), gem_string(";\n")));
#line 1496 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
#line 1497 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1498 "compiler/main.gem"
    GemVal _t3550[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t3550, 1)), gem_string(";\n")));
        } else {
#line 1499 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1500 "compiler/main.gem"
    GemVal _t3551[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t3551, 1)), gem_string(";\n")));
#line 1501 "compiler/main.gem"
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
#line 1502 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1503 "compiler/main.gem"
    GemVal _t3552[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t3552, 1)), gem_string(";\n")));
#line 1504 "compiler/main.gem"
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
#line 1505 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1506 "compiler/main.gem"
    GemVal _t3553[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t3553, 1)), gem_string(";\n")));
#line 1507 "compiler/main.gem"
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
#line 1508 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1509 "compiler/main.gem"
    GemVal _t3554[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t3554, 1)), gem_string(";\n")));
#line 1510 "compiler/main.gem"
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
#line 1511 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1512 "compiler/main.gem"
    GemVal _t3555[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t3555, 1)), gem_string(";\n")));
#line 1513 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
#line 1515 "compiler/main.gem"
    GemVal _t3556[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3556, 1)), gem_string(";\n")));
#line 1516 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1519 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1522 "compiler/main.gem"
    GemVal _t3557[] = {gem_v_name};
    GemVal _t3558[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3557, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t3558, 2));
#line 1525 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1526 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1527 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1528 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1529 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1530 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1531 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1532 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1533 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1534 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1535 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1536 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1537 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1540 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1541 "compiler/main.gem"
    GemVal _t3559[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3559, 1), gem_int(0)))) {
#line 1542 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1544 "compiler/main.gem"
        GemVal gem_v__for_i_75 = gem_int(0);
#line 1544 "compiler/main.gem"
    GemVal _t3560[] = {gem_v_params};
        GemVal gem_v__for_limit_75 = gem_len_fn(NULL, _t3560, 1);
#line 1544 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_75, gem_v__for_limit_75))) break;
#line 1544 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_75;
#line 1544 "compiler/main.gem"
            gem_v__for_i_75 = gem_add(gem_v__for_i_75, gem_int(1));
#line 1545 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1546 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1548 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1549 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1550 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1551 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1552 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1553 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1554 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1555 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1556 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1557 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1558 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1559 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1560 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1562 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1567 "compiler/main.gem"
    GemVal _t3561[] = {gem_v_c_ret};
    GemVal _t3562[] = {gem_v_name};
    GemVal _t3563[] = {gem_v_c_params};
    GemVal _t3564[] = {(*gem_v_extern_c_decls), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3561, 1), gem_string(" ")), gem_to_string_fn(NULL, _t3562, 1)), gem_string("(")), gem_to_string_fn(NULL, _t3563, 1)), gem_string(");"))};
    (void)(gem_push_fn(NULL, _t3564, 2));
    GemVal _t3565 = gem_v_lines;
    gem_pop_frame();
    return _t3565;
}

struct _closure__anon_51 {
    GemVal *gem_v_mangle;
};
static GemVal _anon_51(void *_env, GemVal *args, int argc) {
    struct _closure__anon_51 *_cls = (struct _closure__anon_51 *)_env;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_cap_set = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_51", "compiler/main.gem", 0);
#line 1575 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1577 "compiler/main.gem"
    GemVal gem_v__for_i_76 = gem_int(0);
#line 1577 "compiler/main.gem"
    GemVal _t3567[] = {gem_v_params};
    GemVal gem_v__for_limit_76 = gem_len_fn(NULL, _t3567, 1);
#line 1577 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_76, gem_v__for_limit_76))) break;
#line 1577 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_76;
#line 1577 "compiler/main.gem"
        gem_v__for_i_76 = gem_add(gem_v__for_i_76, gem_int(1));
#line 1578 "compiler/main.gem"
    GemVal _t3568[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t3569 = (*gem_v_mangle);
        GemVal gem_v_mp = _t3569.fn(_t3569.env, _t3568, 1);
#line 1579 "compiler/main.gem"
    GemVal _t3570[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3570, 2))) {
#line 1580 "compiler/main.gem"
    GemVal _t3571[] = {gem_v_mp};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3571, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1581 "compiler/main.gem"
    GemVal _t3572[] = {gem_v_mp};
    GemVal _t3573[] = {gem_v_i};
    GemVal _t3574[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3572, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t3573, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t3574, 1)), gem_string("] : GEM_NIL;\n")));
        } else {
#line 1583 "compiler/main.gem"
    GemVal _t3575[] = {gem_v_mp};
    GemVal _t3576[] = {gem_v_i};
    GemVal _t3577[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3575, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t3576, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t3577, 1)), gem_string("] : GEM_NIL;\n")));
        }
    }

#line 1588 "compiler/main.gem"
    GemVal _t3578 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3578, gem_string("rest_param")), GEM_NIL))) {
#line 1589 "compiler/main.gem"
    GemVal _t3579[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t3579, 1);
#line 1590 "compiler/main.gem"
        GemVal gem_v_end_count = gem_int(0);
#line 1591 "compiler/main.gem"
    GemVal _t3580 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3580, gem_string("block_param")), GEM_NIL))) {
#line 1592 "compiler/main.gem"
            gem_v_end_count = gem_int(1);
        }
#line 1594 "compiler/main.gem"
    GemVal _t3581 = gem_v_node;
    GemVal _t3582[] = {gem_table_get(_t3581, gem_string("rest_param"))};
    GemVal _t3583 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t3583.fn(_t3583.env, _t3582, 1);
#line 1595 "compiler/main.gem"
    GemVal _t3584 = gem_v_node;
    GemVal _t3585[] = {gem_v_cap_set, gem_table_get(_t3584, gem_string("rest_param"))};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3585, 2))) {
#line 1596 "compiler/main.gem"
    GemVal _t3586[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3586, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1597 "compiler/main.gem"
    GemVal _t3587[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3587, 1)), gem_string(" = gem_table_new();\n")));
#line 1598 "compiler/main.gem"
    GemVal _t3588[] = {gem_v_N};
    GemVal _t3589[] = {gem_v_end_count};
    GemVal _t3590[] = {gem_v_mp_rest};
    GemVal _t3591[] = {gem_v_N};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t3588, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t3589, 1)), gem_string("; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t3590, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t3591, 1)), gem_string("), args[_ri]);\n")));
        } else {
#line 1600 "compiler/main.gem"
    GemVal _t3592[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3592, 1)), gem_string(" = gem_table_new();\n")));
#line 1601 "compiler/main.gem"
    GemVal _t3593[] = {gem_v_N};
    GemVal _t3594[] = {gem_v_end_count};
    GemVal _t3595[] = {gem_v_mp_rest};
    GemVal _t3596[] = {gem_v_N};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t3593, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t3594, 1)), gem_string("; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t3595, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t3596, 1)), gem_string("), args[_ri]);\n")));
        }
#line 1603 "compiler/main.gem"
    GemVal _t3597 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3597, gem_string("block_param")), GEM_NIL))) {
#line 1604 "compiler/main.gem"
    GemVal _t3598 = gem_v_node;
    GemVal _t3599[] = {gem_table_get(_t3598, gem_string("block_param"))};
    GemVal _t3600 = (*gem_v_mangle);
            GemVal gem_v_mp_block = _t3600.fn(_t3600.env, _t3599, 1);
#line 1605 "compiler/main.gem"
    GemVal _t3601 = gem_v_node;
    GemVal _t3602[] = {gem_v_cap_set, gem_table_get(_t3601, gem_string("block_param"))};
            if (gem_truthy(gem_fn_set_contains(NULL, _t3602, 2))) {
#line 1606 "compiler/main.gem"
    GemVal _t3603[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3603, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1607 "compiler/main.gem"
    GemVal _t3604[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3604, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            } else {
#line 1609 "compiler/main.gem"
    GemVal _t3605[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3605, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            }
        }
    }
    GemVal _t3606 = gem_v_out;
    gem_pop_frame();
    return _t3606;
}

struct _closure__anon_52 {
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
};
static GemVal _anon_52(void *_env, GemVal *args, int argc) {
    struct _closure__anon_52 *_cls = (struct _closure__anon_52 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal gem_v_body = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_52", "compiler/main.gem", 0);
#line 1619 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1620 "compiler/main.gem"
    GemVal _t3608[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3608, 1), gem_int(0)))) {
#line 1621 "compiler/main.gem"
        GemVal gem_v__for_i_77 = gem_int(0);
#line 1621 "compiler/main.gem"
    GemVal _t3609[] = {gem_v_body};
        GemVal gem_v__for_limit_77 = gem_sub(gem_len_fn(NULL, _t3609, 1), gem_int(1));
#line 1621 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_77, gem_v__for_limit_77))) break;
#line 1621 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_77;
#line 1621 "compiler/main.gem"
            gem_v__for_i_77 = gem_add(gem_v__for_i_77, gem_int(1));
#line 1622 "compiler/main.gem"
    GemVal _t3610[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t3611 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3611.fn(_t3611.env, _t3610, 2)), gem_string("\n"));
        }

#line 1624 "compiler/main.gem"
    GemVal _t3612[] = {gem_v_body};
    GemVal _t3613[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t3612, 1), gem_int(1))), gem_int(1)};
    GemVal _t3614 = (*gem_v_compile_stmt_return);
        gem_v_out = gem_add(gem_add(gem_v_out, _t3614.fn(_t3614.env, _t3613, 2)), gem_string("\n"));
    } else {
#line 1626 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
    GemVal _t3615 = gem_v_out;
    gem_pop_frame();
    return _t3615;
}

struct _closure__anon_53 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_source_name;
};
static GemVal _anon_53(void *_env, GemVal *args, int argc) {
    struct _closure__anon_53 *_cls = (struct _closure__anon_53 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_emit_fn_body = _cls->gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings = _cls->gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_53", "compiler/main.gem", 0);
#line 1634 "compiler/main.gem"
    GemVal gem_v__d78 = gem_v_node;
#line 1634 "compiler/main.gem"
    GemVal _t3617 = gem_v__d78;
    GemVal gem_v_params = gem_table_get(_t3617, gem_string("params"));
#line 1634 "compiler/main.gem"
    GemVal _t3618 = gem_v__d78;
    GemVal gem_v_body = gem_table_get(_t3618, gem_string("body"));
#line 1634 "compiler/main.gem"
    GemVal _t3619 = gem_v__d78;
    GemVal gem_v_name = gem_table_get(_t3619, gem_string("name"));

#line 1637 "compiler/main.gem"
    GemVal _t3620[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t3620, 1);
#line 1638 "compiler/main.gem"
    GemVal _t3621 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3621, gem_string("rest_param")), GEM_NIL))) {
#line 1639 "compiler/main.gem"
    GemVal _t3622 = gem_v_node;
    GemVal _t3623[] = {gem_v_scope_vars, gem_table_get(_t3622, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t3623, 2));
    }
#line 1641 "compiler/main.gem"
    GemVal _t3624 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3624, gem_string("block_param")), GEM_NIL))) {
#line 1642 "compiler/main.gem"
    GemVal _t3625 = gem_v_node;
    GemVal _t3626[] = {gem_v_scope_vars, gem_table_get(_t3625, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t3626, 2));
    }
#line 1644 "compiler/main.gem"
    GemVal _t3627[] = {gem_v_body, gem_v_scope_vars};
    (void)(gem_fn_collect_top_let_names(NULL, _t3627, 2));
#line 1645 "compiler/main.gem"
    GemVal _t3628[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t3629 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t3629.fn(_t3629.env, _t3628, 2);
#line 1647 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1648 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1649 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1650 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1652 "compiler/main.gem"
    GemVal _t3630[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3630, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1655 "compiler/main.gem"
    GemVal _t3631 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3631, gem_string("line")), GEM_NIL))) {
#line 1656 "compiler/main.gem"
    GemVal _t3632 = gem_v_node;
    GemVal _t3633[] = {gem_table_get(_t3632, gem_string("line"))};
    GemVal _t3634[] = {(*gem_v_source_name)};
    GemVal _t3635[] = {gem_fn_escape_c_string(NULL, _t3634, 1)};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t3633, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t3635, 1)), gem_string("\"\n")));
    }
#line 1659 "compiler/main.gem"
    GemVal _t3636[] = {gem_v_node, gem_v_params, gem_v_captured};
    GemVal _t3637 = (*gem_v_emit_param_bindings);
    gem_v_lines = gem_add(gem_v_lines, _t3637.fn(_t3637.env, _t3636, 3));
#line 1661 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1662 "compiler/main.gem"
    GemVal _t3638 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3638, gem_string("line")), GEM_NIL))) {
#line 1663 "compiler/main.gem"
    GemVal _t3639 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t3639, gem_string("line"));
    }
#line 1665 "compiler/main.gem"
    GemVal _t3640[] = {gem_v_name};
    GemVal _t3641[] = {gem_fn_escape_c_string(NULL, _t3640, 1)};
    GemVal _t3642[] = {(*gem_v_source_name)};
    GemVal _t3643[] = {gem_fn_escape_c_string(NULL, _t3642, 1)};
    GemVal _t3644[] = {gem_v_fn_line};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t3641, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t3643, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t3644, 1)), gem_string(");\n")));
#line 1667 "compiler/main.gem"
    GemVal _t3645[] = {gem_v_body};
    GemVal _t3646 = (*gem_v_emit_fn_body);
    gem_v_lines = gem_add(gem_v_lines, _t3646.fn(_t3646.env, _t3645, 1));
#line 1669 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1670 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1671 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
    GemVal _t3647 = gem_v_lines;
    gem_pop_frame();
    return _t3647;
}

struct _closure__anon_54 {
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
static GemVal _anon_54(void *_env, GemVal *args, int argc) {
    struct _closure__anon_54 *_cls = (struct _closure__anon_54 *)_env;
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
    gem_push_frame("_anon_54", "compiler/main.gem", 0);
#line 1678 "compiler/main.gem"
    GemVal gem_v__d79 = gem_v_node;
#line 1678 "compiler/main.gem"
    GemVal _t3649 = gem_v__d79;
    GemVal gem_v_params = gem_table_get(_t3649, gem_string("params"));
#line 1678 "compiler/main.gem"
    GemVal _t3650 = gem_v__d79;
    GemVal gem_v_body = gem_table_get(_t3650, gem_string("body"));

#line 1679 "compiler/main.gem"
    GemVal _t3651 = (*gem_v_anon_name);
    GemVal gem_v_name = _t3651.fn(_t3651.env, NULL, 0);
#line 1682 "compiler/main.gem"
    GemVal _t3652[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t3652, 1);
#line 1683 "compiler/main.gem"
    GemVal _t3653 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3653, gem_string("rest_param")), GEM_NIL))) {
#line 1684 "compiler/main.gem"
    GemVal _t3654 = gem_v_node;
    GemVal _t3655[] = {gem_v_inner_scope, gem_table_get(_t3654, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t3655, 2));
    }
#line 1686 "compiler/main.gem"
    GemVal _t3656 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3656, gem_string("block_param")), GEM_NIL))) {
#line 1687 "compiler/main.gem"
    GemVal _t3657 = gem_v_node;
    GemVal _t3658[] = {gem_v_inner_scope, gem_table_get(_t3657, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t3658, 2));
    }
#line 1689 "compiler/main.gem"
    GemVal _t3659[] = {gem_v_body, gem_v_inner_scope};
    (void)(gem_fn_collect_top_let_names(NULL, _t3659, 2));
#line 1690 "compiler/main.gem"
    GemVal _t3660[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t3661 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t3661.fn(_t3661.env, _t3660, 2);
#line 1692 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1693 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1694 "compiler/main.gem"
    GemVal _t3662[] = {gem_v_captures};
    GemVal _t3663[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t3662, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t3663, 2);
#line 1695 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1698 "compiler/main.gem"
    GemVal _t3664[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t3664, 1));
#line 1699 "compiler/main.gem"
    GemVal _t3665[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t3665, 1)), gem_string(" {\n"));
#line 1700 "compiler/main.gem"
    GemVal gem_v__for_items_80 = gem_v_captures;
#line 1700 "compiler/main.gem"
    GemVal gem_v__for_i_80 = gem_int(0);
#line 1700 "compiler/main.gem"
    while (1) {
        GemVal _t3666[] = {gem_v__for_items_80};
        if (!gem_truthy(gem_lt(gem_v__for_i_80, gem_len_fn(NULL, _t3666, 1)))) break;
#line 1700 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_80, gem_v__for_i_80);
#line 1700 "compiler/main.gem"
        gem_v__for_i_80 = gem_add(gem_v__for_i_80, gem_int(1));
#line 1701 "compiler/main.gem"
    GemVal _t3667[] = {gem_v_cap};
    GemVal _t3668 = (*gem_v_mangle);
    GemVal _t3669[] = {_t3668.fn(_t3668.env, _t3667, 1)};
        gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3669, 1)), gem_string(";\n")));
    }

#line 1703 "compiler/main.gem"
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
#line 1706 "compiler/main.gem"
    GemVal _t3670[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t3670, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1707 "compiler/main.gem"
    GemVal _t3671[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3671, 1), gem_int(0)))) {
#line 1708 "compiler/main.gem"
    GemVal _t3672[] = {gem_v_struct_name};
    GemVal _t3673[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t3672, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t3673, 1)), gem_string(" *)_env;\n")));
#line 1709 "compiler/main.gem"
        GemVal gem_v__for_items_81 = gem_v_captures;
#line 1709 "compiler/main.gem"
        GemVal gem_v__for_i_81 = gem_int(0);
#line 1709 "compiler/main.gem"
        while (1) {
            GemVal _t3674[] = {gem_v__for_items_81};
            if (!gem_truthy(gem_lt(gem_v__for_i_81, gem_len_fn(NULL, _t3674, 1)))) break;
#line 1709 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_81, gem_v__for_i_81);
#line 1709 "compiler/main.gem"
            gem_v__for_i_81 = gem_add(gem_v__for_i_81, gem_int(1));
#line 1710 "compiler/main.gem"
    GemVal _t3675[] = {gem_v_cap};
    GemVal _t3676 = (*gem_v_mangle);
            GemVal gem_v_mc = _t3676.fn(_t3676.env, _t3675, 1);
#line 1711 "compiler/main.gem"
    GemVal _t3677[] = {gem_v_mc};
    GemVal _t3678[] = {gem_v_mc};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3677, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t3678, 1)), gem_string(";\n")));
        }

    }
#line 1715 "compiler/main.gem"
    GemVal _t3679[] = {gem_v_node, gem_v_params, gem_v_inner_captured};
    GemVal _t3680 = (*gem_v_emit_param_bindings);
    gem_v_lines = gem_add(gem_v_lines, _t3680.fn(_t3680.env, _t3679, 3));
#line 1717 "compiler/main.gem"
    GemVal _t3681[] = {gem_v_name};
    GemVal _t3682[] = {gem_fn_escape_c_string(NULL, _t3681, 1)};
    GemVal _t3683[] = {(*gem_v_source_name)};
    GemVal _t3684[] = {gem_fn_escape_c_string(NULL, _t3683, 1)};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t3682, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t3684, 1)), gem_string("\", 0);\n")));
#line 1719 "compiler/main.gem"
    GemVal _t3685[] = {gem_v_body};
    GemVal _t3686 = (*gem_v_emit_fn_body);
    gem_v_lines = gem_add(gem_v_lines, _t3686.fn(_t3686.env, _t3685, 1));
#line 1721 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1722 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1723 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1726 "compiler/main.gem"
    GemVal _t3687[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t3687, 2));
    GemVal _t3688 = gem_table_new();
    gem_table_set(_t3688, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t3688, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t3688, gem_string("captures"), gem_v_captures);
    GemVal _t3689 = _t3688;
    gem_pop_frame();
    return _t3689;
}

struct _closure__anon_55 {
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
static GemVal _anon_55(void *_env, GemVal *args, int argc) {
    struct _closure__anon_55 *_cls = (struct _closure__anon_55 *)_env;
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
    gem_push_frame("_anon_55", "compiler/main.gem", 0);
#line 1735 "compiler/main.gem"
    GemVal _t3691 = gem_table_new();
    GemVal gem_v_fn_defs = _t3691;
#line 1736 "compiler/main.gem"
    GemVal _t3692 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t3692;
#line 1737 "compiler/main.gem"
    GemVal _t3693 = gem_table_new();
    GemVal gem_v_extern_includes = _t3693;
#line 1738 "compiler/main.gem"
    GemVal _t3694 = gem_table_new();
    GemVal gem_v_top_stmts = _t3694;
#line 1740 "compiler/main.gem"
    GemVal _t3695 = gem_v_ast;
    GemVal gem_v__for_items_82 = gem_table_get(_t3695, gem_string("stmts"));
#line 1740 "compiler/main.gem"
    GemVal gem_v__for_i_82 = gem_int(0);
#line 1740 "compiler/main.gem"
    while (1) {
        GemVal _t3696[] = {gem_v__for_items_82};
        if (!gem_truthy(gem_lt(gem_v__for_i_82, gem_len_fn(NULL, _t3696, 1)))) break;
#line 1740 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_82, gem_v__for_i_82);
#line 1740 "compiler/main.gem"
        gem_v__for_i_82 = gem_add(gem_v__for_i_82, gem_int(1));
#line 1741 "compiler/main.gem"
    GemVal _t3697 = gem_v_s;
        if (gem_truthy(gem_eq(gem_table_get(_t3697, gem_string("tag")), gem_string("fn_def")))) {
#line 1742 "compiler/main.gem"
    GemVal _t3698[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t3698, 2));
        } else {
#line 1743 "compiler/main.gem"
    GemVal _t3699 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t3699, gem_string("tag")), gem_string("extern_fn")))) {
#line 1744 "compiler/main.gem"
    GemVal _t3700[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t3700, 2));
            } else {
#line 1745 "compiler/main.gem"
    GemVal _t3701 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t3701, gem_string("tag")), gem_string("extern_include")))) {
#line 1746 "compiler/main.gem"
    GemVal _t3702[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t3702, 2));
                } else {
#line 1748 "compiler/main.gem"
    GemVal _t3703[] = {gem_v_top_stmts, gem_v_s};
                    (void)(gem_push_fn(NULL, _t3703, 2));
                }
            }
        }
    }

#line 1753 "compiler/main.gem"
    GemVal gem_v__for_items_83 = gem_v_extern_fns_list;
#line 1753 "compiler/main.gem"
    GemVal gem_v__for_i_83 = gem_int(0);
#line 1753 "compiler/main.gem"
    while (1) {
        GemVal _t3704[] = {gem_v__for_items_83};
        if (!gem_truthy(gem_lt(gem_v__for_i_83, gem_len_fn(NULL, _t3704, 1)))) break;
#line 1753 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_83, gem_v__for_i_83);
#line 1753 "compiler/main.gem"
        gem_v__for_i_83 = gem_add(gem_v__for_i_83, gem_int(1));
#line 1754 "compiler/main.gem"
    GemVal _t3705 = gem_v_ef;
    GemVal _t3706[] = {(*gem_v_defined_fns), gem_table_get(_t3705, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t3706, 2));
    }

#line 1758 "compiler/main.gem"
    GemVal gem_v__for_items_84 = gem_v_fn_defs;
#line 1758 "compiler/main.gem"
    GemVal gem_v__for_i_84 = gem_int(0);
#line 1758 "compiler/main.gem"
    while (1) {
        GemVal _t3707[] = {gem_v__for_items_84};
        if (!gem_truthy(gem_lt(gem_v__for_i_84, gem_len_fn(NULL, _t3707, 1)))) break;
#line 1758 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_84, gem_v__for_i_84);
#line 1758 "compiler/main.gem"
        gem_v__for_i_84 = gem_add(gem_v__for_i_84, gem_int(1));
#line 1759 "compiler/main.gem"
    GemVal _t3708 = gem_v_fd;
    GemVal _t3709[] = {(*gem_v_defined_fns), gem_table_get(_t3708, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t3709, 2));
#line 1760 "compiler/main.gem"
    GemVal _t3710 = gem_v_fd;
    GemVal _t3711[] = {gem_table_get(_t3710, gem_string("name"))};
        GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3711, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1761 "compiler/main.gem"
    GemVal _t3712[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t3712, 2));
    }

#line 1765 "compiler/main.gem"
    GemVal _t3713[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn_collect_top_let_names(NULL, _t3713, 2));
#line 1766 "compiler/main.gem"
    GemVal _t3714 = gem_table_new();
    GemVal _t3715[] = {gem_v_top_stmts, _t3714};
    GemVal _t3716 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t3716.fn(_t3716.env, _t3715, 2);
#line 1769 "compiler/main.gem"
    GemVal gem_v__for_items_85 = gem_v_extern_fns_list;
#line 1769 "compiler/main.gem"
    GemVal gem_v__for_i_85 = gem_int(0);
#line 1769 "compiler/main.gem"
    while (1) {
        GemVal _t3717[] = {gem_v__for_items_85};
        if (!gem_truthy(gem_lt(gem_v__for_i_85, gem_len_fn(NULL, _t3717, 1)))) break;
#line 1769 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_85, gem_v__for_i_85);
#line 1769 "compiler/main.gem"
        gem_v__for_i_85 = gem_add(gem_v__for_i_85, gem_int(1));
#line 1770 "compiler/main.gem"
    GemVal _t3718[] = {gem_v_ef};
    GemVal _t3719 = (*gem_v_compile_extern_fn);
    GemVal _t3720[] = {(*gem_v_functions), _t3719.fn(_t3719.env, _t3718, 1)};
        (void)(gem_push_fn(NULL, _t3720, 2));
    }

#line 1774 "compiler/main.gem"
    GemVal gem_v__for_items_86 = gem_v_fn_defs;
#line 1774 "compiler/main.gem"
    GemVal gem_v__for_i_86 = gem_int(0);
#line 1774 "compiler/main.gem"
    while (1) {
        GemVal _t3721[] = {gem_v__for_items_86};
        if (!gem_truthy(gem_lt(gem_v__for_i_86, gem_len_fn(NULL, _t3721, 1)))) break;
#line 1774 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_86, gem_v__for_i_86);
#line 1774 "compiler/main.gem"
        gem_v__for_i_86 = gem_add(gem_v__for_i_86, gem_int(1));
#line 1775 "compiler/main.gem"
    GemVal _t3722[] = {gem_v_fd};
    GemVal _t3723 = (*gem_v_compile_fn);
    GemVal _t3724[] = {(*gem_v_functions), _t3723.fn(_t3723.env, _t3722, 1)};
        (void)(gem_push_fn(NULL, _t3724, 2));
    }

#line 1779 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1780 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1781 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1782 "compiler/main.gem"
    GemVal _t3725[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t3726 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t3726.fn(_t3726.env, _t3725, 2);
#line 1783 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1784 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1786 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 1787 "compiler/main.gem"
    GemVal gem_v__for_items_87 = gem_v_fn_defs;
#line 1787 "compiler/main.gem"
    GemVal gem_v__for_i_87 = gem_int(0);
#line 1787 "compiler/main.gem"
    while (1) {
        GemVal _t3727[] = {gem_v__for_items_87};
        if (!gem_truthy(gem_lt(gem_v__for_i_87, gem_len_fn(NULL, _t3727, 1)))) break;
#line 1787 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_87, gem_v__for_i_87);
#line 1787 "compiler/main.gem"
        gem_v__for_i_87 = gem_add(gem_v__for_i_87, gem_int(1));
#line 1788 "compiler/main.gem"
    GemVal _t3728 = gem_v_fd;
        if (gem_truthy(gem_eq(gem_table_get(_t3728, gem_string("name")), gem_string("main")))) {
#line 1789 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 1793 "compiler/main.gem"
    GemVal gem_v_main_c = gem_string("int main(void) {\n");
#line 1794 "compiler/main.gem"
    GemVal _t3729[] = {(*gem_v_source_name)};
    GemVal _t3730[] = {gem_fn_escape_c_string(NULL, _t3729, 1)};
    gem_v_main_c = gem_add(gem_v_main_c, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t3730, 1)), gem_string("\", 0);\n")));
#line 1795 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1796 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
#line 1798 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1799 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
#line 1800 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
#line 1802 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_run_scheduler();\n"));
#line 1803 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_pop_frame();\n    return 0;\n}\n"));
#line 1807 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1808 "compiler/main.gem"
    GemVal gem_v__for_items_88 = gem_v_extern_includes;
#line 1808 "compiler/main.gem"
    GemVal gem_v__for_i_88 = gem_int(0);
#line 1808 "compiler/main.gem"
    while (1) {
        GemVal _t3731[] = {gem_v__for_items_88};
        if (!gem_truthy(gem_lt(gem_v__for_i_88, gem_len_fn(NULL, _t3731, 1)))) break;
#line 1808 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_88, gem_v__for_i_88);
#line 1808 "compiler/main.gem"
        gem_v__for_i_88 = gem_add(gem_v__for_i_88, gem_int(1));
#line 1809 "compiler/main.gem"
    GemVal _t3732 = gem_v_ei;
    GemVal _t3733[] = {gem_table_get(_t3732, gem_string("path"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t3733, 1)), gem_string("\"\n")));
    }

#line 1811 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
#line 1815 "compiler/main.gem"
    GemVal _t3734[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3734, 1), gem_int(0)))) {
#line 1816 "compiler/main.gem"
        GemVal gem_v__for_i_89 = gem_int(0);
#line 1816 "compiler/main.gem"
    GemVal _t3735[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_89 = gem_len_fn(NULL, _t3735, 1);
#line 1816 "compiler/main.gem"
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_89, gem_v__for_limit_89))) break;
#line 1816 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_89;
#line 1816 "compiler/main.gem"
            gem_v__for_i_89 = gem_add(gem_v__for_i_89, gem_int(1));
#line 1817 "compiler/main.gem"
            gem_v_out = gem_add(gem_add(gem_v_out, gem_table_get((*gem_v_extern_c_decls), gem_v_i)), gem_string("\n"));
        }

    }
#line 1822 "compiler/main.gem"
    GemVal gem_v__for_i_90 = gem_int(0);
#line 1822 "compiler/main.gem"
    GemVal _t3736[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_90 = gem_len_fn(NULL, _t3736, 1);
#line 1822 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_90, gem_v__for_limit_90))) break;
#line 1822 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_90;
#line 1822 "compiler/main.gem"
        gem_v__for_i_90 = gem_add(gem_v__for_i_90, gem_int(1));
#line 1823 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
#line 1824 "compiler/main.gem"
    GemVal _t3737[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3737, 1), gem_int(1))))) {
#line 1825 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1828 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1831 "compiler/main.gem"
    GemVal _t3738[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t3738, 1);
#line 1832 "compiler/main.gem"
    GemVal gem_v__for_i_91 = gem_int(0);
#line 1832 "compiler/main.gem"
    GemVal _t3739[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_91 = gem_len_fn(NULL, _t3739, 1);
#line 1832 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_91, gem_v__for_limit_91))) break;
#line 1832 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_91;
#line 1832 "compiler/main.gem"
        gem_v__for_i_91 = gem_add(gem_v__for_i_91, gem_int(1));
#line 1833 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 1834 "compiler/main.gem"
    GemVal _t3740[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3740, 2))) {
#line 1835 "compiler/main.gem"
    GemVal _t3741[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal *gem_v_"), gem_to_string_fn(NULL, _t3741, 1)), gem_string(";\n")));
        } else {
#line 1837 "compiler/main.gem"
    GemVal _t3742[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal gem_v_"), gem_to_string_fn(NULL, _t3742, 1)), gem_string(";\n")));
        }
    }

#line 1840 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1843 "compiler/main.gem"
    GemVal gem_v__for_i_92 = gem_int(0);
#line 1843 "compiler/main.gem"
    GemVal _t3743[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_92 = gem_len_fn(NULL, _t3743, 1);
#line 1843 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v__for_i_92, gem_v__for_limit_92))) break;
#line 1843 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_92;
#line 1843 "compiler/main.gem"
        gem_v__for_i_92 = gem_add(gem_v__for_i_92, gem_int(1));
#line 1844 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
#line 1845 "compiler/main.gem"
    GemVal _t3744[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3744, 1), gem_int(1))))) {
#line 1846 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
        }
    }

#line 1849 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
#line 1851 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    GemVal _t3745 = gem_v_out;
    gem_pop_frame();
    return _t3745;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 140 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_codegen", "compiler/main.gem", 140);
#line 141 "compiler/main.gem"
    GemVal _t1494 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1494;
#line 142 "compiler/main.gem"
    GemVal _t1495 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1495;
#line 143 "compiler/main.gem"
    GemVal _t1496 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1496;
#line 144 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 145 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 146 "compiler/main.gem"
    GemVal _t1497 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1497;
#line 147 "compiler/main.gem"
    GemVal _t1498 = gem_table_new();
    gem_table_set(_t1498, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1498, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1498, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1498, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1498, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1498, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1498, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1498, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1498, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1498, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1498, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1498, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1498, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1498, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1498, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1498, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1498, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1498, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1498, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1498, gem_string("self"), gem_string("gem_self_builtin"));
    gem_table_set(_t1498, gem_string("monitor"), gem_string("gem_monitor_builtin"));
    gem_table_set(_t1498, gem_string("spawn_monitor"), gem_string("gem_spawn_monitor_builtin"));
    gem_table_set(_t1498, gem_string("register"), gem_string("gem_register_builtin"));
    gem_table_set(_t1498, gem_string("whereis"), gem_string("gem_whereis_builtin"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1498;
#line 173 "compiler/main.gem"
    GemVal _t1499 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1499;
#line 174 "compiler/main.gem"
    GemVal gem_v__for_tbl_10 = (*gem_v_builtin_fns);
#line 174 "compiler/main.gem"
    GemVal _t1500[] = {gem_v__for_tbl_10};
    GemVal gem_v__for_keys_10 = gem_keys_fn(NULL, _t1500, 1);
#line 174 "compiler/main.gem"
    GemVal gem_v__for_i_10 = gem_int(0);
#line 174 "compiler/main.gem"
    while (1) {
        GemVal _t1501[] = {gem_v__for_keys_10};
        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t1501, 1)))) break;
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
    GemVal _t1502 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1502;
#line 178 "compiler/main.gem"
    GemVal _t1503 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1503;
#line 179 "compiler/main.gem"
    GemVal _t1504 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1504;
#line 180 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 184 "compiler/main.gem"
    struct _closure__anon_24 *_t1507 = GC_MALLOC(sizeof(struct _closure__anon_24));
    _t1507->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_24, _t1507);
#line 189 "compiler/main.gem"
    struct _closure__anon_25 *_t1510 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1510->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_25, _t1510);
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
    struct _closure__anon_27 *_t1829 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1829->gem_v_builtins = gem_v_builtins;
    _t1829->gem_v_collect_free_node = gem_v_collect_free_node;
    _t1829->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1829->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_27, _t1829);
#line 389 "compiler/main.gem"
    struct _closure__anon_28 *_t1843 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1843->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_28, _t1843);
#line 404 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 405 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 407 "compiler/main.gem"
    struct _closure__anon_29 *_t2017 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t2017->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2017->gem_v_walk_captures = gem_v_walk_captures;
    _t2017->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_29, _t2017);
#line 507 "compiler/main.gem"
    struct _closure__anon_30 *_t2029 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t2029->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_30, _t2029);
#line 518 "compiler/main.gem"
    struct _closure__anon_31 *_t2034 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t2034->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_31, _t2034);
#line 526 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 527 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 528 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 529 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 530 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 531 "compiler/main.gem"
    GemVal *gem_v_compile_if_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if_return = GEM_NIL;
#line 532 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 533 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 534 "compiler/main.gem"
    GemVal *gem_v_compile_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match_return = GEM_NIL;
#line 535 "compiler/main.gem"
    GemVal *gem_v_compile_receive_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_receive_match = GEM_NIL;
#line 536 "compiler/main.gem"
    GemVal *gem_v_compile_receive_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_receive_match_return = GEM_NIL;
#line 537 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 538 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 539 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 540 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 541 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 542 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 543 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 544 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 548 "compiler/main.gem"
    struct _closure__anon_32 *_t2233 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t2233->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2233->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2233->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t2233->gem_v_compile_array = gem_v_compile_array;
    _t2233->gem_v_compile_binop = gem_v_compile_binop;
    _t2233->gem_v_compile_call = gem_v_compile_call;
    _t2233->gem_v_compile_expr = gem_v_compile_expr;
    _t2233->gem_v_compile_table = gem_v_compile_table;
    _t2233->gem_v_defined_fns = gem_v_defined_fns;
    _t2233->gem_v_mangle = gem_v_mangle;
    _t2233->gem_v_tmp = gem_v_tmp;
    _t2233->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_32, _t2233);
#line 633 "compiler/main.gem"
    struct _closure__anon_33 *_t2250 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t2250->gem_v_compile_expr = gem_v_compile_expr;
    _t2250->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_33, _t2250);
#line 648 "compiler/main.gem"
    struct _closure__anon_34 *_t2264 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t2264->gem_v_compile_expr = gem_v_compile_expr;
    _t2264->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_34, _t2264);
#line 662 "compiler/main.gem"
    struct _closure__anon_35 *_t2308 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t2308->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2308->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2308->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t2308->gem_v_mangle = gem_v_mangle;
    _t2308->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_35, _t2308);
#line 697 "compiler/main.gem"
    struct _closure__anon_36 *_t2324 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t2324->gem_v_compile_expr = gem_v_compile_expr;
    _t2324->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_36, _t2324);
#line 718 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_37, NULL);
#line 727 "compiler/main.gem"
    struct _closure__anon_38 *_t2412 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2412->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2412->gem_v_compile_args = gem_v_compile_args;
    _t2412->gem_v_compile_expr = gem_v_compile_expr;
    _t2412->gem_v_defined_fns = gem_v_defined_fns;
    _t2412->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t2412->gem_v_source_name = gem_v_source_name;
    _t2412->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_38, _t2412);
#line 771 "compiler/main.gem"
    struct _closure__anon_39 *_t2565 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2565->gem_v_compile_expr = gem_v_compile_expr;
    _t2565->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_39, _t2565);
#line 850 "compiler/main.gem"
    struct _closure__anon_40 *_t2570 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t2570->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_40, _t2570);
#line 860 "compiler/main.gem"
    struct _closure__anon_41 *_t2821 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2821->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2821->gem_v_compile_expr = gem_v_compile_expr;
    _t2821->gem_v_compile_if = gem_v_compile_if;
    _t2821->gem_v_compile_match = gem_v_compile_match;
    _t2821->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t2821->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2821->gem_v_compile_while = gem_v_compile_while;
    _t2821->gem_v_in_top_level = gem_v_in_top_level;
    _t2821->gem_v_mangle = gem_v_mangle;
    _t2821->gem_v_source_name = gem_v_source_name;
    _t2821->gem_v_tmp = gem_v_tmp;
    _t2821->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t2821->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_41, _t2821);
#line 936 "compiler/main.gem"
    struct _closure__anon_42 *_t3046 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t3046->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3046->gem_v_compile_expr = gem_v_compile_expr;
    _t3046->gem_v_compile_if_return = gem_v_compile_if_return;
    _t3046->gem_v_compile_match_return = gem_v_compile_match_return;
    _t3046->gem_v_compile_receive_match_return = gem_v_compile_receive_match_return;
    _t3046->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3046->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3046->gem_v_compile_while = gem_v_compile_while;
    _t3046->gem_v_mangle = gem_v_mangle;
    _t3046->gem_v_source_name = gem_v_source_name;
    _t3046->gem_v_tmp = gem_v_tmp;
    _t3046->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_42, _t3046);
#line 1003 "compiler/main.gem"
    struct _closure__anon_43 *_t3065 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t3065->gem_v_compile_expr = gem_v_compile_expr;
    _t3065->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_43, _t3065);
#line 1019 "compiler/main.gem"
    struct _closure__anon_44 *_t3101 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t3101->gem_v_compile_expr = gem_v_compile_expr;
    _t3101->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3101->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_44, _t3101);
#line 1054 "compiler/main.gem"
    struct _closure__anon_45 *_t3119 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t3119->gem_v_compile_expr = gem_v_compile_expr;
    _t3119->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_45, _t3119);
#line 1069 "compiler/main.gem"
    struct _closure__anon_46 *_t3176 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t3176->gem_v_compile_expr = gem_v_compile_expr;
    _t3176->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3176->gem_v_compile_stmts = gem_v_compile_stmts;
    _t3176->gem_v_mangle = gem_v_mangle;
    _t3176->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_46, _t3176);
#line 1120 "compiler/main.gem"
    struct _closure__anon_47 *_t3259 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t3259->gem_v_compile_expr = gem_v_compile_expr;
    _t3259->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3259->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3259->gem_v_mangle = gem_v_mangle;
    _t3259->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_47, _t3259);
#line 1199 "compiler/main.gem"
    struct _closure__anon_48 *_t3378 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t3378->gem_v_compile_expr = gem_v_compile_expr;
    _t3378->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3378->gem_v_compile_stmts = gem_v_compile_stmts;
    _t3378->gem_v_mangle = gem_v_mangle;
    _t3378->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match = gem_make_fn(_anon_48, _t3378);
#line 1324 "compiler/main.gem"
    struct _closure__anon_49 *_t3515 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t3515->gem_v_compile_expr = gem_v_compile_expr;
    _t3515->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3515->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3515->gem_v_mangle = gem_v_mangle;
    _t3515->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match_return = gem_make_fn(_anon_49, _t3515);
#line 1452 "compiler/main.gem"
    struct _closure__anon_50 *_t3566 = GC_MALLOC(sizeof(struct _closure__anon_50));
    _t3566->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t3566->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_50, _t3566);
#line 1574 "compiler/main.gem"
    struct _closure__anon_51 *_t3607 = GC_MALLOC(sizeof(struct _closure__anon_51));
    _t3607->gem_v_mangle = gem_v_mangle;
    GemVal *gem_v_emit_param_bindings = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_param_bindings = gem_make_fn(_anon_51, _t3607);
#line 1618 "compiler/main.gem"
    struct _closure__anon_52 *_t3616 = GC_MALLOC(sizeof(struct _closure__anon_52));
    _t3616->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3616->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_fn_body = gem_make_fn(_anon_52, _t3616);
#line 1633 "compiler/main.gem"
    struct _closure__anon_53 *_t3648 = GC_MALLOC(sizeof(struct _closure__anon_53));
    _t3648->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3648->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t3648->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t3648->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3648->gem_v_in_top_level = gem_v_in_top_level;
    _t3648->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_fn = gem_make_fn(_anon_53, _t3648);
#line 1677 "compiler/main.gem"
    struct _closure__anon_54 *_t3690 = GC_MALLOC(sizeof(struct _closure__anon_54));
    _t3690->gem_v_anon_name = gem_v_anon_name;
    _t3690->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3690->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t3690->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t3690->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3690->gem_v_functions = gem_v_functions;
    _t3690->gem_v_in_top_level = gem_v_in_top_level;
    _t3690->gem_v_mangle = gem_v_mangle;
    _t3690->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_54, _t3690);
#line 1733 "compiler/main.gem"
    struct _closure__anon_55 *_t3746 = GC_MALLOC(sizeof(struct _closure__anon_55));
    _t3746->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3746->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t3746->gem_v_compile_fn = gem_v_compile_fn;
    _t3746->gem_v_compile_stmts = gem_v_compile_stmts;
    _t3746->gem_v_defined_fns = gem_v_defined_fns;
    _t3746->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t3746->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3746->gem_v_forward_decls = gem_v_forward_decls;
    _t3746->gem_v_functions = gem_v_functions;
    _t3746->gem_v_in_top_level = gem_v_in_top_level;
    _t3746->gem_v_source_name = gem_v_source_name;
    _t3746->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t3746->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_55, _t3746);
    GemVal _t3747 = gem_table_new();
    gem_table_set(_t3747, gem_string("compile"), gem_v_compile);
    GemVal _t3748 = _t3747;
    gem_pop_frame();
    return _t3748;
}

static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc) {
#line 1861 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("str_replace", "compiler/main.gem", 1861);
#line 1862 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 1863 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 1864 "compiler/main.gem"
    GemVal _t3749[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t3749, 1);
#line 1865 "compiler/main.gem"
    GemVal _t3750[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t3750, 1);
#line 1866 "compiler/main.gem"
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
#line 1867 "compiler/main.gem"
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
#line 1868 "compiler/main.gem"
            GemVal gem_v_found = gem_bool(1);
#line 1869 "compiler/main.gem"
            GemVal gem_v_j = gem_int(0);
#line 1870 "compiler/main.gem"
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
#line 1871 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
#line 1872 "compiler/main.gem"
                    gem_v_found = gem_bool(0);
                    break;
                }
#line 1875 "compiler/main.gem"
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
#line 1877 "compiler/main.gem"
            if (gem_truthy(gem_v_found)) {
#line 1878 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
#line 1879 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
#line 1883 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
#line 1884 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t3751 = gem_v_result;
    gem_pop_frame();
    return _t3751;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 24 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 24);
#line 25 "compiler/main.gem"
    GemVal _t3752 = gem_table_new();
    GemVal gem_v_new_stmts = _t3752;
#line 26 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 27 "compiler/main.gem"
    GemVal _t3753 = gem_v_ast;
    GemVal gem_v__for_items_1 = gem_table_get(_t3753, gem_string("stmts"));
#line 27 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 27 "compiler/main.gem"
    while (1) {
        GemVal _t3754[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t3754, 1)))) break;
#line 27 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 27 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 28 "compiler/main.gem"
    GemVal _t3755[] = {gem_v_stmt};
    GemVal _t3757;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3755, 1), gem_string("table")))) {
        _t3757 = gem_eq(gem_type_fn(NULL, _t3755, 1), gem_string("table"));
    } else {
        GemVal _t3756 = gem_v_stmt;
        _t3757 = gem_eq(gem_table_get(_t3756, gem_string("tag")), gem_string("load"));
    }
        if (gem_truthy(_t3757)) {
#line 29 "compiler/main.gem"
    GemVal _t3758 = gem_v_stmt;
            GemVal gem_v_path = gem_table_get(_t3758, gem_string("path"));
#line 31 "compiler/main.gem"
    GemVal _t3759[] = {gem_v_path};
    GemVal _t3767;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t3759, 1), gem_int(4)))) {
        _t3767 = gem_lt(gem_len_fn(NULL, _t3759, 1), gem_int(4));
    } else {
        GemVal _t3760[] = {gem_v_path};
        GemVal _t3762;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3760, 1), gem_int(4))), gem_string(".")))) {
                _t3762 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3760, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t3761[] = {gem_v_path};
                _t3762 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3761, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t3764;
        if (gem_truthy(_t3762)) {
                _t3764 = _t3762;
        } else {
                GemVal _t3763[] = {gem_v_path};
                _t3764 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3763, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t3766;
        if (gem_truthy(_t3764)) {
                _t3766 = _t3764;
        } else {
                GemVal _t3765[] = {gem_v_path};
                _t3766 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3765, 1), gem_int(1))), gem_string("m"));
        }
        _t3767 = _t3766;
    }
            if (gem_truthy(_t3767)) {
#line 32 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 36 "compiler/main.gem"
    GemVal _t3768[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_fn_gem_path_join(NULL, _t3768, 2);
#line 39 "compiler/main.gem"
    GemVal _t3769[] = {gem_v_full_path};
            if (gem_truthy(gem_eq(gem_fn_gem_file_exists(NULL, _t3769, 1), gem_int(0)))) {
#line 40 "compiler/main.gem"
    GemVal _t3770[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_fn_gem_path_join(NULL, _t3770, 2);
            }
#line 43 "compiler/main.gem"
    GemVal _t3771[] = {gem_v_full_path};
            gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t3771, 1);
#line 46 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                continue;
            }
#line 49 "compiler/main.gem"
            gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 51 "compiler/main.gem"
    GemVal _t3772[] = {gem_v_full_path};
            GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t3772, 1);
#line 52 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 53 "compiler/main.gem"
    GemVal _t3773[] = {gem_v_full_path};
    GemVal _t3774[] = {gem_add(gem_add(gem_string("cannot load '"), gem_to_string_fn(NULL, _t3773, 1)), gem_string("'"))};
                (void)(gem_error_at_fn("compiler/main.gem", 53, _t3774, 1));
            }
#line 56 "compiler/main.gem"
    GemVal _t3775[] = {gem_v_source};
            GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t3775, 1);
#line 57 "compiler/main.gem"
    GemVal _t3776[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t3776, 1);
#line 58 "compiler/main.gem"
    GemVal _t3777[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t3777, 3);
#line 60 "compiler/main.gem"
    GemVal _t3778 = gem_v_resolved;
            GemVal gem_v__for_items_2 = gem_table_get(_t3778, gem_string("stmts"));
#line 60 "compiler/main.gem"
            GemVal gem_v__for_i_2 = gem_int(0);
#line 60 "compiler/main.gem"
            while (1) {
                GemVal _t3779[] = {gem_v__for_items_2};
                if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t3779, 1)))) break;
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
    GemVal _t3780[] = {gem_v_new_stmts};
    GemVal _t3781 = gem_fn_make_program(NULL, _t3780, 1);
    gem_pop_frame();
    return _t3781;
}

int main(void) {
    gem_push_frame("main", "compiler/main.gem", 0);
#line 20 "compiler/main.gem"
    GemVal _t3782[] = {gem_int(0)};
    GemVal _t3783[] = {gem_fn_gem_get_argv(NULL, _t3782, 1)};
    GemVal _t3784[] = {gem_fn_gem_dirname(NULL, _t3783, 1)};
    gem_v_install_root = gem_fn_gem_dirname(NULL, _t3784, 1);
#line 74 "compiler/main.gem"
    gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
#line 75 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 76 "compiler/main.gem"
    GemVal _t3785[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 76, _t3785, 1));
    }
#line 79 "compiler/main.gem"
    GemVal _t3786[] = {gem_int(1)};
    gem_v_src_path = gem_fn_gem_get_argv(NULL, _t3786, 1);
#line 80 "compiler/main.gem"
    GemVal _t3787[] = {gem_v_src_path};
    gem_v_source = gem_fn_gem_read_file(NULL, _t3787, 1);
#line 81 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
#line 82 "compiler/main.gem"
    GemVal _t3788[] = {gem_v_src_path};
    GemVal _t3789[] = {gem_add(gem_add(gem_string("cannot read '"), gem_to_string_fn(NULL, _t3788, 1)), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 82, _t3789, 1));
    }
#line 85 "compiler/main.gem"
    GemVal _t3790[] = {gem_v_source};
    gem_v_ast = gem_fn_parse_source(NULL, _t3790, 1);
#line 86 "compiler/main.gem"
    GemVal _t3791[] = {gem_v_src_path};
    gem_v_base_dir = gem_fn_gem_dirname(NULL, _t3791, 1);
#line 87 "compiler/main.gem"
    GemVal _t3792 = gem_table_new();
    gem_v_loaded = _t3792;
#line 88 "compiler/main.gem"
    GemVal _t3793[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t3793, 3);
#line 90 "compiler/main.gem"
    GemVal _t3794[] = {gem_v_src_path};
    gem_v_cg = gem_fn_make_codegen(NULL, _t3794, 1);
#line 91 "compiler/main.gem"
    GemVal _t3795 = gem_v_cg;
    GemVal _t3796[] = {gem_v_resolved_ast};
    GemVal _t3797 = gem_table_get(_t3795, gem_string("compile"));
    gem_v_c_code = _t3797.fn(_t3797.env, _t3796, 1);
#line 92 "compiler/main.gem"
    GemVal _t3798[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t3798, 1));
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

