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
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t1 = gem_table_new();
    gem_table_set(_t1, gem_string("tag"), gem_string("program"));
    gem_table_set(_t1, gem_string("stmts"), gem_v_stmts);
    return _t1;
}

static GemVal gem_fn_make_int(void *_env, GemVal *args, int argc) {
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t2 = gem_table_new();
    gem_table_set(_t2, gem_string("tag"), gem_string("int"));
    gem_table_set(_t2, gem_string("value"), gem_v_value);
    return _t2;
}

static GemVal gem_fn_make_float(void *_env, GemVal *args, int argc) {
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t3 = gem_table_new();
    gem_table_set(_t3, gem_string("tag"), gem_string("float"));
    gem_table_set(_t3, gem_string("value"), gem_v_value);
    return _t3;
}

static GemVal gem_fn_make_string(void *_env, GemVal *args, int argc) {
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t4 = gem_table_new();
    gem_table_set(_t4, gem_string("tag"), gem_string("string"));
    gem_table_set(_t4, gem_string("value"), gem_v_value);
    return _t4;
}

static GemVal gem_fn_make_bool(void *_env, GemVal *args, int argc) {
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t5 = gem_table_new();
    gem_table_set(_t5, gem_string("tag"), gem_string("bool"));
    gem_table_set(_t5, gem_string("value"), gem_v_value);
    return _t5;
}

static GemVal gem_fn_make_nil_node(void *_env, GemVal *args, int argc) {
    GemVal _t6 = gem_table_new();
    gem_table_set(_t6, gem_string("tag"), gem_string("nil"));
    return _t6;
}

static GemVal gem_fn_make_var(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t7 = gem_table_new();
    gem_table_set(_t7, gem_string("tag"), gem_string("var"));
    gem_table_set(_t7, gem_string("name"), gem_v_name);
    return _t7;
}

static GemVal gem_fn_make_binop(void *_env, GemVal *args, int argc) {
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
    GemVal gem_v_op = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_expr = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t9 = gem_table_new();
    gem_table_set(_t9, gem_string("tag"), gem_string("unop"));
    gem_table_set(_t9, gem_string("op"), gem_v_op);
    gem_table_set(_t9, gem_string("expr"), gem_v_expr);
    return _t9;
}

static GemVal gem_fn_make_call(void *_env, GemVal *args, int argc) {
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
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t11 = gem_table_new();
    gem_table_set(_t11, gem_string("tag"), gem_string("dot"));
    gem_table_set(_t11, gem_string("object"), gem_v_object);
    gem_table_set(_t11, gem_string("field"), gem_v_field);
    return _t11;
}

static GemVal gem_fn_make_index(void *_env, GemVal *args, int argc) {
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t12 = gem_table_new();
    gem_table_set(_t12, gem_string("tag"), gem_string("index"));
    gem_table_set(_t12, gem_string("object"), gem_v_object);
    gem_table_set(_t12, gem_string("key"), gem_v_key);
    return _t12;
}

static GemVal gem_fn_make_anon_fn(void *_env, GemVal *args, int argc) {
    GemVal gem_v_params = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t13 = gem_table_new();
    gem_table_set(_t13, gem_string("tag"), gem_string("anon_fn"));
    gem_table_set(_t13, gem_string("params"), gem_v_params);
    gem_table_set(_t13, gem_string("body"), gem_v_body);
    return _t13;
}

static GemVal gem_fn_make_table(void *_env, GemVal *args, int argc) {
    GemVal gem_v_entries = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t14 = gem_table_new();
    gem_table_set(_t14, gem_string("tag"), gem_string("table"));
    gem_table_set(_t14, gem_string("entries"), gem_v_entries);
    return _t14;
}

static GemVal gem_fn_make_table_entry(void *_env, GemVal *args, int argc) {
    GemVal gem_v_key = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t15 = gem_table_new();
    gem_table_set(_t15, gem_string("key"), gem_v_key);
    gem_table_set(_t15, gem_string("value"), gem_v_value);
    return _t15;
}

static GemVal gem_fn_make_array(void *_env, GemVal *args, int argc) {
    GemVal gem_v_elements = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t16 = gem_table_new();
    gem_table_set(_t16, gem_string("tag"), gem_string("array"));
    gem_table_set(_t16, gem_string("elements"), gem_v_elements);
    return _t16;
}

static GemVal gem_fn_make_interp(void *_env, GemVal *args, int argc) {
    GemVal gem_v_parts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t17 = gem_table_new();
    gem_table_set(_t17, gem_string("tag"), gem_string("interp"));
    gem_table_set(_t17, gem_string("parts"), gem_v_parts);
    return _t17;
}

static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t18 = gem_table_new();
    gem_table_set(_t18, gem_string("tag"), gem_string("let"));
    gem_table_set(_t18, gem_string("name"), gem_v_name);
    gem_table_set(_t18, gem_string("value"), gem_v_value);
    return _t18;
}

static GemVal gem_fn_make_assign(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t19 = gem_table_new();
    gem_table_set(_t19, gem_string("tag"), gem_string("assign"));
    gem_table_set(_t19, gem_string("name"), gem_v_name);
    gem_table_set(_t19, gem_string("value"), gem_v_value);
    return _t19;
}

static GemVal gem_fn_make_dot_assign(void *_env, GemVal *args, int argc) {
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t20 = gem_table_new();
    gem_table_set(_t20, gem_string("tag"), gem_string("dot_assign"));
    gem_table_set(_t20, gem_string("object"), gem_v_object);
    gem_table_set(_t20, gem_string("field"), gem_v_field);
    gem_table_set(_t20, gem_string("value"), gem_v_value);
    return _t20;
}

static GemVal gem_fn_make_index_assign(void *_env, GemVal *args, int argc) {
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t21 = gem_table_new();
    gem_table_set(_t21, gem_string("tag"), gem_string("index_assign"));
    gem_table_set(_t21, gem_string("object"), gem_v_object);
    gem_table_set(_t21, gem_string("key"), gem_v_key);
    gem_table_set(_t21, gem_string("value"), gem_v_value);
    return _t21;
}

static GemVal gem_fn_make_fn_def(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t22 = gem_table_new();
    gem_table_set(_t22, gem_string("tag"), gem_string("fn_def"));
    gem_table_set(_t22, gem_string("name"), gem_v_name);
    gem_table_set(_t22, gem_string("params"), gem_v_params);
    gem_table_set(_t22, gem_string("body"), gem_v_body);
    return _t22;
}

static GemVal gem_fn_make_if(void *_env, GemVal *args, int argc) {
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_then_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t23 = gem_table_new();
    gem_table_set(_t23, gem_string("tag"), gem_string("if"));
    gem_table_set(_t23, gem_string("cond"), gem_v_cond);
    gem_table_set(_t23, gem_string("then"), gem_v_then_body);
    GemVal gem_v_node = _t23;
    gem_table_set(gem_v_node, gem_string("else"), gem_v_else_body);
    return gem_v_node;
}

static GemVal gem_fn_make_while(void *_env, GemVal *args, int argc) {
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t24 = gem_table_new();
    gem_table_set(_t24, gem_string("tag"), gem_string("while"));
    gem_table_set(_t24, gem_string("cond"), gem_v_cond);
    gem_table_set(_t24, gem_string("body"), gem_v_body);
    return _t24;
}

static GemVal gem_fn_make_match(void *_env, GemVal *args, int argc) {
    GemVal gem_v_target = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_whens = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t25 = gem_table_new();
    gem_table_set(_t25, gem_string("tag"), gem_string("match"));
    gem_table_set(_t25, gem_string("target"), gem_v_target);
    gem_table_set(_t25, gem_string("whens"), gem_v_whens);
    GemVal gem_v_node = _t25;
    gem_table_set(gem_v_node, gem_string("else"), gem_v_else_body);
    return gem_v_node;
}

static GemVal gem_fn_make_when(void *_env, GemVal *args, int argc) {
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t26 = gem_table_new();
    gem_table_set(_t26, gem_string("tag"), gem_string("when_clause"));
    gem_table_set(_t26, gem_string("value"), gem_v_value);
    gem_table_set(_t26, gem_string("body"), gem_v_body);
    return _t26;
}

static GemVal gem_fn_make_return(void *_env, GemVal *args, int argc) {
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t27 = gem_table_new();
    gem_table_set(_t27, gem_string("tag"), gem_string("return"));
    gem_table_set(_t27, gem_string("value"), gem_v_value);
    return _t27;
}

static GemVal gem_fn_make_break(void *_env, GemVal *args, int argc) {
    GemVal _t28 = gem_table_new();
    gem_table_set(_t28, gem_string("tag"), gem_string("break"));
    return _t28;
}

static GemVal gem_fn_make_continue(void *_env, GemVal *args, int argc) {
    GemVal _t29 = gem_table_new();
    gem_table_set(_t29, gem_string("tag"), gem_string("continue"));
    return _t29;
}

static GemVal gem_fn_make_load(void *_env, GemVal *args, int argc) {
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t30 = gem_table_new();
    gem_table_set(_t30, gem_string("tag"), gem_string("load"));
    gem_table_set(_t30, gem_string("path"), gem_v_path);
    return _t30;
}

static GemVal gem_fn_make_extern_fn(void *_env, GemVal *args, int argc) {
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
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_type_name = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t32 = gem_table_new();
    gem_table_set(_t32, gem_string("name"), gem_v_name);
    gem_table_set(_t32, gem_string("type"), gem_v_type_name);
    return _t32;
}

static GemVal gem_fn_make_extern_include(void *_env, GemVal *args, int argc) {
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t33 = gem_table_new();
    gem_table_set(_t33, gem_string("tag"), gem_string("extern_include"));
    gem_table_set(_t33, gem_string("path"), gem_v_path);
    return _t33;
}

static GemVal gem_fn_is_digit(void *_env, GemVal *args, int argc) {
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
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
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
    GemVal _t43[] = {gem_v_ch};
    GemVal _t44[] = {gem_add(gem_string("not a digit: "), gem_to_string_fn(NULL, _t43, 1))};
        return gem_error_at_fn("compiler/main.gem", 50, _t44, 1);
    }
}

static GemVal gem_fn_str_to_int(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = gem_int(0);
    {
        GemVal gem_v__for_i_1 = gem_int(0);
    GemVal _t45[] = {gem_v_s};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t45, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
            GemVal gem_v_i = gem_v__for_i_1;
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
    GemVal _t46[] = {gem_table_get(gem_v_s, gem_v_i)};
            gem_v_result = gem_add(gem_mul(gem_v_result, gem_int(10)), gem_fn_digit_val(NULL, _t46, 1));
        }
    }
    return gem_v_result;
}

static GemVal gem_fn_contains_dot(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    {
        GemVal gem_v__for_i_2 = gem_int(0);
    GemVal _t47[] = {gem_v_s};
        GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t47, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
            GemVal gem_v_i = gem_v__for_i_2;
            gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_string(".")))) {
                return gem_bool(1);
            }
        }
    }
    return gem_bool(0);
}

static GemVal gem_fn_build_keyword_set(void *_env, GemVal *args, int argc) {
    GemVal _t48 = gem_table_new();
    GemVal gem_v_t = _t48;
    gem_table_set(gem_v_t, gem_string("fn"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("end"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("let"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("if"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("elif"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("else"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("while"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("return"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("break"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("continue"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("do"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("match"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("when"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("and"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("or"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("not"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("true"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("false"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("nil"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("load"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("extern"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("for"), gem_bool(1));
    gem_table_set(gem_v_t, gem_string("in"), gem_bool(1));
    return gem_v_t;
}

static GemVal gem_fn_tokenize(void *_env, GemVal *args, int argc) {
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_kw = gem_fn_build_keyword_set(NULL, NULL, 0);
    GemVal _t49 = gem_table_new();
    GemVal gem_v_tokens = _t49;
    GemVal gem_v_pos = gem_int(0);
    GemVal gem_v_line = gem_int(1);
    GemVal _t50[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t50, 1);
    GemVal gem_v_interp_depth = gem_int(0);
    GemVal _t51 = gem_table_new();
    GemVal gem_v_interp_brace = _t51;
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
    GemVal _t52;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t52 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t52 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t52)) {
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
            while (1) {
                GemVal _t53;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t53 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t53 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t53)) break;
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
    GemVal _t54;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t54 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t54 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t54)) {
            GemVal gem_v_should_add = gem_bool(1);
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
                gem_v_should_add = gem_bool(0);
            }
            if (gem_truthy(gem_v_should_add)) {
    GemVal _t59 = gem_table_new();
    gem_table_set(_t59, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t59, gem_string("value"), gem_string("\n"));
    gem_table_set(_t59, gem_string("line"), gem_v_line);
    GemVal _t60[] = {gem_v_tokens, _t59};
                (void)(gem_push_fn(NULL, _t60, 2));
            }
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
                gem_v_line = gem_add(gem_v_line, gem_int(1));
            }
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
                GemVal gem_v_c2 = gem_table_get(gem_v_source, gem_v_pos);
                if (gem_truthy(gem_eq(gem_v_c2, gem_string("\n")))) {
                    gem_v_line = gem_add(gem_v_line, gem_int(1));
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                } else {
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
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
                            while (1) {
                                GemVal _t63;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t63 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t63 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t63)) break;
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
    GemVal _t64[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t64, 1))) {
            GemVal gem_v_val = gem_string("");
            while (1) {
                GemVal _t66;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t66 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t65[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t66 = gem_fn_is_digit(NULL, _t65, 1);
                }
                if (!gem_truthy(_t66)) break;
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
    GemVal _t67;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t67 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t67 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t67;
            if (gem_truthy(gem_v_has_dot)) {
                gem_v_val = gem_add(gem_v_val, gem_string("."));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                while (1) {
                    GemVal _t69;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t69 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t68[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t69 = gem_fn_is_digit(NULL, _t68, 1);
                    }
                    if (!gem_truthy(_t69)) break;
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
    GemVal _t70 = gem_table_new();
    gem_table_set(_t70, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t70, gem_string("value"), gem_v_val);
    gem_table_set(_t70, gem_string("line"), gem_v_line);
    GemVal _t71[] = {gem_v_tokens, _t70};
            (void)(gem_push_fn(NULL, _t71, 2));
            continue;
        }
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            GemVal gem_v_val = gem_string("");
            GemVal gem_v_hit_interp = gem_bool(0);
            while (1) {
                GemVal _t72;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t72 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t72 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t72)) break;
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
    GemVal _t73[] = {gem_v_line};
    GemVal _t74[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t73, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 190, _t74, 1));
                    }
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
                            gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                        } else {
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
                                gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                            } else {
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
                                    gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
                                        gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
                                            gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                        } else {
                                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
    GemVal _t75 = gem_table_new();
    gem_table_set(_t75, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t75, gem_string("value"), gem_string(""));
    gem_table_set(_t75, gem_string("line"), gem_v_line);
    GemVal _t76[] = {gem_v_tokens, _t75};
                    (void)(gem_push_fn(NULL, _t76, 2));
    GemVal _t77 = gem_table_new();
    gem_table_set(_t77, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t77, gem_string("value"), gem_v_val);
    gem_table_set(_t77, gem_string("line"), gem_v_line);
    GemVal _t78[] = {gem_v_tokens, _t77};
                    (void)(gem_push_fn(NULL, _t78, 2));
                    gem_v_val = gem_string("");
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
    GemVal _t79[] = {gem_v_line};
    GemVal _t80[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t79, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 226, _t80, 1));
                }
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
    GemVal _t81 = gem_table_new();
    gem_table_set(_t81, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t81, gem_string("value"), gem_v_val);
    gem_table_set(_t81, gem_string("line"), gem_v_line);
    GemVal _t82[] = {gem_v_tokens, _t81};
                (void)(gem_push_fn(NULL, _t82, 2));
            }
            continue;
        }
    GemVal _t83[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t83, 1))) {
            GemVal gem_v_val = gem_string("");
            while (1) {
                GemVal _t85;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t85 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t84[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t85 = gem_fn_is_alnum(NULL, _t84, 1);
                }
                if (!gem_truthy(_t85)) break;
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            if (gem_truthy(gem_eq(gem_table_get(gem_v_kw, gem_v_val), gem_bool(1)))) {
    GemVal _t86 = gem_table_new();
    gem_table_set(_t86, gem_string("type"), gem_v_val);
    gem_table_set(_t86, gem_string("value"), gem_v_val);
    gem_table_set(_t86, gem_string("line"), gem_v_line);
    GemVal _t87[] = {gem_v_tokens, _t86};
                (void)(gem_push_fn(NULL, _t87, 2));
            } else {
    GemVal _t88 = gem_table_new();
    gem_table_set(_t88, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t88, gem_string("value"), gem_v_val);
    gem_table_set(_t88, gem_string("line"), gem_v_line);
    GemVal _t89[] = {gem_v_tokens, _t88};
                (void)(gem_push_fn(NULL, _t89, 2));
            }
            continue;
        }
        GemVal gem_v_next = gem_string("");
        if (gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
            gem_v_next = gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1)));
        }
    GemVal _t90;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t90 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t90 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t90)) {
    GemVal _t91 = gem_table_new();
    gem_table_set(_t91, gem_string("type"), gem_string("=="));
    gem_table_set(_t91, gem_string("value"), gem_string("=="));
    gem_table_set(_t91, gem_string("line"), gem_v_line);
    GemVal _t92[] = {gem_v_tokens, _t91};
            (void)(gem_push_fn(NULL, _t92, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t93;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t93 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t93 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t93)) {
    GemVal _t94 = gem_table_new();
    gem_table_set(_t94, gem_string("type"), gem_string("!="));
    gem_table_set(_t94, gem_string("value"), gem_string("!="));
    gem_table_set(_t94, gem_string("line"), gem_v_line);
    GemVal _t95[] = {gem_v_tokens, _t94};
            (void)(gem_push_fn(NULL, _t95, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t96;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t96 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t96 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t96)) {
    GemVal _t97 = gem_table_new();
    gem_table_set(_t97, gem_string("type"), gem_string("<="));
    gem_table_set(_t97, gem_string("value"), gem_string("<="));
    gem_table_set(_t97, gem_string("line"), gem_v_line);
    GemVal _t98[] = {gem_v_tokens, _t97};
            (void)(gem_push_fn(NULL, _t98, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t99;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t99 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t99 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t99)) {
    GemVal _t100 = gem_table_new();
    gem_table_set(_t100, gem_string("type"), gem_string(">="));
    gem_table_set(_t100, gem_string("value"), gem_string(">="));
    gem_table_set(_t100, gem_string("line"), gem_v_line);
    GemVal _t101[] = {gem_v_tokens, _t100};
            (void)(gem_push_fn(NULL, _t101, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t102;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t102 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t102 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t102)) {
    GemVal _t103 = gem_table_new();
    gem_table_set(_t103, gem_string("type"), gem_string("+="));
    gem_table_set(_t103, gem_string("value"), gem_string("+="));
    gem_table_set(_t103, gem_string("line"), gem_v_line);
    GemVal _t104[] = {gem_v_tokens, _t103};
            (void)(gem_push_fn(NULL, _t104, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t105;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t105 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t105 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t105)) {
    GemVal _t106 = gem_table_new();
    gem_table_set(_t106, gem_string("type"), gem_string("-="));
    gem_table_set(_t106, gem_string("value"), gem_string("-="));
    gem_table_set(_t106, gem_string("line"), gem_v_line);
    GemVal _t107[] = {gem_v_tokens, _t106};
            (void)(gem_push_fn(NULL, _t107, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t108;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t108 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t108 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t108)) {
    GemVal _t109 = gem_table_new();
    gem_table_set(_t109, gem_string("type"), gem_string("*="));
    gem_table_set(_t109, gem_string("value"), gem_string("*="));
    gem_table_set(_t109, gem_string("line"), gem_v_line);
    GemVal _t110[] = {gem_v_tokens, _t109};
            (void)(gem_push_fn(NULL, _t110, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t111;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t111 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t111 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t111)) {
    GemVal _t112 = gem_table_new();
    gem_table_set(_t112, gem_string("type"), gem_string("/="));
    gem_table_set(_t112, gem_string("value"), gem_string("/="));
    gem_table_set(_t112, gem_string("line"), gem_v_line);
    GemVal _t113[] = {gem_v_tokens, _t112};
            (void)(gem_push_fn(NULL, _t113, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t114;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t114 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t114 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t114)) {
    GemVal _t115 = gem_table_new();
    gem_table_set(_t115, gem_string("type"), gem_string("->"));
    gem_table_set(_t115, gem_string("value"), gem_string("->"));
    gem_table_set(_t115, gem_string("line"), gem_v_line);
    GemVal _t116[] = {gem_v_tokens, _t115};
            (void)(gem_push_fn(NULL, _t116, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
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
    GemVal _t121 = gem_table_new();
    gem_table_set(_t121, gem_string("type"), gem_v_ch);
    gem_table_set(_t121, gem_string("value"), gem_v_ch);
    gem_table_set(_t121, gem_string("line"), gem_v_line);
    GemVal _t122[] = {gem_v_tokens, _t121};
            (void)(gem_push_fn(NULL, _t122, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
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
    GemVal _t125 = gem_table_new();
    gem_table_set(_t125, gem_string("type"), gem_v_ch);
    gem_table_set(_t125, gem_string("value"), gem_v_ch);
    gem_table_set(_t125, gem_string("line"), gem_v_line);
    GemVal _t126[] = {gem_v_tokens, _t125};
            (void)(gem_push_fn(NULL, _t126, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
    GemVal _t127;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t127 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t127 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t127)) {
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
    GemVal _t128 = gem_table_new();
    gem_table_set(_t128, gem_string("type"), gem_string("{"));
    gem_table_set(_t128, gem_string("value"), gem_string("{"));
    gem_table_set(_t128, gem_string("line"), gem_v_line);
    GemVal _t129[] = {gem_v_tokens, _t128};
            (void)(gem_push_fn(NULL, _t129, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
    GemVal _t130;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t130 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t130 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t130)) {
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
    GemVal _t131 = gem_table_new();
    gem_table_set(_t131, gem_string("type"), gem_string("}"));
    gem_table_set(_t131, gem_string("value"), gem_string("}"));
    gem_table_set(_t131, gem_string("line"), gem_v_line);
    GemVal _t132[] = {gem_v_tokens, _t131};
                (void)(gem_push_fn(NULL, _t132, 2));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                continue;
            }
            gem_v_interp_depth = gem_sub(gem_v_interp_depth, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            GemVal gem_v_val = gem_string("");
            GemVal gem_v_hit_interp = gem_bool(0);
            while (1) {
                GemVal _t133;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t133 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t133 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t133)) break;
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
    GemVal _t134[] = {gem_v_line};
    GemVal _t135[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t134, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 337, _t135, 1));
                    }
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
                            gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                        } else {
                            if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
                                gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                            } else {
                                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
                                    gem_v_val = gem_add(gem_v_val, gem_string("\""));
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
                                        gem_v_val = gem_add(gem_v_val, gem_string("{"));
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
                                            gem_v_val = gem_add(gem_v_val, gem_string("}"));
                                        } else {
                                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
    GemVal _t136 = gem_table_new();
    gem_table_set(_t136, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t136, gem_string("value"), gem_v_val);
    gem_table_set(_t136, gem_string("line"), gem_v_line);
    GemVal _t137[] = {gem_v_tokens, _t136};
                    (void)(gem_push_fn(NULL, _t137, 2));
                    gem_v_val = gem_string("");
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
    GemVal _t138[] = {gem_v_line};
    GemVal _t139[] = {gem_add(gem_string("unterminated string interpolation at line "), gem_to_string_fn(NULL, _t138, 1))};
                    (void)(gem_error_at_fn("compiler/main.gem", 373, _t139, 1));
                }
    GemVal _t140 = gem_table_new();
    gem_table_set(_t140, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t140, gem_string("value"), gem_v_val);
    gem_table_set(_t140, gem_string("line"), gem_v_line);
    GemVal _t141[] = {gem_v_tokens, _t140};
                (void)(gem_push_fn(NULL, _t141, 2));
    GemVal _t142 = gem_table_new();
    gem_table_set(_t142, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t142, gem_string("value"), gem_string(""));
    gem_table_set(_t142, gem_string("line"), gem_v_line);
    GemVal _t143[] = {gem_v_tokens, _t142};
                (void)(gem_push_fn(NULL, _t143, 2));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
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
    GemVal _t149 = gem_table_new();
    gem_table_set(_t149, gem_string("type"), gem_v_ch);
    gem_table_set(_t149, gem_string("value"), gem_v_ch);
    gem_table_set(_t149, gem_string("line"), gem_v_line);
    GemVal _t150[] = {gem_v_tokens, _t149};
            (void)(gem_push_fn(NULL, _t150, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
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
    GemVal _t154 = gem_table_new();
    gem_table_set(_t154, gem_string("type"), gem_v_ch);
    gem_table_set(_t154, gem_string("value"), gem_v_ch);
    gem_table_set(_t154, gem_string("line"), gem_v_line);
    GemVal _t155[] = {gem_v_tokens, _t154};
            (void)(gem_push_fn(NULL, _t155, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
    GemVal _t156[] = {gem_v_ch};
    GemVal _t157[] = {gem_v_line};
    GemVal _t158[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t156, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t157, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 393, _t158, 1));
    }
    GemVal _t159 = gem_table_new();
    gem_table_set(_t159, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t159, gem_string("value"), gem_string(""));
    gem_table_set(_t159, gem_string("line"), gem_v_line);
    GemVal _t160[] = {gem_v_tokens, _t159};
    (void)(gem_push_fn(NULL, _t160, 2));
    return gem_v_tokens;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t161[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t161, 1);
    GemVal _t162[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t162, 1);
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
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
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
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t170 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t170, gem_string("type")), gem_v_tp))) {
    GemVal _t171[] = {gem_v_tp};
    GemVal _t172 = gem_v_t;
    GemVal _t173[] = {gem_table_get(_t172, gem_string("type"))};
    GemVal _t174 = gem_v_t;
    GemVal _t175[] = {gem_table_get(_t174, gem_string("line"))};
    GemVal _t176[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t171, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t173, 1)), gem_string("' at line ")), gem_to_string_fn(NULL, _t175, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 45, _t176, 1));
    }
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
    while (1) {
        GemVal _t178 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t178, gem_string("type")), gem_string("NEWLINE")))) break;
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
    GemVal _t180 = gem_table_new();
    GemVal gem_v_params = _t180;
    GemVal _t181 = (*gem_v_peek);
    GemVal _t182 = _t181.fn(_t181.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t182, gem_string("type")), gem_string("NAME")))) {
    GemVal _t183 = (*gem_v_advance);
    GemVal _t184 = _t183.fn(_t183.env, NULL, 0);
    GemVal _t185[] = {gem_v_params, gem_table_get(_t184, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t185, 2));
        while (1) {
            GemVal _t186 = (*gem_v_peek);
            GemVal _t187 = _t186.fn(_t186.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t187, gem_string("type")), gem_string(",")))) break;
    GemVal _t188 = (*gem_v_advance);
            (void)(_t188.fn(_t188.env, NULL, 0));
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
    GemVal _t194 = gem_table_new();
    GemVal gem_v_stmts = _t194;
    GemVal _t195 = (*gem_v_skip_nl);
    (void)(_t195.fn(_t195.env, NULL, 0));
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
    GemVal _t209 = (*gem_v_parse_stmt);
    GemVal _t210[] = {gem_v_stmts, _t209.fn(_t209.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t210, 2));
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
    GemVal _t213 = gem_table_new();
    GemVal gem_v_stmts = _t213;
    GemVal _t214 = (*gem_v_skip_nl);
    (void)(_t214.fn(_t214.env, NULL, 0));
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
    GemVal _t219 = (*gem_v_parse_stmt);
    GemVal _t220[] = {gem_v_stmts, _t219.fn(_t219.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t220, 2));
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
    GemVal _t223 = (*gem_v_peek);
    GemVal gem_v_t = _t223.fn(_t223.env, NULL, 0);
    GemVal _t224 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t224, gem_string("type")), gem_string("NUMBER")))) {
    GemVal _t225 = (*gem_v_advance);
        (void)(_t225.fn(_t225.env, NULL, 0));
    GemVal _t226 = gem_v_t;
    GemVal _t227[] = {gem_table_get(_t226, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t227, 1))) {
    GemVal _t228 = gem_v_t;
    GemVal _t229[] = {gem_table_get(_t228, gem_string("value"))};
    GemVal _t230[] = {gem_fn_atof(NULL, _t229, 1)};
            return gem_fn_make_float(NULL, _t230, 1);
        }
    GemVal _t231 = gem_v_t;
    GemVal _t232[] = {gem_table_get(_t231, gem_string("value"))};
    GemVal _t233[] = {gem_fn_str_to_int(NULL, _t232, 1)};
        return gem_fn_make_int(NULL, _t233, 1);
    }
    GemVal _t234 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t234, gem_string("type")), gem_string("STRING")))) {
    GemVal _t235 = (*gem_v_advance);
        (void)(_t235.fn(_t235.env, NULL, 0));
    GemVal _t236 = gem_v_t;
    GemVal _t237[] = {gem_table_get(_t236, gem_string("value"))};
        return gem_fn_make_string(NULL, _t237, 1);
    }
    GemVal _t238 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t238, gem_string("type")), gem_string("INTERP_START")))) {
    GemVal _t239 = (*gem_v_advance);
        (void)(_t239.fn(_t239.env, NULL, 0));
    GemVal _t240 = gem_table_new();
        GemVal gem_v_parts = _t240;
    GemVal _t241 = (*gem_v_skip_nl);
        (void)(_t241.fn(_t241.env, NULL, 0));
        while (1) {
            GemVal _t242 = (*gem_v_peek);
            GemVal _t243 = _t242.fn(_t242.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t243, gem_string("type")), gem_string("INTERP_END")))) break;
    GemVal _t244 = (*gem_v_peek);
    GemVal _t245 = _t244.fn(_t244.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t245, gem_string("type")), gem_string("STRING")))) {
    GemVal _t246 = (*gem_v_advance);
    GemVal _t247 = _t246.fn(_t246.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t247, gem_string("value"));
    GemVal _t248[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t248, 1), gem_int(0)))) {
    GemVal _t249[] = {gem_v_sval};
    GemVal _t250[] = {gem_v_parts, gem_fn_make_string(NULL, _t249, 1)};
                    (void)(gem_push_fn(NULL, _t250, 2));
                }
            } else {
    GemVal _t251 = (*gem_v_parse_expr);
    GemVal _t252[] = {gem_v_parts, _t251.fn(_t251.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t252, 2));
            }
    GemVal _t253 = (*gem_v_skip_nl);
            (void)(_t253.fn(_t253.env, NULL, 0));
        }
    GemVal _t254 = (*gem_v_expect);
    GemVal _t255[] = {gem_string("INTERP_END")};
        (void)(_t254.fn(_t254.env, _t255, 1));
    GemVal _t256[] = {gem_v_parts};
        return gem_fn_make_interp(NULL, _t256, 1);
    }
    GemVal _t257 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t257, gem_string("type")), gem_string("true")))) {
    GemVal _t258 = (*gem_v_advance);
        (void)(_t258.fn(_t258.env, NULL, 0));
    GemVal _t259[] = {gem_bool(1)};
        return gem_fn_make_bool(NULL, _t259, 1);
    }
    GemVal _t260 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t260, gem_string("type")), gem_string("false")))) {
    GemVal _t261 = (*gem_v_advance);
        (void)(_t261.fn(_t261.env, NULL, 0));
    GemVal _t262[] = {gem_bool(0)};
        return gem_fn_make_bool(NULL, _t262, 1);
    }
    GemVal _t263 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t263, gem_string("type")), gem_string("nil")))) {
    GemVal _t264 = (*gem_v_advance);
        (void)(_t264.fn(_t264.env, NULL, 0));
        return gem_fn_make_nil_node(NULL, NULL, 0);
    }
    GemVal _t265 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t265, gem_string("type")), gem_string("fn")))) {
    GemVal _t266 = (*gem_v_advance);
        (void)(_t266.fn(_t266.env, NULL, 0));
    GemVal _t267 = (*gem_v_expect);
    GemVal _t268[] = {gem_string("(")};
        (void)(_t267.fn(_t267.env, _t268, 1));
    GemVal _t269 = (*gem_v_parse_params);
        GemVal gem_v_params = _t269.fn(_t269.env, NULL, 0);
    GemVal _t270 = (*gem_v_expect);
    GemVal _t271[] = {gem_string(")")};
        (void)(_t270.fn(_t270.env, _t271, 1));
    GemVal _t272 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t272.fn(_t272.env, NULL, 0);
    GemVal _t273 = (*gem_v_expect);
    GemVal _t274[] = {gem_string("end")};
        (void)(_t273.fn(_t273.env, _t274, 1));
    GemVal _t275[] = {gem_v_params, gem_v_body};
        return gem_fn_make_anon_fn(NULL, _t275, 2);
    }
    GemVal _t276 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t276, gem_string("type")), gem_string("{")))) {
    GemVal _t277 = (*gem_v_advance);
        (void)(_t277.fn(_t277.env, NULL, 0));
    GemVal _t278 = (*gem_v_skip_nl);
        (void)(_t278.fn(_t278.env, NULL, 0));
    GemVal _t279 = gem_table_new();
        GemVal gem_v_entries = _t279;
        while (1) {
            GemVal _t280 = (*gem_v_peek);
            GemVal _t281 = _t280.fn(_t280.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t281, gem_string("type")), gem_string("}")))) break;
    GemVal _t282 = (*gem_v_expect);
    GemVal _t283[] = {gem_string("NAME")};
    GemVal _t284 = _t282.fn(_t282.env, _t283, 1);
            GemVal gem_v_key = gem_table_get(_t284, gem_string("value"));
    GemVal _t285 = (*gem_v_expect);
    GemVal _t286[] = {gem_string(":")};
            (void)(_t285.fn(_t285.env, _t286, 1));
    GemVal _t287 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t287.fn(_t287.env, NULL, 0);
    GemVal _t288[] = {gem_v_key, gem_v_val};
    GemVal _t289[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t288, 2)};
            (void)(gem_push_fn(NULL, _t289, 2));
    GemVal _t290 = (*gem_v_skip_nl);
            (void)(_t290.fn(_t290.env, NULL, 0));
    GemVal _t291 = (*gem_v_peek);
    GemVal _t292 = _t291.fn(_t291.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t292, gem_string("type")), gem_string(",")))) {
    GemVal _t293 = (*gem_v_advance);
                (void)(_t293.fn(_t293.env, NULL, 0));
    GemVal _t294 = (*gem_v_skip_nl);
                (void)(_t294.fn(_t294.env, NULL, 0));
            }
        }
    GemVal _t295 = (*gem_v_expect);
    GemVal _t296[] = {gem_string("}")};
        (void)(_t295.fn(_t295.env, _t296, 1));
    GemVal _t297[] = {gem_v_entries};
        return gem_fn_make_table(NULL, _t297, 1);
    }
    GemVal _t298 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t298, gem_string("type")), gem_string("[")))) {
    GemVal _t299 = (*gem_v_advance);
        (void)(_t299.fn(_t299.env, NULL, 0));
    GemVal _t300 = (*gem_v_skip_nl);
        (void)(_t300.fn(_t300.env, NULL, 0));
    GemVal _t301 = gem_table_new();
        GemVal gem_v_elements = _t301;
        while (1) {
            GemVal _t302 = (*gem_v_peek);
            GemVal _t303 = _t302.fn(_t302.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t303, gem_string("type")), gem_string("]")))) break;
    GemVal _t304 = (*gem_v_parse_expr);
    GemVal _t305[] = {gem_v_elements, _t304.fn(_t304.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t305, 2));
    GemVal _t306 = (*gem_v_skip_nl);
            (void)(_t306.fn(_t306.env, NULL, 0));
    GemVal _t307 = (*gem_v_peek);
    GemVal _t308 = _t307.fn(_t307.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t308, gem_string("type")), gem_string(",")))) {
    GemVal _t309 = (*gem_v_advance);
                (void)(_t309.fn(_t309.env, NULL, 0));
    GemVal _t310 = (*gem_v_skip_nl);
                (void)(_t310.fn(_t310.env, NULL, 0));
            }
        }
    GemVal _t311 = (*gem_v_expect);
    GemVal _t312[] = {gem_string("]")};
        (void)(_t311.fn(_t311.env, _t312, 1));
    GemVal _t313[] = {gem_v_elements};
        return gem_fn_make_array(NULL, _t313, 1);
    }
    GemVal _t314 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t314, gem_string("type")), gem_string("(")))) {
    GemVal _t315 = (*gem_v_advance);
        (void)(_t315.fn(_t315.env, NULL, 0));
    GemVal _t316 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t316.fn(_t316.env, NULL, 0);
    GemVal _t317 = (*gem_v_expect);
    GemVal _t318[] = {gem_string(")")};
        (void)(_t317.fn(_t317.env, _t318, 1));
        return gem_v_e;
    }
    GemVal _t319 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t319, gem_string("type")), gem_string("NAME")))) {
    GemVal _t320 = (*gem_v_advance);
        (void)(_t320.fn(_t320.env, NULL, 0));
    GemVal _t321 = gem_v_t;
    GemVal _t322[] = {gem_table_get(_t321, gem_string("value"))};
        return gem_fn_make_var(NULL, _t322, 1);
    }
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
    GemVal _t329 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t329.fn(_t329.env, NULL, 0);
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
    GemVal _t330 = (*gem_v_peek);
    GemVal _t331 = _t330.fn(_t330.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t331, gem_string("type")), gem_string("(")))) {
    GemVal _t332 = (*gem_v_peek);
    GemVal _t333 = _t332.fn(_t332.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t333, gem_string("line"));
    GemVal _t334 = (*gem_v_advance);
            (void)(_t334.fn(_t334.env, NULL, 0));
    GemVal _t335 = (*gem_v_skip_nl);
            (void)(_t335.fn(_t335.env, NULL, 0));
    GemVal _t336 = gem_table_new();
            GemVal gem_v_args = _t336;
    GemVal _t337 = (*gem_v_peek);
    GemVal _t338 = _t337.fn(_t337.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t338, gem_string("type")), gem_string(")")))) {
    GemVal _t339 = (*gem_v_parse_expr);
    GemVal _t340[] = {gem_v_args, _t339.fn(_t339.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t340, 2));
                while (1) {
                    GemVal _t341 = (*gem_v_peek);
                    GemVal _t342 = _t341.fn(_t341.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t342, gem_string("type")), gem_string(",")))) break;
    GemVal _t343 = (*gem_v_advance);
                    (void)(_t343.fn(_t343.env, NULL, 0));
    GemVal _t344 = (*gem_v_skip_nl);
                    (void)(_t344.fn(_t344.env, NULL, 0));
    GemVal _t345 = (*gem_v_parse_expr);
    GemVal _t346[] = {gem_v_args, _t345.fn(_t345.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t346, 2));
                }
            }
    GemVal _t347 = (*gem_v_skip_nl);
            (void)(_t347.fn(_t347.env, NULL, 0));
    GemVal _t348 = (*gem_v_expect);
    GemVal _t349[] = {gem_string(")")};
            (void)(_t348.fn(_t348.env, _t349, 1));
    GemVal _t350 = (*gem_v_peek);
    GemVal _t351 = _t350.fn(_t350.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t351, gem_string("type")), gem_string("do")))) {
    GemVal _t352 = (*gem_v_advance);
                (void)(_t352.fn(_t352.env, NULL, 0));
    GemVal _t353 = gem_table_new();
                GemVal gem_v_bparams = _t353;
    GemVal _t354 = (*gem_v_peek);
    GemVal _t355 = _t354.fn(_t354.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t355, gem_string("type")), gem_string("|")))) {
    GemVal _t356 = (*gem_v_advance);
                    (void)(_t356.fn(_t356.env, NULL, 0));
    GemVal _t357 = (*gem_v_parse_params);
                    gem_v_bparams = _t357.fn(_t357.env, NULL, 0);
    GemVal _t358 = (*gem_v_expect);
    GemVal _t359[] = {gem_string("|")};
                    (void)(_t358.fn(_t358.env, _t359, 1));
                }
    GemVal _t360 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t360.fn(_t360.env, NULL, 0);
    GemVal _t361 = (*gem_v_expect);
    GemVal _t362[] = {gem_string("end")};
                (void)(_t361.fn(_t361.env, _t362, 1));
    GemVal _t363[] = {gem_v_bparams, gem_v_body};
    GemVal _t364[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t363, 2)};
                (void)(gem_push_fn(NULL, _t364, 2));
            } else {
    GemVal _t365 = (*gem_v_peek);
    GemVal _t366 = _t365.fn(_t365.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t366, gem_string("type")), gem_string("{")))) {
                    GemVal gem_v_is_block = gem_bool(0);
    GemVal _t367 = (*gem_v_peek_at);
    GemVal _t368[] = {gem_int(1)};
    GemVal _t369 = _t367.fn(_t367.env, _t368, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t369, gem_string("type")), gem_string("|")))) {
                        gem_v_is_block = gem_bool(1);
                    } else {
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
                            gem_v_is_block = gem_bool(0);
                        } else {
                            gem_v_is_block = gem_bool(1);
                        }
                    }
                    if (gem_truthy(gem_v_is_block)) {
    GemVal _t381 = (*gem_v_advance);
                        (void)(_t381.fn(_t381.env, NULL, 0));
    GemVal _t382 = gem_table_new();
                        GemVal gem_v_bparams = _t382;
    GemVal _t383 = (*gem_v_peek);
    GemVal _t384 = _t383.fn(_t383.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t384, gem_string("type")), gem_string("|")))) {
    GemVal _t385 = (*gem_v_advance);
                            (void)(_t385.fn(_t385.env, NULL, 0));
    GemVal _t386 = (*gem_v_parse_params);
                            gem_v_bparams = _t386.fn(_t386.env, NULL, 0);
    GemVal _t387 = (*gem_v_expect);
    GemVal _t388[] = {gem_string("|")};
                            (void)(_t387.fn(_t387.env, _t388, 1));
                        }
    GemVal _t389 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t389.fn(_t389.env, NULL, 0);
    GemVal _t390 = (*gem_v_expect);
    GemVal _t391[] = {gem_string("}")};
                        (void)(_t390.fn(_t390.env, _t391, 1));
    GemVal _t392 = gem_table_new();
    gem_table_set(_t392, gem_int(0), gem_v_bexpr);
    GemVal _t393[] = {gem_v_bparams, _t392};
    GemVal _t394[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t393, 2)};
                        (void)(gem_push_fn(NULL, _t394, 2));
                    }
                }
            }
    GemVal _t395[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t395, 3);
            continue;
        }
    GemVal _t396 = (*gem_v_peek);
    GemVal _t397 = _t396.fn(_t396.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t397, gem_string("type")), gem_string(".")))) {
    GemVal _t398 = (*gem_v_advance);
            (void)(_t398.fn(_t398.env, NULL, 0));
    GemVal _t399 = (*gem_v_expect);
    GemVal _t400[] = {gem_string("NAME")};
    GemVal _t401 = _t399.fn(_t399.env, _t400, 1);
            GemVal gem_v_field = gem_table_get(_t401, gem_string("value"));
    GemVal _t402[] = {gem_v_node, gem_v_field};
            gem_v_node = gem_fn_make_dot(NULL, _t402, 2);
            continue;
        }
    GemVal _t403 = (*gem_v_peek);
    GemVal _t404 = _t403.fn(_t403.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t404, gem_string("type")), gem_string("[")))) {
    GemVal _t405 = (*gem_v_advance);
            (void)(_t405.fn(_t405.env, NULL, 0));
    GemVal _t406 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t406.fn(_t406.env, NULL, 0);
    GemVal _t407 = (*gem_v_expect);
    GemVal _t408[] = {gem_string("]")};
            (void)(_t407.fn(_t407.env, _t408, 1));
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
    GemVal _t411 = (*gem_v_peek);
    GemVal _t412 = _t411.fn(_t411.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t412, gem_string("type")), gem_string("-")))) {
    GemVal _t413 = (*gem_v_advance);
        (void)(_t413.fn(_t413.env, NULL, 0));
    GemVal _t414 = (*gem_v_parse_unary);
    GemVal _t415[] = {gem_string("-"), _t414.fn(_t414.env, NULL, 0)};
        return gem_fn_make_unop(NULL, _t415, 2);
    }
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
    GemVal _t418 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t418.fn(_t418.env, NULL, 0);
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
    GemVal _t427 = (*gem_v_advance);
    GemVal _t428 = _t427.fn(_t427.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t428, gem_string("type"));
    GemVal _t429 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t429.fn(_t429.env, NULL, 0);
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
    GemVal _t432 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t432.fn(_t432.env, NULL, 0);
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
    GemVal _t438 = (*gem_v_advance);
    GemVal _t439 = _t438.fn(_t438.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t439, gem_string("type"));
    GemVal _t440 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t440.fn(_t440.env, NULL, 0);
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
    GemVal _t443 = (*gem_v_parse_add);
    GemVal gem_v_left = _t443.fn(_t443.env, NULL, 0);
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
    GemVal _t461 = (*gem_v_advance);
    GemVal _t462 = _t461.fn(_t461.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t462, gem_string("type"));
    GemVal _t463 = (*gem_v_parse_add);
        GemVal gem_v_right = _t463.fn(_t463.env, NULL, 0);
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
    GemVal _t466 = (*gem_v_peek);
    GemVal _t467 = _t466.fn(_t466.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t467, gem_string("type")), gem_string("not")))) {
    GemVal _t468 = (*gem_v_advance);
        (void)(_t468.fn(_t468.env, NULL, 0));
    GemVal _t469 = (*gem_v_parse_not);
    GemVal _t470[] = {gem_string("not"), _t469.fn(_t469.env, NULL, 0)};
        return gem_fn_make_unop(NULL, _t470, 2);
    }
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
    GemVal _t473 = (*gem_v_parse_not);
    GemVal gem_v_left = _t473.fn(_t473.env, NULL, 0);
    while (1) {
        GemVal _t474 = (*gem_v_peek);
        GemVal _t475 = _t474.fn(_t474.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t475, gem_string("type")), gem_string("and")))) break;
    GemVal _t476 = (*gem_v_advance);
        (void)(_t476.fn(_t476.env, NULL, 0));
    GemVal _t477 = (*gem_v_parse_not);
        GemVal gem_v_right = _t477.fn(_t477.env, NULL, 0);
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
    GemVal _t480 = (*gem_v_parse_and);
    GemVal gem_v_left = _t480.fn(_t480.env, NULL, 0);
    while (1) {
        GemVal _t481 = (*gem_v_peek);
        GemVal _t482 = _t481.fn(_t481.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t482, gem_string("type")), gem_string("or")))) break;
    GemVal _t483 = (*gem_v_advance);
        (void)(_t483.fn(_t483.env, NULL, 0));
    GemVal _t484 = (*gem_v_parse_and);
        GemVal gem_v_right = _t484.fn(_t484.env, NULL, 0);
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
    GemVal _t487 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t487.fn(_t487.env, NULL, 0);
    GemVal _t488 = (*gem_v_peek);
    GemVal _t489 = _t488.fn(_t488.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t489, gem_string("type")), gem_string("=")))) {
    GemVal _t490 = (*gem_v_advance);
        (void)(_t490.fn(_t490.env, NULL, 0));
    GemVal _t491 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t491.fn(_t491.env, NULL, 0);
    GemVal _t492 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t492, gem_string("tag")), gem_string("var")))) {
    GemVal _t493 = gem_v_lhs;
    GemVal _t494[] = {gem_table_get(_t493, gem_string("name")), gem_v_value};
            return gem_fn_make_assign(NULL, _t494, 2);
        }
    GemVal _t495 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t495, gem_string("tag")), gem_string("dot")))) {
    GemVal _t496 = gem_v_lhs;
    GemVal _t497 = gem_v_lhs;
    GemVal _t498[] = {gem_table_get(_t496, gem_string("object")), gem_table_get(_t497, gem_string("field")), gem_v_value};
            return gem_fn_make_dot_assign(NULL, _t498, 3);
        }
    GemVal _t499 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t499, gem_string("tag")), gem_string("index")))) {
    GemVal _t500 = gem_v_lhs;
    GemVal _t501 = gem_v_lhs;
    GemVal _t502[] = {gem_table_get(_t500, gem_string("object")), gem_table_get(_t501, gem_string("key")), gem_v_value};
            return gem_fn_make_index_assign(NULL, _t502, 3);
        }
    GemVal _t503 = (*gem_v_peek);
    GemVal _t504 = _t503.fn(_t503.env, NULL, 0);
    GemVal _t505[] = {gem_table_get(_t504, gem_string("line"))};
    GemVal _t506[] = {gem_add(gem_string("invalid assignment target at line "), gem_to_string_fn(NULL, _t505, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 405, _t506, 1));
    }
    GemVal _t507 = (*gem_v_peek);
    GemVal _t508 = _t507.fn(_t507.env, NULL, 0);
    GemVal _t511;
    if (gem_truthy(gem_eq(gem_table_get(_t508, gem_string("type")), gem_string("+=")))) {
        _t511 = gem_eq(gem_table_get(_t508, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t509 = (*gem_v_peek);
        GemVal _t510 = _t509.fn(_t509.env, NULL, 0);
        _t511 = gem_eq(gem_table_get(_t510, gem_string("type")), gem_string("-="));
    }
    GemVal _t514;
    if (gem_truthy(_t511)) {
        _t514 = _t511;
    } else {
        GemVal _t512 = (*gem_v_peek);
        GemVal _t513 = _t512.fn(_t512.env, NULL, 0);
        _t514 = gem_eq(gem_table_get(_t513, gem_string("type")), gem_string("*="));
    }
    GemVal _t517;
    if (gem_truthy(_t514)) {
        _t517 = _t514;
    } else {
        GemVal _t515 = (*gem_v_peek);
        GemVal _t516 = _t515.fn(_t515.env, NULL, 0);
        _t517 = gem_eq(gem_table_get(_t516, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t517)) {
    GemVal _t518 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t518.fn(_t518.env, NULL, 0);
    GemVal _t519 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t519, gem_string("value")), gem_int(0));
    GemVal _t520 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t520.fn(_t520.env, NULL, 0);
    GemVal _t521 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t521, gem_string("tag")), gem_string("var")))) {
    GemVal _t522 = gem_v_op_tok;
    GemVal _t523[] = {gem_table_get(_t522, gem_string("line"))};
    GemVal _t524[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t523, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 414, _t524, 1));
        }
    GemVal _t525 = gem_v_lhs;
    GemVal _t526 = gem_v_lhs;
    GemVal _t527[] = {gem_table_get(_t526, gem_string("name"))};
    GemVal _t528[] = {gem_v_base_op, gem_fn_make_var(NULL, _t527, 1), gem_v_value};
    GemVal _t529[] = {gem_table_get(_t525, gem_string("name")), gem_fn_make_binop(NULL, _t528, 3)};
        return gem_fn_make_assign(NULL, _t529, 2);
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
    GemVal _t531 = (*gem_v_peek);
    GemVal gem_v_t = _t531.fn(_t531.env, NULL, 0);
    GemVal _t532 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t532, gem_string("type")), gem_string("let")))) {
    GemVal _t533 = (*gem_v_advance);
        (void)(_t533.fn(_t533.env, NULL, 0));
    GemVal _t534 = (*gem_v_expect);
    GemVal _t535[] = {gem_string("NAME")};
    GemVal _t536 = _t534.fn(_t534.env, _t535, 1);
        GemVal gem_v_name = gem_table_get(_t536, gem_string("value"));
    GemVal _t537 = (*gem_v_expect);
    GemVal _t538[] = {gem_string("=")};
        (void)(_t537.fn(_t537.env, _t538, 1));
    GemVal _t539 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t539.fn(_t539.env, NULL, 0);
    GemVal _t540[] = {gem_v_name, gem_v_value};
        return gem_fn_make_let(NULL, _t540, 2);
    }
    GemVal _t541 = gem_v_t;
    GemVal _t545;
    if (!gem_truthy(gem_eq(gem_table_get(_t541, gem_string("type")), gem_string("fn")))) {
        _t545 = gem_eq(gem_table_get(_t541, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t542 = (*gem_v_peek_at);
        GemVal _t543[] = {gem_int(1)};
        GemVal _t544 = _t542.fn(_t542.env, _t543, 1);
        _t545 = gem_eq(gem_table_get(_t544, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t545)) {
    GemVal _t546 = (*gem_v_advance);
        (void)(_t546.fn(_t546.env, NULL, 0));
    GemVal _t547 = (*gem_v_expect);
    GemVal _t548[] = {gem_string("NAME")};
    GemVal _t549 = _t547.fn(_t547.env, _t548, 1);
        GemVal gem_v_name = gem_table_get(_t549, gem_string("value"));
    GemVal _t550 = (*gem_v_expect);
    GemVal _t551[] = {gem_string("(")};
        (void)(_t550.fn(_t550.env, _t551, 1));
    GemVal _t552 = (*gem_v_parse_params);
        GemVal gem_v_params = _t552.fn(_t552.env, NULL, 0);
    GemVal _t553 = (*gem_v_expect);
    GemVal _t554[] = {gem_string(")")};
        (void)(_t553.fn(_t553.env, _t554, 1));
    GemVal _t555 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t555.fn(_t555.env, NULL, 0);
    GemVal _t556 = (*gem_v_expect);
    GemVal _t557[] = {gem_string("end")};
        (void)(_t556.fn(_t556.env, _t557, 1));
    GemVal _t558[] = {gem_v_name, gem_v_params, gem_v_body};
        return gem_fn_make_fn_def(NULL, _t558, 3);
    }
    GemVal _t559 = gem_v_t;
    GemVal _t561;
    if (gem_truthy(gem_eq(gem_table_get(_t559, gem_string("type")), gem_string("if")))) {
        _t561 = gem_eq(gem_table_get(_t559, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t560 = gem_v_t;
        _t561 = gem_eq(gem_table_get(_t560, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t561)) {
    GemVal _t562 = (*gem_v_advance);
        (void)(_t562.fn(_t562.env, NULL, 0));
    GemVal _t563 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t563.fn(_t563.env, NULL, 0);
    GemVal _t564 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t564.fn(_t564.env, NULL, 0);
        GemVal gem_v_else_body = GEM_NIL;
    GemVal _t565 = (*gem_v_peek);
    GemVal _t566 = _t565.fn(_t565.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t566, gem_string("type")), gem_string("elif")))) {
    GemVal _t567 = gem_table_new();
    GemVal _t568 = (*gem_v_parse_stmt);
    gem_table_set(_t567, gem_int(0), _t568.fn(_t568.env, NULL, 0));
            gem_v_else_body = _t567;
        } else {
    GemVal _t569 = (*gem_v_peek);
    GemVal _t570 = _t569.fn(_t569.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t570, gem_string("type")), gem_string("else")))) {
    GemVal _t571 = (*gem_v_advance);
                (void)(_t571.fn(_t571.env, NULL, 0));
    GemVal _t572 = (*gem_v_parse_body);
                gem_v_else_body = _t572.fn(_t572.env, NULL, 0);
            }
        }
    GemVal _t573 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t573, gem_string("type")), gem_string("if")))) {
    GemVal _t574 = (*gem_v_expect);
    GemVal _t575[] = {gem_string("end")};
            (void)(_t574.fn(_t574.env, _t575, 1));
        }
    GemVal _t576[] = {gem_v_cond, gem_v_then_body, gem_v_else_body};
        return gem_fn_make_if(NULL, _t576, 3);
    }
    GemVal _t577 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t577, gem_string("type")), gem_string("while")))) {
    GemVal _t578 = (*gem_v_advance);
        (void)(_t578.fn(_t578.env, NULL, 0));
    GemVal _t579 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t579.fn(_t579.env, NULL, 0);
    GemVal _t580 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t580.fn(_t580.env, NULL, 0);
    GemVal _t581 = (*gem_v_expect);
    GemVal _t582[] = {gem_string("end")};
        (void)(_t581.fn(_t581.env, _t582, 1));
    GemVal _t583[] = {gem_v_cond, gem_v_wbody};
        return gem_fn_make_while(NULL, _t583, 2);
    }
    GemVal _t584 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t584, gem_string("type")), gem_string("for")))) {
    GemVal _t585 = (*gem_v_advance);
        (void)(_t585.fn(_t585.env, NULL, 0));
    GemVal _t586 = (*gem_v_expect);
    GemVal _t587[] = {gem_string("NAME")};
    GemVal _t588 = _t586.fn(_t586.env, _t587, 1);
        GemVal gem_v_var_name = gem_table_get(_t588, gem_string("value"));
    GemVal _t589 = (*gem_v_peek);
    GemVal _t590 = _t589.fn(_t589.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t590, gem_string("type")), gem_string("in")))) {
    GemVal _t591 = (*gem_v_advance);
            (void)(_t591.fn(_t591.env, NULL, 0));
    GemVal _t592 = (*gem_v_parse_expr);
            GemVal gem_v_items_expr = _t592.fn(_t592.env, NULL, 0);
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
    GemVal _t593[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t593, 1));
    GemVal _t594[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t594, 1));
    GemVal _t595 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t595.fn(_t595.env, NULL, 0);
    GemVal _t596 = (*gem_v_expect);
    GemVal _t597[] = {gem_string("end")};
            (void)(_t596.fn(_t596.env, _t597, 1));
    GemVal _t598 = gem_table_new();
            GemVal gem_v_inner_stmts = _t598;
    GemVal _t599[] = {gem_v_items_var};
    GemVal _t600[] = {gem_v_idx_var};
    GemVal _t601[] = {gem_fn_make_var(NULL, _t599, 1), gem_fn_make_var(NULL, _t600, 1)};
    GemVal _t602[] = {gem_v_var_name, gem_fn_make_index(NULL, _t601, 2)};
    GemVal _t603[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t602, 2)};
            (void)(gem_push_fn(NULL, _t603, 2));
    GemVal _t604[] = {gem_v_idx_var};
    GemVal _t605[] = {gem_int(1)};
    GemVal _t606[] = {gem_string("+"), gem_fn_make_var(NULL, _t604, 1), gem_fn_make_int(NULL, _t605, 1)};
    GemVal _t607[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t606, 3)};
    GemVal _t608[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t607, 2)};
            (void)(gem_push_fn(NULL, _t608, 2));
            {
                GemVal gem_v__for_i_1 = gem_int(0);
    GemVal _t609[] = {gem_v_fbody};
                GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t609, 1);
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
                    GemVal gem_v_bi = gem_v__for_i_1;
                    gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
    GemVal _t610[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t610, 2));
                }
            }
    GemVal _t611[] = {gem_v_idx_var};
    GemVal _t612[] = {gem_string("len")};
    GemVal _t613 = gem_table_new();
    GemVal _t614[] = {gem_v_items_var};
    gem_table_set(_t613, gem_int(0), gem_fn_make_var(NULL, _t614, 1));
    GemVal _t615[] = {gem_fn_make_var(NULL, _t612, 1), _t613, gem_int(0)};
    GemVal _t616[] = {gem_string("<"), gem_fn_make_var(NULL, _t611, 1), gem_fn_make_call(NULL, _t615, 3)};
    GemVal _t617[] = {gem_fn_make_binop(NULL, _t616, 3), gem_v_inner_stmts};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t617, 2);
    GemVal _t618 = gem_table_new();
    gem_table_set(_t618, gem_string("tag"), gem_string("block"));
    GemVal _t619 = gem_table_new();
    GemVal _t620[] = {gem_v_items_var, gem_v_items_expr};
    gem_table_set(_t619, gem_int(0), gem_fn_make_let(NULL, _t620, 2));
    GemVal _t621[] = {gem_int(0)};
    GemVal _t622[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t621, 1)};
    gem_table_set(_t619, gem_int(1), gem_fn_make_let(NULL, _t622, 2));
    gem_table_set(_t619, gem_int(2), gem_v_while_node);
    gem_table_set(_t618, gem_string("stmts"), _t619);
            return _t618;
        } else {
    GemVal _t623 = (*gem_v_peek);
    GemVal _t624 = _t623.fn(_t623.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t624, gem_string("type")), gem_string("=")))) {
    GemVal _t625 = (*gem_v_advance);
                (void)(_t625.fn(_t625.env, NULL, 0));
    GemVal _t626 = (*gem_v_parse_expr);
                GemVal gem_v_start_expr = _t626.fn(_t626.env, NULL, 0);
    GemVal _t627 = (*gem_v_expect);
    GemVal _t628[] = {gem_string(",")};
                (void)(_t627.fn(_t627.env, _t628, 1));
    GemVal _t629 = (*gem_v_parse_expr);
                GemVal gem_v_end_expr = _t629.fn(_t629.env, NULL, 0);
    GemVal _t630 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t630.fn(_t630.env, NULL, 0);
    GemVal _t631 = (*gem_v_expect);
    GemVal _t632[] = {gem_string("end")};
                (void)(_t631.fn(_t631.env, _t632, 1));
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
    GemVal _t633[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t633, 1));
    GemVal _t634[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t634, 1));
    GemVal _t635 = gem_table_new();
                GemVal gem_v_inner_stmts = _t635;
    GemVal _t636[] = {gem_v_idx_var};
    GemVal _t637[] = {gem_v_var_name, gem_fn_make_var(NULL, _t636, 1)};
    GemVal _t638[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t637, 2)};
                (void)(gem_push_fn(NULL, _t638, 2));
    GemVal _t639[] = {gem_v_idx_var};
    GemVal _t640[] = {gem_int(1)};
    GemVal _t641[] = {gem_string("+"), gem_fn_make_var(NULL, _t639, 1), gem_fn_make_int(NULL, _t640, 1)};
    GemVal _t642[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t641, 3)};
    GemVal _t643[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t642, 2)};
                (void)(gem_push_fn(NULL, _t643, 2));
                {
                    GemVal gem_v__for_i_2 = gem_int(0);
    GemVal _t644[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t644, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
                        GemVal gem_v_bi = gem_v__for_i_2;
                        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
    GemVal _t645[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t645, 2));
                    }
                }
    GemVal _t646[] = {gem_v_idx_var};
    GemVal _t647[] = {gem_v_limit_var};
    GemVal _t648[] = {gem_string("<"), gem_fn_make_var(NULL, _t646, 1), gem_fn_make_var(NULL, _t647, 1)};
    GemVal _t649[] = {gem_fn_make_binop(NULL, _t648, 3), gem_v_inner_stmts};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t649, 2);
    GemVal _t650 = gem_table_new();
    gem_table_set(_t650, gem_string("tag"), gem_string("block"));
    GemVal _t651 = gem_table_new();
    GemVal _t652[] = {gem_v_idx_var, gem_v_start_expr};
    gem_table_set(_t651, gem_int(0), gem_fn_make_let(NULL, _t652, 2));
    GemVal _t653[] = {gem_v_limit_var, gem_v_end_expr};
    gem_table_set(_t651, gem_int(1), gem_fn_make_let(NULL, _t653, 2));
    gem_table_set(_t651, gem_int(2), gem_v_while_node);
    gem_table_set(_t650, gem_string("stmts"), _t651);
                return _t650;
            } else {
    GemVal _t654 = (*gem_v_peek);
    GemVal _t655 = _t654.fn(_t654.env, NULL, 0);
    GemVal _t656[] = {gem_table_get(_t655, gem_string("line"))};
    GemVal _t657[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t656, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 536, _t657, 1));
            }
        }
    }
    GemVal _t658 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t658, gem_string("type")), gem_string("match")))) {
    GemVal _t659 = (*gem_v_advance);
        (void)(_t659.fn(_t659.env, NULL, 0));
    GemVal _t660 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t660.fn(_t660.env, NULL, 0);
    GemVal _t661 = (*gem_v_skip_nl);
        (void)(_t661.fn(_t661.env, NULL, 0));
    GemVal _t662 = gem_table_new();
        GemVal gem_v_whens = _t662;
        while (1) {
            GemVal _t663 = (*gem_v_peek);
            GemVal _t664 = _t663.fn(_t663.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t664, gem_string("type")), gem_string("when")))) break;
    GemVal _t665 = (*gem_v_advance);
            (void)(_t665.fn(_t665.env, NULL, 0));
    GemVal _t666 = (*gem_v_parse_expr);
            GemVal gem_v_wval = _t666.fn(_t666.env, NULL, 0);
    GemVal _t667 = (*gem_v_parse_body);
            GemVal gem_v_wbody = _t667.fn(_t667.env, NULL, 0);
    GemVal _t668[] = {gem_v_wval, gem_v_wbody};
    GemVal _t669[] = {gem_v_whens, gem_fn_make_when(NULL, _t668, 2)};
            (void)(gem_push_fn(NULL, _t669, 2));
    GemVal _t670 = (*gem_v_skip_nl);
            (void)(_t670.fn(_t670.env, NULL, 0));
        }
        GemVal gem_v_else_body = GEM_NIL;
    GemVal _t671 = (*gem_v_peek);
    GemVal _t672 = _t671.fn(_t671.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t672, gem_string("type")), gem_string("else")))) {
    GemVal _t673 = (*gem_v_advance);
            (void)(_t673.fn(_t673.env, NULL, 0));
    GemVal _t674 = (*gem_v_parse_body);
            gem_v_else_body = _t674.fn(_t674.env, NULL, 0);
        }
    GemVal _t675 = (*gem_v_expect);
    GemVal _t676[] = {gem_string("end")};
        (void)(_t675.fn(_t675.env, _t676, 1));
    GemVal _t677[] = {gem_v_target, gem_v_whens, gem_v_else_body};
        return gem_fn_make_match(NULL, _t677, 3);
    }
    GemVal _t678 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t678, gem_string("type")), gem_string("return")))) {
    GemVal _t679 = (*gem_v_advance);
        (void)(_t679.fn(_t679.env, NULL, 0));
        GemVal gem_v_value = GEM_NIL;
    GemVal _t680 = (*gem_v_peek);
    GemVal _t681 = _t680.fn(_t680.env, NULL, 0);
    GemVal _t684;
    if (!gem_truthy(gem_neq(gem_table_get(_t681, gem_string("type")), gem_string("NEWLINE")))) {
        _t684 = gem_neq(gem_table_get(_t681, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t682 = (*gem_v_peek);
        GemVal _t683 = _t682.fn(_t682.env, NULL, 0);
        _t684 = gem_neq(gem_table_get(_t683, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t687;
    if (!gem_truthy(_t684)) {
        _t687 = _t684;
    } else {
        GemVal _t685 = (*gem_v_peek);
        GemVal _t686 = _t685.fn(_t685.env, NULL, 0);
        _t687 = gem_neq(gem_table_get(_t686, gem_string("type")), gem_string("end"));
    }
    GemVal _t690;
    if (!gem_truthy(_t687)) {
        _t690 = _t687;
    } else {
        GemVal _t688 = (*gem_v_peek);
        GemVal _t689 = _t688.fn(_t688.env, NULL, 0);
        _t690 = gem_neq(gem_table_get(_t689, gem_string("type")), gem_string("elif"));
    }
    GemVal _t693;
    if (!gem_truthy(_t690)) {
        _t693 = _t690;
    } else {
        GemVal _t691 = (*gem_v_peek);
        GemVal _t692 = _t691.fn(_t691.env, NULL, 0);
        _t693 = gem_neq(gem_table_get(_t692, gem_string("type")), gem_string("else"));
    }
    GemVal _t696;
    if (!gem_truthy(_t693)) {
        _t696 = _t693;
    } else {
        GemVal _t694 = (*gem_v_peek);
        GemVal _t695 = _t694.fn(_t694.env, NULL, 0);
        _t696 = gem_neq(gem_table_get(_t695, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t696)) {
    GemVal _t697 = (*gem_v_parse_expr);
            gem_v_value = _t697.fn(_t697.env, NULL, 0);
        }
    GemVal _t698[] = {gem_v_value};
        return gem_fn_make_return(NULL, _t698, 1);
    }
    GemVal _t699 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t699, gem_string("type")), gem_string("break")))) {
    GemVal _t700 = (*gem_v_advance);
        (void)(_t700.fn(_t700.env, NULL, 0));
        return gem_fn_make_break(NULL, NULL, 0);
    }
    GemVal _t701 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t701, gem_string("type")), gem_string("continue")))) {
    GemVal _t702 = (*gem_v_advance);
        (void)(_t702.fn(_t702.env, NULL, 0));
        return gem_fn_make_continue(NULL, NULL, 0);
    }
    GemVal _t703 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t703, gem_string("type")), gem_string("load")))) {
    GemVal _t704 = (*gem_v_advance);
        (void)(_t704.fn(_t704.env, NULL, 0));
    GemVal _t705 = (*gem_v_expect);
    GemVal _t706[] = {gem_string("STRING")};
    GemVal _t707 = _t705.fn(_t705.env, _t706, 1);
        GemVal gem_v_path = gem_table_get(_t707, gem_string("value"));
    GemVal _t708[] = {gem_v_path};
        return gem_fn_make_load(NULL, _t708, 1);
    }
    GemVal _t709 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t709, gem_string("type")), gem_string("extern")))) {
    GemVal _t710 = (*gem_v_advance);
        (void)(_t710.fn(_t710.env, NULL, 0));
    GemVal _t711 = (*gem_v_peek);
    GemVal _t712 = _t711.fn(_t711.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t712, gem_string("type")), gem_string("fn")))) {
    GemVal _t713 = (*gem_v_advance);
            (void)(_t713.fn(_t713.env, NULL, 0));
    GemVal _t714 = (*gem_v_expect);
    GemVal _t715[] = {gem_string("NAME")};
    GemVal _t716 = _t714.fn(_t714.env, _t715, 1);
            GemVal gem_v_name = gem_table_get(_t716, gem_string("value"));
    GemVal _t717 = (*gem_v_expect);
    GemVal _t718[] = {gem_string("(")};
            (void)(_t717.fn(_t717.env, _t718, 1));
    GemVal _t719 = gem_table_new();
            GemVal gem_v_eparams = _t719;
    GemVal _t720 = (*gem_v_peek);
    GemVal _t721 = _t720.fn(_t720.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t721, gem_string("type")), gem_string(")")))) {
    GemVal _t722 = (*gem_v_expect);
    GemVal _t723[] = {gem_string("NAME")};
    GemVal _t724 = _t722.fn(_t722.env, _t723, 1);
                GemVal gem_v_pname = gem_table_get(_t724, gem_string("value"));
    GemVal _t725 = (*gem_v_expect);
    GemVal _t726[] = {gem_string(":")};
                (void)(_t725.fn(_t725.env, _t726, 1));
    GemVal _t727 = (*gem_v_expect);
    GemVal _t728[] = {gem_string("NAME")};
    GemVal _t729 = _t727.fn(_t727.env, _t728, 1);
                GemVal gem_v_ptype = gem_table_get(_t729, gem_string("value"));
    GemVal _t730[] = {gem_v_pname, gem_v_ptype};
    GemVal _t731[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t730, 2)};
                (void)(gem_push_fn(NULL, _t731, 2));
                while (1) {
                    GemVal _t732 = (*gem_v_peek);
                    GemVal _t733 = _t732.fn(_t732.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t733, gem_string("type")), gem_string(",")))) break;
    GemVal _t734 = (*gem_v_advance);
                    (void)(_t734.fn(_t734.env, NULL, 0));
    GemVal _t735 = (*gem_v_expect);
    GemVal _t736[] = {gem_string("NAME")};
    GemVal _t737 = _t735.fn(_t735.env, _t736, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t737, gem_string("value"));
    GemVal _t738 = (*gem_v_expect);
    GemVal _t739[] = {gem_string(":")};
                    (void)(_t738.fn(_t738.env, _t739, 1));
    GemVal _t740 = (*gem_v_expect);
    GemVal _t741[] = {gem_string("NAME")};
    GemVal _t742 = _t740.fn(_t740.env, _t741, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t742, gem_string("value"));
    GemVal _t743[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t744[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t743, 2)};
                    (void)(gem_push_fn(NULL, _t744, 2));
                }
            }
    GemVal _t745 = (*gem_v_expect);
    GemVal _t746[] = {gem_string(")")};
            (void)(_t745.fn(_t745.env, _t746, 1));
            GemVal gem_v_ret_type = gem_string("Nil");
    GemVal _t747 = (*gem_v_peek);
    GemVal _t748 = _t747.fn(_t747.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t748, gem_string("type")), gem_string("->")))) {
    GemVal _t749 = (*gem_v_advance);
                (void)(_t749.fn(_t749.env, NULL, 0));
    GemVal _t750 = (*gem_v_expect);
    GemVal _t751[] = {gem_string("NAME")};
    GemVal _t752 = _t750.fn(_t750.env, _t751, 1);
                gem_v_ret_type = gem_table_get(_t752, gem_string("value"));
            }
    GemVal _t753[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            return gem_fn_make_extern_fn(NULL, _t753, 3);
        } else {
    GemVal _t754 = (*gem_v_peek);
    GemVal _t755 = _t754.fn(_t754.env, NULL, 0);
    GemVal _t758;
    if (!gem_truthy(gem_eq(gem_table_get(_t755, gem_string("type")), gem_string("NAME")))) {
        _t758 = gem_eq(gem_table_get(_t755, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t756 = (*gem_v_peek);
        GemVal _t757 = _t756.fn(_t756.env, NULL, 0);
        _t758 = gem_eq(gem_table_get(_t757, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t758)) {
    GemVal _t759 = (*gem_v_advance);
                (void)(_t759.fn(_t759.env, NULL, 0));
    GemVal _t760 = (*gem_v_expect);
    GemVal _t761[] = {gem_string("STRING")};
    GemVal _t762 = _t760.fn(_t760.env, _t761, 1);
                GemVal gem_v_path = gem_table_get(_t762, gem_string("value"));
    GemVal _t763[] = {gem_v_path};
                return gem_fn_make_extern_include(NULL, _t763, 1);
            } else {
    GemVal _t764 = (*gem_v_peek);
    GemVal _t765 = _t764.fn(_t764.env, NULL, 0);
    GemVal _t766[] = {gem_table_get(_t765, gem_string("line"))};
    GemVal _t767[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t766, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 624, _t767, 1));
            }
        }
    }
    GemVal _t768 = (*gem_v_parse_expr);
    return _t768.fn(_t768.env, NULL, 0);
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
    GemVal _t770 = gem_table_new();
    GemVal gem_v_stmts = _t770;
    GemVal _t771 = (*gem_v_skip_nl);
    (void)(_t771.fn(_t771.env, NULL, 0));
    while (1) {
        GemVal _t772 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t772.fn(_t772.env, NULL, 0)))) break;
    GemVal _t773 = (*gem_v_parse_stmt);
    GemVal _t774[] = {gem_v_stmts, _t773.fn(_t773.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t774, 2));
    GemVal _t775 = (*gem_v_skip_nl);
        (void)(_t775.fn(_t775.env, NULL, 0));
    }
    GemVal _t776[] = {gem_v_stmts};
    return gem_fn_make_program(NULL, _t776, 1);
}

static GemVal gem_fn_make_parser(void *_env, GemVal *args, int argc) {
    GemVal *gem_v_tokens = GC_MALLOC(sizeof(GemVal));
    *gem_v_tokens = (argc > 0) ? args[0] : GEM_NIL;
    GemVal *gem_v_pos = GC_MALLOC(sizeof(GemVal));
    *gem_v_pos = gem_int(0);
    GemVal *gem_v_gensym_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_gensym_counter = gem_int(0);
    struct _closure__anon_1 *_t165 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t165->gem_v_pos = gem_v_pos;
    _t165->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t165);
    struct _closure__anon_2 *_t166 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t166->gem_v_pos = gem_v_pos;
    _t166->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t166);
    struct _closure__anon_3 *_t167 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t167->gem_v_pos = gem_v_pos;
    _t167->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t167);
    struct _closure__anon_4 *_t169 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t169->gem_v_pos = gem_v_pos;
    _t169->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t169);
    struct _closure__anon_5 *_t177 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t177->gem_v_pos = gem_v_pos;
    _t177->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t177);
    struct _closure__anon_6 *_t179 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t179->gem_v_pos = gem_v_pos;
    _t179->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t179);
    struct _closure__anon_7 *_t193 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t193->gem_v_advance = gem_v_advance;
    _t193->gem_v_expect = gem_v_expect;
    _t193->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t193);
    GemVal *gem_v_parse_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_expr = GEM_NIL;
    GemVal *gem_v_parse_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_stmt = GEM_NIL;
    GemVal *gem_v_parse_unary = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_unary = GEM_NIL;
    GemVal *gem_v_parse_not = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_not = GEM_NIL;
    struct _closure__anon_8 *_t212 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t212->gem_v_at_end = gem_v_at_end;
    _t212->gem_v_parse_stmt = gem_v_parse_stmt;
    _t212->gem_v_peek = gem_v_peek;
    _t212->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t212);
    struct _closure__anon_9 *_t222 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t222->gem_v_at_end = gem_v_at_end;
    _t222->gem_v_parse_stmt = gem_v_parse_stmt;
    _t222->gem_v_peek = gem_v_peek;
    _t222->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t222);
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
    struct _closure__anon_12 *_t417 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t417->gem_v_advance = gem_v_advance;
    _t417->gem_v_parse_call = gem_v_parse_call;
    _t417->gem_v_parse_unary = gem_v_parse_unary;
    _t417->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t417);
    struct _closure__anon_13 *_t431 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t431->gem_v_advance = gem_v_advance;
    _t431->gem_v_parse_unary = gem_v_parse_unary;
    _t431->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t431);
    struct _closure__anon_14 *_t442 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t442->gem_v_advance = gem_v_advance;
    _t442->gem_v_parse_mul = gem_v_parse_mul;
    _t442->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t442);
    struct _closure__anon_15 *_t465 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t465->gem_v_advance = gem_v_advance;
    _t465->gem_v_parse_add = gem_v_parse_add;
    _t465->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t465);
    struct _closure__anon_16 *_t472 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t472->gem_v_advance = gem_v_advance;
    _t472->gem_v_parse_compare = gem_v_parse_compare;
    _t472->gem_v_parse_not = gem_v_parse_not;
    _t472->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t472);
    struct _closure__anon_17 *_t479 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t479->gem_v_advance = gem_v_advance;
    _t479->gem_v_parse_not = gem_v_parse_not;
    _t479->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t479);
    struct _closure__anon_18 *_t486 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t486->gem_v_advance = gem_v_advance;
    _t486->gem_v_parse_and = gem_v_parse_and;
    _t486->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t486);
    struct _closure__anon_19 *_t530 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t530->gem_v_advance = gem_v_advance;
    _t530->gem_v_parse_expr = gem_v_parse_expr;
    _t530->gem_v_parse_or = gem_v_parse_or;
    _t530->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t530);
    *gem_v_parse_expr = gem_v_parse_assign_fn;
    struct _closure__anon_20 *_t769 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t769->gem_v_advance = gem_v_advance;
    _t769->gem_v_expect = gem_v_expect;
    _t769->gem_v_gensym_counter = gem_v_gensym_counter;
    _t769->gem_v_parse_body = gem_v_parse_body;
    _t769->gem_v_parse_expr = gem_v_parse_expr;
    _t769->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t769->gem_v_parse_params = gem_v_parse_params;
    _t769->gem_v_parse_stmt = gem_v_parse_stmt;
    _t769->gem_v_peek = gem_v_peek;
    _t769->gem_v_peek_at = gem_v_peek_at;
    _t769->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_20, _t769);
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
    struct _closure__anon_21 *_t777 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t777->gem_v_at_end = gem_v_at_end;
    _t777->gem_v_parse_stmt = gem_v_parse_stmt;
    _t777->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_21, _t777);
    GemVal _t778 = gem_table_new();
    gem_table_set(_t778, gem_string("parse"), gem_v_parse);
    return _t778;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = gem_string("");
    {
        GemVal gem_v__for_i_1 = gem_int(0);
    GemVal _t779[] = {gem_v_s};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t779, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
            GemVal gem_v_i = gem_v__for_i_1;
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
            GemVal gem_v_ch = gem_table_get(gem_v_s, gem_v_i);
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\\")))) {
                gem_v_result = gem_add(gem_v_result, gem_string("\\\\"));
            } else {
                if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
                    gem_v_result = gem_add(gem_v_result, gem_string("\\\""));
                } else {
                    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
                        gem_v_result = gem_add(gem_v_result, gem_string("\\n"));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\t")))) {
                            gem_v_result = gem_add(gem_v_result, gem_string("\\t"));
                        } else {
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
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_result = gem_string("");
    {
        GemVal gem_v__for_i_2 = gem_int(0);
        GemVal gem_v__for_limit_2 = gem_v_n;
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
            GemVal gem_v_i = gem_v__for_i_2;
            gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
            gem_v_result = gem_add(gem_v_result, gem_v_s);
        }
    }
    return gem_v_result;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t780[] = {gem_string("    "), gem_v_indent};
    return gem_fn_repeat_str(NULL, _t780, 2);
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t781[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t781, 1);
    GemVal gem_v_has_dot = gem_bool(0);
    {
        GemVal gem_v__for_i_3 = gem_int(0);
    GemVal _t782[] = {gem_v_s};
        GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t782, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
            GemVal gem_v_i = gem_v__for_i_3;
            gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_string(".")))) {
                gem_v_has_dot = gem_bool(1);
            }
        }
    }
    if (gem_truthy(gem_not(gem_v_has_dot))) {
        gem_v_s = gem_add(gem_v_s, gem_string(".0"));
    }
    return gem_v_s;
}

static GemVal gem_fn_array_contains(void *_env, GemVal *args, int argc) {
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    {
        GemVal gem_v__for_i_4 = gem_int(0);
    GemVal _t783[] = {gem_v_arr};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t783, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
            GemVal gem_v_i = gem_v__for_i_4;
            gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
            if (gem_truthy(gem_eq(gem_table_get(gem_v_arr, gem_v_i), gem_v_val))) {
                return gem_bool(1);
            }
        }
    }
    return gem_bool(0);
}

static GemVal gem_fn_set_add(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1)))) {
        gem_table_set(gem_v_s, gem_v_val, gem_bool(1));
        return GEM_NIL;
    } else {
        return GEM_NIL;
    }
}

static GemVal gem_fn_set_contains(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    return gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t784 = gem_table_new();
    GemVal gem_v_result = _t784;
    GemVal _t785[] = {gem_v_a};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t785, 1);
    GemVal gem_v_i = gem_int(0);
    while (1) {
        GemVal _t786[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t786, 1)))) break;
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t787[] = {gem_v_b};
    GemVal gem_v_ks2 = gem_fn_keys(NULL, _t787, 1);
    GemVal gem_v_j = gem_int(0);
    while (1) {
        GemVal _t788[] = {gem_v_ks2};
        if (!gem_truthy(gem_lt(gem_v_j, gem_len_fn(NULL, _t788, 1)))) break;
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks2, gem_v_j), gem_bool(1));
        gem_v_j = gem_add(gem_v_j, gem_int(1));
    }
    return gem_v_result;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t789 = gem_table_new();
    GemVal gem_v_result = _t789;
    GemVal _t790[] = {gem_v_a};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t790, 1);
    GemVal gem_v_i = gem_int(0);
    while (1) {
        GemVal _t791[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t791, 1)))) break;
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_table_get(gem_v_ks, gem_v_i)), gem_bool(1)))) {
            gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
        }
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    return gem_v_result;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t792[] = {gem_v_s};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t792, 1);
    GemVal gem_v_i = gem_int(1);
    while (1) {
        GemVal _t793[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t793, 1)))) break;
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
        while (1) {
            GemVal _t794;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t794 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t794 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t794)) break;
            gem_table_set(gem_v_ks, gem_add(gem_v_j, gem_int(1)), gem_table_get(gem_v_ks, gem_v_j));
            gem_v_j = gem_sub(gem_v_j, gem_int(1));
        }
        gem_table_set(gem_v_ks, gem_add(gem_v_j, gem_int(1)), gem_v_key);
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    return gem_v_ks;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t795 = gem_table_new();
    GemVal gem_v_s = _t795;
    {
        GemVal gem_v__for_items_5 = gem_v_arr;
        GemVal gem_v__for_i_5 = gem_int(0);
        while (1) {
            GemVal _t796[] = {gem_v__for_items_5};
            if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t796, 1)))) break;
            GemVal gem_v_item = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
            gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
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
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t802[] = {(*gem_v_tmp_counter)};
    return gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t802, 1));
}

struct _closure__anon_23 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t804[] = {(*gem_v_anon_counter)};
    return gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t804, 1));
}

struct _closure__anon_24 {
};
static GemVal _anon_24(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t806[] = {gem_v_name};
    return gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t806, 1));
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
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
        return GEM_NIL;
    }
    GemVal _t807[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t807, 1), gem_string("table")))) {
        return GEM_NIL;
    }
    GemVal _t808 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t808, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
        return GEM_NIL;
    }
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
    GemVal _t809 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t809, gem_string("name"));
    GemVal _t810[] = {gem_v_defined, gem_v_name};
    GemVal _t812;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t810, 2)))) {
        _t812 = gem_not(gem_fn_set_contains(NULL, _t810, 2));
    } else {
        GemVal _t811[] = {(*gem_v_builtins), gem_v_name};
        _t812 = gem_not(gem_fn_set_contains(NULL, _t811, 2));
    }
    GemVal _t814;
    if (!gem_truthy(_t812)) {
        _t814 = _t812;
    } else {
        GemVal _t813[] = {(*gem_v_defined_fns), gem_v_name};
        _t814 = gem_not(gem_fn_set_contains(NULL, _t813, 2));
    }
        if (gem_truthy(_t814)) {
    GemVal _t815[] = {gem_v_free, gem_v_name};
            return gem_fn_set_add(NULL, _t815, 2);
        } else {
            return GEM_NIL;
        }
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t816 = gem_v_node;
    GemVal _t817 = (*gem_v_collect_free_node);
    GemVal _t818[] = {gem_table_get(_t816, gem_string("value")), gem_v_defined, gem_v_free};
            return _t817.fn(_t817.env, _t818, 3);
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t819 = gem_v_node;
                GemVal gem_v_name = gem_table_get(_t819, gem_string("name"));
    GemVal _t820[] = {gem_v_defined, gem_v_name};
    GemVal _t822;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t820, 2)))) {
        _t822 = gem_not(gem_fn_set_contains(NULL, _t820, 2));
    } else {
        GemVal _t821[] = {(*gem_v_builtins), gem_v_name};
        _t822 = gem_not(gem_fn_set_contains(NULL, _t821, 2));
    }
    GemVal _t824;
    if (!gem_truthy(_t822)) {
        _t824 = _t822;
    } else {
        GemVal _t823[] = {(*gem_v_defined_fns), gem_v_name};
        _t824 = gem_not(gem_fn_set_contains(NULL, _t823, 2));
    }
                if (gem_truthy(_t824)) {
    GemVal _t825[] = {gem_v_free, gem_v_name};
                    (void)(gem_fn_set_add(NULL, _t825, 2));
                }
    GemVal _t826 = gem_v_node;
    GemVal _t827 = (*gem_v_collect_free_node);
    GemVal _t828[] = {gem_table_get(_t826, gem_string("value")), gem_v_defined, gem_v_free};
                return _t827.fn(_t827.env, _t828, 3);
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
    GemVal _t829 = gem_v_node;
    GemVal _t830[] = {gem_table_get(_t829, gem_string("params"))};
    GemVal _t831[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t830, 1)};
                    GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t831, 2);
    GemVal _t832 = gem_v_node;
    GemVal _t833 = (*gem_v_collect_free_vars);
    GemVal _t834[] = {gem_table_get(_t832, gem_string("body")), gem_v_inner_defined};
                    GemVal gem_v_inner_free = _t833.fn(_t833.env, _t834, 2);
    GemVal _t835[] = {gem_v_inner_free};
                    GemVal gem_v_ifks = gem_fn_keys(NULL, _t835, 1);
                    GemVal gem_v_ifi = gem_int(0);
                    while (1) {
                        GemVal _t836[] = {gem_v_ifks};
                        if (!gem_truthy(gem_lt(gem_v_ifi, gem_len_fn(NULL, _t836, 1)))) break;
    GemVal _t837[] = {gem_v_free, gem_table_get(gem_v_ifks, gem_v_ifi)};
                        (void)(gem_fn_set_add(NULL, _t837, 2));
                        gem_v_ifi = gem_add(gem_v_ifi, gem_int(1));
                    }
                    return GEM_NIL;
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                        return GEM_NIL;
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t838 = gem_v_node;
    GemVal _t839 = (*gem_v_collect_free_node);
    GemVal _t840[] = {gem_table_get(_t838, gem_string("cond")), gem_v_defined, gem_v_free};
                            (void)(_t839.fn(_t839.env, _t840, 3));
    GemVal _t841 = gem_table_new();
    GemVal _t842[] = {gem_v_defined, _t841};
                            GemVal gem_v_d = gem_fn_set_union(NULL, _t842, 2);
                            GemVal gem_v_thens = gem_table_get(gem_v_node, gem_string("then"));
                            {
                                GemVal gem_v__for_i_6 = gem_int(0);
    GemVal _t843[] = {gem_v_thens};
                                GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t843, 1);
                                while (1) {
                                    if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
                                    GemVal gem_v_i = gem_v__for_i_6;
                                    gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
    GemVal _t844 = (*gem_v_collect_free_node);
    GemVal _t845[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
                                    (void)(_t844.fn(_t844.env, _t845, 3));
    GemVal _t846[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t848;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t846, 1), gem_string("table")))) {
        _t848 = gem_eq(gem_type_fn(NULL, _t846, 1), gem_string("table"));
    } else {
        GemVal _t847 = gem_table_get(gem_v_thens, gem_v_i);
        _t848 = gem_eq(gem_table_get(_t847, gem_string("tag")), gem_string("let"));
    }
                                    if (gem_truthy(_t848)) {
    GemVal _t849 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t850[] = {gem_v_d, gem_table_get(_t849, gem_string("name"))};
                                        (void)(gem_fn_set_add(NULL, _t850, 2));
                                    }
                                }
                            }
                            GemVal gem_v_el = gem_table_get(gem_v_node, gem_string("else"));
                            if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
    GemVal _t851 = gem_table_new();
    GemVal _t852[] = {gem_v_defined, _t851};
                                gem_v_d = gem_fn_set_union(NULL, _t852, 2);
                                {
                                    GemVal gem_v__for_i_7 = gem_int(0);
    GemVal _t853[] = {gem_v_el};
                                    GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t853, 1);
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
                                        GemVal gem_v_i = gem_v__for_i_7;
                                        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
    GemVal _t854 = (*gem_v_collect_free_node);
    GemVal _t855[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t854.fn(_t854.env, _t855, 3));
    GemVal _t856[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t858;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t856, 1), gem_string("table")))) {
        _t858 = gem_eq(gem_type_fn(NULL, _t856, 1), gem_string("table"));
    } else {
        GemVal _t857 = gem_table_get(gem_v_el, gem_v_i);
        _t858 = gem_eq(gem_table_get(_t857, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t858)) {
    GemVal _t859 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t860[] = {gem_v_d, gem_table_get(_t859, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t860, 2));
                                        }
                                    }
                                    return GEM_NIL;
                                }
                            } else {
                                return GEM_NIL;
                            }
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t861 = gem_v_node;
    GemVal _t862 = (*gem_v_collect_free_node);
    GemVal _t863[] = {gem_table_get(_t861, gem_string("cond")), gem_v_defined, gem_v_free};
                                (void)(_t862.fn(_t862.env, _t863, 3));
    GemVal _t864 = gem_table_new();
    GemVal _t865[] = {gem_v_defined, _t864};
                                GemVal gem_v_d = gem_fn_set_union(NULL, _t865, 2);
                                {
                                    GemVal gem_v__for_i_8 = gem_int(0);
    GemVal _t866 = gem_v_node;
    GemVal _t867[] = {gem_table_get(_t866, gem_string("body"))};
                                    GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t867, 1);
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
                                        GemVal gem_v_i = gem_v__for_i_8;
                                        gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
    GemVal _t868 = gem_v_node;
    GemVal _t869 = (*gem_v_collect_free_node);
    GemVal _t870[] = {gem_table_get(gem_table_get(_t868, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t869.fn(_t869.env, _t870, 3));
    GemVal _t871 = gem_v_node;
    GemVal _t872[] = {gem_table_get(gem_table_get(_t871, gem_string("body")), gem_v_i)};
    GemVal _t875;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t872, 1), gem_string("table")))) {
        _t875 = gem_eq(gem_type_fn(NULL, _t872, 1), gem_string("table"));
    } else {
        GemVal _t873 = gem_v_node;
        GemVal _t874 = gem_table_get(gem_table_get(_t873, gem_string("body")), gem_v_i);
        _t875 = gem_eq(gem_table_get(_t874, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t875)) {
    GemVal _t876 = gem_v_node;
    GemVal _t877 = gem_table_get(gem_table_get(_t876, gem_string("body")), gem_v_i);
    GemVal _t878[] = {gem_v_d, gem_table_get(_t877, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t878, 2));
                                        }
                                    }
                                    return GEM_NIL;
                                }
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t879 = gem_v_node;
    GemVal _t880 = (*gem_v_collect_free_node);
    GemVal _t881[] = {gem_table_get(_t879, gem_string("target")), gem_v_defined, gem_v_free};
                                    (void)(_t880.fn(_t880.env, _t881, 3));
                                    {
                                        GemVal gem_v__for_i_10 = gem_int(0);
    GemVal _t882 = gem_v_node;
    GemVal _t883[] = {gem_table_get(_t882, gem_string("whens"))};
                                        GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t883, 1);
                                        while (1) {
                                            if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
                                            GemVal gem_v_i = gem_v__for_i_10;
                                            gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
    GemVal _t884 = gem_v_node;
                                            GemVal gem_v_w = gem_table_get(gem_table_get(_t884, gem_string("whens")), gem_v_i);
    GemVal _t885 = gem_v_w;
    GemVal _t886 = (*gem_v_collect_free_node);
    GemVal _t887[] = {gem_table_get(_t885, gem_string("value")), gem_v_defined, gem_v_free};
                                            (void)(_t886.fn(_t886.env, _t887, 3));
    GemVal _t888 = gem_table_new();
    GemVal _t889[] = {gem_v_defined, _t888};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t889, 2);
                                            {
                                                GemVal gem_v__for_i_9 = gem_int(0);
    GemVal _t890 = gem_v_w;
    GemVal _t891[] = {gem_table_get(_t890, gem_string("body"))};
                                                GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t891, 1);
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
                                                    GemVal gem_v_j = gem_v__for_i_9;
                                                    gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
    GemVal _t892 = gem_v_w;
    GemVal _t893 = (*gem_v_collect_free_node);
    GemVal _t894[] = {gem_table_get(gem_table_get(_t892, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
                                                    (void)(_t893.fn(_t893.env, _t894, 3));
    GemVal _t895 = gem_v_w;
    GemVal _t896[] = {gem_table_get(gem_table_get(_t895, gem_string("body")), gem_v_j)};
    GemVal _t899;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t896, 1), gem_string("table")))) {
        _t899 = gem_eq(gem_type_fn(NULL, _t896, 1), gem_string("table"));
    } else {
        GemVal _t897 = gem_v_w;
        GemVal _t898 = gem_table_get(gem_table_get(_t897, gem_string("body")), gem_v_j);
        _t899 = gem_eq(gem_table_get(_t898, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t899)) {
    GemVal _t900 = gem_v_w;
    GemVal _t901 = gem_table_get(gem_table_get(_t900, gem_string("body")), gem_v_j);
    GemVal _t902[] = {gem_v_d, gem_table_get(_t901, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t902, 2));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    GemVal gem_v_el = gem_table_get(gem_v_node, gem_string("else"));
                                    if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
    GemVal _t903 = gem_table_new();
    GemVal _t904[] = {gem_v_defined, _t903};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t904, 2);
                                        {
                                            GemVal gem_v__for_i_11 = gem_int(0);
    GemVal _t905[] = {gem_v_el};
                                            GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t905, 1);
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
                                                GemVal gem_v_j = gem_v__for_i_11;
                                                gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
    GemVal _t906 = (*gem_v_collect_free_node);
    GemVal _t907[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
                                                (void)(_t906.fn(_t906.env, _t907, 3));
    GemVal _t908[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t910;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t908, 1), gem_string("table")))) {
        _t910 = gem_eq(gem_type_fn(NULL, _t908, 1), gem_string("table"));
    } else {
        GemVal _t909 = gem_table_get(gem_v_el, gem_v_j);
        _t910 = gem_eq(gem_table_get(_t909, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t910)) {
    GemVal _t911 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t912[] = {gem_v_d, gem_table_get(_t911, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t912, 2));
                                                }
                                            }
                                            return GEM_NIL;
                                        }
                                    } else {
                                        return GEM_NIL;
                                    }
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("when_clause")))) {
    GemVal _t913 = gem_v_node;
    GemVal _t914 = (*gem_v_collect_free_node);
    GemVal _t915[] = {gem_table_get(_t913, gem_string("value")), gem_v_defined, gem_v_free};
                                        (void)(_t914.fn(_t914.env, _t915, 3));
    GemVal _t916 = gem_table_new();
    GemVal _t917[] = {gem_v_defined, _t916};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t917, 2);
                                        {
                                            GemVal gem_v__for_i_12 = gem_int(0);
    GemVal _t918 = gem_v_node;
    GemVal _t919[] = {gem_table_get(_t918, gem_string("body"))};
                                            GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t919, 1);
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
                                                GemVal gem_v_i = gem_v__for_i_12;
                                                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
    GemVal _t920 = gem_v_node;
    GemVal _t921 = (*gem_v_collect_free_node);
    GemVal _t922[] = {gem_table_get(gem_table_get(_t920, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                                (void)(_t921.fn(_t921.env, _t922, 3));
    GemVal _t923 = gem_v_node;
    GemVal _t924[] = {gem_table_get(gem_table_get(_t923, gem_string("body")), gem_v_i)};
    GemVal _t927;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t924, 1), gem_string("table")))) {
        _t927 = gem_eq(gem_type_fn(NULL, _t924, 1), gem_string("table"));
    } else {
        GemVal _t925 = gem_v_node;
        GemVal _t926 = gem_table_get(gem_table_get(_t925, gem_string("body")), gem_v_i);
        _t927 = gem_eq(gem_table_get(_t926, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t927)) {
    GemVal _t928 = gem_v_node;
    GemVal _t929 = gem_table_get(gem_table_get(_t928, gem_string("body")), gem_v_i);
    GemVal _t930[] = {gem_v_d, gem_table_get(_t929, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t930, 2));
                                                }
                                            }
                                            return GEM_NIL;
                                        }
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
    GemVal _t931 = gem_table_new();
    GemVal _t932[] = {gem_v_defined, _t931};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t932, 2);
                                            {
                                                GemVal gem_v__for_i_13 = gem_int(0);
    GemVal _t933 = gem_v_node;
    GemVal _t934[] = {gem_table_get(_t933, gem_string("stmts"))};
                                                GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t934, 1);
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
                                                    GemVal gem_v_i = gem_v__for_i_13;
                                                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
    GemVal _t935 = gem_v_node;
    GemVal _t936 = (*gem_v_collect_free_node);
    GemVal _t937[] = {gem_table_get(gem_table_get(_t935, gem_string("stmts")), gem_v_i), gem_v_d, gem_v_free};
                                                    (void)(_t936.fn(_t936.env, _t937, 3));
    GemVal _t938 = gem_v_node;
    GemVal _t939[] = {gem_table_get(gem_table_get(_t938, gem_string("stmts")), gem_v_i)};
    GemVal _t942;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t939, 1), gem_string("table")))) {
        _t942 = gem_eq(gem_type_fn(NULL, _t939, 1), gem_string("table"));
    } else {
        GemVal _t940 = gem_v_node;
        GemVal _t941 = gem_table_get(gem_table_get(_t940, gem_string("stmts")), gem_v_i);
        _t942 = gem_eq(gem_table_get(_t941, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t942)) {
    GemVal _t943 = gem_v_node;
    GemVal _t944 = gem_table_get(gem_table_get(_t943, gem_string("stmts")), gem_v_i);
    GemVal _t945[] = {gem_v_d, gem_table_get(_t944, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t945, 2));
                                                    }
                                                }
                                                return GEM_NIL;
                                            }
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t946 = gem_v_node;
                                                if (gem_truthy(gem_neq(gem_table_get(_t946, gem_string("value")), GEM_NIL))) {
    GemVal _t947 = gem_v_node;
    GemVal _t948 = (*gem_v_collect_free_node);
    GemVal _t949[] = {gem_table_get(_t947, gem_string("value")), gem_v_defined, gem_v_free};
                                                    return _t948.fn(_t948.env, _t949, 3);
                                                } else {
                                                    return GEM_NIL;
                                                }
                                            } else {
    GemVal _t950[] = {gem_v_node};
                                                GemVal gem_v_nks = gem_fn_keys(NULL, _t950, 1);
                                                GemVal gem_v_nki = gem_int(0);
                                                while (1) {
                                                    GemVal _t951[] = {gem_v_nks};
                                                    if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t951, 1)))) break;
                                                    if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
                                                        GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
    GemVal _t952[] = {gem_v_v};
                                                        if (gem_truthy(gem_eq(gem_type_fn(NULL, _t952, 1), gem_string("table")))) {
    GemVal _t953[] = {gem_v_v};
                                                            GemVal gem_v_vks = gem_fn_keys(NULL, _t953, 1);
    GemVal _t954[] = {gem_v_vks};
    GemVal _t956;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t954, 1), gem_int(0)))) {
        _t956 = gem_gt(gem_len_fn(NULL, _t954, 1), gem_int(0));
    } else {
        GemVal _t955[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t956 = gem_eq(gem_type_fn(NULL, _t955, 1), gem_string("int"));
    }
                                                            if (gem_truthy(_t956)) {
                                                                GemVal gem_v_vi = gem_int(0);
                                                                while (1) {
                                                                    GemVal _t957[] = {gem_v_v};
                                                                    if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t957, 1)))) break;
    GemVal _t958[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t958, 1), gem_string("table")))) {
    GemVal _t959 = (*gem_v_collect_free_node);
    GemVal _t960[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
                                                                        (void)(_t959.fn(_t959.env, _t960, 3));
                                                                    }
                                                                    gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                                }
                                                            } else {
    GemVal _t961 = (*gem_v_collect_free_node);
    GemVal _t962[] = {gem_v_v, gem_v_defined, gem_v_free};
                                                                (void)(_t961.fn(_t961.env, _t962, 3));
                                                            }
                                                        }
                                                    }
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
    GemVal _t964 = gem_table_new();
    GemVal gem_v_free = _t964;
    GemVal _t965 = gem_table_new();
    GemVal _t966[] = {gem_v_defined, _t965};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t966, 2);
    {
        GemVal gem_v__for_i_15 = gem_int(0);
    GemVal _t967[] = {gem_v_stmts};
        GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t967, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
            GemVal gem_v_i = gem_v__for_i_15;
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
    GemVal _t968 = (*gem_v_collect_free_node);
    GemVal _t969[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
            (void)(_t968.fn(_t968.env, _t969, 3));
    GemVal _t970[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t972;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t970, 1), gem_string("table")))) {
        _t972 = gem_eq(gem_type_fn(NULL, _t970, 1), gem_string("table"));
    } else {
        GemVal _t971 = gem_table_get(gem_v_stmts, gem_v_i);
        _t972 = gem_eq(gem_table_get(_t971, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t972)) {
    GemVal _t973 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t974[] = {gem_v_d, gem_table_get(_t973, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t974, 2));
            }
    GemVal _t975[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t977;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t975, 1), gem_string("table")))) {
        _t977 = gem_eq(gem_type_fn(NULL, _t975, 1), gem_string("table"));
    } else {
        GemVal _t976 = gem_table_get(gem_v_stmts, gem_v_i);
        _t977 = gem_eq(gem_table_get(_t976, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t977)) {
                {
                    GemVal gem_v__for_i_14 = gem_int(0);
    GemVal _t978 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t979[] = {gem_table_get(_t978, gem_string("stmts"))};
                    GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t979, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
                        GemVal gem_v_j = gem_v__for_i_14;
                        gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
    GemVal _t980 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t981[] = {gem_table_get(gem_table_get(_t980, gem_string("stmts")), gem_v_j)};
    GemVal _t984;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t981, 1), gem_string("table")))) {
        _t984 = gem_eq(gem_type_fn(NULL, _t981, 1), gem_string("table"));
    } else {
        GemVal _t982 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t983 = gem_table_get(gem_table_get(_t982, gem_string("stmts")), gem_v_j);
        _t984 = gem_eq(gem_table_get(_t983, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t984)) {
    GemVal _t985 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t986 = gem_table_get(gem_table_get(_t985, gem_string("stmts")), gem_v_j);
    GemVal _t987[] = {gem_v_d, gem_table_get(_t986, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t987, 2));
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
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
        return GEM_NIL;
    }
    GemVal _t989[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t989, 1), gem_string("table")))) {
        return GEM_NIL;
    }
    GemVal _t990 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t990, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
        return GEM_NIL;
    }
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
    GemVal _t991 = gem_v_node;
    GemVal _t992[] = {gem_table_get(_t991, gem_string("params"))};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t992, 1);
    GemVal _t993 = gem_v_node;
    GemVal _t994 = (*gem_v_collect_free_vars);
    GemVal _t995[] = {gem_table_get(_t993, gem_string("body")), gem_v_inner_defined};
        GemVal gem_v_free = _t994.fn(_t994.env, _t995, 2);
    GemVal _t996[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t996, 2);
    GemVal _t997[] = {gem_v_inter};
        GemVal gem_v_iks = gem_fn_keys(NULL, _t997, 1);
        GemVal gem_v_iki = gem_int(0);
        while (1) {
            GemVal _t998[] = {gem_v_iks};
            if (!gem_truthy(gem_lt(gem_v_iki, gem_len_fn(NULL, _t998, 1)))) break;
    GemVal _t999[] = {gem_v_captured, gem_table_get(gem_v_iks, gem_v_iki)};
            (void)(gem_fn_set_add(NULL, _t999, 2));
            gem_v_iki = gem_add(gem_v_iki, gem_int(1));
        }
    GemVal _t1000 = gem_v_node;
    GemVal _t1001 = gem_v_node;
    GemVal _t1002[] = {gem_table_get(_t1001, gem_string("params"))};
    GemVal _t1003[] = {gem_v_scope_vars, gem_fn_sorted_array_to_set(NULL, _t1002, 1)};
    GemVal _t1004 = (*gem_v_walk_captures);
    GemVal _t1005[] = {gem_table_get(_t1000, gem_string("body")), gem_fn_set_union(NULL, _t1003, 2), gem_v_captured};
        return _t1004.fn(_t1004.env, _t1005, 3);
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
            return GEM_NIL;
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t1006 = gem_v_node;
    GemVal _t1007 = (*gem_v_walk_captures_node);
    GemVal _t1008[] = {gem_table_get(_t1006, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1007.fn(_t1007.env, _t1008, 3));
    GemVal _t1009 = (*gem_v_walk_captures);
    GemVal _t1010[] = {gem_table_get(gem_v_node, gem_string("then")), gem_v_scope_vars, gem_v_captured};
                (void)(_t1009.fn(_t1009.env, _t1010, 3));
                if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
    GemVal _t1011 = (*gem_v_walk_captures);
    GemVal _t1012[] = {gem_table_get(gem_v_node, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                    return _t1011.fn(_t1011.env, _t1012, 3);
                } else {
                    return GEM_NIL;
                }
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t1013 = gem_v_node;
    GemVal _t1014 = (*gem_v_walk_captures_node);
    GemVal _t1015[] = {gem_table_get(_t1013, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                    (void)(_t1014.fn(_t1014.env, _t1015, 3));
    GemVal _t1016 = gem_v_node;
    GemVal _t1017 = (*gem_v_walk_captures);
    GemVal _t1018[] = {gem_table_get(_t1016, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                    return _t1017.fn(_t1017.env, _t1018, 3);
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t1019 = gem_v_node;
    GemVal _t1020 = (*gem_v_walk_captures_node);
    GemVal _t1021[] = {gem_table_get(_t1019, gem_string("target")), gem_v_scope_vars, gem_v_captured};
                        (void)(_t1020.fn(_t1020.env, _t1021, 3));
                        {
                            GemVal gem_v__for_i_16 = gem_int(0);
    GemVal _t1022 = gem_v_node;
    GemVal _t1023[] = {gem_table_get(_t1022, gem_string("whens"))};
                            GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1023, 1);
                            while (1) {
                                if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
                                GemVal gem_v_i = gem_v__for_i_16;
                                gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
    GemVal _t1024 = gem_v_node;
    GemVal _t1025 = gem_table_get(gem_table_get(_t1024, gem_string("whens")), gem_v_i);
    GemVal _t1026 = (*gem_v_walk_captures_node);
    GemVal _t1027[] = {gem_table_get(_t1025, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t1026.fn(_t1026.env, _t1027, 3));
    GemVal _t1028 = gem_v_node;
    GemVal _t1029 = gem_table_get(gem_table_get(_t1028, gem_string("whens")), gem_v_i);
    GemVal _t1030 = (*gem_v_walk_captures);
    GemVal _t1031[] = {gem_table_get(_t1029, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t1030.fn(_t1030.env, _t1031, 3));
                            }
                        }
                        if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
    GemVal _t1032 = (*gem_v_walk_captures);
    GemVal _t1033[] = {gem_table_get(gem_v_node, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                            return _t1032.fn(_t1032.env, _t1033, 3);
                        } else {
                            return GEM_NIL;
                        }
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
    GemVal _t1034 = gem_v_node;
    GemVal _t1035 = (*gem_v_walk_captures);
    GemVal _t1036[] = {gem_table_get(_t1034, gem_string("stmts")), gem_v_scope_vars, gem_v_captured};
                            return _t1035.fn(_t1035.env, _t1036, 3);
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t1037 = gem_v_node;
                                if (gem_truthy(gem_neq(gem_table_get(_t1037, gem_string("value")), GEM_NIL))) {
    GemVal _t1038 = gem_v_node;
    GemVal _t1039 = (*gem_v_walk_captures_node);
    GemVal _t1040[] = {gem_table_get(_t1038, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    return _t1039.fn(_t1039.env, _t1040, 3);
                                } else {
                                    return GEM_NIL;
                                }
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t1041 = gem_v_node;
    GemVal _t1042 = (*gem_v_walk_captures_node);
    GemVal _t1043[] = {gem_table_get(_t1041, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    return _t1042.fn(_t1042.env, _t1043, 3);
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t1044 = gem_v_node;
    GemVal _t1045 = (*gem_v_walk_captures_node);
    GemVal _t1046[] = {gem_table_get(_t1044, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                        return _t1045.fn(_t1045.env, _t1046, 3);
                                    } else {
    GemVal _t1047[] = {gem_v_node};
                                        GemVal gem_v_nks = gem_fn_keys(NULL, _t1047, 1);
                                        GemVal gem_v_nki = gem_int(0);
                                        while (1) {
                                            GemVal _t1048[] = {gem_v_nks};
                                            if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t1048, 1)))) break;
                                            if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
                                                GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
    GemVal _t1049[] = {gem_v_v};
                                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1049, 1), gem_string("table")))) {
    GemVal _t1050[] = {gem_v_v};
                                                    GemVal gem_v_vks = gem_fn_keys(NULL, _t1050, 1);
    GemVal _t1051[] = {gem_v_vks};
    GemVal _t1053;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1051, 1), gem_int(0)))) {
        _t1053 = gem_gt(gem_len_fn(NULL, _t1051, 1), gem_int(0));
    } else {
        GemVal _t1052[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1053 = gem_eq(gem_type_fn(NULL, _t1052, 1), gem_string("int"));
    }
                                                    if (gem_truthy(_t1053)) {
                                                        GemVal gem_v_vi = gem_int(0);
                                                        while (1) {
                                                            GemVal _t1054[] = {gem_v_v};
                                                            if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1054, 1)))) break;
    GemVal _t1055[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                            if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1055, 1), gem_string("table")))) {
    GemVal _t1056 = (*gem_v_walk_captures_node);
    GemVal _t1057[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
                                                                (void)(_t1056.fn(_t1056.env, _t1057, 3));
                                                            }
                                                            gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                        }
                                                    } else {
    GemVal _t1058 = (*gem_v_walk_captures_node);
    GemVal _t1059[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
                                                        (void)(_t1058.fn(_t1058.env, _t1059, 3));
                                                    }
                                                }
                                            }
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
    GemVal _t1061 = gem_table_new();
    GemVal _t1062[] = {gem_v_scope_vars, _t1061};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t1062, 2);
    {
        GemVal gem_v__for_i_18 = gem_int(0);
    GemVal _t1063[] = {gem_v_stmts};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1063, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
            GemVal gem_v_i = gem_v__for_i_18;
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
    GemVal _t1064 = (*gem_v_walk_captures_node);
    GemVal _t1065[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
            (void)(_t1064.fn(_t1064.env, _t1065, 3));
    GemVal _t1066[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1068;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1066, 1), gem_string("table")))) {
        _t1068 = gem_eq(gem_type_fn(NULL, _t1066, 1), gem_string("table"));
    } else {
        GemVal _t1067 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1068 = gem_eq(gem_table_get(_t1067, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1068)) {
    GemVal _t1069 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1070[] = {gem_v_sv, gem_table_get(_t1069, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1070, 2));
            }
    GemVal _t1071[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1073;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1071, 1), gem_string("table")))) {
        _t1073 = gem_eq(gem_type_fn(NULL, _t1071, 1), gem_string("table"));
    } else {
        GemVal _t1072 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1073 = gem_eq(gem_table_get(_t1072, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1073)) {
                {
                    GemVal gem_v__for_i_17 = gem_int(0);
    GemVal _t1074 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1075[] = {gem_table_get(_t1074, gem_string("stmts"))};
                    GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1075, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
                        GemVal gem_v_j = gem_v__for_i_17;
                        gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
    GemVal _t1076 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1077[] = {gem_table_get(gem_table_get(_t1076, gem_string("stmts")), gem_v_j)};
    GemVal _t1080;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1077, 1), gem_string("table")))) {
        _t1080 = gem_eq(gem_type_fn(NULL, _t1077, 1), gem_string("table"));
    } else {
        GemVal _t1078 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1079 = gem_table_get(gem_table_get(_t1078, gem_string("stmts")), gem_v_j);
        _t1080 = gem_eq(gem_table_get(_t1079, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1080)) {
    GemVal _t1081 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1082 = gem_table_get(gem_table_get(_t1081, gem_string("stmts")), gem_v_j);
    GemVal _t1083[] = {gem_v_sv, gem_table_get(_t1082, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1083, 2));
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
    GemVal _t1085 = gem_table_new();
    GemVal gem_v_captured = _t1085;
    GemVal _t1086 = (*gem_v_walk_captures);
    GemVal _t1087[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    (void)(_t1086.fn(_t1086.env, _t1087, 3));
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
    GemVal _t1089 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1089, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("int")))) {
    GemVal _t1090 = gem_table_new();
    GemVal _t1091 = gem_v_node;
    GemVal _t1092[] = {gem_table_get(_t1091, gem_string("value"))};
    gem_table_set(_t1090, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t1092, 1)), gem_string(")")));
    gem_table_set(_t1090, gem_string("setup"), gem_string(""));
        return _t1090;
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("float")))) {
    GemVal _t1093 = gem_table_new();
    GemVal _t1094 = gem_v_node;
    GemVal _t1095[] = {gem_table_get(_t1094, gem_string("value"))};
    GemVal _t1096[] = {gem_fn_format_float(NULL, _t1095, 1)};
    gem_table_set(_t1093, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t1096, 1)), gem_string(")")));
    gem_table_set(_t1093, gem_string("setup"), gem_string(""));
            return _t1093;
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("string")))) {
    GemVal _t1097 = gem_v_node;
    GemVal _t1098[] = {gem_table_get(_t1097, gem_string("value"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1098, 1);
    GemVal _t1099 = gem_table_new();
    GemVal _t1100[] = {gem_v_escaped};
    gem_table_set(_t1099, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t1100, 1)), gem_string("\")")));
    gem_table_set(_t1099, gem_string("setup"), gem_string(""));
                return _t1099;
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("interp")))) {
    GemVal _t1101 = gem_v_node;
                    GemVal gem_v_parts = gem_table_get(_t1101, gem_string("parts"));
    GemVal _t1102[] = {gem_v_parts};
                    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1102, 1), gem_int(0)))) {
    GemVal _t1103 = gem_table_new();
    gem_table_set(_t1103, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t1103, gem_string("setup"), gem_string(""));
                        return _t1103;
                    }
                    GemVal gem_v_setup = gem_string("");
    GemVal _t1104 = gem_table_new();
                    GemVal gem_v_compiled = _t1104;
                    {
                        GemVal gem_v__for_i_19 = gem_int(0);
    GemVal _t1105[] = {gem_v_parts};
                        GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1105, 1);
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
                            GemVal gem_v_i = gem_v__for_i_19;
                            gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
                            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
    GemVal _t1106 = (*gem_v_compile_expr);
    GemVal _t1107[] = {gem_v_part};
                            GemVal gem_v_r = _t1106.fn(_t1106.env, _t1107, 1);
    GemVal _t1108 = gem_v_r;
                            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1108, gem_string("setup")));
    GemVal _t1109 = gem_v_part;
                            if (gem_truthy(gem_eq(gem_table_get(_t1109, gem_string("tag")), gem_string("string")))) {
    GemVal _t1110 = gem_v_r;
    GemVal _t1111[] = {gem_v_compiled, gem_table_get(_t1110, gem_string("expr"))};
                                (void)(gem_push_fn(NULL, _t1111, 2));
                            } else {
    GemVal _t1112 = (*gem_v_tmp);
                                GemVal gem_v_ts = _t1112.fn(_t1112.env, NULL, 0);
    GemVal _t1113[] = {gem_v_ts};
    GemVal _t1114 = gem_v_r;
    GemVal _t1115[] = {gem_table_get(_t1114, gem_string("expr"))};
                                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1113, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1115, 1)), gem_string("};\n")));
    GemVal _t1116[] = {gem_v_ts};
    GemVal _t1117[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t1116, 1)), gem_string(", 1)"))};
                                (void)(gem_push_fn(NULL, _t1117, 2));
                            }
                        }
                    }
                    GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
                    {
                        GemVal gem_v__for_i_20 = gem_int(1);
    GemVal _t1118[] = {gem_v_compiled};
                        GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1118, 1);
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
                            GemVal gem_v_i = gem_v__for_i_20;
                            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
    GemVal _t1119[] = {gem_v_acc};
    GemVal _t1120[] = {gem_table_get(gem_v_compiled, gem_v_i)};
                            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1119, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1120, 1)), gem_string(")"));
                        }
                    }
    GemVal _t1121 = gem_table_new();
    gem_table_set(_t1121, gem_string("expr"), gem_v_acc);
    gem_table_set(_t1121, gem_string("setup"), gem_v_setup);
                    return _t1121;
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("bool")))) {
    GemVal _t1122 = gem_v_node;
                        if (gem_truthy(gem_table_get(_t1122, gem_string("value")))) {
    GemVal _t1123 = gem_table_new();
    gem_table_set(_t1123, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t1123, gem_string("setup"), gem_string(""));
                            return _t1123;
                        }
    GemVal _t1124 = gem_table_new();
    gem_table_set(_t1124, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t1124, gem_string("setup"), gem_string(""));
                        return _t1124;
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("nil")))) {
    GemVal _t1125 = gem_table_new();
    gem_table_set(_t1125, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1125, gem_string("setup"), gem_string(""));
                            return _t1125;
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
    GemVal _t1126 = gem_v_node;
                                GemVal gem_v_name = gem_table_get(_t1126, gem_string("name"));
                                if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
    GemVal _t1127 = gem_table_new();
    gem_table_set(_t1127, gem_string("expr"), gem_string("gem_make_fn(gem_print, NULL)"));
    gem_table_set(_t1127, gem_string("setup"), gem_string(""));
                                    return _t1127;
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
    GemVal _t1128 = gem_table_new();
    gem_table_set(_t1128, gem_string("expr"), gem_string("gem_make_fn(gem_error_fn, NULL)"));
    gem_table_set(_t1128, gem_string("setup"), gem_string(""));
                                        return _t1128;
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("len")))) {
    GemVal _t1129 = gem_table_new();
    gem_table_set(_t1129, gem_string("expr"), gem_string("gem_make_fn(gem_len_fn, NULL)"));
    gem_table_set(_t1129, gem_string("setup"), gem_string(""));
                                            return _t1129;
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("type")))) {
    GemVal _t1130 = gem_table_new();
    gem_table_set(_t1130, gem_string("expr"), gem_string("gem_make_fn(gem_type_fn, NULL)"));
    gem_table_set(_t1130, gem_string("setup"), gem_string(""));
                                                return _t1130;
                                            } else {
                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("to_string")))) {
    GemVal _t1131 = gem_table_new();
    gem_table_set(_t1131, gem_string("expr"), gem_string("gem_make_fn(gem_to_string_fn, NULL)"));
    gem_table_set(_t1131, gem_string("setup"), gem_string(""));
                                                    return _t1131;
                                                } else {
                                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("push")))) {
    GemVal _t1132 = gem_table_new();
    gem_table_set(_t1132, gem_string("expr"), gem_string("gem_make_fn(gem_push_fn, NULL)"));
    gem_table_set(_t1132, gem_string("setup"), gem_string(""));
                                                        return _t1132;
                                                    } else {
    GemVal _t1133[] = {(*gem_v_defined_fns), gem_v_name};
                                                        if (gem_truthy(gem_fn_set_contains(NULL, _t1133, 2))) {
    GemVal _t1134 = gem_table_new();
    GemVal _t1135[] = {gem_v_name};
    gem_table_set(_t1134, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t1135, 1)), gem_string(", NULL)")));
    gem_table_set(_t1134, gem_string("setup"), gem_string(""));
                                                            return _t1134;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
    GemVal _t1136 = (*gem_v_mangle);
    GemVal _t1137[] = {gem_v_name};
                                GemVal gem_v_mname = _t1136.fn(_t1136.env, _t1137, 1);
    GemVal _t1138[] = {(*gem_v_boxed_vars), gem_v_name};
                                if (gem_truthy(gem_fn_set_contains(NULL, _t1138, 2))) {
    GemVal _t1139 = gem_table_new();
    GemVal _t1140[] = {gem_v_mname};
    gem_table_set(_t1139, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t1140, 1)), gem_string(")")));
    gem_table_set(_t1139, gem_string("setup"), gem_string(""));
                                    return _t1139;
                                }
    GemVal _t1141 = gem_table_new();
    gem_table_set(_t1141, gem_string("expr"), gem_v_mname);
    gem_table_set(_t1141, gem_string("setup"), gem_string(""));
                                return _t1141;
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
    GemVal _t1142 = (*gem_v_compile_call);
    GemVal _t1143[] = {gem_v_node};
                                    return _t1142.fn(_t1142.env, _t1143, 1);
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("binop")))) {
    GemVal _t1144 = (*gem_v_compile_binop);
    GemVal _t1145[] = {gem_v_node};
                                        return _t1144.fn(_t1144.env, _t1145, 1);
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("unop")))) {
    GemVal _t1146 = gem_v_node;
    GemVal _t1147 = (*gem_v_compile_expr);
    GemVal _t1148[] = {gem_table_get(_t1146, gem_string("expr"))};
                                            GemVal gem_v_r = _t1147.fn(_t1147.env, _t1148, 1);
    GemVal _t1149 = gem_v_node;
                                            if (gem_truthy(gem_eq(gem_table_get(_t1149, gem_string("op")), gem_string("-")))) {
    GemVal _t1150 = gem_table_new();
    GemVal _t1151 = gem_v_r;
    GemVal _t1152[] = {gem_table_get(_t1151, gem_string("expr"))};
    gem_table_set(_t1150, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t1152, 1)), gem_string(")")));
    GemVal _t1153 = gem_v_r;
    gem_table_set(_t1150, gem_string("setup"), gem_table_get(_t1153, gem_string("setup")));
                                                return _t1150;
                                            } else {
    GemVal _t1154 = gem_v_node;
                                                if (gem_truthy(gem_eq(gem_table_get(_t1154, gem_string("op")), gem_string("not")))) {
    GemVal _t1155 = gem_table_new();
    GemVal _t1156 = gem_v_r;
    GemVal _t1157[] = {gem_table_get(_t1156, gem_string("expr"))};
    gem_table_set(_t1155, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t1157, 1)), gem_string(")")));
    GemVal _t1158 = gem_v_r;
    gem_table_set(_t1155, gem_string("setup"), gem_table_get(_t1158, gem_string("setup")));
                                                    return _t1155;
                                                }
                                            }
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
    GemVal _t1159 = (*gem_v_compile_anon_fn);
    GemVal _t1160[] = {gem_v_node};
                                                return _t1159.fn(_t1159.env, _t1160, 1);
                                            } else {
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("table")))) {
    GemVal _t1161 = (*gem_v_compile_table);
    GemVal _t1162[] = {gem_v_node};
                                                    return _t1161.fn(_t1161.env, _t1162, 1);
                                                } else {
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("array")))) {
    GemVal _t1163 = (*gem_v_compile_array);
    GemVal _t1164[] = {gem_v_node};
                                                        return _t1163.fn(_t1163.env, _t1164, 1);
                                                    } else {
                                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot")))) {
    GemVal _t1165 = gem_v_node;
    GemVal _t1166 = (*gem_v_compile_expr);
    GemVal _t1167[] = {gem_table_get(_t1165, gem_string("object"))};
                                                            GemVal gem_v_r = _t1166.fn(_t1166.env, _t1167, 1);
    GemVal _t1168 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t1168.fn(_t1168.env, NULL, 0);
    GemVal _t1169 = gem_v_r;
    GemVal _t1170[] = {gem_v_t};
    GemVal _t1171 = gem_v_r;
    GemVal _t1172[] = {gem_table_get(_t1171, gem_string("expr"))};
                                                            GemVal gem_v_setup = gem_add(gem_table_get(_t1169, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1170, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1172, 1)), gem_string(";\n")));
    GemVal _t1173 = gem_v_node;
    GemVal _t1174[] = {gem_table_get(_t1173, gem_string("field"))};
                                                            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1174, 1);
    GemVal _t1175 = gem_table_new();
    GemVal _t1176[] = {gem_v_t};
    GemVal _t1177[] = {gem_v_escaped};
    gem_table_set(_t1175, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1176, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1177, 1)), gem_string("\"))")));
    gem_table_set(_t1175, gem_string("setup"), gem_v_setup);
                                                            return _t1175;
                                                        } else {
                                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("index")))) {
    GemVal _t1178 = gem_v_node;
    GemVal _t1179 = (*gem_v_compile_expr);
    GemVal _t1180[] = {gem_table_get(_t1178, gem_string("object"))};
                                                                GemVal gem_v_obj_r = _t1179.fn(_t1179.env, _t1180, 1);
    GemVal _t1181 = gem_v_node;
    GemVal _t1182 = (*gem_v_compile_expr);
    GemVal _t1183[] = {gem_table_get(_t1181, gem_string("key"))};
                                                                GemVal gem_v_key_r = _t1182.fn(_t1182.env, _t1183, 1);
    GemVal _t1184 = gem_table_new();
    GemVal _t1185 = gem_v_obj_r;
    GemVal _t1186[] = {gem_table_get(_t1185, gem_string("expr"))};
    GemVal _t1187 = gem_v_key_r;
    GemVal _t1188[] = {gem_table_get(_t1187, gem_string("expr"))};
    gem_table_set(_t1184, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t1186, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1188, 1)), gem_string(")")));
    GemVal _t1189 = gem_v_obj_r;
    GemVal _t1190 = gem_v_key_r;
    gem_table_set(_t1184, gem_string("setup"), gem_add(gem_table_get(_t1189, gem_string("setup")), gem_table_get(_t1190, gem_string("setup"))));
                                                                return _t1184;
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
    GemVal _t1191 = gem_table_new();
    gem_table_set(_t1191, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1191, gem_string("setup"), gem_add(gem_add(gem_string("/* unknown node: "), gem_v_tag), gem_string(" */\n")));
    return _t1191;
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
    GemVal _t1193 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t1193, gem_string("entries"));
    GemVal _t1194 = (*gem_v_tmp);
    GemVal gem_v_t = _t1194.fn(_t1194.env, NULL, 0);
    GemVal _t1195[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1195, 1)), gem_string(" = gem_table_new();\n"));
    {
        GemVal gem_v__for_items_21 = gem_v_entries;
        GemVal gem_v__for_i_21 = gem_int(0);
        while (1) {
            GemVal _t1196[] = {gem_v__for_items_21};
            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t1196, 1)))) break;
            GemVal gem_v_entry = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
    GemVal _t1197 = gem_v_entry;
    GemVal _t1198 = (*gem_v_compile_expr);
    GemVal _t1199[] = {gem_table_get(_t1197, gem_string("value"))};
            GemVal gem_v_val_r = _t1198.fn(_t1198.env, _t1199, 1);
    GemVal _t1200 = gem_v_entry;
    GemVal _t1201[] = {gem_table_get(_t1200, gem_string("key"))};
            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1201, 1);
    GemVal _t1202 = gem_v_val_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1202, gem_string("setup")));
    GemVal _t1203[] = {gem_v_t};
    GemVal _t1204[] = {gem_v_escaped};
    GemVal _t1205 = gem_v_val_r;
    GemVal _t1206[] = {gem_table_get(_t1205, gem_string("expr"))};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1203, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1204, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1206, 1)), gem_string(");\n")));
        }
    }
    GemVal _t1207 = gem_table_new();
    gem_table_set(_t1207, gem_string("expr"), gem_v_t);
    gem_table_set(_t1207, gem_string("setup"), gem_v_setup);
    return _t1207;
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
    GemVal _t1209 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t1209, gem_string("elements"));
    GemVal _t1210 = (*gem_v_tmp);
    GemVal gem_v_t = _t1210.fn(_t1210.env, NULL, 0);
    GemVal _t1211[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1211, 1)), gem_string(" = gem_table_new();\n"));
    {
        GemVal gem_v__for_i_22 = gem_int(0);
    GemVal _t1212[] = {gem_v_elements};
        GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1212, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
            GemVal gem_v_i = gem_v__for_i_22;
            gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
    GemVal _t1213 = (*gem_v_compile_expr);
    GemVal _t1214[] = {gem_table_get(gem_v_elements, gem_v_i)};
            GemVal gem_v_elem_r = _t1213.fn(_t1213.env, _t1214, 1);
    GemVal _t1215 = gem_v_elem_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1215, gem_string("setup")));
    GemVal _t1216[] = {gem_v_t};
    GemVal _t1217[] = {gem_v_i};
    GemVal _t1218 = gem_v_elem_r;
    GemVal _t1219[] = {gem_table_get(_t1218, gem_string("expr"))};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t1216, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t1217, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t1219, 1)), gem_string(");\n")));
        }
    }
    GemVal _t1220 = gem_table_new();
    gem_table_set(_t1220, gem_string("expr"), gem_v_t);
    gem_table_set(_t1220, gem_string("setup"), gem_v_setup);
    return _t1220;
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
    GemVal _t1222 = gem_v_node;
    GemVal _t1223[] = {gem_table_get(_t1222, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1223, 1);
    GemVal _t1224 = gem_v_node;
    GemVal _t1225 = (*gem_v_collect_free_vars);
    GemVal _t1226[] = {gem_table_get(_t1224, gem_string("body")), gem_v_inner_defined};
    GemVal gem_v_free = _t1225.fn(_t1225.env, _t1226, 2);
    GemVal _t1227[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t1227, 1);
    GemVal _t1228[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1228, 1), gem_int(0)))) {
    GemVal _t1229 = gem_table_new();
    GemVal _t1230 = (*gem_v_compile_closure_fn);
    GemVal _t1231[] = {gem_v_node, _t1229};
        GemVal gem_v_result = _t1230.fn(_t1230.env, _t1231, 2);
    GemVal _t1232 = gem_table_new();
    GemVal _t1233 = gem_v_result;
    GemVal _t1234[] = {gem_table_get(_t1233, gem_string("fn_name"))};
    gem_table_set(_t1232, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1234, 1)), gem_string(", NULL)")));
    gem_table_set(_t1232, gem_string("setup"), gem_string(""));
        return _t1232;
    }
    GemVal _t1235 = (*gem_v_compile_closure_fn);
    GemVal _t1236[] = {gem_v_node, gem_v_captures};
    GemVal gem_v_result = _t1235.fn(_t1235.env, _t1236, 2);
    GemVal gem_v_setup = gem_string("");
    GemVal _t1237 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t1237.fn(_t1237.env, NULL, 0);
    GemVal _t1238 = gem_v_result;
    GemVal _t1239[] = {gem_table_get(_t1238, gem_string("struct_name"))};
    GemVal _t1240[] = {gem_v_env_tmp};
    GemVal _t1241 = gem_v_result;
    GemVal _t1242[] = {gem_table_get(_t1241, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t1239, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t1240, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t1242, 1)), gem_string("));\n")));
    {
        GemVal gem_v__for_items_23 = gem_v_captures;
        GemVal gem_v__for_i_23 = gem_int(0);
        while (1) {
            GemVal _t1243[] = {gem_v__for_items_23};
            if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_len_fn(NULL, _t1243, 1)))) break;
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_23, gem_v__for_i_23);
            gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
    GemVal _t1244 = (*gem_v_mangle);
    GemVal _t1245[] = {gem_v_cap};
            GemVal gem_v_mc = _t1244.fn(_t1244.env, _t1245, 1);
    GemVal _t1246[] = {(*gem_v_boxed_vars), gem_v_cap};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1246, 2))) {
    GemVal _t1247[] = {gem_v_env_tmp};
    GemVal _t1248[] = {gem_v_mc};
    GemVal _t1249[] = {gem_v_mc};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1247, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1248, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1249, 1)), gem_string(";\n")));
            } else {
    GemVal _t1250[] = {gem_v_env_tmp};
    GemVal _t1251[] = {gem_v_mc};
    GemVal _t1252[] = {gem_v_mc};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1250, 1)), gem_string("->")), gem_to_string_fn(NULL, _t1251, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t1252, 1)), gem_string(";\n")));
            }
        }
    }
    GemVal _t1253 = gem_table_new();
    GemVal _t1254 = gem_v_result;
    GemVal _t1255[] = {gem_table_get(_t1254, gem_string("fn_name"))};
    GemVal _t1256[] = {gem_v_env_tmp};
    gem_table_set(_t1253, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t1255, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1256, 1)), gem_string(")")));
    gem_table_set(_t1253, gem_string("setup"), gem_v_setup);
    return _t1253;
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
    GemVal _t1258 = gem_v_node;
    GemVal gem_v_func = gem_table_get(_t1258, gem_string("func"));
    GemVal _t1259 = gem_v_node;
    GemVal gem_v_args = gem_table_get(_t1259, gem_string("args"));
    GemVal _t1260 = gem_v_func;
    GemVal _t1262;
    if (!gem_truthy(gem_eq(gem_table_get(_t1260, gem_string("tag")), gem_string("dot")))) {
        _t1262 = gem_eq(gem_table_get(_t1260, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t1261 = gem_v_func;
        _t1262 = gem_eq(gem_table_get(_t1261, gem_string("field")), gem_string("len"));
    }
    GemVal _t1264;
    if (!gem_truthy(_t1262)) {
        _t1264 = _t1262;
    } else {
        GemVal _t1263[] = {gem_v_args};
        _t1264 = gem_eq(gem_len_fn(NULL, _t1263, 1), gem_int(0));
    }
    if (gem_truthy(_t1264)) {
    GemVal _t1265 = gem_v_func;
    GemVal _t1266 = (*gem_v_compile_expr);
    GemVal _t1267[] = {gem_table_get(_t1265, gem_string("object"))};
        GemVal gem_v_obj_r = _t1266.fn(_t1266.env, _t1267, 1);
    GemVal _t1268 = gem_table_new();
    GemVal _t1269 = gem_v_obj_r;
    GemVal _t1270[] = {gem_table_get(_t1269, gem_string("expr"))};
    gem_table_set(_t1268, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t1270, 1)), gem_string(")")));
    GemVal _t1271 = gem_v_obj_r;
    gem_table_set(_t1268, gem_string("setup"), gem_table_get(_t1271, gem_string("setup")));
        return _t1268;
    }
    GemVal _t1272 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t1272, gem_string("tag")), gem_string("var")))) {
    GemVal _t1273 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t1273, gem_string("name"));
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
    GemVal _t1274 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t1274, gem_string("line"));
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
                gem_v_line = gem_int(0);
            }
    GemVal _t1275[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t1275, 1);
            GemVal gem_v_arg_setups = gem_string("");
    GemVal _t1276 = gem_table_new();
            GemVal gem_v_arg_exprs = _t1276;
            {
                GemVal gem_v__for_i_24 = gem_int(0);
    GemVal _t1277[] = {gem_v_args};
                GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1277, 1);
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
                    GemVal gem_v_i = gem_v__for_i_24;
                    gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
    GemVal _t1278 = (*gem_v_compile_expr);
    GemVal _t1279[] = {gem_table_get(gem_v_args, gem_v_i)};
                    GemVal gem_v_r = _t1278.fn(_t1278.env, _t1279, 1);
    GemVal _t1280 = gem_v_r;
                    gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1280, gem_string("setup")));
    GemVal _t1281 = gem_v_r;
    GemVal _t1282[] = {gem_v_arg_exprs, gem_table_get(_t1281, gem_string("expr"))};
                    (void)(gem_push_fn(NULL, _t1282, 2));
                }
            }
    GemVal _t1283[] = {gem_v_args};
            GemVal gem_v_argc = gem_len_fn(NULL, _t1283, 1);
            if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1284 = gem_table_new();
    GemVal _t1285[] = {gem_v_escaped_file};
    GemVal _t1286[] = {gem_v_line};
    gem_table_set(_t1284, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1285, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1286, 1)), gem_string(", NULL, 0)")));
    gem_table_set(_t1284, gem_string("setup"), gem_v_arg_setups);
                return _t1284;
            }
    GemVal _t1287 = (*gem_v_tmp);
            GemVal gem_v_t = _t1287.fn(_t1287.env, NULL, 0);
            GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
            {
                GemVal gem_v__for_i_25 = gem_int(1);
                GemVal gem_v__for_limit_25 = gem_v_argc;
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
                    GemVal gem_v_i = gem_v__for_i_25;
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
                    gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                }
            }
    GemVal _t1288[] = {gem_v_t};
    GemVal _t1289[] = {gem_v_arr};
            GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1288, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1289, 1)), gem_string("};\n")));
    GemVal _t1290[] = {gem_v_escaped_file};
    GemVal _t1291[] = {gem_v_line};
    GemVal _t1292[] = {gem_v_t};
    GemVal _t1293[] = {gem_v_argc};
            GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t1290, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t1291, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1292, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1293, 1)), gem_string(")"));
    GemVal _t1294 = gem_table_new();
    gem_table_set(_t1294, gem_string("expr"), gem_v_call);
    gem_table_set(_t1294, gem_string("setup"), gem_v_setup);
            return _t1294;
        } else {
    GemVal _t1295;
    if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
        _t1295 = gem_eq(gem_v_name, gem_string("print"));
    } else {
        _t1295 = gem_eq(gem_v_name, gem_string("len"));
    }
    GemVal _t1296;
    if (gem_truthy(_t1295)) {
        _t1296 = _t1295;
    } else {
        _t1296 = gem_eq(gem_v_name, gem_string("type"));
    }
    GemVal _t1297;
    if (gem_truthy(_t1296)) {
        _t1297 = _t1296;
    } else {
        _t1297 = gem_eq(gem_v_name, gem_string("to_string"));
    }
    GemVal _t1298;
    if (gem_truthy(_t1297)) {
        _t1298 = _t1297;
    } else {
        _t1298 = gem_eq(gem_v_name, gem_string("push"));
    }
            if (gem_truthy(_t1298)) {
                GemVal gem_v_fn_name = gem_string("");
                if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
                    gem_v_fn_name = gem_string("gem_print");
                } else {
                    if (gem_truthy(gem_eq(gem_v_name, gem_string("len")))) {
                        gem_v_fn_name = gem_string("gem_len_fn");
                    } else {
                        if (gem_truthy(gem_eq(gem_v_name, gem_string("type")))) {
                            gem_v_fn_name = gem_string("gem_type_fn");
                        } else {
                            if (gem_truthy(gem_eq(gem_v_name, gem_string("to_string")))) {
                                gem_v_fn_name = gem_string("gem_to_string_fn");
                            } else {
                                if (gem_truthy(gem_eq(gem_v_name, gem_string("push")))) {
                                    gem_v_fn_name = gem_string("gem_push_fn");
                                }
                            }
                        }
                    }
                }
                GemVal gem_v_arg_setups = gem_string("");
    GemVal _t1299 = gem_table_new();
                GemVal gem_v_arg_exprs = _t1299;
                {
                    GemVal gem_v__for_i_26 = gem_int(0);
    GemVal _t1300[] = {gem_v_args};
                    GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1300, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
                        GemVal gem_v_i = gem_v__for_i_26;
                        gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
    GemVal _t1301 = (*gem_v_compile_expr);
    GemVal _t1302[] = {gem_table_get(gem_v_args, gem_v_i)};
                        GemVal gem_v_r = _t1301.fn(_t1301.env, _t1302, 1);
    GemVal _t1303 = gem_v_r;
                        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1303, gem_string("setup")));
    GemVal _t1304 = gem_v_r;
    GemVal _t1305[] = {gem_v_arg_exprs, gem_table_get(_t1304, gem_string("expr"))};
                        (void)(gem_push_fn(NULL, _t1305, 2));
                    }
                }
    GemVal _t1306[] = {gem_v_args};
                GemVal gem_v_argc = gem_len_fn(NULL, _t1306, 1);
                if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1307 = gem_table_new();
    GemVal _t1308[] = {gem_v_fn_name};
    gem_table_set(_t1307, gem_string("expr"), gem_add(gem_to_string_fn(NULL, _t1308, 1), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1307, gem_string("setup"), gem_v_arg_setups);
                    return _t1307;
                }
    GemVal _t1309 = (*gem_v_tmp);
                GemVal gem_v_t = _t1309.fn(_t1309.env, NULL, 0);
                GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
                {
                    GemVal gem_v__for_i_27 = gem_int(1);
                    GemVal gem_v__for_limit_27 = gem_v_argc;
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
                        GemVal gem_v_i = gem_v__for_i_27;
                        gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
                        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                    }
                }
    GemVal _t1310[] = {gem_v_t};
    GemVal _t1311[] = {gem_v_arr};
                GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1310, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1311, 1)), gem_string("};\n")));
    GemVal _t1312 = gem_table_new();
    GemVal _t1313[] = {gem_v_fn_name};
    GemVal _t1314[] = {gem_v_t};
    GemVal _t1315[] = {gem_v_argc};
    gem_table_set(_t1312, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1313, 1), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1314, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1315, 1)), gem_string(")")));
    gem_table_set(_t1312, gem_string("setup"), gem_v_setup);
                return _t1312;
            } else {
    GemVal _t1316[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t1316, 2))) {
                    GemVal gem_v_arg_setups = gem_string("");
    GemVal _t1317 = gem_table_new();
                    GemVal gem_v_arg_exprs = _t1317;
                    {
                        GemVal gem_v__for_i_28 = gem_int(0);
    GemVal _t1318[] = {gem_v_args};
                        GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1318, 1);
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
                            GemVal gem_v_i = gem_v__for_i_28;
                            gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
    GemVal _t1319 = (*gem_v_compile_expr);
    GemVal _t1320[] = {gem_table_get(gem_v_args, gem_v_i)};
                            GemVal gem_v_r = _t1319.fn(_t1319.env, _t1320, 1);
    GemVal _t1321 = gem_v_r;
                            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1321, gem_string("setup")));
    GemVal _t1322 = gem_v_r;
    GemVal _t1323[] = {gem_v_arg_exprs, gem_table_get(_t1322, gem_string("expr"))};
                            (void)(gem_push_fn(NULL, _t1323, 2));
                        }
                    }
    GemVal _t1324[] = {gem_v_args};
                    GemVal gem_v_argc = gem_len_fn(NULL, _t1324, 1);
                    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1325 = gem_table_new();
    GemVal _t1326[] = {gem_v_name};
    gem_table_set(_t1325, gem_string("expr"), gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1326, 1)), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1325, gem_string("setup"), gem_v_arg_setups);
                        return _t1325;
                    }
    GemVal _t1327 = (*gem_v_tmp);
                    GemVal gem_v_t = _t1327.fn(_t1327.env, NULL, 0);
                    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
                    {
                        GemVal gem_v__for_i_29 = gem_int(1);
                        GemVal gem_v__for_limit_29 = gem_v_argc;
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_v__for_limit_29))) break;
                            GemVal gem_v_i = gem_v__for_i_29;
                            gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
                            gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                        }
                    }
    GemVal _t1328[] = {gem_v_t};
    GemVal _t1329[] = {gem_v_arr};
                    GemVal gem_v_setup = gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1328, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1329, 1)), gem_string("};\n")));
    GemVal _t1330 = gem_table_new();
    GemVal _t1331[] = {gem_v_name};
    GemVal _t1332[] = {gem_v_t};
    GemVal _t1333[] = {gem_v_argc};
    gem_table_set(_t1330, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t1331, 1)), gem_string("(NULL, ")), gem_to_string_fn(NULL, _t1332, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1333, 1)), gem_string(")")));
    gem_table_set(_t1330, gem_string("setup"), gem_v_setup);
                    return _t1330;
                }
            }
        }
    }
    GemVal _t1334 = (*gem_v_compile_expr);
    GemVal _t1335[] = {gem_v_func};
    GemVal gem_v_func_r = _t1334.fn(_t1334.env, _t1335, 1);
    GemVal gem_v_arg_setups = gem_string("");
    GemVal _t1336 = gem_table_new();
    GemVal gem_v_arg_exprs = _t1336;
    {
        GemVal gem_v__for_i_30 = gem_int(0);
    GemVal _t1337[] = {gem_v_args};
        GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t1337, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
            GemVal gem_v_i = gem_v__for_i_30;
            gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
    GemVal _t1338 = (*gem_v_compile_expr);
    GemVal _t1339[] = {gem_table_get(gem_v_args, gem_v_i)};
            GemVal gem_v_r = _t1338.fn(_t1338.env, _t1339, 1);
    GemVal _t1340 = gem_v_r;
            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1340, gem_string("setup")));
    GemVal _t1341 = gem_v_r;
    GemVal _t1342[] = {gem_v_arg_exprs, gem_table_get(_t1341, gem_string("expr"))};
            (void)(gem_push_fn(NULL, _t1342, 2));
        }
    }
    GemVal _t1343 = gem_v_func_r;
    GemVal gem_v_setup = gem_add(gem_table_get(_t1343, gem_string("setup")), gem_v_arg_setups);
    GemVal _t1344[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t1344, 1);
    GemVal _t1345 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t1345.fn(_t1345.env, NULL, 0);
    GemVal _t1346[] = {gem_v_tmp_fn};
    GemVal _t1347 = gem_v_func_r;
    GemVal _t1348[] = {gem_table_get(_t1347, gem_string("expr"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1346, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1348, 1)), gem_string(";\n")));
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1349 = gem_table_new();
    GemVal _t1350[] = {gem_v_tmp_fn};
    GemVal _t1351[] = {gem_v_tmp_fn};
    gem_table_set(_t1349, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1350, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1351, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t1349, gem_string("setup"), gem_v_setup);
        return _t1349;
    }
    GemVal _t1352 = (*gem_v_tmp);
    GemVal gem_v_tmp_args = _t1352.fn(_t1352.env, NULL, 0);
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
    {
        GemVal gem_v__for_i_31 = gem_int(1);
        GemVal gem_v__for_limit_31 = gem_v_argc;
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_v__for_limit_31))) break;
            GemVal gem_v_i = gem_v__for_i_31;
            gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
            gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
        }
    }
    GemVal _t1353[] = {gem_v_tmp_args};
    GemVal _t1354[] = {gem_v_arr};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1353, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t1354, 1)), gem_string("};\n")));
    GemVal _t1355[] = {gem_v_tmp_fn};
    GemVal _t1356[] = {gem_v_tmp_fn};
    GemVal _t1357[] = {gem_v_tmp_args};
    GemVal _t1358[] = {gem_v_argc};
    GemVal gem_v_call = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1355, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t1356, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t1357, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1358, 1)), gem_string(")"));
    GemVal _t1359 = gem_table_new();
    gem_table_set(_t1359, gem_string("expr"), gem_v_call);
    gem_table_set(_t1359, gem_string("setup"), gem_v_setup);
    return _t1359;
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
    GemVal _t1361 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t1361, gem_string("op"));
    GemVal _t1362 = gem_v_node;
    GemVal _t1363 = (*gem_v_compile_expr);
    GemVal _t1364[] = {gem_table_get(_t1362, gem_string("left"))};
    GemVal gem_v_lc_r = _t1363.fn(_t1363.env, _t1364, 1);
    GemVal _t1365 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t1365, gem_string("expr"));
    GemVal _t1366 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t1366, gem_string("setup"));
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
    GemVal _t1367 = gem_v_node;
    GemVal _t1368 = (*gem_v_compile_expr);
    GemVal _t1369[] = {gem_table_get(_t1367, gem_string("right"))};
        GemVal gem_v_rc_r = _t1368.fn(_t1368.env, _t1369, 1);
    GemVal _t1370 = gem_table_new();
    GemVal _t1371[] = {gem_v_lc};
    GemVal _t1372 = gem_v_rc_r;
    GemVal _t1373[] = {gem_table_get(_t1372, gem_string("expr"))};
    gem_table_set(_t1370, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t1371, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1373, 1)), gem_string(")")));
    GemVal _t1374 = gem_v_rc_r;
    gem_table_set(_t1370, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1374, gem_string("setup"))));
        return _t1370;
    } else {
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
    GemVal _t1375 = gem_v_node;
    GemVal _t1376 = (*gem_v_compile_expr);
    GemVal _t1377[] = {gem_table_get(_t1375, gem_string("right"))};
            GemVal gem_v_rc_r = _t1376.fn(_t1376.env, _t1377, 1);
    GemVal _t1378 = gem_table_new();
    GemVal _t1379[] = {gem_v_lc};
    GemVal _t1380 = gem_v_rc_r;
    GemVal _t1381[] = {gem_table_get(_t1380, gem_string("expr"))};
    gem_table_set(_t1378, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t1379, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1381, 1)), gem_string(")")));
    GemVal _t1382 = gem_v_rc_r;
    gem_table_set(_t1378, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1382, gem_string("setup"))));
            return _t1378;
        } else {
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
    GemVal _t1383 = gem_v_node;
    GemVal _t1384 = (*gem_v_compile_expr);
    GemVal _t1385[] = {gem_table_get(_t1383, gem_string("right"))};
                GemVal gem_v_rc_r = _t1384.fn(_t1384.env, _t1385, 1);
    GemVal _t1386 = gem_table_new();
    GemVal _t1387[] = {gem_v_lc};
    GemVal _t1388 = gem_v_rc_r;
    GemVal _t1389[] = {gem_table_get(_t1388, gem_string("expr"))};
    gem_table_set(_t1386, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t1387, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1389, 1)), gem_string(")")));
    GemVal _t1390 = gem_v_rc_r;
    gem_table_set(_t1386, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1390, gem_string("setup"))));
                return _t1386;
            } else {
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
    GemVal _t1391 = gem_v_node;
    GemVal _t1392 = (*gem_v_compile_expr);
    GemVal _t1393[] = {gem_table_get(_t1391, gem_string("right"))};
                    GemVal gem_v_rc_r = _t1392.fn(_t1392.env, _t1393, 1);
    GemVal _t1394 = gem_table_new();
    GemVal _t1395[] = {gem_v_lc};
    GemVal _t1396 = gem_v_rc_r;
    GemVal _t1397[] = {gem_table_get(_t1396, gem_string("expr"))};
    gem_table_set(_t1394, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t1395, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1397, 1)), gem_string(")")));
    GemVal _t1398 = gem_v_rc_r;
    gem_table_set(_t1394, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1398, gem_string("setup"))));
                    return _t1394;
                } else {
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
    GemVal _t1399 = gem_v_node;
    GemVal _t1400 = (*gem_v_compile_expr);
    GemVal _t1401[] = {gem_table_get(_t1399, gem_string("right"))};
                        GemVal gem_v_rc_r = _t1400.fn(_t1400.env, _t1401, 1);
    GemVal _t1402 = gem_table_new();
    GemVal _t1403[] = {gem_v_lc};
    GemVal _t1404 = gem_v_rc_r;
    GemVal _t1405[] = {gem_table_get(_t1404, gem_string("expr"))};
    gem_table_set(_t1402, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t1403, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1405, 1)), gem_string(")")));
    GemVal _t1406 = gem_v_rc_r;
    gem_table_set(_t1402, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1406, gem_string("setup"))));
                        return _t1402;
                    } else {
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
    GemVal _t1407 = gem_v_node;
    GemVal _t1408 = (*gem_v_compile_expr);
    GemVal _t1409[] = {gem_table_get(_t1407, gem_string("right"))};
                            GemVal gem_v_rc_r = _t1408.fn(_t1408.env, _t1409, 1);
    GemVal _t1410 = gem_table_new();
    GemVal _t1411[] = {gem_v_lc};
    GemVal _t1412 = gem_v_rc_r;
    GemVal _t1413[] = {gem_table_get(_t1412, gem_string("expr"))};
    gem_table_set(_t1410, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t1411, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1413, 1)), gem_string(")")));
    GemVal _t1414 = gem_v_rc_r;
    gem_table_set(_t1410, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1414, gem_string("setup"))));
                            return _t1410;
                        } else {
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
    GemVal _t1415 = gem_v_node;
    GemVal _t1416 = (*gem_v_compile_expr);
    GemVal _t1417[] = {gem_table_get(_t1415, gem_string("right"))};
                                GemVal gem_v_rc_r = _t1416.fn(_t1416.env, _t1417, 1);
    GemVal _t1418 = gem_table_new();
    GemVal _t1419[] = {gem_v_lc};
    GemVal _t1420 = gem_v_rc_r;
    GemVal _t1421[] = {gem_table_get(_t1420, gem_string("expr"))};
    gem_table_set(_t1418, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t1419, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1421, 1)), gem_string(")")));
    GemVal _t1422 = gem_v_rc_r;
    gem_table_set(_t1418, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1422, gem_string("setup"))));
                                return _t1418;
                            } else {
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
    GemVal _t1423 = gem_v_node;
    GemVal _t1424 = (*gem_v_compile_expr);
    GemVal _t1425[] = {gem_table_get(_t1423, gem_string("right"))};
                                    GemVal gem_v_rc_r = _t1424.fn(_t1424.env, _t1425, 1);
    GemVal _t1426 = gem_table_new();
    GemVal _t1427[] = {gem_v_lc};
    GemVal _t1428 = gem_v_rc_r;
    GemVal _t1429[] = {gem_table_get(_t1428, gem_string("expr"))};
    gem_table_set(_t1426, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t1427, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1429, 1)), gem_string(")")));
    GemVal _t1430 = gem_v_rc_r;
    gem_table_set(_t1426, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1430, gem_string("setup"))));
                                    return _t1426;
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
    GemVal _t1431 = gem_v_node;
    GemVal _t1432 = (*gem_v_compile_expr);
    GemVal _t1433[] = {gem_table_get(_t1431, gem_string("right"))};
                                        GemVal gem_v_rc_r = _t1432.fn(_t1432.env, _t1433, 1);
    GemVal _t1434 = gem_table_new();
    GemVal _t1435[] = {gem_v_lc};
    GemVal _t1436 = gem_v_rc_r;
    GemVal _t1437[] = {gem_table_get(_t1436, gem_string("expr"))};
    gem_table_set(_t1434, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t1435, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1437, 1)), gem_string(")")));
    GemVal _t1438 = gem_v_rc_r;
    gem_table_set(_t1434, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1438, gem_string("setup"))));
                                        return _t1434;
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
    GemVal _t1439 = gem_v_node;
    GemVal _t1440 = (*gem_v_compile_expr);
    GemVal _t1441[] = {gem_table_get(_t1439, gem_string("right"))};
                                            GemVal gem_v_rc_r = _t1440.fn(_t1440.env, _t1441, 1);
    GemVal _t1442 = gem_table_new();
    GemVal _t1443[] = {gem_v_lc};
    GemVal _t1444 = gem_v_rc_r;
    GemVal _t1445[] = {gem_table_get(_t1444, gem_string("expr"))};
    gem_table_set(_t1442, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t1443, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1445, 1)), gem_string(")")));
    GemVal _t1446 = gem_v_rc_r;
    gem_table_set(_t1442, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1446, gem_string("setup"))));
                                            return _t1442;
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
    GemVal _t1447 = gem_v_node;
    GemVal _t1448 = (*gem_v_compile_expr);
    GemVal _t1449[] = {gem_table_get(_t1447, gem_string("right"))};
                                                GemVal gem_v_rc_r = _t1448.fn(_t1448.env, _t1449, 1);
    GemVal _t1450 = gem_table_new();
    GemVal _t1451[] = {gem_v_lc};
    GemVal _t1452 = gem_v_rc_r;
    GemVal _t1453[] = {gem_table_get(_t1452, gem_string("expr"))};
    gem_table_set(_t1450, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t1451, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1453, 1)), gem_string(")")));
    GemVal _t1454 = gem_v_rc_r;
    gem_table_set(_t1450, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1454, gem_string("setup"))));
                                                return _t1450;
                                            } else {
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
    GemVal _t1455 = gem_v_node;
    GemVal _t1456 = (*gem_v_compile_expr);
    GemVal _t1457[] = {gem_table_get(_t1455, gem_string("right"))};
                                                    GemVal gem_v_rc_r = _t1456.fn(_t1456.env, _t1457, 1);
    GemVal _t1458 = (*gem_v_tmp);
                                                    GemVal gem_v_t = _t1458.fn(_t1458.env, NULL, 0);
                                                    GemVal gem_v_setup = gem_v_ls;
    GemVal _t1459[] = {gem_v_t};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1459, 1)), gem_string(";\n")));
    GemVal _t1460[] = {gem_v_lc};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t1460, 1)), gem_string(")) {\n")));
    GemVal _t1461[] = {gem_v_t};
    GemVal _t1462[] = {gem_v_lc};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1461, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1462, 1)), gem_string(";\n")));
                                                    gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
    GemVal _t1463 = gem_v_rc_r;
                                                    if (gem_truthy(gem_neq(gem_table_get(_t1463, gem_string("setup")), gem_string("")))) {
    GemVal _t1464 = gem_v_rc_r;
    GemVal _t1465[] = {gem_table_get(_t1464, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_fn_str_replace_all(NULL, _t1465, 3));
                                                    }
    GemVal _t1466[] = {gem_v_t};
    GemVal _t1467 = gem_v_rc_r;
    GemVal _t1468[] = {gem_table_get(_t1467, gem_string("expr"))};
                                                    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1466, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1468, 1)), gem_string(";\n")));
                                                    gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
    GemVal _t1469 = gem_table_new();
    gem_table_set(_t1469, gem_string("expr"), gem_v_t);
    gem_table_set(_t1469, gem_string("setup"), gem_v_setup);
                                                    return _t1469;
                                                } else {
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
    GemVal _t1470 = gem_v_node;
    GemVal _t1471 = (*gem_v_compile_expr);
    GemVal _t1472[] = {gem_table_get(_t1470, gem_string("right"))};
                                                        GemVal gem_v_rc_r = _t1471.fn(_t1471.env, _t1472, 1);
    GemVal _t1473 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1473.fn(_t1473.env, NULL, 0);
                                                        GemVal gem_v_setup = gem_v_ls;
    GemVal _t1474[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1474, 1)), gem_string(";\n")));
    GemVal _t1475[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t1475, 1)), gem_string(")) {\n")));
    GemVal _t1476[] = {gem_v_t};
    GemVal _t1477[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1476, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1477, 1)), gem_string(";\n")));
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
    GemVal _t1478 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t1478, gem_string("setup")), gem_string("")))) {
    GemVal _t1479 = gem_v_rc_r;
    GemVal _t1480[] = {gem_table_get(_t1479, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_fn_str_replace_all(NULL, _t1480, 3));
                                                        }
    GemVal _t1481[] = {gem_v_t};
    GemVal _t1482 = gem_v_rc_r;
    GemVal _t1483[] = {gem_table_get(_t1482, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t1481, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1483, 1)), gem_string(";\n")));
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
    GemVal _t1484 = gem_table_new();
    gem_table_set(_t1484, gem_string("expr"), gem_v_t);
    gem_table_set(_t1484, gem_string("setup"), gem_v_setup);
                                                        return _t1484;
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
    GemVal _t1485 = gem_v_node;
    GemVal _t1486 = (*gem_v_compile_expr);
    GemVal _t1487[] = {gem_table_get(_t1485, gem_string("right"))};
    GemVal gem_v_rc_r = _t1486.fn(_t1486.env, _t1487, 1);
    GemVal _t1488 = gem_table_new();
    gem_table_set(_t1488, gem_string("expr"), gem_string("GEM_NIL"));
    GemVal _t1489 = gem_v_rc_r;
    gem_table_set(_t1488, gem_string("setup"), gem_add(gem_add(gem_add(gem_add(gem_v_ls, gem_table_get(_t1489, gem_string("setup"))), gem_string("/* unknown op: ")), gem_v_op), gem_string(" */\n")));
    return _t1488;
}

struct _closure__anon_36 {
    GemVal *gem_v_compile_stmt;
};
static GemVal _anon_36(void *_env, GemVal *args, int argc) {
    struct _closure__anon_36 *_cls = (struct _closure__anon_36 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_out = gem_string("");
    {
        GemVal gem_v__for_items_32 = gem_v_stmts;
        GemVal gem_v__for_i_32 = gem_int(0);
        while (1) {
            GemVal _t1491[] = {gem_v__for_items_32};
            if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_len_fn(NULL, _t1491, 1)))) break;
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_32, gem_v__for_i_32);
            gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
    GemVal _t1492 = (*gem_v_compile_stmt);
    GemVal _t1493[] = {gem_v_stmt, gem_v_indent};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1492.fn(_t1492.env, _t1493, 2)), gem_string("\n"));
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
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t1495[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1495, 1);
    GemVal _t1496 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1496, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t1497 = gem_v_node;
    GemVal _t1498 = (*gem_v_compile_expr);
    GemVal _t1499[] = {gem_table_get(_t1497, gem_string("value"))};
        GemVal gem_v_r = _t1498.fn(_t1498.env, _t1499, 1);
    GemVal _t1500 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1500, gem_string("name"));
    GemVal _t1501 = (*gem_v_mangle);
    GemVal _t1502[] = {gem_v_name};
        GemVal gem_v_mname = _t1501.fn(_t1501.env, _t1502, 1);
    GemVal _t1503[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1503, 2))) {
    GemVal _t1504 = gem_v_r;
    GemVal _t1505[] = {gem_table_get(_t1504, gem_string("setup"))};
    GemVal _t1506[] = {gem_v_p};
    GemVal _t1507[] = {gem_v_mname};
    GemVal _t1508[] = {gem_v_p};
    GemVal _t1509[] = {gem_v_mname};
    GemVal _t1510 = gem_v_r;
    GemVal _t1511[] = {gem_table_get(_t1510, gem_string("expr"))};
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1505, 1), gem_to_string_fn(NULL, _t1506, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t1507, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1508, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1509, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1511, 1)), gem_string(";"));
        }
    GemVal _t1512 = gem_v_r;
    GemVal _t1513[] = {gem_table_get(_t1512, gem_string("setup"))};
    GemVal _t1514[] = {gem_v_p};
    GemVal _t1515[] = {gem_v_mname};
    GemVal _t1516 = gem_v_r;
    GemVal _t1517[] = {gem_table_get(_t1516, gem_string("expr"))};
        return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1513, 1), gem_to_string_fn(NULL, _t1514, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1515, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1517, 1)), gem_string(";"));
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t1518 = gem_v_node;
    GemVal _t1519 = (*gem_v_compile_expr);
    GemVal _t1520[] = {gem_table_get(_t1518, gem_string("value"))};
            GemVal gem_v_r = _t1519.fn(_t1519.env, _t1520, 1);
    GemVal _t1521 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1521, gem_string("name"));
    GemVal _t1522 = (*gem_v_mangle);
    GemVal _t1523[] = {gem_v_name};
            GemVal gem_v_mname = _t1522.fn(_t1522.env, _t1523, 1);
    GemVal _t1524[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1524, 2))) {
    GemVal _t1525 = gem_v_r;
    GemVal _t1526[] = {gem_table_get(_t1525, gem_string("setup"))};
    GemVal _t1527[] = {gem_v_p};
    GemVal _t1528[] = {gem_v_mname};
    GemVal _t1529 = gem_v_r;
    GemVal _t1530[] = {gem_table_get(_t1529, gem_string("expr"))};
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1526, 1), gem_to_string_fn(NULL, _t1527, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1528, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1530, 1)), gem_string(";"));
            }
    GemVal _t1531 = gem_v_r;
    GemVal _t1532[] = {gem_table_get(_t1531, gem_string("setup"))};
    GemVal _t1533[] = {gem_v_p};
    GemVal _t1534[] = {gem_v_mname};
    GemVal _t1535 = gem_v_r;
    GemVal _t1536[] = {gem_table_get(_t1535, gem_string("expr"))};
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1532, 1), gem_to_string_fn(NULL, _t1533, 1)), gem_to_string_fn(NULL, _t1534, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1536, 1)), gem_string(";"));
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
    GemVal _t1537 = gem_v_node;
    GemVal _t1538 = (*gem_v_compile_expr);
    GemVal _t1539[] = {gem_table_get(_t1537, gem_string("object"))};
                GemVal gem_v_obj_r = _t1538.fn(_t1538.env, _t1539, 1);
    GemVal _t1540 = gem_v_node;
    GemVal _t1541 = (*gem_v_compile_expr);
    GemVal _t1542[] = {gem_table_get(_t1540, gem_string("value"))};
                GemVal gem_v_val_r = _t1541.fn(_t1541.env, _t1542, 1);
    GemVal _t1543 = gem_v_node;
    GemVal _t1544[] = {gem_table_get(_t1543, gem_string("field"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1544, 1);
    GemVal _t1545 = gem_v_obj_r;
    GemVal _t1546[] = {gem_table_get(_t1545, gem_string("setup"))};
    GemVal _t1547 = gem_v_val_r;
    GemVal _t1548[] = {gem_table_get(_t1547, gem_string("setup"))};
    GemVal _t1549[] = {gem_v_p};
    GemVal _t1550 = gem_v_obj_r;
    GemVal _t1551[] = {gem_table_get(_t1550, gem_string("expr"))};
    GemVal _t1552[] = {gem_v_escaped};
    GemVal _t1553 = gem_v_val_r;
    GemVal _t1554[] = {gem_table_get(_t1553, gem_string("expr"))};
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1546, 1), gem_to_string_fn(NULL, _t1548, 1)), gem_to_string_fn(NULL, _t1549, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t1551, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t1552, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t1554, 1)), gem_string(");"));
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("index_assign")))) {
    GemVal _t1555 = gem_v_node;
    GemVal _t1556 = (*gem_v_compile_expr);
    GemVal _t1557[] = {gem_table_get(_t1555, gem_string("object"))};
                    GemVal gem_v_obj_r = _t1556.fn(_t1556.env, _t1557, 1);
    GemVal _t1558 = gem_v_node;
    GemVal _t1559 = (*gem_v_compile_expr);
    GemVal _t1560[] = {gem_table_get(_t1558, gem_string("key"))};
                    GemVal gem_v_key_r = _t1559.fn(_t1559.env, _t1560, 1);
    GemVal _t1561 = gem_v_node;
    GemVal _t1562 = (*gem_v_compile_expr);
    GemVal _t1563[] = {gem_table_get(_t1561, gem_string("value"))};
                    GemVal gem_v_val_r = _t1562.fn(_t1562.env, _t1563, 1);
    GemVal _t1564 = gem_v_obj_r;
    GemVal _t1565[] = {gem_table_get(_t1564, gem_string("setup"))};
    GemVal _t1566 = gem_v_key_r;
    GemVal _t1567[] = {gem_table_get(_t1566, gem_string("setup"))};
    GemVal _t1568 = gem_v_val_r;
    GemVal _t1569[] = {gem_table_get(_t1568, gem_string("setup"))};
    GemVal _t1570[] = {gem_v_p};
    GemVal _t1571 = gem_v_obj_r;
    GemVal _t1572[] = {gem_table_get(_t1571, gem_string("expr"))};
    GemVal _t1573 = gem_v_key_r;
    GemVal _t1574[] = {gem_table_get(_t1573, gem_string("expr"))};
    GemVal _t1575 = gem_v_val_r;
    GemVal _t1576[] = {gem_table_get(_t1575, gem_string("expr"))};
                    return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1565, 1), gem_to_string_fn(NULL, _t1567, 1)), gem_to_string_fn(NULL, _t1569, 1)), gem_to_string_fn(NULL, _t1570, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t1572, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1574, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1576, 1)), gem_string(");"));
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t1577 = (*gem_v_compile_if);
    GemVal _t1578[] = {gem_v_node, gem_v_indent};
                        return _t1577.fn(_t1577.env, _t1578, 2);
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t1579 = (*gem_v_compile_while);
    GemVal _t1580[] = {gem_v_node, gem_v_indent};
                            return _t1579.fn(_t1579.env, _t1580, 2);
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t1581 = (*gem_v_compile_match);
    GemVal _t1582[] = {gem_v_node, gem_v_indent};
                                return _t1581.fn(_t1581.env, _t1582, 2);
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("break")))) {
                                    return gem_add(gem_v_p, gem_string("break;"));
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("continue")))) {
                                        return gem_add(gem_v_p, gem_string("continue;"));
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t1583 = gem_v_node;
                                            if (gem_truthy(gem_neq(gem_table_get(_t1583, gem_string("value")), GEM_NIL))) {
    GemVal _t1584 = gem_v_node;
    GemVal _t1585 = (*gem_v_compile_expr);
    GemVal _t1586[] = {gem_table_get(_t1584, gem_string("value"))};
                                                GemVal gem_v_r = _t1585.fn(_t1585.env, _t1586, 1);
    GemVal _t1587 = gem_v_r;
    GemVal _t1588[] = {gem_table_get(_t1587, gem_string("setup"))};
    GemVal _t1589[] = {gem_v_p};
    GemVal _t1590 = gem_v_r;
    GemVal _t1591[] = {gem_table_get(_t1590, gem_string("expr"))};
                                                return gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1588, 1), gem_to_string_fn(NULL, _t1589, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1591, 1)), gem_string(";"));
                                            }
    GemVal _t1592[] = {gem_v_p};
                                            return gem_add(gem_to_string_fn(NULL, _t1592, 1), gem_string("return GEM_NIL;"));
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
    GemVal _t1593 = (*gem_v_compile_expr);
    GemVal _t1594[] = {gem_v_node};
                                                GemVal gem_v_r = _t1593.fn(_t1593.env, _t1594, 1);
    GemVal _t1595 = gem_v_r;
    GemVal _t1596[] = {gem_table_get(_t1595, gem_string("setup"))};
    GemVal _t1597[] = {gem_v_p};
    GemVal _t1598 = gem_v_r;
    GemVal _t1599[] = {gem_table_get(_t1598, gem_string("expr"))};
                                                return gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1596, 1), gem_to_string_fn(NULL, _t1597, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t1599, 1)), gem_string(");"));
                                            } else {
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                                                    return gem_string("");
                                                } else {
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
    GemVal _t1600[] = {gem_v_p};
                                                        GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t1600, 1), gem_string("{\n"));
                                                        {
    GemVal _t1601 = gem_v_node;
                                                            GemVal gem_v__for_items_33 = gem_table_get(_t1601, gem_string("stmts"));
                                                            GemVal gem_v__for_i_33 = gem_int(0);
                                                            while (1) {
                                                                GemVal _t1602[] = {gem_v__for_items_33};
                                                                if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_len_fn(NULL, _t1602, 1)))) break;
                                                                GemVal gem_v_stmt = gem_table_get(gem_v__for_items_33, gem_v__for_i_33);
                                                                gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
    GemVal _t1603 = (*gem_v_compile_stmt);
    GemVal _t1604[] = {gem_v_stmt, gem_add(gem_v_indent, gem_int(1))};
                                                                gem_v_out = gem_add(gem_add(gem_v_out, _t1603.fn(_t1603.env, _t1604, 2)), gem_string("\n"));
                                                            }
                                                        }
    GemVal _t1605[] = {gem_v_p};
                                                        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1605, 1), gem_string("}")));
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
    GemVal _t1606 = (*gem_v_compile_expr);
    GemVal _t1607[] = {gem_v_node};
    GemVal gem_v_r = _t1606.fn(_t1606.env, _t1607, 1);
    GemVal _t1608 = gem_v_r;
    GemVal _t1609[] = {gem_table_get(_t1608, gem_string("setup"))};
    GemVal _t1610[] = {gem_v_p};
    GemVal _t1611 = gem_v_r;
    GemVal _t1612[] = {gem_table_get(_t1611, gem_string("expr"))};
    return gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1609, 1), gem_to_string_fn(NULL, _t1610, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t1612, 1)), gem_string(");"));
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
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t1614[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1614, 1);
    GemVal _t1615 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1615, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t1616 = gem_v_node;
    GemVal _t1617 = (*gem_v_compile_expr);
    GemVal _t1618[] = {gem_table_get(_t1616, gem_string("value"))};
        GemVal gem_v_r = _t1617.fn(_t1617.env, _t1618, 1);
    GemVal _t1619 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1619, gem_string("name"));
    GemVal _t1620 = (*gem_v_mangle);
    GemVal _t1621[] = {gem_v_name};
        GemVal gem_v_mname = _t1620.fn(_t1620.env, _t1621, 1);
    GemVal _t1622[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1622, 2))) {
    GemVal _t1623 = gem_v_r;
    GemVal _t1624[] = {gem_table_get(_t1623, gem_string("setup"))};
    GemVal _t1625[] = {gem_v_p};
    GemVal _t1626[] = {gem_v_mname};
    GemVal _t1627[] = {gem_v_p};
    GemVal _t1628[] = {gem_v_mname};
    GemVal _t1629 = gem_v_r;
    GemVal _t1630[] = {gem_table_get(_t1629, gem_string("expr"))};
    GemVal _t1631[] = {gem_v_p};
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1624, 1), gem_to_string_fn(NULL, _t1625, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t1626, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t1627, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1628, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1630, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1631, 1)), gem_string("return GEM_NIL;"));
        }
    GemVal _t1632 = gem_v_r;
    GemVal _t1633[] = {gem_table_get(_t1632, gem_string("setup"))};
    GemVal _t1634[] = {gem_v_p};
    GemVal _t1635[] = {gem_v_mname};
    GemVal _t1636 = gem_v_r;
    GemVal _t1637[] = {gem_table_get(_t1636, gem_string("expr"))};
    GemVal _t1638[] = {gem_v_p};
        return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1633, 1), gem_to_string_fn(NULL, _t1634, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1635, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1637, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1638, 1)), gem_string("return GEM_NIL;"));
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t1639 = gem_v_node;
    GemVal _t1640 = (*gem_v_compile_expr);
    GemVal _t1641[] = {gem_table_get(_t1639, gem_string("value"))};
            GemVal gem_v_r = _t1640.fn(_t1640.env, _t1641, 1);
    GemVal _t1642 = (*gem_v_tmp);
            GemVal gem_v_t = _t1642.fn(_t1642.env, NULL, 0);
    GemVal _t1643 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1643, gem_string("name"));
    GemVal _t1644 = (*gem_v_mangle);
    GemVal _t1645[] = {gem_v_name};
            GemVal gem_v_mname = _t1644.fn(_t1644.env, _t1645, 1);
    GemVal _t1646[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1646, 2))) {
    GemVal _t1647 = gem_v_r;
    GemVal _t1648[] = {gem_table_get(_t1647, gem_string("setup"))};
    GemVal _t1649[] = {gem_v_p};
    GemVal _t1650[] = {gem_v_t};
    GemVal _t1651 = gem_v_r;
    GemVal _t1652[] = {gem_table_get(_t1651, gem_string("expr"))};
    GemVal _t1653[] = {gem_v_p};
    GemVal _t1654[] = {gem_v_mname};
    GemVal _t1655[] = {gem_v_t};
    GemVal _t1656[] = {gem_v_p};
    GemVal _t1657[] = {gem_v_t};
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1648, 1), gem_to_string_fn(NULL, _t1649, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1650, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1652, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1653, 1)), gem_string("*")), gem_to_string_fn(NULL, _t1654, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1655, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1656, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1657, 1)), gem_string(";"));
            }
    GemVal _t1658 = gem_v_r;
    GemVal _t1659[] = {gem_table_get(_t1658, gem_string("setup"))};
    GemVal _t1660[] = {gem_v_p};
    GemVal _t1661[] = {gem_v_t};
    GemVal _t1662 = gem_v_r;
    GemVal _t1663[] = {gem_table_get(_t1662, gem_string("expr"))};
    GemVal _t1664[] = {gem_v_p};
    GemVal _t1665[] = {gem_v_mname};
    GemVal _t1666[] = {gem_v_t};
    GemVal _t1667[] = {gem_v_p};
    GemVal _t1668[] = {gem_v_t};
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1659, 1), gem_to_string_fn(NULL, _t1660, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1661, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1663, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1664, 1)), gem_to_string_fn(NULL, _t1665, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1666, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t1667, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1668, 1)), gem_string(";"));
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t1669 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t1669, gem_string("value")), GEM_NIL))) {
    GemVal _t1670 = gem_v_node;
    GemVal _t1671 = (*gem_v_compile_expr);
    GemVal _t1672[] = {gem_table_get(_t1670, gem_string("value"))};
                    GemVal gem_v_r = _t1671.fn(_t1671.env, _t1672, 1);
    GemVal _t1673 = gem_v_r;
    GemVal _t1674[] = {gem_table_get(_t1673, gem_string("setup"))};
    GemVal _t1675[] = {gem_v_p};
    GemVal _t1676 = gem_v_r;
    GemVal _t1677[] = {gem_table_get(_t1676, gem_string("expr"))};
                    return gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1674, 1), gem_to_string_fn(NULL, _t1675, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1677, 1)), gem_string(";"));
                }
    GemVal _t1678[] = {gem_v_p};
                return gem_add(gem_to_string_fn(NULL, _t1678, 1), gem_string("return GEM_NIL;"));
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t1679 = (*gem_v_compile_if_return);
    GemVal _t1680[] = {gem_v_node, gem_v_indent};
                    return _t1679.fn(_t1679.env, _t1680, 2);
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t1681 = (*gem_v_compile_while);
    GemVal _t1682[] = {gem_v_node, gem_v_indent};
    GemVal _t1683[] = {_t1681.fn(_t1681.env, _t1682, 2)};
    GemVal _t1684[] = {gem_v_p};
                        return gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1683, 1), gem_string("\n")), gem_to_string_fn(NULL, _t1684, 1)), gem_string("return GEM_NIL;"));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t1685 = (*gem_v_compile_match_return);
    GemVal _t1686[] = {gem_v_node, gem_v_indent};
                            return _t1685.fn(_t1685.env, _t1686, 2);
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
    GemVal _t1687[] = {gem_v_p};
                                GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t1687, 1), gem_string("{\n"));
                                {
                                    GemVal gem_v__for_i_34 = gem_int(0);
    GemVal _t1688 = gem_v_node;
    GemVal _t1689[] = {gem_table_get(_t1688, gem_string("stmts"))};
                                    GemVal gem_v__for_limit_34 = gem_sub(gem_len_fn(NULL, _t1689, 1), gem_int(1));
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
                                        GemVal gem_v_i = gem_v__for_i_34;
                                        gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
    GemVal _t1690 = gem_v_node;
    GemVal _t1691 = (*gem_v_compile_stmt);
    GemVal _t1692[] = {gem_table_get(gem_table_get(_t1690, gem_string("stmts")), gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                                        gem_v_out = gem_add(gem_add(gem_v_out, _t1691.fn(_t1691.env, _t1692, 2)), gem_string("\n"));
                                    }
                                }
    GemVal _t1693 = gem_v_node;
    GemVal _t1694[] = {gem_table_get(_t1693, gem_string("stmts"))};
                                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1694, 1), gem_int(0)))) {
    GemVal _t1695 = gem_v_node;
    GemVal _t1696 = gem_v_node;
    GemVal _t1697[] = {gem_table_get(_t1696, gem_string("stmts"))};
    GemVal _t1698 = (*gem_v_compile_stmt_return);
    GemVal _t1699[] = {gem_table_get(gem_table_get(_t1695, gem_string("stmts")), gem_sub(gem_len_fn(NULL, _t1697, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t1698.fn(_t1698.env, _t1699, 2)), gem_string("\n"));
                                }
    GemVal _t1700[] = {gem_v_p};
                                gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1700, 1), gem_string("}")));
                                return gem_v_out;
                            } else {
    GemVal _t1701;
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
        _t1701 = gem_eq(gem_v_tag, gem_string("dot_assign"));
    } else {
        _t1701 = gem_eq(gem_v_tag, gem_string("index_assign"));
    }
    GemVal _t1702;
    if (gem_truthy(_t1701)) {
        _t1702 = _t1701;
    } else {
        _t1702 = gem_eq(gem_v_tag, gem_string("break"));
    }
    GemVal _t1703;
    if (gem_truthy(_t1702)) {
        _t1703 = _t1702;
    } else {
        _t1703 = gem_eq(gem_v_tag, gem_string("continue"));
    }
    GemVal _t1704;
    if (gem_truthy(_t1703)) {
        _t1704 = _t1703;
    } else {
        _t1704 = gem_eq(gem_v_tag, gem_string("fn_def"));
    }
                                if (gem_truthy(_t1704)) {
    GemVal _t1705 = (*gem_v_compile_stmt);
    GemVal _t1706[] = {gem_v_node, gem_v_indent};
    GemVal _t1707[] = {_t1705.fn(_t1705.env, _t1706, 2)};
    GemVal _t1708[] = {gem_v_p};
                                    return gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1707, 1), gem_string("\n")), gem_to_string_fn(NULL, _t1708, 1)), gem_string("return GEM_NIL;"));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t1709 = (*gem_v_compile_expr);
    GemVal _t1710[] = {gem_v_node};
    GemVal gem_v_r = _t1709.fn(_t1709.env, _t1710, 1);
    GemVal _t1711 = gem_v_r;
    GemVal _t1712[] = {gem_table_get(_t1711, gem_string("setup"))};
    GemVal _t1713[] = {gem_v_p};
    GemVal _t1714 = gem_v_r;
    GemVal _t1715[] = {gem_table_get(_t1714, gem_string("expr"))};
    return gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1712, 1), gem_to_string_fn(NULL, _t1713, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t1715, 1)), gem_string(";"));
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
    GemVal _t1717[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1717, 1);
    GemVal _t1718 = gem_v_node;
    GemVal _t1719 = (*gem_v_compile_expr);
    GemVal _t1720[] = {gem_table_get(_t1718, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1719.fn(_t1719.env, _t1720, 1);
    GemVal _t1721 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t1721, gem_string("setup"));
    GemVal _t1722[] = {gem_v_p};
    GemVal _t1723 = gem_v_cond_r;
    GemVal _t1724[] = {gem_table_get(_t1723, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1722, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t1724, 1)), gem_string(")) {\n")));
    GemVal _t1725 = (*gem_v_compile_stmts);
    GemVal _t1726[] = {gem_table_get(gem_v_node, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t1725.fn(_t1725.env, _t1726, 2));
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
    GemVal _t1727[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1727, 1), gem_string("} else {\n")));
    GemVal _t1728 = (*gem_v_compile_stmts);
    GemVal _t1729[] = {gem_table_get(gem_v_node, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t1728.fn(_t1728.env, _t1729, 2));
    }
    GemVal _t1730[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1730, 1), gem_string("}")));
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
    GemVal _t1732[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1732, 1);
    GemVal _t1733 = gem_v_node;
    GemVal _t1734 = (*gem_v_compile_expr);
    GemVal _t1735[] = {gem_table_get(_t1733, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1734.fn(_t1734.env, _t1735, 1);
    GemVal _t1736 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t1736, gem_string("setup"));
    GemVal _t1737[] = {gem_v_p};
    GemVal _t1738 = gem_v_cond_r;
    GemVal _t1739[] = {gem_table_get(_t1738, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1737, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t1739, 1)), gem_string(")) {\n")));
    GemVal gem_v_then_body = gem_table_get(gem_v_node, gem_string("then"));
    GemVal _t1740[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1740, 1), gem_int(0)))) {
        {
            GemVal gem_v__for_i_35 = gem_int(0);
    GemVal _t1741[] = {gem_v_then_body};
            GemVal gem_v__for_limit_35 = gem_sub(gem_len_fn(NULL, _t1741, 1), gem_int(1));
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
                GemVal gem_v_i = gem_v__for_i_35;
                gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
    GemVal _t1742 = (*gem_v_compile_stmt);
    GemVal _t1743[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t1742.fn(_t1742.env, _t1743, 2)), gem_string("\n"));
            }
        }
    GemVal _t1744[] = {gem_v_then_body};
    GemVal _t1745 = (*gem_v_compile_stmt_return);
    GemVal _t1746[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t1744, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_add(gem_v_out, _t1745.fn(_t1745.env, _t1746, 2)), gem_string("\n"));
    } else {
    GemVal _t1747[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1747, 1), gem_string("    return GEM_NIL;\n")));
    }
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
    GemVal _t1748[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1748, 1), gem_string("} else {\n")));
        GemVal gem_v_else_body = gem_table_get(gem_v_node, gem_string("else"));
    GemVal _t1749[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1749, 1), gem_int(0)))) {
            {
                GemVal gem_v__for_i_36 = gem_int(0);
    GemVal _t1750[] = {gem_v_else_body};
                GemVal gem_v__for_limit_36 = gem_sub(gem_len_fn(NULL, _t1750, 1), gem_int(1));
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
                    GemVal gem_v_i = gem_v__for_i_36;
                    gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
    GemVal _t1751 = (*gem_v_compile_stmt);
    GemVal _t1752[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t1751.fn(_t1751.env, _t1752, 2)), gem_string("\n"));
                }
            }
    GemVal _t1753[] = {gem_v_else_body};
    GemVal _t1754 = (*gem_v_compile_stmt_return);
    GemVal _t1755[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t1753, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1754.fn(_t1754.env, _t1755, 2)), gem_string("\n"));
        } else {
    GemVal _t1756[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1756, 1), gem_string("    return GEM_NIL;\n")));
        }
    } else {
    GemVal _t1757[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1757, 1), gem_string("} else {\n")));
    GemVal _t1758[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1758, 1), gem_string("    return GEM_NIL;\n")));
    }
    GemVal _t1759[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1759, 1), gem_string("}")));
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
    GemVal _t1761[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1761, 1);
    GemVal _t1762 = gem_v_node;
    GemVal _t1763 = (*gem_v_compile_expr);
    GemVal _t1764[] = {gem_table_get(_t1762, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1763.fn(_t1763.env, _t1764, 1);
    GemVal _t1765[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t1765, 1), gem_string("while (1) {\n"));
    GemVal _t1766 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t1766, gem_string("setup")), gem_string("")))) {
    GemVal _t1767 = gem_v_cond_r;
    GemVal _t1768[] = {gem_v_p};
    GemVal _t1769[] = {gem_table_get(_t1767, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t1768, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_fn_str_replace_all(NULL, _t1769, 3));
    }
    GemVal _t1770[] = {gem_v_p};
    GemVal _t1771 = gem_v_cond_r;
    GemVal _t1772[] = {gem_table_get(_t1771, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1770, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t1772, 1)), gem_string(")) break;\n")));
    GemVal _t1773 = gem_v_node;
    GemVal _t1774 = (*gem_v_compile_stmts);
    GemVal _t1775[] = {gem_table_get(_t1773, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t1774.fn(_t1774.env, _t1775, 2));
    GemVal _t1776[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1776, 1), gem_string("}")));
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
    GemVal _t1778[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1778, 1);
    GemVal _t1779 = gem_v_node;
    GemVal _t1780 = (*gem_v_compile_expr);
    GemVal _t1781[] = {gem_table_get(_t1779, gem_string("target"))};
    GemVal gem_v_target_r = _t1780.fn(_t1780.env, _t1781, 1);
    GemVal _t1782 = (*gem_v_tmp);
    GemVal gem_v_t = _t1782.fn(_t1782.env, NULL, 0);
    GemVal _t1783 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t1783, gem_string("setup"));
    GemVal _t1784[] = {gem_v_p};
    GemVal _t1785[] = {gem_v_t};
    GemVal _t1786 = gem_v_target_r;
    GemVal _t1787[] = {gem_table_get(_t1786, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1784, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1785, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1787, 1)), gem_string(";\n")));
    {
        GemVal gem_v__for_i_37 = gem_int(0);
    GemVal _t1788 = gem_v_node;
    GemVal _t1789[] = {gem_table_get(_t1788, gem_string("whens"))};
        GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t1789, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
            GemVal gem_v_i = gem_v__for_i_37;
            gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
    GemVal _t1790 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t1790, gem_string("whens")), gem_v_i);
    GemVal _t1791 = gem_v_w;
    GemVal _t1792 = (*gem_v_compile_expr);
    GemVal _t1793[] = {gem_table_get(_t1791, gem_string("value"))};
            GemVal gem_v_val_r = _t1792.fn(_t1792.env, _t1793, 1);
            GemVal gem_v_keyword = gem_string("if");
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
                gem_v_keyword = gem_string("} else if");
            }
    GemVal _t1794 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t1794, gem_string("setup")));
    GemVal _t1795[] = {gem_v_p};
    GemVal _t1796[] = {gem_v_keyword};
    GemVal _t1797[] = {gem_v_t};
    GemVal _t1798 = gem_v_val_r;
    GemVal _t1799[] = {gem_table_get(_t1798, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1795, 1), gem_to_string_fn(NULL, _t1796, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t1797, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1799, 1)), gem_string("))) {\n")));
    GemVal _t1800 = gem_v_w;
    GemVal _t1801 = (*gem_v_compile_stmts);
    GemVal _t1802[] = {gem_table_get(_t1800, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_v_out, _t1801.fn(_t1801.env, _t1802, 2));
        }
    }
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
    GemVal _t1803[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1803, 1), gem_string("} else {\n")));
    GemVal _t1804 = (*gem_v_compile_stmts);
    GemVal _t1805[] = {gem_table_get(gem_v_node, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t1804.fn(_t1804.env, _t1805, 2));
    }
    GemVal _t1806[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1806, 1), gem_string("}")));
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
    GemVal _t1808[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1808, 1);
    GemVal _t1809 = gem_v_node;
    GemVal _t1810 = (*gem_v_compile_expr);
    GemVal _t1811[] = {gem_table_get(_t1809, gem_string("target"))};
    GemVal gem_v_target_r = _t1810.fn(_t1810.env, _t1811, 1);
    GemVal _t1812 = (*gem_v_tmp);
    GemVal gem_v_t = _t1812.fn(_t1812.env, NULL, 0);
    GemVal _t1813 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t1813, gem_string("setup"));
    GemVal _t1814[] = {gem_v_p};
    GemVal _t1815[] = {gem_v_t};
    GemVal _t1816 = gem_v_target_r;
    GemVal _t1817[] = {gem_table_get(_t1816, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1814, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t1815, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t1817, 1)), gem_string(";\n")));
    {
        GemVal gem_v__for_i_39 = gem_int(0);
    GemVal _t1818 = gem_v_node;
    GemVal _t1819[] = {gem_table_get(_t1818, gem_string("whens"))};
        GemVal gem_v__for_limit_39 = gem_len_fn(NULL, _t1819, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
            GemVal gem_v_i = gem_v__for_i_39;
            gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
    GemVal _t1820 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t1820, gem_string("whens")), gem_v_i);
    GemVal _t1821 = gem_v_w;
    GemVal _t1822 = (*gem_v_compile_expr);
    GemVal _t1823[] = {gem_table_get(_t1821, gem_string("value"))};
            GemVal gem_v_val_r = _t1822.fn(_t1822.env, _t1823, 1);
            GemVal gem_v_keyword = gem_string("if");
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
                gem_v_keyword = gem_string("} else if");
            }
    GemVal _t1824 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t1824, gem_string("setup")));
    GemVal _t1825[] = {gem_v_p};
    GemVal _t1826[] = {gem_v_keyword};
    GemVal _t1827[] = {gem_v_t};
    GemVal _t1828 = gem_v_val_r;
    GemVal _t1829[] = {gem_table_get(_t1828, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1825, 1), gem_to_string_fn(NULL, _t1826, 1)), gem_string(" (gem_truthy(gem_eq(")), gem_to_string_fn(NULL, _t1827, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t1829, 1)), gem_string("))) {\n")));
    GemVal _t1830 = gem_v_w;
            GemVal gem_v_body = gem_table_get(_t1830, gem_string("body"));
    GemVal _t1831[] = {gem_v_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1831, 1), gem_int(0)))) {
                {
                    GemVal gem_v__for_i_38 = gem_int(0);
    GemVal _t1832[] = {gem_v_body};
                    GemVal gem_v__for_limit_38 = gem_sub(gem_len_fn(NULL, _t1832, 1), gem_int(1));
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
                        GemVal gem_v_j = gem_v__for_i_38;
                        gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
    GemVal _t1833 = (*gem_v_compile_stmt);
    GemVal _t1834[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
                        gem_v_out = gem_add(gem_add(gem_v_out, _t1833.fn(_t1833.env, _t1834, 2)), gem_string("\n"));
                    }
                }
    GemVal _t1835[] = {gem_v_body};
    GemVal _t1836 = (*gem_v_compile_stmt_return);
    GemVal _t1837[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t1835, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t1836.fn(_t1836.env, _t1837, 2)), gem_string("\n"));
            } else {
    GemVal _t1838[] = {gem_v_p};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1838, 1), gem_string("    return GEM_NIL;\n")));
            }
        }
    }
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
    GemVal _t1839[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1839, 1), gem_string("} else {\n")));
        GemVal gem_v_else_body = gem_table_get(gem_v_node, gem_string("else"));
    GemVal _t1840[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1840, 1), gem_int(0)))) {
            {
                GemVal gem_v__for_i_40 = gem_int(0);
    GemVal _t1841[] = {gem_v_else_body};
                GemVal gem_v__for_limit_40 = gem_sub(gem_len_fn(NULL, _t1841, 1), gem_int(1));
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
                    GemVal gem_v_i = gem_v__for_i_40;
                    gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
    GemVal _t1842 = (*gem_v_compile_stmt);
    GemVal _t1843[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t1842.fn(_t1842.env, _t1843, 2)), gem_string("\n"));
                }
            }
    GemVal _t1844[] = {gem_v_else_body};
    GemVal _t1845 = (*gem_v_compile_stmt_return);
    GemVal _t1846[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t1844, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1845.fn(_t1845.env, _t1846, 2)), gem_string("\n"));
        } else {
    GemVal _t1847[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1847, 1), gem_string("    return GEM_NIL;\n")));
        }
    } else {
    GemVal _t1848[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1848, 1), gem_string("} else {\n")));
    GemVal _t1849[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1849, 1), gem_string("    return GEM_NIL;\n")));
    }
    GemVal _t1850[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t1850, 1), gem_string("}")));
    return gem_v_out;
}

struct _closure__anon_44 {
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t1852 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t1852, gem_string("name"));
    GemVal _t1853 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1853, gem_string("params"));
    GemVal _t1854 = gem_v_node;
    GemVal gem_v_ret_type = gem_table_get(_t1854, gem_string("ret_type"));
    GemVal _t1855[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t1855, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
    GemVal _t1856 = gem_table_new();
    GemVal gem_v_c_args = _t1856;
    {
        GemVal gem_v__for_i_41 = gem_int(0);
    GemVal _t1857[] = {gem_v_params};
        GemVal gem_v__for_limit_41 = gem_len_fn(NULL, _t1857, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_limit_41))) break;
            GemVal gem_v_i = gem_v__for_i_41;
            gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
            GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
            GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
    GemVal _t1858[] = {gem_v_i};
    GemVal _t1859[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t1858, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1859, 1)), gem_string("].ival;\n")));
    GemVal _t1860[] = {gem_v_i};
    GemVal _t1861[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1860, 1))};
                (void)(gem_push_fn(NULL, _t1861, 2));
            } else {
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
    GemVal _t1862[] = {gem_v_i};
    GemVal _t1863[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t1862, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1863, 1)), gem_string("].fval;\n")));
    GemVal _t1864[] = {gem_v_i};
    GemVal _t1865[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1864, 1))};
                    (void)(gem_push_fn(NULL, _t1865, 2));
                } else {
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
    GemVal _t1866[] = {gem_v_i};
    GemVal _t1867[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t1866, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1867, 1)), gem_string("].sval;\n")));
    GemVal _t1868[] = {gem_v_i};
    GemVal _t1869[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1868, 1))};
                        (void)(gem_push_fn(NULL, _t1869, 2));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
    GemVal _t1870[] = {gem_v_i};
    GemVal _t1871[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t1870, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1871, 1)), gem_string("].bval;\n")));
    GemVal _t1872[] = {gem_v_i};
    GemVal _t1873[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1872, 1))};
                            (void)(gem_push_fn(NULL, _t1873, 2));
                        } else {
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
    GemVal _t1874[] = {gem_v_i};
    GemVal _t1875[] = {gem_v_i};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t1874, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t1875, 1)), gem_string("].ival;\n")));
    GemVal _t1876[] = {gem_v_i};
    GemVal _t1877[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1876, 1))};
                                (void)(gem_push_fn(NULL, _t1877, 2));
                            } else {
                                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
    GemVal _t1878[] = {gem_v_i};
    GemVal _t1879[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t1878, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t1879, 2));
                                } else {
    GemVal _t1880[] = {gem_v_i};
    GemVal _t1881[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t1880, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t1881, 2));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal gem_v_call_args = gem_string("");
    {
        GemVal gem_v__for_i_42 = gem_int(0);
    GemVal _t1882[] = {gem_v_c_args};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t1882, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
            GemVal gem_v_i = gem_v__for_i_42;
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
                gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
            }
            gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
        }
    }
    GemVal _t1883[] = {gem_v_name};
    GemVal _t1884[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t1883, 1), gem_string("(")), gem_to_string_fn(NULL, _t1884, 1)), gem_string(")"));
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
    GemVal _t1885[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1885, 1)), gem_string(";\n")));
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
    GemVal _t1886[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t1886, 1)), gem_string(";\n")));
        } else {
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
    GemVal _t1887[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t1887, 1)), gem_string(";\n")));
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
    GemVal _t1888[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t1888, 1)), gem_string(";\n")));
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
    GemVal _t1889[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t1889, 1)), gem_string(";\n")));
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
    GemVal _t1890[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t1890, 1)), gem_string(";\n")));
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
    GemVal _t1891[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t1891, 1)), gem_string(";\n")));
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
    GemVal _t1892[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t1892, 1)), gem_string(";\n")));
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
    GemVal _t1893[] = {gem_v_name};
    GemVal _t1894[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t1893, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t1894, 2));
    return gem_v_lines;
}

struct _closure__anon_45 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_mangle;
};
static GemVal _anon_45(void *_env, GemVal *args, int argc) {
    struct _closure__anon_45 *_cls = (struct _closure__anon_45 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t1896 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1896, gem_string("params"));
    GemVal _t1897 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t1897, gem_string("body"));
    GemVal _t1898 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t1898, gem_string("name"));
    GemVal _t1899[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t1899, 1);
    {
        GemVal gem_v__for_i_44 = gem_int(0);
    GemVal _t1900[] = {gem_v_body};
        GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t1900, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
            GemVal gem_v_i = gem_v__for_i_44;
            gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
    GemVal _t1901[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1903;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1901, 1), gem_string("table")))) {
        _t1903 = gem_eq(gem_type_fn(NULL, _t1901, 1), gem_string("table"));
    } else {
        GemVal _t1902 = gem_table_get(gem_v_body, gem_v_i);
        _t1903 = gem_eq(gem_table_get(_t1902, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1903)) {
    GemVal _t1904 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1905[] = {gem_v_scope_vars, gem_table_get(_t1904, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1905, 2));
            }
    GemVal _t1906[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1908;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1906, 1), gem_string("table")))) {
        _t1908 = gem_eq(gem_type_fn(NULL, _t1906, 1), gem_string("table"));
    } else {
        GemVal _t1907 = gem_table_get(gem_v_body, gem_v_i);
        _t1908 = gem_eq(gem_table_get(_t1907, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1908)) {
                {
                    GemVal gem_v__for_i_43 = gem_int(0);
    GemVal _t1909 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1910[] = {gem_table_get(_t1909, gem_string("stmts"))};
                    GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t1910, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
                        GemVal gem_v_j = gem_v__for_i_43;
                        gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
    GemVal _t1911 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1912[] = {gem_table_get(gem_table_get(_t1911, gem_string("stmts")), gem_v_j)};
    GemVal _t1915;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1912, 1), gem_string("table")))) {
        _t1915 = gem_eq(gem_type_fn(NULL, _t1912, 1), gem_string("table"));
    } else {
        GemVal _t1913 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t1914 = gem_table_get(gem_table_get(_t1913, gem_string("stmts")), gem_v_j);
        _t1915 = gem_eq(gem_table_get(_t1914, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1915)) {
    GemVal _t1916 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1917 = gem_table_get(gem_table_get(_t1916, gem_string("stmts")), gem_v_j);
    GemVal _t1918[] = {gem_v_scope_vars, gem_table_get(_t1917, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1918, 2));
                        }
                    }
                }
            }
        }
    }
    GemVal _t1919 = (*gem_v_find_captured_in_scope);
    GemVal _t1920[] = {gem_v_body, gem_v_scope_vars};
    GemVal gem_v_captured = _t1919.fn(_t1919.env, _t1920, 2);
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
    *gem_v_boxed_vars = gem_v_captured;
    GemVal _t1921[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t1921, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
    {
        GemVal gem_v__for_i_45 = gem_int(0);
    GemVal _t1922[] = {gem_v_params};
        GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t1922, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
            GemVal gem_v_i = gem_v__for_i_45;
            gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
    GemVal _t1923 = (*gem_v_mangle);
    GemVal _t1924[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t1923.fn(_t1923.env, _t1924, 1);
    GemVal _t1925[] = {gem_v_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1925, 2))) {
    GemVal _t1926[] = {gem_v_mp};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t1926, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
    GemVal _t1927[] = {gem_v_mp};
    GemVal _t1928[] = {gem_v_i};
    GemVal _t1929[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t1927, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1928, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1929, 1)), gem_string("] : GEM_NIL;\n")));
            } else {
    GemVal _t1930[] = {gem_v_mp};
    GemVal _t1931[] = {gem_v_i};
    GemVal _t1932[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1930, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1931, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1932, 1)), gem_string("] : GEM_NIL;\n")));
            }
        }
    }
    GemVal _t1933[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1933, 1), gem_int(0)))) {
        {
            GemVal gem_v__for_i_46 = gem_int(0);
    GemVal _t1934[] = {gem_v_body};
            GemVal gem_v__for_limit_46 = gem_sub(gem_len_fn(NULL, _t1934, 1), gem_int(1));
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
                GemVal gem_v_i = gem_v__for_i_46;
                gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
    GemVal _t1935 = (*gem_v_compile_stmt);
    GemVal _t1936[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t1935.fn(_t1935.env, _t1936, 2)), gem_string("\n"));
            }
        }
    GemVal _t1937[] = {gem_v_body};
    GemVal _t1938 = (*gem_v_compile_stmt_return);
    GemVal _t1939[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t1937, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t1938.fn(_t1938.env, _t1939, 2)), gem_string("\n"));
    } else {
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    }
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
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
    GemVal _t1941 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1941, gem_string("params"));
    GemVal _t1942 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t1942, gem_string("body"));
    GemVal _t1943 = (*gem_v_anon_name);
    GemVal gem_v_name = _t1943.fn(_t1943.env, NULL, 0);
    GemVal _t1944[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t1944, 1);
    {
        GemVal gem_v__for_i_48 = gem_int(0);
    GemVal _t1945[] = {gem_v_body};
        GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t1945, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
            GemVal gem_v_i = gem_v__for_i_48;
            gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
    GemVal _t1946[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1948;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1946, 1), gem_string("table")))) {
        _t1948 = gem_eq(gem_type_fn(NULL, _t1946, 1), gem_string("table"));
    } else {
        GemVal _t1947 = gem_table_get(gem_v_body, gem_v_i);
        _t1948 = gem_eq(gem_table_get(_t1947, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1948)) {
    GemVal _t1949 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1950[] = {gem_v_inner_scope, gem_table_get(_t1949, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1950, 2));
            }
    GemVal _t1951[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1953;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1951, 1), gem_string("table")))) {
        _t1953 = gem_eq(gem_type_fn(NULL, _t1951, 1), gem_string("table"));
    } else {
        GemVal _t1952 = gem_table_get(gem_v_body, gem_v_i);
        _t1953 = gem_eq(gem_table_get(_t1952, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1953)) {
                {
                    GemVal gem_v__for_i_47 = gem_int(0);
    GemVal _t1954 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1955[] = {gem_table_get(_t1954, gem_string("stmts"))};
                    GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t1955, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
                        GemVal gem_v_j = gem_v__for_i_47;
                        gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
    GemVal _t1956 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1957[] = {gem_table_get(gem_table_get(_t1956, gem_string("stmts")), gem_v_j)};
    GemVal _t1960;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1957, 1), gem_string("table")))) {
        _t1960 = gem_eq(gem_type_fn(NULL, _t1957, 1), gem_string("table"));
    } else {
        GemVal _t1958 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t1959 = gem_table_get(gem_table_get(_t1958, gem_string("stmts")), gem_v_j);
        _t1960 = gem_eq(gem_table_get(_t1959, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1960)) {
    GemVal _t1961 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1962 = gem_table_get(gem_table_get(_t1961, gem_string("stmts")), gem_v_j);
    GemVal _t1963[] = {gem_v_inner_scope, gem_table_get(_t1962, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1963, 2));
                        }
                    }
                }
            }
        }
    }
    GemVal _t1964 = (*gem_v_find_captured_in_scope);
    GemVal _t1965[] = {gem_v_body, gem_v_inner_scope};
    GemVal gem_v_inner_captured = _t1964.fn(_t1964.env, _t1965, 2);
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
    GemVal _t1966[] = {gem_v_captures};
    GemVal _t1967[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t1966, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t1967, 2);
    GemVal _t1968[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t1968, 1));
    GemVal _t1969[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t1969, 1)), gem_string(" {\n"));
    {
        GemVal gem_v__for_items_49 = gem_v_captures;
        GemVal gem_v__for_i_49 = gem_int(0);
        while (1) {
            GemVal _t1970[] = {gem_v__for_items_49};
            if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_len_fn(NULL, _t1970, 1)))) break;
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_49, gem_v__for_i_49);
            gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
    GemVal _t1971 = (*gem_v_mangle);
    GemVal _t1972[] = {gem_v_cap};
    GemVal _t1973[] = {_t1971.fn(_t1971.env, _t1972, 1)};
            gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t1973, 1)), gem_string(";\n")));
        }
    }
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
    GemVal _t1974[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t1974, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
    GemVal _t1975[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1975, 1), gem_int(0)))) {
    GemVal _t1976[] = {gem_v_struct_name};
    GemVal _t1977[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t1976, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t1977, 1)), gem_string(" *)_env;\n")));
        {
            GemVal gem_v__for_items_50 = gem_v_captures;
            GemVal gem_v__for_i_50 = gem_int(0);
            while (1) {
                GemVal _t1978[] = {gem_v__for_items_50};
                if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_len_fn(NULL, _t1978, 1)))) break;
                GemVal gem_v_cap = gem_table_get(gem_v__for_items_50, gem_v__for_i_50);
                gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
    GemVal _t1979 = (*gem_v_mangle);
    GemVal _t1980[] = {gem_v_cap};
                GemVal gem_v_mc = _t1979.fn(_t1979.env, _t1980, 1);
    GemVal _t1981[] = {gem_v_mc};
    GemVal _t1982[] = {gem_v_mc};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t1981, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t1982, 1)), gem_string(";\n")));
            }
        }
    }
    {
        GemVal gem_v__for_i_51 = gem_int(0);
    GemVal _t1983[] = {gem_v_params};
        GemVal gem_v__for_limit_51 = gem_len_fn(NULL, _t1983, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_v__for_limit_51))) break;
            GemVal gem_v_i = gem_v__for_i_51;
            gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
    GemVal _t1984 = (*gem_v_mangle);
    GemVal _t1985[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t1984.fn(_t1984.env, _t1985, 1);
    GemVal _t1986[] = {gem_v_inner_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1986, 2))) {
    GemVal _t1987[] = {gem_v_mp};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t1987, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
    GemVal _t1988[] = {gem_v_mp};
    GemVal _t1989[] = {gem_v_i};
    GemVal _t1990[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t1988, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1989, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1990, 1)), gem_string("] : GEM_NIL;\n")));
            } else {
    GemVal _t1991[] = {gem_v_mp};
    GemVal _t1992[] = {gem_v_i};
    GemVal _t1993[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t1991, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1992, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1993, 1)), gem_string("] : GEM_NIL;\n")));
            }
        }
    }
    GemVal _t1994[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1994, 1), gem_int(0)))) {
        {
            GemVal gem_v__for_i_52 = gem_int(0);
    GemVal _t1995[] = {gem_v_body};
            GemVal gem_v__for_limit_52 = gem_sub(gem_len_fn(NULL, _t1995, 1), gem_int(1));
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
                GemVal gem_v_i = gem_v__for_i_52;
                gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
    GemVal _t1996 = (*gem_v_compile_stmt);
    GemVal _t1997[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t1996.fn(_t1996.env, _t1997, 2)), gem_string("\n"));
            }
        }
    GemVal _t1998[] = {gem_v_body};
    GemVal _t1999 = (*gem_v_compile_stmt_return);
    GemVal _t2000[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t1998, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t1999.fn(_t1999.env, _t2000, 2)), gem_string("\n"));
    } else {
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    }
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
    *gem_v_boxed_vars = gem_v_saved_boxed;
    GemVal _t2001[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t2001, 2));
    GemVal _t2002 = gem_table_new();
    gem_table_set(_t2002, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t2002, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t2002, gem_string("captures"), gem_v_captures);
    return _t2002;
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
    GemVal _t2004 = gem_table_new();
    GemVal gem_v_fn_defs = _t2004;
    GemVal _t2005 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t2005;
    GemVal _t2006 = gem_table_new();
    GemVal gem_v_extern_includes = _t2006;
    GemVal _t2007 = gem_table_new();
    GemVal gem_v_top_stmts = _t2007;
    {
    GemVal _t2008 = gem_v_ast;
        GemVal gem_v__for_items_53 = gem_table_get(_t2008, gem_string("stmts"));
        GemVal gem_v__for_i_53 = gem_int(0);
        while (1) {
            GemVal _t2009[] = {gem_v__for_items_53};
            if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_len_fn(NULL, _t2009, 1)))) break;
            GemVal gem_v_s = gem_table_get(gem_v__for_items_53, gem_v__for_i_53);
            gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
    GemVal _t2010 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t2010, gem_string("tag")), gem_string("fn_def")))) {
    GemVal _t2011[] = {gem_v_fn_defs, gem_v_s};
                (void)(gem_push_fn(NULL, _t2011, 2));
            } else {
    GemVal _t2012 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t2012, gem_string("tag")), gem_string("extern_fn")))) {
    GemVal _t2013[] = {gem_v_extern_fns_list, gem_v_s};
                    (void)(gem_push_fn(NULL, _t2013, 2));
                } else {
    GemVal _t2014 = gem_v_s;
                    if (gem_truthy(gem_eq(gem_table_get(_t2014, gem_string("tag")), gem_string("extern_include")))) {
    GemVal _t2015[] = {gem_v_extern_includes, gem_v_s};
                        (void)(gem_push_fn(NULL, _t2015, 2));
                    } else {
    GemVal _t2016[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t2016, 2));
                    }
                }
            }
        }
    }
    {
        GemVal gem_v__for_items_54 = gem_v_extern_fns_list;
        GemVal gem_v__for_i_54 = gem_int(0);
        while (1) {
            GemVal _t2017[] = {gem_v__for_items_54};
            if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_len_fn(NULL, _t2017, 1)))) break;
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_54, gem_v__for_i_54);
            gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
    GemVal _t2018 = gem_v_ef;
    GemVal _t2019[] = {(*gem_v_defined_fns), gem_table_get(_t2018, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2019, 2));
        }
    }
    {
        GemVal gem_v__for_items_55 = gem_v_fn_defs;
        GemVal gem_v__for_i_55 = gem_int(0);
        while (1) {
            GemVal _t2020[] = {gem_v__for_items_55};
            if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_len_fn(NULL, _t2020, 1)))) break;
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_55, gem_v__for_i_55);
            gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
    GemVal _t2021 = gem_v_fd;
    GemVal _t2022[] = {(*gem_v_defined_fns), gem_table_get(_t2021, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t2022, 2));
    GemVal _t2023 = gem_v_fd;
    GemVal _t2024[] = {gem_table_get(_t2023, gem_string("name"))};
            GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t2024, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
    GemVal _t2025[] = {(*gem_v_forward_decls), gem_v_decl};
            (void)(gem_push_fn(NULL, _t2025, 2));
        }
    }
    {
        GemVal gem_v__for_items_56 = gem_v_extern_fns_list;
        GemVal gem_v__for_i_56 = gem_int(0);
        while (1) {
            GemVal _t2026[] = {gem_v__for_items_56};
            if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_len_fn(NULL, _t2026, 1)))) break;
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_56, gem_v__for_i_56);
            gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
    GemVal _t2027 = (*gem_v_compile_extern_fn);
    GemVal _t2028[] = {gem_v_ef};
    GemVal _t2029[] = {(*gem_v_functions), _t2027.fn(_t2027.env, _t2028, 1)};
            (void)(gem_push_fn(NULL, _t2029, 2));
        }
    }
    {
        GemVal gem_v__for_items_57 = gem_v_fn_defs;
        GemVal gem_v__for_i_57 = gem_int(0);
        while (1) {
            GemVal _t2030[] = {gem_v__for_items_57};
            if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t2030, 1)))) break;
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
            gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
    GemVal _t2031 = (*gem_v_compile_fn);
    GemVal _t2032[] = {gem_v_fd};
    GemVal _t2033[] = {(*gem_v_functions), _t2031.fn(_t2031.env, _t2032, 1)};
            (void)(gem_push_fn(NULL, _t2033, 2));
        }
    }
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
    GemVal _t2034 = gem_table_new();
    GemVal _t2035 = (*gem_v_find_captured_in_scope);
    GemVal _t2036[] = {gem_v_top_stmts, _t2034};
    *gem_v_boxed_vars = _t2035.fn(_t2035.env, _t2036, 2);
    GemVal _t2037 = (*gem_v_compile_stmts);
    GemVal _t2038[] = {gem_v_top_stmts, gem_int(1)};
    GemVal gem_v_main_body = _t2037.fn(_t2037.env, _t2038, 2);
    *gem_v_boxed_vars = gem_v_saved_boxed;
    GemVal gem_v_has_main = gem_bool(0);
    {
        GemVal gem_v__for_items_58 = gem_v_fn_defs;
        GemVal gem_v__for_i_58 = gem_int(0);
        while (1) {
            GemVal _t2039[] = {gem_v__for_items_58};
            if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t2039, 1)))) break;
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
            gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
    GemVal _t2040 = gem_v_fd;
            if (gem_truthy(gem_eq(gem_table_get(_t2040, gem_string("name")), gem_string("main")))) {
                gem_v_has_main = gem_bool(1);
            }
        }
    }
    GemVal gem_v_main_c = gem_string("int main(void) {\n");
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
    if (gem_truthy(gem_v_has_main)) {
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    return 0;\n}\n"));
    GemVal gem_v_out = gem_string("");
    {
        GemVal gem_v__for_items_59 = gem_v_extern_includes;
        GemVal gem_v__for_i_59 = gem_int(0);
        while (1) {
            GemVal _t2041[] = {gem_v__for_items_59};
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_len_fn(NULL, _t2041, 1)))) break;
            GemVal gem_v_ei = gem_table_get(gem_v__for_items_59, gem_v__for_i_59);
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
    GemVal _t2042 = gem_v_ei;
    GemVal _t2043[] = {gem_table_get(_t2042, gem_string("path"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t2043, 1)), gem_string("\"\n")));
        }
    }
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
    {
        GemVal gem_v__for_i_60 = gem_int(0);
    GemVal _t2044[] = {(*gem_v_forward_decls)};
        GemVal gem_v__for_limit_60 = gem_len_fn(NULL, _t2044, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
            GemVal gem_v_i = gem_v__for_i_60;
            gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
    GemVal _t2045[] = {(*gem_v_forward_decls)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2045, 1), gem_int(1))))) {
                gem_v_out = gem_add(gem_v_out, gem_string("\n"));
            }
        }
    }
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
    {
        GemVal gem_v__for_i_61 = gem_int(0);
    GemVal _t2046[] = {(*gem_v_functions)};
        GemVal gem_v__for_limit_61 = gem_len_fn(NULL, _t2046, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_v__for_limit_61))) break;
            GemVal gem_v_i = gem_v__for_i_61;
            gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
    GemVal _t2047[] = {(*gem_v_functions)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t2047, 1), gem_int(1))))) {
                gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
            }
        }
    }
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    return gem_v_out;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t797 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t797;
    GemVal _t798 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t798;
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
    GemVal _t799 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t799;
    GemVal _t800 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t800;
    gem_table_set((*gem_v_builtins), gem_string("print"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("error"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("len"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("type"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("to_string"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("push"), gem_bool(1));
    GemVal _t801 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t801;
    struct _closure__anon_22 *_t803 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t803->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_22, _t803);
    struct _closure__anon_23 *_t805 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t805->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_23, _t805);
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_24, NULL);
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
    struct _closure__anon_25 *_t963 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t963->gem_v_builtins = gem_v_builtins;
    _t963->gem_v_collect_free_node = gem_v_collect_free_node;
    _t963->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t963->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_25, _t963);
    struct _closure__anon_26 *_t988 = GC_MALLOC(sizeof(struct _closure__anon_26));
    _t988->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_26, _t988);
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
    struct _closure__anon_27 *_t1060 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1060->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1060->gem_v_walk_captures = gem_v_walk_captures;
    _t1060->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_27, _t1060);
    struct _closure__anon_28 *_t1084 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1084->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_28, _t1084);
    struct _closure__anon_29 *_t1088 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t1088->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_29, _t1088);
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
    GemVal *gem_v_compile_if_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if_return = GEM_NIL;
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
    GemVal *gem_v_compile_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match_return = GEM_NIL;
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
    struct _closure__anon_30 *_t1192 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t1192->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1192->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t1192->gem_v_compile_array = gem_v_compile_array;
    _t1192->gem_v_compile_binop = gem_v_compile_binop;
    _t1192->gem_v_compile_call = gem_v_compile_call;
    _t1192->gem_v_compile_expr = gem_v_compile_expr;
    _t1192->gem_v_compile_table = gem_v_compile_table;
    _t1192->gem_v_defined_fns = gem_v_defined_fns;
    _t1192->gem_v_mangle = gem_v_mangle;
    _t1192->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_expr = gem_make_fn(_anon_30, _t1192);
    struct _closure__anon_31 *_t1208 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t1208->gem_v_compile_expr = gem_v_compile_expr;
    _t1208->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_31, _t1208);
    struct _closure__anon_32 *_t1221 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t1221->gem_v_compile_expr = gem_v_compile_expr;
    _t1221->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_32, _t1221);
    struct _closure__anon_33 *_t1257 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t1257->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1257->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1257->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t1257->gem_v_mangle = gem_v_mangle;
    _t1257->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_33, _t1257);
    struct _closure__anon_34 *_t1360 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t1360->gem_v_compile_expr = gem_v_compile_expr;
    _t1360->gem_v_defined_fns = gem_v_defined_fns;
    _t1360->gem_v_source_name = gem_v_source_name;
    _t1360->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_34, _t1360);
    struct _closure__anon_35 *_t1490 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t1490->gem_v_compile_expr = gem_v_compile_expr;
    _t1490->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_35, _t1490);
    struct _closure__anon_36 *_t1494 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t1494->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_36, _t1494);
    struct _closure__anon_37 *_t1613 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t1613->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1613->gem_v_compile_expr = gem_v_compile_expr;
    _t1613->gem_v_compile_if = gem_v_compile_if;
    _t1613->gem_v_compile_match = gem_v_compile_match;
    _t1613->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1613->gem_v_compile_while = gem_v_compile_while;
    _t1613->gem_v_mangle = gem_v_mangle;
    *gem_v_compile_stmt = gem_make_fn(_anon_37, _t1613);
    struct _closure__anon_38 *_t1716 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t1716->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1716->gem_v_compile_expr = gem_v_compile_expr;
    _t1716->gem_v_compile_if_return = gem_v_compile_if_return;
    _t1716->gem_v_compile_match_return = gem_v_compile_match_return;
    _t1716->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1716->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1716->gem_v_compile_while = gem_v_compile_while;
    _t1716->gem_v_mangle = gem_v_mangle;
    _t1716->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_38, _t1716);
    struct _closure__anon_39 *_t1731 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t1731->gem_v_compile_expr = gem_v_compile_expr;
    _t1731->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_39, _t1731);
    struct _closure__anon_40 *_t1760 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t1760->gem_v_compile_expr = gem_v_compile_expr;
    _t1760->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1760->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_40, _t1760);
    struct _closure__anon_41 *_t1777 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t1777->gem_v_compile_expr = gem_v_compile_expr;
    _t1777->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_41, _t1777);
    struct _closure__anon_42 *_t1807 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t1807->gem_v_compile_expr = gem_v_compile_expr;
    _t1807->gem_v_compile_stmts = gem_v_compile_stmts;
    _t1807->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_42, _t1807);
    struct _closure__anon_43 *_t1851 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t1851->gem_v_compile_expr = gem_v_compile_expr;
    _t1851->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1851->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1851->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_43, _t1851);
    struct _closure__anon_44 *_t1895 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t1895->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_44, _t1895);
    struct _closure__anon_45 *_t1940 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t1940->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1940->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1940->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1940->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t1940->gem_v_mangle = gem_v_mangle;
    *gem_v_compile_fn = gem_make_fn(_anon_45, _t1940);
    struct _closure__anon_46 *_t2003 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t2003->gem_v_anon_name = gem_v_anon_name;
    _t2003->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2003->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2003->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2003->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2003->gem_v_functions = gem_v_functions;
    _t2003->gem_v_mangle = gem_v_mangle;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_46, _t2003);
    struct _closure__anon_47 *_t2048 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t2048->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2048->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t2048->gem_v_compile_fn = gem_v_compile_fn;
    _t2048->gem_v_compile_stmts = gem_v_compile_stmts;
    _t2048->gem_v_defined_fns = gem_v_defined_fns;
    _t2048->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t2048->gem_v_forward_decls = gem_v_forward_decls;
    _t2048->gem_v_functions = gem_v_functions;
    GemVal gem_v_compile = gem_make_fn(_anon_47, _t2048);
    GemVal _t2049 = gem_table_new();
    gem_table_set(_t2049, gem_string("compile"), gem_v_compile);
    return _t2049;
}

static GemVal gem_fn_split_string(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t2050 = gem_table_new();
    GemVal gem_v_result = _t2050;
    GemVal gem_v_current = gem_string("");
    {
        GemVal gem_v__for_i_62 = gem_int(0);
    GemVal _t2051[] = {gem_v_s};
        GemVal gem_v__for_limit_62 = gem_len_fn(NULL, _t2051, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
            GemVal gem_v_i = gem_v__for_i_62;
            gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_v_delim))) {
    GemVal _t2052[] = {gem_v_result, gem_v_current};
                (void)(gem_push_fn(NULL, _t2052, 2));
                gem_v_current = gem_string("");
            } else {
                gem_v_current = gem_add(gem_v_current, gem_table_get(gem_v_s, gem_v_i));
            }
        }
    }
    GemVal _t2053[] = {gem_v_result, gem_v_current};
    (void)(gem_push_fn(NULL, _t2053, 2));
    return gem_v_result;
}

static GemVal gem_fn_str_replace_all(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_result = gem_string("");
    GemVal gem_v_i = gem_int(0);
    GemVal _t2054[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t2054, 1);
    GemVal _t2055[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t2055, 1);
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
            GemVal gem_v_found = gem_bool(1);
            GemVal gem_v_j = gem_int(0);
            while (1) {
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
                    gem_v_found = gem_bool(0);
                    break;
                }
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
            if (gem_truthy(gem_v_found)) {
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    return gem_v_result;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t2056 = gem_table_new();
    GemVal gem_v_new_stmts = _t2056;
    GemVal gem_v_count = gem_int(0);
    {
    GemVal _t2057 = gem_v_ast;
        GemVal gem_v__for_items_1 = gem_table_get(_t2057, gem_string("stmts"));
        GemVal gem_v__for_i_1 = gem_int(0);
        while (1) {
            GemVal _t2058[] = {gem_v__for_items_1};
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t2058, 1)))) break;
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
    GemVal _t2059[] = {gem_v_stmt};
    GemVal _t2061;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2059, 1), gem_string("table")))) {
        _t2061 = gem_eq(gem_type_fn(NULL, _t2059, 1), gem_string("table"));
    } else {
        GemVal _t2060 = gem_v_stmt;
        _t2061 = gem_eq(gem_table_get(_t2060, gem_string("tag")), gem_string("load"));
    }
            if (gem_truthy(_t2061)) {
    GemVal _t2062 = gem_v_stmt;
                GemVal gem_v_path = gem_table_get(_t2062, gem_string("path"));
    GemVal _t2063[] = {gem_v_path};
    GemVal _t2071;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t2063, 1), gem_int(4)))) {
        _t2071 = gem_lt(gem_len_fn(NULL, _t2063, 1), gem_int(4));
    } else {
        GemVal _t2064[] = {gem_v_path};
        GemVal _t2066;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2064, 1), gem_int(4))), gem_string(".")))) {
                _t2066 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2064, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t2065[] = {gem_v_path};
                _t2066 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2065, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t2068;
        if (gem_truthy(_t2066)) {
                _t2068 = _t2066;
        } else {
                GemVal _t2067[] = {gem_v_path};
                _t2068 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2067, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t2070;
        if (gem_truthy(_t2068)) {
                _t2070 = _t2068;
        } else {
                GemVal _t2069[] = {gem_v_path};
                _t2070 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t2069, 1), gem_int(1))), gem_string("m"));
        }
        _t2071 = _t2070;
    }
                if (gem_truthy(_t2071)) {
                    gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
                }
    GemVal _t2072[] = {gem_v_base_dir, gem_v_path};
    GemVal _t2073[] = {gem_fn_gem_path_join(NULL, _t2072, 2)};
                GemVal gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t2073, 1);
                if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                    continue;
                }
                gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
    GemVal _t2074[] = {gem_v_full_path};
                GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2074, 1);
                if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
    GemVal _t2075[] = {gem_v_full_path};
    GemVal _t2076[] = {gem_add(gem_add(gem_string("cannot load '"), gem_to_string_fn(NULL, _t2075, 1)), gem_string("'"))};
                    (void)(gem_error_at_fn("compiler/main.gem", 39, _t2076, 1));
                }
    GemVal _t2077[] = {gem_v_source};
                GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t2077, 1);
    GemVal _t2078[] = {gem_v_full_path};
                GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t2078, 1);
    GemVal _t2079[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
                GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t2079, 3);
                {
    GemVal _t2080 = gem_v_resolved;
                    GemVal gem_v__for_items_2 = gem_table_get(_t2080, gem_string("stmts"));
                    GemVal gem_v__for_i_2 = gem_int(0);
                    while (1) {
                        GemVal _t2081[] = {gem_v__for_items_2};
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t2081, 1)))) break;
                        GemVal gem_v_rs = gem_table_get(gem_v__for_items_2, gem_v__for_i_2);
                        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
                        gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_rs);
                        gem_v_count = gem_add(gem_v_count, gem_int(1));
                    }
                }
            } else {
                gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_stmt);
                gem_v_count = gem_add(gem_v_count, gem_int(1));
            }
        }
    }
    GemVal _t2082[] = {gem_v_new_stmts};
    return gem_fn_make_program(NULL, _t2082, 1);
}

int main(void) {
    GemVal gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
    GemVal _t2083[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 62, _t2083, 1));
    }
    GemVal _t2084[] = {gem_int(1)};
    GemVal gem_v_src_path = gem_fn_gem_get_argv(NULL, _t2084, 1);
    GemVal _t2085[] = {gem_v_src_path};
    GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t2085, 1);
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
    GemVal _t2086[] = {gem_v_src_path};
    GemVal _t2087[] = {gem_add(gem_add(gem_string("cannot read '"), gem_to_string_fn(NULL, _t2086, 1)), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 68, _t2087, 1));
    }
    GemVal _t2088[] = {gem_v_source};
    GemVal gem_v_ast = gem_fn_parse_source(NULL, _t2088, 1);
    GemVal _t2089[] = {gem_v_src_path};
    GemVal gem_v_base_dir = gem_fn_gem_dirname(NULL, _t2089, 1);
    GemVal _t2090 = gem_table_new();
    GemVal gem_v_loaded = _t2090;
    GemVal _t2091[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    GemVal gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t2091, 3);
    GemVal _t2092[] = {gem_v_src_path};
    GemVal gem_v_cg = gem_fn_make_codegen(NULL, _t2092, 1);
    GemVal _t2093 = gem_v_cg;
    GemVal _t2094 = gem_table_get(_t2093, gem_string("compile"));
    GemVal _t2095[] = {gem_v_resolved_ast};
    GemVal gem_v_c_code = _t2094.fn(_t2094.env, _t2095, 1);
    GemVal _t2096[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t2096, 1));
    return 0;
}

