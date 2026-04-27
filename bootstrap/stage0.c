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
static GemVal gem_fn_get_source_line(void *_env, GemVal *args, int argc);
static GemVal gem_fn_repeat_char(void *_env, GemVal *args, int argc);
static GemVal gem_fn_compile_error(void *_env, GemVal *args, int argc);
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
static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc);
static GemVal gem_fn_collect_block_lets(void *_env, GemVal *args, int argc);
static GemVal gem_fn_collect_top_let_names(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc);
static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc);
static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc);
static GemVal gem_fn_atof(void *_env, GemVal *args, int argc);
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

static GemVal gem_fn_get_source_line(void *_env, GemVal *args, int argc) {
#line 3 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_line_num = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("get_source_line", "compiler/main.gem", 3);
#line 4 "compiler/main.gem"
    GemVal gem_v_current = gem_int(1);
#line 5 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 6 "compiler/main.gem"
    GemVal _t69[] = {gem_v_source};
    GemVal gem_v_slen = gem_len_fn(NULL, _t69, 1);
#line 7 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_slen))) break;
#line 8 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_current, gem_v_line_num))) {
#line 9 "compiler/main.gem"
            GemVal gem_v_start = gem_v_i;
#line 10 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t70;
                if (!gem_truthy(gem_lt(gem_v_i, gem_v_slen))) {
                                _t70 = gem_lt(gem_v_i, gem_v_slen);
                } else {
                                _t70 = gem_neq(gem_table_get(gem_v_source, gem_v_i), gem_string("\n"));
                }
                if (!gem_truthy(_t70)) break;
#line 11 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_int(1));
            }
#line 13 "compiler/main.gem"
    GemVal _t71[] = {gem_v_source, gem_v_start, gem_sub(gem_v_i, gem_v_start)};
            GemVal _t72 = gem_substr_fn(NULL, _t71, 3);
            gem_pop_frame();
            return _t72;
        }
#line 15 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_i), gem_string("\n")))) {
#line 16 "compiler/main.gem"
            gem_v_current = gem_add(gem_v_current, gem_int(1));
        }
#line 18 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t73 = gem_string("");
    gem_pop_frame();
    return _t73;
}

static GemVal gem_fn_repeat_char(void *_env, GemVal *args, int argc) {
#line 23 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("repeat_char", "compiler/main.gem", 23);
#line 24 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 25 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 25 "compiler/main.gem"
    GemVal gem_v__for_limit_1 = gem_v_n;
#line 25 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 25 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_1;
#line 25 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 26 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_v_ch);
    }

    GemVal _t74 = gem_v_result;
    gem_pop_frame();
    return _t74;
}

static GemVal gem_fn_compile_error(void *_env, GemVal *args, int argc) {
#line 31 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_col = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_span_len = (argc > 4) ? args[4] : GEM_NIL;
    GemVal gem_v_msg = (argc > 5) ? args[5] : GEM_NIL;
    GemVal gem_v_hint = (argc > 6) ? args[6] : GEM_NIL;
    gem_push_frame("compile_error", "compiler/main.gem", 31);
#line 32 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_file, GEM_NIL))) {
#line 33 "compiler/main.gem"
        gem_v_file = gem_string("<input>");
    }
#line 35 "compiler/main.gem"
    GemVal _t75;
    if (gem_truthy(gem_eq(gem_v_source, GEM_NIL))) {
        _t75 = gem_eq(gem_v_source, GEM_NIL);
    } else {
        _t75 = gem_eq(gem_v_line, GEM_NIL);
    }
    if (gem_truthy(_t75)) {
#line 36 "compiler/main.gem"
    GemVal _t76[] = {gem_add(gem_add(gem_string("\n[Compile Error]: "), gem_v_msg), gem_string("\n"))};
        (void)(gem_eprint_fn(NULL, _t76, 1));
#line 37 "compiler/main.gem"
    GemVal _t77[] = {gem_int(1)};
        (void)(gem_exit_process_fn(NULL, _t77, 1));
    }
#line 39 "compiler/main.gem"
    GemVal _t78;
    if (gem_truthy(gem_eq(gem_v_col, GEM_NIL))) {
        _t78 = gem_eq(gem_v_col, GEM_NIL);
    } else {
        _t78 = gem_lt(gem_v_col, gem_int(1));
    }
    if (gem_truthy(_t78)) {
#line 40 "compiler/main.gem"
        gem_v_col = gem_int(1);
    }
#line 43 "compiler/main.gem"
    GemVal _t79[] = {gem_v_line};
    GemVal gem_v_line_str = gem_to_string_fn(NULL, _t79, 1);
#line 44 "compiler/main.gem"
    GemVal _t80[] = {gem_v_line_str};
    GemVal gem_v_gutter_width = gem_len_fn(NULL, _t80, 1);
#line 45 "compiler/main.gem"
    GemVal _t81[] = {gem_string(" "), gem_v_gutter_width};
    GemVal gem_v_padding = gem_fn_repeat_char(NULL, _t81, 2);
#line 46 "compiler/main.gem"
    GemVal _t82[] = {gem_v_source, gem_v_line};
    GemVal gem_v_src_line = gem_fn_get_source_line(NULL, _t82, 2);
#line 48 "compiler/main.gem"
    GemVal gem_v_actual_span = gem_v_span_len;
#line 49 "compiler/main.gem"
    GemVal _t83;
    if (gem_truthy(gem_eq(gem_v_actual_span, GEM_NIL))) {
        _t83 = gem_eq(gem_v_actual_span, GEM_NIL);
    } else {
        _t83 = gem_lt(gem_v_actual_span, gem_int(1));
    }
    if (gem_truthy(_t83)) {
#line 50 "compiler/main.gem"
        gem_v_actual_span = gem_int(1);
    }
#line 53 "compiler/main.gem"
    GemVal gem_v_out = gem_add(gem_add(gem_string("\n[Compile Error]: "), gem_v_msg), gem_string("\n"));
#line 54 "compiler/main.gem"
    GemVal _t84[] = {gem_v_col};
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string("  --> ")), gem_v_file), gem_string(":")), gem_v_line_str), gem_string(":")), gem_to_string_fn(NULL, _t84, 1)), gem_string("\n"));
#line 55 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" |\n"));
#line 56 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_line_str), gem_string(" | ")), gem_v_src_line), gem_string("\n"));
#line 57 "compiler/main.gem"
    GemVal _t85[] = {gem_string(" "), gem_sub(gem_v_col, gem_int(1))};
    GemVal _t86[] = {gem_string("^"), gem_v_actual_span};
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" | ")), gem_fn_repeat_char(NULL, _t85, 2)), gem_fn_repeat_char(NULL, _t86, 2)), gem_string("\n"));
#line 58 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" |\n"));
#line 60 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_hint, GEM_NIL))) {
#line 61 "compiler/main.gem"
        gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string("Hint: ")), gem_v_hint), gem_string("\n"));
    }
#line 64 "compiler/main.gem"
    GemVal _t87[] = {gem_v_out};
    (void)(gem_eprint_fn(NULL, _t87, 1));
#line 65 "compiler/main.gem"
    GemVal _t88[] = {gem_int(1)};
    GemVal _t89 = gem_exit_process_fn(NULL, _t88, 1);
    gem_pop_frame();
    return _t89;
}

static GemVal gem_fn_is_digit(void *_env, GemVal *args, int argc) {
#line 15 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_digit", "compiler/main.gem", 15);
    GemVal _t90;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("0")))) {
        _t90 = gem_ge(gem_v_ch, gem_string("0"));
    } else {
        _t90 = gem_le(gem_v_ch, gem_string("9"));
    }
    GemVal _t91 = _t90;
    gem_pop_frame();
    return _t91;
}

static GemVal gem_fn_is_alpha(void *_env, GemVal *args, int argc) {
#line 19 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_alpha", "compiler/main.gem", 19);
    GemVal _t92;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("a")))) {
        _t92 = gem_ge(gem_v_ch, gem_string("a"));
    } else {
        _t92 = gem_le(gem_v_ch, gem_string("z"));
    }
    GemVal _t94;
    if (gem_truthy(_t92)) {
        _t94 = _t92;
    } else {
        GemVal _t93;
        if (!gem_truthy(gem_ge(gem_v_ch, gem_string("A")))) {
                _t93 = gem_ge(gem_v_ch, gem_string("A"));
        } else {
                _t93 = gem_le(gem_v_ch, gem_string("Z"));
        }
        _t94 = _t93;
    }
    GemVal _t95;
    if (gem_truthy(_t94)) {
        _t95 = _t94;
    } else {
        _t95 = gem_eq(gem_v_ch, gem_string("_"));
    }
    GemVal _t96 = _t95;
    gem_pop_frame();
    return _t96;
}

static GemVal gem_fn_is_alnum(void *_env, GemVal *args, int argc) {
#line 23 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_alnum", "compiler/main.gem", 23);
    GemVal _t97[] = {gem_v_ch};
    GemVal _t99;
    if (gem_truthy(gem_fn_is_alpha(NULL, _t97, 1))) {
        _t99 = gem_fn_is_alpha(NULL, _t97, 1);
    } else {
        GemVal _t98[] = {gem_v_ch};
        _t99 = gem_fn_is_digit(NULL, _t98, 1);
    }
    GemVal _t100 = _t99;
    gem_pop_frame();
    return _t100;
}

static GemVal gem_fn_digit_val(void *_env, GemVal *args, int argc) {
#line 29 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("digit_val", "compiler/main.gem", 29);
#line 30 "compiler/main.gem"
    GemVal gem_v__match_1 = gem_v_ch;
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("0")))) {
        GemVal _t101 = gem_int(0);
        gem_pop_frame();
        return _t101;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("1")))) {
        GemVal _t102 = gem_int(1);
        gem_pop_frame();
        return _t102;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("2")))) {
        GemVal _t103 = gem_int(2);
        gem_pop_frame();
        return _t103;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("3")))) {
        GemVal _t104 = gem_int(3);
        gem_pop_frame();
        return _t104;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("4")))) {
        GemVal _t105 = gem_int(4);
        gem_pop_frame();
        return _t105;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("5")))) {
        GemVal _t106 = gem_int(5);
        gem_pop_frame();
        return _t106;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("6")))) {
        GemVal _t107 = gem_int(6);
        gem_pop_frame();
        return _t107;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("7")))) {
        GemVal _t108 = gem_int(7);
        gem_pop_frame();
        return _t108;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("8")))) {
        GemVal _t109 = gem_int(8);
        gem_pop_frame();
        return _t109;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("9")))) {
        GemVal _t110 = gem_int(9);
        gem_pop_frame();
        return _t110;
    } else {
#line 52 "compiler/main.gem"
    GemVal _t111[] = {gem_v_ch};
    GemVal _t112[] = {gem_add(gem_string("not a digit: "), gem_to_string_fn(NULL, _t111, 1))};
        GemVal _t113 = gem_error_at_fn("compiler/main.gem", 52, _t112, 1);
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
    }
}

static GemVal gem_fn_str_to_int(void *_env, GemVal *args, int argc) {
#line 56 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("str_to_int", "compiler/main.gem", 56);
#line 57 "compiler/main.gem"
    GemVal gem_v_result = gem_int(0);
#line 58 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 58 "compiler/main.gem"
    GemVal _t114[] = {gem_v_s};
    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t114, 1);
#line 58 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 58 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_2;
#line 58 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 59 "compiler/main.gem"
    GemVal _t115[] = {gem_table_get(gem_v_s, gem_v_i)};
        gem_v_result = gem_add(gem_mul(gem_v_result, gem_int(10)), gem_fn_digit_val(NULL, _t115, 1));
    }

    GemVal _t116 = gem_v_result;
    gem_pop_frame();
    return _t116;
}

static GemVal gem_fn_contains_dot(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("contains_dot", "compiler/main.gem", 66);
#line 67 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 67 "compiler/main.gem"
    GemVal _t117[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t117, 1);
#line 67 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 67 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_3;
#line 67 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 68 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_string(".")))) {
#line 69 "compiler/main.gem"
            GemVal _t118 = gem_bool(1);
            gem_pop_frame();
            return _t118;
        }
    }

    GemVal _t119 = gem_bool(0);
    gem_pop_frame();
    return _t119;
}

static GemVal gem_fn_apply_escape(void *_env, GemVal *args, int argc) {
#line 80 "compiler/main.gem"
    GemVal gem_v_esc = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_mode = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("apply_escape", "compiler/main.gem", 80);
#line 81 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 82 "compiler/main.gem"
        GemVal _t120 = gem_string("\n");
        gem_pop_frame();
        return _t120;
    } else {
#line 83 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 84 "compiler/main.gem"
    GemVal _t121[] = {gem_int(13)};
            GemVal _t122 = gem_chr_fn(NULL, _t121, 1);
            gem_pop_frame();
            return _t122;
        } else {
#line 85 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 86 "compiler/main.gem"
                GemVal _t123 = gem_string("\t");
                gem_pop_frame();
                return _t123;
            } else {
#line 87 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 88 "compiler/main.gem"
                    GemVal _t124 = gem_string("\\");
                    gem_pop_frame();
                    return _t124;
                } else {
#line 89 "compiler/main.gem"
    GemVal _t125;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
        _t125 = gem_eq(gem_v_esc, gem_string("0"));
    } else {
        _t125 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                    if (gem_truthy(_t125)) {
#line 90 "compiler/main.gem"
    GemVal _t126[] = {gem_int(0)};
                        GemVal _t127 = gem_chr_fn(NULL, _t126, 1);
                        gem_pop_frame();
                        return _t127;
                    } else {
#line 91 "compiler/main.gem"
    GemVal _t128;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
        _t128 = gem_eq(gem_v_esc, gem_string("\""));
    } else {
        _t128 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                        if (gem_truthy(_t128)) {
#line 92 "compiler/main.gem"
                            GemVal _t129 = gem_string("\"");
                            gem_pop_frame();
                            return _t129;
                        } else {
#line 93 "compiler/main.gem"
    GemVal _t130;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
        _t130 = gem_eq(gem_v_esc, gem_string("{"));
    } else {
        _t130 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                            if (gem_truthy(_t130)) {
#line 94 "compiler/main.gem"
                                GemVal _t131 = gem_string("{");
                                gem_pop_frame();
                                return _t131;
                            } else {
#line 95 "compiler/main.gem"
    GemVal _t132;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
        _t132 = gem_eq(gem_v_esc, gem_string("}"));
    } else {
        _t132 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                                if (gem_truthy(_t132)) {
#line 96 "compiler/main.gem"
                                    GemVal _t133 = gem_string("}");
                                    gem_pop_frame();
                                    return _t133;
                                } else {
#line 97 "compiler/main.gem"
    GemVal _t134;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("'")))) {
        _t134 = gem_eq(gem_v_esc, gem_string("'"));
    } else {
        _t134 = gem_eq(gem_v_mode, gem_string("sq"));
    }
                                    if (gem_truthy(_t134)) {
#line 98 "compiler/main.gem"
                                        GemVal _t135 = gem_string("'");
                                        gem_pop_frame();
                                        return _t135;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t136 = GEM_NIL;
    gem_pop_frame();
    return _t136;
}

static GemVal gem_fn_build_keyword_set(void *_env, GemVal *args, int argc) {
#line 105 "compiler/main.gem"
    gem_push_frame("build_keyword_set", "compiler/main.gem", 105);
#line 106 "compiler/main.gem"
    GemVal _t137 = gem_table_new();
    GemVal gem_v_t = _t137;
#line 107 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("fn"), gem_bool(1));
#line 108 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("end"), gem_bool(1));
#line 109 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("let"), gem_bool(1));
#line 110 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("if"), gem_bool(1));
#line 111 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("elif"), gem_bool(1));
#line 112 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("else"), gem_bool(1));
#line 113 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("while"), gem_bool(1));
#line 114 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("return"), gem_bool(1));
#line 115 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("break"), gem_bool(1));
#line 116 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("continue"), gem_bool(1));
#line 117 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("do"), gem_bool(1));
#line 118 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("match"), gem_bool(1));
#line 119 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("when"), gem_bool(1));
#line 120 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("and"), gem_bool(1));
#line 121 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("or"), gem_bool(1));
#line 122 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("not"), gem_bool(1));
#line 123 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("true"), gem_bool(1));
#line 124 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("false"), gem_bool(1));
#line 125 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("nil"), gem_bool(1));
#line 126 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("load"), gem_bool(1));
#line 127 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("extern"), gem_bool(1));
#line 128 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("for"), gem_bool(1));
#line 129 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("in"), gem_bool(1));
#line 130 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("after"), gem_bool(1));
    GemVal _t138 = gem_v_t;
    gem_pop_frame();
    return _t138;
}

static GemVal gem_fn_tokenize(void *_env, GemVal *args, int argc) {
#line 134 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("tokenize", "compiler/main.gem", 134);
#line 135 "compiler/main.gem"
    GemVal gem_v_kw = gem_fn_build_keyword_set(NULL, NULL, 0);
#line 136 "compiler/main.gem"
    GemVal _t139 = gem_table_new();
    GemVal gem_v_tokens = _t139;
#line 137 "compiler/main.gem"
    GemVal gem_v_pos = gem_int(0);
#line 138 "compiler/main.gem"
    GemVal gem_v_line = gem_int(1);
#line 139 "compiler/main.gem"
    GemVal gem_v_line_start = gem_int(0);
#line 140 "compiler/main.gem"
    GemVal _t140[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t140, 1);
#line 141 "compiler/main.gem"
    GemVal gem_v_interp_depth = gem_int(0);
#line 142 "compiler/main.gem"
    GemVal _t141 = gem_table_new();
    GemVal gem_v_interp_brace = _t141;
#line 143 "compiler/main.gem"
    GemVal _t142[] = {gem_int(13)};
    GemVal gem_v_CR = gem_chr_fn(NULL, _t142, 1);
#line 144 "compiler/main.gem"
    GemVal _t143[] = {gem_int(0)};
    GemVal gem_v_NUL = gem_chr_fn(NULL, _t143, 1);
#line 145 "compiler/main.gem"
    GemVal gem_v_in_triple_dq = gem_bool(0);
#line 146 "compiler/main.gem"
    GemVal gem_v_triple_dq_indent = gem_int(0);
#line 147 "compiler/main.gem"
    GemVal gem_v_tstart = gem_int(0);
#line 149 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 150 "compiler/main.gem"
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
#line 151 "compiler/main.gem"
        gem_v_tstart = gem_v_pos;
#line 154 "compiler/main.gem"
    GemVal _t144;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t144 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t144 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t144)) {
#line 155 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 160 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 161 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t145;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t145 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t145 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t145)) break;
#line 162 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 168 "compiler/main.gem"
    GemVal _t146;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t146 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t146 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t146)) {
#line 169 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 170 "compiler/main.gem"
    GemVal _t147[] = {gem_v_tokens};
    GemVal _t150;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t147, 1), gem_int(0)))) {
        _t150 = gem_gt(gem_len_fn(NULL, _t147, 1), gem_int(0));
    } else {
        GemVal _t148[] = {gem_v_tokens};
        GemVal _t149 = gem_table_get(gem_v_tokens, gem_sub(gem_len_fn(NULL, _t148, 1), gem_int(1)));
        _t150 = gem_eq(gem_table_get(_t149, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t150)) {
#line 171 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 173 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 174 "compiler/main.gem"
    GemVal _t151 = gem_table_new();
    gem_table_set(_t151, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t151, gem_string("value"), gem_string("\n"));
    gem_table_set(_t151, gem_string("line"), gem_v_line);
    gem_table_set(_t151, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t152[] = {gem_v_tokens, _t151};
                (void)(gem_push_fn(NULL, _t152, 2));
            }
#line 176 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 177 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 178 "compiler/main.gem"
                gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
            }
#line 180 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 182 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 183 "compiler/main.gem"
                GemVal gem_v_c2 = gem_table_get(gem_v_source, gem_v_pos);
#line 184 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_c2, gem_string("\n")))) {
#line 185 "compiler/main.gem"
                    gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 186 "compiler/main.gem"
                    gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 187 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                } else {
#line 188 "compiler/main.gem"
    GemVal _t153;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t153 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t153 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t154;
    if (gem_truthy(_t153)) {
        _t154 = _t153;
    } else {
        _t154 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t154)) {
#line 189 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 190 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 191 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t155;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t155 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t155 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t155)) break;
#line 192 "compiler/main.gem"
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
#line 202 "compiler/main.gem"
    GemVal _t156[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t156, 1))) {
#line 203 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 204 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t158;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t158 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t157[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t158 = gem_fn_is_digit(NULL, _t157, 1);
                }
                if (!gem_truthy(_t158)) break;
#line 205 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 206 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 208 "compiler/main.gem"
    GemVal _t159;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t159 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t159 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t159;
#line 209 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 210 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 211 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 212 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t161;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t161 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t160[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t161 = gem_fn_is_digit(NULL, _t160, 1);
                    }
                    if (!gem_truthy(_t161)) break;
#line 213 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 214 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 217 "compiler/main.gem"
    GemVal _t162 = gem_table_new();
    gem_table_set(_t162, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t162, gem_string("value"), gem_v_val);
    gem_table_set(_t162, gem_string("line"), gem_v_line);
    gem_table_set(_t162, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t163[] = {gem_v_tokens, _t162};
            (void)(gem_push_fn(NULL, _t163, 2));
            continue;
        }
#line 222 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 223 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 226 "compiler/main.gem"
    GemVal _t164;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t164 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t164 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t165;
    if (!gem_truthy(_t164)) {
        _t165 = _t164;
    } else {
        _t165 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
            if (gem_truthy(_t165)) {
#line 227 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 229 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t167;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t167 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t166;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t166 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t166 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t167 = _t166;
                    }
                    if (!gem_truthy(_t167)) break;
#line 230 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 232 "compiler/main.gem"
    GemVal _t168;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t168 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t168 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t168)) {
#line 233 "compiler/main.gem"
    GemVal _t169[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("triple-quoted string must be followed by a newline"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t169, 7));
                }
#line 235 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 236 "compiler/main.gem"
                gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 237 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 240 "compiler/main.gem"
                GemVal gem_v_tq_scan = gem_v_pos;
#line 241 "compiler/main.gem"
                GemVal gem_v_tq_bi = gem_int(0);
#line 242 "compiler/main.gem"
                GemVal gem_v_tq_found = gem_bool(0);
#line 243 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t170;
                    if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                        _t170 = gem_lt(gem_v_tq_scan, gem_v_length);
                    } else {
                                        _t170 = gem_not(gem_v_tq_found);
                    }
                    if (!gem_truthy(_t170)) break;
#line 244 "compiler/main.gem"
                    GemVal gem_v_tq_ind = gem_int(0);
#line 245 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t171;
                        if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                _t171 = gem_lt(gem_v_tq_scan, gem_v_length);
                        } else {
                                                _t171 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t171)) break;
#line 246 "compiler/main.gem"
                        gem_v_tq_ind = gem_add(gem_v_tq_ind, gem_int(1));
#line 247 "compiler/main.gem"
                        gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                    }
#line 249 "compiler/main.gem"
    GemVal _t172;
    if (!gem_truthy(gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length))) {
        _t172 = gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length);
    } else {
        _t172 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\""));
    }
    GemVal _t173;
    if (!gem_truthy(_t172)) {
        _t173 = _t172;
    } else {
        _t173 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(1))), gem_string("\""));
    }
    GemVal _t174;
    if (!gem_truthy(_t173)) {
        _t174 = _t173;
    } else {
        _t174 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(2))), gem_string("\""));
    }
                    if (gem_truthy(_t174)) {
#line 250 "compiler/main.gem"
                        gem_v_tq_bi = gem_v_tq_ind;
#line 251 "compiler/main.gem"
                        gem_v_tq_found = gem_bool(1);
                    } else {
#line 253 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t175;
                            if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                        _t175 = gem_lt(gem_v_tq_scan, gem_v_length);
                            } else {
                                                        _t175 = gem_neq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t175)) break;
#line 254 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
#line 256 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
#line 257 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
                    }
                }
#line 261 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_tq_found))) {
#line 262 "compiler/main.gem"
    GemVal _t176[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t176, 7));
                }
#line 264 "compiler/main.gem"
                gem_v_triple_dq_indent = gem_v_tq_bi;
#line 267 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 268 "compiler/main.gem"
                GemVal gem_v_hit_interp = gem_bool(0);
#line 269 "compiler/main.gem"
                GemVal gem_v_tq_als = gem_bool(1);
#line 271 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 272 "compiler/main.gem"
                    if (gem_truthy(gem_v_tq_als)) {
#line 273 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(0);
#line 274 "compiler/main.gem"
                        GemVal gem_v_tq_skip = gem_int(0);
#line 275 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t177;
                            if (!gem_truthy(gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent))) {
                                                        _t177 = gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t177 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t178;
                            if (!gem_truthy(_t177)) {
                                                        _t178 = _t177;
                            } else {
                                                        _t178 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t178)) break;
#line 276 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 277 "compiler/main.gem"
                            gem_v_tq_skip = gem_add(gem_v_tq_skip, gem_int(1));
                        }
#line 280 "compiler/main.gem"
    GemVal _t179;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t179 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t179 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t180;
    if (!gem_truthy(_t179)) {
        _t180 = _t179;
    } else {
        _t180 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t181;
    if (!gem_truthy(_t180)) {
        _t181 = _t180;
    } else {
        _t181 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t181)) {
#line 281 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 283 "compiler/main.gem"
    GemVal _t182[] = {gem_v_val};
    GemVal _t184;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t182, 1), gem_int(0)))) {
        _t184 = gem_gt(gem_len_fn(NULL, _t182, 1), gem_int(0));
    } else {
        GemVal _t183[] = {gem_v_val};
        _t184 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t183, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t184)) {
#line 284 "compiler/main.gem"
    GemVal _t185[] = {gem_v_val};
    GemVal _t186[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t185, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t186, 3);
                            }
                            break;
                        }
                    }
#line 290 "compiler/main.gem"
                    GemVal gem_v_tqc = gem_table_get(gem_v_source, gem_v_pos);
#line 291 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\n")))) {
#line 292 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 293 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 294 "compiler/main.gem"
                        gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 295 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 296 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(1);
                        continue;
                    }
#line 299 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\\")))) {
#line 300 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 301 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 302 "compiler/main.gem"
    GemVal _t187[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                            (void)(gem_fn_compile_error(NULL, _t187, 7));
                        }
#line 304 "compiler/main.gem"
                        GemVal gem_v_tqe = gem_table_get(gem_v_source, gem_v_pos);
#line 305 "compiler/main.gem"
    GemVal _t188[] = {gem_v_tqe, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t188, 2);
#line 306 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 307 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 309 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqe);
                        }
#line 311 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 314 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("{")))) {
#line 315 "compiler/main.gem"
    GemVal _t189 = gem_table_new();
    gem_table_set(_t189, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t189, gem_string("value"), gem_string(""));
    gem_table_set(_t189, gem_string("line"), gem_v_line);
    gem_table_set(_t189, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t190[] = {gem_v_tokens, _t189};
                        (void)(gem_push_fn(NULL, _t190, 2));
#line 316 "compiler/main.gem"
    GemVal _t191 = gem_table_new();
    gem_table_set(_t191, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t191, gem_string("value"), gem_v_val);
    gem_table_set(_t191, gem_string("line"), gem_v_line);
    gem_table_set(_t191, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t192[] = {gem_v_tokens, _t191};
                        (void)(gem_push_fn(NULL, _t192, 2));
#line 317 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 318 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 319 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 320 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 321 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 322 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 325 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqc);
#line 326 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 329 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 330 "compiler/main.gem"
    GemVal _t193 = gem_table_new();
    gem_table_set(_t193, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t193, gem_string("value"), gem_v_val);
    gem_table_set(_t193, gem_string("line"), gem_v_line);
    gem_table_set(_t193, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t194[] = {gem_v_tokens, _t193};
                    (void)(gem_push_fn(NULL, _t194, 2));
                }
                continue;
            }
#line 336 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 337 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 338 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t195;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t195 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t195 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t195)) break;
#line 339 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 340 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 341 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 342 "compiler/main.gem"
    GemVal _t196[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t196, 7));
                    }
#line 344 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 345 "compiler/main.gem"
    GemVal _t197[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t197, 2);
#line 346 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 347 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 349 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 351 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 354 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 355 "compiler/main.gem"
    GemVal _t198 = gem_table_new();
    gem_table_set(_t198, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t198, gem_string("value"), gem_string(""));
    gem_table_set(_t198, gem_string("line"), gem_v_line);
    gem_table_set(_t198, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t199[] = {gem_v_tokens, _t198};
                    (void)(gem_push_fn(NULL, _t199, 2));
#line 356 "compiler/main.gem"
    GemVal _t200 = gem_table_new();
    gem_table_set(_t200, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t200, gem_string("value"), gem_v_val);
    gem_table_set(_t200, gem_string("line"), gem_v_line);
    gem_table_set(_t200, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t201[] = {gem_v_tokens, _t200};
                    (void)(gem_push_fn(NULL, _t201, 2));
#line 357 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 358 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 359 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 360 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 361 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 364 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 365 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 367 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 368 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 369 "compiler/main.gem"
    GemVal _t202[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string"), gem_string("this string is never closed")};
                    (void)(gem_fn_compile_error(NULL, _t202, 7));
                }
#line 371 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 372 "compiler/main.gem"
    GemVal _t203 = gem_table_new();
    gem_table_set(_t203, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t203, gem_string("value"), gem_v_val);
    gem_table_set(_t203, gem_string("line"), gem_v_line);
    gem_table_set(_t203, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t204[] = {gem_v_tokens, _t203};
                (void)(gem_push_fn(NULL, _t204, 2));
            }
            continue;
        }
#line 378 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("'")))) {
#line 379 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 382 "compiler/main.gem"
    GemVal _t205;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t205 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t205 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t206;
    if (!gem_truthy(_t205)) {
        _t206 = _t205;
    } else {
        _t206 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
            if (gem_truthy(_t206)) {
#line 383 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 385 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t208;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t208 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t207;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t207 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t207 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t208 = _t207;
                    }
                    if (!gem_truthy(_t208)) break;
#line 386 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 388 "compiler/main.gem"
    GemVal _t209;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t209 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t209 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t209)) {
#line 389 "compiler/main.gem"
    GemVal _t210[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("triple-quoted string must be followed by a newline"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t210, 7));
                }
#line 391 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 392 "compiler/main.gem"
                gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 393 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 396 "compiler/main.gem"
                GemVal gem_v_sq_scan = gem_v_pos;
#line 397 "compiler/main.gem"
                GemVal gem_v_sq_bi = gem_int(0);
#line 398 "compiler/main.gem"
                GemVal gem_v_sq_found = gem_bool(0);
#line 399 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t211;
                    if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                        _t211 = gem_lt(gem_v_sq_scan, gem_v_length);
                    } else {
                                        _t211 = gem_not(gem_v_sq_found);
                    }
                    if (!gem_truthy(_t211)) break;
#line 400 "compiler/main.gem"
                    GemVal gem_v_sq_ind = gem_int(0);
#line 401 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t212;
                        if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                _t212 = gem_lt(gem_v_sq_scan, gem_v_length);
                        } else {
                                                _t212 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t212)) break;
#line 402 "compiler/main.gem"
                        gem_v_sq_ind = gem_add(gem_v_sq_ind, gem_int(1));
#line 403 "compiler/main.gem"
                        gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                    }
#line 405 "compiler/main.gem"
    GemVal _t213;
    if (!gem_truthy(gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length))) {
        _t213 = gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length);
    } else {
        _t213 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("'"));
    }
    GemVal _t214;
    if (!gem_truthy(_t213)) {
        _t214 = _t213;
    } else {
        _t214 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(1))), gem_string("'"));
    }
    GemVal _t215;
    if (!gem_truthy(_t214)) {
        _t215 = _t214;
    } else {
        _t215 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(2))), gem_string("'"));
    }
                    if (gem_truthy(_t215)) {
#line 406 "compiler/main.gem"
                        gem_v_sq_bi = gem_v_sq_ind;
#line 407 "compiler/main.gem"
                        gem_v_sq_found = gem_bool(1);
                    } else {
#line 409 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t216;
                            if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                        _t216 = gem_lt(gem_v_sq_scan, gem_v_length);
                            } else {
                                                        _t216 = gem_neq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t216)) break;
#line 410 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
#line 412 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
#line 413 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
                    }
                }
#line 417 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_sq_found))) {
#line 418 "compiler/main.gem"
    GemVal _t217[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t217, 7));
                }
#line 422 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 423 "compiler/main.gem"
                GemVal gem_v_sq_als = gem_bool(1);
#line 425 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 426 "compiler/main.gem"
                    if (gem_truthy(gem_v_sq_als)) {
#line 427 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(0);
#line 428 "compiler/main.gem"
                        GemVal gem_v_sq_skip = gem_int(0);
#line 429 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t218;
                            if (!gem_truthy(gem_lt(gem_v_sq_skip, gem_v_sq_bi))) {
                                                        _t218 = gem_lt(gem_v_sq_skip, gem_v_sq_bi);
                            } else {
                                                        _t218 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t219;
                            if (!gem_truthy(_t218)) {
                                                        _t219 = _t218;
                            } else {
                                                        _t219 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t219)) break;
#line 430 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 431 "compiler/main.gem"
                            gem_v_sq_skip = gem_add(gem_v_sq_skip, gem_int(1));
                        }
#line 434 "compiler/main.gem"
    GemVal _t220;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t220 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t220 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t221;
    if (!gem_truthy(_t220)) {
        _t221 = _t220;
    } else {
        _t221 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
    GemVal _t222;
    if (!gem_truthy(_t221)) {
        _t222 = _t221;
    } else {
        _t222 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("'"));
    }
                        if (gem_truthy(_t222)) {
#line 435 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 437 "compiler/main.gem"
    GemVal _t223[] = {gem_v_val};
    GemVal _t225;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t223, 1), gem_int(0)))) {
        _t225 = gem_gt(gem_len_fn(NULL, _t223, 1), gem_int(0));
    } else {
        GemVal _t224[] = {gem_v_val};
        _t225 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t224, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t225)) {
#line 438 "compiler/main.gem"
    GemVal _t226[] = {gem_v_val};
    GemVal _t227[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t226, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t227, 3);
                            }
                            break;
                        }
                    }
#line 444 "compiler/main.gem"
                    GemVal gem_v_sqc = gem_table_get(gem_v_source, gem_v_pos);
#line 445 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\n")))) {
#line 446 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 447 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 448 "compiler/main.gem"
                        gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 449 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 450 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(1);
                        continue;
                    }
#line 453 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\\")))) {
#line 454 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 455 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 456 "compiler/main.gem"
    GemVal _t228[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                            (void)(gem_fn_compile_error(NULL, _t228, 7));
                        }
#line 458 "compiler/main.gem"
                        GemVal gem_v_sqe = gem_table_get(gem_v_source, gem_v_pos);
#line 459 "compiler/main.gem"
    GemVal _t229[] = {gem_v_sqe, gem_string("sq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t229, 2);
#line 460 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 461 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 463 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_sqe);
                        }
#line 465 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 468 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_sqc);
#line 469 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 472 "compiler/main.gem"
    GemVal _t230 = gem_table_new();
    gem_table_set(_t230, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t230, gem_string("value"), gem_v_val);
    gem_table_set(_t230, gem_string("line"), gem_v_line);
    gem_table_set(_t230, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t231[] = {gem_v_tokens, _t230};
                (void)(gem_push_fn(NULL, _t231, 2));
                continue;
            }
#line 477 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 478 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t232;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t232 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t232 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
                }
                if (!gem_truthy(_t232)) break;
#line 479 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 480 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 481 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 482 "compiler/main.gem"
    GemVal _t233[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t233, 7));
                    }
#line 484 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 485 "compiler/main.gem"
    GemVal _t234[] = {gem_v_esc, gem_string("sq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t234, 2);
#line 486 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 487 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 489 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
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
    GemVal _t235[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string"), gem_string("this string is never closed")};
                (void)(gem_fn_compile_error(NULL, _t235, 7));
            }
#line 500 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 501 "compiler/main.gem"
    GemVal _t236 = gem_table_new();
    gem_table_set(_t236, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t236, gem_string("value"), gem_v_val);
    gem_table_set(_t236, gem_string("line"), gem_v_line);
    gem_table_set(_t236, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t237[] = {gem_v_tokens, _t236};
            (void)(gem_push_fn(NULL, _t237, 2));
            continue;
        }
#line 506 "compiler/main.gem"
    GemVal _t238[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t238, 1))) {
#line 507 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 508 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t240;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t240 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t239[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t240 = gem_fn_is_alnum(NULL, _t239, 1);
                }
                if (!gem_truthy(_t240)) break;
#line 509 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 510 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 512 "compiler/main.gem"
    GemVal _t241[] = {gem_v_kw, gem_v_val};
            if (gem_truthy(gem_has_key_fn(NULL, _t241, 2))) {
#line 513 "compiler/main.gem"
    GemVal _t242 = gem_table_new();
    gem_table_set(_t242, gem_string("type"), gem_v_val);
    gem_table_set(_t242, gem_string("value"), gem_v_val);
    gem_table_set(_t242, gem_string("line"), gem_v_line);
    gem_table_set(_t242, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t243[] = {gem_v_tokens, _t242};
                (void)(gem_push_fn(NULL, _t243, 2));
            } else {
#line 515 "compiler/main.gem"
    GemVal _t244 = gem_table_new();
    gem_table_set(_t244, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t244, gem_string("value"), gem_v_val);
    gem_table_set(_t244, gem_string("line"), gem_v_line);
    gem_table_set(_t244, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t245[] = {gem_v_tokens, _t244};
                (void)(gem_push_fn(NULL, _t245, 2));
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
    GemVal _t246;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t246 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t246 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t246)) {
#line 527 "compiler/main.gem"
    GemVal _t247 = gem_table_new();
    gem_table_set(_t247, gem_string("type"), gem_string("=="));
    gem_table_set(_t247, gem_string("value"), gem_string("=="));
    gem_table_set(_t247, gem_string("line"), gem_v_line);
    gem_table_set(_t247, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t248[] = {gem_v_tokens, _t247};
            (void)(gem_push_fn(NULL, _t248, 2));
#line 528 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 531 "compiler/main.gem"
    GemVal _t249;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t249 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t249 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t249)) {
#line 532 "compiler/main.gem"
    GemVal _t250 = gem_table_new();
    gem_table_set(_t250, gem_string("type"), gem_string("!="));
    gem_table_set(_t250, gem_string("value"), gem_string("!="));
    gem_table_set(_t250, gem_string("line"), gem_v_line);
    gem_table_set(_t250, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t251[] = {gem_v_tokens, _t250};
            (void)(gem_push_fn(NULL, _t251, 2));
#line 533 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 536 "compiler/main.gem"
    GemVal _t252;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t252 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t252 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t252)) {
#line 537 "compiler/main.gem"
    GemVal _t253 = gem_table_new();
    gem_table_set(_t253, gem_string("type"), gem_string("<="));
    gem_table_set(_t253, gem_string("value"), gem_string("<="));
    gem_table_set(_t253, gem_string("line"), gem_v_line);
    gem_table_set(_t253, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t254[] = {gem_v_tokens, _t253};
            (void)(gem_push_fn(NULL, _t254, 2));
#line 538 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 541 "compiler/main.gem"
    GemVal _t255;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t255 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t255 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t255)) {
#line 542 "compiler/main.gem"
    GemVal _t256 = gem_table_new();
    gem_table_set(_t256, gem_string("type"), gem_string(">="));
    gem_table_set(_t256, gem_string("value"), gem_string(">="));
    gem_table_set(_t256, gem_string("line"), gem_v_line);
    gem_table_set(_t256, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t257[] = {gem_v_tokens, _t256};
            (void)(gem_push_fn(NULL, _t257, 2));
#line 543 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 546 "compiler/main.gem"
    GemVal _t258;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t258 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t258 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t258)) {
#line 547 "compiler/main.gem"
    GemVal _t259 = gem_table_new();
    gem_table_set(_t259, gem_string("type"), gem_string("+="));
    gem_table_set(_t259, gem_string("value"), gem_string("+="));
    gem_table_set(_t259, gem_string("line"), gem_v_line);
    gem_table_set(_t259, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t260[] = {gem_v_tokens, _t259};
            (void)(gem_push_fn(NULL, _t260, 2));
#line 548 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 551 "compiler/main.gem"
    GemVal _t261;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t261 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t261 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t261)) {
#line 552 "compiler/main.gem"
    GemVal _t262 = gem_table_new();
    gem_table_set(_t262, gem_string("type"), gem_string("-="));
    gem_table_set(_t262, gem_string("value"), gem_string("-="));
    gem_table_set(_t262, gem_string("line"), gem_v_line);
    gem_table_set(_t262, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t263[] = {gem_v_tokens, _t262};
            (void)(gem_push_fn(NULL, _t263, 2));
#line 553 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 556 "compiler/main.gem"
    GemVal _t264;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t264 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t264 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t264)) {
#line 557 "compiler/main.gem"
    GemVal _t265 = gem_table_new();
    gem_table_set(_t265, gem_string("type"), gem_string("*="));
    gem_table_set(_t265, gem_string("value"), gem_string("*="));
    gem_table_set(_t265, gem_string("line"), gem_v_line);
    gem_table_set(_t265, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t266[] = {gem_v_tokens, _t265};
            (void)(gem_push_fn(NULL, _t266, 2));
#line 558 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 561 "compiler/main.gem"
    GemVal _t267;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t267 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t267 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t267)) {
#line 562 "compiler/main.gem"
    GemVal _t268 = gem_table_new();
    gem_table_set(_t268, gem_string("type"), gem_string("/="));
    gem_table_set(_t268, gem_string("value"), gem_string("/="));
    gem_table_set(_t268, gem_string("line"), gem_v_line);
    gem_table_set(_t268, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t269[] = {gem_v_tokens, _t268};
            (void)(gem_push_fn(NULL, _t269, 2));
#line 563 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 566 "compiler/main.gem"
    GemVal _t270;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t270 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t270 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t270)) {
#line 567 "compiler/main.gem"
    GemVal _t271 = gem_table_new();
    gem_table_set(_t271, gem_string("type"), gem_string("->"));
    gem_table_set(_t271, gem_string("value"), gem_string("->"));
    gem_table_set(_t271, gem_string("line"), gem_v_line);
    gem_table_set(_t271, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t272[] = {gem_v_tokens, _t271};
            (void)(gem_push_fn(NULL, _t272, 2));
#line 568 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 573 "compiler/main.gem"
    GemVal _t273;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t273 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t273 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t274;
    if (gem_truthy(_t273)) {
        _t274 = _t273;
    } else {
        _t274 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t275;
    if (gem_truthy(_t274)) {
        _t275 = _t274;
    } else {
        _t275 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t276;
    if (gem_truthy(_t275)) {
        _t276 = _t275;
    } else {
        _t276 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t276)) {
#line 574 "compiler/main.gem"
    GemVal _t277 = gem_table_new();
    gem_table_set(_t277, gem_string("type"), gem_v_ch);
    gem_table_set(_t277, gem_string("value"), gem_v_ch);
    gem_table_set(_t277, gem_string("line"), gem_v_line);
    gem_table_set(_t277, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t278[] = {gem_v_tokens, _t277};
            (void)(gem_push_fn(NULL, _t278, 2));
#line 575 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 578 "compiler/main.gem"
    GemVal _t279;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t279 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t279 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t280;
    if (gem_truthy(_t279)) {
        _t280 = _t279;
    } else {
        _t280 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t280)) {
#line 579 "compiler/main.gem"
    GemVal _t281 = gem_table_new();
    gem_table_set(_t281, gem_string("type"), gem_v_ch);
    gem_table_set(_t281, gem_string("value"), gem_v_ch);
    gem_table_set(_t281, gem_string("line"), gem_v_line);
    gem_table_set(_t281, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t282[] = {gem_v_tokens, _t281};
            (void)(gem_push_fn(NULL, _t282, 2));
#line 580 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 584 "compiler/main.gem"
    GemVal _t283;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t283 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t283 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t283)) {
#line 585 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 586 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 587 "compiler/main.gem"
    GemVal _t284 = gem_table_new();
    gem_table_set(_t284, gem_string("type"), gem_string("{"));
    gem_table_set(_t284, gem_string("value"), gem_string("{"));
    gem_table_set(_t284, gem_string("line"), gem_v_line);
    gem_table_set(_t284, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t285[] = {gem_v_tokens, _t284};
            (void)(gem_push_fn(NULL, _t285, 2));
#line 588 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 591 "compiler/main.gem"
    GemVal _t286;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t286 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t286 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t286)) {
#line 592 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 593 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 594 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 595 "compiler/main.gem"
    GemVal _t287 = gem_table_new();
    gem_table_set(_t287, gem_string("type"), gem_string("}"));
    gem_table_set(_t287, gem_string("value"), gem_string("}"));
    gem_table_set(_t287, gem_string("line"), gem_v_line);
    gem_table_set(_t287, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t288[] = {gem_v_tokens, _t287};
                (void)(gem_push_fn(NULL, _t288, 2));
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
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 611 "compiler/main.gem"
                    if (gem_truthy(gem_v_tqr_als)) {
#line 612 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(0);
#line 613 "compiler/main.gem"
                        GemVal gem_v_tqr_skip = gem_int(0);
#line 614 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t289;
                            if (!gem_truthy(gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent))) {
                                                        _t289 = gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t289 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t290;
                            if (!gem_truthy(_t289)) {
                                                        _t290 = _t289;
                            } else {
                                                        _t290 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t290)) break;
#line 615 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 616 "compiler/main.gem"
                            gem_v_tqr_skip = gem_add(gem_v_tqr_skip, gem_int(1));
                        }
#line 619 "compiler/main.gem"
    GemVal _t291;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t291 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t291 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t292;
    if (!gem_truthy(_t291)) {
        _t292 = _t291;
    } else {
        _t292 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t293;
    if (!gem_truthy(_t292)) {
        _t293 = _t292;
    } else {
        _t293 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t293)) {
#line 620 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 621 "compiler/main.gem"
    GemVal _t294[] = {gem_v_val};
    GemVal _t296;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t294, 1), gem_int(0)))) {
        _t296 = gem_gt(gem_len_fn(NULL, _t294, 1), gem_int(0));
    } else {
        GemVal _t295[] = {gem_v_val};
        _t296 = gem_eq(gem_table_get(gem_v_val, gem_sub(gem_len_fn(NULL, _t295, 1), gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t296)) {
#line 622 "compiler/main.gem"
    GemVal _t297[] = {gem_v_val};
    GemVal _t298[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t297, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t298, 3);
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
                        gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 633 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 634 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(1);
                        continue;
                    }
#line 637 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("\\")))) {
#line 638 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 639 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 640 "compiler/main.gem"
    GemVal _t299[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                            (void)(gem_fn_compile_error(NULL, _t299, 7));
                        }
#line 642 "compiler/main.gem"
                        GemVal gem_v_tqre = gem_table_get(gem_v_source, gem_v_pos);
#line 643 "compiler/main.gem"
    GemVal _t300[] = {gem_v_tqre, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t300, 2);
#line 644 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 645 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 647 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqre);
                        }
#line 649 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 652 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("{")))) {
#line 653 "compiler/main.gem"
    GemVal _t301 = gem_table_new();
    gem_table_set(_t301, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t301, gem_string("value"), gem_v_val);
    gem_table_set(_t301, gem_string("line"), gem_v_line);
    gem_table_set(_t301, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t302[] = {gem_v_tokens, _t301};
                        (void)(gem_push_fn(NULL, _t302, 2));
#line 654 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 655 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 656 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 657 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 658 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 659 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 662 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqrc);
#line 663 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 666 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 667 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 668 "compiler/main.gem"
    GemVal _t303[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string interpolation"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t303, 7));
                    }
#line 670 "compiler/main.gem"
    GemVal _t304 = gem_table_new();
    gem_table_set(_t304, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t304, gem_string("value"), gem_v_val);
    gem_table_set(_t304, gem_string("line"), gem_v_line);
    gem_table_set(_t304, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t305[] = {gem_v_tokens, _t304};
                    (void)(gem_push_fn(NULL, _t305, 2));
#line 671 "compiler/main.gem"
    GemVal _t306 = gem_table_new();
    gem_table_set(_t306, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t306, gem_string("value"), gem_string(""));
    gem_table_set(_t306, gem_string("line"), gem_v_line);
    gem_table_set(_t306, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t307[] = {gem_v_tokens, _t306};
                    (void)(gem_push_fn(NULL, _t307, 2));
                }
                continue;
            }
#line 677 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 678 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 679 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t308;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t308 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t308 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t308)) break;
#line 680 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 681 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 682 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 683 "compiler/main.gem"
    GemVal _t309[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t309, 7));
                    }
#line 685 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 686 "compiler/main.gem"
    GemVal _t310[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t310, 2);
#line 687 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 688 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 690 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 692 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 695 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 697 "compiler/main.gem"
    GemVal _t311 = gem_table_new();
    gem_table_set(_t311, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t311, gem_string("value"), gem_v_val);
    gem_table_set(_t311, gem_string("line"), gem_v_line);
    gem_table_set(_t311, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t312[] = {gem_v_tokens, _t311};
                    (void)(gem_push_fn(NULL, _t312, 2));
#line 698 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 699 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 700 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 701 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 702 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 705 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 706 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 708 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 709 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 710 "compiler/main.gem"
    GemVal _t313[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string interpolation"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t313, 7));
                }
#line 712 "compiler/main.gem"
    GemVal _t314 = gem_table_new();
    gem_table_set(_t314, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t314, gem_string("value"), gem_v_val);
    gem_table_set(_t314, gem_string("line"), gem_v_line);
    gem_table_set(_t314, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t315[] = {gem_v_tokens, _t314};
                (void)(gem_push_fn(NULL, _t315, 2));
#line 713 "compiler/main.gem"
    GemVal _t316 = gem_table_new();
    gem_table_set(_t316, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t316, gem_string("value"), gem_string(""));
    gem_table_set(_t316, gem_string("line"), gem_v_line);
    gem_table_set(_t316, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t317[] = {gem_v_tokens, _t316};
                (void)(gem_push_fn(NULL, _t317, 2));
#line 714 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 719 "compiler/main.gem"
    GemVal _t318;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t318 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t318 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t319;
    if (gem_truthy(_t318)) {
        _t319 = _t318;
    } else {
        _t319 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t320;
    if (gem_truthy(_t319)) {
        _t320 = _t319;
    } else {
        _t320 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t321;
    if (gem_truthy(_t320)) {
        _t321 = _t320;
    } else {
        _t321 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t322;
    if (gem_truthy(_t321)) {
        _t322 = _t321;
    } else {
        _t322 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t322)) {
#line 720 "compiler/main.gem"
    GemVal _t323 = gem_table_new();
    gem_table_set(_t323, gem_string("type"), gem_v_ch);
    gem_table_set(_t323, gem_string("value"), gem_v_ch);
    gem_table_set(_t323, gem_string("line"), gem_v_line);
    gem_table_set(_t323, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t324[] = {gem_v_tokens, _t323};
            (void)(gem_push_fn(NULL, _t324, 2));
#line 721 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 724 "compiler/main.gem"
    GemVal _t325;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t325 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t325 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    }
    GemVal _t326;
    if (!gem_truthy(_t325)) {
        _t326 = _t325;
    } else {
        _t326 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("."));
    }
    GemVal _t327;
    if (!gem_truthy(_t326)) {
        _t327 = _t326;
    } else {
        _t327 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    }
    GemVal _t328;
    if (!gem_truthy(_t327)) {
        _t328 = _t327;
    } else {
        _t328 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("."));
    }
        if (gem_truthy(_t328)) {
#line 725 "compiler/main.gem"
    GemVal _t329 = gem_table_new();
    gem_table_set(_t329, gem_string("type"), gem_string("..."));
    gem_table_set(_t329, gem_string("value"), gem_string("..."));
    gem_table_set(_t329, gem_string("line"), gem_v_line);
    gem_table_set(_t329, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t330[] = {gem_v_tokens, _t329};
            (void)(gem_push_fn(NULL, _t330, 2));
#line 726 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
            continue;
        }
#line 729 "compiler/main.gem"
    GemVal _t331;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t331 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t331 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t332;
    if (gem_truthy(_t331)) {
        _t332 = _t331;
    } else {
        _t332 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t333;
    if (gem_truthy(_t332)) {
        _t333 = _t332;
    } else {
        _t333 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t333)) {
#line 730 "compiler/main.gem"
    GemVal _t334 = gem_table_new();
    gem_table_set(_t334, gem_string("type"), gem_v_ch);
    gem_table_set(_t334, gem_string("value"), gem_v_ch);
    gem_table_set(_t334, gem_string("line"), gem_v_line);
    gem_table_set(_t334, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t335[] = {gem_v_tokens, _t334};
            (void)(gem_push_fn(NULL, _t335, 2));
#line 731 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 735 "compiler/main.gem"
    GemVal _t336[] = {gem_v_ch};
    GemVal _t337[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t336, 1)), gem_string("'")), GEM_NIL};
        (void)(gem_fn_compile_error(NULL, _t337, 7));
    }
#line 738 "compiler/main.gem"
    GemVal _t338 = gem_table_new();
    gem_table_set(_t338, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t338, gem_string("value"), gem_string(""));
    gem_table_set(_t338, gem_string("line"), gem_v_line);
    gem_table_set(_t338, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t339[] = {gem_v_tokens, _t338};
    (void)(gem_push_fn(NULL, _t339, 2));
    GemVal _t340 = gem_v_tokens;
    gem_pop_frame();
    return _t340;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 11 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("parse_source", "compiler/main.gem", 11);
#line 12 "compiler/main.gem"
    GemVal _t341[] = {gem_v_source, gem_v_file};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t341, 2);
#line 13 "compiler/main.gem"
    GemVal _t342[] = {gem_v_tokens, gem_v_source, gem_v_file};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t342, 3);
#line 14 "compiler/main.gem"
    GemVal _t343 = gem_v_p;
    GemVal _t344 = gem_table_get(_t343, gem_string("parse"));
    GemVal _t345 = _t344.fn(_t344.env, NULL, 0);
    gem_pop_frame();
    return _t345;
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
    GemVal _t346 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t346;
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
    GemVal _t348 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t348;
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
#line 35 "compiler/main.gem"
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
#line 36 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t350 = gem_v_t;
    gem_pop_frame();
    return _t350;
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
    GemVal _t352 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t353 = gem_eq(gem_table_get(_t352, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t353;
}

struct _closure__anon_5 {
    GemVal *gem_v_file;
    GemVal *gem_v_pos;
    GemVal *gem_v_source;
    GemVal *gem_v_tokens;
};
static GemVal _anon_5(void *_env, GemVal *args, int argc) {
    struct _closure__anon_5 *_cls = (struct _closure__anon_5 *)_env;
    GemVal *gem_v_file = _cls->gem_v_file;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_source = _cls->gem_v_source;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    GemVal gem_v_tp = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_5", "compiler/main.gem", 0);
#line 45 "compiler/main.gem"
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
#line 46 "compiler/main.gem"
    GemVal _t355 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t355, gem_string("type")), gem_v_tp))) {
#line 47 "compiler/main.gem"
    GemVal _t356 = gem_v_t;
    GemVal _t357 = gem_v_t;
    GemVal _t358 = gem_v_t;
    GemVal _t359[] = {gem_table_get(_t358, gem_string("value"))};
    GemVal _t360[] = {gem_v_tp};
    GemVal _t361 = gem_v_t;
    GemVal _t362[] = {gem_table_get(_t361, gem_string("type"))};
    GemVal _t363[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t356, gem_string("line")), gem_table_get(_t357, gem_string("col")), gem_len_fn(NULL, _t359, 1), gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t360, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t362, 1)), gem_string("'")), GEM_NIL};
        (void)(gem_fn_compile_error(NULL, _t363, 7));
    }
#line 49 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t364 = gem_v_t;
    gem_pop_frame();
    return _t364;
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
#line 54 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t366 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t366, gem_string("type")), gem_string("NEWLINE")))) break;
#line 55 "compiler/main.gem"
        *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    }
    gem_pop_frame();
    return GEM_NIL;
}

struct _closure__anon_7 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_file;
    GemVal *gem_v_peek;
    GemVal *gem_v_source;
};
static GemVal _anon_7(void *_env, GemVal *args, int argc) {
    struct _closure__anon_7 *_cls = (struct _closure__anon_7 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_file = _cls->gem_v_file;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_source = _cls->gem_v_source;
    gem_push_frame("_anon_7", "compiler/main.gem", 0);
#line 66 "compiler/main.gem"
    GemVal _t368 = gem_table_new();
    GemVal gem_v_params = _t368;
#line 67 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 68 "compiler/main.gem"
    GemVal gem_v_block_param = GEM_NIL;
#line 70 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t369 = (*gem_v_peek);
        GemVal _t370 = _t369.fn(_t369.env, NULL, 0);
        GemVal _t373;
        if (gem_truthy(gem_eq(gem_table_get(_t370, gem_string("type")), gem_string("NAME")))) {
                _t373 = gem_eq(gem_table_get(_t370, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t371 = (*gem_v_peek);
                GemVal _t372 = _t371.fn(_t371.env, NULL, 0);
                _t373 = gem_eq(gem_table_get(_t372, gem_string("type")), gem_string("..."));
        }
        if (!gem_truthy(_t373)) break;
#line 71 "compiler/main.gem"
    GemVal _t374 = (*gem_v_peek);
    GemVal _t375 = _t374.fn(_t374.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t375, gem_string("type")), gem_string("...")))) {
#line 72 "compiler/main.gem"
    GemVal _t376 = (*gem_v_advance);
            (void)(_t376.fn(_t376.env, NULL, 0));
#line 73 "compiler/main.gem"
    GemVal _t377[] = {gem_string("NAME")};
    GemVal _t378 = (*gem_v_expect);
    GemVal _t379 = _t378.fn(_t378.env, _t377, 1);
            gem_v_rest_param = gem_table_get(_t379, gem_string("value"));
#line 74 "compiler/main.gem"
    GemVal _t380 = (*gem_v_peek);
    GemVal _t381 = _t380.fn(_t380.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t381, gem_string("type")), gem_string(",")))) {
#line 75 "compiler/main.gem"
    GemVal _t382 = (*gem_v_advance);
                (void)(_t382.fn(_t382.env, NULL, 0));
#line 76 "compiler/main.gem"
    GemVal _t383 = (*gem_v_peek);
    GemVal _t384 = _t383.fn(_t383.env, NULL, 0);
                if (gem_truthy(gem_neq(gem_table_get(_t384, gem_string("type")), gem_string("NAME")))) {
#line 77 "compiler/main.gem"
    GemVal _t385 = (*gem_v_peek);
    GemVal _t386 = _t385.fn(_t385.env, NULL, 0);
    GemVal _t387 = (*gem_v_peek);
    GemVal _t388 = _t387.fn(_t387.env, NULL, 0);
    GemVal _t389 = (*gem_v_peek);
    GemVal _t390 = _t389.fn(_t389.env, NULL, 0);
    GemVal _t391[] = {gem_table_get(_t390, gem_string("value"))};
    GemVal _t392[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t386, gem_string("line")), gem_table_get(_t388, gem_string("col")), gem_len_fn(NULL, _t391, 1), gem_string("expected parameter name after rest param"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t392, 7));
                }
#line 79 "compiler/main.gem"
    GemVal _t393 = (*gem_v_advance);
    GemVal _t394 = _t393.fn(_t393.env, NULL, 0);
                gem_v_block_param = gem_table_get(_t394, gem_string("value"));
#line 80 "compiler/main.gem"
    GemVal _t395 = (*gem_v_peek);
    GemVal _t396 = _t395.fn(_t395.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t396, gem_string("type")), gem_string(",")))) {
#line 81 "compiler/main.gem"
    GemVal _t397 = (*gem_v_peek);
    GemVal _t398 = _t397.fn(_t397.env, NULL, 0);
    GemVal _t399 = (*gem_v_peek);
    GemVal _t400 = _t399.fn(_t399.env, NULL, 0);
    GemVal _t401 = (*gem_v_peek);
    GemVal _t402 = _t401.fn(_t401.env, NULL, 0);
    GemVal _t403[] = {gem_table_get(_t402, gem_string("value"))};
    GemVal _t404[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t398, gem_string("line")), gem_table_get(_t400, gem_string("col")), gem_len_fn(NULL, _t403, 1), gem_string("only one parameter is allowed after a rest param"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t404, 7));
                }
            }
            break;
        }
#line 86 "compiler/main.gem"
    GemVal _t405 = (*gem_v_advance);
    GemVal _t406 = _t405.fn(_t405.env, NULL, 0);
    GemVal _t407[] = {gem_v_params, gem_table_get(_t406, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t407, 2));
#line 87 "compiler/main.gem"
    GemVal _t408 = (*gem_v_peek);
    GemVal _t409 = _t408.fn(_t408.env, NULL, 0);
        if (gem_truthy(gem_neq(gem_table_get(_t409, gem_string("type")), gem_string(",")))) {
            break;
        }
#line 90 "compiler/main.gem"
    GemVal _t410 = (*gem_v_advance);
        (void)(_t410.fn(_t410.env, NULL, 0));
    }
    GemVal _t411 = gem_table_new();
    gem_table_set(_t411, gem_string("params"), gem_v_params);
    gem_table_set(_t411, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t411, gem_string("block_param"), gem_v_block_param);
    GemVal _t412 = _t411;
    gem_pop_frame();
    return _t412;
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
#line 109 "compiler/main.gem"
    GemVal _t414 = gem_table_new();
    GemVal gem_v_stmts = _t414;
#line 110 "compiler/main.gem"
    GemVal _t415 = (*gem_v_skip_nl);
    (void)(_t415.fn(_t415.env, NULL, 0));
#line 111 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t416 = (*gem_v_peek);
        GemVal _t417 = _t416.fn(_t416.env, NULL, 0);
        GemVal _t420;
        if (!gem_truthy(gem_neq(gem_table_get(_t417, gem_string("type")), gem_string("end")))) {
                _t420 = gem_neq(gem_table_get(_t417, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t418 = (*gem_v_peek);
                GemVal _t419 = _t418.fn(_t418.env, NULL, 0);
                _t420 = gem_neq(gem_table_get(_t419, gem_string("type")), gem_string("elif"));
        }
        GemVal _t423;
        if (!gem_truthy(_t420)) {
                _t423 = _t420;
        } else {
                GemVal _t421 = (*gem_v_peek);
                GemVal _t422 = _t421.fn(_t421.env, NULL, 0);
                _t423 = gem_neq(gem_table_get(_t422, gem_string("type")), gem_string("else"));
        }
        GemVal _t426;
        if (!gem_truthy(_t423)) {
                _t426 = _t423;
        } else {
                GemVal _t424 = (*gem_v_peek);
                GemVal _t425 = _t424.fn(_t424.env, NULL, 0);
                _t426 = gem_neq(gem_table_get(_t425, gem_string("type")), gem_string("when"));
        }
        GemVal _t429;
        if (!gem_truthy(_t426)) {
                _t429 = _t426;
        } else {
                GemVal _t427 = (*gem_v_peek);
                GemVal _t428 = _t427.fn(_t427.env, NULL, 0);
                _t429 = gem_neq(gem_table_get(_t428, gem_string("type")), gem_string("after"));
        }
        GemVal _t431;
        if (!gem_truthy(_t429)) {
                _t431 = _t429;
        } else {
                GemVal _t430 = (*gem_v_at_end);
                _t431 = gem_not(_t430.fn(_t430.env, NULL, 0));
        }
        if (!gem_truthy(_t431)) break;
#line 112 "compiler/main.gem"
    GemVal _t432 = (*gem_v_parse_stmt);
    GemVal _t433[] = {gem_v_stmts, _t432.fn(_t432.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t433, 2));
#line 113 "compiler/main.gem"
    GemVal _t434 = (*gem_v_skip_nl);
        (void)(_t434.fn(_t434.env, NULL, 0));
    }
    GemVal _t435 = gem_v_stmts;
    gem_pop_frame();
    return _t435;
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
#line 122 "compiler/main.gem"
    GemVal _t437 = gem_table_new();
    GemVal gem_v_stmts = _t437;
#line 123 "compiler/main.gem"
    GemVal _t438 = (*gem_v_skip_nl);
    (void)(_t438.fn(_t438.env, NULL, 0));
#line 124 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t439 = (*gem_v_peek);
        GemVal _t440 = _t439.fn(_t439.env, NULL, 0);
        GemVal _t442;
        if (!gem_truthy(gem_neq(gem_table_get(_t440, gem_string("type")), gem_string("end")))) {
                _t442 = gem_neq(gem_table_get(_t440, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t441 = (*gem_v_at_end);
                _t442 = gem_not(_t441.fn(_t441.env, NULL, 0));
        }
        if (!gem_truthy(_t442)) break;
#line 125 "compiler/main.gem"
    GemVal _t443 = (*gem_v_parse_stmt);
    GemVal _t444[] = {gem_v_stmts, _t443.fn(_t443.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t444, 2));
#line 126 "compiler/main.gem"
    GemVal _t445 = (*gem_v_skip_nl);
        (void)(_t445.fn(_t445.env, NULL, 0));
    }
    GemVal _t446 = gem_v_stmts;
    gem_pop_frame();
    return _t446;
}

struct _closure__anon_10 {
    GemVal *gem_v_advance;
    GemVal *gem_v_at_end;
    GemVal *gem_v_expect;
    GemVal *gem_v_file;
    GemVal *gem_v_fn_depth;
    GemVal *gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body;
    GemVal *gem_v_parse_params;
    GemVal *gem_v_peek;
    GemVal *gem_v_peek_at;
    GemVal *gem_v_skip_nl;
    GemVal *gem_v_source;
};
static GemVal _anon_10(void *_env, GemVal *args, int argc) {
    struct _closure__anon_10 *_cls = (struct _closure__anon_10 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_at_end = _cls->gem_v_at_end;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_file = _cls->gem_v_file;
    GemVal *gem_v_fn_depth = _cls->gem_v_fn_depth;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_parse_fn_body = _cls->gem_v_parse_fn_body;
    GemVal *gem_v_parse_params = _cls->gem_v_parse_params;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_peek_at = _cls->gem_v_peek_at;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    GemVal *gem_v_source = _cls->gem_v_source;
    gem_push_frame("_anon_10", "compiler/main.gem", 0);
#line 135 "compiler/main.gem"
    GemVal _t448 = (*gem_v_peek);
    GemVal gem_v_t = _t448.fn(_t448.env, NULL, 0);
#line 138 "compiler/main.gem"
    GemVal _t449 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t449, gem_string("type")), gem_string("NUMBER")))) {
#line 139 "compiler/main.gem"
    GemVal _t450 = (*gem_v_advance);
        (void)(_t450.fn(_t450.env, NULL, 0));
#line 140 "compiler/main.gem"
    GemVal _t451 = gem_v_t;
    GemVal _t452[] = {gem_table_get(_t451, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t452, 1))) {
#line 141 "compiler/main.gem"
    GemVal _t453 = gem_v_t;
    GemVal _t454[] = {gem_table_get(_t453, gem_string("value"))};
    GemVal _t455[] = {gem_fn_atof(NULL, _t454, 1)};
            GemVal _t456 = gem_fn_make_float(NULL, _t455, 1);
            gem_pop_frame();
            return _t456;
        }
#line 143 "compiler/main.gem"
    GemVal _t457 = gem_v_t;
    GemVal _t458[] = {gem_table_get(_t457, gem_string("value"))};
    GemVal _t459[] = {gem_fn_str_to_int(NULL, _t458, 1)};
        GemVal _t460 = gem_fn_make_int(NULL, _t459, 1);
        gem_pop_frame();
        return _t460;
    }
#line 147 "compiler/main.gem"
    GemVal _t461 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t461, gem_string("type")), gem_string("STRING")))) {
#line 148 "compiler/main.gem"
    GemVal _t462 = (*gem_v_advance);
        (void)(_t462.fn(_t462.env, NULL, 0));
#line 149 "compiler/main.gem"
    GemVal _t463 = gem_v_t;
    GemVal _t464[] = {gem_table_get(_t463, gem_string("value"))};
        GemVal _t465 = gem_fn_make_string(NULL, _t464, 1);
        gem_pop_frame();
        return _t465;
    }
#line 153 "compiler/main.gem"
    GemVal _t466 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t466, gem_string("type")), gem_string("INTERP_START")))) {
#line 154 "compiler/main.gem"
    GemVal _t467 = (*gem_v_advance);
        (void)(_t467.fn(_t467.env, NULL, 0));
#line 155 "compiler/main.gem"
    GemVal _t468 = gem_table_new();
        GemVal gem_v_parts = _t468;
#line 156 "compiler/main.gem"
    GemVal _t469 = (*gem_v_skip_nl);
        (void)(_t469.fn(_t469.env, NULL, 0));
#line 157 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t470 = (*gem_v_peek);
            GemVal _t471 = _t470.fn(_t470.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t471, gem_string("type")), gem_string("INTERP_END")))) break;
#line 158 "compiler/main.gem"
    GemVal _t472 = (*gem_v_peek);
    GemVal _t473 = _t472.fn(_t472.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t473, gem_string("type")), gem_string("STRING")))) {
#line 159 "compiler/main.gem"
    GemVal _t474 = (*gem_v_advance);
    GemVal _t475 = _t474.fn(_t474.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t475, gem_string("value"));
#line 160 "compiler/main.gem"
    GemVal _t476[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t476, 1), gem_int(0)))) {
#line 161 "compiler/main.gem"
    GemVal _t477[] = {gem_v_sval};
    GemVal _t478[] = {gem_v_parts, gem_fn_make_string(NULL, _t477, 1)};
                    (void)(gem_push_fn(NULL, _t478, 2));
                }
            } else {
#line 164 "compiler/main.gem"
    GemVal _t479 = (*gem_v_parse_expr);
    GemVal _t480[] = {gem_v_parts, _t479.fn(_t479.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t480, 2));
            }
#line 166 "compiler/main.gem"
    GemVal _t481 = (*gem_v_skip_nl);
            (void)(_t481.fn(_t481.env, NULL, 0));
        }
#line 168 "compiler/main.gem"
    GemVal _t482[] = {gem_string("INTERP_END")};
    GemVal _t483 = (*gem_v_expect);
        (void)(_t483.fn(_t483.env, _t482, 1));
#line 169 "compiler/main.gem"
    GemVal _t484[] = {gem_v_parts};
        GemVal _t485 = gem_fn_make_interp(NULL, _t484, 1);
        gem_pop_frame();
        return _t485;
    }
#line 173 "compiler/main.gem"
    GemVal _t486 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t486, gem_string("type")), gem_string("true")))) {
#line 174 "compiler/main.gem"
    GemVal _t487 = (*gem_v_advance);
        (void)(_t487.fn(_t487.env, NULL, 0));
#line 175 "compiler/main.gem"
    GemVal _t488[] = {gem_bool(1)};
        GemVal _t489 = gem_fn_make_bool(NULL, _t488, 1);
        gem_pop_frame();
        return _t489;
    }
#line 177 "compiler/main.gem"
    GemVal _t490 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t490, gem_string("type")), gem_string("false")))) {
#line 178 "compiler/main.gem"
    GemVal _t491 = (*gem_v_advance);
        (void)(_t491.fn(_t491.env, NULL, 0));
#line 179 "compiler/main.gem"
    GemVal _t492[] = {gem_bool(0)};
        GemVal _t493 = gem_fn_make_bool(NULL, _t492, 1);
        gem_pop_frame();
        return _t493;
    }
#line 183 "compiler/main.gem"
    GemVal _t494 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t494, gem_string("type")), gem_string("nil")))) {
#line 184 "compiler/main.gem"
    GemVal _t495 = (*gem_v_advance);
        (void)(_t495.fn(_t495.env, NULL, 0));
#line 185 "compiler/main.gem"
        GemVal _t496 = gem_fn_make_nil_node(NULL, NULL, 0);
        gem_pop_frame();
        return _t496;
    }
#line 189 "compiler/main.gem"
    GemVal _t497 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t497, gem_string("type")), gem_string("fn")))) {
#line 190 "compiler/main.gem"
    GemVal _t498 = (*gem_v_advance);
        (void)(_t498.fn(_t498.env, NULL, 0));
#line 191 "compiler/main.gem"
    GemVal _t499[] = {gem_string("(")};
    GemVal _t500 = (*gem_v_expect);
        (void)(_t500.fn(_t500.env, _t499, 1));
#line 192 "compiler/main.gem"
    GemVal _t501 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t501.fn(_t501.env, NULL, 0);
#line 193 "compiler/main.gem"
    GemVal _t502[] = {gem_string(")")};
    GemVal _t503 = (*gem_v_expect);
        (void)(_t503.fn(_t503.env, _t502, 1));
#line 194 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 195 "compiler/main.gem"
    GemVal _t504 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t504.fn(_t504.env, NULL, 0);
#line 196 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 197 "compiler/main.gem"
    GemVal _t505[] = {gem_string("end")};
    GemVal _t506 = (*gem_v_expect);
        (void)(_t506.fn(_t506.env, _t505, 1));
#line 198 "compiler/main.gem"
    GemVal _t507 = gem_v_pr;
    GemVal _t508 = gem_v_pr;
    GemVal _t509 = gem_v_pr;
    GemVal _t510[] = {gem_table_get(_t507, gem_string("params")), gem_table_get(_t508, gem_string("rest_param")), gem_table_get(_t509, gem_string("block_param")), gem_v_body};
        GemVal _t511 = gem_fn_make_anon_fn(NULL, _t510, 4);
        gem_pop_frame();
        return _t511;
    }
#line 203 "compiler/main.gem"
    GemVal _t512 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t512, gem_string("type")), gem_string("{")))) {
#line 204 "compiler/main.gem"
    GemVal _t513 = (*gem_v_advance);
        (void)(_t513.fn(_t513.env, NULL, 0));
#line 205 "compiler/main.gem"
    GemVal _t514 = (*gem_v_skip_nl);
        (void)(_t514.fn(_t514.env, NULL, 0));
#line 206 "compiler/main.gem"
    GemVal _t515 = gem_table_new();
        GemVal gem_v_entries = _t515;
#line 207 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t516 = (*gem_v_peek);
            GemVal _t517 = _t516.fn(_t516.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t517, gem_string("type")), gem_string("}")))) break;
#line 208 "compiler/main.gem"
    GemVal _t518 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t518.fn(_t518.env, NULL, 0);
#line 209 "compiler/main.gem"
    GemVal _t519 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t519, gem_string("type")), gem_string("NAME")))) {
#line 210 "compiler/main.gem"
    GemVal _t520 = (*gem_v_advance);
                (void)(_t520.fn(_t520.env, NULL, 0));
            } else {
#line 211 "compiler/main.gem"
    GemVal _t521 = (*gem_v_at_end);
    GemVal _t525;
    if (!gem_truthy(gem_not(_t521.fn(_t521.env, NULL, 0)))) {
        _t525 = gem_not(_t521.fn(_t521.env, NULL, 0));
    } else {
        GemVal _t522[] = {gem_int(1)};
        GemVal _t523 = (*gem_v_peek_at);
        GemVal _t524 = _t523.fn(_t523.env, _t522, 1);
        _t525 = gem_eq(gem_table_get(_t524, gem_string("type")), gem_string(":"));
    }
                if (gem_truthy(_t525)) {
#line 213 "compiler/main.gem"
    GemVal _t526 = (*gem_v_advance);
                    (void)(_t526.fn(_t526.env, NULL, 0));
                } else {
#line 215 "compiler/main.gem"
    GemVal _t527 = gem_v_key_tok;
    GemVal _t528 = gem_v_key_tok;
    GemVal _t529 = gem_v_key_tok;
    GemVal _t530[] = {gem_table_get(_t529, gem_string("value"))};
    GemVal _t531 = gem_v_key_tok;
    GemVal _t532[] = {gem_table_get(_t531, gem_string("type"))};
    GemVal _t533[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t527, gem_string("line")), gem_table_get(_t528, gem_string("col")), gem_len_fn(NULL, _t530, 1), gem_add(gem_add(gem_string("expected '}' or table key but got '"), gem_to_string_fn(NULL, _t532, 1)), gem_string("'")), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t533, 7));
                }
            }
#line 217 "compiler/main.gem"
    GemVal _t534[] = {gem_string(":")};
    GemVal _t535 = (*gem_v_expect);
            (void)(_t535.fn(_t535.env, _t534, 1));
#line 218 "compiler/main.gem"
    GemVal _t536 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t536.fn(_t536.env, NULL, 0);
#line 219 "compiler/main.gem"
    GemVal _t537 = gem_v_key_tok;
    GemVal _t538[] = {gem_table_get(_t537, gem_string("value")), gem_v_val};
    GemVal _t539[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t538, 2)};
            (void)(gem_push_fn(NULL, _t539, 2));
#line 220 "compiler/main.gem"
    GemVal _t540 = (*gem_v_skip_nl);
            (void)(_t540.fn(_t540.env, NULL, 0));
#line 221 "compiler/main.gem"
    GemVal _t541 = (*gem_v_peek);
    GemVal _t542 = _t541.fn(_t541.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t542, gem_string("type")), gem_string(",")))) {
#line 222 "compiler/main.gem"
    GemVal _t543 = (*gem_v_advance);
                (void)(_t543.fn(_t543.env, NULL, 0));
#line 223 "compiler/main.gem"
    GemVal _t544 = (*gem_v_skip_nl);
                (void)(_t544.fn(_t544.env, NULL, 0));
            }
        }
#line 226 "compiler/main.gem"
    GemVal _t545[] = {gem_string("}")};
    GemVal _t546 = (*gem_v_expect);
        (void)(_t546.fn(_t546.env, _t545, 1));
#line 227 "compiler/main.gem"
    GemVal _t547[] = {gem_v_entries};
        GemVal _t548 = gem_fn_make_table(NULL, _t547, 1);
        gem_pop_frame();
        return _t548;
    }
#line 231 "compiler/main.gem"
    GemVal _t549 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t549, gem_string("type")), gem_string("[")))) {
#line 232 "compiler/main.gem"
    GemVal _t550 = (*gem_v_advance);
        (void)(_t550.fn(_t550.env, NULL, 0));
#line 233 "compiler/main.gem"
    GemVal _t551 = (*gem_v_skip_nl);
        (void)(_t551.fn(_t551.env, NULL, 0));
#line 234 "compiler/main.gem"
    GemVal _t552 = gem_table_new();
        GemVal gem_v_elements = _t552;
#line 235 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t553 = (*gem_v_peek);
            GemVal _t554 = _t553.fn(_t553.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t554, gem_string("type")), gem_string("]")))) break;
#line 236 "compiler/main.gem"
    GemVal _t555 = (*gem_v_parse_expr);
    GemVal _t556[] = {gem_v_elements, _t555.fn(_t555.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t556, 2));
#line 237 "compiler/main.gem"
    GemVal _t557 = (*gem_v_skip_nl);
            (void)(_t557.fn(_t557.env, NULL, 0));
#line 238 "compiler/main.gem"
    GemVal _t558 = (*gem_v_peek);
    GemVal _t559 = _t558.fn(_t558.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t559, gem_string("type")), gem_string(",")))) {
#line 239 "compiler/main.gem"
    GemVal _t560 = (*gem_v_advance);
                (void)(_t560.fn(_t560.env, NULL, 0));
#line 240 "compiler/main.gem"
    GemVal _t561 = (*gem_v_skip_nl);
                (void)(_t561.fn(_t561.env, NULL, 0));
            }
        }
#line 243 "compiler/main.gem"
    GemVal _t562[] = {gem_string("]")};
    GemVal _t563 = (*gem_v_expect);
        (void)(_t563.fn(_t563.env, _t562, 1));
#line 244 "compiler/main.gem"
    GemVal _t564[] = {gem_v_elements};
        GemVal _t565 = gem_fn_make_array(NULL, _t564, 1);
        gem_pop_frame();
        return _t565;
    }
#line 248 "compiler/main.gem"
    GemVal _t566 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t566, gem_string("type")), gem_string("(")))) {
#line 249 "compiler/main.gem"
    GemVal _t567 = (*gem_v_advance);
        (void)(_t567.fn(_t567.env, NULL, 0));
#line 250 "compiler/main.gem"
    GemVal _t568 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t568.fn(_t568.env, NULL, 0);
#line 251 "compiler/main.gem"
    GemVal _t569[] = {gem_string(")")};
    GemVal _t570 = (*gem_v_expect);
        (void)(_t570.fn(_t570.env, _t569, 1));
#line 252 "compiler/main.gem"
        GemVal _t571 = gem_v_e;
        gem_pop_frame();
        return _t571;
    }
#line 256 "compiler/main.gem"
    GemVal _t572 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t572, gem_string("type")), gem_string("NAME")))) {
#line 257 "compiler/main.gem"
    GemVal _t573 = (*gem_v_advance);
        (void)(_t573.fn(_t573.env, NULL, 0));
#line 258 "compiler/main.gem"
    GemVal _t574 = gem_v_t;
    GemVal _t575[] = {gem_table_get(_t574, gem_string("value"))};
        GemVal _t576 = gem_fn_make_var(NULL, _t575, 1);
        gem_pop_frame();
        return _t576;
    }
#line 261 "compiler/main.gem"
    GemVal _t577 = gem_v_t;
    GemVal _t578 = gem_v_t;
    GemVal _t579 = gem_v_t;
    GemVal _t580[] = {gem_table_get(_t579, gem_string("value"))};
    GemVal _t581 = gem_v_t;
    GemVal _t582[] = {gem_table_get(_t581, gem_string("type"))};
    GemVal _t583[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t577, gem_string("line")), gem_table_get(_t578, gem_string("col")), gem_len_fn(NULL, _t580, 1), gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t582, 1)), gem_string("' (expected expression)")), GEM_NIL};
    GemVal _t584 = gem_fn_compile_error(NULL, _t583, 7);
    gem_pop_frame();
    return _t584;
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
#line 266 "compiler/main.gem"
    GemVal _t586 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t586.fn(_t586.env, NULL, 0);
#line 268 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_bool(1))) break;
#line 270 "compiler/main.gem"
    GemVal _t587 = (*gem_v_peek);
    GemVal _t588 = _t587.fn(_t587.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t588, gem_string("type")), gem_string("(")))) {
#line 271 "compiler/main.gem"
    GemVal _t589 = (*gem_v_peek);
    GemVal _t590 = _t589.fn(_t589.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t590, gem_string("line"));
#line 272 "compiler/main.gem"
    GemVal _t591 = (*gem_v_advance);
            (void)(_t591.fn(_t591.env, NULL, 0));
#line 273 "compiler/main.gem"
    GemVal _t592 = (*gem_v_skip_nl);
            (void)(_t592.fn(_t592.env, NULL, 0));
#line 274 "compiler/main.gem"
    GemVal _t593 = gem_table_new();
            GemVal gem_v_args = _t593;
#line 275 "compiler/main.gem"
    GemVal _t594 = (*gem_v_peek);
    GemVal _t595 = _t594.fn(_t594.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t595, gem_string("type")), gem_string(")")))) {
#line 276 "compiler/main.gem"
    GemVal _t596 = (*gem_v_parse_expr);
    GemVal _t597[] = {gem_v_args, _t596.fn(_t596.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t597, 2));
#line 277 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t598 = (*gem_v_peek);
                    GemVal _t599 = _t598.fn(_t598.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t599, gem_string("type")), gem_string(",")))) break;
#line 278 "compiler/main.gem"
    GemVal _t600 = (*gem_v_advance);
                    (void)(_t600.fn(_t600.env, NULL, 0));
#line 279 "compiler/main.gem"
    GemVal _t601 = (*gem_v_skip_nl);
                    (void)(_t601.fn(_t601.env, NULL, 0));
#line 280 "compiler/main.gem"
    GemVal _t602 = (*gem_v_parse_expr);
    GemVal _t603[] = {gem_v_args, _t602.fn(_t602.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t603, 2));
                }
            }
#line 283 "compiler/main.gem"
    GemVal _t604 = (*gem_v_skip_nl);
            (void)(_t604.fn(_t604.env, NULL, 0));
#line 284 "compiler/main.gem"
    GemVal _t605[] = {gem_string(")")};
    GemVal _t606 = (*gem_v_expect);
            (void)(_t606.fn(_t606.env, _t605, 1));
#line 287 "compiler/main.gem"
    GemVal _t607 = (*gem_v_peek);
    GemVal _t608 = _t607.fn(_t607.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t608, gem_string("type")), gem_string("do")))) {
#line 288 "compiler/main.gem"
    GemVal _t609 = (*gem_v_advance);
                (void)(_t609.fn(_t609.env, NULL, 0));
#line 289 "compiler/main.gem"
    GemVal _t610 = gem_table_new();
                GemVal gem_v_bparams = _t610;
#line 290 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 291 "compiler/main.gem"
                GemVal gem_v_bblock = GEM_NIL;
#line 292 "compiler/main.gem"
    GemVal _t611 = (*gem_v_peek);
    GemVal _t612 = _t611.fn(_t611.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t612, gem_string("type")), gem_string("|")))) {
#line 293 "compiler/main.gem"
    GemVal _t613 = (*gem_v_advance);
                    (void)(_t613.fn(_t613.env, NULL, 0));
#line 294 "compiler/main.gem"
    GemVal _t614 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t614.fn(_t614.env, NULL, 0);
#line 295 "compiler/main.gem"
    GemVal _t615 = gem_v_bpr;
                    gem_v_bparams = gem_table_get(_t615, gem_string("params"));
#line 296 "compiler/main.gem"
    GemVal _t616 = gem_v_bpr;
                    gem_v_brest = gem_table_get(_t616, gem_string("rest_param"));
#line 297 "compiler/main.gem"
    GemVal _t617 = gem_v_bpr;
                    gem_v_bblock = gem_table_get(_t617, gem_string("block_param"));
#line 298 "compiler/main.gem"
    GemVal _t618[] = {gem_string("|")};
    GemVal _t619 = (*gem_v_expect);
                    (void)(_t619.fn(_t619.env, _t618, 1));
                }
#line 300 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 301 "compiler/main.gem"
    GemVal _t620 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t620.fn(_t620.env, NULL, 0);
#line 302 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 303 "compiler/main.gem"
    GemVal _t621[] = {gem_string("end")};
    GemVal _t622 = (*gem_v_expect);
                (void)(_t622.fn(_t622.env, _t621, 1));
#line 304 "compiler/main.gem"
    GemVal _t623[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, gem_v_body};
    GemVal _t624[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t623, 4)};
                (void)(gem_push_fn(NULL, _t624, 2));
            } else {
#line 308 "compiler/main.gem"
    GemVal _t625 = (*gem_v_peek);
    GemVal _t626 = _t625.fn(_t625.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t626, gem_string("type")), gem_string("{")))) {
#line 309 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 310 "compiler/main.gem"
    GemVal _t627[] = {gem_int(1)};
    GemVal _t628 = (*gem_v_peek_at);
    GemVal _t629 = _t628.fn(_t628.env, _t627, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t629, gem_string("type")), gem_string("|")))) {
#line 311 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 312 "compiler/main.gem"
    GemVal _t630[] = {gem_int(1)};
    GemVal _t631 = (*gem_v_peek_at);
    GemVal _t632 = _t631.fn(_t631.env, _t630, 1);
    GemVal _t640;
    if (gem_truthy(gem_eq(gem_table_get(_t632, gem_string("type")), gem_string("}")))) {
        _t640 = gem_eq(gem_table_get(_t632, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t633[] = {gem_int(1)};
        GemVal _t634 = (*gem_v_peek_at);
        GemVal _t635 = _t634.fn(_t634.env, _t633, 1);
        GemVal _t639;
        if (!gem_truthy(gem_eq(gem_table_get(_t635, gem_string("type")), gem_string("NAME")))) {
                _t639 = gem_eq(gem_table_get(_t635, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t636[] = {gem_int(2)};
                GemVal _t637 = (*gem_v_peek_at);
                GemVal _t638 = _t637.fn(_t637.env, _t636, 1);
                _t639 = gem_eq(gem_table_get(_t638, gem_string("type")), gem_string(":"));
        }
        _t640 = _t639;
    }
                        if (gem_truthy(_t640)) {
#line 313 "compiler/main.gem"
                            gem_v_is_block = gem_bool(0);
                        } else {
#line 315 "compiler/main.gem"
                            gem_v_is_block = gem_bool(1);
                        }
                    }
#line 317 "compiler/main.gem"
                    if (gem_truthy(gem_v_is_block)) {
#line 318 "compiler/main.gem"
    GemVal _t641 = (*gem_v_advance);
                        (void)(_t641.fn(_t641.env, NULL, 0));
#line 319 "compiler/main.gem"
    GemVal _t642 = gem_table_new();
                        GemVal gem_v_bparams = _t642;
#line 320 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 321 "compiler/main.gem"
                        GemVal gem_v_bblock = GEM_NIL;
#line 322 "compiler/main.gem"
    GemVal _t643 = (*gem_v_peek);
    GemVal _t644 = _t643.fn(_t643.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t644, gem_string("type")), gem_string("|")))) {
#line 323 "compiler/main.gem"
    GemVal _t645 = (*gem_v_advance);
                            (void)(_t645.fn(_t645.env, NULL, 0));
#line 324 "compiler/main.gem"
    GemVal _t646 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t646.fn(_t646.env, NULL, 0);
#line 325 "compiler/main.gem"
    GemVal _t647 = gem_v_bpr;
                            gem_v_bparams = gem_table_get(_t647, gem_string("params"));
#line 326 "compiler/main.gem"
    GemVal _t648 = gem_v_bpr;
                            gem_v_brest = gem_table_get(_t648, gem_string("rest_param"));
#line 327 "compiler/main.gem"
    GemVal _t649 = gem_v_bpr;
                            gem_v_bblock = gem_table_get(_t649, gem_string("block_param"));
#line 328 "compiler/main.gem"
    GemVal _t650[] = {gem_string("|")};
    GemVal _t651 = (*gem_v_expect);
                            (void)(_t651.fn(_t651.env, _t650, 1));
                        }
#line 330 "compiler/main.gem"
    GemVal _t652 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t652.fn(_t652.env, NULL, 0);
#line 331 "compiler/main.gem"
    GemVal _t653[] = {gem_string("}")};
    GemVal _t654 = (*gem_v_expect);
                        (void)(_t654.fn(_t654.env, _t653, 1));
#line 332 "compiler/main.gem"
    GemVal _t655 = gem_table_new();
    gem_table_set(_t655, gem_int(0), gem_v_bexpr);
    GemVal _t656[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, _t655};
    GemVal _t657[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t656, 4)};
                        (void)(gem_push_fn(NULL, _t657, 2));
                    }
                }
            }
#line 337 "compiler/main.gem"
    GemVal _t658[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t658, 3);
            continue;
        }
#line 342 "compiler/main.gem"
    GemVal _t659 = (*gem_v_peek);
    GemVal _t660 = _t659.fn(_t659.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t660, gem_string("type")), gem_string(".")))) {
#line 343 "compiler/main.gem"
    GemVal _t661 = (*gem_v_advance);
            (void)(_t661.fn(_t661.env, NULL, 0));
#line 344 "compiler/main.gem"
    GemVal _t662 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t662.fn(_t662.env, NULL, 0);
#line 345 "compiler/main.gem"
    GemVal _t663 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t663, gem_string("type")), gem_string("NAME")))) {
#line 346 "compiler/main.gem"
    GemVal _t664 = (*gem_v_advance);
                (void)(_t664.fn(_t664.env, NULL, 0));
            } else {
#line 349 "compiler/main.gem"
    GemVal _t665 = (*gem_v_advance);
                (void)(_t665.fn(_t665.env, NULL, 0));
            }
#line 351 "compiler/main.gem"
    GemVal _t666 = gem_v_field_tok;
    GemVal _t667[] = {gem_v_node, gem_table_get(_t666, gem_string("value"))};
            gem_v_node = gem_fn_make_dot(NULL, _t667, 2);
            continue;
        }
#line 356 "compiler/main.gem"
    GemVal _t668 = (*gem_v_peek);
    GemVal _t669 = _t668.fn(_t668.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t669, gem_string("type")), gem_string("[")))) {
#line 357 "compiler/main.gem"
    GemVal _t670 = (*gem_v_advance);
            (void)(_t670.fn(_t670.env, NULL, 0));
#line 358 "compiler/main.gem"
    GemVal _t671 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t671.fn(_t671.env, NULL, 0);
#line 359 "compiler/main.gem"
    GemVal _t672[] = {gem_string("]")};
    GemVal _t673 = (*gem_v_expect);
            (void)(_t673.fn(_t673.env, _t672, 1));
#line 360 "compiler/main.gem"
    GemVal _t674[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t674, 2);
            continue;
        }
        break;
    }
    GemVal _t675 = gem_v_node;
    gem_pop_frame();
    return _t675;
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
#line 372 "compiler/main.gem"
    GemVal _t677 = (*gem_v_peek);
    GemVal _t678 = _t677.fn(_t677.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t678, gem_string("type")), gem_string("-")))) {
#line 373 "compiler/main.gem"
    GemVal _t679 = (*gem_v_advance);
        (void)(_t679.fn(_t679.env, NULL, 0));
#line 374 "compiler/main.gem"
    GemVal _t680 = (*gem_v_parse_unary);
    GemVal _t681[] = {gem_string("-"), _t680.fn(_t680.env, NULL, 0)};
        GemVal _t682 = gem_fn_make_unop(NULL, _t681, 2);
        gem_pop_frame();
        return _t682;
    }
#line 376 "compiler/main.gem"
    GemVal _t683 = (*gem_v_parse_call);
    GemVal _t684 = _t683.fn(_t683.env, NULL, 0);
    gem_pop_frame();
    return _t684;
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
#line 381 "compiler/main.gem"
    GemVal _t686 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t686.fn(_t686.env, NULL, 0);
#line 382 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t687 = (*gem_v_peek);
        GemVal _t688 = _t687.fn(_t687.env, NULL, 0);
        GemVal _t691;
        if (gem_truthy(gem_eq(gem_table_get(_t688, gem_string("type")), gem_string("*")))) {
                _t691 = gem_eq(gem_table_get(_t688, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t689 = (*gem_v_peek);
                GemVal _t690 = _t689.fn(_t689.env, NULL, 0);
                _t691 = gem_eq(gem_table_get(_t690, gem_string("type")), gem_string("/"));
        }
        GemVal _t694;
        if (gem_truthy(_t691)) {
                _t694 = _t691;
        } else {
                GemVal _t692 = (*gem_v_peek);
                GemVal _t693 = _t692.fn(_t692.env, NULL, 0);
                _t694 = gem_eq(gem_table_get(_t693, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t694)) break;
#line 383 "compiler/main.gem"
    GemVal _t695 = (*gem_v_advance);
    GemVal _t696 = _t695.fn(_t695.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t696, gem_string("type"));
#line 384 "compiler/main.gem"
    GemVal _t697 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t697.fn(_t697.env, NULL, 0);
#line 385 "compiler/main.gem"
    GemVal _t698[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t698, 3);
    }
    GemVal _t699 = gem_v_left;
    gem_pop_frame();
    return _t699;
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
#line 392 "compiler/main.gem"
    GemVal _t701 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t701.fn(_t701.env, NULL, 0);
#line 393 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t702 = (*gem_v_peek);
        GemVal _t703 = _t702.fn(_t702.env, NULL, 0);
        GemVal _t706;
        if (gem_truthy(gem_eq(gem_table_get(_t703, gem_string("type")), gem_string("+")))) {
                _t706 = gem_eq(gem_table_get(_t703, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t704 = (*gem_v_peek);
                GemVal _t705 = _t704.fn(_t704.env, NULL, 0);
                _t706 = gem_eq(gem_table_get(_t705, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t706)) break;
#line 394 "compiler/main.gem"
    GemVal _t707 = (*gem_v_advance);
    GemVal _t708 = _t707.fn(_t707.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t708, gem_string("type"));
#line 395 "compiler/main.gem"
    GemVal _t709 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t709.fn(_t709.env, NULL, 0);
#line 396 "compiler/main.gem"
    GemVal _t710[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t710, 3);
    }
    GemVal _t711 = gem_v_left;
    gem_pop_frame();
    return _t711;
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
#line 403 "compiler/main.gem"
    GemVal _t713 = (*gem_v_parse_add);
    GemVal gem_v_left = _t713.fn(_t713.env, NULL, 0);
#line 404 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t714 = (*gem_v_peek);
        GemVal _t715 = _t714.fn(_t714.env, NULL, 0);
        GemVal _t718;
        if (gem_truthy(gem_eq(gem_table_get(_t715, gem_string("type")), gem_string("==")))) {
                _t718 = gem_eq(gem_table_get(_t715, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t716 = (*gem_v_peek);
                GemVal _t717 = _t716.fn(_t716.env, NULL, 0);
                _t718 = gem_eq(gem_table_get(_t717, gem_string("type")), gem_string("!="));
        }
        GemVal _t721;
        if (gem_truthy(_t718)) {
                _t721 = _t718;
        } else {
                GemVal _t719 = (*gem_v_peek);
                GemVal _t720 = _t719.fn(_t719.env, NULL, 0);
                _t721 = gem_eq(gem_table_get(_t720, gem_string("type")), gem_string("<"));
        }
        GemVal _t724;
        if (gem_truthy(_t721)) {
                _t724 = _t721;
        } else {
                GemVal _t722 = (*gem_v_peek);
                GemVal _t723 = _t722.fn(_t722.env, NULL, 0);
                _t724 = gem_eq(gem_table_get(_t723, gem_string("type")), gem_string(">"));
        }
        GemVal _t727;
        if (gem_truthy(_t724)) {
                _t727 = _t724;
        } else {
                GemVal _t725 = (*gem_v_peek);
                GemVal _t726 = _t725.fn(_t725.env, NULL, 0);
                _t727 = gem_eq(gem_table_get(_t726, gem_string("type")), gem_string("<="));
        }
        GemVal _t730;
        if (gem_truthy(_t727)) {
                _t730 = _t727;
        } else {
                GemVal _t728 = (*gem_v_peek);
                GemVal _t729 = _t728.fn(_t728.env, NULL, 0);
                _t730 = gem_eq(gem_table_get(_t729, gem_string("type")), gem_string(">="));
        }
        GemVal _t733;
        if (gem_truthy(_t730)) {
                _t733 = _t730;
        } else {
                GemVal _t731 = (*gem_v_peek);
                GemVal _t732 = _t731.fn(_t731.env, NULL, 0);
                _t733 = gem_eq(gem_table_get(_t732, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t733)) break;
#line 405 "compiler/main.gem"
    GemVal _t734 = (*gem_v_advance);
    GemVal _t735 = _t734.fn(_t734.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t735, gem_string("type"));
#line 406 "compiler/main.gem"
    GemVal _t736 = (*gem_v_parse_add);
        GemVal gem_v_right = _t736.fn(_t736.env, NULL, 0);
#line 407 "compiler/main.gem"
    GemVal _t737[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t737, 3);
    }
    GemVal _t738 = gem_v_left;
    gem_pop_frame();
    return _t738;
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
#line 414 "compiler/main.gem"
    GemVal _t740 = (*gem_v_peek);
    GemVal _t741 = _t740.fn(_t740.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t741, gem_string("type")), gem_string("not")))) {
#line 415 "compiler/main.gem"
    GemVal _t742 = (*gem_v_advance);
        (void)(_t742.fn(_t742.env, NULL, 0));
#line 416 "compiler/main.gem"
    GemVal _t743 = (*gem_v_parse_not);
    GemVal _t744[] = {gem_string("not"), _t743.fn(_t743.env, NULL, 0)};
        GemVal _t745 = gem_fn_make_unop(NULL, _t744, 2);
        gem_pop_frame();
        return _t745;
    }
#line 418 "compiler/main.gem"
    GemVal _t746 = (*gem_v_parse_compare);
    GemVal _t747 = _t746.fn(_t746.env, NULL, 0);
    gem_pop_frame();
    return _t747;
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
#line 423 "compiler/main.gem"
    GemVal _t749 = (*gem_v_parse_not);
    GemVal gem_v_left = _t749.fn(_t749.env, NULL, 0);
#line 424 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t750 = (*gem_v_peek);
        GemVal _t751 = _t750.fn(_t750.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t751, gem_string("type")), gem_string("and")))) break;
#line 425 "compiler/main.gem"
    GemVal _t752 = (*gem_v_advance);
        (void)(_t752.fn(_t752.env, NULL, 0));
#line 426 "compiler/main.gem"
    GemVal _t753 = (*gem_v_parse_not);
        GemVal gem_v_right = _t753.fn(_t753.env, NULL, 0);
#line 427 "compiler/main.gem"
    GemVal _t754[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t754, 3);
    }
    GemVal _t755 = gem_v_left;
    gem_pop_frame();
    return _t755;
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
#line 434 "compiler/main.gem"
    GemVal _t757 = (*gem_v_parse_and);
    GemVal gem_v_left = _t757.fn(_t757.env, NULL, 0);
#line 435 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t758 = (*gem_v_peek);
        GemVal _t759 = _t758.fn(_t758.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t759, gem_string("type")), gem_string("or")))) break;
#line 436 "compiler/main.gem"
    GemVal _t760 = (*gem_v_advance);
        (void)(_t760.fn(_t760.env, NULL, 0));
#line 437 "compiler/main.gem"
    GemVal _t761 = (*gem_v_parse_and);
        GemVal gem_v_right = _t761.fn(_t761.env, NULL, 0);
#line 438 "compiler/main.gem"
    GemVal _t762[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t762, 3);
    }
    GemVal _t763 = gem_v_left;
    gem_pop_frame();
    return _t763;
}

struct _closure__anon_19 {
    GemVal *gem_v_advance;
    GemVal *gem_v_file;
    GemVal *gem_v_parse_expr;
    GemVal *gem_v_parse_or;
    GemVal *gem_v_peek;
    GemVal *gem_v_source;
};
static GemVal _anon_19(void *_env, GemVal *args, int argc) {
    struct _closure__anon_19 *_cls = (struct _closure__anon_19 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_file = _cls->gem_v_file;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_parse_or = _cls->gem_v_parse_or;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_source = _cls->gem_v_source;
    gem_push_frame("_anon_19", "compiler/main.gem", 0);
#line 445 "compiler/main.gem"
    GemVal _t765 = (*gem_v_peek);
    GemVal _t766 = _t765.fn(_t765.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t766, gem_string("line"));
#line 446 "compiler/main.gem"
    GemVal _t767 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t767.fn(_t767.env, NULL, 0);
#line 449 "compiler/main.gem"
    GemVal _t768 = (*gem_v_peek);
    GemVal _t769 = _t768.fn(_t768.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t769, gem_string("type")), gem_string("=")))) {
#line 450 "compiler/main.gem"
    GemVal _t770 = (*gem_v_advance);
        (void)(_t770.fn(_t770.env, NULL, 0));
#line 451 "compiler/main.gem"
    GemVal _t771 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t771.fn(_t771.env, NULL, 0);
#line 452 "compiler/main.gem"
    GemVal _t772 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t772, gem_string("tag")), gem_string("var")))) {
#line 453 "compiler/main.gem"
    GemVal _t773 = gem_v_lhs;
    GemVal _t774[] = {gem_table_get(_t773, gem_string("name")), gem_v_value, gem_v_start_line};
            GemVal _t775 = gem_fn_make_assign(NULL, _t774, 3);
            gem_pop_frame();
            return _t775;
        }
#line 455 "compiler/main.gem"
    GemVal _t776 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t776, gem_string("tag")), gem_string("dot")))) {
#line 456 "compiler/main.gem"
    GemVal _t777 = gem_v_lhs;
    GemVal _t778 = gem_v_lhs;
    GemVal _t779[] = {gem_table_get(_t777, gem_string("object")), gem_table_get(_t778, gem_string("field")), gem_v_value, gem_v_start_line};
            GemVal _t780 = gem_fn_make_dot_assign(NULL, _t779, 4);
            gem_pop_frame();
            return _t780;
        }
#line 458 "compiler/main.gem"
    GemVal _t781 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t781, gem_string("tag")), gem_string("index")))) {
#line 459 "compiler/main.gem"
    GemVal _t782 = gem_v_lhs;
    GemVal _t783 = gem_v_lhs;
    GemVal _t784[] = {gem_table_get(_t782, gem_string("object")), gem_table_get(_t783, gem_string("key")), gem_v_value, gem_v_start_line};
            GemVal _t785 = gem_fn_make_index_assign(NULL, _t784, 4);
            gem_pop_frame();
            return _t785;
        }
#line 461 "compiler/main.gem"
    GemVal _t786[] = {(*gem_v_source), (*gem_v_file), gem_v_start_line, gem_int(1), gem_int(1), gem_string("invalid assignment target (expected variable, field, or index)"), GEM_NIL};
        (void)(gem_fn_compile_error(NULL, _t786, 7));
    }
#line 465 "compiler/main.gem"
    GemVal _t787 = (*gem_v_peek);
    GemVal _t788 = _t787.fn(_t787.env, NULL, 0);
    GemVal _t791;
    if (gem_truthy(gem_eq(gem_table_get(_t788, gem_string("type")), gem_string("+=")))) {
        _t791 = gem_eq(gem_table_get(_t788, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t789 = (*gem_v_peek);
        GemVal _t790 = _t789.fn(_t789.env, NULL, 0);
        _t791 = gem_eq(gem_table_get(_t790, gem_string("type")), gem_string("-="));
    }
    GemVal _t794;
    if (gem_truthy(_t791)) {
        _t794 = _t791;
    } else {
        GemVal _t792 = (*gem_v_peek);
        GemVal _t793 = _t792.fn(_t792.env, NULL, 0);
        _t794 = gem_eq(gem_table_get(_t793, gem_string("type")), gem_string("*="));
    }
    GemVal _t797;
    if (gem_truthy(_t794)) {
        _t797 = _t794;
    } else {
        GemVal _t795 = (*gem_v_peek);
        GemVal _t796 = _t795.fn(_t795.env, NULL, 0);
        _t797 = gem_eq(gem_table_get(_t796, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t797)) {
#line 466 "compiler/main.gem"
    GemVal _t798 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t798.fn(_t798.env, NULL, 0);
#line 467 "compiler/main.gem"
    GemVal _t799 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t799, gem_string("value")), gem_int(0));
#line 468 "compiler/main.gem"
    GemVal _t800 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t800.fn(_t800.env, NULL, 0);
#line 469 "compiler/main.gem"
    GemVal _t801 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t801, gem_string("tag")), gem_string("var")))) {
#line 470 "compiler/main.gem"
    GemVal _t802 = gem_v_op_tok;
    GemVal _t803 = gem_v_op_tok;
    GemVal _t804 = gem_v_op_tok;
    GemVal _t805[] = {gem_table_get(_t804, gem_string("value"))};
    GemVal _t806[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t802, gem_string("line")), gem_table_get(_t803, gem_string("col")), gem_len_fn(NULL, _t805, 1), gem_string("compound assignment requires variable target"), GEM_NIL};
            (void)(gem_fn_compile_error(NULL, _t806, 7));
        }
#line 472 "compiler/main.gem"
    GemVal _t807 = gem_v_lhs;
    GemVal _t808 = gem_v_lhs;
    GemVal _t809[] = {gem_table_get(_t808, gem_string("name"))};
    GemVal _t810[] = {gem_v_base_op, gem_fn_make_var(NULL, _t809, 1), gem_v_value};
    GemVal _t811[] = {gem_table_get(_t807, gem_string("name")), gem_fn_make_binop(NULL, _t810, 3), gem_v_start_line};
        GemVal _t812 = gem_fn_make_assign(NULL, _t811, 3);
        gem_pop_frame();
        return _t812;
    }
    GemVal _t813 = gem_v_lhs;
    gem_pop_frame();
    return _t813;
}

struct _closure__anon_21 {
};
static GemVal _anon_21(void *_env, GemVal *args, int argc) {
    GemVal gem_v_conditions = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_21", "compiler/main.gem", 0);
#line 485 "compiler/main.gem"
    GemVal _t815[] = {gem_v_conditions};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t815, 1), gem_int(0)))) {
#line 486 "compiler/main.gem"
    GemVal _t816[] = {gem_bool(1)};
        GemVal _t817 = gem_fn_make_bool(NULL, _t816, 1);
        gem_pop_frame();
        return _t817;
    }
#line 488 "compiler/main.gem"
    GemVal gem_v_result = gem_table_get(gem_v_conditions, gem_int(0));
#line 489 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(1);
#line 489 "compiler/main.gem"
    GemVal _t818[] = {gem_v_conditions};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t818, 1);
#line 489 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 489 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_1;
#line 489 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 490 "compiler/main.gem"
    GemVal _t819[] = {gem_string("and"), gem_v_result, gem_table_get(gem_v_conditions, gem_v_i)};
        gem_v_result = gem_fn_make_binop(NULL, _t819, 3);
    }

    GemVal _t820 = gem_v_result;
    gem_pop_frame();
    return _t820;
}

struct _closure__anon_20 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_file;
    GemVal *gem_v_parse_pattern;
    GemVal *gem_v_peek;
    GemVal *gem_v_peek_at;
    GemVal *gem_v_skip_nl;
    GemVal *gem_v_source;
};
static GemVal _anon_20(void *_env, GemVal *args, int argc) {
    struct _closure__anon_20 *_cls = (struct _closure__anon_20 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_file = _cls->gem_v_file;
    GemVal *gem_v_parse_pattern = _cls->gem_v_parse_pattern;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_peek_at = _cls->gem_v_peek_at;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    GemVal *gem_v_source = _cls->gem_v_source;
    GemVal gem_v_target_expr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_20", "compiler/main.gem", 0);
#line 484 "compiler/main.gem"
    GemVal gem_v_and_chain = gem_make_fn(_anon_21, NULL);
#line 494 "compiler/main.gem"
    GemVal _t821 = (*gem_v_peek);
    GemVal gem_v_t = _t821.fn(_t821.env, NULL, 0);
#line 497 "compiler/main.gem"
    GemVal _t822 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t822, gem_string("type")), gem_string("{")))) {
#line 498 "compiler/main.gem"
    GemVal _t823 = (*gem_v_advance);
        (void)(_t823.fn(_t823.env, NULL, 0));
#line 499 "compiler/main.gem"
    GemVal _t824 = (*gem_v_skip_nl);
        (void)(_t824.fn(_t824.env, NULL, 0));
#line 500 "compiler/main.gem"
    GemVal _t825 = gem_table_new();
    GemVal _t826[] = {gem_string("type")};
    GemVal _t827 = gem_table_new();
    gem_table_set(_t827, gem_int(0), gem_v_target_expr);
    GemVal _t828[] = {gem_fn_make_var(NULL, _t826, 1), _t827, gem_int(0)};
    GemVal _t829[] = {gem_string("table")};
    GemVal _t830[] = {gem_string("=="), gem_fn_make_call(NULL, _t828, 3), gem_fn_make_string(NULL, _t829, 1)};
    gem_table_set(_t825, gem_int(0), gem_fn_make_binop(NULL, _t830, 3));
        GemVal gem_v_conditions = _t825;
#line 501 "compiler/main.gem"
    GemVal _t831 = gem_table_new();
        GemVal gem_v_bindings = _t831;
#line 502 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t832 = (*gem_v_peek);
            GemVal _t833 = _t832.fn(_t832.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t833, gem_string("type")), gem_string("}")))) break;
#line 503 "compiler/main.gem"
    GemVal _t834 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t834.fn(_t834.env, NULL, 0);
#line 504 "compiler/main.gem"
    GemVal _t835 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t835, gem_string("type")), gem_string("NAME")))) {
#line 505 "compiler/main.gem"
    GemVal _t836 = (*gem_v_advance);
                (void)(_t836.fn(_t836.env, NULL, 0));
            } else {
#line 506 "compiler/main.gem"
    GemVal _t837[] = {gem_int(1)};
    GemVal _t838 = (*gem_v_peek_at);
    GemVal _t839 = _t838.fn(_t838.env, _t837, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t839, gem_string("type")), gem_string(":")))) {
#line 508 "compiler/main.gem"
    GemVal _t840 = (*gem_v_advance);
                    (void)(_t840.fn(_t840.env, NULL, 0));
                } else {
#line 510 "compiler/main.gem"
    GemVal _t841 = gem_v_key_tok;
    GemVal _t842 = gem_v_key_tok;
    GemVal _t843 = gem_v_key_tok;
    GemVal _t844[] = {gem_table_get(_t843, gem_string("value"))};
    GemVal _t845[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t841, gem_string("line")), gem_table_get(_t842, gem_string("col")), gem_len_fn(NULL, _t844, 1), gem_string("expected key name in table pattern"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t845, 7));
                }
            }
#line 512 "compiler/main.gem"
    GemVal _t846 = gem_v_key_tok;
            GemVal gem_v_key_name = gem_table_get(_t846, gem_string("value"));
#line 513 "compiler/main.gem"
    GemVal _t847[] = {gem_v_key_name};
            GemVal gem_v_key_str = gem_fn_make_string(NULL, _t847, 1);
#line 514 "compiler/main.gem"
    GemVal _t848[] = {gem_string("has_key")};
    GemVal _t849 = gem_table_new();
    gem_table_set(_t849, gem_int(0), gem_v_target_expr);
    gem_table_set(_t849, gem_int(1), gem_v_key_str);
    GemVal _t850[] = {gem_fn_make_var(NULL, _t848, 1), _t849, gem_int(0)};
    GemVal _t851[] = {gem_v_conditions, gem_fn_make_call(NULL, _t850, 3)};
            (void)(gem_push_fn(NULL, _t851, 2));
#line 515 "compiler/main.gem"
    GemVal _t852[] = {gem_v_target_expr, gem_v_key_str};
            GemVal gem_v_sub_target = gem_fn_make_index(NULL, _t852, 2);
#line 516 "compiler/main.gem"
    GemVal _t853 = (*gem_v_peek);
    GemVal _t854 = _t853.fn(_t853.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t854, gem_string("type")), gem_string(":")))) {
#line 517 "compiler/main.gem"
    GemVal _t855 = (*gem_v_advance);
                (void)(_t855.fn(_t855.env, NULL, 0));
#line 518 "compiler/main.gem"
    GemVal _t856 = (*gem_v_skip_nl);
                (void)(_t856.fn(_t856.env, NULL, 0));
#line 519 "compiler/main.gem"
    GemVal _t857[] = {gem_v_sub_target};
    GemVal _t858 = (*gem_v_parse_pattern);
                GemVal gem_v_sub = _t858.fn(_t858.env, _t857, 1);
#line 520 "compiler/main.gem"
    GemVal _t859 = gem_v_sub;
    GemVal _t860 = gem_table_get(_t859, gem_string("condition"));
    GemVal _t863;
    if (gem_truthy(gem_neq(gem_table_get(_t860, gem_string("tag")), gem_string("bool")))) {
        _t863 = gem_neq(gem_table_get(_t860, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t861 = gem_v_sub;
        GemVal _t862 = gem_table_get(_t861, gem_string("condition"));
        _t863 = gem_neq(gem_table_get(_t862, gem_string("value")), gem_bool(1));
    }
                if (gem_truthy(_t863)) {
#line 521 "compiler/main.gem"
    GemVal _t864 = gem_v_sub;
    GemVal _t865[] = {gem_v_conditions, gem_table_get(_t864, gem_string("condition"))};
                    (void)(gem_push_fn(NULL, _t865, 2));
                }
#line 523 "compiler/main.gem"
    GemVal _t866 = gem_v_sub;
                GemVal gem_v__for_items_2 = gem_table_get(_t866, gem_string("bindings"));
#line 523 "compiler/main.gem"
                GemVal gem_v__for_i_2 = gem_int(0);
#line 523 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t867[] = {gem_v__for_items_2};
                    if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t867, 1)))) break;
#line 523 "compiler/main.gem"
                    GemVal gem_v_b = gem_table_get(gem_v__for_items_2, gem_v__for_i_2);
#line 523 "compiler/main.gem"
                    gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 524 "compiler/main.gem"
    GemVal _t868[] = {gem_v_bindings, gem_v_b};
                    (void)(gem_push_fn(NULL, _t868, 2));
                }

            } else {
#line 528 "compiler/main.gem"
    GemVal _t869[] = {gem_v_key_name, gem_v_sub_target, gem_int(0)};
    GemVal _t870[] = {gem_v_bindings, gem_fn_make_let(NULL, _t869, 3)};
                (void)(gem_push_fn(NULL, _t870, 2));
            }
#line 530 "compiler/main.gem"
    GemVal _t871 = (*gem_v_skip_nl);
            (void)(_t871.fn(_t871.env, NULL, 0));
#line 531 "compiler/main.gem"
    GemVal _t872 = (*gem_v_peek);
    GemVal _t873 = _t872.fn(_t872.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t873, gem_string("type")), gem_string(",")))) {
#line 532 "compiler/main.gem"
    GemVal _t874 = (*gem_v_advance);
                (void)(_t874.fn(_t874.env, NULL, 0));
#line 533 "compiler/main.gem"
    GemVal _t875 = (*gem_v_skip_nl);
                (void)(_t875.fn(_t875.env, NULL, 0));
            }
        }
#line 536 "compiler/main.gem"
    GemVal _t876[] = {gem_string("}")};
    GemVal _t877 = (*gem_v_expect);
        (void)(_t877.fn(_t877.env, _t876, 1));
#line 537 "compiler/main.gem"
    GemVal _t878 = gem_table_new();
    GemVal _t879[] = {gem_v_conditions};
    GemVal _t880 = gem_v_and_chain;
    gem_table_set(_t878, gem_string("condition"), _t880.fn(_t880.env, _t879, 1));
    gem_table_set(_t878, gem_string("bindings"), gem_v_bindings);
        GemVal _t881 = _t878;
        gem_pop_frame();
        return _t881;
    }
#line 541 "compiler/main.gem"
    GemVal _t882 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t882, gem_string("type")), gem_string("[")))) {
#line 542 "compiler/main.gem"
    GemVal _t883 = (*gem_v_advance);
        (void)(_t883.fn(_t883.env, NULL, 0));
#line 543 "compiler/main.gem"
    GemVal _t884 = (*gem_v_skip_nl);
        (void)(_t884.fn(_t884.env, NULL, 0));
#line 544 "compiler/main.gem"
    GemVal _t885 = gem_table_new();
        GemVal gem_v_sub_patterns = _t885;
#line 545 "compiler/main.gem"
        GemVal gem_v_idx = gem_int(0);
#line 546 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t886 = (*gem_v_peek);
            GemVal _t887 = _t886.fn(_t886.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t887, gem_string("type")), gem_string("]")))) break;
#line 547 "compiler/main.gem"
    GemVal _t888[] = {gem_v_idx};
    GemVal _t889[] = {gem_v_target_expr, gem_fn_make_int(NULL, _t888, 1)};
    GemVal _t890[] = {gem_fn_make_index(NULL, _t889, 2)};
    GemVal _t891 = (*gem_v_parse_pattern);
            GemVal gem_v_sub = _t891.fn(_t891.env, _t890, 1);
#line 548 "compiler/main.gem"
    GemVal _t892[] = {gem_v_sub_patterns, gem_v_sub};
            (void)(gem_push_fn(NULL, _t892, 2));
#line 549 "compiler/main.gem"
            gem_v_idx = gem_add(gem_v_idx, gem_int(1));
#line 550 "compiler/main.gem"
    GemVal _t893 = (*gem_v_skip_nl);
            (void)(_t893.fn(_t893.env, NULL, 0));
#line 551 "compiler/main.gem"
    GemVal _t894 = (*gem_v_peek);
    GemVal _t895 = _t894.fn(_t894.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t895, gem_string("type")), gem_string(",")))) {
#line 552 "compiler/main.gem"
    GemVal _t896 = (*gem_v_advance);
                (void)(_t896.fn(_t896.env, NULL, 0));
#line 553 "compiler/main.gem"
    GemVal _t897 = (*gem_v_skip_nl);
                (void)(_t897.fn(_t897.env, NULL, 0));
            }
        }
#line 556 "compiler/main.gem"
    GemVal _t898[] = {gem_string("]")};
    GemVal _t899 = (*gem_v_expect);
        (void)(_t899.fn(_t899.env, _t898, 1));
#line 557 "compiler/main.gem"
    GemVal _t900 = gem_table_new();
    GemVal _t901[] = {gem_string("type")};
    GemVal _t902 = gem_table_new();
    gem_table_set(_t902, gem_int(0), gem_v_target_expr);
    GemVal _t903[] = {gem_fn_make_var(NULL, _t901, 1), _t902, gem_int(0)};
    GemVal _t904[] = {gem_string("table")};
    GemVal _t905[] = {gem_string("=="), gem_fn_make_call(NULL, _t903, 3), gem_fn_make_string(NULL, _t904, 1)};
    gem_table_set(_t900, gem_int(0), gem_fn_make_binop(NULL, _t905, 3));
        GemVal gem_v_conditions = _t900;
#line 558 "compiler/main.gem"
    GemVal _t906[] = {gem_string("len")};
    GemVal _t907 = gem_table_new();
    gem_table_set(_t907, gem_int(0), gem_v_target_expr);
    GemVal _t908[] = {gem_fn_make_var(NULL, _t906, 1), _t907, gem_int(0)};
    GemVal _t909[] = {gem_v_idx};
    GemVal _t910[] = {gem_string("=="), gem_fn_make_call(NULL, _t908, 3), gem_fn_make_int(NULL, _t909, 1)};
    GemVal _t911[] = {gem_v_conditions, gem_fn_make_binop(NULL, _t910, 3)};
        (void)(gem_push_fn(NULL, _t911, 2));
#line 559 "compiler/main.gem"
    GemVal _t912 = gem_table_new();
        GemVal gem_v_bindings = _t912;
#line 560 "compiler/main.gem"
        GemVal gem_v__for_i_4 = gem_int(0);
#line 560 "compiler/main.gem"
    GemVal _t913[] = {gem_v_sub_patterns};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t913, 1);
#line 560 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 560 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_4;
#line 560 "compiler/main.gem"
            gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 561 "compiler/main.gem"
            GemVal gem_v_sub = gem_table_get(gem_v_sub_patterns, gem_v_i);
#line 562 "compiler/main.gem"
    GemVal _t914 = gem_v_sub;
    GemVal _t915 = gem_table_get(_t914, gem_string("condition"));
    GemVal _t918;
    if (gem_truthy(gem_neq(gem_table_get(_t915, gem_string("tag")), gem_string("bool")))) {
        _t918 = gem_neq(gem_table_get(_t915, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t916 = gem_v_sub;
        GemVal _t917 = gem_table_get(_t916, gem_string("condition"));
        _t918 = gem_neq(gem_table_get(_t917, gem_string("value")), gem_bool(1));
    }
            if (gem_truthy(_t918)) {
#line 563 "compiler/main.gem"
    GemVal _t919 = gem_v_sub;
    GemVal _t920[] = {gem_v_conditions, gem_table_get(_t919, gem_string("condition"))};
                (void)(gem_push_fn(NULL, _t920, 2));
            }
#line 565 "compiler/main.gem"
    GemVal _t921 = gem_v_sub;
            GemVal gem_v__for_items_3 = gem_table_get(_t921, gem_string("bindings"));
#line 565 "compiler/main.gem"
            GemVal gem_v__for_i_3 = gem_int(0);
#line 565 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t922[] = {gem_v__for_items_3};
                if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_len_fn(NULL, _t922, 1)))) break;
#line 565 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_3, gem_v__for_i_3);
#line 565 "compiler/main.gem"
                gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 566 "compiler/main.gem"
    GemVal _t923[] = {gem_v_bindings, gem_v_b};
                (void)(gem_push_fn(NULL, _t923, 2));
            }

        }

#line 569 "compiler/main.gem"
    GemVal _t924 = gem_table_new();
    GemVal _t925[] = {gem_v_conditions};
    GemVal _t926 = gem_v_and_chain;
    gem_table_set(_t924, gem_string("condition"), _t926.fn(_t926.env, _t925, 1));
    gem_table_set(_t924, gem_string("bindings"), gem_v_bindings);
        GemVal _t927 = _t924;
        gem_pop_frame();
        return _t927;
    }
#line 573 "compiler/main.gem"
    GemVal _t928 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t928, gem_string("type")), gem_string("NUMBER")))) {
#line 574 "compiler/main.gem"
    GemVal _t929 = (*gem_v_advance);
        (void)(_t929.fn(_t929.env, NULL, 0));
#line 575 "compiler/main.gem"
        GemVal gem_v_lit = GEM_NIL;
#line 576 "compiler/main.gem"
    GemVal _t930 = gem_v_t;
    GemVal _t931[] = {gem_table_get(_t930, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t931, 1))) {
#line 577 "compiler/main.gem"
    GemVal _t932 = gem_v_t;
    GemVal _t933[] = {gem_table_get(_t932, gem_string("value"))};
    GemVal _t934[] = {gem_fn_atof(NULL, _t933, 1)};
            gem_v_lit = gem_fn_make_float(NULL, _t934, 1);
        } else {
#line 579 "compiler/main.gem"
    GemVal _t935 = gem_v_t;
    GemVal _t936[] = {gem_table_get(_t935, gem_string("value"))};
    GemVal _t937[] = {gem_fn_str_to_int(NULL, _t936, 1)};
            gem_v_lit = gem_fn_make_int(NULL, _t937, 1);
        }
#line 581 "compiler/main.gem"
    GemVal _t938 = gem_table_new();
    GemVal _t939[] = {gem_string("=="), gem_v_target_expr, gem_v_lit};
    gem_table_set(_t938, gem_string("condition"), gem_fn_make_binop(NULL, _t939, 3));
    GemVal _t940 = gem_table_new();
    gem_table_set(_t938, gem_string("bindings"), _t940);
        GemVal _t941 = _t938;
        gem_pop_frame();
        return _t941;
    }
#line 583 "compiler/main.gem"
    GemVal _t942 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t942, gem_string("type")), gem_string("STRING")))) {
#line 584 "compiler/main.gem"
    GemVal _t943 = (*gem_v_advance);
        (void)(_t943.fn(_t943.env, NULL, 0));
#line 585 "compiler/main.gem"
    GemVal _t944 = gem_table_new();
    GemVal _t945 = gem_v_t;
    GemVal _t946[] = {gem_table_get(_t945, gem_string("value"))};
    GemVal _t947[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_string(NULL, _t946, 1)};
    gem_table_set(_t944, gem_string("condition"), gem_fn_make_binop(NULL, _t947, 3));
    GemVal _t948 = gem_table_new();
    gem_table_set(_t944, gem_string("bindings"), _t948);
        GemVal _t949 = _t944;
        gem_pop_frame();
        return _t949;
    }
#line 587 "compiler/main.gem"
    GemVal _t950 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t950, gem_string("type")), gem_string("true")))) {
#line 588 "compiler/main.gem"
    GemVal _t951 = (*gem_v_advance);
        (void)(_t951.fn(_t951.env, NULL, 0));
#line 589 "compiler/main.gem"
    GemVal _t952 = gem_table_new();
    GemVal _t953[] = {gem_bool(1)};
    GemVal _t954[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_bool(NULL, _t953, 1)};
    gem_table_set(_t952, gem_string("condition"), gem_fn_make_binop(NULL, _t954, 3));
    GemVal _t955 = gem_table_new();
    gem_table_set(_t952, gem_string("bindings"), _t955);
        GemVal _t956 = _t952;
        gem_pop_frame();
        return _t956;
    }
#line 591 "compiler/main.gem"
    GemVal _t957 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t957, gem_string("type")), gem_string("false")))) {
#line 592 "compiler/main.gem"
    GemVal _t958 = (*gem_v_advance);
        (void)(_t958.fn(_t958.env, NULL, 0));
#line 593 "compiler/main.gem"
    GemVal _t959 = gem_table_new();
    GemVal _t960[] = {gem_bool(0)};
    GemVal _t961[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_bool(NULL, _t960, 1)};
    gem_table_set(_t959, gem_string("condition"), gem_fn_make_binop(NULL, _t961, 3));
    GemVal _t962 = gem_table_new();
    gem_table_set(_t959, gem_string("bindings"), _t962);
        GemVal _t963 = _t959;
        gem_pop_frame();
        return _t963;
    }
#line 595 "compiler/main.gem"
    GemVal _t964 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t964, gem_string("type")), gem_string("nil")))) {
#line 596 "compiler/main.gem"
    GemVal _t965 = (*gem_v_advance);
        (void)(_t965.fn(_t965.env, NULL, 0));
#line 597 "compiler/main.gem"
    GemVal _t966 = gem_table_new();
    GemVal _t967[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_nil_node(NULL, NULL, 0)};
    gem_table_set(_t966, gem_string("condition"), gem_fn_make_binop(NULL, _t967, 3));
    GemVal _t968 = gem_table_new();
    gem_table_set(_t966, gem_string("bindings"), _t968);
        GemVal _t969 = _t966;
        gem_pop_frame();
        return _t969;
    }
#line 601 "compiler/main.gem"
    GemVal _t970 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t970, gem_string("type")), gem_string("NAME")))) {
#line 602 "compiler/main.gem"
    GemVal _t971 = (*gem_v_advance);
        (void)(_t971.fn(_t971.env, NULL, 0));
#line 603 "compiler/main.gem"
    GemVal _t972 = gem_table_new();
    GemVal _t973[] = {gem_bool(1)};
    gem_table_set(_t972, gem_string("condition"), gem_fn_make_bool(NULL, _t973, 1));
    GemVal _t974 = gem_table_new();
    GemVal _t975 = gem_v_t;
    GemVal _t976[] = {gem_table_get(_t975, gem_string("value")), gem_v_target_expr, gem_int(0)};
    gem_table_set(_t974, gem_int(0), gem_fn_make_let(NULL, _t976, 3));
    gem_table_set(_t972, gem_string("bindings"), _t974);
        GemVal _t977 = _t972;
        gem_pop_frame();
        return _t977;
    }
#line 606 "compiler/main.gem"
    GemVal _t978 = gem_v_t;
    GemVal _t979 = gem_v_t;
    GemVal _t980 = gem_v_t;
    GemVal _t981[] = {gem_table_get(_t980, gem_string("value"))};
    GemVal _t982 = gem_v_t;
    GemVal _t983[] = {gem_table_get(_t982, gem_string("type"))};
    GemVal _t984[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t978, gem_string("line")), gem_table_get(_t979, gem_string("col")), gem_len_fn(NULL, _t981, 1), gem_add(gem_add(gem_string("expected pattern but got '"), gem_to_string_fn(NULL, _t983, 1)), gem_string("'")), GEM_NIL};
    GemVal _t985 = gem_fn_compile_error(NULL, _t984, 7);
    gem_pop_frame();
    return _t985;
}

struct _closure__anon_22 {
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_file;
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
    GemVal *gem_v_source;
};
static GemVal _anon_22(void *_env, GemVal *args, int argc) {
    struct _closure__anon_22 *_cls = (struct _closure__anon_22 *)_env;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_file = _cls->gem_v_file;
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
    GemVal *gem_v_source = _cls->gem_v_source;
    gem_push_frame("_anon_22", "compiler/main.gem", 0);
#line 612 "compiler/main.gem"
    GemVal _t987 = (*gem_v_peek);
    GemVal gem_v_t = _t987.fn(_t987.env, NULL, 0);
#line 615 "compiler/main.gem"
    GemVal _t988 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t988, gem_string("type")), gem_string("let")))) {
#line 616 "compiler/main.gem"
    GemVal _t989 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t989, gem_string("line"));
#line 617 "compiler/main.gem"
    GemVal _t990 = (*gem_v_advance);
        (void)(_t990.fn(_t990.env, NULL, 0));
#line 620 "compiler/main.gem"
    GemVal _t991 = (*gem_v_peek);
    GemVal _t992 = _t991.fn(_t991.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t992, gem_string("type")), gem_string("{")))) {
#line 621 "compiler/main.gem"
    GemVal _t993 = (*gem_v_advance);
            (void)(_t993.fn(_t993.env, NULL, 0));
#line 622 "compiler/main.gem"
    GemVal _t994 = gem_table_new();
            GemVal gem_v_names = _t994;
#line 623 "compiler/main.gem"
    GemVal _t995 = (*gem_v_skip_nl);
            (void)(_t995.fn(_t995.env, NULL, 0));
#line 624 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t996 = (*gem_v_peek);
                GemVal _t997 = _t996.fn(_t996.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t997, gem_string("type")), gem_string("}")))) break;
#line 625 "compiler/main.gem"
    GemVal _t998 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t998.fn(_t998.env, NULL, 0);
#line 626 "compiler/main.gem"
    GemVal _t999 = gem_v_field_tok;
                if (gem_truthy(gem_eq(gem_table_get(_t999, gem_string("type")), gem_string("NAME")))) {
#line 627 "compiler/main.gem"
    GemVal _t1000 = (*gem_v_advance);
                    (void)(_t1000.fn(_t1000.env, NULL, 0));
                } else {
#line 630 "compiler/main.gem"
    GemVal _t1001 = (*gem_v_advance);
                    (void)(_t1001.fn(_t1001.env, NULL, 0));
                }
#line 632 "compiler/main.gem"
    GemVal _t1002 = gem_v_field_tok;
    GemVal _t1003[] = {gem_v_names, gem_table_get(_t1002, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1003, 2));
#line 633 "compiler/main.gem"
    GemVal _t1004 = (*gem_v_skip_nl);
                (void)(_t1004.fn(_t1004.env, NULL, 0));
#line 634 "compiler/main.gem"
    GemVal _t1005 = (*gem_v_peek);
    GemVal _t1006 = _t1005.fn(_t1005.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1006, gem_string("type")), gem_string(",")))) {
#line 635 "compiler/main.gem"
    GemVal _t1007 = (*gem_v_advance);
                    (void)(_t1007.fn(_t1007.env, NULL, 0));
#line 636 "compiler/main.gem"
    GemVal _t1008 = (*gem_v_skip_nl);
                    (void)(_t1008.fn(_t1008.env, NULL, 0));
                }
            }
#line 639 "compiler/main.gem"
    GemVal _t1009[] = {gem_string("}")};
    GemVal _t1010 = (*gem_v_expect);
            (void)(_t1010.fn(_t1010.env, _t1009, 1));
#line 640 "compiler/main.gem"
    GemVal _t1011[] = {gem_string("=")};
    GemVal _t1012 = (*gem_v_expect);
            (void)(_t1012.fn(_t1012.env, _t1011, 1));
#line 641 "compiler/main.gem"
    GemVal _t1013 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1013.fn(_t1013.env, NULL, 0);
#line 642 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 643 "compiler/main.gem"
    GemVal _t1014[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1014, 1));
#line 644 "compiler/main.gem"
    GemVal _t1015 = gem_table_new();
    GemVal _t1016[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t1015, gem_int(0), gem_fn_make_let(NULL, _t1016, 3));
            GemVal gem_v_stmts = _t1015;
#line 645 "compiler/main.gem"
            GemVal gem_v__for_i_5 = gem_int(0);
#line 645 "compiler/main.gem"
    GemVal _t1017[] = {gem_v_names};
            GemVal gem_v__for_limit_5 = gem_len_fn(NULL, _t1017, 1);
#line 645 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_limit_5))) break;
#line 645 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_5;
#line 645 "compiler/main.gem"
                gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 646 "compiler/main.gem"
    GemVal _t1018[] = {gem_v_tmp};
    GemVal _t1019[] = {gem_fn_make_var(NULL, _t1018, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t1020[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_dot(NULL, _t1019, 2), gem_v_line};
    GemVal _t1021[] = {gem_v_stmts, gem_fn_make_let(NULL, _t1020, 3)};
                (void)(gem_push_fn(NULL, _t1021, 2));
            }

#line 648 "compiler/main.gem"
    GemVal _t1022 = gem_table_new();
    gem_table_set(_t1022, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1022, gem_string("stmts"), gem_v_stmts);
            GemVal _t1023 = _t1022;
            gem_pop_frame();
            return _t1023;
        }
#line 652 "compiler/main.gem"
    GemVal _t1024 = (*gem_v_peek);
    GemVal _t1025 = _t1024.fn(_t1024.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1025, gem_string("type")), gem_string("[")))) {
#line 653 "compiler/main.gem"
    GemVal _t1026 = (*gem_v_advance);
            (void)(_t1026.fn(_t1026.env, NULL, 0));
#line 654 "compiler/main.gem"
    GemVal _t1027 = gem_table_new();
            GemVal gem_v_names = _t1027;
#line 655 "compiler/main.gem"
    GemVal _t1028 = (*gem_v_skip_nl);
            (void)(_t1028.fn(_t1028.env, NULL, 0));
#line 656 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1029 = (*gem_v_peek);
                GemVal _t1030 = _t1029.fn(_t1029.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1030, gem_string("type")), gem_string("]")))) break;
#line 657 "compiler/main.gem"
    GemVal _t1031[] = {gem_string("NAME")};
    GemVal _t1032 = (*gem_v_expect);
    GemVal _t1033 = _t1032.fn(_t1032.env, _t1031, 1);
    GemVal _t1034[] = {gem_v_names, gem_table_get(_t1033, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1034, 2));
#line 658 "compiler/main.gem"
    GemVal _t1035 = (*gem_v_skip_nl);
                (void)(_t1035.fn(_t1035.env, NULL, 0));
#line 659 "compiler/main.gem"
    GemVal _t1036 = (*gem_v_peek);
    GemVal _t1037 = _t1036.fn(_t1036.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1037, gem_string("type")), gem_string(",")))) {
#line 660 "compiler/main.gem"
    GemVal _t1038 = (*gem_v_advance);
                    (void)(_t1038.fn(_t1038.env, NULL, 0));
#line 661 "compiler/main.gem"
    GemVal _t1039 = (*gem_v_skip_nl);
                    (void)(_t1039.fn(_t1039.env, NULL, 0));
                }
            }
#line 664 "compiler/main.gem"
    GemVal _t1040[] = {gem_string("]")};
    GemVal _t1041 = (*gem_v_expect);
            (void)(_t1041.fn(_t1041.env, _t1040, 1));
#line 665 "compiler/main.gem"
    GemVal _t1042[] = {gem_string("=")};
    GemVal _t1043 = (*gem_v_expect);
            (void)(_t1043.fn(_t1043.env, _t1042, 1));
#line 666 "compiler/main.gem"
    GemVal _t1044 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1044.fn(_t1044.env, NULL, 0);
#line 667 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 668 "compiler/main.gem"
    GemVal _t1045[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1045, 1));
#line 669 "compiler/main.gem"
    GemVal _t1046 = gem_table_new();
    GemVal _t1047[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t1046, gem_int(0), gem_fn_make_let(NULL, _t1047, 3));
            GemVal gem_v_stmts = _t1046;
#line 670 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 670 "compiler/main.gem"
    GemVal _t1048[] = {gem_v_names};
            GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t1048, 1);
#line 670 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
#line 670 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_6;
#line 670 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 671 "compiler/main.gem"
    GemVal _t1049[] = {gem_v_tmp};
    GemVal _t1050[] = {gem_v_di};
    GemVal _t1051[] = {gem_fn_make_var(NULL, _t1049, 1), gem_fn_make_int(NULL, _t1050, 1)};
    GemVal _t1052[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_index(NULL, _t1051, 2), gem_v_line};
    GemVal _t1053[] = {gem_v_stmts, gem_fn_make_let(NULL, _t1052, 3)};
                (void)(gem_push_fn(NULL, _t1053, 2));
            }

#line 673 "compiler/main.gem"
    GemVal _t1054 = gem_table_new();
    gem_table_set(_t1054, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1054, gem_string("stmts"), gem_v_stmts);
            GemVal _t1055 = _t1054;
            gem_pop_frame();
            return _t1055;
        }
#line 676 "compiler/main.gem"
    GemVal _t1056[] = {gem_string("NAME")};
    GemVal _t1057 = (*gem_v_expect);
    GemVal _t1058 = _t1057.fn(_t1057.env, _t1056, 1);
        GemVal gem_v_name = gem_table_get(_t1058, gem_string("value"));
#line 677 "compiler/main.gem"
    GemVal _t1059[] = {gem_string("=")};
    GemVal _t1060 = (*gem_v_expect);
        (void)(_t1060.fn(_t1060.env, _t1059, 1));
#line 678 "compiler/main.gem"
    GemVal _t1061 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t1061.fn(_t1061.env, NULL, 0);
#line 679 "compiler/main.gem"
    GemVal _t1062[] = {gem_v_name, gem_v_value, gem_v_line};
        GemVal _t1063 = gem_fn_make_let(NULL, _t1062, 3);
        gem_pop_frame();
        return _t1063;
    }
#line 683 "compiler/main.gem"
    GemVal _t1064 = gem_v_t;
    GemVal _t1068;
    if (!gem_truthy(gem_eq(gem_table_get(_t1064, gem_string("type")), gem_string("fn")))) {
        _t1068 = gem_eq(gem_table_get(_t1064, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t1065[] = {gem_int(1)};
        GemVal _t1066 = (*gem_v_peek_at);
        GemVal _t1067 = _t1066.fn(_t1066.env, _t1065, 1);
        _t1068 = gem_eq(gem_table_get(_t1067, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t1068)) {
#line 684 "compiler/main.gem"
    GemVal _t1069 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1069, gem_string("line"));
#line 685 "compiler/main.gem"
    GemVal _t1070 = (*gem_v_advance);
        (void)(_t1070.fn(_t1070.env, NULL, 0));
#line 686 "compiler/main.gem"
    GemVal _t1071[] = {gem_string("NAME")};
    GemVal _t1072 = (*gem_v_expect);
    GemVal _t1073 = _t1072.fn(_t1072.env, _t1071, 1);
        GemVal gem_v_name = gem_table_get(_t1073, gem_string("value"));
#line 687 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 688 "compiler/main.gem"
    GemVal _t1074[] = {gem_v_name};
    GemVal _t1075[] = {gem_v_name};
    GemVal _t1076[] = {(*gem_v_source), (*gem_v_file), gem_v_line, gem_int(1), gem_add(gem_len_fn(NULL, _t1074, 1), gem_int(3)), gem_string("named fn inside function body is not supported"), gem_add(gem_add(gem_string("use: let "), gem_to_string_fn(NULL, _t1075, 1)), gem_string(" = fn(...) ... end"))};
            (void)(gem_fn_compile_error(NULL, _t1076, 7));
        }
#line 690 "compiler/main.gem"
    GemVal _t1077[] = {gem_string("(")};
    GemVal _t1078 = (*gem_v_expect);
        (void)(_t1078.fn(_t1078.env, _t1077, 1));
#line 691 "compiler/main.gem"
    GemVal _t1079 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t1079.fn(_t1079.env, NULL, 0);
#line 692 "compiler/main.gem"
    GemVal _t1080[] = {gem_string(")")};
    GemVal _t1081 = (*gem_v_expect);
        (void)(_t1081.fn(_t1081.env, _t1080, 1));
#line 693 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 694 "compiler/main.gem"
    GemVal _t1082 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t1082.fn(_t1082.env, NULL, 0);
#line 695 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 696 "compiler/main.gem"
    GemVal _t1083[] = {gem_string("end")};
    GemVal _t1084 = (*gem_v_expect);
        (void)(_t1084.fn(_t1084.env, _t1083, 1));
#line 697 "compiler/main.gem"
    GemVal _t1085 = gem_v_pr;
    GemVal _t1086 = gem_v_pr;
    GemVal _t1087 = gem_v_pr;
    GemVal _t1088[] = {gem_v_name, gem_table_get(_t1085, gem_string("params")), gem_table_get(_t1086, gem_string("rest_param")), gem_table_get(_t1087, gem_string("block_param")), gem_v_body, gem_v_line};
        GemVal _t1089 = gem_fn_make_fn_def(NULL, _t1088, 6);
        gem_pop_frame();
        return _t1089;
    }
#line 701 "compiler/main.gem"
    GemVal _t1090 = gem_v_t;
    GemVal _t1092;
    if (gem_truthy(gem_eq(gem_table_get(_t1090, gem_string("type")), gem_string("if")))) {
        _t1092 = gem_eq(gem_table_get(_t1090, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t1091 = gem_v_t;
        _t1092 = gem_eq(gem_table_get(_t1091, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t1092)) {
#line 702 "compiler/main.gem"
    GemVal _t1093 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1093, gem_string("line"));
#line 703 "compiler/main.gem"
    GemVal _t1094 = (*gem_v_advance);
        (void)(_t1094.fn(_t1094.env, NULL, 0));
#line 704 "compiler/main.gem"
    GemVal _t1095 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1095.fn(_t1095.env, NULL, 0);
#line 705 "compiler/main.gem"
    GemVal _t1096 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t1096.fn(_t1096.env, NULL, 0);
#line 706 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 707 "compiler/main.gem"
    GemVal _t1097 = (*gem_v_peek);
    GemVal _t1098 = _t1097.fn(_t1097.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1098, gem_string("type")), gem_string("elif")))) {
#line 709 "compiler/main.gem"
    GemVal _t1099 = gem_table_new();
    GemVal _t1100 = (*gem_v_parse_stmt);
    gem_table_set(_t1099, gem_int(0), _t1100.fn(_t1100.env, NULL, 0));
            gem_v_else_body = _t1099;
        } else {
#line 710 "compiler/main.gem"
    GemVal _t1101 = (*gem_v_peek);
    GemVal _t1102 = _t1101.fn(_t1101.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1102, gem_string("type")), gem_string("else")))) {
#line 711 "compiler/main.gem"
    GemVal _t1103 = (*gem_v_advance);
                (void)(_t1103.fn(_t1103.env, NULL, 0));
#line 712 "compiler/main.gem"
    GemVal _t1104 = (*gem_v_parse_body);
                gem_v_else_body = _t1104.fn(_t1104.env, NULL, 0);
            }
        }
#line 715 "compiler/main.gem"
    GemVal _t1105 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t1105, gem_string("type")), gem_string("if")))) {
#line 716 "compiler/main.gem"
    GemVal _t1106[] = {gem_string("end")};
    GemVal _t1107 = (*gem_v_expect);
            (void)(_t1107.fn(_t1107.env, _t1106, 1));
        }
#line 718 "compiler/main.gem"
    GemVal _t1108[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        GemVal _t1109 = gem_fn_make_if(NULL, _t1108, 4);
        gem_pop_frame();
        return _t1109;
    }
#line 722 "compiler/main.gem"
    GemVal _t1110 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1110, gem_string("type")), gem_string("while")))) {
#line 723 "compiler/main.gem"
    GemVal _t1111 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1111, gem_string("line"));
#line 724 "compiler/main.gem"
    GemVal _t1112 = (*gem_v_advance);
        (void)(_t1112.fn(_t1112.env, NULL, 0));
#line 725 "compiler/main.gem"
    GemVal _t1113 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1113.fn(_t1113.env, NULL, 0);
#line 726 "compiler/main.gem"
    GemVal _t1114 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t1114.fn(_t1114.env, NULL, 0);
#line 727 "compiler/main.gem"
    GemVal _t1115[] = {gem_string("end")};
    GemVal _t1116 = (*gem_v_expect);
        (void)(_t1116.fn(_t1116.env, _t1115, 1));
#line 728 "compiler/main.gem"
    GemVal _t1117[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        GemVal _t1118 = gem_fn_make_while(NULL, _t1117, 3);
        gem_pop_frame();
        return _t1118;
    }
#line 732 "compiler/main.gem"
    GemVal _t1119 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1119, gem_string("type")), gem_string("for")))) {
#line 733 "compiler/main.gem"
    GemVal _t1120 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1120, gem_string("line"));
#line 734 "compiler/main.gem"
    GemVal _t1121 = (*gem_v_advance);
        (void)(_t1121.fn(_t1121.env, NULL, 0));
#line 735 "compiler/main.gem"
    GemVal _t1122[] = {gem_string("NAME")};
    GemVal _t1123 = (*gem_v_expect);
    GemVal _t1124 = _t1123.fn(_t1123.env, _t1122, 1);
        GemVal gem_v_var_name = gem_table_get(_t1124, gem_string("value"));
#line 737 "compiler/main.gem"
    GemVal _t1125 = (*gem_v_peek);
    GemVal _t1126 = _t1125.fn(_t1125.env, NULL, 0);
    GemVal _t1130;
    if (!gem_truthy(gem_eq(gem_table_get(_t1126, gem_string("type")), gem_string(",")))) {
        _t1130 = gem_eq(gem_table_get(_t1126, gem_string("type")), gem_string(","));
    } else {
        GemVal _t1127[] = {gem_int(1)};
        GemVal _t1128 = (*gem_v_peek_at);
        GemVal _t1129 = _t1128.fn(_t1128.env, _t1127, 1);
        _t1130 = gem_eq(gem_table_get(_t1129, gem_string("type")), gem_string("NAME"));
    }
        if (gem_truthy(_t1130)) {
#line 739 "compiler/main.gem"
    GemVal _t1131 = (*gem_v_advance);
            (void)(_t1131.fn(_t1131.env, NULL, 0));
#line 740 "compiler/main.gem"
    GemVal _t1132[] = {gem_string("NAME")};
    GemVal _t1133 = (*gem_v_expect);
    GemVal _t1134 = _t1133.fn(_t1133.env, _t1132, 1);
            GemVal gem_v_val_name = gem_table_get(_t1134, gem_string("value"));
#line 741 "compiler/main.gem"
    GemVal _t1135[] = {gem_string("in")};
    GemVal _t1136 = (*gem_v_expect);
            (void)(_t1136.fn(_t1136.env, _t1135, 1));
#line 742 "compiler/main.gem"
    GemVal _t1137 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t1137.fn(_t1137.env, NULL, 0);
#line 743 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 744 "compiler/main.gem"
    GemVal _t1138[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_add(gem_string("_for_tbl_"), gem_to_string_fn(NULL, _t1138, 1));
#line 745 "compiler/main.gem"
    GemVal _t1139[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_keys_var = gem_add(gem_string("_for_keys_"), gem_to_string_fn(NULL, _t1139, 1));
#line 746 "compiler/main.gem"
    GemVal _t1140[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1140, 1));
#line 747 "compiler/main.gem"
    GemVal _t1141 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t1141.fn(_t1141.env, NULL, 0);
#line 748 "compiler/main.gem"
    GemVal _t1142[] = {gem_string("end")};
    GemVal _t1143 = (*gem_v_expect);
            (void)(_t1143.fn(_t1143.env, _t1142, 1));
#line 749 "compiler/main.gem"
    GemVal _t1144 = gem_table_new();
            GemVal gem_v_inner_stmts = _t1144;
#line 750 "compiler/main.gem"
    GemVal _t1145[] = {gem_v_keys_var};
    GemVal _t1146[] = {gem_v_idx_var};
    GemVal _t1147[] = {gem_fn_make_var(NULL, _t1145, 1), gem_fn_make_var(NULL, _t1146, 1)};
    GemVal _t1148[] = {gem_v_var_name, gem_fn_make_index(NULL, _t1147, 2), gem_v_line};
    GemVal _t1149[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1148, 3)};
            (void)(gem_push_fn(NULL, _t1149, 2));
#line 751 "compiler/main.gem"
    GemVal _t1150[] = {gem_v_tbl_var};
    GemVal _t1151[] = {gem_v_keys_var};
    GemVal _t1152[] = {gem_v_idx_var};
    GemVal _t1153[] = {gem_fn_make_var(NULL, _t1151, 1), gem_fn_make_var(NULL, _t1152, 1)};
    GemVal _t1154[] = {gem_fn_make_var(NULL, _t1150, 1), gem_fn_make_index(NULL, _t1153, 2)};
    GemVal _t1155[] = {gem_v_val_name, gem_fn_make_index(NULL, _t1154, 2), gem_v_line};
    GemVal _t1156[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1155, 3)};
            (void)(gem_push_fn(NULL, _t1156, 2));
#line 752 "compiler/main.gem"
    GemVal _t1157[] = {gem_v_idx_var};
    GemVal _t1158[] = {gem_int(1)};
    GemVal _t1159[] = {gem_string("+"), gem_fn_make_var(NULL, _t1157, 1), gem_fn_make_int(NULL, _t1158, 1)};
    GemVal _t1160[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1159, 3), gem_v_line};
    GemVal _t1161[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1160, 3)};
            (void)(gem_push_fn(NULL, _t1161, 2));
#line 753 "compiler/main.gem"
            GemVal gem_v__for_i_7 = gem_int(0);
#line 753 "compiler/main.gem"
    GemVal _t1162[] = {gem_v_fbody};
            GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1162, 1);
#line 753 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 753 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_7;
#line 753 "compiler/main.gem"
                gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 754 "compiler/main.gem"
    GemVal _t1163[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t1163, 2));
            }

#line 756 "compiler/main.gem"
    GemVal _t1164[] = {gem_v_idx_var};
    GemVal _t1165[] = {gem_string("len")};
    GemVal _t1166 = gem_table_new();
    GemVal _t1167[] = {gem_v_keys_var};
    gem_table_set(_t1166, gem_int(0), gem_fn_make_var(NULL, _t1167, 1));
    GemVal _t1168[] = {gem_fn_make_var(NULL, _t1165, 1), _t1166, gem_int(0)};
    GemVal _t1169[] = {gem_string("<"), gem_fn_make_var(NULL, _t1164, 1), gem_fn_make_call(NULL, _t1168, 3)};
    GemVal _t1170[] = {gem_fn_make_binop(NULL, _t1169, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1170, 3);
#line 761 "compiler/main.gem"
    GemVal _t1171 = gem_table_new();
    gem_table_set(_t1171, gem_string("tag"), gem_string("block"));
    GemVal _t1172 = gem_table_new();
    GemVal _t1173[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    gem_table_set(_t1172, gem_int(0), gem_fn_make_let(NULL, _t1173, 3));
    GemVal _t1174[] = {gem_string("keys")};
    GemVal _t1175 = gem_table_new();
    GemVal _t1176[] = {gem_v_tbl_var};
    gem_table_set(_t1175, gem_int(0), gem_fn_make_var(NULL, _t1176, 1));
    GemVal _t1177[] = {gem_fn_make_var(NULL, _t1174, 1), _t1175, gem_int(0)};
    GemVal _t1178[] = {gem_v_keys_var, gem_fn_make_call(NULL, _t1177, 3), gem_v_line};
    gem_table_set(_t1172, gem_int(1), gem_fn_make_let(NULL, _t1178, 3));
    GemVal _t1179[] = {gem_int(0)};
    GemVal _t1180[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t1179, 1), gem_v_line};
    gem_table_set(_t1172, gem_int(2), gem_fn_make_let(NULL, _t1180, 3));
    gem_table_set(_t1172, gem_int(3), gem_v_while_node);
    gem_table_set(_t1171, gem_string("stmts"), _t1172);
            GemVal _t1181 = _t1171;
            gem_pop_frame();
            return _t1181;
        } else {
#line 767 "compiler/main.gem"
    GemVal _t1182 = (*gem_v_peek);
    GemVal _t1183 = _t1182.fn(_t1182.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1183, gem_string("type")), gem_string("in")))) {
#line 769 "compiler/main.gem"
    GemVal _t1184 = (*gem_v_advance);
                (void)(_t1184.fn(_t1184.env, NULL, 0));
#line 770 "compiler/main.gem"
    GemVal _t1185 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t1185.fn(_t1185.env, NULL, 0);
#line 771 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 772 "compiler/main.gem"
    GemVal _t1186[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t1186, 1));
#line 773 "compiler/main.gem"
    GemVal _t1187[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1187, 1));
#line 774 "compiler/main.gem"
    GemVal _t1188 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t1188.fn(_t1188.env, NULL, 0);
#line 775 "compiler/main.gem"
    GemVal _t1189[] = {gem_string("end")};
    GemVal _t1190 = (*gem_v_expect);
                (void)(_t1190.fn(_t1190.env, _t1189, 1));
#line 776 "compiler/main.gem"
    GemVal _t1191 = gem_table_new();
                GemVal gem_v_inner_stmts = _t1191;
#line 777 "compiler/main.gem"
    GemVal _t1192[] = {gem_v_items_var};
    GemVal _t1193[] = {gem_v_idx_var};
    GemVal _t1194[] = {gem_fn_make_var(NULL, _t1192, 1), gem_fn_make_var(NULL, _t1193, 1)};
    GemVal _t1195[] = {gem_v_var_name, gem_fn_make_index(NULL, _t1194, 2), gem_v_line};
    GemVal _t1196[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1195, 3)};
                (void)(gem_push_fn(NULL, _t1196, 2));
#line 778 "compiler/main.gem"
    GemVal _t1197[] = {gem_v_idx_var};
    GemVal _t1198[] = {gem_int(1)};
    GemVal _t1199[] = {gem_string("+"), gem_fn_make_var(NULL, _t1197, 1), gem_fn_make_int(NULL, _t1198, 1)};
    GemVal _t1200[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1199, 3), gem_v_line};
    GemVal _t1201[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1200, 3)};
                (void)(gem_push_fn(NULL, _t1201, 2));
#line 779 "compiler/main.gem"
                GemVal gem_v__for_i_8 = gem_int(0);
#line 779 "compiler/main.gem"
    GemVal _t1202[] = {gem_v_fbody};
                GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1202, 1);
#line 779 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 779 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_8;
#line 779 "compiler/main.gem"
                    gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 780 "compiler/main.gem"
    GemVal _t1203[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t1203, 2));
                }

#line 782 "compiler/main.gem"
    GemVal _t1204[] = {gem_v_idx_var};
    GemVal _t1205[] = {gem_string("len")};
    GemVal _t1206 = gem_table_new();
    GemVal _t1207[] = {gem_v_items_var};
    gem_table_set(_t1206, gem_int(0), gem_fn_make_var(NULL, _t1207, 1));
    GemVal _t1208[] = {gem_fn_make_var(NULL, _t1205, 1), _t1206, gem_int(0)};
    GemVal _t1209[] = {gem_string("<"), gem_fn_make_var(NULL, _t1204, 1), gem_fn_make_call(NULL, _t1208, 3)};
    GemVal _t1210[] = {gem_fn_make_binop(NULL, _t1209, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1210, 3);
#line 787 "compiler/main.gem"
    GemVal _t1211 = gem_table_new();
    gem_table_set(_t1211, gem_string("tag"), gem_string("block"));
    GemVal _t1212 = gem_table_new();
    GemVal _t1213[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t1212, gem_int(0), gem_fn_make_let(NULL, _t1213, 3));
    GemVal _t1214[] = {gem_int(0)};
    GemVal _t1215[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t1214, 1), gem_v_line};
    gem_table_set(_t1212, gem_int(1), gem_fn_make_let(NULL, _t1215, 3));
    gem_table_set(_t1212, gem_int(2), gem_v_while_node);
    gem_table_set(_t1211, gem_string("stmts"), _t1212);
                GemVal _t1216 = _t1211;
                gem_pop_frame();
                return _t1216;
            } else {
#line 792 "compiler/main.gem"
    GemVal _t1217 = (*gem_v_peek);
    GemVal _t1218 = _t1217.fn(_t1217.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1218, gem_string("type")), gem_string("=")))) {
#line 794 "compiler/main.gem"
    GemVal _t1219 = (*gem_v_advance);
                    (void)(_t1219.fn(_t1219.env, NULL, 0));
#line 795 "compiler/main.gem"
    GemVal _t1220 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t1220.fn(_t1220.env, NULL, 0);
#line 796 "compiler/main.gem"
    GemVal _t1221[] = {gem_string(",")};
    GemVal _t1222 = (*gem_v_expect);
                    (void)(_t1222.fn(_t1222.env, _t1221, 1));
#line 797 "compiler/main.gem"
    GemVal _t1223 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1223.fn(_t1223.env, NULL, 0);
#line 798 "compiler/main.gem"
    GemVal _t1224 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1224.fn(_t1224.env, NULL, 0);
#line 799 "compiler/main.gem"
    GemVal _t1225[] = {gem_string("end")};
    GemVal _t1226 = (*gem_v_expect);
                    (void)(_t1226.fn(_t1226.env, _t1225, 1));
#line 800 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 801 "compiler/main.gem"
    GemVal _t1227[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1227, 1));
#line 802 "compiler/main.gem"
    GemVal _t1228[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t1228, 1));
#line 803 "compiler/main.gem"
    GemVal _t1229 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1229;
#line 804 "compiler/main.gem"
    GemVal _t1230[] = {gem_v_idx_var};
    GemVal _t1231[] = {gem_v_var_name, gem_fn_make_var(NULL, _t1230, 1), gem_v_line};
    GemVal _t1232[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1231, 3)};
                    (void)(gem_push_fn(NULL, _t1232, 2));
#line 805 "compiler/main.gem"
    GemVal _t1233[] = {gem_v_idx_var};
    GemVal _t1234[] = {gem_int(1)};
    GemVal _t1235[] = {gem_string("+"), gem_fn_make_var(NULL, _t1233, 1), gem_fn_make_int(NULL, _t1234, 1)};
    GemVal _t1236[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1235, 3), gem_v_line};
    GemVal _t1237[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1236, 3)};
                    (void)(gem_push_fn(NULL, _t1237, 2));
#line 806 "compiler/main.gem"
                    GemVal gem_v__for_i_9 = gem_int(0);
#line 806 "compiler/main.gem"
    GemVal _t1238[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1238, 1);
#line 806 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 806 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_9;
#line 806 "compiler/main.gem"
                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 807 "compiler/main.gem"
    GemVal _t1239[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1239, 2));
                    }

#line 809 "compiler/main.gem"
    GemVal _t1240[] = {gem_v_idx_var};
    GemVal _t1241[] = {gem_v_limit_var};
    GemVal _t1242[] = {gem_string("<"), gem_fn_make_var(NULL, _t1240, 1), gem_fn_make_var(NULL, _t1241, 1)};
    GemVal _t1243[] = {gem_fn_make_binop(NULL, _t1242, 3), gem_v_inner_stmts, gem_v_line};
                    GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1243, 3);
#line 814 "compiler/main.gem"
    GemVal _t1244 = gem_table_new();
    gem_table_set(_t1244, gem_string("tag"), gem_string("block"));
    GemVal _t1245 = gem_table_new();
    GemVal _t1246[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t1245, gem_int(0), gem_fn_make_let(NULL, _t1246, 3));
    GemVal _t1247[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t1245, gem_int(1), gem_fn_make_let(NULL, _t1247, 3));
    gem_table_set(_t1245, gem_int(2), gem_v_while_node);
    gem_table_set(_t1244, gem_string("stmts"), _t1245);
                    GemVal _t1248 = _t1244;
                    gem_pop_frame();
                    return _t1248;
                } else {
#line 820 "compiler/main.gem"
    GemVal _t1249 = (*gem_v_peek);
    GemVal _t1250 = _t1249.fn(_t1249.env, NULL, 0);
    GemVal _t1251 = (*gem_v_peek);
    GemVal _t1252 = _t1251.fn(_t1251.env, NULL, 0);
    GemVal _t1253 = (*gem_v_peek);
    GemVal _t1254 = _t1253.fn(_t1253.env, NULL, 0);
    GemVal _t1255[] = {gem_table_get(_t1254, gem_string("value"))};
    GemVal _t1256[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1250, gem_string("line")), gem_table_get(_t1252, gem_string("col")), gem_len_fn(NULL, _t1255, 1), gem_string("expected 'in' or '=' after for variable"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t1256, 7));
                }
            }
        }
    }
#line 825 "compiler/main.gem"
    GemVal _t1257 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1257, gem_string("type")), gem_string("match")))) {
#line 826 "compiler/main.gem"
    GemVal _t1258 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1258, gem_string("line"));
#line 827 "compiler/main.gem"
    GemVal _t1259 = (*gem_v_advance);
        (void)(_t1259.fn(_t1259.env, NULL, 0));
#line 828 "compiler/main.gem"
    GemVal _t1260 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1260.fn(_t1260.env, NULL, 0);
#line 829 "compiler/main.gem"
    GemVal _t1261 = (*gem_v_skip_nl);
        (void)(_t1261.fn(_t1261.env, NULL, 0));
#line 830 "compiler/main.gem"
        *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 831 "compiler/main.gem"
    GemVal _t1262[] = {(*gem_v_gensym_counter)};
        GemVal gem_v_match_var = gem_add(gem_string("_match_"), gem_to_string_fn(NULL, _t1262, 1));
#line 832 "compiler/main.gem"
    GemVal _t1263[] = {gem_v_match_var};
        GemVal gem_v_target_var_expr = gem_fn_make_var(NULL, _t1263, 1);
#line 833 "compiler/main.gem"
    GemVal _t1264 = gem_table_new();
        GemVal gem_v_whens = _t1264;
#line 834 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1265 = (*gem_v_peek);
            GemVal _t1266 = _t1265.fn(_t1265.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1266, gem_string("type")), gem_string("when")))) break;
#line 835 "compiler/main.gem"
    GemVal _t1267 = (*gem_v_advance);
            (void)(_t1267.fn(_t1267.env, NULL, 0));
#line 837 "compiler/main.gem"
    GemVal _t1268 = (*gem_v_peek);
    GemVal _t1269 = _t1268.fn(_t1268.env, NULL, 0);
            GemVal gem_v_pt = gem_table_get(_t1269, gem_string("type"));
#line 838 "compiler/main.gem"
    GemVal _t1270[] = {gem_int(1)};
    GemVal _t1271 = (*gem_v_peek_at);
    GemVal _t1272 = _t1271.fn(_t1271.env, _t1270, 1);
            GemVal gem_v_next = gem_table_get(_t1272, gem_string("type"));
#line 839 "compiler/main.gem"
    GemVal _t1273;
    if (gem_truthy(gem_eq(gem_v_pt, gem_string("{")))) {
        _t1273 = gem_eq(gem_v_pt, gem_string("{"));
    } else {
        _t1273 = gem_eq(gem_v_pt, gem_string("["));
    }
            if (gem_truthy(_t1273)) {
#line 840 "compiler/main.gem"
    GemVal _t1274[] = {gem_v_target_var_expr};
    GemVal _t1275 = (*gem_v_parse_pattern);
                GemVal gem_v_pat = _t1275.fn(_t1275.env, _t1274, 1);
#line 841 "compiler/main.gem"
    GemVal _t1276 = (*gem_v_parse_body);
                GemVal gem_v_wbody = _t1276.fn(_t1276.env, NULL, 0);
#line 842 "compiler/main.gem"
    GemVal _t1277 = gem_v_pat;
    GemVal _t1278 = gem_v_pat;
    GemVal _t1279[] = {gem_table_get(_t1277, gem_string("condition")), gem_v_wbody, gem_table_get(_t1278, gem_string("bindings"))};
    GemVal _t1280[] = {gem_v_whens, gem_fn_make_when(NULL, _t1279, 3)};
                (void)(gem_push_fn(NULL, _t1280, 2));
            } else {
#line 843 "compiler/main.gem"
    GemVal _t1285;
    if (!gem_truthy(gem_eq(gem_v_pt, gem_string("NAME")))) {
        _t1285 = gem_eq(gem_v_pt, gem_string("NAME"));
    } else {
        GemVal _t1281;
        if (gem_truthy(gem_eq(gem_v_next, gem_string("NEWLINE")))) {
                _t1281 = gem_eq(gem_v_next, gem_string("NEWLINE"));
        } else {
                _t1281 = gem_eq(gem_v_next, gem_string("EOF"));
        }
        GemVal _t1282;
        if (gem_truthy(_t1281)) {
                _t1282 = _t1281;
        } else {
                _t1282 = gem_eq(gem_v_next, gem_string("end"));
        }
        GemVal _t1283;
        if (gem_truthy(_t1282)) {
                _t1283 = _t1282;
        } else {
                _t1283 = gem_eq(gem_v_next, gem_string("else"));
        }
        GemVal _t1284;
        if (gem_truthy(_t1283)) {
                _t1284 = _t1283;
        } else {
                _t1284 = gem_eq(gem_v_next, gem_string("when"));
        }
        _t1285 = _t1284;
    }
                if (gem_truthy(_t1285)) {
#line 845 "compiler/main.gem"
    GemVal _t1286[] = {gem_v_target_var_expr};
    GemVal _t1287 = (*gem_v_parse_pattern);
                    GemVal gem_v_pat = _t1287.fn(_t1287.env, _t1286, 1);
#line 846 "compiler/main.gem"
    GemVal _t1288 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1288.fn(_t1288.env, NULL, 0);
#line 847 "compiler/main.gem"
    GemVal _t1289 = gem_v_pat;
    GemVal _t1290 = gem_v_pat;
    GemVal _t1291[] = {gem_table_get(_t1289, gem_string("condition")), gem_v_wbody, gem_table_get(_t1290, gem_string("bindings"))};
    GemVal _t1292[] = {gem_v_whens, gem_fn_make_when(NULL, _t1291, 3)};
                    (void)(gem_push_fn(NULL, _t1292, 2));
                } else {
#line 850 "compiler/main.gem"
    GemVal _t1293 = (*gem_v_parse_expr);
                    GemVal gem_v_wval = _t1293.fn(_t1293.env, NULL, 0);
#line 851 "compiler/main.gem"
    GemVal _t1294 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1294.fn(_t1294.env, NULL, 0);
#line 852 "compiler/main.gem"
    GemVal _t1295[] = {gem_string("=="), gem_v_target_var_expr, gem_v_wval};
    GemVal _t1296 = gem_table_new();
    GemVal _t1297[] = {gem_fn_make_binop(NULL, _t1295, 3), gem_v_wbody, _t1296};
    GemVal _t1298[] = {gem_v_whens, gem_fn_make_when(NULL, _t1297, 3)};
                    (void)(gem_push_fn(NULL, _t1298, 2));
                }
            }
#line 854 "compiler/main.gem"
    GemVal _t1299 = (*gem_v_skip_nl);
            (void)(_t1299.fn(_t1299.env, NULL, 0));
        }
#line 856 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 857 "compiler/main.gem"
    GemVal _t1300 = (*gem_v_peek);
    GemVal _t1301 = _t1300.fn(_t1300.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1301, gem_string("type")), gem_string("else")))) {
#line 858 "compiler/main.gem"
    GemVal _t1302 = (*gem_v_advance);
            (void)(_t1302.fn(_t1302.env, NULL, 0));
#line 859 "compiler/main.gem"
    GemVal _t1303 = (*gem_v_parse_body);
            gem_v_else_body = _t1303.fn(_t1303.env, NULL, 0);
        }
#line 861 "compiler/main.gem"
    GemVal _t1304[] = {gem_string("end")};
    GemVal _t1305 = (*gem_v_expect);
        (void)(_t1305.fn(_t1305.env, _t1304, 1));
#line 862 "compiler/main.gem"
    GemVal _t1306[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line, gem_v_match_var};
        GemVal _t1307 = gem_fn_make_match(NULL, _t1306, 5);
        gem_pop_frame();
        return _t1307;
    }
#line 866 "compiler/main.gem"
    GemVal _t1308 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1308, gem_string("type")), gem_string("return")))) {
#line 867 "compiler/main.gem"
    GemVal _t1309 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1309, gem_string("line"));
#line 868 "compiler/main.gem"
    GemVal _t1310 = (*gem_v_advance);
        (void)(_t1310.fn(_t1310.env, NULL, 0));
#line 869 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 870 "compiler/main.gem"
    GemVal _t1311 = (*gem_v_peek);
    GemVal _t1312 = _t1311.fn(_t1311.env, NULL, 0);
    GemVal _t1315;
    if (!gem_truthy(gem_neq(gem_table_get(_t1312, gem_string("type")), gem_string("NEWLINE")))) {
        _t1315 = gem_neq(gem_table_get(_t1312, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t1313 = (*gem_v_peek);
        GemVal _t1314 = _t1313.fn(_t1313.env, NULL, 0);
        _t1315 = gem_neq(gem_table_get(_t1314, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t1318;
    if (!gem_truthy(_t1315)) {
        _t1318 = _t1315;
    } else {
        GemVal _t1316 = (*gem_v_peek);
        GemVal _t1317 = _t1316.fn(_t1316.env, NULL, 0);
        _t1318 = gem_neq(gem_table_get(_t1317, gem_string("type")), gem_string("end"));
    }
    GemVal _t1321;
    if (!gem_truthy(_t1318)) {
        _t1321 = _t1318;
    } else {
        GemVal _t1319 = (*gem_v_peek);
        GemVal _t1320 = _t1319.fn(_t1319.env, NULL, 0);
        _t1321 = gem_neq(gem_table_get(_t1320, gem_string("type")), gem_string("elif"));
    }
    GemVal _t1324;
    if (!gem_truthy(_t1321)) {
        _t1324 = _t1321;
    } else {
        GemVal _t1322 = (*gem_v_peek);
        GemVal _t1323 = _t1322.fn(_t1322.env, NULL, 0);
        _t1324 = gem_neq(gem_table_get(_t1323, gem_string("type")), gem_string("else"));
    }
    GemVal _t1327;
    if (!gem_truthy(_t1324)) {
        _t1327 = _t1324;
    } else {
        GemVal _t1325 = (*gem_v_peek);
        GemVal _t1326 = _t1325.fn(_t1325.env, NULL, 0);
        _t1327 = gem_neq(gem_table_get(_t1326, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t1327)) {
#line 871 "compiler/main.gem"
    GemVal _t1328 = (*gem_v_parse_expr);
            gem_v_value = _t1328.fn(_t1328.env, NULL, 0);
        }
#line 873 "compiler/main.gem"
    GemVal _t1329[] = {gem_v_value, gem_v_line};
        GemVal _t1330 = gem_fn_make_return(NULL, _t1329, 2);
        gem_pop_frame();
        return _t1330;
    }
#line 877 "compiler/main.gem"
    GemVal _t1331 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1331, gem_string("type")), gem_string("break")))) {
#line 878 "compiler/main.gem"
    GemVal _t1332 = (*gem_v_advance);
        (void)(_t1332.fn(_t1332.env, NULL, 0));
#line 879 "compiler/main.gem"
        GemVal _t1333 = gem_fn_make_break(NULL, NULL, 0);
        gem_pop_frame();
        return _t1333;
    }
#line 883 "compiler/main.gem"
    GemVal _t1334 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1334, gem_string("type")), gem_string("continue")))) {
#line 884 "compiler/main.gem"
    GemVal _t1335 = (*gem_v_advance);
        (void)(_t1335.fn(_t1335.env, NULL, 0));
#line 885 "compiler/main.gem"
        GemVal _t1336 = gem_fn_make_continue(NULL, NULL, 0);
        gem_pop_frame();
        return _t1336;
    }
#line 889 "compiler/main.gem"
    GemVal _t1337 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1337, gem_string("type")), gem_string("load")))) {
#line 890 "compiler/main.gem"
    GemVal _t1338 = (*gem_v_advance);
        (void)(_t1338.fn(_t1338.env, NULL, 0));
#line 891 "compiler/main.gem"
    GemVal _t1339[] = {gem_string("STRING")};
    GemVal _t1340 = (*gem_v_expect);
    GemVal _t1341 = _t1340.fn(_t1340.env, _t1339, 1);
        GemVal gem_v_path = gem_table_get(_t1341, gem_string("value"));
#line 892 "compiler/main.gem"
    GemVal _t1342[] = {gem_v_path};
        GemVal _t1343 = gem_fn_make_load(NULL, _t1342, 1);
        gem_pop_frame();
        return _t1343;
    }
#line 896 "compiler/main.gem"
    GemVal _t1344 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1344, gem_string("type")), gem_string("extern")))) {
#line 897 "compiler/main.gem"
    GemVal _t1345 = (*gem_v_advance);
        (void)(_t1345.fn(_t1345.env, NULL, 0));
#line 898 "compiler/main.gem"
    GemVal _t1346 = (*gem_v_peek);
    GemVal _t1347 = _t1346.fn(_t1346.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1347, gem_string("type")), gem_string("fn")))) {
#line 899 "compiler/main.gem"
    GemVal _t1348 = (*gem_v_advance);
            (void)(_t1348.fn(_t1348.env, NULL, 0));
#line 900 "compiler/main.gem"
    GemVal _t1349[] = {gem_string("NAME")};
    GemVal _t1350 = (*gem_v_expect);
    GemVal _t1351 = _t1350.fn(_t1350.env, _t1349, 1);
            GemVal gem_v_name = gem_table_get(_t1351, gem_string("value"));
#line 901 "compiler/main.gem"
    GemVal _t1352[] = {gem_string("(")};
    GemVal _t1353 = (*gem_v_expect);
            (void)(_t1353.fn(_t1353.env, _t1352, 1));
#line 902 "compiler/main.gem"
    GemVal _t1354 = gem_table_new();
            GemVal gem_v_eparams = _t1354;
#line 903 "compiler/main.gem"
    GemVal _t1355 = (*gem_v_peek);
    GemVal _t1356 = _t1355.fn(_t1355.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t1356, gem_string("type")), gem_string(")")))) {
#line 904 "compiler/main.gem"
    GemVal _t1357[] = {gem_string("NAME")};
    GemVal _t1358 = (*gem_v_expect);
    GemVal _t1359 = _t1358.fn(_t1358.env, _t1357, 1);
                GemVal gem_v_pname = gem_table_get(_t1359, gem_string("value"));
#line 905 "compiler/main.gem"
    GemVal _t1360[] = {gem_string(":")};
    GemVal _t1361 = (*gem_v_expect);
                (void)(_t1361.fn(_t1361.env, _t1360, 1));
#line 906 "compiler/main.gem"
    GemVal _t1362[] = {gem_string("NAME")};
    GemVal _t1363 = (*gem_v_expect);
    GemVal _t1364 = _t1363.fn(_t1363.env, _t1362, 1);
                GemVal gem_v_ptype = gem_table_get(_t1364, gem_string("value"));
#line 907 "compiler/main.gem"
    GemVal _t1365[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1366[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1365, 2)};
                (void)(gem_push_fn(NULL, _t1366, 2));
#line 908 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1367 = (*gem_v_peek);
                    GemVal _t1368 = _t1367.fn(_t1367.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1368, gem_string("type")), gem_string(",")))) break;
#line 909 "compiler/main.gem"
    GemVal _t1369 = (*gem_v_advance);
                    (void)(_t1369.fn(_t1369.env, NULL, 0));
#line 910 "compiler/main.gem"
    GemVal _t1370[] = {gem_string("NAME")};
    GemVal _t1371 = (*gem_v_expect);
    GemVal _t1372 = _t1371.fn(_t1371.env, _t1370, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t1372, gem_string("value"));
#line 911 "compiler/main.gem"
    GemVal _t1373[] = {gem_string(":")};
    GemVal _t1374 = (*gem_v_expect);
                    (void)(_t1374.fn(_t1374.env, _t1373, 1));
#line 912 "compiler/main.gem"
    GemVal _t1375[] = {gem_string("NAME")};
    GemVal _t1376 = (*gem_v_expect);
    GemVal _t1377 = _t1376.fn(_t1376.env, _t1375, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t1377, gem_string("value"));
#line 913 "compiler/main.gem"
    GemVal _t1378[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1379[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1378, 2)};
                    (void)(gem_push_fn(NULL, _t1379, 2));
                }
            }
#line 916 "compiler/main.gem"
    GemVal _t1380[] = {gem_string(")")};
    GemVal _t1381 = (*gem_v_expect);
            (void)(_t1381.fn(_t1381.env, _t1380, 1));
#line 917 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 918 "compiler/main.gem"
    GemVal _t1382 = (*gem_v_peek);
    GemVal _t1383 = _t1382.fn(_t1382.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1383, gem_string("type")), gem_string("->")))) {
#line 919 "compiler/main.gem"
    GemVal _t1384 = (*gem_v_advance);
                (void)(_t1384.fn(_t1384.env, NULL, 0));
#line 920 "compiler/main.gem"
    GemVal _t1385[] = {gem_string("NAME")};
    GemVal _t1386 = (*gem_v_expect);
    GemVal _t1387 = _t1386.fn(_t1386.env, _t1385, 1);
                gem_v_ret_type = gem_table_get(_t1387, gem_string("value"));
            }
#line 922 "compiler/main.gem"
    GemVal _t1388[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            GemVal _t1389 = gem_fn_make_extern_fn(NULL, _t1388, 3);
            gem_pop_frame();
            return _t1389;
        } else {
#line 923 "compiler/main.gem"
    GemVal _t1390 = (*gem_v_peek);
    GemVal _t1391 = _t1390.fn(_t1390.env, NULL, 0);
    GemVal _t1394;
    if (!gem_truthy(gem_eq(gem_table_get(_t1391, gem_string("type")), gem_string("NAME")))) {
        _t1394 = gem_eq(gem_table_get(_t1391, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1392 = (*gem_v_peek);
        GemVal _t1393 = _t1392.fn(_t1392.env, NULL, 0);
        _t1394 = gem_eq(gem_table_get(_t1393, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t1394)) {
#line 924 "compiler/main.gem"
    GemVal _t1395 = (*gem_v_advance);
                (void)(_t1395.fn(_t1395.env, NULL, 0));
#line 925 "compiler/main.gem"
    GemVal _t1396[] = {gem_string("STRING")};
    GemVal _t1397 = (*gem_v_expect);
    GemVal _t1398 = _t1397.fn(_t1397.env, _t1396, 1);
                GemVal gem_v_path = gem_table_get(_t1398, gem_string("value"));
#line 926 "compiler/main.gem"
    GemVal _t1399[] = {gem_v_path};
                GemVal _t1400 = gem_fn_make_extern_include(NULL, _t1399, 1);
                gem_pop_frame();
                return _t1400;
            } else {
#line 928 "compiler/main.gem"
    GemVal _t1401 = (*gem_v_peek);
    GemVal _t1402 = _t1401.fn(_t1401.env, NULL, 0);
    GemVal _t1403 = (*gem_v_peek);
    GemVal _t1404 = _t1403.fn(_t1403.env, NULL, 0);
    GemVal _t1405 = (*gem_v_peek);
    GemVal _t1406 = _t1405.fn(_t1405.env, NULL, 0);
    GemVal _t1407[] = {gem_table_get(_t1406, gem_string("value"))};
    GemVal _t1408[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1402, gem_string("line")), gem_table_get(_t1404, gem_string("col")), gem_len_fn(NULL, _t1407, 1), gem_string("expected 'fn' or 'include' after 'extern'"), GEM_NIL};
                (void)(gem_fn_compile_error(NULL, _t1408, 7));
            }
        }
    }
#line 933 "compiler/main.gem"
    GemVal _t1409 = gem_v_t;
    GemVal _t1411;
    if (!gem_truthy(gem_eq(gem_table_get(_t1409, gem_string("type")), gem_string("NAME")))) {
        _t1411 = gem_eq(gem_table_get(_t1409, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1410 = gem_v_t;
        _t1411 = gem_eq(gem_table_get(_t1410, gem_string("value")), gem_string("receive"));
    }
    if (gem_truthy(_t1411)) {
#line 934 "compiler/main.gem"
        GemVal gem_v_rla = gem_int(1);
#line 935 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1412[] = {gem_v_rla};
            GemVal _t1413 = (*gem_v_peek_at);
            GemVal _t1414 = _t1413.fn(_t1413.env, _t1412, 1);
            if (!gem_truthy(gem_eq(gem_table_get(_t1414, gem_string("type")), gem_string("NEWLINE")))) break;
#line 936 "compiler/main.gem"
            gem_v_rla = gem_add(gem_v_rla, gem_int(1));
        }
#line 938 "compiler/main.gem"
    GemVal _t1415[] = {gem_v_rla};
    GemVal _t1416 = (*gem_v_peek_at);
    GemVal _t1417 = _t1416.fn(_t1416.env, _t1415, 1);
        if (gem_truthy(gem_eq(gem_table_get(_t1417, gem_string("type")), gem_string("when")))) {
#line 939 "compiler/main.gem"
    GemVal _t1418 = gem_v_t;
            GemVal gem_v_rline = gem_table_get(_t1418, gem_string("line"));
#line 940 "compiler/main.gem"
    GemVal _t1419 = (*gem_v_advance);
            (void)(_t1419.fn(_t1419.env, NULL, 0));
#line 941 "compiler/main.gem"
    GemVal _t1420 = (*gem_v_skip_nl);
            (void)(_t1420.fn(_t1420.env, NULL, 0));
#line 942 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 943 "compiler/main.gem"
    GemVal _t1421[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_recv_var = gem_add(gem_string("_recv_"), gem_to_string_fn(NULL, _t1421, 1));
#line 944 "compiler/main.gem"
    GemVal _t1422[] = {gem_v_recv_var};
            GemVal gem_v_recv_var_expr = gem_fn_make_var(NULL, _t1422, 1);
#line 945 "compiler/main.gem"
    GemVal _t1423 = gem_table_new();
            GemVal gem_v_rarms = _t1423;
#line 946 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1424 = (*gem_v_peek);
                GemVal _t1425 = _t1424.fn(_t1424.env, NULL, 0);
                if (!gem_truthy(gem_eq(gem_table_get(_t1425, gem_string("type")), gem_string("when")))) break;
#line 947 "compiler/main.gem"
    GemVal _t1426 = (*gem_v_advance);
                (void)(_t1426.fn(_t1426.env, NULL, 0));
#line 948 "compiler/main.gem"
    GemVal _t1427 = (*gem_v_peek);
    GemVal _t1428 = _t1427.fn(_t1427.env, NULL, 0);
                GemVal gem_v_rpt = gem_table_get(_t1428, gem_string("type"));
#line 949 "compiler/main.gem"
    GemVal _t1429[] = {gem_int(1)};
    GemVal _t1430 = (*gem_v_peek_at);
    GemVal _t1431 = _t1430.fn(_t1430.env, _t1429, 1);
                GemVal gem_v_rnext = gem_table_get(_t1431, gem_string("type"));
#line 950 "compiler/main.gem"
    GemVal _t1432;
    if (gem_truthy(gem_eq(gem_v_rpt, gem_string("{")))) {
        _t1432 = gem_eq(gem_v_rpt, gem_string("{"));
    } else {
        _t1432 = gem_eq(gem_v_rpt, gem_string("["));
    }
                if (gem_truthy(_t1432)) {
#line 951 "compiler/main.gem"
    GemVal _t1433[] = {gem_v_recv_var_expr};
    GemVal _t1434 = (*gem_v_parse_pattern);
                    GemVal gem_v_rpat = _t1434.fn(_t1434.env, _t1433, 1);
#line 952 "compiler/main.gem"
    GemVal _t1435 = (*gem_v_parse_body);
                    GemVal gem_v_rwbody = _t1435.fn(_t1435.env, NULL, 0);
#line 953 "compiler/main.gem"
    GemVal _t1436 = gem_table_new();
    gem_table_set(_t1436, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1436, gem_string("body"), gem_v_rwbody);
    GemVal _t1437[] = {gem_v_rarms, _t1436};
                    (void)(gem_push_fn(NULL, _t1437, 2));
                } else {
#line 954 "compiler/main.gem"
    GemVal _t1443;
    if (!gem_truthy(gem_eq(gem_v_rpt, gem_string("NAME")))) {
        _t1443 = gem_eq(gem_v_rpt, gem_string("NAME"));
    } else {
        GemVal _t1438;
        if (gem_truthy(gem_eq(gem_v_rnext, gem_string("NEWLINE")))) {
                _t1438 = gem_eq(gem_v_rnext, gem_string("NEWLINE"));
        } else {
                _t1438 = gem_eq(gem_v_rnext, gem_string("EOF"));
        }
        GemVal _t1439;
        if (gem_truthy(_t1438)) {
                _t1439 = _t1438;
        } else {
                _t1439 = gem_eq(gem_v_rnext, gem_string("end"));
        }
        GemVal _t1440;
        if (gem_truthy(_t1439)) {
                _t1440 = _t1439;
        } else {
                _t1440 = gem_eq(gem_v_rnext, gem_string("else"));
        }
        GemVal _t1441;
        if (gem_truthy(_t1440)) {
                _t1441 = _t1440;
        } else {
                _t1441 = gem_eq(gem_v_rnext, gem_string("when"));
        }
        GemVal _t1442;
        if (gem_truthy(_t1441)) {
                _t1442 = _t1441;
        } else {
                _t1442 = gem_eq(gem_v_rnext, gem_string("after"));
        }
        _t1443 = _t1442;
    }
                    if (gem_truthy(_t1443)) {
#line 955 "compiler/main.gem"
    GemVal _t1444[] = {gem_v_recv_var_expr};
    GemVal _t1445 = (*gem_v_parse_pattern);
                        GemVal gem_v_rpat = _t1445.fn(_t1445.env, _t1444, 1);
#line 956 "compiler/main.gem"
    GemVal _t1446 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1446.fn(_t1446.env, NULL, 0);
#line 957 "compiler/main.gem"
    GemVal _t1447 = gem_table_new();
    gem_table_set(_t1447, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1447, gem_string("body"), gem_v_rwbody);
    GemVal _t1448[] = {gem_v_rarms, _t1447};
                        (void)(gem_push_fn(NULL, _t1448, 2));
                    } else {
#line 959 "compiler/main.gem"
    GemVal _t1449 = (*gem_v_parse_expr);
                        GemVal gem_v_rwval = _t1449.fn(_t1449.env, NULL, 0);
#line 960 "compiler/main.gem"
    GemVal _t1450 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1450.fn(_t1450.env, NULL, 0);
#line 961 "compiler/main.gem"
    GemVal _t1451 = gem_table_new();
    GemVal _t1452 = gem_table_new();
    GemVal _t1453[] = {gem_string("=="), gem_v_recv_var_expr, gem_v_rwval};
    gem_table_set(_t1452, gem_string("condition"), gem_fn_make_binop(NULL, _t1453, 3));
    GemVal _t1454 = gem_table_new();
    gem_table_set(_t1452, gem_string("bindings"), _t1454);
    gem_table_set(_t1451, gem_string("pattern"), _t1452);
    gem_table_set(_t1451, gem_string("body"), gem_v_rwbody);
    GemVal _t1455[] = {gem_v_rarms, _t1451};
                        (void)(gem_push_fn(NULL, _t1455, 2));
                    }
                }
#line 963 "compiler/main.gem"
    GemVal _t1456 = (*gem_v_skip_nl);
                (void)(_t1456.fn(_t1456.env, NULL, 0));
            }
#line 965 "compiler/main.gem"
            GemVal gem_v_rafter_ms = GEM_NIL;
#line 966 "compiler/main.gem"
            GemVal gem_v_rafter_body = GEM_NIL;
#line 967 "compiler/main.gem"
    GemVal _t1457 = (*gem_v_peek);
    GemVal _t1458 = _t1457.fn(_t1457.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1458, gem_string("type")), gem_string("after")))) {
#line 968 "compiler/main.gem"
    GemVal _t1459 = (*gem_v_advance);
                (void)(_t1459.fn(_t1459.env, NULL, 0));
#line 969 "compiler/main.gem"
    GemVal _t1460 = (*gem_v_parse_expr);
                gem_v_rafter_ms = _t1460.fn(_t1460.env, NULL, 0);
#line 970 "compiler/main.gem"
    GemVal _t1461 = (*gem_v_parse_body);
                gem_v_rafter_body = _t1461.fn(_t1461.env, NULL, 0);
            }
#line 972 "compiler/main.gem"
    GemVal _t1462[] = {gem_string("end")};
    GemVal _t1463 = (*gem_v_expect);
            (void)(_t1463.fn(_t1463.env, _t1462, 1));
#line 973 "compiler/main.gem"
    GemVal _t1464[] = {gem_v_rarms, gem_v_rafter_ms, gem_v_rafter_body, gem_v_rline, gem_v_recv_var};
            GemVal _t1465 = gem_fn_make_receive_match(NULL, _t1464, 5);
            gem_pop_frame();
            return _t1465;
        }
    }
#line 978 "compiler/main.gem"
    GemVal _t1466 = (*gem_v_parse_expr);
    GemVal _t1467 = _t1466.fn(_t1466.env, NULL, 0);
    gem_pop_frame();
    return _t1467;
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
#line 987 "compiler/main.gem"
    GemVal _t1469 = gem_table_new();
    GemVal gem_v_stmts = _t1469;
#line 988 "compiler/main.gem"
    GemVal _t1470 = (*gem_v_skip_nl);
    (void)(_t1470.fn(_t1470.env, NULL, 0));
#line 989 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1471 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1471.fn(_t1471.env, NULL, 0)))) break;
#line 990 "compiler/main.gem"
    GemVal _t1472 = (*gem_v_parse_stmt);
    GemVal _t1473[] = {gem_v_stmts, _t1472.fn(_t1472.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1473, 2));
#line 991 "compiler/main.gem"
    GemVal _t1474 = (*gem_v_skip_nl);
        (void)(_t1474.fn(_t1474.env, NULL, 0));
    }
#line 993 "compiler/main.gem"
    GemVal _t1475[] = {gem_v_stmts};
    GemVal _t1476 = gem_fn_make_program(NULL, _t1475, 1);
    gem_pop_frame();
    return _t1476;
}

static GemVal gem_fn_make_parser(void *_env, GemVal *args, int argc) {
#line 19 "compiler/main.gem"
    GemVal *gem_v_tokens = GC_MALLOC(sizeof(GemVal));
    *gem_v_tokens = (argc > 0) ? args[0] : GEM_NIL;
    GemVal *gem_v_source = GC_MALLOC(sizeof(GemVal));
    *gem_v_source = (argc > 1) ? args[1] : GEM_NIL;
    GemVal *gem_v_file = GC_MALLOC(sizeof(GemVal));
    *gem_v_file = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_parser", "compiler/main.gem", 19);
#line 20 "compiler/main.gem"
    GemVal *gem_v_pos = GC_MALLOC(sizeof(GemVal));
    *gem_v_pos = gem_int(0);
#line 21 "compiler/main.gem"
    GemVal *gem_v_gensym_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_gensym_counter = gem_int(0);
#line 22 "compiler/main.gem"
    GemVal *gem_v_fn_depth = GC_MALLOC(sizeof(GemVal));
    *gem_v_fn_depth = gem_int(0);
#line 26 "compiler/main.gem"
    struct _closure__anon_1 *_t347 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t347->gem_v_pos = gem_v_pos;
    _t347->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t347);
#line 30 "compiler/main.gem"
    struct _closure__anon_2 *_t349 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t349->gem_v_pos = gem_v_pos;
    _t349->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t349);
#line 34 "compiler/main.gem"
    struct _closure__anon_3 *_t351 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t351->gem_v_pos = gem_v_pos;
    _t351->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t351);
#line 40 "compiler/main.gem"
    struct _closure__anon_4 *_t354 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t354->gem_v_pos = gem_v_pos;
    _t354->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t354);
#line 44 "compiler/main.gem"
    struct _closure__anon_5 *_t365 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t365->gem_v_file = gem_v_file;
    _t365->gem_v_pos = gem_v_pos;
    _t365->gem_v_source = gem_v_source;
    _t365->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t365);
#line 53 "compiler/main.gem"
    struct _closure__anon_6 *_t367 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t367->gem_v_pos = gem_v_pos;
    _t367->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t367);
#line 65 "compiler/main.gem"
    struct _closure__anon_7 *_t413 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t413->gem_v_advance = gem_v_advance;
    _t413->gem_v_expect = gem_v_expect;
    _t413->gem_v_file = gem_v_file;
    _t413->gem_v_peek = gem_v_peek;
    _t413->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t413);
#line 98 "compiler/main.gem"
    GemVal *gem_v_parse_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_expr = GEM_NIL;
#line 99 "compiler/main.gem"
    GemVal *gem_v_parse_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_stmt = GEM_NIL;
#line 100 "compiler/main.gem"
    GemVal *gem_v_parse_unary = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_unary = GEM_NIL;
#line 101 "compiler/main.gem"
    GemVal *gem_v_parse_not = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_not = GEM_NIL;
#line 102 "compiler/main.gem"
    GemVal *gem_v_parse_pattern = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_pattern = GEM_NIL;
#line 108 "compiler/main.gem"
    struct _closure__anon_8 *_t436 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t436->gem_v_at_end = gem_v_at_end;
    _t436->gem_v_parse_stmt = gem_v_parse_stmt;
    _t436->gem_v_peek = gem_v_peek;
    _t436->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t436);
#line 121 "compiler/main.gem"
    struct _closure__anon_9 *_t447 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t447->gem_v_at_end = gem_v_at_end;
    _t447->gem_v_parse_stmt = gem_v_parse_stmt;
    _t447->gem_v_peek = gem_v_peek;
    _t447->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t447);
#line 134 "compiler/main.gem"
    struct _closure__anon_10 *_t585 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t585->gem_v_advance = gem_v_advance;
    _t585->gem_v_at_end = gem_v_at_end;
    _t585->gem_v_expect = gem_v_expect;
    _t585->gem_v_file = gem_v_file;
    _t585->gem_v_fn_depth = gem_v_fn_depth;
    _t585->gem_v_parse_expr = gem_v_parse_expr;
    _t585->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t585->gem_v_parse_params = gem_v_parse_params;
    _t585->gem_v_peek = gem_v_peek;
    _t585->gem_v_peek_at = gem_v_peek_at;
    _t585->gem_v_skip_nl = gem_v_skip_nl;
    _t585->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t585);
#line 265 "compiler/main.gem"
    struct _closure__anon_11 *_t676 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t676->gem_v_advance = gem_v_advance;
    _t676->gem_v_expect = gem_v_expect;
    _t676->gem_v_fn_depth = gem_v_fn_depth;
    _t676->gem_v_parse_atom = gem_v_parse_atom;
    _t676->gem_v_parse_expr = gem_v_parse_expr;
    _t676->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t676->gem_v_parse_params = gem_v_parse_params;
    _t676->gem_v_peek = gem_v_peek;
    _t676->gem_v_peek_at = gem_v_peek_at;
    _t676->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t676);
#line 371 "compiler/main.gem"
    struct _closure__anon_12 *_t685 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t685->gem_v_advance = gem_v_advance;
    _t685->gem_v_parse_call = gem_v_parse_call;
    _t685->gem_v_parse_unary = gem_v_parse_unary;
    _t685->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t685);
#line 380 "compiler/main.gem"
    struct _closure__anon_13 *_t700 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t700->gem_v_advance = gem_v_advance;
    _t700->gem_v_parse_unary = gem_v_parse_unary;
    _t700->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t700);
#line 391 "compiler/main.gem"
    struct _closure__anon_14 *_t712 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t712->gem_v_advance = gem_v_advance;
    _t712->gem_v_parse_mul = gem_v_parse_mul;
    _t712->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t712);
#line 402 "compiler/main.gem"
    struct _closure__anon_15 *_t739 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t739->gem_v_advance = gem_v_advance;
    _t739->gem_v_parse_add = gem_v_parse_add;
    _t739->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t739);
#line 413 "compiler/main.gem"
    struct _closure__anon_16 *_t748 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t748->gem_v_advance = gem_v_advance;
    _t748->gem_v_parse_compare = gem_v_parse_compare;
    _t748->gem_v_parse_not = gem_v_parse_not;
    _t748->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t748);
#line 422 "compiler/main.gem"
    struct _closure__anon_17 *_t756 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t756->gem_v_advance = gem_v_advance;
    _t756->gem_v_parse_not = gem_v_parse_not;
    _t756->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t756);
#line 433 "compiler/main.gem"
    struct _closure__anon_18 *_t764 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t764->gem_v_advance = gem_v_advance;
    _t764->gem_v_parse_and = gem_v_parse_and;
    _t764->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t764);
#line 444 "compiler/main.gem"
    struct _closure__anon_19 *_t814 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t814->gem_v_advance = gem_v_advance;
    _t814->gem_v_file = gem_v_file;
    _t814->gem_v_parse_expr = gem_v_parse_expr;
    _t814->gem_v_parse_or = gem_v_parse_or;
    _t814->gem_v_peek = gem_v_peek;
    _t814->gem_v_source = gem_v_source;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t814);
#line 479 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 483 "compiler/main.gem"
    struct _closure__anon_20 *_t986 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t986->gem_v_advance = gem_v_advance;
    _t986->gem_v_expect = gem_v_expect;
    _t986->gem_v_file = gem_v_file;
    _t986->gem_v_parse_pattern = gem_v_parse_pattern;
    _t986->gem_v_peek = gem_v_peek;
    _t986->gem_v_peek_at = gem_v_peek_at;
    _t986->gem_v_skip_nl = gem_v_skip_nl;
    _t986->gem_v_source = gem_v_source;
    *gem_v_parse_pattern = gem_make_fn(_anon_20, _t986);
#line 611 "compiler/main.gem"
    struct _closure__anon_22 *_t1468 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1468->gem_v_advance = gem_v_advance;
    _t1468->gem_v_expect = gem_v_expect;
    _t1468->gem_v_file = gem_v_file;
    _t1468->gem_v_fn_depth = gem_v_fn_depth;
    _t1468->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1468->gem_v_parse_body = gem_v_parse_body;
    _t1468->gem_v_parse_expr = gem_v_parse_expr;
    _t1468->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1468->gem_v_parse_params = gem_v_parse_params;
    _t1468->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1468->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1468->gem_v_peek = gem_v_peek;
    _t1468->gem_v_peek_at = gem_v_peek_at;
    _t1468->gem_v_skip_nl = gem_v_skip_nl;
    _t1468->gem_v_source = gem_v_source;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_22, _t1468);
#line 982 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 986 "compiler/main.gem"
    struct _closure__anon_23 *_t1477 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1477->gem_v_at_end = gem_v_at_end;
    _t1477->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1477->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_23, _t1477);
    GemVal _t1478 = gem_table_new();
    gem_table_set(_t1478, gem_string("parse"), gem_v_parse);
    GemVal _t1479 = _t1478;
    gem_pop_frame();
    return _t1479;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("escape_c_string", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal _t1480[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1480, 1);
#line 11 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 12 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 12 "compiler/main.gem"
    GemVal _t1481[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1481, 1);
#line 12 "compiler/main.gem"
    while (1) {
        gem_yield_check();
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
    GemVal _t1482[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1482, 1), gem_int(0)))) {
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

    GemVal _t1483 = gem_v_result;
    gem_pop_frame();
    return _t1483;
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
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 35 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_2;
#line 35 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 36 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_v_s);
    }

    GemVal _t1484 = gem_v_result;
    gem_pop_frame();
    return _t1484;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 41 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("pad_for", "compiler/main.gem", 41);
#line 42 "compiler/main.gem"
    GemVal _t1485[] = {gem_string("    "), gem_v_indent};
    GemVal _t1486 = gem_fn_repeat_str(NULL, _t1485, 2);
    gem_pop_frame();
    return _t1486;
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 45 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("format_float", "compiler/main.gem", 45);
#line 46 "compiler/main.gem"
    GemVal _t1487[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1487, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 49 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 49 "compiler/main.gem"
    GemVal _t1488[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1488, 1);
#line 49 "compiler/main.gem"
    while (1) {
        gem_yield_check();
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
    GemVal _t1489 = gem_v_s;
    gem_pop_frame();
    return _t1489;
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
    GemVal _t1490 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1490;
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 72 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_union", "compiler/main.gem", 72);
#line 73 "compiler/main.gem"
    GemVal _t1491 = gem_table_new();
    GemVal gem_v_result = _t1491;
#line 74 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 74 "compiler/main.gem"
    GemVal _t1492[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_keys_4 = gem_keys_fn(NULL, _t1492, 1);
#line 74 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 74 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1493[] = {gem_v__for_keys_4};
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_len_fn(NULL, _t1493, 1)))) break;
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
    GemVal _t1494[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_keys_5 = gem_keys_fn(NULL, _t1494, 1);
#line 77 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 77 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1495[] = {gem_v__for_keys_5};
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1495, 1)))) break;
#line 77 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_5, gem_v__for_i_5);
#line 77 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_5, gem_table_get(gem_v__for_keys_5, gem_v__for_i_5));
#line 77 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 78 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1496 = gem_v_result;
    gem_pop_frame();
    return _t1496;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 83 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_intersect", "compiler/main.gem", 83);
#line 84 "compiler/main.gem"
    GemVal _t1497 = gem_table_new();
    GemVal gem_v_result = _t1497;
#line 85 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 85 "compiler/main.gem"
    GemVal _t1498[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_keys_6 = gem_keys_fn(NULL, _t1498, 1);
#line 85 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 85 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1499[] = {gem_v__for_keys_6};
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1499, 1)))) break;
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

    GemVal _t1500 = gem_v_result;
    gem_pop_frame();
    return _t1500;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("sorted_array_to_set", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t1501 = gem_table_new();
    GemVal gem_v_s = _t1501;
#line 95 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 95 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 95 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1502[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1502, 1)))) break;
#line 95 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 95 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 96 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1503 = gem_v_s;
    gem_pop_frame();
    return _t1503;
}

static GemVal gem_fn_collect_block_lets(void *_env, GemVal *args, int argc) {
#line 103 "compiler/main.gem"
    GemVal gem_v_stmt = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("collect_block_lets", "compiler/main.gem", 103);
#line 104 "compiler/main.gem"
    GemVal _t1504[] = {gem_v_stmt};
    GemVal _t1506;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1504, 1), gem_string("table")))) {
        _t1506 = gem_eq(gem_type_fn(NULL, _t1504, 1), gem_string("table"));
    } else {
        GemVal _t1505 = gem_v_stmt;
        _t1506 = gem_eq(gem_table_get(_t1505, gem_string("tag")), gem_string("block"));
    }
    if (gem_truthy(_t1506)) {
        {
#line 105 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 105 "compiler/main.gem"
    GemVal _t1507 = gem_v_stmt;
    GemVal _t1508[] = {gem_table_get(_t1507, gem_string("stmts"))};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1508, 1);
#line 105 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 105 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_8;
#line 105 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 106 "compiler/main.gem"
    GemVal _t1509 = gem_v_stmt;
    GemVal _t1510[] = {gem_table_get(gem_table_get(_t1509, gem_string("stmts")), gem_v_j)};
    GemVal _t1513;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1510, 1), gem_string("table")))) {
        _t1513 = gem_eq(gem_type_fn(NULL, _t1510, 1), gem_string("table"));
    } else {
        GemVal _t1511 = gem_v_stmt;
        GemVal _t1512 = gem_table_get(gem_table_get(_t1511, gem_string("stmts")), gem_v_j);
        _t1513 = gem_eq(gem_table_get(_t1512, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1513)) {
#line 107 "compiler/main.gem"
    GemVal _t1514 = gem_v_stmt;
    GemVal _t1515 = gem_table_get(gem_table_get(_t1514, gem_string("stmts")), gem_v_j);
    GemVal _t1516[] = {gem_v_result, gem_table_get(_t1515, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1516, 2));
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
#line 113 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("collect_top_let_names", "compiler/main.gem", 113);
    {
#line 114 "compiler/main.gem"
        GemVal gem_v__for_i_9 = gem_int(0);
#line 114 "compiler/main.gem"
    GemVal _t1517[] = {gem_v_stmts};
        GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1517, 1);
#line 114 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 114 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_9;
#line 114 "compiler/main.gem"
            gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 115 "compiler/main.gem"
            GemVal gem_v_s = gem_table_get(gem_v_stmts, gem_v_i);
#line 116 "compiler/main.gem"
    GemVal _t1518[] = {gem_v_s};
    GemVal _t1520;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1518, 1), gem_string("table")))) {
        _t1520 = gem_eq(gem_type_fn(NULL, _t1518, 1), gem_string("table"));
    } else {
        GemVal _t1519 = gem_v_s;
        _t1520 = gem_eq(gem_table_get(_t1519, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1520)) {
#line 117 "compiler/main.gem"
    GemVal _t1521 = gem_v_s;
    GemVal _t1522[] = {gem_v_result, gem_table_get(_t1521, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1522, 2));
            }
#line 119 "compiler/main.gem"
    GemVal _t1523[] = {gem_v_s, gem_v_result};
            (void)(gem_fn_collect_block_lets(NULL, _t1523, 2));
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
#line 202 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t1536[] = {(*gem_v_tmp_counter)};
    GemVal _t1537 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t1536, 1));
    gem_pop_frame();
    return _t1537;
}

struct _closure__anon_25 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_25(void *_env, GemVal *args, int argc) {
    struct _closure__anon_25 *_cls = (struct _closure__anon_25 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_25", "compiler/main.gem", 0);
#line 207 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t1539[] = {(*gem_v_anon_counter)};
    GemVal _t1540 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t1539, 1));
    gem_pop_frame();
    return _t1540;
}

struct _closure__anon_26 {
};
static GemVal _anon_26(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_26", "compiler/main.gem", 0);
    GemVal _t1542[] = {gem_v_name};
    GemVal _t1543 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t1542, 1));
    gem_pop_frame();
    return _t1543;
}

struct _closure__anon_27 {
    GemVal *gem_v_builtins;
    GemVal *gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_local_names;
};
static GemVal _anon_27(void *_env, GemVal *args, int argc) {
    struct _closure__anon_27 *_cls = (struct _closure__anon_27 *)_env;
    GemVal *gem_v_builtins = _cls->gem_v_builtins;
    GemVal *gem_v_collect_free_node = _cls->gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_defined = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_free = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_27", "compiler/main.gem", 0);
#line 221 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 222 "compiler/main.gem"
        GemVal _t1544 = GEM_NIL;
        gem_pop_frame();
        return _t1544;
    }
#line 224 "compiler/main.gem"
    GemVal _t1545[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1545, 1), gem_string("table")))) {
#line 225 "compiler/main.gem"
        GemVal _t1546 = GEM_NIL;
        gem_pop_frame();
        return _t1546;
    }
#line 227 "compiler/main.gem"
    GemVal _t1547 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1547, gem_string("tag")), GEM_NIL))) {
#line 228 "compiler/main.gem"
        GemVal _t1548 = GEM_NIL;
        gem_pop_frame();
        return _t1548;
    }
#line 231 "compiler/main.gem"
    GemVal gem_v__match_11 = gem_v_node;
    GemVal _t1549[] = {gem_v__match_11};
    GemVal _t1551;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1549, 1), gem_string("table")))) {
        _t1551 = gem_eq(gem_type_fn(NULL, _t1549, 1), gem_string("table"));
    } else {
        GemVal _t1550[] = {gem_v__match_11, gem_string("tag")};
        _t1551 = gem_has_key_fn(NULL, _t1550, 2);
    }
    GemVal _t1552;
    if (!gem_truthy(_t1551)) {
        _t1552 = _t1551;
    } else {
        _t1552 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("var"));
    }
    GemVal _t1554;
    if (!gem_truthy(_t1552)) {
        _t1554 = _t1552;
    } else {
        GemVal _t1553[] = {gem_v__match_11, gem_string("name")};
        _t1554 = gem_has_key_fn(NULL, _t1553, 2);
    }
    if (gem_truthy(_t1554)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 233 "compiler/main.gem"
    GemVal _t1555[] = {gem_v_defined, gem_v_name};
    GemVal _t1559;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1555, 2)))) {
        _t1559 = gem_not(gem_fn_set_contains(NULL, _t1555, 2));
    } else {
        GemVal _t1556[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1558;
        if (gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1556, 2)))) {
                _t1558 = gem_not(gem_fn_set_contains(NULL, _t1556, 2));
        } else {
                GemVal _t1557[] = {(*gem_v_local_names), gem_v_name};
                _t1558 = gem_fn_set_contains(NULL, _t1557, 2);
        }
        _t1559 = _t1558;
    }
    GemVal _t1561;
    if (!gem_truthy(_t1559)) {
        _t1561 = _t1559;
    } else {
        GemVal _t1560[] = {(*gem_v_defined_fns), gem_v_name};
        _t1561 = gem_not(gem_fn_set_contains(NULL, _t1560, 2));
    }
        if (gem_truthy(_t1561)) {
#line 234 "compiler/main.gem"
    GemVal _t1562[] = {gem_v_free, gem_v_name};
            GemVal _t1563 = gem_fn_set_add(NULL, _t1562, 2);
            gem_pop_frame();
            return _t1563;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1564[] = {gem_v__match_11};
    GemVal _t1566;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1564, 1), gem_string("table")))) {
        _t1566 = gem_eq(gem_type_fn(NULL, _t1564, 1), gem_string("table"));
    } else {
        GemVal _t1565[] = {gem_v__match_11, gem_string("tag")};
        _t1566 = gem_has_key_fn(NULL, _t1565, 2);
    }
    GemVal _t1567;
    if (!gem_truthy(_t1566)) {
        _t1567 = _t1566;
    } else {
        _t1567 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("let"));
    }
    GemVal _t1569;
    if (!gem_truthy(_t1567)) {
        _t1569 = _t1567;
    } else {
        GemVal _t1568[] = {gem_v__match_11, gem_string("value")};
        _t1569 = gem_has_key_fn(NULL, _t1568, 2);
    }
    if (gem_truthy(_t1569)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 237 "compiler/main.gem"
    GemVal _t1570[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1571 = (*gem_v_collect_free_node);
        GemVal _t1572 = _t1571.fn(_t1571.env, _t1570, 3);
        gem_pop_frame();
        return _t1572;
    } else {
    GemVal _t1573[] = {gem_v__match_11};
    GemVal _t1575;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1573, 1), gem_string("table")))) {
        _t1575 = gem_eq(gem_type_fn(NULL, _t1573, 1), gem_string("table"));
    } else {
        GemVal _t1574[] = {gem_v__match_11, gem_string("tag")};
        _t1575 = gem_has_key_fn(NULL, _t1574, 2);
    }
    GemVal _t1576;
    if (!gem_truthy(_t1575)) {
        _t1576 = _t1575;
    } else {
        _t1576 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t1578;
    if (!gem_truthy(_t1576)) {
        _t1578 = _t1576;
    } else {
        GemVal _t1577[] = {gem_v__match_11, gem_string("name")};
        _t1578 = gem_has_key_fn(NULL, _t1577, 2);
    }
    GemVal _t1580;
    if (!gem_truthy(_t1578)) {
        _t1580 = _t1578;
    } else {
        GemVal _t1579[] = {gem_v__match_11, gem_string("value")};
        _t1580 = gem_has_key_fn(NULL, _t1579, 2);
    }
    if (gem_truthy(_t1580)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 239 "compiler/main.gem"
    GemVal _t1581[] = {gem_v_defined, gem_v_name};
    GemVal _t1585;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1581, 2)))) {
        _t1585 = gem_not(gem_fn_set_contains(NULL, _t1581, 2));
    } else {
        GemVal _t1582[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1584;
        if (gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1582, 2)))) {
                _t1584 = gem_not(gem_fn_set_contains(NULL, _t1582, 2));
        } else {
                GemVal _t1583[] = {(*gem_v_local_names), gem_v_name};
                _t1584 = gem_fn_set_contains(NULL, _t1583, 2);
        }
        _t1585 = _t1584;
    }
    GemVal _t1587;
    if (!gem_truthy(_t1585)) {
        _t1587 = _t1585;
    } else {
        GemVal _t1586[] = {(*gem_v_defined_fns), gem_v_name};
        _t1587 = gem_not(gem_fn_set_contains(NULL, _t1586, 2));
    }
        if (gem_truthy(_t1587)) {
#line 240 "compiler/main.gem"
    GemVal _t1588[] = {gem_v_free, gem_v_name};
            (void)(gem_fn_set_add(NULL, _t1588, 2));
        }
#line 242 "compiler/main.gem"
    GemVal _t1589[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1590 = (*gem_v_collect_free_node);
        GemVal _t1591 = _t1590.fn(_t1590.env, _t1589, 3);
        gem_pop_frame();
        return _t1591;
    } else {
    GemVal _t1592[] = {gem_v__match_11};
    GemVal _t1594;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1592, 1), gem_string("table")))) {
        _t1594 = gem_eq(gem_type_fn(NULL, _t1592, 1), gem_string("table"));
    } else {
        GemVal _t1593[] = {gem_v__match_11, gem_string("tag")};
        _t1594 = gem_has_key_fn(NULL, _t1593, 2);
    }
    GemVal _t1595;
    if (!gem_truthy(_t1594)) {
        _t1595 = _t1594;
    } else {
        _t1595 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1597;
    if (!gem_truthy(_t1595)) {
        _t1597 = _t1595;
    } else {
        GemVal _t1596[] = {gem_v__match_11, gem_string("params")};
        _t1597 = gem_has_key_fn(NULL, _t1596, 2);
    }
    GemVal _t1599;
    if (!gem_truthy(_t1597)) {
        _t1599 = _t1597;
    } else {
        GemVal _t1598[] = {gem_v__match_11, gem_string("rest_param")};
        _t1599 = gem_has_key_fn(NULL, _t1598, 2);
    }
    GemVal _t1601;
    if (!gem_truthy(_t1599)) {
        _t1601 = _t1599;
    } else {
        GemVal _t1600[] = {gem_v__match_11, gem_string("block_param")};
        _t1601 = gem_has_key_fn(NULL, _t1600, 2);
    }
    GemVal _t1603;
    if (!gem_truthy(_t1601)) {
        _t1603 = _t1601;
    } else {
        GemVal _t1602[] = {gem_v__match_11, gem_string("body")};
        _t1603 = gem_has_key_fn(NULL, _t1602, 2);
    }
    if (gem_truthy(_t1603)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_11, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_11, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_11, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 244 "compiler/main.gem"
    GemVal _t1604[] = {gem_v_params};
    GemVal _t1605[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t1604, 1)};
        GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t1605, 2);
#line 245 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 246 "compiler/main.gem"
    GemVal _t1606[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn_set_add(NULL, _t1606, 2));
        }
#line 248 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 249 "compiler/main.gem"
    GemVal _t1607[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn_set_add(NULL, _t1607, 2));
        }
#line 251 "compiler/main.gem"
    GemVal _t1608[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1609 = (*gem_v_collect_free_vars);
        GemVal gem_v_inner_free = _t1609.fn(_t1609.env, _t1608, 2);
        {
#line 252 "compiler/main.gem"
            GemVal gem_v__for_tbl_12 = gem_v_inner_free;
#line 252 "compiler/main.gem"
    GemVal _t1610[] = {gem_v__for_tbl_12};
            GemVal gem_v__for_keys_12 = gem_keys_fn(NULL, _t1610, 1);
#line 252 "compiler/main.gem"
            GemVal gem_v__for_i_12 = gem_int(0);
#line 252 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1611[] = {gem_v__for_keys_12};
                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t1611, 1)))) break;
#line 252 "compiler/main.gem"
                GemVal gem_v_ifk = gem_table_get(gem_v__for_keys_12, gem_v__for_i_12);
#line 252 "compiler/main.gem"
                GemVal gem_v__ = gem_table_get(gem_v__for_tbl_12, gem_table_get(gem_v__for_keys_12, gem_v__for_i_12));
#line 252 "compiler/main.gem"
                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 253 "compiler/main.gem"
    GemVal _t1612[] = {gem_v_free, gem_v_ifk};
                (void)(gem_fn_set_add(NULL, _t1612, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1613[] = {gem_v__match_11};
    GemVal _t1615;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1613, 1), gem_string("table")))) {
        _t1615 = gem_eq(gem_type_fn(NULL, _t1613, 1), gem_string("table"));
    } else {
        GemVal _t1614[] = {gem_v__match_11, gem_string("tag")};
        _t1615 = gem_has_key_fn(NULL, _t1614, 2);
    }
    GemVal _t1616;
    if (!gem_truthy(_t1615)) {
        _t1616 = _t1615;
    } else {
        _t1616 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1616)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1617[] = {gem_v__match_11};
    GemVal _t1619;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1617, 1), gem_string("table")))) {
        _t1619 = gem_eq(gem_type_fn(NULL, _t1617, 1), gem_string("table"));
    } else {
        GemVal _t1618[] = {gem_v__match_11, gem_string("tag")};
        _t1619 = gem_has_key_fn(NULL, _t1618, 2);
    }
    GemVal _t1620;
    if (!gem_truthy(_t1619)) {
        _t1620 = _t1619;
    } else {
        _t1620 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1622;
    if (!gem_truthy(_t1620)) {
        _t1622 = _t1620;
    } else {
        GemVal _t1621[] = {gem_v__match_11, gem_string("cond")};
        _t1622 = gem_has_key_fn(NULL, _t1621, 2);
    }
    GemVal _t1624;
    if (!gem_truthy(_t1622)) {
        _t1624 = _t1622;
    } else {
        GemVal _t1623[] = {gem_v__match_11, gem_string("then")};
        _t1624 = gem_has_key_fn(NULL, _t1623, 2);
    }
    GemVal _t1626;
    if (!gem_truthy(_t1624)) {
        _t1626 = _t1624;
    } else {
        GemVal _t1625[] = {gem_v__match_11, gem_string("else")};
        _t1626 = gem_has_key_fn(NULL, _t1625, 2);
    }
    if (gem_truthy(_t1626)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_11, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 258 "compiler/main.gem"
    GemVal _t1627[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1628 = (*gem_v_collect_free_node);
        (void)(_t1628.fn(_t1628.env, _t1627, 3));
#line 259 "compiler/main.gem"
    GemVal _t1629 = gem_table_new();
    GemVal _t1630[] = {gem_v_defined, _t1629};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1630, 2);
#line 260 "compiler/main.gem"
        GemVal gem_v__for_i_13 = gem_int(0);
#line 260 "compiler/main.gem"
    GemVal _t1631[] = {gem_v_thens};
        GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1631, 1);
#line 260 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 260 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_13;
#line 260 "compiler/main.gem"
            gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 261 "compiler/main.gem"
    GemVal _t1632[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1633 = (*gem_v_collect_free_node);
            (void)(_t1633.fn(_t1633.env, _t1632, 3));
#line 262 "compiler/main.gem"
    GemVal _t1634[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t1636;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1634, 1), gem_string("table")))) {
        _t1636 = gem_eq(gem_type_fn(NULL, _t1634, 1), gem_string("table"));
    } else {
        GemVal _t1635 = gem_table_get(gem_v_thens, gem_v_i);
        _t1636 = gem_eq(gem_table_get(_t1635, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1636)) {
#line 263 "compiler/main.gem"
    GemVal _t1637 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1638[] = {gem_v_d, gem_table_get(_t1637, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1638, 2));
            }
        }

#line 266 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 267 "compiler/main.gem"
    GemVal _t1639 = gem_table_new();
    GemVal _t1640[] = {gem_v_defined, _t1639};
            gem_v_d = gem_fn_set_union(NULL, _t1640, 2);
            {
#line 268 "compiler/main.gem"
                GemVal gem_v__for_i_14 = gem_int(0);
#line 268 "compiler/main.gem"
    GemVal _t1641[] = {gem_v_el};
                GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1641, 1);
#line 268 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 268 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_14;
#line 268 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 269 "compiler/main.gem"
    GemVal _t1642[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1643 = (*gem_v_collect_free_node);
                    (void)(_t1643.fn(_t1643.env, _t1642, 3));
#line 270 "compiler/main.gem"
    GemVal _t1644[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t1646;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1644, 1), gem_string("table")))) {
        _t1646 = gem_eq(gem_type_fn(NULL, _t1644, 1), gem_string("table"));
    } else {
        GemVal _t1645 = gem_table_get(gem_v_el, gem_v_i);
        _t1646 = gem_eq(gem_table_get(_t1645, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1646)) {
#line 271 "compiler/main.gem"
    GemVal _t1647 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1648[] = {gem_v_d, gem_table_get(_t1647, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1648, 2));
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
    GemVal _t1649[] = {gem_v__match_11};
    GemVal _t1651;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1649, 1), gem_string("table")))) {
        _t1651 = gem_eq(gem_type_fn(NULL, _t1649, 1), gem_string("table"));
    } else {
        GemVal _t1650[] = {gem_v__match_11, gem_string("tag")};
        _t1651 = gem_has_key_fn(NULL, _t1650, 2);
    }
    GemVal _t1652;
    if (!gem_truthy(_t1651)) {
        _t1652 = _t1651;
    } else {
        _t1652 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1654;
    if (!gem_truthy(_t1652)) {
        _t1654 = _t1652;
    } else {
        GemVal _t1653[] = {gem_v__match_11, gem_string("cond")};
        _t1654 = gem_has_key_fn(NULL, _t1653, 2);
    }
    GemVal _t1656;
    if (!gem_truthy(_t1654)) {
        _t1656 = _t1654;
    } else {
        GemVal _t1655[] = {gem_v__match_11, gem_string("body")};
        _t1656 = gem_has_key_fn(NULL, _t1655, 2);
    }
    if (gem_truthy(_t1656)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 276 "compiler/main.gem"
    GemVal _t1657[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1658 = (*gem_v_collect_free_node);
        (void)(_t1658.fn(_t1658.env, _t1657, 3));
#line 277 "compiler/main.gem"
    GemVal _t1659 = gem_table_new();
    GemVal _t1660[] = {gem_v_defined, _t1659};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1660, 2);
        {
#line 278 "compiler/main.gem"
            GemVal gem_v__for_i_15 = gem_int(0);
#line 278 "compiler/main.gem"
    GemVal _t1661[] = {gem_v_body};
            GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1661, 1);
#line 278 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 278 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_15;
#line 278 "compiler/main.gem"
                gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 279 "compiler/main.gem"
    GemVal _t1662[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1663 = (*gem_v_collect_free_node);
                (void)(_t1663.fn(_t1663.env, _t1662, 3));
#line 280 "compiler/main.gem"
    GemVal _t1664[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1666;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1664, 1), gem_string("table")))) {
        _t1666 = gem_eq(gem_type_fn(NULL, _t1664, 1), gem_string("table"));
    } else {
        GemVal _t1665 = gem_table_get(gem_v_body, gem_v_i);
        _t1666 = gem_eq(gem_table_get(_t1665, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1666)) {
#line 281 "compiler/main.gem"
    GemVal _t1667 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1668[] = {gem_v_d, gem_table_get(_t1667, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1668, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1669[] = {gem_v__match_11};
    GemVal _t1671;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1669, 1), gem_string("table")))) {
        _t1671 = gem_eq(gem_type_fn(NULL, _t1669, 1), gem_string("table"));
    } else {
        GemVal _t1670[] = {gem_v__match_11, gem_string("tag")};
        _t1671 = gem_has_key_fn(NULL, _t1670, 2);
    }
    GemVal _t1672;
    if (!gem_truthy(_t1671)) {
        _t1672 = _t1671;
    } else {
        _t1672 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("match"));
    }
    GemVal _t1674;
    if (!gem_truthy(_t1672)) {
        _t1674 = _t1672;
    } else {
        GemVal _t1673[] = {gem_v__match_11, gem_string("target")};
        _t1674 = gem_has_key_fn(NULL, _t1673, 2);
    }
    GemVal _t1676;
    if (!gem_truthy(_t1674)) {
        _t1676 = _t1674;
    } else {
        GemVal _t1675[] = {gem_v__match_11, gem_string("target_var")};
        _t1676 = gem_has_key_fn(NULL, _t1675, 2);
    }
    GemVal _t1678;
    if (!gem_truthy(_t1676)) {
        _t1678 = _t1676;
    } else {
        GemVal _t1677[] = {gem_v__match_11, gem_string("whens")};
        _t1678 = gem_has_key_fn(NULL, _t1677, 2);
    }
    GemVal _t1680;
    if (!gem_truthy(_t1678)) {
        _t1680 = _t1678;
    } else {
        GemVal _t1679[] = {gem_v__match_11, gem_string("else")};
        _t1680 = gem_has_key_fn(NULL, _t1679, 2);
    }
    if (gem_truthy(_t1680)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_11, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_target_var = gem_table_get(gem_v__match_11, gem_string("target_var"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_11, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 285 "compiler/main.gem"
    GemVal _t1681[] = {gem_v_target, gem_v_defined, gem_v_free};
    GemVal _t1682 = (*gem_v_collect_free_node);
        (void)(_t1682.fn(_t1682.env, _t1681, 3));
#line 286 "compiler/main.gem"
    GemVal _t1683 = gem_table_new();
    GemVal _t1684[] = {gem_v_defined, _t1683};
        GemVal gem_v_match_def = gem_fn_set_union(NULL, _t1684, 2);
#line 287 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_target_var, GEM_NIL))) {
#line 288 "compiler/main.gem"
    GemVal _t1685[] = {gem_v_match_def, gem_v_target_var};
            (void)(gem_fn_set_add(NULL, _t1685, 2));
        }
#line 290 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 290 "compiler/main.gem"
    GemVal _t1686[] = {gem_v_whens};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1686, 1);
#line 290 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 290 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_18;
#line 290 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 291 "compiler/main.gem"
            GemVal gem_v_w = gem_table_get(gem_v_whens, gem_v_i);
#line 292 "compiler/main.gem"
    GemVal _t1687 = gem_v_w;
    GemVal _t1688[] = {gem_table_get(_t1687, gem_string("value")), gem_v_match_def, gem_v_free};
    GemVal _t1689 = (*gem_v_collect_free_node);
            (void)(_t1689.fn(_t1689.env, _t1688, 3));
#line 293 "compiler/main.gem"
    GemVal _t1690 = gem_table_new();
    GemVal _t1691[] = {gem_v_match_def, _t1690};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1691, 2);
#line 294 "compiler/main.gem"
    GemVal _t1692 = gem_v_w;
            if (gem_truthy(gem_neq(gem_table_get(_t1692, gem_string("bindings")), GEM_NIL))) {
#line 295 "compiler/main.gem"
                GemVal gem_v__for_i_16 = gem_int(0);
#line 295 "compiler/main.gem"
    GemVal _t1693 = gem_v_w;
    GemVal _t1694[] = {gem_table_get(_t1693, gem_string("bindings"))};
                GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1694, 1);
#line 295 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 295 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_16;
#line 295 "compiler/main.gem"
                    gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 296 "compiler/main.gem"
    GemVal _t1695 = gem_v_w;
    GemVal _t1696[] = {gem_table_get(gem_table_get(_t1695, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1697 = (*gem_v_collect_free_node);
                    (void)(_t1697.fn(_t1697.env, _t1696, 3));
#line 297 "compiler/main.gem"
    GemVal _t1698 = gem_v_w;
    GemVal _t1699[] = {gem_table_get(gem_table_get(_t1698, gem_string("bindings")), gem_v_bi)};
    GemVal _t1702;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1699, 1), gem_string("table")))) {
        _t1702 = gem_eq(gem_type_fn(NULL, _t1699, 1), gem_string("table"));
    } else {
        GemVal _t1700 = gem_v_w;
        GemVal _t1701 = gem_table_get(gem_table_get(_t1700, gem_string("bindings")), gem_v_bi);
        _t1702 = gem_eq(gem_table_get(_t1701, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1702)) {
#line 298 "compiler/main.gem"
    GemVal _t1703 = gem_v_w;
    GemVal _t1704 = gem_table_get(gem_table_get(_t1703, gem_string("bindings")), gem_v_bi);
    GemVal _t1705[] = {gem_v_d, gem_table_get(_t1704, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1705, 2));
                    }
                }

            }
#line 302 "compiler/main.gem"
            GemVal gem_v__for_i_17 = gem_int(0);
#line 302 "compiler/main.gem"
    GemVal _t1706 = gem_v_w;
    GemVal _t1707[] = {gem_table_get(_t1706, gem_string("body"))};
            GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1707, 1);
#line 302 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 302 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_17;
#line 302 "compiler/main.gem"
                gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 303 "compiler/main.gem"
    GemVal _t1708 = gem_v_w;
    GemVal _t1709[] = {gem_table_get(gem_table_get(_t1708, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1710 = (*gem_v_collect_free_node);
                (void)(_t1710.fn(_t1710.env, _t1709, 3));
#line 304 "compiler/main.gem"
    GemVal _t1711 = gem_v_w;
    GemVal _t1712[] = {gem_table_get(gem_table_get(_t1711, gem_string("body")), gem_v_j)};
    GemVal _t1715;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1712, 1), gem_string("table")))) {
        _t1715 = gem_eq(gem_type_fn(NULL, _t1712, 1), gem_string("table"));
    } else {
        GemVal _t1713 = gem_v_w;
        GemVal _t1714 = gem_table_get(gem_table_get(_t1713, gem_string("body")), gem_v_j);
        _t1715 = gem_eq(gem_table_get(_t1714, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1715)) {
#line 305 "compiler/main.gem"
    GemVal _t1716 = gem_v_w;
    GemVal _t1717 = gem_table_get(gem_table_get(_t1716, gem_string("body")), gem_v_j);
    GemVal _t1718[] = {gem_v_d, gem_table_get(_t1717, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1718, 2));
                }
            }

        }

#line 309 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 310 "compiler/main.gem"
    GemVal _t1719 = gem_table_new();
    GemVal _t1720[] = {gem_v_defined, _t1719};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1720, 2);
            {
#line 311 "compiler/main.gem"
                GemVal gem_v__for_i_19 = gem_int(0);
#line 311 "compiler/main.gem"
    GemVal _t1721[] = {gem_v_el};
                GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1721, 1);
#line 311 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 311 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_19;
#line 311 "compiler/main.gem"
                    gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 312 "compiler/main.gem"
    GemVal _t1722[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1723 = (*gem_v_collect_free_node);
                    (void)(_t1723.fn(_t1723.env, _t1722, 3));
#line 313 "compiler/main.gem"
    GemVal _t1724[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t1726;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1724, 1), gem_string("table")))) {
        _t1726 = gem_eq(gem_type_fn(NULL, _t1724, 1), gem_string("table"));
    } else {
        GemVal _t1725 = gem_table_get(gem_v_el, gem_v_j);
        _t1726 = gem_eq(gem_table_get(_t1725, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1726)) {
#line 314 "compiler/main.gem"
    GemVal _t1727 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t1728[] = {gem_v_d, gem_table_get(_t1727, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1728, 2));
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
    GemVal _t1729[] = {gem_v__match_11};
    GemVal _t1731;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1729, 1), gem_string("table")))) {
        _t1731 = gem_eq(gem_type_fn(NULL, _t1729, 1), gem_string("table"));
    } else {
        GemVal _t1730[] = {gem_v__match_11, gem_string("tag")};
        _t1731 = gem_has_key_fn(NULL, _t1730, 2);
    }
    GemVal _t1732;
    if (!gem_truthy(_t1731)) {
        _t1732 = _t1731;
    } else {
        _t1732 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("when_clause"));
    }
    GemVal _t1734;
    if (!gem_truthy(_t1732)) {
        _t1734 = _t1732;
    } else {
        GemVal _t1733[] = {gem_v__match_11, gem_string("value")};
        _t1734 = gem_has_key_fn(NULL, _t1733, 2);
    }
    GemVal _t1736;
    if (!gem_truthy(_t1734)) {
        _t1736 = _t1734;
    } else {
        GemVal _t1735[] = {gem_v__match_11, gem_string("bindings")};
        _t1736 = gem_has_key_fn(NULL, _t1735, 2);
    }
    GemVal _t1738;
    if (!gem_truthy(_t1736)) {
        _t1738 = _t1736;
    } else {
        GemVal _t1737[] = {gem_v__match_11, gem_string("body")};
        _t1738 = gem_has_key_fn(NULL, _t1737, 2);
    }
    if (gem_truthy(_t1738)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_bindings = gem_table_get(gem_v__match_11, gem_string("bindings"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 319 "compiler/main.gem"
    GemVal _t1739[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1740 = (*gem_v_collect_free_node);
        (void)(_t1740.fn(_t1740.env, _t1739, 3));
#line 320 "compiler/main.gem"
    GemVal _t1741 = gem_table_new();
    GemVal _t1742[] = {gem_v_defined, _t1741};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1742, 2);
#line 321 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_bindings, GEM_NIL))) {
#line 322 "compiler/main.gem"
            GemVal gem_v__for_i_20 = gem_int(0);
#line 322 "compiler/main.gem"
    GemVal _t1743[] = {gem_v_bindings};
            GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1743, 1);
#line 322 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 322 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_20;
#line 322 "compiler/main.gem"
                gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 323 "compiler/main.gem"
    GemVal _t1744[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1745 = (*gem_v_collect_free_node);
                (void)(_t1745.fn(_t1745.env, _t1744, 3));
#line 324 "compiler/main.gem"
    GemVal _t1746[] = {gem_table_get(gem_v_bindings, gem_v_bi)};
    GemVal _t1748;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1746, 1), gem_string("table")))) {
        _t1748 = gem_eq(gem_type_fn(NULL, _t1746, 1), gem_string("table"));
    } else {
        GemVal _t1747 = gem_table_get(gem_v_bindings, gem_v_bi);
        _t1748 = gem_eq(gem_table_get(_t1747, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1748)) {
#line 325 "compiler/main.gem"
    GemVal _t1749 = gem_table_get(gem_v_bindings, gem_v_bi);
    GemVal _t1750[] = {gem_v_d, gem_table_get(_t1749, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1750, 2));
                }
            }

        }
        {
#line 329 "compiler/main.gem"
            GemVal gem_v__for_i_21 = gem_int(0);
#line 329 "compiler/main.gem"
    GemVal _t1751[] = {gem_v_body};
            GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t1751, 1);
#line 329 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 329 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_21;
#line 329 "compiler/main.gem"
                gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 330 "compiler/main.gem"
    GemVal _t1752[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1753 = (*gem_v_collect_free_node);
                (void)(_t1753.fn(_t1753.env, _t1752, 3));
#line 331 "compiler/main.gem"
    GemVal _t1754[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1756;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1754, 1), gem_string("table")))) {
        _t1756 = gem_eq(gem_type_fn(NULL, _t1754, 1), gem_string("table"));
    } else {
        GemVal _t1755 = gem_table_get(gem_v_body, gem_v_i);
        _t1756 = gem_eq(gem_table_get(_t1755, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1756)) {
#line 332 "compiler/main.gem"
    GemVal _t1757 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1758[] = {gem_v_d, gem_table_get(_t1757, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1758, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1759[] = {gem_v__match_11};
    GemVal _t1761;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1759, 1), gem_string("table")))) {
        _t1761 = gem_eq(gem_type_fn(NULL, _t1759, 1), gem_string("table"));
    } else {
        GemVal _t1760[] = {gem_v__match_11, gem_string("tag")};
        _t1761 = gem_has_key_fn(NULL, _t1760, 2);
    }
    GemVal _t1762;
    if (!gem_truthy(_t1761)) {
        _t1762 = _t1761;
    } else {
        _t1762 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t1764;
    if (!gem_truthy(_t1762)) {
        _t1764 = _t1762;
    } else {
        GemVal _t1763[] = {gem_v__match_11, gem_string("arms")};
        _t1764 = gem_has_key_fn(NULL, _t1763, 2);
    }
    GemVal _t1766;
    if (!gem_truthy(_t1764)) {
        _t1766 = _t1764;
    } else {
        GemVal _t1765[] = {gem_v__match_11, gem_string("after_ms")};
        _t1766 = gem_has_key_fn(NULL, _t1765, 2);
    }
    GemVal _t1768;
    if (!gem_truthy(_t1766)) {
        _t1768 = _t1766;
    } else {
        GemVal _t1767[] = {gem_v__match_11, gem_string("after_body")};
        _t1768 = gem_has_key_fn(NULL, _t1767, 2);
    }
    if (gem_truthy(_t1768)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_11, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_11, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_11, gem_string("after_body"));
#line 336 "compiler/main.gem"
    GemVal _t1769 = gem_table_new();
    GemVal _t1770[] = {gem_v_defined, _t1769};
        GemVal gem_v_recv_def = gem_fn_set_union(NULL, _t1770, 2);
#line 337 "compiler/main.gem"
    GemVal _t1771 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t1771, gem_string("recv_var")), GEM_NIL))) {
#line 338 "compiler/main.gem"
    GemVal _t1772 = gem_v_node;
    GemVal _t1773[] = {gem_v_recv_def, gem_table_get(_t1772, gem_string("recv_var"))};
            (void)(gem_fn_set_add(NULL, _t1773, 2));
        }
#line 340 "compiler/main.gem"
        GemVal gem_v__for_i_24 = gem_int(0);
#line 340 "compiler/main.gem"
    GemVal _t1774[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1774, 1);
#line 340 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 340 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_24;
#line 340 "compiler/main.gem"
            gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 341 "compiler/main.gem"
            GemVal gem_v_arm = gem_table_get(gem_v_rm_arms, gem_v_i);
#line 342 "compiler/main.gem"
    GemVal _t1775 = gem_v_arm;
    GemVal _t1776 = gem_table_get(_t1775, gem_string("pattern"));
    GemVal _t1777[] = {gem_table_get(_t1776, gem_string("condition")), gem_v_recv_def, gem_v_free};
    GemVal _t1778 = (*gem_v_collect_free_node);
            (void)(_t1778.fn(_t1778.env, _t1777, 3));
#line 343 "compiler/main.gem"
    GemVal _t1779 = gem_table_new();
    GemVal _t1780[] = {gem_v_recv_def, _t1779};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1780, 2);
#line 344 "compiler/main.gem"
    GemVal _t1781 = gem_v_arm;
    GemVal _t1782 = gem_table_get(_t1781, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t1782, gem_string("bindings")), GEM_NIL))) {
#line 345 "compiler/main.gem"
                GemVal gem_v__for_i_22 = gem_int(0);
#line 345 "compiler/main.gem"
    GemVal _t1783 = gem_v_arm;
    GemVal _t1784 = gem_table_get(_t1783, gem_string("pattern"));
    GemVal _t1785[] = {gem_table_get(_t1784, gem_string("bindings"))};
                GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1785, 1);
#line 345 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 345 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_22;
#line 345 "compiler/main.gem"
                    gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 346 "compiler/main.gem"
    GemVal _t1786 = gem_v_arm;
    GemVal _t1787 = gem_table_get(_t1786, gem_string("pattern"));
    GemVal _t1788[] = {gem_table_get(gem_table_get(_t1787, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1789 = (*gem_v_collect_free_node);
                    (void)(_t1789.fn(_t1789.env, _t1788, 3));
#line 347 "compiler/main.gem"
    GemVal _t1790 = gem_v_arm;
    GemVal _t1791 = gem_table_get(_t1790, gem_string("pattern"));
    GemVal _t1792[] = {gem_table_get(gem_table_get(_t1791, gem_string("bindings")), gem_v_bi)};
    GemVal _t1796;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1792, 1), gem_string("table")))) {
        _t1796 = gem_eq(gem_type_fn(NULL, _t1792, 1), gem_string("table"));
    } else {
        GemVal _t1793 = gem_v_arm;
        GemVal _t1794 = gem_table_get(_t1793, gem_string("pattern"));
        GemVal _t1795 = gem_table_get(gem_table_get(_t1794, gem_string("bindings")), gem_v_bi);
        _t1796 = gem_eq(gem_table_get(_t1795, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1796)) {
#line 348 "compiler/main.gem"
    GemVal _t1797 = gem_v_arm;
    GemVal _t1798 = gem_table_get(_t1797, gem_string("pattern"));
    GemVal _t1799 = gem_table_get(gem_table_get(_t1798, gem_string("bindings")), gem_v_bi);
    GemVal _t1800[] = {gem_v_d, gem_table_get(_t1799, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1800, 2));
                    }
                }

            }
#line 352 "compiler/main.gem"
            GemVal gem_v__for_i_23 = gem_int(0);
#line 352 "compiler/main.gem"
    GemVal _t1801 = gem_v_arm;
    GemVal _t1802[] = {gem_table_get(_t1801, gem_string("body"))};
            GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t1802, 1);
#line 352 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 352 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_23;
#line 352 "compiler/main.gem"
                gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 353 "compiler/main.gem"
    GemVal _t1803 = gem_v_arm;
    GemVal _t1804[] = {gem_table_get(gem_table_get(_t1803, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1805 = (*gem_v_collect_free_node);
                (void)(_t1805.fn(_t1805.env, _t1804, 3));
#line 354 "compiler/main.gem"
    GemVal _t1806 = gem_v_arm;
    GemVal _t1807[] = {gem_table_get(gem_table_get(_t1806, gem_string("body")), gem_v_j)};
    GemVal _t1810;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1807, 1), gem_string("table")))) {
        _t1810 = gem_eq(gem_type_fn(NULL, _t1807, 1), gem_string("table"));
    } else {
        GemVal _t1808 = gem_v_arm;
        GemVal _t1809 = gem_table_get(gem_table_get(_t1808, gem_string("body")), gem_v_j);
        _t1810 = gem_eq(gem_table_get(_t1809, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1810)) {
#line 355 "compiler/main.gem"
    GemVal _t1811 = gem_v_arm;
    GemVal _t1812 = gem_table_get(gem_table_get(_t1811, gem_string("body")), gem_v_j);
    GemVal _t1813[] = {gem_v_d, gem_table_get(_t1812, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1813, 2));
                }
            }

        }

#line 359 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 360 "compiler/main.gem"
    GemVal _t1814[] = {gem_v_rm_after_ms, gem_v_recv_def, gem_v_free};
    GemVal _t1815 = (*gem_v_collect_free_node);
            (void)(_t1815.fn(_t1815.env, _t1814, 3));
        }
#line 362 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 363 "compiler/main.gem"
    GemVal _t1816 = gem_table_new();
    GemVal _t1817[] = {gem_v_recv_def, _t1816};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1817, 2);
            {
#line 364 "compiler/main.gem"
                GemVal gem_v__for_i_25 = gem_int(0);
#line 364 "compiler/main.gem"
    GemVal _t1818[] = {gem_v_rm_after_body};
                GemVal gem_v__for_limit_25 = gem_len_fn(NULL, _t1818, 1);
#line 364 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 364 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_25;
#line 364 "compiler/main.gem"
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 365 "compiler/main.gem"
    GemVal _t1819[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1820 = (*gem_v_collect_free_node);
                    (void)(_t1820.fn(_t1820.env, _t1819, 3));
#line 366 "compiler/main.gem"
    GemVal _t1821[] = {gem_table_get(gem_v_rm_after_body, gem_v_j)};
    GemVal _t1823;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1821, 1), gem_string("table")))) {
        _t1823 = gem_eq(gem_type_fn(NULL, _t1821, 1), gem_string("table"));
    } else {
        GemVal _t1822 = gem_table_get(gem_v_rm_after_body, gem_v_j);
        _t1823 = gem_eq(gem_table_get(_t1822, gem_string("tag")), gem_string("let"));
    }
                    if (gem_truthy(_t1823)) {
#line 367 "compiler/main.gem"
    GemVal _t1824 = gem_table_get(gem_v_rm_after_body, gem_v_j);
    GemVal _t1825[] = {gem_v_d, gem_table_get(_t1824, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1825, 2));
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
    GemVal _t1826[] = {gem_v__match_11};
    GemVal _t1828;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1826, 1), gem_string("table")))) {
        _t1828 = gem_eq(gem_type_fn(NULL, _t1826, 1), gem_string("table"));
    } else {
        GemVal _t1827[] = {gem_v__match_11, gem_string("tag")};
        _t1828 = gem_has_key_fn(NULL, _t1827, 2);
    }
    GemVal _t1829;
    if (!gem_truthy(_t1828)) {
        _t1829 = _t1828;
    } else {
        _t1829 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("table"));
    }
    GemVal _t1831;
    if (!gem_truthy(_t1829)) {
        _t1831 = _t1829;
    } else {
        GemVal _t1830[] = {gem_v__match_11, gem_string("entries")};
        _t1831 = gem_has_key_fn(NULL, _t1830, 2);
    }
    if (gem_truthy(_t1831)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_11, gem_string("entries"));
        {
#line 372 "compiler/main.gem"
            GemVal gem_v__for_i_26 = gem_int(0);
#line 372 "compiler/main.gem"
    GemVal _t1832[] = {gem_v_entries};
            GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1832, 1);
#line 372 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 372 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_26;
#line 372 "compiler/main.gem"
                gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 373 "compiler/main.gem"
    GemVal _t1833 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t1834[] = {gem_table_get(_t1833, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1835 = (*gem_v_collect_free_node);
                (void)(_t1835.fn(_t1835.env, _t1834, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1836[] = {gem_v__match_11};
    GemVal _t1838;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1836, 1), gem_string("table")))) {
        _t1838 = gem_eq(gem_type_fn(NULL, _t1836, 1), gem_string("table"));
    } else {
        GemVal _t1837[] = {gem_v__match_11, gem_string("tag")};
        _t1838 = gem_has_key_fn(NULL, _t1837, 2);
    }
    GemVal _t1839;
    if (!gem_truthy(_t1838)) {
        _t1839 = _t1838;
    } else {
        _t1839 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("array"));
    }
    GemVal _t1841;
    if (!gem_truthy(_t1839)) {
        _t1841 = _t1839;
    } else {
        GemVal _t1840[] = {gem_v__match_11, gem_string("elements")};
        _t1841 = gem_has_key_fn(NULL, _t1840, 2);
    }
    if (gem_truthy(_t1841)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_11, gem_string("elements"));
        {
#line 376 "compiler/main.gem"
            GemVal gem_v__for_i_27 = gem_int(0);
#line 376 "compiler/main.gem"
    GemVal _t1842[] = {gem_v_elements};
            GemVal gem_v__for_limit_27 = gem_len_fn(NULL, _t1842, 1);
#line 376 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 376 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_27;
#line 376 "compiler/main.gem"
                gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 377 "compiler/main.gem"
    GemVal _t1843[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_defined, gem_v_free};
    GemVal _t1844 = (*gem_v_collect_free_node);
                (void)(_t1844.fn(_t1844.env, _t1843, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1845[] = {gem_v__match_11};
    GemVal _t1847;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1845, 1), gem_string("table")))) {
        _t1847 = gem_eq(gem_type_fn(NULL, _t1845, 1), gem_string("table"));
    } else {
        GemVal _t1846[] = {gem_v__match_11, gem_string("tag")};
        _t1847 = gem_has_key_fn(NULL, _t1846, 2);
    }
    GemVal _t1848;
    if (!gem_truthy(_t1847)) {
        _t1848 = _t1847;
    } else {
        _t1848 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("block"));
    }
    GemVal _t1850;
    if (!gem_truthy(_t1848)) {
        _t1850 = _t1848;
    } else {
        GemVal _t1849[] = {gem_v__match_11, gem_string("stmts")};
        _t1850 = gem_has_key_fn(NULL, _t1849, 2);
    }
    if (gem_truthy(_t1850)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_11, gem_string("stmts"));
#line 380 "compiler/main.gem"
    GemVal _t1851 = gem_table_new();
    GemVal _t1852[] = {gem_v_defined, _t1851};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1852, 2);
        {
#line 381 "compiler/main.gem"
            GemVal gem_v__for_i_28 = gem_int(0);
#line 381 "compiler/main.gem"
    GemVal _t1853[] = {gem_v_stmts};
            GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1853, 1);
#line 381 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 381 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_28;
#line 381 "compiler/main.gem"
                gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 382 "compiler/main.gem"
    GemVal _t1854[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1855 = (*gem_v_collect_free_node);
                (void)(_t1855.fn(_t1855.env, _t1854, 3));
#line 383 "compiler/main.gem"
    GemVal _t1856[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1858;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1856, 1), gem_string("table")))) {
        _t1858 = gem_eq(gem_type_fn(NULL, _t1856, 1), gem_string("table"));
    } else {
        GemVal _t1857 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1858 = gem_eq(gem_table_get(_t1857, gem_string("tag")), gem_string("let"));
    }
                if (gem_truthy(_t1858)) {
#line 384 "compiler/main.gem"
    GemVal _t1859 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1860[] = {gem_v_d, gem_table_get(_t1859, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1860, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1861[] = {gem_v__match_11};
    GemVal _t1863;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1861, 1), gem_string("table")))) {
        _t1863 = gem_eq(gem_type_fn(NULL, _t1861, 1), gem_string("table"));
    } else {
        GemVal _t1862[] = {gem_v__match_11, gem_string("tag")};
        _t1863 = gem_has_key_fn(NULL, _t1862, 2);
    }
    GemVal _t1864;
    if (!gem_truthy(_t1863)) {
        _t1864 = _t1863;
    } else {
        _t1864 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("return"));
    }
    GemVal _t1866;
    if (!gem_truthy(_t1864)) {
        _t1866 = _t1864;
    } else {
        GemVal _t1865[] = {gem_v__match_11, gem_string("value")};
        _t1866 = gem_has_key_fn(NULL, _t1865, 2);
    }
    if (gem_truthy(_t1866)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 388 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 389 "compiler/main.gem"
    GemVal _t1867[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1868 = (*gem_v_collect_free_node);
            GemVal _t1869 = _t1868.fn(_t1868.env, _t1867, 3);
            gem_pop_frame();
            return _t1869;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
        {
#line 393 "compiler/main.gem"
            GemVal gem_v__for_tbl_29 = gem_v_node;
#line 393 "compiler/main.gem"
    GemVal _t1870[] = {gem_v__for_tbl_29};
            GemVal gem_v__for_keys_29 = gem_keys_fn(NULL, _t1870, 1);
#line 393 "compiler/main.gem"
            GemVal gem_v__for_i_29 = gem_int(0);
#line 393 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1871[] = {gem_v__for_keys_29};
                if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_len_fn(NULL, _t1871, 1)))) break;
#line 393 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_29, gem_v__for_i_29);
#line 393 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_29, gem_table_get(gem_v__for_keys_29, gem_v__for_i_29));
#line 393 "compiler/main.gem"
                gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 394 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 395 "compiler/main.gem"
    GemVal _t1872[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1872, 1), gem_string("table")))) {
#line 396 "compiler/main.gem"
    GemVal _t1873[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t1873, 1);
#line 397 "compiler/main.gem"
    GemVal _t1874[] = {gem_v_vks};
    GemVal _t1876;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1874, 1), gem_int(0)))) {
        _t1876 = gem_gt(gem_len_fn(NULL, _t1874, 1), gem_int(0));
    } else {
        GemVal _t1875[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1876 = gem_eq(gem_type_fn(NULL, _t1875, 1), gem_string("int"));
    }
                        if (gem_truthy(_t1876)) {
#line 398 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 399 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t1877[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1877, 1)))) break;
#line 400 "compiler/main.gem"
    GemVal _t1878[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1878, 1), gem_string("table")))) {
#line 401 "compiler/main.gem"
    GemVal _t1879[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t1880 = (*gem_v_collect_free_node);
                                    (void)(_t1880.fn(_t1880.env, _t1879, 3));
                                }
#line 403 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 406 "compiler/main.gem"
    GemVal _t1881[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t1882 = (*gem_v_collect_free_node);
                            (void)(_t1882.fn(_t1882.env, _t1881, 3));
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
#line 415 "compiler/main.gem"
    GemVal _t1884 = gem_table_new();
    GemVal gem_v_free = _t1884;
#line 416 "compiler/main.gem"
    GemVal _t1885 = gem_table_new();
    GemVal _t1886[] = {gem_v_defined, _t1885};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t1886, 2);
#line 417 "compiler/main.gem"
    GemVal gem_v__for_i_30 = gem_int(0);
#line 417 "compiler/main.gem"
    GemVal _t1887[] = {gem_v_stmts};
    GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t1887, 1);
#line 417 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 417 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_30;
#line 417 "compiler/main.gem"
        gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 418 "compiler/main.gem"
    GemVal _t1888[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1889 = (*gem_v_collect_free_node);
        (void)(_t1889.fn(_t1889.env, _t1888, 3));
#line 419 "compiler/main.gem"
    GemVal _t1890[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1892;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1890, 1), gem_string("table")))) {
        _t1892 = gem_eq(gem_type_fn(NULL, _t1890, 1), gem_string("table"));
    } else {
        GemVal _t1891 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1892 = gem_eq(gem_table_get(_t1891, gem_string("tag")), gem_string("let"));
    }
        if (gem_truthy(_t1892)) {
#line 420 "compiler/main.gem"
    GemVal _t1893 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1894[] = {gem_v_d, gem_table_get(_t1893, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1894, 2));
        }
#line 422 "compiler/main.gem"
    GemVal _t1895[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d};
        (void)(gem_fn_collect_block_lets(NULL, _t1895, 2));
    }

    GemVal _t1896 = gem_v_free;
    gem_pop_frame();
    return _t1896;
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
#line 433 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 434 "compiler/main.gem"
        GemVal _t1898 = GEM_NIL;
        gem_pop_frame();
        return _t1898;
    }
#line 436 "compiler/main.gem"
    GemVal _t1899[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1899, 1), gem_string("table")))) {
#line 437 "compiler/main.gem"
        GemVal _t1900 = GEM_NIL;
        gem_pop_frame();
        return _t1900;
    }
#line 439 "compiler/main.gem"
    GemVal _t1901 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1901, gem_string("tag")), GEM_NIL))) {
#line 440 "compiler/main.gem"
        GemVal _t1902 = GEM_NIL;
        gem_pop_frame();
        return _t1902;
    }
#line 443 "compiler/main.gem"
    GemVal gem_v__match_31 = gem_v_node;
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
        _t1906 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1908;
    if (!gem_truthy(_t1906)) {
        _t1908 = _t1906;
    } else {
        GemVal _t1907[] = {gem_v__match_31, gem_string("params")};
        _t1908 = gem_has_key_fn(NULL, _t1907, 2);
    }
    GemVal _t1910;
    if (!gem_truthy(_t1908)) {
        _t1910 = _t1908;
    } else {
        GemVal _t1909[] = {gem_v__match_31, gem_string("rest_param")};
        _t1910 = gem_has_key_fn(NULL, _t1909, 2);
    }
    GemVal _t1912;
    if (!gem_truthy(_t1910)) {
        _t1912 = _t1910;
    } else {
        GemVal _t1911[] = {gem_v__match_31, gem_string("block_param")};
        _t1912 = gem_has_key_fn(NULL, _t1911, 2);
    }
    GemVal _t1914;
    if (!gem_truthy(_t1912)) {
        _t1914 = _t1912;
    } else {
        GemVal _t1913[] = {gem_v__match_31, gem_string("body")};
        _t1914 = gem_has_key_fn(NULL, _t1913, 2);
    }
    if (gem_truthy(_t1914)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_31, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_31, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_31, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 445 "compiler/main.gem"
    GemVal _t1915[] = {gem_v_params};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1915, 1);
#line 446 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 447 "compiler/main.gem"
    GemVal _t1916[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn_set_add(NULL, _t1916, 2));
        }
#line 449 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 450 "compiler/main.gem"
    GemVal _t1917[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn_set_add(NULL, _t1917, 2));
        }
#line 457 "compiler/main.gem"
    GemVal _t1918[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1919 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t1919.fn(_t1919.env, _t1918, 2);
#line 458 "compiler/main.gem"
    GemVal _t1920[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t1920, 2);
        {
#line 459 "compiler/main.gem"
            GemVal gem_v__for_tbl_32 = gem_v_inter;
#line 459 "compiler/main.gem"
    GemVal _t1921[] = {gem_v__for_tbl_32};
            GemVal gem_v__for_keys_32 = gem_keys_fn(NULL, _t1921, 1);
#line 459 "compiler/main.gem"
            GemVal gem_v__for_i_32 = gem_int(0);
#line 459 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1922[] = {gem_v__for_keys_32};
                if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_len_fn(NULL, _t1922, 1)))) break;
#line 459 "compiler/main.gem"
                GemVal gem_v_ik = gem_table_get(gem_v__for_keys_32, gem_v__for_i_32);
#line 459 "compiler/main.gem"
                GemVal gem_v__ = gem_table_get(gem_v__for_tbl_32, gem_table_get(gem_v__for_keys_32, gem_v__for_i_32));
#line 459 "compiler/main.gem"
                gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 460 "compiler/main.gem"
    GemVal _t1923[] = {gem_v_captured, gem_v_ik};
                (void)(gem_fn_set_add(NULL, _t1923, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1924[] = {gem_v__match_31};
    GemVal _t1926;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1924, 1), gem_string("table")))) {
        _t1926 = gem_eq(gem_type_fn(NULL, _t1924, 1), gem_string("table"));
    } else {
        GemVal _t1925[] = {gem_v__match_31, gem_string("tag")};
        _t1926 = gem_has_key_fn(NULL, _t1925, 2);
    }
    GemVal _t1927;
    if (!gem_truthy(_t1926)) {
        _t1927 = _t1926;
    } else {
        _t1927 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1927)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1928[] = {gem_v__match_31};
    GemVal _t1930;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1928, 1), gem_string("table")))) {
        _t1930 = gem_eq(gem_type_fn(NULL, _t1928, 1), gem_string("table"));
    } else {
        GemVal _t1929[] = {gem_v__match_31, gem_string("tag")};
        _t1930 = gem_has_key_fn(NULL, _t1929, 2);
    }
    GemVal _t1931;
    if (!gem_truthy(_t1930)) {
        _t1931 = _t1930;
    } else {
        _t1931 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1933;
    if (!gem_truthy(_t1931)) {
        _t1933 = _t1931;
    } else {
        GemVal _t1932[] = {gem_v__match_31, gem_string("cond")};
        _t1933 = gem_has_key_fn(NULL, _t1932, 2);
    }
    GemVal _t1935;
    if (!gem_truthy(_t1933)) {
        _t1935 = _t1933;
    } else {
        GemVal _t1934[] = {gem_v__match_31, gem_string("then")};
        _t1935 = gem_has_key_fn(NULL, _t1934, 2);
    }
    GemVal _t1937;
    if (!gem_truthy(_t1935)) {
        _t1937 = _t1935;
    } else {
        GemVal _t1936[] = {gem_v__match_31, gem_string("else")};
        _t1937 = gem_has_key_fn(NULL, _t1936, 2);
    }
    if (gem_truthy(_t1937)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_31, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 465 "compiler/main.gem"
    GemVal _t1938[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t1939 = (*gem_v_walk_captures_node);
        (void)(_t1939.fn(_t1939.env, _t1938, 3));
#line 466 "compiler/main.gem"
    GemVal _t1940[] = {gem_v_thens, gem_v_scope_vars, gem_v_captured};
    GemVal _t1941 = (*gem_v_walk_captures);
        (void)(_t1941.fn(_t1941.env, _t1940, 3));
#line 467 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 468 "compiler/main.gem"
    GemVal _t1942[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t1943 = (*gem_v_walk_captures);
            GemVal _t1944 = _t1943.fn(_t1943.env, _t1942, 3);
            gem_pop_frame();
            return _t1944;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1945[] = {gem_v__match_31};
    GemVal _t1947;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1945, 1), gem_string("table")))) {
        _t1947 = gem_eq(gem_type_fn(NULL, _t1945, 1), gem_string("table"));
    } else {
        GemVal _t1946[] = {gem_v__match_31, gem_string("tag")};
        _t1947 = gem_has_key_fn(NULL, _t1946, 2);
    }
    GemVal _t1948;
    if (!gem_truthy(_t1947)) {
        _t1948 = _t1947;
    } else {
        _t1948 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1950;
    if (!gem_truthy(_t1948)) {
        _t1950 = _t1948;
    } else {
        GemVal _t1949[] = {gem_v__match_31, gem_string("cond")};
        _t1950 = gem_has_key_fn(NULL, _t1949, 2);
    }
    GemVal _t1952;
    if (!gem_truthy(_t1950)) {
        _t1952 = _t1950;
    } else {
        GemVal _t1951[] = {gem_v__match_31, gem_string("body")};
        _t1952 = gem_has_key_fn(NULL, _t1951, 2);
    }
    if (gem_truthy(_t1952)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 471 "compiler/main.gem"
    GemVal _t1953[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t1954 = (*gem_v_walk_captures_node);
        (void)(_t1954.fn(_t1954.env, _t1953, 3));
#line 472 "compiler/main.gem"
    GemVal _t1955[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t1956 = (*gem_v_walk_captures);
        GemVal _t1957 = _t1956.fn(_t1956.env, _t1955, 3);
        gem_pop_frame();
        return _t1957;
    } else {
    GemVal _t1958[] = {gem_v__match_31};
    GemVal _t1960;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1958, 1), gem_string("table")))) {
        _t1960 = gem_eq(gem_type_fn(NULL, _t1958, 1), gem_string("table"));
    } else {
        GemVal _t1959[] = {gem_v__match_31, gem_string("tag")};
        _t1960 = gem_has_key_fn(NULL, _t1959, 2);
    }
    GemVal _t1961;
    if (!gem_truthy(_t1960)) {
        _t1961 = _t1960;
    } else {
        _t1961 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("match"));
    }
    GemVal _t1963;
    if (!gem_truthy(_t1961)) {
        _t1963 = _t1961;
    } else {
        GemVal _t1962[] = {gem_v__match_31, gem_string("target")};
        _t1963 = gem_has_key_fn(NULL, _t1962, 2);
    }
    GemVal _t1965;
    if (!gem_truthy(_t1963)) {
        _t1965 = _t1963;
    } else {
        GemVal _t1964[] = {gem_v__match_31, gem_string("whens")};
        _t1965 = gem_has_key_fn(NULL, _t1964, 2);
    }
    GemVal _t1967;
    if (!gem_truthy(_t1965)) {
        _t1967 = _t1965;
    } else {
        GemVal _t1966[] = {gem_v__match_31, gem_string("else")};
        _t1967 = gem_has_key_fn(NULL, _t1966, 2);
    }
    if (gem_truthy(_t1967)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_31, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_31, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 474 "compiler/main.gem"
    GemVal _t1968[] = {gem_v_target, gem_v_scope_vars, gem_v_captured};
    GemVal _t1969 = (*gem_v_walk_captures_node);
        (void)(_t1969.fn(_t1969.env, _t1968, 3));
#line 475 "compiler/main.gem"
        GemVal gem_v__for_i_34 = gem_int(0);
#line 475 "compiler/main.gem"
    GemVal _t1970[] = {gem_v_whens};
        GemVal gem_v__for_limit_34 = gem_len_fn(NULL, _t1970, 1);
#line 475 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
#line 475 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_34;
#line 475 "compiler/main.gem"
            gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 476 "compiler/main.gem"
    GemVal _t1971 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1972[] = {gem_table_get(_t1971, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1973 = (*gem_v_walk_captures_node);
            (void)(_t1973.fn(_t1973.env, _t1972, 3));
#line 477 "compiler/main.gem"
    GemVal _t1974 = gem_table_get(gem_v_whens, gem_v_i);
            if (gem_truthy(gem_neq(gem_table_get(_t1974, gem_string("bindings")), GEM_NIL))) {
#line 478 "compiler/main.gem"
                GemVal gem_v__for_i_33 = gem_int(0);
#line 478 "compiler/main.gem"
    GemVal _t1975 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1976[] = {gem_table_get(_t1975, gem_string("bindings"))};
                GemVal gem_v__for_limit_33 = gem_len_fn(NULL, _t1976, 1);
#line 478 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
#line 478 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_33;
#line 478 "compiler/main.gem"
                    gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 479 "compiler/main.gem"
    GemVal _t1977 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1978[] = {gem_table_get(gem_table_get(_t1977, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t1979 = (*gem_v_walk_captures_node);
                    (void)(_t1979.fn(_t1979.env, _t1978, 3));
                }

            }
#line 482 "compiler/main.gem"
    GemVal _t1980 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t1981[] = {gem_table_get(_t1980, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t1982 = (*gem_v_walk_captures);
            (void)(_t1982.fn(_t1982.env, _t1981, 3));
        }

#line 484 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 485 "compiler/main.gem"
    GemVal _t1983[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t1984 = (*gem_v_walk_captures);
            GemVal _t1985 = _t1984.fn(_t1984.env, _t1983, 3);
            gem_pop_frame();
            return _t1985;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1986[] = {gem_v__match_31};
    GemVal _t1988;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1986, 1), gem_string("table")))) {
        _t1988 = gem_eq(gem_type_fn(NULL, _t1986, 1), gem_string("table"));
    } else {
        GemVal _t1987[] = {gem_v__match_31, gem_string("tag")};
        _t1988 = gem_has_key_fn(NULL, _t1987, 2);
    }
    GemVal _t1989;
    if (!gem_truthy(_t1988)) {
        _t1989 = _t1988;
    } else {
        _t1989 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t1991;
    if (!gem_truthy(_t1989)) {
        _t1991 = _t1989;
    } else {
        GemVal _t1990[] = {gem_v__match_31, gem_string("arms")};
        _t1991 = gem_has_key_fn(NULL, _t1990, 2);
    }
    GemVal _t1993;
    if (!gem_truthy(_t1991)) {
        _t1993 = _t1991;
    } else {
        GemVal _t1992[] = {gem_v__match_31, gem_string("after_ms")};
        _t1993 = gem_has_key_fn(NULL, _t1992, 2);
    }
    GemVal _t1995;
    if (!gem_truthy(_t1993)) {
        _t1995 = _t1993;
    } else {
        GemVal _t1994[] = {gem_v__match_31, gem_string("after_body")};
        _t1995 = gem_has_key_fn(NULL, _t1994, 2);
    }
    if (gem_truthy(_t1995)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_31, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_31, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_31, gem_string("after_body"));
#line 488 "compiler/main.gem"
        GemVal gem_v__for_i_36 = gem_int(0);
#line 488 "compiler/main.gem"
    GemVal _t1996[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_36 = gem_len_fn(NULL, _t1996, 1);
#line 488 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 488 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_36;
#line 488 "compiler/main.gem"
            gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 489 "compiler/main.gem"
    GemVal _t1997 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t1998 = gem_table_get(_t1997, gem_string("pattern"));
    GemVal _t1999[] = {gem_table_get(_t1998, gem_string("condition")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2000 = (*gem_v_walk_captures_node);
            (void)(_t2000.fn(_t2000.env, _t1999, 3));
#line 490 "compiler/main.gem"
    GemVal _t2001 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2002 = gem_table_get(_t2001, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t2002, gem_string("bindings")), GEM_NIL))) {
#line 491 "compiler/main.gem"
                GemVal gem_v__for_i_35 = gem_int(0);
#line 491 "compiler/main.gem"
    GemVal _t2003 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2004 = gem_table_get(_t2003, gem_string("pattern"));
    GemVal _t2005[] = {gem_table_get(_t2004, gem_string("bindings"))};
                GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t2005, 1);
#line 491 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 491 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_35;
#line 491 "compiler/main.gem"
                    gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 492 "compiler/main.gem"
    GemVal _t2006 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2007 = gem_table_get(_t2006, gem_string("pattern"));
    GemVal _t2008[] = {gem_table_get(gem_table_get(_t2007, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2009 = (*gem_v_walk_captures_node);
                    (void)(_t2009.fn(_t2009.env, _t2008, 3));
                }

            }
#line 495 "compiler/main.gem"
    GemVal _t2010 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2011[] = {gem_table_get(_t2010, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2012 = (*gem_v_walk_captures);
            (void)(_t2012.fn(_t2012.env, _t2011, 3));
        }

#line 497 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 498 "compiler/main.gem"
    GemVal _t2013[] = {gem_v_rm_after_ms, gem_v_scope_vars, gem_v_captured};
    GemVal _t2014 = (*gem_v_walk_captures_node);
            (void)(_t2014.fn(_t2014.env, _t2013, 3));
        }
#line 500 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 501 "compiler/main.gem"
    GemVal _t2015[] = {gem_v_rm_after_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2016 = (*gem_v_walk_captures);
            GemVal _t2017 = _t2016.fn(_t2016.env, _t2015, 3);
            gem_pop_frame();
            return _t2017;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2018[] = {gem_v__match_31};
    GemVal _t2020;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2018, 1), gem_string("table")))) {
        _t2020 = gem_eq(gem_type_fn(NULL, _t2018, 1), gem_string("table"));
    } else {
        GemVal _t2019[] = {gem_v__match_31, gem_string("tag")};
        _t2020 = gem_has_key_fn(NULL, _t2019, 2);
    }
    GemVal _t2021;
    if (!gem_truthy(_t2020)) {
        _t2021 = _t2020;
    } else {
        _t2021 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2023;
    if (!gem_truthy(_t2021)) {
        _t2023 = _t2021;
    } else {
        GemVal _t2022[] = {gem_v__match_31, gem_string("entries")};
        _t2023 = gem_has_key_fn(NULL, _t2022, 2);
    }
    if (gem_truthy(_t2023)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_31, gem_string("entries"));
        {
#line 504 "compiler/main.gem"
            GemVal gem_v__for_i_37 = gem_int(0);
#line 504 "compiler/main.gem"
    GemVal _t2024[] = {gem_v_entries};
            GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t2024, 1);
#line 504 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 504 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_37;
#line 504 "compiler/main.gem"
                gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 505 "compiler/main.gem"
    GemVal _t2025 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t2026[] = {gem_table_get(_t2025, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2027 = (*gem_v_walk_captures_node);
                (void)(_t2027.fn(_t2027.env, _t2026, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
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
        _t2031 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2033;
    if (!gem_truthy(_t2031)) {
        _t2033 = _t2031;
    } else {
        GemVal _t2032[] = {gem_v__match_31, gem_string("elements")};
        _t2033 = gem_has_key_fn(NULL, _t2032, 2);
    }
    if (gem_truthy(_t2033)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_31, gem_string("elements"));
        {
#line 508 "compiler/main.gem"
            GemVal gem_v__for_i_38 = gem_int(0);
#line 508 "compiler/main.gem"
    GemVal _t2034[] = {gem_v_elements};
            GemVal gem_v__for_limit_38 = gem_len_fn(NULL, _t2034, 1);
#line 508 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 508 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_38;
#line 508 "compiler/main.gem"
                gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 509 "compiler/main.gem"
    GemVal _t2035[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_scope_vars, gem_v_captured};
    GemVal _t2036 = (*gem_v_walk_captures_node);
                (void)(_t2036.fn(_t2036.env, _t2035, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2037[] = {gem_v__match_31};
    GemVal _t2039;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2037, 1), gem_string("table")))) {
        _t2039 = gem_eq(gem_type_fn(NULL, _t2037, 1), gem_string("table"));
    } else {
        GemVal _t2038[] = {gem_v__match_31, gem_string("tag")};
        _t2039 = gem_has_key_fn(NULL, _t2038, 2);
    }
    GemVal _t2040;
    if (!gem_truthy(_t2039)) {
        _t2040 = _t2039;
    } else {
        _t2040 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2042;
    if (!gem_truthy(_t2040)) {
        _t2042 = _t2040;
    } else {
        GemVal _t2041[] = {gem_v__match_31, gem_string("stmts")};
        _t2042 = gem_has_key_fn(NULL, _t2041, 2);
    }
    if (gem_truthy(_t2042)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_31, gem_string("stmts"));
#line 512 "compiler/main.gem"
    GemVal _t2043[] = {gem_v_stmts, gem_v_scope_vars, gem_v_captured};
    GemVal _t2044 = (*gem_v_walk_captures);
        GemVal _t2045 = _t2044.fn(_t2044.env, _t2043, 3);
        gem_pop_frame();
        return _t2045;
    } else {
    GemVal _t2046[] = {gem_v__match_31};
    GemVal _t2048;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2046, 1), gem_string("table")))) {
        _t2048 = gem_eq(gem_type_fn(NULL, _t2046, 1), gem_string("table"));
    } else {
        GemVal _t2047[] = {gem_v__match_31, gem_string("tag")};
        _t2048 = gem_has_key_fn(NULL, _t2047, 2);
    }
    GemVal _t2049;
    if (!gem_truthy(_t2048)) {
        _t2049 = _t2048;
    } else {
        _t2049 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2051;
    if (!gem_truthy(_t2049)) {
        _t2051 = _t2049;
    } else {
        GemVal _t2050[] = {gem_v__match_31, gem_string("value")};
        _t2051 = gem_has_key_fn(NULL, _t2050, 2);
    }
    if (gem_truthy(_t2051)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 514 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 515 "compiler/main.gem"
    GemVal _t2052[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2053 = (*gem_v_walk_captures_node);
            GemVal _t2054 = _t2053.fn(_t2053.env, _t2052, 3);
            gem_pop_frame();
            return _t2054;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2055[] = {gem_v__match_31};
    GemVal _t2057;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2055, 1), gem_string("table")))) {
        _t2057 = gem_eq(gem_type_fn(NULL, _t2055, 1), gem_string("table"));
    } else {
        GemVal _t2056[] = {gem_v__match_31, gem_string("tag")};
        _t2057 = gem_has_key_fn(NULL, _t2056, 2);
    }
    GemVal _t2058;
    if (!gem_truthy(_t2057)) {
        _t2058 = _t2057;
    } else {
        _t2058 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2060;
    if (!gem_truthy(_t2058)) {
        _t2060 = _t2058;
    } else {
        GemVal _t2059[] = {gem_v__match_31, gem_string("value")};
        _t2060 = gem_has_key_fn(NULL, _t2059, 2);
    }
    if (gem_truthy(_t2060)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 518 "compiler/main.gem"
    GemVal _t2061[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2062 = (*gem_v_walk_captures_node);
        GemVal _t2063 = _t2062.fn(_t2062.env, _t2061, 3);
        gem_pop_frame();
        return _t2063;
    } else {
    GemVal _t2064[] = {gem_v__match_31};
    GemVal _t2066;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2064, 1), gem_string("table")))) {
        _t2066 = gem_eq(gem_type_fn(NULL, _t2064, 1), gem_string("table"));
    } else {
        GemVal _t2065[] = {gem_v__match_31, gem_string("tag")};
        _t2066 = gem_has_key_fn(NULL, _t2065, 2);
    }
    GemVal _t2067;
    if (!gem_truthy(_t2066)) {
        _t2067 = _t2066;
    } else {
        _t2067 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2069;
    if (!gem_truthy(_t2067)) {
        _t2069 = _t2067;
    } else {
        GemVal _t2068[] = {gem_v__match_31, gem_string("value")};
        _t2069 = gem_has_key_fn(NULL, _t2068, 2);
    }
    if (gem_truthy(_t2069)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 520 "compiler/main.gem"
    GemVal _t2070[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2071 = (*gem_v_walk_captures_node);
        GemVal _t2072 = _t2071.fn(_t2071.env, _t2070, 3);
        gem_pop_frame();
        return _t2072;
    } else {
        {
#line 523 "compiler/main.gem"
            GemVal gem_v__for_tbl_39 = gem_v_node;
#line 523 "compiler/main.gem"
    GemVal _t2073[] = {gem_v__for_tbl_39};
            GemVal gem_v__for_keys_39 = gem_keys_fn(NULL, _t2073, 1);
#line 523 "compiler/main.gem"
            GemVal gem_v__for_i_39 = gem_int(0);
#line 523 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t2074[] = {gem_v__for_keys_39};
                if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_len_fn(NULL, _t2074, 1)))) break;
#line 523 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_39, gem_v__for_i_39);
#line 523 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_39, gem_table_get(gem_v__for_keys_39, gem_v__for_i_39));
#line 523 "compiler/main.gem"
                gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 524 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 525 "compiler/main.gem"
    GemVal _t2075[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2075, 1), gem_string("table")))) {
#line 526 "compiler/main.gem"
    GemVal _t2076[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2076, 1);
#line 527 "compiler/main.gem"
    GemVal _t2077[] = {gem_v_vks};
    GemVal _t2079;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2077, 1), gem_int(0)))) {
        _t2079 = gem_gt(gem_len_fn(NULL, _t2077, 1), gem_int(0));
    } else {
        GemVal _t2078[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2079 = gem_eq(gem_type_fn(NULL, _t2078, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2079)) {
#line 528 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 529 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2080[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2080, 1)))) break;
#line 530 "compiler/main.gem"
    GemVal _t2081[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2081, 1), gem_string("table")))) {
#line 531 "compiler/main.gem"
    GemVal _t2082[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2083 = (*gem_v_walk_captures_node);
                                    (void)(_t2083.fn(_t2083.env, _t2082, 3));
                                }
#line 533 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 536 "compiler/main.gem"
    GemVal _t2084[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t2085 = (*gem_v_walk_captures_node);
                            (void)(_t2085.fn(_t2085.env, _t2084, 3));
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
#line 545 "compiler/main.gem"
    GemVal _t2087 = gem_table_new();
    GemVal _t2088[] = {gem_v_scope_vars, _t2087};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t2088, 2);
    {
#line 546 "compiler/main.gem"
        GemVal gem_v__for_i_40 = gem_int(0);
#line 546 "compiler/main.gem"
    GemVal _t2089[] = {gem_v_stmts};
        GemVal gem_v__for_limit_40 = gem_len_fn(NULL, _t2089, 1);
#line 546 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 546 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_40;
#line 546 "compiler/main.gem"
            gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 547 "compiler/main.gem"
    GemVal _t2090[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t2091 = (*gem_v_walk_captures_node);
            (void)(_t2091.fn(_t2091.env, _t2090, 3));
#line 548 "compiler/main.gem"
    GemVal _t2092[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t2094;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2092, 1), gem_string("table")))) {
        _t2094 = gem_eq(gem_type_fn(NULL, _t2092, 1), gem_string("table"));
    } else {
        GemVal _t2093 = gem_table_get(gem_v_stmts, gem_v_i);
        _t2094 = gem_eq(gem_table_get(_t2093, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t2094)) {
#line 549 "compiler/main.gem"
    GemVal _t2095 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2096[] = {gem_v_sv, gem_table_get(_t2095, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t2096, 2));
            }
#line 551 "compiler/main.gem"
    GemVal _t2097[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv};
            (void)(gem_fn_collect_block_lets(NULL, _t2097, 2));
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
#line 556 "compiler/main.gem"
    GemVal _t2099 = gem_table_new();
    GemVal gem_v_captured = _t2099;
#line 557 "compiler/main.gem"
    GemVal _t2100[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2101 = (*gem_v_walk_captures);
    (void)(_t2101.fn(_t2101.env, _t2100, 3));
    GemVal _t2102 = gem_v_captured;
    gem_pop_frame();
    return _t2102;
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
    GemVal *gem_v_local_names;
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
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal *gem_v_top_level_boxed = _cls->gem_v_top_level_boxed;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_32", "compiler/main.gem", 0);
#line 586 "compiler/main.gem"
    GemVal gem_v__match_41 = gem_v_node;
    GemVal _t2104[] = {gem_v__match_41};
    GemVal _t2106;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2104, 1), gem_string("table")))) {
        _t2106 = gem_eq(gem_type_fn(NULL, _t2104, 1), gem_string("table"));
    } else {
        GemVal _t2105[] = {gem_v__match_41, gem_string("tag")};
        _t2106 = gem_has_key_fn(NULL, _t2105, 2);
    }
    GemVal _t2107;
    if (!gem_truthy(_t2106)) {
        _t2107 = _t2106;
    } else {
        _t2107 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("int"));
    }
    GemVal _t2109;
    if (!gem_truthy(_t2107)) {
        _t2109 = _t2107;
    } else {
        GemVal _t2108[] = {gem_v__match_41, gem_string("value")};
        _t2109 = gem_has_key_fn(NULL, _t2108, 2);
    }
    if (gem_truthy(_t2109)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 588 "compiler/main.gem"
    GemVal _t2110 = gem_table_new();
    GemVal _t2111[] = {gem_v_value};
    gem_table_set(_t2110, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t2111, 1)), gem_string(")")));
    gem_table_set(_t2110, gem_string("setup"), gem_string(""));
        GemVal _t2112 = _t2110;
        gem_pop_frame();
        return _t2112;
    } else {
    GemVal _t2113[] = {gem_v__match_41};
    GemVal _t2115;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2113, 1), gem_string("table")))) {
        _t2115 = gem_eq(gem_type_fn(NULL, _t2113, 1), gem_string("table"));
    } else {
        GemVal _t2114[] = {gem_v__match_41, gem_string("tag")};
        _t2115 = gem_has_key_fn(NULL, _t2114, 2);
    }
    GemVal _t2116;
    if (!gem_truthy(_t2115)) {
        _t2116 = _t2115;
    } else {
        _t2116 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("float"));
    }
    GemVal _t2118;
    if (!gem_truthy(_t2116)) {
        _t2118 = _t2116;
    } else {
        GemVal _t2117[] = {gem_v__match_41, gem_string("value")};
        _t2118 = gem_has_key_fn(NULL, _t2117, 2);
    }
    if (gem_truthy(_t2118)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 590 "compiler/main.gem"
    GemVal _t2119 = gem_table_new();
    GemVal _t2120[] = {gem_v_value};
    GemVal _t2121[] = {gem_fn_format_float(NULL, _t2120, 1)};
    gem_table_set(_t2119, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t2121, 1)), gem_string(")")));
    gem_table_set(_t2119, gem_string("setup"), gem_string(""));
        GemVal _t2122 = _t2119;
        gem_pop_frame();
        return _t2122;
    } else {
    GemVal _t2123[] = {gem_v__match_41};
    GemVal _t2125;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2123, 1), gem_string("table")))) {
        _t2125 = gem_eq(gem_type_fn(NULL, _t2123, 1), gem_string("table"));
    } else {
        GemVal _t2124[] = {gem_v__match_41, gem_string("tag")};
        _t2125 = gem_has_key_fn(NULL, _t2124, 2);
    }
    GemVal _t2126;
    if (!gem_truthy(_t2125)) {
        _t2126 = _t2125;
    } else {
        _t2126 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("string"));
    }
    GemVal _t2128;
    if (!gem_truthy(_t2126)) {
        _t2128 = _t2126;
    } else {
        GemVal _t2127[] = {gem_v__match_41, gem_string("value")};
        _t2128 = gem_has_key_fn(NULL, _t2127, 2);
    }
    if (gem_truthy(_t2128)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 592 "compiler/main.gem"
    GemVal _t2129[] = {gem_v_value};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2129, 1);
#line 593 "compiler/main.gem"
    GemVal _t2130 = gem_table_new();
    GemVal _t2131[] = {gem_v_escaped};
    gem_table_set(_t2130, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t2131, 1)), gem_string("\")")));
    gem_table_set(_t2130, gem_string("setup"), gem_string(""));
        GemVal _t2132 = _t2130;
        gem_pop_frame();
        return _t2132;
    } else {
    GemVal _t2133[] = {gem_v__match_41};
    GemVal _t2135;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2133, 1), gem_string("table")))) {
        _t2135 = gem_eq(gem_type_fn(NULL, _t2133, 1), gem_string("table"));
    } else {
        GemVal _t2134[] = {gem_v__match_41, gem_string("tag")};
        _t2135 = gem_has_key_fn(NULL, _t2134, 2);
    }
    GemVal _t2136;
    if (!gem_truthy(_t2135)) {
        _t2136 = _t2135;
    } else {
        _t2136 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t2138;
    if (!gem_truthy(_t2136)) {
        _t2138 = _t2136;
    } else {
        GemVal _t2137[] = {gem_v__match_41, gem_string("parts")};
        _t2138 = gem_has_key_fn(NULL, _t2137, 2);
    }
    if (gem_truthy(_t2138)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_parts = gem_table_get(gem_v__match_41, gem_string("parts"));
#line 595 "compiler/main.gem"
    GemVal _t2139[] = {gem_v_parts};
        if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2139, 1), gem_int(0)))) {
#line 596 "compiler/main.gem"
    GemVal _t2140 = gem_table_new();
    gem_table_set(_t2140, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t2140, gem_string("setup"), gem_string(""));
            GemVal _t2141 = _t2140;
            gem_pop_frame();
            return _t2141;
        }
#line 598 "compiler/main.gem"
        GemVal gem_v_setup = gem_string("");
#line 599 "compiler/main.gem"
    GemVal _t2142 = gem_table_new();
        GemVal gem_v_compiled = _t2142;
#line 600 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 600 "compiler/main.gem"
    GemVal _t2143[] = {gem_v_parts};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2143, 1);
#line 600 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 600 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_42;
#line 600 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 601 "compiler/main.gem"
            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 602 "compiler/main.gem"
    GemVal _t2144[] = {gem_v_part};
    GemVal _t2145 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2145.fn(_t2145.env, _t2144, 1);
#line 603 "compiler/main.gem"
    GemVal _t2146 = gem_v_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2146, gem_string("setup")));
#line 604 "compiler/main.gem"
    GemVal _t2147 = gem_v_part;
            if (gem_truthy(gem_eq(gem_table_get(_t2147, gem_string("tag")), gem_string("string")))) {
#line 605 "compiler/main.gem"
    GemVal _t2148 = gem_v_r;
    GemVal _t2149[] = {gem_v_compiled, gem_table_get(_t2148, gem_string("expr"))};
                (void)(gem_push_fn(NULL, _t2149, 2));
            } else {
#line 607 "compiler/main.gem"
    GemVal _t2150 = (*gem_v_tmp);
                GemVal gem_v_ts = _t2150.fn(_t2150.env, NULL, 0);
#line 608 "compiler/main.gem"
    GemVal _t2151[] = {gem_v_ts};
    GemVal _t2152 = gem_v_r;
    GemVal _t2153[] = {gem_table_get(_t2152, gem_string("expr"))};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2151, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2153, 1)), gem_string("};\n")));
#line 609 "compiler/main.gem"
    GemVal _t2154[] = {gem_v_ts};
    GemVal _t2155[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t2154, 1)), gem_string(", 1)"))};
                (void)(gem_push_fn(NULL, _t2155, 2));
            }
        }

#line 612 "compiler/main.gem"
        GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
#line 613 "compiler/main.gem"
        GemVal gem_v__for_i_43 = gem_int(1);
#line 613 "compiler/main.gem"
    GemVal _t2156[] = {gem_v_compiled};
        GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t2156, 1);
#line 613 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 613 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_43;
#line 613 "compiler/main.gem"
            gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 614 "compiler/main.gem"
    GemVal _t2157[] = {gem_v_acc};
    GemVal _t2158[] = {gem_table_get(gem_v_compiled, gem_v_i)};
            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2157, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2158, 1)), gem_string(")"));
        }

#line 616 "compiler/main.gem"
    GemVal _t2159 = gem_table_new();
    gem_table_set(_t2159, gem_string("expr"), gem_v_acc);
    gem_table_set(_t2159, gem_string("setup"), gem_v_setup);
        GemVal _t2160 = _t2159;
        gem_pop_frame();
        return _t2160;
    } else {
    GemVal _t2161[] = {gem_v__match_41};
    GemVal _t2163;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2161, 1), gem_string("table")))) {
        _t2163 = gem_eq(gem_type_fn(NULL, _t2161, 1), gem_string("table"));
    } else {
        GemVal _t2162[] = {gem_v__match_41, gem_string("tag")};
        _t2163 = gem_has_key_fn(NULL, _t2162, 2);
    }
    GemVal _t2164;
    if (!gem_truthy(_t2163)) {
        _t2164 = _t2163;
    } else {
        _t2164 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("bool"));
    }
    GemVal _t2166;
    if (!gem_truthy(_t2164)) {
        _t2166 = _t2164;
    } else {
        GemVal _t2165[] = {gem_v__match_41, gem_string("value")};
        _t2166 = gem_has_key_fn(NULL, _t2165, 2);
    }
    if (gem_truthy(_t2166)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 618 "compiler/main.gem"
        if (gem_truthy(gem_v_value)) {
#line 619 "compiler/main.gem"
    GemVal _t2167 = gem_table_new();
    gem_table_set(_t2167, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t2167, gem_string("setup"), gem_string(""));
            GemVal _t2168 = _t2167;
            gem_pop_frame();
            return _t2168;
        }
#line 621 "compiler/main.gem"
    GemVal _t2169 = gem_table_new();
    gem_table_set(_t2169, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t2169, gem_string("setup"), gem_string(""));
        GemVal _t2170 = _t2169;
        gem_pop_frame();
        return _t2170;
    } else {
    GemVal _t2171[] = {gem_v__match_41};
    GemVal _t2173;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2171, 1), gem_string("table")))) {
        _t2173 = gem_eq(gem_type_fn(NULL, _t2171, 1), gem_string("table"));
    } else {
        GemVal _t2172[] = {gem_v__match_41, gem_string("tag")};
        _t2173 = gem_has_key_fn(NULL, _t2172, 2);
    }
    GemVal _t2174;
    if (!gem_truthy(_t2173)) {
        _t2174 = _t2173;
    } else {
        _t2174 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("nil"));
    }
    if (gem_truthy(_t2174)) {
#line 623 "compiler/main.gem"
    GemVal _t2175 = gem_table_new();
    gem_table_set(_t2175, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t2175, gem_string("setup"), gem_string(""));
        GemVal _t2176 = _t2175;
        gem_pop_frame();
        return _t2176;
    } else {
    GemVal _t2177[] = {gem_v__match_41};
    GemVal _t2179;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2177, 1), gem_string("table")))) {
        _t2179 = gem_eq(gem_type_fn(NULL, _t2177, 1), gem_string("table"));
    } else {
        GemVal _t2178[] = {gem_v__match_41, gem_string("tag")};
        _t2179 = gem_has_key_fn(NULL, _t2178, 2);
    }
    GemVal _t2180;
    if (!gem_truthy(_t2179)) {
        _t2180 = _t2179;
    } else {
        _t2180 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("var"));
    }
    GemVal _t2182;
    if (!gem_truthy(_t2180)) {
        _t2182 = _t2180;
    } else {
        GemVal _t2181[] = {gem_v__match_41, gem_string("name")};
        _t2182 = gem_has_key_fn(NULL, _t2181, 2);
    }
    if (gem_truthy(_t2182)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_41, gem_string("name"));
#line 625 "compiler/main.gem"
    GemVal _t2183[] = {(*gem_v_local_names), gem_v_name};
        if (gem_truthy(gem_not(gem_fn_set_contains(NULL, _t2183, 2)))) {
#line 626 "compiler/main.gem"
    GemVal _t2184[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t2184, 2))) {
#line 627 "compiler/main.gem"
    GemVal _t2185 = gem_table_new();
    GemVal _t2186[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name)};
    gem_table_set(_t2185, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2186, 1)), gem_string(", NULL)")));
    gem_table_set(_t2185, gem_string("setup"), gem_string(""));
                GemVal _t2187 = _t2185;
                gem_pop_frame();
                return _t2187;
            } else {
#line 628 "compiler/main.gem"
    GemVal _t2188[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t2188, 2))) {
#line 629 "compiler/main.gem"
    GemVal _t2189 = gem_table_new();
    GemVal _t2190[] = {gem_v_name};
    gem_table_set(_t2189, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t2190, 1)), gem_string(", NULL)")));
    gem_table_set(_t2189, gem_string("setup"), gem_string(""));
                    GemVal _t2191 = _t2189;
                    gem_pop_frame();
                    return _t2191;
                }
            }
        }
#line 632 "compiler/main.gem"
    GemVal _t2192[] = {gem_v_name};
    GemVal _t2193 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2193.fn(_t2193.env, _t2192, 1);
#line 633 "compiler/main.gem"
    GemVal _t2194[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2196;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2194, 2))) {
        _t2196 = gem_fn_set_contains(NULL, _t2194, 2);
    } else {
        GemVal _t2195[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2196 = gem_fn_set_contains(NULL, _t2195, 2);
    }
        if (gem_truthy(_t2196)) {
#line 634 "compiler/main.gem"
    GemVal _t2197 = gem_table_new();
    GemVal _t2198[] = {gem_v_mname};
    gem_table_set(_t2197, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t2198, 1)), gem_string(")")));
    gem_table_set(_t2197, gem_string("setup"), gem_string(""));
            GemVal _t2199 = _t2197;
            gem_pop_frame();
            return _t2199;
        }
#line 636 "compiler/main.gem"
    GemVal _t2200 = gem_table_new();
    gem_table_set(_t2200, gem_string("expr"), gem_v_mname);
    gem_table_set(_t2200, gem_string("setup"), gem_string(""));
        GemVal _t2201 = _t2200;
        gem_pop_frame();
        return _t2201;
    } else {
    GemVal _t2202[] = {gem_v__match_41};
    GemVal _t2204;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2202, 1), gem_string("table")))) {
        _t2204 = gem_eq(gem_type_fn(NULL, _t2202, 1), gem_string("table"));
    } else {
        GemVal _t2203[] = {gem_v__match_41, gem_string("tag")};
        _t2204 = gem_has_key_fn(NULL, _t2203, 2);
    }
    GemVal _t2205;
    if (!gem_truthy(_t2204)) {
        _t2205 = _t2204;
    } else {
        _t2205 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2205)) {
#line 638 "compiler/main.gem"
    GemVal _t2206[] = {gem_v_node};
    GemVal _t2207 = (*gem_v_compile_call);
        GemVal _t2208 = _t2207.fn(_t2207.env, _t2206, 1);
        gem_pop_frame();
        return _t2208;
    } else {
    GemVal _t2209[] = {gem_v__match_41};
    GemVal _t2211;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2209, 1), gem_string("table")))) {
        _t2211 = gem_eq(gem_type_fn(NULL, _t2209, 1), gem_string("table"));
    } else {
        GemVal _t2210[] = {gem_v__match_41, gem_string("tag")};
        _t2211 = gem_has_key_fn(NULL, _t2210, 2);
    }
    GemVal _t2212;
    if (!gem_truthy(_t2211)) {
        _t2212 = _t2211;
    } else {
        _t2212 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("binop"));
    }
    if (gem_truthy(_t2212)) {
#line 640 "compiler/main.gem"
    GemVal _t2213[] = {gem_v_node};
    GemVal _t2214 = (*gem_v_compile_binop);
        GemVal _t2215 = _t2214.fn(_t2214.env, _t2213, 1);
        gem_pop_frame();
        return _t2215;
    } else {
    GemVal _t2216[] = {gem_v__match_41};
    GemVal _t2218;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2216, 1), gem_string("table")))) {
        _t2218 = gem_eq(gem_type_fn(NULL, _t2216, 1), gem_string("table"));
    } else {
        GemVal _t2217[] = {gem_v__match_41, gem_string("tag")};
        _t2218 = gem_has_key_fn(NULL, _t2217, 2);
    }
    GemVal _t2219;
    if (!gem_truthy(_t2218)) {
        _t2219 = _t2218;
    } else {
        _t2219 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t2221;
    if (!gem_truthy(_t2219)) {
        _t2221 = _t2219;
    } else {
        GemVal _t2220[] = {gem_v__match_41, gem_string("expr")};
        _t2221 = gem_has_key_fn(NULL, _t2220, 2);
    }
    GemVal _t2223;
    if (!gem_truthy(_t2221)) {
        _t2223 = _t2221;
    } else {
        GemVal _t2222[] = {gem_v__match_41, gem_string("op")};
        _t2223 = gem_has_key_fn(NULL, _t2222, 2);
    }
    if (gem_truthy(_t2223)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_uexpr = gem_table_get(gem_v__match_41, gem_string("expr"));
#line 0 "compiler/main.gem"
        GemVal gem_v_op = gem_table_get(gem_v__match_41, gem_string("op"));
#line 642 "compiler/main.gem"
    GemVal _t2224[] = {gem_v_uexpr};
    GemVal _t2225 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2225.fn(_t2225.env, _t2224, 1);
#line 643 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 644 "compiler/main.gem"
    GemVal _t2226 = gem_table_new();
    GemVal _t2227 = gem_v_r;
    GemVal _t2228[] = {gem_table_get(_t2227, gem_string("expr"))};
    gem_table_set(_t2226, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t2228, 1)), gem_string(")")));
    GemVal _t2229 = gem_v_r;
    gem_table_set(_t2226, gem_string("setup"), gem_table_get(_t2229, gem_string("setup")));
            GemVal _t2230 = _t2226;
            gem_pop_frame();
            return _t2230;
        } else {
#line 645 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("not")))) {
#line 646 "compiler/main.gem"
    GemVal _t2231 = gem_table_new();
    GemVal _t2232 = gem_v_r;
    GemVal _t2233[] = {gem_table_get(_t2232, gem_string("expr"))};
    gem_table_set(_t2231, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t2233, 1)), gem_string(")")));
    GemVal _t2234 = gem_v_r;
    gem_table_set(_t2231, gem_string("setup"), gem_table_get(_t2234, gem_string("setup")));
                GemVal _t2235 = _t2231;
                gem_pop_frame();
                return _t2235;
            }
        }
#line 648 "compiler/main.gem"
    GemVal _t2236[] = {gem_v_op};
    GemVal _t2237[] = {gem_add(gem_string("unknown unary operator: "), gem_to_string_fn(NULL, _t2236, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 648, _t2237, 1));
    } else {
    GemVal _t2238[] = {gem_v__match_41};
    GemVal _t2240;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2238, 1), gem_string("table")))) {
        _t2240 = gem_eq(gem_type_fn(NULL, _t2238, 1), gem_string("table"));
    } else {
        GemVal _t2239[] = {gem_v__match_41, gem_string("tag")};
        _t2240 = gem_has_key_fn(NULL, _t2239, 2);
    }
    GemVal _t2241;
    if (!gem_truthy(_t2240)) {
        _t2241 = _t2240;
    } else {
        _t2241 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("anon_fn"));
    }
    if (gem_truthy(_t2241)) {
#line 650 "compiler/main.gem"
    GemVal _t2242[] = {gem_v_node};
    GemVal _t2243 = (*gem_v_compile_anon_fn);
        GemVal _t2244 = _t2243.fn(_t2243.env, _t2242, 1);
        gem_pop_frame();
        return _t2244;
    } else {
    GemVal _t2245[] = {gem_v__match_41};
    GemVal _t2247;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2245, 1), gem_string("table")))) {
        _t2247 = gem_eq(gem_type_fn(NULL, _t2245, 1), gem_string("table"));
    } else {
        GemVal _t2246[] = {gem_v__match_41, gem_string("tag")};
        _t2247 = gem_has_key_fn(NULL, _t2246, 2);
    }
    GemVal _t2248;
    if (!gem_truthy(_t2247)) {
        _t2248 = _t2247;
    } else {
        _t2248 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("table"));
    }
    if (gem_truthy(_t2248)) {
#line 652 "compiler/main.gem"
    GemVal _t2249[] = {gem_v_node};
    GemVal _t2250 = (*gem_v_compile_table);
        GemVal _t2251 = _t2250.fn(_t2250.env, _t2249, 1);
        gem_pop_frame();
        return _t2251;
    } else {
    GemVal _t2252[] = {gem_v__match_41};
    GemVal _t2254;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2252, 1), gem_string("table")))) {
        _t2254 = gem_eq(gem_type_fn(NULL, _t2252, 1), gem_string("table"));
    } else {
        GemVal _t2253[] = {gem_v__match_41, gem_string("tag")};
        _t2254 = gem_has_key_fn(NULL, _t2253, 2);
    }
    GemVal _t2255;
    if (!gem_truthy(_t2254)) {
        _t2255 = _t2254;
    } else {
        _t2255 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("array"));
    }
    if (gem_truthy(_t2255)) {
#line 654 "compiler/main.gem"
    GemVal _t2256[] = {gem_v_node};
    GemVal _t2257 = (*gem_v_compile_array);
        GemVal _t2258 = _t2257.fn(_t2257.env, _t2256, 1);
        gem_pop_frame();
        return _t2258;
    } else {
    GemVal _t2259[] = {gem_v__match_41};
    GemVal _t2261;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2259, 1), gem_string("table")))) {
        _t2261 = gem_eq(gem_type_fn(NULL, _t2259, 1), gem_string("table"));
    } else {
        GemVal _t2260[] = {gem_v__match_41, gem_string("tag")};
        _t2261 = gem_has_key_fn(NULL, _t2260, 2);
    }
    GemVal _t2262;
    if (!gem_truthy(_t2261)) {
        _t2262 = _t2261;
    } else {
        _t2262 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t2264;
    if (!gem_truthy(_t2262)) {
        _t2264 = _t2262;
    } else {
        GemVal _t2263[] = {gem_v__match_41, gem_string("object")};
        _t2264 = gem_has_key_fn(NULL, _t2263, 2);
    }
    GemVal _t2266;
    if (!gem_truthy(_t2264)) {
        _t2266 = _t2264;
    } else {
        GemVal _t2265[] = {gem_v__match_41, gem_string("field")};
        _t2266 = gem_has_key_fn(NULL, _t2265, 2);
    }
    if (gem_truthy(_t2266)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_41, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_41, gem_string("field"));
#line 656 "compiler/main.gem"
    GemVal _t2267[] = {gem_v_object};
    GemVal _t2268 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2268.fn(_t2268.env, _t2267, 1);
#line 657 "compiler/main.gem"
    GemVal _t2269 = (*gem_v_tmp);
        GemVal gem_v_t = _t2269.fn(_t2269.env, NULL, 0);
#line 658 "compiler/main.gem"
    GemVal _t2270 = gem_v_r;
    GemVal _t2271[] = {gem_v_t};
    GemVal _t2272 = gem_v_r;
    GemVal _t2273[] = {gem_table_get(_t2272, gem_string("expr"))};
        GemVal gem_v_setup = gem_add(gem_table_get(_t2270, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2271, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2273, 1)), gem_string(";\n")));
#line 659 "compiler/main.gem"
    GemVal _t2274[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2274, 1);
#line 660 "compiler/main.gem"
    GemVal _t2275 = gem_table_new();
    GemVal _t2276[] = {gem_v_t};
    GemVal _t2277[] = {gem_v_escaped};
    gem_table_set(_t2275, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2276, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2277, 1)), gem_string("\"))")));
    gem_table_set(_t2275, gem_string("setup"), gem_v_setup);
        GemVal _t2278 = _t2275;
        gem_pop_frame();
        return _t2278;
    } else {
    GemVal _t2279[] = {gem_v__match_41};
    GemVal _t2281;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2279, 1), gem_string("table")))) {
        _t2281 = gem_eq(gem_type_fn(NULL, _t2279, 1), gem_string("table"));
    } else {
        GemVal _t2280[] = {gem_v__match_41, gem_string("tag")};
        _t2281 = gem_has_key_fn(NULL, _t2280, 2);
    }
    GemVal _t2282;
    if (!gem_truthy(_t2281)) {
        _t2282 = _t2281;
    } else {
        _t2282 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("index"));
    }
    GemVal _t2284;
    if (!gem_truthy(_t2282)) {
        _t2284 = _t2282;
    } else {
        GemVal _t2283[] = {gem_v__match_41, gem_string("object")};
        _t2284 = gem_has_key_fn(NULL, _t2283, 2);
    }
    GemVal _t2286;
    if (!gem_truthy(_t2284)) {
        _t2286 = _t2284;
    } else {
        GemVal _t2285[] = {gem_v__match_41, gem_string("key")};
        _t2286 = gem_has_key_fn(NULL, _t2285, 2);
    }
    if (gem_truthy(_t2286)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_41, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_41, gem_string("key"));
#line 662 "compiler/main.gem"
    GemVal _t2287[] = {gem_v_object};
    GemVal _t2288 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2288.fn(_t2288.env, _t2287, 1);
#line 663 "compiler/main.gem"
    GemVal _t2289[] = {gem_v_key};
    GemVal _t2290 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2290.fn(_t2290.env, _t2289, 1);
#line 664 "compiler/main.gem"
    GemVal _t2291 = gem_table_new();
    GemVal _t2292 = gem_v_obj_r;
    GemVal _t2293[] = {gem_table_get(_t2292, gem_string("expr"))};
    GemVal _t2294 = gem_v_key_r;
    GemVal _t2295[] = {gem_table_get(_t2294, gem_string("expr"))};
    gem_table_set(_t2291, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2293, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2295, 1)), gem_string(")")));
    GemVal _t2296 = gem_v_obj_r;
    GemVal _t2297 = gem_v_key_r;
    gem_table_set(_t2291, gem_string("setup"), gem_add(gem_table_get(_t2296, gem_string("setup")), gem_table_get(_t2297, gem_string("setup"))));
        GemVal _t2298 = _t2291;
        gem_pop_frame();
        return _t2298;
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
#line 667 "compiler/main.gem"
    GemVal _t2299 = gem_v_node;
    GemVal _t2300[] = {gem_table_get(_t2299, gem_string("tag"))};
    GemVal _t2301[] = {gem_add(gem_string("unknown expression node: "), gem_to_string_fn(NULL, _t2300, 1))};
    GemVal _t2302 = gem_error_at_fn("compiler/main.gem", 667, _t2301, 1);
    gem_pop_frame();
    return _t2302;
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
#line 673 "compiler/main.gem"
    GemVal _t2304 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t2304, gem_string("entries"));
#line 674 "compiler/main.gem"
    GemVal _t2305 = (*gem_v_tmp);
    GemVal gem_v_t = _t2305.fn(_t2305.env, NULL, 0);
#line 675 "compiler/main.gem"
    GemVal _t2306[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2306, 1)), gem_string(" = gem_table_new();\n"));
#line 676 "compiler/main.gem"
    GemVal gem_v__for_items_44 = gem_v_entries;
#line 676 "compiler/main.gem"
    GemVal gem_v__for_i_44 = gem_int(0);
#line 676 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2307[] = {gem_v__for_items_44};
        if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_len_fn(NULL, _t2307, 1)))) break;
#line 676 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_44, gem_v__for_i_44);
#line 676 "compiler/main.gem"
        gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 677 "compiler/main.gem"
    GemVal _t2308 = gem_v_entry;
    GemVal _t2309[] = {gem_table_get(_t2308, gem_string("value"))};
    GemVal _t2310 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2310.fn(_t2310.env, _t2309, 1);
#line 678 "compiler/main.gem"
    GemVal _t2311 = gem_v_entry;
    GemVal _t2312[] = {gem_table_get(_t2311, gem_string("key"))};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2312, 1);
#line 679 "compiler/main.gem"
    GemVal _t2313 = gem_v_val_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2313, gem_string("setup")));
#line 680 "compiler/main.gem"
    GemVal _t2314[] = {gem_v_t};
    GemVal _t2315[] = {gem_v_escaped};
    GemVal _t2316 = gem_v_val_r;
    GemVal _t2317[] = {gem_table_get(_t2316, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2314, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2315, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2317, 1)), gem_string(");\n")));
    }

    GemVal _t2318 = gem_table_new();
    gem_table_set(_t2318, gem_string("expr"), gem_v_t);
    gem_table_set(_t2318, gem_string("setup"), gem_v_setup);
    GemVal _t2319 = _t2318;
    gem_pop_frame();
    return _t2319;
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
#line 688 "compiler/main.gem"
    GemVal _t2321 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t2321, gem_string("elements"));
#line 689 "compiler/main.gem"
    GemVal _t2322 = (*gem_v_tmp);
    GemVal gem_v_t = _t2322.fn(_t2322.env, NULL, 0);
#line 690 "compiler/main.gem"
    GemVal _t2323[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2323, 1)), gem_string(" = gem_table_new();\n"));
#line 691 "compiler/main.gem"
    GemVal gem_v__for_i_45 = gem_int(0);
#line 691 "compiler/main.gem"
    GemVal _t2324[] = {gem_v_elements};
    GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2324, 1);
#line 691 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 691 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_45;
#line 691 "compiler/main.gem"
        gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 692 "compiler/main.gem"
    GemVal _t2325[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t2326 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t2326.fn(_t2326.env, _t2325, 1);
#line 693 "compiler/main.gem"
    GemVal _t2327 = gem_v_elem_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2327, gem_string("setup")));
#line 694 "compiler/main.gem"
    GemVal _t2328[] = {gem_v_t};
    GemVal _t2329[] = {gem_v_i};
    GemVal _t2330 = gem_v_elem_r;
    GemVal _t2331[] = {gem_table_get(_t2330, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2328, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t2329, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t2331, 1)), gem_string(");\n")));
    }

    GemVal _t2332 = gem_table_new();
    gem_table_set(_t2332, gem_string("expr"), gem_v_t);
    gem_table_set(_t2332, gem_string("setup"), gem_v_setup);
    GemVal _t2333 = _t2332;
    gem_pop_frame();
    return _t2333;
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
#line 702 "compiler/main.gem"
    GemVal _t2335 = gem_v_node;
    GemVal _t2336[] = {gem_table_get(_t2335, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t2336, 1);
#line 703 "compiler/main.gem"
    GemVal _t2337 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2337, gem_string("rest_param")), GEM_NIL))) {
#line 704 "compiler/main.gem"
    GemVal _t2338 = gem_v_node;
    GemVal _t2339[] = {gem_v_inner_defined, gem_table_get(_t2338, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t2339, 2));
    }
#line 706 "compiler/main.gem"
    GemVal _t2340 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2340, gem_string("block_param")), GEM_NIL))) {
#line 707 "compiler/main.gem"
    GemVal _t2341 = gem_v_node;
    GemVal _t2342[] = {gem_v_inner_defined, gem_table_get(_t2341, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t2342, 2));
    }
#line 709 "compiler/main.gem"
    GemVal _t2343 = gem_v_node;
    GemVal _t2344[] = {gem_table_get(_t2343, gem_string("body")), gem_v_inner_defined};
    GemVal _t2345 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t2345.fn(_t2345.env, _t2344, 2);
#line 710 "compiler/main.gem"
    GemVal _t2346[] = {gem_v_free};
    GemVal _t2347[] = {gem_keys_fn(NULL, _t2346, 1)};
    GemVal gem_v_captures = gem_sort_fn(NULL, _t2347, 1);
#line 712 "compiler/main.gem"
    GemVal _t2348[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2348, 1), gem_int(0)))) {
#line 713 "compiler/main.gem"
    GemVal _t2349 = gem_table_new();
    GemVal _t2350[] = {gem_v_node, _t2349};
    GemVal _t2351 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t2351.fn(_t2351.env, _t2350, 2);
#line 714 "compiler/main.gem"
    GemVal _t2352 = gem_table_new();
    GemVal _t2353 = gem_v_result;
    GemVal _t2354[] = {gem_table_get(_t2353, gem_string("fn_name"))};
    gem_table_set(_t2352, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2354, 1)), gem_string(", NULL)")));
    gem_table_set(_t2352, gem_string("setup"), gem_string(""));
        GemVal _t2355 = _t2352;
        gem_pop_frame();
        return _t2355;
    }
#line 717 "compiler/main.gem"
    GemVal _t2356[] = {gem_v_node, gem_v_captures};
    GemVal _t2357 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t2357.fn(_t2357.env, _t2356, 2);
#line 719 "compiler/main.gem"
    GemVal gem_v_setup = gem_string("");
#line 720 "compiler/main.gem"
    GemVal _t2358 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t2358.fn(_t2358.env, NULL, 0);
#line 721 "compiler/main.gem"
    GemVal _t2359 = gem_v_result;
    GemVal _t2360[] = {gem_table_get(_t2359, gem_string("struct_name"))};
    GemVal _t2361[] = {gem_v_env_tmp};
    GemVal _t2362 = gem_v_result;
    GemVal _t2363[] = {gem_table_get(_t2362, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2360, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t2361, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t2363, 1)), gem_string("));\n")));
#line 722 "compiler/main.gem"
    GemVal gem_v__for_items_46 = gem_v_captures;
#line 722 "compiler/main.gem"
    GemVal gem_v__for_i_46 = gem_int(0);
#line 722 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2364[] = {gem_v__for_items_46};
        if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_len_fn(NULL, _t2364, 1)))) break;
#line 722 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_46, gem_v__for_i_46);
#line 722 "compiler/main.gem"
        gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 723 "compiler/main.gem"
    GemVal _t2365[] = {gem_v_cap};
    GemVal _t2366 = (*gem_v_mangle);
        GemVal gem_v_mc = _t2366.fn(_t2366.env, _t2365, 1);
#line 724 "compiler/main.gem"
    GemVal _t2367[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2367, 2))) {
#line 725 "compiler/main.gem"
    GemVal _t2368[] = {gem_v_env_tmp};
    GemVal _t2369[] = {gem_v_mc};
    GemVal _t2370[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2368, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2369, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2370, 1)), gem_string(";\n")));
        } else {
#line 727 "compiler/main.gem"
    GemVal _t2371[] = {gem_v_env_tmp};
    GemVal _t2372[] = {gem_v_mc};
    GemVal _t2373[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2371, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2372, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t2373, 1)), gem_string(";\n")));
        }
    }

    GemVal _t2374 = gem_table_new();
    GemVal _t2375 = gem_v_result;
    GemVal _t2376[] = {gem_table_get(_t2375, gem_string("fn_name"))};
    GemVal _t2377[] = {gem_v_env_tmp};
    gem_table_set(_t2374, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2376, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2377, 1)), gem_string(")")));
    gem_table_set(_t2374, gem_string("setup"), gem_v_setup);
    GemVal _t2378 = _t2374;
    gem_pop_frame();
    return _t2378;
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
#line 737 "compiler/main.gem"
    GemVal gem_v_arg_setups = gem_string("");
#line 738 "compiler/main.gem"
    GemVal _t2380 = gem_table_new();
    GemVal gem_v_arg_exprs = _t2380;
#line 739 "compiler/main.gem"
    GemVal gem_v__for_i_47 = gem_int(0);
#line 739 "compiler/main.gem"
    GemVal _t2381[] = {gem_v_args};
    GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2381, 1);
#line 739 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 739 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_47;
#line 739 "compiler/main.gem"
        gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 740 "compiler/main.gem"
    GemVal _t2382[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2383 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2383.fn(_t2383.env, _t2382, 1);
#line 741 "compiler/main.gem"
    GemVal _t2384 = gem_v_r;
        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t2384, gem_string("setup")));
#line 742 "compiler/main.gem"
    GemVal _t2385 = gem_v_r;
    GemVal _t2386[] = {gem_v_arg_exprs, gem_table_get(_t2385, gem_string("expr"))};
        (void)(gem_push_fn(NULL, _t2386, 2));
    }

#line 744 "compiler/main.gem"
    GemVal _t2387[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t2387, 1);
#line 745 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 746 "compiler/main.gem"
    GemVal _t2388 = gem_table_new();
    gem_table_set(_t2388, gem_string("setup"), gem_v_arg_setups);
    gem_table_set(_t2388, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t2388, gem_string("argc"), gem_int(0));
        GemVal _t2389 = _t2388;
        gem_pop_frame();
        return _t2389;
    }
#line 748 "compiler/main.gem"
    GemVal _t2390 = (*gem_v_tmp);
    GemVal gem_v_t = _t2390.fn(_t2390.env, NULL, 0);
#line 749 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 750 "compiler/main.gem"
    GemVal gem_v__for_i_48 = gem_int(1);
#line 750 "compiler/main.gem"
    GemVal gem_v__for_limit_48 = gem_v_argc;
#line 750 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 750 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_48;
#line 750 "compiler/main.gem"
        gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 751 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t2391 = gem_table_new();
    GemVal _t2392[] = {gem_v_t};
    GemVal _t2393[] = {gem_v_arr};
    gem_table_set(_t2391, gem_string("setup"), gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2392, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2393, 1)), gem_string("};\n"))));
    gem_table_set(_t2391, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t2391, gem_string("argc"), gem_v_argc);
    GemVal _t2394 = _t2391;
    gem_pop_frame();
    return _t2394;
}

struct _closure__anon_37 {
};
static GemVal _anon_37(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_37", "compiler/main.gem", 0);
#line 758 "compiler/main.gem"
    GemVal _t2396 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2396, gem_string("argc")), gem_int(0)))) {
#line 759 "compiler/main.gem"
    GemVal _t2397 = gem_table_new();
    GemVal _t2398[] = {gem_v_fn_name};
    GemVal _t2399[] = {gem_v_env};
    gem_table_set(_t2397, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2398, 1), gem_string("(")), gem_to_string_fn(NULL, _t2399, 1)), gem_string(", NULL, 0)")));
    GemVal _t2400 = gem_v_ca;
    gem_table_set(_t2397, gem_string("setup"), gem_table_get(_t2400, gem_string("setup")));
        GemVal _t2401 = _t2397;
        gem_pop_frame();
        return _t2401;
    }
    GemVal _t2402 = gem_table_new();
    GemVal _t2403[] = {gem_v_fn_name};
    GemVal _t2404[] = {gem_v_env};
    GemVal _t2405 = gem_v_ca;
    GemVal _t2406[] = {gem_table_get(_t2405, gem_string("arr_name"))};
    GemVal _t2407 = gem_v_ca;
    GemVal _t2408[] = {gem_table_get(_t2407, gem_string("argc"))};
    gem_table_set(_t2402, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2403, 1), gem_string("(")), gem_to_string_fn(NULL, _t2404, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2406, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2408, 1)), gem_string(")")));
    GemVal _t2409 = gem_v_ca;
    gem_table_set(_t2402, gem_string("setup"), gem_table_get(_t2409, gem_string("setup")));
    GemVal _t2410 = _t2402;
    gem_pop_frame();
    return _t2410;
}

struct _closure__anon_38 {
    GemVal *gem_v_builtin_fns;
    GemVal *gem_v_compile_args;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_emit_direct_call;
    GemVal *gem_v_local_names;
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
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_38", "compiler/main.gem", 0);
#line 767 "compiler/main.gem"
    GemVal gem_v__d49 = gem_v_node;
#line 767 "compiler/main.gem"
    GemVal _t2411 = gem_v__d49;
    GemVal gem_v_func = gem_table_get(_t2411, gem_string("func"));
#line 767 "compiler/main.gem"
    GemVal _t2412 = gem_v__d49;
    GemVal gem_v_args = gem_table_get(_t2412, gem_string("args"));

#line 770 "compiler/main.gem"
    GemVal _t2413 = gem_v_func;
    GemVal _t2415;
    if (!gem_truthy(gem_eq(gem_table_get(_t2413, gem_string("tag")), gem_string("dot")))) {
        _t2415 = gem_eq(gem_table_get(_t2413, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t2414 = gem_v_func;
        _t2415 = gem_eq(gem_table_get(_t2414, gem_string("field")), gem_string("len"));
    }
    GemVal _t2417;
    if (!gem_truthy(_t2415)) {
        _t2417 = _t2415;
    } else {
        GemVal _t2416[] = {gem_v_args};
        _t2417 = gem_eq(gem_len_fn(NULL, _t2416, 1), gem_int(0));
    }
    if (gem_truthy(_t2417)) {
#line 771 "compiler/main.gem"
    GemVal _t2418 = gem_v_func;
    GemVal _t2419[] = {gem_table_get(_t2418, gem_string("object"))};
    GemVal _t2420 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2420.fn(_t2420.env, _t2419, 1);
#line 772 "compiler/main.gem"
    GemVal _t2421 = gem_table_new();
    GemVal _t2422 = gem_v_obj_r;
    GemVal _t2423[] = {gem_table_get(_t2422, gem_string("expr"))};
    gem_table_set(_t2421, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t2423, 1)), gem_string(")")));
    GemVal _t2424 = gem_v_obj_r;
    gem_table_set(_t2421, gem_string("setup"), gem_table_get(_t2424, gem_string("setup")));
        GemVal _t2425 = _t2421;
        gem_pop_frame();
        return _t2425;
    }
#line 776 "compiler/main.gem"
    GemVal _t2426 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t2426, gem_string("tag")), gem_string("var")))) {
#line 777 "compiler/main.gem"
    GemVal _t2427 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t2427, gem_string("name"));
#line 779 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 780 "compiler/main.gem"
    GemVal _t2428 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t2428, gem_string("line"));
#line 781 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 782 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 784 "compiler/main.gem"
    GemVal _t2429[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t2429, 1);
#line 785 "compiler/main.gem"
    GemVal _t2430[] = {gem_v_args};
    GemVal _t2431 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t2431.fn(_t2431.env, _t2430, 1);
#line 786 "compiler/main.gem"
    GemVal _t2432 = gem_v_ca;
            if (gem_truthy(gem_eq(gem_table_get(_t2432, gem_string("argc")), gem_int(0)))) {
#line 787 "compiler/main.gem"
    GemVal _t2433 = gem_table_new();
    GemVal _t2434[] = {gem_v_escaped_file};
    GemVal _t2435[] = {gem_v_line};
    gem_table_set(_t2433, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2434, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2435, 1)), gem_string(", NULL, 0)")));
    GemVal _t2436 = gem_v_ca;
    gem_table_set(_t2433, gem_string("setup"), gem_table_get(_t2436, gem_string("setup")));
                GemVal _t2437 = _t2433;
                gem_pop_frame();
                return _t2437;
            }
#line 789 "compiler/main.gem"
    GemVal _t2438 = gem_table_new();
    GemVal _t2439[] = {gem_v_escaped_file};
    GemVal _t2440[] = {gem_v_line};
    GemVal _t2441 = gem_v_ca;
    GemVal _t2442[] = {gem_table_get(_t2441, gem_string("arr_name"))};
    GemVal _t2443 = gem_v_ca;
    GemVal _t2444[] = {gem_table_get(_t2443, gem_string("argc"))};
    gem_table_set(_t2438, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2439, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2440, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2442, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2444, 1)), gem_string(")")));
    GemVal _t2445 = gem_v_ca;
    gem_table_set(_t2438, gem_string("setup"), gem_table_get(_t2445, gem_string("setup")));
            GemVal _t2446 = _t2438;
            gem_pop_frame();
            return _t2446;
        } else {
#line 790 "compiler/main.gem"
    GemVal _t2447[] = {(*gem_v_builtin_fns), gem_v_name};
    GemVal _t2449;
    if (!gem_truthy(gem_has_key_fn(NULL, _t2447, 2))) {
        _t2449 = gem_has_key_fn(NULL, _t2447, 2);
    } else {
        GemVal _t2448[] = {(*gem_v_local_names), gem_v_name};
        _t2449 = gem_not(gem_fn_set_contains(NULL, _t2448, 2));
    }
            if (gem_truthy(_t2449)) {
#line 791 "compiler/main.gem"
    GemVal _t2450[] = {gem_v_args};
    GemVal _t2451 = (*gem_v_compile_args);
    GemVal _t2452[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t2451.fn(_t2451.env, _t2450, 1)};
    GemVal _t2453 = (*gem_v_emit_direct_call);
                GemVal _t2454 = _t2453.fn(_t2453.env, _t2452, 3);
                gem_pop_frame();
                return _t2454;
            } else {
#line 792 "compiler/main.gem"
    GemVal _t2455[] = {(*gem_v_defined_fns), gem_v_name};
    GemVal _t2457;
    if (!gem_truthy(gem_fn_set_contains(NULL, _t2455, 2))) {
        _t2457 = gem_fn_set_contains(NULL, _t2455, 2);
    } else {
        GemVal _t2456[] = {(*gem_v_local_names), gem_v_name};
        _t2457 = gem_not(gem_fn_set_contains(NULL, _t2456, 2));
    }
                if (gem_truthy(_t2457)) {
#line 793 "compiler/main.gem"
    GemVal _t2458[] = {gem_v_name};
    GemVal _t2459[] = {gem_v_args};
    GemVal _t2460 = (*gem_v_compile_args);
    GemVal _t2461[] = {gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t2458, 1)), gem_string("NULL"), _t2460.fn(_t2460.env, _t2459, 1)};
    GemVal _t2462 = (*gem_v_emit_direct_call);
                    GemVal _t2463 = _t2462.fn(_t2462.env, _t2461, 3);
                    gem_pop_frame();
                    return _t2463;
                }
            }
        }
    }
#line 798 "compiler/main.gem"
    GemVal _t2464[] = {gem_v_func};
    GemVal _t2465 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t2465.fn(_t2465.env, _t2464, 1);
#line 799 "compiler/main.gem"
    GemVal _t2466[] = {gem_v_args};
    GemVal _t2467 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t2467.fn(_t2467.env, _t2466, 1);
#line 800 "compiler/main.gem"
    GemVal _t2468 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t2468.fn(_t2468.env, NULL, 0);
#line 801 "compiler/main.gem"
    GemVal _t2469 = gem_v_func_r;
    GemVal _t2470 = gem_v_ca;
    GemVal _t2471[] = {gem_v_tmp_fn};
    GemVal _t2472 = gem_v_func_r;
    GemVal _t2473[] = {gem_table_get(_t2472, gem_string("expr"))};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get(_t2469, gem_string("setup")), gem_table_get(_t2470, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2471, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2473, 1)), gem_string(";\n")));
#line 802 "compiler/main.gem"
    GemVal _t2474 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2474, gem_string("argc")), gem_int(0)))) {
#line 803 "compiler/main.gem"
    GemVal _t2475 = gem_table_new();
    GemVal _t2476[] = {gem_v_tmp_fn};
    GemVal _t2477[] = {gem_v_tmp_fn};
    gem_table_set(_t2475, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2476, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2477, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t2475, gem_string("setup"), gem_v_setup);
        GemVal _t2478 = _t2475;
        gem_pop_frame();
        return _t2478;
    }
    GemVal _t2479 = gem_table_new();
    GemVal _t2480[] = {gem_v_tmp_fn};
    GemVal _t2481[] = {gem_v_tmp_fn};
    GemVal _t2482 = gem_v_ca;
    GemVal _t2483[] = {gem_table_get(_t2482, gem_string("arr_name"))};
    GemVal _t2484 = gem_v_ca;
    GemVal _t2485[] = {gem_table_get(_t2484, gem_string("argc"))};
    gem_table_set(_t2479, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2480, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2481, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t2483, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2485, 1)), gem_string(")")));
    gem_table_set(_t2479, gem_string("setup"), gem_v_setup);
    GemVal _t2486 = _t2479;
    gem_pop_frame();
    return _t2486;
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
#line 811 "compiler/main.gem"
    GemVal _t2488 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t2488, gem_string("op"));
#line 812 "compiler/main.gem"
    GemVal _t2489 = gem_v_node;
    GemVal _t2490[] = {gem_table_get(_t2489, gem_string("left"))};
    GemVal _t2491 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t2491.fn(_t2491.env, _t2490, 1);
#line 813 "compiler/main.gem"
    GemVal _t2492 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t2492, gem_string("expr"));
#line 814 "compiler/main.gem"
    GemVal _t2493 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t2493, gem_string("setup"));
#line 816 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 817 "compiler/main.gem"
    GemVal _t2494 = gem_v_node;
    GemVal _t2495[] = {gem_table_get(_t2494, gem_string("right"))};
    GemVal _t2496 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t2496.fn(_t2496.env, _t2495, 1);
#line 818 "compiler/main.gem"
    GemVal _t2497 = gem_table_new();
    GemVal _t2498[] = {gem_v_lc};
    GemVal _t2499 = gem_v_rc_r;
    GemVal _t2500[] = {gem_table_get(_t2499, gem_string("expr"))};
    gem_table_set(_t2497, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2498, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2500, 1)), gem_string(")")));
    GemVal _t2501 = gem_v_rc_r;
    gem_table_set(_t2497, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2501, gem_string("setup"))));
        GemVal _t2502 = _t2497;
        gem_pop_frame();
        return _t2502;
    } else {
#line 819 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 820 "compiler/main.gem"
    GemVal _t2503 = gem_v_node;
    GemVal _t2504[] = {gem_table_get(_t2503, gem_string("right"))};
    GemVal _t2505 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t2505.fn(_t2505.env, _t2504, 1);
#line 821 "compiler/main.gem"
    GemVal _t2506 = gem_table_new();
    GemVal _t2507[] = {gem_v_lc};
    GemVal _t2508 = gem_v_rc_r;
    GemVal _t2509[] = {gem_table_get(_t2508, gem_string("expr"))};
    gem_table_set(_t2506, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t2507, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2509, 1)), gem_string(")")));
    GemVal _t2510 = gem_v_rc_r;
    gem_table_set(_t2506, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2510, gem_string("setup"))));
            GemVal _t2511 = _t2506;
            gem_pop_frame();
            return _t2511;
        } else {
#line 822 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 823 "compiler/main.gem"
    GemVal _t2512 = gem_v_node;
    GemVal _t2513[] = {gem_table_get(_t2512, gem_string("right"))};
    GemVal _t2514 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t2514.fn(_t2514.env, _t2513, 1);
#line 824 "compiler/main.gem"
    GemVal _t2515 = gem_table_new();
    GemVal _t2516[] = {gem_v_lc};
    GemVal _t2517 = gem_v_rc_r;
    GemVal _t2518[] = {gem_table_get(_t2517, gem_string("expr"))};
    gem_table_set(_t2515, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t2516, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2518, 1)), gem_string(")")));
    GemVal _t2519 = gem_v_rc_r;
    gem_table_set(_t2515, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2519, gem_string("setup"))));
                GemVal _t2520 = _t2515;
                gem_pop_frame();
                return _t2520;
            } else {
#line 825 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 826 "compiler/main.gem"
    GemVal _t2521 = gem_v_node;
    GemVal _t2522[] = {gem_table_get(_t2521, gem_string("right"))};
    GemVal _t2523 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t2523.fn(_t2523.env, _t2522, 1);
#line 827 "compiler/main.gem"
    GemVal _t2524 = gem_table_new();
    GemVal _t2525[] = {gem_v_lc};
    GemVal _t2526 = gem_v_rc_r;
    GemVal _t2527[] = {gem_table_get(_t2526, gem_string("expr"))};
    gem_table_set(_t2524, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t2525, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2527, 1)), gem_string(")")));
    GemVal _t2528 = gem_v_rc_r;
    gem_table_set(_t2524, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2528, gem_string("setup"))));
                    GemVal _t2529 = _t2524;
                    gem_pop_frame();
                    return _t2529;
                } else {
#line 828 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 829 "compiler/main.gem"
    GemVal _t2530 = gem_v_node;
    GemVal _t2531[] = {gem_table_get(_t2530, gem_string("right"))};
    GemVal _t2532 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t2532.fn(_t2532.env, _t2531, 1);
#line 830 "compiler/main.gem"
    GemVal _t2533 = gem_table_new();
    GemVal _t2534[] = {gem_v_lc};
    GemVal _t2535 = gem_v_rc_r;
    GemVal _t2536[] = {gem_table_get(_t2535, gem_string("expr"))};
    gem_table_set(_t2533, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t2534, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2536, 1)), gem_string(")")));
    GemVal _t2537 = gem_v_rc_r;
    gem_table_set(_t2533, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2537, gem_string("setup"))));
                        GemVal _t2538 = _t2533;
                        gem_pop_frame();
                        return _t2538;
                    } else {
#line 831 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 832 "compiler/main.gem"
    GemVal _t2539 = gem_v_node;
    GemVal _t2540[] = {gem_table_get(_t2539, gem_string("right"))};
    GemVal _t2541 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t2541.fn(_t2541.env, _t2540, 1);
#line 833 "compiler/main.gem"
    GemVal _t2542 = (*gem_v_tmp);
                            GemVal gem_v_t = _t2542.fn(_t2542.env, NULL, 0);
#line 834 "compiler/main.gem"
    GemVal _t2543 = gem_v_rc_r;
    GemVal _t2544[] = {gem_v_t};
    GemVal _t2545 = gem_v_rc_r;
    GemVal _t2546[] = {gem_table_get(_t2545, gem_string("expr"))};
    GemVal _t2547[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t2543, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2544, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2546, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2547, 1)), gem_string("};\n")));
#line 835 "compiler/main.gem"
    GemVal _t2548 = gem_table_new();
    GemVal _t2549[] = {gem_v_t};
    gem_table_set(_t2548, gem_string("expr"), gem_add(gem_add(gem_string("gem_has_key_fn(NULL, "), gem_to_string_fn(NULL, _t2549, 1)), gem_string(", 2)")));
    gem_table_set(_t2548, gem_string("setup"), gem_v_setup);
                            GemVal _t2550 = _t2548;
                            gem_pop_frame();
                            return _t2550;
                        } else {
#line 836 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 837 "compiler/main.gem"
    GemVal _t2551 = gem_v_node;
    GemVal _t2552[] = {gem_table_get(_t2551, gem_string("right"))};
    GemVal _t2553 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t2553.fn(_t2553.env, _t2552, 1);
#line 838 "compiler/main.gem"
    GemVal _t2554 = gem_table_new();
    GemVal _t2555[] = {gem_v_lc};
    GemVal _t2556 = gem_v_rc_r;
    GemVal _t2557[] = {gem_table_get(_t2556, gem_string("expr"))};
    gem_table_set(_t2554, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t2555, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2557, 1)), gem_string(")")));
    GemVal _t2558 = gem_v_rc_r;
    gem_table_set(_t2554, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2558, gem_string("setup"))));
                                GemVal _t2559 = _t2554;
                                gem_pop_frame();
                                return _t2559;
                            } else {
#line 839 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 840 "compiler/main.gem"
    GemVal _t2560 = gem_v_node;
    GemVal _t2561[] = {gem_table_get(_t2560, gem_string("right"))};
    GemVal _t2562 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t2562.fn(_t2562.env, _t2561, 1);
#line 841 "compiler/main.gem"
    GemVal _t2563 = gem_table_new();
    GemVal _t2564[] = {gem_v_lc};
    GemVal _t2565 = gem_v_rc_r;
    GemVal _t2566[] = {gem_table_get(_t2565, gem_string("expr"))};
    gem_table_set(_t2563, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t2564, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2566, 1)), gem_string(")")));
    GemVal _t2567 = gem_v_rc_r;
    gem_table_set(_t2563, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2567, gem_string("setup"))));
                                    GemVal _t2568 = _t2563;
                                    gem_pop_frame();
                                    return _t2568;
                                } else {
#line 842 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 843 "compiler/main.gem"
    GemVal _t2569 = gem_v_node;
    GemVal _t2570[] = {gem_table_get(_t2569, gem_string("right"))};
    GemVal _t2571 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t2571.fn(_t2571.env, _t2570, 1);
#line 844 "compiler/main.gem"
    GemVal _t2572 = gem_table_new();
    GemVal _t2573[] = {gem_v_lc};
    GemVal _t2574 = gem_v_rc_r;
    GemVal _t2575[] = {gem_table_get(_t2574, gem_string("expr"))};
    gem_table_set(_t2572, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t2573, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2575, 1)), gem_string(")")));
    GemVal _t2576 = gem_v_rc_r;
    gem_table_set(_t2572, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2576, gem_string("setup"))));
                                        GemVal _t2577 = _t2572;
                                        gem_pop_frame();
                                        return _t2577;
                                    } else {
#line 845 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 846 "compiler/main.gem"
    GemVal _t2578 = gem_v_node;
    GemVal _t2579[] = {gem_table_get(_t2578, gem_string("right"))};
    GemVal _t2580 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t2580.fn(_t2580.env, _t2579, 1);
#line 847 "compiler/main.gem"
    GemVal _t2581 = gem_table_new();
    GemVal _t2582[] = {gem_v_lc};
    GemVal _t2583 = gem_v_rc_r;
    GemVal _t2584[] = {gem_table_get(_t2583, gem_string("expr"))};
    gem_table_set(_t2581, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t2582, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2584, 1)), gem_string(")")));
    GemVal _t2585 = gem_v_rc_r;
    gem_table_set(_t2581, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2585, gem_string("setup"))));
                                            GemVal _t2586 = _t2581;
                                            gem_pop_frame();
                                            return _t2586;
                                        } else {
#line 848 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 849 "compiler/main.gem"
    GemVal _t2587 = gem_v_node;
    GemVal _t2588[] = {gem_table_get(_t2587, gem_string("right"))};
    GemVal _t2589 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t2589.fn(_t2589.env, _t2588, 1);
#line 850 "compiler/main.gem"
    GemVal _t2590 = gem_table_new();
    GemVal _t2591[] = {gem_v_lc};
    GemVal _t2592 = gem_v_rc_r;
    GemVal _t2593[] = {gem_table_get(_t2592, gem_string("expr"))};
    gem_table_set(_t2590, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t2591, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2593, 1)), gem_string(")")));
    GemVal _t2594 = gem_v_rc_r;
    gem_table_set(_t2590, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2594, gem_string("setup"))));
                                                GemVal _t2595 = _t2590;
                                                gem_pop_frame();
                                                return _t2595;
                                            } else {
#line 851 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 852 "compiler/main.gem"
    GemVal _t2596 = gem_v_node;
    GemVal _t2597[] = {gem_table_get(_t2596, gem_string("right"))};
    GemVal _t2598 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t2598.fn(_t2598.env, _t2597, 1);
#line 853 "compiler/main.gem"
    GemVal _t2599 = gem_table_new();
    GemVal _t2600[] = {gem_v_lc};
    GemVal _t2601 = gem_v_rc_r;
    GemVal _t2602[] = {gem_table_get(_t2601, gem_string("expr"))};
    gem_table_set(_t2599, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t2600, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2602, 1)), gem_string(")")));
    GemVal _t2603 = gem_v_rc_r;
    gem_table_set(_t2599, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2603, gem_string("setup"))));
                                                    GemVal _t2604 = _t2599;
                                                    gem_pop_frame();
                                                    return _t2604;
                                                } else {
#line 854 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 855 "compiler/main.gem"
    GemVal _t2605 = gem_v_node;
    GemVal _t2606[] = {gem_table_get(_t2605, gem_string("right"))};
    GemVal _t2607 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t2607.fn(_t2607.env, _t2606, 1);
#line 856 "compiler/main.gem"
    GemVal _t2608 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t2608.fn(_t2608.env, NULL, 0);
#line 857 "compiler/main.gem"
                                                        GemVal gem_v_setup = gem_v_ls;
#line 858 "compiler/main.gem"
    GemVal _t2609[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2609, 1)), gem_string(";\n")));
#line 859 "compiler/main.gem"
    GemVal _t2610[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t2610, 1)), gem_string(")) {\n")));
#line 860 "compiler/main.gem"
    GemVal _t2611[] = {gem_v_t};
    GemVal _t2612[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2611, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2612, 1)), gem_string(";\n")));
#line 861 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 862 "compiler/main.gem"
    GemVal _t2613 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t2613, gem_string("setup")), gem_string("")))) {
#line 863 "compiler/main.gem"
    GemVal _t2614 = gem_v_rc_r;
    GemVal _t2615[] = {gem_table_get(_t2614, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t2615, 3));
                                                        }
#line 865 "compiler/main.gem"
    GemVal _t2616[] = {gem_v_t};
    GemVal _t2617 = gem_v_rc_r;
    GemVal _t2618[] = {gem_table_get(_t2617, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2616, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2618, 1)), gem_string(";\n")));
#line 866 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 867 "compiler/main.gem"
    GemVal _t2619 = gem_table_new();
    gem_table_set(_t2619, gem_string("expr"), gem_v_t);
    gem_table_set(_t2619, gem_string("setup"), gem_v_setup);
                                                        GemVal _t2620 = _t2619;
                                                        gem_pop_frame();
                                                        return _t2620;
                                                    } else {
#line 868 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 869 "compiler/main.gem"
    GemVal _t2621 = gem_v_node;
    GemVal _t2622[] = {gem_table_get(_t2621, gem_string("right"))};
    GemVal _t2623 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t2623.fn(_t2623.env, _t2622, 1);
#line 870 "compiler/main.gem"
    GemVal _t2624 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t2624.fn(_t2624.env, NULL, 0);
#line 871 "compiler/main.gem"
                                                            GemVal gem_v_setup = gem_v_ls;
#line 872 "compiler/main.gem"
    GemVal _t2625[] = {gem_v_t};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2625, 1)), gem_string(";\n")));
#line 873 "compiler/main.gem"
    GemVal _t2626[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t2626, 1)), gem_string(")) {\n")));
#line 874 "compiler/main.gem"
    GemVal _t2627[] = {gem_v_t};
    GemVal _t2628[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2627, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2628, 1)), gem_string(";\n")));
#line 875 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 876 "compiler/main.gem"
    GemVal _t2629 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t2629, gem_string("setup")), gem_string("")))) {
#line 877 "compiler/main.gem"
    GemVal _t2630 = gem_v_rc_r;
    GemVal _t2631[] = {gem_table_get(_t2630, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                                gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t2631, 3));
                                                            }
#line 879 "compiler/main.gem"
    GemVal _t2632[] = {gem_v_t};
    GemVal _t2633 = gem_v_rc_r;
    GemVal _t2634[] = {gem_table_get(_t2633, gem_string("expr"))};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2632, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2634, 1)), gem_string(";\n")));
#line 880 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 881 "compiler/main.gem"
    GemVal _t2635 = gem_table_new();
    gem_table_set(_t2635, gem_string("expr"), gem_v_t);
    gem_table_set(_t2635, gem_string("setup"), gem_v_setup);
                                                            GemVal _t2636 = _t2635;
                                                            gem_pop_frame();
                                                            return _t2636;
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
#line 884 "compiler/main.gem"
    GemVal _t2637[] = {gem_v_op};
    GemVal _t2638[] = {gem_add(gem_string("unknown binary operator: "), gem_to_string_fn(NULL, _t2637, 1))};
    GemVal _t2639 = gem_error_at_fn("compiler/main.gem", 884, _t2638, 1);
    gem_pop_frame();
    return _t2639;
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
#line 890 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 891 "compiler/main.gem"
    GemVal gem_v__for_items_50 = gem_v_stmts;
#line 891 "compiler/main.gem"
    GemVal gem_v__for_i_50 = gem_int(0);
#line 891 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2641[] = {gem_v__for_items_50};
        if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_len_fn(NULL, _t2641, 1)))) break;
#line 891 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_50, gem_v__for_i_50);
#line 891 "compiler/main.gem"
        gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 892 "compiler/main.gem"
    GemVal _t2642[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2643 = (*gem_v_compile_stmt);
        gem_v_out = gem_add(gem_add(gem_v_out, _t2643.fn(_t2643.env, _t2642, 2)), gem_string("\n"));
    }

    GemVal _t2644 = gem_v_out;
    gem_pop_frame();
    return _t2644;
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
#line 900 "compiler/main.gem"
    GemVal _t2646[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2646, 1);
#line 901 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 902 "compiler/main.gem"
    GemVal _t2647 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2647, gem_string("line")), GEM_NIL))) {
#line 903 "compiler/main.gem"
    GemVal _t2648 = gem_v_node;
    GemVal _t2649[] = {gem_table_get(_t2648, gem_string("line"))};
    GemVal _t2650[] = {(*gem_v_source_name)};
    GemVal _t2651[] = {gem_fn_escape_c_string(NULL, _t2650, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2649, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2651, 1)), gem_string("\"\n"));
    }
#line 906 "compiler/main.gem"
    GemVal gem_v__match_51 = gem_v_node;
    GemVal _t2652[] = {gem_v__match_51};
    GemVal _t2654;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2652, 1), gem_string("table")))) {
        _t2654 = gem_eq(gem_type_fn(NULL, _t2652, 1), gem_string("table"));
    } else {
        GemVal _t2653[] = {gem_v__match_51, gem_string("tag")};
        _t2654 = gem_has_key_fn(NULL, _t2653, 2);
    }
    GemVal _t2655;
    if (!gem_truthy(_t2654)) {
        _t2655 = _t2654;
    } else {
        _t2655 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2657;
    if (!gem_truthy(_t2655)) {
        _t2657 = _t2655;
    } else {
        GemVal _t2656[] = {gem_v__match_51, gem_string("name")};
        _t2657 = gem_has_key_fn(NULL, _t2656, 2);
    }
    GemVal _t2659;
    if (!gem_truthy(_t2657)) {
        _t2659 = _t2657;
    } else {
        GemVal _t2658[] = {gem_v__match_51, gem_string("value")};
        _t2659 = gem_has_key_fn(NULL, _t2658, 2);
    }
    if (gem_truthy(_t2659)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_51, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_51, gem_string("value"));
#line 908 "compiler/main.gem"
    GemVal _t2660[] = {gem_v_value};
    GemVal _t2661 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2661.fn(_t2661.env, _t2660, 1);
#line 909 "compiler/main.gem"
    GemVal _t2662[] = {gem_v_name};
    GemVal _t2663 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2663.fn(_t2663.env, _t2662, 1);
#line 910 "compiler/main.gem"
    GemVal _t2665;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t2665 = (*gem_v_in_top_level);
    } else {
        GemVal _t2664[] = {(*gem_v_top_level_vars), gem_v_name};
        _t2665 = gem_fn_set_contains(NULL, _t2664, 2);
    }
        if (gem_truthy(_t2665)) {
#line 911 "compiler/main.gem"
    GemVal _t2666[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2666, 2))) {
#line 912 "compiler/main.gem"
    GemVal _t2667[] = {gem_v_line_dir};
    GemVal _t2668 = gem_v_r;
    GemVal _t2669[] = {gem_table_get(_t2668, gem_string("setup"))};
    GemVal _t2670[] = {gem_v_p};
    GemVal _t2671[] = {gem_v_mname};
    GemVal _t2672[] = {gem_v_p};
    GemVal _t2673[] = {gem_v_mname};
    GemVal _t2674 = gem_v_r;
    GemVal _t2675[] = {gem_table_get(_t2674, gem_string("expr"))};
                GemVal _t2676 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2667, 1), gem_to_string_fn(NULL, _t2669, 1)), gem_to_string_fn(NULL, _t2670, 1)), gem_to_string_fn(NULL, _t2671, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2672, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2673, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2675, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2676;
            }
#line 914 "compiler/main.gem"
    GemVal _t2677[] = {gem_v_line_dir};
    GemVal _t2678 = gem_v_r;
    GemVal _t2679[] = {gem_table_get(_t2678, gem_string("setup"))};
    GemVal _t2680[] = {gem_v_p};
    GemVal _t2681[] = {gem_v_mname};
    GemVal _t2682 = gem_v_r;
    GemVal _t2683[] = {gem_table_get(_t2682, gem_string("expr"))};
            GemVal _t2684 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2677, 1), gem_to_string_fn(NULL, _t2679, 1)), gem_to_string_fn(NULL, _t2680, 1)), gem_to_string_fn(NULL, _t2681, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2683, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2684;
        }
#line 916 "compiler/main.gem"
    GemVal _t2685[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2685, 2))) {
#line 917 "compiler/main.gem"
    GemVal _t2686[] = {gem_v_line_dir};
    GemVal _t2687 = gem_v_r;
    GemVal _t2688[] = {gem_table_get(_t2687, gem_string("setup"))};
    GemVal _t2689[] = {gem_v_p};
    GemVal _t2690[] = {gem_v_mname};
    GemVal _t2691[] = {gem_v_p};
    GemVal _t2692[] = {gem_v_mname};
    GemVal _t2693 = gem_v_r;
    GemVal _t2694[] = {gem_table_get(_t2693, gem_string("expr"))};
            GemVal _t2695 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2686, 1), gem_to_string_fn(NULL, _t2688, 1)), gem_to_string_fn(NULL, _t2689, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2690, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2691, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2692, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2694, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2695;
        }
#line 919 "compiler/main.gem"
    GemVal _t2696[] = {gem_v_line_dir};
    GemVal _t2697 = gem_v_r;
    GemVal _t2698[] = {gem_table_get(_t2697, gem_string("setup"))};
    GemVal _t2699[] = {gem_v_p};
    GemVal _t2700[] = {gem_v_mname};
    GemVal _t2701 = gem_v_r;
    GemVal _t2702[] = {gem_table_get(_t2701, gem_string("expr"))};
        GemVal _t2703 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2696, 1), gem_to_string_fn(NULL, _t2698, 1)), gem_to_string_fn(NULL, _t2699, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2700, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2702, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2703;
    } else {
    GemVal _t2704[] = {gem_v__match_51};
    GemVal _t2706;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2704, 1), gem_string("table")))) {
        _t2706 = gem_eq(gem_type_fn(NULL, _t2704, 1), gem_string("table"));
    } else {
        GemVal _t2705[] = {gem_v__match_51, gem_string("tag")};
        _t2706 = gem_has_key_fn(NULL, _t2705, 2);
    }
    GemVal _t2707;
    if (!gem_truthy(_t2706)) {
        _t2707 = _t2706;
    } else {
        _t2707 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2709;
    if (!gem_truthy(_t2707)) {
        _t2709 = _t2707;
    } else {
        GemVal _t2708[] = {gem_v__match_51, gem_string("name")};
        _t2709 = gem_has_key_fn(NULL, _t2708, 2);
    }
    GemVal _t2711;
    if (!gem_truthy(_t2709)) {
        _t2711 = _t2709;
    } else {
        GemVal _t2710[] = {gem_v__match_51, gem_string("value")};
        _t2711 = gem_has_key_fn(NULL, _t2710, 2);
    }
    if (gem_truthy(_t2711)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_51, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_51, gem_string("value"));
#line 921 "compiler/main.gem"
    GemVal _t2712[] = {gem_v_value};
    GemVal _t2713 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2713.fn(_t2713.env, _t2712, 1);
#line 922 "compiler/main.gem"
    GemVal _t2714[] = {gem_v_name};
    GemVal _t2715 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2715.fn(_t2715.env, _t2714, 1);
#line 923 "compiler/main.gem"
    GemVal _t2716[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2718;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2716, 2))) {
        _t2718 = gem_fn_set_contains(NULL, _t2716, 2);
    } else {
        GemVal _t2717[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2718 = gem_fn_set_contains(NULL, _t2717, 2);
    }
        if (gem_truthy(_t2718)) {
#line 924 "compiler/main.gem"
    GemVal _t2719[] = {gem_v_line_dir};
    GemVal _t2720 = gem_v_r;
    GemVal _t2721[] = {gem_table_get(_t2720, gem_string("setup"))};
    GemVal _t2722[] = {gem_v_p};
    GemVal _t2723[] = {gem_v_mname};
    GemVal _t2724 = gem_v_r;
    GemVal _t2725[] = {gem_table_get(_t2724, gem_string("expr"))};
            GemVal _t2726 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2719, 1), gem_to_string_fn(NULL, _t2721, 1)), gem_to_string_fn(NULL, _t2722, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2723, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2725, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2726;
        }
#line 926 "compiler/main.gem"
    GemVal _t2727[] = {gem_v_line_dir};
    GemVal _t2728 = gem_v_r;
    GemVal _t2729[] = {gem_table_get(_t2728, gem_string("setup"))};
    GemVal _t2730[] = {gem_v_p};
    GemVal _t2731[] = {gem_v_mname};
    GemVal _t2732 = gem_v_r;
    GemVal _t2733[] = {gem_table_get(_t2732, gem_string("expr"))};
        GemVal _t2734 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2727, 1), gem_to_string_fn(NULL, _t2729, 1)), gem_to_string_fn(NULL, _t2730, 1)), gem_to_string_fn(NULL, _t2731, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2733, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2734;
    } else {
    GemVal _t2735[] = {gem_v__match_51};
    GemVal _t2737;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2735, 1), gem_string("table")))) {
        _t2737 = gem_eq(gem_type_fn(NULL, _t2735, 1), gem_string("table"));
    } else {
        GemVal _t2736[] = {gem_v__match_51, gem_string("tag")};
        _t2737 = gem_has_key_fn(NULL, _t2736, 2);
    }
    GemVal _t2738;
    if (!gem_truthy(_t2737)) {
        _t2738 = _t2737;
    } else {
        _t2738 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t2740;
    if (!gem_truthy(_t2738)) {
        _t2740 = _t2738;
    } else {
        GemVal _t2739[] = {gem_v__match_51, gem_string("object")};
        _t2740 = gem_has_key_fn(NULL, _t2739, 2);
    }
    GemVal _t2742;
    if (!gem_truthy(_t2740)) {
        _t2742 = _t2740;
    } else {
        GemVal _t2741[] = {gem_v__match_51, gem_string("value")};
        _t2742 = gem_has_key_fn(NULL, _t2741, 2);
    }
    GemVal _t2744;
    if (!gem_truthy(_t2742)) {
        _t2744 = _t2742;
    } else {
        GemVal _t2743[] = {gem_v__match_51, gem_string("field")};
        _t2744 = gem_has_key_fn(NULL, _t2743, 2);
    }
    if (gem_truthy(_t2744)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_51, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_51, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_51, gem_string("field"));
#line 928 "compiler/main.gem"
    GemVal _t2745[] = {gem_v_object};
    GemVal _t2746 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2746.fn(_t2746.env, _t2745, 1);
#line 929 "compiler/main.gem"
    GemVal _t2747[] = {gem_v_value};
    GemVal _t2748 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2748.fn(_t2748.env, _t2747, 1);
#line 930 "compiler/main.gem"
    GemVal _t2749[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2749, 1);
#line 931 "compiler/main.gem"
    GemVal _t2750[] = {gem_v_line_dir};
    GemVal _t2751 = gem_v_obj_r;
    GemVal _t2752[] = {gem_table_get(_t2751, gem_string("setup"))};
    GemVal _t2753 = gem_v_val_r;
    GemVal _t2754[] = {gem_table_get(_t2753, gem_string("setup"))};
    GemVal _t2755[] = {gem_v_p};
    GemVal _t2756 = gem_v_obj_r;
    GemVal _t2757[] = {gem_table_get(_t2756, gem_string("expr"))};
    GemVal _t2758[] = {gem_v_escaped};
    GemVal _t2759 = gem_v_val_r;
    GemVal _t2760[] = {gem_table_get(_t2759, gem_string("expr"))};
        GemVal _t2761 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2750, 1), gem_to_string_fn(NULL, _t2752, 1)), gem_to_string_fn(NULL, _t2754, 1)), gem_to_string_fn(NULL, _t2755, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2757, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2758, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2760, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2761;
    } else {
    GemVal _t2762[] = {gem_v__match_51};
    GemVal _t2764;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2762, 1), gem_string("table")))) {
        _t2764 = gem_eq(gem_type_fn(NULL, _t2762, 1), gem_string("table"));
    } else {
        GemVal _t2763[] = {gem_v__match_51, gem_string("tag")};
        _t2764 = gem_has_key_fn(NULL, _t2763, 2);
    }
    GemVal _t2765;
    if (!gem_truthy(_t2764)) {
        _t2765 = _t2764;
    } else {
        _t2765 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t2767;
    if (!gem_truthy(_t2765)) {
        _t2767 = _t2765;
    } else {
        GemVal _t2766[] = {gem_v__match_51, gem_string("object")};
        _t2767 = gem_has_key_fn(NULL, _t2766, 2);
    }
    GemVal _t2769;
    if (!gem_truthy(_t2767)) {
        _t2769 = _t2767;
    } else {
        GemVal _t2768[] = {gem_v__match_51, gem_string("key")};
        _t2769 = gem_has_key_fn(NULL, _t2768, 2);
    }
    GemVal _t2771;
    if (!gem_truthy(_t2769)) {
        _t2771 = _t2769;
    } else {
        GemVal _t2770[] = {gem_v__match_51, gem_string("value")};
        _t2771 = gem_has_key_fn(NULL, _t2770, 2);
    }
    if (gem_truthy(_t2771)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_51, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_51, gem_string("key"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_51, gem_string("value"));
#line 933 "compiler/main.gem"
    GemVal _t2772[] = {gem_v_object};
    GemVal _t2773 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2773.fn(_t2773.env, _t2772, 1);
#line 934 "compiler/main.gem"
    GemVal _t2774[] = {gem_v_key};
    GemVal _t2775 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2775.fn(_t2775.env, _t2774, 1);
#line 935 "compiler/main.gem"
    GemVal _t2776[] = {gem_v_value};
    GemVal _t2777 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2777.fn(_t2777.env, _t2776, 1);
#line 936 "compiler/main.gem"
    GemVal _t2778[] = {gem_v_line_dir};
    GemVal _t2779 = gem_v_obj_r;
    GemVal _t2780[] = {gem_table_get(_t2779, gem_string("setup"))};
    GemVal _t2781 = gem_v_key_r;
    GemVal _t2782[] = {gem_table_get(_t2781, gem_string("setup"))};
    GemVal _t2783 = gem_v_val_r;
    GemVal _t2784[] = {gem_table_get(_t2783, gem_string("setup"))};
    GemVal _t2785[] = {gem_v_p};
    GemVal _t2786 = gem_v_obj_r;
    GemVal _t2787[] = {gem_table_get(_t2786, gem_string("expr"))};
    GemVal _t2788 = gem_v_key_r;
    GemVal _t2789[] = {gem_table_get(_t2788, gem_string("expr"))};
    GemVal _t2790 = gem_v_val_r;
    GemVal _t2791[] = {gem_table_get(_t2790, gem_string("expr"))};
        GemVal _t2792 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2778, 1), gem_to_string_fn(NULL, _t2780, 1)), gem_to_string_fn(NULL, _t2782, 1)), gem_to_string_fn(NULL, _t2784, 1)), gem_to_string_fn(NULL, _t2785, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2787, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2789, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2791, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2792;
    } else {
    GemVal _t2793[] = {gem_v__match_51};
    GemVal _t2795;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2793, 1), gem_string("table")))) {
        _t2795 = gem_eq(gem_type_fn(NULL, _t2793, 1), gem_string("table"));
    } else {
        GemVal _t2794[] = {gem_v__match_51, gem_string("tag")};
        _t2795 = gem_has_key_fn(NULL, _t2794, 2);
    }
    GemVal _t2796;
    if (!gem_truthy(_t2795)) {
        _t2796 = _t2795;
    } else {
        _t2796 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t2796)) {
#line 938 "compiler/main.gem"
    GemVal _t2797[] = {gem_v_line_dir};
    GemVal _t2798[] = {gem_v_node, gem_v_indent};
    GemVal _t2799 = (*gem_v_compile_if);
        GemVal _t2800 = gem_add(gem_to_string_fn(NULL, _t2797, 1), _t2799.fn(_t2799.env, _t2798, 2));
        gem_pop_frame();
        return _t2800;
    } else {
    GemVal _t2801[] = {gem_v__match_51};
    GemVal _t2803;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2801, 1), gem_string("table")))) {
        _t2803 = gem_eq(gem_type_fn(NULL, _t2801, 1), gem_string("table"));
    } else {
        GemVal _t2802[] = {gem_v__match_51, gem_string("tag")};
        _t2803 = gem_has_key_fn(NULL, _t2802, 2);
    }
    GemVal _t2804;
    if (!gem_truthy(_t2803)) {
        _t2804 = _t2803;
    } else {
        _t2804 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t2804)) {
#line 940 "compiler/main.gem"
    GemVal _t2805[] = {gem_v_line_dir};
    GemVal _t2806[] = {gem_v_node, gem_v_indent};
    GemVal _t2807 = (*gem_v_compile_while);
        GemVal _t2808 = gem_add(gem_to_string_fn(NULL, _t2805, 1), _t2807.fn(_t2807.env, _t2806, 2));
        gem_pop_frame();
        return _t2808;
    } else {
    GemVal _t2809[] = {gem_v__match_51};
    GemVal _t2811;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2809, 1), gem_string("table")))) {
        _t2811 = gem_eq(gem_type_fn(NULL, _t2809, 1), gem_string("table"));
    } else {
        GemVal _t2810[] = {gem_v__match_51, gem_string("tag")};
        _t2811 = gem_has_key_fn(NULL, _t2810, 2);
    }
    GemVal _t2812;
    if (!gem_truthy(_t2811)) {
        _t2812 = _t2811;
    } else {
        _t2812 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t2812)) {
#line 942 "compiler/main.gem"
    GemVal _t2813[] = {gem_v_line_dir};
    GemVal _t2814[] = {gem_v_node, gem_v_indent};
    GemVal _t2815 = (*gem_v_compile_match);
        GemVal _t2816 = gem_add(gem_to_string_fn(NULL, _t2813, 1), _t2815.fn(_t2815.env, _t2814, 2));
        gem_pop_frame();
        return _t2816;
    } else {
    GemVal _t2817[] = {gem_v__match_51};
    GemVal _t2819;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2817, 1), gem_string("table")))) {
        _t2819 = gem_eq(gem_type_fn(NULL, _t2817, 1), gem_string("table"));
    } else {
        GemVal _t2818[] = {gem_v__match_51, gem_string("tag")};
        _t2819 = gem_has_key_fn(NULL, _t2818, 2);
    }
    GemVal _t2820;
    if (!gem_truthy(_t2819)) {
        _t2820 = _t2819;
    } else {
        _t2820 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t2820)) {
#line 944 "compiler/main.gem"
    GemVal _t2821[] = {gem_v_line_dir};
    GemVal _t2822[] = {gem_v_node, gem_v_indent};
    GemVal _t2823 = (*gem_v_compile_receive_match);
        GemVal _t2824 = gem_add(gem_to_string_fn(NULL, _t2821, 1), _t2823.fn(_t2823.env, _t2822, 2));
        gem_pop_frame();
        return _t2824;
    } else {
    GemVal _t2825[] = {gem_v__match_51};
    GemVal _t2827;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2825, 1), gem_string("table")))) {
        _t2827 = gem_eq(gem_type_fn(NULL, _t2825, 1), gem_string("table"));
    } else {
        GemVal _t2826[] = {gem_v__match_51, gem_string("tag")};
        _t2827 = gem_has_key_fn(NULL, _t2826, 2);
    }
    GemVal _t2828;
    if (!gem_truthy(_t2827)) {
        _t2828 = _t2827;
    } else {
        _t2828 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t2828)) {
#line 946 "compiler/main.gem"
        GemVal _t2829 = gem_add(gem_v_p, gem_string("break;"));
        gem_pop_frame();
        return _t2829;
    } else {
    GemVal _t2830[] = {gem_v__match_51};
    GemVal _t2832;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2830, 1), gem_string("table")))) {
        _t2832 = gem_eq(gem_type_fn(NULL, _t2830, 1), gem_string("table"));
    } else {
        GemVal _t2831[] = {gem_v__match_51, gem_string("tag")};
        _t2832 = gem_has_key_fn(NULL, _t2831, 2);
    }
    GemVal _t2833;
    if (!gem_truthy(_t2832)) {
        _t2833 = _t2832;
    } else {
        _t2833 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t2833)) {
#line 948 "compiler/main.gem"
        GemVal _t2834 = gem_add(gem_v_p, gem_string("continue;"));
        gem_pop_frame();
        return _t2834;
    } else {
    GemVal _t2835[] = {gem_v__match_51};
    GemVal _t2837;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2835, 1), gem_string("table")))) {
        _t2837 = gem_eq(gem_type_fn(NULL, _t2835, 1), gem_string("table"));
    } else {
        GemVal _t2836[] = {gem_v__match_51, gem_string("tag")};
        _t2837 = gem_has_key_fn(NULL, _t2836, 2);
    }
    GemVal _t2838;
    if (!gem_truthy(_t2837)) {
        _t2838 = _t2837;
    } else {
        _t2838 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2840;
    if (!gem_truthy(_t2838)) {
        _t2840 = _t2838;
    } else {
        GemVal _t2839[] = {gem_v__match_51, gem_string("value")};
        _t2840 = gem_has_key_fn(NULL, _t2839, 2);
    }
    if (gem_truthy(_t2840)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_51, gem_string("value"));
#line 950 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 951 "compiler/main.gem"
    GemVal _t2841[] = {gem_v_value};
    GemVal _t2842 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2842.fn(_t2842.env, _t2841, 1);
#line 952 "compiler/main.gem"
    GemVal _t2843 = (*gem_v_tmp);
            GemVal gem_v_t = _t2843.fn(_t2843.env, NULL, 0);
#line 953 "compiler/main.gem"
    GemVal _t2844[] = {gem_v_line_dir};
    GemVal _t2845 = gem_v_r;
    GemVal _t2846[] = {gem_table_get(_t2845, gem_string("setup"))};
    GemVal _t2847[] = {gem_v_p};
    GemVal _t2848[] = {gem_v_t};
    GemVal _t2849 = gem_v_r;
    GemVal _t2850[] = {gem_table_get(_t2849, gem_string("expr"))};
    GemVal _t2851[] = {gem_v_p};
    GemVal _t2852[] = {gem_v_p};
    GemVal _t2853[] = {gem_v_t};
            GemVal _t2854 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2844, 1), gem_to_string_fn(NULL, _t2846, 1)), gem_to_string_fn(NULL, _t2847, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2848, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2850, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2851, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2852, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2853, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2854;
        }
#line 955 "compiler/main.gem"
    GemVal _t2855[] = {gem_v_line_dir};
    GemVal _t2856[] = {gem_v_p};
    GemVal _t2857[] = {gem_v_p};
        GemVal _t2858 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2855, 1), gem_to_string_fn(NULL, _t2856, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2857, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2858;
    } else {
    GemVal _t2859[] = {gem_v__match_51};
    GemVal _t2861;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2859, 1), gem_string("table")))) {
        _t2861 = gem_eq(gem_type_fn(NULL, _t2859, 1), gem_string("table"));
    } else {
        GemVal _t2860[] = {gem_v__match_51, gem_string("tag")};
        _t2861 = gem_has_key_fn(NULL, _t2860, 2);
    }
    GemVal _t2862;
    if (!gem_truthy(_t2861)) {
        _t2862 = _t2861;
    } else {
        _t2862 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2862)) {
#line 957 "compiler/main.gem"
    GemVal _t2863[] = {gem_v_node};
    GemVal _t2864 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2864.fn(_t2864.env, _t2863, 1);
#line 958 "compiler/main.gem"
    GemVal _t2865[] = {gem_v_line_dir};
    GemVal _t2866 = gem_v_r;
    GemVal _t2867[] = {gem_table_get(_t2866, gem_string("setup"))};
    GemVal _t2868[] = {gem_v_p};
    GemVal _t2869 = gem_v_r;
    GemVal _t2870[] = {gem_table_get(_t2869, gem_string("expr"))};
        GemVal _t2871 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2865, 1), gem_to_string_fn(NULL, _t2867, 1)), gem_to_string_fn(NULL, _t2868, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2870, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2871;
    } else {
    GemVal _t2872[] = {gem_v__match_51};
    GemVal _t2874;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2872, 1), gem_string("table")))) {
        _t2874 = gem_eq(gem_type_fn(NULL, _t2872, 1), gem_string("table"));
    } else {
        GemVal _t2873[] = {gem_v__match_51, gem_string("tag")};
        _t2874 = gem_has_key_fn(NULL, _t2873, 2);
    }
    GemVal _t2875;
    if (!gem_truthy(_t2874)) {
        _t2875 = _t2874;
    } else {
        _t2875 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t2875)) {
#line 960 "compiler/main.gem"
        GemVal _t2876 = gem_string("");
        gem_pop_frame();
        return _t2876;
    } else {
    GemVal _t2877[] = {gem_v__match_51};
    GemVal _t2879;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2877, 1), gem_string("table")))) {
        _t2879 = gem_eq(gem_type_fn(NULL, _t2877, 1), gem_string("table"));
    } else {
        GemVal _t2878[] = {gem_v__match_51, gem_string("tag")};
        _t2879 = gem_has_key_fn(NULL, _t2878, 2);
    }
    GemVal _t2880;
    if (!gem_truthy(_t2879)) {
        _t2880 = _t2879;
    } else {
        _t2880 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2882;
    if (!gem_truthy(_t2880)) {
        _t2882 = _t2880;
    } else {
        GemVal _t2881[] = {gem_v__match_51, gem_string("stmts")};
        _t2882 = gem_has_key_fn(NULL, _t2881, 2);
    }
    if (gem_truthy(_t2882)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_51, gem_string("stmts"));
#line 962 "compiler/main.gem"
        GemVal gem_v_out = gem_string("");
#line 963 "compiler/main.gem"
        GemVal gem_v__for_items_52 = gem_v_stmts;
#line 963 "compiler/main.gem"
        GemVal gem_v__for_i_52 = gem_int(0);
#line 963 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t2883[] = {gem_v__for_items_52};
            if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_len_fn(NULL, _t2883, 1)))) break;
#line 963 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_52, gem_v__for_i_52);
#line 963 "compiler/main.gem"
            gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 964 "compiler/main.gem"
    GemVal _t2884[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2885 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2885.fn(_t2885.env, _t2884, 2)), gem_string("\n"));
        }

#line 966 "compiler/main.gem"
        GemVal _t2886 = gem_v_out;
        gem_pop_frame();
        return _t2886;
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
#line 969 "compiler/main.gem"
    GemVal _t2887[] = {gem_v_node};
    GemVal _t2888 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t2888.fn(_t2888.env, _t2887, 1);
    GemVal _t2889[] = {gem_v_line_dir};
    GemVal _t2890 = gem_v_r;
    GemVal _t2891[] = {gem_table_get(_t2890, gem_string("setup"))};
    GemVal _t2892[] = {gem_v_p};
    GemVal _t2893 = gem_v_r;
    GemVal _t2894[] = {gem_table_get(_t2893, gem_string("expr"))};
    GemVal _t2895 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2889, 1), gem_to_string_fn(NULL, _t2891, 1)), gem_to_string_fn(NULL, _t2892, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2894, 1)), gem_string(");"));
    gem_pop_frame();
    return _t2895;
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
#line 976 "compiler/main.gem"
    GemVal _t2897[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2897, 1);
#line 977 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 978 "compiler/main.gem"
    GemVal _t2898 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2898, gem_string("line")), GEM_NIL))) {
#line 979 "compiler/main.gem"
    GemVal _t2899 = gem_v_node;
    GemVal _t2900[] = {gem_table_get(_t2899, gem_string("line"))};
    GemVal _t2901[] = {(*gem_v_source_name)};
    GemVal _t2902[] = {gem_fn_escape_c_string(NULL, _t2901, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2900, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2902, 1)), gem_string("\"\n"));
    }
#line 982 "compiler/main.gem"
    GemVal gem_v__match_53 = gem_v_node;
    GemVal _t2903[] = {gem_v__match_53};
    GemVal _t2905;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2903, 1), gem_string("table")))) {
        _t2905 = gem_eq(gem_type_fn(NULL, _t2903, 1), gem_string("table"));
    } else {
        GemVal _t2904[] = {gem_v__match_53, gem_string("tag")};
        _t2905 = gem_has_key_fn(NULL, _t2904, 2);
    }
    GemVal _t2906;
    if (!gem_truthy(_t2905)) {
        _t2906 = _t2905;
    } else {
        _t2906 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2908;
    if (!gem_truthy(_t2906)) {
        _t2908 = _t2906;
    } else {
        GemVal _t2907[] = {gem_v__match_53, gem_string("name")};
        _t2908 = gem_has_key_fn(NULL, _t2907, 2);
    }
    GemVal _t2910;
    if (!gem_truthy(_t2908)) {
        _t2910 = _t2908;
    } else {
        GemVal _t2909[] = {gem_v__match_53, gem_string("value")};
        _t2910 = gem_has_key_fn(NULL, _t2909, 2);
    }
    if (gem_truthy(_t2910)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_53, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_53, gem_string("value"));
#line 984 "compiler/main.gem"
    GemVal _t2911[] = {gem_v_value};
    GemVal _t2912 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2912.fn(_t2912.env, _t2911, 1);
#line 985 "compiler/main.gem"
    GemVal _t2913[] = {gem_v_name};
    GemVal _t2914 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2914.fn(_t2914.env, _t2913, 1);
#line 986 "compiler/main.gem"
    GemVal _t2915[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2915, 2))) {
#line 987 "compiler/main.gem"
    GemVal _t2916[] = {gem_v_line_dir};
    GemVal _t2917 = gem_v_r;
    GemVal _t2918[] = {gem_table_get(_t2917, gem_string("setup"))};
    GemVal _t2919[] = {gem_v_p};
    GemVal _t2920[] = {gem_v_mname};
    GemVal _t2921[] = {gem_v_p};
    GemVal _t2922[] = {gem_v_mname};
    GemVal _t2923 = gem_v_r;
    GemVal _t2924[] = {gem_table_get(_t2923, gem_string("expr"))};
    GemVal _t2925[] = {gem_v_p};
    GemVal _t2926[] = {gem_v_p};
            GemVal _t2927 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2916, 1), gem_to_string_fn(NULL, _t2918, 1)), gem_to_string_fn(NULL, _t2919, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2920, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2921, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2922, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2924, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2925, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2926, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t2927;
        }
#line 989 "compiler/main.gem"
    GemVal _t2928[] = {gem_v_line_dir};
    GemVal _t2929 = gem_v_r;
    GemVal _t2930[] = {gem_table_get(_t2929, gem_string("setup"))};
    GemVal _t2931[] = {gem_v_p};
    GemVal _t2932[] = {gem_v_mname};
    GemVal _t2933 = gem_v_r;
    GemVal _t2934[] = {gem_table_get(_t2933, gem_string("expr"))};
    GemVal _t2935[] = {gem_v_p};
    GemVal _t2936[] = {gem_v_p};
        GemVal _t2937 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2928, 1), gem_to_string_fn(NULL, _t2930, 1)), gem_to_string_fn(NULL, _t2931, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2932, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2934, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2935, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2936, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2937;
    } else {
    GemVal _t2938[] = {gem_v__match_53};
    GemVal _t2940;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2938, 1), gem_string("table")))) {
        _t2940 = gem_eq(gem_type_fn(NULL, _t2938, 1), gem_string("table"));
    } else {
        GemVal _t2939[] = {gem_v__match_53, gem_string("tag")};
        _t2940 = gem_has_key_fn(NULL, _t2939, 2);
    }
    GemVal _t2941;
    if (!gem_truthy(_t2940)) {
        _t2941 = _t2940;
    } else {
        _t2941 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2943;
    if (!gem_truthy(_t2941)) {
        _t2943 = _t2941;
    } else {
        GemVal _t2942[] = {gem_v__match_53, gem_string("name")};
        _t2943 = gem_has_key_fn(NULL, _t2942, 2);
    }
    GemVal _t2945;
    if (!gem_truthy(_t2943)) {
        _t2945 = _t2943;
    } else {
        GemVal _t2944[] = {gem_v__match_53, gem_string("value")};
        _t2945 = gem_has_key_fn(NULL, _t2944, 2);
    }
    if (gem_truthy(_t2945)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_53, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_53, gem_string("value"));
#line 991 "compiler/main.gem"
    GemVal _t2946[] = {gem_v_value};
    GemVal _t2947 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2947.fn(_t2947.env, _t2946, 1);
#line 992 "compiler/main.gem"
    GemVal _t2948 = (*gem_v_tmp);
        GemVal gem_v_t = _t2948.fn(_t2948.env, NULL, 0);
#line 993 "compiler/main.gem"
    GemVal _t2949[] = {gem_v_name};
    GemVal _t2950 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2950.fn(_t2950.env, _t2949, 1);
#line 994 "compiler/main.gem"
    GemVal _t2951[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2953;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2951, 2))) {
        _t2953 = gem_fn_set_contains(NULL, _t2951, 2);
    } else {
        GemVal _t2952[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2953 = gem_fn_set_contains(NULL, _t2952, 2);
    }
        if (gem_truthy(_t2953)) {
#line 995 "compiler/main.gem"
    GemVal _t2954[] = {gem_v_line_dir};
    GemVal _t2955 = gem_v_r;
    GemVal _t2956[] = {gem_table_get(_t2955, gem_string("setup"))};
    GemVal _t2957[] = {gem_v_p};
    GemVal _t2958[] = {gem_v_t};
    GemVal _t2959 = gem_v_r;
    GemVal _t2960[] = {gem_table_get(_t2959, gem_string("expr"))};
    GemVal _t2961[] = {gem_v_p};
    GemVal _t2962[] = {gem_v_mname};
    GemVal _t2963[] = {gem_v_t};
    GemVal _t2964[] = {gem_v_p};
    GemVal _t2965[] = {gem_v_p};
    GemVal _t2966[] = {gem_v_t};
            GemVal _t2967 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2954, 1), gem_to_string_fn(NULL, _t2956, 1)), gem_to_string_fn(NULL, _t2957, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2958, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2960, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2961, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2962, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2963, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2964, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2965, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2966, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2967;
        }
#line 997 "compiler/main.gem"
    GemVal _t2968[] = {gem_v_line_dir};
    GemVal _t2969 = gem_v_r;
    GemVal _t2970[] = {gem_table_get(_t2969, gem_string("setup"))};
    GemVal _t2971[] = {gem_v_p};
    GemVal _t2972[] = {gem_v_t};
    GemVal _t2973 = gem_v_r;
    GemVal _t2974[] = {gem_table_get(_t2973, gem_string("expr"))};
    GemVal _t2975[] = {gem_v_p};
    GemVal _t2976[] = {gem_v_mname};
    GemVal _t2977[] = {gem_v_t};
    GemVal _t2978[] = {gem_v_p};
    GemVal _t2979[] = {gem_v_p};
    GemVal _t2980[] = {gem_v_t};
        GemVal _t2981 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2968, 1), gem_to_string_fn(NULL, _t2970, 1)), gem_to_string_fn(NULL, _t2971, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2972, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2974, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2975, 1)), gem_to_string_fn(NULL, _t2976, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2977, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2978, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2979, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2980, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2981;
    } else {
    GemVal _t2982[] = {gem_v__match_53};
    GemVal _t2984;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2982, 1), gem_string("table")))) {
        _t2984 = gem_eq(gem_type_fn(NULL, _t2982, 1), gem_string("table"));
    } else {
        GemVal _t2983[] = {gem_v__match_53, gem_string("tag")};
        _t2984 = gem_has_key_fn(NULL, _t2983, 2);
    }
    GemVal _t2985;
    if (!gem_truthy(_t2984)) {
        _t2985 = _t2984;
    } else {
        _t2985 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2987;
    if (!gem_truthy(_t2985)) {
        _t2987 = _t2985;
    } else {
        GemVal _t2986[] = {gem_v__match_53, gem_string("value")};
        _t2987 = gem_has_key_fn(NULL, _t2986, 2);
    }
    if (gem_truthy(_t2987)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_53, gem_string("value"));
#line 999 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1000 "compiler/main.gem"
    GemVal _t2988[] = {gem_v_value};
    GemVal _t2989 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2989.fn(_t2989.env, _t2988, 1);
#line 1001 "compiler/main.gem"
    GemVal _t2990 = (*gem_v_tmp);
            GemVal gem_v_t = _t2990.fn(_t2990.env, NULL, 0);
#line 1002 "compiler/main.gem"
    GemVal _t2991[] = {gem_v_line_dir};
    GemVal _t2992 = gem_v_r;
    GemVal _t2993[] = {gem_table_get(_t2992, gem_string("setup"))};
    GemVal _t2994[] = {gem_v_p};
    GemVal _t2995[] = {gem_v_t};
    GemVal _t2996 = gem_v_r;
    GemVal _t2997[] = {gem_table_get(_t2996, gem_string("expr"))};
    GemVal _t2998[] = {gem_v_p};
    GemVal _t2999[] = {gem_v_p};
    GemVal _t3000[] = {gem_v_t};
            GemVal _t3001 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2991, 1), gem_to_string_fn(NULL, _t2993, 1)), gem_to_string_fn(NULL, _t2994, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2995, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2997, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2998, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2999, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3000, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3001;
        }
#line 1004 "compiler/main.gem"
    GemVal _t3002[] = {gem_v_line_dir};
    GemVal _t3003[] = {gem_v_p};
    GemVal _t3004[] = {gem_v_p};
        GemVal _t3005 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3002, 1), gem_to_string_fn(NULL, _t3003, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3004, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3005;
    } else {
    GemVal _t3006[] = {gem_v__match_53};
    GemVal _t3008;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3006, 1), gem_string("table")))) {
        _t3008 = gem_eq(gem_type_fn(NULL, _t3006, 1), gem_string("table"));
    } else {
        GemVal _t3007[] = {gem_v__match_53, gem_string("tag")};
        _t3008 = gem_has_key_fn(NULL, _t3007, 2);
    }
    GemVal _t3009;
    if (!gem_truthy(_t3008)) {
        _t3009 = _t3008;
    } else {
        _t3009 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3009)) {
#line 1006 "compiler/main.gem"
    GemVal _t3010[] = {gem_v_line_dir};
    GemVal _t3011[] = {gem_v_node, gem_v_indent};
    GemVal _t3012 = (*gem_v_compile_if_return);
        GemVal _t3013 = gem_add(gem_to_string_fn(NULL, _t3010, 1), _t3012.fn(_t3012.env, _t3011, 2));
        gem_pop_frame();
        return _t3013;
    } else {
    GemVal _t3014[] = {gem_v__match_53};
    GemVal _t3016;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3014, 1), gem_string("table")))) {
        _t3016 = gem_eq(gem_type_fn(NULL, _t3014, 1), gem_string("table"));
    } else {
        GemVal _t3015[] = {gem_v__match_53, gem_string("tag")};
        _t3016 = gem_has_key_fn(NULL, _t3015, 2);
    }
    GemVal _t3017;
    if (!gem_truthy(_t3016)) {
        _t3017 = _t3016;
    } else {
        _t3017 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3017)) {
#line 1008 "compiler/main.gem"
    GemVal _t3018[] = {gem_v_line_dir};
    GemVal _t3019[] = {gem_v_node, gem_v_indent};
    GemVal _t3020 = (*gem_v_compile_while);
    GemVal _t3021[] = {_t3020.fn(_t3020.env, _t3019, 2)};
    GemVal _t3022[] = {gem_v_p};
    GemVal _t3023[] = {gem_v_p};
        GemVal _t3024 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3018, 1), gem_to_string_fn(NULL, _t3021, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t3022, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3023, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3024;
    } else {
    GemVal _t3025[] = {gem_v__match_53};
    GemVal _t3027;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3025, 1), gem_string("table")))) {
        _t3027 = gem_eq(gem_type_fn(NULL, _t3025, 1), gem_string("table"));
    } else {
        GemVal _t3026[] = {gem_v__match_53, gem_string("tag")};
        _t3027 = gem_has_key_fn(NULL, _t3026, 2);
    }
    GemVal _t3028;
    if (!gem_truthy(_t3027)) {
        _t3028 = _t3027;
    } else {
        _t3028 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3028)) {
#line 1010 "compiler/main.gem"
    GemVal _t3029[] = {gem_v_line_dir};
    GemVal _t3030[] = {gem_v_node, gem_v_indent};
    GemVal _t3031 = (*gem_v_compile_match_return);
        GemVal _t3032 = gem_add(gem_to_string_fn(NULL, _t3029, 1), _t3031.fn(_t3031.env, _t3030, 2));
        gem_pop_frame();
        return _t3032;
    } else {
    GemVal _t3033[] = {gem_v__match_53};
    GemVal _t3035;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3033, 1), gem_string("table")))) {
        _t3035 = gem_eq(gem_type_fn(NULL, _t3033, 1), gem_string("table"));
    } else {
        GemVal _t3034[] = {gem_v__match_53, gem_string("tag")};
        _t3035 = gem_has_key_fn(NULL, _t3034, 2);
    }
    GemVal _t3036;
    if (!gem_truthy(_t3035)) {
        _t3036 = _t3035;
    } else {
        _t3036 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3036)) {
#line 1012 "compiler/main.gem"
    GemVal _t3037[] = {gem_v_line_dir};
    GemVal _t3038[] = {gem_v_node, gem_v_indent};
    GemVal _t3039 = (*gem_v_compile_receive_match_return);
        GemVal _t3040 = gem_add(gem_to_string_fn(NULL, _t3037, 1), _t3039.fn(_t3039.env, _t3038, 2));
        gem_pop_frame();
        return _t3040;
    } else {
    GemVal _t3041[] = {gem_v__match_53};
    GemVal _t3043;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3041, 1), gem_string("table")))) {
        _t3043 = gem_eq(gem_type_fn(NULL, _t3041, 1), gem_string("table"));
    } else {
        GemVal _t3042[] = {gem_v__match_53, gem_string("tag")};
        _t3043 = gem_has_key_fn(NULL, _t3042, 2);
    }
    GemVal _t3044;
    if (!gem_truthy(_t3043)) {
        _t3044 = _t3043;
    } else {
        _t3044 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3046;
    if (!gem_truthy(_t3044)) {
        _t3046 = _t3044;
    } else {
        GemVal _t3045[] = {gem_v__match_53, gem_string("stmts")};
        _t3046 = gem_has_key_fn(NULL, _t3045, 2);
    }
    if (gem_truthy(_t3046)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_53, gem_string("stmts"));
#line 1014 "compiler/main.gem"
    GemVal _t3047[] = {gem_v_p};
        GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t3047, 1), gem_string("{\n"));
#line 1015 "compiler/main.gem"
        GemVal gem_v__for_i_54 = gem_int(0);
#line 1015 "compiler/main.gem"
    GemVal _t3048[] = {gem_v_stmts};
        GemVal gem_v__for_limit_54 = gem_sub(gem_len_fn(NULL, _t3048, 1), gem_int(1));
#line 1015 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_v__for_limit_54))) break;
#line 1015 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_54;
#line 1015 "compiler/main.gem"
            gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 1016 "compiler/main.gem"
    GemVal _t3049[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3050 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3050.fn(_t3050.env, _t3049, 2)), gem_string("\n"));
        }

#line 1018 "compiler/main.gem"
    GemVal _t3051[] = {gem_v_stmts};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3051, 1), gem_int(0)))) {
#line 1019 "compiler/main.gem"
    GemVal _t3052[] = {gem_v_stmts};
    GemVal _t3053[] = {gem_table_get(gem_v_stmts, gem_sub(gem_len_fn(NULL, _t3052, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3054 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3054.fn(_t3054.env, _t3053, 2)), gem_string("\n"));
        }
#line 1021 "compiler/main.gem"
    GemVal _t3055[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3055, 1), gem_string("}")));
#line 1022 "compiler/main.gem"
        GemVal _t3056 = gem_v_out;
        gem_pop_frame();
        return _t3056;
    } else {
    GemVal _t3057[] = {gem_v__match_53};
    GemVal _t3059;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3057, 1), gem_string("table")))) {
        _t3059 = gem_eq(gem_type_fn(NULL, _t3057, 1), gem_string("table"));
    } else {
        GemVal _t3058[] = {gem_v__match_53, gem_string("tag")};
        _t3059 = gem_has_key_fn(NULL, _t3058, 2);
    }
    GemVal _t3060;
    if (!gem_truthy(_t3059)) {
        _t3060 = _t3059;
    } else {
        _t3060 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("dot_assign"));
    }
    if (gem_truthy(_t3060)) {
#line 1024 "compiler/main.gem"
    GemVal _t3061[] = {gem_v_node, gem_v_indent};
    GemVal _t3062 = (*gem_v_compile_stmt);
    GemVal _t3063[] = {_t3062.fn(_t3062.env, _t3061, 2)};
    GemVal _t3064[] = {gem_v_p};
    GemVal _t3065[] = {gem_v_p};
        GemVal _t3066 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3063, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3064, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3065, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3066;
    } else {
    GemVal _t3067[] = {gem_v__match_53};
    GemVal _t3069;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3067, 1), gem_string("table")))) {
        _t3069 = gem_eq(gem_type_fn(NULL, _t3067, 1), gem_string("table"));
    } else {
        GemVal _t3068[] = {gem_v__match_53, gem_string("tag")};
        _t3069 = gem_has_key_fn(NULL, _t3068, 2);
    }
    GemVal _t3070;
    if (!gem_truthy(_t3069)) {
        _t3070 = _t3069;
    } else {
        _t3070 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("index_assign"));
    }
    if (gem_truthy(_t3070)) {
#line 1026 "compiler/main.gem"
    GemVal _t3071[] = {gem_v_node, gem_v_indent};
    GemVal _t3072 = (*gem_v_compile_stmt);
    GemVal _t3073[] = {_t3072.fn(_t3072.env, _t3071, 2)};
    GemVal _t3074[] = {gem_v_p};
    GemVal _t3075[] = {gem_v_p};
        GemVal _t3076 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3073, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3074, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3075, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3076;
    } else {
    GemVal _t3077[] = {gem_v__match_53};
    GemVal _t3079;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3077, 1), gem_string("table")))) {
        _t3079 = gem_eq(gem_type_fn(NULL, _t3077, 1), gem_string("table"));
    } else {
        GemVal _t3078[] = {gem_v__match_53, gem_string("tag")};
        _t3079 = gem_has_key_fn(NULL, _t3078, 2);
    }
    GemVal _t3080;
    if (!gem_truthy(_t3079)) {
        _t3080 = _t3079;
    } else {
        _t3080 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3080)) {
#line 1028 "compiler/main.gem"
    GemVal _t3081[] = {gem_v_node, gem_v_indent};
    GemVal _t3082 = (*gem_v_compile_stmt);
    GemVal _t3083[] = {_t3082.fn(_t3082.env, _t3081, 2)};
    GemVal _t3084[] = {gem_v_p};
    GemVal _t3085[] = {gem_v_p};
        GemVal _t3086 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3083, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3084, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3085, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3086;
    } else {
    GemVal _t3087[] = {gem_v__match_53};
    GemVal _t3089;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3087, 1), gem_string("table")))) {
        _t3089 = gem_eq(gem_type_fn(NULL, _t3087, 1), gem_string("table"));
    } else {
        GemVal _t3088[] = {gem_v__match_53, gem_string("tag")};
        _t3089 = gem_has_key_fn(NULL, _t3088, 2);
    }
    GemVal _t3090;
    if (!gem_truthy(_t3089)) {
        _t3090 = _t3089;
    } else {
        _t3090 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3090)) {
#line 1030 "compiler/main.gem"
    GemVal _t3091[] = {gem_v_node, gem_v_indent};
    GemVal _t3092 = (*gem_v_compile_stmt);
    GemVal _t3093[] = {_t3092.fn(_t3092.env, _t3091, 2)};
    GemVal _t3094[] = {gem_v_p};
    GemVal _t3095[] = {gem_v_p};
        GemVal _t3096 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3093, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3094, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3095, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3096;
    } else {
    GemVal _t3097[] = {gem_v__match_53};
    GemVal _t3099;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3097, 1), gem_string("table")))) {
        _t3099 = gem_eq(gem_type_fn(NULL, _t3097, 1), gem_string("table"));
    } else {
        GemVal _t3098[] = {gem_v__match_53, gem_string("tag")};
        _t3099 = gem_has_key_fn(NULL, _t3098, 2);
    }
    GemVal _t3100;
    if (!gem_truthy(_t3099)) {
        _t3100 = _t3099;
    } else {
        _t3100 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3100)) {
#line 1032 "compiler/main.gem"
    GemVal _t3101[] = {gem_v_node, gem_v_indent};
    GemVal _t3102 = (*gem_v_compile_stmt);
    GemVal _t3103[] = {_t3102.fn(_t3102.env, _t3101, 2)};
    GemVal _t3104[] = {gem_v_p};
    GemVal _t3105[] = {gem_v_p};
        GemVal _t3106 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3103, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3104, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3105, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3106;
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
#line 1035 "compiler/main.gem"
    GemVal _t3107[] = {gem_v_node};
    GemVal _t3108 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3108.fn(_t3108.env, _t3107, 1);
#line 1036 "compiler/main.gem"
    GemVal _t3109 = (*gem_v_tmp);
    GemVal gem_v_t = _t3109.fn(_t3109.env, NULL, 0);
    GemVal _t3110[] = {gem_v_line_dir};
    GemVal _t3111 = gem_v_r;
    GemVal _t3112[] = {gem_table_get(_t3111, gem_string("setup"))};
    GemVal _t3113[] = {gem_v_p};
    GemVal _t3114[] = {gem_v_t};
    GemVal _t3115 = gem_v_r;
    GemVal _t3116[] = {gem_table_get(_t3115, gem_string("expr"))};
    GemVal _t3117[] = {gem_v_p};
    GemVal _t3118[] = {gem_v_p};
    GemVal _t3119[] = {gem_v_t};
    GemVal _t3120 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3110, 1), gem_to_string_fn(NULL, _t3112, 1)), gem_to_string_fn(NULL, _t3113, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3114, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3116, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3117, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3118, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3119, 1)), gem_string(";"));
    gem_pop_frame();
    return _t3120;
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
#line 1043 "compiler/main.gem"
    GemVal _t3122[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3122, 1);
#line 1044 "compiler/main.gem"
    GemVal _t3123 = gem_v_node;
    GemVal _t3124[] = {gem_table_get(_t3123, gem_string("cond"))};
    GemVal _t3125 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3125.fn(_t3125.env, _t3124, 1);
#line 1045 "compiler/main.gem"
    GemVal _t3126 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t3126, gem_string("setup"));
#line 1046 "compiler/main.gem"
    GemVal _t3127[] = {gem_v_p};
    GemVal _t3128 = gem_v_cond_r;
    GemVal _t3129[] = {gem_table_get(_t3128, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3127, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3129, 1)), gem_string(")) {\n")));
#line 1047 "compiler/main.gem"
    GemVal _t3130 = gem_v_node;
    GemVal _t3131[] = {gem_table_get(_t3130, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3132 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t3132.fn(_t3132.env, _t3131, 2));
#line 1048 "compiler/main.gem"
    GemVal _t3133 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3133, gem_string("else")), GEM_NIL))) {
#line 1049 "compiler/main.gem"
    GemVal _t3134[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3134, 1), gem_string("} else {\n")));
#line 1050 "compiler/main.gem"
    GemVal _t3135 = gem_v_node;
    GemVal _t3136[] = {gem_table_get(_t3135, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3137 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3137.fn(_t3137.env, _t3136, 2));
    }
#line 1052 "compiler/main.gem"
    GemVal _t3138[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3138, 1), gem_string("}")));
    GemVal _t3139 = gem_v_out;
    gem_pop_frame();
    return _t3139;
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
#line 1059 "compiler/main.gem"
    GemVal _t3141[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3141, 1);
#line 1060 "compiler/main.gem"
    GemVal _t3142 = gem_v_node;
    GemVal _t3143[] = {gem_table_get(_t3142, gem_string("cond"))};
    GemVal _t3144 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3144.fn(_t3144.env, _t3143, 1);
#line 1061 "compiler/main.gem"
    GemVal _t3145 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t3145, gem_string("setup"));
#line 1062 "compiler/main.gem"
    GemVal _t3146[] = {gem_v_p};
    GemVal _t3147 = gem_v_cond_r;
    GemVal _t3148[] = {gem_table_get(_t3147, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3146, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3148, 1)), gem_string(")) {\n")));
#line 1063 "compiler/main.gem"
    GemVal _t3149 = gem_v_node;
    GemVal gem_v_then_body = gem_table_get(_t3149, gem_string("then"));
#line 1064 "compiler/main.gem"
    GemVal _t3150[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3150, 1), gem_int(0)))) {
#line 1065 "compiler/main.gem"
        GemVal gem_v__for_i_55 = gem_int(0);
#line 1065 "compiler/main.gem"
    GemVal _t3151[] = {gem_v_then_body};
        GemVal gem_v__for_limit_55 = gem_sub(gem_len_fn(NULL, _t3151, 1), gem_int(1));
#line 1065 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_v__for_limit_55))) break;
#line 1065 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_55;
#line 1065 "compiler/main.gem"
            gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1066 "compiler/main.gem"
    GemVal _t3152[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3153 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3153.fn(_t3153.env, _t3152, 2)), gem_string("\n"));
        }

#line 1068 "compiler/main.gem"
    GemVal _t3154[] = {gem_v_then_body};
    GemVal _t3155[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t3154, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3156 = (*gem_v_compile_stmt_return);
        gem_v_out = gem_add(gem_add(gem_v_out, _t3156.fn(_t3156.env, _t3155, 2)), gem_string("\n"));
    } else {
#line 1070 "compiler/main.gem"
    GemVal _t3157[] = {gem_v_p};
    GemVal _t3158[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3157, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3158, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1072 "compiler/main.gem"
    GemVal _t3159 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3159, gem_string("else")), GEM_NIL))) {
#line 1073 "compiler/main.gem"
    GemVal _t3160[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3160, 1), gem_string("} else {\n")));
#line 1074 "compiler/main.gem"
    GemVal _t3161 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t3161, gem_string("else"));
#line 1075 "compiler/main.gem"
    GemVal _t3162[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3162, 1), gem_int(0)))) {
#line 1076 "compiler/main.gem"
            GemVal gem_v__for_i_56 = gem_int(0);
#line 1076 "compiler/main.gem"
    GemVal _t3163[] = {gem_v_else_body};
            GemVal gem_v__for_limit_56 = gem_sub(gem_len_fn(NULL, _t3163, 1), gem_int(1));
#line 1076 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_v__for_limit_56))) break;
#line 1076 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_56;
#line 1076 "compiler/main.gem"
                gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1077 "compiler/main.gem"
    GemVal _t3164[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3165 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3165.fn(_t3165.env, _t3164, 2)), gem_string("\n"));
            }

#line 1079 "compiler/main.gem"
    GemVal _t3166[] = {gem_v_else_body};
    GemVal _t3167[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t3166, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3168 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3168.fn(_t3168.env, _t3167, 2)), gem_string("\n"));
        } else {
#line 1081 "compiler/main.gem"
    GemVal _t3169[] = {gem_v_p};
    GemVal _t3170[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3169, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3170, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1084 "compiler/main.gem"
    GemVal _t3171[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3171, 1), gem_string("} else {\n")));
#line 1085 "compiler/main.gem"
    GemVal _t3172[] = {gem_v_p};
    GemVal _t3173[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3172, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3173, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1087 "compiler/main.gem"
    GemVal _t3174[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3174, 1), gem_string("}")));
    GemVal _t3175 = gem_v_out;
    gem_pop_frame();
    return _t3175;
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
#line 1094 "compiler/main.gem"
    GemVal _t3177[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3177, 1);
#line 1095 "compiler/main.gem"
    GemVal _t3178 = gem_v_node;
    GemVal _t3179[] = {gem_table_get(_t3178, gem_string("cond"))};
    GemVal _t3180 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3180.fn(_t3180.env, _t3179, 1);
#line 1096 "compiler/main.gem"
    GemVal _t3181[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t3181, 1), gem_string("while (1) {\n"));
#line 1097 "compiler/main.gem"
    GemVal _t3182[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3182, 1), gem_string("    gem_yield_check();\n")));
#line 1098 "compiler/main.gem"
    GemVal _t3183 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t3183, gem_string("setup")), gem_string("")))) {
#line 1099 "compiler/main.gem"
    GemVal _t3184 = gem_v_cond_r;
    GemVal _t3185[] = {gem_v_p};
    GemVal _t3186[] = {gem_table_get(_t3184, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t3185, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_str_replace_fn(NULL, _t3186, 3));
    }
#line 1101 "compiler/main.gem"
    GemVal _t3187[] = {gem_v_p};
    GemVal _t3188 = gem_v_cond_r;
    GemVal _t3189[] = {gem_table_get(_t3188, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3187, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t3189, 1)), gem_string(")) break;\n")));
#line 1102 "compiler/main.gem"
    GemVal _t3190 = gem_v_node;
    GemVal _t3191[] = {gem_table_get(_t3190, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3192 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t3192.fn(_t3192.env, _t3191, 2));
#line 1103 "compiler/main.gem"
    GemVal _t3193[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3193, 1), gem_string("}")));
    GemVal _t3194 = gem_v_out;
    gem_pop_frame();
    return _t3194;
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
#line 1110 "compiler/main.gem"
    GemVal _t3196[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3196, 1);
#line 1111 "compiler/main.gem"
    GemVal _t3197 = gem_v_node;
    GemVal _t3198[] = {gem_table_get(_t3197, gem_string("target"))};
    GemVal _t3199 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3199.fn(_t3199.env, _t3198, 1);
#line 1112 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1113 "compiler/main.gem"
    GemVal _t3200 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3200, gem_string("target_var")), GEM_NIL))) {
#line 1114 "compiler/main.gem"
    GemVal _t3201 = gem_v_node;
    GemVal _t3202[] = {gem_table_get(_t3201, gem_string("target_var"))};
    GemVal _t3203 = (*gem_v_mangle);
        gem_v_t = _t3203.fn(_t3203.env, _t3202, 1);
    } else {
#line 1116 "compiler/main.gem"
    GemVal _t3204 = (*gem_v_tmp);
        gem_v_t = _t3204.fn(_t3204.env, NULL, 0);
    }
#line 1118 "compiler/main.gem"
    GemVal _t3205 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t3205, gem_string("setup"));
#line 1119 "compiler/main.gem"
    GemVal _t3206[] = {gem_v_p};
    GemVal _t3207[] = {gem_v_t};
    GemVal _t3208 = gem_v_target_r;
    GemVal _t3209[] = {gem_table_get(_t3208, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3206, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3207, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3209, 1)), gem_string(";\n")));
#line 1120 "compiler/main.gem"
    GemVal _t3210 = gem_v_node;
    GemVal _t3211[] = {gem_table_get(_t3210, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3211, 1), gem_int(0)))) {
#line 1122 "compiler/main.gem"
    GemVal _t3212 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3212, gem_string("else")), GEM_NIL))) {
#line 1123 "compiler/main.gem"
    GemVal _t3213 = gem_v_node;
    GemVal _t3214[] = {gem_table_get(_t3213, gem_string("else")), gem_v_indent};
    GemVal _t3215 = (*gem_v_compile_stmts);
            gem_v_out = gem_add(gem_v_out, _t3215.fn(_t3215.env, _t3214, 2));
        }
#line 1125 "compiler/main.gem"
    GemVal _t3216[] = {gem_v_p};
    GemVal _t3217[] = {gem_v_t};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3216, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t3217, 1)), gem_string(";")));
#line 1126 "compiler/main.gem"
        GemVal _t3218 = gem_v_out;
        gem_pop_frame();
        return _t3218;
    }
#line 1128 "compiler/main.gem"
    GemVal gem_v__for_i_58 = gem_int(0);
#line 1128 "compiler/main.gem"
    GemVal _t3219 = gem_v_node;
    GemVal _t3220[] = {gem_table_get(_t3219, gem_string("whens"))};
    GemVal gem_v__for_limit_58 = gem_len_fn(NULL, _t3220, 1);
#line 1128 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_v__for_limit_58))) break;
#line 1128 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_58;
#line 1128 "compiler/main.gem"
        gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1129 "compiler/main.gem"
    GemVal _t3221 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t3221, gem_string("whens")), gem_v_i);
#line 1130 "compiler/main.gem"
    GemVal _t3222 = gem_v_w;
    GemVal _t3223[] = {gem_table_get(_t3222, gem_string("value"))};
    GemVal _t3224 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3224.fn(_t3224.env, _t3223, 1);
#line 1131 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1132 "compiler/main.gem"
    GemVal _t3225 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3225, gem_string("setup")));
#line 1133 "compiler/main.gem"
    GemVal _t3226[] = {gem_v_p};
    GemVal _t3227 = gem_v_val_r;
    GemVal _t3228[] = {gem_table_get(_t3227, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3226, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3228, 1)), gem_string(")) {\n")));
        } else {
#line 1135 "compiler/main.gem"
    GemVal _t3229[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3229, 1), gem_string("} else {\n")));
#line 1136 "compiler/main.gem"
    GemVal _t3230 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3230, gem_string("setup")));
#line 1137 "compiler/main.gem"
    GemVal _t3231[] = {gem_v_p};
    GemVal _t3232 = gem_v_val_r;
    GemVal _t3233[] = {gem_table_get(_t3232, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3231, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3233, 1)), gem_string(")) {\n")));
        }
#line 1139 "compiler/main.gem"
    GemVal _t3234 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t3234, gem_string("bindings")), GEM_NIL))) {
#line 1140 "compiler/main.gem"
    GemVal _t3235 = gem_v_w;
            GemVal gem_v__for_items_57 = gem_table_get(_t3235, gem_string("bindings"));
#line 1140 "compiler/main.gem"
            GemVal gem_v__for_i_57 = gem_int(0);
#line 1140 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3236[] = {gem_v__for_items_57};
                if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t3236, 1)))) break;
#line 1140 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
#line 1140 "compiler/main.gem"
                gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
#line 1141 "compiler/main.gem"
    GemVal _t3237[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3238 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3238.fn(_t3238.env, _t3237, 2)), gem_string("\n"));
            }

        }
#line 1144 "compiler/main.gem"
    GemVal _t3239 = gem_v_w;
    GemVal _t3240[] = {gem_table_get(_t3239, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3241 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3241.fn(_t3241.env, _t3240, 2));
    }

#line 1146 "compiler/main.gem"
    GemVal _t3242 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3242, gem_string("else")), GEM_NIL))) {
#line 1147 "compiler/main.gem"
    GemVal _t3243[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3243, 1), gem_string("} else {\n")));
#line 1148 "compiler/main.gem"
    GemVal _t3244 = gem_v_node;
    GemVal _t3245[] = {gem_table_get(_t3244, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3246 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3246.fn(_t3246.env, _t3245, 2));
    }
#line 1151 "compiler/main.gem"
    GemVal _t3247[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3247, 1), gem_string("}")));
#line 1152 "compiler/main.gem"
    GemVal gem_v__for_i_59 = gem_int(1);
#line 1152 "compiler/main.gem"
    GemVal _t3248 = gem_v_node;
    GemVal _t3249[] = {gem_table_get(_t3248, gem_string("whens"))};
    GemVal gem_v__for_limit_59 = gem_len_fn(NULL, _t3249, 1);
#line 1152 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_v__for_limit_59))) break;
#line 1152 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_59;
#line 1152 "compiler/main.gem"
        gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1153 "compiler/main.gem"
    GemVal _t3250[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("\n"), gem_to_string_fn(NULL, _t3250, 1)), gem_string("}")));
    }

    GemVal _t3251 = gem_v_out;
    gem_pop_frame();
    return _t3251;
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
#line 1161 "compiler/main.gem"
    GemVal _t3253[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3253, 1);
#line 1162 "compiler/main.gem"
    GemVal _t3254 = gem_v_node;
    GemVal _t3255[] = {gem_table_get(_t3254, gem_string("target"))};
    GemVal _t3256 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3256.fn(_t3256.env, _t3255, 1);
#line 1163 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1164 "compiler/main.gem"
    GemVal _t3257 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3257, gem_string("target_var")), GEM_NIL))) {
#line 1165 "compiler/main.gem"
    GemVal _t3258 = gem_v_node;
    GemVal _t3259[] = {gem_table_get(_t3258, gem_string("target_var"))};
    GemVal _t3260 = (*gem_v_mangle);
        gem_v_t = _t3260.fn(_t3260.env, _t3259, 1);
    } else {
#line 1167 "compiler/main.gem"
    GemVal _t3261 = (*gem_v_tmp);
        gem_v_t = _t3261.fn(_t3261.env, NULL, 0);
    }
#line 1169 "compiler/main.gem"
    GemVal _t3262 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t3262, gem_string("setup"));
#line 1170 "compiler/main.gem"
    GemVal _t3263[] = {gem_v_p};
    GemVal _t3264[] = {gem_v_t};
    GemVal _t3265 = gem_v_target_r;
    GemVal _t3266[] = {gem_table_get(_t3265, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3263, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3264, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3266, 1)), gem_string(";\n")));
#line 1171 "compiler/main.gem"
    GemVal _t3267 = gem_v_node;
    GemVal _t3268[] = {gem_table_get(_t3267, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3268, 1), gem_int(0)))) {
#line 1173 "compiler/main.gem"
    GemVal _t3269 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3269, gem_string("else")), GEM_NIL))) {
#line 1174 "compiler/main.gem"
    GemVal _t3270 = gem_v_node;
            GemVal gem_v_else_body = gem_table_get(_t3270, gem_string("else"));
#line 1175 "compiler/main.gem"
    GemVal _t3271[] = {gem_v_else_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3271, 1), gem_int(0)))) {
#line 1176 "compiler/main.gem"
                GemVal gem_v__for_i_60 = gem_int(0);
#line 1176 "compiler/main.gem"
    GemVal _t3272[] = {gem_v_else_body};
                GemVal gem_v__for_limit_60 = gem_sub(gem_len_fn(NULL, _t3272, 1), gem_int(1));
#line 1176 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1176 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_60;
#line 1176 "compiler/main.gem"
                    gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1177 "compiler/main.gem"
    GemVal _t3273[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_v_indent};
    GemVal _t3274 = (*gem_v_compile_stmt);
                    gem_v_out = gem_add(gem_add(gem_v_out, _t3274.fn(_t3274.env, _t3273, 2)), gem_string("\n"));
                }

#line 1179 "compiler/main.gem"
    GemVal _t3275[] = {gem_v_else_body};
    GemVal _t3276[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t3275, 1), gem_int(1))), gem_v_indent};
    GemVal _t3277 = (*gem_v_compile_stmt_return);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3277.fn(_t3277.env, _t3276, 2)), gem_string("\n"));
            } else {
#line 1181 "compiler/main.gem"
    GemVal _t3278[] = {gem_v_p};
    GemVal _t3279[] = {gem_v_p};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3278, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3279, 1)), gem_string("return GEM_NIL;\n")));
            }
        } else {
#line 1184 "compiler/main.gem"
    GemVal _t3280[] = {gem_v_p};
    GemVal _t3281[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3280, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3281, 1)), gem_string("return GEM_NIL;\n")));
        }
#line 1186 "compiler/main.gem"
    GemVal _t3282[] = {gem_v_p};
    GemVal _t3283[] = {gem_v_t};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3282, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t3283, 1)), gem_string(";")));
#line 1187 "compiler/main.gem"
        GemVal _t3284 = gem_v_out;
        gem_pop_frame();
        return _t3284;
    }
#line 1189 "compiler/main.gem"
    GemVal gem_v__for_i_63 = gem_int(0);
#line 1189 "compiler/main.gem"
    GemVal _t3285 = gem_v_node;
    GemVal _t3286[] = {gem_table_get(_t3285, gem_string("whens"))};
    GemVal gem_v__for_limit_63 = gem_len_fn(NULL, _t3286, 1);
#line 1189 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_v__for_limit_63))) break;
#line 1189 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_63;
#line 1189 "compiler/main.gem"
        gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1190 "compiler/main.gem"
    GemVal _t3287 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t3287, gem_string("whens")), gem_v_i);
#line 1191 "compiler/main.gem"
    GemVal _t3288 = gem_v_w;
    GemVal _t3289[] = {gem_table_get(_t3288, gem_string("value"))};
    GemVal _t3290 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3290.fn(_t3290.env, _t3289, 1);
#line 1192 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1193 "compiler/main.gem"
    GemVal _t3291 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3291, gem_string("setup")));
#line 1194 "compiler/main.gem"
    GemVal _t3292[] = {gem_v_p};
    GemVal _t3293 = gem_v_val_r;
    GemVal _t3294[] = {gem_table_get(_t3293, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3292, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3294, 1)), gem_string(")) {\n")));
        } else {
#line 1196 "compiler/main.gem"
    GemVal _t3295[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3295, 1), gem_string("} else {\n")));
#line 1197 "compiler/main.gem"
    GemVal _t3296 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3296, gem_string("setup")));
#line 1198 "compiler/main.gem"
    GemVal _t3297[] = {gem_v_p};
    GemVal _t3298 = gem_v_val_r;
    GemVal _t3299[] = {gem_table_get(_t3298, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3297, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3299, 1)), gem_string(")) {\n")));
        }
#line 1200 "compiler/main.gem"
    GemVal _t3300 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t3300, gem_string("bindings")), GEM_NIL))) {
#line 1201 "compiler/main.gem"
    GemVal _t3301 = gem_v_w;
            GemVal gem_v__for_items_61 = gem_table_get(_t3301, gem_string("bindings"));
#line 1201 "compiler/main.gem"
            GemVal gem_v__for_i_61 = gem_int(0);
#line 1201 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3302[] = {gem_v__for_items_61};
                if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_len_fn(NULL, _t3302, 1)))) break;
#line 1201 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_61, gem_v__for_i_61);
#line 1201 "compiler/main.gem"
                gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1202 "compiler/main.gem"
    GemVal _t3303[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3304 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3304.fn(_t3304.env, _t3303, 2)), gem_string("\n"));
            }

        }
#line 1205 "compiler/main.gem"
    GemVal _t3305 = gem_v_w;
        GemVal gem_v_body = gem_table_get(_t3305, gem_string("body"));
#line 1206 "compiler/main.gem"
    GemVal _t3306[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3306, 1), gem_int(0)))) {
#line 1207 "compiler/main.gem"
            GemVal gem_v__for_i_62 = gem_int(0);
#line 1207 "compiler/main.gem"
    GemVal _t3307[] = {gem_v_body};
            GemVal gem_v__for_limit_62 = gem_sub(gem_len_fn(NULL, _t3307, 1), gem_int(1));
#line 1207 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
#line 1207 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_62;
#line 1207 "compiler/main.gem"
                gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1208 "compiler/main.gem"
    GemVal _t3308[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3309 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3309.fn(_t3309.env, _t3308, 2)), gem_string("\n"));
            }

#line 1210 "compiler/main.gem"
    GemVal _t3310[] = {gem_v_body};
    GemVal _t3311[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t3310, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3312 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3312.fn(_t3312.env, _t3311, 2)), gem_string("\n"));
        } else {
#line 1212 "compiler/main.gem"
    GemVal _t3313[] = {gem_v_p};
    GemVal _t3314[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3313, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3314, 1)), gem_string("    return GEM_NIL;\n")));
        }
    }

#line 1215 "compiler/main.gem"
    GemVal _t3315 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3315, gem_string("else")), GEM_NIL))) {
#line 1216 "compiler/main.gem"
    GemVal _t3316[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3316, 1), gem_string("} else {\n")));
#line 1217 "compiler/main.gem"
    GemVal _t3317 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t3317, gem_string("else"));
#line 1218 "compiler/main.gem"
    GemVal _t3318[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3318, 1), gem_int(0)))) {
#line 1219 "compiler/main.gem"
            GemVal gem_v__for_i_64 = gem_int(0);
#line 1219 "compiler/main.gem"
    GemVal _t3319[] = {gem_v_else_body};
            GemVal gem_v__for_limit_64 = gem_sub(gem_len_fn(NULL, _t3319, 1), gem_int(1));
#line 1219 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_v__for_limit_64))) break;
#line 1219 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_64;
#line 1219 "compiler/main.gem"
                gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1220 "compiler/main.gem"
    GemVal _t3320[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3321 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3321.fn(_t3321.env, _t3320, 2)), gem_string("\n"));
            }

#line 1222 "compiler/main.gem"
    GemVal _t3322[] = {gem_v_else_body};
    GemVal _t3323[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t3322, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3324 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3324.fn(_t3324.env, _t3323, 2)), gem_string("\n"));
        } else {
#line 1224 "compiler/main.gem"
    GemVal _t3325[] = {gem_v_p};
    GemVal _t3326[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3325, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3326, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1227 "compiler/main.gem"
    GemVal _t3327[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3327, 1), gem_string("} else {\n")));
#line 1228 "compiler/main.gem"
    GemVal _t3328[] = {gem_v_p};
    GemVal _t3329[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3328, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3329, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1230 "compiler/main.gem"
    GemVal _t3330[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3330, 1), gem_string("}")));
#line 1231 "compiler/main.gem"
    GemVal gem_v__for_i_65 = gem_int(1);
#line 1231 "compiler/main.gem"
    GemVal _t3331 = gem_v_node;
    GemVal _t3332[] = {gem_table_get(_t3331, gem_string("whens"))};
    GemVal gem_v__for_limit_65 = gem_len_fn(NULL, _t3332, 1);
#line 1231 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_v__for_limit_65))) break;
#line 1231 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_65;
#line 1231 "compiler/main.gem"
        gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1232 "compiler/main.gem"
    GemVal _t3333[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("\n"), gem_to_string_fn(NULL, _t3333, 1)), gem_string("}")));
    }

    GemVal _t3334 = gem_v_out;
    gem_pop_frame();
    return _t3334;
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
#line 1240 "compiler/main.gem"
    GemVal _t3336[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3336, 1);
#line 1241 "compiler/main.gem"
    GemVal _t3337 = gem_v_node;
    GemVal gem_v_arms = gem_table_get(_t3337, gem_string("arms"));
#line 1242 "compiler/main.gem"
    GemVal _t3338 = gem_v_node;
    GemVal gem_v_after_ms = gem_table_get(_t3338, gem_string("after_ms"));
#line 1243 "compiler/main.gem"
    GemVal _t3339 = gem_v_node;
    GemVal gem_v_after_body = gem_table_get(_t3339, gem_string("after_body"));
#line 1246 "compiler/main.gem"
    GemVal _t3340 = gem_v_node;
    GemVal _t3341[] = {gem_table_get(_t3340, gem_string("recv_var"))};
    GemVal _t3342 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3342.fn(_t3342.env, _t3341, 1);
#line 1247 "compiler/main.gem"
    GemVal _t3343 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3343.fn(_t3343.env, NULL, 0);
#line 1248 "compiler/main.gem"
    GemVal _t3344 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3344.fn(_t3344.env, NULL, 0);
#line 1249 "compiler/main.gem"
    GemVal _t3345 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3345.fn(_t3345.env, NULL, 0);
#line 1250 "compiler/main.gem"
    GemVal _t3346 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3346.fn(_t3346.env, NULL, 0);
#line 1251 "compiler/main.gem"
    GemVal _t3347 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3347.fn(_t3347.env, NULL, 0);
#line 1253 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1256 "compiler/main.gem"
    GemVal _t3348[] = {gem_v_p};
    GemVal _t3349[] = {gem_v_recv_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3348, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3349, 1)), gem_string(" = GEM_NIL;\n")));
#line 1257 "compiler/main.gem"
    GemVal _t3350[] = {gem_v_p};
    GemVal _t3351[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3350, 1), gem_string("int ")), gem_to_string_fn(NULL, _t3351, 1)), gem_string(" = -1;\n")));
#line 1260 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1261 "compiler/main.gem"
    GemVal _t3352[] = {gem_v_after_ms};
    GemVal _t3353 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3353.fn(_t3353.env, _t3352, 1);
#line 1262 "compiler/main.gem"
    GemVal _t3354 = gem_v_ms_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t3354, gem_string("setup")));
#line 1263 "compiler/main.gem"
    GemVal _t3355[] = {gem_v_p};
    GemVal _t3356[] = {gem_v_deadline_tmp};
    GemVal _t3357 = gem_v_ms_r;
    GemVal _t3358[] = {gem_table_get(_t3357, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3355, 1), gem_string("int64_t ")), gem_to_string_fn(NULL, _t3356, 1)), gem_string(" = gem_now_ms() + (int64_t)(")), gem_to_string_fn(NULL, _t3358, 1)), gem_string(").ival;\n")));
    }
#line 1267 "compiler/main.gem"
    GemVal _t3359[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3359, 1), gem_string("while (1) {\n")));
#line 1268 "compiler/main.gem"
    GemVal _t3360[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn_pad_for(NULL, _t3360, 1);
#line 1271 "compiler/main.gem"
    GemVal _t3361[] = {gem_v_p2};
    GemVal _t3362[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3361, 1), gem_string("GemProcess *")), gem_to_string_fn(NULL, _t3362, 1)), gem_string(" = &gem_proc_table[gem_current_pid];\n")));
#line 1274 "compiler/main.gem"
    GemVal _t3363[] = {gem_v_p2};
    GemVal _t3364[] = {gem_v_prev_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3363, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3364, 1)), gem_string(" = NULL;\n")));
#line 1275 "compiler/main.gem"
    GemVal _t3365[] = {gem_v_p2};
    GemVal _t3366[] = {gem_v_cur_tmp};
    GemVal _t3367[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3365, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3366, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3367, 1)), gem_string("->mailbox.head;\n")));
#line 1276 "compiler/main.gem"
    GemVal _t3368[] = {gem_v_p2};
    GemVal _t3369[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_to_string_fn(NULL, _t3368, 1), gem_to_string_fn(NULL, _t3369, 1)), gem_string(" = -1;\n")));
#line 1277 "compiler/main.gem"
    GemVal _t3370[] = {gem_v_p2};
    GemVal _t3371[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3370, 1), gem_string("while (")), gem_to_string_fn(NULL, _t3371, 1)), gem_string(") {\n")));
#line 1278 "compiler/main.gem"
    GemVal _t3372[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn_pad_for(NULL, _t3372, 1);
#line 1279 "compiler/main.gem"
    GemVal _t3373[] = {gem_v_p3};
    GemVal _t3374[] = {gem_v_recv_tmp};
    GemVal _t3375[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3373, 1), gem_to_string_fn(NULL, _t3374, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3375, 1)), gem_string("->value;\n")));
#line 1282 "compiler/main.gem"
    GemVal gem_v__for_i_66 = gem_int(0);
#line 1282 "compiler/main.gem"
    GemVal _t3376[] = {gem_v_arms};
    GemVal gem_v__for_limit_66 = gem_len_fn(NULL, _t3376, 1);
#line 1282 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_v__for_limit_66))) break;
#line 1282 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_66;
#line 1282 "compiler/main.gem"
        gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1283 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1284 "compiler/main.gem"
    GemVal _t3377 = gem_v_arm;
    GemVal _t3378 = gem_table_get(_t3377, gem_string("pattern"));
    GemVal _t3379[] = {gem_table_get(_t3378, gem_string("condition"))};
    GemVal _t3380 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3380.fn(_t3380.env, _t3379, 1);
#line 1285 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1286 "compiler/main.gem"
    GemVal _t3381 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3381, gem_string("setup")));
#line 1287 "compiler/main.gem"
    GemVal _t3382[] = {gem_v_p3};
    GemVal _t3383 = gem_v_cond_r;
    GemVal _t3384[] = {gem_table_get(_t3383, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3382, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3384, 1)), gem_string(")) {\n")));
        } else {
#line 1289 "compiler/main.gem"
    GemVal _t3385[] = {gem_v_p3};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3385, 1), gem_string("} else {\n")));
#line 1290 "compiler/main.gem"
    GemVal _t3386 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3386, gem_string("setup")));
#line 1291 "compiler/main.gem"
    GemVal _t3387[] = {gem_v_p3};
    GemVal _t3388 = gem_v_cond_r;
    GemVal _t3389[] = {gem_table_get(_t3388, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3387, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3389, 1)), gem_string(")) {\n")));
        }
#line 1293 "compiler/main.gem"
    GemVal _t3390[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3391[] = {gem_fn_pad_for(NULL, _t3390, 1)};
    GemVal _t3392[] = {gem_v_matched_tmp};
    GemVal _t3393[] = {gem_v_i};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3391, 1), gem_to_string_fn(NULL, _t3392, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3393, 1)), gem_string(";\n")));
    }

#line 1296 "compiler/main.gem"
    GemVal gem_v__for_i_67 = gem_int(0);
#line 1296 "compiler/main.gem"
    GemVal _t3394[] = {gem_v_arms};
    GemVal gem_v__for_limit_67 = gem_len_fn(NULL, _t3394, 1);
#line 1296 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_v__for_limit_67))) break;
#line 1296 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_67;
#line 1296 "compiler/main.gem"
        gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1297 "compiler/main.gem"
    GemVal _t3395[] = {gem_v_p3};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3395, 1), gem_string("}")));
#line 1298 "compiler/main.gem"
    GemVal _t3396[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3396, 1), gem_int(1))))) {
#line 1299 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1302 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1305 "compiler/main.gem"
    GemVal _t3397[] = {gem_v_p3};
    GemVal _t3398[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3397, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3398, 1)), gem_string(" >= 0) {\n")));
#line 1306 "compiler/main.gem"
    GemVal _t3399[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn_pad_for(NULL, _t3399, 1);
#line 1307 "compiler/main.gem"
    GemVal _t3400[] = {gem_v_p4};
    GemVal _t3401[] = {gem_v_proc_tmp};
    GemVal _t3402[] = {gem_v_prev_tmp};
    GemVal _t3403[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3400, 1), gem_string("gem_mailbox_remove(&")), gem_to_string_fn(NULL, _t3401, 1)), gem_string("->mailbox, ")), gem_to_string_fn(NULL, _t3402, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3403, 1)), gem_string(");\n")));
#line 1308 "compiler/main.gem"
    GemVal _t3404[] = {gem_v_p4};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3404, 1), gem_string("break;\n")));
#line 1309 "compiler/main.gem"
    GemVal _t3405[] = {gem_v_p3};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3405, 1), gem_string("}\n")));
#line 1312 "compiler/main.gem"
    GemVal _t3406[] = {gem_v_p3};
    GemVal _t3407[] = {gem_v_prev_tmp};
    GemVal _t3408[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3406, 1), gem_to_string_fn(NULL, _t3407, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3408, 1)), gem_string(";\n")));
#line 1313 "compiler/main.gem"
    GemVal _t3409[] = {gem_v_p3};
    GemVal _t3410[] = {gem_v_cur_tmp};
    GemVal _t3411[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3409, 1), gem_to_string_fn(NULL, _t3410, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3411, 1)), gem_string("->next;\n")));
#line 1314 "compiler/main.gem"
    GemVal _t3412[] = {gem_v_p2};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3412, 1), gem_string("}\n")));
#line 1317 "compiler/main.gem"
    GemVal _t3413[] = {gem_v_p2};
    GemVal _t3414[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3413, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3414, 1)), gem_string(" >= 0) break;\n")));
#line 1318 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1319 "compiler/main.gem"
    GemVal _t3415[] = {gem_v_p2};
    GemVal _t3416[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3415, 1), gem_string("if (gem_now_ms() >= ")), gem_to_string_fn(NULL, _t3416, 1)), gem_string(") {\n")));
#line 1320 "compiler/main.gem"
    GemVal _t3417[] = {gem_v_p2};
    GemVal _t3418[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3417, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3418, 1)), gem_string(" = -2;\n")));
#line 1321 "compiler/main.gem"
    GemVal _t3419[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3419, 1), gem_string("    break;\n")));
#line 1322 "compiler/main.gem"
    GemVal _t3420[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3420, 1), gem_string("}\n")));
#line 1323 "compiler/main.gem"
    GemVal _t3421[] = {gem_v_p2};
    GemVal _t3422[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3421, 1), gem_string("gem_selective_yield(")), gem_to_string_fn(NULL, _t3422, 1)), gem_string(");\n")));
#line 1325 "compiler/main.gem"
    GemVal _t3423[] = {gem_v_p2};
    GemVal _t3424[] = {gem_v_proc_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3423, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3424, 1)), gem_string("->timed_out) {\n")));
#line 1326 "compiler/main.gem"
    GemVal _t3425[] = {gem_v_p2};
    GemVal _t3426[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3425, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3426, 1)), gem_string(" = -2;\n")));
#line 1327 "compiler/main.gem"
    GemVal _t3427[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3427, 1), gem_string("    break;\n")));
#line 1328 "compiler/main.gem"
    GemVal _t3428[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3428, 1), gem_string("}\n")));
    } else {
#line 1330 "compiler/main.gem"
    GemVal _t3429[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3429, 1), gem_string("gem_selective_yield(-1);\n")));
    }
#line 1333 "compiler/main.gem"
    GemVal _t3430[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3430, 1), gem_string("}\n")));
#line 1336 "compiler/main.gem"
    GemVal gem_v__for_i_69 = gem_int(0);
#line 1336 "compiler/main.gem"
    GemVal _t3431[] = {gem_v_arms};
    GemVal gem_v__for_limit_69 = gem_len_fn(NULL, _t3431, 1);
#line 1336 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_69, gem_v__for_limit_69))) break;
#line 1336 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_69;
#line 1336 "compiler/main.gem"
        gem_v__for_i_69 = gem_add(gem_v__for_i_69, gem_int(1));
#line 1337 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1338 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1339 "compiler/main.gem"
    GemVal _t3432[] = {gem_v_p};
    GemVal _t3433[] = {gem_v_matched_tmp};
    GemVal _t3434[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3432, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3433, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3434, 1)), gem_string(") {\n")));
        } else {
#line 1341 "compiler/main.gem"
    GemVal _t3435[] = {gem_v_p};
    GemVal _t3436[] = {gem_v_matched_tmp};
    GemVal _t3437[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3435, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3436, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3437, 1)), gem_string(") {\n")));
        }
#line 1344 "compiler/main.gem"
    GemVal _t3438 = gem_v_arm;
    GemVal _t3439 = gem_table_get(_t3438, gem_string("pattern"));
        if (gem_truthy(gem_neq(gem_table_get(_t3439, gem_string("bindings")), GEM_NIL))) {
#line 1345 "compiler/main.gem"
    GemVal _t3440 = gem_v_arm;
    GemVal _t3441 = gem_table_get(_t3440, gem_string("pattern"));
            GemVal gem_v__for_items_68 = gem_table_get(_t3441, gem_string("bindings"));
#line 1345 "compiler/main.gem"
            GemVal gem_v__for_i_68 = gem_int(0);
#line 1345 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3442[] = {gem_v__for_items_68};
                if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_len_fn(NULL, _t3442, 1)))) break;
#line 1345 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_68, gem_v__for_i_68);
#line 1345 "compiler/main.gem"
                gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1346 "compiler/main.gem"
    GemVal _t3443[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3444 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3444.fn(_t3444.env, _t3443, 2)), gem_string("\n"));
            }

        }
#line 1349 "compiler/main.gem"
    GemVal _t3445 = gem_v_arm;
    GemVal _t3446[] = {gem_table_get(_t3445, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3447 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3447.fn(_t3447.env, _t3446, 2));
    }

#line 1353 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1354 "compiler/main.gem"
    GemVal _t3448[] = {gem_v_p};
    GemVal _t3449[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3448, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3449, 1)), gem_string(" == -2) {\n")));
#line 1355 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 1356 "compiler/main.gem"
    GemVal _t3450[] = {gem_v_after_body, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3451 = (*gem_v_compile_stmts);
            gem_v_out = gem_add(gem_v_out, _t3451.fn(_t3451.env, _t3450, 2));
        }
    }
#line 1360 "compiler/main.gem"
    GemVal _t3452[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3452, 1), gem_string("}")));
    GemVal _t3453 = gem_v_out;
    gem_pop_frame();
    return _t3453;
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
#line 1365 "compiler/main.gem"
    GemVal _t3455[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3455, 1);
#line 1366 "compiler/main.gem"
    GemVal _t3456 = gem_v_node;
    GemVal gem_v_arms = gem_table_get(_t3456, gem_string("arms"));
#line 1367 "compiler/main.gem"
    GemVal _t3457 = gem_v_node;
    GemVal gem_v_after_ms = gem_table_get(_t3457, gem_string("after_ms"));
#line 1368 "compiler/main.gem"
    GemVal _t3458 = gem_v_node;
    GemVal gem_v_after_body = gem_table_get(_t3458, gem_string("after_body"));
#line 1370 "compiler/main.gem"
    GemVal _t3459 = gem_v_node;
    GemVal _t3460[] = {gem_table_get(_t3459, gem_string("recv_var"))};
    GemVal _t3461 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3461.fn(_t3461.env, _t3460, 1);
#line 1371 "compiler/main.gem"
    GemVal _t3462 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3462.fn(_t3462.env, NULL, 0);
#line 1372 "compiler/main.gem"
    GemVal _t3463 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3463.fn(_t3463.env, NULL, 0);
#line 1373 "compiler/main.gem"
    GemVal _t3464 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3464.fn(_t3464.env, NULL, 0);
#line 1374 "compiler/main.gem"
    GemVal _t3465 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3465.fn(_t3465.env, NULL, 0);
#line 1375 "compiler/main.gem"
    GemVal _t3466 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3466.fn(_t3466.env, NULL, 0);
#line 1377 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1379 "compiler/main.gem"
    GemVal _t3467[] = {gem_v_p};
    GemVal _t3468[] = {gem_v_recv_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3467, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3468, 1)), gem_string(" = GEM_NIL;\n")));
#line 1380 "compiler/main.gem"
    GemVal _t3469[] = {gem_v_p};
    GemVal _t3470[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3469, 1), gem_string("int ")), gem_to_string_fn(NULL, _t3470, 1)), gem_string(" = -1;\n")));
#line 1382 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1383 "compiler/main.gem"
    GemVal _t3471[] = {gem_v_after_ms};
    GemVal _t3472 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3472.fn(_t3472.env, _t3471, 1);
#line 1384 "compiler/main.gem"
    GemVal _t3473 = gem_v_ms_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t3473, gem_string("setup")));
#line 1385 "compiler/main.gem"
    GemVal _t3474[] = {gem_v_p};
    GemVal _t3475[] = {gem_v_deadline_tmp};
    GemVal _t3476 = gem_v_ms_r;
    GemVal _t3477[] = {gem_table_get(_t3476, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3474, 1), gem_string("int64_t ")), gem_to_string_fn(NULL, _t3475, 1)), gem_string(" = gem_now_ms() + (int64_t)(")), gem_to_string_fn(NULL, _t3477, 1)), gem_string(").ival;\n")));
    }
#line 1388 "compiler/main.gem"
    GemVal _t3478[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3478, 1), gem_string("while (1) {\n")));
#line 1389 "compiler/main.gem"
    GemVal _t3479[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn_pad_for(NULL, _t3479, 1);
#line 1391 "compiler/main.gem"
    GemVal _t3480[] = {gem_v_p2};
    GemVal _t3481[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3480, 1), gem_string("GemProcess *")), gem_to_string_fn(NULL, _t3481, 1)), gem_string(" = &gem_proc_table[gem_current_pid];\n")));
#line 1392 "compiler/main.gem"
    GemVal _t3482[] = {gem_v_p2};
    GemVal _t3483[] = {gem_v_prev_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3482, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3483, 1)), gem_string(" = NULL;\n")));
#line 1393 "compiler/main.gem"
    GemVal _t3484[] = {gem_v_p2};
    GemVal _t3485[] = {gem_v_cur_tmp};
    GemVal _t3486[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3484, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3485, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3486, 1)), gem_string("->mailbox.head;\n")));
#line 1394 "compiler/main.gem"
    GemVal _t3487[] = {gem_v_p2};
    GemVal _t3488[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_to_string_fn(NULL, _t3487, 1), gem_to_string_fn(NULL, _t3488, 1)), gem_string(" = -1;\n")));
#line 1395 "compiler/main.gem"
    GemVal _t3489[] = {gem_v_p2};
    GemVal _t3490[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3489, 1), gem_string("while (")), gem_to_string_fn(NULL, _t3490, 1)), gem_string(") {\n")));
#line 1396 "compiler/main.gem"
    GemVal _t3491[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn_pad_for(NULL, _t3491, 1);
#line 1397 "compiler/main.gem"
    GemVal _t3492[] = {gem_v_p3};
    GemVal _t3493[] = {gem_v_recv_tmp};
    GemVal _t3494[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3492, 1), gem_to_string_fn(NULL, _t3493, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3494, 1)), gem_string("->value;\n")));
#line 1399 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1399 "compiler/main.gem"
    GemVal _t3495[] = {gem_v_arms};
    GemVal gem_v__for_limit_70 = gem_len_fn(NULL, _t3495, 1);
#line 1399 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_70, gem_v__for_limit_70))) break;
#line 1399 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_70;
#line 1399 "compiler/main.gem"
        gem_v__for_i_70 = gem_add(gem_v__for_i_70, gem_int(1));
#line 1400 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1401 "compiler/main.gem"
    GemVal _t3496 = gem_v_arm;
    GemVal _t3497 = gem_table_get(_t3496, gem_string("pattern"));
    GemVal _t3498[] = {gem_table_get(_t3497, gem_string("condition"))};
    GemVal _t3499 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3499.fn(_t3499.env, _t3498, 1);
#line 1402 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1403 "compiler/main.gem"
    GemVal _t3500 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3500, gem_string("setup")));
#line 1404 "compiler/main.gem"
    GemVal _t3501[] = {gem_v_p3};
    GemVal _t3502 = gem_v_cond_r;
    GemVal _t3503[] = {gem_table_get(_t3502, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3501, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3503, 1)), gem_string(")) {\n")));
        } else {
#line 1406 "compiler/main.gem"
    GemVal _t3504[] = {gem_v_p3};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3504, 1), gem_string("} else {\n")));
#line 1407 "compiler/main.gem"
    GemVal _t3505 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3505, gem_string("setup")));
#line 1408 "compiler/main.gem"
    GemVal _t3506[] = {gem_v_p3};
    GemVal _t3507 = gem_v_cond_r;
    GemVal _t3508[] = {gem_table_get(_t3507, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3506, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3508, 1)), gem_string(")) {\n")));
        }
#line 1410 "compiler/main.gem"
    GemVal _t3509[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3510[] = {gem_fn_pad_for(NULL, _t3509, 1)};
    GemVal _t3511[] = {gem_v_matched_tmp};
    GemVal _t3512[] = {gem_v_i};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3510, 1), gem_to_string_fn(NULL, _t3511, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3512, 1)), gem_string(";\n")));
    }

#line 1412 "compiler/main.gem"
    GemVal gem_v__for_i_71 = gem_int(0);
#line 1412 "compiler/main.gem"
    GemVal _t3513[] = {gem_v_arms};
    GemVal gem_v__for_limit_71 = gem_len_fn(NULL, _t3513, 1);
#line 1412 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_71, gem_v__for_limit_71))) break;
#line 1412 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_71;
#line 1412 "compiler/main.gem"
        gem_v__for_i_71 = gem_add(gem_v__for_i_71, gem_int(1));
#line 1413 "compiler/main.gem"
    GemVal _t3514[] = {gem_v_p3};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3514, 1), gem_string("}")));
#line 1414 "compiler/main.gem"
    GemVal _t3515[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3515, 1), gem_int(1))))) {
#line 1415 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1418 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1420 "compiler/main.gem"
    GemVal _t3516[] = {gem_v_p3};
    GemVal _t3517[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3516, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3517, 1)), gem_string(" >= 0) {\n")));
#line 1421 "compiler/main.gem"
    GemVal _t3518[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn_pad_for(NULL, _t3518, 1);
#line 1422 "compiler/main.gem"
    GemVal _t3519[] = {gem_v_p4};
    GemVal _t3520[] = {gem_v_proc_tmp};
    GemVal _t3521[] = {gem_v_prev_tmp};
    GemVal _t3522[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3519, 1), gem_string("gem_mailbox_remove(&")), gem_to_string_fn(NULL, _t3520, 1)), gem_string("->mailbox, ")), gem_to_string_fn(NULL, _t3521, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3522, 1)), gem_string(");\n")));
#line 1423 "compiler/main.gem"
    GemVal _t3523[] = {gem_v_p4};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3523, 1), gem_string("break;\n")));
#line 1424 "compiler/main.gem"
    GemVal _t3524[] = {gem_v_p3};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3524, 1), gem_string("}\n")));
#line 1426 "compiler/main.gem"
    GemVal _t3525[] = {gem_v_p3};
    GemVal _t3526[] = {gem_v_prev_tmp};
    GemVal _t3527[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3525, 1), gem_to_string_fn(NULL, _t3526, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3527, 1)), gem_string(";\n")));
#line 1427 "compiler/main.gem"
    GemVal _t3528[] = {gem_v_p3};
    GemVal _t3529[] = {gem_v_cur_tmp};
    GemVal _t3530[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3528, 1), gem_to_string_fn(NULL, _t3529, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3530, 1)), gem_string("->next;\n")));
#line 1428 "compiler/main.gem"
    GemVal _t3531[] = {gem_v_p2};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3531, 1), gem_string("}\n")));
#line 1430 "compiler/main.gem"
    GemVal _t3532[] = {gem_v_p2};
    GemVal _t3533[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3532, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3533, 1)), gem_string(" >= 0) break;\n")));
#line 1431 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1432 "compiler/main.gem"
    GemVal _t3534[] = {gem_v_p2};
    GemVal _t3535[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3534, 1), gem_string("if (gem_now_ms() >= ")), gem_to_string_fn(NULL, _t3535, 1)), gem_string(") {\n")));
#line 1433 "compiler/main.gem"
    GemVal _t3536[] = {gem_v_p2};
    GemVal _t3537[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3536, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3537, 1)), gem_string(" = -2;\n")));
#line 1434 "compiler/main.gem"
    GemVal _t3538[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3538, 1), gem_string("    break;\n")));
#line 1435 "compiler/main.gem"
    GemVal _t3539[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3539, 1), gem_string("}\n")));
#line 1436 "compiler/main.gem"
    GemVal _t3540[] = {gem_v_p2};
    GemVal _t3541[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3540, 1), gem_string("gem_selective_yield(")), gem_to_string_fn(NULL, _t3541, 1)), gem_string(");\n")));
#line 1437 "compiler/main.gem"
    GemVal _t3542[] = {gem_v_p2};
    GemVal _t3543[] = {gem_v_proc_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3542, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3543, 1)), gem_string("->timed_out) {\n")));
#line 1438 "compiler/main.gem"
    GemVal _t3544[] = {gem_v_p2};
    GemVal _t3545[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3544, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3545, 1)), gem_string(" = -2;\n")));
#line 1439 "compiler/main.gem"
    GemVal _t3546[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3546, 1), gem_string("    break;\n")));
#line 1440 "compiler/main.gem"
    GemVal _t3547[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3547, 1), gem_string("}\n")));
    } else {
#line 1442 "compiler/main.gem"
    GemVal _t3548[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3548, 1), gem_string("gem_selective_yield(-1);\n")));
    }
#line 1445 "compiler/main.gem"
    GemVal _t3549[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3549, 1), gem_string("}\n")));
#line 1448 "compiler/main.gem"
    GemVal gem_v__for_i_74 = gem_int(0);
#line 1448 "compiler/main.gem"
    GemVal _t3550[] = {gem_v_arms};
    GemVal gem_v__for_limit_74 = gem_len_fn(NULL, _t3550, 1);
#line 1448 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_74, gem_v__for_limit_74))) break;
#line 1448 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_74;
#line 1448 "compiler/main.gem"
        gem_v__for_i_74 = gem_add(gem_v__for_i_74, gem_int(1));
#line 1449 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1450 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1451 "compiler/main.gem"
    GemVal _t3551[] = {gem_v_p};
    GemVal _t3552[] = {gem_v_matched_tmp};
    GemVal _t3553[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3551, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3552, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3553, 1)), gem_string(") {\n")));
        } else {
#line 1453 "compiler/main.gem"
    GemVal _t3554[] = {gem_v_p};
    GemVal _t3555[] = {gem_v_matched_tmp};
    GemVal _t3556[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3554, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3555, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3556, 1)), gem_string(") {\n")));
        }
#line 1455 "compiler/main.gem"
    GemVal _t3557 = gem_v_arm;
    GemVal _t3558 = gem_table_get(_t3557, gem_string("pattern"));
        if (gem_truthy(gem_neq(gem_table_get(_t3558, gem_string("bindings")), GEM_NIL))) {
#line 1456 "compiler/main.gem"
    GemVal _t3559 = gem_v_arm;
    GemVal _t3560 = gem_table_get(_t3559, gem_string("pattern"));
            GemVal gem_v__for_items_72 = gem_table_get(_t3560, gem_string("bindings"));
#line 1456 "compiler/main.gem"
            GemVal gem_v__for_i_72 = gem_int(0);
#line 1456 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3561[] = {gem_v__for_items_72};
                if (!gem_truthy(gem_lt(gem_v__for_i_72, gem_len_fn(NULL, _t3561, 1)))) break;
#line 1456 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_72, gem_v__for_i_72);
#line 1456 "compiler/main.gem"
                gem_v__for_i_72 = gem_add(gem_v__for_i_72, gem_int(1));
#line 1457 "compiler/main.gem"
    GemVal _t3562[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3563 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3563.fn(_t3563.env, _t3562, 2)), gem_string("\n"));
            }

        }
#line 1460 "compiler/main.gem"
    GemVal _t3564 = gem_v_arm;
        GemVal gem_v_body = gem_table_get(_t3564, gem_string("body"));
#line 1461 "compiler/main.gem"
    GemVal _t3565[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3565, 1), gem_int(0)))) {
#line 1462 "compiler/main.gem"
            GemVal gem_v__for_i_73 = gem_int(0);
#line 1462 "compiler/main.gem"
    GemVal _t3566[] = {gem_v_body};
            GemVal gem_v__for_limit_73 = gem_sub(gem_len_fn(NULL, _t3566, 1), gem_int(1));
#line 1462 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_73, gem_v__for_limit_73))) break;
#line 1462 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_73;
#line 1462 "compiler/main.gem"
                gem_v__for_i_73 = gem_add(gem_v__for_i_73, gem_int(1));
#line 1463 "compiler/main.gem"
    GemVal _t3567[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3568 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3568.fn(_t3568.env, _t3567, 2)), gem_string("\n"));
            }

#line 1465 "compiler/main.gem"
    GemVal _t3569[] = {gem_v_body};
    GemVal _t3570[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t3569, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3571 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3571.fn(_t3571.env, _t3570, 2)), gem_string("\n"));
        } else {
#line 1467 "compiler/main.gem"
    GemVal _t3572[] = {gem_v_p};
    GemVal _t3573[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3572, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3573, 1)), gem_string("    return GEM_NIL;\n")));
        }
    }

#line 1471 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1472 "compiler/main.gem"
    GemVal _t3574[] = {gem_v_p};
    GemVal _t3575[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3574, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3575, 1)), gem_string(" == -2) {\n")));
#line 1473 "compiler/main.gem"
    GemVal _t3577;
    if (!gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
        _t3577 = gem_neq(gem_v_after_body, GEM_NIL);
    } else {
        GemVal _t3576[] = {gem_v_after_body};
        _t3577 = gem_gt(gem_len_fn(NULL, _t3576, 1), gem_int(0));
    }
        if (gem_truthy(_t3577)) {
#line 1474 "compiler/main.gem"
            GemVal gem_v__for_i_75 = gem_int(0);
#line 1474 "compiler/main.gem"
    GemVal _t3578[] = {gem_v_after_body};
            GemVal gem_v__for_limit_75 = gem_sub(gem_len_fn(NULL, _t3578, 1), gem_int(1));
#line 1474 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_75, gem_v__for_limit_75))) break;
#line 1474 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_75;
#line 1474 "compiler/main.gem"
                gem_v__for_i_75 = gem_add(gem_v__for_i_75, gem_int(1));
#line 1475 "compiler/main.gem"
    GemVal _t3579[] = {gem_table_get(gem_v_after_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3580 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3580.fn(_t3580.env, _t3579, 2)), gem_string("\n"));
            }

#line 1477 "compiler/main.gem"
    GemVal _t3581[] = {gem_v_after_body};
    GemVal _t3582[] = {gem_table_get(gem_v_after_body, gem_sub(gem_len_fn(NULL, _t3581, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3583 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3583.fn(_t3583.env, _t3582, 2)), gem_string("\n"));
        } else {
#line 1479 "compiler/main.gem"
    GemVal _t3584[] = {gem_v_p};
    GemVal _t3585[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3584, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3585, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1482 "compiler/main.gem"
    GemVal _t3586[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3586, 1), gem_string("} else {\n")));
#line 1483 "compiler/main.gem"
    GemVal _t3587[] = {gem_v_p};
    GemVal _t3588[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3587, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3588, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1486 "compiler/main.gem"
    GemVal _t3589[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3589, 1), gem_string("}")));
    GemVal _t3590 = gem_v_out;
    gem_pop_frame();
    return _t3590;
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
#line 1493 "compiler/main.gem"
    GemVal gem_v__d76 = gem_v_node;
#line 1493 "compiler/main.gem"
    GemVal _t3592 = gem_v__d76;
    GemVal gem_v_name = gem_table_get(_t3592, gem_string("name"));
#line 1493 "compiler/main.gem"
    GemVal _t3593 = gem_v__d76;
    GemVal gem_v_params = gem_table_get(_t3593, gem_string("params"));
#line 1493 "compiler/main.gem"
    GemVal _t3594 = gem_v__d76;
    GemVal gem_v_ret_type = gem_table_get(_t3594, gem_string("ret_type"));

#line 1495 "compiler/main.gem"
    GemVal _t3595[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3595, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1497 "compiler/main.gem"
    GemVal _t3596 = gem_table_new();
    GemVal gem_v_c_args = _t3596;
#line 1498 "compiler/main.gem"
    GemVal gem_v__for_i_77 = gem_int(0);
#line 1498 "compiler/main.gem"
    GemVal _t3597[] = {gem_v_params};
    GemVal gem_v__for_limit_77 = gem_len_fn(NULL, _t3597, 1);
#line 1498 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_77, gem_v__for_limit_77))) break;
#line 1498 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_77;
#line 1498 "compiler/main.gem"
        gem_v__for_i_77 = gem_add(gem_v__for_i_77, gem_int(1));
#line 1499 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1500 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1501 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1502 "compiler/main.gem"
    GemVal _t3598[] = {gem_v_i};
    GemVal _t3599[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t3598, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3599, 1)), gem_string("].ival;\n")));
#line 1503 "compiler/main.gem"
    GemVal _t3600[] = {gem_v_i};
    GemVal _t3601[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3600, 1))};
            (void)(gem_push_fn(NULL, _t3601, 2));
        } else {
#line 1504 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1505 "compiler/main.gem"
    GemVal _t3602[] = {gem_v_i};
    GemVal _t3603[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t3602, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3603, 1)), gem_string("].fval;\n")));
#line 1506 "compiler/main.gem"
    GemVal _t3604[] = {gem_v_i};
    GemVal _t3605[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3604, 1))};
                (void)(gem_push_fn(NULL, _t3605, 2));
            } else {
#line 1507 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1508 "compiler/main.gem"
    GemVal _t3606[] = {gem_v_i};
    GemVal _t3607[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t3606, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3607, 1)), gem_string("].sval;\n")));
#line 1509 "compiler/main.gem"
    GemVal _t3608[] = {gem_v_i};
    GemVal _t3609[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3608, 1))};
                    (void)(gem_push_fn(NULL, _t3609, 2));
                } else {
#line 1510 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1511 "compiler/main.gem"
    GemVal _t3610[] = {gem_v_i};
    GemVal _t3611[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t3610, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3611, 1)), gem_string("].bval;\n")));
#line 1512 "compiler/main.gem"
    GemVal _t3612[] = {gem_v_i};
    GemVal _t3613[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3612, 1))};
                        (void)(gem_push_fn(NULL, _t3613, 2));
                    } else {
#line 1513 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1514 "compiler/main.gem"
    GemVal _t3614[] = {gem_v_i};
    GemVal _t3615[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t3614, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t3615, 1)), gem_string("].ival;\n")));
#line 1515 "compiler/main.gem"
    GemVal _t3616[] = {gem_v_i};
    GemVal _t3617[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3616, 1))};
                            (void)(gem_push_fn(NULL, _t3617, 2));
                        } else {
#line 1516 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1517 "compiler/main.gem"
    GemVal _t3618[] = {gem_v_i};
    GemVal _t3619[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3618, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3619, 2));
                            } else {
#line 1519 "compiler/main.gem"
    GemVal _t3620[] = {gem_v_i};
    GemVal _t3621[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3620, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3621, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1524 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1525 "compiler/main.gem"
    GemVal gem_v__for_i_78 = gem_int(0);
#line 1525 "compiler/main.gem"
    GemVal _t3622[] = {gem_v_c_args};
    GemVal gem_v__for_limit_78 = gem_len_fn(NULL, _t3622, 1);
#line 1525 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_78, gem_v__for_limit_78))) break;
#line 1525 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_78;
#line 1525 "compiler/main.gem"
        gem_v__for_i_78 = gem_add(gem_v__for_i_78, gem_int(1));
#line 1526 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1527 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1529 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1532 "compiler/main.gem"
    GemVal _t3623[] = {gem_v_name};
    GemVal _t3624[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3623, 1), gem_string("(")), gem_to_string_fn(NULL, _t3624, 1)), gem_string(")"));
#line 1534 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1535 "compiler/main.gem"
    GemVal _t3625[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3625, 1)), gem_string(";\n")));
#line 1536 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
#line 1537 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1538 "compiler/main.gem"
    GemVal _t3626[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t3626, 1)), gem_string(";\n")));
        } else {
#line 1539 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1540 "compiler/main.gem"
    GemVal _t3627[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t3627, 1)), gem_string(";\n")));
#line 1541 "compiler/main.gem"
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
#line 1542 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1543 "compiler/main.gem"
    GemVal _t3628[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t3628, 1)), gem_string(";\n")));
#line 1544 "compiler/main.gem"
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
#line 1545 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1546 "compiler/main.gem"
    GemVal _t3629[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t3629, 1)), gem_string(";\n")));
#line 1547 "compiler/main.gem"
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
#line 1548 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1549 "compiler/main.gem"
    GemVal _t3630[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t3630, 1)), gem_string(";\n")));
#line 1550 "compiler/main.gem"
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
#line 1551 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1552 "compiler/main.gem"
    GemVal _t3631[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t3631, 1)), gem_string(";\n")));
#line 1553 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
#line 1555 "compiler/main.gem"
    GemVal _t3632[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3632, 1)), gem_string(";\n")));
#line 1556 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1559 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1562 "compiler/main.gem"
    GemVal _t3633[] = {gem_v_name};
    GemVal _t3634[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3633, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t3634, 2));
#line 1565 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1566 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1567 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1568 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1569 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1570 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1571 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1572 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1573 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1574 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1575 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1576 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1577 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1580 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1581 "compiler/main.gem"
    GemVal _t3635[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3635, 1), gem_int(0)))) {
#line 1582 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1584 "compiler/main.gem"
        GemVal gem_v__for_i_79 = gem_int(0);
#line 1584 "compiler/main.gem"
    GemVal _t3636[] = {gem_v_params};
        GemVal gem_v__for_limit_79 = gem_len_fn(NULL, _t3636, 1);
#line 1584 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_79, gem_v__for_limit_79))) break;
#line 1584 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_79;
#line 1584 "compiler/main.gem"
            gem_v__for_i_79 = gem_add(gem_v__for_i_79, gem_int(1));
#line 1585 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1586 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1588 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1589 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1590 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1591 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1592 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1593 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1594 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1595 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1596 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1597 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1598 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1599 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1600 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1602 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1607 "compiler/main.gem"
    GemVal _t3637[] = {gem_v_c_ret};
    GemVal _t3638[] = {gem_v_name};
    GemVal _t3639[] = {gem_v_c_params};
    GemVal _t3640[] = {(*gem_v_extern_c_decls), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3637, 1), gem_string(" ")), gem_to_string_fn(NULL, _t3638, 1)), gem_string("(")), gem_to_string_fn(NULL, _t3639, 1)), gem_string(");"))};
    (void)(gem_push_fn(NULL, _t3640, 2));
    GemVal _t3641 = gem_v_lines;
    gem_pop_frame();
    return _t3641;
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
#line 1615 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1617 "compiler/main.gem"
    GemVal gem_v__for_i_80 = gem_int(0);
#line 1617 "compiler/main.gem"
    GemVal _t3643[] = {gem_v_params};
    GemVal gem_v__for_limit_80 = gem_len_fn(NULL, _t3643, 1);
#line 1617 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_80, gem_v__for_limit_80))) break;
#line 1617 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_80;
#line 1617 "compiler/main.gem"
        gem_v__for_i_80 = gem_add(gem_v__for_i_80, gem_int(1));
#line 1618 "compiler/main.gem"
    GemVal _t3644[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t3645 = (*gem_v_mangle);
        GemVal gem_v_mp = _t3645.fn(_t3645.env, _t3644, 1);
#line 1619 "compiler/main.gem"
    GemVal _t3646[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3646, 2))) {
#line 1620 "compiler/main.gem"
    GemVal _t3647[] = {gem_v_mp};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3647, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1621 "compiler/main.gem"
    GemVal _t3648[] = {gem_v_mp};
    GemVal _t3649[] = {gem_v_i};
    GemVal _t3650[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3648, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t3649, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t3650, 1)), gem_string("] : GEM_NIL;\n")));
        } else {
#line 1623 "compiler/main.gem"
    GemVal _t3651[] = {gem_v_mp};
    GemVal _t3652[] = {gem_v_i};
    GemVal _t3653[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3651, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t3652, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t3653, 1)), gem_string("] : GEM_NIL;\n")));
        }
    }

#line 1628 "compiler/main.gem"
    GemVal _t3654 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3654, gem_string("rest_param")), GEM_NIL))) {
#line 1629 "compiler/main.gem"
    GemVal _t3655[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t3655, 1);
#line 1630 "compiler/main.gem"
        GemVal gem_v_end_count = gem_int(0);
#line 1631 "compiler/main.gem"
    GemVal _t3656 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3656, gem_string("block_param")), GEM_NIL))) {
#line 1632 "compiler/main.gem"
            gem_v_end_count = gem_int(1);
        }
#line 1634 "compiler/main.gem"
    GemVal _t3657 = gem_v_node;
    GemVal _t3658[] = {gem_table_get(_t3657, gem_string("rest_param"))};
    GemVal _t3659 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t3659.fn(_t3659.env, _t3658, 1);
#line 1635 "compiler/main.gem"
    GemVal _t3660 = gem_v_node;
    GemVal _t3661[] = {gem_v_cap_set, gem_table_get(_t3660, gem_string("rest_param"))};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3661, 2))) {
#line 1636 "compiler/main.gem"
    GemVal _t3662[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3662, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1637 "compiler/main.gem"
    GemVal _t3663[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3663, 1)), gem_string(" = gem_table_new();\n")));
#line 1638 "compiler/main.gem"
    GemVal _t3664[] = {gem_v_N};
    GemVal _t3665[] = {gem_v_end_count};
    GemVal _t3666[] = {gem_v_mp_rest};
    GemVal _t3667[] = {gem_v_N};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t3664, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t3665, 1)), gem_string("; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t3666, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t3667, 1)), gem_string("), args[_ri]);\n")));
        } else {
#line 1640 "compiler/main.gem"
    GemVal _t3668[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3668, 1)), gem_string(" = gem_table_new();\n")));
#line 1641 "compiler/main.gem"
    GemVal _t3669[] = {gem_v_N};
    GemVal _t3670[] = {gem_v_end_count};
    GemVal _t3671[] = {gem_v_mp_rest};
    GemVal _t3672[] = {gem_v_N};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t3669, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t3670, 1)), gem_string("; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t3671, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t3672, 1)), gem_string("), args[_ri]);\n")));
        }
#line 1643 "compiler/main.gem"
    GemVal _t3673 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3673, gem_string("block_param")), GEM_NIL))) {
#line 1644 "compiler/main.gem"
    GemVal _t3674 = gem_v_node;
    GemVal _t3675[] = {gem_table_get(_t3674, gem_string("block_param"))};
    GemVal _t3676 = (*gem_v_mangle);
            GemVal gem_v_mp_block = _t3676.fn(_t3676.env, _t3675, 1);
#line 1645 "compiler/main.gem"
    GemVal _t3677 = gem_v_node;
    GemVal _t3678[] = {gem_v_cap_set, gem_table_get(_t3677, gem_string("block_param"))};
            if (gem_truthy(gem_fn_set_contains(NULL, _t3678, 2))) {
#line 1646 "compiler/main.gem"
    GemVal _t3679[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3679, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1647 "compiler/main.gem"
    GemVal _t3680[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3680, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            } else {
#line 1649 "compiler/main.gem"
    GemVal _t3681[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3681, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            }
        }
    }
    GemVal _t3682 = gem_v_out;
    gem_pop_frame();
    return _t3682;
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
#line 1659 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1660 "compiler/main.gem"
    GemVal _t3684[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3684, 1), gem_int(0)))) {
#line 1661 "compiler/main.gem"
        GemVal gem_v__for_i_81 = gem_int(0);
#line 1661 "compiler/main.gem"
    GemVal _t3685[] = {gem_v_body};
        GemVal gem_v__for_limit_81 = gem_sub(gem_len_fn(NULL, _t3685, 1), gem_int(1));
#line 1661 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_81, gem_v__for_limit_81))) break;
#line 1661 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_81;
#line 1661 "compiler/main.gem"
            gem_v__for_i_81 = gem_add(gem_v__for_i_81, gem_int(1));
#line 1662 "compiler/main.gem"
    GemVal _t3686[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t3687 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3687.fn(_t3687.env, _t3686, 2)), gem_string("\n"));
        }

#line 1664 "compiler/main.gem"
    GemVal _t3688[] = {gem_v_body};
    GemVal _t3689[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t3688, 1), gem_int(1))), gem_int(1)};
    GemVal _t3690 = (*gem_v_compile_stmt_return);
        gem_v_out = gem_add(gem_add(gem_v_out, _t3690.fn(_t3690.env, _t3689, 2)), gem_string("\n"));
    } else {
#line 1666 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
    GemVal _t3691 = gem_v_out;
    gem_pop_frame();
    return _t3691;
}

struct _closure__anon_53 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_local_names;
    GemVal *gem_v_source_name;
};
static GemVal _anon_53(void *_env, GemVal *args, int argc) {
    struct _closure__anon_53 *_cls = (struct _closure__anon_53 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_emit_fn_body = _cls->gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings = _cls->gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_53", "compiler/main.gem", 0);
#line 1674 "compiler/main.gem"
    GemVal gem_v__d82 = gem_v_node;
#line 1674 "compiler/main.gem"
    GemVal _t3693 = gem_v__d82;
    GemVal gem_v_params = gem_table_get(_t3693, gem_string("params"));
#line 1674 "compiler/main.gem"
    GemVal _t3694 = gem_v__d82;
    GemVal gem_v_body = gem_table_get(_t3694, gem_string("body"));
#line 1674 "compiler/main.gem"
    GemVal _t3695 = gem_v__d82;
    GemVal gem_v_name = gem_table_get(_t3695, gem_string("name"));

#line 1677 "compiler/main.gem"
    GemVal _t3696[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t3696, 1);
#line 1678 "compiler/main.gem"
    GemVal _t3697 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3697, gem_string("rest_param")), GEM_NIL))) {
#line 1679 "compiler/main.gem"
    GemVal _t3698 = gem_v_node;
    GemVal _t3699[] = {gem_v_scope_vars, gem_table_get(_t3698, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t3699, 2));
    }
#line 1681 "compiler/main.gem"
    GemVal _t3700 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3700, gem_string("block_param")), GEM_NIL))) {
#line 1682 "compiler/main.gem"
    GemVal _t3701 = gem_v_node;
    GemVal _t3702[] = {gem_v_scope_vars, gem_table_get(_t3701, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t3702, 2));
    }
#line 1684 "compiler/main.gem"
    GemVal _t3703[] = {gem_v_body, gem_v_scope_vars};
    (void)(gem_fn_collect_top_let_names(NULL, _t3703, 2));
#line 1686 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1687 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1688 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1689 "compiler/main.gem"
    *gem_v_local_names = gem_v_scope_vars;
#line 1691 "compiler/main.gem"
    GemVal _t3704[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t3705 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t3705.fn(_t3705.env, _t3704, 2);
#line 1693 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1694 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1696 "compiler/main.gem"
    GemVal _t3706[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3706, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1699 "compiler/main.gem"
    GemVal _t3707 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3707, gem_string("line")), GEM_NIL))) {
#line 1700 "compiler/main.gem"
    GemVal _t3708 = gem_v_node;
    GemVal _t3709[] = {gem_table_get(_t3708, gem_string("line"))};
    GemVal _t3710[] = {(*gem_v_source_name)};
    GemVal _t3711[] = {gem_fn_escape_c_string(NULL, _t3710, 1)};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t3709, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t3711, 1)), gem_string("\"\n")));
    }
#line 1703 "compiler/main.gem"
    GemVal _t3712[] = {gem_v_node, gem_v_params, gem_v_captured};
    GemVal _t3713 = (*gem_v_emit_param_bindings);
    gem_v_lines = gem_add(gem_v_lines, _t3713.fn(_t3713.env, _t3712, 3));
#line 1705 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1706 "compiler/main.gem"
    GemVal _t3714 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3714, gem_string("line")), GEM_NIL))) {
#line 1707 "compiler/main.gem"
    GemVal _t3715 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t3715, gem_string("line"));
    }
#line 1709 "compiler/main.gem"
    GemVal _t3716[] = {gem_v_name};
    GemVal _t3717[] = {gem_fn_escape_c_string(NULL, _t3716, 1)};
    GemVal _t3718[] = {(*gem_v_source_name)};
    GemVal _t3719[] = {gem_fn_escape_c_string(NULL, _t3718, 1)};
    GemVal _t3720[] = {gem_v_fn_line};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t3717, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t3719, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t3720, 1)), gem_string(");\n")));
#line 1711 "compiler/main.gem"
    GemVal _t3721[] = {gem_v_body};
    GemVal _t3722 = (*gem_v_emit_fn_body);
    gem_v_lines = gem_add(gem_v_lines, _t3722.fn(_t3722.env, _t3721, 1));
#line 1713 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1714 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1715 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1716 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
    GemVal _t3723 = gem_v_lines;
    gem_pop_frame();
    return _t3723;
}

struct _closure__anon_54 {
    GemVal *gem_v_anon_name;
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_functions;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_local_names;
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
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_captures = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_54", "compiler/main.gem", 0);
#line 1723 "compiler/main.gem"
    GemVal gem_v__d83 = gem_v_node;
#line 1723 "compiler/main.gem"
    GemVal _t3725 = gem_v__d83;
    GemVal gem_v_params = gem_table_get(_t3725, gem_string("params"));
#line 1723 "compiler/main.gem"
    GemVal _t3726 = gem_v__d83;
    GemVal gem_v_body = gem_table_get(_t3726, gem_string("body"));

#line 1724 "compiler/main.gem"
    GemVal _t3727 = (*gem_v_anon_name);
    GemVal gem_v_name = _t3727.fn(_t3727.env, NULL, 0);
#line 1727 "compiler/main.gem"
    GemVal _t3728[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t3728, 1);
#line 1728 "compiler/main.gem"
    GemVal _t3729 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3729, gem_string("rest_param")), GEM_NIL))) {
#line 1729 "compiler/main.gem"
    GemVal _t3730 = gem_v_node;
    GemVal _t3731[] = {gem_v_inner_scope, gem_table_get(_t3730, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t3731, 2));
    }
#line 1731 "compiler/main.gem"
    GemVal _t3732 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3732, gem_string("block_param")), GEM_NIL))) {
#line 1732 "compiler/main.gem"
    GemVal _t3733 = gem_v_node;
    GemVal _t3734[] = {gem_v_inner_scope, gem_table_get(_t3733, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t3734, 2));
    }
#line 1734 "compiler/main.gem"
    GemVal _t3735[] = {gem_v_body, gem_v_inner_scope};
    (void)(gem_fn_collect_top_let_names(NULL, _t3735, 2));
#line 1736 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1737 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1738 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1739 "compiler/main.gem"
    GemVal _t3736[] = {gem_v_captures};
    GemVal _t3737[] = {gem_v_inner_scope, gem_fn_sorted_array_to_set(NULL, _t3736, 1)};
    *gem_v_local_names = gem_fn_set_union(NULL, _t3737, 2);
#line 1741 "compiler/main.gem"
    GemVal _t3738[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t3739 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t3739.fn(_t3739.env, _t3738, 2);
#line 1743 "compiler/main.gem"
    GemVal _t3740[] = {gem_v_captures};
    GemVal _t3741[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t3740, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t3741, 2);
#line 1744 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1747 "compiler/main.gem"
    GemVal _t3742[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t3742, 1));
#line 1748 "compiler/main.gem"
    GemVal _t3743[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t3743, 1)), gem_string(" {\n"));
#line 1749 "compiler/main.gem"
    GemVal gem_v__for_items_84 = gem_v_captures;
#line 1749 "compiler/main.gem"
    GemVal gem_v__for_i_84 = gem_int(0);
#line 1749 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3744[] = {gem_v__for_items_84};
        if (!gem_truthy(gem_lt(gem_v__for_i_84, gem_len_fn(NULL, _t3744, 1)))) break;
#line 1749 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_84, gem_v__for_i_84);
#line 1749 "compiler/main.gem"
        gem_v__for_i_84 = gem_add(gem_v__for_i_84, gem_int(1));
#line 1750 "compiler/main.gem"
    GemVal _t3745[] = {gem_v_cap};
    GemVal _t3746 = (*gem_v_mangle);
    GemVal _t3747[] = {_t3746.fn(_t3746.env, _t3745, 1)};
        gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3747, 1)), gem_string(";\n")));
    }

#line 1752 "compiler/main.gem"
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
#line 1755 "compiler/main.gem"
    GemVal _t3748[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t3748, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1756 "compiler/main.gem"
    GemVal _t3749[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3749, 1), gem_int(0)))) {
#line 1757 "compiler/main.gem"
    GemVal _t3750[] = {gem_v_struct_name};
    GemVal _t3751[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t3750, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t3751, 1)), gem_string(" *)_env;\n")));
#line 1758 "compiler/main.gem"
        GemVal gem_v__for_items_85 = gem_v_captures;
#line 1758 "compiler/main.gem"
        GemVal gem_v__for_i_85 = gem_int(0);
#line 1758 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t3752[] = {gem_v__for_items_85};
            if (!gem_truthy(gem_lt(gem_v__for_i_85, gem_len_fn(NULL, _t3752, 1)))) break;
#line 1758 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_85, gem_v__for_i_85);
#line 1758 "compiler/main.gem"
            gem_v__for_i_85 = gem_add(gem_v__for_i_85, gem_int(1));
#line 1759 "compiler/main.gem"
    GemVal _t3753[] = {gem_v_cap};
    GemVal _t3754 = (*gem_v_mangle);
            GemVal gem_v_mc = _t3754.fn(_t3754.env, _t3753, 1);
#line 1760 "compiler/main.gem"
    GemVal _t3755[] = {gem_v_mc};
    GemVal _t3756[] = {gem_v_mc};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3755, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t3756, 1)), gem_string(";\n")));
        }

    }
#line 1764 "compiler/main.gem"
    GemVal _t3757[] = {gem_v_node, gem_v_params, gem_v_inner_captured};
    GemVal _t3758 = (*gem_v_emit_param_bindings);
    gem_v_lines = gem_add(gem_v_lines, _t3758.fn(_t3758.env, _t3757, 3));
#line 1766 "compiler/main.gem"
    GemVal _t3759[] = {gem_v_name};
    GemVal _t3760[] = {gem_fn_escape_c_string(NULL, _t3759, 1)};
    GemVal _t3761[] = {(*gem_v_source_name)};
    GemVal _t3762[] = {gem_fn_escape_c_string(NULL, _t3761, 1)};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t3760, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t3762, 1)), gem_string("\", 0);\n")));
#line 1768 "compiler/main.gem"
    GemVal _t3763[] = {gem_v_body};
    GemVal _t3764 = (*gem_v_emit_fn_body);
    gem_v_lines = gem_add(gem_v_lines, _t3764.fn(_t3764.env, _t3763, 1));
#line 1770 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1771 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1772 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1773 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1776 "compiler/main.gem"
    GemVal _t3765[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t3765, 2));
    GemVal _t3766 = gem_table_new();
    gem_table_set(_t3766, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t3766, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t3766, gem_string("captures"), gem_v_captures);
    GemVal _t3767 = _t3766;
    gem_pop_frame();
    return _t3767;
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
    GemVal *gem_v_local_names;
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
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_top_level_boxed = _cls->gem_v_top_level_boxed;
    GemVal *gem_v_top_level_vars = _cls->gem_v_top_level_vars;
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_55", "compiler/main.gem", 0);
#line 1785 "compiler/main.gem"
    GemVal _t3769 = gem_table_new();
    GemVal gem_v_fn_defs = _t3769;
#line 1786 "compiler/main.gem"
    GemVal _t3770 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t3770;
#line 1787 "compiler/main.gem"
    GemVal _t3771 = gem_table_new();
    GemVal gem_v_extern_includes = _t3771;
#line 1788 "compiler/main.gem"
    GemVal _t3772 = gem_table_new();
    GemVal gem_v_top_stmts = _t3772;
#line 1790 "compiler/main.gem"
    GemVal _t3773 = gem_v_ast;
    GemVal gem_v__for_items_86 = gem_table_get(_t3773, gem_string("stmts"));
#line 1790 "compiler/main.gem"
    GemVal gem_v__for_i_86 = gem_int(0);
#line 1790 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3774[] = {gem_v__for_items_86};
        if (!gem_truthy(gem_lt(gem_v__for_i_86, gem_len_fn(NULL, _t3774, 1)))) break;
#line 1790 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_86, gem_v__for_i_86);
#line 1790 "compiler/main.gem"
        gem_v__for_i_86 = gem_add(gem_v__for_i_86, gem_int(1));
#line 1791 "compiler/main.gem"
    GemVal _t3775 = gem_v_s;
        if (gem_truthy(gem_eq(gem_table_get(_t3775, gem_string("tag")), gem_string("fn_def")))) {
#line 1792 "compiler/main.gem"
    GemVal _t3776[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t3776, 2));
        } else {
#line 1793 "compiler/main.gem"
    GemVal _t3777 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t3777, gem_string("tag")), gem_string("extern_fn")))) {
#line 1794 "compiler/main.gem"
    GemVal _t3778[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t3778, 2));
            } else {
#line 1795 "compiler/main.gem"
    GemVal _t3779 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t3779, gem_string("tag")), gem_string("extern_include")))) {
#line 1796 "compiler/main.gem"
    GemVal _t3780[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t3780, 2));
                } else {
#line 1798 "compiler/main.gem"
    GemVal _t3781[] = {gem_v_top_stmts, gem_v_s};
                    (void)(gem_push_fn(NULL, _t3781, 2));
                }
            }
        }
    }

#line 1803 "compiler/main.gem"
    GemVal gem_v__for_items_87 = gem_v_extern_fns_list;
#line 1803 "compiler/main.gem"
    GemVal gem_v__for_i_87 = gem_int(0);
#line 1803 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3782[] = {gem_v__for_items_87};
        if (!gem_truthy(gem_lt(gem_v__for_i_87, gem_len_fn(NULL, _t3782, 1)))) break;
#line 1803 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_87, gem_v__for_i_87);
#line 1803 "compiler/main.gem"
        gem_v__for_i_87 = gem_add(gem_v__for_i_87, gem_int(1));
#line 1804 "compiler/main.gem"
    GemVal _t3783 = gem_v_ef;
    GemVal _t3784[] = {(*gem_v_defined_fns), gem_table_get(_t3783, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t3784, 2));
    }

#line 1808 "compiler/main.gem"
    GemVal gem_v__for_items_88 = gem_v_fn_defs;
#line 1808 "compiler/main.gem"
    GemVal gem_v__for_i_88 = gem_int(0);
#line 1808 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3785[] = {gem_v__for_items_88};
        if (!gem_truthy(gem_lt(gem_v__for_i_88, gem_len_fn(NULL, _t3785, 1)))) break;
#line 1808 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_88, gem_v__for_i_88);
#line 1808 "compiler/main.gem"
        gem_v__for_i_88 = gem_add(gem_v__for_i_88, gem_int(1));
#line 1809 "compiler/main.gem"
    GemVal _t3786 = gem_v_fd;
    GemVal _t3787[] = {(*gem_v_defined_fns), gem_table_get(_t3786, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t3787, 2));
#line 1810 "compiler/main.gem"
    GemVal _t3788 = gem_v_fd;
    GemVal _t3789[] = {gem_table_get(_t3788, gem_string("name"))};
        GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3789, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1811 "compiler/main.gem"
    GemVal _t3790[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t3790, 2));
    }

#line 1815 "compiler/main.gem"
    GemVal _t3791[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn_collect_top_let_names(NULL, _t3791, 2));
#line 1816 "compiler/main.gem"
    GemVal _t3792 = gem_table_new();
    GemVal _t3793[] = {gem_v_top_stmts, _t3792};
    GemVal _t3794 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t3794.fn(_t3794.env, _t3793, 2);
#line 1819 "compiler/main.gem"
    GemVal gem_v__for_items_89 = gem_v_extern_fns_list;
#line 1819 "compiler/main.gem"
    GemVal gem_v__for_i_89 = gem_int(0);
#line 1819 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3795[] = {gem_v__for_items_89};
        if (!gem_truthy(gem_lt(gem_v__for_i_89, gem_len_fn(NULL, _t3795, 1)))) break;
#line 1819 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_89, gem_v__for_i_89);
#line 1819 "compiler/main.gem"
        gem_v__for_i_89 = gem_add(gem_v__for_i_89, gem_int(1));
#line 1820 "compiler/main.gem"
    GemVal _t3796[] = {gem_v_ef};
    GemVal _t3797 = (*gem_v_compile_extern_fn);
    GemVal _t3798[] = {(*gem_v_functions), _t3797.fn(_t3797.env, _t3796, 1)};
        (void)(gem_push_fn(NULL, _t3798, 2));
    }

#line 1824 "compiler/main.gem"
    GemVal gem_v__for_items_90 = gem_v_fn_defs;
#line 1824 "compiler/main.gem"
    GemVal gem_v__for_i_90 = gem_int(0);
#line 1824 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3799[] = {gem_v__for_items_90};
        if (!gem_truthy(gem_lt(gem_v__for_i_90, gem_len_fn(NULL, _t3799, 1)))) break;
#line 1824 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_90, gem_v__for_i_90);
#line 1824 "compiler/main.gem"
        gem_v__for_i_90 = gem_add(gem_v__for_i_90, gem_int(1));
#line 1825 "compiler/main.gem"
    GemVal _t3800[] = {gem_v_fd};
    GemVal _t3801 = (*gem_v_compile_fn);
    GemVal _t3802[] = {(*gem_v_functions), _t3801.fn(_t3801.env, _t3800, 1)};
        (void)(gem_push_fn(NULL, _t3802, 2));
    }

#line 1829 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1830 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1831 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1832 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1833 "compiler/main.gem"
    GemVal _t3803 = gem_table_new();
    *gem_v_local_names = _t3803;
#line 1834 "compiler/main.gem"
    GemVal gem_v__for_tbl_91 = (*gem_v_top_level_vars);
#line 1834 "compiler/main.gem"
    GemVal _t3804[] = {gem_v__for_tbl_91};
    GemVal gem_v__for_keys_91 = gem_keys_fn(NULL, _t3804, 1);
#line 1834 "compiler/main.gem"
    GemVal gem_v__for_i_91 = gem_int(0);
#line 1834 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3805[] = {gem_v__for_keys_91};
        if (!gem_truthy(gem_lt(gem_v__for_i_91, gem_len_fn(NULL, _t3805, 1)))) break;
#line 1834 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_91, gem_v__for_i_91);
#line 1834 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_91, gem_table_get(gem_v__for_keys_91, gem_v__for_i_91));
#line 1834 "compiler/main.gem"
        gem_v__for_i_91 = gem_add(gem_v__for_i_91, gem_int(1));
#line 1835 "compiler/main.gem"
    GemVal _t3806[] = {(*gem_v_local_names), gem_v_k};
        (void)(gem_fn_set_add(NULL, _t3806, 2));
    }

#line 1837 "compiler/main.gem"
    GemVal _t3807[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t3808 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t3808.fn(_t3808.env, _t3807, 2);
#line 1838 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1839 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1840 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1842 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 1843 "compiler/main.gem"
    GemVal gem_v__for_items_92 = gem_v_fn_defs;
#line 1843 "compiler/main.gem"
    GemVal gem_v__for_i_92 = gem_int(0);
#line 1843 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3809[] = {gem_v__for_items_92};
        if (!gem_truthy(gem_lt(gem_v__for_i_92, gem_len_fn(NULL, _t3809, 1)))) break;
#line 1843 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_92, gem_v__for_i_92);
#line 1843 "compiler/main.gem"
        gem_v__for_i_92 = gem_add(gem_v__for_i_92, gem_int(1));
#line 1844 "compiler/main.gem"
    GemVal _t3810 = gem_v_fd;
        if (gem_truthy(gem_eq(gem_table_get(_t3810, gem_string("name")), gem_string("main")))) {
#line 1845 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 1849 "compiler/main.gem"
    GemVal gem_v_main_c = gem_string("int main(int argc, char **argv) {\n");
#line 1850 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GC_INIT();\n"));
#line 1851 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_init(argc, argv);\n"));
#line 1852 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_scheduler_init();\n"));
#line 1853 "compiler/main.gem"
    GemVal _t3811[] = {(*gem_v_source_name)};
    GemVal _t3812[] = {gem_fn_escape_c_string(NULL, _t3811, 1)};
    gem_v_main_c = gem_add(gem_v_main_c, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t3812, 1)), gem_string("\", 0);\n")));
#line 1854 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1855 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
#line 1857 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1858 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
#line 1859 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
#line 1861 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_run_scheduler();\n"));
#line 1862 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_pop_frame();\n    return 0;\n}\n"));
#line 1866 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1867 "compiler/main.gem"
    GemVal gem_v__for_items_93 = gem_v_extern_includes;
#line 1867 "compiler/main.gem"
    GemVal gem_v__for_i_93 = gem_int(0);
#line 1867 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3813[] = {gem_v__for_items_93};
        if (!gem_truthy(gem_lt(gem_v__for_i_93, gem_len_fn(NULL, _t3813, 1)))) break;
#line 1867 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_93, gem_v__for_i_93);
#line 1867 "compiler/main.gem"
        gem_v__for_i_93 = gem_add(gem_v__for_i_93, gem_int(1));
#line 1868 "compiler/main.gem"
    GemVal _t3814 = gem_v_ei;
    GemVal _t3815[] = {gem_table_get(_t3814, gem_string("path"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t3815, 1)), gem_string("\"\n")));
    }

#line 1870 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
#line 1874 "compiler/main.gem"
    GemVal _t3816[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3816, 1), gem_int(0)))) {
#line 1875 "compiler/main.gem"
        GemVal gem_v__for_i_94 = gem_int(0);
#line 1875 "compiler/main.gem"
    GemVal _t3817[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_94 = gem_len_fn(NULL, _t3817, 1);
#line 1875 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_94, gem_v__for_limit_94))) break;
#line 1875 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_94;
#line 1875 "compiler/main.gem"
            gem_v__for_i_94 = gem_add(gem_v__for_i_94, gem_int(1));
#line 1876 "compiler/main.gem"
            gem_v_out = gem_add(gem_add(gem_v_out, gem_table_get((*gem_v_extern_c_decls), gem_v_i)), gem_string("\n"));
        }

    }
#line 1881 "compiler/main.gem"
    GemVal gem_v__for_i_95 = gem_int(0);
#line 1881 "compiler/main.gem"
    GemVal _t3818[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_95 = gem_len_fn(NULL, _t3818, 1);
#line 1881 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_95, gem_v__for_limit_95))) break;
#line 1881 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_95;
#line 1881 "compiler/main.gem"
        gem_v__for_i_95 = gem_add(gem_v__for_i_95, gem_int(1));
#line 1882 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
#line 1883 "compiler/main.gem"
    GemVal _t3819[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3819, 1), gem_int(1))))) {
#line 1884 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1887 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1890 "compiler/main.gem"
    GemVal _t3820[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t3820, 1);
#line 1891 "compiler/main.gem"
    GemVal gem_v__for_i_96 = gem_int(0);
#line 1891 "compiler/main.gem"
    GemVal _t3821[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_96 = gem_len_fn(NULL, _t3821, 1);
#line 1891 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_96, gem_v__for_limit_96))) break;
#line 1891 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_96;
#line 1891 "compiler/main.gem"
        gem_v__for_i_96 = gem_add(gem_v__for_i_96, gem_int(1));
#line 1892 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 1893 "compiler/main.gem"
    GemVal _t3822[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3822, 2))) {
#line 1894 "compiler/main.gem"
    GemVal _t3823[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal *gem_v_"), gem_to_string_fn(NULL, _t3823, 1)), gem_string(";\n")));
        } else {
#line 1896 "compiler/main.gem"
    GemVal _t3824[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal gem_v_"), gem_to_string_fn(NULL, _t3824, 1)), gem_string(";\n")));
        }
    }

#line 1899 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1902 "compiler/main.gem"
    GemVal gem_v__for_i_97 = gem_int(0);
#line 1902 "compiler/main.gem"
    GemVal _t3825[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_97 = gem_len_fn(NULL, _t3825, 1);
#line 1902 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_97, gem_v__for_limit_97))) break;
#line 1902 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_97;
#line 1902 "compiler/main.gem"
        gem_v__for_i_97 = gem_add(gem_v__for_i_97, gem_int(1));
#line 1903 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
#line 1904 "compiler/main.gem"
    GemVal _t3826[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3826, 1), gem_int(1))))) {
#line 1905 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
        }
    }

#line 1908 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
#line 1910 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    GemVal _t3827 = gem_v_out;
    gem_pop_frame();
    return _t3827;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 123 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_codegen", "compiler/main.gem", 123);
#line 124 "compiler/main.gem"
    GemVal _t1524 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1524;
#line 125 "compiler/main.gem"
    GemVal _t1525 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1525;
#line 126 "compiler/main.gem"
    GemVal _t1526 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1526;
#line 127 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 128 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 129 "compiler/main.gem"
    GemVal _t1527 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1527;
#line 130 "compiler/main.gem"
    GemVal _t1528 = gem_table_new();
    gem_table_set(_t1528, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1528, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1528, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1528, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1528, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1528, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1528, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1528, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1528, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1528, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1528, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1528, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1528, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1528, gem_string("to_int"), gem_string("gem_to_int_fn"));
    gem_table_set(_t1528, gem_string("to_float"), gem_string("gem_to_float_fn"));
    gem_table_set(_t1528, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1528, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1528, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1528, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1528, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1528, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1528, gem_string("self"), gem_string("gem_self_builtin"));
    gem_table_set(_t1528, gem_string("monitor"), gem_string("gem_monitor_builtin"));
    gem_table_set(_t1528, gem_string("spawn_monitor"), gem_string("gem_spawn_monitor_builtin"));
    gem_table_set(_t1528, gem_string("register"), gem_string("gem_register_builtin"));
    gem_table_set(_t1528, gem_string("whereis"), gem_string("gem_whereis_builtin"));
    gem_table_set(_t1528, gem_string("time_ms"), gem_string("gem_time_ms_builtin"));
    gem_table_set(_t1528, gem_string("kill"), gem_string("gem_exit_builtin"));
    gem_table_set(_t1528, gem_string("link"), gem_string("gem_link_builtin"));
    gem_table_set(_t1528, gem_string("unlink"), gem_string("gem_unlink_builtin"));
    gem_table_set(_t1528, gem_string("spawn_link"), gem_string("gem_spawn_link_builtin"));
    gem_table_set(_t1528, gem_string("process_flag"), gem_string("gem_process_flag_builtin"));
    gem_table_set(_t1528, gem_string("make_ref"), gem_string("gem_make_ref_builtin"));
    gem_table_set(_t1528, gem_string("send_after"), gem_string("gem_send_after_builtin"));
    gem_table_set(_t1528, gem_string("cancel_timer"), gem_string("gem_cancel_timer_builtin"));
    gem_table_set(_t1528, gem_string("process_info"), gem_string("gem_process_info_builtin"));
    gem_table_set(_t1528, gem_string("read_file"), gem_string("gem_read_file_fn"));
    gem_table_set(_t1528, gem_string("write_file"), gem_string("gem_write_file_fn"));
    gem_table_set(_t1528, gem_string("delete"), gem_string("gem_delete_fn"));
    gem_table_set(_t1528, gem_string("pop"), gem_string("gem_pop_fn"));
    gem_table_set(_t1528, gem_string("values"), gem_string("gem_values_fn"));
    gem_table_set(_t1528, gem_string("eprint"), gem_string("gem_eprint_fn"));
    gem_table_set(_t1528, gem_string("exit"), gem_string("gem_exit_process_fn"));
    gem_table_set(_t1528, gem_string("argv"), gem_string("gem_argv_fn"));
    gem_table_set(_t1528, gem_string("sort"), gem_string("gem_sort_fn"));
    gem_table_set(_t1528, gem_string("floor"), gem_string("gem_floor_fn"));
    gem_table_set(_t1528, gem_string("ceil"), gem_string("gem_ceil_fn"));
    gem_table_set(_t1528, gem_string("round"), gem_string("gem_round_fn"));
    gem_table_set(_t1528, gem_string("abs"), gem_string("gem_abs_fn"));
    gem_table_set(_t1528, gem_string("pow"), gem_string("gem_pow_fn"));
    gem_table_set(_t1528, gem_string("sqrt"), gem_string("gem_sqrt_fn"));
    gem_table_set(_t1528, gem_string("random"), gem_string("gem_random_fn"));
    gem_table_set(_t1528, gem_string("append_file"), gem_string("gem_append_file_fn"));
    gem_table_set(_t1528, gem_string("getenv"), gem_string("gem_getenv_fn"));
    gem_table_set(_t1528, gem_string("input"), gem_string("gem_input_fn"));
    gem_table_set(_t1528, gem_string("insert"), gem_string("gem_insert_fn"));
    gem_table_set(_t1528, gem_string("remove_at"), gem_string("gem_remove_at_fn"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1528;
#line 189 "compiler/main.gem"
    GemVal _t1529 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1529;
#line 190 "compiler/main.gem"
    GemVal gem_v__for_tbl_10 = (*gem_v_builtin_fns);
#line 190 "compiler/main.gem"
    GemVal _t1530[] = {gem_v__for_tbl_10};
    GemVal gem_v__for_keys_10 = gem_keys_fn(NULL, _t1530, 1);
#line 190 "compiler/main.gem"
    GemVal gem_v__for_i_10 = gem_int(0);
#line 190 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1531[] = {gem_v__for_keys_10};
        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t1531, 1)))) break;
#line 190 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_10, gem_v__for_i_10);
#line 190 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_10, gem_table_get(gem_v__for_keys_10, gem_v__for_i_10));
#line 190 "compiler/main.gem"
        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 191 "compiler/main.gem"
        gem_table_set((*gem_v_builtins), gem_v_k, gem_bool(1));
    }

#line 193 "compiler/main.gem"
    GemVal _t1532 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1532;
#line 194 "compiler/main.gem"
    GemVal _t1533 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1533;
#line 195 "compiler/main.gem"
    GemVal _t1534 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1534;
#line 196 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 197 "compiler/main.gem"
    GemVal _t1535 = gem_table_new();
    GemVal *gem_v_local_names = GC_MALLOC(sizeof(GemVal));
    *gem_v_local_names = _t1535;
#line 201 "compiler/main.gem"
    struct _closure__anon_24 *_t1538 = GC_MALLOC(sizeof(struct _closure__anon_24));
    _t1538->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_24, _t1538);
#line 206 "compiler/main.gem"
    struct _closure__anon_25 *_t1541 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1541->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_25, _t1541);
#line 211 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_26, NULL);
#line 217 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 218 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 220 "compiler/main.gem"
    struct _closure__anon_27 *_t1883 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1883->gem_v_builtins = gem_v_builtins;
    _t1883->gem_v_collect_free_node = gem_v_collect_free_node;
    _t1883->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1883->gem_v_defined_fns = gem_v_defined_fns;
    _t1883->gem_v_local_names = gem_v_local_names;
    *gem_v_collect_free_node = gem_make_fn(_anon_27, _t1883);
#line 414 "compiler/main.gem"
    struct _closure__anon_28 *_t1897 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1897->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_28, _t1897);
#line 429 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 430 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 432 "compiler/main.gem"
    struct _closure__anon_29 *_t2086 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t2086->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2086->gem_v_walk_captures = gem_v_walk_captures;
    _t2086->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_29, _t2086);
#line 544 "compiler/main.gem"
    struct _closure__anon_30 *_t2098 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t2098->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_30, _t2098);
#line 555 "compiler/main.gem"
    struct _closure__anon_31 *_t2103 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t2103->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_31, _t2103);
#line 563 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 564 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 565 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 566 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 567 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 568 "compiler/main.gem"
    GemVal *gem_v_compile_if_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if_return = GEM_NIL;
#line 569 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 570 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 571 "compiler/main.gem"
    GemVal *gem_v_compile_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match_return = GEM_NIL;
#line 572 "compiler/main.gem"
    GemVal *gem_v_compile_receive_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_receive_match = GEM_NIL;
#line 573 "compiler/main.gem"
    GemVal *gem_v_compile_receive_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_receive_match_return = GEM_NIL;
#line 574 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 575 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 576 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 577 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 578 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 579 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 580 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 581 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 585 "compiler/main.gem"
    struct _closure__anon_32 *_t2303 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t2303->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2303->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2303->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t2303->gem_v_compile_array = gem_v_compile_array;
    _t2303->gem_v_compile_binop = gem_v_compile_binop;
    _t2303->gem_v_compile_call = gem_v_compile_call;
    _t2303->gem_v_compile_expr = gem_v_compile_expr;
    _t2303->gem_v_compile_table = gem_v_compile_table;
    _t2303->gem_v_defined_fns = gem_v_defined_fns;
    _t2303->gem_v_local_names = gem_v_local_names;
    _t2303->gem_v_mangle = gem_v_mangle;
    _t2303->gem_v_tmp = gem_v_tmp;
    _t2303->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_32, _t2303);
#line 672 "compiler/main.gem"
    struct _closure__anon_33 *_t2320 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t2320->gem_v_compile_expr = gem_v_compile_expr;
    _t2320->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_33, _t2320);
#line 687 "compiler/main.gem"
    struct _closure__anon_34 *_t2334 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t2334->gem_v_compile_expr = gem_v_compile_expr;
    _t2334->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_34, _t2334);
#line 701 "compiler/main.gem"
    struct _closure__anon_35 *_t2379 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t2379->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2379->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2379->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t2379->gem_v_mangle = gem_v_mangle;
    _t2379->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_35, _t2379);
#line 736 "compiler/main.gem"
    struct _closure__anon_36 *_t2395 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t2395->gem_v_compile_expr = gem_v_compile_expr;
    _t2395->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_36, _t2395);
#line 757 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_37, NULL);
#line 766 "compiler/main.gem"
    struct _closure__anon_38 *_t2487 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2487->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2487->gem_v_compile_args = gem_v_compile_args;
    _t2487->gem_v_compile_expr = gem_v_compile_expr;
    _t2487->gem_v_defined_fns = gem_v_defined_fns;
    _t2487->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t2487->gem_v_local_names = gem_v_local_names;
    _t2487->gem_v_source_name = gem_v_source_name;
    _t2487->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_38, _t2487);
#line 810 "compiler/main.gem"
    struct _closure__anon_39 *_t2640 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2640->gem_v_compile_expr = gem_v_compile_expr;
    _t2640->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_39, _t2640);
#line 889 "compiler/main.gem"
    struct _closure__anon_40 *_t2645 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t2645->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_40, _t2645);
#line 899 "compiler/main.gem"
    struct _closure__anon_41 *_t2896 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2896->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2896->gem_v_compile_expr = gem_v_compile_expr;
    _t2896->gem_v_compile_if = gem_v_compile_if;
    _t2896->gem_v_compile_match = gem_v_compile_match;
    _t2896->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t2896->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2896->gem_v_compile_while = gem_v_compile_while;
    _t2896->gem_v_in_top_level = gem_v_in_top_level;
    _t2896->gem_v_mangle = gem_v_mangle;
    _t2896->gem_v_source_name = gem_v_source_name;
    _t2896->gem_v_tmp = gem_v_tmp;
    _t2896->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t2896->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_41, _t2896);
#line 975 "compiler/main.gem"
    struct _closure__anon_42 *_t3121 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t3121->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3121->gem_v_compile_expr = gem_v_compile_expr;
    _t3121->gem_v_compile_if_return = gem_v_compile_if_return;
    _t3121->gem_v_compile_match_return = gem_v_compile_match_return;
    _t3121->gem_v_compile_receive_match_return = gem_v_compile_receive_match_return;
    _t3121->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3121->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3121->gem_v_compile_while = gem_v_compile_while;
    _t3121->gem_v_mangle = gem_v_mangle;
    _t3121->gem_v_source_name = gem_v_source_name;
    _t3121->gem_v_tmp = gem_v_tmp;
    _t3121->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_42, _t3121);
#line 1042 "compiler/main.gem"
    struct _closure__anon_43 *_t3140 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t3140->gem_v_compile_expr = gem_v_compile_expr;
    _t3140->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_43, _t3140);
#line 1058 "compiler/main.gem"
    struct _closure__anon_44 *_t3176 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t3176->gem_v_compile_expr = gem_v_compile_expr;
    _t3176->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3176->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_44, _t3176);
#line 1093 "compiler/main.gem"
    struct _closure__anon_45 *_t3195 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t3195->gem_v_compile_expr = gem_v_compile_expr;
    _t3195->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_45, _t3195);
#line 1109 "compiler/main.gem"
    struct _closure__anon_46 *_t3252 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t3252->gem_v_compile_expr = gem_v_compile_expr;
    _t3252->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3252->gem_v_compile_stmts = gem_v_compile_stmts;
    _t3252->gem_v_mangle = gem_v_mangle;
    _t3252->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_46, _t3252);
#line 1160 "compiler/main.gem"
    struct _closure__anon_47 *_t3335 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t3335->gem_v_compile_expr = gem_v_compile_expr;
    _t3335->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3335->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3335->gem_v_mangle = gem_v_mangle;
    _t3335->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_47, _t3335);
#line 1239 "compiler/main.gem"
    struct _closure__anon_48 *_t3454 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t3454->gem_v_compile_expr = gem_v_compile_expr;
    _t3454->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3454->gem_v_compile_stmts = gem_v_compile_stmts;
    _t3454->gem_v_mangle = gem_v_mangle;
    _t3454->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match = gem_make_fn(_anon_48, _t3454);
#line 1364 "compiler/main.gem"
    struct _closure__anon_49 *_t3591 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t3591->gem_v_compile_expr = gem_v_compile_expr;
    _t3591->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3591->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3591->gem_v_mangle = gem_v_mangle;
    _t3591->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match_return = gem_make_fn(_anon_49, _t3591);
#line 1492 "compiler/main.gem"
    struct _closure__anon_50 *_t3642 = GC_MALLOC(sizeof(struct _closure__anon_50));
    _t3642->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t3642->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_50, _t3642);
#line 1614 "compiler/main.gem"
    struct _closure__anon_51 *_t3683 = GC_MALLOC(sizeof(struct _closure__anon_51));
    _t3683->gem_v_mangle = gem_v_mangle;
    GemVal *gem_v_emit_param_bindings = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_param_bindings = gem_make_fn(_anon_51, _t3683);
#line 1658 "compiler/main.gem"
    struct _closure__anon_52 *_t3692 = GC_MALLOC(sizeof(struct _closure__anon_52));
    _t3692->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3692->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_fn_body = gem_make_fn(_anon_52, _t3692);
#line 1673 "compiler/main.gem"
    struct _closure__anon_53 *_t3724 = GC_MALLOC(sizeof(struct _closure__anon_53));
    _t3724->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3724->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t3724->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t3724->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3724->gem_v_in_top_level = gem_v_in_top_level;
    _t3724->gem_v_local_names = gem_v_local_names;
    _t3724->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_fn = gem_make_fn(_anon_53, _t3724);
#line 1722 "compiler/main.gem"
    struct _closure__anon_54 *_t3768 = GC_MALLOC(sizeof(struct _closure__anon_54));
    _t3768->gem_v_anon_name = gem_v_anon_name;
    _t3768->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3768->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t3768->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t3768->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3768->gem_v_functions = gem_v_functions;
    _t3768->gem_v_in_top_level = gem_v_in_top_level;
    _t3768->gem_v_local_names = gem_v_local_names;
    _t3768->gem_v_mangle = gem_v_mangle;
    _t3768->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_54, _t3768);
#line 1783 "compiler/main.gem"
    struct _closure__anon_55 *_t3828 = GC_MALLOC(sizeof(struct _closure__anon_55));
    _t3828->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3828->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t3828->gem_v_compile_fn = gem_v_compile_fn;
    _t3828->gem_v_compile_stmts = gem_v_compile_stmts;
    _t3828->gem_v_defined_fns = gem_v_defined_fns;
    _t3828->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t3828->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3828->gem_v_forward_decls = gem_v_forward_decls;
    _t3828->gem_v_functions = gem_v_functions;
    _t3828->gem_v_in_top_level = gem_v_in_top_level;
    _t3828->gem_v_local_names = gem_v_local_names;
    _t3828->gem_v_source_name = gem_v_source_name;
    _t3828->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t3828->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_55, _t3828);
    GemVal _t3829 = gem_table_new();
    gem_table_set(_t3829, gem_string("compile"), gem_v_compile);
    GemVal _t3830 = _t3829;
    gem_pop_frame();
    return _t3830;
}

static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc) {
#line 1920 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("str_replace", "compiler/main.gem", 1920);
#line 1921 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 1922 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 1923 "compiler/main.gem"
    GemVal _t3831[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t3831, 1);
#line 1924 "compiler/main.gem"
    GemVal _t3832[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t3832, 1);
#line 1925 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
#line 1926 "compiler/main.gem"
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
#line 1927 "compiler/main.gem"
            GemVal gem_v_found = gem_bool(1);
#line 1928 "compiler/main.gem"
            GemVal gem_v_j = gem_int(0);
#line 1929 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
#line 1930 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
#line 1931 "compiler/main.gem"
                    gem_v_found = gem_bool(0);
                    break;
                }
#line 1934 "compiler/main.gem"
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
#line 1936 "compiler/main.gem"
            if (gem_truthy(gem_v_found)) {
#line 1937 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
#line 1938 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
#line 1942 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
#line 1943 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t3833 = gem_v_result;
    gem_pop_frame();
    return _t3833;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 19 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 19);
#line 20 "compiler/main.gem"
    GemVal _t3834 = gem_table_new();
    GemVal gem_v_new_stmts = _t3834;
#line 21 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 22 "compiler/main.gem"
    GemVal _t3835 = gem_v_ast;
    GemVal gem_v__for_items_1 = gem_table_get(_t3835, gem_string("stmts"));
#line 22 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 22 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3836[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t3836, 1)))) break;
#line 22 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 22 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 23 "compiler/main.gem"
    GemVal _t3837[] = {gem_v_stmt};
    GemVal _t3839;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3837, 1), gem_string("table")))) {
        _t3839 = gem_eq(gem_type_fn(NULL, _t3837, 1), gem_string("table"));
    } else {
        GemVal _t3838 = gem_v_stmt;
        _t3839 = gem_eq(gem_table_get(_t3838, gem_string("tag")), gem_string("load"));
    }
        if (gem_truthy(_t3839)) {
#line 24 "compiler/main.gem"
    GemVal _t3840 = gem_v_stmt;
            GemVal gem_v_path = gem_table_get(_t3840, gem_string("path"));
#line 26 "compiler/main.gem"
    GemVal _t3841[] = {gem_v_path};
    GemVal _t3849;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t3841, 1), gem_int(4)))) {
        _t3849 = gem_lt(gem_len_fn(NULL, _t3841, 1), gem_int(4));
    } else {
        GemVal _t3842[] = {gem_v_path};
        GemVal _t3844;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3842, 1), gem_int(4))), gem_string(".")))) {
                _t3844 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3842, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t3843[] = {gem_v_path};
                _t3844 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3843, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t3846;
        if (gem_truthy(_t3844)) {
                _t3846 = _t3844;
        } else {
                GemVal _t3845[] = {gem_v_path};
                _t3846 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3845, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t3848;
        if (gem_truthy(_t3846)) {
                _t3848 = _t3846;
        } else {
                GemVal _t3847[] = {gem_v_path};
                _t3848 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t3847, 1), gem_int(1))), gem_string("m"));
        }
        _t3849 = _t3848;
    }
            if (gem_truthy(_t3849)) {
#line 27 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 31 "compiler/main.gem"
    GemVal _t3850[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_fn_gem_path_join(NULL, _t3850, 2);
#line 34 "compiler/main.gem"
    GemVal _t3851[] = {gem_v_full_path};
            if (gem_truthy(gem_eq(gem_fn_gem_file_exists(NULL, _t3851, 1), gem_int(0)))) {
#line 35 "compiler/main.gem"
    GemVal _t3852[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_fn_gem_path_join(NULL, _t3852, 2);
            }
#line 38 "compiler/main.gem"
    GemVal _t3853[] = {gem_v_full_path};
            gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t3853, 1);
#line 41 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                continue;
            }
#line 44 "compiler/main.gem"
            gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 46 "compiler/main.gem"
    GemVal _t3854[] = {gem_v_full_path};
            GemVal gem_v_source = gem_read_file_fn(NULL, _t3854, 1);
#line 48 "compiler/main.gem"
    GemVal _t3855[] = {gem_v_source, gem_v_full_path};
            GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t3855, 2);
#line 49 "compiler/main.gem"
    GemVal _t3856[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t3856, 1);
#line 50 "compiler/main.gem"
    GemVal _t3857[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t3857, 3);
#line 52 "compiler/main.gem"
    GemVal _t3858 = gem_v_resolved;
            GemVal gem_v__for_items_2 = gem_table_get(_t3858, gem_string("stmts"));
#line 52 "compiler/main.gem"
            GemVal gem_v__for_i_2 = gem_int(0);
#line 52 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3859[] = {gem_v__for_items_2};
                if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t3859, 1)))) break;
#line 52 "compiler/main.gem"
                GemVal gem_v_rs = gem_table_get(gem_v__for_items_2, gem_v__for_i_2);
#line 52 "compiler/main.gem"
                gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 53 "compiler/main.gem"
                gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_rs);
#line 54 "compiler/main.gem"
                gem_v_count = gem_add(gem_v_count, gem_int(1));
            }

        } else {
#line 57 "compiler/main.gem"
            gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_stmt);
#line 58 "compiler/main.gem"
            gem_v_count = gem_add(gem_v_count, gem_int(1));
        }
    }

#line 61 "compiler/main.gem"
    GemVal _t3860[] = {gem_v_new_stmts};
    GemVal _t3861 = gem_fn_make_program(NULL, _t3860, 1);
    gem_pop_frame();
    return _t3861;
}

int main(int argc, char **argv) {
    GC_INIT();
    gem_init(argc, argv);
    gem_scheduler_init();
    gem_push_frame("main", "compiler/main.gem", 0);
#line 15 "compiler/main.gem"
    GemVal _t3862[] = {gem_table_get(gem_argv_fn(NULL, NULL, 0), gem_int(0))};
    GemVal _t3863[] = {gem_fn_gem_dirname(NULL, _t3862, 1)};
    gem_v_install_root = gem_fn_gem_dirname(NULL, _t3863, 1);
#line 66 "compiler/main.gem"
    GemVal _t3864[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_argc = gem_len_fn(NULL, _t3864, 1);
#line 67 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 68 "compiler/main.gem"
    GemVal _t3865[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 68, _t3865, 1));
    }
#line 71 "compiler/main.gem"
    gem_v_src_path = gem_table_get(gem_argv_fn(NULL, NULL, 0), gem_int(1));
#line 72 "compiler/main.gem"
    GemVal _t3866[] = {gem_v_src_path};
    gem_v_source = gem_read_file_fn(NULL, _t3866, 1);
#line 74 "compiler/main.gem"
    GemVal _t3867[] = {gem_v_source, gem_v_src_path};
    gem_v_ast = gem_fn_parse_source(NULL, _t3867, 2);
#line 75 "compiler/main.gem"
    GemVal _t3868[] = {gem_v_src_path};
    gem_v_base_dir = gem_fn_gem_dirname(NULL, _t3868, 1);
#line 76 "compiler/main.gem"
    GemVal _t3869 = gem_table_new();
    gem_v_loaded = _t3869;
#line 77 "compiler/main.gem"
    GemVal _t3870[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t3870, 3);
#line 79 "compiler/main.gem"
    GemVal _t3871[] = {gem_v_src_path};
    gem_v_cg = gem_fn_make_codegen(NULL, _t3871, 1);
#line 80 "compiler/main.gem"
    GemVal _t3872 = gem_v_cg;
    GemVal _t3873[] = {gem_v_resolved_ast};
    GemVal _t3874 = gem_table_get(_t3872, gem_string("compile"));
    gem_v_c_code = _t3874.fn(_t3874.env, _t3873, 1);
#line 81 "compiler/main.gem"
    GemVal _t3875[] = {gem_v_c_code};
    (void)(gem_print(NULL, _t3875, 1));
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

